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
//*  PURPOSE     :         ­×¥¿empslash¼Q¸Ë«ádrÃz¼W¤§bug          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940608 jye                                    *
//*  PURPOSE     :         ©ñ¥Xtwin swordsµ¹ppl´úbug              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         ­×§ïempslash ³B©óautoc stop®É¤£¦©move  *
//*              :             ¤Î¨S®³ªZ¾¹«o¦©move¤§bug            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940624 jye                                    *
//*  PURPOSE     :         ­×§ïtwin¦bmob¦º«áÁÙ·|°l¥´¤§bug         *
//*****************************************************************
/* $Id: skill.c 922 2005-05-10 07:30:11Z keric $ */
#include "skill.h"

//void check_alignment_eq( CHAR_DATA *ch );
//bool psionic_check(CHAR_DATA *ch, int sn );

void LizardmanSkill( CHAR_DATA *ch , CHAR_DATA *victim , int multi);

void do_empslash( CHAR_DATA * ch, CHAR_DATA* victim )
{
	int wp;
	OBJ_DATA *obj;
	int dr, net_dr, chk_dr;
	int hr, net_hr, chk_hr; // 2023/01/06
	int ac, dodge;
	int old_ac, old_dodge;
	int wait = 0, descrip = 0;
	int sklv = get_skill_level(ch,gsn_empslash);
	//int sklv_ber = get_skill_level(ch, gsn_berserk);
	//int level = UMAX( ch->level, 37 );
	//int vlevel = victim->level;  remove at 2023/01/06
	int limit_lv = 0;
	int break_sklv = get_skill_level(ch,gsn_break_shield);
	int level_diff = 0;
	int str_ch = 0;

	if( !can_see(ch, victim) ) {
		send_to_char("§A¬Ý¤£¨ì¥Ø¼Ð¡C\n\r",ch);
		return;
	}

	if( ( IS_NPC(ch) && !is_real_npc(ch) )
		|| !check_skill(ch,gsn_empslash, number_percent())){
		send_to_char( "§A¥¢±Ñ¤F¡C\n\r", ch);
		return;
	}

	if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
		if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
			send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
			return;
		}
		wp = WEAR_WIELD_2;
	}
	else
		wp = WEAR_WIELD;
      
	if(!IS_NPC(ch) ) {
		if( ch-> move < 140 + sklv*10 ){
			send_to_char("§Aªº²¾°Ê¤O¤£°÷¡C\n\r", ch);
			return;
		}
	}

	if( sklv < 6 && obj->value[3] != 1 && obj->value[3] != 3 ){
		send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
		return;
	}

	// autoc stop ªº§P©w¨S¦³·N¸q 2023/01/06
	//if( !IS_NPC(ch) && ch->pcdata->autocombat != AUTOC_STOP){
	if( !IS_NPC(ch) )
	{
		ch->move -= 100 + sklv * 25;
		// war ¨t¦³¸û°ªªº dr ¥[¦¨ 2023/01/06
		if( ch->pcdata->prime_attr == APPLY_STR )
			str_ch = 2;
	}
    
	do_add_qmark(victim, "self empslash_preset -1 1 0 0 1 0");

	descrip = number_range(0,2);
	switch(descrip){
		case 1:
			act(HIY"§AÅQ®ð¤@±Ù¡A®µ¤d­x¸U°¨¤§¶Õ´Â¼Ä¤H²rµM§ð¥h¡I"NOR, ch, NULL, NULL, TO_CHAR); 
			act(HIY"$n"HIY"ÅQ®ð¤@±Ù¡A®µ¤d­x¸U°¨¤§¶Õ´Â¼Ä¤H²rµM§ð¥h¡I"NOR, ch, NULL, NULL, TO_ROOM); 
			break;
		case 2:
			act(HIY"§A»´®Ì¼C¨­¡A¼C¥ú¦p¼Æ¹D«æ¹£ªº¬y¬P´Â¼Ä¤H©b¹£¦Ó¥h¡I"NOR, ch, NULL, NULL, TO_CHAR);
			act(HIY"$n"HIY"»´®Ì¼C¨­¡A¼C¥ú¦p¼Æ¹D«æ¹£ªº¬y¬P´Â¼Ä¤H©b¹£¦Ó¥h¡I"NOR, ch, NULL, NULL, TO_ROOM);
			break;
		default:
			if(ch->sex == SEX_FEMALE){
				act(HIY"©p¾®¯«¤@°{¡AÀþ®ÉµL±¡¦a´Â¼Ä¤H±Ù¥X¼Æ¹D¼C¼v¡I"NOR, ch, NULL, NULL, TO_CHAR);
				act(HIY"$n"HIY"¾®¯«¤@°{¡AÀþ®ÉµL±¡¦a´Â¼Ä¤H±Ù¥X¼Æ¹D¼C¼v¡I"NOR, ch, NULL, NULL, TO_ROOM);
			}else{
				act(HIY"§A¤j³Ü¤@Án ¤â¤¤¤§¼Cµo¥XµÂ±þ¤§®ð¡IÅQ¬Ó¼C·¥©Û¨Ï¥X¡I"NOR, ch, NULL, NULL, TO_CHAR);
				act(HIY"$n"HIY"¤j³Ü¤@Án ¤â¤¤¤§¼Cµo¥XµÂ±þ¤§®ð¡IÅQ¬Ó¼C·¥©Û¨Ï¥X¡I"NOR, ch, NULL, NULL, TO_ROOM);
			}
			break;
	}

	net_dr = dr = ch->damroll;
	// ¥[¤J hr §P©w 2023/01/06
	net_hr = hr = ch->hitroll;


	old_ac = GET_AC(victim);
	old_dodge = GET_DODGE(victim);

	ac = old_ac;
	dodge = old_dodge;

	// ¥¿­È§P©w 2023/01/06
	if( ac > 0 )
		ac = 0;
	if( dodge > 0 )
		dodge = 0;

	old_ac = victim->armor;
	old_dodge = victim->dodge;


	// ±N pc »P npc ¤À¶}¼g§P©w 2023/01/06
	if( IS_NPC(ch) )
	{
		ch->damroll = ( dr * (20 + sklv ) )/20; // dr ¥[¦¨
		victim->dodge -= ( dodge * ( 4 + sklv ) )/20; // dc ¤Ö 20% ¨ì 50%
		victim->armor -= ( ac * ( 4 + sklv ) )/30; // ac ¤Ö 13% ¨ì 33%
	}
	else
	{
		// sklv ¹ï dr ªº¥[¦¨
		if( ( sklv + str_ch ) > 3 )
			ch->damroll = ( dr * (sklv + str_ch) )/3;

		// sklv ¹ï hr ªº¥[¦¨ 2023/01/06
		ch->hitroll = ( hr * (70 + sklv * sklv ) )/70;

		// µ¥¯Å®t­È 2023/01/06
		level_diff = victim->level - ch->level;

		// µ¥¯Å 22 ¥H¤W¤~·|¦³µ¥®t¥[¦¨ , ¦ý¥¿±`±¡ªp¤U¤£·|µo¥Í¦b ppl ¨­¤W ( empslash »Ý lv 34 ¤~¯à¾Ç²ß )
		if( level_diff > 0 )
		{
			// ¨C­Ó level_diff ¼W¥[ dr , ³Ì°ª 3.5 ­¿
			ch->damroll += ( level_diff * ( 1 + sklv ) ) / 2;

			// µ¥®t¤W­­­È
			if( ch->level >= 41 )      limit_lv = 15;
			else if( ch->level >= 40 ) limit_lv = 10;
			else if( ch->level >= 39 ) limit_lv = 6;
			else if( ch->level >= 38 ) limit_lv = 6;
			else if( ch->level >= 37 ) limit_lv = 4;
			else if( ch->level >= 36 ) limit_lv = 4;
			else if( ch->level >= 35 ) limit_lv = 3;
			else if( ch->level >= 34 ) limit_lv = 3;
			else                       limit_lv = 2;

			// ¯}¬Þ³N¯àÃB¥~¼W¥[µ¥®t¤W­­­È
			limit_lv = limit_lv + break_sklv/2;

			limit_lv = UMIN( level_diff , limit_lv );

			victim->dodge -= ( dodge * limit_lv )/40; // dc ³Ì°ª­°§C 45%
			victim->armor -= ( ac * limit_lv )/45; // ac ³Ì°ª­°§C 40%
		}
	}

	/*  ­×§ï empslash µ¥¯Å¤Î sklv ¥[¦¨§P©w 2023/01/06

		// Modified by Keric for dr, 2005/2/13
		if( vlevel + sklv > level )
		{
			if ( sklv > 4 ){
				if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
					ch->damroll  = dr*5/2;
				}else{
					ch->damroll  = dr*2;
				}
				victim->dodge -= dodge*3/4; // ­° 37.5% DC 
				victim->armor -= ac*3/4 ;   // ­° 75%   AC
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
				// ¹ï¤W¶W°ªµ¥¯Å¹ï¤â ¨C¶W¹L¤W­­ªº lv ¼W¥[ 1 dr
				if ( vlevel - ch->level > limit_lv )
					ch->damroll += vlevel - ch->level - limit_lv;
			}
		}
	*/
  
	chk_dr = ch->damroll;//add by jye 0940607
	chk_hr = ch->hitroll;// 2023/01/06

	while(1)
	{
		victim->dodge -= dodge/7;   // ­° 14% DC
		mone_hit( ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE );
		wait += ch->wait;
		ch->wait = 0;
		mone_hit( ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE );
		wait += ch->wait;
		ch->wait = 0;
		if ( IS_NPC(ch))
		{
			mone_hit( ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE );
			mone_hit( ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE );
		}

		if (!IS_NPC(ch))
		{
			if ( ch->fighting != victim || sklv < 1 )
				;//do nothing
			else
			{
				mone_hit( ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE );
				wait += ch->wait;
				ch->wait = 0;
			}

			if ( ch->fighting != victim || sklv < 3 )
				;
			else
			{
				mone_hit( ch, victim, &gsn_empslash, wp, MSG_NORMAL, DT_NOCHANGE );
				wait += ch->wait;
				ch->wait = 0;
			}
		}
		break;
	}

	if( is_qmark(victim, "empslash_preset")){
		// npc ªº del qmark ¥²¶·¨Ï¥Î mpdel_qmark 2022/04/14
		if( IS_NPC(victim) )
			do_mpdel_qmark(victim, "self empslash_preset");
		else
			do_del_qmark(victim, "self empslash_preset");
		victim->armor = old_ac ;
		victim->dodge = old_dodge ;
	}
  
	if ( ch->damroll != 0 )//ch may die! damroll may set to 0!
	{
		// modified by jye 6.07.2005
		// ¼W¥[chk_dr°O¿ý§PÂ_§ðÀ»«e¤§dr
		// ¦bÁÙ­ìppl dr«e¥ýchk_dr(¥ý)´îch->damroll(«á)¥Ñ¤£¬°0ªí¥Ü¦³¼Q¸Ë
		// ¦bÁÙ­ìppl dr®É¦A´î±¼¼Q¸Ë¤§dr
		if ( ch->position == POS_FIGHTING ){
			ch->damroll = dr - (chk_dr - ch->damroll);
		}else{
			ch->damroll = net_dr - (chk_dr - ch->damroll);
		}
	}

	// ¼W¥[ hr ÁÙ­ì§P©w 2023/01/06
	if ( ch->hitroll != 0 )
	{
		if ( ch->position == POS_FIGHTING ){
			ch->hitroll = hr - (chk_hr - ch->hitroll);
		}else{
			ch->hitroll = net_hr - (chk_hr - ch->hitroll);
		}
	}


	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

	WAIT_STATE( ch, skill_table[gsn_empslash].beats );
	WAIT_STATE( ch, wait);
}

//modifyed by jye 6.08 2005
void do_twin( CHAR_DATA *ch, char *argument )/*{{{*/
{
	CHAR_DATA *victim, *rch = NULL;
	OBJ_DATA  *obj;
	int wp, dam;
	int cost = 0;
	int sn = skill_lookup("twin swords");
	int sklv = get_skill_level(ch,sn);
	int rsklv = 0;
	int intToken = 0;

  /* Don't allow charmed mobs to do this, check player's level */
  //if( ( IS_NPC( ch ) && !is_real_npc(ch)) 
  //     || ( !IS_NPC( ch ) && check_skill(ch, gsn_twin, number_percent( )  )))
  //{
  //     send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
  //     return;
  //}
  
	if( !ch->fighting )
	{
		send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
		return; 
	}
  
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "³o¸Ì¸T¤î¾Ô°«. \n\r",ch);
		return;
	}

	if( !can_see(ch, ch->fighting) ) 
	{
		send_to_char("§A¬Ý¤£¨ì¥Ø¼Ð¡C\n\r",ch);
		return;
	}

	if( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) )
	{
		if( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) )
		{
			send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
			return;
		}
		wp = WEAR_WIELD_2;
	}
	else
		wp = WEAR_WIELD;

	if( !IS_NPC(ch) && sklv < 6 && obj->value[3] != 1 && obj->value[3] != 3 )
	{
		send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
		return;
	}

	if( ( IS_NPC(ch) && !is_real_npc(ch) ) || !check_skill(ch,gsn_twin, number_percent()))
	{
		send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
		return;
	}

	if( !IS_NPC(ch) ) 
	{
		// ­×§ï¬°§P©w cooperator 2021/11/23
		//for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
		for( rch = ch->cooperator ; rch; rch = rch->next_in_coop )
		{
			if ( !IS_NPC(rch)
				&& rch->class == SWORDSMAN 
				&& rch != ch
				&& IS_AWAKE( rch )
				//ch ¤]»Ý cooperate rch , ¯à«ü©w°t¦XªÌ 2021/11/23
				//&& rch->pcdata->autocombat == AUTOC_COOPERATE
				//&& is_same_group( rch, ch ))
				&& ( ch->cooperating == rch )
				&& ( ch->in_room == rch->in_room ) )
			{
				intToken = 1;
				if( ch->protecting == rch)
				{
					act("§A¥¿¥þ¤ßÅ@½ÃµÛ$N¡C",ch,NULL,rch,TO_CHAR);
					return;
				}
				if( rch->protecting == ch)
				{
					intToken = 2;
					break;
				}
			}
		}

		/*if( !rch )
			switch( intToken )
			{  
				default:
					send_to_char( "§A§ä¤£¨ì¤H°t¦X§A¡C\n\r", ch );
					return;
				case 1:
					send_to_char( "¨Ã¨S¦³¶¤¤Í¥þ¤ßÅ@½ÃµÛ§A¡C\n\r", ch );
					return;
			}
		*/
		if( !rch || ( intToken == 0 ) ){
			send_to_char( "§A§ä¤£¨ì¤H°t¦X§A¡C\n\r", ch );
			return;
		}
		if( ( intToken != 2 ) ){
			send_to_char( "¦P¦ñ¨Ã¨S¦³¥þ¤ßÅ@½ÃµÛ§A¡C\n\r", ch );
			return;
		}
	}
	else 
	{
		// ­×§ï¬°§P©w cooperator 2021/11/23
		//for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
		for( rch = ch->cooperator ; rch; rch = rch->next_in_coop )
		{
			if ( rch != ch
				&& IS_AWAKE( rch )
				&& ( ch->cooperating == rch )
				&& ( ch->in_room == rch->in_room ) )
			{
				intToken = 2;
				break;
			}
		}
		if( intToken != 2 )
		{
			send_to_char( "§A§ä¤£¨ì¤H°t¦X§A¡C\n\r", ch );
			return;
		}
	}


	if( !IS_NPC(ch) && !check_skill(rch, gsn_twin,number_percent()) )
	{
		act("$NµLªk¦¨¥\\°t¦X§A¡C",ch,NULL,rch,TO_CHAR);
		return;
	}

	cost = UMIN( ch->max_move/5 , rch->max_move/5 );

	if( !IS_NPC(ch) && ( ch->move < cost || rch->move < cost ) )
	{
		send_to_char( "§A­ÌÄ±±o¦n§x¥F¡C\n\r", ch );
		return;
	}

	rsklv = get_skill_level(rch, sn);

	if( !IS_NPC(ch) )
		ch->move -= cost;

	if( !IS_NPC(rch) )
		rch->move -= cost;

	if( !IS_NPC(ch) )
		waste_dura(obj);

	//act( "$n",   ch, NULL, rch, TO_ROOM    );
	act("¦b$NªºÅ@½Ã¤U¡A§A¥þ¤O¬I®i¼C§Þ¡I", ch, NULL, rch, TO_CHAR);
	act("§A¦¬°_§ð¶Õ¡A¥þ¤ßÅ@½Ã$n¡C", ch, NULL, rch, TO_VICT);
	act("$n¤@Án¤j³Ü¡A»P$N¤@¦P¬I®i¥X¡u"HIY"¤é"HIW"¤ë"NOR"Âù¼C"NOR"¡v¡I", ch, NULL, rch, TO_NOTVICT);

	victim = ch->fighting;

	if( victim->deleted || victim->in_room != ch->in_room )
		return;

	if( !IS_NPC(ch) )
		dam = dam + cost/2;

	if( !IS_NPC(ch) )
		dam = UMIN( 1000 , ( rch->armor / - ( 8 - rsklv ) ) ) + ch->damroll * (sklv*3 + 2) + ( rch->played + ch->played) / 100000;
	else
		dam = (ch->level + ch->damroll/2) * (1 + sklv + rsklv);

	dam = dam + rch->pdr;
	
	dam = dam - victim->pdr;

	dam = dam * weakness_damage(victim, sn ) / 100;

	if( IS_NPC(victim) && dam <= victim->pad )
		dam = 0;

	if( victim->deleted || victim->in_room != ch->in_room )
		return;
	victim->hit -= dam;
	dam_message( ch, victim, dam, gsn_twin, WEAR_NONE );
	////brief_damage( ch, victim, 0, gsn_twin, WEAR_NONE);
	damage( ch, victim, 0, gsn_twin, WEAR_NONE, MSG_BRIEF, DMG_NORMAL );

	WAIT_STATE( ch, skill_table[gsn_twin].beats - sklv );
	WAIT_STATE( rch, skill_table[gsn_twin].beats - rsklv );

	if( sklv + rsklv > 9 && !victim->deleted )
	{
		if( victim->deleted || victim->in_room != ch->in_room )
			return;
		victim->hit -= dam;
		dam_message( ch, victim, dam, gsn_twin, WEAR_NONE );
		////brief_damage( ch, victim, 0, gsn_twin, WEAR_NONE);
		damage( ch, victim, 0, gsn_twin, WEAR_NONE, MSG_BRIEF, DMG_NORMAL );
	}

	if( sklv + rsklv > 11 && !victim->deleted )
	{
		if( victim->deleted || victim->in_room != ch->in_room )
			return;
		victim->hit -= dam;
		dam_message( ch, victim, dam, gsn_twin, WEAR_NONE );
		////brief_damage( ch, victim, 0, gsn_twin, WEAR_NONE);
		damage( ch, victim, 0, gsn_twin, WEAR_NONE, MSG_BRIEF, DMG_NORMAL );
	}

	//update_pos(victim);
	if( !victim->deleted && victim->in_room == ch->in_room ) disarm(ch, victim);


	//WAIT_STATE( ch, skill_table[gsn_twin].beats - sklv );
	//WAIT_STATE( rch, skill_table[gsn_twin].beats - rsklv );
	return;
}/*}}}*/


// flaming slash modified by whisperwind 2.17.2005
void do_flaming( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int   wp       = WEAR_NONE;
    int sklv       = get_skill_level(ch, gsn_flaming_slash);

    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_flaming_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && (ch->move < 30 * (1 + sklv) || ch->mana < 43 * (1 + sklv)) ){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
    }else if ( !IS_NPC(ch)){
        ch->move -= 30 * (1 + sklv);
        ch->mana -= 43 * (1 + sklv);
    }

    victim = ch->fighting;
    if ( victim->deleted || victim->in_room != ch->in_room ) return;
        
    if ( !check_blind( ch ) ) return;

    // ²Ä¤@¬q¼s°ì¤õµKªi     
    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        act(HIR"§A¤â¤¤´§»RµÛªø¼C¡A±j¯Pªº¼C®ð±²°_¤j¶qªº¤õ¿V¡I"NOR, ch, NULL, NULL, TO_CHAR); 
        act("$n"HIR"¤â¤¤´§»RµÛªø¼C¡A±j¯Pªº¼C®ð±²°_¤j¶qªº¤õ¿V¡I"NOR, ch, NULL, NULL, TO_ROOM); 
    }else{
        act(HIR"§A±N©À¤O¾®»E¦¨Å]ªk¼C¡A±j¯Pªº¼C®ð±²°_¤j¶qªº¤õ¿V¡I"NOR, ch, NULL, NULL, TO_CHAR); 
        act("$n"HIR"±N©À¤O¾®»E¦¨Å]ªk¼C¡A±j¯Pªº¼C®ð±²°_¤j¶qªº¤õ¿V¡I"NOR, ch, NULL, NULL, TO_ROOM); 
    }
    
    spell_flaming_slash( -1, ch->level, ch, victim );
    
    if( sklv > 5 ){
        spell_flaming_slash( -1, ch->level, ch, victim );
    }
      
    if( get_skill_level(ch, skill_lookup("fireball")) > 3 && sklv > 3){
        spell_flaming_slash( -1, ch->level, ch, victim );
    }
    
    // ²Ä¤G¬q±j¤Æ¤õµKªi
    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if( IS_GOOD(ch) ){
            act(HIR"¤@¹DÄ£²´ªº¤õ¬W¦Û¤Ñ¦Ó­°¡A»P§Aªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIR"¤@¹DÄ£²´ªº¤õ¬W¦Û¤Ñ¦Ó­°¡A»P"NOR"$n"HIR"ªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIR"¤@¹D¶Â¦â¤õ¬W±q¦a­±«_¥X¡A»P§Aªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIR"¤@¹D¶Â¦â¤õ¬W±q¦a­±«_¥X¡A»P"NOR"$n"HIR"ªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIR"¤@¹Dºë¯«§Î¦¨ªº¤õµK±q§A¤â¤¤©¹¼C¤W©µ¦ù¡A»P§Aªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIR"¤@¹Dºë¯«§Î¦¨ªº¤õµK±q"NOR"$n"HIR"¤â¤¤©¹¼C¤W©µ¦ù¡A»P$eªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
        waste_dura(obj);
        mone_hit( ch, victim, &gsn_flaming_slash, wp, MSG_NORMAL, DT_NOCHANGE );
    }else{
        if( IS_GOOD(ch) ){
            act(HIR"¤@¹DÄ£²´ªº¤õ¬W¦Û¤Ñ¦Ó­°¡A»P§Aªº©À¤O²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIR"¤@¹DÄ£²´ªº¤õ¬W¦Û¤Ñ¦Ó­°¡A»P"NOR"$n"HIR"ªº©À¤O²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIR"¤@¹D¶Â¦â¤õ¬W±q¦a­±«_¥X¡A»P§Aªº©À¤O²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIR"¤@¹D¶Â¦â¤õ¬W±q¦a­±«_¥X¡A»P"NOR"$n"HIR"ªº©À¤O²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIR"¤@¹Dºë¯«§Î¦¨ªº¤õµK±q§A²´¤¤©¹Å]ªk¼C¤W©µ¦ù¡A»P§Aªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIR"¤@¹Dºë¯«§Î¦¨ªº¤õµK±q"NOR"$n"HIR"²´¤¤©¹Å]ªk¼C¤W©µ¦ù¡A»P$eªº¼C®ð²£¥Í¦@»ï¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
    }

    if ( ch->fighting != victim ) return;
  
    spell_flaming_slash2( -1, ch->level, ch, victim );

    if ( ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room){
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }
	
	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

    WAIT_STATE( ch, skill_table[gsn_flaming_slash].beats - sklv );
    return;
}

// icecore slash add by whisperwind 2.17.2005
void do_icecore( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int        wp  = WEAR_NONE;
    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_icecore_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && (ch->move < 85 || ch->mana < 85)){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
    }
/*	else if ( !IS_NPC(ch)){
        ch->move -= 180;
        ch->mana -= 200;
    }
*/
    victim = ch->fighting;
    if ( victim->deleted || victim->in_room != ch->in_room ) return;

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if( IS_GOOD(ch) ) {
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F§AªºªZ¾¹¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X·À¼Äªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F"NOR"$n"HIC"ªºªZ¾¹¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X·À¼Äªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F§AªºªZ¾¹¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X¦º¤`ªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F"NOR"$n"HIC"ªºªZ¾¹¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X¦º¤`ªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIC"¤@ªÑ­á®ðÅ¢¸n¤F§AªºªZ¾¹¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X±j¯Pªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIC"¤@ªÑ­á®ðÅ¢¸n¤F"NOR"$n"HIC"ªºªZ¾¹¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X±j¯Pªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
        waste_dura(obj);
        mone_hit( ch, victim, &gsn_icecore_slash, wp, MSG_NORMAL, DT_NOCHANGE );
    }else{
        if( IS_GOOD(ch) ) {
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F§AªºÆF¼C¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X·À¼Äªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F"NOR"$n"HIC"ªºÆF¼C¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X·À¼Äªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F§AªºÆF¼C¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X¦º¤`ªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIC"¤@ªÑ´H®ðÅ¢¸n¤F"NOR"$n"HIC"ªºÆF¼C¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X¦º¤`ªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIC"¤@ªÑ­á®ðÅ¢¸n¤F§AªºÆF¼C¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X±j¯Pªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIC"¤@ªÑ­á®ðÅ¢¸n¤F"NOR"$n"HIC"ªºÆF¼C¡A¹ï²´«e©Ò¦³¼Ä¤H»R¥X±j¯Pªº¦B¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
    }
    
    spell_icecore_slash( -1, ch->level, ch, victim );

	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

    if ( ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room){
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }
    WAIT_STATE( ch, skill_table[gsn_icecore_slash].beats );
    return; 
}

// enables beholder performing magical slash under sklv 6 inborn ability, added by Keelar
// fulgurant slash add by whisperwind 2.17.2005
void do_fulgurant( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    ROOM_INDEX_DATA *	pFightRoom = NULL;

    int wp      = WEAR_NONE;
    int times   = 2; //¬q¼Æ³]©w
    int sklv    = get_skill_level(ch, gsn_fulgurant_slash);

    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_fulgurant_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && (ch->move < 13 || ch->mana< 13)){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
	}
 /*   else if ( !IS_NPC(ch)){
        ch->move -= 100;
        ch->mana -= 150;
    }
 */
    victim = ch->fighting;
    if ( victim->deleted || victim->in_room != ch->in_room )
        return;

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if( IS_GOOD(ch) ) {
            act(HIW"§Aªº¼C®ð¤Û¤Æ¦¨¥Õ¦âªº¯e¹q¡A±i¤ú»R¤ö¦a¼A¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"ªº¼C®ð¤Û¤Æ¦¨¥Õ¦âªº¯e¹q¡A±i¤ú»R¤ö¦a¼A¦V$eªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIW"§Aªº¼C®ð¤Û¤Æ¦¨µµ¦âªº¨g¹q¡A±i¤ú»R¤ö¦aÅ§¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"ªº¼C®ð¤Û¤Æ¦¨µµ¦âªº¨g¹q¡A±i¤ú»R¤ö¦aÅ§¦V$eªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIW"§Aªº¼C®ðÅÜ¤Û¦¨«C¦âªº©b¹q¡A±i¤ú»R¤ö¦a¼A¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"ªº¼C®ðÅÜ¤Û¦¨«C¦âªº©b¹q¡A±i¤ú»R¤ö¦a¼A¦V$eªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
        waste_dura(obj);
        mone_hit( ch, victim, &gsn_fulgurant_slash, wp, MSG_NORMAL, DT_NOCHANGE );
    }else{
        if( IS_GOOD(ch) ) {
            act(HIW"§Aªººë¯«¤O¤Û¤Æ¦¨¥Õ¦âªº¯e¹p¼C¡A±i¤ú»R¤ö¦a¼A¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"ªººë¯«¤O¤Û¤Æ¦¨¥Õ¦âªº¯e¹p¼C¡A±i¤ú»R¤ö¦a¼A¦V$eªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIW"§Aªººë¯«¤O¤Û¤Æ¦¨µµ¦âªº¨g¹p¼C¡A±i¤ú»R¤ö¦aÅ§¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"ªººë¯«¤O¤Û¤Æ¦¨µµ¦âªº¨g¹p¼C¡A±i¤ú»R¤ö¦aÅ§¦V$eªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIW"§Aªººë¯«¤OÅÜ¤Û¦¨«C¦âªº©b¹p¼C¡A±i¤ú»R¤ö¦a¼A¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"ªººë¯«¤OÅÜ¤Û¦¨«C¦âªº©b¹p¼C¡A±i¤ú»R¤ö¦a¼A¦V$eªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
    }

    if ( ch->fighting != victim )
        return;
  
    if ( !check_blind( ch ) )
        return;

    if( sklv > 1 )
        times += 1;
    if( sklv > 3 )
        times += 1;
    if( ch->level > 36 )
        times += 1;
    if( ch->level > 38 )
        times += 1;
    if( ch->level > 40 )
        times += 1;
    
    times = UMIN(times, 6);

    if( sklv > 5 )
        times += 1;

  // maximize fulgurant part      
    if( !IS_NPC(ch) && sklv > 3 ) {
        times += UMAX(0,get_skill_level(ch, gsn_swordmaster));
		if( get_skill_level(ch, gsn_swordmaster) > 4) times += 1;
		if( get_skill_level(ch, gsn_swordmaster) > 5) times += 1;
    }
 
    for(pFightRoom = victim->in_room; 
        times > 0 && !victim->deleted && pFightRoom == ch->in_room && pFightRoom == victim->in_room;
        times--){
        if ( ch->mana < 13 || ch->move < 13) break;
        spell_fulgurant_slash( -1, ch->level, ch, victim );
    }

    if ( ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room){
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

    WAIT_STATE( ch, skill_table[gsn_fulgurant_slash].beats );
    return;
}

// storm slash modified by whisperwind 2.18.2005
void do_storm( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int   wp    = WEAR_NONE;
    int sklv    = get_skill_level(ch, gsn_storm_slash);

    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_storm_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 && obj->value[3] != 4){
            send_to_char( "¨g­·±Ù­n¥Î¼C©ÎÃ@¬I®i¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }
      
    if ( !IS_NPC(ch) && (ch->move < 15 || ch->mana< 15)){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
    }
/*	else if ( !IS_NPC(ch)){
        ch->move -= 150;
        ch->mana -= 180;
    }
*/
    victim = ch->fighting;
    if ( victim->deleted || victim->in_room != ch->in_room )
        return;

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if( IS_GOOD(ch) ) {
            act(HIG"§A§Ö³t´§°Ê§L¾¹¡A¥O©P³òªÅ®ð¤Æ¬°±j«lªº±Û­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIG"§Ö³t´§°Ê§L¾¹¡A¥O©P³òªÅ®ð¤Æ¬°±j«lªº±Û­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIG"§A§Ö³t´§°Ê§L¾¹¡A¥O©P³òªÅ®ð¤Æ¬°ÀI´cªº³±­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIG"§Ö³t´§°Ê§L¾¹¡A¥O©P³òªÅ®ð¤Æ¬°ÀI´cªº³±­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIG"§A§Ö³t´§°Ê§L¾¹¡A±Ù¥X¼Æ¹D¯uªÅªi¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIG"§Ö³t´§°Ê§L¾¹¡A±Ù¥X¼Æ¹D¯uªÅªi¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
        waste_dura(obj);
        mone_hit( ch, victim, &gsn_storm_slash, wp, MSG_NORMAL, DT_NOCHANGE );
    }else{
        if( IS_GOOD(ch) ) {
            act(HIG"§A±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A¥O©P³òªÅ®ð¤Æ¬°±j«lªº±Û­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIG"±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A¥O©P³òªÅ®ð¤Æ¬°±j«lªº±Û­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(HIG"§A±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A¥O©P³òªÅ®ð¤Æ¬°ÀI´cªº³±­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIG"±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A¥O©P³òªÅ®ð¤Æ¬°ÀI´cªº³±­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(HIG"§A±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A±Ù¥X¼Æ¹D¯uªÅªi¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIG"±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A±Ù¥X¼Æ¹D¯uªÅªi¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
    }

    if ( ch->fighting != victim ) return;
  
    if ( !check_blind( ch ) ) return;

    spell_storm_slash( -1, ch->level, ch, victim );
    
    if( !IS_NPC(ch) && get_skill_level(ch, skill_lookup("whirlwind")) > 4 && sklv > 3 && ch->mana > 30) {
        if( IS_GOOD(ch) ) {
            act(HIG"±Û­·³vº¥ÂX¤j¡AÂà²´¶¡¤Æ¦¨¥¨¤jªºÀs±²­·¼É¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIG"±Û­·³vº¥ÂX¤j¡AÂà²´¶¡¤Æ¦¨¥¨¤jªºÀs±²­·¼É¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ){
            act(HIG"³±­·²r¯PÂ«°Ê¡AÅÜ¤Û¦¨¨g¯PªºÅ]©Ê¤§­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIG"³±­·²r¯PÂ«°Ê¡AÅÜ¤Û¦¨¨g¯PªºÅ]©Ê¤§­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else{
            act(HIG"¯uªÅªi¼¹µõ¤F©P¾Dªº¤j®ð¡A§Î¦¨§Q¤b¯ëªº±Û­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act(HIG"¯uªÅªi¼¹µõ¤F©P¾Dªº¤j®ð¡A§Î¦¨§Q¤b¯ëªº±Û­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
        spell_whirlwind( gsn_storm_slash, ch->level, ch, victim );
        ch->mana -= 30;
        if ( get_skill_level(ch, skill_lookup("whirlwind")) > 5 && ch->mana > 30){
            spell_whirlwind( gsn_storm_slash, ch->level, ch, victim );
            ch->mana -= 30;
        }
    }

    if ( ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room){
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

    WAIT_STATE( ch, skill_table[gsn_storm_slash].beats );
    return;
}

void do_elemental( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int   wp       = WEAR_NONE;
    int nFlameSklv  = get_skill_level(ch, gsn_flaming_slash);
    int nStormSklv  = get_skill_level(ch, gsn_storm_slash);
    int nIceCoSklv  = get_skill_level(ch, gsn_icecore_slash);
    int nFulguSklv  = get_skill_level(ch, gsn_fulgurant_slash);
    int nEarthSklv  = get_skill_level(ch, gsn_earth);

    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_elemental_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }

    if (!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }

    if (nFlameSklv + nIceCoSklv + nStormSklv + nEarthSklv + nFulguSklv < 19){
        send_to_char( "§A¹ï¤¸¯À¼Cªº¤F¸ÑÁÙ¤£°÷ºë´ï¡I\n\r", ch);
        return;
    }

    if ( !IS_NPC(ch) && ch->hit > ch->max_hit / 2 ){
        send_to_char( "§Aªº¦M¾÷·NÃÑ¤£¨¬¥H³ê°_§A¼ç¦bªº¤O¶q¡I\n\r", ch);
        return;
    }

    if ( !IS_NPC(ch) && (ch->move < 150 || ch->mana< 150)){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
    }else if ( !IS_NPC(ch)){
        ch->move -= 150;
        ch->mana -= 150;
    }

    victim = ch->fighting;
    if ( victim->deleted || victim->in_room != ch->in_room )
        return;

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        act(HIG"§A§Ö³t´§°Ê§L¾¹¡A±Ù¥X¼Æ¹D¯uªÅªi¡I"NOR, ch, NULL, NULL, TO_CHAR); 
        act("$n"HIG"§Ö³t´§°Ê§L¾¹¡A±Ù¥X¼Æ¹D¯uªÅªi¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        waste_dura(obj);
        mone_hit( ch, victim, &gsn_elemental_slash, wp, MSG_NORMAL, DT_NOCHANGE );
    }else{
        act(HIG"§A±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A¥O©P³òªÅ®ð¤Æ¬°±j«lªº±Û­·¡I"NOR, ch, NULL, NULL, TO_CHAR); 
        act("$n"HIG"±Nºë¯«¤O¤Û¤Æ¬°­¸»RªºÅ]ªk¼C¡A¥O©P³òªÅ®ð¤Æ¬°±j«lªº±Û­·¡I"NOR, ch, NULL, NULL, TO_ROOM); 
    }

    spell_elemental_slash( -1, ch->level, ch, victim );
    
    if ( ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room){
        stop_fighting(ch, FALSE);
        set_fighting(ch, victim);
    }

    WAIT_STATE( ch, skill_table[gsn_elemental_slash].beats );
    return;
}

void do_sun(  CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int   wp    = WEAR_NONE;
    int sklv    = get_skill_level(ch, gsn_sun_slash);

    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_sun_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }

    if ( !IS_NPC( ch ))
    {
        if ( IS_EVIL( ch ) )
        {
            send_to_char( "§Aªº¤ßÆFµLªk¾®»E¯«¸tªº¤O¶q.\n\r", ch );
            return;
        }
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && (ch->mana < 35 || ch->move < 70)){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
    }
/*	else if ( !IS_NPC(ch)){
        ch->move -=  220;
        ch->mana -=  180;
    }
*/
    victim = ch->fighting;
    if ( victim->deleted || victim->in_room != ch->in_room ) return;

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if(IS_GOOD(ch)){
            act(HIW"§A¾®»E¤F¤é»P¤ëªº¯«¸t¤O¶q¡A¨Ï¥XÄ£²´µL¤ñªº¥ú¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"¾®»E¤F¤é»P¤ëªº¯«¸t¤O¶q¡A¨Ï¥XÄ£²´µL¤ñªº¥ú¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else{
            act(HIW"§A¤â¤¤ªºªZ¾¹µo¥X¸U¤Vªº¥ú¨~¡A±Ù¥XµL¼Æ¹D¥ú¤§¼C¤b¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIW"¤â¤¤ªºªZ¾¹¬ðµMµo¥X¸U¤Vªº¥ú¨~¡A±Ù¥XµL¼Æ¹D¥ú¤§¼C¤b¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
        waste_dura(obj);
        mone_hit( ch, victim, &gsn_sun_slash, wp, MSG_NORMAL, DT_NOCHANGE );
    }else{
        if(IS_GOOD(ch)){
            act(HIY"§A±N¤é»P¤ëªº¯«¸t¤O¶q¤Û¤Æ¬°¥ú¼C¡A¨Ï¥XÄ£²´µL¤ñªº¥ú¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIY"±N¤é»P¤ëªº¯«¸t¤O¶q¤Û¤Æ¬°¥ú¼C¡A¨Ï¥XÄ£²´µL¤ñªº¥ú¤§¼C»R¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }else{
            act(HIY"§A±Nºë¯«¤O¤Û¤Æ¬°µL¼ÆÄ£²´ªº¥ú¤§¼C¤b¡A­¸¦V§Aªº¼Ä¤H¡I"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"HIY"±Nºë¯«¤O¤Û¤Æ¬°µL¼ÆÄ£²´ªº¥ú¤§¼C¤b¡I"NOR, ch, NULL, NULL, TO_ROOM); 
        }
    }

    victim = ch->fighting;
	if(get_skill_level( ch, gsn_sun_flash) >= 4 && (ch->mana >= 80 && ch->move >= 70)){
		spell_sun_flash( gsn_sun_slash, ch->level, ch, victim );
		ch->mana -= 80; ch->move -= 70;
	}
	if(get_skill_level( ch, gsn_sun_flash) >= 6 && (ch->mana >= 80 && ch->move >= 70)){
		spell_sun_flash( gsn_sun_slash, ch->level, ch, victim );
		ch->mana -= 80; ch->move -= 70;
	}
    if(IS_NPC(ch) || (ch->mana >= 35 && ch->move >= 70)){
        spell_sun_slash( -1, ch->level, ch, victim );
        ch->mana -= 35; ch->move -= 70;
    }
    if(IS_NPC(ch) || (ch->mana >= 35 && ch->move >= 70 && sklv >= 4)){
        spell_sun_slash( -1, ch->level, ch, victim );
        ch->mana -= 35; ch->move -= 70;
    }
    if(IS_NPC(ch) || (ch->mana >= 35 && ch->move >= 70 && sklv >= 6)){
        spell_sun_slash( -1, ch->level, ch, victim );
        ch->mana -= 35; ch->move -= 70;
    }
	
	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);
	
    WAIT_STATE( ch, skill_table[gsn_sun_slash].beats );
}

void do_sprinkle( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj = NULL;
    int   wp       = WEAR_NONE;
    char arg  [MAX_INPUT_LENGTH]; // add at 2020/09/21
    int sklv = get_skill_level(ch, gsn_sprinkle_slash);
	// ªi¯¾¼C one_hit ¶Ë®`©R¤¤¥[¦¨
	int hr_bonus = sklv * 20;
	int dr_bonus = sklv * 40;

	one_argument( argument, arg ); // add at 2020/09/21
	if( !ch->in_room ) return;    // add at 2020/09/21
    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || (   !IS_NPC( ch ) 
            &&   get_skill_percent(ch, gsn_sprinkle_slash) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }
	/*  ·s¼W«D¾Ô°«¤¤¨Ï¥Î¤Î«ü©w¥Ø¼Ð, ¬G²¾°£ 2020/09/21
    if ( !ch->fighting ){
        send_to_char( "§A¤£¬O¦b¾Ô°«¤¤¡C\n\r", ch );
        return; 
    }
	*/

	// safe room  2020/09/21
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) ){
		send_to_char( "³o¸Ì¸T¤î¾Ô°«. \n\r",ch);
		return;
	}

    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("§AµLªk¦b¥Û¤Æª¬ºA¤U¨Ï¥Î§Þ¥©.\n\r", ch);
        return;
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && sklv < 6 && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }
	
	if ( !check_blind( ch ) ) return;

    if ( !IS_NPC(ch) && (ch->move < 40 * (1 + sklv) || ch->mana < 40 * (1 + sklv) )){
        send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
        return;
    }
	
	if (!IS_NPC(ch)){
        ch->move -= 40 * (1 + sklv);
        ch->mana -= 40 * (1 + sklv);
    }

	//victim = ch->fighting;
	// ·s¼W«D¾Ô°«¤¤¨Ï¥Î      2020/09/21
	// µL«ü©w®É§P©w¦³µL·í«e¾Ô°«¥Ø¼Ð
	if ( arg[0] == '\0' ){
		if ( !ch->fighting ){
			send_to_char( "§A­n¹ï½Ö¨Ï¥Îªi¯¾¼C¡H\n\r", ch );
			return; 
		}else{
			victim = ch->fighting;
		}
	}else{
		if ( !( victim = get_char_room( ch, arg ) ) || !can_see(ch, victim) ){
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}
        if( is_same_group( ch, victim ) ) {
            send_to_char("[33m§A¤£¯à§ðÀ»¦Û¤vªº¶¤¤Í¡C[0m\n\r", ch);
            return;
        }
	}
	
	if ( victim->deleted || victim->in_room != ch->in_room )
		return;

	act(HIC"¡ã¡ã¡ã¡ã¡ãªi   ¯¾   ¼C¡ã¡ã¡ã¡ã¡ã"NOR, ch, NULL, NULL, TO_CHAR); 
	act(HIC"¡ã¡ã¡ã¡ã¡ãªi   ¯¾   ¼C¡ã¡ã¡ã¡ã¡ã"NOR, ch, NULL, NULL, TO_ROOM); 
	if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)) {
		waste_dura(obj);
	}
	
	//if ( ch->fighting != victim ) return;
	//if ( !check_blind( ch ) ) return;
	
	// ªi¯¾¼C one_hit ¶Ë®`©R¤¤¥[¦¨
	ch->hitroll += hr_bonus;
	ch->damroll += dr_bonus;
	one_hit( ch, victim, gsn_sprinkle_slash, wp );
	ch->hitroll -= hr_bonus;
	ch->damroll -= dr_bonus;
    
	spell_sprinkle_slash( -1, ch->level+sklv*2, ch, victim );

	if ( ch->fighting != victim && victim->in_room && victim->in_room == ch->in_room){
		stop_fighting(ch, FALSE);
		set_fighting(ch, victim);
	}
	
	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

	int nChance = 25 + sklv * 15;
	nChance  = nChance * (old_saves_spell( ch->pp , victim,MAGIC_COLD)) / 100;
	nChance  = nChance * weakness_percent(victim, gsn_sprinkle_slash) / 100;

	WAIT_STATE( ch, 9 - sklv );
	if ( victim->deleted ) return;
	if(number_percent() < nChance && number_percent() > (get_curr_dex(victim) - 10) * 2){
		AFFECT_DATA af;
		int nSlowAct = -10 - sklv * (sklv + 4) * 2 / 3;

		act(HIC"§Aªºªi¯¾¼C®ðÄñÂ¶µÛ"NOR"$N"HIC"¡A¨Ï¥LµLªk¦æ°Ê¦Û¦p¡I"NOR, ch, NULL, victim, TO_CHAR);
		act("$n"HIC"ªºªi¯¾¼C®ðÄñÂ¶µÛ§A¡A¨Ï§AµLªk¦æ°Ê¦Û¦p¡I"NOR, ch, NULL, victim, TO_VICT);
		act("$n"HIC"ªºªi¯¾¼C®ðÄñÂ¶µÛ"NOR"$N"HIC"¡A¨Ï¥LµLªk¦æ°Ê¦Û¦p¡I"NOR, ch, NULL, victim, TO_NOTVICT);
		af.type      = gsn_sprinkle_slash;
		af.duration  = 0 + sklv / 5 + sklv / 6;
		if (IS_NPC(victim)) {
			af.duration *= 2;
		}
		af.location  = APPLY_ACTION_ABILITY;
		af.modifier  = nSlowAct;
		af.bitvector = 0;
		if(number_percent() < (sklv + 1) * 10){
			af.bitvector = AFF_HOLD;
			act(HIB"±j¯Pªºªi¯¾¼C®ð¨Ï±o"NOR"$N"HIB"§¹¥þµLªk°k¶]¡I"NOR, ch, NULL, victim, TO_CHAR);
			act(HIB"±j¯Pªºªi¯¾¼C®ð¨Ï±o§A§¹¥þµLªk°k¶]¡I"NOR, ch, NULL, victim, TO_VICT);
			act(HIB"±j¯Pªºªi¯¾¼C®ð¨Ï±o"NOR"$N"HIB"§¹¥þµLªk°k¶]¡I"NOR, ch, NULL, victim, TO_NOTVICT);
			affect_strip(victim, gsn_sprinkle_slash);
			affect_join( victim, &af );
		}else if(is_affected(victim, gsn_sprinkle_slash)){
			if(affected_value(victim, gsn_sprinkle_slash, APPLY_ACTION_ABILITY) > nSlowAct){
				affect_strip(victim, gsn_sprinkle_slash);
				affect_join( victim, &af );
			}else{
				return;
			}
		}else{
			affect_join( victim, &af );
		}
	}

/* Previous Sprinkle Slash *
    if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR ){
        WAIT_STATE( ch, 85 - ( get_curr_dex( ch ) * 2 ) - sklv * 4);
        if ( is_qmark(victim, "spell_sprinkle_timer") ) return;
        WAIT_STATE( victim, get_curr_int( ch ) + 5 +sklv * 3);
    }else{
        WAIT_STATE( ch, 110 - ( get_curr_dex( ch ) * 2 ) - sklv * 4);
        //  ¦pªG¦³¤Fqmark,´Nreturn,Åýmob·|¦b1 tick¤§¶¡¤£·|¦A«×³Q¼vÅT
        if (  is_qmark(victim, "spell_sprinkle_timer") ) return;
        WAIT_STATE( victim, get_curr_int( ch ) - 10 + sklv);
    }

    sprintf( buf, "self spell_sprinkle_timer 2 0 0 0 0 1" );
    do_add_qmark( victim, buf );
*/
	return;
}

void do_earth( CHAR_DATA *ch, char *argument )/*{{{*/
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj  = NULL;
    int        wp   = WEAR_NONE;

    if (   ( IS_NPC( ch )  && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) ) 
        || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_earth) < number_percent( ) ) ){
        send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
        return;
    }

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
        if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) && !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) ){
            if ( !( obj = get_eq_char( ch, WEAR_WIELD_2 ) ) ){
                send_to_char( "¦n¤ï®³§âªZ¾¹§a¡H\n\r", ch );
                return;
            }
            wp = WEAR_WIELD_2;
        }else
            wp = WEAR_WIELD;
          
        if ( !IS_NPC(ch) && obj->value[3] != 1 && obj->value[3] != 3 ){
            send_to_char( "¼C³N¬O­n¥Î¼C¬I®iªº¡C\n\r", ch );
            return;
        }
    }else if(get_skill_level(ch, gsn_inborn_ability) < 3){
        send_to_char( "§Aªº¤Ñ½áÁÙ¤£¨¬¥H±Nºë¯«¤O¾®»E¦¨Å]ªk¼C¡C\n\r", ch );
        return;
    }

    if( !IS_NPC(ch )){
        if( ch->move < 55 || ch->mana < 55 ){
            send_to_char( "§AÄ±±o¦n§x¥F¡C\n\r", ch );
            return;
        }
	/*
		else{
            ch->move -= 250;
            ch->mana -= 200;
        }  */
    }

    victim = ch->fighting ? ch->fighting : NULL ;
    if ( victim && (victim->deleted || victim->in_room != ch->in_room ))
        return;

    if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER)){
		    waste_dura(obj);
		    if(victim) one_hit( ch, victim, gsn_earth, wp );
    }
    
    if ( ch->fighting != victim &&  ch->in_room->sector_type == SECT_AIR )
        return;

    spell_earth_slash( -1, ch->level , ch, NULL );

	// ª«²z§Þ·|§P©w lizardman ¤Ñ¥Í§Þ 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);
	
    WAIT_STATE( ch, skill_table[gsn_earth].beats );
    return;

}/*}}}*/

