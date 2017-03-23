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
:courtNumText(nullptr)
,courtNumSlider(nullptr)
,shuffleRandomPairCheckBox(nullptr)
,shuffleRandomPointCheckBox(nullptr)
,allocationAllCheckBox(nullptr)
,allocationByLevelCheckBox(nullptr)
,showWinNumCheckBox(nullptr)
,hideWinNumCheckBox(nullptr)
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
    
    // コート関連
    auto courtPanel = scrollView->cocos2d::Node::getChildByName<ui::Layout *>("CourtPanel");
    courtNumText = courtPanel->getChildByName<ui::Text *>("TextCourtNum");
    courtNumSlider = courtPanel->getChildByName<ui::Slider *>("SliderCourtNum");
    shuffleRandomPairCheckBox = courtPanel->getChildByName<ui::CheckBox *>("CheckBoxShuffleRadomPair");
    shuffleRandomPointCheckBox = courtPanel->getChildByName<ui::CheckBox *>("CheckBoxShuffleRandomPoint");
    allocationAllCheckBox = courtPanel->getChildByName<ui::CheckBox *>("CheckBoxAllocationAll");
    allocationByLevelCheckBox = courtPanel->getChildByName<ui::CheckBox *>("CheckBoxAllocationByCourt");
    
    this->addSliderEvent(courtNumSlider, SliderTag::CourtNum);
    this->addCheckBoxEvent(shuffleRandomPairCheckBox, CheckBoxTag::ShuffleRandomPair);
    this->addCheckBoxEvent(shuffleRandomPointCheckBox, CheckBoxTag::ShuffleRandomPoint);
    this->addCheckBoxEvent(allocationAllCheckBox, CheckBoxTag::AllocationAll);
    this->addCheckBoxEvent(allocationByLevelCheckBox, CheckBoxTag::AllocationByLevel);
    
    
    // 戦績関連
    auto historyPanel = scrollView->cocos2d::Node::getChildByName<ui::Layout *>("HistoryPanel");
    showWinNumCheckBox = historyPanel->getChildByName<ui::CheckBox *>("CheckBoxShowWinNum");
    hideWinNumCheckBox = historyPanel->getChildByName<ui::CheckBox *>("CheckBoxHideWinNum");
    
    this->addCheckBoxEvent(showWinNumCheckBox, CheckBoxTag::ShowWinNum);
    this->addCheckBoxEvent(hideWinNumCheckBox, CheckBoxTag::HideWinNum);
    this->addButtonEvent(dynamic_cast<ui::Button *>(historyPanel->getChildByName("ButtonShowWinList")), ButtonTag::ShowWinList);
    this->addButtonEvent(dynamic_cast<ui::Button *>(historyPanel->getChildByName("ButtonResetWinNum")), ButtonTag::ResetWinNum);
    this->addButtonEvent(dynamic_cast<ui::Button *>(historyPanel->getChildByName("ButtonResetGameHistory")), ButtonTag::ResetHistory);
    
    // 表示更新
    this->updateCourtNumSlider();
    this->updateCheckBox();
    
    return true;
}


void SettingLayer::updateCourtNumSlider()
{
    int courtNum = Manager::Court::getInstance()->getUseCourtNum();
    this->setSliderValue(this->courtNumSlider, MIN_COURT_NUM, MAX_COURT_NUM, courtNum);
    this->courtNumText->setString(StringUtils::toString(courtNum));
}

void SettingLayer::updateCheckBox()
{
    // シャッフルタイプ
    auto shuffleType = Manager::Member::getInstance()->getShuffleType();
    if (shuffleType == ShuffleType::RandomPair)
    {
        this->shuffleRandomPairCheckBox->setSelected(true);
        this->shuffleRandomPointCheckBox->setSelected(false);
    }
    else
    {
        this->shuffleRandomPairCheckBox->setSelected(false);
        this->shuffleRandomPointCheckBox->setSelected(true);
    }
    
    // 振り分けタイプ
    auto allocationType = Manager::Member::getInstance()->getAllocationType();
    if (allocationType == AllocationType::All)
    {
        this->allocationAllCheckBox->setSelected(true);
        this->allocationByLevelCheckBox->setSelected(false);
    }
    else
    {
        this->allocationAllCheckBox->setSelected(false);
        this->allocationByLevelCheckBox->setSelected(true);
    }
    
    // 勝敗数表示
    bool showWinNum = Manager::Court::getInstance()->shouldShowWinNum();
    this->showWinNumCheckBox->setSelected(showWinNum);
    this->hideWinNumCheckBox->setSelected(!showWinNum);
    
    
}

void SettingLayer::changedSliderValue(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
    auto slider = dynamic_cast<ui::Slider *>(pSender);
    SliderTag tag = (SliderTag)slider->getTag();
    switch (tag) {
        case SliderTag::CourtNum:
            Manager::Court::getInstance()->setUseCourtNum(this->getSliderValueInt(slider, MIN_COURT_NUM, MAX_COURT_NUM));
            break;
    }
    this->updateCourtNumSlider();
}

void SettingLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::ResetWinNum: this->showResetWinNumConfirm(); break;
        case ButtonTag::ShowWinList: Kyarochon::Event::sendCustomEvent(EVENT_SHOW_WIN_LIST); return;
        case ButtonTag::ResetHistory: this->showResetHistoryConfirm(); return;
    }
}

void SettingLayer::pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
    if (type == cocos2d::ui::CheckBox::EventType::UNSELECTED) return;
    
    auto courtManager  = Manager::Court::getInstance();
    auto memberManager = Manager::Member::getInstance();

    auto checkBox = dynamic_cast<ui::CheckBox *>(pSender);
    CheckBoxTag tag = (CheckBoxTag)checkBox->getTag();
    switch (tag) {
        case CheckBoxTag::ShuffleRandomPair: memberManager->setShuffleType(ShuffleType::RandomPair); break;
        case CheckBoxTag::ShuffleRandomPoint: memberManager->setShuffleType(ShuffleType::RandomPoint); break;
        case CheckBoxTag::AllocationAll: memberManager->setAllocationType(AllocationType::All); break;
        case CheckBoxTag::AllocationByLevel: memberManager->setAllocationType(AllocationType::ByLevel); break;
        case CheckBoxTag::ShowWinNum: courtManager->setShouldShowWinNum(true); break;
        case CheckBoxTag::HideWinNum: courtManager->setShouldShowWinNum(false); break;
    }
    this->updateCheckBox();
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
