//
//  TimeUtil.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2017/01/20.
//
//

#ifndef TimeUtil_h
#define TimeUtil_h

#include <time.h>
#include "cocos2d.h"
#include "CommonDefine.h"

namespace Kyarochon {
namespace Time {

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

    
// 指定日数あたりの秒数を取得
static double getSecondsOfDay(int days)
{
    return 60.0 * 60.0 * 24.0 * (double)days;
}
// 指定月数あたりの秒数を取得
static double getSecondsOfMonth(int months)
{
    return getSecondsOfDay(DAYS_PER_MONTH * months);
}

    
    

    
    
}   // end namespace Time
}   // end namespace Kyarochon




#endif /* TimeUtil_h */
