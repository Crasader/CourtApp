//
//  CourtManager.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/04.
//
//

#include "CourtManager.hpp"
#include "MemberManager.hpp"
#include "DatabaseManager.h"
#include "UserDefaultUtil.h"

namespace Manager {
    
Court* Court::courtManager = nullptr;
    
#pragma mark - 初期化
Court::Court()
{
}
    
Court* Court::getInstance()
{
    if(courtManager == nullptr)
    {
        courtManager = new Court();
        courtManager->initialize();
    }
        
    return courtManager;
}
    
// 値の初期化
void Court::initialize()
{
    // 日付別コート一覧
    courtDateList = Manager::DataBase::getInstance()->getCourtDateInfoVec();
    
    // コート情報一覧
    courtHistory  = Manager::DataBase::getInstance()->getCourtInfoVec();
    this->updateCurrentCourtId();
    
    for (auto courtInfo : courtHistory)
    {
        auto memberManager = Manager::Member::getInstance();
        
        // 参加回数、ペア／対戦回数を追加
        memberManager->addGameMemberInfo(courtInfo->memberList);
        
        // 勝敗結果を追加
        if (courtInfo->winState != WinState::Unknown)
        {
            memberManager->addGameResult(courtInfo->getWinnerList(), courtInfo->getLoserList());
        }
    }
    
    // XX試合
    currentGameNum   = -1;
    for (auto court : courtHistory)
    {
        if (court->historyState == HistoryState::InProgress) currentGameNum = MAX(currentGameNum, court->gameNum);
    }
    selectedGameNum  = currentGameNum;
    
    // その他オプション
    useCourtNum      = Kyarochon::UserDefault::getInt(UD_KEY_COURT_NUM);
    showWinNum       = false;
    selectedMember   = std::make_pair(-1, nullptr);
}
    
#pragma mark - ********** 第XX試合とか **********
void Court::incrementGameNum()
{
    currentGameNum++;
    selectedGameNum = currentGameNum;
}
    
void Court::decrementGameNum()
{
    currentGameNum--;
    if (selectedGameNum > currentGameNum)
        selectedGameNum = currentGameNum;
}

void Court::incrementSelectedGameNum()
{
    if (selectedGameNum < currentGameNum)
        selectedGameNum++;
}

void Court::decrementSelectedGameNum()
{
    if (selectedGameNum > 0)
        selectedGameNum--;    
}
    
#pragma mark - ********** コート情報 **********

// 現在のコートIDを更新
void Court::updateCurrentCourtDateId()
{
    int id = 0;
    for (auto courtDateInfo : courtDateList)
    {
        id = MAX(courtDateInfo->id, id);
    }
    currentCourtDateId = id;
}
    
//    日付別コートのあたりじっそう

    
#pragma mark - ********** コート情報 **********
    
// 現在のコートIDを更新
void Court::updateCurrentCourtId()
{
    int id = 0;
    for (auto courtInfo : courtHistory)
    {
        id = MAX(courtInfo->id + 1, id);
    }
    currentCourtId = id;
}
    
// 次のコート生成＆取得
CourtInfo* Court::getNextCourt(int courtNum, std::vector<UserInfo *> participants)
{
    CourtInfo *info = new CourtInfo(currentCourtId, currentGameNum, courtNum, participants);
    courtHistory.push_back(info);
    Manager::DataBase::getInstance()->addCourtInfo(info);
    
    currentCourtId++;
    
    return info;
}

// 直前のコート情報を削除
void Court::removeCurrentCourt()
{
    // 現在のゲーム数 かつ 進行中のものを削除
    auto it = courtHistory.begin();
    while (it != courtHistory.end()) {
        if ( ((*it)->gameNum == currentGameNum) && ((*it)->historyState == HistoryState::InProgress) )
        {
            Manager::DataBase::getInstance()->removeCourtInfo((*it)->id);
            it = courtHistory.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    // ゲーム数 -1
    this->decrementGameNum();
    
    // コートIDを更新
    this->updateCurrentCourtId();
}
    
    
void Court::resetAllHistory()
{
    auto it = courtHistory.begin();
    while (it != courtHistory.end())
    {
        Manager::DataBase::getInstance()->removeCourtInfo((*it)->id);
        it = courtHistory.erase(it);
    }
    
    currentGameNum   = -1;
    selectedGameNum  = -1;
    selectedMember   = std::make_pair(-1, nullptr);

    this->updateCurrentCourtId();
    
    
    // 参加回数、ペア／対戦回数、勝利回数をリセット
    Manager::Member::getInstance()->resetAllCount();
    
    // 参加者リストの表示更新
    Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_PARTICIPANT_LIST);
}
    
    
// 現在表示するコート一覧
std::vector<CourtInfo *> Court::getSelectedCourtList()
{
    std::vector<CourtInfo *> courtList;
    if (selectedGameNum < 0) return courtList;
    
    for (auto court : this->courtHistory)
    {
        if (court->gameNum == selectedGameNum && court->historyState == HistoryState::InProgress)
            courtList.push_back(court);
    }
    
    return courtList;
}
 
// 指定IDのコート情報
CourtInfo *Court::getCourtInfo(int courtId)
{
    for (auto court : this->courtHistory)
    {
        if (court->id == courtId && court->historyState == HistoryState::InProgress) return court;
    }
    
    return nullptr;
}
    
    
    
#pragma mark - ********** 勝敗情報の更新 **********
void Court::updateGameResult(int courtId, WinState winState)
{
    auto memberManager = Manager::Member::getInstance();
    auto courtInfo = this->getCourtInfo(courtId);
    
    // 既に勝敗情報が入っている場合に、前回の情報を消す
    if (courtInfo->winState != WinState::Unknown)
    {
        memberManager->resetGameResult(courtInfo->getWinnerList(), courtInfo->getLoserList());
    }
    
    // 新しい情報を追加する
    courtInfo->winState = winState;
    if (winState != WinState::Unknown)
    {
        memberManager->addGameResult(courtInfo->getWinnerList(), courtInfo->getLoserList());
    }
    
    Manager::DataBase::getInstance()->updateCourtInfo(courtInfo);
}
    

#pragma mark - ********** 組み合わせ入れ替え **********
void Court::selectReplaceMember(int courtId, UserInfo *member)
{
    // 選択されていなければ登録
    if (selectedMember.first < 0 || !selectedMember.second)
    {
        selectedMember = std::make_pair(courtId, member);
    }
    // 選択されていれば入れ替え
    else
    {
        auto dataBaseManager = Manager::DataBase::getInstance();
        auto oldCourt = this->getCourtInfo(selectedMember.first);
        oldCourt->replaceMember(selectedMember.second, member);
        dataBaseManager->updateCourtInfo(oldCourt);
        
        // 入れ替え前後でコートが違う場合
        if (selectedMember.first != courtId)
        {
            auto newCourt = this->getCourtInfo(courtId);
            newCourt->replaceMember(member, selectedMember.second);
            dataBaseManager->updateCourtInfo(newCourt);
        }
        
        selectedMember = std::make_pair(-1, nullptr);
        Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_COURT_LIST);
    }
    
}
    
void Court::finishSelectReplaceMember(int courtId, UserInfo *member)
{
    // 選択されていれば入れ替え、そうでなければ何もしない
    if (selectedMember.first < 0 || !selectedMember.second) return;
    
    this->selectReplaceMember(courtId, member);
}
    
    
    
    
    
}   // end namespace Manager

