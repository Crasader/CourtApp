//
//  GameHistoryLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/28.
//
//

#ifndef GameHistoryLayer_hpp
#define GameHistoryLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class GameHistoryLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Close,
    };
    
public:
    static GameHistoryLayer *create(UserInfo *userInfo);
    
protected:
    GameHistoryLayer();
    virtual ~GameHistoryLayer();
    bool init(UserInfo *userInfo);
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};


#endif /* GameHistoryLayer_hpp */
