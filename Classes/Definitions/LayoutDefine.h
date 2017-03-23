//
//  LayoutDefine.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/06/30.
//
//

#ifndef LayoutDefine_h
#define LayoutDefine_h


enum class TabType
{
    Member,
    Participant,
    Court,
    Setting,
    None,
};


enum class SortType
{
    Syllabary,  // 五十音順
    Gender,     // 性別
    Level,      // レベル別
    Count,      // 参加回数が多い順
    LastDate,   // 参加日が新しい順
};

enum class ShuffleType
{
    RandomPair,     // 同じ人とペアが偏らない
    RandomPoint,    // レベルが均等になる
};

enum class AllocationType
{
    All,        // すべてごちゃまぜ
    ByLevel,    // レベル別でコートを分ける
};


// UI／イベント
#define LONG_TAP_TIME (0.3f)


// レイアウト
#define DEFAULT_SCREEN_HEIGHT (1136.0f)
#define DEFAULT_SCREEN_WIDTH  (640.0f)

#define HEADER_HEIGHT (100.0f)
#define HEADER_WIDTH  (640.0f)

#define TAB_SPACE  (10.0f)
#define TAB_HEIGHT (85.0f)
#define TAB_WIDTH  (640.0f)

#define SCROLL_WIDTH  (600.0f)
#define SCROLL_MARGIN (25.0f)


// メンバーリスト
#define MEMBER_CATEGORY_ITEM_HEIGHT (40.0f)
#define MEMBER_CATEGORY_ITEM_WIDTH  (295.0f)
#define MEMBER_ITEM_HEIGHT (105.0f)
#define MEMBER_ITEM_WIDTH  (300.0f)


// 参加者
#define PARTICIPANT_CATEGORY_ITEM_HEIGHT (40.0f)
#define PARTICIPANT_CATEGORY_ITEM_WIDTH  (295.0f)
#define PARTICIPANT_ITEM_HEIGHT (100.0f)
#define PARTICIPANT_ITEM_WIDTH  (300.0f)

// コート
#define COURT_MEMBER_ITEM_HEIGHT (60.0f)
#define COURT_MEMBER_ITEM_WIDTH  (270.0f)

#define COURT_ITEM_HEIGHT (220.0f)
#define COURT_ITEM_WIDTH  (540.0f)

#define HISTORY_SCROLL_HEIGHT (750.0f)
#define HISTORY_ITEM_HEIGHT   (50.0f)
#define HISTORY_ITEM_WIDTH    (300.0f)

#define WIN_SCROLL_HEIGHT (750.0f)
#define WIN_ITEM_HEIGHT (50.0f)
#define WIN_ITEM_WIDTH  (300.0f)


#endif /* LayoutDefine_h */
