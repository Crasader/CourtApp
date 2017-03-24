//
//  CourtLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/29.
//
//

#ifndef CourtLayer_hpp
#define CourtLayer_hpp

#include "CsbLayerBase.hpp"
#include "CourtInfo.hpp"

class CourtLayer : public CsbLayerBase
{
    
    enum ButtonTag
    {
        Right,
        Left,
        Start,
        Speech,
    };
    
public:
    static CourtLayer *create();
    
protected:
    CourtLayer();
    virtual ~CourtLayer();
    bool init() override;
    
    // ボタンイベント
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type) override;
    
    // スクロールビューを初期化
    virtual void initScrollView(int itemNum);
    
private:
    // 新規ゲーム開始
    bool lockedStartGame;
    void showNewCourt();
    void startNewGame();
    void cancelNewGame();
    
    // 過去のゲーム表示
    void showCourtHistory();
    void showCourtView(CourtInfo *courtInfo);
    
    // 矢印ボタンの表示更新
    void updateArrowButton();
    
    // 音声読み上げ
    std::string makeSpeechText(std::vector< std::vector<UserInfo *> > members);
    std::string makeSpeechText(std::vector<CourtInfo *> courtInfoList);
    std::string makeSpeechTextEachCourt(int courtNum, std::vector<UserInfo *> memberList);
    
private:
    cocos2d::ui::Button *rightButton;
    cocos2d::ui::Button *leftButton;
    
    std::vector< std::vector<UserInfo *> > currentSelectedMembers;
    
    
    void eventUpdateCourtList(EventCustom *event);
    
    
};


#endif /* CourtLayer_hpp */
