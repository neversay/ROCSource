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
	// �� mystic armor ���@�έt�� 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
		if ( sklv + old_saves_spell( ch->pp/2, victim, MAGIC_COLD ) > (number_percent( ) * 100 / weakness_percent( victim, sn)) )
		{
			int ChillDR = (get_damroll(victim, WEAR_WIELD) - affected_value(victim,sn,APPLY_DAMROLL))/(16 - sklv);
			af.type     = sn;
			//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// �ק�@�ήɶ� 2020/11/06
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
				act( HIC "�զ⪺�����л\\��"NOR"$N"HIC"���C�T�ֽ��A�O�L�����餣�_Ÿ�ݡC"NOR"", ch, NULL, victim, TO_CHAR );
				act( HIC "���ϩ����F�A���֦סA�방���H�N���ĸ����I"NOR"" , ch, NULL, victim, TO_VICT);
				act( HIC "�զ⪺�����л\\��"NOR"$N"HIC"���C�T�ֽ��A�O�L�����餣�_Ÿ�ݡC"NOR"", ch, NULL, victim, TO_NOTVICT );
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

    send_to_char( HIC "�A���W�F�����A�·t���ϩ��X�{�F�B���k���A�����H�ۦo���R�ЦӴ��_�I\n\r"NOR"", ch );
    act(  "$n "HIC"���W�����A�P��Ů�ū��J���A���t�]�V�ӶV�֡A�����Ʀ��F�j�P���B�P���ɡI"NOR"", ch, NULL, NULL, TO_ROOM );    
    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
	victim_next = victim->next_in_room;
	if ( is_same_group( ch, victim ) )
	{
		continue;
	}
	if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
	        continue;
	// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
	if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
	{
		act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
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
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;

			af.type      = sn;
			af.bitvector = 0;
			//af.duration  = 2 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// �ק�@�ήɶ� 2020/11/06
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
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;

			af.type      = sn;
			af.bitvector = 0;
			//af.duration  = 2 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// �ק�@�ήɶ� 2020/11/06
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
		// npc �� del_qmark �����ϥ� mpdel_qmark 2022/04/14
		if( !IS_NPC(ch) )
			do_del_qmark(ch, buf);
		else
			do_mpdel_qmark(ch, buf);
	}

	if( !(qm = is_qmark(ch, "cast_accu_mark"))){
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		act( GRN "$n �P��v���Φ��@�βH�Ŧ⪺���w�I"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("�A�}�l�����믫....\n\r", ch);
		do_add_qmark(ch, buf);
		return;
	}
	else if( qm->v0 != sn ) {
		qm->v0 = sn;
		qm->v1 = 1;
		act( "$n �P�򪺥��w�v�����h..."NOR"", ch, NULL, NULL, TO_ROOM );
		act( GRN "$n �P��v���Φ��@�βH�Ŧ⪺���w�I"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("�A����F�즳���G��A���s�}�l�����믫....\n\r", ch);
		return;
	}
	else if( qm->v1 < 2 - sklv/6) {
		++qm->v1;
		act( YEL "$n �P�򪺲H�Ŧ⪺���w�U�ӷU�@�I"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("�A���믫�v�����E....\n\r", ch);
		return;
	}
	send_to_char("�A����X�������]�k��q�I\n\r", ch);
	act( HIC "$n �P���Ŧ���w�������᭸�V�ѪšI"NOR"", ch, NULL, victim, TO_ROOM );
	act( ""HIC"�b�A���I�ꤧ�U�A���j���B���k�P�ۻ��������議�{�A��"NOR"$N"HIC"���h�I"NOR"", 
	     ch, NULL, victim, TO_CHAR );
	act( ""HIC"���j���B���k�P�į}���Ū����h�A����"NOR"$n"HIC"���R�O��"NOR"$N"HIC"�e�t���h�I"NOR"" , 
	     ch, NULL, victim, TO_NOTVICT);
	act( ""HIC"�A���쥨�j���B���k�P���a�۱j�j����աA�������ʦa���A����V�����ĨӡI"NOR"", 
	     ch, NULL, victim, TO_VICT );	

	sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
	// npc �� del_qmark �����ϥ� mpdel_qmark 2022/04/14
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
