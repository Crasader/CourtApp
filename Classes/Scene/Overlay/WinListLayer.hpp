//
//  WinListLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/02.
//
//

#ifndef WinListLayer_hpp
#define WinListLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class WinListLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Close,
    };
    
public:
    static WinListLayer *create();
    
protected:
    WinListLayer();
    virtual ~WinListLayer();
    bool init();
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};


#endif /* WinListLayer_hpp */
