//
//  NativeBridge.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/10/27.
//
//

#ifndef NativeBridge_h
#define NativeBridge_h

class NativeBridge
{
public:
    // 音声読み上げ
    static void readSpeech(const char *text);
    static void stopSpeech();
    
    
/*
    // 課金関連
    static void makePaymentRequest(int productIndex);
    static void validateProductIdentifiers();
    static int getItemPriceAt(int productIndex);
    static std::string getItemPriceTextAt(int productIndex);
    static void checkOwnedItem();
    
    // バージョン取得
    static std::string getVersion();
    static bool isConnected();
    
    // シェア関連
    static void share(ShareMedia shareMedia, const char *filePath, const char *shareText, ShareType shareType);
    
    // レビュー
    static void showRVAlert();
    static void accessRVPage();
    
    // 画像保存
    static void savePicture(const char *filePath);
    
    // 通知登録
    static void registerNotification();
    static int getNoticeType();
    static void setNextNotification(int remTime, const char *notifText);
    static void setDailyNotification(const char *notifText);
    static void cancelAllNotifications();
    static void showPushSettingAlert();
    
    // 広告関連
    static void showUulWall();
    
    //GA
    static void gaStartTracking();
    static void gaSendViewTracking(const char *screenName);
    static void gaSendEventTrackingEventWithParams(const char *category, const char *action, const char *label);
    static void gaSendEventTrackingEventWithParams(const char *category, const char *action, const char *label, int value);
    
    
    // 音楽再生関連
    static void changeBgmSetting(bool isMute);
 */
};


#endif /* defined(NativeBridge_h) */

