//
//  TabNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/29.
//
//

#ifndef TabNode_hpp
#define TabNode_hpp

#include "CsbLayerBase.hpp"

class TabNode : public CsbLayerBase
{
public:
    static TabNode *create();

    void changeSelectedTab(TabType tabType);

protected:
    // コンストラクタ／デストラクタ
    TabNode();
    virtual ~TabNode();
    
    // 初期化
    virtual bool init();
    
    // ボタン
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void setTabButtonEnabled(TabType tabType, bool enabled);
    TabType currentTabType;
};
#endif /* TabNode_hpp */
