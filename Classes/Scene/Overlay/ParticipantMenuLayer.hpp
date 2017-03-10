//
//  ParticipantMenuLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/28.
//
//

#ifndef ParticipantMenuLayer_hpp
#define ParticipantMenuLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class ParticipantMenuLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        PlusGameNum,
        MinusGameNum,
        EntryModeNormal,
        EntryModeEntryGame,
        EntryModeRest,
        Close,
        ShowHistory,
    };
    
    
public:
    static ParticipantMenuLayer *create(std::vector<UserInfo *> userInfoList);
    
    
protected:
    ParticipantMenuLayer();
    virtual ~ParticipantMenuLayer();
    bool init(std::vector<UserInfo *> userInfoList);
    
    // ユーザ情報
    std::vector<UserInfo *> userInfoList;
    
    // ボタン
    cocos2d::ui::Button *buttonNormal;
    cocos2d::ui::Button *buttonEntryGame;
    cocos2d::ui::Button *buttonRest;
    void updateEntryButtons();

    
protected:
    // ボタンイベント
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

    // 閉じる
    void close();
    
};


#endif /* ParticipantMenuLayer_hpp */
