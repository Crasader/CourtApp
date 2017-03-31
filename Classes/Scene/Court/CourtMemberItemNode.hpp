//
//  CourtMemberItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/03.
//
//

#ifndef CourtMemberItemNode_hpp
#define CourtMemberItemNode_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"
#include <time.h>

class CourtMemberItemNode : public CsbLayerBase
{
    
public:
    static CourtMemberItemNode *create(int courtId, UserInfo *userInfo, int num, bool shouldShowLevel);
    
protected:
    CourtMemberItemNode();
    virtual ~CourtMemberItemNode();
    bool init(int courtId, UserInfo *userInfo, int num, bool shouldShowLevel);
    

private:
    UserInfo *userInfo;
    int courtId;
    cocos2d::ui::Text *nameText;
    cocos2d::ui::Text *levelText;
    
protected:
    // ボタン
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void shortTappedButton(int tag);
    virtual void longTappedButton(int tag);    
};

#endif /* CourtMemberItemNode_hpp */
