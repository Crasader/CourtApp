//
//  CourtItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/03.
//
//

#include "CourtItemNode.hpp"
#include "CourtMemberItemNode.hpp"
#include "MemberManager.hpp"
#include "CourtManager.hpp"
#include "LayoutUtil.h"

USING_NS_CC;
using namespace cocostudio;


CourtItemNode::CourtItemNode()
:courtInfo(nullptr)
,buttonWinRight(nullptr)
,buttonWinLeft(nullptr)
,imageWinRight(nullptr)
,imageWinLeft(nullptr)
,imageLoseRight(nullptr)
,imageLoseLeft(nullptr)
{
}

CourtItemNode::~CourtItemNode() {
}


#pragma mark - 初期化
CourtItemNode *CourtItemNode::create(CourtInfo *courtInfo)
{
    CourtItemNode *ret = new CourtItemNode();
    if (ret->init(courtInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CourtItemNode::init(CourtInfo *courtInfo)
{
    std::string csbName = "CourtItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->courtInfo = courtInfo;
    
    // コート数
    mainLayer->getChildByName<ui::Text *>("TextCourtNum")->setString(StringUtils::format("No.%d", courtInfo->courtNum + 1));
    
    // パネル
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->setSwallowTouches(false);
    
    // メンバー
    int num = 0;
    
    float serverTitleHeight = 40.0f;
    for (auto member : courtInfo->memberList)
    {
        auto memberItemNode = CourtMemberItemNode::create(courtInfo->id, member, num);
        float x = ((float)(num / 2) + 0.5f) * COURT_MEMBER_ITEM_WIDTH;
        float y = panel->getContentSize().height - 40.0f - (floorf(num % 2) + 0.5f ) * COURT_MEMBER_ITEM_HEIGHT + (DEFAULT_SCREEN_HEIGHT - Kyarochon::Layout::getScaledHeight());
        
        memberItemNode->setPosition(Vec2(x, y));
        panel->addChild(memberItemNode);
        num++;
    }
    
    // Win/Lose
    imageWinRight = panel->getChildByName<ui::ImageView *>("ImageWinRight");
    imageLoseRight = panel->getChildByName<ui::ImageView *>("ImageLoseRight");
    buttonWinRight = panel->getChildByName<ui::Button *>("ButtonWinRight");
    this->addButtonEvent(buttonWinRight, WinRight);

    imageWinLeft  = panel->getChildByName<ui::ImageView *>("ImageWinLeft");
    imageLoseLeft = panel->getChildByName<ui::ImageView *>("ImageLoseLeft");
    buttonWinLeft  = panel->getChildByName<ui::Button *>("ButtonWinLeft");
    this->addButtonEvent(buttonWinLeft, WinLeft);
    
    this->updateWinImage();    
    
    return true;
}

void CourtItemNode::updateWinImage()
{
    switch (courtInfo->winState) {
        case WinState::Unknown:
            imageWinRight->setVisible(false);
            imageWinLeft->setVisible(false);
            imageLoseRight->setVisible(false);
            imageLoseLeft->setVisible(false);
            buttonWinRight->setVisible(true);
            buttonWinLeft->setVisible(true);
            break;
        case WinState::WinLeft:
            imageWinRight->setVisible(false);
            imageWinLeft->setVisible(true);
            imageLoseRight->setVisible(true);
            imageLoseLeft->setVisible(false);
            buttonWinRight->setVisible(true);
            buttonWinLeft->setVisible(false);
            break;
        case WinState::WinRight:
            imageWinRight->setVisible(true);
            imageWinLeft->setVisible(false);
            imageLoseRight->setVisible(false);
            imageLoseLeft->setVisible(true);
            buttonWinRight->setVisible(false);
            buttonWinLeft->setVisible(true);
            break;
    }
}

void CourtItemNode::pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto courtManager = Manager::Court::getInstance();
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    
    switch (tag) {
        case WinRight: courtManager->updateGameResult(courtInfo->id, WinState::WinRight); break;
        case WinLeft:  courtManager->updateGameResult(courtInfo->id, WinState::WinLeft);  break;
    }
    this->updateWinImage();
    
    
}


