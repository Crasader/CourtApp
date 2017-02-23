//
//  MemberManager.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/02.
//
//

#include "MemberManager.hpp"
#include "DatabaseManager.h"
#include "CourtManager.hpp"

#include "TimeUtil.h"
#include "UserDefaultUtil.h"

// スコアの重み付け、数が小さいほうが選ばれやすくなる
#define PAIR_COUNT_WEIGHT  (1)
#define FIGHT_COUNT_WEIGHT (2)


namespace Manager {
    
Member* Member::memberManager = nullptr;
    
#pragma mark - 初期化
Member::Member()
{
}
    
Member* Member::getInstance()
{
    if(memberManager == nullptr)
    {
        memberManager = new Member();
        memberManager->initialize();
    }
    
    return memberManager;
}
    
void Member::initialize()
{
    memberList = Manager::DataBase::getInstance()->getUserInfoVec();
    
    
    currentId = 0;
    for (auto member : memberList)
    {
        if (member->id > currentId) currentId = member->id;
    }
    
    this->sortType = (SortType)Kyarochon::UserDefault::getInt(UD_KEY_SORT_TYPE);
    this->filterDays = Kyarochon::UserDefault::getInt(UD_KEY_FILTER_DAYS);
    
    filterGenderVec.clear();
    if (Kyarochon::UserDefault::getBool(UD_KEY_FILTER_MALE)) filterGenderVec.push_back(Gender::Male);
    if (Kyarochon::UserDefault::getBool(UD_KEY_FILTER_FEMALE)) filterGenderVec.push_back(Gender::Female);
    
    filterLevelVec.clear();
    if (Kyarochon::UserDefault::getBool(UD_KEY_FILTER_TRAINING)) filterLevelVec.push_back(Level::Training);
    if (Kyarochon::UserDefault::getBool(UD_KEY_FILTER_BEGINNER)) filterLevelVec.push_back(Level::Beginner);
    if (Kyarochon::UserDefault::getBool(UD_KEY_FILTER_MIDDLE)) filterLevelVec.push_back(Level::Middle);
    if (Kyarochon::UserDefault::getBool(UD_KEY_FILTER_HIGHER)) filterLevelVec.push_back(Level::Higher);
}

    
#pragma mark - ********** メンバー **********

// 全メンバーリストを取得
std::vector<UserInfo *> Member::getMemberList()
{
    std::vector<UserInfo *> sortedMemberList = this->getFilteredMemberList();
    
    // ソートする
    std::sort(sortedMemberList.begin(), sortedMemberList.end(), [this](const UserInfo *a, const UserInfo *b){
        switch (this->sortType) {
            case SortType::Syllabary: return (a->nameFurigana < b->nameFurigana);
            case SortType::Gender:    return (a->gender == b->gender) ? (a->nameFurigana < b->nameFurigana) : (a->gender < b->gender);
            case SortType::Level:     return (a->level == b->level)   ? (a->nameFurigana < b->nameFurigana) : (a->level < b->level);
            case SortType::Count:     return (a->count == b->count)   ? (a->nameFurigana < b->nameFurigana) : (a->count > b->count);
            case SortType::LastDate:  return (a->lastTime < b->lastTime);
        }
    });
    
    return sortedMemberList;
}
    
std::vector<std::pair<std::string, std::vector<int>>> Member::getCategorizedMemberList()
{
//    std::vector<std::pair<std::string, std::vector<int>>> categorizedMemberList;
    switch (this->sortType) {
        case SortType::Syllabary:   return this->getCategorizedMemberListSyllabary();
        case SortType::Level:       return this->getCategorizedMemberListLevel();
        case SortType::Count:       return this->getCategorizedMemberListCount();
        case SortType::LastDate:    return this->getCategorizedMemberListLastTime();
        default:    break;
    }
    
    return this->getCategorizedMemberListSyllabary();
}
    
    
// 五十音順
std::vector<std::pair<std::string, std::vector<int>>> Member::getCategorizedMemberListSyllabary()
{
    std::vector<std::pair<std::string, std::vector<int>>> categorizedMemberList;
    auto memberList = this->getMemberList();

    std::vector<std::string> titleList = {"あ", "か", "さ", "た", "な", "は", "ま", "や", "ら", "わ", "ん"};
    for (int i = 0; i < titleList.size() - 1; i++)
    {
        std::string current = titleList.at(i);
        std::string next = titleList.at(i + 1);
        std::vector<int> categoryIdList;
        
        for (auto it = memberList.begin(); it != memberList.end(); )
        {
            auto member = (*it);
            if (member->nameFurigana > current && member->nameFurigana < next)
            {
                categoryIdList.push_back(member->id);
                it = memberList.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        if (!categoryIdList.empty())
        {
            categorizedMemberList.emplace_back(current + "行", categoryIdList);
        }
    }
    
    // 「あ」〜「ん」の間に無いものは「その他」枠に入れる
    if (!memberList.empty())
    {
        std::vector<int> categoryIdList;
        for (auto member : memberList)
        {
            categoryIdList.push_back(member->id);
        }
        categorizedMemberList.emplace_back("その他", categoryIdList);
    }
    
    return categorizedMemberList;
}

// レベル別
std::vector<std::pair<std::string, std::vector<int>>> Member::getCategorizedMemberListLevel()
{
    std::vector<std::pair<std::string, std::vector<int>>> categorizedMemberList;
    auto memberList = this->getMemberList();
    
    std::vector<Level> levelList = {Level::Training, Level::Beginner, Level::Middle, Level::Higher};

    for (auto level : levelList)
    {
        std::vector<int> categoryIdList;
        std::string levelText = "";
        
        for (auto it = memberList.begin(); it != memberList.end(); )
        {
            auto member = (*it);
            if (member->level == level)
            {
                levelText = member->getLevelText();
                categoryIdList.push_back(member->id);
                it = memberList.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        if (!categoryIdList.empty())
        {
            categorizedMemberList.emplace_back(levelText, categoryIdList);
        }
    }
    
    // レベルが無いものは「その他」枠に入れる
    if (!memberList.empty())
    {
        std::vector<int> categoryIdList;
        for (auto member : memberList)
        {
            categoryIdList.push_back(member->id);
        }
        categorizedMemberList.emplace_back("その他", categoryIdList);
    }
    
    return categorizedMemberList;
}


// 回数順
std::vector<std::pair<std::string, std::vector<int>>> Member::getCategorizedMemberListCount()
{
    std::vector<std::pair<std::string, std::vector<int>>> categorizedMemberList;
    auto memberList = this->getMemberList();
    
    std::vector<int> countList = {30, 20, 10, 5, 4, 3, 2, 1, 0};

    for (int count : countList)
    {
        std::vector<int> categoryIdList;
        std::string levelText = "";

        for (auto it = memberList.begin(); it != memberList.end(); )
        {
            auto member = (*it);
            if (member->count >= count)
            {
                levelText = StringUtils::format("%d回", count);
                categoryIdList.push_back(member->id);
                it = memberList.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        if (!categoryIdList.empty())
        {
            categorizedMemberList.emplace_back(levelText, categoryIdList);
        }
    }
    
    return categorizedMemberList;
}

    
std::vector<std::pair<std::string, std::vector<int>>> Member::getCategorizedMemberListLastTime()
{
    std::vector<std::pair<std::string, std::vector<int>>> categorizedMemberList;
    auto memberList = this->getMemberList();
    
    std::vector<std::pair<double, std::string>> timePairList;
    timePairList.emplace_back(Kyarochon::Time::getSecondsOfDay(5), "5日以内");
    timePairList.emplace_back(Kyarochon::Time::getSecondsOfDay(10), "10日以内");
    timePairList.emplace_back(Kyarochon::Time::getSecondsOfDay(20), "20日以内");
    timePairList.emplace_back(Kyarochon::Time::getSecondsOfMonth(1), "1ヶ月以内");
    timePairList.emplace_back(Kyarochon::Time::getSecondsOfMonth(3), "3ヶ月以内");

    for (auto timePair : timePairList)
    {
        std::vector<int> categoryIdList;
        for (auto it = memberList.begin(); it != memberList.end(); )
        {
            auto member = (*it);
            double elapsedTime = member->getElapsedTimeFromLastTime();
            if (elapsedTime <= timePair.first)
            {
                categoryIdList.push_back(member->id);
                it = memberList.erase(it);
            }
            else
            {
                ++it;
            }
            
        }
        if (!categoryIdList.empty())
        {
            categorizedMemberList.emplace_back(timePair.second, categoryIdList);
        }
    }
    
    
    // レベルが無いものは「それ以上」枠に入れる
    if (!memberList.empty())
    {
        std::vector<int> categoryIdList;
        for (auto member : memberList)
        {
            categoryIdList.push_back(member->id);
        }
        categorizedMemberList.emplace_back("それ以上", categoryIdList);
    }

    
    return categorizedMemberList;
}
    
    
    
std::vector<UserInfo *> Member::getFilteredMemberList()
{
    std::vector<UserInfo *> filteredMemberList;
    filteredMemberList.insert(filteredMemberList.end(), memberList.begin(), memberList.end());
    
    auto it = filteredMemberList.begin();
    while (it != filteredMemberList.end()) {
        bool filtered = false;
        
        // 検索ワードでフィルタ
        if (!searchWord.empty())
        {
            filtered = ((*it)->name.find(searchWord) == std::string::npos &&
                        (*it)->nickname.find(searchWord) == std::string::npos &&
                        (*it)->nameFurigana.find(searchWord) == std::string::npos);
        }
        
        // 性別でフィルタ
        if (!filtered)
        {
            filtered = std::find(filterGenderVec.begin(), filterGenderVec.end(), (*it)->gender) == filterGenderVec.end();
        }
            
        // レベルでフィルタ
        if (!filtered)
        {
            filtered = std::find(filterLevelVec.begin(), filterLevelVec.end(), (*it)->level) == filterLevelVec.end();
        }
        
        // 最終参加日でフィルタ
        if (!filtered)
        {
            double filterSeconds = Kyarochon::Time::getSecondsOfDay(this->filterDays);
            filtered = (filterSeconds > 0.0 && (*it)->getElapsedTimeFromLastTime() > filterSeconds);
        }
        
        
        // 除外条件に含まれていたらリストから削除
        if (filtered)
            it = filteredMemberList.erase(it);
        else
            ++it;
    }
    
    return filteredMemberList;
}
    
// 特定IDのメンバーを取得
UserInfo *Member::getMemberAt(int id)
{
    for (auto userInfo : memberList)
    {
        if (userInfo->id == id) return userInfo;
    }
    
    return nullptr;
}

// メンバー追加
void Member::addMember(std::string name, std::string nameFurigana, std::string nickname, Gender gender, std::string birthdayStr, Level level, int count, int point, std::string lastTime)
{
    currentId++;
    auto newMember = new UserInfo(currentId, name, nameFurigana, nickname, gender, birthdayStr, level, count, point, lastTime);
    
    // DBに保存
    Manager::DataBase::getInstance()->addUserInfo(newMember);
    
    // メンバーリストにも追加
    memberList.push_back(newMember);
}

    
// メンバー情報更新
void Member::updateMember(UserInfo *userInfo)
{
    // DBの情報更新
    Manager::DataBase::getInstance()->updateUserInfo(userInfo);
}
    
// メンバー削除
void Member::removeMemberAt(int id)
{
    // DBの情報更新
    Manager::DataBase::getInstance()->removeUserInfo(id);
    
    // メンバーリストから削除
    auto it = memberList.begin();
    while(it != memberList.end())
    {
        if ((*it)->id == id)
            it = memberList.erase(it);
        else
            ++it;
    }
}
    
    
    
#pragma mark -  ********** 参加者 **********
    
// 参加／不参加情報の更新
void Member::updateIsEntry(int id, bool isEntry)
{
    auto member = this->getMemberAt(id);
    if (!member) return;
    
    member->isEntry = isEntry;
    Manager::DataBase::getInstance()->updateUserInfo(member);
}

// 全参加者リストを取得
std::vector<UserInfo *> Member::getEntryMemberList(bool containsRest)
{
    std::vector<UserInfo *> entryList;
    for (auto member : this->memberList)
    {
        if (containsRest && member->entryMode == EntryMode::Rest) continue;
        
        // 参加しているメンバーのみ取得
        if (member->isEntry) entryList.push_back(member);
    }
        
    return entryList;
}
    
// レベル別に全参加者リストを取得
    std::vector< std::pair<std::string, std::vector<UserInfo *>> > Member::getEntryMemberListPerLevel(bool containsRest)
{
    std::vector<UserInfo *> entryList = this->getEntryMemberList(containsRest);
    std::vector< std::pair<std::string, std::vector<UserInfo *>> > entryListPerLevel;
    std::vector<Level> levelList = {Level::Training, Level::Beginner, Level::Middle, Level::Higher, Level::Unknown};
    
    // レベル別に分ける
    for (auto level : levelList)
    {
        auto it = entryList.begin();
        std::vector<UserInfo *> list;
        while (it != entryList.end())
        {
            if ((*it)->level == level)
            {
                list.push_back(*it);
                it = entryList.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (!list.empty())
        {
            entryListPerLevel.emplace_back(list.front()->getLevelText(), list);
        }
    }

    return entryListPerLevel;
}
    
    
// 特定の試合数の参加者リストを取得
std::vector<UserInfo *> Member::getEntryMemberListWithGameCount(int gameCount, bool containsEntryGame)
{
    std::vector<UserInfo *> entryList;
    for (auto member : this->memberList)
    {
        // 参加 ＆ 休憩モードじゃない ＆ gameCountと同じ試合回数
        bool shouldEntry = member->isEntry && member->entryMode != EntryMode::Rest && member->gameCount == gameCount;
        
        // 絶対参加を含む場合
        if (containsEntryGame)
        {
            if (member->entryMode == EntryMode::EntryGame) shouldEntry = true;
        }
        
        if (shouldEntry) entryList.push_back(member);
    }
    
    return entryList;
}


    
    
// 次の試合に参加するメンバーを決定して取得
std::vector< std::vector<UserInfo *> > Member::selectNextMatchMembers(int courtNum)
{
    std::vector< std::vector<UserInfo *> > entryMemberListVec;
    std::vector<UserInfo *> entryMemberList;
    
    bool containsRest = true;
    int entryMemberNum = (int)this->getEntryMemberList(containsRest).size();
    
    // 参加者が4人以下なら試合できない
    if (entryMemberNum < 4)
    {
        Kyarochon::Event::showAlertView("参加者が4人に達していません");
        return entryMemberListVec;
    }
    
    // 試合回数の少ない人から取ってくる
    if (courtNum * 4 > entryMemberNum) courtNum = (int)(entryMemberNum / 4);
    std::vector<UserInfo *> minMemberList;
    std::vector<UserInfo *> maxMemberList;
    int minCount = this->getMinGameCount();
    int maxCount = this->getMaxGameCount();
    for (int i = minCount; i <= maxCount; i++)
    {
        bool containsEntryGame = (i == minCount);
        std::vector<UserInfo *> tempList = this->getEntryMemberListWithGameCount(i, containsEntryGame);
        
        if (minMemberList.empty() || minMemberList.size() + tempList.size() <= courtNum * 4)
            minMemberList.insert(minMemberList.end(), tempList.begin(), tempList.end());
        else
            maxMemberList.insert(maxMemberList.end(), tempList.begin(), tempList.end());
    }
    
    
    // 確実に参加できる人を持ってくる
    // 1. minの人は全員参加、maxから何人か持ってくるパターン
    if (minMemberList.size() <= courtNum * 4)
    {
        entryMemberList.insert(entryMemberList.end(), minMemberList.begin(), minMemberList.end());
        minMemberList.clear();
        minMemberList.insert(minMemberList.end(), maxMemberList.begin(), maxMemberList.end());
        maxMemberList.clear();
    }
    
    // 2. minの人の中で参加できない人が出る、maxは全員参加不可パターン
    else
    {
        // 絶対参加者をtempListに追加し、minMemberListから削除する
        std::vector<UserInfo *> tempList;
        for (std::vector<UserInfo *>::iterator it = minMemberList.begin() ; it != minMemberList.end() ;)
        {
            if ((*it)->entryMode == EntryMode::EntryGame)
            {
                tempList.push_back(*it);
                it = minMemberList.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        // a) 絶対参加者が定員に収まる場合 → 絶対参加者をentryMemberListに追加、残りはminMemberListに
        if (tempList.size() <= courtNum * 4 )
        {
            entryMemberList.insert(entryMemberList.end(), tempList.begin(), tempList.end());
        }
        // b) 絶対参加者が定員をオーバーする場合 → entryMemberListは空、絶対参加者のみminMemberListに
        else
        {
            minMemberList.clear();
            minMemberList.insert(minMemberList.end(), tempList.begin(), tempList.end());
        }
        
        maxMemberList.clear();
    }
    
    
    std::shuffle(entryMemberList.begin(), entryMemberList.end(), std::mt19937());
    if (entryMemberList.size() == courtNum * 4)
        entryMemberListVec = getEntryListByCourt(entryMemberList);
    else
        entryMemberListVec = getEntryListByCourt( getEntryListConsideredRelation(entryMemberList, minMemberList, courtNum * 4 - (int)entryMemberList.size()) );
    
    // 試合回数／ペア回数／対戦回数を更新
//    this->addGameMemberInfo(entryMemberListVec);
    
    return entryMemberListVec;
}


// 参加回数／最終参加日を更新
void Member::updateParticipantCount()
{
    auto entryMemberList = this->getEntryMemberList();
    for (auto &member : entryMemberList)
    {
        member->lastTime = Kyarochon::Time::getCurrentTimeString();
        member->count++;
        this->updateMember(member);
    }    
}
    
#pragma mark - ********** ソート／検索条件 **********
    
// 検索条件更新
// 引数が既にリストにあれば削除、なければ追加
void Member::updateGenderFilter(Gender gender)
{
    bool containsTarget = false;
    auto it = filterGenderVec.begin();
    while (it != filterGenderVec.end()) {
        if ((*it) == gender)
        {
            it = filterGenderVec.erase(it);
            containsTarget = true;
        }
        else
        {
            ++it;
        }
    }
    
    if (!containsTarget)
        filterGenderVec.push_back(gender);
}

void Member::updateLevelFilter(Level level)
{
    bool containsTarget = false;
    auto it = filterLevelVec.begin();
    while (it != filterLevelVec.end())
    {
        if ((*it) == level)
        {
            it = filterLevelVec.erase(it);
            containsTarget = true;
        }
        else
        {
            ++it;
        }
    }
    
    if (!containsTarget)
        filterLevelVec.push_back(level);
}
    
  
void Member::updateLastDateFilter(int days)
{
    this->filterDays = (days == this->filterDays) ? 0 : days;
}
    
    
#pragma mark - ********** 試合メンバー決定補助用メソッド **********
    
// 最小試合回数
int Member::getMinGameCount()
{
    int minCount = INT_MAX;
    for (auto member : this->memberList)
    {
        if (member->isEntry)
            minCount = MIN(minCount, member->gameCount);
    }
    return minCount;
}
    
// 最大試合回数
int Member::getMaxGameCount()
{
    int maxCount = -1;
    for (auto member : this->memberList)
    {
        if (member->isEntry)
            maxCount = MAX(maxCount, member->gameCount);
    }
    return maxCount;
}
    
    
// 試合数／ペア数の少ない人を取得する
std::vector<UserInfo *> Member::getEntryListConsideredRelation(std::vector<UserInfo *> entryMemberList, std::vector<UserInfo *> minMemberList, int addNum)
{
    std::shuffle(entryMemberList.begin(), entryMemberList.end(), std::mt19937());
    if (addNum == 0) return entryMemberList;
    
    int minCount = INT_MAX;
    UserInfo *selectedMember = nullptr;

    std::shuffle(minMemberList.begin(), minMemberList.end(), std::mt19937());
    for (auto member : minMemberList)
    {
        int count = 0;
        for (auto entryMember : entryMemberList)
        {
            count += member->fightCountArray[entryMember->id];
            count += member->pairCountArray[entryMember->id];
        }
        if (minCount > count)
        {
            minCount = count;
            selectedMember = member;
        }
    }
    
    if (!selectedMember) CCLOG("ERROR!!!");
    
    
    entryMemberList.push_back(selectedMember);
    for(auto it = minMemberList.begin(); it != minMemberList.end();)
    {
        if ((*it) == selectedMember)
            it = minMemberList.erase(it);
        else
            ++it;
    }
    addNum--;
    
    return getEntryListConsideredRelation(entryMemberList, minMemberList, addNum);
}
    
    
// コート用に4人ずつで分けたリストを返す
std::vector< std::vector<UserInfo *> > Member::getEntryListByCourt(std::vector<UserInfo *> entryList)
{
    std::vector< std::vector<UserInfo *> > entryListByCourt;
    
    std::vector<UserInfo *> courtMembers;
    while (entryList.size() > 0) {
        // 各コート一人目は最初の人を持ってくる
        if (courtMembers.size() == 0)
        {
            courtMembers.push_back(entryList.at(0));
            entryList.erase(entryList.begin());
            continue;
        }
        
        UserInfo *newMember = nullptr;
        int minScore = INT_MAX;
        for (auto member : entryList)
        {
            // Aとペア回数の少ない人
            int currentScore = INT_MAX;
            if (courtMembers.size() == 1)
            {
                currentScore = member->pairCountArray[courtMembers.at(0)->id] * PAIR_COUNT_WEIGHT;
            }
            // A,Bと対戦回数の少ない人
            else if (courtMembers.size() == 2)
            {
                currentScore = member->fightCountArray[courtMembers.at(0)->id] * FIGHT_COUNT_WEIGHT
                             + member->fightCountArray[courtMembers.at(1)->id] * FIGHT_COUNT_WEIGHT;
            }
            // A,Bと対戦回数の少ないかつCとペア回数の少ない人
            else
            {
                currentScore = member->fightCountArray[courtMembers.at(0)->id] * FIGHT_COUNT_WEIGHT
                             + member->fightCountArray[courtMembers.at(1)->id] * FIGHT_COUNT_WEIGHT
                             + member->pairCountArray[courtMembers.at(2)->id]  * PAIR_COUNT_WEIGHT;
            }
            
            if (minScore > currentScore)
            {
                minScore = currentScore;
                newMember = member;
            }
        }
        
        
        // 元のリストから削除
        for (auto it = entryList.begin(); it != entryList.end();)
        {
            if ((*it) == newMember)
                it = entryList.erase(it);
            else
                ++it;
        }
        
        // 4人に達したらコート毎のリストに追加
        courtMembers.push_back(newMember);
        if (courtMembers.size() == 4)
        {
            entryListByCourt.push_back(courtMembers);
            courtMembers.clear();
        }
    }
    
    return entryListByCourt;
}


    
#pragma mark - ********** 試合情報の追加／削除 **********
// 組み合わせ情報を追加(一括)
void Member::addGameMemberInfoList(std::vector< std::vector<UserInfo *> > entryMemberListVec)
{
    for (auto entryMemberList : entryMemberListVec)
    {
        this->addGameMemberInfo(entryMemberList);
    }
}
    
// 組み合わせ情報を追加(個別)
void Member::addGameMemberInfo(std::vector<UserInfo *> entryMemberList, int addNum)
{
    for (int i = 0; i < entryMemberList.size(); i++)
    {
        auto member = entryMemberList.at(i);
        // ペア回数／対戦回数を追加
        switch (i) {
            case 0:
                member->pairCountArray[entryMemberList[1]->id]  += addNum;
                member->fightCountArray[entryMemberList[2]->id] += addNum;
                member->fightCountArray[entryMemberList[3]->id] += addNum;
                break;
            case 1:
                member->pairCountArray[entryMemberList[0]->id]  += addNum;
                member->fightCountArray[entryMemberList[2]->id] += addNum;
                member->fightCountArray[entryMemberList[3]->id] += addNum;
                break;
            case 2:
                member->pairCountArray[entryMemberList[3]->id]  += addNum;
                member->fightCountArray[entryMemberList[0]->id] += addNum;
                member->fightCountArray[entryMemberList[1]->id] += addNum;
                break;
            case 3:
                member->pairCountArray[entryMemberList[2]->id]  += addNum;
                member->fightCountArray[entryMemberList[0]->id] += addNum;
                member->fightCountArray[entryMemberList[1]->id] += addNum;
                break;
        }
        
        // 試合回数を追加
        member->gameCount += addNum;
    }
}
    
// 勝敗の結果を入力
void Member::addGameResult(std::vector<UserInfo *> winnerList, std::vector<UserInfo *> loserList)
{
    for (auto winner : winnerList)
    {
        for (auto loser : loserList)
        {
            winner->winCountArray[loser->id]++;
        }
    }
}
    
// 前回入力した勝敗をリセット
void Member::resetGameResult(std::vector<UserInfo *> winnerList, std::vector<UserInfo *> loserList)
{
    for (auto winner : winnerList)
    {
        for (auto loser : loserList)
        {
            winner->winCountArray[loser->id]--;
        }
    }
}
    
// 全ての勝敗結果をリセット
void Member::resetAllGameResult()
{
    for (auto member : memberList)
    {
        member->resetWinCount();
    }
}
    
// 全ての勝敗結果、ペア／対戦回数、試合回数をリセット
void Member::resetAllCount()
{
    for (auto member : memberList)
    {
        for (int i = 0; i < 1000; i++)
        {
            member->pairCountArray[i] = 0;
            member->fightCountArray[i] = 0;
            member->winCountArray[i] = 0;
        }
        member->gameCount = 0;
        member->isEntry = false;
        
        Manager::DataBase::getInstance()->updateUserInfo(member);
    }
    
}
    
}   // end namespace Manager

