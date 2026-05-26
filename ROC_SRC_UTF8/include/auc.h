// auc.h: Data structures and prototypes for the game's auction communication channel.
// auc.h: 拍賣通訊頻道（Auction）的資料結構與相關函式原型宣告。
#include "merc.h"
typedef struct auction_data AUCTION_DATA;

#define ROOM_AUCTION 32768
/*
 *  * Auction data
 *   *
 *    * Written by zcecil
 *     *
 *      */

struct auction_data {
    CHAR_DATA *owner;
    CHAR_DATA *buyer;
    OBJ_DATA  *obj;
    int        price;
    int        time;
};
