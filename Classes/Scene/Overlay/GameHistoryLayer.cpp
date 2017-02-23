//
//  GameHistoryLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/28.
//
//

#include "GameHistoryLayer.hpp"
#include "MemberManager.hpp"
#include "GameHistoryItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;

GameHistoryLayer::GameHistoryLayer()
{
}

GameHistoryLayer::~GameHistoryLayer() {
}





#pragma mark - 初期化
GameHistoryLayer *GameHistoryLayer::create(UserInfo *userInfo)
{
    GameHistoryLayer *ret = new GameHistoryLayer();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GameHistoryLayer::init(UserInfo *userInfo)
{
    std::string csbName = "GameHistoryLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    
    mainLayer->getChildByName<ui::Text *>("TextTitle")->setString(userInfo->name + "の試合履歴");
    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonClose"), ButtonTag::Close);

    // スクロールビュー初期化
    auto entryMemberList = Manager::Member::getInstance()->getEntryMemberList();
    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    
    auto contentsHeight = ceil((double)entryMemberList.size() / 2.0 ) * HISTORY_ITEM_HEIGHT;
    if (contentsHeight < HISTORY_SCROLL_HEIGHT)
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, contentsHeight));
    }
    scrollView->setInnerContainerSize(Size(scrollView->getInnerContainerSize().width, contentsHeight));
    
    
    int i = 0;
    for (auto member : entryMemberList)
    {
        auto gameHistoryItemNode = GameHistoryItemNode::create(member, userInfo->id);
        gameHistoryItemNode->setPosition(Vec2(HISTORY_ITEM_WIDTH * (0.5 + float(i % 2)),
                                              contentsHeight - HISTORY_ITEM_HEIGHT * (0.5 + float(i / 2))
                                              )
                                         );
        scrollView->addChild(gameHistoryItemNode);
        i++;
    }
    
    return true;
}




void GameHistoryLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::Close:
            this->removeFromParent();
            break;
    }
    
    
}
