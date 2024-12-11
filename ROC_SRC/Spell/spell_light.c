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
		send_to_char( "�A�����F�L�k���E���t���O�q.\n\r", ch );
		return; 
	}

	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam /= 3; }
  
	if ( IS_GOOD( victim ) )
	{
		act( "�@�ѯ��t���O�qŢ�n�F $N.", ch, NULL, victim, TO_ROOM );
		act( "$N �ݨӧ��������v�T.", ch, NULL, victim, TO_CHAR );
		return;
	}

	dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
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
    
    send_to_char( HIW "�A�P����F�@���ũ��A�@���������P������G���Ȯɮ����F�I\n\r"NOR"", ch );
    act(  "$n "HIW"��X���t��ë��A���e���L�ϩ��Ʀ��F���j���Q�r�[�A���o�X���i�I�Ǫ��t���I"NOR"", ch, NULL, NULL, TO_ROOM );
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
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			// �a���t���N�B�~��K 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(victim, MAGIC_SAINT) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(victim, MAGIC_SAINT) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
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
			// �ק�@�ήɶ�, �ϧܩʻP�t�[������ 2020/11/06
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
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			// �a���t���N�B�~��K 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(victim, MAGIC_SAINT) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(victim, MAGIC_SAINT) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
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
			// �ק�@�ήɶ�, �ϧܩʻP�t�[������ 2020/11/06
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
		send_to_char( "�A�����F�L�k���E���t���O�q.\n\r", ch );
		return; 
	}
	
	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam = dam/3; 
	}
	
	if( !IS_NPC( ch ) && ch->class == BISHOP )
		dam = UMAX(dam * int_effect( ch ) , dam * wis_effect( ch ) );
	else
		dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
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
		 IS_AFFECTED( victim, AFF_GHOUL ) || // aff_ghoul ���P�� race �ܬ� undead 2022/01/16
       is_affected(victim, gsn_stone_form))
	{
		// �� mystic armor ���@�έt�� 2020/10/26
		if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
			/*if ( nSklv >= 4 && affected_value(victim,sn,APPLY_SAVING_SAINT) < 30 && nSklv*2 + old_saves_spell( ch->pp, victim, MAGIC_SAINT ) 
				 > (number_percent( ) * 100 / weakness_percent( victim, sn)) )
			*/
			nChance = nSklv * 2 + old_saves_spell( ch->pp, victim, MAGIC_SAINT );
			nChance = nChance * weakness_percent( victim, sn ) / 100;
			// �a���t���N�B�~��K 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(victim, MAGIC_SAINT) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(victim, MAGIC_SAINT) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
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
					act( ""HIW"�A�����t�O�q��"NOR"$N"HIW"�����c�𮧫���a�d�z�I"NOR"", ch, NULL, victim, TO_CHAR );
					act( "$n"HIW"�����t�O�q���A���W�����c�𮧺��������I"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$n"HIW"�����t�O�q��"NOR"$N"HIW"�����c�𮧫���a�d�z�I"NOR"", ch, NULL, victim, TO_NOTVICT );    
				}
			}
		}
		int nRaceSklv = get_skill_level(victim, gsn_inborn_ability);
		dam = dam * (100 - nRaceSklv * (nRaceSklv + 4) * 2 / 3) / 100;
		magic__damage( ch, victim, dam, sn, WEAR_NONE );
	}else{
		send_to_char("�A�u��W�礣���t�ͪ�.\n\r", ch);
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
    act("$n"HIW"�ۥX���t���q���A�b�ƤF$N�C"NOR"", ch, NULL, victim, TO_NOTVICT);
    act(""HIW"�A�ۥX���t���q���A�b�ƤF"NOR"$N"HIW"�C"NOR"", ch, NULL, victim, TO_CHAR);
    act("$n"HIW"�ۥX���t���q���A�b�ƤF�A�C"NOR"", ch, NULL, victim, TO_VICT);

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
            send_to_char( "�A�����F�L�k���E���t���O�q.\n\r", ch );
            return;
        }
    }

    if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
    {
       send_to_char( "�o�̸T��԰�. \n\r",ch);
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

    send_to_char( "�A�ǵ۪k�O�W�T�������G�סI\n\r", ch );
    act( "$n ���Ⱚ�|�j�n�@�ܡA�|�P�����u��Mģ���o�i���}�����I", ch, NULL, NULL, TO_ROOM );

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
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, vch, TO_CHAR);
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
				send_to_char( HIW"�������O�q�i�X�@������A�u�@�A����ı�����o�i���z���Iŧ�I"NOR"\n\r", vch );
				continue;
			}

			nChance = new_saves_spell( ch->pp, vch, MAGIC_SAINT) + WIS_DIFF( ch, vch ) + nSklv * 3;
			nChance = nChance * weakness_percent(vch, sn)/100;

			// �a���t���N�B�~��K 2022/10/26
			if( ( IS_AFFECTED( vch, AFF_SANCTUARY ) || IS_SET(race_table[vch->race].race_abilities, RACE_SANCT) )
				&& number_percent() < (0 - NEW_GET_SVS(vch, MAGIC_SAINT) ) )
			{
				if( is_affected( vch, gsn_sanctuary) && number_percent() > (0 - NEW_GET_SVS(vch, MAGIC_SAINT) )/2 ){
					affect_strip( vch, gsn_sanctuary );
					send_to_char( "�A���t���N�Q�}�a�F.\n\r", vch );
				}
				nChance = nChance + NEW_GET_SVS(vch, MAGIC_SAINT) - NEW_GET_SVS(ch, MAGIC_SAINT);
			}

			if( nChance > ( number_percent( ) + 2 ) )
			{
				  dam = dam * wis_effect( ch );
				  
				// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
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
				// �� mystic armor ���@�έt�� 2020/10/26
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

					act( "$N "HIG"��M���h�F���O!"NOR"", ch, NULL, vch, TO_CHAR  );
					send_to_char( "�A�����e�@�G�A�@�ɳ��ܶ¤F�I\n\r", vch );
					act( "$N "HIG"��M���h�F���O!"NOR"", ch, NULL, vch, TO_NOTVICT );
				}else if(sn == gsn_virtual_swords && nWillFort < 0){
					send_to_char(HIC"���F���پ��ϧA�K�����۳N��ı���ˮ`"NOR"\n\r", vch);
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
	        send_to_char( "�A�����F�L�k���E���t���O�q.\n\r", ch );
        	return; }

		/* remove neutral damage limit 18/6/13
		if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
			dam = dam/2;
		}
		*/			

	dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
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
		// mass turn �] obj_cast_spell �� level URANGE ���@�� �ҥH�N�J obj lv ���
		obj_lv = obj_lv/1000;
		sklv = 0;
	}else{
		obj_lv = 0;
	}
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		race_ib = get_skill_level(ch, gsn_inborn_ability);

    if( !ch->in_room ) return;

	//if( !IS_NPC(ch) && sklv < 6) {
	// �P�w���B�~ obj lv ��I�k���󪺼v�T( weapon master , wand , staves , scroll) add at 2020/10/14
	if( !IS_NPC(ch) && sklv < 6 && obj_lv < 60) {
		// inborn DETECT_ALIGN ���B�~�I�k����v�T 2020/05/29
		if ( race_ib*10 + obj_lv + sklv*10 < 60 ){
			for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room){
				if( IS_NPC(ch) ) continue;
				if( !is_same_group( ch, victim ) || ch == victim ) continue;
				if( victim -> class == SAVANT ) break;
			}
			if( !victim ) {
				send_to_char("�A�䤣���̡C\n\r", ch);
				return;
			}
			if( victim->mana < 100 ) {
				send_to_char("�A���٦�L�k�t�X�A�C\n\r", ch);
				return;
			}
			victim->mana -= 100;
		}
	}

    send_to_char( HIW "�A�q�������Ѱ�l��F�t���ƥ��I\n\r"NOR"", ch );
    act( HIW "$n �q�������Ѱ�l��F�t���ƥ��I"NOR"", ch, NULL, NULL, TO_ROOM );
    for( victim = ch->in_room->people ; victim ; victim = next_victim ){
         next_victim = victim->next_in_room;
        if( is_same_group(ch, victim)) continue;
        if( victim->deleted ) continue;
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
			continue;
		}
		// �[�J storm_form �P�w �� aff_ghoul ���P�w 2022/01/16
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
