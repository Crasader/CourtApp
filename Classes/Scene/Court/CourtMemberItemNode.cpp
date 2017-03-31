//
//  CourtMemberItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/03.
//
//

#include "CourtMemberItemNode.hpp"
#include "CourtManager.hpp"

USING_NS_CC;
using namespace cocostudio;


CourtMemberItemNode::CourtMemberItemNode()
:userInfo(nullptr)
,courtId(-1)
,nameText(nullptr)
,levelText(nullptr)
{
}

CourtMemberItemNode::~CourtMemberItemNode()
{
    if (nameText)
    {
        nameText->stopAllActions();
    }
}


#pragma mark - 初期化
CourtMemberItemNode *CourtMemberItemNode::create(int courtId, UserInfo *userInfo, int num, bool shouldShowLevel)
{
    CourtMemberItemNode *ret = new CourtMemberItemNode();
    if (ret->init(courtId, userInfo, num, shouldShowLevel))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CourtMemberItemNode::init(int courtId, UserInfo *userInfo, int num, bool shouldShowLevel)
{
    std::string csbName = "CourtMemberItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->userInfo = userInfo;
    this->courtId  = courtId;
    
    // パネル
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->setSwallowTouches(false);
    
    // ボタン
    auto button = panel->getChildByName<ui::Button *>("ButtonTransparent");
    this->addButtonEvent(button, 0);
    
    // テキスト
    nameText = panel->getChildByName<ui::Text *>("TextNickname");
    nameText->setString(userInfo->getNickname());
    nameText->setColor(userInfo->gender == Gender::Male ? Color3B::BLUE : Color3B::RED);
    
    levelText = panel->getChildByName<ui::Text *>("TextLevel");
    if (shouldShowLevel)
    {
        nameText->setPositionY(40.0f);
        levelText->setString("(" + userInfo->getLevelText() + ")");
    }
    else
    {
        nameText->setPositionY(30.0f);
        levelText->setString("");
    }
    
    
    
    return true;
}



void CourtMemberItemNode::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    CsbLayerBase::pushedButton(pSender, type);
}

void CourtMemberItemNode::shortTappedButton(int tag)
{
    // 既に選択されていれば入れ替え
    Manager::Court::getInstance()->finishSelectReplaceMember(courtId, userInfo);
}

void CourtMemberItemNode::longTappedButton(int tag)
{
    // 選択したメンバーを登録
    Manager::Court::getInstance()->selectReplaceMember(courtId, userInfo);
    
    // 点滅開始
    auto action = Sequence::create(FadeOut::create(1.0f), FadeIn::create(1.0f), NULL);
    nameText->setTextColor(Color4B::RED);
    nameText->runAction(RepeatForever::create(action));
}


