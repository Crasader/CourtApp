//
//  MemberEditLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/03.
//
//

#include "MemberEditLayer.hpp"
#include "MemberManager.hpp"
#include "GameHistoryItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;

MemberEditLayer::MemberEditLayer()
:nameEditBox(nullptr)
,nameFuriganaEditBox(nullptr)
,nicknameEditBox(nullptr)
,maleCheckBox(nullptr)
,femaleCheckBox(nullptr)
,trainingCheckBox(nullptr)
,beginnerCheckBox(nullptr)
,middleCheckBox(nullptr)
,higherCheckBox(nullptr)
,countEditBox(nullptr)
,lastTimeEditBox(nullptr)
,deleteButton(nullptr)
,saveButton(nullptr)
,backButton(nullptr)
{
}

MemberEditLayer::~MemberEditLayer()
{
}





#pragma mark - 初期化
MemberEditLayer *MemberEditLayer::create(UserInfo *userInfo)
{
    MemberEditLayer *ret = new MemberEditLayer();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool MemberEditLayer::init(UserInfo *userInfo)
{
    std::string csbName = "MemberEditLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->userInfo = userInfo;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    auto headerPanel = panel->getChildByName<ui::Layout *>("HeaderPanel");
    
    float x = 295.0f;
    float y = 937.0f;
    nameEditBox = this->makeEditBox(Vec2(x, y), "[必須]氏名");
    y -= 80.0f;
    nameFuriganaEditBox = this->makeEditBox(Vec2(x, y), "[必須]ふりがな");
    y -= 80.0f;
    nicknameEditBox = this->makeEditBox(Vec2(x, y), "[任意]ニックネーム");

    auto contentPanel = panel->getChildByName<ui::Layout *>("PanelContent");
    maleCheckBox     = contentPanel->getChildByName<ui::CheckBox *>("CheckBoxGenderMale");
    femaleCheckBox   = contentPanel->getChildByName<ui::CheckBox *>("CheckBoxGenderFemale");
    trainingCheckBox = contentPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelTraining");
    beginnerCheckBox = contentPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelBeginner");
    middleCheckBox  = contentPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelMiddle");
    higherCheckBox   = contentPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelHigher");
    
    y -= 300.0f;
    countEditBox = this->makeEditBox(Vec2(x, y), "参加回数(編集非推奨)");
    
    y -= 80.0f;
    lastTimeEditBox = this->makeEditBox(Vec2(x, y), "最終参加日(編集非推奨)");
    
    deleteButton  = headerPanel->getChildByName<ui::Button *>("ButtonDelete");
    saveButton    = headerPanel->getChildByName<ui::Button *>("ButtonSave");
    backButton    = headerPanel->getChildByName<ui::Button *>("ButtonBack");
   
    this->addCheckBoxEvent(maleCheckBox,     CheckBoxTag::Male);
    this->addCheckBoxEvent(femaleCheckBox,   CheckBoxTag::Female);
    this->addCheckBoxEvent(trainingCheckBox, CheckBoxTag::Training);
    this->addCheckBoxEvent(beginnerCheckBox, CheckBoxTag::Beginner);
    this->addCheckBoxEvent(middleCheckBox,   CheckBoxTag::Middle);
    this->addCheckBoxEvent(higherCheckBox,   CheckBoxTag::Higher);
    this->addButtonEvent(deleteButton,   ButtonTag::Delete);
    this->addButtonEvent(saveButton,     ButtonTag::Save);
    this->addButtonEvent(backButton,        ButtonTag::Back);
    
    // 新規
    if (!userInfo)
    {
        headerPanel->getChildByName<ui::Text *>("TextTitle")->setString("新規");
        deleteButton->setVisible(false);
    }
    // 編集
    else
    {
        headerPanel->getChildByName<ui::Text *>("TextTitle")->setString("編集");
        name         = userInfo->name;
        nameFurigana = userInfo->nameFurigana;
        nickname     = userInfo->nickname;
        gender       = userInfo->gender;
        level        = userInfo->level;
        countStr     = StringUtils::toString(userInfo->count);
        lastTime     = userInfo->lastTime;
    }
    
    // テキスト更新
    nameEditBox->setText(name.c_str());
    nameFuriganaEditBox->setText(nameFurigana.c_str());
    nicknameEditBox->setText(nickname.c_str());
    
    countEditBox->setText(countStr.c_str());
    lastTimeEditBox->setText(lastTime.c_str());
    
    
    // ボタン表示更新
    this->updateGenderCheckBox();
    this->updateLevelCheckBox();
    
    return true;
}


void MemberEditLayer::updateGenderCheckBox()
{
    bool checkedMale   = false;
    bool checkedFemale = false;
    switch (gender) {
        case Gender::Male:   checkedMale = true;   break;
        case Gender::Female: checkedFemale = true; break;
        default: break;
    }
    
    this->maleCheckBox->setSelected(checkedMale);
    this->maleCheckBox->setEnabled(!checkedMale);
    this->femaleCheckBox->setSelected(checkedFemale);
    this->femaleCheckBox->setEnabled(!checkedFemale);
}

void MemberEditLayer::updateLevelCheckBox()
{
    bool checkedTraining = false;
    bool checkedBeginner = false;
    bool checkedMiddle   = false;
    bool checkedHigher   = false;
    switch (level) {
        case Level::Training: checkedTraining = true;  break;
        case Level::Beginner: checkedBeginner = true;  break;
        case Level::Middle:   checkedMiddle   = true;  break;
        case Level::Higher:   checkedHigher   = true;  break;
        default:    break;
    }
    
    this->trainingCheckBox->setSelected(checkedTraining);
    this->trainingCheckBox->setEnabled(!checkedTraining);
    this->beginnerCheckBox->setSelected(checkedBeginner);
    this->beginnerCheckBox->setEnabled(!checkedBeginner);
    this->middleCheckBox->setSelected(checkedMiddle);
    this->middleCheckBox->setEnabled(!checkedMiddle);
    this->higherCheckBox->setSelected(checkedHigher);
    this->higherCheckBox->setEnabled(!checkedHigher);
}



void MemberEditLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::Delete:     this->showDeleteConfirm();  return;
        case ButtonTag::Save:       this->saveUserInfo();       return;
        case ButtonTag::Back:      this->close();              return;
    }
}

void MemberEditLayer::pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
    auto checkBox = dynamic_cast<ui::CheckBox *>(pSender);
    CheckBoxTag tag = (CheckBoxTag)checkBox->getTag();
    switch (tag) {
        case CheckBoxTag::Male:       gender = Gender::Male;      break;
        case CheckBoxTag::Female:     gender = Gender::Female;    break;
        case CheckBoxTag::Training:   level  = Level::Training;   break;
        case CheckBoxTag::Beginner:   level  = Level::Beginner;   break;
        case CheckBoxTag::Middle:     level  = Level::Middle;     break;
        case CheckBoxTag::Higher:     level  = Level::Higher;     break;
    }
    
    this->updateGenderCheckBox();
    this->updateLevelCheckBox();
}



void MemberEditLayer::showDeleteConfirm()
{
    std::string text = StringUtils::format("%sのデータを削除します\nよろしいですか？", name.c_str());
    std::function<void()> yesCallback = [this](){this->deleteUserInfo();};  // Yesなら削除
    std::function<void()> noCallback  = [](){};                             // Noなら何もしない
    
    ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
    Kyarochon::Event::showConfirmView(info);
}


void MemberEditLayer::deleteUserInfo()
{
    // 削除
    Manager::Member::getInstance()->removeMemberAt(userInfo->id);
    
    this->close();
}

void MemberEditLayer::saveUserInfo()
{
    // 入力情報取得
    name         = std::string(nameEditBox->getText());
    nameFurigana = std::string(nameFuriganaEditBox->getText());
    nickname     = std::string(nicknameEditBox->getText());
    
    countStr     = std::string(countEditBox->getText());
    lastTime     = std::string(lastTimeEditBox->getText());
    
    int count  = std::all_of(countStr.cbegin(), countStr.cend(), isdigit) ? std::atoi(countStr.c_str()) : -1;
    
    
    // 入力チェック
    std::string errorText = "";
    if (name.empty())
    {
        errorText += "氏名が入力されていません\n";
    }
    if (nameFurigana.empty())
    {
        errorText += "ふりがなが入力されていません\n";
    }
    if (gender == Gender::Unknown)
    {
        errorText += "性別が入力されていません\n";
    }
    if (level == Level::Unknown)
    {
        errorText += "レベルが入力されていません\n";
    }
    if (count < 0)
    {
        errorText += "参加回数は0以上で入力してください\n";
    }
    if (!lastTime.empty() && lastTime != "NULL" && lastTime.length() != 19)
    {
        errorText += "最終参加日は`YYYY/MM/DD hh:mm:ss`\nの形式で入力してください\n";
    }
    
    
    if (!errorText.empty())
    {
        errorText.pop_back();
        Kyarochon::Event::showAlertView(errorText);
        return;
    }
        
        
    // 新規
    if (!userInfo)
    {
        // 新規追加
        Manager::Member::getInstance()->addMember(name, nameFurigana, nickname, gender, "", level, count, 0, lastTime);
    }
    // 編集
    else
    {
        // 更新
        userInfo->name         = name;
        userInfo->nameFurigana = nameFurigana;
        userInfo->nickname     = nickname;
        userInfo->gender       = gender;
        userInfo->level        = level;
        userInfo->count        = std::atoi(countStr.c_str());
        userInfo->lastTime     = lastTime;
        
        // マネージャに通知
        Manager::Member::getInstance()->updateMember(userInfo);
    }
    
    this->close();
}

void MemberEditLayer::close()
{
    Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_MEMBER_LIST);
    this->removeFromParent();
}





