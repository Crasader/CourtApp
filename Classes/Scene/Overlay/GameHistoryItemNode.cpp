//
//  GameHistoryItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/28.
//
//

#include "GameHistoryItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;

GameHistoryItemNode::GameHistoryItemNode()
{
}

GameHistoryItemNode::~GameHistoryItemNode() {
}



//CCBをつくって表示する


#pragma mark - 初期化
GameHistoryItemNode *GameHistoryItemNode::create(UserInfo *userInfo, int memberId)
{
    GameHistoryItemNode *ret = new GameHistoryItemNode();
    if (ret->init(userInfo, memberId)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GameHistoryItemNode::init(UserInfo *userInfo, int memberId)
{
    std::string csbName = "GameHistoryItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->getChildByName<ui::Text *>("TextName")->setString(userInfo->name);
    
    std::string pairText = (userInfo->id == memberId) ? "-" : StringUtils::toString(userInfo->pairCountArray[memberId]);
    std::string fightText = (userInfo->id == memberId) ? "-" : StringUtils::toString(userInfo->fightCountArray[memberId]);
    
    panel->getChildByName<ui::Text *>("TextPairNum")->setString(pairText);
    panel->getChildByName<ui::Text *>("TextFightNum")->setString(fightText);
    
    return true;
}
