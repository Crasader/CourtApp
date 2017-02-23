//
//  ParticipantItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/03.
//
//

#ifndef ParticipantItemNode_hpp
#define ParticipantItemNode_hpp


#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"
#include <time.h>

class ParticipantItemNode : public CsbLayerBase
{
    
public:
    static ParticipantItemNode *create(UserInfo *userInfo);
    
    
protected:
    ParticipantItemNode();
    virtual ~ParticipantItemNode();
    bool init(UserInfo *userInfo);
    
    UserInfo *userInfo;
    
    // テキスト
    cocos2d::ui::Text *textNickname;
    cocos2d::ui::Text *textGameCount;
    cocos2d::ui::Text *textPoint;
    cocos2d::ui::Text *textWinCount;
    cocos2d::Sprite *entrySprite;
    cocos2d::Sprite *restSprite;
    
    
    
    // スプライト
    cocos2d::Sprite *checkImage;
    
    // ボタンイベント
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void shortTappedButton(int tag);
    virtual void longTappedButton(int tag);
    
    
    // テキスト更新
    void updateEntryModeSprite();
    void updateGameCountText();
    void updateWinNumText();
    
    // イベント
    void eventUpdateText(cocos2d::EventCustom *event);
    void eventUpdateShowWinNum(cocos2d::EventCustom *event);

};

#endif /* ParticipantItemNode_hpp */
