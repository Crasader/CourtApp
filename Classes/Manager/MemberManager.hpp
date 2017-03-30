//
//  MemberManager.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/02.
//
//

#ifndef MemberManager_hpp
#define MemberManager_hpp

#include <vector>

#include "UserInfo.hpp"
#include "LayoutDefine.h"

namespace Manager {
    
    class Member {
        
    private:
        Member();
        static Member* memberManager;
        
        std::vector<UserInfo *> memberList;
        
        // 検索／ソート用
        SortType sortType = SortType::Syllabary;
        std::vector<Gender> filterGenderVec;
        std::vector<Level>  filterLevelVec;
        int filterDays;
        std::string searchWord = "";
        
        int currentId = 0;
        
    public:
        // 初期化
        static Member* getInstance();
        void initialize();
        
        
    public:
        // ********** メンバー **********

        // 全メンバーリストを取得
        std::vector<UserInfo *> getMemberList(bool filtered = true);
        
        // カテゴライズされた後のメンバーリストを取得
        std::vector<std::pair<std::string, std::vector<int>>> getCategorizedMemberList();
        std::vector<std::pair<std::string, std::vector<int>>> getCategorizedMemberListSyllabary();
        std::vector<std::pair<std::string, std::vector<int>>> getCategorizedMemberListLevel();
        std::vector<std::pair<std::string, std::vector<int>>> getCategorizedMemberListCount();
        std::vector<std::pair<std::string, std::vector<int>>> getCategorizedMemberListLastTime();
        
        // フィルタ後のメンバーリストを取得
        std::vector<UserInfo *> getFilteredMemberList();
        
        // 特定IDのメンバーを取得
        UserInfo *getMemberAt(int id);
        
        // メンバー追加
        void addMember(std::string name, std::string nameFurigana, std::string nickname, Gender gender, std::string birthdayStr, Level level, int count = 0, int point = 0, std::string lastTime = "");
        
        // メンバー情報更新
        void updateMember(UserInfo *userInfo);
        
        // メンバー削除
        void removeMemberAt(int id);
        
        
        // ********** 参加者 **********
        
        // 参加／不参加情報の更新
        void updateIsEntry(int id, bool isEntry);

        // 全参加者リストを取得
        std::vector<UserInfo *> getEntryMemberList(bool containsRest = false, Level level = Level::Unknown);
        
        // レベル別に全参加者リストを取得
        std::vector< std::pair<std::string, std::vector<UserInfo *>> > getEntryMemberListPerLevel(bool containsRest = false);
        
        // 特定の試合数の参加者リストを取得
        std::vector<UserInfo *> getEntryMemberListWithGameCount(int gameCount, bool containsEntryGame = false, Level level = Level::Unknown);
        
        // 次の試合に参加するメンバーを決定して取得
        std::vector< std::vector<UserInfo *> > selectNextMatchMembers(int courtNum);
        std::vector< std::vector<UserInfo *> > selectNextMatchMembersRandomPair(int courtNum, Level level = Level::Unknown);
        std::vector< std::vector<UserInfo *> > selectNextMatchMembersByLevel(int courtNum);
        
        // どのレベルのメンバーがエントリーしているか
        std::vector<Level> getUniqueEntryLevelList();
        
        // 参加回数／最終参加日を更新
        void updateParticipantCount();
        
        
        // ********** ソート／検索 **********
        
        // ソート順設定／取得
        void setSortType(SortType type);
        SortType getSortType();
        
        // 検索条件変更
        void updateGenderFilter(Gender gender);
        void updateLevelFilter(Level level);
        void updateLastDateFilter(int days);
        
        // 検索条件取得
        std::vector<Gender> getGenderFilter(){return this->filterGenderVec;}
        std::vector<Level> getLevelFilter(){return this->filterLevelVec;}
        int getLastDateFilter(){return this->filterDays;};
        
        // 検索ワード設定／取得
        void setSearchWord(std::string word){this->searchWord = word;}
        std::string getSearchWord(){return this->searchWord;}
        
        
    private:
        // ********** 試合メンバー決定補助用メソッド **********
        
        // シャッフルタイプ
        ShuffleType shuffleType = ShuffleType::RandomPair;
        AllocationType allocationType = AllocationType::All;
        
        // 試合回数の取得
        int getMinGameCount(Level level = Level::Unknown);
        int getMaxGameCount(Level level = Level::Unknown);
        
        // 試合数／ペア数の少ない人を取得する（再帰）
        std::vector<UserInfo *> getEntryListConsideredRelation(std::vector<UserInfo *> entryMemberList, std::vector<UserInfo *> minMemberList, int addNum);

        
        // ********** 各コートのペア決定補助用メソッド **********
        
        // コート用に4人ずつで分けたリストを返す
        std::vector< std::vector<UserInfo *> > getEntryListByCourt(std::vector<UserInfo *> entryList);
      
        // 条件に沿った形でペアを組んだリストを返す
        std::vector<std::pair<UserInfo *, UserInfo *>> getPairListByCourtRandomPair(std::vector<UserInfo *> entryList);
        std::vector<std::pair<UserInfo *, UserInfo *>> getPairListByCourtRandomPoint(std::vector<UserInfo *> entryList);
        
        
    public:
        // ********** 試合情報の追加／削除 **********
        
        // シャッフルタイプ設定／取得
        void setShuffleType(ShuffleType type);
        ShuffleType getShuffleType();
        void setAllocationType(AllocationType type);
        AllocationType getAllocationType();
        
        
        // 組み合わせ情報を追加
        void addGameMemberInfoList(std::vector< std::vector<UserInfo *> > entryMemberListVec);
        void addGameMemberInfo(std::vector<UserInfo *>entryMemberList, int addNum = 1);
        
        // 勝敗の結果を入力
        void addGameResult(std::vector<UserInfo *> winnerList, std::vector<UserInfo *> loserList);
        
        // 過去の勝敗結果をリセット
        void resetGameResult(std::vector<UserInfo *> winnerList, std::vector<UserInfo *> loserList);
        
        // 全ての勝敗結果をリセット
        void resetAllGameResult();
        
        // 全ての勝敗結果、ペア／対戦回数、試合回数をリセット
        void resetAllCount();
        
    };
    
}   // end namespace Manager


#endif /* MemberManager_hpp */
