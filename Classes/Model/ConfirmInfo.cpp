//
//  ConfirmInfo.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/09.
//
//

#include "ConfirmInfo.hpp"

ConfirmInfo::ConfirmInfo(std::string text, std::function<void()> yesCallback, std::function<void()> noCallback)
{
    this->text = text;
    this->yesCallback = yesCallback;
    this->noCallback = noCallback;
}

ConfirmInfo::~ConfirmInfo()
{
    
}