//
//  ConfirmInfo.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/09.
//
//

#ifndef ConfirmInfo_hpp
#define ConfirmInfo_hpp

#include <string>
#include <functional>

class ConfirmInfo
{
public:
    // コンストラクタ／デストラクタ
    ConfirmInfo(std::string text, std::function<void()> yesCallback, std::function<void()> noCallback);
    virtual ~ConfirmInfo();
    
public:
    std::string text;
    std::function<void()> yesCallback;
    std::function<void()> noCallback;
};
#endif /* ConfirmInfo_hpp */
