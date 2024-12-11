//*****************************************************************
//*  PROGRAM ID  : spell_flame.c                                  *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940622 Razgriz                                *
//*  PURPOSE     :         修正 spell_fireball() 中，             *
//*              :         在 magic_damage() 之後，加入 ch 是否   *
//*              :         deleted 的判斷，以避免 for 迴圈的判斷  *
//*              :         中 ch->in_room 為 NULL，而造成 crash.  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940903 Jye                                    *
//*  PURPOSE     :         修正 spell_phlogistic_verse() 中，     *
//*              :              day of protection也可擋           *
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
	// 有 mystic armor 不作用負面 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
		if (nBurnRate > number_percent( ) * 100 ){
			//nMaxAcDecay = sklv * (sklv + 4) * 5;
			//if(!IS_NPC(ch)) nMaxAcDecay = nMaxAcDecay * new_saves_spell(ch->pp, victim, MAGIC_FIRE ) / 100;
			nMaxSvfDecay = (sklv - 4) * (sklv + 4);
			if(sklv > 4 && affected_value(victim,sn,APPLY_SAVING_FIRE) < nMaxSvfDecay ){
				af.type      = sn;
				//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
				// 修改作用時間 2020/11/06
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
				act( "$N"HIR"被火燄擊中，紫色的燄火瞬間纏繞在他的身上。"NOR"", ch, NULL, victim, TO_CHAR );
				act( HIR "黃色與紫色的焰火同時往你襲來！"NOR"" , ch, NULL, victim, TO_VICT);
				act( "$N"HIR"被火燄擊中，紫色的燄火瞬間纏繞在他的身上。"NOR"", ch, NULL, victim, TO_NOTVICT );
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
	send_to_char("這裡已經有一道火牆了。\n\r", ch);
	return;
    }

    af.type = sn;
    af.bitvector = R_AFF_FIREWALL;
    af.duration = number_fuzzy(level/5);
    af.modifier = number_fuzzy(get_curr_int(ch) + get_curr_wis(ch)) * 2;
    affect_to_room(ch->in_room, &af);

    send_to_char( "你在這裡建立了一道火牆！\n\r", ch );
    act( "$n 在這裡建立了一道火牆！", ch, NULL, NULL, TO_ROOM );
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

    send_to_char( HIR "你的背後出現灼熱的光輝，強大的烈焰竄過你的身旁，燒向目標！\n\r"NOR"", ch );
    act(  "$n "HIR"吟誦著遠古詩文，金色的火鳥瞬間出現在他背後，並化成猛烈的燄火席捲而來！"NOR"", ch, NULL, NULL, TO_ROOM );
    
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
				// 有 mystic armor 不作用負面 2020/10/26
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
					continue;
				af.type      = sn;
				af.bitvector = 0;
				//af.type      = sn;
				//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
				// 修改作用時間, 使抗性與負加成相關 2020/11/06
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
                            msg = "$p 突然起火燃燒！";    
                            break;
                        case ITEM_FOUNTAIN:      
                            msg = "$p 沸騰而蒸發了！";    
                            break; 
                        case ITEM_CORPSE_NPC:    
                            msg = "$p 燒成一堆焦炭！";    
                            break;
                        case ITEM_BOAT:          
                            msg = "$p 燒成一堆焦炭！";    
                            break;
                        case ITEM_DRINK_CON:     
                            msg = "$p 沸騰而蒸發了！";    
                            break;
                        case ITEM_TRASH:         
                            msg = "$p 燒成一堆焦炭！";    
                            break;
                        case ITEM_TREASURE:      
                            msg = "$p 燒成一堆焦炭！";    
                            break;
                        case ITEM_CONTAINER:     
                            msg = "$p 突然起火燃燒！";    
                            //obj_to_room( obj, victim->in_room );
                            act( "$p 裡面的東西掉了出來！",	
                                    victim, obj_lose, NULL, TO_CHAR );
                            act( "$p 裡面的東西從 $n 身上掉了出來！", 
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
                            msg = "$p 沸騰而蒸發了！";    
                            break;
                        case ITEM_SCROLL:	 
                            msg = "$p 從邊緣開始延燒！";  
                            break;
                        case ITEM_STAFF:	 
                            msg = "$p 冒出陣陣黑煙！";    
                            break;
                        case ITEM_WAND:	         
                            msg = "$p 濺出點點火星！";    
                            break;
                        case ITEM_FOOD:	         
                            msg = "$p 發出焦臭味！";      
                            break;
                        case ITEM_PILL:	         
                            msg = "$p 一顆顆融化了！";    
                            break;
                        case ITEM_FURNITURE:     
                            msg = "$p 燒成一堆焦炭！";    
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
			// 有 mystic armor 不作用負面 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			af.type      = sn;
            af.bitvector = 0;
            //af.type      = sn;
            //af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
			// 修改作用時間, 使抗性與負加成相關 2020/11/06
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
                        msg = "$p 突然起火燃燒！";    
                        break;
                    case ITEM_FOUNTAIN:      
                        msg = "$p 沸騰而蒸發了！";    
                        break; 
                    case ITEM_CORPSE_NPC:    
                        msg = "$p 燒成一堆焦炭！";    
                        break;
                    case ITEM_BOAT:          
                        msg = "$p 燒成一堆焦炭！";    
                        break;
                    case ITEM_DRINK_CON:     
                        msg = "$p 沸騰而蒸發了！";    
                        break;
                    case ITEM_TRASH:         
                        msg = "$p 燒成一堆焦炭！";    
                        break;
                    case ITEM_TREASURE:      
                        msg = "$p 燒成一堆焦炭！";    
                        break;
                    case ITEM_CONTAINER:     
                        msg = "$p 突然起火燃燒！"; 
                        act( "$p 裡面的東西掉了出來！",	
                                victim, obj_lose, NULL, TO_CHAR );
                        act( "$p 裡面的東西從 $n 身上掉了出來！", 
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
                    case ITEM_POTION:	 msg = "$p 沸騰而蒸發了！";    break;
                    case ITEM_SCROLL:	 msg = "$p 從邊緣開始延燒！";  break;
                    case ITEM_STAFF:	 msg = "$p 冒出陣陣黑煙！";    break;
                    case ITEM_WAND:	         msg = "$p 濺出點點火星！";    break;
                    case ITEM_FOOD:	         msg = "$p 發出焦臭味！";      break;
                    case ITEM_PILL:	         msg = "$p 一顆顆融化了！";    break;
                    case ITEM_FURNITURE:     msg = "$p 燒成一堆焦炭！";    break;
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
    sprintf(newbuf,"火球第一段 level：%f ,level=%d", dam, level);
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
    sprintf(newbuf,"火球第二段 sklv：%f sn=%d", dam,sn);
    send_to_all_char( newbuf );
}*/		
        dam = dam * weakness_damage( victim, sn ) / 100;
        dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;

/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"火球第三段 pp svf：%f pp-svf=%d ",dam, new_saves_spell( ch->pp, victim,MAGIC_FIRE));
    send_to_all_char( newbuf );
}*/
	if( !IS_NPC(ch) )
		dam = number_range( dam * 3 , dam * 4.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.25 );
/*if(TRUE)
{
    char newbuf[MAX_INPUT_LENGTH];
    sprintf(newbuf,"火球第四段 range：%f", dam);
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
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
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

	act( HIR "一絲火星鑽入了" NOR "$N" HIR "體內，在敵人體內殘酷地爆破！" NOR "", ch, NULL, victim, TO_CHAR );
	act( HIR "你的身體忽然傳來劇痛，接著一股灼熱的巨大能量由你體內往外猛衝！"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$n" HIR "手一揮，耀目的火星冷不防從" NOR "$N" HIR "身上竄出！"NOR"", ch, NULL, victim, TO_NOTVICT );

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
    sprintf(newbuf,"星火第四段 range：%f", dam);
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
		// npc 的 del_qmark 必須使用 mpdel_qmark 2022/04/14
		if( !IS_NPC(ch) )
			do_del_qmark(ch, buf);
		else
			do_mpdel_qmark(ch, buf);
	}

	// 將 high explosive 魔力消耗寫於技能下 2022/04/15
	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			single_cost = 90 - sklv - nSpellSklv;
		else
			single_cost = 150 - sklv - nSpellSklv;

		// instant 消耗增加
		if(ch->pcdata->autocombat == AUTOC_INSTANT)
			single_cost = single_cost + 10;

		if(ch->mana < single_cost){
			send_to_char( "你的法力不夠了.\n\r", ch);
			return;
		}
	}

	if( !(qm = is_qmark(ch, "cast_accu_mark"))){
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		act( HIW "$n 閉目集中精神，一顆光球吸收魔力逐漸變亮。"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你開始集中精神....\n\r", ch);
		do_add_qmark(ch, buf);
		return;
	}
	else if( qm->v0 != sn ) {
		qm->v0 = sn;
		qm->v1 = 1;
		act( "$n 身前的魔法球體消散了..."NOR"", ch, NULL, NULL, TO_ROOM );
		act( HIW "$n 閉目集中精神，一顆光球吸收魔力逐漸變亮。"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你釋放了原有的咒文，重新開始集中精神....\n\r", ch);
		return;
	}
	else if( qm->v1 < 2 - sklv/6) {
		++qm->v1;
		act( HIW "$n 眼前的魔法光球亮度越愈來愈高！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你的精神逐漸凝聚....\n\r", ch);
		return;
	}
	send_to_char("你釋放出全部的魔法能量！\n\r", ch);
	act( HIW "$n 張開雙眼，爆破眼前高質量的魔法光球。"NOR"", ch, NULL, victim, TO_ROOM );
	act( ""HIW"你朝"NOR"$N"HIW"直擊一道高能的魔法光束，不留情地殲滅敵人！"NOR"", ch, NULL, victim, TO_CHAR );
	act( ""HIW"一道熾熱的毀滅光束，朝著"NOR"$N"HIW"直擊而去！"NOR"" , ch, NULL, victim, TO_NOTVICT);
	act( ""HIW"你看到一道熾熱的毀滅光束猛烈地朝著你直擊而來！"NOR"", ch, NULL, victim, TO_VICT );

	sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
	// npc 的 del_qmark 必須使用 mpdel_qmark 2022/04/14
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

	// 保存基本 dam
	base_dam = dam;
	
	// 將 high explosive 傷害修改為多段 2022/04/15
	if( !IS_NPC(ch) ){
		while( ch->mana >= total_cost ){
			times++;

			total_cost = single_cost * times;

			if( ch->mana < total_cost )
				break;

			// 最少保留的最大 mana 量的 5% mana , 但足夠放 1 發的情況下將能放 1 發
			if( times > 1 && ch->mana < ch->max_mana/20 )
				break;

			ch->mana = ch->mana - total_cost;
			if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD ){
				act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
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

	act( ""HIR"高能的光束以"NOR"$N"HIR"為中心收束，緊接著產生毀滅性的爆炸！"NOR"", ch, NULL, victim, TO_CHAR );
	act( ""HIR"高能光束以"NOR"$N"HIR"為中心收束，緊接著產生毀滅性的爆炸！"NOR"" , ch, NULL, victim, TO_NOTVICT);
	act( ""HIR"高能光束以你為中心收束為一點，緊接著產生毀滅性的爆炸！"NOR"", ch, NULL, victim, TO_VICT );
	
	// 爆破傷害 2022/04/15
	if( IS_SET( victim->special, ANTI_MAGIC ) ){
		if( ch->class == WIZARD )
			magic_damage( ch, victim, base_dam * 0.25, sn, WEAR_NONE );
	}else{
		magic_damage( ch, victim, base_dam * 0.25, sn, WEAR_NONE );
	}

	// 如果施放數為 1 發以下則不給予額外爆破傷害 2022/04/15
	if( times < 2 ) return;

	// 記錄被指定的目標 2022/04/15
	old_vch = victim;

	for( victim = ch->in_room->people; victim; victim = victim_next)
	{
		victim_next = victim->next_in_room;

		if ( is_same_group( ch, victim ) ) continue;

		// 被指定的目標不會受到爆炸波及 2022/04/15
		if ( victim == old_vch ) continue;
		
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS)) continue;
		
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			//act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
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
						// 範圍傷害修改為機率命中 2022/04/15
						if( number_percent() > get_curr_dex(victim)*2 - sklv*2 ){
							act( ""HIR"爆炸的威力波及到你。"NOR"", ch, NULL, victim, TO_VICT );
							magic_damage( ch, victim, dam * 0.12, sn, WEAR_NONE );
						}
					}
				}
			}else{
				if( victim->fighting == ch || !IS_NPC( victim ) )
				{
					// 範圍傷害修改為機率命中 2022/04/15
					if( number_percent() > get_curr_dex(victim)*2 - sklv*2 ){
						act( ""HIR"爆炸的威力波及到你。"NOR"", ch, NULL, victim, TO_VICT );
						magic_damage( ch, victim, dam * 0.12, sn, WEAR_NONE );
					}
				}
			}
		}else if( victim->fighting == ch || ( IS_NPC( victim ) 
				&& !IS_AFFECTED( victim, AFF_CHARM ) 
				&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
		{
			// 範圍傷害修改為機率命中 2022/04/15
			if( number_percent() > get_curr_dex(victim)*2 - sklv*2 ){
				act( ""HIR"爆炸的威力波及到你。"NOR"", ch, NULL, victim, TO_VICT );
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
		act( "$p從$n身上滾落在地。", victim, obj_lose, NULL, TO_ROOM );
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
		act( "$p從$n身上滾落在地。", victim, obj_lose, NULL, TO_ROOM );
		act( "$p從$n身上滾落在地。", victim, obj_lose, NULL, TO_VICT );
		obj_from_obj ( obj_lose );
		if ( IS_SET( obj_lose->extra_flags , ITEM_PERSONAL ) )
            obj_to_char( obj_lose, victim);
		else
			obj_to_room( obj_lose, victim->in_room );
		continue;
	    case ITEM_CONTAINER: msg = "$p 突然起火燃燒！";	  break;
	    case ITEM_POTION:	 msg = "$p 沸騰而蒸發了！";	  break;
	    case ITEM_SCROLL:	 msg = "$p 從邊緣開始延燒！";  break;
	    case ITEM_STAFF:	 msg = "$p 冒出陣陣黑煙！";	  break;
	    case ITEM_WAND:	 msg = "$p 濺出點點火星！"; break;
	    case ITEM_FOOD:	 msg = "$p 發出焦臭味！"; break;
	    case ITEM_PILL:	 msg = "$p 一顆顆融化了！";	  break;
	    case ITEM_FURNITURE: msg = "$p 燒成一堆焦炭！";  break;
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

