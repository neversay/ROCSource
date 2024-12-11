/* $Id: spell_light.c 916 2005-05-10 03:56:16Z zcecil $ */
#include "skill.h"
extern int GetWillFortressEffect(CHAR_DATA *rch);


// dispel evil modified by whisperwind 8.26.2004
void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
 	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv   = get_skill_level(ch,sn);
	double dam;
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

	int ali_diff =0;

	if( victim ) ali_diff = ch->alignment - victim->alignment;
  
	if ( !IS_NPC( ch ) && IS_EVIL( ch )) {
		send_to_char( "你的心靈無法凝聚神聖的力量.\n\r", ch );
		return; 
	}

	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam /= 3; }
  
	if ( IS_GOOD( victim ) )
	{
		act( "一股神聖的力量籠罩了 $N.", ch, NULL, victim, TO_ROOM );
		act( "$N 看來完全不受影響.", ch, NULL, victim, TO_CHAR );
		return;
	}

	dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN 的 race (fairy , gnome) 有額外加成 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

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
	dam = dam * ( old_saves_spell( ch->pp, victim,MAGIC_SAINT)) / 60;
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.5 , dam * 3.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1 , dam * 2 );

	dam += ali_diff / 10;

	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}

// holy judgement add by whisperwind 10.31.2004
void spell_holy_judgement ( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
 	
    int sklv = get_skill_level(ch,sn);
    int        dam;
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
    
    send_to_char( HIW "你感到心靈一片空明，一切的邪念與雜思似乎都暫時消失了！\n\r"NOR"", ch );
    act(  "$n "HIW"唸出神聖的禱文，眼前的他彷彿化成了巨大的十字架，散發出不可侵犯的聖光！"NOR"", ch, NULL, NULL, TO_ROOM );
    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
	victim_next = victim->next_in_room;
	if ( is_same_group( ch, victim ) )
	{
		continue;
	}
	if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
	        continue;
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
			if( sklv > 4 )
	        		dam *= 1.1;
			if( sklv > 5 )
	        		dam *= 1.1;

        		dam = dam * weakness_damage( victim, sn ) / 100;
        		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SAINT)) / 100;
			dam = number_range( dam * 8 , dam * 10 );
			
			if(magic_damage( ch, victim, dam, sn, WEAR_NONE ))
				continue;
			// 有 mystic armor 不作用負面 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			// 帶有聖光術額外減免 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(victim, MAGIC_SAINT) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(victim, MAGIC_SAINT) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "你的聖光術被破壞了.\n\r", victim );
				}
				continue;
			}
			/*
			af.type      = sn;
			af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			af.bitvector = 0;
			af.location  = APPLY_SAVING_SAINT;
			af.modifier  = 3 + sklv + sklv/3;
			affect_join( victim, &af );
	    	*/
			af.type      = sn;
            af.bitvector = 0;
            //af.type      = sn;
            //af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
			// 修改作用時間, 使抗性與負加成相關 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_SAINT) < (affected_value(victim,sn,APPLY_SAVING_SAINT) - NEW_GET_SVS(victim, MAGIC_SAINT) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_SAINT) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_SAINT;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
			victim->mana = UMAX( 0 , victim->mana - victim->max_mana / 5);
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
			if( sklv > 4 )
	        		dam *= 1.1;
			if( sklv > 5 )
	        		dam *= 1.1;

        		dam = dam * weakness_damage( victim, sn ) / 100;
        		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SAINT)) / 100;
			dam = number_range( dam * 8 , dam * 10 );
			
			if(magic_damage( ch, victim, dam, sn, WEAR_NONE ))
				continue;
			// 有 mystic armor 不作用負面 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			// 帶有聖光術額外減免 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(victim, MAGIC_SAINT) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(victim, MAGIC_SAINT) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "你的聖光術被破壞了.\n\r", victim );
				}
				continue;
			}
			/*
			af.type      = sn;
			af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			af.bitvector = 0;
			af.location  = APPLY_SAVING_SAINT;
			af.modifier  = 3 + sklv + sklv/3;
			affect_join( victim, &af );
			*/
			af.type      = sn;
            af.bitvector = 0;
            //af.type      = sn;
            //af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
			// 修改作用時間, 使抗性與負加成相關 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_SAINT) < (affected_value(victim,sn,APPLY_SAVING_SAINT) - NEW_GET_SVS(victim, MAGIC_SAINT) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_SAINT) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_SAINT;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
			victim->mana = UMAX( 0 , victim->mana - victim->max_mana / 5);
       	}
    }
}

// turn undead modified by whisperwind 8.23.2004
void spell_turn_undead( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	
	int ali_diff =0;
	int nSklv     = get_skill_level(ch,sn);
	int nCereSklv = get_skill_level(ch, gsn_burial_ceremony);
	int nTurnSklv = get_skill_level(ch, gsn_turn_undead);
	int nMassSklv = get_skill_level(ch, skill_lookup("mass turn"));
	int nChance = 0;
	double dam;
	int    obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}
	
	if( victim ) ali_diff = ch->alignment - victim->alignment;
	if ( !IS_NPC( ch ) && IS_EVIL( ch )) {
		send_to_char( "你的心靈無法凝聚神聖的力量.\n\r", ch );
		return; 
	}
	
	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam = dam/3; 
	}
	
	if( !IS_NPC( ch ) && ch->class == BISHOP )
		dam = UMAX(dam * int_effect( ch ) , dam * wis_effect( ch ) );
	else
		dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN 的 race (fairy , gnome) 有額外加成 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;
	
	if( nSklv > 0 )
		dam = dam * ( 10 + nSklv * (nSklv + 4) / 3) / 10 ;

	if( nSklv > 2 )
		dam = dam * 1.08;
	if( nSklv > 3 )
		dam = dam * 1.08;
	if( nSklv > 4 )
		dam = dam * 1.08;
	if( nSklv > 5 )
		dam = dam * 1.08;
	
	dam = dam * ( 18 + nCereSklv * 2 + nTurnSklv * 2 + nMassSklv * 2) / 18;
	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SAINT)) / 100;
	
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.25 , dam * 3.25 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.25 , dam * 1.75 );
	
	dam += ali_diff / 10;
	
	if ( IS_SET(  race_table[victim->race].race_abilities , RACE_UNDEAD) ||
	     IS_SET(  race_table[victim->race].race_abilities , RACE_VAMPIRE) ||
		 IS_AFFECTED( victim, AFF_GHOUL ) || // aff_ghoul 等同於 race 變為 undead 2022/01/16
       is_affected(victim, gsn_stone_form))
	{
		// 有 mystic armor 不作用負面 2020/10/26
		if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
			/*if ( nSklv >= 4 && affected_value(victim,sn,APPLY_SAVING_SAINT) < 30 && nSklv*2 + old_saves_spell( ch->pp, victim, MAGIC_SAINT ) 
				 > (number_percent( ) * 100 / weakness_percent( victim, sn)) )
			*/
			nChance = nSklv * 2 + old_saves_spell( ch->pp, victim, MAGIC_SAINT );
			nChance = nChance * weakness_percent( victim, sn ) / 100;
			// 帶有聖光術額外減免 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(victim, MAGIC_SAINT) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(victim, MAGIC_SAINT) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "你的聖光術被破壞了.\n\r", victim );
				}
				nChance = nChance + NEW_GET_SVS(victim, MAGIC_SAINT) - NEW_GET_SVS(ch, MAGIC_SAINT);
			}
			if( nChance > number_percent( ) )
			{
				if ( affected_value(victim,sn,APPLY_SAVING_SAINT) < nSklv * nSklv + nSklv*2 + 2)
				{
					af.type     = sn;
					af.duration  = 2;
					af.bitvector = 0;
					af.location  = APPLY_SAVING_SAINT;
					af.modifier  = (nSklv * nSklv + nSklv*2 + 2) / 5;
					affect_join( victim, &af );
					act( ""HIW"你的神聖力量讓"NOR"$N"HIW"的邪惡氣息持續地削弱！"NOR"", ch, NULL, victim, TO_CHAR );
					act( "$n"HIW"的神聖力量讓你身上的邪惡氣息漸漸消散！"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$n"HIW"的神聖力量讓"NOR"$N"HIW"的邪惡氣息持續地削弱！"NOR"", ch, NULL, victim, TO_NOTVICT );    
				}
			}
		}
		int nRaceSklv = get_skill_level(victim, gsn_inborn_ability);
		dam = dam * (100 - nRaceSklv * (nRaceSklv + 4) * 2 / 3) / 100;
		magic__damage( ch, victim, dam, sn, WEAR_NONE );
	}else{
		send_to_char("你只能超渡不死系生物.\n\r", ch);
	}
	return;
}

void spell_exorcise( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn) ;


	if( is_affected( victim, gsn_vampiric_bite ) ){
		affect_strip( victim, gsn_vampiric_bite );
	}

    act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
    act("$n"HIW"唱出神聖的歌曲，淨化了$N。"NOR"", ch, NULL, victim, TO_NOTVICT);
    act(""HIW"你唱出神聖的歌曲，淨化了"NOR"$N"HIW"。"NOR"", ch, NULL, victim, TO_CHAR);
    act("$n"HIW"唱出神聖的歌曲，淨化了你。"NOR"", ch, NULL, victim, TO_VICT);

    return;
}


void spell_sun_flash( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch, *vch_next;
    int dam;
    int nSklv    = get_skill_level( ch, sn);
    int nDmgRate = 100;
    int nWillFort= 0;
	int nChance = 0;
    AFFECT_DATA af;

    if(sn == gsn_sun_slash) nSklv = get_skill_level( ch, gsn_sun_flash);

    if ( !IS_NPC( ch ) && sn != gsn_virtual_swords)
    {
        if ( IS_EVIL( ch ) || IS_NEUTRAL( ch ) )
        {
            send_to_char( "你的心靈無法凝聚神聖的力量.\n\r", ch );
            return;
        }
    }

    if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
    {
       send_to_char( "這裡禁止戰鬥. \n\r",ch);
       return;
    }

    if ( !IS_OUTSIDE( ch ) && !IS_NPC( ch ) && sn != gsn_virtual_swords)
    {
        if( number_percent( ) > 40 + (nSklv * nSklv + 4))
        {
          nDmgRate = 75;
        }
    }

    if ( weather_info.sky >= SKY_RAINING && !IS_NPC( ch ) && sn != gsn_virtual_swords)
    {
        if(weather_info.sky != SKY_RAINING || nSklv < 6)
        {
          nDmgRate /= 2;
        }
    }

    send_to_char( "你藉著法力增幅陽光的亮度！\n\r", ch );
    act( "$n 雙手高舉大聲一喝，四周的光線突然耀眼得張不開眼睛！", ch, NULL, NULL, TO_ROOM );

    for ( vch = ch->in_room->people; vch; vch = vch_next )
    {
		int obj_lv = level;
		if( level >= 1000){
			obj_lv = obj_lv/1000;
			nSklv = 0;
			if( obj_lv >= 2)  nSklv = 1;
			if( obj_lv >= 10) nSklv = 2;
			if( obj_lv >= 22) nSklv = 3;
			if( obj_lv >= 40) nSklv = 4;
			if( obj_lv >= 61) nSklv = 5;
			if( obj_lv >= 90) nSklv = 6;
			dam  = (40 + obj_lv)/2;
		}else{
			dam  = (40 + level)/2;
		}
		vch_next = vch->next_in_room;
		if ( vch == ch || vch->deleted || vch->position == POS_DEAD ) continue;
		if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, vch, TO_CHAR);
			continue;
		}
		if ( !IS_NPC(vch) && is_affected(vch, gsn_invoke) && get_skill_level(vch, gsn_invoke) > 3 )
			continue;

		if( !is_same_group( ch, vch )
			&& ( ( IS_NPC( ch ) && ( vch->fighting == ch || !IS_NPC( vch ) ) )
				|| ( !IS_NPC( ch )
					&& ( vch->fighting == ch
						|| ( IS_NPC( vch ) && !IS_AFFECTED( vch, AFF_CHARM ) && !IS_AFFECTED( vch, AFF_SUMMONED ) ) ) ) ) )
		{
			if (affected_value( vch, skill_lookup("day of brightness"), APPLY_DOB_SAVING_FLASH) > number_percent( ) )
			{
				send_to_char( HIW"水神的力量張出一面光鏡，守護你的視覺不受這波光爆的侵襲！"NOR"\n\r", vch );
				continue;
			}

			nChance = new_saves_spell( ch->pp, vch, MAGIC_SAINT) + WIS_DIFF( ch, vch ) + nSklv * 3;
			nChance = nChance * weakness_percent(vch, sn)/100;

			// 帶有聖光術額外減免 2022/10/26
			if( ( IS_AFFECTED( vch, AFF_SANCTUARY ) || IS_SET(race_table[vch->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(vch, MAGIC_SAINT) ) )
			{
				if( is_affected( vch, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(vch, MAGIC_SAINT) )/2 ){
					affect_strip( vch, gsn_sanctuary );
					send_to_char( "你的聖光術被破壞了.\n\r", vch );
				}
				nChance = nChance + NEW_GET_SVS(vch, MAGIC_SAINT) - NEW_GET_SVS(ch, MAGIC_SAINT);
			}

			if( nChance > ( number_percent( ) + 2 ) )
			{
				  dam = dam * wis_effect( ch );
				  
				// inborn DETECT_ALIGN 的 race (fairy , gnome) 有額外加成 2020/05/29
				if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
					dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

				if( nSklv > 0 ) dam = dam * ( 10 + nSklv) / 10;

				if( nSklv > 2 ) dam = dam * 1.08;
				if( nSklv > 3 ) dam = dam * 1.08;
				if( nSklv > 4 ) dam = dam * 1.08;
				if( nSklv > 5 ) dam = dam * 1.08;

				if( !IS_NPC(ch) )
					dam = number_range( dam * 3 , dam * 4 );
				else if ( IS_NPC(ch) )
					dam = number_range( dam * 2 , dam * 3 );
					
				dam = dam * weakness_damage( vch, gsn_sun_flash ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, vch ,MAGIC_SAINT)) / 100;
				dam = dam * nDmgRate / 100;
				magic__damage( ch, vch, dam-vch->mdr, gsn_sun_flash, WEAR_NONE );

				if ( IS_AFFECTED( vch, AFF_BLIND ) || IS_SET( vch->special, NO_BLIND )
					 || number_percent() > nDmgRate * (10 + nSklv) / 20) continue;

				if ( sn == gsn_sun_slash && number_percent() > get_skill_level(ch, gsn_sun_slash) * 10 )
					continue;
				// 有 mystic armor 不作用負面 2020/10/26
				if( IS_AFFECTED(vch, AFF_MYSTIC_ARMOR ) )
					continue;

				nWillFort = GetWillFortressEffect(vch);
				if(sn != gsn_virtual_swords || number_percent() < 50 + nSklv * 12 - get_curr_wis(vch) + nWillFort){
					af.type       = gsn_sun_flash;
					af.duration  = 1 + nSklv/2;
					if(sn == gsn_sun_slash) af.duration /= 2;
					af.location  = APPLY_HITROLL;
					af.modifier  = -12 - nSklv*4;
					af.bitvector = AFF_BLIND;
					affect_to_char( vch, &af );

					act( "$N "HIG"突然失去了視力!"NOR"", ch, NULL, vch, TO_CHAR  );
					send_to_char( "你的眼前一亮，世界都變黑了！\n\r", vch );
					act( "$N "HIG"突然失去了視力!"NOR"", ch, NULL, vch, TO_NOTVICT );
				}else if(sn == gsn_virtual_swords && nWillFort < 0){
					send_to_char(HIC"心靈的障壁使你免於受到幻術視覺的傷害"NOR"\n\r", vch);
				}
			}else{
				magic__damage( ch, vch, 0, gsn_sun_flash, WEAR_NONE );
			}
		}
	}
	return;
}

void spell_flamestrike( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
	int sklv   = get_skill_level( ch, sn);
	int obj_lv = level;

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
    if( sn == gsn_flaming_armor ) {
        spell_flaming_armor_strike( sn, level, ch, vo );
        return;
    }

        if ( !IS_NPC( ch ) && IS_EVIL( ch )) {
	        send_to_char( "你的心靈無法凝聚神聖的力量.\n\r", ch );
        	return; }

		/* remove neutral damage limit 18/6/13
		if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
			dam = dam/2;
		}
		*/			

	dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN 的 race (fairy , gnome) 有額外加成 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

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
        dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SAINT)) / 100;
		//vch align effect 18/6/13
		dam = dam - dam * (victim->alignment - UMIN(2 * ch->amdr , 1000))/2000;
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2 , dam * 2.8 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam , dam * 1.2 );
        // Keric modified to magic_damage
	//dam -= victim->mdr;
	// dam/2 for balance magic spell.
    magic_damage( ch, victim, dam , sn, WEAR_NONE );

    return;
}

void spell_mass_turn( int sn, int level, CHAR_DATA *ch, void *vo )
{
  
    CHAR_DATA  *victim = (CHAR_DATA *) vo, *next_victim;
	
	int sklv   = get_skill_level( ch, sn);
	int obj_lv = level;
	int race_ib = 0;

	if( level >= 1000){
		// mass turn 跑 obj_cast_spell 的 level URANGE 不作用 所以代入 obj lv 實值
		obj_lv = obj_lv/1000;
		sklv = 0;
	}else{
		obj_lv = 0;
	}
	// inborn DETECT_ALIGN 的 race (fairy , gnome) 有額外加成 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		race_ib = get_skill_level(ch, gsn_inborn_ability);

    if( !ch->in_room ) return;

	//if( !IS_NPC(ch) && sklv < 6) {
	// 判定有額外 obj lv 對施法條件的影響( weapon master , wand , staves , scroll) add at 2020/10/14
	if( !IS_NPC(ch) && sklv < 6 && obj_lv < 60) {
		// inborn DETECT_ALIGN 的額外施法條件影響 2020/05/29
		if ( race_ib*10 + obj_lv + sklv*10 < 60 ){
			for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room){
				if( IS_NPC(ch) ) continue;
				if( !is_same_group( ch, victim ) || ch == victim ) continue;
				if( victim -> class == SAVANT ) break;
			}
			if( !victim ) {
				send_to_char("你找不到賢者。\n\r", ch);
				return;
			}
			if( victim->mana < 100 ) {
				send_to_char("你的夥伴無法配合你。\n\r", ch);
				return;
			}
			victim->mana -= 100;
		}
	}

    send_to_char( HIW "你從遙遠的天堂召喚了聖潔曙光！\n\r"NOR"", ch );
    act( HIW "$n 從遙遠的天堂召喚了聖潔曙光！"NOR"", ch, NULL, NULL, TO_ROOM );
    for( victim = ch->in_room->people ; victim ; victim = next_victim ){
         next_victim = victim->next_in_room;
        if( is_same_group(ch, victim)) continue;
        if( victim->deleted ) continue;
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
			continue;
		}
		// 加入 storm_form 判定 及 aff_ghoul 的判定 2022/01/16
        //if ( IS_SET(  race_table[victim->race].race_abilities , RACE_UNDEAD) ||
        //     IS_SET(  race_table[victim->race].race_abilities , RACE_VAMPIRE) )
		if( IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD) ||
			IS_SET( race_table[victim->race].race_abilities , RACE_VAMPIRE) ||
			IS_AFFECTED( victim, AFF_GHOUL ) ||
			is_affected( victim, gsn_stone_form) )
        {
            spell_turn_undead( sn, level, ch, (void*)victim);
        }
    }

    return;
}
