//
//  AlertLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/04.
//
//

#include "AlertLayer.hpp"

USING_NS_CC;
using namespace cocostudio;

AlertLayer::AlertLayer()
{
}

AlertLayer::~AlertLayer()
{
}





#pragma mark - 初期化
AlertLayer *AlertLayer::create(std::string text)
{
    AlertLayer *ret = new AlertLayer();
    if (ret->init(text)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool AlertLayer::init(std::string text)
{
    std::string csbName = "AlertLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    auto window = panel->getChildByName<ui::Layout *>("PanelWindow");
    window->getChildByName<ui::Text *>("TextDescription")->setString(text);
    this->addButtonEvent(window->getChildByName<ui::Button *>("ButtonClose"), ButtonTag::Close);
    
    return true;
}


void AlertLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::Close:      this->close();              return;
    }
}


void AlertLayer::close()
{
    this->removeFromParent();
}