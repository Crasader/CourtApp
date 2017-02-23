//
//  GameHistoryItemNode.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/07/28.
//
//

#ifndef GameHistoryItemNode_hpp
#define GameHistoryItemNode_hpp


#include "CsbLayerBase.hpp"
#include "UserInfo.hpp"

class GameHistoryItemNode : public CsbLayerBase
{

public:
    static GameHistoryItemNode *create(UserInfo *userInfo, int memberId);
    
protected:
    GameHistoryItemNode();
    virtual ~GameHistoryItemNode();
    bool init(UserInfo *userInfo, int memberId);
    
};


#endif /* GameHistoryItemNode_hpp */
