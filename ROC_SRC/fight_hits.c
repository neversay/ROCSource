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
//*  PROGRAM ID  : fight_hits.c                                 *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940611 jye                                    *
//*  PURPOSE     :         修改met之目標有被pro時變slash之bug     *
//*              :         及ber之目標有被pro時會被取消之bug      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941012 Razgriz                                *
//*  PURPOSE     :         mone_hit() 加入中文註解                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941014 Razgriz                                *
//*  PURPOSE     :         改寫 mone_hit() 介面                   *
//*              :         移除 brief_mone_hit                    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941015 Razgriz                                *
//*  PURPOSE     :         加入背刺的 hr 加成,以及夜襲的hr dr加成 *
//*****************************************************************

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

extern int check_item_magic_flag(OBJ_DATA *, int, int);
extern bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);
extern bool check_race_special args( ( CHAR_DATA *ch ) );
extern bool check_evade args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
//extern bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff ) );
extern bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn ) );
extern bool check_block args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff ) );
extern bool check_tactics args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

extern void death_cry args( ( CHAR_DATA *ch ) );
extern void group_gain args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
extern int  xp_compute args( ( CHAR_DATA *gch, CHAR_DATA *victim ) );
extern bool is_wielding_poisoned args( ( CHAR_DATA *ch, int wpn ) );
extern void make_corpse args( ( CHAR_DATA *killer, CHAR_DATA *ch ) );
extern int  artifact(CHAR_DATA *ch, int curr_wis, int obj_percent, int bonus);

extern void check_position( CHAR_DATA *ch, CHAR_DATA *victim, int dam );
extern int  GetDamageUnderProtection(CHAR_DATA *ch, CHAR_DATA *victim, int nDamage);
/*
 * Local functions.
 */
extern void dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
extern void c_dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
int one_hit args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn ) );
int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode ); //Modified by Razgriz 20051014
//int   brief_mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int* dt, int wpn );

void use_magical_item args( ( CHAR_DATA *ch ) );
void track_hunt args( (CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door ) );
int interpolate1( int level1, int value1, int level2, int value2, int level );
void check_corpse_count( CHAR_DATA * ch, CHAR_DATA *victim);
int get_align( CHAR_DATA *ch );
void show_vs_graphy(CHAR_DATA *ch, CHAR_DATA *victim);
void death_declare(CHAR_DATA *ch, CHAR_DATA *victim);
void pk_kill(CHAR_DATA *ch, CHAR_DATA *victim);

extern QMARK *qmark_list, *qmark_free;
extern QMARK_STRING *qmark_string_list;
extern CLAN_WAR_DATA *clan_war;

extern char* stat_color1(int cur, int max);

extern bool check_can_do_hit(CHAR_DATA *ch,CHAR_DATA *victim);

int GetAttackChance(CHAR_DATA *ch, int nGsn, int nMod)
{
	int nSklv = get_skill_level(ch,nGsn);
	int nPerc = get_skill_percent(ch, nGsn);
	int nChance = nSklv * (nSklv + 6) + nPerc / nMod;

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ))
	{
		int nInbSklv = get_skill_level(ch, gsn_inborn_ability);
		nChance += 10 + nInbSklv * (nInbSklv + 4);
	}

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_WEREWOLF ))
	{
		int nInbSklv = get_skill_level(ch, gsn_inborn_ability);
		nChance += 15 + nInbSklv * nInbSklv;
	}

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		if (   !get_eq_char( ch, WEAR_WIELD )
			&& !get_eq_char( ch, WEAR_WIELD_2 )
			&& !get_eq_char( ch, WEAR_TWO_HAND ) )
		{
			int nInbSklv = get_skill_level(ch, gsn_inborn_ability);
			nChance += 5 + (nInbSklv + 2) * (nInbSklv + 1) / 4;
		}
	}
	return nChance;
}

bool IsUsingShortWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{

	if ( !wield ) return FALSE;

	if ( !wield || wield->value[3] != 11 ) // dagger
	{
		return FALSE;
	}
	return TRUE;
}

bool IsUsingMediumWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{

	if ( !wield ) return FALSE;

	if (wield) {
		switch(wield->value[3]) {
			case 1: // blade, slice
			case 3: // sword, slash
			case 13: // axe, machete, chop
			return TRUE;
		}
	}
	return FALSE;
}

bool IsUsingLongWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{

	if ( !wield ) return FALSE;

	if (wield) {
		switch(wield->value[3]) {
			case 2: // lance, stab
			case 7: // staff, mace, pound
			return TRUE;
		}
	}
	return FALSE;
}

bool IsUsingHeavyWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{
	if ( !wield ) return FALSE;

	if (wield) {
		switch(wield->value[3]) {
			case 8:  // greathammer, crush
			case 9:  // greataxe,halberd, grep
			return TRUE;
		}
	}
	return FALSE;
}

bool IsUsingMagicWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{
	if ( !wield ) return FALSE;

	if (wield) {
		switch(wield->value[3]) {
			case 6:  // blast
			case 12: // suction
			return TRUE;
		}
	}
	return FALSE;
}

//int IsUsingLance(CHAR_DATA *ch)
bool IsUsingLance(CHAR_DATA *ch, OBJ_DATA *wield )
{
	/*OBJ_DATA *wield = NULL;
	if( get_eq_char( ch , WEAR_WIELD ) )
		wield = get_eq_char(ch, WEAR_WIELD) ;
	else if( get_eq_char( ch, WEAR_WIELD_2 ) )
		wield = get_eq_char(ch, WEAR_WIELD_2) ;
	else if( get_eq_char( ch, WEAR_TWO_HAND ) )
		wield = get_eq_char(ch, WEAR_TWO_HAND ); */

	if ( !wield ) return FALSE;
	if ( !wield || wield->value[3] != 2 )
	{
		return FALSE;
	}
	return TRUE;
}

bool IsUsingBowShot(CHAR_DATA *ch, OBJ_DATA *wield )
{

	OBJ_DATA *arrow = NULL;

	if ( !wield ) return FALSE;

	if( wield->wear_loc != WEAR_TWO_HAND )
		return FALSE;

	if( get_eq_char( ch, WEAR_ARROW ) )
		arrow = get_eq_char( ch, WEAR_ARROW );

	if( !arrow || !wield || wield->value[3] != 14 ) // bow shot
	{
		return FALSE;
	}
	return 1;
}

bool IsUsingGunShot( CHAR_DATA *ch , int wpn )
{
	OBJ_DATA *ammo = NULL;
	OBJ_DATA *wp = NULL;

	if( get_eq_char( ch , WEAR_TWO_HAND ) )
		wp = get_eq_char( ch, WEAR_TWO_HAND );
	else
		wp = get_eq_char( ch, wpn );

	if( !wp ) return FALSE;

	if( wp->value[3] != 16 ) // gun ammo
		return FALSE;

	if( get_eq_char( ch, WEAR_AMMO ) )
		ammo = get_eq_char( ch, WEAR_AMMO );
	
	if( !ammo ) return FALSE;
		
	//if( ammo->item_type != ITEM_AMMO || ammo->value[0] <= 0 )
	if( ammo->item_type != ITEM_AMMO )
		return FALSE;

	// 增加 -1 情況的判定 2023/01/05
	if( ammo->value[0] <= 0 && ammo->value[0] != -1 )
		return FALSE;

	return TRUE;
}

// 修改為帶入 weapon 判定 2022/12/03
//int IsUsingKnightWeapon(CHAR_DATA *ch)
bool IsUsingKnightWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{
	if ( !wield ) return FALSE;

	if( get_skill_level(ch, gsn_lance_mastery) > 5 ){
		if (wield) {
			switch(wield->value[3]) {
				case 1: // blade, slice
				case 2: // lance, stab
				case 3: // sword, slash
				case 7: // staff, mace, pound
				case 9: // greataxe,halberd, grep
					return TRUE;
			}
		}
	}else if( wield->value[3] == 2 ){
		return TRUE;
	}

	/*OBJ_DATA *wield = NULL;

	if( get_eq_char( ch , WEAR_WIELD ) )
		wield = get_eq_char(ch, WEAR_WIELD) ;
	else if( get_eq_char( ch, WEAR_WIELD_2 ) )
		wield = get_eq_char(ch, WEAR_WIELD_2) ;
	else if( get_eq_char( ch, WEAR_TWO_HAND ) )
		wield = get_eq_char(ch, WEAR_TWO_HAND );

	if (wield) {
		switch(wield->value[3]) {
			case 1: // blade, slice
			case 2: // lance, stab
			case 3: // sword, slash
			case 7: // staff, mace, pound
			case 9: // greataxe,halberd, grep
			return 1;
		}
	}*/

	return FALSE;
}

// 修改為帶入 weapon 判定 2022/12/03
//int IsUsingTrickyWeapon(CHAR_DATA *ch )
bool IsUsingTrickyWeapon(CHAR_DATA *ch, OBJ_DATA *wield )
{
	if ( !wield ) return FALSE;

	if( get_skill_level(ch, gsn_tricky_pacing) > 4 ){
		if (wield) {
			switch(wield->value[3]) {
				case 4:  // whip
				case 5:  // claw
				case 10: // bite
				case 11: // dagger
					return TRUE;
				case 14: // bow
					if(IsUsingBowShot)
						return TRUE;
					else
						return FALSE;
			}
		}
	}else if( wield->value[3] == 11 ){
		return TRUE;
	}

	/*OBJ_DATA *wield = NULL;

	if( get_eq_char( ch , WEAR_WIELD ) )
		wield = get_eq_char(ch, WEAR_WIELD) ;
	else if( get_eq_char( ch, WEAR_WIELD_2 ) )
		wield = get_eq_char(ch, WEAR_WIELD_2) ;
	else if( get_eq_char( ch, WEAR_TWO_HAND ) )
		wield = get_eq_char(ch, WEAR_TWO_HAND );

	if (wield) {
		switch(wield->value[3]) {
			case 4:  // whip
			case 5:  // claw
			case 10: // bite
			return 1;
		}
	} */
	return FALSE;
}

int GetBonusedValue_LanceMaster(CHAR_DATA *ch, int nPerc)
{
	int nLancePerc = get_skill_percent(ch, gsn_lance_mastery);
	int nLanceSklv = get_skill_level(ch, gsn_lance_mastery);

	if(IS_NPC(ch)) return nPerc;

	if(nLancePerc == 0) return nPerc;

	// 將 IsUsingKnightWeapon 的 weapon 分離出去 2022/12/03
	//if(ch->class != KNIGHT) return nPerc;

	/*if(!IsUsingLance(ch)) {
		if (nLanceSklv < 5) return nPerc;
		if (!IsUsingKnightWeapon(ch)) return nPerc;
	}*/

	return nPerc * (100 + nLanceSklv * (nLanceSklv + 4)) * (1 + nLancePerc) / 10000; // lv 6  *1.6
}

int TrickyPacingBase(CHAR_DATA *ch, int nPerc)
{
	int nTrickyPerc = get_skill_percent(ch, gsn_tricky_pacing);
	int nTrickySklv = get_skill_level(ch, gsn_tricky_pacing);
	if(IS_NPC(ch)) return nPerc;

	if(nTrickyPerc == 0) return nPerc;

	//if(ch->class != ASSASSIN && ch->class != NINJA && ch->class != RANGER) return nPerc;

	// 將 IsUsingTrickyWeapon 的 weapon 分離出去 2022/12/03
	/*if(!IsUsingShortWeapon(ch)) {
		if (nTrickySklv < 4) return nPerc;
		if (!IsUsingTrickyWeapon(ch)) {
			if (!IsUsingBowShot(ch)) return nPerc;
		}
	} */

	return nPerc * (100 + nTrickySklv * (nTrickySklv + 4)) * (1 + nTrickyPerc) / 10000;
}

// 針對 artifact 的情況, 讓被 sleep 及charm 的目標會確實停止攻擊, 避免造成crash 2022/05/03
bool HitSleepCharm(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if( !victim || ( victim == NULL ) || victim->deleted ) return TRUE;
	if( ch->deleted ) return TRUE;
	if( IS_AFFECTED( victim, AFF_SLEEP ) )
		return TRUE;
	if( IS_AFFECTED( ch, AFF_SLEEP ) ) // ch 執行 multi_hit 中可能被反擊造成 sleep
		return TRUE;

	// charm 的部分修改, 加入攻擊者是否為 master 2022/12/31
	if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch )
		return TRUE;
	if( IS_AFFECTED( ch, AFF_CHARM ) && ch->fighting == victim && ch->master == victim ) // ch 執行 multi_hit 中可能被反擊造成 charm
		return TRUE;

	return FALSE;
}

// 針對 飛行武器 在發射後的dr hr apdr 屬性加成 2022/05/06
int ShootObjAffect( CHAR_DATA *ch, OBJ_DATA *obj , int type )
{
	AFFECT_DATA *af;
	int number = 0;
	
	if( !ch ) return number;
	
	if( !obj ) return number;

	for( af = obj->affected; af; af = af->next ){
		// hr
		if( af->location == APPLY_SHOOT_HITROLL && type == 1 )
			number = number + af->modifier;
		// dr
		if( af->location == APPLY_SHOOT_DAMROLL && type == 2 )
			number = number + af->modifier;
		// apdr
		if( af->location == APPLY_SHOOT_ADAMAGE && type == 3 )
			number = number + af->modifier;
	}
	return number;
}

// 針對 飛行武器 在發射後數量計算 2023/01/04
void ShotObjCount( CHAR_DATA *ch , OBJ_DATA *obj )
{

	if( !ch || ch->deleted )
		return;

	if( IS_NPC(ch) )
		return;

	if( !obj )
		return;

	if( obj->item_type != ITEM_ARROW && obj->item_type != ITEM_AMMO )
		return;

	if( obj->value[0] > 0 )
		obj->value[0] -= (IS_NPC( ch ) && !ch->clan && !IS_AFFECTED(ch, AFF_CHARM) && !IS_AFFECTED( ch, AFF_SUMMONED)) ? 0 : 1;

	// 可能被 purge 掉
	if( !obj )
		return;

	// 比 0 小就射完脫掉
	if( obj->value[0] <= 0 )
	{
		if( !obj )
			return;

		// no remove 將不會自動脫掉
		if( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) )
			return;


		// -1 的情況為無限, 在 mone_hit 中有作判定, 理論上這邊跑不到
		if( obj->value[0] == -1 )
			return;

		// 箭矢的部分
		if( obj->item_type == ITEM_ARROW )
		{
			// magic arrow 射光後自我消失 2022/04/29
			if( obj->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW ){
				//send_to_char("你的魔法箭射完了。\n\r", ch);
				act( "你的 $p 射完了.", ch, obj, NULL, TO_CHAR ); // 修改為 act 顯示射光的物品 2023/01/05
				unequip_char(ch, obj);
				obj_from_char(obj);
				extract_obj(obj);
			}else{
				//send_to_char("你的箭射完了。\n\r", ch);
				act( "你的 $p 射完了.", ch, obj, NULL, TO_CHAR );
				unequip_char(ch, obj);
			}
		}
		// 彈藥的部分
		if( obj->item_type == ITEM_AMMO )
		{
			// magic ammo 射光後自我消失 2022/04/29
			if( obj->pIndexData->vnum == OBJ_VNUM_MAGIC_AMMO ){
				//send_to_char("你的魔法彈用完了。\n\r", ch);
				// 修改為 act 顯示射光的物品 2023/01/05
				act( "你的 $p 射光了.", ch, obj, NULL, TO_CHAR );
				unequip_char(ch, obj);
				obj_from_char(obj);
				extract_obj(obj);
			}else{
				//send_to_char("你的彈藥用完了。\n\r", ch);
				act( "你的 $p 射光了.", ch, obj, NULL, TO_CHAR );
				unequip_char(ch, obj);
			}
		}
	}
	return;
}

void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{

	int chance1;
	int chance2;
	int chance3;
	int chance4;
	int bonus_chance; // 2022/12/03
	int nLancePerc = get_skill_percent(ch, gsn_lance_mastery); // 2022/12/03
	int nTrickyPerc = get_skill_percent(ch, gsn_tricky_pacing); // 2022/12/03
	int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
	int nPbSklv  = get_skill_level(ch, gsn_powerblow);
	int weight = 0;
	int min_str = 0;
	int str_diff = 0;
	int ch_dex;
	int gun_chance = 0; // 2022/05/05
	int totaldam = 0, dam = 0;
	int totalhit = 0, landedhit = 0;
	int mydt = dt;
	int mydt2;
	char ibuf[256];
	int vch_prog = 0;
	int ch_prog = 0;
	OBJ_DATA *wield = NULL;
	OBJ_DATA *arrow = NULL;  // add at 2022/05/02
	CHAR_DATA  *protector;

	if(!check_can_do_hit(ch,victim))
		return;
	if( IS_AFFECTED( ch, AFF_SLEEP ))
		return;

	/*
	* Set the fighting fields now.
	*/
	if( victim->position > POS_INCAP )
	{
		if( !victim->fighting ){
			set_fighting( victim, ch );
			// npc victim 被開戰 22022/10/31
			if( IS_NPC(victim) ) vch_prog = 1;
		}
		//victim->position = POS_FIGHTING;
	}

	if( !ch->fighting )
	{
		set_fighting( ch, victim );
		ch->position = POS_FIGHTING;
		// npc ch 開戰 2022/10/31
		if( IS_NPC(ch) ) ch_prog = 1;
	}

	//龍息與毒咬
	if( !IS_NPC( ch ) && ch->fighting )
	{
		if( check_race_special( ch ) )
		{
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// mob 如果有此 qmark 則不作普通攻擊 , 此處 dt 傳入為 TYPE_UNDEFINED 則必為普通攻擊 2022/10/31
	if( IS_NPC(ch) && is_qmark(ch,"disable_round_hit" ) && dt == TYPE_UNDEFINED )
		return;

	// 判定 protect target
	if( ( protector = victim->protector ) && protector != victim ) 
	{
		for( ; protector  && protector != protector->next_in_protect ; protector = protector ->next_in_protect ) 
		{
			if( protector != ch 
			   && protector != victim 
			   && ch->in_room
			   && protector->in_room == ch->in_room  )
			{
				if( ( IS_NPC( protector ) && ( (protector->level + GetGeneralMobSklv(protector)* 24)/2 > number_percent( ) ) )
					 || ( !IS_NPC( protector ) && ( get_skill_percent( protector, gsn_protect ) 
						   + get_skill_level(protector, gsn_protect) * 5 > number_percent() )))
				{
					//modify by jye
					//修改為不stop_fighting
					//因為stop_fighting會將ch之cast去掉
					//及met變slash之bug
					ch->fighting = NULL; 
					if( ch->class == RANGER )
					{
						ch->fighting = protector;
						if( ch->protector)
						{
							protector->fighting = ch->protector;
							victim->fighting = ch->protector;
						}
					}   
					else
					{
						set_fighting( ch, protector ); 
					}

					ch->temp_char = protector;
					act( "$n奮不顧身地保護$N！", protector, NULL, victim, TO_ROOM );
					act( "你奮不顧身地保護$N！", protector, NULL, victim, TO_CHAR );
					victim = protector;
					// victim->attacked = ch;
					break;
				}
				else
				{
					act( "$n想要保護$N，可惜失敗了！", protector, NULL, victim, TO_ROOM );
					act( "你想要保護$N，可惜失敗了！", protector, NULL, victim, TO_CHAR );
				}
			}
		}
	}

	++totalhit;

	// 針對流星箭會變為流星斬的問題作修正 2022/05/02
	if( dt == gsn_meteoric_arrow )
	{
		// 如果有箭則把 dt 從 gsn_meteoric_arrow 轉換為 TYPE_UNDEFINED (-1)
		if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
		{
			mydt = TYPE_UNDEFINED;
		}else{
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}
	//dam = brief_mone_hit( ch, victim, &mydt, WEAR_WIELD );
	dam = mone_hit ( ch, victim, &mydt, WEAR_WIELD, MSG_BRIEF, DT_CHANGE );

	if( dam >= 1 && dt == gsn_circle)
		dam = dam * weakness_damage(victim,dt)/100;
	  
	if(dam >= 0) landedhit++;

	if( dam >= 1 && mydt > 0)
	{
		totaldam += dam;
		sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", dt, totaldam);
		if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
			send_to_char(ibuf, ch);
		  
		if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
			send_to_char(ibuf, victim);
	}
    
	if( dam == -1 && !is_brief(ch, victim) ) 
	{
		dam_message(ch, victim, -1, mydt, WEAR_WIELD);
	}
  
	if( !ch->fighting 
		|| !ch->in_room 
		|| ch->in_room != victim->in_room 
		|| dt == gsn_backstab 
		|| dt == gsn_circle )
	{
		if( is_brief(ch, victim))
		{
			brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
		}
		// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
		if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
		{
			if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
			if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
		}
		return;
	}

	// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
	if( HitSleepCharm(ch,victim) ) return;

	if( IS_NPC( ch ) )
	{
		chance1 = ch->level << 2;
		chance2 = IS_SET( ch->class, 1 ) ? ch->level * 2 : ch->level << 1;
		chance3 = IS_SET( ch->class, 1 ) ? ch->level /2+ch->level/4: 0;
		chance4 = IS_SET( ch->class, 1 ) ? ch->level /3+ch->level/5: 0;
	}
	else
	{
		ch_dex = get_curr_dex( ch ) + ( IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ? 5 : 0 );
		chance1 = GetAttackChance(ch, gsn_second_attack, 2) + UMAX( 0, (ch_dex - 10) * 2 );
		chance2 = GetAttackChance(ch, gsn_third_attack,  2) + UMAX( 0, (ch_dex - 12) * 2 );
		chance3 = GetAttackChance(ch, gsn_fourth_attack, 3) + UMAX( 0, (ch_dex - 15) * 3 );
		chance4 = GetAttackChance(ch, gsn_fifth_attack,  4) + UMAX( 0, (ch_dex - 18) * 4 );
		/* lance mastery 及 tricky pacing 的判定修改為對武器而非 ch 加成 2022/12/03
		int nLanceBonus = GetBonusedValue_LanceMaster(ch, 100);
		int nTrickyBonus = TrickyPacingBase(ch, 100);
		chance1 = chance1 * nLanceBonus / 100;
		chance2 = chance2 * nLanceBonus / 100;
		chance3 = chance3 * nLanceBonus / 100;
		chance4 = chance4 * nLanceBonus / 100;
		chance1 = chance1 * nTrickyBonus / 100;
		chance2 = chance2 * nTrickyBonus / 100;
		chance3 = chance3 * nTrickyBonus / 100;
		chance4 = chance4 * nTrickyBonus / 100;*/
	}

	// 武器判定 2022/12/03

	if( get_eq_char( ch, WEAR_WIELD ) ){
		wield = get_eq_char( ch, WEAR_WIELD );
	}
	if( get_eq_char( ch, WEAR_TWO_HAND ) ){
		wield = get_eq_char( ch, WEAR_TWO_HAND );
	}

	// pc 的 weapon weight 判定 主要武器 及 雙手武器 是否太重, 給予敘述 2022/12/06
	if( wield && !IS_NPC(ch) )
	{
		if( IS_SET( ch->act, PLR_NOTIFY ) ) // 接受系統訊息才會顯示
		{
			weight = get_obj_weight( wield );

			switch( wield->wear_loc )
			{
				case WEAR_WIELD :
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
						if( str_app[min_str].wield >= weight ){
							break;
						}
					}
					str_diff = get_curr_str ( ch ) - min_str;
					if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
					if( str_diff < -5 )
						send_to_char(""CYN"你的主要武器太重了！"NOR"\n\r",ch);
					break;

				case WEAR_TWO_HAND :
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
						if ( str_app[min_str].twohand >= weight )
							break;
						}
					str_diff = get_curr_str ( ch ) - min_str;
					if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
					if( str_diff < -5 )
						send_to_char(""CYN"你的雙手武器太重了！"NOR"\n\r",ch);
					break;
			}
		}
	}

	bonus_chance = 100;
	if( !IS_NPC(ch) )
	{
		if( wield ){
			// lance mastery 及 tricky pacing 的加成 2022/12/03
			if( nLancePerc > 0 && IsUsingKnightWeapon(ch , wield) )
				bonus_chance = GetBonusedValue_LanceMaster(ch, 100);
			if( nTrickyPerc > 0 && IsUsingTrickyWeapon(ch , wield) )
				bonus_chance = TrickyPacingBase(ch, 100);

			// str 與武器重量 影響攻擊次數的機率 2022/12/11
			if( str_diff < 0 ){
				str_diff = UMAX( -4 , str_diff ) * 15; // 最低 -60%
			}else{
				str_diff = UMIN( 15 , str_diff ) * 2; // 最高 30%
			}
			bonus_chance += str_diff;
		}
	}

	if( number_percent( ) < (chance1 * bonus_chance)/100 )
	{
		++totalhit;
		// 針對流星箭會變為流星斬的問題作修正 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// 如果有箭則把 dt 從 gsn_meteoric_arrow 轉換為 TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
				&& arrow->item_type == ITEM_ARROW
				&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}
		//dam = brief_mone_hit( ch, victim, &mydt, WEAR_WIELD );
		dam = mone_hit ( ch, victim, &mydt, WEAR_WIELD, MSG_BRIEF, DT_CHANGE );

		if( dam >= 1 && dt == gsn_circle)
			dam = dam * weakness_damage(victim,dt)/100;

		if(dam >= 0) landedhit++;
		if( dam >= 1 && mydt > 0)
		{
			totaldam += dam;
			sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", dt, totaldam);
			if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
				send_to_char(ibuf, ch);

			if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
				send_to_char(ibuf, victim);
		}

		if( dam == -1 && !is_brief(ch, victim) ) 
		{
			dam_message(ch, victim, -1, mydt, WEAR_WIELD);
		}

		if( !ch->fighting 
			|| !ch->in_room 
			|| ch->in_room != victim->in_room
			|| dt == gsn_backstab 
			|| dt == gsn_circle )
		{
			if( is_brief(ch, victim))
				brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
	if( HitSleepCharm(ch,victim) ) return;

	// 判定 主要武器 或 雙手武器 是否為槍械射擊 2022/05/05
	if( get_eq_char( ch, WEAR_WIELD ) ){
		if( IsUsingGunShot( ch , get_eq_char( ch, WEAR_WIELD ) ) )
			gun_chance = 40;
	}else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
		if( IsUsingGunShot( ch , get_eq_char( ch, WEAR_TWO_HAND ) ) )
			gun_chance = 40;
	}

	// 槍械射擊 降低攻擊次數出現率 2022/05/05
	if( gun_chance + number_percent( ) < (chance2 * bonus_chance)/100 )
	{
		++totalhit;
		// 針對流星箭會變為流星斬的問題作修正 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// 如果有箭則把 dt 從 gsn_meteoric_arrow 轉換為 TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}
		//dam=brief_mone_hit( ch, victim, &mydt, WEAR_WIELD );
		dam = mone_hit ( ch, victim, &mydt, WEAR_WIELD, MSG_BRIEF, DT_CHANGE );

		if(dam >= 0) landedhit++;
		if( dam >= 1 && mydt > 0)
		{
			totaldam += dam;
			sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", dt, totaldam);
			if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
				send_to_char(ibuf, ch);

			if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
				send_to_char(ibuf, victim);
		}

		if( dam == -1 && !is_brief(ch, victim) ) 
		{
			dam_message(ch, victim, -1, mydt, WEAR_WIELD);
		}

		if( !ch->fighting 
			|| !ch->in_room 
			|| ch->in_room != victim->in_room 
			|| dt == gsn_backstab 
			|| dt == gsn_circle )
		{
			if( is_brief(ch, victim))
				brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
	if( HitSleepCharm(ch,victim) ) return;

	// 槍械射擊 降低攻擊次數出現率 2022/05/05
	if( gun_chance + number_percent( ) < (chance3 * bonus_chance)/100 )
	{
		++totalhit;
		// 針對流星箭會變為流星斬的問題作修正 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// 如果有箭則把 dt 從 gsn_meteoric_arrow 轉換為 TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}
		//dam = brief_mone_hit( ch, victim, &mydt, WEAR_WIELD );
		dam = mone_hit ( ch, victim, &mydt, WEAR_WIELD, MSG_BRIEF, DT_CHANGE );

		if(dam >= 0) landedhit++;
		if( dam >= 1 && mydt > 0)
		{
			totaldam += dam;
			sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", dt, totaldam);
			if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
				send_to_char(ibuf, ch);

			if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
				send_to_char(ibuf, victim);
		}

		if( dam == -1 && !is_brief(ch, victim) ) 
		{
			dam_message(ch, victim, -1, mydt, WEAR_WIELD);
		}

		if( !ch->fighting || !ch->in_room ||ch->in_room != victim->in_room )
		{
			if( is_brief(ch, victim))
				brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
	if( HitSleepCharm(ch,victim) ) return;

	// 槍械射擊 降低攻擊次數出現率 2022/05/05
	if( gun_chance + number_percent( ) < (chance4 * bonus_chance)/100 )
	{
		++totalhit;
		// 針對流星箭會變為流星斬的問題作修正 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// 如果有箭則把 dt 從 gsn_meteoric_arrow 轉換為 TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}
		//dam = brief_mone_hit( ch, victim, &mydt, WEAR_WIELD );
		dam = mone_hit ( ch, victim, &mydt, WEAR_WIELD, MSG_BRIEF, DT_CHANGE );

		if( dam >= 0 && mydt > 0)
		{
			++landedhit;
			totaldam += dam;
		}

		if( dam == -1 && !is_brief(ch, victim) ) 
		{
			dam_message(ch, victim, -1, mydt, WEAR_WIELD);
		}

		if( !ch->fighting 
			|| !ch->in_room 
			|| ch->in_room != victim->in_room
			|| dt == gsn_backstab 
			|| dt == gsn_circle )
		{
			if( is_brief(ch, victim))
				brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
	if( HitSleepCharm(ch,victim) ) return;

	wield = NULL;

	// dual 有穿 次要武器的情況
	if( ( wield = get_eq_char( ch, WEAR_WIELD_2 ) ) 
		&& wield->item_type == ITEM_WEAPON
		&& ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_DUAL ) ) 
		||( !IS_NPC( ch ) && get_skill_percent(ch, gsn_dual))))
	{
		// 修改 dual 技能百分比與等級影響, 並針對不同 chance 作不同觸發率 2022/05/03
		int nDualPerc = get_skill_percent(ch, gsn_dual) + get_skill_level(ch, gsn_dual) * 7;

		dt = TYPE_HIT + wield->value[3];

		mydt2 = dt;

		++totalhit;

		// dual 的第一下一定會出 2022/05/03
		//dam = brief_mone_hit( ch, victim, &mydt2, WEAR_WIELD_2 );
		dam = mone_hit ( ch, victim, &mydt2, WEAR_WIELD_2, MSG_BRIEF, DT_CHANGE );

		// pc 的 weapon weight 判定 次要武器 是否太重, 給予敘述 2022/12/06
		if( wield && !IS_NPC(ch) )
		{
			if( IS_SET( ch->act, PLR_NOTIFY ) ) // 接受系統訊息才會顯示
			{
				weight = get_obj_weight( wield );

				for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
					if( str_app[min_str].wield2 >= weight ){
						break;
					}
				}
				str_diff = get_curr_str ( ch ) - min_str;
				if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
				if( str_diff < -5 ){
					if( wield->wear_loc == WEAR_WIELD_2 )
						send_to_char(""CYN"你的次要武器太重了！"NOR"\n\r",ch);
				}
			}
		}

		if(dam >= 0) landedhit++;
		if( dam >= 1 && mydt2 > 0)
		{
			totaldam += dam;
			sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", mydt2, totaldam);
			if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
				send_to_char(ibuf, ch);

			if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
				send_to_char(ibuf, victim);
		}

		if( dam == -1 && !is_brief(ch, victim) ) 
		{
			dam_message(ch, victim, -1, mydt2, WEAR_WIELD_2);
		}

		if ( !ch->fighting 
			|| ch->fighting != victim 
			|| !ch->in_room 
			|| ch->in_room != victim->in_room
			|| dt == gsn_backstab 
			|| dt == gsn_circle )
		{
			if( is_brief(ch, victim))
				brief_dam_message(ch, victim, totaldam, mydt2, WEAR_WIELD_2,totalhit, landedhit);
			// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}

		// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
		if( HitSleepCharm(ch,victim) ) return;

		bonus_chance = 100;
		if( !IS_NPC(ch) )
		{
			if( wield ){
				// lance mastery 及 tricky pacing 的加成 2022/12/03
				if( nLancePerc > 0 && IsUsingKnightWeapon(ch , wield) )
					bonus_chance = GetBonusedValue_LanceMaster(ch, 100);
				if( nTrickyPerc > 0 && IsUsingTrickyWeapon(ch , wield) )
					bonus_chance = TrickyPacingBase(ch, 100);

				// str 與武器重量 影響攻擊次數的機率 2022/12/11
				if( str_diff < 0 ){
					str_diff = UMAX( -4 , str_diff ) * 15; // 最低 -60%
				}else{
					str_diff = UMIN( 15 , str_diff ) * 2; // 最高 30%
				}
				bonus_chance += str_diff;
			}
		}

		// dual 的 chance1 機率為 nDualPerc
		if( number_percent( ) < (chance1 * bonus_chance)/100 && number_percent() <= nDualPerc)
		{
			++totalhit;
			//dam = brief_mone_hit( ch, victim, &mydt2, WEAR_WIELD_2 );
			dam = mone_hit ( ch, victim, &mydt2, WEAR_WIELD_2, MSG_BRIEF, DT_CHANGE );

			if(dam >= 0) landedhit++;
			if( dam >= 1 && mydt2 > 0)
			{
				totaldam += dam;
				sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", mydt2, totaldam);
				if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
					send_to_char(ibuf, ch);

				if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
					send_to_char(ibuf, victim);
			}

			if( dam == -1 && !is_brief(ch, victim) ) 
			{
				dam_message(ch, victim, -1, mydt2, WEAR_WIELD_2);
			}

			if( !ch->fighting 
				|| ch->fighting != victim 
				|| !ch->in_room 
				|| ch->in_room != victim->in_room
				|| dt == gsn_backstab 
				|| dt == gsn_circle )
			{
				if( is_brief(ch, victim))
					brief_dam_message(ch, victim, totaldam, mydt2, WEAR_WIELD_2,totalhit, landedhit);
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}

		// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
		if( HitSleepCharm(ch,victim) ) return;

		// 判定 次要武器 是否為槍械射擊 2022/05/05
		gun_chance = 0;
		if( IsUsingGunShot( ch , wield ) )
			gun_chance = 40;

		// 槍械射擊 降低攻擊次數出現率 2022/05/05
		// dual 的 chance2 機率為 nDualPerc - 10
		if( gun_chance + number_percent( ) < (chance2 * bonus_chance)/100 && number_percent() <= nDualPerc - 10)
		{
			++totalhit;
			//dam = brief_mone_hit( ch, victim, &mydt2, WEAR_WIELD_2 );
			dam = mone_hit ( ch, victim, &mydt2, WEAR_WIELD_2, MSG_BRIEF, DT_CHANGE );
			if(dam >= 0) landedhit++;

			if( dam >= 1 && mydt2 > 0)
			{
				totaldam += dam;
				sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", mydt2, totaldam);
				if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
					send_to_char(ibuf, ch);

				if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
					send_to_char(ibuf, victim);
			}

			if( dam == -1 && !is_brief(ch, victim) ) 
			{
				dam_message(ch, victim, -1, mydt2, WEAR_WIELD_2);
			}

			if( !ch->fighting 
				|| ch->fighting != victim 
				|| !ch->in_room 
				|| ch->in_room != victim->in_room 
				|| dt == gsn_backstab 
				|| dt == gsn_circle )
			{
				if( is_brief(ch, victim))
					brief_dam_message(ch, victim, totaldam, mydt2, WEAR_WIELD_2,totalhit, landedhit);
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}

		// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
		if( HitSleepCharm(ch,victim) ) return;

		// 槍械射擊 降低攻擊次數出現率 2022/05/05
		// dual 的 chance3 機率為 nDualPerc - 20
		if( gun_chance + number_percent( ) < (chance3 * bonus_chance)/100 && number_percent() <= nDualPerc - 20)
		{
			++totalhit;
			//dam = brief_mone_hit( ch, victim, &mydt2, WEAR_WIELD_2 );
			dam = mone_hit ( ch, victim, &mydt2, WEAR_WIELD_2, MSG_BRIEF, DT_CHANGE );
			if(dam >= 0) landedhit++;

			if( dam >= 1 && mydt2 > 0)
			{
				totaldam += dam;
				sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", mydt2, totaldam);
				if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
					send_to_char(ibuf, ch);

				if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
					send_to_char(ibuf, victim);
			}

			if( dam == -1 && !is_brief(ch, victim) ) 
			{
				dam_message(ch, victim, -1, mydt2, WEAR_WIELD_2);
			}

			if( !ch->fighting 
				|| ch->fighting != victim 
				|| !ch->in_room 
				|| ch->in_room != victim->in_room
				|| dt == gsn_backstab 
				|| dt == gsn_circle )
			{
				if( is_brief(ch, victim))
					brief_dam_message(ch, victim, totaldam, mydt2, WEAR_WIELD_2,totalhit, landedhit);
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}

		// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
		if( HitSleepCharm(ch,victim) ) return;

		// 槍械射擊 降低攻擊次數出現率 2022/05/05
		// dual 的 chance4 機率為 nDualPerc - 30
		if( gun_chance + number_percent( ) < (chance4 * bonus_chance)/100 && number_percent() <= nDualPerc - 30 )
		{
			++totalhit;
			//dam = brief_mone_hit( ch, victim, &mydt2, WEAR_WIELD_2 );
			dam = mone_hit ( ch, victim, &mydt2, WEAR_WIELD_2, MSG_BRIEF, DT_CHANGE );
			if(dam >= 0) landedhit++;

			if( dam >= 1 && mydt2 > 0)
			{
				totaldam += dam;
				sprintf(ibuf, "(dt="HIC"%d"NOR", totaldam="HIR"%d"NOR")\n\r", mydt2, totaldam);
				if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
					send_to_char(ibuf, ch);

				if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
					send_to_char(ibuf, victim);
			}

			if( dam == -1 && !is_brief(ch, victim) ) 
			{
				dam_message(ch, victim, -1, mydt2, WEAR_WIELD_2);
			}

			if( !ch->fighting 
				|| ch->fighting != victim 
				|| !ch->in_room 
				|| ch->in_room != victim->in_room
				|| dt == gsn_backstab 
				|| dt == gsn_circle )
			{
				if( is_brief(ch, victim))
					brief_dam_message(ch, victim, totaldam, mydt2, WEAR_WIELD_2,totalhit, landedhit);
				// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}
	}// dual 判定結束

	// 如果 ch 或 victim 被 sleep , charm 則停止攻擊
	if( HitSleepCharm(ch,victim) ) return;

	// 判定 tactic 的 trip 及 disarm
    if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_tactics] > 0)
    {
        int chance;
  			int nTacPerc = get_skill_percent(ch, gsn_tactics);
  			int nTacSklv = get_skill_level(ch, gsn_tactics);
  			int nCurInt  = get_curr_int( ch );
  			chance = (14 + nTacSklv * nTacSklv) * nTacPerc * (nCurInt + 12) / 4000;
        //chance = ( ( UMAX( 0, ( get_curr_int( ch ) - 13 ) ) << 2 ) )
        //    * get_skill_percent(ch, gsn_tactics) / 40 + get_skill_level(ch, gsn_tactics) ;
            
        if( number_percent() > chance )
        {
            if(!IS_NPC( ch ) || number_percent() > 50){
                if( number_bits(1) == 1 )
                    trip(ch, victim);
                else if( wield )
                    disarm(ch, victim);
            }
        }
    }

	// 判定 lizardman 特技 , 如果 dt 為流星箭則不觸發 lizardman 特技 2022/05/02
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) && dt != gsn_meteoric_arrow )
	{
		int nSklv = get_skill_level(ch, gsn_inborn_ability);
		if( !victim->deleted ){
			//  噴酸攻擊
			if( ch->pcdata->condition[COND_DRUNK] > 10 
				&& number_percent() < UMAX(40, ch->pcdata->condition[COND_DRUNK] * 2) + nSklv * (nSklv + 4) / 2){
				AFFECT_DATA af;
				int nSVE        = NEW_GET_SVS(victim, MAGIC_EARTH);
				int nPoisonRate = 0; if(nSklv > 3) nPoisonRate =   5 * (nSklv - 3);
				int nBlindRate  = 0; if(nSklv > 5) nBlindRate  =   7;
				int nDamage     = dice( 4 + nSklv, ch->level + nSklv) + ch->amdr * nSklv / 5;

				nPoisonRate = nPoisonRate * (100 + nSVE) / 100; if(nPoisonRate < 0) nPoisonRate = 0;
				nBlindRate  = nBlindRate  * (100 + nSVE) / 100; if(nBlindRate  < 0) nBlindRate  = 0;
				nDamage     = nDamage     * (100 + nSVE) / 100; 
				nDamage    -= victim->mdr;
				if(nDamage  < 0) nDamage     = 0;

				// 命中判定修改為 weakness_percent 2020/09/18
				//nPoisonRate = nPoisonRate *  weakness_damage(victim, gsn_poison) / 100;
				//nBlindRate  = nBlindRate  *  weakness_damage(victim, gsn_blindness) / 100;
				nPoisonRate = nPoisonRate *  weakness_percent(victim, gsn_poison) / 100;
				nBlindRate  = nBlindRate  *  weakness_percent(victim, gsn_blindness) / 100;

				if(number_percent() < nPoisonRate && !IS_SET(victim->special, NO_POISON)){
					// 天賦等級四以上、有機會讓敵方中毒
					af.type      = gsn_poison;
					af.duration  = UMAX( 0, 3 + nSklv - get_curr_con(victim) / 5);
					af.bitvector = AFF_POISON;
					if( affected_value(victim, gsn_poison, APPLY_CON) > -8){
						af.location = APPLY_CON;
						af.modifier = -2 * weakness_damage( victim, gsn_poison ) * 0.01;
						affect_join( victim, &af );
					}

					if( affected_value(victim, gsn_poison, APPLY_SAVING_POISON) < (14 + nSklv * 6) * 10){
						af.location = APPLY_SAVING_POISON;
						af.modifier = ((14 + nSklv * 6) * 25 / get_curr_con(victim)) 
										* weakness_damage( victim, gsn_poison ) * 0.01;
						affect_join( victim, &af );
					}
					act( "$N"HIG"臉色略為泛青，毒素已經侵入了他的體內！"NOR"", ch, NULL, victim, TO_CHAR );
					act( HIG "噁心感與刺痛感同時襲來，你覺得相當地難受！"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$N"HIG"的身體顫抖了一下，表情變得有些苦澀！"NOR"", ch, NULL, victim, TO_NOTVICT );
				}

				if(number_percent() < nBlindRate && !IS_AFFECTED( victim, AFF_BLIND ) 
				   && !IS_SET( victim->special, NO_BLIND )
				   && !(is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 2) ){
					// 天賦等級六以上、有機會讓敵人盲目
					af.type      = gsn_blindness;
					af.duration  = 0;
					af.location  = APPLY_HITROLL;
					af.modifier  = (-10) * weakness_damage( victim, gsn_blindness ) * 0.01;
					af.bitvector = AFF_BLIND;
					affect_to_char( victim, &af );

					act( "你"HIG"的酸液傷到了$N"HIG"的眼睛！使得他失明了！"NOR"", ch, NULL, victim, TO_CHAR );
					act( "$n"HIG"的酸液傷到了你"HIG"的眼睛！你什麼都看不到了！"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$n"HIG"的酸液傷到了$N"HIG"的眼睛！使得他失明了！"NOR"", ch, NULL, victim, TO_NOTVICT );
				}
				damage( ch, victim, nDamage, gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			}
		}
        
		if( !victim->deleted )
		{   // 掃尾攻擊
			CHAR_DATA *vch      = NULL;
			CHAR_DATA *vch_next = NULL;
			int nTailAttackRate = 30 + nSklv * ( 4 + nSklv);
			int nTripRate       = get_curr_dex(ch) / 2 + nSklv * 3;
			if( number_percent() < nTailAttackRate)
			{
				if(mone_hit ( ch, victim, &gsn_tactics, -2, MSG_NORMAL, DT_NOCHANGE ) >= 0
					&& !victim->deleted && number_percent() < nTripRate)
				{
					trip(ch, victim);
				}
			}
			if(nSklv > 3){
				for( vch = ch->in_room->people; vch; vch = vch_next)
				{
					vch_next = vch->next_in_room;
					if( vch->deleted ) continue;
					if( vch->fighting != ch ) continue;
					if( vch == victim) continue;
					if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
					if( number_percent() < (nTailAttackRate / 2) )
					{
						if(mone_hit ( ch, vch, &gsn_tactics, -2, MSG_NORMAL, DT_NOCHANGE ) >= 0
							&& !vch->deleted && number_percent() < nTripRate)
						{
							trip(ch, vch);
						}
					}
				}
			}
		}
	}// lizardman 特技判定結束

    if( is_brief(ch, victim))
        brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
	// npc 開戰的第一回合會觸發 fight_prog 22022/10/31
	if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
	{
		if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
		if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
	}
    return;
}


int get_critical_damage(int dam, CHAR_DATA *ch, CHAR_DATA *victim, int dexdiff, bool fCritical, int dt, int darkraid)
{
    int chance = 0;
    int nSklv  = get_skill_level(ch, gsn_critical);
	int nAcSklv = get_skill_level(ch, gsn_deadly_strike); // 2022/07/14
    if(( !IS_NPC( ch )    // PPL case
			&& ( fCritical || get_skill_percent(ch, gsn_critical) + nSklv * 4 > number_percent() ) )
			//&& ( fCritical || ch->pcdata->learned[gsn_critical] + nSklv * 4 > number_percent() ) )
		|| ( ( IS_NPC( ch )  // NPC case
				&& ( IS_SET( ch->act, ACT_CRITICAL ) || IS_SET( ch->class, 4 ) )
				&& ( dt != 1014) ) // arrow
			&& ( chance = UMIN( 30, ch->level >> 1 ) ) ) )
    {
		// critical base 機率
		chance = get_skill_percent(ch, gsn_critical)/10 + nSklv * (nSklv + 14) / 4;

		// 修改為 darkraid 只判定作用 , 不判定是否為 pc 及 攻擊模式 2020/09/16
        //if ( dt == gsn_backstab && !IS_NPC(ch) && darkraid )
		if ( darkraid )
			chance += 4 + get_skill_percent(ch, gsn_dark_raid) / 9 + get_skill_level(ch, gsn_dark_raid) * get_skill_level(ch, gsn_dark_raid);

		// autoc critical 判定  2022/07/14
		if( fCritical && ( number_percent() < ( nAcSklv * 13 + get_skill_percent( ch, gsn_deadly_strike)/ 3 ) ) )
			// deadly strike 等級影響 critical 出現率的倍數 2022/07/14
			chance = nAcSklv + ( chance * ( 6 + nAcSklv ) )/5;

        if ( (chance + dexdiff > number_percent( ) ) )
        {
            // Original:  from x2 to x3
            // dam = number_range(1, 2) * dam + dam; 

            // old version , based on Sklv  sklv 0 from x1 to x1.8 , sklv 6 from x1.6 to x3
			// new version 2020/09/16 , sklv 0 from x1.1 to x1.2 , sklv 6 from sklv x1.7 to x2.4
			if (!IS_NPC( ch )){
				//dam = dam * number_range(10 + nSklv, 18 + nSklv * nSklv / 3) / 10;
				dam = dam * number_range(11 + nSklv , 12 + nSklv * nSklv / 3) / 10;
			}else{
				dam = dam * number_range(12 + ch->level/10, 14 + ch->level/10)/10;
			}
            if( !is_chinese(ch, victim))
            {
                act( "You hit $N's weakest point!!!", ch, NULL, victim, TO_CHAR );
                act( "Ouch! $n hit your weakest point!!!",ch, NULL, victim, TO_VICT );
                act( "$n hit $N's weakest point!!!",ch, NULL, victim, TO_NOTVICT );
            }
            else
            {
                act( "你運用技巧試著向 $N 的弱點突擊!!!", ch, NULL, victim, TO_CHAR );
                act( "$n 對準了你的弱點致命一擊!!!",ch, NULL, victim, TO_VICT );
                act( "$n 試著向 $N 的弱點進行致命一擊!!!",ch, NULL, victim, TO_NOTVICT );
            }
        }
	}
    return dam;
}

void check_reload_ammo( CHAR_DATA *ch , OBJ_DATA *wield )
{
	OBJ_DATA *ammo = NULL;
	OBJ_DATA *reload = NULL;
	OBJ_DATA *reload_next = NULL;
	int HasReload;

	// 加入武器形態判定 2023/01/05
	if( !wield || wield == NULL )
		return;

	if( wield->value[3] != 16 )
		return;

	if( ch->move < 350 - get_skill_level(ch, gsn_firearm_mastery)*20 )
		return;

	ammo = get_eq_char( ch, WEAR_AMMO );

	// 有裝備 ammo , 沒有裝備 ammo 則直接找身上有無彈藥
	if( ammo ){
		// ammo 位置是彈藥且數量大於 0 或無限彈藥(-1) 則不動作 2023/01/05
		if( ammo->item_type == ITEM_AMMO && ( ammo->value[0] > 0 || ammo->value[0] == -1 ) )
			return;
		// 如果 彈藥 的裝備是 no remove 的情況 則不動作
		if( IS_SET( ammo->extra_flags, ITEM_NOREMOVE ) )
			return;
	}

	HasReload = 0;
	
	// 尋找身上可用的彈藥
	for( reload = ch->carrying; reload; reload = reload_next )
	{
		reload_next = reload->next_content;
		if( reload->wear_loc == WEAR_NONE                          // reload 必須在身上
			&& reload->item_type == ITEM_AMMO                      // reload 必須為彈藥
			&& CAN_WEAR(reload, ITEM_WEAR_AMMO)                    // reload 要能被穿在 ammo 的位置
			&& ( reload->value[0] > 0 || reload->value[0] == -1 )  // 彈藥數必須大於 0, 或是 -1 ( 無限多 )
			&& can_see_obj( ch, reload )                           // 要看得見 reload
			&& ch->level >= reload->level )                        // ch 等級要在 obj 以上
		{
			HasReload++;
			break;
		}
	}


	// 就算沒有可以 reload 的情況下, 跑到這如果有裝備 ammo 則 v0 必定小於 0 但不為 -1 , 則判定數量是否脫掉彈藥 2023/01/04
	if( ammo )
		ShotObjCount( ch , ammo );

	// 沒有找到可用的彈藥
	if( HasReload == 0 )
		return;

	/*
		// 有裝備在彈藥則脫掉該裝備 ( 該位置裝備type 不為 ammo, 或 ammo v0 <= 0 )
		if( ammo )
			unequip_char( ch, ammo );
	*/

	// 裝備找到的彈藥
	if( reload ){
		equip_char( ch, reload, WEAR_AMMO );
		ch->move = ch->move - (350 - get_skill_level(ch, gsn_firearm_mastery)*20);
		act( "你快速裝備起 $p.", ch, reload, NULL, TO_CHAR );
		act( "$n 快速裝備起 $p.", ch, reload, NULL, TO_ROOM );
	}

	return;
}


/*
 * 加入中文註解 by Razgriz 20051012
 * 介面改寫 by Razgriz 20051014
 */
int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode )
{
    OBJ_DATA  *wield = NULL;
    OBJ_DATA  *bow = NULL;
    OBJ_DATA  *arrow = NULL;
	OBJ_DATA  *gun = NULL;   //2022/04/30 槍械
    OBJ_DATA  *ammo = NULL;   //2022/04/30 彈藥
	OBJ_DATA  *gloves = NULL; //2020/01/20 gloves
    CHAR_DATA *vch;
    char       buf [ MAX_STRING_LENGTH ];
    int        victim_dodge = 0;
    int        thac0;
    int        thac0_00;
    int        thac0_42;
    int        chance = 0;
    int        dam=-2;
    int        fist_dam = 4;
    int        min_str, weight, str_diff;
    int        diceroll;
    int        place,i;
    OBJ_DATA  *armor = NULL;
    int        race_hit_bonus;
    int        fCritical = FALSE;
    int        darkraid = 0;
    //    int leveldiff = ch->level - victim->level;
    int        dexdiff = get_curr_dex( ch ) - get_curr_dex( victim );
    int        dt = *dt_orig; //Added by Razgriz 20051014
	int        nAcSklv = get_skill_level(ch, gsn_deadly_strike); // 2022/07/14
    bool       isLizTail = FALSE;
    bool       isReposte = FALSE;
    bool       isPowerBlow = FALSE; // 統一 powerblow 的判定 2021/12/25
	bool       isBloodThirsty = FALSE; // 主動 blood thirsty 判定 2021/12/25
	bool       isShootCost = FALSE; // 2022/12/07
	int        nLmPerc = get_skill_percent(ch, gsn_lance_mastery);
	int        nLmSklv = get_skill_level(ch, gsn_lance_mastery);
	int        nTpPerc = get_skill_percent(ch, gsn_tricky_pacing);
	int        nTpSklv = get_skill_level(ch, gsn_tricky_pacing);

    if(dt == gsn_tactics) isLizTail = TRUE;

	// 主動 blood thirsty 判定 2021/12/25
	if( !IS_NPC(ch)
		&& ch->class == MARTIALMAN
		&& get_skill_percent(ch, gsn_blood_thirsty) > 0
		&& is_affected( ch, gsn_blood_thirsty)
		&& affected_value(ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 )
	{
		isBloodThirsty = TRUE;
	}

	// 加入 hit <= 0 時也會 return 的判定 2022/12/30
    if(!check_can_do_hit(ch,victim) || victim->position == POS_DEAD || victim->hit <= 0 ){
        if(dt != gsn_reposte || get_skill_level(ch, gsn_reposte) < 5)
            return -2;
    }

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD
		|| victim->hit <= 0  // 加入 hit <= 0 時也會 return 的判定 2022/12/30
		|| !ch->in_room
		|| ch->in_room != victim->in_room ){
        sprintf( buf, "one_hit: ch %s not with victim %s, or victim POS_DEAD",ch->name, victim->name );
        bug( buf, 0 );
        return -2;
    }

    // 處理 autoc critical , move 消耗受技能等級影響 2022/07/14
    if( !IS_NPC(ch)
		&& IS_SET( ch->pcdata->autocombat, AUTOC_CRITICAL)
		&& (ch->move > 80 - get_skill_level(ch, gsn_deadly_strike) * 5 ) )
	{
		/* 	autoc critical 判定移置 get_critical_damage 內, 並移除普擊不出現的設定 2022/07/14

        if ( number_percent() > (   get_skill_level(ch, gsn_deadly_strike) * 2
                                  + get_skill_percent( ch, gsn_deadly_strike) / 3 ) ){
            return -2;
        }
        chance += get_skill_percent( ch, gsn_deadly_strike ) / 3 + get_skill_level(ch, gsn_deadly_strike); */

		// fCritical 必成立, 代表可略過 critical skill percent 的初判定 2022/07/14
        fCritical = TRUE;

        ch->move = UMAX( 0 , ch->move - 80 + get_skill_level(ch, gsn_deadly_strike) * 5 );
    }

	//處理 精準射擊 trueshot
	if ( !IS_NPC( ch ) && is_affected(ch,gsn_trueshot)){
		if ( ch->move > 10 )
			ch->move -= 10;
		else{
			send_to_char("你的體力不夠維持精準射擊的狀態.\n\r",ch);
			affect_strip(ch,gsn_trueshot);
		}
	}

    /*
     * Figure out the type of damage message.
     */

    //根據傳進來的 wpn 裝備位置(幾乎都是 WEAR_WIELD, 還沒看過不是的 orz), 找出 ch 的 weapon
	// wpn 基本上以 wear_wield 或 wear_wield_2 進入, 不會以 wear_two_hand 進入
    wield = get_eq_char( ch, wpn );

	// wpn 如果以 wear_two_hand 進入, 則此處 wield 會為 NULL
	if(!wield){
		if ( ( wield = get_eq_char( ch, WEAR_TWO_HAND ) ) ) { // 雙手武器
			wpn = WEAR_TWO_HAND;

			// 判斷是否為射箭，成立則消耗箭矢數量
			if ( wield->value[3] == 14 ){ // 武器攻擊型態為 射出箭矢 shot
				vch = NULL;
				if ( !IS_NPC(ch) )
				{
					int nRideSklv = get_skill_level(ch, gsn_ride);
					//int nTrickySklv = get_skill_level(ch, gsn_tricky_pacing);
					if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
						if(nTpSklv < 1 ){
							if(nRideSklv < 2 || !get_eq_char( ch , WEAR_RIDE )){
								//看看 ch (pc) 是否處於被打狀態
								for( vch = ch->in_room->people; vch; vch = vch->next_in_room ){
									if ( !vch->deleted && vch->fighting == ch && !IS_NPC( ch ) )
										break;
								}
							}
						}
					}
				}

				//ch 沒被打, 且還有箭, 處理箭隻消耗
				if ( !vch
					&& ( arrow = get_eq_char( ch, WEAR_ARROW ) )
					&& arrow->item_type == ITEM_ARROW
					&& arrow->value[0] > 0 )
				{
					isShootCost = TRUE;
					// 扣除 arrow 數量移到 return 前判定 2023/01/04
					//arrow->value[0] -= ( IS_NPC( ch ) && !ch->clan && !IS_AFFECTED(ch, AFF_CHARM) && !IS_AFFECTED( ch, AFF_SUMMONED)) ? 0 : 1;
				}
				else
				{
					/* 箭數為 -1 的情況 , 箭矢無限制 2023/01/05
						if( arrow
							&& arrow->value[0] == -1 )
							&& !IS_NPC(ch)
							&& ch->mana > 1
							&& arrow->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW ) */
					if( arrow && arrow->value[0] == -1 )
					{
						// 但使用魔法箭的箭數為 -1 會變為判定 mana 每箭至少需要 1 mana 2023/01/05
						if( arrow->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW )
						{
							// 箭數為 -1 , 則一箭消耗 1 mana
							if(ch->mana >= 1){
								ch->mana -= 1;
							}else{ // mana 不夠則轉換成 揮舞劈砍 slash
								arrow = NULL;
								dt = 1003;
								if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
							}
						}
					}
					else
					{ //  ch 被打, ch 攻擊型態轉為 揮舞劈砍 slash
						arrow = NULL;
						dt = 1003;
						if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
					}
				}
			}
		}
	}else{ // 處理當意外把 bow 設為單手(wield) 的情況 2022/12/06
		if ( wield->value[3] == 14 ){
			dt = 1003;
			if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
		}
	}

    //當攻擊型態為 射出箭矢 shot, 但是沒箭或沒武器或不是雙手武器時, 攻擊型態轉為 揮舞劈砍 slash
    if (dt == 1014 && (!arrow || !wield || wield->wear_loc != WEAR_TWO_HAND) ){
        dt = 1003;
        if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
    }

    //間隙反擊, 把攻擊型態換成武器的攻擊型態, 之後 reposte 加成全看 isReposte 是否成立 2022/05/07
	if ( dt == gsn_reposte ){
		dt = TYPE_HIT;
		isReposte = TRUE;
		if ( wield && wield->item_type == ITEM_WEAPON ){
			dt += wield->value[3];
		}
		if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
	}

    //當攻擊型態未定義時(就是普擊), 將由武器的攻擊型態來決定
    //可參考 fight_const.h, TYPE_HIT 的意義可看成 奮力擊打 hit
    if ( dt == TYPE_UNDEFINED ){
        dt = TYPE_HIT;
        if ( wield && wield->item_type == ITEM_WEAPON )
            dt += wield->value[3];
        if ( dt_mode == DT_CHANGE )
            *dt_orig = dt;
    }

	// autoc reload 在每次攻擊中會先確認彈藥, 並加入武器判定 2023/01/05
	if( !IS_NPC(ch) && wield ){
		if( ch->pcdata->autocombat == AUTOC_RELOAD )
			check_reload_ammo( ch , wield );
	}

    //當攻擊型態為 槍械射擊 ammo 時 , 但沒子彈, 攻擊型態轉為 萬鈞力搥 pound 2022/04/30
    if( dt == 1016 ){
		/* 移到外部作判定 2023/01/05
		if( !IS_NPC(ch) ){
			// autoc reload 在每次攻擊中會先確認彈藥 2022/05/02
			if( ch->pcdata->autocombat == AUTOC_RELOAD )
				check_reload_ammo( ch );
		} */
		ammo = get_eq_char( ch, WEAR_AMMO );
		if( ammo
			&& ammo->item_type == ITEM_AMMO
			&& ( ammo->value[0] > 0 || ammo->value[0] == -1 ) ) // 加入 -1 的情況 2023/01/05
		{
			// -1 的情況下彈藥不減少 ( 無限 ) 2023/01/05
			if ( ammo->value[0] != -1 )
				isShootCost = TRUE;
			// 扣除 ammo 數量移到 return 前判定 2023/01/04
			//ammo->value[0] -= (IS_NPC( ch ) && !ch->clan && !IS_AFFECTED(ch, AFF_CHARM) && !IS_AFFECTED( ch, AFF_SUMMONED)) ? 0 : 1;
		}else{
			if( ammo )
				ammo = NULL;
			dt = 1007;
			if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
		}
    }

	if( wield && !IS_NPC(ch) ) // 加入 pc 限定 2022/12/04
	{
        weight = get_obj_weight( wield );

        int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
        int nPbSklv  = get_skill_level(ch, gsn_powerblow);

        switch( wield->wear_loc )
		{
			case WEAR_WIELD :
				for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
					if( str_app[min_str].wield >= weight ){
						break;
					}
				}
				str_diff = get_curr_str ( ch ) - min_str;
				if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
				if( str_diff < -5 ) return -2;
				break;
			case WEAR_WIELD_2 :
				for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
					if( str_app[min_str].wield2 >= weight ){
						break;
					}
				}
				str_diff = get_curr_str ( ch ) - min_str;
				if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
				if( str_diff < -5 ) return -2;
				break;
			case WEAR_TWO_HAND :
				for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
					if ( str_app[min_str].twohand >= weight )
						break;
					}
				str_diff = get_curr_str ( ch ) - min_str;
				if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
				if( str_diff < -5 ) return -2;
					break;
        }
    }

	/*
	 * Calculate to-hit-armor-class-0 versus armor.
	 */

    //做出影響命中率的一些先天因素
    //跟 interpolate(), class_table , thac0的公式 對照來看, 大概可看出以下原則
    //1. 讓轉職過的比未轉的有較好的命中率 (thac0_00, 越小越好)
    //2. 不同職業, 隨著等級提升, 造成命中率提升程度不同 (thac0_42 - thac0_00, 越小越好)

    if ( IS_NPC( ch ) ){
		//  mob部分
        if ( IS_SET( ch->class ,1 ) ){
            thac0_00 =  10;
            thac0_42 = -10;
        }else{
            thac0_00 =  20;
            thac0_42 =   0;
        }
    }else{
		//  玩家部分
		//  thac0_42 讓 dex 高時的 命中率提升 增加
		//  計算 thac0, 越小表示命中率越高
        thac0_00 = class_table[ch->class].thac0_00;
        thac0_42 = class_table[ch->class].thac0_42 - ( get_curr_dex( ch ) - 16 ) * 2;
    }

    //  讓種族體積大小的差距, 影響一點點命中率 , 我小他大則命中增加, 反之命中減少
	//race_hit_bonus = ( race_table[victim->race].size - race_table[ch->race].size );
	race_hit_bonus = get_curr_size( victim ) - get_curr_size( ch ); // 替換體型 size 公式 2022/12/23

    /* Weapon-specific hitroll and damroll */

    if( wield )
	{
        int whr = get_hitroll(ch, wpn);

		//  reposte 會隨 hitroll 及技能等級變動命中加成 2022/01/13
		//  reposte 判定改以 看 isReposte 的 bool 2022/05/07
		//if(dt == gsn_reposte && ch->hitroll > 0)
		if( isReposte && ch->hitroll > 0 )
			whr += ( ch->hitroll * ( 25 + get_skill_level(ch, gsn_reposte) * 20
										+ get_skill_level(ch, gsn_reposte) * get_skill_level(ch, gsn_reposte) ) ) / 100;

		//  Mar 外的frenzy , sklv 小於 6 hr 超過 60 之後, hr 的增加將會趨緩, 超過 1 hr 減 0.25 hr
		//  ex: whr 80 降到 75, whr 100 降到 90
        if( dt == gsn_frenzy && whr > 60) {
            //if(!IS_NPC(ch) && get_skill_level(ch, gsn_frenzy) < 6){
			if(!IS_NPC(ch) && ch->class != MARTIALMAN && get_skill_level(ch, gsn_frenzy) < 6){
                whr -= interpolate1( 60, 0, 100, 10, (whr- 60));
            }
        }


		//  tactics 影響微量命中
        if(!IS_NPC(ch) && get_skill_percent(ch, gsn_tactics)){
            int nTacPerc = get_skill_percent(ch, gsn_tactics);
            int nTacSklv = get_skill_level(ch, gsn_tactics);
            whr = whr * (20000 + nTacPerc * (8 + nTacSklv * (nTacSklv + 1)) ) / 20000;
        }

		// 加入 wp , pc 及 lance mastery 熟練度判定 2022/12/03
		//if( !IS_NPC(ch) && get_skill_percent(ch, gsn_lance_mastery) > 0 && IsUsingKnightWeapon(ch , wield) )
		if( !IS_NPC(ch) && nLmPerc > 0 && IsUsingKnightWeapon(ch , wield) )
			whr = GetBonusedValue_LanceMaster(ch, whr);

		//  給予背刺 sklv 與 dex 的基礎命中率補正 2022/11/11
		if( dt == gsn_backstab )
			whr += ( get_skill_level ( ch, gsn_backstab ) * get_curr_dex( ch ) )/21;

		//  背刺 sklv 影響命中率加成, Added by Razgriz 20051015
        if (     dt == gsn_backstab && !IS_NPC (ch)
             && (ch->class == ASSASSIN || ch->class == NINJA || ch->class == RANGER) ){
            switch ( get_skill_level ( ch, gsn_backstab ) ){
                case 6: whr += whr;          break;
                case 5: whr += whr / 10 * 7; break;
                case 4: whr += whr / 10 * 5; break;
                case 3: whr += whr / 10 * 3; break;
                case 2: whr += whr / 10 * 2; break;
                case 1: whr += whr / 10    ; break;
                default: break;
            }
            //有 夜襲 命中率再加成
            if ( ch->class == ASSASSIN && IS_AFFECTED ( ch, AFF_DARK_RAID ) ){
                switch ( get_skill_level ( ch, gsn_dark_raid ) ){
                    case 6: whr += whr;          break;
                    case 5: whr += whr / 10 * 7; break;
                    case 4: whr += whr / 10 * 5; break;
                    case 3: whr += whr / 10 * 3; break;
                    case 2: whr += whr / 10 * 2; break;
                    case 1: whr += whr / 10    ; break;
                    default: break;
                }
            }
        }

		// 新版箭術命中加成 -- 直接影響 whr 並一併列入武器計算
		if(dt == 1014 && arrow != NULL && !IS_NPC( ch )){
			int nSklvShot = get_skill_level(ch, gsn_shot_arrow);
			int nCurrDex  = get_curr_dex(ch);
			int nHrBonus  = get_skill_percent(ch, gsn_shot_arrow) + 1;

			// 增加 飛行武器 hr 效果加成 2022/05/06
			whr += ShootObjAffect( ch , arrow , 1 );

			// DEX(25), SHOT(6): 216 / 10 + 60 + 18  = 99%
			// DEX(25), SHOT(4):  64 / 10 + 32 + 12  = 50%
			// DEX(23), SHOT(6): 99% * 28 * 28 / 900 = 86%
			// DEX(23), SHOT(4): 50% * 28 * 28 / 900 = 43%
			// DEX(19), SHOT(6): 99% * 24 * 24 / 900 = 63%
			// DEX(19), SHOT(5): 72% * 24 * 24 / 900 = 46%
			// DEX(19), SHOT(4): 50% * 24 * 24 / 900 = 32%

			nHrBonus = (nSklvShot * nSklvShot * nSklvShot / 10 + nSklvShot * (nSklvShot + 4) + nSklvShot * 3)
						*  nHrBonus / 100 * ((nCurrDex + 5) * (nCurrDex + 5)) / 900;
			whr      = whr * (100 + nHrBonus) / 100;
		}

        // 槍械專家 對 槍械射擊的命中加成 2022/04/30
        if( dt == 1016 && ammo != NULL && !IS_NPC( ch ) ){
			// 槍械的武器殺傷力為精準度 , 當使用射擊時只會影響到命中 , 傷害只看彈藥 2022/05/01
			int nAccuray = number_range( wield->value[1], wield->value[2] );
			int nSklvFA = get_skill_level(ch, gsn_firearm_mastery);
			int nFABonus = 0;

			// 槍械的精準度為 avg 的 2 倍
			nAccuray = nAccuray * 2;

			if( get_skill_percent(ch, gsn_firearm_mastery) > 0 ){
				nFABonus = nSklvFA + nSklvFA * nSklvFA;
				nAccuray += 5;
			}
			
			// 增加 飛行武器 hr 效果加成 2022/05/06
			whr += ShootObjAffect( ch , ammo , 1 );
			
			nAccuray = nAccuray - 500/(10 + nFABonus);

			whr = whr * (100 + nAccuray) / 100;
			
			whr = whr + nFABonus;
        }

			//thac0 的計算公式, 這邊就是命中率的集大成了, 之後都以 thac0 來當做命中率了
			//大致說明
			//thac0 =   先天因素
			//        - 種族體積影響
			//        - 武器的神聖屬性影響
			//        - (武器命中+武器材質影響) * (武器種族命中加成)
			//        - 武器種族命中點數
			//        - 射箭影響 * (武器種族命中加成)  -- Modified by Keelar

			/* 舊板
			thac0 = interpolate( ch->level, thac0_00, thac0_42 )
					- (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield) / 100
					- get_vrace_hrq(victim, wield)
					- race_hit_bonus
					- ( IS_SET(wield->extra_flags, ITEM_HOLY) ?  (wield->level / 9) : 0); */
				
        thac0 = interpolate( ch->level, thac0_00, thac0_42 )
                - race_hit_bonus
                // 武器 flag 的 ITEM_HOLY 命中加成換為 ITEM_BLESS
				//- ( IS_SET(wield->extra_flags, ITEM_HOLY) ?  (wield->level / 9) : 0);
				- ( IS_SET(wield->extra_flags, ITEM_BLESS) ?  (wield->level / 5) : 0);
				
		if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 ){
			int nWmSklv   = get_skill_level(ch,gsn_weapon_master);
			int nCurrInt  = get_curr_int(ch);
		
			//  Weapon master 武器種族命中倍率放大
			if( get_vrace_hrp(victim, wield) > 100 ){
				thac0 -= (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield)
						  * (1 + nWmSklv * ( nWmSklv + 2 ) * nCurrInt /2000) / 100;
			}else{
				thac0 -= (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield) / 100;
			}

			//  Weapon master 武器種族命中加成放大
			if( get_vrace_hrq(victim, wield) > 0 ){
				thac0 -= get_vrace_hrq(victim, wield) * (1 + nWmSklv * ( nWmSklv + 4 ) * nCurrInt /2000);
			}else{
				thac0 -= get_vrace_hrq(victim, wield);
			}
		}else{
			thac0 = thac0 - (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield) / 100
					 - get_vrace_hrq(victim, wield);
		}
			
			/* 舊版箭術影響 *
			 * SKLV 6: 25 + 3 + 12 + 4 = 43
			 * SKLV 4: 25 + 3 + 8      = 36
			 * SKLV 0: 25 + 3          = 28
					  - ( (arrow != NULL) ? 
							(!IS_NPC( ch ) ? 
							  (   get_skill_percent( ch, gsn_shot_arrow ) / 4 + 3
								+ get_skill_level(ch, gsn_shot_arrow)*2
								+ (get_skill_level(ch,gsn_shot_arrow) / 6 ) * 4) * get_vrace_hrp(victim, wield) / 100
							: IS_SET( ch->class, 16 ) ? 20 : -60 )
						  :  0 );
			*/

    }else{  //  沒武器時
        int phr = (   ch->hitroll 
                    + (   str_app[get_curr_str(ch)].tohit * 6
                        + dex_app[get_curr_dex(ch)].tohit * 5
                        + wis_app[get_curr_wis(ch)].tohit * 3 ) / 100 ) ;

		//  reposte 會隨 hitroll 及技能等級變動命中加成 2022/01/13
		//  reposte 判定改以 看 isReposte 的 bool 2022/05/07
		//if(dt == gsn_reposte && ch->hitroll > 0)
		if( isReposte && ch->hitroll > 0 )
			phr += ( ch->hitroll * ( 25 + get_skill_level(ch, gsn_reposte) * 20
										+ get_skill_level(ch, gsn_reposte) * get_skill_level(ch, gsn_reposte) ) ) / 100;
		//  frenzy , hr 超過 50 之後, hr 的增加將會趨緩, 超過 1 hr 減 0.28 hr
		//  ex: phr 80 降到 71.6, phr 100 降到 86
        if ( dt == gsn_frenzy && phr > 50 ){
            phr -= interpolate1(50, 1, 100, 15, (phr-50));
        }

        //thac0 的計算公式, 這邊就是命中率的集大成了, 之後都以 thac0 來當做命中率了
        //大致說明
        //thac0 =   先天因素
        //        - 空手命中率
        //        - 種族體積影響
        //        - 空手 frenzy 命中變差
        //        - 空手射箭 (這是指 ran 空手 hr 還有加成就是了 ? orz)
        thac0 = interpolate( ch->level, thac0_00, thac0_42 )
				- phr
				- race_hit_bonus
				- ( ( dt == gsn_frenzy ) ? UMIN( (ch->level - 56), -8 ) : 0 )
				// archery 0 , thac0 = 55
				- ( ( arrow != NULL ) ? ( !IS_NPC( ch )
											? get_skill_percent( ch, gsn_shot_arrow ) / 4 + 3
												+ get_skill_level(ch, gsn_shot_arrow) * 2
												+ (get_skill_level(ch,gsn_shot_arrow) / 6 ) * 2
											: IS_SET( ch->class, 16 ) ? 20 : -60 ) : 0 );
    }

    //  處理蜥蝪人天賦對甩尾的命中加成
    //  天人合一時對 hr 加成 50%
	//  lizardman 空手甩尾命中加成與 fist 加成相同, 並加入 blind fight 設定 2022/02/23
    if(isLizTail){
		int nFistSklv = get_skill_level(ch, gsn_fist_fight);
        int nIbaSklv = get_skill_level(ch, gsn_inborn_ability);
        thac0 -= (ch->hitroll * nIbaSklv) / 12;
		if( IS_SET( ch->act, PLR_FIST_FIGHT ) ){
			//thac0 += thac0 * (100 + nFistSklv * 10) / 100;
			thac0 += thac0 * number_range(34 + nFistSklv * nFistSklv, 78 + nFistSklv * nFistSklv * 2) / 100;
            if( !check_blind(ch) && get_skill_percent(ch, gsn_blind_fight) > number_percent() ){
				thac0 -= 30 + get_skill_level(ch, gsn_blind_fight)*2;
            }else if( check_blind(ch) ){
				thac0 += (thac0 * get_skill_level(ch, gsn_fist_fight)) / 8;
            }
		}
    }

    //  針對 pc skill 額外再處理 thac0, 跟 fist_dam
    //  徒手攻擊 的影響(thac0, fist_dam) 跟 發勁 處理(fist_dam)
    //  這邊的 fist_dam 是針對, 徒手攻擊, 發勁 的傷害加成, 在後面傷害計算時才會用到
    if ( !IS_NPC( ch ) ){
		if (   IS_SET( ch->act, PLR_FIST_FIGHT ) && !isLizTail
			&& number_percent() < get_skill_percent( ch, gsn_fist_fight) )
		{
			//  修正徒手攻擊時的 hr 加成順序 Modified by Razgriz 20060725
			int nFistSklv = get_skill_level(ch, gsn_fist_fight);
			// Previous Version  1.5 倍
			// thac0 += thac0 + thac0 / 2;
			if( dt != gsn_frenzy ){
				thac0 += thac0 * (90 + nFistSklv * 10) / 100;
			}else{
				thac0 += thac0 * number_range(34 + nFistSklv * nFistSklv, 78 + nFistSklv * nFistSklv * 2) / 100;
			}
            if ( !check_blind(ch) && get_skill_percent(ch, gsn_blind_fight) > number_percent() ){
				//  修正正負號 Modified by Razgriz 20060722
				thac0 -= 30 + get_skill_level(ch, gsn_blind_fight)*2;
            }else if( check_blind(ch) ){
				thac0 += (thac0 * get_skill_level(ch, gsn_fist_fight)) / 8;
            }

			fist_dam = (9 + get_skill_level(ch, gsn_fist_fight) * 5) / 3;

			if ( is_affected(ch, gsn_powerup) && ch->move > 20 ) {
				ch->move -= 20;
				// powerup 加成受 str 影響傷害
				// fist_dam += 1 + get_skill_level(ch, gsn_powerup)/2;
				fist_dam += 1 + get_skill_level(ch, gsn_powerup) * ( 1 + get_curr_str(ch) * get_curr_str(ch)/250 )/2;
			}
		}
	}

	//  嗜血 配合 全力猛攻 的影響 (thac0, fist_dam)
	//  這邊的 fist_dam 是針對 嗜血 配合 全力猛攻 的傷害加成, 在後面傷害計算時才會用到
	//  整合新舊公式 Modified by Razgriz 20060717
	if ( ( !IS_NPC (ch) && ch->pcdata->learned[gsn_blood_thirsty] > 0
			//&& is_affected(ch,gsn_berserk)  移除 blood 必需在 berserk 下才能作用 2022//02/21
			&& number_percent() < get_skill_percent( ch, gsn_blood_thirsty ) + get_skill_level( ch, gsn_blood_thirsty ) * 5 ) )
	{
        int temp = thac0;
        //int cdex = get_curr_dex(ch);  for old version thac0
        int cstr = get_curr_str(ch);
		int ccon = get_curr_con(ch);
        int btsklv = get_skill_level(ch,gsn_blood_thirsty);
		//int MaxHpdiff = ch->max_hit - ch->hit;
		int nMaxHp = ch->max_hit/100;
		int bt_hit = 0;
		int bt_dam = affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT); // add at 2022/02/21

		//  blood thirsty 加成判定由最大血量百分比, 修改為戰鬥中造成傷害及受到傷害 2022/02/21

		// 命中加成
		if ( nMaxHp < 0 ) nMaxHp = 0; // 理論上不會發生最大 hp 為負值的情況

		bt_hit = 100 + 5 * btsklv + bt_dam * (4 + btsklv/3)/300;

		if( bt_dam > 5000 ) // 命中加成在累計值為 5000 時達頂峰隨後下降 , 最大 hp 能有效減緩命中加成下降
			bt_hit = bt_hit - ( bt_dam - 5000 ) * (4 + btsklv/3) / ( 150 + nMaxHp );

		thac0 = temp * bt_hit / 100;

		// 傷害加成
		fist_dam = (10000 + bt_dam/8 + 2 * ccon * ccon + btsklv * cstr * cstr)/5100 + ((1 + btsklv * btsklv) * bt_dam)/85000;

		if( is_affected(ch,gsn_berserk) ) // 全力猛攻 下有較高的傷害加成
			fist_dam = fist_dam + fist_dam/3;

		/*   old version
		if( ch->pcdata->prime_attr == APPLY_STR){
			//hp 100%
			if( ch->hit * 20 <= ch->max_hit * 20 ){
				thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
				fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3600;
				//hp 95%
				if( ch->hit * 20 <= ch->max_hit * 19 ){
					thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
					fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3400;
					//hp 90%
					if ( ch->hit * 20 <= ch->max_hit * 18 ){
						thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
						fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3200;
						//hp 85%
						if ( ch->hit * 20 <= ch->max_hit * 17 ){
							thac0 = temp*(1+ UMAX(1,btsklv*3)/14 + MaxHpdiff/50000);
							fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3000;
							//hp 80%
							if ( ch->hit * 20 <= ch->max_hit * 16 ){
								thac0 = temp*(1+ UMAX(1,btsklv*3)/13 + MaxHpdiff/50000);
								fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2900;
								//hp 75%
								if( ch->hit * 20 <= ch->max_hit * 15 ){
									thac0 = temp*(1+ UMAX(1,btsklv*3)/12 + MaxHpdiff/50000);
									fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2800;
									//hp 70%
									if ( ch->hit * 20 <= ch->max_hit * 14 ){
										thac0 = temp*(1+ UMAX(1,btsklv*3)/11 + MaxHpdiff/50000);
										fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2700;
										//hp 65%
										if(ch->hit * 20 <= ch->max_hit * 13 ){
											thac0 = temp*(1+ UMAX(1,btsklv*3)/10 + MaxHpdiff/50000);
											fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2600;
											//hp 60%
											if( ch->hit * 20 <= ch->max_hit * 12 ){
												thac0 = temp*(1+ UMAX(1,btsklv*3)/9 + MaxHpdiff/50000);
												fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2500;
												//hp 55%
												if ( ch->hit * 20 <= ch->max_hit * 11 ){
													thac0 = temp*(1+ UMAX(1,btsklv*3)/8 + MaxHpdiff/50000);
													fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2400;
													//hp 50%
													if( ch->hit * 20 <= ch->max_hit * 10 ){
														thac0 = temp*(1+ UMAX(1,btsklv*3)/8 + MaxHpdiff/50000);
														fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2300;
														//hp 45%
														if(ch->hit * 20 <= ch->max_hit * 9){
															thac0 = temp*(1+ UMAX(1,btsklv*3)/9 + MaxHpdiff/50000);
															fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2200;
															//hp 40%
															if(ch->hit * 20 <= ch->max_hit * 8){
																thac0 = temp*(1+ UMAX(1,btsklv*3)/11 + MaxHpdiff/50000);
																fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2150;
																//hp 35%
																if(ch->hit * 20 <= ch->max_hit * 7){
																	thac0 = temp*(1+ UMAX(1,btsklv*3)/12 + MaxHpdiff/50000);
																	fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2010;
																	//hp 30%
																	if(ch->hit * 20 <= ch->max_hit * 6){
																		thac0 = temp*(1+ UMAX(1,btsklv*3)/13 + MaxHpdiff/50000);
																		fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1860;
																		//hp 25%
																		if(ch->hit * 20 <= ch->max_hit * 5){
																			thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
																			fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1650;
																			//hp 20%
																			if(ch->hit * 20 <= ch->max_hit * 4){
																				thac0 = temp*(1+ UMAX(1,btsklv*3)/17 + MaxHpdiff/50000);
																				fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1500;
																				//hp 15%
																				if(ch->hit * 20 <= ch->max_hit * 3){
																					thac0 = temp*(1+ UMAX(1,btsklv*3)/18 + MaxHpdiff/50000);
																					fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1450;
																					//hp 10%
																					if(ch->hit * 20 <= ch->max_hit * 2){
																						thac0 = temp*(1+ UMAX(1,btsklv*3)/20 + MaxHpdiff/50000);
																						fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1400;
																						//hp 5%
																						if(ch->hit * 20 <= ch->max_hit * 1){
																							thac0 = temp*(1+ UMAX(1,btsklv*3)/21 + MaxHpdiff/50000);
																							fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1400;
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}else{
			//hp 100%
			if( ch->hit * 20 <= ch->max_hit * 20 ){
				thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
				fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3400;
				//hp 95%
				if( ch->hit * 20 <= ch->max_hit * 19 ){
					thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
					fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3200;
					//hp 90%
					if ( ch->hit * 20 <= ch->max_hit * 18 ){
						thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
						fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/3000;
						//hp 85%
						if ( ch->hit * 20 <= ch->max_hit * 17 ){
							thac0 = temp*(1+ UMAX(1,btsklv*3)/14 + MaxHpdiff/50000);
							fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2700;
							//hp 80%
							if ( ch->hit * 20 <= ch->max_hit * 16 ){
								thac0 = temp*(1+ UMAX(1,btsklv*3)/13 + MaxHpdiff/50000);
								fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2600;
								//hp 75%
								if( ch->hit * 20 <= ch->max_hit * 15 ){
									thac0 = temp*(1+ UMAX(1,btsklv*3)/12 + MaxHpdiff/50000);
									fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2550;
									//hp 70%
									if ( ch->hit * 20 <= ch->max_hit * 14 ){
										thac0 = temp*(1+ UMAX(1,btsklv*3)/11 + MaxHpdiff/50000);
										fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2500;
										//hp 65%
										if(ch->hit * 20 <= ch->max_hit * 13 ){
											thac0 = temp*(1+ UMAX(1,btsklv*3)/10 + MaxHpdiff/50000);
											fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2450;
											//hp 60%
											if( ch->hit * 20 <= ch->max_hit * 12 ){
												thac0 = temp*(1+ UMAX(1,btsklv*3)/9 + MaxHpdiff/50000);
												fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2400;
												//hp 55%
												if ( ch->hit * 20 <= ch->max_hit * 10 ){
													thac0 = temp*(1+ UMAX(1,btsklv*3)/8 + MaxHpdiff/50000);
													fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2350;
													//hp 50%
													if( ch->hit * 20 <= ch->max_hit * 10 ){
														thac0 = temp*(1+ UMAX(1,btsklv*3)/8 + MaxHpdiff/50000);
														fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2200;
														//hp 45%
														if(ch->hit * 20 <= ch->max_hit * 9){
															thac0 = temp*(1+ UMAX(1,btsklv*3)/9 + MaxHpdiff/50000);
															fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2100;
															//hp 40%
															if(ch->hit * 20 <= ch->max_hit * 8){
																thac0 = temp*(1+ UMAX(1,btsklv*3)/11 + MaxHpdiff/50000);
																fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/2000;
																//hp 35%
																if(ch->hit * 20 <= ch->max_hit * 7){
																	thac0 = temp*(1+ UMAX(1,btsklv*3)/12 + MaxHpdiff/50000);
																	fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1900;
																	//hp 30%
																	if(ch->hit * 20 <= ch->max_hit * 6){
																		thac0 = temp*(1+ UMAX(1,btsklv*3)/13 + MaxHpdiff/50000);
																		fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1800;
																		//hp 25%
																		if(ch->hit * 20 <= ch->max_hit * 5){
																			thac0 = temp*(1+ UMAX(1,btsklv*3)/15 + MaxHpdiff/50000);
																			fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1600;
																			//hp 20%
																			if(ch->hit * 20 <= ch->max_hit * 4){
																				thac0 = temp*(1+ UMAX(1,btsklv*3)/17 + MaxHpdiff/50000);
																				fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1500;
																				//hp 15%
																				if(ch->hit * 20 <= ch->max_hit * 3){
																					thac0 = temp*(1+ UMAX(1,btsklv*3)/18 + MaxHpdiff/50000);
																					fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1400;
																					//hp 10%
																					if(ch->hit * 20 <= ch->max_hit * 2){
																						thac0 = temp*(1+ UMAX(1,btsklv*3)/20 + MaxHpdiff/50000);
																						fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1350;
																						//hp 5%
																						if(ch->hit * 20 <= ch->max_hit * 1){
																							thac0 = temp*(1+ UMAX(1,btsklv*3)/21 + MaxHpdiff/50000);
																							fist_dam = (10000+ccon*ccon*2+cstr*cstr*UMAX(1,btsklv))/1350;
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		} */
    }// END -- 嗜血的影響
	
	// weakness_percent 命中判定 add at 2020/09/18
	if(    dt == gsn_frenzy
		|| dt == gsn_backstab
		|| dt == gsn_bashhead
		|| dt == gsn_circular
		|| dt == gsn_tactics  // lizardman 掃尾
		|| dt == gsn_empslash)
	{
		thac0 = thac0 * weakness_percent(victim , dt) / 100;
	}

	// 普擊 跟 weapon type weakness_percent 命中弱點個別設定 add at 2020/09/18
	// weapon master 對 type 弱點增加命中 2020/10/14

	if( dt == 1000 ) // 空手攻擊
		thac0 = thac0 * weakness_percent(victim , gsn_hit_attack) / 100;
	if( wield && wield->value[3] == 1 )  // 銳利切割
	{
		if( weakness_percent(victim , gsn_slice_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_slice_attack)
							+ (weakness_percent(victim , gsn_slice_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_slice_attack) / 100;
	}
	if( wield && wield->value[3] == 2 )  // 威猛一戳
	{
		if( weakness_percent(victim , gsn_stab_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_stab_attack)
							+ (weakness_percent(victim , gsn_stab_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_stab_attack) / 100;
	}
	if( wield && wield->value[3] == 3 )  // 揮舞劈砍
	{
		if( weakness_percent(victim , gsn_slash_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_slash_attack)
							+ (weakness_percent(victim , gsn_slash_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_slash_attack) / 100;
	}
	if( wield && wield->value[3] == 4 )  // 灼熱一鞭
	{
		if( weakness_percent(victim , gsn_whip_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_whip_attack)
							+ (weakness_percent(victim , gsn_whip_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_whip_attack) / 100;
	}
	if( wield && wield->value[3] == 5 )  // 用力一抓
	{
		if( weakness_percent(victim , gsn_claw_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_claw_attack)
							+ (weakness_percent(victim , gsn_claw_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_claw_attack) / 100;
	}
	if( wield && wield->value[3] == 6 )  // 爆發噴擊
	{
		if( weakness_percent(victim , gsn_blast_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_blast_attack)
							+ (weakness_percent(victim , gsn_blast_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_blast_attack) / 100;
	}
	if( wield && wield->value[3] == 7 )  // 萬鈞力搥
	{
		if( weakness_percent(victim , gsn_pound_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_pound_attack)
							+ (weakness_percent(victim , gsn_pound_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_pound_attack) / 100;
	}
	if( wield && wield->value[3] == 8 )  // 狠狠碾壓
	{
		if( weakness_percent(victim , gsn_crush_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_crush_attack)
							+ (weakness_percent(victim , gsn_crush_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_crush_attack) / 100;
	}
	if( wield && wield->value[3] == 9 )  // 犀利一剁
	{
		if( weakness_percent(victim , gsn_grep_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_grep_attack)
							+ (weakness_percent(victim , gsn_grep_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_grep_attack) / 100;
	}
	if( wield && wield->value[3] == 10 ) // 張口猛咬
	{
		if( weakness_percent(victim , gsn_bite_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_bite_attack)
							+ (weakness_percent(victim , gsn_bite_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_bite_attack) / 100;
	}
	if( wield && wield->value[3] == 11 ) // 尖銳刺擊
	{
		if( weakness_percent(victim , gsn_pierce_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_pierce_attack)
							+ (weakness_percent(victim , gsn_pierce_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_pierce_attack) / 100;
	}
	if( wield && wield->value[3] == 12 ) // 猛烈吸吮
	{
		if( weakness_percent(victim , gsn_suction_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_suction_attack)
							+ (weakness_percent(victim , gsn_suction_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_suction_attack) / 100;
	}
	if( wield && wield->value[3] == 13 ) // 剁菜連斬
	{
		if( weakness_percent(victim , gsn_chop_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_chop_attack)
							+ (weakness_percent(victim , gsn_chop_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_chop_attack) / 100;
	}
	if( wield                            // 射出箭矢 , 槍械射擊
		&& (   ( wield->value[3] == 14 && arrow )
			|| ( wield->value[3] == 16 && ammo ) ) )
	{
		if( weakness_percent(victim , gsn_shot_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_shot_attack)
							+ (weakness_percent(victim , gsn_shot_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_shot_attack) / 100;
	}


    //比例縮小 dodge 數值, 以方便等等跟 thac0 做比較
    //1. victim 是 mob 的話, mob dodge 縮為 1/10
    //2. npc 扁 pc 時, pc dodge 縮為 1/6
    //3. 其他的情況(也就是 PK), dodge 縮為 1/8
    if( IS_NPC(victim) ){
        victim_dodge = GET_DODGE( victim ) / 10;
    }else if( IS_NPC(ch) ){
        victim_dodge = GET_DODGE( victim ) / 6;
    }else{
        victim_dodge = GET_DODGE( victim ) / 8;
    }

    // mob class 有 thief 時 dc 由增加 50% 下降為 25% 
	// 並且加入 dc 必須為負值時才會作用 2020/05/01
    if ( IS_NPC( victim ) && IS_SET( victim->class, 4 ) && victim_dodge < 0 )
        victim_dodge += victim_dodge / 4;

    //ch 看不到 victim 時, 提升 victim 的 dodge, 等於變相降低 ch 的命中率
    if ( !can_see( ch, victim ) )
        victim_dodge -= 60;

    //   0 <= number_bits( 5 ) <= 31
    // -30 <= diceroll         <=  0
    //這邊是代表, 絕對命中的緩衝空間
    //ex: dodge -700 的 mob
    //    hr > 100, 完全命中, 但還是有 1/32 機率打不到 (diceroll = -30, 11110 = 30)
    //    70 < hr < 100, 隨著 hr 越高, 打不到的機率越小
    //    hr < 70, 打不到, 但還是有 1/32 機率能打到 (diceroll = 0, 00000 = 0)
    while ( ( diceroll = - number_bits( 5 ) ) <= -30 ) ;


    //pc 跟 沒有設 ACT_SPEEDHIT 的 npc 都要進來判斷是否命中, 跑進去代表攻擊 miss 
    //繞刺 跟 日月雙劍 必中就不必進來了
	if( !IS_NPC( ch ) || !IS_SET( ch->act, ACT_SPEEDHIT ) )
	{
		if (   dt != gsn_circle
			&& dt != gsn_twin
			&& ( ( diceroll != 0 && diceroll < thac0 - victim_dodge )
				|| diceroll == -30 ) )
		{
			/* Miss. */
			//進來這邊就代表沒打到了

			// 1/4 機率消掉 darkraid
			if ( IS_AFFECTED( ch, AFF_DARK_RAID ) && number_bits( 2 ) == 1 ){
				affect_strip( ch, gsn_dark_raid );
				send_to_char( "你的專注力被打散了！\n\r", ch );
				darkraid = 1;
			}

			// 1/4 機率消掉 nightstalk
			if ( IS_AFFECTED( ch, AFF_NIGHTSTALK )  && number_bits( 2 ) == 1 ){
				affect_strip( ch, gsn_nightstalk );
			}

			if ( msg_mode == MSG_NORMAL ) dam_message( ch, victim, -1, dt, wpn );

			dam = -1; // -1 denotes a missing attack
					  // -2 denotes a failure attack

			// 加入 攻擊 miss 的情況下對箭與彈藥數量判定 2022/05/04
			if( isShootCost )
			{
				if( arrow )
					ShotObjCount( ch , arrow );
				if( ammo )
					ShotObjCount( ch , ammo );
			}

			// mob (victim) 逃跑處理 , 有ACT_NO_FLEE 就不會作do_flee
			if ( IS_NPC( victim ) && !IS_SET( victim->act, ACT_NO_FLEE ) ){
				if (   (   IS_SET( victim->act, ACT_WIMPY ) && number_bits( 1 ) == 0
						  && victim->hit < victim->max_hit / 2 && victim->wait == 0 )
					|| (  (   IS_AFFECTED( victim, AFF_CHARM ) || victim->clan
						   || IS_AFFECTED( victim, AFF_SUMMONED ))
						&& victim->master && victim->in_room
						&& victim->master->in_room != victim->in_room ) )
				{
					do_flee( victim, "" );
				}
			}

			// modified by coconet for let ppl in stun can not wimpy 0614
			if (  !IS_NPC( victim )
				&& victim->hit  > 0
				&& victim->hit  <= victim->wimpy
				&& victim->wait < 3 )
			{
				do_flee( victim, "" );
			}

			tail_chain( );
			return dam;
		}
	}

	/*  移置最前方作判定 2022/05/07
    //間隙反擊 成功, 把攻擊型態換成武器的攻擊型態
    if ( dt == gsn_reposte ){
        dt = TYPE_HIT;
        isReposte = TRUE;
        if ( wield && wield->item_type == ITEM_WEAPON ){
            dt += wield->value[3];
        }
        if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
    } */

    //傷害計算開始 !!

    //武器造成的基本傷害計算
    if ( IS_NPC( ch ) ){
    //  npc 部分, 以 有沒拿武器 跟 npc等級 來計算基本傷害
		//  加入 槍械射擊 的判定 2022/05/01
        if( !arrow && !ammo ){
            //dam   = number_range( ch->level * 1.2 , ch->level * 1.5 );  2020/09/20
			dam   = number_range( ch->level * 2.1 , ch->level * 2.3 );
        }else if ( wield ){
            //dam   = number_range( ch->level * 1.2 , ch->level * 1.5 );  2020/09/20
			dam   = number_range( ch->level * 2.1 , ch->level * 2.3 );
            if ( arrow ){
				dam   = (dam * 3) / 5;
                bow   = wield;
                wield = arrow;
            }
			// 轉換 槍械射擊 的武器 2022/05/01
			if ( ammo ){
				dam   = (dam * 4) / 5;
                gun   = wield;
                wield = ammo;
			}
        }else{
            //dam = number_range( ch->level * 1.5 , ch->level * 2 ); 2020/09/20
			dam = number_range( ch->level * 17 , ch->level * 17.3 );
        }
    }else{
	//  pc 部分, 有拿武器時
		if ( wield )
		{
		//  處理武器磨損
			if( wield->value[3] != 14 && number_bits(5) < 8 ){ // 7/(16 + 8) , 29%
				if( get_skill_level(ch,gsn_weapon_master) * 6 < number_percent() ) // weapon master 等級降低武器損壞機率 2022/12/02
					waste_dura( wield );
			}

		//  武器傷害力 (value[1] 及 value[2] 為 weapon 最小最大傷害
			// 槍械射擊 不受 武器傷害力 影響 2022/05/01
			if( wield->value[3] == 16 && ammo )
			{
				;
			}
			else
			{
				dam = number_range( wield->value[1], wield->value[2] );
				dam += dam/4;
			}
		//  雙手武器時, 傷害的加成處理

			/*  原公式過多錯誤且不作用 , 重新修改判定式 2021/10/13
				if( wpn == ITEM_TWO_HAND && !CAN_WEAR(wield, ITEM_TWO_HAND ) ){
					if( number_percent() < get_skill_percent(ch, gsn_powerblow)){
						dam = wield->value[2];
						dam += dam/3;
					}
					dam += dam/3 ;

					if( ch->class == MARTIALMAN ){
						if( dt == gsn_frenzy ){
							dam = dam + ( dam / 4) + ( dam*get_skill_level(ch, gsn_powerblow) ) / 15 ;
						}else{
							dam = dam +( dam / 5) +(dam*get_skill_level(ch, gsn_powerblow))/20 ;
						}
					}else{
						dam += (get_curr_str(ch)*2) + get_skill_level(ch, gsn_powerblow)*2 ;
					}
				}else if( wpn == ITEM_TWO_HAND ){ // 單手武器雙手拿時的傷害加成
					if( number_percent() < get_skill_percent( ch, gsn_powerblow) ){
						dam = wield->value[2];
						dam += dam/3;
					}

					dam += dam/3;

					if( ch->class == MARTIALMAN && dt == gsn_frenzy ){
						dam = dam +( dam / 3) +(dam*get_skill_level(ch, gsn_powerblow))/15 ;
					}
				}
			*/

			// 裝備雙手武器且不為 射箭 或 槍械 , 2022/05/01
			//if( wpn == WEAR_TWO_HAND && wield->value[3] != 14 ){  // 將判定武器類形改為判定是有箭或彈藥
			if( wpn == WEAR_TWO_HAND && !arrow && !ammo ){
				if( CAN_WEAR(wield, ITEM_WIELD ) ){  // item wield 與 twohand 並存時會優先判定為前者(能單手穿)
					dam += dam/5;  // 加成 20%
					if( (number_percent() // powerblow 額外加成 lv 6 , 80% 機率
							< ( get_skill_percent(ch, gsn_powerblow)/5 + get_skill_level(ch, gsn_powerblow)*10 ))
						|| isBloodThirsty ) // 主動 blood thirsty 狀態下 100% 成功 add at 2021/12/25
					{
						dam = wield->value[2];
						dam += dam/5;
						dam += dam * get_skill_level(ch, gsn_powerblow)/24; // 最高多 25%
						isPowerBlow = TRUE; // 判定 powerblow 成立 2021/12/25
					}
				}else{ // 裝備不能 item wield 則必定為 item twohand ( npc reset 的裝備不會發生在此處 )
					dam += dam/3;  // 加成 33%
					if( (number_percent()
							< ( get_skill_percent(ch, gsn_powerblow)/5 + get_skill_level(ch, gsn_powerblow)*10 ))
						|| isBloodThirsty )
					{
						dam = wield->value[2];
						dam += dam/3;
						dam += dam * get_skill_level(ch, gsn_powerblow)/20; // 最高多 30%
						isPowerBlow = TRUE;  // 判定 powerblow 成立 2021/12/25
					}
				}
			}

		//  武器磨損對傷害的影響
			//  移置所有傷害判定的最後並修改數值使磨損影響更大 2021/10/13
				// 槍械射擊 時磨損度不會影響到傷害 2022/05/01
			if( !indestructible( wield ) && !ammo ){
				int cd = get_curr_dura(wield), md = get_max_dura(wield);
				/* if( cd * 2 <= md ) {
					dam -= (dam/3);
				}else{
					if(cd * 3 <= md ) dam /= 2;
				}*/

				// 加入高耐久度武器設定 , 300 以上則為高耐久度武器 2022/12/02
				// 武器耐久度大於 300 不會判定減傷 2022/12/02
				if( cd <= 300 )
				{
					// 最大耐久度大於 300 時, 則以 300 作為最大耐久度 2022/12/02
					if( md > 300 ) md = 300;
					if( cd * 10 <= md )          // cd 10% , dam 10%
						dam = dam - dam * 9/10;
					else if( cd * 10 <= md * 2 ) // cd 20% , dam 30%
						dam = dam - dam * 7/10;
					else if( cd * 3 <= md )      // cd 33% 磨損, dam 50%
						dam = dam - dam * 5/10;
					else if( cd * 10 <= md * 5 ) // cd 50% , dam 70%
						dam = dam - dam * 3/10;
					else if( cd * 10 <= md * 7 ) // cd 70% , dam 90%
						dam = dam - dam * 1/10;
				}
			}

			//  拿弓時，將箭設為主要的武器，而原武器則改用 bow 來存
			if ( arrow && dt != gsn_frenzy ){
				bow   = wield;
				wield = arrow;
			}
			// 轉換 槍械射擊 的武器 2022/05/01
			if ( ammo ){
                gun   = wield;
                wield = ammo;
			}
		}else{  //  pc 部分, 沒拿武器時 , 傷害受 體型 size 影響
			//dam = number_range( 1, 2 ) * race_table[ ch->race ].size;
			dam = number_range( 1, 2 ) * UMAX( 10 , get_curr_size( ch ) ); // 替換體型 size 公式 2022/12/23
		}

		/* 拿著武器且傷害超過 1000, log 起來
		if ( wield && dam > 1000 ){
			sprintf( buf, "One_hit dam range > 1000 from %d to %d",wield->value[1], wield->value[2] );
			bug( buf, 0 );
		}*/
    }


	/* waste durability */
	//被攻擊方的裝甲磨損處理
	if( !IS_NPC(victim) ){
		for(i=0 ;i<15;++i ){
		//  max try 15 times
			place = number_range (1,15);
			if( ( armor = get_eq_char(victim, place)) && armor->item_type == ITEM_ARMOR){
				waste_dura(armor);
				break;
			}
		}
	}

	//額外傷害加成

	//處理武器對種族的加成, 武器材質的加成, 還有之前的 fist_dam 加成
	if( wield && !IS_NPC(ch) && !isLizTail){
		//  使用箭射擊時，也同時考慮弓本身對種族的加成    added by keelar 
		if(arrow != NULL && dt != gsn_frenzy && arrow == wield && bow){
			int nBowAvg   = number_range( bow->value[1], bow->value[2] );
			int nArwAvg   = number_range( arrow->value[1], arrow->value[2] );
			int nArchSklv = get_skill_level(ch, gsn_shot_arrow);
			int nDamBonus = 0;
			
			// 高avg的傷害成長降低
			if( !IS_SET( bow->extra_flags, ITEM_CLAN ) && nBowAvg > 87)
				nBowAvg = 87 + (nBowAvg - 87) * 0.5;
			// clan wp 高avg的傷害成長降低
			if( IS_SET( bow->extra_flags, ITEM_CLAN ) && nBowAvg > 70)
				nBowAvg = 70 + (nBowAvg - 70) * 0.1;
			
			// nBowAvg 到達 35 .. 會有 fist 的水準
			// 增加 飛行武器 dr 效果加成 2022/05/06
			nDamBonus = ( get_damroll(ch, wpn) + ShootObjAffect(ch, arrow, 2) )
						* (nBowAvg * 2 + nArwAvg) * get_vrace_drp(victim, bow) / 5500;

			// 降低 archery 對傷害的影響 , 拉高基本傷害加成  2022/05/01
			//nDamBonus = nDamBonus * (100 + nArchSklv * 50) / 100;
			//dam += nDamBonus * (100 + 2 * get_skill_percent( ch, gsn_shot_arrow )) / 200;
			nDamBonus = nDamBonus * (190 + nArchSklv * 35) / 100;
			dam += nDamBonus * (201 + get_skill_percent( ch, gsn_shot_arrow )) / 200;

			// Previous versiono of damage calculation
			// dam += (( get_damroll(ch, wpn) * fist_dam * get_vrace_drp(victim, bow)) / 500)
			//         * get_vrace_drp(victim, wield) / 100;
			dam += (bow->material->value[1] ) + (wield->material->value[1] );
			dam += (get_vrace_drq(victim, bow) + get_vrace_drq(victim, wield)) * (nBowAvg * 2 + nArwAvg) / 55;
		//  槍械射擊的傷害 2022/05/01
		}else if( ammo != NULL && ammo == wield && gun ){
			int nAmmoDam = number_range( (ammo->value[1]*19)/20 , (ammo->value[1]*20)/19 );
			int nFMSklv = get_skill_level(ch, gsn_firearm_mastery);
			int nDamBonus = 0;
			// 增加 飛行武器 dr 效果加成 2022/05/06
			nDamBonus = ( ( get_damroll(ch, wpn) + ShootObjAffect(ch, ammo, 2) + nAmmoDam ) * 23 ) * get_vrace_drp(victim, gun) / 550;
			nDamBonus = nDamBonus * (400 + nFMSklv * 75) / 200;
			dam += nDamBonus * (201 + get_skill_percent( ch, gsn_firearm_mastery )) / 200;

			dam += ( ( gun->material->value[1] + wield->material->value[1]
					+ get_vrace_drq(victim, gun) + get_vrace_drq(victim, wield)
					+ nAmmoDam ) * 23 ) / 55;
        }else{
            int nWpnAvg = number_range( wield->value[1], wield->value[2] );
			
			// 高avg的傷害成長降低
			if( !IS_SET( wield->extra_flags, ITEM_CLAN ) && nWpnAvg > 87)
				nWpnAvg = 87 + (nWpnAvg - 87) * 0.5;
			// clan wp 高avg的傷害成長降低
			if( IS_SET( wield->extra_flags, ITEM_CLAN ) && nWpnAvg > 70)
				nWpnAvg = 70 + (nWpnAvg - 70) * 0.1;
			
            if( !IS_NPC(ch) && ch->pcdata->learned[gsn_powerblow] > 0){
                int nPbSklv  = get_skill_level(ch, gsn_powerblow);
                int nPbBonus = wield->weight * (nPbSklv * nPbSklv) / 300;
				// 統一單次攻擊的 powerblow 判定 2021/12/25
                //int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
                //if(nPbPerc / 3 + 1 + nPbSklv * 11 >= number_percent()){
				if( isPowerBlow ){
                    nWpnAvg = wield->value[2];
                }
                nWpnAvg = nWpnAvg * (110 + nPbBonus) / 100;
            }

			if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 ){
				int nWmSklv  = get_skill_level(ch,gsn_weapon_master);
				int nWmBonus = (nWmSklv * 15 + 10) * 10 / UMAX((10 + wield->weight - get_curr_str(ch)), 8);
				nWpnAvg = nWpnAvg * (100 + nWmBonus) / 100;
            }
            
            // nWpnAvg 到達 44 (柴刀).. 會有以前的水準 (44 / 55) = 4 / 5, 
            // fist_dam / 4 用作加成比例
			if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 && get_vrace_drp(victim, wield) > 100){
				// Weapon master 武器種族傷害百分比加成
				int nWmSklv  = get_skill_level(ch,gsn_weapon_master);
				dam += get_damroll(ch, wpn) * (nWpnAvg + 44) * fist_dam * get_vrace_drp(victim, wield)
					    * (1 + nWmSklv * ( nWmSklv + 2 ) * get_curr_int(ch) /2200) / (8 * 5500);
			}else{
				dam += get_damroll(ch, wpn) * (nWpnAvg + 44) * fist_dam * get_vrace_drp(victim, wield) / (8 * 5500);
			}
            // Previous version of damage calculation
            // dam += ( get_damroll(ch, wpn) * fist_dam * get_vrace_drp(victim, wield)) / 500;
			dam += (wield->material->value[1] );
			if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 && get_vrace_drq(victim, wield) > 0){
				// Weapon master 武器種族傷害加成
				int nWmSklv  = get_skill_level(ch,gsn_weapon_master);
				dam += fist_dam * (nWpnAvg + 44) * get_vrace_drq(victim, wield)
					   * (1 + nWmSklv * ( nWmSklv + 4 ) * get_curr_int(ch) /2200) / (8 * 55);
			}else{
				dam += fist_dam * (nWpnAvg + 44) * get_vrace_drq(victim, wield) / (8 * 55);
			}
        }
    }else if( wield && IS_NPC(ch)){  // npc wielded dr
        //dam += ( get_damroll(ch, wpn)  *  7 * fist_dam * get_vrace_drp(victim, wield)) / 3200; // old ver. 7 * fist_dam(4)  = 28
		//dam += ( get_damroll(ch, wpn) * ch->level * get_vrace_drp(victim, wield)) / 3050;  // dam + dr * level/6 2020/09/20
		dam += ( get_damroll(ch, wpn) * ch->level * fist_dam * get_vrace_drp(victim, wield) )/ 6500;
		//dam += ( get_vrace_drq(victim, wield) * ch->level * fist_dam )/2500;
		if( wield->item_type == ITEM_WEAPON ) // weapon avg damage effect add at 2021/10/11
			dam += number_range( wield->value[1], wield->value[2] )/5;
		dam += (wield->material->value[1] );
		//dam += get_vrace_drq(victim, wield);
	}else if(!isLizTail){
		if(!IS_NPC(ch)){
			dam += ( 2 * get_skill_percent( ch, gsn_fist_fight) + 100) // 空手 dam += dr * 3;
					* ( ((get_damroll( ch, wpn ) ) * fist_dam ) / 4 ) / 300;
		}else{
			dam += get_damroll(ch, wpn);
			if( IS_SET( ch->class, 32 ) && dam > 0){ //mob 有 class 32 (bloody) 時的空手攻擊傷害加成  2021/10/11
				if( IS_SET( ch->class, 1 ) ) // 避免 class 1 (warrior) 兩次加成過高  2021/10/11
					dam = dam + dam * 2;
				else
					dam = dam + dam * 3;
			}
		}
	}else{
		dam += get_damroll(ch, wpn);
	}
	
	/*  修改 poisoned weapon 功能 , 移除直接性的傷害加成 2021/11/10
    //非射箭時, 有毒武器傷害加成 1/4
    if ( wield && IS_SET( wield->extra_flags, ITEM_POISONED ) && dt != 1014 ){
        //clan武器加成 1/8, 手動抹毒武器加成 1/4 (? )
		dam += dam / 8;
		// 18/07/20 clan weapon 傷害加成 1/8 , 其它加成 1/4
		if( !IS_SET( wield->extra_flags, ITEM_CLAN ) )
			dam += dam / 8;

		//傷害平衡 移除額外加成 20200916
        //if(dt == gsn_circle)
        //    dam += dam * (20 + get_skill_level(ch, gsn_circle) * 4) / 100;
        //else if(dt == gsn_backstab)
        //    dam += dam * (20 + get_skill_level(ch, gsn_backstab) * 4) / 100;

    }
	*/

	// victim 為暗殺目標時,傷害加成 2020/09/16
	if ( ch->assassinating && ch->assassinating == victim )
	{
		dam += dam * get_skill_level(ch, gsn_ass)/20;
	}

	/*  kick 不會跑入 mone_hit , 改寫於 do_kick 之下 2022/02/23

	// mon kick 傷害加成 4 倍
	if( !IS_NPC(ch) && ch->class == MONK && dt == gsn_kick ){
		dam  *= 4;
	}
	*/

    //  tactics 影響微量傷害
	if(!IS_NPC(ch) && get_skill_percent(ch, gsn_tactics)){
		int nTacPerc = get_skill_percent(ch, gsn_tactics);
		int nTacSklv = get_skill_level(ch, gsn_tactics);
		dam = dam * (30000 + nTacPerc * (9 + nTacSklv * nTacSklv) ) / 30000;
	}

	//weapon master 的加成處理
	if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 && !isLizTail){
		int nCurrInt = get_curr_int( ch );
		int nWmSklv  = get_skill_level(ch,gsn_weapon_master);
		int nWmPerc  = get_skill_percent(ch, gsn_weapon_master);
		int nWmBonus = (nWmSklv * nWmSklv + 4) * nCurrInt/ 13;
		if ( is_affected( ch, gsn_berserk ) ){
			int nBerSklv = get_skill_level(ch, gsn_berserk);
			// ( 60 * (108 + 42)) / 100 =  90%  for those int = 23
			nWmBonus = nWmBonus * (80 + nBerSklv * nBerSklv) / 100;
		}

		// 射擊的加成只有 1/2 2022/05/02
		if( dt == 1016 )
			nWmBonus = nWmBonus/2;

		dam += dam * nWmBonus / 100;

		if(isReposte){
			dam += dam * nWmBonus / 100;
		}

		if(number_percent() < (nWmPerc)){
			dam += dam * nWmBonus / 100;
		}
	}
    
	//enhanced damage 的加成處理
	if (    !IS_NPC( ch )
		&& ( get_skill_percent(ch, gsn_enhanced_damage) + get_skill_level(ch, gsn_enhanced_damage) * 5 > number_percent()
			|| nTpPerc + nTpSklv * 5 > number_percent()
			// 追加 powerblow 的判定 2022/02/23
			|| get_skill_percent(ch, gsn_powerblow) + get_skill_level(ch, gsn_powerblow) * 5 > number_percent() )
		&& dt != 1014 // 箭矢不受 ed 加成 2022/05/01
		&& dt != 1016 // 槍械射擊不受 ed 加成 2022/05/01
		&& (   dt >= TYPE_HIT
			|| dt == gsn_frenzy
			//|| dt == gsn_kick  /* kick 不會被代入 mone_hit 之中*/
			|| dt == gsn_bashhead
			|| dt == gsn_backstab
			|| dt == gsn_circle
			|| dt == gsn_circular
			|| dt == gsn_sprinkle_slash
			|| dt == gsn_flaming_slash
			|| dt == gsn_storm_slash
			|| dt == gsn_earth
			|| dt == gsn_empslash
			|| isLizTail))
	{
		dam = dam * 2 + (dam * get_skill_level(ch, gsn_enhanced_damage) )/ 4;
		if( ch->class == KNIGHT && ch->pcdata->prime_attr == APPLY_STR )
			dam += (dam + dam/3);
		if( ch->class == KNIGHT && ch->pcdata->prime_attr != APPLY_STR )
			dam += (dam + dam/3);
		if( ch->class == WARLORD )
			dam += ( dam /2);
		if( ch->class == MONK ){
			if( ch->pcdata->snd_attr == APPLY_STR )
				dam += ( dam /2);
			else
				dam += ( dam /4);
		}
		if( ch->class == SWORDSMAN )
			dam += ( dam /2);
		if( ch->class == ASSASSIN )
			dam += ( dam /2);
		if( ch->class == LORD )
			dam += ( dam /3);
		if( ch->class == ALCHEMIST )
			dam += ( dam /3);
		if( ch->class == MARTIALMAN ){
			if( get_skill_percent(ch, gsn_powerblow) > 0 )
				dam += dam * (5 + get_curr_str(ch) + get_skill_level(ch, gsn_powerblow)*5 )/ 100;
			dam += ( dam /3); // mar ed 加成 2022/02/23
		}
		if( ch->pcdata->prime_attr == APPLY_STR){
			dam += dam / 2;
		}else if( ch->pcdata->snd_attr == APPLY_STR){
			dam += dam / 3;
		}else{
			dam += dam * get_curr_str(ch) / 100;
		}
		if( IsUsingTrickyWeapon(ch , wield) )
			dam += dam * ( TrickyPacingBase(ch, 100) - 90 )/350;
	}else if (IsSorGateMobByChar(ch)){
		dam = dam * (1 + (ch->damroll)*(ch->damroll)/10000);
	}

	//mob 不是 射箭 及 槍彈 時的傷害加成
	if ( IS_NPC ( ch ) && dt != 1014  && dt != 1016 )
	{
		//mob 為 class 1 (warrior) 時的傷害加成
		if ( IS_SET( ch->class, 1 ) )
			dam = dam + dam;
	}

	//  powerbolt 效果判定 2022/05/01
	if( is_affected(ch, gsn_powerbolt) )
	{
		//  有箭的情況下
		if( arrow ){
			//  每下攻擊會損 mana , 當次攻擊如果 mana 不夠則移除效果
			if( arrow->pIndexData->vnum != OBJ_VNUM_MAGIC_ARROW ){
				if( ch->mana < 5)
					affect_strip(ch, gsn_powerbolt);
				else
					ch->mana -= 5;
			}else{
				//  用魔法箭耗損較低
				if( ch->mana < 2)
					affect_strip(ch, gsn_powerbolt);
				else
					ch->mana -= 2;
			}
		}else{
			//  沒箭的情況下直接移除 powerbolt 效果
			affect_strip(ch, gsn_powerbolt);
		}
	}

	//  處理蜥蝪人天賦對甩尾的傷害加成
	if(isLizTail){ //  lizardman 的甩尾攻擊
		int nIbaSklv = get_skill_level(ch, gsn_inborn_ability);
		dam  = dam * (40 + nIbaSklv * (4 + nIbaSklv)) / 40;
		dam  = dam * get_curr_str(ch) / 20;
		dam += ch->apdr * (40 + nIbaSklv * (4 + nIbaSklv)) / 50;
	}
	if(!IS_NPC(ch) && get_skill_percent(ch, gsn_tactics)){
		int nTacSklv = get_skill_level(ch, gsn_tactics);
		dam += ch->apdr * nTacSklv / 3;
	}

	if(!IS_NPC(ch) && IS_SET( ch->act, PLR_FIST_FIGHT ) && get_skill_percent(ch, gsn_fist_fight)){
		int nFistSklv = get_skill_level(ch, gsn_fist_fight);
		dam += ch->apdr * nFistSklv / 2;
	}

	//  count this factor in damage()
    //  if ( !IS_AWAKE( victim ) ) dam *= 2;


	//攻擊型態為 射出箭矢 shot, 且有箭時 , ran的倍數傷害
	if ( dt == 1014 && arrow )
	{

		/*   考慮 ran 會使用其他技能(如 frenzy) 導致 dt 轉變不為射箭(1014) 使判定不成立 ,
			 且 powerbolt 可於使用後 remove arrow 造成無 arrow 使判定也不成立
			 故移至 apdr 加成前作判定 2022/05/01

			if( is_affected(ch, gsn_powerbolt) ) 
			{
				if( ch->mana < 5){
					affect_strip(ch, gsn_powerbolt);
				}else if(arrow->pIndexData->vnum != OBJ_VNUM_MAGIC_ARROW ){
					ch->mana -= 2; 
				}else{
					ch->mana -= 1;
				}
			} */
		
		//加上箭隻傷害力
		int nArwAvg = number_range(arrow->value[1] , arrow->value[2]);
		dam += nArwAvg * (get_damroll(ch, wpn) + 100) * get_vrace_drp(victim, bow) / 10000;

		if( !IS_NPC(ch) ) 
		{
			//射箭的倍數加成, 兩種加成
			int nCurrWis = get_curr_wis(ch);
			int nCurrDex = get_curr_dex(ch);
			
			if( number_percent() < ((nCurrWis - 12) * abs(nCurrWis - 12)) / 20)
			//if( number_percent() < (((get_curr_dex(ch)+get_curr_wis(ch) )* 10) / 40))
			{
				//thief 正轉傷害加成
				if(ch->pcdata->prime_attr == APPLY_DEX)
				{
					dam *= 2 + (nCurrDex*nCurrDex / 400) + (get_skill_level(ch,gsn_shot_arrow)) / 2;
				}else{
					dam *= 2 + (get_skill_level(ch,gsn_shot_arrow)) / 2;
				}
				//  將箭的加成傷害列入 ED 加成及致命一擊之計算之中  modified by keelar
				dam += arrow->value[3] * (1 + get_skill_level(ch, gsn_shot_arrow) * 1 / 10);
				// pdr 減傷寫於 fight_damage 會造成高倍數碰到極高 pdr 的情況下依然造成傷害 2020/11/01
				dam -= victim->pdr;
			}
			else if(nCurrWis > number_percent())
			// else if(  ( (get_curr_dex(ch) + get_curr_wis(ch) )) / 2 > number_percent() )
			{
				//thief 正轉傷害加成
				if(ch->pcdata->prime_attr == APPLY_DEX)
				{
					dam *= 1 + (nCurrDex*nCurrDex / 500) + (get_skill_level(ch,gsn_shot_arrow) ) / 5;
				}else{
					dam *= 1 + (get_skill_level(ch,gsn_shot_arrow) ) / 5;
				}
				//  將箭的加成傷害列入 ED 加成及致命一擊之計算之中  modified by keelar
				dam += arrow->value[3] * (1 + get_skill_level(ch, gsn_shot_arrow) * 1 / 12);
				dam -= victim->pdr;
			}else{
				dam += arrow->value[3];
				dam -= victim->pdr;
			}
		}
		if( IS_NPC(ch) && number_bits( 3 )== 2 )
		{
			//射箭的倍數加成
			dam *= 2+(get_skill_level(ch,gsn_shot_arrow)*2)/4;
			dam -= victim->pdr;
		}
	}

	//狂暴攻擊加成
	if ( !IS_NPC(ch) && dt == gsn_frenzy && ch->class == MARTIALMAN)
	{
		dam = (dam * ch->level) / 28;
	}
	else if( !IS_NPC(ch) && dt == gsn_frenzy )
	{
		dam = (dam*ch->level) / 32;
	}

	//背刺加成
	if ( dt == gsn_backstab )
	{
		if( !IS_NPC(ch) && ch->class == ASSASSIN)
		{
			/*  舊版 asn , lv 41 sklv 6  dam = 13.8 倍
			dam = dam + (dam * (   UMIN( ( ch->level / 3 ), 14 ) / 2
								 + (   get_skill_level(ch,gsn_backstab)
									 + get_skill_level(ch,gsn_ass ) ) /2 ) ) ;
			*/

			// sklv 6 dam = 10 倍  add at 2020/09/16
			dam = dam + ( dam * ( 1 + get_skill_level(ch,gsn_backstab)*1.5 ) );
		}
		else
		{
			/*  舊版其他class , lv 41 sklv 6  dam = 8.63 倍
			dam = dam /2 + (dam * (   UMIN( ( ch->level / 4 ), 11 ) / 2
									+ (   get_skill_level(ch,gsn_backstab)
										+ get_skill_level(ch, gsn_ass) ) /2 ) ) ;
			*/

			// sklv 6  dam = 8 倍  add at 2020/09/16
			dam = dam + ( dam * ( 1 + get_skill_level(ch,gsn_backstab) ) );
		}
		//有 夜襲 傷害再加成, Added by Razgriz 20051015
		if ( !IS_NPC (ch) && ch->class == ASSASSIN && IS_AFFECTED ( ch, AFF_DARK_RAID ) )
		{
			switch ( get_skill_level ( ch, gsn_dark_raid ) )
			{
				case 6: dam += ( dam * 10 ) / 40; break; // 25%
				case 5: dam += ( dam * 8 ) / 40; break;  // 20%
				case 4: dam += ( dam * 7 ) / 40; break;  // 17.5%
				case 3: dam += ( dam * 6 ) / 40; break;  // 15%
				case 2: dam += ( dam * 5 ) / 40; break;  // 12.5%
				case 1: dam += ( dam * 4 ) / 40; break;  // 10%
				default: break;
			}
		}

		dam = dam * weakness_damage(victim, gsn_backstab) / 100; // by amethyst
	}

	//繞刺加成
	if ( dt == gsn_circle )       /* 200% to 300% at lev. 40 */
	{
		//    dam += dam  + ( dam * (ch->level+get_skill_level(ch,gsn_circle)+3*get_skill_level(ch,gsn_circle)/6)) / 10;
		dam += dam + ( dam * ( 2 * get_skill_level(ch,gsn_circle)
								+ 3 * get_skill_level(ch,gsn_circle) / 2
								+ 3 * get_skill_level(ch,gsn_circle) / 5
								+ 4 * get_skill_level(ch,gsn_circle) / 6 ) / 14);

		//有 夜襲 傷害再加成 add at 2020/09/16
		if ( !IS_NPC (ch) && ch->class == ASSASSIN && IS_AFFECTED ( ch, AFF_DARK_RAID ) )
		{
			switch ( get_skill_level ( ch, gsn_dark_raid ) )
			{
				case 6: dam += ( dam * 10 ) / 40; break; // 25%
				case 5: dam += ( dam * 8 ) / 40; break;  // 20%
				case 4: dam += ( dam * 7 ) / 40; break;  // 17.5%
				case 3: dam += ( dam * 6 ) / 40; break;  // 15%
				case 2: dam += ( dam * 5 ) / 40; break;  // 12.5%
				case 1: dam += ( dam * 4 ) / 40; break;  // 10%
				default: break;
			}
		}
		dam = dam * weakness_damage(victim, gsn_circle) / 100; //by amethyst
	}

    /* if ( ch->race == race_lookup( "Vampire" ) && !wield ) dam += dam / 2; */


	//有 魔力 效應的武器, 對於某些種族, 有機率性的 1/5 傷害加成
	dam += check_item_magic_flag(wield, victim->race, dam);

	// 有 暗黑 效應的武器, 機率性增加傷害加成
	// 修改為增加武器 avg 傷害 並增加武器毀損 2021/10/11
	if( wield && IS_SET(wield->extra_flags, ITEM_DARK)){
		if( number_bits(2) == 1 && wield->item_type == ITEM_WEAPON ){ // 25%
			if( !IS_SET( wield->extra_flags, ITEM_CLAN ) )
				dam += number_range( wield->value[1], wield->value[2] );
			else
				dam += number_range( wield->value[1], wield->value[2] )/3;
			if( !IS_NPC(ch) || IS_SET( ch->act, ACT_PET ) )
				waste_dura(wield);
			//dam += wield->level / 4;
		}
	}

	// empslash 對 物理傷害 加成
	if(dt == gsn_empslash){
		int nEmpSklv = get_skill_level(ch, gsn_empslash);
		dam += ( ch->apdr * nEmpSklv) / 3;
	}

	// powerblow 成立時對apdr(物理傷害)加成 , 且槍械不會作用 2022/05/02
	if( isPowerBlow && dt != 1016 ){
		int nPbSklv = get_skill_level(ch, gsn_powerblow);
		dam += (ch->apdr * nPbSklv) / 2;
	}

	// 槍械射擊 對 物理傷害 加成
	if(dt == 1016){
		int nFMSklv = get_skill_level(ch, gsn_firearm_mastery);
		// 增加 飛行武器 apdr 效果加成 2022/05/06
		dam += ( ( ch->apdr + ShootObjAffect(ch, ammo, 3) ) * nFMSklv )/ 3;
	}

	//全力猛攻加成
	if( is_affected(ch, gsn_berserk)) 
	{
		int nBerSklv = get_skill_level(ch, gsn_berserk);
		/*if( ch->class == SHAMAN || ch->class == MARTIALMAN){
			dam = dam * (100 + nBerSklv * (nBerSklv + 4)) / 100; */
		if( ch->class == SHAMAN || ch->class == MARTIALMAN ){
			dam = dam * (90 + nBerSklv * (nBerSklv + 4)) / 90;
		}else{
			dam = dam * (120 + nBerSklv * (nBerSklv + 4)) / 120;
		}
	}

	//處理致命一擊
	dam = get_critical_damage(dam, ch, victim, dexdiff, fCritical, dt, darkraid);

	// 追加判定 dam 有無小於 0  , 2020/11/08
	if ( dam <= 0 )
	{
		dam = 0;
	}
	//護甲防衛力(armor, ac) 的傷害減輕處理
	if ( !IS_NPC(victim))
	{
		if ( arrow || ammo ) // 加入 槍械射擊 的判定 2022/05/01
		{
			int nEnemyAC = GET_AC(victim);
			if(nEnemyAC >= -300){
				dam = dam * (600 + GET_AC(victim)) / 300;
			}else{
				dam = dam * 300 / (0 - GET_AC(victim));
			}
		}else{
			// 加入 powerblow 降低對手 ac 一定比例的判定 2021/12/25
			int nPbAc = 0;
			// 對手 ac 比 0 小 , sklv 6 約降低 40% ac
			if( isPowerBlow && GET_AC(victim) < 0 && ( ( isBloodThirsty && number_percent() < 30 ) || number_percent() < 20 ) ){
				nPbAc = GET_AC(victim) * ( 100 - (5400/(54 + get_skill_level(ch, gsn_powerblow) * get_skill_level(ch, gsn_powerblow) ) ) );
				nPbAc = nPbAc / 100;
			}

			int nEnemyAC = ( GET_AC(victim) - nPbAc ) * 0.1; // ac 數值過大造成計算為負值 2020/09/20
			if(GET_AC(victim) <= -1000){
				dam = dam * 3150 / ( nEnemyAC * nEnemyAC + 500 ); //  ac 1200 78% , ac 1500 86%
				//dam = dam * 1820000 / (nEnemyAC * nEnemyAC + 3000000);  數值過大 2020/09/20
			}else{
				dam = UMIN( dam + dam * ( GET_AC(victim) - nPbAc )/1429 , dam + ( GET_AC(victim) - nPbAc )/8 ); // ac 1000 70% , 800 55%
			}
		}
		//  dam = dam + UMIN( 0, ( arrow ) ? GET_SVS( victim ) * 3
		//    : (GET_AC( victim ) / 3 ));
	}
	else
	{
		if ( arrow || ammo )  // 加入 槍械射擊 的判定 2022/05/01
		{
			//int nEnemyAC = GET_AC(victim);
			if(GET_AC(victim) >= -480){
				dam = dam * (1000 + GET_AC(victim) ) / 480;
			}else{
				dam = dam * 480 / (0 - GET_AC(victim));
			}
		}else{
			// 加入 powerblow 降低對手 ac 一定比例的判定 2021/12/25
			int nPbAc = 0;

			if( isPowerBlow && GET_AC(victim) < 0 && ( ( isBloodThirsty && number_percent() < 35 ) || number_percent() < 20 ) ){
				nPbAc = GET_AC(victim) * ( 100 - (5400/(54 + get_skill_level(ch, gsn_powerblow) * get_skill_level(ch, gsn_powerblow) ) ) );
				nPbAc = nPbAc / 100;
			}

			int nEnemyAC = GET_AC(victim) - nPbAc;
			if(nEnemyAC > 0){
				dam = dam + dam * UMIN( GET_AC(victim)/100 , 1); // ac 為正值傷害增加最高一倍
			}
			else
			{
				if(nEnemyAC < -7000){
					dam = dam + nEnemyAC + 7000 - dam * 4 / 5; // ac -7001 以上每 ac 少 1 傷害
				}
				else if(nEnemyAC >= -2000  ){
					dam = UMIN( dam + dam * nEnemyAC/8000 , dam + nEnemyAC/7 ); // ac -2000 傷害減少約 25%
				}
				else if(nEnemyAC >= -4000){
					dam = dam + dam * (nEnemyAC + 2000)/8500 - dam/4; // ac -4000 減少約 48.5%
				}
				else {
					dam = dam + dam * (nEnemyAC + 4000)/9000 - (dam * 20)/85 - dam/4; // ac -7000 減少約 81% , -6000 減少約 70%
				}
			}
			/*
			dam = dam + UMIN( 0, (GET_AC( victim ) / 7 ) );

			int nEnemyAC = GET_AC(victim);
			if(nEnemyAC <= -750){
				dam = dam * 2000 / (1250 - nEnemyAC);
			}else{
				dam = dam * (1750 + nEnemyAC) / 1000;
			}
			*/
		}
	}

	/* mana shield 移到 fight_damage 中判定 2023/01/09

		// 魔力屏障 的傷害減輕處理
		if ( IS_AFFECTED( victim, AFF_MANA_SHIELD ) && dam > 0)
		{
			int nMsSklv      = get_skill_level(victim, gsn_mana_shield);
			int nDmgToMpRate = 5 + nMsSklv * 3 / 2;
			int nBlkDmg      = UMIN(dam, victim->mana * nDmgToMpRate);

			if(nBlkDmg < 0) nBlkDmg = 0;

			dam -= nBlkDmg * (nMsSklv + 6) / 12;

			nBlkDmg += (ch->apdr - victim->pdr);

			nBlkDmg = GetDamageUnderProtection(ch, victim, nBlkDmg);

			if(nBlkDmg < 0) nBlkDmg = 0;

			victim->mana -= 0 + nBlkDmg / nDmgToMpRate;

			if(victim->mana <= nDmgToMpRate){
				victim->mana = 0;
				affect_strip( victim, gsn_mana_shield );
			}
		} */

    //Weak point for emp and circular , bash head add at 20.01.24
	// add frenzy 2020/10/29
    if( dt == gsn_empslash
		|| dt == gsn_circular
		|| dt == gsn_bashhead
		|| dt == gsn_frenzy )
    {
        dam = dam * weakness_damage(victim, dt) / 100; // by amethyst
    }

    if ( dam <= 0 )
    {
        dam = 0;
    }
	
	// 普擊 及 weapon type weakness_damage 傷害弱點個別設定 add at 2020/09/18
	// weapon master 對 type 弱點增加傷害 2020/10/14

	if( dt == 1000 ) // 空手攻擊
		dam = dam * weakness_damage(victim , gsn_hit_attack) / 100;
	if( wield && wield->value[3] == 1 )  // 銳利切割
	{
		if( weakness_damage(victim , gsn_slice_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_slice_attack)
						+ (weakness_damage(victim , gsn_slice_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_slice_attack) / 100;
	}
	if( wield && wield->value[3] == 2 )  // 威猛一戳
	{
		if( weakness_damage(victim , gsn_stab_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_stab_attack)
						+ (weakness_damage(victim , gsn_stab_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_stab_attack) / 100;
	}
	if( wield && wield->value[3] == 3 )  // 揮舞劈砍
	{
		if( weakness_damage(victim , gsn_slash_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_slash_attack)
						+ (weakness_damage(victim , gsn_slash_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_slash_attack) / 100;
	}
	if( wield && wield->value[3] == 4 )  // 灼熱一鞭
	{
		if( weakness_damage(victim , gsn_whip_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_whip_attack)
						+ (weakness_damage(victim , gsn_whip_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_whip_attack) / 100;
	}
	if( wield && wield->value[3] == 5 )  // 用力一抓
	{
		if( weakness_damage(victim , gsn_claw_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_claw_attack)
						+ (weakness_damage(victim , gsn_claw_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_claw_attack) / 100;
	}
	if( wield && wield->value[3] == 6 )  // 爆發噴擊
	{
		if( weakness_damage(victim , gsn_blast_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_blast_attack)
						+ (weakness_damage(victim , gsn_blast_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_blast_attack) / 100;
	}
	if( wield && wield->value[3] == 7 )  // 萬鈞力搥
	{
		if( weakness_damage(victim , gsn_pound_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_pound_attack)
						+ (weakness_damage(victim , gsn_pound_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_pound_attack) / 100;
	}
	if( wield && wield->value[3] == 8 )  // 狠狠碾壓
	{
		if( weakness_damage(victim , gsn_crush_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_crush_attack)
						+ (weakness_damage(victim , gsn_crush_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_crush_attack) / 100;
	}
	if( wield && wield->value[3] == 9 )  // 犀利一剁
	{
		if( weakness_damage(victim , gsn_grep_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_grep_attack)
						+ (weakness_damage(victim , gsn_grep_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_grep_attack) / 100;
	}
	if( wield && wield->value[3] == 10 ) // 張口猛咬
	{
		if( weakness_damage(victim , gsn_bite_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_bite_attack)
						+ (weakness_damage(victim , gsn_bite_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_bite_attack) / 100;
	}
	if( wield && wield->value[3] == 11 ) // 尖銳刺擊
	{
		if( weakness_damage(victim , gsn_pierce_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_pierce_attack)
						+ (weakness_damage(victim , gsn_pierce_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_pierce_attack) / 100;
	}
	if( wield && wield->value[3] == 12 ) // 猛烈吸吮
	{
		if( weakness_damage(victim , gsn_suction_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_suction_attack)
						+ (weakness_damage(victim , gsn_suction_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_suction_attack) / 100;
	}
	if( wield && wield->value[3] == 13 ) // 剁菜連斬
	{
		if( weakness_damage(victim , gsn_chop_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_chop_attack)
						+ (weakness_damage(victim , gsn_chop_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_chop_attack) / 100;
	}
	// 射出箭矢 及 槍械射擊 , 此時的 wield 分別為 arrow 及 ammo , 原 wield 為 bow 及 gun 2022/05/01
	if( ( bow && bow->value[3] == 14 && arrow ) || ( gun && gun->value[3] == 16 && ammo ) )
	{
		if( weakness_damage(victim , gsn_shot_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_shot_attack)
						+ (weakness_damage(victim , gsn_shot_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_shot_attack) / 100;
	}


    //傷害計算結束!!!!

    //送出傷害     Modified by Razgriz 20060705
	if (     msg_mode == MSG_NORMAL 
		|| ( msg_mode == MSG_BRIEF && !is_brief ( ch, victim ) ) )
	{
		// 將 reposte dmg_mode 修改為 DMG_ADJUST 以判定 pdb 2021/11/16
		//if ( dt == gsn_empslash )
		if ( dt == gsn_empslash || isReposte )
		{
			////_damage( ch, victim, dam , dt, wpn );
			damage( ch, victim, dam, dt, wpn, MSG_NORMAL, DMG_ADJUST );
		}
        else
        {
			////damage( ch, victim, dam , dt, wpn );
			damage( ch, victim, dam, dt, wpn, MSG_NORMAL, DMG_NORMAL );
        }
	}
	else
	{
		// 將 reposte 修改為 DMG_ADJUST , 並移除 arrow 在brief 為 DMG_ADJUST 的設定 2021/11/16
		//if( dt == 1014 || dt == gsn_empslash )
		if( dt == gsn_empslash || isReposte )
		{
			////dam = brief__damage( ch, victim, dam, dt, wpn );
			dam = damage( ch, victim, dam, dt, wpn, MSG_BRIEF, DMG_ADJUST );
		}
		else
		{
			////dam = brief_damage( ch, victim, dam, dt, wpn );
			dam = damage( ch, victim, dam, dt, wpn, MSG_BRIEF, DMG_NORMAL );
		}
	}

	// 處理 mob 死前攻擊的問題  2022/02/24
	// mob death_prog 中的攻擊指令被 reposte 觸發 hp 會出現無限連段的問題
	// 當前作法為 mob pos_dead 的情況下, 直接阻斷普擊 hp 觸發
	// 加入 hit <= 0 時也會 return 的判定 2022/12/30
	if( IS_NPC(victim) && ( victim->position == POS_DEAD || victim->hit <= 0 ) )
	{
		// 如果 mob 目標死亡, 則扣除彈藥 2023/01/04
		if( isShootCost )
		{
			if( arrow )
				ShotObjCount( ch , arrow );
			if( ammo )
				ShotObjCount( ch , ammo );
		}
		return dam;
	}

	// 處理 彈藥 ammo 的附加法術 2022/05/01
	if( ammo && ammo->value[3] > 0 && ammo->value[3] < MAX_SKILL )
	{
		if ( skill_table[ammo->value[3]].target == TAR_CHAR_SELF && victim != ch ){
			obj_cast_spell( ammo->value[3], ammo->value[2], ch, ch, ammo );
		}else if( skill_table[ammo->value[3]].target == TAR_IGNORE ){
			if (   ammo->value[3] == 314
				|| ammo->value[3] == 315
				|| ammo->value[3] == 316
				|| ammo->value[3] == 317
				|| ammo->value[3] == 318
				|| ammo->value[3] == 319
				|| ammo->value[3] == 320
				|| ammo->value[3] == 321 )
				obj_cast_spell( ammo->value[3], ammo->value[2], ch, ch, ammo );
			else
				obj_cast_spell( ammo->value[3], ammo->value[2], ch, ch, ammo );
		}else if( skill_table[ammo->value[3]].target == TAR_ARG ){
			obj_cast_spell( ammo->value[3], ammo->value[2], ch, ch, ammo );
		}else{
			obj_cast_spell( ammo->value[3], ammo->value[2], ch, victim, ammo );
		}
	}

    //處理 artifact
    if ( !ch->deleted && !victim->deleted && ch->in_room == victim->in_room )
    {
		/*武器有 闇黑 效應的處理
		if( wield&&  IS_SET(wield->extra_flags, ITEM_DARK))
		{
		  switch(number_percent())
		  {
			  case 5:
				  trigger_obj_hp( wield, skill_lookup("lifesteal"), wield->level, ch, victim);
				  break;
			  case 26:
				  trigger_obj_hp( wield, skill_lookup("soulsteal"), wield->level, ch, victim);
				  break;
		  }
		} */

		chance = 0;

		// frenzy 發數降低 1/3 , 給予額外 artifact 加成 add at 2020/10/20
		if(dt == gsn_frenzy) chance += 25;

		//對於 背刺 跟 繞刺 給予 artifact 成功機率的加成
		if(dt == gsn_backstab || dt == gsn_circle) chance += 90;
		if(dt == gsn_empslash || dt == gsn_sprinkle_slash) {
			int nEmpSklv = get_skill_level(ch, gsn_empslash);
			chance += 20 + (nEmpSklv + 4) * nEmpSklv;
		}
		if(dt == gsn_circular) {
			int nCirSklv = get_skill_level(ch, gsn_circular);
			chance += 10 + (nCirSklv + 4) * nCirSklv;
		}
		if(isReposte) chance += 80;
		int nCurrWis = get_curr_wis(ch);
		nCurrWis += get_skill_level(ch, gsn_artifact);
		if (nCurrWis > 25) nCurrWis = 25;


		// 對 LanceMaster 的加成
		/*int nLmBonus = GetBonusedValue_LanceMaster(ch, 100);
		nCurrWis = nCurrWis * nLmBonus / 100;
		chance += (nLmBonus - 100) / 100; */
		int nLmBonus = 0;
		nCurrWis = nCurrWis * GetBonusedValue_LanceMaster(ch, 100) / 100;
		nLmBonus = (GetBonusedValue_LanceMaster(ch, 100) - 100) / 100;

		// 對 TrickyPacing 的加成
		/*int nTpBonus = TrickyPacingBase(ch, 100);
		if( arrow ){
			nCurrWis = nCurrWis * nTpBonus / 150;
			chance += (nTpBonus - 100) / 150;
		}else{
			nCurrWis = nCurrWis * nTpBonus / 100;
			chance += (nTpBonus - 100) / 100;
		}*/
		int nTpBonus = 0;
		if( arrow ){
			if( nTpSklv > 4 ){
				nCurrWis = nCurrWis * TrickyPacingBase(ch, 100) / 150;
				nTpBonus = (TrickyPacingBase(ch, 100) - 100) / 150;
			}
		}else{
			nCurrWis = nCurrWis * TrickyPacingBase(ch, 100) / 100;
			nTpBonus = (TrickyPacingBase(ch, 100) - 100) / 100;
		}


		// 處理 箭矢 的 artifact
		if( arrow )
		{
			// 增加 obj_hit 的判定 2022/10/23
			// 增加 ch 不被 force 才會成立 避免無限連擊 2022/11/02
			if ( arrow->pIndexData->progtypes & OBJ_HIT && !ch->forced )
				oprog_hit_trigger(arrow);

			// tricky pacing 額外的 bonus 2022/12/03
			if( !IS_NPC(ch) )
			{
				chance += nTpBonus;
			}
			
			OBJ_HP * ohp = arrow->hp;
			for( ; ohp; ohp = ohp->next ) {
				if ( IS_SET( ohp->event, HP_ARROW) ) { // HP_ARROW 為射擊命中觸發
					if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
						trigger_obj_hp(arrow, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
					}
				}
			}
		}
		// 處理 槍彈 的 artifact
		else if( ammo )
		{
			// 增加 obj_hit 的判定 2022/10/23
			// 增加 ch 不被 force 才會成立 避免無限連擊 2022/11/02
			if ( ammo->pIndexData->progtypes & OBJ_HIT && !ch->forced )
				oprog_hit_trigger(ammo);
			OBJ_HP * ohp = ammo->hp;
			for( ; ohp; ohp = ohp->next ) {
				if ( IS_SET( ohp->event, HP_ARROW) ) {
					if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
						trigger_obj_hp(ammo, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
					}
				}
			}
		}
		// 處理 武器 的 artifact
		//else if ( wield && ( wield->hp || wield->material->hp) )
		else if ( wield ) // 增加 obj_hit 判定 , 把 wield 與 hp 判定分開寫 2022/10/23
		{
			// 增加 ch 不被 force 才會成立 避免無限連擊 2022/11/02
			if ( wield->pIndexData->progtypes & OBJ_HIT && !ch->forced )
				oprog_hit_trigger(wield);

			// lance mastery 及 tricky pacing 額外的 bonus 2022/12/03
			if( !IS_NPC(ch) )
			{
				if( nLmPerc > 0 && IsUsingKnightWeapon(ch , wield) )
					chance += nLmBonus;
				if( nTpPerc > 0 && IsUsingTrickyWeapon(ch , wield) )
					chance += nTpBonus;
			}

			if ( wield->hp || wield->material->hp)
			{
				OBJ_HP * ohp = wield->hp;
			
				//Weapon Master 等級對武器戰鬥觸發的 hp 等級加成	
				int WmArtLv = 0;
				WmArtLv += get_skill_level(ch,gsn_weapon_master);
				if (get_skill_level(ch,gsn_weapon_master) > 3) WmArtLv += 2;
				if (get_skill_level(ch,gsn_weapon_master) > 4) WmArtLv += 3;
				if (get_skill_level(ch,gsn_weapon_master) > 5) WmArtLv += 4;

				//武器本身的 artifact 
				for( ; ohp; ohp = ohp->next )
				{
					if( IS_SET( ohp->event, HP_FIGHT ) )
					{
						if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
							trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, ch, victim);
						}
					}

					if( IS_SET( ohp->event, HP_WEAPON ) ) {
						if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
							trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, ch, victim);
						}
					}
				}

				//武器材質的 artifact
				if( wield->material->hp )
				{
					for( ohp=wield->material->hp ; ohp; ohp = ohp->next )
					{
						if( IS_SET( ohp->event, HP_FIGHT ) )
						{
							if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
								trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, ch, victim);
							}
						}

						if( IS_SET( ohp->event, HP_WEAPON ) ) {
							if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
								trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level + WmArtLv, ch, victim);
							}
						}
					}
				}
			}
		}

		// 沒穿武器,處理第一手套的 artifact
		gloves = get_eq_char( ch, WEAR_HANDS );
		wield = get_eq_char( ch, WEAR_WIELD_2 );
		// 增加 obj_hit 的判定 2022/10/23
		// 增加 ch 不被 force 才會成立 避免無限連擊 2022/11/02
		if ( gloves && gloves->pIndexData->progtypes & OBJ_HIT && !ch->forced )
			oprog_hit_trigger(gloves);

		if( gloves && (gloves->hp || gloves->material->hp)
			&& !(get_eq_char( ch, WEAR_WIELD ) || get_eq_char( ch, WEAR_TWO_HAND )) )
		{
			if( !wield || (wield && wield->value[3] != 0) )
			{
				if(dt == 1000 || dt == gsn_frenzy || dt == gsn_circular || dt == gsn_bashhead) // 加入bashhead 2020/11/01
				{
					OBJ_HP * ohp = gloves->hp;
					// 手套耐久耗損
					if(number_bits(5) < 8) waste_dura(gloves);

					//手套本身的 hp 
					for( ; ohp; ohp = ohp->next )
					{
						if( IS_SET( ohp->event, HP_FIGHT ) )
						{
							if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
								trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
						if( IS_SET( ohp->event, HP_WEAPON ) )
						{
							if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
								trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
					}
					//手套材質的 hp
					if( gloves->material->hp )
					{
						for( ohp=gloves->material->hp ; ohp; ohp = ohp->next )
						{
							if( IS_SET( ohp->event, HP_FIGHT ) )
							{
								if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent())
								{
									trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
								}
							}
							if( IS_SET( ohp->event, HP_WEAPON ) )
							{
								if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent())
								{
									trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
								}
							}
						}
					}
				}
			}
		}
		// 沒穿武器,處理第二手套的 artifact (beholder專用)
		gloves = get_eq_char( ch, WEAR_HANDS_2 );
		// 增加 obj_hit 的判定 2022/10/23
		// 增加 ch 不被 force 才會成立 避免無限連擊 2022/11/02
		if ( gloves && gloves->pIndexData->progtypes & OBJ_HIT && !ch->forced )
			oprog_hit_trigger(gloves);

		if( gloves && (gloves->hp || gloves->material->hp)
			&& (dt == 1000 || dt == gsn_frenzy || (dt == gsn_bashhead && !get_eq_char( ch, WEAR_HANDS ))) ) // bashhead 副手套判定 2020/11/01
		{
			OBJ_HP * ohp = gloves->hp;
			// 手套耐久耗損
			if(number_bits(5) < 8) waste_dura(gloves);

			//手套本身的 hp 
			for( ; ohp; ohp = ohp->next )
			{
				if( IS_SET( ohp->event, HP_FIGHT ) )
				{
					if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
						trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
					}
				}
				if( IS_SET( ohp->event, HP_WEAPON ) ) {
					if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
						trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
					}
				}
			}
			//手套材質的 hp
			if( gloves->material->hp )
			{
				for( ohp=gloves->material->hp ; ohp; ohp = ohp->next )
				{
					if( IS_SET( ohp->event, HP_FIGHT ) )
					{
						if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
							trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
					if( IS_SET( ohp->event, HP_WEAPON ) ) {
						if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
							trigger_obj_hp(gloves, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
				}
			}
		}
	}

	/* bashhead stun 判定  2020/11/01 */

	// 傷害大於 0 且 目標不為 stun 或 半死 狀態 (避免連續 stun)
	if ( dam > 0 && victim->position > POS_STUNNED && ( dt == gsn_bashhead ) ){
		int BashStun = 0;
		// berserk 狀態下額外加成
		if( is_affected(ch, gsn_berserk ) && get_skill_level(ch, gsn_berserk) > 2 )
			BashStun += get_skill_level(ch, gsn_berserk) - 2;
		// powerup 狀態下額外加成
		if( is_affected(ch, gsn_powerup) && get_skill_level(ch, gsn_powerup) > 2 )
			BashStun += get_skill_level(ch, gsn_powerup) - 2;
		// 徒手搏鬥狀態下 str 高 則機率較高
		if( IS_SET( ch->act, PLR_FIST_FIGHT) )
			BashStun += get_curr_str( ch ) - get_curr_con( victim ) / (1 + get_skill_level(ch, gsn_bashhead));
		else
			BashStun += (get_curr_str( ch ) - 3) - get_curr_con( victim ) / (1 + get_skill_level(ch, gsn_bashhead));
		if ( BashStun > number_percent() * weakness_percent(victim, gsn_bashhead) / 100 ){
			victim->position = POS_STUNNED;
			WAIT_STATE( victim, skill_table[gsn_bashhead].beats );
			send_to_char( "你對他的頭部猛擊，把他打暈了！\n\r", ch );
			act( "$n 往 $N 頭擊猛擊把他打暈了！", ch, NULL, victim, TO_ROOM );
			act( "$n 朝你頭部狠狠猛擊，打得你天旋地轉！", ch, NULL, victim, TO_VICT );
		}
	}

	// damage 中會用到 arrow 或 ammo 故最後才計算數量扣除 2023/01/04
	if( isShootCost )
	{
		if( arrow )
			ShotObjCount( ch , arrow );
		if( ammo )
			ShotObjCount( ch , ammo );
	}

	tail_chain( );
	return dam;
}



	/*
	 * Hit one guy once.
	 */
	 
int one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn )
{

	CHAR_DATA *protector;
	int gundt = -1;

	if(!check_can_do_hit(ch,victim))
		return -2;

	protector = victim->protector;
	if( protector && protector != victim ) {
		for( ; protector  && protector != protector->next_in_protect ; protector = protector ->next_in_protect ) {
			if( protector != ch && protector != victim && ch->in_room 
				&& protector->in_room == ch->in_room ) {
				if(   ( IS_NPC(protector) && (protector->level + GetGeneralMobSklv(protector)* 24)/2 > number_percent())
					||(!IS_NPC(protector) && get_skill_percent(protector, gsn_protect)
						+ get_skill_level(ch, gsn_protect) * 4 > number_percent())){
					//  modify by jye
					//  修改為不stop_fighting
					//  因為stop_fighting會將ch之cast去掉
					ch->fighting = NULL;
					stop_fighting( protector, FALSE );
					set_fighting( ch, protector );
					set_fighting( protector, ch );
					act( "$n奮不顧身地保護$N！", protector, NULL, victim, TO_ROOM );
					act( "你奮不顧身地保護$N！", protector, NULL, victim, TO_CHAR );
					victim = protector;
					break;
				}else{
					act( "$n想要保護$N，可惜失敗了！", protector, NULL, victim, TO_ROOM );
					act( "你想要保護$N，可惜失敗了！", protector, NULL, victim, TO_CHAR );
				}
			}
		}
	}

	// 槍械射擊 成立時, 轉變 skill 的 dt 2022/05/01
	if( IsUsingGunShot(ch , wpn ) ){
		return mone_hit ( ch, victim, &gundt, wpn, MSG_BRIEF, DT_CHANGE );
	}else if( dt == TYPE_HIT && is_brief( ch, victim ) ){
		return mone_hit ( ch, victim, &dt, wpn, MSG_BRIEF, DT_CHANGE );
	}else{
		return mone_hit ( ch, victim, &dt, wpn, MSG_NORMAL, DT_NOCHANGE );
	}
}
