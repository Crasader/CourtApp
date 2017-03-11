//
//  ParticipantLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/29.
//
//

#include "ParticipantLayer.hpp"
#include "MemberManager.hpp"
#include "ParticipantItemNode.hpp"
#include "MemberCategoryItemNode.hpp"
#include "EventUtil.h"
#include "LayoutUtil.h"

USING_NS_CC;
using namespace cocostudio;

ParticipantLayer::ParticipantLayer()
:scrollView(nullptr)
{
    Kyarochon::Event::setEventListener(EVENT_UPDATE_PARTICIPANT_LIST, CC_CALLBACK_1(ParticipantLayer::eventUpdateParticipantList, this));
}

ParticipantLayer::~ParticipantLayer()
{
    Kyarochon::Event::removeEventListener(EVENT_UPDATE_PARTICIPANT_LIST);
}


#pragma mark - 初期化
ParticipantLayer *ParticipantLayer::create()
{
    ParticipantLayer *ret = new ParticipantLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ParticipantLayer::init()
{
    std::string csbName = "ParticipantLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    

    // ボタン
    auto headerPanel = mainLayer->getChildByName<ui::Layout *>("HeaderPanel");
    this->addButtonEvent(headerPanel->getChildByName<ui::Button *>("ButtonSetting"), ButtonTag::Setting);

    // 表示更新
    scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    this->updateParticipantList();
    
    return true;
}



void ParticipantLayer::pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    
    switch (tag) {
        case ButtonTag::Setting:
        {
            std::vector<int> selectedIdList;
            auto entryMemberList = Manager::Member::getInstance()->getEntryMemberList();
            for (auto entryMember : entryMemberList)
            {
                if (entryMember->checked)
                    selectedIdList.push_back(entryMember->id);
            }
            
            if (selectedIdList.empty())
                Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_ALERT_VIEW, "編集したい参加者を\n1人以上チェックしてください");
            else
                Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_SOME_PARTICIPANTS_MENU, selectedIdList);
            
            break;
        }
    }
}

// 表示更新
void ParticipantLayer::updateParticipantList()
{    
    auto entryListPerLevel = Manager::Member::getInstance()->getEntryMemberListPerLevel();
    auto entryList = Manager::Member::getInstance()->getEntryMemberList();
    
    // スクロールビューサイズ更新
/*
    float scrollHeight = (float)entryList.size() * PARTICIPANT_ITEM_HEIGHT;
    if (scrollView->getContentSize().height > scrollHeight)
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, scrollHeight));
    }
    scrollView->setInnerContainerSize(Size(600.0f, scrollHeight));
    scrollView->removeAllChildren();
*/
    
    // ScrollViewサイズ更新
    float contentHeight = 0.0f;
    float scrollHeight  = Kyarochon::Layout::getScrollHeight(HEADER_HEIGHT, TAB_HEIGHT, SCROLL_MARGIN);
    for (std::pair<std::string, std::vector<UserInfo *>> entryMemberListPair : entryListPerLevel)
    {
        contentHeight += PARTICIPANT_CATEGORY_ITEM_HEIGHT;
        contentHeight += ceil((float)(entryMemberListPair.second.size()) / 2.0f) * PARTICIPANT_ITEM_HEIGHT;
    }
    
    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    if (scrollHeight > contentHeight)
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, contentHeight));
    }
    else
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, scrollHeight));
    }
    scrollView->setInnerContainerSize(Size(SCROLL_WIDTH, contentHeight));
    scrollView->removeAllChildren();
    
    // ScrollViewにNodeを追加
    float y = contentHeight + (DEFAULT_SCREEN_HEIGHT - Kyarochon::Layout::getScaledHeight());
    for (std::pair<std::string, std::vector<UserInfo *>> entryMemberListPair : entryListPerLevel)
    {
        std::string categoryName = entryMemberListPair.first;
        std::vector<UserInfo *> memberList = entryMemberListPair.second;
        
        // カテゴリ
        auto categoryItemNode = MemberCategoryItemNode::create(categoryName);
        categoryItemNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        y -= MEMBER_CATEGORY_ITEM_HEIGHT / 2.0f;
        categoryItemNode->setPosition(MEMBER_CATEGORY_ITEM_WIDTH / 2.0f, y);
        scrollView->addChild(categoryItemNode);
        
        // メンバーリスト
        y -= (MEMBER_CATEGORY_ITEM_HEIGHT + PARTICIPANT_ITEM_HEIGHT) / 2.0f;
        for (int i = 0; i < memberList.size(); i++)
        {
            UserInfo *member = memberList.at(i);
            if (i > 0 && i % 2 == 0) y -= PARTICIPANT_ITEM_HEIGHT;
            
            // Vectorに格納されていない場合のみcreateする
            auto participantItemNode = ParticipantItemNode::create(member);
            float x = ((float)(i % 2) + 0.5f) * PARTICIPANT_ITEM_WIDTH;
            participantItemNode->setPosition(Vec2(x, y));
            scrollView->addChild(participantItemNode);
        }
        y -= PARTICIPANT_ITEM_HEIGHT / 2.0f;
    }
    
    // 人数関連
    int entryNum = (int)entryList.size();
    mainLayer->getChildByName<ui::Text *>("TextWarning")->setVisible(entryNum == 0);
    
    auto headerPanel = mainLayer->getChildByName<ui::Layout *>("HeaderPanel");
    headerPanel->getChildByName<ui::Text *>("TextEntryNum")->setString(StringUtils::format("(%d名)", entryNum));
}

void ParticipantLayer::eventUpdateParticipantList(cocos2d::EventCustom *event)
{
    this->updateParticipantList();
}

