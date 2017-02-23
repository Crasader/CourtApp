//
//  ConfirmLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/08.
//
//

#ifndef ConfirmLayer_hpp
#define ConfirmLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class ConfirmLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Yes,
        No,
    };
    
public:
    static ConfirmLayer *create(ConfirmInfo *info);
    
protected:
    ConfirmLayer();
    virtual ~ConfirmLayer();
    bool init(ConfirmInfo *info);
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    ConfirmInfo *confirmInfo;
    
    void pushedYes();
    void pushedNo();
};

#endif /* ConfirmLayer_hpp */
