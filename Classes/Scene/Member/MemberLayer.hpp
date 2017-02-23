//
//  MemberLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/20.
//
//

#ifndef MemberLayer_hpp
#define MemberLayer_hpp

#include "CsbLayerBase.hpp"
#include "SearchDrawerLayer.hpp"
#include "MemberItemNode.hpp"
#include "TabNode.hpp"

class MemberLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Sort,
        AddMember,
    };
    
    
public:
    static MemberLayer *create();
    void setTabNode(TabNode *tabNode){this->tabNode = tabNode;}
    void show();
    
protected:
    MemberLayer();
    virtual ~MemberLayer();
    bool init() override;
    
    // ボタン
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type) override;

private:
    
    TabNode *tabNode;
    
    // 検索／ソート用
    SearchDrawerLayer *searchDrawerLayer;
    bool isShowingSearchDrawerLayer;
    bool isMovingDrawer;
    
    // メンバーノード
    cocos2d::Vector<MemberItemNode *> memberItemNodeList;
    MemberItemNode *getMemberItemNodeAt(int memberId);
    
    
private:
    // 表示更新
    void updateMemberList();
    
    // イベント
    void eventUpdateMemberList(EventCustom *event);

    //
    void moveSearchDrawerLayer();
    
};


#endif /* MemberLayer_hpp */
