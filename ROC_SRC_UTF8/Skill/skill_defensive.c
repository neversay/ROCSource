/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.			   *
 *									   *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and	   *
 *  Mitchell Tse.							   *
 *									   *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.	   *
 *									   *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in	   *
 *  'license.txt', as well as the Envy license in 'license.nvy'.	   *
 *  In particular, you may not remove either of these copyright notices.   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
/* $Id: skill.c 916 2005-05-10 03:56:16Z zcecil $ */
#include "skill.h"

extern void check_alignment_eq(CHAR_DATA *ch);
extern bool IsSorGateMobEarth(int MOB_INDEX);
// bool psionic_check(CHAR_DATA *ch, int sn );

void do_defensive(CHAR_DATA *ch, char *argument) {
    AFFECT_DATA af;
    int         bonus = 0;
    int         delay;
    char        buf[BUF_STRING_LENGTH];
    QMARK      *q;

    if (is_affected(ch, gsn_defensive))
        return;

    if (!IS_NPC(ch)) {
        if (number_percent() > get_skill_percent(ch, gsn_defensive)) {
            send_to_char("你失敗了。\n\r", ch);
            return;
        }
        if (ch->move < 50 || ch->hit < 50) {
            send_to_char("你的體力不夠，無法使用護身罡氣。\n\r", ch);
            return;
        }
        if ((q = is_qmark(ch, "skill_defensive_mark"))) {
            if (q->timer > 36) {
                send_to_char("你還需要一天半左右的時間才能回復體力。\n\r", ch);
            } else if (q->timer > 24) {
                send_to_char("你還需要一天左右的時間才能回復體力。\n\r", ch);
            } else if (q->timer > 12) {
                send_to_char("你還需要半天左右的時間才能回復體力。\n\r", ch);
            } else
                send_to_char("你還沒從之前的疲累中回復過來。\n\r", ch);
            // Modified by Keric for KUSO!
            send_to_char("孩子，你累了。要不要來碗清湯？\n\r", ch);
            return;
        }
    }

    act("你集中全力，運起護身罡氣。", ch, NULL, NULL, TO_CHAR);
    act("$n集中全力，運起護身罡氣。", ch, NULL, NULL, TO_ROOM);
    bonus = ch->level / 7;

    af.type      = gsn_defensive;
    af.duration  = 3;
    af.location  = APPLY_DODGE;
    af.modifier  = bonus * 10 + bonus * get_skill_level(ch, gsn_defensive) * 2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    // delay = 69 - (bonus * 2);
    delay = abs(ch->level - 32);
    delay = 81 - (delay * 3);
    sprintf(buf, "self skill_defensive_mark %d 0 0 0 0 1", delay - get_skill_level(ch, gsn_defensive) * 6);
    do_add_qmark(ch, buf);

    /* Modified by Keric for PDR enhancement 2005/2/13 */
    af.type      = gsn_defensive;
    af.duration  = 3;
    af.location  = APPLY_RDAMAGE;
    af.modifier  = bonus * ((get_curr_wis(ch) + get_curr_con(ch)) / (7 - get_skill_level(ch, gsn_defensive)));
    af.bitvector = 0;
    affect_to_char(ch, &af);
}

void do_rescue(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    CHAR_DATA *fch = NULL;
    char       arg[MAX_INPUT_LENGTH];
    int        count;
    int        sklv = get_skill_level(ch, gsn_rescue); // 2023/01/12
    char       buf[BUF_STRING_LENGTH];                 // 2023/01/12
    int        rescue_round = 0;                       // 2023/01/12
    bool       IsProcess    = FALSE;                   // 2023/01/12

    /* Don't allow charmed mobs to do this, check player's level */
    if (IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM)) {
        send_to_char("什麼？\n\r", ch);
        return;
    }

    if (is_affected(ch, gsn_stone_form)) {
        send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
        return;
    }

    if (is_affected(ch, gsn_berserk)) {
        send_to_char("你正殺得眼紅！\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("要解救誰?\n\r", ch);
        return;
    }

    if (!(victim = get_char_room(ch, arg))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (victim == ch) {
        send_to_char("想自救？試試 flee 指令吧！\n\r", ch);
        return;
    }

    if (ch->fighting == victim) {
        send_to_char("一刀讓他解脫比較快.\n\r", ch);
        return;
    }

    if (!victim->fighting) {
        send_to_char("你只能解救戰鬥中的目標.\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && !is_same_group(ch, victim) && ch->clan != victim->clan) {
        send_to_char("你幹嘛這麼多事？\n\r", ch);
        return;
    }

    if (!check_blind(ch))
        return;

    // rescue 成功時給予不被 change 的 round 數 2023/01/12
    if (sklv > 0)
        rescue_round += 1;
    if (sklv > 2)
        rescue_round += 1;
    if (sklv > 5)
        rescue_round += 1;
    if (sklv > 6)
        rescue_round += 1;

    count = 0;
    for (fch = victim->in_room->people; fch; fch = fch->next_in_room) {
        if (fch->deleted)
            continue;
        if (fch->fighting == victim) {
            if (!IS_NPC(ch) && ch->move < 200 - sklv * 20) {
                send_to_char("你太累了。\n\r", ch);
                return;
            }

            if (!IS_NPC(ch))
                ch->move = ch->move - 200 + sklv * 20;

            // 判定是否有執行 rescue 的動作 2023/01/12
            IsProcess = TRUE;

            if (!IS_NPC(ch) && number_percent() > (get_skill_percent(ch, gsn_rescue) - 20) + sklv + sklv * sklv / 2) {
                if (!is_chinese(ch, victim)) {
                    act("You tried to rescue $N, but FAILED.", ch, NULL, victim, TO_CHAR);
                    act("$n tried to rescue $N, but FAILED.", ch, NULL, victim, TO_NOTVICT);
                } else {
                    act("你試著解救$N，可惜失敗了。", ch, NULL, victim, TO_CHAR);
                    act("$n試著解救$N，可惜失敗了。", ch, NULL, victim, TO_NOTVICT);
                }
                if (!is_chinese(victim, ch))
                    act(HIG "$n tried to rescue you, but FAILED." NOR, ch, NULL, victim, TO_VICT);
                else
                    act(HIG "$n試著解救你，可惜失敗了。" NOR, ch, NULL, victim, TO_VICT);

                continue;
            }
            if (IS_NPC(ch) && number_percent() > UMIN(85, GetGeneralMobSklv(ch) * 12 + ch->level / 2)) {
                if (!is_chinese(ch, victim)) {
                    act("You tried to rescue $N, but FAILED.", ch, NULL, victim, TO_CHAR);
                    act("$n tried to rescue $N, but FAILED.", ch, NULL, victim, TO_NOTVICT);
                } else {
                    act("你試著解救$N，可惜失敗了。", ch, NULL, victim, TO_CHAR);
                    act("$n試著解救$N，可惜失敗了。", ch, NULL, victim, TO_NOTVICT);
                }
                if (!is_chinese(victim, ch))
                    act(HIG "$n tried to rescue you, but FAILED." NOR, ch, NULL, victim, TO_VICT);
                else
                    act(HIG "$n試著解救你，可惜失敗了。" NOR, ch, NULL, victim, TO_VICT);

                continue;
            }

            if (!is_chinese(ch, victim)) {
                act(HIG "You rescue $N!" NOR, ch, NULL, victim, TO_CHAR);
                act(HIG "$n rescue $N!" NOR, ch, NULL, victim, TO_NOTVICT);
            } else {
                act(HIG "你解救了 $N！" NOR, ch, NULL, victim, TO_CHAR);
                act(HIG "$n 解救了 $N！" NOR, ch, NULL, victim, TO_NOTVICT);
            }

            if (!is_chinese(victim, ch))
                act(HIG "$n rescue you!" NOR, ch, NULL, victim, TO_VICT);
            else
                act(HIG "$n 解救了你！" NOR, ch, NULL, victim, TO_VICT);

            stop_fighting(fch, FALSE);
            stop_fighting(ch, FALSE);
            set_fighting(fch, ch);
            set_fighting(ch, fch);

            /*   2023/01/12
             *   rescue 成功時, 給予攻擊 victim 的人(fch) qmark,
             *   使該角色在 v0 round 前無法使用 do_change, v0 小於 0 時, 則可以再次使用 change
             *   該 qmark 於戰鬥後或 timer 到期消滅, 其間無法再次對目標加入 qmark
             */
            if (!(is_qmark(fch, "skill_rescue_round")) && rescue_round > 0) {
                sprintf(buf, "self skill_rescue_round 2 %d 0 0 1 0", rescue_round);
                send_to_char("你被人盯上了！短期間內無法轉換戰鬥目標。\n\r", fch);
                do_add_qmark(fch, buf);
            }
        }
    }

    // 確實有執行過 rescue 的動作時才會有 stun 2023/01/12
    if (IsProcess)
        WAIT_STATE(ch, 14 - sklv * 2);
    return;
}

void do_disarm(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];
    int        percent;

    /* Don't allow charmed mobiles to do this, check player's level */
    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && ch->pcdata->learned[gsn_disarm] == 0)) {
        send_to_char("你不懂得如何擊落別人的武器.\n\r", ch);
        return;
    }

    if (!get_eq_char(ch, WEAR_WIELD) && !get_eq_char(ch, WEAR_WIELD_2) && !get_eq_char(ch, WEAR_TWO_HAND)) {
        send_to_char("你得先拿把武器才行.\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你並沒有在戰鬥.\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    victim = ch->fighting;

    if (arg[0] != '\0') {
        if (!(victim = get_char_room(ch, arg))) {
            send_to_char("這裡沒有這個人.\n\r", ch);
            return;
        }
    }

    if (victim->fighting != ch && ch->fighting != victim) {
        act("$E 並沒有在和你戰鬥！", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (!get_eq_char(victim, WEAR_WIELD) && !get_eq_char(victim, WEAR_WIELD_2) && !get_eq_char(victim, WEAR_TWO_HAND)) {
        send_to_char("你的目標並沒有裝備武器.\n\r", ch);
        return;
    }

    percent = number_percent() + (victim->level - ch->level) * 2 + get_skill_level(ch, gsn_disarm) * 2;
    percent = (percent * weakness_percent(victim, gsn_disarm)) / 100;
    if (!get_eq_char(ch, WEAR_WIELD) && !get_eq_char(ch, WEAR_TWO_HAND))
        percent *= 2; /* 1/2 as likely with only 2nd weapon */
    if (IS_NPC(ch)) {
        if (percent < number_percent()) {
            disarm(ch, victim);
            WAIT_STATE(victim, skill_table[gsn_disarm].beats);
        }
    } else if (percent < get_skill_percent(ch, gsn_disarm) * 2 / 3) {
        disarm(ch, victim);
        WAIT_STATE(victim, skill_table[gsn_disarm].beats);
    } else
        send_to_char("你失敗了.\n\r", ch);
    WAIT_STATE(ch, skill_table[gsn_disarm].beats);

    mprog_hurt_trigger(victim, ch, gsn_disarm);

    return;
}

void do_barehand(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];
    int        percent;

    /* Don't allow charmed mobiles to do this, check player's level */
    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && ch->pcdata->learned[gsn_barehand] == 0)) {
        send_to_char("你不懂得如何空手入白刃.\n\r", ch);
        return;
    }

    if (get_eq_char(ch, WEAR_WIELD) || get_eq_char(ch, WEAR_WIELD_2)) {
        send_to_char("你必須空手.\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你並沒有在戰鬥.\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    victim = ch->fighting;

    if (arg[0] != '\0') {
        if (!(victim = get_char_room(ch, arg))) {
            send_to_char("這裡沒有這個人.\n\r", ch);
            return;
        }
    }

    if (victim->fighting != ch && ch->fighting != victim) {
        act("$E 並沒有在和你戰鬥。", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (!get_eq_char(victim, WEAR_WIELD) && !get_eq_char(victim, WEAR_WIELD_2) && !get_eq_char(victim, WEAR_TWO_HAND)) {
        send_to_char("你的目標並沒有握著武器.\n\r", ch);
        return;
    }

    percent = number_percent() + victim->level - ch->level + get_skill_level(ch, gsn_barehand) + get_skill_level(ch, gsn_barehand) / 2;
    percent = (percent * weakness_percent(victim, gsn_barehand)) / 100;

    if (IS_NPC(ch) || percent < get_skill_percent(ch, gsn_barehand) * 2 / 3) {
        disarm(ch, victim);
        WAIT_STATE(victim, skill_table[gsn_barehand].beats);
    } else
        send_to_char("你失敗了.\n\r", ch);
    WAIT_STATE(ch, skill_table[gsn_barehand].beats);

    mprog_hurt_trigger(victim, ch, gsn_barehand);

    return;
}

void do_invoke(CHAR_DATA *ch, char *argument) {
    AFFECT_DATA af;
    int         sklv;
    char        arg[MAX_INPUT_LENGTH];
    char        qmark[MAX_INPUT_LENGTH];

    if (is_affected(ch, gsn_invoke)) {
        send_to_char("你身旁還有魔神呢！\n\r", ch);
        return;
    }
    one_argument(argument, arg);
    if (arg[0] == '\0') {
        sprintf(arg, "hrdr");
    }
    sprintf(qmark, "skill_invoke_%s", arg);
    sklv = get_skill_level(ch, gsn_invoke);

    if ((!IS_NPC(ch) && number_percent() > get_skill_percent(ch, gsn_invoke)) || (IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM))) {
        send_to_char("魔神們都沒空！\n\r", ch);
        ch->move = UMAX(0, ch->move - 50);
        ch->mana = UMAX(0, ch->mana - 100);
        return;
    }
    switch (arg[0]) {
    case 'h':
    case 'H':
        if (!str_cmp(arg, "hrdr")) {
            if (!IS_NPC(ch)) {
                if (ch->pcdata->condition[COND_DRUNK] > 30 - sklv * 2) {
                    send_to_char("喝那麼醉，不怕褻瀆神明嗎！\n\r", ch);
                    return;
                }
                if ((race_table[ch->race].thirst_mod && ch->pcdata->condition[COND_THIRST] < 16) || (race_table[ch->race].hunger_mod && ch->pcdata->condition[COND_FULL] < 16) || ch->move < ch->max_move / 2 || ch->move < 150 || ch->mana < 210) {
                    send_to_char("你沒有足夠的精力！\n\r", ch);
                    return;
                } else {
                    if (race_table[ch->race].thirst_mod)
                        ch->pcdata->condition[COND_THIRST] -= 16;
                    if (race_table[ch->race].hunger_mod)
                        ch->pcdata->condition[COND_FULL] -= 16;
                    ch->move -= 80 + 10 * get_skill_level(ch, gsn_invoke);
                    ch->mana -= 20 + 5 * get_skill_level(ch, gsn_invoke);
                }
            }

            if (!IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))
                ch->alignment = UMAX(-1000, ch->alignment - 50);

            af.type      = gsn_invoke;
            af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location  = APPLY_HITROLL;
            af.modifier  = ch->level / 10 * get_skill_level(ch, gsn_invoke);
            af.bitvector = 0;
            affect_to_char(ch, &af);

            af.type      = gsn_invoke;
            af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location  = APPLY_ADAMAGE;
            af.modifier  = ch->level / 4 * 4 * get_skill_level(ch, gsn_invoke);
            af.bitvector = 0;
            affect_to_char(ch, &af);

            af.type      = gsn_invoke;
            af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location  = APPLY_AMAGIC;
            af.modifier  = ch->level / 5 * 3 * get_skill_level(ch, gsn_invoke);
            af.bitvector = 0;
            affect_to_char(ch, &af);

            // 追加毒素耐性 2021/11/04
            af.type     = gsn_invoke;
            af.duration = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location = APPLY_SAVING_POISON;
            af.modifier = -100 - get_skill_level(ch, gsn_invoke) * 100;
            affect_to_char(ch, &af);

            send_to_char("你覺得你自己....變成神了！\n\r", ch);
            act("$n 全身突然發出凜烈的罡氣！", ch, NULL, NULL, TO_ROOM);
        }
        break;
    case 'd':
    case 'D':
        if (!str_cmp("def", arg)) {
            if (sklv < 5) {
                send_to_char("你的虔誠度還不夠，無法召喚神靈。\n\r", ch);
                return;
            }
            if (!IS_NPC(ch)) {
                if (ch->pcdata->condition[COND_DRUNK] > 30 - sklv * 2) {
                    send_to_char("喝那麼醉，不怕褻瀆神明嗎！\n\r", ch);
                    return;
                }
                if ((race_table[ch->race].thirst_mod && ch->pcdata->condition[COND_THIRST] < 16) || (race_table[ch->race].hunger_mod && ch->pcdata->condition[COND_FULL] < 16) || ch->move < ch->max_move / 2 || ch->move < 150 || ch->mana < 210) {
                    send_to_char("你沒有足夠的精力！\n\r", ch);
                    return;
                } else {
                    if (race_table[ch->race].thirst_mod)
                        ch->pcdata->condition[COND_THIRST] -= 16;
                    if (race_table[ch->race].hunger_mod)
                        ch->pcdata->condition[COND_FULL] -= 16;
                    ch->move -= 80 + 10 * get_skill_level(ch, gsn_invoke);
                    ch->mana -= 20 + 5 * get_skill_level(ch, gsn_invoke);
                }
            }
            if (!IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))
                ch->alignment = UMAX(-1000, ch->alignment - 50);

            af.type      = gsn_invoke;
            af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location  = APPLY_RDAMAGE;
            af.modifier  = ch->level / 4 + get_skill_level(ch, gsn_invoke) * 6;
            af.bitvector = 0;
            affect_to_char(ch, &af);
            af.location = APPLY_RMAGIC;
            af.modifier = ch->level / 4 + get_skill_level(ch, gsn_invoke) * 6;
            affect_to_char(ch, &af);
            // 追加毒素耐性 2021/11/04
            af.location = APPLY_SAVING_POISON;
            af.modifier = -100 - get_skill_level(ch, gsn_invoke) * 100;
            affect_to_char(ch, &af);

            if (sklv > 5) {
                af.location = APPLY_AC;
                af.modifier = -150;
                affect_to_char(ch, &af);
            }

            send_to_char("你的肌膚突然變得堅似鋼鐵！\n\r", ch);
            act("$n 全身突然發出凜烈的罡氣！", ch, NULL, NULL, TO_ROOM);
        }
        break;
    case 's':
    case 'S':
        if (!str_cmp("svs", arg)) {
            if (sklv < 1) {
                send_to_char("你的虔誠度還不夠，無法召喚神靈。\n\r", ch);
                return;
            }
            if (!IS_NPC(ch)) {
                if (ch->pcdata->condition[COND_DRUNK] > 30 - sklv * 2) {
                    send_to_char("喝那麼醉，不怕褻瀆神明嗎！\n\r", ch);
                    return;
                }
                if ((race_table[ch->race].thirst_mod && ch->pcdata->condition[COND_THIRST] < 16) || (race_table[ch->race].hunger_mod && ch->pcdata->condition[COND_FULL] < 16) || ch->move < ch->max_move / 2 || ch->move < 150 || ch->mana < 210) {
                    send_to_char("你沒有足夠的精力！\n\r", ch);
                    return;
                } else {
                    if (race_table[ch->race].thirst_mod)
                        ch->pcdata->condition[COND_THIRST] -= 16;
                    if (race_table[ch->race].hunger_mod)
                        ch->pcdata->condition[COND_FULL] -= 16;
                    ch->move -= 80 + 10 * get_skill_level(ch, gsn_invoke);
                    ch->mana -= 20 + 5 * get_skill_level(ch, gsn_invoke);
                }
            }
            if (!IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))
                ch->alignment = UMAX(-1000, ch->alignment - 50);

            af.type      = gsn_invoke;
            af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location  = APPLY_SAVING_SPELL;
            af.modifier  = -ch->level / 4 - get_skill_level(ch, gsn_invoke) * 2;
            af.bitvector = 0;
            affect_to_char(ch, &af);

            af.type      = gsn_invoke;
            af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location  = APPLY_SAVING_ELEMENT;
            af.modifier  = -ch->level / 4 - get_skill_level(ch, gsn_invoke) * 2;
            af.bitvector = 0;
            affect_to_char(ch, &af);

            // 追加毒素耐性 2021/11/04
            af.type     = gsn_invoke;
            af.duration = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
            af.location = APPLY_SAVING_POISON;
            af.modifier = -100 - get_skill_level(ch, gsn_invoke) * 100;
            affect_to_char(ch, &af);

            if (sklv > 5) {
                af.type      = gsn_invoke;
                af.duration  = 4 + get_skill_level(ch, gsn_invoke) * 3 / 2;
                af.location  = APPLY_SAVING_BREATH;
                af.modifier  = -ch->level / 4 - get_skill_level(ch, gsn_invoke) * 2;
                af.bitvector = 0;
                affect_to_char(ch, &af);
            }

            send_to_char("藉由天神的加護，你的周身籠罩一層光氣！\n\r", ch);
            act("$n 全身突然發出凜烈的罡氣！", ch, NULL, NULL, TO_ROOM);
        }
        break;
    }
    check_alignment_eq(ch);
    WAIT_STATE(ch, skill_table[gsn_invoke].beats);
}

void do_darkraid(CHAR_DATA *ch, char *argument) {
    AFFECT_DATA af;
    int         sklv      = get_skill_level(ch, gsn_dark_raid);
    int         BonusTime = sklv - 4;

    if (BonusTime < 0)
        BonusTime = 0;

    if (ch->move > 300 - sklv * sklv * 4)
        ch->move -= 300 - sklv * sklv * 4;
    else {
        send_to_char("[36m你沒有足夠的體力！[m\n\r", ch);
        return;
    }
    // sklv time bonus add at 2020/09/14
    if (time_info.hour > 4 + BonusTime && time_info.hour < 20 - BonusTime) {
        send_to_char("[36m天色比你想像中明亮，你失敗了！[m\n\r", ch);
        return;
    }

    if ((!IS_NPC(ch) && number_percent() > get_skill_percent(ch, gsn_dark_raid)) || (IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan))) {
        send_to_char("這門技巧對你而言太高深了！\n\r", ch);
        return;
    }

    send_to_char("[36m你依靠夜晚的寧靜提高專注力，為下一擊作足準備！[m\n\r", ch);

    affect_strip(ch, gsn_dark_raid);

    af.type      = gsn_dark_raid;
    af.duration  = 5 + sklv;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char(ch, &af);

    af.bitvector = AFF_DARK_RAID;
    af.location  = APPLY_HITROLL;
    // af.modifier  = ( ch->level + get_curr_dex( ch ) ) / 6 + sklv + sklv/6;
    af.modifier = 10 + get_curr_dex(ch) * sklv * sklv / 10;
    affect_to_char(ch, &af);

    // sklv > 2  dr bonus add at 2020/09/14
    if (sklv > 2) {
        af.bitvector = AFF_DARK_RAID;
        af.location  = APPLY_DAMROLL;
        af.modifier  = get_curr_dex(ch) * sklv * sklv / 15;
        affect_to_char(ch, &af);
    }

    WAIT_STATE(ch, 1);
}

/** protect 指令處理function
 * last update by zcecil 2004/07/05
 */
void do_protect(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];

    /* Don't allow charmed mobs to do this, check player's level */
    if (IS_NPC(ch) && !is_real_npc(ch)) {
        if (!IsSorGateMobEarth(ch->pIndexData->vnum)) {
            send_to_char("你失敗了。\n\r", ch);
            return;
        }
    }

    if (is_affected(ch, gsn_stone_form)) {
        send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
        return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        if (!ch->protecting) {
            send_to_char("要保護誰？\n\r", ch);
        } else {
            act("你正在保護的對象是$N。", ch, NULL, ch->protecting, TO_CHAR);
        }
        return;
    }

    if (!(victim = get_char_room(ch, arg))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (ch->protecting) {
        if (victim != ch->protecting) {
            stop_protecting(ch, ch->protecting);
        } else {
            act("你已經在保護$N了。", ch, NULL, ch->protecting, TO_CHAR);
            return;
        }
    }

    if (victim == ch) {
        return;
    }

    if ((!is_same_group(ch, victim))) {
        send_to_char("你幹嘛這麼多事！\n\r", ch);
        return;
    }

    add_protector(ch, victim);
}
