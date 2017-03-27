//
//  MemberEditLayer.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/03.
//
//

#ifndef MemberEditLayer_hpp
#define MemberEditLayer_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"
#include <string>

class MemberEditLayer : public CsbLayerBase
{
    enum ButtonTag
    {
        Delete,
        Save,
        Back,
    };
    
    enum CheckBoxTag
    {
        Male,
        Female,
        Training,
        Beginner,
        Middle,
        Higher,
    };
    
public:
    static MemberEditLayer *create(UserInfo *userInfo);
    
protected:
    MemberEditLayer();
    virtual ~MemberEditLayer();
    bool init(UserInfo *userInfo);
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void pushedCheckBox(cocos2d::Ref *pSender, cocos2d::ui::CheckBox::EventType type);


private:
    UserInfo *userInfo;
    std::string name = "";
    std::string nameFurigana = "";
    std::string nickname = "";
    Gender gender = Gender::Unknown;
    Level level   = Level::Unknown;
    std::string countStr = "0";
    std::string lastTime = "";
    
    cocos2d::ui::EditBox *nameEditBox;
    cocos2d::ui::EditBox *nameFuriganaEditBox;
    cocos2d::ui::EditBox *nicknameEditBox;
    
    cocos2d::ui::CheckBox *maleCheckBox;
    cocos2d::ui::CheckBox *femaleCheckBox;
    cocos2d::ui::CheckBox *trainingCheckBox;
    cocos2d::ui::CheckBox *beginnerCheckBox;
    cocos2d::ui::CheckBox *middleCheckBox;
    cocos2d::ui::CheckBox *higherCheckBox;
    cocos2d::ui::EditBox *countEditBox;
    cocos2d::ui::EditBox *lastTimeEditBox;
    cocos2d::ui::Button *deleteButton;
    cocos2d::ui::Button *saveButton;
    cocos2d::ui::Button *backButton;
    
    void updateGenderCheckBox();
    void updateLevelCheckBox();
    
    void showDeleteConfirm();
    void deleteUserInfo();
    void saveUserInfo();
    void close();
};


#endif /* MemberEditLayer_hpp */
