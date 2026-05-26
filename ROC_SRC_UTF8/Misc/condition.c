
#include "merc.h"
#include <stdio.h>

// show_position_to_char: Formats and displays a character's current posture/combat position.
// show_position_to_char: 格式化並向玩家角色顯示其目前的姿勢、體態或戰鬥位置狀態。
bool show_position_to_char(CHAR_DATA *ch) {
    switch (ch->position) {
    case POS_DEAD:
        send_to_char("靜靜躺著吧，你已經死了.\n\r", ch);
        return TRUE;

    case POS_MORTAL:
    case POS_INCAP:
        send_to_char("你的狀況太糟了.\n\r", ch);
        return TRUE;

    case POS_STUNNED:
        send_to_char("你頭昏眼花.\n\r", ch);
        return TRUE;

    case POS_PARALYZED:
        send_to_char("你麻痺了.\n\r", ch);
        return TRUE;

    case POS_SLEEPING:
        send_to_char("你正在睡夢中，請先 wake 起來。\n\r", ch);
        return TRUE;

    case POS_RESTING:
        send_to_char("啊...你覺得有點懶散...(你必須先'站'起來)\n\r", ch);
        return TRUE;

    case POS_FIGHTING:
        send_to_char("不行！  你還在戰鬥中！\n\r", ch);
        return TRUE;
    }
    return FALSE;
}
