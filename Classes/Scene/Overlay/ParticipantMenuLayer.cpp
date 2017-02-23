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
ParticipantMenuLayer *ParticipantMenuLayer::create(UserInfo *userInfo)
{
    ParticipantMenuLayer *ret = new ParticipantMenuLayer();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ParticipantMenuLayer::init(UserInfo *userInfo)
{
    this->userInfo =  userInfo;
    
    std::string csbName = "ParticipantMenuLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->getChildByName<ui::Text *>("TextTitle")->setString(userInfo->name);
    
    buttonNormal    = panel->getChildByName<ui::Button *>("ButtonNormal");
    buttonEntryGame = panel->getChildByName<ui::Button *>("ButtonEntryGame");
    buttonRest      = panel->getChildByName<ui::Button *>("ButtonRest");
    
    this->addButtonEvent(buttonNormal,    ButtonTag::EntryModeNormal);
    this->addButtonEvent(buttonEntryGame, ButtonTag::EntryModeEntryGame);
    this->addButtonEvent(buttonRest,      ButtonTag::EntryModeRest);
    this->addButtonEvent(panel->getChildByName<ui::Button *>("ButtonPlusGameNum"),  ButtonTag::PlusGameNum);
    this->addButtonEvent(panel->getChildByName<ui::Button *>("ButtonMinusGameNum"), ButtonTag::MinusGameNum);
    this->addButtonEvent(panel->getChildByName<ui::Button *>("ButtonClose"),        ButtonTag::Close);
    this->addButtonEvent(panel->getChildByName<ui::Button *>("ButtonShowGameHistory"), ShowHistory);
    
    this->updateEntryButtons();
    
    return true;
}


void ParticipantMenuLayer::updateEntryButtons()
{
    bool normalEnabled = true;
    bool entryGameEnabled =true;
    bool restEnabled = true;
    
    switch (userInfo->entryMode) {
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
    
    this->setButtonEnabled(buttonNormal, normalEnabled);
    this->setButtonEnabled(buttonEntryGame, entryGameEnabled);
    this->setButtonEnabled(buttonRest, restEnabled);
    
    
}

void ParticipantMenuLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    
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

void ParticipantMenuLayer::close()
{
    // 閉じる
    this->removeFromParent();
}

