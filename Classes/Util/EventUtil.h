//
//  EventUtil.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/30.
//
//

#ifndef EventUtil_h
#define EventUtil_h

#include "cocos2d.h"
#include "ConfirmInfo.hpp"
USING_NS_CC;

#define EVENT_SELECT_TAB            "EventTypeSelectTab"
// 情報更新
#define EVENT_UPDATE_MEMBER_LIST      "EventTypeUpdateMemberList"
#define EVENT_UPDATE_PARTICIPANT_LIST "EventTypeUpdateParticipantList"  // 全体
#define EVENT_UPDATE_PARTICIPANT      "EventTypeUpdateParticipant"      // 個別
#define EVENT_UPDATE_COURT_LIST       "EventTypeUpdateCourtList"
#define EVENT_UPDATE_SHOW_WIN_NUM     "EventTypeShowWinNum"
// オーバーレイ表示
#define EVENT_SHOW_PARTICIPANT_MENU "EventTypeShowParticipantMenu"
#define EVENT_SHOW_GAME_HISTORY     "EventTypeShowGameHistory"
#define EVENT_SHOW_WIN_LIST         "EventTypeShowWinList"
#define EVENT_SHOW_MEMBER_EDIT      "EventTypeShowMemberEdit"
// アラート表示
#define EVENT_SHOW_ALERT_VIEW       "EventTypeShowAlertView"
#define EVENT_SHOW_CONFIRM_VIEW     "EventTypeShowConfirmView"

namespace Kyarochon {
namespace Event {
        
    // --- 単純なイベント送信 ---
    static void sendCustomEvent(std::string eventName) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            auto event = EventCustom(eventName);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    
    // --- データ付きイベント送信 ---
    // int
    static void sendCustomEventWithData(std::string eventName, int data) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            auto event = EventCustom(eventName);
            auto value = Value(data);
            event.setUserData(&value);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    // double
    static void sendCustomEventWithData(std::string eventName, double data) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            auto event = EventCustom(eventName);
            auto value = Value(data);
            event.setUserData(&value);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    // bool
    static void sendCustomEventWithData(std::string eventName, bool data) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            auto event = EventCustom(eventName);
            auto value = Value(data);
            event.setUserData(&value);
            
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    // std::string
    static void sendCustomEventWithData(std::string eventName, std::string data) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            auto event = EventCustom(eventName);
            auto value = Value(data);
            event.setUserData(&value);
            
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    // Vec2
    static void sendCustomEventWithData(std::string eventName, cocos2d::Vec2 data) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            std::map<std::string, float> mapData;
            mapData["x"] = data.x;
            mapData["y"] = data.y;
            auto event = EventCustom(eventName);
            event.setUserData(&mapData);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    // ConfirmInfo(独自クラス)
    static void sendCustomEventWithData(std::string eventName, ConfirmInfo *info) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            auto event = EventCustom(eventName);
            event.setUserData(info);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        });
    }
    
    // --- データの取得 ---
    // int
    static int getEventDataInt(EventCustom *event) {
        auto value = (Value *)event->getUserData();
        return value->asInt();
    }
    // float
    static float getEventDataFloat(EventCustom *event) {
        auto value = (Value *)event->getUserData();
        return value->asFloat();
    }
    // double
    static double getEventDataDouble(EventCustom *event) {
        auto value = (Value *)event->getUserData();
        return value->asDouble();
    }
    // bool
    static bool getEventDataBool(EventCustom *event) {
        auto value = (Value *)event->getUserData();
        return value->asBool();
    }
    // std::string
    static std::string getEventDataString(EventCustom *event) {
        auto value = (Value *)event->getUserData();
        return value->asString();
    }
    // Vec2
    static cocos2d::Vec2 getEventDataVec2(EventCustom *event) {
        auto value = static_cast<std::map<std::string, float> *>(event->getUserData());
        float x = value->at("x");
        float y = value->at("y");
        return cocos2d::Vec2(x, y);
    }
    static ConfirmInfo *getEventDataConfimInfo(EventCustom *event) {
        auto value = (ConfirmInfo *)event->getUserData();
        return value;
    }
    
    
    
    // --- イベントの登録 ---
    static void setEventListener(std::string eventName, const std::function<void(EventCustom *)> callback)
    {
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(eventName, callback);
    }
    
    static void removeEventListener(std::string eventName)
    {
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(eventName);
    }
    
    
    // --- アラート表示 ---
    static void showAlertView(std::string text)
    {
        sendCustomEventWithData(EVENT_SHOW_ALERT_VIEW, text);
    }
    static void showConfirmView(ConfirmInfo *info)
    {
        sendCustomEventWithData(EVENT_SHOW_CONFIRM_VIEW, info);
    }

    
}   // end namespace Event
}   // end namespace Kyarochon



#endif /* EventUtil_h */
