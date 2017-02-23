//
//  WinItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/02.
//
//

#ifndef WinItemNode_hpp
#define WinItemNode_hpp

#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class WinItemNode : public CsbLayerBase
{
    
public:
    static WinItemNode *create(UserInfo *userInfo);
    
protected:
    WinItemNode();
    virtual ~WinItemNode();
    bool init(UserInfo *userInfo);
};

#endif /* WinItemNode_hpp */
