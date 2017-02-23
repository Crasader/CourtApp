//
//  DatabaseManager.h
//  SaltyIdol
//
//  Created by saitou kiyotaka on 2015/06/29.
//
//

#ifndef __SaltyIdol__DatabaseManager__
#define __SaltyIdol__DatabaseManager__

#include "cocos2d.h"
#include "sqlite3.h"

#include "UserInfo.hpp"
#include "CourtInfo.hpp"
#include "CourtDateInfo.hpp"

namespace Manager {

class DataBase {

private:
    DataBase();
    static DataBase* dataBase;
    sqlite3 *_db;
    
    // ---------- テーブルの作成 -----------
    void createUserTable();
    void createCourtTable();
    void createCourtDateTable();
    
    void dropAll();
    
    // ----------- 共通処理 ---------------
    // SQL文の実行
    void execSql(const char *sql);
    
    // レコード数を確認
    int checkRecordCount(const char *tableName);
  
    // テーブルの削除
    void dropTable(const char *tableName);

    
public:
    // 初期化
    static DataBase* getInstance();
    void initialize();
        
    void resetAll();

    
    // *************** メンバー ***************
    // データの取得
    std::vector<UserInfo *> getUserInfoVec();
    
    // データの追加
    void addUserInfo(UserInfo *userInfo);
    
    // データの更新
    void updateUserInfo(UserInfo *userInfo);
    
    // データの削除
    void removeUserInfo(int id);
    
    
    // *************** コート履歴 ***************
    // データの取得
    std::vector<CourtInfo *> getCourtInfoVec();
    
    // データの追加
    void addCourtInfo(CourtInfo *courtInfo);
    
    // データの更新
    void updateCourtInfo(CourtInfo *courtInfo);
    
    // データの削除
    void removeCourtInfo(int id);
    
    
    // *************** 日付別コートリスト ***************
    // データの取得
    std::vector<CourtDateInfo *> getCourtDateInfoVec();
    
    // データの追加
    void addCourtDateInfo(CourtDateInfo *courtDateInfo);
    
    // データの更新
    void updateCourtDateInfo(CourtDateInfo *courtDateInfo);
    
    // データの削除
    void removeCourtDateInfo(int id);
};
    

}   // end namespace Manager


#endif /* defined(__SaltyIdol__DatabaseManager__) */
