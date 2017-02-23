//
//  UserInfo.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/01.
//
//

#include "UserInfo.hpp"
#include "StringUtil.h"

// DBから読み込んだver
UserInfo::UserInfo(int id, std::string name, std::string nameFurigana, std::string nickname, std::string genderStr, std::string birthdayStr, std::string levelStr, bool isEntry, int count, int point, std::string lastTimeStr)
{
    this->id            = id;
    this->name          = name;
    this->nameFurigana  = nameFurigana;
    this->nickname      = nickname;
    this->gender        = this->convertStringToGender(genderStr);
    this->birthday      = birthdayStr;
    this->level         = this->convertStringToLevel(levelStr);
    this->isEntry       = isEntry;
    this->count         = count;
    this->point         = point;
    this->lastTime      = lastTimeStr;
    this->initialize();
}


// アプリ内で追加したver
UserInfo::UserInfo(int id, std::string name, std::string nameFurigana, std::string nickname, Gender gender, std::string birthdayStr, Level level, int count, int point, std::string lastTime)
{
    this->id            = id;
    this->name          = name;
    this->nameFurigana  = nameFurigana;
    this->nickname      = nickname;
    this->gender        = gender;
    this->birthday      = birthdayStr;
    this->level         = level;
    this->isEntry       = false;
    this->count         = count;
    this->point         = point;
    this->lastTime      = lastTime;
    this->initialize();
}



UserInfo::~UserInfo() {
    
}

// 参加情報を初期化
void UserInfo::initialize()
{
    this->gameCount = 0;
    this->entryMode  = EntryMode::Normal;
    for (int i = 0; i < 1000; i++)
    {
        pairCountArray[i] = 0;
        fightCountArray[i] = 0;
        winCountArray[i] = 0;
    }
}


Gender UserInfo::convertStringToGender(std::string genderStr)
{
    if (genderStr == "male")
        return Gender::Male;
    else if (genderStr == "female")
        return Gender::Female;
    else
        return Gender::Unknown;
}

std::string UserInfo::convertGenderToString(Gender gender)
{
    switch (gender) {
        case Gender::Male:      return "male";
        case Gender::Female:    return "female";
        default:                return "unknown";
    }
}

Level UserInfo::convertStringToLevel(std::string levelStr)
{
    if (levelStr == "training")
        return Level::Training;
    else if (levelStr == "beginner")
        return Level::Beginner;
    else if (levelStr == "middle")
        return Level::Middle;
    else if (levelStr == "higher")
        return Level::Higher;
    else
        return Level::Unknown;
}

std::string UserInfo::convertLevelToString(Level level)
{
    switch (level) {
        case Level::Training:   return "training";
        case Level::Beginner:   return "begginer";
        case Level::Middle:     return "middle";
        case Level::Higher:     return "higher";
        default:                return "unknown";
    }
}


std::string UserInfo::getLevelText()
{
    switch (level) {
        case Level::Training:   return "練習";
        case Level::Beginner:   return "初級";
        case Level::Middle:     return "中級";
        case Level::Higher:     return "上級";
        default:                return "その他";
    }
}

std::string UserInfo::getNickname()
{
    if (nickname.empty())
        return name;
    else
        return nickname;
}


// 勝敗数
int UserInfo::getWinCount()
{
    int count = 0;
    for (auto winCount : winCountArray)
    {
        count += winCount;
    }
    
    return count / 2;
}

void UserInfo::addWinCount(int memberId)
{
    winCountArray[memberId]++;
}

void UserInfo::resetWinCount()
{
    for (int i = 0; i < 1000; i++)
    {
        winCountArray[i] = 0;
    }
}


// 最終参加日
std::string UserInfo::getLastDate()
{
    if (lastTime.length() != 19)
        return "なし";
    else
        return lastTime.substr(0, 10);
}

double UserInfo::getElapsedTimeFromLastTime()
{
    time_t lastTime = Kyarochon::String::getTimeFromString(this->lastTime);
    time_t now = time(NULL);

    return difftime(now, lastTime);
}
