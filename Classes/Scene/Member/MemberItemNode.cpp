//
//  MemberItemNode.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/02.
//
//

#include "MemberItemNode.hpp"
#include "MemberManager.hpp"

USING_NS_CC;
using namespace cocostudio;


MemberItemNode::MemberItemNode()
:checkImage(nullptr)
{
}

MemberItemNode::~MemberItemNode() {
}


#pragma mark - 初期化
MemberItemNode *MemberItemNode::create(UserInfo *userInfo)
{
    MemberItemNode *ret = new MemberItemNode();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MemberItemNode::init(UserInfo *userInfo)
{
    std::string csbName = "MemberItemNode.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->userInfo = userInfo;
    
    // パネル
    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
//    auto bgName = (userInfo->gender == Gender::Male) ? "bg_blue.png" : "bg_red.png";
//    panel->setBackGroundImage(bgName);
    panel->setSwallowTouches(false);
    
    // テキスト
    panel->getChildByName<ui::Text *>("TextNickname")->setString(userInfo->getNickname());
//    panel->getChildByName<ui::Text *>("TextName")->setString("氏名：" + userInfo->name);
    
    
    
    // ボタン
    auto button = panel->getChildByName<ui::Button *>("Button");
    button->setSwallowTouches(false);
    this->addButtonEvent(button, 0);
    
    // チェックボックス
    checkImage = panel->getChildByName<Sprite *>("CheckImage");
    checkImage->setVisible(userInfo->isEntry);

    
    // 画像
    auto imageGender = panel->getChildByName<ui::ImageView *>("ImageGender");
    if (userInfo->gender == Gender::Male)
    {
        imageGender->loadTexture("ico_mark_man_big.png");
    }
    else
    {
        imageGender->loadTexture("ico_mark_woman_big.png");
    }
    
    this->updateOptionText();
    
    return true;
}

void MemberItemNode::updateOptionText()
{
    std::string optionText = "";
    auto sortType = Manager::Member::getInstance()->getSortType();
    switch (sortType)
    {
        case SortType::Syllabary:   optionText = userInfo->nameFurigana; break;
        case SortType::Gender:      optionText = "氏名："  + userInfo->name; break;
        case SortType::Level:       optionText = "氏名："  + userInfo->name; break;
        case SortType::Count:       optionText = "氏名："  + userInfo->name;  break;
        case SortType::LastDate:    optionText = "参加：" + userInfo->getLastDate(); break;
        default: break;
    }

    auto panel =  mainLayer->getChildByName<ui::Layout *>("Panel");
    panel->getChildByName<ui::Text *>("TextOption")->setString(optionText);
    
    this->checkImage->setVisible(userInfo->isEntry);
}



void MemberItemNode::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    CsbLayerBase::pushedButton(pSender, type);
}

void MemberItemNode::shortTappedButton(int tag)
{
    this->checkImage->setVisible(!this->checkImage->isVisible());
    Manager::Member::getInstance()->updateIsEntry(this->userInfo->id, this->checkImage->isVisible());
}

void MemberItemNode::longTappedButton(int tag)
{
    Kyarochon::Event::sendCustomEventWithData(EVENT_SHOW_MEMBER_EDIT, this->userInfo->id);
}
