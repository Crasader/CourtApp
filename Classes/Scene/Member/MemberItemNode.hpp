//
//  MemberItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/02.
//
//

#ifndef MemberItemNode_hpp
#define MemberItemNode_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"
#include <time.h>

class MemberItemNode : public CsbLayerBase
{
    
public:
    static MemberItemNode *create(UserInfo *userInfo);
    int getMemberId(){return this->userInfo->id;};
    void updateOptionText();
    
protected:
    MemberItemNode();
    virtual ~MemberItemNode();
    bool init(UserInfo *userInfo);
    
    UserInfo *userInfo;
    
    // スプライト
    cocos2d::Sprite *checkImage;
    
    // ボタン
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void shortTappedButton(int tag);
    virtual void longTappedButton(int tag);
    
};

#endif /* MemberItemNode_hpp */
