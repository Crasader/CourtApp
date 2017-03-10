//
//  ParticipantLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/29.
//
//

#ifndef ParticipantLayer_hpp
#define ParticipantLayer_hpp

#include "CsbLayerBase.hpp"

class ParticipantLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Setting,
    };

    
public:
    static ParticipantLayer *create();
    
protected:
    ParticipantLayer();
    virtual ~ParticipantLayer();
    bool init() override;
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type) override;

    
private:
    // スクロールビュー
    cocos2d::ui::ScrollView *scrollView;

    // 表示更新
    void updateParticipantList();
    
    // イベント
    void eventUpdateParticipantList(EventCustom *event);
};


#endif /* ParticipantLayer_hpp */
