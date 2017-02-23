//
//  MainScene.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/20.
//
//

#include "MainScene.hpp"
#include "MemberManager.hpp"

#include "MemberLayer.hpp"
#include "ParticipantLayer.hpp"
#include "CourtLayer.hpp"
#include "SettingLayer.hpp"

#include "Tabnode.hpp"
#include "ParticipantMenuLayer.hpp"
#include "GameHistoryLayer.hpp"
#include "WinListLayer.hpp"
#include "MemberEditLayer.hpp"
#include "AlertLayer.hpp"
#include "ConfirmLayer.hpp"

USING_NS_CC;


MainScene::MainScene()
:tabNode(nullptr)
,selectedLayer(nullptr)
{
    // イベント登録
    Kyarochon::Event::setEventListener(EVENT_SELECT_TAB, CC_CALLBACK_1(MainScene::eventSelectTab, this));
    Kyarochon::Event::setEventListener(EVENT_SHOW_PARTICIPANT_MENU, CC_CALLBACK_1(MainScene::eventShowMemberMenu, this));
    Kyarochon::Event::setEventListener(EVENT_SHOW_GAME_HISTORY, CC_CALLBACK_1(MainScene::eventShowGameHistory, this));
    Kyarochon::Event::setEventListener(EVENT_SHOW_WIN_LIST, CC_CALLBACK_1(MainScene::eventShowWinList, this));
    Kyarochon::Event::setEventListener(EVENT_SHOW_MEMBER_EDIT, CC_CALLBACK_1(MainScene::eventShowMemberEdit, this));
    Kyarochon::Event::setEventListener(EVENT_SHOW_ALERT_VIEW, CC_CALLBACK_1(MainScene::eventShowAlertView, this));
    Kyarochon::Event::setEventListener(EVENT_SHOW_CONFIRM_VIEW, CC_CALLBACK_1(MainScene::eventShowConfirmView, this));
}

MainScene::~MainScene()
{
    // イベント削除
    Kyarochon::Event::removeEventListener(EVENT_SELECT_TAB);
    Kyarochon::Event::removeEventListener(EVENT_SHOW_PARTICIPANT_MENU);
    Kyarochon::Event::removeEventListener(EVENT_SHOW_GAME_HISTORY);
    Kyarochon::Event::removeEventListener(EVENT_SHOW_WIN_LIST);
    Kyarochon::Event::removeEventListener(EVENT_SHOW_MEMBER_EDIT);
    Kyarochon::Event::removeEventListener(EVENT_SHOW_ALERT_VIEW);
    Kyarochon::Event::removeEventListener(EVENT_SHOW_CONFIRM_VIEW);
}

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MainScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    Manager::Member::getInstance()->getCategorizedMemberListLastTime();
    
    
    std::string csbName = "MainScene.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    auto size = Director::getInstance()->getWinSize();

    // タブ
    tabNode = TabNode::create();
    this->addChild(tabNode, MainOrder::Tab);
    tabNode->changeSelectedTab(TabType::Member);
    
    // メンバーレイヤだけ作成しておく
    memberLayer = MemberLayer::create();
    memberLayer->setTabNode(tabNode);
    memberLayer->retain();  // removeしたときに開放されないように

    return true;
}


#pragma mark - イベント
void MainScene::eventSelectTab(cocos2d::EventCustom *event)
{
    if (selectedLayer)
    {
        selectedLayer->removeFromParent();
        selectedLayer = nullptr;
    }
    
    TabType type = (TabType)Kyarochon::Event::getEventDataInt(event);
    switch (type) {
        case TabType::Member:
            selectedLayer = this->memberLayer;
            CCLOG("TabType::Member referenceCount:%d", selectedLayer->getReferenceCount());
            break;
        case TabType::Participant:
            selectedLayer = ParticipantLayer::create();
            CCLOG("TabType::Participant referenceCount:%d", selectedLayer->getReferenceCount());
            break;
        case TabType::Court:
            selectedLayer = CourtLayer::create();
            CCLOG("TabType::Court referenceCount:%d", selectedLayer->getReferenceCount());
            break;
        case TabType::Setting:
            selectedLayer = SettingLayer::create();
            CCLOG("TabType::Setting referenceCount:%d", selectedLayer->getReferenceCount());
        default:
            break;
    }
    
    
    if (selectedLayer)
    {
        this->addChild(selectedLayer, MainOrder::Layer);
    }
}




void MainScene::eventShowMemberMenu(cocos2d::EventCustom *event)
{
    int memberId = Kyarochon::Event::getEventDataInt(event);
    auto userInfo = Manager::Member::getInstance()->getMemberAt(memberId);
    
    auto participantMenuLayer = ParticipantMenuLayer::create(userInfo);
    this->addChild(participantMenuLayer, MainOrder::Overlay);
}

void MainScene::eventShowGameHistory(cocos2d::EventCustom *event)
{
    int memberId = Kyarochon::Event::getEventDataInt(event);
    auto userInfo = Manager::Member::getInstance()->getMemberAt(memberId);
    
    auto gameHistoryLayer = GameHistoryLayer::create(userInfo);
    this->addChild(gameHistoryLayer, MainOrder::Overlay);
}

void MainScene::eventShowWinList(cocos2d::EventCustom *event)
{
    auto winListLayer = WinListLayer::create();
    this->addChild(winListLayer, MainOrder::Overlay);
}

void MainScene::eventShowMemberEdit(cocos2d::EventCustom *event)
{
    int memberId = Kyarochon::Event::getEventDataInt(event);
    UserInfo *userInfo = (memberId > 0) ? Manager::Member::getInstance()->getMemberAt(memberId) : nullptr;
    
    auto memberEditLayer = MemberEditLayer::create(userInfo);
    this->addChild(memberEditLayer, MainOrder::Overlay);
}

void MainScene::eventShowAlertView(cocos2d::EventCustom *event)
{
    std::string text = Kyarochon::Event::getEventDataString(event);

    auto alertLayer = AlertLayer::create(text);
    this->addChild(alertLayer, MainOrder::Alert);
}

void MainScene::eventShowConfirmView(cocos2d::EventCustom *event)
{
    ConfirmInfo *info = Kyarochon::Event::getEventDataConfimInfo(event);
    
    auto confirmLayer = ConfirmLayer::create(info);
    this->addChild(confirmLayer, MainOrder::Alert);
}
