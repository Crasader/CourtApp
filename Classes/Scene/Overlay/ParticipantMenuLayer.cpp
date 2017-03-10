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
:buttonNormal(nullptr)
,buttonEntryGame(nullptr)
,buttonRest(nullptr)
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
    auto panelWindow = panel->getChildByName<ui::Layout *>("PanelWindow");
    
    std::string title = (userInfoList.size() == 1) ? userInfoList.front()->name : "まとめて編集";
    panelWindow->getChildByName<ui::Text *>("TextTitle")->setString(title);
    
    buttonNormal = panelWindow->getChildByName<ui::Button *>("ButtonNormal");
    buttonEntryGame = panelWindow->getChildByName<ui::Button *>("ButtonEntryGame");
    buttonRest = panelWindow->getChildByName<ui::Button *>("ButtonRest");
    
    this->addButtonEvent(buttonNormal,    ButtonTag::EntryModeNormal);
    this->addButtonEvent(buttonEntryGame, ButtonTag::EntryModeEntryGame);
    this->addButtonEvent(buttonRest,      ButtonTag::EntryModeRest);
    this->addButtonEvent(panelWindow->getChildByName<ui::Button *>("ButtonPlusGameNum"),  ButtonTag::PlusGameNum);
    this->addButtonEvent(panelWindow->getChildByName<ui::Button *>("ButtonMinusGameNum"), ButtonTag::MinusGameNum);
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
    
    this->updateEntryButtons();
    
    return true;
}


void ParticipantMenuLayer::updateEntryButtons()
{
    bool normalEnabled = true;
    bool entryGameEnabled =true;
    bool restEnabled = true;
    
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
                normalEnabled = false;
                break;
            case EntryMode::EntryGame:
                entryGameEnabled = false;
                break;
            case EntryMode::Rest:
                restEnabled = false;
                break;
        }
    }
    
    this->setButtonEnabled(buttonNormal, normalEnabled);
    this->setButtonEnabled(buttonEntryGame, entryGameEnabled);
    this->setButtonEnabled(buttonRest, restEnabled);
    
    
}

void ParticipantMenuLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    
    
    for (auto userInfo : userInfoList)
    {
        switch (tag) {
            case PlusGameNum:
                userInfo->increaseGameCount();
                break;
            case MinusGameNum:
                userInfo->decreaseGameCount();
                break;
            case EntryModeNormal:
                userInfo->entryMode = EntryMode::Normal;
                break;
            case EntryModeEntryGame:
                userInfo->entryMode = EntryMode::EntryGame;
                break;
            case EntryModeRest:
                userInfo->entryMode = EntryMode::Rest;
                break;
            case ShowHistory:
                Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_GAME_HISTORY, userInfo->id);
                // breakなし
            case Close:
                this->close();
                return;
        }
        
        this->updateEntryButtons();
        Kyarochon::Event::sendCustomEventWithData(EVENT_UPDATE_PARTICIPANT, userInfo->id);
    }

}

void ParticipantMenuLayer::close()
{
    // 閉じる
    this->removeFromParent();
}

