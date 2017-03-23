//
//  UserDefaultUtil.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/11.
//
//

#ifndef UserDefaultUtil_h
#define UserDefaultUtil_h

#include "cocos2d.h"
USING_NS_CC;


#define UD_KEY_COURT_NUM    "CourtNum"      // 試合に使うコート数
#define UD_KEY_SORT_TYPE    "SortType"      // ソートタイプ
#define UD_KEY_SHUFFLE_TYPE "ShuffleType"   // シャッフルタイプ
#define UD_KEY_ALLOCATION_TYPE "AllocationType"   // 振り分けタイプ

// 条件検索
#define UD_KEY_FILTER_MALE     "FilterMale"     // 男性
#define UD_KEY_FILTER_FEMALE   "FilterFemale"   // 女性
#define UD_KEY_FILTER_TRAINING "FilterTraining" // 入門
#define UD_KEY_FILTER_BEGINNER "FilterBeginner" // 初級
#define UD_KEY_FILTER_MIDDLE   "FilterMiddle"   // 中級
#define UD_KEY_FILTER_HIGHER   "FilterHigher"   // 上級
#define UD_KEY_FILTER_DAYS     "FilterDays"     // 最終参加日


namespace Kyarochon {
namespace UserDefault {
    
    // ---------- 初期化 ----------
    // ※ 初期化時は複数データの保存が想定されるのでflush()を最後に呼ぶ必要あり
    static void initInt(const char *key, int defaultValue) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setIntegerForKey(key, userDefault->getIntegerForKey(key, defaultValue));
    }
    static void initFloat(const char *key, float defaultValue) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setFloatForKey(key, userDefault->getFloatForKey(key, defaultValue));
    }
    static void initDouble(const char *key, double defaultValue) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setDoubleForKey(key, userDefault->getDoubleForKey(key, defaultValue));
    }
    static void initBool(const char *key, bool defaultValue) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setBoolForKey(key, userDefault->getBoolForKey(key, defaultValue));
    }
    static void initString(const char *key, std::string defaultValue) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setStringForKey(key, userDefault->getStringForKey(key, defaultValue));
    }
    static void initData(const char *key, Data defaultValue) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setDataForKey(key, userDefault->getDataForKey(key, defaultValue));
    }
    static void flush() {
        cocos2d::UserDefault::getInstance()->flush();
    }
    
    
    // ---------- 保存 ----------
    static void setInt(const char *key, int value) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setIntegerForKey(key, value);
        userDefault->flush();
    }
    static void setFloat(const char *key, float value) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setFloatForKey(key, value);
        userDefault->flush();
    }
    static void setDouble(const char *key, double value) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setDoubleForKey(key, value);
        userDefault->flush();
    }
    static void setBool(const char *key, bool value) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setBoolForKey(key, value);
        userDefault->flush();
    }
    static void setString(const char *key, std::string value) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setStringForKey(key, value);
        userDefault->flush();
    }
    static void setData(const char *key, Data value) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        userDefault->setDataForKey(key, value);
        userDefault->flush();
    }
    
    // ---------- 読み出し ----------
    static int getInt(const char *key) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        return userDefault->getIntegerForKey(key);
    }
    static float getFloat(const char *key) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        return userDefault->getFloatForKey(key);
    }
    static double getDouble(const char *key) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        return userDefault->getDoubleForKey(key);
    }
    static bool getBool(const char *key) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        return userDefault->getBoolForKey(key);
    }
    static std::string getString(const char *key) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        return userDefault->getStringForKey(key);
    }
    static Data getData(const char *key) {
        auto userDefault = cocos2d::UserDefault::getInstance();
        return userDefault->getDataForKey(key);
    }
        
}   // end namespace UserDefault
}   // end namespace Kyarochon




#endif /* UserDefaultUtil_h */
