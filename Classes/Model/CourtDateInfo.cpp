//
//  CourtDateInfo.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/12.
//
//

#include "CourtDateInfo.hpp"
#include "TimeUtil.h"

// DBから追加したver
CourtDateInfo::CourtDateInfo(int id, std::string time, std::string place)
{
    this->id    = id;
    this->time  = time;
    this->place = place;
}

// アプリ内で追加したver
CourtDateInfo::CourtDateInfo(int id, std::string place)
{
    this->id    = id;
    this->place = place;
    this->time  = Kyarochon::Time::getCurrentTimeString();
}


CourtDateInfo::~CourtDateInfo()
{
}


