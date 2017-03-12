//
//  SearchDrawerLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/12/11.
//
//

#ifndef SearchDrawerLayer_hpp
#define SearchDrawerLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class SearchDrawerLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        HideLayer,
    };
    
    enum CheckBoxTag
    {
        // ソート
        SortSyllabary,
        SortGender,
        SortLevel,
        SortCount,
        SortLastDate,
        // 検索
        SearchMale,
        SearchFemale,
        SearchTraining,
        SearchBeginner,
        SearchMiddle,
        SearchHigher,
        SearchLastDayTen,
        SearchLastMonthOne,
        SearchLastMonthThree,
    };
    
public:
    static SearchDrawerLayer *create();
    std::string getSearchText();
    void showLayer();
    void hideLayer();
    
    
protected:
    SearchDrawerLayer();
    virtual ~SearchDrawerLayer();
    bool init();
    
    virtual void pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type);
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    Gender gender = Gender::Unknown;
    Level level   = Level::Unknown;
    std::string searchWord = "";
    
    cocos2d::ui::Button *hideLayerButton;
    
    cocos2d::ui::CheckBox *syllabaryCheckBox;
    cocos2d::ui::CheckBox *genderCheckBox;
    cocos2d::ui::CheckBox *levelCheckBox;
    cocos2d::ui::CheckBox *countCheckBox;
    cocos2d::ui::CheckBox *lastDateCheckBox;
    cocos2d::ui::CheckBox *maleCheckBox;
    cocos2d::ui::CheckBox *femaleCheckBox;
    cocos2d::ui::CheckBox *trainingCheckBox;
    cocos2d::ui::CheckBox *beginnerCheckBox;
    cocos2d::ui::CheckBox *middleCheckBox;
    cocos2d::ui::CheckBox *higherCheckBox;
    cocos2d::ui::CheckBox *lastDayTenCheckBox;
    cocos2d::ui::CheckBox *lastMonthOneCheckBox;
    cocos2d::ui::CheckBox *lastMonthThreeCheckBox;
    cocos2d::ui::EditBox *wordEditBox;
    
    void updateSortCheckBox();
    void updateSearchCheckBox();
};


#endif /* SearchDrawerLayer_hpp */
