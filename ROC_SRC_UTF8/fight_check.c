//*******************************************************************
//*  PROGRAM ID  : fight_check.c                                    *
//*  LANGUAGE    : c                                                *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                      *
//*  PURPOSE     :         新增EXP_PER_LEVEL ppl_class變數          *
//*******************************************************************
#include "ansi.h"
#include "merc.h"
#include <stdio.h>

int  check_item_magic_flag(OBJ_DATA *wield, int vrace, int dam);
bool check_evade(CHAR_DATA *ch, CHAR_DATA *victim);
bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);
// bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff );
bool check_parry(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn); // 2022/12/04
bool check_block(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff);
bool check_tactics(CHAR_DATA *ch, CHAR_DATA *victim);
bool frenzy_check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);
void check_killer(CHAR_DATA *ch, CHAR_DATA *victim);
bool is_wielding_poisoned(CHAR_DATA *ch, int wpn);
bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim);
void check_corpse_count(CHAR_DATA *ch, CHAR_DATA *victim);

bool licensed(CHAR_DATA *ch);
bool registered(CHAR_DATA *ch, CHAR_DATA *victim);

void check_wimpout(CHAR_DATA *ch, CHAR_DATA *victim);
bool check_reflexion(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam, int dexdiff);
int  check_damage_adjust(CHAR_DATA *ch, int dam);
int  check_spellfury(CHAR_DATA *ch, CHAR_DATA *victim, int dam);
int  check_dirty_raid(CHAR_DATA *ch, CHAR_DATA *victim, int dam);
bool check_can_do_hit(CHAR_DATA *ch, CHAR_DATA *victim);

CHAR_DATA *check_mob_ai(CHAR_DATA *ch);
CHAR_DATA *mob_ai_lv3(CHAR_DATA *ch);
CHAR_DATA *mob_ai_lv2(CHAR_DATA *ch);
CHAR_DATA *mob_ai_lv1(CHAR_DATA *ch);

// Not used -- head --
void pc_breathe(CHAR_DATA *ch);
void pc_screech(CHAR_DATA *ch);
void pc_spit(CHAR_DATA *ch);
bool check_race_special(CHAR_DATA *ch);
// -- tail --

extern int artifact(CHAR_DATA *ch);
////extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
extern int mone_hit(CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode); // Modified by Razgriz 20051014
extern int GetBonusedValue_LanceMaster(CHAR_DATA *ch, int nPerc);

// check_item_magic_flag: Combat logic: check_item_magic_flag - Applies combat calculations, checks, or damage formulas.
// check_item_magic_flag: 戰鬥邏輯：check_item_magic_flag - 進行戰鬥數值判定、安全檢查或傷害公式運算。
int check_item_magic_flag(OBJ_DATA *wield, int vrace, int dam) {
    if (wield && IS_SET(wield->extra_flags, ITEM_MAGIC))
        if (number_bits(2) == 1)
            if (!str_cmp(race_table[vrace].name, "Vampire") || !str_cmp(race_table[vrace].name, "Undead") || !str_cmp(race_table[vrace].name, "Mindflayer") || !str_cmp(race_table[vrace].name, "Githyanki") || !str_cmp(race_table[vrace].name, "Phantom") || !str_cmp(race_table[vrace].name, "Ghost") || !str_cmp(race_table[vrace].name, "Mist"))
                return (dam / 5);
    return 0;
}

// check_dodge: Combat logic: check_dodge - Applies combat calculations, checks, or damage formulas.
// check_dodge: 戰鬥邏輯：check_dodge - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff) {
    int  chance;
    int  ppl_dex;
    int  mod = 100;
    int  reposte;         // 20191213 dodge reposte
    int  dual_reposte;    // 2021/11/16 dual_reposte
    bool fReposte = TRUE; // 20191213 dodge reposte

    if (!IS_AWAKE(victim))
        return FALSE;

    if (IS_NPC(victim)) {
        fReposte = FALSE; // 191213
        /* Tuan was here.  :) */
        if (IS_SET(victim->special, EXPERT))
            chance = UMIN(40, victim->level / 3 + UMAX(0, victim->level - ((victim->class == 1) ? 30 : 60)));
        else
            chance = UMIN(30, victim->level / 4 + UMAX(0, victim->level - ((victim->class == 1) ? 20 : 40)));
        // special bonus for war and thi class
        if (!IS_NPC(ch)) {
            if (ch->pcdata->prime_attr == APPLY_STR || ch->pcdata->snd_attr == APPLY_STR)
                chance -= 10;
            if (ch->pcdata->prime_attr == APPLY_DEX || ch->pcdata->snd_attr == APPLY_DEX)
                chance -= 20;
        }
    } else if (victim->pcdata->learned[gsn_fool] + victim->pcdata->learned[gsn_dodge] == 0)
        return FALSE;
    else
        chance = get_skill_percent(victim, gsn_fool) / 4;

    if (!IS_NPC(victim)) {
        if (victim->pcdata->autocombat == AUTOC_DODGE) {
            mod += get_skill_percent(victim, gsn_autododge) / 2 + get_skill_level(victim, gsn_autododge) * 3;
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_PARRY) {
            mod += get_skill_percent(victim, gsn_autoparry) / 3 + get_skill_level(victim, gsn_autoparry);
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_STOP || victim->pcdata->autocombat == AUTOC_CONCENTRATION) { // 191213 加入autoc cast判定
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_CRITICAL) {
            mod += get_skill_percent(victim, gsn_deadly_strike) / 5 + get_skill_level(victim, gsn_deadly_strike);
        }
    }

    chance += get_skill_level(victim, gsn_fool) + get_skill_level(victim, gsn_fool) / 2;

    // TrickyPacing dodge chance + 32%
    chance += get_skill_percent(victim, gsn_tricky_pacing) / 30 + get_skill_level(victim, gsn_tricky_pacing) * 5;

    // stone form 狀況下chance 減半 2022/12/05
    if (is_affected(victim, gsn_stone_form)) {
        fReposte = FALSE;
        chance /= 2;
    }

    if (!IS_NPC(victim) && (number_percent() * 100 / mod <= chance - UMIN(15, (leveldiff * 2) + (dexdiff * 4)))) {
        act((is_chinese(ch, victim)) ? "$N 用迅捷的身法地閃過你的攻擊." : "$N fools you and dodges your attack.",
            ch, NULL, victim, TO_CHAR);
        act((is_chinese(victim, ch)) ? "你用迅捷的身法躲過 $n 的攻擊." : "You fool $n and dodges $n's attack.",
            ch, NULL, victim, TO_VICT);

        if (get_skill_level(victim, gsn_fool) > 4) {
            if (number_percent() < get_skill_level(victim, gsn_fool) * 3)
                trip(victim, ch);
        }
        return TRUE;
    }

    if (!IS_NPC(victim)) {
        ppl_dex = get_curr_dex(victim);
        chance  = get_skill_percent(victim, gsn_dodge) / 11;                                         // 9
        chance += (get_skill_level(victim, gsn_dodge) * get_skill_level(victim, gsn_dodge) * 2) / 3; // 24
        chance += ppl_dex - 15;
        chance = UMIN(chance - leveldiff / 2, 40 + get_skill_level(victim, gsn_dodge) * 2);
        chance -= UMIN(0, (GET_DODGE(victim) * get_skill_level(victim, gsn_dodge) * get_skill_level(victim, gsn_dodge)) / 6000); // lv 6 dc -1000 6
    }

    if (number_percent() * 100 / mod >= chance)
        return FALSE;

    // 191213  dodge reposte 判定
    if (fReposte) {
        reposte = get_skill_percent(victim, gsn_reposte) / 5;
        reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4);
        dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
        dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 2;

        if (reposte > number_percent()) {
            act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack." NOR : HIY "$N 巧妙利用你攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_CHAR);
            act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack." NOR : HIY "你巧妙利用 $n 攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_VICT);
            mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE);
            /*
                    修改為 dual 的情況下作出副手武器反擊 2021/11/16

                    if( ch->class != NINJA && (victim->class == NINJA && !IS_NPC(victim))
                            && number_percent() < get_curr_dex(victim) / 2 + reposte / 2 )
                    {
                            act( (!is_chinese(ch, victim)) ?
                                    HIY"$N repostes your attack again."NOR :
                                    HIY"$N 再次利用你攻擊時的破綻反擊."NOR, ch, NULL, victim, TO_CHAR );
                            act( (!is_chinese(victim, ch)) ?
                                    HIY"You repostes $n's attack again." NOR:
                                    HIY"你再次利用 $n 攻擊時的破綻反擊."NOR,  ch, NULL, victim, TO_VICT );
                            mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
                    }
            */
            if (get_eq_char(victim, WEAR_WIELD_2) && (number_percent() < dual_reposte)) {
                act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack with secondary weapon." NOR : HIY "$N 在反擊中以副手武器對你進行追擊." NOR, ch, NULL, victim, TO_CHAR);
                act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack with secondary weapon." NOR : HIY "你在反擊中以副手武器對 $n 進行追擊." NOR, ch, NULL, victim, TO_VICT);
                mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE);
            }
            return TRUE;
        }
    }

    act((!is_chinese(ch, victim)) ? "$N dodges your attack." : "$N 靈巧地閃避你的攻擊.", ch, NULL, victim, TO_CHAR);
    act((!is_chinese(victim, ch)) ? "You dodges $n's attack." : "你靈巧地閃避$n的攻擊.", ch, NULL, victim, TO_VICT);
    return TRUE;
}

// check_evade: Combat logic: check_evade - Applies combat calculations, checks, or damage formulas.
// check_evade: 戰鬥邏輯：check_evade - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_evade(CHAR_DATA *ch, CHAR_DATA *victim) {
    int chance;
    int mod = 100;

    if (!IS_AWAKE(victim))
        return FALSE;

    if (IS_NPC(victim)) {
        /* Tuan was here.  :) */
        // if( IS_SET(victim->special, EXPERT))
        //   chance        = victim->level / 8 ;
        // else chance = 0;
        chance = 0;
        return FALSE;
    } else {
        if (number_percent() > get_skill_percent(victim, gsn_evade))
            return FALSE;
        if (victim->move < 70)
            return FALSE;

        chance = -1 * (GET_DODGE(victim) * (10 + get_skill_level(victim, gsn_evade) * get_skill_level(victim, gsn_evade))) / 1000;

        chance += get_skill_level(victim, gsn_evade);

        if (victim->pcdata->autocombat == AUTOC_DODGE) {
            chance += get_skill_percent(victim, gsn_autododge) / 33 + get_skill_level(victim, gsn_autododge) * 3;
        } else if (victim->pcdata->autocombat == AUTOC_CRITICAL) {
            chance += get_skill_percent(victim, gsn_deadly_strike) / 33 + get_skill_level(victim, gsn_deadly_strike);
        }
    }

    if (number_percent() * 100 / mod >= chance)
        return FALSE;

    act((!is_chinese(ch, victim)) ? "$N evades your spell." : "$N 靈巧地迴避你的咒文.", ch, NULL, victim, TO_CHAR);
    act((!is_chinese(victim, ch)) ? "You evades $n's spell." : "你靈巧地迴避$n的咒文.", ch, NULL, victim, TO_VICT);
    victim->move -= 70;
    return TRUE;
}
/*
 * Check for parry.
 */
// check_parry 加入 dt 值及 wpn , ch 攻擊時使用的武器 2022/12/04
// bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff )
// check_parry: Combat logic: check_parry - Applies combat calculations, checks, or damage formulas.
// check_parry: 戰鬥邏輯：check_parry - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_parry(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn) {
    // int chance;
    int  mob_chance = 0;
    int  chance     = 0;
    int  chance2    = 0; // 2022/12/03
    int  reposte;
    int  dual_reposte; // 2021/11/16 dual_reposte
    bool fReposte  = TRUE;
    bool isTwohand = FALSE; // 判定 wp 是否為 twohand 2022/12/06
    // int mod = 100;
    int mod = 0;
    // add at 2022/12/03
    bool      isWp2Parry  = FALSE;
    bool      isFistParry = FALSE;
    OBJ_DATA *wp          = NULL;
    OBJ_DATA *wp2         = NULL;
    OBJ_DATA *ch_wp       = NULL; // 2022/12/28
    OBJ_DATA *ch_wp2      = NULL; // 2022/12/28
    int       weight_diff = 0;
    int       min_str;
    int       str_diff;

    if (!IS_AWAKE(victim))
        return FALSE;

    // 阻檔 射箭 及 槍械 判定 parry 2022/12/28
    if (dt == 1014 || dt == 1016)
        return FALSE;

    if (!IS_NPC(victim)) {
        if (victim->pcdata->autocombat == AUTOC_STOP || victim->pcdata->autocombat == AUTOC_CONCENTRATION || victim->pcdata->autocombat == AUTOC_INSTANT || victim->pcdata->autocombat == AUTOC_DODGE) {
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_PARRY) {
            int nAutoParryPerc = get_skill_percent(victim, gsn_autoparry);
            int nAutoParrySklv = get_skill_level(victim, gsn_autoparry);
            if (victim->class == KNIGHT) {
                mod += 3 + (nAutoParrySklv * nAutoParrySklv) / 3; // lv6 15 , max 20
                weight_diff -= 5;
            } else {
                mod += 1 + (nAutoParrySklv * nAutoParrySklv) / 4; // lv6 9 , max 15
            }
            weight_diff -= nAutoParrySklv * 5;
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_CRITICAL) {
            mod += get_skill_percent(victim, gsn_deadly_strike) / 20;                                             // base 5
            mod += (get_skill_level(victim, gsn_deadly_strike) * get_skill_level(victim, gsn_deadly_strike)) / 4; // lv 6 9 , max 14
        }
    }

    // 將 victim get_eq_char 換成 wp  及 wp2  使 parry 能各別判定 2022/12/03
    if (get_eq_char(victim, WEAR_TWO_HAND)) {
        isTwohand = TRUE;
        wp        = get_eq_char(victim, WEAR_TWO_HAND);
    } else if (get_eq_char(victim, WEAR_WIELD))
        wp = get_eq_char(victim, WEAR_WIELD);

    if (get_eq_char(victim, WEAR_WIELD_2))
        wp2 = get_eq_char(victim, WEAR_WIELD_2);

    if (IS_NPC(victim)) {
        /*  wp 及 wp2 分開判定, 並對 mob 空手的情況下作 class 判定 2022/12/03
        if ( !get_eq_char( victim, WEAR_WIELD ) && !get_eq_char( victim, WEAR_TWO_HAND ) )
        {
                if ( !get_eq_char( victim, WEAR_WIELD_2 ) )
                        chance /= 3;
                else
                        chance = 3 * chance / 4;
        } */

        fReposte = FALSE;
        /* Tuan was here.  :) */
        if (IS_SET(victim->special, EXPERT)) { // npc 基本的 chance
            mob_chance = UMIN(20, (victim->class == 1) ? victim->level - 20 : victim->level - 35);
        } else {
            mob_chance = UMIN(10, (victim->class == 1) ? victim->level - 35 : victim->level - 40);
        }

        if (!wp && !wp2) {
            if (IS_SET(victim->class, 32)) { // 如果 npc 沒有 class bloody 則不會空手格擋
                isFistParry = TRUE;
                chance      = (mob_chance * 3) / 4;
            } else
                return FALSE;
        } else {
            if (wp)
                chance = mob_chance;

            if (wp2)
                chance2 = (mob_chance * 4) / 5;
        }

        // special bonus for war and thi class
        if (!IS_NPC(ch)) {
            if (ch->pcdata->prime_attr == APPLY_STR || ch->pcdata->snd_attr == APPLY_STR)
                leveldiff += 7;
            if (ch->pcdata->prime_attr == APPLY_DEX || ch->pcdata->snd_attr == APPLY_DEX)
                leveldiff += 12;
        }
        if (IS_SET(ch->special, EXPERT)) // 進階 npc
            leveldiff += 10;
    } else // pc 的 parry chance 計算
    {
        if (!wp && !wp2) // 無 wp 的情況下
        {
            // 空手跑 chance 視同以主要 wp parry
            if (get_skill_percent(victim, gsn_barehand) > 0) // max 49
            {
                isFistParry = TRUE;
                // parry 基本值
                chance = (get_skill_percent(victim, gsn_barehand) + get_skill_percent(victim, gsn_parry)) / 15; // 13
                chance += get_skill_level(victim, gsn_barehand) * 3;                                            // lv 6 18
                chance += (get_skill_level(victim, gsn_parry) * get_skill_level(victim, gsn_parry)) / 2;        // lv 6 18
            } else {
                return FALSE;
            }
        } else // 有 wp 的情況下
        {
            /*  將 parry 分為 wp 及 wp2 2022/12/03
            if ( !get_eq_char( victim, WEAR_WIELD ) && !get_eq_char( victim, WEAR_TWO_HAND ) )
            {
                    if ( !get_eq_char( victim, WEAR_WIELD_2 ) )
                    if( !wp2 )
                            return FALSE;
                    //chance = get_skill_percent(victim, gsn_parry)/8;
                    chance = get_skill_percent(victim, gsn_dual) / 6;
            }
            else
                    chance = get_skill_percent(victim, gsn_parry) / 5; */

            if (wp) // max 37
            {
                // parry 基本值 , max 33
                chance = get_skill_percent(victim, gsn_parry) / 11;                                          // learn 99% = 9
                chance += (get_skill_level(victim, gsn_parry) * get_skill_level(victim, gsn_parry) * 2) / 3; // lv 6 24

                // lance mastery 會確實依 wp type 作出反應 2022/12/03
                // chance  = chance + GetBonusedValue_LanceMaster(ch, chance)/ 8;
                if (wp->value[3] == 2) {
                    chance = chance + GetBonusedValue_LanceMaster(victim, chance) / 15; // lv 6  max 4
                } else if (get_skill_level(victim, gsn_lance_mastery) > 5) {
                    if (wp->value[3] == 1 || wp->value[3] == 3 || wp->value[3] == 7 || wp->value[3] == 9) {
                        chance = chance + GetBonusedValue_LanceMaster(victim, chance) / 15;
                    }
                }

                // 判定是否拿得動 wp, 過重會降低 parry 效益 2022/12/03
                if (isTwohand) { // 重量判定雙手武器與單手不同
                    for (min_str = 0; min_str <= MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].twohand >= get_obj_weight(wp))
                            break;
                    }
                } else {
                    for (min_str = 0; min_str <= MAX_STR + 6; ++min_str) {
                        if (str_app[min_str].wield >= get_obj_weight(wp))
                            break;
                    }
                }
                str_diff = get_curr_str(victim) - min_str;
                if (get_skill_percent(victim, gsn_powerblow) > 0)
                    str_diff += get_skill_level(victim, gsn_powerblow) * get_skill_level(victim, gsn_powerblow) / 6;
                if (str_diff < 0) {
                    if (str_diff < -5)
                        chance /= 10;
                    else
                        chance /= 2;
                }
            }

            if (wp2) // max 27
            {
                // 有 dual 的情況是不可能出現雙手武器(twohand)的狀態, 如果有就是 bug
                if (isTwohand) {
                    bug("Parry with wp2 but twohand player name :", 0);
                    bug(victim->name, 0);
                    return FALSE;
                }
                // wp2 parry 基本值 ,max 24
                chance2 = (get_skill_percent(victim, gsn_parry) + get_skill_percent(victim, gsn_dual)) / 16; // dual , parry 99% = 12
                chance2 += (get_skill_level(victim, gsn_parry) * get_skill_level(victim, gsn_parry)) / 6;    // lv 6 6
                chance2 += (get_skill_level(victim, gsn_dual) * get_skill_level(victim, gsn_dual)) / 6;      // lv 6 6

                // 加入副手 lance mastery 判定 2022/12/03
                if (wp2->value[3] == 2) {
                    chance2 = chance2 + GetBonusedValue_LanceMaster(victim, chance2) / 15; // lv 6 max 3
                } else if (get_skill_level(victim, gsn_lance_mastery) > 5) {
                    if (wp2->value[3] == 1 || wp2->value[3] == 3 || wp2->value[3] == 7 || wp2->value[3] == 9) {
                        chance2 = chance2 + GetBonusedValue_LanceMaster(victim, chance2) / 15;
                    }
                }

                // 判定是否拿得動 wp2 , 過重會降低 parry 效益 2022/12/03
                for (min_str = 0; min_str <= MAX_STR + 6; ++min_str) {
                    if (str_app[min_str].wield2 >= get_obj_weight(wp2))
                        break;
                }
                str_diff = get_curr_str(victim) - min_str;
                if (get_skill_percent(victim, gsn_powerblow) > 0)
                    str_diff += get_skill_level(victim, gsn_powerblow) * get_skill_level(victim, gsn_powerblow) / 6;
                if (str_diff < 0) {
                    if (str_diff < -5)
                        chance2 /= 10;
                    else
                        chance2 /= 2;
                }
            }
        }
    }

    if (!isFistParry)
        if (!wp && !wp2)
            return FALSE;

    // stone form 狀況下chance 減半 2022/12/05
    if (is_affected(victim, gsn_stone_form)) {
        fReposte = FALSE;
        chance /= 2;
        chance2 /= 2;
    }

    if (number_percent() - mod >= chance - URANGE(-10, leveldiff, 30)) // chance parry failed
    {
        if (!wp2 || (number_percent() - mod >= chance2 - URANGE(-10, leveldiff, 30))) // chance2 parry failed
        {
            if (IS_NPC(victim))
                return FALSE;

            if (fReposte) // 失敗判定 reposte
            {
                reposte = get_skill_percent(victim, gsn_reposte) / 6;
                reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4) / 4;
                dual_reposte = get_skill_percent(victim, gsn_dual) / 12;
                dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 8;
                if (reposte > number_percent()) {
                    act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack." NOR : HIY "$N 巧妙利用你攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_CHAR);
                    act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack." NOR : HIY "你巧妙利用 $n 攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_VICT);
                    mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE);

                    // if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
                    if (wp2 && (number_percent() < dual_reposte)) {
                        act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack with secondary weapon." NOR : HIY "$N 在反擊中以副手武器對你進行追擊." NOR, ch, NULL, victim, TO_CHAR);
                        act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack with secondary weapon." NOR : HIY "你在反擊中以副手武器對 $n 進行追擊." NOR, ch, NULL, victim, TO_VICT);
                        mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE);
                    }
                    return TRUE;
                }
            }
            return FALSE;
        }
        isWp2Parry = TRUE; //  wp2 的 chance2 parry 成功 2022/12/03
    }

    if (fReposte) // 判定 reposte
    {
        reposte = get_skill_percent(victim, gsn_reposte) / 5;
        reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4);
        dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
        dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 2;

        if (reposte > number_percent()) {
            act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack." NOR : HIY "$N 巧妙利用你攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_CHAR);
            act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack." NOR : HIY "你巧妙利用 $n 攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_VICT);
            mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE);

            // if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
            if (wp2 && (number_percent() < dual_reposte)) {
                act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack with secondary weapon." NOR : HIY "$N 在反擊中以副手武器對你進行追擊." NOR, ch, NULL, victim, TO_CHAR);
                act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack with secondary weapon." NOR : HIY "你在反擊中以副手武器對 $n 進行追擊." NOR, ch, NULL, victim, TO_VICT);
                mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE);
            }
            return TRUE;
        }
    }

    /* 加入攻擊重量對 parry 突破 2022/12/03 */

    // ch 攻擊重量計算

    if (!IS_NPC(ch)) {                           // 職業加成
        if (ch->pcdata->prime_attr == APPLY_STR) // ch base war
            weight_diff += 5;

        if (ch->pcdata->snd_attr == APPLY_STR) // ch snd war
            weight_diff += 5;
    } else {
        if (IS_SET(ch->class, 1)) // npc ch war
            weight_diff += ch->level / 5;

        if (IS_SET(ch->special, EXPERT)) // 進階 npc
            weight_diff += ch->level / 4;
    }

    if (is_affected(ch, gsn_powerup)) // powerup 加成
        weight_diff += 5 + get_skill_level(ch, gsn_powerup) * 10;

    // 代入命中武器直接統合 ch 使用的裝備 2022/12/28
    ch_wp  = check_using_wp(ch, dt, wpn);
    ch_wp2 = check_using_wp2(ch, dt);

    if (dt == gsn_kick) // kick
    {
        weight_diff += 10 + get_skill_level(ch, gsn_kick) * 10;
        /*if( get_eq_char( ch, WEAR_FEET ) )  // 加入 ch feet 裝備重量
                weight_diff += get_eq_char( ch, WEAR_FEET )->weight;
        if( get_eq_char( ch, WEAR_FEET_2 ) )
                weight_diff += get_eq_char(ch, WEAR_FEET_2 )->weight; */
        if (ch_wp)
            weight_diff += ch_wp->weight;
        if (ch_wp2)
            weight_diff += ch_wp2->weight;
    } else if (dt == TYPE_HIT) // 空手攻擊
    {
        if (!IS_NPC(ch)) {
            if (IS_SET(ch->act, PLR_FIST_FIGHT)) // pc fist fight
                weight_diff += 10 + get_skill_level(ch, gsn_fist_fight) * 5;
        } else {
            weight_diff += ch->level;
        }
        /*if( get_eq_char( ch, WEAR_HANDS ) ) // 加入 ch hands 裝備重量
                weight_diff += get_eq_char( ch, WEAR_HANDS )->weight;
        if( get_eq_char( ch, WEAR_HANDS_2 ) )
                weight_diff += get_eq_char(ch, WEAR_HANDS_2 )->weight; */
        if (ch_wp)
            weight_diff += ch_wp->weight;
        if (ch_wp2)
            weight_diff += ch_wp2->weight;
    } else {
        if (!IS_NPC(ch)) {
            if (get_skill_percent(ch, gsn_powerblow) > 0) // powerblow
                weight_diff += 10 + get_skill_level(ch, gsn_powerblow) * 5;
        }
        /*if( get_eq_char( ch, wpn ) )  // 加入 ch 攻擊武器的重量
                weight_diff += get_eq_char( ch, wpn )->weight; */
        // 除 kick 及 type_hit 外, 有手持武器且使用副武器的只會是 射箭 及 槍械 所以不判定 ch_wp2 2022/12/28
        if (ch_wp)
            weight_diff += ch_wp->weight;
    }

    // victim 格擋重量計算

    if (isFistParry) { // 空手格擋
        if (!IS_NPC(victim)) {
            weight_diff = weight_diff - (victim->level * 3) / 4 - get_skill_level(victim, gsn_barehand) * 3; // pc 空手 parry 基本值
        } else {
            weight_diff = weight_diff - victim->level; // npc 空手 parry 基本值為 lv
        }
        if (get_eq_char(victim, WEAR_HANDS)) // 扣除 hands 裝備重量
            weight_diff = weight_diff - get_eq_char(victim, WEAR_HANDS)->weight;
        if (get_eq_char(victim, WEAR_HANDS_2))
            weight_diff = weight_diff - get_eq_char(victim, WEAR_HANDS_2)->weight;
    }

    // 雙手武器能降低 20% 重量差 2022/12/30
    if (isTwohand && weight_diff > 0)
        weight_diff = (weight_diff * 8) / 10;

    else if (isWp2Parry && wp2) // 扣除 wp2 重量
        weight_diff = weight_diff - wp2->weight;
    else if (wp) // 扣除 wp 重量
        weight_diff = weight_diff - wp->weight - 10;

    // 算式成立則 parry 被突破 , parry sklv*2 為保證不被突破的基本值
    if ((get_skill_level(victim, gsn_parry) * 2 < number_percent()) && weight_diff > number_percent()) {
        act((!is_chinese(ch, victim)) ? "You BREAK $N's parry!" : "你的攻擊突破 $N 的武器格擋！", ch, NULL, victim, TO_CHAR);
        act((!is_chinese(victim, ch)) ? "$n's attack BREAK your parry!" : "你的武器格擋被 $n 突破了！", ch, NULL, victim, TO_VICT);
        return FALSE;
    }

    act((!is_chinese(ch, victim)) ? "$N parries your attack." : "$N 架開了你的攻擊.", ch, NULL, victim, TO_CHAR);
    act((!is_chinese(victim, ch)) ? "You parry $n's attack." : "你架開了 $n 的攻擊.", ch, NULL, victim, TO_VICT);

    // 加入 parry 對武器磨損, obj_block prog, obj_hp 判定 2022/12/03
    if (isWp2Parry && wp2) // 以 wp2 parry
    {
        if (!IS_NPC(victim)) // 武器磨損
        {
            if (wp2->value[3] != 14 && number_bits(5) < 8) {
                if (get_skill_level(victim, gsn_weapon_master) * 6 < number_percent())
                    waste_dura(wp2);
            }
        }
        if (wp2 && wp2->pIndexData->progtypes & OBJ_BLOCK) // obj_block prog 判定
            oprog_block_trigger(wp2);
        if (wp2 && (wp2->hp || wp2->material->hp)) // hp_block 判定
        {
            // Weapon Master 等級對武器戰鬥觸發的 hp 等級加成
            int WmArtLv = 0;
            if (!IS_NPC(victim) && wp2->wear_loc == WEAR_WIELD_2) {
                WmArtLv += get_skill_level(victim, gsn_weapon_master);
                if (get_skill_level(victim, gsn_weapon_master) > 3)
                    WmArtLv += 2;
                if (get_skill_level(victim, gsn_weapon_master) > 4)
                    WmArtLv += 3;
                if (get_skill_level(victim, gsn_weapon_master) > 5)
                    WmArtLv += 4;
            }

            OBJ_HP *ohp = wp2->hp;
            for (; ohp; ohp = ohp->next) {
                if (IS_SET(ohp->event, HP_BLOCK)) {
                    if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_ARTIFACT) &&
                         (artifact(victim) * ohp->percent * get_curr_wis(victim) / 5000 +
                          (get_skill_level(victim, gsn_artifact) == 6 ? 20 : 10)) > number_percent()) ||
                        (IS_NPC(victim) && ohp->percent * get_curr_wis(victim) / 50 > number_percent())) {
                        trigger_obj_hp(wp2, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
                    }
                }
            }
            if (wp2->material->hp) {
                for (ohp = wp2->material->hp; ohp; ohp = ohp->next) {
                    if (IS_SET(ohp->event, HP_BLOCK)) {
                        if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_ARTIFACT) &&
                             (artifact(victim) * ohp->percent * get_curr_wis(victim) / 5000 +
                              (get_skill_level(victim, gsn_artifact) == 6 ? 20 : 10)) > number_percent()) ||
                            (IS_NPC(victim) && ohp->percent * get_curr_wis(victim) / 50 > number_percent())) {
                            trigger_obj_hp(wp2, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
                        }
                    }
                }
            }
        }
    } else if (wp) // 以 wp parry
    {
        if (!IS_NPC(victim)) // 武器磨損
        {
            if (wp->value[3] != 14 && number_bits(5) < 8) {
                if (get_skill_level(victim, gsn_weapon_master) * 6 < number_percent())
                    waste_dura(wp);
            }
        }
        if (wp && wp->pIndexData->progtypes & OBJ_BLOCK) // obj_block prog 判定
            oprog_block_trigger(wp);
        if (wp && (wp->hp || wp->material->hp)) // hp_block 判定
        {
            // Weapon Master 等級對武器戰鬥觸發的 hp 等級加成
            int WmArtLv = 0;
            if (!IS_NPC(victim) && (wp->wear_loc == WEAR_TWO_HAND || wp->wear_loc == WEAR_WIELD)) {
                WmArtLv += get_skill_level(victim, gsn_weapon_master);
                if (get_skill_level(victim, gsn_weapon_master) > 3)
                    WmArtLv += 2;
                if (get_skill_level(victim, gsn_weapon_master) > 4)
                    WmArtLv += 3;
                if (get_skill_level(victim, gsn_weapon_master) > 5)
                    WmArtLv += 4;
            }

            OBJ_HP *ohp = wp->hp;
            for (; ohp; ohp = ohp->next) {
                if (IS_SET(ohp->event, HP_BLOCK)) {
                    if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_ARTIFACT) &&
                         (artifact(victim) * ohp->percent * get_curr_wis(victim) / 3000 +
                          (get_skill_level(victim, gsn_artifact) == 6 ? 35 : 20)) > number_percent()) ||
                        (IS_NPC(victim) && ohp->percent * get_curr_wis(victim) / 25 > number_percent())) {
                        trigger_obj_hp(wp, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
                    }
                }
            }
            if (wp->material->hp) {
                for (ohp = wp->material->hp; ohp; ohp = ohp->next) {
                    if (IS_SET(ohp->event, HP_BLOCK)) {
                        if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_ARTIFACT) &&
                             (artifact(victim) * ohp->percent * get_curr_wis(victim) / 3000 +
                              (get_skill_level(victim, gsn_artifact) == 6 ? 35 : 20)) > number_percent()) ||
                            (IS_NPC(victim) && ohp->percent * get_curr_wis(victim) / 25 > number_percent())) {
                            trigger_obj_hp(wp, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
                        }
                    }
                }
            }
        }
    }

    if (!IS_NPC(ch) && is_affected(ch, gsn_powerup) && get_skill_level(ch, gsn_powerup) > 5) {
        if (number_percent() < get_skill_level(ch, gsn_powerup) * 4) {
            disarm(ch, victim);
        }
    }

    return TRUE;
}

/*
 * Check for block.
 */
// check_block: Combat logic: check_block - Applies combat calculations, checks, or damage formulas.
// check_block: 戰鬥邏輯：check_block - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_block(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff) {
    OBJ_DATA *obj;
    int       weight;
    int       chance;
    int       break_shield;
    int       mod   = 100;
    int       bsmod = 100;
    int       class_bonus;
    int       nBlockSklv = get_skill_level(victim, gsn_block);
    int       nBlockPerc = get_skill_percent(victim, gsn_block);
    int       reposte;         // 191213 加入 block reposte判定
    int       dual_reposte;    // 2021/11/16 dual_reposte
    bool      fReposte = TRUE; // 191213 加入 block reposte判定

    if (!(obj = get_eq_char(victim, WEAR_SHIELD)))
        return FALSE;

    if (!IS_AWAKE(victim))
        return FALSE;

    weight = get_obj_weight(obj);

    if (!IS_NPC(victim)) {
        if (victim->pcdata->autocombat == AUTOC_PARRY) {
            int nAutoParryPerc = get_skill_percent(victim, gsn_autoparry);
            int nAutoParrySklv = get_skill_level(victim, gsn_autoparry);
            // MAX_ORG += 50 + 12
            mod += nAutoParryPerc / 4 + nAutoParrySklv * (nAutoParrySklv + 4) / 2;
            bsmod += nAutoParryPerc / 2 + nAutoParrySklv * (nAutoParrySklv + 4);
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_DODGE) {
            int nAutoDodgePerc = get_skill_percent(victim, gsn_autododge);
            int nAutoDodgeSklv = get_skill_level(victim, gsn_autododge);
            bsmod += nAutoDodgePerc / 4 + nAutoDodgeSklv * (nAutoDodgeSklv + 4) / 2;
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_STOP // 191213 加入 auto stop respote判定
                   || victim->pcdata->autocombat == AUTOC_CONCENTRATION) {
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_CRITICAL) {
            mod += get_skill_percent(victim, gsn_deadly_strike) / 3 + get_skill_level(victim, gsn_deadly_strike);
            bsmod += get_skill_percent(victim, gsn_autododge) / 5 + get_skill_level(victim, gsn_autododge);
        }
    }

    class_bonus = IS_NPC(ch) ? 0 : (ch->pcdata->prime_attr == APPLY_STR || ch->pcdata->snd_attr == APPLY_STR) ? 1
                               : (ch->pcdata->prime_attr == APPLY_DEX || ch->pcdata->snd_attr == APPLY_DEX)   ? 2
                                                                                                              : 0;
    if (IS_NPC(victim)) {
        fReposte = FALSE; // 191213
        /* Tuan was here.  :) */
        if (IS_SET(victim->special, EXPERT))
            chance = UMIN(60, (weight + victim->level) / 2 - class_bonus * 15);
        else
            chance = UMIN(30, (weight + victim->level) / 3 - class_bonus * 8);
    } else {
        // int nBlockSklv = get_skill_level(victim, gsn_block);
        // int nBlockPerc = get_skill_percent(victim, gsn_block);
        //  13 15 19 23 29 35 43
        chance = nBlockSklv * (nBlockSklv + 4) / 2 + nBlockPerc / 6 + weight / 3;
        bsmod += chance;
        //   * ORG_MAX = 15 + W/2 + Sklv * 1.5 = 15 + 9 + W/2 = 24 + W/2
        //      chance = (get_skill_percent(victim, gsn_block) / 3 + weight) / 2
        //             +  get_skill_level(victim, gsn_block) + get_skill_level(victim, gsn_block) / 2;
    }

    // stone form 狀況下chance 減半 2022/12/05
    if (is_affected(victim, gsn_stone_form)) {
        fReposte = FALSE;
        chance /= 2;
    }

    if (URANGE(2 + get_curr_str(victim) / 3, chance - UMAX(0, leveldiff), 60) < number_percent() * 100 / mod)
        return FALSE;

    // break_shield = ( IS_NPC( ch ) ) ? IS_SET( ch->class, 8 ) ? ch->level : 0
    //				: get_skill_percent(ch, gsn_break_shield) / 6;
    if (IS_NPC(ch) && IS_SET(ch->class, 8))
        break_shield = UMIN(25, ch->level / 3); // lv 50 , 16 ; lv 55 , 18  ; lv 60 , 20
    else if (!IS_NPC(ch))
        break_shield = get_skill_percent(ch, gsn_break_shield) / 5; // 19.8
    else
        break_shield = 0;

    if (!IS_NPC(ch))
        // break_shield += get_skill_level(ch, gsn_break_shield) * (get_skill_level(ch, gsn_break_shield) + 4) * 2 / 3;
        break_shield += get_skill_level(ch, gsn_break_shield) * (get_skill_level(ch, gsn_break_shield) + 4); // 60
    else
        break_shield += get_skill_level(ch, gsn_break_shield) * 3; // 18

    // block sklv have better chance to prevent break shield.
    if (!IS_NPC(victim)) {
        if (victim->pcdata->prime_attr == APPLY_STR)
            break_shield = break_shield - nBlockSklv * 3;
        else
            break_shield = break_shield - nBlockSklv * 2;
    }
    if (break_shield > number_percent() * bsmod / 100) {
        /*
        act( (is_chinese(ch, victim)) ?
                "你突破 $N 的盾牌防禦!" :
                "You break through $N's shield blocks!", ch, NULL, victim, TO_CHAR );
        act( (is_chinese(victim, ch)) ?
                "$n 突破你的盾牌防禦!" :
                "$n break through your shield blocks!",  ch, NULL, victim, TO_VICT );
        */
        act((is_chinese(ch, victim)) ? "你崩解 $N 的盾牌抵擋!" : "You break $N's shield blocks!", ch, NULL, victim, TO_CHAR);
        act((is_chinese(victim, ch)) ? "$n 崩解你的盾牌抵擋!" : "$n break your shield blocks!", ch, NULL, victim, TO_VICT);

        if (number_bits(3) == 1 && !IS_SET(obj->extra_flags, ITEM_NOREMOVE)) {
            unequip_char(victim, obj);
            waste_dura(obj);
            obj_from_char(obj);
            if (IS_NPC(victim) || nBlockSklv >= 3) {
                obj_to_char(obj, victim);
            } else if (can_drop_obj(victim, obj)) {
                obj_to_room(obj, victim->in_room);
            } else
                obj_to_char(obj, victim);

            // 色碼修正 2022/11/04
            if (!is_chinese(ch, victim)) {
                // act( HIY"You disarm $N's shield!"NOR,  ch, NULL, victim, TO_CHAR	 );
                // act( "[1;31m$n DISARMS $N's shield![m",  ch, NULL, victim, TO_NOTVICT );
                act("" HIY "You disarm " NOR "$N" HIY "'s shield!" NOR "", ch, NULL, victim, TO_CHAR);
                act("$n" HIY " DISARM " NOR "$N" HIY "'s shield!" NOR "", ch, NULL, victim, TO_NOTVICT);
            } else {
                // act( HIY"你擊落了 $N 的盾牌！"NOR,  ch, NULL, victim, TO_CHAR	 );
                // act( "[1;31m$n 擊落了 $N 的盾牌！[m",  ch, NULL, victim, TO_NOTVICT );
                act("" HIY "你擊落了 " NOR "$N" HIY " 的盾牌！" NOR "", ch, NULL, victim, TO_CHAR);
                act("$n" HIY " 擊落了 " NOR "$N" HIY " 的盾牌！" NOR "", ch, NULL, victim, TO_NOTVICT);
            }
            // 被擊落者會看到自己被擊落的 obj 名 2022/11/04
            if (!is_chinese(victim, ch)) {
                // act( "[1;31m$n DISARMS your shield![m", ch, NULL, victim, TO_VICT	 );
                act("$n" HIY " DISARMS your shield " NOR "$p" HIY " !" NOR "", ch, obj, victim, TO_VICT);
            } else {
                // act( "[1;31m$n 擊落了你的盾牌！[m", ch, NULL, victim, TO_VICT	 );
                act("$n" HIY " 擊落了你的盾牌 " NOR "$p" HIY " ！" NOR "", ch, obj, victim, TO_VICT);
            }
        }
        return FALSE;
    }

    // 191213  加入 block reposte 判定
    if (fReposte) {
        reposte = get_skill_percent(victim, gsn_reposte) / 5;
        reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4);
        dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
        dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 2;

        if (reposte > number_percent()) {
            act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack." NOR : HIY "$N 巧妙利用你攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_CHAR);
            act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack." NOR : HIY "你巧妙利用 $n 攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_VICT);
            mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE);
            /*
                    修改為 dual 的情況下作出副手武器反擊 2021/11/16

                    if( ch->class != NINJA && (victim->class == NINJA && !IS_NPC(victim))
                            && number_percent() < get_curr_dex(victim) / 2 + reposte / 2 )
                    {
                            act( (!is_chinese(ch, victim)) ?
                                    HIY"$N repostes your attack again."NOR :
                                    HIY"$N 再次利用你攻擊時的破綻反擊."NOR, ch, NULL, victim, TO_CHAR );
                            act( (!is_chinese(victim, ch)) ?
                                    HIY"You repostes $n's attack again." NOR:
                                    HIY"你再次利用 $n 攻擊時的破綻反擊."NOR,  ch, NULL, victim, TO_VICT );
                            mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
                    }
            */
            if (get_eq_char(victim, WEAR_WIELD_2) && (number_percent() < dual_reposte)) {
                act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack with secondary weapon." NOR : HIY "$N 在反擊中以副手武器對你進行追擊." NOR, ch, NULL, victim, TO_CHAR);
                act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack with secondary weapon." NOR : HIY "你在反擊中以副手武器對 $n 進行追擊." NOR, ch, NULL, victim, TO_VICT);
                mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE);
            }
            return TRUE;
        }
    }

    act((is_chinese(ch, victim)) ? "$N 用盾牌擋住你的攻擊." : "$N blocks your attack.", ch, NULL, victim, TO_CHAR);
    act((is_chinese(victim, ch)) ? "你用盾牌擋住 $n 的攻擊." : "You block $n's attack.", ch, NULL, victim, TO_VICT);

    // 增加 block obj_block 的判定 2022/10/23
    if (obj && obj->pIndexData->progtypes & OBJ_BLOCK)
        oprog_block_trigger(obj);

    if (obj && (obj->hp || obj->material->hp)) {
        OBJ_HP *ohp = obj->hp;
        for (; ohp; ohp = ohp->next) {
            if (IS_SET(ohp->event, HP_BLOCK)) {
                if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_ARTIFACT) &&
                     (artifact(victim) * ohp->percent * get_curr_wis(victim) / 2500 +
                      (get_skill_level(victim, gsn_artifact) == 6 ? 40 : 30)) > number_percent()) ||
                    (IS_NPC(victim) && ohp->percent * get_curr_wis(victim) / 25 > number_percent())) {
                    trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, victim, ch);
                }
            }
        }
        if (obj->material->hp) {
            for (ohp = obj->material->hp; ohp; ohp = ohp->next) {
                if (IS_SET(ohp->event, HP_BLOCK)) {
                    if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_ARTIFACT) &&
                         (artifact(victim) * ohp->percent * get_curr_wis(victim) / 2500 +
                          (get_skill_level(victim, gsn_artifact) == 6 ? 40 : 30)) > number_percent()) ||
                        (IS_NPC(victim) && ohp->percent * get_curr_wis(victim) / 25 > number_percent())) {
                        trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, victim, ch);
                    }
                }
            }
        }
    }

    // mon 有 powerup的情況下被 block 一定機率 disarm 對手 shield
    // if( !IS_NPC(ch) && is_affected(ch, gsn_powerup) && get_skill_level(ch, gsn_powerup) > 5)
    if (is_affected(ch, gsn_powerup)) {
        // if( number_percent() < 2 * get_skill_level(ch, gsn_powerup) ) {
        /*  remove at 2020/10/30
        unequip_char( victim, obj);
        waste_dura(obj);
        obj_from_char( obj );

        if ( IS_NPC( victim ) )
                obj_to_char( obj, victim );
        else if ( can_drop_obj(victim, obj))
        {
                obj_to_room( obj, victim->in_room );
        }
        else
                obj_to_char( obj, victim );
        waste_dura(obj);
        obj_from_char( obj );
        if ( IS_NPC( victim ) )
        obj_to_char( obj, victim );
        else
        obj_to_room( obj, victim->in_room );
        if( !IS_NPC( ch ) &&!IS_SET( ch->act, PLR_CHINESE ) ) */

        // 增加 str 差值及 block sklv 判定  2020/10/30
        if (2 * nBlockSklv + number_percent() < 4 * get_skill_level(ch, gsn_powerup) + get_curr_str(ch) - get_curr_str(victim)) {
            // 增加判定是否有 no remove 及技能等級擊落是否掉落地面 2020/10/30
            if (!IS_SET(obj->extra_flags, ITEM_NOREMOVE)) {
                unequip_char(victim, obj);
                waste_dura(obj);
                obj_from_char(obj);
                if (IS_NPC(victim) || nBlockSklv >= 3) {
                    obj_to_char(obj, victim);
                } else if (can_drop_obj(victim, obj)) {
                    obj_to_room(obj, victim->in_room);
                } else
                    obj_to_char(obj, victim);

                if (!is_chinese(ch, victim)) {
                    act(HIY "You disarm $N's shield!" NOR, ch, NULL, victim, TO_CHAR);
                    act("[1;31m$n DISARMS $N's shield![m", ch, NULL, victim, TO_NOTVICT);
                } else {
                    act(HIY "你擊落了 $N 的盾牌！" NOR, ch, NULL, victim, TO_CHAR);
                    act("[1;31m$n 擊落了 $N 的盾牌！[m", ch, NULL, victim, TO_NOTVICT);
                }

                if (!is_chinese(victim, ch)) {
                    act("[1;31m$n DISARMS your shield![m", ch, NULL, victim, TO_VICT);
                } else {
                    act("[1;31m$n 擊落了你的盾牌！[m", ch, NULL, victim, TO_VICT);
                }
            }
        }
    }
    return TRUE;
}

/*
 * Check for tactics.
 */
// victim 與 ch 錯亂, 故改為與 fight_damage 的位置一樣 2022/12/04
// bool check_tactics( CHAR_DATA *ch, CHAR_DATA *victim )
// check_tactics: Combat logic: check_tactics - Applies combat calculations, checks, or damage formulas.
// check_tactics: 戰鬥邏輯：check_tactics - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_tactics(CHAR_DATA *victim, CHAR_DATA *ch) {
    int  chance = 0;
    int  reposte;         // 191213 加入 block reposte判定
    int  dual_reposte;    // 2021/11/16 dual_reposte
    bool fReposte = TRUE; // 191213 加入 block reposte判定
    int  nTacSklv = get_skill_level(victim, gsn_tactics);

    if (!IS_NPC(victim) && (victim->pcdata->autocombat == AUTOC_PARRY || victim->pcdata->autocombat == AUTOC_DODGE || victim->pcdata->autocombat == AUTOC_CONCENTRATION || victim->pcdata->autocombat == AUTOC_STOP)) {
        fReposte = FALSE;
    }

    if (IS_NPC(victim))
        fReposte = FALSE;

    if (!IS_AWAKE(victim))
        return FALSE;

    if (!IS_NPC(victim)) {
        // 阻擋不會 tactic 的情況 2022/12/04
        if (victim->pcdata->learned[gsn_tactics]) {
            if (victim->class == WARLORD)
                chance += get_curr_int(victim);
            else
                chance += get_curr_int(victim) / 3;
            chance += ((nTacSklv * nTacSklv * 2) / 3) * get_skill_percent(victim, gsn_tactics) / 100;
            if (number_percent() > chance)
                return FALSE;
        } else {
            return FALSE;
        }
    } else if (!IS_SET(victim->act, ACT_TACTICS)) {
        return FALSE;
    } else {
        chance += get_curr_int(victim) / 2;
        chance += (nTacSklv * nTacSklv * 2) / 5;
        chance = UMIN(30, victim->level / 2);
        if (number_percent() > chance)
            return FALSE;
    }
    // 191213  加入 tactics reposte 判定
    if (fReposte) {
        reposte = get_skill_percent(victim, gsn_reposte) / 8;
        reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 2);
        dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
        dual_reposte += get_skill_level(victim, gsn_dual) * get_skill_level(victim, gsn_dual) / 2;

        if (reposte > number_percent()) {
            act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack." NOR : HIY "$N 巧妙利用你攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_CHAR);
            act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack." NOR : HIY "你巧妙利用 $n 攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_VICT);
            mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE);
            /*
                    修改為 dual 的情況下作出副手武器反擊 2021/11/16

                    if( ch->class != NINJA && (victim->class == NINJA && !IS_NPC(victim))
                            && number_percent() < get_curr_dex(victim) / 2 + reposte / 2 )
                    {
                            act( (!is_chinese(ch, victim)) ?
                                    HIY"$N repostes your attack again."NOR :
                                    HIY"$N 再次利用你攻擊時的破綻反擊."NOR, ch, NULL, victim, TO_CHAR );
                            act( (!is_chinese(victim, ch)) ?
                                    HIY"You repostes $n's attack again." NOR:
                                    HIY"你再次利用 $n 攻擊時的破綻反擊."NOR,  ch, NULL, victim, TO_VICT );
                            mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
                    }
            */
            if (get_eq_char(victim, WEAR_WIELD_2) && (number_percent() < dual_reposte)) {
                act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack with secondary weapon." NOR : HIY "$N 在反擊中以副手武器對你進行追擊." NOR, ch, NULL, victim, TO_CHAR);
                act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack with secondary weapon." NOR : HIY "你在反擊中以副手武器對 $n 進行追擊." NOR, ch, NULL, victim, TO_VICT);
                mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE);
            }
            return TRUE;
        }
    }

    if (!is_chinese(ch, victim)) {
        act("$N use tactics to parry your attack.", ch, NULL, victim, TO_CHAR);
    } else {
        act("$N 看出了你的動向，輕鬆架開你的攻擊.", ch, NULL, victim, TO_CHAR);
    }
    if (!is_chinese(victim, ch)) {
        act("You use tactics to parry $n's attack.", ch, NULL, victim, TO_VICT);
    } else {
        act("你看出了 $n 的動向，輕鬆架開$s攻擊.", ch, NULL, victim, TO_VICT);
    }
    return TRUE;
}

// frenzy_check_dodge: Core Engine function: frenzy_check_dodge - Main C routine handling game mechanics.
// frenzy_check_dodge: 核心引擎函式：frenzy_check_dodge - 處理遊戲底層機制的核心 C 語言子程序。
bool frenzy_check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff) {
    int  chance;
    int  ppl_dex;
    int  mod = 100;
    int  reposte;         // 191213 dodge reposte
    int  dual_reposte;    // 2021/11/16 dual_reposte
    bool fReposte = TRUE; // 191213 dodge reposte

    if (!IS_AWAKE(victim))
        return FALSE;

    if (IS_NPC(victim)) {
        fReposte = FALSE; // 191213
        /* Tuan was here.  :) */
        if (IS_SET(victim->special, EXPERT))
            chance = UMAX(10, victim->level / 3 + UMAX(0, victim->level - ((victim->class == 1) ? 30 : 60)));
        else
            chance = UMAX(8, victim->level / 4 + UMAX(0, victim->level - ((victim->class == 1) ? 20 : 40)));
    }

    if ((get_skill_level(victim, gsn_fool) + get_skill_level(victim, gsn_dodge) + get_skill_level(victim, gsn_tricky_pacing)) < 4)
        return FALSE;
    else
        chance = (get_skill_percent(victim, gsn_fool) + get_skill_percent(victim, gsn_dodge)) / 33; // 6 , 3

    if (!IS_NPC(victim)) {
        if (victim->pcdata->autocombat == AUTOC_DODGE) {
            chance += get_skill_percent(victim, gsn_autododge) / 33 + get_skill_level(victim, gsn_autododge) * 2; // 15
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_PARRY) {
            chance += get_skill_percent(victim, gsn_autoparry) / 33 + get_skill_level(victim, gsn_autoparry) * 2; // 15
            fReposte = FALSE;
        } else if (victim->pcdata->autocombat == AUTOC_CRITICAL) {
            chance += get_skill_percent(victim, gsn_deadly_strike) / 33 + get_skill_level(victim, gsn_deadly_strike); // 9
        } else if (victim->pcdata->autocombat == AUTOC_STOP || victim->pcdata->autocombat == AUTOC_CONCENTRATION) {
            fReposte = FALSE;
        }
    }

    if (!IS_NPC(victim)) {
        ppl_dex = get_curr_dex(victim);
        chance += get_skill_level(victim, gsn_fool) * 3 / 2 + get_skill_level(victim, gsn_dodge) * 5;                            // 39 , 30
        chance += ppl_dex / 12 - dexdiff / 12;                                                                                   // 2.5 + 2.5
        chance -= URANGE(0, leveldiff / 5, 15);                                                                                  // 0
        chance -= UMIN(0, (GET_DODGE(victim) * get_skill_level(victim, gsn_dodge) * get_skill_level(victim, gsn_dodge)) / 4500); // lv 6 dc -1000 8%
    }

    // TrickyPacing dodge chance + 20%
    chance += get_skill_percent(victim, gsn_tricky_pacing) / 30 + get_skill_level(victim, gsn_tricky_pacing) * 3;

    // stone form 狀況下chance 減半 2022/12/05
    if (is_affected(victim, gsn_stone_form)) {
        fReposte = FALSE;
        chance /= 2;
    }

    if (number_percent() * 100 / mod >= chance)
        return FALSE;

    // 191213  frenzy dodge reposte 判定
    if (fReposte) {
        reposte = 0;
        reposte += get_skill_level(victim, gsn_reposte) * 5;
        dual_reposte = 0;
        dual_reposte += get_skill_level(victim, gsn_dual) * 2;

        if (reposte > number_percent() && (get_skill_level(victim, gsn_reposte) > 4) && (victim->move > 100)) {
            act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack." NOR : HIY "$N 巧妙利用你攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_CHAR);
            act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack." NOR : HIY "你巧妙利用 $n 攻擊時的破綻反擊." NOR, ch, NULL, victim, TO_VICT);
            mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE);
            if (get_eq_char(victim, WEAR_WIELD_2) && (number_percent() < dual_reposte)) {
                act((!is_chinese(ch, victim)) ? HIY "$N repostes your attack with secondary weapon." NOR : HIY "$N 在反擊中以副手武器對你進行追擊." NOR, ch, NULL, victim, TO_CHAR);
                act((!is_chinese(victim, ch)) ? HIY "You repostes $n's attack with secondary weapon." NOR : HIY "你在反擊中以副手武器對 $n 進行追擊." NOR, ch, NULL, victim, TO_VICT);
                mone_hit(victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE);
            }
            victim->move -= 100;
            return TRUE;
        }
    }

    act((!is_chinese(ch, victim)) ? "$N dodges your attack." : "$N 靈巧地閃避你的攻擊.", ch, NULL, victim, TO_CHAR);
    act((!is_chinese(victim, ch)) ? "You dodges $n's attack." : "你靈巧地閃避$n的攻擊.", ch, NULL, victim, TO_VICT);
    return TRUE;
}

/*
 * See if an attack justifies a KILLER flag.
 */
// check_killer: Combat logic: check_killer - Applies combat calculations, checks, or damage formulas.
// check_killer: 戰鬥邏輯：check_killer - 進行戰鬥數值判定、安全檢查或傷害公式運算。
void check_killer(CHAR_DATA *ch, CHAR_DATA *victim) {
    // CHAR_DATA *vch;

    if (!ch || !victim || ch->deleted || victim->deleted)
        return;

    if (ch->in_room != victim->in_room)
        return;

    // ch 等於 victim
    if (ch == victim)
        return;

    // pk room
    if (IS_SET(victim->in_room->room_flags, ROOM_PK))
        return;

    // 任一方處於 crazy 的狀態下直接不判定兇手
    if (IS_AFFECTED(ch, AFF_CRAZY) || IS_AFFECTED(victim, AFF_CRAZY))
        return;

    // 判定 npc 被打
    if (IS_NPC(victim) && !IS_AFFECTED(victim, AFF_CHARM) && !IS_AFFECTED(victim, AFF_SUMMONED) && !victim->master) {
        return;
    }
    /*  下方已經有 npc 的 victim 轉換 , 故移除此段判定 2022/12/24
    else if( IS_NPC(victim)
                    && ( IS_AFFECTED( victim, AFF_CHARM ) || IS_AFFECTED( victim, AFF_SUMMONED) )
                    && victim->master && !IS_NPC(victim->master) )
    {
            victim = victim->master;
    } */

    // 有 pc 主人的 npc 攻擊的情況
    if (IS_NPC(ch)) {
        // charm 及 clan mob 判定 follow 的是否為 pc , 如果是則 ch 轉換成 follow 的角色
        // 注意: 不寫 pet 是因為 pet 一定是 charm mob , 所以 charm 就等於 pet
        if ((IS_AFFECTED(ch, AFF_CHARM) || ch->clan) && ch->master && !IS_NPC(ch->master))
            ch = ch->master;
        // sor mob 判定 remb 是否為 pc , 如果是則 ch 轉換成 remb
        if (IS_AFFECTED(ch, AFF_SUMMONED) && ch->rembch && !IS_NPC(ch->rembch))
            ch = ch->rembch;
    }

    // 有 pc 主人的 npc 被攻擊的情況
    if (IS_NPC(victim)) {
        // charm 及 clan mob 判定 follow 的是否為 pc , 如果是則 victim 轉換成 follow 的角色
        if ((IS_AFFECTED(victim, AFF_CHARM) || victim->clan) && victim->master && !IS_NPC(victim->master)) {
            victim = victim->master;
        }
        // sor mob 判定 remb 是否為 pc , 如果是則 victim 轉換成 remb
        if (IS_AFFECTED(victim, AFF_SUMMONED) && victim->rembch && !IS_NPC(victim->rembch)) {
            victim = victim->rembch;
        }
        // gate mob 判定 protect 的角色是否為 pc , 如果是則 victim 轉換成 protect 的角色
        if (victim->qmark && !str_cmp(victim->qmark->mark_name, "gated_mob") && victim->protecting)
            victim = victim->protecting; // I supposed the qmark "gated mob" is the toppest qmark on the gated mob
    }

    // 轉換後可能發生 ch 與 victim 不存在
    if (!ch || !victim || ch->deleted || victim->deleted)
        return;

    // 轉換後可能發生 ch 與 victim 不在同 room
    if (ch->in_room != victim->in_room)
        return;

    /*
     * NPC's are cool of course
     * Hitting yourself is cool too (bleeding).
     * Hitting immortals are fine.
     * 因為上面已經把 pc 使用 pet, charm, gate, sor mob, clan mob 轉換完成
     * 故跑到此處如果其中一方為 npc 則代表不會發生 pk 自然不會有 killer 產生
     * 但轉換後可能發生兩個主人相同, 或是主人殺自己的 npc
     * 所以當 ch 為 victim 時也不會產生 killer
     * 並移除等級差不產生 killer 的影響 2022/12/24
     */
    // if ( IS_NPC( ch ) || IS_NPC(victim) || ch == victim || victim->level > LEVEL_HERO + 5 )
    if (IS_NPC(ch) || IS_NPC(victim) || ch == victim)
        return;

    // 此處的 crazy 判定主要是針對 charm , 召喚物 , gate mob 轉換成 pc 後作的判定 2022/11/27
    if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_CRAZY))
        return;
    if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_CRAZY))
        return;

    // pc 被 charm 的情況下, 不會判定兇手 2022/11/27
    if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM) && IS_NPC(ch->master))
        return;
    if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_CHARM) && IS_NPC(victim->master))
        return;

    // pc 有一方為 killer 或是 thief 的情況下不會判定兇手 2022/11/27
    if (!IS_NPC(ch) && (IS_SET(ch->act, PLR_KILLER) || IS_SET(ch->act, PLR_THIEF)))
        return;
    if (!IS_NPC(victim) && (IS_SET(victim->act, PLR_KILLER) || IS_SET(victim->act, PLR_THIEF)))
        return;

    /*
     * KILLERs are fair game.
     * THIEVES are fair game too.
     * KILLER aggressors should not be penalized 1000 exps per attack but
     * per combat started.
     */
    if (IS_SET(victim->act, PLR_KILLER) || IS_SET(victim->act, PLR_THIEF) || (IS_SET(ch->act, PLR_KILLER) && ch->fighting))
        return;

    // Vampires are fair game.
    if (victim->race == race_lookup("Vampire"))
        return;

    if (!licensed(ch) && !licensed(victim)) {
        // 移除等級限制 2022/11/27
        // if ( registered( ch, victim ) && ch->level - victim->level < 6 )
        if (registered(ch, victim)) {
            return;
        }
    }

    // 修改兇手判定, 並移除兇手脫裝備的設定 2022/11/27
    if (!registered(ch, victim) || (!licensed(ch) && !licensed(victim)) && victim->fighting != ch) {
        char buf[BUF_STRING_LENGTH];
        sprintf(buf, "救命啊！  %s 正在攻擊我！", ch->name);
        do_shout(victim, buf);
        send_to_char("你是個令人厭惡的兇手！.\n\r", ch);
        SET_BIT(ch->act, PLR_KILLER);
    }

    /*if ( ( ch->level - victim->level > 5 && victim->fighting != ch ) || !registered( ch, victim ) || (!licensed( ch ) && !licensed(victim) ) )
    {
            OBJ_DATA *obj;
            OBJ_DATA *obj_next;
            char buf [ BUF_STRING_LENGTH ];

            send_to_char( "兇手！  你失去了 "HIG"600"NOR" 點經驗.\n\r", ch );
            sprintf( buf, "救命啊！  %s 正在攻擊我！", ch->name );
            do_shout( victim, buf );
            SET_BIT( ch->act, PLR_KILLER );
            for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
            {
                    if ( IS_NPC( vch ) )
                            continue;
                    if ( is_same_group( vch, ch ) )
                            SET_BIT( ch->act, PLR_KILLER );
            }
            gain_exp( ch, -600 );
            //demote_level( ch );

            for ( obj = ch->carrying; obj; obj = obj_next )
            {
                    obj_next = obj->next_content;

                    if ( obj->deleted )
                            continue;

                    //add by coconet for check horse
                    if ( IS_SET( obj->wear_flags , ITEM_WEAR_RIDE ) )
                            continue;

                    obj_from_char( obj );

                    // Remove item inventories Includes licenses to kill
                    if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
                    {
                            extract_obj( obj );
                            continue;
                    }

                    obj_to_char( obj, ch );
            }

            save_char_obj( ch );
    }*/

    return;
}

/** Check to see if weapon is poisoned.
 */
// is_wielding_poisoned: Core Engine function: is_wielding_poisoned - Main C routine handling game mechanics.
// is_wielding_poisoned: 核心引擎函式：is_wielding_poisoned - 處理遊戲底層機制的核心 C 語言子程序。
bool is_wielding_poisoned(CHAR_DATA *ch, int wpn) {
    OBJ_DATA *obj   = NULL;
    OBJ_DATA *arrow = NULL;
    OBJ_DATA *ammo  = NULL; // 2022/05/03

    if ((obj = get_eq_char(ch, wpn)) && IS_SET(obj->extra_flags, ITEM_POISONED) && obj->value[3] != 14)
        return TRUE;

    if ((obj = get_eq_char(ch, wpn)) && obj->value[3] == 14 && (arrow = get_eq_char(ch, WEAR_ARROW)) && arrow->item_type == ITEM_ARROW && IS_SET(arrow->extra_flags, ITEM_POISONED))
        return TRUE;

    // 新增槍械彈藥判定 2022/05/03
    if ((obj = get_eq_char(ch, wpn)) && obj->value[3] == 16 && (ammo = get_eq_char(ch, WEAR_AMMO)) && ammo->item_type == ITEM_AMMO && IS_SET(ammo->extra_flags, ITEM_POISONED))
        return TRUE;

    return FALSE;
}

// is_safe: Core Engine function: is_safe - Main C routine handling game mechanics.
// is_safe: 核心引擎函式：is_safe - 處理遊戲底層機制的核心 C 語言子程序。
bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim) {
    if (victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)) {
        return TRUE;
    }
    if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM) && ch->group && ch->group->leader == victim) {
        send_to_char("你不能對親愛的主人下手。\n\r", ch);
        return TRUE;
    }

    if (victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_PK))
        return FALSE;

    if (IS_SET(race_table[victim->race].race_abilities, RACE_VAMPIRE))
        return FALSE;

    /* suicide? OK....Amenda */
    if (ch == victim && !IS_SET(victim->act, PLR_HORSE))
        return FALSE;

    // 召喚物 打 pc 的 pk 判定
    if (IS_NPC(ch) && !IS_NPC(victim)) // added by Amethyst
    {
        // gate mob
        if (ch->qmark && !str_cmp(ch->qmark->mark_name, "gated_mob") && ch->protecting && !IS_NPC(ch->protecting) && !IS_SET(ch->protecting->act, AFF_CRAZY) // 如果 crazy 就能 pk 2022/12/24
            && !IS_SET(ch->protecting->act, PLR_REGISTER)) {
            if (!IS_NPC(ch->protecting)) // 如果召喚主是 pc 才會成立 2022/12/24
            {
                send_to_char("對方沒參加 PK ！", ch->protecting);
                return TRUE;
            }
        }
        // sor mob
        if (IS_AFFECTED(ch, AFF_SUMMONED) && ch->qmark && !str_cmp(ch->qmark->mark_name, "sum_life_time") // 加入 sor mob qmark 2022/12/24
            && ch->rembch && !IS_SET(ch->rembch->act, AFF_CRAZY)                                          // 如果 crazy 就能 pk 2022/12/24
            && !IS_SET(ch->rembch->act, PLR_REGISTER)) {
            if (!IS_NPC(ch->rembch)) // 如果召喚主是 pc 才會成立 2022/12/24
            {
                send_to_char("對方沒參加 PK ！", ch->protecting);
                return TRUE;
            }
        }
    }

    // pc 打 召喚物 的 pk 判定
    if (IS_NPC(victim) && !IS_NPC(ch)) // !IS_NPC is add here
    {
        // gate mob
        if (victim->qmark && !str_cmp(victim->qmark->mark_name, "gated_mob") && victim->protecting && !IS_NPC(victim->protecting) && !IS_SET(victim->protecting->act, AFF_CRAZY) // 如果 crazy 就能 pk 2022/12/24
            && !IS_SET(victim->protecting->act, PLR_REGISTER)) {
            if (!IS_NPC(victim->protecting)) // 如果召喚主是 pc 才會成立 2022/12/24
            {
                send_to_char("它的主人沒參加 PK ！", ch);
                return TRUE;
            }
        }
        // sor mob
        if (IS_AFFECTED(victim, AFF_SUMMONED) && victim->qmark && !str_cmp(victim->qmark->mark_name, "sum_life_time") // 加入 sor mob qmark 2022/12/24
            && victim->rembch && !IS_SET(victim->rembch->act, AFF_CRAZY)                                              // 如果 crazy 就能 pk 2022/12/24
            && !IS_SET(victim->rembch->act, PLR_REGISTER)) {
            if (!IS_NPC(victim->rembch)) // 如果召喚主是 pc 才會成立 2022/12/24
            {
                send_to_char("它的主人沒參加 PK ！", ch);
                return TRUE;
            }
        }
        if (IS_AFFECTED(victim, AFF_SUMMONED) && victim->qmark && !str_cmp(victim->qmark->mark_name, "sum_life_time") // 加入 sor mob qmark 2022/12/24
            && victim->rembch && !IS_SET(victim->rembch->act, AFF_CRAZY)                                              // 如果 crazy 就能 pk 2022/12/24
            && !IS_SET(victim->rembch->act, PLR_REGISTER)) {
            if (!IS_NPC(victim->rembch)) // 如果召喚主是 pc 才會成立 2022/12/24
            {
                send_to_char("它的主人沒參加 PK ！", ch);
                return TRUE;
            }
        }
    }

    if (IS_NPC(ch) || IS_NPC(victim))
        return FALSE;

    if (IS_SET(victim->act, PLR_KILLER) || IS_SET(victim->act, PLR_THIEF))
        return FALSE;

    /*  移除保護設定 2022/12/24
    if ( ch->level < 16 && !ch->fighting )
    {
            send_to_char( "你的等級還太低，不能參加 PK.\n\r", ch );
            return TRUE;
    }

    if ( victim->level < 25 )
    {
            act( "$N還不算冒險家，天神還在保護$e", ch, NULL, victim, TO_CHAR );
            return TRUE;
    } */

    // 如果 crazy 就能 pk 2022/12/24
    if (!IS_SET(ch->act, PLR_REGISTER) && !IS_AFFECTED(ch, AFF_CRAZY) && !IS_AFFECTED(victim, AFF_CRAZY)) {
        send_to_char("你沒有參加 PK.\n\r", ch);
        return TRUE;
    }

    if (ch->level >= L_APP) {
        send_to_char("你不能參加 PK.\n\r", ch);
        return TRUE;
    }

    if (victim->fighting)
        return FALSE;

    return FALSE;
}

// check_corpse_count: Combat logic: check_corpse_count - Applies combat calculations, checks, or damage formulas.
// check_corpse_count: 戰鬥邏輯：check_corpse_count - 進行戰鬥數值判定、安全檢查或傷害公式運算。
void check_corpse_count(CHAR_DATA *ch, CHAR_DATA *victim) {
    char   buf_q[MAX_INPUT_LENGTH];
    QMARK *qm;
    int    count = 0;

    if (IS_NPC(victim))
        return;

    if (!IS_NPC(ch)) {
        if (ch == victim) {
            if (victim->class > BARBARIAN) {
                gain_exp(victim, -victim->exp / (65 - victim->level / 4));
            }
            if (victim->class <= BARBARIAN) {
                gain_exp(victim, -victim->exp / (85 - victim->level / 4));
            }
        }
        return;
    }
    sprintf(buf_q, "corpse_count_%d", ch->pIndexData->vnum);
    qm = is_qmark(victim, buf_q);
    if (victim->in_room) {
        // for ( obj = victim->in_room->contents; obj; obj = obj->next_content )
        //	if ( is_name( victim->name, obj->short_descr ) )
        //		count ++;
        if (qm)
            count = qm->v0;
        else
            count = 0;

        if (count > 5) {
            count -= 2;
            victim->exp = victim->exp - 100 * count * count;
            send_to_char("你因為堆積屍體，天神減少你的經驗與生命作為處罰！\n\r", victim);
            victim->max_hit -= count * count;
            victim->max_hit = UMAX(victim->max_hit, 1);
            // if (ch->clan){
            //	ch->clan->score -=  500*ch->level ;
            //	ch->clan->bank -= UMAX( 0, 500*ch->level * 2 );
            //	ch->pcdata->clanexp -= 500*ch->level;
            // }
        } else {
            // 20060722 modify by jye
            // 新增EXP_PER_LEVEL ppl_class變數
            if (victim->exp <= EXP_PER_LEVEL(victim->level - 1, victim->class) * victim->level)
                return;
            if (victim->class > BARBARIAN) {
                gain_exp(victim, -victim->exp / (60 - victim->level / 4));
            }
            if (victim->class <= BARBARIAN) {
                gain_exp(victim, -victim->exp / (80 - victim->level / 4));
            }
            // 20060722 modify by jye
            // 新增EXP_PER_LEVEL ppl_class變數
            victim->exp = UMAX(victim->exp, EXP_PER_LEVEL(victim->level - 1, victim->class) * victim->level);
        }
    }
}

CHAR_DATA *check_mob_ai(CHAR_DATA *ch) {
    if (!IS_NPC(ch))
        return ch->fighting;
    if (IS_SET(ch->special, MOBAI_LV3)) {
        SET_BIT(ch->act, ACT_MOB_AI);
        return mob_ai_lv3(ch);
    } else if (IS_SET(ch->special, MOBAI_LV2)) {
        SET_BIT(ch->act, ACT_MOB_AI);
        return mob_ai_lv2(ch);
    } else
        return mob_ai_lv1(ch);
}

CHAR_DATA *mob_ai_lv3(CHAR_DATA *ch) {
    CHAR_DATA *victim = ch->fighting, *rch;
    int        class_count[MAX_CLASS];
    int        i;
    // int easy_kill;

    for (i = 0; i < MAX_CLASS; ++i) {
        class_count[i] = 0;
    }
    if (victim->level > 35 && ch->level > 45 &&
        (number_percent() < IS_SET(ch->act, ACT_MOB_AI) ? ch->level : ch->level - 45)) {
        for (rch = ch->in_room->people; rch; rch = rch->next_in_room) {
            if (rch->fighting == ch && !rch->deleted) {
                if (!can_see(ch, rch))
                    continue;
                if (ch->hitroll < -GET_DODGE(rch) / 8)
                    continue;
                if (rch->class == MONK || rch->class == WARLORD || rch->class == ALCHEMIST || rch->class == MARTIALMAN) {
                    victim = rch;
                    // return a void value?? fix by neversay on 2004/June/22
                    // return;
                    break;
                }
                if (rch->class == SAVANT || rch->class == BISHOP)
                    victim = rch;

                if (rch->class != SHAMAN && number_bits(3) == 1) {
                    victim = rch;
                    break;
                }
            }
        }
    }
    return victim;
}

CHAR_DATA *mob_ai_lv2(CHAR_DATA *ch) {
    CHAR_DATA *victim = ch->fighting, *rch;
    int        type;
    int        min;
    if (ch && ch->class && (IS_SET(ch->class, 16) || IS_SET(ch->class, 2)))
        type = 0; // mage
    else
        type = 1; // warrior
    switch (type) {
    case 1:
        min = ch->fighting->hit * 5 - GET_AC(ch->fighting) * 3;
        if (victim->level > 35 && ch->level > 45 &&
            (number_percent() < IS_SET(ch->act, ACT_MOB_AI) ? ch->level : ch->level - 45)) {
            for (rch = ch->in_room->people; rch; rch = rch->next_in_room) {
                if (rch->fighting == ch && !rch->deleted) {
                    if (!can_see(ch, rch))
                        continue;
                    if (ch->hitroll < -GET_DODGE(rch) / 8)
                        continue;

                    if (rch->hit * 5 - GET_AC(rch) * 3 < min) {
                        min    = rch->hit * 5 - GET_AC(rch) * 3;
                        victim = rch;
                    }
                }
            }
        }
        break;

    case 0:
        min = victim->saving_throw[0] * -3 + victim->hit;
        if (victim->level > 35 && ch->level > 45 &&
            (number_percent() < IS_SET(ch->act, ACT_MOB_AI) ? ch->level : ch->level - 45)) {
            for (rch = ch->in_room->people; rch; rch = rch->next_in_room) {
                if (rch->fighting == ch && !rch->deleted) {
                    if (!can_see(ch, rch))
                        continue;
                    if (rch->saving_throw[0] * -3 + rch->hit < min) {
                        min    = rch->saving_throw[0] * -3 + rch->hit;
                        victim = rch;
                    }
                }
            }
        }
        break;
    }
    return victim;
}

CHAR_DATA *mob_ai_lv1(CHAR_DATA *ch) {
    CHAR_DATA *victim = ch->fighting, *rch;
    if (victim->level > 35 && ch->level > 45 &&
        (number_percent() < IS_SET(ch->act, ACT_MOB_AI) ? ch->level : ch->level - 45 + (victim->class == SHAMAN ? 20 : 0))) {
        for (rch = ch->in_room->people; rch; rch = rch->next_in_room) {
            if (rch->fighting == ch && !rch->deleted) {
                if (!rch)
                    continue;
                if (!rch->class)
                    continue;
                if (!can_see(ch, rch))
                    continue;
                if (ch->hitroll < -GET_DODGE(rch) / 8)
                    continue;
                if (rch->class == SAVANT || rch->class == BISHOP)
                    victim = rch;
                else if (rch->class == WIZARD || rch->class == ALCHEMIST)
                    victim = rch;
                else if (rch->class == LORD)
                    victim = rch;
                else if (rch->class == CLERIC || rch->class == MAGE)
                    victim = rch;

                if (rch->class != SHAMAN && number_bits(2) == 1) {
                    victim = rch;
                    break;
                }
            }
        }
    }
    return victim;
}

/*have registered or have license or is vampire*/
// licensed: Core Engine function: licensed - Main C routine handling game mechanics.
// licensed: 核心引擎函式：licensed - 處理遊戲底層機制的核心 C 語言子程序。
bool licensed(CHAR_DATA *ch) {
    OBJ_DATA *obj;

    if (ch->race == race_lookup("Vampire"))
        return TRUE;

    if (!IS_SET(ch->act, PLR_REGISTER))
        return FALSE;

    /*
     * If already fighting then we just jump out.
     */
    if (ch->fighting)
        return TRUE;

    for (obj = ch->carrying; obj; obj = obj->next_content) {
        if (obj->pIndexData->vnum == OBJ_VNUM_LICENSE)
            return TRUE;
    }

    return FALSE;
}

/*have registered or is vampire*/
bool registered(CHAR_DATA *ch, CHAR_DATA *victim) /*{{{*/
{
    if (ch->in_room == victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_PK))
        return TRUE;
    if (IS_SET(victim->act, PLR_REGISTER))
        return TRUE;

    if (ch->race == race_lookup("Vampire"))
        return TRUE;

    return FALSE;

} /*}}}*/

/* This code is for PC's who polymorph into dragons.
 * Yeah I know this is specialized code, but this is fun.  :)
 * Breathe on friend and enemy alike.
 * -Kahn
 */

// pc_breathe: Core Engine function: pc_breathe - Main C routine handling game mechanics.
// pc_breathe: 核心引擎函式：pc_breathe - 處理遊戲底層機制的核心 C 語言子程序。
void pc_breathe(CHAR_DATA *ch) {
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    int        sn;

    send_to_char("You feel the urge to burp!\n\r", ch);
    act("$n belches!", ch, NULL, NULL, TO_ROOM);
    for (victim = ch->in_room->people; victim; victim = victim_next) {
        victim_next = victim->next_in_room;
        if (victim->deleted)
            continue;

        if (victim == ch)
            continue;

        if (!IS_NPC(victim) && (!registered(ch, victim) || (!licensed(ch) && victim->race != race_lookup("Vampire"))))
            continue;

        sn = skill_lookup("fire breath");
        (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim);
    }

    return;
}

/* This code is for PC's who polymorph into harpies.
 * Yeah I know this is specialized code, but this is fun.  :)
 * Scream into the ears of enemy and friend alike.
 * -Kahn
 */

// pc_screech: Core Engine function: pc_screech - Main C routine handling game mechanics.
// pc_screech: 核心引擎函式：pc_screech - 處理遊戲底層機制的核心 C 語言子程序。
void pc_screech(CHAR_DATA *ch) {
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    int        sn;

    send_to_char("You feel the urge to scream!\n\r", ch);
    interpret(ch, "scream");
    for (victim = ch->in_room->people; victim; victim = victim_next) {
        victim_next = victim->next_in_room;
        if (victim->deleted)
            continue;

        if (victim == ch)
            continue;

        if (!IS_NPC(victim) && (!registered(ch, victim) || (!licensed(ch) && victim->race != race_lookup("Vampire"))))
            continue;

        act("Your ears pop from $n's scream.  Ouch!", ch, NULL, victim,
            TO_VICT);
        sn = skill_lookup("agitation");
        (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim);
    }

    return;
}

// pc_spit: Core Engine function: pc_spit - Main C routine handling game mechanics.
// pc_spit: 核心引擎函式：pc_spit - 處理遊戲底層機制的核心 C 語言子程序。
void pc_spit(CHAR_DATA *ch) {
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    int        sn;

    send_to_char("You feel the urge to spit!\n\r", ch);
    act("$n spews vitriol!", ch, NULL, NULL, TO_ROOM);
    for (victim = ch->in_room->people; victim; victim = victim_next) {
        victim_next = victim->next_in_room;
        if (victim->deleted)
            continue;

        if (victim == ch)
            continue;

        if (!IS_NPC(victim) && (!registered(ch, victim) || (!licensed(ch) && victim->race != race_lookup("Vampire"))))
            continue;

        act("You are splattered with $n's vitriol.  Ouch!", ch, NULL, victim,
            TO_VICT);
        sn = skill_lookup("poison");
        (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim);

        ////damage( ch, victim, number_range( 1, ch->level ), gsn_poison_weapon, WEAR_NONE );
        damage(ch, victim, number_range(1, ch->level), gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL);
    }

    return;
}

// check_race_special: Combat logic: check_race_special - Applies combat calculations, checks, or damage formulas.
// check_race_special: 戰鬥邏輯：check_race_special - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_race_special(CHAR_DATA *ch) {
    return FALSE;
    if (ch->race == race_lookup("Dragon")) {
        if (number_percent() < ch->level) {
            pc_breathe(ch);
            return TRUE;
        }
    }

    if (ch->race == race_lookup("Harpy")) {
        /*
        if ( number_percent( ) < ch->level )
        {
            pc_screech( ch );
            return TRUE;
        }
        */
    }

    if (ch->race == race_lookup("Arachnid") || ch->race == race_lookup("Snake")) {
        if (number_percent() < ch->level) {
            pc_spit(ch);
            return TRUE;
        }
    }

    return FALSE;
}

// check_wimpout: Combat logic: check_wimpout - Applies combat calculations, checks, or damage formulas.
// check_wimpout: 戰鬥邏輯：check_wimpout - 進行戰鬥數值判定、安全檢查或傷害公式運算。
void check_wimpout(CHAR_DATA *ch, CHAR_DATA *victim) {
    //----恐懼狀態會自動逃跑

    if (IS_AFFECTED(victim, AFF_FEAR)) {
        do_flee(victim, "");
    }

    if (IS_NPC(victim)) {
        if ((IS_SET(victim->act, ACT_WIMPY) && number_bits(1) == 0 && victim->hit < victim->max_hit / 2 && victim->wait == 0) || ((IS_AFFECTED(victim, AFF_CHARM) || IS_AFFECTED(victim, AFF_SUMMONED) || victim->clan) && victim->master && victim->in_room && victim->master->in_room != victim->in_room)) {
            do_flee(victim, "");
        }
    }
    // modified by coconet for let ppl in stun can not wimpy
    else {
        if (victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait < 3) {
            do_flee(victim, "");
        }
    }
}

/* Reflexion skill by Amenda, 01/2/3 */
/* Managed by Chris,    05/7/2 */

// check_reflexion: Combat logic: check_reflexion - Applies combat calculations, checks, or damage formulas.
// check_reflexion: 戰鬥邏輯：check_reflexion - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_reflexion(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam, int dexdiff) {
    int chance = 0;

    // 反向成立的情況下在 safe room 不會作用 2021/12/22
    if (is_safe(ch, victim))
        return FALSE;

    // 此處永遠不會跑到 gsn_reposte , reposte 修改為 DMG_ADJUST 於 fight_damage 內阻擋 2021/11/16
    // if ( ( dt >= TYPE_HIT || dt == gsn_frenzy ) && dt != gsn_reposte && dt != 1015)
    if ((dt >= TYPE_HIT || dt == gsn_frenzy) && dt != 1015) {
        int ref_cost;
        int nReflexSklv = get_skill_level(victim, gsn_reflexion);
        /* npc 判定沒有意義, 且 reflexion sklv 應判定 victim 而非 ch 2022/12/30
        ref_cost = ( IS_NPC( victim ) ) ? 0 : dam / 8 + 1;
        ref_cost = UMAX(10, ref_cost - get_skill_level(ch, gsn_reflexion)*4); */
        // sklv 會確實顯著影響到 mv 扣除 2022/12/30
        ref_cost = dam / (10 + nReflexSklv);
        ref_cost = UMAX(10 - nReflexSklv, ref_cost);

        // ch hp <= 0 時 mv cost 除 2 ( 死前攻擊 )2022/12/30
        if (ch->hit <= 0)
            ref_cost = ref_cost / 2;

        if ((victim->move >= ref_cost) && ((!IS_NPC(victim) && get_skill_percent(victim, gsn_reflexion)) || (IS_NPC(victim) && IS_SET(victim->act, ACT_REFLEXION)))) {
            // int nReflexSklv = get_skill_level(victim, gsn_reflexion);

            // beholder inborn 5 以上有較高的 reflexion
            if (IS_SET(race_table[victim->race].race_abilities, RACE_BEHOLDER)) {
                chance += (get_curr_dex(victim) * (3 + get_skill_level(victim, gsn_inborn_ability))) / 10;
            } else {
                // 沒有 wp2 的情況 reflexion 的 dex 加成下降為 80%
                if (!get_eq_char(victim, WEAR_WIELD_2))
                    chance += (get_curr_dex(victim) * 4) / 5;
                else
                    chance += get_curr_dex(victim);
            }

            chance += nReflexSklv * (nReflexSklv + 3);

            // 每點 dex 差影響 3%
            chance -= dexdiff * 3;

            if (!IS_NPC(victim)) {
                // 非 autoc_dodge 的情況下
                if (victim->pcdata->autocombat != AUTOC_DODGE) {
                    // cost 變成 1.5 倍 2021/11/16
                    ref_cost = ref_cost * 1.5;
                } else {
                    // 加入 autododge 的 sklv 影響 2022/12/30
                    chance += 2 + get_skill_level(victim, gsn_autododge) * 3;
                }
                // cost 不夠的情況下就失敗
                if (victim->move < ref_cost)
                    return FALSE;
            }

            // stone form 狀況下chance 減半 2022/12/05
            if (is_affected(victim, gsn_stone_form)) {
                chance /= 2;
            }

            if (number_percent() < chance) {
                if (!IS_NPC(victim)) {
                    victim->move -= ref_cost;
                }
                if (dt != 1014) {
                    return TRUE;
                } else if (number_percent() * 3 < chance * 2) {
                    act((is_chinese(ch, victim)) ? "$N 運用技巧接住了你的箭矢 !" : "$N receives your shot.", ch, NULL, victim, TO_CHAR);
                    act((is_chinese(victim, ch)) ? "你運用技巧接住了 $n 的箭矢 !" : "You receive $n's shot.", ch, NULL, victim, TO_VICT);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

// 根據攻擊者的Level對傷害進行調降，以延長高等級的戰鬥
// check_damage_adjust: Combat logic: check_damage_adjust - Applies combat calculations, checks, or damage formulas.
// check_damage_adjust: 戰鬥邏輯：check_damage_adjust - 進行戰鬥數值判定、安全檢查或傷害公式運算。
int check_damage_adjust(CHAR_DATA *ch, int dam) {
    if (ch->level > 29) {
        dam = UMAX(1, dam - (dam / 4) + 1);
    }

    if (ch->level > 49) {
        dam = UMAX(1, dam - (dam / 5) + 1);
    }

    // dam = UMAX(1, dam - ( dam / 4) + 1);

    // Marked by Razgriz   20060605
    // 維持原本的傷害輸出
    // if ( IS_NPC(ch) )
    //{
    dam = dam * 15 / 16;
    //}

    return dam;
}

// Spell Fury
// check_spellfury: Combat logic: check_spellfury - Applies combat calculations, checks, or damage formulas.
// check_spellfury: 戰鬥邏輯：check_spellfury - 進行戰鬥數值判定、安全檢查或傷害公式運算。
int check_spellfury(CHAR_DATA *ch, CHAR_DATA *victim, int dam) {
    if (IS_NPC(ch) && IS_SET(ch->special, SPELL_FURY)) {
        if (number_percent() < ch->level + 10)
            dam += dam / 4;
        if (IS_SET(ch->special, EXPERT) && number_percent() < ch->level / 3)
            dam += dam / 2;
    } else {
        int fury  = 0;
        int nSklv = get_skill_level(ch, gsn_spell_fury);
        int nPerc = get_skill_percent(ch, gsn_spell_fury);
        int nInt  = get_curr_int(ch);
        // Beholder 天生有fury add at 19/2/2
        int nBehSklv     = 0;
        int nBehFuryPerc = get_skill_percent(ch, gsn_inborn_ability);

        //  int cint = get_curr_int(ch)+nSklv+nSklv/2+nSklv/3+(nSklv/6)*3;
        //  OLD SPELL FURY
        //      SKLV    0    1    2    3    4    5    6
        //              0    1    3    5    7    8   14
        //  引發 MIN   25   26   29   30   32   33   39
        //  威力 MIN 1.07  1.1  1.2  1.3  1.4  1.5  1.6
        //  引發 MED   10 10.1 11.7   12 12.5   13   15
        //  威力 MED 1.10            1.5            2.0
        //  威力 MAX 1.17         1.8         2.6

        //  MIN        0    1    2    3     4     5     6
        //  MIN     10.0 20.0 30.0 40.0  50.0  60.0  70.0
        //  MAX     20.0 32.5 50.0 72.5 100.0 132.5 170.0

        // Beholder 天生有fury add at 19/2/2
        if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
            nBehSklv = get_skill_level(ch, gsn_inborn_ability);

            if (nPerc > nBehFuryPerc / 3) {
                // nPerc = nPerc + nBehFuryPerc/9 + nBehSklv * 6;
                //  ib lv 6 99% , fury percent mag = 89% , wiz = 124%
                nPerc = 2 + nPerc + nBehFuryPerc / 9 + nBehSklv * 2;
            } else {
                nPerc = nBehFuryPerc / 9 + nBehSklv * 8;
            }
            if ((nBehSklv - 1) > nSklv)
                nSklv = nBehSklv - 1;
        }

        fury = 0;
        if (nPerc > 0 && number_percent() < nInt / 2 + nBehSklv * 2 + nSklv * (nSklv + 14) / 4) {
            // fury = number_range(10 + nSklv * 10, 20 + nSklv * 10 + 10 * nSklv * nSklv / 4);
            //  降低fury最大值倍數 2020/09/18
            //   old 60% sklv 6  1.51 ~ 2.25 , beh  1.64 ~ 2.56
            //   old 99% sklv 6  1.7 ~ 2.7 , beh  1.86 ~  3.17
            //   new 60% sklv 6  1.55 ~ 2 ,  beh  1.7 ~ 2.26
            //   new 99% sklv 6  1.75 ~ 2.36 , beh  1.87 ~ 2.6
            fury = number_range(15 + nSklv * 10, 37 + nSklv * 12 + 3 * nSklv * nSklv / 4);
            dam  = dam * (10000 + fury * (nPerc * 2 + 102) / 3) / 10000;

            if (fury * (nPerc * 2 + 102) / 3 < 10000) {
                if (is_chinese(ch, victim))
                    send_to_char("你的魔法能量造成非比尋常的傷害！\n\r", ch);
                else
                    send_to_char("Your spell power goes into fury!\n\r", ch);
            } else {
                if (is_chinese(ch, victim))
                    send_to_char("你的魔法能量造成超乎想像的傷害！\n\r", ch);
                else
                    send_to_char("Your spell power goes into extreme fury!\n\r", ch);
            }
        }
        /* OLD VERSION *
        int cint = get_curr_int(ch) + nSklv + nSklv/2 + nSklv/3 + (nSklv/6)*3;

        if( number_percent() < (nPerc * cint) / 100)
        {
                dam += dam*nSklv/10 + (nSklv == 0 ? dam/15 : 0 );
            fury = 1;
        }

        if( number_percent() < (nPerc * cint) / 250)
        {
            dam += dam*nSklv/6 + (nSklv == 0 ? dam/10 : 0);
            ++fury;
        }

        switch( fury )
        {
            case 1:
                if( is_chinese(ch, victim) )
                {
                    send_to_char("你的魔法能量造成非比尋常的傷害！\n\r", ch);
                }
                else
                {
                    send_to_char("Your spell power goes into fury!\n\r", ch);
                }
                break;

            case 2:
                if( is_chinese(ch, victim) )
                {
                    send_to_char("你的魔法能量造成超乎想像的傷害！\n\r", ch);
                }
                else
                {
                    send_to_char("Your spell power goes into extreme fury!\n\r", ch);
                }
                break;
        };
        **/
    }
    return dam;
}

// 計算偷襲的加成與攻擊招喚物的後果
// check_dirty_raid: Combat logic: check_dirty_raid - Applies combat calculations, checks, or damage formulas.
// check_dirty_raid: 戰鬥邏輯：check_dirty_raid - 進行戰鬥數值判定、安全檢查或傷害公式運算。
int check_dirty_raid(CHAR_DATA *ch, CHAR_DATA *victim, int dam) {
    if (victim->position >= POS_SLEEPING) {
        if (!victim->fighting)
            set_fighting(victim, ch);

        if (!ch->fighting)
            set_fighting(ch, victim);

        if (IS_NPC(ch) && IS_NPC(victim) && victim->master && ch->in_room && victim->master != ch && victim->master->in_room == ch->in_room && number_bits(2) == 0) {
            stop_fighting(ch, FALSE);
            set_fighting(ch, victim->master);
            return -1;
        }
    } else {
        dam = dam + dam / 2;
    }

    // 攻擊自己的寵物或招喚物
    if (victim->master == ch) {
        stop_follower(victim);
        // REMOVE_BIT( victim->affected_by, AFF_SUMMONED );  開戰移除 aff_summoned會造成sor mob許多判定上問題  19/11/27
        REMOVE_BIT(victim->affected_by, AFF_CHARM);
    }
    return dam;
}

// check_can_do_hit: Combat logic: check_can_do_hit - Applies combat calculations, checks, or damage formulas.
// check_can_do_hit: 戰鬥邏輯：check_can_do_hit - 進行戰鬥數值判定、安全檢查或傷害公式運算。
bool check_can_do_hit(CHAR_DATA *ch, CHAR_DATA *victim) {
    if (!victim || (victim == NULL) || victim->deleted)
        return FALSE;

    if (ch->deleted)
        return FALSE;

    if (ch->position == POS_PARALYZED)
        return FALSE;

    if (is_affected(ch, gsn_stone_form))
        return FALSE;

    if (!IS_NPC(ch)) {
        if (ch->pcdata->autocombat == AUTOC_STOP)
            return FALSE;
        if (ch->pcdata->autocombat == AUTOC_DODGE)
            return FALSE;
        if (ch->pcdata->autocombat == AUTOC_PARRY)
            return FALSE;
        if (ch->pcdata->autocombat == AUTOC_CONCENTRATION)
            return FALSE;
        if (ch->pcdata->autocombat == AUTOC_INSTANT)
            return FALSE;
    }

    if (!IS_NPC(ch) && ch->position == POS_RESTING)
        return FALSE;

    if (ch->wait > 0)
        return FALSE;

    return TRUE;
}
