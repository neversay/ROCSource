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
//*  PROGRAM ID  : fight.c                                        *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940619 Razgriz                                *
//*  PURPOSE     :         ­×§ï do_feed¡A¤£ºÞºØ±Ú¬°¦ó             *
//*              :         ¦b autoc stop ªº±¡§Î¤U¡Afeed ¨S¤¤®É¡A  *
//*              :         ¨ÌµM·|¶}¾Ô                             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940701 jye                                    *
//*  PURPOSE     :         ­×§ï do_kill ®³±¼¤Ukill«ü¥O«á·|stun    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940713 jye                                    *
//*  PURPOSE     :         ­×§ï group_gain obj ·lÃa«á±¼¦bppl¨­¤W  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940723 jye                                    *
//*  PURPOSE     :         ­×§ï do_flee ªº±ø¥ó                    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940909 Razgriz                                *
//*  PURPOSE     :         ­×§ï violence_update(),¦b ch->wait ¬° 0*
//*              :         ®É, ²¾±¼ qmark -- earthquake_stun      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940909 Razgriz                                *
//*  PURPOSE     :         ­«¼g stop_fighting(),¤@¤è­±¸Ñ¨M·í fBoth*
//*              :         ¬° true ®É¥i¯à·|³y¦¨ªº¤@¨ÇÅÞ¿è¿ù»~, ¥t *
//*              :         ¤@¤è­±, ¥[¤W¹ï qmark -- earthquake_stun*
//*              :         ªº³B²z, ·í°±¤î¾Ô°«®É´N²¾±¼¦¹ qmark     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940919 jye                                    *
//*  PURPOSE     :         ­×§ï ride lv4 ·|¦Y±¼¤@¥óeqªºbug        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941005 Razgriz                                *
//*  PURPOSE     :         ­×§ï trigger_obj_hp Á×§K murder self   *
//*              :         ²£¥Íªº artifact µL³d¥ô³sµoªº bug       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950714 jye                                    *
//*  PURPOSE     :         ­×§ï xp_compute                        *
//*              :              ppl¤j©ómobµ¥¯Å5¯Å®Éreturn 0       *
/*****************************************************************/
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"

extern int GetArmorMasteryPdrBonus(CHAR_DATA *ch);
extern bool IsSorGateMob(int nMobIndex);
extern int check_item_magic_flag(OBJ_DATA *, int, int);
extern bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);
extern bool frenzy_check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff );
extern int get_critical_damage(int dam, CHAR_DATA *ch, CHAR_DATA *victim, int dexdiff, bool fCritical, int dt, int darkraid);
/*
 * Local functions.
 */
extern bool check_evade(CHAR_DATA *ch, CHAR_DATA *victim);
//extern bool check_parry(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff);
extern bool check_parry(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn); // 2022/12/04
extern bool check_block(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff);
extern bool check_tactics(CHAR_DATA *ch, CHAR_DATA *victim);
void dam_message(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn);
void c_dam_message(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn);
void death_cry( CHAR_DATA *ch);
void group_gain(CHAR_DATA *ch, CHAR_DATA *victim);
int xp_compute(CHAR_DATA *gch, CHAR_DATA *victim);
bool is_wielding_poisoned (CHAR_DATA *ch, int wpn);
void make_corpse(CHAR_DATA *killer, CHAR_DATA *ch);
int one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn);
int artifact( CHAR_DATA *ch);
////int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode ); //Modified by Razgriz 20051014
////int brief_mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int* dt, int wpn );

extern bool check_race_special( CHAR_DATA *ch);
void use_magical_item( CHAR_DATA *ch );
void track_hunt(CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door);
int interpolate1( int level1, int value1, int level2, int value2, int level );
extern void check_corpse_count( CHAR_DATA * ch, CHAR_DATA *victim);
int get_align( CHAR_DATA *ch );
void show_vs_graphy(CHAR_DATA *ch, CHAR_DATA *victim);
void death_declare(CHAR_DATA *ch, CHAR_DATA *victim);
void pk_kill(CHAR_DATA *ch, CHAR_DATA *victim);

QMARK *qmark_list, *qmark_free;
QMARK_STRING *qmark_string_list;
CLAN_WAR_DATA *clan_war;

const int movement_loss2[ SECT_MAX ] =
{
    1, 2, 2, 3, 4, 6, 4, 1, 5, 10, 6
};


char* stat_color1(int cur, int max)
{
  int ratio ;
  if( max == 0 ) return HIR;
  ratio = cur * 100 / max;
  if( ratio < 25 ) return HIR;
  else if(ratio < 50) return HIY;
  else return HIW;
}

/*  ¬rÅ§­×§ï¬°¨C¤U§ðÀ»¶Ë®` 2021/11/04
void VenomousHurt(CHAR_DATA *pVictim)
{
    int nVenomDmg = affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON);
    int nDamage = pVictim->max_hit * nVenomDmg / 30000 + nVenomDmg / 2;
    if(nDamage > nVenomDmg * 10) nDamage = nVenomDmg * 10;
    if(!IS_NPC(pVictim)) nDamage /= 2;
	if(pVictim->hit < pVictim->max_hit * 0.05) nDamage = 0;
    pVictim->hit -= nDamage;
}
*/

void BloodThirstyStopCheck(CHAR_DATA *ch) /* blood thirsty °±¤î¾Ô°««á¼Æ­È§P©w 2022/02/21 */
{
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, gsn_blood_thirsty);

	if( affected_value( ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) )
	{
		if(affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) > 0)
		{
			ch->hit = UMIN( ch->max_hit ,
							ch->hit + 10 + affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT)/10
									* ( 3 + get_skill_level( ch, gsn_blood_thirsty ) * 2 ) );
		}
		affect_strip( ch, gsn_blood_thirsty);
		return;
	}

	if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) > 0 )
	{
		af.type      = gsn_blood_thirsty;
		af.duration  = 0;
		af.location  = APPLY_DAMAGE_COUNT;
		af.modifier  = 0 - ( 100 + affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT)/(2 + sklv/6) );
		af.bitvector = 0;
		affect_join( ch, &af );
	}

	if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) <= 0 )
		affect_strip( ch, gsn_blood_thirsty );

	return;
}

// obj_fight , ©ó¾Ô°«¤¤ obj prog ªº§P©w 2022/05/07
void CheckObjFight(CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	for ( obj = ch->carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;

		if( !(obj->carried_by) ) continue;

		if ( obj->pIndexData->progtypes & OBJ_FIGHT )
			oprog_fight_trigger(obj);
	}
	return;
}

// obj_kill , ©ó¾Ô°«¤¤ obj prog ªº§P©w 2022/05/08
void CheckObjKill(CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	for ( obj = ch->carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;

		if( !(obj->carried_by) ) continue;

		if ( obj->pIndexData->progtypes & OBJ_KILL )
			oprog_kill_trigger(obj);
	}
	return;
}

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 * Slightly less efficient than Merc 2.2.  Takes 10% of 
 *  total CPU time.
 */
void violence_update( void )
{
	CHAR_DATA *ch;
	CHAR_DATA *victim;
	CHAR_DATA *rch;
	bool       mobfighting;
	int        nRaceSklv;

	for ( ch = char_list; ch; ch = ch->next )
	{
		if ( !ch->in_room || ch->deleted )
			continue;

		if ( IS_NPC( ch )  ) {
			if( ch->fighting && ch->position == POS_RESTING ){
				ch->position = POS_FIGHTING;
				ch->wait += 9;
				continue;
			}
			if( ch->wait ){
				ch->wait = UMAX( 0, ch->wait - 9 );
				continue;
			}
		}
		if ( !ch->desc  ) {
			if( ch->wait ){
				ch->wait = UMAX( 0, ch->wait - 6 );
				continue;
			}
			if( ch->fighting && ch->position == POS_RESTING ){
				ch->position = POS_FIGHTING;
				ch->wait += 6;
				continue;
			}
		}

		nRaceSklv = get_skill_level(ch, gsn_inborn_ability);

		if ( ( victim = ch->fighting ) )
		{
			if ( victim->deleted || ch->in_room != victim->in_room )
			{
				stop_fighting( ch, TRUE );
				continue;
			}
			if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) ) {
				stop_fighting( ch, TRUE);
				continue;
			}

			if( ch->master == victim ) {
				if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
				}else{
					stop_follower(ch);
				}
			}
			if( victim->master == ch ) {
				if(victim->pIndexData != NULL && IsSorGateMob(victim->pIndexData->vnum)){
				}else{
					stop_follower(victim);
				}
			}
			
			// °õ¦æ obj_fight prog 2022/05/07
			CheckObjFight(ch);

			// affect by no heal wouldn't gain hitpoint add at 2018.11.20
			if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_VAMPIRE )
				&& ( time_info.hour < 6 || time_info.hour > 18 )
				&& !IS_AFFECTED(ch, AFF_NO_HEAL ) )
			{
				if( ( !IS_NPC(ch) && !IS_NPC(victim) )
					|| (!IS_NPC( victim )
						&& victim->pcdata->clanpk
						&& IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
					ch->hit += ch->level / 4 + nRaceSklv + nRaceSklv * nRaceSklv / 2;
				else
					ch->hit += ch->level + nRaceSklv * 5 + nRaceSklv * nRaceSklv * 2;
				ch->hit = UMIN( ch->hit, ch->max_hit);
				//  ch->hit = ( ch->hit > ch->max_hit ) ? ch->max_hit : ch->hit;
			}

			if ( is_affected(ch, gsn_stone_form) && !IS_AFFECTED(ch, AFF_NO_HEAL ) )
			{
				// ¥[¤J gargoyle ¨Ï¥Îªº stone form ¤~·|¦^¦å 2022/12/05
				if( ( affected_value( ch, gsn_stone_form, APPLY_RACE ) == -16 ) && ( ch->race == race_lookup( "Statue" ) ) )
				{
					if( (    !IS_NPC(ch) && !IS_NPC(victim) )
						|| ( !IS_NPC( victim ) && victim->pcdata->clanpk && IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
					{
						//ch->hit += ch->level / 4 + nRaceSklv + nRaceSklv * nRaceSklv / 2;
						ch->hit += ch->level + ( ch->max_hit * ( 2 + nRaceSklv ) ) / 200; // pk ¤U¬° lv + 1~ 4%
					}
					else
					{
						//ch->hit += ch->level + nRaceSklv * 5 + nRaceSklv * nRaceSklv * 2;
						ch->hit += ch->level + ( ch->max_hit * ( 2 + nRaceSklv ) ) / 100; // lv + 2 ~ 8%
					}
					ch->hit = UMIN( ch->hit, ch->max_hit);
					ch->mana -= 30 + nRaceSklv * 20;
					if(ch->mana < 0){
						ch->mana = 0;
						affect_strip(ch, gsn_stone_form);
						send_to_char("§A¨­Åéªº¥Û¤Æª¬ºAº¥º¥®ø¥¢¤F.\n\r"NOR, ch);
						act( "$n"CYN"¨ø¤U¤FÀJ¹³¯ëªº¥~´ß¡A±q¦a­±®¶¯Í­¸µ¾¨ìªÅ¤¤¡C"NOR, ch, NULL, NULL, TO_NOTVICT );
					}
				}
			}

			//¾Ô°«¤¤¨Cround¤@©w¾÷²v¸Ñ°£mob °k¶]­­¨î , 2021/12/22
			if( IS_NPC(ch) ){
				if( IS_SET( ch->act, ACT_NO_FLEE ) && number_bits( 1 ) == 1 )
					REMOVE_BIT( ch->act, ACT_NO_FLEE );
			}

			/* Continue cure affect.*/

			// AFF_CONTINUE_CURE ®ÄªG¨C round ¦^¦å ( bis cure critical)
			if ( IS_AFFECTED( ch, AFF_CONTINUE_CURE ) && !IS_AFFECTED( ch, AFF_NO_HEAL ) )
					ch->hit = UMIN( ch->max_hit, ch->hit + ch->level * 3 );

			// «ùÄòªvÀø³Nªº®ÄªG¨C round ¦^¦å ( continous heal ) 2022/04/08
			if( is_affected( ch, gsn_cont_heal ) && !IS_AFFECTED( ch, AFF_NO_HEAL ) ){
				if ( affected_value(ch, gsn_cont_heal, APPLY_LEVEL ) > 0 ){
					int ContHeSklv = get_skill_level(ch,gsn_cont_heal); // ¦³§Þ¯àµ¥¯ÅªÌ±j«×¸û°ª
					int ContHeAff = affected_value(ch, gsn_cont_heal, APPLY_LEVEL );
					if( !IS_NPC(ch) )
						ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * ( 9 + ContHeAff*3 + ContHeSklv ) )/1000 );
					else
						ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * ( 10 + ContHeAff*2 ) )/1000 );
				}
			}
			// blood thirsty buff ªº APPLY_SAVING_LIFE ¥Î©ó¤£¦º§P©w, ¨C round ¤Ö 1 ª½¨ì 0 ®ø¥¢ add at 2020/10/16
			if ( is_affected( ch, gsn_blood_thirsty) && affected_value(ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) )
			{
				if(affected_value(ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 1){
					AFFECT_DATA af;
					af.type      = gsn_blood_thirsty;
					af.duration  = 0;
					af.location  = APPLY_SAVING_LIFE;
					af.modifier  = -1;
					af.bitvector = AFF_NO_HEAL;
					affect_join( ch, &af );
				}else{
					// ·s¼Wµ²§ôblood thirsty ®É¦^¦å¤Î±Ô­z 2021/12/22
					send_to_char("ªÅ®ð¤¤ªº¦å¸{¨ýº¡¨¬§Aªº¶Ý¦å±ý±æ¡A§A¸£³U©ñªÅ¤£¦A±þ±oµo¨g¡C\n\r",ch);
					//if(affected_value(ch, gsn_blood_thirsty, APPLY_ADAMAGE) > 0){
					if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) > 0 ){
						ch->hit = UMIN( ch->max_hit ,
										ch->hit + 10 + affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT)/10
												* ( 3 + get_skill_level( ch, gsn_blood_thirsty ) * 2 ) ); 
					}
					ch->wait = 20;
					affect_strip ( ch, gsn_blood_thirsty);
					// blood stun ¤¤²¾°£ berserk ®ÄªG 2021/12/26
					if ( is_affected ( ch, gsn_berserk ) )
						affect_strip ( ch, gsn_berserk );
				}
			}

			// sun flash debuff ¨C round hr ¦^´_§P©w, ³Ì¦h 3 round ®ø¥¢ 2021/11/03
			if ( is_affected( ch, gsn_sun_flash) )
			{
				if(affected_value(ch, gsn_sun_flash, APPLY_HITROLL) <= -12){
					AFFECT_DATA af;
					af.type      = gsn_sun_flash;
					af.duration  = 0;
					af.location  = APPLY_HITROLL;
					af.modifier  = 12;
					af.bitvector = AFF_BLIND;
					affect_join( ch, &af );
				}else{
					affect_strip ( ch, gsn_sun_flash);
				}
			}

			// sprinkle debuff ¨C round ¦æ°Ê¤O¦^´_§P©w§P©w lv 5 3 round , lv 6 4 round 2022/12/29
			if ( is_affected( ch, gsn_sprinkle_slash) )
			{
				if(affected_value(ch, gsn_sprinkle_slash, APPLY_ACTION_ABILITY) < 0){
					AFFECT_DATA af;
					af.type      = gsn_sprinkle_slash;
					af.duration  = 0;
					af.location  = APPLY_ACTION_ABILITY;
					af.modifier  = 9 - affected_value(ch, gsn_sprinkle_slash, APPLY_ACTION_ABILITY)/4;
					af.bitvector = AFF_HOLD;
					affect_join( ch, &af );
				}else{
					affect_strip ( ch, gsn_sprinkle_slash);
				}
				if( is_affected( ch, gsn_sprinkle_slash) && affected_value(ch, gsn_sprinkle_slash, APPLY_ACTION_ABILITY) >= 0 )
					affect_strip ( ch, gsn_sprinkle_slash);
			}


			// paralyze debuff ¨C round dc ¦^´_§P©w , ³Ì¦h 4 round Âk 0 2022/06/14
			if ( is_affected( ch, gsn_paralyze) )
			{
				if( affected_value(ch, gsn_paralyze, APPLY_DODGE) > 0 ){
					AFFECT_DATA af;
					af.type      = gsn_paralyze;
					af.duration  = 0;
					af.location  = APPLY_DODGE;
					af.modifier  = 0 - 50 - affected_value(ch, gsn_paralyze, APPLY_DODGE)/3;
					af.bitvector = AFF_PARALYZED;
					affect_join( ch, &af );
					// ·í buff ªº dc 0 ¥H¤U®É, ²¾°£ buff  , ­«·s¥[¤J buff ¨Ï¨¤¦âµu®É¶¡¤º§K¬Ì³Â·ô
					if( affected_value(ch, gsn_paralyze, APPLY_DODGE) <= 0 ){
						affect_strip( ch, gsn_paralyze );
						AFFECT_DATA af;
						af.type      = gsn_paralyze;
						af.duration  = 0;
						af.location  = APPLY_MAGIC_IMMUNE;
						af.modifier  = 0;
						af.bitvector = 0;
						affect_join( ch, &af );
					}
				}
			}

			// Às®§ debuff ¨C round ­°§C breath ³y¦¨ªº¶Ë®`¥[¦¨­È 2022/04/16
			if ( is_affected( ch, gsn_acid_breath) )
			{
				if( affected_value(ch, gsn_acid_breath, APPLY_DAMAGE_COUNT) > 0 ){
					AFFECT_DATA af;
					af.type      = gsn_acid_breath;
					af.duration  = 0;
					af.location  = APPLY_DAMAGE_COUNT;
					af.modifier  = 0 - 1 - affected_value(ch, gsn_acid_breath, APPLY_DAMAGE_COUNT)/25;
					af.bitvector = 0;
					affect_join( ch, &af );
				}else{
					affect_strip ( ch, gsn_acid_breath );
				}
			}
			if ( is_affected( ch, gsn_fire_breath) )
			{
				if( affected_value(ch, gsn_fire_breath, APPLY_DAMAGE_COUNT) > 0 ){
					AFFECT_DATA af;
					af.type      = gsn_fire_breath;
					af.duration  = 0;
					af.location  = APPLY_DAMAGE_COUNT;
					af.modifier  = 0 - 1 - affected_value(ch, gsn_fire_breath, APPLY_DAMAGE_COUNT)/25;
					af.bitvector = 0;
					affect_join( ch, &af );
				}else{
					affect_strip ( ch, gsn_fire_breath );
				}
			}
			if ( is_affected( ch, gsn_frost_breath) )
			{
				if( affected_value(ch, gsn_frost_breath, APPLY_DAMAGE_COUNT) > 0 ){
					AFFECT_DATA af;
					af.type      = gsn_frost_breath;
					af.duration  = 0;
					af.location  = APPLY_DAMAGE_COUNT;
					af.modifier  = 0 - 1 - affected_value(ch, gsn_frost_breath, APPLY_DAMAGE_COUNT)/25;
					af.bitvector = 0;
					affect_join( ch, &af );
				}else{
					affect_strip ( ch, gsn_frost_breath );
				}
			}
			if ( is_affected( ch, gsn_lightning_breath) )
			{
				if( affected_value(ch, gsn_lightning_breath, APPLY_DAMAGE_COUNT) > 0 ){
					AFFECT_DATA af;
					af.type      = gsn_lightning_breath;
					af.duration  = 0;
					af.location  = APPLY_DAMAGE_COUNT;
					af.modifier  = 0 - 1 - affected_value(ch, gsn_lightning_breath, APPLY_DAMAGE_COUNT)/25;
					af.bitvector = 0;
					affect_join( ch, &af );
				}else{
					affect_strip ( ch, gsn_lightning_breath );
				}
			}

			// vampiric bite ¥X¦å®ÄªG
			if( IS_AFFECTED( ch, AFF_VAMP_BITE ) ){
				if( affected_value(ch, gsn_vampiric_bite, APPLY_DAMAGE_COUNT) > 0 ){
					ch->hit -= affected_value(ch, gsn_vampiric_bite, APPLY_DAMAGE_COUNT);
				}
				ch->hit -= ch->level * 5;
				// ¤£·|³Q¬y¦å­P¦º
				if( ch->hit <= 0 )
					ch->hit = 1;
			}

			/* 
				¬rÅ§­×§ï¬°¨C¤U§ðÀ»Ä²µo°l¥[¶Ë®` , dot ³¡¤À»P poison ¬Û¦P 2021/11/04
				if(is_affected(ch, gsn_venomous_strike)){
					VenomousHurt(ch);
				}
			*/

			/*  ­×§ï adore ®ÄªG 2022/11/06
				// ±a¦³¼W¥[³Ì¤j hp ªº regeneracy buff ®É, ¨C round hp ¦^¼W¥[ªº­È,ª½¨ìº¡¦å«á²¾°£buff add at 2020/10/16
				if (   is_affected( ch, gsn_regeneracy)
					&& affected_value(ch, gsn_regeneracy, APPLY_HIT) > 0
					&& !IS_AFFECTED( ch, AFF_NO_HEAL ) )
				{
					ch->hit = UMIN( ch->max_hit, ch->hit + 10 + affected_value(ch, gsn_regeneracy, APPLY_HIT)/2);
					if( ch->hit >= ch->max_hit)
						affect_strip( ch, gsn_regeneracy);
				}
			*/

			if( ch != victim
				&& !IS_AFFECTED(ch, AFF_NO_HEAL ) // no heal wouldn't regeneracy add at 2018.11.20
				// ²¾°£ invoke §P©w , ¤¤¬r¤U§ï¬°§P©w¦Û¨­¬r¯À¬O§_¬°­t­È¦¨¥ß 2021/11/04
				//&& ((is_affected( ch, gsn_invoke ) && get_skill_level(ch, gsn_invoke) > 5) || !IS_AFFECTED(ch, AFF_POISON) )
				&& (     IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
					||   IS_SET( ch->special, NO_POISON )
					|| (!IS_AFFECTED(ch, AFF_POISON))
					|| ( IS_AFFECTED(ch, AFF_POISON) && (NEW_GET_SVS(ch,MAGIC_POISON) <= ch->level) ) ) // poison °ò¥»­È¬°¨¤¦â level
				&& (     IS_SET( race_table[ ch->race ].race_abilities, RACE_REGENERACY )
					|| ( IS_NPC( ch ) && IS_SET(ch->act, ACT_REGENERACY) )
					|| (!IS_NPC( ch ) && ch->pcdata->learned[gsn_regeneracy] > 0) ) )
			{
				if ( IS_NPC( ch ) )
				{
					ch->hit += ch->level * 10;
					if ( ch->hit > ch->max_hit )
						ch->hit = ch->max_hit;
				}
				else if ( ch != ch->fighting )
				{
					int factor = 100;
					int nCurrCon = get_curr_con( ch );
					int sklv = get_skill_level(ch, gsn_regeneracy);
					int base = get_skill_percent(ch, gsn_regeneracy) + sklv * 2;
					int difflv = victim->level - ( ch->level > 36 ? 36 : ch->level ) + UMAX(0, sklv - 4) * 2;
					int nRegSklv = 0; // add at 2022/04/07
					if( IS_SET( race_table[ ch->race ].race_abilities, RACE_REGENERACY )){
						base += 10 + nRaceSklv * (nRaceSklv + 4);
						// ´£°ª inborn reg sklv ¹ï con ±`¼Æªº¼vÅT 2022/04/07
						//if(nRaceSklv > 4) nCurrCon += (nRaceSklv - 4);
						if(nRaceSklv > 3) nCurrCon += (nRaceSklv - 3);
						if(nCurrCon > 25) nCurrCon = 25;
						// inborn reg ¨ü³Ì¤j hp ¼vÅT 2022/04/07
						nRegSklv += nRaceSklv * 3;
					}
					if( difflv > 15 )        factor = 200;
					else if ( difflv > 10 )  factor = 180;
					else if ( difflv > 7 )   factor = 160;
					else if ( difflv > 4 )   factor = 140;
					else if ( difflv > -2 )  factor = 100;
					else if ( difflv > -8 )  factor = 80;
					else if ( difflv > -10 ) factor = 50;
					else if ( difflv > -15 ) factor = 30;
					else factor = 10;
					// if ( ch->level <= victim->level ){
					if( factor ) factor += sklv*2;
					if( sklv > 4 )
					{
						ch->move += get_curr_con( ch ) * (sklv);
						if( ch->move > ch->max_move )
							ch->move = ch->max_move;
					}
					//regeneracy wouldn't occur if target's hp does not below max hp
					//modified by Amethyst
					if( ch->hit < ch->max_hit )
					{
						// nRaceSklv ªº®ÄªG¬ù¬° sklv ªº 30%  2022/04/07
						nRegSklv = nRegSklv + sklv * 10;

						if( ( !IS_NPC(ch) && !IS_NPC(victim) ) ||
							( !IS_NPC( victim ) && victim->pcdata->clanpk 
							&& IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
							ch->hit += ( ( UMAX( 1, ( nCurrCon - 10 ) ) * base * factor ) / 100 / 
										( ( victim->level - ch->level > 5 ) ? 4 : 5))/8
										// ¥[¤J inborn reg sklv ¼vÅT³Ì¤j hp ¦^¦å®ÄªG 2022/04/07
										// hp 10000 ®É sklv 6 ¦³ 300 ÂI¦^¦å 
										// hp 10000 ®É inborn 6 ¦³ 90 ÂI¦^¦å
										//+ ch->max_hit * 0.002 * sklv;
										+ ( ch->max_hit * 2 * nRegSklv )/10000;
						else
							ch->hit += ( UMAX( 1, ( nCurrCon - 10 ) ) * base * factor ) / 100 / 
										( ( victim->level - ch->level > 5 ) ? 4 : 5 )
										//+ ch->max_hit * 0.005 * sklv;
										+ ( ch->max_hit * 5 * nRegSklv )/10000;
						if ( ch->hit > ch->max_hit )
							ch->hit = ch->max_hit;
					}
				// }
				}
			}

			QMARK *qmark = NULL;
			if( ch->wait == 0 )
			{
				/* Added by Razgriz	20050909
				* deal with qmark -- earthquake_stun
				*/
				// ²¾¥X if ¥H§P©w¨ä¥Lªº qmark 2023/01/12
				//QMARK *qmark = NULL;
				if( ( qmark = is_qmark ( ch, "earthquake_stun" )  ) != NULL )
					del_qmark( qmark );

				mprog_hitprcnt_trigger( ch, victim );
				mprog_fight_trigger( ch, victim );
			}

			// §ðÀ»ªº¥Ø¼Ð³Q rescue «áµLªk¨Ï¥Î change ªº qmark §P©w 2023/01/12
			if( ( qmark = is_qmark ( ch, "skill_rescue_round" )  ) != NULL ){
				if( qmark->v0 > 0 )
					qmark->v0 -= 1;
			}

			if( !ch->fighting
				|| victim->deleted
				|| ch->in_room != victim->in_room
				|| IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
				continue;

			/*	²¾¤J multi_hit §@§P©w 2022/10/31

				// ¨Ï mob ¤£§@¥X´¶³q§ðÀ»ªº qmark  2020/05/28
				if( IS_NPC(ch) && is_qmark(ch,"disable_round_hit" ) )
					continue;
			*/

			if ( IS_AWAKE( ch )
				&& !victim->deleted
				&& (victim = ch->fighting) )
			{   //victim may change
				/* Ok here we test for switch if victim is charmed */
				if ( ( IS_AFFECTED( victim, AFF_CHARM )
						|| IS_AFFECTED( victim, AFF_SUMMONED )
						|| (IS_NPC(victim) && victim->clan) )
					&& victim->master
					&& victim->in_room
					&& victim->in_room == victim->master->in_room
					&& victim->master != ch
					&& number_percent() > 30 )
					//  && number_percent() > 50 && can_see( ch, victim->master ) )
				{
					stop_fighting( ch, FALSE );
					multi_hit( ch, victim->master, TYPE_UNDEFINED );
				}
				if( IS_AFFECTED(ch, AFF_PARALYZED) )
					continue;
				if( !IS_NPC(ch) && ch->position == POS_RESTING )
					continue;
				if( !check_blind(ch) && get_skill_percent(ch, gsn_blind_fight) <= number_percent() )
					continue;
				if( !ch->fighting || ch->fighting->deleted )
					continue;
				victim = check_mob_ai(ch);
				if( victim )
					multi_hit( ch, victim, TYPE_UNDEFINED );
				continue;
			}
			else
				stop_fighting( ch, FALSE );

			continue;
		}
		else
			update_pos(ch);

		


		/*
		IF ( IS_AFFECTED( ch, AFF_BLIND )
			|| !IS_AWAKE( ch )
			|| ( IS_NPC( ch )
			&& ch->pIndexData->pShop ) )
			continue;
		*/

		if( !IS_AWAKE(ch) )
			continue;

		/*
		* Ok. So ch is not fighting anyone.
		* Is there a fight going on?
		* 
		*
		*/

		mobfighting = FALSE;

		// group ¾Ô°«§P©w , ¶]¨ì³o¥Nªí ch ¨S¦³¦b¾Ô°«¤¤
		for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
		{
			if ( (ch == rch)
				|| rch->deleted
				|| !IS_AWAKE( rch )
				|| !( rch->fighting ) )
				continue;

			// ¦pªG¦P©Ð¶¡¤¤ªº group ¦¨­û(rch)¦b¾Ô°«, «h¸õ¥Xµ{§Ç
			if ( is_same_group( ch, rch ) )
				break;

			// ¼W¥[ qmark ªý¾× mob ÀH¾÷¤¶¤J¾Ô°« , ¨Ã¥[¤J pet ¤£·|¤¶¤JÀH¾÷¾Ô°«ªº³]©w 2022/11/24
			if( is_qmark( ch, "enable_mob_assist") )
			{
				if (   IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CHARM ) && !IS_AFFECTED( ch, AFF_SUMMONED ) && !ch->clan && !IS_SET( ch->act, ACT_PET )
					&& IS_NPC( rch ) && !IS_AFFECTED( rch, AFF_CHARM ) && !IS_AFFECTED ( rch, AFF_SUMMONED ) && !rch->clan && !IS_SET( rch->act, ACT_PET ) )
				{
					mobfighting = TRUE;
					break;
				}
			}
		}

		// add by coco to try fix config + assist bug 0620
		//	if ( !victim || !rch )
		//		continue;

		if ( !rch || !(rch->fighting) )
			continue;

		// ±N victim ³]¬° rch->fighting ( rch ªº¾Ô°«¥Ø¼Ð )
		victim = rch->fighting;

		/*
		* Now that someone is fighting, consider fighting another pc
		* or not at all.
		* mobfighting ¬° TRUE ªº±¡ªp¤U, ch ( ¦¹³B ch ¥²¬° npc )«D¾Ô°«¤¤
		* ¦pªG¦P room ¦³¨ä¥L npc (rch) ¥¿µo¥Í¾Ô°«, «h ch ·|¤¶¤J¥Ø¼Ðµ¥¯Å®t 5 ¥H¤ºªº¾Ô°«
		*/
		if ( mobfighting )
		{
			CHAR_DATA *vch;
			int number;

			number = 0;
			for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
			{
				if (   can_see( ch, vch )
					&& is_same_group( vch, victim )
					&& number_range( 0, number ) == 0 )
				{
					victim = vch;
					number++;
				}
			}

			if( ( rch->pIndexData != ch->pIndexData && number_bits( 3 ) != 0 )
				|| abs( victim->level - ch->level ) > 5 )
				//|| ( IS_GOOD( ch ) && IS_GOOD( victim ) ))  ²¾°£°}Àç§P©w 2022/11/24
				continue;
		}

		// plr_assist ¥[¤J¥u¹ï pc §P©w 2022/11/24
		//if( ( IS_NPC(ch) && !is_same_group( ch, victim ) ) || IS_SET(ch->act, PLR_ASSIST) )
		if(    (  IS_NPC(ch) && !is_same_group( ch, victim ) )
			|| ( !IS_NPC(ch) && IS_SET(ch->act, PLR_ASSIST) ) )
		{
			if( !ch->fighting ){
				set_fighting( ch, victim );
				ch->position = POS_FIGHTING;
			}
			multi_hit( ch, victim, TYPE_UNDEFINED );
		}
	}
	for( ch = char_list; ch; ch = ch->next )
		if(ch && !IS_NPC(ch) && ch->fighting)
			show_vs_graphy(ch, ch->fighting);
	return;
}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{

	if( !IS_NPC(victim) 
		&& victim->hit > 0 
		&& victim->position == POS_RESTING ) 
		return;
  
	if ( victim->hit > 0 )
	{
		if ( victim->position <= POS_STUNNED )
			victim->position = POS_STANDING;
		else if( victim->position == POS_PARALYZED )
			victim->position = POS_STANDING;
		else if( victim->position == POS_FIGHTING 
				&& ( !victim->fighting || victim->fighting->in_room != victim->in_room ))
			victim->position = POS_STANDING;
		return;
	}

	if ( IS_NPC( victim ) || victim->hit <= -21 )
	{
		victim->position = POS_DEAD;
		return;
	}

	if ( victim->hit <= -6 )
		victim->position = POS_MORTAL;
	else if ( victim->hit <= -3 )
		victim->position = POS_INCAP;
	else
		victim->position = POS_STUNNED;

	//Modified by Keric for hp = -20 will not autoupdate position. 2005/2/14 
	if ( victim->hit < 0 )
		victim->hit -= 3;

	return;
}

/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{

	//char buf [ MAX_STRING_LENGTH ];
	if(!victim || victim->deleted)
		return;
	if( ch->fighting )
	{
		/*  remove at 2020/05/31 ÁY´î¤£¥²­nªº bug log
			bug( "Set_fighting: already fighting", 0 );
			bugf(  "...%s(%s) attacking %s(%s) at %d", 
			ch->short_descr, ch->name, victim->short_descr, victim->name,
			(victim->in_room) ? victim->in_room->vnum : 0);
       */
		return;
	}

	if( IS_AFFECTED( ch, AFF_SLEEP ) )
		affect_strip( ch, gsn_sleep );

	ch->fighting = victim;
	if( IS_AFFECTED(ch, AFF_PARALYZED) ) return;
	if( !IS_NPC(ch) && ch->position == POS_RESTING ) return;
	if( !check_blind(ch) && get_skill_percent(ch, gsn_blind_fight) <= number_percent() ) return;
	if( !ch->fighting || ch->fighting->deleted ) return;
	ch->position = POS_FIGHTING;

	// ¨Ï¶}¾Ôªº²Ä¤@¦^¦X¯àª½°õ¦æ obj_fight prog 2022/05/08
	CheckObjFight(ch);
  
	if( IS_AFFECTED(victim, AFF_PARALYZED) ) return;
	if( !IS_NPC(victim) && victim->position == POS_RESTING ) return;
	//Marked by Razgriz 20060810
	//if( !check_blind(victim) && get_skill_percent(victim, gsn_blind_fight) <= number_percent() ) return;
	victim->position = POS_FIGHTING;

	return;
}



/* Mark by Razgriz	20050909
 * Stop fights.
 *
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
  CHAR_DATA *fch, *vch;

  REMOVE_BIT(ch->affected_by, AFF_CAST_ACC);
  for ( fch = char_list; fch; fch = fch->next )
  {
    if ( fch == ch )
    {
      fch->fighting = NULL;
      fch->position = POS_STANDING;
      if ( is_affected( fch, gsn_berserk ) )
        affect_strip( fch, gsn_berserk );
      update_pos( fch );
    }
    else if ( fBoth && fch->fighting == ch && !fch->deleted )
    {
      for( vch = fch->in_room->people; vch; vch = vch->next_in_room )
        if ( vch->fighting == fch && !vch->deleted )
        {
          fch->fighting = vch;
          return;
        }
      fch->fighting = NULL;
      fch->position = POS_STANDING;
      if ( is_affected( fch, gsn_berserk ) )
        affect_strip( fch, gsn_berserk );
      update_pos( fch );
    }
  }
  return;
}
*/

/*
 * Rewrite by Razgriz	20050909
 * Eliminate logical bugs in old version
 * and add handle for qmark -- earthquake_stun
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
	CHAR_DATA *tch = NULL, *fch, *vch;
	bool fight_change;
	QMARK *qmark = NULL;

	REMOVE_BIT ( ch->affected_by, AFF_CAST_ACC );
	for ( fch = char_list; fch; fch = fch->next )
	{
		if ( fch == ch )
			tch = ch;
		else if ( fBoth && fch->fighting == ch && !fch->deleted )
		{
			fight_change = FALSE;
			for( vch = fch->in_room->people; vch; vch = vch->next_in_room )
			{
				if ( vch->fighting == fch && !vch->deleted && vch != ch ) {
					fch->fighting = vch;
					fight_change = TRUE;
					fch->attacked = NULL;                       //Added by Razgriz 20060822
					break;
				}
			}

			if ( !fight_change )
			{
					fch->fighting = NULL;
					fch->position = POS_STANDING;
					fch->attacked = NULL;                       //Added by Razgriz 20060822
					if ( is_affected ( fch, gsn_berserk ) )
						affect_strip ( fch, gsn_berserk );
					if( is_affected( fch, gsn_blood_thirsty) )  // blood thirsty °±¾Ô«á§P©w 2022/02/21
						BloodThirstyStopCheck(fch);
					//deal with qmark -- earthquake_stun
					if( ( qmark = is_qmark ( fch, "earthquake_stun" )  ) != NULL ) {
						del_qmark( qmark );
						fch->wait = 0;//trade-off
					}
					// ²¾°£ rescue µLªk change ªº qmark 2023/01/12
					if( ( qmark = is_qmark ( fch, "skill_rescue_round" )  ) != NULL ) {
						del_qmark( qmark );
					}
					update_pos ( fch );
			}
		}//end else if
	}//end for

	if ( tch )
	{
		tch->fighting = NULL;
		tch->position = POS_STANDING;
		tch->attacked = NULL;                                   //Added by Razgriz 20060822
		if ( is_affected ( tch, gsn_berserk ) )
			affect_strip ( tch, gsn_berserk );
		if( is_affected( tch, gsn_blood_thirsty) )             // blood thirsty °±¾Ô«á§P©w 2022/02/21
			BloodThirstyStopCheck(tch);
		//deal with qmark -- earthquake_stun
		if( ( qmark = is_qmark ( tch, "earthquake_stun" )  ) != NULL ) {
			del_qmark( qmark );
			tch->wait = 0;//trade-off
		}
		// ²¾°£ rescue µLªk change ªº qmark 2023/01/12
		if( ( qmark = is_qmark ( tch, "skill_rescue_round" )  ) != NULL ) {
			del_qmark( qmark );
		}
		update_pos ( tch );
	}

	return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *killer, CHAR_DATA *ch )
{
	OBJ_DATA *corpse;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	CHAR_DATA *horse;
	OBJ_DATA *obj_horse;
	OBJ_DATA *obj_horse_next;
	// ROOM_INDEX_DATA *location;
	char *name;
	char buf [ BUF_STRING_LENGTH ];
	bool obj_corpse = FALSE; // ±N pc corpse °òÂ¦ timer ­×§ï¬° 2 tick , Á×§K¤j¶q´c·N°ï«Í 2022/11/02

	if ( ( killer == ch ) && ( ch->attacked ) )
		killer = ch->attacked;

	if ( IS_NPC( ch ) ) // npc ³Q±þ
	{
		/*
		* This longwinded corpse creation routine comes about because
		* we dont want anything created AFTER a corpse to be placed  
		* INSIDE a corpse.  This had caused crashes from obj_update()
		* in extract_obj() when the updating list got shifted from
		* object_list to obj_free.      --- Thelonius (Monk)
		*/
		if ( is_qmark( ch, "spell_finger_of_death" ) )
			return;

		name = ch->name;
		corpse = create_object( get_obj_index( OBJ_VNUM_CORPSE_NPC ), 0 );
		corpse->timer = number_range( 8, 14 );
		sprintf( buf, corpse->name, name );
		free_string( corpse->name );
		corpse->name = str_dup( buf );
	}
	else // pc ³Q±þ
	{
		// npc ³sÄò±þ¦º pc ®É·|­p¼Æ, ­p¼Æ¤Ó°ª·|¦³­°§C¼Æ­ÈªºÃg»@
		if( IS_NPC( killer ) && IS_SET( killer->act , ACT_CORPSE_COUNT ) )
		{
			char buf_q[MAX_INPUT_LENGTH];
			sprintf(buf_q, "corpse_count_%d", killer->pIndexData->vnum);
			if( is_qmark( ch, buf_q))
			{
				sprintf( buf, "%s ", ch->name );
				strcat( buf, buf_q );
				strcat(buf, " v0 + 1");
				do_mod_qmark( ch, buf );
				sprintf( buf, "%s ", ch->name );
				strcat( buf, buf_q );
				strcat(buf, " timer + 60");
				do_mod_qmark( ch, buf );
			}
			else
			{
				sprintf( buf, "%s ", ch->name );
				strcat(buf, buf_q);
				strcat( buf, " 50 1 0 0 0 1" );
				do_add_qmark( ch, buf );
			}
		}
		name = ch->name;
		corpse = create_object( get_obj_index( OBJ_VNUM_CORPSE_PC ), 0 );
		corpse->timer = 100;
	}

	corpse->value[0] = 0;
	corpse->value[1] = 0;
	corpse->value[2] = 0;
	corpse->value[3] = 0;

	if ( ch->gold > 0 )
	{
		OBJ_DATA *coins;
		int gold = ch->gold;

		// ¨ú®ø npc gold ·|¨üµ¥¯Å®t¼vÅT 2022/11/02
		/* if( !IS_NPC(killer) && IS_NPC(ch) && killer->class > BARBARIAN)
		{
			int difflv = killer->level - ch->level;
			if( difflv < 6 ){gold = gold;}
			else if( difflv < 8 ){gold = gold * 4 / 5;}
			else if( difflv < 10 ){gold = gold * 3 / 5;}
			else if( difflv < 12 ){gold = gold * 2 / 5;}
			else{gold /= 10;}
		} */

		if( !IS_NPC(ch) )
			obj_corpse = TRUE;

		coins = create_money( gold );
		obj_to_obj( coins, corpse );
		ch->gold = 0;
	}

	sprintf( buf, corpse->name, name );
	free_string( corpse->name );
	corpse->name = str_dup( buf );

	if ( IS_NPC( ch ) )
		sprintf( buf, corpse->short_descr, ch->short_descr );
	else
		sprintf( buf, corpse->short_descr, name );

	free_string( corpse->short_descr );
	corpse->short_descr = str_dup( buf );

	if ( !killer )
		killer = ch;

	sprintf( buf, corpse->description, ch->short_descr, killer->short_descr );
	free_string( corpse->description );
	corpse->description = str_dup( buf );
	corpse->level = ch->level;

	for ( obj = ch->carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;

		if ( obj->deleted )
			continue;

		obj_from_char( obj );

		/*
		* Remove item inventories from all corpses.
		* Includes licenses to kill
		*/
		if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) || ( IS_NPC( ch ) && ch->pIndexData->pShop ) )
		{
			extract_obj( obj );
		}
		else
		{
			if( IS_SET( obj->wear_flags , ITEM_WEAR_RIDE ) && !IS_NPC(ch) && get_skill_level(ch,gsn_ride) < 4 )
			{
				if (  get_mob_index(obj->pIndexData->horse)  == NULL )
				{
					bug( "Ride - Bad mob vnum: vnum %d.", obj->pIndexData->vnum );
					return;
				}

				horse = create_mobile( get_mob_index( obj->pIndexData->horse) );

				for( obj_horse = obj->contains; obj_horse ; obj_horse = obj_horse_next )
				{
					obj_horse_next = obj_horse->next_content;
					obj_from_obj( obj_horse );
					obj_to_char( obj_horse, horse );
				}

				extract_obj( obj );
				char_to_room( horse, ch->in_room);
				SET_BIT( horse->act, ACT_HORSE );
				SET_BIT( horse->act, ACT_SENTINEL );
				add_follower( horse, ch );
				add_rember( horse , ch );

				continue;
			}

			// pc make corpse ®É¦³ 50% ¾÷²vª««~¦b¦º«á¤£±¼¸¨ 2022/11/02
			if( !IS_NPC(ch) && ( IS_SET(obj->extra_flags, ITEM_PERSONAL ) || number_percent( ) > 50 ) ){
				obj_to_char(obj, ch);
			}else{
				if( !IS_NPC(ch) && !obj_corpse )
					obj_corpse = TRUE;
				obj_to_obj( obj, corpse ); /* eq upon death - Kahn */
			}
		}
	}

	if( !IS_NPC(ch) ){
		if( obj_corpse ){
			// ¦º¤`«á§ä¦^«ÍÅé«eªº¤@¬q®É¶¡¤ºµLªk¨Ï¥Î backup 2022/10/10
			// ¦]¬° raw_kill ¦b make_corpse «á·|²¾°£ fDeath ªº qmark , ©Ò¥H¦¹³B³]¬° del_qmark «á¦A¥[¤J
			if( is_qmark(ch, "death_stop_backup") )
				do_del_qmark(ch, "self death_stop_backup");
			do_add_qmark(ch, "self death_stop_backup 24 0 0 0 0 0");
			corpse->timer = 100;
		}else{
			corpse->timer = 2;
		}
	}

	obj_to_room( corpse, ch->in_room );

	return;
}


/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry( CHAR_DATA *ch )
{
  ROOM_INDEX_DATA *was_in_room;
  char      *msg;
  int        vnum;
  int        door;

  vnum = 0;
  if(!is_chinese(ch, NULL) )
  {
    switch ( number_bits( 4 ) )
    {
      default: msg  = "You hear $n's death cry.";       break;
      case  0: msg  = "$n hits the ground ... DEAD.";     break;
      case  1: msg  = "$n splatters blood on your armor.";    break;
      case  2: msg  = "You smell $n's sphincter releasing in death.";
           vnum = OBJ_VNUM_FINAL_TURD;        break;
      case  3: msg  = "$n's severed head plops on the ground.";
           vnum = OBJ_VNUM_SEVERED_HEAD;        break;
      case  4: msg  = "$n's heart is torn from $s chest.";
           vnum = OBJ_VNUM_TORN_HEART;        break;
      case  5: msg  = "$n's arm is sliced from $s dead body.";
           vnum = OBJ_VNUM_SLICED_ARM;        break;
      case  6: msg  = "$n's leg is sliced from $s dead body.";
           vnum = OBJ_VNUM_SLICED_LEG;        break;
    }
  }
  else
  {
    switch ( number_bits( 4 ) )
    {
      default: msg  = "§AÅ¥¨£ $n Á{¦º«eªººG¥s.";        break;
      case  0: msg  = "$n ­Ë¦b¦a¤W¡A¦R¥X³Ì«á¤@¤f®ð...¦º¤F.";      break;
      case  1: msg  = "$n ¼Q¥XÂA¦å¡AÂq±o§Aº¡¨­.";   break;
      case  2: msg  = "§A»D¨ì¤@ªÑ´c¯ä.";
           vnum = OBJ_VNUM_FINAL_TURD;        break;
      case  3: msg  = "$n ªºÀY³Q¬å¤U¡Aºu¸¨¦b¦a.";
           vnum = OBJ_VNUM_SEVERED_HEAD;        break;
      case  4: msg  = "$n ªº¤ßÅ¦³Q«õ¤F¥X¨Ó.";
           vnum = OBJ_VNUM_TORN_HEART;        break;
      case  5: msg  = "$n ªº¤âÁu³Q¬å¤U¡A±¼¸¨¦b¦a¤W.";
           vnum = OBJ_VNUM_SLICED_ARM;        break;
      case  6: msg  = "$n ªº»L³Q¬å¤U¡A±¼¸¨¦b¦a¤W.";
           vnum = OBJ_VNUM_SLICED_LEG;        break;
    }
  }

  act( msg, ch, NULL, NULL, TO_ROOM );

  if ( vnum != 0 )
  {
    OBJ_DATA *obj;
    char   *name, *cname;
    char    buf [ BUF_STRING_LENGTH ];

    name    = ch->name;
    cname   = ch->short_descr;
    obj   = create_object( get_obj_index( vnum ), 0 );
    obj->timer  = number_range( 4, 7 );

    sprintf( buf, obj->short_descr, cname );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );

    sprintf( buf, obj->name, name );
    free_string( obj->name );
    obj->name = str_dup( buf );

    sprintf( buf, obj->description, cname, name );
    free_string( obj->description );
    obj->description = str_dup( buf );

    if ( IS_AFFECTED( ch, AFF_POISON ) )
      obj->value[3] = 1;

    obj_to_room( obj, ch->in_room );
  }

  if ( IS_NPC( ch ) )
    msg = "§AÅ¥¨£¤@ÁnºG¥s.";
  else
    msg = "§AÅ¥¨£¤@ÁnºG¥s.";

  was_in_room = ch->in_room;
  for ( door = 0; door <= 5; door++ )
  {
    EXIT_DATA *pexit;

    if ( ( pexit = was_in_room->exit[door] )
        && pexit->to_room
        && pexit->to_room != was_in_room )
    {
      ch->in_room = pexit->to_room;
      act( msg, ch, NULL, NULL, TO_ROOM );
    }
  }
  ch->in_room = was_in_room;

  return;
}

void init_char_data( CHAR_DATA *ch)
{
	int i = 0;
	int nRaceSklv = get_skill_level(ch, gsn_inborn_ability);

	ch->armor = 100;
	ch->dodge = 0;
	if( ch->class > BARBARIAN ){
		ch->armor  += race_table[ch->race]. nature_ac;
		ch->dodge  += race_table[ch->race]. nature_dodge;
	}
	ch->position   = POS_RESTING;
	if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_VAMPIRE 
		|| ch->class < WIZARD 
		|| ch->level < 31 ) )
	{
		ch->hit  = ((ch->max_hit*10)/ 100);
		// 2023/01/14
		//gain_condition( ch, COND_THIRST, 100 );
	}else{
		ch->hit    = UMAX( 1, ch->hit  );
	}
	ch->mana     = UMAX( 1, ch->mana );
	ch->move     = UMAX( 1, ch->move );
	ch->hitroll  = 0;
	ch->damroll  = 0;

	if(IS_SET(race_table[ ch->race ].race_abilities, RACE_FLY))
	{
		ch->dodge -= nRaceSklv * 2 + (nRaceSklv * nRaceSklv) / 2;
		ch->dodge += race_table[ ch->race ].size * 5;
	}

	switch(ch->level)
	{
		default:
			ch->pp = 0;
			ch->pdr = 0;
			ch->mdr = 0;
			break;
		case 40:
			ch->pp = 3;
			ch->pdr = 4;
			ch->mdr = 4;
			break;
		case 41:
			ch->pp = 7;
			ch->pdr = 12;
			ch->mdr = 12;
			break;
		case 42:
			ch->pp = 10;
			ch->pdr = 15;
			ch->mdr = 15;
			break;
		case 43:
		case 44:
		case 45:
			ch->pp = 13;
			ch->pdr = 25;
			ch->mdr = 25;
			break;
	};
	ch->pdr += GetArmorMasteryPdrBonus(ch);
	ch->apdr = 0;
	ch->amdr = 0;
	ch->pad = 0;
	ch->mad = 0;

	for( i = 0 ; i < MAX_SAVING ; ++ i )
	{
		ch->saving_throw[i] = 0;
	}

	ch->mod_str = 0;
	ch->mod_int = 0;
	ch->mod_wis = 0;
	ch->mod_dex = 0;
	ch->mod_con = 0;
	ch->mod_size = 0; // Åé«¬ 2022/12/23

	if(IS_SET(race_table[ ch->race ].race_abilities, RACE_DETECT_MAGIC)){
		ch->pp += nRaceSklv * nRaceSklv / 5;
		ch->amdr += nRaceSklv * nRaceSklv;
	}
}

bool raw_kill( CHAR_DATA *ch, CHAR_DATA *victim )
{
	AFFECT_DATA *paf;
	CHAR_DATA *horse;
	AFFECT_DATA af;
	CLAN_DATA *cn;
	ROOM_INDEX_DATA *loc;
	OBJ_DATA* obj ,* obj_next;
	OBJ_DATA *obj_horse;
	OBJ_DATA *obj_horse_next;
	int i;
	int vicvnum;
	int tempa, tempb;

	//CHAR_DATA *pOrgCh = ch;
	if( IS_NPC(ch) ) {
		if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
			if(ch->master != NULL) ch = ch->master;
		}
	}

	if ( IS_NPC(victim) && ch != victim ) 
	{
		if( IS_SET(victim->act, ACT_MPROG_DEATH))
			return FALSE;
		SET_BIT(victim->act, ACT_MPROG_DEATH);
		mprog_death_trigger( ch, victim );
	}
	else
	{
		if ( victim->attacked )
			mprog_death_trigger( ch, victim->attacked );
	}

	if( IS_NPC(ch) ) mprog_kill_trigger(ch, victim);

	// obj_kill §P©w 2022/05/08
	CheckObjKill(ch);

	if( !IS_NPC(ch) && !IS_NPC(victim) && ch != victim ) {
		ch->pcdata->pkwin += 1;
		victim->pcdata->pklose += 1;
	}
	stop_fighting( victim, TRUE );
	if ( victim->deleted )//for the 'mppurge self' in mprog_death_trigger condition
	{
		for ( paf = victim->affected; paf; paf = paf->next )
		{
			if ( paf->deleted )
				continue;
			affect_remove( victim, paf );
		}
		victim->pIndexData->killed++;
		kill_table[URANGE( 0, victim->level, MAX_LEVEL-1 )].killed++;
		return TRUE;
	}
	/* death_cry( victim );*/


	death_declare( ch,victim);
	logf_envy("%s is killed by %s.", victim->name, ch->name);
	// pc ¦º¤`°O¿ý¨ì bug log , ¨Ã¥[¤J·í¤U®É¶¡ 2022/12/11
	if( !IS_NPC( victim ) ){
		bugf("[PC Death]: %s is killed by %s. %s", victim->name, ch->name , (char *) ctime( &current_time ) );
	}
	for ( paf = victim->affected; paf; paf = paf->next )
	{
		if ( paf->deleted )
			continue;

		affect_remove( victim, paf );

		// ¥[¤W dispel_magic function ¤¤¡A¦b affect_remove «á¯S§O°w¹ï polymorph ©M flying ªº³]©w
		if(paf->type == gsn_polymorph || paf->type == gsn_morphing )
		{
			if( paf->type == gsn_morphing )
				victim->temp_race = '\0' ; //NULL;
			dm_effect( victim );
		}
		/* ¦º¤`ªºª¬ºA¤U¤£·|¨ü fall_effect ¼vÅT 2023/01/14
		if( paf->type == gsn_flying )
			fall_effect( victim ); */
		// end of additional setting 
	}

	if( !IS_NPC(victim) 
		&& ( ( !IS_NPC(ch) 
				&& victim->in_room 
				&& IS_SET(victim->in_room->room_flags, ROOM_PK) )
			|| ( victim->level < 4 && victim->class < 5 )
			|| victim->pcdata->clanpk ) )
	{
		pk_kill(ch, victim);

		return TRUE; 
	}

	make_corpse( ch, victim );

	// if ( IS_NPC( victim ) || IS_SET( victim->act, PLR_HORSE ) )
	if ( IS_NPC( victim ) )
	{
		if ( ch->class == WARLORD && (victim->level >= ch->level || victim->level > 36 ) && is_affected( ch, gsn_adore ) )
		{
			int sklv = get_skill_level(ch, gsn_adore);
			if( ch->hitroll > 20 - sklv * 3 ) {
				af.type  = gsn_adore;
				if( ch->level > victim->level )
					af.duration  = 2 + sklv / 3;
				else
					af.duration  = 3 + sklv / 2;
				af.location  = APPLY_HITROLL;
				af.bitvector = 0;
				af.modifier  = - 3 + (sklv > 3 ? 1 : 0 ) + (sklv > 5 ? 1 : 0);
				affect_join( ch, &af );
				if( ch->level > victim->level )
					af.duration  = 2 + sklv / 3;
				else
					af.duration  = 3 + sklv / 2;
				af.location  = APPLY_DAMROLL;
				af.bitvector = 0;
				af.modifier  = 5 + sklv * 2 / 3;
				affect_join( ch, &af );
				send_to_char( "³o­Ó¹ï¤â¾Ä¾Ô¤£¾Óªº­^«º²`²`¬M¦b§Aªº¸£®ü¡C\n\r", ch );
				act( "$n²´¤¤ªº¥ú¨~§ó¥[¿K¯P¤F¡I", ch, NULL, NULL, TO_ROOM );
			}
		}
		//calculate clan hire points
		cn  = ch->clan;
		if ( cn && victim->level > ch->level && victim->pIndexData->killed < 4 
			&& IS_SET( victim->act, ACT_AGGRESSIVE ) )
			cn->construct++;
		if ( victim->level > 39 && cn && !victim->pIndexData->pShop && !victim->clan
			&& !IS_SET( victim->act, ACT_PRACTICE ) && victim->pIndexData->vnum != cn->kill[0] )
		{
			vicvnum = victim->pIndexData->vnum;
			tempa = cn->kill[0];
			cn->kill[0] = vicvnum;
			for( i = 0; i < CLAN_KILL - 1; i++ )//remembered the last 10 killed mob
			{
				if ( vicvnum == cn->kill[i+1] )
				{
					cn->kill[i+1] = tempa;
					break;
				}
				tempb = cn->kill[i+1];
				cn->kill[i+1] = tempa;
				tempa = tempb;
			}
			if ( i == CLAN_KILL - 1 ) //not found in cn->kill list, add hire points
			{
				if ( victim->level < 45 )
					cn->hire += 1;
				else if ( victim->level < 50 )
					cn->hire += 3;
				else if ( victim->level < 55 )
					cn->hire += 10;
				else if ( victim->level < 60 )
					cn->hire += 20;
				else if ( victim->level < 65 )
					cn->hire += 40;
				else
					cn->hire += 80;
			}
		}

		if ( victim->clan )
		{
			victim->clan->hire -= ( 10 + UMAX( 0, victim->level - 35 ) * 3
									+ UMAX( 0, victim->level - 38 ) * 5 );
			if ( victim->spec_fun )
				victim->clan->hire -= 15;
			if ( victim->class != 0 )
				victim->clan->hire -= 15;
		}
		extract_char( victim, TRUE );
		return TRUE;
	}

	if ( IS_SET( victim->act, PLR_KILLER ) )
		REMOVE_BIT( victim->act, PLR_KILLER );

	extract_char( victim, FALSE );

	for ( obj = victim->carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;

		if ( obj->deleted )
			continue;
		/*
		 * Remove item inventories from all corpses.
		 * Includes licenses to kill
		 */
		if( IS_SET( obj->wear_flags , ITEM_WEAR_RIDE ) )
		{

			obj_from_char( obj );
			if (  get_mob_index(obj->pIndexData->horse)  == NULL )
			{
				bug( "Ride - Bad mob vnum: vnum %d.", obj->pIndexData->vnum );
				return FALSE;
			}

			horse = create_mobile( get_mob_index( obj->pIndexData->horse) );


			for( obj_horse = obj->contains; obj_horse ; obj_horse = obj_horse_next )
			{
				obj_horse_next = obj_horse->next_content;
				obj_from_obj( obj_horse );
				obj_to_char( obj_horse, horse );
			}           

			char_to_room( horse, victim->in_room);
			SET_BIT( horse->act, ACT_HORSE   );
			SET_BIT( horse->act, ACT_SENTINEL);
			add_follower( horse, victim );
			add_rember( horse , victim );

			continue;
		}    
	}

	init_char_data(victim);

	if ( victim->race == race_lookup( "vampire" ) )
	{
		victim->pcdata->condition[COND_FULL  ] = 40;
		victim->pcdata->condition[COND_THIRST] = 0;
	}

	loc = victim->in_room;

	do_save(victim, "");
	if( loc != victim->in_room ) {
		char_from_room(victim);
		char_to_room(victim, loc);
	}
	send_to_char("¦º¦Ó´_¥Í«á¡A°O±o»°ºò§ä¦^§Aªº«ÍÅé©Mª««~¡A¨Ã¥B­n¶i¦æ save ªº°Ê§@¡C\n\r", victim);
	stop_fighting(victim, TRUE);
	WAIT_STATE( victim, 2 * PULSE_VIOLENCE );

	return TRUE;
}


// group_gain modified by jye 7.13.2005
void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
	
  CHAR_DATA *gch;
  CHAR_DATA *lch;
  char       buf[ BUF_STRING_LENGTH ];
  int        members;
  int        xp;
  bool    found = FALSE;

  /*
   * Monsters don't get kill xp's or alignment changes.
   * Dying of mortal wounds or poison doesn't give xp to anyone!
   */
  //    if ( IS_NPC( ch ) || victim == ch )
  //  return;

  if( victim == ch )
  {
      return;
  }
  
  if( !ch->in_room )
  {
      return;
  }
  
  if( IS_NPC(victim) && IS_SET(victim->act, ACT_MPROG_DEATH)) 
  {
      return;
  }
  members = 0;
  if( IS_NPC(ch) ) {
      if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
          if(ch->master != NULL) ch = ch->master;
      }
  }
  
  for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
  {
       if ( is_same_group( gch, ch ) )
         members++;
  }

  if( members == 0 )
  {
      bug( "Group_gain: members.", members );
      members = 1;
  }

  lch = ( ch->group && ch->group->leader ) ? ch->group->leader : ch;
  if(lch->in_room != ch->in_room) lch = ch;

  for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
  {
       OBJ_DATA *obj;
       OBJ_DATA *obj_next;

    if( !is_same_group( gch, ch ) && 
        ( gch != ch->master ||( !IS_AFFECTED( ch, AFF_CHARM ) && !IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
        continue;

    if( IS_NPC( ch ) && ch->master != gch && (  ch->level - gch->level > 5 || ch->level - gch->level < -5 ) ){
        continue;
    }

    if( gch->level - ch->level >= 6 )
    {
        send_to_char( "§Aªºµ¥¯Å¤Ó°ª¤F.\n\r", gch );
        continue;
    }

    if( gch->level - ch->level <= -6 )
    {
        send_to_char( "§Aªºµ¥¯Å¤Ó§C¤F.\n\r",  gch );
        continue;
    }
  
    if( !IS_NPC( gch ) && gch->class <= BARBARIAN && !IS_SET( gch->act, PLR_HORSE ) )
        found = TRUE;
        
    if( members < 3 )
        xp = xp_compute( gch, victim );
    else if( members < 7 )
        xp = xp_compute( gch, victim ) * 6 / 5;
    else if( members < 11 )
        xp = xp_compute( gch, victim ) * 9 / 8;
    else 
        xp = xp_compute( gch, victim ) * (members > 15 ? 15 : members ) / members;

    if( !str_infix( race_table[lch->race].name,race_table[gch->race].hate) && members > 1 )
    {
        send_to_char( "¦]¬°¶¤¥î¸Ì¦³­Ó°Q¹½ªº³Ã¥ë¡A§A·l¥¢¤F¤»¤À¤§¤@ªº¸gÅç.\n\r", gch );
        xp -= xp/6;
    }
    
    if( gch && found && gch->class > BARBARIAN && gch == lch )
    {
        send_to_char( "§A¦]¬°±a»â¦~»´«_ÀIªÌ¡A¦h±o¤T¤À¤§¤@ªº¸gÅç.\n\r", gch );
        xp += xp/3;
    }
    
    if( IS_SET( gch->act, PLR_HORSE ) )
        xp -= xp/3;
        
    if( ch->level > 35 && gch->level < 36 )
        xp /= UMAX( 1,( ch->level - gch->level - members + 3 ));
        
    if( ch->level < 37 )
        xp = xp*3/2;

    sprintf( buf, "§AÀò±o¤F "HIG"%d"NOR" ÂI¸gÅç.\n\r", xp );
    send_to_char( buf, gch );
    gain_exp( gch, xp );

    for( obj = gch->carrying; obj; obj = obj_next )
    {
         obj_next = obj->next_content;
         
         if( obj->deleted )
             continue;
             
         if( obj->wear_loc == WEAR_NONE )
             continue;
             
         if( is_broken(obj) )
         {
             act( "$p·lÃa¤F.", gch, obj, NULL, TO_CHAR );
             unequip_char(gch, obj);
             //modified by jye 0940713
             obj_from_char( obj );
             obj_to_char( obj, gch );
             continue;
         }
         
         if(( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_EVIL   ( gch ) )
             ||( IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) && IS_GOOD   ( gch ) )
             ||( IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL ) && IS_NEUTRAL( gch ) ) )
         {
             act( "§A³Q $p µ¹±Æ¥¸¤F.", gch, obj, NULL, TO_CHAR );
             act( "$n ³Q $p ±Æ¥¸¤F.",   gch, obj, NULL, TO_ROOM );
             unequip_char(gch, obj);
             obj_from_char( obj );
         
             if( IS_SET(obj->extra_flags , ITEM_PERSONAL) )
               obj_to_char( obj, gch );
             else
               obj_to_room( obj, gch->in_room );
             
             continue;
         }
         if( obj->pIndexData->max_level > 0 && gch->level > obj->pIndexData->max_level )
         {
             sprintf( buf, "§A¥²¶·¦bµ¥¯Å %d ¥H¤º¤~¯à¨Ï¥Î³o¼Ëª««~.\n\r",obj->pIndexData->max_level );
             send_to_char( buf, gch );
             unequip_char(gch, obj);
             continue;
         }
    }
  }

  return;
}



/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
  OBJ_DATA *obj;
  float     bonus;
  int       xp;
  int       align;
  int       extra;
  int       level;
  int       number;

  level = gch->level - victim->level;
	if(level < -18) level = -18;
  if(victim->level < 40 && level < -10) level = -10;
  if(gch->class < WIZARD && gch->level < 36) level -= 5;
  bonus = 1.0;
  xp    = 100;
  if( IS_NPC(victim) ) xp += victim->exp;
  if(xp < 0) xp = 0;
	if(level > 0) // low Lv wins high Lv
		xp  = xp * (100 - level * 15) / 100;
	else
		xp  = xp * 100 / (100 + level * 5);

  if(xp < 0) xp = 0;
  /* Previous exp
  // xp    = 80 - URANGE( -10, level, 5 ) * 20;
  //20060714 modify by jye
  //ppl¤j©ómobµ¥¯Å5¯Å®Éreturn 0
  //µ¥¯Å¸ò victim ¦b¤­¥H¤º, ¤~¯à»â¨ì area writer ³]ªºÃB¥~ exp, modified by
  //razgriz 20060716
  if ( level <= 5 )
  {
    if( IS_NPC(victim) )
      xp += victim->exp;
  }

  if ( level < -9 )
    xp -= level * 8;
  */
  if( is_same_group(gch, victim) ) return 0;

  align = gch->alignment - victim->alignment;


  if ( !IS_NPC( gch ) || IS_AFFECTED( gch, AFF_SUMMONED )
      || IS_AFFECTED( gch, AFF_CHARM ) ) { //npc's alignment is fixed.
    if( IS_NEUTRAL( gch ) ) //neutral has slower alignment change rate
    {
      gch->alignment += (((( 0 - victim->alignment ) / 200) + ( (align > 0 ) ? (1) : (-1)))) / 2 ;
    }
    else if (IS_GOOD( victim)){ // kill good char always decrease align
      gch->alignment -= ( (((victim->alignment - GOOD_BASE) * (victim->alignment - GOOD_BASE) / 45000 ) + 3 ) / 5 );
    }
    else if (IS_EVIL( victim )){ // kill evil char always gain alignment
      gch->alignment += ( (((EVIL_BASE - victim->alignment ) * (EVIL_BASE - victim->alignment ) / 45000 ) + 3 ) / 5 );
    }
    else {
      gch->alignment += (((( align) / 300) + ( (align > 0 ) ? (1) : (-1))) / 5 );
    }

    xp = 5 * xp / 4; 
/*
    if ( align >  500 )
      xp = 5 * xp / 4;
    else if ( align < -500 )
      xp = 5 * xp / 4;
    else
      xp = 3 * xp / 4;
*/
    if ( IS_SET( race_table[ gch->race ].race_abilities, RACE_VAMPIRE ) || 
         IS_SET( race_table[ gch->race ].race_abilities, RACE_GARGOYLE ) )
      gch->alignment = UMIN( 0, gch->alignment );

    gch->alignment = URANGE( -1000, gch->alignment, 1000);
  }

  if ( IS_AFFECTED( victim, AFF_SANCTUARY )
      || IS_SET( race_table[ victim->race ].race_abilities, RACE_SANCT ) )
    bonus += (float)1/2;

  if ( IS_AFFECTED( victim, AFF_FLAMING ) )
    bonus += (float)4/10;

  if( IS_AFFECTED( victim, AFF_MYSTIC_MIRROR))
    bonus += (float) 3/10;


  if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) )
    bonus += (float)1/4;

  if ( ( obj = get_eq_char( victim, WEAR_TWO_HAND ) ) )
    bonus += (float)1/4;

  if ( ( obj = get_eq_char( victim, WEAR_WIELD_2 ) ) )
    bonus += (float)1/5;

  if ( !str_infix( race_table[victim->race].name,
        race_table[gch->race].hate ) )
    bonus += (float)1/10;

  if ( victim->race == gch->race )
    bonus -= (float)1/8;

  if ( IS_NPC( victim ) )
  {
    if( IS_SET( victim->special, NO_EXP ) )
      return 0;
    if( IS_SET( victim->special, REF_MAGIC ) )
      bonus += (float)2/5;
    if( IS_SET( victim->special, NO_DM) )
      bonus += (float)1/5;
    if( IS_SET( victim->special, ALY_SAN ) )
      bonus += (float)1/5;
    if( IS_SET( victim->special, NO_MUTE) 
        ||  IS_SET(victim->special, NO_SEALMAGIC)
        ||  IS_SET(victim->special, NO_CRAZY))
      bonus += (float)1/10;
    if( IS_SET( victim->special, MOBAI_LV2) )
      bonus += (float)1/10;
    if( IS_SET( victim->special, MOBAI_LV3) )
      bonus += (float)1/5;
    if( IS_SET( victim->special, NO_PARA) )
      bonus += (float)1/10;
    if( IS_SET( victim->special, NO_TARDY) )
      bonus += (float)1/10;
    if( IS_SET( victim->special, ALY_BAR) )
      bonus += (float)1/10;
    if( IS_SET( victim->special, ANTI_MAGIC ) )
      bonus += (float)2/5;
    if( IS_SET( victim->special, NO_BLIND))
      bonus += (float)1/10;
    if( victim->level > 50 && IS_SET(victim->special, EXPERT))
      bonus += (float) (0.05)* ( victim->level - 50);
    if ( IS_SET( victim->act, ACT_MOB_AI) )
      bonus += (float)1/7;
    if ( IS_SET( victim->act, ACT_AGGRESSIVE ) )
      bonus += (float)1/4;
    if( victim->jcl || victim->psk )
      bonus -= (float)7/8;

    if ( victim->pIndexData->pShop )
      bonus -= (float)4/5;

    if ( victim->spec_fun )
    {
      if (   victim->spec_fun == spec_lookup( "spec_breath_any"     )
          || victim->spec_fun == spec_lookup( "spec_cast_psionicist"  )
          || victim->spec_fun == spec_lookup( "spec_cast_undead"      )
          || victim->spec_fun == spec_lookup( "spec_breath_gas"     )
          || victim->spec_fun == spec_lookup( "spec_cast_mage"      ) )
        bonus += (float)1/3;

      if (   victim->spec_fun == spec_lookup( "spec_breath_fire"      )
          || victim->spec_fun == spec_lookup( "spec_breath_cold"      )
          || victim->spec_fun == spec_lookup( "spec_breath_acid"      )
          || victim->spec_fun == spec_lookup( "spec_breath_lightning" )
          || victim->spec_fun == spec_lookup( "spec_cast_cleric"      )
          || victim->spec_fun == spec_lookup( "spec_cast_judge"     )
          || victim->spec_fun == spec_lookup( "spec_cast_ghost"     ) )
        bonus += (float)1/5;

      if (   victim->spec_fun == spec_lookup( "spec_poison"     )
          || victim->spec_fun == spec_lookup( "spec_thief"      ) )
        bonus += (float)1/10;

      if ( victim->spec_fun == spec_lookup( "spec_cast_adept"     ) )
        bonus -= (float)3/4;

      if ( gch->class == CLERIC || gch->class == SAVANT )
        bonus += (float)1/5;
      if ( gch->class > BARBARIAN && !IS_SET(victim->special, EXPERT) && gch->level > 29 )
        bonus -= (float)7/8;
      else if ( gch->class > BARBARIAN )
        bonus -= 0.4f;

      if ( !strcmp( race_table[ gch->race ].name, "Gnome" ) )
      {
        //    send_to_char("because gnome u gain more exp\n\r",gch);
        bonus += (float)1/4;
      }
    }
    bonus += (bonus - 1) * 0.1;
    if(bonus < 0) bonus = 0;
  }
  else
  {
    bonus = 0.0;
  }

  xp = (int) ( xp * bonus );

  /*
   * Adjust for popularity of target:
   *   -1/8 for each target over  'par' (down to - 75%)
   *   +1/8 for each target under 'par' (  up to + 50%)
   * if PC only kill one type of mob in that mob level, then
   * the xp PC gets will degenerate till -50%, on the other
   * hand, if that mob seldom being killed by PC, then the
   * xp PC gets may up to +25%
   */
  if ( IS_NPC( victim )) {
    if  (!IS_SET(victim->act, ACT_NOPAR))  /* mud school protection */
    {
      level  = URANGE( 0, victim->level, MAX_LEVEL - 1 );
      number = UMAX( 1, kill_table[level].number );
      extra  = victim->pIndexData->killed - kill_table[level].killed
        / number;
      xp    -= xp * URANGE( -4, extra, 6 ) / 8;
    }
  }

  xp     = number_range( xp , 2 * xp );
// ·h¨ì³Ì«e­±¥ý­pºâ modified by razgriz 20060716
// if( IS_NPC(victim) )
// xp += victim->exp;
  xp = UMAX( 0, xp );
  if ( !IS_NPC( victim ) )
      xp = UMIN( xp, 250 );
  if(xp > 100000){
      xp = number_range(80000, 120000);
  }
  return xp;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
	OBJ_DATA *obj;

	if ( !IS_NPC(victim) && victim->class < 4 )
		return;

	//if ( (race_table[ch->race].size - race_table[victim->race].size ) < -2 )
	if ( (get_curr_size( ch ) - get_curr_size( victim ) ) < -2 ) // ´À´«Åé«¬ size ¤½¦¡ 2022/12/23
		return;

	if ( !( obj = get_eq_char( victim, WEAR_WIELD ) ) )
		if ( !( obj = get_eq_char( victim, WEAR_WIELD_2 ) ) )
			if ( !( obj = get_eq_char( victim, WEAR_TWO_HAND ) ) ){
				send_to_char("§Aªº¥Ø¼Ð¨Ã¨S¦³¸Ë³ÆªZ¾¹¡I\n\r", ch);
				return;
			}

	// npc disarm ®É¦³ 50% ¾÷²v¤£¦¨¥\ disarm 2022/02/18
	//if( !IS_NPC(victim) && victim->pcdata->learned[gsn_swordmaster] == 0)
	if( IS_NPC(ch) )
	{
		if( 50 > number_percent( ) ){
			send_to_char("§AµLªkÀ»¸¨¹ï¤âªºªZ¾¹¡C\n\r", ch);
			return;
		}
	}

	// Âù«ùªZ¾¹¦³ 40% ¾÷²v¤£³Q disarm
	if ( get_eq_char( victim, WEAR_TWO_HAND ) && 40 > number_percent( ) )
	{
		send_to_char("¹ï¤èÂù¤âºò´¤ªZ¾¹¡A§AµLªkÀ»¸¨¹ï¤èªºªZ¾¹¡C\n\r", ch);
		return;
	}

	// pc victim ·| swordmaster ªº±¡ªp
	if( !IS_NPC( victim ) && victim->pcdata->learned[gsn_swordmaster] > 0 ){
		// ±N pc ¤Î npc ©ñ disarm ¤À¶}§P©w 2022/02/18
		if( !IS_NPC( ch ) ){
			if( ( get_skill_percent(victim,gsn_swordmaster)/2 + get_skill_level(victim, gsn_swordmaster) * 30
				 + ( victim->class == SWORDSMAN ? get_skill_level(victim, gsn_swordmaster) * 10 : 0 )
				 - get_skill_level(ch, gsn_barehand) * 3 - get_skill_level(ch, gsn_disarm) * 3 )
				 > number_percent( ) )
			{
				if( !is_chinese(ch, victim))
					act( "$N's swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_NOTVICT );
				else
					act( "$N °ª¶Wªº±s¼C³N¨¾¤î $n À»¸¨$SªZ¾¹¡I", ch, NULL, victim, TO_NOTVICT );

				if( !is_chinese(victim, ch))
					act( "Your swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_VICT );
				else
					act( "§A°ª¶Wªº±s¼C³N¨¾¤î $n À»¸¨§AªºªZ¾¹¡I", ch, NULL, victim, TO_VICT );
				return;
			}
		}else{
			if( ( get_skill_percent(victim,gsn_swordmaster)/2 + get_skill_level(victim, gsn_swordmaster) * 45
				+ ( victim->class == SWORDSMAN ? get_skill_level(victim, gsn_swordmaster) * 10 : 0 )
				- ch->level - get_skill_level(ch, gsn_disarm) * get_skill_level(ch, gsn_disarm) )
				> number_percent( ) )
			{
				if( !is_chinese(ch, victim))
					act( "$N's swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_NOTVICT );
				else
					act( "$N °ª¶Wªº±s¼C³N¨¾¤î $n À»¸¨$SªZ¾¹¡I", ch, NULL, victim, TO_NOTVICT );

				if( !is_chinese(victim, ch))
					act( "Your swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_VICT );
				else
					act( "§A°ª¶Wªº±s¼C³N¨¾¤î $n À»¸¨§AªºªZ¾¹¡I", ch, NULL, victim, TO_VICT );
				return;
			}
		}
	}
	// npc victim §P©w
	if( IS_NPC( victim ) ){
		if( IS_SET( victim->special, NO_DISARM )
			|| ( IS_SET( ch->act, ACT_HARD_DIS )
				&& ( victim->level - get_skill_level(ch, gsn_disarm) * 5 - get_skill_level(ch, gsn_barehand) * 5
					> number_percent( ) ) ) )
		{
			act( (!is_chinese(ch, victim)) ?
				"$N's swordmaster skill prevent from $n's disarm!":
				"$N °ª¶Wªº±s¼C³N¨¾¤î $n À»¸¨$SªZ¾¹¡I", ch, NULL, victim, TO_NOTVICT );
			return;
		}
	}

	if ( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) )
		//|| IS_SET( obj->extra_flags, ITEM_NODROP ) ) // nodrop ¬°¤£·|±¼¨ì¦a¤W¤£µ¥©ó¤£·|±q¤â¤¤©ñ¤U , ¬G²¾°£ 2022/02/18
	{
		send_to_char("§AµLªkÀ»¸¨¹ï¤èªºªZ¾¹¡C\n\r", ch);
		return;
	}


	if( IS_NPC( ch ) )
	{
		if(    !IS_SET( ch->class, 32 ) // npc class 32(bloody) µ¥¦P©ó fist fight ®ÄªG 2022/02/18
			&& !get_eq_char( ch, WEAR_WIELD   )
			&& !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) )
		{
			send_to_char("§A¥²¶·¥ý¸Ë³ÆªZ¾¹¡C\n\r", ch);
			return;
		}
	}

	// ­×§ïÅã¥Ü¦â½X 2022/11/04
	if( !is_chinese(ch, victim) )
	{
		//act( HIY"You disarm $N!"NOR,  ch, NULL, victim, TO_CHAR  );
		//act( "[1;31m$n DISARMS $N![m",  ch, NULL, victim, TO_NOTVICT );
		act( ""HIY"You disarm "NOR"$N"HIY"'s weapon!"NOR"",  ch, NULL, victim, TO_CHAR );
		act( "$n"HIY" DISARM "NOR"$N"HIY"'s weapon!"NOR"",  ch, NULL, victim, TO_NOTVICT );
	}
	else
	{
		//act( HIY"§AÀ»¸¨¤F $N ªºªZ¾¹¡I"NOR,  ch, NULL, victim, TO_CHAR  );
		//act( "[1;31m$n À»¸¨¤F $N ªºªZ¾¹¡I[m",  ch, NULL, victim, TO_NOTVICT );
		act( ""HIY"§AÀ»¸¨¤F "NOR"$N"HIY" ªºªZ¾¹¡I"NOR"",  ch, NULL, victim, TO_CHAR );
		act( "$n"HIY" À»¸¨¤F "NOR"$N"HIY" ªºªZ¾¹¡I"NOR"",  ch, NULL, victim, TO_NOTVICT );
	}

	// ³QÀ»¸¨ªÌ·|¬Ý¨ì¦Û¤v³QÀ»¸¨ªº obj ¦W 2022/11/04
	if( !is_chinese(victim, ch)){
		//act( "[1;31m$n DISARMS you![m", ch, NULL, victim, TO_VICT  );
		act( "$n"HIY" DISARM your "NOR"$p"HIY" !"NOR,  ch, obj, victim, TO_VICT );
	}else{
		//act( "[1;31m$n À»¸¨¤F§AªºªZ¾¹¡I[m", ch, NULL, victim, TO_VICT );
		act( "$n"HIY" À»¸¨¤F§AªºªZ¾¹ "NOR"$p"HIY" ¡I"NOR"", ch, obj, victim, TO_VICT );
	}

	obj_from_char( obj );
	if ( IS_NPC( victim )
		|| get_skill_level(victim, gsn_swordmaster) >= 3
		|| IS_SET( obj->extra_flags, ITEM_NODROP ) ) // item_nodrop ¤£·|±¼¨ì¦a¤W 2022/02/19
		obj_to_char( obj, victim );
	else if ( can_drop_obj(ch, obj) )
		obj_to_room( obj, victim->in_room );
	else
		obj_to_char( obj, victim );

	return;
}



/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
	// char  buf[MAX_STRING_LENGTH];
	int mod = 100;

	if( is_flying(victim)
		|| victim->position <= POS_RESTING
		|| IS_SET(race_table[victim->race].race_abilities, RACE_LIZARDMAN ) )
	return;

	if( !IS_NPC(victim) 
		&& ( victim->class == KNIGHT || victim->class == LORD ) 
		&& get_eq_char( victim, WEAR_RIDE) )
	return; // zcecil

	if( is_affected(victim, gsn_stone_form) ) return;

	if( IS_NPC( victim ) && IS_SET( victim->special, NO_TRIP ) )
	{
		if( !is_chinese(ch, victim))
			act( HIG"$N's bodycontrol skill prevent from your trip!"NOR, ch, NULL, victim, TO_CHAR );
		else
			act( HIG"$N ¥©§®¦a«O«ù¦í¨­Åéªº¥­¿Å¡I"NOR, ch, NULL, victim, TO_CHAR );
		return;
	}
	if( !IS_NPC(victim)){
		if( victim->pcdata->autocombat == AUTOC_DODGE ) {
			mod -= get_skill_percent(victim, gsn_autododge)/2 + get_skill_level(victim, gsn_autododge);
		}else if (victim->pcdata->autocombat == AUTOC_PARRY){
			mod += get_skill_percent(victim, gsn_autoparry)/3 + get_skill_level(victim, gsn_autoparry);
		}
	}

	if( ( !IS_NPC( victim )
			&& ( get_skill_level(victim, gsn_bodycontrol)
				+ (get_skill_level(victim, gsn_bodycontrol) > 5 ? get_skill_level(victim, gsn_bodycontrol) : 0 )
				+ get_skill_percent(victim, gsn_bodycontrol)
				+ 10 * ( get_curr_wis( victim ) - 15 ) ) * 3 / 4 > (number_percent( ) * mod / 100)
				+ ((IS_SET(race_table[ch->race].race_abilities, RACE_LIZARDMAN)) ? 20 : 0))
		|| ( IS_NPC(victim) && IS_SET(victim->special, EXPERT) && number_percent()  > ch->level - 20))
	{
		if( !is_chinese(ch, victim)){
			act( HIG"$N's bodycontrol skill prevent from your trip!"NOR, ch, NULL, victim, TO_CHAR );
		}else{
			act( HIG"$N ¥©§®¦a«O«ù¦í¨­Åéªº¥­¿Å¡I"NOR, ch, NULL, victim, TO_CHAR );
		}

		if( !is_chinese(victim, ch)){
			act( HIG"Your bodycontrol skill prevent from $n's trip!"NOR, ch, NULL, victim, TO_VICT );
		}else{
			act( HIG"§A¥©§®¦a«O«ù¤F¨­Åéªº¥­¿Å¡I"NOR, ch, NULL, victim, TO_VICT );
		}
		return;
	}

	if ( victim->wait == 0 )
	{
		//if( !IS_NPC( ch ) &&!IS_SET( ch->act, PLR_CHINESE ) )
		if( !is_chinese(ch, victim)){
			act( HIY"You trip $N and $N goes down!"NOR, ch, NULL, victim, TO_CHAR );
			act( HIY"$n trips $N and $N goes down!"NOR, ch, NULL, victim, TO_NOTVICT );
		}else{
			act( HIY"§A²Ì­Ë¤F $N¡I"NOR, ch, NULL, victim, TO_CHAR );
			act( HIY"$n §â $N ²Ì­Ë¦b¦a¡I"NOR, ch, NULL, victim, TO_NOTVICT );
		}

		if( !is_chinese(victim, ch)){
			act( HIY"$n trips you and you go down!"NOR, ch, NULL, victim, TO_VICT );
		}else {
			act( HIY"$n §â§A²Ì­Ë¦b¦a¡I"NOR, ch, NULL, victim, TO_VICT );
		}

		//  WAIT_STATE( ch, PULSE_VIOLENCE );
		WAIT_STATE( victim, PULSE_VIOLENCE * 2);
		victim->position = POS_RESTING;
	}

	return;
}



void do_kill( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§AµLªk¦b¥Û¤Æª¬ºA¥D°Ê§ðÀ».\n\r", ch);
		return;
	}

	if ( arg[0] == '\0' )
	{
		send_to_char( "­n±þ½Ö¡H\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
	}
	if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)){
		send_to_char("³o¸Ì¸T¤î¾Ô°«¡C\n\r", ch);
		return ;
	}
	if ( victim == ch )
	{
		send_to_char( "½Ð¨Ï¥Î MURDER ¡I\n\r", ch );
		// multi_hit( ch, ch, TYPE_UNDEFINED );
		return;
	}

	if ( !IS_NPC( victim ) )
	{
		if (   !IS_SET( victim->act, PLR_KILLER )
			&& !IS_SET( victim->act, PLR_THIEF  )
			&& !IS_SET( race_table[ victim->race ].race_abilities, RACE_VAMPIRE )
			&& !IS_SET( victim->in_room->room_flags, ROOM_PK) )
		{
			send_to_char( "½Ð¨Ï¥Î MURDER.\n\r", ch );
			return;
		}
	}
	else
	{
		if (( IS_AFFECTED( victim, AFF_CHARM ) || IS_AFFECTED( victim, AFF_SUMMONED) ) && victim->master && !IS_NPC(victim->master) )
		{
			send_to_char( "³o°¦¥Íª«¬O¦³¥D¤Hªº¡A½Ð¨Ï¥Î MURDER.\n\r", ch );
			return;
		}
	}


	if ( is_safe( ch, victim ) )
		return;

	if ( IS_AFFECTED( ch, AFF_CHARM ) && !IS_NPC( victim ) )
	{
		send_to_char( "§AµLªk§ðÀ»§O¤H¡I\n\r", ch );
		return;
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "§A¤w¸g¦b¾Ô°«¤¤¤F¡I\n\r", ch );
		return;
	}

	//WAIT_STATE( ch,  PULSE_VIOLENCE / 2);

	multi_hit( ch, victim, TYPE_UNDEFINED );

	// °w¹ï artifact ªº±¡ªp, Åý³Q sleep ¤Îcharm ªº¥Ø¼Ð·|½T¹ê°±¤î§ðÀ»¡AÁ×§K³y¦¨crash
	if( IS_AFFECTED( victim, AFF_SLEEP )) return;
	if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch ) return; // ¥[¤J master §P©w 2022/12/31

	if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
		stop_fighting(victim, FALSE);

	// victim ¶}¾Ô²Ä¤@¦^¦X´N¦º¤`«h«áÄò¤£§P©w, ¥H§K ch ¸òªÅ®ð¥´¬[ 2022/07/14
	if( !victim || victim->deleted ) return;

	// ¥Ø¼Ð°k¶]«á ch ·|¸òªÅ®ð¥´¬[ ¬G²¾°£ 2022/07/15
	//set_fighting(victim, ch);

	return;
}


void do_murder( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim = NULL;;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if(ch == NULL) return;

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§AµLªk¦b¥Û¤Æª¬ºA¥D°Ê§ðÀ».\n\r", ch);
		return;
	}

	if ( arg[0] == '\0' )
	{
		send_to_char( "Murder ½Ö¡H\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
	}

	if(victim == NULL) return;

	if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)){
		send_to_char("³o¸Ì¸T¤î¾Ô°«¡C\n\r", ch);
		return ;
	}

	if ( victim == ch )
	{
		send_to_char( "§A¹ï¦Û¤vµo°Ê§ðÀ»¡I\n\r", ch );
		multi_hit( ch, ch, TYPE_UNDEFINED );
		return;
	}

	if( !IS_SET( ch->act, PLR_REGISTER ) && (ch->in_room && !IS_SET(ch->in_room->room_flags, ROOM_PK))) {
		send_to_char("§A¨Ã¨S¦³°Ñ¥[ PK¡I\n\r", ch);
		return;
	}

	if ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim , AFF_SUMMONED) &&  (!victim->master || IS_NPC(victim->master)))
	{
		do_kill( ch, arg );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "§A¤w¸g¦b¾Ô°«¤¤¤F¡I\n\r", ch );
		return;
	}

	//WAIT_STATE( ch, PULSE_VIOLENCE / 2);
	//if( victim->master 
	check_killer( ch, victim );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return;

}

void do_flee( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	ROOM_INDEX_DATA *was_in;
	ROOM_INDEX_DATA *now_in;
	char arg[MAX_INPUT_LENGTH];
	int attempt;
	int dir;
	int adjust = 100;
	int mob_adjust  = UMIN( 30 + ch->level, number_percent() + ch->level/2 );
	int sklv = get_skill_level(ch, gsn_escape);
	int int_number_percent = number_percent();
	int int_escape_percent = get_skill_percent(ch, gsn_escape);
	int nStun = 6 - sklv * (sklv - 1) / 6;
	int IsFear = 0;  // ³B²z fear ª¬ºA 2022/10/13

	if( !ch || ch->deleted )
		return;

	// ¥H hp ©Î pos_dead §P©w, ¦]¬° raw_kill ¤£¤@©w¬° pos_dead  2022/12/29
	if( ch->position == POS_DEAD || ch->hit <= 0 )
		return;

	if ( get_eq_char( ch , WEAR_RIDE ) )
	{
		if (get_skill_level(ch, gsn_ride) > 4) nStun--;
	}

	if( IS_AFFECTED( ch, AFF_FEAR ) ) IsFear = 1; // ¦pªG¦³ fear ª¬ºA«h¥¢±Ñ®É¤£·| stun 2022/10/13

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		int nInbornSklv = get_skill_level(ch, gsn_inborn_ability);
		if (nInbornSklv > 4) nStun -= nInbornSklv - 4;

		int_escape_percent = (100 * nInbornSklv + int_escape_percent * 6) / (nInbornSklv + 6);
	}

	if (nStun < 1) nStun = 1;

	if( !( victim = ch->fighting ) )
	{
		update_pos( ch );
		send_to_char( "¦n¦nªº¨S¨Æ°k¤°»ò¡H\n\r", ch );
		return;
	}
	if( !ch->in_room || victim->deleted ) return;

	if( IS_SET( ch->in_room->room_flags, ROOM_NO_FLEE ) )
	{
		send_to_char( "§A¨Sªk°k¶]¡C\n\r", ch );
		return;
	}

	if( is_affected( ch, gsn_berserk ) )
	{
		send_to_char( "§A¥¿¥þ¤O§ë¤J¾Ô°«¡A«D±o­n¤À¥X­Ó¼Ä¦º§Ú¬¡¤~¦æ¡C\n\r", ch );
		return;
	}

	// affect stone form ­×¥¿ 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§A¦b¥Û¤Æª¬ºA¤U¤£¯à²¾°Ê.\n\r", ch);
		return;
	}

	if( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare ±Ô­z­×§ï 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m§A¥¿³Q§x¦b³´¨À¸Ì°Ê¼u¤£±o¡I[m\n\r", ch );
		}else{
			send_to_char( "[33m§A¸ÕµÛ°k¶]¡A¦ý¨­Åé«o°Ê¼u¤£±o¡I[m\n\r", ch );
		}
		act( "$n ¸ÕµÛ°k¶]¡A¦ý¬O«o°Ê¼u¤£±o¡C",ch, NULL, NULL, TO_ROOM );
		return;
	}

	if( ch->move < 20 )
	{
		send_to_char( "§A¨S¦³¨¬°÷ªºÅé¤O°k¶]¡I\n\r", ch );
		return;
	}
	// ¨¾¤î mob ¾Ô°«¤¤µL­­°k¶]
	if( IS_NPC(ch) && IS_SET( ch->act, ACT_NO_FLEE ))
		return;


	if( !IS_NPC(ch) )
	{
		//°ò¥»Â¾
		switch( ch->pcdata->prime_attr ) 
		{
			case APPLY_DEX:
				adjust = 25;
				break;
			case APPLY_STR:
				adjust = 20;
				break;
			case APPLY_CON:
				adjust = 15;
				break;
			case APPLY_WIS:
				adjust = 10;
				break;
			case APPLY_INT:
				adjust =  5;
				break;
		};

		//¶i¶¥Â¾
		switch( ch->pcdata->snd_attr ) 
		{
			case APPLY_INT:
				adjust += 25;
				break;
			case APPLY_DEX:
				adjust += 20;
				break;
			case APPLY_CON:
				adjust += 15;
				break;
			case APPLY_WIS:
				adjust += 10;
				break;
			case APPLY_STR:
				adjust += 5;
				break;
		};

		adjust += int_escape_percent / 10;
		adjust += get_curr_dex(ch) / 4;

		if( ch->class == ASSASSIN )
			adjust += 20;
		else if( ch->class == NINJA ) 
			adjust += 30;
		else if( ch->class < WIZARD ) 
			adjust = 100;
	}
	if ( get_eq_char( ch , WEAR_RIDE ) )
	{
		int nRideSklv = get_skill_level(ch, gsn_ride);
		if(nRideSklv > 0) adjust += (nRideSklv + 4) * nRideSklv / 2;
	}

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ))
	{
		int nInbornSklv = get_skill_level(ch, gsn_inborn_ability);
		adjust += (nInbornSklv + 4) * nInbornSklv / 2;
	}

	adjust += sklv * (sklv + 4) / 2;
	adjust = UMIN( 90 + sklv, adjust);
	one_argument(argument, arg);

	was_in = ch->in_room;

	// ªý¤îmob³sÄòflee
	if( IS_NPC(ch) ) SET_BIT( ch->act, ACT_NO_FLEE );

	if( arg[0] == '\0' || ( !IS_NPC(ch) && int_number_percent + 30 > int_escape_percent + sklv * 6)) 
	{

		for( attempt = 0; attempt < 6; attempt++ )
		{
			EXIT_DATA *pexit;
			int door;

			door = number_door( );/*generate a random number from 0-5*/
			if( ( pexit = was_in->exit[door] ) == 0 
				|| !pexit->to_room
				|| ( IS_SET( pexit->exit_info, EX_CLOSED )
					&& !IS_AFFECTED( ch, AFF_PASS_DOOR )
					&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) 
					&& ! IS_SET( pexit->exit_info, EX_PASSPROOF ))
				|| ( IS_NPC( ch )
					&& ( IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB )
						|| ( IS_SET( ch->act, ACT_STAY_AREA )
							&& pexit->to_room->area != ch->in_room->area ) ) ) )
			continue;

			if( victim->level > 40 || (!IS_NPC(victim) && victim->level > 34))
			{
				if( int_number_percent > ( IS_NPC(ch) ?  mob_adjust:adjust ) )
				{
					send_to_char( "§A¸ÕµÛ°k¶]¡A¦ý«o³Q¹ï¤âªý¾×¤U¨Ó¡I\n\r", ch );
					send_to_char("§Aªý¾×¹ï¤â°k¶]ªº·N¹Ï¡I\n\r", victim);
					if( !IS_NPC(ch) )
					{
						if( ch->pcdata->learned[gsn_escape] )
						{
							if( int_number_percent + 1 > int_escape_percent / 2 + sklv * 5)
							{
							   send_to_char( "¥¢±Ñ¤F¡I §A¥¢¥h¤F "HIG"20"NOR" ÂI¸gÅç.\n\r", ch );
							   ch->exp = ch->exp - 20;
							}
						}
						else if( sklv < 6 )
						{
							send_to_char( "¥¢±Ñ¤F¡I §A¥¢¥h¤F "HIG"15"NOR" ÂI¸gÅç.\n\r", ch );
							ch->exp = ch->exp-15;
						}
						if( IsFear == 0 ) // ³B²z fear ª¬ºA flee ¥¢±ÑµL­­ stun ªº°ÝÃD 2022/10/13
							WAIT_STATE(ch, 3);
						return;
					}
					return;
				}
			}

			move_char( ch, door );
			if( ( now_in = ch->in_room ) == was_in )
			{
				if( ch->fighting ) stop_fighting(ch, FALSE);
				if( victim->fighting ) stop_fighting(victim, FALSE);
				set_fighting(ch, victim);
				set_fighting(victim, ch);
				continue;
			}

			ch->in_room = was_in;
			act( "$n °k¨«¤F¡I", IS_SET( ch->act, PLR_HORSE ) && ch->desc ? ch->pcdata->horse : ch, NULL, NULL, TO_ROOM );
			ch->in_room = now_in;

			if( !IS_NPC(ch) )
			{
				if( ch->pcdata->learned[gsn_escape])
				{
					if( int_number_percent + 1 > int_escape_percent / 2 + sklv * 5)
					{
						send_to_char( "§A¦Û¾Ô°«¤¤°k¶]¡A·l¥¢¤F "HIG"25"NOR" ÂI¸gÅç.\n\r", ch );
						ch->exp = ch->exp - 25;
					}
				}
				else if( sklv < 6 )
				{
					send_to_char( "§A¦Û¾Ô°«¤¤°k¶]¡A·l¥¢¤F "HIG"20"NOR" ÂI¸gÅç.\n\r", ch );
					ch->exp = ch->exp - 20;
				}
			}

			//Added by Razgriz 20060822
			ch->attacked = NULL;
			victim->attacked = NULL;

			track_hunt( ch, victim, was_in, door );

			if( victim->level > ch->level/2 + sklv * 2)
				WAIT_STATE( ch, nStun);
			return;
		}

		if( !IS_NPC(ch) )
		{
			if( ch->pcdata->learned[gsn_escape])
			{
				if( int_number_percent +1> int_escape_percent/2 + sklv * 5)
				{
					send_to_char( "¥¢±Ñ¤F¡I §A¥¢¥h¤F "HIG"20"NOR" ÂI¸gÅç.\n\r", ch );
					ch->exp = ch->exp -20;
				}
			}
			else if(sklv < 6 )
			{
				send_to_char( "¥¢±Ñ¤F¡I §A¥¢¥h¤F "HIG"15"NOR" ÂI¸gÅç.\n\r", ch );
				ch->exp = ch->exp -15;
			}
			if( IsFear == 0 ) // ³B²z fear ª¬ºA flee ¥¢±ÑµL­­ stun ªº°ÝÃD 2022/10/13
				WAIT_STATE(ch, 2 + nStun);
			return;
		}
		return;
	}
	else 
	{
		EXIT_DATA *pexit;
		if( !str_prefix(arg, "up"))
		{
			dir = DIR_UP;
		}
		else if ( !str_prefix(arg, "down"))
		{
			dir = DIR_DOWN;
		}
		else if ( !str_prefix(arg, "north"))
		{
			dir = DIR_NORTH;
		}
		else if ( !str_prefix(arg, "west"))
		{
			dir = DIR_WEST;
		}
		else if ( !str_prefix(arg, "east"))
		{
			dir = DIR_EAST;
		}
		else if ( !str_prefix(arg, "south"))
		{
			dir = DIR_SOUTH;
		}
		else 
		{
			send_to_char("¨S¦³³o­Ó¤è¦V¡C\n\r", ch);
			return;
		}

		if( ( pexit = was_in->exit[dir] ) == 0
			|| !pexit->to_room
			|| ( IS_SET( pexit->exit_info, EX_CLOSED )
				&& !IS_AFFECTED( ch, AFF_PASS_DOOR )
				&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) 
				&& ! IS_SET( pexit->exit_info, EX_PASSPROOF ))
			|| ( IS_NPC( ch )
				&& ( IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB )
					|| ( IS_SET( ch->act, ACT_STAY_AREA )
						&& pexit->to_room->area != ch->in_room->area ) ) ) )
		{
			send_to_char("¨º­Ó¤è¦V¨S¦³¸ô¡I\n\r", ch);
			return;
		}

		if( victim->level > 40 || (!IS_NPC(victim) && victim->level > 34 ))
		{
			if( int_number_percent > ( IS_NPC(ch) ?  mob_adjust:adjust ) )
			{
				send_to_char( "§A¸ÕµÛ°k¶]¡A¦ý«o³Q¹ï¤âªý¾×¤U¨Ó¡I\n\r", ch );
				send_to_char("§Aªý¾×¹ï¤â°k¶]ªº·N¹Ï¡I\n\r", victim);

				if( !IS_NPC(ch) )
				{
					if( ch->pcdata->learned[gsn_escape])
					{
						if( int_number_percent + 1 > int_escape_percent / 2 + sklv * 5)
						{
							send_to_char( "¥¢±Ñ¤F¡I §A¥¢¥h¤F "HIG"20"NOR" ÂI¸gÅç.\n\r", ch );
							ch->exp -= 20; 
						}
					}
					else if(sklv < 6 )
					{
						send_to_char( "¥¢±Ñ¤F¡I §A¥¢¥h¤F "HIG"15"NOR" ÂI¸gÅç.\n\r", ch );
						ch->exp -= 15;
					}
					if( IsFear == 0 ) // ³B²z fear ª¬ºA flee ¥¢±ÑµL­­ stun ªº°ÝÃD 2022/10/13
						WAIT_STATE(ch, 2 + nStun);
					return;
				}
				return;
			}
		}

		move_char( ch, dir);
		if( ( now_in = ch->in_room ) == was_in )
		{
			if( ch->fighting ) stop_fighting(ch, FALSE);
			if( victim->fighting ) stop_fighting(victim, FALSE);
			set_fighting(ch, victim);
			set_fighting(victim, ch);
			return;
		}

		ch->in_room = was_in;
		act( "$n °k¨«¤F¡I", IS_SET( ch->act, PLR_HORSE ) && ch->desc ? ch->pcdata->horse : ch, NULL, NULL, TO_ROOM );
		ch->in_room = now_in;

		if( !IS_NPC(ch) )
		{
			if( ch->pcdata->learned[gsn_escape])
			{
				if( int_number_percent +1> int_escape_percent/2 + sklv * 2)
				{
					send_to_char( "§A¦Û¾Ô°«¤¤°k¶]¡A·l¥¢¤F "HIG"25"NOR" ÂI¸gÅç.\n\r", ch );
					ch->exp=ch->exp-25;
				}
			}
			else 
			{
				send_to_char( "§A¦Û¾Ô°«¤¤°k¶]¡A·l¥¢¤F "HIG"20"NOR" ÂI¸gÅç.\n\r", ch );
				gain_exp( ch, -20 );
			}
		}

		//Added by Razgriz 20060822
		ch->attacked = NULL;
		victim->attacked = NULL;

		track_hunt( ch, victim, was_in, dir);

		if( victim->level > ch->level/2)
			WAIT_STATE( ch, nStun);
		return;
	}
}

CHAR_DATA * fighting_by( CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *room ) 
{
	CHAR_DATA *fch = NULL;
	for( fch = room->people ; fch ; fch = fch->next_in_room ) {
		if( fch->fighting == ch && fch != victim )
			return fch;
	}
	return fch;
}

void track_hunt ( CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door )
{
	CHAR_DATA *vch, *hch, *rch, *vch_next_in_room;
	if(!ch || !victim || !victim->in_room || !ch->in_room || !was_in) return;

	for( vch = was_in->people; vch; vch = vch_next_in_room )
	{
		vch_next_in_room = vch->next_in_room;
		if ( vch != victim || vch->deleted || !can_see( vch, ch ) )
			continue;

		if ( IS_NPC( vch ) && IS_SET( vch->act, ACT_HUNT ) )
		{
			if ( ( hch = vch->hunting ) )
			{
				if ( hch->hunted == vch )
					hch->hunted = vch->next_in_hunt;
				else
					for( rch = hch->hunted; rch; rch = rch->next_in_hunt )
						if ( rch->next_in_hunt == vch )
							rch->next_in_hunt = vch->next_in_hunt;
				act( "$N °±¤îÂy±þ§A¡C", hch, NULL, vch, TO_CHAR );

				vch->hunting = ch;
				vch->next_in_hunt = ch->hunted;
				ch->hunted = vch;
				act( "§A¶}©lÂy±þ $N!\n\r", vch, NULL, ch, TO_CHAR);
				act( "$n ¶}©lÂy±þ§A¡I\n\r", vch, NULL, ch, TO_VICT);
			}
			else
			{
				vch->hunting = ch;
				vch->next_in_hunt = ch->hunted;
				ch->hunted = vch;
				act( "$n ¶}©lÂy±þ§A¡I\n\r", vch, NULL, ch, TO_VICT);
			}
		}

		if ( IS_AFFECTED( vch, AFF_PARALYZED ) ) {
			return;
		}
		// affect stone form ­×¥¿ 2022/12/04
		if( is_affected(vch, gsn_stone_form) ){
			return;
		}
		if ( IS_AFFECTED( vch, AFF_HOLD ) ) {
			return;
		}
		if( auction_list ) {
			AUC_DATA * auction = NULL;
			for(auction = auction_list ; auction; auction = auction->next ){
				if( auction->owner == vch || auction->buyer == vch )
					return;
			}
		}
		if (    ( IS_NPC( vch ) && !IS_SET( vch->act, ACT_TRACK ) ) 
			||  ( !IS_NPC( vch ) && ( vch->tracking != ch || number_percent() > get_skill_percent(vch, gsn_track) - get_skill_level(ch,gsn_escape) * 3 ) ) )
			continue;
		if( IS_NPC(vch) && number_percent() < get_skill_level(ch, gsn_escape) * 3 ) continue;
		if( fighting_by( ch, victim, was_in)) continue;

		if ( IS_NPC( vch ) && IS_SET( vch->act, ACT_TRACK ) )
		{
			if ( vch->track_times >= 6 )
			{
				ch->tracked = vch->next_in_track;
				vch->tracking = NULL;
				vch->next_in_track = NULL;
				act( "§A°±¤î¹ï $N ªº°lÂÜ¡C\n\r", vch, NULL, ch, TO_CHAR);
				act( "$n °±¤î¹ï§Aªº°lÂÜ¡C\n\r", vch, NULL, ch, TO_VICT);
				vch->track_times = 0;
				return;
			}
			else
			{
				act( "$n °lÂÜµÛ $N ªº¦æ°Ê¡A±q«áÀY°l¤F¹L¥h.", vch, NULL, ch, TO_ROOM );
				act( "§A°lÂÜ $N ªº¦æ°Ê¡I", vch, NULL, ch, TO_CHAR );
				move_char( vch, door );
				vch->track_times += 1;
			}
		}

		if( was_in == vch->in_room ) return;
		if ( ch->in_room == vch->in_room )//ROOM_ENTRY_TRIGGER may mptransfer vch!
		{
			stop_fighting(vch, FALSE);
			set_fighting( vch, ch );
			if ( ch->position != POS_FIGHTING )
				set_fighting( ch, vch );
		}
		//track_hunt( vch, vch->fighting, was_in, door );
		vch_next_in_room = was_in->people;//vch->next_in_room may tracking and leave
	}
}


void do_sla( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *rch;

	rch = get_char( ch );

	if ( !authorized( rch, "slay" ) )
		return;

	send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
	return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	CHAR_DATA *rch;
	char arg [ MAX_INPUT_LENGTH ];

	rch = get_char( ch );

	if ( !authorized( rch, "slay" ) )
		return;

	one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		send_to_char( "Slay whom?\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
	}

	if ( ch == victim )
	{
		send_to_char( "Suicide is a mortal sin.\n\r", ch );
		return;
	}

	if ( !IS_NPC( victim ) && victim->level >= ch->level )
	{
		send_to_char( "You failed.\n\r", ch );
		return;
	}

	act( "You slay $M in cold blood!",  ch, NULL, victim, TO_CHAR    );
	act( "$n slays you in cold blood!", ch, NULL, victim, TO_VICT    );
	act( "$n slays $N in cold blood!",  ch, NULL, victim, TO_NOTVICT );
	raw_kill( ch, victim );
	return;
}


void do_fee( CHAR_DATA *ch, char *argument )
{
	send_to_char( "¦pªG§A·Q­n§l¦å¡A§A±o§â¥¦¥´§¹¥þ¡C\n\r", ch );
	return;
}

/* Vampiric bite.  Feeding */
void do_feed( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];
	int dam, nDamCH;
	int nSklv = get_skill_level(ch, gsn_inborn_ability);
	int nHitRate;
	int nHitRoll, nDamRoll;

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§AµLªk¦b¥Û¤Æª¬ºA¤U¨Ï¥Î§Þ¥©.\n\r", ch);
		return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' && !ch->fighting  )
	{
		send_to_char( "·Q«r½Ö?\n\r", ch );
		return;
	}

	if( arg[0] == '\0' && ch->fighting && ch->fighting->in_room &&
			ch->fighting->in_room != ch->in_room )
	{
		send_to_char( "·Q«r½Ö?\n\r", ch );
		return;
	}

	if ( !( victim = ch->fighting ) )
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "³o¸Ì¨S³o­Ó¤H.\n\r", ch );
			return;
		}

	if( victim->deleted ) return;
	if ( victim == ch )
	{
		send_to_char( "§Aªº¼L«r¤£¨ì¦Û¤vªº³ïÄV.\n\r", ch );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;
	check_killer( ch, victim );

	send_to_char( "§A¦n·Q³ÜÂIÂA¦å!\n\r", ch );
	act( "$n°§´÷¦a¦V$Nªº³ïÄV«r¥h!", ch, NULL, victim, TO_NOTVICT );
	act( "$n°§´÷¦a«r¦V§Aªº³ïÄV!", ch, NULL, victim, TO_VICT );

	// «D§l¦å°­ªººØ±Ú¥Î feed ¤@©w¤£©R¤¤
	if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_VAMPIRE ) )
	{
		send_to_char( "§A¨S¦³«r¨ì!\n\r", ch );
		// ¦¹³B¦s¦b°ÝÃD, ·í ch ¦b¾Ô°«¤¤ feed «D¾Ô°«¤¤ªº victim ®É, victim ¤£·|Ä²µo¹ï ch ªº¾Ô°«
		// ¦ý«D§l¦å°­¨¤¦âªº feed ¥Ã»·¤£³y¦¨¶Ë®`, ¾Ô°«¤¤ feed «D¾Ô victim ¥u·|³y¦¨¦Û¤v stun
		if ( !ch->fighting )
		{
			set_fighting( ch, victim );
			set_fighting( victim, ch );			//Added by Razgriz	20050619
			// npc ¥H feed ¶}¾Ôªº²Ä¤@¦^¦X·|Ä²µo fight_prog 2022/07/11
			if( IS_NPC(ch) )
				mprog_fight_trigger( ch , victim);
			// npc ³Q feed ¶}¾Ôªº²Ä¤@¦^¦X·|Ä²µo fight_prog 2022/07/11
			if( IS_NPC(victim) && !victim->fighting )
				mprog_fight_trigger( victim , ch);
			WAIT_STATE( ch , 4 );
		}
		return;
	}

	if(ch->move < 10 + nSklv * 5){
		send_to_char( "§A¤Ó²Ö¤F!\n\r", ch );
		return;
	}
	ch->move -= 10 + nSklv * 5;

	nHitRoll = get_hitroll(ch, (get_eq_char(ch, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) );
	nDamRoll = get_damroll(ch, (get_eq_char(ch, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) );
	if(IS_NPC(victim)){
		nHitRate = (nHitRoll + nSklv * (4 + nSklv) * 3 / 4 + GET_DODGE(victim) / 30)
		           * weakness_percent(ch, gsn_vampiric_bite) / 100;
	}else{
		nHitRate = (nHitRoll + nSklv * (4 + nSklv) / 2 + GET_DODGE(victim) / 10)
		           * weakness_percent(ch, gsn_vampiric_bite) / 100;
	}

	int nEdSklv = get_skill_level(ch, gsn_enhanced_damage);

	/* If the victim is asleep, he deserves to be bitten.  :) */
	// ²¾°£§P©w awake ªº³]©w, ­ì³]©w¦b ch ¶i¤J¾Ô°«¥B victim sleep ªº±¡ªp¤U victim ¤£·|¹ï¶Ë®`§@¥X¤ÏÀ³ 2022/07/11
	//if(IS_AWAKE( victim ) && number_percent() >	nHitRate)
	if( number_percent() >	nHitRate )
	{
		if(GET_DODGE(victim) < 0){
			nHitRate = nHitRoll * 1200 / (1000 - GET_DODGE(victim));
		}else{
			nHitRate = nHitRoll * (1200 + GET_DODGE(victim)) / 1000;
		}
		if(nHitRate < 0) nHitRate = 0;
		if(!IS_NPC(victim)) nHitRate = nHitRate * 2 / 3;
		if(number_percent() > nHitRate + nSklv){
			WAIT_STATE(ch, 5);
			send_to_char( "§A¨S¦³«r¨ì!\n\r", ch );
			set_fighting( ch, victim );
			set_fighting( victim, ch );			//Added by Razgriz	20050619
			// npc ¥H feed ¶}¾Ôªº²Ä¤@¦^¦X·|Ä²µo fight_prog 2022/07/11
			if(!ch->fighting && IS_NPC(ch))
				mprog_fight_trigger(ch , victim);
			// npc ³Q feed ¶}¾Ôªº²Ä¤@¦^¦X·|Ä²µo fight_prog 2022/07/11
			if(!victim->fighting && IS_NPC(victim))
				mprog_fight_trigger(victim , ch);
			return;
		}
	}

	dam = (nDamRoll + nDamRoll * abs(nDamRoll) / 100) / 2;
	if(dam > 200) dam = 200;
	dam	= number_range(dam * (5 + nSklv), dam * (7 + nSklv * 2));
	dam = dam * (1600 + get_skill_percent(ch, gsn_enhanced_damage) * nEdSklv) / 2000;
	if(get_skill_level(ch, gsn_enhanced_damage) >= 4) dam *= 1.05;
	if(get_skill_level(ch, gsn_enhanced_damage) >= 6) dam *= 1.10;
	dam = dam * (30 + get_curr_str(ch)) / 50;

	if(dam > 1000 + nSklv * 100 + get_skill_level(ch, gsn_enhanced_damage) * 100)
		dam = 1000 + nSklv * 100 + get_skill_level(ch, gsn_enhanced_damage) * 100;

	if(IS_NPC(ch)) dam /= 3;

	if(IS_NPC(victim)){
		if(GET_AC(victim) < 0){
			dam = dam * 2000 / (2000 - GET_AC(victim));
		}else{
			dam = dam * (2000 + GET_AC(victim)) / 2000;
		}
	}else{
		if(GET_AC(victim) < 0){
			dam = dam * 750 / (750 - GET_AC(victim));
		}else{
			dam = dam * (750 + GET_AC(victim)) / 750;
		}
	}
	dam += ch->apdr * (12 + nSklv * 3 + nEdSklv * 2 + nSklv * nEdSklv / 2) / 10;
	dam -= victim->pdr;
	dam  = ( dam * weakness_damage(ch, gsn_vampiric_bite) ) / 100;
	if(dam < 0) dam = 0;
	nDamCH  = get_critical_damage(dam, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
	damage ( ch, victim, nDamCH, gsn_vampiric_bite, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );

	// ª`·N victim ¬O§_¤w¦º
	if ( victim->in_room == ch->in_room && 15 + nSklv + nSklv * nSklv / 4 > number_percent( ) ){
		nDamCH	= get_critical_damage(dam, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
		damage( ch, victim, nDamCH, gsn_vampiric_bite, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}

	/* Lets see if we can get some food from this attack */
	if ( !IS_NPC( ch ) && number_percent( ) < ( 25 - victim->level + ch->level + nSklv * 5 + nSklv * nSklv / 2) )
	{
		// affect by no heal wouldn't gain hitpoint add at 2018.11.20
		if(number_percent() <= 24 + nSklv * nSklv && !is_affected(ch, AFF_NO_HEAL ))
		{
			send_to_char( "°Ú¡I ¬ü¨ýªºÂA¦å¡A´þ¾i¤F§Aªº¥Í©R¡I\n\r", ch );
			if(IS_NPC(victim))
				ch->hit = UMIN( ch->max_hit * 11 / 10, ch->hit + dam / 4);
			else
				ch->hit = UMIN( ch->max_hit * 11 / 10, ch->hit + dam / 20);
		}else{
			send_to_char( "°Ú¡I ¬ü¨ýªºÂA¦å¡I\n\r", ch );
		}
		// ­×§ï gain_condition ¤£»Ý­n§P©w ·í«e¼Æ­È 2023/01/13
		//if ( ch->pcdata->condition[COND_FULL  ] > 40 && ch->pcdata->condition[COND_THIRST] > 40)
		//{ ; }else
		gain_condition( ch, COND_FULL, nSklv*5 + 5 );
		gain_condition( ch, COND_THIRST, nSklv*5 + 5 );
	}

	WAIT_STATE( ch, 10 );
	return;

}

void do_stake( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA  *obj;
	AFFECT_DATA  af;
	char arg [ MAX_INPUT_LENGTH ];
	bool found = FALSE;
	int chance;

	if ( !check_blind( ch ) )
		return;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "Stake whom?\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "They are not here.\n\r", ch );
		return;
	}

	if ( IS_NPC( victim ) )
	{
		send_to_char( "You may not stake NPC's.\n\r", ch );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;
	check_killer( ch, victim );

	/* Check here to see if they have a stake in hand */

	for ( obj = ch->carrying; obj; obj = obj->next_content )
	{
		if ( obj->pIndexData->vnum == OBJ_VNUM_STAKE && obj->wear_loc == WEAR_WIELD )
		{
			found = TRUE;
			break;
		}
	}

	if ( !found )
	{
		send_to_char( "You lack a primary wielded stake to stake.\n\r", ch );
		act( "$n screams and lunges at $N then realizes $e doesnt have a stake", ch, NULL, victim, TO_ROOM );
		multi_hit( ch, victim, TYPE_UNDEFINED );
		return;
	}

	act( "$n screams and lunges at $N with $p.", ch, obj, victim, TO_NOTVICT );
	act( "You scream and lunge at $N with $p.", ch, obj, victim, TO_CHAR );
	act( "$n screams and lunges at you with $p.", ch, obj, victim, TO_VICT );

	/* Calculate chances, give vampire the ghoul effect,
	autokill the vampire. */
	chance = ( IS_NPC( ch ) ? ch->level : ch->pcdata->learned[gsn_stake] );
	chance = chance - victim->level + ch->level;

	if ( number_percent( ) < chance
		|| !IS_AWAKE( victim )
		|| victim->race != race_lookup( "Vampire" ) )
	{
		if ( victim->race == race_lookup( "vampire" ) )
		{
			damage( ch, victim, victim->hit + 11, gsn_stake, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );

			af.type  = gsn_stake;
			af.duration  = 10;
			af.location  = APPLY_NONE;
			af.modifier  = 0;
			af.bitvector = AFF_GHOUL;
			affect_join( victim, &af );
		}
		else
		{
			multi_hit( ch, victim, TYPE_UNDEFINED );
		}
	}
	else
	{
		do_feed( victim, ch->name );
	}

	return;
}



/* Mobs using magical items by Spitfire from merc mailing list */
/* Modified to give every magical item an equal chance of being used plus
 * eating pills -Kahn */
void use_magical_item( CHAR_DATA *ch )/*{{{*/
{
	OBJ_DATA *obj;
	OBJ_DATA *cobj = NULL;
	int number = 0;
	char buf[ MAX_INPUT_LENGTH ];

	for ( obj = ch->carrying; obj; obj = obj->next_content )
	{
		if( (      obj->item_type == ITEM_SCROLL
				|| obj->item_type == ITEM_WAND
				|| obj->item_type == ITEM_STAFF
				|| obj->item_type == ITEM_PILL )
			&& number_range( 0, number ) == 0 )
		{
			cobj = obj;
			number++;
		}
	}

	if ( !cobj )
		return;

	switch( cobj->item_type )
	{
		case ITEM_SCROLL:
			do_recite( ch, "scroll" );
			break;
		case ITEM_WAND:
			if ( cobj->wear_loc == WEAR_HOLD )
			do_zap( ch, "" );
			break;
		case ITEM_STAFF:
			if ( cobj->wear_loc == WEAR_HOLD )
			do_brandish( ch, "" );
			break;
		case ITEM_POTION:
			do_quaff( ch, "potion" );
			break;
		case ITEM_PILL:
			sprintf( buf, "%s", cobj->name );
			do_eat( ch, buf );
			break;
	}
	return;

}/*}}}*/

int get_align( CHAR_DATA *ch )
{
	if( IS_EVIL( ch ) )
		return 0;
	if( IS_GOOD( ch ) )
		return 1;
	else
		return 2;
}

void trigger_obj_hp( OBJ_DATA *obj, int sn,int level, CHAR_DATA *ch, void* vo)
{
	CHAR_DATA *victim = NULL;

	//Added by Razgriz 20051005
	//Á×§K murder self ²£¥Íªº artifact µL³d¥ô³sµoªº bug
	if ( ch->fighting && ch->fighting == ch )
		return;

	// check whether the class/race/sex can use the hp or not
	if( obj->pIndexData->hp_use_class ){
		if(    ( IS_SET( obj->pIndexData->hp_use_class, ALLOW_BIT ) && !IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ch->class ) ) )
			|| ( IS_SET( obj->pIndexData->hp_use_class, DENY_BIT ) && IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ch->class ) ) ) )
		return;
	}
	if( obj->pIndexData->hp_use_race ){
		if(    ( IS_SET( obj->pIndexData->hp_use_race, ALLOW_BIT ) && !IS_SET( obj->pIndexData->hp_use_race, NUM_BIT( ch->race ) ) )
			|| ( IS_SET( obj->pIndexData->hp_use_race, DENY_BIT ) && IS_SET( obj->pIndexData->hp_use_race, NUM_BIT( ch->race ) ) ) )
		return;
	}
	if( obj->pIndexData->hp_use_sex ){
		if(    ( IS_SET( obj->pIndexData->hp_use_sex, ALLOW_BIT ) && !IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( ch->sex ) ) )
			|| ( IS_SET( obj->pIndexData->hp_use_sex, DENY_BIT ) && IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( ch->sex ) ) ) )
		return;
	}
	if( obj->pIndexData->hp_use_sex ){
		if(    ( IS_SET( obj->pIndexData->hp_use_align, ALLOW_BIT ) && !IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( get_align( ch ) ) ) )
			|| ( IS_SET( obj->pIndexData->hp_use_align, DENY_BIT ) && IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( get_align( ch ) ) ) ) )
		return;
	}

	switch( skill_table[sn].target )
	{
		default:
			victim = ch;
			break;
		case TAR_CHAR_OFFENSIVE:
			victim = (CHAR_DATA*) vo;
			break;
	}

	if( !victim || victim->deleted)
		return;
	obj_cast_spell( sn, level , ch, victim, obj );
}



//show the graphy fighting prompt by Neversay 2004/June/23

void show_vs_graphy(CHAR_DATA *ch, CHAR_DATA *victim){
  	CHAR_DATA *vch;
  	char *victim_symbol[] = {"  ","¡Å","¢b","¢c","¢d","¢e","¢f","¢g","¢h","¢i","¡»"},
     	*color_of_victim = HIW,
     	buffer[512] = "\0",
     	state[256] = "|",
     	unknow_state[] = HIB"**"NOR"|";
  	int victim_hp_remain = (victim->hit*10)/victim->max_hit,
    	fighting_number = 0;

  //player "config +status" command check.
  	if(!IS_SET(ch->act, PLR_STATUS))       return;
  	if(!victim || victim->deleted ) return;
	if(!ch || !ch->in_room || ch->deleted ) return;


  	if(victim_hp_remain > 10) victim_hp_remain = 10;
  	 
  	//counting the number of figthing enemy directly face to you!
  	for(vch = ch->in_room->people; vch; vch = vch->next_in_room)
    	if( vch && vch->fighting == ch)
      		fighting_number++;

  if(IS_AFFECTED(victim, AFF_SANCTUARY ))
    strcat(state, HIW"¸t"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim, AFF_FAERIE_FIRE))
    strcat(state, HIP"¯»"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim, AFF_TARDY ) )
    strcat(state, HIY"Ãú"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim,AFF_MYSTIC_ARMOR))
    strcat(state, HIB"¾À"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_mana_shield))
    strcat(state, HIU"¬Þ"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_mystic_mirror))
    strcat(state, HIW"Ãè"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_flaming_armor))
    strcat(state, HIP"µK"NOR"|");
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim, AFF_CRAZY ))
    strcat(state, CYN"ºÆ"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_blindness))
    strcat(state, HIB"ª¼"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_flying( victim ))
    strcat(state, HIG"­¸"NOR"|");   //no problem
  else strcat(state, unknow_state);


  //The fuzzy range is +-10% when the victim is approaching the death.
  if(victim_hp_remain <= 5){
    if(number_bits(3) == 1)
      victim_hp_remain++;
    if((victim_hp_remain > 0) && (number_bits(3) == 2))
      victim_hp_remain--;
  }

  if(victim_hp_remain < 7 && victim_hp_remain > 4) color_of_victim = HIY;
  if(victim_hp_remain < 4 && victim_hp_remain > 0) color_of_victim = HIR;
  if(victim_hp_remain < 0) color_of_victim = NOR;

  snprintf(buffer, 512, "|%2d%s%s"BBLU"%s"NOR"%s%s"NOR, 
      fighting_number, state, 
      color_of_victim, victim_symbol[victim_hp_remain], 
      color_of_victim, victim->short_descr);

  act( buffer, ch, NULL, victim, TO_CHAR);
}


void death_declare(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MAX_STRING_LENGTH *2];
	if ( !IS_NPC( victim ) && victim->level > 3 && !IS_SET( victim->act, PLR_HORSE ) && victim != ch && !IS_IMMORTAL(ch))
	{
		if ( ch->sex == SEX_FEMALE )
		{
			switch ( number_bits(3) )
			{
				case 0: sprintf( buf, ""HIR"¤j¥D±ÐÂL©g¨È(Dynia)¼¦±¤¦a«Å¥¬¡G¡u%s¤£©¯¾Ô¦º¨F³õ¡A¦³¤H¯àÀ°§U¥L¶Ü¡H¡v"NOR"\n\r",
										victim->name );
						break;
				case 1: sprintf( buf, ""HIR"%s"HIR"¤j³Û¡G¡uµL¯àªº«_ÀIªÌ%s³¯«Í©ó%s¡A¦³½ÖÄ@·NÀ°§U¥L¡H¡v"NOR"\n\r",
										ch->short_descr, victim->name, ch->in_room->name );
						break;
				case 2: sprintf( buf, ""HIR"¦º¯«®´¨Ó¤@¥÷°T®§¡G¡u%s´dºG¦a³Q%s"HIR"±þ±¼¤F¡A½Ð¤j®aÀ°§U¥L¡C¡v"NOR"\n\r",
										victim->name, ch->short_descr );
						break;
				case 3: sprintf( buf, ""HIR"%s"HIR"Å¸§Ý¦a°ª³Û¡G¡uÁV¿|¡A§Ú¥¢¤â§â%s±þ¦º¤F¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 4: sprintf( buf, ""HIR"%s"HIR"¦y¥sµÛ¡G¡u%s¦³¤@¨ã%sªº«ÍÅé¡A³ìÂÅ¦h(Joe Rando)³Â·Ð§Ö¦¬¨«¦n¶Ü¡C¡v"NOR"\n\r",
										ch->short_descr, ch->in_room->name, victim->name );
						break;
				case 5: sprintf( buf, ""HIR"%s"HIR"©IÆ~µÛ¡G¡u%s§AÁÙ¤Ó¹à¤F¡A½Ð§O¨Ó¥´ÂZ§Úªº¥Í¬¡¡C¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 6: sprintf( buf, ""HIR"%s"HIR"±{±¤¦a»¡¡G¡u%s¤S³Q±þ±¼¤F¡A§Æ±æ¤U¦¸îî¤§­ß¤ý¬P´¶¾|¦h(Pluto)¯à°÷«O¦ö¥L¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 7: sprintf( buf, ""HIR"%s"HIR"´d±­¦a³Û¡G¡u¦­¤wº¡¤âÂA¦å¡A²{¦b¤S¦h¤F%sªº¤`»î¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
			}
		}
		else
		{
			switch ( number_bits(3) )
			{
				case 0: sprintf( buf, ""HIR"%s"HIR"°ª³ÛµÛ¡G¡u«z«¢«¢¡A¦³½ÖÄ@·NÀ°%s¦¬«Í°Ú¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 1: sprintf( buf, ""HIR"%s"HIR"®¢¯ºµÛ¡G¡uµL¯àªº%s¡A§AÁÙ¦³Áx¨Ó¤@¦¸¶Ü¡H¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 2: sprintf( buf, ""HIR"%s"HIR"¨g¯ºµÛ¡G¡u«z«¢«¢¡A§ÚÀHÀH«K«K´N§â%s±þ±¼¤F¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 3: sprintf( buf, ""HIR"%s"HIR"¨g¯º»¡¡G¡u¥i¼¦ªº%s¡A¦^®a¦V§Aªº¥À¿Ë­ú¶D§a¡C¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 4: sprintf( buf, ""HIR"%s"HIR"¤j³ÛµÛ¡G¡u%s¡A§ÚÁÙ¥i¥H¦A±þ§A¤@¦¸¡A¥u­n§A´±¨Óªº¸Ü¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 5: sprintf( buf, ""HIR"%s"HIR"¤j¯ºµÛ¡G¡u¤Ñ©³¤U©~µMÁÙ¦³¹³%s³o»ò¦Û¤£¶q¤Oªº¤H¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 6: sprintf( buf, ""HIR"%s"HIR"¨g¯º»¡¡G¡u¬ÝµÛ%sÂA¦å¶Ã¼Q¡A¯u¬O¤@ºØµL¤Wªº¨É¨ü¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 7: sprintf( buf, ""HIR"%s"HIR"äú¤ßªº¤j¥s¡G¡u¥´¦º%s¡A´N¹³½ò«ó¤@°¦Á­½¸¤@¼Ë¡A¯u¬Oäú¤ß¡I¡v"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
			}
		}
		send_to_all_char( buf );
	}
}

void pk_kill(CHAR_DATA *ch, CHAR_DATA *victim)
{
  AFFECT_DATA *paf;
  CHAR_DATA   *vch, *rch;
  ROOM_INDEX_DATA * was_in_room;
  char buf[MAX_STRING_LENGTH];

  for ( paf = victim->affected; paf; paf = paf->next )
  {
    if ( paf->deleted )
      continue;

    affect_remove( victim, paf );
  }

  if ( ( vch = victim->hunting ) )
  {
    victim->hunting = NULL;
    if ( vch->hunted == victim )
    {
      vch->hunted = victim->next_in_hunt;
      victim->next_in_hunt = NULL;
    }
    else
      for( rch = vch->hunted; rch; rch = rch->next_in_hunt )
        if ( rch->next_in_hunt == victim )
        {
          rch->next_in_hunt = victim->next_in_hunt;
          victim->next_in_hunt = NULL;
          break;
        }
    act( "$n °±¤îÂy±þ§A", victim, NULL, vch, TO_VICT );
    act( "§A°±¤îÂy±þ $N", victim, NULL, vch, TO_CHAR );
  }

  if ( ( victim->hunted ) )
  {
    for( vch = victim->hunted; vch; vch = rch )
    {
      rch = vch->next_in_hunt;
      vch->hunting = NULL;
      vch->next_in_hunt = NULL;
      act( "$N °±¤îÂy±þ§A", victim, NULL, vch, TO_CHAR );
      act( "§A°±¤îÂy±þ $n", victim, NULL, vch, TO_VICT );
    }
    victim->hunted = NULL;
  }

  if ( ( vch = victim->assassinating ) )
  {
    victim->assassinating = NULL;
    if ( vch->assassinated == victim )
    {
      vch->assassinated = victim->next_in_ass;
      victim->next_in_ass = NULL;
    }
    else
      for( rch = vch->assassinated; rch; rch = rch->next_in_ass )
        if ( rch->next_in_ass == victim )
        {
          rch->next_in_ass = victim->next_in_ass;
          victim->next_in_ass = NULL;
          break;
        }
    act( "§A°±¤î·t±þ $N", victim, NULL, vch, TO_CHAR );
  }

  if ( ( victim->assassinated ) )
  {
    for( vch = victim->assassinated; vch; vch = rch )
    {
      rch = vch->next_in_ass;
      vch->assassinating = NULL;
      vch->next_in_ass = NULL;
      act( "§A°±¤î·t±þ $n", victim, NULL, vch, TO_VICT );
    }
  }

  if ( ( vch = victim->tracking ) )
  {
    victim->tracking = NULL;
    if ( vch->tracked == victim )
    {
      vch->tracked = victim->next_in_track;
    }
    else{
      for( rch = vch->tracked; rch; rch = rch->next_in_track ){
        if ( rch->next_in_track == victim )
        {
          rch->next_in_track = victim->next_in_track;
          break;
        }
      }
    }
    victim->next_in_track = NULL;
    act( "§A°±¤î°lÂÜ $N", victim, NULL, vch, TO_CHAR );
  }

  if ( ( victim->tracked ) )
  {
    for( vch = victim->tracked; vch; vch = rch )
    {
      rch = vch->next_in_track;
      vch->tracking = NULL;
      vch->next_in_track = NULL;
      act( "§A°±¤î°lÂÜ $n", victim, NULL, vch, TO_VICT );
    }
    victim->tracked = NULL;
  }

  if ( ( vch = victim->rembch ) )
    stop_rember(victim, vch);
  //if( ( vch = ch->rembed ))
  //  vch->rembch = NULL;

	// add at 2021/11/21
	if( ( vch = victim->linking ) ){
		stop_linking(victim, vch);
	}

	// add at 2021/11/20
	if( ( vch = victim->cooperating ) ){
		stop_cooperating(victim, vch);
	}
	if( ( vch = victim->cooperator ) ){
		remove_cooperator(victim);
	}

  if ( ( vch = victim->protecting ) )
  {
    stop_protecting(victim, vch);
  }

  if( ( vch = victim->protector )){
    remove_protector(victim);
  }

  if ( victim->pcdata->clanpk )
  {
    int i;
    victim->pcdata->clanpk = 0;
    if ( victim->clan == clan_war->clan_a )
    {
      clan_war->clan_a_num --;
      for(i=0; i < 10; i++)
        if ( victim == clan_war->clan_a_member[i] )
          clan_war->clan_a_member[i] = NULL;
    }
    else if( victim->clan == clan_war->clan_b )
    {
      clan_war->clan_b_num --;
      for(i=0; i < 10; i++)
        if ( victim == clan_war->clan_b_member[i] )
          clan_war->clan_b_member[i] = NULL;
    }
    else {
      bugf("[clanpk]: victim %s clan %s in pk but is not a and b", victim->name, victim->clan->name);
    }
    if ( clan_war->clan_b_num == 0 )
    {
      sprintf( buf, "¾Ô¯«°¨º¸´µ«Å¥¬¡G%s À»±Ñ±j¼Ä %s, Àò±o¥»¦¸¾Ô§Ðªº³Ó§Q¡I\n\r",
          clan_war->clan_a->name, clan_war->clan_b->name );
      send_to_all_char( buf );
      clan_war->clan_a->bank += 1000000;
      clan_war->clan_a->warwin ++;
      clan_war->clan_b->warlose ++;
      for(i=0; i < 10; i++)
        if ( clan_war->clan_a_member[i] )
        {
          clan_war->clan_a_member[i]->pcdata->clanpk = 0;
          clan_war->clan_a_member[i] = NULL;
          clan_war->clan_b_member[i] = NULL;
        }
      clan_war->fighting = 0;
      clan_war->clan_a = NULL;
      clan_war->clan_b = NULL;
      clan_war->clan_a_num = 0;
    }
    else if ( clan_war->clan_a_num == 0 )
    {
      sprintf( buf, "¾Ô¯«°¨º¸´µ«Å¥¬¡G%s À»±Ñ±j¼Ä %s, Àò±o¥»¦¸¾Ô§Ðªº³Ó§Q¡I\n\r",
          clan_war->clan_b->name, clan_war->clan_a->name );
      send_to_all_char( buf );
      clan_war->clan_b->bank += 1000000;
      clan_war->clan_b->warwin ++;
      clan_war->clan_a->warlose ++;
      for(i=0; i < 10; i++)
        if ( clan_war->clan_b_member[i] )
        {
          clan_war->clan_b_member[i]->pcdata->clanpk = 0;
          clan_war->clan_b_member[i] = NULL;
          clan_war->clan_a_member[i] = NULL;
        }
      clan_war->fighting = 0;
      clan_war->clan_a = NULL;
      clan_war->clan_b = NULL;
      clan_war->clan_b_num = 0;
    }
  }

  if ( IS_SET( victim->act, PLR_KILLER ) )
    REMOVE_BIT( victim->act, PLR_KILLER );

  victim->hit     = victim->max_hit;
  victim->mana    = victim->max_mana;
  victim->move    = victim->max_move;

  was_in_room = victim->in_room;
  if( get_room_index(was_in_room->area->death)){
    char_from_room( victim );
    char_to_room( victim, get_room_index(was_in_room->area->death ));
  }
  else if( get_room_index(was_in_room->area->recall)){
    char_from_room( victim );
    char_to_room( victim, get_room_index(was_in_room->area->recall));
  }

  //    if ( !IS_NPC(victim) && IS_SET( victim->act, PLR_HORSE ) && 
  //IS_SET(victim->pcdata->horse->pcdata->ride, 2))
  //    {
  //    char_from_room(victim->pcdata->horse);
  //  char_to_room(victim->pcdata->horse,get_room_index(was_in_room->area->recall));
  //  }

  victim->position   = POS_RESTING;
  stop_fighting(victim, TRUE);
  do_look(victim, "auto");

  WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
}
