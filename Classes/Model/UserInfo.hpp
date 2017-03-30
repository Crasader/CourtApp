//
//  UserInfo.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/01.
//
//

#ifndef UserInfo_hpp
#define UserInfo_hpp

#include <string>

enum class Gender
{
    Male,
    Female,
    Unknown,
};

enum class Level
{
    Training,
    Beginner,
    Middle,
    Higher,
    Unknown,
};

enum class EntryMode
{
    Normal,     // 通常
    EntryGame,  // 絶対参加
    Rest,       // 休憩
};


class UserInfo
{
public:
    // コンストラクタ／デストラクタ
    UserInfo(int id, std::string name, std::string nameFurigana, std::string nickname, std::string genderStr, std::string birthdayStr, std::string levelStr, bool isEntry, int count, int point, std::string lastTimeStr);
    UserInfo(int id, std::string name, std::string nameFurigana, std::string nickname, Gender gender, std::string birthdayStr, Level level, int count = 0, int point = 0, std::string lastTime = "");
    virtual ~UserInfo();
    

public:
    // 基本情報
    int id;
    std::string name;
    std::string nameFurigana;
    std::string nickname;
    Gender gender;
    std::string birthday;
    Level level;
    bool isEntry;
    int count;
    int point;
    std::string lastTime;
    
    // 参加情報
    int gameCount;
    EntryMode entryMode;
    bool checked;
    int pairCountArray[1000];
    int fightCountArray[1000];
    int winCountArray[1000];
    
    
    // 試合回数増減
    void increaseGameCount(){this->gameCount++;};
    void decreaseGameCount(){ if(gameCount>0)this->gameCount--;};
    
    // データ取得
    std::string getGenderStr(){return this->convertGenderToString(this->gender);};
    std::string getLevelStr(){return this->convertLevelToString(this->level);};
    std::string getLevelText();
    int getLevelPoint();
    std::string getNickname();
    
    // 試合参加／キャンセル
    void entryGame();
    void leaveGame();
    
    // 勝敗数
    int getWinCount();
    void addWinCount(int memberId);
    void resetWinCount();
    
    // 最終参加(日付まで)
    std::string getLastDate();
    
    // 最終参加日からの経過時間
    double getElapsedTimeFromLastTime();
    
protected:
    // 初期化
    void initialize();
    
    // 性別と文字列の変換
    Gender convertStringToGender(std::string genderStr);
    std::string convertGenderToString(Gender gender);
    // レベルと文字列の変換
    Level convertStringToLevel(std::string levelStr);
    std::string convertLevelToString(Level level);
};

#endif /* UserInfo_hpp */
