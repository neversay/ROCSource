/* $Id: spell_ice.c 916 2005-05-10 03:56:16Z zcecil $ */
#include "skill.h"
extern bool IsPK(CHAR_DATA *ch, CHAR_DATA *victim);

// chill touch modified by whisperwind 8.23.2004
void spell_chill_touch( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	int    sklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

	dam = dam * int_effect( ch );

	// weather effect
	if( weather_info.sky == SKY_SNOWING )
		dam = dam * 1.05;
	if( weather_info.sky == SKY_SNOWSTORM )
		dam = dam * 1.1;
	
	// sklv effect
	if( sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;

	if( sklv > 2 )
		dam *= 1.08;
	if( sklv > 3 )
		dam *= 1.08;
	if( sklv > 4 )
		dam *= 1.08;
	if( sklv > 5 )
		dam *= 1.08;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 2 , dam * 3 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1 , dam * 1.5 );

	if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
		return;
	// 有 mystic armor 不作用負面 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
		if ( sklv + old_saves_spell( ch->pp/2, victim, MAGIC_COLD ) > (number_percent( ) * 100 / weakness_percent( victim, sn)) )
		{
			int ChillDR = (get_damroll(victim, WEAR_WIELD) - affected_value(victim,sn,APPLY_DAMROLL))/(16 - sklv);
			af.type     = sn;
			//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// 修改作用時間 2020/11/06
			//if(IsPK(ch, victim)) af.duration /= 2;
			af.bitvector = 0;
			if( ( ChillDR + affected_value(victim,sn,APPLY_DAMROLL) ) > 0 ){
				if(affected_value(victim,sn,APPLY_DAMROLL) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 3;
				}
				af.location  = APPLY_DAMROLL;
				af.modifier  = -ChillDR/2;
				affect_join( victim, &af );
				act( HIC "白色的薄霜覆蓋\住"NOR"$N"HIC"的每吋皮膚，令他的身體不斷顫抖。"NOR"", ch, NULL, victim, TO_CHAR );
				act( HIC "凍氣彷彿刺穿了你的皮肉，刺骨的寒意直衝腦門！"NOR"" , ch, NULL, victim, TO_VICT);
				act( HIC "白色的薄霜覆蓋\住"NOR"$N"HIC"的每吋皮膚，令他的身體不斷顫抖。"NOR"", ch, NULL, victim, TO_NOTVICT );
			}
			if( sklv > 4 && affected_value(victim,sn,APPLY_ADAMAGE) > 0 - (sklv - 4) * 25) {
				if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 3;
				}
				af.location  = APPLY_ADAMAGE;
				af.modifier  = -((sklv - 4) * 25);
				affect_join( victim, &af );
			}
		}
	}

	return;
}

void spell_colour_spray( int sn, int level, CHAR_DATA *ch, void *vo )
{
 	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}
	
	dam = dam * int_effect( ch );
	
	// weather effect
	if( weather_info.sky == SKY_SNOWING )
		dam = dam * 1.05;
	if( weather_info.sky == SKY_SNOWSTORM )
		dam = dam * 1.1;

	// sklv effect
	if( sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;

	if( sklv > 2 )
	  dam = dam * 1.08;
	if( sklv > 3 )
	  dam = dam * 1.08;
	if( sklv > 4 )
	  dam = dam * 1.08;
	if( sklv > 5 )
	  dam = dam * 1.08;
		
  dam = dam * weakness_damage( victim, sn ) / 100;
  dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
        
	if( !IS_NPC(ch) )
		dam = number_range( dam * 4 , dam * 4.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.5 );

      magic_damage( ch, victim, dam, sn, WEAR_NONE );

  return;
}

// agglomerate descant add by whisperwind 10.31.2004
void spell_agglomerate_descant ( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
 	
    int    sklv = get_skill_level(ch,sn);
    int    dam;
	int    obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

    send_to_char( HIC "你閉上了眼睛，黑暗中彷彿出現了冰雪女神，風雪隨著她的舞蹈而揚起！\n\r"NOR"", ch );
    act(  "$n "HIC"閉上眼睛，周圍空氣溫度驟降，風速也越來越快，瞬間化成了強烈的冰嵐風暴！"NOR"", ch, NULL, NULL, TO_ROOM );    
    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
	victim_next = victim->next_in_room;
	if ( is_same_group( ch, victim ) )
	{
		continue;
	}
	if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
	        continue;
	// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
	if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
	{
		act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
		continue;
	}
	if ( IS_NPC( ch ) )
	{
		if ( victim->fighting == ch || !IS_NPC( victim ) )
       	{
			dam = 0;
			dam = (40 + ch->level)/2 ;
			dam = dam * int_effect( ch );
				
			if( sklv > 0 )
				dam = dam * ( 10 + sklv) / 10 ;
			if( sklv > 3 )
	        		dam *= 1.1;
			if( sklv > 5 )
	        		dam *= 1.1;

        		dam = dam * weakness_damage( victim, sn ) / 100;
        		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
			dam = number_range( dam * 6 , dam * 8 );
			
	                if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
				continue;
            if (is_affected( victim, skill_lookup("prot. from cold")))
	    		continue;
			// 有 mystic armor 不作用負面 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;

			af.type      = sn;
			af.bitvector = 0;
			//af.duration  = 2 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// 修改作用時間 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_COLD) < (affected_value(victim,sn,APPLY_SAVING_COLD) - NEW_GET_SVS(victim, MAGIC_COLD) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_COLD) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_COLD;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
			
			if( affected_value(victim,sn,APPLY_STR) > -3 ){
				if(affected_value(victim,sn,APPLY_STR) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_STR;
				af.modifier  = -1;
				affect_join( victim, &af );
			}

			if(affected_value(victim,sn,APPLY_DAMROLL) > -30 ){
				if(affected_value(victim,sn,APPLY_DAMROLL) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_DAMROLL;
				af.modifier  = -4 - sklv;
				affect_join( victim, &af );
			}

			if( affected_value(victim,sn,APPLY_ADAMAGE) > -100 ){
				if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_ADAMAGE;
				af.modifier  = -20 - sklv*5;
				affect_join( victim, &af );
			}
	    }
	}
	else if ( victim->fighting == ch ||
		  ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
       	{
			dam = 0;
			dam = (40 + ch->level)/2 ;
			dam = dam * int_effect( ch );
				
			if( sklv > 0 )
				dam = dam * ( 10 + sklv) / 10 ;
			if( sklv > 3 )
	        		dam *= 1.1;
			if( sklv > 5 )
	        		dam *= 1.1;

        		dam = dam * weakness_damage( victim, sn ) / 100;
        		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
			dam = number_range( dam * 6 , dam * 8 );
			
	                if(magic_damage( ch, victim, dam, sn, WEAR_NONE ))
				continue;

            if (is_affected( victim, skill_lookup("prot. from cold")))
	    		continue;
			// 有 mystic armor 不作用負面 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;

			af.type      = sn;
			af.bitvector = 0;
			//af.duration  = 2 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// 修改作用時間 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_COLD) < (affected_value(victim,sn,APPLY_SAVING_COLD) - NEW_GET_SVS(victim, MAGIC_COLD) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_COLD) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_COLD;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
			
			if( affected_value(victim,sn,APPLY_STR) > -3 ){
				if(affected_value(victim,sn,APPLY_STR) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_STR;
				af.modifier  = -1;
				affect_join( victim, &af );
			}

			if(affected_value(victim,sn,APPLY_DAMROLL) > -30 ){
				if(affected_value(victim,sn,APPLY_DAMROLL) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_DAMROLL;
				af.modifier  = -4 - sklv;
				affect_join( victim, &af );
			}

			if( affected_value(victim,sn,APPLY_ADAMAGE) > -100 ){
				if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_ADAMAGE;
				af.modifier  = -20 - sklv*5;
				affect_join( victim, &af );
			}
       	}
    }
}

// lore of frost add by whisperwind 11.26.2004
void spell_lore_of_frost ( int sn, int level, CHAR_DATA *ch, void *vo)
{
 	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv = get_skill_level(ch,sn);
	double dam ;
	int    obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}
	
	
	dam = dam * int_effect( ch );
	
	// weather effect
	if( weather_info.sky == SKY_SNOWING )
		dam = dam * 1.1;
	if( weather_info.sky == SKY_SNOWSTORM )
		dam = dam * 1.3;

	if( sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;

	if( sklv > 2 )
		dam *= 1.1;		
	if( sklv > 3 )
		dam *= 1.1;
	if( sklv > 4 )
		dam *= 1.1;
	if( sklv > 5 )
		dam *= 1.1;
		
	dam = dam * weakness_damage( victim, sn ) / 100;
	if( victim->saving_throw[MAGIC_COLD] < victim->saving_throw[MAGIC_WIND] )
		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
	else
		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
        
	if( !IS_NPC(ch) )
		dam = number_range( dam * 3 , dam * 5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.5 );
	
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}


void spell_ice_comet( int sn, int level, CHAR_DATA *ch, void *vo )
{
	QMARK *qm;
	char buf[MAX_INPUT_LENGTH];
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int sklv    = get_skill_level(ch,sn);
	int nCPSklv = get_skill_level(ch, skill_lookup("colour spray"));
	double dam ;
	int    obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

	if( !ch->in_room ) return;
	if( !ch->fighting && victim && !victim->deleted) {
		set_fighting(ch, victim);
		if(!victim->fighting) set_fighting(victim, ch);
	}
	if( !IS_AFFECTED(ch, AFF_CAST_ACC) ) {
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		SET_BIT(ch->affected_by, AFF_CAST_ACC);
		// npc 的 del_qmark 必須使用 mpdel_qmark 2022/04/14
		if( !IS_NPC(ch) )
			do_del_qmark(ch, buf);
		else
			do_mpdel_qmark(ch, buf);
	}

	if( !(qm = is_qmark(ch, "cast_accu_mark"))){
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		act( GRN "$n 周圍逐漸形成一團淡藍色的光暈！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你開始集中精神....\n\r", ch);
		do_add_qmark(ch, buf);
		return;
	}
	else if( qm->v0 != sn ) {
		qm->v0 = sn;
		qm->v1 = 1;
		act( "$n 周圍的光暈逐漸散去..."NOR"", ch, NULL, NULL, TO_ROOM );
		act( GRN "$n 周圍逐漸形成一團淡藍色的光暈！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你釋放了原有的咒文，重新開始集中精神....\n\r", ch);
		return;
	}
	else if( qm->v1 < 2 - sklv/6) {
		++qm->v1;
		act( YEL "$n 周圍的淡藍色的光暈愈來愈濃！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你的精神逐漸凝聚....\n\r", ch);
		return;
	}
	send_to_char("你釋放出全部的魔法能量！\n\r", ch);
	act( HIC "$n 周圍的藍色光暈凝結之後飛向天空！"NOR"", ch, NULL, victim, TO_ROOM );
	act( ""HIC"在你的呼喚之下，巨大的冰之彗星自遙遠的彼方降臨，朝"NOR"$N"HIC"飛去！"NOR"", 
	     ch, NULL, victim, TO_CHAR );
	act( ""HIC"巨大的冰之彗星衝破高空的雲層，順著"NOR"$n"HIC"的命令往"NOR"$N"HIC"疾速飛去！"NOR"" , 
	     ch, NULL, victim, TO_NOTVICT);
	act( ""HIC"你見到巨大的冰之彗星挾帶著強大的氣勢，不偏不倚地往你的方向正面衝來！"NOR"", 
	     ch, NULL, victim, TO_VICT );	

	sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
	// npc 的 del_qmark 必須使用 mpdel_qmark 2022/04/14
	if( !IS_NPC(ch) )
		do_del_qmark(ch, buf);
	else
		do_mpdel_qmark(ch, buf);
	REMOVE_BIT(ch->affected_by, AFF_CAST_ACC);
	dam = dam * int_effect( ch );
	
	// weather effect
	if( weather_info.sky == SKY_SNOWING )
		dam = dam * 1.1;
	if( weather_info.sky == SKY_SNOWSTORM )
		dam = dam * 1.2;

	// sklv effect
	if( sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;

	if( sklv > 2 )
			dam = dam * 1.1;	
	if( sklv > 3 )
			dam = dam * 1.1;
	if( sklv > 4 )
			dam = dam * 1.2;
	if( sklv > 5 )
		dam = dam * 1.25;
	
	if(!IS_NPC(ch)) dam = dam * (180 + nCPSklv * (nCPSklv + 4)) / 180;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
	if( !IS_NPC(ch) )
		dam = number_range( dam * 12 , dam * 13 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 6 , dam * 8 );

	magic_damage( ch, victim, dam, sn, WEAR_NONE );
	if( sklv > 5 ) {
		WAIT_MAGIC_STATE(victim, sklv*2);
	}
}
// modified by whisperwind 8.24.2004
