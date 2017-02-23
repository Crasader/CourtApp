//
//  CsbLayerBase.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/20.
//
//

#include "CsbLayerBase.hpp"

USING_NS_CC;
using namespace cocostudio;


CsbLayerBase::CsbLayerBase()
:mainLayer(nullptr)
{
}

CsbLayerBase::~CsbLayerBase() {
}


#pragma mark - 初期化
bool CsbLayerBase::init(std::string csbName, bool isFullSize)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    mainLayer = static_cast<Layer *>(CSLoader::getInstance()->createNode(csbName));

    auto layerSize = mainLayer->getContentSize();
    auto winSize = Director::getInstance()->getWinSize();
    
    cocos2d::log("layerSize : %f x %f", layerSize.width, layerSize.height);
    cocos2d::log("winSize : %f x %f", winSize.width, winSize.height);
    if (isFullSize)
    {
        float ratio = DEFAULT_SCREEN_WIDTH / winSize.width;
        float scaledHeight = winSize.height * ratio;
        
        mainLayer->setScale(ratio);
        mainLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        mainLayer->setPositionY(scaledHeight - DEFAULT_SCREEN_HEIGHT);
        
/*
        float widthRatio  = winSize.width / layerSize.width;
        float heightRatio = winSize.height / layerSize.height;
        if (widthRatio < heightRatio)
        {
        	mainLayer->setScale(widthRatio);
            mainLayer->setPositionY((winSize.height - layerSize.height * widthRatio) / 2.0f);
        }
        else
        {
        	mainLayer->setScale(heightRatio);
            mainLayer->setPositionX((winSize.width - layerSize.width * heightRatio) / 2.0f);
        }
 */
    }    

    this->addChild(mainLayer);

    return true;
}



#pragma mark - Buton
void CsbLayerBase::addButtonEvent(ui::Button *button, int tag)
{
    button->setTag(tag);
    button->addTouchEventListener(CC_CALLBACK_2(CsbLayerBase::pushedButton, this));
}


void CsbLayerBase::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    auto button = dynamic_cast<ui::Button *>(pSender);
    
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            touchStartPos = button->getWorldPosition();
            touchStartTime = clock();
        }
            break;
            
        case ui::Widget::TouchEventType::ENDED:
        {
            // タッチ中にスクロールしてたらreturn
            auto touchEndPos = button->getWorldPosition();
            if (touchStartPos.distance(touchEndPos) > 5.0f) return;
            
            // タッチ継続時間で変わる
            float time = (float)(clock() - touchStartTime) / (float)CLOCKS_PER_SEC;
            if (time > LONG_TAP_TIME)
                this->longTappedButton(button->getTag());
            else
                this->shortTappedButton(button->getTag());
            break;
        }

        case ui::Widget::TouchEventType::MOVED:
        case ui::Widget::TouchEventType::CANCELED:
            break;
    }

}

void CsbLayerBase::setButtonEnabled(cocos2d::ui::Button *button, bool enabled)
{
    button->setEnabled(enabled);
    button->setHighlighted(!enabled);
}

#pragma mark - CheckBox
void CsbLayerBase::addCheckBoxEvent(cocos2d::ui::CheckBox *checkBox, int tag)
{
    checkBox->setTag(tag);
    checkBox->addEventListener(CC_CALLBACK_2(CsbLayerBase::pushedCheckBox, this));
}


void CsbLayerBase::pushedCheckBox(Ref *pSender, ui::CheckBox::EventType type)
{
    auto checkBox = dynamic_cast<ui::CheckBox *>(pSender);
    int tag = checkBox->getTag();
    switch (type) {
        case ui::CheckBox::EventType::SELECTED:
            CCLOG("CheckBox[%d] Selected", tag);
            break;
        case ui::CheckBox::EventType::UNSELECTED:
            CCLOG("CheckBox[%d] UnSelected", tag);
    }
}

#pragma mark - EditBox
ui::EditBox *CsbLayerBase::makeEditBox(Vec2 pos, std::string placeHolder, Node *parent)
{
    auto editBox = ui::EditBox::create(Size(280.0f, 42.0f),
                                       ui::Scale9Sprite::create("bg_search_black.png"));
    editBox->setPosition(pos);
    editBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    editBox->setFont("res/Font/rounded-x-mplus-1c-heavy.ttf", 26.0f);
    editBox->setPlaceHolder(placeHolder.c_str());
    editBox->setPlaceholderFont("res/Font/rounded-x-mplus-1c-heavy.ttf", 26.0f);
    editBox->setFontColor(Color3B::BLACK);
    
    if (parent)
    {
        parent->addChild(editBox);
    }
    else
    {
        this->mainLayer->addChild(editBox);
    }
    
    return editBox;
}
