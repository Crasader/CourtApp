//
//  MainScene.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/20.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "CsbLayerBase.hpp"
#include "cocos2d.h"
#include "TabNode.hpp"
#include "MemberLayer.hpp"

class MainScene : public CsbLayerBase
{
public:
    enum MainOrder
    {
        Layer   = 100,
        Tab     = 200,
        Overlay = 300,
        Alert   = 500,
    };
    
protected:
    
    MainScene();
    virtual ~MainScene();
    
    // Event
    void eventSelectTab(cocos2d::EventCustom *event);
    void eventShowMemberMenu(cocos2d::EventCustom *event);
    void eventShowGameHistory(cocos2d::EventCustom *event);
    void eventShowWinList(cocos2d::EventCustom *event);
    void eventShowMemberEdit(cocos2d::EventCustom *event);
    void eventShowAlertView(cocos2d::EventCustom *event);
    void eventShowConfirmView(cocos2d::EventCustom *event);
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);
    
    TabNode *tabNode;
    MemberLayer *memberLayer;
    cocos2d::Layer *selectedLayer;
};



#endif /* MainScene_hpp */
