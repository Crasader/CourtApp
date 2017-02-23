//
//  LayoutUtil.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2017/02/20.
//
//

#ifndef LayoutUtil_h
#define LayoutUtil_h

#include "LayoutDefine.h"
#include "cocos2d.h"


namespace Kyarochon {
namespace Layout {


// スケーリング後の高さを取得
static float getScaledHeight()
{
    auto winSize = cocos2d::Director::getInstance()->getWinSize();
    
    float ratio = DEFAULT_SCREEN_WIDTH / winSize.width;
    float scaledHeight = winSize.height * ratio;
    
    cocos2d::log("winSize : %5.1f x %5.1f → %5.1f x %5.1f", winSize.width, winSize.height, DEFAULT_SCREEN_WIDTH, scaledHeight);
    
    return scaledHeight;
}

// スクロールビューの高さを取得
static float getScrollHeight(float headerHeight, float tabHeight, float scrollMargin)
{
    float scrollHeight = getScaledHeight() - headerHeight - tabHeight - scrollMargin * 2.0f;
    return scrollHeight;
}

}   // end namespace Layout
}   // end namespace Kyarochon

#endif /* LayoutUtil_h */
