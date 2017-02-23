//
//  CourtInfo.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/04.
//
//

#include "CourtInfo.hpp"
#include "MemberManager.hpp"
#include "CourtManager.hpp"
#include "TimeUtil.h"
#include "StringUtil.h"

// DBから追加したver
CourtInfo::CourtInfo(int id, int gameNum, int courtNum, std::string memberListStr, int winState, int historyState, std::string time)
{
    // 基本情報
    this->id           = id;
    this->gameNum      = gameNum;
    this->courtNum     = courtNum;
    this->memberList   = this->convertTextToMemberList(memberListStr);
    this->winState     = (WinState)winState;
    this->historyState = (HistoryState)historyState;
    this->time         = time;
}

// アプリ内で追加したver
CourtInfo::CourtInfo(int id, int gameNum, int courtNum, std::vector<UserInfo *> memberList)
{
    // 基本情報
    this->id           = id;
    this->gameNum      = gameNum;
    this->courtNum     = courtNum;
    this->memberList   = memberList;
    this->winState     = WinState::Unknown;
    this->historyState = HistoryState::InProgress;
    this->time         = Kyarochon::Time::getCurrentTimeString();
}


CourtInfo::~CourtInfo()
{
    
}



std::vector<UserInfo *> CourtInfo::getWinnerList()
{
    std::vector<UserInfo *> winnerList;
    switch (this->winState) {
        case WinState::Unknown:
            break;
        case WinState::WinLeft:
            winnerList.push_back(memberList.at(0));
            winnerList.push_back(memberList.at(1));
            break;
        case WinState::WinRight:
            winnerList.push_back(memberList.at(2));
            winnerList.push_back(memberList.at(3));
            break;
    }
    return winnerList;
}

std::vector<UserInfo *> CourtInfo::getLoserList()
{
    std::vector<UserInfo *> loserList;
    switch (this->winState) {
        case WinState::Unknown:
            break;
        case WinState::WinLeft:
            loserList.push_back(memberList.at(2));
            loserList.push_back(memberList.at(3));
            break;
        case WinState::WinRight:
            loserList.push_back(memberList.at(0));
            loserList.push_back(memberList.at(1));
            break;
    }
    return loserList;
}


void CourtInfo::replaceMember(UserInfo *oldMember, UserInfo *newMember)
{
    // ペア／対戦回数をリセット
    Manager::Member::getInstance()->addGameMemberInfo(memberList, -1);
    
    // 入れ替え
    auto it = memberList.begin();
    while (it != memberList.end())
    {
        if ((*it) == oldMember)
        {
            it = memberList.erase(it);
            memberList.insert(it, newMember);
        }
        else if ((*it) == newMember)
        {
            it = memberList.erase(it);
            memberList.insert(it, oldMember);
        }
                
        ++it;
    }
    
    // 勝敗数が既に入っている場合はリセット
    if (winState != WinState::Unknown)
    {
        Manager::Court::getInstance()->updateGameResult(this->id, WinState::Unknown);
    }
    
    
    // ペア／対戦回数を再設定
    Manager::Member::getInstance()->addGameMemberInfo(memberList, +1);
}



// 変換
std::vector<UserInfo *> CourtInfo::convertTextToMemberList(std::string text)
{
    std::vector<UserInfo *> memberList;
    std::vector<std::string> idTextList = Kyarochon::String::split(text, ',');
    for (auto idText : idTextList)
    {
        int memberId = atoi(idText.c_str());
        auto member = Manager::Member::getInstance()->getMemberAt(memberId);
        memberList.push_back(member);
    }
    
    return memberList;
}

std::string CourtInfo::convertMemberListToText(std::vector<UserInfo *> memberList)
{
    std::string text = "";
    for (auto member : memberList)
    {
        if (!text.empty()) text += ",";
        text += StringUtils::toString(member->id);
    }
    return text;
}


