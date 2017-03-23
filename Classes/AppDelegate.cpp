#include "AppDelegate.h"
#include "MainScene.hpp"
#include "UserDefaultUtil.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // デバッグ
    director->setDisplayStats(false);

    // 60FPS
    director->setAnimationInterval(1.0 / 60);
    
    // 画面サイズ設定
    auto size = Director::getInstance()->getWinSize();
    float width  = 640.0f;
    float height = size.height / size.width * width;
    glview->setDesignResolutionSize(width, height, ResolutionPolicy::SHOW_ALL);

    
    // ファイルパス設定
    FileUtils::getInstance()->addSearchPath("plist");
    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("res/Default");
    FileUtils::getInstance()->addSearchPath("res/Font");
    FileUtils::getInstance()->addSearchPath("res/Image");
    FileUtils::getInstance()->addSearchPath("res/Image/Icon");
    FileUtils::getInstance()->addSearchPath("res/Image/Button");
    
    FileUtils::getInstance()->addSearchPath("res/Scene");
    FileUtils::getInstance()->addSearchPath("res/Scene/Member");
    FileUtils::getInstance()->addSearchPath("res/Scene/Participant");
    FileUtils::getInstance()->addSearchPath("res/Scene/Court");
    FileUtils::getInstance()->addSearchPath("res/Scene/Setting");
    FileUtils::getInstance()->addSearchPath("res/Scene/Overlay");
    FileUtils::getInstance()->addSearchPath("res/Scene/DrawerMenu");

    
    // 値の初期化
    Kyarochon::UserDefault::initInt(UD_KEY_COURT_NUM, 1);
    Kyarochon::UserDefault::initInt(UD_KEY_SORT_TYPE, (int)SortType::Syllabary);
    Kyarochon::UserDefault::initInt(UD_KEY_SHUFFLE_TYPE, (int)ShuffleType::RandomPair);
    Kyarochon::UserDefault::initInt(UD_KEY_ALLOCATION_TYPE, (int)AllocationType::All);
    Kyarochon::UserDefault::initBool(UD_KEY_FILTER_MALE, true);
    Kyarochon::UserDefault::initBool(UD_KEY_FILTER_FEMALE, true);
    Kyarochon::UserDefault::initBool(UD_KEY_FILTER_TRAINING, true);
    Kyarochon::UserDefault::initBool(UD_KEY_FILTER_BEGINNER, true);
    Kyarochon::UserDefault::initBool(UD_KEY_FILTER_MIDDLE, true);
    Kyarochon::UserDefault::initBool(UD_KEY_FILTER_HIGHER, true);
    Kyarochon::UserDefault::initInt(UD_KEY_FILTER_DAYS, 0);

    Kyarochon::UserDefault::flush();
    
    
    // メインシーン開始
    auto scene = MainScene::createScene();
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
