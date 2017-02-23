//
//  ConfirmLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/08.
//
//

#include "ConfirmLayer.hpp"

USING_NS_CC;
using namespace cocostudio;

ConfirmLayer::ConfirmLayer()
{
}

ConfirmLayer::~ConfirmLayer()
{
}





#pragma mark - 初期化
ConfirmLayer *ConfirmLayer::create(ConfirmInfo *info)
{
    ConfirmLayer *ret = new ConfirmLayer();
    if (ret->init(info)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ConfirmLayer::init(ConfirmInfo *info)
{
    std::string csbName = "ConfirmLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->confirmInfo = info;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    auto window = panel->getChildByName<ui::Layout *>("PanelWindow");
    window->getChildByName<ui::Text *>("TextDescription")->setString(confirmInfo->text);
    this->addButtonEvent(window->getChildByName<ui::Button *>("ButtonYes"), ButtonTag::Yes);
    this->addButtonEvent(window->getChildByName<ui::Button *>("ButtonNo"), ButtonTag::No);
    
    return true;
}


void ConfirmLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::Yes:    this->pushedYes();  return;
        case ButtonTag::No:     this->pushedNo();   return;
    }
}


void ConfirmLayer::pushedYes()
{
    if (confirmInfo->yesCallback) confirmInfo->yesCallback();
    this->removeFromParent();
}

void ConfirmLayer::pushedNo()
{
    if (confirmInfo->noCallback) confirmInfo->noCallback();
    this->removeFromParent();
}