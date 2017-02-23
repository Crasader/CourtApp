//
//  SettingLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2017/01/09.
//
//

#include "SettingLayer.hpp"
#include "CourtManager.hpp"
#include "MemberManager.hpp"


USING_NS_CC;
using namespace cocostudio;

SettingLayer::SettingLayer()
:courtNumButton1(nullptr)
,courtNumButton2(nullptr)
,courtNumButton3(nullptr)
,courtNumButton4(nullptr)
,courtNumButton5(nullptr)
,courtNumButton6(nullptr)
,showWinNumButton(nullptr)
,hideWinNumButton(nullptr)
{
}

SettingLayer::~SettingLayer() {
}

#pragma mark - 初期化
SettingLayer *SettingLayer::create()
{
    SettingLayer *ret = new SettingLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool SettingLayer::init()
{
    std::string csbName = "SettingLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;

    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    courtNumButton1 = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonCourt1"));
    courtNumButton2 = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonCourt2"));
    courtNumButton3 = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonCourt3"));
    courtNumButton4 = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonCourt4"));
    courtNumButton5 = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonCourt5"));
    courtNumButton6 = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonCourt6"));
    showWinNumButton = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonShowWinNum"));
    hideWinNumButton = dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonHideWinNum"));
    
    this->addButtonEvent(courtNumButton1, ButtonTag::Court1);
    this->addButtonEvent(courtNumButton2, ButtonTag::Court2);
    this->addButtonEvent(courtNumButton3, ButtonTag::Court3);
    this->addButtonEvent(courtNumButton4, ButtonTag::Court4);
    this->addButtonEvent(courtNumButton5, ButtonTag::Court5);
    this->addButtonEvent(courtNumButton6, ButtonTag::Court6);
    this->addButtonEvent(showWinNumButton, ButtonTag::ShowWinNum);
    this->addButtonEvent(hideWinNumButton, ButtonTag::HideWinNum);
    this->addButtonEvent(dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonShowWinList")), ButtonTag::ShowWinList);
    this->addButtonEvent(dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonResetWinNum")), ButtonTag::ResetWinNum);
    this->addButtonEvent(dynamic_cast<ui::Button *>(scrollView->getChildByName("ButtonResetGameHistory")), ButtonTag::ResetHistory);
    
    this->updateCourtNumButton();
    this->updateWinNumButton();
    
    return true;
}


void SettingLayer::updateCourtNumButton()
{
    int courtNum = Manager::Court::getInstance()->getUseCourtNum();
    bool enableCourt1 = true;
    bool enableCourt2 = true;
    bool enableCourt3 = true;
    bool enableCourt4 = true;
    bool enableCourt5 = true;
    bool enableCourt6 = true;
    
    switch (courtNum) {
        case 1: enableCourt1 = false; break;
        case 2: enableCourt2 = false; break;
        case 3: enableCourt3 = false; break;
        case 4: enableCourt4 = false; break;
        case 5: enableCourt5 = false; break;
        case 6: enableCourt6 = false; break;
    }
    
    this->setButtonEnabled(courtNumButton1, enableCourt1);
    this->setButtonEnabled(courtNumButton2, enableCourt2);
    this->setButtonEnabled(courtNumButton3, enableCourt3);
    this->setButtonEnabled(courtNumButton4, enableCourt4);
    this->setButtonEnabled(courtNumButton5, enableCourt5);
    this->setButtonEnabled(courtNumButton6, enableCourt6);
}

void SettingLayer::updateWinNumButton()
{
    bool showWinNum = Manager::Court::getInstance()->shouldShowWinNum();
    this->setButtonEnabled(showWinNumButton, !showWinNum);
    this->setButtonEnabled(hideWinNumButton, showWinNum);
}

void SettingLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto courtManager  = Manager::Court::getInstance();
    auto memberManager = Manager::Member::getInstance();
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::Court1: courtManager->setUsecourtNum(1); break;
        case ButtonTag::Court2: courtManager->setUsecourtNum(2); break;
        case ButtonTag::Court3: courtManager->setUsecourtNum(3); break;
        case ButtonTag::Court4: courtManager->setUsecourtNum(4); break;
        case ButtonTag::Court5: courtManager->setUsecourtNum(5); break;
        case ButtonTag::Court6: courtManager->setUsecourtNum(6); break;
        case ButtonTag::ShowWinNum:  courtManager->setShouldShowWinNum(true);  break;
        case ButtonTag::HideWinNum:  courtManager->setShouldShowWinNum(false); break;
        case ButtonTag::ResetWinNum: this->showResetWinNumConfirm();                 break;
            
            
        case ButtonTag::ShowWinList: Kyarochon::Event::sendCustomEvent(EVENT_SHOW_WIN_LIST); return;
        case ButtonTag::ResetHistory: this->showResetHistoryConfirm(); return;
    }
    
    this->updateCourtNumButton();
    this->updateWinNumButton();
}

void SettingLayer::showResetWinNumConfirm()
{
    std::string text = "勝敗数をリセットします\nよろしいですか？";
    std::function<void()> yesCallback = [](){Manager::Member::getInstance()->resetAllGameResult();};
    std::function<void()> noCallback  = [](){};
    
    ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
    Kyarochon::Event::showConfirmView(info);
}

void SettingLayer::showResetHistoryConfirm()
{
    std::string text = "試合履歴を全てリセットします\nよろしいですか？";
    std::function<void()> yesCallback = [](){Manager::Court::getInstance()->resetAllHistory();};
    std::function<void()> noCallback  = [](){};
    
    ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
    Kyarochon::Event::showConfirmView(info);
}
