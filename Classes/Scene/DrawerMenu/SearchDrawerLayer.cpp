//
//  SearchDrawerLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/12/11.
//
//

#include "SearchDrawerLayer.hpp"
//#include "CourtManager.hpp"
#include "MemberManager.hpp"
#include "TimeUtil.h"

USING_NS_CC;
using namespace cocostudio;

SearchDrawerLayer::SearchDrawerLayer()
:syllabaryCheckBox(nullptr)
,genderCheckBox(nullptr)
,levelCheckBox(nullptr)
,countCheckBox(nullptr)
,lastDateCheckBox(nullptr)
,maleCheckBox(nullptr)
,femaleCheckBox(nullptr)
,trainingCheckBox(nullptr)
,beginnerCheckBox(nullptr)
,middleCheckBox(nullptr)
,higherCheckBox(nullptr)
,lastDayTenCheckBox(nullptr)
,lastMonthOneCheckBox(nullptr)
,lastMonthThreeCheckBox(nullptr)
,wordEditBox(nullptr)
{
    
}

SearchDrawerLayer::~SearchDrawerLayer() {
}

#pragma mark - 初期化
SearchDrawerLayer *SearchDrawerLayer::create()
{
    SearchDrawerLayer *ret = new SearchDrawerLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool SearchDrawerLayer::init()
{
    std::string csbName = "SearchDrawerLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    
    // ソート
    auto sortPanel = panel->getChildByName<ui::Layout *>("PanelSort");
    syllabaryCheckBox = sortPanel->getChildByName<ui::CheckBox *>("CheckBoxSortSyllabary");
    genderCheckBox    = sortPanel->getChildByName<ui::CheckBox *>("CheckBoxSortGender");
    levelCheckBox     = sortPanel->getChildByName<ui::CheckBox *>("CheckBoxSortLevel");
    countCheckBox     = sortPanel->getChildByName<ui::CheckBox *>("CheckBoxSortCount");
    lastDateCheckBox  = sortPanel->getChildByName<ui::CheckBox *>("CheckBoxSortLastDate");
    
    
    this->addCheckBoxEvent(syllabaryCheckBox, CheckBoxTag::SortSyllabary);
    this->addCheckBoxEvent(genderCheckBox,    CheckBoxTag::SortGender);
    this->addCheckBoxEvent(levelCheckBox,     CheckBoxTag::SortLevel);
    this->addCheckBoxEvent(countCheckBox,     CheckBoxTag::SortCount);
    this->addCheckBoxEvent(lastDateCheckBox,  CheckBoxTag::SortLastDate);
    
    // 検索
    auto searchPanel = panel->getChildByName<ui::Layout *>("PanelSearch");
    maleCheckBox       = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxGenderMale");
    femaleCheckBox     = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxGenderFemale");
    trainingCheckBox   = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelTraining");
    beginnerCheckBox   = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelBeginner");
    middleCheckBox     = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelMiddle");
    higherCheckBox     = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLevelHigher");
    lastDayTenCheckBox   = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLastDayTen");
    lastMonthOneCheckBox = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLastMonthOne");
    lastMonthThreeCheckBox  = searchPanel->getChildByName<ui::CheckBox *>("CheckBoxLastMonthThree");
    
    this->addCheckBoxEvent(maleCheckBox, CheckBoxTag::SearchMale);
    this->addCheckBoxEvent(femaleCheckBox, CheckBoxTag::SearchFemale);
    this->addCheckBoxEvent(trainingCheckBox, CheckBoxTag::SearchTraining);
    this->addCheckBoxEvent(beginnerCheckBox, CheckBoxTag::SearchBeginner);
    this->addCheckBoxEvent(middleCheckBox, CheckBoxTag::SearchMiddle);
    this->addCheckBoxEvent(higherCheckBox, CheckBoxTag::SearchHigher);
    this->addCheckBoxEvent(lastDayTenCheckBox, CheckBoxTag::SearchLastDayTen);
    this->addCheckBoxEvent(lastMonthOneCheckBox, CheckBoxTag::SearchLastMonthOne);
    this->addCheckBoxEvent(lastMonthThreeCheckBox, CheckBoxTag::SearchLastMonthThree);
    
    wordEditBox = this->makeEditBox(Vec2(45.0f, 1090.0f), "検索ワード", panel);
    wordEditBox->setText(Manager::Member::getInstance()->getSearchWord().c_str());
//    wordEditBox->setFontColor(Color3B::BLACK);
//    wordEditBox->setColor(Color3B::BLACK);
//    wordEditBox->setPlaceholderFontColor(Color3B::BLACK);
    
    // ボタン表示更新
    this->updateSortCheckBox();
    this->updateSearchCheckBox();

    return true;
}

void SearchDrawerLayer::updateSortCheckBox()
{
    auto sortType = Manager::Member::getInstance()->getSortType();
    
    bool enabledSyllabary = false;
    bool enabledGender = false;
    bool enabledLevel = false;
    bool enabledCount = false;
    bool enabledLastDate = false;
    
    
    switch (sortType) {
        case SortType::Syllabary:   enabledSyllabary = true;   break;
        case SortType::Gender:      enabledGender    = true;   break;
        case SortType::Level:       enabledLevel     = true;   break;
        case SortType::Count:       enabledCount     = true;   break;
        case SortType::LastDate:    enabledLastDate  = true;   break;
    }
    
    syllabaryCheckBox->setSelected(enabledSyllabary);
    genderCheckBox->setSelected(enabledGender);
    levelCheckBox->setSelected(enabledLevel);
    countCheckBox->setSelected(enabledCount);
    lastDateCheckBox->setSelected(enabledLastDate);
}


void SearchDrawerLayer::updateSearchCheckBox()
{
    auto memberManager = Manager::Member::getInstance();
    
    // 性別
    auto filterGenderVec = memberManager->getGenderFilter();
    bool enabledMale   = std::find(filterGenderVec.begin(), filterGenderVec.end(), Gender::Male)   != filterGenderVec.end();
    bool enabledFemale = std::find(filterGenderVec.begin(), filterGenderVec.end(), Gender::Female) != filterGenderVec.end();
//    for (auto gender : filterGenderVec)
//    {
//        switch (gender) {
//            case Gender::Male:      enabledMale   = true;  break;
//            case Gender::Female:    enabledFemale = true;  break;
//            default:    break;
//        }
//    }
    maleCheckBox->setSelected(enabledMale);
    femaleCheckBox->setSelected(enabledFemale);
    
    // レベル
    auto filterLevelVec = memberManager->getLevelFilter();
    bool enabledTraining = std::find(filterLevelVec.begin(), filterLevelVec.end(), Level::Training) != filterLevelVec.end();
    bool enabledBeginner = std::find(filterLevelVec.begin(), filterLevelVec.end(), Level::Beginner) != filterLevelVec.end();
    bool enabledMiddle   = std::find(filterLevelVec.begin(), filterLevelVec.end(), Level::Middle)   != filterLevelVec.end();
    bool enabledHigher   = std::find(filterLevelVec.begin(), filterLevelVec.end(), Level::Higher)   != filterLevelVec.end();
//    for (auto level : filterLevelVec)
//    {
//        switch (level) {
//            case Level::Training: enabledTraining = false;  break;
//            case Level::Beginner: enabledBeginner = false;  break;
//            case Level::Middle:   enabledMiddle   = false;  break;
//            case Level::Higher:   enabledHigher   = false;  break;
//            default:    break;
//        }
//    }
    trainingCheckBox->setSelected(enabledTraining);
    beginnerCheckBox->setSelected(enabledBeginner);
    middleCheckBox->setSelected(enabledMiddle);
    higherCheckBox->setSelected(enabledHigher);
    
    // 最終参加日
    bool enabledLastDayTen   = false;
    bool enabledLastMonthOne = false;
    bool enabledLastMonthThree  = false;
    int filterLastDate = memberManager->getLastDateFilter();
    if (filterLastDate == 10)
    {
        enabledLastDayTen = true;
    }
    else if (filterLastDate == DAYS_PER_MONTH * 1)
    {
        enabledLastMonthOne = true;
    }
    else if (filterLastDate == DAYS_PER_MONTH * 3)
    {
        enabledLastMonthThree = true;
    }
    lastDayTenCheckBox->setSelected(enabledLastDayTen);
    lastMonthOneCheckBox->setSelected(enabledLastMonthOne);
    lastMonthThreeCheckBox->setSelected(enabledLastMonthThree);
}


void SearchDrawerLayer::pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
    auto checkBox = dynamic_cast<ui::CheckBox *>(pSender);
    CheckBoxTag tag = (CheckBoxTag)checkBox->getTag();
    auto memberManager = Manager::Member::getInstance();
    
    switch (tag) {
        // --- ソート ---
        case CheckBoxTag::SortSyllabary:    memberManager->setSortType(SortType::Syllabary);    break;
        case CheckBoxTag::SortGender:       memberManager->setSortType(SortType::Gender);       break;
        case CheckBoxTag::SortLevel:        memberManager->setSortType(SortType::Level);        break;
        case CheckBoxTag::SortCount:        memberManager->setSortType(SortType::Count);        break;
        case CheckBoxTag::SortLastDate:     memberManager->setSortType(SortType::LastDate);     break;
            
        // --- 検索 ---
        // 性別
        case CheckBoxTag::SearchMale:     memberManager->updateGenderFilter(Gender::Male);   break;
        case CheckBoxTag::SearchFemale:   memberManager->updateGenderFilter(Gender::Female); break;
        // レベル
        case CheckBoxTag::SearchTraining: memberManager->updateLevelFilter(Level::Training); break;
        case CheckBoxTag::SearchBeginner: memberManager->updateLevelFilter(Level::Beginner); break;
        case CheckBoxTag::SearchMiddle:   memberManager->updateLevelFilter(Level::Middle);   break;
        case CheckBoxTag::SearchHigher:   memberManager->updateLevelFilter(Level::Higher);   break;
        // 回数
        case CheckBoxTag::SearchLastDayTen:     memberManager->updateLastDateFilter(10);  break;
        case CheckBoxTag::SearchLastMonthOne:   memberManager->updateLastDateFilter(DAYS_PER_MONTH * 1);  break;
        case CheckBoxTag::SearchLastMonthThree: memberManager->updateLastDateFilter(DAYS_PER_MONTH * 3);  break;
    }
    
    this->updateSortCheckBox();
    this->updateSearchCheckBox();
}

// 検索用テキストの内容を取得
std::string SearchDrawerLayer::getSearchText()
{
    return std::string(this->wordEditBox->getText());
}
