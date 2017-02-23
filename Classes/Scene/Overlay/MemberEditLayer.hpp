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
        Male,
        Female,
        Training,
        Beginner,
        Middle,
        Higher,
        Delete,
        Save,
        Back,
    };
    
public:
    static MemberEditLayer *create(UserInfo *userInfo);
    
protected:
    MemberEditLayer();
    virtual ~MemberEditLayer();
    bool init(UserInfo *userInfo);
    
    virtual void pushedButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

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
    
    cocos2d::ui::Button *maleButton;
    cocos2d::ui::Button *femaleButton;
    cocos2d::ui::Button *trainingButton;
    cocos2d::ui::Button *beginnerButton;
    cocos2d::ui::Button *middleButton;
    cocos2d::ui::Button *higherButton;
    cocos2d::ui::EditBox *countEditBox;
    cocos2d::ui::EditBox *lastTimeEditBox;
//    cocos2d::ui::TextField *countTextField;
//    cocos2d::ui::TextField *lastTimeTextField;
    cocos2d::ui::Button *deleteButton;
    cocos2d::ui::Button *saveButton;
    cocos2d::ui::Button *backButton;
    
    void updateGenderButton();
    void updateLevelButton();
    
    void showDeleteConfirm();
    void deleteUserInfo();
    void saveUserInfo();
    void close();
};


#endif /* MemberEditLayer_hpp */
