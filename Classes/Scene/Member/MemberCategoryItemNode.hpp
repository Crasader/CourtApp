//
//  MemberCategoryItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2017/01/17.
//
//

#ifndef MemberCategoryItemNode_hpp
#define MemberCategoryItemNode_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class MemberCategoryItemNode : public CsbLayerBase
{
    
public:
    static MemberCategoryItemNode *create(std::string title);
    
    
protected:
    MemberCategoryItemNode();
    virtual ~MemberCategoryItemNode();
    bool init(std::string title);
/*
    //
    int memberId;
    
    // スプライト
    cocos2d::Sprite *checkImage;
    
    // ボタン
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void shortTappedButton(int tag);
    virtual void longTappedButton(int tag);
 */
    
};
#endif /* MemberCategoryItemNode_hpp */
