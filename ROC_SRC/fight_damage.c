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
//*  PURPOSE     :         �ק�met���ؼЦ��Qpro����slash��bug     *
//*              :         ��ber���ؼЦ��Qpro�ɷ|�Q������bug      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941013 Razgriz                                *
//*  PURPOSE     :         damage () �[�W�������                 *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941014 Razgriz                                *
//*  PURPOSE     :         ��g damage () ����                    *
//*              :         ���� _damage brief_damage brief__damage*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0981011 Keelar                                *
//*  PURPOSE     :         �j���°ϰ�NPC                    *
//*              :         �����ӧO half_damage �Ҧ� NPC ���N�� half_damage �ĪG*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991207 Tale                                *
//*  PURPOSE     :         ���� half_damage �^�_��l�]�w                   *
//*              :         ���� half_damage �B�^�_�ӧO half_damage�ĪG*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1000410 Tale                                *
//*  PURPOSE     :         �����]�k�ˮ`���v                  *
//*              :         ���ɪ��a�]�k�ˮ`���v 10 / 7 ��*
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

// �����R���������P�w 2022/12/25
bool IsSpellElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsWindElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsFireElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsColdElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsEarthElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsLightningElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsSaintElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );
bool IsDarkElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast );

// �rŧ�קאּ�������\Ĳ�o�~�l�[�ˮ` 2021/11/04
//void VenomousStrike(CHAR_DATA *pCh, CHAR_DATA *pVictim)
// �W�[���~�P�w 2021//11/10
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

	// �קK�۱a�@�r�����~�L�z , �����򥻬r�� 2021/11/10
	if( obj->item_type == ITEM_WEAPON )
		nToxicity += number_range( obj->value[1], obj->value[2] ) / 2;
	if( obj->item_type == ITEM_ARROW || obj->item_type == ITEM_DART )
		nToxicity += number_range( obj->value[1], obj->value[2] ) / 4;
	if( nToxicity < 0 ) nToxicity = 0;

	// �P�w�����ؼЪ����~�������@�r�� 2021/11/10
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
		// �ؼ� svs 0  �� 1.32% ���|�Wpoison , �C sklv + 1%  2021/11/05
		// ���~���r�ʼv�T���r���v�B�W�[���r�ˮ` 2021/11/10
		if((nPoisonSklv + nToxicity/30 + new_saves_spell( pCh->level , pVictim, MAGIC_SPELL)/200)
			> number_percent( )){
			// ���~���r�� ,�ؼ���� �� poison sklv �v�T�ˮ` 2021/11/10
			nCurDmg = (pCh->level + nToxicity) * (38 - nVictimCon) / 20;
			if ( nCurDmg < 0 ) nCurDmg = 0;
			// wp poison �W�� 2020/10/29
			if( affected_value(pVictim, gsn_poison, APPLY_SAVING_POISON) < (nToxicity + (14 + nPoisonSklv*6)*10) ){
				af.type      = gsn_poison;
				af.duration  = 1;
				af.location  = APPLY_SAVING_POISON;
				af.modifier  = nCurDmg;
				af.bitvector = AFF_POISON;
				affect_join( pVictim, &af );
			}
			// �l�[�ԭz 2021/11/11
			act( HIG"�A�Z���W���r���I�J�L���餺�I"NOR"", pCh, NULL, pVictim, TO_CHAR );
			act( HIG "���߷P�P��h�P�P��ŧ�ӡA��誺�Z�����G���r�I"NOR"" , pCh, NULL, pVictim, TO_VICT);
			act( "$N"HIG"�y�Ⲥ���x�C�A�Z�����r���I�J�F�L���餺�I"NOR"", pCh, NULL, pVictim, TO_NOTVICT );
		}
	}else{
		//int nVictimCon = get_curr_con(pVictim);
		// �Y�p�ʤ���v�T , ��j sklv �v�T , �[�J���~�r�ʼv�T 2021/11/10
		nMaxDmg = ( nToxicity + nVenomPerc/2 + nVenomSklv * nVenomSklv*5 ) * (38 - nVictimCon) / 20;
		if(nMaxDmg < 0) nMaxDmg = 0;
		//nMaxDmg = nMaxDmg * new_saves_spell( 10 + nVenomSklv * (nVenomSklv + 4), pVictim, MAGIC_SPELL) / 100;
		//nMaxDmg = nMaxDmg * weakness_damage(pVictim, gsn_poison) / 100;

		// �[�J weakness �R���P�w, �˳Ƭr�ʷ|�v�T���\�v 2021/11/04
		if( (nMaxDmg * weakness_percent(pVictim, gsn_poison) / 100) < number_percent() * (14 - nVenomSklv) )
			return nCurDmg;

		nMaxDmg = nMaxDmg * new_saves_spell( nVenomSklv * (nVenomSklv + 4), pVictim, MAGIC_SPELL) / 100;

		nCurDmg = affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON);

		nDamage = nMaxDmg / 7;
		nCurDmg += nDamage;
		if(nCurDmg > nMaxDmg) nCurDmg = nMaxDmg;
		
		act( HIG"�A�B�Χޥ��N�Z�����r���`�J�L���餺�I"NOR"", pCh, NULL, pVictim, TO_CHAR );
		act( HIG "�A��M�P�쨭�餣�A�A��誺�Z�����G���r�I"NOR"" , pCh, NULL, pVictim, TO_VICT);
		if( !is_affected(pVictim, gsn_venomous_strike) )
			act( "$N"HIG"���ܱo���ǭW�ߡA���G����F�޲��������I"NOR"", pCh, NULL, pVictim, TO_NOTVICT );
		
		// �C�ƭȪ��r�������|�|�L������ 2021/11/04
		if(affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON) > nCurDmg)
			nCurDmg = affected_value(pVictim, gsn_venomous_strike, APPLY_SAVING_POISON);
		
		af.type      = gsn_venomous_strike;
		af.duration  = 1;
		af.bitvector = AFF_POISON;
		af.location  = APPLY_SAVING_POISON;
		af.modifier  = nCurDmg; // �[�W�˳Ƭr�� 2021/11/10
		affect_strip(pVictim, gsn_venomous_strike);
		affect_join( pVictim, &af );

		// �rŧ���ˮ`�p�� 2021/11/04
		nCurDmg = (nCurDmg * (1 + nVenomSklv*4) )/10;
		// �ˮ`�[�J�ؼз�e�r���� 2022/04/23
		if( NEW_GET_SVS(pVictim,MAGIC_POISON) >= 0 )
			nCurDmg = nCurDmg + (5 * NEW_GET_SVS(pVictim,MAGIC_POISON))/(10 - nVenomSklv);
		else
			nCurDmg = nCurDmg + NEW_GET_SVS(pVictim,MAGIC_POISON)/(1 + nVenomSklv/6);
		// �ˮ`�[�W�˳Ƭr�� 2021/11/10
		nCurDmg = nCurDmg * weakness_damage(pVictim, gsn_poison) / 100;
	}
	// �קK�t��
	if( nCurDmg < 0 ) nCurDmg = 0;
	return nCurDmg;
}

// �ԫ����۰ʳB�z�P�w 2022/05/08
void AutoCorpse( CHAR_DATA *ch , CHAR_DATA *victim , char *vchname )
{
	int split = 0;
	char buf[10];
	char cbuf[MAX_INPUT_LENGTH];
	OBJ_DATA *corpse;

	if( !ch ) return;

	if( !victim ) return;

	corpse = get_obj_list( ch, vchname, ch->in_room->contents );

	if( !corpse ) return; // ���e purge �S�����骺���p�N return

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
	// ch �۴ݪ����p�U
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
	// �[�J�j��g�� , �t�����Y , ���ħ��Y 2022/04/29
	//if(nDmgType == 1014 || nDmgType == 1015 || nDmgType == gsn_throwing ){
	if(nDmgType == 1014
	|| nDmgType == 1015
	|| nDmgType == 1016 // gun shoot
	|| nDmgType == gsn_throwing // �@����Y���~
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

// �ק� protection ���P�}��ĪG 2022/10/26
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
	
	// �p�G ch �P vitctim �P�}��h���@�� 2022/10/26
	if(    ( IS_GOOD( ch ) && IS_GOOD( victim ) )
		|| ( IS_NEUTRAL( ch ) && IS_NEUTRAL( victim ) )
		|| ( IS_EVIL( ch ) && IS_EVIL( victim ) ) )
	{
		nDamage = nDamage * nDamageRate / 100;
		return nDamage;
	}

	if( IS_SET( race_table[ victim->race ].race_abilities, RACE_PROTECTION ) || IS_AFFECTED(victim, AFF_PROTECT) )
	{
		// npc �P�w
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

// �ر� Anti Magic ���]�k�ˮ`��K
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

// �޵P���z��� 2020/10/30
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

	// charge �ޯ൥�ŭ��C�޵P��� , lv 0 �� 40%  lv 6 ���C 94% , 2022/04/03
	if( dt == gsn_charge )
		reduce = reduce - reduce * (40 + 9 * Charge_Sklv) / 100;

	// npc block �ʯ�P���Ŧ��� , lv 120 �~�֦� block lv 6 ���ĪG 2022/02/18
	if( IS_NPC(victim) ){
		Block_Sklv = UMAX( 6 , victim->level/20 );
		Block_Perc = victim->level;
	}

	// break shield ���m�Χޯ൥�Ŧ����|�L���޵P��� , block ���m�Χޯ൥�żv�T���Q��z 2022/02/18
	if( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->class, 8 ) ) )
	{
		if( Break_Perc/20 + Break_Sklv * 10 >= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n �B�Χޥ���}�A���޵P���m!"NOR :
				HIY"$n break through your shield defend!"NOR,  ch, NULL, victim, TO_VICT );
			return nDamage;
		}
	}

	// powerup ���A�U, �L���޵P��˪����v�P dr �[������ , block ���m�Χޯ൥�żv�T���Q��z 2022/02/18
	if( is_affected(ch, gsn_powerup) )
	{
		//if( 3 * ( get_skill_level(ch , gsn_powerup ) + get_curr_str(ch) - get_curr_str(victim) ) >= number_percent() )
		if( Power_Perc/33 + affected_value( ch, gsn_powerup , APPLY_DAMROLL ) * 2
			>= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n �B�εo�l��}�A���޵P���m!"NOR :
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

// �޵P�]�k��� 2020/10/30
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

	// npc block �ʯ�P���Ŧ��� , lv 120 �~�֦� block lv 6 ���ĪG 2022/02/18
	if( IS_NPC(victim) ){
		Block_Sklv = UMAX( 6 , victim->level/20 );
		Block_Perc = victim->level;
	}

	// break shield ���m�Χޯ൥�Ŧ����|�L���޵P��� , block ���m�Χޯ൥�żv�T���Q��z 2022/02/18
	if( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->class, 8 ) ) )
	{
		if( Break_Perc/20 + Break_Sklv * 10 >= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n �B�Χޥ���}�A���޵P���m!"NOR :
				HIY"$n break through your shield defend!"NOR,  ch, NULL, victim, TO_VICT );
			return nDamage;
		}
	}

	// powerup ���A�U, �L���޵P��˪����v�P dr �[������ , block ���m�Χޯ൥�żv�T���Q��z 2022/02/18
	if( is_affected(ch, gsn_powerup) )
	{
		//if( 3 * ( get_skill_level(ch , gsn_powerup ) + get_curr_str(ch) - get_curr_str(victim) ) >= number_percent() )
		if( Power_Perc/33 + affected_value( ch, gsn_powerup , APPLY_DAMROLL ) * 2
			>= number_percent() + Block_Perc/20 + Block_Sklv * Block_Sklv )
		{
			act( (is_chinese(victim, ch)) ?
				HIY"$n �B�εo�l��}�A���޵P���m!"NOR :
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

// �����R���������P�w 2022/12/25
bool IsSpellElement( CHAR_DATA *ch, int dt, OBJ_DATA *wpn , bool IsCast )
{
	if(    dt == 9     // cause critical
		|| dt == 10    // cause light
		|| dt == 11    // cause serious
		|| dt == 44    // powerball
		|| dt == 60    // magic missile
		|| dt == 140 ) // high explosive
		return TRUE;

	// �p�G�O�k�N, �S�]�� dt �N�| false
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

// �˳ƯS�w�����]�ƭȧP�w 2022/12/25
int GetObjElement( OBJ_DATA *obj , int type )
{
	int EncDam = 0;
	int EncTemp = 0;

	if( obj == NULL )
		return EncDam;

	if( !type )
		return EncDam;

	AFFECT_DATA *o_paf;

	for ( o_paf = obj->affected; o_paf; o_paf = o_paf->next ) // ���~�B�~�[�J�� affect
	{
		if ( o_paf->location == type ){
			EncTemp = o_paf->modifier;
			EncDam += EncTemp;
		}
	}
	for ( o_paf = obj->pIndexData->affected; o_paf; o_paf = o_paf->next ) // �g�b area ���� affect
	{
		if ( o_paf->location == type ){
			EncTemp = o_paf->modifier;
			EncDam += EncTemp;
		}
	}
	for ( o_paf = obj->material->affected; o_paf; o_paf = o_paf->next ) // ����
	{
		if ( o_paf->location == type ){
			EncTemp = o_paf->modifier;
			EncDam += EncTemp;
		}
	}

	return EncDam;
}

// ���]�k�N�ˮ`�p�� 2021/10/04
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

	// alc artifacts mastery �[�� 2022/04/29
	if( !IS_NPC(ch) && EncDam > 0 )
		EncDam = EncDam *( 6 + AmSklv)/6;

	return EncDam;
}

// ���]�ˮ`�p�� 2021/10/06
int PhysicalElemental(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
	AFFECT_DATA *paf;
	int EncDam = 0;
	int EncTemp = 0;
	int VchMdr = 0;
	int AmSklv = get_skill_level(ch, gsn_artifacts_mastery); // 2022/04/29

	// ���˪��]���p��t��
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
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ) ) // inborn detect align ����t�[��
			EncTemp = ( EncTemp * (24 + get_skill_level(ch, gsn_inborn_ability)) )/24;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100;
	}
	if( GetObjElement( obj, APPLY_WEAPON_DARK ) > 0 )
	{
		EncTemp = GetObjElement( obj, APPLY_WEAPON_DARK );
		EncTemp = EncTemp + ( EncTemp * get_curr_int(ch) ) / 200;
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ) ) // inborn detect align ����t�[��
			EncTemp = ( EncTemp * (24 + get_skill_level(ch, gsn_inborn_ability)) )/24;
		EncDam += EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100;
	}

	// �������[���� �˳ƭ@�[�����ӷl
	if( EncDam > 0
		&& ( !IS_NPC(ch) || ( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) ) )
		&& ( obj->item_type == ITEM_ARMOR || obj->item_type == ITEM_WEAPON ) )
			waste_dura(obj);

	// anti_magic inborn �]�k���
	if(IS_SET( race_table[ victim->race ].race_abilities, RACE_ANTI_MAGIC ))
	{
		if(IS_NPC(victim)){
			EncDam = EncDam * 85 / 100;
		}else{
			EncDam = EncDam * (99 - 34 * (get_skill_level(victim, gsn_inborn_ability) 
							* get_skill_level(victim, gsn_inborn_ability) ) / 36) / 100;
		}
	}

	// �ˮ`�� pp �� 1/40 �� 2022/12/20
	if( ch->pp > 0 )
		EncDam = ( EncDam * (40 + ch->pp) )/40;

	// �ˮ`�[�W md 2022/12/20
	if( ch->amdr > 0 )
		EncDam = EncDam + ch->amdr;

	// �ˮ`���� mdr �̫�ˮ`���|���t��
	EncDam = UMAX(0 , EncDam - victim->mdr);

	// mob �]�k���e( at )
	if( IS_NPC(victim) && EncDam < victim->mad )
		EncDam = 0;

	// alc artifacts mastery �[�� 2022/04/29
	if( !IS_NPC(ch) && EncDam > 0 )
		EncDam = EncDam *( 6 + AmSklv)/6;

	return EncDam;
}

// ���]�k�N�ˮ`�[�� 2021/10/04
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

// ���]���z�ˮ`�[�� 2021/10/04
//int GetEnchantmentPhysicalDamage(CHAR_DATA *ch, CHAR_DATA *victim, int dt , int wield)
int GetEnchantmentPhysicalDamage(CHAR_DATA *ch, CHAR_DATA *victim, int dt , OBJ_DATA *wpn , OBJ_DATA *wpn2 )
{
	int nDamage = 0;

	if( wpn == NULL && wpn2 == NULL )
		return nDamage;

	if( dt == 108 ){ // kick �W�ߧP�w
		if( !wpn && !wpn2 )
			return nDamage;
		if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )
			nDamage = nDamage + PhysicalElemental( ch, victim, wpn );
		if( wpn2 && IS_OBJ_STAT( wpn2, ITEM_SPELL ) )
			nDamage = nDamage + PhysicalElemental( ch, victim, wpn2 )/2;
	}else{
		if(    get_eq_char( ch, WEAR_WIELD )  // ������Z��
			|| get_eq_char( ch, WEAR_TWO_HAND )
			|| get_eq_char( ch, WEAR_WIELD_2 ))
		{
			if(    dt != 1014  // shot
				&& dt != 1015  // dart �g�� skill �U
				&& dt != 1016
				&& dt != gsn_throwing // throw �g�� skill �U
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
			else{ // charge , icecore ���L�˳Ƨޯ�
				if( !wpn )
					return nDamage;
				if( wpn && IS_OBJ_STAT( wpn, ITEM_SPELL ) )
					nDamage = nDamage + PhysicalElemental( ch, victim, wpn );
			}
		}
		else{  // �L����Z��
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

/*  �ק� blood thirsty �]�w , �G���� 2022/02/22
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

// blood thirsty �ˮ`�p�� 2022/02/20
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

// �w�� ����Z�� �b�o�g�᪺ apdr �ݩʥ[�� 2022/05/06
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

// �p�� mystic armor �ƭȤζˮ`��� 2023/01/09
int MysticArmorCount( CHAR_DATA *ch , int dam )
{

	if( !IS_AFFECTED(ch, AFF_MYSTIC_ARMOR ) )
		return dam;

	int MALv = affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);

	if( dam <= 0 ){
		act( "$n ���]�O�پ��l���F�]�k�I", ch, NULL, NULL, TO_ROOM );
		send_to_char( "�A���]�O�پ��l���F�]�k�I\n\r", ch );
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

		act( "$n ���]�O�پ��l���F�]�k�I", ch, NULL, NULL, TO_ROOM );
		send_to_char( "�A���]�O�پ��l���F�]�k�I\n\r", ch );
	}
	else if( MALv == 0 )
	{
		
		if( get_skill_level(ch, gsn_mystic_armor) > 2 )
			ch->mana += affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) / 50;
		ch->mana = UMIN( ch->max_mana , ch->mana );

		affect_strip(ch, gsn_mystic_armor);
		REMOVE_BIT(ch->affected_by, AFF_MYSTIC_ARMOR);
		
		dam = 0;
		
		act( "$n ���]�O�پ��l���]�k��}�H�F�I", ch, NULL, NULL, TO_ROOM );
		send_to_char( "�A���]�O�پ��l���]�k��}�H�F�I\n\r", ch );
	}
	else if( MALv < 0 )
	{
		if( get_skill_level(ch, gsn_mystic_armor) > 2 )
			ch->mana += affected_value(ch, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) / 100;
		ch->mana = UMIN( ch->max_mana , ch->mana );

		affect_strip(ch, gsn_mystic_armor);
		REMOVE_BIT(ch->affected_by, AFF_MYSTIC_ARMOR);

		dam = dam + MALv;

		act( "$n ���]�O�پ��Q����F�I", ch, NULL, NULL, TO_ROOM );
		send_to_char( "�A���]�O�پ��Q����F�I\n\r", ch );
	}
	return dam;
}


/*
 * Inflict damage from a hit.
 * �[�J�������  by Razgriz 20051013
 * ������g by Razgriz 20051014
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
	int  isDart   = (dt == 1015);  //0121 dart �P�w
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

	//�ˬd �ϦV���k
	isReflexion = FALSE;
	if( check_reflexion(ch,victim, dt, dam, dexdiff) && dt != gsn_lifeshare )
	{
		// ���� reposte ���Q�ϦV , �קאּ�P�w dmg_mode 2021/11/16
		//if(dt != 1014){
		if( dmg_mode == DMG_NORMAL && dt != 1014 && dt != 1016 ){ // �W�[ �j�񤣳Q�ϦV
			// AC �v�T���Ϩ禡
			isReflexion = TRUE;
			int nEnemyAC = GET_AC(victim) * 0.1;
			if(!IS_NPC(victim)){
				if(GET_AC(victim) < 0){
					// ac -1000 �ɬ�����ˮ`�� 1.25 , ac 0 �ɬ�����ˮ`�� 2.5
					nEnemyAC = ( nEnemyAC * nEnemyAC )/ 10 + 1000;
					dam = (dam * 2500) / nEnemyAC;
				}else{
					// ac �j�� 0 �ɬ�����ˮ`�� 2.5 + ac/1000 , �̰��� 4 ( ac 1500 )
					dam = dam * UMIN( 4 , (2500 + GET_AC(victim))/1000);
				}
			}else{
				if(GET_AC(victim) < 0){
					// ac -1000 �ɬ����� 0.9 �� , ac 0 �ɻP����ˮ`�� 1.13 , ����ˮ`�̧C�� 40% 
					nEnemyAC = UMIN( 12500 , (nEnemyAC * nEnemyAC)/10 + 3500 );
					dam = (dam * 4500) / nEnemyAC;
				}else{
					// ac �j�� 0 �ɬ�����ˮ`�� 1 + ac/2000 , �̰��� 3 ( ac 6000 )
					dam = dam * UMIN( 3 , (4500 + GET_AC(victim))/4000);
				}
			}
			if(!IS_NPC(ch))
				dam = dam + UMIN( 0, (GET_AC( ch ) / 2 ) );
			if(IS_NPC(victim))
				dam /= 2;
			// �w�� npc ���e�@��, reflexion ���ഫ victim �� ch , �N�ԭz���ܬ� dodge ���\ �� return -1 2022/12/29
			if( IS_NPC(ch) && ch->hit <= 0 ){
				act( (!is_chinese(ch, victim)) ?
						"$N dodges your attack." :
						"$N �F���a�{�קA������.", ch, NULL, victim, TO_CHAR );
				act( (!is_chinese(victim, ch)) ?
						"You dodges $n's attack.":
						"�A�F���a�{��$n������.", ch, NULL, victim, TO_VICT  );
				return -1;
			}else{
				ch->attacked = victim;
				victim = ch;
			}
		}else{
			// �Y���b�ڧ���, �h�ϦV���b
			if(dt == 1014)
				return 0;
		}
	}

	//�[�W���z�ˮ`�O�P�w  2020/11/01
	if( dt != gsn_lifeshare ){
		dam += ch->apdr;
		// �w�� ����Z�� �� apdr �ĪG�[�� 2022/05/06
		if( isAmmo )
			dam += ShootApdrAffect(ch , get_eq_char( ch, WEAR_AMMO ) );
	}
	// tricky pacing pd �B�~�[�� 2021/11/05
	if( !IS_NPC(ch)
		&& (victim != ch)
		&& dt != gsn_lifeshare  // lifeshare ���ˮ`���|�Q�P�w�� 2021/11/22
		&& get_skill_level(ch, gsn_tricky_pacing) > 0 ){
		dam += ch->apdr * get_skill_level(ch, gsn_tricky_pacing) / 3;
		// �w�� ����Z�� �� apdr �ĪG�[�� 2022/05/06
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
	// �|�y���ˮ`�ƹL�� , �G���� 2020/11/01
	//bool hasIgnoredPDR = FALSE;

	if( dt != gsn_lifeshare )
	{
		//�ھڧ����̪�Level��ˮ`�i��խ��A�H���������Ū��԰�
		//dam = check_damage_adjust(ch,dam);

		//�[�W���z�ˮ`�O , ���� dam > 0 �P�w�e 2020/11/01
		//dam += ch->apdr; 

		//�������A�� �g�X�b�� shot, ���a���@�w���v�H WIS �H�� �b�N �� �y�P�b �� sklv �ӵL������ �ܪ��z�ˮ`
		if (isShot)
		{
			// �w�� ����Z�� �� apdr �ĪG�[�� 2022/05/06
			if( !IS_NPC(ch) ){
				dam += ( ch->apdr + ShootApdrAffect(ch , get_eq_char( ch, WEAR_ARROW ) ) )* nSklvShot / 6;

				// shot �����p�U�� tricky pacing �@ ����Z�� �� apdr �ĪG�[�� 2022/05/06
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

	/*  �ק� blood thirsty �]�w , �G���� 2022/02/22
		// ���a�ݦ媬�A�U�Q�ϦV, �Q�ϦV�ˮ`�j�T���C
		if (isReflexion) {
			dam = GetReflexionDamageUnderBloodThirsty(victim, dam);
		}
	*/


	//�B�z �ˮ`���e��
	if( IS_NPC(victim) && dam <= victim->pad && dt != gsn_lifeshare )
	{
		//�M�h �� 1/4 �����v �L���ˮ`���e��
		if( !IS_NPC(ch) && ch->class == KNIGHT && number_bits(2) >= 1 )
			;
		// ran �L�� gate �|�y���ˮ`�ƹL�� , �G���� 2020/11/01
		//else if(hasIgnoredPDR == FALSE)
		else
			dam = 0;
	}

	//�ˮ`�j�� 30000 �� log �_��
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

	//���p���O�ۤv���ۤv���� ( reflexion �����p�Uvictim = ch )
	// lifeshare ���ˮ`���|�Q�P�w�� 2021/11/22
	if ( victim != ch && dt != gsn_lifeshare )
	{
		// invis �����P�w�㫬
		if ( IS_AFFECTED( ch, AFF_INVISIBLE ) && !IS_SET(ch->special, ALY_INVIS))
		{
			affect_strip( ch, gsn_invis );
			affect_strip( ch, gsn_mass_invis );
			REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
			act( "$n �����v�����B�{.", ch, NULL, NULL, TO_ROOM );
		}

		// hide �����P�w�㫬
		if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_SET(ch->special, ALY_HIDE) )
		{
			REMOVE_BIT( ch->affected_by, AFF_HIDE );
			act( "$n �����v�����B�{.", ch, NULL, NULL, TO_ROOM );
		}

		// nightstalk �����P�w�㫬
		if ( IS_AFFECTED( ch, AFF_NIGHTSTALK ) )
			affect_strip( ch, gsn_nightstalk );

		// dark raid �����P�w
		if ( IS_AFFECTED( ch, AFF_DARK_RAID ) )
		{
			affect_strip( ch, gsn_dark_raid );
			send_to_char( "�A�����e�`�A��ؼЬI�������@���I\n\r", ch );
			darkraid = 1;
		}

		// safe room �����L��
		if ( is_safe( ch, victim ) ) return -2;

		check_killer( ch, victim );

		//pc��pc �� pc��clanpk�B�bpk room ��, �ˮ`�ܦ� 1/8
		if( IsPK(ch, victim)
			|| (  !IS_NPC( victim ) && victim->pcdata->clanpk 
				&& IS_SET(victim->in_room->room_flags, ROOM_PK) ) )
		dam /= PK_DMG_DENOMINATOR;

		//�p�ⰽŧ���[���P�����۳ꪫ����G
		dam = check_dirty_raid(ch,victim,dam);

		// �|�y���ˮ`�L�k��� 2021/11/23
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

	///// �즹���� �����ˮ`(dam) �w�g�w�U�ӤF, ���ᤣ���ӦA���ܰ�

		/* CAST_ACC */
		//�����k�N������ɶ�, �f�۫B(meteor shower), �B���k�P(ice comet), ������z(high explosive)
		if( dam > 0 )
		{
			REMOVE_BIT(victim->affected_by, AFF_CAST_ACC);
		}

		/////�ޯ� disarm trip parry dodge block tactics �B�z�}�l
		/*
				2021/11/16 �ק�

				�ثe�Ȧ��ޯ઺ dmg_mode �|�O DMG_ADJUST ( empslash �� icecore )
				�] gsn_reposte �b fight_hits �� dt �ഫ������, �ӳy������pdb �� reflexion �P�w���@��
				�G�� reposte ��X�� dmg_mode �אּ DMG_ADJUST �ò��� DMG_ADJUST �� pdb ���P�w

			//DMG_ADJUST ��, �u�|�ﴶ���� parry dodge block
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
		//DMG_NORMAL �ɪ� disarm trip parry dodge block tactics �P�_
		if ( dmg_mode == DMG_NORMAL )
		{
			//����, ���L, �D�ݤ��Ocon��pc�g�ɧ���
			//npc �ϥΪ��j�۱ٷ|�Q���v pdb 2022/04/03
			if (   dt >= TYPE_HIT
				|| dt == gsn_kick
				|| ( !IS_NPC(ch)
					&& dt == gsn_frenzy
					&& ch->pcdata->prime_attr != APPLY_CON)
				|| ( IS_NPC(ch) && dt == gsn_circular ) )
			{
				//�B�z ���� �� ���L, �ˮ`��0�ɤ]��@��
				if( dt != gsn_frenzy )
				{
					//�����Z�� (npc�~��)
					if ( IS_NPC( ch )
						&& number_percent( ) < UMIN( 20,UMAX( leveldiff + ( intdiff * 2 ), 5 ) ) )
					{
						disarm( ch, victim );
						if ( !get_eq_char( ch, wpn ) )
						{
							if( dt != gsn_kick )// �קK mob kick ���W�䧮�ܦ��Ť���� 2021/12/19
								dt = TYPE_HIT;//reflexion disarm, ch disarm ch,
						}
					}    //dt should became TYPE_HIT

					//�۰ʲ̭� (npc�~��)
					if (   IS_NPC( ch )
						&& ch->level > 10
						&& number_percent( ) < UMIN( ( leveldiff + intdiff * 4 ), ( IS_SET( ch->class, 1 ) ? 15 : 3 ) ) )
					{
						trip( ch, victim );
					}

					//�D�ݩʬOstr �Ϊ� ¾�~�����Q �� pc, �N�����|�۰ʲ̭˹��
					if (   !IS_NPC( ch )
						&& ( ch->pcdata->prime_attr == APPLY_STR || ch->class == 13 )
						&& number_percent( )
							< UMAX( 15, get_skill_percent(ch, gsn_fool ) / 6
								+ UMAX( 0, ( intdiff + dexdiff ) * 4 ) + leveldiff + 10 ) )
					{
						trip( ch, victim );
					}

					//�ѥ� RACE_WEAPON_WIELD �� mob, ���@�w���v�ϥΨ��W�� scroll wand staff potion pill
					if (   IS_NPC( ch )
						&& IS_SET( race_table[ ch->race ].race_abilities,RACE_WEAPON_WIELD )
						&& number_percent() < UMIN( 25, UMAX( 10, ch->level ) )
						&& !IS_NPC( victim ) )
					{
						use_magical_item( ch ); // �o�Ө禡�S�P�_�n, potion �S�P�_��
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
							// �קK mob kick ���W�䧮�ܦ��Ť���� 2021/12/19
							if( dt != gsn_kick )
								dt = TYPE_HIT;
						}
					}
				}

				//�ˮ`�j��0��
				if ( dam >= 0 )
				{
					//�������A ���O �g�X�b�� shot �� �j��g�� ammo ��
					if ( !isShot && !isAmmo ) // if( dt != 1014 )
					{
						//�g�ɧ���
						if( dt == gsn_frenzy )
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
							//pc �D�ݩʬ�str��kni, �����|�ά޵P�צ� �D�ݩʤ��Ocon���g�ɧ���
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

							//�D�ݩʤ��Ocon���g�ɧ���, �����|�Q parry dodge �ɱ�
							//if ( check_parry( ch, victim, leveldiff ) )
							if ( check_parry( ch, victim, leveldiff, dt , wpn ) )
								return 0;
						}
						//npc ���j�۱٦����|�Qparry�� 2022/04/03
						else if( dt == gsn_circular )
						{
							leveldiff += 5;
							//if ( check_parry( ch, victim, leveldiff ) )
							if ( check_parry( ch, victim, leveldiff, dt , wpn ) )
								return 0;
							if ( check_tactics( victim, ch ) )
								return 0;
						}
						//���ؤ��� �����|�Q dodge tactics �ɱ�.......���O, �o�ӧP�_���Ӷ]����a -_-
						else if( dt == gsn_reposte )
						{
							if ( check_dodge( ch, victim, leveldiff, dexdiff ) )
								return -1;
							if ( check_tactics( victim, ch ) )
								return 0;
						}
						//���� �����|�Q parry dodge block tactics �ɱ�
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
						// shot �~�|�Q block 2022/05/03
						if ( isShot && check_block( ch, victim, leveldiff ) )
							return 0;
					}
				}
			}
			//�D�ݩʬ�con �� npc ���g�ɧ���
			else if( dam >= 0 && dt == gsn_frenzy && !IS_NPC(victim) )
			{
				//�����|�Q�D�ݩʬ�str��kni�H parry dodge block �צ�
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
					//�����|�Qdodge
					if ( frenzy_check_dodge( ch, victim, leveldiff, dexdiff ) )
						return -1;
				}
			}
		}// �ޯ� disarm trip parry dodge block tactics �B�z����
	}//���O�ۤv���ۤv �B�z����

    /*
     * Damage modifiers.
     */

	// �]�O�̻� mana shield  2023/01/09
	// �q fight_hit ���� fight_damage ���B�z, �H�P�w�����ޯ���
	// �å[�J�w�� gsn_lifeshare �� ch �۴ݪ����p���|�@��
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

	// �N�[�^�ª� empslash �� icecore slash ���z�q�}�t�����ĪG����P�w poison �Ϊ��]�e 2022/10/26
	if( dt == gsn_empslash || dt == gsn_icecore_slash )
		dam *= 2;

	// �����R����, �[�J�R�����˳ƧP�w 2022/12/25
	OBJ_DATA *using_wpn = NULL;
	OBJ_DATA *using_wpn2 = NULL;
	using_wpn = check_using_wp( ch, dt , wpn );
	using_wpn2 = check_using_wp2( ch, dt );


	// ���]�ˮ`�[�� 2021/10/06
	// ch ���O victim �B���O lifeshare 2021/11/22
	if ( victim != ch && dt != gsn_lifeshare )
	{
		//dam += GetEnchantmentPhysicalDamage( ch, victim, dt , wpn);
		if( using_wpn || using_wpn2 )
			Enc_Psy_Dam = GetEnchantmentPhysicalDamage( ch, victim, dt , using_wpn , using_wpn2 );
		// mystic armor ��֪��]�ˮ` 2023/01/09
		if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && dt != gsn_lifeshare && ch != victim && Enc_Psy_Dam > 0 )
			Enc_Psy_Dam = MysticArmorCount( victim , Enc_Psy_Dam );
		if( Enc_Psy_Dam > 0 )
			dam += Enc_Psy_Dam;
	}
	
	//�B�z ���r�Z�� ����褤�r(���F�L�k���rflag)
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
		// �rŧ�קאּ�C�U����Ĳ�o�l�[�ˮ`, �ðl�[���~�P�w 2021/11/04
		//VenomousStrike(ch, victim);
		dam += VenomousStrike(ch, victim, wpn );
	}
	
	//�B�z ���r�b�ڮg�� �� �j�u ����褤�r(���F�L�k���rflag)
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
		// �rŧ�קאּ�C�U����Ĳ�o�l�[�ˮ`, �ðl�[���~�P�w 2021/11/04
		//VenomousStrike(ch, victim);
		dam += VenomousStrike(ch, victim, wpn)/2;
	}

	/*  �����t���ˮ`��b���ĪG, ���������Ҧ��˥b��, �۷��Ҧ��H��o�ѥ͸t�� 2022/10/26

		//�t�� �ѥ͸t�� �ˮ`��b
		//�w�� reposte �P�w TYPE_HIT 2021/11/16
		if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities,RACE_SANCT ) )
			&& ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT ) )
			dam /= 2; */
	dam /= 2;


    //�ѥͶˮ`��b(npc only) , �[�J IS_NPC(victim) 2022/12/20
	if( IS_NPC(victim) && IS_SET( victim->special, DAM_HALF ) && dt != gsn_lifeshare)
		dam /= 2;

    //���F���� �ˮ`���1/3
    if( is_affected(victim, gsn_invoke ) && dt != gsn_lifeshare )
        dam = (dam * 2)/3;

    //�@���\�� �ˮ`��b
    if( is_affected(victim, gsn_defensive ) && dt != gsn_lifeshare )
        dam /=2;

    //ch (������)�� �@���\�� �ˮ`��X��b
    if( is_affected(ch, gsn_defensive) )
        dam /=2;

    //�Ѩϥ[�@�N �ѥͤѨϥ[�@�N �i��� 1/4 ���c�������ˮ`
	// �w�� reposte �P�w TYPE_HIT 2021/11/16
    if ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT ){
        dam = GetDamageUnderProtection(ch, victim, dam);
    }
	if ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT ){
		dam = GetPhysicalDamageUnderRiding(victim, dam, dt);
	}
	// ���z�޵P��� 2020/10/30
	if ( dmg_mode == DMG_NORMAL || dt >= TYPE_HIT || dt == gsn_empslash ){
		// ���z�޵P��˥[�J dt �P�w 2022/04/03
		dam = GetPhysicalWithShield(ch, victim, dam , dt);
	}

	// lifeshare �ĪG 2021/11/20
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
	//�e�X�ˮ`�T��
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

	// blood thirsty �����ˮ`�p�� 2021/12/22
	if( !IS_NPC(ch) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(ch, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( ch , dam );
		}
	}
	// blood thirsty �Q�ˮ`�p�� 2022/02/20
	if( !IS_NPC(victim) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(victim, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( victim , dam*2 );
		}
	}

	// adore �C�U�ˮ`���\�^�� 2022/11/06
	if( is_affected( ch, gsn_regeneracy ) && affected_value(ch, gsn_regeneracy, APPLY_HIT) > 0 )
		ch->hit = UMIN( ch->max_hit , ch->hit + affected_value(ch, gsn_regeneracy, APPLY_HIT) );

    //�l��a !!!!!
    victim->hit -= UMAX(dam, 0);

    //Ĳ�o mob hurt prog, �N�O mob �Q�ۭt�ɷQ������ orz
	if( IS_NPC(victim) ){
		mprog_hurt_trigger(victim, ch, dt);
		// �[�J�̿�Z�������z�ˮ` weapon dt �P�w , �i��|�y���G���P�w 2020/09/18
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
		// ���˦p�G�a�������ˮ` �hĲ�o dt �ΰw���ݩʪ� hurt prog 2022/12/25
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

    //�����ڥB���] VERBOSEALL �� debug �T��
    if ( msg_mode == MSG_BRIEF )
    {
		// �[�J dmg_mode ��� 2021/11/16
        //sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR")\n\r", dt, dam);
		sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR", dmg_mode="HIR"%d"NOR")\n\r", dt, dam , dmg_mode);
        if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
            send_to_char (ibuf, ch);
        if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
            send_to_char (ibuf, victim);
    }

    //���ڤ����� XD
    if (    !IS_NPC( victim )
         && IS_IMMORTAL( victim )
         && victim->hit < 1 )
    {
        victim->hit = 1;
    }
	
	// mar �b blood thirsty buff �ĪG�����|�Q���� add at 2020/10/16
	if ( is_affected( victim, gsn_blood_thirsty) && affected_value(victim, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 
		&& get_skill_percent(victim, gsn_blood_thirsty) > 0 && victim->hit < 1 )
	{
		victim->hit = 1;
	}

    //�B�z �����Z��, ����(���F�g�b,�j��) ���L �g�ɧ��� �I�� ¶��, ���|�Q�N��
	if (   !isShot                   //  dt != 1014
		&& !isDart                   //  dt != 1015
		&& !isThrow                  //  ���Y 2022/04/29
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

    //�� �����ε��� �� victim �� berserk �����ޱ�
    if (    is_affected( victim, gsn_berserk )
         && victim->position <= POS_STUNNED )
    {
        affect_strip( victim, gsn_berserk );
    }


    //��s���誺���A--����, �԰�, ��, �·�, ��ı, STUN, ����, ����
    //�o�˪���..�����e �ޱ� berserk ���P�_, ���N�S�ΤF..? @@
    check_position( ch, victim, dam );

    /*
     * Sleep spells and extremely wounded folks.
     */
    //���A�� ��ı STUN ���� ���� ��, �Ѱ��԰����A
    if ( !IS_AWAKE( victim ) && victim->fighting )
        stop_fighting( victim, FALSE );


    //�@�}�l������O ch, ��������O victim = orig
    //���p�ϦV���\, �������ܦ� orig, ���������ܦ� victim = ch
    /*
     * Payoff for killing things.
     */
    //��������(victim) ������
    if ( victim->position == POS_DEAD )
    {
        //��������o�g���
        if( orig != victim )
        {   //�o��O ch(victim) �Q orig ���ϦV���� -_-
            group_gain( orig, victim );
        }
        else
        {   //�o��O ch ���� orig(victim)
            group_gain(ch, orig);
        }

        //pc �Q��
        if ( !IS_NPC( victim ) )
        {
            int  gold = 0;
            char buf[BUF_STRING_LENGTH];

            //�d�� log....���o�� log �O ch �Q orig �Ϧ��� �~�O�諸
            //���p�O���` ch ko orig..�� log_buf ���� orig ���ӭn���� ch �~��
            if( victim->in_room )
            {
                sprintf( log_buf, "%s(%s)�Q%s(%s)����%d",
                victim->short_descr, victim->name, orig->short_descr, orig->name,
                victim->in_room->vnum );
                log_string( log_buf );
            }

	      /*
	       * Dying penalty:
	       * 1/2 way back to previous 2 levels.
	       */
            //�ˬd���
            if( orig != victim )
                check_corpse_count(orig, victim);
            else 
                check_corpse_count(ch, victim);

            //�b�D PK room, pc�H�ϦV����pc��, �i�H�q���̨��W���o�@�w��Ҫ���.....��ԣ�@�w�n�ϦV�� @_@
            if ( victim->in_room && !IS_SET(victim->in_room->room_flags, ROOM_PK))
            {
                if ( orig != victim && !IS_NPC(orig) && !IS_NPC(victim))
                {
                    gold = victim->gold * number_range( 10, 20 ) / 100;
                    orig->gold += gold;
                    victim->gold -= gold;
                    sprintf( buf, "�A��o�F "HIY"%d"NOR" �T����.\n\r", gold );
                    send_to_char( buf, orig );
                    sprintf( buf, "�A���h�F "HIY"%d"NOR" �T����.\n\r", gold );
                    send_to_char( buf, victim );
                }
            }
        }

        //raw_kill( ch, victim );

        //���@��victim�����ɭn������, �򥻤W�o��n�� return������
        //�N�O victim �O npc, �B�� death prog ��
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

        //�B�z �۰ʾ߿� �۰ʤ��� �۰ʾ߾ԧQ�~ �۰ʩ^�m����
        //pc �ΤϦV ko npc ��
		// �N�۰ʾ߫ͥt�~�P�w, �קK�������, ����S�� �����p�X�{ �o�̨S�� corpse �� �r�� 2022/05/08
        if ( orig != victim && !IS_NPC( orig ) && IS_NPC( victim ) )
        {
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( orig , victim , cbuf );
        }
        //pc ���` ko npc ��
        else if ( orig == victim && !IS_NPC(ch) && IS_NPC(victim))
        {
            char cbuf[MAX_INPUT_LENGTH];
            sprintf(cbuf, "'corpse %s'", victim->name);
			AutoCorpse( ch , victim , cbuf );
        }

        /*
         * Remove victims of pk who no longer have exps left
         */
        //���a������, ���pexp���S�F, �ɭP�Q�t�� deny��, �j��a quit
        if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_DENY ) )
        {
            do_quit( victim, "" );
        }

        return dam;
    }

    //��o�䪺�ܪ�� ��������(victim) �٬��� XD

    //��o�� victim = ch ����ر��p�F
    //1. �ۤv���ۤv
    //2. ch �Q orig �ϦV
    //(�]�N�O���Q�ϦV���쪺��, �S�� �_�u�۰�recall �� wimpy�۰ʰk�] ���P�w�N�O�F..)
    if ( victim == ch )
    {
        return dam;
    }
    
    /* remove this setting -- by keelar
    //�_�u���� pc, �D��������, ���@�w���v�۰� recall
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
    //�k�]�ˬd
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

	// adore �C�U�ˮ`���\�^�� 2022/11/06
	if( is_affected( ch, gsn_regeneracy ) && affected_value(ch, gsn_regeneracy, APPLY_HIT) > 0 )
		ch->hit = UMIN( ch->max_hit , ch->hit + affected_value(ch, gsn_regeneracy, APPLY_HIT) );

    if( dt != skill_lookup("magic missile"))
        dam += ch->amdr;
    
    if(!IS_NPC(ch)){
        if(get_skill_percent(ch, dt) < 10) dam = dam * 90 / 100;
    }

	//spell fury ���@�Ω� MM �W 18/7/15
	if( dt != skill_lookup("magic missile"))
		dam = check_spellfury(ch,victim,dam);

	dam += GetEnchantmentMagicDamage(ch, victim, dt);  // 2021/10/04 ���]�Z�����]�k�ˮ`�v�T

	//MM ����Mdr �v�T 18/7/15
	if( dt != skill_lookup("magic missile") )
		dam -= victim->mdr;
	
    if(dam < 0) dam = 0;

    if( dam > 0 )
    {
		//�ھڧ����̪�Level��ˮ`�i��խ��A�H���������Ū��԰�
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
            act( "$n �����v�����B�{.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_SET(ch->special, ALY_HIDE) )
        {
            REMOVE_BIT( ch->affected_by, AFF_HIDE );
            act( "$n �����v�����B�{.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_DARK_RAID ) )
        {
            affect_strip( ch, gsn_dark_raid );
            send_to_char( "�A���M�`�O�Q�����F�I\n\r", ch );
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

        //�p�ⰽŧ���[���P�����۳ꪫ����G
        dam = check_dirty_raid(ch,victim,dam);
		// return �|�y���ˮ`�L�k���
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
			// alc �� sor �]�k�ˮ`�[�j 2022/04/10
			else if(!IS_NPC(ch) && (ch->class == ALCHEMIST || ch->class == SORCERER) )
				dam = dam * 3 / 4;
            else
                dam /= 2;
           dam = dam * 10 / 7; //modified by tale due to the abnormal of magic damage after server os version changing.
        }

        dam = GetDamageUnderProtection(ch, victim, dam);
        dam = GetMagicalDamageUnderRiding(victim, dam, dt);
        // �ر� anti magic ���]�k�ˮ`��K
        dam = GetMagicalUnderAntiMagic(ch, victim, dam);
		// �޵P��� 2020/10/30
		dam = GetMagicalWithShield(ch, victim, dam);
    }

    if ( dam < 0 ) dam = 0;

	// mystic armor ���m�]�k�ˮ`�P�w�U 2020/10/26
	if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
		&& dt != gsn_fire_breath
		&& dt != gsn_acid_breath
		&& dt != gsn_frost_breath
		&& dt != gsn_gas_breath
		&& dt != gsn_lightning_breath )
	{
		dam = MysticArmorCount( victim , dam );
		// �]��o dam <= 0 , �N�� mystic armor ��k�N�������� 2023/01/09
		if( dam <= 0 ) return;
	}

	// lifeshare �ĪG 2021/11/20
	if( is_affected(victim, gsn_lifeshare) && dt != gsn_lifeshare ){
		if( victim->linking != NULL )
			dam = LifeShareDamage( victim , ch , dam );
	}

	// blood thirsty �����ˮ`�p�� 2022/02/20
	if( !IS_NPC(ch) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(ch, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( ch , dam );
		}
	}
	// blood thirsty �Q�ˮ`�p�� 2022/02/20
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
		// �k�N�����|Ĳ�o�S�w�ݩʤ����ݩʪ� dt hurt prog 2022/12/25
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
	
	// mar �b blood thirsty buff �ĪG�����|�Q���� add at 2020/10/16
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
				sprintf( log_buf, "%s(%s)�Q%s(%s)����%d",victim->short_descr, victim->name,
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
					sprintf( buf, "�A��o�F "HIY"%d"NOR" �T����.\n\r", gold );
					send_to_char( buf, orig );
					sprintf( buf, "�A���h�F "HIY"%d"NOR" �T����.\n\r", gold );
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

		// �N�۰ʾ߫ͥt�~�P�w, �קK�������, ����S�� �����p�X�{ �o�̨S�� corpse �� �r�� 2022/05/08
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
	//spell fury ���@�Ω�H�U spell 18/7/23
	if(    dt != gsn_vampiric_bite
		&& dt != gsn_fire_breath
		&& dt != gsn_acid_breath
		&& dt != gsn_frost_breath
		&& dt != gsn_gas_breath
		&& dt != gsn_lightning_breath )
		dam = check_spellfury(ch,victim,dam);

	dam += GetEnchantmentMagicDamage(ch, victim, dt);  // 2021/10/04 ���]�Z�����]�k�ˮ`�v�T

    if( dam > 0 ){

        dam -= victim->mdr;
        if(dam < 0) dam = 0;
        //�ھڧ����̪�Level��ˮ`�i��խ��A�H���������Ū��԰�
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
            act( "$n �����v�����B�{.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_SET(ch->special, ALY_HIDE) )
        {
            REMOVE_BIT( ch->affected_by, AFF_HIDE );
            act( "$n �����v�����B�{.", ch, NULL, NULL, TO_ROOM );
        }

        if ( IS_AFFECTED( ch, AFF_DARK_RAID ) )
        {
            affect_strip( ch, gsn_dark_raid );
            send_to_char( "�A���M�`�O�Q�����F�I\n\r", ch );
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

        //�p�ⰽŧ���[���P�����۳ꪫ����G
        dam = check_dirty_raid(ch,victim,dam);
        if (dam < 0) dam = 0;//return -1;

        /*
         * Damage modifiers.
         */

		/*  �����t���ˮ`��b���ĪG, �����ϩҦ����z���˥b��, �۷��Ҧ��H��o�ѥ͸t�� 2022/10/26

        if (    IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities,RACE_SANCT ) )
        {
            dam /= 2;
        } */
		dam /= 2;

        if ( IS_NPC(victim) )
        {
            if(!IS_NPC(ch) && ch->class == WIZARD)
                dam = dam * 4 / 5;
			// alc �� sor �]�k�ˮ`�[�j 2022/04/10
			else if(!IS_NPC(ch) && (ch->class == ALCHEMIST || ch->class == SORCERER) )
				dam = dam * 3 / 4;
            else
                dam /= 2;
	
           dam = dam * 10 / 7; //modified by tale due to the abnormal of magic damage after server os version changing.
        }

        //���F���� �ˮ`���1/3
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

        //Protection���@��
        dam = GetDamageUnderProtection(ch, victim, dam);
        dam = GetMagicalDamageUnderRiding(victim, dam, dt);
        // �ر� anti magic ���]�k�ˮ`��K
        dam = GetMagicalUnderAntiMagic(ch, victim, dam);
		// �޵P��� 2020/10/30
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
                  send_to_char("�A���\\�{���F�k�N�I�n�I�ڡI\n\r",victim );
            act( "$n ���\\�{���F�k�N�I�u�O�ӰڡI", victim, NULL, NULL, TO_ROOM );
            }
        return -2;
    }

	// �s���ĪG�[�� 2022/04/06
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

		// wpn �N�J�H�˳� hp �ɪ� sklv
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

		// �r���P�w�P��L�������P
		if( dt == gsn_gas_breath )
		{
			// �P���ܩʮt�v�T�̤j�W��
			if( nSvSDiff >= 0 )
				nSvSDiff = 40 + (1 + nBreathSklv) * nSvSDiff;
			else
				nSvSDiff = 30 + (10 * nSvSDiff)/(10 + nBreathSklv);
			// �W���]�w
			if( affected_value(victim, dt , APPLY_SAVING_POISON ) < nSvSDiff * (1 + nBreathDecay )/( 1 + get_curr_con(victim) ) )
			{
				// ���|���r �� �w�� debuff �ĪG, �N����ܱԭz
				if(    !affected_value(victim, dt , APPLY_SAVING_POISON) > 0
					&& !IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON )
					&& !IS_SET( victim->special, NO_POISON ) )
				{
					act( "$N"HIG"�Q�@�r�R��Ţ�n�A�L���y�Ⲥ���x�C�C"NOR"", ch, NULL, victim, TO_CHAR );
					act( HIG "�@�r�R��Ţ�n�A���|�P�A�@�}���߷P���A�۷������C"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$N"HIG"�Q�@�r�R��Ţ�n����Ÿ�ݤF�@�U�A���ܱo���ǭW�ߡC"NOR"", ch, NULL, victim, TO_NOTVICT );
				}
				af.type = dt;
				if( affected_value(victim, dt , APPLY_SAVING_POISON) > 0 )
					af.duration = 0;
				else
					af.duration = 12;
				af.location = APPLY_SAVING_POISON;
				// no poison �h���W poison �ĪG , ���̵M�|����j��
				if( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) || IS_SET( victim->special, NO_POISON ) )
					af.bitvector = 0;
				else
					af.bitvector = AFF_POISON;
				if( nVchSvB <= 0 )
					af.modifier = UMAX( nBreathSklv*nBreathSklv , ( dam * (100 + nBreathDecay*2 + (nVchSvB*3)/2 ) ) / ( (100 + nBreathDecay*2) * 60 ) );
				else
					af.modifier = dam * (100 + nBreathSklv*nBreathSklv + nVchSvB ) / (100 * 15);
				affect_join( victim, &af );
				// �W�L�W���N�]���W��
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
			// �P���ܩʮt�v�T�̤j�W��
			if( nSvSDiff >= 0 )
				nSvSDiff = (1 + nBreathSklv) * nSvSDiff;
			else
				nSvSDiff = (100 * nSvSDiff) / (10 + nBreathSklv);
			// �W���]�w
			if( affected_value(victim, dt , APPLY_DAMAGE_COUNT ) < 76 + 9 * nBreathDecay + nSvSDiff )
			{
				// �w�� debuff �ĪG�N����ܱԭz
				if( !affected_value(victim, dt , APPLY_DAMAGE_COUNT) > 0 )
				{
					if( dt == gsn_acid_breath ){
						act( "$N"HIG"�Q�j�ĦR���Q���A�ĲG���ۨ�L���W�C"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIG "�j�ĦR���§A�Q�ӡA�A���W�g����G�k���ĲG�I"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIG"�Q�j�ĦR���Q���A�ĲG���ۨ�L���W�C"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
					if( dt == gsn_fire_breath ){
						act( "$N"HIR"�Q�`�����R���Q���A����_�X�}�}���ϡC"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIR "�`�����R���§A�Q�ӡA�A����_�X�}�}���ϡI"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIR"�Q�`�����R���Q���A����_�X�}�}���ϡC"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
					if( dt == gsn_frost_breath ){
						act( "$N"HIC"�Q�B�N���R���Q���A�ֽ��ᵲ�X�h�h�B���C"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIC "�B�N���R���§A�Q�ӡA�A���ֽ����X�h�h�B���I"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIC"�Q�B�N���R���Q���A�ֽ��ᵲ�X�h�h�B���C"NOR"", ch, NULL, victim, TO_NOTVICT );
					}
					if( dt == gsn_lightning_breath ){
						act( "$N"HIY"�Q�p�q�R���Q���A�����y��۰����q�y�C"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIY "�p�q�R���§A�Q�ӡA�A�����y��۰����q�y�I"NOR"" , ch, NULL, victim, TO_VICT);
						act( "$N"HIY"�Q�p�q�R���Q���A�����y��۰����q�y�C"NOR"", ch, NULL, victim, TO_NOTVICT );
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
				// �W�L�W���N�]���W��
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
	
	// �s�� wpn �N�J�˳� hp Ĳ�o�� sklv , ���ˮ`�� 0 �h�� wpn �ഫ�� WEAR_NONE 2022/04/16
	if( wpn != WEAR_NONE )
		wpn = WEAR_NONE;

	if( IsPK(ch, victim) )
	{
		dam /= PK_DMG_DENOMINATOR;
	}
	
	// mystic armor ���m�]�k�ˮ`�P�w�U 2020/10/26
	if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
		&& dt != gsn_fire_breath
		&& dt != gsn_acid_breath
		&& dt != gsn_frost_breath
		&& dt != gsn_gas_breath
		&& dt != gsn_lightning_breath )
	{
		dam = MysticArmorCount( victim , dam );
		// �]��o dam <= 0 , �N�� mystic armor ��k�N�������� 2023/01/09
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

    //////////////// �k�۫B���̫�ˮ`�����v�ĪG ///////////////////////
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
                act( "$N"HIY"����j�j���k�ۧ����A����y�ɰʼu���o�I"NOR, ch, NULL, victim, TO_CHAR  );
                send_to_char( "�A����j�j���k�ۧ����A����y�ɰʼu���o�I\n\r", victim);
                act( "$N"HIY"����j�j���k�ۧ����A����y�ɰʼu���o�I"NOR, ch, NULL, victim, TO_NOTVICT );
            }
        }
    }

	// lifeshare �ĪG 2021/11/20
	if( is_affected(victim, gsn_lifeshare) && dt != gsn_lifeshare ){
		if( victim->linking != NULL )
			dam = LifeShareDamage( victim , ch , dam );
	}

	// blood thirsty �����ˮ`�p�� 2022/02/20
	if( !IS_NPC(ch) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(ch, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( ch , dam );
		}
	}
	// blood thirsty �Q�ˮ`�p�� 2022/02/20
	if( !IS_NPC(victim) && ch != victim && dt != gsn_lifeshare && dam > 0)
	{
		if( get_skill_percent(victim, gsn_blood_thirsty) > 0 )
		{
			BloodThirstyDamCount( victim , dam*2 );
		}
	}

	// adore �C�U�ˮ`���\�^�� 2022/11/06
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
		// �k�N�����|Ĳ�o�S�w�ݩʤ����ݩʪ� dt hurt prog 2022/12/25
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
	
	// mar �b blood thirsty buff �ĪG�����|�Q���� add at 2020/10/16
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
                sprintf( log_buf, "%s(%s)�Q%s(%s)����%d",victim->short_descr,
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
                    sprintf( buf, "�A��o�F "HIY"%d"NOR" �T����.\n\r", gold );
                    send_to_char( buf, orig );
                    sprintf( buf, "�A���h�F "HIY"%d"NOR" �T����.\n\r", gold );
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

		// �N�۰ʾ߫ͥt�~�P�w, �קK�������, ����S�� �����p�X�{ �o�̨S�� corpse �� �r�� 2022/05/08
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
