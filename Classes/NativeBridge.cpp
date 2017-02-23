//
//  NativeBridge.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/10/27.
//
//

#include "NativeBridge.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
// パッケージ名と Java クラス名
//#define CLASS_NAME "jp/access_app/toysrush/AppActivity"
#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"





// ====== getStaticMethodInfo(methodInfo, クラス名, メソッド名, 引数／返り値); ======
// 第4引数：(引数)返り値 (I)V = void test(int value)
// 参考：http://gaomar.blog.fc2.com/blog-entry-81.html
// ============================================================================
// CallStaticVoidMethod(t.classID, t.methodID, 引数);

void NativeBridge::readSpeech(const char *text)
{
    // Ljava/lang/StringLjava/lang/StringZ ＝ char* char* boolean
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "readSpeech", "(Ljava/lang/String;)V")) {
        jstring jText  = t.env->NewStringUTF(text);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jText);
        t.env->DeleteLocalRef(jText);
        t.env->DeleteLocalRef(t.classID);
    }
}


void NativeBridge::stopSpeech()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopSpeech", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }

}



// 課金関連
/*
void NativeBridge::makePaymentRequest(int productIndex) {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "makePaymentRequest", "(I)V")) {
     t.env->CallStaticVoidMethod(t.classID, t.methodID, (jint)productIndex);						// CallStaticVoidMethod(t.classID, t.methodID, 引数);
     t.env->DeleteLocalRef(t.classID);
     }
}
void NativeBridge::validateProductIdentifiers() {
}
int NativeBridge::getItemPriceAt(int productIndex) {
    return 0;
}

std::string NativeBridge::getItemPriceTextAt(int productIndex) {
    cocos2d::JniMethodInfo t;
    std::string priceText = "";
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getItemPriceTextAt", "(I)Ljava/lang/String;")) {
     //		priceText = t.env->CallStaticIntMethod(t.classID, t.methodID, productIndex);
     jstring jPriceText = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, (jint)productIndex);
     const char* str = t.env->GetStringUTFChars(jPriceText, NULL);
     priceText = str;
     
     t.env->ReleaseStringUTFChars(jPriceText, str);
     t.env->DeleteLocalRef(t.classID);
     }
    return priceText;
    
}

void NativeBridge::checkOwnedItem() {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "checkOwnedItem", "()V")) {
     t.env->CallStaticVoidMethod(t.classID, t.methodID);
     t.env->DeleteLocalRef(t.classID);
     }
}


// バージョン取得
std::string NativeBridge::getVersion() {
    return "test";
}

bool NativeBridge::isConnected() {
    bool isConnected = false;
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "isConnected", "()Z")) {
        isConnected = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    return isConnected;
}


// シェア関連
void NativeBridge::share(ShareMedia shareMedia, const char *filePath, const char *shareText, ShareType shareType) {
    const char *methodName;
    switch (shareMedia) {
        case ShareMedia::LINE:
            methodName = "lineShare";
            break;
        case ShareMedia::FACEBOOK:
            methodName = "facebookShare";
            break;
        case ShareMedia::TWITTER:
            methodName = "twitterShare";
            break;
    }
     // Ljava/lang/StringLjava/lang/StringZ ＝ char* char* boolean
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, methodName, "(Ljava/lang/String;Ljava/lang/String;ZI)V")) {
     jstring jFilePath  = t.env->NewStringUTF(filePath);
     jstring jShareText = t.env->NewStringUTF(shareText);
     t.env->CallStaticVoidMethod(t.classID, t.methodID, jFilePath, jShareText, giveBonus, (jint)pictureType);
     t.env->DeleteLocalRef(jFilePath);
     t.env->DeleteLocalRef(jShareText);
     t.env->DeleteLocalRef(t.classID);
     }
}

// レビュー
void NativeBridge::showRVAlert() {
}
void NativeBridge::accessRVPage() {
     KwmrSystemManager::getInstance()->enterBackground = true;
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "accessReviewPage", "()V")) {
     t.env->CallStaticVoidMethod(t.classID, t.methodID);
     t.env->DeleteLocalRef(t.classID);
     }
}


// 画像保存
void NativeBridge::savePicture(const char *filePath) {
    
}


// 通知登録
void NativeBridge::registerNotification() {
    // android 登録の必要無し
}
int NativeBridge::getNoticeType() {
    return 7;
}
void NativeBridge::setNextNotification(int remTime, const char *notifText) {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setNextNotification", "(I)V")) {
     t.env->CallStaticVoidMethod(t.classID, t.methodID, (jint)remTime);
     t.env->DeleteLocalRef(t.classID);
     }
}
void NativeBridge::setDailyNotification(const char *notifText) {
}
void NativeBridge::cancelAllNotifications() {
}
void NativeBridge::showPushSettingAlert() {
}

// 広告関連
void NativeBridge::showUulWall() {
}


//GA
void NativeBridge::gaStartTracking() {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "gaStartTracking", "()V")) {
     t.env->CallStaticVoidMethod(t.classID, t.methodID);
     t.env->DeleteLocalRef(t.classID);
     }
}
void NativeBridge::gaSendViewTracking(const char *screenName) {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "gaSendViewTracking", "(Ljava/lang/String;)V")) {
     jstring jScreenName = t.env->NewStringUTF(screenName);
     t.env->CallStaticVoidMethod(t.classID, t.methodID, jScreenName);
     t.env->DeleteLocalRef(jScreenName);
     t.env->DeleteLocalRef(t.classID);
     }
}
void NativeBridge::gaSendEventTrackingEventWithParams(const char *category, const char *action, const char *label) {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "gaSendEventTrackingEventWithParams", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
     jstring jCategory  = t.env->NewStringUTF(category);
     jstring jAction  = t.env->NewStringUTF(action);
     jstring jLabel = t.env->NewStringUTF(label);
     t.env->CallStaticVoidMethod(t.classID, t.methodID, jCategory, jAction, jLabel);
     t.env->DeleteLocalRef(t.classID);
     }
}
void NativeBridge::gaSendEventTrackingEventWithParams(const char *category, const char *action, const char *label, int value) {
     cocos2d::JniMethodInfo t;
     if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "gaSendEventTrackingEventWithParams", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V")) {
     jstring jCategory  = t.env->NewStringUTF(category);
     jstring jAction  = t.env->NewStringUTF(action);
     jstring jLabel = t.env->NewStringUTF(label);
     t.env->CallStaticVoidMethod(t.classID, t.methodID, jCategory, jAction, jLabel, (jint)value);
     t.env->DeleteLocalRef(t.classID);
     }
}


// 音楽再生関連
void NativeBridge::changeBgmSetting(bool isMute) {
}
*/


//
