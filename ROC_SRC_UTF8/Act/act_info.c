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
//*  PROGRAM ID  : act_info.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940607 jye                                    *
//*  PURPOSE     :         將practice之+上色                      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         修改lv44以上看不到clan_channel之bug    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         修改lv45以上才可使用tell【竊聽】功能   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         修改who imm時trust 43也會出現          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         修改tell【竊聽】增加all 的功能         *
//*              :             tell【竊聽】增加close 的功能       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 jye                                    *
//*  PURPOSE     :         修改who 上下限之bug及增加who可查詢race *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         修改判斷souvenir值使改用變數           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         新增EXP_PER_LEVEL ppl_class變數        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050214 jye                                    *
//*  PURPOSE     :         修改do_exits顯示字型"–"改用"－"       *
//*****************************************************************
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#if defined(unix)
#include <unistd.h>
#endif
#endif
#include "ansi.h"
#include "merc.h"
#include "olc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
extern bool IsSorGateMob(int nMobIndex);
extern bool IsUnderFistFight(CHAR_DATA *ch);
bool can_scout_check(CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room,
                     int sklv);

extern const struct social_type social_table[];
char *const where_name[] = {
    /*  "<used as light>     ",
        "<worn on finger>  ",
        "<worn on finger>  ",
        "<worn around neck>  ",
        "<worn around neck>  ",
        "<worn on body>  ",
        "<worn on head>  ",
        "<worn on legs>  ",
        "<worn on feet>  ",
        "<worn on hands>   ",
        "<worn on arms>  ",
        "<worn as shield>  ",
        "<worn about body>   ",
        "<worn about waist>  ",
        "<worn around wrist> ",
        "<worn around wrist> ",
        "<wielded, primary>  ",
        "<held>    ",
        "<wielded, second>   " */
    "< 光  源 > ",
    "< 手  指 > ",
    "< 手  指 > ",
    "< 頸  部 > ",
    "< 頸  部 > ",
    "< 身  體 > ",
    "< 頭  部 > ",
    "< 腿  部 > ",
    "< 腳  部 > ",
    "< 手  部 > ",
    "< 手  臂 > ",
    "< 盾  牌 > ",
    "<圍繞身體> ",
    "< 腰  部 > ",
    "< 腕  部 > ",
    "< 腕  部 > ",
    "<主要武器> ",
    "< 手  握 > ",
    "<次要武器> ",
    "<雙手武器> ",
    "< 背  掛 > ",
    "", //  MAX_WEAR
    "< 觸  手 > ",
    "< 觸  手 > ",
    "< 騎  乘 > ",
    "<次 手 臂> ",
    "<次 手 部> ",
    "<次 腿 部> ",
    "<次 腳 部> ",
    "< 彈  藥 > " // wear loc ammo add at 2022/04/30
};

TOP10_DATA tmudage[TOP_COUNT + 1], texp[TOP_COUNT + 1], tgold[TOP_COUNT + 1],
    thp[TOP_COUNT + 1], t_mp[TOP_COUNT + 1], tmv[TOP_COUNT + 1],
    tpk[TOP_COUNT + 1], tresp[TOP_COUNT + 1], tmerit[TOP_COUNT + 1];
/*
 * Local functions.
 */
char *format_obj_to_char args((OBJ_DATA * obj, CHAR_DATA *ch, bool fShort));
void show_list_to_char args((OBJ_DATA * list, CHAR_DATA *ch, bool fShort,
                             bool fShowNothing));
void show_char_to_char_0 args((CHAR_DATA * victim, CHAR_DATA *ch));
void show_char_to_char_1 args((CHAR_DATA * victim, CHAR_DATA *ch));
void show_char_to_char args((CHAR_DATA * list, CHAR_DATA *ch));
int is_skname args((const char *str, char *namelist));
int class_lookup args((const char *name));
int str_length args((char *string));

char *skill_exp_desc(int exp);

char *format_obj_to_char_b(OBJ_DATA *obj, CHAR_DATA *ch, bool fShort) {
    static char buf[BUF_STRING_LENGTH];

    buf[0] = '\0';

    if (IS_IMMORTAL(ch))
        sprintf(buf, "[" HIG "%5d" NOR "]", obj->pIndexData->vnum);
    if (IS_SET(obj->pIndexData->souvenir, Z1))
        strcat(buf, "(" HIW "紀" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_INVIS))
        strcat(buf, "(" YEL "隱" NOR ")");
    if ((IS_AFFECTED(ch, AFF_DETECT_EVIL) ||
         (IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_ALIGN) &&
          !IS_EVIL(ch))) &&
        IS_OBJ_STAT(obj, ITEM_EVIL))
        strcat(buf, "(" HIR "邪" NOR ")");
    if ((IS_AFFECTED(ch, AFF_DETECT_MAGIC) ||
         IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_MAGIC)) &&
        IS_OBJ_STAT(obj, ITEM_MAGIC))
        strcat(buf, "(" HIC "魔" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_GLOW))
        strcat(buf, "(" HIY "光" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_HUM))
        strcat(buf, "(" HIP "鳴" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_POISONED))
        strcat(buf, "(" HIG "毒" NOR ")");
    if (!indestructible(obj) && (get_curr_dura(obj) * 3 < get_max_dura(obj)))
        strcat(buf, "(" HIP "損" NOR ")");
    if (is_broken(obj))
        strcat(buf, "(" HIB "壞" NOR ")");

    if (fShort) {
        if (obj->short_descr)
            strcat(buf, obj->short_descr);
        else {
            if (obj->name) {
                strcat(buf, "(");
                strcat(buf, obj->name);
                strcat(buf, ")");
            } else {
                strcat(buf, "(" HIB "不明物品" NOR ")");
            }
        }
        /*
          if ( obj->name ) {
              strcat( buf, "(" );
              strcat( buf, obj->name );
              strcat( buf, ")" );
          }
        */
    } else {
        if (obj->description)
            strcat(buf, obj->description);
        else
            strcat(buf, obj->short_descr);
    }

    return buf;
}

char *format_obj_to_char(OBJ_DATA *obj, CHAR_DATA *ch, bool fShort) {
    static char buf[BUF_STRING_LENGTH];
    AFFECT_DATA *paf; // 2021/09/28
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int h = 0;
    int enc = 0;

    buf[0] = '\0';

    if (IS_IMMORTAL(ch))
        sprintf(buf, "[" HIG "%5d" NOR "]", obj->pIndexData->vnum);
    if (IS_SET(obj->pIndexData->souvenir, Z1))
        strcat(buf, "(" HIW "紀念" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_INVIS))
        strcat(buf, "(" YEL "隱形" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_SPELL)) {
        for (paf = obj->affected; paf; paf = paf->next) {
            if (paf->location == APPLY_WEAPON_SPELL)
                a = a + paf->modifier;
            if (paf->location == APPLY_WEAPON_WIND)
                b = b + paf->modifier;
            if (paf->location == APPLY_WEAPON_FIRE)
                c = c + paf->modifier;
            if (paf->location == APPLY_WEAPON_COLD)
                d = d + paf->modifier;
            if (paf->location == APPLY_WEAPON_EARTH)
                e = e + paf->modifier;
            if (paf->location == APPLY_WEAPON_LIGHTNING)
                f = f + paf->modifier;
            if (paf->location == APPLY_WEAPON_SAINT)
                g = g + paf->modifier;
            if (paf->location == APPLY_WEAPON_DARK)
                h = h + paf->modifier;
        }
        for (paf = obj->pIndexData->affected; paf; paf = paf->next) {
            if (paf->location == APPLY_WEAPON_SPELL)
                a = a + paf->modifier;
            if (paf->location == APPLY_WEAPON_WIND)
                b = b + paf->modifier;
            if (paf->location == APPLY_WEAPON_FIRE)
                c = c + paf->modifier;
            if (paf->location == APPLY_WEAPON_COLD)
                d = d + paf->modifier;
            if (paf->location == APPLY_WEAPON_EARTH)
                e = e + paf->modifier;
            if (paf->location == APPLY_WEAPON_LIGHTNING)
                f = f + paf->modifier;
            if (paf->location == APPLY_WEAPON_SAINT)
                g = g + paf->modifier;
            if (paf->location == APPLY_WEAPON_DARK)
                h = h + paf->modifier;
        }
        for (paf = obj->material->affected; paf; paf = paf->next) {
            if (paf->location == APPLY_WEAPON_SPELL)
                a = a + paf->modifier;
            if (paf->location == APPLY_WEAPON_WIND)
                b = b + paf->modifier;
            if (paf->location == APPLY_WEAPON_FIRE)
                c = c + paf->modifier;
            if (paf->location == APPLY_WEAPON_COLD)
                d = d + paf->modifier;
            if (paf->location == APPLY_WEAPON_EARTH)
                e = e + paf->modifier;
            if (paf->location == APPLY_WEAPON_LIGHTNING)
                f = f + paf->modifier;
            if (paf->location == APPLY_WEAPON_SAINT)
                g = g + paf->modifier;
            if (paf->location == APPLY_WEAPON_DARK)
                h = h + paf->modifier;
        }
        /*if( a > 0 || b > 0 || c > 0 || d > 0 || e > 0 || f > 0 || g > 0 || h >
        0 ){ enc = UMAX( h , UMAX( g , UMAX( f , UMAX( e , UMAX( d , UMAX( c ,
        UMAX( b , a ) )))))); if( enc == a ) strcat( buf, "("HIU"附魔"NOR")" );
                else if( enc == b )
                        strcat( buf, "("HIG"纏風"NOR")" );
                else if( enc == c )
                        strcat( buf, "("HIR"灼熱"NOR")" );
                else if( enc == d )
                        strcat( buf, "("HIC"寒霜"NOR")" );
                else if( enc == e )
                        strcat( buf, "("YEL"覆岩"NOR")" );
                else if( enc == f )
                        strcat( buf, "("HIY"電氣"NOR")" );
                else if( enc == g )
                        strcat( buf, "("HIW"閃耀"NOR")" );
                else if( enc == h )
                        strcat( buf, "("HIB"晦暗"NOR")" );
        } */
        if (a > 0)
            enc += 1;
        if (b > 0)
            enc += 1;
        if (c > 0)
            enc += 1;
        if (d > 0)
            enc += 1;
        if (e > 0)
            enc += 1;
        if (f > 0)
            enc += 1;
        if (g > 0)
            enc += 1;
        if (h > 0)
            enc += 1;
        // 修改附魔顯示 2022/12/10
        if (enc > 0) {
            strcat(buf, "(");
            if (enc > 1) { // 裝備有兩個以上正值附魔, 顯示抗性單一字
                if (a > 0)
                    strcat(buf, HIU "魔" NOR);
                if (b > 0)
                    strcat(buf, HIG "風" NOR);
                if (c > 0)
                    strcat(buf, HIR "火" NOR);
                if (d > 0)
                    strcat(buf, HIC "冰" NOR);
                if (e > 0)
                    strcat(buf, YEL "地" NOR);
                if (f > 0)
                    strcat(buf, HIY "雷" NOR);
                if (g > 0)
                    strcat(buf, HIW "光" NOR);
                if (h > 0)
                    strcat(buf, HIB "闇" NOR);
            } else { // 裝備只有一種正值附魔
                if (a > 0)
                    strcat(buf, HIU "附魔" NOR);
                if (b > 0)
                    strcat(buf, HIG "纏風" NOR);
                if (c > 0)
                    strcat(buf, HIR "灼熱" NOR);
                if (d > 0)
                    strcat(buf, HIC "寒霜" NOR);
                if (e > 0)
                    strcat(buf, YEL "覆岩" NOR);
                if (f > 0)
                    strcat(buf, HIY "電氣" NOR);
                if (g > 0)
                    strcat(buf, HIW "閃耀" NOR);
                if (h > 0)
                    strcat(buf, HIB "晦暗" NOR);
            }
            strcat(buf, ")");
        }
    }
    if ((IS_AFFECTED(ch, AFF_DETECT_EVIL) ||
         (IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_ALIGN) &&
          !IS_EVIL(ch))) &&
        IS_OBJ_STAT(obj, ITEM_EVIL))
        strcat(buf, "(" HIR "邪氣" NOR ")");
    if ((IS_AFFECTED(ch, AFF_DETECT_MAGIC) ||
         IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_MAGIC)) &&
        IS_OBJ_STAT(obj, ITEM_MAGIC))
        strcat(buf, "(" HIC "魔力" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_GLOW))
        strcat(buf, "(" HIY "發光" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_HUM))
        strcat(buf, "(" HIP "低鳴" NOR ")");
    if (IS_OBJ_STAT(obj, ITEM_POISONED))
        strcat(buf, "(" HIG "劇毒" NOR ")");
    if (!indestructible(obj) && !is_broken(obj) &&
        (get_curr_dura(obj) * 3 < get_max_dura(obj)))
        strcat(buf, "(" HIP "磨損" NOR ")");
    if (is_broken(obj) || IS_OBJ_STAT(obj, ITEM_BROKEN))
        strcat(buf, "(" HIB "損壞" NOR ")");

    if (fShort) {
        if (obj->short_descr)
            strcat(buf, obj->short_descr);
        else {
            if (obj->name) {
                strcat(buf, "(");
                strcat(buf, obj->name);
                strcat(buf, ")");
            } else {
                strcat(buf, "(" HIB "不明物品" NOR ")");
            }
        }
        /*
                if ( obj->name ) {
                        strcat( buf, "(" );
                        strcat( buf, obj->name );
                        strcat( buf, ")" );
          }
        */
    } else {
        if (obj->description)
            strcat(buf, obj->description);
        else
            strcat(buf, obj->short_descr);
    }

    return buf;
}

/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char(OBJ_DATA *list, CHAR_DATA *ch, bool fShort,
                       bool fShowNothing) {
    OBJ_DATA *obj;
    char buf[BUF_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 7];
    char **prgpstrShow;
    char *pstrShow;
    int *prgnShow;
    int nShow;
    int iShow;
    int count;
    int iLine = 0;
    bool fCombine;

    if (!ch->desc || !list)
        return;

    buf1[0] = '\0';
    /*
     * Alloc space for output lines.
     */
    count = 0;
    for (obj = list; obj; obj = obj->next_content) {
        if (obj->deleted)
            continue;
        count++;
    }

    prgpstrShow = alloc_mem(count * sizeof(char *));
    prgnShow = alloc_mem(count * sizeof(int));
    nShow = 0;

    /*
     * Format the list of objects.
     */
    for (obj = list; obj; obj = obj->next_content) {
        if (obj->wear_loc == WEAR_NONE && can_see_obj(ch, obj)) {
            pstrShow = format_obj_to_char(obj, ch, fShort);
            fCombine = FALSE;

            if (IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE)) {
                /*
                 * Look for duplicates, case sensitive.
                 * Matches tend to be near end so run loop backwords.
                 */
                for (iShow = nShow - 1; iShow >= 0; iShow--) {
                    if (!strcmp(prgpstrShow[iShow], pstrShow)) {
                        prgnShow[iShow]++;
                        fCombine = TRUE;
                        break;
                    }
                }
            }

            /*
             * Couldn't combine, or didn't want to.
             */
            if (!fCombine) {
                prgpstrShow[nShow] = str_dup(pstrShow);
                prgnShow[nShow] = 1;
                nShow++;
            }
        }
    }

    /*
     * Output the formatted list.
     */
    for (iShow = 0; iShow < nShow; iShow++) {
        if (IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE)) {
            if (prgnShow[iShow] != 1) {
                sprintf(buf, "(%2d)", prgnShow[iShow]);
                strcat(buf1, buf);
            } else {
                strcat(buf1, "    ");
            }
        }
        strcat(buf1, prgpstrShow[iShow]);
        strcat(buf1, "\n\r");
        free_string(prgpstrShow[iShow]);
        ++iLine;
        // 修改最大 pagelength , 故可顯示上限提高 2023/01/12
        if (iLine > 80) {
            // if( iLine > 50 ) {
            send_to_char(buf1, ch);
            buf1[0] = '\0';
            iLine = 0;
        }
    }

    if (fShowNothing && nShow == 0) {
        if (IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE))
            strcat(buf1, "    ");
        strcat(buf1, "什麼都沒有。\n\r");
    }

    /*
     * Clean up.
     */
    free_mem(prgpstrShow /*, count * sizeof( char * ) */);
    free_mem(prgnShow /*,    count * sizeof( int )   */);

    send_to_char(buf1, ch);
    return;
}

void show_list_to_char1(char *out, OBJ_DATA *list, CHAR_DATA *ch, bool fShort,
                        bool fShowNothing) {
    OBJ_DATA *obj;
    char buf[BUF_STRING_LENGTH * 2];
    char buf1[MAX_STRING_LENGTH * 9];
    char **prgpstrShow;
    char *pstrShow;
    int *prgnShow;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if (!ch->desc || !list)
        return;

    send_to_char("身上的物品有：\n\r\n\r", ch);
    buf1[0] = '\0';
    /*
     * Alloc space for output lines.
     */
    count = 0;
    for (obj = list; obj; obj = obj->next_content) {
        if (obj->deleted)
            continue;
        count++;
    }

    prgpstrShow = alloc_mem(count * sizeof(char *));
    prgnShow = alloc_mem(count * sizeof(int));
    nShow = 0;

    /*
     * Format the list of objects.
     */
    for (obj = list; obj; obj = obj->next_content) {
        if (obj->wear_loc == WEAR_NONE && can_see_obj(ch, obj)) {
            pstrShow = format_obj_to_char(obj, ch, fShort);
            fCombine = FALSE;

            if (IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE)) {
                /*
                 * Look for duplicates, case sensitive.
                 * Matches tend to be near end so run loop backwords.
                 */
                for (iShow = nShow - 1; iShow >= 0; iShow--) {
                    if (!strcmp(prgpstrShow[iShow], pstrShow)) {
                        prgnShow[iShow]++;
                        fCombine = TRUE;
                        break;
                    }
                }
            }

            /*
             * Couldn't combine, or didn't want to.
             */
            if (!fCombine) {
                prgpstrShow[nShow] = str_dup(pstrShow);
                prgnShow[nShow] = 1;
                nShow++;
            }
        }
    }

    /*
     * Output the formatted list.
     */
    count = 0;
    for (iShow = 0; iShow < nShow; iShow++) {
        if (IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE)) {
            if (prgnShow[iShow] != 1) {
                sprintf(buf, "(%2d)", prgnShow[iShow]);
                strcat(buf1, buf);
            } else {
                strcat(buf1, "    ");
            }
        }
        strcat(buf1, prgpstrShow[iShow]);
        strcat(buf1, "\n\r");
        free_string(prgpstrShow[iShow]);
        ++count;
        // 修改最大 pagelength , 故可顯示上限提高 2023/01/12
        // if( count >= 50 ) {
        if (count >= 80) {
            buf1[0] = '\0';
            send_to_char(buf1, ch);
            count = 0;
        }
    }

    if (fShowNothing && nShow == 0) {
        if (IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE))
            strcat(buf1, " ");
        strcat(buf1, "什麼都沒有。\n\r");
    }

    /*
     * Clean up.
     */
    free_mem(prgpstrShow /*, count * sizeof( char * )*/);
    free_mem(prgnShow /*,    count * sizeof( int )    */);

    send_to_char(buf1, ch);
    //    sprintf( out, buf1 );
    //    直接送出給player，這樣可以減少crash的問題
    return;
}

void show_char_to_char_0b(CHAR_DATA *victim, CHAR_DATA *ch) {
    char buf[BUF_STRING_LENGTH * 4];
    OBJ_DATA *pObj;

    buf[0] = '\0';

    if (IS_NPC(victim) && IS_IMMORTAL(ch)) {
        sprintf(buf, "[" HIG "%5d" NOR "]", victim->pIndexData->vnum);
    }

    // 角色在同個房間中長時間未移動 2023/01/17
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_IDLE) && IS_IMMORTAL(ch))
        strcat(buf, "(" HIB "閒置" NOR ")");

    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS) &&
        can_see(ch, victim))
        strcat(buf, "(" HIY "隱" NOR ")");
    if (IS_AFFECTED(victim, AFF_INVISIBLE))
        strcat(buf, "(" YEL "隱" NOR ")");
    if (IS_AFFECTED(victim, AFF_HIDE))
        strcat(buf, "(" HIB "藏" NOR ")");
    if (IS_AFFECTED(victim, AFF_NIGHTSTALK))
        strcat(buf, "(" HIU "暗" NOR ")");
    if (IS_AFFECTED(victim, AFF_CHARM))
        strcat(buf, "(" HIP "惑" NOR ")");
    if (IS_AFFECTED(victim, AFF_CRAZY))
        strcat(buf, "(" CYN "瘋" NOR ")");
    if (IS_AFFECTED(victim, AFF_PASS_DOOR) ||
        (IS_SET(race_table[victim->race].race_abilities, RACE_PASSDOOR) &&
         (!str_cmp(race_table[victim->race].name, "Undead") ||
          !str_cmp(race_table[victim->race].name, "Vampire"))))
        strcat(buf, "(" CYN "透" NOR ")");
    if (IS_AFFECTED(victim, AFF_FAERIE_FIRE))
        strcat(buf, "(" HIP "霧" NOR ")");
    if (IS_AFFECTED(victim, AFF_TARDY))
        strcat(buf, "(" HIY "霧" NOR ")");
    if (IS_EVIL(victim) &&
        (IS_AFFECTED(ch, AFF_DETECT_EVIL) ||
         (IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_ALIGN) &&
          !IS_EVIL(ch))))
        strcat(buf, "(" HIR "氣" NOR ")");
    if (IS_AFFECTED(victim, AFF_SANCTUARY) ||
        IS_SET(race_table[victim->race].race_abilities, RACE_SANCT))
        strcat(buf, "(" HIW "光" NOR ")");
    if (IS_AFFECTED(victim, AFF_FLAMING))
        strcat(buf, "(" HIP "甲" NOR ")");
    if (is_affected(victim, gsn_invoke))
        strcat(buf, "(" HIB "氣" NOR ")");
    if (IS_AFFECTED(victim, AFF_MYSTIC_ARMOR))
        strcat(buf, "(" HIB "壁" NOR ")");
    if (IS_AFFECTED(victim, AFF_MYSTIC_MIRROR))
        strcat(buf, "(" HIW "鏡" NOR ")");
    if (IS_AFFECTED(victim, AFF_MANA_SHIELD))
        strcat(buf, "(" HIU "盾" NOR ")");
    if (IS_AFFECTED(victim, AFF_FEAR))
        strcat(buf, "(" CYN "恐" NOR ")");
    if (is_affected(victim, gsn_meditate))
        strcat(buf, "(" CYN "冥" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER))
        strcat(buf, "(" RED "兇" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF))
        strcat(buf, "(" RED "偷" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_REGISTER))
        strcat(buf, "(" HIG "PK" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_AFK))
        strcat(buf, "(" HIP "呆" NOR ")");

    if (victim->position == POS_STANDING && victim->long_descr[0] != '\0') {
        strcat(buf, victim->long_descr);
        send_to_char(buf, ch);
        return;
    }

    strcat(buf, PERS(victim, ch));

    if (!IS_NPC(victim) && !IS_SET(ch->act, PLR_BRIEF)) {
        strcat(buf, " ");
        strcat(buf, victim->pcdata->title);
        strcat(buf, " ");
    }

    // add by coconet for horse 0517
    for (pObj = victim->carrying; pObj; pObj = pObj->next_content) {
        if (pObj->wear_loc == WEAR_RIDE) {
            /* 加入 switch 判定 ride 當前 position 狀態 2022/06/16
                    strcat( buf, "騎在");
                    strcat( buf, pObj->description);
                    strcat( buf, "上。 ");
                    strcat( buf, "\n\r" );
                    buf[0] = UPPER( buf[0] );
                    send_to_char( buf, ch );
                    return;
            */
            strcat(buf, "騎在");
            strcat(buf, pObj->description);
            strcat(buf, "上");
            switch (victim->position) {
            case POS_DEAD:
                strcat(buf, "死了！！");
                break;
            case POS_MORTAL:
                strcat(buf, "受了致命傷。");
                break;
            case POS_INCAP:
                strcat(buf, "失去行動能力了。");
                break;
            case POS_STUNNED:
                strcat(buf, "昏倒。");
                break;
            case POS_SLEEPING:
                strcat(buf, "睡覺。");
                break;
            case POS_RESTING:
                strcat(buf, "休息。");
                break;
            case POS_STANDING:
                if (is_flying(victim))
                    strcat(buf, "浮在半空中。");
                else
                    strcat(buf, "。");
                break;
            case POS_FIGHTING:
                strcat(buf, "和");
                if (!victim->fighting)
                    strcat(buf, "空氣(？)打架。");
                else if (victim->fighting == ch)
                    strcat(buf, "你(！)打架。");
                else if (victim->in_room == victim->fighting->in_room) {
                    strcat(buf, PERS(victim->fighting, ch));
                    strcat(buf, "打架。");
                } else {
                    strcat(buf, "不在這裡的誰打架？");
                    if (victim->fighting) {
                        stop_fighting(victim, TRUE);
                    } else
                        stop_fighting(victim, FALSE);
                }
                break;
            }
            strcat(buf, "\n\r");
            buf[0] = UPPER(buf[0]);
            send_to_char(buf, ch);
            return;
        }
    }

    switch (victim->position) {
    case POS_DEAD:
        strcat(buf, "死了！！");
        break;
    case POS_MORTAL:
        strcat(buf, "受了致命傷。");
        break;
    case POS_INCAP:
        strcat(buf, "失去行動能力了。");
        break;
    case POS_STUNNED:
        strcat(buf, "昏倒在地。");
        break;
    case POS_SLEEPING:
        // if( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_MEDITATING))
        //   strcat( buf, "在這裡冥想。"        );
        // else
        strcat(buf, "在這裡睡覺。");
        break;
    case POS_RESTING:
        strcat(buf, "在這裡休息。");
        break;
    case POS_STANDING:
        if (is_flying(victim))
            strcat(buf, "浮在半空中。");
        else
            strcat(buf, "站在這兒。");
        break;
    case POS_FIGHTING:
        strcat(buf, "正在這裡和");
        if (!victim->fighting)
            strcat(buf, "空氣(？)打架。");
        else if (victim->fighting == ch)
            strcat(buf, "你(！)打架。");
        else if (victim->in_room == victim->fighting->in_room) {
            strcat(buf, PERS(victim->fighting, ch));
            strcat(buf, "打架。");
        } else {
            strcat(buf, "不在這裡的誰打架？");
            if (victim->fighting) {
                stop_fighting(victim, TRUE);
            } else
                stop_fighting(victim, FALSE);
        }
        break;
    }

    strcat(buf, "\n\r");
    buf[0] = UPPER(buf[0]);
    send_to_char(buf, ch);
    return;
}
/* called by show_char_to_char */
void show_char_to_char_0(CHAR_DATA *victim, CHAR_DATA *ch) {
    char buf[BUF_STRING_LENGTH * 4];
    OBJ_DATA *pObj;

    buf[0] = '\0';
    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF)) {
        show_char_to_char_0b(victim, ch);
        return;
    }

    if (IS_NPC(victim) && IS_IMMORTAL(ch)) {
        sprintf(buf, "[" HIG "%5d" NOR "]", victim->pIndexData->vnum);
    }

    // 角色在同個房間中長時間未移動 2023/01/17
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_IDLE) && IS_IMMORTAL(ch))
        strcat(buf, "(" HIB "閒置" NOR ")");

    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS) &&
        can_see(ch, victim))
        strcat(buf, "(" HIY "神隱" NOR ")");
    if (IS_AFFECTED(victim, AFF_INVISIBLE))
        strcat(buf, "(" YEL "隱形" NOR ")");
    if (IS_AFFECTED(victim, AFF_HIDE))
        strcat(buf, "(" HIB "藏匿" NOR ")");
    if (IS_AFFECTED(victim, AFF_NIGHTSTALK))
        strcat(buf, "(" HIU "暗匿" NOR ")");
    if (IS_SET(victim->act, ACT_PET) && IS_AFFECTED(victim, AFF_CHARM))
        strcat(buf, "(" HIP "寵物" NOR ")");
    else if (IS_AFFECTED(victim, AFF_CHARM))
        strcat(buf, "(" HIP "迷失" NOR ")");

    if (IS_AFFECTED(victim, AFF_CRAZY))
        strcat(buf, "(" CYN "瘋狂" NOR ")");
    if (IS_AFFECTED(victim, AFF_PASS_DOOR) ||
        (IS_SET(race_table[victim->race].race_abilities, RACE_PASSDOOR) &&
         (!str_cmp(race_table[victim->race].name, "Undead") ||
          !str_cmp(race_table[victim->race].name, "Vampire")))) {
        strcat(buf, "(" CYN "半透明" NOR ")");
    }
    if (IS_AFFECTED(victim, AFF_FAERIE_FIRE))
        strcat(buf, "(" HIP "紫霧" NOR ")");
    if (IS_AFFECTED(victim, AFF_TARDY))
        strcat(buf, "(" HIY "黃霧" NOR ")");
    if (IS_EVIL(victim) &&
        (IS_AFFECTED(ch, AFF_DETECT_EVIL) ||
         (IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_ALIGN) &&
          !IS_EVIL(ch)))) {
        strcat(buf, "(" HIR "邪氣" NOR ")");
    }
    if (IS_AFFECTED(victim, AFF_SANCTUARY) ||
        IS_SET(race_table[victim->race].race_abilities, RACE_SANCT))
        strcat(buf, "(" HIW "聖光" NOR ")");
    if (IS_AFFECTED(victim, AFF_FLAMING))
        strcat(buf, "(" HIP "焰甲" NOR ")");
    if (is_affected(victim, gsn_invoke))
        strcat(buf, "(" HIB "罡氣" NOR ")");
    if (IS_AFFECTED(victim, AFF_MYSTIC_ARMOR))
        strcat(buf, "(" HIB "魔壁" NOR ")");
    if (IS_AFFECTED(victim, AFF_MYSTIC_MIRROR))
        strcat(buf, "(" HIW "魔鏡" NOR ")");
    if (IS_AFFECTED(victim, AFF_MANA_SHIELD))
        strcat(buf, "(" HIU "魔盾" NOR ")");
    if (IS_AFFECTED(victim, AFF_FEAR))
        strcat(buf, "(" CYN "恐懼" NOR ")");
    if (is_affected(victim, gsn_meditate))
        strcat(buf, "(" CYN "冥想" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER))
        strcat(buf, "(" RED "兇手" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF))
        strcat(buf, "(" RED "小偷" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_REGISTER))
        strcat(buf, "(" HIG "PK" NOR ")");
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_AFK))
        strcat(buf, "(" HIP "發呆" NOR ")");

    if (victim->position == POS_STANDING && victim->long_descr[0] != '\0') {
        strcat(buf, victim->long_descr);
        send_to_char(buf, ch);
        return;
    }
    /*
    bug( "show_char_to_char_0 log: 1.ch->name, 2.vic->name", 0 );
    bug( ch->name, 0 );
    bug( victim->name, 0 );
    */
    strcat(buf, PERS(victim, ch));

    if (!IS_NPC(victim) && !IS_SET(ch->act, PLR_BRIEF)) {
        strcat(buf, " ");
        strcat(buf, victim->pcdata->title);
        strcat(buf, " ");
    }

    // add by coconet for horse 0517
    for (pObj = victim->carrying; pObj; pObj = pObj->next_content) {
        if (pObj->wear_loc == WEAR_RIDE) {
            /* 加入 switch 判定 ride 當前 position 狀態 2022/06/16
                    strcat( buf, "騎在");
                    strcat( buf, pObj->description);
                    strcat( buf, "上。 ");
                    strcat( buf, "\n\r" );
                    buf[0] = UPPER( buf[0] );
                    send_to_char( buf, ch );
                    return;
            */
            strcat(buf, "騎在");
            strcat(buf, pObj->description);
            strcat(buf, "上");
            switch (victim->position) {
            case POS_DEAD:
                strcat(buf, "死了！！");
                break;
            case POS_MORTAL:
                strcat(buf, "受了致命傷。");
                break;
            case POS_INCAP:
                strcat(buf, "失去行動能力了。");
                break;
            case POS_STUNNED:
                strcat(buf, "昏倒。");
                break;
            case POS_SLEEPING:
                strcat(buf, "睡覺。");
                break;
            case POS_RESTING:
                strcat(buf, "休息。");
                break;
            case POS_STANDING:
                if (is_flying(victim))
                    strcat(buf, "浮在半空中。");
                else
                    strcat(buf, "。");
                break;
            case POS_FIGHTING:
                strcat(buf, "和");
                if (!victim->fighting)
                    strcat(buf, "空氣(？)打架。");
                else if (victim->fighting == ch)
                    strcat(buf, "你(！)打架。");
                else if (victim->in_room == victim->fighting->in_room) {
                    strcat(buf, PERS(victim->fighting, ch));
                    strcat(buf, "打架。");
                } else {
                    strcat(buf, "不在這裡的誰打架？");
                    if (victim->fighting) {
                        stop_fighting(victim, TRUE);
                    } else
                        stop_fighting(victim, FALSE);
                }
                break;
            }
            strcat(buf, "\n\r");
            buf[0] = UPPER(buf[0]);
            send_to_char(buf, ch);
            return;
        }
    }

    switch (victim->position) {
    case POS_DEAD:
        strcat(buf, "死了！！");
        break;
    case POS_MORTAL:
        strcat(buf, "受了致命傷。");
        break;
    case POS_INCAP:
        strcat(buf, "失去行動能力了。");
        break;
    case POS_STUNNED:
        strcat(buf, "昏倒在地。");
        break;
    case POS_SLEEPING:
        strcat(buf, "在這裡睡覺。");
        break;
    case POS_RESTING:
        strcat(buf, "在這裡休息。");
        break;
    case POS_STANDING:
        if (is_flying(victim))
            strcat(buf, "浮在半空中。");
        else
            strcat(buf, "站在這兒。");
        break;

    case POS_FIGHTING:
        strcat(buf, "正在這裡和");
        if (!victim->fighting)
            strcat(buf, "空氣(？)打架。");
        else if (victim->fighting == ch)
            strcat(buf, "你(！)打架。");
        else if (victim->in_room == victim->fighting->in_room) {
            strcat(buf, PERS(victim->fighting, ch));
            strcat(buf, "打架。");
        } else {
            strcat(buf, "不在這裡的誰打架？");
            if (victim->fighting) {
                stop_fighting(victim, TRUE);
            } else
                stop_fighting(victim, FALSE);
        }
        break;
    }

    strcat(buf, "\n\r");
    buf[0] = UPPER(buf[0]);
    send_to_char(buf, ch);
    return;
}

/* called when look someone */
void show_char_to_char_1(CHAR_DATA *victim, CHAR_DATA *ch) {
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH * 15];
    char buf1[MAX_STRING_LENGTH * 16];
    int iWear;
    int percent;
    bool found;
    int count = 0;
    int race;

    if (can_see(victim, ch)) {
        act("[36m$n仔細打量著你。[m", ch, NULL, victim, TO_VICT);
        act("[36m$n仔細打量著$N。[m", ch, NULL, victim, TO_NOTVICT);
    }

    race = ((victim->temp_race) ? victim->temp_race - 1 : victim->race);

    // 新增 aff_ghoul 及 stone form 的情況下在種族名前顯示不死系 2022/01/16
    // sprintf( buf1, "$N - %s(%s):\n\r", race_table[ race ].cname, race_table[
    // race ].name );
    if (is_affected(victim, gsn_stone_form) ||
        (!IS_SET(race_table[victim->race].race_abilities, RACE_UNDEAD) &&
         IS_AFFECTED(victim, AFF_GHOUL)))
        sprintf(buf1, "$N - 不死系%s(%s):\n\r", race_table[race].cname,
                race_table[race].name);
    else
        sprintf(buf1, "$N - %s(%s):\n\r", race_table[race].cname,
                race_table[race].name);

    if (victim->description[0] != '\0') {
        send_to_char(victim->description, ch);
        send_to_char("\n\r", ch);
    } else {
        strcat(buf1, "$E看起來沒有什麼特色。");
    }

    if (victim->max_hit > 0)
        percent = (100 * victim->hit) / victim->max_hit;
    else
        percent = -1;

    sprintf(buf, PERS(victim, ch));

    if (percent >= 100)
        strcat(buf, HIG "健康狀況良好。" NOR "\n\r");
    else if (percent >= 90)
        strcat(buf, HIG "受了輕微的擦傷。" NOR "\n\r");
    else if (percent >= 80)
        strcat(buf, HIG "身上有一點小瘀血。" NOR "\n\r");
    else if (percent >= 70)
        strcat(buf, HIY "身上有一些傷痕。" NOR "\n\r");
    else if (percent >= 60)
        strcat(buf, HIY "受了小傷。" NOR "\n\r");
    else if (percent >= 50)
        strcat(buf, HIY "看來狀況並不太好。" NOR "\n\r");
    else if (percent >= 40)
        strcat(buf, HIR "傷痕累累。" NOR "\n\r");
    else if (percent >= 30)
        strcat(buf, HIR "渾身是血。" NOR "\n\r");
    else if (percent >= 20)
        strcat(buf, HIR "受了十分嚴重的傷，隨時都可能暈倒。" NOR "\n\r");
    else if (percent >= 10)
        strcat(buf, RED "已經可以看到死神對他微笑。" NOR "\n\r");
    else
        strcat(buf, RED "已是進氣少，出氣多，眼看就要死去。" NOR "\n\r");

    buf[0] = UPPER(buf[0]);
    strcat(buf1, buf);
    buf[0] = '\0';

    found = FALSE;
    if (!IS_SET(race_table[victim->race].race_abilities, RACE_BEHOLDER)) {
        for (iWear = 0; iWear < MAX_WEAR; iWear++) {
            if ((obj = get_eq_char(victim, iWear)) && can_see_obj(ch, obj)) {
                if (!found) {
                    strcat(buf1, "\n\r");
                    strcat(buf1, "$E身上穿著：\n\r");
                    found = TRUE;
                }
                strcat(buf1, where_name[iWear]);
                strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                strcat(buf1, "\n\r");
                ++count;
            }
            if (iWear == WEAR_LEGS) {
                if ((obj = get_eq_char(victim, WEAR_LEGS_2)) &&
                    can_see_obj(ch, obj)) {
                    if (!found) {
                        strcat(buf1, "\n\r");
                        strcat(buf1, "$E身上穿著：\n\r");
                        found = TRUE;
                    }
                    strcat(buf1, where_name[WEAR_LEGS_2]);
                    strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                    strcat(buf1, "\n\r");
                    ++count;
                }
            }
            if (iWear == WEAR_FEET) {
                if ((obj = get_eq_char(victim, WEAR_FEET_2)) &&
                    can_see_obj(ch, obj)) {
                    if (!found) {
                        strcat(buf1, "\n\r");
                        strcat(buf1, "$E身上穿著：\n\r");
                        found = TRUE;
                    }
                    strcat(buf1, where_name[WEAR_FEET_2]);
                    strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                    strcat(buf1, "\n\r");
                    ++count;
                }
            }
        }
        // 新增彈藥裝備 2022/04/30
        if ((obj = get_eq_char(victim, WEAR_AMMO)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_AMMO]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
        if ((obj = get_eq_char(victim, WEAR_RIDE)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_RIDE]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
    } else { // Code for beholder's wearing
        for (iWear = 0; iWear <= WEAR_FINGER_R;
             iWear++) { // from light source to the second ring.
            if ((obj = get_eq_char(victim, iWear)) && can_see_obj(ch, obj)) {
                if (!found) {
                    strcat(buf1, "\n\r");
                    strcat(buf1, "$E身上穿著：\n\r");
                    found = TRUE;
                }
                strcat(buf1, where_name[iWear]);
                strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                strcat(buf1, "\n\r");
                ++count;
            }
        }
        // Finger 3 and Finger 4 for beholders
        for (iWear = WEAR_FINGER_3; iWear <= WEAR_FINGER_4; iWear++) {
            if ((obj = get_eq_char(victim, iWear)) && can_see_obj(ch, obj)) {
                if (!found) {
                    strcat(buf1, "\n\r");
                    strcat(buf1, "$E身上穿著：\n\r");
                    found = TRUE;
                }
                strcat(buf1, where_name[iWear]);
                strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                strcat(buf1, "\n\r");
                ++count;
            }
        }

        for (iWear = WEAR_NECK_1; iWear <= WEAR_HANDS;
             iWear++) { // from necklace to the end
            if ((obj = get_eq_char(victim, iWear)) && can_see_obj(ch, obj)) {
                if (!found) {
                    strcat(buf1, "\n\r");
                    strcat(buf1, "$E身上穿著：\n\r");
                    found = TRUE;
                }
                strcat(buf1, where_name[iWear]);
                strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                strcat(buf1, "\n\r");
                ++count;
            }
        }
        if ((obj = get_eq_char(victim, WEAR_HANDS_2)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_HANDS_2]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
        if ((obj = get_eq_char(victim, WEAR_ARMS)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_ARMS]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
        if ((obj = get_eq_char(victim, WEAR_ARMS_2)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_ARMS_2]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
        for (iWear = WEAR_SHIELD; iWear < MAX_WEAR;
             iWear++) { // from necklace to the end
            if ((obj =
                     get_eq_char(victim, iWear)) // MAX_WEAR  excludes ride,
                                                 // additional rings and sleeves
                && can_see_obj(ch, obj)) {
                if (!found) {
                    strcat(buf1, "\n\r");
                    strcat(buf1, "$E身上穿著：\n\r");
                    found = TRUE;
                }
                strcat(buf1, where_name[iWear]);
                strcat(buf1, format_obj_to_char(obj, ch, TRUE));
                strcat(buf1, "\n\r");
                ++count;
            }
        }
        // 新增彈藥裝備 2022/04/30
        if ((obj = get_eq_char(victim, WEAR_AMMO)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_AMMO]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
        if ((obj = get_eq_char(victim, WEAR_RIDE)) && can_see_obj(ch, obj)) {
            if (!found) {
                strcat(buf1, "\n\r");
                strcat(buf1, "$E身上穿著：\n\r");
                found = TRUE;
            }
            strcat(buf1, where_name[WEAR_RIDE]);
            strcat(buf1, format_obj_to_char(obj, ch, TRUE));
            strcat(buf1, "\n\r");
            ++count;
        }
    }

    act(buf1, ch, NULL, victim, TO_CHAR);
    buf1[0] = '\0';
    count = 0;
    if (victim != ch && !IS_NPC(ch) &&
        number_percent() < get_skill_percent(ch, gsn_peek)) {
        if (IS_NPC(victim) && victim->pIndexData->pShop)
            strcat(buf1, "\n\r一種奇怪的力量阻絕你的窺視！\n\r");
        else
            show_list_to_char1(buf, victim->carrying, ch, TRUE, TRUE);
    }

    act(buf1, ch, NULL, victim, TO_CHAR);
    return;
}

/* called when PC look environment or enter a room */
void show_char_to_char(CHAR_DATA *list, CHAR_DATA *ch) {
    CHAR_DATA *rch;

    if (!list)
        return;

    for (rch = list; rch; rch = rch->next_in_room) {
        if (rch->deleted || rch == ch)
            continue;

        if (!can_see(ch, rch))
            continue;
        // if ( !IS_NPC( rch )
        //   && IS_SET( rch->act, PLR_WIZINVIS ))
        // continue;

        if (can_see(ch, rch)) {
            show_char_to_char_0(rch, ch);
        } else if (room_is_dark(ch->in_room) &&
                   (IS_AFFECTED(rch, AFF_INFRARED) ||
                    IS_SET(race_table[rch->race].race_abilities,
                           RACE_INFRAVISION))) {
            send_to_char("你感到有人正注視著你！\n\r", ch);
        }
    }

    return;
}

bool check_blind(CHAR_DATA *ch) {
    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) && IS_IMMORTAL(ch))
        return TRUE;

    if (IS_AFFECTED(ch, AFF_BLIND)) {
        send_to_char("你什麼都看不到！\n\r", ch);
        return FALSE;
    }

    return TRUE;
}

void top10_remove(TOP10_DATA *td, CHAR_DATA *ch, int value, bool fIMM) {
    int i, j;
    // int fFound = FALSE;
    char *id = str_dup(ch->name);
    char *none = "n / a";

    if (!fIMM && IS_IMMORTAL(ch))
        return;

    for (i = 0; i < TOP_COUNT; ++i) {
        if (td[i].id && td[i].id[0] != '\0' && !str_cmp(td[i].id, id)) {
            for (j = i; j < TOP_COUNT - 1; ++j) {
                td[j].id = td[j + 1].id;
                td[j].value = td[j + 1].value;
            }
            td[TOP_COUNT].id = none;
            td[TOP_COUNT].value = 0;
            break;
        }
    }

    if (value <= td[TOP_COUNT].value)
        return;

    for (i = 0; i < TOP_COUNT; ++i) {
        if (td[i].value < value) {
            for (j = TOP_COUNT; j > i; --j) {
                td[j].id = td[j - 1].id;
                td[j].value = td[j - 1].value;
            }
            td[i].id = id;
            td[i].value = value;
            break;
        }
    }

    return;
}

void top10_check(CHAR_DATA *ch) {
    if (IS_SET(ch->act, PLR_HORSE))
        return;
    top10_remove(tmudage, ch, ch->played / 3600, FALSE);
    top10_remove(tgold, ch, ch->bank + ch->gold, FALSE);
    top10_remove(texp, ch, ch->exp + ch->practice * 5000, FALSE);
    top10_remove(thp, ch, ch->max_hit, FALSE);
    top10_remove(t_mp, ch, ch->max_mana, FALSE);
    top10_remove(tmv, ch, ch->max_move, FALSE);
    top10_remove(tpk, ch, ch->pcdata->pkwin, FALSE);
    top10_remove(tresp, ch, ch->pcdata->respect, FALSE);
    top10_remove(tmerit, ch, ch->pcdata->merit, FALSE);
    top10_save();
}

void show_top10(CHAR_DATA *ch, TOP10_DATA *td, char *title, char *prefix,
                char *posfix) {
    int i;
    char buf[MAX_STRING_LENGTH];
    char show[MAX_STRING_LENGTH];
    send_to_char(title, ch);
    send_to_char("\n\r==============================="
                 "=========================================\n\r",
                 ch);
    show[0] = 0;
    for (i = 0; i < TOP_COUNT; ++i) {
        if (i < 10) {
            sprintf(buf, "%s[%2d] %-20s %s %5d %s。" NOR "\n\r",
                    (str_cmp(td[i].id, ch->name)) ? HIW : HIY, i + 1, td[i].id,
                    prefix, td[i].value, posfix);
        } else {
            sprintf(buf, "%s[%2d] %-20s %s %5d %s。" NOR "\n\r",
                    (str_cmp(td[i].id, ch->name)) ? CYN : HIC, i + 1, td[i].id,
                    prefix, td[i].value, posfix);
        }
        strcat(show, buf);
    }
    send_to_char(show, ch);
    send_to_char("\n\r==============================="
                 "=========================================\n\r",
                 ch);
}

void do_top10(CHAR_DATA *ch, char *argument) {
    // char buf [MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int i, j;
    char *none = "n / a";
    TOP10_DATA *ranks[9] = {tmudage, tgold, texp,  thp,   t_mp,
                            tmv,     tpk,   tresp, tmerit};
    argument = one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("請輸入想看的排名種類。\n\r", ch);
        send_to_char("可選擇的種類有：hp, mana, move, resp, merit, gold, "
                     "mudage, pk\n\r",
                     ch);
        return;
    }

    if (!str_cmp(arg, "reset")) {
        if (!IS_IMMORTAL(ch)) {
            send_to_char("什麼？\n\r", ch);
            return;
        }
        if (IS_NPC(ch) || get_trust(ch) < 45) {
            send_to_char("你的權限不夠。\n\r", ch);
            return;
        }
        for (j = 0; j < 9; ++j) {
            for (i = 0; i < TOP_COUNT + 1; ++i) {
                ranks[j][i].id = none;
                ranks[j][i].value = 0;
            }
        }

        return;
    }

    if (!str_cmp(arg, "pk")) {
        show_top10(ch, tpk, "[幻世PK王排行]", "PK 紀錄", "勝");
    } else if (!str_cmp(arg, "mudage")) {
        show_top10(ch, tmudage, "[幻世上線時間排行]", "上線時間", "小時");
    } else if (!str_cmp(arg, "gold")) {
        show_top10(ch, tgold, "[幻世 50 大富豪排行]", "財產共有", "枚史幣");
    } else if (!str_cmp(arg, "exp")) {
        show_top10(ch, texp, "[幻世 50 大米蟲排行]", "經驗點數", "點");
    } else if (!str_cmp(arg, "hp")) {
        show_top10(ch, thp, "[幻世 50 大血牛排行]", "生命力", "點");
    } else if (!str_cmp(arg, "mana")) {
        show_top10(ch, t_mp, "[幻世 50 大法力排行]", "法力", "點");
    } else if (!str_cmp(arg, "move")) {
        show_top10(ch, tmv, "[幻世 50 大體能排行]", "體力", "點");
    } else if (!str_cmp(arg, "resp")) {
        show_top10(ch, tresp, "[幻世 50 大名人排行]", "威望值", "點");
    } else if (!str_cmp(arg, "merit")) {
        show_top10(ch, tmerit, "[幻世 50 大功勳排行]", "功勳值", "點");
    }

    /* OLD TOP10

    if( !str_cmp( arg, "pk" )){
    send_to_char("[幻世十大PK王排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s PK 紀錄 %4d 勝。%s\n\r",
          ( str_cmp(tpk[i].id, ch->name) ) ?
      PUR: HIP,
      i+1,
      tpk[i].id,
      tpk[i].value ,
      NOR);

      send_to_char(buf, ch);
    }

    send_to_char("========================================================================\n\r",
    ch); return;
      }
      if( !str_cmp( arg, "mudage" )){
    send_to_char("[幻世十大上線時間排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s 上線時間 %4d 小時。%s\n\r",
          ( str_cmp(tmudage[i].id, ch->name) ) ?
      WHT: HIW,
      i+1,
      tmudage[i].id,
      tmudage[i].value / 3600,
      NOR);

      send_to_char(buf, ch);
    }

    send_to_char("========================================================================\n\r",
    ch); return;
      }
      if( !str_cmp( arg, "gold" )){
    send_to_char("[幻世十大富豪排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s 財產共有 %d 枚金幣。%s\n\r",
          ( str_cmp(tgold[i].id, ch->name) ) ?
      YEL : HIY,
      i+1,
      tgold[i].id,
      tgold[i].value ,
      NOR);

      send_to_char(buf, ch);
    }

    send_to_char("========================================================================\n\r",
    ch); return;
      }
      if( !str_cmp( arg, "exp" )){
    send_to_char("[幻世十大高手排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s 經驗點數 %10d 點。%s\n\r",
          ( str_cmp(texp[i].id, ch->name) ) ?
      GRN : HIG,
      i+1,
      texp[i].id,
      texp[i].value ,
      NOR);

      send_to_char(buf, ch);
    }

    send_to_char("========================================================================\n\r",
    ch); return;
      }
      if( !str_cmp( arg, "hp" )){
    send_to_char("[幻世十大血牛排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s 生命點數 %5d 點。%s\n\r",
          ( str_cmp(thp[i].id, ch->name) ) ?
      CYN : HIC,
      i+1,
      thp[i].id,
      thp[i].value ,
      NOR);

      send_to_char(buf, ch);
    }

    send_to_char("========================================================================\n\r",
    ch); return;
      }
      if( !str_cmp( arg, "mana" )){
    send_to_char("[幻世十大魔法高手排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s 魔力點數 %5d 點。%s\n\r",
          ( str_cmp(t_mp[i].id, ch->name) ) ?
      CYN : HIC,
      i+1,
    t_mp[i].id,
      t_mp[i].value ,
      NOR);

      send_to_char(buf, ch);
    }

    send_to_char("========================================================================\n\r",
    ch); return;
      }
      if( !str_cmp( arg, "move" )){
    send_to_char("[幻世十大體力高手排行]\n\r========================================================================\n\r",
    ch);

    for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s 移動力 %5d 點。%s\n\r",
          ( str_cmp(tmv[i].id, ch->name) ) ?
      CYN : HIC,
      i+1,
      tmv[i].id,
      tmv[i].value ,
      NOR);

      send_to_char(buf, ch);
    }
    send_to_char("========================================================================\n\r",
    ch); return;
      }

      if( !str_cmp( arg, "resp" )){
        send_to_char("[幻世十大名人排行]\n\r========================================================================\n\r",
    ch); for( i=0; i< 10; ++i){ sprintf(buf, "%s[%2d] %-20s 威望值 %5d
    點。%s\n\r", ( str_cmp(tresp[i].id, ch->name) ) ? CYN : HIC, i+1,
              tresp[i].id,
              tresp[i].value ,
              NOR);
          send_to_char(buf, ch);
        }
        send_to_char("========================================================================\n\r",
    ch); return;
      }

      if( !str_cmp( arg, "merit" )){
        send_to_char("[幻世十大功\勳排行]\n\r========================================================================\n\r",
    ch); for( i=0; i< 10; ++i){ sprintf(buf, "%s[%2d] %-20s 功\勳值 %5d
    點。%s\n\r", ( str_cmp(tmerit[i].id, ch->name) ) ? CYN : HIC, i+1,
              tmerit[i].id,
              tmerit[i].value ,
              NOR);
          send_to_char(buf, ch);
        }
        send_to_char("========================================================================\n\r",
    ch); return;
      }
      return;
    */
}

void do_look(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *victim, *rch;
    EXIT_DATA *pexit;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pdesc;
    int door;

    if (!IS_NPC(ch) && !ch->desc)
        return;

    if (ch->position < POS_SLEEPING) {
        send_to_char(
            "[33m你只能看到星星而已！多美麗啊！[m\n\r", ch);
        return;
    }

    if (ch->position == POS_SLEEPING) {
        send_to_char("[33m你只能看到夢境而已！Zzz...[m\n\r", ch);
        return;
    }

    if (!check_blind(ch))
        return;

    if (!IS_NPC(ch) && !IS_SET(ch->act, PLR_HOLYLIGHT) &&
        room_is_dark(ch->in_room)) {
        send_to_char("一片漆黑....\n\r", ch);
        show_char_to_char(ch->in_room->people, ch);
        return;
    }

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0' || !str_cmp(arg1, "auto") || !str_cmp(arg1, "move")) {
        /* 'look' or 'look auto' */
        if (IS_IMMORTAL(ch) ||
            (!IS_NPC(ch) && ch->clan && IS_PPLBUILDER(ch, ch->in_room->area))) {
            sprintf(buf, "[" HIY "%d" NOR "]", ch->in_room->vnum);
            send_to_char(buf, ch);
        }

        send_to_char(ch->in_room->name, ch);
        send_to_char(" - ", ch);
        if (!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT))
            do_exits(ch, "auto");

        if (arg1[0] == '\0' || (!IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF)))
            send_to_char("    ", ch);

        send_to_char(ch->in_room->description, ch);

        if (IS_SET(ch->in_room->room_flags, ROOM_CONE_OF_SILENCE))
            send_to_char("這裡異常地安靜。\n\r", ch);

        // evacuate mark
        if ((get_skill_level(ch, gsn_evacuate) > 2) &&
            IS_SET(ch->in_room->room_flags, ROOM_CAMP))
            send_to_char(HIU "這裡的地面流竄著魔力。" NOR "\n\r", ch);

#ifdef HAS_ROOM_AFFECT

        if (IS_AFFECTED(ch->in_room, R_AFF_FIREWALL)) {
            send_to_char(HIR "\n\r\t一道火牆正在這裡燃燒著。" NOR "\n\r", ch);
        }
#endif

        if (ch->in_room->people) {
            send_to_char("\n\r", ch);
            if (str_cmp(arg1, "move") || !IS_SET(ch->act, PLR_BRIEF))
                show_char_to_char(ch->in_room->people, ch);
            else // in big group, grep the redundant message.
            {
                for (rch = ch->in_room->people; rch; rch = rch->next_in_room) {
                    if (rch->deleted || rch == ch)
                        continue;

                    if (!IS_NPC(rch) && IS_SET(rch->act, PLR_WIZINVIS))
                        continue;
                    if (!can_see(ch, rch))
                        continue;

                    if (can_see(ch, rch) &&
                        (!is_same_group(ch, rch) ||
                         (ch->group && ch->group->leader == rch))) {
                        show_char_to_char_0(rch, ch);
                    } else if (room_is_dark(ch->in_room) &&
                               (IS_AFFECTED(rch, AFF_INFRARED) ||
                                IS_SET(race_table[rch->race].race_abilities,
                                       RACE_INFRAVISION))) {
                        send_to_char("你感到有人正注視著你！\n\r", ch);
                    }
                }
            }
        }
        if (ch->in_room->contents) {
            send_to_char("\n\r", ch);
            show_list_to_char(ch->in_room->contents, ch, FALSE, FALSE);
        }
        return;
    }

    if (!str_cmp(arg1, "in") || (!str_prefix(arg1, "in") && arg2[0])) {
        /* 'look in' */
        if (arg2[0] == '\0') {
            send_to_char("[33m要往什麼裡面看？[m\n\r", ch);
            return;
        }

        if (!(obj = get_obj_here(ch, arg2))) {
            send_to_char("[33m你在這裡沒看到那玩意兒。[m\n\r",
                         ch);
            return;
        }

        // 對能被 look in 的 obj type 新增 obj_act look_in 2022/02/12
        switch (obj->item_type) {
        default:
            send_to_char("[33m那不是個容器。[m\n\r", ch);
            break;

        case ITEM_DRINK_CON:
            if (obj->value[2] >= LIQ_MAX || obj->value[2] < 0) {
                bug("Do_drink: bad liquid number %d.", obj->pIndexData->vnum);
                obj->value[2] = 0;
            }
            if (obj->value[1] <= 0 && obj->value[1] != -1) {
                send_to_char("[36m那是空的。[m\n\r", ch);
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("look_in", ch, obj);
                }
                break;
            }
            sprintf(buf, "那裡面有%s%s液體。\n\r",
                    (obj->value[1] == -1) ? "如同小宇宙般巨量的"
                    : obj->value[1] < obj->value[0] / 4     ? "不到一半的"
                    : obj->value[1] < 3 * obj->value[0] / 4 ? "大約一半的"
                                                            : "超過一半的",
                    liq_table[obj->value[2]].liq_color);
            send_to_char(buf, ch);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;

        case ITEM_FOUNTAIN:
            if (obj->value[2] >= LIQ_MAX || obj->value[2] < 0) {
                bug("Do_drink: bad liquid number %d.", obj->pIndexData->vnum);
                obj->value[2] = 0;
            }
            if (obj->value[1] <= 0 && obj->value[1] != -1) {
                send_to_char("[36m那是乾的。[m\n\r", ch);
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("look_in", ch, obj);
                }
                break;
            }
            sprintf(buf, "那裡面是%s%s。\n\r",
                    (obj->value[1] == -1)                   ? "源源不絕的"
                    : obj->value[1] < obj->value[0] / 4     ? "涓涓的"
                    : obj->value[1] < 3 * obj->value[0] / 4 ? "緩緩流動的"
                                                            : "淙淙的",
                    liq_table[obj->value[2]].liq_name);
            send_to_char(buf, ch);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;

        case ITEM_CONTAINER:
        case ITEM_CORPSE_NPC:
        case ITEM_CORPSE_PC:
            if (IS_SET(obj->value[1], CONT_CLOSED)) {
                send_to_char("[33m它是關著的。[m\n\r", ch);
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("look_in", ch, obj);
                }
                break;
            }

            act("$p裡面有：", ch, obj, NULL, TO_CHAR);
            show_list_to_char(obj->contains, ch, TRUE, TRUE);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;
        case ITEM_HORSE:
            if (obj->value[0] == -1) {
                send_to_char("[33m牠身上沒有負載任何東西。[m\n\r",
                             ch);
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("look_in", ch, obj);
                }
                break;
            }

            act("$p身上負載著：", ch, obj, NULL, TO_CHAR);
            show_list_to_char(obj->contains, ch, TRUE, TRUE);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;
        case ITEM_ARROW:
            if (obj->value[0] <= 0) {
                // 增加 -1 的情況 2023/01/05
                if (obj->value[0] == -1)
                    send_to_char("[36m裡面箭矢的數量有如浩瀚的小宇宙。["
                                 "m\n\r",
                                 ch);
                else
                    send_to_char("[36m那是空的。[m\n\r", ch);
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("look_in", ch, obj);
                }
                break;
            }

            sprintf(buf, "那裡面還有%d支箭。\n\r", obj->value[0]);
            send_to_char(buf, ch);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;

        case ITEM_AMMO: // add at 2022/05/06
            if (obj->value[0] <= 0) {
                // 增加 -1 的情況 2023/01/05
                if (obj->value[0] == -1)
                    send_to_char("[36m裡面彈藥的數量有如浩瀚的小宇宙。["
                                 "m\n\r",
                                 ch);
                else
                    send_to_char("[36m那是空的。[m\n\r", ch);
                if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                    oprog_act_trigger("look_in", ch, obj);
                }
                break;
            }

            sprintf(buf, "還有%d發彈藥。\n\r", obj->value[0]);
            send_to_char(buf, ch);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;

        case ITEM_DART:

            sprintf(buf, "還有%d件道具。\n\r", obj->value[0]);
            send_to_char(buf, ch);
            if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
                oprog_act_trigger("look_in", ch, obj);
            }
            break;
        }
        return;
    }

    if ((victim = get_char_room(ch, arg1))) {
        show_char_to_char_1(victim, ch);
        return;
    }

    pdesc = get_extra_descr(arg1, ch->in_room->extra_descr);
    if (pdesc) {
        send_to_char(pdesc, ch);
        return;
    }

    for (obj = ch->carrying; obj; obj = obj->next_content) {
        if (can_see_obj(ch, obj)) {
            pdesc = get_extra_descr(arg1, obj->extra_descr);
            if (pdesc) {
                send_to_char(obj->short_descr, ch);
                send_to_char(" - ", ch);
                send_to_char(obj->material->short_descr, ch);
                send_to_char("\n\r", ch);
                send_to_char(pdesc, ch);
                return;
            }

            pdesc = get_extra_descr(arg1, obj->pIndexData->extra_descr);
            if (pdesc) {
                send_to_char(obj->short_descr, ch);
                send_to_char(" - ", ch);
                send_to_char(obj->material->short_descr, ch);
                send_to_char("\n\r", ch);
                send_to_char(pdesc, ch);
                return;
            }
        }

        if (is_name(arg1, obj->name)) {
            send_to_char(obj->short_descr, ch);
            send_to_char(" - ", ch);
            send_to_char(obj->material->short_descr, ch);
            send_to_char("\n\r", ch);
            send_to_char(obj->description, ch);
            send_to_char("\n\r", ch);
            return;
        }
    }

    for (obj = ch->in_room->contents; obj; obj = obj->next_content) {
        if (can_see_obj(ch, obj)) {
            pdesc = get_extra_descr(arg1, obj->extra_descr);
            if (pdesc) {
                send_to_char(obj->short_descr, ch);
                send_to_char(" - ", ch);
                send_to_char(obj->material->short_descr, ch);
                send_to_char("\n\r", ch);
                send_to_char(pdesc, ch);
                return;
            }

            pdesc = get_extra_descr(arg1, obj->pIndexData->extra_descr);
            if (pdesc) {
                send_to_char(obj->short_descr, ch);
                send_to_char(" - ", ch);
                send_to_char(obj->material->short_descr, ch);
                send_to_char("\n\r", ch);
                send_to_char(pdesc, ch);
                return;
            }
        }

        if (is_name(arg1, obj->name)) {
            send_to_char(obj->short_descr, ch);
            send_to_char(" - ", ch);
            send_to_char(obj->material->short_descr, ch);
            send_to_char("\n\r", ch);
            send_to_char(obj->description, ch);
            send_to_char("\n\r", ch);
            return;
        }
    }

    pdesc = get_pfxextra_descr(arg1, ch->in_room->extra_descr);
    if (pdesc) {
        // if( obj ) {
        //	send_to_char( obj->short_descr , ch);
        //	send_to_char( " - ", ch);
        //	send_to_char( obj->material->short_descr, ch);
        //	send_to_char("\n\r", ch);
        // }
        send_to_char(pdesc, ch);
        return;
    }

    //	2022/02/02
    //  用 get_obj_here 取代 obj 的 pfx 判定
    // 以此避免特定位置抓不到 obj name 的不顯示名稱 ( eg: look 2.sword ,
    // look 3.wa )
    /*
    for ( obj = ch->carrying; obj; obj = obj->next_content )
    {
            if ( can_see_obj( ch, obj ) )
            {
                    pdesc = get_pfxextra_descr( arg1, obj->extra_descr );
                    if ( pdesc )
                    {
                            send_to_char( obj->short_descr , ch);
                            send_to_char( " - ", ch);
                            send_to_char( obj->material->short_descr, ch);
                            send_to_char("\n\r", ch);
                            send_to_char( pdesc, ch );
                            return;
                    }

                    pdesc = get_pfxextra_descr( arg1,
    obj->pIndexData->extra_descr ); if ( pdesc )
                    {
                            send_to_char( obj->short_descr , ch);
                            send_to_char( " - ", ch);
                            send_to_char( obj->material->short_descr, ch);
                            send_to_char("\n\r", ch);
                            send_to_char( pdesc, ch );
                            return;
                    }
            }

            if ( is_pfxname( arg1, obj->name ) )
            {
                    send_to_char( obj->short_descr , ch);
                    send_to_char( " - ", ch);
                    send_to_char( obj->material->short_descr, ch);
                    send_to_char("\n\r", ch);
                    send_to_char( obj->description, ch );
                    send_to_char( "\n\r", ch );
                    return;
            }
    }

    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
    {
            if ( can_see_obj( ch, obj ) )
            {
                    pdesc = get_pfxextra_descr( arg1, obj->extra_descr );
                    if ( pdesc )
                    {
                            send_to_char( obj->short_descr , ch);
                            send_to_char( " - ", ch);
                            send_to_char( obj->material->short_descr, ch);
                            send_to_char("\n\r", ch);
                            send_to_char( pdesc, ch );
                            return;
                    }

                    pdesc = get_pfxextra_descr( arg1,
    obj->pIndexData->extra_descr ); if ( pdesc )
                    {
                            send_to_char( obj->short_descr , ch);
                            send_to_char( " - ", ch);
                            send_to_char( obj->material->short_descr, ch);
                            send_to_char("\n\r", ch);
                            send_to_char( pdesc, ch );
                            return;
                    }
            }

            if ( is_pfxname( arg1, obj->name ) )
            {
                    send_to_char( obj->short_descr , ch);
                    send_to_char( " - ", ch);
                    send_to_char( obj->material->short_descr, ch);
                    send_to_char("\n\r", ch);
                    send_to_char( obj->description, ch );
                    send_to_char( "\n\r", ch );
                    return;
            }
    }*/
    if ((obj = get_obj_here(ch, arg1))) {

        pdesc = get_pfxextra_descr(arg1, obj->extra_descr);
        if (pdesc) {
            send_to_char(obj->short_descr, ch);
            send_to_char(" - ", ch);
            send_to_char(obj->material->short_descr, ch);
            send_to_char("\n\r", ch);
            send_to_char(pdesc, ch);
            return;
        }

        pdesc = get_pfxextra_descr(arg1, obj->pIndexData->extra_descr);
        if (pdesc) {
            send_to_char(obj->short_descr, ch);
            send_to_char(" - ", ch);
            send_to_char(obj->material->short_descr, ch);
            send_to_char("\n\r", ch);
            send_to_char(pdesc, ch);
            return;
        }

        send_to_char(obj->short_descr, ch);
        send_to_char(" - ", ch);
        send_to_char(obj->material->short_descr, ch);
        send_to_char("\n\r", ch);
        send_to_char(obj->description, ch);
        send_to_char("\n\r", ch);
        return;
    }

    if (!str_prefix(arg1, "north"))
        door = 0;
    else if (!str_prefix(arg1, "east"))
        door = 1;
    else if (!str_prefix(arg1, "south"))
        door = 2;
    else if (!str_prefix(arg1, "west"))
        door = 3;
    else if (!str_prefix(arg1, "up"))
        door = 4;
    else if (!str_prefix(arg1, "down"))
        door = 5;
    else {
        send_to_char("[33m你在這裡沒看到那玩意兒。[m\n\r", ch);
        return;
    }

    /* 'look direction' */
    if (!(pexit = ch->in_room->exit[door])) {
        send_to_char("[33m那裡沒什麼特別的東西。[m\n\r", ch);
        return;
    }

    if (pexit->description && pexit->description[0] != '\0')
        send_to_char(pexit->description, ch);
    else
        send_to_char("[33m那裡沒什麼特別的東西。[m\n\r", ch);

    if (pexit->keyword && pexit->keyword[0] != '\0' &&
        pexit->keyword[0] != ' ') {
        if (IS_SET(pexit->exit_info, EX_BASHED))
            act("[36m$d被撞壞了。[m", ch, NULL, pexit->keyword, TO_CHAR);
        else if (IS_SET(pexit->exit_info, EX_CLOSED))
            act("[36m$d關了。[m", ch, NULL, pexit->keyword, TO_CHAR);
        else if (IS_SET(pexit->exit_info, EX_ISDOOR))
            act("[36m$d是開著的。[m", ch, NULL, pexit->keyword, TO_CHAR);
    }

    return;
}

void do_examine(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("[33m要檢查什麼物品？[m\n\r", ch);
        return;
    }

    if ((obj = get_obj_here(ch, arg))) {
        if (!obj)
            return;
        int diff;
        // 移除以 do_look 來顯示物品名及材質 2022/02/01
        // do_look( ch, argument );
        send_to_char(obj->short_descr, ch);
        send_to_char(" - ", ch);
        send_to_char(obj->material->short_descr, ch);
        send_to_char("\n\r", ch);
        send_to_char(obj->description, ch);
        send_to_char("\n\r", ch);
        // 移除 serial 顯示 2022/02/02
        // sprintf(buf,"物品序號：%ld\n\r", (long)obj->serial);
        // send_to_char(buf, ch);
        // 新增 obj_act examine , examine 用在部分 item type 上可能會與 look_in
        // 重覆 2022/02/12
        if (IS_SET(obj->pIndexData->progtypes, OBJ_ACT)) {
            oprog_act_trigger("examine", ch, obj);
        }
        switch (obj->item_type) {
        default:
            break;

        case ITEM_LIGHT:
            if (obj->value[2] != -1) {
                sprintf(buf, "看來還能使用 %d 小時。\n\r", obj->value[2]);
                send_to_char(buf, ch);
            }
            break;

        /* 將case armor 及 weapon 分開判定 2021/12/13 */
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
            /* 新增 weapon type 顯示 2021/12/13 */
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
            else if (obj->value[3] == 16) // add at 2022/05/06
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
        case ITEM_CONTAINER:
        case ITEM_CORPSE_NPC:
        case ITEM_CORPSE_PC:
        case ITEM_FOUNTAIN:
            send_to_char("你往裡面看的時候，你看到：\n\r", ch);
            sprintf(buf, "in %s", arg);
            do_look(ch, buf);
            break;

        case ITEM_ARROW:
            if (obj->value[0] <= 0) {
                // 增加 -1 的情況 2023/01/05
                if (obj->value[0] == -1)
                    send_to_char("[36m裡面箭矢的數量有如浩瀚的小宇宙。["
                                 "m\n\r",
                                 ch);
                else
                    send_to_char("[36m那是空的。[m\n\r", ch);
                break;
            }
            sprintf(buf, "那裡面還有%d支箭。\n\r", obj->value[0]);
            send_to_char(buf, ch);
            break;
        case ITEM_AMMO: // add at 2022/05/06
            if (obj->value[0] <= 0) {
                // 增加 -1 的情況 2023/01/05
                if (obj->value[0] == -1)
                    send_to_char("[36m裡面彈藥的數量有如浩瀚的小宇宙。["
                                 "m\n\r",
                                 ch);
                else
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
        case ITEM_HORSE:
            send_to_char("你的馬身上放著：\n\r", ch);
            sprintf(buf, "in %s", arg);
            do_look(ch, buf);
            break;
        }
    }

    // 沒有叫作 arg 的東西在房間中 2022/02/02
    if (!obj)
        send_to_char("[33m你在這裡沒看到那玩意兒。[m\n\r", ch);

    return;
}

/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits(CHAR_DATA *ch, char *argument) {
    EXIT_DATA *pexit;
    extern char *const dir_name[];
    char buf[MAX_STRING_LENGTH];
    int door;
    bool found;
    bool fAuto;

    buf[0] = '\0';
    fAuto = !str_cmp(argument, "auto");

    if (!check_blind(ch))
        return;

    /*If (!fAuto)
    {
      if ( in_room->roomprogs && ( in_room->progtypes & ROOM_MASK )
      && !IS_SET( ch->act, PLR_WIZINVIS ) )
      rprog_leave_trigger( in_room, ch, door );
    }*/

    strcpy(buf, fAuto ? "[" HIW "出口" NOR ":" : HIW "明顯的出口" NOR ":\n\r");

    found = FALSE;
    for (door = 0; door <= 5; door++) {
        if ((pexit = ch->in_room->exit[door]) && pexit->to_room &&
            !IS_SET(pexit->exit_info, EX_CLOSED)) {
            found = TRUE;
            if (fAuto) {
                if (!IS_SET(pexit->exit_info, EX_INVIS)) {
                    strcat(buf, " " HIC);
                    strcat(buf, dir_name[door]);
                    strcat(buf, NOR);
                } else if (IS_IMMORTAL(ch)) {
                    strcat(buf, " " HIB);
                    strcat(buf, dir_name[door]);
                    strcat(buf, NOR);
                }
            } else {
                sprintf(buf + strlen(buf), "%-5s － %s\n\r",
                        capitalize(dir_name[door]),
                        room_is_dark(pexit->to_room) ? "這邊太暗了"
                                                     : pexit->to_room->name);
            }
        }
    }

    if (!found)
        strcat(buf, fAuto ? " " HIC "無" NOR : HIC "無" NOR "。\n\r");

    if (fAuto)
        strcat(buf, "]\n\r");

    send_to_char(buf, ch);
    return;
}

char *stat_color(int cur, int max) {
    int ratio;
    if (max == 0)
        return HIR;
    ratio = cur * 100 / max;
    if (ratio < 30)
        return HIR;
    else if (ratio < 60)
        return HIY;
    else if (ratio <= 100)
        return HIG;
    else
        return HIW;
}

char *gift_color(int cur, int ori) {
    if (cur > ori)
        return HIC;
    else if (cur < ori)
        return HIR;
    else
        return HIW;
}

char *alignment_color(int align) {
    // 修改 align color 區間與字串變化相符 2023/01/16
    if (align > 799)
        return HIW;
    else if (align > 299)
        return HIC;
    else if (align > -300)
        return HIG;
    else if (align > -600)
        return HIY;
    else
        return HIR;
}

void do_elescore(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (!(victim = get_char_room(ch, arg))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }
    if (!(victim->pIndexData != NULL &&
          IsSorGateMob(victim->pIndexData->vnum))) {
        send_to_char("他不是召喚物.\n\r", ch);
        return;
    }
    // 修改為召喚物 remb 的角色 2022/12/20
    // if(victim->master != ch){
    if (victim->rembch != ch) {
        send_to_char("別想偷虧別人召喚物的資料\n\r", ch);
        return;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    MOB_WEAKNESS *pWeak = NULL;

    char rgLore[MAX_STRING_LENGTH];
    char rgBuf[MAX_STRING_LENGTH];
    char rgSec1[200], rgSec2[200], rgSec3[200];
    char rgSec4[200], rgSec5[200], rgSec6[500];

    rgLore[0] = 0;
    sprintf(rgBuf, "你的召喚物向你報告他的狀態。\n\n\r");
    strcat(rgLore, rgBuf);

    // Mob basic Info
    sprintf(rgBuf, "☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n\r");
    strcat(rgLore, rgBuf);
    sprintf(rgBuf,
            "%s , %s:\n\r種族：%s(%s)  等級：%2d  技能等級：%d  性別：%s  "
            "陣營：%d\n\r",
            victim->short_descr, victim->name, race_table[victim->race].cname,
            race_table[victim->race].name, victim->level,
            GetGeneralMobSklv(victim),
            victim->sex == SEX_MALE     ? "男"
            : victim->sex == SEX_FEMALE ? "女"
                                        : "中",
            victim->alignment);
    strcat(rgLore, rgBuf);

    // Five Basic Attributes
    sprintf(rgBuf,
            "力量：%2d/%2d  智力：%2d/%2d  智慧：%2d/%2d   敏捷：%2d/%2d   "
            "體質：%2d/%2d \n\r",
            get_curr_str(victim), get_max_str(victim), get_curr_int(victim),
            get_max_int(victim), get_curr_wis(victim), get_max_wis(victim),
            get_curr_dex(victim), get_max_dex(victim), get_curr_con(victim),
            get_max_con(victim));
    strcat(rgLore, rgBuf);

    // HP/MP/MV Info
    sprintf(rgBuf, "生命力：%4d/%4d   魔法力：%4d/%4d  移動力：%4d/%4d\n\r",
            victim->hit, victim->max_hit, victim->mana, victim->max_mana,
            victim->move, victim->max_move);

    strcat(rgLore, rgBuf);

    // Fighting Related
    strcat(rgLore, "\n\r戰鬥能力資料：\n\r");
    sprintf(rgSec1, "%5d", get_hitroll(victim, WEAR_WIELD));
    sprintf(rgSec3, "%4d", get_damroll(victim, WEAR_WIELD));
    sprintf(rgBuf, "主要武器命中：%s   主要武器傷害：%s\n\r", rgSec1, rgSec3);
    strcat(rgLore, rgBuf);

    if (get_eq_char(victim, WEAR_WIELD_2)) {
        sprintf(rgSec2, "%5d", get_hitroll(victim, WEAR_WIELD_2));
        sprintf(rgSec4, "%4d", get_damroll(victim, WEAR_WIELD_2));
        sprintf(rgBuf, "次要武器命中：%s   次要武器傷害：%s\n\r", rgSec2,
                rgSec4);
        strcat(rgLore, rgBuf);
    }

    sprintf(rgSec4, "%5d", victim->pp);
    sprintf(rgSec5, "%4d", victim->apdr);
    sprintf(rgSec6, "%4d", victim->amdr);
    sprintf(rgBuf, "    魔法潛力：%s   物理傷害：%s  魔法傷害：%s\n\r", rgSec4,
            rgSec5, rgSec6);
    strcat(rgLore, rgBuf);

    // Physical Resistance
    strcat(rgLore, "\n\r防禦基本資料：\n\r");
    sprintf(rgSec4, "%4d", victim->pdr);
    sprintf(rgSec5, "%4d", victim->pad);

    sprintf(rgBuf, "    護甲防衛：%5d   物理抗性：%s  物理障壁：%s\n\r",
            GET_AC(victim), rgSec4, rgSec5);
    strcat(rgLore, rgBuf);

    // Dodge and Magical Resistance
    sprintf(rgSec4, "%4d", victim->mdr);
    sprintf(rgSec5, "%4d", victim->mad);

    sprintf(rgBuf, "    閃避能力：%5d   魔法抗性：%s  魔法障壁：%s\n\r",
            GET_DODGE(victim), rgSec4, rgSec5);
    strcat(rgLore, rgBuf);

    // Magical Resistance in Elemental
    strcat(rgLore, "\n\r元素魔法抗性：\n\r");
    sprintf(rgSec6, "    抗魔法：%4d   抗炎燄：%4d   抗翔風：%4d\n\r",
            GET_SVS(victim), NEW_GET_SVS(victim, MAGIC_FIRE),
            NEW_GET_SVS(victim, MAGIC_WIND));
    strcat(rgLore, rgSec6);

    sprintf(rgSec6, "    抗雷閃：%4d   抗冰霜：%4d   抗大地：%4d\n\r",
            NEW_GET_SVS(victim, MAGIC_LIGHTNING),
            NEW_GET_SVS(victim, MAGIC_COLD), NEW_GET_SVS(victim, MAGIC_EARTH));
    strcat(rgLore, rgSec6);

    strcat(rgLore, "\n\r特殊魔法抗性：\n\r");
    sprintf(rgSec4, "    抗龍息：%4d   抗神聖：%4d   抗闇黑：%4d\n\r",
            NEW_GET_SVS(victim, MAGIC_BREATH), NEW_GET_SVS(victim, MAGIC_SAINT),
            NEW_GET_SVS(victim, MAGIC_DARK));
    strcat(rgLore, rgSec4);

    // Weakness
    strcat(rgLore, "\n\r對特殊技能的抗性專長及弱點：\n\r");
    for (pWeak = victim->weakness, rgBuf[0] = 0; pWeak != NULL;
         pWeak = pWeak->next) {
        sprintf(rgSec6, "[%20s] 技能命中加成 %3d%%，傷害加成 %3d%%。\n\r",
                pWeak->name, pWeak->percent - 100, pWeak->damage - 100);
        strcat(rgBuf, rgSec6);
    }
    strcat(rgLore, rgBuf);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (victim->affected) {

        AFFECT_DATA *paf;
        int nAffectCnt = 0;

        for (paf = victim->affected; paf; paf = paf->next) {
            if (paf->deleted)
                continue;

            if (nAffectCnt == 0)
                strcat(rgLore, "正受到以下特殊影響：\n\r");
            nAffectCnt++;

            sprintf(rgBuf, "特效：「" HIC "%s" NOR "(%s)」",
                    skill_table[paf->type].cname, skill_table[paf->type].name);
            strcat(rgLore, rgBuf);

            strcat(rgLore, "(");
            if (strcmp(c_affect_loc_name(paf->location), "無")) {
                sprintf(rgBuf, "影響" HIY "%s" NOR,
                        c_affect_loc_name(paf->location));
                strcat(rgLore, rgBuf);
            }
            if (paf->modifier) {
                sprintf(rgBuf, " %s%d " NOR "點  ",
                        paf->modifier < 0
                            ? HIU "-"
                            : (paf->modifier == 0 ? HIW : HIC "+"),
                        paf->modifier < 0 ? -paf->modifier : paf->modifier);
                strcat(rgLore, rgBuf);
            }
            if (paf->duration >= 0) {
                sprintf(rgBuf, "尚有 " HIG "%d" NOR " 小時的效用。",
                        paf->duration);
                strcat(rgLore, rgBuf);
            } else {
                sprintf(rgBuf, "持續作用中。");
                strcat(rgLore, rgBuf);
            }
            strcat(rgLore, ")");

            strcat(rgLore, "\n\r");
        }
    }

    strcat(rgLore, "\n\r");
    send_to_char(rgLore, ch);
}

void do_score(CHAR_DATA *ch, char *argument) {
    AFFECT_DATA *paf;
    char buf[BUF_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 8];
    char z_buf[MAX_INPUT_LENGTH];
    char key[MAX_INPUT_LENGTH];
    CHAR_DATA *vch = NULL;
    CHAR_DATA *victim = NULL;
    int race;
    int z_t = 0;
    buf1[0] = '\0';
    z_buf[0] = '\0';

    argument = one_argument(argument, z_buf);
    if (IS_NPC(ch))
        return;
    if (!z_buf || z_buf[0] == '\0') {
        victim = ch;
        sprintf(key, "all");
    } else if (!str_cmp(z_buf, "b") || !str_cmp(z_buf, "f") ||
               !str_cmp(z_buf, "a")) {
        victim = ch;
        sprintf(key, z_buf);
    } else {
        if (!(victim = get_char_world(ch, z_buf))) {
            send_to_char("這裡沒有這個人。\n\r", ch);
            return;
        }

        if ((!IS_SET(victim->act, PLR_HORSE) || victim != ch->pcdata->horse ||
             victim->in_room != ch->in_room) &&
            (ch->desc && ch->desc->original != victim) && victim != ch &&
            !IS_IMMORTAL(ch)) {
            send_to_char("你不能看別人的資料。\n\r", ch);
            return;
        }
        one_argument(argument, key);
        if (!key || key[0] == '\0')
            sprintf(key, "all");
    }
    /*
    ▁▁▂▂▃▃▄▄▅▅▆▆▇▇█傳 說 中 的 一 頁█▇▇▆▆▅▅▄▄▃▃▂▂▁▁
    <PKer> <兇手> <小偷> Zcecil, Raidiant Spirit  翻譯之神
    性別：男性    種族：神          PK紀錄：  12勝 / 129敗
    等級：39      職業：鍊金師      年齡： 39歲（真實時間 178小時）
    力量：30  智力：30  智慧：30  敏捷：30  體格：30
    物品：1000/1000 件, 重量：1000000/1000000 公斤    分頁：20 行
    生命力：99999/99999 點, 魔力：99999/99999 點, 體力：99999/99999 點
    Wimpy ：99999 點,       復活點數： 50 點
    金錢  ：2147283647 元,  存款：2147483647 元
    Autogold: 是    Autoloot: 是      Autosac: 是    Autoexit: 是
    Wizinvis: 否    Holylight: 是
    陣  營：990, 你有如天使般善良.
    經驗值：12345678 / MAX       練習點數：1234 點
    命中率：1222+22 點       傷害力：1231+21 點      抗魔力：-123 點
    勇  氣：999/299 點, 影響命中率 10 點, 傷害力 10 點.
    防禦力：-9999 點, 你的武裝真不可思議！
    你正站著.你現在不適合作徒手戰鬥.
    你正被城市守衛(Cityguard)獵殺.
    你正獵殺著市長(Mayor).
    你身上正作用著以下法術：
    技能：全力防禦(defence), 影響命中率 -3 點, 還有 1111 小時的效用.
    ☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
                        IS_SET( wch->act, PLR_KILLER   ) ? "（"RED"兇手"NOR"）"
    : "", IS_SET( wch->act, PLR_THIEF    ) ? "（"RED"小偷"NOR") "  : "", IS_SET(
    wch->act, PLR_REGISTER ) ? "（"HIG"PK"NOR"）"
    */
    /*
  <PK>Reticent, 玩家別叫我幫忙拿裝備 QQ  Reticent(Reticent)
  你是一個 20 歲的 42 級男性人類族狂戰士傳奇。(上線時間 50 小時)
  你的幫派是: 光明聖使團。        威望:10, 你開始慢慢嶄露頭角。
  力量: 17/16  智力: 14/17  智慧: 16/16  敏捷: 15/17  體質: 17/22
  生命力: 589/1856      魔法力: 200/1000      移動力: 1061/1061
  PK 紀錄:  0 勝/ 0 敗    經驗值：6311435/MAX        練習點數: 351
  陣   營: -1000, 你為眾人所詛咒，簡直就是撒旦的走狗。

  你目前的作戰型態為: 停止攻擊。    生命力少於   0 點時自動逃跑。
  陣型: 無      現在狀態: 口渴、肚子餓了、休息、徒手搏鬥
  物品: 56/72   負重: 414/500 公斤  金錢: 1000000 元  存款: 1000000 元
  分頁: 20 行   Autoexit: 是   Autoloot: 否   Autogold: 是 Autosac: 否

  主要武器命中點數: 52   閃  避  率:   14, 你比蝸牛快不了多少。
  主要武器攻擊點數: 69   護甲防衛力:  -96, 你裝備起來。
  次要武器命中點數: 無   豁  免  率: 法術：-14  風系：-11  火系：-9
  次要武器攻擊點數: 無               水系：-9   地系：-9   吐息：-9
  魔  法    潛  力:  0

  你是一個 120 歲的 43 級中性神族巫師。(上線時間 1163 小時)，被大神信任為第
  46級。 你的幫派是：凡特錫學院。      威望：4, 你是個無名小卒。 力量: 25/25
  智力: 21/21  智慧: 21/21  敏捷: 24/24  體質: 21/21 生命力: 1515/1515  魔法力:
  2025/2025  移動力: 1624/1624 經驗值：1244880/MAX 練習點數: 424
  技能經驗值：145499/145500/1400000
  陣營：60, 你謹守中立本分。

  你在休息。
  你目前的作戰型態為迅捷施法.     生命力少於 0 點時自動逃跑
  現在狀態：
  物品: 60/1000  負重 83/1000000 公斤  金錢：207457814 元  存款：106011205 元
  分頁: 23 行。 Autoexit: 是  Autoloot: 否  Autogold: 是  Autosac: 否
  Wizinvis: 否(0)  Holylight: 是
  主要武器命中點數：  44　閃  避  率：-1531,　你慢一點! 別讓你的影子跟丟了！！！
  主要武器攻擊點數：  44  護甲防衛力： -274,　你穿著重型裝備。
  次要武器命中點數：　無　法術：-441　風系：-227　火系：-230
  次要武器攻擊點數：　無　水系：-128　地系：-131　吐息： -30
  魔  法    潛  力：1015　雷系： -22　
  [輸入指令 (c)繼續, (r)重印本頁, (b)上頁, (h)說明, (q)離開, 或按 ENTER]:
  物　理 傷 害　力：   5　抗　物 理 傷　害：1969
  魔　法 傷 害　力：   0　抗　魔 法 傷　害：1985

  */

    race = ((victim->temp_race) ? victim->temp_race - 1 : victim->race);
    if (IS_NPC(victim))
        return;

    if (!str_cmp(key, "all") || !str_cmp(key, "b")) {
        sprintf(buf,
                "\n\r" HIW "▁▁▂▂▃▃▄▄▅▅▆▆▇▇█ " HIC "傳 " HIP "說 " HIR "中 " HIY
                "的 " HIU "一 " HIG "頁" HIW " █▇▇▆▆▅▅▄▄▃▃▂▂▁▁" NOR "\n\r");
        // 角色名稱
        strcat(buf1, buf);
        sprintf(buf, HIW "%s%s%s" HIY "%s" NOR ", " NOR "%s" NOR " %s\n\r",
                IS_SET(victim->act, PLR_KILLER) ? HIR "<兇手>" NOR : "",
                IS_SET(victim->act, PLR_THIEF) ? HIP "<小偷>" NOR : "",
                IS_SET(victim->act, PLR_REGISTER) ? HIG "<PK>" NOR : "",
                victim->name, IS_NPC(victim) ? "" : victim->short_descr,
                victim->pcdata->title);
        strcat(buf1, buf);

        // 信任等級 , 移到角色名下方 2022/12/22
        if (get_trust(victim) != victim->level) {
            sprintf(buf, "大神信任為第 " HIC "%d" NOR " 級。\n\r",
                    get_trust(victim));
            strcat(buf1, buf);
        } else
            strcat(buf1, "\n\r");

        sprintf(buf,
                "你是一個 " HIC "%d" NOR " 歲的 " HIY "%d" NOR " 級" CYN
                "%s" NOR "性" HIW "%s%s" NOR "(%s %s) - (上線" HIC " %d " NOR
                "小時)\n\r",
                get_age(victim), victim->level,
                (victim->sex == SEX_MALE)     ? "男"
                : (victim->sex == SEX_FEMALE) ? "女"
                                              : "中",
                race_table[race].cname, class_table[victim->class].cname,
                race_table[race].name, class_table[victim->class].name,
                //( victim->level == 41 ? "英雄" : victim->level == 42 ? "傳奇"
                //: ""),
                (victim->played / 3600));

        strcat(buf1, buf);

        // 幫派
        if (victim->clan) {
            sprintf(buf, "你的幫派是：%s。", victim->clan->name);
            strcat(buf1, buf);
        } else {
            sprintf(buf, "你現在沒有加入幫派。");
            strcat(buf1, buf);
        }
        // 威望
        if (victim->level >= LEVEL_HERO) {
            sprintf(buf, "      威望：" HIW "%d" NOR ", ",
                    victim->pcdata->respect);
            strcat(buf1, buf);
        }
        // strcat(buf1, "你");
        if (victim->pcdata->respect < 0)
            strcat(buf1, HIC "你就算死在路邊也沒有人會發現。" NOR "\n\r");
        else if (victim->pcdata->respect < 20)
            strcat(buf1, HIW "你是個無名小卒。" NOR "\n\r");
        else if (victim->pcdata->respect < 40)
            strcat(buf1, HIG "你慢慢開始嶄露頭角。" NOR "\n\r");
        else if (victim->pcdata->respect < 60)
            strcat(buf1, "你是號人物。\n\r");
        else if (victim->pcdata->respect < 80)
            strcat(buf1, "你威震一方。\n\r");
        else if (victim->pcdata->respect < 100)
            strcat(buf1, "你的事跡被吟遊詩人在酒吧傳誦。\n\r");
        else if (victim->pcdata->respect < 150)
            strcat(buf1, HIP "你的聲名無人不知，無人不曉。" NOR "\n\r");
        else if (victim->pcdata->respect < 300)
            strcat(buf1, HIR "你是個已被神化的傳奇人物。" NOR "\n\r");
        else
            strcat(buf1, RED "你是自天神以降的第一人。" NOR "\n\r");

        // 功勳
        sprintf(buf, "你的功勳：" HIG "%d" NOR "  ", victim->pcdata->merit);
        strcat(buf1, buf);
        if (victim->pcdata->merit <= 0)
            strcat(buf1, HIC "你還沒建立任何戰功。" NOR "\n\r");
        else if (victim->pcdata->merit < 20000)
            strcat(buf1, "你開始建立戰功。" NOR "\n\r");
        else if (victim->pcdata->merit < 40000)
            strcat(buf1, "你是號人物。\n\r");
        else if (victim->pcdata->merit < 80000)
            strcat(buf1, "你的戰果豐碩。\n\r");
        else if (victim->pcdata->merit < 100000)
            strcat(buf1, HIY "你的戰功被吟遊詩人在酒吧傳誦。" NOR "\n\r");
        else if (victim->pcdata->merit < 200000)
            strcat(buf1, HIP "你的戰功無人不知，無人不曉。" NOR "\n\r");
        else if (victim->pcdata->merit < 500000)
            strcat(buf1, HIR "你的事蹟有如戰神第二。" NOR "\n\r");
        else
            strcat(buf1, HIW "你是自天神以降的第一人。" NOR "\n\r");

        // 屬性 , 角色體型 2022/12/23
        sprintf(buf,
                "力量: %s%d" HIW "/%d%s  智力: %s%d" HIW "/%d%s  智慧: %s%d" HIW
                "/%d%s  敏捷: %s%d" HIW "/%d%s  體質: %s%d" HIW
                "/%d%s  體型: " HIY "%d" NOR "\n\r",
                gift_color(get_eff_str(victim), get_max_str(victim)),
                get_curr_str(victim), get_max_str(victim), NOR,
                gift_color(get_eff_int(victim), get_max_int(victim)),
                get_curr_int(victim), get_max_int(victim), NOR,
                gift_color(get_eff_wis(victim), get_max_wis(victim)),
                get_curr_wis(victim), get_max_wis(victim), NOR,
                gift_color(get_eff_dex(victim), get_max_dex(victim)),
                get_curr_dex(victim), get_max_dex(victim), NOR,
                gift_color(get_eff_con(victim), get_max_con(victim)),
                get_curr_con(victim), get_max_con(victim), NOR,
                get_curr_size(victim) // 加入體型 2022/12/22
        );
        strcat(buf1, buf);

        // hp mana move
        /*sprintf( buf,
        "你有[1m%d[m/[1m%d[m點生命力、[1m%d[m/[1m%d[m點魔法力、[1m%d[m/[1m%d[m點移動力、[1m%d[m點練習點數。\n\r",
                  ch->hit,  ch->max_hit,
                  ch->mana, ch->max_mana,
                  ch->move, ch->max_move,
                  ch->practice );*/
        sprintf(buf,
                "生命力: %s%d%s/" HIG "%d" NOR "    魔法力: %s%d%s/" HIG
                "%d" NOR "    移動力: %s%d%s/" HIG "%d" NOR "\n\r",
                stat_color(victim->hit, victim->max_hit), victim->hit, NOR,
                victim->max_hit, stat_color(victim->mana, victim->max_mana),
                victim->mana, NOR, victim->max_mana,
                stat_color(victim->move, victim->max_move), victim->move, NOR,
                victim->max_move);
        strcat(buf1, buf);

        /*sprintf( buf,
          "你帶著有[1m%d[m/[1m%d[m項物品，共重[1m%d[m/[1m%d[m公斤。\n\r",
          ch->carry_number, can_carry_n( ch ),
          ch->carry_weight, can_carry_w( ch ) ); */
        if (victim->level >= (LEVEL_IMMORTAL - 1)) {
            sprintf(z_buf, "%s", "MAX");
        } else {
            // 20060722 modify by jye
            // 新增EXP_PER_LEVEL ppl_class變數
            z_t = EXP_PER_LEVEL(victim->level, victim->class) *
                  (victim->level + 1);
            sprintf(z_buf, "%d", z_t);
        }
        sprintf(buf,
                "經驗值：%s%d" NOR "/" HIG "%s" NOR "   練習點數: " HIG "%d" NOR
                "",
                (victim->exp >= z_t) ? CYN : HIC, victim->exp, z_buf,
                victim->practice);
        strcat(buf1, buf);

        // pk紀錄
        if (!IS_NPC(victim) && IS_SET(victim->act, PLR_REGISTER)) {
            sprintf(buf, "   PK 紀錄： " HIC " %d 勝/" HIR " %d 敗" NOR "\n\r",
                    victim->pcdata->pkwin, victim->pcdata->pklose);
            strcat(buf1, buf);
        } else
            strcat(buf1, "\n\r");

        // 技能經驗
        if (victim->class >= WIZARD) {
            sprintf(buf,
                    "技能經驗值：" HIC "%d" NOR "/" HIY "%d" NOR "/" HIW
                    "%d" NOR "  (功勳提升你" HIG " %d " NOR "上限點)\n\r",
                    get_eff_skill_exp(victim), get_skill_exp(victim),
                    get_max_skill_exp(victim),
                    get_merit_bonus_max_skexp(victim));
            strcat(buf1, buf);
        }
        // 陣營
        if (!IS_NPC(victim) && victim->level >= 10) {
            sprintf(buf, "陣營：%s%d" NOR "  ",
                    alignment_color(victim->alignment), victim->alignment);
            strcat(buf1, buf);
        }

        // strcat(buf1,"你");
        //  修改區間數值與色碼, 增加從中立轉換前 20 align 變色 2023/01/16
        if (victim->alignment > 799)
            strcat(buf1,
                   HIC "你有如天使般善良。" NOR "                   \n\r");
        else if (victim->alignment > 599)
            strcat(buf1,
                   HIW "你的品德有如聖人般公正。" NOR "             \n\r");
        else if (victim->alignment > 315)
            strcat(buf1,
                   HIG "你奉公守法，行事正義。" NOR "               \n\r");
        else if (victim->alignment > 284)
            strcat(buf1,
                   CYN "你對人還算仁慈。" NOR "                     \n\r");
        else if (victim->alignment > 149)
            strcat(buf1, "你對人還算仁慈。                          \n\r");
        else if (victim->alignment > -150)
            strcat(buf1, "你謹守中立本分。                          \n\r");
        else if (victim->alignment > -285)
            strcat(buf1, "你有一點卑鄙。                            \n\r");
        else if (victim->alignment > -300)
            strcat(buf1,
                   PUR "你有一點卑鄙。" NOR "                       \n\r");
        else if (victim->alignment > -316)
            strcat(buf1,
                   PUR "你目無法紀，行為邪惡。" NOR "               \n\r");
        else if (victim->alignment > -600)
            strcat(buf1,
                   HIP "你目無法紀，行為邪惡。" NOR "               \n\r");
        else if (victim->alignment > -800)
            strcat(buf1,
                   HIR "你是個十惡不赦的惡魔。" NOR "               \n\r");
        else
            strcat(buf1,
                   RED "你為眾人所詛咒，簡直就是撒旦的走狗。" NOR " \n\r");
        strcat(buf1, "\n\r");
        // 現在的行為
        switch (victim->position) {
        case POS_DEAD:
            strcat(buf1, "你" RED "死" NOR "了！！");
            break;
        case POS_MORTAL:
            strcat(buf1, "你" HIR "受了致命" NOR "傷。");
            break;
        case POS_INCAP:
            strcat(buf1, "你" HIB "失去行動能力" NOR "了。");
            break;
        case POS_STUNNED:
            strcat(buf1, "你" HIB "不省人事" NOR "了。");
            break;
        case POS_SLEEPING:
            strcat(buf1, "你" HIU "睡著" NOR "了。");
            break;
        case POS_RESTING:
            strcat(buf1, "你在" HIG "休息" NOR "。");
            break;
        case POS_STANDING:
            if (is_flying(victim)) // 加入 aff_flying 的判定 2021/11/05
                strcat(buf1, "你漂浮在這裡。");
            else
                strcat(buf1, "你站立在這裡。");
            break;
        case POS_FIGHTING:
            if (is_flying(victim)) // 加入 aff_flying 的判定 2021/11/05
                strcat(buf1, "你漂浮在這裡" HIY "戰鬥" NOR "著。");
            else
                strcat(buf1, "你站在這裡" HIY "戰鬥" NOR "著。");
            break;
        default:
            break;
        }
        // 新增 link char 顯示 2021/11/20
        if ((vch = victim->linking)) {
            sprintf(buf, HIU "你的靈魂與" NOR "%s" HIU "產生共鳴" NOR ".\n\r",
                    vch->short_descr);
            strcat(buf1, buf);
        } else {
            strcat(buf1, "\n\r");
        }
        // 作戰型態
        if (!IS_NPC(victim) /*&& victim->pcdata->autocombat != AUTOC_NORMAL*/) {
            strcat(buf1, "目前的作戰型態");
            switch (victim->pcdata->autocombat) {
            case AUTOC_NORMAL:
                strcat(buf1, "  普  通.");
                break;
            case AUTOC_STOP:
                strcat(buf1, HIR "停止攻擊." NOR);
                break;
            case AUTOC_DODGE:
                strcat(buf1, HIC "全力閃躲." NOR);
                break;
            case AUTOC_PARRY:
                strcat(buf1, HIG "全力守禦." NOR);
                break;
            case AUTOC_CONCENTRATION:
                strcat(buf1, HIY "專注祈禱." NOR);
                break;
            case AUTOC_CRITICAL:
                strcat(buf1, HIP "致命攻擊." NOR);
                break;
            case AUTOC_COOPERATE:
                strcat(buf1, HIW "群體作戰." NOR);
                break;
            case AUTOC_POWER:
                strcat(buf1, HIC "引發魔力." NOR);
                break;
            case AUTOC_INSTANT:
                strcat(buf1, HIY "迅捷施法." NOR);
                break;
            case AUTOC_CHARGE:
                strcat(buf1, HIY "衝刺突擊." NOR);
                break;
            case AUTOC_RELOAD: // 用槍械戰鬥時 自動上彈 2022/05/01
                strcat(buf1, HIB "快速填彈." NOR);
                break;
            default:
                strcat(buf1, HIU "  不  明." NOR);
                break;
            }
        }

        // sprintf( buf, "在生命值剩下[1m%d[m以下的時候會自動逃跑。\n\r",
        // ch->wimpy );
        /*sprintf( buf,
          "你有[1m%d[m點經驗值，有[1m%d[m枚金幣。\n\r",
          ch->exp,  ch->gold );
          */
        // 自動逃跑
        sprintf(buf, "            生命力少於 " CYN "%d" NOR " 點時自動逃跑\n\r",
                victim->wimpy);
        strcat(buf1, buf);

        // 現在狀態
        //  計算狀態顯示的個數, 太多則換行 2021/11/05
        int affect_count = 0;
        ;
        strcat(buf1, "現在狀態：");
        // 將fist 及藏匿移至最前方 2021/11/05
        if (IS_SET(victim->act, PLR_FIST_FIGHT)) {
            sprintf(buf, "徒手搏鬥 ");
            strcat(buf1, buf);
            affect_count += 2;
        }
        if (IS_AFFECTED(victim, AFF_HIDE)) {
            sprintf(buf, HIB "藏匿" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_INVISIBLE)) {
            sprintf(buf, YEL "隱形" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        // 修改 Cond 的最大數值 從 48 變為 100 , 變更敘述的數值區間及增加敘述
        // 2023/01/13
        if (!IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] >= 70) {
            strcat(buf1, HIR "酒精中毒" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] < 70 &&
            victim->pcdata->condition[COND_DRUNK] >= 50 /*36*/) {
            strcat(buf1, HIR "爛醉" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] < 50 /*50*/
            && victim->pcdata->condition[COND_DRUNK] >= 20 /*10*/) {
            strcat(buf1, HIR "喝醉" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] < 20 /*10*/
            && victim->pcdata->condition[COND_DRUNK] > 0) {
            strcat(buf1, RED "微醺" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->level < LEVEL_IMMORTAL &&
            victim->pcdata->condition[COND_THIRST] < 40 /*15*/
            && victim->pcdata->condition[COND_THIRST] > 10 /*0*/) {
            strcat(buf1, HIY "口渴" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->level < LEVEL_IMMORTAL &&
            victim->pcdata->condition[COND_FULL] < 40 /*15*/
            && victim->pcdata->condition[COND_FULL] > 10 /*0*/) {
            strcat(buf1, HIP "肚子餓了" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->level < LEVEL_IMMORTAL &&
            victim->pcdata->condition[COND_THIRST] <= 10 &&
            victim->pcdata->condition[COND_THIRST] >= 0 /* == 0*/) {
            strcat(buf1, YEL "脫水" NOR " ");
            affect_count += 1;
        }
        if (!IS_NPC(victim) && victim->level < LEVEL_IMMORTAL &&
            victim->pcdata->condition[COND_FULL] <= 10 &&
            victim->pcdata->condition[COND_FULL] >= 0 /* == 0*/) {
            strcat(buf1, PUR "飢餓" NOR " ");
            affect_count += 1;
        }
        // 新增 中毒 , 生病 , 恐懼 , 瘋狂 , 麻痺等負面狀態 2021/11/05
        if (IS_AFFECTED(victim, AFF_POISON)) {
            sprintf(buf, HIG "中毒" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_DISEASE)) {
            sprintf(buf, HIU "生病" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_PARALYZED)) {
            if (affect_count >= 8) {
                strcat(buf1, "\n\r          ");
                affect_count = 0;
            }
            sprintf(buf, YEL "麻痺" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_FEAR)) {
            if (affect_count >= 8) {
                strcat(buf1, "\n\r          ");
                affect_count = 0;
            }
            sprintf(buf, CYN "恐懼" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_CRAZY)) {
            if (affect_count >= 8) {
                strcat(buf1, "\n\r          ");
                affect_count = 0;
            }
            sprintf(buf, PUR "瘋狂" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_CURSE)) {
            if (affect_count >= 8) {
                strcat(buf1, "\n\r          ");
                affect_count = 0;
            }
            sprintf(buf, HIB "詛咒" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_MUTE)) {
            if (affect_count >= 8) {
                strcat(buf1, "\n\r          ");
                affect_count = 0;
            }
            sprintf(buf, HIY "沉默" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_AFFECTED(victim, AFF_SEAL_MAGIC)) {
            if (affect_count >= 8) {
                strcat(buf1, "\n\r          ");
                affect_count = 0;
            }
            sprintf(buf, HIC "封咒" NOR " ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        if (IS_SET(victim->act, PLR_RIDE)) {
            sprintf(buf, "座騎 ");
            strcat(buf1, buf);
            affect_count += 1;
        }
        strcat(buf1, "\n\r");
        sprintf(buf,
                "物品 " CYN "%d" NOR "/" HIC "%d" NOR "  負重 " CYN "%d" NOR
                "/" HIC "%d" NOR " 公斤    ",

                victim->carry_number, can_carry_n(victim), victim->carry_weight,
                can_carry_w(victim));
        strcat(buf1, buf);

        sprintf(buf, "金錢：" HIY "%d" NOR " 元  存款：" HIY "%d" NOR " 元\n\r",
                victim->gold, victim->bank);
        strcat(buf1, buf);

        // 分頁 pagelength
        if (!IS_NPC(victim))
            sprintf(buf, "分頁: " HIW "%d" NOR " 行。  ",
                    victim->pcdata->pagelen);
        else
            sprintf(buf, "。\n\r");
        strcat(buf1, buf);
        buf[0] = '\0';

        // 自動行為
        sprintf(
            buf, "Autoexit: %s  Autoloot: %s  Autogold: %s  Autosac: %s\n\r",
            (!IS_NPC(victim) && IS_SET(victim->act, PLR_AUTOEXIT)) ? HIW
                "是" NOR
                                                                   : "否",
            (!IS_NPC(victim) && IS_SET(victim->act, PLR_AUTOLOOT)) ? HIW
                "是" NOR
                                                                   : "否",
            (!IS_NPC(victim) && IS_SET(victim->act, PLR_AUTOGOLD)) ? HIW
                "是" NOR
                                                                   : "否",
            (!IS_NPC(victim) && IS_SET(victim->act, PLR_AUTOSAC)) ? HIW "是" NOR
                                                                  : "否");
        strcat(buf1, buf);
        // 神隱
        if (!IS_NPC(victim) && victim->level >= L_APP) {
            QMARK *qm = is_qmark(victim, "wizinvis");
            sprintf(buf, "Wizinvis: %s(%d)  Holylight: %s  \n\r",
                    (!IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS))
                        ? HIW "是" NOR
                        : "否",
                    (qm ? qm->v0 : 0),
                    (!IS_NPC(victim) && IS_SET(victim->act, PLR_HOLYLIGHT))
                        ? HIW "是" NOR
                        : "否");
            strcat(buf1, buf);
        } else
            strcat(buf1, "\n\r");
    }

    // 戰鬥數值 , score f
    if (!str_cmp(key, "all") || !str_cmp(key, "f")) {
        if (!str_cmp(key, "f")) {
            strcat(buf1, "你的戰鬥數值如下：\n\r");
        }

        if (ch->level >= 15) {
            // wp hr
            strcat(buf1, "主要武器");
            sprintf(buf, "命中點數：" HIG "%4d" NOR "　",
                    get_hitroll(victim, (get_eq_char(victim, WEAR_TWO_HAND)
                                             ? WEAR_TWO_HAND
                                             : WEAR_WIELD)));
            strcat(buf1, buf);

            // dc
            strcat(buf1, "閃  避  率：");
            if (ch->level >= LEVEL_HERO) {
                sprintf(buf, HIW "%5d" NOR ",　你", GET_DODGE(victim));
                strcat(buf1, buf);
                buf[0] = '\0';
            } else
                strcat(buf1, "你");
            if (GET_DODGE(victim) >= 200)
                strcat(buf1, "根本就是一塊岩石。");
            else if (GET_DODGE(victim) >= 100)
                strcat(buf1, "還能動真是奇蹟！");
            else if (GET_DODGE(victim) >= 0)
                strcat(buf1, "緩慢而痛苦地爬行著。");
            else if (GET_DODGE(victim) >= -50)
                strcat(buf1, "比蝸牛快不了多少。");
            else if (GET_DODGE(victim) >= -150)
                strcat(buf1, "終於學會走路。");
            else if (GET_DODGE(victim) >= -200)
                strcat(buf1, "抬頭挺胸，器宇軒昂。");
            else if (GET_DODGE(victim) >= -400)
                strcat(buf1, "的速度不錯。");
            else if (GET_DODGE(victim) >= -500)
                strcat(buf1, "行動非常敏捷。");
            else if (GET_DODGE(victim) >= -750)
                strcat(buf1, "動如脫兔，一洩千里。");
            else if (GET_DODGE(victim) >= -900)
                strcat(buf1, "一定是世界賽跑冠軍。");
            else if (GET_DODGE(victim) >= -1000)
                strcat(buf1, "速度快得沒人看得到。");
            else if (GET_DODGE(victim) >= -1200)
                strcat(buf1, "根本就像一陣旋風。");
            else if (GET_DODGE(victim) >= -1300)
                strcat(buf1, "快到可以追上自己的影子！");
            else
                strcat(buf1, "慢一點! 別讓你的影子跟丟了！！！");
            strcat(buf1, NOR "\n\r");

            // wp dr
            sprintf(buf, "主要武器攻擊點數：" HIG "%4d" NOR "  ",
                    get_damroll(victim, (get_eq_char(victim, WEAR_TWO_HAND)
                                             ? WEAR_TWO_HAND
                                             : WEAR_WIELD)));
            strcat(buf1, buf);

            // ac
            strcat(buf1, "護甲防衛力：");
            if (ch->level > 25) {
                sprintf(buf, HIW "%5d" NOR ",　你", GET_AC(victim));
                strcat(buf1, buf);
                buf[0] = '\0';
            } else
                strcat(buf1, "你");
            if (GET_AC(victim) >= 200)
                strcat(buf1, "比裸體的還誇張！");
            else if (GET_AC(victim) >= 100)
                strcat(buf1, "跟祼體沒什麼兩樣。");
            else if (GET_AC(victim) >= 0)
                strcat(buf1, "只穿著內衣內褲。");
            else if (GET_AC(victim) >= -50)
                strcat(buf1, "穿著普通的衣服。");
            else if (GET_AC(victim) >= -200)
                strcat(buf1, "多少有點裝備。");
            else if (GET_AC(victim) >= -400)
                strcat(buf1, "裝備起來。");
            else if (GET_AC(victim) >= -600)
                strcat(buf1, "的裝備不錯。");
            else if (GET_AC(victim) >= -800)
                strcat(buf1, "的裝備很好。");
            else if (GET_AC(victim) >= -1000)
                strcat(buf1, "穿著重型裝備。");
            else if (GET_AC(victim) >= -1100)
                strcat(buf1, "穿著超級的裝備。");
            else if (GET_AC(victim) >= -1200)
                strcat(buf1, "的裝備只應天上有。");
            else if (GET_AC(victim) >= -1400)
                strcat(buf1, "這樣的裝備天下無敵。");
            else if (GET_AC(victim) >= -1600)
                strcat(buf1, "穿著超級無敵黃金霹靂的裝備。");
            else
                strcat(buf1, "的裝備太誇張了，是用 bug 嗎？");
            strcat(buf1, NOR "\n\r");

            if (get_eq_char(victim, WEAR_WIELD_2)) {
                // wp2 hr
                sprintf(buf, "次要武器命中點數：" HIG "%4d" NOR "　",
                        get_hitroll(victim, WEAR_WIELD_2));
                strcat(buf1, buf);

                // svs , wind , fire , poison
                if (ch->level > 36) {
                    sprintf(buf,
                            "法術：" HIW "%4d" NOR "　風系：" HIW "%4d" NOR
                            "　火系：" HIW "%4d" NOR "　毒素：" HIW "%4d" NOR
                            "\n\r",
                            NEW_GET_SVS(victim, MAGIC_SPELL),
                            NEW_GET_SVS(victim, MAGIC_WIND),
                            NEW_GET_SVS(victim, MAGIC_FIRE),
                            NEW_GET_SVS(victim, MAGIC_POISON)); // 增加中毒每tick受到傷害顯示
                                                                // 2021/11/03
                    strcat(buf1, buf);
                } else
                    strcat(buf1, "\n\r");

                // wp2 dr
                sprintf(buf, "次要武器攻擊點數：" HIG "%4d" NOR "　",
                        get_damroll(victim, WEAR_WIELD_2));
                strcat(buf1, buf);

                // water , earth, breath
                if (ch->level > 36) {
                    sprintf(buf,
                            NOR "水系：" HIW "%4d" NOR "　地系：" HIW "%4d" NOR
                                "　吐息：" HIW "%4d" NOR "\n\r",
                            NEW_GET_SVS(victim, MAGIC_COLD),
                            NEW_GET_SVS(victim, MAGIC_EARTH),
                            NEW_GET_SVS(victim, MAGIC_BREATH));
                    strcat(buf1, buf);
                } else
                    strcat(buf1, "\n\r");
            } else {
                sprintf(buf, "次要武器命中點數：" HIG "　無" NOR "　");
                strcat(buf1, buf);

                if (ch->level > 36) {
                    sprintf(buf,
                            "法術：" HIW "%4d" NOR "　風系：" HIW "%4d" NOR
                            "　火系：" HIW "%4d" NOR "　毒素：" HIW "%4d" NOR
                            "\n\r",
                            NEW_GET_SVS(victim, MAGIC_SPELL),
                            NEW_GET_SVS(victim, MAGIC_WIND),
                            NEW_GET_SVS(victim, MAGIC_FIRE),
                            NEW_GET_SVS(victim, MAGIC_POISON)); // 增加中毒每tick受到傷害顯示
                                                                // 2021/11/03
                    strcat(buf1, buf);
                } else
                    strcat(buf1, "\n\r");

                sprintf(buf, "次要武器攻擊點數：" HIG "　無" NOR "　");
                strcat(buf1, buf);

                if (ch->level > 36) {
                    sprintf(buf,
                            NOR "水系：" HIW "%4d" NOR "　地系：" HIW "%4d" NOR
                                "　吐息：" HIW "%4d" NOR "\n\r",
                            NEW_GET_SVS(victim, MAGIC_COLD),
                            NEW_GET_SVS(victim, MAGIC_EARTH),
                            NEW_GET_SVS(victim, MAGIC_BREATH));
                    strcat(buf1, buf);
                } else
                    strcat(buf1, "\n\r");
            }
        }

        if (ch->level > 36) {
            sprintf(buf, "魔  法    潛  力：" HIP "%4d" NOR "　", victim->pp);
            strcat(buf1, buf);

            sprintf(buf,
                    NOR "雷系：" HIW "%4d" NOR "　光系：" HIW "%4d" NOR
                        "　闇系：" HIW "%4d" NOR "\n\r",
                    NEW_GET_SVS(victim, MAGIC_LIGHTNING),
                    NEW_GET_SVS(victim, MAGIC_SAINT),
                    NEW_GET_SVS(victim, MAGIC_DARK));
            strcat(buf1, buf);

            sprintf(buf, "物　理 傷 害　力：" HIP "%4d" NOR "　", victim->apdr);
            strcat(buf1, buf);

            sprintf(buf, "抗　物 理 傷　害：" HIP "%4d" NOR "\n\r",
                    victim->pdr);
            strcat(buf1, buf);

            sprintf(buf, "魔　法 傷 害　力：" HIP "%4d" NOR "　", victim->amdr);
            strcat(buf1, buf);

            sprintf(buf, "抗　魔 法 傷　害：" HIP "%4d" NOR "\n\r",
                    victim->mdr);
            strcat(buf1, buf);
        }
        // 將戰鬥記錄改到 score f 之下, 並加入完整顯示 2021/11/21
        // if ( !str_cmp( key, "all" ) || !str_cmp( key, "a" ) )
        if (!str_cmp(key, "f")) {
            strcat(buf1, "你的戰鬥記錄如下：\n\r");
            if ((vch = victim->tracking)) {
                sprintf(buf, YEL "你正在追蹤 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
                strcat(buf1, "\n\r");
            }
            if ((vch = victim->assassinating)) {
                sprintf(buf, RED "你正在暗殺 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
                strcat(buf1, "\n\r");
            }
            if ((vch = victim->hunting)) {
                sprintf(buf, HIR "你正在獵殺 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
                strcat(buf1, "\n\r");
            }
            if ((vch = victim->cooperating)) {
                sprintf(buf, HIW "你正在協助 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
                strcat(buf1, "\n\r");
            }
            if ((vch = victim->protecting)) {
                sprintf(buf, HIY "你正在保護 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
                strcat(buf1, "\n\r");
            }
            if ((victim->hunted)) {
                int hunt_num = 0;
                strcat(buf1, "" HIR "正在獵殺你：" NOR "\n\r");
                for (vch = victim->hunted; vch; vch = vch->next_in_hunt) {
                    hunt_num++;
                    if (hunt_num > 3) {
                        hunt_num = 0;
                        strcat(buf1, "\n\r");
                        sprintf(buf, "%s ", vch->short_descr);
                        strcat(buf1, buf);
                    } else {
                        sprintf(buf, "%s ", vch->short_descr);
                        strcat(buf1, buf);
                    }
                }
                strcat(buf1, ".\n\r");
                strcat(buf1, "\n\r");
            }
            if ((vch = victim->cooperator)) {
                int coo_num = 0;
                strcat(buf1, "" HIW "正在協助你：" NOR "\n\r");
                for (vch = victim->cooperator; vch; vch = vch->next_in_coop) {
                    coo_num++;
                    if (coo_num > 3) {
                        coo_num = 0;
                        strcat(buf1, "\n\r");
                        sprintf(buf, "%s ", vch->short_descr);
                        strcat(buf1, buf);
                    } else {
                        sprintf(buf, "%s ", vch->short_descr);
                        strcat(buf1, buf);
                    }
                }
                strcat(buf1, ".\n\r");
                strcat(buf1, "\n\r");
            }
            if ((vch = victim->protector)) {
                int pro_num = 0;
                strcat(buf1, "" HIY "正在保護你：" NOR "\n\r");
                for (vch = victim->protector; vch; vch = vch->next_in_protect) {
                    pro_num++;
                    if (pro_num > 3) {
                        pro_num = 0;
                        strcat(buf1, "\n\r");
                        sprintf(buf, "%s ", vch->short_descr);
                        strcat(buf1, buf);
                    } else {
                        sprintf(buf, "%s ", vch->short_descr);
                        strcat(buf1, buf);
                    }
                }
                strcat(buf1, ".\n\r");
            }
        } else {
            if ((vch = victim->tracking)) {
                sprintf(buf, YEL "你正在追蹤 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
            }
            if ((vch = victim->assassinating)) {
                sprintf(buf, RED "你正在暗殺 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
            }
            if ((vch = victim->hunting)) {
                sprintf(buf, HIR "你正在獵殺 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
            }
            // 新增 cooperator 顯示 2021/11/20
            if ((vch = victim->cooperating)) {
                sprintf(buf, HIW "你正在協助 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
            }
            if ((vch = victim->protecting)) {
                sprintf(buf, HIY "你正在保護 " NOR "%s" NOR ".\n\r",
                        vch->short_descr);
                strcat(buf1, buf);
            }
            // 新增 hunted 計數在 config +COMBINE 時縮減顯示 2021/11/21
            if ((victim->hunted)) {
                int hunt_num = 0;
                for (vch = victim->hunted; vch; vch = vch->next_in_hunt) {
                    hunt_num++;
                }
                for (vch = victim->hunted; vch; vch = vch->next_in_hunt) {
                    if (hunt_num > 3 && IS_SET(ch->act, PLR_COMBINE)) {
                        sprintf(buf,
                                "(" HIR "%d" NOR ")%s " HIR "正在獵殺你" NOR
                                ".\n\r",
                                hunt_num, vch->short_descr);
                        strcat(buf1, buf);
                        break;
                    } else {
                        sprintf(buf, "%s " HIR "正在獵殺你" NOR ".\n\r",
                                vch->short_descr);
                        strcat(buf1, buf);
                    }
                }
            }
            // 新增 cooperator 顯示 2021/11/20
            if ((vch = victim->cooperator)) {
                int coo_num = 0;
                for (vch = victim->cooperator; vch; vch = vch->next_in_coop) {
                    coo_num++;
                }
                for (vch = victim->cooperator; vch; vch = vch->next_in_coop) {
                    if (coo_num > 3 && IS_SET(ch->act, PLR_COMBINE)) {
                        sprintf(buf,
                                "(" HIW "%d" NOR ")%s " HIW "正在協助你" NOR
                                ".\n\r",
                                coo_num, vch->short_descr);
                        strcat(buf1, buf);
                        break;
                    } else {
                        sprintf(buf, "%s " HIW "正在協助你" NOR ".\n\r",
                                vch->short_descr);
                        strcat(buf1, buf);
                    }
                }
            }
            // 新增 protected 計數在 config +COMBINE 時縮減顯示 2021/11/21
            if ((vch = victim->protector)) {
                int pro_num = 0;
                for (vch = victim->protector; vch; vch = vch->next_in_protect) {
                    pro_num++;
                }
                for (vch = victim->protector; vch; vch = vch->next_in_protect) {
                    if (pro_num > 3 && IS_SET(ch->act, PLR_COMBINE)) {
                        sprintf(buf,
                                "(" HIY "%d" NOR ")%s " HIY "正在保護你" NOR
                                ".\n\r",
                                pro_num, vch->short_descr);
                        strcat(buf1, buf);
                        break;
                    } else {
                        sprintf(buf, "%s " HIY "正在保護你" NOR ".\n\r",
                                vch->short_descr);
                        strcat(buf1, buf);
                    }
                }
            }
        }
    }

    // affected_by 特殊影響,  score a
    if (!str_cmp(key, "all") || !str_cmp(key, "a")) {
        if (!str_cmp(key, "a")) {
            strcat(buf1, "你正受到以下特殊影響：\n\r");
        }

        if (victim->affected) {
            bool printed = FALSE;

            for (paf = victim->affected; paf; paf = paf->next) {
                if (paf->deleted)
                    continue;

                if (!printed && str_cmp(key, "a")) {
                    strcat(buf1, "你正受到以下特殊影響：\n\r");
                    printed = TRUE;
                }

                sprintf(buf, "特效：「" HIC "%s" NOR "(%s)」",
                        skill_table[paf->type].cname,
                        skill_table[paf->type].name);
                strcat(buf1, buf);

                if (ch->level >= 20) {
                    strcat(buf1, "(");
                    if (strcmp(c_affect_loc_name(paf->location), "無")) {
                        sprintf(buf, "影響" HIY "%s" NOR,
                                c_affect_loc_name(paf->location));
                        strcat(buf1, buf);
                    }
                    if (paf->modifier) {
                        sprintf(buf, " %s%d " NOR "點  ",
                                paf->modifier < 0
                                    ? HIU "-"
                                    : (paf->modifier == 0 ? HIW : HIC "+"),
                                paf->modifier < 0 ? -paf->modifier
                                                  : paf->modifier);
                        strcat(buf1, buf);
                    }
                    if (paf->duration >= 0) {
                        sprintf(buf, "尚有 " HIG "%d" NOR " 小時的效用。",
                                paf->duration);
                        strcat(buf1, buf);
                    } else {
                        sprintf(buf, "持續作用中。");
                        strcat(buf1, buf);
                    }
                    strcat(buf1, ")");
                }

                strcat(buf1, "\n\r");
            }
        }
    }

    if (!str_cmp(key, "all")) {
        strcat(buf1, NOR "☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★" NOR "\n\r");
    }

    send_to_char(buf1, ch);
    return;
}

char *const day_name[] = {"月", "風", "蒼", "雷", "自由", "聖神", "星"};

char *const month_name[] = {"冬",   "冬狼", "霜之巨人", "遠古", "大戰",  "春",
                            "自然", "荒廢", "龍",       "太陽", "暑氣",  "巫術",
                            "暗影", "幽冥", "長夜",     "黑暗", "大惡魔"};

void do_time(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];
    extern char str_boot_time[];
    extern time_t down_time; //, boot_time;
    char *suf;
    int day;

    // day = time_info.day + 1;
    day = time_info.day;

    if (day > 4 && day < 20)
        suf = "th";
    else if (day % 10 == 1)
        suf = "st";
    else if (day % 10 == 2)
        suf = "nd";
    else if (day % 10 == 3)
        suf = "rd";
    else
        suf = "th";

    if (IS_IMMORTAL(ch))
        sprintf(
            buf,
            "[36m現在是%s%d時，%s之日，%d%s，%s(%d)之月。[m\n\r",
            // time_info.hour >= 12 ? "下午" : "上午",
            "",
            //( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
            time_info.hour % 24, day_name[day % 7], day, suf,
            month_name[time_info.month], time_info.month);
    else
        sprintf(buf,
                "[36m現在是%s%d時，%s之日，%d%s，%s之月。[m\n\r",
                // time_info.hour >= 12 ? "下午" : "上午",
                "",
                //( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
                time_info.hour % 24, day_name[day % 7], day, suf,
                month_name[time_info.month]);
    send_to_char(buf, ch);
    sprintf(buf,
            "[36m混沌幻世啟動於 %s\n\r系統時間是     %s[m\n\r",
            str_boot_time, (char *)ctime(&current_time));
    send_to_char(buf, ch);

    if (down_time > 0) {
        sprintf(buf, "[36m下次%s將於 %s[m\r",
                Reboot ? "Reboot" : "Shutdown", (char *)ctime(&down_time));
        send_to_char(buf, ch);
    }
    return;
}

void do_weather(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];

    // winter check add at 2018.11.06
    static char *const sky_look[6] = {"萬里無雲", "多雲",
                                      "降著小雨", "打著閃電下著暴雨",
                                      "飄著小雪", "刮起陣陣暴風雪"};

    if (!IS_OUTSIDE(ch)) {
        send_to_char("[33m在這裡你觀察不出天空的變化.[m\n\r",
                     ch);
        return;
    }

    // monsoon change add at 2018.11.06
    sprintf(buf, "天空%s, 而且%s.\n\r", sky_look[weather_info.sky],
            time_info.month <= 4
                ? "有北方凜冽的寒風吹過"
                : ((time_info.month >= 5 && time_info.month <= 8)
                       ? "有東方溫暖的和風吹過"
                       : ((time_info.month >= 9 && time_info.month <= 12)
                              ? "有南方潮濕的熱風吹過"
                              : "有西方清爽的涼風吹過")));
    send_to_char(buf, ch);
    return;
}

void do_sector(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *in_room = NULL;
    int RoomSector = ch->in_room->sector_type;

    if (!check_blind(ch))
        return;

    if (room_is_dark(ch->in_room)) {
        send_to_char("四周一片漆黑無法得知周圍環境....\n\r", ch);
        return;
    }

    static char *const SectorType[11] = {"周圍環境沒什麼特別的",
                                         "周圍環境有著許多建築物的城市",
                                         "周圍環境是一片平坦的原野",
                                         "周圍環境是有著許多樹木的森林",
                                         "周圍環境是地面高低不平的丘陵",
                                         "周圍環境是空氣稀薄的高山",
                                         "你位在平靜的水面上",
                                         "你身處在湍急的水流",
                                         "你身處在水中",
                                         "你身處在半空中",
                                         "周圍環境是乾旱的荒漠"};
    sprintf(buf, "%s", SectorType[RoomSector]);

    if (IS_SET(ch->in_room->room_flags, ROOM_DARK) ||
        IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ||
        IS_SET(ch->in_room->room_flags, ROOM_PK) ||
        IS_SET(ch->in_room->room_flags, ROOM_UNDERGROUND)) {
        sprintf(buf, "%s.\r\n\r\n這裡", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_PK))
            sprintf(buf, "%s是鬥技場", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
            sprintf(buf, "%s受到詛咒", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_UNDERGROUND))
            sprintf(buf, "%s陽光被遮蔽", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_DARK))
            sprintf(buf, "%s異常黑暗", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_CAMP) ||
            IS_SET(ch->in_room->room_flags, ROOM_SAFE)) {
            sprintf(buf, "%s, ", buf);
            if (IS_SET(ch->in_room->room_flags, ROOM_CAMP))
                sprintf(buf, "%s地面傳來魔力", buf);
            if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
                sprintf(buf, "%s相當安全", buf);
        }
    } else if (IS_SET(ch->in_room->room_flags, ROOM_CAMP) ||
               IS_SET(ch->in_room->room_flags, ROOM_SAFE)) {
        sprintf(buf, "%s.\r\n\r\n這裡", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_CAMP))
            sprintf(buf, "%s的地面傳來魔力", buf);
        if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
            sprintf(buf, "%s相當安全", buf);
    }
    sprintf(buf, "%s.\r\n", buf);
    send_to_char(buf, ch);
    return;
}

void do_help(CHAR_DATA *ch, char *argument) {
    HELP_DATA *pHelp;
    bool hfound = FALSE; /* modification to show multiple helps
    by Robin Goodall from merc mailing list */

    if (argument[0] == '\0')
        argument = "summary";

    for (pHelp = help_first; pHelp; pHelp = pHelp->next) {
        if (pHelp->level > get_trust(ch))
            continue;

        if (is_name(argument, pHelp->keyword)) {
            if (hfound)
                send_to_char("\n\r---------------------------------------------"
                             "-------------------------------\n\r\n\r",
                             ch);

            if (pHelp->level >= 0 && str_cmp(argument, "imotd")) {
                send_to_char(pHelp->keyword, ch);
                send_to_char("\n\r", ch);
            }

            /*
             * Strip leading '.' to allow initial blanks.
             */
            if (pHelp->text[0] == '.')
                send_to_char(pHelp->text + 1, ch);
            else
                send_to_char(pHelp->text, ch);
            hfound = TRUE;
        }
    }

    if (!hfound)
        send_to_char("[33m沒有關於這個字的說明喔！[m\n\r", ch);
    return;
}

/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
// do_who modified by jye 6.18.2005
int MAX_PPL;
void do_who(CHAR_DATA *ch, char *argument) {
    DESCRIPTOR_DATA *d;
    // CLAN_DATA *cn;
    char buf[MAX_STRING_LENGTH * 7];
    char buf2[BUF_STRING_LENGTH * 2];
    // char     name_clan [ MAX_INPUT_LENGTH ];
    char arg1[MAX_INPUT_LENGTH];
    int iClass;
    int iRace;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    int current_ppl = 0;
    int curr_lv, i, comp_lv;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_RACE];
    bool fRaceRestrict;
    bool fClassRestrict;
    bool fImmortalOnly;
    bool fChinese = FALSE;
    bool fVerbose = FALSE;
    bool fGroup = FALSE;
    bool fPK = FALSE;
    int count = 0;
    bool fListGroup = FALSE;
    bool fRecall = FALSE;
    bool who_clan = FALSE;
    // bool   cn_exist = FALSE;
    CHAR_DATA *wch[500];
    CHAR_DATA *leader = NULL;
    char const *class[500];

    /*
     * Set default arguments.
     */
    iLevelLower = 0;
    iLevelUpper = L_ACG; /*Used to be Max_level */
    fClassRestrict = FALSE;
    fImmortalOnly = FALSE;
    fRaceRestrict = FALSE;

    for (iClass = 0; iClass < MAX_CLASS; iClass++)
        rgfClass[iClass] = FALSE;

    for (iRace = 0; iRace < MAX_RACE; iRace++)
        rgfRace[iRace] = FALSE;

    if (IS_SET(ch->act, PLR_CHINESE))
        fChinese = TRUE;
    /*
     * Parse arguments.
     */
    nNumber = 0;
    for (;;) {
        char arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);
        argument = one_argument(argument, arg1);

        if (arg[0] == '\0')
            break;

        if (is_number(arg)) {
            // modify by jye 0940618
            iLevelLower = atoi(arg);
            iLevelUpper = (0 < atoi(arg1) ? atoi(arg1) : iLevelUpper);
            // switch ( ++nNumber )
            //{
            //    case 1: iLevelLower = atoi( arg ); break;
            //    case 2: iLevelUpper = atoi( arg1 ); break;
            //    default:
            //         send_to_char( "[33m只能打兩個等級。[m\n\r", ch
            //         ); return;
            // }
        } else if (arg[0] == '-') {
            switch (arg[1]) {
            case 'c':
                fChinese = TRUE;
                break;
            case 'e':
                fChinese = FALSE;
                break;
            case 'v':
                fVerbose = TRUE;
                break;
            case 'r':
                fRecall = TRUE;
                break;

            default:
                send_to_char(YEL "沒有這個參數。" NOR "\n\r", ch);
                return;
            }
        } else {
            int iClass;

            if (!str_cmp(arg, "group")) {
                fGroup = TRUE;
                //              one_argument( argument, buf2);
                if (arg1[0] == '\0')
                    break;

                //              log_string( "who group test 1" );
                //              log_string( arg1);
                leader = get_char_world(ch, arg1);

                if (!leader || IS_NPC(leader)) {
                    send_to_char(YEL "沒有這個玩家。" NOR "\n\r", ch);
                    return;
                }
                fGroup = FALSE;
                fListGroup = TRUE;
                break;
            }
            /*
                        if ( !str_cmp( arg, "clan" ) )
                        {
                          cn = clan_list;
                          who_clan = TRUE;
                          if( arg1[0] != '\0' )
                          {
                            for( cn; cn; cn = cn->next )
                            {
                              if ( !str_cmp( cn->leader, arg1 ) )
                              {
                                sprintf( name_clan, cn->name );
                                cn_exist = TRUE;
                                break;
                              }
                              else
                              {
                                send_to_char( "這人是幫主嗎？\n\r", ch );
                                return;
                              }
                            }
                          }
                          send_to_char( cn->name, ch );
                          break;
                        }
            */
            if (!str_cmp(arg, "pk")) {
                fPK = TRUE;
                break;
            }

            /*
             * Look for classes to turn on.
             */
            // arg[3]    = '\0';
            if (!str_cmp(arg, "imm")) {
                fImmortalOnly = TRUE;
            } else {
                if (strlen(arg) < 3) {
                    send_to_char("[33m職業或種族名稱要打得更長些。["
                                 "m\n\r",
                                 ch);
                    return;
                }

                for (iClass = 0; iClass < MAX_CLASS; iClass++) {
                    if (!str_prefix(arg, class_table[iClass].who_name) ||
                        !str_prefix(arg, class_table[iClass].name)) {
                        rgfClass[iClass] = TRUE;
                        fClassRestrict = TRUE;
                        break;
                    }
                }

                // add by jye 0940618
                if (!fClassRestrict) {
                    for (iRace = 0; iRace < MAX_RACE; iRace++) {
                        if (!str_prefix(arg, race_table[iRace].name)) {
                            rgfRace[iRace] = TRUE;
                            fRaceRestrict = TRUE;
                            break;
                        }
                    }
                }

                if (!fClassRestrict && !fRaceRestrict) {
                    send_to_char(
                        "[33m那不是一個職業或種族！。[m\n\r", ch);
                    return;
                }
            }
        }
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    sprintf(buf,
            "[ " HIC "混沌幻世" NOR " ]%s%s%s\n\r"
            "=================================================================="
            "======" NOR "\n\r",
            (fPK || fImmortalOnly || fListGroup || who_clan) ? " - " : "",
            fListGroup
                ? ((leader->group) ? leader->group->name : leader->short_descr)
                : (fPK ? "參加 PK 玩家列表"
                       : (fImmortalOnly ? "神族列表"
                                        : (who_clan ? "幫員列表" : ""))),
            fListGroup ? (leader->group ? "" : "的隊伍") : "");
    for (d = descriptor_list, i = 0; d; d = d->next, i++) {
        current_ppl += 1;

        wch[i] = (d->original) ? d->original : d->character;

        /*
         * Check for match against restrictions.
         * Don't use trust as that exposes trusted mortals.
         */
        if (d->connected != CON_PLAYING || !can_see(ch, wch[i])) {
            i--;
            continue;
        }

        if (IS_NPC(wch[i])) {
            i--;
            continue;
        }

        if (!(wch[i]->level >= iLevelLower && wch[i]->level <= iLevelUpper) ||
            (fImmortalOnly &&
             (wch[i]->level < L_APP && wch[i]->trust < L_APP)) ||
            (fPK && !IS_SET(wch[i]->act, PLR_REGISTER)) ||
            (fClassRestrict && !rgfClass[wch[i]->class]) ||
            (fRaceRestrict && !rgfRace[wch[i]->race]) ||
            (fListGroup && !is_same_group(wch[i], leader)) ||
            (fGroup && (!wch[i]->group || wch[i] != wch[i]->group->leader)) ||
            (fRecall &&
             wch[i]->in_room->area->recall != ch->in_room->area->recall)
            //|| ( fGroup && wch[i]->leader != wch[i] && wch[i]->leader->leader
            //!= wch[i] )
        ) {
            i--;
            continue;
        }

        nMatch++;

        /*
         * Figure out what to print for class.
         */

        if (fChinese)
            class[i] = class_table[wch[i]->class].cwho_name;
        else
            class[i] = class_table[wch[i]->class].who_name;

        if (wch[i]->level >= 41) // LEVEL_IMMORTAL )
            switch (wch[i]->level) {
            default:
                break;
            case L_ACG:
                class[i] = (fChinese) ? HIW "顧  問" NOR : HIW "ANC" NOR;
                break;
            case L_DIR:
                class[i] = (fChinese) ? HIC "創世神" NOR : HIC "ADM" NOR;
                break;
            case L_SEN:
                class[i] = (fChinese) ? HIY "大  神" NOR : HIY "GOD" NOR;
                break;
            case L_JUN:
                class[i] = (fChinese) ? HIU "  神  " NOR : HIU "GOD" NOR;
                break;
            case L_APP:
                class[i] = (fChinese) ? HIG "天  使" NOR : HIG "ANG" NOR;
                break;
            case 42:
                class[i] = (fChinese) ? HIR "傳  奇" NOR : HIR "LGD" NOR;
                break;
            }
    }
    /*
     * Format it up.
     */

    curr_lv = (who_clan ? 5 : MAX_LEVEL);
    comp_lv = (who_clan ? 0 : 1);

    for (; curr_lv >= comp_lv; curr_lv--) { // sort lv
        for (i = 0; i < nMatch; i++) { // for each lv, we search for every match
            if (curr_lv !=
                (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level))
                continue;
            /*if( cn_exist )
            {
              if( wch[i]->clan )
              {
                if ( str_cmp( name_clan, wch[i]->clan->whoname ) == FALSE )
                  continue;
              }
              else
                continue;
            }*/
            ++count;
            if (fChinese) {
                if (wch[i]->level < (LEVEL_IMMORTAL - 2)) {
                    sprintf(
                        buf + strlen(buf),
                        "【" HIW " %2d" NOR " " HIW "%6s" NOR
                        " 】 %s%s%s%s%s(" HIY "%s" NOR ") %s (" HIG "%s" NOR
                        ")\n\r",
                        (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level),
                        (who_clan ? (wch[i]->clan ? wch[i]->clan->whoname
                                                  : HIW "無幫派" NOR)
                                  : class[i]),
                        IS_SET(wch[i]->act, PLR_KILLER) ? "(" RED "兇手" NOR ")"
                                                        : "",
                        IS_SET(wch[i]->act, PLR_THIEF) ? "(" RED "小偷" NOR ")"
                                                       : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_REGISTER))
                            ? "(" HIG "PK" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_AFK))
                            ? "(" HIP "發呆" NOR ")"
                            : "",
                        // wch[i]->short_descr,
                        wch[i]->cname, wch[i]->name, wch[i]->pcdata->title,
                        race_table[((wch[i]->temp_race) ? wch[i]->temp_race - 1
                                                        : wch[i]->race)]
                            .cname);
                    // race_table[ wch[i]->race ].cname );
                } else if (wch[i]->level == (LEVEL_IMMORTAL - 2)) {
                    sprintf(
                        buf + strlen(buf),
                        "【" HIW " %2d" NOR " " HIP "%6s" NOR
                        " 】 %s%s%s%s%s(" HIY "%s" NOR ") %s (" HIG "%s" NOR
                        ")\n\r",
                        (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level),
                        (who_clan ? (wch[i]->clan ? wch[i]->clan->whoname
                                                  : HIW "無幫派" NOR)
                                  : class[i]),
                        IS_SET(wch[i]->act, PLR_KILLER) ? "(" RED "兇手" NOR ")"
                                                        : "",
                        IS_SET(wch[i]->act, PLR_THIEF) ? "(" RED "小偷" NOR ")"
                                                       : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_REGISTER))
                            ? "(" HIG "PK" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_AFK))
                            ? "(" HIP "發呆" NOR ")"
                            : "",
                        // wch[i]->short_descr,
                        wch[i]->cname, wch[i]->name, wch[i]->pcdata->title,
                        race_table[((wch[i]->temp_race)
                                        ? (wch[i]->temp_race - 1)
                                        : (wch[i]->race))]
                            .cname);
                    // race_table[ wch[i]->race ].cname );
                } else {
                    sprintf(
                        buf + strlen(buf),
                        "【" HIW " %2d" NOR " %6s" NOR " 】 %s%s%s%s%s%s(" HIY
                        "%s" NOR ") %s (" HIG "%s" NOR ")\n\r",
                        (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level),
                        (who_clan ? (wch[i]->clan ? wch[i]->clan->whoname
                                                  : HIW "無幫派" NOR)
                                  : class[i]),
                        IS_SET(wch[i]->act, PLR_WIZINVIS) ? "(" HIY "神隱" NOR
                                                            ")"
                                                          : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_KILLER))
                            ? "(" RED "兇手" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_THIEF))
                            ? "(" RED "小偷" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_REGISTER))
                            ? "(" HIG "PK" NOR ")"
                            : "",
                        (IS_SET(wch[i]->act, PLR_AFK)) ? "(" HIP "發呆" NOR ")"
                                                       : "",
                        // wch[i]->short_descr,
                        wch[i]->cname, wch[i]->name, wch[i]->pcdata->title,
                        race_table[((wch[i]->temp_race)
                                        ? (wch[i]->temp_race - 1)
                                        : (wch[i]->race))]
                            .cname);
                    // race_table[ wch[i]->race ].cname );
                }
            } else {
                if (wch[i]->level < (LEVEL_IMMORTAL - 2)) {
                    sprintf(
                        buf + strlen(buf),
                        "【" HIW " %2d" NOR " " HIW "%3s" NOR " 】 %s%s%s%s" HIY
                        "%s" NOR " %s (" HIG "%s" NOR ")\n\r",
                        (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level),
                        (who_clan ? (wch[i]->clan ? wch[i]->clan->whoname
                                                  : HIW "無幫派" NOR)
                                  : class[i]),
                        IS_SET(wch[i]->act, PLR_KILLER) ? "(" RED "兇手" NOR ")"
                                                        : "",
                        IS_SET(wch[i]->act, PLR_THIEF) ? "(" RED "小偷" NOR ")"
                                                       : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_REGISTER))
                            ? "(" HIG "PK" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_AFK))
                            ? "(" HIP "發呆" NOR ")"
                            : "",
                        // wch[i]->cname,
                        wch[i]->name, wch[i]->pcdata->title,
                        race_table[((wch[i]->temp_race)
                                        ? (wch[i]->temp_race - 1)
                                        : (wch[i]->race))]
                            .name);
                    // race_table[ wch[i]->race ].name );
                } else if (wch[i]->level == (LEVEL_IMMORTAL - 2)) {
                    sprintf(
                        buf + strlen(buf),
                        "【" HIW " %2d" NOR " " HIP "%3s" NOR " 】 %s%s%s%s" HIY
                        "%s" NOR " %s (" HIG "%s" NOR ")\n\r",
                        (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level),
                        (who_clan ? (wch[i]->clan ? wch[i]->clan->whoname
                                                  : HIW "無幫派" NOR)
                                  : class[i]),
                        IS_SET(wch[i]->act, PLR_KILLER) ? "(" RED "兇手" NOR ")"
                                                        : "",
                        IS_SET(wch[i]->act, PLR_THIEF) ? "(" RED "小偷" NOR ")"
                                                       : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_REGISTER))
                            ? "(" HIG "PK" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_AFK))
                            ? "(" HIP "發呆" NOR ")"
                            : "",
                        // wch[i]->cname,
                        wch[i]->name, wch[i]->pcdata->title,
                        race_table[((wch[i]->temp_race) ? wch[i]->temp_race - 1
                                                        : wch[i]->race)]
                            .name);
                    // race_table[ wch[i]->race ].name );
                } else {
                    sprintf(
                        buf + strlen(buf),
                        "【" HIW " %2d" NOR " %s" NOR " 】 %s%s%s%s%s" HIY
                        "%s" NOR " %s (" HIG "%s" NOR ")\n\r",
                        (who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level),
                        (who_clan ? (wch[i]->clan ? wch[i]->clan->whoname
                                                  : HIW "無幫派" NOR)
                                  : class[i]),
                        IS_SET(wch[i]->act, PLR_WIZINVIS) ? "(" HIY "神隱" NOR
                                                            ")"
                                                          : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_KILLER))
                            ? "(" RED "兇手" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_THIEF))
                            ? "(" RED "小偷" NOR ")"
                            : "",
                        (fVerbose && IS_SET(wch[i]->act, PLR_REGISTER))
                            ? "(" HIG "PK" NOR ")"
                            : "",
                        IS_SET(wch[i]->act, PLR_AFK) ? "(" HIP "發呆" NOR ")"
                                                     : "",
                        // wch[i]->cname,
                        wch[i]->name, wch[i]->pcdata->title,
                        race_table[((wch[i]->temp_race)
                                        ? (wch[i]->temp_race - 1)
                                        : (wch[i]->race))]
                            .name);
                    // race_table[ wch[i]->race ].name );
                }
            }
        } // j end

        // 修改最大 pagelength , 故可顯示上限提高 2023/01/12
        // if( count >= 50 ) {
        if (count >= 80) {
            send_to_char(buf, ch);
            buf[0] = '\0';
            count = 0;
        }
    } // sort lv end

    sprintf(buf2, "%s有 " HIW "%d / %d" NOR " 名勇者正在創造新的傳說。\n\r",
            "=================================================================="
            "======" NOR "\n\r",
            nMatch, current_ppl);
    strcat(buf, buf2);

    if (current_ppl > MAX_PPL)
        MAX_PPL = current_ppl;
    sprintf(buf2, "最多曾有 " HIW "%d" NOR " 名勇者同時在這個世界遊歷。\n\r",
            MAX_PPL);
    strcat(buf, buf2);

    send_to_char(buf, ch);
    return;
}

/* Contributed by Kaneda */
void do_whois(CHAR_DATA *ch, char *argument) {
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    char name[MAX_INPUT_LENGTH];

    one_argument(argument, name);

    if (name[0] == '\0') {
        send_to_char("[33m使用方法：whois <name>[m\n\r", ch);
        return;
    }

    name[0] = UPPER(name[0]);

    buf[0] = '\0';
    for (d = descriptor_list; d; d = d->next) {
        CHAR_DATA *wch;
        char const *class;

        wch = (d->original) ? d->original : d->character;

        if (d->connected != CON_PLAYING || !can_see(ch, wch))
            continue;

        if (str_prefix(name, wch->name))
            continue;

        class = class_table[wch->class].cname;
        if (wch->level >= LEVEL_IMMORTAL)
            switch (wch->level) {
            case L_ACG:
                class = HIW "顧問" NOR;
                break;
            case L_DIR:
                class = HIW "天神" NOR;
                break;
            case L_SEN:
                class = HIY "大神" NOR;
                break;
            case L_JUN:
                class = HIG " 神 " NOR;
                break;
            case L_APP:
                class = HIC "天使" NOR;
                break;
                /*
                        case L_DIR: class = "[1;36m大大神[m"; break;
                        case L_SEN: class = "[1;36m 大神 [m"; break;
                        case L_JUN: class = "[1;36m  神  [m"; break;
                        case L_APP: class = "[1;36m見習神[m"; break;
                */
            }

        /*
         * Format it up.
         */
        if (wch->level < LEVEL_IMMORTAL)
            sprintf(buf + strlen(buf), "(%2d %s %-6s(%-8s))%s%s%s%s%s%s %s\n\r",
                    wch->level, class, race_table[wch->race].cname,
                    race_table[wch->race].name,
                    IS_SET(wch->act, PLR_KILLER) ? "(" RED "兇手" NOR ")" : "",
                    IS_SET(wch->act, PLR_THIEF) ? "(" RED "小偷" NOR ")" : "",
                    IS_SET(wch->act, PLR_REGISTER) ? "(" HIG "PK" NOR ")" : "",
                    IS_SET(wch->act, PLR_AFK) ? "(" HIP "發呆" NOR ")" : "",
                    wch->short_descr, wch->pcdata->title, "");
        else
            sprintf(buf + strlen(buf),
                    "(        %s        )%s%s%s%s%s%s%s %s\n\r", class,
                    IS_SET(wch->act, PLR_WIZINVIS) ? "(" HIY "神隱" NOR ")"
                                                   : "",
                    IS_SET(wch->act, PLR_KILLER) ? "(" RED "兇手" NOR ")" : "",
                    IS_SET(wch->act, PLR_THIEF) ? "(" RED "小偷" NOR ")" : "",
                    IS_SET(wch->act, PLR_REGISTER) ? "(" HIG "PK" NOR ")" : "",
                    IS_SET(wch->act, PLR_AFK) ? "(" HIP "發呆" NOR ")" : "",
                    wch->short_descr, wch->pcdata->title, "");
    }

    if (buf[0] == '\0')
        send_to_char("[33m沒有人合你的條件。[m\n\r", ch);
    else
        send_to_char(buf, ch);
    return;
}

void do_inventory(CHAR_DATA *ch, char *argument) {
    if (IS_IMMORTAL(ch)) {
        char arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        one_argument(argument, arg);
        if (arg[0] == '\0')
            victim = ch;
        else if (!(victim = get_char_world(ch, arg))) {
            send_to_char("這裡沒有這個人。\n\r", ch);
            return;
        }
        send_to_char("你身上帶著有：\n\r\n\r", ch);
        show_list_to_char(victim->carrying, ch, TRUE, TRUE);
        return;
    }
    send_to_char("你身上帶著有：\n\r\n\r", ch);
    show_list_to_char(ch->carrying, ch, TRUE, TRUE);
    return;
}

void do_equipment(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    int iWear;
    bool found;
    CHAR_DATA *victim;
    if (IS_IMMORTAL(ch)) {
        char arg[MAX_INPUT_LENGTH];
        one_argument(argument, arg);
        if (arg[0] == '\0')
            victim = ch;
        else if (!(victim = get_char_world(ch, arg))) {
            send_to_char("這裡沒有這個人。\n\r", ch);
            return;
        }
    } else {
        victim = ch;
    }

    send_to_char("你身上裝備著：\n\r", ch);
    found = FALSE;
    if (!IS_SET(race_table[victim->race].race_abilities, RACE_BEHOLDER)) {
        for (iWear = 0; iWear < MAX_WEAR; iWear++) {
            if (obj = get_eq_char(victim, iWear)) {
                send_to_char(where_name[iWear], ch);
                if (can_see_obj(ch, obj)) {
                    send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                    send_to_char("\n\r", ch);
                } else {
                    send_to_char("不明物品。\n\r", ch);
                }
                found = TRUE;
            }
            if (iWear == WEAR_LEGS) {
                if (obj = get_eq_char(victim, WEAR_LEGS_2)) {
                    send_to_char(where_name[WEAR_LEGS_2], ch);
                    if (can_see_obj(ch, obj)) {
                        send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                        send_to_char("\n\r", ch);
                    } else {
                        send_to_char("不明物品。\n\r", ch);
                    }
                    found = TRUE;
                }
            } else if (iWear == WEAR_FEET) {
                if (obj = get_eq_char(victim, WEAR_FEET_2)) {
                    send_to_char(where_name[WEAR_FEET_2], ch);
                    if (can_see_obj(ch, obj)) {
                        send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                        send_to_char("\n\r", ch);
                    } else {
                        send_to_char("不明物品。\n\r", ch);
                    }
                    found = TRUE;
                }
            }
        }
        // 彈藥裝備位置 add at 2022/04/30
        if (obj = get_eq_char(victim, WEAR_AMMO)) {
            send_to_char(where_name[WEAR_AMMO], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        if (!(obj = get_eq_char(victim, WEAR_RIDE)))
            ;
        else {
            send_to_char(where_name[WEAR_RIDE], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
    } else { // Code for Beholder
        for (iWear = 0; iWear <= WEAR_FINGER_R; iWear++) {
            if (!(obj = get_eq_char(victim, iWear)))
                continue;

            send_to_char(where_name[iWear], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        for (iWear = WEAR_FINGER_3; iWear <= WEAR_FINGER_4; iWear++) {
            if (!(obj = get_eq_char(victim, iWear)))
                continue;

            send_to_char(where_name[iWear], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        for (iWear = WEAR_NECK_1; iWear <= WEAR_HANDS; iWear++) {
            if (!(obj = get_eq_char(victim, iWear)))
                continue;

            send_to_char(where_name[iWear], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        if ((obj = get_eq_char(victim, WEAR_HANDS_2))) {
            send_to_char(where_name[WEAR_HANDS_2], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        if ((obj = get_eq_char(victim, WEAR_ARMS))) {
            send_to_char(where_name[WEAR_ARMS], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        if ((obj = get_eq_char(victim, WEAR_ARMS_2))) {
            send_to_char(where_name[WEAR_ARMS_2], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        for (iWear = WEAR_SHIELD; iWear < MAX_WEAR; iWear++) {
            if (!(obj = get_eq_char(victim, iWear)))
                continue;

            send_to_char(where_name[iWear], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        // 彈藥裝備位置 add at 2022/04/30
        if (obj = get_eq_char(victim, WEAR_AMMO)) {
            send_to_char(where_name[WEAR_AMMO], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
        // check WEAR_RIDE
        if ((obj = get_eq_char(victim, WEAR_RIDE))) {
            send_to_char(where_name[WEAR_RIDE], ch);
            if (can_see_obj(ch, obj)) {
                send_to_char(format_obj_to_char(obj, ch, TRUE), ch);
                send_to_char("\n\r", ch);
            } else {
                send_to_char("不明物品。\n\r", ch);
            }
            found = TRUE;
        }
    }
    if (!found)
        send_to_char("什麼也沒有。\n\r", ch);

    return;
}

void do_compare(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    char *msg;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int value1;
    int value2;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0') {
        send_to_char("[33m要比較什麼跟什麼？[m\n\r", ch);
        return;
    }

    if (!(obj1 = get_obj_carry(ch, arg1))) {
        send_to_char("[33m你沒有那樣東西。[m\n\r", ch);
        return;
    }

    if (arg2[0] == '\0') {
        for (obj2 = ch->carrying; obj2; obj2 = obj2->next_content) {
            if (obj2->wear_loc != WEAR_NONE && can_see_obj(ch, obj2) &&
                obj1->item_type == obj2->item_type &&
                (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0)
                break;
        }

        if (!obj2) {
            send_to_char("[33m你沒有穿著可以拿來做為比較的東西。["
                         "m\n\r",
                         ch);
            return;
        }
    } else {
        if (!(obj2 = get_obj_carry(ch, arg2))) {
            char new_arg2[MAX_INPUT_LENGTH];
            int number;

            /*  Strip off number argument, subtract one, paste it together */
            number = number_argument(arg2, arg2);
            if (number > 1)
                number--;
            sprintf(new_arg2, "%d.%s", number, arg2);

            if (!(obj2 = get_obj_wear(ch, new_arg2))) {
                send_to_char("[33m你沒有那樣東西。[m\n\r", ch);
                return;
            }

            if ((obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) == 0) {
                send_to_char(
                    "[33m這不是可以拿來比較的東西。[m\n\r", ch);
                return;
            }
        }
    }

    msg = NULL;
    value1 = 0;
    value2 = 0;

    if (obj1 == obj2) {
        msg = "[33m你把$p和它自己拿來比較。它們看起來差不多是一樣的。["
              "m";
    } else if (obj1->item_type != obj2->item_type) {
        msg = "[33m你不能拿$P來跟$p比較。[m";
    } else {
        switch (obj1->item_type) {
        default:
            msg = "[33m$p和$P不能拿來比較。[m";
            break;

        case ITEM_ARMOR:
            value1 = obj1->value[0];
            value2 = obj2->value[0];
            break;

        case ITEM_WEAPON:
            value1 = obj1->value[1] + obj1->value[2];
            value2 = obj2->value[1] + obj2->value[2];
            break;
        }
    }

    if (!msg) {
        if (obj2->wear_loc != WEAR_NONE) {
            if (value1 == value2)
                msg = "[36m$p和$P（裝備在身上的）看起來差不多是一樣的。["
                      "m";
            else if (value1 > value2)
                msg = "[36m$p看起來比$P（裝備在身上的）要來得好。["
                      "m";
            else
                msg = "[36m$p看起來比$P（裝備在身上的）還來得爛。["
                      "m";
        } else {
            if (value1 == value2)
                msg = "[36m$p和$P看起來差不多是一樣的。[m";
            else if (value1 > value2)
                msg = "[36m$p看起來比$P要來得好。[m";
            else
                msg = "[36m$p看起來比$P還來得爛。[m";
        }
    }

    act(msg, ch, obj1, obj2, TO_CHAR);
    return;
}

void do_credits(CHAR_DATA *ch, char *argument) {
    do_help(ch, "diku");
    return;
}

void do_where(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    bool found;

    if (!check_blind(ch))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("[36m在你附近的有：[m\n\r", ch);
        found = FALSE;
        for (d = descriptor_list; d; d = d->next) {
            if (d->connected == CON_PLAYING && (victim = d->character) &&
                !IS_NPC(victim) && victim->in_room &&
                victim->in_room->area == ch->in_room->area &&
                can_see(ch, victim)) {
                found = TRUE;
                sprintf(buf, "%-18s %s\n\r", victim->short_descr,
                        victim->in_room->name);
                send_to_char(buf, ch);
            }
        }
        if (!found)
            send_to_char("[33m無[m\n\r", ch);
    } else {
        found = FALSE;
        for (victim = char_list; victim; victim = victim->next) {
            if (!victim->in_room || IS_AFFECTED(victim, AFF_HIDE) ||
                IS_AFFECTED(victim, AFF_SNEAK))
                continue;

            if (victim->in_room->area == ch->in_room->area &&
                can_see(ch, victim) && is_pfxname(arg, victim->name)) {
                found = TRUE;
                sprintf(buf, "%-28s %s\n\r", PERS(victim, ch),
                        victim->in_room->name);
                send_to_char(buf, ch);
                break;
            }
        }
        if (!found)
            act("[33m你找不到什麼$T。[m", ch, NULL, arg, TO_CHAR);
    }

    return;
}

void do_consider(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    char *buf = '\0';
    char *msg;
    char arg[MAX_INPUT_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    int diff;
    int hpdiff;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("[33m要考量殺誰？[m\n\r", ch);
        return;
    }

    if (!(victim = get_char_room(ch, arg))) {
        send_to_char("[33m他不在這兒。[m\n\r", ch);
        return;
    }

    if (!IS_NPC(victim) && !IS_SET(victim->act, PLR_KILLER)) {
        send_to_char("[33m想當殺人犯？考慮這麼多幹嘛，幹下去(murder)就對了！["
                     "m\n\r",
                     ch);
        return;
    }

    diff = victim->level - ch->level;

    if (diff <= -20)
        msg = HIG "你不穿防具也可以空手做掉$N。" NOR;
    else if (diff <= -10)
        msg = HIG "$N不值得你去殺他。" NOR;
    else if (diff <= -5)
        msg = HIG "$N看起來很好解決。" NOR;
    else if (diff <= 5)
        msg = HIY "棋逢敵手！" NOR;
    else if (diff <= 10)
        msg = HIY "你需要一點運氣才能打敗$N" NOR;
    else if (diff <= 20)
        msg = HIR "$N毫不留情地對你大笑。" NOR;
    else
        msg = HIR "死神會感謝你的禮物。" NOR;

    act(msg, ch, NULL, victim, TO_CHAR);

    /* additions by amenda at 4/20/02 */
    if (victim->sex == 1)
        sprintf(buf1, "\n\r他");
    else if (victim->sex == 2)
        sprintf(buf1, "\n\r她");
    else
        sprintf(buf1, "\n\r它");

    if (get_curr_size(victim) >= 9)
        strcat(buf1, "的體型跟恐龍差不多大。");
    else if (get_curr_size(victim) >= 8)
        strcat(buf1, "的體型像一頭飛龍。");
    else if (get_curr_size(victim) >= 7)
        strcat(buf1, "的體型能與巨人媲美。");
    else if (get_curr_size(victim) >= 6)
        strcat(buf1, "的身體與巨魔差不多大。，");
    else if (get_curr_size(victim) >= 5)
        strcat(buf1, "跟托斯馬克的雕像差不多大。");
    else if (get_curr_size(victim) >= 4)
        strcat(buf1, "比一般成年人高大一些。");
    else if (get_curr_size(victim) >= 3)
        strcat(buf1, "沒特別高大，與人類差不多。");
    else if (get_curr_size(victim) >= 2)
        strcat(buf1, "矮小的身體，像個半身人。");
    else if (get_curr_size(victim) >= 1)
        strcat(buf1, "如同小妖精一樣，嬌小玲瓏。");
    else
        strcat(buf1, "似乎不存在半點質量，跟空氣差不多！");
    send_to_char(buf1, ch);

    if (victim->sex == 1)
        sprintf(buf1, "\n\r他");
    else if (victim->sex == 2)
        sprintf(buf1, "\n\r她");
    else
        sprintf(buf1, "\n\r它");

    if (GET_DODGE(victim) >= 200)
        strcat(buf1, "行動起來根本像一塊岩石。");
    else if (GET_DODGE(victim) >= 0)
        strcat(buf1, "動作像緩慢而痛苦地爬行。");
    else if (GET_DODGE(victim) >= -50)
        strcat(buf1, "動作比蝸牛快不了多少。");
    else if (GET_DODGE(victim) >= -150)
        strcat(buf1, "動作終於學會走路。");
    else if (GET_DODGE(victim) >= -300)
        strcat(buf1, "行動速度不錯。");
    else if (GET_DODGE(victim) >= -500)
        strcat(buf1, "行動非常敏捷。");
    else if (GET_DODGE(victim) >= -700)
        strcat(buf1, "動如脫兔，一洩千里。");
    else if (GET_DODGE(victim) >= -950)
        strcat(buf1, "敏捷得就像一隻麋鹿。");
    else if (GET_DODGE(victim) >= -1000)
        strcat(buf1, "速度快得你看不到。");
    else if (GET_DODGE(victim) >= -1200)
        strcat(buf1, "速度根本就像一陣旋風。");
    else if (GET_DODGE(victim) >= -1400)
        strcat(buf1, "速度快到可以追上自己的影子。");
    else
        strcat(buf1, "" HIR "速度快到已經追上自己的影子了！！" NOR "");
    send_to_char(buf1, ch);

    if (victim->sex == 1)
        sprintf(buf1, "\n\r他");
    else if (victim->sex == 2)
        sprintf(buf1, "\n\r她");
    else
        sprintf(buf1, "\n\r它");

    if (GET_AC(victim) >= 300)
        strcat(buf1, "身上的穿著比裸體的還誇張！");
    else if (GET_AC(victim) >= 100)
        strcat(buf1, "跟祼體沒什麼兩樣。");
    else if (GET_AC(victim) >= 0)
        strcat(buf1, "只穿著內衣內褲。");
    else if (GET_AC(victim) >= -400)
        strcat(buf1, "穿著普通的衣服。");
    else if (GET_AC(victim) >= -500)
        strcat(buf1, "多少有點裝備。");
    else if (GET_AC(victim) >= -1000)
        strcat(buf1, "裝備起來。");
    else if (GET_AC(victim) >= -1500)
        strcat(buf1, "的裝備不錯。");
    else if (GET_AC(victim) >= -2000)
        strcat(buf1, "的裝備很好。");
    else if (GET_AC(victim) >= -2500)
        strcat(buf1, "穿著重型裝備。");
    else if (GET_AC(victim) >= -3000)
        strcat(buf1, "穿著超級的裝備。");
    else if (GET_AC(victim) >= -3500)
        strcat(buf1, "的裝備只應天上有。");
    else if (GET_AC(victim) >= -4000)
        strcat(buf1, "的裝備天下無敵。");
    else if (GET_AC(victim) >= -5000)
        strcat(buf1, "穿著超級無敵黃金霹靂的裝備。");
    else
        strcat(buf1, "" HIR "的裝備太誇張了，小心點，或許是用 bug！" NOR "");
    send_to_char(buf1, ch);

    if (victim->sex == 1)
        sprintf(buf1, "\n\r他");
    else if (victim->sex == 2)
        sprintf(buf1, "\n\r她");
    else
        sprintf(buf1, "\n\r它");

    if (get_damroll(victim, WEAR_WIELD) < 50)
        strcat(buf1, "看來毫無破壞力，");
    else if (get_damroll(victim, WEAR_WIELD) < 100)
        strcat(buf1, "的武裝勉強能造成一些傷害，");
    else if (get_damroll(victim, WEAR_WIELD) < 150)
        strcat(buf1, "似乎蠻強壯，");
    else if (get_damroll(victim, WEAR_WIELD) < 200)
        strcat(buf1, "能造成很大傷害，");
    else
        strcat(buf1, "" HIR "似乎能開碑裂石！" NOR "");

    send_to_char(buf1, ch);
    /* additions by king@tinuviel.cs.wcu.edu */
    hpdiff = (ch->hit - victim->hit);

    if (((diff >= 0) && (hpdiff <= 0)) || ((diff <= 0) && (hpdiff >= 0))) {
        send_to_char("而且", ch);
    } else {
        send_to_char("不過", ch);
    }

    if (hpdiff >= 101)
        buf = "你的生命力比$E多滿多的。[m";
    if (hpdiff <= 100)
        buf = "你的生命力比$E多。[m";
    if (hpdiff <= 50)
        buf = "你的生命力比$E稍多。[m";
    if (hpdiff <= 25)
        buf = "你的生命力比$E多一點點。[m";
    if (hpdiff <= 0)
        buf = "$E的生命力比你多一點點。[m";
    if (hpdiff <= -25)
        buf = "$E的生命力比你稍多。[m";
    if (hpdiff <= -50)
        buf = "$E的生命力比你多。[m";
    if (hpdiff <= -100)
        buf = "$E的生命力比你多滿多的。[m";

    act(buf, ch, NULL, victim, TO_CHAR);
    return;
}

void set_hname(CHAR_DATA *ch, char *cname) {
    char buf[BUF_STRING_LENGTH];
    char buf1[BUF_STRING_LENGTH];

    buf[0] = '\0';

    strcpy(buf, cname);
    free_string(ch->pcdata->horse->name);
    ch->pcdata->horse->name = str_dup(buf);

    sprintf(buf1, "%s(%s)", ch->pcdata->horse->cname, ch->pcdata->horse->name);
    free_string(ch->pcdata->horse->short_descr);
    ch->pcdata->horse->short_descr = str_dup(buf1);

    sprintf(buf1, "%s(%s)乖乖地站在這裡。\n\r", ch->pcdata->horse->cname,
            ch->pcdata->horse->name);
    free_string(ch->pcdata->horse->long_descr);
    ch->pcdata->horse->long_descr = str_dup(buf1);
    /*
    if ( IS_NPC( ch ) ) {
  free_string( ch->pIndexData->short_descr );
  ch->pIndexData->short_descr=str_dup( buf );
    }
    */
    return;
}

void set_cname(CHAR_DATA *ch, char *cname) {
    char buf[BUF_STRING_LENGTH];
    char buf1[BUF_STRING_LENGTH];

    buf[0] = '\0';

    strcpy(buf, cname);
    free_string(ch->cname);
    ch->cname = str_dup(buf);

    sprintf(buf1, "%s(%s)", cname, ch->name);
    free_string(ch->short_descr);
    ch->short_descr = str_dup(buf1);

    if (IS_SET(ch->act, PLR_HORSE)) {
        sprintf(buf, "%s(%s)乖乖地站在這裡。\n\r", ch->cname, ch->name);
        free_string(ch->long_descr);
        ch->long_descr = str_dup(buf);
    }

    if (IS_NPC(ch)) {
        free_string(ch->pIndexData->short_descr);
        ch->pIndexData->short_descr = str_dup(buf);
    }
    return;
}

void do_hname(CHAR_DATA *ch, char *arg) {
    char argument[MAX_INPUT_LENGTH];
    if (IS_NPC(ch))
        return;
    one_argument(arg, argument);

    if (argument[0] == '\0') {
        send_to_char("[33m要把馬的名字改成什麼？[m\n\r", ch);
        return;
    }

    if (!ch->pcdata->horse) {
        send_to_char(YEL "你沒有馬。" NOR "\n\r", ch);
        return;
    }

    if (IS_SET(ch->act, PLR_HORSE)) {
        send_to_char(YEL "馬不能幫自己取名字。" NOR "\n\r", ch);
        return;
    }

    if (strlen(argument) > 21) {
        argument[21] = '\0';
        send_to_char(YEL "你的馬名字太長了，請取一個短一點的。" NOR "\n\r", ch);
        return;
    }
    if (!check_parse_name(argument)) {
        send_to_char("那是不合法的名字，請重新取一個。\n\r", ch);
        return;
    }

    if (!str_prefix(argument, ch->name) || !str_prefix(ch->name, argument)) {
        send_to_char("馬的名字不能和主人太像。\r\n", ch);
        return;
    }

    if (!str_infix(argument, ch->name) || !str_infix(ch->name, argument)) {
        send_to_char("馬的名字不能和主人太像。\r\n", ch);
        return;
    }

    if (has_player(argument)) {
        send_to_char(YEL "不能用別的玩者的名字。" NOR "\n\r", ch);
        return;
    }

    smash_tilde(argument);
    set_hname(ch, argument);
    send_to_char("[36mOK.[m\n\r", ch);
}

void do_cname(CHAR_DATA *ch, char *argument) {
    char no_ansi[MAX_STRING_LENGTH];
    int len, len2;

    if (IS_NPC(ch))
        return;

    strip_ansi(argument, no_ansi);

    for (len = 0; len < 120; len++) {
        if (no_ansi[len] == '\0')
            break;
    }

    for (len2 = 0; len2 < 120; len2++) {
        if (argument[len2] == '\0')
            break;
    }

    if (len == 0) {
        send_to_char("[33m要把你的中文名字改成什麼？[m\n\r", ch);
        return;
    } else if (len > 12 || len2 > 100) {
        argument[12] = '\0';
        send_to_char(YEL "你的名字太長了，請取一個短一點的。" NOR "\n\r", ch);
        return;
    }

    smash_tilde(argument);
    set_cname(ch, argument);
    send_to_char("[36mOK.[m\n\r", ch);
}

int str_length(char *str) {
    int len = 0;
    int i, k;
    len = strlen(str);
    k = len;
    for (i = 0; i < len; i++) {
        if (str[i] == '' || str[i] == '[' || str[i] == 'm' || str[i] == ';' ||
            str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' ||
            str[i] == '4' || str[i] == '5' || str[i] == '6' || str[i] == '7' ||
            str[i] == '8' || str[i] == '9') {
            k--;
        }
    }
    return k;
}

void set_title(CHAR_DATA *ch, char *title) {
    char buf[BUF_STRING_LENGTH];

    if (IS_NPC(ch)) {
        bug("Set_title: NPC.", 0);
        return;
    }

    buf[0] = '\0';

    /*
    if ( isalpha( title[0] ) || isdigit( title[0] ) )
    {
  buf[0] = ' ';
  strcpy( buf+1, title );
    }
    else
    {*/
    strcpy(buf, title);
    // }

    free_string(ch->pcdata->title);
    ch->pcdata->title = str_dup(buf);
    return;
}

void do_title(CHAR_DATA *ch, char *argument) {
    if (IS_NPC(ch))
        return;

    if (argument[0] == '\0') {
        send_to_char("[33m要把你的稱號改成什麼？[m\n\r", ch);
        return;
    }

    if (str_length(argument) > 30) {
        argument[30] = '\0';
        send_to_char(YEL "你的稱號太長了，請想一個短一點的，響亮一點的。" NOR
                         "\n\r",
                     ch);
    }

    smash_tilde(argument);
    set_title(ch, argument);
    send_to_char("[36mOK.[m\n\r", ch);
}

void do_description(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];

    if (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) ||
        IS_SET(ch->act, ACT_PET))
        return;

    if (argument[0] != '\0') {
        buf[0] = '\0';
        smash_tilde(argument);
        if (argument[0] == '+') {
            if (strlen(buf) + strlen(argument) >= 500) {
                send_to_char("[33m描述太長了。[m\n\r", ch);
                return;
            }
            if (ch->description)
                strcat(buf, ch->description);
            argument++;
            while (isspace(*argument))
                argument++;
        }

        if (strlen(buf) + strlen(argument) >= 500) {
            send_to_char("[33m描述太長了。[m\n\r", ch);
            return;
        }

        strcat(buf, argument);
        strcat(buf, "\n\r");
        free_string(ch->description);
        ch->description = str_dup(buf);
    }

    send_to_char("你的描述是：\n\r", ch);
    send_to_char(ch->description ? ch->description : "（無）。\n\r", ch);
    return;
}

void do_report(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    sprintf(buf,
            "[36m你的狀態：%d/%d點生命力、%d/%d點魔法力、%d/"
            "%d點移動力、%d點經驗值。[m\n\r",
            ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move,
            ch->max_move, ch->exp);

    send_to_char(buf, ch);

    sprintf(buf,
            "[36m$n報告狀態：%d/%d點生命力、%d/%d點魔法力、%d/"
            "%d點移動力、%d點經驗值。[m\n\r",
            ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move,
            ch->max_move, ch->exp);

    act(buf, ch, NULL, NULL, TO_ROOM);

    return;
}

// practice modified by jye 6.07.2005
void do_practice(CHAR_DATA *ch, char *argument) {
    char buf[BUF_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 4];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *temp;
    int req_lv;

    int sn;
    int money = ch->level * ch->level * 20;
    bool can_learn;

    if (IS_NPC(ch))
        return;

    if (ch->class < 5)
        money /= 2;

    temp = argument;
    temp = one_argument(temp, arg1);
    one_argument(temp, arg2);

    buf1[0] = '\0';

    if (argument[0] == '\0') {
        CHAR_DATA *mob;
        int col;

        for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
            if (mob->deleted)
                continue;

            if (IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) && (mob->psk))
                break;
        }

        if (!mob || !mob->psk) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        col = 0;

        for (sn = 0; sn < MAX_SKILL; sn++) {
            can_learn = FALSE;
            if (!skill_table[sn].name)
                break;
            // if ( ch->pcdata->learned[sn] == 0 )
            // continue;
            if (skill_req_level(ch, sn) == L_APP)
                continue;
            if (skill_req_level(ch, sn) == -1 &&
                skill_preq_level(ch, sn) == L_APP)
                continue;

            if (is_skname(skill_table[sn].name, mob->psk) != -1)
                can_learn = TRUE;

            sprintf(buf, HIW "%c" NOR "%14s(%16s) %3d%% ",
                    can_learn ? '+' : ' ', skill_table[sn].cname,
                    skill_table[sn].name, get_skill_percent(ch, sn));
            strcat(buf1, buf);
            if (++col % 2 == 0)
                strcat(buf1, "\n\r");
        }

        if (col % 3 != 0)
            strcat(buf1, "\n\r");

        sprintf(buf, "你還有%d點練習點數。\n\r", ch->practice);
        strcat(buf1, buf);
        sprintf(buf, "練習的費用是%d枚金幣。\n\r", money);
        strcat(buf1, buf);
        send_to_char(buf1, ch);
    } else if (arg2[0] == '\0') {
        CHAR_DATA *mob;
        QMARK *qmark;
        int adept;

        if (ch->level < 3) {
            send_to_char("[33m你得要在三級以上才能練習(practice)，先去訓練("
                         "train)吧！[m\n\r",
                         ch);
            return;
        }

        if (!IS_AWAKE(ch)) {
            send_to_char("[33m是要在夢裡練習嗎？[m\n\r", ch);
            return;
        }

        for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
            if (mob->deleted)
                continue;

            if (IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE))
                break;
        }

        if (!mob || !mob->psk) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        if (ch->practice <= 0) {
            send_to_char("[33m你沒有練習點數了。[m\n\r", ch);
            return;
        } else if (money > ch->gold && ch->class > 4) {
            send_to_char("[33m你付不起練習的費用。[m\n\r", ch);
            return;
        }

        if ((sn = skill_lookup(argument)) < 0 ||
            ((skill_req_level(ch, sn) > -1 &&
              ch->level < skill_req_level(ch, sn)))) {
            send_to_char("[33m你不能練習那個。[m\n\r", ch);
            return;
        }

        if (skill_req_level(ch, sn) == -1) {
            // req_lv =
            // skill_table[sn].skill_level[apply_class[ch->pcdata->prime_attr]];
            req_lv = skill_preq_level(ch, sn);
            if (req_lv == L_APP) {
                ;
            } else
                req_lv = req_lv / 2 + 1;

            if (ch->level < req_lv) {
                send_to_char("[33m你不能練習那個。[m\n\r", ch);
                return;
            }
        }

        switch (is_skname(argument, mob->psk)) {
        case -1:
            act("$n 說：對不起，我並不會那項技巧。", mob, NULL, ch, TO_VICT);
            return;
        case 1:
            for (qmark = ch->qmark; qmark; qmark = qmark->next)
                if (!str_cmp(qmark->mark_name, skill_table[sn].name))
                    break;
            if (!qmark) {
                act("$n 說：對不起，你還不夠資格學習這項技巧。", mob, NULL, ch,
                    TO_VICT);
                return;
            }
            break;
        default:
            break;
        }

        // adept = IS_NPC( ch ) ? 100 : class_table[ch->class].skill_adept;
        adept = skill_max_percent(ch, sn);

        if ( // ch->pcdata->learned[sn] >= adept )
            get_skill_percent(ch, sn) >= adept) {
            sprintf(buf,
                    "[33m你已經無法提升%s(%s)的熟練度了。[m\n\r",
                    skill_table[sn].cname, skill_table[sn].name);
            send_to_char(buf, ch);
        } else {
            ch->practice--;
            if (ch->class > 4)
                ch->gold -= money;
            gain_skill_percent(ch, sn, int_app[get_curr_int(ch)].learn);

            if (get_skill_percent(ch, sn) < adept) {
                act("[36m你練習了$t($T)。[m", ch, skill_table[sn].cname,
                    skill_table[sn].name, TO_CHAR);
                act("[36m$n練習了$t($T)。[m", ch, skill_table[sn].cname,
                    skill_table[sn].name, TO_ROOM);
            } else {
                act("[36m你現在是$t($T)的專家了。[m", ch,
                    skill_table[sn].cname, skill_table[sn].name, TO_CHAR);
                act("[36m$n現在是$t($T)的專家了。[m", ch,
                    skill_table[sn].cname, skill_table[sn].name, TO_ROOM);
            }
        }
    } else // prac skill tutor
    {
        CHAR_DATA *mob;
        QMARK *qmark;
        int adept;

        for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
            if (mob->deleted)
                continue;

            if (IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) &&
                is_pfxname(arg2, mob->name))
                break;
        }

        if (!mob || !IS_SET(mob->act, ACT_PRACTICE)) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        if (!mob->psk) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }
        if (ch->practice <= 0) {
            send_to_char("[33m你沒有練習點數了。[m\n\r", ch);
            return;
        }

        if (!str_cmp(arg1, "+")) {
            int col;

            col = 0;

            for (sn = 0; sn < MAX_SKILL; sn++) {
                can_learn = FALSE;
                if (!skill_table[sn].name)
                    break;
                if (skill_req_level(ch, sn) == L_APP)
                    continue;
                if (skill_req_level(ch, sn) == -1 &&
                    skill_preq_level(ch, sn) == L_APP)
                    continue;

                if (is_skname(skill_table[sn].name, mob->psk) != -1)
                    can_learn = TRUE;

                sprintf(buf, HIW "%c" NOR "%14s(%16s) %3d%% ",
                        can_learn ? '+' : ' ', skill_table[sn].cname,
                        skill_table[sn].name, get_skill_percent(ch, sn));
                strcat(buf1, buf);
                if (++col % 2 == 0)
                    strcat(buf1, "\n\r");
            }

            if (col % 3 != 0)
                strcat(buf1, "\n\r");

            sprintf(buf, "你還有%d點練習點數。\n\r", ch->practice);
            strcat(buf1, buf);
            sprintf(buf, "練習的費用是%d枚金幣。\n\r", money);
            strcat(buf1, buf);
            send_to_char(buf1, ch);
            return;
        } else if (money > ch->gold && ch->class > 4) {
            send_to_char("[33m你付不起練習的費用。[m\n\r", ch);
            return;
        }

        if ((sn = skill_lookup(arg1)) < 0 ||
            ((skill_req_level(ch, sn) > -1 &&
              ch->level < skill_req_level(ch, sn)))) {
            send_to_char("[33m你不能練習那個。[m\n\r", ch);
            return;
        }
        if (skill_req_level(ch, sn) == -1) {
            req_lv = skill_preq_level(ch, sn);
            if (req_lv == L_APP) {
                ;
            } else
                req_lv = req_lv / 2 + 1;

            if (ch->level < req_lv) {
                send_to_char("[33m你不能練習那個。[m\n\r", ch);
                return;
            }
        }

        switch (is_skname(arg1, mob->psk)) {
        case -1:
            act("$n 說：對不起，我並不會那項技巧。", mob, NULL, ch, TO_VICT);
            return;
        case 1:
            for (qmark = ch->qmark; qmark; qmark = qmark->next)
                if (!str_cmp(qmark->mark_name, skill_table[sn].name))
                    break;
            if (!qmark) {
                act("$n 說：對不起，你還不夠資格學習這項技巧。", mob, NULL, ch,
                    TO_VICT);
                return;
            }
            break;
        default:
            break;
        }

        // adept = IS_NPC( ch ) ? 100 : class_table[ch->class].skill_adept;
        adept = skill_max_percent(ch, sn);

        if (get_skill_percent(ch, sn) >= adept) {
            sprintf(buf,
                    "[33m你已經無法提升%s(%s)的熟練度了。[m\n\r",
                    skill_table[sn].cname, skill_table[sn].name);
            send_to_char(buf, ch);
        } else {
            ch->practice--;
            if (ch->class > 4)
                ch->gold -= money;
            gain_skill_percent(ch, sn, int_app[get_curr_int(ch)].learn);
            if (get_skill_percent(ch, sn) < adept) {
                act("[36m你練習了$t($T)。[m", ch, skill_table[sn].cname,
                    skill_table[sn].name, TO_CHAR);
                act("[36m$n練習了$t($T)。[m", ch, skill_table[sn].cname,
                    skill_table[sn].name, TO_ROOM);
            } else {
                act("[36m你現在是$t($T)的專家了。[m", ch,
                    skill_table[sn].cname, skill_table[sn].name, TO_CHAR);
                act("[36m$n現在是$t($T)的專家了。[m", ch,
                    skill_table[sn].cname, skill_table[sn].name, TO_ROOM);
            }
        }
    }
    return;
}
/* return -1 : mob don't teach this skill
 * return  0 : mob teach this skill, and no need to check pc's qmark
 * return  1 : mob teach this skill, but need pc has corresponding qmark
 */
int is_skname(const char *str, char *namelist) {
    char name[MAX_INPUT_LENGTH];
    char *nameptr;
    bool qualify;

    if (!namelist)
        return FALSE;
    for (;;) {
        qualify = FALSE;
        namelist = one_argument(namelist, name);
        if (name[0] == '\0')
            return -1;

        nameptr = name;
        if (name[0] == '*') {
            nameptr += 1;
            qualify = TRUE;
        }

        if (!str_prefix(str, nameptr))
            return qualify ? 1 : 0;
    }
}

void do_join(CHAR_DATA *ch, char *argument) {
    char buf[BUF_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 2];
    int complement[MAX_CLASS] = {3, 4, 2, 1, 5, 3, 3, 4, 4, 2,
                                 2, 1, 1, 1, 5, 3, 4, 2, 5, 5};
    int cn;
    int money = ch->level * ch->level * 40;

    if (IS_NPC(ch) || ch->class > 4) {
        send_to_char("你不需要再轉職了。\n\r", ch);
        return;
    }

    buf1[0] = '\0';

    if (ch->level < 20) {
        send_to_char("[33m你得要在二十級以上才能轉職(join)，先設法升級吧！["
                     "m\n\r",
                     ch);
        return;
    }

    if (argument[0] == '\0') {
        CHAR_DATA *mob;
        int col;

        for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
            if (mob->deleted)
                continue;
            if (IS_NPC(mob) && (mob->jcl))
                break;
        }

        if (!mob) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        col = 0;

        strcat(buf1, "你能夠加入的進階職業有：\n\r");
        for (cn = 5; cn < MAX_CLASS; cn++) {
            if (!class_table[cn].name)
                break;
            if (class_table[cn].bclass[ch->class] == L_APP)
                continue;

            if (is_name(class_table[cn].name, mob->jcl)) {
                sprintf(buf, "%s ( %s ), ", class_table[cn].cname,
                        class_table[cn].name);
                strcat(buf1, buf);
                if (++col % 4 == 0)
                    strcat(buf1, "\n\r");
            }
        }

        if (col % 4 != 0)
            strcat(buf1, "\n\r");

        sprintf(buf, "轉職所需的費用是%d枚金幣。\n\r", money);
        strcat(buf1, buf);
        send_to_char(buf1, ch);
    } else {
        CHAR_DATA *mob;
        OBJ_DATA *obj;
        if (!IS_AWAKE(ch)) {
            send_to_char("[33m是要在夢裡轉職嗎？[m\n\r", ch);
            return;
        }

        for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
            if (mob->deleted)
                continue;
            if (IS_NPC(mob) && (mob->jcl))
                break;
        }

        if (!mob) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        if (money > ch->gold) {
            send_to_char("[33m你付不起轉職的費用。[m\n\r", ch);
            return;
        }

        if ((cn = class_lookup(argument)) < 0 ||
            !is_name(class_table[cn].name, mob->jcl))

        {
            send_to_char("[33m你無法在此加入那個職業。[m\n\r",
                         ch);
            return;
        }

        if (!IS_NPC(ch) && class_table[cn].bclass[ch->class] > ch->level) {
            sprintf(
                buf,
                "[33m你至少要到%d級才能加入那門職業。[m\n\r",
                class_table[cn].bclass[ch->class]);
            send_to_char(buf, ch);
            return;
        }

        do_remove(ch, "all");

        for (obj = ch->carrying; obj; obj = obj->next_content) {
            if (obj->wear_loc != WEAR_NONE) {
                unequip_char(ch, obj);
                act("$n stops using $p.", ch, obj, NULL, TO_ROOM);
                act("You stop using $p.", ch, obj, NULL, TO_CHAR);
            }
        }

        die_follower(ch, "");

        ch->gold -= money;
        ch->max_hit = UMAX(100, ch->max_hit / 2);
        ch->max_mana = UMAX(100, ch->max_mana / 2);
        ch->max_move = UMAX(100, ch->max_move / 2);
        ch->pcdata->snd_attr = class_table[cn].attr_prime;
        ch->level = 1;
        ch->exp = 1000;
        ch->dodge += race_table[ch->race].nature_dodge;
        ch->armor += race_table[ch->race].nature_ac;
        // for(int i=0; i< 6; ++i)
        //   ch->saving_throw[i] += race_table[ ch->race ].nature_svs[i];
        if (ch->pcdata->prime_attr == ch->pcdata->snd_attr) {
            ch->pcdata->snd_attr = complement[cn];
        }
        ch->class = cn;

        do_wimpy(ch, "");
        do_save(ch, "");

        sprintf(buf, HIW "%s 大聲地宣布：歡迎 %s 成為 %s 的成員！\n\r" NOR,
                mob->short_descr, ch->name, class_table[cn].cname);
        send_to_all_char(buf);

        sprintf(buf, HIP "恭喜你！你現在可以開始提升技能等級了！\n\r"
                         "在解過任務或擊敗特定目標，記得向技能小天使(Angel of "
                         "Skill)領取技能經驗點喔！\n\r"
                         "\n\r"
                         "關於技能等級系統，請輸入 help skexp, help skadv 及 "
                         "help skdemote 參考說明文件\n\r" NOR);
        send_to_char(buf, ch);
        gain_skill_exp(ch, 100000);
        send_to_char("\n\r你得到了 10 萬點技能經驗點！\n\r", ch);
    }
    return;
}

/*
 * Lookup a class by name.
 */
int class_lookup(const char *name) {
    int cn;

    for (cn = 0; cn < MAX_CLASS; cn++) {
        if (!class_table[cn].name)
            break;
        if (LOWER(name[0]) == LOWER(class_table[cn].name[0]) &&
            !str_prefix(name, class_table[cn].name))
            return cn;
    }

    return -1;
}

/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy(CHAR_DATA *ch, char *argument) {
    char buf[BUF_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument(argument, arg);

    if (arg[0] == '\0')
        wimpy = ch->max_hit / 5;
    else
        wimpy = atoi(arg);

    if (wimpy < 0) {
        send_to_char("[33m你的勇氣要比你的智慧來得高。[m\n\r",
                     ch);
        return;
    }

    if (wimpy > ch->max_hit / 2) {
        send_to_char("[33m你是膽小鬼呀？[m\n\r", ch);
        return;
    }

    ch->wimpy = wimpy;
    sprintf(buf,
            "[36mWimpy設定到%d點生命值以下會自動逃跑。[m\n\r",
            wimpy);
    send_to_char(buf, ch);
    return;
}

void do_password(CHAR_DATA *ch, char *argument) {
    char *pArg;
    char *pwdnew;
    char *p;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char cEnd;

    if (IS_NPC(ch))
        return;

    if (IS_SET(ch->act, PLR_HORSE)) {
        send_to_char("馬是不用 password 的。\n\r", ch);
        return;
    }
    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while (isspace(*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0') {
        if (*argument == cEnd) {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while (isspace(*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0') {
        if (*argument == cEnd) {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';
    *argument = '\0';

    if (arg1[0] == '\0' || arg2[0] == '\0') {
        send_to_char("[33m格式：password <舊密碼> <新密碼>。[m\n\r",
                     ch);
        return;
    }

    if (strcmp(crypt(arg1, ch->pcdata->pwd), ch->pcdata->pwd)) {
        WAIT_STATE(ch, 40);
        send_to_char("[33m密碼錯誤。等十秒鐘。[m\n\r", ch);
        return;
    }

    if (strlen(arg2) < 5) {
        send_to_char("[33m新的密碼最少得要有五個字。[m\n\r", ch);
        return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt(arg2, ch->name);
    for (p = pwdnew; *p != '\0'; p++) {
        if (*p == '~') {
            send_to_char("[33m新的密碼不能用，試試別的。[m\n\r",
                         ch);
            return;
        }
    }

    free_string(ch->pcdata->pwd);
    ch->pcdata->pwd = str_dup(pwdnew);
    save_char_obj(ch);
    send_to_char("[36mOK.[m\n\r", ch);
    return;
}

void do_socials(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 4];
    int iSocial;
    int col;

    buf1[0] = '\0';
    col = 0;
    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++) {
        sprintf(buf, "%-12s", social_table[iSocial].name);
        strcat(buf1, buf);
        if (++col % 6 == 0)
            strcat(buf1, "\n\r");
    }

    if (col % 6 != 0)
        strcat(buf1, "\n\r");

    send_to_char(buf1, ch);
    return;
}

/*
 * Contributed by Alander.
 */
void do_commands(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 4];
    int cmd;
    int col;

    buf1[0] = '\0';
    col = 0;
    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++) {
        if (cmd_table[cmd].level < LEVEL_HERO &&
            cmd_table[cmd].level <= get_trust(ch) && cmd_table[cmd].toPPL) {
            sprintf(buf, "%-16s", cmd_table[cmd].name);
            strcat(buf1, buf);
            if (++col % 5 == 0)
                strcat(buf1, "\n\r");
        }
    }

    if (col % 5 != 0)
        strcat(buf1, "\n\r");

    send_to_char(buf1, ch);
    return;
}

// do_channels modified by jye 6.12.2005
void do_channels(CHAR_DATA *ch, char *argument) {
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    // char bug [ MAX_INPUT_LENGTH  ];
    char buf[MAX_INPUT_LENGTH];
    CLAN_DATA *cn = clan_list;
    CHAR_DATA *vch = char_list;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (arg1[0] == '\0') {
        if (!IS_NPC(ch) &&
            (IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE))) {
            send_to_char("[33m你被禁止使用頻道了。[m\n\r", ch);
            return;
        }

        send_to_char("[36m頻道：", ch);

        send_to_char(
            !IS_SET(ch->deaf, CHANNEL_AUCTION) ? " +AUCTION" : " -auction", ch);

        send_to_char(!IS_SET(ch->deaf, CHANNEL_CHAT) ? " +CHAT" : " -chat", ch);

        if (IS_IMMORTAL(ch)) {
            send_to_char(!IS_SET(ch->deaf, CHANNEL_IMMTALK) ? " +IMMTALK"
                                                            : " -immtalk",
                         ch);
        }
        if (IS_HERO(ch)) {
            send_to_char(!IS_SET(ch->deaf, CHANNEL_HEROTALK) ? " +HEROTALK"
                                                             : " -herotalk",
                         ch);
        }
        if (IS_IMMORTAL(ch)) {
            send_to_char(!IS_SET(ch->deaf, CHANNEL_SYSTEM) ? " +SYSTEM"
                                                           : " -system",
                         ch);
        }

        send_to_char(!IS_SET(ch->deaf, CHANNEL_MUSIC) ? " +MUSIC" : " -music",
                     ch);

        send_to_char(!IS_SET(ch->deaf, CHANNEL_QUESTION) ? " +QUESTION"
                                                         : " -question",
                     ch);

        send_to_char(!IS_SET(ch->deaf, CHANNEL_SHOUT) ? " +SHOUT" : " -shout",
                     ch);

        send_to_char(!IS_SET(ch->deaf, CHANNEL_YELL) ? " +YELL" : " -yell", ch);

        send_to_char(!IS_SET(ch->deaf, CHANNEL_RUMOR) ? " +RUMOR" : " -rumor",
                     ch);

        send_to_char("。[m\n\r", ch);

        if (ch->trust >= 44 || ch->level >= 44) {
            send_to_char("你所接收的幫會頻道：", ch);

            if (!str_cmp(ch->clan_channel, "all")) {
                send_to_char("所有幫會", ch);
            } else if (ch->clan_channel) {
                send_to_char(ch->clan_channel, ch);
            } else {
                send_to_char("無", ch);
            }

            send_to_char("！\n\r", ch);
            send_to_char("幫會名稱列表：\n\r", ch);

            for (cn = clan_list; cn; cn = cn->next) {
                sprintf(buf, "%s (%s     ) ", cn->whoname, cn->name);
                send_to_char(buf, ch);
                send_to_char("\n\r", ch);
            }
        }
    } else {
        int bit;
        bool fClear;

        if (ch->trust >= 44 || ch->level >= 44) {

            if (!str_cmp(arg1, "tell")) {
                if (arg2[0] == '\0')
                    return;
                if (ch->trust >= 45 || ch->level >= 45) {
                    if (!str_cmp(arg2, "all")) {
                        ch->tell_channel = str_dup(arg2);
                        sprintf(buf, HIW "你啟動了衛星竊聽系統！" NOR "\n\r");
                        send_to_char(buf, ch);
                        return;
                    } else if (!str_cmp(arg2, "close")) {
                        ch->tell_channel = NULL;
                        sprintf(buf, HIW "你關閉了衛星竊聽系統！" NOR "\n\r");
                        send_to_char(buf, ch);
                        return;
                    } else {
                        for (vch = char_list; vch; vch = vch->next) {
                            if (!str_cmp(vch->name, arg2)) {
                                if (IS_IMMORTAL(vch)) {
                                    sprintf(buf,
                                            HIB "你想偷偷在%s身上裝竊聽器！" NOR
                                                "\n\r",
                                            arg2);
                                    send_to_char(buf, ch);
                                    sprintf(buf,
                                            HIW "啊被發現了！！" NOR "\n\r");
                                    send_to_char(buf, ch);
                                    return;
                                } else {
                                    ch->tell_channel = str_dup(arg2);
                                    sprintf(buf,
                                            HIB "你偷偷在%s身上裝了竊聽器！" NOR
                                                "\n\r",
                                            arg2);
                                    send_to_char(buf, ch);
                                    return;
                                }
                            }
                        }
                    }
                }
            } else if (!str_cmp(arg1, "all")) {
                ch->clan_channel = str_dup(arg1);
                send_to_char("你開始接收所有幫會頻道！\n\r", ch);
                return;
            } else if (!str_cmp(arg1, "close")) {
                ch->clan_channel = NULL;
                send_to_char("你關閉幫會頻道！\n\r", ch);
                send_to_char("\n\r", ch);
                return;
            } else {
                for (cn = clan_list; cn; cn = cn->next) {
                    if (!str_cmp(cn->name, arg1) ||
                        !str_cmp(cn->whoname, arg1)) {
                        sprintf(buf, "你開始接收%s的幫會頻道！\n\r",
                                cn->whoname);
                        send_to_char(buf, ch);
                        ch->clan_channel = cn->whoname;
                        return;
                    }
                }
            }
        }

        if (arg1[0] == '+')
            fClear = TRUE;
        else if (arg1[0] == '-')
            fClear = FALSE;
        else {
            send_to_char("[33mChannels -channel or +channel?[m\n\r", ch);
            return;
        }

        if (!str_cmp(arg1 + 1, "auction"))
            bit = CHANNEL_AUCTION;
        else if (!str_cmp(arg1 + 1, "chat"))
            bit = CHANNEL_CHAT;
        else if (!str_cmp(arg1 + 1, "immtalk"))
            bit = CHANNEL_IMMTALK;
        else if (!str_cmp(arg1 + 1, "music"))
            bit = CHANNEL_MUSIC;
        else if (!str_cmp(arg1 + 1, "question"))
            bit = CHANNEL_QUESTION;
        else if (!str_cmp(arg1 + 1, "shout"))
            bit = CHANNEL_SHOUT;
        else if (!str_cmp(arg1 + 1, "yell"))
            bit = CHANNEL_YELL;
        else if (!str_cmp(arg1 + 1, "rumor"))
            bit = CHANNEL_RUMOR;
        else if (!str_cmp(arg1 + 1, "system"))
            bit = CHANNEL_SYSTEM;
        else if (!str_cmp(arg1 + 1, "herotalk"))
            bit = CHANNEL_HEROTALK;
        else {
            send_to_char("[33m要開關哪一個頻道？[m\n\r", ch);
            return;
        }

        if (fClear)
            REMOVE_BIT(ch->deaf, bit);
        else
            SET_BIT(ch->deaf, bit);

        send_to_char("OK！\n\r", ch);
    }

    return;
}

/*
 * Contributed by Grodyn.
 */
void do_config(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("[36m[  關鍵字  ] 選項\n\r[m", ch);

        send_to_char(IS_SET(ch->act, PLR_AUTOEXIT)
                         ? "[+AUTOEXIT ] 你自動找出口。\n\r"
                         : "[-autoexit ] 你不自動找出口。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_AUTOLOOT)
                         ? "[+AUTOLOOT ] 你自動從屍體裡掠取戰利品。\n\r"
                         : "[-autoloot ] 你不自動從屍體裡掠取戰利品。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_AUTOGOLD)
                         ? "[+AUTOGOLD ] 你自動從屍體裡拿金幣。\n\r"
                         : "[-autogold ] 你不自動從屍體裡拿金幣。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_AUTOSAC)
                         ? "[+AUTOSAC  ] 你自動奉獻屍體。\n\r"
                         : "[-autosac  ] 你不自動奉獻屍體。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_AUTOSPLIT)
                         ? "[+AUTOSPLIT] 你自動分享金幣。\n\r"
                         : "[-autosplit] 你不自動分享金幣。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_BLANK)
                         ? "[+BLANK    ] 你在提示字串前有一行空白。\n\r"
                         : "[-blank    ] 你在提示字串前沒有一行空白。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_BRIEF)
                         ? "[+BRIEF    ] 你要看簡短的描述。\n\r"
                         : "[-brief    ] 你要看完整的描述。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_COMBINE)
                         ? "[+COMBINE  ] 你使用結合的方式看物品清單。\n\r"
                         : "[-combine  ] 你使用單獨的方式看物品清單。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_PROMPT)
                         ? "[+PROMPT   ] 你有提示字串。\n\r"
                         : "[-prompt   ] 你沒有提示字串。\n\r",
                     ch);
        /*
          send_to_char(  IS_SET( ch->act, PLR_TELNET_GA )
              ? "[+TELNETGA ] 你接受telnet GA字串。\n\r"
              : "[-telnetga ] 你不接受telnet GA字串。\n\r"
              , ch );
        */
        send_to_char(IS_SET(ch->act, PLR_ARTIFACT)
                         ? "[+ARTIFACT ] 你使用裝備的隱藏力量。\n\r"
                         : "[-artifact ] 你不使用裝備的隱藏力量。\n\r",
                     ch);

        send_to_char(IS_SET(ch->act, PLR_SILENCE)
                         ? "[+SILENCE  ] 你被沈默化了。\n\r"
                         : "",
                     ch);

        send_to_char(!IS_SET(ch->act, PLR_NO_EMOTE)
                         ? ""
                         : "[-emote    ] 你不可以表達情感。\n\r",
                     ch);

        send_to_char(!IS_SET(ch->act, PLR_NO_TELL)
                         ? ""
                         : "[-tell     ] 你不能用「tell」。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_COMBAT)
                         ? "[+COMBAT   ] 你接收完整戰鬥訊息。\n\r"
                         : "[-combat   ] 你僅接受精簡戰鬥訊息。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_CHINESE)
                         ? "[+CHINESE  ] 你啟動中文模式。\n\r"
                         : "[-chinese  ] 你不啟動中文模式。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_STATUS)
                         ? "[+STATUS   ] 你啟動戰鬥血量計分表。\n\r"
                         : "[-status   ] 你關閉戰鬥血量計分表。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_TITLE)
                         ? "[+TITLE    ] 你的稱號不會變動。\n\r"
                         : "[-title    ] 你的稱號會自動改變。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_ASSIST)
                         ? "[+ASSIST   ] 你自動協助隊友作戰。\n\r"
                         : "[-assist   ] 你不協助隊友作戰。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_NOTIFY)
                         ? "[+NOTIFY   ] 你接收系統提示訊息。\n\r"
                         : "[-notify   ] 你不想接收系統提示訊息。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_DEAD)
                         ? "[+DEAD     ] 你死亡之後將自動離開隊伍。\n\r"
                         : "[-dead     ] 你死亡之後將不會自動離開隊伍。\n\r",
                     ch);
        send_to_char(IS_SET(ch->act, PLR_GRAPHIC)
                         ? "[+GRAPHIC  ] 你使用圖形化隊伍資料。\n\r"
                         : "[-graphic  ] 你不使用圖形化隊伍資料。\n\r",
                     ch);
        if (IS_IMMORTAL(ch)) {
            send_to_char(IS_SET(ch->act, PLR_VERBOSEALL)
                             ? "[+VERBOSEALL] 你顯示全部詳細資料。\n\r"
                             : "[-verboseall] 你不想顯示全部詳細資料。\n\r",
                         ch);
        }
    } else {
        char buf[BUF_STRING_LENGTH];
        long long int bit;
        bool fSet;

        if (arg[0] == '+')
            fSet = TRUE;
        else if (arg[0] == '-')
            fSet = FALSE;
        else {
            send_to_char("[33mConfig -option or +option?[m\n\r", ch);
            return;
        }

        if (!str_cmp(arg + 1, "autoexit"))
            bit = PLR_AUTOEXIT;
        else if (!str_cmp(arg + 1, "autoloot"))
            bit = PLR_AUTOLOOT;
        else if (!str_cmp(arg + 1, "autogold"))
            bit = PLR_AUTOGOLD;
        else if (!str_cmp(arg + 1, "autosac"))
            bit = PLR_AUTOSAC;
        else if (!str_cmp(arg + 1, "blank"))
            bit = PLR_BLANK;
        else if (!str_cmp(arg + 1, "brief"))
            bit = PLR_BRIEF;
        else if (!str_cmp(arg + 1, "combine"))
            bit = PLR_COMBINE;
        else if (!str_cmp(arg + 1, "prompt"))
            bit = PLR_PROMPT;
        else if (!str_cmp(arg + 1, "notify"))
            bit = PLR_NOTIFY;
        else if (!str_cmp(arg + 1, "artifact"))
            bit = PLR_ARTIFACT;
        else if (!str_cmp(arg + 1, "combat"))
            bit = PLR_COMBAT;
        else if (!str_cmp(arg + 1, "chinese"))
            bit = PLR_CHINESE;
        else if (!str_cmp(arg + 1, "status"))
            bit = PLR_STATUS;
        else if (!str_cmp(arg + 1, "title"))
            bit = PLR_TITLE;
        else if (!str_cmp(arg + 1, "assist"))
            bit = PLR_ASSIST;
        else if (!str_cmp(arg + 1, "autosplit"))
            bit = PLR_AUTOSPLIT;
        else if (!str_cmp(arg + 1, "dead"))
            bit = PLR_DEAD;
        else if (!str_cmp(arg + 1, "graphic"))
            bit = PLR_GRAPHIC;
        else if (!str_cmp(arg + 1, "verboseall")) {
            if (!IS_IMMORTAL(ch)) {
                return;
            }
            bit = PLR_VERBOSEALL;
        } else {
            send_to_char("[33m要設哪一個選項？[m\n\r", ch);
            return;
        }

        if (fSet) {
            SET_BIT(ch->act, bit);
            sprintf(buf, "[36m%s現在打開了。[m\n\r", arg + 1);
            buf[5] = UPPER(buf[5]);
            send_to_char(buf, ch);
        } else {
            REMOVE_BIT(ch->act, bit);
            sprintf(buf, "[36m%s現在關起來了。[m\n\r", arg + 1);
            buf[5] = UPPER(buf[5]);
            send_to_char(buf, ch);
        }
    }

    return;
}

void do_wizlist(CHAR_DATA *ch, char *argument) {

    do_help(ch, "wizlist");
    return;
}

void do_spells(CHAR_DATA *ch, char *argument) {
    char buf[BUF_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 4];
    int sn;
    int col;

    if (IS_NPC(ch))
    //|| ( !IS_NPC( ch ) && !class_table[ch->class].fMana ) )
    {
        send_to_char("[33m你不需要使用咒文。[m\n\r", ch);
        return;
    }

    buf1[0] = '\0';

    col = 0;
    strcat(buf, "咒   文   列   表：\n\r");
    for (sn = 1; sn < MAX_SKILL; sn++) {
        if (!skill_table[sn].name)
            break;

        //|| ( skill_table[sn].skill_level[ch->class] > LEVEL_HERO ) )
        if ((skill_table[sn].spell_fun == spell_null))
            continue;

        // if(ch->pcdata->learned[sn] == 0) continue;
        if (!get_skill_percent(ch, sn))
            continue;

        sprintf(buf, "%15s(%17s) %3d點", skill_table[sn].cname,
                skill_table[sn].name, MANA_COST(ch, sn));
        strcat(buf1, buf);

        if (++col % 2 == 0)
            strcat(buf1, "\n\r");
    }

    if (col % 2 != 0)
        strcat(buf1, "\n\r");

    send_to_char(buf1, ch);
    return;
}

int skill_eff_percent(CHAR_DATA *ch, int sn) {
    if (IS_NPC(ch))
        return 0;
    if (ch->class < WIZARD) {
        return skill_max_percent(ch, sn);
    } else
        return UMAX(skill_max_percent(ch, sn),
                    ((skill_table[sn]
                          .skill_level[apply_class[ch->pcdata->prime_attr]]) /
                     1000));
}

void do_slist(CHAR_DATA *ch, char *argument) {
    char buf[BUF_STRING_LENGTH * 2];
    char buf1[MAX_STRING_LENGTH * 8];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int sn;
    int col;
    int level;
    int max_level;
    bool pSpell;
    bool fFound;
    int count = 0;
    int skdif = 0;

    one_argument(argument, arg);

    if (arg[0] == '\0')
        victim = ch;
    else {
        if (!(victim = get_char_world(ch, arg))) {
            send_to_char("這裡沒有這個人。\n\r", ch);
            return;
        }

        if ((!IS_SET(victim->act, PLR_HORSE) || victim != ch->pcdata->horse ||
             victim->in_room != ch->in_room) &&
            !IS_IMMORTAL(ch)) {
            send_to_char("你不能看別人的資料。\n\r", ch);
            return;
        }
    }

    if (IS_NPC(victim)) {
        send_to_char("[33m你不需要用到技能。[m\n\r", ch);
        return;
    }

    buf1[0] = '\0';

    strcat(buf1, "你的職業可以用的技能如下：\n\r\n\r");
    strcat(buf1, "等級          技能/魔法名稱                   熟練度     "
                 "技能等級  所需經驗\n\r\n\r");
    //              [ 1]  123456789012345678(123456789012345678) 123%

    max_level = (victim->level < L_APP) ? 42 : L_APP;
    fFound = FALSE;
    level = 0;
    // 避免下指令一直跑出bug log overflow[0] 將 sn 修改為從 1 開始算 2021/12/20
    // for ( sn = 0; sn < MAX_SKILL; sn++ )
    for (sn = 1; sn < MAX_SKILL; sn++) {
        if (!skill_table[sn].name)
            break;
        if (skill_req_level(victim, sn) != level &&
            !(level == 0 && get_skill_percent(victim, sn) != 0 &&
              skill_req_level(victim, sn) == L_APP))
            // skill_table[sn].skill_level[victim->class] != level &&
            // victim->pcdata->learned[sn] != 0 &&
            // all skills which basic class can learn but adv class can't will
            // show behind [ 1] skill_table[sn].skill_level[victim->class] ==
            // L_APP ) )
            continue;

        fFound = TRUE;
        break;
    }
    for (level = (fFound) ? 0 : 1; level <= max_level; level++) {

        col = 0;
        pSpell = TRUE;
        // 避免下指令一直跑出bug log overflow[0] 將 sn 修改為從 1 開始算
        // 2021/12/20
        // for ( sn = 0; sn < MAX_SKILL; sn++ )
        for (sn = 1; sn < MAX_SKILL; sn++) {
            if (!skill_table[sn].name)
                break;
            if (skill_req_level(victim, sn) > -1 &&
                skill_req_level(victim, sn) != level &&
                !(level == 0 && get_skill_percent(victim, sn) != 0 &&
                  skill_req_level(victim, sn) == L_APP))
                // skill_table[sn].skill_level[victim->class] > -1
                //&& skill_table[sn].skill_level[victim->class] != level&&
                // victim->pcdata->learned[sn] != 0 &&
                // all skills which basic class can learn but adv class can't
                // will show behind [ 1]
                // skill_table[sn].skill_level[victim->class] == L_APP ) )
                continue;
            // if(skill_table[sn].skill_level[victim->class] == -1){
            if (skill_req_level(victim, sn) == -1) {
                // if(sskill_table[sn].skill_level[apply_class[victim->pcdata->prime_attr]]
                // < L_APP) {
                if (skill_preq_level(victim, sn) < L_APP) {
                    // if((skill_table[sn].skill_level[apply_class[victim->pcdata->prime_attr]]
                    // / 2 +1  )!= level) {
                    if ((skill_preq_level(victim, sn) / 2 + 1) != level) {
                        continue;
                    }
                } else
                    continue;
            }

            if (pSpell) {
                sprintf(buf, "[%2d]  ", level);
                strcat(buf1, buf);
                pSpell = FALSE;
            } else {
                strcat(buf1, "      ");
            }
            // else if ( col++ % 4 == 0 )
            //   strcat ( buf1, "    " );

            ++count;
            if (victim->class < WIZARD) {
                // sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s \n\r",
                // skill_table[sn].cname,
                sprintf(buf, "%16s(%25s)  %3d%%/%3d%%   %8s \n\r",
                        skill_table[sn]
                            .cname, // 部分技能名過長,修為 25 字 2021/10/10
                        skill_table[sn].name,
                        // victim->pcdata->learned[sn]
                        get_skill_percent(victim, sn),
                        skill_eff_percent(victim, sn),
                        skill_exp_desc(get_skill_level(victim, sn)));
            } else {
                skdif = skill_difficulty(victim, sn);
                if (skdif >= 0) {
                    // sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s [%6d]\n\r",
                    // skill_table[sn].cname,
                    sprintf(buf, "%16s(%25s)  %3d%%/%3d%%   %8s  [%6d]\n\r",
                            skill_table[sn].cname, skill_table[sn].name,
                            // victim->pcdata->learned[sn]
                            get_skill_percent(victim, sn),
                            skill_eff_percent(victim, sn),
                            skill_exp_desc(get_skill_level(victim, sn)),
                            skill_difficulty(victim, sn));
                } else {
                    // sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s
                    // [ＭＡＸ]\n\r", skill_table[sn].cname,
                    sprintf(buf, "%16s(%25s)  %3d%%/%3d%%   %8s  [ＭＡＸ]\n\r",
                            skill_table[sn].cname, skill_table[sn].name,
                            // victim->pcdata->learned[sn]
                            get_skill_percent(victim, sn),
                            skill_eff_percent(victim, sn),
                            skill_exp_desc(get_skill_level(victim, sn)));
                    //,skill_difficulty(victim,sn) );
                }
            }
            strcat(buf1, buf);
            // 修改最大 pagelength 為 70, 故可顯示上限提高 2023/01/12
            // if( count >= 50 ) {
            if (count >= 80) {
                send_to_char(buf1, ch);
                count = 0;
                buf1[0] = '\0';
            }
        }

        // if ( col % 4 != 0 )
        //	strcat ( buf1, "\n\r" );
    }

    send_to_char(buf1, ch);
    return;
}

/* bypassing the config command - Kahn */

void do_autoexit(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_AUTOEXIT) ? sprintf(buf, "-autoexit")
                                   : sprintf(buf, "+autoexit"));

    do_config(ch, buf);

    return;
}

void do_autoloot(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_AUTOLOOT) ? sprintf(buf, "-autoloot")
                                   : sprintf(buf, "+autoloot"));

    do_config(ch, buf);

    return;
}

void do_autogold(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_AUTOGOLD) ? sprintf(buf, "-autogold")
                                   : sprintf(buf, "+autogold"));

    do_config(ch, buf);

    return;
}

void do_autosac(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_AUTOSAC) ? sprintf(buf, "-autosac")
                                  : sprintf(buf, "+autosac"));

    do_config(ch, buf);

    return;
}

void do_blank(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_BLANK) ? sprintf(buf, "-blank")
                                : sprintf(buf, "+blank"));

    do_config(ch, buf);

    return;
}

void do_brief(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_BRIEF) ? sprintf(buf, "-brief")
                                : sprintf(buf, "+brief"));

    do_config(ch, buf);

    return;
}

void do_combine(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];

    (IS_SET(ch->act, PLR_COMBINE) ? sprintf(buf, "-combine")
                                  : sprintf(buf, "+combine"));

    do_config(ch, buf);

    return;
}

void do_pagelen(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int lines;

    if (IS_NPC(ch))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0')
        lines = 40;
    else
        lines = atoi(arg);

    if (lines < 19) {
        send_to_char("[33m每頁不能少於十九行。[m", ch);
        return;
    }

    if (lines > 70) {
        send_to_char("[33m請勿設定至超過七十行！[m\n\r", ch);
        lines = 70;
    }

    ch->pcdata->pagelen = lines;
    sprintf(buf, "[36m分頁設定設到一頁%d行。[m\n\r", lines);
    send_to_char(buf, ch);
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt(CHAR_DATA *ch, char *argument) {
    char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';

    /* Unswitched NPC's get kicked out */
    if (!ch->desc)
        return;

    /* Will always have a pc ch after this */
    if (IS_NPC(ch))
        ch = (ch->desc->original ? ch->desc->original : ch->desc->character);

    if (argument[0] == '\0') {
        (IS_SET(ch->act, PLR_PROMPT) ? sprintf(buf, "-prompt")
                                     : sprintf(buf, "+prompt"));

        do_config(ch, buf);
        return;
    }

    if (!strcmp(argument, "all"))
        strcat(buf,
               "[m<生命[1;32m%h[m魔法[1;33m%m[m移動[1;35m%v[m>");
    else if (!strcmp(argument, "max")) {
        strcat(buf, NOR "< %h/" HIC "%H " NOR "%m/" HIC "%M " NOR "%v/" HIC
                        "%V" NOR " >");
    } else {
        if (strlen(argument) > 200)
            argument[200] = '\0';
        smash_tilde(argument);
        strcat(buf, argument);
    }
    strcat(buf, " ");

    free_string(ch->pcdata->prompt);
    ch->pcdata->prompt = str_dup(buf);
    do_config(ch, "+prompt");
    send_to_char("[36m沒問題。[m\n\r", ch);
    return;
}

void do_auto(CHAR_DATA *ch, char *argument) {

    do_config(ch, "");
    return;
}

void do_afk(CHAR_DATA *ch, char *argument) {
    if (IS_NPC(ch))
        return;

    if (IS_SET(ch->act, PLR_AFK)) {
        REMOVE_BIT(ch->act, PLR_AFK);
        send_to_char("[36m你回到了傳說中。[m\n\r", ch);
        act("[36m$n回到了傳說中。[m", ch, NULL, ch, TO_ROOM);
    } else {
        SET_BIT(ch->act, PLR_AFK);
        send_to_char("[36m你暫時離開傳說。[m\n\r", ch);
        act("[36m$n暫時離開傳說。[m", ch, NULL, ch, TO_ROOM);
    }

    return;
}

/* scan skill added by Amenda 00/12/6 */
void do_scan(CHAR_DATA *ch, char *argument) {
    EXIT_DATA *pexit;
    int i;

    if (!IS_NPC(ch) &&
        (ch->race != race_lookup("Hobbit")) // hobbit scan 不會失敗 2022/02/01
        && number_percent() > get_skill_percent(ch, gsn_scan)) {
        send_to_char("你失敗了!\n\r", ch);
        return;
    }
    // affect stone form 修正 2022/12/04
    if (is_affected(ch, gsn_stone_form)) {
        send_to_char("你在石化狀態下不能移動.\n\r", ch);
        return;
    }

    if (IS_AFFECTED(ch, AFF_HOLD)) {
        // snare 敘述修改 2022/12/04
        if (is_affected(ch, gsn_snare)) {
            send_to_char(
                "[33m你被困在陷阱裡了！你不能移動！[m\n\r",
                ch);
        } else {
            send_to_char(
                "[33m你的身體動彈不得！你不能移動！[m\n\r",
                ch);
        }
        return;
    }

    if (IS_AFFECTED(ch, AFF_PARALYZED)) {
        send_to_char("你因麻痺而動彈不得！\n\r", ch);
        return;
    }

    /*  移除目前房間黑暗則無法使用的設定 2022/02/01
    if ( room_is_dark( ch->in_room ) || !check_blind( ch ) )
    {
            send_to_char( "四周一片漆黑，你什麼都看不到!\n\r", ch );
            return;
    }
    */
    for (i = 0; i < 6; i++) {
        if ((pexit = ch->in_room->exit[i]) &&
            !IS_SET(pexit->exit_info, EX_CLOSED)) {
            scan_print(ch, pexit, i, 1);
            if (pexit->to_room && (pexit = pexit->to_room->exit[i]) &&
                !IS_SET(pexit->exit_info, EX_CLOSED)) {
                scan_print(ch, pexit, i, 2);
                if (pexit->to_room && (pexit = pexit->to_room->exit[i]) &&
                    !IS_SET(pexit->exit_info, EX_CLOSED))
                    scan_print(ch, pexit, i, 3);
            }
        }
    }
}

void scan_print(CHAR_DATA *ch, EXIT_DATA *pexit, int door, int level) {
    CHAR_DATA *pch;
    char buf[MAX_STRING_LENGTH * 2];

    if (room_is_dark(pexit->to_room))
        return;

    for (pch = pexit->to_room->people; pch; pch = pch->next_in_room) {
        if (!IS_NPC(ch) && !IS_IMMORTAL(ch) &&
            (
                // number_percent() > UMIN( ch->pcdata->learned[gsn_scan], 95)
                number_percent() > UMIN(get_skill_percent(ch, gsn_scan), 95) ||
                !can_see(ch, pch)))
            continue;

        strcpy(buf, "$N 在你的 ");
        switch (door) {
        case 0:
            strcat(buf, "北邊");
            break;
        case 1:
            strcat(buf, "東邊");
            break;
        case 2:
            strcat(buf, "南邊");
            break;
        case 3:
            strcat(buf, "西邊");
            break;
        case 4:
            strcat(buf, "上面");
            break;
        case 5:
            strcat(buf, "下面");
            break;
        }
        switch (level) {
        case 1:
            break;
        case 2:
            strcat(buf, " 不遠");
            break;
        case 3:
            strcat(buf, " 較遠處");
            break;
        }
        act(buf, ch, NULL, pch, TO_CHAR);
    }
}

/* scout skill added by Amenda 2002/6/28 */
void do_scout(CHAR_DATA *ch, char *argument) {
    int i;
    int sklv = get_skill_level(ch, skill_lookup("scout"));
    // int nMoveCR = sklv * (sklv + 1) * 5;
    int nMoveCR = 0;
    ROOM_INDEX_DATA *in_room, *to_room;
    EXIT_DATA *pexit;

    if (ch->fighting || IS_NPC(ch) ||
        number_percent() > get_skill_percent(ch, skill_lookup("scout"))) {
        send_to_char("你失敗了!\n\r", ch);
        return;
    }
    // affect stone form 修正 2022/12/04
    if (is_affected(ch, gsn_stone_form)) {
        send_to_char("你在石化狀態下不能移動.\n\r", ch);
        return;
    }

    if (IS_AFFECTED(ch, AFF_HOLD)) {
        // snare 敘述修改 2022/12/04
        if (is_affected(ch, gsn_snare)) {
            send_to_char(
                "[33m你被困在陷阱裡了！你不能移動！[m\n\r",
                ch);
        } else {
            send_to_char(
                "[33m你的身體動彈不得！你不能移動！[m\n\r",
                ch);
        }
        return;
    }

    if (IS_AFFECTED(ch, AFF_PARALYZED)) {
        send_to_char("你因麻痺而動彈不得!\n\r", ch);
        return;
    }

    if (auction_list) {
        AUC_DATA *auction;
        for (auction = auction_list; auction; auction = auction->next) {
            if (ch == auction->owner || ch == auction->buyer) {
                send_to_char("你正在進行拍賣，不能離開這裡。\n\r", ch);
                return;
            }
        }
    }

    /*  移除目前房間黑暗則無法使用的設定 2022/02/01
    if ( room_is_dark( ch->in_room ) || !check_blind( ch ) )
    {
            send_to_char( "四周一片漆黑，你什麼都看不到!\n\r", ch );
            return;
    }
    */

    // hobbit 有額外 sklv 加成 , 使 sklv 最高可為 7  2022/02/01
    if (ch->race == race_lookup("Hobbit"))
        sklv += 1;

    // mv 最低消耗 1  2022/02/01
    nMoveCR = UMIN(299, sklv * (sklv + 1) * 5);

    // 修正不渴種族不會判定渴值 2021/10/02
    if (race_table[ch->race].thirst_mod >= 0 &&
        ch->pcdata->condition[COND_THIRST] <= 15 && sklv < 4) {
        send_to_char("你太渴了。\n\r", ch);
        return;
    }
    // 修正不饑餓種族不會判定饑餓值 2021/10/02
    if (race_table[ch->race].hunger_mod >= 0 &&
        ch->pcdata->condition[COND_FULL] <= 15 && sklv < 5) {
        send_to_char("你太餓了。\n\r", ch);
        return;
    }
    if (ch->move < 300 - nMoveCR) {
        send_to_char("你沒有足夠的體力。\n\r", ch);
        return;
    } else {
        // 修正不饑餓種族不會扣除饑餓值  2021/10/02
        if (sklv < 4 && race_table[ch->race].thirst_mod >= 0)
            ch->pcdata->condition[COND_THIRST] -= 12 - sklv * 2;
        // 飽足狀態下不會減少饑餓值，且不饑餓種族不會扣除饑餓值 2021/10/02
        if (!is_affected(ch, gsn_satiety)) {
            if (sklv < 5 && race_table[ch->race].hunger_mod >= 0)
                ch->pcdata->condition[COND_FULL] -= 12 - sklv * 2;
        }
        ch->move -= 300 - nMoveCR;
    }

    if (!str_prefix(argument, "north"))
        i = 0;
    else if (!str_prefix(argument, "east"))
        i = 1;
    else if (!str_prefix(argument, "south"))
        i = 2;
    else if (!str_prefix(argument, "west"))
        i = 3;
    else if (!str_prefix(argument, "up"))
        i = 4;
    else if (!str_prefix(argument, "down"))
        i = 5;
    else {
        send_to_char("你到底有沒有方向感啊？\n\r", ch);
        return;
    }

    in_room = ch->in_room;

    // 第一次 scout , 修改敘述加入方向 2022/12/22
    // send_to_char( ""HIB"你試著偵察靠近你的地方："NOR"\n\r", ch );
    if (i == 0)
        send_to_char("" HIB "你試著往" HIC "北" HIB "邊進行偵察：" NOR "\n\r",
                     ch);
    if (i == 1)
        send_to_char("" HIB "你試著往" HIC "東" HIB "邊進行偵察：" NOR "\n\r",
                     ch);
    if (i == 2)
        send_to_char("" HIB "你試著往" HIC "南" HIB "邊進行偵察：" NOR "\n\r",
                     ch);
    if (i == 3)
        send_to_char("" HIB "你試著往" HIC "西" HIB "邊進行偵察：" NOR "\n\r",
                     ch);
    if (i == 4)
        send_to_char("" HIB "你試著往" HIC "上" HIB "方進行偵察：" NOR "\n\r",
                     ch);
    if (i == 5)
        send_to_char("" HIB "你試著往" HIC "下" HIB "方進行偵察：" NOR "\n\r",
                     ch);
    if (!(pexit = ch->in_room->exit[i]) || !(to_room = pexit->to_room)) {
        send_to_char("[33m哎呀，此路不通。[m\n\r", ch);
        return;
    } else {
        if (can_scout_check(ch, pexit, to_room, sklv))
            scout(ch, pexit, to_room, sklv);
        else
            return;
    }

    // 所處房間有 ROOM_SCOUT_ONE 的 room flag 則只能查一格
    if (IS_SET(in_room->room_flags, ROOM_SCOUT_ONE)) {
        send_to_char("" HIB "你試著偵察不遠處，但你失敗了！" NOR "\n\r", ch);
    } else {
        // 第二次 scout
        if (i == 0)
            send_to_char(
                "" HIB "你試著偵察" HIC "北" HIB "邊不遠處：" NOR "\n\r", ch);
        if (i == 1)
            send_to_char(
                "" HIB "你試著偵察" HIC "東" HIB "邊不遠處：" NOR "\n\r", ch);
        if (i == 2)
            send_to_char(
                "" HIB "你試著偵察" HIC "南" HIB "邊不遠處：" NOR "\n\r", ch);
        if (i == 3)
            send_to_char(
                "" HIB "你試著偵察" HIC "西" HIB "邊不遠處：" NOR "\n\r", ch);
        if (i == 4)
            send_to_char(
                "" HIB "你試著偵察" HIC "上" HIB "方不遠處：" NOR "\n\r", ch);
        if (i == 5)
            send_to_char(
                "" HIB "你試著偵察" HIC "下" HIB "方不遠處：" NOR "\n\r", ch);
        // send_to_char( ""HIB"你試著偵察不遠處："NOR"\n\r", ch );
        if (!(pexit = to_room->exit[i]) || !(to_room = pexit->to_room)) {
            send_to_char("[33m哎呀，此路不通。[m\n\r", ch);
        } else {
            if (can_scout_check(ch, pexit, to_room, sklv)) {
                scout(ch, pexit, to_room, sklv);
                // 第三次 scout
                if (sklv > 5) {
                    if (i == 0)
                        send_to_char("" HIB "你試著偵察" HIC "北" HIB
                                     "邊較遠處：" NOR "\n\r",
                                     ch);
                    if (i == 1)
                        send_to_char("" HIB "你試著偵察" HIC "東" HIB
                                     "邊較遠處：" NOR "\n\r",
                                     ch);
                    if (i == 2)
                        send_to_char("" HIB "你試著偵察" HIC "南" HIB
                                     "邊較遠處：" NOR "\n\r",
                                     ch);
                    if (i == 3)
                        send_to_char("" HIB "你試著偵察" HIC "西" HIB
                                     "邊較遠處：" NOR "\n\r",
                                     ch);
                    if (i == 4)
                        send_to_char("" HIB "你試著偵察" HIC "上" HIB
                                     "方較遠處：" NOR "\n\r",
                                     ch);
                    if (i == 5)
                        send_to_char("" HIB "你試著偵察" HIC "下" HIB
                                     "方較遠處：" NOR "\n\r",
                                     ch);
                    if (!(pexit = to_room->exit[i]) ||
                        !(to_room = pexit->to_room)) {
                        send_to_char("[33m哎呀，此路不通。[m\n\r",
                                     ch);
                    } else {
                        if (can_scout_check(ch, pexit, to_room, sklv))
                            scout(ch, pexit, to_room, sklv);
                    }
                }
            }
        }
    }

    char_from_room(ch);
    char_to_room(ch, in_room);
    act("你發現 $n 風塵僕僕，滿身大汗地從陰暗角落出現。", ch, NULL, NULL,
        TO_ROOM);
}

// 判定 scout 的房間是否能進入 2022/12/22
bool can_scout_check(CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room,
                     int sklv) {

    if (IS_SET(pexit->exit_info, EX_CLOSED)) {
        if (!IS_AFFECTED(ch, AFF_PASS_DOOR) &&
            !IS_SET(race_table[ch->race].race_abilities, RACE_PASSDOOR)) {
            act("[33m$d是關著的。[m", ch, NULL, pexit->keyword, TO_CHAR);
            return FALSE;
        }
        if (IS_SET(pexit->exit_info, EX_PASSPROOF)) {
            act("[33m你穿不過$d。好痛！[m", ch, NULL, pexit->keyword,
                TO_CHAR);
            return FALSE;
        }
    }
    if (to_room->sector_type == SECT_AIR) {
        if (!is_flying(ch)) {
            send_to_char("[33m你飛不起來。[m\n\r", ch);
            return FALSE;
        }
    }

    if (to_room->sector_type != SECT_WATER_NOSWIM &&
        to_room->sector_type != SECT_UNDERWATER &&
        strcmp(race_table[ch->race].name, "God") &&
        strcmp(race_table[ch->race].name, "Bear") &&
        strcmp(race_table[ch->race].name, "Lizardman") &&
        IS_SET(race_table[ch->race].race_abilities, RACE_SWIM)) {
        send_to_char("[33m你在原地繞來繞去卻游不過去！[m\n\r",
                     ch);
        return FALSE;
    }

    if (to_room->sector_type == SECT_WATER_NOSWIM) {
        OBJ_DATA *obj;
        bool found; // Look for a boat.

        found = FALSE;

        // Suggestion for flying above water by Sludge
        if (is_flying(ch) ||
            IS_SET(race_table[ch->race].race_abilities, RACE_WATERWALK) ||
            IS_SET(race_table[ch->race].race_abilities, RACE_SWIM))
            found = TRUE;

        for (obj = ch->carrying; obj; obj = obj->next_content) {
            if (obj->item_type == ITEM_BOAT) {
                found = TRUE;
                break;
            }
        }
        if (!found) {
            send_to_char("[33m你得要有一艘船才過得去。[m\n\r",
                         ch);
            return FALSE;
        }
    }

    // door size 判定 2022/12/22
    if (sklv < 2) {
        // if ( pexit->exit_size > 0 && ( race_table[ch->race].size >
        // pexit->exit_size ) )
        if (pexit->exit_size > 0 &&
            (get_curr_size(ch) >
             pexit->exit_size)) // 替換體型 size 公式 2022/12/23
        {
            send_to_char("[33m你體型太大，進入不了通道。[m\n\r",
                         ch);
            return FALSE;
        }
    }

    // room size 人數判定 2022/12/22
    if (sklv < 4) {
        if (room_is_private(ch, to_room)) {
            send_to_char("[33m那裡的空間大小，容納不下你的體積。["
                         "m\n\r",
                         ch);
            return FALSE;
        }
    }

    return TRUE;
}
void scout(CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room,
           int sklv) {
    int room_flags = 0; // 2022/12/22

    // remove at 2022/12/22
    // 此段判定僅能處理被 scout 的那格房間, 當第一格房間無法進入時,
    // 無法阻擋進入第二格房間 故移至上方作成 bool can_scout_check
    // 來獨立判定是否能對房間作 scout
    /*if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
    {
            if ( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && !IS_SET( race_table[
    ch->race ].race_abilities, RACE_PASSDOOR ) )
            {
                    act( "[33m$d是關著的。[m",ch, NULL, pexit->keyword,
    TO_CHAR ); return;
            }
            if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
            {
                    act( "[33m你穿不過$d。好痛！[m",ch, NULL, pexit->keyword,
    TO_CHAR ); return;
            }
    }
    if ( to_room->sector_type == SECT_AIR )
    {
            if( !is_flying(ch))
            {
                    send_to_char( "[33m你飛不起來。[m\n\r", ch );
                    return;
            }
    }

    if (   to_room->sector_type != SECT_WATER_NOSWIM
            && to_room->sector_type != SECT_UNDERWATER
            && strcmp( race_table[ ch->race ].name, "God" )
            && strcmp( race_table[ ch->race ].name, "Bear" )
            && strcmp( race_table[ ch->race ].name, "Lizardman" )
            && IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ))
    {
            send_to_char( "[33m你在原地繞來繞去卻游不過去！[m\n\r", ch
    ); return;
    }

    if ( to_room->sector_type == SECT_WATER_NOSWIM )
    {
            OBJ_DATA *obj;
            bool found; // Look for a boat.

            found = FALSE;

            // Suggestion for flying above water by Sludge
            if(    is_flying( ch )
                    || IS_SET( race_table[ ch->race ].race_abilities,
    RACE_WATERWALK )
                    || IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM
    ) ) found = TRUE;

            for ( obj = ch->carrying; obj; obj = obj->next_content )
            {
                    if ( obj->item_type == ITEM_BOAT )
                    {
                            found = TRUE;
                            break;
                    }
            }
            if ( !found )
            {
                    send_to_char( "[33m你得要有一艘船才過得去。[m\n\r", ch
    ); return;
            }
    } */

    char_from_room(ch);
    char_to_room(ch, to_room);
    do_look(ch, "");
    // 加入 sklv room flag 敘述 2022/12/22
    send_to_char("這裡", ch);
    if (sklv >= 3) {
        if (IS_SET(to_room->room_flags, ROOM_NO_RECALL)) {
            if (IS_SET(to_room->room_flags, ROOM_PK))
                send_to_char("是受阻咒的", ch);
            else
                send_to_char("受到阻咒", ch);
            room_flags += 1;
        }

        if (room_flags < 0)
            send_to_char("是", ch);

        if (IS_SET(to_room->room_flags, ROOM_PK)) {
            send_to_char("鬥技場", ch);
            room_flags += 1;
        }
    }
    if (sklv >= 1) {
        if (room_flags > 0)
            send_to_char("，", ch);

        if (IS_SET(to_room->room_flags, ROOM_INDOORS)) {
            send_to_char("位於室內", ch);
            room_flags += 1;
        }
        if (IS_SET(to_room->room_flags, ROOM_DARK)) {
            send_to_char("四周相當黑暗", ch);
            room_flags += 1;
        }

        if (IS_SET(to_room->room_flags, ROOM_SAFE)) {
            if (room_flags > 0)
                send_to_char("，不過看起來很安全", ch);
            else
                send_to_char("看起來很安全", ch);
            room_flags += 1;
        }
        if (sklv >= 4) {
            if (IS_SET(to_room->room_flags, ROOM_CAMP)) {
                if (room_flags > 0)
                    send_to_char("，並且地面流竄著魔力", ch);
                else
                    send_to_char("地面流竄著魔力", ch);
                room_flags += 1;
            }
        }
    }
    if (room_flags > 0) {
        send_to_char("。\n\r", ch);
    } else {
        send_to_char("沒什麼特別的。\n\r", ch);
    }

    // 加入 sklv 判定 2022/12/22
    if (sklv > 5) {
        if (IS_SET(to_room->room_flags, ROOM_CAMP))
            if (to_room->roomprogs)
                send_to_char(
                    "此外，你敏銳的觀察力發現這裡似乎有什麼不對勁。\n\n\r", ch);
    }
}

int skill_exp_level(int exp) { return 0; }

char *skill_exp_desc(int exp) {
    switch (exp) {
    case 6:
        return HIW "天人合一" NOR;
        break;
    case 5:
        return HIC "深不可測" NOR;
        break;
    case 4:
        return HIY "超凡入聖" NOR;
        break;
    case 3:
        return HIG "神乎其技" NOR;
        break;
    case 2:
        return HIR "得心應手" NOR;
        break;
    case 1:
        return HIP "略知一二" NOR;
        break;
    case 0:
        return HIU "新學乍練" NOR;
        break;
    default:
        return "BUG CHAR";
        break;
    }
}

void do_skadv(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[BUF_STRING_LENGTH];
    int sn = 0;
    int cost;
    CHAR_DATA *mob;

    if (IS_NPC(ch))
        return;

    if (ch->class < WIZARD) {
        send_to_char("你必須轉職後才能提升技能等級。\n\r", ch);
        return;
    }
    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg2);

    if (arg[0] == '\0') {
        send_to_char("你要提升什麼技能？\n\r", ch);
        return;
    }

    sn = skill_lookup(arg);

    if (sn < 0) {
        send_to_char("沒有這個技能。\n\r", ch);
        return;
    }

    if (get_skill_percent(ch, sn) < 1) {
        send_to_char("你並不會這項技能。\n\r", ch);
        return;
    }

    if (sn == gsn_battle_array)
        return;

    if (sn == gsn_inborn_ability) {
        AFFECT_DATA *paf = NULL;
        for (paf = ch->affected; paf; paf = paf->next) {
            if (paf->deleted)
                continue;
            if (paf->type == gsn_polymorph) {
                send_to_char("你不能在變族的情況提升你的天賦。\n\r", ch);
                return;
            }
        }
    }

    // 針對特殊 room 的情況, 之後可以加入其他 room_flag 作 room skadv 的判定
    if (ch->in_room->vnum == 11000)
        ;
    else {
        if (arg2[0] == '\0') {
            for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
                if (mob->deleted)
                    continue;
                if (IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE))
                    break;
            }
        } else {
            for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
                if (mob->deleted)
                    continue;
                if (IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) &&
                    is_pfxname(arg2, mob->name))
                    break;
            }
        }

        if (!mob || !IS_SET(mob->act, ACT_PRACTICE)) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        if (!mob->psk) {
            send_to_char("[33m你找不到導師。[m\n\r", ch);
            return;
        }

        switch (is_skname(arg, mob->psk)) {
        case -1:
            act("$n 說：對不起，我並不會那項技巧。", mob, NULL, ch, TO_VICT);
            return;
        case 1:
            if (!is_qmark(ch, skill_table[sn].name)) {
                act("$n 說：對不起，你還不夠資格學習這項技巧。", mob, NULL, ch,
                    TO_VICT);
                return;
            }
            break;
        default:
            break;
        }
    }

    /* 將 sn 判定移到最前方, 是 is_skname 能夠產生作用 2023/02/20

            if( arg[0] == '\0' ) {
                    send_to_char("你要提升什麼技能？\n\r", ch);
                    return;
            }
            sn = skill_lookup(arg);
            if( sn < 0 ) {
                    send_to_char("沒有這個技能。\n\r", ch);
                    return;
            }
            if(get_skill_percent(ch,sn) < 1 ) {
                    send_to_char("你並不會這項技能。\n\r", ch);
                    return;
            }

            if ( sn == gsn_battle_array )
                    return;

            if( sn == gsn_inborn_ability)
            {
                    AFFECT_DATA *paf = NULL;
                    for ( paf = ch->affected; paf; paf = paf->next )
                    {
                            if ( paf->deleted )
                                    continue;
                            if ( paf->type == gsn_polymorph){
                                    send_to_char("你不能在變族的情況提升你的天賦。\n\r",
       ch); return;
                            }
                    }
            }
    */

    if ((cost = skill_difficulty(ch, sn)) < 0) {
        send_to_char("你在這項技能上已經無法再進步了。\n\r", ch);
        return;
    }
    if (get_eff_skill_exp(ch) < cost) {
        send_to_char("你的可用技能經驗值不夠了。\n\r", ch);
        return;
    }

    sprintf(buf, "你用去了 %d 點技能經驗值。\n\r", skill_difficulty(ch, sn));
    send_to_char(buf, ch);

    act("[36m你提升了$t($T)。[m", ch, skill_table[sn].cname,
        skill_table[sn].name, TO_CHAR);
    act("[36m$n提升了$t($T)。[m", ch, skill_table[sn].cname,
        skill_table[sn].name, TO_ROOM);

    use_skill_exp(ch, skill_difficulty(ch, sn));
    gain_skill_level(ch, sn, 1);

    if (IsUnderFistFight(ch)) {
        SET_BIT(ch->act, PLR_FIST_FIGHT);
    } else {
        REMOVE_BIT(ch->act, PLR_FIST_FIGHT);
    }
}

void do_skdemote(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];
    // char buf[BUF_STRING_LENGTH];
    int sn;
    // int cost;

    if (IS_NPC(ch))
        return;
    if (ch->class < WIZARD) {
        send_to_char("你必須轉職後才能降低技能等級。\n\r", ch);
        return;
    }
    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("你要降低什麼技能？\n\r", ch);
        return;
    }
    sn = skill_lookup(arg);
    if (sn < 0) {
        send_to_char("沒有這個技能。\n\r", ch);
        return;
    }
    if (get_skill_percent(ch, sn) < 1) {
        send_to_char("你並不會這項技能。\n\r", ch);
        return;
    }

    if (get_skill_level(ch, sn) < 1) {
        send_to_char("你在這項技能上已經無法再降低了。\n\r", ch);
        return;
    }

    if (sn == gsn_inborn_ability) {
        AFFECT_DATA *paf = NULL;
        for (paf = ch->affected; paf; paf = paf->next) {
            if (paf->deleted)
                continue;
            if (paf->type == gsn_polymorph) {
                send_to_char("你不能在變族的情況降低你的天賦。\n\r", ch);
                return;
            }
        }
    }

    act("[36m你降低了$t($T)。[m", ch, skill_table[sn].cname,
        skill_table[sn].name, TO_CHAR);
    act("[36m$n降低了$t($T)。[m", ch, skill_table[sn].cname,
        skill_table[sn].name, TO_ROOM);

    gain_skill_level(ch, sn, -1);
    regain_skill_exp(ch, skill_difficulty(ch, sn));

    if (IsUnderFistFight(ch)) {
        SET_BIT(ch->act, PLR_FIST_FIGHT);
    } else {
        REMOVE_BIT(ch->act, PLR_FIST_FIGHT);
    }
}
