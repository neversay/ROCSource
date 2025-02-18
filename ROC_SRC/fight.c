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
//*  PURPOSE     :         修改 do_feed，不管種族為何             *
//*              :         在 autoc stop 的情形下，feed 沒中時，  *
//*              :         依然會開戰                             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940701 jye                                    *
//*  PURPOSE     :         修改 do_kill 拿掉下kill指令後會stun    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940713 jye                                    *
//*  PURPOSE     :         修改 group_gain obj 損壞後掉在ppl身上  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940723 jye                                    *
//*  PURPOSE     :         修改 do_flee 的條件                    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940909 Razgriz                                *
//*  PURPOSE     :         修改 violence_update(),在 ch->wait 為 0*
//*              :         時, 移掉 qmark -- earthquake_stun      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940909 Razgriz                                *
//*  PURPOSE     :         重寫 stop_fighting(),一方面解決當 fBoth*
//*              :         為 true 時可能會造成的一些邏輯錯誤, 另 *
//*              :         一方面, 加上對 qmark -- earthquake_stun*
//*              :         的處理, 當停止戰鬥時就移掉此 qmark     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940919 jye                                    *
//*  PURPOSE     :         修改 ride lv4 會吃掉一件eq的bug        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941005 Razgriz                                *
//*  PURPOSE     :         修改 trigger_obj_hp 避免 murder self   *
//*              :         產生的 artifact 無責任連發的 bug       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950714 jye                                    *
//*  PURPOSE     :         修改 xp_compute                        *
//*              :              ppl大於mob等級5級時return 0       *
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

/*  毒襲修改為每下攻擊傷害 2021/11/04
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

void BloodThirstyStopCheck(CHAR_DATA *ch) /* blood thirsty 停止戰鬥後數值判定 2022/02/21 */
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

// obj_fight , 於戰鬥中 obj prog 的判定 2022/05/07
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

// obj_kill , 於戰鬥中 obj prog 的判定 2022/05/08
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
			
			// 執行 obj_fight prog 2022/05/07
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
				// 加入 gargoyle 使用的 stone form 才會回血 2022/12/05
				if( ( affected_value( ch, gsn_stone_form, APPLY_RACE ) == -16 ) && ( ch->race == race_lookup( "Statue" ) ) )
				{
					if( (    !IS_NPC(ch) && !IS_NPC(victim) )
						|| ( !IS_NPC( victim ) && victim->pcdata->clanpk && IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
					{
						//ch->hit += ch->level / 4 + nRaceSklv + nRaceSklv * nRaceSklv / 2;
						ch->hit += ch->level + ( ch->max_hit * ( 2 + nRaceSklv ) ) / 200; // pk 下為 lv + 1~ 4%
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
						send_to_char("你身體的石化狀態漸漸消失了.\n\r"NOR, ch);
						act( "$n"CYN"卸下了雕像般的外殼，從地面振翅飛翔到空中。"NOR, ch, NULL, NULL, TO_NOTVICT );
					}
				}
			}

			//戰鬥中每round一定機率解除mob 逃跑限制 , 2021/12/22
			if( IS_NPC(ch) ){
				if( IS_SET( ch->act, ACT_NO_FLEE ) && number_bits( 1 ) == 1 )
					REMOVE_BIT( ch->act, ACT_NO_FLEE );
			}

			/* Continue cure affect.*/

			// AFF_CONTINUE_CURE 效果每 round 回血 ( bis cure critical)
			if ( IS_AFFECTED( ch, AFF_CONTINUE_CURE ) && !IS_AFFECTED( ch, AFF_NO_HEAL ) )
					ch->hit = UMIN( ch->max_hit, ch->hit + ch->level * 3 );

			// 持續治療術的效果每 round 回血 ( continous heal ) 2022/04/08
			if( is_affected( ch, gsn_cont_heal ) && !IS_AFFECTED( ch, AFF_NO_HEAL ) ){
				if ( affected_value(ch, gsn_cont_heal, APPLY_LEVEL ) > 0 ){
					int ContHeSklv = get_skill_level(ch,gsn_cont_heal); // 有技能等級者強度較高
					int ContHeAff = affected_value(ch, gsn_cont_heal, APPLY_LEVEL );
					if( !IS_NPC(ch) )
						ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * ( 9 + ContHeAff*3 + ContHeSklv ) )/1000 );
					else
						ch->hit = UMIN( ch->max_hit, ch->hit + ( ch->max_hit * ( 10 + ContHeAff*2 ) )/1000 );
				}
			}
			// blood thirsty buff 的 APPLY_SAVING_LIFE 用於不死判定, 每 round 少 1 直到 0 消失 add at 2020/10/16
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
					// 新增結束blood thirsty 時回血及敘述 2021/12/22
					send_to_char("空氣中的血腥味滿足你的嗜血欲望，你腦袋放空不再殺得發狂。\n\r",ch);
					//if(affected_value(ch, gsn_blood_thirsty, APPLY_ADAMAGE) > 0){
					if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) > 0 ){
						ch->hit = UMIN( ch->max_hit ,
										ch->hit + 10 + affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT)/10
												* ( 3 + get_skill_level( ch, gsn_blood_thirsty ) * 2 ) ); 
					}
					ch->wait = 20;
					affect_strip ( ch, gsn_blood_thirsty);
					// blood stun 中移除 berserk 效果 2021/12/26
					if ( is_affected ( ch, gsn_berserk ) )
						affect_strip ( ch, gsn_berserk );
				}
			}

			// sun flash debuff 每 round hr 回復判定, 最多 3 round 消失 2021/11/03
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

			// sprinkle debuff 每 round 行動力回復判定判定 lv 5 3 round , lv 6 4 round 2022/12/29
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


			// paralyze debuff 每 round dc 回復判定 , 最多 4 round 歸 0 2022/06/14
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
					// 當 buff 的 dc 0 以下時, 移除 buff  , 重新加入 buff 使角色短時間內免疫麻痺
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

			// 龍息 debuff 每 round 降低 breath 造成的傷害加成值 2022/04/16
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

			// vampiric bite 出血效果
			if( IS_AFFECTED( ch, AFF_VAMP_BITE ) ){
				if( affected_value(ch, gsn_vampiric_bite, APPLY_DAMAGE_COUNT) > 0 ){
					ch->hit -= affected_value(ch, gsn_vampiric_bite, APPLY_DAMAGE_COUNT);
				}
				ch->hit -= ch->level * 5;
				// 不會被流血致死
				if( ch->hit <= 0 )
					ch->hit = 1;
			}

			/* 
				毒襲修改為每下攻擊觸發追加傷害 , dot 部分與 poison 相同 2021/11/04
				if(is_affected(ch, gsn_venomous_strike)){
					VenomousHurt(ch);
				}
			*/

			/*  修改 adore 效果 2022/11/06
				// 帶有增加最大 hp 的 regeneracy buff 時, 每 round hp 回增加的值,直到滿血後移除buff add at 2020/10/16
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
				// 移除 invoke 判定 , 中毒下改為判定自身毒素是否為負值成立 2021/11/04
				//&& ((is_affected( ch, gsn_invoke ) && get_skill_level(ch, gsn_invoke) > 5) || !IS_AFFECTED(ch, AFF_POISON) )
				&& (     IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
					||   IS_SET( ch->special, NO_POISON )
					|| (!IS_AFFECTED(ch, AFF_POISON))
					|| ( IS_AFFECTED(ch, AFF_POISON) && (NEW_GET_SVS(ch,MAGIC_POISON) <= ch->level) ) ) // poison 基本值為角色 level
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
						// 提高 inborn reg sklv 對 con 常數的影響 2022/04/07
						//if(nRaceSklv > 4) nCurrCon += (nRaceSklv - 4);
						if(nRaceSklv > 3) nCurrCon += (nRaceSklv - 3);
						if(nCurrCon > 25) nCurrCon = 25;
						// inborn reg 受最大 hp 影響 2022/04/07
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
						// nRaceSklv 的效果約為 sklv 的 30%  2022/04/07
						nRegSklv = nRegSklv + sklv * 10;

						if( ( !IS_NPC(ch) && !IS_NPC(victim) ) ||
							( !IS_NPC( victim ) && victim->pcdata->clanpk 
							&& IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
							ch->hit += ( ( UMAX( 1, ( nCurrCon - 10 ) ) * base * factor ) / 100 / 
										( ( victim->level - ch->level > 5 ) ? 4 : 5))/8
										// 加入 inborn reg sklv 影響最大 hp 回血效果 2022/04/07
										// hp 10000 時 sklv 6 有 300 點回血 
										// hp 10000 時 inborn 6 有 90 點回血
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
				// 移出 if 以判定其他的 qmark 2023/01/12
				//QMARK *qmark = NULL;
				if( ( qmark = is_qmark ( ch, "earthquake_stun" )  ) != NULL )
					del_qmark( qmark );

				mprog_hitprcnt_trigger( ch, victim );
				mprog_fight_trigger( ch, victim );
			}

			// 攻擊的目標被 rescue 後無法使用 change 的 qmark 判定 2023/01/12
			if( ( qmark = is_qmark ( ch, "skill_rescue_round" )  ) != NULL ){
				if( qmark->v0 > 0 )
					qmark->v0 -= 1;
			}

			if( !ch->fighting
				|| victim->deleted
				|| ch->in_room != victim->in_room
				|| IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
				continue;

			/*	移入 multi_hit 作判定 2022/10/31

				// 使 mob 不作出普通攻擊的 qmark  2020/05/28
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

		// group 戰鬥判定 , 跑到這代表 ch 沒有在戰鬥中
		for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
		{
			if ( (ch == rch)
				|| rch->deleted
				|| !IS_AWAKE( rch )
				|| !( rch->fighting ) )
				continue;

			// 如果同房間中的 group 成員(rch)在戰鬥, 則跳出程序
			if ( is_same_group( ch, rch ) )
				break;

			// 增加 qmark 阻擋 mob 隨機介入戰鬥 , 並加入 pet 不會介入隨機戰鬥的設定 2022/11/24
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

		// 將 victim 設為 rch->fighting ( rch 的戰鬥目標 )
		victim = rch->fighting;

		/*
		* Now that someone is fighting, consider fighting another pc
		* or not at all.
		* mobfighting 為 TRUE 的情況下, ch ( 此處 ch 必為 npc )非戰鬥中
		* 如果同 room 有其他 npc (rch) 正發生戰鬥, 則 ch 會介入目標等級差 5 以內的戰鬥
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
				//|| ( IS_GOOD( ch ) && IS_GOOD( victim ) ))  移除陣營判定 2022/11/24
				continue;
		}

		// plr_assist 加入只對 pc 判定 2022/11/24
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
		/*  remove at 2020/05/31 縮減不必要的 bug log
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

	// 使開戰的第一回合能直執行 obj_fight prog 2022/05/08
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
					if( is_affected( fch, gsn_blood_thirsty) )  // blood thirsty 停戰後判定 2022/02/21
						BloodThirstyStopCheck(fch);
					//deal with qmark -- earthquake_stun
					if( ( qmark = is_qmark ( fch, "earthquake_stun" )  ) != NULL ) {
						del_qmark( qmark );
						fch->wait = 0;//trade-off
					}
					// 移除 rescue 無法 change 的 qmark 2023/01/12
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
		if( is_affected( tch, gsn_blood_thirsty) )             // blood thirsty 停戰後判定 2022/02/21
			BloodThirstyStopCheck(tch);
		//deal with qmark -- earthquake_stun
		if( ( qmark = is_qmark ( tch, "earthquake_stun" )  ) != NULL ) {
			del_qmark( qmark );
			tch->wait = 0;//trade-off
		}
		// 移除 rescue 無法 change 的 qmark 2023/01/12
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
	bool obj_corpse = FALSE; // 將 pc corpse 基礎 timer 修改為 2 tick , 避免大量惡意堆屍 2022/11/02

	if ( ( killer == ch ) && ( ch->attacked ) )
		killer = ch->attacked;

	if ( IS_NPC( ch ) ) // npc 被殺
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
	else // pc 被殺
	{
		// npc 連續殺死 pc 時會計數, 計數太高會有降低數值的懲罰
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

		// 取消 npc gold 會受等級差影響 2022/11/02
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

			// pc make corpse 時有 50% 機率物品在死後不掉落 2022/11/02
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
			// 死亡後找回屍體前的一段時間內無法使用 backup 2022/10/10
			// 因為 raw_kill 在 make_corpse 後會移除 fDeath 的 qmark , 所以此處設為 del_qmark 後再加入
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
      default: msg  = "你聽見 $n 臨死前的慘叫.";        break;
      case  0: msg  = "$n 倒在地上，吐出最後一口氣...死了.";      break;
      case  1: msg  = "$n 噴出鮮血，濺得你滿身.";   break;
      case  2: msg  = "你聞到一股惡臭.";
           vnum = OBJ_VNUM_FINAL_TURD;        break;
      case  3: msg  = "$n 的頭被砍下，滾落在地.";
           vnum = OBJ_VNUM_SEVERED_HEAD;        break;
      case  4: msg  = "$n 的心臟被挖了出來.";
           vnum = OBJ_VNUM_TORN_HEART;        break;
      case  5: msg  = "$n 的手臂被砍下，掉落在地上.";
           vnum = OBJ_VNUM_SLICED_ARM;        break;
      case  6: msg  = "$n 的腿被砍下，掉落在地上.";
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
    msg = "你聽見一聲慘叫.";
  else
    msg = "你聽見一聲慘叫.";

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
	ch->mod_size = 0; // 體型 2022/12/23

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

	// obj_kill 判定 2022/05/08
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
	// pc 死亡記錄到 bug log , 並加入當下時間 2022/12/11
	if( !IS_NPC( victim ) ){
		bugf("[PC Death]: %s is killed by %s. %s", victim->name, ch->name , (char *) ctime( &current_time ) );
	}
	for ( paf = victim->affected; paf; paf = paf->next )
	{
		if ( paf->deleted )
			continue;

		affect_remove( victim, paf );

		// 加上 dispel_magic function 中，在 affect_remove 後特別針對 polymorph 和 flying 的設定
		if(paf->type == gsn_polymorph || paf->type == gsn_morphing )
		{
			if( paf->type == gsn_morphing )
				victim->temp_race = '\0' ; //NULL;
			dm_effect( victim );
		}
		/* 死亡的狀態下不會受 fall_effect 影響 2023/01/14
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
				send_to_char( "這個對手奮戰不懈的英姿深深映在你的腦海。\n\r", ch );
				act( "$n眼中的光芒更加熾烈了！", ch, NULL, NULL, TO_ROOM );
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
	send_to_char("死而復生後，記得趕緊找回你的屍體和物品，並且要進行 save 的動作。\n\r", victim);
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
        send_to_char( "你的等級太高了.\n\r", gch );
        continue;
    }

    if( gch->level - ch->level <= -6 )
    {
        send_to_char( "你的等級太低了.\n\r",  gch );
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
        send_to_char( "因為隊伍裡有個討厭的傢伙，你損失了六分之一的經驗.\n\r", gch );
        xp -= xp/6;
    }
    
    if( gch && found && gch->class > BARBARIAN && gch == lch )
    {
        send_to_char( "你因為帶領年輕冒險者，多得三分之一的經驗.\n\r", gch );
        xp += xp/3;
    }
    
    if( IS_SET( gch->act, PLR_HORSE ) )
        xp -= xp/3;
        
    if( ch->level > 35 && gch->level < 36 )
        xp /= UMAX( 1,( ch->level - gch->level - members + 3 ));
        
    if( ch->level < 37 )
        xp = xp*3/2;

    sprintf( buf, "你獲得了 "HIG"%d"NOR" 點經驗.\n\r", xp );
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
             act( "$p損壞了.", gch, obj, NULL, TO_CHAR );
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
             act( "你被 $p 給排斥了.", gch, obj, NULL, TO_CHAR );
             act( "$n 被 $p 排斥了.",   gch, obj, NULL, TO_ROOM );
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
             sprintf( buf, "你必須在等級 %d 以內才能使用這樣物品.\n\r",obj->pIndexData->max_level );
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
  //ppl大於mob等級5級時return 0
  //等級跟 victim 在五以內, 才能領到 area writer 設的額外 exp, modified by
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
// 搬到最前面先計算 modified by razgriz 20060716
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
	if ( (get_curr_size( ch ) - get_curr_size( victim ) ) < -2 ) // 替換體型 size 公式 2022/12/23
		return;

	if ( !( obj = get_eq_char( victim, WEAR_WIELD ) ) )
		if ( !( obj = get_eq_char( victim, WEAR_WIELD_2 ) ) )
			if ( !( obj = get_eq_char( victim, WEAR_TWO_HAND ) ) ){
				send_to_char("你的目標並沒有裝備武器！\n\r", ch);
				return;
			}

	// npc disarm 時有 50% 機率不成功 disarm 2022/02/18
	//if( !IS_NPC(victim) && victim->pcdata->learned[gsn_swordmaster] == 0)
	if( IS_NPC(ch) )
	{
		if( 50 > number_percent( ) ){
			send_to_char("你無法擊落對手的武器。\n\r", ch);
			return;
		}
	}

	// 雙持武器有 40% 機率不被 disarm
	if ( get_eq_char( victim, WEAR_TWO_HAND ) && 40 > number_percent( ) )
	{
		send_to_char("對方雙手緊握武器，你無法擊落對方的武器。\n\r", ch);
		return;
	}

	// pc victim 會 swordmaster 的情況
	if( !IS_NPC( victim ) && victim->pcdata->learned[gsn_swordmaster] > 0 ){
		// 將 pc 及 npc 放 disarm 分開判定 2022/02/18
		if( !IS_NPC( ch ) ){
			if( ( get_skill_percent(victim,gsn_swordmaster)/2 + get_skill_level(victim, gsn_swordmaster) * 30
				 + ( victim->class == SWORDSMAN ? get_skill_level(victim, gsn_swordmaster) * 10 : 0 )
				 - get_skill_level(ch, gsn_barehand) * 3 - get_skill_level(ch, gsn_disarm) * 3 )
				 > number_percent( ) )
			{
				if( !is_chinese(ch, victim))
					act( "$N's swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_NOTVICT );
				else
					act( "$N 高超的御劍術防止 $n 擊落$S武器！", ch, NULL, victim, TO_NOTVICT );

				if( !is_chinese(victim, ch))
					act( "Your swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_VICT );
				else
					act( "你高超的御劍術防止 $n 擊落你的武器！", ch, NULL, victim, TO_VICT );
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
					act( "$N 高超的御劍術防止 $n 擊落$S武器！", ch, NULL, victim, TO_NOTVICT );

				if( !is_chinese(victim, ch))
					act( "Your swordmaster skill prevent from $n's disarm!", ch, NULL, victim, TO_VICT );
				else
					act( "你高超的御劍術防止 $n 擊落你的武器！", ch, NULL, victim, TO_VICT );
				return;
			}
		}
	}
	// npc victim 判定
	if( IS_NPC( victim ) ){
		if( IS_SET( victim->special, NO_DISARM )
			|| ( IS_SET( ch->act, ACT_HARD_DIS )
				&& ( victim->level - get_skill_level(ch, gsn_disarm) * 5 - get_skill_level(ch, gsn_barehand) * 5
					> number_percent( ) ) ) )
		{
			act( (!is_chinese(ch, victim)) ?
				"$N's swordmaster skill prevent from $n's disarm!":
				"$N 高超的御劍術防止 $n 擊落$S武器！", ch, NULL, victim, TO_NOTVICT );
			return;
		}
	}

	if ( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) )
		//|| IS_SET( obj->extra_flags, ITEM_NODROP ) ) // nodrop 為不會掉到地上不等於不會從手中放下 , 故移除 2022/02/18
	{
		send_to_char("你無法擊落對方的武器。\n\r", ch);
		return;
	}


	if( IS_NPC( ch ) )
	{
		if(    !IS_SET( ch->class, 32 ) // npc class 32(bloody) 等同於 fist fight 效果 2022/02/18
			&& !get_eq_char( ch, WEAR_WIELD   )
			&& !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) )
		{
			send_to_char("你必須先裝備武器。\n\r", ch);
			return;
		}
	}

	// 修改顯示色碼 2022/11/04
	if( !is_chinese(ch, victim) )
	{
		//act( HIY"You disarm $N!"NOR,  ch, NULL, victim, TO_CHAR  );
		//act( "[1;31m$n DISARMS $N![m",  ch, NULL, victim, TO_NOTVICT );
		act( ""HIY"You disarm "NOR"$N"HIY"'s weapon!"NOR"",  ch, NULL, victim, TO_CHAR );
		act( "$n"HIY" DISARM "NOR"$N"HIY"'s weapon!"NOR"",  ch, NULL, victim, TO_NOTVICT );
	}
	else
	{
		//act( HIY"你擊落了 $N 的武器！"NOR,  ch, NULL, victim, TO_CHAR  );
		//act( "[1;31m$n 擊落了 $N 的武器！[m",  ch, NULL, victim, TO_NOTVICT );
		act( ""HIY"你擊落了 "NOR"$N"HIY" 的武器！"NOR"",  ch, NULL, victim, TO_CHAR );
		act( "$n"HIY" 擊落了 "NOR"$N"HIY" 的武器！"NOR"",  ch, NULL, victim, TO_NOTVICT );
	}

	// 被擊落者會看到自己被擊落的 obj 名 2022/11/04
	if( !is_chinese(victim, ch)){
		//act( "[1;31m$n DISARMS you![m", ch, NULL, victim, TO_VICT  );
		act( "$n"HIY" DISARM your "NOR"$p"HIY" !"NOR,  ch, obj, victim, TO_VICT );
	}else{
		//act( "[1;31m$n 擊落了你的武器！[m", ch, NULL, victim, TO_VICT );
		act( "$n"HIY" 擊落了你的武器 "NOR"$p"HIY" ！"NOR"", ch, obj, victim, TO_VICT );
	}

	obj_from_char( obj );
	if ( IS_NPC( victim )
		|| get_skill_level(victim, gsn_swordmaster) >= 3
		|| IS_SET( obj->extra_flags, ITEM_NODROP ) ) // item_nodrop 不會掉到地上 2022/02/19
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
			act( HIG"$N 巧妙地保持住身體的平衡！"NOR, ch, NULL, victim, TO_CHAR );
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
			act( HIG"$N 巧妙地保持住身體的平衡！"NOR, ch, NULL, victim, TO_CHAR );
		}

		if( !is_chinese(victim, ch)){
			act( HIG"Your bodycontrol skill prevent from $n's trip!"NOR, ch, NULL, victim, TO_VICT );
		}else{
			act( HIG"你巧妙地保持了身體的平衡！"NOR, ch, NULL, victim, TO_VICT );
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
			act( HIY"你絆倒了 $N！"NOR, ch, NULL, victim, TO_CHAR );
			act( HIY"$n 把 $N 絆倒在地！"NOR, ch, NULL, victim, TO_NOTVICT );
		}

		if( !is_chinese(victim, ch)){
			act( HIY"$n trips you and you go down!"NOR, ch, NULL, victim, TO_VICT );
		}else {
			act( HIY"$n 把你絆倒在地！"NOR, ch, NULL, victim, TO_VICT );
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
		send_to_char("你無法在石化狀態主動攻擊.\n\r", ch);
		return;
	}

	if ( arg[0] == '\0' )
	{
		send_to_char( "要殺誰？\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "這裡沒有這個人.\n\r", ch );
		return;
	}
	if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)){
		send_to_char("這裡禁止戰鬥。\n\r", ch);
		return ;
	}
	if ( victim == ch )
	{
		send_to_char( "請使用 MURDER ！\n\r", ch );
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
			send_to_char( "請使用 MURDER.\n\r", ch );
			return;
		}
	}
	else
	{
		if (( IS_AFFECTED( victim, AFF_CHARM ) || IS_AFFECTED( victim, AFF_SUMMONED) ) && victim->master && !IS_NPC(victim->master) )
		{
			send_to_char( "這隻生物是有主人的，請使用 MURDER.\n\r", ch );
			return;
		}
	}


	if ( is_safe( ch, victim ) )
		return;

	if ( IS_AFFECTED( ch, AFF_CHARM ) && !IS_NPC( victim ) )
	{
		send_to_char( "你無法攻擊別人！\n\r", ch );
		return;
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "你已經在戰鬥中了！\n\r", ch );
		return;
	}

	//WAIT_STATE( ch,  PULSE_VIOLENCE / 2);

	multi_hit( ch, victim, TYPE_UNDEFINED );

	// 針對 artifact 的情況, 讓被 sleep 及charm 的目標會確實停止攻擊，避免造成crash
	if( IS_AFFECTED( victim, AFF_SLEEP )) return;
	if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch ) return; // 加入 master 判定 2022/12/31

	if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
		stop_fighting(victim, FALSE);

	// victim 開戰第一回合就死亡則後續不判定, 以免 ch 跟空氣打架 2022/07/14
	if( !victim || victim->deleted ) return;

	// 目標逃跑後 ch 會跟空氣打架 故移除 2022/07/15
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
		send_to_char("你無法在石化狀態主動攻擊.\n\r", ch);
		return;
	}

	if ( arg[0] == '\0' )
	{
		send_to_char( "Murder 誰？\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "這裡沒有這個人.\n\r", ch );
		return;
	}

	if(victim == NULL) return;

	if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)){
		send_to_char("這裡禁止戰鬥。\n\r", ch);
		return ;
	}

	if ( victim == ch )
	{
		send_to_char( "你對自己發動攻擊！\n\r", ch );
		multi_hit( ch, ch, TYPE_UNDEFINED );
		return;
	}

	if( !IS_SET( ch->act, PLR_REGISTER ) && (ch->in_room && !IS_SET(ch->in_room->room_flags, ROOM_PK))) {
		send_to_char("你並沒有參加 PK！\n\r", ch);
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
		send_to_char( "你已經在戰鬥中了！\n\r", ch );
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
	int IsFear = 0;  // 處理 fear 狀態 2022/10/13

	if( !ch || ch->deleted )
		return;

	// 以 hp 或 pos_dead 判定, 因為 raw_kill 不一定為 pos_dead  2022/12/29
	if( ch->position == POS_DEAD || ch->hit <= 0 )
		return;

	if ( get_eq_char( ch , WEAR_RIDE ) )
	{
		if (get_skill_level(ch, gsn_ride) > 4) nStun--;
	}

	if( IS_AFFECTED( ch, AFF_FEAR ) ) IsFear = 1; // 如果有 fear 狀態則失敗時不會 stun 2022/10/13

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		int nInbornSklv = get_skill_level(ch, gsn_inborn_ability);
		if (nInbornSklv > 4) nStun -= nInbornSklv - 4;

		int_escape_percent = (100 * nInbornSklv + int_escape_percent * 6) / (nInbornSklv + 6);
	}

	if (nStun < 1) nStun = 1;

	if( !( victim = ch->fighting ) )
	{
		update_pos( ch );
		send_to_char( "好好的沒事逃什麼？\n\r", ch );
		return;
	}
	if( !ch->in_room || victim->deleted ) return;

	if( IS_SET( ch->in_room->room_flags, ROOM_NO_FLEE ) )
	{
		send_to_char( "你沒法逃跑。\n\r", ch );
		return;
	}

	if( is_affected( ch, gsn_berserk ) )
	{
		send_to_char( "你正全力投入戰鬥，非得要分出個敵死我活才行。\n\r", ch );
		return;
	}

	// affect stone form 修正 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("你在石化狀態下不能移動.\n\r", ch);
		return;
	}

	if( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare 敘述修改 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m你正被困在陷阱裡動彈不得！[m\n\r", ch );
		}else{
			send_to_char( "[33m你試著逃跑，但身體卻動彈不得！[m\n\r", ch );
		}
		act( "$n 試著逃跑，但是卻動彈不得。",ch, NULL, NULL, TO_ROOM );
		return;
	}

	if( ch->move < 20 )
	{
		send_to_char( "你沒有足夠的體力逃跑！\n\r", ch );
		return;
	}
	// 防止 mob 戰鬥中無限逃跑
	if( IS_NPC(ch) && IS_SET( ch->act, ACT_NO_FLEE ))
		return;


	if( !IS_NPC(ch) )
	{
		//基本職
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

		//進階職
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

	// 阻止mob連續flee
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
					send_to_char( "你試著逃跑，但卻被對手阻擋下來！\n\r", ch );
					send_to_char("你阻擋對手逃跑的意圖！\n\r", victim);
					if( !IS_NPC(ch) )
					{
						if( ch->pcdata->learned[gsn_escape] )
						{
							if( int_number_percent + 1 > int_escape_percent / 2 + sklv * 5)
							{
							   send_to_char( "失敗了！ 你失去了 "HIG"20"NOR" 點經驗.\n\r", ch );
							   ch->exp = ch->exp - 20;
							}
						}
						else if( sklv < 6 )
						{
							send_to_char( "失敗了！ 你失去了 "HIG"15"NOR" 點經驗.\n\r", ch );
							ch->exp = ch->exp-15;
						}
						if( IsFear == 0 ) // 處理 fear 狀態 flee 失敗無限 stun 的問題 2022/10/13
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
			act( "$n 逃走了！", IS_SET( ch->act, PLR_HORSE ) && ch->desc ? ch->pcdata->horse : ch, NULL, NULL, TO_ROOM );
			ch->in_room = now_in;

			if( !IS_NPC(ch) )
			{
				if( ch->pcdata->learned[gsn_escape])
				{
					if( int_number_percent + 1 > int_escape_percent / 2 + sklv * 5)
					{
						send_to_char( "你自戰鬥中逃跑，損失了 "HIG"25"NOR" 點經驗.\n\r", ch );
						ch->exp = ch->exp - 25;
					}
				}
				else if( sklv < 6 )
				{
					send_to_char( "你自戰鬥中逃跑，損失了 "HIG"20"NOR" 點經驗.\n\r", ch );
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
					send_to_char( "失敗了！ 你失去了 "HIG"20"NOR" 點經驗.\n\r", ch );
					ch->exp = ch->exp -20;
				}
			}
			else if(sklv < 6 )
			{
				send_to_char( "失敗了！ 你失去了 "HIG"15"NOR" 點經驗.\n\r", ch );
				ch->exp = ch->exp -15;
			}
			if( IsFear == 0 ) // 處理 fear 狀態 flee 失敗無限 stun 的問題 2022/10/13
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
			send_to_char("沒有這個方向。\n\r", ch);
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
			send_to_char("那個方向沒有路！\n\r", ch);
			return;
		}

		if( victim->level > 40 || (!IS_NPC(victim) && victim->level > 34 ))
		{
			if( int_number_percent > ( IS_NPC(ch) ?  mob_adjust:adjust ) )
			{
				send_to_char( "你試著逃跑，但卻被對手阻擋下來！\n\r", ch );
				send_to_char("你阻擋對手逃跑的意圖！\n\r", victim);

				if( !IS_NPC(ch) )
				{
					if( ch->pcdata->learned[gsn_escape])
					{
						if( int_number_percent + 1 > int_escape_percent / 2 + sklv * 5)
						{
							send_to_char( "失敗了！ 你失去了 "HIG"20"NOR" 點經驗.\n\r", ch );
							ch->exp -= 20; 
						}
					}
					else if(sklv < 6 )
					{
						send_to_char( "失敗了！ 你失去了 "HIG"15"NOR" 點經驗.\n\r", ch );
						ch->exp -= 15;
					}
					if( IsFear == 0 ) // 處理 fear 狀態 flee 失敗無限 stun 的問題 2022/10/13
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
		act( "$n 逃走了！", IS_SET( ch->act, PLR_HORSE ) && ch->desc ? ch->pcdata->horse : ch, NULL, NULL, TO_ROOM );
		ch->in_room = now_in;

		if( !IS_NPC(ch) )
		{
			if( ch->pcdata->learned[gsn_escape])
			{
				if( int_number_percent +1> int_escape_percent/2 + sklv * 2)
				{
					send_to_char( "你自戰鬥中逃跑，損失了 "HIG"25"NOR" 點經驗.\n\r", ch );
					ch->exp=ch->exp-25;
				}
			}
			else 
			{
				send_to_char( "你自戰鬥中逃跑，損失了 "HIG"20"NOR" 點經驗.\n\r", ch );
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
				act( "$N 停止獵殺你。", hch, NULL, vch, TO_CHAR );

				vch->hunting = ch;
				vch->next_in_hunt = ch->hunted;
				ch->hunted = vch;
				act( "你開始獵殺 $N!\n\r", vch, NULL, ch, TO_CHAR);
				act( "$n 開始獵殺你！\n\r", vch, NULL, ch, TO_VICT);
			}
			else
			{
				vch->hunting = ch;
				vch->next_in_hunt = ch->hunted;
				ch->hunted = vch;
				act( "$n 開始獵殺你！\n\r", vch, NULL, ch, TO_VICT);
			}
		}

		if ( IS_AFFECTED( vch, AFF_PARALYZED ) ) {
			return;
		}
		// affect stone form 修正 2022/12/04
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
				act( "你停止對 $N 的追蹤。\n\r", vch, NULL, ch, TO_CHAR);
				act( "$n 停止對你的追蹤。\n\r", vch, NULL, ch, TO_VICT);
				vch->track_times = 0;
				return;
			}
			else
			{
				act( "$n 追蹤著 $N 的行動，從後頭追了過去.", vch, NULL, ch, TO_ROOM );
				act( "你追蹤 $N 的行動！", vch, NULL, ch, TO_CHAR );
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
		send_to_char( "這裡沒有這個人.\n\r", ch );
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
	send_to_char( "如果你想要吸血，你得把它打完全。\n\r", ch );
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
		send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
		return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' && !ch->fighting  )
	{
		send_to_char( "想咬誰?\n\r", ch );
		return;
	}

	if( arg[0] == '\0' && ch->fighting && ch->fighting->in_room &&
			ch->fighting->in_room != ch->in_room )
	{
		send_to_char( "想咬誰?\n\r", ch );
		return;
	}

	if ( !( victim = ch->fighting ) )
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "這裡沒這個人.\n\r", ch );
			return;
		}

	if( victim->deleted ) return;
	if ( victim == ch )
	{
		send_to_char( "你的嘴咬不到自己的喉嚨.\n\r", ch );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;
	check_killer( ch, victim );

	send_to_char( "你好想喝點鮮血!\n\r", ch );
	act( "$n飢渴地向$N的喉嚨咬去!", ch, NULL, victim, TO_NOTVICT );
	act( "$n飢渴地咬向你的喉嚨!", ch, NULL, victim, TO_VICT );

	// 非吸血鬼的種族用 feed 一定不命中
	if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_VAMPIRE ) )
	{
		send_to_char( "你沒有咬到!\n\r", ch );
		// 此處存在問題, 當 ch 在戰鬥中 feed 非戰鬥中的 victim 時, victim 不會觸發對 ch 的戰鬥
		// 但非吸血鬼角色的 feed 永遠不造成傷害, 戰鬥中 feed 非戰 victim 只會造成自己 stun
		if ( !ch->fighting )
		{
			set_fighting( ch, victim );
			set_fighting( victim, ch );			//Added by Razgriz	20050619
			// npc 以 feed 開戰的第一回合會觸發 fight_prog 2022/07/11
			if( IS_NPC(ch) )
				mprog_fight_trigger( ch , victim);
			// npc 被 feed 開戰的第一回合會觸發 fight_prog 2022/07/11
			if( IS_NPC(victim) && !victim->fighting )
				mprog_fight_trigger( victim , ch);
			WAIT_STATE( ch , 4 );
		}
		return;
	}

	if(ch->move < 10 + nSklv * 5){
		send_to_char( "你太累了!\n\r", ch );
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
	// 移除判定 awake 的設定, 原設定在 ch 進入戰鬥且 victim sleep 的情況下 victim 不會對傷害作出反應 2022/07/11
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
			send_to_char( "你沒有咬到!\n\r", ch );
			set_fighting( ch, victim );
			set_fighting( victim, ch );			//Added by Razgriz	20050619
			// npc 以 feed 開戰的第一回合會觸發 fight_prog 2022/07/11
			if(!ch->fighting && IS_NPC(ch))
				mprog_fight_trigger(ch , victim);
			// npc 被 feed 開戰的第一回合會觸發 fight_prog 2022/07/11
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

	// 注意 victim 是否已死
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
			send_to_char( "啊！ 美味的鮮血，滋養了你的生命！\n\r", ch );
			if(IS_NPC(victim))
				ch->hit = UMIN( ch->max_hit * 11 / 10, ch->hit + dam / 4);
			else
				ch->hit = UMIN( ch->max_hit * 11 / 10, ch->hit + dam / 20);
		}else{
			send_to_char( "啊！ 美味的鮮血！\n\r", ch );
		}
		// 修改 gain_condition 不需要判定 當前數值 2023/01/13
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
	//避免 murder self 產生的 artifact 無責任連發的 bug
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
  	char *victim_symbol[] = {"  ","ˍ","▁","▂","▃","▄","▅","▆","▇","█","◆"},
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
    strcat(state, HIW"聖"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim, AFF_FAERIE_FIRE))
    strcat(state, HIP"粉"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim, AFF_TARDY ) )
    strcat(state, HIY"霧"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim,AFF_MYSTIC_ARMOR))
    strcat(state, HIB"壁"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_mana_shield))
    strcat(state, HIU"盾"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_mystic_mirror))
    strcat(state, HIW"鏡"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_flaming_armor))
    strcat(state, HIP"焰"NOR"|");
  else strcat(state, unknow_state);
  if(IS_AFFECTED(victim, AFF_CRAZY ))
    strcat(state, CYN"瘋"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_affected(victim, gsn_blindness))
    strcat(state, HIB"盲"NOR"|");   //no problem
  else strcat(state, unknow_state);
  if(is_flying( victim ))
    strcat(state, HIG"飛"NOR"|");   //no problem
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
				case 0: sprintf( buf, ""HIR"大主教黛妮亞(Dynia)憐惜地宣布：「%s不幸戰死沙場，有人能幫助他嗎？」"NOR"\n\r",
										victim->name );
						break;
				case 1: sprintf( buf, ""HIR"%s"HIR"大喊：「無能的冒險者%s陳屍於%s，有誰願意幫助他？」"NOR"\n\r",
										ch->short_descr, victim->name, ch->in_room->name );
						break;
				case 2: sprintf( buf, ""HIR"死神捎來一份訊息：「%s悲慘地被%s"HIR"殺掉了，請大家幫助他。」"NOR"\n\r",
										victim->name, ch->short_descr );
						break;
				case 3: sprintf( buf, ""HIR"%s"HIR"顫抖地高喊：「糟糕，我失手把%s殺死了！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 4: sprintf( buf, ""HIR"%s"HIR"尖叫著：「%s有一具%s的屍體，喬藍多(Joe Rando)麻煩快收走好嗎。」"NOR"\n\r",
										ch->short_descr, ch->in_room->name, victim->name );
						break;
				case 5: sprintf( buf, ""HIR"%s"HIR"呼籲著：「%s你還太嫩了，請別來打擾我的生活。」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 6: sprintf( buf, ""HIR"%s"HIR"惋惜地說：「%s又被殺掉了，希望下次闇之冥王星普魯多(Pluto)能夠保佑他！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 7: sprintf( buf, ""HIR"%s"HIR"悲戚地喊：「早已滿手鮮血，現在又多了%s的亡魂！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
			}
		}
		else
		{
			switch ( number_bits(3) )
			{
				case 0: sprintf( buf, ""HIR"%s"HIR"高喊著：「哇哈哈，有誰願意幫%s收屍啊！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 1: sprintf( buf, ""HIR"%s"HIR"恥笑著：「無能的%s，你還有膽來一次嗎？」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 2: sprintf( buf, ""HIR"%s"HIR"狂笑著：「哇哈哈，我隨隨便便就把%s殺掉了！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 3: sprintf( buf, ""HIR"%s"HIR"狂笑說：「可憐的%s，回家向你的母親哭訴吧。」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 4: sprintf( buf, ""HIR"%s"HIR"大喊著：「%s，我還可以再殺你一次，只要你敢來的話！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 5: sprintf( buf, ""HIR"%s"HIR"大笑著：「天底下居然還有像%s這麼自不量力的人！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 6: sprintf( buf, ""HIR"%s"HIR"狂笑說：「看著%s鮮血亂噴，真是一種無上的享受！」"NOR"\n\r",
										ch->short_descr, victim->name );
						break;
				case 7: sprintf( buf, ""HIR"%s"HIR"噁心的大叫：「打死%s，就像踩扁一隻蟑螂一樣，真是噁心！」"NOR"\n\r",
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
    act( "$n 停止獵殺你", victim, NULL, vch, TO_VICT );
    act( "你停止獵殺 $N", victim, NULL, vch, TO_CHAR );
  }

  if ( ( victim->hunted ) )
  {
    for( vch = victim->hunted; vch; vch = rch )
    {
      rch = vch->next_in_hunt;
      vch->hunting = NULL;
      vch->next_in_hunt = NULL;
      act( "$N 停止獵殺你", victim, NULL, vch, TO_CHAR );
      act( "你停止獵殺 $n", victim, NULL, vch, TO_VICT );
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
    act( "你停止暗殺 $N", victim, NULL, vch, TO_CHAR );
  }

  if ( ( victim->assassinated ) )
  {
    for( vch = victim->assassinated; vch; vch = rch )
    {
      rch = vch->next_in_ass;
      vch->assassinating = NULL;
      vch->next_in_ass = NULL;
      act( "你停止暗殺 $n", victim, NULL, vch, TO_VICT );
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
    act( "你停止追蹤 $N", victim, NULL, vch, TO_CHAR );
  }

  if ( ( victim->tracked ) )
  {
    for( vch = victim->tracked; vch; vch = rch )
    {
      rch = vch->next_in_track;
      vch->tracking = NULL;
      vch->next_in_track = NULL;
      act( "你停止追蹤 $n", victim, NULL, vch, TO_VICT );
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
      sprintf( buf, "戰神馬爾斯宣布：%s 擊敗強敵 %s, 獲得本次戰役的勝利！\n\r",
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
      sprintf( buf, "戰神馬爾斯宣布：%s 擊敗強敵 %s, 獲得本次戰役的勝利！\n\r",
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
