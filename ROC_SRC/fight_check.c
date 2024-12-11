//*******************************************************************
//*  PROGRAM ID  : fight_check.c                                    *
//*  LANGUAGE    : c                                                *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                      *
//*  PURPOSE     :         ·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ          *
//*******************************************************************
#include <stdio.h>
#include "merc.h"
#include "ansi.h"

int check_item_magic_flag(OBJ_DATA * wield, int vrace, int dam);
bool check_evade( CHAR_DATA *ch, CHAR_DATA *victim );
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff );
//bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff );
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn ); // 2022/12/04
bool check_block( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff );
bool check_tactics( CHAR_DATA *ch, CHAR_DATA *victim );
bool frenzy_check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim );
bool is_wielding_poisoned( CHAR_DATA *ch, int wpn );
bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim );
void check_corpse_count( CHAR_DATA * ch, CHAR_DATA *victim);

bool licensed ( CHAR_DATA *ch );
bool registered ( CHAR_DATA *ch, CHAR_DATA *victim );



void check_wimpout(CHAR_DATA *ch, CHAR_DATA *victim);
bool check_reflexion(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam, int dexdiff);
int check_damage_adjust(CHAR_DATA *ch, int dam);
int check_spellfury(CHAR_DATA *ch, CHAR_DATA *victim, int dam);
int check_dirty_raid(CHAR_DATA *ch, CHAR_DATA *victim, int dam);
bool check_can_do_hit(CHAR_DATA *ch, CHAR_DATA *victim);


CHAR_DATA * check_mob_ai( CHAR_DATA *ch);
CHAR_DATA * mob_ai_lv3 (CHAR_DATA *ch);
CHAR_DATA * mob_ai_lv2 (CHAR_DATA *ch);
CHAR_DATA * mob_ai_lv1 (CHAR_DATA *ch);

// Not used -- head --
void pc_breathe( CHAR_DATA *ch );
void pc_screech( CHAR_DATA *ch );
void pc_spit( CHAR_DATA *ch );
bool check_race_special( CHAR_DATA *ch );
// -- tail --

extern int  artifact( CHAR_DATA *ch);
////extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode ); //Modified by Razgriz 20051014
extern int GetBonusedValue_LanceMaster(CHAR_DATA *ch, int nPerc);

int check_item_magic_flag(OBJ_DATA * wield, int vrace, int dam) 
{
	if( wield && IS_SET(wield->extra_flags, ITEM_MAGIC))
		if( number_bits(2) == 1 )
			if( !str_cmp(race_table[vrace].name, "Vampire")
					||  !str_cmp(race_table[vrace].name, "Undead")
					||  !str_cmp(race_table[vrace].name, "Mindflayer")
					||  !str_cmp(race_table[vrace].name, "Githyanki")
					||  !str_cmp(race_table[vrace].name, "Phantom")
					||  !str_cmp(race_table[vrace].name, "Ghost")
					||  !str_cmp(race_table[vrace].name, "Mist")
			  )
				return (dam / 5);
	return 0;
}

bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff )
{
	int chance;
	int ppl_dex;
	int mod = 100;
	int reposte;  // 20191213 dodge reposte
	int dual_reposte; // 2021/11/16 dual_reposte
	bool fReposte = TRUE;  // 20191213 dodge reposte

	if ( !IS_AWAKE( victim ) )
		return FALSE;

	if ( IS_NPC( victim ) ){
		fReposte = FALSE; //191213
		/* Tuan was here.  :) */
		if( IS_SET(victim->special, EXPERT))
			chance  = UMIN( 40, victim->level / 3 
						+ UMAX( 0, victim->level 
								- ( ( victim->class == 1 ) ? 30 : 60 ) ) );
		else
			chance  = UMIN( 30, victim->level / 4 
						+ UMAX( 0, victim->level 
								- ( ( victim->class == 1 ) ? 20 : 40 ) ) );
		//special bonus for war and thi class
		if ( !IS_NPC( ch ) )
		{
			if ( ch->pcdata->prime_attr == APPLY_STR || ch->pcdata->snd_attr == APPLY_STR )
				chance -= 10;
			if ( ch->pcdata->prime_attr == APPLY_DEX || ch->pcdata->snd_attr == APPLY_DEX )
				chance -= 20;
		}
	} else if ( victim->pcdata->learned[gsn_fool] + victim->pcdata->learned[gsn_dodge] == 0 )
		return FALSE;
	else
		chance  = get_skill_percent(victim, gsn_fool)/4;

	if( !IS_NPC(victim) ) {
		if (victim->pcdata->autocombat == AUTOC_DODGE ){
			mod += get_skill_percent(victim, gsn_autododge)/2
					+ get_skill_level(victim, gsn_autododge)*3;
			fReposte = FALSE;
		}
		else if (victim->pcdata->autocombat == AUTOC_PARRY) {
			mod += get_skill_percent(victim, gsn_autoparry)/3
					+ get_skill_level(victim, gsn_autoparry);
			fReposte = FALSE;
		}
		else if( victim->pcdata->autocombat == AUTOC_STOP 
			|| victim->pcdata->autocombat == AUTOC_CONCENTRATION) {  //191213 ¥[¤Jautoc cast§P©w
			fReposte = FALSE;
		}
		else if ( victim->pcdata->autocombat == AUTOC_CRITICAL ) {
			mod += get_skill_percent(victim, gsn_deadly_strike)/5 + get_skill_level(victim, gsn_deadly_strike);
		}
	}

	chance += get_skill_level(victim, gsn_fool) + get_skill_level(victim ,gsn_fool)/2;
				
	// TrickyPacing dodge chance + 32%
	chance += get_skill_percent(victim, gsn_tricky_pacing)/30 + get_skill_level(victim, gsn_tricky_pacing)*5;

	// stone form ª¬ªp¤Uchance ´î¥b 2022/12/05
	if( is_affected(victim, gsn_stone_form) ){
		fReposte = FALSE;
		chance /= 2;
	}

	if ( !IS_NPC( victim ) && ( number_percent( ) * 100 / mod <= chance - UMIN( 15, (leveldiff * 2) + (dexdiff * 4))))
	{
		act( (is_chinese(ch, victim)) ?
				"$N ¥Î¨³±¶ªº¨­ªk¦a°{¹L§Aªº§ðÀ»." :
				"$N fools you and dodges your attack.", 
						ch, NULL, victim, TO_CHAR );
		act( (is_chinese(victim, ch)) ?
				"§A¥Î¨³±¶ªº¨­ªk¸ú¹L $n ªº§ðÀ».":
				"You fool $n and dodges $n's attack.", 
					ch, NULL, victim, TO_VICT);
		
		if( get_skill_level(victim, gsn_fool) > 4 ) {
			if( number_percent() < get_skill_level(victim,gsn_fool)*3)
				trip(victim, ch);
		}
		return TRUE;
	}

	if ( !IS_NPC( victim ) )
	{
		ppl_dex = get_curr_dex( victim );
		chance  = get_skill_percent(victim, gsn_dodge) / 11; // 9
		chance += ( get_skill_level(victim, gsn_dodge) * get_skill_level(victim, gsn_dodge) * 2 ) / 3; // 24
		chance += ppl_dex - 15;
		chance  = UMIN( chance - leveldiff / 2, 40 + get_skill_level(victim, gsn_dodge) * 2);
		chance -= UMIN( 0 , ( GET_DODGE( victim ) * get_skill_level(victim, gsn_dodge) * get_skill_level(victim, gsn_dodge) )/6000); // lv 6 dc -1000 6
	}

	if ( number_percent( ) * 100 /mod  >= chance )
		return FALSE;

	//191213  dodge reposte §P©w
	if( fReposte )
	{
		reposte  = get_skill_percent(victim, gsn_reposte) / 5;
		reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4);
		dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
		dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 2;

		if ( reposte > number_percent( ))
		{
			act( (!is_chinese(ch, victim)) ?
					HIY"$N repostes your attack."NOR :
					HIY"$N ¥©§®§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
			act( (!is_chinese(victim, ch)) ?
				HIY"You repostes $n's attack." NOR:
				HIY"§A¥©§®§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
			mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			/* 
				­×§ï¬° dual ªº±¡ªp¤U§@¥X°Æ¤âªZ¾¹¤ÏÀ» 2021/11/16

				if( ch->class != NINJA && (victim->class == NINJA && !IS_NPC(victim))
					&& number_percent() < get_curr_dex(victim) / 2 + reposte / 2 )
				{
					act( (!is_chinese(ch, victim)) ? 
						HIY"$N repostes your attack again."NOR :
						HIY"$N ¦A¦¸§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
					act( (!is_chinese(victim, ch)) ?
						HIY"You repostes $n's attack again." NOR:
						HIY"§A¦A¦¸§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
					mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
				}
			*/
			if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
			{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack with secondary weapon."NOR :
					HIY"$N ¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï§A¶i¦æ°lÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack with secondary weapon." NOR:
					HIY"§A¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï $n ¶i¦æ°lÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			}
			return TRUE;
		}
	}

	act( (!is_chinese(ch, victim)) ?
			"$N dodges your attack." :
			"$N ÆF¥©¦a°{Á×§Aªº§ðÀ».", ch, NULL, victim, TO_CHAR );
	act( (!is_chinese(victim, ch)) ?
			"You dodges $n's attack.":
			"§AÆF¥©¦a°{Á×$nªº§ðÀ».", ch, NULL, victim, TO_VICT  );
	return TRUE;
}

bool check_evade( CHAR_DATA *ch, CHAR_DATA *victim )
{
	int chance;
	int mod = 100;

	if ( !IS_AWAKE( victim ) )
		return FALSE;

	if ( IS_NPC( victim ) ){
		/* Tuan was here.  :) */
		//if( IS_SET(victim->special, EXPERT))
		  //  chance        = victim->level / 8 ;
		//else chance = 0;
		chance = 0;
		return FALSE;
	} else {
		if( number_percent() > get_skill_percent(victim, gsn_evade) ) 
			return FALSE;
		if( victim->move < 70 )
			return FALSE;

		chance = -1 * (GET_DODGE( victim ) * (10 + get_skill_level(victim,gsn_evade) * get_skill_level(victim,gsn_evade)))/1000 ;

		chance += get_skill_level(victim,gsn_evade);

		if (victim->pcdata->autocombat == AUTOC_DODGE ){
			chance += get_skill_percent(victim, gsn_autododge)/33 + get_skill_level(victim, gsn_autododge) * 3;
		} else if ( victim->pcdata->autocombat == AUTOC_CRITICAL ) {
			chance += get_skill_percent(victim, gsn_deadly_strike)/33 + get_skill_level(victim, gsn_deadly_strike);
		}
	}

	if ( number_percent( ) * 100 / mod  >= chance )
		return FALSE;

	act( (!is_chinese(ch, victim)) ? 
			"$N evades your spell." :
			"$N ÆF¥©¦a°jÁ×§Aªº©G¤å.", ch, NULL, victim, TO_CHAR);
	act( (!is_chinese(victim, ch)) ?
			"You evades $n's spell.":
			"§AÆF¥©¦a°jÁ×$nªº©G¤å.", ch, NULL, victim, TO_VICT );
	victim->move -= 70;
	return TRUE;
}
/*
 * Check for parry.
 */
// check_parry ¥[¤J dt ­È¤Î wpn , ch §ðÀ»®É¨Ï¥ÎªºªZ¾¹ 2022/12/04
//bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff )
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn )
{
	//int chance;
	int mob_chance = 0;
	int chance = 0;
	int chance2 = 0; //2022/12/03
	int reposte;
	int dual_reposte; // 2021/11/16 dual_reposte
	bool fReposte = TRUE;
	bool isTwohand = FALSE; // §P©w wp ¬O§_¬° twohand 2022/12/06
	//int mod = 100;
	int mod = 0;
	// add at 2022/12/03
	bool isWp2Parry = FALSE;
	bool isFistParry = FALSE;
	OBJ_DATA *wp = NULL;
	OBJ_DATA *wp2 = NULL;
	OBJ_DATA *ch_wp = NULL; // 2022/12/28
	OBJ_DATA *ch_wp2 = NULL; // 2022/12/28
	int weight_diff = 0;
	int min_str;
	int str_diff;

	if ( !IS_AWAKE( victim ) )
		return FALSE;

	// ªýÀÉ ®g½b ¤Î ºj±ñ §P©w parry 2022/12/28
	if( dt == 1014 || dt == 1016 )
		return FALSE;

	if( !IS_NPC(victim))
	{
		if(    victim->pcdata->autocombat == AUTOC_STOP 
			|| victim->pcdata->autocombat == AUTOC_CONCENTRATION
			|| victim->pcdata->autocombat == AUTOC_INSTANT
			|| victim->pcdata->autocombat == AUTOC_DODGE)
		{
			fReposte = FALSE;
		}
		else if( victim->pcdata->autocombat == AUTOC_PARRY )
		{
			int nAutoParryPerc = get_skill_percent(victim, gsn_autoparry);
			int nAutoParrySklv = get_skill_level(victim, gsn_autoparry); 
			if( victim->class == KNIGHT ){
				mod += 3 + (nAutoParrySklv * nAutoParrySklv ) / 3; // lv6 15 , max 20
				weight_diff -= 5;
			}else{
				mod += 1 + (nAutoParrySklv * nAutoParrySklv) / 4; // lv6 9 , max 15
			}
			weight_diff -= nAutoParrySklv * 5;
			fReposte = FALSE;
		}
		else if (victim->pcdata->autocombat == AUTOC_CRITICAL)
		{
			mod += get_skill_percent(victim, gsn_deadly_strike)/20; // base 5
			mod += ( get_skill_level(victim, gsn_deadly_strike) * get_skill_level(victim, gsn_deadly_strike) ) / 4; // lv 6 9 , max 14
		}
	}

	// ±N victim get_eq_char ´«¦¨ wp  ¤Î wp2  ¨Ï parry ¯à¦U§O§P©w 2022/12/03
	if( get_eq_char( victim, WEAR_TWO_HAND ) ){
		isTwohand = TRUE;
		wp = get_eq_char( victim, WEAR_TWO_HAND );
	}
	else if( get_eq_char( victim, WEAR_WIELD ) )
		wp = get_eq_char( victim, WEAR_WIELD );

	if( get_eq_char( victim, WEAR_WIELD_2 ) )
		wp2 = get_eq_char( victim, WEAR_WIELD_2 );

	if ( IS_NPC( victim ) )
	{
		/*  wp ¤Î wp2 ¤À¶}§P©w, ¨Ã¹ï mob ªÅ¤âªº±¡ªp¤U§@ class §P©w 2022/12/03
		if ( !get_eq_char( victim, WEAR_WIELD ) && !get_eq_char( victim, WEAR_TWO_HAND ) )
		{
			if ( !get_eq_char( victim, WEAR_WIELD_2 ) )
				chance /= 3;
			else
				chance = 3 * chance / 4;
		} */

		fReposte = FALSE;
		/* Tuan was here.  :) */
		if( IS_SET(victim->special, EXPERT) ){ // npc °ò¥»ªº chance
			mob_chance = UMIN( 20, ( victim->class == 1 ) ? victim->level - 20 : victim->level - 35 );
		}else{
			mob_chance = UMIN( 10, ( victim->class == 1 ) ? victim->level - 35 : victim->level - 40 );
		}

		if( !wp && !wp2 ){
			if( IS_SET( victim->class, 32 ) ){ // ¦pªG npc ¨S¦³ class bloody «h¤£·|ªÅ¤â®æ¾×
				isFistParry = TRUE;
				chance = ( mob_chance * 3 )/ 4;
			}
			else
				return FALSE;
		}else{
			if( wp )
				chance = mob_chance;

			if( wp2 )
				chance2 = ( mob_chance * 4 )/ 5;
		}

		//special bonus for war and thi class
		if ( !IS_NPC( ch ) )
		{
			if ( ch->pcdata->prime_attr == APPLY_STR || ch->pcdata->snd_attr == APPLY_STR )
				leveldiff += 7;
			if ( ch->pcdata->prime_attr == APPLY_DEX || ch->pcdata->snd_attr == APPLY_DEX )
				leveldiff += 12;
		}
		if( IS_SET( ch->special, EXPERT ) ) // ¶i¶¥ npc
			leveldiff += 10;
	}
	else // pc ªº parry chance ­pºâ
	{
		if ( !wp && !wp2 ) // µL wp ªº±¡ªp¤U
		{
			// ªÅ¤â¶] chance µø¦P¥H¥D­n wp parry
			if( get_skill_percent(victim, gsn_barehand) > 0 ) // max 49
			{
				isFistParry = TRUE;
				// parry °ò¥»­È
				chance = ( get_skill_percent(victim, gsn_barehand) + get_skill_percent(victim, gsn_parry) ) / 15; // 13
				chance += get_skill_level(victim,gsn_barehand)*3; // lv 6 18
				chance += (get_skill_level(victim, gsn_parry) * get_skill_level(victim, gsn_parry) ) / 2; // lv 6 18
			}else{
				return FALSE;
			}
		}
		else // ¦³ wp ªº±¡ªp¤U
		{

			/*  ±N parry ¤À¬° wp ¤Î wp2 2022/12/03
			if ( !get_eq_char( victim, WEAR_WIELD ) && !get_eq_char( victim, WEAR_TWO_HAND ) )
			{
				if ( !get_eq_char( victim, WEAR_WIELD_2 ) )
				if( !wp2 )
					return FALSE;
				//chance = get_skill_percent(victim, gsn_parry)/8;
				chance = get_skill_percent(victim, gsn_dual) / 6;
			}
			else
				chance = get_skill_percent(victim, gsn_parry) / 5; */

			if ( wp ) // max 37
			{
				// parry °ò¥»­È , max 33
				chance = get_skill_percent(victim, gsn_parry) / 11; // learn 99% = 9
				chance += ( get_skill_level(victim, gsn_parry) * get_skill_level(victim, gsn_parry ) * 2 ) / 3; // lv 6 24

				// lance mastery ·|½T¹ê¨Ì wp type §@¥X¤ÏÀ³ 2022/12/03
				// chance  = chance + GetBonusedValue_LanceMaster(ch, chance)/ 8;
				if( wp->value[3] == 2 )
				{
					chance = chance + GetBonusedValue_LanceMaster( victim, chance )/15; // lv 6  max 4
				}
				else if( get_skill_level(victim, gsn_lance_mastery) > 5 )
				{
					if( wp->value[3] == 1 || wp->value[3] == 3 || wp->value[3] == 7 || wp->value[3] == 9 )
					{
						chance = chance + GetBonusedValue_LanceMaster( victim, chance )/15;
					}
				}

				// §P©w¬O§_®³±o°Ê wp, ¹L­«·|­°§C parry ®Ä¯q 2022/12/03
				if( isTwohand ){ // ­«¶q§P©wÂù¤âªZ¾¹»P³æ¤â¤£¦P
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
						if( str_app[min_str].twohand >= get_obj_weight( wp ) )
							break;
					}
				}else{
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
						if( str_app[min_str].wield >= get_obj_weight( wp ) )
							break;
					}
				}
				str_diff = get_curr_str ( victim ) - min_str;
				if( get_skill_percent(victim, gsn_powerblow) > 0)
					str_diff += get_skill_level(victim, gsn_powerblow) * get_skill_level(victim, gsn_powerblow) / 6;
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						chance /= 10;
					else
						chance /= 2;
				}
			}

			if ( wp2 ) // max 27
			{
				// ¦³ dual ªº±¡ªp¬O¤£¥i¯à¥X²{Âù¤âªZ¾¹(twohand)ªºª¬ºA, ¦pªG¦³´N¬O bug
				if( isTwohand ){
					bug( "Parry with wp2 but twohand player name :" , 0 );
					bug( victim->name , 0 );
					return FALSE;
				}
				// wp2 parry °ò¥»­È ,max 24
				chance2 = (get_skill_percent(victim, gsn_parry) + get_skill_percent(victim, gsn_dual) ) / 16; // dual , parry 99% = 12
				chance2 += ( get_skill_level(victim, gsn_parry) * get_skill_level(victim, gsn_parry) ) / 6; // lv 6 6
				chance2 += ( get_skill_level(victim, gsn_dual) * get_skill_level(victim, gsn_dual) )/ 6; // lv 6 6

				// ¥[¤J°Æ¤â lance mastery §P©w 2022/12/03
				if( wp2->value[3] == 2 )
				{
					chance2 = chance2 + GetBonusedValue_LanceMaster( victim, chance2 )/15;  // lv 6 max 3
				}
				else if( get_skill_level(victim, gsn_lance_mastery) > 5 )
				{
					if( wp2->value[3] == 1 || wp2->value[3] == 3 || wp2->value[3] == 7 || wp2->value[3] == 9 )
					{
						chance2 = chance2 + GetBonusedValue_LanceMaster( victim, chance2 )/15;
					}
				}

				// §P©w¬O§_®³±o°Ê wp2 , ¹L­«·|­°§C parry ®Ä¯q 2022/12/03
				for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
					if( str_app[min_str].wield2 >= get_obj_weight( wp2 ) )
						break;
				}
				str_diff = get_curr_str ( victim ) - min_str;
				if( get_skill_percent(victim, gsn_powerblow) > 0)
					str_diff += get_skill_level(victim, gsn_powerblow) * get_skill_level(victim, gsn_powerblow) / 6;
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						chance2 /= 10;
					else
						chance2 /= 2;
				}
			}
		}
	}

	if( !isFistParry )
		if ( !wp && !wp2 )
			return FALSE;

	// stone form ª¬ªp¤Uchance ´î¥b 2022/12/05
	if( is_affected(victim, gsn_stone_form) ){
		fReposte = FALSE;
		chance /= 2;
		chance2 /= 2;
	}

    if( number_percent( ) - mod >= chance - URANGE( -10 , leveldiff , 30 ) ) // chance parry failed
	{
		if ( !wp2 || ( number_percent( ) - mod >= chance2 - URANGE( -10 , leveldiff , 30 ) ) ) // chance2 parry failed
		{
			if ( IS_NPC(victim) )
				return FALSE;

			if( fReposte ) // ¥¢±Ñ§P©w reposte
			{
				reposte  = get_skill_percent(victim, gsn_reposte) / 6;
				reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4) / 4;
				dual_reposte = get_skill_percent(victim, gsn_dual) / 12;
				dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 8;
				if ( reposte > number_percent( ))
				{
					act( (!is_chinese(ch, victim)) ? 
							HIY"$N repostes your attack."NOR :
							HIY"$N ¥©§®§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
					act( (!is_chinese(victim, ch)) ?
							HIY"You repostes $n's attack." NOR:
							HIY"§A¥©§®§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
					mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );

					//if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
					if( wp2 && ( number_percent() < dual_reposte ) )
					{
						act( (!is_chinese(ch, victim)) ? 
							HIY"$N repostes your attack with secondary weapon."NOR :
							HIY"$N ¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï§A¶i¦æ°lÀ»."NOR, ch, NULL, victim, TO_CHAR );
						act( (!is_chinese(victim, ch)) ?
							HIY"You repostes $n's attack with secondary weapon." NOR:
							HIY"§A¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï $n ¶i¦æ°lÀ»."NOR,  ch, NULL, victim, TO_VICT );
						mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
					}
					return TRUE;
				}
			}
			return FALSE;
		}
		isWp2Parry = TRUE; //  wp2 ªº chance2 parry ¦¨¥\ 2022/12/03
    }

	if( fReposte ) // §P©w reposte
    {
        reposte  = get_skill_percent(victim, gsn_reposte) / 5;
        reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4);
		dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
		dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 2;

        if ( reposte > number_percent( ) )
        {
            act( (!is_chinese(ch, victim)) ? 
                    HIY"$N repostes your attack."NOR :
                    HIY"$N ¥©§®§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
            act( (!is_chinese(victim, ch)) ?
                    HIY"You repostes $n's attack." NOR:
                    HIY"§A¥©§®§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
            mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );

			//if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
			if( wp2 && ( number_percent() < dual_reposte ) )
			{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack with secondary weapon."NOR :
					HIY"$N ¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï§A¶i¦æ°lÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack with secondary weapon." NOR:
					HIY"§A¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï $n ¶i¦æ°lÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			}
            return TRUE;
        }
    }

	/* ¥[¤J§ðÀ»­«¶q¹ï parry ¬ð¯} 2022/12/03 */

	// ch §ðÀ»­«¶q­pºâ

	if( !IS_NPC(ch) ){ // Â¾·~¥[¦¨
		if( ch->pcdata->prime_attr == APPLY_STR ) // ch base war
			weight_diff += 5;

		if( ch->pcdata->snd_attr == APPLY_STR ) // ch snd war
			weight_diff += 5;
	}else{
		if( IS_SET( ch->class, 1 ) ) // npc ch war
			weight_diff += ch->level/5;

		if( IS_SET( ch->special, EXPERT ) ) // ¶i¶¥ npc
			weight_diff += ch->level/4;
	}

	if( is_affected(ch, gsn_powerup) )  // powerup ¥[¦¨
		weight_diff += 5 + get_skill_level(ch, gsn_powerup) * 10;

	// ¥N¤J©R¤¤ªZ¾¹ª½±µ²Î¦X ch ¨Ï¥Îªº¸Ë³Æ 2022/12/28
	ch_wp = check_using_wp( ch, dt , wpn );
	ch_wp2 = check_using_wp2( ch, dt );

	if ( dt == gsn_kick ) // kick
	{
		weight_diff += 10 + get_skill_level(ch, gsn_kick) * 10;
		/*if( get_eq_char( ch, WEAR_FEET ) )  // ¥[¤J ch feet ¸Ë³Æ­«¶q
			weight_diff += get_eq_char( ch, WEAR_FEET )->weight;
		if( get_eq_char( ch, WEAR_FEET_2 ) )
			weight_diff += get_eq_char(ch, WEAR_FEET_2 )->weight; */
		if( ch_wp )
			weight_diff += ch_wp->weight;
		if( ch_wp2 )
			weight_diff += ch_wp2->weight;
	}
	else if( dt == TYPE_HIT ) // ªÅ¤â§ðÀ»
	{
		if( !IS_NPC(ch) ){
			if( IS_SET( ch->act, PLR_FIST_FIGHT ) ) // pc fist fight
				weight_diff += 10 + get_skill_level(ch, gsn_fist_fight) * 5;
		}else{
			weight_diff += ch->level;
		}
		/*if( get_eq_char( ch, WEAR_HANDS ) ) // ¥[¤J ch hands ¸Ë³Æ­«¶q
			weight_diff += get_eq_char( ch, WEAR_HANDS )->weight;
		if( get_eq_char( ch, WEAR_HANDS_2 ) )
			weight_diff += get_eq_char(ch, WEAR_HANDS_2 )->weight; */
		if( ch_wp )
			weight_diff += ch_wp->weight;
		if( ch_wp2 )
			weight_diff += ch_wp2->weight;
	}
	else
	{
		if( !IS_NPC(ch) ){
			if( get_skill_percent(ch, gsn_powerblow) > 0 ) // powerblow
				weight_diff += 10 + get_skill_level(ch, gsn_powerblow) * 5;
		}
		/*if( get_eq_char( ch, wpn ) )  // ¥[¤J ch §ðÀ»ªZ¾¹ªº­«¶q
			weight_diff += get_eq_char( ch, wpn )->weight; */
		// °£ kick ¤Î type_hit ¥~, ¦³¤â«ùªZ¾¹¥B¨Ï¥Î°ÆªZ¾¹ªº¥u·|¬O ®g½b ¤Î ºj±ñ ©Ò¥H¤£§P©w ch_wp2 2022/12/28
		if( ch_wp )
			weight_diff += ch_wp->weight;
	}

	// victim ®æ¾×­«¶q­pºâ

	if( isFistParry ){ // ªÅ¤â®æ¾×
		if( !IS_NPC(victim) ){
			weight_diff = weight_diff - (victim->level * 3)/4 - get_skill_level(victim,gsn_barehand) * 3; // pc ªÅ¤â parry °ò¥»­È
		}else{
			weight_diff = weight_diff - victim->level; // npc ªÅ¤â parry °ò¥»­È¬° lv
		}
		if( get_eq_char( victim, WEAR_HANDS ) ) // ¦©°£ hands ¸Ë³Æ­«¶q
			weight_diff = weight_diff - get_eq_char( victim, WEAR_HANDS )->weight;
		if( get_eq_char( victim, WEAR_HANDS_2 ) )
			weight_diff = weight_diff - get_eq_char(victim, WEAR_HANDS_2 )->weight;
	}

	// Âù¤âªZ¾¹¯à­°§C 20% ­«¶q®t 2022/12/30
	if( isTwohand && weight_diff > 0 )
		weight_diff = ( weight_diff * 8 )/10;
		

	else if( isWp2Parry && wp2 ) // ¦©°£ wp2 ­«¶q
		weight_diff = weight_diff - wp2->weight;
	else if( wp ) // ¦©°£ wp ­«¶q
		weight_diff = weight_diff - wp->weight - 10;

	// ºâ¦¡¦¨¥ß«h parry ³Q¬ð¯} , parry sklv*2 ¬°«OÃÒ¤£³Q¬ð¯}ªº°ò¥»­È
	if( ( get_skill_level(victim, gsn_parry)*2 < number_percent() ) && weight_diff > number_percent() )
	{
		act( (!is_chinese(ch, victim)) ? "You BREAK $N's parry!" : "§Aªº§ðÀ»¬ð¯} $N ªºªZ¾¹®æ¾×¡I", ch, NULL, victim, TO_CHAR );
		act( (!is_chinese(victim, ch)) ? "$n's attack BREAK your parry!" : "§AªºªZ¾¹®æ¾×³Q $n ¬ð¯}¤F¡I",  ch, NULL, victim, TO_VICT );
		return FALSE;
	}

    act( (!is_chinese(ch, victim)) ? "$N parries your attack." : "$N ¬[¶}¤F§Aªº§ðÀ».", ch, NULL, victim, TO_CHAR );
    act( (!is_chinese(victim, ch)) ? "You parry $n's attack." : "§A¬[¶}¤F $n ªº§ðÀ».",  ch, NULL, victim, TO_VICT );


	// ¥[¤J parry ¹ïªZ¾¹¿i·l, obj_block prog, obj_hp §P©w 2022/12/03
	if( isWp2Parry && wp2 ) // ¥H wp2 parry
	{
		if( !IS_NPC(victim) ) // ªZ¾¹¿i·l
		{
			if( wp2->value[3] != 14 && number_bits(5) < 8 ){
				if( get_skill_level( victim, gsn_weapon_master) * 6 < number_percent() )
					waste_dura( wp2 );
			}
		}
		if ( wp2 && wp2->pIndexData->progtypes & OBJ_BLOCK ) // obj_block prog §P©w
			oprog_block_trigger(wp2);
		if ( wp2 && (wp2->hp || wp2->material->hp) ) // hp_block §P©w
		{
			//Weapon Master µ¥¯Å¹ïªZ¾¹¾Ô°«Ä²µoªº hp µ¥¯Å¥[¦¨	
			int WmArtLv = 0;
			if( !IS_NPC(victim) && wp2->wear_loc == WEAR_WIELD_2 ){
				WmArtLv += get_skill_level(victim,gsn_weapon_master);
				if (get_skill_level(victim,gsn_weapon_master) > 3) WmArtLv += 2;
				if (get_skill_level(victim,gsn_weapon_master) > 4) WmArtLv += 3;
				if (get_skill_level(victim,gsn_weapon_master) > 5) WmArtLv += 4;
			}

			OBJ_HP * ohp = wp2->hp;
			for( ; ohp; ohp = ohp->next ){
				if( IS_SET( ohp->event, HP_BLOCK) ){
					if ( ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_ARTIFACT ) && 
							( artifact(victim) * ohp->percent * get_curr_wis( victim ) / 5000 + 
							( get_skill_level(victim,gsn_artifact) == 6 ? 20 : 10) ) > number_percent( ) )
						|| ( IS_NPC( victim ) && ohp->percent * get_curr_wis( victim ) / 50 > number_percent( ) ) )
					{
						trigger_obj_hp(wp2, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
					}
				}
			}
			if( wp2->material->hp ) {
				for( ohp=wp2->material->hp ; ohp; ohp = ohp->next ){
					if( IS_SET( ohp->event, HP_BLOCK) ){
						if( ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_ARTIFACT ) && 
								( artifact(victim) * ohp->percent * get_curr_wis( victim ) / 5000 +
								(get_skill_level(victim,gsn_artifact) == 6 ? 20 : 10 )) > number_percent( ) )
							||( IS_NPC( victim ) && ohp->percent * get_curr_wis( victim ) / 50 > number_percent( ) ) )
						{
							trigger_obj_hp(wp2, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
						}
					}
				}
			}
		}
	}
	else if( wp ) // ¥H wp parry
	{
		if( !IS_NPC(victim) ) // ªZ¾¹¿i·l
		{
			if( wp->value[3] != 14 && number_bits(5) < 8 ){
				if( get_skill_level( victim, gsn_weapon_master) * 6 < number_percent() )
					waste_dura( wp );
			}
		}
		if ( wp && wp->pIndexData->progtypes & OBJ_BLOCK ) // obj_block prog §P©w
			oprog_block_trigger(wp);
		if ( wp && (wp->hp || wp->material->hp) ) // hp_block §P©w
		{
			//Weapon Master µ¥¯Å¹ïªZ¾¹¾Ô°«Ä²µoªº hp µ¥¯Å¥[¦¨	
			int WmArtLv = 0;
			if( !IS_NPC(victim) && ( wp->wear_loc == WEAR_TWO_HAND || wp->wear_loc == WEAR_WIELD ) ){
				WmArtLv += get_skill_level(victim,gsn_weapon_master);
				if (get_skill_level(victim,gsn_weapon_master) > 3) WmArtLv += 2;
				if (get_skill_level(victim,gsn_weapon_master) > 4) WmArtLv += 3;
				if (get_skill_level(victim,gsn_weapon_master) > 5) WmArtLv += 4;
			}

			OBJ_HP * ohp = wp->hp;
			for( ; ohp; ohp = ohp->next ){
				if( IS_SET( ohp->event, HP_BLOCK) ){
					if ( ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_ARTIFACT ) && 
							( artifact(victim) * ohp->percent * get_curr_wis( victim ) / 3000 + 
							( get_skill_level(victim,gsn_artifact) == 6 ? 35 : 20)) > number_percent( ) )
						|| ( IS_NPC( victim ) && ohp->percent * get_curr_wis( victim ) / 25
							> number_percent( ) ) )
					{
						trigger_obj_hp(wp, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
					}
				}
			}
			if( wp->material->hp ) {
				for( ohp=wp->material->hp ; ohp; ohp = ohp->next ){
					if( IS_SET( ohp->event, HP_BLOCK) ){
						if( ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_ARTIFACT ) && 
								( artifact(victim) * ohp->percent * get_curr_wis( victim ) / 3000 +
								( get_skill_level(victim,gsn_artifact) == 6 ? 35 : 20 )) > number_percent( ) )
							||( IS_NPC( victim ) && ohp->percent * get_curr_wis( victim ) / 25 > number_percent( ) ) )
						{
							trigger_obj_hp(wp, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, victim, ch);
						}
					}
				}
			}
		}
	}

    if( !IS_NPC(ch) && is_affected(ch, gsn_powerup) && get_skill_level(ch, gsn_powerup) > 5) {
        if( number_percent() < get_skill_level(ch, gsn_powerup) * 4 ) {
            disarm(ch,victim);
        }
    }

    return TRUE;
}

/*
 * Check for block.
 */
bool check_block( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff )
{
	OBJ_DATA * obj;
	int weight;
	int chance;
	int break_shield;
	int mod = 100;
	int bsmod = 100;
	int class_bonus;
	int nBlockSklv = get_skill_level(victim, gsn_block);
	int nBlockPerc = get_skill_percent(victim, gsn_block);
	int reposte;         // 191213 ¥[¤J block reposte§P©w
	int dual_reposte; // 2021/11/16 dual_reposte
	bool fReposte = TRUE; // 191213 ¥[¤J block reposte§P©w
	
	if ( !( obj = get_eq_char( victim, WEAR_SHIELD ) ) )
		return FALSE;

	if ( !IS_AWAKE( victim ) )
		return FALSE;

	weight = get_obj_weight( obj );

	if( !IS_NPC(victim) ) {
		if(victim->pcdata->autocombat == AUTOC_PARRY){
			int nAutoParryPerc = get_skill_percent(victim, gsn_autoparry);
			int nAutoParrySklv = get_skill_level(victim, gsn_autoparry);
			// MAX_ORG += 50 + 12  
			mod   += nAutoParryPerc / 4 + nAutoParrySklv * (nAutoParrySklv + 4) / 2;
			bsmod += nAutoParryPerc / 2 + nAutoParrySklv * (nAutoParrySklv + 4);
			fReposte = FALSE;
		}else if(victim->pcdata->autocombat == AUTOC_DODGE){
			int nAutoDodgePerc = get_skill_percent(victim, gsn_autododge);
			int nAutoDodgeSklv = get_skill_level(victim, gsn_autododge);
			bsmod += nAutoDodgePerc / 4 + nAutoDodgeSklv * (nAutoDodgeSklv + 4) / 2;
			fReposte = FALSE;
		}else if(victim->pcdata->autocombat == AUTOC_STOP  // 191213 ¥[¤J auto stop respote§P©w
				|| victim->pcdata->autocombat == AUTOC_CONCENTRATION){
			fReposte = FALSE;
		}else if(victim->pcdata->autocombat == AUTOC_CRITICAL){
			mod   += get_skill_percent(victim, gsn_deadly_strike)/3+get_skill_level(victim, gsn_deadly_strike);
			bsmod += get_skill_percent(victim, gsn_autododge)/5+get_skill_level(victim, gsn_autododge);
		}
	}

	class_bonus = IS_NPC( ch ) ? 0 :
					( ch->pcdata->prime_attr == APPLY_STR || ch->pcdata->snd_attr == APPLY_STR ) ? 1 :
					( ch->pcdata->prime_attr == APPLY_DEX || ch->pcdata->snd_attr == APPLY_DEX ) ? 2 : 0;
	if(IS_NPC(victim)){
		fReposte = FALSE; //191213
		/* Tuan was here.  :) */
		if( IS_SET(victim->special, EXPERT))
			chance	= UMIN( 60, ( weight +  victim->level ) / 2 - class_bonus * 15 );
		else
			chance	= UMIN( 30, ( weight +  victim->level ) / 3 - class_bonus * 8 );
	}else{
		//int nBlockSklv = get_skill_level(victim, gsn_block);
		//int nBlockPerc = get_skill_percent(victim, gsn_block);
				// 13 15 19 23 29 35 43
		chance = nBlockSklv * (nBlockSklv + 4) / 2 + nBlockPerc / 6 + weight / 3;
		bsmod += chance;
		//   * ORG_MAX = 15 + W/2 + Sklv * 1.5 = 15 + 9 + W/2 = 24 + W/2
		//      chance = (get_skill_percent(victim, gsn_block) / 3 + weight) / 2 
		//             +  get_skill_level(victim, gsn_block) + get_skill_level(victim, gsn_block) / 2;
	}

	// stone form ª¬ªp¤Uchance ´î¥b 2022/12/05
	if( is_affected(victim, gsn_stone_form) ){
		fReposte = FALSE;
		chance /= 2;
	}

	if(URANGE(2 + get_curr_str(victim) / 3, chance - UMAX( 0, leveldiff ), 60 ) < number_percent( ) * 100 / mod)
		return FALSE;

	//break_shield = ( IS_NPC( ch ) ) ? IS_SET( ch->class, 8 ) ? ch->level : 0
	//				: get_skill_percent(ch, gsn_break_shield) / 6;
	if( IS_NPC(ch) && IS_SET( ch->class, 8 ) )
		break_shield = UMIN( 25 ,ch->level / 3 ); // lv 50 , 16 ; lv 55 , 18  ; lv 60 , 20
	else if( !IS_NPC(ch) )
		break_shield = get_skill_percent(ch, gsn_break_shield) / 5; // 19.8
	else
		break_shield = 0;
    
	if(!IS_NPC(ch))
		//break_shield += get_skill_level(ch, gsn_break_shield) * (get_skill_level(ch, gsn_break_shield) + 4) * 2 / 3;
		break_shield += get_skill_level(ch, gsn_break_shield) * (get_skill_level(ch, gsn_break_shield) + 4); // 60
	else
		break_shield += get_skill_level(ch, gsn_break_shield) * 3; // 18

	// block sklv have better chance to prevent break shield.
	if ( !IS_NPC( victim) ){
		if( victim->pcdata->prime_attr == APPLY_STR )
			break_shield = break_shield - nBlockSklv * 3;
		else
			break_shield = break_shield - nBlockSklv * 2;
	}
	if ( break_shield > number_percent( ) * bsmod / 100 )
	{
		/*
		act( (is_chinese(ch, victim)) ?
			"§A¬ð¯} $N ªº¬ÞµP¨¾¿m!" :
			"You break through $N's shield blocks!", ch, NULL, victim, TO_CHAR );
		act( (is_chinese(victim, ch)) ?
			"$n ¬ð¯}§Aªº¬ÞµP¨¾¿m!" :
			"$n break through your shield blocks!",  ch, NULL, victim, TO_VICT );
		*/
		act( (is_chinese(ch, victim)) ?
			"§A±Y¸Ñ $N ªº¬ÞµP©è¾×!" :
			"You break $N's shield blocks!", ch, NULL, victim, TO_CHAR );
		act( (is_chinese(victim, ch)) ?
			"$n ±Y¸Ñ§Aªº¬ÞµP©è¾×!" :
			"$n break your shield blocks!",  ch, NULL, victim, TO_VICT );

		if ( number_bits(3) == 1 && !IS_SET(obj->extra_flags,ITEM_NOREMOVE)){
			unequip_char( victim, obj);
			waste_dura(obj);
			obj_from_char( obj );
			if ( IS_NPC( victim ) || nBlockSklv >= 3) {
				obj_to_char( obj, victim );
			} else if ( can_drop_obj(victim, obj)){
				obj_to_room( obj, victim->in_room );
			}else
				obj_to_char( obj, victim );

			// ¦â½X­×¥¿ 2022/11/04
			if( !is_chinese(ch, victim)){
				//act( HIY"You disarm $N's shield!"NOR,  ch, NULL, victim, TO_CHAR	 );
				//act( "[1;31m$n DISARMS $N's shield![m",  ch, NULL, victim, TO_NOTVICT );
				act( ""HIY"You disarm "NOR"$N"HIY"'s shield!"NOR"", ch, NULL, victim, TO_CHAR );
				act( "$n"HIY" DISARM "NOR"$N"HIY"'s shield!"NOR"", ch, NULL, victim, TO_NOTVICT );
			}else{
				//act( HIY"§AÀ»¸¨¤F $N ªº¬ÞµP¡I"NOR,  ch, NULL, victim, TO_CHAR	 );
				//act( "[1;31m$n À»¸¨¤F $N ªº¬ÞµP¡I[m",  ch, NULL, victim, TO_NOTVICT );
				act( ""HIY"§AÀ»¸¨¤F "NOR"$N"HIY" ªº¬ÞµP¡I"NOR"", ch, NULL, victim, TO_CHAR );
				act( "$n"HIY" À»¸¨¤F "NOR"$N"HIY" ªº¬ÞµP¡I"NOR"", ch, NULL, victim, TO_NOTVICT );
			}
			// ³QÀ»¸¨ªÌ·|¬Ý¨ì¦Û¤v³QÀ»¸¨ªº obj ¦W 2022/11/04
			if( !is_chinese(victim, ch)){
				//act( "[1;31m$n DISARMS your shield![m", ch, NULL, victim, TO_VICT	 );
				act( "$n"HIY" DISARMS your shield "NOR"$p"HIY" !"NOR"", ch, obj, victim, TO_VICT );
			}else{
				//act( "[1;31m$n À»¸¨¤F§Aªº¬ÞµP¡I[m", ch, NULL, victim, TO_VICT	 );
				act( "$n"HIY" À»¸¨¤F§Aªº¬ÞµP "NOR"$p"HIY" ¡I"NOR"", ch, obj, victim, TO_VICT );
			}
		}
		return FALSE;
	}

	// 191213  ¥[¤J block reposte §P©w
	if( fReposte )
	{
		reposte  = get_skill_percent(victim, gsn_reposte) / 5;
		reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 4);
		dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
		dual_reposte += get_skill_level(victim, gsn_dual) * (get_skill_level(victim, gsn_dual) + 4) / 2;

		if ( reposte > number_percent( ))
		{
			act( (!is_chinese(ch, victim)) ? 
				HIY"$N repostes your attack."NOR :
				HIY"$N ¥©§®§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
			act( (!is_chinese(victim, ch)) ?
				HIY"You repostes $n's attack." NOR:
				HIY"§A¥©§®§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
			mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		/* 
			­×§ï¬° dual ªº±¡ªp¤U§@¥X°Æ¤âªZ¾¹¤ÏÀ» 2021/11/16

			if( ch->class != NINJA && (victim->class == NINJA && !IS_NPC(victim))
				&& number_percent() < get_curr_dex(victim) / 2 + reposte / 2 )
			{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack again."NOR :
					HIY"$N ¦A¦¸§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack again." NOR:
					HIY"§A¦A¦¸§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			}
		*/
			if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
			{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack with secondary weapon."NOR :
					HIY"$N ¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï§A¶i¦æ°lÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack with secondary weapon." NOR:
					HIY"§A¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï $n ¶i¦æ°lÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			}
			return TRUE;
		}
	}

	act( (is_chinese(ch, victim)) ? "$N ¥Î¬ÞµP¾×¦í§Aªº§ðÀ»." : "$N blocks your attack.", ch, NULL, victim, TO_CHAR );
	act( (is_chinese(victim, ch)) ? "§A¥Î¬ÞµP¾×¦í $n ªº§ðÀ»." : "You block $n's attack.",  ch, NULL, victim, TO_VICT );

	// ¼W¥[ block obj_block ªº§P©w 2022/10/23
	if ( obj && obj->pIndexData->progtypes & OBJ_BLOCK )
		oprog_block_trigger(obj);

	if ( obj && (obj->hp || obj->material->hp) ){
		OBJ_HP * ohp = obj->hp;
		for( ; ohp; ohp = ohp->next ){
			if( IS_SET( ohp->event, HP_BLOCK) ){
				if ( ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_ARTIFACT ) && 
					( artifact(victim) * ohp->percent *  get_curr_wis( victim ) / 2500 + 
					(get_skill_level(victim,gsn_artifact) == 6 ? 40 : 30)) > number_percent( ) )
					|| ( IS_NPC( victim ) && ohp->percent * get_curr_wis( victim ) / 25 
					> number_percent( ) ) ){
					trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, victim, ch);
				}
			}
        }
		if( obj->material->hp ) {
			for( ohp=obj->material->hp ; ohp; ohp = ohp->next ){
				if( IS_SET( ohp->event, HP_BLOCK) ){
					if( ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_ARTIFACT ) && 
						(  artifact(victim) * ohp->percent * get_curr_wis( victim ) / 2500 +
						(get_skill_level(victim,gsn_artifact) == 6 ? 40 : 30 )) 
						> number_percent( ) )
						||( IS_NPC( victim ) && ohp->percent * get_curr_wis( victim ) / 25
						> number_percent( ) ) ){
						trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, victim, ch);
					}
				}
			}
		}
	}

	// mon ¦³ powerupªº±¡ªp¤U³Q block ¤@©w¾÷²v disarm ¹ï¤â shield
	//if( !IS_NPC(ch) && is_affected(ch, gsn_powerup) && get_skill_level(ch, gsn_powerup) > 5)
	if( is_affected(ch, gsn_powerup) )
	{
		//if( number_percent() < 2 * get_skill_level(ch, gsn_powerup) ) {
			/*  remove at 2020/10/30
			unequip_char( victim, obj);
			waste_dura(obj);
			obj_from_char( obj );

			if ( IS_NPC( victim ) )
				obj_to_char( obj, victim );
			else if ( can_drop_obj(victim, obj))
			{
				obj_to_room( obj, victim->in_room );
			}
			else
				obj_to_char( obj, victim );
			waste_dura(obj);
			obj_from_char( obj );
			if ( IS_NPC( victim ) )
			obj_to_char( obj, victim );
			else
			obj_to_room( obj, victim->in_room );
			if( !IS_NPC( ch ) &&!IS_SET( ch->act, PLR_CHINESE ) ) */

		// ¼W¥[ str ®t­È¤Î block sklv §P©w  2020/10/30
		if( 2 * nBlockSklv + number_percent() < 4 * get_skill_level(ch, gsn_powerup) + get_curr_str(ch) - get_curr_str(victim) ) {
			// ¼W¥[§P©w¬O§_¦³ no remove ¤Î§Þ¯àµ¥¯ÅÀ»¸¨¬O§_±¼¸¨¦a­± 2020/10/30
			if ( !IS_SET(obj->extra_flags,ITEM_NOREMOVE)){
				unequip_char( victim, obj);
				waste_dura(obj);
				obj_from_char( obj );
				if ( IS_NPC( victim ) || nBlockSklv >= 3) {
					obj_to_char( obj, victim );
				} else if ( can_drop_obj(victim, obj)){
					obj_to_room( obj, victim->in_room );
				}else
					obj_to_char( obj, victim );

				if( !is_chinese(ch, victim)){
					act( HIY"You disarm $N's shield!"NOR,  ch, NULL, victim, TO_CHAR	 );
					act( "[1;31m$n DISARMS $N's shield![m",  ch, NULL, victim, TO_NOTVICT );
				}else{
					act( HIY"§AÀ»¸¨¤F $N ªº¬ÞµP¡I"NOR,  ch, NULL, victim, TO_CHAR	 );
					act( "[1;31m$n À»¸¨¤F $N ªº¬ÞµP¡I[m",  ch, NULL, victim, TO_NOTVICT );
				}

				if( !is_chinese(victim, ch)){
					act( "[1;31m$n DISARMS your shield![m", ch, NULL, victim, TO_VICT	 );
				}else{
					act( "[1;31m$n À»¸¨¤F§Aªº¬ÞµP¡I[m", ch, NULL, victim, TO_VICT	 );
				}
			}
        }
    }
    return TRUE;
}





/*
 * Check for tactics.
 */
// victim »P ch ¿ù¶Ã, ¬G§ï¬°»P fight_damage ªº¦ì¸m¤@¼Ë 2022/12/04
//bool check_tactics( CHAR_DATA *ch, CHAR_DATA *victim )
bool check_tactics( CHAR_DATA *victim, CHAR_DATA *ch )
{
	int chance = 0;
	int reposte;         // 191213 ¥[¤J block reposte§P©w
	int dual_reposte; // 2021/11/16 dual_reposte
	bool fReposte = TRUE; // 191213 ¥[¤J block reposte§P©w
	int nTacSklv = get_skill_level(victim, gsn_tactics);

	if( !IS_NPC(victim)
		&& (   victim->pcdata->autocombat == AUTOC_PARRY
			|| victim->pcdata->autocombat == AUTOC_DODGE
			|| victim->pcdata->autocombat == AUTOC_CONCENTRATION
			|| victim->pcdata->autocombat == AUTOC_STOP))
	{
		fReposte = FALSE;
	}

	if( IS_NPC(victim) ) fReposte = FALSE;


	if ( !IS_AWAKE( victim ) )
		return FALSE;

	if ( !IS_NPC( victim ) )
	{
		// ªý¾×¤£·| tactic ªº±¡ªp 2022/12/04
		if( victim->pcdata->learned[gsn_tactics] ){
			if( victim->class == WARLORD )
				chance += get_curr_int(victim);
			else
				chance += get_curr_int(victim)/3;
			chance += ( ( nTacSklv * nTacSklv * 2 ) / 3 ) * get_skill_percent(victim, gsn_tactics) / 100;
			if ( number_percent() > chance )
				return FALSE;
		}else{
			return FALSE;
		}
	}
	else if ( !IS_SET( victim->act, ACT_TACTICS ) )
	{
		return FALSE;
	}
	else
	{
		chance += get_curr_int(victim)/2;
		chance += ( nTacSklv * nTacSklv * 2 ) / 5;
		chance = UMIN( 30, victim->level / 2 );
		if ( number_percent() > chance )
			return FALSE;
	}
	// 191213  ¥[¤J tactics reposte §P©w
	if( fReposte )
	{
		reposte  = get_skill_percent(victim, gsn_reposte) / 8;
		reposte += get_skill_level(victim, gsn_reposte) * (get_skill_level(victim, gsn_reposte) + 2);
		dual_reposte = get_skill_percent(victim, gsn_dual) / 10;
		dual_reposte += get_skill_level(victim, gsn_dual) * get_skill_level(victim, gsn_dual) / 2;

		if ( reposte > number_percent( ))
		{
			act( (!is_chinese(ch, victim)) ? 
				HIY"$N repostes your attack."NOR :
				HIY"$N ¥©§®§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
			act( (!is_chinese(victim, ch)) ?
				HIY"You repostes $n's attack." NOR:
				HIY"§A¥©§®§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
			mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			/* 
				­×§ï¬° dual ªº±¡ªp¤U§@¥X°Æ¤âªZ¾¹¤ÏÀ» 2021/11/16

				if( ch->class != NINJA && (victim->class == NINJA && !IS_NPC(victim))
					&& number_percent() < get_curr_dex(victim) / 2 + reposte / 2 )
				{
					act( (!is_chinese(ch, victim)) ? 
						HIY"$N repostes your attack again."NOR :
						HIY"$N ¦A¦¸§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
					act( (!is_chinese(victim, ch)) ?
						HIY"You repostes $n's attack again." NOR:
						HIY"§A¦A¦¸§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
					mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
				}
			*/
			if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
			{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack with secondary weapon."NOR :
					HIY"$N ¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï§A¶i¦æ°lÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack with secondary weapon." NOR:
					HIY"§A¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï $n ¶i¦æ°lÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			}
			return TRUE;
		}
	}

	if(!is_chinese(ch, victim)){
		act( "$N use tactics to parry your attack.", ch, NULL, victim, TO_CHAR );
	}else{
		act( "$N ¬Ý¥X¤F§Aªº°Ê¦V¡A»´ÃP¬[¶}§Aªº§ðÀ».", ch, NULL, victim, TO_CHAR );
	}
	if( !is_chinese(victim, ch)){
		act( "You use tactics to parry $n's attack.",  ch, NULL, victim, TO_VICT );
	}else{
		act( "§A¬Ý¥X¤F $n ªº°Ê¦V¡A»´ÃP¬[¶}$s§ðÀ».",  ch, NULL, victim, TO_VICT );
	}
	return TRUE;
}

bool frenzy_check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff )
{
    int chance;
    int ppl_dex;
    int mod = 100;
	int reposte;  // 191213 dodge reposte
	int dual_reposte; // 2021/11/16 dual_reposte
	bool fReposte = TRUE;  // 191213 dodge reposte
    
    if ( !IS_AWAKE( victim ) )
        return FALSE;

    if ( IS_NPC( victim ) ){
		fReposte = FALSE; //191213
        /* Tuan was here.  :) */
        if( IS_SET(victim->special, EXPERT))
            chance        = UMAX( 10, victim->level / 3 + UMAX( 0, victim->level - ( ( victim->class == 1 ) ? 30 : 60 ) ) );
        else
            chance        = UMAX( 8, victim->level / 4 + UMAX( 0, victim->level - ( ( victim->class == 1 ) ? 20 : 40 ) ) );
    }

	if ((get_skill_level(victim, gsn_fool) + get_skill_level(victim, gsn_dodge) + get_skill_level(victim, gsn_tricky_pacing)) < 4)
		return FALSE;
    else
        chance = (get_skill_percent(victim, gsn_fool) + get_skill_percent(victim, gsn_dodge)) / 33; // 6 , 3

    if( !IS_NPC(victim) ) {
        if (victim->pcdata->autocombat == AUTOC_DODGE ){
            chance += get_skill_percent(victim, gsn_autododge)/33 + get_skill_level(victim, gsn_autododge)*2; // 15
			fReposte = FALSE;
        }
        else if (victim->pcdata->autocombat == AUTOC_PARRY) {
            chance += get_skill_percent(victim, gsn_autoparry)/33 + get_skill_level(victim, gsn_autoparry)*2; // 15
			fReposte = FALSE;
        }
        else if ( victim->pcdata->autocombat == AUTOC_CRITICAL ) {
            chance += get_skill_percent(victim, gsn_deadly_strike)/33 + get_skill_level(victim, gsn_deadly_strike); // 9
        }
		else if( victim->pcdata->autocombat == AUTOC_STOP 
			|| victim->pcdata->autocombat == AUTOC_CONCENTRATION) {
            fReposte = FALSE;
		}
    }

    if ( !IS_NPC( victim ) )
    {
        ppl_dex = get_curr_dex( victim );
        chance += get_skill_level(victim, gsn_fool)*3/2 + get_skill_level(victim, gsn_dodge)*5; // 39 , 30
        chance += ppl_dex / 12 - dexdiff / 12;  //2.5 + 2.5
        chance -= URANGE( 0 , leveldiff / 5 , 15);  //0
        chance -= UMIN( 0 , ( GET_DODGE( victim )
                            * get_skill_level(victim, gsn_dodge) * get_skill_level(victim, gsn_dodge) )/4500); // lv 6 dc -1000 8%
    }
	
    // TrickyPacing dodge chance + 20%
    chance += get_skill_percent(victim, gsn_tricky_pacing)/30 + get_skill_level(victim, gsn_tricky_pacing)*3;

	// stone form ª¬ªp¤Uchance ´î¥b 2022/12/05
	if( is_affected(victim, gsn_stone_form) ){
		fReposte = FALSE;
		chance /= 2;
	}

    if ( number_percent( ) * 100 /mod  >= chance )
        return FALSE;

	//191213  frenzy dodge reposte §P©w
	if( fReposte )
	{
		reposte  = 0;
        reposte += get_skill_level(victim, gsn_reposte) * 5;
		dual_reposte = 0;
		dual_reposte += get_skill_level(victim, gsn_dual) * 2;

		if ( reposte > number_percent( ) 
			&& (get_skill_level(victim, gsn_reposte) > 4)
			&& (victim->move > 100))
		{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack."NOR :
					HIY"$N ¥©§®§Q¥Î§A§ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack." NOR:
					HIY"§A¥©§®§Q¥Î $n §ðÀ»®Éªº¯}ºì¤ÏÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			if( get_eq_char( victim, WEAR_WIELD_2 ) && ( number_percent() < dual_reposte ) )
			{
				act( (!is_chinese(ch, victim)) ? 
					HIY"$N repostes your attack with secondary weapon."NOR :
					HIY"$N ¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï§A¶i¦æ°lÀ»."NOR, ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
					HIY"You repostes $n's attack with secondary weapon." NOR:
					HIY"§A¦b¤ÏÀ»¤¤¥H°Æ¤âªZ¾¹¹ï $n ¶i¦æ°lÀ»."NOR,  ch, NULL, victim, TO_VICT );
				mone_hit ( victim, ch, &gsn_reposte, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			}
				victim->move -= 100;
                return TRUE;
		}
	}

    act( (!is_chinese(ch, victim)) ? 
            "$N dodges your attack." :
            "$N ÆF¥©¦a°{Á×§Aªº§ðÀ».", ch, NULL, victim, TO_CHAR );
    act( (!is_chinese(victim, ch)) ?
            "You dodges $n's attack.":
            "§AÆF¥©¦a°{Á×$nªº§ðÀ».", ch, NULL, victim, TO_VICT );
    return TRUE;
}

/*
 * See if an attack justifies a KILLER flag.
 */
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
	//CHAR_DATA *vch;

	if( !ch || !victim || ch->deleted || victim->deleted )
		return;

	if( ch->in_room != victim->in_room )
		return;

	// ch µ¥©ó victim
	if( ch == victim )
		return;

	// pk room
	if( IS_SET(victim->in_room->room_flags, ROOM_PK) )
		return;

	// ¥ô¤@¤è³B©ó crazy ªºª¬ºA¤Uª½±µ¤£§P©w¥û¤â
	if( IS_AFFECTED( ch, AFF_CRAZY ) || IS_AFFECTED( victim, AFF_CRAZY ) )
		return;

	// §P©w npc ³Q¥´
	if( IS_NPC( victim )
		&& !IS_AFFECTED(victim, AFF_CHARM)
		&& !IS_AFFECTED(victim, AFF_SUMMONED)
		&& !victim->master )
	{
		return;
	}
	/*  ¤U¤è¤w¸g¦³ npc ªº victim Âà´« , ¬G²¾°£¦¹¬q§P©w 2022/12/24
	else if( IS_NPC(victim)
			&& ( IS_AFFECTED( victim, AFF_CHARM ) || IS_AFFECTED( victim, AFF_SUMMONED) )
			&& victim->master && !IS_NPC(victim->master) )
	{
		victim = victim->master;
	} */

	// ¦³ pc ¥D¤Hªº npc §ðÀ»ªº±¡ªp
	if( IS_NPC( ch ) )
	{
		// charm ¤Î clan mob §P©w follow ªº¬O§_¬° pc , ¦pªG¬O«h ch Âà´«¦¨ follow ªº¨¤¦â
		// ª`·N: ¤£¼g pet ¬O¦]¬° pet ¤@©w¬O charm mob , ©Ò¥H charm ´Nµ¥©ó pet
		if( ( IS_AFFECTED( ch, AFF_CHARM ) || ch->clan ) && ch->master && !IS_NPC( ch->master ) )
			ch = ch->master;
		// sor mob §P©w remb ¬O§_¬° pc , ¦pªG¬O«h ch Âà´«¦¨ remb
		if( IS_AFFECTED( ch, AFF_SUMMONED) && ch->rembch && !IS_NPC( ch->rembch ) )
			ch = ch->rembch;
	}

	// ¦³ pc ¥D¤Hªº npc ³Q§ðÀ»ªº±¡ªp
	if( IS_NPC( victim ) )
	{
		// charm ¤Î clan mob §P©w follow ªº¬O§_¬° pc , ¦pªG¬O«h victim Âà´«¦¨ follow ªº¨¤¦â
		if( ( IS_AFFECTED( victim, AFF_CHARM ) || victim->clan ) && victim->master && !IS_NPC( victim->master ) ){
			victim = victim->master;
		}
		// sor mob §P©w remb ¬O§_¬° pc , ¦pªG¬O«h victim Âà´«¦¨ remb
		if( IS_AFFECTED( victim, AFF_SUMMONED) && victim->rembch && !IS_NPC( victim->rembch ) ){
			victim = victim->rembch;
		}
		// gate mob §P©w protect ªº¨¤¦â¬O§_¬° pc , ¦pªG¬O«h victim Âà´«¦¨ protect ªº¨¤¦â
		if( victim->qmark && !str_cmp( victim->qmark->mark_name, "gated_mob" ) && victim->protecting )
			victim = victim->protecting; //I supposed the qmark "gated mob" is the toppest qmark on the gated mob
	}

	// Âà´««á¥i¯àµo¥Í ch »P victim ¤£¦s¦b
	if( !ch || !victim || ch->deleted || victim->deleted )
		return;

	// Âà´««á¥i¯àµo¥Í ch »P victim ¤£¦b¦P room
	if( ch->in_room != victim->in_room )
		return;

	/*
	* NPC's are cool of course
	* Hitting yourself is cool too (bleeding).
	* Hitting immortals are fine.
	* ¦]¬°¤W­±¤w¸g§â pc ¨Ï¥Î pet, charm, gate, sor mob, clan mob Âà´«§¹¦¨
	* ¬G¶]¨ì¦¹³B¦pªG¨ä¤¤¤@¤è¬° npc «h¥Nªí¤£·|µo¥Í pk ¦ÛµM¤£·|¦³ killer ²£¥Í
	* ¦ýÂà´««á¥i¯àµo¥Í¨â­Ó¥D¤H¬Û¦P, ©Î¬O¥D¤H±þ¦Û¤vªº npc
	* ©Ò¥H·í ch ¬° victim ®É¤]¤£·|²£¥Í killer
	* ¨Ã²¾°£µ¥¯Å®t¤£²£¥Í killer ªº¼vÅT 2022/12/24
	*/
	//if ( IS_NPC( ch ) || IS_NPC(victim) || ch == victim || victim->level > LEVEL_HERO + 5 )
	if( IS_NPC( ch ) || IS_NPC(victim) || ch == victim  )
		return;

	// ¦¹³Bªº crazy §P©w¥D­n¬O°w¹ï charm , ¥l³êª« , gate mob Âà´«¦¨ pc «á§@ªº§P©w 2022/11/27
	if( !IS_NPC(ch) && IS_AFFECTED( ch, AFF_CRAZY ) )
		return;
	if( !IS_NPC(victim) && IS_AFFECTED( victim, AFF_CRAZY ) )
		return;

	// pc ³Q charm ªº±¡ªp¤U, ¤£·|§P©w¥û¤â 2022/11/27
	if( !IS_NPC(ch) && IS_AFFECTED( ch, AFF_CHARM ) && IS_NPC( ch->master ) )
		return;
	if( !IS_NPC(victim) && IS_AFFECTED( victim, AFF_CHARM ) && IS_NPC( victim->master ) )
		return;

	// pc ¦³¤@¤è¬° killer ©Î¬O thief ªº±¡ªp¤U¤£·|§P©w¥û¤â 2022/11/27
	if( !IS_NPC(ch) && (IS_SET( ch->act, PLR_KILLER ) || IS_SET( ch->act, PLR_THIEF ) ) )
		return;
	if( !IS_NPC(victim) && (IS_SET( victim->act, PLR_KILLER ) || IS_SET( victim->act, PLR_THIEF ) ) )
		return;

	/*
	* KILLERs are fair game.
	* THIEVES are fair game too.
	* KILLER aggressors should not be penalized 1000 exps per attack but
	* per combat started.
	*/
	if ( IS_SET( victim->act, PLR_KILLER ) || IS_SET( victim->act, PLR_THIEF ) || ( IS_SET( ch->act, PLR_KILLER ) && ch->fighting ) )
		return;

	// Vampires are fair game.
	if ( victim->race == race_lookup( "Vampire" ) )
		return;

	if ( !licensed( ch ) && !licensed(victim) )
	{
		// ²¾°£µ¥¯Å­­¨î 2022/11/27
		//if ( registered( ch, victim ) && ch->level - victim->level < 6 )
		if ( registered( ch, victim ) )
		{
			return;
		}
	}

	//­×§ï¥û¤â§P©w, ¨Ã²¾°£¥û¤â²æ¸Ë³Æªº³]©w 2022/11/27
	if( !registered( ch, victim ) || (!licensed( ch ) && !licensed(victim) ) && victim->fighting != ch )
	{
		char buf [ BUF_STRING_LENGTH ];
		sprintf( buf, "±Ï©R°Ú¡I  %s ¥¿¦b§ðÀ»§Ú¡I", ch->name );
		do_shout( victim, buf );
		send_to_char( "§A¬O­Ó¥O¤H¹½´cªº¥û¤â¡I.\n\r", ch );
		SET_BIT( ch->act, PLR_KILLER );
	}

	/*if ( ( ch->level - victim->level > 5 && victim->fighting != ch ) || !registered( ch, victim ) || (!licensed( ch ) && !licensed(victim) ) )
	{
		OBJ_DATA *obj;
		OBJ_DATA *obj_next;
		char buf [ BUF_STRING_LENGTH ];

		send_to_char( "¥û¤â¡I  §A¥¢¥h¤F "HIG"600"NOR" ÂI¸gÅç.\n\r", ch );
		sprintf( buf, "±Ï©R°Ú¡I  %s ¥¿¦b§ðÀ»§Ú¡I", ch->name );
		do_shout( victim, buf );
		SET_BIT( ch->act, PLR_KILLER );
		for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		{
			if ( IS_NPC( vch ) )
				continue;
			if ( is_same_group( vch, ch ) )
				SET_BIT( ch->act, PLR_KILLER );
		}
		gain_exp( ch, -600 );
		//demote_level( ch );

		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( obj->deleted )
				continue;

			//add by coconet for check horse
			if ( IS_SET( obj->wear_flags , ITEM_WEAR_RIDE ) )
				continue;

			obj_from_char( obj );

			// Remove item inventories Includes licenses to kill
			if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
			{
				extract_obj( obj );
				continue;
			}

			obj_to_char( obj, ch );
		}

		save_char_obj( ch );
	}*/

	return;
}

/** Check to see if weapon is poisoned.
 */
bool is_wielding_poisoned( CHAR_DATA *ch, int wpn )
{
	OBJ_DATA *obj = NULL;
	OBJ_DATA *arrow = NULL;
	OBJ_DATA *ammo = NULL; // 2022/05/03

	if ( ( obj = get_eq_char( ch, wpn ) )
		&& IS_SET( obj->extra_flags, ITEM_POISONED )
		&& obj->value[3] != 14 )
	return TRUE;

	if ( ( obj = get_eq_char( ch, wpn ) )
		&& obj->value[3] == 14
		&& ( arrow = get_eq_char( ch, WEAR_ARROW ) )
		&& arrow->item_type == ITEM_ARROW
		&& IS_SET( arrow->extra_flags, ITEM_POISONED ) )
	return TRUE;

	// ·s¼Wºj±ñ¼uÃÄ§P©w 2022/05/03
	if ( ( obj = get_eq_char( ch, wpn ) )
		&& obj->value[3] == 16
		&& ( ammo = get_eq_char( ch, WEAR_AMMO ) )
		&& ammo->item_type == ITEM_AMMO
		&& IS_SET( ammo->extra_flags, ITEM_POISONED ) )
	return TRUE;

	return FALSE;

}

bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)){
		return TRUE;
    }
    if( !IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM) && ch->group && ch->group->leader == victim ) {
        send_to_char("§A¤£¯à¹ï¿Ë·Rªº¥D¤H¤U¤â¡C\n\r", ch);
        return TRUE;
    }

    if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_PK))
		return FALSE;

	if( IS_SET( race_table[ victim->race ].race_abilities, RACE_VAMPIRE ) )
		return FALSE;
	
	/* suicide? OK....Amenda */
    if ( ch == victim && !IS_SET( victim->act, PLR_HORSE ) )
		return FALSE;

	// ¥l³êª« ¥´ pc ªº pk §P©w
	if( IS_NPC( ch ) && !IS_NPC( victim ) ) // added by Amethyst
	{
		// gate mob
	    if ( ch->qmark && !str_cmp( ch->qmark->mark_name, "gated_mob" )
			&& ch->protecting
			&& !IS_NPC( ch->protecting )
			&& !IS_SET( ch->protecting->act, AFF_CRAZY ) // ¦pªG crazy ´N¯à pk 2022/12/24
			&& !IS_SET( ch->protecting->act, PLR_REGISTER ) )
		{
			if( !IS_NPC(ch->protecting) ) // ¦pªG¥l³ê¥D¬O pc ¤~·|¦¨¥ß 2022/12/24
			{
				send_to_char( "¹ï¤è¨S°Ñ¥[ PK ¡I", ch->protecting );
				return TRUE;
			}
		}
		// sor mob
		if ( IS_AFFECTED( ch, AFF_SUMMONED )
			&& ch->qmark && !str_cmp( ch->qmark->mark_name, "sum_life_time" ) // ¥[¤J sor mob qmark 2022/12/24
			&& ch->rembch
			&& !IS_SET( ch->rembch->act, AFF_CRAZY ) // ¦pªG crazy ´N¯à pk 2022/12/24
			&& !IS_SET( ch->rembch->act, PLR_REGISTER ) )
		{
			if( !IS_NPC( ch->rembch ) ) // ¦pªG¥l³ê¥D¬O pc ¤~·|¦¨¥ß 2022/12/24
			{
				send_to_char( "¹ï¤è¨S°Ñ¥[ PK ¡I", ch->protecting );
				return TRUE;
			}
		}
	}

	// pc ¥´ ¥l³êª« ªº pk §P©w
	if ( IS_NPC( victim ) && !IS_NPC( ch ) ) // !IS_NPC is add here
	{
		// gate mob
		if( victim->qmark && !str_cmp( victim->qmark->mark_name, "gated_mob" )
			&& victim->protecting
			&& !IS_NPC( victim->protecting )
			&& !IS_SET( victim->protecting->act, AFF_CRAZY ) // ¦pªG crazy ´N¯à pk 2022/12/24
			&& !IS_SET( victim->protecting->act, PLR_REGISTER ) )
		{
			if( !IS_NPC( victim->protecting ) ) // ¦pªG¥l³ê¥D¬O pc ¤~·|¦¨¥ß 2022/12/24
			{
				send_to_char( "¥¦ªº¥D¤H¨S°Ñ¥[ PK ¡I", ch );
				return TRUE;
			}
		}
		// sor mob
		if ( IS_AFFECTED( victim, AFF_SUMMONED )
			&& victim->qmark && !str_cmp( victim->qmark->mark_name, "sum_life_time" ) // ¥[¤J sor mob qmark 2022/12/24
			&& victim->rembch
			&& !IS_SET( victim->rembch->act, AFF_CRAZY ) // ¦pªG crazy ´N¯à pk 2022/12/24
			&& !IS_SET( victim->rembch->act, PLR_REGISTER ) )
		{
			if( !IS_NPC( victim->rembch ) ) // ¦pªG¥l³ê¥D¬O pc ¤~·|¦¨¥ß 2022/12/24
			{
				send_to_char( "¥¦ªº¥D¤H¨S°Ñ¥[ PK ¡I", ch );
				return TRUE;
			}
		}
		if ( IS_AFFECTED( victim, AFF_SUMMONED )
			&& victim->qmark && !str_cmp( victim->qmark->mark_name, "sum_life_time" ) // ¥[¤J sor mob qmark 2022/12/24
			&& victim->rembch
			&& !IS_SET( victim->rembch->act, AFF_CRAZY ) // ¦pªG crazy ´N¯à pk 2022/12/24
			&& !IS_SET( victim->rembch->act, PLR_REGISTER ) )
		{
			if( !IS_NPC( victim->rembch ) ) // ¦pªG¥l³ê¥D¬O pc ¤~·|¦¨¥ß 2022/12/24
			{
				send_to_char( "¥¦ªº¥D¤H¨S°Ñ¥[ PK ¡I", ch );
				return TRUE;
			}
		}
	}

	if ( IS_NPC( ch ) || IS_NPC( victim ) )
		return FALSE;

	if ( IS_SET( victim->act, PLR_KILLER ) || IS_SET( victim->act, PLR_THIEF ) )
		return FALSE;

	/*  ²¾°£«OÅ@³]©w 2022/12/24
	if ( ch->level < 16 && !ch->fighting )
	{
		send_to_char( "§Aªºµ¥¯ÅÁÙ¤Ó§C¡A¤£¯à°Ñ¥[ PK.\n\r", ch );
		return TRUE;
	}

	if ( victim->level < 25 )
	{
		act( "$NÁÙ¤£ºâ«_ÀI®a¡A¤Ñ¯«ÁÙ¦b«OÅ@$e", ch, NULL, victim, TO_CHAR );
		return TRUE;
	} */

	// ¦pªG crazy ´N¯à pk 2022/12/24
	if( !IS_SET(ch->act, PLR_REGISTER) && !IS_AFFECTED( ch, AFF_CRAZY ) && !IS_AFFECTED( victim, AFF_CRAZY ) )
	{
		send_to_char("§A¨S¦³°Ñ¥[ PK.\n\r", ch);
		return TRUE;
	}

	if ( ch->level >= L_APP  )
	{
		send_to_char( "§A¤£¯à°Ñ¥[ PK.\n\r", ch );
		return TRUE;
	}

	if ( victim->fighting )
		return FALSE;

	return FALSE;
}

void check_corpse_count( CHAR_DATA * ch, CHAR_DATA *victim)
{
	char buf_q[MAX_INPUT_LENGTH];
	QMARK *qm ;
	int count = 0;

	if( IS_NPC(victim) ) return;

	if( !IS_NPC(ch) ) {
		if( ch == victim ) {
			if( victim->class >BARBARIAN ){
				gain_exp(victim,  - victim->exp / ( 65 - victim->level / 4));
			}
			if( victim->class <= BARBARIAN ) {
				gain_exp(victim,  - victim->exp / ( 85 - victim->level / 4));
			}
		}
		return;
	}
	sprintf(buf_q, "corpse_count_%d", ch->pIndexData->vnum);
	qm =  is_qmark(victim, buf_q);
	if( victim->in_room ) {
	//for ( obj = victim->in_room->contents; obj; obj = obj->next_content )
	//	if ( is_name( victim->name, obj->short_descr ) )
	//		count ++;
	if( qm )
		count = qm->v0;
	else count = 0;

	if ( count > 5 )
	{
		count -= 2;
		victim->exp = victim->exp - 100 * count * count;
		send_to_char( "§A¦]¬°°ï¿n«ÍÅé¡A¤Ñ¯«´î¤Ö§Aªº¸gÅç»P¥Í©R§@¬°³B»@¡I\n\r", victim );
		victim->max_hit -= count * count;
		victim->max_hit = UMAX( victim->max_hit, 1);
		//if (ch->clan){
		//	ch->clan->score -=  500*ch->level ;
		//	ch->clan->bank -= UMAX( 0, 500*ch->level * 2 );
		//	ch->pcdata->clanexp -= 500*ch->level;
		//}
	}else{


			//20060722 modify by jye
				//·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ
			if (victim->exp <= EXP_PER_LEVEL(victim->level-1,victim->class) * victim->level )
				return;
			if( victim->class > BARBARIAN ){
				gain_exp(victim,  - victim->exp / ( 60 - victim->level / 4));
			}
			if( victim->class <= BARBARIAN ) {
				gain_exp(victim,  - victim->exp / ( 80 - victim->level / 4));
			}
			//20060722 modify by jye
				//·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ
			victim->exp = UMAX(victim->exp, EXP_PER_LEVEL(victim->level -1,victim->class) * victim->level);
		}
	}
}

CHAR_DATA * check_mob_ai( CHAR_DATA *ch)
{
	if( !IS_NPC(ch) )
		return ch->fighting;
	if( IS_SET(ch->special, MOBAI_LV3)){
		SET_BIT(ch->act, ACT_MOB_AI);
		return mob_ai_lv3(ch);
	}
	else if (IS_SET(ch->special, MOBAI_LV2)){
		SET_BIT(ch->act, ACT_MOB_AI);
		return mob_ai_lv2(ch);
	}
	else
		return mob_ai_lv1(ch);
}

CHAR_DATA * mob_ai_lv3 (CHAR_DATA *ch)
{
    CHAR_DATA *victim = ch->fighting, *rch;
    int class_count[ MAX_CLASS ];
    int i;
    //int easy_kill;
    
    for( i = 0; i < MAX_CLASS; ++ i ) {
	class_count[i] = 0;
    }
    if ( victim->level > 35 && ch->level > 45 &&
	    ( number_percent( ) < IS_SET( ch->act, ACT_MOB_AI ) ? 
	      ch->level : ch->level - 45  ) )
    {
	for( rch = ch->in_room->people; rch; rch = rch->next_in_room ){
	    if ( rch->fighting == ch && !rch->deleted )
	    {
		if ( !can_see( ch, rch ) )
		    continue;
		if ( ch->hitroll < - GET_DODGE( rch ) / 8 )
		    continue;
		if ( rch->class == MONK || rch->class == WARLORD 
		   || rch->class == ALCHEMIST ||rch->class == MARTIALMAN )
		{
			victim = rch;
			//return a void value?? fix by neversay on 2004/June/22
			//return;
			break;
		}
		if ( rch->class == SAVANT || rch->class == BISHOP )
		    victim	= rch;

		if ( rch->class != SHAMAN && number_bits(3) == 1 )
		{
		    victim = rch;
		    break;
		}
	    }
        }
    }
    return victim;

}

CHAR_DATA * mob_ai_lv2 (CHAR_DATA *ch)
{
    CHAR_DATA *victim = ch->fighting, *rch;
    int type;
    int min;
    if( ch && ch->class && (IS_SET(ch->class, 16) || IS_SET(ch->class, 2)))
	type = 0; //mage
    else
	type = 1; //warrior
    switch( type ){
	case 1:
	    min = ch->fighting->hit * 5 - GET_AC(ch->fighting) * 3;
	    if ( victim->level > 35 && ch->level > 45 &&
		    ( number_percent( ) < IS_SET( ch->act, ACT_MOB_AI ) ? 
		      ch->level : ch->level - 45 ))
	    {
		for( rch = ch->in_room->people; rch; rch = rch->next_in_room ){
		    if ( rch->fighting == ch && !rch->deleted )
		    {
			if ( !can_see( ch, rch ) )
			    continue;
			if ( ch->hitroll < - GET_DODGE( rch ) / 8 )
			    continue;

			if( rch->hit * 5 - GET_AC(rch) * 3 < min ){
			    min = rch->hit*5- GET_AC(rch) * 3 ;
			    victim = rch;
			}
		    }
		}
	    }
	    break;

	case 0:
	    min = victim->saving_throw[0] * -3 + victim->hit;
	    if ( victim->level > 35 && ch->level > 45 &&
		    ( number_percent( ) < IS_SET( ch->act, ACT_MOB_AI ) ? 
		      ch->level : ch->level - 45 ))
	    {
		for( rch = ch->in_room->people; rch; rch = rch->next_in_room ){
		    if ( rch->fighting == ch && !rch->deleted )
		    {
			if ( !can_see( ch, rch ) )
			    continue;
			if( rch->saving_throw[0] * -3 + rch->hit < min ){
			    min = rch->saving_throw[0] * -3 + rch->hit;
			    victim = rch;
			}
		    }
		}
	    }
	    break;
    }
    return victim;

}

CHAR_DATA * mob_ai_lv1 (CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting, *rch;
	if ( victim->level > 35 && ch->level > 45 &&
			( number_percent( ) < IS_SET( ch->act, ACT_MOB_AI ) ? 
			  ch->level : 
			  ch->level - 45 + ( victim->class == SHAMAN ? 20 : 0 ) ) )
	{
		for( rch = ch->in_room->people; rch; rch = rch->next_in_room ){
			if ( rch->fighting == ch && !rch->deleted )
			{
				if ( !rch )
					continue;
				if ( !rch->class )
					continue;
				if ( !can_see( ch, rch ) )
					continue;
				if ( ch->hitroll < - GET_DODGE( rch ) / 8 )
					continue;
				if ( rch->class == SAVANT || rch->class == BISHOP )
					victim	= rch;
				else if( rch->class == WIZARD || rch->class == ALCHEMIST )
					victim	= rch;
				else if( rch->class == LORD )
					victim	= rch;
				else if ( rch->class == CLERIC || rch->class == MAGE )
					victim	= rch;

				if ( rch->class != SHAMAN && number_bits(2) == 1 )
				{
					victim = rch;
					break;
				}
			}
		}
	}
	return victim;

}

/*have registered or have license or is vampire*/
bool licensed ( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if ( ch->race == race_lookup( "Vampire" ) )
	return TRUE;

    if ( !IS_SET( ch->act, PLR_REGISTER ) )
	return FALSE;

    /*
     * If already fighting then we just jump out.
     */
    if ( ch->fighting )
	return TRUE;

    for ( obj = ch->carrying; obj; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == OBJ_VNUM_LICENSE )
	    return TRUE;
    }

    return FALSE;

}

/*have registered or is vampire*/
bool registered ( CHAR_DATA *ch, CHAR_DATA *victim )/*{{{*/
{
    if( ch->in_room == victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_PK))
	return TRUE;
    if ( IS_SET( victim->act, PLR_REGISTER ) )
	return TRUE;

    if ( ch->race == race_lookup( "Vampire" ) )
	return TRUE;

    return FALSE;

}/*}}}*/

/* This code is for PC's who polymorph into dragons.
 * Yeah I know this is specialized code, but this is fun.  :)
 * Breathe on friend and enemy alike.
 * -Kahn
 */

void pc_breathe( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    int	       sn;

    send_to_char( "You feel the urge to burp!\n\r", ch );
    act( "$n belches!", ch, NULL, NULL, TO_ROOM );
    for ( victim = ch->in_room->people; victim; victim = victim_next )
    {
	victim_next = victim->next_in_room;
	if ( victim->deleted )
	    continue;

	if ( victim == ch )
	    continue;

	if ( !IS_NPC( victim )
	    && ( !registered( ch, victim )
		|| ( !licensed( ch )
		    && victim->race != race_lookup( "Vampire" ) ) ) )
	    continue;

	sn = skill_lookup( "fire breath" );
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    }

    return;
}

/* This code is for PC's who polymorph into harpies.
 * Yeah I know this is specialized code, but this is fun.  :)
 * Scream into the ears of enemy and friend alike.
 * -Kahn
 */

void pc_screech( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    int	       sn;

    send_to_char( "You feel the urge to scream!\n\r", ch );
    interpret( ch, "scream" );
    for ( victim = ch->in_room->people; victim; victim = victim_next )
    {
	victim_next = victim->next_in_room;
	if ( victim->deleted )
	    continue;

	if ( victim == ch )
	    continue;

	if ( !IS_NPC( victim )
	    && ( !registered( ch, victim )
		|| ( !licensed( ch )
		    && victim->race != race_lookup( "Vampire" ) ) ) )
	    continue;

	act( "Your ears pop from $n's scream.  Ouch!", ch, NULL, victim,
	    TO_VICT );
	sn = skill_lookup( "agitation" );
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    }

    return;
}



void pc_spit( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    int	       sn;

    send_to_char( "You feel the urge to spit!\n\r", ch );
    act( "$n spews vitriol!", ch, NULL, NULL, TO_ROOM );
    for ( victim = ch->in_room->people; victim; victim = victim_next )
    {
	victim_next = victim->next_in_room;
	if ( victim->deleted )
	    continue;

	if ( victim == ch )
	    continue;

	if ( !IS_NPC( victim )
	    && ( !registered( ch, victim )
		|| ( !licensed( ch ) 
		    && victim->race != race_lookup( "Vampire" ) ) ) )
	    continue;

	act( "You are splattered with $n's vitriol.  Ouch!", ch, NULL, victim,
	    TO_VICT );
	sn = skill_lookup( "poison" );
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );

	////damage( ch, victim, number_range( 1, ch->level ), gsn_poison_weapon, WEAR_NONE );
	damage( ch, victim, number_range( 1, ch->level ), gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
    }

    return;
}


bool check_race_special( CHAR_DATA *ch )
{
    return FALSE;
    if ( ch->race == race_lookup( "Dragon" ) )
    {
	if ( number_percent( ) < ch->level )
	{
	    pc_breathe( ch );
	    return TRUE;
	}
    }

    if ( ch->race == race_lookup( "Harpy" ) )
    {
	/*
	if ( number_percent( ) < ch->level )
	{
	    pc_screech( ch );
	    return TRUE;
	}
	*/
    }

    if ( ch->race == race_lookup( "Arachnid" )
	|| ch->race == race_lookup( "Snake" ) )
    {
	if ( number_percent( ) < ch->level )
	{
	    pc_spit( ch );
	    return TRUE;
	}
    }

    return FALSE;
}


void check_wimpout(CHAR_DATA *ch, CHAR_DATA *victim)
{
    //----®£Äßª¬ºA·|¦Û°Ê°k¶]

    if ( IS_AFFECTED( victim, AFF_FEAR ) )
    {
        do_flee(victim, "");
    }


    if ( IS_NPC( victim ) )
    {
	if (    (    IS_SET( victim->act, ACT_WIMPY )
	          && number_bits( 1 ) == 0
                  && victim->hit < victim->max_hit / 2 
                  && victim->wait == 0 )
             || (    (    IS_AFFECTED( victim, AFF_CHARM ) 
                       || IS_AFFECTED( victim, AFF_SUMMONED ) 
                       || victim->clan )
                  && victim->master && victim->in_room 
                  && victim->master->in_room != victim->in_room ) )
        {
	    do_flee( victim, "" );
	}
    }
    //modified by coconet for let ppl in stun can not wimpy
    else
    {
    	if(    victim->hit   > 0
            && victim->hit  <= victim->wimpy 
            && victim->wait < 3 )
    	{
	    do_flee( victim, "" );
    	}
    }
}



/* Reflexion skill by Amenda, 01/2/3 */
/* Managed by Chris,    05/7/2 */

bool check_reflexion(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam, int dexdiff)
{
	int chance = 0;

	// ¤Ï¦V¦¨¥ßªº±¡ªp¤U¦b safe room ¤£·|§@¥Î 2021/12/22
	if( is_safe( ch, victim ) )
		return FALSE;

	// ¦¹³B¥Ã»·¤£·|¶]¨ì gsn_reposte , reposte ­×§ï¬° DMG_ADJUST ©ó fight_damage ¤ºªý¾× 2021/11/16
	//if ( ( dt >= TYPE_HIT || dt == gsn_frenzy ) && dt != gsn_reposte && dt != 1015)
	if ( ( dt >= TYPE_HIT || dt == gsn_frenzy ) && dt != 1015 )
	{
		int ref_cost;
		int nReflexSklv = get_skill_level(victim, gsn_reflexion);
			/* npc §P©w¨S¦³·N¸q, ¥B reflexion sklv À³§P©w victim ¦Ó«D ch 2022/12/30
			ref_cost = ( IS_NPC( victim ) ) ? 0 : dam / 8 + 1;
			ref_cost = UMAX(10, ref_cost - get_skill_level(ch, gsn_reflexion)*4); */
		// sklv ·|½T¹êÅãµÛ¼vÅT¨ì mv ¦©°£ 2022/12/30
		ref_cost = dam / ( 10 + nReflexSklv );
		ref_cost = UMAX( 10 - nReflexSklv , ref_cost );

		// ch hp <= 0 ®É mv cost °£ 2 ( ¦º«e§ðÀ» )2022/12/30
		if( ch->hit <= 0 )
			ref_cost = ref_cost/2;

		if( (victim->move >= ref_cost )
			&& (   ( !IS_NPC( victim ) && get_skill_percent(victim, gsn_reflexion) )
				|| ( IS_NPC( victim ) && IS_SET( victim->act, ACT_REFLEXION ) ) ) )
		{
			//int nReflexSklv = get_skill_level(victim, gsn_reflexion);

			// beholder inborn 5 ¥H¤W¦³¸û°ªªº reflexion
			if(IS_SET( race_table[ victim->race ].race_abilities, RACE_BEHOLDER )){
				chance += ( get_curr_dex(victim) * ( 3 + get_skill_level(victim, gsn_inborn_ability) ) )/ 10;
			}else{
				// ¨S¦³ wp2 ªº±¡ªp reflexion ªº dex ¥[¦¨¤U­°¬° 80%
				if( !get_eq_char( victim, WEAR_WIELD_2 ) )
					chance += ( get_curr_dex(victim) * 4 )/ 5;
				else
					chance += get_curr_dex(victim);
			}

			chance += nReflexSklv * (nReflexSklv + 3);

			//¨CÂI dex ®t¼vÅT 3%
			chance -= dexdiff * 3;

			if (!IS_NPC(victim))
			{
				// «D autoc_dodge ªº±¡ªp¤U
				if (victim->pcdata->autocombat != AUTOC_DODGE ){
					// cost ÅÜ¦¨ 1.5 ­¿ 2021/11/16
					ref_cost = ref_cost * 1.5;
				}else{
					// ¥[¤J autododge ªº sklv ¼vÅT 2022/12/30
					chance += 2 + get_skill_level(victim, gsn_autododge) * 3;
				}
				// cost ¤£°÷ªº±¡ªp¤U´N¥¢±Ñ
				if( victim->move < ref_cost )
					return FALSE;
			}

			// stone form ª¬ªp¤Uchance ´î¥b 2022/12/05
			if( is_affected(victim, gsn_stone_form) ){
				chance /= 2;
			}

			if ( number_percent() < chance )
			{
				if ( !IS_NPC( victim ) ){
					victim->move -= ref_cost;
				}
				if(dt != 1014){
					return TRUE;
				}else if(number_percent() * 3 < chance * 2){
					act( (is_chinese(ch, victim)) ?
						"$N ¹B¥Î§Þ¥©±µ¦í¤F§Aªº½b¥Ú !" :
						"$N receives your shot.", ch, NULL, victim, TO_CHAR );
					act( (is_chinese(victim, ch)) ?
						"§A¹B¥Î§Þ¥©±µ¦í¤F $n ªº½b¥Ú !" :
						"You receive $n's shot.",  ch, NULL, victim, TO_VICT );
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}




//®Ú¾Ú§ðÀ»ªÌªºLevel¹ï¶Ë®`¶i¦æ½Õ­°¡A¥H©µªø°ªµ¥¯Åªº¾Ô°«
int check_damage_adjust(CHAR_DATA *ch, int dam)
{
    if( ch->level > 29 )
    {
        dam = UMAX(1, dam - ( dam / 4) + 1);
    }

    if( ch->level > 49 )
    {
        dam = UMAX(1, dam - ( dam / 5) + 1);
    }

    //dam = UMAX(1, dam - ( dam / 4) + 1);
  
    // Marked by Razgriz   20060605
	// ºû«ù­ì¥»ªº¶Ë®`¿é¥X
    //if ( IS_NPC(ch) )
    //{
	  dam = dam * 15 / 16;
    //}

    return dam;
}



//Spell Fury
int check_spellfury(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    if( IS_NPC(ch) && IS_SET(ch->special, SPELL_FURY) )
    {
        if( number_percent() < ch->level + 10 )
            dam += dam / 4;
        if( IS_SET(ch->special, EXPERT) && number_percent() < ch->level / 3 )
            dam += dam / 2;
    }
    else
    {
        int fury  = 0;
        int nSklv = get_skill_level(ch, gsn_spell_fury);
        int nPerc = get_skill_percent( ch, gsn_spell_fury );
        int nInt  = get_curr_int(ch);
        // Beholder ¤Ñ¥Í¦³fury add at 19/2/2
        int nBehSklv = 0;
        int nBehFuryPerc = get_skill_percent(ch, gsn_inborn_ability);

        //  int cint = get_curr_int(ch)+nSklv+nSklv/2+nSklv/3+(nSklv/6)*3;
        //  OLD SPELL FURY
        //      SKLV    0    1    2    3    4    5    6
        //              0    1    3    5    7    8   14
        //  ¤Þµo MIN   25   26   29   30   32   33   39
        //  «Â¤O MIN 1.07  1.1  1.2  1.3  1.4  1.5  1.6
        //  ¤Þµo MED   10 10.1 11.7   12 12.5   13   15
        //  «Â¤O MED 1.10            1.5            2.0
        //  «Â¤O MAX 1.17         1.8         2.6

        //  MIN        0    1    2    3     4     5     6
        //  MIN     10.0 20.0 30.0 40.0  50.0  60.0  70.0
        //  MAX     20.0 32.5 50.0 72.5 100.0 132.5 170.0

        // Beholder ¤Ñ¥Í¦³fury add at 19/2/2
        if(!IS_NPC(ch) && IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER )){

			nBehSklv = get_skill_level(ch, gsn_inborn_ability);

            if(nPerc > nBehFuryPerc/3){
               //nPerc = nPerc + nBehFuryPerc/9 + nBehSklv * 6;
			   // ib lv 6 99% , fury percent mag = 89% , wiz = 124%
			   nPerc = 2 + nPerc + nBehFuryPerc/9 + nBehSklv * 2;
            }else{
               nPerc = nBehFuryPerc/9 + nBehSklv * 8;
            }
            if((nBehSklv - 1) > nSklv) nSklv = nBehSklv - 1;
        }

        fury = 0;
        if(nPerc > 0 && number_percent() < nInt / 2 + nBehSklv*2 + nSklv * (nSklv + 14) / 4){
			//fury = number_range(10 + nSklv * 10, 20 + nSklv * 10 + 10 * nSklv * nSklv / 4);
			// ­°§Cfury³Ì¤j­È­¿¼Æ 2020/09/18
			//  old 60% sklv 6  1.51 ~ 2.25 , beh  1.64 ~ 2.56
			//  old 99% sklv 6  1.7 ~ 2.7 , beh  1.86 ~  3.17
			//  new 60% sklv 6  1.55 ~ 2 ,  beh  1.7 ~ 2.26
			//  new 99% sklv 6  1.75 ~ 2.36 , beh  1.87 ~ 2.6
            fury = number_range(15 + nSklv * 10, 37 + nSklv * 12 + 3 * nSklv * nSklv / 4);
            dam  = dam * (10000 + fury * (nPerc * 2 + 102) / 3) / 10000;

            if(fury * (nPerc * 2 + 102) / 3 < 10000){
                if( is_chinese(ch, victim) )
                    send_to_char("§AªºÅ]ªk¯à¶q³y¦¨«D¤ñ´M±`ªº¶Ë®`¡I\n\r", ch);
                else
                    send_to_char("Your spell power goes into fury!\n\r", ch);
            }else{
                if( is_chinese(ch, victim) )
                    send_to_char("§AªºÅ]ªk¯à¶q³y¦¨¶W¥G·Q¹³ªº¶Ë®`¡I\n\r", ch);
                else
                    send_to_char("Your spell power goes into extreme fury!\n\r", ch);
            }
        }
        /* OLD VERSION *
        int cint = get_curr_int(ch) + nSklv + nSklv/2 + nSklv/3 + (nSklv/6)*3;
        
        if( number_percent() < (nPerc * cint) / 100)
        {
          	dam += dam*nSklv/10 + (nSklv == 0 ? dam/15 : 0 );
            fury = 1;
        }
        
        if( number_percent() < (nPerc * cint) / 250)
        {
            dam += dam*nSklv/6 + (nSklv == 0 ? dam/10 : 0);
            ++fury;
        }
        
        switch( fury )
        {
            case 1:
                if( is_chinese(ch, victim) )
                {
                    send_to_char("§AªºÅ]ªk¯à¶q³y¦¨«D¤ñ´M±`ªº¶Ë®`¡I\n\r", ch);
                }
                else
                {
                    send_to_char("Your spell power goes into fury!\n\r", ch);
                }
                break;

            case 2:
                if( is_chinese(ch, victim) )
                {
                    send_to_char("§AªºÅ]ªk¯à¶q³y¦¨¶W¥G·Q¹³ªº¶Ë®`¡I\n\r", ch);
                }
                else
                {
                    send_to_char("Your spell power goes into extreme fury!\n\r", ch);
                }
                break;
        };
        **/
    }
    return dam;
}



//­pºâ°½Å§ªº¥[¦¨»P§ðÀ»©Û³êª«ªº«áªG
int check_dirty_raid(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    if ( victim->position >= POS_SLEEPING )
    {
	if ( !victim->fighting )
	    set_fighting( victim, ch );

	if ( !ch->fighting )
	    set_fighting( ch, victim );

	if (    IS_NPC( ch )
	     && IS_NPC( victim )
	     && victim->master
	     && ch->in_room
             && victim->master != ch
	     && victim->master->in_room == ch->in_room
	     && number_bits( 2 ) == 0 )
	{
	    stop_fighting( ch, FALSE );
    	    set_fighting( ch, victim->master );
    	    return -1;
	}
    }
    else
    {
        dam = dam + dam / 2;
    }

    //§ðÀ»¦Û¤vªºÃdª«©Î©Û³êª«
    if ( victim->master == ch )
    {
        stop_follower( victim );
       // REMOVE_BIT( victim->affected_by, AFF_SUMMONED );  ¶}¾Ô²¾°£ aff_summoned·|³y¦¨sor mob³\¦h§P©w¤W°ÝÃD  19/11/27
        REMOVE_BIT( victim->affected_by, AFF_CHARM );
    }
    return dam;
}


bool check_can_do_hit(CHAR_DATA *ch, CHAR_DATA *victim)
{

	if( !victim || ( victim == NULL ) || victim->deleted)
		return FALSE;

	if( ch->deleted )
		return FALSE;

	if( ch->position == POS_PARALYZED )
		return FALSE;

	if( is_affected(ch, gsn_stone_form) )
		return FALSE;

	if( !IS_NPC(ch) ){
		if(ch->pcdata->autocombat == AUTOC_STOP) return FALSE;
		if(ch->pcdata->autocombat == AUTOC_DODGE) return FALSE;
		if(ch->pcdata->autocombat == AUTOC_PARRY) return FALSE;
		if(ch->pcdata->autocombat == AUTOC_CONCENTRATION) return FALSE;
		if(ch->pcdata->autocombat == AUTOC_INSTANT) return FALSE;
	}

	if( !IS_NPC(ch) && ch->position == POS_RESTING )
		return FALSE;

	if( ch->wait > 0 ) return FALSE;

	return TRUE;
}


