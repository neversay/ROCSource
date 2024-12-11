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
//*******************************************************************
//*  PROGRAM ID  : update.c                                         *
//*  LANGUAGE    : c                                                *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940625 Razgriz                                  *
//*  PURPOSE     :         �s�W GLOBAL_OBJ_UPDATE �����ܼ�          *
//*              :         �ק� obj_update() �b oprog_rand_trigger()*
//*              :         �e��[�W obj->update_checked ���B�⦡    *
//*              :         �åB�b obj_update() ���Y���]�[�W         *
//*              :         GLOBAL_OBJ_UPDATE ���B�⦡               *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940702 Razgriz                                  *
//*  PURPOSE     :         �s�W updating_obj �����ܼ�               *
//*              :         �b obj_update() ���[�J updating_obj ��   *
//*              :         �B�⦡                                   *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 Jye                                      *
//*  PURPOSE     :         �ק�time_update����if�令else if         *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940917 Razgriz                                  *
//*  PURPOSE     :         �ק� obj_update()                        *
//*              :         �s�W obj affect �� duration �B�z         *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                  *
//*  PURPOSE     :         �ק� obj_update()                        *
//*              :         �s�W obj_extra_value ���ɮĳB�z          *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                      *
//*  PURPOSE     :         �ק�P�_souvenir�Ȩϧ���ܼ�             *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Razgriz                                  *
//*  PURPOSE     :         �ק� obj_extra_value �� t0 t1 �|��� -1  *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0940924 jye                                      *
//*  PURPOSE     :         �ק�char_update ��evacuate QMARK         *
//*              :         �@�q�ɶ�hp mp mv ���N���|�^�_            *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                      *
//*  PURPOSE     :         �s�WEXP_PER_LEVEL ppl_class�ܼ�          *
//*-----------------------------------------------------------------*
//*  MODIFY  BY  : 1050426 Jye                                      *
//*  PURPOSE     :         �ק� Bachi���|�R��                       *
//*******************************************************************
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

/*
 * Externals
 */ 
extern bool merc_down;
extern void talk_channel  args( ( CHAR_DATA *ch, char *argument, int channel, const char *verb, const char *color) );

/*
 * Globals
 */
bool delete_obj;
bool delete_char;
QMARK *qmark_list;
MOB_ACTION *mob_action_list;
bool GLOBAL_OBJ_UPDATE;    //Added by Razgriz  20050625
OBJ_DATA *updating_obj;    //Added by Razgriz  20050702

/*
 * Local functions.
 */
int hit_gain  args( ( CHAR_DATA *ch ) );
int mana_gain args( ( CHAR_DATA *ch ) );
int move_gain args( ( CHAR_DATA *ch ) );
void  mobile_update args( ( void ) );
void  weather_update  args( ( void ) );
void  char_update args( ( void ) );
void  obj_update  args( ( void ) );
void  aggr_update args( ( void ) );
void  call_out_update args( ( void ) );
void  rcall_out_update  args( ( void ) );
void  rforce_update args( ( void ) );
void  action_process_cmnd args( ( char * cmnd, CHAR_DATA *mob ) );
char  *action_next_command  args( ( char *clist ) );

/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA *ch )
{
	char buf [ MAX_STRING_LENGTH ];
	int  add_hp;
	int  add_mana;
	int  add_move;
	int  add_prac;
	int  mod;
	int  total, max;

	if( IS_NPC(ch) || !IS_SET(ch->act, PLR_TITLE))
	{
		if( IS_NPC( ch )|| IS_SET( ch->act, PLR_CHINESE )  )
		{
			sprintf( buf, "- %s", 
			c_title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
		}
		else
		{
			sprintf( buf, "the %s",
			title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
		}
		set_title( ch, buf );
	}
	if( !IS_NPC(ch) )
	{
		affect_strip(ch, gsn_morphing);
		ch->temp_race = '\0'; //NULL;
	}

	add_hp  = con_app[get_curr_con( ch )].hitp + number_range( class_table[ch->class].hp_min, class_table[ch->class].hp_max ) + race_table[ ch->race ].hp_gain;
	max = con_app[get_curr_con( ch )].hitp + class_table[ch->class].hp_max + race_table[ ch->race ].hp_gain;
	total = add_hp;
	switch(ch->class)
	{
		case MAGE:
		case WIZARD:
		case ALCHEMIST:
		case SORCERER:
		case NINJA:
			mod = ((4*get_curr_int( ch ) + 1*get_curr_wis( ch ) + 2*race_table[ch->race].mana_gain ) / 7);
			break;
		case CLERIC:
		case SAVANT:
		case BISHOP:
		case SHAMAN:
			mod = ((2*get_curr_int( ch ) + 3*get_curr_wis( ch ) + 2*race_table[ch->race].mana_gain ) / 7);
			break;
		case LORD:
			if ( ch->pcdata->prime_attr == APPLY_WIS )
				mod = ((2*get_curr_int( ch ) + 3*get_curr_wis( ch ) + 2*race_table[ch->race].mana_gain ) / 7);
			else
				mod = ((3*get_curr_int( ch ) + 2*get_curr_wis( ch ) +2*race_table[ch->race].mana_gain ) / 7);
			default:
				mod = ((3*get_curr_int( ch ) + 2*get_curr_wis( ch ) + 2*race_table[ch->race].mana_gain ) / 7);
				break;
	};

	if( class_table[ ch->class ].fMana )
	{
		add_mana = number_range( mod -4, mod);
	}
	else if ( ch->pcdata->prime_attr == APPLY_WIS || ch->pcdata->prime_attr == APPLY_INT )
	{
		mod /= 4;
		add_mana = number_range( mod - 2, mod );
		add_mana = URANGE( 0, add_mana, 4);
	}
	else
	{
		mod = 0;
		add_mana = 0;
	}
	max += mod;
	total += add_mana;
	add_move  =
	number_range( 5,( ( get_curr_con( ch ) + 4 * get_curr_dex( ch ) + 2* race_table[ch->race].move_gain)  / 3)) ;
	max +=   ( get_curr_con( ch ) + 4 * get_curr_dex( ch ) + 2*race_table[ch->race].move_gain) / 3 ;
	total += add_move;
	add_prac  = wis_app[get_curr_wis( ch )].practice;
	if( total > max -3 ) /* you are too lucky */
	{
		add_hp--;
		add_mana--;
		add_move--;
	}
	else if( total *3 < max*2 ) /* you are too unlucky */
	{
		add_hp++;
		add_mana++;
		add_move++;
		add_prac++;
	}

	// add_hp = UMAX(  1, add_hp   );
	add_mana = UMAX( 0, add_mana );
	// add_move = UMAX( 10, add_move );
	switch( ch->level )
	{
		case 31:
			add_hp = add_hp * 3/2;
			add_mana = add_mana* 3/2;
			add_move = add_move* 3/2;
			send_to_char(HIW"���ߡI�A�{�b��O��_�I�̤F�I"NOR"\n\r", ch);
			break;
		case 36:
			send_to_char(HIW"���ߡI�A�{�b�O�즳�g�窺�_�I�̤F�I"NOR"\n\r", ch);
			add_hp = add_hp *  2;
			add_mana = add_mana* 2;
			add_move = add_move *2;
			add_prac = add_prac *2;
			break;
		case 38:
		if (ch->class >= WIZARD) {
			send_to_char(
			HIW"�]���A���ޯ��[�M��A����ϥΨ����ޯ�(skdemote)�N�|\n\r"
			"�l���������ޯ�g���I��I\n\r"NOR, ch);
		}
		case 37:
		case 39:
			add_hp = add_hp *  2;
			add_mana = add_mana* 2;
			add_move = add_move *2;
			add_prac = add_prac *2;
			break;
		case 40:
			send_to_char(HIW"���ߡI�A�{�b�O��u�����i�̤F�I"NOR"\n\r", ch);
			send_to_char(HIW"�A�P��ۤv�@�����ï�O���F�������i�B�I"NOR"\n\r", ch);
			add_hp = add_hp  *5;
			add_mana = add_mana *5;
			add_move = add_move*5;
			add_prac = add_prac*5;
			ch->pp += 3;
			ch->mdr += 4;
			ch->pdr += 4;
			break;
		case 41:
			send_to_char(HIW"���ߡI�A�{�b�O��u�����^���F�I"NOR"\n\r", ch);
			add_hp *= 11;
			add_mana *= 11;
			add_move *= 11;
			add_prac *= 11;
			ch->pp += 4;
			ch->mdr += 8;
			ch->pdr += 8;
			break;
		case 42:
			send_to_char(HIW"���ߡI�A�{�b�O��ǩ_�^���F�I"NOR"\n\r", ch);
			add_hp *= 25;
			add_mana *= 25;
			add_move *= 25;
			add_prac *= 25;
			ch->pp += 6;
			ch->mdr += 12;
			ch->pdr += 12;
			break;
		default:
			add_hp = add_hp * 3/2;
			add_mana = add_mana* 3/2;
			add_move = add_move* 3/2;
			break;
	};

	add_hp   *= PPL_HP_MULTIPLYER;
	add_mana *= PPL_HP_MULTIPLYER;
	add_move *= PPL_HP_MULTIPLYER;
	ch->max_hit += add_hp;
	ch->max_mana  += add_mana;
	ch->max_move  += add_move;
	ch->practice  += add_prac;

	//    if ( !IS_NPC( ch ) )
	//  REMOVE_BIT( ch->act, PLR_BOUGHT_PET );

	sprintf( buf, "�A��o�F %d/%d �I�ͩR�O  %d/%d �I�]�k�O  %d/%d �I���ʤO  %d/%d �m���I.\n\r",
				  add_hp, ch->max_hit, add_mana, ch->max_mana, add_move, ch->max_move, add_prac, ch->practice );

	send_to_char( buf, ch );
	do_save(ch, "");

	if( ch->level > 30 || ch->level % 5 == 0 )
		do_backup(ch,"");

	sprintf( buf, "%s "HIG"�����Ŵ��ɤF�I"NOR"\n\r", ch->cname );
	act( buf, ch, NULL, NULL, TO_ROOM);
	return;
}


/*
 * Demote stuff.
 */
void demote_level( CHAR_DATA *ch )
{
    char buf [ MAX_STRING_LENGTH ];
    int  add_hp;
    int  add_mana;
    int  add_move;
    int  add_prac;
    int  mod;
    int  total, max;

    if ( ch->level == 1 )
  return;

    sprintf( buf, "the %s",
  title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
    set_title( ch, buf );

    add_hp  = con_app[get_curr_con( ch )].hitp + number_range(
        class_table[ch->class].hp_min,
        class_table[ch->class].hp_max ) 
        + race_table[ ch->race ].hp_gain;
    max = con_app[get_curr_con( ch )].hitp + 
        class_table[ch->class].hp_max 
        + race_table[ ch->race ].hp_gain;
    total = add_hp;
    mod = class_table[ ch->class ].fMana ? 
  ((3*get_curr_int( ch ) + 2 * get_curr_wis( ch ) + 2*race_table[ch->race].mana_gain ) / 7):
  0;
    max += mod;
    add_mana = class_table[ch->class].fMana ? number_range( mod -4, mod) :0;
    total += add_mana;
    add_move  =
      number_range( 5,( ( get_curr_con( ch ) + (3 * get_curr_dex( ch )) + (2* race_table[ch->race].move_gain) ) / 3) );
    max +=   ( get_curr_con( ch ) + 2 * get_curr_dex( ch ) + 2*race_table[ch->race].move_gain) / 3 ;
    total += add_move;
    add_prac  = wis_app[get_curr_wis( ch )].practice;
    if( total > max -3 ) /* you are too lucky */
    {
  add_hp--;
  add_mana--;
  add_move--;
    }
    else if( total *3 < max*2 ) /* you are too unlucky */
    {
  add_hp++;
  add_mana++;
  add_move++;
  add_prac++;
    }
    add_mana = UMAX(0, add_mana);
    switch( ch->level )
    {
  case 36:
      send_to_char(HIW"���ߡI�A�{�b�O��^���F�I"NOR"\n\r", ch);
  case 37:
  case 38:
  case 39:
      add_hp = add_hp * 2;
      add_mana = add_mana* 2;
      add_move = add_move *2;
      add_prac = add_prac *2;
      break;
  case 40:
      send_to_char(HIW"�A�P��ۤv�@�����ï�O���F�������h�B�I"NOR"\n\r", ch);
      add_hp = add_hp *  5;
      add_mana = add_mana*5;
      add_move = add_move*5;
      add_prac = add_prac*5;
      ch->pp -= 2;
      ch->mdr -= 2;
      ch->pdr -= 2;
      break;
  case 41:
      add_hp *= 11;
      add_mana *= 11;
      add_move *= 11;
      add_prac *= 11;
      ch->mdr += 4;
      ch->pdr += 4;
      break;
  case 42:
      add_hp *= 25;
      add_mana *= 25;
      add_move *= 25;
      add_prac *= 25;
      ch->pp += 3;
      ch->mdr += 4;
      ch->pdr += 4;
      break;
  default:
      break;
    };

    ch->max_hit -= add_hp;
    ch->max_mana  -= add_mana;
    ch->max_move  -= add_move;
    ch->practice  -= add_prac;

//    if ( !IS_NPC( ch ) )
//  REMOVE_BIT( ch->act, PLR_BOUGHT_PET );

    ch->level -= 1;
    sprintf( buf,
//      "Your loss is: %d/%d hp, %d/%d mp, %d/%d mv %d/%d prac.\n\r",
      "�A���h�F %d/%d �I�ͩR�O  %d/%d �I�]�k�O  %d/%d �I���ʤO  %d/%d �m���I.\n\r",
      add_hp, ch->max_hit,
      add_mana, ch->max_mana,
      add_move, ch->max_move,
      add_prac, ch->practice );
    send_to_char( buf, ch );
    return;
}   



void gain_exp( CHAR_DATA *ch, int gain )
{
    int clanexp;

    if ( IS_NPC( ch ) || ch->level >= L_APP )
        return;
 //   if ( ch->race == gnome && gain > 0 )
   //     gain = gain*6/5;
 
    if ( ch->clan )
    {
        clanexp = gain / ( 7 - ch->pcdata->clanlevel );
        gain -= clanexp;
        if ( ch->clan->bank > clanexp && clanexp>0 )
        {
            ch->clan->score += ( clanexp / 10 );
            ch->clan->bank -= UMAX( 0, clanexp * 2 );
            ch->pcdata->clanexp += clanexp;
        }
    }

/*    if (ch->exp< EXP_PER_LEVEL(ch->level-1) * ch->level )
    {
        if( gain > 0)
            ch->exp = ch->exp + gain;
        return;
    } */
    //ch->exp = UMAX( EXP_PER_LEVEL( ch->level - 1 ) * ch->level, ch->exp + gain );
    ch->exp = UMAX( 0 , ch->exp + gain );
    //20060722 modify by jye
	//�s�WEXP_PER_LEVEL ppl_class�ܼ�
    while ( ch->level < L_APP && ch->exp >= EXP_PER_LEVEL( ch->level,ch->class ) * ( ch->level + 1 ) )
    {
        if( ch->level == 39 ) {
            if( !is_qmark(ch, "enable_level_40")){
                send_to_char("�A���g�����M�״I�A���O���m���M�����C\n\r", ch);
                break;
            }
        }
        if( ch->level == 40 ) {
            if( !is_qmark(ch, "enable_level_41")){
                send_to_char("�A���g�����M�״I�A���O���m���M�����C\n\r", ch);
                break;
            }
        }
        if( ch->level == 41 ) {
            if( !is_qmark(ch, "enable_level_42")){
                send_to_char("�A���g�����M�״I�A���O���m���M�����C\n\r", ch);
                break;
            }
        }
        if( ch->level == 42 ) 
            break;
        send_to_char(HIW"�A�����Ŵ��@�F�I�I "NOR, ch);
        ch->level += 1;
        advance_level( ch );
        if ( ch->class < 4 && ch->level == 30)
        {
          send_to_char("���F���A���ȳ~��[�����A�ѯ����ɤF�A���ޯ൥�šC\n\r",ch);
          send_to_char("�A�o�{�ۤv��ޯ઺�ϥί�O�ܱo��j�F�C\n\r",ch);
          send_to_char("���O�Ъ`�N�A��A��¾��A�ѯ��N�|���^�o���u�f�C\n\r",ch);          
        }
        if ( ch->level % 5 == 0 )
            do_backup( ch, "" );
    }

    if( ch->exp <= 0 ) {
        send_to_char(HIR"�A���g��w�g�κɡA�çO�F�C"NOR"\n\r", ch);
        if( !IS_NPC(ch)){
            ch->pcdata->denied = time(NULL) + 8640000;
            SET_BIT( ch->act, PLR_DENY );
            bugf("%s's exp < 0", ch->name);
            do_quit(ch,"");
        }
        return;
    }

    return;
}

void NightGeneration(CHAR_DATA *ch)
{
	int nGain = 0;
	if(!IS_NPC(ch) && IS_SET(race_table[ ch->race ].race_abilities, RACE_INFRAVISION) ){
	if ( time_info.hour >= 6 && time_info.hour <= 19 ){
		}else{
			int nSklv = get_skill_level(ch, gsn_inborn_ability);
			// 0 1 3 6 10 15 21
			nGain = (nSklv + 4) * nSklv / 2;
			// no healing affect  2018.11.20
			if ( !IS_AFFECTED(ch, AFF_NO_HEAL ) )
				ch->hit  += nGain * get_curr_con(ch) / 10;
			// gargoyle �� stone form ���|�^ mana 2022/12/05
			if ( is_affected(ch, gsn_stone_form) && affected_value( ch, gsn_stone_form, APPLY_RACE ) == -16 && ch->race == race_lookup( "Statue" ) )
				ch->mana += 0;
			else
				ch->mana += nGain * get_curr_int(ch) / 15;
			ch->move += nGain * get_curr_dex(ch) / 10;
			if(nGain > 0) send_to_char(HIU"��]�W�j�F�A���A�ͯ�O�I"NOR"\n\r", ch);
		}
	}
}

/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
	int gain, nDrink, nFood;
	int nDrunk; // �[�J �K�s�� ���ƭ� 2023/01/13

	nDrunk = 0;

	if ( IS_NPC( ch ) )
	{
		gain = ch->max_hit/50 + ch->level * 4;
	}
	else
	{
		gain =   UMAX( 10, ch->level )
			   + UMAX((get_curr_con( ch ) - 10),0) * 5 
			   + get_skill_percent(ch, gsn_body_building) / 11
			   + 4 * get_skill_level(ch, gsn_body_building) * (4 + get_skill_level(ch, gsn_body_building));

		gain += ch->max_hit / 300;

		nFood  = 40; 
		if ( race_table[ch->race].hunger_mod >= 0 ) {
			nFood  = ch->pcdata->condition[COND_FULL];
			if(nFood > 40) nFood = 40;
		}
		nDrink = 40;
		if ( race_table[ch->race].thirst_mod >= 0 ) {
			nDrink = ch->pcdata->condition[COND_THIRST];
			if(nDrink > 40) nDrink = 40;
		}

		// �N�J ch ���K�s�� 2023/01/13
		nDrunk = ch->pcdata->condition[COND_DRUNK];

		//gain = gain * (20 + nFood + nDrink) / 100;
		gain = gain * (nFood * nDrink) / 900;

		switch ( ch->position )
		{
			case POS_SLEEPING:
				gain = gain * 2.5;
				break;
			case POS_RESTING:
				gain = gain * 2;
				break;
		}

		// ���׾j���ȤӧC�ɤ��|�^���� hp 2023/01/13
		if( nFood * nDrink > 0 )
			gain += gain * (4 + race_table[ ch->race ].hp_gain) / 7;
	}

	gain *= PPL_HP_MULTIPLYER;

	// �����|���r���رڤ����v�T 2021/11/02
	// aff_ghoul ������r�v�T 2022/01/16
	if ( IS_AFFECTED( ch, AFF_POISON )
		&& !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
		&& !IS_SET( ch->special, NO_POISON )
		&& !IS_AFFECTED( ch, AFF_GHOUL ) )
	{
		// ���r���A�U , �r�����t�Ȯɥu���C 20% 2021/11/04
		if( NEW_GET_SVS(ch,MAGIC_POISON) <= 0 )
			gain = gain * 4/5;
		else
			gain = 0;
	}

	// �K�s�Ȥj�� 70 ���s�뤤�r, ���|��_, 50 ����K�įq���C�@�b 2023/01/13
	if( nDrunk >= 70 )
	{
		gain = 0;
	}
	else if( nDrunk >= 50 )
	{
		gain = gain/2;
	}

	// no healing affect add at 2018.11.20
	if ( IS_AFFECTED( ch, AFF_NO_HEAL ) ) gain = 0;

	return gain;
}

int mana_gain( CHAR_DATA *ch )
{
	int gain, nDrink, nFood;
	int nDrunk; // �[�J �K�s�� ���ƭ� 2023/01/13

	nDrunk = 0;

	if ( IS_NPC( ch ) )
	{
		gain = ch->max_mana/100 + ch->level * 4;
	}
	else
	{
		gain =   UMAX( 10 , ch->level/2 )
			   + UMAX((get_curr_int( ch ) - 10),0)
			   + UMAX((get_curr_wis( ch ) - 10),0)  // add at 200128
			   + get_skill_percent(ch, gsn_mana_condense) / 11
			   + 3 * get_skill_level(ch, gsn_mana_condense) * (4 + get_skill_level(ch, gsn_mana_condense));

		gain += ch->max_mana / 100; // 10%

		nFood  = 40; 
		if ( race_table[ch->race].hunger_mod >= 0 ) {
			nFood  = ch->pcdata->condition[COND_FULL];
			if(nFood > 40) nFood = 40;
		}
		nDrink = 40;
		if ( race_table[ch->race].thirst_mod >= 0 ) {
			nDrink = ch->pcdata->condition[COND_THIRST];
			if(nDrink > 40) nDrink = 40;
		}

		// �N�J ch ���K�s�� 2023/01/13
		nDrunk = ch->pcdata->condition[COND_DRUNK];

		//gain = gain * (20 + nFood + nDrink) / 100;
		gain = gain * (nFood * nDrink) / 900;

		switch ( ch->position )
		{
			case POS_SLEEPING:
				gain = gain * 2.5;
				break;

			case POS_RESTING:
				gain = gain * 2;
				break;
		}

		// ���׾j���ȤӧC�ɤ��|�^���� mana 2023/01/13
		if( nFood * nDrink > 0 )
			gain += gain * (4 + race_table[ ch->race ].mana_gain) / 7;

		// �ק� Cond drunk ������ mana ��_, �ѩT�w�ȭקאּ�ʤ���[�� 2023/01/13
		//gain += ch->pcdata->condition[COND_DRUNK]*3;  // max = 48 * 3 * 2 = 288
		gain = gain + gain * nDrunk/200;
	}

	// �����|���r���رڤ����v�T 2021/11/02
	// aff_ghoul ������r�v�T 2022/01/16
	if ( IS_AFFECTED( ch, AFF_POISON )
		&& !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
		&& !IS_SET( ch->special, NO_POISON )
		&& !IS_AFFECTED( ch, AFF_GHOUL ) )
	{
		// ���r���A�U , �r�����t�Ȯɥu���C 20% 2021/11/04
		if( NEW_GET_SVS(ch,MAGIC_POISON) <= 0 )
			gain = gain * 4/5;
		else
			gain = 0;
	}

	// �l�ꪫ�L�k�^ mana
	if ( IsSorGateMobByChar(ch) ) gain = 0;

	if( !IS_NPC(ch) && ( is_affected(ch, gsn_meditate)) )
	{
		if( ch->pcdata->prime_attr == APPLY_INT ){
			gain = (   gain * (get_skill_percent(ch, gsn_meditate) * 3 
					+  get_skill_level(ch, gsn_meditate) * 20 
					+ (get_skill_level(ch, gsn_meditate) / 2) * 10) ) / 120;
		}else if( ch->pcdata->prime_attr == APPLY_WIS ){
			gain = (   gain * (get_skill_percent(ch, gsn_meditate) * 3
					+  get_skill_level(ch, gsn_meditate) * 20
					+ (get_skill_level(ch, gsn_meditate) / 2) * 10 )) / 150;
		}else{
			gain = (   gain * (get_skill_percent(ch, gsn_meditate) * 3
					+  get_skill_level(ch, gsn_meditate) * 20
					+ (get_skill_level(ch, gsn_meditate) / 2) * 10 )) / 200;
		}
		gain += get_curr_int( ch ) * 3 + get_skill_level(ch, gsn_meditate) * 2; 
	}

	//gain *= PPL_HP_MULTIPLYER * PPL_HP_MULTIPLYER;
	gain *= PPL_HP_MULTIPLYER;

	return gain;
}



int move_gain( CHAR_DATA *ch )
{
	int gain, nDrink, nFood;
	int nDrunk; // �[�J �K�s�� ���ƭ� 2023/01/13

	nDrunk = 0;

	if ( IS_NPC( ch ) )
	{
		gain = ch->max_move/100 + ch->level * 4;
	}
	else
	{
		gain =   UMAX( 10, ch->level )
			   + UMAX((get_curr_con( ch ) - 10), 0) * 5 
			   + get_skill_percent(ch, gsn_body_building) / 11
			   + 4 * get_skill_level(ch, gsn_body_building) * (4 + get_skill_level(ch, gsn_body_building));

		gain += ch->max_move / 100;

		nFood  = 40; 
		if ( race_table[ch->race].hunger_mod >= 0 ) {
			nFood  = ch->pcdata->condition[COND_FULL];
			if(nFood > 40) nFood = 40;
		}
		nDrink = 40;
		if ( race_table[ch->race].thirst_mod >= 0 ) {
			nDrink = ch->pcdata->condition[COND_THIRST];
			if(nDrink > 40) nDrink = 40;
		}

		// �N�J ch ���K�s�� 2023/01/13
		nDrunk = ch->pcdata->condition[COND_DRUNK];

		//gain = gain * (20 + nFood + nDrink) / 100;
		gain = gain * (nFood * nDrink) / 900;

		switch ( ch->position )
		{
			case POS_SLEEPING: 
				gain = gain * 3 + 100; 
				break;

			case POS_RESTING:  
				gain = gain * 2 + 50;
				break;
		}

		gain += gain * (4 + race_table[ ch->race ].move_gain) / 7;
	}

	//gain *= PPL_HP_MULTIPLYER * PPL_HP_MULTIPLYER;
	gain *= PPL_HP_MULTIPLYER;

	gain += 22;

	// �����|���r���رڤ����v�T 2021/11/02
	// aff_ghoul ������r�v�T 2022/01/16
	if ( IS_AFFECTED( ch, AFF_POISON )
		&& !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
		&& !IS_SET( ch->special, NO_POISON )
		&& !IS_AFFECTED( ch, AFF_GHOUL ) )
	{
		// ���r���A�U , �r�����t�Ȯɥu���C 20% 2021/11/04
		if( NEW_GET_SVS(ch,MAGIC_POISON) <= 0 )
			gain = gain * 4/5;
		else
			gain = 0;
	}

	// �K�s�Ȥj�� 70 ���s�뤤�r, ���|��_, 50 ����K�įq���C�@�b 2023/01/13
	if( nDrunk >= 70 )
	{
		gain = 0;
	}
	else if( nDrunk >= 50 )
	{
		gain = gain/2;
	}

	//return UMIN( gain, ch->max_move - ch->move );
	return gain;
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
   /*   2023/01/13
	*   1. �� gain_condition �A�����פ��|�j���رڼƭ��ܧ�
	*   2. �ק� Condition ���̤j�� , �q 48 �ܬ� 100
	*   3. �ק� Cond �ԭz, �å[�J�s�뤤�r�����p
	*   4. �N act_obj �� eat �� drink ���ԭz���J gain_condition �ѨM���бԭz�����D
	*/

	int condition;

	if ( value == 0 || IS_NPC( ch ) ) return;

	// ���|�j���ر� 2023/01/13
	if( iCond == COND_FULL && race_table[ch->race].hunger_mod < 0 )
		return;
	if( iCond == COND_THIRST && race_table[ch->race].thirst_mod < 0 )
		return;

	condition = ch->pcdata->condition[ iCond ];

	// �w�g�O�̤j�Ȫ����p 2023/01/13
	if( value > 0 && condition >= 100 )
		return;

	// �ק�̤j�� 2023/01/13
	ch->pcdata->condition[iCond]  = URANGE( 0, condition + value, 100 /*48*/ );

	if ( ch->level < LEVEL_HERO || ( ch->level < L_APP && IS_SET(ch->act, PLR_NOTIFY)))
	{
		/*  �ק�ԭz 2023/01/13
			if ( ch->pcdata->condition[iCond] == 0 )
				switch ( iCond )
				{
					case COND_FULL:
						send_to_char( "�A���{�l�B�P�B�P�a�s.\n\r", ch );
						break;
		  
					case COND_THIRST:
						send_to_char( "�A�f������.\n\r", ch );
						break;
				}
			else if ( iCond == COND_DRUNK && ch->pcdata->condition[ iCond ] > 10 )
				send_to_char( "�A�K�F.\n\r", ch );*/

		// �s�뤤�r���|��ܾj���r��
		if ( ch->pcdata->condition[ COND_DRUNK ] < 70 )
		{
			// cond ���t�ȥN���|�j���
			if ( ch->pcdata->condition[iCond] >= 0 )
			{
				// �t�Ȫ����p�U��ܬO�_�j��
				if ( value < 0 )
				{
					if ( iCond == COND_FULL )
					{
						if ( ch->pcdata->condition[iCond] < 10 )
							send_to_char( "�A�j������L�O.\n\r", ch );
						else if ( ch->pcdata->condition[iCond] < 20 )
							send_to_char( "�A���{�l�B�P�B�P�a�s.\n\r", ch );
						else if ( ch->pcdata->condition[iCond] < 40 )
							send_to_char( "�A���I�j�F.\n\r", ch );
					}
					if ( iCond == COND_THIRST )
					{
						if ( ch->pcdata->condition[iCond] < 10 )
							send_to_char( "�A����F.\n\r", ch );
						else if ( ch->pcdata->condition[iCond] < 20 )
							send_to_char( "�A�f������.\n\r", ch );
						else if ( ch->pcdata->condition[iCond] < 40 )
							send_to_char( "�A���I���F.\n\r", ch );
					}
				}
				else  // �p�G value �����ȫh��ܬO�_�Y���ܨ�( ��act_obj �����J ) 2023/01/13
				{
					if ( iCond == COND_FULL )
					{
						// �쥻���O�̤j�� �ӥ[�ƭȫᬰ�̤j�� �h�]�o��
						if ( condition < 100 && ch->pcdata->condition[iCond] == 100 ){
							send_to_char( "�A�Y���F.\n\r", ch );
						}else{
							// �P�_���A�C���C�Ӷ��q ��ܤ��P�T��
							if ( condition < 10 ){
								if( ch->pcdata->condition[iCond] >= 10 && ch->pcdata->condition[iCond] < 40 )
									send_to_char( "�A���A�P��پj.\n\r", ch );
								if( ch->pcdata->condition[iCond] >= 40 && ch->pcdata->condition[iCond] < 100 )
									send_to_char( "�A���F.\n\r", ch );
								if( ch->pcdata->condition[iCond] == 100 )
									send_to_char( "�A�Y���F.\n\r", ch );
							}
							if ( condition < 40 && condition >= 10 ){
								if( ch->pcdata->condition[iCond] >= 40 && ch->pcdata->condition[iCond] < 100 )
									send_to_char( "�A���F.\n\r", ch );
								if( ch->pcdata->condition[iCond] == 100 )
									send_to_char( "�A�Y���F.\n\r", ch );
							}
						}
					}
					if ( iCond == COND_THIRST )
					{
						if ( condition < 100 && ch->pcdata->condition[iCond] == 100 ){
							send_to_char( "�A�ܨ��F.\n\r", ch );
						}else{
							if ( condition < 10 ){
								if( ch->pcdata->condition[iCond] >= 10 && ch->pcdata->condition[iCond] < 40 )
									send_to_char( "�A�����F����g��.\n\r", ch );
								if( ch->pcdata->condition[iCond] >= 40 && ch->pcdata->condition[iCond] < 100 )
									send_to_char( "�A���A�P��f��.\n\r", ch );
								if( ch->pcdata->condition[iCond] == 100 )
									send_to_char( "�A�ܨ��F.\n\r", ch );
							}
							if ( condition < 40 && condition >= 10 ){
								if( ch->pcdata->condition[iCond] >= 40 && ch->pcdata->condition[iCond] < 100 )
									send_to_char( "�A���A�P��f��.\n\r", ch );
								if( ch->pcdata->condition[iCond] == 100 )
									send_to_char( "�A�ܨ��F.\n\r", ch );
							}
						}
					}
				}
			}
		}

		// �s��ԭz 2023/01/13
		if ( iCond == COND_DRUNK )
		{
			// �s�뤤�r 2023/01/13
			if ( ch->pcdata->condition[ iCond ] >= 70 )
			{
				gain_condition( ch, COND_FULL, - number_range(15, 30) );
				gain_condition( ch, COND_THIRST, - number_range(15, 30) );
				send_to_char( "�A�@�}�ϭG�A�æR����I\n\r", ch );
				act( "$n ��M�y��o�աA�æR����I", ch, NULL, NULL, TO_ROOM );
			}
			else if ( ch->pcdata->condition[ iCond ] >= 60 )
				send_to_char( "�s�����A���ǤϭG.\n\r", ch );
			else if ( ch->pcdata->condition[ iCond ] >= 50 )
				send_to_char( "�A��K�p�d.\n\r", ch );
			else if ( ch->pcdata->condition[ iCond ] >= 40 )
				send_to_char( "�A�K�o����í�F.\n\r", ch );
			else if ( ch->pcdata->condition[ iCond ] >= 20 )
				send_to_char( "�A�K�F.\n\r", ch );
			else if ( ch->pcdata->condition[ iCond ] >= 10 )
				send_to_char( "�A���ǾK�F.\n\r", ch );
			else if ( ch->pcdata->condition[ iCond ] > 0 )
				send_to_char( "�A���I�K�N.\n\r", ch );
		}
	}

	return;
}



/*
 * Mob autonomous action.
 * This function takes 25% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
	CHAR_DATA *ch;
	EXIT_DATA *pexit;
	int        door;

	/* Examine all mobs. */
	for ( ch = char_list; ch; ch = ch->next )
	{
		int rnum;

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;


		if ( !IS_NPC( ch ) || !ch->in_room || IS_AFFECTED( ch, AFF_CHARM ) )
			continue;


		//  if( ch->wait && !ch->fighting ) ch->wait = 0;

		/* Examine call for special procedure */
		if ( ch->spec_fun && !IS_AFFECTED(ch, AFF_PARALYZED))
		{
			if ( ( *ch->spec_fun ) ( ch ) )
				continue;
		}

		/* That's all for sleeping / busy monster */
		if ( ch->position < POS_STANDING )
			continue;

		/* MOBprogram random trigger */
		if ( ch->in_room->area->nplayer > 0 )
		{
			mprog_random_trigger( ch );
			/* If ch dies or changes
			position due to it's random
			trigger, continue - Kahn */
			if ( ch->position < POS_STANDING )
				continue;
		}

		/* Scavenge */
		if (   IS_SET( ch->act, ACT_SCAVENGER )
			&& ch->in_room->contents
			&& number_bits( 2 ) == 0 )
		{
			OBJ_DATA *obj;
			OBJ_DATA *obj_best;
			int       max;

			max   = 1;
			obj_best  = 0;
			for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
			{
				if (   CAN_WEAR( obj, ITEM_TAKE )
					&& obj->cost > max
					&& can_see_obj( ch, obj ) )
				{
					obj_best  = obj;
					max   = obj->cost;
				}
			}

			if ( obj_best )
			{
				char picbuf[MAX_INPUT_LENGTH];
				sprintf(picbuf,"'%s'", obj_best->name);
				//obj_from_room( obj_best );
				//obj_to_char( obj_best, ch );
				//act( "$n �߰_�F $p.", ch, obj_best, NULL, TO_ROOM );
				do_get(ch, picbuf);
			}
		}

		/* Wander or Flee  - modifications by Jason Dinkel */
		if ( ch->hit < ch->max_hit / 2 )
			rnum = 3;
		else
			rnum = 5;

		if (     !IS_SET( ch->act, ACT_SENTINEL )
			&& ( door = number_bits( rnum ) ) <= 5
			&& ( pexit = ch->in_room->exit[door] )
			&&   pexit->to_room
			&&   !IS_SET( pexit->exit_info, EX_CLOSED )
			&&   !IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB )
			&& ( !IS_SET( ch->act, ACT_STAY_AREA ) || pexit->to_room->area == ch->in_room->area ) )
		{
			/* Give message if hurt */
			if ( rnum == 3 )
				act( "$n �W�i�a�k�]�I", ch, NULL, NULL, TO_ROOM );

			move_char( ch, door );
			/* If ch changes position due
			to it's or someother mob's
			movement via MOBProgs,
			continue - Kahn */
			if ( ch->position < POS_STANDING )
				continue;

			/* If people are in the room, then flee. */
			if ( rnum == 3 )
			{
				CHAR_DATA *rch;

				for ( rch = pexit->to_room->people; rch; rch = rch->next_in_room )
				{
					if ( rch->deleted )
						continue;
					if ( !IS_NPC( rch ) )
					{
						int direction;

						door = -1;
						act( "$n �W�i�a�k�]�I", ch, NULL, NULL, TO_ROOM );

						/* Find an exit giving each one an equal chance */
						for ( direction = 0; direction <= 5; direction++ )
						{
							if ( ch->in_room->exit[direction] && number_range( 0, direction ) == 0 )
								door = direction;
						}

						/* If no exit, attack.  Else flee! */
						if ( door == -1 )
							multi_hit( ch, rch, TYPE_UNDEFINED );
						else
							move_char( ch, door );
						break;
					}
				}
			}
		}
	}
	return;
}



/*
 * Update the weather.
 */
void weather_update( void )
{
	DESCRIPTOR_DATA *d, *d_next;
	char       buf [ MAX_STRING_LENGTH ];
	int        diff;

	buf[0] = '\0';

	switch ( ++time_info.hour )
	{
		case  6:
			weather_info.sunlight = SUN_RISE;
			strcat( buf, HIY"����F�@�A���⪺������ģ�ۤj�a�C"NOR"\n\r" );
			break;

		case  7:
			weather_info.sunlight = SUN_LIGHT;
			strcat( buf, HIW"�s���@�Ѷ}�l�F�C"NOR"\n\r");
			break;

		case 19:
			weather_info.sunlight = SUN_SET;
			strcat( buf, YEL "�i����U�A�̫�@�D�������������C"NOR"\n\r");
			break;

		case 20:
			weather_info.sunlight = SUN_DARK;
			strcat( buf, HIB"�©]���{�A�]��Ţ�n�ۤj�a�C"NOR"\n\r" );
			break;

		case 24:
			time_info.hour = 0;
			time_info.day++;
			break;
	}
	mpact[time_info.hour] = 0;

	if ( time_info.day >= 35 )
	{
		time_info.day = 0;
		time_info.month++;
	}

	if ( time_info.month >= 17 )
	{
		time_info.month = 0;
		time_info.year++;
	}


	/* Weather change. */

	/* old weather change
	if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
	else
	diff = weather_info.mmhg > 1015 ? -2 : 2;
	*/
	// new weather for seasonal 2018.11.9
	if ( time_info.month <= 3 )
		diff = weather_info.mmhg > 1015 ? -3 : 2;
	else if ( time_info.month <= 8 )
		diff = weather_info.mmhg > 985 ? -2 : 2;
	else if ( time_info.month <= 12 )
		diff = weather_info.mmhg > 985 ? -3 : 2;
	else
		diff = weather_info.mmhg > 1015 ? -2 : 2;

	weather_info.change += diff * dice( 1, 4 ) + dice( 2, 6 ) - dice( 2, 6 );
	weather_info.change  = UMAX( weather_info.change, -12 );
	weather_info.change  = UMIN( weather_info.change,  12 );

	weather_info.mmhg   += weather_info.change;
	weather_info.mmhg    = UMAX( weather_info.mmhg,  960 );
	weather_info.mmhg    = UMIN( weather_info.mmhg, 1040 );

	switch ( weather_info.sky )
	{
		default: 
			bug( "Weather_update: bad sky %d.", weather_info.sky );
			weather_info.sky = SKY_CLOUDLESS;
			break;

		case SKY_CLOUDLESS:
			if ( weather_info.mmhg < 990 || ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
			{
				strcat( buf, "�ѤW�����h�v���ܫp�C\n\r" );
				weather_info.sky = SKY_CLOUDY;
			}
			break;

		case SKY_CLOUDY:
			if ( weather_info.mmhg < 970 || ( weather_info.mmhg < 990 && number_bits( 2 ) == 0 ) )
			{
				//strcat( buf, HIU "�}�l�U�B�F�C"NOR"\n\r");
				//winter check add at 2018.11.06
				if (   time_info.month == 0
					|| time_info.month == 1
					|| time_info.month == 2
					|| time_info.month == 3
					|| ( time_info.month == 4 && time_info.day <= 10 && number_bits( 1 ) == 1 )
					|| ( time_info.month == 16 && time_info.day >= 20 && number_bits( 1 ) == 1 ) )
				{
					strcat( buf, HIW "�ѪŶ}�l�Ƹ�����C"NOR"\n\r");
					weather_info.sky = SKY_SNOWING;
				}else{
					strcat( buf, HIU "�ѪŶ}�l���U�p�B�C"NOR"\n\r");
					weather_info.sky = SKY_RAINING;
				}
			}
			if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
			{
				strcat( buf, "�ѤW�����������h�A�U���L���C\n\r");
				weather_info.sky = SKY_CLOUDLESS;
			}
			break;

		case SKY_RAINING:
			if ( weather_info.mmhg < 970 && number_bits( 2 ) == 0 )
			{
				strcat(buf, HIY"�@�Dģ�����{�q���L�ѻڡA���U�g�����B�C"NOR"\n\r");
				weather_info.sky = SKY_LIGHTNING;
			}
			if ( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
			{
				strcat( buf, "�B���F�C\n\r");
				weather_info.sky = SKY_CLOUDY;
			}
			break;

		case SKY_LIGHTNING:
			if ( weather_info.mmhg > 1010 || ( weather_info.mmhg > 990 && number_bits( 2 ) == 0 ) )
			{
				strcat( buf, "�ѪŤ����{�q����F�C\n\r");
				weather_info.sky = SKY_RAINING;
				//break;  remove at 2022/05/07
			}
			break;

		//add at 2018.11.06 winter snowing
		case SKY_SNOWING:
			if ( weather_info.mmhg < 970 && number_bits( 2 ) == 0 )
			{
				strcat(buf, WHT"�Ѧ��M��t�A��_�}�}�j���j���C"NOR"\n\r");
				weather_info.sky = SKY_SNOWSTORM;
			}
			if ( weather_info.mmhg > 1030 || ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
			{
				strcat( buf, "�����F�C\n\r");
				weather_info.sky = SKY_CLOUDY;
			}
			break;

		//add at 2018.11.06 winter snowstorm
		case SKY_SNOWSTORM:
			if ( weather_info.mmhg > 1010 || ( weather_info.mmhg > 990 && number_bits( 2 ) == 0 ) )
			{
				strcat( buf, "�ɭ�������F�C\n\r");
				weather_info.sky = SKY_SNOWING;
			}
			break;
		//break;  remove at 2022/05/07
	}

	if ( buf[0] != '\0' )
	{
		for ( d = descriptor_list; d; d = d->next )
		{
			if ( d->connected == CON_PLAYING && IS_OUTSIDE( d->character ) && IS_AWAKE( d->character ) )
				send_to_char( buf, d->character );
		}
	}
	for ( d = descriptor_list; d; d = d_next )
	{
		d_next = d->next;
		if ( (d->connected != CON_PLAYING) ){
			if ( --d->repeat <= -5 )
			{
				sprintf( log_buf, "%s idle too long !", d->host );
				log_string( log_buf );
				write_to_buffer( d, "���m�L�[, �����s�u���A�C\n\r", 0 );
				close_socket ( d );
			}
		}
		/*
		// ���m�s��ۦP���O�p�� 2022/12/14
		if ( d->connected == CON_PLAYING ){
			if( d->repeat > 0 )
				d->repeat = 0;
		} */
	}

	return;
}



/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{   
    CHAR_DATA *ch;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    time_t save_time;
	//ROOM_INDEX_DATA *location;
    char buf [MAX_STRING_LENGTH];
    QMARK *qmark = NULL;
    int nSklv = 0;

    ch_save = NULL;
    ch_quit = NULL;
    save_time = current_time;


	for ( ch = char_list; ch; ch = ch->next )
	{
		AFFECT_DATA *paf;

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		if( !ch->in_room ) continue;
		nSklv = get_skill_level(ch, gsn_inborn_ability);
      
		if( is_affected(ch, gsn_cont_heal) && !IS_AFFECTED( ch, AFF_NO_HEAL ) ) {
			// �H���Ū�ܨ��W continous heal ���j�� 2022/04/08
			if ( affected_value(ch, gsn_cont_heal, APPLY_LEVEL ) > 0 ){
				int nChSklv =  get_skill_level(ch,gsn_cont_heal); // ���ޯ൥�Ū̱j�׸���
				int nChAff = affected_value(ch, gsn_cont_heal, APPLY_LEVEL );
				int nHeal = 0;
				if(!IS_NPC(ch)){
					//nHeal =  PPL_HP_MULTIPLYER * PPL_HP_MULTIPLYER * (65 + nChSklv * 40 + nChSklv * nChSklv * nChSklv);
					nHeal = (ch->max_hit * ( 10 + nChAff*20 + nChSklv*5 ) )/1000; // 15~17%
				}else{
					//nHeal =  PPL_HP_MULTIPLYER * (ch->max_hit * (5 + nChSklv * 2) / 100);
					nHeal = (ch->max_hit * (3 + nChAff) )/ 200; // 5%
				}
				ch->hit = UMIN( ch->hit + nHeal, ch->max_hit );
				send_to_char("�@�ѩM�w���x�y�y�M�A����.\n\r", ch);
			}
		}

		if( is_affected(ch, gsn_stone_form) )
		{
			// �[�J gargoyle �ϥΪ� stone form �B �̤j hp �����Ȥ~�|�^�� 2022/12/05
			if( affected_value( ch, gsn_stone_form, APPLY_RACE ) == -16
				&& ch->race == race_lookup( "Statue" )
				&& ch->max_hit >= 0
				&& !IS_AFFECTED( ch, AFF_NO_HEAL ) )
			{
				int nHeal = 0;
				if(!IS_NPC(ch)){
					//nHeal =  PPL_HP_MULTIPLYER * PPL_HP_MULTIPLYER * (65 + nSklv * 40 + nSklv * nSklv * nSklv);
					nHeal = ( ch->max_hit * ( 6 + nSklv * 4 ) ) / 100; // tick �^ 6 ~ 30%
				}else{
					//nHeal =  PPL_HP_MULTIPLYER * (ch->max_hit * (1 + nSklv * 2) / 100);
					nHeal = ( ch->max_hit * (5 + nSklv * 5) ) / 100; // tick �^ 5 ~ 35%
				}
				ch->hit = UMIN( ch->hit + nHeal, ch->max_hit );
				send_to_char("�ۤƪ��ĪG�[�t�F�A���A�ͯ�O.\n\r", ch);
				ch->mana -= 50 + nSklv * 30;  // �[�J tick �� mana �ί} stone form 2022/12/05
				if(ch->mana < 0){
					ch->mana = 0;
					affect_strip(ch, gsn_stone_form);
					send_to_char("�A���骺�ۤƪ��A���������F.\n\r"NOR, ch);
					act( "$n"CYN"���U�F�J���몺�~�ߡA�q�a�����ͭ�����Ť��C"NOR, ch, NULL, NULL, TO_NOTVICT );
				}
			}
		}

		// if ( IS_NPC( ch ) && ch->position > POS_FIGHTING )
		// if ( IS_NPC( ch ) && ( ch->position > POS_FIGHTING || is_qmark(ch, "enable_fight_tick")))

		/*
         *   ���l�� mob �L�����A��@��tick prog
         *   �� sleep �� rest ���A�� mob �b�� qmark �U��@�� tick prog
		 */
		if( IsSorGateMobByChar(ch)
			&& (   ch->position == POS_STANDING 
				|| ch->position == POS_RESTING
				|| ch->position == POS_SLEEPING
				|| ch->position == POS_FIGHTING
				|| ch->position == POS_STUNNED
				|| ch->position == POS_PARALYZED))
		{
			mprog_tick_trigger( ch );
			if ( ch->position < POS_STANDING || ch->deleted )
			continue;
		}else if( IS_NPC( ch )
				&& !IS_AFFECTED( ch, AFF_SLEEP )
				&& !IS_AFFECTED( ch, AFF_CHARM )
				&& (    ch->position == POS_STANDING
					|| (ch->position == POS_RESTING && is_qmark(ch, "enable_rest_tick"))
					|| (ch->position == POS_SLEEPING && is_qmark(ch, "enable_sleep_tick"))
					|| (ch->position == POS_FIGHTING && is_qmark(ch, "enable_fight_tick"))))
		{
			mprog_tick_trigger( ch );
				/* If ch dies or changes
				position due to it's random'
				trigger, continue - Kahn */
			if ( ch->position < POS_STANDING || ch->deleted )
				continue;
		}

		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && ch->level > 2 && !is_qmark(ch, "roc_ppl_salary") && number_bits(5) < 3 ){
			send_to_char("�A���o�~���ơA�i�H�h���~���F�C\n\r", ch);
		}

		// ����T�{���� 2022/11/02
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && ch->level > 2 && is_qmark(ch, "death_stop_backup") && number_bits(1) < 1 ){
			send_to_char("�A�٨S��^���`�e�����~�Ϊ����C\n\r", ch);
		}

		// �s�W cd �ޯ� tick �t�δ��� add at 2020/10/16
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "evacuate_loc") ){
			if( (qmark = is_qmark( ch, "evacuate_loc" ))!= NULL ){
				if( qmark->timer == 2 )
					send_to_char("�A�]�w���ǰe�аO�Y�N���ġC\n\r", ch);
				if( qmark->timer == 1 )
					send_to_char("�A�]�w���ǰe�аO���ĤF�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "spell_mark") ){
			if( (qmark = is_qmark( ch, "spell_mark" ))!= NULL ){
				if( qmark->timer == 2 )
					send_to_char("�A�]�w���]�O�L�O�Y�N���ġC\n\r", ch);
				if( qmark->timer == 1 )
					send_to_char("�A�]�w���]�O�L�O���ĤF�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "spell_armageddon_timer") ){
			if( (qmark = is_qmark( ch, "spell_armageddon_timer" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q�I�k�l�Ӧ^�_�A�S��ϥΥ���T�G�F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "wizard_rage_timer") ){
			if( (qmark = is_qmark( ch, "wizard_rage_timer" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q�I�k�l�Ӧ^�_�A�S��ϥξԧŤѫ�F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "spell_group_full_heal") ){
			if( (qmark = is_qmark( ch, "spell_group_full_heal" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q�I�k�l�Ӧ^�_�A�S��ϥθs�駹���v¡�N�F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "spell_group_full_refresh") ){
			if( (qmark = is_qmark( ch, "spell_group_full_refresh" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q�I�k�l�Ӧ^�_�A�S��ϥθs����O���_�N�F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "spell_resurrection_timer") ){
			if( (qmark = is_qmark( ch, "spell_resurrection_timer" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�P���F���ܦ��餤�A��A�������_���N�F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "skill_defensive_mark") ){
			if( (qmark = is_qmark( ch, "skill_defensive_mark" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q�h�֤��^�_�A�S��I���@���\\��F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "skill_bloodthirsty_mark") ){
			if( (qmark = is_qmark( ch, "skill_bloodthirsty_mark" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q�z�o��^�_�A�S��I��ݦ�F�C\n\r", ch);
			}
		}
		if( !IS_NPC(ch) && IS_SET(ch->act, PLR_NOTIFY) && is_qmark(ch, "skill_reg_adore") ){
			if( (qmark = is_qmark( ch, "skill_reg_adore" ))!= NULL ){
				if( qmark->timer == 1 )
					send_to_char("�A�q��}��^�_�A�S��I��^�P��F�C\n\r", ch);
			}
		}

		//if ( ch->forced ) ch->forced = FALSE; // delete forced
		
		/*
		 *  Add by coconet for check auto state for ppl
		 */
		if ( !IS_NPC(ch)
			&& ch->pcdata->autocombat == AUTOC_INSTANT
			&& get_skill_percent(ch, gsn_instant_cast) < 1)
			ch->pcdata->autocombat = AUTOC_NORMAL;

	   /*
	    *   Find dude with oldest save time.
	    */
		if ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_HORSE )
			&& ( !ch->desc || ch->desc->connected == CON_PLAYING )
			&& ch->level >= 2
			&& ch->save_time < save_time )
		{
			ch_save = ch;
			save_time = ch->save_time;
		}

		if( ch->position >= POS_STUNNED )
		{
			if( (qmark = is_qmark( ch, "cast_evacuate_mark" ))!= NULL )
			{
				if( qmark->v0 > 1 )
					do_mod_qmark( ch, "self cast_evacuate_mark v0 - 1" );
				else
					do_del_qmark( ch, "self cast_evacuate_mark");
			}
			else
			{
				NightGeneration(ch);
				// limit NightGeneration hp , mana , mv  2021/11/02
				if(ch->hit > ch->max_hit)   ch->hit = ch->max_hit;
				if(ch->mana > ch->max_mana) ch->mana = ch->max_mana;
				if(ch->move > ch->max_move) ch->move = ch->max_move;

				if( ch->hit <= ch->max_hit )
					ch->hit = UMIN( ch->max_hit, ch->hit + hit_gain( ch ) );
				else if ( ch->hit > ch->max_hit * 2 )
					ch->hit = ch->max_hit * 2;
				else 
					ch->hit = UMAX( ch->max_hit, ch->hit - hit_gain( ch ) );

				// gargoyle �� stone form ���|�^ mana 2022/12/05
				if ( is_affected(ch, gsn_stone_form) && affected_value( ch, gsn_stone_form, APPLY_RACE ) == -16 && ch->race == race_lookup( "Statue" ) )
				{
					ch->mana += 0;
				}else{
					if( ch->mana <= ch->max_mana )
						ch->mana = UMIN( ch->max_mana, ch->mana + mana_gain( ch ) );
					else if ( ch->mana > ch->max_mana * 2 )
						ch->mana = ch->max_mana * 2;
					else
						ch->mana = UMAX( ch->max_mana, ch->mana - mana_gain( ch ) );
					if( is_affected(ch, gsn_meditate))
					{
						send_to_char( "[36m�ũ����ߴ����A���k�O�ֳt�W�[�C[m\n\r", ch );
						if( ch->mana >= ch->max_mana ) 
						{
							ch->wait = 0;
							affect_strip( ch, gsn_meditate );
						}
					}
				}

				if( ch->move <= ch->max_move )
					ch->move = UMIN( ch->max_move, ch->move + move_gain( ch ) );
				else if( ch->move > ch->max_move * 2 )
					ch->move = ch->max_move * 2;
				else
					ch->move = UMAX( ch->max_move, ch->move - move_gain( ch ) );
			}
		}

		if ( ch->position <= POS_PARALYZED )
			update_pos( ch );

		if ( ch->position == POS_DEAD )
		{
			raw_kill( ch, ch );
			continue;
		}

		// pc �C tick �W�[ stay_time , npc �������u�|�b�� qmark �����p�U�@ �̤j�Ȭ� 10000  2023/01/15
		if( ( !IS_NPC(ch) || ( IS_NPC(ch) && is_qmark(ch, "count_stay_time") ) ) && ch->stay_time < 10000 )
			ch->stay_time += 1;

		if ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_HORSE ) && ch->level < LEVEL_IMMORTAL && !IS_SWITCHED( ch ) )
		{
			OBJ_DATA *obj;

			if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) && obj->item_type == ITEM_LIGHT && obj->value[2] > 0 )
			{
				if ( --obj->value[2] == 0 )
				{
					--ch->in_room->light;
					act( "$p �N���F.", ch, obj, NULL, TO_ROOM );
					act( "$p �N���F.", ch, obj, NULL, TO_CHAR );
					extract_obj( obj );
				}
			}

			/*  2023/01/15
			*   �s�W ch->stay_time �Ω�P�w ch �O�_��P�ж��ݤӤ[
			*   �p�G char �b safe room 12 tick �� 20 tick �����ʱN�|�Q�ФW PLR_IDLE
			*   PLR_IDLE �u��� move_char ����, �Ǧ��P�_���a�O�_�u�o�b���Υu�O��ª� robot idle
			*   �ï�z��X���b�i��C�������a
			*/
			if( !IS_SET(ch->act, PLR_IDLE) && !ch->fighting )
			{
				if( ch->stay_time >= 3 && IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
					SET_BIT(ch->act, PLR_IDLE);
				else if( ch->stay_time >= 10 )
					SET_BIT(ch->act, PLR_IDLE);
			}

			if ( ++ch->timer >= 24 )
			{
				/* �����s�u���A�U�| idle quit , idle quit �ȷ|�o�ͦb�_�u�� 2023/01/15
					ch->was_in_room = ch->in_room;
					if ( ch->fighting )
						stop_fighting( ch, TRUE );

					send_to_char( "�A�v�������b��L����.\n\r", ch );
					act( "$n �v�������b��L����.", ch, NULL, NULL, TO_ROOM );

					SET_BIT(ch->act, PLR_IDLE);
					if ( IS_SET(ch->act, PLR_IDLE) ) //ch->in_room->vnum == 11001 || ch->in_room->vnum == ROOM_VNUM_LIMBO)
					{
						do_idlequit( ch, "" );
						continue;
					}
				*/

				//do_save(ch, "");

				if ( !ch->desc )
				{
					ch->was_in_room = ch->in_room;
					if ( ch->fighting )
						stop_fighting( ch, TRUE );

					send_to_char( "�A�v�������b��L����.\n\r", ch );
					act( "$n �v�������b��L����.", ch, NULL, NULL, TO_ROOM );
					sprintf( buf, "%s lost link too long, force quit!", ch->name );
					bug( buf, 0 );
					do_idlequit( ch, "" );
					continue;
				}
			}

			/* �����s�u���A�U�| idle quit , idle quit �ȷ|�o�ͦb�_�u�� 2023/01/15
				if ( ch->timer > 96 && !IS_SWITCHED( ch ) ){
					if( ch->was_in_room ) {
						char_from_room(ch);
						char_to_room(ch, ch->was_in_room);
					}
					do_idlequit(ch, ""); //ch_quit = ch;
				}
			*/

			gain_condition( ch, COND_DRUNK,  -1 );
			// �ק� Cond �̤j�ȱq 48 �� 100  2023/01/13
			if ( ch->level < 4 && ch->class < 5 )
			{
				ch->pcdata->condition[COND_FULL] = 100/*48*/;
				ch->pcdata->condition[COND_THIRST] = 100 /*48*/;
			}
			else
			{
				if(!is_affected(ch, gsn_satiety)){
					if(race_table[ch->race].hunger_mod >= 0)
						gain_condition( ch, COND_FULL,   - number_range(1, 1 + race_table[ch->race].hunger_mod));
				}
				if(race_table[ch->race].thirst_mod >= 0)
					gain_condition( ch, COND_THIRST, - number_range(1, 1 + race_table[ch->race].thirst_mod));

				// ���|�j�δ����ر� �C tick ���ȫ�_��̤j 2023/01/13
				if( race_table[ch->race].hunger_mod < 0 )
					ch->pcdata->condition[COND_FULL] = 100;
				if( race_table[ch->race].thirst_mod < 0 )
					ch->pcdata->condition[COND_THIRST] = 100;

				// �S�w tick �������|�j���رڤֶq���پj��
				if(race_table[ch->race].hunger_mod < 0 && (time_info.hour == 12 || time_info.hour == 0)){
					ch->pcdata->condition[COND_FULL] = 99/*40*/;
				}
			}
		}
		/*  horse �w�g����, �G�������q  2023/01/15
		else if ( ch->desc && IS_SET(ch->act, PLR_HORSE))
		{
			if ( ++ch->timer >= 24 )
			{
				if ( !ch->was_in_room )
				{
					ch->was_in_room = ch->in_room;
					if ( ch->fighting )
						stop_fighting( ch, TRUE );

					send_to_char( "�A�v�������b��L����.\n\r", ch );
					act( "$n �v�������b��L����.", ch, NULL, NULL, TO_ROOM );
					act( "$n �v�������b��L����.", ch->desc->original, NULL, NULL, TO_ROOM );

					SET_BIT(ch->act, PLR_IDLE);
					if( IS_SET(ch->act, PLR_IDLE) )//ch->in_room->vnum == ROOM_VNUM_LIMBO)
					{
						do_idlequit(ch, "");
						continue;
					}
				}
			}
			if ( ch->timer > 96 && !IS_SWITCHED( ch ) && ch->desc) {
				CHAR_DATA * ppl = ch->desc->original;
				do_ride(ch, "");
				do_idlequit(ppl, "");
			}
		} */

		for ( paf = ch->affected; paf; paf = paf->next )
		{
			if ( paf->deleted )
				continue;
			if ( paf->duration > 0 )
				paf->duration--;
			else if ( paf->duration < 0 )
				;
			else
			{
				if ( !paf->next
					|| paf->next->type != paf->type
					|| paf->next->duration > 0 )
				{
					if ( paf->type > 0 && skill_table[paf->type].msg_off )
					{
						send_to_char( skill_table[paf->type].msg_off, ch );
						send_to_char( "\n\r", ch );
					}
				}
				// link ���A�H�k�N�������� 2021/11/21
				if( paf->type == gsn_lifeshare ){
					if( ch->linking != NULL ){
						stop_linking( ch , ch->linking );
					}
				}
				affect_remove( ch, paf );
				if( paf->type == gsn_polymorph || paf->type == gsn_morphing )
				{
					if( !IS_NPC(ch))
					{
						ch->temp_race = '\0';
						dm_effect( ch );
					}
				}
				// �N���B fall �P�w����U�� 2023/01/14
				//if( paf->type == gsn_flying )
				//	fall_effect( ch );
			}
		}

		// link ���A�P�w���W���L�S�w affect 2021/11/23
		if( ch->linking ){
			if( !is_affected(ch, gsn_lifeshare) ){
				stop_linking( ch , ch->linking );
			}
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		// blood thirsty tick ��֤ή����P�w 2021/12/22
		if( is_affected( ch, gsn_blood_thirsty) && !ch->fighting )
		{
			int blood_sklv = get_skill_level(ch, gsn_blood_thirsty);
			if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) > 0 )
			{
				AFFECT_DATA af;
				af.type      = gsn_blood_thirsty;
				af.duration  = -1;
				af.location  = APPLY_DAMAGE_COUNT;
				af.modifier  = 0 - ( 100/(1 + blood_sklv/6) + affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT)/(2 + blood_sklv/6) );
				af.bitvector = 0;
				affect_join( ch, &af );
			}
			if( affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) <= 0 )
				affect_strip( ch, gsn_blood_thirsty );
		}

		// �s�� debuff �԰��~�C tick ���C breath �y�����ˮ`�[���� 2022/04/16
		if ( is_affected( ch, gsn_acid_breath) && !ch->fighting )
		{
			if( affected_value(ch, gsn_acid_breath, APPLY_DAMAGE_COUNT) > 0 ){
				AFFECT_DATA af;
				af.type      = gsn_acid_breath;
				af.duration  = 0;
				af.location  = APPLY_DAMAGE_COUNT;
				af.modifier  = 0 - 20 - affected_value(ch, gsn_acid_breath, APPLY_DAMAGE_COUNT)/5;
				af.bitvector = 0;
				affect_join( ch, &af );
			}
			if( affected_value(ch, gsn_acid_breath, APPLY_DAMAGE_COUNT) <= 0 ){
				affect_strip ( ch, gsn_acid_breath );
				send_to_char( skill_table[gsn_acid_breath].msg_off, ch );
				send_to_char( "\n\r", ch );
			}
		}
		if ( is_affected( ch, gsn_fire_breath) && !ch->fighting )
		{
			if( affected_value(ch, gsn_fire_breath, APPLY_DAMAGE_COUNT) > 0 ){
				AFFECT_DATA af;
				af.type      = gsn_fire_breath;
				af.duration  = 0;
				af.location  = APPLY_DAMAGE_COUNT;
				af.modifier  = 0 - 20 - affected_value(ch, gsn_fire_breath, APPLY_DAMAGE_COUNT)/5;
				af.bitvector = 0;
				affect_join( ch, &af );
			}
			if( affected_value(ch, gsn_fire_breath, APPLY_DAMAGE_COUNT) <= 0 ){
				affect_strip ( ch, gsn_fire_breath );
				send_to_char( skill_table[gsn_fire_breath].msg_off, ch );
				send_to_char( "\n\r", ch );
			}
		}
		if ( is_affected( ch, gsn_frost_breath) && !ch->fighting )
		{
			if( affected_value(ch, gsn_frost_breath, APPLY_DAMAGE_COUNT) > 0 ){
				AFFECT_DATA af;
				af.type      = gsn_frost_breath;
				af.duration  = 0;
				af.location  = APPLY_DAMAGE_COUNT;
				af.modifier  = 0 - 20 - affected_value(ch, gsn_frost_breath, APPLY_DAMAGE_COUNT)/5;
				af.bitvector = 0;
				affect_join( ch, &af );
			}
			if( affected_value(ch, gsn_frost_breath, APPLY_DAMAGE_COUNT) <= 0 ){
				affect_strip ( ch, gsn_frost_breath );
				send_to_char( skill_table[gsn_frost_breath].msg_off, ch );
				send_to_char( "\n\r", ch );
			}
		}
		if ( is_affected( ch, gsn_lightning_breath) && !ch->fighting )
		{
			if( affected_value(ch, gsn_lightning_breath, APPLY_DAMAGE_COUNT) > 0 ){
				AFFECT_DATA af;
				af.type      = gsn_lightning_breath;
				af.duration  = 0;
				af.location  = APPLY_DAMAGE_COUNT;
				af.modifier  = 0 - 20 - affected_value(ch, gsn_lightning_breath, APPLY_DAMAGE_COUNT)/5;
				af.bitvector = 0;
				affect_join( ch, &af );
			}
			if( affected_value(ch, gsn_lightning_breath, APPLY_DAMAGE_COUNT) <= 0 ){
				affect_strip ( ch, gsn_lightning_breath );
				send_to_char( skill_table[gsn_lightning_breath].msg_off, ch );
				send_to_char( "\n\r", ch );
			}
		}

	   /*
		*   Careful with the damages here,
		*      MUST NOT refer to ch after damage taken,
		*      as it may be lethal damage (on NPC).
		*/
		affect_strip( ch, gsn_moon_night );
		if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_VAMPIRE ) )
		{
			AFFECT_DATA af;
			//affect_strip( ch, gsn_moon_night );
			if ( time_info.hour > 6 && time_info.hour < 18 )
			{
				if (   !IS_SET( ch->in_room->room_flags, ROOM_UNDERGROUND )
					&& !IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
				{
					int dmg = ch->max_hit;

					send_to_char( "�l�尭���Ӧb�դѬ��ʪ��I\n\r", ch );
					if( ch->class < 5 && ch->level < 30 ){
						dmg = ch->hit / 5;
					}else{
						//dmg = ((ch->hit / 2)+2);
						dmg = ch->hit * (10 - nSklv) / 30;
						if(nSklv > 4) dmg -= (nSklv - 4) * 25;
						if(dmg < 0) dmg = ch->hit /10;
					}
			  
					damage( ch, ch, dmg, gsn_vampiric_bite, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					if(ch->deleted) break;
				}
				//  �l�尭(vampire)    12   13   12   15   11  
				int nDecay = (time_info.hour > 12) ? (time_info.hour - 12) : (12 - time_info.hour);
				int nMaxHr =  8 - nSklv/2 - (nSklv * nSklv) / 6;
				int nMaxDr =  8 - nSklv/2 - (nSklv * nSklv) / 6;

				if(nSklv < 4) nDecay /= 2;

				nMaxHr -= nDecay;
				nMaxDr -= nDecay;

				if(nMaxHr < 0) nMaxHr = 0;
				if(nMaxDr < 0) nMaxDr = 0;

				if(nMaxHr != 0){
					affect_strip( ch, gsn_moon_night );
					af.type      = gsn_moon_night;
					af.duration  = -1;
					af.location  = APPLY_HITROLL;
					af.modifier  = - nMaxHr;
					af.bitvector = 0;
					affect_to_char( ch, &af );
				}
				if(nMaxDr != 0){
					af.location  = APPLY_DAMROLL;
					af.modifier  = - nMaxDr;
					affect_to_char( ch, &af );
				}
			}else{
				//  �l�尭(vampire)    12   13   12   15   11  
				int nDecay = (time_info.hour > 12) ? (24 - time_info.hour) : (time_info.hour);
				int nMaxHr =  15 + (nSklv + nSklv * nSklv) / 2; // nMaxHR(6) = 14 + 6 + 35 / 4 = 28
				int nMaxDr =  15 + (nSklv + nSklv * nSklv) / 2; // 15 16 18 21 25 30 36

				affect_strip( ch, gsn_moon_night );
				af.type      = gsn_moon_night;
				af.duration  = -1;
				af.location  = APPLY_HITROLL;
				af.modifier  = nMaxHr - nDecay * 1;
				af.bitvector = 0;
				affect_to_char( ch, &af );
				af.location  = APPLY_DAMROLL;
				af.modifier  = nMaxDr - nDecay * 1;
				affect_to_char( ch, &af );
			}
		}

		affect_strip( ch, gsn_werewolf );

		if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_WEREWOLF ) && !IS_NPC(ch) && !is_affected(ch, gsn_polymorph))
		{
			AFFECT_DATA af;
			// if (time_info.hour > 3 + nSklv && time_info.hour < 21 - nSklv){
			if (time_info.hour > 4 + nSklv * 2 / 3 && time_info.hour < 20 - nSklv * 2 / 3){
				if(ch->class >= 5)
				{
					af.type      = gsn_werewolf;
					af.duration  = -1;
					af.location  = APPLY_RACE;
					af.modifier  = -ch->race;
					af.bitvector = 0;
					affect_to_char( ch, &af );
					/*
						af.location  = APPLY_AC;
						af.modifier  = 100 - nSklv * (nSklv + 4);
						affect_to_char( ch, &af );

						af.location  = APPLY_DODGE;
						af.modifier  = 100 - nSklv * (nSklv + 4);
						affect_to_char( ch, &af );
					*/
				}
			}else{
				int nDecay   = (time_info.hour > 12) ? (24 - time_info.hour) : (time_info.hour);
				int nMaxPD   = 10 + nSklv * 20;
				int nMaxAC   = nSklv * nSklv * nSklv + nSklv * 20;
				int nMaxHR   = 5 + (nSklv + nSklv * nSklv)/2;
				if(nSklv == 0) nMaxPD = 0;
				nMaxAC = (nMaxAC / 10) * 10;

				af.type      = gsn_werewolf;
				af.duration  = -1;
				af.bitvector = 0;

				if(nDecay <= 6){
					nMaxPD   = nMaxPD * (12 - nDecay) / 12;
					nMaxAC   = nMaxAC * (12 - nDecay) / 12;
					nMaxHR   = nMaxHR * (12 - nDecay) / 12;
				}else if(nSklv < 6){
					nMaxPD = nMaxPD / 4;
					nMaxAC = nMaxAC / 4;
					nMaxHR = nMaxAC / 4;
				}else{
					nMaxPD = nMaxPD / 3;
					nMaxAC = nMaxAC / 3;
					nMaxHR = nMaxHR / 3;
				}

				if(nMaxPD > 0){
					af.location  = APPLY_ADAMAGE;
					af.modifier  = nMaxPD;
					affect_to_char( ch, &af );
				}

				if(nMaxAC > 0){
					af.location  = APPLY_AC;
					af.modifier  = - nMaxAC;
					affect_to_char( ch, &af );
				}

				if(nMaxHR > 0){
					af.location  = APPLY_HITROLL;
					af.modifier  = nMaxHR;
					affect_to_char( ch, &af );
				}

				af.location  = APPLY_RACE;
				af.modifier  = 0;
				affect_to_char( ch, &af );

				if(nDecay <= 4)
				{
					if(number_percent() >= 40 + get_curr_wis(ch) * 2 + nSklv * nSklv && ch->level >= 36 && ch->class >= 5)
					{
						if(is_affected(ch, gsn_will_fortress)){
						   act("�N�ӭn����F�A�b����U�ƨg������.", ch, NULL, NULL, TO_CHAR);
						}else{
						   act(HIC"�A��פ������]�O�A���������a��g�F�_�ӡI"NOR, ch, NULL, NULL, TO_CHAR);
						   act(HIC"�b���䪺����U�A$n"HIC"��M�����a��g�F�_�ӡI"NOR, ch, NULL, NULL, TO_ROOM);
						   af.type       = skill_lookup("demon tremor");
						   af.bitvector  = AFF_CRAZY;
						   af.duration   =   0;

						   af.location   = APPLY_HITROLL;
						   af.modifier   = -10;
						   affect_to_char(ch, &af);

						   af.location   = APPLY_DAMROLL;
						   af.modifier   =  10;
						   affect_to_char(ch, &af);

						   af.location   = APPLY_INT;
						   af.modifier   = -2;
						   affect_to_char(ch, &af);

						   af.location   = APPLY_STR;
						   af.modifier   =  2;
						   affect_to_char(ch, &af);
						}
					}
				}
			}
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		if (   (  ch->in_room->sector_type == SECT_UNDERWATER
				&& ( !IS_IMMORTAL( ch )
				&& !IS_AFFECTED( ch, AFF_GILLS )
				&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_WATERBREATH ) ) )
			|| (   ch->in_room->sector_type != SECT_UNDERWATER
				&& ch->in_room->sector_type != SECT_WATER_NOSWIM
				&& IS_SET( race_table[ ch->race ].race_abilities, RACE_WATERBREATH )
				&& ( str_cmp( race_table[ ch->race ].name, "Object" ) 
				&& str_cmp( race_table[ ch->race ].name, "God" ) 
				&& str_cmp( race_table[ ch->race ].name, "Bachi" )
				&& str_cmp( race_table[ ch->race ].name, "Merfolk" )
				&& str_cmp( race_table[ ch->race ].name, "Kuo-toa" )) ) )
		{
			send_to_char( "�A����I�l�F�I\n\r", ch );
			act( "$n �@�}�r�y�A�n������I�l�F�I", ch, NULL, NULL, TO_ROOM );
			damage( ch, ch, number_range(ch->max_hit/20, ch->max_hit/10), gsn_breathe_water, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		if ( IS_AFFECTED( ch, AFF_POISON ) )
			//&& !IS_IMMORTAL( ch ) 
			//&& !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON))
		{
			/*
			// poison won't kill mob, by Amethyst
			if ( IS_NPC( ch ) )
				ch->hit = ( ( ( ch->hit - ch->saving_throw[MAGIC_POISON] ) <= 0 ) ? 
							1 : ch->hit - ch->saving_throw[MAGIC_POISON] );
			else
					ch->hit = ch->hit - ch->saving_throw[MAGIC_POISON];
			*/

			// race_no_poison �� mob no_poison ���|��ܤ��r�r�� 2021//11/02
			// �s�W aff_ghoul ���|��ܤ��r�r��2022/01/16
			if(    !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON)
				&& !IS_SET( ch->special, NO_POISON )
				&& !IS_AFFECTED( ch, AFF_GHOUL ))
			{
				send_to_char("�A�h�W�aŸ��.\n\r", ch);
				act( "$n �h�W�aŸ��.", ch, NULL, NULL, TO_ROOM );
			}
			// �s�W magic_poison �p�� 0 ���P�w
			if( NEW_GET_SVS(ch,MAGIC_POISON) < 0 ){
				// race_undead �󤤬r�U�i��L�����r�ܩ���o�^�� 2021/11/02
				// �ؼбa�� AFF_GHOUL ���P�����ܬ������� 2022/01/16
				if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD) || IS_AFFECTED( ch, AFF_GHOUL ) ){
					ch->hit = ch->hit - NEW_GET_SVS(ch,MAGIC_POISON)/2;
					send_to_char("�@�r��_�F�A���ͩR.\n\r", ch);
				}
			}else{
				// ���r�ˮ`�̧C���ܤ֬� ch ��e level 2021/11/02
				// �s�W aff_ghoul ���|���r�ˮ` 2022/01/16
				if(    !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON)
					&& !IS_SET( ch->special, NO_POISON )
					&& !IS_AFFECTED( ch, AFF_GHOUL ) )
				{
					ch->hit = ch->hit - NEW_GET_SVS(ch,MAGIC_POISON);
				}
			}
			// �@�r�ˮ` imm , npc ���Q�r�� 2022/04/22
			if( ( IS_IMMORTAL( ch ) || IS_NPC( ch ) ) && NEW_GET_SVS(ch,MAGIC_POISON) >= 0 )
			{
				if( (ch->hit - NEW_GET_SVS(ch,MAGIC_POISON) ) <= 0 )
					ch->hit = 1;
			}
			update_pos( ch );
			if ( ch->position == POS_DEAD )
			{
				raw_kill( ch, ch );
				continue;
			}
		}
		if ( IS_AFFECTED( ch, AFF_DISEASE ) && !IS_IMMORTAL( ch ) )
		{
			send_to_char("�A���骺��q���b�y��.\n\r", ch);
			act( "$n ����q���b�y��.", ch, NULL, NULL, TO_ROOM );
			if (IS_NPC(ch))
			{
				// disease won't kill mob, by Amethyst
				if ( (ch->hit - ch->level*2) <= 0 )
					ch->hit = 1;
				else
				{
					ch->hit -= ch->level*2;
					ch->mana -= ch->level*2;
					ch->move -= ch->level*2;
				}
			}
			else
			{
				ch->hit = ch->hit - ch->hit/12;
				ch->mana = ch->mana - ch->mana/12;
				ch->move = ch->move - ch->move/12;
			}
			update_pos( ch );
			if ( ch->position == POS_DEAD )
			{
				raw_kill( ch, ch );
				continue;
			}
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		if ( ch->position == POS_MORTAL )
		{
			damage( ch, ch, 2, TYPE_UNDEFINED, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		if ( ch->position == POS_INCAP )
		{
			damage( ch, ch, 1, TYPE_UNDEFINED, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		if( !IS_NPC(ch) && !IS_IMMORTAL(ch) && is_affected(ch, gsn_mental_empower)){
			send_to_char("�A�P��@�}�L�O.\n\r", ch);
			ch->move -= ch->level + ch->level / 2;
			// ���� ch mv �p�� 0 �����p 2023/01/13
			if( ch->move < 0 ) ch->move = 0;
		}

		if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			continue;

		// fall �P�w 2023/01/14
		if( !is_flying( ch ) )
			fall_effect( ch );
	}

    /*
    * Autosave and autoquit.
    * Check that these chars still exist.
     */
	if ( ch_save || ch_quit )
	{
		for ( ch = char_list; ch; ch = ch->next )
		{

			if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
				continue;

			if ( ch == ch_save )
				save_char_obj( ch );
			if ( ch == ch_quit )
			{
				if( auction_list )
				{
					AUC_DATA * auction;
					AUC_DATA * auc_next = NULL;
					for( auction = auction_list ; auction ; auction = auc_next )
					{
						if( ch == auction->owner )
						{
							talk_channel( auction->owner , "�ѩ󥻤H�_�u�F�A���]������C", CHANNEL_AUCTION, "�i���j",HIY );
							obj_to_char( auction->obj, ch );

							if( auction == auction_list ){
								auction_list = auction_list->next;
							}else{
								for( auc_next = auction_list ; auc_next; auc_next = auc_next->next )
								{
									if( auc_next->next == auction ){
										auc_next->next = auction->next;
										auc_next = auction->next;
										break;
									}
								}
							}
							free_auction_data(auction);
							continue;
						}
						else if( ch == auction->buyer )
						{
							talk_channel( auction->owner , "�ѩ�R���_�u�F�A���]������C", CHANNEL_AUCTION, "�i���j",HIY );
							obj_to_char( auction->obj, auction->owner );

							if( auction == auction_list ){
								auction_list = auction_list->next;
							}else{
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
							free_auction_data(auction);
							continue;
						}
						auc_next = auction->next;
					}
				}
				if( ch->was_in_room ) {
				char_from_room(ch);
				char_to_room(ch, ch->was_in_room);
				}
				do_idlequit( ch, "" );
			}
		}
	}

	return;
}
 
/*
 * Update all objs.
 * This function is performance sensitive.
 * �[�s���� + �ƪ� by Razgriz 20050917
 */
void obj_update ( void )
{   
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	char buf[MAX_STRING_LENGTH];

	GLOBAL_OBJ_UPDATE = TRUE;     //Added by Razgriz  20050625

	//check every object in the world
	for ( obj = object_list; obj; obj = obj_next )
	{
		CHAR_DATA *rch;
		CHAR_DATA *cch; // 2022/11/30
		char *message;

		obj_next = obj->next;
		if ( obj->deleted   ) continue;
		if ( obj->in_eqbank ) continue;

		updating_obj = obj;     //Added by Razgriz  20050702

		//Modified by Razgriz 20050625
		if( obj->carried_by )
		{
			obj->update_checked = FALSE;
			oprog_rand_trigger ( obj );
			obj->update_checked = TRUE;
		}
		else if ( obj->in_room )
			oprog_tick_trigger ( obj );

		if ( obj->timer < -1 )
			obj->timer = -1;

		if( obj->carried_by && obj->wear_loc != -1 && IS_SET (obj->pIndexData->souvenir, Z1) )
		{
			act ( "�A�Q $p ���ƥ��F.", obj->carried_by, obj, NULL, TO_CHAR );
			act ( "$n �Q $p �ƥ��F." , obj->carried_by, obj, NULL, TO_ROOM );
			unequip_char( obj->carried_by, obj );
		}

		/* Added by Razgriz 20050917
		 * �[�J�� obj affect �� duration �B�z
		 */
		AFFECT_DATA *paf = NULL;
		AFFECT_DATA *pafPre = NULL;
		AFFECT_DATA *pafNext = NULL;

		pafPre = obj->affected;
		for ( paf = obj->affected; paf; paf = pafNext )
		{
			pafNext = paf->next;

			// 2022/11/29
			if ( paf->duration == -1 )
				continue;

			if ( paf->duration != 0 )
			{
				if ( paf->duration > 0 )
					paf->duration--;
				pafPre = paf;
				continue;
			}

			if ( paf == obj->affected )
				obj->affected = pafPre = paf->next;
			else
				pafPre->next = paf->next;

			// �[�J�ݩʪ��]�����ɪ��ԭz�� flag ���� 2021/09/28
			if( paf->location == APPLY_WEAPON_SPELL && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�W�Y���[���]�k�����F.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�W�Y���[���]�k�����F.", rch, obj, NULL, TO_ROOM );
						act ( "$p�W�Y���[���]�k�����F.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_WIND && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�P����¶����۳v���촲.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�P����¶����۳v���촲.", rch, obj, NULL, TO_ROOM );
						act ( "$p�P����¶����۳v���촲.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_FIRE && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�U�N���P�������a�N�o�U��.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�U�N���P�������a�N�o�U��.", rch, obj, NULL, TO_ROOM );
						act ( "$p�U�N���P�������a�N�o�U��.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_COLD && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p���X�����źC�C�a�����F.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p���X�����źC�C�a�����F.", rch, obj, NULL, TO_ROOM );
						act ( "$p���X�����źC�C�a�����F.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_EARTH && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�W���۪������H���渨�F.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�W���۪������H���渨�F.", rch, obj, NULL, TO_ROOM );
						act ( "$p�W���۪������H���渨�F.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_LIGHTNING && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�W�A�ԧ@�T���q�𺥺��a�R�F�U��.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�W�A�ԧ@�T���q�𺥺��a�R�F�U��.", rch, obj, NULL, TO_ROOM );
						act ( "$p�W�A�ԧ@�T���q�𺥺��a�R�F�U��.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_SAINT && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�o�X���벴�������a�t�H�U��.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�o�X���벴�������a�t�H�U��.", rch, obj, NULL, TO_ROOM );
						act ( "$p�o�X���벴�������a�t�H�U��.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			if( paf->location == APPLY_WEAPON_DARK && IS_SET( obj->extra_flags, ITEM_SPELL ) ){
				if ( obj->carried_by ){
					act( "$p�P�򪺥��u�S�A�����G�F�_��.", obj->carried_by, obj, NULL, TO_CHAR );
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p�P�򪺥��u�S�A�����G�F�_��.", rch, obj, NULL, TO_ROOM );
						act ( "$p�P�򪺥��u�S�A�����G�F�_��.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_SPELL );
			}
			// ��ʩ٬r���~�r�ʮĪG���� 2021/11/11
			if( paf->location == APPLY_TOXICITY_POISON && IS_SET( obj->extra_flags, ITEM_POISONED ) ){
				// �Z���r�ʮ�������ӭ@�[ 25%
				if ( !indestructible(obj) && obj->item_type == ITEM_WEAPON ){
					obj->value[0] = get_curr_dura(obj) - get_max_dura(obj)/4;
					if( obj->value[0] < 0 ) obj->value[0] = 0;
				}
				if ( obj->carried_by ){
					cch = obj->carried_by;
					act( "$p���@�r���������F.", obj->carried_by, obj, NULL, TO_CHAR );
					// �˳Ʃ٬r�Z�������ɧP�w�O�_���l
					if( is_broken(obj) && obj->wear_loc > -1 ){
						act( "$p�l�a�F.", obj->carried_by, obj, NULL, TO_CHAR );
						// 2022/11/29
						//	1. obj_from_char ���N�J obj �ӫD char
						//	2. obj_from_char ��ϥ� obj->carried_by �|�줣��ؼ� , ���� null
						//unequip_char(obj->carried_by, obj );
						//obj_from_char( obj->carried_by );
						//obj_to_char( obj, obj->carried_by );
						unequip_char( obj->carried_by, obj );
						obj_from_char( obj );
						obj_to_char( obj, cch );
					}
				}else{
					if( obj->in_room && ( rch = obj->in_room->people ))
					{
						act ( "$p���@�r���������F.", rch, obj, NULL, TO_ROOM );
						act ( "$p���@�r���������F.", rch, obj, NULL, TO_CHAR );
					}
				}
				REMOVE_BIT( obj->extra_flags, ITEM_POISONED );
			}
			// ��^ paf , �N��I�� poison weapon �h�r�l�a, �ثe�٬O��@�� 2022/11/29
			if( obj->carried_by && obj->wear_loc != -1 ){
				affect_modify(  obj->carried_by , paf, FALSE );
			}
			free_affect ( paf );
		}
		/* obj affect �B�z���� */


		/* Added by Razgriz 20050918
		 * �[�J�� obj_extra_value ���ɮĳB�z
		 */
		if ( ( obj->extra_value ).used )
		{
			( obj->extra_value ).t[0] = UMAX ( -1, ( obj->extra_value ).t[0] - 1 );
			( obj->extra_value ).t[1] = UMAX ( -1, ( obj->extra_value ).t[1] - 1 );
			if ( ( obj->extra_value ).t[0] == 0 )
			{
				( obj->extra_value ).v[0] = 0;
				( obj->extra_value ).v[1] = 0;
			}
		}
		/* obj_extra_value �B�z���� */

		switch ( obj->item_type )
		{
			default:
				break;
			// recover flag �� eq �@�ΦA��
			case ITEM_WEAPON:
				if( IS_SET(obj->extra_flags, ITEM_RECOVER)
					//&& obj->value[0] > 0)
					&& get_max_dura( obj ) > 0 )
					repair_dura(obj , 1);
				break;
			case ITEM_ARMOR:
				if( IS_SET(obj->extra_flags, ITEM_RECOVER) && obj->value[2] > 0){
					repair_dura(obj , 1);
					if( obj->value[1] > 0){
						obj->value[0] += 1;
						obj->value[1] -= 1;
					}
				}
				break;
			//�u���A�� 2020/01/30
			case ITEM_FOUNTAIN:
				if ( obj->value[0] < 0) break;
				if ( obj->value[0] > obj->value[1]
					&& obj->value[0] > 0
					&& obj->value[1] != -1 )
				{
					if(weather_info.sky == SKY_RAINING)
						obj->value[1] += obj->value[0] * 1/100;
					if(weather_info.sky == SKY_LIGHTNING)
						obj->value[1] += obj->value[0] * 4/100;
					if(weather_info.sky == SKY_SNOWING)
						obj->value[1] += obj->value[0] * 2/100;
					if(weather_info.sky == SKY_SNOWSTORM)
						obj->value[1] += obj->value[0] * 3/100;

					obj->value[1] += 1 + obj->value[0] * 1/100;

					if(obj->value[1] < 0) obj->value[1] = 0;

					obj->value[1] = UMIN( obj->value[0] , obj->value[1] );
				}
				break;
		}

		if ( obj->timer < 0 ) continue;

		if ( obj->in_obj && obj->in_obj->value[0] == -1 ) continue;

		//obj �b mob ���W�L�� timer
		if ( obj->carried_by && ( IS_NPC ( obj->carried_by ) && !IS_SET ( obj->carried_by->act, ACT_HORSE ) ) )
		{
			continue;
		}

		/*
		 *  Bug fix:  used to shift to obj_free if an object whose
		 *  timer ran out contained obj->next. Bug was reported
		 *  only when the object(s) inside also had timer run out
		 *  during the same tick.     --Thelonius (Monk)
		*/
		//obj timer �� 0 , obj �n�����F
		if ( --obj->timer == 0 )
		{
			//����檺��, �ˬd�O�_�Ӫ��~�b��椤
			if( auction_list )
			{
				AUC_DATA *auction, *auc_next;
				for( auction = auction_list ; auction; auction = auc_next )
				{
					if( obj == auction->obj )
					{
						sprintf ( buf, "�ѩ�%s�����F�A��椤��C", obj->short_descr);
						talk_channel ( auction->owner , buf, CHANNEL_AUCTION, "�i���j",HIY );
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
						free_auction_data(auction);
						break;
					}
					auc_next = auction->next;

				}// end for auction
				oprog_disappear_trigger(obj);

			}// end if auction_list


			//added by Amethyst
			//Reset the value of a poisoned weapon
			//For those undamaged weapon, it will repair automatically
			if( IS_SET ( obj->extra_flags, ITEM_BROKEN ) && get_max_dura( obj ) < 0 )
			{
				REMOVE_BIT ( obj->extra_flags, ITEM_BROKEN );
				obj->timer = -1;
				if ( obj->carried_by )
					act( "$p �����_��F.", obj->carried_by, obj, NULL, TO_CHAR );
				continue;
			}

			//�ˬd�٬r�Z�����ɮ� , �٬r�Z���ɶ��P�w���� obj affect  2021/11/11
			if ( IS_SET ( obj->extra_flags, ITEM_POISONED_WEAPON ) )
			{
				/*  �s���٬r�Z�����A�ϥ� obj timer ,�B���|�a���Z���h�r�ᤣ�|�l�� 2021/11/11
				if ( get_max_dura( obj ) < 0 )
				{
					SET_BIT( obj->extra_flags, ITEM_BROKEN );
					obj->timer = 120;
				}
				else
				{
					obj->timer = -1;
					obj->value[0] = 0;
				}
				*/
				obj->timer = -1;
				REMOVE_BIT( obj->extra_flags, ITEM_POISONED );
				REMOVE_BIT( obj->extra_flags, ITEM_POISONED_WEAPON );
				if ( obj->carried_by )
				{
					act( "$p ���r���������F.", obj->carried_by, obj, NULL, TO_CHAR );
					if( obj->wear_loc > -1 )
					{
						act( "$p �w�g����A�ϥΤF.", obj->carried_by, obj, NULL, TO_CHAR );
						act( "$n ��U $p.", obj->carried_by, obj, NULL, TO_ROOM );
						act( "�A��U $p.", obj->carried_by, obj, NULL, TO_CHAR );
						unequip_char(obj->carried_by, obj);
					}
				}
				continue;
			}


			//obj �������ԭz
			switch ( obj->item_type )
			{
				default:              message = "$p �����F.";   break;
				case ITEM_FOUNTAIN:   message = "$p �\\�ܤF.";    break;
				case ITEM_CORPSE_NPC: message = "$p �Ʀ��Ǯ����F."; break;
				case ITEM_CORPSE_PC:  message = "$p �Ʀ��Ǯ����F."; break;
				case ITEM_FOOD:       message = "$p �G�ѤF."; break;
			}

			//�� obj �������ԭz�e�X�h
			if ( obj->carried_by )
			{
				act( message, obj->carried_by, obj, NULL, TO_CHAR );
			}
			else
			{
				if( obj->in_room && ( rch = obj->in_room->people ))
				{
					act ( message, rch, obj, NULL, TO_ROOM );
					act ( message, rch, obj, NULL, TO_CHAR );
				}
			}

			//������� obj ���X�O����
			if ( obj == object_list )
			{
				extract_obj ( obj );
				obj_next = object_list;
			}
			else        /* (obj != object_list) */
			{
				OBJ_DATA *	previous;
   
				for ( previous = object_list; previous; previous = previous->next )
				{
					if ( previous->next == obj )
						break;
				}

				if ( !previous )  /* Can't see how, but... */
					bug( "Obj_update: obj %d no longer in object_list", obj->pIndexData->vnum );

				extract_obj( obj );
				obj_next = previous->next;
			}

		}// end if obj->timer == 0

	}// end for obj = obj_list
	updating_obj = NULL;      //Added by Razgriz  20050702

	//�B�z��檫�~������
	if ( auction_list ) /*auction process...*/
	{
		AUC_DATA *auction, *auc_next = NULL;
		for ( auction = auction_list ; auction; auction = auc_next )
		{
			//�S�R�a��
			if ( !auction->buyer )
			{
				if( auction->time >= 4 )
				{
					obj_to_char ( auction->obj, auction->owner );
					sprintf ( buf, "�A�� %s �L�H�z�|�A��楢�ѤF�C\n\r", auction->obj->short_descr );
					send_to_char ( buf, auction->owner );
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
					free_auction_data (auction);
					continue;
				}
				else
				{
					sprintf ( buf, "%s�A�_�� %d �T�����A�w��U���v�СC",
						auction->obj->short_descr,
						auction->price );
					talk_channel ( auction->owner, buf, CHANNEL_AUCTION, "�i���j",HIY );
					auction->time++;
					auc_next = auction->next;
					continue;
				}
			}

			//���H�X��, �L 2 ticks �ᦨ��
			if ( auction->time >= 2 )
			{
				//����Ჾ�檫�~
				sprintf ( buf, "����%s�H %d �T�����������ʱo %s�I",
								auction->buyer->name,
								auction->price,
								auction->obj->short_descr );
				talk_channel ( auction->owner, buf, CHANNEL_AUCTION, "�i���j",HIY );
				obj_to_char ( auction->obj, auction->buyer );
				save_char_obj( auction->buyer );
				send_to_char (CYN"���|�u�� 5% �����J��������O�C"NOR"\n\r", auction->owner );
				sprintf ( buf,CYN"���|���A�e��%s�C"NOR"\n\r", auction->obj->short_descr );
				send_to_char (buf, auction->buyer );

				//�b clan ���ú���a�O
				if( auction->owner->in_room->area->clan )
				{
					auction->owner->in_room->area->clan->bank += ( auction->price / 30 );
					sprintf (buf, CYN"3%% ������O�����%s�������a���ѶO�C"NOR"\n\r", auction->owner->in_room->area->clan->name );
					send_to_char (buf, auction->owner );
				}
				//auction->buyer->gold = auction->buyer->gold - auction->price;
				auction->owner->bank += ( auction->price - auction->price / 20 );
				save_char_obj ( auction->owner );

				//�����Ʊq�O���餤�M��
				if( auction == auction_list )
				{
					auction_list = auction_list->next;
				}
				else
				{
					for ( auc_next = auction_list ; auc_next; auc_next = auc_next->next )
					{
						if( auc_next->next == auction )
						{
							auc_next->next = auction->next;
							auc_next = auction->next;
							break;
						}
					}
				}
				free_auction_data (auction);
				continue;
			}

			//���R�a�X���F, ����L�H�X����
			sprintf ( buf, "%s�ثe������O %d �T�����C�X���̰����O%s�A�w��j�a�v�СC",
							auction->obj->short_descr,
							auction->price,
							auction->buyer->name );
			auction->time++;
			talk_channel ( auction->owner, buf, CHANNEL_AUCTION, "�i���j",HIY );

			//���X�U�@�������, �ǳƦb�U�@�j��B�z
			auc_next = auction->next;

		}// end for auction

	}// end if auction_list
    
	GLOBAL_OBJ_UPDATE = FALSE;      //Added by Razgriz  20050625

	return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *     aggress on some random PC
 *
 * This function takes .2% of total CPU time.
 *
 * -Kahn
 */
void aggr_update( void )
{
	CHAR_DATA *ch;
	CHAR_DATA *mch;
	CHAR_DATA *vch;
	CHAR_DATA *victim;
	CHAR_DATA *wch_next;
	DESCRIPTOR_DATA *d;

	/*
	* Let's not worry about link dead characters. -Kahn
	*/
	for ( d = descriptor_list; d; d = d->next )
	{

		ch = d->character;

		if ( d->connected != CON_PLAYING
			|| !ch
			|| ch->level >= LEVEL_IMMORTAL
			|| !ch->in_room )
			continue;

		// �����ױ� ch �� npc �����p 2023/01/12
		if( IS_NPC(ch) ) continue;

		/* mch wont get hurt */
		for ( mch = ch->in_room->people; mch; mch = mch->next_in_room )
		{
			int count;
			// bool hate = FALSE;

			// 2023/01/12
			if( !mch || !mch->in_room || mch->deleted )
				continue;

			// �����ױ� mch �� pc �����p 2023/01/12
			if( !IS_NPC( mch ) ) continue;

			if( mch == ch) continue;

			#if 0
			if ( IS_NPC( mch ) && !mch->deleted && mch->mpactnum > 0 && mch->in_room->area->nplayer > 0 )
			{
				MPROG_ACT_LIST * tmp_act, *tmp2_act;
				for ( tmp_act = mch->mpact; tmp_act != NULL; tmp_act = tmp_act->next )
				{
					mprog_wordlist_check( tmp_act->buf,mch, tmp_act->ch, tmp_act->obj, tmp_act->vo, ACT_PROG );
					free_string( tmp_act->buf );
				}
				for ( tmp_act = mch->mpact; tmp_act != NULL; tmp_act = tmp2_act )
				{
					tmp2_act = tmp_act->next;
					free_mem( tmp_act /*, sizeof( MPROG_ACT_LIST ) */ );
				}
				mch->mpactnum = 0;
				mch->mpact = NULL;
			}
			#endif

			if( !mch || !mch->in_room ) continue;

			if ( !IS_NPC( mch )
				|| IS_SET(mch->in_room->room_flags, ROOM_SAFE)  // safe room �����|���� 2022/07/11
				|| IS_SET(ch->in_room->room_flags, ROOM_SAFE)   // safe room �����|���� 2022/07/11
				|| mch->deleted
				|| mch->fighting
				|| IS_AFFECTED( mch, AFF_CHARM )
				|| IS_AFFECTED( mch, AFF_SUMMONED )
				|| !IS_AWAKE( mch )
				|| ( IS_SET( mch->act, ACT_WIMPY ) && IS_AWAKE( ch ) )
				|| !can_see( mch, ch )
				|| ( !IS_SET( mch->act, ACT_AGGRESSIVE ) && !IS_AFFECTED( mch, AFF_CRAZY ) ) ) /*added by Amenda */
				/*  && ( str_infix( race_table[ch->race].name, race_table[mch->race].hate )
						|| ( !str_infix( race_table[ch->race].name, race_table[mch->race].hate )
							&& abs( mch->level - ch->level ) > 4 ) ) ) )*/
				continue;
			/* if ( !str_infix( race_table[ch->race].name, race_table[mch->race].hate ) && number_bits( 3 ) == 1 )
				hate = TRUE;*/

			/*
			* Ok we have a 'ch' player character and a 'mch' npc aggressor.
			* Now make the aggressor fight a RANDOM pc victim in the room,
			*   giving each 'vch' an equal chance of selection.
			*/

			count  = 0;
			victim = NULL;
			for ( vch = mch->in_room->people; vch; vch = vch->next_in_room )
			{
				if ( vch->deleted || vch->level >= LEVEL_IMMORTAL )
					continue;

				if( mch == vch )
					continue;

				// mch crazy �������ؼ� 2022/12/24
				if( !IS_AFFECTED( mch, AFF_CRAZY ) ){
					if ( IS_NPC( vch ) )
						continue;
				}

				if ( ( !IS_SET( mch->act, ACT_WIMPY ) || !IS_AWAKE( vch ) ) && can_see( mch, vch ) )
				{
					// �D aggressive mob �b crazy �����p�U, �C���v�����ж��������� 2022/12/24
					// �[�J nplayer �P�w�ϰ줤�n�� pc, crazy �~�|�����H 2023/01/12
					if( !IS_SET( mch->act, ACT_AGGRESSIVE ) && IS_AFFECTED( mch, AFF_CRAZY ) && mch->in_room->area->nplayer > 0 )
					{
						if( number_percent() >= 95 ){
							victim = vch; // ��쪺�ؼ� 2023/01/12
							break;
						}
						continue;
					}

					// if ( !hate || ( hate && !str_infix( race_table[vch->race].name, race_table[mch->race].hate ) ) )
					{
						if ( number_range( 0, count ) == 0 )
							victim = vch;
						count++;
					}
				}
			}

			if ( !victim )
				continue;

			multi_hit( mch, victim, TYPE_UNDEFINED );

		} /* mch loop */

	} /* descriptor loop */

	// ED Fix

	for ( ch = char_list; ch != NULL; ch = wch_next )
	{
		wch_next = ch->next;

		/* MOBProgram ACT_PROG trigger */
		if ( IS_NPC( ch ) && ch->mpactnum > 0)
		// Merc 2.2 only freed this memory when a player was in the zone, I see
		// no reason to wait for that to happen--Slash
		//     && wch->in_room->area->nplayer > 0 )
		{
			// Examine everything the mobile heard since the last aggr_update(), and
			// respond to it
			MPROG_ACT_LIST * tmp_act, *tmp2_act;
			for ( tmp_act = ch->mpact; tmp_act != NULL; tmp_act = tmp_act->next )
			{
				mprog_wordlist_check( tmp_act->buf, ch, tmp_act->ch, tmp_act->obj, tmp_act->vo, ACT_PROG );
				free_string( tmp_act->buf );
			}

			// Free the memory allocated to the 'act() memory'
			for ( tmp_act = ch->mpact; tmp_act != NULL; tmp_act = tmp2_act )
			{
				tmp2_act = tmp_act->next;
				free_mem( tmp_act /*, sizeof( MPROG_ACT_LIST ) */ );
			}

			// Zap the pointer to the memory
			ch->mpactnum = 0;
			ch->mpact = NULL;
		}
	}

	return;
}

/* Update the check on time for autoshutdown */
void time_update( void )
{
	FILE *fp;
	char buf [ MAX_STRING_LENGTH ];
	extern bool wizlock, numlock;
	extern time_t warning0;
	extern time_t warning1;
	extern time_t warning2;
	extern time_t down_time;

	//const char warning_figlet[] =
	//      HIY"  _    _                      _               \n\r"
	//      " | |  | |                    (_)              \n\r"
	//      " | |  | |  __ _  _ __  _ __   _  _ __    __ _ \n\r"
	//      " | |/\\| | / _` || '__|| '_ \\ | || '_ \\  / _` |\n\r"
	//      " \\  /\\  /| (_| || |   | | | || || | | || (_| |\n\r"
	//      "  \\/  \\/  \\__,_||_|   |_| |_||_||_| |_| \\__, |\n\r"
	//      "                                         __/ |\n\r"
	//      "                                        |___/ \n\r"NOR;

	const char warning_figlet[] =
			HIY
			"                                        �~����                                \n\r"
			"�~���������~���������~���������~���~�����~�����~���~�����~���������~�����~����\n\r"
			"�������������~���m�����m�~�������m�~�������m�����m�~�������m�~�������m�����m��\n\r"
			"����  ������������������������������������������������������������������������\n\r"
			"������������������������������������������������������������������������������\n\r"
			"                                      "HIW"System Reboot Warning by jye"HIY"������������\n\r"NOR;

	if( down_time <= 0 )
		return;

	if( current_time > warning0 && warning0 > 0 )
	{
		sprintf( buf, HIR"�Ĺs��ĵ�i!"NOR"\n\r\n\r�t�αN�b "HIW"%d"NOR" ����("HIW"%d"NOR" ��) ��"HIY"%s"NOR"�C"NOR"\n\r",
						(int) ( down_time - current_time ) / 60,
						(int) ( down_time - current_time ) ,
						Reboot ? "���}" : "����");

		send_to_all_char(warning_figlet);
		send_to_all_char( buf );
		warning0 = 0;
	}
	else if( current_time > warning1 && warning1 > 0 )
	{
		sprintf( buf, HIR"�Ĥ@��ĵ�i!"NOR"\n\r\n\r�t�αN�b "HIW"%d"NOR" ����("HIW"%d"NOR" ��) ��"HIY"%s"NOR"�C"NOR"\n\r",
						(int) ( down_time - current_time ) / 60,
						(int) ( down_time - current_time ) ,
						Reboot ? "���}" : "����" );

		numlock = TRUE;
		send_to_all_char(warning_figlet);
		send_to_all_char( buf );
		warning1 = 0;
	}
	else if( current_time > warning2 && warning2 > 0 )
	{
		sprintf( buf, HIR"�ĤG��ĵ�i!"NOR"\n\r\n\r�t�αN�b "HIW"%d"NOR" ����("HIW"%d"NOR" ��) ��"HIY"%s"NOR"�C"NOR"\n\r",
		(int) ( down_time - current_time ) / 60,
		(int) ( down_time - current_time ) ,
		Reboot ? "���}" : "����"
		);
		wizlock = TRUE;
		send_to_all_char(warning_figlet);
		send_to_all_char( buf );
		warning2 = 0;
	}
	else if( current_time + 30 > down_time && warning2 == 0 )
	{
		sprintf( buf, HIR"�̫�ĵ�i!"NOR"\n\r\n\r�t�αN�b "HIW"30"NOR" ������"HIY"%s"NOR"�C"NOR"\n\r",
		Reboot ? "���}" : "����");
		send_to_all_char(warning_figlet);
		send_to_all_char( buf );
		warning2--;
	}
	else if( current_time > down_time )
	{
		sprintf( buf, "%s by system.\n\r", Reboot ? "Reboot" : "Shutdown" );

		/* OLC 1.1b */
		do_asave( NULL, "" );

		send_to_all_char( buf );
		log_string( buf );

		end_of_game( );

		if( !Reboot )
		{
			if (fpReserve) {
				fclose( fpReserve );
				fpReserve = NULL;
			}
			if( !( fp = fopen( SHUTDOWN_FILE, "a" ) ) )
			{
				perror( SHUTDOWN_FILE );
				bug( "Could not open the Shutdown file!", 0 );
			}
			else
			{
				fprintf( fp, "Shutdown by System\n" );
				fclose ( fp );
			}
			fpReserve = fopen ( NULL_FILE, "r" );
		}
		merc_down = TRUE;
	}

	return;

}

/*
 * Remove deleted EXTRA_DESCR_DATA from objects.
 * Remove deleted AFFECT_DATA from chars and objects.
 * Remove deleted CHAR_DATA and OBJ_DATA from char_list and object_list.
 */

void list_update( void )
{
	CHAR_DATA *ch;
	CHAR_DATA *ch_next;
	OBJ_DATA  *obj;
	OBJ_DATA  *obj_next;
	extern bool delete_obj;
	extern bool delete_char;

	if ( delete_char )
	{
		for ( ch = char_list; ch; ch = ch_next )
		{
			AFFECT_DATA *paf;
			AFFECT_DATA *paf_next;

			for ( paf = ch->affected; paf; paf = paf_next )
			{
				paf_next = paf->next;

				if ( paf->deleted || ch->deleted )
				{
					if ( ch->affected == paf )
					{
						ch->affected = paf->next;
					}
					else
					{
						AFFECT_DATA *prev;

						for ( prev = ch->affected; prev; prev = prev->next )
						{
							if ( prev->next == paf )
							{
								prev->next = paf->next;
								break;
							}
						}

						if ( !prev )
						{
							bug( "List_update: cannot find paf on ch.", 0 );
							continue;
						}
					}

					paf->next = affect_free;
					affect_free = paf;
				}
			}

			ch_next = ch->next;

			if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			{
				if ( ch == char_list )
				{
					char_list = ch->next;
				}
				else
				{
					CHAR_DATA *prev;

					for ( prev = char_list; prev; prev = prev->next )
					{
						if ( prev->next == ch )
						{
							prev->next = ch->next;
							break;
						}
					}

					if ( !prev )
					{
						char buf [ MAX_STRING_LENGTH ];

						sprintf( buf, "List_update: char %s not found.", ch->name );
						bug( buf, 0 );
						continue;
					}
				}

				free_char( ch );
			}
		}
	}

	if ( delete_obj )
	{
		for ( obj = object_list; obj; obj = obj_next )
		{
			AFFECT_DATA *paf;
			AFFECT_DATA *paf_next;
			EXTRA_DESCR_DATA *ed;
			EXTRA_DESCR_DATA *ed_next;
			OBJ_HP *ohp, *ohp_next;

			for ( ed = obj->extra_descr; ed; ed = ed_next )
			{
				ed_next = ed->next;

				if ( obj->deleted )
				{
					free_string( ed->description );
					free_string( ed->keyword );
					ed->next = extra_descr_free;
					extra_descr_free = ed;
				}
			}

			for( ohp = obj->hp ; ohp; ohp = ohp_next ) {
				ohp_next = ohp->next;
				if( obj->deleted ) {
					free_string(ohp->hp_name);
					ohp->next = obj_hp_free;
					obj_hp_free = ohp;
				}
			}

			for ( paf = obj->affected; paf; paf = paf_next )
			{
				paf_next = paf->next;

				if ( obj->deleted )
				{
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
			}

			obj_next = obj->next;

			if ( obj->deleted )
			{
				if ( obj == object_list )
				{
					object_list = obj->next;
				}
				else
				{
					OBJ_DATA *prev;

					for ( prev = object_list; prev; prev = prev->next )
					{
						if ( prev->next == obj )
						{
							prev->next = obj->next;
							break;
						}
					}

					if ( !prev )
					{
						bug( "List_update: obj %d not found.", obj->pIndexData->vnum );
						continue;
					}
				}


				free_string( obj->name );
				free_string( obj->description );
				free_string( obj->short_descr );
				//free_string( obj->cname );
				--obj->pIndexData->count;

				obj->next = obj_free;
				obj_free  = obj;
			}
		}
	}

	delete_obj  = FALSE;
	delete_char = FALSE;

	return;
}

void new_list_update( void )
{
	CHAR_DATA *ch;
	CHAR_DATA *ch_next;
	OBJ_DATA  *obj;
	OBJ_DATA  *obj_next;
	extern bool delete_obj;
	extern bool delete_char;

	if ( delete_char ){
		for ( ch = char_list; ch; ch = ch_next )
		{
			AFFECT_DATA *paf;
			AFFECT_DATA *paf_next;

			for ( paf = ch->affected; paf; paf = paf_next )
			{
				paf_next = paf->next;

				if ( paf->deleted || ch->deleted )
				{
					if ( ch->affected == paf )
					{
						ch->affected = paf->next;
					}
					else
					{
						AFFECT_DATA *prev;

						for ( prev = ch->affected; prev; prev = prev->next )
						{
							if ( prev->next == paf )
							{
								prev->next = paf->next;
								break;
							}
						}

						if ( !prev )
						{
							bug( "List_update: cannot find paf on ch.", 0 );
							continue;
						}
					}

					paf->next = affect_free;
					affect_free = paf;
				}
			}

			ch_next = ch->next;

			if ( !ch || ch->deleted || !ch->in_room ) // �� deleted �~�A�[�W in_room �� !ch �P�w 2023/01/14
			{
				if ( ch == char_list )
				{
					char_list = ch->next;
				}
				else
				{
					CHAR_DATA *prev;

					for ( prev = char_list; prev; prev = prev->next )
					{
						if ( prev->next == ch )
						{
							prev->next = ch->next;
							break;
						}
					}

					if ( !prev )
					{
						char buf [ MAX_STRING_LENGTH ];

						sprintf( buf, "List_update: char %s not found.", ch->name );
						bug( buf, 0 );
						continue;
					}
				}

				free_char( ch );
			}
		}
	}

	if ( delete_obj )
	{
		for ( obj = object_list; obj; obj = obj_next )
		{
			AFFECT_DATA *paf;
			AFFECT_DATA *paf_next;
			EXTRA_DESCR_DATA *ed;
			EXTRA_DESCR_DATA *ed_next;
			OBJ_HP *ohp, *ohp_next;

			obj_next = obj->next;

			if ( obj->deleted )
			{
				for ( ed = obj->extra_descr; ed; ed = ed_next )
				{
					ed_next = ed->next;

					free_string( ed->description );
					free_string( ed->keyword );
					ed->next = extra_descr_free;
					extra_descr_free = ed;
				}

				for( ohp = obj->hp ; ohp; ohp = ohp_next ) {
					ohp_next = ohp->next;
					free_string(ohp->hp_name);
					ohp->next = obj_hp_free;
					obj_hp_free = ohp;
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

						paf->next   = affect_free;
						affect_free = paf;
					}
				}

				if ( obj == object_list )
				{
					object_list = obj->next;
				}
				else
				{
					OBJ_DATA *prev;

					for ( prev = object_list; prev; prev = prev->next )
					{
						if ( prev->next == obj )
						{
							prev->next = obj->next;
							break;
						}
					}

					if ( !prev )
					{
						bug( "List_update: obj %d not found.", obj->pIndexData->vnum );
						continue;
					}
				}


				free_string( obj->name );
				free_string( obj->description );
				free_string( obj->short_descr );
				// free_string( obj->cname );
				--obj->pIndexData->count;

				obj->next = obj_free;
				obj_free  = obj;
			}
		}
	}

	delete_obj  = FALSE;
	delete_char = FALSE;
	return;
}

/* Deduct qmark timer, and delete the qmark if
 * its timer reach 0. -- Amenda
 */
void qmark_update( void )
{
	QMARK *qmark, *qmark_next_in_list;

	if ( ( qmark = qmark_list ) )
	{
		while( qmark )
		{
			if ( qmark->timer > 0 )
			{
				qmark->timer --;
				if ( qmark->timer == 0 )
				{
					qmark_next_in_list = qmark->next_in_list;
					del_qmark( qmark );
					qmark = qmark_next_in_list;
					continue;
				}
			}
			qmark = qmark->next_in_list;
		}
	}
}

void mob_action_update( void )
{
	MOB_ACTION *mob_action;
	CHAR_DATA  *mob;
	char       *cmnd;

	for(mob_action = mob_action_list; mob_action != NULL; mob_action = mob_action->next_in_list){
		if ( mob_action->owner == NULL
		  || mob_action->owner->deleted
		  || mob_action->owner->position < POS_SLEEPING
		  || mob_action->owner->position == POS_FIGHTING ){
			continue;
		}

		if( IS_AFFECTED(mob_action->owner, AFF_CRAZY)
		|| IS_AFFECTED(mob_action->owner, AFF_PARALYZED)){
			continue;
		}
		if ( mob_action->start_time != time_info.hour && mob_action->mode != 1){
			mob_action->mode = 0;
			continue;
		}
		if ( mob_action->mode == 0 ){
			mob_action->mode = 1;
		}
		if( mob_action->mode == 2 ){
			continue;
		}
		mob  = mob_action->owner;
		cmnd = mob_action->com_list;
		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' ){
			mob_action->mode = 2;
			mob_action->com_list = mob_action->old_com_list;
		}else{
			// if( mob_action->mode == 2 ) {
			// mob_action = mob_action->next_in_list;
			// continue;
			// }
			action_process_cmnd( cmnd, mob );
			mob_action->com_list = action_next_command( mob_action->com_list );
		}
	}
}

char *action_next_command( char *clist )
{
	char *pointer = clist;

	while ( *pointer != '\n' && *pointer != '\0' )
		pointer++;

	if ( *pointer == '\n' || *pointer == '\r' )
		pointer++;

	return ( pointer );
}

void action_process_cmnd( char *cmnd, CHAR_DATA *mob )
{
	char buf[ MAX_INPUT_LENGTH * 5];
	char *str;
	char *point;

	point = buf;
	str   = cmnd;

	for ( ; ; )
	{
		while ( *str != '+' )
		{
			if ( *str != '\n' )
			{
				*point++ = *str++;
				continue;
			}
			else
			{
				*point = '\0';
				if ( !mob->deleted )
					interpret( mob, buf );
				return;
			}
		}
		*point = '\0';
		if ( !mob->deleted )
			interpret( mob, buf );
		point = buf;
		str++;
	}
}

// Deduct room timer -- Amenda
void rprg_update( void )
{
	RPROG_DATA *rprg;

	if ( rprg_list )
	{
		for( rprg = rprg_list; rprg; rprg = rprg->next_in_list )
		{
			if ( rprg->timer[0] > 0 )
			{
				--rprg->timer[0];
				if (rprg->timer[0] == 0 )
				{
					rprg->v0 = 0;
					rprg->v1 = 0;
				}
			}
			if( rprg->timer[1] > 0 )
				--rprg->timer[1];
		}
	}
}

void room_rand( void )
{
	RPROG_DATA *rprg;

	if ( rprg_list )
		for( rprg = rprg_list; rprg; rprg = rprg->next_in_list )
			if ( ( rprg->type & ROOM_RAND ) && rprg->owner->people )
				rprog_rand_trigger( rprg );
}

void room_time( void )
{
	RPROG_DATA *rprg;

	if ( rprg_list )
		for( rprg = rprg_list; rprg; rprg = rprg->next_in_list )
			// if ( ( rprg->type & ROOM_TIME ) && rprg->owner->people )
			if ( ( rprg->type & ROOM_TIME ) )
				rprog_time_trigger( rprg );
			
}

/*
 * Update the ban file upon call.
 * Written by Tre of EnvyMud and modified by Kahn
 */
void ban_update( void )
{
	FILE *fp;
	BAN_DATA *pban;

	if (fpReserve) {
		fclose( fpReserve );
		fpReserve = NULL;
	}

	if ( !( fp = fopen ( BAN_FILE, "w" ) ) )
	{
		bug( "Ban_update:  fopen of BAN_FILE failed", 0 );
		return;
	}

	for ( pban = ban_list; pban; pban = pban->next )
		fprintf( fp, "%s~\n", pban->name );

	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );

	return;

}

#ifdef R_AFFECT
void firewall_update(R_AFFECT_DATA *paf)
{
	CHAR_DATA *vch;
	for( vch = paf->owner->people; vch ; vch = vch->next_in_room){
		r_damage(paf->owner, vch, paf->modifier,skill_lookup("fire wall"));
	}
}

void raffect_update()
{
	R_AFFECT_DATA * paf;
	if( !r_affect_list ) return;

	for( paf = r_affect_list; paf; paf = paf->next_in_list ) {
		if( paf->duration == 0 )
			r_affect_remove(paf->owner, paf);
		else if( paf->duration > 0 )
			--paf->duration;

		if( paf->bitvector == R_AFF_FIREWALL)
			firewall_update(paf);
	}
}
#endif

/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
	static int pulse_area;
	static int pulse_mobile;
	static int pulse_violence;
	static int pulse_point;
	static int pulse_db_dump; /* OLC 1.1b */
	static int pulse_call_out;

	if ( --pulse_area   <= 0 )
	{
		pulse_area  = number_range( PULSE_AREA, 5 * PULSE_AREA / 4 );
		area_update ( );
	}

	if ( --pulse_call_out <= 0 )
	{
		pulse_call_out = number_range(PULSE_CALL_OUT,5*PULSE_VIOLENCE/4);
		call_out_update();
		rcall_out_update();
		rforce_update();
		time_update( );
	}

	if ( --pulse_violence <= 0 )
	{
		pulse_violence  = number_range(PULSE_VIOLENCE, 5*PULSE_VIOLENCE/4);
		violence_update ( );
	}

	if ( --pulse_mobile   <= 0 )
	{
		pulse_mobile  = number_range( PULSE_MOBILE, 5 * PULSE_MOBILE / 4 );
		mobile_update( );
		room_rand( );
		mob_action_update( );
	}

	if ( --pulse_point    <= 0 )
	{
		pulse_point = number_range( PULSE_TICK, 5 * PULSE_TICK / 4 );
		// �� tick bug �� 2023/01/14
		//bug( "tick bug check start", 0 );
		weather_update( );
		//bug( "weather_update end", 0 );
		char_update( );
		//bug( "char_update end", 0 );
		list_update( );
		//bug( "list_update end", 0 );
		qmark_update( );
		//bug( "qmark_update end", 0 );
		rprg_update( );
		//bug( "rprg_update end", 0 );
		room_time( );
		//bug( "room_time end", 0 );
		obj_update( );
		//bug( "tick bug check end", 0 );
	}

	/* OLC 1.1b */
	if ( --pulse_db_dump  <= 0 )
	{
		pulse_db_dump = PULSE_DB_DUMP;
		// do_asave( NULL, "" );
	}

	aggr_update( );
	tail_chain( );
	return;
}

void rforce_update()
{
	CHAR_DATA* ch;

	for ( ch = char_list; ch; ch = ch->next )
	{
		if( ch->forced )
			ch->forced = FALSE;
	}
	return;
}

void rcall_out_update()
{
	RCALL_OUT_DATA * cod, *cod_next;
	for( cod = rcall_out_list ; cod ; cod = cod_next ){
		cod_next = cod->next;
		if( !cod->owner ) {
			free_rcall_out( cod );
			continue;
		}
		if( --cod->timer <= 0 ) {
			rinterpret( cod->owner, NULL,cod->cmd);
			free_rcall_out(cod);
			continue;
		}
	}
}
void call_out_update()
{
	CALL_OUT_DATA * cod, *cod_next;
	for( cod = call_out_list ; cod ; cod = cod_next ){
		cod_next = cod->next;
		if( !cod->owner || cod->owner->deleted || !cod->owner->in_room ) {
			free_call_out( cod );
			continue;
		}
		if( --cod->timer <= 0 ) {
			if( !IS_AFFECTED( cod->owner, AFF_CHARM ) )
				interpret( cod->owner, cod->cmd);
			free_call_out(cod);
			continue;
		}
	}
}

