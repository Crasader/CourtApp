//
//  ParticipantItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/03.
//
//

#include "ParticipantItemNode.hpp"
#include "EventUtil.h"
#include "CourtManager.hpp"

USING_NS_CC;
using namespace cocostudio;


ParticipantItemNode::ParticipantItemNode()
:userInfo(nullptr)
,checkImage(nullptr)
,textNickname(nullptr)
,textGameCount(nullptr)
,entrySprite(nullptr)
,restSprite(nullptr)
,textPoint(nullptr)
,textWinCount(nullptr)
{
    Kyarochon::Event::setEventListener(EVENT_UPDATE_PARTICIPANT, CC_CALLBACK_1(ParticipantItemNode::eventUpdateText, this));
    Kyarochon::Event::setEventListener(EVENT_UPDATE_SHOW_WIN_NUM, CC_CALLBACK_1(ParticipantItemNode::eventUpdateShowWinNum, this));
}

ParticipantItemNode::~ParticipantItemNode()
{
    Kyarochon::Event::removeEventListener(EVENT_UPDATE_PARTICIPANT);
    Kyarochon::Event::removeEventListener(EVENT_UPDATE_SHOW_WIN_NUM);
}


#pragma mark - 初期化
ParticipantItemNode *ParticipantItemNode::create(UserInfo *userInfo)
{
    ParticipantItemNode *ret = new ParticipantItemNode();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ParticipantItemNode::init(UserInfo *userInfo)
{
    std::string csbName = "ParticipantItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->userInfo = userInfo;
    
    // パネル
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
//    auto bgName = (userInfo->gender == Gender::Male) ? "bg_blue.png" : "bg_red.png";
//    panel->setBackGroundImage(bgName);
    panel->setSwallowTouches(false);
    
    // テキスト
    textNickname = panel->getChildByName<ui::Text *>("TextNickname");
    textNickname->setString(userInfo->getNickname());
    
    textGameCount = panel->getChildByName<ui::Text *>("TextGameCount");
    this->updateGameCountText();
    
    entrySprite = panel->getChildByName<Sprite *>("SpriteParticipant");
    restSprite = panel->getChildByName<Sprite *>("SpriteRest");
    this->updateEntryModeSprite();
    
    textPoint = panel->getChildByName<ui::Text *>("TextPoint");
    textPoint->setVisible(false);
    
    textWinCount = panel->getChildByName<ui::Text *>("TextWinCount");
    this->updateWinNumText();

    // ボタン
    auto button = panel->getChildByName<ui::Button *>("Button");
    button->setSwallowTouches(false);
    this->addButtonEvent(button, 0);
    
    // チェックボックス
    checkImage = panel->getChildByName<Sprite *>("CheckImage");
    checkImage->setVisible(userInfo->checked);
    
    
    return true;
}



void ParticipantItemNode::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    CsbLayerBase::pushedButton(pSender, type);    
}

void ParticipantItemNode::shortTappedButton(int tag)
{
    userInfo->checked = !userInfo->checked;
    checkImage->setVisible(userInfo->checked);
}

void ParticipantItemNode::longTappedButton(int tag)
{
    Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_PARTICIPANT_MENU, userInfo->id);
}


#pragma mark - ********** テキストの更新 **********

void ParticipantItemNode::updateEntryModeSprite()
{
    // 参加
    switch(userInfo->entryMode)
    {
        case EntryMode::Normal:
            entrySprite->setVisible(false);
            restSprite->setVisible(false);
            break;
        case EntryMode::EntryGame:
            entrySprite->setVisible(true);
            restSprite->setVisible(false);
            break;
        case EntryMode::Rest:
            entrySprite->setVisible(false);
            restSprite->setVisible(true);
            break;
    }
}

void ParticipantItemNode::updateGameCountText()
{
    textGameCount->setString(StringUtils::format("参加：%d回", userInfo->gameCount));
}

void ParticipantItemNode::updateWinNumText()
{
    if (!textWinCount) return;
    textWinCount->setString(StringUtils::format("win:%d", userInfo->getWinCount()));
    textWinCount->setVisible(Manager::Court::getInstance()->shouldShowWinNum());
}



#pragma mark - ********** イベント **********

void ParticipantItemNode::eventUpdateText(EventCustom *event)
{
    int memberId = Kyarochon::Event::getEventDataInt(event);
    if (!userInfo || userInfo->id != memberId) return;
    
    this->updateEntryModeSprite();
    this->updateGameCountText();
}

void ParticipantItemNode::eventUpdateShowWinNum(EventCustom *event)
{
    if (userInfo)
    {
        this->updateWinNumText();
    }
}





