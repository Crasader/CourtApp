//
//  WinItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/02.
//
//

#include "WinItemNode.hpp"
USING_NS_CC;
using namespace cocostudio;

WinItemNode::WinItemNode()
{
}

WinItemNode::~WinItemNode() {
}



//CCBをつくって表示する


#pragma mark - 初期化
WinItemNode *WinItemNode::create(UserInfo *userInfo)
{
    WinItemNode *ret = new WinItemNode();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool WinItemNode::init(UserInfo *userInfo)
{
    std::string csbName = "WinItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->getChildByName<ui::Text *>("TextName")->setString(userInfo->name);
    panel->getChildByName<ui::Text *>("TextWinNum")->setString(StringUtils::toString(userInfo->getWinCount()));
    
    return true;
}