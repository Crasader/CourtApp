//
//  CourtInfo.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/04.
//
//

#ifndef CourtInfo_hpp
#define CourtInfo_hpp


#include "UserInfo.hpp"
#include <string>
#include <vector>

enum class WinState
{
    WinLeft,
    WinRight,
    Unknown,
};

enum class HistoryState
{
    InProgress,
    Finished,
};

class CourtInfo
{
    
public:
    // コンストラクタ／デストラクタ
    CourtInfo(int id, int gameNum, int courtNum, std::string memberListStr, int winState, int historyState, std::string time);
    CourtInfo(int id, int gameNum, int courtNum, std::vector<UserInfo *> memberList);
    virtual ~CourtInfo();
    
    
public:
    int id;         // コートID(Unique)
    int gameNum;    // 第x試合
    int courtNum;   // 第xコート
    std::vector<UserInfo *> memberList;  // 参加者リスト
    WinState winState;
    HistoryState historyState;
    std::string time;
    
    std::vector<UserInfo *> getWinnerList();
    std::vector<UserInfo *> getLoserList();
    
    void replaceMember(UserInfo *oldMember, UserInfo *newMember);
    
    
    std::vector<UserInfo *> convertTextToMemberList(std::string text);
    std::string convertMemberListToText(std::vector<UserInfo *> memberList);
    
    
//    std::vector<int> winnerIdList;       // 勝者ID
//    std::vector<int> loserIdList;        // 敗者ID
};


#endif /* CourtInfo_hpp */
