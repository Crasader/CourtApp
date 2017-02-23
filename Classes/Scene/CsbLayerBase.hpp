//
//  CsbLayerBase.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/20.
//
//

#ifndef CsbLayerBase_hpp
#define CsbLayerBase_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "EventUtil.h"
#include "LayoutDefine.h"
#include "CommonDefine.h"
#include "ConfirmInfo.hpp"
#include "NativeBridge.h"

class CsbLayerBase : public cocos2d::Layer
{
protected:
    // メインレイヤ
    cocos2d::Layer *mainLayer;

    // コンストラクタ／デストラクタ
    CsbLayerBase();
    virtual ~CsbLayerBase();
    
    // 初期化
    virtual bool init(std::string csbName, bool isFullSize = true);


public:
//    static cocos2d::Scene* createScene();

    // Button
    virtual void addButtonEvent(cocos2d::ui::Button *button, int tag);
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void longTappedButton(int tag){};
    virtual void shortTappedButton(int tag){};
    virtual void setButtonEnabled(cocos2d::ui::Button *button, bool enabled);
    
    
    // CheckBox
    virtual void addCheckBoxEvent(cocos2d::ui::CheckBox *checkBox, int tag);
    virtual void pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type);
    
    // EditBox作成
    virtual cocos2d::ui::EditBox *makeEditBox(cocos2d::Vec2 pos, std::string placeHolder, cocos2d::Node *parent = nullptr);

protected:
    //  ロングタップ用
    cocos2d::Vec2 touchStartPos;
    clock_t touchStartTime;

};




#endif /* CsbLayerBase_hpp */
