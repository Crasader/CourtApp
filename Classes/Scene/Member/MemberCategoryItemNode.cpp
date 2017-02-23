//
//  MemberCategoryItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2017/01/17.
//
//

#include "MemberCategoryItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;


MemberCategoryItemNode::MemberCategoryItemNode()
{
}

MemberCategoryItemNode::~MemberCategoryItemNode() {
}


#pragma mark - 初期化
MemberCategoryItemNode *MemberCategoryItemNode::create(std::string title)
{
    MemberCategoryItemNode *ret = new MemberCategoryItemNode();
    if (ret->init(title)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MemberCategoryItemNode::init(std::string title)
{
    std::string csbName = "MemberCategoryItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    // パネル
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->getChildByName<ui::Text *>("TextCategory")->setString(title);
    
    return true;
}
