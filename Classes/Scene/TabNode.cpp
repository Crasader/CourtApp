//
//  TabNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/29.
//
//

#include "TabNode.hpp"


USING_NS_CC;
using namespace cocostudio;


TabNode::TabNode()
:currentTabType(TabType::None)
{
}

TabNode::~TabNode() {
}


#pragma mark - 初期化
TabNode *TabNode::create() {
    TabNode *ret = new TabNode();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TabNode::init()
{
    std::string csbName = "TabNode.csb";
    bool isFullSize = false;
    if ( !CsbLayerBase::init(csbName, isFullSize) ) return false;
    
    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonMember"), (int)TabType::Member);
    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonParticipant"), (int)TabType::Participant);
    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonCourt"), (int)TabType::Court);
    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonSetting"), (int)TabType::Setting);
    
    
    this->setPosition(Vec2(TAB_WIDTH / 2.0f, TAB_SPACE + TAB_HEIGHT / 2.0f));
    
    return true;
}



void TabNode::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    // TODO: SE鳴らす
    
    auto tabType = (TabType)dynamic_cast<ui::Button *>(pSender)->getTag();
    this->changeSelectedTab(tabType);
}



void TabNode::changeSelectedTab(TabType tabType)
{
    // 同じタブを選択されている場合は何もしない
    if (tabType == currentTabType) return;
    
    // タブの有効／無効を更新
    this->setTabButtonEnabled(currentTabType, true);
    this->setTabButtonEnabled(tabType, false);
    
    Kyarochon::Event::sendCustomEventWithData(EVENT_SELECT_TAB, (int)tabType);
    currentTabType = tabType;
}


void TabNode::setTabButtonEnabled(TabType tabType, bool enabled)
{
    std::string buttonName = "";
    switch (tabType) {
        case TabType::Member:       buttonName = "ButtonMember";       break;
        case TabType::Participant:  buttonName = "ButtonParticipant";  break;
        case TabType::Court:        buttonName = "ButtonCourt";        break;
        case TabType::Setting:        buttonName = "ButtonSetting";        break;
        default: break;
    }
    if (buttonName.empty()) return;
    
    auto button = mainLayer->getChildByName<ui::Button *>(buttonName);
    button->setEnabled(enabled);
    button->setHighlighted(!enabled);
    if (enabled)
        button->getTitleRenderer()->setTextColor(Color4B::BLACK);
    else
        button->getTitleRenderer()->setTextColor(Color4B::WHITE);
    
}


