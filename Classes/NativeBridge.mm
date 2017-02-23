//
//  NativeBridge.m
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/10/27.
//
//

#include "NativeBridge.h"

#include "ReadSpeech_objc.h"

/*
#include "InAppPurchase_objc.h"
#include "ShareUtil_objc.h"
#include "GoogleAnalyticsTracker_objc.h"
#include "Util_objc.h"
#include "AdUtil_objc.h"
*/

#pragma mark - 音声読み上げ
void NativeBridge::readSpeech(const char *text)
{
    [[ReadSpeech_objc getInstance] speech:[NSString stringWithUTF8String:text]];
}

void NativeBridge::stopSpeech()
{
    [[ReadSpeech_objc getInstance] stop];
}


/*
#pragma mark - 課金関連
void NativeBridge::validateProductIdentifiers() {
    [[InAppPurchase_objc sharedManager] validateProductIdentifiers];
}

int NativeBridge::getItemPriceAt(int productIndex) {
    return [[InAppPurchase_objc sharedManager] getItemPriceAt:productIndex];
}

void NativeBridge::makePaymentRequest(int productIndex)
{
    [[InAppPurchase_objc sharedManager] makePaymentRequest:productIndex];
}

std::string NativeBridge::getItemPriceTextAt(int productIndex) {
    // androidのみ
    return "";
}

void NativeBridge::checkOwnedItem() {
    // androidのみ
}


#pragma mark - バージョン取得
std::string NativeBridge::getVersion() {
    NSString *versionNo = [Util_objc getVersion];
    std::string ret([versionNo UTF8String]);
    return ret;
}

bool NativeBridge::isConnected() {
    return true;
}


#pragma mark シェア
void NativeBridge::share(ShareMedia shareMedia, const char *filePath, const char *shareText, ShareType shareType) {
    CGRect trimArea;
    switch (shareType) {
        case ShareType::GAME:
            trimArea = CGRectMake(0, 0, DEVICE_WIDTH * 2, DEVICE_HEIGHT * 2);
            break;
        case ShareType::EVOLUTION:
            trimArea = CGRectMake(35, 135, 569, 505 - (IPHONE_5_HEIGHT - DEVICE_HEIGHT * 2));
            break;
        case ShareType::DETAIL:
            trimArea = CGRectMake(35, 145 - (IPHONE_5_HEIGHT - DEVICE_HEIGHT * 2) / 2, 569, 690);
            break;
    }
    
    
    switch (shareMedia) {
        case ShareMedia::LINE:
            [ShareUtil_objc lineShare:[NSString stringWithUTF8String:filePath]
                             withText:[NSString stringWithUTF8String:shareText]
                         withTrimArea:trimArea];
            break;
        case ShareMedia::FACEBOOK:
            [ShareUtil_objc facebookShare:[NSString stringWithUTF8String:filePath]
                                 withText:[NSString stringWithUTF8String:shareText]
                             withTrimArea:trimArea];
            break;
        case ShareMedia::TWITTER:
            [ShareUtil_objc twitterShare:[NSString stringWithUTF8String:filePath]
                                withText:[NSString stringWithUTF8String:shareText]
                            withTrimArea:trimArea];
            break;
    }
}


#pragma mark - レビュー
void NativeBridge::accessRVPage() {
    [Util_objc accessRVPage];
}
void NativeBridge::showRVAlert() {
    [Util_objc showRVAlert];
}


#pragma mark - 画像保存
void NativeBridge::savePicture(const char *filePath) {
    [Util_objc savePicture:[NSString stringWithUTF8String:filePath]];
    
}



#pragma mark - 通知
void NativeBridge::registerNotification() {
    [Util_objc registerNotification];
}
int NativeBridge::getNoticeType() {
    return [Util_objc  getNoticeType];
}

void NativeBridge::setNextNotification(int remTime, const char *notifText) {
    [Util_objc setNextNotification:remTime withText:[NSString stringWithUTF8String:notifText]];
}

void NativeBridge::setDailyNotification(const char *notifText) {
    [Util_objc setDailyNotification:[NSString stringWithUTF8String:notifText]];
}

void NativeBridge::cancelAllNotifications() {
    [Util_objc cancelAllNotifications];
}

void NativeBridge::showPushSettingAlert() {
    [Util_objc showPushSettingAlert];
}




#pragma mark - 広告
void NativeBridge::showUulWall() {
    [AdUtil_objc showUulWall];
}




#pragma mark - GA
void NativeBridge::gaStartTracking() {
    [GoogleAnalyticsTracker_objc startTracking];
}
void NativeBridge::gaSendViewTracking(const char *screenName) {
    [GoogleAnalyticsTracker_objc sendViewTracking:[NSString stringWithUTF8String:screenName]];
}
void NativeBridge::gaSendEventTrackingEventWithParams(const char *category, const char *action, const char *label) {
    [GoogleAnalyticsTracker_objc sendEventTrackingEventWithCategory:[NSString stringWithUTF8String:category]
                                                         withAction:[NSString stringWithUTF8String:action]
                                                          withLabel:[NSString stringWithUTF8String:label]];
}
void NativeBridge::gaSendEventTrackingEventWithParams(const char *category, const char *action, const char *label, int value) {
    [GoogleAnalyticsTracker_objc sendEventTrackingEventWithCategory:[NSString stringWithUTF8String:category]
                                                         withAction:[NSString stringWithUTF8String:action]
                                                          withLabel:[NSString stringWithUTF8String:label]
                                                          withValue:@(value)    ];
}



#pragma mark - 音楽再生
void NativeBridge::changeBgmSetting(bool isMute) {
    [Util_objc changeBgmSetting:isMute];
}
*/
