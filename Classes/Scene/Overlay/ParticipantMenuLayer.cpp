//
//  ParticipantMenuLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/28.
//
//

#include "ParticipantMenuLayer.hpp"
#include "MemberManager.hpp"

USING_NS_CC;
using namespace cocostudio;

ParticipantMenuLayer::ParticipantMenuLayer()
:checkBoxNormal(nullptr)
,checkBoxEntryGame(nullptr)
,checkBoxRest(nullptr)
{
}

ParticipantMenuLayer::~ParticipantMenuLayer() {
}


#pragma mark - 初期化
ParticipantMenuLayer *ParticipantMenuLayer::create(std::vector<UserInfo *> userInfoList)
{
    ParticipantMenuLayer *ret = new ParticipantMenuLayer();
    if (ret->init(userInfoList)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ParticipantMenuLayer::init(std::vector<UserInfo *> userInfoList)
{
    this->userInfoList = userInfoList;
    
    std::string csbName = "ParticipantMenuLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
    this->addButtonEvent(panel->getChildByName<ui::Button *>("ButtonTransparent"),  ButtonTag::Close);

    auto panelWindow = panel->getChildByName<ui::Layout *>("PanelWindow");
    
    std::string title = (userInfoList.size() == 1) ? userInfoList.front()->name : "まとめて編集";
    panelWindow->getChildByName<ui::Text *>("TextTitle")->setString(title);
    
    checkBoxNormal = panelWindow->getChildByName<ui::CheckBox *>("CheckBoxNormal");
    checkBoxEntryGame = panelWindow->getChildByName<ui::CheckBox *>("CheckBoxEntryGame");
    checkBoxRest = panelWindow->getChildByName<ui::CheckBox *>("CheckBoxRest");
    this->addCheckBoxEvent(checkBoxNormal,    CheckBoxTag::EntryModeNormal);
    this->addCheckBoxEvent(checkBoxEntryGame, CheckBoxTag::EntryModeEntryGame);
    this->addCheckBoxEvent(checkBoxRest,      CheckBoxTag::EntryModeRest);
    this->addButtonEvent(panelWindow->getChildByName<ui::Button *>("ButtonPlusGameNum"),  ButtonTag::PlusGameNum);
    this->addButtonEvent(panelWindow->getChildByName<ui::Button *>("ButtonMinusGameNum"), ButtonTag::MinusGameNum);
    this->addButtonEvent(panelWindow->getChildByName<ui::Button *>("ButtonCancelEntry"), ButtonTag::CancelEntry);
    this->addButtonEvent(panelWindow->getChildByName<ui::Button *>("ButtonClose"),        ButtonTag::Close);
    
    auto historyButton = panelWindow->getChildByName<ui::Button *>("ButtonShowGameHistory");
    if (userInfoList.size() == 1)
    {
        historyButton->setVisible(true);
        this->addButtonEvent(historyButton, ShowHistory);
    }
    else
    {
        historyButton->setVisible(false);
    }
    
    this->updateEntryCheckBoxes();
    
    return true;
}


void ParticipantMenuLayer::updateEntryCheckBoxes()
{
    bool normalChecked = false;
    bool entryGameChecked = false;
    bool restChecked = false;
    
    bool isSameMode = true;
    for (int i = 0; i < userInfoList.size() - 1; i++)
    {
        if (userInfoList[i]->entryMode != userInfoList[i + 1]->entryMode)
        {
            isSameMode = false;
            break;
        }
    }
    
    if (isSameMode)
    {
        switch (userInfoList.front()->entryMode) {
            case EntryMode::Normal:
                normalChecked = true;
                break;
            case EntryMode::EntryGame:
                entryGameChecked = true;
                break;
            case EntryMode::Rest:
                restChecked = true;
                break;
        }
    }
    
    checkBoxNormal->setSelected(normalChecked);
    checkBoxEntryGame->setSelected(entryGameChecked);
    checkBoxRest->setSelected(restChecked);
}

void ParticipantMenuLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    
    switch (tag) {
        case PlusGameNum:
            this->increaseGamecount();
            break;
        case MinusGameNum:
            this->decreaseGameCount();
            break;
        case CancelEntry:
            this->leaveGame();
            break;
        case ShowHistory:
            Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_GAME_HISTORY, userInfoList.front()->id);
            // breakなし
        case Close:
            this->close();
            return;
    }
}

void ParticipantMenuLayer::increaseGamecount()
{
    for (auto userInfo :userInfoList)
    {
        userInfo->increaseGameCount();
        Kyarochon::Event::sendCustomEventWithData(EVENT_UPDATE_PARTICIPANT, userInfo->id);
    }
}

void ParticipantMenuLayer::decreaseGameCount()
{
    for (auto userInfo :userInfoList)
    {
        userInfo->decreaseGameCount();
        Kyarochon::Event::sendCustomEventWithData(EVENT_UPDATE_PARTICIPANT, userInfo->id);
    }
}

void ParticipantMenuLayer::leaveGame()
{
    std::string text = (userInfoList.size() == 1) ? userInfoList.front()->name + "さん" : "選択されたメンバー";
    text += "を\n参加者から外します\nよろしいですか？";
    std::function<void()> yesCallback = [this](){
        for (auto userInfo : userInfoList)
        {
            userInfo->leaveGame();
        }
        
        // 表示更新
        Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_PARTICIPANT_LIST);
        Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_MEMBER_LIST);
        
        // 閉じる
        this->close();
    };
    std::function<void()> noCallback  = [](){};
    
    ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
    Kyarochon::Event::showConfirmView(info);
}

void ParticipantMenuLayer::pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
    auto checkBox = dynamic_cast<ui::CheckBox *>(pSender);
    CheckBoxTag tag = (CheckBoxTag)checkBox->getTag();

    for (auto userInfo : userInfoList)
    {
        switch (tag) {
            case EntryModeNormal:
                userInfo->entryMode = EntryMode::Normal;
                break;
            case EntryModeEntryGame:
                userInfo->entryMode = EntryMode::EntryGame;
                break;
            case EntryModeRest:
                userInfo->entryMode = EntryMode::Rest;
                break;
        }
        Kyarochon::Event::sendCustomEventWithData(EVENT_UPDATE_PARTICIPANT, userInfo->id);
    }
    
    this->updateEntryCheckBoxes();
}

void ParticipantMenuLayer::close()
{
    // 閉じる
    this->removeFromParent();
}

