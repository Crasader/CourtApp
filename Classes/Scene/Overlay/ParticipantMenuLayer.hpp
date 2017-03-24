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
        Close,
        CancelEntry,
        ShowHistory,
    };

    enum CheckBoxTag
    {
        EntryModeNormal,
        EntryModeEntryGame,
        EntryModeRest,
    };
    
    
public:
    static ParticipantMenuLayer *create(std::vector<UserInfo *> userInfoList);
    
    
private:
    void increaseGamecount();
    void decreaseGameCount();
    void leaveGame();
    
    
protected:
    ParticipantMenuLayer();
    virtual ~ParticipantMenuLayer();
    bool init(std::vector<UserInfo *> userInfoList);
    
    // ユーザ情報
    std::vector<UserInfo *> userInfoList;
    
    // ボタン
    virtual void pushedButton(Ref *pSender, ui::Widget::TouchEventType type);

    
    // チェックボックス
    cocos2d::ui::CheckBox *checkBoxNormal;
    cocos2d::ui::CheckBox *checkBoxEntryGame;
    cocos2d::ui::CheckBox *checkBoxRest;
    void updateEntryCheckBoxes();
    virtual void pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type);

    // 閉じる
    void close();
    
};


#endif /* ParticipantMenuLayer_hpp */
