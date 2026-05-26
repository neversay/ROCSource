/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael    *
 *  Chastain, Michael Quan, and Mitchell Tse.          *
 *                     *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.         *
 *                     *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and    *
 *  Mitchell Tse.                *
 *                     *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.     *
 *                     *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in    *
 *  'license.txt', as well as the Envy license in 'license.nvy'.     *
 *  In particular, you may not remove either of these copyright notices.   *
 *                     *
 *  Much time and thought has gone into this software and you are    *
 *  benefitting.  We hope that you share your changes too.  What goes    *
 *  around, comes around.              *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : act_obj.c                                      *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         修改eqbank open如已bank->open時        *
//*              :             檢查線上ppl是否有人使用            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940619 jye                                    *
//*  PURPOSE     :         修改eqbank list可尋抓相關字之eq name   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940825 Razgriz                                *
//*  PURPOSE     :         把 fwrite_neweqbank 跟 free_neweqbank  *
//*              :         宣告在檔案開頭以便讓 close_socket 使用 *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 jye                                    *
//*  PURPOSE     :         修改do_give 時imm無視ITEM_PERSONAL     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         修改判斷souvenir值使改用變數           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940926 Razgriz                                *
//*  PURPOSE     :         修改 do_steal 不能偷穿在身上的東西     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941003 Jye                                    *
//*  PURPOSE     :         修改do_brandish 排除imm char           *
//*****************************************************************
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include "ansi.h"
#include "merc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * Local functions.
 */
#define CD CHAR_DATA
void get_obj args((CHAR_DATA * ch, OBJ_DATA *obj, OBJ_DATA *container));
void wear_obj args((CHAR_DATA * ch, OBJ_DATA *obj, bool fReplace, int iWear));
CD *find_keeper args((CHAR_DATA * ch, char *argument));
int get_cost args((CHAR_DATA * keeper, OBJ_DATA *obj, bool fBuy));
#undef CD
void show_list_to_char(OBJ_DATA *list, CHAR_DATA *ch, bool fShort,
                       bool fShowNothing);

/*
 * Global functions and variables.  -- Razgriz
 */
// declaration for close_socket()	Added by Razgriz 20050825
void fwrite_neweqbank args((CHAR_DATA * ch, FILE *fp));
void free_neweqbank args((BANK_DATA * bank));

extern int top_bank;
extern bool IsSorGateMob(int nMobIndex);

// free_neweqbank_obj: Frees memory allocated for an equipment bank object.
// free_neweqbank_obj: 釋放為裝備銀行物品所配置的記憶體。
void free_neweqbank_obj(OBJ_DATA *obj) {
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    EXTRA_DESCR_DATA *ed;
    EXTRA_DESCR_DATA *ed_next;
    OBJ_HP *ohp, *ohp_next;

    for (ed = obj->extra_descr; ed; ed = ed_next) {
        ed_next = ed->next;

        free_string(ed->description);
        free_string(ed->keyword);
        ed->next = extra_descr_free;
        extra_descr_free = ed;
    }
    for (ohp = obj->hp; ohp; ohp = ohp_next) {
        ohp_next = ohp->next;
        free_string(ohp->hp_name);
        ohp->next = obj_hp_free;
        obj_hp_free = ohp;
    }

    for (paf = obj->affected; paf; paf = paf_next) {
        paf_next = paf->next;

        if (obj->affected == paf) {
            obj->affected = paf->next;
        } else {
            AFFECT_DATA *prev;

            for (prev = obj->affected; prev; prev = prev->next) {
                if (prev->next == paf) {
                    prev->next = paf->next;
                    break;
                }
            }

            if (!prev) {
                bug("[void free_neweqbank_obj]List_update: cannot find paf on "
                    "obj.",
                    0);
                continue;
            }
        }

        paf->next = affect_free;
        affect_free = paf;
    }

    free_string(obj->name);
    free_string(obj->description);
    free_string(obj->short_descr);
    /*        free_string( obj->cname     );*/
    --obj->pIndexData->count;

    obj->next = obj_free;
    obj_free = obj;
}

// 完成 eqb_do_examine 的指令, 並移除 eqb_do_look 將兩者統合為單一指令
// 2022/12/01
// eqb_do_examine: Examines an object in the equipment bank.
// eqb_do_examine: 檢查裝備銀行中的特定物品。
void eqb_do_examine(CHAR_DATA *ch, OBJ_DATA *obj) {
    char buf[MAX_STRING_LENGTH];
    int diff;

    if (!obj) {
        send_to_char("[33m要檢查什麼？[m\n\r", ch);
        return;
    }

    send_to_char(obj->short_descr, ch);
    send_to_char(" - ", ch);
    send_to_char(obj->material->short_descr, ch);
    send_to_char("\n\r", ch);
    send_to_char(obj->description, ch);
    send_to_char("\n\r", ch);
    switch (obj->item_type) {
    default:
        break;

    case ITEM_LIGHT:
        if (obj->value[2] != -1) {
            sprintf(buf, "看來還能使用 %d 小時。\n\r", obj->value[2]);
            send_to_char(buf, ch);
        }
        break;

    case ITEM_ARMOR:
        send_to_char("你仔細打量之後，發現它", ch);
        if (obj->value[1] != 0)
            send_to_char("受到了腐蝕，外觀看起來", ch);

        if (indestructible(obj)) {
            send_to_char(HIW "正處在完美的狀況下。" NOR "\n\r", ch);
            break;
        } else if (get_max_dura(obj) == 0) {
            send_to_char(RED "好像一旦用壞就無法修復。" NOR "\n\r", ch);
            break;
        }

        diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

        if (diff == 100)
            send_to_char(HIW "正處在完美的狀況下" NOR "。\n\r", ch);
        else if (diff > 70)
            send_to_char(HIC "看起來相當良好" NOR "。\n\r", ch);
        else if (diff > 50)
            send_to_char(HIY "稍微有些刮痕" NOR "。\n\r", ch);
        else if (diff > 33)
            send_to_char(YEL "狀況不太好" NOR "。\n\r", ch);
        else if (diff > 20)
            send_to_char(HIR "差不多快壞了" NOR "。\n\r", ch);
        else if (diff > 10)
            send_to_char(RED "上頭的部件劇烈晃動著" NOR "。\n\r", ch);
        else if (diff > 0)
            send_to_char(RED "好像輕輕一碰就會解體" NOR "。\n\r", ch);
        else
            send_to_char(HIB "已經完全損壞，不堪使用" NOR "。\n\r", ch);

        break;

    case ITEM_WEAPON:
        send_to_char("你仔細打量之後，發現它", ch);
        if (obj->value[3] == 1)
            send_to_char("能作為利刃切割，外觀看起來", ch);
        else if (obj->value[3] == 2)
            send_to_char("能作為長槍突刺，外觀看起來", ch);
        else if (obj->value[3] == 3)
            send_to_char("能作為刀劍劈砍，外觀看起來", ch);
        else if (obj->value[3] == 4)
            send_to_char("能作為鞭子揮舞，外觀看起來", ch);
        else if (obj->value[3] == 5)
            send_to_char("能用來抓擊對手，外觀看起來", ch);
        else if (obj->value[3] == 6)
            send_to_char("能噴出魔法，外觀看起來", ch);
        else if (obj->value[3] == 7)
            send_to_char("能用來敲擊對手，外觀看起來", ch);
        else if (obj->value[3] == 8)
            send_to_char("能作為重鎚粉碎對手，外觀看起來", ch);
        else if (obj->value[3] == 9)
            send_to_char("能作為重型利器，外觀看起來", ch);
        else if (obj->value[3] == 10)
            send_to_char("能咬住生物，外觀看起來", ch);
        else if (obj->value[3] == 11)
            send_to_char("能作為匕首刺擊，外觀看起來", ch);
        else if (obj->value[3] == 12)
            send_to_char("能吸住生物，外觀看起來", ch);
        else if (obj->value[3] == 13)
            send_to_char("能作為柴刀連斬，外觀看起來", ch);
        else if (obj->value[3] == 14)
            send_to_char("能作為弓使用，外觀看起來", ch);
        else if (obj->value[3] == 16)
            send_to_char("能作為槍械使用，外觀看起來", ch);

        if (indestructible(obj)) {
            send_to_char(HIW "正處在完美的狀況下" NOR "。\n\r", ch);
            break;
        } else if (get_max_dura(obj) == 0) {
            send_to_char(RED "好像一旦用壞就無法修復" NOR "。\n\r", ch);
            break;
        }

        diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

        if (diff == 100)
            send_to_char(HIW "正處在完美的狀況下" NOR "。\n\r", ch);
        else if (diff > 70)
            send_to_char(HIC "看起來狀況好的很" NOR "。\n\r", ch);
        else if (diff > 50)
            send_to_char(HIY "稍微有些刮痕" NOR "。\n\r", ch);
        else if (diff > 33)
            send_to_char(YEL "狀況不太好" NOR "。\n\r", ch);
        else if (diff > 20)
            send_to_char(HIR "差不多快壞了" NOR "。\n\r", ch);
        else if (diff > 10)
            send_to_char(RED "上頭的部件劇烈晃動著" NOR "。\n\r", ch);
        else if (diff > 0)
            send_to_char(RED "好像輕輕一碰就會解體" NOR "。\n\r", ch);
        else
            send_to_char(HIB "已經完全損壞，不堪使用" NOR "。\n\r", ch);

        break;

    case ITEM_DRINK_CON:
        if (obj->value[2] >= LIQ_MAX || obj->value[2] < 0) {
            bug("Do_drink: bad liquid number %d.", obj->pIndexData->vnum);
            obj->value[2] = 0;
        }
        if (obj->value[1] <= 0 && obj->value[1] != -1) {
            send_to_char("[36m那是空的。[m\n\r", ch);
            break;
        }
        sprintf(buf, "那裡面有%s%s液體。\n\r",
                (obj->value[1] == -1) ? "如同小宇宙般巨量的"
                : obj->value[1] < obj->value[0] / 4     ? "不到一半的"
                : obj->value[1] < 3 * obj->value[0] / 4 ? "大約一半的"
                                                        : "超過一半的",
                liq_table[obj->value[2]].liq_color);
        send_to_char(buf, ch);
        break;

    case ITEM_FOUNTAIN:
        if (obj->value[2] >= LIQ_MAX || obj->value[2] < 0) {
            bug("Do_drink: bad liquid number %d.", obj->pIndexData->vnum);
            obj->value[2] = 0;
        }
        if (obj->value[1] <= 0 && obj->value[1] != -1) {
            send_to_char("[36m那是乾的。[m\n\r", ch);
            break;
        }
        sprintf(buf, "那裡面是%s%s。\n\r",
                (obj->value[1] == -1)                   ? "源源不絕的"
                : obj->value[1] < obj->value[0] / 4     ? "涓涓的"
                : obj->value[1] < 3 * obj->value[0] / 4 ? "緩緩流動的"
                                                        : "淙淙的",
                liq_table[obj->value[2]].liq_name);
        send_to_char(buf, ch);
        break;

    case ITEM_CONTAINER:
    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:
        if (IS_SET(obj->value[1], CONT_CLOSED)) {
            send_to_char("[33m它是關著的。[m\n\r", ch);
            break;
        }
        act("$p裡面有：", ch, obj, NULL, TO_CHAR);
        show_list_to_char(obj->contains, ch, TRUE, TRUE);
        break;

    case ITEM_ARROW:
        if (obj->value[0] <= 0) {
            send_to_char("[36m那是空的。[m\n\r", ch);
            break;
        }
        sprintf(buf, "那裡面還有%d支箭。\n\r", obj->value[0]);
        send_to_char(buf, ch);
        break;

    case ITEM_AMMO:
        if (obj->value[0] <= 0) {
            send_to_char("[36m那是空的。[m\n\r", ch);
            break;
        }
        sprintf(buf, "還有%d發彈藥。\n\r", obj->value[0]);
        send_to_char(buf, ch);
        break;

    case ITEM_DART:
        sprintf(buf, "還有%d件道具。\n\r", obj->value[0]);
        send_to_char(buf, ch);
        break;
    }

    return;
}

/*  重新編寫 eqb_do_examine 並移除 eqb_do_look 2022/12/01
// eqb_do_examine: Examines an object in the equipment bank.
// eqb_do_examine: 檢查裝備銀行中的特定物品。
void eqb_do_examine( CHAR_DATA *ch, OBJ_DATA *obj )
{
        int diff;

        if ( !obj )
        {
                send_to_char( "[33m要檢查什麼？[m\n\r", ch );
                return;
        }


        switch ( obj->item_type )
        {
                default:
                        break;

                case ITEM_ARMOR:
                        eqb_do_look( ch, obj );
                        if( obj->value[1]  != 0)
                                send_to_char("看來好像受到腐蝕了。\n\r", ch);
                        break;

                case ITEM_WEAPON:
                        eqb_do_look( ch, obj );
                        send_to_char("你仔細打量之後，發現它", ch);
                        if( indestructible(obj) ) {
                                send_to_char(HIW"正處在完美的狀況下。"NOR"\n\r",
ch); break;
                        }
                        else if ( get_max_dura(obj) == 0 ) {
                                send_to_char(RED"好像一旦用壞就無法修復。"NOR"\n\r",
ch); break;
                        }
                        diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

                        if( diff == 100 )
send_to_char(HIW"正處在完美的狀況下。"NOR"\n\r", ch); else if( diff > 90)
send_to_char(HIC"看起來狀況好的很。"NOR"\n\r", ch); else if( diff > 70)
send_to_char(HIY"稍微有些刮痕。"NOR"\n\r", ch); else if( diff > 40)
send_to_char(YEL "狀況不太好。"NOR"\n\r", ch); else if( diff > 10)
send_to_char(HIR"差不多快壞了。"NOR"\n\r", ch); else if( diff > 0 )
send_to_char(RED"好像輕輕一碰就會解體。"NOR"\n\r", ch); else
send_to_char(HIB"完全損壞，不堪使用。"NOR"\n\r", ch); break;

                case ITEM_DRINK_CON:
                case ITEM_CONTAINER:
                case ITEM_CORPSE_NPC:
                case ITEM_CORPSE_PC:
                eqb_do_look( ch, obj );
        }

    return;
}

// eqb_do_look: Looks at an object in the equipment bank.
// eqb_do_look: 查看裝備銀行中的特定物品。
void eqb_do_look( CHAR_DATA *ch, OBJ_DATA *obj )
{
        char buf  [ MAX_STRING_LENGTH ];


        if ( !( obj  ) )
        {
                send_to_char( "[33m你在這裡沒看到那玩意兒。[m\n\r", ch
); return;
        }


        send_to_char( obj->short_descr , ch);
        send_to_char( " - ", ch);
        send_to_char( obj->material->short_descr, ch);
        send_to_char("\n\r", ch);
        send_to_char( obj->description, ch );
        send_to_char( "\n\r", ch );
        switch ( obj->item_type )
        {
                default:
                        break;

                case ITEM_DRINK_CON:
                        send_to_char( "你往裡面看的時候，你看到：\n\r", ch );
                        if ( obj->value[1] <= 0 )
                        {
                                send_to_char( "[36m那是空的。[m\n\r", ch
); break;
                        }

                        sprintf( buf, "那裡面有%s%s液體。\n\r",
                        obj->value[1] <     obj->value[0] / 4
                        ? "不到一半的" :
                        obj->value[1] < 3 * obj->value[0] / 4
                        ? "大約一半的"     : "超過一半的",
                        liq_table[obj->value[2]].liq_color
                        );

                        send_to_char( buf, ch );
                        break;

                case ITEM_CONTAINER:
                case ITEM_CORPSE_NPC:
                case ITEM_CORPSE_PC:
                        if ( IS_SET( obj->value[1], CONT_CLOSED ) )
                        {
                                send_to_char( "[33m它是關著的。[m\n\r", ch
); break;
                        }

                        send_to_char( "你往裡面看的時候，你看到：\n\r", ch );
                        act( "$p裡面有：", ch, obj, NULL, TO_CHAR );
                        show_list_to_char( obj->contains, ch, TRUE, TRUE );
                        break;

                case ITEM_ARROW:
                        send_to_char( "你往裡面看的時候，你看到：\n\r", ch );
                        if ( obj->value[0] <= 0 )
                        {
                                send_to_char( "[36m那是空的。[m\n\r", ch
); break;
                        }

                        sprintf( buf, "那裡面還有%d支箭。\n\r", obj->value[0] );

                        send_to_char( buf, ch );
                        break;
        }
        return;
} */

// get_obj: Picks up or retrieves an object, handling inventory and weight limits.
// get_obj: 拾取或取得物品，處理背包空間與重量限制。
void get_obj(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container) {
    if (!CAN_WEAR(obj, ITEM_TAKE) && obj->item_type != ITEM_CORPSE_PC) {
        send_to_char("你無法拿起.\n\r", ch);
        return;
    } else if (obj->item_type == ITEM_CORPSE_PC) {
        if (!consented(ch, obj)) {
            send_to_char("你無法拿起.\n\r", ch);
            return;
        }
        /*  if( obj->in_room) //add by coconet for consent
          {
                obj->area = obj->in_room->area;
                bug( "test consent: obj %d " ,obj->area->name);
          }
          else
                return;*/
    }
    if (IS_SET(obj->extra_flags, ITEM_UNIQUE) && IS_NPC(ch)) {
        send_to_char("你無法拿起.\n\r", ch);
        return;
    }

    if (!(obj->item_type == ITEM_MONEY) &&
        (!container || container->carried_by != ch)) {
        if (ch->carry_number + get_obj_number(obj) > can_carry_n(ch)) {
            act("$d: 你無法拿那麼多東西.", ch, NULL, obj->name, TO_CHAR);
            return;
        }

        if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch)) {
            act("$d: 你拿不動了.", ch, NULL, obj->name, TO_CHAR);
            return;
        }
    }

    // 把敘述與物品移動分開 , 因 obj_act 可能包含 purge 2022/02/12
    if (container) {
        // modify by coco 0511 for horse
        if (container->item_type == ITEM_HORSE) {
            act("你從 $P 卸載下 $p.", ch, obj, container, TO_CHAR);
            act("$n 從 $P 卸載下 $p.", ch, obj, container, TO_ROOM);
            // obj_from_obj( obj );
        } else {
            act("你從 $P 拿出 $p.", ch, obj, container, TO_CHAR);
            act("$n 從 $P 拿出 $p.", ch, obj, container, TO_ROOM);
            // obj_from_obj( obj );
        }
    } else {
        act("你拿起 $p.", ch, obj, container, TO_CHAR);
        act("$n 拿起 $p.", ch, obj, container, TO_ROOM);
        // obj_from_room( obj );
    }

    if (container) {
        obj_from_obj(obj);
    } else {
        obj_from_room(obj); // remove obj from in_room->contents list
    }

    // holy evil 燒傷判定
    if (IS_OBJ_STAT(obj, ITEM_HOLY) && IS_EVIL(ch)) {
        act("你被 $p 上的神聖力量給灼傷了！", ch, obj, NULL, TO_CHAR);
        act("$n 被 $p 上的神聖力量給灼傷了！", ch, obj, NULL, TO_ROOM);
        damage(ch, ch, 50, gsn_burning_hands, WEAR_NONE, MSG_NORMAL,
               DMG_NORMAL);
    } else if (IS_OBJ_STAT(obj, ITEM_EVIL) && IS_GOOD(ch)) {
        act("你被 $p 上的邪惡力量給灼傷了！", ch, obj, NULL, TO_CHAR);
        act("$n 被 $p 上的邪惡力量給灼傷了！", ch, obj, NULL, TO_ROOM);
        damage(ch, ch, 50, gsn_burning_hands, WEAR_NONE, MSG_NORMAL,
               DMG_NORMAL);
    }

    // obj type 為 item_moeny 時的判定
    if (obj->item_type == ITEM_MONEY) {
        char buf[MAX_STRING_LENGTH];
        int amount;

        amount = obj->value[0];
        ch->gold += amount;

        if (amount > 1) {
            sprintf(buf, "你拿出了 %d 枚金幣.\n\r", amount);
            send_to_char(buf, ch);
        }
        // add at 2022/02/13
        obj_to_char(obj, ch);
        // obj_act get 2022/02/12
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("get", ch, obj);
        }
        if (obj->deleted)
            ;
        else
            extract_obj(obj); /*remove money obj from room/pc/container...*/
    } else {
        obj_to_char(obj, ch); /*add obj to char->carrying list*/
    }

    // pc corpse
    if (container && container->item_type == ITEM_CORPSE_PC &&
        !container->contains) {
        act("$p 消失了.", ch, container, container, TO_CHAR);
        act("$p 消失了.", ch, container, container, TO_ROOM);
        extract_obj(container);
        save_char_obj(ch);
        // container->timer /= 3;
    }

    // obj_act get 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("get", ch, obj);
    }

    return;
}

// do_autoget: Command: do_autoget - Performs the 'autoget' command.
// do_autoget: 指令：do_autoget - 執行 'autoget' 指令。
int do_autoget(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *container;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    bool found;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    /* Get type. */
    if (arg1[0] == '\0') {
        send_to_char("要拿什麼?\n\r", ch);
        return 0;
    }

    if (arg2[0] == '\0') {
        if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
            /* 'get obj' */
            obj = get_obj_list(ch, arg1, ch->in_room->contents);
            if (!obj) {
                act("這裡沒有 $T.", ch, NULL, arg1, TO_CHAR);
                return 0;
            }

            get_obj(ch, obj, NULL);
        } else {
            /* 'get all' or 'get all.obj' */
            OBJ_DATA *obj_next;

            found = FALSE;
            for (obj = ch->in_room->contents; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj)) {
                    found = TRUE;
                    get_obj(ch, obj, NULL);
                }
            }

            if (!found) {
                if (arg1[3] == '\0')
                    send_to_char("這裡沒有任何物品.\n\r", ch);
                else
                    act("這裡沒有 $T.", ch, NULL, &arg1[4], TO_CHAR);
            }
        }
    } else {
        /* 'get ... container' */
        if (!str_cmp(arg2, "all") || !str_prefix("all.", arg2)) {
            send_to_char("你無法這麼做.\n\r", ch);
            return 0;
        }

        if (!(container = get_obj_here(ch, arg2))) {
            act("這裡沒有 $T.", ch, NULL, arg2, TO_CHAR);
            return 0;
        }

        switch (container->item_type) {
        default:
            send_to_char("那不是個容器.\n\r", ch);
            return 0;

        case ITEM_CONTAINER:
        case ITEM_CORPSE_NPC:
            break;

        case ITEM_CORPSE_PC: {
            CHAR_DATA *gch;
            char *pd, *cd;
            char name[MAX_INPUT_LENGTH];
            char chname[MAX_INPUT_LENGTH];

            if (IS_NPC(ch)) {
                send_to_char("你無法這麼做.\n\r", ch);
                return 0;
            }

            pd = container->short_descr; /*short_descr;
             pd = one_argument( pd, name );
             pd = one_argument( pd, name );*/
            cd = ch->name;
            cd = one_argument(cd, chname);

            if (str_cmp(name, chname) && !IS_IMMORTAL(ch)) {
                bool fGroup;

                fGroup = FALSE;
                for (gch = char_list; gch; gch = gch->next) {
                    if (!IS_NPC(gch) && is_same_group(ch, gch) &&
                        !str_cmp(name, gch->name)) {
                        fGroup = TRUE;
                        break;
                    }
                }
                if (!fGroup) {
                    send_to_char("你無法這麼做.\n\r", ch);
                    return 0;
                }
            }
        }
        }

        if (container->item_type == ITEM_CONTAINER &&
            IS_SET(container->value[1], CONT_CLOSED)) {
            act("$d 是關著的.", ch, NULL, container->name, TO_CHAR);
            return 0;
        }

        if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
            int value;
            /* 'get obj container' */
            obj = get_obj_list(ch, arg1, container->contains);
            if (!obj) {
                // auto get corpse 沒東西不會顯示字串 2022/05/08
                // act( "$T 裡沒有這樣東西.", ch, NULL, arg2, TO_CHAR );
                return 0;
            }
            value = obj->value[0];
            get_obj(ch, obj, container);
            return value;
        } else {
            /* 'get all container' or 'get all.obj container' */
            OBJ_DATA *obj_next;

            found = FALSE;
            for (obj = container->contains; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj)) {
                    found = TRUE;
                    get_obj(ch, obj, container);
                }
            }
            /*  auto get corpse 沒東西不會顯示字串 2022/05/08
            if ( !found )
            {
                    if ( arg1[3] == '\0' )
                            act( "$T 裡沒有這樣東西.", ch, NULL, arg2, TO_CHAR
            ); else act( "$T 裡沒有這樣東西.", ch, NULL, arg2, TO_CHAR );
            } */
        }
    }

    return 0;
}

BANK_DATA *new_bank() {
    BANK_DATA *bank = NULL;

    if (!eqbank_free) {
        bank = alloc_mem(sizeof(*bank));

        ++top_bank;
    } else {
        bank = eqbank_free;
        eqbank_free = bank->next;
    }
    bank->name = NULL;
    bank->next = NULL;
    bank->password = NULL;
    bank->objs = NULL;
    bank->amount = 0;
    bank->opened = TRUE;
    return bank;
}

// fwrite_neweqbank: Writes equipment bank data to a file.
// fwrite_neweqbank: 將新裝備銀行的資料寫入檔案中。
void fwrite_neweqbank(CHAR_DATA *ch, FILE *fp) {

    if (!ch->neweqbank)
        return;

    fprintf(fp, "#EQBANK_DATA\n");
    fprintf(fp, "%s~\n", ch->neweqbank->name);
    fprintf(fp, "%s~\n", ch->neweqbank->password);
    fprintf(fp, "%d\t", ch->neweqbank->amount);
    fprintf(fp, "%d\n", ch->neweqbank->opened);
    if (ch->neweqbank->objs)
        fwrite_newbank_obj(ch, ch->neweqbank->objs, fp, 0);
    fprintf(fp, "#END\n");
    return;
}

// fread_neweqbank: Reads equipment bank data from a file.
// fread_neweqbank: 從檔案中讀取新裝備銀行的資料。
int fread_neweqbank(BANK_DATA *ch, FILE *fp) {

    char letter;
    char *word;
    int status;
    for (;;) {
        letter = fread_letter(fp);
        if (letter != '#') {
            bug("[read_neweqbank] : # not found", 0);
            return FALSE;
        }
        word = fread_word(fp, &status);

        if (!str_cmp(word, "EQBANK_DATA")) {
            ch->name = fread_string(fp, &status);
            ch->password = fread_string(fp, &status);
            ch->amount = fread_number(fp, &status);
            ch->opened = fread_number(fp, &status);
            continue;
        } else if (!str_cmp(word, "EQBANK_OBJECT")) {

            if (!fread_newbank_obj(ch, fp))
                return FALSE;
            continue;
        } else if (!str_cmp(word, "END")) {
            break;
        } else {
            bug("[read_neweqbank]: unknown section name.", 0);
            return FALSE;
        }
    }

    return TRUE;
}

bool check_parse_name(char *name);
// free_neweqbank: Frees memory allocated for an equipment bank.
// free_neweqbank: 釋放新裝備銀行所佔用的記憶體空間。
void free_neweqbank(BANK_DATA *bank) {
    OBJ_DATA *obj, *nobj;
    free_string(bank->name);
    free_string(bank->password);
    for (obj = bank->objs; obj; obj = nobj) {
        nobj = obj->next_content;
        obj->deleted = TRUE;
        obj->next = object_list;
        object_list = obj;
        obj->next_content = NULL;
    }
    /*
    for( obj = bank->objs ; obj; obj = nobj){
  if( obj->contains ) {
      OBJ_DATA * nnobj;
      for(nobj = obj->contains; nobj; nobj = nnobj ){
    nnobj = nobj->next_content;
    nobj->deleted = TRUE;
    nobj->next_content = NULL;
    free_neweqbank_obj( nobj );
      }
  }
  nobj = obj->next_content;
  obj->deleted = TRUE;
  obj->next_content = NULL;
  free_neweqbank_obj( obj );
    }*/
    bank->name = NULL;
    bank->password = NULL;
    bank->objs = NULL;
    bank->next = eqbank_free;
    eqbank_free = bank;
    return;
}

// check_container: Validates a container's properties and security status.
// check_container: 驗證容器物品的屬性與安全性狀態。
bool check_container(CHAR_DATA *ch, OBJ_DATA *container) {
    OBJ_DATA *obj;
    int O_Weight = 0; // 2022/12/01
    int O_Number = 0; // 2022/12/01

    for (obj = container->contains; obj; obj = obj->next_content) {
        // 內容物的 container 包有 container 將無法保存 2022/12/01
        if (obj->item_type == ITEM_CONTAINER && !(obj->contains == NULL)) {
            send_to_char("裝備銀行老闆聳聳肩說：「對不起，我們不收容器內的容器"
                         "裡面還放著物品的容器。」\n\r",
                         ch);
            return FALSE;
        }
        // container 的內容物可以保存 key 及有 timer 的 eq , 加入無法存入 horse
        // 2022/12/01
        if ((ch->level < (obj->level - 5) && ch->class < 5)
            //|| obj->item_type == ITEM_KEY
            || obj->item_type == ITEM_HORSE ||
            IS_SET(obj->extra_flags, ITEM_NOSAVE) ||
            IS_SET(obj->extra_flags, ITEM_PERSONAL) ||
            IS_SET(obj->extra_flags, ITEM_UNIQUE) || obj->deleted)
        //|| obj->timer > 0 )
        {
            send_to_char("裝備銀行老闆聳聳肩說：「對不起，這容器內放著我們不收"
                         "的物品。」\n\r",
                         ch);
            return FALSE;
        }
        if (obj->weight > 0)
            O_Weight += obj->weight;
        ++O_Number;
    }

    // 避免無法取出, 增加總重量及物品數計算 2022/12/01
    if (container->value[3] > 100)
        O_Weight = (O_Weight * container->value[3]) / 100;
    else if (container->value[3] == 100)
        O_Weight = O_Weight - 1;
    else if (container->value[3] > 0)
        O_Weight = (O_Weight * (100 - container->value[3])) / 100;
    else if (container->value[3] < 0)
        O_Number = O_Number + container->value[3];

    if (O_Weight > 800) {
        send_to_char("裝備銀行老闆聳聳肩說：「對不起，這件容器的總重量超過 800 "
                     "公斤。」\n\r",
                     ch);
        return FALSE;
    }

    if (O_Number > 90) {
        send_to_char("裝備銀行老闆聳聳肩說：「對不起，這件容器的物品數量超過 "
                     "90 件。」\n\r",
                     ch);
        return FALSE;
    }

    return TRUE;
}

// check_eqbank: Validates an equipment bank structure.
// check_eqbank: 驗證裝備銀行結構的有效性。
bool check_eqbank(BANK_DATA *bank) {
    DESCRIPTOR_DATA *d;
    CHAR_DATA *to;

    for (d = descriptor_list; d; d = d->next) {
        to = (d->original) ? d->original : d->character;
        if (!to)
            continue;
        if (to->neweqbank) {
            if (!str_cmp(to->neweqbank->name, bank->name))
                return TRUE;
        }
    }

    return FALSE;
}
// do_eqbank: Command: do_eqbank - Performs the 'eqbank' command.
// do_eqbank: 指令：do_eqbank - 執行 'eqbank' 指令。
void do_eqbank(CHAR_DATA *ch, char *argument) {
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char out_buf[MAX_STRING_LENGTH * 12];
    char tmp[255];
    char no_ansi[255];
    CHAR_DATA *vch = NULL;
    BANK_DATA *bank = NULL;
    OBJ_DATA *obj = NULL;
    OBJ_DATA *vobj = NULL;
    bool fBank = TRUE;
    int count = 0;
    FILE *fp = NULL;
    int i = 0;
    int cost = 0;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    argument = one_argument(argument, arg3);

    if (IS_NPC(ch))
        return;

    if (!ch->in_room || !ch->in_room->area)
        return;

    if (IS_SET(ch->act, PLR_HORSE))
        return;

    for (vch = ch->in_room->people; vch; vch = vch->next_in_room) {
        // 加入 npc 限定, 並修改 eqbanker 的判定由 name 變更為 qmark 2022/12/01
        if (!IS_NPC(vch))
            continue;
        // if ( is_name( "sharas-eqb", vch->name ) )
        if (is_qmark(vch, "is_eqbanker"))
            break;
    }

    if (!vch) {
        send_to_char("裝備銀行老闆不在這兒。\n\r", ch);
        return;
    }

    if (arg1[0] == '\0') {
        send_to_char("請 help eqbank 以取得用法說明。\n\r", ch);
        return;
    }

    if (!str_cmp(arg1, "passwd")) {
        if (!ch->neweqbank) {
            send_to_char("裝備銀行老闆皺著眉說：「您得先打開倉庫。」\n\r", ch);
            return;
        }

        if (str_cmp(arg2, arg3)) {
            send_to_char("裝備銀行老闆皺著眉說：「您兩次的密碼不一樣。」\n\r",
                         ch);
            return;
        }

        free_string(ch->neweqbank->password);
        ch->neweqbank->password = str_dup(crypt(arg3, ch->neweqbank->name));
        sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(ch->neweqbank->name),
                capitalize(ch->neweqbank->name));

        if ((fp = fopen(tmp, "w"))) {
            fwrite_neweqbank(ch, fp);
            fclose(fp);
        }

        send_to_char("裝備銀行老闆說著：「密碼設定完成。」\n\r", ch);
        return;
    }

    if (!str_cmp(arg1, "new")) {
        if (ch->neweqbank) {
            send_to_char("裝備銀行老闆皺著眉說：「您得先把使用中的倉庫給關閉("
                         "close)。」\n\r",
                         ch);
            return;
        }

        if (strlen(arg1) > 20) {
            send_to_char("裝備銀行老闆皺著眉說：「您新倉庫的名字太長了。」\n\r",
                         ch);
            return;
        }

        if (!check_parse_name(arg2)) {
            send_to_char("裝備銀行老闆皺著眉說：「那是不合法的名字。」\n\r",
                         ch);
            return;
        }

        if (arg2[0] == '\0' || arg3[0] == '\0') {
            send_to_char("裝備銀行老闆皺著眉說：「您得告訴我新倉庫的名字和密碼"
                         "啊。」\n\r",
                         ch);
            return;
        }

        sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(arg2), capitalize(arg2));

        if ((fp = fopen(tmp, "r"))) {
            fclose(fp);
            send_to_char(
                "裝備銀行老闆皺著眉說：「真抱歉，這個名字已經有人用了。」\n\r",
                ch);
            return;
        }

        sprintf(tmp, "%s%s/%s.gz", PLAYER_DIR, initial(arg2), capitalize(arg2));

        if ((fp = fopen(tmp, "r"))) {
            fclose(fp);
            if (str_cmp(arg2, ch->name)) {
                send_to_char("裝備銀行老闆皺著眉說：「抱歉，請不要用其他玩家的"
                             "名字。」\n\r",
                             ch);
                return;
            }
        }

        sprintf(tmp, "%s%s/%s", PLAYER_DIR, initial(arg2), capitalize(arg2));

        if ((fp = fopen(tmp, "r"))) {
            fclose(fp);
            if (str_cmp(arg2, ch->name)) {
                send_to_char("裝備銀行老闆皺著眉說：「抱歉，請不要用其他玩家的"
                             "名字。」\n\r",
                             ch);
                return;
            }
        }

        cost = 500000;
        fBank = TRUE;

        if (ch->bank < cost) {
            fBank = FALSE;
            if (ch->gold < cost) {
                send_to_char(
                    "裝備銀行老闆皺著眉說：「真抱歉，你的錢不夠。」\n\r", ch);
                return;
            }
            ch->gold -= cost;
        } else {
            if (ch->bank < cost) {
                send_to_char(
                    "裝備銀行老闆皺著眉說：「真抱歉，你的錢不夠。」\n\r", ch);
                return;
            }
            ch->bank -= cost;
        }

        bank = new_bank();
        bank->name = str_dup(arg2);
        bank->password = str_dup(crypt(arg3, arg2));
        bank->opened = TRUE;
        ch->neweqbank = bank;

        sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(arg2), capitalize(arg2));

        if ((fp = fopen(tmp, "w"))) {
            fwrite_neweqbank(ch, fp);
            fclose(fp);
        }

        sprintf(tmp, "%s opened new storage '%s'", ch->name, arg2);
        bug(tmp, 0);
        send_to_char("裝備銀行老闆說著：「開戶完成。」\n\r", ch);
        if (fBank)
            send_to_char("你從銀行轉帳，付出了 " HIY, ch);
        else
            send_to_char("你付出了 " HIY, ch);
        sprintf(tmp, "%d", cost);
        send_to_char(tmp, ch);
        send_to_char(NOR " 元的開戶費。\n\r", ch);
        return;
    }

    if (!str_cmp(arg1, "backup")) {
        if (!ch->neweqbank) {
            send_to_char("裝備銀行老闆皺著眉說：「麻煩請先打開個倉庫。」\n\r",
                         ch);
            return;
        }
        sprintf(tmp, "%s%s/%s", EQBANK_BACKUPDIR, initial(ch->neweqbank->name),
                capitalize(ch->neweqbank->name));
        ch->neweqbank->opened = FALSE;
        if ((fp = fopen(tmp, "w"))) {
            // fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
            fwrite_neweqbank(ch, fp);
            fclose(fp);
            send_to_char("裝備銀行老闆說著：「倉庫備份完成。」\n\r", ch);
        } else {
            bug("eqbank: file open error!!", 0);
            send_to_char("發生嚴重問題，請向大神回報。\n\r", ch);
            return;
        }
        ch->neweqbank->opened = TRUE;
        return;
    }

    if (!str_prefix(arg1, "store")) {
        if (!ch->neweqbank) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您得先打開個倉庫。」\n\r", ch);
            return;
        }
        if (arg2[0] == '\0') {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您要存放什麼裝備？」\n\r", ch);
            return;
        }

        if ((obj = get_obj_carry(ch, arg2))) {
            cost = (obj->level < 5)    ? 8000
                   : (obj->level < 36) ? obj->level * 200
                                       : obj->level * 500;

            if (obj->item_type == ITEM_CONTAINER) {
                cost = (10000 * (get_obj_number(obj) + 1));
            }

            if (obj->timer > 0)
                cost *= 10;

            if (ch->bank < cost) {
                fBank = FALSE;
                if (ch->gold < cost) {
                    send_to_char("裝備銀行老闆苦笑著說：「對不起，沒有足夠的保"
                                 "管費我無法雇用保全人員。」\n\r",
                                 ch);
                    return;
                }
            }

            if (ch->neweqbank->amount > 150) {
                send_to_char("裝備銀行老闆苦笑著說：「對不起，您庫存的裝備數量"
                             "已達 150 件的上限。」\n\r",
                             ch);
                return;
            }

            // 單一物品重量上限 2022/12/01
            if (obj->weight > 800) {
                send_to_char("裝備銀行老闆聳聳肩說：「對不起，每件裝備的重量不"
                             "能超過 800 公斤。」\n\r",
                             ch);
                return;
            }

            // 移除 item_key 不能存, 加入item_horse 不能存 2022/12/01
            if ((ch->level < (obj->level - 5) && ch->class < 5)
                //|| obj->item_type == ITEM_KEY
                || obj->item_type == ITEM_HORSE ||
                IS_SET(obj->extra_flags, ITEM_NOSAVE) ||
                IS_SET(obj->extra_flags, ITEM_PERSONAL) ||
                IS_SET(obj->extra_flags, ITEM_UNIQUE) || obj->deleted)
            //|| obj->timer > 0 )
            {
                send_to_char(
                    "裝備銀行老闆聳聳肩說：「對不起，這類裝備我們不收。」\n\r",
                    ch);
                return;
            }

            if (obj->item_type == ITEM_CONTAINER && !check_container(ch, obj))
                return;

            obj_from_char(obj);
            obj->value[4] = ch->in_room->area->recall;
            if (obj->item_type == ITEM_CONTAINER) {
                OBJ_DATA *vobj_next = NULL;
                for (vobj = object_list; vobj; vobj = vobj_next) {
                    if (!object_list->next || !vobj->next)
                        break;
                    // if( object_list->next == object_list ) break;
                    if (vobj->next == vobj)
                        break;

                    if (object_list == obj) {
                        object_list->value[4] = ch->in_room->area->recall;
                        vobj_next = obj->next;
                        object_list = obj->next;
                        obj->next = NULL;
                        continue;
                    }
                    if (object_list->in_obj == obj) {
                        object_list->value[4] = ch->in_room->area->recall;
                        vobj_next = object_list->next;
                        object_list->next = NULL;
                        object_list = vobj_next;
                        continue;
                    }
                    if (vobj->next == obj) {
                        vobj->next->value[4] = ch->in_room->area->recall;
                        vobj_next = vobj;
                        vobj->next = obj->next;
                        obj->next = NULL;
                        continue;
                    }
                    if (vobj->next->in_obj == obj) {
                        vobj->next->value[4] = ch->in_room->area->recall;
                        vobj_next = vobj->next;
                        vobj->next = vobj->next->next;
                        vobj_next->next = NULL;
                        vobj_next = vobj;
                        continue;
                    }
                    vobj_next = vobj->next;
                }
            } else {
                if (obj == object_list) {
                    object_list = obj->next;
                    obj->next = NULL;
                } else {
                    for (vobj = object_list; vobj; vobj = vobj->next) {
                        if (obj == vobj->next) {
                            vobj->next = obj->next;
                            obj->next = NULL;
                            break;
                        }
                    }
                }
            }

            obj->next_content = ch->neweqbank->objs;
            ch->neweqbank->objs = obj;

            if (fBank)
                ch->bank -= cost;
            else
                ch->gold -= cost;

            // 移除 to_room 避免大量存取物品造成他人畫面凌亂 2022/02/01
            // act( "裝備銀行老闆小心地將$n的$p拿到後面的保險庫存放。", ch, obj,
            // NULL, TO_ROOM );
            act("裝備銀行老闆小心地將你的$p拿到後面的保險庫存放。", ch, obj,
                NULL, TO_CHAR);

            if (fBank)
                send_to_char("你從銀行轉帳，付出了 " HIY, ch);
            else
                send_to_char("你付出了 " HIY, ch);
            sprintf(tmp, "%d", cost);
            send_to_char(tmp, ch);
            send_to_char(NOR " 元的保管費。\n\r", ch);
            do_save(ch, "");
            ch->neweqbank->amount += 1;
            sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(ch->neweqbank->name),
                    capitalize(ch->neweqbank->name));
            if ((fp = fopen(tmp, "w"))) {
                // fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
                fwrite_neweqbank(ch, fp);
                fclose(fp);
                return;
            } else {
                bug("eqbank: file open error.", 0);
                return;
            }
            return;
        }
        send_to_char("裝備銀行老闆狐疑地看著你說：「我沒看到這項裝備啊？」\n\r",
                     ch);
        return;
    }

    if (!str_cmp(arg1, "fopen")) {
        if (ch->neweqbank) {
            send_to_char(
                "裝備銀行老闆皺著眉說：「麻煩請先關上使用中的倉庫。」\n\r", ch);
            return;
        }
        if (get_trust(ch) < 43) {
            send_to_char("裝備銀行老闆皺著眉說：「對不起，您的等級不夠。」\n\r",
                         ch);
            return;
        }
        if (arg2[0] == '\0') {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「請告訴我倉庫名稱。」\n\r", ch);
            return;
        }

        sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(arg2), capitalize(arg2));
        bank = new_bank();

        if ((fp = fopen(tmp, "r"))) {
            fread_neweqbank(bank, fp);
            fclose(fp);
        } else {
            send_to_char("裝備銀行老闆狐疑地看著你說：「沒有這個倉庫哦。」\n\r",
                         ch);
            return;
        }
        ch->neweqbank = bank;
        bank->opened = TRUE;
        if ((fp = fopen(tmp, "w"))) {
            // fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
            fwrite_neweqbank(ch, fp);
            fclose(fp);
        } else {
            bug("eqbank: file open error.", 0);
        }
        send_to_char("裝備銀行老闆說著：「開啟成功。」\n\r", ch);
        return;
    }

    /*if( !str_cmp(arg1, "forceopen")){
    if( ch->neweqbank ) {
    send_to_char( "裝備銀行老闆皺著眉說：「麻煩請先關上使用中的倉庫。」\n\r", ch
    ); return;
    }
    if( arg2[0] == '\0' || arg3[0] == '\0' ){
    send_to_char(
    "裝備銀行老闆狐疑地看著你說：「請告訴我倉庫名稱和密碼。」\n\r", ch );
    return;
    }
    sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
    bank = new_bank();
    if( (fp= fopen(tmp, "r"))){
    fread_neweqbank(bank, fp);
    fclose(fp);
    }
    else {
    send_to_char( "裝備銀行老闆狐疑地看著你說：「沒有這個倉庫哦。」\n\r", ch );
    return;
    }
    if( str_cmp( bank->password, crypt(arg3, arg2))){
    send_to_char( "裝備銀行老闆狐疑地看著你說：「您確定密碼是對的嗎？。」\n\r",
    ch ); return;
    }
    ch->neweqbank = bank;
    bank->opened = TRUE;
    bugf("%s opened bank [%s]", ch->name, arg2);
    bank->recall = ch->in_room->area->recall;
    if(( fp = fopen(tmp, "w"))){
    //fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
    fwrite_neweqbank(ch, fp);
    fclose(fp);
    }
    else{
    bug("eqbank: file open error.",0);
    }
    send_to_char( "裝備銀行老闆說著：「開啟成功\。」\n\r", ch );
    return;

    }*/

    if (!str_cmp(arg1, "open")) {
        if (ch->neweqbank) {
            send_to_char(
                "裝備銀行老闆皺著眉說：「麻煩請先關上使用中的倉庫。」\n\r", ch);
            return;
        }
        if (arg2[0] == '\0' || arg3[0] == '\0') {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「請告訴我倉庫名稱和密碼。」\n\r",
                ch);
            return;
        }
        sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(arg2), capitalize(arg2));
        bank = new_bank();
        if ((fp = fopen(tmp, "r"))) {
            fread_neweqbank(bank, fp);
            fclose(fp);
        } else {
            send_to_char("裝備銀行老闆狐疑地看著你說：「沒有這個倉庫哦。」\n\r",
                         ch);
            return;
        }
        if (str_cmp(bank->password, crypt(arg3, arg2))) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您確定密碼是對的嗎？。」\n\r",
                ch);
            free_neweqbank(bank);
            return;
        }
        if (bank->opened) {
            if (check_eqbank(bank)) {
                send_to_char("裝備銀行老闆說著：「抱歉，這個倉庫目前有別人在使"
                             "用中。」\n\r",
                             ch);
                free_neweqbank(bank);
                return;
            }
        }
        ch->neweqbank = bank;
        bank->opened = TRUE;
        bank->recall = ch->in_room->area->recall;
        if ((fp = fopen(tmp, "w"))) {
            // fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
            fwrite_neweqbank(ch, fp);
            fclose(fp);
        } else {
            bug("eqbank: file open error.", 0);
        }
        send_to_char("裝備銀行老闆說著：「開啟成功。」\n\r", ch);
        return;
    }

    if (!str_prefix(arg1, "exam")) // 完成 eqbank exam 的指令 2022/12/01
    {
        if (!ch->neweqbank) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您得先打開個倉庫。」\n\r", ch);
            return;
        }
        if (!is_number(arg2)) {
            send_to_char("請用 'eqbank exam 編號' 來檢視您的裝備。\n\r", ch);
            return;
        }
        if (!ch->neweqbank->objs) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您的倉庫裡沒有東西啊？」\n\r",
                ch);
            return;
        }

        count = atoi(arg2);

        i = 1;

        if (count == 1 &&
            ch->neweqbank->objs->value[4] == ch->in_room->area->recall) {
            obj = ch->neweqbank->objs;
        } else if (ch->neweqbank->objs
                       ->next_content) //&& ch->neweqbank->objs->next_content )
        {
            if (ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
                ++i;
            for (vobj = ch->neweqbank->objs; vobj; vobj = vobj->next_content) {
                if (!vobj->next_content)
                    break;
                if (vobj->next_content->value[4] != ch->in_room->area->recall)
                    continue;
                if (count == i) {
                    if (vobj->next_content->value[4] ==
                        ch->in_room->area->recall) {
                        obj = vobj->next_content;
                        break;
                    } else
                        continue;
                }
                i++;
            }
        }
        if (obj) {
            eqb_do_examine(ch, obj);
            return;
        }
        send_to_char("裝備銀行老闆查閱了櫃臺上的登記簿說：奇怪，沒有這項裝備的"
                     "紀錄喔！\n\r",
                     ch);
        return;
    }

    if (!str_prefix(arg1, "id")) {
        if (!ch->neweqbank) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您得先打開個倉庫。」\n\r", ch);
            return;
        }
        if (!is_number(arg2)) {
            send_to_char("請用 'eqbank id 編號' 來鑑定您的裝備。\n\r", ch);
            return;
        }
        if (!ch->neweqbank->objs) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您的倉庫裡沒有東西啊？」\n\r",
                ch);
            return;
        }

        count = atoi(arg2);

        i = 1;
        if (count == 1 &&
            ch->neweqbank->objs->value[4] == ch->in_room->area->recall) {
            obj = ch->neweqbank->objs;
        } else if (ch->neweqbank->objs
                       ->next_content) //&& ch->neweqbank->objs->next_content )
        {
            if (ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
                ++i;
            for (vobj = ch->neweqbank->objs; vobj; vobj = vobj->next_content) {
                if (!vobj->next_content)
                    break;
                if (vobj->next_content->value[4] != ch->in_room->area->recall)
                    continue;
                if (count == i) {
                    if (vobj->next_content->value[4] ==
                        ch->in_room->area->recall) {
                        obj = vobj->next_content;
                        break;
                    } else
                        continue;
                }
                i++;
            }
        }
        if (obj) {
            cost = URANGE(300, obj->cost / 10, 3000);
            if (ch->bank < cost) {
                fBank = FALSE;
                if (ch->gold < cost) {
                    send_to_char("裝備銀行老闆苦笑著說：「對不起，沒有足夠的鑑"
                                 "定費我無法雇用保全人員。」\n\r",
                                 ch);
                    return;
                }
            }
            if (fBank)
                ch->bank -= cost;
            else
                ch->gold -= cost;
            if (fBank)
                send_to_char("你從銀行轉帳，付出了 " HIY, ch);
            else
                send_to_char("你付出了 " HIY, ch);
            sprintf(tmp, "%d", cost);
            send_to_char(tmp, ch);
            send_to_char(NOR " 元的鑑定費。\n\r", ch);
            spell_identify(gsn_invoke, ch->level, ch, obj);
            return;
        }
        send_to_char("裝備銀行老闆查閱了櫃臺上的登記簿說：奇怪，沒有這項裝備的"
                     "紀錄喔！\n\r",
                     ch);
        return;
    }

    if (!str_prefix(arg1, "receive")) {
        if (!ch->neweqbank) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您得先打開個倉庫。」\n\r", ch);
            return;
        }
        if (!is_number(arg2)) {
            send_to_char("請用 'eqbank receive 編號' 來領取您的裝備。\n\r", ch);
            return;
        }
        if (!ch->neweqbank->objs) {
            send_to_char(
                "裝備銀行老闆狐疑地看著你說：「您的倉庫裡沒有東西啊？」\n\r",
                ch);
            return;
        }

        count = atoi(arg2);

        i = 1;
        if (count == 1 &&
            ch->neweqbank->objs->value[4] == ch->in_room->area->recall) {
            obj = ch->neweqbank->objs;
            if (ch->carry_number + get_obj_number(obj) > can_carry_n(ch)) {
                do_say(vch, "對不起，您拿不動更多東西了。");
                return;
            }

            if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch)) {
                do_say(vch, "對不起，您的負重不夠了。");
                return;
            }
            ch->neweqbank->objs = obj->next_content;
            obj->next_content = NULL;
        } else if (ch->neweqbank->objs
                       ->next_content) //&& ch->neweqbank->objs->next_content )
        {
            if (ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
                ++i;
            for (vobj = ch->neweqbank->objs; vobj; vobj = vobj->next_content) {
                if (!vobj->next_content)
                    break;
                if (vobj->next_content->value[4] != ch->in_room->area->recall)
                    continue;
                if (count == i) {
                    if (vobj->next_content->value[4] ==
                        ch->in_room->area->recall) {
                        obj = vobj->next_content;
                        if (ch->carry_number + get_obj_number(obj) >
                            can_carry_n(ch)) {
                            do_say(vch, "對不起，您拿不動更多東西了。");
                            return;
                        }

                        if (ch->carry_weight + get_obj_weight(obj) >
                            can_carry_w(ch)) {
                            do_say(vch, "對不起，您的負重不夠了。");
                            return;
                        }
                        vobj->next_content = obj->next_content;
                        obj->next_content = NULL;
                        break;
                    } else
                        continue;
                }
                i++;
            }
        }
        if (obj) {
            /*
            if( obj->item_type == ITEM_CONTAINER ) {
            OBJ_DATA * vobj_next = NULL;
            for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj_next ){
            if( ch->neweqbank->objs->in_obj == obj ) {
            vobj_next = ch->neweqbank->objs->next_content;
            ch->neweqbank->objs->next = object_list;
            object_list = ch->neweqbank->objs;
            object_list->next_content = obj->contains;
            obj->contains = object_list;
            ch->neweqbank->objs = vobj_next;
            continue;
            }
            if( vobj->next_content->in_obj == obj ) {
            vobj_next = vobj;
            vobj->next_content->next = object_list;
            object_list = vobj->next_content;
            vobj->next_content = vobj->next_content->next_content;
            object_list->next_content = obj->contains;
            obj->contains = object_list;
            continue;
            }
            }
            }*/

            if (obj->item_type == ITEM_CONTAINER) {
                for (vobj = obj->contains; vobj; vobj = vobj->next_content) {
                    vobj->next = object_list;
                    object_list = vobj;
                }
            }
            obj_to_char(obj, ch);
            obj->next = object_list;
            object_list = obj;
            // 移除 to_room 避免大量存取物品造成他人畫面凌亂 2022/02/01
            // act( "裝備銀行老闆自後面的庫房將$p捧來交給$n。", ch, obj, NULL,
            // TO_ROOM ); act(
            // "裝備銀行老闆一鞠躬說：「請您檢查有無任何損傷，謝謝光臨。」", ch,
            // obj, NULL, TO_ROOM );
            act("裝備銀行老闆自後面的庫房將$p捧來交給你。", ch, obj, NULL,
                TO_CHAR);
            act("裝備銀行老闆一鞠躬說：「請您檢查有無任何損傷，謝謝光臨。」",
                ch, obj, NULL, TO_CHAR);
            ch->neweqbank->amount -= 1;
            sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(ch->neweqbank->name),
                    capitalize(ch->neweqbank->name));
            if ((fp = fopen(tmp, "w"))) {
                // fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
                fwrite_neweqbank(ch, fp);
                fclose(fp);
                do_save(ch, "");
                return;
            } else {
                bug("eqbank: file open error.", 0);
                do_save(ch, "");
                return;
            }
            return;
        }
        send_to_char("裝備銀行老闆查閱了櫃臺上的登記簿說：奇怪，沒有這項裝備的"
                     "紀錄喔！\n\r",
                     ch);
        return;
    }

    if (!str_cmp(arg1, "list")) {
        if (!ch->neweqbank) {
            send_to_char("裝備銀行老闆說著：「您還沒打開倉庫呢。」\n\r", ch);
            return;
        }

        send_to_char("你目前存在裝備銀行保險庫的裝備有：\n\r", ch);

        if (!ch->neweqbank->objs) {
            send_to_char("\n\r什麼都沒有！\n\r\n\r", ch);
            return;
        }

        send_to_char("【編號】  物  品  名  稱                                 "
                     " 等級\n\r",
                     ch);

        for (vobj = ch->neweqbank->objs; vobj;) {
            out_buf[0] = 0;

            /* 有 Buffer 版本 *

                    for(count = 0; count < 50 && vobj; count++, vobj =
            vobj->next_content){ if( vobj->in_obj ) continue; if( vobj->value[4]
            != ch->in_room->area->recall ) continue; i++;
                    //add by jye 0940619
                    if( arg2[0] != '\0')
                    {
                    if( !is_pfxname( arg2, vobj->name )) continue;
                    }
                    strip_ansi(vobj->short_descr, no_ansi);
                    sprintf ( tmp, "【%4d】%-48s  [%2d] \n\r",
                    i,
                    can_see_obj(ch, vobj) ? no_ansi :
                    "不明物品",
                    vobj->level
                    );
                    strcat(out_buf, tmp);
                    }
                    send_to_char(out_buf, ch);

            /**/

            /* 無 Buffer 版本 */

            for (; vobj; vobj = vobj->next_content) {
                if (vobj->in_obj)
                    continue;
                if (vobj->value[4] != ch->in_room->area->recall)
                    continue;

                i++;

                if (arg2[0] != '\0') // add by jye 0940619
                {
                    if (!is_pfxname(arg2, vobj->name))
                        continue;
                }
                strip_ansi(vobj->short_descr, no_ansi);
                sprintf(tmp, "【%4d】%-48s  [%2d] \n\r", i,
                        can_see_obj(ch, vobj) ? no_ansi : "不明物品",
                        vobj->level);
                send_to_char(tmp, ch);
            }

            /**/
        }
        send_to_char("\n\r", ch);
        return;
    }

    if (!str_cmp(arg1, "close")) {
        if (!ch->neweqbank) {
            send_to_char("裝備銀行老闆狐疑地看著你說：「您得先打開個倉庫才能關"
                         "上。」\n\r",
                         ch);
            return;
        }

        sprintf(tmp, "%s%s/%s", EQBANK_DIR, initial(ch->neweqbank->name),
                capitalize(ch->neweqbank->name));
        ch->neweqbank->opened = FALSE;

        if ((fp = fopen(tmp, "w"))) {
            // fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
            fwrite_neweqbank(ch, fp);
            fclose(fp);
            send_to_char("裝備銀行老闆說著：「倉庫關閉完成。」\n\r", ch);
        } else {
            bug("eqbank: file open error!!", 0);
            send_to_char("發生嚴重問題，請向大神回報。\n\r", ch);
            return;
        }
        free_neweqbank(ch->neweqbank);
        ch->neweqbank = NULL;
        return;
    }
    send_to_char("請 help eqbank 以取得用法說明。\n\r", ch);
    return;
}

// do_get_silence: Command: do_get_silence - Performs the 'get_silence' command.
// do_get_silence: 指令：do_get_silence - 執行 'get_silence' 指令。
void do_get_silence(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *container;
    // char buf [ MAX_INPUT_LENGTH ];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    bool found;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    /* Get type. */
    if (arg1[0] == '\0') {
        send_to_char("要拿什麼?\n\r", ch);
        return;
    }

    if (arg2[0] == '\0') {
        if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
            /* 'get obj' */
            obj = get_obj_list(ch, arg1, ch->in_room->contents);
            if (!obj) {
                return;
            }

            get_obj(ch, obj, NULL);
        } else {
            /* 'get all' or 'get all.obj' */
            OBJ_DATA *obj_next;

            found = FALSE;
            for (obj = ch->in_room->contents; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj)) {
                    found = TRUE;
                    get_obj(ch, obj, NULL);
                }
            }
        }
    } else {
        /* 'get ... container' */
        if (!str_cmp(arg2, "all") || !str_prefix("all.", arg2)) {
            return;
        }
        /*
        sprintf( buf, "%s 的屍體(corpse)", ch->name );
        for ( obj = object_list; obj; obj = obj->next )
        {
        if( !str_cmp( buf, obj->short_descr ) )
        {
        send_to_char( "你先處理好你自己吧！\n\r", ch );
        return;
        }
        }
        */
        if (!(container = get_obj_list(ch, arg2, ch->in_room->contents))) {
            if (!(container = get_obj_here(ch, arg2))) {
                return;
            }
        }

        switch (container->item_type) {
        default:
            return;

        case ITEM_CONTAINER:
        case ITEM_CORPSE_NPC:
            break;

        case ITEM_CORPSE_PC: {
            CHAR_DATA *gch;
            char *pd, *cd;
            char name[MAX_INPUT_LENGTH];
            char chname[MAX_INPUT_LENGTH];

            if (IS_NPC(ch)) {
                return;
            }

            pd = container->short_descr; /*short_descr;
             pd = one_argument( pd, name );
             pd = one_argument( pd, name );*/
            pd = one_argument(pd, name);
            cd = ch->name;
            cd = one_argument(cd, chname);

            if (str_cmp(name, chname) && !IS_IMMORTAL(ch)) {
                bool fGroup;

                fGroup = FALSE;
                for (gch = char_list; gch; gch = gch->next) {
                    if (!IS_NPC(gch) && is_same_group(ch, gch) &&
                        !str_cmp(name, gch->name)) {
                        fGroup = TRUE;
                        break;
                    }
                }
                if (!fGroup) {
                    return;
                }
            }
        }
        }

        if (IS_SET(container->value[1], CONT_CLOSED)) {
            return;
        }

        if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
            /* 'get obj container' */
            obj = get_obj_list(ch, arg1, container->contains);
            if (!obj) {
                return;
            }
            get_obj(ch, obj, container);
        } else {
            /* 'get all container' or 'get all.obj container' */
            OBJ_DATA *obj_next;

            found = FALSE;
            for (obj = container->contains; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj)) {
                    found = TRUE;
                    get_obj(ch, obj, container);
                }
            }
        }
    }

    return;
}
// do_get: Performs the 'get' command to pick up items from the ground or containers.
// do_get: 執行 'get' 指令從地面或容器中拾取物品。
void do_get(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *container;
    // char      buf [ MAX_INPUT_LENGTH ];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    bool found;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    /* Get type. */
    if (arg1[0] == '\0') {
        send_to_char("要拿什麼?\n\r", ch);
        return;
    }

    if (arg2[0] == '\0') {
        if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
            /* 'get obj' */
            obj = get_obj_list(ch, arg1, ch->in_room->contents);
            if (!obj) {
                act("這裡沒有 $T.", ch, NULL, arg1, TO_CHAR);
                return;
            }

            get_obj(ch, obj, NULL);
        } else {
            /* 'get all' or 'get all.obj' */
            OBJ_DATA *obj_next;

            found = FALSE;
            for (obj = ch->in_room->contents; obj; obj = obj_next) {
                obj_next = obj->next_content;
                // if( obj->item_type == ITEM_CORPSE_PC && !IS_IMMORTAL(ch) )
                //   continue;
                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj)) {
                    found = TRUE;
                    get_obj(ch, obj, NULL);
                }
            }

            if (!found) {
                if (arg1[3] == '\0')
                    send_to_char("這裡沒有任何物品.\n\r", ch);
                else
                    act("這裡沒有 $T.", ch, NULL, &arg1[4], TO_CHAR);
            }
        }
    } else {
        /* 'get ... container' */
        if (!str_cmp(arg2, "all") || !str_prefix("all.", arg2)) {
            send_to_char("你無法這麼做.\n\r", ch);
            return;
        }
        /*
                sprintf( buf, "%s 的屍體(corpse)", ch->name );
                for ( obj = object_list; obj; obj = obj->next )
                {
                if( !str_cmp( buf, obj->short_descr ) )
                {
                  send_to_char( "你先處理好你自己吧！\n\r", ch );
                  return;
                }
                }
        */
        if (!(container = get_obj_list(ch, arg2, ch->in_room->contents))) {
            if (!(container = get_obj_here(ch, arg2))) {
                act("這裡沒有 $T.", ch, NULL, arg2, TO_CHAR);
                return;
            }
        }

        switch (container->item_type) {
        default:
            send_to_char("那不是個容器.\n\r", ch);
            return;

        case ITEM_CONTAINER:
        case ITEM_CORPSE_NPC:
        case ITEM_HORSE:
            break;

        case ITEM_CORPSE_PC: {
            // CHAR_DATA *gch;
            char *pd, *cd;
            char name[MAX_INPUT_LENGTH];
            char chname[MAX_INPUT_LENGTH];

            if (IS_NPC(ch)) {
                send_to_char("你無法這麼做.\n\r", ch);
                return;
            }

            pd = container->short_descr; /*short_descr;
             pd = one_argument( pd, name );
             pd = one_argument( pd, name );*/
            pd = one_argument(pd, name);
            cd = ch->name;
            cd = one_argument(cd, chname);

            if (str_cmp(name, chname) && !IS_IMMORTAL(ch)) {
                bool fGroup;

                fGroup = FALSE;
                /*for ( gch = char_list; gch; gch = gch->next )
                {
                  if ( !IS_NPC( gch )
                && is_same_group( ch, gch )
                                                  &&  !consented(ch, obj)
                && !str_cmp( name, gch->name ) )
                  {
                fGroup = TRUE;
                break;
                  }
                }*/
                if (!consented(ch, container)) {
                    send_to_char("你無法這麼做.\n\r", ch);
                    return;
                }
            }
            // 撿回屍體後可以再次進行 backup 的動作 2022/10/10
            if (is_qmark(ch, "death_stop_backup"))
                do_del_qmark(ch, "self death_stop_backup");
        }
        }

        if (container->item_type == ITEM_CONTAINER &&
            IS_SET(container->value[1], CONT_CLOSED)) {
            act("$d 是關著的.", ch, NULL, container->name, TO_CHAR);
            return;
        }

        if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
            /* 'get obj container' */
            obj = get_obj_list(ch, arg1, container->contains);
            if (!obj) {
                act("$T 裡沒有這樣東西.", ch, NULL, arg2, TO_CHAR);
                return;
            }
            get_obj(ch, obj, container);
        } else {
            /* 'get all container' or 'get all.obj container' */
            OBJ_DATA *obj_next;

            found = FALSE;
            for (obj = container->contains; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj)) {
                    found = TRUE;
                    get_obj(ch, obj, container);
                }
            }

            if (!found) {
                if (arg1[3] == '\0')
                    act("$T 裡沒有這樣東西.", ch, NULL, arg2, TO_CHAR);
                else
                    act("$T 裡沒有這樣東西.", ch, NULL, arg2, TO_CHAR);
            }
        }
    }
    save_char_obj(ch);
    return;
}

// do_put: Command: do_put - Performs the 'put' command.
// do_put: 指令：do_put - 執行 'put' 指令。
void do_put(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *container;
    OBJ_DATA *obj;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument =
        one_argument(argument, arg1); // get the first parameter from argument
    one_argument(argument, arg2);     // and remove it from argument, then get
    // the second parameter from argument.
    if (arg1[0] == '\0' || arg2[0] == '\0') {
        send_to_char("把什麼放進什麼？\n\r", ch);
        return;
    }

    if (!str_cmp(arg2, "all") || !str_prefix("all.", arg2)) {
        send_to_char("你無法這麼做.\n\r", ch);
        return;
    }

    if (!(container = get_obj_here(ch, arg2))) {
        act("這裡沒有 $T.", ch, NULL, arg2, TO_CHAR);
        return;
    }

    if (container->item_type != ITEM_CONTAINER &&
        container->item_type != ITEM_HORSE) {
        send_to_char("那不是個容器.\n\r", ch);
        return;
    }

    if (container->item_type == ITEM_CONTAINER &&
        IS_SET(container->value[1], CONT_CLOSED)) {
        act("$d 是關著的.", ch, NULL, container->name, TO_CHAR);
        return;
    }

    // add by coconet 0511 for check whether the horse can carry things or not,
    // if v0 = -1, means can't
    if (container->item_type == ITEM_HORSE && container->value[0] == -1) {
        send_to_char("牠無法負載任何東西喔。\n\r", ch);
        return;
    }

    if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
        /* 'put obj container' */
        if (!(obj = get_obj_carry(ch, arg1))) {
            send_to_char("你沒有這個東西.\n\r", ch);
            return;
        }

        if (obj == container) {
            send_to_char("你無法把它塞進它自己裡.\n\r", ch);
            return;
        }

        if (!can_drop_obj(ch, obj)) {
            send_to_char("你丟不掉.\n\r", ch);
            return;
        }

        if (container->item_type == ITEM_CONTAINER) {
            if (get_obj_weight(obj) + get_obj_weight(container) -
                    container->weight >
                container->value[0]) {
                if (container->value[0] != -1) {
                    send_to_char("放不進去.\n\r", ch);
                    return;
                }
            }
        } else {

            if (get_obj_weight(obj) + get_obj_weight(container) -
                    container->weight >
                container->value[0]) {
                send_to_char(" 馬匹已經負載太多東西了。\n\r", ch);
                return;
            }
        }

        if (container->item_type != ITEM_HORSE) {
            act("你把 $p 放入 $P.", ch, obj, container, TO_CHAR);
            act("$n 把 $p 放入 $P.", ch, obj, container, TO_ROOM);
        } else {
            act("你把 $p 放在 $P背上.", ch, obj, container, TO_CHAR);
            act("$n 把 $p 放在 $P背上.", ch, obj, container, TO_ROOM);
        }

        // 2022/02/12
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("put", ch, obj);
        }

        // add at 2022/02/12
        if (!obj) {
            save_char_obj(ch);
            return;
        }

        obj_from_char(obj);
        obj_to_obj(obj, container);
    } else {
        /* 'put all container' or 'put all.obj container' */
        OBJ_DATA *obj_next;

        if (container->item_type == ITEM_CONTAINER) {
            for (obj = ch->carrying; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj) && obj->wear_loc == WEAR_NONE &&
                    obj != container && can_drop_obj(ch, obj) &&
                    (get_obj_weight(obj) + get_obj_weight(container) <=
                         container->value[0] ||
                     container->value[0] == -1)) {
                    act("你把 $p 放入 $P.", ch, obj, container, TO_CHAR);
                    act("$n 把 $p 放入 $P.", ch, obj, container, TO_ROOM);

                    // 2022/02/12
                    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                        oprog_act_trigger("put", ch, obj);
                    }

                    // add at 2022/02/12
                    if (!obj)
                        continue;

                    obj_from_char(obj);
                    obj_to_obj(obj, container);
                }
            }
        } else {
            for (obj = ch->carrying; obj; obj = obj_next) {
                obj_next = obj->next_content;

                if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                    can_see_obj(ch, obj) && obj->wear_loc == WEAR_NONE &&
                    obj != container && can_drop_obj(ch, obj) &&
                    (get_obj_weight(obj) + get_obj_weight(container) <=
                     container->value[0])) {
                    act("你把 $p 放在 $P背上.", ch, obj, container, TO_CHAR);
                    act("$n 把 $p 放在 $P背上.", ch, obj, container, TO_ROOM);

                    // 2022/02/12
                    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                        oprog_act_trigger("put", ch, obj);
                    }

                    // obj 可能在 obj_act 時被 purge 2022/02/12
                    if (!obj)
                        continue;

                    obj_from_char(obj);
                    obj_to_obj(obj, container);
                }
            }
        }
    }
    save_char_obj(ch);
    return;
}

// do_drop: Performs the 'drop' command to drop items onto the ground.
// do_drop: 執行 'drop' 指令將身上攜帶的物品丟棄到地上。
void do_drop(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    bool found;

    argument = one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("幹啥?\n\r", ch);
        return;
    }

    if (is_number(arg)) {
        /* 'drop NNNN coins' */
        OBJ_DATA *obj_next;
        int amount;

        amount = atoi(arg);
        one_argument(argument, arg);

        if (amount <= 0 || str_prefix(arg, "coins")) {
            send_to_char("對不起, 你無法這麼做.\n\r", ch);
            return;
        }

        if (ch->class <= BARBARIAN && ch->level < 3) {
            send_to_char("你年紀還小，別亂丟金錢。\n\r", ch);
            return;
        }

        if (ch->gold < amount) {
            send_to_char("你沒有那麼多錢.\n\r", ch);
            return;
        }

        ch->gold -= amount;

        for (obj = ch->in_room->contents; obj; obj = obj_next) {
            obj_next = obj->next_content;

            if (obj->deleted)
                continue;

            switch (obj->pIndexData->vnum) {
            case OBJ_VNUM_MONEY_ONE:
                amount += 1;
                extract_obj(obj);
                break;

            case OBJ_VNUM_MONEY_SOME:
                amount += obj->value[0];
                extract_obj(obj);
                break;
            }
        }

        obj_to_room(create_money(amount), ch->in_room);
        send_to_char("OK.\n\r", ch);
        act("$n 丟下一些錢.", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (str_cmp(arg, "all") && str_prefix("all.", arg)) {
        /* 'drop obj' */
        if (!(obj = get_obj_carry(ch, arg))) {
            send_to_char("你沒有那樣東西.\n\r", ch);
            return;
        }

        if (!can_drop_obj(ch,
                          obj) // && !IS_SET(obj->extra_flags, ITEM_PERSONAL))
            && obj->pIndexData->vnum != OBJ_VNUM_MAGIC_ARROW &&
            obj->pIndexData->vnum !=
                OBJ_VNUM_MAGIC_AMMO) // 加入魔法彈可以被丟棄 2022/04/30
        {
            send_to_char("你丟不掉它.\n\r", ch);
            return;
        }
        act("你丟下 $p.", ch, obj, NULL, TO_CHAR);
        act("$n 丟下 $p.", ch, obj, NULL, TO_ROOM);
        obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        // 新增 obj_act drop 2022/02/12
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("drop", ch, obj);
        }
        // 敘述移到 obj 離開 ch 前 2022/02/12
        // act( "你丟下 $p.", ch, obj, NULL, TO_CHAR );
        // act( "$n 丟下 $p.", ch, obj, NULL, TO_ROOM );
        if (IS_SET(obj->extra_flags, ITEM_PERSONAL))
            extract_obj(obj);
    } else {
        /* 'drop all' or 'drop all.obj' */
        OBJ_DATA *obj_next;

        found = FALSE;
        for (obj = ch->carrying; obj; obj = obj_next) {
            obj_next = obj->next_content;

            if ((arg[3] == '\0' || is_name(&arg[4], obj->name)) &&
                can_see_obj(ch, obj) && obj->wear_loc == WEAR_NONE &&
                can_drop_obj(ch, obj)) {
                act("你丟下 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 丟下 $p.", ch, obj, NULL, TO_ROOM);
                found = TRUE;
                obj_from_char(obj);
                obj_to_room(obj, ch->in_room);
                // 新增 obj_act drop 2022/02/12
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("drop", ch, obj);
                }
                // 敘述移到 obj 離開 ch 前 2022/02/12
                // act( "你丟下 $p.", ch, obj, NULL, TO_CHAR );
                // act( "$n 丟下 $p.", ch, obj, NULL, TO_ROOM );
            }
        }

        if (!found) {
            if (arg[3] == '\0')
                send_to_char("你身上沒半點東西.", ch);
            else
                act("你身上沒有 $T.", ch, NULL, &arg[4], TO_CHAR);
        }
    }
    save_char_obj(ch);
    return;
}

// do_give: Command: do_give - Performs the 'give' command.
// do_give: 指令：do_give - 執行 'give' 指令。
void do_give(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0') {
        send_to_char("要給誰什麼？\n\r", ch);
        return;
    }

    if (is_number(arg1)) {
        /* 'give NNNN coins victim' */
        char buf[MAX_STRING_LENGTH];
        int amount;

        amount = atoi(arg1);
        if (amount <= 0 || str_prefix(arg2, "coins")) {
            send_to_char("Sorry, 你無法這麼做.\n\r", ch);
            return;
        }

        one_argument(argument, arg2);

        if (arg2[0] == '\0') {
            send_to_char("要給誰什麼？\n\r", ch);
            return;
        }
        if (ch->class <= BARBARIAN && ch->level < 3) {
            send_to_char("你年紀還小，別亂丟金錢。\n\r", ch);
            return;
        }

        if (!(victim = get_char_room(ch, arg2))) {
            send_to_char("這裡沒有這個人.\n\r", ch);
            return;
        }

        if (victim == ch) {
            send_to_char("你把口袋裡的錢拿出來把玩一下。\n\r", ch);
            return;
        }

        if (ch->gold < amount) {
            send_to_char("你沒有這麼多錢.\n\r", ch);
            return;
        }

        ch->gold -= amount;
        if (!IS_NPC(victim))
            victim->gold += amount;
        act("你給了$N一些錢.", ch, NULL, victim, TO_CHAR);

        sprintf(buf, "$n給了你 %d 枚金幣.", amount);
        act(buf, ch, NULL, victim, TO_VICT);

        act("$n給了$N一些錢.", ch, NULL, victim, TO_NOTVICT);
        mprog_bribe_trigger(victim, ch, amount);
        return;
    }

    if (!(obj = get_obj_carry(ch, arg1))) {
        send_to_char("你身上沒有這樣東西.\n\r", ch);
        return;
    }

    if (obj->wear_loc != WEAR_NONE) {
        send_to_char("你得先把它脫下來.\n\r", ch);
        return;
    }

    if (IS_SET(obj->extra_flags, ITEM_PERSONAL) && !IS_IMMORTAL(ch)) {
        send_to_char("這樣東西不能給人.\n\r", ch);
        return;
    }

    if (!(victim = get_char_room(ch, arg2))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    // if( victim == ch ) {
    // 	send_to_char("你不需要給你自己東西。\n\r", ch );
    // 	return;
    // }
    if (IS_SET(obj->extra_flags, ITEM_UNIQUE) && IS_NPC(victim)) {
        send_to_char("不能交給非玩家角色.\n\r", ch);
        return;
    }

    if (!can_drop_obj(ch, obj)) {
        send_to_char("你無法放開它.\n\r", ch);
        return;
    }

    if (IS_OBJ_STAT(obj, ITEM_HOLY) && victim->race == race_lookup("Vampire")) {
        act("$N 拒絕收下 $p.", ch, obj, victim, TO_CHAR);
        act("$n 給了 $N $p 但是被拒絕了.", ch, obj, victim, TO_ROOM);
        act("你拒絕收下 $n 給的 $p.", ch, obj, victim, TO_VICT);
        return;
    }

    if (victim->carry_number + get_obj_number(obj) > can_carry_n(victim)) {
        act("$N拿不動更多東西了.", ch, NULL, victim, TO_CHAR);
        act("$n試著要給你一些東西，可是你身上已經放滿東西了.", ch, NULL, victim,
            TO_VICT);
        return;
    }

    if (victim->carry_weight + get_obj_weight(obj) > can_carry_w(victim)) {
        act("$N拿不動更多重量了.", ch, NULL, victim, TO_CHAR);
        act("$n試著要給你一些東西，可是你已經拿不動了.", ch, NULL, victim,
            TO_VICT);
        return;
    }

    if (!can_see_obj(victim, obj)) {
        act("$N 看不見這樣東西.", ch, NULL, victim, TO_CHAR);
        return;
    }

    act("你給了$N$p.", ch, obj, victim, TO_CHAR);
    act("$n給了你$p.", ch, obj, victim, TO_VICT);
    act("$n給了$N$p.", ch, obj, victim, TO_NOTVICT);
    obj_from_char(obj);
    obj_to_char(obj, victim);
    // 敘述移到 obj 離開 ch 前 2022/02/12
    // act( "你給了$N$p.", ch, obj, victim, TO_CHAR );
    // act( "$n給了你$p.",   ch, obj, victim, TO_VICT );
    // act( "$n給了$N$p.", ch, obj, victim, TO_NOTVICT );

    // 針對 ch 作用的 obj_act give 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("give", ch, obj);
    }
    // obj 可能在 obj_act 時被 purge 2022/02/13
    if (!obj) {
        save_char_obj(ch);
        return;
    }
    // 針對 victim 作用的 obj_act give_target 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("give_target", victim, obj);
    }
    if (!obj) {
        save_char_obj(ch);
        return;
    }

    mprog_give_trigger(victim, ch, obj);

    // add at 2022/02/12
    if (!obj) {
        save_char_obj(ch);
        return;
    }

    if (IS_NPC(victim) && (victim->pIndexData->pShop) && obj && !obj->deleted) {
        // 避免 shop 觸發 give_prog drop 或 give 物品後消失 add at 2020/10/30
        if (!(obj = get_obj_carry(victim, arg1))) {
            save_char_obj(ch);
            return;
        }
        obj_from_char(obj);
        extract_obj(obj);
    }
    save_char_obj(ch);
    return;
}

// do_fill: Command: do_fill - Performs the 'fill' command.
// do_fill: 指令：do_fill - 執行 'fill' 指令。
void do_fill(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    bool found;

    if (!check_blind(ch))
        return;

    argument =
        one_argument(argument, arg); // get the first parameter from argument
    one_argument(argument, arg1);    // and remove it from argument, then get
                                  // the second parameter from argument.

    if (arg[0] == '\0') {
        send_to_char("Fill 什麼？\n\r", ch);
        return;
    }

    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你身上沒有這樣東西.\n\r", ch);
        return;
    }

    found = FALSE;
    if (arg1[0] == '\0') {
        for (fountain = ch->in_room->contents; fountain;
             fountain = fountain->next_content) {
            if (fountain->item_type == ITEM_FOUNTAIN) {
                found = TRUE;
                break;
            }
        }
    } else {
        fountain = get_obj_list(ch, arg1, ch->in_room->contents);
        if (!fountain) {
            act("這裡沒有 $T.", ch, NULL, arg1, TO_CHAR);
            return;
        }
        if (fountain->item_type == ITEM_FOUNTAIN) {
            found = TRUE;
        } else {
            send_to_char("這東西不是水源!\n\r", ch);
            return;
        }
    }

    if (!found) {
        send_to_char("這裡沒有水源！\n\r", ch);
        return;
    }

    if (obj->item_type != ITEM_DRINK_CON) {
        send_to_char("那樣東西不能裝水.\n\r", ch);
        return;
    }

    if (obj->value[0] < 1 || obj->value[1] == -1) {
        send_to_char("這樣東西裝不了水.\n\r", ch);
        return;
    }

    if (fountain->value[1] < 1) {
        send_to_char("這個水源乾涸了.\n\r", ch);
        return;
    }

    if (obj->value[1] >= obj->value[0]) {
        send_to_char("那已經裝滿了.\n\r", ch);
        return;
    }

    act("你把 $p 倒光。", ch, obj, NULL, TO_CHAR);
    act("你重新裝滿 $p.", ch, obj, NULL, TO_CHAR);
    obj->value[2] = fountain->value[2];
    obj->value[1] = UMIN(obj->value[0], fountain->value[1]);
    obj->value[3] = fountain->value[3];
    fountain->value[1] -= obj->value[1];
    // 加入 obj_act fill 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("fill", ch, obj);
    }
    // add at 2022/02/12
    if (!obj)
        return;
    // 加入 obj_act filled 判定泉水被filled 的當下 2022/02/12
    if (IS_SET(fountain->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("filled", ch, fountain);
    }

    return;
}

// do_drink: Command: do_drink - Performs the 'drink' command.
// do_drink: 指令：do_drink - 執行 'drink' 指令。
void do_drink(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int amount;
    int liquid;

    if (!check_blind(ch))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        for (obj = ch->in_room->contents; obj; obj = obj->next_content) {
            if (obj->item_type == ITEM_FOUNTAIN)
                break;
        }

        if (!obj) {
            send_to_char("喝什麼？\n\r", ch);
            return;
        }
    } else {
        if (!(obj = get_obj_here(ch, arg))) {
            send_to_char("這裡沒有這樣東西.\n\r", ch);
            return;
        }
    }

    switch (obj->item_type) {
    default:
        send_to_char("那不能喝.\n\r", ch);
        break;

    case ITEM_FOUNTAIN:

        if (obj->value[2] >= LIQ_MAX || obj->value[2] < 0) {
            bug("Do_drink: bad liquid number %d.", obj->pIndexData->vnum);
            obj->value[2] = 0;
        }
        liquid = obj->value[2];

        /*
                if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
                {
                        bug( "Do_drink: bad liquid number %d.", liquid );
                        liquid = obj->value[2] = 0;
                }
        */

        if (obj->value[1] <= 0 && obj->value[1] != -1) {
            act("$p乾涸了.", ch, obj, NULL, TO_CHAR);
            return;
        }

        // 修改酒醉的條件 2023/01/13
        // if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 &&
        // liq_table[liquid].liq_affect[COND_DRUNK] > 0)
        if (!IS_NPC(ch)) {
            // 酒精中毒的情況下無法進食 2023/01/13
            if (ch->pcdata->condition[COND_DRUNK] >= 70) {
                send_to_char("你感到胃在糾結，根本無法進食...\n\r", ch);
                return;
            }
            // 越醉越喝不到酒
            if (ch->pcdata->condition[COND_DRUNK] >= 20 &&
                liq_table[liquid].liq_affect[COND_DRUNK] > 0) {
                if (ch->pcdata->condition[COND_DRUNK] < number_percent()) {
                    send_to_char("咦？怎麼老是喝不到啊...  *呃*\n\r", ch);
                    return;
                }
            }
        }

        /*   remove at 19/12/22
                if ( ch->race == race_lookup( "vampire" ) && obj->value[2] !=
           13)
                {
                        act( "你不喝$T.", ch, NULL, liq_table[liquid].liq_name,
           TO_CHAR ); break;
                }

                移除喝 fountain 直接把 thirst 補到滿的設定 remove at 19/12/22
                if ( !IS_NPC( ch ) ) ch->pcdata->condition[COND_THIRST] = 48;

                act( "你從 $p 裡喝了幾口$T.", ch, obj,
           liq_table[liquid].liq_name, TO_CHAR ); act( "$n 從 $p 裡喝了幾口$T.",
           ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        */
        // 把所有 Condition 從 48 修改為最大值 100  2023/01/13
        if (!IS_NPC(ch) && liq_table[liquid].liq_affect[COND_DRUNK] == 0 &&
            ch->race != race_lookup("vampire")) {
            // 判定比較解渴的飲料
            if (liq_table[liquid].liq_affect[COND_THIRST] >
                    liq_table[liquid].liq_affect[COND_FULL] &&
                liq_table[liquid].liq_affect[COND_THIRST] > 0) {
                // 不渴且飲料可以飽足的情況
                if (ch->pcdata->condition[COND_THIRST] >= 40 &&
                    liq_table[liquid].liq_affect[COND_FULL] > 0)
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) /
                               liq_table[liquid].liq_affect[COND_FULL]);
                else
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) /
                               liq_table[liquid].liq_affect[COND_THIRST]);
            }
            // 判定比較飽足的飲料
            else if (liq_table[liquid].liq_affect[COND_THIRST] <
                         liq_table[liquid].liq_affect[COND_FULL] &&
                     liq_table[liquid].liq_affect[COND_FULL] > 0) {
                // 不餓且飲料可以解渴的情況
                if (ch->pcdata->condition[COND_FULL] >= 40 &&
                    liq_table[liquid].liq_affect[COND_THIRST] > 0)
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) /
                               liq_table[liquid].liq_affect[COND_THIRST]);
                else
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) /
                               liq_table[liquid].liq_affect[COND_FULL]);
            }
            // 解渴飽足相同的飲料
            else if (liq_table[liquid].liq_affect[COND_THIRST] > 0 &&
                     liq_table[liquid].liq_affect[COND_FULL] > 0) {
                if (ch->pcdata->condition[COND_THIRST] >
                    ch->pcdata->condition[COND_FULL])
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) /
                               liq_table[liquid].liq_affect[COND_FULL]);
                else
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) /
                               liq_table[liquid].liq_affect[COND_THIRST]);
            } else {
                // 具有口渴或饑餓效果的飲料
                amount = number_range(1, 2);
            }
        } else {
            // 酒精飲料或吸血鬼
            amount = number_range(1, 2);
        }

        if (obj->value[1] != -1)
            amount = UMIN(amount, obj->value[1]);

        if (amount > 1) {
            act("你從 $p 裡喝了幾口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_CHAR);
            act("$n 從 $p 裡喝了幾口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_ROOM);
        } else {
            act("你從 $p 裡喝了口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_CHAR);
            act("$n 從 $p 裡喝了口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_ROOM);
        }

        // 得到酒精的狀況
        gain_condition(ch, COND_DRUNK,
                       amount * liq_table[liquid].liq_affect[COND_DRUNK]);

        // 得到飽足及解渴的情況
        if (ch->race != race_lookup("vampire")) {
            gain_condition(ch, COND_FULL,
                           amount * liq_table[liquid].liq_affect[COND_FULL]);
            gain_condition(ch, COND_THIRST,
                           amount * liq_table[liquid].liq_affect[COND_THIRST]);
        } else if (liquid == 13) // blood
        {
            // 判定 vampire 喝血
            if (!IS_NPC(ch)) {
                amount =
                    UMAX(1, /*48*/ 100 - (ch->pcdata->condition[COND_THIRST]));
                if (obj->value[1] != -1)
                    amount = UMIN(amount, obj->value[1]);
            }
            amount = amount * 4;
            gain_condition(ch, COND_FULL, amount);
            gain_condition(ch, COND_THIRST, amount);
        }

        /* 敘述改由 gain_condition 顯示 2023/01/13

        if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK ] > 10 )
                send_to_char( "你感到有些醉了.\n\r", ch );

        if ( ch->race != race_lookup( "vampire" ) )
        {
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
                        send_to_char( "你飽了.\n\r", ch );
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
                        send_to_char( "你不再感到口渴.\n\r", ch );
}
        else if ( liquid == 13 )
        {
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
                        send_to_char( "你飽了.\n\r", ch );
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
                        send_to_char( "你不再感到口渴.\n\r", ch );
        } */

        /* 加入 mob no_poison 判定及 race_undead 中毒 2021/11/04
if (obj->value[3] != 0
    && (   ch->race != race_lookup( "Vampire" )
        && ch->race != race_lookup( "Undead" )
        && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ))
        */

        if (obj->value[3] != 0) {
            /* The shit was poisoned! */
            AFFECT_DATA af;

            if (IS_SET(race_table[ch->race].race_abilities, RACE_NO_POISON) ||
                IS_SET(ch->special, NO_POISON)) {
                // undead 中毒判定 2021/11/04
                // 作用 AFF_GHOUL 等同於變為 undead 2022/01/16
                if (IS_SET(race_table[ch->race].race_abilities, RACE_UNDEAD) ||
                    IS_AFFECTED(ch, AFF_GHOUL)) {
                    send_to_char("飲料中的毒素侵入了你的體內。\n\r", ch);
                    af.type = gsn_poison;
                    af.duration = UMAX(0, amount - get_curr_con(ch));
                    af.location = APPLY_SAVING_POISON;
                    af.modifier = amount;
                    af.bitvector = AFF_POISON;
                    affect_join(ch, &af);
                }
            } else {
                send_to_char("你感到不太對勁，突然劇烈地咳了起來！\n\r", ch);
                act("$n 表情不太對勁，突然劇烈地咳了起來！", ch, NULL, NULL,
                    TO_ROOM);
                af.type = gsn_poison;
                af.duration = UMAX(0, amount - get_curr_con(ch));
                af.location = APPLY_SAVING_POISON;
                af.modifier = amount;
                af.bitvector = AFF_POISON;
                affect_join(ch, &af);
            }
        }
        if (obj->value[1] != -1) {
            obj->value[1] -= amount;
            obj->value[1] = UMAX(0, obj->value[1]);
        }

        if (obj->value[1] == 0) {
            act("你把 $p 喝乾了.", ch, obj, NULL, TO_CHAR);
        }

        // add at 19/12/22
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("drink", ch, obj);
        }
        break;

        // 把所有 Condition 從 48 修改為最大值 100  2023/01/13
    case ITEM_DRINK_CON:

        if (obj->value[1] <= 0 && obj->value[1] != -1) {
            send_to_char("那已經被喝乾了.\n\r", ch);
            return;
        }

        if (obj->value[2] >= LIQ_MAX || obj->value[2] < 0) {
            bug("Do_drink: bad liquid number %d.", obj->pIndexData->vnum);
            obj->value[2] = 0;
        }
        liquid = obj->value[2];
        /*
                if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
                {
                        bug( "Do_drink: bad liquid number %d.", liquid );
                        liquid = obj->value[2] = 0;
                }
        */

        // 修改酒醉的條件 2023/01/13
        // if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 36 &&
        // liq_table[liquid].liq_affect[COND_DRUNK] > 0)
        if (!IS_NPC(ch)) {
            // 酒精中毒的情況下無法進食 2023/01/13
            if (ch->pcdata->condition[COND_DRUNK] >= 70) {
                send_to_char("你感到胃在糾結，根本無法進食...\n\r", ch);
                return;
            }
            // 越醉越喝不到酒
            if (ch->pcdata->condition[COND_DRUNK] >= 20 &&
                liq_table[liquid].liq_affect[COND_DRUNK] > 0) {
                if (ch->pcdata->condition[COND_DRUNK] < number_percent()) {
                    send_to_char("咦？怎麼老是喝不到啊...  *呃*\n\r", ch);
                    return;
                }
            }
        }

        if (!IS_NPC(ch) && liq_table[liquid].liq_affect[COND_DRUNK] == 0 &&
            ch->race != race_lookup("vampire")) {
            if (liq_table[liquid].liq_affect[COND_THIRST] >
                    liq_table[liquid].liq_affect[COND_FULL] &&
                liq_table[liquid].liq_affect[COND_THIRST] > 0) {
                if (ch->pcdata->condition[COND_THIRST] >= 40 &&
                    liq_table[liquid].liq_affect[COND_FULL] > 0)
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) /
                               liq_table[liquid].liq_affect[COND_FULL]);
                else
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) /
                               liq_table[liquid].liq_affect[COND_THIRST]);
            } else if (liq_table[liquid].liq_affect[COND_THIRST] <
                           liq_table[liquid].liq_affect[COND_FULL] &&
                       liq_table[liquid].liq_affect[COND_FULL] > 0) {
                if (ch->pcdata->condition[COND_FULL] >= 40 &&
                    liq_table[liquid].liq_affect[COND_THIRST] > 0)
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) /
                               liq_table[liquid].liq_affect[COND_THIRST]);
                else
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) /
                               liq_table[liquid].liq_affect[COND_FULL]);
            } else if (liq_table[liquid].liq_affect[COND_THIRST] > 0 &&
                       liq_table[liquid].liq_affect[COND_FULL] > 0) {
                if (ch->pcdata->condition[COND_THIRST] >
                    ch->pcdata->condition[COND_FULL])
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) /
                               liq_table[liquid].liq_affect[COND_FULL]);
                else
                    amount = UMAX(
                        1, (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) /
                               liq_table[liquid].liq_affect[COND_THIRST]);
            } else {
                amount = number_range(1, 2);
            }
        } else {
            amount = number_range(1, 2);
        }

        if (obj->value[1] != -1)
            amount = UMIN(amount, obj->value[1]);

        if (amount > 1) {
            act("你從 $p 裡喝了幾口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_CHAR);
            act("$n 從 $p 裡喝了幾口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_ROOM);
        } else {
            act("你從 $p 裡喝了口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_CHAR);
            act("$n 從 $p 裡喝了口$T.", ch, obj, liq_table[liquid].liq_name,
                TO_ROOM);
        }

        gain_condition(ch, COND_DRUNK,
                       amount * liq_table[liquid].liq_affect[COND_DRUNK]);
        if (ch->race != race_lookup("vampire")) {
            gain_condition(ch, COND_FULL,
                           amount * liq_table[liquid].liq_affect[COND_FULL]);
            gain_condition(ch, COND_THIRST,
                           amount * liq_table[liquid].liq_affect[COND_THIRST]);
        } else if (liquid == 13) {
            if (!IS_NPC(ch)) {
                amount =
                    UMAX(1, /*48*/ 100 - (ch->pcdata->condition[COND_THIRST]));
                if (obj->value[1] != -1)
                    amount = UMIN(amount, obj->value[1]);
            }
            gain_condition(ch, COND_FULL, amount);
            gain_condition(ch, COND_THIRST, amount);
        }

        /* 敘述改由 gain_condition 顯示 2023/01/13

        if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK ] > 10 )
                send_to_char( "你感到有些醉了.\n\r", ch );

        if ( ch->race != race_lookup( "vampire" ) )
        {
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
                        send_to_char( "你飽了.\n\r", ch );
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
                        send_to_char( "你不再感到口渴.\n\r", ch );
}
        else if ( liquid == 13 )
        {
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
                        send_to_char( "你飽了.\n\r", ch );
                if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
                        send_to_char( "你不再感到口渴.\n\r", ch );
        } */

        /* 加入 mob no_poison 判定及 race_undead 中毒 2021/11/04
if (obj->value[3] != 0
    && (   ch->race != race_lookup( "Vampire" )
        && ch->race != race_lookup( "Undead" )
        && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ))
        */
        if (obj->value[3] != 0) {
            /* The shit was poisoned! */
            AFFECT_DATA af;

            if (IS_SET(race_table[ch->race].race_abilities, RACE_NO_POISON) ||
                IS_SET(ch->special, NO_POISON)) {
                // undead 中毒判定 2021/11/04
                // 作用 AFF_GHOUL 等同於變為 undead 2022/01/16
                if (IS_SET(race_table[ch->race].race_abilities, RACE_UNDEAD) ||
                    IS_AFFECTED(ch, AFF_GHOUL)) {
                    send_to_char("飲料中的毒素侵入了你的體內。\n\r", ch);
                    af.type = gsn_poison;
                    af.duration = UMAX(0, amount - get_curr_con(ch));
                    af.location = APPLY_SAVING_POISON;
                    af.modifier = amount;
                    af.bitvector = AFF_POISON;
                    affect_join(ch, &af);
                }
            } else {
                send_to_char("你感到不太對勁，突然劇烈地咳了起來！\n\r", ch);
                act("$n 表情不太對勁，突然劇烈地咳了起來！", ch, NULL, NULL,
                    TO_ROOM);
                af.type = gsn_poison;
                af.duration = UMAX(0, amount - get_curr_con(ch));
                af.location = APPLY_SAVING_POISON;
                af.modifier = amount;
                af.bitvector = AFF_POISON;
                affect_join(ch, &af);
            }
        }
        // add at 19/12/22
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("drink", ch, obj);
        }

        if (obj->value[1] != -1) {
            obj->value[1] -= amount;
            obj->value[1] = UMAX(0, obj->value[1]);
        }
        if (obj->value[1] == 0) {
            // add at 19/12/22
            if (IS_SET(obj->pIndexData->souvenir, Z4) || obj->value[0] >= 50) {
                send_to_char("你把它喝光了.\n\r", ch);
            } else {
                send_to_char("你喝光之後，順手把它給丟了.\n\r", ch);
                extract_obj(obj);
            }
        }
        break;
    }

    return;
}

// do_eat: Command: do_eat - Performs the 'eat' command.
// do_eat: 指令：do_eat - 執行 'eat' 指令。
void do_eat(CHAR_DATA *ch, char *argument) {
    AFFECT_DATA af;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("要吃什麼？\n\r", ch);
        return;
    }

    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你身上沒有這樣東西.\n\r", ch);
        return;
    }

    if (!IS_IMMORTAL(ch)) {
        if (obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL &&
            !(IS_SET(race_table[ch->race].race_abilities, RACE_ORC) &&
              obj->item_type == ITEM_CORPSE_NPC)) {
            send_to_char("那看起來不能吃.\n\r", ch);
            return;
        }

        if (!IS_NPC(ch) && obj->item_type != ITEM_PILL &&
            obj->item_type != ITEM_CORPSE_NPC &&
            ch->pcdata->condition[COND_FULL] >= 100) {
            send_to_char("你再也吃不下了.\n\r", ch);
            return;
        }
    }

    switch (obj->item_type) {

    case ITEM_FOOD:
        if (!IS_NPC(ch)) {
            // 酒精中毒的情況下無法進食 2023/01/13
            if (ch->pcdata->condition[COND_DRUNK] >= 70) {
                send_to_char("你感到胃在糾結，根本無法進食...\n\r", ch);
                return;
            }
            act("你吃下 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 吃下 $p.", ch, obj, NULL, TO_ROOM);
            int condition;

            condition = ch->pcdata->condition[COND_FULL];
            if (ch->race != race_lookup("vampire"))
                gain_condition(ch, COND_FULL, obj->value[0]);

            /* 敘述改由 gain_condition 顯示 2023/01/13

            if ( ch->pcdata->condition[COND_FULL] > 40 )
                    send_to_char( "你吃飽了.\n\r", ch );
            else if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
                    send_to_char( "你不再感到飢餓.\n\r", ch ); */
        }

        if (IS_NPC(ch)) {
            act("你吃下 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 吃下 $p.", ch, obj, NULL, TO_ROOM);
        }

        /* 加入 mob no_poison 判定及 race_undead 中毒 2021/11/04
        if (   obj->value[3] != 0
                && ch->race != race_lookup( "Vampire" )
                && ch->race != race_lookup( "Undead" )
                && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON
        ) ){
        */
        if (obj->value[3] != 0) {
            /* The shit was poisoned! */

            if (IS_SET(race_table[ch->race].race_abilities, RACE_NO_POISON) ||
                IS_SET(ch->special, NO_POISON)) {
                // undead 中毒判定 2021/11/04
                // 作用 AFF_GHOUL 等同於變為 undead 2022/01/16
                if (IS_SET(race_table[ch->race].race_abilities, RACE_UNDEAD) ||
                    IS_AFFECTED(ch, AFF_GHOUL)) {
                    send_to_char("食物中的毒素侵入了你的體內。\n\r", ch);
                    af.type = gsn_poison;
                    af.duration = UMAX(0, obj->value[0] - get_curr_con(ch) * 2);
                    af.location = APPLY_SAVING_POISON;
                    af.modifier = obj->value[0];
                    af.bitvector = AFF_POISON;
                    affect_join(ch, &af);
                }
            } else {
                send_to_char("你感到不太對勁，突然劇烈地咳了起來！\n\r", ch);
                act("$n 表情不太對勁，突然劇烈地咳了起來！", ch, NULL, NULL,
                    TO_ROOM);
                af.type = gsn_poison;
                af.duration = UMAX(0, obj->value[0] - get_curr_con(ch) * 2);
                af.location = APPLY_SAVING_POISON;
                af.modifier = obj->value[0];
                af.bitvector = AFF_POISON;
                affect_join(ch, &af);
            }
        }
        if (obj->value[1] > 0 && obj->value[1] <= 49 &&
            ch->level >= obj->level) {
            affect_strip(ch, gsn_satiety);
            send_to_char("品嘗美味的食物讓你心境產生了變化。\n\r", ch);
            af.type = gsn_satiety;
            af.duration = UMAX(1, obj->value[0] / 2);
            af.location = obj->value[1];
            af.modifier = obj->value[2];
            af.bitvector = 0;
            affect_to_char(ch, &af);
        }
        break;
    case ITEM_PILL:
        if (obj->level > ch->level) {
            act("你不能吃等級比你高的藥丸.", ch, obj, NULL, TO_CHAR);
            return;
        } else {
            act("你吃下 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 吃下 $p.", ch, obj, NULL, TO_ROOM);
            obj_cast_spell(obj->value[1], obj->value[0], ch, ch, NULL);
            obj_cast_spell(obj->value[2], obj->value[0], ch, ch, NULL);
            obj_cast_spell(obj->value[3], obj->value[0], ch, ch, NULL);
            if (!IS_NPC(ch) ||
                (IS_NPC(ch) && is_affected(ch, gsn_charm_person))) {
                if (ch->fighting)
                    WAIT_STATE(ch, 1);
                else
                    WAIT_STATE(ch, 2);
            } else {
                WAIT_STATE(ch, 2);
            }
        }
        break;
    case ITEM_CORPSE_NPC:
        if (!IS_NPC(ch)) {
            // 酒精中毒的情況下無法進食 2023/01/13
            if (ch->pcdata->condition[COND_DRUNK] >= 70) {
                send_to_char("你感到胃在糾結，根本無法進食...\n\r", ch);
                return;
            }

            act("你吃下 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 吃下 $p.", ch, obj, NULL, TO_ROOM);

            int condition = ch->pcdata->condition[COND_FULL];
            int nSklv = get_skill_level(ch, gsn_inborn_ability);

            if (ch->race != race_lookup("vampire"))
                gain_condition(ch, COND_FULL, obj->value[0]);

            /* 敘述改由 gain_condition 顯示 2023/01/13

            if ( ch->pcdata->condition[COND_FULL] > 40 )
                    send_to_char( "你吃飽了.\n\r", ch );
            else if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
                    send_to_char( "你不再感到飢餓.\n\r", ch ); */

            if ((obj->level >= ch->level + 2 - nSklv ||
                 obj->level > 37 - nSklv / 3) &&
                obj->level < ch->level * 4) {
                int nBonusHR = (obj->level + nSklv - ch->level) / (11 - nSklv);
                int nBonusDR = (obj->level + nSklv - ch->level) / (9 - nSklv);
                if (nBonusHR < 1)
                    nBonusHR = 1;
                if (nBonusDR < 2)
                    nBonusDR = 2;

                nBonusHR += nSklv * 2 / 3;
                nBonusDR += nSklv;

                if (nSklv > 3) {
                    nBonusDR += (nSklv - 1) * 2;
                    nBonusHR += (nSklv - 1);
                }

                if (nBonusHR > 2 + nSklv * (nSklv + 4) / 6)
                    nBonusHR = 2 + nSklv * (nSklv + 4) / 6;
                if (nBonusDR > 5 + nSklv * (nSklv + 4) / 3)
                    nBonusDR = 5 + nSklv * (nSklv + 4) / 3;

                affect_strip(ch, gsn_excited);
                send_to_char("你感到全身都充滿了力量。\n\r", ch);
                af.type = gsn_excited;
                af.duration = 2 + nSklv * nSklv / 6;
                af.location = APPLY_HITROLL;
                af.modifier = nBonusHR;
                af.bitvector = 0;
                affect_to_char(ch, &af);

                af.location = APPLY_DAMROLL;
                af.modifier = nBonusDR;
                affect_to_char(ch, &af);
            }
        }
        break;
    default:
        if (IS_IMMORTAL(ch)) {
            act("你吃下 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 吃下 $p.", ch, obj, NULL, TO_ROOM);
        }
    }

    // if ( !IS_NPC( ch )
    // || ( IS_NPC( ch ) && is_affected( ch, gsn_charm_person ) ) )
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("eat", ch, obj);
    }
    if (obj) // obj may dest self in prog
        extract_obj(obj);
    return;
}

/*
 * Remove an object.
 */
// remove_obj: Unequips an object from a character's body wear slot.
// remove_obj: 將特定裝備自角色的裝備欄位上卸下。
bool remove_obj(CHAR_DATA *ch, int iWear, bool fReplace) {
    OBJ_DATA *obj;

    if (!(obj = get_eq_char(ch, iWear)))
        return TRUE;

    if (!fReplace)
        return FALSE;

    if (iWear == WEAR_RIDE && !IS_IMMORTAL(ch))
        return FALSE;

    if (IS_SET(obj->extra_flags, ITEM_NOREMOVE) && !IS_IMMORTAL(ch)) {
        act("你無法脫下 $p.", ch, obj, NULL, TO_CHAR);
        return FALSE;
    }

    unequip_char(ch, obj);

    // modified by coco 0513
    if (iWear == WEAR_RIDE) {
        act("$n 跳下 $p.", ch, obj, NULL, TO_ROOM);
        act("你跳下 $p.", ch, obj, NULL, TO_CHAR);
    } else {
        act("$n 脫下 $p.", ch, obj, NULL, TO_ROOM);
        act("你脫下 $p.", ch, obj, NULL, TO_CHAR);
    }
    // 新增作用在確實脫下裝備時的 remove 2020/10/24
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("bremove", ch, obj);
    }
    return TRUE;
}

// fremove_obj: Forcibly unequips an object from a character.
// fremove_obj: 強制將特定裝備自角色身上卸下。
bool fremove_obj(CHAR_DATA *ch, int iWear, bool fReplace) {
    OBJ_DATA *obj;

    if (!(obj = get_eq_char(ch, iWear)))
        return TRUE;

    if (!fReplace)
        return FALSE;

    if (iWear == WEAR_RIDE && !IS_IMMORTAL(ch))
        return FALSE;

    unequip_char(ch, obj);
    // modified by coco 0513
    if (iWear == WEAR_RIDE) {
        act("$n 跳下 $p.", ch, obj, NULL, TO_ROOM);
        act("你跳下 $p.", ch, obj, NULL, TO_CHAR);
    } else {
        act("$n 脫下 $p.", ch, obj, NULL, TO_ROOM);
        act("你脫下 $p.", ch, obj, NULL, TO_CHAR);
    }
    // 新增作用在確實脫下裝備時的 remove 2020/10/24
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("bremove", ch, obj);
    }
    return TRUE;
}

/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
typedef struct wear_map {
    char *name;
    int place;
} WEAR_MAP;

// wear_obj: Equips an object onto a character's body wear slot.
// wear_obj: 將特定物品裝備至角色的裝備欄位上。
void wear_obj(CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace, int iWear) {
    char buf[MAX_STRING_LENGTH];
    bool fForce = FALSE;
    int nSklv = get_skill_level(ch, gsn_inborn_ability);
    int min_str, str_diff;

    if (iWear != 0)
        fForce = TRUE;

    if (iWear == WEAR_RIDE && !IS_IMMORTAL(ch))
        return;
    /*
    static const   WEAR_MAP wear_table[17]= {
    {   "about",      ITEM_WEAR_ABOUT},
    {   "arm",      ITEM_WEAR_ARMS},
    {   "arrow",      ITEM_WEAR_ARROW},
    {   "body",     ITEM_WEAR_BODY },
    {   "feet",     ITEM_WEAR_FEET },
    {   "finger",     ITEM_WEAR_FINGER },
    {   "hand",     ITEM_WEAR_HANDS},
    {   "head",     ITEM_WEAR_HEAD },
    {   "hold",     ITEM_HOLD},
    {   "leg",      ITEM_WEAR_LEGS },
    {   "neck",     ITEM_WEAR_NECK },
    {   "shield",     ITEM_WEAR_SHIELD},
    {   "twohand",      ITEM_TWO_HAND},
    {   "waist",      ITEM_WEAR_WAIST},
    {   "weapon",     ITEM_WIELD},
    {   "wrist",      ITEM_WEAR_WRIST},
    {   "ride",         ITEM_WEAR_RIDE},
    };
    */
    if (obj->item_type != ITEM_LIGHT && !CAN_WEAR(obj, ITEM_WEAR_BODY) &&
        !CAN_WEAR(obj, ITEM_WEAR_SHIELD) && !CAN_WEAR(obj, ITEM_WIELD) &&
        !CAN_WEAR(obj, ITEM_TWO_HAND) && !CAN_WEAR(obj, ITEM_WEAR_HEAD)) {
        if (ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)) {
            act("$n 試著穿上 $p, 可惜看來並不合適.", ch, obj, NULL, TO_ROOM);
            return;
        }
    }

    if (ch->level < obj->level && !(IS_NPC(ch) && obj->level < 42)) {
        sprintf(buf, "你必須到等級 %d 才能使用%s.\n\r", obj->level,
                obj->short_descr);
        send_to_char(buf, ch);
        act("$n 試著穿上 $p, 可惜看來經驗不足.", ch, obj, NULL, TO_ROOM);
        return;
    }
    if (obj->pIndexData->max_level > 0 &&
        ch->level > obj->pIndexData->max_level) {
        sprintf(buf, "你必須在等級 %d 以內才能使用這樣物品.\n\r",
                obj->pIndexData->max_level);
        send_to_char(buf, ch);
        return;
    }

    if (!IS_NPC(ch) ||
        (ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum))) {
        if (IS_SET(obj->pIndexData->souvenir, Z1) && !IS_IMMORTAL(ch)) {
            send_to_char("此物品乃紀念品，只供收藏，不能穿戴！\n\r", ch);
            return;
        }
        if (IS_SET(obj->pIndexData->souvenir, Z2) && !IS_IMMORTAL(ch)) {
            send_to_char("此物品為Mob 專用！\n\r", ch);
            act("$n 試著穿上 $p, 可惜看來並不合適.", ch, obj, NULL, TO_ROOM);
            return;
        }
    }

    if (IS_SET(obj->extra_flags, ITEM_LORE)) {
        OBJ_DATA *pObj;
        int vnum = obj->pIndexData->vnum;
        for (pObj = ch->carrying; pObj; pObj = pObj->next_content) {
            if (pObj->pIndexData->vnum == vnum && pObj->wear_loc != WEAR_NONE) {
                send_to_char("你身上已經穿著一件了。\n\r", ch);
                return;
            }
        }
    }

    if (!fForce) {
        if (obj->item_type == ITEM_LIGHT) {
            if (!remove_obj(ch, WEAR_LIGHT, fReplace))
                return;
            act("你點亮了 $p 並且用手握住它.", ch, obj, NULL, TO_CHAR);
            act("$n 點亮了 $p 並且用手握住它.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_LIGHT);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_FINGER)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
                if (get_eq_char(ch, WEAR_FINGER_L) &&
                    get_eq_char(ch, WEAR_FINGER_R) &&
                    !remove_obj(ch, WEAR_FINGER_L, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_R, fReplace))
                    return;
            } else {
                if (get_eq_char(ch, WEAR_FINGER_L) &&
                    get_eq_char(ch, WEAR_FINGER_R) &&
                    get_eq_char(ch, WEAR_FINGER_3) &&
                    get_eq_char(ch, WEAR_FINGER_4) &&
                    !remove_obj(ch, WEAR_FINGER_L, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_R, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_3, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_4, fReplace))
                    return;
            }

            if (!get_eq_char(ch, WEAR_FINGER_L)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FINGER_L);
                return;
            }

            if (!get_eq_char(ch, WEAR_FINGER_R)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FINGER_R);
                return;
            }

            if (!get_eq_char(ch, WEAR_FINGER_3)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FINGER_3);
                return;
            }

            if (!get_eq_char(ch, WEAR_FINGER_4)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FINGER_4);
                return;
            }

            bug("Wear_obj: no free finger.", 0);
            send_to_char("你已經戴滿兩只戒指了.\n\r", ch);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_NECK)) {
            if (get_eq_char(ch, WEAR_NECK_1) && get_eq_char(ch, WEAR_NECK_2) &&
                !remove_obj(ch, WEAR_NECK_1, fReplace) &&
                !remove_obj(ch, WEAR_NECK_2, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_NECK_1)) {
                act("你 穿戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_NECK_1);
                return;
            }

            if (!get_eq_char(ch, WEAR_NECK_2)) {
                act("你 穿戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_NECK_2);
                return;
            }

            bug("Wear_obj: no free neck.", 0);
            send_to_char("你的頸子上穿戴不下更多東西了.\n\r", ch);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_BODY) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (IS_SET(race_table[ch->race].race_abilities, RACE_CENTAUR)) {
                if (strstr(obj->name, "robe") || strstr(obj->name, "cloth") ||
                    strstr(obj->name, "skirt")) {
                    send_to_char("這連身長袍穿在你身上並不合身.\n\r", ch);
                    act("$n 試著穿上 $p, 可是卻套不下去.", ch, obj, NULL,
                        TO_ROOM);
                    return;
                }
                if (get_skill_level(ch, gsn_inborn_ability) < 6) {
                    act("你試著穿上$p但是卻穿不上去, 可能是你天賦太差了.", ch,
                        obj, NULL, TO_CHAR);
                    act("$n 試著穿上 $p, 可是卻感覺渾身不靈活而脫下了.", ch,
                        obj, NULL, TO_ROOM);
                    return;
                }
            }
            if (!remove_obj(ch, WEAR_BODY, fReplace))
                return;
            act("你穿上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_BODY);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_HEAD)) {
            if (!remove_obj(ch, WEAR_HEAD, fReplace))
                return;
            act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_HEAD);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_LEGS) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_CENTAUR)) {
                if (!remove_obj(ch, WEAR_LEGS, fReplace))
                    return;
                act("你穿上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_LEGS);
                return;
            } else {
                if (get_eq_char(ch, WEAR_LEGS) &&
                    get_eq_char(ch, WEAR_LEGS_2) &&
                    !remove_obj(ch, WEAR_LEGS, fReplace) &&
                    !remove_obj(ch, WEAR_LEGS_2, fReplace))
                    return;

                if (!get_eq_char(ch, WEAR_LEGS)) {
                    act("你 穿上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_LEGS);
                    return;
                }

                if (!get_eq_char(ch, WEAR_LEGS_2)) {
                    act("你 穿上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_LEGS_2);
                    return;
                }

                send_to_char("你的腿已經穿滿腿甲了.\n\r", ch);
                return;
            }
        }

        if (CAN_WEAR(obj, ITEM_WEAR_FEET) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_CENTAUR)) {
                if (!remove_obj(ch, WEAR_FEET, fReplace))
                    return;
                act("你穿上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FEET);
                return;
            } else {
                if (get_eq_char(ch, WEAR_FEET) &&
                    get_eq_char(ch, WEAR_FEET_2) &&
                    !remove_obj(ch, WEAR_FEET, fReplace) &&
                    !remove_obj(ch, WEAR_FEET_2, fReplace))
                    return;

                if (!get_eq_char(ch, WEAR_FEET)) {
                    act("你 穿上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_FEET);
                    return;
                }

                if (!get_eq_char(ch, WEAR_FEET_2)) {
                    act("你 穿上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_FEET_2);
                    return;
                }

                send_to_char("你的腳已經穿滿靴子了.\n\r", ch);
                return;
            }
        }

        if (CAN_WEAR(obj, ITEM_WEAR_HANDS)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER) ||
                nSklv < 5) {
                if (!remove_obj(ch, WEAR_HANDS, fReplace))
                    return;
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_HANDS);
                return;
            } else { // Beholder with Inborn Ability higher or equal to Sklv 5,
                     // added by Keelar
                if (get_eq_char(ch, WEAR_HANDS) &&
                    get_eq_char(ch, WEAR_HANDS_2) &&
                    !remove_obj(ch, WEAR_HANDS, fReplace) &&
                    !remove_obj(ch, WEAR_HANDS_2, fReplace))
                    return;

                if (!get_eq_char(ch, WEAR_HANDS)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_HANDS);
                    return;
                }

                if (!get_eq_char(ch, WEAR_HANDS_2)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_HANDS_2);
                    return;
                }

                bug("Wear_obj: no free gloves for beholder with sklv higher "
                    "than 4.",
                    0);
                send_to_char("你的手已經戴滿手套了.\n\r", ch);
                return;
            }
        }

        if (CAN_WEAR(obj, ITEM_WEAR_ARMS)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER) ||
                nSklv < 6) {
                if (!remove_obj(ch, WEAR_ARMS, fReplace))
                    return;
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_ARMS);
                return;
            } else { // for beholder with inborn ability higher than or equal to
                     // 6, added by Keelar
                if (get_eq_char(ch, WEAR_ARMS) &&
                    get_eq_char(ch, WEAR_ARMS_2) &&
                    !remove_obj(ch, WEAR_ARMS, fReplace) &&
                    !remove_obj(ch, WEAR_ARMS_2, fReplace))
                    return;

                if (!get_eq_char(ch, WEAR_ARMS)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_ARMS);
                    return;
                }

                if (!get_eq_char(ch, WEAR_ARMS_2)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_ARMS_2);
                    return;
                }

                bug("Wear_obj: no free sleeves for beholder with sklv higher "
                    "than 5.",
                    0);
                send_to_char("你的手臂已經穿滿了.\n\r", ch);
                return;
            }
        }

        if (CAN_WEAR(obj, ITEM_WEAR_ABOUT)) {
            if (!remove_obj(ch, WEAR_ABOUT, fReplace))
                return;
            if (IS_SET(race_table[ch->race].race_abilities, RACE_CENTAUR)) {
                if (strstr(obj->name, "robe") || strstr(obj->name, "cloth") ||
                    strstr(obj->name, "skirt")) {
                    send_to_char("這連身長袍穿在你身上並不合身.\n\r", ch);
                    act("$n 試著穿上 $p, 可是卻套不下去.", ch, obj, NULL,
                        TO_ROOM);
                    return;
                }
                if (get_skill_level(ch, gsn_inborn_ability) < 5) {
                    act("你不知道怎麼批上$p才讓你感到行動自如, "
                        "可能是你天賦太差了.",
                        ch, obj, NULL, TO_CHAR);
                    act("$n 試著批上 $p, 可是卻感覺渾身不靈活而脫下了.", ch,
                        obj, NULL, TO_ROOM);
                    return;
                }
            }
            act("你披上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 披上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_ABOUT);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_WAIST) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_CENTAUR)) {
                if (!remove_obj(ch, WEAR_WAIST, fReplace))
                    return;
                act("你繫上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 繫上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_WAIST);
                return;
            } else {
                send_to_char("你找不到地方繫上腰帶.\n\r", ch);
                act("$n 試著繫上 $p, 可是卻找不到合適的地方.", ch, obj, NULL,
                    TO_ROOM);
                return;
            }
        }

        if (CAN_WEAR(obj, ITEM_WEAR_WRIST)) {
            if (get_eq_char(ch, WEAR_WRIST_L) &&
                get_eq_char(ch, WEAR_WRIST_R) &&
                !remove_obj(ch, WEAR_WRIST_L, fReplace) &&
                !remove_obj(ch, WEAR_WRIST_R, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_WRIST_L)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_WRIST_L);
                return;
            }

            if (!get_eq_char(ch, WEAR_WRIST_R)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_WRIST_R);
                return;
            }

            bug("Wear_obj: no free wrist.", 0);
            send_to_char("你雙手手腕都戴著東西.\n\r", ch);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_SHIELD)) {
            // 增加 Shield 減傷重量敘述 2018.11.27
            int ShieldWt = get_obj_weight(obj);
            int RequireStr =
                1 + get_curr_str(ch) + 2 * get_skill_level(ch, gsn_block);

            if (!remove_obj(ch, WEAR_WIELD_2, fReplace))
                return;
            if (!remove_obj(ch, WEAR_SHIELD, fReplace))
                return;
            if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                return;
            act("$n 拿起 $p 作為盾牌.", ch, obj, NULL, TO_ROOM);
            act("你拿起 $p 作為盾牌.", ch, obj, NULL, TO_CHAR);
            if (RequireStr < ShieldWt / 4) {
                act("$p 對你來說有點沉重, 使用上無法得心應手.", ch, obj, NULL,
                    TO_CHAR);
            }
            equip_char(ch, obj, WEAR_SHIELD);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WIELD) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (IS_NPC(ch) || get_skill_percent(ch, gsn_dual) != 0) {
                /*
                 * If you think this looks ugly now, just imagine how
                 * I felt trying to write it!  --- Thelonius (Monk)
                 * Now, it will attempt to wear in 1 if open, then 2 if
                 * open, else replace 2, else replace 1.
                 */
                if (!remove_obj(ch, WEAR_TWO_HAND, fReplace) ||
                    (get_eq_char(ch, WEAR_WIELD) &&
                     ((get_eq_char(ch, WEAR_WIELD_2) &&
                       !remove_obj(ch, WEAR_WIELD_2,
                                   fReplace)) // WEAR_HOLD & WEAR_SHIELD
                      || !remove_obj(ch, WEAR_HOLD,
                                     fReplace) // is at the same position of
                      ||
                      !remove_obj(ch, WEAR_SHIELD, fReplace)) // of WEAR_WIELD_2
                     && !remove_obj(ch, WEAR_WIELD, fReplace)))
                    return;

                if (!get_eq_char(ch, WEAR_WIELD)) {

                    int weight = get_obj_weight(obj);

                    for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].wield >= weight)
                            break;
                    }
                    str_diff = get_curr_str(ch) - min_str;

                    int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                    int nPbSklv = get_skill_level(ch, gsn_powerblow);
                    if (nPbPerc > 0)
                        str_diff += 1 + nPbSklv * nPbSklv / 4;

                    act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                    act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                    if (str_diff < 0) {
                        if (str_diff < -5)
                            act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                        else
                            act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                    }
                    equip_char(ch, obj, WEAR_WIELD);
                    return;
                }

                if (!get_eq_char(ch, WEAR_WIELD_2)) {

                    int weight = get_obj_weight(obj);

                    for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].wield2 >= weight)
                            break;
                    }
                    str_diff = get_curr_str(ch) - min_str;

                    int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                    int nPbSklv = get_skill_level(ch, gsn_powerblow);
                    if (nPbPerc > 0)
                        str_diff += 1 + nPbSklv * nPbSklv / 4;

                    act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                    act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                    if (str_diff < 0) {
                        if (str_diff < -5)
                            act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                        else
                            act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                    }
                    equip_char(ch, obj, WEAR_WIELD_2);
                    return;
                }
                bug("Wear_obj: no free weapon slot.", 0);
                send_to_char("你雙手都握著武器了.\n\r", ch);
                return;
            } else { /* can only wield one weapon */
                int weight = get_obj_weight(obj);
                if (!remove_obj(ch, WEAR_WIELD, fReplace))
                    return;
                if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                    return;

                for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                    // 代入的位置不正確 , 此處應為 .wield
                    // if(str_app[min_str].twohand >= weight)
                    if (str_app[min_str].wield >= weight)
                        break;
                }
                str_diff = get_curr_str(ch) - min_str;

                int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                int nPbSklv = get_skill_level(ch, gsn_powerblow);
                if (nPbPerc > 0)
                    str_diff += 1 + nPbSklv * nPbSklv / 4;

                act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                if (str_diff < 0) {
                    if (str_diff < -5)
                        act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                    else
                        act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                }
                equip_char(ch, obj, WEAR_WIELD);
                return;
            }
        }

        if (CAN_WEAR(obj, ITEM_HOLD)) {
            if (!remove_obj(ch, WEAR_WIELD_2, fReplace))
                return;
            if (!remove_obj(ch, WEAR_HOLD, fReplace))
                return;
            act("你握住 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 握住 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_HOLD);
            return;
        }

        if (CAN_WEAR(obj, ITEM_TWO_HAND) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            int weight = get_obj_weight(obj);
            if (!remove_obj(ch, WEAR_WIELD, fReplace))
                return;
            if (!remove_obj(ch, WEAR_WIELD_2, fReplace))
                return;
            if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                return;
            if (!remove_obj(ch, WEAR_SHIELD, fReplace))
                return;

            for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                if (str_app[min_str].twohand >= weight)
                    break;
            }
            str_diff = get_curr_str(ch) - min_str;

            int nPbPerc = get_skill_percent(ch, gsn_powerblow);
            int nPbSklv = get_skill_level(ch, gsn_powerblow);
            if (nPbPerc > 0)
                str_diff += 1 + nPbSklv * nPbSklv / 4;

            act("你用雙手握住 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 用雙手握住 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_TWO_HAND);
            if (str_diff < 0) {
                if (str_diff < -5)
                    act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                else
                    act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
            }
            return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_ARROW)) {
            if (obj->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW) {
                if (obj->value[3] >
                    30 + get_skill_level(ch, gsn_magic_arrow) * 20) {
                    // if(obj->value[3] > get_skill_level(ch, gsn_magic_arrow) *
                    // 20){
                    act("你無法控制這捆魔法箭的力量.", ch, obj, NULL, TO_CHAR);
                    return;
                }
            }
            if (!remove_obj(ch, WEAR_ARROW, fReplace))
                return;
            act("你把 $p 斜掛在背上.", ch, obj, NULL, TO_CHAR);
            act("$n 把 $p 斜掛在背上.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_ARROW);
            return;
        }
        // 彈藥裝備 add at 2022/04/30
        if (CAN_WEAR(obj, ITEM_WEAR_AMMO)) {
            if (!remove_obj(ch, WEAR_AMMO, fReplace))
                return;
            act("你把 $p 穿戴在容易填裝的位置.", ch, obj, NULL, TO_CHAR);
            act("$n 把 $p 穿戴在容易填裝的位置.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_AMMO);
            return;
        }
    } else {
        /*
                if ( obj->item_type == ITEM_LIGHT )
                {
                        if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
                                return;
                        act( "你點亮了 $p 並且用手握住它.",  ch, obj, NULL,
           TO_CHAR ); act( "$n 點亮了 $p 並且用手握住它.", ch, obj, NULL,
           TO_ROOM ); equip_char( ch, obj, WEAR_LIGHT ); return;
                }
        */

        if (iWear == ITEM_WEAR_FINGER && CAN_WEAR(obj, ITEM_WEAR_FINGER)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
                if (get_eq_char(ch, WEAR_FINGER_L) &&
                    get_eq_char(ch, WEAR_FINGER_R) &&
                    !remove_obj(ch, WEAR_FINGER_L, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_R, fReplace))
                    return;
            } else {
                if (get_eq_char(ch, WEAR_FINGER_L) &&
                    get_eq_char(ch, WEAR_FINGER_R) &&
                    get_eq_char(ch, WEAR_FINGER_3) &&
                    get_eq_char(ch, WEAR_FINGER_4) &&
                    !remove_obj(ch, WEAR_FINGER_L, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_R, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_3, fReplace) &&
                    !remove_obj(ch, WEAR_FINGER_4, fReplace))
                    return;
                if (!get_eq_char(ch, WEAR_FINGER_3)) {
                    act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_FINGER_3);
                    return;
                }

                if (!get_eq_char(ch, WEAR_FINGER_4)) {
                    act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_FINGER_4);
                    return;
                }
            }

            if (!get_eq_char(ch, WEAR_FINGER_L)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FINGER_L);
                return;
            }

            if (!get_eq_char(ch, WEAR_FINGER_R)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FINGER_R);
                return;
            }

            bug("Wear_obj: no free finger.", 0);
            send_to_char("你已經戴滿戒指了.\n\r", ch);
            return;
        }

        if (iWear == ITEM_WEAR_NECK && CAN_WEAR(obj, ITEM_WEAR_NECK)) {
            if (get_eq_char(ch, WEAR_NECK_1) && get_eq_char(ch, WEAR_NECK_2) &&
                !remove_obj(ch, WEAR_NECK_1, fReplace) &&
                !remove_obj(ch, WEAR_NECK_2, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_NECK_1)) {
                act("你 穿戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_NECK_1);
                return;
            }

            if (!get_eq_char(ch, WEAR_NECK_2)) {
                act("你 穿戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_NECK_2);
                return;
            }

            bug("Wear_obj: no free neck.", 0);
            send_to_char("你的頸子上穿戴不下更多東西了.\n\r", ch);
            return;
        }

        if (iWear == ITEM_WEAR_BODY && CAN_WEAR(obj, ITEM_WEAR_BODY) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!remove_obj(ch, WEAR_BODY, fReplace))
                return;
            act("你穿上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_BODY);
            return;
        }

        if (iWear == ITEM_WEAR_HEAD && CAN_WEAR(obj, ITEM_WEAR_HEAD)) {
            if (!remove_obj(ch, WEAR_HEAD, fReplace))
                return;
            act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_HEAD);
            return;
        }

        if (iWear == ITEM_WEAR_LEGS && CAN_WEAR(obj, ITEM_WEAR_LEGS) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!remove_obj(ch, WEAR_LEGS, fReplace))
                return;
            act("你穿上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_LEGS);
            return;
        }

        if (iWear == ITEM_WEAR_FEET && CAN_WEAR(obj, ITEM_WEAR_FEET) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!remove_obj(ch, WEAR_FEET, fReplace))
                return;
            act("你穿上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_FEET);
            return;
        }

        /*
                //add by coco for WEAR_RIDE
                if ( iWear == ITEM_WEAR_RIDE && CAN_WEAR( obj, ITEM_WEAR_RIDE )
           )
                {
                        if ( !remove_obj( ch, WEAR_RIDE, fReplace ) )
                                return;

                        if ( !IS_NPC(ch) && ch->class != KNIGHT && ch->class !=
           LORD ) return;

                        act( "你騎上 $p.", ch, obj, NULL, TO_CHAR );
                        act( "$n 騎上 $p.",   ch, obj, NULL, TO_ROOM );
                        equip_char( ch, obj, WEAR_RIDE );
                        return;
                }
        */

        if (iWear == ITEM_WEAR_HANDS && CAN_WEAR(obj, ITEM_WEAR_HANDS)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER) ||
                nSklv < 5) {
                if (!remove_obj(ch, WEAR_HANDS, fReplace))
                    return;
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_HANDS);
                return;
            } else { // Beholder with Inborn Ability higher or equal to Sklv 5,
                     // added by Keelar

                if (get_eq_char(ch, WEAR_HANDS) &&
                    get_eq_char(ch, WEAR_HANDS_2) &&
                    !remove_obj(ch, WEAR_HANDS, fReplace) &&
                    !remove_obj(ch, WEAR_HANDS_2, fReplace))
                    return;

                if (!get_eq_char(ch, WEAR_HANDS)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_HANDS);
                    return;
                }

                if (!get_eq_char(ch, WEAR_HANDS_2)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_HANDS_2);
                    return;
                }

                bug("Wear_obj: no free gloves for beholder with sklv higher "
                    "than 4.",
                    0);
                send_to_char("你的手已經戴滿手套了.\n\r", ch);
                return;
            }
        }

        if (iWear == ITEM_WEAR_ARMS && CAN_WEAR(obj, ITEM_WEAR_ARMS)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER) ||
                nSklv < 6) {
                if (!remove_obj(ch, WEAR_ARMS, fReplace))
                    return;
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_ARMS);
                return;
            } else { // for beholder with inborn ability higher than or equal to
                     // 6, added by Keelar
                if (get_eq_char(ch, WEAR_ARMS) &&
                    get_eq_char(ch, WEAR_ARMS_2) &&
                    !remove_obj(ch, WEAR_ARMS, fReplace) &&
                    !remove_obj(ch, WEAR_ARMS_2, fReplace))
                    return;

                if (!get_eq_char(ch, WEAR_ARMS)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_ARMS);
                    return;
                }

                if (!get_eq_char(ch, WEAR_ARMS_2)) {
                    act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                    act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                    equip_char(ch, obj, WEAR_ARMS_2);
                    return;
                }

                bug("Wear_obj: no free sleeves for beholder with sklv higher "
                    "than 5.",
                    0);
                send_to_char("你的手臂已經穿滿了.\n\r", ch);
                return;
            }
        }

        if (iWear == ITEM_WEAR_ABOUT && CAN_WEAR(obj, ITEM_WEAR_ABOUT)) {
            if (!remove_obj(ch, WEAR_ABOUT, fReplace))
                return;
            act("你披上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 披上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_ABOUT);
            return;
        }

        if (iWear == ITEM_WEAR_WAIST && CAN_WEAR(obj, ITEM_WEAR_WAIST) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (!remove_obj(ch, WEAR_WAIST, fReplace))
                return;
            act("你繫上 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 繫上 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_WAIST);
            return;
        }

        if (iWear == ITEM_WEAR_WRIST && CAN_WEAR(obj, ITEM_WEAR_WRIST)) {
            if (get_eq_char(ch, WEAR_WRIST_L) &&
                get_eq_char(ch, WEAR_WRIST_R) &&
                !remove_obj(ch, WEAR_WRIST_L, fReplace) &&
                !remove_obj(ch, WEAR_WRIST_R, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_WRIST_L)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_WRIST_L);
                return;
            }

            if (!get_eq_char(ch, WEAR_WRIST_R)) {
                act("你戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_WRIST_R);
                return;
            }

            bug("Wear_obj: no free wrist.", 0);
            send_to_char("你雙手手腕都戴著東西.\n\r", ch);
            return;
        }

        if (iWear == ITEM_WEAR_SHIELD && CAN_WEAR(obj, ITEM_WEAR_SHIELD)) {
            if (!remove_obj(ch, WEAR_WIELD_2, fReplace))
                return;
            if (!remove_obj(ch, WEAR_SHIELD, fReplace))
                return;
            if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                return;
            act("$n 拿起 $p 作為盾牌.", ch, obj, NULL, TO_ROOM);
            act("你拿起 $p 作為盾牌.", ch, obj, NULL, TO_CHAR);
            equip_char(ch, obj, WEAR_SHIELD);
            return;
        }
        // 彈藥裝備 add at 2022/04/30
        if (iWear == ITEM_WEAR_AMMO && CAN_WEAR(obj, ITEM_WEAR_AMMO)) {
            if (!remove_obj(ch, WEAR_AMMO, fReplace))
                return;
            act("你把 $p 穿戴在容易填裝的位置.", ch, obj, NULL, TO_CHAR);
            act("$n 把 $p 穿戴在容易填裝的位置.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_AMMO);
            return;
        }

        // iWear weapon
        if (iWear == ITEM_WIELD && CAN_WEAR(obj, ITEM_WIELD) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (IS_NPC(ch) || get_skill_percent(ch, gsn_dual) != 0) {
                /*
                 * If you think this looks ugly now, just imagine how
                 * I felt trying to write it!  --- Thelonius (Monk)
                 * Now, it will attempt to wear in 1 if open, then 2 if
                 * open, else replace 2, else replace 1.
                 */
                if (!remove_obj(ch, WEAR_TWO_HAND, fReplace) ||
                    (get_eq_char(ch, WEAR_WIELD) &&
                     ((get_eq_char(ch, WEAR_WIELD_2) &&
                       !remove_obj(ch, WEAR_WIELD_2,
                                   fReplace)) // WEAR_HOLD & WEAR_SHIELD
                      || !remove_obj(ch, WEAR_HOLD,
                                     fReplace) // is at the same position of
                      ||
                      !remove_obj(ch, WEAR_SHIELD, fReplace)) // of WEAR_WIELD_2
                     && !remove_obj(ch, WEAR_WIELD, fReplace)))
                    return;
                if (!get_eq_char(ch, WEAR_WIELD)) {
                    int weight = get_obj_weight(obj);

                    for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].wield >= weight)
                            break;
                    }
                    str_diff = get_curr_str(ch) - min_str;

                    int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                    int nPbSklv = get_skill_level(ch, gsn_powerblow);
                    if (nPbPerc > 0)
                        str_diff += 1 + nPbSklv * nPbSklv / 4;

                    act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                    act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                    if (str_diff < 0) {
                        if (str_diff < -5)
                            act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                        else
                            act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                    }
                    equip_char(ch, obj, WEAR_WIELD);
                    return;
                }

                if (!get_eq_char(ch, WEAR_WIELD_2)) {
                    int weight = get_obj_weight(obj);

                    for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].wield2 >= weight)
                            break;
                    }
                    str_diff = get_curr_str(ch) - min_str;

                    int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                    int nPbSklv = get_skill_level(ch, gsn_powerblow);
                    if (nPbPerc > 0)
                        str_diff += 1 + nPbSklv * nPbSklv / 4;

                    act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                    act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                    if (str_diff < 0) {
                        if (str_diff < -5)
                            act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                        else
                            act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                    }
                    equip_char(ch, obj, WEAR_WIELD_2);
                    return;
                }
                bug("Wear_obj: no free weapon slot.", 0);
                send_to_char("你雙手都握著武器了.\n\r", ch);
                return;
            } else /* can only wield one weapon */
            {
                int weight = get_obj_weight(obj);
                if (!remove_obj(ch, WEAR_WIELD, fReplace))
                    return;
                if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                    return;

                for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                    if (str_app[min_str].wield >= weight)
                        break;
                }
                str_diff = get_curr_str(ch) - min_str;

                int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                int nPbSklv = get_skill_level(ch, gsn_powerblow);
                if (nPbPerc > 0)
                    str_diff += 1 + nPbSklv * nPbSklv / 4;

                act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                if (str_diff < 0) {
                    if (str_diff < -5)
                        act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                    else
                        act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                }
                equip_char(ch, obj, WEAR_WIELD);
                return;
            }
        }

        if (iWear == ITEM_HOLD && CAN_WEAR(obj, ITEM_HOLD)) {
            if (!remove_obj(ch, WEAR_WIELD_2, fReplace))
                return;
            if (!remove_obj(ch, WEAR_HOLD, fReplace))
                return;
            act("你握住 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 握住 $p.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_HOLD);
            return;
        }

        // iWear twohand
        if (iWear == ITEM_TWO_HAND //&& (CAN_WEAR( obj, ITEM_TWO_HAND ) ||
                                   //ch->class == MARTIALMAN)
            && !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            int weight = get_obj_weight(obj);
            int min_str;
            if (!CAN_WEAR(obj, ITEM_WIELD) && !CAN_WEAR(obj, ITEM_TWO_HAND)) {
                send_to_char("這件物品不能作為武器。\n\r", ch);
                return;
            }
            if (!remove_obj(ch, WEAR_WIELD, fReplace))
                return;
            if (!remove_obj(ch, WEAR_WIELD_2, fReplace))
                return;
            if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                return;
            if (!remove_obj(ch, WEAR_SHIELD, fReplace))
                return;
            for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                if (str_app[min_str].twohand >= weight)
                    break;
            }
            str_diff = get_curr_str(ch) - min_str;

            int nPbPerc = get_skill_percent(ch, gsn_powerblow);
            int nPbSklv = get_skill_level(ch, gsn_powerblow);
            if (nPbPerc > 0)
                str_diff += 1 + nPbSklv * nPbSklv / 4;

            act("你用雙手握住 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 用雙手握住 $p.", ch, obj, NULL, TO_ROOM);
            if (str_diff < 0) {
                if (str_diff < -5)
                    act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                else
                    act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
            }
            equip_char(ch, obj, WEAR_TWO_HAND);
            return;
        }

        if (iWear == ITEM_WEAR_ARROW && CAN_WEAR(obj, ITEM_WEAR_ARROW)) {
            if (!remove_obj(ch, WEAR_ARROW, fReplace))
                return;
            act("你把 $p 斜掛在背上.", ch, obj, NULL, TO_CHAR);
            act("$n 把 $p 斜掛在背上.", ch, obj, NULL, TO_ROOM);
            equip_char(ch, obj, WEAR_ARROW);
            return;
        }

        // 新增關鍵字 wp1 用來裝備主要武器 2022/04/05
        if (iWear == WEAR_WIELD && CAN_WEAR(obj, ITEM_WIELD) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            /*  此段判定錯誤 2022/12/06
            if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
                    || ( ( ( get_eq_char( ch, WEAR_WIELD ) && !remove_obj( ch,
            WEAR_WIELD, fReplace ) )
                            || !remove_obj( ch, WEAR_HOLD, fReplace )
                            || !remove_obj( ch, WEAR_SHIELD,  fReplace ) ) ) )
                    return; */

            if (!remove_obj(ch, WEAR_WIELD, fReplace))
                return;
            if (!remove_obj(ch, WEAR_TWO_HAND, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_WIELD)) {
                int weight = get_obj_weight(obj);

                for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                    if (str_app[min_str].wield >= weight)
                        break;
                }
                str_diff = get_curr_str(ch) - min_str;

                int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                int nPbSklv = get_skill_level(ch, gsn_powerblow);
                if (nPbPerc > 0)
                    str_diff += 1 + nPbSklv * nPbSklv / 4;

                act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                if (str_diff < 0) {
                    if (str_diff < -5)
                        act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                    else
                        act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                }
                equip_char(ch, obj, WEAR_WIELD);
                return;
            }
            bug("Wear_obj: when wear wp , no free weapon slot.", 0);
            send_to_char("你雙手都握著武器了.\n\r", ch);
            return;
        }

        // 新增 wp2 裝備次要武器 2022/04/05
        if (iWear == WEAR_WIELD_2 && CAN_WEAR(obj, ITEM_WIELD) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            if (IS_NPC(ch) || get_skill_percent(ch, gsn_dual) != 0) {
                if (!remove_obj(ch, WEAR_TWO_HAND, fReplace) ||
                    (((get_eq_char(ch, WEAR_WIELD_2) &&
                       !remove_obj(ch, WEAR_WIELD_2, fReplace)) ||
                      !remove_obj(ch, WEAR_HOLD, fReplace) ||
                      !remove_obj(ch, WEAR_SHIELD, fReplace))))
                    return;

                if (!get_eq_char(ch, WEAR_WIELD_2)) {
                    int weight = get_obj_weight(obj);

                    for (min_str = 0; min_str < MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].wield2 >= weight)
                            break;
                    }
                    str_diff = get_curr_str(ch) - min_str;

                    int nPbPerc = get_skill_percent(ch, gsn_powerblow);
                    int nPbSklv = get_skill_level(ch, gsn_powerblow);
                    if (nPbPerc > 0)
                        str_diff += 1 + nPbSklv * nPbSklv / 4;

                    act("你裝備 $p 作武器.", ch, obj, NULL, TO_CHAR);
                    act("$n 裝備 $p 作武器.", ch, obj, NULL, TO_ROOM);
                    if (str_diff < 0) {
                        if (str_diff < -5)
                            act("太重了！你拿不動！", ch, obj, NULL, TO_CHAR);
                        else
                            act("$p 對你來說有點沉重.", ch, obj, NULL, TO_CHAR);
                    }
                    equip_char(ch, obj, WEAR_WIELD_2);
                    return;
                }
                bug("Wear_obj: when wear wp2 , no free weapon slot.", 0);
                send_to_char("你雙手都握著武器了.\n\r", ch);
                return;
            } else {
                send_to_char("你無法裝備次要武器.\n\r", ch);
                return;
            }
        }

        // 新增 hand1 裝備次手套 2022/04/05
        if (iWear == WEAR_HANDS && CAN_WEAR(obj, ITEM_WEAR_HANDS)) {

            if (get_eq_char(ch, WEAR_HANDS) &&
                !remove_obj(ch, WEAR_HANDS, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_HANDS)) {
                act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_HANDS);
                return;
            }

            bug("Wear_obj: hand2 change no free gloves for beholder with sklv "
                "higher than 4.",
                0);
            send_to_char("你的手已經戴滿手套了.\n\r", ch);
            return;
        }

        // 新增 hand2 裝備次手套 2022/04/05
        if (iWear == WEAR_HANDS_2 && CAN_WEAR(obj, ITEM_WEAR_HANDS)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER) ||
                nSklv < 5)
                return;

            if (get_eq_char(ch, WEAR_HANDS_2) &&
                !remove_obj(ch, WEAR_HANDS_2, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_HANDS_2)) {
                act("你 戴上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 戴上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_HANDS_2);
                return;
            }

            bug("Wear_obj: hand2 change no free gloves for beholder with sklv "
                "higher than 4.",
                0);
            send_to_char("你的手已經戴滿手套了.\n\r", ch);
            return;
        }

        // 新增 feet1 裝備次手套 2022/04/05
        if (iWear == WEAR_FEET && CAN_WEAR(obj, ITEM_WEAR_FEET)) {

            if (get_eq_char(ch, WEAR_FEET) &&
                !remove_obj(ch, WEAR_FEET, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_FEET)) {
                act("你 穿上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FEET);
                return;
            }

            bug("Wear_obj: feet2 change no free feet for centaur.", 0);
            send_to_char("你的腳已經穿滿鞋子了.\n\r", ch);
            return;
        }

        // 新增 feet2 裝備次腳部 2022/04/05
        if (iWear == WEAR_FEET_2 && CAN_WEAR(obj, ITEM_WEAR_FEET)) {
            if (!IS_SET(race_table[ch->race].race_abilities, RACE_CENTAUR))
                return;

            if (get_eq_char(ch, WEAR_FEET_2) &&
                !remove_obj(ch, WEAR_FEET_2, fReplace))
                return;

            if (!get_eq_char(ch, WEAR_FEET_2)) {
                act("你 穿上 $p.", ch, obj, NULL, TO_CHAR);
                act("$n 穿上 $p.", ch, obj, NULL, TO_ROOM);
                equip_char(ch, obj, WEAR_FEET_2);
                return;
            }

            bug("Wear_obj: feet2 change no free feet for centaur.", 0);
            send_to_char("你的腳已經穿滿鞋子了.\n\r", ch);
            return;
        }
    }

    if (fReplace) // some objs just cannot be wear/hold/wield...
        send_to_char("你無法裝備這樣東西.\n\r", ch);

    return;
}

/* Added by Keric for personal eq AC recover */
// do_personal_recover: Command: do_personal_recover - Performs the 'personal_recover' command.
// do_personal_recover: 指令：do_personal_recover - 執行 'personal_recover' 指令。
void do_personal_recover(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    float adj = 1.0;
    int price;
    bool fBank = FALSE;
    char arg[MAX_STRING_LENGTH];

    if (!ch->in_room)
        return;

    for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
        if (IS_NPC(mob) && IS_SET(mob->act, ACT_SMITH))
            break;
    }

    if (!mob) {
        send_to_char("什麼？你想幹什麼？\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("你要修什麼？\n\r", ch);
        return;
    }

    if (!str_cmp("all", arg)) {
        for (obj = ch->carrying; obj; obj = obj->next_content) {
            if (obj->wear_loc != WEAR_NONE)
                continue;
            if (!(obj->item_type == ITEM_ARMOR))
                continue;
            if (obj->item_type == ITEM_ARMOR && obj->value[1] < 1)
                continue;
            /*
            if( !(obj->extra_flags & ITEM_PERSONAL) ) continue;

            if( obj->item_type == ITEM_ARMOR && obj->value[1] < 0 )
            {
                    send_to_char("你在尋我開心嗎？\n\r", ch);
                    continue;
            }
            */

            if (obj->level < 5)
                price = 5;
            else if (obj->level < 15)
                price = 10;
            else if (obj->level < 30)
                price = 50;
            else if (obj->level <= LEVEL_HERO)
                price = 100;
            else
                price = 150;
            price = URANGE(1, price, ((obj->cost * 2)));

            price *= obj->value[1] * 100;
            if (IS_SET(obj->extra_flags, ITEM_BLESS))
                adj += 1.5;
            if (IS_SET(obj->extra_flags, ITEM_HUM))
                adj += 1.5;
            if (!(obj->extra_flags & ITEM_PERSONAL))
                price *= 30;

            price = (int)(price * adj);
            price = UMAX(100, price);

            sprintf(arg, "修理 %s 需要 %d 枚金幣。\n\r", obj->short_descr,
                    price);

            send_to_char(arg, ch);
            fBank = FALSE;

            if (ch->gold < price) {
                fBank = TRUE;
                if (ch->bank < price) {
                    act("$N搖了搖頭，說：「您的錢不夠。」", ch, NULL, mob,
                        TO_CHAR);
                    continue;
                }
            }

            if (fBank) {
                sprintf(arg, "你從銀行付出 %d 枚金幣。 \n\r", price);
                ch->bank -= price;
            } else {
                sprintf(arg, "你付出 %d 枚金幣。 \n\r", price);
                ch->gold -= price;
            }

            send_to_char(arg, ch);

            act("你把 $p 交給$N。", ch, obj, mob, TO_CHAR);
            act("$N聚精會神，$p泛起了一道光芒。", ch, obj, mob, TO_CHAR);
            act("$N抹抹額頭上的汗珠，把$p交還給你。", ch, obj, mob, TO_CHAR);

            if ((obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0)) {
                obj->value[0] += obj->value[1];
                obj->value[1] = 0;
            }
        }
        return;
    }

    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你身上沒有這樣東西。\n\r", ch);
        return;
    }

    // if( !(obj->extra_flags & ITEM_PERSONAL) )
    if (!(obj->item_type == ITEM_ARMOR)) {
        act("$N搖了搖頭，說：「這樣東西我不能修。」", ch, NULL, mob, TO_CHAR);
        return;
    }

    if (obj->item_type == ITEM_ARMOR && obj->value[1] < 1) {
        act("$N搖了搖頭，說：「這樣東西根本沒壞！」", ch, NULL, mob, TO_CHAR);
        return;
    }

    if (obj->level < 5)
        price = 5;
    else if (obj->level < 15)
        price = 10;
    else if (obj->level < 30)
        price = 50;
    else if (obj->level <= LEVEL_HERO)
        price = 100;
    else
        price = 150;
    price = UMIN(price, ((obj->cost * 2)));

    price *= obj->value[1] * 100;
    if (IS_SET(obj->extra_flags, ITEM_BLESS))
        adj += 1.5;
    if (IS_SET(obj->extra_flags, ITEM_HUM))
        adj += 1.5;
    if (!(obj->extra_flags & ITEM_PERSONAL))
        price *= 30;

    price = (int)(price * adj);
    price = UMAX(100, price);

    sprintf(arg, "修理 %s 需要 %d 枚金幣。\n\r", obj->short_descr, price);
    send_to_char(arg, ch);
    fBank = FALSE;
    if (ch->gold < price) {
        fBank = TRUE;
        if (ch->bank < price) {
            act("$N搖了搖頭，說：「您的錢不夠。」", ch, NULL, mob, TO_CHAR);
            return;
        }
    }

    if (fBank) {
        sprintf(arg, "你從銀行付出 %d 枚金幣。 \n\r", price);
        ch->bank -= price;
    } else {
        sprintf(arg, "你付出 %d 枚金幣。 \n\r", price);
        ch->gold -= price;
    }

    send_to_char(arg, ch);

    act("你把 $p 交給$N。", ch, obj, mob, TO_CHAR);
    act("$N聚精會神，$p泛起了一陣光芒。", ch, obj, mob, TO_CHAR);
    act("$N抹抹額頭上的汗珠，把$p交還給你。", ch, obj, mob, TO_CHAR);

    if ((obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0)) {
        obj->value[0] += obj->value[1];
        obj->value[1] = 0;
    }
    return;
}

// do_repair: Command: do_repair - Performs the 'repair' command.
// do_repair: 指令：do_repair - 執行 'repair' 指令。
void do_repair(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *mob;
    char arg[MAX_STRING_LENGTH];
    int curr_d, max_d, diff, price;
    float adj = 1.0;
    bool fBank = FALSE;
    if (!ch->in_room)
        return;

    for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
        if (IS_NPC(mob) && IS_SET(mob->act, ACT_SMITH))
            break;
    }

    if (!mob) {
        send_to_char("你找不到人幫你修復裝備。\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("你要修什麼？\n\r", ch);
        return;
    }

    if (!str_cmp("all", arg)) {
        for (obj = ch->carrying; obj; obj = obj->next_content) {
            if (obj->wear_loc != WEAR_NONE)
                continue;

            if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
                continue;

            curr_d = get_curr_dura(obj);
            max_d = get_max_dura(obj);
            diff = max_d - curr_d;

            if (diff < 1 || indestructible(obj)) {
                continue;
            }

            if (obj->level < 5)
                price = 1;
            else if (obj->level < 15)
                price = 2;
            else if (obj->level < 30)
                price = 5;
            else if (obj->level <= LEVEL_HERO)
                price = 10;
            else
                price = 30;

            price = URANGE(1, price, ((obj->cost * 2)));

            price *= diff;
            if (IS_SET(obj->extra_flags, ITEM_BLESS))
                adj += 0.5;
            if (IS_SET(obj->extra_flags, ITEM_HUM))
                adj += 0.2;

            price = (int)(price * adj);
            price = UMAX(100, price);
            // if( obj->item_type == ITEM_ARMOR && obj->value[1] > 0 )
            // price += obj-> level * 1000 * obj->value[1];

            sprintf(arg, "修理 %s 需要 %d 枚金幣。\n\r", obj->short_descr,
                    price);
            send_to_char(arg, ch);
            fBank = FALSE;
            if (ch->gold < price) {
                fBank = TRUE;
                if (ch->bank < price) {
                    act("$N搖了搖頭，說：「您的錢不夠。」", ch, NULL, mob,
                        TO_CHAR);
                    continue;
                }
            }

            if (fBank) {
                sprintf(arg, "你從銀行付出 %d 枚金幣。 \n\r", price);
                ch->bank -= price;
            } else {
                sprintf(arg, "你付出 %d 枚金幣。 \n\r", price);
                ch->gold -= price;
            }
            send_to_char(arg, ch);

            act("你把 $p 交給$N。", ch, obj, mob, TO_CHAR);
            act("$N一陣敲敲打打，$p慢慢回復了正常的狀態。", ch, obj, mob,
                TO_CHAR);
            act("$N抹抹額頭上的汗珠，把$p交還給你。", ch, obj, mob, TO_CHAR);

            repair_dura(obj, diff);
            /*if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) )
                    {
                    price += obj-> level * 10 * obj->value[1];
                    obj->value[0] += obj->value[1];
                    obj->value[1] = 0;
                    }
            */
        }
        return;
    }

    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你身上沒有這樣東西。\n\r", ch);
        return;
    }

    // if( !can_drop_obj(ch, obj)){
    //   send_to_char("你沒辦法把它給人。\n\r", ch);
    //   return;
    //   }

    curr_d = get_curr_dura(obj);
    max_d = get_max_dura(obj);
    diff = max_d - curr_d;

    if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR) {
        act("$N搖了搖頭，說：「這樣東西我沒辦法修。」", ch, NULL, mob, TO_CHAR);
        return;
    }
    if (diff < 1 || indestructible(obj)) {
        act("$N搖了搖頭，說：「這樣東西根本沒壞！」", ch, NULL, mob, TO_CHAR);
        return;
    }

    if (obj->level < 5)
        price = 1;
    else if (obj->level < 15)
        price = 2;
    else if (obj->level < 30)
        price = 5;
    else if (obj->level <= LEVEL_HERO)
        price = 10;
    else
        price = 30;

    price *= diff;
    price = UMIN(price, ((obj->cost * 2)));
    if (IS_SET(obj->extra_flags, ITEM_BLESS))
        adj += 0.5;
    if (IS_SET(obj->extra_flags, ITEM_HUM))
        adj += 0.2;

    price = (int)(price * adj);
    price = UMAX(100, price);
    // if( obj->item_type == ITEM_ARMOR && obj->value[1] > 0 )
    // price += obj-> level * 1000 * obj->value[1];

    sprintf(arg, "修理 %s 需要 %d 枚金幣。\n\r", obj->short_descr, price);
    send_to_char(arg, ch);
    fBank = FALSE;
    if (ch->gold < price) {
        fBank = TRUE;
        if (ch->bank < price) {
            act("$N搖了搖頭，說：「您的錢不夠。」", ch, NULL, mob, TO_CHAR);
            return;
        }
    }

    if (fBank) {
        sprintf(arg, "你從銀行付出 %d 枚金幣。 \n\r", price);
        ch->bank -= price;
    } else {
        sprintf(arg, "你付出 %d 枚金幣。 \n\r", price);
        ch->gold -= price;
    }
    send_to_char(arg, ch);

    act("你把 $p 交給$N。", ch, obj, mob, TO_CHAR);
    act("$N一陣敲敲打打，$p慢慢回復了正常的狀態。", ch, obj, mob, TO_CHAR);
    act("$N抹抹額頭上的汗珠，把$p交還給你。", ch, obj, mob, TO_CHAR);

    repair_dura(obj, diff);
    /*if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) ){
    obj->value[0] += obj->value[1];
    obj->value[1] = 0;
    }*/

    return;
}

// do_wear: Performs the 'wear' command to equip armor, clothing, or accessories.
// do_wear: 執行 'wear' 指令裝備防具、服飾或飾品。
void do_wear(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char place[MAX_INPUT_LENGTH];
    int iWear = 0;
    // static const   WEAR_MAP wear_table[17]= {
    static const WEAR_MAP wear_table[24] = {
        {"about", ITEM_WEAR_ABOUT},
        {"arm", ITEM_WEAR_ARMS},
        {"arrow", ITEM_WEAR_ARROW},
        {"body", ITEM_WEAR_BODY},
        {"feet", ITEM_WEAR_FEET},
        {"finger", ITEM_WEAR_FINGER},
        {"hand", ITEM_WEAR_HANDS},
        {"head", ITEM_WEAR_HEAD},
        {"hold", ITEM_HOLD},
        {"leg", ITEM_WEAR_LEGS},
        {"neck", ITEM_WEAR_NECK},
        {"shield", ITEM_WEAR_SHIELD},
        {"twohand", ITEM_TWO_HAND},
        {"waist", ITEM_WEAR_WAIST},
        {"weapon", ITEM_WIELD},
        {"wrist", ITEM_WEAR_WRIST},
        //{   "ride",       ITEM_WEAR_RIDE}, // ride 移到最後 2022/04/05
        {"wp1", WEAR_WIELD},     // add at 2022/04/05
        {"wp2", WEAR_WIELD_2},   // add at 2022/04/05
        {"hand1", WEAR_HANDS},   // add at 2022/04/05
        {"hand2", WEAR_HANDS_2}, // add at 2022/04/05
        {"feet1", WEAR_FEET},    // add at 2022/04/05
        {"feet2", WEAR_FEET_2},  // add at 2022/04/05
        {"ammo", WEAR_AMMO},     // add at 2022/04/30  alc 彈藥
        {"ride", ITEM_WEAR_RIDE},
    };

    argument = one_argument(argument, arg);
    one_argument(argument, place);
    if (place[0] != '\0') {
        int i;
        for (i = 0; i < 23; ++i) {
            if (!str_prefix(place, wear_table[i].name)) {
                iWear = wear_table[i].place;
                break;
            }
        }
    }

    if (arg[0] == '\0') {
        send_to_char("要裝備什麼？\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "all")) {
        OBJ_DATA *obj_next;

        for (obj = ch->carrying; obj; obj = obj_next) {
            obj_next = obj->next_content;

            if (obj->wear_loc != WEAR_NONE || !can_see_obj(ch, obj))
                continue;

            if (CAN_WEAR(obj, ITEM_WIELD) // some race cannot wield weapon
                && !IS_SET(race_table[ch->race].race_abilities,
                           RACE_WEAPON_WIELD)) // this check seem could put in
                continue;                      // wear_obj()?

            wear_obj(ch, obj, FALSE, 0);
            if (!IS_NPC(ch) && ch->position == POS_DEAD) {
                raw_kill(ch, ch);
                break;
            }
        }
        return;
    } else {
        if (!(obj = get_obj_carry(ch, arg))) {
            send_to_char("你身上沒有這樣東西.\n\r", ch);
            return;
        }

        if (CAN_WEAR(obj, ITEM_WIELD) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_WEAPON_WIELD)) {
            send_to_char("你無法裝備武器.\n\r", ch);
            return;
        }

        wear_obj(ch, obj, TRUE, iWear);
        if (!IS_NPC(ch) && ch->position == POS_DEAD)
            raw_kill(ch, ch);
    }

    return;
}

// do_fremove: Command: do_fremove - Performs the 'fremove' command.
// do_fremove: 指令：do_fremove - 執行 'fremove' 指令。
void do_fremove(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int wear_loc;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("要脫下什麼？\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "all")) {
        for (wear_loc = 0; wear_loc <= 20; wear_loc++)
            fremove_obj(ch, wear_loc, TRUE);
        fremove_obj(ch, WEAR_FINGER_3, TRUE);
        fremove_obj(ch, WEAR_FINGER_4, TRUE);
        fremove_obj(ch, WEAR_ARMS_2, TRUE);
        fremove_obj(ch, WEAR_HANDS_2, TRUE);
        return;
    }

    if (!(obj = get_obj_wear(ch, arg))) {
        send_to_char("你身上沒有這樣東西.\n\r", ch);
        return;
    }

    fremove_obj(ch, obj->wear_loc, TRUE);
    return;
}

// do_remove: Performs the 'remove' command to unequip items from wear slots.
// do_remove: 執行 'remove' 指令卸下身上已裝備的物品。
void do_remove(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int wear_loc;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("要脫下什麼？\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "all")) {
        for (wear_loc = 0; wear_loc <= 20; wear_loc++)
            remove_obj(ch, wear_loc, TRUE);
        remove_obj(ch, WEAR_FINGER_3, TRUE);
        remove_obj(ch, WEAR_FINGER_4, TRUE);
        remove_obj(ch, WEAR_HANDS_2, TRUE);
        remove_obj(ch, WEAR_ARMS_2, TRUE);
        remove_obj(ch, WEAR_FEET_2, TRUE);
        remove_obj(ch, WEAR_LEGS_2, TRUE);
        remove_obj(ch, WEAR_AMMO, TRUE); // 彈藥裝備位置 add at 2022/04/30
        return;
    }

    if (!(obj = get_obj_wear(ch, arg))) {
        send_to_char("你身上沒有這樣東西.\n\r", ch);
        return;
    }

    remove_obj(ch, obj->wear_loc, TRUE);
    return;
}

// do_sacrifice: Command: do_sacrifice - Performs the 'sacrifice' command.
// do_sacrifice: 指令：do_sacrifice - 執行 'sacrifice' 指令。
void do_sacrifice(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (arg[0] == '\0' || !str_cmp(arg, ch->name)) {
        send_to_char("天神很感謝你的奉獻，也許祂以後會接受。", ch);
        act("$n 把自己奉獻給天神，這是多麼偉大的情操。", ch, NULL, NULL,
            TO_ROOM);
        return;
    }

    obj = get_obj_list(ch, arg, ch->in_room->contents);
    if (!obj) {
        send_to_char("你找不到這個東西。\n\r", ch);
        return;
    }

    /* 移至 obj_act 後判定, 讓無法 sac 的物品也能觸發 2022/11/27
            if ( !CAN_WEAR( obj, ITEM_TAKE ) || IS_SET(obj->extra_flags,
       ITEM_NOSAC))
            {
                    act( "$p 無法被奉獻。", ch, obj, NULL, TO_CHAR );
                    return;
            } */

    // 修改 obj_act sac 也能讓 mob 作用 2022/02/12
    // if( !IS_NPC(ch) && IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("sac", ch, obj);
    }
    // obj 可能在 obj_act 時被 purge 2022/02/13
    if (!obj || obj->deleted)
        return;

    if (!CAN_WEAR(obj, ITEM_TAKE) || IS_SET(obj->extra_flags, ITEM_NOSAC)) {
        act("$p 無法被奉獻。", ch, obj, NULL, TO_CHAR);
        return;
    }

    if (!IS_SET(obj->extra_flags, ITEM_CLAN)) {
        // 讓被 sac obj 能被 ch 看見 2022/11/27
        // send_to_char( "為了感謝你的奉獻，天神給了你一些獎勵。\n\r", ch );
        act("你把 $p 奉獻給天神，獲得了一些獎勵。", ch, obj, NULL, TO_CHAR);
        if (!IS_NPC(ch))
            ch->gold += obj->level;
    } else {
        act("你把 $p 奉獻給天神。", ch, obj, NULL, TO_CHAR);
    }

    if (obj->pIndexData->vnum == OBJ_VNUM_CORPSE_NPC && !IS_NPC(ch) &&
        get_skill_percent(ch, gsn_burial_ceremony) > number_percent()) {
        int nBurySklv = get_skill_level(ch, gsn_burial_ceremony);
        int nManaBonus = nBurySklv * nBurySklv / 10;
        ch->mana +=
            obj->level +
            UMAX(0, obj->level + get_skill_level(ch, gsn_burial_ceremony) -
                        ch->level - 5) *
                3 +
            (get_skill_level(ch, gsn_burial_ceremony) << 1);
        ch->mana += obj->level +
                    UMAX(0, obj->level + nBurySklv * 2 - ch->level - 5) *
                        (3 + nManaBonus) +
                    nBurySklv * (nBurySklv + 4);
        send_to_char("你將死者超渡，天神給你一些法力以為回報。\n\r", ch);
    }

    act("$n 把 $p 奉獻給天神。", ch, obj, NULL, TO_ROOM);

    // 新增 a_sac 用於系統字串顯示後 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("a_sac", ch, obj);
    }
    if (!obj || obj->deleted)
        return;

    extract_obj(obj);
    return;
}

// do_adore: Command: do_adore - Performs the 'adore' command.
// do_adore: 指令：do_adore - 執行 'adore' 指令。
void do_adore(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    AFFECT_DATA af;
    char arg[MAX_INPUT_LENGTH];
    int sklv = get_skill_level(ch, gsn_adore);

    if ((IS_NPC(ch) &&
         (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))) ||
        (!IS_NPC(ch)
         //&& ch->pcdata->learned[gsn_adore] < number_percent( ) ) )
         && get_skill_percent(ch, gsn_adore) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    one_argument(argument, arg);
    if (arg[0] == '\0')
        sprintf(arg, "corpse");

    obj = get_obj_list(ch, arg, ch->in_room->contents);
    if (!obj || obj->item_type != ITEM_CORPSE_NPC) {
        send_to_char("你找不到屍體。\n\r", ch);
        return;
    }

    if (obj->item_type == ITEM_CORPSE_PC) {
        act("其他冒險者的屍體是不准碰的。", ch, obj, NULL, TO_CHAR);
        return;
    }

    if (obj->level >= ch->level || obj->level > 36) {
        send_to_char(
            "藉著莊嚴的儀式，你對戰場上陣亡的戰士給予最崇高的敬意。\n\r", ch);
        act("" HIP
            "$n崇敬地對陣亡戰士舉行莊嚴的祭禮，$s眼中同時閃起異樣的光輝。" NOR
            "",
            ch, NULL, NULL, TO_ROOM);
        if (!is_affected(ch, gsn_adore) && ch->hitroll > 2) {

            // 加入 obj_act adore 2022/02/12
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("adore", ch, obj);
            }

            // corpse 可能在 prog 中 被 purge 2022/02/12
            if (!obj)
                return;

            af.type = gsn_adore;
            if (obj->level < ch->level)
                af.duration = obj->level / 4;
            else
                af.duration = obj->level / 3;
            af.location = APPLY_HITROLL;
            af.bitvector = 0;
            af.modifier = -(obj->level / 6) + (sklv > 4 ? 1 : 0);
            affect_to_char(ch, &af);

            if (obj->level < ch->level)
                af.duration = obj->level / 4;
            else
                af.duration = obj->level / 3;
            af.location = APPLY_DAMROLL;
            af.bitvector = 0;
            af.modifier = obj->level / 3 + sklv * 2 / 3;
            affect_to_char(ch, &af);
            // corpse 移除將只有在 adore 成功時成立 2022/02/12
            extract_obj(obj);
        } else {
            send_to_char("你的心已經充滿太多景仰了。\n\r", ch);
        }
    } else {
        send_to_char("這具膽小鬼的屍首不值得你的尊敬。\n\r", ch);
    }
    // remove at 2022/02/12
    // extract_obj( obj );
    return;
}

// do_quaff: Command: do_quaff - Performs the 'quaff' command.
// do_quaff: 指令：do_quaff - 執行 'quaff' 指令。
void do_quaff(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("要飲用什麼藥劑？\n\r", ch);
        return;
    }

    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你身上沒有這樣東西.\n\r", ch);
        return;
    }

    if (obj->item_type != ITEM_POTION) {
        send_to_char("那不是魔法藥劑.\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > ch->level * 5) {
        send_to_char("你喝不下了.\n\r", ch);
    }

    act("你喝下 $p.", ch, obj, NULL, TO_CHAR);
    act("$n 喝下 $p.", ch, obj, NULL, TO_ROOM);

    if (obj->level > ch->level) {
        act("你無法飲用等級比你還高的魔法藥劑.", ch, obj, NULL, TO_CHAR);
    } else {
        /* obj->value[0] is not used for potions */
        obj_cast_spell(obj->value[1], obj->value[0], ch, ch, NULL);
        obj_cast_spell(obj->value[2], obj->value[0], ch, ch, NULL);
        obj_cast_spell(obj->value[3], obj->value[0], ch, ch, NULL);
    }

    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT))
        oprog_act_trigger("quaff", ch, obj);

    if (!IS_NPC(ch) || (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) ||
                                       IS_AFFECTED(ch, AFF_SUMMONED)))) {
        // if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
        extract_obj(obj);
        if (ch->fighting)
            WAIT_STATE(ch, 8);
        else
            WAIT_STATE(ch, 2);
        // cond thirst 修改成 obj weight 2023/01/13
        // gain_condition( ch, COND_THIRST, 8);
        gain_condition(ch, COND_THIRST, obj->weight);
    } else
        WAIT_STATE(ch, 2);
    return;
}

// do_recite: Command: do_recite - Performs the 'recite' command.
// do_recite: 指令：do_recite - 執行 'recite' 指令。
void do_recite(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *scroll;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int sklv = get_skill_level(ch, gsn_scrolls);
    int obj_lv = 0;

    obj_lv += sklv;

    // if(sklv > 3) obj_lv += 1;
    // if(sklv > 4) obj_lv += 3;
    // if(sklv > 5) obj_lv += 5;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (!(scroll = get_obj_carry(ch, arg1))) {
        send_to_char("你身上沒有那樣東西.\n\r", ch);
        return;
    }

    if (IS_AFFECTED(ch, AFF_MUTE) ||
        IS_SET(race_table[ch->race].race_abilities, RACE_MUTE) ||
        IS_SET(ch->in_room->room_flags, ROOM_CONE_OF_SILENCE)) {
        send_to_char("你的嘴唇在動，但是沒有聲音發出來.\n\r", ch);
        return;
    }

    if (scroll->item_type != ITEM_SCROLL) {
        act("你大聲唸誦著$p.", ch, scroll, NULL, TO_CHAR);
        // 不是 scroll 也能觸發 recite obj_act 2022/02/13
        if (IS_SET(scroll->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("recite", ch, scroll);
        }
        return;
    }

    /*check obj_act when recite scroll , add by coconet*/
    if (IS_SET(scroll->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("recite", ch, scroll);
    }

    // scroll 可能在 obj_act 時消失 2022/02/13
    if (!scroll)
        return;

    /*
    if ( scroll->item_type != ITEM_SCROLL )
    {
            send_to_char( "那不是魔法卷軸.\n\r", ch );
            return;
    }
    */

    obj = NULL;
    if (arg2[0] == '\0') {
        victim = ch;
        if (ch->fighting)
            victim = ch->fighting;
    } else {
        if (!(victim = get_char_room(ch, arg2)) &&
            !(obj = get_obj_here(ch, arg2))) {
            send_to_char("你找不到目標.\n\r", ch);
            return;
        }
    }

    if ((IS_NPC(ch) && IS_SET(ch->act, ACT_PET)) ||
        IS_AFFECTED(ch, AFF_CHARM)) {
        act("你現在不能自主.", ch, scroll, NULL, TO_CHAR);
        return;
    }

    if (scroll->level > ch->level + sklv * 2) {
        act("你無法朗誦比你高級的魔法卷軸.", ch, scroll, NULL, TO_CHAR);
        return;
    }

    // 讓 recite stun 受到 sklv 影響 2022/01/13
    // WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
    WAIT_STATE(ch, 16 - sklv);

    act("你朗誦了 $p.", ch, scroll, NULL, TO_CHAR);
    act("$n 朗誦了 $p.", ch, scroll, NULL, TO_ROOM);

    /* Scrolls skill by Binky for EnvyMud, modified by Thelonius */
    if (!IS_NPC(ch) &&
        !(number_percent() < get_skill_percent(ch, gsn_scrolls) + sklv * 10)) {
        switch (number_bits(3)) {
        case 0:
        case 1:
        case 2:
        case 3:
            act("你根本看不懂 $p 的內容.", ch, scroll, NULL, TO_CHAR);
            return;
        case 4:
        case 5:
        case 6:
            send_to_char("你一定是有那裡念錯了.\n\r", ch);
            act("$p 突然起火燃燒，轉眼間燒個精光.", ch, scroll, NULL, TO_CHAR);
            act("$p 突然起火燃燒，轉眼間燒個精光.", ch, scroll, NULL, TO_ROOM);
            extract_obj(scroll);
            return;
        case 7:
            act("你順利地朗誦完畢， $p 發出一陣耀眼的光芒，而後消失了。", ch,
                scroll, NULL, TO_CHAR);
            act("$p 發出一陣耀眼的光芒，然後消失在你眼前。", ch, scroll, NULL,
                TO_ROOM);
            /*
             * damage( ) call after extract_obj in case the damage would
             * have extracted ch.  This is okay because we merely mark
             * obj->deleted; it still retains all values until list_update.
             * Sloppy?  Okay, create another integer variable. ---Thelonius
             */
            extract_obj(scroll);
            ////damage( ch, ch, scroll->level, gsn_scrolls, WEAR_NONE );
            damage(ch, ch, scroll->level, gsn_scrolls, WEAR_NONE, MSG_NORMAL,
                   DMG_NORMAL);
            return;
        }
    }

    /* scroll->value[0] is now used for scrolls */

    // 防止 TAR_CHAR_SELF 的 spell 對 obj 使用成功後造成crash add at 2021/09/15
    // 防止 TAR_IGNORE 及 TAR_ARG 對 spell 使用抓不到目標造成 crash add at
    // 2021/09/22

    // first spell
    if (skill_table[scroll->value[1]].target == TAR_CHAR_SELF && victim != ch) {
        obj_cast_spell(scroll->value[1], scroll->value[0] + obj_lv, ch, ch,
                       obj);
    } else if (skill_table[scroll->value[1]].target == TAR_IGNORE) {
        // 裝備附魔 2021/10/02
        if (scroll->value[1] == 314 || scroll->value[1] == 315 ||
            scroll->value[1] == 316 || scroll->value[1] == 317 ||
            scroll->value[1] == 318 || scroll->value[1] == 319 ||
            scroll->value[1] == 320 || scroll->value[1] == 321)
            obj_cast_spell(scroll->value[1], scroll->value[0] + obj_lv, ch, ch,
                           obj);
        else
            obj_cast_spell(scroll->value[1], scroll->value[0] + obj_lv, ch, ch,
                           obj);
    } else if (skill_table[scroll->value[1]].target == TAR_ARG) {
        obj_cast_spell(scroll->value[1], scroll->value[0] + obj_lv, ch, ch,
                       obj);
    } else {
        obj_cast_spell(scroll->value[1], scroll->value[0] + obj_lv, ch, victim,
                       obj);
    }
    // second spell
    if (skill_table[scroll->value[2]].target == TAR_CHAR_SELF && victim != ch) {
        obj_cast_spell(scroll->value[2], scroll->value[0] + obj_lv, ch, ch,
                       obj);
    } else if (skill_table[scroll->value[2]].target == TAR_IGNORE) {
        if (scroll->value[2] == 314 || scroll->value[2] == 315 ||
            scroll->value[2] == 316 || scroll->value[2] == 317 ||
            scroll->value[2] == 318 || scroll->value[2] == 319 ||
            scroll->value[2] == 320 || scroll->value[2] == 321)
            obj_cast_spell(scroll->value[2], scroll->value[0] + obj_lv, ch, ch,
                           obj);
        else
            obj_cast_spell(scroll->value[2], scroll->value[0] + obj_lv, ch, ch,
                           obj);
    } else if (skill_table[scroll->value[2]].target == TAR_ARG) {
        obj_cast_spell(scroll->value[2], scroll->value[0] + obj_lv, ch, ch,
                       obj);
    } else {
        obj_cast_spell(scroll->value[2], scroll->value[0] + obj_lv, ch, victim,
                       obj);
    }
    // third spell
    if (skill_table[scroll->value[3]].target == TAR_CHAR_SELF && victim != ch) {
        obj_cast_spell(scroll->value[3], scroll->value[0] + obj_lv, ch, ch,
                       obj);
    } else if (skill_table[scroll->value[3]].target == TAR_IGNORE) {
        if (scroll->value[3] == 314 || scroll->value[3] == 315 ||
            scroll->value[3] == 316 || scroll->value[3] == 317 ||
            scroll->value[3] == 318 || scroll->value[3] == 319 ||
            scroll->value[3] == 320 || scroll->value[3] == 321)
            obj_cast_spell(scroll->value[3], scroll->value[0] + obj_lv, ch, ch,
                           obj);
        else
            obj_cast_spell(scroll->value[3], scroll->value[0] + obj_lv, ch, ch,
                           obj);
    } else if (skill_table[scroll->value[3]].target == TAR_ARG) {
        obj_cast_spell(scroll->value[3], scroll->value[0] + obj_lv, ch, ch,
                       obj);
    } else {
        obj_cast_spell(scroll->value[3], scroll->value[0] + obj_lv, ch, victim,
                       obj);
    }
    /*
            obj_cast_spell( scroll->value[1], scroll->value[0] , ch, victim, obj
       ); obj_cast_spell( scroll->value[2], scroll->value[0] , ch, victim, obj
       ); obj_cast_spell( scroll->value[3], scroll->value[0] , ch, victim, obj
       );
    */
    // if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
    if (!IS_NPC(ch) || (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) ||
                                       IS_AFFECTED(ch, AFF_SUMMONED))))
        extract_obj(scroll);
    return;
}

// do_brandish: Command: do_brandish - Performs the 'brandish' command.
// do_brandish: 指令：do_brandish - 執行 'brandish' 指令。
void do_brandish(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *staff;
    CHAR_DATA *vch, *victim;
    int sn;
    int chance;
    int sklv = get_skill_level(ch, gsn_staves);
    int obj_lv = 0;

    obj_lv += sklv;
    // if(sklv > 3) obj_lv += 1;
    // if(sklv > 4) obj_lv += 3;
    // if(sklv > 5) obj_lv += 5;

    if (!(staff = get_eq_char(ch, WEAR_HOLD))) {
        send_to_char("你揮了揮你的雙手，看來什麼事也沒發生。\n\r", ch);
        return;
    }

    if (staff->item_type != ITEM_STAFF) {
        act("你揮舞著$p.", ch, staff, NULL, TO_CHAR);
        // 不是 item_staff 也可以觸發 brandish obj_act 2022/02/13
        if (IS_SET(staff->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("brandish", ch, staff);
        }
        return;
    }

    if (IS_SET(staff->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("brandish", ch, staff);
    }

    // staff 可能在 obj_act 時被 prog 2022/02/13
    if (!staff)
        return;

    /*
    if ( staff->item_type != ITEM_STAFF )
    {
            send_to_char( "只有法杖可以拿來揮舞.\n\r", ch );
            return;
    }
    */

    // if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
    // || IS_AFFECTED( ch, AFF_CHARM ) )
    if (IS_AFFECTED(ch, AFF_CHARM)) {
        act("你試著揮舞 $p, 但是你的雙手看來不聽指揮.", ch, staff, NULL,
            TO_CHAR);
        act("$n 試著揮舞 $p, 但是雙手好像不太聽話.", ch, staff, NULL, TO_ROOM);
        return;
    }

    if ((sn = staff->value[3]) < 0 || sn >= MAX_SKILL ||
        skill_table[sn].spell_fun == 0) {
        bug("Do_brandish: bad sn %d.", sn);
        return;
    }

    // WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
    WAIT_STATE(ch, 20 - sklv);

    if (staff->value[2] > 0) {
        CHAR_DATA *vch_next;

        act("你揮動 $p，引發內含的魔力.", ch, staff, NULL, TO_CHAR);
        act("$n 揮舞 $p.", ch, staff, NULL, TO_ROOM);

        /* Staves skill by Binky for EnvyMud, modified by Thelonius */
        /* NPC can use staff by Amenda */

        // ch->pcdata->learned[gsn_staves];
        //  if ( !IS_NPC( ch ) && !( number_percent( ) <
        //  ch->pcdata->learned[gsn_staves] ) )
        chance = (IS_NPC(ch)) ? ch->level * 2 + 10
                              : get_skill_percent(ch, gsn_staves) + sklv * 10;
        if (chance < number_percent()) {
            switch (number_bits(3)) {
            case 0:
            case 1:
            case 2:
            case 3:
                act("你無法引發 $p 的魔力.", ch, staff, NULL, TO_CHAR);
                act("$n 無法引發 $p 的魔力.", ch, staff, NULL, TO_ROOM);
                return;
            case 4:
            case 5:
            case 6:
                act("你引發了 $p 的魔力, 但是它立刻消散了.", ch, staff, NULL,
                    TO_CHAR);
                act("$n 引發了 $p 的魔力, 但是它立刻消散了.", ch, staff, NULL,
                    TO_ROOM);
                if (!IS_NPC(ch) ||
                    (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) ||
                                    IS_AFFECTED(ch, AFF_SUMMONED)))) {
                    if (--staff->value[2] <= 0) {
                        // 有 z4 的 staff 不會消失 add at 2020/10/18
                        if (IS_SET(staff->pIndexData->souvenir, Z4)) {
                            act(HIY
                                "$p 耗盡魔力發出一陣強光後逐漸暗淡下來." NOR,
                                ch, staff, NULL, TO_CHAR);
                            act(HIY "$n 手中的 $p "
                                    "耗盡魔力發出一陣強光後逐漸暗淡下來." NOR,
                                ch, staff, NULL, TO_ROOM);
                            staff->value[2] = 0;
                        } else {
                            act(HIY "$p 發出一陣強光，接著化為灰燼." NOR, ch,
                                staff, NULL, TO_CHAR);
                            act(HIY
                                "$n 手中的 $p 發出一陣強光，接著化為灰燼." NOR,
                                ch, staff, NULL, TO_ROOM);
                            extract_obj(staff);
                        }
                    }
                }
                return;
            case 7:
                /*
                 * damage( ) call after extract_obj in case the damage would
                 * have extracted ch.  This is okay because we merely mark
                 * obj->deleted; it still retains all values until list_update.
                 * Sloppy?  Okay, create another integer variable. ---Thelonius
                 */
                // 有 z4 的 staff 不會消失 add at 2020/10/18
                if (IS_SET(staff->pIndexData->souvenir, Z4)) {
                    act(HIR "不妙！你控制不了 $p，能量從中噴湧而出！" NOR, ch,
                        staff, NULL, TO_CHAR);
                    act(HIR "$n 控制不了 $p，被魔力炸傷了！" NOR, ch, staff,
                        NULL, TO_ROOM);
                    staff->value[2] = 0;
                } else {
                    act(HIR "不妙！你控制不了 $p "
                            "的魔力，眼睜睜地看著它在你手中化為碎片！" NOR,
                        ch, staff, NULL, TO_CHAR);
                    act(HIR "$n 手中的 $p 突然化成碎片！" NOR, ch, staff, NULL,
                        TO_ROOM);
                    extract_obj(staff);
                }
                // damage( ch, ch, staff->level, gsn_staves, WEAR_NONE );
                damage(ch, ch, staff->level, gsn_staves, WEAR_NONE, MSG_NORMAL,
                       DMG_NORMAL);
                return;
            }
        }
        // 移除正常等級下 staff 對單體使用的設定 2022/01/13
        // if ( sklv * 4 + staff->value[0] < 25 )
        if (staff->value[0] < 0) {
            if (argument[0] == '\0') {
                if (ch->fighting)
                    victim = ch->fighting;
                else
                    victim = ch;
            } else if (!(victim = get_char_room(ch, argument))) {
                send_to_char("這裡沒有這個人。\n\r", ch);
                return;
            }
            if (skill_table[sn].target == TAR_CHAR_SELF && victim != ch)
                return;
            obj_cast_spell(staff->value[3], staff->value[0] + obj_lv, ch,
                           victim, NULL);
            staff->value[0] - obj_lv;
        } else
            for (vch = ch->in_room->people; vch; vch = vch_next) {
                vch_next = vch->next_in_room;

                if (vch->deleted)
                    continue;

                if (!IS_NPC(ch) && IS_SET(ch->act, PLR_WIZINVIS))
                    continue;

                if (!IS_NPC(vch) &&
                    IS_SET(vch->act,
                           PLR_WIZINVIS)) // 0941003 by jye 排除imm char
                    continue;

                switch (skill_table[sn].target) {
                default:
                    bug("Do_brandish: bad target for sn %d.", sn);
                    return;

                case TAR_IGNORE:
                    if (vch != ch)
                        continue;
                    break;

                case TAR_CHAR_OFFENSIVE:
                    if (is_same_group(ch, vch))
                        continue;
                    break;
                /*
                case TAR_CHAR_DEFENSIVE:
                        if ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
                                continue;
                                break;
                */
                case TAR_CHAR_SELF:
                    if (vch != ch)
                        continue;
                    break;

                case TAR_CHAR_DEFENSIVE:
                    break;
                }

                /* staff->value[0] is now used for staves */
                obj_cast_spell(staff->value[3], staff->value[0] + obj_lv, ch,
                               vch, NULL);
            }
    } else {
        // staff v2 <= 0 的情況下 add at 2020/10/18
        act("你試著引發 $p 的魔力, 但毫無任何反應.", ch, staff, NULL, TO_CHAR);
        act("$n 試著引發 $p 的魔力, 但毫無任何反應.", ch, staff, NULL, TO_ROOM);
        return;
    }

    if (!IS_NPC(ch) || (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) ||
                                       IS_AFFECTED(ch, AFF_SUMMONED)))) {
        // if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
        if (number_percent() < 2 * sklv * sklv)
            return;

        if (--staff->value[2] <= 0) {
            // 有 z4 的 staff 不會消失 add at 2020/10/18
            if (IS_SET(staff->pIndexData->souvenir, Z4)) {
                act(HIY "$p 耗盡魔力發出一陣強光後逐漸暗淡下來." NOR, ch, staff,
                    NULL, TO_CHAR);
                act(HIY "$n 手中的 $p 耗盡魔力發出一陣強光後逐漸暗淡下來." NOR,
                    ch, staff, NULL, TO_ROOM);
                staff->value[2] = 0;
            } else {
                act(HIY "$p 發出一陣強光，接著化為灰燼." NOR, ch, staff, NULL,
                    TO_CHAR);
                act(HIY "$n 手中的 $p 發出一陣強光，接著化為灰燼." NOR, ch,
                    staff, NULL, TO_ROOM);
                extract_obj(staff);
            }
        }
    }

    return;
}

// do_zap: Command: do_zap - Performs the 'zap' command.
// do_zap: 指令：do_zap - 執行 'zap' 指令。
void do_zap(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *wand;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int sn, chance;
    int sklv = get_skill_level(ch, gsn_wands);
    int obj_lv = 0;

    obj_lv += sklv;
    // if(sklv > 3) obj_lv += 1;
    // if(sklv > 4) obj_lv += 3;
    // if(sklv > 5) obj_lv += 5;

    one_argument(argument, arg);
    if (arg[0] == '\0' && !ch->fighting) {
        send_to_char("Zap 什麼?\n\r", ch);
        return;
    }

    if (!(wand = get_eq_char(ch, WEAR_HOLD))) {
        send_to_char("你彈了彈手指，看來什麼事也沒發生.\n\r", ch);
        return;
    }

    if (wand->item_type != ITEM_WAND) {
        act("你揮動著$p.", ch, wand, NULL, TO_CHAR);
        // 不是 item_wand 的手握也能觸發 2022/02/13
        if (IS_SET(wand->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("zap", ch, wand);
        }
        return;
    }

    if (IS_SET(wand->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("zap", ch, wand);
    }

    // wand 可能在 obj_act 被 purge 2022/02/13
    if (!wand)
        return;

    /*
            if ( wand->item_type != ITEM_WAND )
            {
                    send_to_char( "那不是魔杖.\n\r", ch );
                    return;
            }
    */

    obj = NULL;
    if (arg[0] == '\0') {
        if (ch->fighting) {
            victim = ch->fighting;
        } else {
            send_to_char("Zap 什麼?\n\r", ch);
            return;
        }
    } else {
        if (!(victim = get_char_room(ch, arg)) &&
            !(obj = get_obj_here(ch, arg))) {
            send_to_char("這裡沒有這樣東西.\n\r", ch);
            return;
        }
    }

    //    if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
    //  || IS_AFFECTED( ch, AFF_CHARM ) )
    if (IS_AFFECTED(ch, AFF_CHARM)) {
        act("你想要使用 $p, 但是你的雙手看來不聽指揮.", ch, wand, NULL,
            TO_CHAR);
        act("$n 試著使用 $p, 但是雙手不聽指揮.", ch, wand, NULL, TO_ROOM);
        return;
    }

    if ((sn = wand->value[3]) < 0 || sn >= MAX_SKILL ||
        skill_table[sn].spell_fun == 0) {
        bug("Do_zap: bad sn %d.", sn);
        return;
    }

    // WAIT_STATE( ch, 2 * PULSE_VIOLENCE);
    WAIT_STATE(ch, 16 - sklv);

    if (wand->value[2] > 0) {
        if (victim) {
            if (victim == ch) {
                act("你對自己使用了 $p.", ch, wand, NULL, TO_CHAR);
                act("$n 對自己使用了 $p.", ch, wand, NULL, TO_ROOM);
            } else {
                act("你對 $N 使用了 $p.", ch, wand, victim, TO_CHAR);
                act("$n 對 $N 使用了 $p.", ch, wand, victim, TO_ROOM);
            }
        } else {
            act("你對 $P 使用了 $p.", ch, wand, obj, TO_CHAR);
            act("$n 對 $P 使用了 $p.", ch, wand, obj, TO_ROOM);
        }

        /* Wands skill by Binky for EnvyMud, modified by Thelonius */
        // ch->pcdata->learned[gsn_wands];
        //   if ( !IS_NPC( ch )
        //       && !( number_percent( ) < ch->pcdata->learned[gsn_wands] ) )
        chance = (IS_NPC(ch)) ? ch->level * 2 + 10
                              : get_skill_percent(ch, gsn_wands) + sklv * 2;
        if (chance < number_percent()) {
            switch (number_bits(3)) {
            case 0:
            case 1:
            case 2:
            case 3:
                act("你無法引發 $p 的魔力.", ch, wand, NULL, TO_CHAR);
                act("$n 無法引發 $p 的魔力.", ch, wand, NULL, TO_ROOM);
                return;
            case 4:
            case 5:
            case 6:
                act("你引發了 $p 的魔力, 但是它立刻消散了.", ch, wand, NULL,
                    TO_CHAR);
                act("$n 引發了 $p 的魔力, 但是它立刻消散了.", ch, wand, NULL,
                    TO_ROOM);
                if (!IS_NPC(ch) ||
                    (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) ||
                                    IS_AFFECTED(ch, AFF_SUMMONED)))) {
                    if (--wand->value[2] <= 0) {
                        // 有 z4 的 wand 不會消失 add at 2020/10/18
                        if (IS_SET(wand->pIndexData->souvenir, Z4)) {
                            act(HIY
                                "$p 耗盡魔力發出一陣強光後逐漸暗淡下來." NOR,
                                ch, wand, NULL, TO_CHAR);
                            act(HIY "$n 手中的 $p "
                                    "耗盡魔力發出一陣強光後逐漸暗淡下來." NOR,
                                ch, wand, NULL, TO_ROOM);
                            wand->value[2] = 0;
                        } else {
                            act(HIY "$p 發出一陣強光，然後化為灰燼." NOR, ch,
                                wand, NULL, TO_CHAR);
                            act(HIY
                                "$n 手中的 $p 發出一陣強光，然後化為灰燼." NOR,
                                ch, wand, NULL, TO_ROOM);
                            extract_obj(wand);
                        }
                    }
                }
                return;
            case 7:
                /*
                 * damage( ) call after extract_obj in case the damage would
                 * have extracted ch.  This is okay because we merely mark
                 * obj->deleted; it still retains all values until list_update.
                 * Sloppy?  Okay, create another integer variable. ---Thelonius
                 */
                // 有 z4 的 wand 不會消失 add at 2020/10/18
                if (IS_SET(wand->pIndexData->souvenir, Z4)) {
                    act(HIR "不妙！你控制不了 $p，能量從中噴湧而出！" NOR, ch,
                        wand, NULL, TO_CHAR);
                    act(HIR "$n 控制不了 $p，被魔力炸傷了！" NOR, ch, wand,
                        NULL, TO_ROOM);
                    wand->value[2] = 0;
                } else {
                    act(HIR "不妙！你控制不了 $p "
                            "的魔力，眼睜睜地看著它在你手中化為碎片！" NOR,
                        ch, wand, NULL, TO_CHAR);
                    act(HIR "$n 手中的 $p 突然化成碎片！" NOR, ch, wand, NULL,
                        TO_ROOM);
                    extract_obj(wand);
                }
                // damage( ch, ch, wand->level, gsn_wands, WEAR_NONE );
                damage(ch, ch, wand->level, gsn_wands, WEAR_NONE, MSG_NORMAL,
                       DMG_NORMAL);
                return;
            }
        }

        /* wand->value[0] is now used for wands */

        // 防止 TAR_CHAR_SELF 的 spell 對 obj 使用成功後造成crash add at
        // 2021/09/15 防止 TAR_IGNORE 及 TAR_ARG 對 spell 使用抓不到目標造成
        // crash add at 2021/09/22

        if (skill_table[wand->value[3]].target == TAR_CHAR_SELF &&
            victim != ch) {
            obj_cast_spell(wand->value[3], wand->value[0] + obj_lv, ch, ch,
                           obj);
        } else if (skill_table[wand->value[3]].target == TAR_IGNORE) {
            if (wand->value[3] == 314 || wand->value[3] == 315 ||
                wand->value[3] == 316 || wand->value[3] == 317 ||
                wand->value[3] == 318 || wand->value[3] == 319 ||
                wand->value[3] == 320 || wand->value[3] == 321)
                obj_cast_spell(wand->value[3], wand->value[0] + obj_lv, ch, ch,
                               obj);
            else
                obj_cast_spell(wand->value[3], wand->value[0] + obj_lv, ch, ch,
                               obj);
        } else if (skill_table[wand->value[3]].target == TAR_ARG) {
            obj_cast_spell(wand->value[3], wand->value[0] + obj_lv, ch, ch,
                           obj);
        } else {
            obj_cast_spell(wand->value[3], wand->value[0] + obj_lv, ch, victim,
                           obj);
        }
    } else {
        // wand v2 <= 0 的情況下 add at 2020/10/18
        act("你試著引發 $p 的魔力, 但毫無任何反應.", ch, wand, NULL, TO_CHAR);
        act("$n 試著引發 $p 的魔力, 但毫無任何反應.", ch, wand, NULL, TO_ROOM);
        return;
    }

    // if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) ){
    if (!IS_NPC(ch) || (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) ||
                                       IS_AFFECTED(ch, AFF_SUMMONED)))) {
        if (number_percent() < 2 * sklv * sklv)
            return;
        if (--wand->value[2] <= 0) {
            // 有 z4 的 wand 不會消失 add at 2020/10/18
            if (IS_SET(wand->pIndexData->souvenir, Z4)) {
                act(HIY "$p 耗盡魔力發出一陣強光後逐漸暗淡下來." NOR, ch, wand,
                    NULL, TO_CHAR);
                act(HIY "$n 手中的 $p 耗盡魔力發出一陣強光後逐漸暗淡下來." NOR,
                    ch, wand, NULL, TO_ROOM);
                wand->value[2] = 0;
            } else {
                act("$p 耗盡魔力，化成碎片.", ch, wand, NULL, TO_CHAR);
                act("$n 手中的 $p 耗盡魔力，化成碎片.", ch, wand, NULL,
                    TO_ROOM);
                extract_obj(wand);
            }
        }
    }

    return;
}

// do_steal: Command: do_steal - Performs the 'steal' command.
// do_steal: 指令：do_steal - 執行 'steal' 指令。
void do_steal(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int number;
    int count;
    int percent;
    int sklv = get_skill_level(ch, gsn_steal);

    if (IS_NPC(ch) || IS_SET(ch->act, PLR_HORSE)) {
        send_to_char("不行.\n\r", ch);
        return;
    }

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0') {
        send_to_char("從誰身上偷什麼?\n\r", ch);
        return;
    }

    if (!(victim = get_char_room(ch, arg2))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (IS_NPC(victim) && IS_SET(victim->act, ACT_HORSE)) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (victim == ch) {
        send_to_char("你摸了摸自己的口袋.\n\r", ch);
        return;
    }

    if (is_safe(ch, victim))
        return;

    obj = NULL;

    WAIT_STATE(ch, skill_table[gsn_steal].beats);

    /* Modified chances for stealing by Morpheus */
    percent = ch->level * 3 / 2; /* Base value */

    percent += number_range(-10, 10); /* Luck */

    percent -= victim->level * 2; /* Character level vs victim's */

    percent += sklv + sklv / 2 + (sklv / 6) * 4;

    if (!IS_AWAKE(victim))
        percent += 20; /* Sleeping characters are easier */

    // percent += ch->pcdata->learned[gsn_steal]; /* Character ability */
    percent += get_skill_percent(ch, gsn_steal);

    if (IS_AFFECTED(ch, AFF_SNEAK))
        percent += 5; /* Quiet characters steal better */

    if (!can_see(ch, victim))
        percent += 10; /* Unseen characters steal better */

    percent = (percent * weakness_percent(victim, gsn_steal)) / 100;
    if (!str_prefix(arg1, "coins") || !str_cmp(arg1, "gold"))
        percent = (int)(percent * 1.2); /* Gold is fairly easy to steal */
    else {
        number = number_argument(arg1, arg);
        count = 0;
        for (obj = victim->carrying; obj; obj = obj->next_content) {
            if ( // obj->wear_loc == WEAR_NONE
                /*&&*/ can_see_obj(ch, obj) && is_name(arg, obj->name)) {
                if (++count == number)
                    break;
            }
        }

        if (!obj) {
            send_to_char("沒有這樣物品.\n\r", ch);
            return;
        }

        if (obj->wear_loc == WEAR_NONE) {
            /* Items wearing are harder */
            if (victim->level - ch->level < 3)
                percent = (int)(percent * .8);
            else if (victim->level - ch->level < 8)
                percent = (int)(percent * .6);
            else if (victim->level - ch->level < 15)
                percent = (int)(percent * .4);
            else
                percent = (int)(percent * .2);
        } else
            percent = (int)(percent * .05);
    }
    if (IS_NPC(victim) && victim->pIndexData->pShop) {
        percent = -1;
    }

    if ((!IS_NPC(victim) && ((!IS_SET(victim->act, PLR_KILLER) &&
                              !IS_SET(victim->act, PLR_THIEF) &&
                              !IS_SET(victim->act, PLR_REGISTER) &&
                              victim->race != race_lookup("Vampire")) ||
                             ch->level - victim->level < 5)) ||
        percent < number_percent()) {
        /* Failure.*/
        send_to_char("糟糕，被發現了！\n\r", ch);
        act("你發現 $n 想從你身上偷東西！\n\r", ch, NULL, victim, TO_VICT);
        act("$n 想從 $N 身上偷東西，但是被發現了！ \n\r", ch, NULL, victim,
            TO_NOTVICT);
        sprintf(buf, "%s 是個可惡的小偷！", ch->name);
        do_shout(victim, buf);
        if (IS_NPC(victim)) {
            multi_hit(victim, ch, TYPE_UNDEFINED);
        } else {
            if (!licensed(ch)) {
                //  send_to_char(
                //       "你沒有 PK 執照！  你損失了 "HIG"400"NOR" 點經驗.\n\r",
                //       ch );
                //  gain_exp( ch, -400 );
                if (registered(ch, victim) && ch->level - victim->level < 6)
                    return;
            }

            if (ch->level - victim->level > 5 || !registered(ch, victim)) {
                log_string(buf);
                if (!IS_SET(ch->act, PLR_THIEF)) {
                    SET_BIT(ch->act, PLR_THIEF);
                    send_to_char(HIP "***  你是個小偷！！ ***" NOR "\n\r", ch);
                    save_char_obj(ch);
                }
            }
        }
        return;
    }

    if (!str_prefix(arg1, "coins") || !str_cmp(arg1, "gold")) {
        int amount;

        amount = victim->gold * number_range(1, 10) / 20;
        if (amount <= 0) {
            send_to_char("你偷不到半毛錢.\n\r", ch);
            return;
        }

        ch->gold += amount;
        victim->gold -= amount;
        sprintf(buf, "得手了！  你偷到了 " HIY "%d" NOR " 枚金幣.\n\r", amount);
        send_to_char(buf, ch);
        return;
    }

    if (!can_drop_obj(ch, obj) || IS_SET(obj->extra_flags, ITEM_INVENTORY) ||
        obj->level > ch->level) {
        send_to_char("你摸到了，可是卻拿不走.\n\r", ch);
        return;
    }

    if (ch->carry_number + get_obj_number(obj) > can_carry_n(ch)) {
        send_to_char("你拿不下更多東西了.\n\r", ch);
        return;
    }

    if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch)) {
        send_to_char("你帶不動更多重量了.\n\r", ch);
        return;
    }

    // Modified by Razgriz 20050926
    if (obj->wear_loc != WEAR_NONE) {
        send_to_char("你摸到了，可是卻拿不走.\n\r", ch);
        return;
        // unequip_char( victim, obj );
    }

    // 敘述移到 obj 離開前 2022/02/12
    send_to_char("得手了！\n\r", ch);
    send_to_char("Ok.\n\r", ch);
    obj_from_char(obj);
    obj_to_char(obj, ch);

    // 新增 obj_act steal 判定偷竊的角色 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("steal", ch, obj);
    }

    // obj 可能在 obj_act 時被 purge 2022/02/13
    if (!obj)
        return;

    // 新增 obj_act steal_target 判定被偷的角色 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("steal_target", victim, obj);
    }

    // send_to_char( "得手了！\n\r", ch );
    // send_to_char( "Ok.\n\r", ch );
    return;
}

/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;
    char buf[MAX_STRING_LENGTH];

    pShop = NULL;

    for (keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room) {
        /*
        if ( IS_NPC( keeper ) && ( pShop = keeper->pIndexData->pShop )
                && ( argument[0] == '\0' || is_name( argument, keeper->name ) )
        ) break;
        */
        if (!IS_NPC(keeper))
            continue;

        if (IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) &&
            argument[0] == '\0')
            break;
    }
    /*
    if ( !keeper || !pShop || ( keeper && IS_AFFECTED( keeper, AFF_CHARM ) ) )
    {
            send_to_char( "你無法這麼做 .\n\r", ch );
            return NULL;
    }
    */
    /*
    if ( argument[0] != '\0' && !keeper ){
            send_to_char( "這裡沒有這個人.\n\r", ch );
            return NULL;
    }

    if ( !IS_NPC( keeper ) ){
            send_to_char( "玩家買賣請用 bid 及 auction 指令.\n\r", ch );
            return NULL;
    }
    */
    if (argument[0] != '\0') {
        keeper = get_char_room(ch, argument);
        if (keeper == NULL) {
            send_to_char("沒有這個人.\n\r", ch);
            return NULL;
        }
        if (!IS_NPC(keeper)) {
            send_to_char("玩家買賣請用 bid 及 auction 指令.\n\r", ch);
            return NULL;
        }
        pShop = keeper->pIndexData->pShop;
    }

    if (!keeper) {
        send_to_char("這裡沒有人跟你進行買賣.\n\r", ch);
        return NULL;
    }

    if (!pShop) {
        send_to_char("他不跟你進行買賣.\n\r", ch);
        return NULL;
    }

    if (keeper && IS_AFFECTED(keeper, AFF_CHARM)) {
        send_to_char("他沒有買賣的意願.\n\r", ch);
        return NULL;
    }

    /*
     * Undesirables.
     */
    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_KILLER)) {
        do_say(keeper, "這裡不歡迎兇手！");
        sprintf(buf, "可惡的兇手 %s 在這裡！\n\r", ch->name);
        do_shout(keeper, buf);
        return NULL;
    }

    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_THIEF)) {
        do_say(keeper, "這裡不歡迎小偷！");
        sprintf(buf, "可惡的小偷 %s 在這裡！ \n\r", ch->name);
        do_shout(keeper, buf);
        return NULL;
    }

    /*
     * Shop hours.
     */
    /*      old version
    if ( time_info.hour < pShop->open_hour )
    {
            do_say( keeper, "抱歉，本店尚未開張，請晚點再來." );
            return NULL;
    }

    if ( time_info.hour > pShop->close_hour )
    {
            do_say( keeper, "今天已經打烊了，明天請早." );
            return NULL;
    } */

    // 新增open > close(夜間營業 or 日夜顛倒商店) 及 open = close 的判定 ,
    // 20/02/24
    if (pShop->close_hour > pShop->open_hour) {
        if (time_info.hour < pShop->open_hour) {
            do_say(keeper, "本店尚未開張，請晚點再來.");
            return NULL;
        }

        if (time_info.hour > pShop->close_hour) {
            do_say(keeper, "今天已經打烊了，明天請早.");
            return NULL;
        }
    } else if (pShop->close_hour < pShop->open_hour) {
        if (time_info.hour < pShop->open_hour &&
            time_info.hour - pShop->close_hour > 0) {
            do_say(keeper, "今天已經打烊了，明天請早.");
            return NULL;
        }
    } else {
        if (time_info.hour != pShop->open_hour) {
            do_say(keeper, "今天已經打烊了，明天請早.");
            return NULL;
        }
    }

    /*
     * Invisible or hidden people.
     */
    if (!can_see(keeper, ch)) {
        do_say(keeper, "誰在和我說話？");
        return NULL;
    }

    return keeper;
}

// get_cost: Calculates the buy/sell price of an item at a shop keeper.
// get_cost: 計算向商人買入或賣出特定物品時的價格。
int get_cost(CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy) {
    SHOP_DATA *pShop;
    int cost;

    if (!obj || !(pShop = keeper->pIndexData->pShop))
        return 0;

    if (fBuy) {
        cost = obj->cost * UMAX(pShop->profit_buy, 100) / 100;
    } else {
        OBJ_DATA *obj2;
        int itype;

        cost = 0;
        for (itype = 0; itype < MAX_TRADE; itype++) {
            if (obj->item_type == pShop->buy_type[itype]) {
                cost = obj->cost * UMIN(pShop->profit_sell, 100) / 100;
                break;
            }
        }

        for (obj2 = keeper->carrying; obj2; obj2 = obj2->next_content) {
            if (obj->pIndexData == obj2->pIndexData)
                cost /= 2;
        }
    }

    if (obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND)
        cost = cost * obj->value[2] / obj->value[1];

    return cost;
}

// 新增以 qmark 判定的非賣品 2022/05/05
// not_for_sale: Checks if an item from a shop keeper is not for sale.
// not_for_sale: 檢查商人身上的特定物品是否為非賣品。
bool not_for_sale(CHAR_DATA *ch, CHAR_DATA *keeper, OBJ_DATA *obj) {

    /*  目前有 5 種 qmark , 每個 value 代表一個 obj vnum
            因此每個 mob 能有 15 件非賣品
            商人 not_for_sale v0 v1 v2 值分別對應 買家 for_sale 的 v0 值 :

                                                    v0       v1       v3
            not_for_sale_1 :    1        2        4
            not_for_sale_2 :    8        16       32
            not_for_sale_3 :    64       128      256
            not_for_sale_4 :    512      1024     2048
            not_for_sale_5 :    4096     8192     16384

            如果值對應到, 則 not_for_sale 就不成立 (買家可以購買該物品)
            註: 不用擔心 list 跟 buy 的編號, 這些本來就依 char 不同而不同 */

    QMARK *qm;
    char saleqm[MAX_INPUT_LENGTH];

    // 判定 買家 有沒有 for_sale_vnum( mob 商人的 vnum ) 的 qmark
    sprintf(saleqm, "for_sale_%d", keeper->pIndexData->vnum);
    qm = is_qmark(ch, saleqm);

    // 判定 obj vnum 是否對應 商人 身上特定 qmark value
    if (is_qmark(keeper, "not_for_sale_1")) {
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_1")->v0) {
            // 判定 買家 身上是否有對應的 qmark value
            if (qm && qm->v0 & 1)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_1")->v1) {
            if (qm && qm->v0 & 2)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_1")->v2) {
            if (qm && qm->v0 & 4)
                return FALSE;
            else
                return TRUE;
        }
    }
    if (is_qmark(keeper, "not_for_sale_2")) {
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_2")->v0) {
            if (qm && qm->v0 & 8)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_2")->v1) {
            if (qm && qm->v0 & 16)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_2")->v2) {
            if (qm && qm->v0 & 32)
                return FALSE;
            else
                return TRUE;
        }
    }
    if (is_qmark(keeper, "not_for_sale_3")) {
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_3")->v0) {
            if (qm && qm->v0 & 64)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_3")->v1) {
            if (qm && qm->v0 & 128)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_3")->v2) {
            if (qm && qm->v0 & 256)
                return FALSE;
            else
                return TRUE;
        }
    }
    if (is_qmark(keeper, "not_for_sale_4")) {
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_4")->v0) {
            if (qm && qm->v0 & 512)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_4")->v1) {
            if (qm && qm->v0 & 1024)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_4")->v2) {
            if (qm && qm->v0 & 2048)
                return FALSE;
            else
                return TRUE;
        }
    }
    if (is_qmark(keeper, "not_for_sale_5")) {
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_5")->v0) {
            if (qm && qm->v0 & 4096)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_5")->v1) {
            if (qm && qm->v0 & 8192)
                return FALSE;
            else
                return TRUE;
        }
        if (obj->pIndexData->vnum == is_qmark(keeper, "not_for_sale_5")->v2) {
            if (qm && qm->v0 & 16384)
                return FALSE;
            else
                return TRUE;
        }
    }

    // 如果 obj vnum 沒對應該特定的 qmark 則不成立
    return FALSE;
}

/*
 * Multiple object buy modifications by Erwin Andreasen
 * Obtained from Canth's snippets page at:
 * http://www.xs4all.nl/~phule/snippets/snippets.html
 */
// do_buy: Command: do_buy - Performs the 'buy' command.
// do_buy: 指令：do_buy - 執行 'buy' 指令。
void do_buy(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    int type;
    QMARK *qmark;

    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg2);
    one_argument(argument, arg3);

    if (arg[0] == '\0') {
        send_to_char("要買什麼？\n\r", ch);
        return;
    }

    if (IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP)) {
        CHAR_DATA *pet;
        ROOM_INDEX_DATA *pRoomIndexNext;
        ROOM_INDEX_DATA *in_room;
        char buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
            return;

        pRoomIndexNext = get_room_index(ch->in_room->vnum + 1);

        if (!pRoomIndexNext) {
            bug("Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum);
            send_to_char("這裡沒有賣那樣東西.\n\r", ch);
            return;
        }

        in_room = ch->in_room;
        ch->in_room = pRoomIndexNext;
        pet = get_char_room(ch, arg);
        ch->in_room = in_room;

        if (!pet || !IS_SET(pet->act, ACT_PET)) {
            send_to_char("這裡沒有賣那樣東西.\n\r", ch);
            return;
        }
        /*
          if ( IS_SET( ch->act, PLR_BOUGHT_PET ) && !IS_IMMORTAL( ch ) )
          {
                  send_to_char( "你已經有一隻寵物了.\n\r", ch );
                  return;
          }
        */
        if (ch->gold < pet->level * pet->level * pet->level) {
            send_to_char("你買不起.\n\r", ch);
            return;
        }

        if (!can_see(ch, ch)) {
            send_to_char("你挑的寵物看不到你！\n\r", ch);
            return;
        }

        if (ch->level < pet->level) {
            send_to_char("你的等級不夠.\n\r", ch);
            return;
        }

        if ((qmark = ch->qmark))
            for (qmark = ch->qmark; qmark != NULL; qmark = qmark->next)
                if (!str_cmp(qmark->mark_name, "bought_pet_timer")) {
                    send_to_char(
                        "你不久前才剛買過一隻寵物，必須過一陣子才能再買。\n\r",
                        ch);
                    return;
                }

        ch->gold -= pet->level * pet->level * pet->level;
        pet = create_mobile(pet->pIndexData);
        pet->level = pet->pIndexData->level;
        //  SET_BIT( ch->act,    PLR_BOUGHT_PET );
        SET_BIT(pet->act, ACT_PET);
        SET_BIT(pet->affected_by, AFF_CHARM);
        pet->clan = ch->clan;

        one_argument(argument, arg);

        if (arg[0] != '\0') {
            sprintf(buf, "%s %s", pet->name, arg);
            free_string(pet->name);
            pet->name = str_dup(buf);
        }

        sprintf(buf, "%s項圈上寫著：「我的主人是 %s」.\n\r", pet->description,
                /*ch->clan ? ch->clan->name : */ ch->name);
        free_string(pet->description);
        pet->description = str_dup(buf);

        char_to_room(pet, ch->in_room);
        mprog_repop_trigger(pet);
        // if ( !ch->clan )
        add_follower(pet, ch);
        send_to_char("請好好善待你的寵物.\n\r", ch);
        act("$n 買下了一隻 $N.", ch, NULL, pet, TO_ROOM);

        sprintf(arg, "%s ", ch->name);
        strcat(arg, "bought_pet_timer 30 0 0 0 0 0");
        do_mpadd_qmark(pet, arg);
        return;
    } else if (IS_SET(ch->in_room->room_flags, ROOM_HORSE_SHOP)) {
        CHAR_DATA *horse;
        ROOM_INDEX_DATA *pRoomIndexNext;
        ROOM_INDEX_DATA *in_room; // temp for IN_ROOM
        char buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
            return;

        if (get_skill_percent(ch, gsn_ride) == 0) {
            send_to_char("你又不會騎馬！買坐騎做什麼！\n\r", ch);
            return;
        }

        pRoomIndexNext = get_room_index(ch->in_room->vnum + 1);
        if (!pRoomIndexNext) {
            bug("Do_buy: bad horse shop at vnum %d.", ch->in_room->vnum);
            send_to_char("這裡沒有賣這種坐騎.\n\r", ch);
            return;
        }

        in_room = ch->in_room;
        ch->in_room = pRoomIndexNext;
        horse = get_char_room(ch, arg);
        ch->in_room = in_room;

        if (!horse || !IS_SET(horse->act, ACT_HORSE)) {
            send_to_char("這裡沒有賣這種坐騎.\n\r", ch);
            return;
        }

        if (ch->gold < horse->level * horse->level * horse->level) {
            send_to_char("你買不起.\n\r", ch);
            return;
        }

        if (!can_see(horse, ch)) {
            send_to_char("你挑的坐騎看不到你！\n\r", ch);
            return;
        }

        if (ch->level < horse->level) {
            send_to_char("你的等級不夠.\n\r", ch);
            return;
        }

        if ((qmark = ch->qmark))
            for (qmark = ch->qmark; qmark != NULL; qmark = qmark->next)
                if (!str_cmp(qmark->mark_name, "bought_horse_timer")) {
                    send_to_char("你不是不久前才買過一匹馬嗎？\n\r", ch);
                    return;
                }

        ch->gold -= horse->level * horse->level * horse->level;
        horse = create_mobile(horse->pIndexData);
        horse->level = horse->pIndexData->level;
        //  SET_BIT( ch->act,    PLR_BOUGHT_PET );
        SET_BIT(horse->act, ACT_HORSE);
        SET_BIT(horse->act, ACT_SENTINEL);
        //  SET_BIT( horse->affected_by, AFF_CHARM    );

        one_argument(argument, arg);

        if (arg[0] != '\0') {
            sprintf(buf, "%s %s", horse->name, arg);
            free_string(horse->name);
            horse->name = str_dup(buf);
        }

        char_to_room(horse, ch->in_room);
        mprog_repop_trigger(horse);
        add_rember(horse, ch);
        // if ( !ch->clan )
        add_follower(horse, ch);
        send_to_char("請好好善待你的坐騎.\n\r", ch);
        act("$n 雇用了一隻 $N.", ch, NULL, horse, TO_ROOM);
        sprintf(arg, "%s ", ch->name);
        strcat(arg, "bought_horse_timer 30 0 0 0 0 1");
        do_mpadd_qmark(horse, arg);
        return;
    } else { // keeper 對話上色 2022/05/05
        OBJ_DATA *obj;
        CHAR_DATA *keeper;
        int cost;
        int item_count = 1; /* buy only one by default */

        if (is_number(arg)) {
            if (arg2[0] != '\0') {
                item_count = atoi(arg);
                strcpy(arg, arg2);
                strcpy(arg2, arg3);
            }
        }

        if (!(keeper = find_keeper(ch, arg2)))
            return;

        if (is_number(arg)) {
            int i = 1;
            type = atoi(arg);
            for (obj = keeper->carrying; obj; obj = obj->next_content, i++) {
                if (obj->wear_loc != WEAR_NONE ||
                    (cost = get_cost(keeper, obj, TRUE)) < 0 ||
                    not_for_sale(ch, keeper,
                                 obj)) // 新增 qmark 判定非賣品 2022/05/05
                {
                    --i;
                    continue;
                }
                if (i == type)
                    break;
            }
        } else {
            obj = get_obj_carry(keeper, arg);
        }

        cost = get_cost(keeper, obj, TRUE);

        if (cost <= 0 || !can_see_obj(ch, obj) ||
            not_for_sale(ch, keeper, obj)) // 新增 qmark 判定非賣品 2022/05/05
        {
            act("$n" HIG
                "告訴你「我沒有賣這樣東西，請用 'list' 看看我的貨物。」" NOR "",
                keeper, NULL, ch, TO_VICT);
            ch->reply = keeper;
            return;
        }

        if (item_count < 1) {
            send_to_char("要買多少？\n\r", ch);
            return;
        }

        if (ch->gold < (cost * item_count)) {
            if (item_count == 1) {
                act("$n" HIG "告訴你「你身上沒有足夠的錢。」" NOR "", keeper,
                    obj, ch, TO_VICT);
                ch->reply = keeper;
                return;
            } else {
                char buf[MAX_STRING_LENGTH];

                if ((ch->gold / cost) > 0)
                    sprintf(buf,
                            "$n" HIG "告訴你「你的錢只夠買 %d 個。」" NOR "",
                            (ch->gold / cost));
                else
                    sprintf(buf,
                            "$n" HIG "告訴你「你連一個也買不起。」" NOR "");

                act(buf, keeper, obj, ch, TO_VICT);
                ch->reply = keeper;
                return;
            }
        }

        if (obj->level > ch->level) {
            act("$n" HIG "告訴你「你還不能使用 $p。」" NOR "", keeper, obj, ch,
                TO_VICT);
            ch->reply = keeper;
            return;
        }

        if (ch->carry_number + (item_count * get_obj_number(obj)) >
            can_carry_n(ch)) {
            send_to_char("你無法再拿動更多東西了.\n\r", ch);
            return;
        }

        if (ch->carry_weight + (item_count * get_obj_weight(obj)) >
            can_carry_w(ch)) {
            send_to_char("你無法再拿動更多重量了.\n\r", ch);
            return;
        }

        if ((item_count > 1) && !IS_SET(obj->extra_flags, ITEM_INVENTORY)) {
            act("$n" HIG "告訴你「抱歉, $p 只剩下一個了。」" NOR "", keeper,
                obj, ch, TO_CHAR);
            ch->reply = keeper;
            return;
        }

        // Anti-duper by Chris
        if (obj && keeper->pIndexData->pShop &&
            keeper->pIndexData->pShop->profit_buy < 100) {
            bugf("Dupe by Shops : %s try to buy %s to mob %d for %d percents "
                 "cost!",
                 ch->name, obj->short_descr, keeper->pIndexData->vnum,
                 keeper->pIndexData->pShop->profit_buy);
        }

        if (item_count == 1) {
            act("你買下 $p.", ch, obj, NULL, TO_CHAR);
            act("$n 買下 $p.", ch, obj, NULL, TO_ROOM);
        } else {
            char buf[MAX_STRING_LENGTH];

            sprintf(buf, "$n 買下 %d * $p.", item_count);
            act(buf, ch, obj, NULL, TO_ROOM);
            sprintf(buf, "你買下 %d * $p.", item_count);
            act(buf, ch, obj, NULL, TO_CHAR);
        }

        ch->gold -= cost * item_count;

        if (IS_SET(obj->extra_flags, ITEM_INVENTORY))
            for (; item_count > 0; item_count--) {
                obj = fcreate_object(obj->pIndexData, obj->level);
                obj_to_char(obj, ch);
            }
        else {
            obj_from_char(obj);
            obj_to_char(obj, ch);
        }

        // 新增 obj_act buy 針對買物品的角色 2022/02/12
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("buy", ch, obj);
        }

        // obj 可能在 obj_act 時被 purge 2022/02/13
        if (!obj)
            return;

        // 新增 obj_act buy_target 針對被買物品的角色(npc 商人) 2022/02/12
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("buy_target", keeper, obj);
        }

        return;
    }
}

// do_list: Command: do_list - Performs the 'list' command.
// do_list: 指令：do_list - 執行 'list' 指令。
void do_list(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 4];
    int wear_filter = 0;
    // static const   WEAR_MAP wear_table[17]=
    static const WEAR_MAP wear_table[18] = {
        {"about", ITEM_WEAR_ABOUT}, {"arm", ITEM_WEAR_ARMS},
        {"arrow", ITEM_WEAR_ARROW}, {"body", ITEM_WEAR_BODY},
        {"feet", ITEM_WEAR_FEET},   {"finger", ITEM_WEAR_FINGER},
        {"hand", ITEM_WEAR_HANDS},  {"head", ITEM_WEAR_HEAD},
        {"hold", ITEM_HOLD},        {"leg", ITEM_WEAR_LEGS},
        {"neck", ITEM_WEAR_NECK},   {"shield", ITEM_WEAR_SHIELD},
        {"twohand", ITEM_TWO_HAND}, {"waist", ITEM_WEAR_WAIST},
        {"weapon", ITEM_WIELD},     {"wrist", ITEM_WEAR_WRIST},
        {"ammo", ITEM_WEAR_AMMO}, // 彈藥位置 2022/04/30
        {"ride", ITEM_WEAR_RIDE},
    };

    buf1[0] = '\0';

    if (IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP)) {
        CHAR_DATA *pet;
        ROOM_INDEX_DATA *pRoomIndexNext;
        bool found;

        pRoomIndexNext = get_room_index(ch->in_room->vnum + 1);

        if (!pRoomIndexNext) {
            bug("Do_list: bad pet shop at vnum %d.", ch->in_room->vnum);
            send_to_char("你無法這麼做 .\n\r", ch);
            return;
        }

        found = FALSE;

        for (pet = pRoomIndexNext->people; pet; pet = pet->next_in_room) {
            if (IS_SET(pet->act, ACT_PET)) {
                if (!found) {
                    found = TRUE;
                    strcat(buf1, "寵物出售:\n\r");
                }
                sprintf(buf, "[%2d] %8d - %s\n\r", pet->level,
                        pet->level * pet->level * pet->level, pet->short_descr);
                strcat(buf1, buf);
            }
        }
        if (!found)
            send_to_char("抱歉，本店目前缺貨.\n\r", ch);

        send_to_char(buf1, ch);
        return;
    } else if (IS_SET(ch->in_room->room_flags, ROOM_HORSE_SHOP)) {
        CHAR_DATA *horse;
        ROOM_INDEX_DATA *pRoomIndexNext;
        bool found;

        pRoomIndexNext = get_room_index(ch->in_room->vnum + 1);
        if (!pRoomIndexNext) {
            bug("Do_list: bad pet shop at vnum %d.", ch->in_room->vnum);
            send_to_char("你無法這麼做 .\n\r", ch);
            return;
        }

        found = FALSE;

        for (horse = pRoomIndexNext->people; horse;
             horse = horse->next_in_room) {
            if (IS_SET(horse->act, ACT_HORSE)) {
                if (!found) {
                    found = TRUE;
                    strcat(buf1, "坐騎出售:\n\r");
                }
                sprintf(buf, "[%2d] %8d - %s\n\r", horse->level,
                        horse->level * horse->level * horse->level,
                        horse->short_descr);
                strcat(buf1, buf);
            }
        }
        if (!found)
            send_to_char("抱歉，本店目前缺貨.\n\r", ch);

        send_to_char(buf1, ch);
        return;
    } else {
        OBJ_DATA *obj;
        CHAR_DATA *keeper;
        char arg[MAX_INPUT_LENGTH];
        char arg2[MAX_INPUT_LENGTH];
        int cost;
        bool found;
        int count = 0;
        int i;

        argument = one_argument(argument, arg);
        one_argument(argument, arg2);

        if (!(keeper = find_keeper(ch, arg2)))
            return;

        found = FALSE;
        if (arg[0] != '\0') {
            // add wear loc ammo 2022/04/30
            // for( i = 0; i < 16 ; ++i )
            for (i = 0; i < 17; ++i) {
                if (!str_cmp(arg, wear_table[i].name)) {
                    wear_filter = wear_table[i].place;
                    break;
                }
            }
        }
        for (i = 1, obj = keeper->carrying; obj; obj = obj->next_content, ++i) {
            // obj 被商人穿著 或是 賣價小於 0  則不會顯示在 list 中
            if (obj->wear_loc != WEAR_NONE ||
                (cost = get_cost(keeper, obj, TRUE)) < 0 ||
                not_for_sale(ch, keeper,
                             obj)) // 新增 qmark 判定非賣品 2022/05/05
            {
                --i;
                continue;
            }

            if (can_see_obj(ch, obj) &&
                (arg[0] == '\0' || !str_cmp(arg, "all") ||
                 (CAN_WEAR(obj, wear_filter)) || is_name(arg, obj->name))) {
                if (!found) {
                    found = TRUE;
                    strcat(buf1, "[編號   Lv  價 錢 ] 物 品 名 稱\n\r");
                }

                sprintf(buf, "[%4d   %2d %7d] %s.\n\r", i, obj->level, cost,
                        capitalize(obj->short_descr));
                ++count;
                strcat(buf1, buf);
            }

            if (count >= 35) {
                count = 0;
                send_to_char(buf1, ch);
                buf1[0] = '\0';
            }
        }

        if (!found) {
            if (arg[0] == '\0')
                send_to_char("這裡買不到東西.\n\r", ch);
            else
                send_to_char("他不賣那樣東西.\n\r", ch);
            return;
        }

        send_to_char(buf1, ch);
        return;
    }
}

// do_sell: Command: do_sell - Performs the 'sell' command.
// do_sell: 指令：do_sell - 執行 'sell' 指令。
void do_sell(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *keeper;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int cost;

    argument = one_argument(argument, arg);
    one_argument(argument, arg2);

    if (arg[0] == '\0') {
        send_to_char("要賣什麼?\n\r", ch);
        return;
    }

    if (!(keeper = find_keeper(ch, arg2)))
        return;

    if (!(obj = get_obj_carry(ch, arg))) {
        act("$n 告訴你, '你沒有那樣東西'.", keeper, NULL, ch, TO_VICT);
        ch->reply = keeper;
        return;
    }

    if (!can_drop_obj(ch, obj)) {
        send_to_char("你無法放下它.\n\r", ch);
        return;
    }

    if (!can_see_obj(keeper, obj)) {
        act("$n 告訴你, '我看不到你想賣的東西'.", keeper, NULL, ch, TO_VICT);
        ch->reply = keeper;
        return;
    }

    // Anti-duper by Chris
    if (obj && keeper->pIndexData->pShop &&
        keeper->pIndexData->pShop->profit_sell > 100) {
        bugf("Dupe by Shops : %s try to sell %s to mob %d for %d percents "
             "profit!",
             ch->name, obj->short_descr, keeper->pIndexData->vnum,
             keeper->pIndexData->pShop->profit_sell);
    }

    if ((cost = get_cost(keeper, obj, FALSE)) <= 0 ||
        obj->level > LEVEL_IMMORTAL || IS_SET(obj->extra_flags, ITEM_CLAN)) {
        act("$n 搖搖頭，看來對 $p 一點興趣也沒有.", keeper, obj, ch, TO_VICT);
        return;
    }

    if (IS_SET(obj->extra_flags, ITEM_POISONED)) {
        act("$n 告訴你, '我不買有毒的東西'", keeper, NULL, ch, TO_VICT);
        ch->reply = keeper;
        return;
    }

    sprintf(buf, "你以 %d 枚金幣的價錢賣掉了 $p.", cost);
    act(buf, ch, obj, NULL, TO_CHAR);
    act("$n 賣掉 $p.", ch, obj, NULL, TO_ROOM);
    ch->gold += cost;

    if (obj->item_type == ITEM_TRASH) {
        extract_obj(obj);
    } else {
        obj_from_char(obj);
        obj_to_char(obj, keeper);
    }

    // 新增 obj_act sell 針對賣物品的角色 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("sell", ch, obj);
    }

    // obj 可能在 obj_act 時被 purge 2022/02/13
    if (!obj)
        return;

    // 新增 obj_act sell_target 針對收購物品的角色(npc 商人) 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("sell_target", keeper, obj);
    }

    return;
}

// do_value: Command: do_value - Performs the 'value' command.
// do_value: 指令：do_value - 執行 'value' 指令。
void do_value(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *keeper;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int cost;

    argument = one_argument(argument, arg);
    one_argument(argument, arg2);

    if (arg[0] == '\0') {
        send_to_char("要為什麼估價?\n\r", ch);
        return;
    }

    if (!(keeper = find_keeper(ch, arg2)))
        return;

    if (!(obj = get_obj_carry(ch, arg))) {
        act("$n 告訴你, '你沒有那樣東西'.", keeper, NULL, ch, TO_VICT);
        ch->reply = keeper;
        return;
    }

    if (!can_drop_obj(ch, obj)) {
        send_to_char("你無法放下它.\n\r", ch);
        return;
    }

    if (!can_see_obj(keeper, obj)) {
        act("$n 告訴你, '我不曉得怎麼鑑定空氣'.", keeper, NULL, ch, TO_VICT);
        ch->reply = keeper;
        return;
    }

    if ((cost = get_cost(keeper, obj, FALSE)) <= 0) {
        act("$n 搖搖頭，看來對 $p 一點興趣也沒有.", keeper, obj, ch, TO_VICT);
        return;
    }

    if (IS_SET(obj->extra_flags, ITEM_POISONED)) {
        act("$n 告訴你, '我不買有毒的東西'", keeper, NULL, ch, TO_VICT);
        ch->reply = keeper;
        return;
    }

    sprintf(buf, "$n 告訴你, '我願意出 %d 枚金幣的價錢買下 $p'.", cost);
    act(buf, keeper, obj, ch, TO_VICT);
    ch->reply = keeper;

    return;
}

/* Poison weapon by Thelonius for EnvyMud */
// do_poison_weapon: Command: do_poison_weapon - Performs the 'poison_weapon' command.
// do_poison_weapon: 指令：do_poison_weapon - 執行 'poison_weapon' 指令。
void do_poison_weapon(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *pobj;
    OBJ_DATA *wobj;
    AFFECT_DATA *paf; // 2021/11/10
    char arg[MAX_INPUT_LENGTH];
    int sklv = get_skill_level(ch, gsn_poison_weapon);
    /* Don't allow mobs or unskilled pcs to do this */
    if (IS_NPC(ch) ||
        (!IS_NPC(ch) && get_skill_percent(ch, gsn_poison_weapon) == 0)) {
        send_to_char("什麼？\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("你要在什麼東西上抹毒？\n\r", ch);
        return;
    }
    if (ch->fighting) {
        send_to_char("你正在戰鬥，緩不出手來.\n\r", ch);
        return;
    }
    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你沒有這樣東西.\n\r", ch);
        return;
    }
    /*
    if ( obj->item_type != ITEM_WEAPON  )
    {
            send_to_char( "那不是武器.\n\r", ch );
            return;
    }
    */
    // 新增 arrow 及 dart 抹毒 2021/11/10
    if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARROW &&
        obj->item_type != ITEM_DART) {
        send_to_char("那件物品不能抹毒.\n\r", ch);
        return;
    }
    if (IS_OBJ_STAT(obj, ITEM_POISONED)) {
        send_to_char("那已經有毒了.\n\r", ch);
        return;
    }

    /* Now we have a valid weapon...check to see if we have the powder. */
    for (pobj = ch->carrying; pobj; pobj = pobj->next_content) {
        if (pobj->pIndexData->vnum == OBJ_VNUM_BLACK_POWDER)
            break;
    }
    if (!pobj) {
        send_to_char("你沒有毒粉.\n\r", ch);
        return;
    }

    /* Okay, we have the powder...do we have water? */
    for (wobj = ch->carrying; wobj; wobj = wobj->next_content) {
        // 避免使 特殊珍藏品 拿去燒掉
        if (IS_SET(wobj->pIndexData->souvenir, Z4))
            continue;
        if (wobj->item_type == ITEM_DRINK_CON && wobj->value[1] > 0 &&
            wobj->value[2] == 0)
            break;
    }
    if (!wobj) {
        send_to_char("你得要有水來混合毒粉.\n\r", ch);
        return;
    }

    /* Great, we have the ingredients...but is the thief smart enough? */
    if (!IS_NPC(ch) && get_curr_wis(ch) + sklv / 2 < 11 &&
        get_curr_int(ch) + sklv / 2 < 11) {
        send_to_char("你笨手笨腳的，結果失敗了...\n\r", ch);
        return;
    }
    /* And does the thief have steady enough hands? */
    if (!IS_NPC(ch) && (get_curr_dex(ch) + sklv / 2 < 17 ||
                        ch->pcdata->condition[COND_DRUNK] > 0)) {
        send_to_char("你一個不小心，把作好的毒液給打翻了！\n\r", ch);
        return;
    }

    WAIT_STATE(ch, skill_table[gsn_poison_weapon].beats);

    /* Check the skill percentage */
    if (!IS_NPC(ch)
        //&& number_percent( ) > ch->pcdata->learned[gsn_poison_weapon] )
        && number_percent() >
               sklv * 2 + get_skill_percent(ch, gsn_poison_weapon)) {
        send_to_char("你一個不小心，把毒液濺到了自己身上！\n\r", ch);
        // damage( ch, ch, ch->level, gsn_poison_weapon, WEAR_NONE );
        damage(ch, ch, ch->level, gsn_poison_weapon, WEAR_NONE, MSG_NORMAL,
               DMG_NORMAL);
        act("$n 把毒液濺得到處都是！", ch, NULL, NULL, TO_ROOM);
        extract_obj(pobj);
        extract_obj(wobj);
        return;
    }

    /* Well, I'm tired of waiting.  Are you? */
    act("你把 $p 倒進 $P, 製成毒液。", ch, pobj, wobj, TO_CHAR);
    act("$n 把 $p 倒進 $P, 製成了毒液。", ch, pobj, wobj, TO_ROOM);
    act("你把毒液均勻地抹在 $p 上。", ch, obj, NULL, TO_CHAR);
    act("$n 把毒液均勻地抹在 $p上。", ch, obj, NULL, TO_ROOM);

    /*  拔除 timer 設定及 item_poison_weapon
    if( obj->timer <= 0 )
            SET_BIT( obj->extra_flags, ITEM_POISONED_WEAPON );
    if( obj->timer < 0 )
    {
            obj->timer = 40 + ch->level + sklv * sklv * 8;

            // obj->cost *= ch->level;

            // Set an object timer.  Dont want proliferation of poisoned weapons
            //obj->timer = 40 + ch->level + sklv * sklv * 8;

            if ( IS_OBJ_STAT( obj, ITEM_BLESS ) )
                    obj->timer = obj->timer + ( obj->timer * ( sklv / 2 ) / 10
    );

            if ( IS_OBJ_STAT( obj, ITEM_MAGIC ) )
                    obj->timer = UMAX( 20, obj->timer - 60 );

            // WHAT?  All of that, just for that one bit?  How lame. ;)
    }
    */

    SET_BIT(obj->extra_flags, ITEM_POISONED);
    // 加入抹毒物品的毒性判定 2021/11/11
    paf = new_affect();
    paf->type = obj->pIndexData->vnum;
    paf->duration = 5 + sklv * sklv;
    paf->location = APPLY_TOXICITY_POISON;
    paf->modifier = pobj->level * (1 + sklv / 6);
    paf->bitvector = 0;
    paf->next = obj->affected;
    obj->affected = paf;

    act("剩下的毒液把 $p 給腐蝕了.", ch, wobj, NULL, TO_CHAR);
    act("剩下的毒液把 $p 給腐蝕了.", ch, wobj, NULL, TO_ROOM);
    extract_obj(pobj);
    extract_obj(wobj);

    return;
}

/* Contributed by BoneCrusher of EnvyMud. */
// do_donate: Command: do_donate - Performs the 'donate' command.
// do_donate: 指令：do_donate - 執行 'donate' 指令。
void do_donate(CHAR_DATA *ch, char *arg) {
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char arg1[MAX_INPUT_LENGTH];

    arg = one_argument(arg, arg1);

    if (arg1[0] == '\0') {
        send_to_char("你要捐獻什麼？\n\r", ch);
        return;
    }

    if (!ch->in_room)
        return;

    /*
                if ( ( container = get_obj_world( ch, "donation" ) ) == NULL )
                {
                        send_to_char( "這個世界上沒有奉獻箱.\n\r", ch );
                        return;
                }
        */

    container = NULL;
    for (container = object_list; container; container = container->next) {
        if (IS_SET(container->extra_flags, ITEM_DONATION_PIT)) {
            if (!container->in_room ||
                container->in_room->area->recall != ch->in_room->area->recall)
                continue;
            else
                break;
        }
    }
    if (!container) {
        send_to_char("這個世界上沒有奉獻箱。\n\r", ch);
        return;
    }
    if (get_obj_number(container) > 100) {
        send_to_char("奉獻箱滿了。\n\r", ch);
        return;
    }

    if (str_cmp(arg1, "all") && str_prefix("all.", arg1)) {
        if ((obj = get_obj_carry(ch, arg1)) == NULL) {
            send_to_char("你沒有這樣東西.\n\r", ch);
            return;
        }

        if (!can_drop_obj(ch, obj) || IS_SET(obj->extra_flags, ITEM_NOSAC)) {
            send_to_char("你無法拋開這樣東西.\n\r", ch);
            return;
        }

        if (get_obj_weight(obj) + get_obj_weight(container) >
            container->value[0]) {
            send_to_char("這樣東西裝不進奉獻箱.\n\r", ch);
            return;
        }

        act("你奉獻 $p 進 $P.", ch, obj, container, TO_CHAR);

        if (obj->item_type == ITEM_TRASH || obj->item_type == ITEM_FOOD ||
            obj->item_type == ITEM_KEY || obj->item_type == ITEM_PILL) {
            // 新增 obj_act donate 2022/02/12
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("donate", ch, obj);
            }

            // obj 可能在 obj_act 時被 purge 2022/02/13
            if (!obj) {
                save_char_obj(ch);
                return;
            }

            // act( "你奉獻 $p 進 $P.", ch, obj, container, TO_CHAR );
            extract_obj(obj);
            return;
        }

        act("$n 奉獻 $p 進 $P.", ch, obj, container, TO_ROOM);
        send_to_room("奉獻箱裡傳來一陣響聲！", container->in_room);
        obj_from_char(obj);
        obj_to_obj(obj, container);
        // 新增 obj_act donate 2022/02/12

        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("donate", ch, obj);
        }

        // obj 可能在 obj_act 時被 purge 2022/02/13
        if (!obj) {
            save_char_obj(ch);
            return;
        }

        // 敘述移至 obj 從 ch 身上移除前 2022/02/12
        // act( "$n 奉獻 $p 進 $P.", ch, obj, container, TO_ROOM );
        // act( "你奉獻 $p 進 $P.", ch, obj, container, TO_CHAR );
        // send_to_room( "奉獻箱裡傳來一陣響聲！", container->in_room );
    } else {
        for (obj = ch->carrying; obj; obj = obj_next) {
            obj_next = obj->next_content;

            if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name)) &&
                can_see_obj(ch, obj) && obj->wear_loc == WEAR_NONE &&
                obj != container && can_drop_obj(ch, obj) &&
                get_obj_weight(obj) + get_obj_weight(container) <=
                    container->value[0]) {
                act("你奉獻 $p 進 $P.", ch, obj, container, TO_CHAR);

                if (obj->item_type == ITEM_TRASH ||
                    obj->item_type == ITEM_FOOD || obj->item_type == ITEM_KEY ||
                    obj->item_type == ITEM_PILL) {
                    // 新增 obj_act donate 2022/02/12
                    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                        oprog_act_trigger("donate", ch, obj);
                    }

                    // obj 可能在 obj_act 時被 purge 2022/02/13
                    if (!obj)
                        continue;

                    // act( "你奉獻 $p 進 $P.", ch, obj, container, TO_CHAR );
                    extract_obj(obj);
                    continue;
                }

                act("$n 奉獻 $p 進 $P.", ch, obj, container, TO_ROOM);
                send_to_room("奉獻箱裡傳來一陣響聲！", container->in_room);
                obj_from_char(obj);
                obj_to_obj(obj, container);

                // 新增 obj_act donate 2022/02/12
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("donate", ch, obj);
                }

                // obj 可能在 obj_act 時被 purge 2022/02/13
                if (!obj)
                    continue;

                // 敘述移至 obj 從 ch 身上移除前 2022/02/12
                // act( "$n 奉獻 $p 進 $P.", ch, obj, container, TO_ROOM );
                // act( "你奉獻 $p 進 $P.", ch, obj, container, TO_CHAR );
                // send_to_room( "奉獻箱裡傳來一陣聲響！\n\r",
                // container->in_room );
            }
        }
    }

    save_char_obj(ch);
    return;
}

// do_register: Command: do_register - Performs the 'register' command.
// do_register: 指令：do_register - 執行 'register' 指令。
void do_register(CHAR_DATA *ch, char *arg) {
    CHAR_DATA *mob;
    int cost = ch->level * 5000;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    mob = NULL;
    for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
        if (IS_NPC(mob) && mob->pIndexData->vnum == MOB_VNUM_SECRETARY)
            break;
    }

    if (!mob) {
        send_to_char("這裡不能進行 PK 登記.\n\r", ch);
        return;
    }

    if (ch->level < 25) {
        do_say(mob, "你的等級還太低了.");
        return;
    }

    /*if ( ch->level > LEVEL_HERO + 6 )
    {
            do_say( mob, "神不可以參加 PK." );
            return;
    } */

    if (IS_SET(ch->act, PLR_REGISTER)) {
        if (ch->gold < cost * 5) {
            sprintf(buf, "你沒有足夠的錢來退出 PK.");
            do_say(mob, buf);
            return;
        }
        sprintf(buf, "你付出了 %d 枚金幣，退出 PK.", cost * 5);
        // do_say( mob, buf );
        send_to_char(buf, ch);
        ch->gold -= 5 * cost;
        REMOVE_BIT(ch->act, PLR_REGISTER);
        return;
    }

    if (ch->gold < cost) {
        sprintf(buf, "你沒有足夠的錢來付登記費.");
        do_say(mob, buf);
        return;
    }

    sprintf(buf, "你付出了 %d 枚金幣，登記參加 PK.", cost);
    // do_say( mob, buf );
    send_to_char(buf, ch);

    ch->gold -= cost;

    SET_BIT(ch->act, PLR_REGISTER);

    return;
}

// do_deposit: Command: do_deposit - Performs the 'deposit' command.
// do_deposit: 指令：do_deposit - 執行 'deposit' 指令。
void do_deposit(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *vch;
    char arg[MAX_INPUT_LENGTH];
    int num;

    for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
        if (IS_NPC(vch) &&
            IS_SET(vch->act, ACT_BANKER)) // 加入 npc 限定 , 必免 plr_thief
                                          // 也會被當 banker 2022/11/27
            break;

    one_argument(argument, arg);
    if (is_number(arg))
        num = atoi(arg);
    else {
        send_to_char("請明確說出一個數字。\n\r", ch);
        return;
    }

    if (num < 1) {
        send_to_char("請至少存一元。\n\r", ch);
        return;
    }

    if (vch) {
        if (num <= ch->gold) {
            if (num > 2147483647 - ch->bank) { // MAXIMUM Integer 2147483647
                send_to_char("你的戶頭存不了這麼多錢。\n\r", ch);
                return;
            }
            ch->gold -= num;
            ch->bank += num;
            act("$N向你一鞠躬說：謝謝您的光臨，大陸連鎖銀行永遠竭誠為您服務。",
                ch, NULL, vch, TO_CHAR);
            act("$N向$n一鞠躬說：謝謝您的光臨，大陸連鎖銀行永遠竭誠為您服務。",
                ch, NULL, vch, TO_ROOM);
            return;
        } else {
            send_to_char("喔！你身上沒那麼多錢。\n\r", ch);
            return;
        }
    } else
        send_to_char("你找不到人幫你存錢。\n\r", ch);
    return;
}

// do_withdraw: Command: do_withdraw - Performs the 'withdraw' command.
// do_withdraw: 指令：do_withdraw - 執行 'withdraw' 指令。
void do_withdraw(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *vch;
    char arg[MAX_INPUT_LENGTH];
    int num;

    for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
        if (IS_NPC(vch) &&
            IS_SET(vch->act, ACT_BANKER)) // 加入 npc 限定 , 必免 plr_thief
                                          // 也會被當 banker 2022/11/27
            break;

    one_argument(argument, arg);
    if (is_number(arg))
        num = atoi(arg);
    else {
        send_to_char("請明確說出一個數字。\n\r", ch);
        return;
    }
    if (num < 1) {
        send_to_char("請至少領一元。\n\r", ch);
        return;
    }

    if (vch) {
        if (num <= ch->bank) {
            if (num > 2147483647 - ch->gold) { // MAXIMUM Integer 2147483647
                send_to_char("你的身上帶不了這麼多錢。\n\r", ch);
                return;
            }
            ch->gold += num;
            ch->bank -= num;
            act("$N向你一鞠躬說：這是您的金幣，請點收。謝謝您的光臨。", ch,
                NULL, vch, TO_CHAR);
            act("$N向$n一鞠躬說：這是您的金幣，請點收。謝謝您的光臨。", ch,
                NULL, vch, TO_ROOM);
            return;
        } else {
            send_to_char("搶劫啊？你哪來那麼多錢？\n\r", ch);
            return;
        }
    } else
        send_to_char("你找不到人幫你提錢。\n\r", ch);
    return;
}

// do_store: Command: do_store - Performs the 'store' command.
// do_store: 指令：do_store - 執行 'store' 指令。
void do_store(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *vobj;
    CHAR_DATA *vch;
    char buf[10];
    int cost;
    bool fBank = TRUE;
    int number = 0;

    if (IS_NPC(ch))
        return;

    for (vch = ch->in_room->people; vch; vch = vch->next_in_room) {
        // 加入 npc 限定, 並修改 eqbanker 的判定由 name 變更為 qmark 2022/12/01
        if (!IS_NPC(vch))
            continue;
        // if ( is_name( "sharas-eqb", vch->name ) )
        if (is_qmark(vch, "is_eqbanker"))
            break;
    }
    if (!vch) {
        send_to_char("裝備銀行老闆不在這兒。\n\r", ch);
        return;
    }

    if (ch->in_room->area->recall != ROOM_VNUM_TEMPLE) {
        send_to_char("只有史卡拉貝城可以使用舊裝備銀行。\n\r", ch);
        return;
    }

    if (argument[0] == '\0') {
        send_to_char(
            "裝備銀行老闆皺著眉說：「您不說要存什麼，我也幫不上忙。」\n\r", ch);
        return;
    }

    for (obj = ch->eqbank; obj; obj = obj->next_content) {
        ++number;
    }

    if (number >= 50) {
        send_to_char("裝備銀行老闆皺著眉說：「抱歉，您的戶頭已經滿了，開個新的"
                     "倉庫吧。」\n\r",
                     ch);
        return;
    }

    if ((obj = get_obj_carry(ch, argument))) {
        cost = (obj->level < 10)   ? 10000
               : (obj->level < 36) ? obj->level * 200
                                   : obj->level * 500;
        if (ch->bank < cost) {
            fBank = FALSE;
            if (ch->gold < cost) {
                send_to_char("裝備銀行老闆苦笑著說：「對不起，沒有足夠的保管費"
                             "我無法雇用保全人員。」\n\r",
                             ch);
                return;
            }
        }
        if ((ch->level < (obj->level - 5) && ch->class < 5) ||
            obj->item_type == ITEM_KEY ||
            IS_SET(obj->extra_flags, ITEM_NOSAVE) ||
            IS_SET(obj->extra_flags, ITEM_UNIQUE) || obj->deleted ||
            obj->item_type == ITEM_CONTAINER || obj->timer > 0) {
            send_to_char(
                "裝備銀行老闆聳聳肩說：「對不起，這類裝備我們不收。」\n\r", ch);
            return;
        }
        obj_from_char(obj);
        if (obj == object_list) {
            object_list = obj->next;
            obj->next = NULL;
        } else
            for (vobj = object_list; vobj; vobj = vobj->next) {
                if (obj == vobj->next) {
                    vobj->next = obj->next;
                    obj->next = NULL;
                    break;
                }
            }
        obj->next_content = ch->eqbank;
        ch->eqbank = obj;
        if (fBank)
            ch->bank -= cost;
        else
            ch->gold -= cost;
        // 移除 to_room 避免大量存取物品造成他人畫面凌亂 2022/02/01
        // act( "裝備銀行老闆小心地將$n的$p拿到後面的保險庫存放。", ch, obj,
        // NULL, TO_ROOM );
        act("裝備銀行老闆小心地將你的$p拿到後面的保險庫存放。", ch, obj, NULL,
            TO_CHAR);
        if (fBank)
            send_to_char("你從銀行轉帳，付出了 " HIY, ch);
        else
            send_to_char("你付出了 " HIY, ch);
        sprintf(buf, "%d", cost);
        send_to_char(buf, ch);
        send_to_char(NOR " 元的保管費。\n\r", ch);
        do_save(ch, "");
        return;
    }
    send_to_char("裝備銀行老闆狐疑地看著你說：「我沒看到這項裝備啊？」\n\r",
                 ch);
}

// do_receive: Command: do_receive - Performs the 'receive' command.
// do_receive: 指令：do_receive - 執行 'receive' 指令。
void do_receive(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *vch;
    OBJ_DATA *obj = NULL;
    OBJ_DATA *vobj;
    // int number;
    int i = 0;
    int count = 0;
    // char  arg [ MAX_INPUT_LENGTH ];
    char buf[MAX_STRING_LENGTH];
    char no_ansi[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    // number = number_argument( argument, arg );
    // count = atoi(argument);
    for (vch = ch->in_room->people; vch; vch = vch->next_in_room) {
        // 加入 npc 限定, 並修改 eqbanker 的判定由 name 變更為 qmark 2022/12/01
        if (!IS_NPC(vch))
            continue;
        // if ( is_name( "sharas-eqb", vch->name ) )
        if (is_qmark(vch, "is_eqbanker"))
            break;
    }
    if (!vch) {
        send_to_char("裝備銀行老闆不在這兒。\n\r", ch);
        return;
    }

    if (ch->in_room->area->recall != ROOM_VNUM_TEMPLE) {
        send_to_char("只有史卡拉貝城可以使用舊裝備銀行。\n\r", ch);
        return;
    }

    if (argument[0] == '\0') {
        send_to_char("你目前存在裝備銀行保險庫的裝備有：\n\r", ch);
        if (!ch->eqbank) {
            send_to_char("\n\r什麼都沒有！\n\r\n\r", ch);
            return;
        }

        send_to_char("【編號】  物  品  名  稱                                 "
                     " 等級\n\r",
                     ch);
        for (vobj = ch->eqbank; vobj; vobj = vobj->next_content) {
            i++;
            strip_ansi(vobj->short_descr, no_ansi);
            sprintf(buf, "【%4d】%-48s  [%2d] \n\r", i,
                    can_see_obj(ch, vobj) ? no_ansi : "不明物品", vobj->level);
            send_to_char(buf, ch);
            // act( "$p", ch, vobj, NULL, TO_CHAR );
        }
        send_to_char("\n\r", ch);
        return;
    }
    if (!is_number(argument)) {
        send_to_char("請用 'receive 編號' 來領取您的裝備。\n\r", ch);
        return;
    }
    count = atoi(argument);

    if (!ch->eqbank) {
        send_to_char(
            "裝備銀行老闆狐疑地看著你說：「您的倉庫裡沒有東西啊？」\n\r", ch);
        return;
    }

    i = 2;
    if (ch->eqbank && count == 1) {
        obj = ch->eqbank;
        if (ch->carry_number + get_obj_number(obj) > can_carry_n(ch)) {
            do_say(vch, "對不起，您拿不動更多東西了。");
            return;
        }

        if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch)) {
            do_say(vch, "對不起，您的負重不夠了。");
            return;
        }
        ch->eqbank = obj->next_content;
        obj->next_content = NULL;
    } else if (ch->eqbank && ch->eqbank->next_content) {
        for (vobj = ch->eqbank; vobj->next_content; vobj = vobj->next_content) {

            // sprintf(buf, "[%d][%d]\n\r", count, i );
            // send_to_char(buf, ch);
            if (count == i) {
                obj = vobj->next_content;
                if (ch->carry_number + get_obj_number(obj) > can_carry_n(ch)) {
                    do_say(vch, "對不起，您拿不動更多東西了。");
                    return;
                }

                if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch)) {
                    do_say(vch, "對不起，您的負重不夠了。");
                    return;
                }
                vobj->next_content = obj->next_content;
                obj->next_content = NULL;

                break;
            }
            i++;
        }
    }
    /*
    if ( ch->eqbank && is_pfxname( arg, ch->eqbank->name ) )
    {
    obj = ch->eqbank;
    ch->eqbank = obj->next_content;
    }
    else if ( ch->eqbank && ch->eqbank->next_content )
    for ( vobj = ch->eqbank; vobj->next_content; vobj = vobj->next_content )
    if ( is_pfxname( arg, vobj->next_content->name ) )
    {
    if ( ++count == number )
    {
    obj = vobj->next_content;
    vobj->next_content = obj->next_content;
    break;
    }
    }
    */
    if (obj) {
        obj_to_char(obj, ch);
        obj->next = object_list;
        object_list = obj;
        // 移除 to_room 避免大量存取物品造成他人畫面凌亂 2022/02/01
        act("裝備銀行老闆自後面的庫房將$p捧來交給你。", ch, obj, NULL, TO_CHAR);
        // act( "裝備銀行老闆自後面的庫房將$p捧來交給$n。", ch, obj, NULL,
        // TO_ROOM ); act(
        // "裝備銀行老闆一鞠躬說：「請您檢查有無任何損傷，謝謝光臨。」", ch,
        // obj, NULL, TO_ROOM );
        act("裝備銀行老闆一鞠躬說：「請您檢查有無任何損傷，謝謝光臨。」", ch,
            obj, NULL, TO_CHAR);
        do_save(ch, "");
        return;
    }
    send_to_char(
        "裝備銀行老闆查閱了櫃臺上的登記簿說：奇怪，沒有這項裝備的紀錄喔！\n\r",
        ch);
}

// do_destroy: Command: do_destroy - Performs the 'destroy' command.
// do_destroy: 指令：do_destroy - 執行 'destroy' 指令。
void do_destroy(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("請輸入 destroy <物品>\n\r", ch);
        return;
    }

    if (!(obj = get_obj_carry(ch, arg))) {
        send_to_char("你沒有那樣東西.\n\r", ch);
        return;
    }

    if (!IS_SET(obj->extra_flags, ITEM_PERSONAL)) {
        send_to_char("你無法銷毀這樣東西.\n\r", ch);
        return;
    }
    act("你毀去 $p.", ch, obj, NULL, TO_CHAR);
    act("$n 毀去 $p.", ch, obj, NULL, TO_ROOM);
    // 新增 obj_act destroy 2022/02/12
    if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
        oprog_act_trigger("destroy", ch, obj);
    }
    obj_from_char(obj);

    // obj 可能在 prog 中被 purge 2022/02/13
    // if(obj)
    if (obj->deleted)
        ;
    else
        extract_obj(obj);
}

/*
// do_consent: Command: do_consent - Performs the 'consent' command.
// do_consent: 指令：do_consent - 執行 'consent' 指令。
void do_consent( CHAR_DATA *ch, char* argument)
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char qm[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *old_in = NULL;

    one_argument( argument, arg);

    if( IS_NPC(ch) ) return;
    if( arg[0] == '\0' ) {
        send_to_char("你要授權給誰？\n\r", ch);
        return;
    }
    if( !(victim = get_char_world( ch, arg) )
    ||  IS_NPC(victim) ){
        send_to_char("這裡沒有這個人。\n\r", ch);
        return;
    }

    if( victim == ch )
  {
    send_to_char("你本來就可以自己撿自己的屍體了！\n\r",ch);
    return;
  }

    one_argument(ch->name, arg1);
    sprintf(qm, "consent_%s", arg1);
    if( is_qmark(victim, qm) ) {
        if( ch->in_room != victim->in_room ) {
            old_in = ch->in_room;
            char_from_room(ch);
            char_to_room(ch, victim->in_room);
        }
        sprintf(buf,"'%s' %s ", arg, qm);
        do_del_qmark(ch, buf);
        if( old_in ) {
            char_from_room(ch);
            char_to_room(ch, old_in);
        }
        act("你收回對$N的授權。", ch, NULL, victim, TO_CHAR);
        act("$n不再授權你撿取$s屍體。", ch, NULL, victim, TO_VICT);
        return;
    }

    sprintf(buf,"'%s' %s -1 0 0 0 0 0", arg, qm);
    if( ch->in_room != victim->in_room ) {
        old_in = ch->in_room;
        char_from_room(ch);
        char_to_room(ch, victim->in_room);
    }
    do_add_qmark(ch, buf);
    if( old_in ) {
        char_from_room(ch);
        char_to_room(ch, old_in);
    }
    act("你授權$N撿取你的屍體。", ch, NULL, victim, TO_CHAR);
    act("$n授權你撿取$s屍體。", ch, NULL, victim, TO_VICT);
}
*/
// do_bandage: Command: do_bandage - Performs the 'bandage' command.
// do_bandage: 指令：do_bandage - 執行 'bandage' 指令。
void do_bandage(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *pobj;
    CHAR_DATA *vch;
    int factor = 0;
    int sklv = get_skill_level(ch, gsn_bandage);

    if (IS_NPC(ch) ||
        (!IS_NPC(ch) && get_skill_percent(ch, gsn_bandage) == 0)) {
        send_to_char("什麼？\n\r", ch);
        return;
    }

    if (ch->fighting) {
        send_to_char("你正在戰鬥，緩不出手來.\n\r", ch);
        return;
    }
    one_argument(argument, arg);

    if (arg[0] == '\0')
        vch = ch;
    else if (!(vch = get_char_room(ch, arg))) {
        send_to_char("這裡沒有這個人。\n\r", ch);
        return;
    }
    if (vch->fighting) {
        send_to_char("對方正在戰鬥，你沒辦法仔細的包紮。\n\r", ch);
        return;
    }

    for (pobj = ch->carrying; pobj; pobj = pobj->next_content) {
        if (pobj->pIndexData->vnum == OBJ_VNUM_BANDAGE)
            break;
    }
    if (!pobj) {
        send_to_char("你身上沒有繃帶.\n\r", ch);
        return;
    }

    extract_obj(pobj);

    if (ch != vch) {
        act("你細心的為$N包紮傷勢.", ch, NULL, vch, TO_CHAR);
        act("$n細心的為$N包紮傷勢.", ch, NULL, vch, TO_NOTVICT);
        act("$n細心的為你包紮傷勢.", ch, NULL, vch, TO_VICT);
        WAIT_STATE(vch, skill_table[gsn_bandage].beats);
        WAIT_STATE(ch, skill_table[gsn_bandage].beats);
    }

    factor = UMAX(ch->level / 6 + ch->level / 5, 5);
    factor += sklv;
    if (ch->pcdata->prime_attr == APPLY_WIS) {
        int extra;
        extra = number_bits(2);
        factor += sklv * 2;
        switch (extra) {
        case 0:
            factor += 5 + sklv;
            break;
        case 1:
            factor += sklv * 2;
            break;
        case 2:
            if (is_affected(vch, gsn_poison)) {
                act("一陣清新的感覺充滿你全身.", ch, NULL, vch, TO_VICT);
            }
            break;
        case 3:
            vch->move =
                UMIN(vch->move + sklv * 4 + (sklv / 3) * 10, vch->max_move);
            break;
        };
    }
    int nHeal = (vch->max_hit * factor) / (114 - sklv * 4);
    nHeal *= PPL_HP_MULTIPLYER;
    vch->hit = UMIN(vch->hit + nHeal, vch->max_hit);
    ch->move = UMAX(0, ch->move - (10 + factor / 10));
    send_to_char("好的。\n\r", ch);

    WAIT_STATE(ch, skill_table[gsn_bandage].beats);
}
