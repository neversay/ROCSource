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
//*  PROGRAM ID  : skill_slash.c                                  *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940607 jye                                    *
//*  PURPOSE     :         修正empslash噴裝後dr爆增之bug          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940608 jye                                    *
//*  PURPOSE     :         放出twin swords給ppl測bug              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         修改empslash 處於autoc stop時不扣move  *
//*              :             及沒拿武器卻扣move之bug            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940624 jye                                    *
//*  PURPOSE     :         修改twin在mob死後還會追打之bug         *
//*****************************************************************
/* $Id: skill.c 922 2005-05-10 07:30:11Z keric $ */
#include "skill.h"

// void check_alignment_eq( CHAR_DATA *ch );
// bool psionic_check(CHAR_DATA *ch, int sn );

void LizardmanSkill(CHAR_DATA *ch, CHAR_DATA *victim, int multi);

void do_empslash(CHAR_DATA *ch, CHAR_DATA *victim) {
    int       wp;
    OBJ_DATA *obj;
    int       dr, net_dr, chk_dr;
    int       hr, net_hr, chk_hr; // 2023/01/06
    int       ac, dodge;
    int       old_ac, old_dodge;
    int       wait = 0, descrip = 0;
    int       sklv = get_skill_level(ch, gsn_empslash);
    // int sklv_ber = get_skill_level(ch, gsn_berserk);
    // int level = UMAX( ch->level, 37 );
    // int vlevel = victim->level;  remove at 2023/01/06
    int limit_lv   = 0;
    int break_sklv = get_skill_level(ch, gsn_break_shield);
    int level_diff = 0;
    int str_ch     = 0;

    if (!can_see(ch, victim)) {
        send_to_char("你看不到目標。\n\r", ch);
        return;
    }

    if ((IS_NPC(ch) && !is_real_npc(ch)) || !check_skill(ch, gsn_empslash, number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
            send_to_char("好歹拿把武器吧？\n\r", ch);
            return;
        }
        wp = WEAR_WIELD_2;
    } else
        wp = WEAR_WIELD;

    if (!IS_NPC(ch)) {
        if (ch->move < 140 + sklv * 10) {
            send_to_char("你的移動力不夠。\n\r", ch);
            return;
        }
    }

    if (sklv < 6 && obj->value[3] != 1 && obj->value[3] != 3) {
        send_to_char("劍術是要用劍施展的。\n\r", ch);
        return;
    }

    // autoc stop 的判定沒有意義 2023/01/06
    // if( !IS_NPC(ch) && ch->pcdata->autocombat != AUTOC_STOP){
    if (!IS_NPC(ch)) {
        ch->move -= 100 + sklv * 25;
        // war 系有較高的 dr 加成 2023/01/06
        if (ch->pcdata->prime_attr == APPLY_STR)
            str_ch = 2;
    }

    do_add_qmark(victim, "self empslash_preset -1 1 0 0 1 0");

    descrip = number_range(0, 2);
    switch (descrip) {
    case 1:
        act(HIY "你霸氣一斬，挾千軍萬馬之勢朝敵人猛然攻去！" NOR, ch, NULL, NULL, TO_CHAR);
        act(HIY "$n" HIY "霸氣一斬，挾千軍萬馬之勢朝敵人猛然攻去！" NOR, ch, NULL, NULL, TO_ROOM);
        break;
    case 2:
        act(HIY "你輕晃劍身，劍光如數道急馳的流星朝敵人奔馳而去！" NOR, ch, NULL, NULL, TO_CHAR);
        act(HIY "$n" HIY "輕晃劍身，劍光如數道急馳的流星朝敵人奔馳而去！" NOR, ch, NULL, NULL, TO_ROOM);
        break;
    default:
        if (ch->sex == SEX_FEMALE) {
            act(HIY "妳凝神一閃，瞬時無情地朝敵人斬出數道劍影！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIY "$n" HIY "凝神一閃，瞬時無情地朝敵人斬出數道劍影！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIY "你大喝一聲 手中之劍發出肅殺之氣！霸皇劍極招使出！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIY "$n" HIY "大喝一聲 手中之劍發出肅殺之氣！霸皇劍極招使出！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        break;
    }

    net_dr = dr = ch->damroll;
    // 加入 hr 判定 2023/01/06
    net_hr = hr = ch->hitroll;

    old_ac    = GET_AC(victim);
    old_dodge = GET_DODGE(victim);

    ac    = old_ac;
    dodge = old_dodge;

    // 正值判定 2023/01/06
    if (ac > 0)
        ac = 0;
    if (dodge > 0)
        dodge = 0;

    old_ac    = victim->armor;
    old_dodge = victim->dodge;

    // 將 pc 與 npc 分開寫判定 2023/01/06
    if (IS_NPC(ch)) {
        ch->damroll = (dr * (20 + sklv)) / 20;      // dr 加成
        victim->dodge -= (dodge * (4 + sklv)) / 20; // dc 少 20% 到 50%
        victim->armor -= (ac * (4 + sklv)) / 30;    // ac 少 13% 到 33%
    } else {
        // sklv 對 dr 的加成
        if ((sklv + str_ch) > 3)
            ch->damroll = (dr * (sklv + str_ch)) / 3;

        // sklv 對 hr 的加成 2023/01/06
        ch->hitroll = (hr * (70 + sklv * sklv)) / 70;

        // 等級差值 2023/01/06
        level_diff = victim->level - ch->level;

        // 等級 22 以上才會有等差加成 , 但正常情況下不會發生在 ppl 身上 ( empslash 需 lv 34 才能學習 )
        if (level_diff > 0) {
            // 每個 level_diff 增加 dr , 最高 3.5 倍
            ch->damroll += (level_diff * (1 + sklv)) / 2;

            // 等差上限值
            if (ch->level >= 41)
                limit_lv = 15;
            else if (ch->level >= 40)
                limit_lv = 10;
            else if (ch->level >= 39)
                limit_lv = 6;
            else if (ch->level >= 38)
                limit_lv = 6;
            else if (ch->level >= 37)
                limit_lv = 4;
            else if (ch->level >= 36)
                limit_lv = 4;
            else if (ch->level >= 35)
                limit_lv = 3;
            else if (ch->level >= 34)
                limit_lv = 3;
            else
                limit_lv = 2;

            // 破盾術能額外增加等差上限值
            limit_lv = limit_lv + break_sklv / 2;

            limit_lv = UMIN(level_diff, limit_lv);

            victim->dodge -= (dodge * limit_lv) / 40; // dc 最高降低 45%
            victim->armor -= (ac * limit_lv) / 45;    // ac 最高降低 40%
        }
    }

    /*  修改 empslash 等級及 sklv 加成判定 2023/01/06

            // Modified by Keric for dr, 2005/2/13
            if( vlevel + sklv > level )
            {
                    if ( sklv > 4 ){
                            if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
                                    ch->damroll  = dr*5/2;
                            }else{
                                    ch->damroll  = dr*2;
                            }
                            victim->dodge -= dodge*3/4; // 降 37.5% DC
                            victim->armor -= ac*3/4 ;   // 降 75%   AC
                    }else if ( sklv > 3 ){
                            if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
                                    ch->damroll  = dr*7/3;
                            }else{
                                    ch->damroll  = dr*3/2;
                            }
                            victim->dodge -= dodge*2/4  ;
                            victim->armor -= ac*1/2 ;
                    }else if(  sklv > 2 || ch->level > 39){
                            if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
                                    ch->damroll = dr*2;
                            }
                            victim->dodge -= dodge*2/4;
                            victim->armor -= ac*1/3;
                    }else if(  sklv > 1 || ch->level > 38){
                            if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
                                    ch->damroll = dr*6/4;
                            }
                            victim->dodge -= dodge*1/3;
                            victim->armor -= ac*1/4;
                    }else if( sklv > 0 || ch->level > 37){
                            victim->dodge -= dodge/3;
                            victim->armor -= ac*1/4;
                    }
                    // empslash level diff effect
                    if(vlevel > ch->level)
                    {
                            if( ch->level >= 37 ) limit_lv += 1;
                            if( ch->level >= 38 ) limit_lv += 1;
                            if( ch->level >= 39 ) limit_lv += 2;
                            if( ch->level >= 40 ) limit_lv += 4;
                            if( ch->level >= 41 ) limit_lv += 5;

                            limit_lv = UMAX( limit_lv , limit_lv + break_sklv + sklv);
                            ch->damroll += dr * UMIN( limit_lv , vlevel - ch->level )/17;
                            // 對上超高等級對手 每超過上限的 lv 增加 1 dr
                            if ( vlevel - ch->level > limit_lv )
                                    ch->damroll += vlevel - ch->level - limit_lv;
                    }
            }
    */

    chk_dr = ch->damroll; // add by jye 0940607
    chk_hr = ch->hitroll; // 2023/01/06

    while (1) {
        victim->dodge -= dodge / 7; // 降 14% DC
        mone_hit(ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE);
        wait += ch->wait;
        ch->wait = 0;
        mone_hit(ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE);
        wait += ch->wait;
        ch->wait = 0;
        if (IS_NPC(ch)) {
            mone_hit(ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE);
            mone_hit(ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE);
        }

        if (!IS_NPC(ch)) {
            if (ch->fighting != victim || sklv < 1)
                ; // do nothing
            else {
                mone_hit(ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE);
                wait += ch->wait;
                ch->wait = 0;
            }

            if (ch->fighting != victim || sklv < 3)
                ;
            else {
                mone_hit(ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE);
                wait += ch->wait;
                ch->wait = 0;
            }
        }
        break;
    }

    if (is_qmark(victim, "empslash_preset")) {
        // npc 的 del qmark 必須使用 mpdel_qmark 2022/04/14
        if (IS_NPC(victim))
            do_mpdel_qmark(victim, "self empslash_preset");
        else
            do_del_qmark(victim, "self empslash_preset");
        victim->armor = old_ac;
        victim->dodge = old_dodge;
    }

    if (ch->damroll != 0) // ch may die! damroll may set to 0!
    {
        // modified by jye 6.07.2005
        // 增加chk_dr記錄判斷攻擊前之dr
        // 在還原ppl dr前先chk_dr(先)減ch->damroll(後)由不為0表示有噴裝
        // 在還原ppl dr時再減掉噴裝之dr
        if (ch->position == POS_FIGHTING) {
            ch->damroll = dr - (chk_dr - ch->damroll);
        } else {
            ch->damroll = net_dr - (chk_dr - ch->damroll);
        }
    }

    // 增加 hr 還原判定 2023/01/06
    if (ch->hitroll != 0) {
        if (ch->position == POS_FIGHTING) {
            ch->hitroll = hr - (chk_hr - ch->hitroll);
        } else {
            ch->hitroll = net_hr - (chk_hr - ch->hitroll);
        }
    }

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    WAIT_STATE(ch, skill_table[gsn_empslash].beats);
    WAIT_STATE(ch, wait);
}

// modifyed by jye 6.08 2005
void do_twin(CHAR_DATA *ch, char *argument) /*{{{*/
{
    CHAR_DATA *victim, *rch = NULL;
    OBJ_DATA  *obj;
    int        wp, dam;
    int        cost     = 0;
    int        sn       = skill_lookup("twin swords");
    int        sklv     = get_skill_level(ch, sn);
    int        rsklv    = 0;
    int        intToken = 0;

    /* Don't allow charmed mobs to do this, check player's level */
    // if( ( IS_NPC( ch ) && !is_real_npc(ch))
    //      || ( !IS_NPC( ch ) && check_skill(ch, gsn_twin, number_percent( )  )))
    //{
    //      send_to_char("你失敗了。\n\r", ch);
    //      return;
    // }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (IS_SET(ch->in_room->room_flags, ROOM_SAFE)) {
        send_to_char("這裡禁止戰鬥. \n\r", ch);
        return;
    }

    if (!can_see(ch, ch->fighting)) {
        send_to_char("你看不到目標。\n\r", ch);
        return;
    }

    if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
            send_to_char("好歹拿把武器吧？\n\r", ch);
            return;
        }
        wp = WEAR_WIELD_2;
    } else
        wp = WEAR_WIELD;

    if (!IS_NPC(ch) && sklv < 6 && obj->value[3] != 1 && obj->value[3] != 3) {
        send_to_char("劍術是要用劍施展的。\n\r", ch);
        return;
    }

    if ((IS_NPC(ch) && !is_real_npc(ch)) || !check_skill(ch, gsn_twin, number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch)) {
        // 修改為判定 cooperator 2021/11/23
        // for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
        for (rch = ch->cooperator; rch; rch = rch->next_in_coop) {
            if (!IS_NPC(rch) && rch->class == SWORDSMAN && rch != ch && IS_AWAKE(rch)
                // ch 也需 cooperate rch , 能指定配合者 2021/11/23
                //&& rch->pcdata->autocombat == AUTOC_COOPERATE
                //&& is_same_group( rch, ch ))
                && (ch->cooperating == rch) && (ch->in_room == rch->in_room)) {
                intToken = 1;
                if (ch->protecting == rch) {
                    act("你正全心護衛著$N。", ch, NULL, rch, TO_CHAR);
                    return;
                }
                if (rch->protecting == ch) {
                    intToken = 2;
                    break;
                }
            }
        }

        /*if( !rch )
                switch( intToken )
                {
                        default:
                                send_to_char( "你找不到人配合你。\n\r", ch );
                                return;
                        case 1:
                                send_to_char( "並沒有隊友全心護衛著你。\n\r", ch );
                                return;
                }
        */
        if (!rch || (intToken == 0)) {
            send_to_char("你找不到人配合你。\n\r", ch);
            return;
        }
        if ((intToken != 2)) {
            send_to_char("同伴並沒有全心護衛著你。\n\r", ch);
            return;
        }
    } else {
        // 修改為判定 cooperator 2021/11/23
        // for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
        for (rch = ch->cooperator; rch; rch = rch->next_in_coop) {
            if (rch != ch && IS_AWAKE(rch) && (ch->cooperating == rch) && (ch->in_room == rch->in_room)) {
                intToken = 2;
                break;
            }
        }
        if (intToken != 2) {
            send_to_char("你找不到人配合你。\n\r", ch);
            return;
        }
    }

    if (!IS_NPC(ch) && !check_skill(rch, gsn_twin, number_percent())) {
        act("$N無法成功\配合你。", ch, NULL, rch, TO_CHAR);
        return;
    }

    cost = UMIN(ch->max_move / 5, rch->max_move / 5);

    if (!IS_NPC(ch) && (ch->move < cost || rch->move < cost)) {
        send_to_char("你們覺得好困乏。\n\r", ch);
        return;
    }

    rsklv = get_skill_level(rch, sn);

    if (!IS_NPC(ch))
        ch->move -= cost;

    if (!IS_NPC(rch))
        rch->move -= cost;

    if (!IS_NPC(ch))
        waste_dura(obj);

    // act( "$n",   ch, NULL, rch, TO_ROOM    );
    act("在$N的護衛下，你全力施展劍技！", ch, NULL, rch, TO_CHAR);
    act("你收起攻勢，全心護衛$n。", ch, NULL, rch, TO_VICT);
    act("$n一聲大喝，與$N一同施展出「" HIY "日" HIW "月" NOR "雙劍" NOR "」！", ch, NULL, rch, TO_NOTVICT);

    victim = ch->fighting;

    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!IS_NPC(ch))
        dam = dam + cost / 2;

    if (!IS_NPC(ch))
        dam = UMIN(1000, (rch->armor / -(8 - rsklv))) + ch->damroll * (sklv * 3 + 2) + (rch->played + ch->played) / 100000;
    else
        dam = (ch->level + ch->damroll / 2) * (1 + sklv + rsklv);

    dam = dam + rch->pdr;

    dam = dam - victim->pdr;

    dam = dam * weakness_damage(victim, sn) / 100;

    if (IS_NPC(victim) && dam <= victim->pad)
        dam = 0;

    if (victim->deleted || victim->in_room != ch->in_room)
        return;
    victim->hit -= dam;
    dam_message(ch, victim, dam, gsn_twin, WEAR_NONE);
    ////brief_damage( ch, victim, 0, gsn_twin, WEAR_NONE);
    damage(ch, victim, 0, gsn_twin, WEAR_NONE, MSG_BRIEF, DMG_NORMAL);

    WAIT_STATE(ch, skill_table[gsn_twin].beats - sklv);
    WAIT_STATE(rch, skill_table[gsn_twin].beats - rsklv);

    if (sklv + rsklv > 9 && !victim->deleted) {
        if (victim->deleted || victim->in_room != ch->in_room)
            return;
        victim->hit -= dam;
        dam_message(ch, victim, dam, gsn_twin, WEAR_NONE);
        ////brief_damage( ch, victim, 0, gsn_twin, WEAR_NONE);
        damage(ch, victim, 0, gsn_twin, WEAR_NONE, MSG_BRIEF, DMG_NORMAL);
    }

    if (sklv + rsklv > 11 && !victim->deleted) {
        if (victim->deleted || victim->in_room != ch->in_room)
            return;
        victim->hit -= dam;
        dam_message(ch, victim, dam, gsn_twin, WEAR_NONE);
        ////brief_damage( ch, victim, 0, gsn_twin, WEAR_NONE);
        damage(ch, victim, 0, gsn_twin, WEAR_NONE, MSG_BRIEF, DMG_NORMAL);
    }

    // update_pos(victim);
    if (!victim->deleted && victim->in_room == ch->in_room)
        disarm(ch, victim);

    // WAIT_STATE( ch, skill_table[gsn_twin].beats - sklv );
    // WAIT_STATE( rch, skill_table[gsn_twin].beats - rsklv );
    return;
} /*}}}*/

// flaming slash modified by whisperwind 2.17.2005
void do_flaming(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA  *obj  = NULL;
    int        wp   = WEAR_NONE;
    int        sklv = get_skill_level(ch, gsn_flaming_slash);

    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_flaming_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && (ch->move < 30 * (1 + sklv) || ch->mana < 43 * (1 + sklv))) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    } else if (!IS_NPC(ch)) {
        ch->move -= 30 * (1 + sklv);
        ch->mana -= 43 * (1 + sklv);
    }

    victim = ch->fighting;
    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!check_blind(ch))
        return;

    // 第一段廣域火焰波
    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        act(HIR "你手中揮舞著長劍，強烈的劍氣捲起大量的火燄！" NOR, ch, NULL, NULL, TO_CHAR);
        act("$n" HIR "手中揮舞著長劍，強烈的劍氣捲起大量的火燄！" NOR, ch, NULL, NULL, TO_ROOM);
    } else {
        act(HIR "你將念力凝聚成魔法劍，強烈的劍氣捲起大量的火燄！" NOR, ch, NULL, NULL, TO_CHAR);
        act("$n" HIR "將念力凝聚成魔法劍，強烈的劍氣捲起大量的火燄！" NOR, ch, NULL, NULL, TO_ROOM);
    }

    spell_flaming_slash(-1, ch->level, ch, victim);

    if (sklv > 5) {
        spell_flaming_slash(-1, ch->level, ch, victim);
    }

    if (get_skill_level(ch, skill_lookup("fireball")) > 3 && sklv > 3) {
        spell_flaming_slash(-1, ch->level, ch, victim);
    }

    // 第二段強化火焰波
    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (IS_GOOD(ch)) {
            act(HIR "一道耀眼的火柱自天而降，與你的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIR "一道耀眼的火柱自天而降，與" NOR "$n" HIR "的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIR "一道黑色火柱從地面冒出，與你的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIR "一道黑色火柱從地面冒出，與" NOR "$n" HIR "的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIR "一道精神形成的火焰從你手中往劍上延伸，與你的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIR "一道精神形成的火焰從" NOR "$n" HIR "手中往劍上延伸，與$e的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        waste_dura(obj);
        mone_hit(ch, victim, &gsn_flaming_slash, wp, MSG_NORMAL, DT_NOCHANGE);
    } else {
        if (IS_GOOD(ch)) {
            act(HIR "一道耀眼的火柱自天而降，與你的念力產生共鳴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIR "一道耀眼的火柱自天而降，與" NOR "$n" HIR "的念力產生共鳴！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIR "一道黑色火柱從地面冒出，與你的念力產生共鳴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIR "一道黑色火柱從地面冒出，與" NOR "$n" HIR "的念力產生共鳴！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIR "一道精神形成的火焰從你眼中往魔法劍上延伸，與你的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIR "一道精神形成的火焰從" NOR "$n" HIR "眼中往魔法劍上延伸，與$e的劍氣產生共鳴！" NOR, ch, NULL, NULL, TO_ROOM);
        }
    }

    if (ch->fighting != victim)
        return;

    spell_flaming_slash2(-1, ch->level, ch, victim);

    if (ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room) {
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    WAIT_STATE(ch, skill_table[gsn_flaming_slash].beats - sklv);
    return;
}

// icecore slash add by whisperwind 2.17.2005
void do_icecore(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int        wp  = WEAR_NONE;
    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_icecore_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && (ch->move < 85 || ch->mana < 85)) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    }
    /*	else if ( !IS_NPC(ch)){
            ch->move -= 180;
            ch->mana -= 200;
        }
    */
    victim = ch->fighting;
    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (IS_GOOD(ch)) {
            act(HIC "一股寒氣籠罩了你的武器，對眼前所有敵人舞出滅敵的冰之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIC "一股寒氣籠罩了" NOR "$n" HIC "的武器，對眼前所有敵人舞出滅敵的冰之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIC "一股寒氣籠罩了你的武器，對眼前所有敵人舞出死亡的冰之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIC "一股寒氣籠罩了" NOR "$n" HIC "的武器，對眼前所有敵人舞出死亡的冰之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIC "一股凍氣籠罩了你的武器，對眼前所有敵人舞出強烈的冰之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIC "一股凍氣籠罩了" NOR "$n" HIC "的武器，對眼前所有敵人舞出強烈的冰之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        waste_dura(obj);
        mone_hit(ch, victim, &gsn_icecore_slash, wp, MSG_NORMAL, DT_NOCHANGE);
    } else {
        if (IS_GOOD(ch)) {
            act(HIC "一股寒氣籠罩了你的靈劍，對眼前所有敵人舞出滅敵的冰之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIC "一股寒氣籠罩了" NOR "$n" HIC "的靈劍，對眼前所有敵人舞出滅敵的冰之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIC "一股寒氣籠罩了你的靈劍，對眼前所有敵人舞出死亡的冰之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIC "一股寒氣籠罩了" NOR "$n" HIC "的靈劍，對眼前所有敵人舞出死亡的冰之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIC "一股凍氣籠罩了你的靈劍，對眼前所有敵人舞出強烈的冰之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIC "一股凍氣籠罩了" NOR "$n" HIC "的靈劍，對眼前所有敵人舞出強烈的冰之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        }
    }

    spell_icecore_slash(-1, ch->level, ch, victim);

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    if (ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room) {
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }
    WAIT_STATE(ch, skill_table[gsn_icecore_slash].beats);
    return;
}

// enables beholder performing magical slash under sklv 6 inborn ability, added by Keelar
// fulgurant slash add by whisperwind 2.17.2005
void do_fulgurant(CHAR_DATA *ch, char *argument) {
    CHAR_DATA       *victim;
    OBJ_DATA        *obj        = NULL;
    ROOM_INDEX_DATA *pFightRoom = NULL;

    int wp    = WEAR_NONE;
    int times = 2; // 段數設定
    int sklv  = get_skill_level(ch, gsn_fulgurant_slash);

    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_fulgurant_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && (ch->move < 13 || ch->mana < 13)) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    }
    /*   else if ( !IS_NPC(ch)){
           ch->move -= 100;
           ch->mana -= 150;
       }
    */
    victim = ch->fighting;
    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (IS_GOOD(ch)) {
            act(HIW "你的劍氣幻化成白色的疾電，張牙舞爪地劈向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "的劍氣幻化成白色的疾電，張牙舞爪地劈向$e的敵人！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIW "你的劍氣幻化成紫色的狂電，張牙舞爪地襲向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "的劍氣幻化成紫色的狂電，張牙舞爪地襲向$e的敵人！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIW "你的劍氣變幻成青色的奔電，張牙舞爪地劈向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "的劍氣變幻成青色的奔電，張牙舞爪地劈向$e的敵人！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        waste_dura(obj);
        mone_hit(ch, victim, &gsn_fulgurant_slash, wp, MSG_NORMAL, DT_NOCHANGE);
    } else {
        if (IS_GOOD(ch)) {
            act(HIW "你的精神力幻化成白色的疾雷劍，張牙舞爪地劈向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "的精神力幻化成白色的疾雷劍，張牙舞爪地劈向$e的敵人！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIW "你的精神力幻化成紫色的狂雷劍，張牙舞爪地襲向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "的精神力幻化成紫色的狂雷劍，張牙舞爪地襲向$e的敵人！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIW "你的精神力變幻成青色的奔雷劍，張牙舞爪地劈向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "的精神力變幻成青色的奔雷劍，張牙舞爪地劈向$e的敵人！" NOR, ch, NULL, NULL, TO_ROOM);
        }
    }

    if (ch->fighting != victim)
        return;

    if (!check_blind(ch))
        return;

    if (sklv > 1)
        times += 1;
    if (sklv > 3)
        times += 1;
    if (ch->level > 36)
        times += 1;
    if (ch->level > 38)
        times += 1;
    if (ch->level > 40)
        times += 1;

    times = UMIN(times, 6);

    if (sklv > 5)
        times += 1;

    // maximize fulgurant part
    if (!IS_NPC(ch) && sklv > 3) {
        times += UMAX(0, get_skill_level(ch, gsn_swordmaster));
        if (get_skill_level(ch, gsn_swordmaster) > 4)
            times += 1;
        if (get_skill_level(ch, gsn_swordmaster) > 5)
            times += 1;
    }

    for (pFightRoom = victim->in_room;
         times > 0 && !victim->deleted && pFightRoom == ch->in_room && pFightRoom == victim->in_room;
         times--) {
        if (ch->mana < 13 || ch->move < 13)
            break;
        spell_fulgurant_slash(-1, ch->level, ch, victim);
    }

    if (ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room) {
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    WAIT_STATE(ch, skill_table[gsn_fulgurant_slash].beats);
    return;
}

// storm slash modified by whisperwind 2.18.2005
void do_storm(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA  *obj  = NULL;
    int        wp   = WEAR_NONE;
    int        sklv = get_skill_level(ch, gsn_storm_slash);

    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_storm_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 && obj->value[3] != 4) {
            send_to_char("狂風斬要用劍或鞭施展。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && (ch->move < 15 || ch->mana < 15)) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    }
    /*	else if ( !IS_NPC(ch)){
            ch->move -= 150;
            ch->mana -= 180;
        }
    */
    victim = ch->fighting;
    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (IS_GOOD(ch)) {
            act(HIG "你快速揮動兵器，令周圍空氣化為強勁的旋風！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIG "快速揮動兵器，令周圍空氣化為強勁的旋風！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIG "你快速揮動兵器，令周圍空氣化為險惡的陰風！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIG "快速揮動兵器，令周圍空氣化為險惡的陰風！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIG "你快速揮動兵器，斬出數道真空波！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIG "快速揮動兵器，斬出數道真空波！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        waste_dura(obj);
        mone_hit(ch, victim, &gsn_storm_slash, wp, MSG_NORMAL, DT_NOCHANGE);
    } else {
        if (IS_GOOD(ch)) {
            act(HIG "你將精神力幻化為飛舞的魔法劍，令周圍空氣化為強勁的旋風！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIG "將精神力幻化為飛舞的魔法劍，令周圍空氣化為強勁的旋風！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIG "你將精神力幻化為飛舞的魔法劍，令周圍空氣化為險惡的陰風！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIG "將精神力幻化為飛舞的魔法劍，令周圍空氣化為險惡的陰風！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIG "你將精神力幻化為飛舞的魔法劍，斬出數道真空波！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIG "將精神力幻化為飛舞的魔法劍，斬出數道真空波！" NOR, ch, NULL, NULL, TO_ROOM);
        }
    }

    if (ch->fighting != victim)
        return;

    if (!check_blind(ch))
        return;

    spell_storm_slash(-1, ch->level, ch, victim);

    if (!IS_NPC(ch) && get_skill_level(ch, skill_lookup("whirlwind")) > 4 && sklv > 3 && ch->mana > 30) {
        if (IS_GOOD(ch)) {
            act(HIG "旋風逐漸擴大，轉眼間化成巨大的龍捲風暴！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIG "旋風逐漸擴大，轉眼間化成巨大的龍捲風暴！" NOR, ch, NULL, NULL, TO_ROOM);
        } else if (IS_EVIL(ch)) {
            act(HIG "陰風猛烈竄動，變幻成狂烈的魔性之風！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIG "陰風猛烈竄動，變幻成狂烈的魔性之風！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIG "真空波撕裂了周遭的大氣，形成利刃般的旋風！" NOR, ch, NULL, NULL, TO_CHAR);
            act(HIG "真空波撕裂了周遭的大氣，形成利刃般的旋風！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        spell_whirlwind(gsn_storm_slash, ch->level, ch, victim);
        ch->mana -= 30;
        if (get_skill_level(ch, skill_lookup("whirlwind")) > 5 && ch->mana > 30) {
            spell_whirlwind(gsn_storm_slash, ch->level, ch, victim);
            ch->mana -= 30;
        }
    }

    if (ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room) {
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    WAIT_STATE(ch, skill_table[gsn_storm_slash].beats);
    return;
}

void do_elemental(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA  *obj        = NULL;
    int        wp         = WEAR_NONE;
    int        nFlameSklv = get_skill_level(ch, gsn_flaming_slash);
    int        nStormSklv = get_skill_level(ch, gsn_storm_slash);
    int        nIceCoSklv = get_skill_level(ch, gsn_icecore_slash);
    int        nFulguSklv = get_skill_level(ch, gsn_fulgurant_slash);
    int        nEarthSklv = get_skill_level(ch, gsn_earth);

    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_elemental_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (nFlameSklv + nIceCoSklv + nStormSklv + nEarthSklv + nFulguSklv < 19) {
        send_to_char("你對元素劍的了解還不夠精湛！\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && ch->hit > ch->max_hit / 2) {
        send_to_char("你的危機意識不足以喚起你潛在的力量！\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && (ch->move < 150 || ch->mana < 150)) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    } else if (!IS_NPC(ch)) {
        ch->move -= 150;
        ch->mana -= 150;
    }

    victim = ch->fighting;
    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        act(HIG "你快速揮動兵器，斬出數道真空波！" NOR, ch, NULL, NULL, TO_CHAR);
        act("$n" HIG "快速揮動兵器，斬出數道真空波！" NOR, ch, NULL, NULL, TO_ROOM);
        waste_dura(obj);
        mone_hit(ch, victim, &gsn_elemental_slash, wp, MSG_NORMAL, DT_NOCHANGE);
    } else {
        act(HIG "你將精神力幻化為飛舞的魔法劍，令周圍空氣化為強勁的旋風！" NOR, ch, NULL, NULL, TO_CHAR);
        act("$n" HIG "將精神力幻化為飛舞的魔法劍，令周圍空氣化為強勁的旋風！" NOR, ch, NULL, NULL, TO_ROOM);
    }

    spell_elemental_slash(-1, ch->level, ch, victim);

    if (ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room) {
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

    WAIT_STATE(ch, skill_table[gsn_elemental_slash].beats);
    return;
}

void do_sun(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA  *obj  = NULL;
    int        wp   = WEAR_NONE;
    int        sklv = get_skill_level(ch, gsn_sun_slash);

    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_sun_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!ch->fighting) {
        send_to_char("你不是在戰鬥中。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch)) {
        if (IS_EVIL(ch)) {
            send_to_char("你的心靈無法凝聚神聖的力量.\n\r", ch);
            return;
        }
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch) && (ch->mana < 35 || ch->move < 70)) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    }
    /*	else if ( !IS_NPC(ch)){
            ch->move -=  220;
            ch->mana -=  180;
        }
    */
    victim = ch->fighting;
    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (IS_GOOD(ch)) {
            act(HIW "你凝聚了日與月的神聖力量，使出耀眼無比的光之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "凝聚了日與月的神聖力量，使出耀眼無比的光之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIW "你手中的武器發出萬丈的光芒，斬出無數道光之劍刃！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIW "手中的武器突然發出萬丈的光芒，斬出無數道光之劍刃！" NOR, ch, NULL, NULL, TO_ROOM);
        }
        waste_dura(obj);
        mone_hit(ch, victim, &gsn_sun_slash, wp, MSG_NORMAL, DT_NOCHANGE);
    } else {
        if (IS_GOOD(ch)) {
            act(HIY "你將日與月的神聖力量幻化為光劍，使出耀眼無比的光之劍舞！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIY "將日與月的神聖力量幻化為光劍，使出耀眼無比的光之劍舞！" NOR, ch, NULL, NULL, TO_ROOM);
        } else {
            act(HIY "你將精神力幻化為無數耀眼的光之劍刃，飛向你的敵人！" NOR, ch, NULL, NULL, TO_CHAR);
            act("$n" HIY "將精神力幻化為無數耀眼的光之劍刃！" NOR, ch, NULL, NULL, TO_ROOM);
        }
    }

    victim = ch->fighting;
    if (get_skill_level(ch, gsn_sun_flash) >= 4 && (ch->mana >= 80 && ch->move >= 70)) {
        spell_sun_flash(gsn_sun_slash, ch->level, ch, victim);
        ch->mana -= 80;
        ch->move -= 70;
    }
    if (get_skill_level(ch, gsn_sun_flash) >= 6 && (ch->mana >= 80 && ch->move >= 70)) {
        spell_sun_flash(gsn_sun_slash, ch->level, ch, victim);
        ch->mana -= 80;
        ch->move -= 70;
    }
    if (IS_NPC(ch) || (ch->mana >= 35 && ch->move >= 70)) {
        spell_sun_slash(-1, ch->level, ch, victim);
        ch->mana -= 35;
        ch->move -= 70;
    }
    if (IS_NPC(ch) || (ch->mana >= 35 && ch->move >= 70 && sklv >= 4)) {
        spell_sun_slash(-1, ch->level, ch, victim);
        ch->mana -= 35;
        ch->move -= 70;
    }
    if (IS_NPC(ch) || (ch->mana >= 35 && ch->move >= 70 && sklv >= 6)) {
        spell_sun_slash(-1, ch->level, ch, victim);
        ch->mana -= 35;
        ch->move -= 70;
    }

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    WAIT_STATE(ch, skill_table[gsn_sun_slash].beats);
}

void do_sprinkle(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int        wp  = WEAR_NONE;
    char       arg[MAX_INPUT_LENGTH]; // add at 2020/09/21
    int        sklv = get_skill_level(ch, gsn_sprinkle_slash);
    // 波紋劍 one_hit 傷害命中加成
    int hr_bonus = sklv * 20;
    int dr_bonus = sklv * 40;

    one_argument(argument, arg); // add at 2020/09/21
    if (!ch->in_room)
        return; // add at 2020/09/21
    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_sprinkle_slash) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }
    /*  新增非戰鬥中使用及指定目標, 故移除 2020/09/21
if ( !ch->fighting ){
    send_to_char( "你不是在戰鬥中。\n\r", ch );
    return;
}
    */

    // safe room  2020/09/21
    if (IS_SET(ch->in_room->room_flags, ROOM_SAFE)) {
        send_to_char("這裡禁止戰鬥. \n\r", ch);
        return;
    }

    if (is_affected(ch, gsn_stone_form)) {
        send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && sklv < 6 && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!check_blind(ch))
        return;

    if (!IS_NPC(ch) && (ch->move < 40 * (1 + sklv) || ch->mana < 40 * (1 + sklv))) {
        send_to_char("你覺得好困乏。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch)) {
        ch->move -= 40 * (1 + sklv);
        ch->mana -= 40 * (1 + sklv);
    }

    // victim = ch->fighting;
    //  新增非戰鬥中使用      2020/09/21
    //  無指定時判定有無當前戰鬥目標
    if (arg[0] == '\0') {
        if (!ch->fighting) {
            send_to_char("你要對誰使用波紋劍？\n\r", ch);
            return;
        } else {
            victim = ch->fighting;
        }
    } else {
        if (!(victim = get_char_room(ch, arg)) || !can_see(ch, victim)) {
            send_to_char("[33m他不在這兒。[m\n\r", ch);
            return;
        }
        if (is_same_group(ch, victim)) {
            send_to_char("[33m你不能攻擊自己的隊友。[0m\n\r", ch);
            return;
        }
    }

    if (victim->deleted || victim->in_room != ch->in_room)
        return;

    act(HIC "～～～～～波   紋   劍～～～～～" NOR, ch, NULL, NULL, TO_CHAR);
    act(HIC "～～～～～波   紋   劍～～～～～" NOR, ch, NULL, NULL, TO_ROOM);
    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        waste_dura(obj);
    }

    // if ( ch->fighting != victim ) return;
    // if ( !check_blind( ch ) ) return;

    // 波紋劍 one_hit 傷害命中加成
    ch->hitroll += hr_bonus;
    ch->damroll += dr_bonus;
    one_hit(ch, victim, gsn_sprinkle_slash, wp);
    ch->hitroll -= hr_bonus;
    ch->damroll -= dr_bonus;

    spell_sprinkle_slash(-1, ch->level + sklv * 2, ch, victim);

    if (ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room) {
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    int nChance = 25 + sklv * 15;
    nChance     = nChance * (old_saves_spell(ch->pp, victim, MAGIC_COLD)) / 100;
    nChance     = nChance * weakness_percent(victim, gsn_sprinkle_slash) / 100;

    WAIT_STATE(ch, 9 - sklv);
    if (victim->deleted)
        return;
    if (number_percent() < nChance && number_percent() > (get_curr_dex(victim) - 10) * 2) {
        AFFECT_DATA af;
        int         nSlowAct = -10 - sklv * (sklv + 4) * 2 / 3;

        act(HIC "你的波紋劍氣纏繞著" NOR "$N" HIC "，使他無法行動自如！" NOR, ch, NULL, victim, TO_CHAR);
        act("$n" HIC "的波紋劍氣纏繞著你，使你無法行動自如！" NOR, ch, NULL, victim, TO_VICT);
        act("$n" HIC "的波紋劍氣纏繞著" NOR "$N" HIC "，使他無法行動自如！" NOR, ch, NULL, victim, TO_NOTVICT);
        af.type     = gsn_sprinkle_slash;
        af.duration = 0 + sklv / 5 + sklv / 6;
        if (IS_NPC(victim)) {
            af.duration *= 2;
        }
        af.location  = APPLY_ACTION_ABILITY;
        af.modifier  = nSlowAct;
        af.bitvector = 0;
        if (number_percent() < (sklv + 1) * 10) {
            af.bitvector = AFF_HOLD;
            act(HIB "強烈的波紋劍氣使得" NOR "$N" HIB "完全無法逃跑！" NOR, ch, NULL, victim, TO_CHAR);
            act(HIB "強烈的波紋劍氣使得你完全無法逃跑！" NOR, ch, NULL, victim, TO_VICT);
            act(HIB "強烈的波紋劍氣使得" NOR "$N" HIB "完全無法逃跑！" NOR, ch, NULL, victim, TO_NOTVICT);
            affect_strip(victim, gsn_sprinkle_slash);
            affect_join(victim, &af);
        } else if (is_affected(victim, gsn_sprinkle_slash)) {
            if (affected_value(victim, gsn_sprinkle_slash, APPLY_ACTION_ABILITY) > nSlowAct) {
                affect_strip(victim, gsn_sprinkle_slash);
                affect_join(victim, &af);
            } else {
                return;
            }
        } else {
            affect_join(victim, &af);
        }
    }

    /* Previous Sprinkle Slash *
        if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
            WAIT_STATE( ch, 85 - ( get_curr_dex( ch ) * 2 ) - sklv * 4);
            if ( is_qmark(victim, "spell_sprinkle_timer") ) return;
            WAIT_STATE( victim, get_curr_int( ch ) + 5 +sklv * 3);
        }else{
            WAIT_STATE( ch, 110 - ( get_curr_dex( ch ) * 2 ) - sklv * 4);
            //  如果有了qmark,就return,讓mob會在1 tick之間不會再度被影響
            if (  is_qmark(victim, "spell_sprinkle_timer") ) return;
            WAIT_STATE( victim, get_curr_int( ch ) - 10 + sklv);
        }

        sprintf( buf, "self spell_sprinkle_timer 2 0 0 0 0 1" );
        do_add_qmark( victim, buf );
    */
    return;
}

void do_earth(CHAR_DATA *ch, char *argument) /*{{{*/
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int        wp  = WEAR_NONE;

    if ((IS_NPC(ch) && (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED) || ch->clan)) || (!IS_NPC(ch) && get_skill_percent(ch, gsn_earth) < number_percent())) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        if (!(obj = get_eq_char(ch, WEAR_WIELD)) && !(obj = get_eq_char(ch, WEAR_TWO_HAND))) {
            if (!(obj = get_eq_char(ch, WEAR_WIELD_2))) {
                send_to_char("好歹拿把武器吧？\n\r", ch);
                return;
            }
            wp = WEAR_WIELD_2;
        } else
            wp = WEAR_WIELD;

        if (!IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3) {
            send_to_char("劍術是要用劍施展的。\n\r", ch);
            return;
        }
    } else if (get_skill_level(ch, gsn_inborn_ability) < 3) {
        send_to_char("你的天賦還不足以將精神力凝聚成魔法劍。\n\r", ch);
        return;
    }

    if (!IS_NPC(ch)) {
        if (ch->move < 55 || ch->mana < 55) {
            send_to_char("你覺得好困乏。\n\r", ch);
            return;
        }
        /*
                else{
            ch->move -= 250;
            ch->mana -= 200;
        }  */
    }

    victim = ch->fighting ? ch->fighting : NULL;
    if (victim && (victim->deleted || victim->in_room != ch->in_room))
        return;

    if (!IS_SET(race_table[ch->race].race_abilities, RACE_BEHOLDER)) {
        waste_dura(obj);
        if (victim)
            one_hit(ch, victim, gsn_earth, wp);
    }

    if (ch->fighting != victim && ch->in_room->sector_type == SECT_AIR)
        return;

    spell_earth_slash(-1, ch->level, ch, NULL);

    // 物理技會判定 lizardman 天生技 2022/02/23
    if (!IS_NPC(ch) && IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN))
        LizardmanSkill(ch, victim, 0);

    WAIT_STATE(ch, skill_table[gsn_earth].beats);
    return;

} /*}}}*/
