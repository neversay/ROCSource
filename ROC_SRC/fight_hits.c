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
//*  PURPOSE     :         �ק�met���ؼЦ��Qpro����slash��bug     *
//*              :         ��ber���ؼЦ��Qpro�ɷ|�Q������bug      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941012 Razgriz                                *
//*  PURPOSE     :         mone_hit() �[�J�������                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941014 Razgriz                                *
//*  PURPOSE     :         ��g mone_hit() ����                   *
//*              :         ���� brief_mone_hit                    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941015 Razgriz                                *
//*  PURPOSE     :         �[�J�I�몺 hr �[��,�H�Ω]ŧ��hr dr�[�� *
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

	// �W�[ -1 ���p���P�w 2023/01/05
	if( ammo->value[0] <= 0 && ammo->value[0] != -1 )
		return FALSE;

	return TRUE;
}

// �קאּ�a�J weapon �P�w 2022/12/03
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

// �קאּ�a�J weapon �P�w 2022/12/03
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

	// �N IsUsingKnightWeapon �� weapon �����X�h 2022/12/03
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

	// �N IsUsingTrickyWeapon �� weapon �����X�h 2022/12/03
	/*if(!IsUsingShortWeapon(ch)) {
		if (nTrickySklv < 4) return nPerc;
		if (!IsUsingTrickyWeapon(ch)) {
			if (!IsUsingBowShot(ch)) return nPerc;
		}
	} */

	return nPerc * (100 + nTrickySklv * (nTrickySklv + 4)) * (1 + nTrickyPerc) / 10000;
}

// �w�� artifact �����p, ���Q sleep ��charm ���ؼз|�T�갱�����, �קK�y��crash 2022/05/03
bool HitSleepCharm(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if( !victim || ( victim == NULL ) || victim->deleted ) return TRUE;
	if( ch->deleted ) return TRUE;
	if( IS_AFFECTED( victim, AFF_SLEEP ) )
		return TRUE;
	if( IS_AFFECTED( ch, AFF_SLEEP ) ) // ch ���� multi_hit ���i��Q�����y�� sleep
		return TRUE;

	// charm �������ק�, �[�J�����̬O�_�� master 2022/12/31
	if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch )
		return TRUE;
	if( IS_AFFECTED( ch, AFF_CHARM ) && ch->fighting == victim && ch->master == victim ) // ch ���� multi_hit ���i��Q�����y�� charm
		return TRUE;

	return FALSE;
}

// �w�� ����Z�� �b�o�g�᪺dr hr apdr �ݩʥ[�� 2022/05/06
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

// �w�� ����Z�� �b�o�g��ƶq�p�� 2023/01/04
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

	// �i��Q purge ��
	if( !obj )
		return;

	// �� 0 �p�N�g���汼
	if( obj->value[0] <= 0 )
	{
		if( !obj )
			return;

		// no remove �N���|�۰ʲ汼
		if( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) )
			return;


		// -1 �����p���L��, �b mone_hit �����@�P�w, �z�פW�o��]����
		if( obj->value[0] == -1 )
			return;

		// �b�ڪ�����
		if( obj->item_type == ITEM_ARROW )
		{
			// magic arrow �g����ۧڮ��� 2022/04/29
			if( obj->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW ){
				//send_to_char("�A���]�k�b�g���F�C\n\r", ch);
				act( "�A�� $p �g���F.", ch, obj, NULL, TO_CHAR ); // �קאּ act ��ܮg�������~ 2023/01/05
				unequip_char(ch, obj);
				obj_from_char(obj);
				extract_obj(obj);
			}else{
				//send_to_char("�A���b�g���F�C\n\r", ch);
				act( "�A�� $p �g���F.", ch, obj, NULL, TO_CHAR );
				unequip_char(ch, obj);
			}
		}
		// �u�Ī�����
		if( obj->item_type == ITEM_AMMO )
		{
			// magic ammo �g����ۧڮ��� 2022/04/29
			if( obj->pIndexData->vnum == OBJ_VNUM_MAGIC_AMMO ){
				//send_to_char("�A���]�k�u�Χ��F�C\n\r", ch);
				// �קאּ act ��ܮg�������~ 2023/01/05
				act( "�A�� $p �g���F.", ch, obj, NULL, TO_CHAR );
				unequip_char(ch, obj);
				obj_from_char(obj);
				extract_obj(obj);
			}else{
				//send_to_char("�A���u�ĥΧ��F�C\n\r", ch);
				act( "�A�� $p �g���F.", ch, obj, NULL, TO_CHAR );
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
			// npc victim �Q�}�� 22022/10/31
			if( IS_NPC(victim) ) vch_prog = 1;
		}
		//victim->position = POS_FIGHTING;
	}

	if( !ch->fighting )
	{
		set_fighting( ch, victim );
		ch->position = POS_FIGHTING;
		// npc ch �}�� 2022/10/31
		if( IS_NPC(ch) ) ch_prog = 1;
	}

	//�s���P�r�r
	if( !IS_NPC( ch ) && ch->fighting )
	{
		if( check_race_special( ch ) )
		{
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// mob �p�G���� qmark �h���@���q���� , ���B dt �ǤJ�� TYPE_UNDEFINED �h�������q���� 2022/10/31
	if( IS_NPC(ch) && is_qmark(ch,"disable_round_hit" ) && dt == TYPE_UNDEFINED )
		return;

	// �P�w protect target
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
					//�קאּ��stop_fighting
					//�]��stop_fighting�|�Nch��cast�h��
					//��met��slash��bug
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
					act( "$n�Ĥ��U���a�O�@$N�I", protector, NULL, victim, TO_ROOM );
					act( "�A�Ĥ��U���a�O�@$N�I", protector, NULL, victim, TO_CHAR );
					victim = protector;
					// victim->attacked = ch;
					break;
				}
				else
				{
					act( "$n�Q�n�O�@$N�A�i�����ѤF�I", protector, NULL, victim, TO_ROOM );
					act( "�A�Q�n�O�@$N�A�i�����ѤF�I", protector, NULL, victim, TO_CHAR );
				}
			}
		}
	}

	++totalhit;

	// �w��y�P�b�|�ܬ��y�P�٪����D�@�ץ� 2022/05/02
	if( dt == gsn_meteoric_arrow )
	{
		// �p�G���b�h�� dt �q gsn_meteoric_arrow �ഫ�� TYPE_UNDEFINED (-1)
		if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
		{
			mydt = TYPE_UNDEFINED;
		}else{
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
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
		// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
		if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
		{
			if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
			if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
		}
		return;
	}

	// �p�G ch �� victim �Q sleep , charm �h�������
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
		/* lance mastery �� tricky pacing ���P�w�קאּ��Z���ӫD ch �[�� 2022/12/03
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

	// �Z���P�w 2022/12/03

	if( get_eq_char( ch, WEAR_WIELD ) ){
		wield = get_eq_char( ch, WEAR_WIELD );
	}
	if( get_eq_char( ch, WEAR_TWO_HAND ) ){
		wield = get_eq_char( ch, WEAR_TWO_HAND );
	}

	// pc �� weapon weight �P�w �D�n�Z�� �� ����Z�� �O�_�ӭ�, �����ԭz 2022/12/06
	if( wield && !IS_NPC(ch) )
	{
		if( IS_SET( ch->act, PLR_NOTIFY ) ) // �����t�ΰT���~�|���
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
						send_to_char(""CYN"�A���D�n�Z���ӭ��F�I"NOR"\n\r",ch);
					break;

				case WEAR_TWO_HAND :
					for( min_str = 0; min_str <= MAX_STR + 6 ; ++ min_str ){
						if ( str_app[min_str].twohand >= weight )
							break;
						}
					str_diff = get_curr_str ( ch ) - min_str;
					if(nPbPerc > 0) str_diff += nPbSklv * nPbSklv / 6;
					if( str_diff < -5 )
						send_to_char(""CYN"�A������Z���ӭ��F�I"NOR"\n\r",ch);
					break;
			}
		}
	}

	bonus_chance = 100;
	if( !IS_NPC(ch) )
	{
		if( wield ){
			// lance mastery �� tricky pacing ���[�� 2022/12/03
			if( nLancePerc > 0 && IsUsingKnightWeapon(ch , wield) )
				bonus_chance = GetBonusedValue_LanceMaster(ch, 100);
			if( nTrickyPerc > 0 && IsUsingTrickyWeapon(ch , wield) )
				bonus_chance = TrickyPacingBase(ch, 100);

			// str �P�Z�����q �v�T�������ƪ����v 2022/12/11
			if( str_diff < 0 ){
				str_diff = UMAX( -4 , str_diff ) * 15; // �̧C -60%
			}else{
				str_diff = UMIN( 15 , str_diff ) * 2; // �̰� 30%
			}
			bonus_chance += str_diff;
		}
	}

	if( number_percent( ) < (chance1 * bonus_chance)/100 )
	{
		++totalhit;
		// �w��y�P�b�|�ܬ��y�P�٪����D�@�ץ� 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// �p�G���b�h�� dt �q gsn_meteoric_arrow �ഫ�� TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
				&& arrow->item_type == ITEM_ARROW
				&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
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
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// �p�G ch �� victim �Q sleep , charm �h�������
	if( HitSleepCharm(ch,victim) ) return;

	// �P�w �D�n�Z�� �� ����Z�� �O�_���j��g�� 2022/05/05
	if( get_eq_char( ch, WEAR_WIELD ) ){
		if( IsUsingGunShot( ch , get_eq_char( ch, WEAR_WIELD ) ) )
			gun_chance = 40;
	}else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
		if( IsUsingGunShot( ch , get_eq_char( ch, WEAR_TWO_HAND ) ) )
			gun_chance = 40;
	}

	// �j��g�� ���C�������ƥX�{�v 2022/05/05
	if( gun_chance + number_percent( ) < (chance2 * bonus_chance)/100 )
	{
		++totalhit;
		// �w��y�P�b�|�ܬ��y�P�٪����D�@�ץ� 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// �p�G���b�h�� dt �q gsn_meteoric_arrow �ഫ�� TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
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
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// �p�G ch �� victim �Q sleep , charm �h�������
	if( HitSleepCharm(ch,victim) ) return;

	// �j��g�� ���C�������ƥX�{�v 2022/05/05
	if( gun_chance + number_percent( ) < (chance3 * bonus_chance)/100 )
	{
		++totalhit;
		// �w��y�P�b�|�ܬ��y�P�٪����D�@�ץ� 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// �p�G���b�h�� dt �q gsn_meteoric_arrow �ഫ�� TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
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
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// �p�G ch �� victim �Q sleep , charm �h�������
	if( HitSleepCharm(ch,victim) ) return;

	// �j��g�� ���C�������ƥX�{�v 2022/05/05
	if( gun_chance + number_percent( ) < (chance4 * bonus_chance)/100 )
	{
		++totalhit;
		// �w��y�P�b�|�ܬ��y�P�٪����D�@�ץ� 2022/05/02
		if( dt == gsn_meteoric_arrow )
		{
			// �p�G���b�h�� dt �q gsn_meteoric_arrow �ഫ�� TYPE_UNDEFINED (-1)
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) )
			&& arrow->item_type == ITEM_ARROW
			&& arrow->value[0] > 0 )
			{
				mydt = TYPE_UNDEFINED;
			}else{
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
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
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}
	}

	// �p�G ch �� victim �Q sleep , charm �h�������
	if( HitSleepCharm(ch,victim) ) return;

	wield = NULL;

	// dual ���� ���n�Z�������p
	if( ( wield = get_eq_char( ch, WEAR_WIELD_2 ) ) 
		&& wield->item_type == ITEM_WEAPON
		&& ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_DUAL ) ) 
		||( !IS_NPC( ch ) && get_skill_percent(ch, gsn_dual))))
	{
		// �ק� dual �ޯ�ʤ���P���żv�T, �ðw�藍�P chance �@���PĲ�o�v 2022/05/03
		int nDualPerc = get_skill_percent(ch, gsn_dual) + get_skill_level(ch, gsn_dual) * 7;

		dt = TYPE_HIT + wield->value[3];

		mydt2 = dt;

		++totalhit;

		// dual ���Ĥ@�U�@�w�|�X 2022/05/03
		//dam = brief_mone_hit( ch, victim, &mydt2, WEAR_WIELD_2 );
		dam = mone_hit ( ch, victim, &mydt2, WEAR_WIELD_2, MSG_BRIEF, DT_CHANGE );

		// pc �� weapon weight �P�w ���n�Z�� �O�_�ӭ�, �����ԭz 2022/12/06
		if( wield && !IS_NPC(ch) )
		{
			if( IS_SET( ch->act, PLR_NOTIFY ) ) // �����t�ΰT���~�|���
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
						send_to_char(""CYN"�A�����n�Z���ӭ��F�I"NOR"\n\r",ch);
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
			// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
			if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
			{
				if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
			}
			return;
		}

		// �p�G ch �� victim �Q sleep , charm �h�������
		if( HitSleepCharm(ch,victim) ) return;

		bonus_chance = 100;
		if( !IS_NPC(ch) )
		{
			if( wield ){
				// lance mastery �� tricky pacing ���[�� 2022/12/03
				if( nLancePerc > 0 && IsUsingKnightWeapon(ch , wield) )
					bonus_chance = GetBonusedValue_LanceMaster(ch, 100);
				if( nTrickyPerc > 0 && IsUsingTrickyWeapon(ch , wield) )
					bonus_chance = TrickyPacingBase(ch, 100);

				// str �P�Z�����q �v�T�������ƪ����v 2022/12/11
				if( str_diff < 0 ){
					str_diff = UMAX( -4 , str_diff ) * 15; // �̧C -60%
				}else{
					str_diff = UMIN( 15 , str_diff ) * 2; // �̰� 30%
				}
				bonus_chance += str_diff;
			}
		}

		// dual �� chance1 ���v�� nDualPerc
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
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}

		// �p�G ch �� victim �Q sleep , charm �h�������
		if( HitSleepCharm(ch,victim) ) return;

		// �P�w ���n�Z�� �O�_���j��g�� 2022/05/05
		gun_chance = 0;
		if( IsUsingGunShot( ch , wield ) )
			gun_chance = 40;

		// �j��g�� ���C�������ƥX�{�v 2022/05/05
		// dual �� chance2 ���v�� nDualPerc - 10
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
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}

		// �p�G ch �� victim �Q sleep , charm �h�������
		if( HitSleepCharm(ch,victim) ) return;

		// �j��g�� ���C�������ƥX�{�v 2022/05/05
		// dual �� chance3 ���v�� nDualPerc - 20
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
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}

		// �p�G ch �� victim �Q sleep , charm �h�������
		if( HitSleepCharm(ch,victim) ) return;

		// �j��g�� ���C�������ƥX�{�v 2022/05/05
		// dual �� chance4 ���v�� nDualPerc - 30
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
				// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
				if( ch->fighting && victim->fighting && !ch->deleted && !victim->deleted )
				{
					if( ch_prog == 1 ) mprog_fight_trigger( ch , victim);
					if( vch_prog == 1 ) mprog_fight_trigger( victim , ch);
				}
				return;
			}
		}
	}// dual �P�w����

	// �p�G ch �� victim �Q sleep , charm �h�������
	if( HitSleepCharm(ch,victim) ) return;

	// �P�w tactic �� trip �� disarm
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

	// �P�w lizardman �S�� , �p�G dt ���y�P�b�h��Ĳ�o lizardman �S�� 2022/05/02
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) && dt != gsn_meteoric_arrow )
	{
		int nSklv = get_skill_level(ch, gsn_inborn_ability);
		if( !victim->deleted ){
			//  �Q�ħ���
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

				// �R���P�w�קאּ weakness_percent 2020/09/18
				//nPoisonRate = nPoisonRate *  weakness_damage(victim, gsn_poison) / 100;
				//nBlindRate  = nBlindRate  *  weakness_damage(victim, gsn_blindness) / 100;
				nPoisonRate = nPoisonRate *  weakness_percent(victim, gsn_poison) / 100;
				nBlindRate  = nBlindRate  *  weakness_percent(victim, gsn_blindness) / 100;

				if(number_percent() < nPoisonRate && !IS_SET(victim->special, NO_POISON)){
					// �ѽᵥ�ť|�H�W�B�����|���Ĥ褤�r
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
					act( "$N"HIG"�y�Ⲥ���x�C�A�r���w�g�I�J�F�L���餺�I"NOR"", ch, NULL, victim, TO_CHAR );
					act( HIG "���߷P�P��h�P�P��ŧ�ӡA�Aı�o�۷�a�����I"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$N"HIG"������Ÿ�ݤF�@�U�A���ܱo���ǭW�ߡI"NOR"", ch, NULL, victim, TO_NOTVICT );
				}

				if(number_percent() < nBlindRate && !IS_AFFECTED( victim, AFF_BLIND ) 
				   && !IS_SET( victim->special, NO_BLIND )
				   && !(is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 2) ){
					// �ѽᵥ�Ť��H�W�B�����|���ĤH����
					af.type      = gsn_blindness;
					af.duration  = 0;
					af.location  = APPLY_HITROLL;
					af.modifier  = (-10) * weakness_damage( victim, gsn_blindness ) * 0.01;
					af.bitvector = AFF_BLIND;
					affect_to_char( victim, &af );

					act( "�A"HIG"���ĲG�˨�F$N"HIG"�������I�ϱo�L�����F�I"NOR"", ch, NULL, victim, TO_CHAR );
					act( "$n"HIG"���ĲG�˨�F�A"HIG"�������I�A���򳣬ݤ���F�I"NOR"" , ch, NULL, victim, TO_VICT);
					act( "$n"HIG"���ĲG�˨�F$N"HIG"�������I�ϱo�L�����F�I"NOR"", ch, NULL, victim, TO_NOTVICT );
				}
				damage( ch, victim, nDamage, gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			}
		}
        
		if( !victim->deleted )
		{   // ��������
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
	}// lizardman �S�ާP�w����

    if( is_brief(ch, victim))
        brief_dam_message(ch, victim, totaldam, mydt, WEAR_WIELD,totalhit, landedhit);
	// npc �}�Ԫ��Ĥ@�^�X�|Ĳ�o fight_prog 22022/10/31
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
		// critical base ���v
		chance = get_skill_percent(ch, gsn_critical)/10 + nSklv * (nSklv + 14) / 4;

		// �קאּ darkraid �u�P�w�@�� , ���P�w�O�_�� pc �� �����Ҧ� 2020/09/16
        //if ( dt == gsn_backstab && !IS_NPC(ch) && darkraid )
		if ( darkraid )
			chance += 4 + get_skill_percent(ch, gsn_dark_raid) / 9 + get_skill_level(ch, gsn_dark_raid) * get_skill_level(ch, gsn_dark_raid);

		// autoc critical �P�w  2022/07/14
		if( fCritical && ( number_percent() < ( nAcSklv * 13 + get_skill_percent( ch, gsn_deadly_strike)/ 3 ) ) )
			// deadly strike ���żv�T critical �X�{�v������ 2022/07/14
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
                act( "�A�B�Χޥ��յۦV $N ���z�I����!!!", ch, NULL, victim, TO_CHAR );
                act( "$n ��ǤF�A���z�I�P�R�@��!!!",ch, NULL, victim, TO_VICT );
                act( "$n �յۦV $N ���z�I�i��P�R�@��!!!",ch, NULL, victim, TO_NOTVICT );
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

	// �[�J�Z���κA�P�w 2023/01/05
	if( !wield || wield == NULL )
		return;

	if( wield->value[3] != 16 )
		return;

	if( ch->move < 350 - get_skill_level(ch, gsn_firearm_mastery)*20 )
		return;

	ammo = get_eq_char( ch, WEAR_AMMO );

	// ���˳� ammo , �S���˳� ammo �h�����䨭�W���L�u��
	if( ammo ){
		// ammo ��m�O�u�ĥB�ƶq�j�� 0 �εL���u��(-1) �h���ʧ@ 2023/01/05
		if( ammo->item_type == ITEM_AMMO && ( ammo->value[0] > 0 || ammo->value[0] == -1 ) )
			return;
		// �p�G �u�� ���˳ƬO no remove �����p �h���ʧ@
		if( IS_SET( ammo->extra_flags, ITEM_NOREMOVE ) )
			return;
	}

	HasReload = 0;
	
	// �M�䨭�W�i�Ϊ��u��
	for( reload = ch->carrying; reload; reload = reload_next )
	{
		reload_next = reload->next_content;
		if( reload->wear_loc == WEAR_NONE                          // reload �����b���W
			&& reload->item_type == ITEM_AMMO                      // reload �������u��
			&& CAN_WEAR(reload, ITEM_WEAR_AMMO)                    // reload �n��Q��b ammo ����m
			&& ( reload->value[0] > 0 || reload->value[0] == -1 )  // �u�ļƥ����j�� 0, �άO -1 ( �L���h )
			&& can_see_obj( ch, reload )                           // �n�ݱo�� reload
			&& ch->level >= reload->level )                        // ch ���ŭn�b obj �H�W
		{
			HasReload++;
			break;
		}
	}


	// �N��S���i�H reload �����p�U, �]��o�p�G���˳� ammo �h v0 ���w�p�� 0 ������ -1 , �h�P�w�ƶq�O�_�汼�u�� 2023/01/04
	if( ammo )
		ShotObjCount( ch , ammo );

	// �S�����i�Ϊ��u��
	if( HasReload == 0 )
		return;

	/*
		// ���˳Ʀb�u�īh�汼�Ӹ˳� ( �Ӧ�m�˳�type ���� ammo, �� ammo v0 <= 0 )
		if( ammo )
			unequip_char( ch, ammo );
	*/

	// �˳Ƨ�쪺�u��
	if( reload ){
		equip_char( ch, reload, WEAR_AMMO );
		ch->move = ch->move - (350 - get_skill_level(ch, gsn_firearm_mastery)*20);
		act( "�A�ֳt�˳ư_ $p.", ch, reload, NULL, TO_CHAR );
		act( "$n �ֳt�˳ư_ $p.", ch, reload, NULL, TO_ROOM );
	}

	return;
}


/*
 * �[�J������� by Razgriz 20051012
 * ������g by Razgriz 20051014
 */
int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode )
{
    OBJ_DATA  *wield = NULL;
    OBJ_DATA  *bow = NULL;
    OBJ_DATA  *arrow = NULL;
	OBJ_DATA  *gun = NULL;   //2022/04/30 �j��
    OBJ_DATA  *ammo = NULL;   //2022/04/30 �u��
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
    bool       isPowerBlow = FALSE; // �Τ@ powerblow ���P�w 2021/12/25
	bool       isBloodThirsty = FALSE; // �D�� blood thirsty �P�w 2021/12/25
	bool       isShootCost = FALSE; // 2022/12/07
	int        nLmPerc = get_skill_percent(ch, gsn_lance_mastery);
	int        nLmSklv = get_skill_level(ch, gsn_lance_mastery);
	int        nTpPerc = get_skill_percent(ch, gsn_tricky_pacing);
	int        nTpSklv = get_skill_level(ch, gsn_tricky_pacing);

    if(dt == gsn_tactics) isLizTail = TRUE;

	// �D�� blood thirsty �P�w 2021/12/25
	if( !IS_NPC(ch)
		&& ch->class == MARTIALMAN
		&& get_skill_percent(ch, gsn_blood_thirsty) > 0
		&& is_affected( ch, gsn_blood_thirsty)
		&& affected_value(ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 )
	{
		isBloodThirsty = TRUE;
	}

	// �[�J hit <= 0 �ɤ]�| return ���P�w 2022/12/30
    if(!check_can_do_hit(ch,victim) || victim->position == POS_DEAD || victim->hit <= 0 ){
        if(dt != gsn_reposte || get_skill_level(ch, gsn_reposte) < 5)
            return -2;
    }

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD
		|| victim->hit <= 0  // �[�J hit <= 0 �ɤ]�| return ���P�w 2022/12/30
		|| !ch->in_room
		|| ch->in_room != victim->in_room ){
        sprintf( buf, "one_hit: ch %s not with victim %s, or victim POS_DEAD",ch->name, victim->name );
        bug( buf, 0 );
        return -2;
    }

    // �B�z autoc critical , move ���Ө��ޯ൥�żv�T 2022/07/14
    if( !IS_NPC(ch)
		&& IS_SET( ch->pcdata->autocombat, AUTOC_CRITICAL)
		&& (ch->move > 80 - get_skill_level(ch, gsn_deadly_strike) * 5 ) )
	{
		/* 	autoc critical �P�w���m get_critical_damage ��, �ò����������X�{���]�w 2022/07/14

        if ( number_percent() > (   get_skill_level(ch, gsn_deadly_strike) * 2
                                  + get_skill_percent( ch, gsn_deadly_strike) / 3 ) ){
            return -2;
        }
        chance += get_skill_percent( ch, gsn_deadly_strike ) / 3 + get_skill_level(ch, gsn_deadly_strike); */

		// fCritical ������, �N��i���L critical skill percent ����P�w 2022/07/14
        fCritical = TRUE;

        ch->move = UMAX( 0 , ch->move - 80 + get_skill_level(ch, gsn_deadly_strike) * 5 );
    }

	//�B�z ��Ǯg�� trueshot
	if ( !IS_NPC( ch ) && is_affected(ch,gsn_trueshot)){
		if ( ch->move > 10 )
			ch->move -= 10;
		else{
			send_to_char("�A����O����������Ǯg�������A.\n\r",ch);
			affect_strip(ch,gsn_trueshot);
		}
	}

    /*
     * Figure out the type of damage message.
     */

    //�ھڶǶi�Ӫ� wpn �˳Ʀ�m(�X�G���O WEAR_WIELD, �٨S�ݹL���O�� orz), ��X ch �� weapon
	// wpn �򥻤W�H wear_wield �� wear_wield_2 �i�J, ���|�H wear_two_hand �i�J
    wield = get_eq_char( ch, wpn );

	// wpn �p�G�H wear_two_hand �i�J, �h���B wield �|�� NULL
	if(!wield){
		if ( ( wield = get_eq_char( ch, WEAR_TWO_HAND ) ) ) { // ����Z��
			wpn = WEAR_TWO_HAND;

			// �P�_�O�_���g�b�A���߫h���ӽb�ڼƶq
			if ( wield->value[3] == 14 ){ // �Z���������A�� �g�X�b�� shot
				vch = NULL;
				if ( !IS_NPC(ch) )
				{
					int nRideSklv = get_skill_level(ch, gsn_ride);
					//int nTrickySklv = get_skill_level(ch, gsn_tricky_pacing);
					if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
						if(nTpSklv < 1 ){
							if(nRideSklv < 2 || !get_eq_char( ch , WEAR_RIDE )){
								//�ݬ� ch (pc) �O�_�B��Q�����A
								for( vch = ch->in_room->people; vch; vch = vch->next_in_room ){
									if ( !vch->deleted && vch->fighting == ch && !IS_NPC( ch ) )
										break;
								}
							}
						}
					}
				}

				//ch �S�Q��, �B�٦��b, �B�z�b������
				if ( !vch
					&& ( arrow = get_eq_char( ch, WEAR_ARROW ) )
					&& arrow->item_type == ITEM_ARROW
					&& arrow->value[0] > 0 )
				{
					isShootCost = TRUE;
					// ���� arrow �ƶq���� return �e�P�w 2023/01/04
					//arrow->value[0] -= ( IS_NPC( ch ) && !ch->clan && !IS_AFFECTED(ch, AFF_CHARM) && !IS_AFFECTED( ch, AFF_SUMMONED)) ? 0 : 1;
				}
				else
				{
					/* �b�Ƭ� -1 �����p , �b�ڵL���� 2023/01/05
						if( arrow
							&& arrow->value[0] == -1 )
							&& !IS_NPC(ch)
							&& ch->mana > 1
							&& arrow->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW ) */
					if( arrow && arrow->value[0] == -1 )
					{
						// ���ϥ��]�k�b���b�Ƭ� -1 �|�ܬ��P�w mana �C�b�ܤֻݭn 1 mana 2023/01/05
						if( arrow->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW )
						{
							// �b�Ƭ� -1 , �h�@�b���� 1 mana
							if(ch->mana >= 1){
								ch->mana -= 1;
							}else{ // mana �����h�ഫ�� ���R�A�� slash
								arrow = NULL;
								dt = 1003;
								if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
							}
						}
					}
					else
					{ //  ch �Q��, ch �������A�ର ���R�A�� slash
						arrow = NULL;
						dt = 1003;
						if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
					}
				}
			}
		}
	}else{ // �B�z��N�~�� bow �]�����(wield) �����p 2022/12/06
		if ( wield->value[3] == 14 ){
			dt = 1003;
			if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
		}
	}

    //��������A�� �g�X�b�� shot, ���O�S�b�ΨS�Z���Τ��O����Z����, �������A�ର ���R�A�� slash
    if (dt == 1014 && (!arrow || !wield || wield->wear_loc != WEAR_TWO_HAND) ){
        dt = 1003;
        if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
    }

    //���ؤ���, ��������A�����Z�����������A, ���� reposte �[������ isReposte �O�_���� 2022/05/07
	if ( dt == gsn_reposte ){
		dt = TYPE_HIT;
		isReposte = TRUE;
		if ( wield && wield->item_type == ITEM_WEAPON ){
			dt += wield->value[3];
		}
		if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
	}

    //��������A���w�q��(�N�O����), �N�ѪZ�����������A�ӨM�w
    //�i�Ѧ� fight_const.h, TYPE_HIT ���N�q�i�ݦ� �ĤO���� hit
    if ( dt == TYPE_UNDEFINED ){
        dt = TYPE_HIT;
        if ( wield && wield->item_type == ITEM_WEAPON )
            dt += wield->value[3];
        if ( dt_mode == DT_CHANGE )
            *dt_orig = dt;
    }

	// autoc reload �b�C���������|���T�{�u��, �å[�J�Z���P�w 2023/01/05
	if( !IS_NPC(ch) && wield ){
		if( ch->pcdata->autocombat == AUTOC_RELOAD )
			check_reload_ammo( ch , wield );
	}

    //��������A�� �j��g�� ammo �� , ���S�l�u, �������A�ର �U�v�Oݴ pound 2022/04/30
    if( dt == 1016 ){
		/* ����~���@�P�w 2023/01/05
		if( !IS_NPC(ch) ){
			// autoc reload �b�C���������|���T�{�u�� 2022/05/02
			if( ch->pcdata->autocombat == AUTOC_RELOAD )
				check_reload_ammo( ch );
		} */
		ammo = get_eq_char( ch, WEAR_AMMO );
		if( ammo
			&& ammo->item_type == ITEM_AMMO
			&& ( ammo->value[0] > 0 || ammo->value[0] == -1 ) ) // �[�J -1 �����p 2023/01/05
		{
			// -1 �����p�U�u�Ĥ���� ( �L�� ) 2023/01/05
			if ( ammo->value[0] != -1 )
				isShootCost = TRUE;
			// ���� ammo �ƶq���� return �e�P�w 2023/01/04
			//ammo->value[0] -= (IS_NPC( ch ) && !ch->clan && !IS_AFFECTED(ch, AFF_CHARM) && !IS_AFFECTED( ch, AFF_SUMMONED)) ? 0 : 1;
		}else{
			if( ammo )
				ammo = NULL;
			dt = 1007;
			if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
		}
    }

	if( wield && !IS_NPC(ch) ) // �[�J pc ���w 2022/12/04
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

    //���X�v�T�R���v���@�ǥ��Ѧ]��
    //�� interpolate(), class_table , thac0������ ��ӨӬ�, �j���i�ݥX�H�U��h
    //1. ����¾�L�����઺�����n���R���v (thac0_00, �V�p�V�n)
    //2. ���P¾�~, �H�۵��Ŵ���, �y���R���v���ɵ{�פ��P (thac0_42 - thac0_00, �V�p�V�n)

    if ( IS_NPC( ch ) ){
		//  mob����
        if ( IS_SET( ch->class ,1 ) ){
            thac0_00 =  10;
            thac0_42 = -10;
        }else{
            thac0_00 =  20;
            thac0_42 =   0;
        }
    }else{
		//  ���a����
		//  thac0_42 �� dex ���ɪ� �R���v���� �W�[
		//  �p�� thac0, �V�p��ܩR���v�V��
        thac0_00 = class_table[ch->class].thac0_00;
        thac0_42 = class_table[ch->class].thac0_42 - ( get_curr_dex( ch ) - 16 ) * 2;
    }

    //  ���ر���n�j�p���t�Z, �v�T�@�I�I�R���v , �ڤp�L�j�h�R���W�[, �Ϥ��R�����
	//race_hit_bonus = ( race_table[victim->race].size - race_table[ch->race].size );
	race_hit_bonus = get_curr_size( victim ) - get_curr_size( ch ); // �����髬 size ���� 2022/12/23

    /* Weapon-specific hitroll and damroll */

    if( wield )
	{
        int whr = get_hitroll(ch, wpn);

		//  reposte �|�H hitroll �Χޯ൥���ܰʩR���[�� 2022/01/13
		//  reposte �P�w��H �� isReposte �� bool 2022/05/07
		//if(dt == gsn_reposte && ch->hitroll > 0)
		if( isReposte && ch->hitroll > 0 )
			whr += ( ch->hitroll * ( 25 + get_skill_level(ch, gsn_reposte) * 20
										+ get_skill_level(ch, gsn_reposte) * get_skill_level(ch, gsn_reposte) ) ) / 100;

		//  Mar �~��frenzy , sklv �p�� 6 hr �W�L 60 ����, hr ���W�[�N�|�ͽw, �W�L 1 hr �� 0.25 hr
		//  ex: whr 80 ���� 75, whr 100 ���� 90
        if( dt == gsn_frenzy && whr > 60) {
            //if(!IS_NPC(ch) && get_skill_level(ch, gsn_frenzy) < 6){
			if(!IS_NPC(ch) && ch->class != MARTIALMAN && get_skill_level(ch, gsn_frenzy) < 6){
                whr -= interpolate1( 60, 0, 100, 10, (whr- 60));
            }
        }


		//  tactics �v�T�L�q�R��
        if(!IS_NPC(ch) && get_skill_percent(ch, gsn_tactics)){
            int nTacPerc = get_skill_percent(ch, gsn_tactics);
            int nTacSklv = get_skill_level(ch, gsn_tactics);
            whr = whr * (20000 + nTacPerc * (8 + nTacSklv * (nTacSklv + 1)) ) / 20000;
        }

		// �[�J wp , pc �� lance mastery ���m�קP�w 2022/12/03
		//if( !IS_NPC(ch) && get_skill_percent(ch, gsn_lance_mastery) > 0 && IsUsingKnightWeapon(ch , wield) )
		if( !IS_NPC(ch) && nLmPerc > 0 && IsUsingKnightWeapon(ch , wield) )
			whr = GetBonusedValue_LanceMaster(ch, whr);

		//  �����I�� sklv �P dex ����¦�R���v�ɥ� 2022/11/11
		if( dt == gsn_backstab )
			whr += ( get_skill_level ( ch, gsn_backstab ) * get_curr_dex( ch ) )/21;

		//  �I�� sklv �v�T�R���v�[��, Added by Razgriz 20051015
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
            //�� �]ŧ �R���v�A�[��
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

		// �s���b�N�R���[�� -- �����v�T whr �ä@�֦C�J�Z���p��
		if(dt == 1014 && arrow != NULL && !IS_NPC( ch )){
			int nSklvShot = get_skill_level(ch, gsn_shot_arrow);
			int nCurrDex  = get_curr_dex(ch);
			int nHrBonus  = get_skill_percent(ch, gsn_shot_arrow) + 1;

			// �W�[ ����Z�� hr �ĪG�[�� 2022/05/06
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

        // �j��M�a �� �j��g�����R���[�� 2022/04/30
        if( dt == 1016 && ammo != NULL && !IS_NPC( ch ) ){
			// �j�񪺪Z�����ˤO����ǫ� , ��ϥήg���ɥu�|�v�T��R�� , �ˮ`�u�ݼu�� 2022/05/01
			int nAccuray = number_range( wield->value[1], wield->value[2] );
			int nSklvFA = get_skill_level(ch, gsn_firearm_mastery);
			int nFABonus = 0;

			// �j�񪺺�ǫ׬� avg �� 2 ��
			nAccuray = nAccuray * 2;

			if( get_skill_percent(ch, gsn_firearm_mastery) > 0 ){
				nFABonus = nSklvFA + nSklvFA * nSklvFA;
				nAccuray += 5;
			}
			
			// �W�[ ����Z�� hr �ĪG�[�� 2022/05/06
			whr += ShootObjAffect( ch , ammo , 1 );
			
			nAccuray = nAccuray - 500/(10 + nFABonus);

			whr = whr * (100 + nAccuray) / 100;
			
			whr = whr + nFABonus;
        }

			//thac0 ���p�⤽��, �o��N�O�R���v�����j���F, ���᳣�H thac0 �ӷ��R���v�F
			//�j�P����
			//thac0 =   ���Ѧ]��
			//        - �ر���n�v�T
			//        - �Z�������t�ݩʼv�T
			//        - (�Z���R��+�Z������v�T) * (�Z���رکR���[��)
			//        - �Z���رکR���I��
			//        - �g�b�v�T * (�Z���رکR���[��)  -- Modified by Keelar

			/* �ªO
			thac0 = interpolate( ch->level, thac0_00, thac0_42 )
					- (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield) / 100
					- get_vrace_hrq(victim, wield)
					- race_hit_bonus
					- ( IS_SET(wield->extra_flags, ITEM_HOLY) ?  (wield->level / 9) : 0); */
				
        thac0 = interpolate( ch->level, thac0_00, thac0_42 )
                - race_hit_bonus
                // �Z�� flag �� ITEM_HOLY �R���[������ ITEM_BLESS
				//- ( IS_SET(wield->extra_flags, ITEM_HOLY) ?  (wield->level / 9) : 0);
				- ( IS_SET(wield->extra_flags, ITEM_BLESS) ?  (wield->level / 5) : 0);
				
		if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 ){
			int nWmSklv   = get_skill_level(ch,gsn_weapon_master);
			int nCurrInt  = get_curr_int(ch);
		
			//  Weapon master �Z���رکR�����v��j
			if( get_vrace_hrp(victim, wield) > 100 ){
				thac0 -= (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield)
						  * (1 + nWmSklv * ( nWmSklv + 2 ) * nCurrInt /2000) / 100;
			}else{
				thac0 -= (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield) / 100;
			}

			//  Weapon master �Z���رکR���[����j
			if( get_vrace_hrq(victim, wield) > 0 ){
				thac0 -= get_vrace_hrq(victim, wield) * (1 + nWmSklv * ( nWmSklv + 4 ) * nCurrInt /2000);
			}else{
				thac0 -= get_vrace_hrq(victim, wield);
			}
		}else{
			thac0 = thac0 - (whr + wield->material->value[0]) * get_vrace_hrp(victim, wield) / 100
					 - get_vrace_hrq(victim, wield);
		}
			
			/* �ª��b�N�v�T *
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

    }else{  //  �S�Z����
        int phr = (   ch->hitroll 
                    + (   str_app[get_curr_str(ch)].tohit * 6
                        + dex_app[get_curr_dex(ch)].tohit * 5
                        + wis_app[get_curr_wis(ch)].tohit * 3 ) / 100 ) ;

		//  reposte �|�H hitroll �Χޯ൥���ܰʩR���[�� 2022/01/13
		//  reposte �P�w��H �� isReposte �� bool 2022/05/07
		//if(dt == gsn_reposte && ch->hitroll > 0)
		if( isReposte && ch->hitroll > 0 )
			phr += ( ch->hitroll * ( 25 + get_skill_level(ch, gsn_reposte) * 20
										+ get_skill_level(ch, gsn_reposte) * get_skill_level(ch, gsn_reposte) ) ) / 100;
		//  frenzy , hr �W�L 50 ����, hr ���W�[�N�|�ͽw, �W�L 1 hr �� 0.28 hr
		//  ex: phr 80 ���� 71.6, phr 100 ���� 86
        if ( dt == gsn_frenzy && phr > 50 ){
            phr -= interpolate1(50, 1, 100, 15, (phr-50));
        }

        //thac0 ���p�⤽��, �o��N�O�R���v�����j���F, ���᳣�H thac0 �ӷ��R���v�F
        //�j�P����
        //thac0 =   ���Ѧ]��
        //        - �Ť�R���v
        //        - �ر���n�v�T
        //        - �Ť� frenzy �R���ܮt
        //        - �Ť�g�b (�o�O�� ran �Ť� hr �٦��[���N�O�F ? orz)
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

    //  �B�z�h簤H�ѽ��ϧ����R���[��
    //  �ѤH�X�@�ɹ� hr �[�� 50%
	//  lizardman �Ť�ϧ��R���[���P fist �[���ۦP, �å[�J blind fight �]�w 2022/02/23
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

    //  �w�� pc skill �B�~�A�B�z thac0, �� fist_dam
    //  �{����� ���v�T(thac0, fist_dam) �� �o�l �B�z(fist_dam)
    //  �o�䪺 fist_dam �O�w��, �{�����, �o�l ���ˮ`�[��, �b�᭱�ˮ`�p��ɤ~�|�Ψ�
    if ( !IS_NPC( ch ) ){
		if (   IS_SET( ch->act, PLR_FIST_FIGHT ) && !isLizTail
			&& number_percent() < get_skill_percent( ch, gsn_fist_fight) )
		{
			//  �ץ��{������ɪ� hr �[������ Modified by Razgriz 20060725
			int nFistSklv = get_skill_level(ch, gsn_fist_fight);
			// Previous Version  1.5 ��
			// thac0 += thac0 + thac0 / 2;
			if( dt != gsn_frenzy ){
				thac0 += thac0 * (90 + nFistSklv * 10) / 100;
			}else{
				thac0 += thac0 * number_range(34 + nFistSklv * nFistSklv, 78 + nFistSklv * nFistSklv * 2) / 100;
			}
            if ( !check_blind(ch) && get_skill_percent(ch, gsn_blind_fight) > number_percent() ){
				//  �ץ����t�� Modified by Razgriz 20060722
				thac0 -= 30 + get_skill_level(ch, gsn_blind_fight)*2;
            }else if( check_blind(ch) ){
				thac0 += (thac0 * get_skill_level(ch, gsn_fist_fight)) / 8;
            }

			fist_dam = (9 + get_skill_level(ch, gsn_fist_fight) * 5) / 3;

			if ( is_affected(ch, gsn_powerup) && ch->move > 20 ) {
				ch->move -= 20;
				// powerup �[���� str �v�T�ˮ`
				// fist_dam += 1 + get_skill_level(ch, gsn_powerup)/2;
				fist_dam += 1 + get_skill_level(ch, gsn_powerup) * ( 1 + get_curr_str(ch) * get_curr_str(ch)/250 )/2;
			}
		}
	}

	//  �ݦ� �t�X ���O�r�� ���v�T (thac0, fist_dam)
	//  �o�䪺 fist_dam �O�w�� �ݦ� �t�X ���O�r�� ���ˮ`�[��, �b�᭱�ˮ`�p��ɤ~�|�Ψ�
	//  ��X�s�¤��� Modified by Razgriz 20060717
	if ( ( !IS_NPC (ch) && ch->pcdata->learned[gsn_blood_thirsty] > 0
			//&& is_affected(ch,gsn_berserk)  ���� blood ���ݦb berserk �U�~��@�� 2022//02/21
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

		//  blood thirsty �[���P�w�ѳ̤j��q�ʤ���, �קאּ�԰����y���ˮ`�Ψ���ˮ` 2022/02/21

		// �R���[��
		if ( nMaxHp < 0 ) nMaxHp = 0; // �z�פW���|�o�ͳ̤j hp ���t�Ȫ����p

		bt_hit = 100 + 5 * btsklv + bt_dam * (4 + btsklv/3)/300;

		if( bt_dam > 5000 ) // �R���[���b�֭p�Ȭ� 5000 �ɹF���p�H��U�� , �̤j hp �঳�Ĵ�w�R���[���U��
			bt_hit = bt_hit - ( bt_dam - 5000 ) * (4 + btsklv/3) / ( 150 + nMaxHp );

		thac0 = temp * bt_hit / 100;

		// �ˮ`�[��
		fist_dam = (10000 + bt_dam/8 + 2 * ccon * ccon + btsklv * cstr * cstr)/5100 + ((1 + btsklv * btsklv) * bt_dam)/85000;

		if( is_affected(ch,gsn_berserk) ) // ���O�r�� �U���������ˮ`�[��
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
    }// END -- �ݦ媺�v�T
	
	// weakness_percent �R���P�w add at 2020/09/18
	if(    dt == gsn_frenzy
		|| dt == gsn_backstab
		|| dt == gsn_bashhead
		|| dt == gsn_circular
		|| dt == gsn_tactics  // lizardman ����
		|| dt == gsn_empslash)
	{
		thac0 = thac0 * weakness_percent(victim , dt) / 100;
	}

	// ���� �� weapon type weakness_percent �R���z�I�ӧO�]�w add at 2020/09/18
	// weapon master �� type �z�I�W�[�R�� 2020/10/14

	if( dt == 1000 ) // �Ť����
		thac0 = thac0 * weakness_percent(victim , gsn_hit_attack) / 100;
	if( wield && wield->value[3] == 1 )  // �U�Q����
	{
		if( weakness_percent(victim , gsn_slice_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_slice_attack)
							+ (weakness_percent(victim , gsn_slice_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_slice_attack) / 100;
	}
	if( wield && wield->value[3] == 2 )  // �²r�@�W
	{
		if( weakness_percent(victim , gsn_stab_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_stab_attack)
							+ (weakness_percent(victim , gsn_stab_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_stab_attack) / 100;
	}
	if( wield && wield->value[3] == 3 )  // ���R�A��
	{
		if( weakness_percent(victim , gsn_slash_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_slash_attack)
							+ (weakness_percent(victim , gsn_slash_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_slash_attack) / 100;
	}
	if( wield && wield->value[3] == 4 )  // �`���@�@
	{
		if( weakness_percent(victim , gsn_whip_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_whip_attack)
							+ (weakness_percent(victim , gsn_whip_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_whip_attack) / 100;
	}
	if( wield && wield->value[3] == 5 )  // �ΤO�@��
	{
		if( weakness_percent(victim , gsn_claw_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_claw_attack)
							+ (weakness_percent(victim , gsn_claw_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_claw_attack) / 100;
	}
	if( wield && wield->value[3] == 6 )  // �z�o�Q��
	{
		if( weakness_percent(victim , gsn_blast_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_blast_attack)
							+ (weakness_percent(victim , gsn_blast_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_blast_attack) / 100;
	}
	if( wield && wield->value[3] == 7 )  // �U�v�Oݴ
	{
		if( weakness_percent(victim , gsn_pound_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_pound_attack)
							+ (weakness_percent(victim , gsn_pound_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_pound_attack) / 100;
	}
	if( wield && wield->value[3] == 8 )  // �����V��
	{
		if( weakness_percent(victim , gsn_crush_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_crush_attack)
							+ (weakness_percent(victim , gsn_crush_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_crush_attack) / 100;
	}
	if( wield && wield->value[3] == 9 )  // �R�Q�@��
	{
		if( weakness_percent(victim , gsn_grep_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_grep_attack)
							+ (weakness_percent(victim , gsn_grep_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_grep_attack) / 100;
	}
	if( wield && wield->value[3] == 10 ) // �i�f�r�r
	{
		if( weakness_percent(victim , gsn_bite_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_bite_attack)
							+ (weakness_percent(victim , gsn_bite_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_bite_attack) / 100;
	}
	if( wield && wield->value[3] == 11 ) // �y�U����
	{
		if( weakness_percent(victim , gsn_pierce_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_pierce_attack)
							+ (weakness_percent(victim , gsn_pierce_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_pierce_attack) / 100;
	}
	if( wield && wield->value[3] == 12 ) // �r�P�l�m
	{
		if( weakness_percent(victim , gsn_suction_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_suction_attack)
							+ (weakness_percent(victim , gsn_suction_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_suction_attack) / 100;
	}
	if( wield && wield->value[3] == 13 ) // ���s��
	{
		if( weakness_percent(victim , gsn_chop_attack) > 100 )
			thac0 = thac0 * ( weakness_percent(victim , gsn_chop_attack)
							+ (weakness_percent(victim , gsn_chop_attack) - 100 )
							* get_skill_level(ch,gsn_weapon_master)/15 ) / 100;
		else
			thac0 = thac0 * weakness_percent(victim , gsn_chop_attack) / 100;
	}
	if( wield                            // �g�X�b�� , �j��g��
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


    //����Y�p dodge �ƭ�, �H��K������ thac0 �����
    //1. victim �O mob ����, mob dodge �Y�� 1/10
    //2. npc �� pc ��, pc dodge �Y�� 1/6
    //3. ��L�����p(�]�N�O PK), dodge �Y�� 1/8
    if( IS_NPC(victim) ){
        victim_dodge = GET_DODGE( victim ) / 10;
    }else if( IS_NPC(ch) ){
        victim_dodge = GET_DODGE( victim ) / 6;
    }else{
        victim_dodge = GET_DODGE( victim ) / 8;
    }

    // mob class �� thief �� dc �ѼW�[ 50% �U���� 25% 
	// �åB�[�J dc �������t�Ȯɤ~�|�@�� 2020/05/01
    if ( IS_NPC( victim ) && IS_SET( victim->class, 4 ) && victim_dodge < 0 )
        victim_dodge += victim_dodge / 4;

    //ch �ݤ��� victim ��, ���� victim �� dodge, �����ܬۭ��C ch ���R���v
    if ( !can_see( ch, victim ) )
        victim_dodge -= 60;

    //   0 <= number_bits( 5 ) <= 31
    // -30 <= diceroll         <=  0
    //�o��O�N��, ����R�����w�ĪŶ�
    //ex: dodge -700 �� mob
    //    hr > 100, �����R��, ���٬O�� 1/32 ���v������ (diceroll = -30, 11110 = 30)
    //    70 < hr < 100, �H�� hr �V��, �����쪺���v�V�p
    //    hr < 70, ������, ���٬O�� 1/32 ���v�ॴ�� (diceroll = 0, 00000 = 0)
    while ( ( diceroll = - number_bits( 5 ) ) <= -30 ) ;


    //pc �� �S���] ACT_SPEEDHIT �� npc ���n�i�ӧP�_�O�_�R��, �]�i�h�N����� miss 
    //¶�� �� ������C �����N�����i�ӤF
	if( !IS_NPC( ch ) || !IS_SET( ch->act, ACT_SPEEDHIT ) )
	{
		if (   dt != gsn_circle
			&& dt != gsn_twin
			&& ( ( diceroll != 0 && diceroll < thac0 - victim_dodge )
				|| diceroll == -30 ) )
		{
			/* Miss. */
			//�i�ӳo��N�N��S����F

			// 1/4 ���v���� darkraid
			if ( IS_AFFECTED( ch, AFF_DARK_RAID ) && number_bits( 2 ) == 1 ){
				affect_strip( ch, gsn_dark_raid );
				send_to_char( "�A���M�`�O�Q�����F�I\n\r", ch );
				darkraid = 1;
			}

			// 1/4 ���v���� nightstalk
			if ( IS_AFFECTED( ch, AFF_NIGHTSTALK )  && number_bits( 2 ) == 1 ){
				affect_strip( ch, gsn_nightstalk );
			}

			if ( msg_mode == MSG_NORMAL ) dam_message( ch, victim, -1, dt, wpn );

			dam = -1; // -1 denotes a missing attack
					  // -2 denotes a failure attack

			// �[�J ���� miss �����p�U��b�P�u�ļƶq�P�w 2022/05/04
			if( isShootCost )
			{
				if( arrow )
					ShotObjCount( ch , arrow );
				if( ammo )
					ShotObjCount( ch , ammo );
			}

			// mob (victim) �k�]�B�z , ��ACT_NO_FLEE �N���|�@do_flee
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

	/*  ���m�̫e��@�P�w 2022/05/07
    //���ؤ��� ���\, ��������A�����Z�����������A
    if ( dt == gsn_reposte ){
        dt = TYPE_HIT;
        isReposte = TRUE;
        if ( wield && wield->item_type == ITEM_WEAPON ){
            dt += wield->value[3];
        }
        if ( dt_mode == DT_CHANGE ) *dt_orig = dt;
    } */

    //�ˮ`�p��}�l !!

    //�Z���y�����򥻶ˮ`�p��
    if ( IS_NPC( ch ) ){
    //  npc ����, �H ���S���Z�� �� npc���� �ӭp��򥻶ˮ`
		//  �[�J �j��g�� ���P�w 2022/05/01
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
			// �ഫ �j��g�� ���Z�� 2022/05/01
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
	//  pc ����, �����Z����
		if ( wield )
		{
		//  �B�z�Z���i�l
			if( wield->value[3] != 14 && number_bits(5) < 8 ){ // 7/(16 + 8) , 29%
				if( get_skill_level(ch,gsn_weapon_master) * 6 < number_percent() ) // weapon master ���ŭ��C�Z���l�a���v 2022/12/02
					waste_dura( wield );
			}

		//  �Z���ˮ`�O (value[1] �� value[2] �� weapon �̤p�̤j�ˮ`
			// �j��g�� ���� �Z���ˮ`�O �v�T 2022/05/01
			if( wield->value[3] == 16 && ammo )
			{
				;
			}
			else
			{
				dam = number_range( wield->value[1], wield->value[2] );
				dam += dam/4;
			}
		//  ����Z����, �ˮ`���[���B�z

			/*  �줽���L�h���~�B���@�� , ���s�ק�P�w�� 2021/10/13
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
				}else if( wpn == ITEM_TWO_HAND ){ // ���Z�����⮳�ɪ��ˮ`�[��
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

			// �˳�����Z���B���� �g�b �� �j�� , 2022/05/01
			//if( wpn == WEAR_TWO_HAND && wield->value[3] != 14 ){  // �N�P�w�Z�����Χאּ�P�w�O���b�μu��
			if( wpn == WEAR_TWO_HAND && !arrow && !ammo ){
				if( CAN_WEAR(wield, ITEM_WIELD ) ){  // item wield �P twohand �æs�ɷ|�u���P�w���e��(�����)
					dam += dam/5;  // �[�� 20%
					if( (number_percent() // powerblow �B�~�[�� lv 6 , 80% ���v
							< ( get_skill_percent(ch, gsn_powerblow)/5 + get_skill_level(ch, gsn_powerblow)*10 ))
						|| isBloodThirsty ) // �D�� blood thirsty ���A�U 100% ���\ add at 2021/12/25
					{
						dam = wield->value[2];
						dam += dam/5;
						dam += dam * get_skill_level(ch, gsn_powerblow)/24; // �̰��h 25%
						isPowerBlow = TRUE; // �P�w powerblow ���� 2021/12/25
					}
				}else{ // �˳Ƥ��� item wield �h���w�� item twohand ( npc reset ���˳Ƥ��|�o�ͦb���B )
					dam += dam/3;  // �[�� 33%
					if( (number_percent()
							< ( get_skill_percent(ch, gsn_powerblow)/5 + get_skill_level(ch, gsn_powerblow)*10 ))
						|| isBloodThirsty )
					{
						dam = wield->value[2];
						dam += dam/3;
						dam += dam * get_skill_level(ch, gsn_powerblow)/20; // �̰��h 30%
						isPowerBlow = TRUE;  // �P�w powerblow ���� 2021/12/25
					}
				}
			}

		//  �Z���i�l��ˮ`���v�T
			//  ���m�Ҧ��ˮ`�P�w���̫�íק�ƭȨϿi�l�v�T��j 2021/10/13
				// �j��g�� �ɿi�l�פ��|�v�T��ˮ` 2022/05/01
			if( !indestructible( wield ) && !ammo ){
				int cd = get_curr_dura(wield), md = get_max_dura(wield);
				/* if( cd * 2 <= md ) {
					dam -= (dam/3);
				}else{
					if(cd * 3 <= md ) dam /= 2;
				}*/

				// �[�J���@�[�תZ���]�w , 300 �H�W�h�����@�[�תZ�� 2022/12/02
				// �Z���@�[�פj�� 300 ���|�P�w��� 2022/12/02
				if( cd <= 300 )
				{
					// �̤j�@�[�פj�� 300 ��, �h�H 300 �@���̤j�@�[�� 2022/12/02
					if( md > 300 ) md = 300;
					if( cd * 10 <= md )          // cd 10% , dam 10%
						dam = dam - dam * 9/10;
					else if( cd * 10 <= md * 2 ) // cd 20% , dam 30%
						dam = dam - dam * 7/10;
					else if( cd * 3 <= md )      // cd 33% �i�l, dam 50%
						dam = dam - dam * 5/10;
					else if( cd * 10 <= md * 5 ) // cd 50% , dam 70%
						dam = dam - dam * 3/10;
					else if( cd * 10 <= md * 7 ) // cd 70% , dam 90%
						dam = dam - dam * 1/10;
				}
			}

			//  ���}�ɡA�N�b�]���D�n���Z���A�ӭ�Z���h��� bow �Ӧs
			if ( arrow && dt != gsn_frenzy ){
				bow   = wield;
				wield = arrow;
			}
			// �ഫ �j��g�� ���Z�� 2022/05/01
			if ( ammo ){
                gun   = wield;
                wield = ammo;
			}
		}else{  //  pc ����, �S���Z���� , �ˮ`�� �髬 size �v�T
			//dam = number_range( 1, 2 ) * race_table[ ch->race ].size;
			dam = number_range( 1, 2 ) * UMAX( 10 , get_curr_size( ch ) ); // �����髬 size ���� 2022/12/23
		}

		/* ���۪Z���B�ˮ`�W�L 1000, log �_��
		if ( wield && dam > 1000 ){
			sprintf( buf, "One_hit dam range > 1000 from %d to %d",wield->value[1], wield->value[2] );
			bug( buf, 0 );
		}*/
    }


	/* waste durability */
	//�Q�����誺�˥ҿi�l�B�z
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

	//�B�~�ˮ`�[��

	//�B�z�Z����رڪ��[��, �Z�����誺�[��, �٦����e�� fist_dam �[��
	if( wield && !IS_NPC(ch) && !isLizTail){
		//  �ϥνb�g���ɡA�]�P�ɦҼ{�}������رڪ��[��    added by keelar 
		if(arrow != NULL && dt != gsn_frenzy && arrow == wield && bow){
			int nBowAvg   = number_range( bow->value[1], bow->value[2] );
			int nArwAvg   = number_range( arrow->value[1], arrow->value[2] );
			int nArchSklv = get_skill_level(ch, gsn_shot_arrow);
			int nDamBonus = 0;
			
			// ��avg���ˮ`�������C
			if( !IS_SET( bow->extra_flags, ITEM_CLAN ) && nBowAvg > 87)
				nBowAvg = 87 + (nBowAvg - 87) * 0.5;
			// clan wp ��avg���ˮ`�������C
			if( IS_SET( bow->extra_flags, ITEM_CLAN ) && nBowAvg > 70)
				nBowAvg = 70 + (nBowAvg - 70) * 0.1;
			
			// nBowAvg ��F 35 .. �|�� fist ������
			// �W�[ ����Z�� dr �ĪG�[�� 2022/05/06
			nDamBonus = ( get_damroll(ch, wpn) + ShootObjAffect(ch, arrow, 2) )
						* (nBowAvg * 2 + nArwAvg) * get_vrace_drp(victim, bow) / 5500;

			// ���C archery ��ˮ`���v�T , �԰��򥻶ˮ`�[��  2022/05/01
			//nDamBonus = nDamBonus * (100 + nArchSklv * 50) / 100;
			//dam += nDamBonus * (100 + 2 * get_skill_percent( ch, gsn_shot_arrow )) / 200;
			nDamBonus = nDamBonus * (190 + nArchSklv * 35) / 100;
			dam += nDamBonus * (201 + get_skill_percent( ch, gsn_shot_arrow )) / 200;

			// Previous versiono of damage calculation
			// dam += (( get_damroll(ch, wpn) * fist_dam * get_vrace_drp(victim, bow)) / 500)
			//         * get_vrace_drp(victim, wield) / 100;
			dam += (bow->material->value[1] ) + (wield->material->value[1] );
			dam += (get_vrace_drq(victim, bow) + get_vrace_drq(victim, wield)) * (nBowAvg * 2 + nArwAvg) / 55;
		//  �j��g�����ˮ` 2022/05/01
		}else if( ammo != NULL && ammo == wield && gun ){
			int nAmmoDam = number_range( (ammo->value[1]*19)/20 , (ammo->value[1]*20)/19 );
			int nFMSklv = get_skill_level(ch, gsn_firearm_mastery);
			int nDamBonus = 0;
			// �W�[ ����Z�� dr �ĪG�[�� 2022/05/06
			nDamBonus = ( ( get_damroll(ch, wpn) + ShootObjAffect(ch, ammo, 2) + nAmmoDam ) * 23 ) * get_vrace_drp(victim, gun) / 550;
			nDamBonus = nDamBonus * (400 + nFMSklv * 75) / 200;
			dam += nDamBonus * (201 + get_skill_percent( ch, gsn_firearm_mastery )) / 200;

			dam += ( ( gun->material->value[1] + wield->material->value[1]
					+ get_vrace_drq(victim, gun) + get_vrace_drq(victim, wield)
					+ nAmmoDam ) * 23 ) / 55;
        }else{
            int nWpnAvg = number_range( wield->value[1], wield->value[2] );
			
			// ��avg���ˮ`�������C
			if( !IS_SET( wield->extra_flags, ITEM_CLAN ) && nWpnAvg > 87)
				nWpnAvg = 87 + (nWpnAvg - 87) * 0.5;
			// clan wp ��avg���ˮ`�������C
			if( IS_SET( wield->extra_flags, ITEM_CLAN ) && nWpnAvg > 70)
				nWpnAvg = 70 + (nWpnAvg - 70) * 0.1;
			
            if( !IS_NPC(ch) && ch->pcdata->learned[gsn_powerblow] > 0){
                int nPbSklv  = get_skill_level(ch, gsn_powerblow);
                int nPbBonus = wield->weight * (nPbSklv * nPbSklv) / 300;
				// �Τ@�榸������ powerblow �P�w 2021/12/25
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
            
            // nWpnAvg ��F 44 (��M).. �|���H�e������ (44 / 55) = 4 / 5, 
            // fist_dam / 4 �Χ@�[�����
			if( !IS_NPC(ch) && ch->pcdata->learned[gsn_weapon_master] > 0 && get_vrace_drp(victim, wield) > 100){
				// Weapon master �Z���رڶˮ`�ʤ���[��
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
				// Weapon master �Z���رڶˮ`�[��
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
			dam += ( 2 * get_skill_percent( ch, gsn_fist_fight) + 100) // �Ť� dam += dr * 3;
					* ( ((get_damroll( ch, wpn ) ) * fist_dam ) / 4 ) / 300;
		}else{
			dam += get_damroll(ch, wpn);
			if( IS_SET( ch->class, 32 ) && dam > 0){ //mob �� class 32 (bloody) �ɪ��Ť�����ˮ`�[��  2021/10/11
				if( IS_SET( ch->class, 1 ) ) // �קK class 1 (warrior) �⦸�[���L��  2021/10/11
					dam = dam + dam * 2;
				else
					dam = dam + dam * 3;
			}
		}
	}else{
		dam += get_damroll(ch, wpn);
	}
	
	/*  �ק� poisoned weapon �\�� , ���������ʪ��ˮ`�[�� 2021/11/10
    //�D�g�b��, ���r�Z���ˮ`�[�� 1/4
    if ( wield && IS_SET( wield->extra_flags, ITEM_POISONED ) && dt != 1014 ){
        //clan�Z���[�� 1/8, ��ʩ٬r�Z���[�� 1/4 (? )
		dam += dam / 8;
		// 18/07/20 clan weapon �ˮ`�[�� 1/8 , �䥦�[�� 1/4
		if( !IS_SET( wield->extra_flags, ITEM_CLAN ) )
			dam += dam / 8;

		//�ˮ`���� �����B�~�[�� 20200916
        //if(dt == gsn_circle)
        //    dam += dam * (20 + get_skill_level(ch, gsn_circle) * 4) / 100;
        //else if(dt == gsn_backstab)
        //    dam += dam * (20 + get_skill_level(ch, gsn_backstab) * 4) / 100;

    }
	*/

	// victim ���t���ؼЮ�,�ˮ`�[�� 2020/09/16
	if ( ch->assassinating && ch->assassinating == victim )
	{
		dam += dam * get_skill_level(ch, gsn_ass)/20;
	}

	/*  kick ���|�]�J mone_hit , ��g�� do_kick ���U 2022/02/23

	// mon kick �ˮ`�[�� 4 ��
	if( !IS_NPC(ch) && ch->class == MONK && dt == gsn_kick ){
		dam  *= 4;
	}
	*/

    //  tactics �v�T�L�q�ˮ`
	if(!IS_NPC(ch) && get_skill_percent(ch, gsn_tactics)){
		int nTacPerc = get_skill_percent(ch, gsn_tactics);
		int nTacSklv = get_skill_level(ch, gsn_tactics);
		dam = dam * (30000 + nTacPerc * (9 + nTacSklv * nTacSklv) ) / 30000;
	}

	//weapon master ���[���B�z
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

		// �g�����[���u�� 1/2 2022/05/02
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
    
	//enhanced damage ���[���B�z
	if (    !IS_NPC( ch )
		&& ( get_skill_percent(ch, gsn_enhanced_damage) + get_skill_level(ch, gsn_enhanced_damage) * 5 > number_percent()
			|| nTpPerc + nTpSklv * 5 > number_percent()
			// �l�[ powerblow ���P�w 2022/02/23
			|| get_skill_percent(ch, gsn_powerblow) + get_skill_level(ch, gsn_powerblow) * 5 > number_percent() )
		&& dt != 1014 // �b�ڤ��� ed �[�� 2022/05/01
		&& dt != 1016 // �j��g������ ed �[�� 2022/05/01
		&& (   dt >= TYPE_HIT
			|| dt == gsn_frenzy
			//|| dt == gsn_kick  /* kick ���|�Q�N�J mone_hit ����*/
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
			dam += ( dam /3); // mar ed �[�� 2022/02/23
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

	//mob ���O �g�b �� �j�u �ɪ��ˮ`�[��
	if ( IS_NPC ( ch ) && dt != 1014  && dt != 1016 )
	{
		//mob �� class 1 (warrior) �ɪ��ˮ`�[��
		if ( IS_SET( ch->class, 1 ) )
			dam = dam + dam;
	}

	//  powerbolt �ĪG�P�w 2022/05/01
	if( is_affected(ch, gsn_powerbolt) )
	{
		//  ���b�����p�U
		if( arrow ){
			//  �C�U�����|�l mana , �������p�G mana �����h�����ĪG
			if( arrow->pIndexData->vnum != OBJ_VNUM_MAGIC_ARROW ){
				if( ch->mana < 5)
					affect_strip(ch, gsn_powerbolt);
				else
					ch->mana -= 5;
			}else{
				//  ���]�k�b�ӷl���C
				if( ch->mana < 2)
					affect_strip(ch, gsn_powerbolt);
				else
					ch->mana -= 2;
			}
		}else{
			//  �S�b�����p�U�������� powerbolt �ĪG
			affect_strip(ch, gsn_powerbolt);
		}
	}

	//  �B�z�h簤H�ѽ��ϧ����ˮ`�[��
	if(isLizTail){ //  lizardman ���ϧ�����
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


	//�������A�� �g�X�b�� shot, �B���b�� , ran�����ƶˮ`
	if ( dt == 1014 && arrow )
	{

		/*   �Ҽ{ ran �|�ϥΨ�L�ޯ�(�p frenzy) �ɭP dt ���ܤ����g�b(1014) �ϧP�w������ ,
			 �B powerbolt �i��ϥΫ� remove arrow �y���L arrow �ϧP�w�]������
			 �G���� apdr �[���e�@�P�w 2022/05/01

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
		
		//�[�W�b���ˮ`�O
		int nArwAvg = number_range(arrow->value[1] , arrow->value[2]);
		dam += nArwAvg * (get_damroll(ch, wpn) + 100) * get_vrace_drp(victim, bow) / 10000;

		if( !IS_NPC(ch) ) 
		{
			//�g�b�����ƥ[��, ��إ[��
			int nCurrWis = get_curr_wis(ch);
			int nCurrDex = get_curr_dex(ch);
			
			if( number_percent() < ((nCurrWis - 12) * abs(nCurrWis - 12)) / 20)
			//if( number_percent() < (((get_curr_dex(ch)+get_curr_wis(ch) )* 10) / 40))
			{
				//thief ����ˮ`�[��
				if(ch->pcdata->prime_attr == APPLY_DEX)
				{
					dam *= 2 + (nCurrDex*nCurrDex / 400) + (get_skill_level(ch,gsn_shot_arrow)) / 2;
				}else{
					dam *= 2 + (get_skill_level(ch,gsn_shot_arrow)) / 2;
				}
				//  �N�b���[���ˮ`�C�J ED �[���έP�R�@�����p�⤧��  modified by keelar
				dam += arrow->value[3] * (1 + get_skill_level(ch, gsn_shot_arrow) * 1 / 10);
				// pdr ��˼g�� fight_damage �|�y�������ƸI�췥�� pdr �����p�U�̵M�y���ˮ` 2020/11/01
				dam -= victim->pdr;
			}
			else if(nCurrWis > number_percent())
			// else if(  ( (get_curr_dex(ch) + get_curr_wis(ch) )) / 2 > number_percent() )
			{
				//thief ����ˮ`�[��
				if(ch->pcdata->prime_attr == APPLY_DEX)
				{
					dam *= 1 + (nCurrDex*nCurrDex / 500) + (get_skill_level(ch,gsn_shot_arrow) ) / 5;
				}else{
					dam *= 1 + (get_skill_level(ch,gsn_shot_arrow) ) / 5;
				}
				//  �N�b���[���ˮ`�C�J ED �[���έP�R�@�����p�⤧��  modified by keelar
				dam += arrow->value[3] * (1 + get_skill_level(ch, gsn_shot_arrow) * 1 / 12);
				dam -= victim->pdr;
			}else{
				dam += arrow->value[3];
				dam -= victim->pdr;
			}
		}
		if( IS_NPC(ch) && number_bits( 3 )== 2 )
		{
			//�g�b�����ƥ[��
			dam *= 2+(get_skill_level(ch,gsn_shot_arrow)*2)/4;
			dam -= victim->pdr;
		}
	}

	//�g�ɧ����[��
	if ( !IS_NPC(ch) && dt == gsn_frenzy && ch->class == MARTIALMAN)
	{
		dam = (dam * ch->level) / 28;
	}
	else if( !IS_NPC(ch) && dt == gsn_frenzy )
	{
		dam = (dam*ch->level) / 32;
	}

	//�I��[��
	if ( dt == gsn_backstab )
	{
		if( !IS_NPC(ch) && ch->class == ASSASSIN)
		{
			/*  �ª� asn , lv 41 sklv 6  dam = 13.8 ��
			dam = dam + (dam * (   UMIN( ( ch->level / 3 ), 14 ) / 2
								 + (   get_skill_level(ch,gsn_backstab)
									 + get_skill_level(ch,gsn_ass ) ) /2 ) ) ;
			*/

			// sklv 6 dam = 10 ��  add at 2020/09/16
			dam = dam + ( dam * ( 1 + get_skill_level(ch,gsn_backstab)*1.5 ) );
		}
		else
		{
			/*  �ª���Lclass , lv 41 sklv 6  dam = 8.63 ��
			dam = dam /2 + (dam * (   UMIN( ( ch->level / 4 ), 11 ) / 2
									+ (   get_skill_level(ch,gsn_backstab)
										+ get_skill_level(ch, gsn_ass) ) /2 ) ) ;
			*/

			// sklv 6  dam = 8 ��  add at 2020/09/16
			dam = dam + ( dam * ( 1 + get_skill_level(ch,gsn_backstab) ) );
		}
		//�� �]ŧ �ˮ`�A�[��, Added by Razgriz 20051015
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

	//¶��[��
	if ( dt == gsn_circle )       /* 200% to 300% at lev. 40 */
	{
		//    dam += dam  + ( dam * (ch->level+get_skill_level(ch,gsn_circle)+3*get_skill_level(ch,gsn_circle)/6)) / 10;
		dam += dam + ( dam * ( 2 * get_skill_level(ch,gsn_circle)
								+ 3 * get_skill_level(ch,gsn_circle) / 2
								+ 3 * get_skill_level(ch,gsn_circle) / 5
								+ 4 * get_skill_level(ch,gsn_circle) / 6 ) / 14);

		//�� �]ŧ �ˮ`�A�[�� add at 2020/09/16
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


	//�� �]�O �������Z��, ���Y�Ǻر�, �����v�ʪ� 1/5 �ˮ`�[��
	dam += check_item_magic_flag(wield, victim->race, dam);

	// �� �t�� �������Z��, ���v�ʼW�[�ˮ`�[��
	// �קאּ�W�[�Z�� avg �ˮ` �üW�[�Z�����l 2021/10/11
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

	// empslash �� ���z�ˮ` �[��
	if(dt == gsn_empslash){
		int nEmpSklv = get_skill_level(ch, gsn_empslash);
		dam += ( ch->apdr * nEmpSklv) / 3;
	}

	// powerblow ���߮ɹ�apdr(���z�ˮ`)�[�� , �B�j�񤣷|�@�� 2022/05/02
	if( isPowerBlow && dt != 1016 ){
		int nPbSklv = get_skill_level(ch, gsn_powerblow);
		dam += (ch->apdr * nPbSklv) / 2;
	}

	// �j��g�� �� ���z�ˮ` �[��
	if(dt == 1016){
		int nFMSklv = get_skill_level(ch, gsn_firearm_mastery);
		// �W�[ ����Z�� apdr �ĪG�[�� 2022/05/06
		dam += ( ( ch->apdr + ShootObjAffect(ch, ammo, 3) ) * nFMSklv )/ 3;
	}

	//���O�r��[��
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

	//�B�z�P�R�@��
	dam = get_critical_damage(dam, ch, victim, dexdiff, fCritical, dt, darkraid);

	// �l�[�P�w dam ���L�p�� 0  , 2020/11/08
	if ( dam <= 0 )
	{
		dam = 0;
	}
	//�@�Ҩ��äO(armor, ac) ���ˮ`��B�z
	if ( !IS_NPC(victim))
	{
		if ( arrow || ammo ) // �[�J �j��g�� ���P�w 2022/05/01
		{
			int nEnemyAC = GET_AC(victim);
			if(nEnemyAC >= -300){
				dam = dam * (600 + GET_AC(victim)) / 300;
			}else{
				dam = dam * 300 / (0 - GET_AC(victim));
			}
		}else{
			// �[�J powerblow ���C��� ac �@�w��Ҫ��P�w 2021/12/25
			int nPbAc = 0;
			// ��� ac �� 0 �p , sklv 6 �����C 40% ac
			if( isPowerBlow && GET_AC(victim) < 0 && ( ( isBloodThirsty && number_percent() < 30 ) || number_percent() < 20 ) ){
				nPbAc = GET_AC(victim) * ( 100 - (5400/(54 + get_skill_level(ch, gsn_powerblow) * get_skill_level(ch, gsn_powerblow) ) ) );
				nPbAc = nPbAc / 100;
			}

			int nEnemyAC = ( GET_AC(victim) - nPbAc ) * 0.1; // ac �ƭȹL�j�y���p�⬰�t�� 2020/09/20
			if(GET_AC(victim) <= -1000){
				dam = dam * 3150 / ( nEnemyAC * nEnemyAC + 500 ); //  ac 1200 78% , ac 1500 86%
				//dam = dam * 1820000 / (nEnemyAC * nEnemyAC + 3000000);  �ƭȹL�j 2020/09/20
			}else{
				dam = UMIN( dam + dam * ( GET_AC(victim) - nPbAc )/1429 , dam + ( GET_AC(victim) - nPbAc )/8 ); // ac 1000 70% , 800 55%
			}
		}
		//  dam = dam + UMIN( 0, ( arrow ) ? GET_SVS( victim ) * 3
		//    : (GET_AC( victim ) / 3 ));
	}
	else
	{
		if ( arrow || ammo )  // �[�J �j��g�� ���P�w 2022/05/01
		{
			//int nEnemyAC = GET_AC(victim);
			if(GET_AC(victim) >= -480){
				dam = dam * (1000 + GET_AC(victim) ) / 480;
			}else{
				dam = dam * 480 / (0 - GET_AC(victim));
			}
		}else{
			// �[�J powerblow ���C��� ac �@�w��Ҫ��P�w 2021/12/25
			int nPbAc = 0;

			if( isPowerBlow && GET_AC(victim) < 0 && ( ( isBloodThirsty && number_percent() < 35 ) || number_percent() < 20 ) ){
				nPbAc = GET_AC(victim) * ( 100 - (5400/(54 + get_skill_level(ch, gsn_powerblow) * get_skill_level(ch, gsn_powerblow) ) ) );
				nPbAc = nPbAc / 100;
			}

			int nEnemyAC = GET_AC(victim) - nPbAc;
			if(nEnemyAC > 0){
				dam = dam + dam * UMIN( GET_AC(victim)/100 , 1); // ac �����ȶˮ`�W�[�̰��@��
			}
			else
			{
				if(nEnemyAC < -7000){
					dam = dam + nEnemyAC + 7000 - dam * 4 / 5; // ac -7001 �H�W�C ac �� 1 �ˮ`
				}
				else if(nEnemyAC >= -2000  ){
					dam = UMIN( dam + dam * nEnemyAC/8000 , dam + nEnemyAC/7 ); // ac -2000 �ˮ`��֬� 25%
				}
				else if(nEnemyAC >= -4000){
					dam = dam + dam * (nEnemyAC + 2000)/8500 - dam/4; // ac -4000 ��֬� 48.5%
				}
				else {
					dam = dam + dam * (nEnemyAC + 4000)/9000 - (dam * 20)/85 - dam/4; // ac -7000 ��֬� 81% , -6000 ��֬� 70%
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

	/* mana shield ���� fight_damage ���P�w 2023/01/09

		// �]�O�̻� ���ˮ`��B�z
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
	
	// ���� �� weapon type weakness_damage �ˮ`�z�I�ӧO�]�w add at 2020/09/18
	// weapon master �� type �z�I�W�[�ˮ` 2020/10/14

	if( dt == 1000 ) // �Ť����
		dam = dam * weakness_damage(victim , gsn_hit_attack) / 100;
	if( wield && wield->value[3] == 1 )  // �U�Q����
	{
		if( weakness_damage(victim , gsn_slice_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_slice_attack)
						+ (weakness_damage(victim , gsn_slice_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_slice_attack) / 100;
	}
	if( wield && wield->value[3] == 2 )  // �²r�@�W
	{
		if( weakness_damage(victim , gsn_stab_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_stab_attack)
						+ (weakness_damage(victim , gsn_stab_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_stab_attack) / 100;
	}
	if( wield && wield->value[3] == 3 )  // ���R�A��
	{
		if( weakness_damage(victim , gsn_slash_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_slash_attack)
						+ (weakness_damage(victim , gsn_slash_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_slash_attack) / 100;
	}
	if( wield && wield->value[3] == 4 )  // �`���@�@
	{
		if( weakness_damage(victim , gsn_whip_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_whip_attack)
						+ (weakness_damage(victim , gsn_whip_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_whip_attack) / 100;
	}
	if( wield && wield->value[3] == 5 )  // �ΤO�@��
	{
		if( weakness_damage(victim , gsn_claw_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_claw_attack)
						+ (weakness_damage(victim , gsn_claw_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_claw_attack) / 100;
	}
	if( wield && wield->value[3] == 6 )  // �z�o�Q��
	{
		if( weakness_damage(victim , gsn_blast_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_blast_attack)
						+ (weakness_damage(victim , gsn_blast_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_blast_attack) / 100;
	}
	if( wield && wield->value[3] == 7 )  // �U�v�Oݴ
	{
		if( weakness_damage(victim , gsn_pound_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_pound_attack)
						+ (weakness_damage(victim , gsn_pound_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_pound_attack) / 100;
	}
	if( wield && wield->value[3] == 8 )  // �����V��
	{
		if( weakness_damage(victim , gsn_crush_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_crush_attack)
						+ (weakness_damage(victim , gsn_crush_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_crush_attack) / 100;
	}
	if( wield && wield->value[3] == 9 )  // �R�Q�@��
	{
		if( weakness_damage(victim , gsn_grep_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_grep_attack)
						+ (weakness_damage(victim , gsn_grep_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_grep_attack) / 100;
	}
	if( wield && wield->value[3] == 10 ) // �i�f�r�r
	{
		if( weakness_damage(victim , gsn_bite_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_bite_attack)
						+ (weakness_damage(victim , gsn_bite_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_bite_attack) / 100;
	}
	if( wield && wield->value[3] == 11 ) // �y�U����
	{
		if( weakness_damage(victim , gsn_pierce_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_pierce_attack)
						+ (weakness_damage(victim , gsn_pierce_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_pierce_attack) / 100;
	}
	if( wield && wield->value[3] == 12 ) // �r�P�l�m
	{
		if( weakness_damage(victim , gsn_suction_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_suction_attack)
						+ (weakness_damage(victim , gsn_suction_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_suction_attack) / 100;
	}
	if( wield && wield->value[3] == 13 ) // ���s��
	{
		if( weakness_damage(victim , gsn_chop_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_chop_attack)
						+ (weakness_damage(victim , gsn_chop_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_chop_attack) / 100;
	}
	// �g�X�b�� �� �j��g�� , ���ɪ� wield ���O�� arrow �� ammo , �� wield �� bow �� gun 2022/05/01
	if( ( bow && bow->value[3] == 14 && arrow ) || ( gun && gun->value[3] == 16 && ammo ) )
	{
		if( weakness_damage(victim , gsn_shot_attack) > 100 )
			dam = dam * ( weakness_damage(victim , gsn_shot_attack)
						+ (weakness_damage(victim , gsn_shot_attack) - 100 ) 
						* get_skill_level(ch,gsn_weapon_master)/10 ) / 100;
		else
			dam = dam * weakness_damage(victim , gsn_shot_attack) / 100;
	}


    //�ˮ`�p�⵲��!!!!

    //�e�X�ˮ`     Modified by Razgriz 20060705
	if (     msg_mode == MSG_NORMAL 
		|| ( msg_mode == MSG_BRIEF && !is_brief ( ch, victim ) ) )
	{
		// �N reposte dmg_mode �קאּ DMG_ADJUST �H�P�w pdb 2021/11/16
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
		// �N reposte �קאּ DMG_ADJUST , �ò��� arrow �bbrief �� DMG_ADJUST ���]�w 2021/11/16
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

	// �B�z mob ���e���������D  2022/02/24
	// mob death_prog �����������O�Q reposte Ĳ�o hp �|�X�{�L���s�q�����D
	// ��e�@�k�� mob pos_dead �����p�U, �������_���� hp Ĳ�o
	// �[�J hit <= 0 �ɤ]�| return ���P�w 2022/12/30
	if( IS_NPC(victim) && ( victim->position == POS_DEAD || victim->hit <= 0 ) )
	{
		// �p�G mob �ؼЦ��`, �h�����u�� 2023/01/04
		if( isShootCost )
		{
			if( arrow )
				ShotObjCount( ch , arrow );
			if( ammo )
				ShotObjCount( ch , ammo );
		}
		return dam;
	}

	// �B�z �u�� ammo �����[�k�N 2022/05/01
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

    //�B�z artifact
    if ( !ch->deleted && !victim->deleted && ch->in_room == victim->in_room )
    {
		/*�Z���� ��� �������B�z
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

		// frenzy �o�ƭ��C 1/3 , �����B�~ artifact �[�� add at 2020/10/20
		if(dt == gsn_frenzy) chance += 25;

		//��� �I�� �� ¶�� ���� artifact ���\���v���[��
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


		// �� LanceMaster ���[��
		/*int nLmBonus = GetBonusedValue_LanceMaster(ch, 100);
		nCurrWis = nCurrWis * nLmBonus / 100;
		chance += (nLmBonus - 100) / 100; */
		int nLmBonus = 0;
		nCurrWis = nCurrWis * GetBonusedValue_LanceMaster(ch, 100) / 100;
		nLmBonus = (GetBonusedValue_LanceMaster(ch, 100) - 100) / 100;

		// �� TrickyPacing ���[��
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


		// �B�z �b�� �� artifact
		if( arrow )
		{
			// �W�[ obj_hit ���P�w 2022/10/23
			// �W�[ ch ���Q force �~�|���� �קK�L���s�� 2022/11/02
			if ( arrow->pIndexData->progtypes & OBJ_HIT && !ch->forced )
				oprog_hit_trigger(arrow);

			// tricky pacing �B�~�� bonus 2022/12/03
			if( !IS_NPC(ch) )
			{
				chance += nTpBonus;
			}
			
			OBJ_HP * ohp = arrow->hp;
			for( ; ohp; ohp = ohp->next ) {
				if ( IS_SET( ohp->event, HP_ARROW) ) { // HP_ARROW ���g���R��Ĳ�o
					if (artifact(ch, nCurrWis, ohp->percent, chance) > number_percent()) {
						trigger_obj_hp(arrow, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
					}
				}
			}
		}
		// �B�z �j�u �� artifact
		else if( ammo )
		{
			// �W�[ obj_hit ���P�w 2022/10/23
			// �W�[ ch ���Q force �~�|���� �קK�L���s�� 2022/11/02
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
		// �B�z �Z�� �� artifact
		//else if ( wield && ( wield->hp || wield->material->hp) )
		else if ( wield ) // �W�[ obj_hit �P�w , �� wield �P hp �P�w���}�g 2022/10/23
		{
			// �W�[ ch ���Q force �~�|���� �קK�L���s�� 2022/11/02
			if ( wield->pIndexData->progtypes & OBJ_HIT && !ch->forced )
				oprog_hit_trigger(wield);

			// lance mastery �� tricky pacing �B�~�� bonus 2022/12/03
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
			
				//Weapon Master ���Ź�Z���԰�Ĳ�o�� hp ���ť[��	
				int WmArtLv = 0;
				WmArtLv += get_skill_level(ch,gsn_weapon_master);
				if (get_skill_level(ch,gsn_weapon_master) > 3) WmArtLv += 2;
				if (get_skill_level(ch,gsn_weapon_master) > 4) WmArtLv += 3;
				if (get_skill_level(ch,gsn_weapon_master) > 5) WmArtLv += 4;

				//�Z�������� artifact 
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

				//�Z�����誺 artifact
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

		// �S��Z��,�B�z�Ĥ@��M�� artifact
		gloves = get_eq_char( ch, WEAR_HANDS );
		wield = get_eq_char( ch, WEAR_WIELD_2 );
		// �W�[ obj_hit ���P�w 2022/10/23
		// �W�[ ch ���Q force �~�|���� �קK�L���s�� 2022/11/02
		if ( gloves && gloves->pIndexData->progtypes & OBJ_HIT && !ch->forced )
			oprog_hit_trigger(gloves);

		if( gloves && (gloves->hp || gloves->material->hp)
			&& !(get_eq_char( ch, WEAR_WIELD ) || get_eq_char( ch, WEAR_TWO_HAND )) )
		{
			if( !wield || (wield && wield->value[3] != 0) )
			{
				if(dt == 1000 || dt == gsn_frenzy || dt == gsn_circular || dt == gsn_bashhead) // �[�Jbashhead 2020/11/01
				{
					OBJ_HP * ohp = gloves->hp;
					// ��M�@�[�ӷl
					if(number_bits(5) < 8) waste_dura(gloves);

					//��M������ hp 
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
					//��M���誺 hp
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
		// �S��Z��,�B�z�ĤG��M�� artifact (beholder�M��)
		gloves = get_eq_char( ch, WEAR_HANDS_2 );
		// �W�[ obj_hit ���P�w 2022/10/23
		// �W�[ ch ���Q force �~�|���� �קK�L���s�� 2022/11/02
		if ( gloves && gloves->pIndexData->progtypes & OBJ_HIT && !ch->forced )
			oprog_hit_trigger(gloves);

		if( gloves && (gloves->hp || gloves->material->hp)
			&& (dt == 1000 || dt == gsn_frenzy || (dt == gsn_bashhead && !get_eq_char( ch, WEAR_HANDS ))) ) // bashhead �Ƥ�M�P�w 2020/11/01
		{
			OBJ_HP * ohp = gloves->hp;
			// ��M�@�[�ӷl
			if(number_bits(5) < 8) waste_dura(gloves);

			//��M������ hp 
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
			//��M���誺 hp
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

	/* bashhead stun �P�w  2020/11/01 */

	// �ˮ`�j�� 0 �B �ؼФ��� stun �� �b�� ���A (�קK�s�� stun)
	if ( dam > 0 && victim->position > POS_STUNNED && ( dt == gsn_bashhead ) ){
		int BashStun = 0;
		// berserk ���A�U�B�~�[��
		if( is_affected(ch, gsn_berserk ) && get_skill_level(ch, gsn_berserk) > 2 )
			BashStun += get_skill_level(ch, gsn_berserk) - 2;
		// powerup ���A�U�B�~�[��
		if( is_affected(ch, gsn_powerup) && get_skill_level(ch, gsn_powerup) > 2 )
			BashStun += get_skill_level(ch, gsn_powerup) - 2;
		// �{��i�����A�U str �� �h���v����
		if( IS_SET( ch->act, PLR_FIST_FIGHT) )
			BashStun += get_curr_str( ch ) - get_curr_con( victim ) / (1 + get_skill_level(ch, gsn_bashhead));
		else
			BashStun += (get_curr_str( ch ) - 3) - get_curr_con( victim ) / (1 + get_skill_level(ch, gsn_bashhead));
		if ( BashStun > number_percent() * weakness_percent(victim, gsn_bashhead) / 100 ){
			victim->position = POS_STUNNED;
			WAIT_STATE( victim, skill_table[gsn_bashhead].beats );
			send_to_char( "�A��L���Y���r���A��L���w�F�I\n\r", ch );
			act( "$n �� $N �Y���r����L���w�F�I", ch, NULL, victim, TO_ROOM );
			act( "$n �§A�Y�������r���A���o�A�ѱۦa��I", ch, NULL, victim, TO_VICT );
		}
	}

	// damage ���|�Ψ� arrow �� ammo �G�̫�~�p��ƶq���� 2023/01/04
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
					//  �קאּ��stop_fighting
					//  �]��stop_fighting�|�Nch��cast�h��
					ch->fighting = NULL;
					stop_fighting( protector, FALSE );
					set_fighting( ch, protector );
					set_fighting( protector, ch );
					act( "$n�Ĥ��U���a�O�@$N�I", protector, NULL, victim, TO_ROOM );
					act( "�A�Ĥ��U���a�O�@$N�I", protector, NULL, victim, TO_CHAR );
					victim = protector;
					break;
				}else{
					act( "$n�Q�n�O�@$N�A�i�����ѤF�I", protector, NULL, victim, TO_ROOM );
					act( "�A�Q�n�O�@$N�A�i�����ѤF�I", protector, NULL, victim, TO_CHAR );
				}
			}
		}
	}

	// �j��g�� ���߮�, ���� skill �� dt 2022/05/01
	if( IsUsingGunShot(ch , wpn ) ){
		return mone_hit ( ch, victim, &gundt, wpn, MSG_BRIEF, DT_CHANGE );
	}else if( dt == TYPE_HIT && is_brief( ch, victim ) ){
		return mone_hit ( ch, victim, &dt, wpn, MSG_BRIEF, DT_CHANGE );
	}else{
		return mone_hit ( ch, victim, &dt, wpn, MSG_NORMAL, DT_NOCHANGE );
	}
}
