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
//*****************************************************************
//*  PROGRAM ID  : skill_lead.c                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 Razgriz                                *
//*  PURPOSE     :         修正 do_lead_assistance                *
//*              :         在檢查 ch 是否為 leader 前             *
//*              :         加入 ch 與 protector 是否為同隊之判斷  *
//*              :         以避免之後的 leader 檢查造成 crash     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 Jye                                    *
//*  PURPOSE     :         修正 do_lead_ucharge 造成crash之bug    *
//*****************************************************************
/* $Id: skill_lead.c 3439 2005-10-14 15:55:48Z akira $ */
#include "skill.h"

void check_alignment_eq( CHAR_DATA *ch );
bool psionic_check(CHAR_DATA *ch, int sn );

//Add by Neversay at 2004/July/11
void do_lead_ucharge(CHAR_DATA *ch, char* argument)
{
	CHAR_DATA *rider;
	char arg[MAX_INPUT_LENGTH];
	int sklv = get_skill_level(ch, skill_lookup("lead"));

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "這裡禁止戰鬥. \n\r",ch);
		return;
	}
    
	if(!ch->fighting){
		send_to_char( "你不在戰鬥中. \n\r",ch);
		return;
	}
    
	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
		send_to_char("你失敗了。\n\r", ch);
		return;
	}
    
	if(ch->move >= 200-(sklv<<3))
	{
		ch->move -= 200 - (sklv<<3);
		send_to_char(""HIW"你率領騎兵向前衝鋒！"NOR"\n\r", ch);
		act("$n率領騎兵衝鋒！\n\r", ch, NULL, NULL, TO_ROOM);
		sprintf(arg, "%d", sklv);
		do_charge(ch, arg);
	}
	else
	{
		send_to_char("你的體力不夠了！\n\r", ch);
		return;
	}

	/*
			2022/04/05
			移除需為 kni 才會跟著衝鋒 , 意味著 lord 也能跟著衝刺
			移除下指令者必需為 leader 的設定
			修改為以 rider 是否有 cooperate ch 來判定是否跟著衝刺
	for( rider = ch->in_room->people; rider; rider = rider->next_in_room)
	{
		if( !is_same_group(ch,rider) )
			continue;
		
		if(    !rider->group
			|| !rider->group->leader
			|| rider->deleted
			|| rider->class != KNIGHT )
				continue;
				
		if( rider->group->leader == ch && rider->wait <= skill_table[gsn_charge].beats )
		{
			sprintf(arg, "%d", sklv);
			do_charge(rider, arg);
		}
	}
	*/
	for( rider = ch->cooperator; rider; rider = rider->next_in_coop )
	{
		if( rider->deleted )
			continue;

		if( !is_same_group(ch,rider) )
			continue;
		
		if( rider->cooperating != ch )
			continue;

		if( rider->in_room != ch->in_room )
			continue;

		if( IS_NPC(rider) )
			continue;
		
		if( get_skill_percent( rider ,gsn_charge) <= 0 )
			continue;

		if( rider->wait <= skill_table[gsn_charge].beats )
		{
			sprintf(arg, "%d", sklv);
			do_charge(rider, arg);
		}
	}

	WAIT_STATE(ch, 6);
}

//Add by Neversay at 2004/July/11
void do_lead_ufire(CHAR_DATA *ch, char* argument)
{
	CHAR_DATA *archer = NULL, *rch;
	OBJ_DATA *obj, *arrow;
	int sn = skill_lookup("lead"), sklv, sklv_archery, sklv_mete, number_enemy = 0, wait, archer_hr, fire_number, temp;
	bool can_fire = FALSE;
	AFFECT_DATA aff;
	int dt_undefined = TYPE_UNDEFINED; //Added by Razgriz 20051014

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
        send_to_char( "這裡禁止戰鬥. \n\r",ch);
        return;
    }
    if(!ch->fighting){
        send_to_char( "你不在戰鬥中. \n\r",ch);
        return;
    }
    if( get_skill_percent(ch, sn) <= number_percent()){
        send_to_char("你失敗了。\n\r", ch);
        return;
    }
    sklv = get_skill_level(ch, sn);
    if(sklv < 1){
        send_to_char("你的領導技巧不夠純熟。\n\r", ch);
        return;
    }

    if(ch->move >= 100-(sklv<<3))
    {
        ch->move -= 100 - (sklv<<3);
        send_to_char(""HIW"你指揮弓箭手箭矢齊放！"NOR"\n\r", ch);
        act("$n指揮弓箭手箭矢齊放！", ch, NULL, NULL, TO_ROOM);
    }
    else
    {
        send_to_char("你的體力不夠了！\n\r", ch);
        return;
    }

    //Calculate the number of enemy and archer in group.
    for(rch = ch->in_room->people;rch;rch = rch->next_in_room)
    {
        if(rch->deleted)continue;
        if(rch->fighting && is_same_group(rch->fighting, ch))
            number_enemy++;
    }

    if(!number_enemy)
    {
        send_to_char("沒有敵軍可以讓你射擊。\n\r",ch);
        return;
    }

    //The penalty of hitroll when use this skill.
    aff.type	 = gsn_meteoric_arrow;
    aff.duration = 1;
    aff.location = APPLY_HITROLL;

    //Fire by archers who leading by lord.
	// 修改為 以 cooperate 為判定 2022/04/05
    //for(archer = ch->in_room->people; archer; archer=archer->next_in_room)
	for(archer = ch->cooperator; archer; archer = archer->next_in_coop)
    {
		
		/*
			2022/04/05

			移除下指令者必需為 leader 的設定
			修改為以 archer 是否有 cooperate ch 來判定是否跟著衝刺
			並將每個條件分開判定, 以加入其他條件

        if( archer->deleted
                || get_skill_percent(archer, gsn_meteoric_arrow )<= 0 
                || archer->wait > skill_table[gsn_meteoric_arrow].beats 
                || !archer->group 
                || archer->group->leader != ch 
                || !archer->fighting 
                || (IS_NPC( archer ) 
                    && ( IS_AFFECTED( archer, AFF_CHARM )
                        || IS_AFFECTED( archer, AFF_SUMMONED ) 
                        || archer->clan )))
        {
            continue;
        }
		*/

		if( archer->deleted )
			continue;
		
		if( !is_same_group(ch,archer) )
			continue;

		if( archer->cooperating != ch )
			continue;

		if( archer->in_room != ch->in_room )
			continue;

		if( IS_NPC( archer ) ){
			if( IS_AFFECTED( archer, AFF_CHARM ) )
				continue;
			// mob class 16 is archer
			if( !IS_SET( archer->class, 16 ) )
				continue;
		}

		if( !IS_NPC( archer ) && get_skill_percent(archer, gsn_meteoric_arrow ) <= 0 )
			continue;

		if( archer->wait > skill_table[gsn_meteoric_arrow].beats )
			continue;

        if ( !( obj = get_eq_char( archer, WEAR_TWO_HAND ) ) || obj->value[3] != 14 )
        {
            //send_to_char( "好歹拿把弓吧？\n\r", archer );
            continue;
        }

        if ( !( arrow = get_eq_char( archer, WEAR_ARROW ) ) || arrow->value[0] == 0 )
        {
            //send_to_char( "喔喔，沒箭了!\n\r", archer );
            continue;
        }

        if(!IS_NPC(archer)
			&&( (get_skill_percent(archer, gsn_meteoric_arrow)>>1)
				+ (sklv<<3) < number_percent() ) )
        {
            send_to_char("你試圖聽從領主的指揮，可惜失敗了！",archer);
            continue;
        }

        for( can_fire = TRUE, rch = ch->in_room->people; rch; rch = rch->next_in_room )
        {
            if ( rch->fighting == archer )
            {
				int ride_sklv   = get_skill_level(archer, gsn_ride );
				int tricky_sklv = get_skill_level(archer, gsn_tricky_pacing );

				// 可以自由射箭的條件 2022/04/05
				if( get_eq_char( archer, WEAR_RIDE ) && ride_sklv > 2 && ride_sklv >= tricky_sklv )
				{
					if( number_percent( ) < 10 + ride_sklv * 15 )
						break;
				}
				else if( tricky_sklv > 2 && number_percent( ) < 10 + tricky_sklv * 15 )
				{
					break;
				}
				send_to_char( "你被打得手忙腳亂，沒時間拉弓。\n\r", archer );
				can_fire = FALSE;
				break;
            }
        }
        
        if(!can_fire)
            continue;

        if ( archer->move < 150 )
        {
            send_to_char( "你覺得好累！\n\r", archer );
            continue;
        }

        sklv_mete = get_skill_level(archer, gsn_meteoric_arrow);
        sklv_archery = get_skill_level(archer, gsn_shot_arrow);
        fire_number = UMAX((3+sklv_mete*3)/number_enemy, 1);
        archer_hr = (((sklv<<1)
					+sklv_archery
					-(sklv>number_bits(2)?20:50))<<2)/number_enemy;
        archer->move -= 150;
        send_to_char( "你挽弓砲轟，許\多箭矢如流星般往敵陣飛去！\n\r", archer );

        //Join the affect to archer and remove it after firing.
        if((archer->hitroll>>2) + archer_hr < 0)
            send_to_char("你的飛箭射歪了！\n\r", archer);
        aff.modifier = archer_hr;
        aff.bitvector = 0;
        affect_to_char( archer, &aff );
        wait = archer->wait;archer->wait = 0;

        //archer fire to enemies.
        for(rch = ch->in_room->people; rch; rch = rch->next_in_room){
            if(rch->deleted || !rch->fighting) continue;
            if(is_same_group(rch->fighting, ch))
                for(temp = 0;temp < fire_number; temp++) {
                    ////mone_hit( archer, rch, TYPE_UNDEFINED, WEAR_WIELD );
                    mone_hit( archer, rch, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
                    wait += archer->wait;archer->wait = 0;
                }
        }

        affect_strip( archer, gsn_meteoric_arrow);

        if ( sklv_mete < 5 || archer->level < 42 )
            WAIT_STATE( archer, skill_table[gsn_meteoric_arrow].beats );
        else
        {
            WAIT_STATE( archer, skill_table[gsn_meteoric_arrow ].beats );
            WAIT_STATE( archer, skill_table[gsn_meteoric_arrow ].beats / 3);
        }

        WAIT_STATE(archer, wait);
    }

    WAIT_STATE(ch, (skill_table[gsn_meteoric_arrow].beats<<1));
}

//Add by Neversay at 2004/July/12
void do_lead_assault(CHAR_DATA *ch, char* argument)
{
    CHAR_DATA *rch;
    int sklv;

    if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
    {
        send_to_char( "這裡禁止戰鬥. \n\r",ch);
        return;
    }
    if(!ch->fighting || ch->fighting->deleted )
    {
        send_to_char("你尚未身處戰鬥中！\n\r",ch);
        return;
    }
    
    if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent())
    {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }
    
    sklv = get_skill_level(ch, skill_lookup("lead"));
    
    if(sklv < 3)
    {
        send_to_char("你的領導技巧不夠純熟。\n\r", ch);
        return;
    }
    
    if(ch->move >= 150-(sklv<<3))
    {
        ch->move -= 150 - (sklv<<3);
        act("你帶頭衝向敵人$N！", ch, NULL, ch->fighting, TO_CHAR);
        act("$n帶領大家衝進敵陣，突襲$N！", ch, NULL, ch->fighting, TO_ROOM);
    }
    else
    {
        send_to_char("你的體力不夠了！\n\r", ch);
        return;
    }

    for(rch = ch->in_room->people;rch;rch = rch->next_in_room){
        if(!is_same_group(rch, ch)
                || !check_blind(rch)
                || !can_see(rch, ch->fighting)
                || rch->fighting == ch->fighting 
                || (IS_NPC( rch ) 
                    && ( IS_AFFECTED( rch, AFF_CHARM )
                        || IS_AFFECTED( rch, AFF_SUMMONED ) 
                        || rch->clan )) 
                || (!IS_NPC(rch) 
                    && (ch->pcdata->autocombat 
                        & (AUTOC_CONCENTRATION 
                            | AUTOC_DODGE 
                            | AUTOC_PARRY 
                            | AUTOC_STOP))) )
            continue;
        //Changing percentage from 20%~100%
        if((UMAX(get_skill_percent( ch, gsn_change_fighting),get_skill_percent( rch, gsn_change_fighting))>>2)
                + get_curr_int(rch) + get_curr_dex(rch) + sklv*5 < number_percent()){
            if( !is_chinese(rch, ch->fighting))
                send_to_char("You cannot break through the defend of enemies！\n\r", rch);
            else
                send_to_char("你衝不過敵方的防線！\n\r", rch);
        }
        else{
            if( !is_chinese(rch, ch->fighting))
                act( "You change your fighting target to $N!",  rch, NULL, ch->fighting, TO_CHAR);
            else
                act( "你開始攻擊 $N！",  rch, NULL, ch->fighting, TO_CHAR);
            if( !is_chinese(ch->fighting, rch))
                act( "$n turn $s target to you!", rch, NULL, ch->fighting, TO_VICT);
            else
                act( "$n 轉身向你發動攻擊！", rch, NULL, ch->fighting, TO_VICT  );
            //Do changing fighting.
            rch->fighting        = NULL;
            rch->position        = POS_STANDING;
            WAIT_STATE( rch, skill_table[gsn_change_fighting].beats );
            update_pos( rch );
            stop_fighting(rch, FALSE);
            set_fighting( rch, ch->fighting );
        }
    }
    WAIT_STATE( ch, (skill_table[gsn_change_fighting].beats<<2) - sklv*3 );
    return;
}

//Add by Neversay at 2004/July/12
void do_lead_assistance(CHAR_DATA *ch, CHAR_DATA *protector)
{
	CHAR_DATA *rch;
	// 移除 number_rescue 參數, 讓 wait 在每個 rescue 時判定 2023/01/12
	//int sklv, wait, number_rescue = 0;
	int sklv;
	int wait;
	int p_move_cost; // 2023/01/12

	if( !ch->group ) 
	{
		send_to_char("你並沒有在隊伍中。\n\r", ch);
		return;
	}

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "這裡禁止戰鬥. \n\r",ch);
		return;
	}
	if(!ch->fighting || ch->fighting->deleted){
		send_to_char("你尚未身處戰鬥中！\n\r",ch);
		return;
	}
	if(!protector) {
		send_to_char("沒有人可以保護隊伍！\n\r",ch);
		return;
	}
	
	if( !is_same_group( ch, protector ) )			//Added by Razgriz  20050618
	{
		send_to_char("對方不在你的隊伍中！\n\r",ch);
		return;
	}
	
	if( protector->group->leader && protector->group->leader != ch)
	{
		send_to_char("對方不受你指揮！\n\r",ch);
		return;
	}

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
		send_to_char("你失敗了。\n\r", ch);
		return;
	}
	sklv = get_skill_level(ch, skill_lookup("lead"));

	if(sklv < 2){
		send_to_char("你的領導技巧不夠純熟。\n\r", ch);
		return;
	}

	// stone form 的狀態下無法執行 lead 2022/12/05
	if( is_affected(protector, gsn_stone_form) ){
		send_to_char("他正處在石化狀態下，無法執行你的指揮。\n\r", ch);
		return;
	}

	// 判定 protector 的 mv 是否足夠使用 rescue 2023/01/12
	p_move_cost = 200 - get_skill_level(protector, gsn_rescue)*20;

	if( protector->move < p_move_cost ){
		send_to_char("他的體力不足以執行這個指示。\n\r", ch);
		return;
	}

	// 判定 protector wait 是否太高 2023/01/12
	if( protector->wait > 11 + sklv * 2 )
	{
		send_to_char( "[33m他已經手忙腳亂了！[m\n\r", ch );
		return;
	}

	if( ch->move >= 100 - (sklv<<3) ){
		ch->move -= 100 - (sklv<<3);
		act("你指揮$N保護整個隊伍！", ch, NULL, protector, TO_CHAR);
		act("$n指揮你保護整個隊伍！", ch, NULL, protector, TO_VICT);
		act("$n指揮$N保護整個隊伍！", ch, NULL, protector, TO_NOTVICT);
	}
	else{
		send_to_char("你的體力不夠了！\n\r", ch);
		return;
	}

	wait = protector->wait;

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room)
	{
		// protector 的 mv 小於能使用 rescue 的值時, 直接跳出程序 2023/01/12
		if( protector->move < p_move_cost ){
			send_to_char( "你的體力不足以繼續執行指示。\n\r", protector );
			break;
		}

		if( rch->deleted || !is_same_group(rch, ch) || protector == rch )
			continue;

		// 目標不在戰鬥中 (config -assist 的情況) 2023/01/12
		if ( !rch->fighting )
			continue;

		/* 2023/01/12
			修改 assistance , 直接以 do_rescue 判定是否成功
			並直接以 wait 作為每次 rescue 後的總值

		if(sklv + 1 >= number_bits(3)){
			do_rescue(protector, rch->name);
			number_rescue++;
		}else if(rch->fighting)
			act("你試圖保護$N，可是失敗了！", protector, NULL, rch, TO_CHAR); */

		do_rescue( protector, rch->name );
		// 每次 rescue 最少 wait + 1 2023/01/12
		wait += UMAX( 1 , protector->wait - wait - sklv/2 );
	}

	WAIT_STATE( ch, 12 - sklv );
	// 直接將 protector 的 wait 變更為 wait( 初始 + 多次rescue 的總值) 2023/01/12
	//WAIT_STATE( protector, number_rescue*(2-sklv/6) );
	protector->wait = wait;
}

//Add by Neversay at 2004/July/16
void do_lead_hidemarch(CHAR_DATA * ch, char* argument)
{
	CHAR_DATA * rch;
	AFFECT_DATA af;
	int number_of_scout, sklv;

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
            send_to_char("你失敗了。\n\r", ch);
            return;
        }
	sklv = get_skill_level(ch, skill_lookup("lead"));
        if(sklv < 4){
            send_to_char("你的領導技巧不夠純熟。\n\r", ch);
            return;
        }
        if(ch->move >= 200-(sklv<<4)){
	    ch->move -= 200 - (sklv<<4);
	    act("你指揮隊伍隱蔽身形，藏匿行軍！", ch, NULL, NULL, TO_CHAR);
	    act("$n指揮隊伍隱蔽身形，藏匿行軍", ch, NULL, NULL, TO_ROOM);
	}
	else{
	    send_to_char("你的體力不夠了！\n\r", ch);
	    return;
	}

	for(number_of_scout = 0,rch = ch->in_room->people;rch && number_of_scout <2;rch=rch->next_in_room)
	    if(!IS_NPC(rch) && (rch->class == NINJA || rch->class == ASSASSIN) &&
		is_same_group(rch,ch))
		number_of_scout++;

	if(number_of_scout < 2){
	    send_to_char("你隊伍中的斥侯不足！\n\r", ch);
	    return;
	}

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(is_same_group(rch,ch) ){
		if((time_info.hour < 4+(sklv>>1) || 
		    time_info.hour > 20-(sklv>>1)) &&
		    !IS_AFFECTED( rch, AFF_HIDE)){
		    SET_BIT( rch->affected_by, AFF_HIDE);
		    send_to_char("你照著領主的指示藏匿了起來。\n\r", rch);
		}
		if(!is_affected(rch, gsn_sneak)){
		    af.type      = gsn_sneak;
		    af.duration  = ch->level;
		    af.location  = APPLY_NONE;
		    af.modifier  = 0;
		    af.bitvector = AFF_SNEAK;
		    affect_to_char( rch, &af );
		    send_to_char("你照著領主的指示放輕腳步。\n\r", rch);
		}
	    }
	}
}

//Snare all enemy.
void do_lead_envelopment(CHAR_DATA * ch, char * argument){
	CHAR_DATA * rch;
	AFFECT_DATA af;
	int number_front = 0,number_back = 0,number_member = 1,
	    number_enemy = 0,sklv,chance = 0,sector_type, class_array[20];

	if( !ch->in_room ) return;
	sector_type = ch->in_room->sector_type;

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
            send_to_char("你失敗了。\n\r", ch);
            return;
        }
	if(!ch->fighting || ch->fighting->deleted){
	    send_to_char("你尚未身處戰鬥中！\n\r",ch);return;
	}
	if(ch->wait > skill_table[gsn_snare].beats){
	    send_to_char("你手忙腳亂，無法指揮隊伍！\n\r",ch);return;
	}
	sklv = get_skill_level(ch, skill_lookup("lead"));
        if(sklv < 5){
            send_to_char("你的領導技巧不夠純熟。\n\r", ch);
            return;
        }
        if(ch->move >= 300-(sklv<<5)){
	    ch->move -= 300 - (sklv<<5);
	    act("你指揮隊伍，包抄合圍敵軍！", ch, NULL, NULL, TO_CHAR);
	    act("$n指揮隊伍，包抄合圍敵軍！", ch, NULL, NULL, TO_ROOM);
	}
	else{
	    send_to_char("你的體力不夠了！\n\r", ch);
	    return;
	}

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(rch->deleted)continue;
	    else if(!IS_NPC(rch) && rch->group && rch->group->leader == ch){
		number_member++;
		class_array[rch->class]++;
	    }
	    else if(rch->fighting && is_same_group(rch->fighting, ch))
		number_enemy++;
	}

	if(number_enemy >= number_member){
	    send_to_char("你的團員數量太少，不足以包圍敵軍！\n\r", ch);
	    return;
	}
	if(!(number_front = class_array[SHAMAN]+class_array[KNIGHT]+
	    class_array[WARLORD]+class_array[MARTIALMAN])){
	    send_to_char("你的前線團員數量太少，不足以包圍敵軍！\n\r", ch);
	    return;
	}
	if(!(number_back = class_array[NINJA]+class_array[RANGER])){
	    send_to_char("你所能支派的包抄人員太少，不足以包圍敵軍！\n\r", ch);
	    return;
	}

	//calculate chance.
	//basic chance 0~85%
	chance = UMIN((((sklv*sklv)<<1)*(number_member+number_back+number_front))/(number_enemy<<2), 85);
	//Adject 1~12%
	chance+= (get_curr_int(ch)+get_curr_wis(ch))>>2;
	//for per mage or cleric, get -4%
	chance-= (class_array[MAGE]+class_array[WIZARD]+class_array[ALCHEMIST]+ class_array[SAVANT]+class_array[BISHOP])<<2;
	//Adject by sector of room and class.
	switch(sector_type){
	    case SECT_INSIDE:
		chance+=(class_array[NINJA]*10+class_array[MONK]*5+class_array[ASSASSIN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[RANGER]);
		break;
	    case SECT_CITY:
		chance+=(class_array[SWORDSMAN]*10+class_array[RANGER]*5+class_array[WARLORD]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[ASSASSIN]);
		break;
	    case SECT_FIELD:
		chance+=(class_array[KNIGHT]*10+class_array[WARLORD]*5+class_array[MARTIALMAN]*5);
		chance-=((class_array[ASSASSIN]<<1)+class_array[NINJA]);
		break;
	    case SECT_FOREST:
		chance+=(class_array[RANGER]*10+class_array[WARLORD]*5+class_array[SHAMAN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[MARTIALMAN]);
		break;
	    case SECT_HILLS:
		chance+=(class_array[WARLORD]*10+class_array[MONK]*5+class_array[MARTIALMAN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[NINJA]);
		break;
	    case SECT_MOUNTAIN:
		chance+=(class_array[WARLORD]*10+class_array[SHAMAN]*5+class_array[MARTIALMAN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[NINJA]);
		break;
	    case SECT_WATER_SWIM:
	    case SECT_WATER_NOSWIM:
	    case SECT_UNDERWATER:
		chance+=(class_array[MONK]*10+class_array[NINJA]*5+class_array[ASSASSIN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[LORD]);
		break;
	    case SECT_AIR:
		chance+=(class_array[NINJA]*10+class_array[SHAMAN]*5+class_array[ASSASSIN]*5);
		chance-=((class_array[WARLORD]<<1)+class_array[MONK]);
		break;
	    case SECT_DESERT:
		chance+=(class_array[WARLORD]*10+class_array[MONK]*5+class_array[KNIGHT]*5);
		chance-=((class_array[RANGER]<<1)+class_array[ASSASSIN]);
		break;
	}

	//final chance from 12.5% to 100%
	chance = URANGE((get_curr_wis(ch)+get_curr_int(ch))>>1,chance,100);
	if(number_percent() > chance){
	    send_to_char("你包圍敵軍的企圖被識破！\n\r",ch);
	    return;
	}
	send_to_char("你成功\地包圍敵軍！\n\r",ch);
	act("$n成功\地指揮，合圍包抄敵軍！",ch, NULL, NULL, TO_ROOM);

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(rch->deleted)continue;
	    if(IS_NPC(rch) && rch->fighting && is_same_group(rch->fighting, ch)){
		affect_strip( rch, gsn_snare );
		af.type      = gsn_snare;
		af.duration  = 1 + sklv;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_HOLD;
		affect_to_char( rch, &af );
		send_to_char("你被敵人包圍了！\n\r", rch);
		act("你順利包圍$N！",ch, NULL, rch, TO_CHAR);
		mprog_hurt_trigger(rch, ch, gsn_snare);
	    }
	}
	WAIT_STATE( ch, skill_table[gsn_snare].beats<<1 );
}

