//
//  AlertLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/04.
//
//

#ifndef AlertLayer_hpp
#define AlertLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class AlertLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Close,
    };
    
public:
    static AlertLayer *create(std::string text);
    
protected:
    AlertLayer();
    virtual ~AlertLayer();
    bool init(std::string text);
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    void close();
};


#endif /* AlertLayer_hpp */
