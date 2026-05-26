// update.h: Periodic updates (ticks) for chars, objects, weather, and game zones.
// update.h: 處理角色、物品、天氣及遊戲區域定期更新週期（Ticks）的函式原型宣告。
#ifndef __UPDATE_H__
#define __UPDATE_H__

/* update all lists, main entrance */
void list_update(void);

/* subroutine to update char list and ob list */
void char_list_update(void);
void obj_list_update(void);

#endif
