//
//  MemberLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/20.
//
//

#include "MemberLayer.hpp"
#include "EventUtil.h"
#include "LayoutUtil.h"
#include "MemberManager.hpp"

#include "MemberItemNode.hpp"
#include "MemberCategoryItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;


MemberLayer::MemberLayer()
:memberNumText(nullptr)
,tabNode(nullptr)
,searchDrawerLayer(nullptr)
,isShowingSearchDrawerLayer(false)
,isMovingDrawer(false)
{
    Kyarochon::Event::setEventListener(EVENT_UPDATE_MEMBER_LIST, CC_CALLBACK_1(MemberLayer::eventUpdateMemberList, this));
    Kyarochon::Event::setEventListener(EVENT_HIDE_DRAWER_LAYER, CC_CALLBACK_1(MemberLayer::eventHideDrawerLayer, this));
}

MemberLayer::~MemberLayer()
{
    Kyarochon::Event::removeEventListener(EVENT_UPDATE_MEMBER_LIST);
}


#pragma mark - 初期化
MemberLayer *MemberLayer::create()
{
    MemberLayer *ret = new MemberLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MemberLayer::init()
{
    std::string csbName = "MemberLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    // ボタン
    auto headerPanel = mainLayer->getChildByName<ui::Layout *>("HeaderPanel");
    this->memberNumText = headerPanel->getChildByName<ui::Text *>("TextMemberNum");
    this->addButtonEvent(headerPanel->getChildByName<ui::Button *>("ButtonSort"), ButtonTag::Sort);
    this->addButtonEvent(headerPanel->getChildByName<ui::Button *>("ButtonAddMember"), ButtonTag::AddMember);
    
    // 検索メニュー
    this->searchDrawerLayer = SearchDrawerLayer::create();
    searchDrawerLayer->setPosition(Vec2(-searchDrawerLayer->getContentSize().width / 2.0f, 0.0f));
    this->addChild(searchDrawerLayer);
    
    // Nodeを作成しておく
    memberItemNodeList.clear();
    auto memberList = Manager::Member::getInstance()->getMemberList();
    for (auto member : memberList)
    {
        auto memberItemNode = MemberItemNode::create(member);
        memberItemNodeList.pushBack(memberItemNode);
    }
    
    // 表示更新
    this->updateMemberList();

    return true;
}

// 表示されたとき
void MemberLayer::show()
{
    this->updateMemberList();
}

MemberItemNode *MemberLayer::getMemberItemNodeAt(int memberId)
{
    for (auto memberItemNode : memberItemNodeList)
    {
        if (memberItemNode->getMemberId() == memberId)
            return memberItemNode;
    }
    
    return nullptr;
}

void MemberLayer::updateMemberList()
{
    std::vector<std::pair<std::string, std::vector<int>>> categorizedMemberList = Manager::Member::getInstance()->getCategorizedMemberList();
    
    // タイトル更新
    auto allMemberList      = Manager::Member::getInstance()->getMemberList(false);
    auto filteredMemberList = Manager::Member::getInstance()->getMemberList(true);
    this->memberNumText->setString(StringUtils::format("[%d/%d]", (int)filteredMemberList.size(), (int)allMemberList.size()));
    
    // ScrollViewサイズ更新
    float contentHeight = 0.0f;
    float scrollHeight  = Kyarochon::Layout::getScrollHeight(HEADER_HEIGHT, TAB_HEIGHT, SCROLL_MARGIN);
    for (std::pair<std::string, std::vector<int>> categorizedPair : categorizedMemberList)
    {
        contentHeight += MEMBER_CATEGORY_ITEM_HEIGHT;
        contentHeight += ceil((float)(categorizedPair.second.size()) / 2.0f) * MEMBER_ITEM_HEIGHT;
    }
    
    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    if (scrollHeight > contentHeight)
    {
        scrollView->setContentSize(Size(SCROLL_WIDTH, contentHeight));
    }
    else
    {
        scrollView->setContentSize(Size(SCROLL_WIDTH, scrollHeight));
    }
    scrollView->setInnerContainerSize(Size(SCROLL_WIDTH, contentHeight));
    scrollView->removeAllChildren();
    
    CCLOG("contentHeight:%f, scrollHeight:%f", contentHeight, scrollHeight);
    // ScrollViewにNodeを追加
    float y = contentHeight + (DEFAULT_SCREEN_HEIGHT - Kyarochon::Layout::getScaledHeight());
    for (std::pair<std::string, std::vector<int>> categorizedPair : categorizedMemberList)
    {
        std::string categoryName = categorizedPair.first;
        std::vector<int> memberIdList = categorizedPair.second;
        
        // カテゴリ
        auto categoryItemNode = MemberCategoryItemNode::create(categoryName);
        categoryItemNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        y -= MEMBER_CATEGORY_ITEM_HEIGHT / 2.0f;
        categoryItemNode->setPosition(MEMBER_CATEGORY_ITEM_WIDTH / 2.0f, y);
        scrollView->addChild(categoryItemNode);

        // メンバーリスト
        y -= (MEMBER_CATEGORY_ITEM_HEIGHT + MEMBER_ITEM_HEIGHT) / 2.0f;
        for (int i = 0; i < memberIdList.size(); i++)
        {
            int memberId = memberIdList.at(i);
            if (i > 0 && i % 2 == 0) y -= MEMBER_ITEM_HEIGHT;
            
            // Vectorに格納されていない場合のみcreateする
            MemberItemNode *memberItemNode = this->getMemberItemNodeAt(memberId);
            if (memberItemNode)
            {
                memberItemNode->updateOptionText();
            }
            else
            {
                auto member = Manager::Member::getInstance()->getMemberAt(memberIdList.at(i));
                memberItemNode = MemberItemNode::create(member);
            }
            
            float x = ((float)(i % 2) + 0.5f) * MEMBER_ITEM_WIDTH;
            memberItemNode->setPosition(Vec2(x, y));
            scrollView->addChild(memberItemNode);
        }
        y -= MEMBER_ITEM_HEIGHT / 2.0f;
    }
}


void MemberLayer::pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    
    switch (tag) {
        case ButtonTag::Sort:
            this->moveSearchDrawerLayer();
            break;
        case ButtonTag::AddMember:
            Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_MEMBER_EDIT, -1);
            break;
    }
}


void MemberLayer::eventUpdateMemberList(cocos2d::EventCustom *event)
{
    this->updateMemberList();
}

void MemberLayer::eventHideDrawerLayer(cocos2d::EventCustom *event)
{
    this->moveSearchDrawerLayer();
}




void MemberLayer::moveSearchDrawerLayer()
{
    if (isMovingDrawer) return;
    isMovingDrawer = true;
    
    float moveWidth;
    if (this->isShowingSearchDrawerLayer)
    {
        moveWidth  = -this->searchDrawerLayer->getContentSize().width / 2.0f;
        this->searchDrawerLayer->hideLayer();
        this->tabNode->setTabEnabled(true);
    }
    else
    {
        moveWidth  = this->searchDrawerLayer->getContentSize().width / 2.0f;
        this->searchDrawerLayer->showLayer();
        this->tabNode->setTabEnabled(false);
    }
    
    this->isShowingSearchDrawerLayer = !this->isShowingSearchDrawerLayer;
    
    // 閉じた場合は表示更新
    if (!this->isShowingSearchDrawerLayer)
    {
        // 検索ワード設定
        Manager::Member::getInstance()->setSearchWord(searchDrawerLayer->getSearchText());
        // 表示更新
        Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_MEMBER_LIST);
    }

    
    auto endFunc = [this](){
        this->isMovingDrawer = false;
    };
    
    this->mainLayer->runAction(MoveBy::create(0.1f, Vec2(moveWidth, 0.0f)));
    this->tabNode->runAction(MoveBy::create(0.1f, Vec2(moveWidth, 0.0f)));
    searchDrawerLayer->runAction(Sequence::create(
                                                  MoveBy::create(0.1f, Vec2(moveWidth, 0.0f)),
                                                  CallFunc::create(endFunc),
                                                  nullptr
                                                  ));
}
