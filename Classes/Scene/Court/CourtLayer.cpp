//
//  CourtLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/29.
//
//

#include "CourtLayer.hpp"
#include "CourtItemNode.hpp"
#include "MemberManager.hpp"
#include "CourtManager.hpp"
#include "LayoutUtil.h"

USING_NS_CC;
using namespace cocostudio;

CourtLayer::CourtLayer()
:rightButton(nullptr)
,leftButton(nullptr)
,lockedStartGame(false)
{
    Kyarochon::Event::setEventListener(EVENT_UPDATE_COURT_LIST, CC_CALLBACK_1(CourtLayer::eventUpdateCourtList, this));
}

CourtLayer::~CourtLayer() {
    Kyarochon::Event::removeEventListener(EVENT_UPDATE_COURT_LIST);
}


#pragma mark - 初期化
CourtLayer *CourtLayer::create()
{
    CourtLayer *ret = new CourtLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CourtLayer::init()
{
    std::string csbName = "CourtLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    
    // テキスト初期化
    auto headerPanel = mainLayer->getChildByName<ui::Layout *>("HeaderPanel");
    headerPanel->getChildByName<ui::Text *>("TextGameNum")->setString("-----");

    // ボタン初期化
    rightButton = headerPanel->getChildByName<ui::Button *>("ButtonRight");
    leftButton  = headerPanel->getChildByName<ui::Button *>("ButtonLeft");
    this->addButtonEvent(rightButton, ButtonTag::Right);
    this->addButtonEvent(leftButton, ButtonTag::Left);
    this->addButtonEvent(headerPanel->getChildByName<ui::Button *>("ButtonStartGame"), ButtonTag::Start);
//    this->addButtonEvent(mainLayer->getChildByName<ui::Button *>("ButtonSpeech"), ButtonTag::Speech);
    
    // 選択されている履歴表示
    this->showCourtHistory();
    
    currentSelectedMembers.clear();

    return true;
}

void CourtLayer::pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    auto courtManager = Manager::Court::getInstance();
    if (button->getTag() == ButtonTag::Right)
    {
        courtManager->incrementSelectedGameNum();
        this->showCourtHistory();
    }
    else if (button->getTag() == ButtonTag::Left)
    {
        courtManager->decrementSelectedGameNum();
        this->showCourtHistory();
    }
    else if (button->getTag() == ButtonTag::Start)
    {
        this->showNewCourt();
    }
    else if (button->getTag() == ButtonTag::Speech)
    {
        auto selectedCourtList = Manager::Court::getInstance()->getSelectedCourtList();
        if (selectedCourtList.empty())return;
        
        std::string speechText = this->makeSpeechText(selectedCourtList);
        NativeBridge::readSpeech(speechText.c_str());
    }
}

void CourtLayer::initScrollView(int itemNum)
{
    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    
    float scrollHeight  = Kyarochon::Layout::getScrollHeight(HEADER_HEIGHT, TAB_HEIGHT, SCROLL_MARGIN);
    float contentHeight = COURT_ITEM_HEIGHT * itemNum;
    if (scrollHeight > contentHeight)
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, contentHeight));
    }
    else
    {
        scrollView->setContentSize(Size(scrollView->getContentSize().width, scrollHeight));
    }
    scrollView->setInnerContainerSize(Size(scrollView->getInnerContainerSize().width, contentHeight));
    scrollView->removeAllChildren();
}


void CourtLayer::showNewCourt()
{
    if (this->lockedStartGame) return;
    this->lockedStartGame = true;
    
    auto courtManager  = Manager::Court::getInstance();
    auto memberManager = Manager::Member::getInstance();
    
    
    // コート情報生成＆表示
    int i = 0;
    currentSelectedMembers = memberManager->selectNextMatchMembers(courtManager->getUseCourtNum());
    if (currentSelectedMembers.empty()) return;
    
    // ゲームカウント+1
    courtManager->incrementGameNum();
    
    // スクロールビューサイズ調整
    this->initScrollView((int)currentSelectedMembers.size());
    
    for (auto courtMembers : currentSelectedMembers)
    {
        auto courtInfo = courtManager->getNextCourt(i, courtMembers);
        this->showCourtView(courtInfo);
        
        i++;
    }
    this->updateArrowButton();
    
    
    // 確認アラートを表示
    std::string text = "この組み合わせで試合を開始しますか？";
    std::function<void()> yesCallback = [this](){this->startNewGame();};
    std::function<void()> noCallback  = [this](){this->cancelNewGame();};
    ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
    Kyarochon::Event::showConfirmView(info);
}


void CourtLayer::startNewGame()
{
    this->lockedStartGame = false;

    // 読み上げテキストを生成
    std::string speechText = this->makeSpeechText(currentSelectedMembers);
    
    // メンバーの参加回数に追加
    Manager::Member::getInstance()->addGameMemberInfoList(currentSelectedMembers);
    currentSelectedMembers.clear();
    
    // 初回の場合は記録開始するか確認する
    if (Manager::Court::getInstance()->getSelectedGameNum() == 0)
    {
        // 確認アラートを表示
        std::string text = "試合の記録を開始しますか？";
        std::function<void()> yesCallback = [this](){
            Manager::Member::getInstance()->updateParticipantCount();
        };
        std::function<void()> noCallback  = [this](){
        };
        ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
        Kyarochon::Event::showConfirmView(info);
    }
    
    // 試合内容を読み上げ（一旦削除）
//    NativeBridge::readSpeech(speechText.c_str());
}

void CourtLayer::cancelNewGame()
{
    this->lockedStartGame = false;
    
    // 直前のコート情報削除
    Manager::Court::getInstance()->removeCurrentCourt();
    
    // ひとつ前の履歴表示
    this->showCourtHistory();
    
    currentSelectedMembers.clear();
}


void CourtLayer::showCourtHistory()
{
    auto selectedCourtList = Manager::Court::getInstance()->getSelectedCourtList();
    
    // スクロールビューサイズ調整
    this->initScrollView((int)selectedCourtList.size());

    for (auto courtInfo : selectedCourtList)
    {
        this->showCourtView(courtInfo);
    }
    this->updateArrowButton();
}

void CourtLayer::showCourtView(CourtInfo *courtInfo)
{
    // 試合数表示
    int gameNum = Manager::Court::getInstance()->getSelectedGameNum();
    auto headerPanel = mainLayer->getChildByName<ui::Layout *>("HeaderPanel");
    headerPanel->getChildByName<ui::Text *>("TextGameNum")->setString(StringUtils::format("第%d試合", gameNum + 1));
    
    // スクロールビューに追加
    auto scrollView = mainLayer->getChildByName<ui::ScrollView *>("ScrollView");
    auto innnerSize = scrollView->getInnerContainerSize();
    
    auto courtItemNode = CourtItemNode::create(courtInfo);
    float x = COURT_ITEM_WIDTH / 2.0f + 50.0f;
    float y = innnerSize.height - (courtInfo->courtNum + 0.5f) * COURT_ITEM_HEIGHT + (DEFAULT_SCREEN_HEIGHT - Kyarochon::Layout::getScaledHeight());
    courtItemNode->setPosition(Vec2(x, y));
    scrollView->addChild(courtItemNode);
}

void CourtLayer::updateArrowButton()
{
    auto courtManager = Manager::Court::getInstance();
    
    leftButton->setVisible(!courtManager->isSelectedFirstGame());
    rightButton->setVisible(!courtManager->isSelectedLastGame());
}

std::string CourtLayer::makeSpeechText(std::vector< std::vector<UserInfo *> > memberListVec)
{
    std::string text = "";
    for (int i = 0; i < memberListVec.size(); i++)
    {
        text += this->makeSpeechTextEachCourt(i + 1, memberListVec.at(i));
    }
    return text;
}

std::string CourtLayer::makeSpeechText(std::vector<CourtInfo *> courtInfoList)
{
    std::string text = "";
    for (auto courtInfo : courtInfoList)
    {
        text += this->makeSpeechTextEachCourt(courtInfo->courtNum + 1, courtInfo->memberList);
    }
    return text;
}

std::string CourtLayer::makeSpeechTextEachCourt(int courtNum, std::vector<UserInfo *> memberList)
{
    std::string text = "";
    auto server       = memberList.at(0)->nameFurigana;
    auto serverPair   = memberList.at(1)->nameFurigana;
    auto receiver     = memberList.at(2)->nameFurigana;
    auto receiverPair = memberList.at(3)->nameFurigana;

    text += "第" + StringUtils::toString(courtNum) + "コート、";
    text += server + "、" + serverPair + "ペア、たい、" + receiver + "、"+ receiverPair + "ペア、";
    text += "、サーブ、" + server;
    text += "、レシーブ、" + receiver;
    text += "、、、";
    
    return text;
}

void CourtLayer::eventUpdateCourtList(cocos2d::EventCustom *event)
{
    this->showCourtHistory();
}
