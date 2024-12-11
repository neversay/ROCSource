//*****************************************************************
//*  PROGRAM ID  : spell_flame.c                                  *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940622 Razgriz                                *
//*  PURPOSE     :         �ץ� spell_fireball() ���A             *
//*              :         �b magic_damage() ����A�[�J ch �O�_   *
//*              :         deleted ���P�_�A�H�קK for �j�骺�P�_  *
//*              :         �� ch->in_room �� NULL�A�ӳy�� crash.  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940903 Jye                                    *
//*  PURPOSE     :         �ץ� spell_phlogistic_verse() ���A     *
//*              :              day of protection�]�i��           *
//*****************************************************************
/* $Id: spell_flame.c 1809 2005-06-22 15:38:28Z akira $ */
#include "skill.h"
extern bool IsPK(CHAR_DATA *ch, CHAR_DATA *victim);

// burning hands modified by whisperwind 8.23.2004
void spell_burning_hands( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	int    sklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    nBurnRate;
	//int    nMaxAcDecay;
	int    nMaxSvfDecay;
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

	// sklv effect
	if( sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;
	if( sklv > 2 ) dam *= 1.08;
	if( sklv > 3 ) dam *= 1.08;
	if( sklv > 4 ) dam *= 1.08;
	if( sklv > 5 ) dam *= 1.08;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
        
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2 , dam * 3.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1 , dam * 1.75 );

	if(magic_damage( ch, victim, dam, sn, WEAR_NONE ) == -2) return;
	if(!victim || victim->deleted) return;

	nBurnRate =  weakness_percent( victim, sn) * (sklv * 2 + old_saves_spell( ch->pp/2, victim, MAGIC_FIRE ));
	// �� mystic armor ���@�έt�� 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
		if (nBurnRate > number_percent( ) * 100 ){
			//nMaxAcDecay = sklv * (sklv + 4) * 5;
			//if(!IS_NPC(ch)) nMaxAcDecay = nMaxAcDecay * new_saves_spell(ch->pp, victim, MAGIC_FIRE ) / 100;
			nMaxSvfDecay = (sklv - 4) * (sklv + 4);
			if(sklv > 4 && affected_value(victim,sn,APPLY_SAVING_FIRE) < nMaxSvfDecay ){
				af.type      = sn;
				//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
				// �ק�@�ήɶ� 2020/11/06
				if(is_affected(victim, sn )){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				if(IsPK(ch, victim)) af.duration /= 2;
				af.bitvector = 0;
				af.location  = APPLY_SAVING_FIRE;
				af.modifier  = (sklv - 4) + (sklv - 5) * 2;
				affect_join( victim, &af );
				act( "$N"HIR"�Q���V�����A���⪺�V��������¶�b�L�����W�C"NOR"", ch, NULL, victim, TO_CHAR );
				act( HIR "����P���⪺�K���P�ɩ��Aŧ�ӡI"NOR"" , ch, NULL, victim, TO_VICT);
				act( "$N"HIR"�Q���V�����A���⪺�V��������¶�b�L�����W�C"NOR"", ch, NULL, victim, TO_NOTVICT );
			}
		}
	}
	return;
}

void spell_fire_wall( int sn, int level, CHAR_DATA *ch, void *vo )
{
#ifdef HAS_ROOM_AFFECT
    R_AFFECT_DATA af;
    if( !ch->in_room ) return;
    if( !IS_IMMORTAL(ch)) return;
    return;

    if( IS_AFFECTED(ch->in_room, R_AFF_FIREWALL )){
	send_to_char("�o�̤w�g���@�D����F�C\n\r", ch);
	return;
    }

    af.type = sn;
    af.bitvector = R_AFF_FIREWALL;
    af.duration = number_fuzzy(level/5);
    af.modifier = number_fuzzy(get_curr_int(ch) + get_curr_wis(ch)) * 2;
    affect_to_room(ch->in_room, &af);

    send_to_char( "�A�b�o�̫إߤF�@�D����I\n\r", ch );
    act( "$n �b�o�̫إߤF�@�D����I", ch, NULL, NULL, TO_ROOM );
#endif
    return;
}


// phlogistic verse added by whisperwind 10.22.2004
void spell_phlogistic_verse ( int sn, int level, CHAR_DATA *ch, void *vo)
{	
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    OBJ_DATA  *obj_lose;
    OBJ_DATA  *obj_next;

    int sklv = get_skill_level(ch,sn);
    double        dam;
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

    send_to_char( HIR "�A���I��X�{�`���������A�j�j���P�K«�L�A�����ǡA�N�V�ؼСI\n\r"NOR"", ch );
    act(  "$n "HIR"�u�w�ۻ��j�֤�A���⪺���������X�{�b�L�I��A�äƦ��r�P���V���u���ӨӡI"NOR"", ch, NULL, NULL, TO_ROOM );
    
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
				dam  = (40 + ch->level)/2;
				dam = dam * int_effect( ch );

                if( sklv > 0 )
                    dam = dam * ( 10 + sklv) / 10 ;
                if( sklv > 3 )
                    dam *= 1.1;
                if( sklv > 5 )
                    dam *= 1.1;

                dam = dam * (weakness_damage( victim, sn ) / 100);
                dam = dam * (new_saves_spell( ch->pp, victim,MAGIC_FIRE))/100;
                dam = number_range( dam * 8 , dam * 10 );

                if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
		    continue;
		
                if (is_affected( victim, skill_lookup("prot. from fire")))
                    continue;
				// �� mystic armor ���@�έt�� 2020/10/26
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
					continue;
				af.type      = sn;
				af.bitvector = 0;
				//af.type      = sn;
				//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
				// �ק�@�ήɶ�, �ϧܩʻP�t�[������ 2020/11/06
				if( affected_value(victim,sn,APPLY_SAVING_FIRE) < (affected_value(victim,sn,APPLY_SAVING_FIRE) - NEW_GET_SVS(victim, MAGIC_FIRE) ) ){
					if(affected_value(victim,sn,APPLY_SAVING_FIRE) > 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_SAVING_FIRE;
					af.modifier  = 4 + sklv;
					affect_join( victim, &af );
				}


                //add by jye 0940903
                if (is_affected( victim, skill_lookup("day of protection")))
                    continue;
                for ( obj_lose = victim->carrying; 
                        obj_lose; 
                        obj_lose = obj_next )
                {
                    char *msg;

                    obj_next = obj_lose->next_content;
                    if ( obj_lose->deleted )
                        continue;
                    if ( IS_SET(obj_lose->pIndexData->souvenir, Z4) )
                        continue;
                    if ( IS_SET(obj_lose->extra_flags, ITEM_PERSONAL) )
                        continue;
                    if ( number_bits( 2 ) != 0 )
                        continue;
                    switch ( obj_lose->item_type )
                    {
                        default:		 
                            continue;
                        case ITEM_ARROW:         
                            msg = "$p ��M�_���U�N�I";    
                            break;
                        case ITEM_FOUNTAIN:      
                            msg = "$p �m�˦ӻ]�o�F�I";    
                            break; 
                        case ITEM_CORPSE_NPC:    
                            msg = "$p �N���@��J���I";    
                            break;
                        case ITEM_BOAT:          
                            msg = "$p �N���@��J���I";    
                            break;
                        case ITEM_DRINK_CON:     
                            msg = "$p �m�˦ӻ]�o�F�I";    
                            break;
                        case ITEM_TRASH:         
                            msg = "$p �N���@��J���I";    
                            break;
                        case ITEM_TREASURE:      
                            msg = "$p �N���@��J���I";    
                            break;
                        case ITEM_CONTAINER:     
                            msg = "$p ��M�_���U�N�I";    
                            //obj_to_room( obj, victim->in_room );
                            act( "$p �̭����F�豼�F�X�ӡI",	
                                    victim, obj_lose, NULL, TO_CHAR );
                            act( "$p �̭����F��q $n ���W���F�X�ӡI", 
                                    victim, obj_lose, NULL, TO_ROOM );
                            {
                                OBJ_DATA *obj_in, *obj_in_next;
                                for ( obj_in = obj_lose->contains; 
                                        obj_in; 
                                        obj_in = obj_in_next )
                                {
                                    obj_in_next = obj_in->next_content;
                                    if ( obj_in->deleted )
                                        continue;
                                    obj_from_obj( obj_in );
                                    obj_to_room( obj_in, ch->in_room );
                                }
                            }
	                    break;
                        case ITEM_POTION:	 
                            msg = "$p �m�˦ӻ]�o�F�I";    
                            break;
                        case ITEM_SCROLL:	 
                            msg = "$p �q��t�}�l���N�I";  
                            break;
                        case ITEM_STAFF:	 
                            msg = "$p �_�X�}�}�·ϡI";    
                            break;
                        case ITEM_WAND:	         
                            msg = "$p �q�X�I�I���P�I";    
                            break;
                        case ITEM_FOOD:	         
                            msg = "$p �o�X�J����I";      
                            break;
                        case ITEM_PILL:	         
                            msg = "$p �@�����ĤƤF�I";    
                            break;
                        case ITEM_FURNITURE:     
                            msg = "$p �N���@��J���I";    
                            break;
                    }

                    act( msg, victim, obj_lose, NULL, TO_CHAR );
                    extract_obj( obj_lose );
                }
            }
        }
        else if ( victim->fighting == ch ||
                ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
        {
            dam = (40 + ch->level)/2 ;
            dam = dam * int_effect( ch );

            if( sklv > 0 )
                dam = dam * ( 10 + sklv) / 10 ;
            if( sklv > 3 )
                dam = dam * 1.1;
            if( sklv > 5 )
                dam = dam * 1.1;

            dam = dam * weakness_damage( victim, sn ) / 100;
            dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
            dam = number_range( dam * 8 , dam * 10 );

            if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
				continue;
	
            if (is_affected( victim, skill_lookup("prot. from fire")))
                continue;
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			af.type      = sn;
            af.bitvector = 0;
            //af.type      = sn;
            //af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
			// �ק�@�ήɶ�, �ϧܩʻP�t�[������ 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_FIRE) < (affected_value(victim,sn,APPLY_SAVING_FIRE) - NEW_GET_SVS(victim, MAGIC_FIRE) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_FIRE) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_FIRE;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
            //add by jye 0940903
            if (is_affected( victim, skill_lookup("day of protection")))
                continue;

            for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next )
            {
                char *msg;

                obj_next = obj_lose->next_content;
                if ( obj_lose->deleted )
                    continue;
                if ( number_bits( 2 ) != 0 )
                    continue;
                switch ( obj_lose->item_type )
                {
                    default:		 
                        continue;
                    case ITEM_ARROW:        
                        msg = "$p ��M�_���U�N�I";    
                        break;
                    case ITEM_FOUNTAIN:      
                        msg = "$p �m�˦ӻ]�o�F�I";    
                        break; 
                    case ITEM_CORPSE_NPC:    
                        msg = "$p �N���@��J���I";    
                        break;
                    case ITEM_BOAT:          
                        msg = "$p �N���@��J���I";    
                        break;
                    case ITEM_DRINK_CON:     
                        msg = "$p �m�˦ӻ]�o�F�I";    
                        break;
                    case ITEM_TRASH:         
                        msg = "$p �N���@��J���I";    
                        break;
                    case ITEM_TREASURE:      
                        msg = "$p �N���@��J���I";    
                        break;
                    case ITEM_CONTAINER:     
                        msg = "$p ��M�_���U�N�I"; 
                        act( "$p �̭����F�豼�F�X�ӡI",	
                                victim, obj_lose, NULL, TO_CHAR );
                        act( "$p �̭����F��q $n ���W���F�X�ӡI", 
                                victim, obj_lose, NULL, TO_ROOM );
                        {
                            OBJ_DATA *obj_in, *obj_in_next;
                            for ( obj_in = obj_lose->contains; 
                                    obj_in; 
                                    obj_in = obj_in_next )
                            {
                                obj_in_next = obj_in->next_content;
                                if ( obj_in->deleted )
                                    continue;
                                obj_from_obj( obj_in );
                                obj_to_room( obj_in, ch->in_room );
                            }
                        }
                        break;
                    case ITEM_POTION:	 msg = "$p �m�˦ӻ]�o�F�I";    break;
                    case ITEM_SCROLL:	 msg = "$p �q��t�}�l���N�I";  break;
                    case ITEM_STAFF:	 msg = "$p �_�X�}�}�·ϡI";    break;
                    case ITEM_WAND:	         msg = "$p �q�X�I�I���P�I";    break;
                    case ITEM_FOOD:	         msg = "$p �o�X�J����I";      break;
                    case ITEM_PILL:	         msg = "$p �@�����ĤƤF�I";    break;
                    case ITEM_FURNITURE:     msg = "$p �N���@��J���I";    break;
                }

                act( msg, victim, obj_lose, NULL, TO_CHAR );
                /*if( obj_lose->item_type == ITEM_CONTAINER )
                  {
                  check_fire_container( obj_lose, victim );
                  }*/
                extract_obj( obj_lose );
            }
        }
    }
}

// song of firestorm add by whisperwind 11.26.2004
void spell_song_of_firestorm ( int sn, int level, CHAR_DATA *ch, void *vo)
{
 	CHAR_DATA *victim = (CHAR_DATA *) vo;

    int sklv = get_skill_level(ch,sn);
    double        dam;
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
        if( victim->saving_throw[MAGIC_FIRE] < victim->saving_throw[MAGIC_WIND] )
        	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
        else
        	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
        
	if( !IS_NPC(ch) )
		dam = number_range( dam * 3 , dam * 5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.5 );
	
        magic_damage( ch, victim, dam, sn, WEAR_NONE );

        return;
}

// fireball modified by whisperwind 9.18.2004
void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo )
{
 	CHAR_DATA *victim = (CHAR_DATA *) vo;
	CHAR_DATA *vch= (CHAR_DATA *) vo;

	int time = 0;
    int sklv = get_skill_level(ch,sn);
    double        dam;
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
	
/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"���y�Ĥ@�q level�G%f ,level=%d", dam, level);
    send_to_all_char( newbuf );
}*/
	dam = dam * int_effect( ch );

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
/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"���y�ĤG�q sklv�G%f sn=%d", dam,sn);
    send_to_all_char( newbuf );
}*/		
        dam = dam * weakness_damage( victim, sn ) / 100;
        dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;

/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"���y�ĤT�q pp svf�G%f pp-svf=%d ",dam, new_saves_spell( ch->pp, victim,MAGIC_FIRE));
    send_to_all_char( newbuf );
}*/
	if( !IS_NPC(ch) )
		dam = number_range( dam * 3 , dam * 4.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.25 );
/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"���y�ĥ|�q range�G%f", dam);
    send_to_all_char( newbuf );
}*/			
        magic_damage( ch, victim, dam, sn, WEAR_NONE );

	//Added by Razgriz	20050622
	if ( ch->deleted ) {
		logf_envy("spell_fireball: ch(%s) is deleted.", ch->name);
		return;
	}

        for( victim = ch->in_room->people; victim && time <= sklv;
    	victim = victim->next_in_room, time++ ) 
        {
		if( victim->deleted ) continue;
		if( victim->fighting != ch ) continue;
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
		    continue;
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
			continue;
		}
		if( victim == vch ) continue;
//		if( number_bits( 2 ) == 1 ) continue;		//25% misses the attack.


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
	        dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
		if( !IS_NPC(ch) )
			dam = number_range( dam * 1.5 , dam * 2 );
		else if ( IS_NPC(ch) )
			dam = number_range( dam * 0.75 , dam * 1.5 );
	
		magic_damage( ch, victim, dam, sn, WEAR_NONE );

		//Added by Razgriz	20050622
		if ( ch->deleted ) {
			logf_envy("spell_fireball: ch(%s) is deleted. (for_loop)", ch->name);
			break;
		}
        }
        return;
}

/* starfire spell added by Amenda 12/26/00 */
// modified by whisperwind 8.23.2004
void spell_starfire( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim    = (CHAR_DATA *) vo;
	int        nSFSklv = get_skill_level(ch, sn);
	int        nFBSklv = get_skill_level(ch, skill_lookup("fireball"));
	double     dam;
	int        obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSFSklv = 0;
		nFBSklv = 0;
		if( obj_lv >= 2)  nSFSklv = 1;
		if( obj_lv >= 10) nSFSklv = 2;
		if( obj_lv >= 22) nSFSklv = 3;
		if( obj_lv >= 40) nSFSklv = 4;
		if( obj_lv >= 61) nSFSklv = 5;
		if( obj_lv >= 90) nSFSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

	act( HIR "�@�����P�p�J�F" NOR "$N" HIR "�餺�A�b�ĤH�餺�ݻŦa�z�}�I" NOR "", ch, NULL, victim, TO_CHAR );
	act( HIR "�A�����驿�M�ǨӼ@�h�A���ۤ@�Ѩ`�������j��q�ѧA�餺���~�r�ġI"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$n" HIR "��@���Aģ�ت����P�N�����q" NOR "$N" HIR "���W«�X�I"NOR"", ch, NULL, victim, TO_NOTVICT );

	dam = dam * int_effect( ch );

	// sklv effect
	if( nSFSklv > 0 ) dam = dam * ( 10 + nSFSklv) / 10 ;
	if( nSFSklv > 2 ) dam *= 1.08;
	if( nSFSklv > 3 ) dam *= 1.11;
	if( nSFSklv > 4 ) dam *= 1.16;
	if( nSFSklv > 5 ) dam *= 1.2;
	if(!IS_NPC(ch))   dam = dam * (180 + nFBSklv * (nFBSklv + 4)) / 180;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
        
	if( !IS_NPC(ch) )
		dam = number_range( dam * 6 , dam * 7 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 4 , dam * 5 );

/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"�P���ĥ|�q range�G%f", dam);
    send_to_all_char( newbuf );
}*/
		
	// Keric modfied to magic_damage
        //dam -= victim->mdr;
	
        magic__damage( ch, victim, dam, sn, WEAR_NONE );

        return;
}

/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
void spell_high_explosive( int sn, int level, CHAR_DATA *ch, void *vo )
{
	QMARK *qm;
	char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim = (CHAR_DATA *) vo , *victim_next;
	int    sklv = get_skill_level (ch, sn);
	int    PBsklv = get_skill_level (ch, skill_lookup("powerball"));
	double dam; // basic value;
	int    obj_lv = level;
	// 2022/04/15
	CHAR_DATA *old_vch;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);
	int times = 0;
	int single_cost = 0;
	int total_cost = 0;
	int base_dam = 0;

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

	if( !IS_AFFECTED(ch, AFF_CAST_ACC) ) {
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		SET_BIT(ch->affected_by, AFF_CAST_ACC);
		// npc �� del_qmark �����ϥ� mpdel_qmark 2022/04/14
		if( !IS_NPC(ch) )
			do_del_qmark(ch, buf);
		else
			do_mpdel_qmark(ch, buf);
	}

	// �N high explosive �]�O���Ӽg��ޯ�U 2022/04/15
	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			single_cost = 90 - sklv - nSpellSklv;
		else
			single_cost = 150 - sklv - nSpellSklv;

		// instant ���ӼW�[
		if(ch->pcdata->autocombat == AUTOC_INSTANT)
			single_cost = single_cost + 10;

		if(ch->mana < single_cost){
			send_to_char( "�A���k�O�����F.\n\r", ch);
			return;
		}
	}

	if( !(qm = is_qmark(ch, "cast_accu_mark"))){
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		act( HIW "$n ���ض����믫�A�@�����y�l���]�O�v���ܫG�C"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("�A�}�l�����믫....\n\r", ch);
		do_add_qmark(ch, buf);
		return;
	}
	else if( qm->v0 != sn ) {
		qm->v0 = sn;
		qm->v1 = 1;
		act( "$n ���e���]�k�y������F..."NOR"", ch, NULL, NULL, TO_ROOM );
		act( HIW "$n ���ض����믫�A�@�����y�l���]�O�v���ܫG�C"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("�A����F�즳���G��A���s�}�l�����믫....\n\r", ch);
		return;
	}
	else if( qm->v1 < 2 - sklv/6) {
		++qm->v1;
		act( HIW "$n ���e���]�k���y�G�׶V�U�ӷU���I"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("�A���믫�v�����E....\n\r", ch);
		return;
	}
	send_to_char("�A����X�������]�k��q�I\n\r", ch);
	act( HIW "$n �i�}�����A�z�}���e����q���]�k���y�C"NOR"", ch, NULL, victim, TO_ROOM );
	act( ""HIW"�A��"NOR"$N"HIW"�����@�D���઺�]�k�����A���d���a����ĤH�I"NOR"", ch, NULL, victim, TO_CHAR );
	act( ""HIW"�@�D�K�������������A�µ�"NOR"$N"HIW"�����ӥh�I"NOR"" , ch, NULL, victim, TO_NOTVICT);
	act( ""HIW"�A�ݨ�@�D�K�������������r�P�a�µۧA�����ӨӡI"NOR"", ch, NULL, victim, TO_VICT );

	sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
	// npc �� del_qmark �����ϥ� mpdel_qmark 2022/04/14
	if( !IS_NPC(ch) )
		do_del_qmark(ch, buf);
	else
		do_mpdel_qmark(ch, buf);
	REMOVE_BIT(ch->affected_by, AFF_CAST_ACC);

	dam = dam * int_effect( ch );
	// sklv effect
	if( !IS_NPC(ch) && sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;
	else if ( IS_NPC(ch) )
		dam = dam * 1.7;

	if( sklv > 2 )
		dam = dam * 1.15;
	if( sklv > 3 )
		dam = dam * 1.15;
	if( sklv > 4 )
		dam = dam * 1.15;
	if( sklv > 5 )
		dam = dam * 1.16;

	if(!IS_NPC(ch)) dam = dam * (120 + PBsklv * 10) / 120;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;

	// �O�s�� dam
	base_dam = dam;
	
	// �N high explosive �ˮ`�קאּ�h�q 2022/04/15
	if( !IS_NPC(ch) ){
		while( ch->mana >= total_cost ){
			times++;

			total_cost = single_cost * times;

			if( ch->mana < total_cost )
				break;

			// �̤֫O�d���̤j mana �q�� 5% mana , �������� 1 �o�����p�U�N��� 1 �o
			if( times > 1 && ch->mana < ch->max_mana/20 )
				break;

			ch->mana = ch->mana - total_cost;
			if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD ){
				act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
				break;
			}
			// dam scale
			dam = number_range( base_dam * 36 , base_dam * 38 );
			magic_damage( ch, victim, dam * 0.1, sn, WEAR_NONE );
		}
	}else{
		times = 1 + ch->level/10;
		while( times-- ){
			// dam scale
			dam = number_range( base_dam * 20 , base_dam * 28 );
			magic_damage( ch, victim, dam * 0.13, sn, WEAR_NONE );
		}
		times = ch->level/10;
	}

	base_dam = dam;

	act( ""HIR"���઺�����H"NOR"$N"HIR"�����ߦ����A�򱵵۲��ͷ����ʪ��z���I"NOR"", ch, NULL, victim, TO_CHAR );
	act( ""HIR"��������H"NOR"$N"HIR"�����ߦ����A�򱵵۲��ͷ����ʪ��z���I"NOR"" , ch, NULL, victim, TO_NOTVICT);
	act( ""HIR"��������H�A�����ߦ������@�I�A�򱵵۲��ͷ����ʪ��z���I"NOR"", ch, NULL, victim, TO_VICT );
	
	// �z�}�ˮ` 2022/04/15
	if( IS_SET( victim->special, ANTI_MAGIC ) ){
		if( ch->class == WIZARD )
			magic_damage( ch, victim, base_dam * 0.25, sn, WEAR_NONE );
	}else{
		magic_damage( ch, victim, base_dam * 0.25, sn, WEAR_NONE );
	}

	// �p�G�I��Ƭ� 1 �o�H�U�h�������B�~�z�}�ˮ` 2022/04/15
	if( times < 2 ) return;

	// �O���Q���w���ؼ� 2022/04/15
	old_vch = victim;

	for( victim = ch->in_room->people; victim; victim = victim_next)
	{
		victim_next = victim->next_in_room;

		if ( is_same_group( ch, victim ) ) continue;

		// �Q���w���ؼФ��|�����z���i�� 2022/04/15
		if ( victim == old_vch ) continue;
		
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS)) continue;
		
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			//act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
			continue;
		}
		
		// dam scale
		dam = number_range( base_dam * 0.7 , base_dam );

		// NPC Casting
		if ( IS_NPC( ch ) )
		{
			if(IS_AFFECTED(ch, AFF_SUMMONED ) || ch->master )
			{
				if( victim->fighting == ch->master || victim->fighting == ch || (IS_NPC( victim )
					&& !IS_AFFECTED( victim, AFF_CHARM )
					&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
				{
					{
						// �d��ˮ`�קאּ���v�R�� 2022/04/15
						if( number_percent() > get_curr_dex(victim)*2 - sklv*2 ){
							act( ""HIR"�z�����¤O�i�Ψ�A�C"NOR"", ch, NULL, victim, TO_VICT );
							magic_damage( ch, victim, dam * 0.12, sn, WEAR_NONE );
						}
					}
				}
			}else{
				if( victim->fighting == ch || !IS_NPC( victim ) )
				{
					// �d��ˮ`�קאּ���v�R�� 2022/04/15
					if( number_percent() > get_curr_dex(victim)*2 - sklv*2 ){
						act( ""HIR"�z�����¤O�i�Ψ�A�C"NOR"", ch, NULL, victim, TO_VICT );
						magic_damage( ch, victim, dam * 0.12, sn, WEAR_NONE );
					}
				}
			}
		}else if( victim->fighting == ch || ( IS_NPC( victim ) 
				&& !IS_AFFECTED( victim, AFF_CHARM ) 
				&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
		{
			// �d��ˮ`�קאּ���v�R�� 2022/04/15
			if( number_percent() > get_curr_dex(victim)*2 - sklv*2 ){
				act( ""HIR"�z�����¤O�i�Ψ�A�C"NOR"", ch, NULL, victim, TO_VICT );
				magic_damage( ch, victim, dam * 0.09 , sn, WEAR_NONE );
			}
		}
	}

	return;
}

void check_fire_container( OBJ_DATA *con, CHAR_DATA* victim )
{
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;

    if( ! con->contains ) return;

	for ( obj_lose = con->contains ; obj_lose; obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( obj_lose->deleted )
		continue;
	    if ( number_bits( 2 ) != 0 || IS_SET(obj_lose->extra_flags, ITEM_BLESS))
	    {
		act( "$p�q$n���W�u���b�a�C", victim, obj_lose, NULL, TO_ROOM );
		obj_from_obj ( obj_lose );
		if ( IS_SET( obj_lose->extra_flags , ITEM_PERSONAL ) )
			obj_to_char( obj_lose, victim);
		else
			obj_to_room( obj_lose, victim->in_room );

		continue;
	    }

	    switch ( obj_lose->item_type )
	    {
	    default:		
		act( "$p�q$n���W�u���b�a�C", victim, obj_lose, NULL, TO_ROOM );
		act( "$p�q$n���W�u���b�a�C", victim, obj_lose, NULL, TO_VICT );
		obj_from_obj ( obj_lose );
		if ( IS_SET( obj_lose->extra_flags , ITEM_PERSONAL ) )
            obj_to_char( obj_lose, victim);
		else
			obj_to_room( obj_lose, victim->in_room );
		continue;
	    case ITEM_CONTAINER: msg = "$p ��M�_���U�N�I";	  break;
	    case ITEM_POTION:	 msg = "$p �m�˦ӻ]�o�F�I";	  break;
	    case ITEM_SCROLL:	 msg = "$p �q��t�}�l���N�I";  break;
	    case ITEM_STAFF:	 msg = "$p �_�X�}�}�·ϡI";	  break;
	    case ITEM_WAND:	 msg = "$p �q�X�I�I���P�I"; break;
	    case ITEM_FOOD:	 msg = "$p �o�X�J����I"; break;
	    case ITEM_PILL:	 msg = "$p �@�����ĤƤF�I";	  break;
	    case ITEM_FURNITURE: msg = "$p �N���@��J���I";  break;
	    }

	    act( msg, victim, obj_lose, NULL, TO_CHAR );
	    if( obj_lose->item_type == ITEM_CONTAINER )
	    {
		check_fire_container( obj_lose, victim );
	    }
	    extract_obj( obj_lose );
	}
}


/* Flame shield spell from Malice of EnvyMud */
void spell_flaming( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED( victim, AFF_FLAMING ) )
	return;

    af.type	 = sn;
    af.duration	 = number_fuzzy( level / 8 );
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_FLAMING;
    affect_to_char( victim, &af );

    send_to_char( "You are surrounded by a flaming shield.\n\r", victim );
    act( "$n is surrounded by a flaming shield.",
	victim, NULL, NULL, TO_ROOM );
    return;

}

void spell_angry_fire_storm( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;
	int	sklv   = get_skill_level(ch, sn);
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
		dam = number_range( obj_lv/2 , obj_lv );
	}else{
		dam = number_range( level/2 , level );
	}

    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * (1 + sklv) * weakness_damage(victim, sn) / 100;

    magic_damage( ch, victim, dam, sn, WEAR_NONE );
    
	if ( victim->deleted || victim->position == POS_DEAD ||
		 victim->in_room != ch->in_room )
		return;
		
	if ( ch->mana < 50 ) return;
	
	ch->mana -= 50;
	
    dam = UMAX( 5 + sklv , 4 + INT_DIFF( ch, victim ) + WIS_DIFF( ch, victim ) + sklv )
				* new_saves_spell( ch->pp, victim, MAGIC_FIRE ) / 4;
    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * weakness_damage(victim, sn) / 100;

    magic_damage( ch, victim, dam, sn, WEAR_NONE );

	if ( victim->deleted || victim->position == POS_DEAD ||
		 victim->in_room != ch->in_room )
		return;

	if ( ch->mana < 50 ) return;
	
	ch->mana -= 50;

    dam = number_range( sklv * 9 , sklv * 13 ) * new_saves_spell( ch->pp, victim , MAGIC_BREATH) / 7;

    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * weakness_damage(victim, sn) / 100;

    magic_damage( ch, victim, dam, sn, WEAR_NONE );

    return;
}

