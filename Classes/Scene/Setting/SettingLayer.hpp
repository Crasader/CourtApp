//
//  SettingLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2017/01/09.
//
//

#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include "CsbLayerBase.hpp"

class SettingLayer : public CsbLayerBase
{
    enum SliderTag
    {
        CourtNum,
    };
    
    enum ButtonTag
    {
        ShowWinList,
        ResetWinNum,
        ResetHistory,
    };

    enum CheckBoxTag
    {
        ShuffleRandomPair,
        ShuffleRandomPoint,
        AllocationAll,
        AllocationByLevel,
        ShowWinNum,
        HideWinNum,
    };
    
    
    
public:
    static SettingLayer *create();
    
protected:
    SettingLayer();
    virtual ~SettingLayer();
    bool init() override;
    
    // UIイベント
    virtual void changedSliderValue(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type) override;
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type) override;
    virtual void pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type) override;
    
    
    // スクロールビューを初期化
//    virtual void initScrollView(int itemNum);

private:
    
    // コート関連
    cocos2d::ui::Text *courtNumText;
    cocos2d::ui::Slider *courtNumSlider;
    cocos2d::ui::CheckBox *shuffleRandomPairCheckBox;
    cocos2d::ui::CheckBox *shuffleRandomPointCheckBox;
    cocos2d::ui::CheckBox *allocationAllCheckBox;
    cocos2d::ui::CheckBox *allocationByLevelCheckBox;
    
    // 戦績関連
    cocos2d::ui::CheckBox *showWinNumCheckBox;
    cocos2d::ui::CheckBox *hideWinNumCheckBox;
    
    void updateCourtNumSlider();
    void updateCheckBox();
    
    void showResetWinNumConfirm();
    void showResetHistoryConfirm();
};

#endif /* SettingLayer_hpp */
