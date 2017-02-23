//
//  WinListLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/02.
//
//

#include "WinListLayer.hpp"
#include "MemberManager.hpp"
#include "WinItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;

WinListLayer::WinListLayer()
{
}

WinListLayer::~WinListLayer() {
}





#pragma mark - 初期化
WinListLayer *WinListLayer::create()
{
    WinListLayer *ret = new WinListLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool WinListLayer::init()
{
    std::string csbName = "WinListLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    // ボタンイベント
    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonClose"), ButtonTag::Close);
    
    // メンバー一覧を勝数順にソート
    auto entryMemberList = Manager::Member::getInstance()->getEntryMemberList();
    std::sort(entryMemberList.begin(), entryMemberList.end(), [](UserInfo *a, UserInfo *b){
        return a->getWinCount() > b->getWinCount();
    });
    
    // スクロールビュー初期化
    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    auto contentsHeight = ceil((double)entryMemberList.size() / 2.0 ) * WIN_ITEM_HEIGHT;
    if (contentsHeight < WIN_SCROLL_HEIGHT)
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, contentsHeight));
    }
    scrollView->setInnerContainerSize(Size(scrollView->getInnerContainerSize().width, contentsHeight));
    
    
    int i = 0;
    for (auto member : entryMemberList)
    {
        auto winItemNode = WinItemNode::create(member);
        winItemNode->setPosition(Vec2(WIN_ITEM_WIDTH * (0.5 + float(i % 2)),
                                      contentsHeight - WIN_ITEM_HEIGHT * (0.5 + float(i / 2)) )
                                 );
        scrollView->addChild(winItemNode);
        i++;
    }
    
    return true;
}




void WinListLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
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