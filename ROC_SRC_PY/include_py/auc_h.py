# Auto-converted from auc.h
import typing

# auc.h: Data structures and prototypes for the game's auction communication channel.
# auc.h: 拍賣通訊頻道（Auction）的資料結構與相關函式原型宣告。
# typedef struct auction_data AUCTION_DATA
ROOM_AUCTION = 32768
price: 'int' = 0
time: 'int' = 0

class auction_data:
    """ C struct auction_data """
    def __init__(self):
        self.owner: 'CHAR_DATA' = None
        self.buyer: 'CHAR_DATA' = None
        self.obj: 'OBJ_DATA' = None
