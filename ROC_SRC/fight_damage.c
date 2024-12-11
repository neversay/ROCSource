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
//*  PROGRAM ID  : fight_damage.c                                 *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940611 jye                                    *
//*  PURPOSE     :         修改met之目標有被pro時變slash之bug     *
//*              :         及ber之目標有被pro時會被取消之bug      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941013 Razgriz                                *
//*  PURPOSE     :         damage () 加上中文註解                 *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941014 Razgriz                                *
//*  PURPOSE     :         改寫 damage () 介面                    *
//*              :         移除 _damage brief_damage brief__damage*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0981011 Keelar                                *
//*  PURPOSE     :         強化舊區域NPC                    *
//*              :         移除個別 half_damage 所有 NPC 都將有 half_damage 效果*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991207 Tale                                *
//*  PURPOSE     :         移除 half_damage 回復初始設定                   *
//*              :         移除 half_damage 且回復個別 half_damage效果*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1000410 Tale                                *
//*  PURPOSE     :         提升魔法傷害倍率                  *
//*              :         提升玩家魔法傷害倍率 10 / 7 倍*
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
extern bool check_race_special   args( ( CHAR_DATA *ch ) );
extern bool check_evade args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
//extern bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff ) );
extern bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn ) ); // 2022/12/04
extern bool check_block args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff ) );
extern bool check_tactics args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
extern bool frenzy_check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);  /* 20/01/20 */

extern void death_cry args( ( CHAR_DATA *ch ) );
extern void group_gain args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
extern int  xp_compute args( ( CHAR_DATA *gch, CHAR_DATA *victim ) );
extern bool is_wielding_poisoned args( ( CHAR_DATA *ch, int wpn ) );
extern void make_corpse args( ( CHAR_DATA *killer, CHAR_DATA *ch ) );
extern int artifact( CHAR_DATA *ch);

extern void check_position( CHAR_DATA *ch, CHAR_DATA *victim, int dam );
extern void check_wimpout(CHAR_DATA *ch, CHAR_DATA *victim);
extern bool check_reflexion(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam, int dexdiff);
extern int check_damage_adjust(CHAR_DATA *ch, int dam);
extern int check_spellfury(CHAR_DATA *ch, CHAR_DATA *victim, int dam);
extern int check_dirty_raid(CHAR_DATA *ch, CHAR_DATA *victim, int dam);

extern bool IsPK(CHAR_DATA *ch, CHAR_DATA *victim);
/*
 * Local functions.
 */
extern void dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
extern void c_dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
extern int one_hit args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn ) );
extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode );
//extern int   brief_mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int* dt, int wpn );

void  use_magical_item     args( ( CHAR_DATA *ch ) );
void  track_hunt       args( (CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door ) );
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

// 攻擊命中的元素判定 2022/12/25
bool IsSpellElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsWindElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsFireElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsColdElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsEarthElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsLightningElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsSaintElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsDarkElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );

// 毒襲修改為攻擊成功觸發才追加傷害 2021/11/04
//void VenomousStrike(CHAR_DATA *pCh, CHAR_DATA *pVictim)
// 增加物品判定 2021//11/10
//int VenomousStrike(CHAR_DATA *pCh, CHAR_DATA *pVictim)
int VenomousStrike(CHAR_DATA *pCh, CHAR_DATA *pVictim, int wpn)
{
	AFFECT_DATA af;
	AFFECT_DATA *paf; // 2021/11/10
	OBJ_DATA *obj; // 2021/11/10
	static int nVenomSN = 0;

	if(nVenomSN == 0) nVenomSN = skill_lookup("venomous strike");
	int nVenomPerc = get_skill_percent(pCh, nVenomSN);
	int nVenomSklv = get_skill_level(pCh, nVenomSN);
	int nMaxDmg    = 0;
	int nCurDmg    = 0;
	int nDamage    = 0;
	int nVictimCon = get_curr_con(pVictim);
	int nToxicity  = 0; // 2021/11/10
	int nToxicityTemp = 0; // 2021/11/11
	int nPoisonSklv = get_skill_level(pCh, gsn_poison); // 2021/11/10

	obj = get_eq_char( pCh, wpn );

	if( !obj ) return nCurDmg;

	// 避免自帶劇毒的物品過弱 , 給予基本毒性 2021/11/10
	if( obj->item_type == ITEM_WEAPON )
		nToxicity += number_range( obj->value[1], obj->value[2] ) / 2;
	if( obj->item_type == ITEM_ARROW || obj->item_type == ITEM_DART )
		nToxicity += number_range( obj->value[1], obj->value[2] ) / 4;
	if( nToxicity < 0 ) nToxicity = 0;

	// 判定擊中目標的物品本身的劇毒性 2021/11/10
	for ( paf = obj->affected; paf; paf = paf->next )
	{
		nToxicityTemp = paf->modifier;
		if ( paf->location == APPLY_TOXICITY_POISON ){
			nToxicity += nToxicityTemp;
		}
	}
	for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		nToxicityTemp = paf->modifier;
		if ( paf->location == APPLY_TOXICITY_POISON ){
			nToxicity += nToxicityTemp;
		}
	}
	for ( paf = obj->material->affected; paf; paf = paf->next )
	{
		nToxicityTemp = paf->modifier;
		if ( paf->location == APPLY_TOXICITY_POISON ){
			nToxicity += nToxicityTemp;
		}
	}

	if( nVenomPerc <= 0 || pCh == pVictim ){
		//if(saves_spell( pCh->level, pVictim ) + 20 < number_percent( )){
		// 目標 svs 0  有 1.32% 機會上poison , 每 sklv + 1%  2021/11/05
		// 物品的毒性影響中毒機率且增加中毒傷害 2021/11/10
		if((nPoisonSklv + nToxicity/30 + new_saves_spell( pCh->level , pVictim, MAGIC_SPELL)/200)
			> number_percent( )){
			// 物品的毒性 ,目標體質 及 poison sklv 影響傷害 2021/11/10
			nCurDmg = (pCh->level + nToxicity) * (38 - nVictimCon) / 20;
			if ( nCurDmg < 0 ) nCurDmg = 0;
			// wp poison 上限 2020/10/29
			if( affected_value(pVictim, gsn_poison, APPLY_SAVING_POISON) < (nToxicity + (14 + nPoisonSklv*6)*10) ){
				af.type      = gsn_poison;
				af.duration  = 1;
				af.location  = APPLY_SAVING_POISON;
				af.modifier  = nCurDmg;
				af.bitvector = AFF_POISON;
				affect_join( pVictim, &af );
			}
			// 追加敘述 2021/11/11
			act( HIG"你武器上的毒素侵入他的體內！"NOR"", pCh, NULL, pVictim, TO_CHAR );
			act( HIG "噁心感與刺痛感同時襲來，對方的武器似乎有毒！"NOR"" , pCh, NULL, pVictim, TO_VICT);
			act( "$N"HIG"臉色略為泛青，武器的毒素侵入了他的體內！"NOR"", pCh, NULL, pVictim, TO_NOTVICT );
		}
	}else{
		//int nVictimCon = get_curr_con(pVictim);
		// 縮小百分比影響 , 放大 sklv 影響 , 加入物品毒性影響 2021/11/10
		nMaxDmg = ( nToxicity + nVenomPerc/2 + nVenomSklv * nVenomSklv*5 ) * (38 - nVictimCon) / 20;
		if(nMaxDmg < 0) nMaxDmg = 0;
		//nMaxDmg = nMaxDmg * new_saves_spell( 10 + nVenomSklv * (nVenomSklv + 4), pVictim, MAGIC_SPELL) / 100;
		//nMaxDmg = nMaxDmg * weakness_damage(pVictim, gsn_poison) / 100;

		// 加入 weakness 命中判定, 裝備毒性會影響成功率 2021/11/04
		if( (nMaxDmg * weakness_percent(pVictim, gsn_poison) / 100) < number_percent() * (14 - nVenomSklv) )
			return nCurDmg;

		nMaxDmg = nMaxDmg * new_saves_spell( nVenomSklv * (nVenomSklv + 4), pVictim, MAGIC_SPELL) / 100;

		nCurDmg = affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON);

		nDamage = nMaxDmg / 7;
		nCurDmg += nDamage;
		if(nCurDmg > nMaxDmg) nCurDmg = nMaxDmg;
		
		act( HIG"你運用技巧將武器的毒素注入他的體內！"NOR"", pCh, NULL, pVictim, TO_CHAR );
		act( HIG "你突然感到身體不適，對方的武器似乎有毒！"NOR"" , pCh, NULL, pVictim, TO_VICT);
		if( !is_affected(pVictim, gsn_venomous_strike) )
			act( "$N"HIG"表情變得有些苦澀，似乎受到了詭異的攻擊！"NOR"", pCh, NULL, pVictim, TO_NOTVICT );
		
		// 低數值的毒攻擊不會疊過較高的 2021/11/04
		if(affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON) > nCurDmg)
			nCurDmg = affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON);
		
		af.type      = gsn_venomous_strike;
		af.duration  = 1;
		af.bitvector = AFF_POISON;
		af.location  = APPLY_SAVING_POISON;
		af.modifier  = nCurDmg; // 加上裝備毒性 2021/11/10
		affect_strip(pVictim, gsn_venomous_strike);
		affect_join( pVictim, &af );

		// 毒襲的傷害計算 2021/11/04
		nCurDmg = (nCurDmg * (1 + nVenomSklv*4) )/10;
		// 傷害加入目標當前毒素值 2022/04/23
		if( NEW_GET_SVS(pVictim,MAGIC_POISON) >= 0 )
			nCurDmg = nCurDmg + (5 * NEW_GET_SVS(pVictim,MAGIC_POISON))/(10 - nVenomSklv);
		else
			nCurDmg = nCurDmg + NEW_GET_SVS(pVictim,MAGIC_POISON)/(1 + nVenomSklv/6);
		// 傷害加上裝備毒性 2021/11/10
		nCurDmg = nCurDmg * weakness_damage(pVictim, gsn_poison) / 100;
	}
	// 避免負值
	if( nCurDmg < 0 ) nCurDmg = 0;
	return nCurDmg;
}

// 戰後屍體自動處理判定 2022/05/08
void AutoCorpse( CHAR_DATA *ch , CHAR_DATA *victim , char *vchname )
{
	int split = 0;
	char buf[10];
	char cbuf[MAX_INPUT_LENGTH];
	OBJ_DATA *corpse;

	if( !ch ) return;

	if( !victim ) return;

	corpse = get_obj_list( ch, vchname, ch->in_room->contents );

	if( !corpse ) return; // 死前 purge 沒有屍體的情況就 return

	sprintf(cbuf, "coins 'corpse %s'", victim->name);

	/* Autogold by Morpheus */
	if ( IS_SET( ch->act, PLR_AUTOGOLD ) )
		split = do_autoget( ch, cbuf );  /* autogold mod by Canth */

	if( IS_SET(ch->act, PLR_AUTOSPLIT) && split > 100)
	{
		sprintf(buf,"%d", split);
		do_split(ch, buf);
	}

	sprintf(cbuf, "all 'corpse %s'", victim->name);

	if ( IS_SET( ch->act, PLR_AUTOLOOT ) )
	{
		do_get_silence( ch, cbuf );
	}
	else
	{
		sprintf(cbuf, "in 'corpse %s'", victim->name);
		do_look( ch, cbuf );
	}

	sprintf(cbuf, "'corpse %s'", victim->name);
	if ( IS_SET( ch->act, PLR_AUTOSAC  ) )
	{
		do_sacrifice( ch, cbuf );
	}
	return;
}

int LifeShareDamage(CHAR_DATA *ch, CHAR_DATA *och,int dam )
{
	CHAR_DATA *vch;
	int LSDam;

	if( ch->in_room && IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
		return dam;

	vch = ch->linking;
	
	if( vch->linking == NULL )
		return dam;

	if( vch->in_room != ch->in_room )
		return dam;

	if( ch->linking != vch || vch->linking != ch )
		return dam;

	if( get_skill_level( ch , gsn_lifeshare ) > get_skill_level( vch , gsn_lifeshare ) )
		LSDam = dam/( 5 + get_skill_level( ch, gsn_lifeshare)/2 );
	else
		LSDam = dam/( 5 + get_skill_level( vch , gsn_lifeshare)/2 );
	
	dam = dam - dam/5;
	// ch 自殘的情況下
	if ( ch == och )
		och = vch;
	if ( LSDam > 0 ){
		dam_message( vch, vch, LSDam, gsn_lifeshare, WEAR_NONE );
		damage( och, vch, LSDam, gsn_lifeshare, WEAR_NONE, MSG_BRIEF, DMG_ADJUST );
	}
	return dam;
}

int IsRangedAttack(int nDmgType)
{
	// 加入槍械射擊 , 暗器投擲 , 炸藥投擲 2022/04/29
	//if(nDmgType == 1014 || nDmgType == 1015 || nDmgType == gsn_throwing ){
	if(nDmgType == 1014
	|| nDmgType == 1015
	|| nDmgType == 1016 // gun shoot
	|| nDmgType == gsn_throwing // 一般投擲物品
	|| nDmgType == gsn_dart_mastery // coin-dart
	|| nDmgType == gsn_artifacts_mastery){
		return 1;
	}
	return 0;
}

int IsMeleeSpell(int nDmgType)
{
    static int  s_rgMsSn[8] = {0};
    static char s_szMsName[8][30] = 
            {"flaming slash", "storm slash", "earth slash", "icecore slash", 
             "fulgurant slash", "virtual swords", "sun slash", "sprinkle slash"};
    int x;
    for(x = 0; x < 8; x++){
        if(s_rgMsSn[x] == 0){
           s_rgMsSn[x] = skill_lookup(s_szMsName[x]);
        }
        if(nDmgType == s_rgMsSn[x]) return 1;
    }
    return 0;
}

int GetPhysicalDamageUnderRiding(CHAR_DATA *pVictim, int nDamage, int nDmgType)
{
  if (get_eq_char( pVictim , WEAR_RIDE ) == NULL ||
      !IS_SET( race_table[ pVictim->race ].race_abilities, RACE_CENTAUR )){
    return nDamage;
  }
  int nRideSklv = get_skill_level(pVictim, gsn_ride);
  int nDmgRate  = 100;

  if (IS_SET( race_table[ pVictim->race ].race_abilities, RACE_CENTAUR )) {
    nRideSklv = get_skill_level(pVictim, gsn_inborn_ability);
  }

  // shot type damage
  if(IsRangedAttack(nDmgType)){
    nDmgRate  = 96 - nRideSklv;
  }else{
    nDmgRate  = 116 - nRideSklv;
  }

  return nDamage * nDmgRate / 100;
}

int GetMagicalDamageUnderRiding(CHAR_DATA *pVictim, int nDamage, int nDmgType)
{
  if (get_eq_char( pVictim , WEAR_RIDE ) == NULL ||
      !IS_SET( race_table[ pVictim->race ].race_abilities, RACE_CENTAUR )){
    return nDamage;
  }
  int nRideSklv = get_skill_level(pVictim, gsn_ride);
  int nDmgRate  = 100;
  
  if (IS_SET( race_table[ pVictim->race ].race_abilities, RACE_CENTAUR )) {
    nRideSklv = get_skill_level(pVictim, gsn_inborn_ability);
  }
  
  if(!IsMeleeSpell(nDmgType)){
      nDmgRate = 116 - nRideSklv;
  }else{
      nDmgRate = 96 - nRideSklv;
  }
 
  return nDamage * nDmgRate / 100;
}

// 修改 protection 不同陣營效果 2022/10/26
int GetDamageUnderProtection(CHAR_DATA *ch, CHAR_DATA *victim, int nDamage)
{
	int nDamageRate = 100;     // The Original Damage Rate of Protection, which is 3 / 4, 75%.

	/*if(IS_SET( race_table[ victim->race ].race_abilities, RACE_PROTECTION )){
		if(IS_NPC(victim)){
			nDamageRate = 85;
			nDamage = nDamage * nDamageRate / 100;
			return nDamage;
		}else{
			int nIbaSklv  = get_skill_level(victim, gsn_inborn_ability);
			nDamageRate   = 85 - ((nIbaSklv + 1) / 2) * 5;
			if(nIbaSklv < 4 && (IS_EVIL(victim) || !IS_EVIL(ch))){
				nDamageRate = 100;
			}else{
				if(IS_GOOD(ch)){
					nDamageRate = (nIbaSklv < 6) ? 100 : (100 + nDamageRate) / 2;
				}
				if(!IS_EVIL(ch)){
					nDamageRate = (nIbaSklv < 5) ? 100 : (100 + nDamageRate) / 2;
				}
				if(IS_EVIL(victim)){
					nDamageRate = (nIbaSklv < 4) ? 100 : (100 + nDamageRate) / 2;
				}
			}
		}
	}

	if (nDamageRate > 75 && IS_AFFECTED(victim, AFF_PROTECT)
		&& IS_EVIL( ch ) && !IS_EVIL(victim ) ){
		nDamageRate = 75;
	} */
	
	// 如果 ch 與 vitctim 同陣營則不作用 2022/10/26
	if(    ( IS_GOOD( ch ) && IS_GOOD( victim ) )
		|| ( IS_NEUTRAL( ch ) && IS_NEUTRAL( victim ) )
		|| ( IS_EVIL( ch ) && IS_EVIL( victim ) ) )
	{
		nDamage = nDamage * nDamageRate / 100;
		return nDamage;
	}

	if( IS_SET( race_table[ victim->race ].race_abilities, RACE_PROTECTION ) || IS_AFFECTED(victim, AFF_PROTECT) )
	{
		// npc 判定
		if( IS_NPC(victim) )
		{
			if( IS_GOOD( victim ) ){
				if( IS_EVIL( ch ) )
					nDamageRate = 80;
				else
					nDamageRate = 90;
			}else if( IS_EVIL( victim ) ){
				if( IS_GOOD( ch ) )
					nDamageRate = 80;
				else
					nDamageRate = 90;
			}else{
				nDamageRate = 85;
			}
		}else{
			int nProSklv = get_skill_level( victim, skill_lookup("protection") );
			int nIbaSklv = 0;

			if( IS_SET( race_table[ victim->race ].race_abilities, RACE_PROTECTION ) )
				nIbaSklv = get_skill_level( victim, gsn_inborn_ability);

			if( nIbaSklv > nProSklv ){
				nDamageRate = 99 - 50 * (nIbaSklv * nIbaSklv) / 120; // sklv 6 = 84%
				nDamageRate = nDamageRate - nProSklv;
			}else{
				nDamageRate = 99 - 50 * (nProSklv * nProSklv) / 120;
				nDamageRate = nDamageRate - nIbaSklv;
			}
			if( IS_GOOD( victim ) ){
				if( IS_EVIL( ch ) )
					nDamageRate = nDamageRate - 9;
			}else if( IS_EVIL( victim ) ){
				if( IS_GOOD( ch ) )
					nDamageRate = nDamageRate - 9;
			}else{
				nDamageRate = nDamageRate - 4;
			}
		}
	}

	nDamage = nDamage * nDamageRate / 100;
	return nDamage;
}

// 種族 Anti Magic 對魔法傷害減免
int GetMagicalUnderAntiMagic(CHAR_DATA *ch, CHAR_DATA *victim, int nDamage)
{
    int nDamageRate = 100;
    if(victim == ch) return nDamage;
    if(IS_SET( race_table[ victim->race ].race_abilities, RACE_ANTI_MAGIC )){
        if(IS_NPC(victim)){
            nDamageRate = 85;
            nDamage = nDamage * nDamageRate / 100;
            return nDamage;
        }else{
               int AMInborn = get_skill_level(victim, gsn_inborn_ability);
               nDamageRate = 99 - 34 * (AMInborn * AMInborn) / 36;
        }
    }

    nDamage = nDamage * nDamageRate / 100;
    return nDamage;
}

// 盾牌物理減傷 2020/10/30
int GetPhysicalWithShield(CHAR_DATA *ch, CHAR_DATA *victim, int nDamage, int dt)
{
	OBJ_DATA *obj;
	int nDamageRate = 100;
	int weight;
	int require;
	int reduce;
	int Break_Perc = get_skill_percent(ch , gsn_break_shield );
	int Break_Sklv = get_skill_level(ch , gsn_break_shield );
	int Power_Perc = get_skill_level(ch , gsn_powerup );
	int Block_Perc = get_skill_percent(victim , gsn_block );
	int Block_Sklv = get_skill_level(victim , gsn_block );
	int Charge_Sklv = get_skill_level(ch , gsn_charge );
	if(victim == ch) return nDamage;
	if( !( obj = get_eq_char( victim, WEAR_SHIELD ) ) )
		return nDamage;
	
	weight = get_obj_weight( obj );
	
	if( weight < 1) return nDamage;
	
	require = 1 + get_curr_str( victim ) + 2 * get_skill_level(victim, gsn_block );
	
	// max = 38 * 4 = 152
	if( IS_NPC(victim) || weight <= 4 * require )
		reduce = weight;
	else
		reduce = 4 * require + (weight - 4 * require)/10;

	// charge 技能等級降低盾牌減傷 , lv 0 降 40%  lv 6 降低 94% , 2022/04/03
	if( dt == gsn_charge )
		reduce = reduce - reduce * (40 + 9 * Charge_Sklv) / 100;

	// npc block 性能與等級有關 , lv 120 才擁有 block lv 6 的效果 2022/02/18
	if( IS_NPC(victim) ){
		Block_Sklv = UMAX( 6 , victim->level/20 );
		Block_Perc = victim->level;
	}

	// break shield 熟練及技能等級有機會無視盾牌減傷 , block 熟練及技能等級影響不被穿透 2022/02/18
	if( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->class, 8 ) ) )
	{
		if( Break_Perc/20 + Break_Sklv * 10 >= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n 運用技巧突破你的盾牌防禦!"NOR :
				HIY"$n break through your shield defend!"NOR,  ch, NULL, victim, TO_VICT );
			return nDamage;
		}
	}

	// powerup 狀態下, 無視盾牌減傷的機率與 dr 加成有關 , block 熟練及技能等級影響不被穿透 2022/02/18
	if( is_affected(ch, gsn_powerup) )
	{
		//if( 3 * ( get_skill_level(ch , gsn_powerup ) + get_curr_str(ch) - get_curr_str(victim) ) >= number_percent() )
		if( Power_Perc/33 + affected_value( ch, gsn_powerup , APPLY_DAMROLL ) * 2
			>= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n 運用發勁突破你的盾牌防禦!"NOR :
				HIY"$n break through your shield defend!"NOR,  ch, NULL, victim, TO_VICT );
			return nDamage;
		}
	}

	if ( reduce < 0 ) reduce = 0;

	if( reduce <= 30 )
		reduce = 4 + reduce * 20 / 100; // 10
	else if( reduce <= 70 )
		reduce = 10 + (reduce - 30) * 25 / 100;  // 20
	else if( reduce <= 100 )
		reduce = 20 + (reduce - 70) * 30 / 100;  // 29
	else if( reduce <= 150 )
		reduce = 29 + (reduce - 100) * 22 / 100;  // 40
	else
		reduce = 40 + (reduce - 150) * 5 / 100;  // per weight 0.05
	
	if( reduce > 99 ) reduce = 99;

	nDamageRate = 100 - reduce;
	nDamage = nDamage * nDamageRate / 100;

	return nDamage;
}

// 盾牌魔法減傷 2020/10/30
int GetMagicalWithShield(CHAR_DATA *ch, CHAR_DATA *victim, int nDamage)
{
	OBJ_DATA *obj;
	int nDamageRate = 100;
	int weight;
	int require;
	int reduce;
	int Break_Perc = get_skill_percent(ch , gsn_break_shield );
	int Break_Sklv = get_skill_level(ch , gsn_break_shield );
	int Power_Perc = get_skill_level(ch , gsn_powerup );
	int Block_Perc = get_skill_percent(victim , gsn_block );
	int Block_Sklv = get_skill_level(victim , gsn_block );
	if(victim == ch) return nDamage;
	if( !( obj = get_eq_char( victim, WEAR_SHIELD ) ) )
		return nDamage;
	
	weight = get_obj_weight( obj );
	
	if( weight < 1) return nDamage;
	
	require = 1 + get_curr_str( victim ) + 2 * get_skill_level(victim, gsn_block );
	
	if( IS_NPC(victim) || weight <= 4 * require )
		reduce = weight;
	else
		reduce = 4 * require + (weight - 4 * require)/10;

	// npc block 性能與等級有關 , lv 120 才擁有 block lv 6 的效果 2022/02/18
	if( IS_NPC(victim) ){
		Block_Sklv = UMAX( 6 , victim->level/20 );
		Block_Perc = victim->level;
	}

	// break shield 熟練及技能等級有機會無視盾牌減傷 , block 熟練及技能等級影響不被穿透 2022/02/18
	if( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->class, 8 ) ) )
	{
		if( Break_Perc/20 + Break_Sklv * 10 >= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n 運用技巧突破你的盾牌防禦!"NOR :
				HIY"$n break through your shield defend!"NOR,  ch, NULL, victim, TO_VICT );
			return nDamage;
		}
	}

	// powerup 狀態下, 無視盾牌減傷的機率與 dr 加成有關 , block 熟練及技能等級影響不被穿透 2022/02/18
	if( is_affected(ch, gsn_powerup) )
	{
		//if( 3 * ( get_skill_level(ch , gsn_powerup ) + get_curr_str(ch) - get_curr_str(victim) ) >= number_percent() )
		if( Power_Perc/33 + affected_value( ch, gsn_powerup , APPLY_DAMROLL ) * 2
			>= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n 運用發勁突破你的盾牌防禦!"NOR :
				HIY"$n break through your shield defend!"NOR,  ch, NULL, victim, TO_VICT );
			return nDamage;
		}
	}

	if( reduce <= 30 )
		reduce = 4 + reduce * 20 / 100;
	else if( reduce <= 70 )
		reduce = 10 + (reduce - 30) * 25 / 100;
	else if( reduce <= 100 )
		reduce = 20 + (reduce - 70) * 30 / 100;
	else if( reduce <= 150 )
		reduce = 29 + (reduce - 100) * 22 / 100;
	else
		reduce = 40 + (reduce - 150) * 5 / 100;

	reduce = reduce * 2 / 3;

	if( reduce > 99 ) reduce = 99;

	nDamageRate = 100 - reduce;
	nDamage = nDamage * nDamageRate / 100;

	return nDamage;
}

// 攻擊命中的元素判定 2022/12/25
bool IsSpellElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 9     // cause critical
		|| dt == 10    // cause light
		|| dt == 11    // cause serious
		|| dt == 44    // powerball
		|| dt == 60    // magic missile
		|| dt == 140 ) // high explosive
		return TRUE;

	// 如果是法術, 沒跑到 dt 就會 false
	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsSpellElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_SPELL ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsWindElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 77    // shocking grasp
		|| dt == 92    // gas breath
		|| dt == 95    // whirlwind
		|| dt == 187   // storm slash
		|| dt == 203   // wind screaming
		|| dt == 273   // wail of tempest
		|| dt == 280   // song of firestorm
		|| dt == 281 ) // lore of frost
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsWindElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_WIND ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsFireElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 7     // burning hands
		|| dt == 43    // fireball
		|| dt == 45    // star fire
		|| dt == 90    // fire breath
		|| dt == 165   // flaming armor
		|| dt == 185   // flaming slash
		|| dt == 204   // angry fire storm
		|| dt == 272   // phlogistic verse
		|| dt == 280   // song of firestorm
		|| dt == 282 ) // soul of magma
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsFireElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_FIRE ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsColdElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 14    // chill touch
		|| dt == 15    // colour spray
		|| dt == 91    // frost breath
		|| dt == 188   // sprinkle slash
		|| dt == 248   // ice comet
		|| dt == 264   // icecore slash
		|| dt == 275   // agglomerate descant
		|| dt == 281   // lore of frost
		|| dt == 283 ) // fang of slime
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsColdElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_COLD ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsEarthElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 1     // acid blast
		|| dt == 37    // earthquake
		|| dt == 89    // acid breath
		|| dt == 186   // earth slash
		|| dt == 247   // meteor shower
		|| dt == 276   // earth bind
		|| dt == 282   // soul of magma
		|| dt == 283 ) // fang of slime
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsEarthElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_EARTH ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsLightningElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 8     // call lightning
		|| dt == 57    // thunder strike
		|| dt == 58    // lightning bolt
		|| dt == 96    // lightning breath
		|| dt == 263   // chain lightning
		|| dt == 265   // fulgurant slash
		|| dt == 274 ) // fulgurant dance
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsLightningElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_LIGHTNING ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsSaintElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 34    // dispel evil
		|| dt == 46    // flamestrike
		|| dt == 83    // turn undead
		|| dt == 84    // mass turn
		|| dt == 205   // sun flash
		|| dt == 277   // holy judgement
		|| dt == 292 ) // sun slash
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsSaintElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_SAINT ) > 0 )
		return TRUE;

	return FALSE;
}
bool IsDarkElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 35    // dispel good
		|| dt == 39    // energy drain
		|| dt == 51    // harm
		|| dt == 163   // lifesteal
		|| dt == 270   // apocalypse
		|| dt == 271   // dark containment
		|| dt == 278 ) // hell southerncross
		return TRUE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		return FALSE;

	if( IsCast )
		bug("IsDarkElement: IsCast TRUE no used", 0 );

	if ( wpn == NULL )
		return FALSE;

	if( wpn && GetObjElement( wpn , APPLY_WEAPON_DARK ) > 0 )
		return TRUE;

	return FALSE;
}

// 裝備特定的附魔數值判定 2022/12/25
int GetObjElement( OBJ_DATA *obj , int type )
{
	int EncDam = 0;
	int EncTemp = 0;

	if( obj == NULL )
		return EncDam;

	if( !type )
		return EncDam;

	AFFECT_DATA *o_paf;

	for ( o_paf = obj->affected; o_paf; o_paf = o_paf->next ) // 物品額外加入的 affect
	{
		if ( o_paf->location == type ){
			EncTemp = o_paf->modifier;
			EncDam += EncTemp;
		}
	}
	for ( o_paf = obj->pIndexData->affected; o_paf; o_paf = o_paf->next ) // 寫在 area 中的 affect
	{
		if ( o_paf->location == type ){
			EncTemp = o_paf->modifier;
			EncDam += EncTemp;
		}
	}
	for ( o_paf = obj->material->affected; o_paf; o_paf = o_paf->next ) // 材質
	{
		if ( o_paf->location == type ){
			EncTemp = o_paf->modifier;
			EncDam += EncTemp;
		}
	}

	return EncDam;
}

// 附魔法術傷害計算 2021/10/04
int MagicElemental(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj , int dt)
{
	AFFECT_DATA *paf;
	int EncDam = 0;
	int EncTemp = 0;
	int AmSklv = get_skill_level(ch, gsn_artifacts_mastery); // 2022/04/29

	if( IsSpellElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_SPELL );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if( dt == 60 )     // magic missile
			EncTemp = EncTemp/10;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_SPELL)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsWindElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_WIND );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if( dt == 280 || dt == 281 )  // song of firestorm , lore of frost
			EncTemp = EncTemp/2;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_WIND)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsFireElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_FIRE );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if( dt == 280 || dt == 282 ) // song of firestorm , soul of magma
			EncTemp = EncTemp/2;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_FIRE)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsColdElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_COLD );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if( dt == 281 || dt == 283 ) // lore of frost , fang of slime
			EncTemp = EncTemp/2;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_COLD)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsEarthElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_EARTH );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if( dt == 282 || dt == 283 ) // fang of slime , soul of magma
			EncTemp = EncTemp/2;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_EARTH)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsLightningElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_LIGHTNING );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if( dt == 8 && ch->class == SAVANT && !IS_NPC(ch) )
		{
			if ( IS_GOOD(ch) || IS_EVIL(ch) )
				EncTemp = EncTemp/2;
		}
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_LIGHTNING)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsSaintElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_SAINT );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_wis(ch) ) / 200;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100;
		else
			EncDam += EncTemp*1.5;
	}
	if( IsDarkElement( ch, dt, NULL, TRUE ) )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_DARK );
		if( EncTemp > 0 )
			EncTemp = EncTemp + ( EncTemp * get_curr_wis(ch) ) / 200;
		if( EncTemp > 0 )
			EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100;
		else
			EncDam += EncTemp*1.5;
	}

	if( dt == 8 && ch->class == SAVANT && !IS_NPC(ch) )
	{
		if( IS_GOOD(ch) )
		{
			EncTemp = GetObjElement( obj, APPLY_WEAPON_SAINT );
			if( EncTemp > 0 )
				EncTemp = EncTemp + ( EncTemp * get_curr_wis(ch) ) / 200;
			EncTemp += EncTemp/2;
			if( EncTemp > 0 )
				EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100;
			else
				EncDam += EncTemp*1.5;
		}

		if( IS_EVIL(ch) )
		{
			EncTemp = GetObjElement( obj, APPLY_WEAPON_DARK );
			if( EncTemp > 0 )
				EncTemp = EncTemp + ( EncTemp * get_curr_wis(ch) ) / 200;
			EncTemp += EncTemp/2;
			if( EncTemp > 0 )
				EncDam += EncTemp * (150 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100;
			else
				EncDam += EncTemp*1.5;
		}
	}

	if( EncDam == 0 )
		return EncDam;

	// alc artifacts mastery 加成 2022/04/29
	if( !IS_NPC(ch) && EncDam > 0 )
		EncDam = EncDam *( 6 + AmSklv)/6;

	return EncDam;
}

// 附魔傷害計算 2021/10/06
int PhysicalElemental(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
	AFFECT_DATA *paf;
	int EncDam = 0;
	int EncTemp = 0;
	int VchMdr = 0;
	int AmSklv = get_skill_level(ch, gsn_artifacts_mastery); // 2022/04/29

	// 物傷附魔不計算負值
	if( GetObjElement( obj, APPLY_WEAPON_SPELL ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_SPELL );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SPELL)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_WIND ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_WIND );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_WIND)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_FIRE ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_FIRE );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_FIRE)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_COLD ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_COLD );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_COLD)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_EARTH ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_EARTH );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_EARTH)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_LIGHTNING ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_LIGHTNING );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_LIGHTNING)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_SAINT ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_SAINT );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ) ) // inborn detect align 對光暗加成
			EncTemp = ( EncTemp * (24 + get_skill_level(ch, gsn_inborn_ability)) )/24;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_DARK ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_DARK );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ) ) // inborn detect align 對光暗加成
			EncTemp = ( EncTemp * (24 + get_skill_level(ch, gsn_inborn_ability)) )/24;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100;
	}

	// 有正面加成時 裝備耐久較易耗損
	if( EncDam > 0
		&& ( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) ) )
		&& ( obj->item_type == ITEM_ARMOR || obj->item_type == ITEM_WEAPON ) )
			waste_dura(obj);

	// anti_magic inborn 魔法減傷
	if(IS_SET( race_table[ victim->race ].race_abilities, RACE_ANTI_MAGIC ))
	{
		if(IS_NPC(victim)){
			EncDam = EncDam * 85 / 100;
		}else{
			EncDam = EncDam * (99 - 34 * (get_skill_level(victim, gsn_inborn_ability) 
							* get_skill_level(victim, gsn_inborn_ability) ) / 36) / 100;
		}
	}

	// 傷害為 pp 的 1/40 倍 2022/12/20
	if( ch->pp > 0 )
		EncDam = ( EncDam * (40 + ch->pp) )/40;

	// 傷害加上 md 2022/12/20
	if( ch->amdr > 0 )
		EncDam = EncDam + ch->amdr;

	// 傷害扣除 mdr 最後傷害不會有負值
	EncDam = UMAX(0 , EncDam - victim->mdr);

	// mob 魔法門檻( at )
	if( IS_NPC(victim) && EncDam < victim->mad )
		EncDam = 0;

	// alc artifacts mastery 加成 2022/04/29
	if( !IS_NPC(ch) && EncDam > 0 )
		EncDam = EncDam *( 6 + AmSklv)/6;

	return EncDam;
}

// 附魔法術傷害加成 2021/10/04
int GetEnchantmentMagicDamage(CHAR_DATA *ch, CHAR_DATA *victim , int dt)
{
	int nDamage = 0;
	OBJ_DATA *obj;
	OBJ_DATA *obj2;
	OBJ_DATA *obj3;

	obj = NULL;
	obj2 = NULL;
	obj3 = NULL;

	if( get_eq_char( ch, WEAR_WIELD ) ){
		obj = get_eq_char( ch, WEAR_WIELD );
		if( get_eq_char( ch, WEAR_AMMO ) && obj->value[3] == 16 ) // type ammo for wp
			obj3 = get_eq_char( ch, WEAR_AMMO );
	}
	else if( get_eq_char( ch, WEAR_TWO_HAND ) )
		obj = get_eq_char( ch, WEAR_TWO_HAND );
	else if( get_eq_char( ch, WEAR_HANDS ) )
		obj = get_eq_char( ch, WEAR_HANDS );


	if( get_eq_char( ch, WEAR_WIELD_2 ) ){
		obj2 = get_eq_char( ch, WEAR_WIELD_2 );
		if( get_eq_char( ch, WEAR_AMMO ) && obj2->value[3] == 16 ) // type ammo for wp2
			if( !obj3 ) obj3 = get_eq_char( ch, WEAR_AMMO );
	}
	else if( get_eq_char( ch, WEAR_HANDS_2 ) )
		obj2 = get_eq_char( ch, WEAR_HANDS_2 );


	if( !obj && !obj2 && !obj3 ) return nDamage;
	
	if( !dt ) return nDamage;


	if( obj3 ){
		if( IS_OBJ_STAT( obj3, ITEM_SPELL ) )
			nDamage = nDamage + MagicElemental( ch, victim, obj3, dt);
		if( obj && IS_OBJ_STAT( obj, ITEM_SPELL ) )
			nDamage = nDamage + MagicElemental( ch, victim, obj, dt)/2;
		if( obj2 && IS_OBJ_STAT( obj2, ITEM_SPELL ) )
			nDamage = nDamage + MagicElemental( ch, victim, obj2, dt)/2;
	}else{
		if( obj && IS_OBJ_STAT( obj, ITEM_SPELL ) )
			nDamage = nDamage + MagicElemental( ch, victim, obj, dt);
		if( obj2 && IS_OBJ_STAT( obj2, ITEM_SPELL ) )
			nDamage = nDamage + MagicElemental( ch, victim, obj2, dt)/2;
	}

	if(IS_NPC(ch) && nDamage >= 0)
		nDamage = nDamage * 2/3;
	
	return nDamage;
}

// 附魔物理傷害加成 2021/10/04
//int GetEnchantmentPhysicalDamage(CHAR_DATA *ch, CHAR_DATA *victim, int dt , int wield)
int GetEnchantmentPhysicalDamage(CHAR_DATA *ch, CHAR_DATA *victim, int dt , OBJ_DATA *wpn , OBJ_DATA *wpn2 )
{
	int nDamage = 0;

	if( wpn == NULL && wpn2 == NULL )
		return nDamage;

	if( dt == 108 ){ // kick 獨立判定
		if( !wpn && !wpn2 )
			return nDamage;
		if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )
			nDamage = nDamage + PhysicalElemental( ch, victim, wpn );
		if( wpn2 && IS_OBJ_STAT( wpn2, ITEM_SPELL ) )
			nDamage = nDamage + PhysicalElemental( ch, victim, wpn2 )/2;
	}else{
		if(    get_eq_char( ch, WEAR_WIELD )  // 有手持武器
			|| get_eq_char( ch, WEAR_TWO_HAND )
			|| get_eq_char( ch, WEAR_WIELD_2 ))
		{
			if(    dt != 1014  // shot
				&& dt != 1015  // dart 寫於 skill 下
				&& dt != 1016
				&& dt != gsn_throwing // throw 寫於 skill 下
				&& (   dt >= TYPE_HIT
					|| dt == gsn_frenzy
					|| dt == gsn_bashhead
					|| dt == gsn_charge
					|| dt == gsn_backstab
					|| dt == gsn_circle
					|| dt == gsn_circular
					|| dt == gsn_sprinkle_slash
					|| dt == gsn_flaming_slash
					|| dt == gsn_storm_slash
					|| dt == gsn_fulgurant_slash
					|| dt == gsn_earth
					|| dt == gsn_empslash ))
			{
				if( !wpn )
					return nDamage;
				if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn );
			}
			else if( dt == 1014 ){ // shot
				if( !wpn || !wpn2 )
					return nDamage;
				if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )  // bow
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn )/3;
				if( wpn2 && IS_OBJ_STAT( wpn2, ITEM_SPELL ) )  // arrow
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn2 )*3/4;
			}
			else if( dt == 1016 ){ // ammo 2022/05/04
				if( !wpn || !wpn2 )
					return nDamage;
				if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )  // gun
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn )/3;
				if( wpn2 && IS_OBJ_STAT( wpn2, ITEM_SPELL ) )  // ammo
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn2 );
			}
			else{ // charge , icecore 等無裝備技能
				if( !wpn )
					return nDamage;
				if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn );
			}
		}
		else{  // 無手持武器
			if( !wpn && !wpn2 )
				return nDamage;
			if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )
				nDamage = nDamage + PhysicalElemental( ch, victim, wpn );
			if( wpn2 && IS_OBJ_STAT( wpn2, ITEM_SPELL ) )
				nDamage = nDamage + PhysicalElemental( ch, victim, wpn2 )/2;
		}
	}

	if( !wpn && !wpn2 ) return nDamage;

	if( !dt ) return nDamage;

	if(IS_NPC(ch) && nDamage >= 0)
		nDamage = nDamage * 4/5;

	return nDamage;
}

/*  修改 blood thirsty 設定 , 故移除 2022/02/22
int GetReflexionDamageUnderBloodThirsty(CHAR_DATA *ch, int dam)
{
	if (IS_NPC(ch)) return dam;

	if (get_skill_percent( ch, gsn_blood_thirsty ) <= 0) return dam;

	int nSklv = get_skill_level(ch, gsn_blood_thirsty);
	int nCon  = get_curr_con(ch);
	if (ch->pcdata->prime_attr == APPLY_CON) nCon *= 2;

	if ( is_affected( ch, gsn_berserk ) ) {
		dam = 100 * dam / (180 + nCon * nSklv * nSklv / 2);
	}
	return dam;
}
*/

// blood thirsty 傷害計數 2022/02/20
void BloodThirstyDamCount(CHAR_DATA *ch, int dam)
{
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, gsn_blood_thirsty);

	if( !ch->fighting )
		return;

	af.type      = gsn_blood_thirsty;
	if( is_affected(ch, gsn_blood_thirsty) )
		af.duration  = 0;
	else
		af.duration  = -1;
	af.location  = APPLY_DAMAGE_COUNT;
	if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) >= 6000 ){
		if( is_affected(ch,gsn_berserk) )
			af.modifier = 1 + sklv*sklv + ( ( 1 + sklv/6 ) * dam)/1200;
		else
			af.modifier = 1 + sklv*sklv/2 + ( ( 1 + sklv/6 ) * dam)/1500;
	}else{
		if( is_affected(ch,gsn_berserk) )
			af.modifier = 1 + sklv*sklv + ( ( 1 + sklv/6 ) * dam)/600;
		else
			af.modifier = 1 + sklv*sklv/2 + ( ( 1 + sklv/6 ) * dam)/800;
	}
	af.bitvector = 0;
	affect_join( ch, &af );
	return;
}

// 針對 飛行武器 在發射後的 apdr 屬性加成 2022/05/06
int ShootApdrAffect( CHAR_DATA *ch , OBJ_DATA *obj)
{
	AFFECT_DATA *af;
	int number = 0;
	
	if( !ch ) return number;
	
	if( !obj ) return number;

	for( af = obj->affected; af; af = af->next ){
		if( af->location == APPLY_SHOOT_ADAMAGE ){
			number = number + af->modifier;
		}
	}

	return number;
}

// 計算 mystic armor 數值及傷害減少 2023/01/09
int MysticArmorCount( CHAR_DATA *ch , int dam )
{

	if( !IS_AFFECTED(ch, AFF_MYSTIC_ARMOR ) )
		return dam;

	int MALv = affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);

	if( dam <= 0 ){
		act( "$n 的魔力障壁吸收了魔法！", ch, NULL, NULL, TO_ROOM );
		send_to_char( "你的魔力障壁吸收了魔法！\n\r", ch );
		return dam;
	}

	MALv -= dam;

	if( MALv > 0 )
	{
		if( get_skill_level(ch, gsn_mystic_armor) > 2 )
			ch->mana += dam / 50;
		ch->mana = UMIN( ch->max_mana , ch->mana );

		AFFECT_DATA af;
		af.type  = gsn_mystic_armor;
		af.duration = 0;
		af.location = APPLY_MAGIC_IMMUNE;
		af.modifier = MALv - affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);
		af.bitvector = AFF_MYSTIC_ARMOR;
		affect_join( ch, &af );

		dam = 0;

		act( "$n 的魔力障壁吸收了魔法！", ch, NULL, NULL, TO_ROOM );
		send_to_char( "你的魔力障壁吸收了魔法！\n\r", ch );
	}
	else if( MALv == 0 )
	{
		
		if( get_skill_level(ch, gsn_mystic_armor) > 2 )
			ch->mana += affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) / 50;
		ch->mana = UMIN( ch->max_mana , ch->mana );

		affect_strip(ch, gsn_mystic_armor);
		REMOVE_BIT(ch->affected_by, AFF_MYSTIC_ARMOR);
		
		dam = 0;
		
		act( "$n 的魔力障壁吸收魔法後破碎了！", ch, NULL, NULL, TO_ROOM );
		send_to_char( "你的魔力障壁吸收魔法後破碎了！\n\r", ch );
	}
	else if( MALv < 0 )
	{
		if( get_skill_level(ch, gsn_mystic_armor) > 2 )
			ch->mana += affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) / 100;
		ch->mana = UMIN( ch->max_mana , ch->mana );

		affect_strip(ch, gsn_mystic_armor);
		REMOVE_BIT(ch->affected_by, AFF_MYSTIC_ARMOR);

		dam = dam + MALv;

		act( "$n 的魔力障壁被擊穿了！", ch, NULL, NULL, TO_ROOM );
		send_to_char( "你的魔力障壁被擊穿了！\n\r", ch );
	}
	return dam;
}


/*
 * Inflict damage from a hit.
 * 加入中文註解  by Razgriz 20051013
 * 介面改寫 by Razgriz 20051014
 */ 
int damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn, int msg_mode, int dmg_mode )
{
	//OBJ_DATA *obj;
	int  sntemp;
	int  leveldiff = ch->level - victim->level;
	int  intdiff = get_curr_int( ch ) - get_curr_int( victim );
	int  dexdiff = get_curr_dex( ch ) - get_curr_dex( victim );
	char ibuf [256];
	int  darkraid = 0;
	int  isShot   = (dt == 1014);
	int  isDart   = (dt == 1015);  //0121 dart 判定
	int  isAmmo   = (dt == 1016);  // 2022/05/03
	int  isThrow  = (dt == gsn_throwing); // 2022/05/03
	bool isReflexion;
	CHAR_DATA *orig = victim;
	//int count = 0;
	int Enc_Psy_Dam = 0; // 2022/12/25

	if ( victim->position == POS_DEAD )
	{
		return -2;
	}
	// if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE)) return;

	//檢查 反向殺法
	isReflexion = FALSE;
	if( check_reflexion(ch,victim, dt, dam, dexdiff) && dt != gsn_lifeshare )
	{
		// 為使 reposte 不被反向 , 修改為判定 dmg_mode 2021/11/16
		//if(dt != 1014){
		if( dmg_mode == DMG_NORMAL && dt != 1014 && dt != 1016 ){ // 增加 槍械不被反向
			// AC 影響的反函式
			isReflexion = TRUE;
			int nEnemyAC = GET_AC(victim) * 0.1;
			if(!IS_NPC(victim)){
				if(GET_AC(victim) < 0){
					// ac -1000 時為受到傷害乘 1.25 , ac 0 時為受到傷害乘 2.5
					nEnemyAC = ( nEnemyAC * nEnemyAC )/ 10 + 1000;
					dam = (dam * 2500) / nEnemyAC;
				}else{
					// ac 大於 0 時為受到傷害乘 2.5 + ac/1000 , 最高乘 4 ( ac 1500 )
					dam = dam * UMIN( 4 , (2500 + GET_AC(victim))/1000);
				}
			}else{
				if(GET_AC(victim) < 0){
					// ac -1000 時為受到 0.9 倍 , ac 0 時與受到傷害乘 1.13 , 遞減傷害最低為 40% 
					nEnemyAC = UMIN( 12500 , (nEnemyAC * nEnemyAC)/10 + 3500 );
					dam = (dam * 4500) / nEnemyAC;
				}else{
					// ac 大於 0 時為受到傷害乘 1 + ac/2000 , 最高乘 3 ( ac 6000 )
					dam = dam * UMIN( 3 , (4500 + GET_AC(victim))/4000);
				}
			}
			if(!IS_NPC(ch))
				dam = dam + UMIN( 0, (GET_AC( ch ) / 2 ) );
			if(IS_NPC(victim))
				dam /= 2;
			// 針對 npc 死前一擊, reflexion 不轉換 victim 為 ch , 將敘述轉變為 dodge 成功 並 return -1 2022/12/29
			if( IS_NPC(ch) && ch->hit <= 0 ){
				act( (!is_chinese(ch, victim)) ?
						"$N dodges your attack." :
						"$N 靈巧地閃避你的攻擊.", ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
						"You dodges $n's attack.":
						"你靈巧地閃避$n的攻擊.", ch, NULL, victim, TO_VICT  );
				return -1;
			}else{
				ch->attacked = victim;
				victim = ch;
			}
		}else{
			// 若為箭矢攻擊, 則反向接箭
			if(dt == 1014)
				return 0;
		}
	}

	//加上物理傷害力判定  2020/11/01
	if( dt != gsn_lifeshare ){
		dam += ch->apdr;
		// 針對 飛行武器 的 apdr 效果加成 2022/05/06
		if( isAmmo )
			dam += ShootApdrAffect(ch , get_eq_char( ch, WEAR_AMMO ) );
	}
	// tricky pacing pd 額外加成 2021/11/05
	if( !IS_NPC(ch)
		&& (victim != ch)
		&& dt != gsn_lifeshare  // lifeshare 的傷害不會被判定到 2021/11/22
		&& get_skill_level(ch, gsn_tricky_pacing) > 0 ){
		dam += ch->apdr * get_skill_level(ch, gsn_tricky_pacing) / 3;
		// 針對 飛行武器 的 apdr 效果加成 2022/05/06
		if( isAmmo )
			dam += ShootApdrAffect(ch , get_eq_char( ch, WEAR_AMMO ) ) * get_skill_level(ch, gsn_tricky_pacing) / 3;
	}
    /*
     * Stop up any residual loopholes.
     */
	int nSklvShot      = get_skill_level(ch,gsn_shot_arrow);
	int nSklvMete      = get_skill_level(ch, gsn_meteoric_arrow);
	// Gnome  cle/ran [Shot(6), Mete(6), WIS(25)] = ((60 + 60) / 2) * (20 * 20 / 400) = 60.0%
	// hobbit cle/ran [Shot(6), Mete(6), WIS(24)] = ((60 + 60) / 2) * (19 * 19 / 400) = 54.1%
	// hobbit ---/ran [Shot(6), Mete(6), WIS(21)] = ((60 + 60) / 2) * (16 * 16 / 400) = 38.4%
	// Orc    war/ran (Shot(6), Mete(6), WIS(15)] = ((60 + 60) / 2) * (10 * 10 / 400) = 15.0%
	int nRateIgnorePDR = ((nSklvShot * (nSklvShot + 4) + nSklvMete * (nSklvMete + 4)) / 2)
							* (get_curr_wis(ch) - 5) * (get_curr_wis(ch) - 5) / 400;
	// 會造成傷害飄過高 , 故移除 2020/11/01
	//bool hasIgnoredPDR = FALSE;

	if( dt != gsn_lifeshare )
	{
		//根據攻擊者的Level對傷害進行調降，以延長高等級的戰鬥
		//dam = check_damage_adjust(ch,dam);

		//加上物理傷害力 , 移到 dam > 0 判定前 2020/11/01
		//dam += ch->apdr; 

		//攻擊型態為 射出箭矢 shot, 玩家有一定機率以 WIS 以及 箭術 跟 流星箭 的 sklv 來無視部分 抗物理傷害
		if (isShot)
		{
			// 針對 飛行武器 的 apdr 效果加成 2022/05/06
			if( !IS_NPC(ch) ){
				dam += ( ch->apdr + ShootApdrAffect(ch , get_eq_char( ch, WEAR_ARROW ) ) )* nSklvShot / 6;

				// shot 的情況下對 tricky pacing 作 飛行武器 的 apdr 效果加成 2022/05/06
				if( get_skill_level(ch, gsn_tricky_pacing) > 0 )
					dam += ShootApdrAffect(ch , get_eq_char( ch, WEAR_ARROW ) ) * get_skill_level(ch, gsn_tricky_pacing) / 3;
			}

			if(IS_NPC(victim)){
				if(victim->pdr >= 0 && number_percent() >= nRateIgnorePDR){
					dam -= victim->pdr* 3 / 2;
				} else {
					;
				}
			}else{
				dam -= victim->pdr * 3;
			}
		}else{
			dam -= victim->pdr;
			nRateIgnorePDR = 0;
		}
		dam = UMAX ( dam , 0 );

	}

	/*  修改 blood thirsty 設定 , 故移除 2022/02/22
		// 玩家嗜血狀態下被反向, 被反向傷害大幅降低
		if (isReflexion) {
			dam = GetReflexionDamageUnderBloodThirsty(victim, dam);
		}
	*/


	//處理 傷害門檻值
	if( IS_NPC(victim) && dam <= victim->pad && dt != gsn_lifeshare )
	{
		//騎士 有 1/4 的機率 無視傷害門檻值
		if( !IS_NPC(ch) && ch->class == KNIGHT && number_bits(2) >= 1 )
			;
		// ran 無視 gate 會造成傷害飄過高 , 故移除 2020/11/01
		//else if(hasIgnoredPDR == FALSE)
		else
			dam = 0;
	}

	//傷害大於 30000 的 log 起來
	if ( dam > 30000 )
	{
		//char buf [ MAX_STRING_LENGTH ];
		if ( IS_NPC( ch ) && ch->desc )
		{
			bugf("Damage: %d from %s by %s: > 30000 points with %d dt!",dam, ch->name, ch->desc->original->name, dt );
		}
		else
		{
			bugf("Damage: %d from %s: > 30000 points with %d dt!",dam, ch->name, dt );
		}
		/*dam = 1000;*/
	}

	//假如不是自己打自己的話 ( reflexion 的情況下victim = ch )
	// lifeshare 的傷害不會被判定到 2021/11/22
	if ( victim != ch && dt != gsn_lifeshare )
	{
		// invis 攻擊判定顯型
		if ( IS_AFFECTED( ch, AFF_INVISIBLE ) && !IS_SET(ch->special, ALY_INVIS))
		{
			affect_strip( ch, gsn_invis );
			affect_strip( ch, gsn_mass_invis );
			REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
			act( "$n 的身影漸漸浮現.", ch, NULL, NULL, TO_ROOM );
		}

		// hide 攻擊判定顯型
		if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_SET(ch->special, ALY_HIDE) )
		{
			REMOVE_BIT( ch->affected_by, AFF_HIDE );
			act( "$n 的身影漸漸浮現.", ch, NULL, NULL, TO_ROOM );
		}

		// nightstalk 攻擊判定顯型
		if ( IS_AFFECTED( ch, AFF_NIGHTSTALK ) )
			affect_strip( ch, gsn_nightstalk );

		// dark raid 攻擊判定
		if ( IS_AFFECTED( ch, AFF_DARK_RAID ) )
		{
			affect_strip( ch, gsn_dark_raid );
			send_to_char( "你全神貫注，對目標施予必殺一擊！\n\r", ch );
			darkraid = 1;
		}

		// safe room 攻擊無效
		if ( is_safe( ch, victim ) ) return -2;

		check_killer( ch, victim );

		//pc打pc 或 pc有clanpk且在pk room 時, 傷害變成 1/8
		if( IsPK(ch, victim)
			|| (  !IS_NPC( victim ) && victim->pcdata->clanpk 
				&& IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
		dam /= PK_DMG_DENOMINATOR;

		//計算偷襲的加成與攻擊招喚物的後果
		dam = check_dirty_raid(ch,victim,dam);

		// 會造成傷害無法顯示 2021/11/23
		if (dam < 0) dam = 0;//return -2;

	/*
		if (   dmg_mode == DMG_NORMAL
			&& (    IS_AFFECTED( victim, AFF_PROTECT )
				|| IS_SET( race_table[ victim->race ].race_abilities,RACE_PROTECTION )  )
			&& IS_EVIL( ch )
			&& !IS_EVIL(victim) )
		{
			dam -= dam / 4;
		}
	*/
		if ( dam < -1 )
		{
			dam = 0;
		}

	///// 到此為止 攻擊傷害(dam) 已經定下來了, 之後不應該再有變動

		/* CAST_ACC */
		//打掉法術的集氣時間, 殞石雨(meteor shower), 冰之彗星(ice comet), 高能核爆(high explosive)
		if( dam > 0 )
		{
			REMOVE_BIT(victim->affected_by, AFF_CAST_ACC);
		}

		/////技能 disarm trip parry dodge block tactics 處理開始
		/*
				2021/11/16 修改

				目前僅有技能的 dmg_mode 會是 DMG_ADJUST ( empslash 及 icecore )
				因 gsn_reposte 在 fight_hits 內 dt 轉換為普擊, 而造成後續pdb 及 reflexion 判定不作用
				故把 reposte 輸出的 dmg_mode 改為 DMG_ADJUST 並移除 DMG_ADJUST 對 pdb 的判定

			//DMG_ADJUST 時, 只會對普擊有 parry dodge block
			if ( dmg_mode == DMG_ADJUST )
			{
				if ( dt >= TYPE_HIT && dam > 0 )
				{
					if(number_percent() > 50){
						if( !isShot )  // if( dt != 1014 )
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
						}
						if( !isShot )  // if( dt != 1014 )
						{
							if ( check_parry( ch, victim, leveldiff ) )
								return 0;
						}
						if ( check_block( ch, victim, leveldiff ) )
							return 0;
				   }else{
						if( !isShot )  // if( dt != 1014 )
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
						}
						if ( check_block( ch, victim, leveldiff ) )
							return 0;

						if( !isShot )  // if( dt != 1014 )
						{
							if ( check_parry( ch, victim, leveldiff ) )
								return 0;
						}
				   }
				}
			}
			else
		*/
		//DMG_NORMAL 時的 disarm trip parry dodge block tactics 判斷
		if ( dmg_mode == DMG_NORMAL )
		{
			//普擊, 飛腿, 主屬不是con的pc狂暴攻擊
			//npc 使用的迴旋斬會被機率 pdb 2022/04/03
			if (   dt >= TYPE_HIT
				|| dt == gsn_kick
				|| ( !IS_NPC(ch)
					&& dt == gsn_frenzy
					&& ch->pcdata->prime_attr != APPLY_CON)
				|| ( IS_NPC(ch) && dt == gsn_circular ) )
			{
				//處理 普擊 跟 飛腿, 傷害為0時也能作用
				if( dt != gsn_frenzy )
				{
					//擊落武器 (npc才有)
					if ( IS_NPC( ch )
						&& number_percent( ) < UMIN( 20,UMAX( leveldiff + ( intdiff * 2 ), 5 ) ) )
					{
						disarm( ch, victim );
						if ( !get_eq_char( ch, wpn ) )
						{
							if( dt != gsn_kick )// 避免 mob kick 莫名其妙變成空手攻擊 2021/12/19
								dt = TYPE_HIT;//reflexion disarm, ch disarm ch,
						}
					}    //dt should became TYPE_HIT

					//自動絆倒 (npc才有)
					if (   IS_NPC( ch )
						&& ch->level > 10
						&& number_percent( ) < UMIN( ( leveldiff + intdiff * 4 ), ( IS_SET( ch->class, 1 ) ? 15 : 3 ) ) )
					{
						trip( ch, victim );
					}

					//主屬性是str 或者 職業為僧侶 的 pc, 將有機會自動絆倒對方
					if (   !IS_NPC( ch )
						&& ( ch->pcdata->prime_attr == APPLY_STR || ch->class == 13 )
						&& number_percent( )
							< UMAX( 15, get_skill_percent(ch, gsn_fool ) / 6
								+ UMAX( 0, ( intdiff + dexdiff ) * 4 ) + leveldiff + 10 ) )
					{
						trip( ch, victim );
					}

					//天生 RACE_WEAPON_WIELD 的 mob, 有一定機率使用身上的 scroll wand staff potion pill
					if (   IS_NPC( ch )
						&& IS_SET( race_table[ ch->race ].race_abilities,RACE_WEAPON_WIELD )
						&& number_percent() < UMIN( 25, UMAX( 10, ch->level ) )
						&& !IS_NPC( victim ) )
					{
						use_magical_item( ch ); // 這個函式沒判斷好, potion 沒判斷到
						if (   ch->deleted
							|| victim->deleted
							|| !ch->in_room
							|| ch->in_room != victim->in_room
							|| !ch->fighting )
						{
							return -2;
						}
						if ( !get_eq_char( ch, wpn ) ) //item exploded
						{
							// 避免 mob kick 莫名其妙變成空手攻擊 2021/12/19
							if( dt != gsn_kick )
								dt = TYPE_HIT;
						}
					}
				}

				//傷害大於0時
				if ( dam >= 0 )
				{
					//攻擊型態 不是 射出箭矢 shot 或 槍械射擊 ammo 時
					if ( !isShot && !isAmmo ) // if( dt != 1014 )
					{
						//狂暴攻擊
						if( dt == gsn_frenzy )
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
							//pc 主屬性為str的kni, 有機會用盾牌擋住 主屬性不是con的狂暴攻擊
							if(    !IS_NPC(victim)
								&& victim->class == KNIGHT
								&& victim->pcdata->prime_attr == APPLY_STR)
							{
								leveldiff += (leveldiff / 3);
								if ( check_block( ch, victim, leveldiff ) )
									return 0;
								//if ( check_tactics( victim, ch ) )
								//    return;
							}

							//主屬性不是con的狂暴攻擊, 有機會被 parry dodge 檔掉
							//if ( check_parry( ch, victim, leveldiff ) )
							if ( check_parry( ch, victim, leveldiff, dt , wpn ) )
								return 0;
						}
						//npc 的迴旋斬有機會被parry掉 2022/04/03
						else if( dt == gsn_circular )
						{
							leveldiff += 5;
							//if ( check_parry( ch, victim, leveldiff ) )
							if ( check_parry( ch, victim, leveldiff, dt , wpn ) )
								return 0;
							if ( check_tactics( victim, ch ) )
								return 0;
						}
						//間隙反擊 有機會被 dodge tactics 檔掉.......但是, 這個判斷應該跑不到吧 -_-
						else if( dt == gsn_reposte )
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
							if ( check_tactics( victim, ch ) )
								return 0;
						}
						//普擊 有機會被 parry dodge block tactics 檔掉
						else
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
							if ( number_percent() > 50){
								//if ( check_parry( ch, victim, leveldiff ) )
								if ( check_parry( ch, victim, leveldiff, dt, wpn ) )
									return 0;
								if ( check_block( ch, victim, leveldiff ) )
									return 0;
							}else{
								if ( check_block( ch, victim, leveldiff ) )
									return 0;
								//if ( check_parry( ch, victim, leveldiff ) )
								if ( check_parry( ch, victim, leveldiff, dt, wpn ) )
									return 0;
							}
							if ( check_tactics( victim, ch ) )
								return 0;
						}
					}else{
						// shot 才會被 block 2022/05/03
						if ( isShot && check_block( ch, victim, leveldiff ) )
							return 0;
					}
				}
			}
			//主屬性為con 或 npc 的狂暴攻擊
			else if( dam >= 0 && dt == gsn_frenzy && !IS_NPC(victim) )
			{
				//有機會被主屬性為str的kni以 parry dodge block 擋住
				if ( victim->class == KNIGHT && victim->pcdata->prime_attr == APPLY_STR)
				{
					leveldiff += (leveldiff / 3);

					if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
						return -1;
					if(number_percent() > 50){
						//if ( check_parry( ch, victim, leveldiff ) )
						if ( check_parry( ch, victim, leveldiff, dt, wpn ) )
							return 0;
						if ( check_block( ch, victim, leveldiff ) )
							return 0;
					}else{
						if ( check_block( ch, victim, leveldiff ) )
							return 0;
						//if ( check_parry( ch, victim, leveldiff ) )
						if ( check_parry( ch, victim, leveldiff, dt, wpn ) )
							return 0;
					}
				}else{
					//有機會被dodge
					if ( frenzy_check_dodge( ch, victim, leveldiff, dexdiff ) )
						return -1;
				}
			}
		}// 技能 disarm trip parry dodge block tactics 處理結束
	}//不是自己打自己 處理結束

    /*
     * Damage modifiers.
     */

	// 魔力屏障 mana shield  2023/01/09
	// 從 fight_hit 移到 fight_damage 中處理, 以判定部分技能減傷
	// 並加入針對 gsn_lifeshare 及 ch 自殘的情況不會作用
	if ( IS_AFFECTED( victim, AFF_MANA_SHIELD ) && dam > 0 && dt != gsn_lifeshare && victim != ch )
	{
		int nMsSklv      = get_skill_level(victim, gsn_mana_shield);
		int nDmgToMpRate = 5 + nMsSklv * 3 / 2;
		int nBlkDmg      = UMIN(dam, victim->mana * nDmgToMpRate);

		if(nBlkDmg < 0) nBlkDmg = 0;

		dam -= nBlkDmg * (nMsSklv + 6) / 12;

		if( dam < 0 ) dam = 0;

		nBlkDmg += (ch->apdr - victim->pdr);

		nBlkDmg = GetDamageUnderProtection(ch, victim, nBlkDmg);

		if(nBlkDmg < 0) nBlkDmg = 0;

		victim->mana -= 0 + nBlkDmg / nDmgToMpRate;

		if(victim->mana <= nDmgToMpRate){
			victim->mana = 0;
			affect_strip( victim, gsn_mana_shield );
		}
	}

	// 將加回舊版 empslash 及 icecore slash 物理段破聖光的效果移到判定 poison 及附魔前 2022/10/26
	if( dt == gsn_empslash || dt == gsn_icecore_slash )
		dam *= 2;

	// 攻擊命中時, 加入命中的裝備判定 2022/12/25
	OBJ_DATA *using_wpn = NULL;
	OBJ_DATA *using_wpn2 = NULL;
	using_wpn = check_using_wp( ch, dt , wpn );
	using_wpn2 = check_using_wp2( ch, dt );


	// 附魔傷害加成 2021/10/06
	// ch 不是 victim 且不是 lifeshare 2021/11/22
	if ( victim != ch && dt != gsn_lifeshare )
	{
		//dam += GetEnchantmentPhysicalDamage( ch, victim, dt , wpn);
		if( using_wpn || using_wpn2 )
			Enc_Psy_Dam = GetEnchantmentPhysicalDamage( ch, victim, dt , using_wpn , using_wpn2 );
		// mystic armor 減少附魔傷害 2023/01/09
		if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && dt != gsn_lifeshare && ch != victim && Enc_Psy_Dam > 0 )
			Enc_Psy_Dam = MysticArmorCount( victim , Enc_Psy_Dam );
		if( Enc_Psy_Dam > 0 )
			dam += Enc_Psy_Dam;
	}
	
	//處理 有毒武器 讓對方中毒(除了無法中毒flag)
	if (   dam > 0
		&& dt != gsn_lifeshare
		&& dt != gsn_kick
		&& dt != 1014
		&& dt != 1016
		&& dt != gsn_firearm_mastery // head shot
		&& dt != gsn_dart_mastery // coin dart
		&& dt != 146 // feed
		&& is_wielding_poisoned( ch, wpn )
		&& !IS_SET( victim->special, NO_POISON )
		&& !IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) )
		//&& !IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD )
		//&& victim->race != race_lookup ( "Vampire" )
		//&& victim->race != race_lookup ( "Undead" ) )
	{
		// 毒襲修改為每下攻擊觸發追加傷害, 並追加物品判定 2021/11/04
		//VenomousStrike(ch, victim);
		dam += VenomousStrike(ch, victim, wpn );
	}
	
	//處理 有毒箭矢射擊 或 槍彈 讓對方中毒(除了無法中毒flag)
	if (   dam > 0
		&& (dt == 1014 || dt == 1016 || dt == gsn_firearm_mastery)
		&& dt != gsn_lifeshare
		&& is_wielding_poisoned( ch, wpn )
		&& !IS_SET( victim->special, NO_POISON )
		&& !IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) )
		//&& !IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD )
		//&& victim->race != race_lookup ( "Vampire" )
		//&& victim->race != race_lookup ( "Undead" ) )
	{
		// 毒襲修改為每下攻擊觸發追加傷害, 並追加物品判定 2021/11/04
		//VenomousStrike(ch, victim);
		dam += VenomousStrike(ch, victim, wpn)/2;
	}

	/*  移除聖光傷害減半的效果, 直接給予所有傷半減, 相當於所有人獲得天生聖光 2022/10/26

		//聖光 天生聖光 傷害減半
		//針對 reposte 判定 TYPE_HIT 2021/11/16
		if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities,RACE_SANCT ) )
			&& ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT ) )
			dam /= 2; */
	dam /= 2;


    //天生傷害減半(npc only) , 加入 IS_NPC(victim) 2022/12/20
	if( IS_NPC(victim) && IS_SET( victim->special, DAM_HALF ) && dt != gsn_lifeshare)
		dam /= 2;

    //神靈附體 傷害減少1/3
    if( is_affected(victim, gsn_invoke ) && dt != gsn_lifeshare )
        dam = (dam * 2)/3;

    //護身罡氣 傷害減半
    if( is_affected(victim, gsn_defensive ) && dt != gsn_lifeshare )
        dam /=2;

    //ch (攻擊者)有 護身罡氣 傷害輸出減半
    if( is_affected(ch, gsn_defensive) )
        dam /=2;

    //天使加護術 天生天使加護術 可抵擋 1/4 邪惡的攻擊傷害
	// 針對 reposte 判定 TYPE_HIT 2021/11/16
    if ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT ){
        dam = GetDamageUnderProtection(ch, victim, dam);
    }
	if ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT ){
		dam = GetPhysicalDamageUnderRiding(victim, dam, dt);
	}
	// 物理盾牌減傷 2020/10/30
	if ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT || dt == gsn_empslash ){
		// 物理盾牌減傷加入 dt 判定 2022/04/03
		dam = GetPhysicalWithShield(ch, victim, dam , dt);
	}

	// lifeshare 效果 2021/11/20
	if( is_affected(victim, gsn_lifeshare) && dt != gsn_lifeshare ){
		if( victim->linking != NULL )
			if(isReflexion)
				dam = LifeShareDamage( victim , orig , dam );
			else
				dam = LifeShareDamage( victim , ch , dam );
	}
    /*
     * We moved dam_message out of the victim != ch if above
     * so self damage would show.  Other valid type_undefined
     * damage is ok to avoid like mortally wounded damage - Kahn
     */
	//送出傷害訊息
    if ( dt != TYPE_UNDEFINED && msg_mode == MSG_NORMAL )
    {
	  //if ( ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_CHINESE ) )
	  //  || ( IS_NPC( ch ) && !IS_NPC( victim ) && !IS_SET( victim->act, PLR_CHINESE ) ) )
        dam_message( ch, victim, dam, dt, wpn );
    }

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */

	// blood thirsty 給予傷害計數 2021/12/22
	if( !IS_NPC(ch) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(ch, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( ch , dam );
		}
	}
	// blood thirsty 被傷害計數 2022/02/20
	if( !IS_NPC(victim) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(victim, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( victim , dam*2 );
		}
	}

	// adore 每下傷害成功回血 2022/11/06
	if( is_affected( ch, gsn_regeneracy ) && affected_value(ch, gsn_regeneracy, APPLY_HIT) > 0 )
		ch->hit = UMIN( ch->max_hit , ch->hit + affected_value(ch, gsn_regeneracy, APPLY_HIT) );

    //損血吧 !!!!!
    victim->hit -= UMAX(dam, 0);

    //觸發 mob hurt prog, 就是 mob 被欺負時想做的事 orz
	if( IS_NPC(victim) ){
		mprog_hurt_trigger(victim, ch, dt);
		// 加入依賴武器的物理傷害 weapon dt 判定 , 可能會造成二次判定 2020/09/18
		if( !(get_eq_char( ch, wpn )) && dt != 1000 ){
			mprog_hurt_trigger(victim, ch, TYPE_HIT );
		}else{
			if( dt != 1014 && get_eq_char( ch, wpn )
				&&(dt == gsn_bashhead
				|| dt == gsn_frenzy
				|| dt == gsn_empslash
				|| dt == gsn_backstab
				|| dt == gsn_circle
				|| dt == gsn_circular ) )
			{
				OBJ_DATA *wp_dt;
				wp_dt = get_eq_char( ch, wpn );
				mprog_hurt_trigger(victim, ch, TYPE_HIT + wp_dt->value[3] );
			}
		}
		// 物傷如果帶有元素傷害 則觸發 dt 及針對屬性的 hurt prog 2022/12/25
		if( Enc_Psy_Dam != 0 )
		{
			if( using_wpn )
			{
				if( IsSpellElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_magic_enchantment );
				if( IsWindElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_wind_enchantment );
				if( IsFireElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_flame_enchantment );
				if( IsColdElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_water_enchantment );
				if( IsEarthElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_earth_enchantment );
				if( IsLightningElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_lightning_enchantment );
				if( IsSaintElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_saint_enchantment );
				if( IsDarkElement( ch, dt, using_wpn, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_dark_enchantment );
			}
			if( using_wpn2 )
			{
				if( IsSpellElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_magic_enchantment );
				if( IsWindElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_wind_enchantment );
				if( IsFireElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_flame_enchantment );
				if( IsColdElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_water_enchantment );
				if( IsEarthElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_earth_enchantment );
				if( IsLightningElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_lightning_enchantment );
				if( IsSaintElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_saint_enchantment );
				if( IsDarkElement( ch, dt, using_wpn2, FALSE ) )
					mprog_hurt_trigger(victim, ch, gsn_dark_enchantment );
			}
		}
	}

    //給神族且有設 VERBOSEALL 的 debug 訊息
    if ( msg_mode == MSG_BRIEF )
    {
		// 加入 dmg_mode 顯示 2021/11/16
        //sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR")\n\r", dt, dam);
		sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR", dmg_mode="HIR"%d"NOR")\n\r", dt, dam , dmg_mode);
        if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
            send_to_char (ibuf, ch);
        if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
            send_to_char (ibuf, victim);
    }

    //神族不死身 XD
    if (    !IS_NPC( victim )
         && IS_IMMORTAL( victim )
         && victim->hit < 1 )
    {
        victim->hit = 1;
    }
	
	// mar 在 blood thirsty buff 效果內不會被打死 add at 2020/10/16
	if ( is_affected( victim, gsn_blood_thirsty) && affected_value(victim, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 
		&& get_skill_percent(victim, gsn_blood_thirsty) > 0 && victim->hit < 1 )
	{
		victim->hit = 1;
	}

    //處理 炎之鎧甲, 普擊(除了射箭,槍械) 飛腿 狂暴攻擊 背刺 繞刺, 都會被燒到
	if (   !isShot                   //  dt != 1014
		&& !isDart                   //  dt != 1015
		&& !isThrow                  //  投擲 2022/04/29
		&& !isAmmo                   //  dt != 1016  2022/05/03
		&& ( dt >= TYPE_HIT
			|| dt == gsn_kick
			|| dt == gsn_frenzy
			|| dt == gsn_backstab
			|| dt == gsn_circle )
		&& ( dam > -1 )
		&& IS_AFFECTED( victim, AFF_FLAMING )
		&& victim != ch
		&& victim->hit > 0 
		&& victim->mana > 7 )
    {
        victim->mana -= 7;
        if(get_skill_level(victim, gsn_spellmaster) > 3)
            victim->mana += get_skill_level(victim, gsn_spellmaster) - 3;

        sntemp = skill_lookup( "flamestrike" );

        (*skill_table[sntemp].spell_fun) ( gsn_flaming_armor, ( victim->level / 2 ),victim, ch );

        if (    victim->deleted
             || ch->deleted
             || !ch->in_room
             || ch->in_room != victim->in_room )
        {
            return dam; //ch may be REF_MAGIC mob
        }
    }

    //把 死掉或等死 的 victim 的 berserk 效應拔掉
    if (    is_affected( victim, gsn_berserk )
         && victim->position <= POS_STUNNED )
    {
        affect_strip( victim, gsn_berserk );
    }


    //更新雙方的狀態--站著, 戰鬥, 休息, 麻痺, 睡覺, STUN, 等死, 死掉
    //這樣的話..那之前 拔掉 berserk 的判斷, 不就沒用了..? @@
    check_position( ch, victim, dam );

    /*
     * Sleep spells and extremely wounded folks.
     */
    //當狀態為 睡覺 STUN 等死 死掉 時, 解除戰鬥狀態
    if ( !IS_AWAKE( victim ) && victim->fighting )
        stop_fighting( victim, FALSE );


    //一開始攻擊方是 ch, 受攻擊方是 victim = orig
    //假如反向成功, 攻擊方變成 orig, 受攻擊方變成 victim = ch
    /*
     * Payoff for killing things.
     */
    //受攻擊方(victim) 死掉時
    if ( victim->position == POS_DEAD )
    {
        //攻擊方取得經驗值
        if( orig != victim )
        {   //這邊是 ch(victim) 被 orig 的反向打死 -_-
            group_gain( orig, victim );
        }
        else
        {   //這邊是 ch 做掉 orig(victim)
            group_gain(ch, orig);
        }

        //pc 被殺
        if ( !IS_NPC( victim ) )
        {
            int  gold = 0;
            char buf[BUF_STRING_LENGTH];

            //留個 log....但這個 log 是 ch 被 orig 反死時 才是對的
            //假如是正常 ch ko orig..那 log_buf 中的 orig 應該要換成 ch 才對
            if( victim->in_room )
            {
                sprintf( log_buf, "%s(%s)被%s(%s)殺於%d",
                victim->short_descr, victim->name, orig->short_descr, orig->name,
                victim->in_room->vnum );
                log_string( log_buf );
            }

	      /*
	       * Dying penalty:
	       * 1/2 way back to previous 2 levels.
	       */
            //檢查堆屍
            if( orig != victim )
                check_corpse_count(orig, victim);
            else 
                check_corpse_count(ch, victim);

            //在非 PK room, pc以反向打死pc時, 可以從死者身上取得一定比例的錢.....為啥一定要反向阿 @_@
            if ( victim->in_room && !IS_SET(victim->in_room->room_flags, ROOM_PK))
            {
                if ( orig != victim && !IS_NPC(orig) && !IS_NPC(victim))
                {
                    gold = victim->gold * number_range( 10, 20 ) / 100;
                    orig->gold += gold;
                    victim->gold -= gold;
                    sprintf( buf, "你獲得了 "HIY"%d"NOR" 枚金幣.\n\r", gold );
                    send_to_char( buf, orig );
                    sprintf( buf, "你失去了 "HIY"%d"NOR" 枚金幣.\n\r", gold );
                    send_to_char( buf, victim );
                }
            }
        }

        //raw_kill( ch, victim );

        //做一些victim死掉時要做的事, 基本上這邊要能 return的條件
        //就是 victim 是 npc, 且有 death prog 時
        if( orig != victim )
        {
            if( !raw_kill( orig, victim ) ) return dam;
        }
        else
        {
            if( !raw_kill(ch, orig)) return dam;
        }

          /* Ok, now we want to remove the deleted flag from the
           * PC victim.
           */

        if (    !IS_NPC( victim )
             && !IS_SET( victim->act, PLR_HORSE ) )
        {
            victim->deleted = FALSE;
        }

        //處理 自動撿錢 自動分錢 自動撿戰利品 自動奉獻屍體
        //pc 用反向 ko npc 時
		// 將自動撿屍另外判定, 避免屍體消失, 屍體沒錢 等情況出現 這裡沒有 corpse 的 字串 2022/05/08
        if ( orig != victim && !IS_NPC( orig ) && IS_NPC( victim ) )
        {
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( orig , victim , cbuf );
        }
        //pc 正常 ko npc 時
        else if ( orig == victim && !IS_NPC(ch) && IS_NPC(victim))
        {
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( ch , victim , cbuf );
        }

        /*
         * Remove victims of pk who no longer have exps left
         */
        //當玩家死掉後, 假如exp減到沒了, 導致被系統 deny時, 強制玩家 quit
        if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_DENY ) )
        {
            do_quit( victim, "" );
        }

        return dam;
    }

    //到這邊的話表示 受攻擊方(victim) 還活著 XD

    //到這邊 victim = ch 有兩種情況了
    //1. 自己打自己
    //2. ch 被 orig 反向
    //(也就是說被反向打到的話, 沒有 斷線自動recall 跟 wimpy自動逃跑 的判定就是了..)
    if ( victim == ch )
    {
        return dam;
    }
    
    /* remove this setting -- by keelar
    //斷線中的 pc, 遭受攻擊時, 有一定機率自動 recall
    if (    !IS_NPC( victim )
         && !IS_SET( victim->act, PLR_HORSE )
         && !victim->desc
         && !IS_SET( victim->pcdata->ride, 2 ) )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return dam;
        }
    }
    */

    /*
     * Wimp out?
     */
    //逃跑檢查
    check_wimpout(ch,victim);

    tail_chain( );
    return dam;
}

void magic__damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn )
{
    CHAR_DATA *orig = victim;
	CHAR_DATA *CoVch; // 2021/11/20
    
    if ( victim == NULL ) return;

    if ( victim->position == POS_DEAD ) return;

    if( ch->in_room != victim->in_room ) return;

	// adore 每下傷害成功回血 2022/11/06
	if( is_affected( ch, gsn_regeneracy ) && affected_value(ch, gsn_regeneracy, APPLY_HIT) > 0 )
		ch->hit = UMIN( ch->max_hit , ch->hit + affected_value(ch, gsn_regeneracy, APPLY_HIT) );

    if( dt != skill_lookup("magic missile"))
        dam += ch->amdr;
    
    if(!IS_NPC(ch)){
        if(get_skill_percent(ch, dt) < 10) dam = dam * 90 / 100;
    }

	//spell fury 不作用於 MM 上 18/7/15
	if( dt != skill_lookup("magic missile"))
		dam = check_spellfury(ch,victim,dam);

	dam += GetEnchantmentMagicDamage(ch, victim, dt);  // 2021/10/04 附魔武器對魔法傷害影響

	//MM 不受Mdr 影響 18/7/15
	if( dt != skill_lookup("magic missile") )
		dam -= victim->mdr;
	
    if(dam < 0) dam = 0;

    if( dam > 0 )
    {
		//根據攻擊者的Level對傷害進行調降，以延長高等級的戰鬥
        //dam = check_damage_adjust(ch,dam);

        if( IS_NPC(victim) && dam < victim->mad )
        {
            dam = 0;
        }
    }
    if ( dam > 30000 )
    {
    //char buf [ MAX_STRING_LENGTH ];

    if ( IS_NPC( ch ) && ch->desc )
    {
              bugf( "Damage: %d from %s by %s: > 30000 points with %d dt!",dam, ch->name, ch->desc->original->name, dt );
    }
    else
          {
              bugf("Damage: %d from %s: > 30000 points with %d dt!",dam, ch->name, dt );
    }
    /*dam = 1000;*/
    }

    if ( victim != ch ){
    /*
     * Inviso attacks ... not.
     */
        if ( IS_AFFECTED( ch, AFF_INVISIBLE ) && !IS_SET(ch->special, ALY_INVIS))
        {
            affect_strip( ch, gsn_invis );
            affect_strip( ch, gsn_mass_invis );
            REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
            act( "$n 的身影漸漸浮現.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_SET(ch->special, ALY_HIDE) )
        {
            REMOVE_BIT( ch->affected_by, AFF_HIDE );
            act( "$n 的身影漸漸浮現.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_DARK_RAID ) )
        {
            affect_strip( ch, gsn_dark_raid );
            send_to_char( "你的專注力被打散了！\n\r", ch );
        }

        if ( IS_AFFECTED( ch, AFF_NIGHTSTALK ) )
        {
            affect_strip( ch, gsn_nightstalk );
        }
      /*
       * Certain attacks are forbidden.
       * Most other attacks are returned.
       */
        if ( is_safe( ch, victim ) )
        {
            return;
        }

        check_killer( ch, victim );
    
        if( IsPK(ch, victim) )
            dam /= PK_DMG_DENOMINATOR;

        //計算偷襲的加成與攻擊招喚物的後果
        dam = check_dirty_raid(ch,victim,dam);
		// return 會造成傷害無法顯示
        if (dam < 0) dam = 0;//return;
      /*
       * Damage modifiers.
       */

        if( is_affected(victim, gsn_defensive ) ) dam /=2;
        if( is_affected(ch, gsn_defensive)) dam /=2;
        if ( IS_NPC(victim) )
        {
            if(!IS_NPC(ch) && ch->class == WIZARD)
                dam = dam * 4 / 5;
			// alc 及 sor 魔法傷害加強 2022/04/10
			else if(!IS_NPC(ch) && (ch->class == ALCHEMIST || ch->class == SORCERER) )
				dam = dam * 3 / 4;
            else
                dam /= 2;
           dam = dam * 10 / 7; //modified by tale due to the abnormal of magic damage after server os version changing.
        }

        dam = GetDamageUnderProtection(ch, victim, dam);
        dam = GetMagicalDamageUnderRiding(victim, dam, dt);
        // 種族 anti magic 對魔法傷害減免
        dam = GetMagicalUnderAntiMagic(ch, victim, dam);
		// 盾牌減傷 2020/10/30
		dam = GetMagicalWithShield(ch, victim, dam);
    }

    if ( dam < 0 ) dam = 0;

	// mystic armor 移置魔法傷害判定下 2020/10/26
	if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
		&& dt != gsn_fire_breath
		&& dt != gsn_acid_breath
		&& dt != gsn_frost_breath
		&& dt != gsn_gas_breath
		&& dt != gsn_lightning_breath )
	{
		dam = MysticArmorCount( victim , dam );
		// 跑到這 dam <= 0 , 代表 mystic armor 把法術直接防掉 2023/01/09
		if( dam <= 0 ) return;
	}

	// lifeshare 效果 2021/11/20
	if( is_affected(victim, gsn_lifeshare) && dt != gsn_lifeshare ){
		if( victim->linking != NULL )
			dam = LifeShareDamage( victim , ch , dam );
	}

	// blood thirsty 給予傷害計數 2022/02/20
	if( !IS_NPC(ch) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(ch, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( ch , dam );
		}
	}
	// blood thirsty 被傷害計數 2022/02/20
	if( !IS_NPC(victim) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(victim, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( victim , dam*2 );
		}
	}

    /*
     * We moved dam_message out of the victim != ch if above
     * so self damage would show.  Other valid type_undefined
     * damage is ok to avoid like mortally wounded damage - Kahn
     */

    if ( dt != TYPE_UNDEFINED )
    {
        dam_message( ch, victim, dam, dt, wpn );
    }

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */

    victim->hit -= UMAX(dam,0);

    if( IS_NPC(victim) ){
        mprog_hurt_trigger(victim, ch, dt);
		// 法術攻擊會觸發特定屬性元素屬性的 dt hurt prog 2022/12/25
		if( IsSpellElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_magic_enchantment );
		if( IsWindElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_wind_enchantment );
		if( IsFireElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_flame_enchantment );
		if( IsColdElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_water_enchantment );
		if( IsEarthElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_earth_enchantment );
		if( IsLightningElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_lightning_enchantment );
		if( IsSaintElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_saint_enchantment );
		if( IsDarkElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_dark_enchantment );
	}

    if (    !IS_NPC( victim )
         && IS_IMMORTAL( victim )
         && victim->hit < 1 )
    {
        victim->hit = 1;
    }
	
	// mar 在 blood thirsty buff 效果內不會被打死 add at 2020/10/16
	if ( is_affected( victim, gsn_blood_thirsty) && affected_value(victim, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 
		&& get_skill_percent(victim, gsn_blood_thirsty) > 0 && victim->hit < 1 )
	{
		victim->hit = 1;
	}

    if (    is_affected( victim, gsn_berserk )
         && victim->position <= POS_STUNNED )
    {
		affect_strip( victim, gsn_berserk );
    }

    check_position(ch, victim, dam );


    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE( victim ) && victim->fighting )
        stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
		if( orig != victim )
			group_gain( orig, victim );
		else    
			group_gain(ch, orig);

		if ( !IS_NPC( victim ) )
		{
			int  gold = 0;
			//int  exp  = 0;
			char buf[BUF_STRING_LENGTH];

			if( victim->in_room )
			{
				sprintf( log_buf, "%s(%s)被%s(%s)殺於%d",victim->short_descr, victim->name,
									orig->short_descr, orig->name,victim->in_room->vnum );
				log_string( log_buf );
			}

			  /*
			   * Dying penalty:
			   * 1/2 way back to previous 2 levels.
			   */
			if( orig != victim )
				check_corpse_count(orig, victim);
			else 
				check_corpse_count(ch, victim);

			//if ( victim->exp > EXP_PER_LEVEL( victim->level -2 ) * ( victim->level - 1 ) ){
			//    gain_exp( victim, ( EXP_PER_LEVEL( victim->level - 2 ) * ( victim->level - 1 ) 
			//                   - victim->exp ) / 30 );
			// }
			if ( victim->in_room && !IS_SET(victim->in_room->room_flags, ROOM_PK))
			{
				if ( orig != victim && !IS_NPC(orig) && !IS_NPC(victim))
				{
					gold = victim->gold * number_range( 10, 20 ) / 100;
					orig->gold += gold;
					victim->gold -= gold;
					sprintf( buf, "你獲得了 "HIY"%d"NOR" 枚金幣.\n\r", gold );
					send_to_char( buf, orig );
					sprintf( buf, "你失去了 "HIY"%d"NOR" 枚金幣.\n\r", gold );
					send_to_char( buf, victim );
				}
			}
		}

		if( !raw_kill(ch, victim)) return;

		  /* Ok, now we want to remove the deleted flag from the
		   * PC victim.
		   */

		if ( !IS_NPC( victim ) && !IS_SET( victim->act, PLR_HORSE ) )
			victim->deleted = FALSE;

		// 將自動撿屍另外判定, 避免屍體消失, 屍體沒錢 等情況出現 這裡沒有 corpse 的 字串 2022/05/08
		if ( orig != victim && !IS_NPC( orig ) && IS_NPC( victim ) )
		{
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( orig, victim  , cbuf );
		}
		else if ( orig == victim && !IS_NPC(ch) && IS_NPC(victim))
		{
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( ch, victim  , cbuf );
		}

		  /*
		   * Remove victims of pk who no longer have exps left
		   */
		if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_DENY ) )
		{
			do_quit( victim, "" );
		}
		return;
	}

    if ( victim == ch )
		return;

    /*
     * Take care of link dead people.
     */
    if (    !IS_NPC( victim )
         && !IS_SET( victim->act, PLR_HORSE )
         && !victim->desc
         && !IS_SET( victim->pcdata->ride, 2 ) )
    {
		if ( number_range( 0, victim->wait ) == 0 )
		{
			do_recall( victim, "" );
			return;
		}
    }

    /*
     * Wimp out?
     */
	check_wimpout(ch,victim);

	tail_chain( );
	return;
}


int magic_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn )
{

    //OBJ_DATA *obj;
    //int sntemp;
    CHAR_DATA *orig = victim;
	CHAR_DATA *CoVch; // 2021/11/20
    //int count = 0;
    
    if ( victim == NULL ) return;

    if ( victim->position == POS_DEAD )
            return -1;

    if( ch->in_room != victim->in_room ) return -1;

    dam += ch->amdr;

    if(!IS_NPC(ch)){
        dam += dam;
        if(get_skill_percent(ch, dt) < 10) dam = dam * 90 / 100;
    }
	//spell fury 不作用於以下 spell 18/7/23
	if(    dt != gsn_vampiric_bite
		&& dt != gsn_fire_breath
		&& dt != gsn_acid_breath
		&& dt != gsn_frost_breath
		&& dt != gsn_gas_breath
		&& dt != gsn_lightning_breath )
		dam = check_spellfury(ch,victim,dam);

	dam += GetEnchantmentMagicDamage(ch, victim, dt);  // 2021/10/04 附魔武器對魔法傷害影響

    if( dam > 0 ){

        dam -= victim->mdr;
        if(dam < 0) dam = 0;
        //根據攻擊者的Level對傷害進行調降，以延長高等級的戰鬥
        //dam = check_damage_adjust(ch,dam);

        if( IS_NPC(victim) && dam < victim->mad ) dam = 0;
        if(dam < 0) dam = 0;
    }
 
    if ( dam > 30000 )
    {
        if ( IS_NPC( ch ) && ch->desc )
            bugf("Damage: %d from %s by %s: > 30000 points with %d dt!",dam, ch->name, ch->desc->original->name, dt );
        else
            bugf("Damage: %d from %s: > 30000 points with %d dt!",dam, ch->name, dt );
    }


    if ( victim != ch )
    {
        if ( IS_AFFECTED( ch, AFF_INVISIBLE ) && !IS_SET(ch->special, ALY_INVIS))
        {
            affect_strip( ch, gsn_invis      );
            affect_strip( ch, gsn_mass_invis );
            REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
            act( "$n 的身影漸漸浮現.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_SET(ch->special, ALY_HIDE) )
        {
            REMOVE_BIT( ch->affected_by, AFF_HIDE );
            act( "$n 的身影漸漸浮現.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_DARK_RAID ) )
        {
            affect_strip( ch, gsn_dark_raid );
            send_to_char( "你的專注力被打散了！\n\r", ch );
        }

        if ( IS_AFFECTED( ch, AFF_NIGHTSTALK ) )
        {
            affect_strip( ch, gsn_nightstalk );
        }

        if ( is_safe( ch, victim ) )
        {
            return -1;
        }

        check_killer( ch, victim );

        //計算偷襲的加成與攻擊招喚物的後果
        dam = check_dirty_raid(ch,victim,dam);
        if (dam < 0) dam = 0;//return -1;

        /*
         * Damage modifiers.
         */

		/*  移除聖光傷害減半的效果, 直接使所有不透光傷半減, 相當於所有人獲得天生聖光 2022/10/26

        if (    IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities,RACE_SANCT ) )
        {
            dam /= 2;
        } */
		dam /= 2;

        if ( IS_NPC(victim) )
        {
            if(!IS_NPC(ch) && ch->class == WIZARD)
                dam = dam * 4 / 5;
			// alc 及 sor 魔法傷害加強 2022/04/10
			else if(!IS_NPC(ch) && (ch->class == ALCHEMIST || ch->class == SORCERER) )
				dam = dam * 3 / 4;
            else
                dam /= 2;
	
           dam = dam * 10 / 7; //modified by tale due to the abnormal of magic damage after server os version changing.
        }

        //神靈附體 傷害減少1/3
        if( is_affected(victim, gsn_invoke ))
        {
            dam = (dam * 2)/3;
        }

        if( is_affected(victim, gsn_defensive ) )
        {
            dam /=2;
        }

        if( is_affected(ch, gsn_defensive))
        {
            dam /=2;
        }

        //Protection的作用
        dam = GetDamageUnderProtection(ch, victim, dam);
        dam = GetMagicalDamageUnderRiding(victim, dam, dt);
        // 種族 anti magic 對魔法傷害減免
        dam = GetMagicalUnderAntiMagic(ch, victim, dam);
		// 盾牌減傷 2020/10/30
		dam = GetMagicalWithShield(ch, victim, dam);
    }
    
    if( dam > 0 && check_evade(ch, victim) ) 
    {
        if( !is_chinese(victim, NULL))
        {
            send_to_char( "You passed by the spell, Huh.\n\r", victim );
            act( "$n passed by the spell. What a miracle!", victim, NULL, NULL, TO_ROOM );
        }
            else
            {
                  send_to_char("你成功\閃躲了法術！好險啊！\n\r",victim );
            act( "$n 成功\閃躲了法術！真是帥啊！", victim, NULL, NULL, TO_ROOM );
            }
        return -2;
    }

	// 龍息效果加成 2022/04/06
	if( dam > 0
		&& (   dt == gsn_acid_breath
			|| dt == gsn_fire_breath
			|| dt == gsn_frost_breath
			|| dt == gsn_gas_breath
			|| dt == gsn_lightning_breath ) )
	{
		AFFECT_DATA af;
		int nBreathSklv = get_skill_level(ch,dt);
		int nVchSvB = NEW_GET_SVS(victim, MAGIC_BREATH);
		int nBreathDecay = 0;
		int nBreathSvS = 0;
		int nSvSDiff = 0;

		// wpn 代入以裝備 hp 時的 sklv
		if( wpn != WEAR_NONE ){
			nBreathSklv = wpn;
			wpn = WEAR_NONE;
		}

		nBreathDecay = nBreathSklv * nBreathSklv;

		if( dt == gsn_acid_breath )
			nBreathSvS = MAGIC_EARTH;
		if( dt == gsn_fire_breath )
			nBreathSvS = MAGIC_FIRE;
		if( dt == gsn_frost_breath )
			nBreathSvS = MAGIC_COLD;
		if( dt == gsn_gas_breath )
			nBreathSvS = MAGIC_WIND;
		if( dt == gsn_lightning_breath )
			nBreathSvS = MAGIC_LIGHTNING;

		nSvSDiff = NEW_GET_SVS(victim, nBreathSvS) - NEW_GET_SVS(ch, nBreathSvS);

		// 毒息判定與其他種類不同
		if( dt == gsn_gas_breath )
		{
			// 與對手抗性差影響最大上限
			if( nSvSDiff >= 0 )
				nSvSDiff = 40 + (1 + nBreathSklv) * nSvSDiff;
			else
				nSvSDiff = 30 + (10 * nSvSDiff)/(10 + nBreathSklv);
			// 上限設定
			if( affected_value(victim, dt , APPLY_SAVING_POISON ) < nSvSDiff * (1 + nBreathDecay )/( 1 + get_curr_con(victim) ) )
			{
				// 不會中毒 或 已有 debuff 效果, 就不顯示敘述
				if(    !affected_value(victim, dt , APPLY_SAVING_POISON) > 0
					&& !IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON )
					&& !IS_SET( victim->special, NO_POISON ) )
				{
					act( "$N"HIG"被劇毒吐息籠罩，他的臉色略為泛青。"NOR"", ch, NULL, victim, TO_CHAR );
					act( HIG "劇毒吐息籠罩你的四周，一陣噁心感讓你相當難受。"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$N"HIG"被劇毒吐息籠罩身體顫抖了一下，表情變得有些苦澀。"NOR"", ch, NULL, victim, TO_NOTVICT );
				}
				af.type = dt;
				if( affected_value(victim, dt , APPLY_SAVING_POISON) > 0 )
					af.duration = 0;
				else
					af.duration = 12;
				af.location = APPLY_SAVING_POISON;
				// no poison 則不上 poison 效果 , 但依然會受到強化
				if( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) || IS_SET( victim->special, NO_POISON ) )
					af.bitvector = 0;
				else
					af.bitvector = AFF_POISON;
				if( nVchSvB <= 0 )
					af.modifier = UMAX( nBreathSklv*nBreathSklv , ( dam * (100 + nBreathDecay*2 + (nVchSvB*3)/2 ) ) / ( (100 + nBreathDecay*2) * 60 ) );
				else
					af.modifier = dam * (100 + nBreathSklv*nBreathSklv + nVchSvB ) / (100 * 15);
				affect_join( victim, &af );
				// 超過上限就設為上限
				if( affected_value(victim, dt , APPLY_SAVING_POISON ) > nSvSDiff * (1 + nBreathDecay )/( 1 + get_curr_con(victim) ) )
				{
					af.type = dt;
					af.duration = 0;
					af.location = APPLY_SAVING_POISON;
					if( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) || IS_SET( victim->special, NO_POISON ) )
						af.bitvector = 0;
					else
						af.bitvector = AFF_POISON;
					af.modifier = nSvSDiff * (1 + nBreathDecay )/( 1 + get_curr_con(victim) ) - affected_value(victim, dt , APPLY_SAVING_POISON);
					affect_join( victim, &af );
				}
			}
		}
		else
		{
			// 與對手抗性差影響最大上限
			if( nSvSDiff >= 0 )
				nSvSDiff = (1 + nBreathSklv) * nSvSDiff;
			else
				nSvSDiff = (100 * nSvSDiff) / (10 + nBreathSklv);
			// 上限設定
			if( affected_value(victim, dt , APPLY_DAMAGE_COUNT ) < 76 + 9 * nBreathDecay + nSvSDiff )
			{
				// 已有 debuff 效果就不顯示敘述
				if( !affected_value(victim, dt , APPLY_DAMAGE_COUNT) > 0 )
				{
					if( dt == gsn_acid_breath ){
						act( "$N"HIG"被強酸吐息噴中，酸液附著到他身上。"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIG "強酸吐息朝你噴來，你身上沾附到腐蝕的酸液！"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIG"被強酸吐息噴中，酸液附著到他身上。"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
					if( dt == gsn_fire_breath ){
						act( "$N"HIR"被灼熱的吐息噴中，身體冒出陣陣熱煙。"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIR "灼熱的吐息朝你噴來，你身體冒出陣陣熱煙！"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIR"被灼熱的吐息噴中，身體冒出陣陣熱煙。"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
					if( dt == gsn_frost_breath ){
						act( "$N"HIC"被冰冷的吐息噴中，皮膚凍結出層層冰霜。"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIC "冰冷的吐息朝你噴來，你的皮膚結出層層冰霜！"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIC"被冰冷的吐息噴中，皮膚凍結出層層冰霜。"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
					if( dt == gsn_lightning_breath ){
						act( "$N"HIY"被雷電吐息噴中，全身流串著高壓電流。"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIY "雷電吐息朝你噴來，你全身流串著高壓電流！"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIY"被雷電吐息噴中，全身流串著高壓電流。"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
				}
				af.type = dt;
				if( affected_value(victim, dt , APPLY_DAMAGE_COUNT) > 0 )
					af.duration = 0;
				else
					af.duration = 3;
				af.location = APPLY_DAMAGE_COUNT;
				af.bitvector = 0;
				if( nVchSvB <= 0 )
					af.modifier = UMAX( nBreathSklv*nBreathSklv , ( dam * (100 + nBreathDecay*2 + (nVchSvB*3)/2 ) ) / ( (100 + nBreathDecay*2) * 60 ) );
				else
					af.modifier = dam * (100 + nBreathSklv*nBreathSklv + nVchSvB ) / (100 * 15);
				affect_join( victim, &af );
				// 超過上限就設為上限
				if( affected_value(victim, dt , APPLY_DAMAGE_COUNT ) > 76 + 9 * nBreathDecay + nSvSDiff )
				{
					af.type = dt;
					af.duration = 0;
					af.location = APPLY_DAMAGE_COUNT;
					af.modifier = 76 + 9 * nBreathDecay + nSvSDiff - affected_value(victim, dt , APPLY_DAMAGE_COUNT );
					affect_join( victim, &af );
				}
			}
		}
	}
	
	// 龍息 wpn 代入裝備 hp 觸發的 sklv , 但傷害為 0 則把 wpn 轉換為 WEAR_NONE 2022/04/16
	if( wpn != WEAR_NONE )
		wpn = WEAR_NONE;

	if( IsPK(ch, victim) )
	{
		dam /= PK_DMG_DENOMINATOR;
	}
	
	// mystic armor 移置魔法傷害判定下 2020/10/26
	if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
		&& dt != gsn_fire_breath
		&& dt != gsn_acid_breath
		&& dt != gsn_frost_breath
		&& dt != gsn_gas_breath
		&& dt != gsn_lightning_breath )
	{
		dam = MysticArmorCount( victim , dam );
		// 跑到這 dam <= 0 , 代表 mystic armor 把法術直接防掉 2023/01/09
		if( dam <= 0 ) return;
	}

    /*
     * We moved dam_message out of the victim != ch if above
     * so self damage would show.  Other valid type_undefined
     * damage is ok to avoid like mortally wounded damage - Kahn
     */

    if ( dt != TYPE_UNDEFINED )
    {
        dam_message( ch, victim, dam, dt, wpn );
    }

    //////////////// 隕石雨視最後傷害之機率效果 ///////////////////////
    if(dt == gsn_meteor_shower){
				int nOrgDmg = dam;
				if( IsPK(ch, victim) ) nOrgDmg *= PK_DMG_DENOMINATOR;
        int nEQSklv = get_skill_level(ch, skill_lookup("earthquake"));
        int nMSSklv = get_skill_level(ch, gsn_meteor_shower);
        if(nMSSklv > 5 && nEQSklv > 5 && nOrgDmg > 1000){
            if( number_percent() < (nOrgDmg - 1000) / 50){
                AFFECT_DATA af;
                af.type      = gsn_paralyze;
                af.duration  = 0;
                af.location  = APPLY_DODGE;
                af.modifier  = 100;
                af.bitvector = AFF_PARALYZED;
                affect_to_char( victim, &af );
                act( "$N"HIY"受到強大的隕石攻擊，身體頓時動彈不得！"NOR, ch, NULL, victim, TO_CHAR  );
                send_to_char( "你受到強大的隕石攻擊，身體頓時動彈不得！\n\r", victim);
                act( "$N"HIY"受到強大的隕石攻擊，身體頓時動彈不得！"NOR, ch, NULL, victim, TO_NOTVICT );
            }
        }
    }

	// lifeshare 效果 2021/11/20
	if( is_affected(victim, gsn_lifeshare) && dt != gsn_lifeshare ){
		if( victim->linking != NULL )
			dam = LifeShareDamage( victim , ch , dam );
	}

	// blood thirsty 給予傷害計數 2022/02/20
	if( !IS_NPC(ch) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(ch, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( ch , dam );
		}
	}
	// blood thirsty 被傷害計數 2022/02/20
	if( !IS_NPC(victim) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(victim, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( victim , dam*2 );
		}
	}

	// adore 每下傷害成功回血 2022/11/06
	if( is_affected( ch, gsn_regeneracy ) && affected_value(ch, gsn_regeneracy, APPLY_HIT) > 0 )
		ch->hit = UMIN( ch->max_hit , ch->hit + affected_value(ch, gsn_regeneracy, APPLY_HIT) );

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */

    victim->hit -= UMAX(dam,0);

    if( IS_NPC(victim) )
    {
        mprog_hurt_trigger(victim, ch, dt);
		// 法術攻擊會觸發特定屬性元素屬性的 dt hurt prog 2022/12/25
		if( IsSpellElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_magic_enchantment );
		if( IsWindElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_wind_enchantment );
		if( IsFireElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_flame_enchantment );
		if( IsColdElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_water_enchantment );
		if( IsEarthElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_earth_enchantment );
		if( IsLightningElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_lightning_enchantment );
		if( IsSaintElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_saint_enchantment );
		if( IsDarkElement( ch, dt, NULL, TRUE ) )
			mprog_hurt_trigger(victim, ch, gsn_dark_enchantment );
    }

    if (    !IS_NPC( victim )
         && IS_IMMORTAL( victim )
         && victim->hit < 1 )
    {
        victim->hit = 1;
    }
	
	// mar 在 blood thirsty buff 效果內不會被打死 add at 2020/10/16
	if ( is_affected( victim, gsn_blood_thirsty) && affected_value(victim, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 
		&& get_skill_percent(victim, gsn_blood_thirsty) > 0 && victim->hit < 1 )
	{
		victim->hit = 1;
	}

    if (    is_affected( victim, gsn_berserk )
         && victim->position <= POS_STUNNED )
    {
            affect_strip( victim, gsn_berserk );
    }

    check_position(ch, victim,dam);


    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE( victim ) && victim->fighting )
    {
            stop_fighting( victim, FALSE );
    }
    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
        if( orig != victim )
        {
            group_gain( orig, victim );
        }
        else
        {    
            group_gain(ch, orig);
        }

        if ( !IS_NPC( victim ) )
        {
            int  gold = 0;
            char buf[BUF_STRING_LENGTH];

            if( victim->in_room )
            {
                sprintf( log_buf, "%s(%s)被%s(%s)殺於%d",victim->short_descr,
                         victim->name, orig->short_descr, orig->name,victim->in_room->vnum );

                log_string( log_buf );
            }

              /*
               * Dying penalty:
               * 1/2 way back to previous 2 levels.
               */
            if ( orig != victim )
                check_corpse_count(orig, victim);
            else 
                check_corpse_count(ch, victim);

            
            if ( victim->in_room && !IS_SET(victim->in_room->room_flags, ROOM_PK))
            {
                if ( orig != victim && !IS_NPC(orig) && !IS_NPC(victim))
                {

                    gold = victim->gold * number_range( 10, 20 ) / 100;
                    orig->gold += gold;
                    victim->gold -= gold;
                    sprintf( buf, "你獲得了 "HIY"%d"NOR" 枚金幣.\n\r", gold );
                    send_to_char( buf, orig );
                    sprintf( buf, "你失去了 "HIY"%d"NOR" 枚金幣.\n\r", gold );
                    send_to_char( buf, victim );
                }
            }
        }

        if( !raw_kill(ch, victim))
        {
            return -1;
        }
          /* Ok, now we want to remove the deleted flag from the
           * PC victim.
           */
        if ( !IS_NPC( victim ) && !IS_SET( victim->act, PLR_HORSE ) )
        {
            victim->deleted = FALSE;
        }

		// 將自動撿屍另外判定, 避免屍體消失, 屍體沒錢 等情況出現 這裡沒有 corpse 的 字串 2022/05/08
        if ( orig != victim && !IS_NPC( orig ) && IS_NPC( victim ) )
        {
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( orig, victim  , cbuf );
        }
        else if ( orig == victim && !IS_NPC(ch) && IS_NPC(victim) )
        {
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( ch, victim  , cbuf );
        }

          /*
           * Remove victims of pk who no longer have exps left
           */
        if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_DENY ) )
        {
            do_quit( victim, "" );
        }
        return -1;
    }


    if ( victim == ch )
        return -1;

    /*
     * Take care of link dead people.
     */

    if (    !IS_NPC( victim )
         && !IS_SET( victim->act, PLR_HORSE )
         && !victim->desc
         && !IS_SET( victim->pcdata->ride, 2 ) )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return -1;
        }
    }

    /*
     * Wimp out?
     */
    check_wimpout(ch,victim);

    tail_chain( );
    return -1;
}
