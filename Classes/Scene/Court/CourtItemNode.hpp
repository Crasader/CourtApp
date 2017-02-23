//
//  CourtItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/03.
//
//

#ifndef CourtItemNode_hpp
#define CourtItemNode_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"
#include "CourtInfo.hpp"
#include <time.h>

class CourtItemNode : public CsbLayerBase
{
    enum ButtonTag
    {
        WinLeft,
        WinRight,
    };
    
public:
    static CourtItemNode *create(CourtInfo *courtInfo);
    
    
protected:
    CourtItemNode();
    virtual ~CourtItemNode();
    bool init(CourtInfo *courtInfo);
    
    CourtInfo *courtInfo;
    
    cocos2d::ui::Button *buttonWinRight;
    cocos2d::ui::Button *buttonWinLeft;
    
    cocos2d::ui::ImageView *imageWinRight;
    cocos2d::ui::ImageView *imageWinLeft;
    cocos2d::ui::ImageView *imageLoseRight;
    cocos2d::ui::ImageView *imageLoseLeft;
    
    void updateWinImage();
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* CourtItemNode_hpp */
