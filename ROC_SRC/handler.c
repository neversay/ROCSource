 /**************************************************************************
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
//*  PROGRAM ID  : handler.c                                      *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         修改affect_bit_name之cast name         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940616 jye                                    *
//*  PURPOSE     :         修改remove eq會將原有cast移除之bug     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940622 Razgriz                                *
//*  PURPOSE     :         修正 get_skill_level() 中，在 mob lv   *
//*              :         大於 60 ，mob 被自己的法術 k 掛，      *
//*              :         使得在 extract_char() 後，ch->in_room  *
//*              :         變成 NULL，而造成 crash.               *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940625 Razgriz                                *
//*  PURPOSE     :         修改 get_obj_wear2()，在檢查 ch 所攜帶 *
//*              :         的物品時，加入是否已被 obj_update()    *
//*              :         檢查過的判斷，以避免 object program    *
//*              :         在 1 tick 中重複對 ch 造成影響         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940630 Razgriz                                *
//*  PURPOSE     :         修改 extract_char()，玩家角色死後      *
//*              :         酒醉狀態消失                           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940701 Razgriz                                *
//*  PURPOSE     :         修改 extract_char()，當 npc 死後，     *
//*              :         若此 npc 是被 gate 出來的，且召喚者    *
//*              :         也為 npc 時，將此召喚者的 qmark 中的   *
//*              :         v0 減 1                                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940702 Razgriz                                *
//*  PURPOSE     :         修改 get_obj_wear2()，                 *
//*              :         將 update_checked 的判斷 mark 掉，     *
//*              :         新增 updating_obj 的判斷               *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 Razgriz                                  *
//*  PURPOSE     :         修改 extract_char()，當 ch 為玩家且      *
//*              :         ch->desc->connected 要為 CON_PLAYING 時  *
//*              :         才會呼叫 char_from_room() 的函式，       *
//*              :         以避免造成可存檔的 area 中，nplayer      *
//*              :         計算錯誤而造成該區域 mprog_random_trigger*
//*              :         不會執行的 bug.                          *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940713 Razgriz                                  *
//*  PURPOSE     :         修改 get_obj_wear()以及get_obj_here()    *
//*              :         加入 ch 為 NULL 時就 return NULL，       *
//*              :         以避免 crash 的發生                      *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940831 Razgriz                                  *
//*  PURPOSE     :         新增 get_char_world2 ()                  *
//*              :         找出一 char 不管是否互相看的見           *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940908 Razgriz                                  *
//*  PURPOSE     :         新增 check_exit() 函數, 判斷 ch 是否可以 *
//*              :         從房間中某個出口的方向(check)離開        *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 jye                                      *
//*  PURPOSE     :         修改can_drop_obj 時imm無視ITEM_PERSONAL  *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 jye                                      *
//*  PURPOSE     :         修改obj_to_char 當對像為NPC不增加其負重  *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0941001 Razgriz                                  *
//*  PURPOSE     :         修改affect_loc_name增加一些沒寫進去的效應*
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0941004 jye                                      *
//*  PURPOSE     :         修改char_from_room  不記算imm char       *
//*              :             char_from_room2                      *
//*              :             char_to_room                         *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0950805 jye                                      *
//*  PURPOSE     :         修改get_skill_level回傳上限6             *
//*******************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"



/*
 * Local functions.
 */
void affect_modify args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
extern void talk_channel  args( ( CHAR_DATA *ch, char *argument,
int channel, const char *verb, const char *color) );
void extract_bank_obj args( ( OBJ_DATA *obj ) );

/*
 * Global functions and variables.  -- Keric 
 */

extern bool fremove_obj( CHAR_DATA *ch, int iWear, bool fReplace );
extern bool  GLOBAL_OBJ_UPDATE;
extern OBJ_DATA *updating_obj;


void HpMultiply_Affect(AFFECT_DATA *paf)
{
	if( paf->location == APPLY_HIT || paf->location == APPLY_MANA || paf->location == APPLY_MOVE )
		paf->modifier *= PPL_HP_MULTIPLYER;
}

void HpDeMultiply_Affect(AFFECT_DATA *paf)
{
	if( paf->location == APPLY_HIT || paf->location == APPLY_MANA || paf->location == APPLY_MOVE )
		paf->modifier /= PPL_HP_MULTIPLYER;
}

// 判定 hp mp mv 是否因為裝備小於 0 2023/04/10
bool CheckMaxHpMpMv( CHAR_DATA *ch , AFFECT_DATA *paf )
{
	if( paf->location == APPLY_HIT ){
		if( ch->max_hit + ( paf->modifier * PPL_HP_MULTIPLYER ) <= 0 )
			return FALSE;
		else
			return TRUE;
	}
	if( paf->location == APPLY_MANA ){
		if( ch->max_mana + ( paf->modifier * PPL_HP_MULTIPLYER ) <= 0 )
			return FALSE;
		else
			return TRUE;
	}
	if( paf->location == APPLY_MOVE ){
		if( ch->max_move + ( paf->modifier * PPL_HP_MULTIPLYER ) <= 0 )
			return FALSE;
		else
			return TRUE;
	}
	return TRUE;
}

bool IsUnderFistFight(CHAR_DATA *ch) {
	OBJ_DATA *ob;

	if (!( !IS_NPC( ch ) && ( ch->class == MONK ))) return FALSE;

	if ( ( ob = get_eq_char(ch, WEAR_BODY) ) ){
		if( get_skill_level(ch, gsn_fist_fight) < 5 || ob->weight > get_curr_str(ch) * 2) {
			return FALSE;
		}
	}

	if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		if (get_eq_char( ch, WEAR_LEGS ))
			return FALSE;
	} else {
		int nInbornSklv = get_skill_level(ch, gsn_inborn_ability);
		int nLegCount = 0;
		if (get_eq_char(ch, WEAR_LEGS)) nLegCount++;
		if (get_eq_char(ch, WEAR_LEGS_2)) nLegCount++;

		if (nLegCount != 0 && nInbornSklv < nLegCount + 3) {
			return FALSE;
		}
	}

	if (!get_eq_char( ch, WEAR_HOLD ) && !get_eq_char( ch, WEAR_SHIELD )
		&& !get_eq_char( ch, WEAR_WIELD ) && !get_eq_char( ch, WEAR_WIELD_2 )
		&& !get_eq_char( ch, WEAR_TWO_HAND ) ){
		return TRUE;
	}
  
	return FALSE;
}

int GetArmorMasteryPdrBonus(CHAR_DATA *ch)
{
	int nPDR = 0;
	if( !IS_NPC(ch) && get_skill_percent(ch,gsn_armor_mastery) > 0 ){
		int sklv = get_skill_level( ch,gsn_armor_mastery );
		if(sklv > 4) nPDR += 30;
		if(sklv > 5) nPDR += 40;
	}
	return nPDR;
}

int GetArmorMasteryAcBonus(CHAR_DATA *ch)
{
	int bonus = 0;
	if( !IS_NPC(ch) && get_skill_percent(ch,gsn_armor_mastery) > 0 )
	{
		int sklv = get_skill_level( ch,gsn_armor_mastery );
		bonus = (get_skill_percent(ch,gsn_armor_mastery) + 120) / 20 + (sklv * sklv) / 2;
	}
	return bonus;
}

int HasMageSorMaster(CHAR_DATA *ch)
{
	/*
		if(ch->master && !IS_NPC(ch->master)){
		if(ch->master->pcdata->prime_attr == APPLY_INT)
		return 1;
		}
	*/
	if(!(ch->rembch))
		return 0;

	if(!IS_NPC(ch->rembch) && (ch->rembch->pcdata->prime_attr == APPLY_INT))
		return 1;

	return 0;
}

int GetSorGateMobLevel(int nMobIndex)
{
	/*
		if(nMobIndex == MOB_VNUM_SOR_FLAME2 ||
		nMobIndex == MOB_VNUM_SOR_WATER2 ||
		nMobIndex == MOB_VNUM_SOR_EARTH2 ||
		nMobIndex == MOB_VNUM_SOR_WIND2  ||
		nMobIndex == MOB_VNUM_SOR_LIGHTNING2){
			return 2;
		}
		if(nMobIndex == MOB_VNUM_SOR_FLAME1 ||
		nMobIndex == MOB_VNUM_SOR_WATER1 ||
		nMobIndex == MOB_VNUM_SOR_EARTH1 ||
		nMobIndex == MOB_VNUM_SOR_WIND1  ||
		nMobIndex == MOB_VNUM_SOR_LIGHTNING1){
			return 1;
		}
		if(nMobIndex == MOB_VNUM_SOR_FLAME0 ||
		nMobIndex == MOB_VNUM_SOR_WATER0 ||
		nMobIndex == MOB_VNUM_SOR_EARTH0 ||
		nMobIndex == MOB_VNUM_SOR_WIND0  ||
		nMobIndex == MOB_VNUM_SOR_LIGHTNING0){
			return 0;
		}
	*/
	if(nMobIndex == MOB_VNUM_SOR_FLAME2
	|| nMobIndex == MOB_VNUM_SOR_WATER2
	|| nMobIndex == MOB_VNUM_SOR_EARTH2
	|| nMobIndex == MOB_VNUM_SOR_WIND2
	|| nMobIndex == MOB_VNUM_SOR_LIGHTNING2)
	{
		return 1;
	}
	if(nMobIndex == MOB_VNUM_SOR_FLAME1
	|| nMobIndex == MOB_VNUM_SOR_WATER1
	|| nMobIndex == MOB_VNUM_SOR_EARTH1
	|| nMobIndex == MOB_VNUM_SOR_WIND1
	|| nMobIndex == MOB_VNUM_SOR_LIGHTNING1
	|| nMobIndex == MOB_VNUM_SOR_FLAME0
	|| nMobIndex == MOB_VNUM_SOR_WATER0
	|| nMobIndex == MOB_VNUM_SOR_EARTH0
	|| nMobIndex == MOB_VNUM_SOR_WIND0
	|| nMobIndex == MOB_VNUM_SOR_LIGHTNING0)
	{
		return 0;
	}
	return -1;
}

bool IsSorGateMobFlame(int nMobIndex)
{
	if(nMobIndex == MOB_VNUM_SOR_FLAME0
	|| nMobIndex == MOB_VNUM_SOR_FLAME1
	|| nMobIndex == MOB_VNUM_SOR_FLAME2)
	{
		return TRUE;
	}
	return FALSE;
}

bool IsSorGateMobWater(int nMobIndex)
{
	if(nMobIndex == MOB_VNUM_SOR_WATER0
	|| nMobIndex == MOB_VNUM_SOR_WATER1
	|| nMobIndex == MOB_VNUM_SOR_WATER2)
	{
		return TRUE;
	}
	return FALSE;
}

bool IsSorGateMobWind(int nMobIndex)
{
	if(nMobIndex == MOB_VNUM_SOR_WIND0
	|| nMobIndex == MOB_VNUM_SOR_WIND1
	|| nMobIndex == MOB_VNUM_SOR_WIND2)
	{
		return TRUE;
	}
	return FALSE;
}

bool IsSorGateMobEarth(int nMobIndex)
{
	if(nMobIndex == MOB_VNUM_SOR_EARTH0
	|| nMobIndex == MOB_VNUM_SOR_EARTH1
	|| nMobIndex == MOB_VNUM_SOR_EARTH2)
	{
		return TRUE;
	}
	return FALSE;
}

bool IsSorGateMobLightning(int nMobIndex)
{
	if(nMobIndex == MOB_VNUM_SOR_LIGHTNING0
	|| nMobIndex == MOB_VNUM_SOR_LIGHTNING1
	|| nMobIndex == MOB_VNUM_SOR_LIGHTNING2)
	{
		return TRUE;
	}
	return FALSE;
}

bool IsSorGateMob(int nMobIndex)
{
	if(GetSorGateMobLevel(nMobIndex) > -1)
		return TRUE;
	return FALSE;
}

bool IsSorGateMobByChar(CHAR_DATA *ch)
{
	if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)) {
		return TRUE;
	}
	return FALSE;
}

double SorGateMobBonus(CHAR_DATA *ch, int base) // 判定 sor mob 的數值受 master 的 pp 影響
{
	int sklv = 0;
	if (ch->master == NULL) return 0;
	if (!IS_NPC(ch)) return 0;

	sklv = GetGeneralMobSklv(ch);

	return (double)(base * (100 + (sklv + 1) * ch->master->pp)) / 100;
}

bool IsPK(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if(!IS_NPC(victim)){
		if(!IS_NPC(ch)){
			return TRUE;
		}else if(IsSorGateMob(ch->pIndexData->vnum)){
			if(ch->master && !IS_NPC(ch->master)){
				return TRUE;
			}
		}
	}
	return FALSE;
}

int NEW_GET_SVS( CHAR_DATA * ch,int type){

	/*讓陣營影響svsSaint和svsDark的值*/
	if ( type == MAGIC_SAINT )
	{
		if( IS_GOOD(ch) )
			return ( IS_NPC(ch) ? ch->saving_throw[type] : 15 - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type] - 10; 
		if( IS_EVIL(ch) )
			return ( IS_NPC(ch) ? ch->saving_throw[type] : 15 - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type] + 10;

		return ( IS_NPC(ch) ? ch->saving_throw[type] : 15 - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type] + 5; 
	}
	else if( type == MAGIC_DARK )
	{
		if( IS_GOOD(ch) )
			return ( IS_NPC(ch) ? ch->saving_throw[type] : 15 - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type] + 10; 
		if( IS_EVIL(ch) )
			return ( IS_NPC(ch) ? ch->saving_throw[type] : 15 - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type] - 10;

		return ( IS_NPC(ch) ? ch->saving_throw[type] : 15 - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type] + 5; 
	}
	else if( type == MAGIC_POISON ) // 天生毒素基礎值 2021/11/04
	{
		return ( IS_NPC(ch) ? ch->saving_throw[type] : ch->saving_throw[type] + ch->level );
	}
	else
		return ( IS_NPC(ch) ? ch->saving_throw[type] : 30 - get_curr_int(ch) - get_curr_wis(ch) + ch->saving_throw[type]) + race_table[ch->race].nature_svs[type];


}
int get_allow_deny_bit( CHAR_DATA *ch );

int GET_SVS( CHAR_DATA * ch){
	/*
		#define GET_SVS( ch )( IS_NPC( ch ) ? ch->saving_throw : 30 - get_curr_int( ch ) - \get_curr_wis( ch ) + ch->saving_throw )
	*/
	return ( IS_NPC(ch) ? ch->saving_throw[0] : 30 - get_curr_int(ch) - get_curr_wis(ch) + ch->saving_throw[0])+ race_table[ch->race].nature_svs[0];

}
int get_vrace_hrq ( CHAR_DATA * victim, OBJ_DATA * wpn )
{
	int q = 0;
	OBJ_VRACE * ovr ;
	int type;
	if( wpn->item_type == ITEM_ARROW )
		type = 14;
	else
		type = wpn->value[3];

	if( wpn->vrace ) {
		for( ovr = wpn->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q += ovr->hr_mod_q;
				break;
			}
		}
	}

	if( wpn->material->vrace ) {
		for( ovr = wpn->material->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q += ovr->hr_mod_q;
				break;
			}
		}
	}

	if( type > -1 && type < MAX_TYPES ) {
		for( ovr = (basic_types[type])->vrace ; ovr ; ovr = ovr->next){
			if( victim->race == ovr->race ) {
				q += ovr->hr_mod_q;
				break;
			}
		}
	}
	return q;
}

int get_vrace_drq ( CHAR_DATA * victim, OBJ_DATA * wpn )
{
	int q = 0;
	OBJ_VRACE * ovr ;
	int type;

	if( wpn->item_type == ITEM_ARROW )
		type = 14;
	else
		type = wpn->value[3];

	if( wpn->vrace ) {
		for( ovr = wpn->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q += ovr->dr_mod_q;
				break;
			}
		}
	}

	if( wpn->material->vrace ) {
		for( ovr = wpn->material->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q += ovr->dr_mod_q;
				break;
			}
		}
	}
	if( type > -1 && type < MAX_TYPES ) {
		for( ovr = (basic_types[type])->vrace ; ovr ; ovr = ovr->next){
			if( victim->race == ovr->race ) {
				q += ovr->dr_mod_q;
				break;
			}
		}
	}
	return q;
}
int get_vrace_hrp ( CHAR_DATA * victim, OBJ_DATA * wpn )
{
	int q = 100;
	OBJ_VRACE * ovr ;
	int type;

	if( wpn->item_type == ITEM_ARROW )
		type = 14;
	else
		type = wpn->value[3];

	if( wpn->vrace ) {
		for( ovr = wpn->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q = ovr->hr_mod_p;
				break;
			}
		}
	}

	if( wpn->material->vrace ) {
		for( ovr = wpn->material->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q = ( q * ovr->hr_mod_p ) / 100;
				break;
			}
		}
	}
	if( type > -1 && type < MAX_TYPES ) {
		for( ovr = (basic_types[type])->vrace ; ovr ; ovr = ovr->next){
			if( victim->race == ovr->race ) {
				q  = ( q * ovr->hr_mod_p ) / 100;
				break;
			}
		}
	}
	return q;
}
int get_vrace_drp ( CHAR_DATA * victim, OBJ_DATA * wpn )
{
	int q = 100;
	OBJ_VRACE * ovr ;
	int type;

	if( wpn->item_type == ITEM_ARROW )
		type = 14;
	else
		type = wpn->value[3];

	if( wpn->vrace ) {
		for( ovr = wpn->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q = ovr->dr_mod_p;
				break;
			}
		}
	}

	if( wpn->material->vrace ) {
		for( ovr = wpn->material->vrace ; ovr ; ovr = ovr ->next ){
			if( victim->race == ovr -> race ){
				q = ( q * ovr->dr_mod_p ) / 100;
				break;
			}
		}
	}


	if( type > -1 && type < MAX_TYPES ) {
		for( ovr = (basic_types[type])->vrace ; ovr ; ovr = ovr->next){
			if( victim->race == ovr->race ) {
				q  = ( q * (ovr->dr_mod_p + 300)) / 400;
				break;
			}
		}
	}

	return q;
}
/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
	if( !ch ) return 0;
	if ( ch->desc && ch->desc->original )
		ch = ch->desc->original;

	if ( ch->trust != 0 )
		return ch->trust;

	if ( IS_NPC( ch ) && ch->level >= LEVEL_HERO )
		return ( L_APP - 1 );
	else
		return ch->level;
}


int GET_DODGE( CHAR_DATA * ch )
{
  int nMobIndex = 0;
  int nDodge = 0;
  nDodge = ch->carry_weight;
  if(nDodge < 0) nDodge = 0;
  if( IS_NPC(ch ) ){
      nDodge += ch->dodge
             + ((IS_AWAKE(ch)) ? dex_app[get_curr_dex(ch)].defensive : 100)
             + ( is_flying(ch) ? - 40 : 0 );
    if (IsSorGateMobByChar(ch)) {
      nMobIndex = ch->pIndexData->vnum;
      if (IsSorGateMobWind(nMobIndex)) {
        nDodge += SorGateMobBonus(ch, -200);
      } else {
        nDodge += SorGateMobBonus(ch, -150);
      }
    }
  }else{
    nDodge += (ch->class == NINJA ?
                (ch->pcdata->prime_attr == APPLY_DEX ? -60 :
                (ch->pcdata->prime_attr == APPLY_STR ? -40 :
                (ch->pcdata->prime_attr == APPLY_CON ? -20 : 0))) : 0)
           + ch->dodge
           + ((IS_AWAKE(ch)) ? dex_app[get_curr_dex(ch)].defensive : 100)
           + (ch->level * (-5) + (is_flying(ch) ? - 40 : 0 ));
  }
  return nDodge;
}

/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
	if( IS_NPC(ch) )
		return 10;
	else
		return 16 + ( ch->played + (int) ( current_time - ch->logon ) ) / 40000;
	// return 16 + ( ch->played + (int) ( current_time - ch->logon ) ) / 428400;

    /* 428400 assumes 30 secs/mud hour * 24 hours/day * 35 days/month *
       17 months/year - Kahn */
}


int get_eff_str( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].str_mod;
    value = 13 + mod;
    if ( is_affected( ch, gsn_giant ) || is_affected( ch, skill_lookup("holy crown")) )
      value += 1;
    if ( is_affected( ch, gsn_weaken ) )
      value -= 1;

    if( IS_NPC(ch))  return max;

    value += ( ch->pcdata->prime_attr == APPLY_STR ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_STR ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_STR, value );

    return max;
}



/*
 * Retrieve character's current intelligence.
 */
int get_eff_int( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].int_mod;
    value = 13 + mod;

    if ( is_affected( ch, gsn_mental_empower) || is_affected( ch, skill_lookup("holy crown")) )
      value += 1;
    if( IS_NPC(ch) ) return max;

    value += ( ch->pcdata->prime_attr == APPLY_INT ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_INT ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_INT, value );


    return max;
}



/*
 * Retrieve character's current wisdom.
 */
int get_eff_wis( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

      
    mod   = race_table[ch->race].wis_mod;
    value = 13 + mod;

    if( is_affected(ch,skill_lookup("holy crown"))) value += 1; 

    if( IS_NPC(ch)) return max;

    value += ( ch->pcdata->prime_attr == APPLY_WIS ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_WIS ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_WIS, value );


    return max;
}



/*
 * Retrieve character's current dexterity.
 */
int get_eff_dex( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].dex_mod;
    value = 13 + mod;
    if ( is_affected( ch, skill_lookup("combat mind")) || is_affected( ch, skill_lookup("holy crown")) )
      value += 1;

    if( IS_NPC(ch) ) return max;

    value += ( ch->pcdata->prime_attr == APPLY_DEX ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_DEX ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_DEX, value );

    return max;
}



/*
 * Retrieve character's current constitution.
 */
int get_eff_con( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].con_mod;
    value = 13 + mod;
    if ( is_affected( ch, gsn_adrenaline ) || is_affected( ch, skill_lookup("holy crown")) )
      value += 1;

    if( IS_NPC(ch) ) return max;

    value += ( ch->pcdata->prime_attr == APPLY_CON ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_CON ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_CON, value );

    return max;
}

int get_max_str( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].str_mod;
    value = 13 + mod;

    if( IS_NPC(ch) ) {
        if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
            max = 15 + GetSorGateMobLevel(ch->pIndexData->vnum) * 3;
            if(IsSorGateMobLightning(ch->pIndexData->vnum)) max += 6;
            if(IsSorGateMobFlame(ch->pIndexData->vnum)) max += 3;
        }
        return max;
		}

    value += ( ch->pcdata->prime_attr == APPLY_STR ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_STR ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_STR, value );

    return max;
}



/*
 * Retrieve character's current intelligence.
 */
int get_max_int( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].int_mod;
    value = 13 + mod;

    if( IS_NPC(ch) ) {
        if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
            max = 15 + GetSorGateMobLevel(ch->pIndexData->vnum) * 3;
            if(IsSorGateMobFlame(ch->pIndexData->vnum)) max += 6;
            if(IsSorGateMobWind(ch->pIndexData->vnum)) max += 3;
        }
        return max;
		}

    value += ( ch->pcdata->prime_attr == APPLY_INT ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_INT ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_INT, value );


    return max;
}



/*
 * Retrieve character's current wisdom.
 */
int get_max_wis( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].wis_mod;
    value = 13 + mod;

    if( IS_NPC(ch) ) {
        if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
            max = 15 + GetSorGateMobLevel(ch->pIndexData->vnum) * 3;
            if(IsSorGateMobWater(ch->pIndexData->vnum)) max += 6;
            if(IsSorGateMobEarth(ch->pIndexData->vnum)) max += 3;
        }
        return max;
		}

    value += ( ch->pcdata->prime_attr == APPLY_WIS ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_WIS ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_WIS, value );


    return max;
}



/*
 * Retrieve character's current dexterity.
 */
int get_max_dex( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].dex_mod;
    value = 13 + mod;

    if( IS_NPC(ch) ) {
        if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
            max = 15 + GetSorGateMobLevel(ch->pIndexData->vnum) * 3;
            if(IsSorGateMobWind(ch->pIndexData->vnum)) max += 6;
            if(IsSorGateMobWater(ch->pIndexData->vnum)) max += 3;
        }
        return max;
		}

    value += ( ch->pcdata->prime_attr == APPLY_DEX ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_DEX ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_DEX, value );

    return max;
}



/*
 * Retrieve character's current constitution.
 */
int get_max_con( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].con_mod;
    value = 13 + mod;

    if( IS_NPC(ch) ) {
        if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
            max = 15 + GetSorGateMobLevel(ch->pIndexData->vnum) * 3;
            if(IsSorGateMobEarth(ch->pIndexData->vnum)) max += 6;
            if(IsSorGateMobLightning(ch->pIndexData->vnum)) max += 3;
        }
        return max;
		}

    value += ( ch->pcdata->prime_attr == APPLY_CON ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_CON ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class
    max = UMIN( MAX_CON, value );

    return max;
}

/*
 * Retrieve character's current strength.
 */
int get_curr_str( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].str_mod;
    value = 13 + mod;
    if ( is_affected( ch, gsn_giant ) || is_affected( ch, skill_lookup("holy crown")) )
        value += 1;
    if ( is_affected( ch, gsn_weaken ) )
        value -= 1;

    if( IS_NPC(ch)) {
      if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        int nBonus =  GetSorGateMobLevel(ch->pIndexData->vnum);
        max = 12 + nBonus * 3;
        if(IsSorGateMobLightning(ch->pIndexData->vnum)) {max += 6; nBonus += 2;}
        if(IsSorGateMobFlame(ch->pIndexData->vnum)) {max += 3; nBonus ++;}
        if (IsSorGateMobLightning(ch->pIndexData->vnum)) {
          nBonus += SorGateMobBonus(ch, 2) - 1;
        } else {
          nBonus += SorGateMobBonus(ch, 1) / 2;
        }
        return URANGE( 3, 12 + nBonus * 2 + ch->mod_str, max);
      }
      return URANGE( 1, ch->perm_str + ch->mod_str, max );
    }
    value += ( ch->pcdata->prime_attr == APPLY_STR ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_STR ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_STR, value );
    return URANGE( 1, ch->perm_str + ch->mod_str, max );
}



/*
 * Retrieve character's current intelligence.
 */
int get_curr_int( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].int_mod;
    value = 13 + mod;

    if ( is_affected( ch, gsn_mental_empower) || is_affected( ch, skill_lookup("holy crown")) )
        value += 1;
    if( IS_NPC(ch)) {
      if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        int nBonus =  GetSorGateMobLevel(ch->pIndexData->vnum);
        max = 12 + nBonus * 3;
        if(IsSorGateMobFlame(ch->pIndexData->vnum)) {max += 6; nBonus += 2;}
        if(IsSorGateMobWind(ch->pIndexData->vnum)) {max += 3; nBonus ++;}
        if (IsSorGateMobFlame(ch->pIndexData->vnum)) {
          nBonus += SorGateMobBonus(ch, 2) - 1;
        } else if (IsSorGateMobWater(ch->pIndexData->vnum)){
          nBonus += SorGateMobBonus(ch, 1) - 1;
        } else if (IsSorGateMobWind(ch->pIndexData->vnum)){
          nBonus += SorGateMobBonus(ch, 1) - 1;
        } else {
          nBonus += SorGateMobBonus(ch, 1) / 2;
        }
        return URANGE( 3, 12 + nBonus * 2 + ch->mod_int, max);
      }
      return URANGE( 3, ch->perm_int + ch->mod_int, max );
    }

    value += ( ch->pcdata->prime_attr == APPLY_INT ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_INT ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_INT, value );

    return URANGE( 3, ch->perm_int + ch->mod_int, max );
}



/*
 * Retrieve character's current wisdom.
 */
int get_curr_wis( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].wis_mod;
    value = 13 + mod;
    
    if ( is_affected( ch, skill_lookup("holy crown")) )
        value += 1;
    if( IS_NPC(ch)) {

      if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        int nBonus =  GetSorGateMobLevel(ch->pIndexData->vnum);
        max = 12 + nBonus * 3;
        if(IsSorGateMobWater(ch->pIndexData->vnum))     {max += 6; nBonus += 2;}
        if(IsSorGateMobEarth(ch->pIndexData->vnum)) {max += 3; nBonus ++;}
        if (IsSorGateMobWater(ch->pIndexData->vnum)) {
          nBonus += SorGateMobBonus(ch, 2) - 1;
        } else {
          nBonus += SorGateMobBonus(ch, 1) / 2;
        }
        return URANGE( 3, 12 + nBonus * 2 + ch->mod_wis, max);
      }
      return URANGE( 3, ch->perm_wis + ch->mod_wis, max );
    }

    value += ( ch->pcdata->prime_attr == APPLY_WIS ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_WIS ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_WIS, value );
    return URANGE( 3, ch->perm_wis + ch->mod_wis, max );
}



/*
 * Retrieve character's current dexterity.
 */
int get_curr_dex( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].dex_mod;
    value = 13 + mod;
    if ( is_affected( ch, skill_lookup("combat mind")) || is_affected( ch, skill_lookup("holy crown")))
        value += 1;
    if( IS_NPC(ch)) {
      if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        int nBonus =  GetSorGateMobLevel(ch->pIndexData->vnum);
        max = 12 + nBonus * 3;
        if(IsSorGateMobWind(ch->pIndexData->vnum))     {max += 6; nBonus += 2;}
        if(IsSorGateMobWater(ch->pIndexData->vnum)) {max += 3; nBonus ++;}
        if (IsSorGateMobWind(ch->pIndexData->vnum)) {
          nBonus += SorGateMobBonus(ch, 2) - 1;
        } else {
          nBonus += SorGateMobBonus(ch, 1) / 2;
        }
        return URANGE( 3, 12 + nBonus * 2 + ch->mod_dex, max);
      }
      return URANGE( 3, ch->perm_dex + ch->mod_dex, max );
    }

    value += ( ch->pcdata->prime_attr == APPLY_DEX ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_DEX ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_DEX, value );
    
    return URANGE( 3, ch->perm_dex + ch->mod_dex, max );
}



/*
 * Retrieve character's current constitution.
 */
int get_curr_con( CHAR_DATA *ch )
{
    int max = 25;
    int mod;
    int value;

    mod   = race_table[ch->race].con_mod;
    value = 13 + mod;
    if ( is_affected( ch, gsn_adrenaline ) || is_affected( ch, skill_lookup("holy crown")) )
        value += 1;
    if( IS_NPC(ch)) {
      if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        int nBonus =  GetSorGateMobLevel(ch->pIndexData->vnum);
        max = 12 + nBonus * 3;
        if(IsSorGateMobEarth(ch->pIndexData->vnum))     {max += 6; nBonus += 2;}
        if(IsSorGateMobLightning(ch->pIndexData->vnum)) {max += 3; nBonus ++;}
        if (IsSorGateMobEarth(ch->pIndexData->vnum)) {
          nBonus += SorGateMobBonus(ch, 2) - 1;
        } else {
          nBonus += SorGateMobBonus(ch, 1) / 2;
        }
        return URANGE( 3, 12 + nBonus * 2 + ch->mod_con, max);
      }
      return URANGE( 3, ch->perm_con + ch->mod_con, max );
    }

    value += ( ch->pcdata->prime_attr == APPLY_CON ) ? 8 :
          ( ch->pcdata->snd_attr == APPLY_CON ) ? 5 :
          ( ch->class > 4 ) ? 2 : 0;
    value += ( ch->level > 39 ) ? 1 : 0;
    if(ch->race == 0 && ch->class > 4 && !is_affected(ch, gsn_werewolf)) 
        value++;  // Extra 1 Max Point for Human with Advanced class

    max = UMIN( MAX_CON, value );

    return URANGE( 3, ch->perm_con + ch->mod_con, max );
}


// 角色體型 size 總合 2022/12/23
int get_curr_size( CHAR_DATA *ch )
{
	int mod;

	mod = race_table[ch->race].size;

    return UMAX( 0 , mod + ch->mod_size );
}


/*
 * Retrieve character's current hitroll for given weapon location
 */
int get_hitroll( CHAR_DATA *ch, int wpn )
{
	OBJ_DATA  *other_wield;
	AFFECT_DATA *paf;
	OBJ_DATA  *wield;
	int other_wpn;
	int hitroll;
	int min_str, str_diff;
	int str = get_curr_str(ch);
	int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
	int nPbSklv  = get_skill_level(ch, gsn_powerblow);
	int nDualSklv = get_skill_level(ch, gsn_dual); // 2022/12/23

	if(nPbPerc > 0)
		str += nPbSklv * nPbSklv / 6;

	hitroll = ch->hitroll;

	if (!IS_NPC(ch)) {
		hitroll += UMAX(get_curr_int(ch) - 16 , 0);
		hitroll += UMAX(get_curr_wis(ch) - 15 , 0);
		hitroll += UMAX(get_curr_dex(ch) - 16 , 0);
	}

	if (IsSorGateMobByChar(ch)) {
		int nMobIndex = ch->pIndexData->vnum;
		hitroll /= 5;
		if (IsSorGateMobWind(nMobIndex)) {
			hitroll += SorGateMobBonus(ch, 20);
		} else if (IsSorGateMobLightning(nMobIndex)){
			hitroll += SorGateMobBonus(ch, 15);
		} else {
			hitroll += SorGateMobBonus(ch,  8);
		}
	}

	if ( wpn == WEAR_WIELD || wpn == WEAR_TWO_HAND )
		other_wpn = WEAR_WIELD_2;
	else if ( wpn == WEAR_WIELD_2 )
		other_wpn = WEAR_WIELD;
	else
	{
		//char buf [ MAX_STRING_LENGTH ];
		//sprintf( buf, "get_hitroll: Invalid weapon location %d on %s.",
		//  wpn, ch->name );
		//bug( buf, 0 );
		//  bugf("get_hitroll: Invalid weapon location %d on %s.", wpn, ch->name);
		return ch->hitroll;
	}

	if ( !( other_wield = get_eq_char( ch, other_wpn ) ) )
	{
		if( (wield = get_eq_char(ch, wpn )))
		{
			switch( wpn )
			{
				case WEAR_WIELD:
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++min_str ) {
						if( str_app[min_str].wield >= wield->weight )
							break;
					}
					str_diff = str - min_str;
					if( str_diff > 2 ) {
						hitroll += UMIN( str_diff, 12 );
					}else if ( str_diff < 0 ){
						hitroll += str_diff * 5;
					}
					break;
				case WEAR_WIELD_2:
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++min_str ) {
						if( str_app[min_str].wield2 >= wield->weight )
							break;
					}
					str_diff = str - min_str;
					if( str_diff > 2 ) {
						hitroll += UMIN( str_diff, 12 );
					}else if ( str_diff < 0 ){
						hitroll += str_diff * 5;
					}
					break;
				case WEAR_TWO_HAND:
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++min_str ) {
						if( str_app[min_str].twohand >= wield->weight )
							break;
					}
					str_diff = str - min_str;
					if( str_diff > 2 ) {
						hitroll += UMIN( str_diff, 12 );
					}else if ( str_diff < 0 ){
						hitroll += str_diff * 5;
					}
					break;
			}

			switch(wield->value[3])
			{
				case 0: //hit
				case 1: //slice
				case 2: //stab
				case 3: //slash
				case 4: //whip
				case 5: //claw
				case 6: //blast
				case 7: //pound
				case 8: //crush
				case 9: //grep
				case 10: //bite
				case 11: //pierce
				case 12: //suction
				case 13: //chop
				case 14: //shot
				case 16: //ammo 2022/12/23
					hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
					hitroll += get_curr_wis(ch) + get_curr_dex(ch) - 26;
					break;
			}
			// 次要武器 hr 降低 20% , dual sklv 最高可以加成 18%  2022/12/23
			if ( wpn == WEAR_WIELD_2 ){
				hitroll = ( hitroll * 80 + nDualSklv + (nDualSklv * nDualSklv)/3 ) / 100;
			}
		}
		else
		{
			hitroll += ( ( get_curr_str(ch) - 12 ) + wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
		}
		return hitroll ;
	}
	//a specific weapon hr should deduct the other weapon's hr
	for( paf = other_wield->pIndexData->affected; paf; paf = paf->next )
		if ( paf->location == APPLY_HITROLL )
			hitroll -= paf->modifier; // obj's original affected hr
	for( paf = other_wield->material->affected; paf; paf = paf->next )
		if ( paf->location == APPLY_HITROLL )
			hitroll -= paf->modifier; // obj's material affected hr, like
	// hr come from enchant weapon spell
	for( paf = other_wield->affected; paf; paf = paf->next )
		if ( paf->location == APPLY_HITROLL )
			hitroll -= paf->modifier; // obj's run-time affected hr, like
	// hr come from enchant weapon spell
	if( (wield = get_eq_char(ch, wpn )))
	{
		switch( wpn )
		{
			case WEAR_WIELD:
				for( min_str = 0; min_str <= MAX_STR + 6 ; ++min_str ) {
					if( str_app[min_str].wield >= wield->weight )
						break;
				}
				str_diff = str - min_str;
				if( str_diff > 2 ) {
					hitroll += UMIN( str_diff, 12 );
				}
				else if ( str_diff < 0 ){
					hitroll += str_diff * 5;
				}
				break;
			case WEAR_WIELD_2:
				for( min_str = 0; min_str <= MAX_STR + 6 ; ++min_str ) {
					if( str_app[min_str].wield2 >= wield->weight )
						break;
				}
				str_diff = str - min_str;
				if( str_diff > 2 ) {
					hitroll += UMIN( str_diff, 12 );
				}
				else if ( str_diff < 0 ){
					hitroll += str_diff * 5;
				}
				break;
			case WEAR_TWO_HAND:
				for( min_str = 0; min_str <= MAX_STR + 6; ++min_str ) {
					if( str_app[min_str].twohand >= wield->weight )
						break;
				}
				str_diff = str - min_str;
				if( str_diff > 2 ) {
					hitroll += UMIN( str_diff, 12 );
				}
				else if ( str_diff < 0 ){
					hitroll += str_diff * 5;
				}
				break;
		}

		switch(wield->value[3])
		{
			// 加入 wis int 使有 other wp 的情況下 wp 的命中不受影響, 並且另外計算次要武器減少的 hr 2022/12/23
			case 0: //hit
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//break;
			case 1: //slice
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//break;
			case 2: //stab
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll += 4;
				//break;
			case 3: //slash
				//hitroll += 1;
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//break;
			case 4: //whip
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll += 2;
				//break;
			case 5: //claw
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll -= 2;
				//break;
			case 6: //blast
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll -= 3;
				//break;
			case 7: //pound
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll -= 5;
				//break;
			case 8: //crush
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll -= 4;
				//break;
			case 9: //grep
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll += 1;
				//break;
			case 10: //bite
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll -= 1;
				//break;
			case 11: //pierce
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//hitroll += 5;
				//break;
			case 12: //suction
				// hitroll += 2;
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//break;
			case 13: //chop
				//hitroll -= 2;
				//hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				//break;
			case 14: //shot
			case 16: //ammo 2022/12/23
				hitroll += ( wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
				hitroll += get_curr_wis(ch) + get_curr_dex(ch) - 26; // 2022/12/23
				break;
		}
		// 次要武器 hr 降低 20% , dual sklv 最高可以加成 18%  2022/12/23
		if ( wpn == WEAR_WIELD_2 ){
			hitroll = ( hitroll * (80 + nDualSklv + (nDualSklv * nDualSklv)/3 ) ) / 100;
		}
	}else{
		hitroll += ( ( get_curr_str(ch) - 12 ) + wis_app[get_curr_wis(ch)].tohit + dex_app[get_curr_dex(ch)].tohit );
	}

	return hitroll;
}



/*
 * Retrieve character's current damroll for given weapon location
 */
int get_damroll( CHAR_DATA *ch, int wpn )
{
	OBJ_DATA  *other_wield;
	AFFECT_DATA *paf;
	int other_wpn;
	int damroll;

	damroll = ch->damroll + ( str_app[get_curr_str( ch )].todam ) + ( str_app[get_curr_str( ch )].todam / 2);

	if (IsSorGateMobByChar(ch)) {
		int nMobIndex = ch->pIndexData->vnum;
		damroll /= 4;
		if (IsSorGateMobLightning(nMobIndex)) {
			damroll += SorGateMobBonus(ch, 25);
		} else if (IsSorGateMobEarth(nMobIndex)){
			damroll += SorGateMobBonus(ch, 20);
		} else {
			damroll += SorGateMobBonus(ch, 15);
		}
	}

	if (!IS_NPC(ch)) {
		damroll += UMAX(get_curr_con(ch) - 13 , 0);
		damroll += UMAX(get_curr_con(ch) - 15 , 0);
		damroll += UMAX(get_curr_str(ch) - 15 , 0);
	}

	if ( wpn == WEAR_WIELD || wpn == WEAR_TWO_HAND )
		other_wpn = WEAR_WIELD_2;
	else if ( wpn == WEAR_WIELD_2 )
		other_wpn = WEAR_WIELD;
	else
	{
		//char buf [ MAX_STRING_LENGTH ];
		//sprintf( buf, "get_damroll: Invalid weapon location %d on %s.",
		//  wpn, ch->name );
		//bug( buf, 0 );
		//bugf("get_damroll: Invalid weapon location %d on %s.", wpn, ch->name);
		return damroll;
	}

	if ( !( other_wield = get_eq_char( ch, other_wpn ) ) )
		return damroll;

	for( paf = other_wield->pIndexData->affected; paf; paf = paf->next )
		if ( paf->location == APPLY_DAMROLL )
			damroll -= paf->modifier;
	for( paf = other_wield->material->affected; paf; paf = paf->next )
		if ( paf->location == APPLY_DAMROLL )
			damroll -= paf->modifier;
	for( paf = other_wield->affected; paf; paf = paf->next )
		if ( paf->location == APPLY_DAMROLL )
			damroll -= paf->modifier;

	return damroll;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
	if ( !IS_NPC( ch ) && ch->level >= LEVEL_IMMORTAL )
		return 1000;

	if ( IS_NPC( ch ) ){
		if( IS_SET( ch->act, ACT_PET ) )
			return ch->level / 4;
		// 針對 charm mob 最大的持有數 2021/12/25
		else if( IS_AFFECTED( ch, AFF_CHARM ) )
			return ch->level / 10;
	}

	return MAX_WEAR + 2 * get_curr_dex( ch ) + ch->level / 2;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
	if ( !IS_NPC( ch ) && ch->level >= LEVEL_IMMORTAL )
		return 1000000;

	// if ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
	//    return 0;

	/*  新增 pet 判定可拿最大重量, 及 charm mob 一次可拿最大重量 201/12/25 */

	//	if ( IS_NPC( ch ) )
	//      return ch->level * 4 + 30;
	if ( IS_NPC( ch ) ){
		if( IS_SET( ch->act, ACT_PET ) )
			return ch->level + 35 * get_curr_str( ch );
		else if( IS_AFFECTED( ch, AFF_CHARM ) )
			return get_curr_str( ch ) * 2 + 50;
		else
			return 999;
	}

	return str_app[get_curr_str( ch )].carry;
}



/*
 * See if a string is one of the names of an object.
 * New is_name sent in by Alander.
 */

bool is_name( const char *str, char *namelist )
{
	char name [ MAX_INPUT_LENGTH ];
    
	if( str[0] == '\'' && str[1] == namelist[0] ) {
		one_argument((char*)str, name);
		if( !str_cmp(name, namelist) )
			return TRUE;
	}
	
	if( str[0] == namelist[0] && !str_cmp( str, namelist ) )
		return TRUE;
	
	for ( ; ; )
	{
		namelist = one_argument( namelist, name );
		if ( name[0] == '\0' )
			return FALSE;
		if ( !str_cmp( str, name ) )
			return TRUE;
	}
}


bool is_pfxname( const char *str, char *namelist )
{
	char name [ MAX_INPUT_LENGTH ];

	if( str[0] == '\'' && str[1] == namelist[0] ) {
		one_argument((char*)str, name);
		if( !str_prefix(name, namelist) )
			return TRUE;
	}
	
	if( str[0] == namelist[0] && !str_prefix( str, namelist ) )
		return TRUE;
	
	for ( ; ; )
	{
		namelist = one_argument( namelist, name );
		if ( name[0] == '\0' )
			return FALSE;
		//if ( !str_prefix( str, name ) && str_cmp( str, name ) )
		if ( !str_prefix( str, name ) )
			return TRUE;
	}
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
	OBJ_DATA *obj;

	if ( !ch || !ch->in_room )
	{
		bug( "Char_from_room: NULL.", 0 );
		return;
	}

	if ( !IS_NPC( ch ) && !IS_IMMORTAL(ch) )
		--ch->in_room->area->nplayer;

	if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) && obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room->light > 0 )
		--ch->in_room->light;

	if ( ch == ch->in_room->people )
	{
		ch->in_room->people = ch->next_in_room;
	}
	else
	{
		CHAR_DATA *prev;

		for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
		{
			if ( prev->next_in_room == ch )
			{
				prev->next_in_room = ch->next_in_room;
				break;
			}
		}

		if ( !prev )
		{
			bug( "Char_from_room: ch not found.", 0 );
			bug( ch->name,0 );
		}
	}

	if ( ch->fighting )
		stop_fighting( ch, TRUE );

	ch->in_room      = NULL;
	ch->next_in_room = NULL;
	return;
}

void char_from_room2( CHAR_DATA *ch )
{
	OBJ_DATA *obj;

	if ( !ch->in_room )
	{
		bug( "Char_from_room: NULL.", 0 );
		return;
	}

	if ( !IS_NPC( ch ) && !IS_IMMORTAL(ch) )
		--ch->in_room->area->nplayer;

	if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) && obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room->light > 0 )
		--ch->in_room->light;

	if ( ch == ch->in_room->people )
	{
		ch->in_room->people = ch->next_in_room;
	}
	else
	{
		CHAR_DATA *prev;

		for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
		{
			if ( prev->next_in_room == ch )
			{
				prev->next_in_room = ch->next_in_room;
				break;
			}
		}

		if ( !prev )
		{
			bug( "Char_from_room: ch not found.", 0 );
			bug( ch->name,0 );
		}
	}

	if ( ch->fighting )
		stop_fighting( ch, FALSE );

	ch->in_room      = NULL;
	ch->next_in_room = NULL;
	return;
}

/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
	OBJ_DATA *obj;
	CHAR_DATA *phorse;

	if ( !ch || !pRoomIndex )
	{
		bug( "Char_to_room: NULL.", 0 );
		return;
	}

	ch->in_room   = pRoomIndex;
	ch->next_in_room  = pRoomIndex->people;
	pRoomIndex->people  = ch;

	if ( !IS_NPC( ch ) && !IS_IMMORTAL(ch) )
		++ch->in_room->area->nplayer;

	if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) && obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
		++ch->in_room->light;

	if (!IS_NPC( ch ) && IS_SET( ch->act, PLR_HORSE ) && ch->desc )
	{
		phorse = ch->pcdata->horse;
		char_from_room( phorse );
		char_to_room( phorse, pRoomIndex );
		if ( phorse->hunting || phorse->assassinating || phorse->hunted || phorse->assassinated )
			check_asshunt( phorse );
	}

	// char 如果有移動將 ch->stay_time 歸 0 並移除 PLR_IDLE 2023/01/15
	ch->stay_time = 0;

	if( !IS_NPC(ch) && IS_SET(ch->act, PLR_IDLE) )
		REMOVE_BIT( ch->act, PLR_IDLE );

	return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
	if( !ch || !obj || obj->deleted )
		return;
	obj->next_content  = ch->carrying;
	ch->carrying   = obj;
	obj->carried_by  = ch;
	obj->in_room   = NULL;
	obj->in_obj    = NULL;

	//modified by coconet 0522
	if( obj->item_type == ITEM_HORSE )
	{
		ch->carry_number += 1;
		ch->carry_weight += obj->weight;
	}
	else
	{
		ch->carry_number += get_obj_number( obj );

		// 增加 pet 重量判定 2021/12/25
		if( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) ))
			ch->carry_weight += get_obj_weight( obj );
	}
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
	CHAR_DATA *ch;

	// obj 被 purge 掉的情況 2022/02/13
	if ( obj->deleted )
	{
		return;
	}

	if ( !( ch = obj->carried_by ) )
	{
		bug( "Obj_from_char: null ch.", 0 );
		bug( "obj vnum : %d", obj->pIndexData->vnum );
		return;
	}
	//modified by coconet 0522
	if ( obj->item_type == ITEM_HORSE)
	{
		ch->carry_number -= 1;
		ch->carry_weight -= obj->weight;
	}
	else
	{
		ch->carry_number  -= get_obj_number( obj );
		// 一般 npc 不會增加 carry weight 扣除會造成負值
		// 故只針對 pc 及 pet 判定扣除 obj weight 2021/12/25
		if( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) ))
			ch->carry_weight  -= get_obj_weight( obj );
	}
    
	if ( obj->wear_loc != WEAR_NONE )
		unequip_char( ch, obj );

	if ( ch->carrying == obj )
	{
		ch->carrying = obj->next_content;
	}
	else
	{
		OBJ_DATA *prev;

		for ( prev = ch->carrying; prev; prev = prev->next_content )
		{
			if ( prev->next_content == obj )
			{
				prev->next_content = obj->next_content;
				break;
			}
		}

		if ( !prev )
			bug( "Obj_from_char: obj not in list.", 0 );
	}

	obj->carried_by  = NULL;
	obj->next_content  = NULL;
	return;
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear )
{
	if ( obj->item_type != ITEM_ARMOR )
		return 0;

	switch ( iWear )
	{
		case WEAR_BODY:     return 3 * obj->value[0];
		case WEAR_HEAD:     return 2 * obj->value[0];
		case WEAR_LEGS:     return 2 * obj->value[0];
		case WEAR_FEET:     return     obj->value[0];
		case WEAR_HANDS:    return     obj->value[0];
		case WEAR_HANDS_2:  return     obj->value[0];
		case WEAR_ARMS:     return     obj->value[0];
		case WEAR_ARMS_2:   return     obj->value[0];
		case WEAR_SHIELD:   return     obj->value[0];
		case WEAR_FINGER_L: return     obj->value[0];
		case WEAR_FINGER_R: return     obj->value[0];
		case WEAR_FINGER_3: return     obj->value[0];
		case WEAR_FINGER_4: return     obj->value[0];
		case WEAR_NECK_1:   return     obj->value[0];
		case WEAR_NECK_2:   return     obj->value[0];
		case WEAR_ABOUT:    return 2 * obj->value[0];
		case WEAR_WAIST:    return     obj->value[0];
		case WEAR_WRIST_L:  return     obj->value[0];
		case WEAR_WRIST_R:  return     obj->value[0];
		case WEAR_HOLD:     return     obj->value[0];
		case WEAR_ARROW:    return     obj->value[0];
	}

	return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
	OBJ_DATA *obj;

	for ( obj = ch->carrying; obj; obj = obj->next_content )
	{
		if ( obj->deleted )
			continue;
		if ( obj->wear_loc == iWear )
		{
			return obj;
		}
	}

	return NULL;
}

int get_ppl_race_bit(int nRace)
{
	if(nRace == 35)  //  WEREWOLF
		return 11;
	if(nRace == 64)  //  MUMMY
		return 12;
	if(nRace == 65)  //  GARGOYLE
		return 13;
	if(nRace == 54)  //  CENTAUR
		return 14;
	return nRace;
}

/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
	AFFECT_DATA *paf;
	char buf [ MAX_STRING_LENGTH ];
	int ac;

	/* remove at 2022/11/27
		// bug item check
		int affect_count = 0;
		for (affect_count = 0, paf = obj->affected; paf; paf = paf->next, affect_count++ ){
		}
		if (affect_count > 20) {
			send_to_char("你無法負荷裝備上的魔力！\n\r", ch);
			act( "$p 把你彈開，掉落在地.", ch, obj, NULL, TO_CHAR );
			act( "$p 從 $n 身上彈落在地.",  ch, obj, NULL, TO_ROOM );
			obj_from_char( obj );
			obj_to_room( obj, ch->in_room );
			return;
		}
	*/

	if ( get_eq_char( ch, iWear ) )
	{
		sprintf( buf, "Equip_char: %s already equipped at %d.", ch->name, iWear );
		bug( buf, 0 );
		return;
	}

	if( is_broken(obj) || IS_OBJ_STAT( obj, ITEM_BROKEN ) ){
		act( "$p 損壞了.", ch, obj, NULL, TO_CHAR );
		return;
	}

	/*add by coconet for fix wear bug when die */
	if( !obj->carried_by )
		return;


	if (   ( IS_OBJ_STAT( obj, ITEM_NO_MALE ) && ch->sex == SEX_MALE)
		|| ( IS_OBJ_STAT( obj, ITEM_NO_FEMALE ) && ch->sex == SEX_FEMALE )
		|| ( IS_OBJ_STAT( obj, ITEM_NO_NEUTRAL) && ch->sex == SEX_NEUTRAL) )
	{
		// Thanks to Morgenes for the bug fix here!
		// act( "你的性別無法使用這樣裝備！\n\r", ch );

		if( can_drop_obj(ch, obj) ) {
			act( "$p 把你彈開，掉落在地.", ch, obj, NULL, TO_CHAR );
			act( "$p 從 $n 身上彈落在地.",  ch, obj, NULL, TO_ROOM );
			obj_from_char( obj );
			if ( IS_SET(obj->extra_flags , ITEM_PERSONAL) )
				obj_to_char( obj, ch );
			else
				obj_to_room( obj, ch->in_room );
		}else{
			act( "$p 把你彈開.", ch, obj, NULL, TO_CHAR );
			act( "$p 排斥了 $n .",  ch, obj, NULL, TO_ROOM );
			obj_from_char( obj );
			obj_to_char(obj, ch);
		}
		return;
	}

	if (   ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_EVIL   ( ch ) )
		|| ( IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) && IS_GOOD   ( ch ) )
		|| ( IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL( ch ) ) )
	{
		//Thanks to Morgenes for the bug fix here!

		if( can_drop_obj(ch, obj)) {
			act( "$p 把你彈開，掉落在地.", ch, obj, NULL, TO_CHAR );
			act( "$p 從 $n 身上彈落在地.",  ch, obj, NULL, TO_ROOM );
			obj_from_char( obj );
			if ( IS_SET(obj->extra_flags , ITEM_PERSONAL) )
				obj_to_room( obj, ch->in_room );
			else
				obj_to_char( obj, ch );
		}else{
			act( "$p 把你彈開.", ch, obj, NULL, TO_CHAR );
			act( "$p 排斥了 $n .",  ch, obj, NULL, TO_ROOM );
			obj_from_char( obj );
			obj_to_char(obj, ch);
		}
		return;
	}

	if ( ( IS_OBJ_STAT( obj, ITEM_SPECIAL ) ) )
	{
		if ( obj->material->class && !IS_NPC( ch ) )
			if (   ( IS_SET( obj->material->class, ALLOW_BIT ) && !( IS_SET( obj->material->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) )
				|| ( IS_SET( obj->material->class, DENY_BIT ) && ( IS_SET( obj->material->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) ) )
			{
				send_to_char( "你的職業無法使用這樣裝備！\n\r", ch );
				return;
			}
	}
	else
	{
		if ( obj->material->class && !IS_NPC( ch ) )
			if (   ( IS_SET( obj->material->class, ALLOW_BIT ) && !IS_SET( obj->material->class, NUM_BIT( ch->class ) ) )
				|| ( IS_SET( obj->material->class, DENY_BIT ) && IS_SET( obj->material->class, NUM_BIT( ch->class ) ) ) )
			{
				send_to_char( "你的職業無法使用這樣裝備！\n\r", ch );
				return;
			}
	}

	if ( obj->material->race && !IS_NPC( ch ) ){
		int nRaceBit = get_ppl_race_bit(ch->race);
		if (   ( IS_SET( obj->material->race, ALLOW_BIT ) && !IS_SET( obj->material->race, NUM_BIT( nRaceBit ) ) )
			|| ( IS_SET( obj->material->race, DENY_BIT ) && IS_SET( obj->material->race, NUM_BIT( nRaceBit ) ) ) )
		{
			send_to_char( "你的種族無法使用這樣裝備！\n\r", ch );
			return;
		}
	}
	if ( ( IS_OBJ_STAT( obj, ITEM_SPECIAL ) ) )
	{
		if ( obj->class && !IS_NPC( ch ) )
			if(    ( IS_SET( obj->class, ALLOW_BIT ) && !( IS_SET( obj->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) )
				|| ( IS_SET( obj->class, DENY_BIT ) && ( IS_SET( obj->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) ) )
			{
				send_to_char( "你的職業無法使用這樣裝備！\n\r", ch );
				return;
			}
	}
	else
	{
		if ( obj->class && !IS_NPC( ch ) )
			if(    ( IS_SET( obj->class, ALLOW_BIT ) && !IS_SET( obj->class, NUM_BIT( ch->class ) ) )
				|| ( IS_SET( obj->class, DENY_BIT ) && IS_SET( obj->class, NUM_BIT( ch->class ) ) ) )
			{
				send_to_char( "你的職業無法使用這樣裝備！\n\r", ch );
				return;
			}
	}

	if ( obj->race && !IS_NPC( ch ) ){
		int nRaceBit = get_ppl_race_bit(ch->race);
		if (   ( IS_SET( obj->race, ALLOW_BIT ) && !IS_SET( obj->race, NUM_BIT( nRaceBit ) ) )
			|| ( IS_SET( obj->race, DENY_BIT ) && IS_SET( obj->race, NUM_BIT( nRaceBit ) ) ) )
		{
			send_to_char( "你的種族無法使用這樣裝備！\n\r", ch );
			return;
		}
	}

	// 避免裝備後 hp mp mv 數值變為 0 造成 crash 2023/04/10
	for ( paf = obj->material->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			if( !CheckMaxHpMpMv( ch , paf ) ){
				if( paf->location == APPLY_HIT )
					act( "你的生命力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MANA )
					act( "你的魔力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MOVE )
					act( "你的體力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				act( "$n 試著穿上 $p 但好像不適合.",  ch, obj, NULL, TO_ROOM );
				return;
			}
		}
	}
	for ( paf = obj->pIndexData->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			if( !CheckMaxHpMpMv( ch , paf ) ){
				if( paf->location == APPLY_HIT )
					act( "你的生命力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MANA )
					act( "你的魔力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MOVE )
					act( "你的體力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				act( "$n 試著穿上 $p 但好像不適合.",  ch, obj, NULL, TO_ROOM );
				return;
			}
		}
	}
	for ( paf = obj->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			if( !CheckMaxHpMpMv( ch , paf ) ){
				if( paf->location == APPLY_HIT )
					act( "你的生命力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MANA )
					act( "你的魔力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MOVE )
					act( "你的體力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				act( "$n 試著穿上 $p 但好像不適合.",  ch, obj, NULL, TO_ROOM );
				return;
			}
		}
	}


	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("wear", ch, obj);
	}

	if( !obj->carried_by ) return;

	ac = apply_ac( obj, iWear );
	//if( !IS_NPC(ch) && ch->pcdata->learned[gsn_armor_mastery] > 0 ) ac += (ac/3);
	ch->armor -= ac;
	obj->wear_loc  = iWear;
	//ch->armor  -= obj->material->value[2];

	for ( paf = obj->material->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, TRUE );
			HpDeMultiply_Affect(paf);
		}
	}

	for ( paf = obj->pIndexData->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, TRUE );
			HpDeMultiply_Affect(paf);
		}
	}
	for ( paf = obj->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, TRUE );
			HpDeMultiply_Affect(paf);
		}
	}

	if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room )
		++ch->in_room->light;

	if (IsUnderFistFight(ch)) {
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	}else{
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}

	if( IS_HORSE(ch) ) {
		if(   !get_eq_char( ch, WEAR_HOLD ) && !get_eq_char( ch, WEAR_SHIELD )
			&& !get_eq_char( ch, WEAR_WIELD ) && !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_LIGHT ) && !get_eq_char( ch, WEAR_FINGER_R )
			&& !get_eq_char( ch, WEAR_FINGER_L ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_HANDS ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) )
		{
			SET_BIT(ch->act, PLR_RIDE);
		}
		else
		{
			if( ch->desc && ch->desc->original )
				affect_strip( ch->desc->original, gsn_ride);
			REMOVE_BIT(ch->act, PLR_RIDE);
		}
	}

	// 新增觸發於裝備確實在身上的wear obj_act 2020/10/24
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("bwear", ch, obj);
	}

	if( !obj->carried_by ) return;

	if ( obj->pIndexData->on_message )
		// act( obj->pIndexData->on_message, ch, obj, NULL, TO_ROOM );
		act( obj->pIndexData->on_message, ch, obj, NULL, TO_CHAR );

	if ( obj && (obj->hp||obj->material->hp) )
	{
		OBJ_HP * ohp = obj->hp;
		for( ; ohp; ohp = ohp->next )
		{
			if( IS_SET( ohp->event, HP_WEAR ) )
			{
				if ( ( !IS_NPC( ch )
						&& IS_SET( ch->act, PLR_ARTIFACT )
						&& get_skill_percent( ch, gsn_artifact) * ohp->percent * get_curr_wis( ch ) / 2500 > number_percent( ) )
					|| ( IS_NPC( ch )
						&& ohp->percent * get_curr_wis( ch ) / 25 > number_percent( ) ) )
				{
					trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, ch, ch);
				}
			}
		}
		if( obj->material->hp )
		{
			for( ohp = obj->material->hp ; ohp; ohp = ohp->next )
			{
				if( IS_SET( ohp->event, HP_WEAR ) )
				{
					if ( ( !IS_NPC( ch )
							&& IS_SET( ch->act, PLR_ARTIFACT )
							&& get_skill_percent( ch, gsn_artifact) * ohp->percent * get_curr_wis( ch ) / 2500 > number_percent( ) )
						|| ( IS_NPC( ch )
							&& ohp->percent * get_curr_wis( ch ) / 25 > number_percent( ) ) )
					{
						trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, ch, ch);
					}
				}
			}
		}
	}
	update_pos( ch );

	// 如果 ch hp 小於 0 則直接死亡, 避免 hp 為 0 crash 的情況 2023/04/10
	if( ch->max_hit <= 0 )
		raw_kill( ch, ch );

	return;
}

// 新增不受 hp , obj_act 及 obj message 影響的 obj 裝備方式 2022/11/29
void equip_char2( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
	AFFECT_DATA *paf;
	char buf [ MAX_STRING_LENGTH ];
	int ac;

	if ( get_eq_char( ch, iWear ) )
	{
		sprintf( buf, "equip_char2: %s already equipped at %d.", ch->name, iWear );
		bug( buf, 0 );
		return;
	}

	if( !obj->carried_by )
		return;

	if( is_broken(obj) || IS_OBJ_STAT( obj, ITEM_BROKEN ) ){
		act( "$p 損壞了.", ch, obj, NULL, TO_CHAR );
		return;
	}

	if( !obj->carried_by )
		return;


	if ( ( IS_OBJ_STAT( obj, ITEM_SPECIAL ) ) )
	{
		if ( obj->material->class && !IS_NPC( ch ) )
			if (   ( IS_SET( obj->material->class, ALLOW_BIT ) && !( IS_SET( obj->material->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) )
				|| ( IS_SET( obj->material->class, DENY_BIT ) && ( IS_SET( obj->material->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) ) )
			{
				return;
			}
	}
	else
	{
		if ( obj->material->class && !IS_NPC( ch ) )
			if (   ( IS_SET( obj->material->class, ALLOW_BIT ) && !IS_SET( obj->material->class, NUM_BIT( ch->class ) ) )
				|| ( IS_SET( obj->material->class, DENY_BIT ) && IS_SET( obj->material->class, NUM_BIT( ch->class ) ) ) )
			{
				return;
			}
	}

	if ( obj->material->race && !IS_NPC( ch ) ){
		int nRaceBit = get_ppl_race_bit(ch->race);
		if (   ( IS_SET( obj->material->race, ALLOW_BIT ) && !IS_SET( obj->material->race, NUM_BIT( nRaceBit ) ) )
			|| ( IS_SET( obj->material->race, DENY_BIT ) && IS_SET( obj->material->race, NUM_BIT( nRaceBit ) ) ) )
		{
			return;
		}
	}
	if ( ( IS_OBJ_STAT( obj, ITEM_SPECIAL ) ) )
	{
		if ( obj->class && !IS_NPC( ch ) )
			if(    ( IS_SET( obj->class, ALLOW_BIT ) && !( IS_SET( obj->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) )
				|| ( IS_SET( obj->class, DENY_BIT ) && ( IS_SET( obj->class, NUM_BIT( ch->class ) ) || IS_SET( obj->class, NUM_BIT( get_allow_deny_bit( ch ) ) ) ) ) )
			{
				return;
			}
	}
	else
	{
		if ( obj->class && !IS_NPC( ch ) )
			if(    ( IS_SET( obj->class, ALLOW_BIT ) && !IS_SET( obj->class, NUM_BIT( ch->class ) ) )
				|| ( IS_SET( obj->class, DENY_BIT ) && IS_SET( obj->class, NUM_BIT( ch->class ) ) ) )
			{
				return;
			}
	}

	if ( obj->race && !IS_NPC( ch ) ){
		int nRaceBit = get_ppl_race_bit(ch->race);
		if (   ( IS_SET( obj->race, ALLOW_BIT ) && !IS_SET( obj->race, NUM_BIT( nRaceBit ) ) )
			|| ( IS_SET( obj->race, DENY_BIT ) && IS_SET( obj->race, NUM_BIT( nRaceBit ) ) ) )
		{
			return;
		}
	}

	// 避免裝備後 hp mp mv 數值變為 0 造成 crash 2023/04/10
	for ( paf = obj->material->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			if( !CheckMaxHpMpMv( ch , paf ) ){
				if( paf->location == APPLY_HIT )
					act( "你的生命力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MANA )
					act( "你的魔力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MOVE )
					act( "你的體力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				act( "$n 試著穿上 $p 但好像不適合.",  ch, obj, NULL, TO_ROOM );
				return;
			}
		}
	}
	for ( paf = obj->pIndexData->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			if( !CheckMaxHpMpMv( ch , paf ) ){
				if( paf->location == APPLY_HIT )
					act( "你的生命力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MANA )
					act( "你的魔力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MOVE )
					act( "你的體力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				act( "$n 試著穿上 $p 但好像不適合.",  ch, obj, NULL, TO_ROOM );
				return;
			}
		}
	}
	for ( paf = obj->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			if( !CheckMaxHpMpMv( ch , paf ) ){
				if( paf->location == APPLY_HIT )
					act( "你的生命力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MANA )
					act( "你的魔力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				if( paf->location == APPLY_MOVE )
					act( "你的體力不足以穿上$p.", ch, obj, NULL, TO_CHAR );
				act( "$n 試著穿上 $p 但好像不適合.",  ch, obj, NULL, TO_ROOM );
				return;
			}
		}
	}

	if( !obj->carried_by ) return;

	ac = apply_ac( obj, iWear );
	//if( !IS_NPC(ch) && ch->pcdata->learned[gsn_armor_mastery] > 0 ) ac += (ac/3);
	ch->armor -= ac;
	obj->wear_loc  = iWear;
	//ch->armor  -= obj->material->value[2];

	for ( paf = obj->material->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, TRUE );
			HpDeMultiply_Affect(paf);
		}
	}

	for ( paf = obj->pIndexData->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, TRUE );
			HpDeMultiply_Affect(paf);
		}
	}
	for ( paf = obj->affected; paf; paf = paf->next ){
		if (!is_affected(ch,paf->type)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, TRUE );
			HpDeMultiply_Affect(paf);
		}
	}

	if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room )
		++ch->in_room->light;

	if (IsUnderFistFight(ch)) {
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	}else{
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}

	if( IS_HORSE(ch) ) {
		if(   !get_eq_char( ch, WEAR_HOLD ) && !get_eq_char( ch, WEAR_SHIELD )
			&& !get_eq_char( ch, WEAR_WIELD ) && !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_LIGHT ) && !get_eq_char( ch, WEAR_FINGER_R )
			&& !get_eq_char( ch, WEAR_FINGER_L ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_HANDS ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) )
		{
			SET_BIT(ch->act, PLR_RIDE);
		}
		else
		{
			if( ch->desc && ch->desc->original )
				affect_strip( ch->desc->original, gsn_ride);
			REMOVE_BIT(ch->act, PLR_RIDE);
		}
	}

	// 如果 ch hp 小於 0 則直接死亡, 避免 hp 為 0 crash 的情況 2023/04/10
	if( ch->max_hit <= 0 )
		raw_kill( ch, ch );

	return;
}

// get the base class
// allow/deny bit of a advance class
int get_allow_deny_bit( CHAR_DATA *ch )
{
	switch ( ch->pcdata->prime_attr )
	{
		case APPLY_STR: return 3;
		case APPLY_DEX: return 2;
		case APPLY_INT: return 0;
		case APPLY_WIS: return 1;
	}
	return 4;
}
/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
	int ac;
	AFFECT_DATA *paf;
	char buf [ MAX_STRING_LENGTH ];
	int	chk_cast;

	if( obj->wear_loc == WEAR_NONE )
	{
		sprintf( buf, "Unequip_char: %s already unequipped with %d.",ch->name, obj->pIndexData->vnum );
		bug( buf, 0 );
		return;
	}

	ac = apply_ac( obj, obj->wear_loc );
	//if( !IS_NPC(ch) && ch->pcdata->learned[gsn_armor_mastery] > 0 )
	//  ac += (ac/3);
	ch->armor += ac;
	obj->wear_loc  = -1;

	for( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		chk_cast = skill_lookup(affect_bit_name(paf->bitvector));
		if( !is_affected( ch, chk_cast)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, FALSE );
			HpDeMultiply_Affect(paf);
		}
	}

	for( paf = obj->material->affected; paf; paf = paf->next ){
		HpMultiply_Affect(paf);
		affect_modify( ch, paf, FALSE );
		HpDeMultiply_Affect(paf);
	}

	for( paf = obj->affected; paf; paf = paf->next ){
		HpMultiply_Affect(paf);
		affect_modify( ch, paf, FALSE );
		HpDeMultiply_Affect(paf);
	}

	if( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room && ch->in_room->light > 0 )
		--ch->in_room->light;

	if (IsUnderFistFight(ch)) {
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	} else {
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}

	if( IS_HORSE(ch) ) 
	{
		if(    !get_eq_char( ch, WEAR_HOLD ) && !get_eq_char( ch, WEAR_SHIELD )
			&& !get_eq_char( ch, WEAR_WIELD ) && !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_LIGHT ) && !get_eq_char( ch, WEAR_FINGER_R )
			&& !get_eq_char( ch, WEAR_FINGER_L ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_HANDS ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) && !get_eq_char( ch, WEAR_ABOUT) )
		{
			SET_BIT(ch->act, PLR_RIDE);
		}
		else
		{
			if( ch->desc && ch->desc->original )
				affect_strip( ch->desc->original, gsn_ride);

			REMOVE_BIT(ch->act, PLR_RIDE);
		}
	}

	if( obj->pIndexData->off_message )
		act( obj->pIndexData->off_message, ch, obj, NULL, TO_CHAR );
		// act( obj->pIndexData->off_message, ch, obj, NULL, TO_ROOM );

	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("remove", ch, obj);
	}

	// 如果 ch hp 小於 0 則直接死亡, 避免 hp 為 0 crash 的情況 2023/04/10
	if( ch->max_hit <= 0 )
		raw_kill( ch, ch );

	return;
}
// 新增不受 obj_act 及 obj message 影響的 obj 脫除方式 2022/11/29
void unequip_char2( CHAR_DATA *ch, OBJ_DATA *obj )
{
	int ac;
	AFFECT_DATA *paf;
	char buf [ MAX_STRING_LENGTH ];
	int	chk_cast;

	if( obj->wear_loc == WEAR_NONE )
	{
		sprintf( buf, "unequip_char2: %s already unequipped with %d.",ch->name, obj->pIndexData->vnum );
		bug( buf, 0 );
		return;
	}

	ac = apply_ac( obj, obj->wear_loc );
	//if( !IS_NPC(ch) && ch->pcdata->learned[gsn_armor_mastery] > 0 )
	//  ac += (ac/3);
	ch->armor += ac;
	obj->wear_loc  = -1;

	for( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		chk_cast = skill_lookup(affect_bit_name(paf->bitvector));
		if( !is_affected( ch, chk_cast)){
			HpMultiply_Affect(paf);
			affect_modify( ch, paf, FALSE );
			HpDeMultiply_Affect(paf);
		}
	}

	for( paf = obj->material->affected; paf; paf = paf->next ){
		HpMultiply_Affect(paf);
		affect_modify( ch, paf, FALSE );
		HpDeMultiply_Affect(paf);
	}

	for( paf = obj->affected; paf; paf = paf->next ){
		HpMultiply_Affect(paf);
		affect_modify( ch, paf, FALSE );
		HpDeMultiply_Affect(paf);
	}

	if( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room && ch->in_room->light > 0 )
		--ch->in_room->light;

	if (IsUnderFistFight(ch)) {
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	} else {
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}

	if( IS_HORSE(ch) ) 
	{
		if(    !get_eq_char( ch, WEAR_HOLD ) && !get_eq_char( ch, WEAR_SHIELD )
			&& !get_eq_char( ch, WEAR_WIELD ) && !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_LIGHT ) && !get_eq_char( ch, WEAR_FINGER_R )
			&& !get_eq_char( ch, WEAR_FINGER_L ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_HANDS ) && !get_eq_char( ch, WEAR_FINGER_3 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) && !get_eq_char( ch, WEAR_ABOUT) )
		{
			SET_BIT(ch->act, PLR_RIDE);
		}
		else
		{
			if( ch->desc && ch->desc->original )
				affect_strip( ch->desc->original, gsn_ride);

			REMOVE_BIT(ch->act, PLR_RIDE);
		}
	}

	// 如果 ch hp 小於 0 則直接死亡, 避免 hp 為 0 crash 的情況 2023/04/10
	if( ch->max_hit <= 0 )
		raw_kill( ch, ch );

	return;
}

/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
	OBJ_DATA *obj;
	int       nMatch;

    nMatch = 0;
	for ( obj = list; obj; obj = obj->next_content )
	{
		if ( obj->deleted )
			continue;
		if ( obj->pIndexData == pObjIndex )
		nMatch++;
	}

	return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
	ROOM_INDEX_DATA *in_room;

	// obj 被 purge 掉的情況 2022/02/13
	if ( obj->deleted )
	{
		return;
	}

	if ( !( in_room = obj->in_room ) )
	{
		bug( "obj_from_room: NULL.", 0 );
		return;
	}

	if ( obj == in_room->contents )/* obj is the first obj in this room?*/
	{
		in_room->contents = obj->next_content;
	}
	else /*looking the in_room->contents for this obj*/
	{
		OBJ_DATA *prev;

		for ( prev = in_room->contents; prev; prev = prev->next_content )
		{
			if ( prev->next_content == obj )
			{
				prev->next_content = obj->next_content;
				break;
			}
		}

		if ( !prev )
		{
			bug( "Obj_from_room: obj not found.", 0 );
			return;
		}
	}

	// 在房間地上的光源被移除後減少房間中的光源數 2022/01/16
	if( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && obj->in_room->light > 0 )
		--obj->in_room->light;

	obj->in_room      = NULL;
	obj->next_content = NULL;
	return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
	// obj 被 purge 掉的情況 2022/02/13
	if ( obj->deleted )
	{
		return;
	}
    obj->next_content   = pRoomIndex->contents;
    pRoomIndex->contents  = obj;
    obj->in_room    = pRoomIndex;
    obj->carried_by   = NULL;
    obj->in_obj     = NULL;
	// 在房間地上的光源增加房間中的光源數 2022/01/16
	if( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && obj->in_room )
		++obj->in_room->light;
    return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
	OBJ_DATA *top_obj;
	// obj 被 purge 掉的情況 2022/02/13
	if ( obj->deleted )
	{
		return;
	}
	if ( obj_to->deleted )
	{
		bug( "Obj_to_obj:  Obj_to already deleted", 0 );
		return;
	}

	for(top_obj = obj_to ; top_obj ; top_obj = top_obj->in_obj ){
		if( !top_obj->in_obj )
			break;
	}
    
	//add by coconet 0522 for horse
	// obj_to 修改為 top_obj , 避免 login 時馬身上容器裝有的物品反應到 char 2022/01/02
	//if( obj_to->item_type == ITEM_HORSE )
	if( top_obj->item_type == ITEM_HORSE )
		;
	else if( top_obj->carried_by )
	{
		top_obj->carried_by->carry_number -= get_obj_number( top_obj );
		// 普通 mob put 物品進身上容器時重量不增減 2023/02/09
		if( !IS_NPC( top_obj->carried_by ) || ( IS_NPC( top_obj->carried_by ) && IS_SET( top_obj->carried_by->act, ACT_PET ) ) )
			top_obj->carried_by->carry_weight -= get_obj_weight( top_obj );
	}

	obj->next_content = obj_to->contains;
	obj_to->contains  = obj;
	obj->in_obj       = obj_to;
	obj->in_room      = NULL;
	obj->carried_by   = NULL;

	//if ( top_obj->carried_by && top_obj->item_type != ITEM_HORSE )
	// obj_to 修改為 top_obj  2022/01/02
	if ( top_obj->carried_by && top_obj->item_type != ITEM_HORSE )
	{
		top_obj->carried_by->carry_number += get_obj_number( top_obj );
		// 普通 mob put 物品進身上容器時重量不增減 2023/02/09
		if( !IS_NPC( top_obj->carried_by ) || ( IS_NPC( top_obj->carried_by ) && IS_SET( top_obj->carried_by->act, ACT_PET ) ) )
			top_obj->carried_by->carry_weight += get_obj_weight( top_obj );
	}
	return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
	OBJ_DATA *obj_from;

	// obj 被 purge 掉的情況 2022/02/13
	if ( obj->deleted )
	{
		return;
	}

	if ( !( obj_from = obj->in_obj ) )
	{
		bug( "Obj_from_obj: null obj_from.", 0 );
		return;
	}
	if( /*obj_from->item_type == ITEM_CONTAINER &&*/ obj_from->carried_by && obj_from->item_type != ITEM_HORSE ) 
	{
		obj_from->carried_by->carry_number -= get_obj_number( obj_from );
		obj_from->carried_by->carry_weight -= get_obj_weight( obj_from );
	}


	if ( obj == obj_from->contains )
	{
		obj_from->contains = obj->next_content;
	}
	else
	{
		OBJ_DATA *prev;

		for ( prev = obj_from->contains; prev; prev = prev->next_content )
		{
			if ( prev->next_content == obj )
			{
				prev->next_content = obj->next_content;
				break;
			}
		}

		if ( !prev )
		{
			bug( "Obj_from_obj: obj not found.", 0 );
			return;
		}
	}

	obj->next_content = NULL;
	obj->in_obj       = NULL;

	for ( ; obj_from; obj_from = obj_from->in_obj )
	{
		if ( obj_from->deleted )
			continue;
		if ( obj_from->carried_by && obj_from->item_type != ITEM_HORSE )
		{
			obj_from->carried_by->carry_number += get_obj_number( obj_from );
			obj_from->carried_by->carry_weight += get_obj_weight( obj_from );
		}
	}

	return;
}



/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA *obj )
{
	OBJ_DATA *obj_content;
	OBJ_DATA *obj_next;
	extern bool delete_obj;

	if ( obj->deleted )
	{
		bug( "Extract_obj:  Obj already deleted", 0 );
		return;
	}

	if ( obj->in_room )
		obj_from_room( obj );
	else if ( obj->carried_by )
		obj_from_char( obj );
	else if ( obj->in_obj )
		obj_from_obj( obj  );

	for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
	{
		obj_next = obj_content->next_content;
		if( obj_content->deleted )
			continue;
		extract_obj( obj_content );
	}

	obj->deleted = TRUE;

	delete_obj   = TRUE;
	return;
}

void extract_bank_obj( OBJ_DATA *obj )
{
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	EXTRA_DESCR_DATA *ed;
	EXTRA_DESCR_DATA *ed_next;

	for ( ed = obj->extra_descr; ed; ed = ed_next )
	{
		ed_next = ed->next;

		free_string( ed->description );
		free_string( ed->keyword );
		ed->next = extra_descr_free;
		extra_descr_free = ed;
	}

	for ( paf = obj->affected; paf; paf = paf_next )
	{
		paf_next = paf->next;

		if ( obj->affected == paf )
		{
			obj->affected = paf->next;
		}
		else
		{
			AFFECT_DATA *prev;

			for ( prev = obj->affected; prev; prev = prev->next )
			{
				if ( prev->next == paf )
				{
					prev->next = paf->next;
					break;
				}
			}

			if ( !prev )
			{
				bug( "List_update: cannot find paf on obj.", 0 );
				continue;
			}
		}  
		paf->next   = affect_free;
		affect_free = paf;
	}

	obj->next_content = NULL;

	free_string( obj->name );
	free_string( obj->description );
	free_string( obj->short_descr );
	/* free_string( obj->cname );*/
	--obj->pIndexData->count;
	obj->next = obj_free;
	obj_free  = obj;
}

/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA *ch, bool fPull )
{
	int recall;
	CHAR_DATA *wch;
	CHAR_DATA *vch = NULL, *rch= NULL;
	QMARK *qmark;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	//GROUP_DATA *pG;
	ROOM_INDEX_DATA *room;
	char buf[ MAX_INPUT_LENGTH ];
	int death;
	int i;
	extern bool delete_char;

	if( !ch->in_room )
	{
		bug( "Extract_char: NULL. Char name: %s", ch->name );
		return;
	}

	REMOVE_BIT(ch->affected_by, AFF_BATTLE);

	if( ch->fighting )
		stop_fighting( ch, TRUE );

	if( IS_SET(ch->act, PLR_DEAD) ) 
	{
		//do_follow(ch, "self");
		remove_member(ch);
	}

	//hunting
	if( ( vch = ch->hunting ) )
	{
		ch->hunting = NULL;
		if( vch->hunted == ch )
		{
			vch->hunted = ch->next_in_hunt;
		}
		else
		{
			for( rch = vch->hunted; rch; rch = rch->next_in_hunt )
			{
				if( rch->next_in_hunt == ch )
				{
					rch->next_in_hunt = ch->next_in_hunt;
					break;
				}
			}
		}

		ch->next_in_hunt = NULL;
		act( "$n 停止獵殺你", ch, NULL, vch, TO_VICT );
		act( "你停止獵殺 $N", ch, NULL, vch, TO_CHAR );
	}

	//hunted
	if( ( ch->hunted ) )
	{
		for( vch = ch->hunted; vch; vch = rch )
		{
			rch = vch->next_in_hunt;
			vch->hunting = NULL;
			vch->next_in_hunt = NULL;
			act( "$N 停止獵殺你", ch, NULL, vch, TO_CHAR );
			act( "你停止獵殺 $n", ch, NULL, vch, TO_VICT );
		}
		ch->hunted = NULL;
	}

	//assassinating
	if( ( vch = ch->assassinating ) )
	{
		ch->assassinating = NULL;
		if( vch->assassinated == ch )
		{
			vch->assassinated = ch->next_in_ass;
		}
		else 
		{
			for( rch = vch->assassinated; rch; rch = rch->next_in_ass )
			{
				if( rch->next_in_ass == ch )
				{
					rch->next_in_ass = ch->next_in_ass;
					break;
				}
			}
		}
		ch->next_in_ass = NULL;
		act( "你停止暗殺 $N", ch, NULL, vch, TO_CHAR );
	}

	//assassinated
	if( ( ch->assassinated ) )
	{
		for( vch = ch->assassinated; vch; vch = rch )
		{
			rch = vch->next_in_ass;
			vch->assassinating = NULL;
			vch->next_in_ass = NULL;
			act( "你停止暗殺 $n", ch, NULL, vch, TO_VICT );
		}
	}

	//tracking
	if( (ch->tracking))
	{
		vch = ch->tracking;
		ch->tracking = NULL;
		if( vch->tracked == ch ) 
		{
			vch->tracked = ch->next_in_track;
		}
		else 
		{
			for( rch = vch->tracked ; rch; rch = rch->next_in_track )
			{
				if( rch->next_in_track == ch ) 
				{
					rch->next_in_track = ch->next_in_track;
				}
			}
		}
		ch->next_in_track = NULL;
	}

	//tracked
	if( ( ch->tracked ) )
	{
		for( vch = ch->tracked; vch; vch = rch )
		{
			rch = vch->next_in_track;
			vch->tracking = NULL;
			vch->next_in_track = NULL;
			// act( "$N 停止追蹤你", ch, NULL, vch, TO_CHAR );
			act( "你停止追蹤 $n", ch, NULL, vch, TO_VICT );
		}
		ch->tracked = NULL;
	}

	if( ( vch = ch->rembch ) )
		stop_rember(ch, vch);

	if( ( room = ch->rrembed ) )
		remove_rrember( ch);

	// add at 2021/11/20
	if( ( vch = ch->linking ) )
	{
		stop_linking( ch, vch );
	}

	// cooperating add at 2021/11/20
	if( ( vch = ch->cooperator ) )
	{
		remove_cooperator(ch);
	}
	if( ( vch = ch->cooperating ) )
	{
		stop_cooperating(ch, vch);
	}

	// protect target
	if( ( vch = ch->protector ) )
	{
		remove_protector(ch);
	}
	if( ( vch = ch->protecting ) )
	{
		stop_protecting(ch, vch);
	}

	//clan_war
	if( !IS_NPC( ch ) && ch->pcdata->clanpk == 1 )
	{
		if( clan_war->fighting < 2 )
		{
			if( clan_war->clan_a == ch->clan )
			{
				for( i=0; i<10; i++){
					if( clan_war->clan_a_member[i] == ch )
						clan_war->clan_a_member[i] = NULL;
				}

				clan_war->clan_a_num --;
				if( clan_war->clan_a_num == 0 )
				{
					clan_war->fighting = 0;
					clan_war->clan_a = NULL;
					clan_war->clan_b = NULL;
				}
			}
			else
			{
				for( i=0; i<10; i++){
					if( clan_war->clan_a_member[i] == ch )
						clan_war->clan_a_member[i] = NULL;
				}

				clan_war->clan_b_num --;
				if( clan_war->clan_b_num == 0 )
				{
					clan_war->fighting = 0;
					clan_war->clan_a = NULL;
					clan_war->clan_b = NULL;
				}
			}
		}
		else
		{
			sprintf( buf, "%s 的膽小鬼 %s 背棄自己的幫會，逃離戰場！\n\r",
			ch->clan->name, ch->name );
			send_to_all_char( buf );
			ch->pcdata->clanpk = 0;
			if( ch->clan == clan_war->clan_a )
			{
				clan_war->clan_a_num --;
				for( i=0; i < 10; i++){
					if( ch == clan_war->clan_a_member[i] )
						clan_war->clan_a_member[i] = NULL;
				}
			}
			else
			{
				clan_war->clan_b_num --;
				for( i=0; i < 10; i++){
					if( ch == clan_war->clan_b_member[i] )
						clan_war->clan_b_member[i] = NULL;
				}
			}
			if( clan_war->clan_b_num == 0 )
			{
				sprintf( buf, "戰神馬爾斯宣布：%s 擊敗強敵 %s, 獲得本次戰役的勝利！\n\r",
				clan_war->clan_a->name, clan_war->clan_b->name );
				send_to_all_char( buf );
				clan_war->clan_a->bank += 1000000;
				clan_war->clan_a->warwin ++;
				clan_war->clan_b->warlose ++;
				for( i=0; i < 10; i++)
				{
					if( clan_war->clan_a_member[i] )
					{
						clan_war->clan_a_member[i]->pcdata->clanpk = 0;
						clan_war->clan_a_member[i] = NULL;
						clan_war->clan_b_member[i] = NULL;
					}
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
				for( i=0; i < 10; i++)
				{
					if( clan_war->clan_b_member[i] )
					{
						clan_war->clan_b_member[i]->pcdata->clanpk = 0;
						clan_war->clan_b_member[i] = NULL;
						clan_war->clan_a_member[i] = NULL;
					}
				}
				clan_war->fighting = 0;
				clan_war->clan_a = NULL;
				clan_war->clan_b = NULL;
				clan_war->clan_b_num = 0;
			}
		}
	}

	/*
	* Added by Razgriz 20050701
	* for update qmark "max_gate_mobs"
	*/
	if( IS_NPC( ch ) && ch->gater && IS_NPC( ch->gater )) 
	{
		if( check_qmark ( ch->gater, "max_gate_mobs" ) )
			do_mpmod_qmark ( ch->gater, "self max_gate_mobs v0 - 1" );
		ch->gater = NULL;
	}

	if( ch->qmark ){
		for( qmark = ch->qmark; qmark; qmark = qmark->next ){
			if( qmark->fDeath || IS_NPC( ch ) )
				del_qmark( qmark ); 
		}
	}

	//auction
	if( auction_list )
	{
		AUC_DATA *auction, *auc_next = NULL;
		for( auction = auction_list ; auction ; auction = auc_next )
		{
			if( ch == auction->owner )
			{
				talk_channel( auction->owner , "因為本人在拍賣中途死亡，拍賣因此中止。", CHANNEL_AUCTION, "【拍賣】",HIY ); 
				obj_to_char( auction->obj, auction->owner );
				if( auction == auction_list ) 
				{
					auction_list = auction_list->next;
				}
				else 
				{
					for( auc_next = auction_list ; auc_next; auc_next = auc_next->next )
					{
						if( auc_next->next == auction )
						{
							auc_next->next = auction->next;
							auc_next = auction->next;
							break;
						}
					}
				}
				//[S] Modified by Tale on 2012/04/05, return gold to buyer once the auction is terminated abnormally.
				if( auction->buyer ) {
					auction->buyer->gold += auction->price;
				}
				//[E] Modified by Tale on 2012/04/05, return gold to buyer once the auction is terminated abnormally.
				free_auction_data(auction);
				continue;
			}


			if( ch == auction->buyer )
			{
				talk_channel( auction->owner , "因為出價者在拍賣中途死亡，拍賣因此中止。", CHANNEL_AUCTION, "【拍賣】",HIY ); 
				obj_to_char( auction->obj, auction->owner );
				if( auction == auction_list ) 
				{
					auction_list = auction_list->next;
				}
				else
				{
					for( auc_next = auction_list ; auc_next; auc_next = auc_next->next )
					{
						if( auc_next->next == auction )
						{
							auc_next->next = auction->next;
							auc_next = auction->next;
							break;
						}
					}
				}
				//[S] Modified by Tale on 2012/04/05, return gold to buyer once the auction is terminated abnormally.
				if( auction->buyer ) {
					auction->buyer->gold += auction->price;
				}
				//[E] Modified by Tale on 2012/04/05, return gold to buyer once the auction is terminated abnormally.
				free_auction_data(auction);
				continue;
			}
			auc_next = auction->next;
		}
	}

	if( fPull )
	{
		char* name;

		if( IS_NPC ( ch ) )
			name = ch->short_descr;
		else
			name = ch->name;

		die_follower( ch, name );

		if( ( vch = ch->rembed ) )
			remove_rember(ch);

		if( ch->group ) {
			remove_member(ch);
		}

		/* Get rid of weapons _first_ 
		- from Erwin Andreasen <erwin@pip.dknet.dk> */

		{
		OBJ_DATA *obj, *obj2, *obj3;

		obj  = get_eq_char( ch, WEAR_WIELD   );
		obj2 = get_eq_char( ch, WEAR_WIELD_2 );
		obj3 = get_eq_char( ch, WEAR_TWO_HAND );

		if( obj )
			extract_obj( obj );

		/* Now kill obj2 if it exists no matter if on body or floor */
		if( obj2 )
			extract_obj( obj2 );
		if( obj3 )
			extract_obj( obj3 );

		}

		for( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;
			if( obj->deleted )
				continue;
			extract_obj( obj );
		}

		if( ch->eqbank )
		{
			for( obj = ch->eqbank; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				obj->next = object_list;
				object_list = obj;
				obj->next_content = NULL;
				obj->deleted = TRUE;
			}
		}
	}

	recall = ch->in_room->area->recall;
	death = ch->in_room->area->death;

	//Modified by Razgriz   20050709
	if(       IS_NPC ( ch )
		|| ( !IS_NPC ( ch ) && ch->desc && ch->desc->connected == CON_PLAYING )
		|| ( !IS_NPC ( ch ) && !ch->desc ) )
	char_from_room( ch );

	if( !fPull )
	{
		ROOM_INDEX_DATA *location;
		ch->position = POS_RESTING;

		if( !IS_NPC (ch) && ch->pcdata )            //Added by Razgriz  20050630
			ch->pcdata->condition[COND_DRUNK] = 0;  //Added by Razgriz  20050630

		// if( IS_SET(ch->act, PLR_DEAD) ) {
		// do_follow(ch, "self");
		// }

		if( recall == ROOM_VNUM_TEMPLE )
		{
			if( ( location = get_room_index( death ) ) )
				char_to_room( ch, location );
			else if ( ( location = get_room_index( ROOM_VNUM_ALTAR ) ) )
				char_to_room( ch, location );
			else
			{
				bug( "Area death/recall room does not exist!", 0 );
				char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ) );
			}
		}
		else
		{
			if( ( location = get_room_index( death ) ) )
			char_to_room( ch, location );
			else if ( ( location = get_room_index( recall ) ) )
			char_to_room( ch, location );
			else
			{
			bug( "Area death/recall room does not exist!", 0 );
			char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ) );
			}
		}
		return;
	}

	if( IS_NPC( ch ) )
		--ch->pIndexData->count;
	else if( IS_SET( ch->act, PLR_HORSE ) )
	{
		if( ch->pcdata->horse )
		{
			ch->pcdata->horse->pcdata->ride = 0;
			ch->pcdata->horse->pcdata->horse = NULL;
		}
		ch->pcdata->horse = NULL;
	}

	if( ch->desc && ch->desc->original )
		do_return( ch, "" );

	for( wch = char_list; wch; wch = wch->next )
	{
		if( wch->reply == ch )
			wch->reply = NULL;
	}

	ch->position = POS_DEAD;
	ch->deleted = TRUE;

	if( ch->desc )
		ch->desc->character = NULL;

	delete_char = TRUE;
	return;
}



/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *rch;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	number = number_argument( argument, arg );
	count  = 0;
	if( !str_cmp(arg, "someone"))
		return NULL;
	if ( !str_cmp( arg, "self" ) )
		return ch;
	for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	{
		if ( !can_see( ch, rch ) || !is_name( arg, rch->name ) || rch->deleted)
			continue;
		if ( ++count == number )
			return rch;
	}
	for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	{
		if ( !can_see( ch, rch ) || !is_pfxname( arg, rch->name ) || rch->deleted)
			continue;
		if ( ++count == number )
			return rch;
	}

	return NULL;
}




/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *wch;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if( !str_cmp(arg, "someone"))
		return NULL;
	if ( ( wch = get_char_room( ch, argument ) ) )
		return wch;

	number = number_argument( argument, arg );
	count  = 0;
	for ( wch = char_list; wch ; wch = wch->next )
	{
		if ( !can_see( ch, wch ) || !is_name( arg, wch->name )|| wch->deleted )
			continue;
		if ( ++count == number )
			return wch;
	}
	for ( wch = char_list; wch ; wch = wch->next )
	{
		if ( !can_see( ch, wch ) || !is_pfxname( arg, wch->name ) || wch->deleted)
			continue;
		if ( ++count == number )
			return wch;
	}

	return NULL;
}

/*
 * Added by Razgriz	20050831
 * Find a char in the world whether ch can see wch or not.
 */
CHAR_DATA *get_char_world2( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *wch;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if ( !str_cmp (arg, "someone") )
		return NULL;
	if ( ( wch = get_char_room( ch, argument ) ) )
	   return wch;

	number = number_argument ( argument, arg );
	count  = 0;
	for ( wch = char_list; wch ; wch = wch->next )
	{
		if ( !is_name ( arg, wch->name ) || wch->deleted )
			continue;
		if ( ++count == number )
			return wch;
	}
	for ( wch = char_list; wch ; wch = wch->next )
	{
		if ( !is_pfxname ( arg, wch->name ) || wch->deleted)
			continue;
		if ( ++count == number )
			return wch;
	}

	return NULL;
}

/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
	OBJ_DATA *obj;

	for ( obj = object_list; obj; obj = obj->next )
	{
		if ( obj->deleted )
			continue;

		if ( obj->pIndexData == pObjIndex )
			return obj;
	}

	return NULL;
}

int get_max_dura(OBJ_DATA *obj)
{
	OBJ_INDEX_DATA * pOID = obj->pIndexData;

	switch( obj->item_type )
	{
		case ITEM_ARMOR:
			//modified by coconet 20041012
			if( obj->value[3] > 0 )
				return obj->value[3];
			else if( pOID->value[2] == DURA_AUTO )
				return pOID->material->value[3];
			else if( pOID->value[2] > DURA_MAX ){
				int du = pOID->value[2];
				//du -= ( du % 10000 );
				return (du % 1000);
			}
			else
				return pOID->value[2];
			break;
		case ITEM_WEAPON:
			if( obj->value[3] == 14 /* bow */)
				return DURA_INDESTRUCTIBLE;
			if( pOID->value[0] == DURA_AUTO ){
				return pOID->material->value[3];
			}else if( pOID->value[0] > DURA_MAX ){
				int du = pOID->value[0];
				//du -= ( du % 10000 );
				return (du % 1000);
			}
			else
				return pOID->value[0];
			break;
		default:
			return DURA_INDESTRUCTIBLE;
	}
	return 1;
}


int get_curr_dura( OBJ_DATA * obj )
{
	switch( obj->item_type ) {
		case ITEM_ARMOR:
			if( obj->value[3] != 14 /* bow */)
				return obj->value[2];
			else
				return DURA_INDESTRUCTIBLE;
			break;
		case ITEM_WEAPON:
			return obj->value[0];
			break;
		default:
			return DURA_INDESTRUCTIBLE;
	}
	return 1;
}


bool indestructible( OBJ_DATA *obj )
{
	if( get_max_dura(obj) == -1 )
		return TRUE;
	if(get_curr_dura(obj) == -1 )
		return TRUE;
	return FALSE;
}

bool is_broken( OBJ_DATA *obj )
{
	if(indestructible(obj) )
		return FALSE;
	else if(get_curr_dura(obj) == 0 )
		return TRUE;
	return FALSE;
}

void waste_durax( OBJ_DATA *obj )
{
	if( is_broken(obj) || indestructible(obj))
		return;
	if( number_bits(1) == 1 )
		return; //50%
	if( IS_SET(obj->extra_flags, ITEM_HUM) && number_bits(2) != 1 ) // 25%
		return; //12.5% ???

	switch( obj->item_type )
	{
		case ITEM_ARMOR:
			obj->value[2] -= 10;
			break;
		case ITEM_WEAPON:
			obj->value[0] -= 10;
			break;
	}
}

void waste_dura( OBJ_DATA *obj )
{
	if( is_broken(obj) || indestructible(obj))
		return;
	// 附魔物品損壞機率提高 2021/09/30
	if( !IS_SET(obj->extra_flags, ITEM_SPELL) && number_bits(1) == 1 )
		return; //50%
	if( IS_SET(obj->extra_flags, ITEM_HUM) && number_bits(2) != 1 ) // 25%
		return; //12.5% ???

	switch( obj->item_type )
	{
		case ITEM_ARMOR:
			obj->value[2] -= 1;
			break;
		case ITEM_WEAPON:
			obj->value[0] -= 1;
			break;
	}
}

void repair_dura( OBJ_DATA *obj ,int value)
{
	if(indestructible(obj))
		return;

	switch( obj->item_type )
	{
		case ITEM_ARMOR:
			obj->value[2] += value;
			obj->value[2] = UMIN( get_max_dura(obj), obj->value[2]);
			break;
		case ITEM_WEAPON:
			obj->value[0] += value;
			obj->value[0] = UMIN( get_max_dura(obj), obj->value[0]);
			break;
		default:
			break;
	}
	return;
}

/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	number = number_argument( argument, arg );
	count  = 0;
	for ( obj = list; obj; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
		{
			if ( ++count == number )
				return obj;
		}
	}
	for ( obj = list; obj; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) && is_pfxname( arg, obj->name ) )
		{
			if ( ++count == number )
				return obj;
		}
	}

	return NULL;
}

/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if( !ch ) return NULL;

	if ( !is_number( argument ) )
	{
		number = number_argument( argument, arg );
		count  = 0;
		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if ( obj->wear_loc == WEAR_NONE
				&& can_see_obj( ch, obj )
				&& is_pfxname( arg, obj->name ) )
			{
				if ( ++count == number )
					return obj;
			}
		}
	}
	else
	{
		number = atoi( argument );
		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if ( obj->pIndexData->vnum == number 
				&& obj->wear_loc == WEAR_NONE
				&& can_see_obj( ch, obj ))
				return obj;
		}
	}

	return NULL;
}

// 判定 ch 身上所有的 obj 2023/01/08
OBJ_DATA *get_char_obj( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if( !ch ) return NULL;

	if ( !is_number( argument ) )
	{
		number = number_argument( argument, arg );
		count  = 0;
		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if( !obj ) continue;
			if ( is_pfxname( arg, obj->name ) )
			{
				if ( ++count == number )
					return obj;
			}
		}
	}
	else
	{
		number = atoi( argument );
		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if( !obj ) continue;
			if ( obj->pIndexData->vnum == number )
				return obj;
		}
	}

	return NULL;
}

/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if( !ch ) return NULL;     //Added by Razgriz 20050713

	if( !is_number( argument ) )
	{
		number = number_argument( argument, arg );
		count  = 0;
		for( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if( !obj ) continue;
			if( obj->wear_loc != WEAR_NONE
				&& can_see_obj( ch, obj )
				&& is_pfxname( arg, obj->name ) )
			{
				if( ++count == number )
					return obj;
			}
		}
	}
	else
	{
		number = atoi( argument );
		for( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if( !obj ) continue;
			if( obj->wear_loc != WEAR_NONE
				&& can_see_obj( ch, obj )
				&& obj->pIndexData->vnum == number )
				return obj;
		}
	}
	return NULL;
}

OBJ_DATA *get_obj_wear2( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg[ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if( !is_number ( argument ) )
	{
		number = number_argument ( argument, arg );
		count  = 0;
		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if ( !obj ) continue;
			// if ( GLOBAL_OBJ_UPDATE && obj->update_checked ) continue; //Mark by Razgriz 20050702
			if ( obj->wear_loc != WEAR_NONE && is_pfxname( arg, obj->name ))
			{
				//Added by Razgriz  20050702
				if ( GLOBAL_OBJ_UPDATE && ( obj->pIndexData->vnum == updating_obj->pIndexData->vnum ) && ( obj != updating_obj ))
					continue;
				if( ++count == number )
					return obj;
			}
		}
	}
	else
	{
		number = atoi ( argument );
		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if ( !obj ) continue;
			// if ( GLOBAL_OBJ_UPDATE && obj->update_checked ) continue; //Mark by Razgriz 20050702
			if( obj->wear_loc != WEAR_NONE && obj->pIndexData->vnum == number)
			{
				//Added by Razgriz  20050702
				if ( GLOBAL_OBJ_UPDATE && ( obj->pIndexData->vnum == updating_obj->pIndexData->vnum ) && ( obj != updating_obj ) )
					continue;
				return obj;
			}
		}
	}
	return NULL;
}

/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;

	if ( !ch || !ch->in_room )
		return NULL;		//Added by Razgriz	20050713

	// 判定移到 !ch 之下 2023/01/08
	if ( ( obj = get_obj_carry( ch, argument ) ) )
		return obj;

	// 判定移到 !ch 之下 2023/01/08
	if ( ( obj = get_obj_wear( ch, argument ) ) )
		return obj;

	obj = get_obj_list( ch, argument, ch->in_room->contents );

	if ( obj )
		return obj;

	return NULL;
}

// 抓出房間中所有的 obj 2023/01/08
OBJ_DATA *get_room_obj( ROOM_INDEX_DATA *pRoomIndex, char *argument )
{
	OBJ_DATA *obj = NULL;
	OBJ_DATA *pobj = NULL;
	OBJ_DATA *pobj_next = NULL;
	CHAR_DATA *rch = NULL;

	if( !pRoomIndex )
		return NULL;

	// 這個優先用於沒有 char 的情況, 所以先抓房間中的東西
	if( pRoomIndex->contents )
	{
		for ( pobj = pRoomIndex->contents; pobj; pobj = pobj_next )
		{
			pobj_next = pobj->next_content;
			if( is_pfxname( argument, pobj->name ) ){
				obj = pobj;
				return obj;
			}
		}
	}

	// 房間中沒東西再抓房間中 char 的東西
	for( rch = pRoomIndex->people; rch; rch = rch->next_in_room )
	{
		if ( rch->deleted )
			continue;
		if ( ( obj = get_char_obj( rch, argument ) ) )
			return obj;
	}

	if ( obj )
		return obj;

	return NULL;
}

int count_obj_index_vnum( int vnum ) 
{
	OBJ_DATA *obj;
	int i=0;
	for( obj = object_list ; obj ; obj = obj->next )
	{
		if( obj->pIndexData->vnum == vnum && !obj->deleted )
			++i;
	}
	return i;
}

OBJ_DATA *get_obj_index_vnum( int vnum ) 
{
	OBJ_DATA *obj;
	for( obj = object_list ; obj ; obj = obj->next )
	{
		if( obj->pIndexData->vnum == vnum && !obj->deleted )
			return obj;
	}
	return NULL;
}

/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	if ( ( obj = get_obj_here( ch, argument ) ) )
		return obj;

	number = number_argument( argument, arg );
	count  = 0;
	for ( obj = object_list; obj; obj = obj->next )
	{
		if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
		{
			if ( ++count == number )
				return obj;
		}
	}
	for ( obj = object_list; obj; obj = obj->next )
	{
		if ( can_see_obj( ch, obj ) && is_pfxname( arg, obj->name ) )
		{
			if ( ++count == number )
				return obj;
		}
	}

	return NULL;
}



/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int amount )
{
	OBJ_DATA *obj;

	if ( amount <= 0 )
	{
		bug( "Create_money: zero or negative money %d.", amount );
		amount = 1;
	}

	if ( amount == 1 )
	{
		obj = create_object( get_obj_index( OBJ_VNUM_MONEY_ONE  ), 0 );
	}
	else
	{
		obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ), 0 );
	}

	obj->value[0] = amount;
	return obj;
}



/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number( OBJ_DATA *obj )
{
	int number;
	OBJ_DATA *pobj;

	number = 0;
	// if ( obj->item_type == ITEM_CONTAINER ){
	if( obj->contains )
	{
		for ( pobj = obj->contains; pobj; pobj = pobj->next_content )
		{
			if ( pobj->deleted )
				continue;
			number += get_obj_number( pobj );
		}
		if( obj->item_type == ITEM_CONTAINER && obj->value[0] == -1 )
			number = 0;
		else if( obj->item_type == ITEM_CONTAINER && obj->value[3] < 0 )
			number = UMAX( 0, number + obj->value[3]);
	}
	else
		number = 1;

	return number;
}


// 抓出物理攻擊命中敵人時的裝備 2022/12/25
OBJ_DATA *check_using_wp( CHAR_DATA *ch, int dt , int wield )
{
	OBJ_DATA *obj;

	obj = NULL;

	if( dt == 108 ) // kick
	{
		if( get_eq_char( ch, WEAR_FEET ) )
			obj = get_eq_char( ch, WEAR_FEET );

		if( !obj ) return NULL;

		return obj;
	}else{
		if(    get_eq_char( ch, WEAR_WIELD )  // 有手持武器
			|| get_eq_char( ch, WEAR_TWO_HAND )
			|| get_eq_char( ch, WEAR_WIELD_2 )
			&& dt != gsn_throwing )
		{
			obj = get_eq_char( ch, wield );
			if( !obj )
				return NULL;
			return obj;
		}else{  // 無手持武器
			if( get_eq_char( ch, WEAR_HANDS ) )
				obj = get_eq_char( ch, WEAR_HANDS );
			if( !obj )
				return NULL;
			return obj;
		}
	}

	if( !obj ) return NULL;
}

// 抓出物理攻擊命中敵人時的次裝備, 如 箭矢,彈藥,次手套,次腳部 ( 但不包含次要武器 ) 2022/12/25
OBJ_DATA *check_using_wp2( CHAR_DATA *ch, int dt )
{
	OBJ_DATA *obj;

	obj = NULL;

	if( dt == 108 ) // kick 獨立判定
	{
		if( get_eq_char( ch, WEAR_FEET_2 ) )
			obj = get_eq_char( ch, WEAR_FEET_2 );
		if( !obj )
			return NULL;
		return obj;
	}else{
		if(    get_eq_char( ch, WEAR_WIELD )  // 有手持武器
			|| get_eq_char( ch, WEAR_TWO_HAND )
			|| get_eq_char( ch, WEAR_WIELD_2 )
			&& dt != gsn_throwing )
		{
			if( dt == 1014 ){ // shot , 跑到 dt 1014 就該有 弓 裝備
				if( get_eq_char( ch, WEAR_ARROW ) )
					obj = get_eq_char( ch, WEAR_ARROW );
				if( !obj )
					return NULL;
				return obj;
			}else if( dt == 1016 ){ // ammo , 跑到 dt 1016 就該有 槍 裝備
				if( get_eq_char( ch, WEAR_AMMO ) )
					obj = get_eq_char( ch, WEAR_AMMO );
				if( !obj )
					return NULL;
				return obj;
			}
		}else{  // 無手持武器
			if( get_eq_char( ch, WEAR_HANDS_2 ) )
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			if( !obj )
				return NULL;
			return obj;
		}
	}

	if( !obj ) return NULL;
}

/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
	int weight;
	OBJ_DATA *pObj;

	if( !obj ) return 0;
	weight = obj->weight;
	for ( pObj = obj->contains; pObj; pObj = pObj->next_content )
	{
		if ( pObj->deleted )
			continue;
		weight += get_obj_weight( pObj );
	}

	if( obj->item_type == ITEM_CONTAINER && obj->value[0] == -1 ) 
	{
		weight = UMAX( obj->weight, 0 );
	}
	else if( obj->item_type == ITEM_CONTAINER && obj->value[3] > 0 ) 
	{
		weight = UMAX( obj->weight,( ( weight - obj->weight ) * obj->value[3] )/ 100);
	}

	if(weight < 0)
		weight = 0;

	return weight;
}



/*
 * True if room is dark.
 */
bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex )
{
	OBJ_DATA *obj;

	if ( !pRoomIndex )
		return TRUE;

	if ( pRoomIndex->light > 0 )
		return FALSE;

	for ( obj = pRoomIndex->contents; obj; obj = obj->next_content )
	{
		if ( obj->deleted )
			continue;
		if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
			return FALSE;
	}

	if ( IS_SET( pRoomIndex->room_flags, ROOM_DARK ) )
		return TRUE;

	if ( pRoomIndex->sector_type == SECT_INSIDE || pRoomIndex->sector_type == SECT_CITY )
		return FALSE;

	if ( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
		return TRUE;

	return FALSE;
}



/*
 * True if room is private.
 */
// 修改 room_is_private 代入值 2022/12/20
//bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
bool room_is_private( CHAR_DATA *ch , ROOM_INDEX_DATA *pRoomIndex )
{
	CHAR_DATA *rch;
	int count;
	int total_size; // 2022/12/20

	// room_size > 0 時, 限制房間可進入 char 體型 2022/12/20
	//if( pRoomIndex->room_size > 0 && ( race_table[ch->race].size > pRoomIndex->room_size) )
	if( pRoomIndex->room_size > 0 && ( get_curr_size( ch ) > pRoomIndex->room_size) ) // 替換體型 size 公式 2022/12/23
		return TRUE;

	count = 0;
	total_size = 0;

	for( rch = pRoomIndex->people; rch; rch = rch->next_in_room )
	{
		if ( rch->deleted )
			continue;

		if ( IS_IMMORTAL( rch ) )
			continue;

		count++;
		// 房間內 char 總 size 2022/12/20
		//total_size += race_table[rch->race].size;
		total_size += get_curr_size( rch );
	}

	//total_size = total_size + race_table[ch->race].size;

	total_size = total_size + get_curr_size( ch );

	// room_space > 0 時, 限制房間內 char 體型總合 2022/12/20
	if( pRoomIndex->room_space > 0 && ( total_size > pRoomIndex->room_space ) )
		return TRUE;


		/*  移除 room_solitary 的效果, 並把 room_private 修改為是否有人數限制的判定 2022/12/17
		if ( IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE ) && count >= 2 )
			return TRUE;
		if ( IS_SET( pRoomIndex->room_flags, ROOM_SOLITARY ) && count >= 1 )
			return TRUE; */

	// 需要有 private 的 flag 才會限制人數 2022/12/17
	if( IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE ) )
	{
		// max_char 小於 0 時為不限制人數 (max_char 以 room command modvrt l 來設定)
		if( pRoomIndex->max_char <= 0 ){
			// room_private 的原設定為最多 2 人
			if( count >= 2 )
				return TRUE;
		}else{
			if( count >= pRoomIndex->max_char )
				return TRUE;
		}
	}

	return FALSE;
}

int get_wizinvis_level( CHAR_DATA *ch)
{
	QMARK *qm;
	if( IS_NPC(ch) ) return -1;
	if( !( qm = is_qmark(ch, "wizinvis") ))
	{
		if(IS_SET(ch->act, PLR_WIZINVIS) )
			return get_trust(ch);
		else
			return -1;
	}
	return qm->v0;
}


/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if ( !ch || !victim || victim->deleted )
		return FALSE;

	if ( ch == victim )
		return TRUE;

	// sor 召喚的 mob 永遠能看得見召喚者 2022/12/18
	if( IS_NPC(ch) && IsSorGateMob(ch->pIndexData->vnum) && ch->rembch == victim )
		return TRUE;

	/* All mobiles cannot see wizinvised immorts */
	if ( IS_NPC( ch )
	    && !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) )
		return FALSE;

	if ( !IS_NPC( victim )
	    && IS_SET( victim->act, PLR_WIZINVIS )
	    && get_trust( ch ) < get_wizinvis_level( victim ) )
		return FALSE;

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_HOLYLIGHT ) && IS_IMMORTAL( ch ) )
		return TRUE;

	// 修正 nightstalk sklv 影響作用時間  2020/09/16
	// 新增能看見 nightstalk 角色的 qmark 2022/04/02
	//if ( IS_AFFECTED( victim, AFF_NIGHTSTALK ) && (time_info.hour < 5 || time_info.hour > 19 )){
	if ( IS_AFFECTED( victim, AFF_NIGHTSTALK ) && (!is_qmark( ch, "enable_detect_nightstalk" )) ){
		int NtBonus = get_skill_level( victim , gsn_nightstalk) - 4;
		if (NtBonus < 0) NtBonus = 0;
		if ((time_info.hour < 5 + NtBonus) || (time_info.hour > 19 - NtBonus)){
			if(!IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER )){
				return FALSE;
			}else{
				int nInbSklv = get_skill_level(ch, gsn_inborn_ability);
				if(nInbSklv < 6)
					return FALSE;
			}
		}
	}

	if ( IS_AFFECTED( ch, AFF_BLIND ) )
		return FALSE;

	//    if ( room_is_dark( ch->in_room )
	//  && !IS_SET( race_table[ ch->race ].race_abilities, RACE_INFRAVISION )
	//  && !IS_AFFECTED( ch, AFF_INFRARED ) )
	//  return FALSE;

	if ( victim->position == POS_DEAD )
		return TRUE;

	if ( ( IS_AFFECTED( victim, AFF_INVISIBLE ) || IS_SET( race_table[ch->race].race_abilities, RACE_INVIS))
	     && !IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_INVIS )
	     && !IS_AFFECTED( ch, AFF_DETECT_INVIS ) )
		return FALSE;

	if ( IS_AFFECTED( victim, AFF_HIDE )
	    && !IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_HIDDEN )
	    && !IS_AFFECTED( ch, AFF_DETECT_HIDDEN )
	    && !victim->fighting )
	return FALSE;

	return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
	if ( obj->deleted )
		return FALSE;

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_HOLYLIGHT ) )
		return TRUE;

	if ( IS_AFFECTED( ch, AFF_BLIND ) && ( obj->item_type != ITEM_POTION && obj->item_type != ITEM_PILL))
		return FALSE;

	if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
		return TRUE;

	if (   room_is_dark( ch->in_room )
		&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_INFRAVISION )
		&& !IS_AFFECTED( ch, AFF_INFRARED ) )
	return FALSE;

	if (   IS_SET( obj->extra_flags, ITEM_INVIS )
		&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_INVIS )
		&& !IS_AFFECTED( ch, AFF_DETECT_INVIS ) )
	return FALSE;

	return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{

	if( IS_SET(obj->extra_flags, ITEM_PERSONAL) && !IS_IMMORTAL( ch )) 
		return FALSE;

	if ( !IS_SET( obj->extra_flags, ITEM_NODROP ) )
		return TRUE;
	else if ( IS_SET( obj->extra_flags, ITEM_REMOVE_NODROP ) )
	{
		REMOVE_BIT( obj->extra_flags, ITEM_REMOVE_NODROP );
		return TRUE;
	}

	if ( IS_IMMORTAL( ch ) )
		return TRUE;

	if ( IS_NPC( ch ) )
		return TRUE;

	return FALSE;
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA *obj )
{
	OBJ_DATA *in_obj;
	char buf [ MAX_STRING_LENGTH ];

	switch ( obj->item_type )
	{
		case ITEM_LIGHT:      return "light";
		case ITEM_SCROLL:     return "scroll";
		case ITEM_WAND:       return "wand";
		case ITEM_STAFF:      return "staff";
		case ITEM_WEAPON:     return "weapon";
		case ITEM_TREASURE:   return "treasure";
		case ITEM_ARMOR:      return "armor";
		case ITEM_POTION:     return "potion";
		case ITEM_FURNITURE:  return "furniture";
		case ITEM_TRASH:      return "trash";
		case ITEM_CONTAINER:  return "container";
		case ITEM_DRINK_CON:  return "drink container";
		case ITEM_KEY:        return "key";
		case ITEM_FOOD:       return "food";
		case ITEM_MONEY:      return "money";
		case ITEM_BOAT:       return "boat";
		case ITEM_CORPSE_NPC: return "npc corpse";
		case ITEM_CORPSE_PC:  return "pc corpse";
		case ITEM_FOUNTAIN:   return "fountain";
		case ITEM_PILL:       return "pill";
		case ITEM_ARROW:      return "arrow";
		case ITEM_DART:       return "dart";
		case ITEM_HORSE:      return "horse";
		case ITEM_AMMO:       return "ammo"; // 2022/04/29
	}

	for ( in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj )
		;

	if ( in_obj->carried_by )
		sprintf( buf, "Item_type_name: unknown type %d from %s owned by %s.", obj->item_type, obj->name, obj->carried_by->name );
	else
		sprintf( buf, "Item_type_name: unknown type %d from %s owned by (unknown).", obj->item_type, obj->name );

	bug( buf, 0 );
	return "(unknown)";
}



/* Modified by Razgriz 20051001
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
	switch ( location )
	{
		case APPLY_NONE:             return "none";
		case APPLY_STR:              return "strength";
		case APPLY_DEX:              return "dexterity";
		case APPLY_INT:              return "intelligence";
		case APPLY_WIS:              return "wisdom";
		case APPLY_CON:              return "constitution";
		case APPLY_SEX:              return "sex";
		case APPLY_CLASS:            return "class";
		case APPLY_LEVEL:            return "level";
		case APPLY_AGE:              return "age";
		case APPLY_HEIGHT:           return "height";
		case APPLY_WEIGHT:           return "weight";
		case APPLY_MANA:             return "mana";
		case APPLY_HIT:              return "hp";
		case APPLY_MOVE:             return "moves";
		case APPLY_GOLD:             return "gold";
		case APPLY_EXP:              return "experience";
		case APPLY_AC:               return "armor class";
		case APPLY_HITROLL:          return "hit roll";
		case APPLY_DAMROLL:          return "damage roll";
		case APPLY_SAVING_PARA:      return "save vs paralysis";
		case APPLY_SAVING_LIFE:      return "save vs life";
		case APPLY_SAVING_PETRI:     return "save vs petrification";
		case APPLY_SAVING_BREATH:    return "save vs breath";
		case APPLY_SAVING_SPELL:     return "save vs spell";
		case APPLY_SAVING_ANCIENT:   return "save vs ancient";
		case APPLY_SAVING_SPIRIT:    return "save vs spirit";
		case APPLY_SAVING_FIRE:      return "save vs fire";
		case APPLY_SAVING_COLD:      return "save vs cold";
		case APPLY_SAVING_LIGHTNING: return "save vs lightning";
		case APPLY_SAVING_EARTH:     return "save vs earth";
		case APPLY_SAVING_WIND:      return "save vs wind";
		case APPLY_SAVING_SAINT:     return "save vs saint";
		case APPLY_SAVING_DARK:      return "save vs dark";
		case APPLY_SAVING_POISON:    return "save vs poison";
		case APPLY_SAVING_ELEMENT:   return "save vs element";
		case APPLY_SAVING_GOOD:      return "save vs good";
		case APPLY_SAVING_EVIL:      return "save vs evil";
		case APPLY_RACE:             return "race";
		case APPLY_DODGE:            return "dodge rate";
		case APPLY_POWER_POTENTIAL:  return "power potential";
		case APPLY_RDAMAGE:          return "reduce physical damage";
		case APPLY_RMAGIC:           return "reduce magic damage";
		case APPLY_ADAMAGE:          return "enhance physical damage";
		case APPLY_AMAGIC:           return "enhance magic damage";
		case APPLY_GPHYSICAL:        return "physical damage gate";
		case APPLY_GMAGIC:           return "magic damage gate";
		case APPLY_SAVING_ILLUSION:  return "save vs illusion";
		case APPLY_ACTION_ABILITY:   return "action ability";
		case APPLY_DOB_SAVING_FLASH: return "save vs sun flash";
		case APPLY_WEAPON_SPELL:     return "spell weapon";  // add at 2021/09/19
		case APPLY_WEAPON_WIND:      return "wind weapon";
		case APPLY_WEAPON_FIRE:      return "fire weapon";
		case APPLY_WEAPON_COLD:      return "cold weapon";
		case APPLY_WEAPON_EARTH:     return "earth weapon";
		case APPLY_WEAPON_LIGHTNING: return "lightning weapon";
		case APPLY_WEAPON_SAINT:     return "saint weapon";
		case APPLY_WEAPON_DARK:      return "dark weapon";
		case APPLY_MAGIC_IMMUNE:     return "magic immune";      // add at 2021/11/03 for mystic armor
		case APPLY_TOXICITY_POISON:  return "toxicity poison";   // add at 2021/11/10 for poison item
		case APPLY_DAMAGE_COUNT:     return "damage count";      // add at 2022/02/21 for blood thirsty
		case APPLY_SHOOT_HITROLL:    return "shoot hitroll";      // add at 22022/05/06
		case APPLY_SHOOT_DAMROLL:    return "shoot damroll";      // add at 2022/05/06
		case APPLY_SHOOT_ADAMAGE:    return "shoot physical damage"; // add at 2022/05/06
    }

	bug( "Affect_location_name: unknown location %d.", location );
	return "(unknown)";
}

// modified by jye 6.15.2005
char *affect_bit_name( long long int vector )
{
    static char buf [ 1024 ];

    buf[0] = '\0';
    if ( vector & AFF_BLIND ) strcat( buf, " blindness" );
    if ( vector & AFF_INVISIBLE ) strcat( buf, " invis" );
    if ( vector & AFF_DETECT_EVIL ) strcat( buf, " detect evil" );
    if ( vector & AFF_DETECT_INVIS ) strcat( buf, " detect invis" );
    if ( vector & AFF_DETECT_MAGIC ) strcat( buf, " detect magic" );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect hidden" );
    if ( vector & AFF_HOLD ) strcat( buf, " hold" );
    if ( vector & AFF_SANCTUARY ) strcat( buf, " sanctuary" );
    if ( vector & AFF_FAERIE_FIRE ) strcat( buf, " faerie fire" );
    if ( vector & AFF_INFRARED ) strcat( buf, " infrared" );
    if ( vector & AFF_WATERWALK ) strcat( buf, " waterwalk" );
    if ( vector & AFF_CURSE ) strcat( buf, " curse" );
    if ( vector & AFF_CHANGE_SEX ) strcat( buf, " change sex" );
    if ( vector & AFF_POISON ) strcat( buf, " poison" );
    if ( vector & AFF_PROTECT ) strcat( buf, " protect" );
    if ( vector & AFF_POLYMORPH ) strcat( buf, " polymorph" );
    if ( vector & AFF_SLEEP ) strcat( buf, " sleep" );
    if ( vector & AFF_SNEAK ) strcat( buf, " sneak" );
    if ( vector & AFF_HIDE ) strcat( buf, " hide" );
    if ( vector & AFF_CHARM ) strcat( buf, " charm" );
    if ( vector & AFF_FLYING ) strcat( buf, " fly" );
    if ( vector & AFF_PASS_DOOR ) strcat( buf, " pass door" );
    if ( vector & AFF_MUTE ) strcat( buf, " mute" );
    if ( vector & AFF_SEAL_MAGIC ) strcat( buf, " seal magic" );
    if ( vector & AFF_GILLS ) strcat( buf, " gills" );
    if ( vector & AFF_VAMP_BITE ) strcat( buf, " vampiric bite" );
    if ( vector & AFF_GHOUL ) strcat( buf, " undead curse"  ); // 把ghoul curse 換成 undead curse 2022/01/16
    if ( vector & AFF_FLAMING ) strcat( buf, " flaming shield" );
    if ( vector & AFF_PARALYZED ) strcat( buf, " paralyzed" );
    if ( vector & AFF_CRAZY ) strcat( buf, " crazy" );
    if ( vector & AFF_WEAKEN ) strcat( buf, " weaken" );
    if ( vector & AFF_NIGHTSTALK ) strcat( buf, " nightstalk" );
    if ( vector & AFF_DARK_RAID ) strcat( buf, " dark raid" );
    if ( vector & AFF_MANA_SHIELD ) strcat( buf, " mana shield" );
    if ( vector & AFF_TARDY ) strcat( buf, " tardy" );
	if ( vector & AFF_CONTINUE_CURE ) strcat( buf, " continue cure" ); //add at 2022.11.02
	if ( vector & AFF_DISEASE ) strcat( buf, " disease" ); //add at 2022.11.02
	if ( vector & AFF_FEAR ) strcat( buf, " fear" ); //add at 2022.11.02
	if ( vector & AFF_NO_HEAL ) strcat( buf, " no heal" ); //add at 2018.11.20
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

/*
 * Return ascii name of an affect bit vector.
 */
char *r_affect_bit_name( long long int vector )
{
	static char buf [ 512 ];

	buf[0] = '\0';
	if ( vector & R_AFF_FIREWALL )
		strcat( buf, " firewall" );
	return ( buf[0] != '\0' ) ? buf+1 : "none";
}



/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
	static char buf [ 1024 ];

	buf[0] = '\0';
	if ( extra_flags & ITEM_GLOW   ) strcat( buf, " glow"   );
	if ( extra_flags & ITEM_HUM    ) strcat( buf, " hum"    );
	if ( extra_flags & ITEM_DARK   ) strcat( buf, " dark"   );
	if ( extra_flags & ITEM_LOCK   ) strcat( buf, " lock"   );
	if ( extra_flags & ITEM_EVIL   ) strcat( buf, " evil"   );
	if ( extra_flags & ITEM_INVIS  ) strcat( buf, " invis"  );
	if ( extra_flags & ITEM_MAGIC  ) strcat( buf, " magic"  );
	if ( extra_flags & ITEM_NODROP   ) strcat( buf, " nodrop" );
	if ( extra_flags & ITEM_BLESS  ) strcat( buf, " bless"  );
	if ( extra_flags & ITEM_ANTI_GOOD  ) strcat( buf, " anti-good"  );
	if ( extra_flags & ITEM_ANTI_EVIL  ) strcat( buf, " anti-evil"  );
	if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );
	if ( extra_flags & ITEM_NOREMOVE   ) strcat( buf, " noremove" );
	if ( extra_flags & ITEM_INVENTORY  ) strcat( buf, " inventory"  );
	if ( extra_flags & ITEM_POISONED   ) strcat( buf, " poisoned" );
	if ( extra_flags & ITEM_BROKEN   ) strcat( buf, " broken" );
	if ( extra_flags & ITEM_VAMPIRE_BANE ) strcat( buf, " vampire bane" );
	if ( extra_flags & ITEM_HOLY   ) strcat( buf, " holy"   );
	if ( extra_flags & ITEM_NOSAVE) strcat( buf, " nosave"    );
	if ( extra_flags & ITEM_UNIQUE) strcat( buf, " unique"    );
	if ( extra_flags & ITEM_CLAN) strcat( buf, " clan"    );
	if ( extra_flags & ITEM_PERSONAL) strcat( buf, " personal"    );
	if ( extra_flags & ITEM_SPELL) strcat( buf, " spell"    ); // ITEM_RACIAL change to ITEM_SPELL 2021/09/19
	if ( extra_flags & ITEM_LORE) strcat( buf, " lore"    );
	if ( extra_flags & ITEM_NOSAC) strcat( buf, " nosac"    );
	if ( extra_flags & ITEM_SPECIAL) strcat( buf, "special"   );
    if ( extra_flags & ITEM_NO_MALE) strcat( buf, " nomale"   );
    if ( extra_flags & ITEM_NO_FEMALE) strcat( buf, " nofemale"   );
    if ( extra_flags & ITEM_NO_NEUTRAL) strcat( buf, " noneutral"   );
    if ( extra_flags & ITEM_RECOVER) strcat( buf, " recovered"    );
    if ( extra_flags & ITEM_REMOVE_NODROP  ) strcat( buf, " remove_nodrop");
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

CHAR_DATA *get_char( CHAR_DATA *ch )
{
	if ( IS_NPC( ch ) )
	{
		bug( "get_char", 0 );
		bug( "mob vnum = %d", ch->pIndexData->vnum );
		return NULL;
	}
	if ( !ch->pcdata )
		return ch->desc->original;
	else
		return ch;
}

bool longstring( CHAR_DATA *ch, char *argument )
{
	if ( strlen( argument) > 60 )
	{
		send_to_char( "不能超過 60 個字元.\n\r", ch );
		return TRUE;
	}
	else
		return FALSE;
}

bool authorized( CHAR_DATA *ch, char *skllnm )
{

	char buf [ MAX_STRING_LENGTH ];

	if( !IS_IMMORTAL( ch ) ) return FALSE;

	if ( ( !IS_NPC( ch ) && str_infix( skllnm, ch->pcdata->immskll ) ) || IS_NPC( ch ) )
	{
		sprintf( buf, "Sorry, you are not authorized to use %s.\n\r", skllnm );
		send_to_char( buf, ch );
		return FALSE;
	}

	return TRUE;

}

void end_of_game( void )
{
	DESCRIPTOR_DATA *d;
	DESCRIPTOR_DATA *d_next;

	for ( d = descriptor_list; d; d = d_next )
	{
		d_next = d->next;
		if ( d->connected == CON_PLAYING )
		{
			if ( d->character->position == POS_FIGHTING )
				interpret( d->character, "save" );
			else
				interpret( d->character, "quit" );
		}
		else
			close_socket( d );
	}

	return;
}
char *wear_bit_name( int vector )
{
	static char buf [ 2048 ];
	buf[0] = '\0';
	if ( vector & ITEM_TAKE     ) strcat( buf, " take"      );
	if ( vector & ITEM_WEAR_FINGER  ) strcat( buf, " finger"      );
	if ( vector & ITEM_WEAR_NECK    ) strcat( buf, " neck"      );
	if ( vector & ITEM_WEAR_BODY    ) strcat( buf, " body"      );
	if ( vector & ITEM_WEAR_HEAD    ) strcat( buf, " head"      );
	if ( vector & ITEM_WEAR_LEGS    ) strcat( buf, " legs"      );
	if ( vector & ITEM_WEAR_FEET    ) strcat( buf, " feet"      );
	if ( vector & ITEM_WEAR_HANDS   ) strcat( buf, " hands"     );
	if ( vector & ITEM_WEAR_ARMS    ) strcat( buf, " arms"      );
	if ( vector & ITEM_WEAR_SHIELD  ) strcat( buf, " shield"      );
	if ( vector & ITEM_WEAR_ABOUT   ) strcat( buf, " about"     );
	if ( vector & ITEM_WEAR_WAIST   ) strcat( buf, " waist"     );
	if ( vector & ITEM_WEAR_WRIST   ) strcat( buf, " wrist"     );
	if ( vector & ITEM_WIELD   ) strcat( buf, " wield"      );
	if ( vector & ITEM_HOLD    ) strcat( buf, " hold"     );
	if ( vector & ITEM_TWO_HAND     ) strcat( buf, " two-handed"    );
	if ( vector & ITEM_WEAR_ARROW   ) strcat( buf, " arrow"    );
	if ( vector & ITEM_WEAR_RIDE    ) strcat( buf, " ride"     );
	if ( vector & ITEM_WEAR_AMMO    ) strcat( buf, " ammo"     ); // add at 2022/04/30

	return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *special_bit_name( int vector )
{
	static char buf [ 2048 ];
	buf[0] = '\0';
	if ( vector & NO_DM) strcat( buf, " no_dm"      );
	if ( vector & NO_LORE) strcat( buf, " no_lore"      );
	if ( vector & NO_BLIND) strcat( buf, " no_blind"      );
	if ( vector & NO_CHARM) strcat( buf, " no_charm"    );
	if ( vector & NO_CURSE) strcat( buf, " no_curse"    );
	if ( vector & NO_FAEFIRE) strcat( buf, " no_faefire"    );
	if ( vector & NO_POISON) strcat( buf, " no_poison"    );
	if ( vector & NO_SLEEP) strcat( buf, " no_sleep"    );
	if ( vector & DAM_HALF) strcat( buf, " half_magic"    );
	if ( vector & ANTI_MAGIC) strcat( buf, " anti_magic"    );
	if ( vector & REF_MAGIC) strcat( buf, " ref_magic"    );
	if ( vector & ALY_SAN) strcat( buf, " aly_san"    );
	if ( vector & ALY_INVIS) strcat( buf, " alc_inv"    );
	if ( vector & ALY_HIDE) strcat( buf, " aly_hide"    );
	if ( vector & ALY_SNEAK) strcat( buf, " aly_sneak"    );
	if ( vector & ALY_DI) strcat( buf, " aly_di"    );
	if ( vector & ALY_DH) strcat( buf, " aly_dh"    );
	if ( vector & ALY_INF) strcat( buf, " aly_inf"    );
	if ( vector & ALY_BAR) strcat( buf, " aly_bar"    );
	if ( vector & NO_TARDY) strcat( buf, " no_tardy"    );
	if ( vector & NO_MUTE) strcat( buf, " no_mute"    );
	if ( vector & NO_SEALMAGIC) strcat( buf, " no_sealmagic"    );
	if ( vector & EXPERT) strcat( buf, " expert"    );
	if ( vector & NO_CRAZY) strcat( buf, " no_crazy"    );
	if ( vector & MOBAI_LV2) strcat( buf, " mobai_2"    );
	if ( vector & MOBAI_LV3) strcat( buf, " mobai_3"    );
	if ( vector & NO_PARA)   strcat( buf, " no_para"  );
	if ( vector & SPELL_FURY)  strcat( buf, " spell_fury" );
	if ( vector & NO_FEAR)   strcat( buf, " no_fear"  );
	if ( vector & NO_DISARM) strcat( buf, " no_disarm"  );
	if ( vector & NO_TRIP)   strcat( buf, " no_trip"  );
	return ( buf[0] != '\0' ) ? buf+1 : "none";
}



char *ppl_act_bit_name( long long int vector )
{
	static char buf [ 3072 ];
	buf[0] = '\0';
	if ( vector & PLR_IS_NPC ) strcat( buf, " npc"    );
	if ( vector & PLR_STATUS ) strcat( buf, " status"    );
	if ( vector & PLR_REGISTER ) strcat( buf, " pk"    );
	if ( vector & PLR_AUTOEXIT ) strcat( buf, " autoexit"    );
	if ( vector & PLR_AUTOLOOT ) strcat( buf, " autoloot"    );
	if ( vector & PLR_AUTOSAC ) strcat( buf, " autosac"    );
	if ( vector & PLR_BLANK ) strcat( buf, " blank"    );
	if ( vector & PLR_BRIEF ) strcat( buf, " brief"    );
	if ( vector & PLR_COMBINE ) strcat( buf, " combine"    );
	if ( vector & PLR_PROMPT ) strcat( buf, " prompt"    );
	if ( vector & PLR_AUTOSPLIT ) strcat( buf, " autosplit"    );
	if ( vector & PLR_HOLYLIGHT ) strcat( buf, " holylight"    );
	if ( vector & PLR_WIZINVIS ) strcat( buf, " wizinvis"    );
	if ( vector & PLR_WIZBIT ) strcat( buf, " wizify"    );
	if ( vector & PLR_SILENCE ) strcat( buf, " silence"    );
	if ( vector & PLR_NO_EMOTE ) strcat( buf, " no_emote"    );
	if ( vector & PLR_MOVED ) strcat( buf, " moved"    );
	if ( vector & PLR_NO_TELL ) strcat( buf, " no_tell"    );
	if ( vector & PLR_LOG ) strcat( buf, " log"    );
	if ( vector & PLR_DENY ) strcat( buf, " denied"    );
	if ( vector & PLR_FREEZE ) strcat( buf, " freezed"    );
	if ( vector & PLR_THIEF ) strcat( buf, " thief"    );
	if ( vector & PLR_KILLER ) strcat( buf, " killer"    );
	if ( vector & PLR_AUTOGOLD ) strcat( buf, " autogold"    );
	if ( vector & PLR_AFK ) strcat( buf, " afk"    );
	if ( vector & PLR_ARTIFACT ) strcat( buf, " artifact"    );
	if ( vector & PLR_FIST_FIGHT ) strcat( buf, " fist_fight"    );
	if ( vector & PLR_COMBAT ) strcat( buf, " combat"    );
	if ( vector & PLR_CHINESE ) strcat( buf, " chinese"    );
	if ( vector & PLR_HORSE ) strcat( buf, " horse"    );
	if ( vector & PLR_NOTIFY ) strcat( buf, " notify"    );
	if ( vector & PLR_VERBOSEALL ) strcat( buf, " verall"    );
	if ( vector & PLR_TITLE ) strcat( buf, " title"    );
	if ( vector & PLR_ASSIST ) strcat( buf, " assist"    );
	if ( vector & PLR_RIDE ) strcat( buf, " ride"    );
	if ( vector & PLR_DEAD) strcat( buf, " dead"    );
	if ( vector & PLR_GRAPHIC) strcat( buf, " graphic"    );
	if ( vector & PLR_IDLE) strcat( buf, " idle"    );
	return ( buf[0] != '\0' ) ? buf+1 : "none";
}
char *act_bit_name( long long int vector )
{
	static char buf [ 3072 ];
	buf[0] = '\0';
	if ( vector & ACT_IS_NPC )
		strcat( buf, " npc" );
	else {
		return ppl_act_bit_name(vector);
	}
	if ( vector & ACT_SENTINEL      ) strcat( buf, " sentinel"      );
	if ( vector & ACT_CLAN ) strcat( buf, " clan-mob"     );
	if ( vector & ACT_SCAVENGER     ) strcat( buf, " scavenger"     );
	if ( vector & ACT_AGGRESSIVE    ) strcat( buf, " aggressive"    );
	if ( vector & ACT_STAY_AREA     ) strcat( buf, " stay-area"    );
	if ( vector & ACT_WIMPY     ) strcat( buf, " wimpy"    );
	if ( vector & ACT_PET     ) strcat( buf, " pet"    );
	if ( vector & ACT_TRAIN     ) strcat( buf, " train"    );
	if ( vector & ACT_PRACTICE      ) strcat( buf, " practice"    );
	if ( vector & ACT_CRITICAL      ) strcat( buf, " critical"    );
	if ( vector & ACT_HARD_DIS      ) strcat( buf, " swordmaster"    );
	if ( vector & ACT_HUNT      ) strcat( buf, " hunt"    );
	if ( vector & ACT_DUAL      ) strcat( buf, " dual"    );
	if ( vector & ACT_NO_FLEE     ) strcat( buf, " mob_no_flee"    ); // 將 no_flee改為 mob_no_flee 避免 null bstr 2022/01/03
	if ( vector & ACT_MOVED     ) strcat( buf, " moved"    );
	if ( vector & ACT_TACTICS     ) strcat( buf, " tactics"    );
	if ( vector & ACT_TRACK     ) strcat( buf, " track"    );
	if ( vector & ACT_REGENERACY    ) strcat( buf, " regeneracy"    );
	if ( vector & ACT_GAMBLE      ) strcat( buf, " gamble"    );
	if ( vector & ACT_BANKER      ) strcat( buf, " banker"    );
	if ( vector & ACT_MOB_AI      ) strcat( buf, " mob-ai"    );
	if ( vector & ACT_REFLEXION     ) strcat( buf, " reflexion"    );
	if ( vector & ACT_MPROG_DEATH) strcat( buf, " deathprog"    );
	if ( vector & ACT_NOPAR) strcat( buf, " nopar"    );
	if ( vector & ACT_IGNORE_NOMOB) strcat( buf, " ignore_no_mob"    );
	if ( vector & ACT_SMITH) strcat( buf, " smith"    );
	if ( vector & ACT_HORSE) strcat( buf, " horse");
	return ( buf[0] != '\0' ) ? buf+1 : "none";
}


OBJ_INDEX_DATA * material_lookup( const char *race )
{
	OBJ_INDEX_DATA * mat;
	for( mat = material_list ; mat ; mat = mat -> next )
	{
		if( !str_cmp( race, mat->name ) )
			break; 
	}

	return mat;

}

char *qmark_lookup( const char * str)
{
	QMARK_STRING *q ;
	static const char n[] = "null_qmark_string";
	char b[MAX_INPUT_LENGTH];
	if( !str )
		return (char *)n;
	for( q = qmark_string_list; q; q  = q ->next )
	{
		if( !str_cmp(str, q->name)){
			return q->name;
		}
	}
	q = new_qmark_string( );
	sprintf(b, "%s", str);
	q->name = str_dup(b);
	q -> next = qmark_string_list;
	qmark_string_list = q;
	return q->name ;
}

int race_lookup( const char *race )
{
	int index;

	for ( index = 0; index < MAX_RACE; index++ )
	{
		if ( !str_prefix( race, race_table[index].name ) )
			return index;
	}

	return -1;

}

int affect_lookup( const char *affectname )
{
	int index;

	for ( index = 0; index < MAX_SKILL; index++ )
	{
		if ( !str_cmp( affectname, skill_table[index].name ) )
			return index;
	}

	return -1;

}

bool meet_req_second(CHAR_DATA * ch, int sn, int attr)
{
	const int attr_map[6] = { 
	SECOND_ALL, SECOND_STR, SECOND_DEX, SECOND_INT,
	SECOND_WIS, SECOND_CON};

	if( IS_NPC(ch))
		return TRUE;

	if( skill_table[sn].require_attr & attr_map[ch->pcdata->snd_attr] )
		return TRUE;

	return FALSE; 
}


bool meet_req_prime(CHAR_DATA * ch,int sn, int attr )
{
	const int attr_map[6] = { PRIME_ALL,
	PRIME_STR, PRIME_DEX, PRIME_INT, PRIME_WIS, PRIME_CON};

	if( IS_NPC(ch))
		return TRUE;

	if( skill_table[sn].require_attr & attr_map[ch->pcdata->prime_attr] )
		return TRUE;

	return FALSE; 
}


int room_clanlv( ROOM_INDEX_DATA *pRoom)
{
	if( IS_SET(pRoom->room_flags, ROOM_CLAN_LV3 ))      return 3;
	else if( IS_SET(pRoom->room_flags, ROOM_CLAN_LV2 )) return 2;
	else if( IS_SET(pRoom->room_flags, ROOM_CLAN_LV1 )) return 1;
	else return 0;
}

long long int symbol_lookup(char *name)
{
	SYMBOL_DATA *pSym;
	int letter, i = 0;
	long long int number = 0;

	if( !name || name[0] == '\0' ) return -1;
	if( !isalpha( ( letter = name[0] ))) return -1;
	letter = LOWER(letter) - 'a';

	for( pSym = symbol_hash[letter] ; pSym ; pSym = pSym->next )
	{
		if( !str_cmp( pSym->str, name ))
		{
			return pSym->num;
		}
	}

	do {
		letter = name[i];
		if( islower(letter) && letter < 'g')
		{
			number |= ( 1 << ( letter - 'a' + 26 ) );
			i++;
		}
		if( isupper(letter) )
		number |= ( 1 << (letter - 'A') );
		i++;
	}
	while( letter != '\0' );

	return (number > 0) ? number : -1;
}

bool has_player(char* arg2)
{
	char tmp[MAX_STRING_LENGTH];
	FILE *fp;
	sprintf(tmp, "%s%s/%s.gz", PLAYER_DIR,initial( arg2 ), capitalize(arg2 ));
	if( ( fp = fopen( tmp, "r"))){
		fflush(fp);
		fclose(fp);
		return TRUE;
	}
	sprintf(tmp, "%s%s/%s", PLAYER_DIR,initial( arg2 ), capitalize(arg2 ));
	if( ( fp = fopen( tmp, "r"))){
		fflush(NULL);
		fclose(fp);
		return TRUE;
	}

	return FALSE;
}






#ifdef HAS_ROOM_AFFECT
void r_affect_modify( ROOM_INDEX_DATA *ch, R_AFFECT_DATA *paf, bool fAdd )
{

	if ( fAdd )
	{
		SET_BIT( ch->affected_by, paf->bitvector );
	}
	else
	{
		REMOVE_BIT( ch->affected_by, paf->bitvector );
	}

	return;
}



/*
 * Give an affect to a char.
 */
void affect_to_room( ROOM_INDEX_DATA *ch, R_AFFECT_DATA *paf )
{
	R_AFFECT_DATA *paf_new;

	paf_new = new_r_affect();

	*paf_new = *paf;
	paf_new->deleted  = FALSE;
	paf_new->next = ch->affected;
	paf_new->next_in_list = r_affect_list;
	if( !r_affect_list )
		paf_new->prev_in_list = NULL;
	else
		r_affect_list->prev_in_list  = paf_new;
	paf_new->owner = ch;
	r_affect_list = paf_new;
	ch->affected  = paf_new;

	r_affect_modify( ch, paf_new, TRUE );
	return;
}
/*
 * Remove an affect from a char.
 */
void r_affect_remove( ROOM_INDEX_DATA *ch, R_AFFECT_DATA *paf )
{
	R_AFFECT_DATA * vaf;
	if ( !ch->affected )
	{
		bug( "Affect_remove: no affect.", 0 );
		return;
	}

	r_affect_modify( ch, paf, FALSE );

	//paf->deleted = TRUE;
	if( paf->prev_in_list )
		paf->prev_in_list->next_in_list = paf->next_in_list;

	if( paf->next_in_list )
		paf->next_in_list->prev_in_list = paf->prev_in_list;

	if( paf == paf->owner->affected ) {
		paf->owner->affected = paf->next;
	}else{
		for( vaf = paf->owner->affected ; vaf; vaf = vaf->next )
		{
			if( vaf->next == paf ) {
				vaf->next = paf->next;
				break;
			}
		}
	}
	free_r_affect(paf);

	return;
}



/*
 * Strip all affects of a given sn.
 */
void r_affect_strip( ROOM_INDEX_DATA *ch, int sn )
{
	R_AFFECT_DATA *paf;

	for ( paf = ch->affected; paf; paf = paf->next )
	{
		if ( paf->deleted )
			continue;
		if ( paf->type == sn )
			r_affect_remove( ch, paf );
	}

	return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool r_is_affected( ROOM_INDEX_DATA *ch, int sn )
{
	R_AFFECT_DATA *paf;

	for ( paf = ch->affected; paf; paf = paf->next )
	{
		if ( paf->deleted )
			continue;
		if ( paf->type == sn )
			return TRUE;
	}

	return FALSE;
}



/*
 * Add or enhance an affect.
 */
void r_affect_join( ROOM_INDEX_DATA*ch, R_AFFECT_DATA *paf )
{
	return;
}


#endif


QMARK * is_prefix_qmark( CHAR_DATA *ch, char* q)
{
	QMARK *qm = NULL;
	for( qm = ch->qmark ; qm ; qm = qm ->next ) {
		if( !str_cmp( qm->mark_name, q))
			break;
	}
	return qm;
}

QMARK * is_qmark( CHAR_DATA *ch, char* q)
{
	QMARK *qm = NULL;
	for( qm = ch->qmark ; qm ; qm = qm ->next ) {
		if( !str_cmp( qm->mark_name, q))
			break;
	}
	return qm;
}

// 修為與 merc.h 相同 2023/01/14
//bool is_flying( CHAR_DATA *victim)
bool is_flying(CHAR_DATA *ch)
{
	OBJ_DATA* horse;
    
	if( IS_AFFECTED(ch, AFF_FLYING) || IS_SET( race_table[ch->race].race_abilities, RACE_FLY ) )
		return TRUE;
    
	// 修正 horse obj 數值使用錯誤 2022/06/16
	//if( (horse = get_eq_char(victim,WEAR_RIDE)) && horse->value[2] & 8 )
	if( (horse = get_eq_char(ch,WEAR_RIDE)) && horse->value[1] & 512 )
		return TRUE;
    
	return FALSE;
}


void free_rcall_out( RCALL_OUT_DATA *rm)
{
	RCALL_OUT_DATA * cod, *cod_next;
	if( rm == rcall_out_list ){
		rcall_out_list = rm->next;
	}else{
		cod_next = rcall_out_list;
		for( cod = rcall_out_list ; cod_next ; cod = cod_next )
		{
			cod_next = cod->next;
			if( cod_next == rm )
			{
				cod->next = rm->next;
				break;
			}
		}
	}
	rm->next = rcall_out_free;
	rcall_out_free = rm;
	free_string(rm->cmd);
	rm->owner = NULL;
	rm->timer = 0;
	rm->cmd = NULL;
}

void free_call_out( CALL_OUT_DATA *rm)
{
	CALL_OUT_DATA * cod, *cod_next;
	if( rm == call_out_list ){
		call_out_list = rm->next;
	}else{
		cod_next = call_out_list;
		for( cod = call_out_list ; cod_next ; cod = cod_next )
		{
			cod_next = cod->next;
			if( cod_next == rm ) {
				cod->next = rm->next;
				break;
			}
		}
	}
	rm->next = call_out_free;
	call_out_free = rm;
	free_string(rm->cmd);
	rm->owner = NULL;
	rm->timer = 0;
	rm->cmd = NULL;
}

void add_call_out( CALL_OUT_DATA * call)
{
	call->next = call_out_list;
	call_out_list = call;
}


void WAIT_MAGIC_STATE( CHAR_DATA *ch, int delay)
{
	int nPerc = get_skill_percent(ch, gsn_spellcraft);
	if(nPerc > 0)
	{
		int nSklv = get_skill_level(ch, gsn_spellcraft);

		delay -= delay * (nSklv + 1) * (nSklv + 4) * nPerc / 10000;

		if (nSklv > 5) delay--;

		delay  = UMAX(0, delay);
	}
	WAIT_STATE(ch, delay);
}

void WAIT_STATE( CHAR_DATA *ch, int wait)
{
	if( IS_NPC(ch) && is_qmark(ch,"delayed_stun" ) )
	{
		ch->wait = 0;
		return;
	}

	if( wait == -999)
		wait = skill_table[gsn_bashhead].beats;
	else if( is_affected(ch, gsn_hasten) )
	{
		int sklv = get_skill_level(ch, gsn_hasten);
		wait = UMAX(1, (wait - sklv/2) / (sklv / 3+1));
	}

	int nMediSlow = affected_value(ch, gsn_meditate, APPLY_ACTION_ABILITY);

	if(is_affected(ch, gsn_meditate) && nMediSlow < 0)
	{
		if(nMediSlow < -50)
			nMediSlow = -50;
		wait = wait * 100 / (100 + nMediSlow);
	}

	int nSpriSlow = affected_value(ch, gsn_sprinkle_slash, APPLY_ACTION_ABILITY);
	if(is_affected(ch, gsn_sprinkle_slash) && nSpriSlow < 0){
		if(nSpriSlow < -50)
			nSpriSlow = -50;
		wait = wait * 100 / (100 + nSpriSlow);
	}
	ch->wait = UMIN( 300, ch->wait + wait );
}

int GetGeneralMobSklv(CHAR_DATA *ch)
{
    int moblv = 0;
	QMARK *qmark = NULL;
    if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        //return GetSorGateMobLevel(ch->pIndexData->vnum) * 2 + HasMageSorMaster(ch);
		if( (qmark = is_qmark( ch, "sum_count" ))!= NULL ){
			return URANGE(0,(qmark->v2)+GetSorGateMobLevel(ch->pIndexData->vnum)+HasMageSorMaster(ch),5);
		}else{
			return GetSorGateMobLevel(ch->pIndexData->vnum);
		}
    }
    if(ch->level > 35)
		moblv = (ch->level - 35) / 5;
    if(IS_SET(ch->special, EXPERT))
		moblv = (moblv + 7) / 2;
    moblv = UMIN(6, moblv);
    return moblv;
}

int get_skill_level( CHAR_DATA *ch, int sn )
{
    //int moblv = 0;
    //int roomchar = 0;
    //CHAR_DATA *vch;
    
    if( !ch)
        return 0;
        
    if( IS_NPC(ch) ) 
    {
        return GetGeneralMobSklv(ch);
        //if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
        //    return GetSorGateMobLevel(ch->pIndexData->vnum) * 2;
        //}else{
				//	  if(ch->level > 35) moblv = (ch->level - 35) / 5;
				//		if(IS_SET(ch->special, EXPERT)) moblv = (moblv + 7) / 2;
				//		moblv = UMIN(6, moblv);
					  /* Previous MOB Sklv Settings *
            if( ch->level > 40 ) ++moblv;
            if( ch->level > 50 ) ++moblv;
            if( ch->level > 60 ) 
            {
                if ( !ch->deleted ) {   //Added by Razgriz  20050622
                    for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
                        if( vch && !IS_NPC( vch ) )
                            roomchar ++;
                         
                    moblv += ( roomchar / 10 );
                }       //Added by Razgriz  20050622
                if( ch->level > 65 ) 
                    ++moblv;
            }
            
            if( IS_SET(ch->special, EXPERT) ) moblv+=2;
            moblv = UMIN(6,moblv);
						*/
        //}
        //return moblv;
    }
    else if( ch->class < WIZARD && ch->level > 29){
        return 2;
    }

    //20060805 modify by jye
    //修改回傳上限為6
    return ( UMIN(6,ch->pcdata->learned[sn] / 1000) );
}

int gain_skill_level( CHAR_DATA *ch, int sn, int value )
{
	int nOrgSklv = get_skill_level(ch, sn);
	int nNewSklv = 0;
	int nRace    = ch->race;
	if( IS_NPC(ch) ) return 0;
	//mod = UMAX( 0 , mod + value );
	nNewSklv = nOrgSklv + value;

	if(sn == gsn_inborn_ability){
		// race detect magic pp , md bonus
		if(IS_SET(race_table[ nRace ].race_abilities, RACE_DETECT_MAGIC)){
			ch->pp -= nOrgSklv * nOrgSklv / 5;
			ch->pp += nNewSklv * nNewSklv / 5;
			ch->amdr -= nOrgSklv * nOrgSklv;
			ch->amdr += nNewSklv * nNewSklv;
		}
		// updating the DC of natural fly based on sklv, added by Keelar
		if(IS_SET(race_table[ nRace ].race_abilities, RACE_FLY)){
			ch->dodge += nOrgSklv * 2 + (nOrgSklv * nOrgSklv) / 2;
			ch->dodge -= nNewSklv * 2 + (nNewSklv * nNewSklv) / 2;
		}
		// removing beholder's eq when necessary, added by Keelar
		if(IS_SET(race_table[ nRace ].race_abilities, RACE_BEHOLDER)){
			if(nNewSklv < 6 && nOrgSklv >= 6)
			  fremove_obj( ch, WEAR_ARMS_2, TRUE );
			if(nNewSklv < 5 && nOrgSklv >= 5)
			  fremove_obj( ch, WEAR_HANDS_2, TRUE );
		}
	}else if(sn == gsn_armor_mastery){
		ch->pdr -= GetArmorMasteryPdrBonus(ch);
		ch->mdr -= GetArmorMasteryPdrBonus(ch)/2;
	}

	ch->pcdata->learned[sn] = nNewSklv * 1000 + get_skill_percent(ch, sn);

	if(sn == gsn_armor_mastery){
		ch->pdr += GetArmorMasteryPdrBonus(ch);
		ch->mdr += GetArmorMasteryPdrBonus(ch)/2;
	}
	return nNewSklv;

}


int gain_skill_percent( CHAR_DATA *ch, int sn, int value )
{
    int mod = get_skill_percent(ch, sn);
    if( IS_NPC(ch) ) return 0;

    mod = UMIN( skill_max_percent(ch, sn)  , mod + value ); ch->pcdata->learned[sn] = mod + (ch->class >= WIZARD ? get_skill_level(ch, sn) * 1000 : 0 );
    return mod;

}

int skill_max_percent( CHAR_DATA *ch, int sn)
{
    if( IS_NPC(ch) ) return 0;

    return ( ( ( skill_table[sn].skill_level[ch->class] ) / 1000 ) );
}


int skill_preq_level( CHAR_DATA *ch, int sn)
{
    if( IS_NPC(ch) ) return 0;

    return (( ( skill_table[sn].skill_level[apply_class[ch->pcdata->prime_attr]] ) /10 % 100 )-1);
}

int skill_req_level( CHAR_DATA *ch, int sn)
{
    if( IS_NPC(ch) ) return 0;

    return (( ( skill_table[sn].skill_level[ch->class] ) /10 % 100 ) -1 );
}

int get_skill_percent( CHAR_DATA *ch, int sn)
{
	if( IS_NPC(ch) ) {
		if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){
			return GetSorGateMobLevel(ch->pIndexData->vnum) * 10 + 50;
		}
		return 0;
	}
	if( sn > MAX_SKILL || sn < 1 ) {
		//bugf("get_skill_percent: sn overflow[%d]",sn);
		bugf("%s get_skill_percent : sn overflow[%d]",ch->name , sn);
		return 0;
	}
	return ( ch->pcdata->learned[sn] % 1000 );
}


int GET_AC( CHAR_DATA *ch )
{
	int armor;
	int bonus;      //add by coconet 0703 for AM bonus
	int sklv;
	int nMobIndex = 0;
  
	armor = ch->armor - race_table[ch->race].nature_ac; 
	// 對 monk ac 新增 IsUnderFistFight 的判定 2022/02/24
	if( !IS_NPC(ch) && IsUnderFistFight(ch) )
	{
		int MonAc = 0;
		int WeightRate = 0;

		MonAc += race_table[ch->race].nature_ac;
		MonAc += con_app[get_curr_con(ch)].armor;
		MonAc = MonAc/3;
		
		WeightRate = UMIN( 1000 , can_carry_w(ch) ) - ch->carry_weight*2;

		sklv = get_skill_level( ch, gsn_fist_fight );
		if( ch->class == MONK && WeightRate >= 0 )
		{
			// 修改 monk ac 隨負重比例及技能等級變化 2022/02/24
			//if( ch->carry_weight * 3 <= can_carry_w(ch) )
			//	armor = armor - ch->level * 2;
			armor = armor - ch->level * (2 + sklv/3 );
			if( MonAc < 0 )
				armor += MonAc * WeightRate * ( 7 + sklv/2 ) / 10000;
		}
	}

	armor += con_app[get_curr_con(ch)].armor;
	armor += race_table[ch->race].nature_ac;
 
	if (IsSorGateMobByChar(ch)) {
		nMobIndex = ch->pIndexData->vnum;
		if (IsSorGateMobEarth(nMobIndex)) {
			armor += SorGateMobBonus(ch, -280);
		} else if (IsSorGateMobLightning(nMobIndex)){
			armor += SorGateMobBonus(ch, -220);
		} else {
			armor += SorGateMobBonus(ch, -150);
		}
	}

	//modified by coconet for modify AM's affect
	if( !IS_NPC(ch) && get_skill_percent(ch,gsn_armor_mastery) > 0 )
	{
		sklv = get_skill_level( ch,gsn_armor_mastery );
		bonus = GetArmorMasteryAcBonus(ch);
		if(armor < 0){
			if( ch->pcdata->prime_attr == APPLY_STR ){
				armor  = armor * (100 + 1.3 * bonus) / 100;
				if(get_skill_level(ch, gsn_armor_mastery) >= 5) armor -= 100;
				if(get_skill_level(ch, gsn_armor_mastery) >= 6) armor -= 150;
			}else{
				armor  = armor * (100 + 1.1 * bonus) / 100;
				if(get_skill_level(ch, gsn_armor_mastery) >= 5) armor -= 80;
				if(get_skill_level(ch, gsn_armor_mastery) >= 6) armor -= 100;
			}
		}else{
			armor  = armor - 100;
		}
	}
	return armor;
}

// merit bonus skexp system developed by keelar
int get_merit_bonus_max_skexp( CHAR_DATA *ch )
{
	if (IS_NPC(ch)) return 0;
	if (ch->class < WIZARD) return 0;

	int merit = ch->pcdata->merit;

	return ceil(MERIT_BONUS_MAX_SKEXP - (double) MERIT_BONUS_MAX_SKEXP * BASE_MERIT / (BASE_MERIT + merit));
}

int get_resp_bonus_max_skexp( CHAR_DATA *ch )
{
	if (IS_NPC(ch)) return 0;
	if (ch->class < WIZARD) return 0;

	int resp = ch->pcdata->respect;
	if (resp > 150) resp = 150;

	return resp * 1000;
}

int get_max_skill_exp( CHAR_DATA *ch )
{
	QMARK *qm;
	int i = 0;
	int m = 0;

	if( IS_NPC(ch) ) return 0;
	if( ( qm = is_qmark(ch, "max_skill_exp" )))
		i += qm->v0;

	if( ch->class < WIZARD ) return 0;
	m = (UMIN(36,ch->level))*35000;
	m += i * 3000;
	m +=( ch->level > 36 ?  ((ch->level - 36) ) * 20000 : 0);

	m += get_merit_bonus_max_skexp(ch);
	m += get_resp_bonus_max_skexp(ch);

	return m;
}

void gain_merit(CHAR_DATA *ch, int merit)
{
	if (IS_NPC(ch)) return;
	char buf[1000];

	if (ch->level < 41) {
		if( !is_qmark(ch, "enable_level_41")) {
			send_to_char("你尚未成為英雄還不能建立功\勳。\n\r", ch);
			return;
		}
	}

	if (merit >= 0) {
		sprintf(buf, "你的功\勳提升了 %d 點！\n\r", merit);
	} else {
		sprintf(buf, "你的功\勳降低了 %d 點！\n\r", -merit);
	}
	send_to_char(buf, ch);
	ch->pcdata->merit += merit;
}

bool has_free_skdemote(CHAR_DATA *ch)
{
	if (ch->level < 43) return TRUE;    //   skdemote free enable by toi 20130108 
	if (ch->in_room->vnum == 11000) return TRUE;

	/* here, set special classes which have been just patched
		switch(ch->class)
		{
			case WIZARD:
			case ALCHEMIST:
			case SORCERER:
			case SAVANT:
			case BISHOP:
			return TRUE;
		}
	*/

	return FALSE;
}

void regain_skill_exp( CHAR_DATA *ch, int xp)
{
	char buf[1000];
	if( IS_NPC(ch) ) return;
	ch->pcdata->skexp_earned -= xp; 
	ch->pcdata->skexp_used -= xp;
	if (!has_free_skdemote(ch)) {
		gain_skill_exp(ch, ( xp - xp/20 ) );
		sprintf(buf, "你失去了 %d 點技能經驗點.\n\r", xp / 20);
		send_to_char(buf, ch);
	}else{
		gain_skill_exp(ch, xp );
	}
}


void gain_skill_exp( CHAR_DATA *ch, int xp)
{
	if( IS_NPC(ch) ) return;
	//if( ch->class < WIZARD ) return;
	// ch->pcdata->skexp_earned = UMIN( ch->pcdata->skexp_earned + xp , get_max_skill_exp(ch));
	ch->pcdata->skexp_earned = ch->pcdata->skexp_earned + xp;
}

int get_eff_skill_exp( CHAR_DATA *ch)
{
	if( IS_NPC(ch) ) return 0;
	if( ch->pcdata->skexp_earned > get_max_skill_exp(ch))
		return get_max_skill_exp(ch) - ch->pcdata->skexp_used;
	else
		return ch->pcdata->skexp_earned - ch->pcdata->skexp_used;
}

int use_skill_exp(CHAR_DATA *ch, int value)
{
	if( IS_NPC(ch) ) return 0;
	// value = UMIN(value, (ch->pcdata->skexp_earned - ch->pcdata->skexp_used));
	ch->pcdata->skexp_used += value;
	return value;
}

int get_skill_exp(CHAR_DATA *ch)
{
	if( IS_NPC(ch) ) return 0;
	return ch->pcdata->skexp_earned;
}

bool consented( CHAR_DATA *ch, OBJ_DATA *cor)
{
	char buf[MAX_INPUT_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	char *c;

	if( IS_NPC(ch) ) return FALSE;
	if( IS_IMMORTAL(ch) ) return TRUE;
	if( !cor ) return FALSE;
	c = one_argument(cor->name, arg); // "corpse"
	one_argument(c, arg);
	sprintf(buf, "consent_%s", arg);
	return (is_qmark(ch, buf)) ? TRUE : FALSE;
}

#define ENABLE_INBORN_ABILITY 1

int skill_difficulty( CHAR_DATA *ch, int sn)
{
    int lv = get_skill_level(ch, sn);
    int cost, nRaceAbiID;
    // AFFECT_DATA *pAffData;
    if( IS_NPC(ch) || ch->class < WIZARD ) return -1;
    if( get_skill_level(ch, sn) >5 ) return -1;

    if( skill_table[sn].apply_attr[ch->pcdata->prime_attr-1] < 0 ) return -1;
    if( skill_table[sn].apply_attr[ch->pcdata->snd_attr-1] < 0 ) return -1;

    if(sn != gsn_inborn_ability){
        return ((skill_table[sn].apply_attr[ch->pcdata->prime_attr-1] * 3 +
                skill_table[sn].apply_attr[ch->pcdata->snd_attr-1] * 2) *
                (lv/4+1)*(lv/5 +1)*(lv+1) * 300);
    }else{
        /*
        for(pAffData = ch->affected; pAffData; pAffData = pAffData->next){
            if(pAffData->type == skill_lookup( "polymorph other" )){
                return -1;
            }
        }
        if(!IS_SET(race_table[ch->race].race_abilities, RACE_PC_AVAIL)) return -1;
        */
        int nRace  = ch->race;
        if(is_affected(ch, gsn_werewolf)){
            nRace -= affected_value(ch, gsn_werewolf, APPLY_RACE);
            while(nRace >= MAX_RACE){ nRace -= MAX_RACE; }
            while(nRace  < 0) { nRace += MAX_RACE; }
        }
        if(is_affected(ch, gsn_stone_form)){
            nRace -= affected_value(ch, gsn_stone_form, APPLY_RACE);
            while(nRace >= MAX_RACE){ nRace -= MAX_RACE; }
            while(nRace  < 0) { nRace += MAX_RACE; }
				}
        cost = 0;
        for(nRaceAbiID = 1; nRaceAbiID < RACE_ABILITY_COUNT; nRaceAbiID++){
            if(IS_SET( race_table[nRace].race_abilities, 1 << (nRaceAbiID-1)))
                cost += inborn_ability_cost[nRaceAbiID];
        }
#if ENABLE_INBORN_ABILITY
        return cost * 100 * (lv / 4 + 1) * (lv / 5 + 1) * (lv + 1);
#else
        return -1;
#endif
    }
    

    
    return 0;
}

bool IS_HORSE(CHAR_DATA *ch)
{
	if( IS_NPC(ch) ) return FALSE;

	return (IS_SET(ch->act, PLR_HORSE)) ? TRUE :FALSE;
}

bool check_int( int i )
{
	return TRUE;
}

int artifact(CHAR_DATA *ch, int curr_wis, int obj_percent, int bonus)
{
	if (IS_NPC(ch) || IS_SET(ch->act, PLR_ARTIFACT)) {
		int arti_perc = get_skill_percent(ch, gsn_artifact);
		int arti_sklv = get_skill_level(ch, gsn_artifact);
		int race_ib = 0;
		// inborn DETECT_ALIGN 的 race (fairy , gnome)  有額外artifact 機率加成 2020/05/29
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
			race_ib = get_skill_level(ch, gsn_inborn_ability);
		if (IS_NPC(ch)) arti_perc = 100;
		// the maximum here without bonus point is 80 + 50 = 130
		// for wis 18, arti sklv 6, it would be 80 + 36 = 116
		// dectect_align race the maximum here without bonus point is 80 + 62 = 142
		// dectect_align race for wis 18, arti sklv 6, it would be 80 + 36 = 128
		int chance = (arti_sklv + 2) * (arti_sklv + 4) + (curr_wis + race_ib) * 2;
		chance = chance * (100 + bonus * arti_sklv / 3) / 100;

		return chance * obj_percent * arti_perc / 10000;
	}
	return 0;
}


void add_member( CHAR_DATA *ch, CHAR_DATA *member)
{
	GROUP_DATA * pGroup = ch->group;
	if( !pGroup ) {
		bugf("[add]%s has no group!", ch->name);
		return;
	}
	if( ch == member ) {
		bugf("[add] %s is trying to group with itself",ch->name);
		return;
	}
	if( member->group ) {
		bugf("[add] %s already has group",member->name);
		return;
	}

	member->group = pGroup;
	member->next_in_group = pGroup->members;
	pGroup->members = member;
}

void remove_member( CHAR_DATA *member)
{
	GROUP_DATA *pGroup = member->group;
	CHAR_DATA *next;
	if( !pGroup ) {
		bugf("[remove]%s has no group!", member->name);
		return;
	}

	if( member == pGroup->leader ) {
		clear_member(member);
		return;
	}

	if( pGroup->members == member ){
		pGroup->members = member->next_in_group;
	}
	else{
		for( next = pGroup->members ; next ;next=next->next_in_group ){
			if(next->next_in_group == member ) {
				next->next_in_group = member->next_in_group;
			}
		}
	}
	member->next_in_group = NULL;
	member->group = NULL;
	// add at 2021/11/20
	if( ( member->linking ) )
	{
		stop_linking(member,member->linking);
	}
	// add at 2021/11/20
	if( ( member->cooperator ) )
	{
		remove_cooperator(member);
	}
	if( ( member->cooperating ) )
	{
		stop_cooperating(member,member->cooperating);
	}
	if ( ( member->protector ) )
	{
		remove_protector(member);
	}
	if ( ( member->protecting ) )
	{
		stop_protecting(member,member->protecting );
	}
    
}

void clear_member( CHAR_DATA *ch)
{
	CHAR_DATA *pCh, *pCh_next;
	GROUP_DATA *pG;

	if( !ch->group ) {
		bugf("[clear]%s has no group!", ch->name);
		return;
	}
	pG= ch->group;

	for( pCh = pG->members ; pCh; pCh = pCh_next){
		pCh_next = pCh->next_in_group;
		pCh->next_in_group = NULL;
		pCh->group = NULL;
		// add at 2021/11/20
		if( ( pCh->linking ) )
		{
			stop_linking(pCh, pCh->linking);
		}
		// add at 2021/11/20
		if( ( pCh->cooperator ) )
		{
			remove_cooperator(pCh);
		}
		if( ( pCh->cooperating ) )
		{
			stop_cooperating(pCh, pCh->cooperating);
		}
		if ( ( pCh->protector ) )
		{
			remove_protector(pCh);
		}
		if ( ( pCh->protecting ) )
		{
			stop_protecting(pCh, pCh->protecting);
		}
	}
	pG->members = NULL;
	free_group(pG);
}

/** 檢查 npc 是否成功使用 sn 的技能，檢定的機率是 chance
 *  預設是都傳回true
 *
 * last update: zcecil 2004/07/04
 */
bool npc_check_skill(CHAR_DATA *ch,int sn, int chance)
{
	return TRUE;
}

/** 檢查 pc 是否成功使用 sn 的技能，如果 learned sn >= chance
 *  則成功
 *
 *  last update: zcecil 2004/07/04
 */
bool check_skill(CHAR_DATA *ch,int sn, int chance)
{
	if( IS_NPC(ch) ) {
		return npc_check_skill(ch,sn,chance);
	}
	return( get_skill_percent(ch,sn) < chance ) ? FALSE : TRUE;

}

/* Added by Razgriz	20050908
 * DESCRIPTION
 * 判斷 ch 是否可以從房間中某個出口的方向(check)離開
 * 假如 check = 6, 代表檢查全部的出口
 * 0:北 1:東 2:南 3:西 4:上 5:下
 * 
 * RETURN VALUE
 * find 表示找到幾個可以離開的出口, 若 check 不為 6
 * 那 find 值只會為 0 或 1
 */
int check_exit ( CHAR_DATA * ch, int check )
{
	int i, door, find = 0, max_exit = 6;
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA	*in_room;
	ROOM_INDEX_DATA	*to_room;

	if (check < 0)
		return find;
	if (check > max_exit)
		check = max_exit;
	if (check == max_exit)
		door = 0;
	else
		door = check;
	
	for (i = door; i <= check && i < max_exit; i++)
	{
		//check room exist
		if( !(in_room = ch->in_room) )
			continue;

		//check road exist
		if ( !( pexit = in_room->exit[i] ) || !( to_room = pexit->to_room ) )
			continue;

		//check if ch can pass door
		if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			if ( !IS_AFFECTED( ch, AFF_PASS_DOOR )
				&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) )
				continue;

			if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
				continue;
		}

		find++;
	}

	return find;
}
