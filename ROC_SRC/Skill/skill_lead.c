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
//*  PURPOSE     :         ­×¥¿ do_lead_assistance                *
//*              :         ¦bÀË¬d ch ¬O§_¬° leader «e             *
//*              :         ¥[¤J ch »P protector ¬O§_¬°¦P¶¤¤§§PÂ_  *
//*              :         ¥HÁ×§K¤§«áªº leader ÀË¬d³y¦¨ crash     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 Jye                                    *
//*  PURPOSE     :         ­×¥¿ do_lead_ucharge ³y¦¨crash¤§bug    *
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
		send_to_char( "³o¸Ì¸T¤î¾Ô°«. \n\r",ch);
		return;
	}
    
	if(!ch->fighting){
		send_to_char( "§A¤£¦b¾Ô°«¤¤. \n\r",ch);
		return;
	}
    
	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
		send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
		return;
	}
    
	if(ch->move >= 200-(sklv<<3))
	{
		ch->move -= 200 - (sklv<<3);
		send_to_char(""HIW"§A²v»âÃM§L¦V«e½Ä¾W¡I"NOR"\n\r", ch);
		act("$n²v»âÃM§L½Ä¾W¡I\n\r", ch, NULL, NULL, TO_ROOM);
		sprintf(arg, "%d", sklv);
		do_charge(ch, arg);
	}
	else
	{
		send_to_char("§AªºÅé¤O¤£°÷¤F¡I\n\r", ch);
		return;
	}

	/*
			2022/04/05
			²¾°£»Ý¬° kni ¤~·|¸òµÛ½Ä¾W , ·N¨ýµÛ lord ¤]¯à¸òµÛ½Ä¨ë
			²¾°£¤U«ü¥OªÌ¥²»Ý¬° leader ªº³]©w
			­×§ï¬°¥H rider ¬O§_¦³ cooperate ch ¨Ó§P©w¬O§_¸òµÛ½Ä¨ë
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
        send_to_char( "³o¸Ì¸T¤î¾Ô°«. \n\r",ch);
        return;
    }
    if(!ch->fighting){
        send_to_char( "§A¤£¦b¾Ô°«¤¤. \n\r",ch);
        return;
    }
    if( get_skill_percent(ch, sn) <= number_percent()){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }
    sklv = get_skill_level(ch, sn);
    if(sklv < 1){
        send_to_char("§Aªº»â¾É§Þ¥©¤£°÷¯Â¼ô¡C\n\r", ch);
        return;
    }

    if(ch->move >= 100-(sklv<<3))
    {
        ch->move -= 100 - (sklv<<3);
        send_to_char(""HIW"§A«ü´§¤}½b¤â½b¥Ú»ô©ñ¡I"NOR"\n\r", ch);
        act("$n«ü´§¤}½b¤â½b¥Ú»ô©ñ¡I", ch, NULL, NULL, TO_ROOM);
    }
    else
    {
        send_to_char("§AªºÅé¤O¤£°÷¤F¡I\n\r", ch);
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
        send_to_char("¨S¦³¼Ä­x¥i¥HÅý§A®gÀ»¡C\n\r",ch);
        return;
    }

    //The penalty of hitroll when use this skill.
    aff.type	 = gsn_meteoric_arrow;
    aff.duration = 1;
    aff.location = APPLY_HITROLL;

    //Fire by archers who leading by lord.
	// ­×§ï¬° ¥H cooperate ¬°§P©w 2022/04/05
    //for(archer = ch->in_room->people; archer; archer=archer->next_in_room)
	for(archer = ch->cooperator; archer; archer = archer->next_in_coop)
    {
		
		/*
			2022/04/05

			²¾°£¤U«ü¥OªÌ¥²»Ý¬° leader ªº³]©w
			­×§ï¬°¥H archer ¬O§_¦³ cooperate ch ¨Ó§P©w¬O§_¸òµÛ½Ä¨ë
			¨Ã±N¨C­Ó±ø¥ó¤À¶}§P©w, ¥H¥[¤J¨ä¥L±ø¥ó

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
            //send_to_char( "¦n¤ï®³§â¤}§a¡H\n\r", archer );
            continue;
        }

        if ( !( arrow = get_eq_char( archer, WEAR_ARROW ) ) || arrow->value[0] == 0 )
        {
            //send_to_char( "³á³á¡A¨S½b¤F!\n\r", archer );
            continue;
        }

        if(!IS_NPC(archer)
			&&( (get_skill_percent(archer, gsn_meteoric_arrow)>>1)
				+ (sklv<<3) < number_percent() ) )
        {
            send_to_char("§A¸Õ¹ÏÅ¥±q»â¥Dªº«ü´§¡A¥i±¤¥¢±Ñ¤F¡I",archer);
            continue;
        }

        for( can_fire = TRUE, rch = ch->in_room->people; rch; rch = rch->next_in_room )
        {
            if ( rch->fighting == archer )
            {
				int ride_sklv   = get_skill_level(archer, gsn_ride );
				int tricky_sklv = get_skill_level(archer, gsn_tricky_pacing );

				// ¥i¥H¦Û¥Ñ®g½bªº±ø¥ó 2022/04/05
				if( get_eq_char( archer, WEAR_RIDE ) && ride_sklv > 2 && ride_sklv >= tricky_sklv )
				{
					if( number_percent( ) < 10 + ride_sklv * 15 )
						break;
				}
				else if( tricky_sklv > 2 && number_percent( ) < 10 + tricky_sklv * 15 )
				{
					break;
				}
				send_to_char( "§A³Q¥´±o¤â¦£¸}¶Ã¡A¨S®É¶¡©Ô¤}¡C\n\r", archer );
				can_fire = FALSE;
				break;
            }
        }
        
        if(!can_fire)
            continue;

        if ( archer->move < 150 )
        {
            send_to_char( "§AÄ±±o¦n²Ö¡I\n\r", archer );
            continue;
        }

        sklv_mete = get_skill_level(archer, gsn_meteoric_arrow);
        sklv_archery = get_skill_level(archer, gsn_shot_arrow);
        fire_number = UMAX((3+sklv_mete*3)/number_enemy, 1);
        archer_hr = (((sklv<<1)
					+sklv_archery
					-(sklv>number_bits(2)?20:50))<<2)/number_enemy;
        archer->move -= 150;
        send_to_char( "§A®¾¤}¯¥ÅF¡A³\\¦h½b¥Ú¦p¬y¬P¯ë©¹¼Ä°}­¸¥h¡I\n\r", archer );

        //Join the affect to archer and remove it after firing.
        if((archer->hitroll>>2) + archer_hr < 0)
            send_to_char("§Aªº­¸½b®g¬n¤F¡I\n\r", archer);
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
        send_to_char( "³o¸Ì¸T¤î¾Ô°«. \n\r",ch);
        return;
    }
    if(!ch->fighting || ch->fighting->deleted )
    {
        send_to_char("§A©|¥¼¨­³B¾Ô°«¤¤¡I\n\r",ch);
        return;
    }
    
    if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent())
    {
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }
    
    sklv = get_skill_level(ch, skill_lookup("lead"));
    
    if(sklv < 3)
    {
        send_to_char("§Aªº»â¾É§Þ¥©¤£°÷¯Â¼ô¡C\n\r", ch);
        return;
    }
    
    if(ch->move >= 150-(sklv<<3))
    {
        ch->move -= 150 - (sklv<<3);
        act("§A±aÀY½Ä¦V¼Ä¤H$N¡I", ch, NULL, ch->fighting, TO_CHAR);
        act("$n±a»â¤j®a½Ä¶i¼Ä°}¡A¬ðÅ§$N¡I", ch, NULL, ch->fighting, TO_ROOM);
    }
    else
    {
        send_to_char("§AªºÅé¤O¤£°÷¤F¡I\n\r", ch);
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
                send_to_char("You cannot break through the defend of enemies¡I\n\r", rch);
            else
                send_to_char("§A½Ä¤£¹L¼Ä¤èªº¨¾½u¡I\n\r", rch);
        }
        else{
            if( !is_chinese(rch, ch->fighting))
                act( "You change your fighting target to $N!",  rch, NULL, ch->fighting, TO_CHAR);
            else
                act( "§A¶}©l§ðÀ» $N¡I",  rch, NULL, ch->fighting, TO_CHAR);
            if( !is_chinese(ch->fighting, rch))
                act( "$n turn $s target to you!", rch, NULL, ch->fighting, TO_VICT);
            else
                act( "$n Âà¨­¦V§Aµo°Ê§ðÀ»¡I", rch, NULL, ch->fighting, TO_VICT  );
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
	// ²¾°£ number_rescue °Ñ¼Æ, Åý wait ¦b¨C­Ó rescue ®É§P©w 2023/01/12
	//int sklv, wait, number_rescue = 0;
	int sklv;
	int wait;
	int p_move_cost; // 2023/01/12

	if( !ch->group ) 
	{
		send_to_char("§A¨Ã¨S¦³¦b¶¤¥î¤¤¡C\n\r", ch);
		return;
	}

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "³o¸Ì¸T¤î¾Ô°«. \n\r",ch);
		return;
	}
	if(!ch->fighting || ch->fighting->deleted){
		send_to_char("§A©|¥¼¨­³B¾Ô°«¤¤¡I\n\r",ch);
		return;
	}
	if(!protector) {
		send_to_char("¨S¦³¤H¥i¥H«OÅ@¶¤¥î¡I\n\r",ch);
		return;
	}
	
	if( !is_same_group( ch, protector ) )			//Added by Razgriz  20050618
	{
		send_to_char("¹ï¤è¤£¦b§Aªº¶¤¥î¤¤¡I\n\r",ch);
		return;
	}
	
	if( protector->group->leader && protector->group->leader != ch)
	{
		send_to_char("¹ï¤è¤£¨ü§A«ü´§¡I\n\r",ch);
		return;
	}

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
		send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
		return;
	}
	sklv = get_skill_level(ch, skill_lookup("lead"));

	if(sklv < 2){
		send_to_char("§Aªº»â¾É§Þ¥©¤£°÷¯Â¼ô¡C\n\r", ch);
		return;
	}

	// stone form ªºª¬ºA¤UµLªk°õ¦æ lead 2022/12/05
	if( is_affected(protector, gsn_stone_form) ){
		send_to_char("¥L¥¿³B¦b¥Û¤Æª¬ºA¤U¡AµLªk°õ¦æ§Aªº«ü´§¡C\n\r", ch);
		return;
	}

	// §P©w protector ªº mv ¬O§_¨¬°÷¨Ï¥Î rescue 2023/01/12
	p_move_cost = 200 - get_skill_level(protector, gsn_rescue)*20;

	if( protector->move < p_move_cost ){
		send_to_char("¥LªºÅé¤O¤£¨¬¥H°õ¦æ³o­Ó«ü¥Ü¡C\n\r", ch);
		return;
	}

	// §P©w protector wait ¬O§_¤Ó°ª 2023/01/12
	if( protector->wait > 11 + sklv * 2 )
	{
		send_to_char( "[33m¥L¤w¸g¤â¦£¸}¶Ã¤F¡I[m\n\r", ch );
		return;
	}

	if( ch->move >= 100 - (sklv<<3) ){
		ch->move -= 100 - (sklv<<3);
		act("§A«ü´§$N«OÅ@¾ã­Ó¶¤¥î¡I", ch, NULL, protector, TO_CHAR);
		act("$n«ü´§§A«OÅ@¾ã­Ó¶¤¥î¡I", ch, NULL, protector, TO_VICT);
		act("$n«ü´§$N«OÅ@¾ã­Ó¶¤¥î¡I", ch, NULL, protector, TO_NOTVICT);
	}
	else{
		send_to_char("§AªºÅé¤O¤£°÷¤F¡I\n\r", ch);
		return;
	}

	wait = protector->wait;

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room)
	{
		// protector ªº mv ¤p©ó¯à¨Ï¥Î rescue ªº­È®É, ª½±µ¸õ¥Xµ{§Ç 2023/01/12
		if( protector->move < p_move_cost ){
			send_to_char( "§AªºÅé¤O¤£¨¬¥HÄ~Äò°õ¦æ«ü¥Ü¡C\n\r", protector );
			break;
		}

		if( rch->deleted || !is_same_group(rch, ch) || protector == rch )
			continue;

		// ¥Ø¼Ð¤£¦b¾Ô°«¤¤ (config -assist ªº±¡ªp) 2023/01/12
		if ( !rch->fighting )
			continue;

		/* 2023/01/12
			­×§ï assistance , ª½±µ¥H do_rescue §P©w¬O§_¦¨¥\
			¨Ãª½±µ¥H wait §@¬°¨C¦¸ rescue «áªºÁ`­È

		if(sklv + 1 >= number_bits(3)){
			do_rescue(protector, rch->name);
			number_rescue++;
		}else if(rch->fighting)
			act("§A¸Õ¹Ï«OÅ@$N¡A¥i¬O¥¢±Ñ¤F¡I", protector, NULL, rch, TO_CHAR); */

		do_rescue( protector, rch->name );
		// ¨C¦¸ rescue ³Ì¤Ö wait + 1 2023/01/12
		wait += UMAX( 1 , protector->wait - wait - sklv/2 );
	}

	WAIT_STATE( ch, 12 - sklv );
	// ª½±µ±N protector ªº wait ÅÜ§ó¬° wait( ªì©l + ¦h¦¸rescue ªºÁ`­È) 2023/01/12
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
            send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
            return;
        }
	sklv = get_skill_level(ch, skill_lookup("lead"));
        if(sklv < 4){
            send_to_char("§Aªº»â¾É§Þ¥©¤£°÷¯Â¼ô¡C\n\r", ch);
            return;
        }
        if(ch->move >= 200-(sklv<<4)){
	    ch->move -= 200 - (sklv<<4);
	    act("§A«ü´§¶¤¥îÁô½ª¨­§Î¡AÂÃ°Î¦æ­x¡I", ch, NULL, NULL, TO_CHAR);
	    act("$n«ü´§¶¤¥îÁô½ª¨­§Î¡AÂÃ°Î¦æ­x", ch, NULL, NULL, TO_ROOM);
	}
	else{
	    send_to_char("§AªºÅé¤O¤£°÷¤F¡I\n\r", ch);
	    return;
	}

	for(number_of_scout = 0,rch = ch->in_room->people;rch && number_of_scout <2;rch=rch->next_in_room)
	    if(!IS_NPC(rch) && (rch->class == NINJA || rch->class == ASSASSIN) &&
		is_same_group(rch,ch))
		number_of_scout++;

	if(number_of_scout < 2){
	    send_to_char("§A¶¤¥î¤¤ªº¥¸«J¤£¨¬¡I\n\r", ch);
	    return;
	}

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(is_same_group(rch,ch) ){
		if((time_info.hour < 4+(sklv>>1) || 
		    time_info.hour > 20-(sklv>>1)) &&
		    !IS_AFFECTED( rch, AFF_HIDE)){
		    SET_BIT( rch->affected_by, AFF_HIDE);
		    send_to_char("§A·ÓµÛ»â¥Dªº«ü¥ÜÂÃ°Î¤F°_¨Ó¡C\n\r", rch);
		}
		if(!is_affected(rch, gsn_sneak)){
		    af.type      = gsn_sneak;
		    af.duration  = ch->level;
		    af.location  = APPLY_NONE;
		    af.modifier  = 0;
		    af.bitvector = AFF_SNEAK;
		    affect_to_char( rch, &af );
		    send_to_char("§A·ÓµÛ»â¥Dªº«ü¥Ü©ñ»´¸}¨B¡C\n\r", rch);
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
            send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
            return;
        }
	if(!ch->fighting || ch->fighting->deleted){
	    send_to_char("§A©|¥¼¨­³B¾Ô°«¤¤¡I\n\r",ch);return;
	}
	if(ch->wait > skill_table[gsn_snare].beats){
	    send_to_char("§A¤â¦£¸}¶Ã¡AµLªk«ü´§¶¤¥î¡I\n\r",ch);return;
	}
	sklv = get_skill_level(ch, skill_lookup("lead"));
        if(sklv < 5){
            send_to_char("§Aªº»â¾É§Þ¥©¤£°÷¯Â¼ô¡C\n\r", ch);
            return;
        }
        if(ch->move >= 300-(sklv<<5)){
	    ch->move -= 300 - (sklv<<5);
	    act("§A«ü´§¶¤¥î¡A¥]§Û¦X³ò¼Ä­x¡I", ch, NULL, NULL, TO_CHAR);
	    act("$n«ü´§¶¤¥î¡A¥]§Û¦X³ò¼Ä­x¡I", ch, NULL, NULL, TO_ROOM);
	}
	else{
	    send_to_char("§AªºÅé¤O¤£°÷¤F¡I\n\r", ch);
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
	    send_to_char("§Aªº¹Î­û¼Æ¶q¤Ó¤Ö¡A¤£¨¬¥H¥]³ò¼Ä­x¡I\n\r", ch);
	    return;
	}
	if(!(number_front = class_array[SHAMAN]+class_array[KNIGHT]+
	    class_array[WARLORD]+class_array[MARTIALMAN])){
	    send_to_char("§Aªº«e½u¹Î­û¼Æ¶q¤Ó¤Ö¡A¤£¨¬¥H¥]³ò¼Ä­x¡I\n\r", ch);
	    return;
	}
	if(!(number_back = class_array[NINJA]+class_array[RANGER])){
	    send_to_char("§A©Ò¯à¤ä¬£ªº¥]§Û¤H­û¤Ó¤Ö¡A¤£¨¬¥H¥]³ò¼Ä­x¡I\n\r", ch);
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
	    send_to_char("§A¥]³ò¼Ä­xªº¥ø¹Ï³QÃÑ¯}¡I\n\r",ch);
	    return;
	}
	send_to_char("§A¦¨¥\\¦a¥]³ò¼Ä­x¡I\n\r",ch);
	act("$n¦¨¥\\¦a«ü´§¡A¦X³ò¥]§Û¼Ä­x¡I",ch, NULL, NULL, TO_ROOM);

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
		send_to_char("§A³Q¼Ä¤H¥]³ò¤F¡I\n\r", rch);
		act("§A¶¶§Q¥]³ò$N¡I",ch, NULL, rch, TO_CHAR);
		mprog_hurt_trigger(rch, ch, gsn_snare);
	    }
	}
	WAIT_STATE( ch, skill_table[gsn_snare].beats<<1 );
}

