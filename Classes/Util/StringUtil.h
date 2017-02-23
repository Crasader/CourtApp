//
//  StringUtil.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/10.
//
//

#ifndef StringUtil_h
#define StringUtil_h

#include <sstream>
#include <iostream>
#include <time.h>
#include "cocos2d.h"

namespace Kyarochon {
namespace String {

// 指定した文字列で区切る
static std::vector<std::string> split(const std::string &str, char delim)
{
    std::istringstream iss(str);
    std::string tmp;
    std::vector<std::string> res;
    while(getline(iss, tmp, delim)){
        res.push_back(tmp);
    }
    return res;
}

// 現在時刻を文字列で取得
static std::string getCurrentTimeString()
{
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    
    auto text = cocos2d::StringUtils::format("%04d/%02d/%02d %02d:%02d:%02d",
                                    pnow->tm_year + 1900,
                                    pnow->tm_mon + 1,
                                    pnow->tm_mday,
                                    pnow->tm_hour,
                                    pnow->tm_min,
                                    pnow->tm_sec);
    return text;
}
    
    
// 文字列から時刻データを返す
// 引数の形式は"1990/01/01 12:00:00"の形
static time_t getTimeFromString(std::string strTime)
{
    // 正しくないデータの場合、現在時刻を返す
    if (strTime.length() != 19)
    {
        strTime = "1970/01/01 00:00:00";
    }
    
#if 0
    char buf[19];
    std::strncpy(buf, strTime.c_str(), 19);
    
    struct tm *pTime;
    strptime(buf, "%Y/%m/%d %H:%M:%S", pTime);
#else
    struct tm pTime;
    memset(&pTime,0x00,sizeof(struct tm));
    strptime(strTime.c_str(), "%Y/%m/%d %H:%M:%S", &pTime);
#endif
    
    return mktime(&pTime);
}
    
    
    
}   // end namespace String
}   // end namespace Kyarochon

#endif /* StringUtil_h */
