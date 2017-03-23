//
//  CourtManager.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/04.
//
//

#ifndef CourtManager_hpp
#define CourtManager_hpp



#include "CourtInfo.hpp"
#include "CourtDateInfo.hpp"
#include "EventUtil.h"

namespace Manager {
    
    class Court {
        
    private:
        Court();
        static Court* courtManager;
        
        int currentCourtDateId; // 日付別コートごとに一意のID
        int currentCourtId;     // コートごとに一意のID
        int useCourtNum;        // 試合に使うコート数
        int currentGameNum;     // nゲーム目(最新のもの)
        int selectedGameNum;    // 表示するゲーム
        bool showWinNum;        // 勝敗数を表示するかどうか
        
        // 日付別コート一覧
        std::vector<CourtDateInfo *> courtDateList;
        
        // コート情報の履歴
        std::vector<CourtInfo *> courtHistory;
        
        // 入れ替え用
        std::pair<int, UserInfo *> selectedMember;
        
    public:
        // 初期化
        static Court* getInstance();
        void initialize();
        

        // ********** 第XX試合とか **********
        
        // トータルのゲーム数を±1
        void incrementGameNum();
        void decrementGameNum();
        
        // 表示するゲーム数を±1する
        int getSelectedGameNum(){return this->selectedGameNum;}
        void incrementSelectedGameNum();
        void decrementSelectedGameNum();

        // 矢印表示切り替え用
        bool isSelectedFirstGame(){return selectedGameNum <= 0;};
        bool isSelectedLastGame(){return selectedGameNum == currentGameNum;};
        
        
        
        // ********** 日付別コート情報 **********

        // 現在のコートIDを更新
        void updateCurrentCourtDateId();


        
        // ********** コート情報 **********
        
        // 現在のコートIDを更新
        void updateCurrentCourtId();
        
        // 次のコート生成＆取得
        CourtInfo *getNextCourt(int courtNum, std::vector<UserInfo *> participants);
        
        // 直前のコート情報を削除
        void removeCurrentCourt();
        
        // 全ての履歴を削除
        void resetAllHistory();
        
        // 現在表示するコート一覧
        std::vector<CourtInfo *> getSelectedCourtList();
        
        
        // 指定IDのコート情報
        CourtInfo *getCourtInfo(int courtId);
        
        
        // 試合に使うコート数取得／設定
        void setUseCourtNum(int courtNum);
        int getUseCourtNum();

        
        // ********** 勝敗情報の更新 **********
        
        // 勝敗情報の更新
        void updateGameResult(int courtId, WinState winState);
        
        // 勝敗数表示するかどうか
        void setShouldShowWinNum(bool showWinNum){
            this->showWinNum = showWinNum;
            Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_SHOW_WIN_NUM);
        };
        bool shouldShowWinNum(){return showWinNum;}
        
        
        
        // ********** 入れ替え機能 **********
        
        // 選択
        void selectReplaceMember(int courtId, UserInfo *member);
        void finishSelectReplaceMember(int courtId, UserInfo *member);
        
    };
    
}   // end namespace Manager



#endif /* CourtManager_hpp */
