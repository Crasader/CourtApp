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
    enum ButtonTag
    {
        Court1,
        Court2,
        Court3,
        Court4,
        Court5,
        Court6,
        ShowWinNum,
        HideWinNum,
        ShowWinList,
        ResetWinNum,
        ResetHistory,
    };

    
public:
    static SettingLayer *create();
    
protected:
    SettingLayer();
    virtual ~SettingLayer();
    bool init() override;
    
    // ボタンイベント
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type) override;
    
    // スクロールビューを初期化
//    virtual void initScrollView(int itemNum);

private:
    cocos2d::ui::Button *courtNumButton1;
    cocos2d::ui::Button *courtNumButton2;
    cocos2d::ui::Button *courtNumButton3;
    cocos2d::ui::Button *courtNumButton4;
    cocos2d::ui::Button *courtNumButton5;
    cocos2d::ui::Button *courtNumButton6;
    cocos2d::ui::Button *showWinNumButton;
    cocos2d::ui::Button *hideWinNumButton;
    
    void updateCourtNumButton();
    void updateWinNumButton();
    
    void showResetWinNumConfirm();
    void showResetHistoryConfirm();
};

#endif /* SettingLayer_hpp */
