//
//  CourtDateInfo.hpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/12.
//
//

#ifndef CourtDateInfo_hpp
#define CourtDateInfo_hpp

#include <string>

class CourtDateInfo
{
    
public:
    // コンストラクタ／デストラクタ
    CourtDateInfo(int id, std::string time, std::string place);
    CourtDateInfo(int id, std::string place);
    virtual ~CourtDateInfo();
    
    
public:
    int id;
    std::string time;
    std::string place;
};

#endif /* CourtDateInfo_hpp */
