typedef struct          auction_data            AUCTION_DATA;

#define ROOM_AUCTION            32768
/*
 *  * Auction data
 *   *
 *    * Written by zcecil
 *     *
 *      */

struct auction_data
{
	CHAR_DATA *owner;
	CHAR_DATA *buyer;
	OBJ_DATA *obj;
	int price;
	int time;
};

