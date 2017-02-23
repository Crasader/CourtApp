//
//  DatabaseManager.cpp
//  SaltyIdol
//
//  Created by saitou kiyotaka on 2015/06/29.
//
//

#include "DatabaseManager.h"


static const std::string DB_NAME = "court_app.db";


// ユーザ
static const std::string SQL_CREATE_USER = "CREATE TABLE IF NOT EXISTS users (id integer unsigned PRIMARY KEY, name text DEFAULT NULL, name_furigana text DEFAULT NULL, nickname text DEFAULT NULL, gender text DEFAULT NULL, birthday text DEFAULT NULL, level text DEFAULT NULL, is_entry integer unsigned DEFAULT 0, count integer unsigned DEFAULT 0, point integer unsigned DEFAULT 0, last_time text DEFAULT NULL);";
static const std::string SQL_INSERT_USER = "INSERT INTO users VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
static const std::string SQL_SELECT_USER = "SELECT id, name, name_furigana, nickname, gender, birthday, level, is_entry, count, point, last_time FROM users";
static const std::string SQL_UPDATE_USER = "UPDATE users SET name = ?, name_furigana = ?, nickname = ?, gender = ?, birthday = ?, level = ?, is_entry = ?, count = ?, point = ?, last_time = ? WHERE id = ?";
static const std::string SQL_DELETE_USER = "DELETE FROM users WHERE id = ?";

// コート
static const std::string SQL_CREATE_COURT = "CREATE TABLE IF NOT EXISTS courts (id integer unsigned PRIMARY KEY, game_num integer unsigned, court_num integer unsigned, member_list text DEFAULT NULL, win_state integer, history_state integer, time text DEFAULT NULL);";
static const std::string SQL_INSERT_COURT = "INSERT INTO courts VALUES (?, ?, ?, ?, ?, ?, ?);";
static const std::string SQL_SELECT_COURT = "SELECT id, game_num, court_num, member_list, win_state, history_state, time FROM courts";
static const std::string SQL_UPDATE_COURT = "UPDATE courts SET game_num = ?, court_num = ?, member_list = ?, win_state = ?, history_state = ?, time = ? WHERE id = ?";
static const std::string SQL_DELETE_COURT = "DELETE FROM courts WHERE id = ?";

// コート
static const std::string SQL_CREATE_COURT_DATE = "CREATE TABLE IF NOT EXISTS court_dates (id integer unsigned PRIMARY KEY, time text DEFAULT NULL, place text DEFAULT NULL);";
static const std::string SQL_INSERT_COURT_DATE = "INSERT INTO court_dates VALUES (?, ?, ?);";
static const std::string SQL_SELECT_COURT_DATE = "SELECT id, time, place FROM court_dates";
static const std::string SQL_UPDATE_COURT_DATE = "UPDATE court_dates SET time = ?, place = ? WHERE id = ?";
static const std::string SQL_DELETE_COURT_DATE = "DELETE FROM court_dates WHERE id = ?";


namespace Manager {

DataBase* DataBase::dataBase = nullptr;

DataBase::DataBase()
{
}

DataBase* DataBase::getInstance()
{
    if(dataBase == nullptr)
    {
        dataBase = new DataBase();
        dataBase->initialize();
    }
    
    return dataBase;
}

// 値の初期化
void DataBase::initialize()
{
    // DBの初期化
    auto filePath = cocos2d::FileUtils::getInstance()->getWritablePath();
    filePath.append(DB_NAME);
    auto status = sqlite3_open(filePath.c_str(), &_db);
    if (status != SQLITE_OK) {
        cocos2d::log("db open えらー！");
    }
        
    // 各種テーブルの作成
    this->createUserTable();
    this->createCourtTable();
    this->createCourtDateTable();
}
    
void DataBase::resetAll() {
    // テーブルを全削除
    this->dropAll();

    // 作成しなおし
    this->createUserTable();
}
    
    
void DataBase::dropAll() {
    this->dropTable("user");
}


#pragma mark - 共通
#pragma mark SQL文実行
void DataBase::execSql(const char *sql) {
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
    
#pragma mark レコード数取得
int DataBase::checkRecordCount(const char *tableName) {
    int recordCount = 0;
    char sql[100];
    sprintf(sql, "SELECT count(*) AS cnt FROM %s;", tableName);
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int i = 0;
            recordCount = sqlite3_column_int(stmt, i++);
        }
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return recordCount;

}
    
#pragma mark テーブル削除
void DataBase::dropTable(const char *tableName) {
    char sql[100];
    sprintf(sql, "DROP TABLE IF EXISTS %s;", tableName);
    this->execSql(sql);
}
    
    
    
#pragma mark - playerテーブル
/* ======================================================================
 *  [users] テーブル
 *  id            : (integer)識別用ID
 *  name          : (text)氏名
 *  name_furigana : (text)ふりがな
 *  nickname      : (text)ニックネーム
 *  gender        : (text)性別
 *  birthday      : (text)誕生日
 *  level         : (text)レベル
 *  is_entry      : (integer)参加してたか
 *  count         : (integer)参加回数
 *  point         : (integer)ポイント
 *  last_time     : (text)最終参加日
 * ====================================================================== */

void DataBase::createUserTable() {
    // テーブルを作成
    this->execSql(SQL_CREATE_USER.c_str());
    
    // レコードが既に存在していればreturn
    if (this->checkRecordCount("users") != 0) {
        return;
    }
    
    // plist読み込み
    cocos2d::ValueVector vect = cocos2d::FileUtils::getInstance()->getValueVectorFromFile("users.plist");
    for (cocos2d::Value v : vect) {
        cocos2d::ValueMap map = v.asValueMap();
        int id                   = map.at("id").asInt();
        std::string name         = map.at("name").asString();
        std::string nameFurigana = map.at("name_furigana").asString();
        std::string nickname     = map.at("nickname").asString();
        std::string gender       = map.at("gender").asString();
        std::string birthday     = map.at("birthday").asString();
        std::string level        = map.at("level").asString();
        int isEntry              = map.at("is_entry").asInt();
        int count                = map.at("count").asInt();
        int point                = map.at("point").asInt();
        std::string lastTime     = map.at("last_time").asString();
        
        
        sqlite3_stmt *stmt = nullptr;
        if (sqlite3_prepare_v2(_db, SQL_INSERT_USER.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            // ? のところに値をバインド
            int i = 1;
            sqlite3_bind_int   (stmt, i++, id);
            sqlite3_bind_text  (stmt, i++, name.c_str(),         (int)strlen(name.c_str()),         SQLITE_TRANSIENT);
            sqlite3_bind_text  (stmt, i++, nameFurigana.c_str(), (int)strlen(nameFurigana.c_str()), SQLITE_TRANSIENT);
            sqlite3_bind_text  (stmt, i++, nickname.c_str(),     (int)strlen(nickname.c_str()),     SQLITE_TRANSIENT);
            sqlite3_bind_text  (stmt, i++, gender.c_str(),       (int)strlen(gender.c_str()),       SQLITE_TRANSIENT);
            sqlite3_bind_text  (stmt, i++, birthday.c_str(),     (int)strlen(birthday.c_str()),     SQLITE_TRANSIENT);
            sqlite3_bind_text  (stmt, i++, level.c_str(),        (int)strlen(level.c_str()),        SQLITE_TRANSIENT);
            sqlite3_bind_int   (stmt, i++, isEntry);
            sqlite3_bind_int   (stmt, i++, count);
            sqlite3_bind_int   (stmt, i++, point);
            sqlite3_bind_text  (stmt, i++, lastTime.c_str(),     (int)strlen(lastTime.c_str()),     SQLITE_TRANSIENT);
            
            // sql文実行
            sqlite3_step(stmt);
        }
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    }
}
    
std::vector<UserInfo *> DataBase::getUserInfoVec() {
    std::vector<UserInfo *> usesrInfoVec;
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_SELECT_USER.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // sql文実行
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            int i = 0;
            int id                   = sqlite3_column_int(stmt, i++);
            std::string name         = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            std::string nameFurigana = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            std::string nickname     = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            std::string gender       = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            std::string birthday     = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            std::string level        = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            bool isEntry             = (bool)sqlite3_column_int(stmt, i++);
            int count                = sqlite3_column_int(stmt, i++);
            int point                = sqlite3_column_int(stmt, i++);
            std::string lastTime     = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            
            auto userInfo = new UserInfo(id, name, nameFurigana, nickname, gender, birthday, level, isEntry, count, point, lastTime);
            usesrInfoVec.push_back(userInfo);
        }
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    
    return usesrInfoVec;
}
    
void DataBase::addUserInfo(UserInfo *userInfo)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_INSERT_USER.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, userInfo->id);
        sqlite3_bind_text  (stmt, i++, userInfo->name.c_str(),           (int)strlen(userInfo->name.c_str()),           SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->nameFurigana.c_str(),   (int)strlen(userInfo->nameFurigana.c_str()),   SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->nickname.c_str(),       (int)strlen(userInfo->nickname.c_str()),       SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->getGenderStr().c_str(), (int)strlen(userInfo->getGenderStr().c_str()), SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->birthday.c_str(),       (int)strlen(userInfo->birthday.c_str()),       SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->getLevelStr().c_str(),  (int)strlen(userInfo->getLevelStr().c_str()),  SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, (int)userInfo->isEntry);
        sqlite3_bind_int   (stmt, i++, userInfo->count);
        sqlite3_bind_int   (stmt, i++, userInfo->point);
        sqlite3_bind_text  (stmt, i++, userInfo->lastTime.c_str(),       (int)strlen(userInfo->lastTime.c_str()),       SQLITE_TRANSIENT);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
void DataBase::updateUserInfo(UserInfo *userInfo)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_UPDATE_USER.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_text  (stmt, i++, userInfo->name.c_str(),           (int)strlen(userInfo->name.c_str()),           SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->nameFurigana.c_str(),   (int)strlen(userInfo->nameFurigana.c_str()),   SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->nickname.c_str(),       (int)strlen(userInfo->nickname.c_str()),       SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->getGenderStr().c_str(), (int)strlen(userInfo->getGenderStr().c_str()), SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->birthday.c_str(),       (int)strlen(userInfo->birthday.c_str()),       SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, userInfo->getLevelStr().c_str(),  (int)strlen(userInfo->getLevelStr().c_str()),  SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, (int)userInfo->isEntry);
        sqlite3_bind_int   (stmt, i++, userInfo->count);
        sqlite3_bind_int   (stmt, i++, userInfo->point);
        sqlite3_bind_text  (stmt, i++, userInfo->lastTime.c_str(),       (int)strlen(userInfo->lastTime.c_str()),       SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, userInfo->id);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}


void DataBase::removeUserInfo(int id)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_DELETE_USER.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, id);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    

    
    
#pragma mark - courtsテーブル
/* ======================================================================
 *  [courts] テーブル
 *  id            : (integer)識別用ID
 *  game_num      : (integer)第XX試合
 *  court_num     : (integer)第XXコート
 *  member_list   : (text)参加メンバーのIDリストをテキストで保存 例："10,2,4,7"
 *  win_state     : (integer)どっちが勝ったか
 *  history_state : (integer)進行中 or 既に終了
 *  time          : (text)試合開始時間
 * ====================================================================== */

void DataBase::createCourtTable()
{
    // テーブルを作成
    this->execSql(SQL_CREATE_COURT.c_str());
        
    // レコードが既に存在していればreturn
    if (this->checkRecordCount("courts") != 0) {
        return;
    }
}
    
std::vector<CourtInfo *> DataBase::getCourtInfoVec()
{
    std::vector<CourtInfo *> courtInfoVec;
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(_db, SQL_SELECT_COURT.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        
        // sql文実行
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            int i = 0;
            int id                 = sqlite3_column_int(stmt, i++);
            int gameNum            = sqlite3_column_int(stmt, i++);
            int courtNum           = sqlite3_column_int(stmt, i++);
            std::string memberList = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            int winState           = sqlite3_column_int(stmt, i++);
            int historyState       = sqlite3_column_int(stmt, i++);
            std::string time       = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            
            auto courtInfo = new CourtInfo(id, gameNum, courtNum, memberList, winState, historyState, time);
            courtInfoVec.push_back(courtInfo);
        }
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    
    return courtInfoVec;
}
    
void DataBase::addCourtInfo(CourtInfo *courtInfo)
{
    std::string memberListText = courtInfo->convertMemberListToText(courtInfo->memberList);
    
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_INSERT_COURT.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, courtInfo->id);
        sqlite3_bind_int   (stmt, i++, courtInfo->gameNum);
        sqlite3_bind_int   (stmt, i++, courtInfo->courtNum);
        sqlite3_bind_text  (stmt, i++, memberListText.c_str(),  (int)strlen(memberListText.c_str()),    SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, (int)courtInfo->winState);
        sqlite3_bind_int   (stmt, i++, (int)courtInfo->historyState);
        sqlite3_bind_text  (stmt, i++, courtInfo->time.c_str(),  (int)strlen(courtInfo->time.c_str()),  SQLITE_TRANSIENT);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
void DataBase::updateCourtInfo(CourtInfo *courtInfo)
{
    std::string memberListText = courtInfo->convertMemberListToText(courtInfo->memberList);
    
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_UPDATE_COURT.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, courtInfo->gameNum);
        sqlite3_bind_int   (stmt, i++, courtInfo->courtNum);
        sqlite3_bind_text  (stmt, i++, memberListText.c_str(),  (int)strlen(memberListText.c_str()),  SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, (int)courtInfo->winState);
        sqlite3_bind_int   (stmt, i++, (int)courtInfo->historyState);
        sqlite3_bind_text  (stmt, i++, courtInfo->time.c_str(), (int)strlen(courtInfo->time.c_str()), SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, courtInfo->id);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
    
void DataBase::removeCourtInfo(int id)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_DELETE_COURT.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, id);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
 
    
    
    
    
    
    
#pragma mark - court_datesテーブル
/* ======================================================================
 *  [court_dates] テーブル
 *  id    : (integer)識別用ID
 *  time  : (text)試合開始時間
 *  place : (text)場所
 * ====================================================================== */
    
void DataBase::createCourtDateTable()
{
    // テーブルを作成
    this->execSql(SQL_CREATE_COURT_DATE.c_str());
        
    // レコードが既に存在していればreturn
    if (this->checkRecordCount("court_dates") != 0) {
        return;
    }
}
    
std::vector<CourtDateInfo *> DataBase::getCourtDateInfoVec()
{
    std::vector<CourtDateInfo *> courtDateInfoVec;
    sqlite3_stmt *stmt = nullptr;
    
    if (sqlite3_prepare_v2(_db, SQL_SELECT_COURT_DATE.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        
        // sql文実行
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            int i = 0;
            int id            = sqlite3_column_int(stmt, i++);
            std::string time  = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            std::string place = std::string( (const char*)sqlite3_column_text(stmt, i++) );
            
            auto courtDateInfo = new CourtDateInfo(id, time, place);
            courtDateInfoVec.push_back(courtDateInfo);
        }
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    
    return courtDateInfoVec;
}
    
void DataBase::addCourtDateInfo(CourtDateInfo *courtDateInfo)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_INSERT_COURT_DATE.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, courtDateInfo->id);
        sqlite3_bind_text  (stmt, i++, courtDateInfo->time.c_str(),  (int)strlen(courtDateInfo->time.c_str()),  SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, courtDateInfo->place.c_str(), (int)strlen(courtDateInfo->place.c_str()), SQLITE_TRANSIENT);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
void DataBase::updateCourtDateInfo(CourtDateInfo *courtDateInfo)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_UPDATE_COURT_DATE.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_text  (stmt, i++, courtDateInfo->time.c_str(),  (int)strlen(courtDateInfo->time.c_str()), SQLITE_TRANSIENT);
        sqlite3_bind_text  (stmt, i++, courtDateInfo->place.c_str(), (int)strlen(courtDateInfo->place.c_str()), SQLITE_TRANSIENT);
        sqlite3_bind_int   (stmt, i++, courtDateInfo->id);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
    
void DataBase::removeCourtDateInfo(int id)
{
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(_db, SQL_DELETE_COURT_DATE.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int i = 1;
        sqlite3_bind_int   (stmt, i++, id);
        
        // sql文実行
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}
    
    
}   // end namespace Manager

