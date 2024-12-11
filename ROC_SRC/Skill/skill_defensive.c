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

extern void check_alignment_eq( CHAR_DATA *ch );
extern bool IsSorGateMobEarth(int MOB_INDEX);
//bool psionic_check(CHAR_DATA *ch, int sn );

void do_defensive( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	int bonus = 0;
	int delay;
	char buf[BUF_STRING_LENGTH];
	QMARK *q;

	if( is_affected(ch, gsn_defensive) ) return;

	if( !IS_NPC(ch) ) {
		if( number_percent() > get_skill_percent( ch, gsn_defensive)){
			send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
			return;
		}
		if( ch->move < 50 || ch->hit < 50 ) {
			send_to_char("§AªºÅé¤O¤£°÷¡AµLªk¨Ï¥ÎÅ@¨­Ó\\®ð¡C\n\r", ch);
			return;
		}
		if( ( q = is_qmark( ch, "skill_defensive_mark" ) )){
			if( q->timer > 36 ) {
				send_to_char("§AÁÙ»Ý­n¤@¤Ñ¥b¥ª¥kªº®É¶¡¤~¯à¦^´_Åé¤O¡C\n\r", ch);
			}
			else if( q->timer > 24 ) {
				send_to_char("§AÁÙ»Ý­n¤@¤Ñ¥ª¥kªº®É¶¡¤~¯à¦^´_Åé¤O¡C\n\r", ch);
			}
			else if( q->timer > 12 ) {
				send_to_char("§AÁÙ»Ý­n¥b¤Ñ¥ª¥kªº®É¶¡¤~¯à¦^´_Åé¤O¡C\n\r", ch);
			}
			else
				send_to_char("§AÁÙ¨S±q¤§«eªº¯h²Ö¤¤¦^´_¹L¨Ó¡C\n\r", ch);
			//Modified by Keric for KUSO!
			send_to_char("«Ä¤l¡A§A²Ö¤F¡C­n¤£­n¨Ó¸J²M´ö¡H\n\r", ch);
			return;
		}
	}

	act("§A¶°¤¤¥þ¤O¡A¹B°_Å@¨­Ó\\®ð¡C", ch, NULL,  NULL, TO_CHAR); 
	act("$n¶°¤¤¥þ¤O¡A¹B°_Å@¨­Ó\\®ð¡C", ch, NULL,  NULL, TO_ROOM); 
	bonus = ch->level / 7;

	af.type      = gsn_defensive;
	af.duration  = 3 ;
	af.location  = APPLY_DODGE;
	af.modifier  = bonus * 10 
					+ bonus * get_skill_level(ch, gsn_defensive) * 2 ;
	af.bitvector = 0;
	affect_to_char( ch, &af ); 
	//delay = 69 - (bonus * 2);
	delay = abs(ch->level - 32);
	delay = 81 - (delay * 3);
	sprintf( buf, "self skill_defensive_mark %d 0 0 0 0 1", delay - get_skill_level(ch,gsn_defensive)*6);
	do_add_qmark( ch, buf );

	/* Modified by Keric for PDR enhancement 2005/2/13 */
	af.type     = gsn_defensive;
	af.duration = 3 ;
	af.location = APPLY_RDAMAGE;
	af.modifier = bonus * ( (get_curr_wis(ch) + get_curr_con(ch))/(7-get_skill_level(ch,gsn_defensive)) );
	af.bitvector = 0;
	affect_to_char( ch, &af );
}


void do_rescue( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *fch = NULL;
    char arg [ MAX_INPUT_LENGTH ];
    int count;
	int sklv = get_skill_level(ch, gsn_rescue); // 2023/01/12
	char buf[BUF_STRING_LENGTH]; // 2023/01/12
	int rescue_round = 0; // 2023/01/12
	bool IsProcess = FALSE; // 2023/01/12

    /* Don't allow charmed mobs to do this, check player's level */
    if ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
    {
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
    }

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§AµLªk¦b¥Û¤Æª¬ºA¤U¨Ï¥Î§Þ¥©.\n\r", ch);
		return;
	}

    if ( is_affected( ch, gsn_berserk ) )
    {
		send_to_char( "§A¥¿±þ±o²´¬õ¡I\n\r", ch );
		return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		send_to_char( "­n¸Ñ±Ï½Ö?\n\r", ch );
		return;
    }

    if ( !( victim = get_char_room( ch, arg ) ) )
    {
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
    }

    if ( victim == ch )
    {
		send_to_char( "·Q¦Û±Ï¡H¸Õ¸Õ flee «ü¥O§a¡I\n\r", ch );
		return;
    }

    if ( ch->fighting == victim )
    {
		send_to_char( "¤@¤MÅý¥L¸Ñ²æ¤ñ¸û§Ö.\n\r", ch );
		return;
    }

    if ( !victim->fighting )
    {
		send_to_char( "§A¥u¯à¸Ñ±Ï¾Ô°«¤¤ªº¥Ø¼Ð.\n\r", ch );
		return;
    }

    if ( !IS_NPC( ch ) && !is_same_group( ch, victim ) && ch->clan != victim->clan )
    {
		send_to_char( "§A·F¹À³o»ò¦h¨Æ¡H\n\r", ch );
		return;
    }

    if ( !check_blind ( ch ) )
		return;

	// rescue ¦¨¥\®Éµ¹¤©¤£³Q change ªº round ¼Æ 2023/01/12
	if( sklv > 0 )
		rescue_round += 1;
	if( sklv > 2 )
		rescue_round += 1;
	if( sklv > 5 )
		rescue_round += 1;
	if( sklv > 6 )
		rescue_round += 1;

    count = 0;
    for ( fch = victim->in_room->people; fch; fch = fch->next_in_room )
    {
		if (fch->deleted )
			continue;
		if ( fch->fighting == victim )
		{
			if ( !IS_NPC(ch) && ch->move < 200 - sklv*20 ){
				send_to_char( "§A¤Ó²Ö¤F¡C\n\r", ch );
				return;
			}

			if ( !IS_NPC(ch) )
				ch->move = ch->move - 200 + sklv*20;

			// §P©w¬O§_¦³°õ¦æ rescue ªº°Ê§@ 2023/01/12
			IsProcess = TRUE;

			if( !IS_NPC(ch) && number_percent() > (get_skill_percent(ch, gsn_rescue) - 20) + sklv + sklv*sklv/2 )
			{
				if( !is_chinese(ch, victim))
				{
					act( "You tried to rescue $N, but FAILED.", ch, NULL, victim, TO_CHAR );
					act( "$n tried to rescue $N, but FAILED.", ch, NULL, victim, TO_NOTVICT );
				}
				else
				{
					act( "§A¸ÕµÛ¸Ñ±Ï$N¡A¥i±¤¥¢±Ñ¤F¡C", ch, NULL, victim, TO_CHAR );
					act( "$n¸ÕµÛ¸Ñ±Ï$N¡A¥i±¤¥¢±Ñ¤F¡C", ch, NULL, victim, TO_NOTVICT );
				}
				if( !is_chinese(victim, ch))
					act( HIG"$n tried to rescue you, but FAILED."NOR, ch, NULL, victim, TO_VICT );
				else
					act( HIG"$n¸ÕµÛ¸Ñ±Ï§A¡A¥i±¤¥¢±Ñ¤F¡C"NOR, ch, NULL, victim, TO_VICT );

				continue;
			}
			if( IS_NPC(ch) && number_percent() > UMIN( 85 , GetGeneralMobSklv(ch) * 12 + ch->level/2 ) )
			{
				if( !is_chinese(ch, victim))
				{
					act( "You tried to rescue $N, but FAILED.", ch, NULL, victim, TO_CHAR );
					act( "$n tried to rescue $N, but FAILED.", ch, NULL, victim, TO_NOTVICT );
				}
				else
				{
					act( "§A¸ÕµÛ¸Ñ±Ï$N¡A¥i±¤¥¢±Ñ¤F¡C", ch, NULL, victim, TO_CHAR );
					act( "$n¸ÕµÛ¸Ñ±Ï$N¡A¥i±¤¥¢±Ñ¤F¡C", ch, NULL, victim, TO_NOTVICT );
				}
				if( !is_chinese(victim, ch))
					act( HIG"$n tried to rescue you, but FAILED."NOR, ch, NULL, victim, TO_VICT );
				else
					act( HIG"$n¸ÕµÛ¸Ñ±Ï§A¡A¥i±¤¥¢±Ñ¤F¡C"NOR, ch, NULL, victim, TO_VICT );

				continue;
			}

			if( !is_chinese(ch, victim))
			{
				act( HIG"You rescue $N!"NOR,  ch, NULL, victim, TO_CHAR	 );
				act( HIG"$n rescue $N!"NOR,  ch, NULL, victim, TO_NOTVICT );
			}
			else
			{
				act( HIG"§A¸Ñ±Ï¤F $N¡I"NOR,  ch, NULL, victim, TO_CHAR	 );
				act( HIG"$n ¸Ñ±Ï¤F $N¡I"NOR,  ch, NULL, victim, TO_NOTVICT );
			}

			if( !is_chinese(victim, ch))
				act( HIG"$n rescue you!"NOR, ch, NULL, victim, TO_VICT	 );
			else
				act( HIG"$n ¸Ñ±Ï¤F§A¡I"NOR, ch, NULL, victim, TO_VICT	 );

			stop_fighting( fch, FALSE );
			stop_fighting( ch, FALSE );
			set_fighting( fch, ch );
			set_fighting( ch, fch );

           /*   2023/01/12
			*   rescue ¦¨¥\®É, µ¹¤©§ðÀ» victim ªº¤H(fch) qmark,
			*   ¨Ï¸Ó¨¤¦â¦b v0 round «eµLªk¨Ï¥Î do_change, v0 ¤p©ó 0 ®É, «h¥i¥H¦A¦¸¨Ï¥Î change
			*   ¸Ó qmark ©ó¾Ô°««á©Î timer ¨ì´Á®ø·À, ¨ä¶¡µLªk¦A¦¸¹ï¥Ø¼Ð¥[¤J qmark
			*/
			if( !(is_qmark( fch, "skill_rescue_round" ) ) && rescue_round > 0 )
			{
				sprintf( buf, "self skill_rescue_round 2 %d 0 0 1 0", rescue_round );
				send_to_char( "§A³Q¤H¨n¤W¤F¡Iµu´Á¶¡¤ºµLªkÂà´«¾Ô°«¥Ø¼Ð¡C\n\r", fch );
				do_add_qmark( fch , buf );
			}
		}
	}

	// ½T¹ê¦³°õ¦æ¹L rescue ªº°Ê§@®É¤~·|¦³ stun 2023/01/12
	if( IsProcess )
		WAIT_STATE( ch, 14 - sklv*2 );
	return;
}

void do_disarm( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];
	int percent;

	/* Don't allow charmed mobiles to do this, check player's level */
	if ( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
	|| ( !IS_NPC( ch )
		&& ch->pcdata->learned[gsn_disarm] == 0 ) )
	{
		send_to_char( "§A¤£À´±o¦p¦óÀ»¸¨§O¤HªºªZ¾¹.\n\r", ch );
		return;
	}

	if (   !get_eq_char( ch, WEAR_WIELD	  )
		&& !get_eq_char( ch, WEAR_WIELD_2 )
		&& !get_eq_char( ch, WEAR_TWO_HAND ) )
	{
		send_to_char( "§A±o¥ý®³§âªZ¾¹¤~¦æ.\n\r", ch );
		return;
	}

	if ( !ch->fighting )
	{
		send_to_char( "§A¨Ã¨S¦³¦b¾Ô°«.\n\r", ch );
		return;
	}

	one_argument( argument, arg );

	victim = ch->fighting;

	if ( arg[0] != '\0' ){
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
			return;
		}
	}

	if ( victim->fighting != ch && ch->fighting != victim )
	{
		act( "$E ¨Ã¨S¦³¦b©M§A¾Ô°«¡I", ch, NULL, victim, TO_CHAR );
		return;
	}

	if (   !get_eq_char( victim, WEAR_WIELD   )
		&& !get_eq_char( victim, WEAR_WIELD_2 )
		&& !get_eq_char( victim, WEAR_TWO_HAND ) )
	{
		send_to_char( "§Aªº¥Ø¼Ð¨Ã¨S¦³¸Ë³ÆªZ¾¹.\n\r", ch );
		return;
	}

	percent = number_percent( ) + ( victim->level - ch->level ) * 2 + get_skill_level(ch, gsn_disarm) * 2;
	percent = ( percent * weakness_percent(victim, gsn_disarm) ) / 100;
	if ( !get_eq_char( ch, WEAR_WIELD ) && !get_eq_char( ch, WEAR_TWO_HAND ) )
		percent *= 2;		/* 1/2 as likely with only 2nd weapon */
	if ( IS_NPC( ch ) )
	{
		if ( percent < number_percent( ) ) {
			disarm( ch, victim );
			WAIT_STATE( victim, skill_table[gsn_disarm].beats );
		}
	}
	else if ( percent < get_skill_percent(ch, gsn_disarm) * 2 / 3 ) {
		disarm( ch, victim );
		WAIT_STATE( victim, skill_table[gsn_disarm].beats );
	}
	else
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
	WAIT_STATE( ch, skill_table[gsn_disarm].beats );

	mprog_hurt_trigger(victim, ch, gsn_disarm);

	return;
}

void do_barehand( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];
	int percent;

	/* Don't allow charmed mobiles to do this, check player's level */
	if ( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		|| ( !IS_NPC( ch )
		&& ch->pcdata->learned[gsn_barehand] == 0 ) )
	{
		send_to_char( "§A¤£À´±o¦p¦óªÅ¤â¤J¥Õ¤b.\n\r", ch );
		return;
	}

	if (   get_eq_char( ch, WEAR_WIELD   )
		|| get_eq_char( ch, WEAR_WIELD_2 ) )
	{
		send_to_char( "§A¥²¶·ªÅ¤â.\n\r", ch );
		return;
	}

	if ( !ch->fighting )
	{
		send_to_char( "§A¨Ã¨S¦³¦b¾Ô°«.\n\r", ch );
		return;
	}

	one_argument( argument, arg );

	victim = ch->fighting;

	if ( arg[0] != '\0' ){
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
			return;
		}
	}

	if ( victim->fighting != ch && ch->fighting != victim )
	{
		act( "$E ¨Ã¨S¦³¦b©M§A¾Ô°«¡C", ch, NULL, victim, TO_CHAR );
		return;
	}

	if (   !get_eq_char( victim, WEAR_WIELD   )
		&& !get_eq_char( victim, WEAR_WIELD_2 )
		&& !get_eq_char( victim, WEAR_TWO_HAND ) )
	{
		send_to_char( "§Aªº¥Ø¼Ð¨Ã¨S¦³´¤µÛªZ¾¹.\n\r", ch );
		return;
	}

	percent = number_percent( ) + victim->level - ch->level + get_skill_level(ch,gsn_barehand) + get_skill_level(ch, gsn_barehand)/2;
	percent = ( percent * weakness_percent(victim, gsn_barehand) ) / 100;

	if ( IS_NPC( ch ) || percent < get_skill_percent(ch, gsn_barehand) * 2 / 3 ){
		disarm( ch, victim );
		WAIT_STATE( victim, skill_table[gsn_barehand].beats );
	}
	else
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
	WAIT_STATE( ch, skill_table[gsn_barehand].beats );

	mprog_hurt_trigger(victim, ch, gsn_barehand);

	return;
}


void do_invoke( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	int sklv ;
	char arg[MAX_INPUT_LENGTH];
	char qmark[MAX_INPUT_LENGTH];

	if ( is_affected( ch, gsn_invoke ) ){
		send_to_char("§A¨­®ÇÁÙ¦³Å]¯«©O¡I\n\r", ch);
		return;
	}
	one_argument(argument, arg);
	if( arg[0] == '\0' ) {
		sprintf(arg, "hrdr");
	}
	sprintf(qmark, "skill_invoke_%s", arg);
	sklv = get_skill_level(ch, gsn_invoke);

	if ( ( !IS_NPC( ch ) && number_percent() > get_skill_percent(ch, gsn_invoke) )
		|| ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) )
	{
		send_to_char( "Å]¯«­Ì³£¨SªÅ¡I\n\r", ch );
		ch->move = UMAX( 0 , ch->move - 50);
		ch->mana = UMAX( 0, ch->mana - 100 );
		return;    
	}
	switch(arg[0]){
		case 'h':
		case 'H':
			if( !str_cmp(arg, "hrdr")){
				if ( !IS_NPC( ch ) )
				{
					if ( ch->pcdata->condition[COND_DRUNK] > 30 - sklv * 2 )
					{
						send_to_char( "³Ü¨º»ò¾K¡A¤£©ÈÁ¶Âp¯«©ú¶Ü¡I\n\r", ch );
						return;
					}
					if ( ( race_table[ch->race].thirst_mod  && ch->pcdata->condition[COND_THIRST] < 16)
							|| ( race_table[ch->race].hunger_mod && ch->pcdata->condition[COND_FULL] < 16 )
							|| ch->move < ch->max_move / 2 
							|| ch->move < 150
							|| ch->mana < 210)
					{
						send_to_char( "§A¨S¦³¨¬°÷ªººë¤O¡I\n\r", ch );
						return;
					}
					else
					{
						if( race_table[ch->race].thirst_mod )
							ch->pcdata->condition[COND_THIRST] -= 16;
						if( race_table[ch->race].hunger_mod )
							ch->pcdata->condition[COND_FULL] -= 16;
						ch->move -= 80 + 10*get_skill_level(ch,gsn_invoke);
						ch->mana -= 20 + 5*get_skill_level(ch,gsn_invoke);
					}
				}



				if( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))
					ch->alignment = UMAX( -1000, ch->alignment - 50 );

				af.type	 = gsn_invoke;
				af.duration	 = 4 + get_skill_level(ch,gsn_invoke) * 3/2;
				af.location	 = APPLY_HITROLL;
				af.modifier	 = ch->level / 10 * get_skill_level(ch,gsn_invoke);
				af.bitvector = 0;
				affect_to_char( ch, &af );

				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_ADAMAGE;
				af.modifier	 = ch->level / 4 * 4 * get_skill_level(ch,gsn_invoke);
				af.bitvector = 0;
				affect_to_char( ch, &af );
				
				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_AMAGIC;
				af.modifier	 = ch->level / 5 * 3 * get_skill_level(ch,gsn_invoke);
				af.bitvector = 0;
				affect_to_char( ch, &af );
				
				// °l¥[¬r¯À­@©Ê 2021/11/04
				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_SAVING_POISON;
				af.modifier	 = -100 - get_skill_level(ch,gsn_invoke)*100;
				affect_to_char( ch, &af );

				send_to_char( "§AÄ±±o§A¦Û¤v....ÅÜ¦¨¯«¤F¡I\n\r", ch );
				act( "$n ¥þ¨­¬ðµMµo¥X»þ¯PªºÓ\\®ð¡I", ch, NULL, NULL, TO_ROOM );
			}
			break;
		case 'd':
		case 'D':
			if( !str_cmp("def", arg)){
				if( sklv < 5 ) {
					send_to_char("§Aªº°@¸Û«×ÁÙ¤£°÷¡AµLªk¥l³ê¯«ÆF¡C\n\r", ch);
					return;
				}
				if ( !IS_NPC( ch ) )
				{
					if ( ch->pcdata->condition[COND_DRUNK] > 30 - sklv * 2 )
					{
						send_to_char( "³Ü¨º»ò¾K¡A¤£©ÈÁ¶Âp¯«©ú¶Ü¡I\n\r", ch );
						return;
					}
					if ( ( race_table[ch->race].thirst_mod  && ch->pcdata->condition[COND_THIRST] < 16)
							|| ( race_table[ch->race].hunger_mod && ch->pcdata->condition[COND_FULL] < 16 )
							|| ch->move < ch->max_move / 2 
							|| ch->move < 150
							|| ch->mana < 210)
					{
						send_to_char( "§A¨S¦³¨¬°÷ªººë¤O¡I\n\r", ch );
						return;
					}
					else
					{
						if( race_table[ch->race].thirst_mod )
							ch->pcdata->condition[COND_THIRST] -= 16;
						if( race_table[ch->race].hunger_mod )
							ch->pcdata->condition[COND_FULL] -= 16;
						ch->move -= 80+10*get_skill_level(ch,gsn_invoke);
						ch->mana -= 20+5*get_skill_level(ch,gsn_invoke);
					}
				}
				if( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))
					ch->alignment = UMAX( -1000, ch->alignment - 50 );


				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_RDAMAGE;
				af.modifier	 = ch->level / 4 + get_skill_level(ch,gsn_invoke)*6;
				af.bitvector = 0;
				affect_to_char( ch, &af );
				af.location	 = APPLY_RMAGIC;
				af.modifier	 = ch->level / 4 + get_skill_level(ch,gsn_invoke)*6;
				affect_to_char( ch, &af );
				// °l¥[¬r¯À­@©Ê 2021/11/04
				af.location	 = APPLY_SAVING_POISON;
				af.modifier	 = -100 - get_skill_level(ch,gsn_invoke)*100;
				affect_to_char( ch, &af );

				if(sklv > 5){
					af.location	 = APPLY_AC;
					af.modifier	 = -150;
					affect_to_char( ch, &af );
				}

				send_to_char( "§Aªº¦Ù½§¬ðµMÅÜ±o°í¦ü¿ûÅK¡I\n\r", ch );
				act( "$n ¥þ¨­¬ðµMµo¥X»þ¯PªºÓ\\®ð¡I", ch, NULL, NULL, TO_ROOM );
			}
			break;
		case 's':
		case 'S':
			if( !str_cmp("svs", arg)){
				if( sklv < 1 ) {
					send_to_char("§Aªº°@¸Û«×ÁÙ¤£°÷¡AµLªk¥l³ê¯«ÆF¡C\n\r", ch);
					return;
				}
				if ( !IS_NPC( ch ) )
				{
					if ( ch->pcdata->condition[COND_DRUNK] > 30 - sklv * 2 )
					{
						send_to_char( "³Ü¨º»ò¾K¡A¤£©ÈÁ¶Âp¯«©ú¶Ü¡I\n\r", ch );
						return;
					}
					if ( ( race_table[ch->race].thirst_mod  && ch->pcdata->condition[COND_THIRST] < 16)
							|| ( race_table[ch->race].hunger_mod && ch->pcdata->condition[COND_FULL] < 16 )
							|| ch->move < ch->max_move / 2 
							|| ch->move < 150
							|| ch->mana < 210)
					{
						send_to_char( "§A¨S¦³¨¬°÷ªººë¤O¡I\n\r", ch );
						return;
					}
					else
					{
						if( race_table[ch->race].thirst_mod )
							ch->pcdata->condition[COND_THIRST] -= 16;
						if( race_table[ch->race].hunger_mod )
							ch->pcdata->condition[COND_FULL] -= 16;
						ch->move -= 80+10*get_skill_level(ch,gsn_invoke);
						ch->mana -= 20+5*get_skill_level(ch,gsn_invoke);
					}
				}
				if( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))
					ch->alignment = UMAX( -1000, ch->alignment - 50 );

				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_SAVING_SPELL;
				af.modifier	 = -ch->level / 4-get_skill_level(ch,gsn_invoke)*2;
				af.bitvector = 0;
				affect_to_char( ch, &af );

				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_SAVING_ELEMENT;
				af.modifier	 = -ch->level / 4-get_skill_level(ch,gsn_invoke)*2;
				af.bitvector = 0;
				affect_to_char( ch, &af );

				// °l¥[¬r¯À­@©Ê 2021/11/04
				af.type	 = gsn_invoke;
				af.duration	 = 4+get_skill_level(ch,gsn_invoke)*3/2;
				af.location	 = APPLY_SAVING_POISON;
				af.modifier	 = -100 - get_skill_level(ch,gsn_invoke)*100;
				affect_to_char( ch, &af );

				if(sklv > 5){
					af.type      = gsn_invoke;
					af.duration  = 4 + get_skill_level(ch,gsn_invoke)*3/2;
					af.location  = APPLY_SAVING_BREATH;
					af.modifier  = -ch->level / 4 - get_skill_level(ch,gsn_invoke)*2;
					af.bitvector = 0;
					affect_to_char( ch, &af );
				}

				send_to_char( "ÂÇ¥Ñ¤Ñ¯«ªº¥[Å@¡A§Aªº©P¨­Å¢¸n¤@¼h¥ú®ð¡I\n\r", ch );
				act( "$n ¥þ¨­¬ðµMµo¥X»þ¯PªºÓ\\®ð¡I", ch, NULL, NULL, TO_ROOM );
			}
			break;
	}
	check_alignment_eq( ch );
	WAIT_STATE( ch, skill_table[gsn_invoke].beats );
}

void do_darkraid( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    int sklv = get_skill_level(ch,gsn_dark_raid);
	int BonusTime = sklv - 4;

	if (BonusTime < 0) BonusTime = 0;

	if ( ch->move > 300 - sklv * sklv * 4 )
		ch->move -= 300 - sklv * sklv * 4;
	else
	{
		send_to_char( "[36m§A¨S¦³¨¬°÷ªºÅé¤O¡I[m\n\r", ch );
		return;	
	}
		// sklv time bonus add at 2020/09/14
    	if ( time_info.hour > 4 + BonusTime && time_info.hour < 20  - BonusTime)
    	{
    		send_to_char( "[36m¤Ñ¦â¤ñ§A·Q¹³¤¤©ú«G¡A§A¥¢±Ñ¤F¡I[m\n\r", ch );
			return;    		
    	}

	if ( ( !IS_NPC( ch ) && number_percent() > get_skill_percent(ch, gsn_dark_raid) )
		|| ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) )
	{
		send_to_char( "³oªù§Þ¥©¹ï§A¦Ó¨¥¤Ó°ª²`¤F¡I\n\r", ch );
		return;	
	}

	send_to_char( "[36m§A¨Ì¾a©]±ßªº¹çÀR´£°ª±Mª`¤O¡A¬°¤U¤@À»§@¨¬·Ç³Æ¡I[m\n\r", ch );

	affect_strip( ch, gsn_dark_raid );

	af.type      = gsn_dark_raid;
	af.duration  = 5 + sklv;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_INFRARED;
	affect_to_char( ch, &af );

	af.bitvector = AFF_DARK_RAID;
	af.location  = APPLY_HITROLL;
	//af.modifier  = ( ch->level + get_curr_dex( ch ) ) / 6 + sklv + sklv/6;
	af.modifier  = 10 + get_curr_dex( ch ) * sklv * sklv / 10;
	affect_to_char( ch, &af );
	
	//sklv > 2  dr bonus add at 2020/09/14
	if( sklv > 2 ){
		af.bitvector = AFF_DARK_RAID;
		af.location  = APPLY_DAMROLL;
		af.modifier  = get_curr_dex( ch ) * sklv * sklv / 15;
		affect_to_char( ch, &af );
	}

	WAIT_STATE( ch, 1 );
}


/** protect «ü¥O³B²zfunction 
 * last update by zcecil 2004/07/05
 */
void do_protect( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	/* Don't allow charmed mobs to do this, check player's level */
	if ( IS_NPC(ch) && !is_real_npc(ch) ) 
	{
		if(!IsSorGateMobEarth(ch->pIndexData->vnum)){
			send_to_char( "§A¥¢±Ñ¤F¡C\n\r", ch);
			return;
		}
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§AµLªk¦b¥Û¤Æª¬ºA¤U¨Ï¥Î§Þ¥©.\n\r", ch);
		return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		if( !ch->protecting ) {
			send_to_char( "­n«OÅ@½Ö¡H\n\r", ch );
		}
		else {
			act( "§A¥¿¦b«OÅ@ªº¹ï¶H¬O$N¡C", ch, NULL, ch->protecting, TO_CHAR );
		}
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
	}

	if( ch->protecting ){
		if( victim != ch->protecting ) {
			stop_protecting(ch, ch->protecting);
		}
		else {
			act( "§A¤w¸g¦b«OÅ@$N¤F¡C", ch, NULL, ch->protecting, TO_CHAR );
			return;
		}
	}

	if( victim == ch ) {
		return;
	}

	if( ( !is_same_group( ch, victim))) {
		send_to_char("§A·F¹À³o»ò¦h¨Æ¡I\n\r", ch);
		return;
	}

	add_protector(ch, victim);
}

