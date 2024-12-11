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
//*  PROGRAM ID  : act_obj.c                                      *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         �ק�eqbank open�p�wbank->open��        *
//*              :             �ˬd�u�Wppl�O�_���H�ϥ�            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940619 jye                                    *
//*  PURPOSE     :         �ק�eqbank list�i�M������r��eq name   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940825 Razgriz                                *
//*  PURPOSE     :         �� fwrite_neweqbank �� free_neweqbank  *
//*              :         �ŧi�b�ɮ׶}�Y�H�K�� close_socket �ϥ� *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 jye                                    *
//*  PURPOSE     :         �ק�do_give ��imm�L��ITEM_PERSONAL     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         �ק�P�_souvenir�Ȩϧ���ܼ�           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940926 Razgriz                                *
//*  PURPOSE     :         �ק� do_steal ���ఽ��b���W���F��     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941003 Jye                                    *
//*  PURPOSE     :         �ק�do_brandish �ư�imm char           *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"


/*
 * Local functions.
 */
#define CD CHAR_DATA
void get_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container ) );
void wear_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace, int iWear ) );
CD * find_keeper args( ( CHAR_DATA *ch, char *argument ) );
int get_cost args( ( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
#undef  CD
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing );

/*
 * Global functions and variables.  -- Razgriz
 */
//declaration for close_socket()	Added by Razgriz 20050825
void fwrite_neweqbank	args( ( CHAR_DATA * ch, FILE* fp ) );
void free_neweqbank	args( ( BANK_DATA * bank ) );

extern  int top_bank;
extern bool IsSorGateMob(int nMobIndex);

void free_neweqbank_obj( OBJ_DATA *obj )
{
    AFFECT_DATA    *paf;
    AFFECT_DATA    *paf_next;
    EXTRA_DESCR_DATA *ed;
    EXTRA_DESCR_DATA *ed_next;
    OBJ_HP *ohp, *ohp_next;

    for ( ed = obj->extra_descr; ed; ed = ed_next )
    {
  ed_next = ed->next;

  free_string( ed->description );
  free_string( ed->keyword     );
  ed->next     = extra_descr_free;
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
    bug( "[void free_neweqbank_obj]List_update: cannot find paf on obj.", 0 );
    continue;
      }
  }

  paf->next   = affect_free;
  affect_free = paf;
    }


    free_string( obj->name      );
    free_string( obj->description );
    free_string( obj->short_descr );
    /*        free_string( obj->cname     );*/
    --obj->pIndexData->count;

    obj->next = obj_free;
    obj_free  = obj;
}

// ���� eqb_do_examine �����O, �ò��� eqb_do_look �N��̲ΦX����@���O 2022/12/01
void eqb_do_examine( CHAR_DATA *ch, OBJ_DATA *obj )
{
	char buf[ MAX_STRING_LENGTH ];
	int diff;

	if ( !obj )
	{
		send_to_char( "[33m�n�ˬd����H[m\n\r", ch );
		return;
	}

	send_to_char( obj->short_descr , ch);
	send_to_char( " - ", ch);
	send_to_char( obj->material->short_descr, ch);
	send_to_char("\n\r", ch);
	send_to_char( obj->description, ch );
	send_to_char( "\n\r", ch );
	switch ( obj->item_type )
	{
		default:
			break;

		case ITEM_LIGHT:
			if( obj->value[2] != -1 ){
				sprintf(buf, "�ݨ��ٯ�ϥ� %d �p�ɡC\n\r", obj->value[2] );
				send_to_char(buf, ch);
			}
			break;

		case ITEM_ARMOR:
			send_to_char("�A�J�ӥ��q����A�o�{��", ch);
			if( obj->value[1] != 0)
				send_to_char("����F�G�k�A�~�[�ݰ_��", ch);

			if( indestructible(obj) ) {
				send_to_char(HIW"���B�b���������p�U�C"NOR"\n\r", ch);
				break;
			}else if ( get_max_dura(obj) == 0 ) {
				send_to_char(RED"�n���@�����a�N�L�k�״_�C"NOR"\n\r", ch);
				break;
			}

			diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

			if( diff == 100 ) send_to_char(HIW"���B�b���������p�U"NOR"�C\n\r", ch);
			else if( diff > 70) send_to_char(HIC"�ݰ_�Ӭ۷�}�n"NOR"�C\n\r", ch);
			else if( diff > 50) send_to_char(HIY"�y�L���Ǩ�"NOR"�C\n\r", ch);
			else if( diff > 33) send_to_char(YEL"���p���Ӧn"NOR"�C\n\r", ch);
			else if( diff > 20) send_to_char(HIR"�t���h���a�F"NOR"�C\n\r", ch);
			else if( diff > 10) send_to_char(RED"�W�Y������@�P�̰ʵ�"NOR"�C\n\r", ch);
			else if( diff > 0 ) send_to_char(RED"�n�������@�I�N�|����"NOR"�C\n\r", ch);
			else send_to_char(HIB"�w�g�����l�a�A�����ϥ�"NOR"�C\n\r", ch);

			break;

		case ITEM_WEAPON:
			send_to_char("�A�J�ӥ��q����A�o�{��", ch);
			if( obj->value[3] == 1 )
				send_to_char("��@���Q�b���ΡA�~�[�ݰ_��", ch);
			else if( obj->value[3] == 2 )
				send_to_char("��@�����j���A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 3 )
				send_to_char("��@���M�C�A��A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 4 )
				send_to_char("��@���@�l���R�A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 5 )
				send_to_char("��Ψӧ������A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 6 )
				send_to_char("��Q�X�]�k�A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 7 )
				send_to_char("��ΨӺV�����A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 8 )
				send_to_char("��@�����񯻸H���A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 9 )
				send_to_char("��@�������Q���A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 10 )
				send_to_char("��r��ͪ��A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 11 )
				send_to_char("��@���P�������A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 12 )
				send_to_char("��l��ͪ��A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 13 )
				send_to_char("��@����M�s�١A�~�[�ݰ_��", ch);
			else if( obj->value[3] == 14 )
				send_to_char("��@���}�ϥΡA�~�[�ݰ_��", ch);
			else if( obj->value[3] == 16 )
				send_to_char("��@���j��ϥΡA�~�[�ݰ_��", ch);

			if( indestructible(obj) ) {
				send_to_char(HIW"���B�b���������p�U"NOR"�C\n\r", ch);
				break;
			}else if ( get_max_dura(obj) == 0 ) {
				send_to_char(RED"�n���@�����a�N�L�k�״_"NOR"�C\n\r", ch);
				break;
			}

			diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

			if( diff == 100 ) send_to_char(HIW"���B�b���������p�U"NOR"�C\n\r", ch);
			else if( diff > 70) send_to_char(HIC"�ݰ_�Ӫ��p�n����"NOR"�C\n\r", ch);
			else if( diff > 50) send_to_char(HIY"�y�L���Ǩ�"NOR"�C\n\r", ch);
			else if( diff > 33) send_to_char(YEL"���p���Ӧn"NOR"�C\n\r", ch);
			else if( diff > 20) send_to_char(HIR"�t���h���a�F"NOR"�C\n\r", ch);
			else if( diff > 10) send_to_char(RED"�W�Y������@�P�̰ʵ�"NOR"�C\n\r", ch);
			else if( diff > 0 ) send_to_char(RED"�n�������@�I�N�|����"NOR"�C\n\r", ch);
			else send_to_char(HIB"�w�g�����l�a�A�����ϥ�"NOR"�C\n\r", ch);

			break;

		case ITEM_DRINK_CON:
			if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ){
				bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
				obj->value[2] = 0;
			}
			if ( obj->value[1] <= 0 && obj->value[1] != -1 ){
				send_to_char( "[36m���O�Ū��C[m\n\r", ch );
				break;
			}
			sprintf( buf, "���̭���%s%s�G��C\n\r",
			(obj->value[1] == -1) 
			? "�p�P�p�t�z�륨�q��" : 
			obj->value[1] <     obj->value[0] / 4 
			? "����@�b��" : 
			obj->value[1] < 3 * obj->value[0] / 4 
			? "�j���@�b��" :       "�W�L�@�b��",
			liq_table[obj->value[2]].liq_color);
			send_to_char( buf, ch );
			break;

		case ITEM_FOUNTAIN:
			if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ){
				bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
				obj->value[2] = 0;
			}
			if ( obj->value[1] <= 0 && obj->value[1] != -1 ){
				send_to_char( "[36m���O�����C[m\n\r", ch );
				break;
			}
			sprintf( buf, "���̭��O%s%s�C\n\r",
				(obj->value[1] == -1) 
				? "����������" : 
				obj->value[1] <     obj->value[0] / 4 
				? "������" : 
				obj->value[1] < 3 * obj->value[0] / 4 
				? "�w�w�y�ʪ�" :       "�F�F��",
				liq_table[obj->value[2]].liq_name);
				send_to_char( buf, ch );
			break;

		case ITEM_CONTAINER:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
			if ( IS_SET( obj->value[1], CONT_CLOSED ) ){
				send_to_char( "[33m���O���۪��C[m\n\r", ch );
				break;
			}
			act( "$p�̭����G", ch, obj, NULL, TO_CHAR );
			show_list_to_char( obj->contains, ch, TRUE, TRUE );
			break;

		case ITEM_ARROW:
			if ( obj->value[0] <= 0 ){
				send_to_char( "[36m���O�Ū��C[m\n\r", ch );
				break;
			}
			sprintf( buf, "���̭��٦�%d��b�C\n\r", obj->value[0] );
			send_to_char( buf, ch );
			break;

		case ITEM_AMMO:
			if ( obj->value[0] <= 0 ){
				send_to_char( "[36m���O�Ū��C[m\n\r", ch );
				break;
			}
			sprintf( buf, "�٦�%d�o�u�ġC\n\r", obj->value[0] );
			send_to_char( buf, ch );
			break;

		case ITEM_DART:
			sprintf( buf, "�٦�%d��D��C\n\r", obj->value[0] );
			send_to_char( buf, ch );
			break;
	}

	return;
}

/*  ���s�s�g eqb_do_examine �ò��� eqb_do_look 2022/12/01
void eqb_do_examine( CHAR_DATA *ch, OBJ_DATA *obj )
{
	int diff;

	if ( !obj )
	{
		send_to_char( "[33m�n�ˬd����H[m\n\r", ch );
		return;
	}


	switch ( obj->item_type )
	{
		default:
			break;

		case ITEM_ARMOR:
			eqb_do_look( ch, obj );
			if( obj->value[1]  != 0)
				send_to_char("�ݨӦn������G�k�F�C\n\r", ch);
			break;

		case ITEM_WEAPON:
			eqb_do_look( ch, obj );
			send_to_char("�A�J�ӥ��q����A�o�{��", ch);
			if( indestructible(obj) ) {
				send_to_char(HIW"���B�b���������p�U�C"NOR"\n\r", ch);
				break;
			}
			else if ( get_max_dura(obj) == 0 ) {
				send_to_char(RED"�n���@�����a�N�L�k�״_�C"NOR"\n\r", ch);
				break;
			}
			diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

			if( diff == 100 ) send_to_char(HIW"���B�b���������p�U�C"NOR"\n\r", ch);
			else if( diff > 90) send_to_char(HIC"�ݰ_�Ӫ��p�n���ܡC"NOR"\n\r", ch);
			else if( diff > 70) send_to_char(HIY"�y�L���Ǩ��C"NOR"\n\r", ch);
			else if( diff > 40) send_to_char(YEL "���p���Ӧn�C"NOR"\n\r", ch);
			else if( diff > 10) send_to_char(HIR"�t���h���a�F�C"NOR"\n\r", ch);
			else if( diff > 0 ) send_to_char(RED"�n�������@�I�N�|����C"NOR"\n\r", ch);
			else send_to_char(HIB"�����l�a�A�����ϥΡC"NOR"\n\r", ch);
			break;

		case ITEM_DRINK_CON:
		case ITEM_CONTAINER:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
		eqb_do_look( ch, obj );
	}

    return;
}

void eqb_do_look( CHAR_DATA *ch, OBJ_DATA *obj )
{
	char buf  [ MAX_STRING_LENGTH ];


	if ( !( obj  ) )
	{
		send_to_char( "[33m�A�b�o�̨S�ݨ쨺���N��C[m\n\r", ch );
		return;
	}


	send_to_char( obj->short_descr , ch);
	send_to_char( " - ", ch);
	send_to_char( obj->material->short_descr, ch);
	send_to_char("\n\r", ch);
	send_to_char( obj->description, ch );
	send_to_char( "\n\r", ch );
	switch ( obj->item_type )
	{
		default:
			break;

		case ITEM_DRINK_CON:
			send_to_char( "�A���̭��ݪ��ɭԡA�A�ݨ�G\n\r", ch );
			if ( obj->value[1] <= 0 )
			{
				send_to_char( "[36m���O�Ū��C[m\n\r", ch );
				break;
			}

			sprintf( buf, "���̭���%s%s�G��C\n\r",
			obj->value[1] <     obj->value[0] / 4
			? "����@�b��" :
			obj->value[1] < 3 * obj->value[0] / 4
			? "�j���@�b��"     : "�W�L�@�b��",
			liq_table[obj->value[2]].liq_color
			);

			send_to_char( buf, ch );
			break;

		case ITEM_CONTAINER:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
			if ( IS_SET( obj->value[1], CONT_CLOSED ) )
			{
				send_to_char( "[33m���O���۪��C[m\n\r", ch );
				break;
			}

			send_to_char( "�A���̭��ݪ��ɭԡA�A�ݨ�G\n\r", ch );
			act( "$p�̭����G", ch, obj, NULL, TO_CHAR );
			show_list_to_char( obj->contains, ch, TRUE, TRUE );
			break;

		case ITEM_ARROW:
			send_to_char( "�A���̭��ݪ��ɭԡA�A�ݨ�G\n\r", ch );
			if ( obj->value[0] <= 0 )
			{
				send_to_char( "[36m���O�Ū��C[m\n\r", ch );
				break;
			}

			sprintf( buf, "���̭��٦�%d��b�C\n\r", obj->value[0] );

			send_to_char( buf, ch );
			break;
	}
	return;
} */

void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
	if ( !CAN_WEAR( obj, ITEM_TAKE ) && obj->item_type != ITEM_CORPSE_PC)
	{
		send_to_char( "�A�L�k���_.\n\r", ch );
		return;
	}
	else if ( obj->item_type == ITEM_CORPSE_PC ) {
			if( !consented(ch, obj)){
				send_to_char( "�A�L�k���_.\n\r", ch );
				return;
			}
			/*  if( obj->in_room) //add by coconet for consent
			  {
				obj->area = obj->in_room->area;
				bug( "test consent: obj %d " ,obj->area->name);
			  } 
			  else
				return;*/
	}
	if( IS_SET(obj->extra_flags, ITEM_UNIQUE) && IS_NPC(ch)){
		send_to_char("�A�L�k���_.\n\r", ch);
		return;
	}

	if ( !( obj->item_type == ITEM_MONEY ) && ( !container || container->carried_by != ch ))
	{
		if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
		{
			act( "$d: �A�L�k������h�F��.", ch, NULL, obj->name, TO_CHAR );
			return;
		}

		if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
		{
			act( "$d: �A�����ʤF.", ch, NULL, obj->name, TO_CHAR );
			return;
		}
	}

	// ��ԭz�P���~���ʤ��} , �] obj_act �i��]�t purge 2022/02/12
	if ( container )
	{
		//modify by coco 0511 for horse
		if( container->item_type == ITEM_HORSE )
		{
			act( "�A�q $P �����U $p.", ch, obj, container, TO_CHAR );
			act( "$n �q $P �����U $p.", ch, obj, container, TO_ROOM );
			//obj_from_obj( obj );
		}
		else 
		{
			act( "�A�q $P ���X $p.", ch, obj, container, TO_CHAR );
			act( "$n �q $P ���X $p.", ch, obj, container, TO_ROOM );
			//obj_from_obj( obj );
		}
	}
	else
	{
		act( "�A���_ $p.", ch, obj, container, TO_CHAR );
		act( "$n ���_ $p.", ch, obj, container, TO_ROOM );
		//obj_from_room( obj );
	}

	if ( container ){
		obj_from_obj( obj );
	}else{
		obj_from_room( obj ); //remove obj from in_room->contents list
	}

	// holy evil �N�˧P�w
	if ( IS_OBJ_STAT( obj, ITEM_HOLY  ) && IS_EVIL(ch))
	{
		act( "�A�Q $p �W�����t�O�q���`�ˤF�I", ch, obj, NULL, TO_CHAR );
		act( "$n �Q $p �W�����t�O�q���`�ˤF�I", ch, obj, NULL, TO_ROOM );
		damage( ch, ch, 50, gsn_burning_hands, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}
	else if ( IS_OBJ_STAT( obj, ITEM_EVIL) && IS_GOOD(ch))
	{
		act( "�A�Q $p �W�����c�O�q���`�ˤF�I", ch, obj, NULL, TO_CHAR );
		act( "$n �Q $p �W�����c�O�q���`�ˤF�I", ch, obj, NULL, TO_ROOM );
		damage( ch, ch, 50, gsn_burning_hands, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}

	// obj type �� item_moeny �ɪ��P�w
	if ( obj->item_type == ITEM_MONEY )
	{
		char buf [ MAX_STRING_LENGTH ];
		int  amount;

		amount = obj->value[0];
		ch->gold += amount;

		if ( amount > 1 )
		{
			sprintf( buf, "�A���X�F %d �T����.\n\r", amount );
			send_to_char( buf, ch );
		}
		// add at 2022/02/13
		obj_to_char( obj, ch );
		// obj_act get 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("get", ch, obj);
		}
		if( obj->deleted )
			;
		else
			extract_obj( obj );/*remove money obj from room/pc/container...*/
	}
	else
	{
		obj_to_char( obj, ch );/*add obj to char->carrying list*/
	}

	// pc corpse
	if( container && container->item_type == ITEM_CORPSE_PC && !container->contains )
	{
		act( "$p �����F.", ch, container, container, TO_CHAR );
		act( "$p �����F.", ch, container, container, TO_ROOM );
		extract_obj(container);
		save_char_obj(ch);
		//container->timer /= 3;
	}

	// obj_act get 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("get", ch, obj);
	}

	return;
}


int do_autoget( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *container;
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	bool found;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	/* Get type. */
	if ( arg1[0] == '\0' )
	{
		send_to_char( "�n������?\n\r", ch );
		return 0;
	}

	if ( arg2[0] == '\0' )
	{
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj' */
			obj = get_obj_list( ch, arg1, ch->in_room->contents );
			if ( !obj )
			{
				act( "�o�̨S�� $T.", ch, NULL, arg1, TO_CHAR );
				return 0;
			}

			get_obj( ch, obj, NULL );
		}
		else
		{
			/* 'get all' or 'get all.obj' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = ch->in_room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, NULL );
				}
			}

			if ( !found ) 
			{
				if ( arg1[3] == '\0' )
					send_to_char( "�o�̨S�����󪫫~.\n\r", ch );
				else
					act( "�o�̨S�� $T.", ch, NULL, &arg1[4], TO_CHAR );
			}
		}
	}
	else
	{
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		{
			send_to_char( "�A�L�k�o��.\n\r", ch );
			return 0;
		}

		if ( !( container = get_obj_here( ch, arg2 ) ) )
		{
			act( "�o�̨S�� $T.", ch, NULL, arg2, TO_CHAR );
			return 0;
		}

		switch ( container->item_type )
		{
			default:
				send_to_char( "�����O�Ӯe��.\n\r", ch );
				return 0;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
				break ;

			case ITEM_CORPSE_PC:
			{
				CHAR_DATA *gch;
				char  *pd, *cd;
				char   name [ MAX_INPUT_LENGTH ];
				char   chname [ MAX_INPUT_LENGTH ];

				if ( IS_NPC( ch ) )
				{
					send_to_char( "�A�L�k�o��.\n\r", ch );
					return 0;
				}

				pd = container->short_descr;/*short_descr;
				pd = one_argument( pd, name );
				pd = one_argument( pd, name );*/
				cd = ch->name;
				cd = one_argument( cd, chname );

				if ( str_cmp( name, chname ) && !IS_IMMORTAL( ch ) )
				{
					bool fGroup;

					fGroup = FALSE;
					for ( gch = char_list; gch; gch = gch->next )
					{
						if ( !IS_NPC( gch ) && is_same_group( ch, gch ) && !str_cmp( name, gch->name ) )
						{
							fGroup = TRUE;
							break;
						}
					}
					if ( !fGroup )
					{
						send_to_char( "�A�L�k�o��.\n\r", ch );
						return 0;
					}
				}
			}
		}

		if ( container->item_type == ITEM_CONTAINER && IS_SET( container->value[1], CONT_CLOSED ) )
		{
			act( "$d �O���۪�.", ch, NULL, container->name, TO_CHAR );
			return 0;
		}

		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			int value;
			/* 'get obj container' */
			obj = get_obj_list( ch, arg1, container->contains );
			if ( !obj )
			{
				// auto get corpse �S�F�褣�|��ܦr�� 2022/05/08
				//act( "$T �̨S���o�˪F��.", ch, NULL, arg2, TO_CHAR );
				return 0;
			}
			value = obj->value[0];
			get_obj( ch, obj, container );
			return value;
		}
		else
		{
			/* 'get all container' or 'get all.obj container' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = container->contains; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, container );
				}
			}
			/*  auto get corpse �S�F�褣�|��ܦr�� 2022/05/08
			if ( !found )
			{
				if ( arg1[3] == '\0' )
					act( "$T �̨S���o�˪F��.", ch, NULL, arg2, TO_CHAR );
				else
					act( "$T �̨S���o�˪F��.", ch, NULL, arg2, TO_CHAR );
			} */
		}
	}

	return 0;
}

BANK_DATA * new_bank(){
    BANK_DATA * bank = NULL;

    if( !eqbank_free ) {
        bank = alloc_mem( sizeof( *bank));

        ++top_bank;
    }
    else {
        bank = eqbank_free;
        eqbank_free = bank->next;
    }
    bank->name = NULL;
    bank->next = NULL;
    bank->password = NULL;
    bank->objs = NULL;
    bank->amount = 0;
    bank->opened = TRUE;
    return bank;
}

void fwrite_neweqbank( CHAR_DATA * ch, FILE* fp )
{
    
    if( !ch->neweqbank ) return;



    fprintf(fp,"#EQBANK_DATA\n");
    fprintf(fp, "%s~\n", ch->neweqbank->name);
    fprintf(fp, "%s~\n", ch->neweqbank->password);
    fprintf(fp, "%d\t", ch->neweqbank->amount);
    fprintf(fp, "%d\n", ch->neweqbank->opened);
    if( ch->neweqbank->objs )
  fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
    fprintf(fp,"#END\n");
    return;
}

int fread_neweqbank( BANK_DATA * ch, FILE* fp){

    char letter;
    char *word;
    int status;
    for ( ;; ) {
  letter = fread_letter(fp);
  if( letter != '#' ) {
      bug("[read_neweqbank] : # not found", 0);
      return FALSE;
  }
  word = fread_word(fp, &status);

  if( !str_cmp(word, "EQBANK_DATA")){
      ch->name = fread_string(fp, &status);
      ch->password = fread_string(fp, &status);
      ch->amount = fread_number(fp, &status);
      ch->opened = fread_number(fp, &status);
      continue;
  }
  else if( !str_cmp(word, "EQBANK_OBJECT")){

      if( !fread_newbank_obj( ch, fp )) return FALSE;
      continue;
  }
  else if( !str_cmp(word, "END")){
      break;
  }
  else
  {
      bug("[read_neweqbank]: unknown section name.",0);
      return FALSE;
  }
  
    }

    return TRUE;
}

bool check_parse_name( char *name );
void free_neweqbank( BANK_DATA * bank )
{
    OBJ_DATA * obj, *nobj;
    free_string(bank->name);
    free_string(bank->password);
    for( obj = bank->objs ; obj ; obj = nobj ){
        nobj = obj->next_content;
        obj->deleted = TRUE;
        obj->next = object_list;
        object_list = obj;
        obj->next_content = NULL;
    }
    /*
    for( obj = bank->objs ; obj; obj = nobj){
  if( obj->contains ) {
      OBJ_DATA * nnobj;
      for(nobj = obj->contains; nobj; nobj = nnobj ){
    nnobj = nobj->next_content;
    nobj->deleted = TRUE;
    nobj->next_content = NULL;
    free_neweqbank_obj( nobj );
      }
  }
  nobj = obj->next_content;
  obj->deleted = TRUE;
  obj->next_content = NULL;
  free_neweqbank_obj( obj );
    }*/
    bank->name = NULL;
    bank->password = NULL;
    bank->objs = NULL;
    bank->next = eqbank_free;
    eqbank_free = bank;
    return;
}

bool check_container( CHAR_DATA *ch, OBJ_DATA *container)
{
	OBJ_DATA *obj;
	int O_Weight = 0; // 2022/12/01
	int O_Number = 0; // 2022/12/01


	for( obj = container->contains; obj ; obj = obj->next_content)
	{
		// ���e���� container �]�� container �N�L�k�O�s 2022/12/01
		if( obj->item_type == ITEM_CONTAINER && !( obj->contains == NULL ) )
		{
			send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�ڭ̤����e�������e���̭��٩�۪��~���e���C�v\n\r", ch );
			return FALSE;
		}
		// container �����e���i�H�O�s key �Φ� timer �� eq , �[�J�L�k�s�J horse 2022/12/01
		if ( ( ch->level < ( obj->level - 5 ) && ch->class < 5 )
			//|| obj->item_type == ITEM_KEY
			|| obj->item_type == ITEM_HORSE
			|| IS_SET( obj->extra_flags, ITEM_NOSAVE )
			|| IS_SET( obj->extra_flags, ITEM_PERSONAL )
			|| IS_SET( obj->extra_flags, ITEM_UNIQUE )
			|| obj->deleted )
			//|| obj->timer > 0 )
		{
			send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�o�e������ۧڭ̤��������~�C�v\n\r", ch );
			return FALSE;
		}
		if( obj->weight > 0 )
			O_Weight += obj->weight;
		++O_Number;
	}

	// �קK�L�k���X, �W�[�`���q�Ϊ��~�ƭp�� 2022/12/01
	if( container->value[3] > 100 )
		O_Weight = (O_Weight * container->value[3]) / 100;
	else if( container->value[3] == 100 )
		O_Weight = O_Weight - 1;
	else if( container->value[3] > 0 )
		O_Weight = ( O_Weight * ( 100 - container->value[3] ) ) / 100;
	else if( container->value[3] < 0 )
		O_Number = O_Number + container->value[3];

	if( O_Weight > 800 ){
		send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�o��e�����`���q�W�L 800 ����C�v\n\r", ch );
		return FALSE;
	}

	if( O_Number > 90 ){
		send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�o��e�������~�ƶq�W�L 90 ��C�v\n\r", ch );
		return FALSE;
	}

	return TRUE;
}

bool check_eqbank( BANK_DATA * bank )
{
	DESCRIPTOR_DATA *d;
	CHAR_DATA *to;

	for( d = descriptor_list; d; d = d->next ) 
	{
		to = (d->original) ? d->original : d->character;
		if( !to ) continue;
		if( to->neweqbank )
		{
			if( !str_cmp( to->neweqbank->name , bank->name ))
				return TRUE;
		}
	}

	return FALSE;
}
void do_eqbank( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	char out_buf [ MAX_STRING_LENGTH * 12 ];
	char tmp[255];
	char no_ansi[255];
	CHAR_DATA * vch = NULL;
	BANK_DATA * bank = NULL;
	OBJ_DATA * obj=NULL;
	OBJ_DATA * vobj=NULL;
	bool fBank = TRUE;
	int count = 0;
	FILE * fp = NULL;
	int i = 0;
	int cost = 0;

	argument = one_argument( argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);

	if(IS_NPC(ch)) return;

	if( !ch->in_room || !ch->in_room->area ) return;

	if( IS_SET(ch->act, PLR_HORSE)) return;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
	{
		// �[�J npc ���w, �íק� eqbanker ���P�w�� name �ܧ� qmark 2022/12/01
		if( !IS_NPC(vch) ) continue;
		//if ( is_name( "sharas-eqb", vch->name ) )
		if( is_qmark(vch, "is_eqbanker") )
			break;
	}

	if ( !vch ){
		send_to_char( "�˳ƻȦ���󤣦b�o��C\n\r", ch );
		return;
	}

	if( arg1[0] == '\0' ){
		send_to_char("�� help eqbank �H���o�Ϊk�����C\n\r", ch);
		return;
	}

	if( !str_cmp(arg1, "passwd" ))
	{
		if( !ch->neweqbank ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�z�o�����}�ܮw�C�v\n\r", ch );
			return;
		}

		if( str_cmp(arg2, arg3) ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�z�⦸���K�X���@�ˡC�v\n\r", ch );
			return;
		}

		free_string( ch->neweqbank->password );
		ch->neweqbank->password = str_dup( crypt(arg3,ch->neweqbank->name));
		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));

		if(( fp = fopen(tmp, "w"))){
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}

		send_to_char( "�˳ƻȦ���󻡵ۡG�u�K�X�]�w�����C�v\n\r", ch );
		return;
	}

	if( !str_cmp( arg1, "new"))
	{
		if( ch->neweqbank ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�z�o����ϥΤ����ܮw������(close)�C�v\n\r", ch );
			return;
		}

		if( strlen( arg1 ) > 20 ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�z�s�ܮw���W�r�Ӫ��F�C�v\n\r", ch );
			return;
		}

		if( !check_parse_name(arg2)){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u���O���X�k���W�r�C�v\n\r", ch );
			return;
		}

		if( arg2[0] == '\0' || arg3[0] == '\0'){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�z�o�i�D�ڷs�ܮw���W�r�M�K�X�ڡC�v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2 ), capitalize(arg2 ));

		if( ( fp = fopen( tmp, "r" ))){
			fclose(fp);
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�u��p�A�o�ӦW�r�w�g���H�ΤF�C�v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s.gz", PLAYER_DIR,initial( arg2 ), capitalize(arg2 ));

		if( ( fp = fopen( tmp, "r")))
		{
			fclose(fp);
			if( str_cmp( arg2, ch->name )){
				send_to_char( "�˳ƻȦ����K�ܻ۬��G�u��p�A�Ф��n�Ψ�L���a���W�r�C�v\n\r", ch );
				return;
			}
		}

		sprintf(tmp, "%s%s/%s", PLAYER_DIR,initial( arg2 ), capitalize(arg2 ));

		if( ( fp = fopen( tmp, "r")))
		{
			fclose(fp);
			if( str_cmp( arg2, ch->name )){
				send_to_char( "�˳ƻȦ����K�ܻ۬��G�u��p�A�Ф��n�Ψ�L���a���W�r�C�v\n\r", ch );
				return;
			}
		}

		cost = 500000;
		fBank = TRUE;

		if( ch->bank < cost ){
			fBank = FALSE;
			if( ch->gold < cost){
				send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�u��p�A�A���������C�v\n\r", ch );
				return;
			}
			ch->gold -= cost;
		}else{
			if( ch->bank < cost){
				send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�u��p�A�A���������C�v\n\r", ch );
				return;
			}
			ch->bank -= cost;
		}

		bank = new_bank();
		bank->name = str_dup(arg2);
		bank->password = str_dup( crypt(arg3,arg2));
		bank->opened = TRUE;
		ch->neweqbank = bank;

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));

		if(( fp = fopen(tmp, "w"))){
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}

		sprintf(tmp, "%s opened new storage '%s'", ch->name, arg2);
		bug(tmp, 0);
		send_to_char( "�˳ƻȦ���󻡵ۡG�u�}�᧹���C�v\n\r", ch );
		if( fBank )
			send_to_char("�A�q�Ȧ���b�A�I�X�F "HIY, ch);
		else
			send_to_char("�A�I�X�F "HIY, ch);
		sprintf(tmp, "%d", cost);
		send_to_char(tmp, ch);
		send_to_char(NOR " �����}��O�C\n\r", ch);
		return;
	}

	if( !str_cmp(arg1, "backup"))
	{
		if( !ch->neweqbank ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�·нХ����}�ӭܮw�C�v\n\r", ch );
			return;
		}
		sprintf(tmp, "%s%s/%s", EQBANK_BACKUPDIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
		ch->neweqbank->opened = FALSE;
		if(( fp = fopen(tmp, "w"))){
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
			send_to_char( "�˳ƻȦ���󻡵ۡG�u�ܮw�ƥ������C�v\n\r", ch );
		}else{
			bug("eqbank: file open error!!",0);
			send_to_char("�o���Y�����D�A�ЦV�j���^���C\n\r", ch);
			return;
		}
		ch->neweqbank->opened = TRUE;
		return;
	}

	if( !str_prefix(arg1, "store"))
	{
		if( !ch->neweqbank ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�o�����}�ӭܮw�C�v\n\r", ch );
			return;
		}
		if( arg2[0] == '\0' ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�n�s�񤰻�˳ơH�v\n\r", ch );
			return;
		}

		if ( ( obj = get_obj_carry( ch, arg2 ) ) )
		{
			cost = ( obj->level < 5 ) ? 8000 : ( obj->level < 36 ) ? obj->level * 200 : obj->level * 500;

			if( obj->item_type == ITEM_CONTAINER )
			{
				cost = (10000 * (get_obj_number(obj) + 1 ));
			}

			if( obj->timer > 0) cost *= 10;

			if ( ch->bank < cost )
			{
				fBank = FALSE;
				if( ch->gold < cost ){
					send_to_char( "�˳ƻȦ����W���ۻ��G�u�藍�_�A�S���������O�޶O�ڵL�k���ΫO���H���C�v\n\r", ch );
					return;
				}
			}

			if (ch->neweqbank->amount > 150){
				send_to_char( "�˳ƻȦ����W���ۻ��G�u�藍�_�A�z�w�s���˳Ƽƶq�w�F 150 �󪺤W���C�v\n\r", ch );
				return;
			}

			// ��@���~���q�W�� 2022/12/01
			if( obj->weight > 800 ){
				send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�C��˳ƪ����q����W�L 800 ����C�v\n\r", ch );
				return;
			}

			// ���� item_key ����s, �[�Jitem_horse ����s 2022/12/01
			if ( ( ch->level < ( obj->level - 5 ) && ch->class < 5 )
				//|| obj->item_type == ITEM_KEY
				|| obj->item_type == ITEM_HORSE
				|| IS_SET( obj->extra_flags, ITEM_NOSAVE )
				|| IS_SET( obj->extra_flags, ITEM_PERSONAL)
				|| IS_SET( obj->extra_flags, ITEM_UNIQUE)
				|| obj->deleted )
				//|| obj->timer > 0 )
			{
				send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�o���˳Ƨڭ̤����C�v\n\r", ch );
				return;
			}

			if( obj->item_type == ITEM_CONTAINER && !check_container(ch, obj))
				return;

			obj_from_char( obj );
			obj->value[4] = ch->in_room->area->recall;
			if( obj->item_type == ITEM_CONTAINER )
			{
				OBJ_DATA * vobj_next = NULL;
				for( vobj = object_list ; vobj ; vobj = vobj_next )
				{
					if( !object_list->next || !vobj->next ) break;
					//if( object_list->next == object_list ) break;
					if( vobj->next == vobj ) break;

					if( object_list == obj ){
						object_list->value[4] = ch->in_room->area->recall;
						vobj_next = obj->next;
						object_list = obj->next;
						obj->next = NULL;
						continue;
					}
					if( object_list->in_obj == obj ){
						object_list->value[4] = ch->in_room->area->recall;
						vobj_next = object_list->next;
						object_list->next = NULL;
						object_list = vobj_next;
						continue;
					}
					if( vobj->next == obj ){
						vobj->next->value[4] = ch->in_room->area->recall;
						vobj_next = vobj;
						vobj->next = obj->next;
						obj->next = NULL;
						continue;
					}
					if( vobj->next->in_obj == obj ){
						vobj->next->value[4] = ch->in_room->area->recall;
						vobj_next = vobj->next;
						vobj->next = vobj->next->next;
						vobj_next->next = NULL;
						vobj_next = vobj;
						continue;
					}
					vobj_next = vobj->next;
				}
			}
			else
			{
				if ( obj == object_list )
				{
					object_list = obj->next;
					obj->next = NULL; 
				}
				else
				{
					for ( vobj = object_list; vobj; vobj = vobj->next )
					{
						if ( obj == vobj->next )
						{
							vobj->next = obj->next;
							obj->next = NULL;
							break;
						}
					}
				}
			}

			obj->next_content = ch->neweqbank->objs;
			ch->neweqbank->objs = obj;

			if( fBank )
				ch->bank -= cost;
			else
				ch->gold -= cost;

			// ���� to_room �קK�j�q�s�����~�y���L�H�e����� 2022/02/01
			//act( "�˳ƻȦ����p�ߦa�N$n��$p����᭱���O�I�w�s��C", ch, obj, NULL, TO_ROOM );
			act( "�˳ƻȦ����p�ߦa�N�A��$p����᭱���O�I�w�s��C", ch, obj, NULL, TO_CHAR );

			if( fBank )
				send_to_char("�A�q�Ȧ���b�A�I�X�F "HIY, ch);
			else
				send_to_char("�A�I�X�F "HIY, ch);
			sprintf(tmp, "%d", cost);
			send_to_char(tmp, ch);
			send_to_char(NOR " �����O�޶O�C\n\r", ch);
			do_save( ch, "" );
			ch->neweqbank->amount += 1;
			sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
			if(( fp = fopen(tmp, "w"))){
				//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
				fwrite_neweqbank(ch, fp);
				fclose(fp);
				return;
			}else{
				bug("eqbank: file open error.",0);
				return;
			}
			return;
		}
		send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�ڨS�ݨ�o���˳ưڡH�v\n\r", ch );
		return;
	}

	if( !str_cmp(arg1, "fopen"))
	{
		if( ch->neweqbank ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�·нХ����W�ϥΤ����ܮw�C�v\n\r", ch );
			return;
		}
		if( get_trust(ch) < 43 ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�藍�_�A�z�����Ť����C�v\n\r", ch );
			return;
		}
		if( arg2[0] == '\0' ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�Чi�D�ڭܮw�W�١C�v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
		bank = new_bank();

		if( (fp= fopen(tmp, "r")))
		{
			fread_neweqbank(bank, fp); 
			fclose(fp);
		}else{
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�S���o�ӭܮw�@�C�v\n\r", ch );
			return;
		}
		ch->neweqbank = bank;
		bank->opened = TRUE;
		if(( fp = fopen(tmp, "w")))
		{
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}else{
			bug("eqbank: file open error.",0);
		}
		send_to_char( "�˳ƻȦ���󻡵ۡG�u�}�Ҧ��\\�C�v\n\r", ch );
		return;
	}

	/*if( !str_cmp(arg1, "forceopen")){
	if( ch->neweqbank ) {
	send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�·нХ����W�ϥΤ����ܮw�C�v\n\r", ch );
	return;
	}
	if( arg2[0] == '\0' || arg3[0] == '\0' ){
	send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�Чi�D�ڭܮw�W�٩M�K�X�C�v\n\r", ch );
	return;
	}
	sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
	bank = new_bank();
	if( (fp= fopen(tmp, "r"))){
	fread_neweqbank(bank, fp); 
	fclose(fp);
	}
	else {
	send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�S���o�ӭܮw�@�C�v\n\r", ch );
	return;
	}
	if( str_cmp( bank->password, crypt(arg3, arg2))){
	send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�T�w�K�X�O�諸�ܡH�C�v\n\r", ch );
	return;
	}
	ch->neweqbank = bank;
	bank->opened = TRUE;
	bugf("%s opened bank [%s]", ch->name, arg2);
	bank->recall = ch->in_room->area->recall;
	if(( fp = fopen(tmp, "w"))){
	//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
	fwrite_neweqbank(ch, fp);
	fclose(fp);
	}
	else{
	bug("eqbank: file open error.",0);
	}
	send_to_char( "�˳ƻȦ���󻡵ۡG�u�}�Ҧ��\\�C�v\n\r", ch );
	return;

	}*/

	if( !str_cmp(arg1, "open"))
	{
		if( ch->neweqbank ){
			send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�·нХ����W�ϥΤ����ܮw�C�v\n\r", ch );
			return;
		}
		if( arg2[0] == '\0' || arg3[0] == '\0' ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�Чi�D�ڭܮw�W�٩M�K�X�C�v\n\r", ch );
			return;
		}
		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
		bank = new_bank();
		if( (fp= fopen(tmp, "r"))){
			fread_neweqbank(bank, fp); 
			fclose(fp);
		}else{
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�S���o�ӭܮw�@�C�v\n\r", ch );
			return;
		}
		if( str_cmp( bank->password, crypt(arg3, arg2))){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�T�w�K�X�O�諸�ܡH�C�v\n\r", ch );
			free_neweqbank(bank);
			return;
		}
		if( bank->opened ) 
		{
			if( check_eqbank( bank ) ){
				send_to_char( "�˳ƻȦ���󻡵ۡG�u��p�A�o�ӭܮw�ثe���O�H�b�ϥΤ��C�v\n\r", ch );
				free_neweqbank(bank);
				return;
			}
		}
		ch->neweqbank = bank;
		bank->opened = TRUE;
		bank->recall = ch->in_room->area->recall;
		if(( fp = fopen(tmp, "w"))){
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}else{
			bug("eqbank: file open error.",0);
		}
		send_to_char( "�˳ƻȦ���󻡵ۡG�u�}�Ҧ��\\�C�v\n\r", ch );
		return;
	}

	if( !str_prefix(arg1, "exam")) // ���� eqbank exam �����O 2022/12/01
	{
		if( !ch->neweqbank ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�o�����}�ӭܮw�C�v\n\r", ch );
			return;
		}
		if ( !is_number( arg2 ) ){
			send_to_char( "�Х� 'eqbank exam �s��' ���˵��z���˳ơC\n\r", ch );
			return;
		}
		if( !ch->neweqbank->objs ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z���ܮw�̨S���F��ڡH�v\n\r", ch );
			return;
		}

		count = atoi( arg2);

		i = 1;

		if( count == 1 && ch->neweqbank->objs->value[4] == ch->in_room->area->recall )
		{
			obj = ch->neweqbank->objs;
		}
		else if ( ch->neweqbank->objs->next_content )//&& ch->neweqbank->objs->next_content )
		{
			if( ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
				++i;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj->next_content )
			{
				if( !vobj->next_content ) break;
				if( vobj->next_content->value[4] != ch->in_room->area->recall ) continue;
				if( count == i )
				{
					if( vobj->next_content->value[4] == ch->in_room->area->recall )
					{
						obj = vobj->next_content;
						break;
					}
					else
						continue;
				}
				i++;
			}
		}
		if ( obj )
		{
			eqb_do_examine(ch, obj);
			return;
		}
		send_to_char( "�˳ƻȦ����d�\\�F�d�O�W���n�Oï���G�_�ǡA�S���o���˳ƪ�������I\n\r", ch );
		return;
	}

	if( !str_prefix(arg1, "id"))
	{
		if( !ch->neweqbank ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�o�����}�ӭܮw�C�v\n\r", ch );
			return;
		}
		if ( !is_number( arg2 ) ){
			send_to_char( "�Х� 'eqbank id �s��' ��Ų�w�z���˳ơC\n\r", ch );
			return;
		}
		if( !ch->neweqbank->objs ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z���ܮw�̨S���F��ڡH�v\n\r", ch );
			return;
		}

		count = atoi( arg2);

		i = 1;
		if( count == 1 && ch->neweqbank->objs->value[4] == ch->in_room->area->recall )
		{
			obj = ch->neweqbank->objs;
		}
		else if ( ch->neweqbank->objs->next_content )//&& ch->neweqbank->objs->next_content )
		{
			if( ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
				++i;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj->next_content )
			{
				if( !vobj->next_content ) break;
				if( vobj->next_content->value[4] != ch->in_room->area->recall ) continue;
				if( count == i )
				{
					if( vobj->next_content->value[4] == ch->in_room->area->recall )
					{
						obj = vobj->next_content;
						break;
					}
					else
						continue;
				}
				i++;
			}
		}
		if ( obj )
		{
			cost = URANGE( 300, obj->cost / 10 , 3000);
			if ( ch->bank < cost ){
				fBank = FALSE;
				if( ch->gold < cost ){
					send_to_char( "�˳ƻȦ����W���ۻ��G�u�藍�_�A�S��������Ų�w�O�ڵL�k���ΫO���H���C�v\n\r", ch );
					return;
				}
			}
			if( fBank )
				ch->bank -= cost;
			else
				ch->gold -= cost;
			if( fBank )
				send_to_char("�A�q�Ȧ���b�A�I�X�F "HIY, ch);
			else
				send_to_char("�A�I�X�F "HIY, ch);
			sprintf(tmp, "%d", cost);
			send_to_char(tmp, ch);
			send_to_char(NOR " ����Ų�w�O�C\n\r", ch);
			spell_identify(gsn_invoke, ch->level,ch, obj);
			return;
		}
		send_to_char( "�˳ƻȦ����d�\\�F�d�O�W���n�Oï���G�_�ǡA�S���o���˳ƪ�������I\n\r", ch );
		return;
	}

	if( !str_prefix(arg1, "receive"))
	{
		if( !ch->neweqbank ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�o�����}�ӭܮw�C�v\n\r", ch );
			return;
		}
		if ( !is_number( arg2 ) ){
			send_to_char( "�Х� 'eqbank receive �s��' �ӻ���z���˳ơC\n\r", ch );
			return;
		}
		if( !ch->neweqbank->objs ){
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z���ܮw�̨S���F��ڡH�v\n\r", ch );
			return;
		}

		count = atoi( arg2);

		i = 1;
		if( count == 1 && ch->neweqbank->objs->value[4] == ch->in_room->area->recall )
		{
			obj = ch->neweqbank->objs;
			if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) ){
				do_say(vch, "�藍�_�A�z�����ʧ�h�F��F�C");
				return;
			}

			if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) ){
				do_say(vch,"�藍�_�A�z���t�������F�C");
				return;
			}
			ch->neweqbank->objs = obj->next_content;
			obj->next_content = NULL;
		}
		else if ( ch->neweqbank->objs->next_content )//&& ch->neweqbank->objs->next_content )
		{
			if( ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
				++i;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj->next_content )
			{
				if( !vobj->next_content ) break;
				if( vobj->next_content->value[4] != ch->in_room->area->recall ) continue;
				if( count == i )
				{
					if( vobj->next_content->value[4] == ch->in_room->area->recall )
					{
						obj = vobj->next_content;
						if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) )
						{
							do_say(vch, "�藍�_�A�z�����ʧ�h�F��F�C");
							return;
						}

						if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) )
						{
							do_say(vch,"�藍�_�A�z���t�������F�C");
							return;
						}
						vobj->next_content = obj->next_content;
						obj->next_content = NULL;
						break;
					}
					else
						continue;
				}
				i++;
			}
		}
		if ( obj )
		{
			/*
			if( obj->item_type == ITEM_CONTAINER ) {
			OBJ_DATA * vobj_next = NULL;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj_next ){
			if( ch->neweqbank->objs->in_obj == obj ) {
			vobj_next = ch->neweqbank->objs->next_content;
			ch->neweqbank->objs->next = object_list;
			object_list = ch->neweqbank->objs;
			object_list->next_content = obj->contains;
			obj->contains = object_list;
			ch->neweqbank->objs = vobj_next;
			continue;
			}
			if( vobj->next_content->in_obj == obj ) {
			vobj_next = vobj;
			vobj->next_content->next = object_list;
			object_list = vobj->next_content;
			vobj->next_content = vobj->next_content->next_content;
			object_list->next_content = obj->contains;
			obj->contains = object_list;
			continue;
			}
			}
			}*/

			if( obj->item_type == ITEM_CONTAINER)
			{
				for( vobj = obj->contains; vobj ; vobj = vobj->next_content )
				{
					vobj->next = object_list;
					object_list = vobj;
				}
			}
			obj_to_char ( obj, ch );
			obj->next = object_list;
			object_list = obj;
			// ���� to_room �קK�j�q�s�����~�y���L�H�e����� 2022/02/01
			//act( "�˳ƻȦ����۫᭱���w�бN$p���ӥ浹$n�C", ch, obj, NULL, TO_ROOM );
			//act( "�˳ƻȦ����@���`���G�u�бz�ˬd���L����l�ˡA���¥��{�C�v", ch, obj, NULL, TO_ROOM );
			act( "�˳ƻȦ����۫᭱���w�бN$p���ӥ浹�A�C", ch, obj, NULL, TO_CHAR );
			act( "�˳ƻȦ����@���`���G�u�бz�ˬd���L����l�ˡA���¥��{�C�v", ch, obj, NULL, TO_CHAR );
			ch->neweqbank->amount -= 1;
			sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
			if(( fp = fopen(tmp, "w"))){
				//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
				fwrite_neweqbank( ch, fp);
				fclose(fp);
				do_save( ch, "" );
				return;
			}else{
				bug("eqbank: file open error.",0);
				do_save( ch, "" );
				return;
			}
			return;
		}
		send_to_char( "�˳ƻȦ����d�\\�F�d�O�W���n�Oï���G�_�ǡA�S���o���˳ƪ�������I\n\r", ch );
		return;
	}

	if( !str_cmp(arg1, "list"))
	{
		if(!ch->neweqbank){
			send_to_char( "�˳ƻȦ���󻡵ۡG�u�z�٨S���}�ܮw�O�C�v\n\r", ch );
			return;
		}

		send_to_char( "�A�ثe�s�b�˳ƻȦ�O�I�w���˳Ʀ��G\n\r", ch );

		if ( !ch->neweqbank->objs ){
			send_to_char( "\n\r���򳣨S���I\n\r\n\r", ch );
			return; 
		}

		send_to_char("�i�s���j  ��  �~  �W  ��                                  ����\n\r", ch);

		for ( vobj = ch->neweqbank->objs; vobj;)
		{
			out_buf[0] = 0;

			/* �� Buffer ���� *

				for(count = 0; count < 50 && vobj; count++, vobj = vobj->next_content){
				if( vobj->in_obj ) continue;
				if( vobj->value[4] != ch->in_room->area->recall ) continue;
				i++;
				//add by jye 0940619
				if( arg2[0] != '\0')
				{
				if( !is_pfxname( arg2, vobj->name )) continue;
				}
				strip_ansi(vobj->short_descr, no_ansi);
				sprintf ( tmp, "�i%4d�j%-48s  [%2d] \n\r", 
				i,  
				can_see_obj(ch, vobj) ? no_ansi :
				"�������~",
				vobj->level
				);
				strcat(out_buf, tmp);
				}
				send_to_char(out_buf, ch);

			/**/

			/* �L Buffer ���� */

			for(;vobj; vobj = vobj->next_content)
			{
				if( vobj->in_obj )
					continue;
				if( vobj->value[4] != ch->in_room->area->recall )
					continue;

				i++;

				if( arg2[0] != '\0')  //add by jye 0940619
				{
					if( !is_pfxname( arg2, vobj->name ) )
						continue;
				}
				strip_ansi(vobj->short_descr, no_ansi);
				sprintf ( tmp, "�i%4d�j%-48s  [%2d] \n\r",
								i,
								can_see_obj(ch, vobj) ? no_ansi : "�������~",
								vobj->level );
				send_to_char(tmp, ch);
			}

			/**/
		}
		send_to_char( "\n\r", ch );
		return;
	}

	if( !str_cmp(arg1, "close"))
	{
		if( !ch->neweqbank ) {
			send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z�o�����}�ӭܮw�~�����W�C�v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
		ch->neweqbank->opened = FALSE;

		if(( fp = fopen(tmp, "w")))
		{
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
			send_to_char( "�˳ƻȦ���󻡵ۡG�u�ܮw���������C�v\n\r", ch );
		}
		else
		{
			bug("eqbank: file open error!!",0);
			send_to_char("�o���Y�����D�A�ЦV�j���^���C\n\r", ch);
			return;
		}
		free_neweqbank(ch->neweqbank);
		ch->neweqbank = NULL;
		return;
	}
	send_to_char("�� help eqbank �H���o�Ϊk�����C\n\r", ch);
	return;
}

void do_get_silence( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *container;
	//char buf [ MAX_INPUT_LENGTH ];
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	bool found;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	/* Get type. */
	if ( arg1[0] == '\0' )
	{
		send_to_char( "�n������?\n\r", ch );
		return;
	}

	if ( arg2[0] == '\0' )
	{
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj' */
			obj = get_obj_list( ch, arg1, ch->in_room->contents );
			if ( !obj )
			{
				return;
			}

			get_obj( ch, obj, NULL );
		}
		else
		{
			/* 'get all' or 'get all.obj' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = ch->in_room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, NULL );
				}
			}
		}
	}
	else
	{
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		{
			return;
		}
		/*
		sprintf( buf, "%s ������(corpse)", ch->name );
		for ( obj = object_list; obj; obj = obj->next )
		{
		if( !str_cmp( buf, obj->short_descr ) )
		{
		send_to_char( "�A���B�z�n�A�ۤv�a�I\n\r", ch );
		return;
		}
		}
		*/
		if ( !(container = get_obj_list( ch, arg2, ch->in_room->contents )) ){
			if ( !( container = get_obj_here( ch, arg2 ) ) )
			{
				return;
			}
		}

		switch ( container->item_type )
		{
			default:
			return;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			break;

			case ITEM_CORPSE_PC:
			{
				CHAR_DATA *gch;
				char  *pd, *cd;
				char   name [ MAX_INPUT_LENGTH ];
				char   chname [ MAX_INPUT_LENGTH ];

				if ( IS_NPC( ch ) )
				{
					return;
				}

				pd = container->short_descr;/*short_descr;
				pd = one_argument( pd, name );
				pd = one_argument( pd, name );*/
				pd = one_argument( pd, name );
				cd = ch->name;
				cd = one_argument( cd, chname );

				if ( str_cmp( name, chname ) && !IS_IMMORTAL( ch ) )
				{
					bool fGroup;

					fGroup = FALSE;
					for ( gch = char_list; gch; gch = gch->next )
					{
						if ( !IS_NPC( gch ) && is_same_group( ch, gch ) && !str_cmp( name, gch->name ) )
						{
							fGroup = TRUE;
							break;
						}
					}
					if ( !fGroup )
					{
						return;
					}
				}
			}
		}

		if ( IS_SET( container->value[1], CONT_CLOSED ) )
		{
			return;
		}

		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj container' */
			obj = get_obj_list( ch, arg1, container->contains );
			if ( !obj )
			{
				return;
			}
			get_obj( ch, obj, container );
		}
		else
		{
			/* 'get all container' or 'get all.obj container' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = container->contains; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, container );
				}
			}
		}
	}

	return;
}
void do_get( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *container;
	//char      buf [ MAX_INPUT_LENGTH ];
	char      arg1 [ MAX_INPUT_LENGTH ];
	char      arg2 [ MAX_INPUT_LENGTH ];
	bool      found;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

    /* Get type. */
	if ( arg1[0] == '\0' )
	{
		send_to_char( "�n������?\n\r", ch );
		return;
	}

	if ( arg2[0] == '\0' )
	{
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj' */
			obj = get_obj_list( ch, arg1, ch->in_room->contents );
			if ( !obj )
			{
				act( "�o�̨S�� $T.", ch, NULL, arg1, TO_CHAR );
				return;
			}
    
			get_obj( ch, obj, NULL );
		}
		else
		{
			/* 'get all' or 'get all.obj' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = ch->in_room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				//if( obj->item_type == ITEM_CORPSE_PC && !IS_IMMORTAL(ch) )
				//  continue;
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, NULL );
				}
			}

			if ( !found ) 
			{
				if ( arg1[3] == '\0' )
					send_to_char( "�o�̨S�����󪫫~.\n\r", ch );
				else
					act( "�o�̨S�� $T.", ch, NULL, &arg1[4], TO_CHAR );
			}
		}
	}
	else
	{
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		{
			send_to_char( "�A�L�k�o��.\n\r", ch );
			return;
		}
		/*
			sprintf( buf, "%s ������(corpse)", ch->name );
			for ( obj = object_list; obj; obj = obj->next )
			{
			if( !str_cmp( buf, obj->short_descr ) )
			{
			  send_to_char( "�A���B�z�n�A�ۤv�a�I\n\r", ch );
			  return;
			}
			}
		*/
		if ( !(container = get_obj_list( ch, arg2, ch->in_room->contents )) ){
			if ( !( container = get_obj_here( ch, arg2 ) ) )
			{
				act( "�o�̨S�� $T.", ch, NULL, arg2, TO_CHAR );
				return;
			}
		}

		switch ( container->item_type )
		{
			default:
				send_to_char( "�����O�Ӯe��.\n\r", ch );
				return;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_HORSE:
				break;

			case ITEM_CORPSE_PC:
			{
				//CHAR_DATA *gch;
				char  *pd, *cd;
				char   name [ MAX_INPUT_LENGTH ];
				char   chname [ MAX_INPUT_LENGTH ];

				if ( IS_NPC( ch ) )
				{
					send_to_char( "�A�L�k�o��.\n\r", ch );
					return;
				}

				pd = container->short_descr;/*short_descr;
				pd = one_argument( pd, name );
				pd = one_argument( pd, name );*/
				pd = one_argument( pd, name );
				cd = ch->name;
				cd = one_argument( cd, chname );

				if ( str_cmp( name, chname ) && !IS_IMMORTAL( ch ) )
				{
					bool fGroup;
      
					fGroup = FALSE;
					/*for ( gch = char_list; gch; gch = gch->next )
					{
					  if ( !IS_NPC( gch )
					&& is_same_group( ch, gch )
									  &&  !consented(ch, obj)
					&& !str_cmp( name, gch->name ) )
					  {
					fGroup = TRUE;
					break;
					  }
					}*/
					if ( !consented(ch, container))
					{
						send_to_char( "�A�L�k�o��.\n\r", ch );
						return;
					}
				}
				// �ߦ^�����i�H�A���i�� backup ���ʧ@ 2022/10/10
				if( is_qmark(ch, "death_stop_backup") )
					do_del_qmark( ch, "self death_stop_backup" );
			}
		}

		if ( container->item_type == ITEM_CONTAINER && IS_SET( container->value[1], CONT_CLOSED ) )
		{
			act( "$d �O���۪�.", ch, NULL, container->name, TO_CHAR );
			return;
		}

		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj container' */
			obj = get_obj_list( ch, arg1, container->contains );
			if ( !obj )
			{
				act( "$T �̨S���o�˪F��.", ch, NULL, arg2, TO_CHAR );
				return;
			}
			get_obj( ch, obj, container );
		}
		else
		{
			/* 'get all container' or 'get all.obj container' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = container->contains; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, container );
				}
			}

			if ( !found )
			{
				if ( arg1[3] == '\0' )
					act( "$T �̨S���o�˪F��.", ch, NULL, arg2, TO_CHAR );
				else
					act( "$T �̨S���o�˪F��.", ch, NULL, arg2, TO_CHAR );
			}
		}
	}
	save_char_obj( ch );
	return;
}



void do_put( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *container;
	OBJ_DATA *obj;
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];

	argument = one_argument( argument, arg1 );//get the first parameter from argument
	one_argument( argument, arg2 );           //and remove it from argument, then get
	//the second parameter from argument.
	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "�⤰���i����H\n\r", ch );
		return;
	}

	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
		send_to_char( "�A�L�k�o��.\n\r", ch );
		return;
	}

	if ( !( container = get_obj_here( ch, arg2 ) ) )
	{
		act( "�o�̨S�� $T.", ch, NULL, arg2, TO_CHAR );
		return;
	}

	if ( container->item_type != ITEM_CONTAINER && container->item_type != ITEM_HORSE)
	{
		send_to_char( "�����O�Ӯe��.\n\r", ch );
		return;
	}

	if ( container->item_type == ITEM_CONTAINER && IS_SET( container->value[1], CONT_CLOSED ) )
	{
		act( "$d �O���۪�.", ch, NULL, container->name, TO_CHAR );
		return;
	}
    
	//add by coconet 0511 for check whether the horse can carry things or not,
	//if v0 = -1, means can't
	if ( container->item_type == ITEM_HORSE && container->value[0] == -1 )
	{
		send_to_char("�e�L�k�t������F���C\n\r",ch);
		return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
		/* 'put obj container' */
		if ( !( obj = get_obj_carry( ch, arg1 ) ) )
		{
			send_to_char( "�A�S���o�ӪF��.\n\r", ch );
			return;
		}

		if ( obj == container )
		{
			send_to_char( "�A�L�k�⥦��i���ۤv��.\n\r", ch );
			return;
		}

		if ( !can_drop_obj( ch, obj ) )
		{
			send_to_char( "�A�ᤣ��.\n\r", ch );
			return;
		}

		if ( container->item_type == ITEM_CONTAINER )
		{
			if (  get_obj_weight( obj ) + get_obj_weight( container ) - container->weight > container->value[0] )
			{
				if ( container->value[0] != -1 )
				{
					send_to_char( "�񤣶i�h.\n\r", ch );
					return;
				}
			}
		}
		else
		{

			if ( get_obj_weight( obj ) + get_obj_weight( container ) - container->weight > container->value[0]  )
			{
				send_to_char(" ���Ǥw�g�t���Ӧh�F��F�C\n\r",ch);
				return;
			}
		}

		if( container->item_type != ITEM_HORSE )
		{
			act( "�A�� $p ��J $P.", ch, obj, container, TO_CHAR );
			act( "$n �� $p ��J $P.", ch, obj, container, TO_ROOM );
		}
		else
		{
			act( "�A�� $p ��b $P�I�W.", ch, obj, container, TO_CHAR );
			act( "$n �� $p ��b $P�I�W.", ch, obj, container, TO_ROOM );    
		}

		// 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("put", ch, obj);
		}

		// add at 2022/02/12
		if (!obj){
			save_char_obj( ch );
			return;
		}
		
		obj_from_char( obj );
		obj_to_obj( obj, container );
	}
	else
	{
		/* 'put all container' or 'put all.obj container' */
		OBJ_DATA *obj_next;
  
		if ( container->item_type == ITEM_CONTAINER )
		{
			for ( obj = ch->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
      
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& obj != container
					&& can_drop_obj( ch, obj )
					&& ( get_obj_weight( obj ) + get_obj_weight( container ) <= container->value[0] || container->value[0] == -1 ) )
				{
					act( "�A�� $p ��J $P.", ch, obj, container, TO_CHAR );
					act( "$n �� $p ��J $P.", ch, obj, container, TO_ROOM );

					// 2022/02/12
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("put", ch, obj);
					}

					// add at 2022/02/12
					if (!obj) continue;

					obj_from_char( obj );
					obj_to_obj( obj, container );
				}
			}
		}
		else
		{
			for( obj = ch->carrying; obj ; obj = obj_next )
			{
				obj_next = obj->next_content;
      
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& obj != container
					&& can_drop_obj( ch, obj )
					&& ( get_obj_weight( obj ) + get_obj_weight( container ) <= container->value[0] ))
				{
					act( "�A�� $p ��b $P�I�W.", ch, obj, container, TO_CHAR );
					act( "$n �� $p ��b $P�I�W.", ch, obj, container, TO_ROOM );

					// 2022/02/12
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("put", ch, obj);
					}

					// obj �i��b obj_act �ɳQ purge 2022/02/12
					if (!obj) continue;

					obj_from_char( obj );
					obj_to_obj( obj, container );
				}
			}   
		}
	}
	save_char_obj( ch );
	return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	bool      found;

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "�Fԣ?\n\r", ch );
		return;
	}

	if ( is_number( arg ) )
	{
		/* 'drop NNNN coins' */
		OBJ_DATA *obj_next;
		int   amount;

		amount   = atoi( arg );
		one_argument( argument, arg );

		if ( amount <= 0 || str_prefix( arg, "coins" ) )
		{
		  send_to_char( "�藍�_, �A�L�k�o��.\n\r", ch );
		  return;
		}

		if( ch->class <= BARBARIAN && ch->level < 3 ) {
		  send_to_char("�A�~���٤p�A�O�å�����C\n\r", ch);
		  return;
		}

		if ( ch->gold < amount )
		{
		  send_to_char( "�A�S������h��.\n\r", ch );
		  return;
		}

		ch->gold -= amount;

		for ( obj = ch->in_room->contents; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( obj->deleted )
				continue;

			switch ( obj->pIndexData->vnum )
			{
				case OBJ_VNUM_MONEY_ONE:
					amount += 1;
					extract_obj( obj );
				break;

				case OBJ_VNUM_MONEY_SOME:
					amount += obj->value[0];
					extract_obj( obj );
				break;
			}
		}

		obj_to_room( create_money( amount ), ch->in_room );
		send_to_char( "OK.\n\r", ch );  
		act( "$n ��U�@�ǿ�.", ch, NULL, NULL, TO_ROOM );
		return;
	}

	if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
	{
		/* 'drop obj' */
		if ( !( obj = get_obj_carry( ch, arg ) ) )
		{
			send_to_char( "�A�S�����˪F��.\n\r", ch );
			return;
		}

		if ( !can_drop_obj( ch, obj ) // && !IS_SET(obj->extra_flags, ITEM_PERSONAL))
			&& obj->pIndexData->vnum != OBJ_VNUM_MAGIC_ARROW
			&& obj->pIndexData->vnum != OBJ_VNUM_MAGIC_AMMO ) // �[�J�]�k�u�i�H�Q��� 2022/04/30
		{
			send_to_char( "�A�ᤣ����.\n\r", ch );
			return;
		}
		act( "�A��U $p.", ch, obj, NULL, TO_CHAR );
		act( "$n ��U $p.", ch, obj, NULL, TO_ROOM );
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
		// �s�W obj_act drop 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("drop", ch, obj);
		}
		// �ԭz���� obj ���} ch �e 2022/02/12
		//act( "�A��U $p.", ch, obj, NULL, TO_CHAR );
		//act( "$n ��U $p.", ch, obj, NULL, TO_ROOM );
		if( IS_SET(obj->extra_flags, ITEM_PERSONAL))
			extract_obj(obj);
	}
	else
	{
		/* 'drop all' or 'drop all.obj' */
		OBJ_DATA *obj_next;

		found = FALSE;
		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
				&& can_see_obj( ch, obj )
				&& obj->wear_loc == WEAR_NONE
				&& can_drop_obj( ch, obj ) )
			{
				act( "�A��U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ��U $p.", ch, obj, NULL, TO_ROOM );
				found = TRUE;
				obj_from_char( obj );
				obj_to_room( obj, ch->in_room );
				// �s�W obj_act drop 2022/02/12
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("drop", ch, obj);
				}
				// �ԭz���� obj ���} ch �e 2022/02/12
				//act( "�A��U $p.", ch, obj, NULL, TO_CHAR );
				//act( "$n ��U $p.", ch, obj, NULL, TO_ROOM );
			}
		}

		if ( !found )
		{
			if ( arg[3] == '\0' )
				send_to_char( "�A���W�S�b�I�F��.", ch );
			else
				act( "�A���W�S�� $T.", ch, NULL, &arg[4], TO_CHAR );
		}
	}
	save_char_obj( ch );
	return;
}



void do_give( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA  *obj;
	char       arg1 [ MAX_INPUT_LENGTH ];
	char       arg2 [ MAX_INPUT_LENGTH ];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "�n���֤���H\n\r", ch );
		return;
	}

	if ( is_number( arg1 ) )
	{
		/* 'give NNNN coins victim' */
		char buf[ MAX_STRING_LENGTH ];
		int  amount;

		amount   = atoi( arg1 );
		if ( amount <= 0 || str_prefix( arg2, "coins" ) )
		{
			send_to_char( "Sorry, �A�L�k�o��.\n\r", ch );
			return;
		}

		one_argument( argument, arg2 );

		if ( arg2[0] == '\0' )
		{
			send_to_char( "�n���֤���H\n\r", ch );
			return;
		}
		if( ch->class <= BARBARIAN && ch->level < 3 ) {
			send_to_char("�A�~���٤p�A�O�å�����C\n\r", ch);
			return;
		}

		if ( !( victim = get_char_room( ch, arg2 ) ) )
		{
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
			return;
		}

		if( victim == ch ) {
			send_to_char("�A��f�U�̪������X�ӧ⪱�@�U�C\n\r", ch);
			return;
		}

		if ( ch->gold < amount )
		{
			send_to_char( "�A�S���o��h��.\n\r", ch );
			return;
		}

		ch->gold     -= amount;
		if( !IS_NPC(victim) )
			victim->gold += amount;
		act( "�A���F$N�@�ǿ�.", ch, NULL, victim, TO_CHAR    );

		sprintf( buf, "$n���F�A %d �T����.", amount );
		act( buf, ch, NULL, victim, TO_VICT    );

		act( "$n���F$N�@�ǿ�.", ch, NULL, victim, TO_NOTVICT );
		mprog_bribe_trigger( victim, ch, amount );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg1 ) ) )
	{
		send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
		return;
	}

	if ( obj->wear_loc != WEAR_NONE )
	{
		send_to_char( "�A�o���⥦��U��.\n\r", ch );
		return;
	}

	if( IS_SET(obj->extra_flags, ITEM_PERSONAL ) && !IS_IMMORTAL( ch ))
	{
		send_to_char("�o�˪F�褣�൹�H.\n\r", ch);
		return;
	}


	if ( !( victim = get_char_room( ch, arg2 ) ) )
	{
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		return;
	}

	// if( victim == ch ) {
	// 	send_to_char("�A���ݭn���A�ۤv�F��C\n\r", ch );
	// 	return;
	// }
	if( IS_SET(obj->extra_flags, ITEM_UNIQUE) && IS_NPC(victim)){
		send_to_char("����浹�D���a����.\n\r", ch);
		return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
		send_to_char( "�A�L�k��}��.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_HOLY ) && victim->race == race_lookup( "Vampire" ) )
	{
		act( "$N �ڵ����U $p.", ch, obj, victim, TO_CHAR );
		act( "$n ���F $N $p ���O�Q�ڵ��F.",
		ch, obj, victim, TO_ROOM );
		act( "�A�ڵ����U $n ���� $p.",
		ch, obj, victim, TO_VICT );
		return;
	}

	if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
	{
		act( "$N�����ʧ�h�F��F.", ch, NULL, victim, TO_CHAR );
		act( "$n�յۭn���A�@�ǪF��A�i�O�A���W�w�g�񺡪F��F.", ch, NULL, victim, TO_VICT );
		return;
	}

	if ( victim->carry_weight + get_obj_weight( obj ) > can_carry_w( victim ) )
	{
		act( "$N�����ʧ�h���q�F.", ch, NULL, victim, TO_CHAR );
		act( "$n�յۭn���A�@�ǪF��A�i�O�A�w�g�����ʤF.",ch, NULL, victim, TO_VICT );
		return;
	}

	if ( !can_see_obj( victim, obj ) )
	{
		act( "$N �ݤ����o�˪F��.", ch, NULL, victim, TO_CHAR );
		return;
	}

	act( "�A���F$N$p.", ch, obj, victim, TO_CHAR );
	act( "$n���F�A$p.",   ch, obj, victim, TO_VICT );
	act( "$n���F$N$p.", ch, obj, victim, TO_NOTVICT );
	obj_from_char( obj );
	obj_to_char( obj, victim );
	// �ԭz���� obj ���} ch �e 2022/02/12
	//act( "�A���F$N$p.", ch, obj, victim, TO_CHAR );
	//act( "$n���F�A$p.",   ch, obj, victim, TO_VICT );
	//act( "$n���F$N$p.", ch, obj, victim, TO_NOTVICT );

	// �w�� ch �@�Ϊ� obj_act give 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("give", ch, obj);
	}
	// obj �i��b obj_act �ɳQ purge 2022/02/13
	if (!obj){
		save_char_obj( ch );
		return;
	}
	// �w�� victim �@�Ϊ� obj_act give_target 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("give_target", victim, obj);
	}
	if (!obj){
		save_char_obj( ch );
		return;
	}
	
	mprog_give_trigger( victim, ch, obj );
	
	// add at 2022/02/12
	if(!obj){
		save_char_obj( ch );
		return;
	}

	if ( IS_NPC( victim ) && ( victim->pIndexData->pShop )
		&& obj && !obj->deleted )
	{
		// �קK shop Ĳ�o give_prog drop �� give ���~����� add at 2020/10/30
		if( !(obj = get_obj_carry( victim,  arg1) ) ){
			save_char_obj( ch );
			return;
		}
		obj_from_char( obj );
		extract_obj( obj );
	}
	save_char_obj( ch );
	return;
}




void do_fill( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    char      arg [ MAX_INPUT_LENGTH ];
    char      arg1[ MAX_INPUT_LENGTH ];
    bool      found;

    if ( !check_blind( ch ) )
		return;

    argument = one_argument( argument, arg  );//get the first parameter from argument
    one_argument( argument, arg1 );       //and remove it from argument, then get
                //the second parameter from argument.

	if ( arg[0] == '\0' )
	{
		send_to_char( "Fill ����H\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
		return;
	}
    
	found = FALSE;
	if ( arg1[0] == '\0' )
	{
		for ( fountain = ch->in_room->contents; fountain; fountain = fountain->next_content )
		{
			if ( fountain->item_type == ITEM_FOUNTAIN )
			{
				found = TRUE;
				break;
			}
		}
	}
	else
    {
		fountain = get_obj_list( ch, arg1, ch->in_room->contents );
		if ( !fountain )
		{
			act( "�o�̨S�� $T.", ch, NULL, arg1, TO_CHAR );
			return;
		}
		if ( fountain->item_type == ITEM_FOUNTAIN )
		{
			found = TRUE;
		}
		else
		{
			send_to_char( "�o�F�褣�O����!\n\r" ,ch);
			return;   
		} 
	}

	if ( !found )
	{
		send_to_char( "�o�̨S�������I\n\r", ch );
		return;
	}

	if ( obj->item_type != ITEM_DRINK_CON )
	{
		send_to_char( "���˪F�褣��ˤ�.\n\r", ch );
		return;
	}

	if ( obj->value[0] < 1 || obj->value[1] == -1)
	{
		send_to_char( "�o�˪F��ˤ��F��.\n\r", ch );
		return;
	}
	
	if ( fountain->value[1] < 1 )
	{
		send_to_char( "�o�Ӥ������U�F.\n\r", ch );
		return;
	}

	if ( obj->value[1] >= obj->value[0] )
	{
		send_to_char( "���w�g�˺��F.\n\r", ch );
		return;
    }

	act("�A�� $p �˥��C", ch, obj, NULL, TO_CHAR);
	act( "�A���s�˺� $p.", ch, obj, NULL, TO_CHAR );
	obj->value[2] = fountain->value[2];
	obj->value[1] = UMIN(obj->value[0] , fountain->value[1]);
	obj->value[3] = fountain->value[3];
	fountain->value[1] -= obj->value[1];
	// �[�J obj_act fill 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("fill", ch, obj);
	}
	// add at 2022/02/12
	if(!obj)
		return;
	// �[�J obj_act filled �P�w�u���Qfilled ����U 2022/02/12
	if( IS_SET(fountain->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("filled", ch, fountain);
	}

	return;
}



void do_drink( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg [ MAX_INPUT_LENGTH ];
    int amount;
    int liquid;

    if ( !check_blind( ch ) ) return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
        {
            if ( obj->item_type == ITEM_FOUNTAIN ) break;
        }

        if ( !obj )
        {
            send_to_char( "�ܤ���H\n\r", ch );
            return;
        }
    }
    else
    {
        if ( !( obj = get_obj_here( ch, arg ) ) )
        {
            send_to_char( "�o�̨S���o�˪F��.\n\r", ch );
            return;
        }
    }

    switch ( obj->item_type )
    {
    default:
        send_to_char( "�������.\n\r", ch );
        break;

    case ITEM_FOUNTAIN:

		if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
		{
			bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
			obj->value[2] = 0;
		}
		liquid = obj->value[2];

			/*
				if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
				{
					bug( "Do_drink: bad liquid number %d.", liquid );
					liquid = obj->value[2] = 0;
				}
			*/

        if ( obj->value[1] <= 0 && obj->value[1] != -1)
        {
            act( "$p���U�F.", ch, obj, NULL, TO_CHAR );
            return;
        }

		// �ק�s�K������ 2023/01/13
		//if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 && liq_table[liquid].liq_affect[COND_DRUNK] > 0)
		if ( !IS_NPC( ch ) )
		{
			// �s�뤤�r�����p�U�L�k�i�� 2023/01/13
			if( ch->pcdata->condition[COND_DRUNK] >= 70 )
			{
				send_to_char( "�A�P��G�b�ȵ��A�ڥ��L�k�i��...\n\r", ch );
				return;
			}
			// �V�K�V�ܤ���s
			if( ch->pcdata->condition[COND_DRUNK] >= 20 && liq_table[liquid].liq_affect[COND_DRUNK] > 0 )
			{
				if( ch->pcdata->condition[COND_DRUNK] < number_percent() )
				{
					send_to_char( "�x�H���ѬO�ܤ����...  *�c*\n\r", ch );
					return;
				}
			}
		}

			/*   remove at 19/12/22
				if ( ch->race == race_lookup( "vampire" ) && obj->value[2] != 13)
				{
					act( "�A����$T.", ch, NULL, liq_table[liquid].liq_name, TO_CHAR );
					break;
				}
				
				������ fountain ������ thirst �ɨ캡���]�w remove at 19/12/22
				if ( !IS_NPC( ch ) ) ch->pcdata->condition[COND_THIRST] = 48;

				act( "�A�q $p �̳ܤF�X�f$T.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );
				act( "$n �q $p �̳ܤF�X�f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
			*/
		// ��Ҧ� Condition �q 48 �קאּ�̤j�� 100  2023/01/13
		if( !IS_NPC(ch) 
			&& liq_table[liquid].liq_affect[COND_DRUNK] == 0 
			&& ch->race != race_lookup( "vampire" ) )
		{
			// �P�w����Ѵ�������
			if( liq_table[liquid].liq_affect[COND_THIRST] > liq_table[liquid].liq_affect[COND_FULL]
				&& liq_table[liquid].liq_affect[COND_THIRST] > 0 )
			{
				// �����B���ƥi�H���������p
				if( ch->pcdata->condition[COND_THIRST] >= 40 && liq_table[liquid].liq_affect[COND_FULL] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			// �P�w�������������
			else if( liq_table[liquid].liq_affect[COND_THIRST] < liq_table[liquid].liq_affect[COND_FULL]
					&& liq_table[liquid].liq_affect[COND_FULL] > 0)
			{
				// ���j�B���ƥi�H�Ѵ������p
				if( ch->pcdata->condition[COND_FULL] >= 40 && liq_table[liquid].liq_affect[COND_THIRST] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
			}
			// �Ѵ������ۦP������
			else if( liq_table[liquid].liq_affect[COND_THIRST] > 0 && liq_table[liquid].liq_affect[COND_FULL] > 0 )
			{
				if( ch->pcdata->condition[COND_THIRST] > ch->pcdata->condition[COND_FULL] )
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			else
			{
				// �㦳�f�����Ⱦj�ĪG������
				amount = number_range( 1, 2 );
			}
		}
		else
		{
			// �s�붼�ƩΧl�尭
			amount = number_range( 1, 2 );
		}

		if( obj->value[1] != -1 )
			amount = UMIN( amount, obj->value[1] );


        if( amount > 1){
            act( "�A�q $p �̳ܤF�X�f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n �q $p �̳ܤF�X�f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }else{
            act( "�A�q $p �̳ܤF�f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n �q $p �̳ܤF�f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }

		// �o��s�몺���p
        gain_condition( ch, COND_DRUNK, amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );

		// �o�칡���θѴ������p
        if ( ch->race != race_lookup( "vampire" ) )
        {
            gain_condition( ch, COND_FULL,   amount * liq_table[liquid].liq_affect[COND_FULL] );
            gain_condition( ch, COND_THIRST, amount * liq_table[liquid].liq_affect[COND_THIRST] );
        }
        else if ( liquid == 13 ) // blood
        {
			// �P�w vampire �ܦ�
            if( !IS_NPC(ch) )
            {
                amount = UMAX( 1 , /*48*/ 100 - (ch->pcdata->condition[COND_THIRST]) );
                if( obj->value[1] != -1 )
                    amount = UMIN( amount, obj->value[1] );
            }
			amount = amount * 4;
            gain_condition( ch, COND_FULL, amount );
            gain_condition( ch, COND_THIRST, amount );
        }

		/* �ԭz��� gain_condition ��� 2023/01/13

		if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK ] > 10 )
			send_to_char( "�A�P�즳�ǾK�F.\n\r", ch );

		if ( ch->race != race_lookup( "vampire" ) )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "�A���F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "�A���A�P��f��.\n\r", ch );
        }
		else if ( liquid == 13 )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "�A���F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "�A���A�P��f��.\n\r", ch );
		} */

		/* �[�J mob no_poison �P�w�� race_undead ���r 2021/11/04
        if (obj->value[3] != 0
            && (   ch->race != race_lookup( "Vampire" )
                && ch->race != race_lookup( "Undead" ) 
                && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ))
		*/

		if( obj->value[3] != 0 ){
			/* The shit was poisoned! */
			AFFECT_DATA af;

			if( IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
				|| IS_SET( ch->special, NO_POISON ) )
			{
				// undead ���r�P�w 2021/11/04
				// �@�� AFF_GHOUL ���P���ܬ� undead 2022/01/16
				if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( ch, AFF_GHOUL ) ){
					send_to_char( "���Ƥ����r���I�J�F�A���餺�C\n\r", ch );
					af.type  = gsn_poison;
					af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
					af.location  = APPLY_SAVING_POISON;
					af.modifier  = amount;
					af.bitvector = AFF_POISON;
					affect_join( ch, &af );
				}
			}else{
				send_to_char( "�A�P�줣�ӹ�l�A��M�@�P�a�y�F�_�ӡI\n\r", ch );
				act( "$n �����ӹ�l�A��M�@�P�a�y�F�_�ӡI", ch, NULL, NULL, TO_ROOM );
				af.type  = gsn_poison;
				af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
				af.location  = APPLY_SAVING_POISON;
				af.modifier  = amount;
				af.bitvector = AFF_POISON;
				affect_join( ch, &af );
			}
		}
		if( obj->value[1] != -1 )
		{
			obj->value[1] -= amount;
			obj->value[1] = UMAX( 0, obj->value[1]);
		}

        if ( obj->value[1] == 0 )
        {
            act( "�A�� $p �ܰ��F.", ch, obj, NULL, TO_CHAR );
        }

		// add at 19/12/22
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("drink", ch, obj);
		}
        break;

	// ��Ҧ� Condition �q 48 �קאּ�̤j�� 100  2023/01/13
    case ITEM_DRINK_CON:

        if ( obj->value[1] <= 0 && obj->value[1] != -1)
        {
            send_to_char( "���w�g�Q�ܰ��F.\n\r", ch );
            return;
        }

		if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
		{
			bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
			obj->value[2] = 0;
		}
		liquid = obj->value[2];
			/*
				if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
				{
					bug( "Do_drink: bad liquid number %d.", liquid );
					liquid = obj->value[2] = 0;
				}
			*/

		// �ק�s�K������ 2023/01/13
		//if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 36 && liq_table[liquid].liq_affect[COND_DRUNK] > 0)
		if ( !IS_NPC( ch ) )
		{
			// �s�뤤�r�����p�U�L�k�i�� 2023/01/13
			if( ch->pcdata->condition[COND_DRUNK] >= 70 )
			{
				send_to_char( "�A�P��G�b�ȵ��A�ڥ��L�k�i��...\n\r", ch );
				return;
			}
			// �V�K�V�ܤ���s
			if( ch->pcdata->condition[COND_DRUNK] >= 20 && liq_table[liquid].liq_affect[COND_DRUNK] > 0 )
			{
				if( ch->pcdata->condition[COND_DRUNK] < number_percent() )
				{
					send_to_char( "�x�H���ѬO�ܤ����...  *�c*\n\r", ch );
					return;
				}
			}
		}

		if( !IS_NPC(ch) 
			&& liq_table[liquid].liq_affect[COND_DRUNK] == 0 
			&& ch->race != race_lookup( "vampire" ) )
		{
			if( liq_table[liquid].liq_affect[COND_THIRST] > liq_table[liquid].liq_affect[COND_FULL]
				&& liq_table[liquid].liq_affect[COND_THIRST] > 0 )
			{
				if( ch->pcdata->condition[COND_THIRST] >= 40 && liq_table[liquid].liq_affect[COND_FULL] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			else if( liq_table[liquid].liq_affect[COND_THIRST] < liq_table[liquid].liq_affect[COND_FULL]
					&& liq_table[liquid].liq_affect[COND_FULL] > 0)
			{
				if( ch->pcdata->condition[COND_FULL] >= 40 && liq_table[liquid].liq_affect[COND_THIRST] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
			}
			else if( liq_table[liquid].liq_affect[COND_THIRST] > 0 && liq_table[liquid].liq_affect[COND_FULL] > 0 )
			{
				if( ch->pcdata->condition[COND_THIRST] > ch->pcdata->condition[COND_FULL] )
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			else
			{
				amount = number_range( 1, 2 );
			}
		}
		else
		{
			amount = number_range( 1, 2 );
		}

        if( obj->value[1] != -1 )
            amount = UMIN( amount, obj->value[1] );

        if( amount > 1){
            act( "�A�q $p �̳ܤF�X�f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n �q $p �̳ܤF�X�f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }else{
            act( "�A�q $p �̳ܤF�f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n �q $p �̳ܤF�f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }

        gain_condition( ch, COND_DRUNK, amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
        if ( ch->race != race_lookup( "vampire" ) )
        {
            gain_condition( ch, COND_FULL,   amount * liq_table[liquid].liq_affect[COND_FULL   ] );
            gain_condition( ch, COND_THIRST, amount * liq_table[liquid].liq_affect[COND_THIRST ] );
        }
        else if ( liquid == 13 )
        {
            if( !IS_NPC(ch) )
            {
                amount = UMAX( 1 , /*48*/ 100 - (ch->pcdata->condition[COND_THIRST]) );
                if( obj->value[1] != -1 )
                    amount = UMIN( amount, obj->value[1] );
            }
            gain_condition( ch, COND_FULL, amount );
            gain_condition( ch, COND_THIRST, amount );
        }

		/* �ԭz��� gain_condition ��� 2023/01/13

		if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK ] > 10 )
			send_to_char( "�A�P�즳�ǾK�F.\n\r", ch );

		if ( ch->race != race_lookup( "vampire" ) )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "�A���F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "�A���A�P��f��.\n\r", ch );
        }
		else if ( liquid == 13 )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "�A���F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "�A���A�P��f��.\n\r", ch );
		} */

		/* �[�J mob no_poison �P�w�� race_undead ���r 2021/11/04
        if (obj->value[3] != 0
            && (   ch->race != race_lookup( "Vampire" )
                && ch->race != race_lookup( "Undead" ) 
                && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ))
		*/
		if( obj->value[3] != 0 ){
			/* The shit was poisoned! */
			AFFECT_DATA af;

			if( IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
				|| IS_SET( ch->special, NO_POISON ) )
			{
				// undead ���r�P�w 2021/11/04
				// �@�� AFF_GHOUL ���P���ܬ� undead 2022/01/16
				if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( ch, AFF_GHOUL ) ){
					send_to_char( "���Ƥ����r���I�J�F�A���餺�C\n\r", ch );
					af.type  = gsn_poison;
					af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
					af.location  = APPLY_SAVING_POISON;
					af.modifier  = amount;
					af.bitvector = AFF_POISON;
					affect_join( ch, &af );
				}
			}else{
				send_to_char( "�A�P�줣�ӹ�l�A��M�@�P�a�y�F�_�ӡI\n\r", ch );
				act( "$n �����ӹ�l�A��M�@�P�a�y�F�_�ӡI", ch, NULL, NULL, TO_ROOM );
				af.type  = gsn_poison;
				af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
				af.location  = APPLY_SAVING_POISON;
				af.modifier  = amount;
				af.bitvector = AFF_POISON;
				affect_join( ch, &af );
			}
		}
		// add at 19/12/22
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("drink", ch, obj);
		}

        if( obj->value[1] != -1 ) {
            obj->value[1] -= amount;
            obj->value[1] = UMAX( 0, obj->value[1]);
        }
		if ( obj->value[1] == 0 )
		{
			// add at 19/12/22
			if( IS_SET(obj->pIndexData->souvenir, Z4)
				|| obj->value[0] >= 50 )
			{
				send_to_char( "�A�⥦�ܥ��F.\n\r", ch );
			}
			else
			{
				send_to_char( "�A�ܥ�����A����⥦����F.\n\r", ch );
				extract_obj( obj );
			}
		}
		break;
    }

    return;
}



void do_eat( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );
	if ( arg[0] == '\0' ){
		send_to_char( "�n�Y����H\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) ){
		send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
		return;
	}

	if ( !IS_IMMORTAL( ch ) )
	{
		if( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL
			&& !( IS_SET( race_table[ ch->race ].race_abilities, RACE_ORC )
			&& obj->item_type == ITEM_CORPSE_NPC ) )
		{
			send_to_char( "���ݰ_�Ӥ���Y.\n\r", ch );
			return;
		}

		if( !IS_NPC( ch )
			&& obj->item_type != ITEM_PILL
			&& obj->item_type != ITEM_CORPSE_NPC
			&& ch->pcdata->condition[COND_FULL] >= 100 )
		{
			send_to_char( "�A�A�]�Y���U�F.\n\r", ch );
			return;
		}
	}

	switch ( obj->item_type )
	{

		case ITEM_FOOD:
			if ( !IS_NPC( ch ) )
			{
				// �s�뤤�r�����p�U�L�k�i�� 2023/01/13
				if( ch->pcdata->condition[COND_DRUNK] >= 70 )
				{
					send_to_char( "�A�P��G�b�ȵ��A�ڥ��L�k�i��...\n\r", ch );
					return;
				}
				act( "�A�Y�U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �Y�U $p.", ch, obj, NULL, TO_ROOM );
				int condition;

				condition = ch->pcdata->condition[COND_FULL];
				if ( ch->race != race_lookup( "vampire" ) )
					gain_condition( ch, COND_FULL, obj->value[0] );

				/* �ԭz��� gain_condition ��� 2023/01/13

				if ( ch->pcdata->condition[COND_FULL] > 40 )
					send_to_char( "�A�Y���F.\n\r", ch );
				else if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
					send_to_char( "�A���A�P�찧�j.\n\r", ch ); */
			}

			if( IS_NPC(ch) )
			{
				act( "�A�Y�U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �Y�U $p.", ch, obj, NULL, TO_ROOM );
			}

			/* �[�J mob no_poison �P�w�� race_undead ���r 2021/11/04
			if (   obj->value[3] != 0
				&& ch->race != race_lookup( "Vampire" )
				&& ch->race != race_lookup( "Undead" ) 
				&& !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ){
			*/
			if( obj->value[3] != 0 ){
				/* The shit was poisoned! */

				if( IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
					|| IS_SET( ch->special, NO_POISON ) )
				{
					// undead ���r�P�w 2021/11/04
					// �@�� AFF_GHOUL ���P���ܬ� undead 2022/01/16
					if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( ch, AFF_GHOUL ) ){
						send_to_char( "���������r���I�J�F�A���餺�C\n\r", ch );
						af.type  = gsn_poison;
						af.duration  = UMAX( 0 , obj->value[0] - get_curr_con(ch)*2 );
						af.location  = APPLY_SAVING_POISON;
						af.modifier  = obj->value[0];
						af.bitvector = AFF_POISON;
						affect_join( ch, &af );
					}
				}else{
					send_to_char( "�A�P�줣�ӹ�l�A��M�@�P�a�y�F�_�ӡI\n\r", ch );
					act( "$n �����ӹ�l�A��M�@�P�a�y�F�_�ӡI", ch, NULL, NULL, TO_ROOM );
					af.type  = gsn_poison;
					af.duration  = UMAX( 0 , obj->value[0] - get_curr_con(ch)*2 );
					af.location  = APPLY_SAVING_POISON;
					af.modifier  = obj->value[0];
					af.bitvector = AFF_POISON;
					affect_join( ch, &af );
				}
			}
			if( obj->value[1] > 0 && obj->value[1] <= 49 && ch->level >= obj->level){
				affect_strip(ch, gsn_satiety);
				send_to_char("�~���������������A�߹Ҳ��ͤF�ܤơC\n\r", ch);
				af.type      = gsn_satiety;
				af.duration  = UMAX(1 , obj->value[0]/2);
				af.location  = obj->value[1];
				af.modifier  = obj->value[2];
				af.bitvector = 0;
				affect_to_char( ch, &af );
			}
			break;
		case ITEM_PILL:
			if ( obj->level > ch->level ){
				act( "�A����Y���Ť�A�����ĤY.", ch, obj, NULL, TO_CHAR );
				return;
			}else{
				act( "�A�Y�U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �Y�U $p.", ch, obj, NULL, TO_ROOM );
				obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
				obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
				obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
				if (    !IS_NPC( ch )
					|| ( IS_NPC( ch ) && is_affected( ch, gsn_charm_person ) ) ){
					if( ch->fighting )
						WAIT_STATE( ch, 1 );
					else
						WAIT_STATE( ch, 2);
				}else{
					WAIT_STATE(ch, 2);
				}
			}
			break;
		case ITEM_CORPSE_NPC:
			if ( !IS_NPC( ch ) )
			{
				// �s�뤤�r�����p�U�L�k�i�� 2023/01/13
				if( ch->pcdata->condition[COND_DRUNK] >= 70 )
				{
					send_to_char( "�A�P��G�b�ȵ��A�ڥ��L�k�i��...\n\r", ch );
					return;
				}

				act( "�A�Y�U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �Y�U $p.", ch, obj, NULL, TO_ROOM );

				int condition = ch->pcdata->condition[COND_FULL];
				int nSklv     = get_skill_level(ch, gsn_inborn_ability);

				if ( ch->race != race_lookup( "vampire" ) )
					gain_condition( ch, COND_FULL, obj->value[0] );

				/* �ԭz��� gain_condition ��� 2023/01/13

				if ( ch->pcdata->condition[COND_FULL] > 40 )
					send_to_char( "�A�Y���F.\n\r", ch );
				else if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
					send_to_char( "�A���A�P�찧�j.\n\r", ch ); */

				if ( (obj->level >= ch->level + 2 - nSklv || obj->level > 37 - nSklv / 3) && obj->level < ch->level * 4){
					int nBonusHR = ( obj->level + nSklv - ch->level ) / (11 - nSklv);
					int nBonusDR = ( obj->level + nSklv - ch->level ) / ( 9 - nSklv);
					if(nBonusHR < 1) nBonusHR = 1;
					if(nBonusDR < 2) nBonusDR = 2;

					nBonusHR += nSklv * 2 / 3;
					nBonusDR += nSklv;

					if (nSklv > 3) {
					  nBonusDR += (nSklv - 1) * 2;
					  nBonusHR += (nSklv - 1);
					}

					if(nBonusHR > 2 + nSklv * (nSklv + 4) / 6) nBonusHR = 2 + nSklv * (nSklv + 4) / 6;
					if(nBonusDR > 5 + nSklv * (nSklv + 4) / 3) nBonusDR = 5 + nSklv * (nSklv + 4) / 3;

					affect_strip(ch, gsn_excited);
					send_to_char("�A�P��������R���F�O�q�C\n\r", ch);
					af.type      = gsn_excited;
					af.duration  = 2 + nSklv * nSklv / 6;
					af.location  = APPLY_HITROLL;
					af.modifier  = nBonusHR;
					af.bitvector = 0;
					affect_to_char( ch, &af );

					af.location  = APPLY_DAMROLL;
					af.modifier  = nBonusDR;
					affect_to_char( ch, &af );
				}
			}
			break;
		default:
			if ( IS_IMMORTAL( ch ) ){
				act( "�A�Y�U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �Y�U $p.", ch, obj, NULL, TO_ROOM );
			}
	}

	// if ( !IS_NPC( ch )
	// || ( IS_NPC( ch ) && is_affected( ch, gsn_charm_person ) ) )
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("eat", ch, obj);
	}
	if( obj ) // obj may dest self in prog
		extract_obj( obj );
	return;
}


/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
	OBJ_DATA *obj;

	if( !( obj = get_eq_char( ch, iWear ) ) )
		return TRUE;

	if( !fReplace )
		return FALSE;

	if( iWear == WEAR_RIDE && !IS_IMMORTAL(ch) )
		return FALSE;

	if( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) && !IS_IMMORTAL(ch) )
	{
		act( "�A�L�k��U $p.", ch, obj, NULL, TO_CHAR );
		return FALSE;
	}

	unequip_char( ch, obj );

	//modified by coco 0513
	if( iWear == WEAR_RIDE )
	{
		act( "$n ���U $p.", ch, obj, NULL, TO_ROOM );
		act( "�A���U $p.", ch, obj, NULL, TO_CHAR );
	}
	else
	{
		act( "$n ��U $p.", ch, obj, NULL, TO_ROOM );
		act( "�A��U $p.", ch, obj, NULL, TO_CHAR );
	}
	// �s�W�@�Φb�T���U�˳Ʈɪ� remove 2020/10/24
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("bremove", ch, obj);
	}
	return TRUE;
}

bool fremove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
	OBJ_DATA *obj;

	if ( !( obj = get_eq_char( ch, iWear ) ) )
		return TRUE;

	if ( !fReplace )
		return FALSE;

	if ( iWear == WEAR_RIDE && !IS_IMMORTAL(ch))
		return FALSE;

	unequip_char( ch, obj );
	//modified by coco 0513
	if( iWear == WEAR_RIDE )
	{
		act( "$n ���U $p.", ch, obj, NULL, TO_ROOM );
		act( "�A���U $p.", ch, obj, NULL, TO_CHAR );
	}
	else
	{
		act( "$n ��U $p.", ch, obj, NULL, TO_ROOM );
		act( "�A��U $p.", ch, obj, NULL, TO_CHAR );
	}
	// �s�W�@�Φb�T���U�˳Ʈɪ� remove 2020/10/24
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("bremove", ch, obj);
	}
	return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
typedef struct wear_map {
    char *name;
    int place;
} WEAR_MAP;



void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace, int iWear )
{
	char buf [ MAX_STRING_LENGTH ];
	bool    fForce = FALSE;
	int     nSklv  = get_skill_level(ch, gsn_inborn_ability);
	int     min_str, str_diff;

	if( iWear != 0 ) fForce = TRUE;

	if ( iWear == WEAR_RIDE && !IS_IMMORTAL(ch) )
		return;
	/*
	static const   WEAR_MAP wear_table[17]= {
	{   "about",      ITEM_WEAR_ABOUT},
	{   "arm",      ITEM_WEAR_ARMS},
	{   "arrow",      ITEM_WEAR_ARROW},
	{   "body",     ITEM_WEAR_BODY },
	{   "feet",     ITEM_WEAR_FEET },
	{   "finger",     ITEM_WEAR_FINGER },
	{   "hand",     ITEM_WEAR_HANDS},
	{   "head",     ITEM_WEAR_HEAD },
	{   "hold",     ITEM_HOLD},
	{   "leg",      ITEM_WEAR_LEGS },
	{   "neck",     ITEM_WEAR_NECK },
	{   "shield",     ITEM_WEAR_SHIELD},
	{   "twohand",      ITEM_TWO_HAND},
	{   "waist",      ITEM_WEAR_WAIST},
	{   "weapon",     ITEM_WIELD},
	{   "wrist",      ITEM_WEAR_WRIST},
	{   "ride",         ITEM_WEAR_RIDE},
	};
	*/
	if ( obj->item_type != ITEM_LIGHT && !CAN_WEAR(obj, ITEM_WEAR_BODY) && 
		 !CAN_WEAR(obj, ITEM_WEAR_SHIELD) && !CAN_WEAR(obj, ITEM_WIELD) && 
		 !CAN_WEAR(obj, ITEM_TWO_HAND) && !CAN_WEAR(obj, ITEM_WEAR_HEAD)){
		if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){    
			act( "$n �յ۬�W $p, �i���ݨӨä��X�A.", ch, obj, NULL, TO_ROOM );
			return;
		}
	}

	if ( ch->level < obj->level && !(IS_NPC(ch) && obj->level < 42)){
		sprintf( buf, "�A�����쵥�� %d �~��ϥ�%s.\n\r",
		obj->level , obj->short_descr);
		send_to_char( buf, ch );
		act( "$n �յ۬�W $p, �i���ݨӸg�礣��.",
			 ch, obj, NULL, TO_ROOM );
		return;
	}
	if ( obj->pIndexData->max_level > 0 && ch->level > obj->pIndexData->max_level ){
		sprintf( buf, "�A�����b���� %d �H���~��ϥγo�˪��~.\n\r",
		obj->pIndexData->max_level );
		send_to_char( buf, ch );
		return;
	}

	if( !IS_NPC( ch ) || (ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)) ){
		if( IS_SET(obj->pIndexData->souvenir, Z1) && !IS_IMMORTAL( ch ) ){
			send_to_char( "�����~�D�����~�A�u�Ѧ��áA��������I\n\r", ch );
			return;
		}
		if( IS_SET(obj->pIndexData->souvenir, Z2) && !IS_IMMORTAL( ch ) ){
			send_to_char( "�����~��Mob �M�ΡI\n\r", ch );
			act( "$n �յ۬�W $p, �i���ݨӨä��X�A.", ch, obj, NULL, TO_ROOM );
			return;
		}
	}

	if( IS_SET(obj->extra_flags, ITEM_LORE)){
		OBJ_DATA *pObj;
		int vnum = obj->pIndexData->vnum;
		for( pObj = ch->carrying ; pObj ; pObj = pObj->next_content ) {
			if( pObj->pIndexData->vnum == vnum && pObj->wear_loc != WEAR_NONE){
				send_to_char("�A���W�w�g��ۤ@��F�C\n\r", ch);
				return;
			}
		}
	}

	if( !fForce ) {
		if ( obj->item_type == ITEM_LIGHT ){
			if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
				return;
			act( "�A�I�G�F $p �åB�Τⴤ��.",  ch, obj, NULL, TO_CHAR );
			act( "$n �I�G�F $p �åB�Τⴤ��.", ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_LIGHT );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
				if (    get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
					 return;
			}else{
				if (    get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&&  get_eq_char( ch, WEAR_FINGER_3 )
					&&  get_eq_char( ch, WEAR_FINGER_4 )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_3, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_4, fReplace ) )
					return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_L ) ){
				act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_R ) ){
				act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_R );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_3 ) ){
				act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_3 );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_4 ) ){
				act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_4 );
				return;
			}

			bug( "Wear_obj: no free finger.", 0 );
			send_to_char( "�A�w�g������u�٫��F.\n\r", ch );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) ){
			if (    get_eq_char( ch, WEAR_NECK_1 )
				&&  get_eq_char( ch, WEAR_NECK_2 )
				&& !remove_obj( ch, WEAR_NECK_1, fReplace )
				&& !remove_obj( ch, WEAR_NECK_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_NECK_1 ) ){
				act( "�A �����W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �����W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_1 );
				return;
			}

			if ( !get_eq_char( ch, WEAR_NECK_2 ) ){
				act( "�A �����W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �����W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_2 );
				return;
			}

			bug( "Wear_obj: no free neck.", 0 );
			send_to_char( "�A���V�l�W�������U��h�F��F.\n\r", ch );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WEAR_BODY ) 
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ) {
			  if (strstr(obj->name, "robe") ||
				  strstr(obj->name, "cloth") ||
				  strstr(obj->name, "skirt")) {
				send_to_char("�o�s�����T��b�A���W�ä��X��.\n\r", ch);
				act("$n �յ۬�W $p, �i�O�o�M���U�h.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			  if (get_skill_level(ch, gsn_inborn_ability) < 6) {
				act("�A�յ۬�W$p���O�o�藍�W�h, �i��O�A�ѽ�Ӯt�F.", ch, obj, NULL, TO_CHAR);
				act("$n �յ۬�W $p, �i�O�o�Pı�������F���Ӳ�U�F.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			}
			if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
				return;
			act( "�A��W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_BODY );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) ){
			if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
				return;
			act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_HEAD );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WEAR_LEGS )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )){
			  if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
				  return;
			  act( "�A��W $p.", ch, obj, NULL, TO_CHAR );
			  act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
			  equip_char( ch, obj, WEAR_LEGS );
			  return;
			} else {
			  if (    get_eq_char( ch, WEAR_LEGS )
				  &&  get_eq_char( ch, WEAR_LEGS_2 )
				  && !remove_obj( ch, WEAR_LEGS, fReplace )
				  && !remove_obj( ch, WEAR_LEGS_2, fReplace ) )
				return;

			  if ( !get_eq_char( ch, WEAR_LEGS ) ) {
				  act( "�A ��W $p.", ch, obj, NULL, TO_CHAR );
				  act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
				  equip_char( ch, obj, WEAR_LEGS );
				return;
			  }

			  if ( !get_eq_char( ch, WEAR_LEGS_2 ) ) {
				  act( "�A ��W $p.", ch, obj, NULL, TO_CHAR );
				  act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
				  equip_char( ch, obj, WEAR_LEGS_2 );
				return;
			  }

			  send_to_char( "�A���L�w�g�ﺡ�L�ҤF.\n\r", ch );
			  return;
			}
		}

		if (     CAN_WEAR( obj, ITEM_WEAR_FEET )  
			 && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )){
			  if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
				  return;
			  act( "�A��W $p.", ch, obj, NULL, TO_CHAR );
			  act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
			  equip_char( ch, obj, WEAR_FEET );
			  return;
			} else {
				if (    get_eq_char( ch, WEAR_FEET )
					&&  get_eq_char( ch, WEAR_FEET_2 )
					&& !remove_obj( ch, WEAR_FEET, fReplace )
					&& !remove_obj( ch, WEAR_FEET_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_FEET ) ){
					act( "�A ��W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FEET );
					return;
				}

				if ( !get_eq_char( ch, WEAR_FEET_2 ) ){
					act( "�A ��W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FEET_2 );
					return;
				}

				send_to_char( "�A���}�w�g�ﺡ�u�l�F.\n\r", ch );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 5){
				if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
					return;
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS );
				return;
			}else{ // Beholder with Inborn Ability higher or equal to Sklv 5, added by Keelar
				if (    get_eq_char( ch, WEAR_HANDS )
					&&  get_eq_char( ch, WEAR_HANDS_2 )
					&& !remove_obj( ch, WEAR_HANDS, fReplace )
					&& !remove_obj( ch, WEAR_HANDS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_HANDS ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_HANDS_2 ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS_2 );
					return;
				}

				bug( "Wear_obj: no free gloves for beholder with sklv higher than 4.", 0 );
				send_to_char( "�A����w�g������M�F.\n\r", ch );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 6){
				if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
					return;
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_ARMS );
				return;
			}else{ // for beholder with inborn ability higher than or equal to 6, added by Keelar
				if (    get_eq_char( ch, WEAR_ARMS )
					&&  get_eq_char( ch, WEAR_ARMS_2 )
					&& !remove_obj( ch, WEAR_ARMS, fReplace )
					&& !remove_obj( ch, WEAR_ARMS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_ARMS ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_ARMS_2 ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS_2 );
					return;
				}

				bug( "Wear_obj: no free sleeves for beholder with sklv higher than 5.", 0 );
				send_to_char( "�A�����u�w�g�ﺡ�F.\n\r", ch );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) ) {
			if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
				return;
			if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ) {
			  if (strstr(obj->name, "robe") ||
				  strstr(obj->name, "cloth") ||
				  strstr(obj->name, "skirt")) {
				send_to_char("�o�s�����T��b�A���W�ä��X��.\n\r", ch);
				act("$n �յ۬�W $p, �i�O�o�M���U�h.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			  if (get_skill_level(ch, gsn_inborn_ability) < 5) {
				act("�A�����D����W$p�~���A�P���ʦۦp, �i��O�A�ѽ�Ӯt�F.", ch, obj, NULL, TO_CHAR);
				act("$n �յۧ�W $p, �i�O�o�Pı�������F���Ӳ�U�F.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			}
			act( "�A�ܤW $p.", ch, obj, NULL, TO_CHAR );
			act( "$n �ܤW $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ABOUT );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WEAR_WAIST )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
			  if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
				  return;
			  act( "�Aô�W $p.", ch, obj, NULL, TO_CHAR );
			  act( "$n ô�W $p.",   ch, obj, NULL, TO_ROOM );
			  equip_char( ch, obj, WEAR_WAIST );
			  return;
			} else {
			  send_to_char("�A�䤣��a��ô�W�y�a.\n\r", ch);
			  act("$n �յ�ô�W $p, �i�O�o�䤣��X�A���a��.", ch, obj, NULL, TO_ROOM);
			  return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) ){
			if (    get_eq_char( ch, WEAR_WRIST_L )
				&&  get_eq_char( ch, WEAR_WRIST_R )
				&& !remove_obj( ch, WEAR_WRIST_L, fReplace )
				&& !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_WRIST_L ) ){
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_WRIST_R ) ){
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_R );
				return;
			}

			bug( "Wear_obj: no free wrist.", 0 );
			send_to_char( "�A�����ó����۪F��.\n\r", ch );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) ){
			// �W�[ Shield ��˭��q�ԭz 2018.11.27
			int ShieldWt = get_obj_weight( obj );
			int RequireStr = 1 + get_curr_str( ch ) + 2 * get_skill_level(ch, gsn_block );

			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			act( "$n ���_ $p �@���޵P.", ch, obj, NULL, TO_ROOM );
			act( "�A���_ $p �@���޵P.", ch, obj, NULL, TO_CHAR );
			if( RequireStr < ShieldWt / 4 ) {
				act( "$p ��A�ӻ����I�I��, �ϥΤW�L�k�o������.", ch, obj, NULL, TO_CHAR );
			}
			equip_char( ch, obj, WEAR_SHIELD );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WIELD )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if( IS_NPC( ch ) || get_skill_percent( ch, gsn_dual ) != 0 )
			{
			  /*
			   * If you think this looks ugly now, just imagine how
			   * I felt trying to write it!  --- Thelonius (Monk)
			   * Now, it will attempt to wear in 1 if open, then 2 if
			   * open, else replace 2, else replace 1.
			   */
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
					|| ( get_eq_char( ch, WEAR_WIELD  )
					&& ( ( get_eq_char( ch, WEAR_WIELD_2 )
					&& !remove_obj( ch, WEAR_WIELD_2, fReplace ) ) //WEAR_HOLD & WEAR_SHIELD
					|| !remove_obj( ch, WEAR_HOLD,    fReplace )     //is at the same position of
					|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) )   //of WEAR_WIELD_2
					&&  !remove_obj( ch, WEAR_WIELD, fReplace ) ) )
					return;

				if ( !get_eq_char( ch, WEAR_WIELD ) ){
		
					int weight = get_obj_weight( obj );
		
					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
					act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
					}
					equip_char( ch, obj, WEAR_WIELD );
					return;
				}
		
				if ( !get_eq_char( ch, WEAR_WIELD_2 ) ){

					int weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield2 >= weight)
						  break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
					act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
					}
					equip_char( ch, obj, WEAR_WIELD_2 );
					return;
				}
				bug( "Wear_obj: no free weapon slot.", 0 );
				send_to_char( "�A���ⳣ���۪Z���F.\n\r", ch );
				return;
			}else{  /* can only wield one weapon */
				int weight = get_obj_weight( obj );
				if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
					return;
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace ) )
					return;
		
				for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
					// �N�J����m�����T , ���B���� .wield
					//if(str_app[min_str].twohand >= weight)
					if(str_app[min_str].wield >= weight)
						break;
				}
				str_diff = get_curr_str( ch ) - min_str;

				int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
				int nPbSklv  = get_skill_level(ch, gsn_powerblow);
				if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

				act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
				act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
					else
						act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
				}
				equip_char( ch, obj, WEAR_WIELD );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_HOLD ) ){
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_HOLD,    fReplace ) )
				return;
			act( "�A���� $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ���� $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_HOLD );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_TWO_HAND )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			int weight = get_obj_weight( obj );
			if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;

			for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
				if(str_app[min_str].twohand >= weight)
					break;
			}
			str_diff = get_curr_str( ch ) - min_str;

			int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
			int nPbSklv  = get_skill_level(ch, gsn_powerblow);
			if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

			act( "�A�����ⴤ�� $p.", ch, obj, NULL, TO_CHAR );
			act( "$n �����ⴤ�� $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_TWO_HAND );
			if( str_diff < 0 ) {
				if( str_diff < -5 )
					act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
				else
					act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
			}
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_ARROW ) ){
			if(obj->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW){
				if(obj->value[3] > 30 + get_skill_level(ch, gsn_magic_arrow) * 20){
				//if(obj->value[3] > get_skill_level(ch, gsn_magic_arrow) * 20){
					act( "�A�L�k����o���]�k�b���O�q.", ch, obj, NULL, TO_CHAR );
					return;
				}
			}
			if ( !remove_obj( ch, WEAR_ARROW, fReplace ) )
				return;
			act( "�A�� $p �ױ��b�I�W.", ch, obj, NULL, TO_CHAR );
			act( "$n �� $p �ױ��b�I�W.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ARROW );
			return;
		}
		// �u�ĸ˳� add at 2022/04/30
		if ( CAN_WEAR( obj, ITEM_WEAR_AMMO ) ){
			if ( !remove_obj( ch, WEAR_AMMO, fReplace ) )
				return;
			act( "�A�� $p �����b�e����˪���m.", ch, obj, NULL, TO_CHAR );
			act( "$n �� $p �����b�e����˪���m.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_AMMO );
			return;
		}
	}else{
		/*
			if ( obj->item_type == ITEM_LIGHT )
			{
				if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
					return;
				act( "�A�I�G�F $p �åB�Τⴤ��.",  ch, obj, NULL, TO_CHAR );
				act( "$n �I�G�F $p �åB�Τⴤ��.", ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_LIGHT );
				return;
			}
		*/

		if ( iWear == ITEM_WEAR_FINGER && CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
				if (    get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
					return;
			}else{
				if (   get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&&  get_eq_char( ch, WEAR_FINGER_3 )
					&&  get_eq_char( ch, WEAR_FINGER_4 )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_3, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_4, fReplace ) )
					return;
				if ( !get_eq_char( ch, WEAR_FINGER_3 ) ){
					act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FINGER_3 );
					return;
				}

				if ( !get_eq_char( ch, WEAR_FINGER_4 ) ){
					act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FINGER_4 );
					return;
				}
			}

			if ( !get_eq_char( ch, WEAR_FINGER_L ) ){
				act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_R ) ){ 
				act( "�A���W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_R );
				return;
			}


			bug( "Wear_obj: no free finger.", 0 );
			send_to_char( "�A�w�g�����٫��F.\n\r", ch );
			return;
		}

		if ( iWear == ITEM_WEAR_NECK && CAN_WEAR( obj, ITEM_WEAR_NECK ) )
		{
			if (    get_eq_char( ch, WEAR_NECK_1 )
				&&  get_eq_char( ch, WEAR_NECK_2 )
				&& !remove_obj( ch, WEAR_NECK_1, fReplace )
				&& !remove_obj( ch, WEAR_NECK_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_NECK_1 ) ){
				act( "�A �����W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �����W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_1 );
				return;
			}

			if ( !get_eq_char( ch, WEAR_NECK_2 ) ){
				act( "�A �����W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �����W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_2 );
				return;
			}

			bug( "Wear_obj: no free neck.", 0 );
			send_to_char( "�A���V�l�W�������U��h�F��F.\n\r", ch );
			return;
		}

		if ( iWear == ITEM_WEAR_BODY && CAN_WEAR( obj, ITEM_WEAR_BODY ) 
            && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
            if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
                return;
            act( "�A��W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_BODY );
            return;
        }

		if (iWear == ITEM_WEAR_HEAD && CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
		{
            if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
                return;
            act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_HEAD );
            return;
        }

		if ( iWear == ITEM_WEAR_LEGS && CAN_WEAR( obj, ITEM_WEAR_LEGS )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
            if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
                return;
            act( "�A��W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_LEGS );
            return;
        }

		if ( iWear == ITEM_WEAR_FEET && CAN_WEAR( obj, ITEM_WEAR_FEET )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
            if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
                return;
            act( "�A��W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_FEET );
            return;
        }

		/*
			//add by coco for WEAR_RIDE
			if ( iWear == ITEM_WEAR_RIDE && CAN_WEAR( obj, ITEM_WEAR_RIDE )   )
			{
				if ( !remove_obj( ch, WEAR_RIDE, fReplace ) )
					return;

				if ( !IS_NPC(ch) && ch->class != KNIGHT && ch->class != LORD )
					return;

				act( "�A�M�W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n �M�W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_RIDE );
				return;
			}   
		*/

		if ( iWear == ITEM_WEAR_HANDS && CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 5){
				if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
					return;
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS );
				return;
			}else{ // Beholder with Inborn Ability higher or equal to Sklv 5, added by Keelar
			
				if (    get_eq_char( ch, WEAR_HANDS )
					&&  get_eq_char( ch, WEAR_HANDS_2 )
					&& !remove_obj( ch, WEAR_HANDS, fReplace )
					&& !remove_obj( ch, WEAR_HANDS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_HANDS ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_HANDS_2 ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS_2 );
					return;
				}

				bug( "Wear_obj: no free gloves for beholder with sklv higher than 4.", 0 );
				send_to_char( "�A����w�g������M�F.\n\r", ch );
				return;
			}
		}

		if ( iWear == ITEM_WEAR_ARMS && CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 6){
				if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
					return;
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_ARMS );
				return;
			}else{ // for beholder with inborn ability higher than or equal to 6, added by Keelar
				if (    get_eq_char( ch, WEAR_ARMS )
					&&  get_eq_char( ch, WEAR_ARMS_2 )
					&& !remove_obj( ch, WEAR_ARMS, fReplace )
					&& !remove_obj( ch, WEAR_ARMS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_ARMS ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_ARMS_2 ) ){
					act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS_2 );
					return;
				}

				bug( "Wear_obj: no free sleeves for beholder with sklv higher than 5.", 0 );
				send_to_char( "�A�����u�w�g�ﺡ�F.\n\r", ch );
				return;
			}
		}

		if ( iWear == ITEM_WEAR_ABOUT && CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
		{
			if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
				return;
			act( "�A�ܤW $p.", ch, obj, NULL, TO_CHAR );
			act( "$n �ܤW $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ABOUT );
			return;
		}

		if ( iWear == ITEM_WEAR_WAIST && CAN_WEAR( obj, ITEM_WEAR_WAIST )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
				return;
			act( "�Aô�W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ô�W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_WAIST );
			return;
		}

		if ( iWear == ITEM_WEAR_WRIST && CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
		{
			if ( get_eq_char( ch, WEAR_WRIST_L )
			&&   get_eq_char( ch, WEAR_WRIST_R )
			&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
			&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_WRIST_L )) {
				act( "�A���W $p.",ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_WRIST_R ) ){
				act( "�A���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_R );
				return;
			}

			bug( "Wear_obj: no free wrist.", 0 );
			send_to_char( "�A�����ó����۪F��.\n\r", ch );
			return;
		}

		if ( iWear == ITEM_WEAR_SHIELD && CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
		{
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			act( "$n ���_ $p �@���޵P.", ch, obj, NULL, TO_ROOM );
			act( "�A���_ $p �@���޵P.", ch, obj, NULL, TO_CHAR );
			equip_char( ch, obj, WEAR_SHIELD );
			return;
		}
		// �u�ĸ˳� add at 2022/04/30
		if ( iWear == ITEM_WEAR_AMMO && CAN_WEAR( obj, ITEM_WEAR_AMMO ) ){
			if ( !remove_obj( ch, WEAR_AMMO, fReplace ) )
				return;
			act( "�A�� $p �����b�e����˪���m.", ch, obj, NULL, TO_CHAR );
			act( "$n �� $p �����b�e����˪���m.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_AMMO );
			return;
		}

		// iWear weapon
		if ( iWear == ITEM_WIELD && CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			if( IS_NPC( ch ) || get_skill_percent(ch, gsn_dual ) != 0 )
			{
			  /*
			   * If you think this looks ugly now, just imagine how
			   * I felt trying to write it!  --- Thelonius (Monk)
			   * Now, it will attempt to wear in 1 if open, then 2 if
			   * open, else replace 2, else replace 1.
			   */
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
					|| ( get_eq_char( ch, WEAR_WIELD  )
					&& ( ( get_eq_char( ch, WEAR_WIELD_2 )
					&& !remove_obj( ch, WEAR_WIELD_2, fReplace ) ) //WEAR_HOLD & WEAR_SHIELD
					|| !remove_obj( ch, WEAR_HOLD,    fReplace )     //is at the same position of
					|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) )   //of WEAR_WIELD_2
					&& !remove_obj( ch, WEAR_WIELD, fReplace ) ) )
					return;
				if ( !get_eq_char( ch, WEAR_WIELD ) )
				{
					int   weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
					act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
					}
					equip_char( ch, obj, WEAR_WIELD );
					return;
				}

				if ( !get_eq_char( ch, WEAR_WIELD_2 ) )
				{
					int   weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield2 >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
					act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
					}
						equip_char( ch, obj, WEAR_WIELD_2 );
						return;
				}
				bug( "Wear_obj: no free weapon slot.", 0 );
				send_to_char( "�A���ⳣ���۪Z���F.\n\r", ch );
				return;
			}
			else  /* can only wield one weapon */
			{
				int   weight = get_obj_weight( obj );
				if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
					return;
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace ) )
					return;

				for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
					if(str_app[min_str].wield >= weight)
						break;
				}
				str_diff = get_curr_str( ch ) - min_str;

				int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
				int nPbSklv  = get_skill_level(ch, gsn_powerblow);
				if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

				act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
				act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
					else
						act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
				}
				equip_char( ch, obj, WEAR_WIELD );
				return;
			}
		}

		if ( iWear == ITEM_HOLD && CAN_WEAR( obj, ITEM_HOLD ) )
		{
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_HOLD,    fReplace ) )
				return;
			act( "�A���� $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ���� $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_HOLD );
			return;
		}

		// iWear twohand
		if ( iWear == ITEM_TWO_HAND //&& (CAN_WEAR( obj, ITEM_TWO_HAND ) || ch->class == MARTIALMAN)
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			int   weight = get_obj_weight( obj );
			int min_str;
			if( !CAN_WEAR(obj, ITEM_WIELD) && !CAN_WEAR(obj, ITEM_TWO_HAND)){
				send_to_char("�o�󪫫~����@���Z���C\n\r", ch);
				return;
			}
			if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;
			for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
				if(str_app[min_str].twohand >= weight)
					break;
			}
			str_diff = get_curr_str( ch ) - min_str;

			int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
			int nPbSklv  = get_skill_level(ch, gsn_powerblow);
			if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

			act( "�A�����ⴤ�� $p.", ch, obj, NULL, TO_CHAR );
			act( "$n �����ⴤ�� $p.",   ch, obj, NULL, TO_ROOM );
			if( str_diff < 0 ) {
				if( str_diff < -5 )
					act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
				else
					act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
			}
			equip_char( ch, obj, WEAR_TWO_HAND );
			return;
		}

		if ( iWear == ITEM_WEAR_ARROW && CAN_WEAR( obj, ITEM_WEAR_ARROW ) )
		{
			if ( !remove_obj( ch, WEAR_ARROW, fReplace ) )
				return;
			act( "�A�� $p �ױ��b�I�W.", ch, obj, NULL, TO_CHAR );
			act( "$n �� $p �ױ��b�I�W.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ARROW );
			return;
		}

		// �s�W����r wp1 �ΨӸ˳ƥD�n�Z�� 2022/04/05
		if ( iWear == WEAR_WIELD && CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			/*  ���q�P�w���~ 2022/12/06
			if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
				|| ( ( ( get_eq_char( ch, WEAR_WIELD ) && !remove_obj( ch, WEAR_WIELD, fReplace ) )
					|| !remove_obj( ch, WEAR_HOLD, fReplace )
					|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) ) ) )
				return; */

			if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_WIELD ) )
			{
				int   weight = get_obj_weight( obj );

				for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
					if(str_app[min_str].wield >= weight)
						break;
				}
				str_diff = get_curr_str( ch ) - min_str;

				int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
				int nPbSklv  = get_skill_level(ch, gsn_powerblow);
				if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

				act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
				act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
					else
						act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
				}
				equip_char( ch, obj, WEAR_WIELD );
				return;
			}
			bug( "Wear_obj: when wear wp , no free weapon slot.", 0 );
			send_to_char( "�A���ⳣ���۪Z���F.\n\r", ch );
			return;
		}

		// �s�W wp2 �˳Ʀ��n�Z�� 2022/04/05
		if ( iWear == WEAR_WIELD_2 && CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			if( IS_NPC( ch ) || get_skill_percent(ch, gsn_dual ) != 0 )
			{
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
					|| ( ( ( get_eq_char( ch, WEAR_WIELD_2 ) && !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
						|| !remove_obj( ch, WEAR_HOLD, fReplace )
						|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) ) ) )
					return;

				if ( !get_eq_char( ch, WEAR_WIELD_2 ) )
				{
					int   weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield2 >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "�A�˳� $p �@�Z��.", ch, obj, NULL, TO_CHAR );
					act( "$n �˳� $p �@�Z��.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "�ӭ��F�I�A�����ʡI", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ��A�ӻ����I�I��.", ch, obj, NULL, TO_CHAR );
					}
						equip_char( ch, obj, WEAR_WIELD_2 );
						return;
				}
				bug( "Wear_obj: when wear wp2 , no free weapon slot.", 0 );
				send_to_char( "�A���ⳣ���۪Z���F.\n\r", ch );
				return;
			}else{
				send_to_char( "�A�L�k�˳Ʀ��n�Z��.\n\r", ch );
				return;
			}
		}

		// �s�W hand1 �˳Ʀ���M 2022/04/05
		if ( iWear == WEAR_HANDS && CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
		{

			if ( get_eq_char( ch, WEAR_HANDS ) && !remove_obj( ch, WEAR_HANDS, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_HANDS ) ){
				act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS );
				return;
			}

			bug( "Wear_obj: hand2 change no free gloves for beholder with sklv higher than 4.", 0 );
			send_to_char( "�A����w�g������M�F.\n\r", ch );
			return;
		}

		// �s�W hand2 �˳Ʀ���M 2022/04/05
		if ( iWear == WEAR_HANDS_2 && CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 5)
				return;

			if ( get_eq_char( ch, WEAR_HANDS_2 ) && !remove_obj( ch, WEAR_HANDS_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_HANDS_2 ) ){
				act( "�A ���W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ���W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS_2 );
				return;
			}

			bug( "Wear_obj: hand2 change no free gloves for beholder with sklv higher than 4.", 0 );
			send_to_char( "�A����w�g������M�F.\n\r", ch );
			return;
		}

		// �s�W feet1 �˳Ʀ���M 2022/04/05
		if ( iWear == WEAR_FEET && CAN_WEAR( obj, ITEM_WEAR_FEET ) )
		{

			if ( get_eq_char( ch, WEAR_FEET ) && !remove_obj( ch, WEAR_FEET, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_FEET ) ){
				act( "�A ��W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FEET );
				return;
			}

			bug( "Wear_obj: feet2 change no free feet for centaur.", 0 );
			send_to_char( "�A���}�w�g�ﺡ�c�l�F.\n\r", ch );
			return;
		}

		// �s�W feet2 �˳Ʀ��}�� 2022/04/05
		if ( iWear == WEAR_FEET_2 && CAN_WEAR( obj, ITEM_WEAR_FEET ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) )
				return;

			if ( get_eq_char( ch, WEAR_FEET_2 ) && !remove_obj( ch, WEAR_FEET_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_FEET_2 ) ){
				act( "�A ��W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ��W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FEET_2 );
				return;
			}

			bug( "Wear_obj: feet2 change no free feet for centaur.", 0 );
			send_to_char( "�A���}�w�g�ﺡ�c�l�F.\n\r", ch );
			return;
		}
	}

	if ( fReplace ) //some objs just cannot be wear/hold/wield...
		send_to_char( "�A�L�k�˳Ƴo�˪F��.\n\r", ch );

	return;
}

/* Added by Keric for personal eq AC recover */
void do_personal_recover( CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *mob;
	OBJ_DATA *obj;
	float adj = 1.0;
	int price;
	bool fBank = FALSE;
	char arg[MAX_STRING_LENGTH];
    
	if( !ch->in_room ) return;

	for( mob = ch->in_room->people ; mob; mob = mob->next_in_room )
	{
		if( IS_NPC(mob) && IS_SET(mob->act, ACT_SMITH)) break;
	}

	if( !mob )
	{
		send_to_char("����H�A�Q�F����H\n\r", ch);
		return;
	}

	one_argument(argument, arg);
    
	if( arg[0] == '\0') {
		send_to_char("�A�n�פ���H\n\r", ch);
		return;
	}

	if( !str_cmp("all", arg)) {
		for( obj = ch->carrying ; obj ; obj = obj->next_content )
		{
			if( obj->wear_loc != WEAR_NONE ) continue;
			if( !(obj->item_type == ITEM_ARMOR) ) continue;
			if( obj->item_type == ITEM_ARMOR && obj->value[1] < 1 ) continue;
			/*
			if( !(obj->extra_flags & ITEM_PERSONAL) ) continue;

			if( obj->item_type == ITEM_ARMOR && obj->value[1] < 0 )    
			{ 
				send_to_char("�A�b�M�ڶ}�߶ܡH\n\r", ch);
				continue;
			}
			*/

			if( obj->level < 5 ) price = 5;
			else if (obj->level < 15) price = 10;
			else if ( obj->level < 30) price = 50;
			else if ( obj->level <= LEVEL_HERO ) price = 100;
			else price = 150;
			price = URANGE(1,price, ((obj->cost * 2) ));

			price *= obj->value[1]*100;
			if( IS_SET(obj->extra_flags, ITEM_BLESS)) adj += 1.5;
			if( IS_SET(obj->extra_flags, ITEM_HUM)) adj += 1.5;
			if( !(obj->extra_flags & ITEM_PERSONAL)) price *= 30;

			price = (int)(price * adj);
			price = UMAX( 100, price);

			sprintf(arg, "�ײz %s �ݭn %d �T�����C\n\r", obj->short_descr, price);

			send_to_char(arg, ch);
			fBank = FALSE;

			if( ch->gold < price ) 
			{
				fBank = TRUE;
				if( ch->bank < price ) 
				{
					act("$N�n�F�n�Y�A���G�u�z���������C�v", ch, NULL, mob, TO_CHAR);
					continue;
				}
			}

			if( fBank ) 
			{
				sprintf(arg, "�A�q�Ȧ�I�X %d �T�����C \n\r", price);
				ch->bank -= price;
			}
			else 
			{
				sprintf(arg, "�A�I�X %d �T�����C \n\r", price);
				ch->gold -= price;
			}

			send_to_char(arg, ch);

			act("�A�� $p �浹$N�C", ch, obj, mob, TO_CHAR);
			act("$N�E��|���A$p�x�_�F�@�D���~�C", ch, obj, mob, TO_CHAR);
			act("$N�٩��B�Y�W�����]�A��$p���ٵ��A�C", ch, obj, mob, TO_CHAR);

			if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) )
			{
				obj->value[0] += obj->value[1];
				obj->value[1] = 0;
			}
		}
		return;
	}

	if( !(obj = get_obj_carry(ch, arg))){
		send_to_char("�A���W�S���o�˪F��C\n\r", ch);
		return;
	}

	//if( !(obj->extra_flags & ITEM_PERSONAL) ) 
	if( !(obj->item_type == ITEM_ARMOR) )
	{
		act("$N�n�F�n�Y�A���G�u�o�˪F��ڤ���סC�v", ch, NULL, mob, TO_CHAR);
		return;
	}
  
	if( obj->item_type == ITEM_ARMOR && obj->value[1] < 1 )
	{
		act("$N�n�F�n�Y�A���G�u�o�˪F��ڥ��S�a�I�v", ch, NULL, mob, TO_CHAR);
		return;
    }

	if( obj->level < 5 ) price = 5;
	else if (obj->level < 15) price = 10;
	else if ( obj->level < 30) price = 50;
	else if ( obj->level <= LEVEL_HERO ) price = 100;
	else price = 150;
	price = UMIN(price, ((obj->cost * 2) ));

	price *= obj->value[1]*100;
	if( IS_SET(obj->extra_flags, ITEM_BLESS)) adj += 1.5;
	if( IS_SET(obj->extra_flags, ITEM_HUM)) adj += 1.5;
	if( !(obj->extra_flags & ITEM_PERSONAL)) price *= 30;

	price = (int)(price * adj);
	price = UMAX( 100, price);

	sprintf(arg, "�ײz %s �ݭn %d �T�����C\n\r", obj->short_descr, price);
	send_to_char(arg, ch);
	fBank = FALSE;
	if( ch->gold < price )
	{
		fBank = TRUE;
		if( ch->bank < price ) {
			act("$N�n�F�n�Y�A���G�u�z���������C�v", ch, NULL, mob, TO_CHAR);
			return;
		}
	}

	if( fBank ){
		sprintf(arg, "�A�q�Ȧ�I�X %d �T�����C \n\r", price);
		ch->bank -= price;
	}else{
		sprintf(arg, "�A�I�X %d �T�����C \n\r", price);
		ch->gold -= price;
	}

	send_to_char(arg, ch);

	act("�A�� $p �浹$N�C", ch, obj, mob, TO_CHAR);
	act("$N�E��|���A$p�x�_�F�@�}���~�C", ch, obj, mob, TO_CHAR);
	act("$N�٩��B�Y�W�����]�A��$p���ٵ��A�C", ch, obj, mob, TO_CHAR);

	if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) ){
		obj->value[0] += obj->value[1];
		obj->value[1] = 0;
	}
	return;
}

void do_repair( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	CHAR_DATA *mob;
	char arg[ MAX_STRING_LENGTH];
	int curr_d, max_d, diff, price;
	float adj = 1.0;
	bool fBank = FALSE;
	if( !ch->in_room ) return;

	for( mob = ch->in_room->people ; mob; mob = mob->next_in_room){
		if( IS_NPC(mob) && IS_SET(mob->act, ACT_SMITH)) break;
	}

	if( !mob ) {
		send_to_char("�A�䤣��H���A�״_�˳ơC\n\r", ch);
		return;
	}

	one_argument(argument, arg);

	if( arg[0] == '\0') {
		send_to_char("�A�n�פ���H\n\r", ch);
		return;
	}

	if( !str_cmp("all", arg)) {
		for( obj = ch->carrying ; obj ; obj = obj->next_content )
		{
			if( obj->wear_loc != WEAR_NONE ) continue;

			if( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
				continue;

			curr_d = get_curr_dura(obj);
			max_d = get_max_dura(obj);
			diff = max_d - curr_d;

			if( diff < 1 || indestructible(obj) )    
			{ 
				continue;
			}

			if( obj->level < 5 )
				price = 1;
			else if (obj->level < 15)
				price = 2;
			else if ( obj->level < 30)
				price = 5;
			else if ( obj->level <= LEVEL_HERO )
				price = 10;
			else
				price = 30;

			price = URANGE(1,price, ((obj->cost * 2) ));

			price *= diff;
			if( IS_SET(obj->extra_flags, ITEM_BLESS))
				adj += 0.5;
			if( IS_SET(obj->extra_flags, ITEM_HUM))
				adj += 0.2;


			price = (int)(price * adj);
			price = UMAX( 100, price);
			// if( obj->item_type == ITEM_ARMOR && obj->value[1] > 0 )
			// price += obj-> level * 1000 * obj->value[1];

			sprintf(arg, "�ײz %s �ݭn %d �T�����C\n\r", obj->short_descr, price);
			send_to_char(arg, ch);
			fBank = FALSE;
			if( ch->gold < price ) 
			{
				fBank = TRUE;
				if( ch->bank < price ) 
				{
					act("$N�n�F�n�Y�A���G�u�z���������C�v", ch, NULL, mob, TO_CHAR);
					continue;
				}
			}

			if( fBank ) 
			{
				sprintf(arg, "�A�q�Ȧ�I�X %d �T�����C \n\r", price);
				ch->bank -= price;
			}
			else 
			{
				sprintf(arg, "�A�I�X %d �T�����C \n\r", price);
				ch->gold -= price;
			}
			send_to_char(arg, ch);

			act("�A�� $p �浹$N�C", ch, obj, mob, TO_CHAR);
			act("$N�@�}�V�V�����A$p�C�C�^�_�F���`�����A�C", ch, obj, mob, TO_CHAR);
			act("$N�٩��B�Y�W�����]�A��$p���ٵ��A�C", ch, obj, mob, TO_CHAR);

			repair_dura(obj, diff);
				/*if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) )
					{
					price += obj-> level * 10 * obj->value[1];
					obj->value[0] += obj->value[1];
					obj->value[1] = 0;
					}
				*/
		}
		return;
	}

	if( !(obj = get_obj_carry(ch, arg))){
		send_to_char("�A���W�S���o�˪F��C\n\r", ch);
		return;
	}

	//if( !can_drop_obj(ch, obj)){
	//  send_to_char("�A�S��k�⥦���H�C\n\r", ch);
	//  return;
	//  }

	curr_d = get_curr_dura(obj);
	max_d = get_max_dura(obj);
	diff = max_d - curr_d;

	if( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
	{
		act("$N�n�F�n�Y�A���G�u�o�˪F��ڨS��k�סC�v", ch, NULL, mob, TO_CHAR);
		return;
	}
	if( diff < 1 || indestructible(obj))
	{
		act("$N�n�F�n�Y�A���G�u�o�˪F��ڥ��S�a�I�v", ch, NULL, mob, TO_CHAR);
		return;
	}

	if( obj->level < 5 )
		price = 1;
	else if (obj->level < 15)
		price = 2;
	else if ( obj->level < 30)
		price = 5;
	else if ( obj->level <= LEVEL_HERO )
		price = 10;
	else
		price = 30;

	price *= diff;
	price = UMIN(price, ((obj->cost * 2) ));
	if( IS_SET(obj->extra_flags, ITEM_BLESS))
		adj += 0.5;
	if( IS_SET(obj->extra_flags, ITEM_HUM))
		adj += 0.2;


	price = (int)(price * adj);
	price = UMAX( 100, price);
	// if( obj->item_type == ITEM_ARMOR && obj->value[1] > 0 )
	// price += obj-> level * 1000 * obj->value[1];

	sprintf(arg, "�ײz %s �ݭn %d �T�����C\n\r", obj->short_descr, price);
	send_to_char(arg, ch);
	fBank = FALSE;
	if( ch->gold < price )
	{
		fBank = TRUE;
		if( ch->bank < price ) {
			act("$N�n�F�n�Y�A���G�u�z���������C�v", ch, NULL, mob, TO_CHAR);
			return;
		}
	}

	if( fBank )
	{
		sprintf(arg, "�A�q�Ȧ�I�X %d �T�����C \n\r", price);
		ch->bank -= price;
	}else{
		sprintf(arg, "�A�I�X %d �T�����C \n\r", price);
		ch->gold -= price;
	}
	send_to_char(arg, ch);

	act("�A�� $p �浹$N�C", ch, obj, mob, TO_CHAR);
	act("$N�@�}�V�V�����A$p�C�C�^�_�F���`�����A�C", ch, obj, mob, TO_CHAR);
	act("$N�٩��B�Y�W�����]�A��$p���ٵ��A�C", ch, obj, mob, TO_CHAR);

	repair_dura(obj, diff);
	/*if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) ){
	obj->value[0] += obj->value[1];
	obj->value[1] = 0;
	}*/

	return;
}

void do_wear( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	char place [MAX_INPUT_LENGTH];
	int iWear = 0;
	//static const   WEAR_MAP wear_table[17]= {
	static const   WEAR_MAP wear_table[24]= {
	{   "about",      ITEM_WEAR_ABOUT},
	{   "arm",        ITEM_WEAR_ARMS},
	{   "arrow",      ITEM_WEAR_ARROW},
	{   "body",       ITEM_WEAR_BODY },
	{   "feet",       ITEM_WEAR_FEET },
	{   "finger",     ITEM_WEAR_FINGER },
	{   "hand",       ITEM_WEAR_HANDS},
	{   "head",       ITEM_WEAR_HEAD },
	{   "hold",       ITEM_HOLD},
	{   "leg",        ITEM_WEAR_LEGS },
	{   "neck",       ITEM_WEAR_NECK },
	{   "shield",     ITEM_WEAR_SHIELD},
	{   "twohand",    ITEM_TWO_HAND},
	{   "waist",      ITEM_WEAR_WAIST},
	{   "weapon",     ITEM_WIELD},
	{   "wrist",      ITEM_WEAR_WRIST},
	//{   "ride",       ITEM_WEAR_RIDE}, // ride ����̫� 2022/04/05
	{   "wp1",        WEAR_WIELD},       // add at 2022/04/05
	{   "wp2",        WEAR_WIELD_2},     // add at 2022/04/05
	{   "hand1",      WEAR_HANDS},       // add at 2022/04/05
	{   "hand2",      WEAR_HANDS_2},     // add at 2022/04/05
	{   "feet1",      WEAR_FEET},        // add at 2022/04/05
	{   "feet2",      WEAR_FEET_2},      // add at 2022/04/05
	{   "ammo",       WEAR_AMMO},        // add at 2022/04/30  alc �u��
	{   "ride",       ITEM_WEAR_RIDE},
	};

	argument = one_argument( argument, arg );
	one_argument(argument, place);
	if( place[0] != '\0' ){
		int i;
		for( i = 0; i < 23 ; ++i ){
			if( !str_prefix(place, wear_table[i].name)){
				iWear = wear_table[i].place;
				break;
			}
		}
	}

	if ( arg[0] == '\0' )
	{
		send_to_char( "�n�˳Ƥ���H\n\r", ch );
		return;
	}

	if ( !str_cmp( arg, "all" ) )
	{
		OBJ_DATA *obj_next;

		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( obj->wear_loc != WEAR_NONE || !can_see_obj( ch, obj ) )
			continue;

			if ( CAN_WEAR( obj, ITEM_WIELD ) //some race cannot wield weapon
				&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_WEAPON_WIELD ) )//this check seem could put in
			continue;     //wear_obj()?

			wear_obj( ch, obj, FALSE, 0);
			if ( !IS_NPC( ch ) && ch->position == POS_DEAD )
			{
				raw_kill( ch, ch );
				break;
			}
		}
		return;
	}else{
		if ( !( obj = get_obj_carry( ch, arg ) ) )
		{
			send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_WEAPON_WIELD ) )
		{
			send_to_char( "�A�L�k�˳ƪZ��.\n\r", ch );
			return;
		}

		wear_obj( ch, obj, TRUE , iWear);
		if ( !IS_NPC( ch ) && ch->position == POS_DEAD )
			raw_kill( ch, ch );

	}

	return;
}


void do_fremove( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [ MAX_INPUT_LENGTH ];
    int   wear_loc;

    one_argument( argument, arg );

    if ( arg[0] == '\0' ){
        send_to_char( "�n��U����H\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "all" ) ){
        for( wear_loc = 0; wear_loc <= 20; wear_loc++ )
            fremove_obj( ch, wear_loc, TRUE );
        fremove_obj( ch, WEAR_FINGER_3, TRUE );
        fremove_obj( ch, WEAR_FINGER_4, TRUE );
        fremove_obj( ch, WEAR_ARMS_2, TRUE );
        fremove_obj( ch, WEAR_HANDS_2, TRUE );
        return;
    }

    if ( !( obj = get_obj_wear( ch, arg ) ) ){
        send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
        return;
    }

    fremove_obj( ch, obj->wear_loc, TRUE );
    return;
}

void do_remove( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [ MAX_INPUT_LENGTH ];
    int   wear_loc;

    one_argument( argument, arg );

    if ( arg[0] == '\0' ){
        send_to_char( "�n��U����H\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "all" ) ){
        for( wear_loc = 0; wear_loc <= 20; wear_loc++ )
			remove_obj( ch, wear_loc, TRUE );
        remove_obj( ch, WEAR_FINGER_3, TRUE );
        remove_obj( ch, WEAR_FINGER_4, TRUE );
        remove_obj( ch, WEAR_HANDS_2, TRUE );
        remove_obj( ch, WEAR_ARMS_2, TRUE );
        remove_obj( ch, WEAR_FEET_2, TRUE );
        remove_obj( ch, WEAR_LEGS_2, TRUE );
		remove_obj( ch, WEAR_AMMO, TRUE ); // �u�ĸ˳Ʀ�m add at 2022/04/30
        return;
    }

    if ( !( obj = get_obj_wear( ch, arg ) ) ){
        send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
        return;
    }

    remove_obj( ch, obj->wear_loc, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
	{
		send_to_char( "�ѯ��ܷP�§A���^�m�A�]�\\͢�H��|�����C", ch );
		act( "$n ��ۤv�^�m���ѯ��A�o�O�h�򰶤j�����ޡC", ch, NULL, NULL, TO_ROOM );
		return;
	}

	obj = get_obj_list( ch, arg, ch->in_room->contents );
	if ( !obj )
	{
		send_to_char( "�A�䤣��o�ӪF��C\n\r", ch );
		return;
	}

	/* ���� obj_act ��P�w, ���L�k sac �����~�]��Ĳ�o 2022/11/27
		if ( !CAN_WEAR( obj, ITEM_TAKE ) || IS_SET(obj->extra_flags, ITEM_NOSAC))
		{
			act( "$p �L�k�Q�^�m�C", ch, obj, NULL, TO_CHAR );
			return;
		} */

	// �ק� obj_act sac �]���� mob �@�� 2022/02/12
	//if( !IS_NPC(ch) && IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT) ){
		oprog_act_trigger("sac", ch, obj);
	}
	// obj �i��b obj_act �ɳQ purge 2022/02/13
	if( !obj || obj->deleted ) return;

	if ( !CAN_WEAR( obj, ITEM_TAKE ) || IS_SET(obj->extra_flags, ITEM_NOSAC))
	{
		act( "$p �L�k�Q�^�m�C", ch, obj, NULL, TO_CHAR );
		return;
	}

	if( !IS_SET(obj->extra_flags, ITEM_CLAN)) {
		// ���Q sac obj ��Q ch �ݨ� 2022/11/27 
		//send_to_char( "���F�P�§A���^�m�A�ѯ����F�A�@�Ǽ��y�C\n\r", ch );
		act( "�A�� $p �^�m���ѯ��A��o�F�@�Ǽ��y�C", ch, obj, NULL, TO_CHAR );
		if( !IS_NPC( ch ) )
			ch->gold += obj->level;
	}else{
		act( "�A�� $p �^�m���ѯ��C", ch, obj, NULL, TO_CHAR );
	}

	if ( obj->pIndexData->vnum == OBJ_VNUM_CORPSE_NPC && !IS_NPC( ch ) && get_skill_percent(ch, gsn_burial_ceremony ) > number_percent())
	{
		int nBurySklv = get_skill_level(ch, gsn_burial_ceremony);
		int nManaBonus= nBurySklv * nBurySklv / 10;
		ch->mana += obj->level + UMAX( 0, obj->level + get_skill_level(ch, gsn_burial_ceremony) - ch->level - 5 ) * 3
								+ (get_skill_level(ch, gsn_burial_ceremony)<<1);
		ch->mana += obj->level + UMAX( 0, obj->level + nBurySklv * 2 - ch->level - 5 ) * (3 + nManaBonus)
								+ nBurySklv * (nBurySklv + 4);
		send_to_char( "�A�N���̶W��A�ѯ����A�@�Ǫk�O�H���^���C\n\r", ch );
	}

	act( "$n �� $p �^�m���ѯ��C", ch, obj, NULL, TO_ROOM );

	// �s�W a_sac �Ω�t�Φr����ܫ� 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT) ){
		oprog_act_trigger("a_sac", ch, obj);
	}
	if( !obj || obj->deleted ) return;

	extract_obj( obj );
	return;
}

void do_adore( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;
	char arg[MAX_INPUT_LENGTH];
	int sklv = get_skill_level(ch, gsn_adore);

	if (   ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) )
		|| ( !IS_NPC( ch )
	  //&& ch->pcdata->learned[gsn_adore] < number_percent( ) ) )
			&& get_skill_percent( ch, gsn_adore ) < number_percent()))
	{
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}

	one_argument(argument, arg);
	if( arg[0] == '\0' ) sprintf( arg, "corpse");

	obj = get_obj_list( ch, arg, ch->in_room->contents );
	if ( !obj || obj->item_type != ITEM_CORPSE_NPC )
	{
		send_to_char( "�A�䤣�����C\n\r", ch );
		return;
	}

	if ( obj->item_type == ITEM_CORPSE_PC )
	{
		act( "��L�_�I�̪�����O����I���C", ch, obj, NULL, TO_CHAR );
		return;
	}

	if ( obj->level >= ch->level || obj->level > 36 )
	{
		send_to_char( "�ǵ۲��Y�������A�A��Գ��W�}�`���Ԥh�����̱R�����q�N�C\n\r", ch );
		act( ""HIP"$n�R�q�a��}�`�Ԥh�|����Y����§�A$s�����P�ɰ{�_���˪������C"NOR"", ch, NULL, NULL, TO_ROOM );
		if ( !is_affected( ch, gsn_adore ) && ch->hitroll > 2){

			// �[�J obj_act adore 2022/02/12
			if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
				oprog_act_trigger("adore", ch, obj);
			}

			// corpse �i��b prog �� �Q purge 2022/02/12
			if(!obj) return;

			af.type  = gsn_adore;
			if( obj->level < ch->level )
				af.duration  = obj->level / 4;
			else
				af.duration  = obj->level / 3;
			af.location  = APPLY_HITROLL;
			af.bitvector = 0;
			af.modifier  = - (obj->level / 6) + (sklv > 4 ? 1 : 0 );
			affect_to_char( ch, &af );

			if( obj->level < ch->level )
				af.duration  = obj->level / 4;
			else
				af.duration  = obj->level / 3;
			af.location  = APPLY_DAMROLL;
			af.bitvector = 0;
			af.modifier  = obj->level / 3 + sklv * 2 / 3;
			affect_to_char( ch, &af );
			// corpse �����N�u���b adore ���\�ɦ��� 2022/02/12
			extract_obj( obj );
		}else{
			send_to_char( "�A���ߤw�g�R���Ӧh�����F�C\n\r", ch );
		}
	}else{
		send_to_char( "�o���x�p�����ͭ����ȱo�A���L�q�C\n\r", ch );
	}
	// remove at 2022/02/12
	//extract_obj( obj );
	return;
}

void do_quaff( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "�n���Τ����ľ��H\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "�A���W�S���o�˪F��.\n\r", ch );
		return;
	}

	if ( obj->item_type != ITEM_POTION )
	{
		send_to_char( "�����O�]�k�ľ�.\n\r", ch );
		return;
	}

	if( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > ch->level * 5 )
	{
		send_to_char("�A�ܤ��U�F.\n\r", ch);
	}

	act( "�A�ܤU $p.", ch, obj, NULL ,TO_CHAR );
	act( "$n �ܤU $p.", ch, obj, NULL, TO_ROOM );

	if ( obj->level > ch->level )
	{
		act( "�A�L�k���ε��Ť�A�ٰ����]�k�ľ�.", ch, obj, NULL, TO_CHAR );
	}else{
		/* obj->value[0] is not used for potions */
		obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
		obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
		obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	}

	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
		oprog_act_trigger("quaff", ch, obj);

	if (    !IS_NPC( ch )
		|| ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
	{
		// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
		extract_obj( obj );
		if( ch->fighting )
			WAIT_STATE(ch, 8);
		else
			WAIT_STATE(ch, 2);
		// cond thirst �ק令 obj weight 2023/01/13
		//gain_condition( ch, COND_THIRST, 8);
		gain_condition( ch, COND_THIRST, obj->weight );
	}
	else
		WAIT_STATE(ch, 2);
	return;
}



void do_recite( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *scroll;
	OBJ_DATA  *obj;
	CHAR_DATA *victim;
	char       arg1 [ MAX_INPUT_LENGTH ];
	char       arg2 [ MAX_INPUT_LENGTH ];
	int sklv = get_skill_level(ch, gsn_scrolls);
	int obj_lv = 0;

	obj_lv += sklv;

	//if(sklv > 3) obj_lv += 1;
	//if(sklv > 4) obj_lv += 3;
	//if(sklv > 5) obj_lv += 5;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	if ( !( scroll = get_obj_carry( ch, arg1 ) ) ){
		send_to_char( "�A���W�S�����˪F��.\n\r", ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "�A���L�B�b�ʡA���O�S���n���o�X��.\n\r", ch );
		return;
	}
	
	if ( scroll->item_type != ITEM_SCROLL )
	{
		act( "�A�j�n��w��$p.", ch, scroll, NULL, TO_CHAR );
		// ���O scroll �]��Ĳ�o recite obj_act 2022/02/13
		if( IS_SET(scroll->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("recite", ch, scroll);
		}
		return;
	}

	/*check obj_act when recite scroll , add by coconet*/
	if( IS_SET(scroll->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("recite", ch, scroll);
	}

	// scroll �i��b obj_act �ɮ��� 2022/02/13
	if(!scroll) return;

	/*
	if ( scroll->item_type != ITEM_SCROLL )
	{
		send_to_char( "�����O�]�k���b.\n\r", ch );
		return;
	}
	*/

	obj = NULL;
	if ( arg2[0] == '\0' )
	{
		victim = ch;
		if ( ch->fighting )
			victim = ch->fighting;
	}
	else
	{
		if ( !( victim = get_char_room ( ch, arg2 ) )
			&& !( obj  = get_obj_here  ( ch, arg2 ) ) )
		{
			send_to_char( "�A�䤣��ؼ�.\n\r", ch );
			return;
		}
	}

	if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) ) || IS_AFFECTED( ch, AFF_CHARM ) )
	{
		act( "�A�{�b����ۥD.", ch, scroll, NULL, TO_CHAR );
		return;
	}
	
	if ( scroll->level > ch->level + sklv * 2 ){
		act( "�A�L�k�Իw��A���Ū��]�k���b.", ch, scroll, NULL, TO_CHAR );
		return;
	}


	// �� recite stun ���� sklv �v�T 2022/01/13
	//WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	WAIT_STATE( ch, 16 - sklv);

	act( "�A�Իw�F $p.", ch, scroll, NULL, TO_CHAR );
	act( "$n �Իw�F $p.", ch, scroll, NULL, TO_ROOM );

	/* Scrolls skill by Binky for EnvyMud, modified by Thelonius */
	if ( !IS_NPC( ch ) && !( number_percent( ) < get_skill_percent(ch, gsn_scrolls)+ sklv * 10))
	{
		switch ( number_bits( 3 ) )
		{
			case 0: 
			case 1:          
			case 2:
			case 3: 
				act( "�A�ڥ��ݤ��� $p �����e.", ch, scroll, NULL, TO_CHAR );
				return;          
			case 4:          
			case 5:          
			case 6:          
				send_to_char( "�A�@�w�O�����̩����F.\n\r", ch );
				act( "$p ��M�_���U�N�A�಴���N�Ӻ��.", ch, scroll, NULL, TO_CHAR );
				act( "$p ��M�_���U�N�A�಴���N�Ӻ��.", ch, scroll, NULL, TO_ROOM );
				extract_obj( scroll );
				return;
			case 7:
				act( "�A���Q�a�Իw�����A $p �o�X�@�}ģ�������~�A�ӫ�����F�C", ch, scroll, NULL, TO_CHAR );
				act( "$p �o�X�@�}ģ�������~�A�M������b�A���e�C", ch, scroll, NULL, TO_ROOM );
			  /*
			   * damage( ) call after extract_obj in case the damage would
			   * have extracted ch.  This is okay because we merely mark
			   * obj->deleted; it still retains all values until list_update.
			   * Sloppy?  Okay, create another integer variable. ---Thelonius
			   */
				extract_obj( scroll );
				////damage( ch, ch, scroll->level, gsn_scrolls, WEAR_NONE );
				damage( ch, ch, scroll->level, gsn_scrolls, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
				return;
		}
	}

	/* scroll->value[0] is now used for scrolls */

	// ���� TAR_CHAR_SELF �� spell �� obj �ϥΦ��\��y��crash add at 2021/09/15
	// ���� TAR_IGNORE �� TAR_ARG �� spell �ϥΧ줣��ؼгy�� crash add at 2021/09/22
	
	// first spell
	if ( skill_table[scroll->value[1]].target == TAR_CHAR_SELF && victim != ch ){
		obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[1]].target == TAR_IGNORE ){
		// �˳ƪ��] 2021/10/02
		if (   scroll->value[1] == 314
			|| scroll->value[1] == 315
			|| scroll->value[1] == 316
			|| scroll->value[1] == 317
			|| scroll->value[1] == 318
			|| scroll->value[1] == 319
			|| scroll->value[1] == 320
			|| scroll->value[1] == 321 )
			obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
		else
			obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[1]].target == TAR_ARG ){
		obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
	}else{
		obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, victim, obj );
	}
	// second spell
	if ( skill_table[scroll->value[2]].target == TAR_CHAR_SELF && victim != ch ){
		obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[2]].target == TAR_IGNORE ){
		if (   scroll->value[2] == 314
			|| scroll->value[2] == 315
			|| scroll->value[2] == 316
			|| scroll->value[2] == 317
			|| scroll->value[2] == 318
			|| scroll->value[2] == 319
			|| scroll->value[2] == 320
			|| scroll->value[2] == 321 )
			obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
		else
			obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[2]].target == TAR_ARG ){
		obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
	}else{
		obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, victim, obj );
	}
	// third spell
	if ( skill_table[scroll->value[3]].target == TAR_CHAR_SELF && victim != ch ){
		obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[3]].target == TAR_IGNORE ){
		if (   scroll->value[3] == 314
			|| scroll->value[3] == 315
			|| scroll->value[3] == 316
			|| scroll->value[3] == 317
			|| scroll->value[3] == 318
			|| scroll->value[3] == 319
			|| scroll->value[3] == 320
			|| scroll->value[3] == 321 )
			obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
		else
			obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[3]].target == TAR_ARG ){
		obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
	}else{
		obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, victim, obj );
	}
/*
	obj_cast_spell( scroll->value[1], scroll->value[0] , ch, victim, obj );
	obj_cast_spell( scroll->value[2], scroll->value[0] , ch, victim, obj );
	obj_cast_spell( scroll->value[3], scroll->value[0] , ch, victim, obj );
*/
	// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
	if ( !IS_NPC( ch )
		|| ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
		extract_obj( scroll );
	return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *staff;
	CHAR_DATA *vch, *victim;
	int        sn;
	int   chance;
	int sklv = get_skill_level(ch, gsn_staves);
	int obj_lv = 0;

	obj_lv += sklv;
	//if(sklv > 3) obj_lv += 1;
	//if(sklv > 4) obj_lv += 3;
	//if(sklv > 5) obj_lv += 5;
    
	if ( !( staff = get_eq_char( ch, WEAR_HOLD ) ) )
	{
		send_to_char( "�A���F���A������A�ݨӤ���Ƥ]�S�o�͡C\n\r", ch );
		return;
	}
 
 	if ( staff->item_type != ITEM_STAFF )
	{
		act( "�A���R��$p.", ch, staff, NULL, TO_CHAR );
		// ���O item_staff �]�i�HĲ�o brandish obj_act 2022/02/13
		if( IS_SET(staff->pIndexData->progtypes , OBJ_ACT))
		{
			oprog_act_trigger("brandish", ch, staff);
		}
		return;
	}
 
	if( IS_SET(staff->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("brandish", ch, staff);
	}

	// staff �i��b obj_act �ɳQ prog 2022/02/13
	if(!staff) return;

	/*
	if ( staff->item_type != ITEM_STAFF )
	{
		send_to_char( "�u���k���i�H���Ӵ��R.\n\r", ch );
		return;
	}
	*/

	// if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
	// || IS_AFFECTED( ch, AFF_CHARM ) )
	if ( IS_AFFECTED( ch, AFF_CHARM ) )
	{
		act( "�A�յ۴��R $p, ���O�A������ݨӤ�ť����.", ch, staff, NULL, TO_CHAR );
		act( "$n �յ۴��R $p, ���O����n������ť��.",  ch, staff, NULL, TO_ROOM );
		return;
	}

	if ( ( sn = staff->value[3] ) < 0
		|| sn >= MAX_SKILL
		|| skill_table[sn].spell_fun == 0 )
	{
		bug( "Do_brandish: bad sn %d.", sn );
		return;
	}

	//WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	WAIT_STATE( ch, 20 - sklv);

	if ( staff->value[2] > 0 )
	{
		CHAR_DATA *vch_next;

		act( "�A���� $p�A�޵o���t���]�O.",  ch, staff, NULL, TO_CHAR );
		act( "$n ���R $p.", ch, staff, NULL, TO_ROOM );

		/* Staves skill by Binky for EnvyMud, modified by Thelonius */
		/* NPC can use staff by Amenda */

		//ch->pcdata->learned[gsn_staves];
		// if ( !IS_NPC( ch ) && !( number_percent( ) < ch->pcdata->learned[gsn_staves] ) )
		chance = ( IS_NPC( ch ) ) ? ch->level * 2 + 10 : get_skill_percent( ch, gsn_staves) + sklv * 10;
		if ( chance < number_percent( ) )
		{
			switch ( number_bits( 3 ) )
			{
				case 0: 
				case 1:      
				case 2:      
				case 3: 
					act( "�A�L�k�޵o $p ���]�O.", ch, staff, NULL, TO_CHAR );
					act( "$n �L�k�޵o $p ���]�O.", ch, staff, NULL, TO_ROOM );
					return;        
				case 4:      
				case 5:      
				case 6:      
					act( "�A�޵o�F $p ���]�O, ���O���ߨ�����F.", ch, staff, NULL, TO_CHAR );
					act( "$n �޵o�F $p ���]�O, ���O���ߨ�����F.", ch, staff, NULL, TO_ROOM );
					if ( !IS_NPC( ch ) || ( IS_NPC( ch )
						&& ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
					{
						if ( --staff->value[2] <= 0 )
						{
							// �� z4 �� staff ���|���� add at 2020/10/18
							if( IS_SET(staff->pIndexData->souvenir, Z4) )
							{
								act( HIY"$p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, staff, NULL, TO_CHAR );
								act( HIY"$n �⤤�� $p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, staff, NULL, TO_ROOM );
								staff->value[2] = 0;
							}
							else
							{
								act( HIY"$p �o�X�@�}�j���A���ۤƬ����u."NOR, ch, staff, NULL, TO_CHAR );
								act( HIY"$n �⤤�� $p �o�X�@�}�j���A���ۤƬ����u."NOR, ch, staff, NULL, TO_ROOM );
								extract_obj( staff );
							}
						}
					}
					return;
				case 7:
					/*
					 * damage( ) call after extract_obj in case the damage would
					 * have extracted ch.  This is okay because we merely mark
					 * obj->deleted; it still retains all values until list_update.
					 * Sloppy?  Okay, create another integer variable. ---Thelonius
					 */
					// �� z4 �� staff ���|���� add at 2020/10/18
					if( IS_SET(staff->pIndexData->souvenir, Z4) )
					{
						act( HIR"�����I�A����F $p�A��q�q���Q��ӥX�I"NOR, ch, staff, NULL, TO_CHAR );
						act( HIR"$n ����F $p�A�Q�]�O���ˤF�I"NOR, ch, staff, NULL, TO_ROOM );
						staff->value[2] = 0;
					}
					else
					{
						act( HIR"�����I�A����F $p ���]�O�A���C�C�a�ݵۥ��b�A�⤤�Ƭ��H���I"NOR, ch, staff, NULL, TO_CHAR );
						act( HIR"$n �⤤�� $p ��M�Ʀ��H���I"NOR, ch, staff, NULL, TO_ROOM );
						extract_obj( staff );
					}
					//damage( ch, ch, staff->level, gsn_staves, WEAR_NONE );
					damage( ch, ch, staff->level, gsn_staves, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					return;
			}
		}
		// �������`���ŤU staff �����ϥΪ��]�w 2022/01/13
		//if ( sklv * 4 + staff->value[0] < 25 )
		if ( staff->value[0] < 0 )
		{
			if ( argument[0] == '\0' )
			{
				if ( ch->fighting )
					victim = ch->fighting;
				else
					victim = ch;
			}
			else if ( !( victim = get_char_room( ch, argument ) ) )
			{
				send_to_char( "�o�̨S���o�ӤH�C\n\r", ch );
				return;
			}
			if ( skill_table[sn].target == TAR_CHAR_SELF && victim != ch )
			return;
			obj_cast_spell( staff->value[3], staff->value[0] + obj_lv, ch, victim, NULL );
			staff->value[0] - obj_lv;
		}
		else for ( vch = ch->in_room->people; vch; vch = vch_next )
		{
			vch_next = vch->next_in_room;

			if ( vch->deleted )
				continue;
    			
			if( !IS_NPC(ch) && IS_SET(ch->act, PLR_WIZINVIS))
				continue;

			if( !IS_NPC(vch) && IS_SET(vch->act, PLR_WIZINVIS))//0941003 by jye �ư�imm char
				continue;

			switch ( skill_table[sn].target )
			{
				default:
					bug( "Do_brandish: bad target for sn %d.", sn );
					return;

				case TAR_IGNORE:
					if ( vch != ch )
						continue;
					break;

				case TAR_CHAR_OFFENSIVE:
					if ( is_same_group( ch, vch ) )
						continue;
					break;
				/*    
				case TAR_CHAR_DEFENSIVE:
					if ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
						continue;
						break;
				*/
				case TAR_CHAR_SELF:
					if ( vch != ch )
						continue;
					break;

				case TAR_CHAR_DEFENSIVE:
					break;
			}

			/* staff->value[0] is now used for staves */
			obj_cast_spell( staff->value[3], staff->value[0] + obj_lv, ch, vch, NULL );
		}
	}else{
		// staff v2 <= 0 �����p�U add at 2020/10/18
		act( "�A�յۤ޵o $p ���]�O, ���@�L�������.", ch, staff, NULL, TO_CHAR );
		act( "$n �յۤ޵o $p ���]�O, ���@�L�������.", ch, staff, NULL, TO_ROOM );
		return;
	}

	if ( !IS_NPC( ch ) || ( IS_NPC( ch )
		&& ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
	{
		// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
		if( number_percent() < 2 * sklv * sklv)
			return;

		if ( --staff->value[2] <= 0 )
		{
			// �� z4 �� staff ���|���� add at 2020/10/18
			if( IS_SET(staff->pIndexData->souvenir, Z4) )
			{
				act( HIY"$p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, staff, NULL, TO_CHAR );
				act( HIY"$n �⤤�� $p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, staff, NULL, TO_ROOM );
				staff->value[2] = 0;
			}
			else
			{
				act( HIY"$p �o�X�@�}�j���A���ۤƬ����u."NOR, ch, staff, NULL, TO_CHAR );
				act( HIY"$n �⤤�� $p �o�X�@�}�j���A���ۤƬ����u."NOR, ch, staff, NULL, TO_ROOM );
				extract_obj( staff );
			}
		}
	}

	return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *wand;
	OBJ_DATA  *obj;
	CHAR_DATA *victim;
	char       arg [ MAX_INPUT_LENGTH ];
	int        sn, chance;
	int        sklv = get_skill_level(ch, gsn_wands);
	int obj_lv = 0;

	obj_lv += sklv;
	//if(sklv > 3) obj_lv += 1;
	//if(sklv > 4) obj_lv += 3;
	//if(sklv > 5) obj_lv += 5;

	one_argument( argument, arg );
	if ( arg[0] == '\0' && !ch->fighting )
	{
		send_to_char( "Zap ����?\n\r", ch );
		return;
	}

	if ( !( wand = get_eq_char( ch, WEAR_HOLD ) ) )
	{
		send_to_char( "�A�u�F�u����A�ݨӤ���Ƥ]�S�o��.\n\r", ch );
		return;
	}
	
	if ( wand->item_type != ITEM_WAND )
	{
		act( "�A���ʵ�$p.", ch, wand, NULL, TO_CHAR );
		// ���O item_wand ���ⴤ�]��Ĳ�o 2022/02/13
		if( IS_SET(wand->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("zap", ch, wand);
		}
		return;
	}

	if( IS_SET(wand->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("zap", ch, wand);
	}

	// wand �i��b obj_act �Q purge 2022/02/13
	if(!wand) return;

	/* 
		if ( wand->item_type != ITEM_WAND )
		{
			send_to_char( "�����O�]��.\n\r", ch );
			return;
		}
	*/

	obj = NULL;
	if ( arg[0] == '\0' ){
		if ( ch->fighting ){
			victim = ch->fighting;
		}else{
			send_to_char( "Zap ����?\n\r", ch );
			return;
		}
    }else{
		if ( !( victim = get_char_room ( ch, arg ) ) && !( obj  = get_obj_here  ( ch, arg ) ) )
		{
			send_to_char( "�o�̨S���o�˪F��.\n\r", ch );
			return;
		}
	}

	//    if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
	//  || IS_AFFECTED( ch, AFF_CHARM ) )
	if ( IS_AFFECTED( ch, AFF_CHARM ) ){
		act( "�A�Q�n�ϥ� $p, ���O�A������ݨӤ�ť����.", ch, wand, NULL, TO_CHAR );
		act( "$n �յۨϥ� $p, ���O���⤣ť����.", ch, wand, NULL, TO_ROOM );
		return;
	}

	if ( ( sn = wand->value[3] ) < 0 || sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 ){
		bug( "Do_zap: bad sn %d.", sn );
		return;
    }

	//WAIT_STATE( ch, 2 * PULSE_VIOLENCE);
	WAIT_STATE( ch, 16 - sklv);

	if ( wand->value[2] > 0 )
	{
		if ( victim )
		{
			if ( victim == ch ){
				act( "�A��ۤv�ϥΤF $p.", ch, wand, NULL,   TO_CHAR );
				act( "$n ��ۤv�ϥΤF $p.",   ch, wand, NULL,   TO_ROOM );
			}else{
				act( "�A�� $N �ϥΤF $p.",    ch, wand, victim, TO_CHAR );
				act( "$n �� $N �ϥΤF $p.",   ch, wand, victim, TO_ROOM );
			}
		}else{
			act( "�A�� $P �ϥΤF $p.",      ch, wand, obj,    TO_CHAR );
			act( "$n �� $P �ϥΤF $p.",     ch, wand, obj,    TO_ROOM );
		}

		/* Wands skill by Binky for EnvyMud, modified by Thelonius */
		//ch->pcdata->learned[gsn_wands];
		//  if ( !IS_NPC( ch ) 
		//      && !( number_percent( ) < ch->pcdata->learned[gsn_wands] ) )
		chance = ( IS_NPC( ch ) ) ? ch->level * 2 + 10 : get_skill_percent(ch, gsn_wands)+sklv*2;
		if ( chance < number_percent( ) )
		{
			switch ( number_bits( 3 ) )
			{
				case 0: 
				case 1:      
				case 2:      
				case 3: 
					act( "�A�L�k�޵o $p ���]�O.", ch, wand, NULL, TO_CHAR );
					act( "$n �L�k�޵o $p ���]�O.", ch, wand, NULL, TO_ROOM );
					return;        
				case 4:      
				case 5:      
				case 6:      
					act( "�A�޵o�F $p ���]�O, ���O���ߨ�����F.", ch, wand, NULL, TO_CHAR );
					act( "$n �޵o�F $p ���]�O, ���O���ߨ�����F.", ch, wand, NULL, TO_ROOM );
					if ( !IS_NPC( ch ) || ( IS_NPC( ch )
						&& ( IS_AFFECTED( ch, AFF_CHARM )
						|| IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
					{
						if ( --wand->value[2] <= 0 )
						{
							// �� z4 �� wand ���|���� add at 2020/10/18
							if( IS_SET(wand->pIndexData->souvenir, Z4) )
							{
								act( HIY"$p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, wand, NULL, TO_CHAR );
								act( HIY"$n �⤤�� $p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, wand, NULL, TO_ROOM );
								wand->value[2] = 0;
							}
							else
							{
								act( HIY"$p �o�X�@�}�j���A�M��Ƭ����u."NOR, ch, wand, NULL, TO_CHAR );
								act( HIY"$n �⤤�� $p �o�X�@�}�j���A�M��Ƭ����u."NOR, ch, wand, NULL, TO_ROOM );
								extract_obj( wand );
							}
						}
					}
					return;
				case 7:
				/*
					* damage( ) call after extract_obj in case the damage would
					* have extracted ch.  This is okay because we merely mark
					* obj->deleted; it still retains all values until list_update.
					* Sloppy?  Okay, create another integer variable. ---Thelonius
				*/
					// �� z4 �� wand ���|���� add at 2020/10/18
					if( IS_SET(wand->pIndexData->souvenir, Z4) )
					{
						act( HIR"�����I�A����F $p�A��q�q���Q��ӥX�I"NOR, ch, wand, NULL, TO_CHAR );
						act( HIR"$n ����F $p�A�Q�]�O���ˤF�I"NOR, ch, wand, NULL, TO_ROOM );
						wand->value[2] = 0;
					}
					else
					{
						act( HIR"�����I�A����F $p ���]�O�A���C�C�a�ݵۥ��b�A�⤤�Ƭ��H���I"NOR, ch, wand, NULL, TO_CHAR );
						act( HIR"$n �⤤�� $p ��M�Ʀ��H���I"NOR, ch, wand, NULL, TO_ROOM );
						extract_obj( wand );
					}
					//damage( ch, ch, wand->level, gsn_wands, WEAR_NONE );
					damage( ch, ch, wand->level, gsn_wands, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					return;
			}
		}
		
		/* wand->value[0] is now used for wands */
		
		// ���� TAR_CHAR_SELF �� spell �� obj �ϥΦ��\��y��crash add at 2021/09/15
		// ���� TAR_IGNORE �� TAR_ARG �� spell �ϥΧ줣��ؼгy�� crash add at 2021/09/22
		
		if ( skill_table[wand->value[3]].target == TAR_CHAR_SELF && victim != ch ){
			obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
		}else if ( skill_table[wand->value[3]].target == TAR_IGNORE ){
			if (   wand->value[3] == 314
				|| wand->value[3] == 315
				|| wand->value[3] == 316
				|| wand->value[3] == 317
				|| wand->value[3] == 318
				|| wand->value[3] == 319
				|| wand->value[3] == 320
				|| wand->value[3] == 321 )
				obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
			else
				obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
		}else if ( skill_table[wand->value[3]].target == TAR_ARG ){
			obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
		}else{
			obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, victim, obj );
		}
    }
	else
	{
		// wand v2 <= 0 �����p�U add at 2020/10/18
		act( "�A�յۤ޵o $p ���]�O, ���@�L�������.", ch, wand, NULL, TO_CHAR );
		act( "$n �յۤ޵o $p ���]�O, ���@�L�������.", ch, wand, NULL, TO_ROOM );
		return;
	}

	// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) ){
	if ( !IS_NPC( ch ) || ( IS_NPC( ch )
		&& ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
	{
		if( number_percent() < 2 * sklv * sklv) return;
		if ( --wand->value[2] <= 0 )
		{
			// �� z4 �� wand ���|���� add at 2020/10/18
			if( IS_SET(wand->pIndexData->souvenir, Z4) )
			{
				act( HIY"$p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, wand, NULL, TO_CHAR );
				act( HIY"$n �⤤�� $p �Ӻ��]�O�o�X�@�}�j����v���t�H�U��."NOR, ch, wand, NULL, TO_ROOM );
				wand->value[2] = 0;
			}
			else
			{
				act( "$p �Ӻ��]�O�A�Ʀ��H��.", ch, wand, NULL, TO_CHAR );
				act( "$n �⤤�� $p �Ӻ��]�O�A�Ʀ��H��.", ch, wand, NULL, TO_ROOM );
				extract_obj( wand );
			}
		}
	}

	return;
}



void do_steal( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *victim;
	char       buf  [ MAX_STRING_LENGTH ];
	char       arg1 [ MAX_INPUT_LENGTH  ];
	char       arg2 [ MAX_INPUT_LENGTH  ];
	char       arg [ MAX_INPUT_LENGTH ];
	int        number;
	int        count;
	int        percent;
	int sklv = get_skill_level(ch, gsn_steal);

	if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_HORSE ) )
	{
		send_to_char( "����.\n\r", ch );
		return;
	}

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "�q�֨��W������?\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg2 ) ) )
	{
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		return;
	}

	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_HORSE ) )
	{
		send_to_char( "�A���ѤF�C\n\r",ch);
		return;
	}

	if ( victim == ch )
	{
		send_to_char( "�A�N�F�N�ۤv���f�U.\n\r", ch );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;

	obj = NULL;

	WAIT_STATE( ch, skill_table[gsn_steal].beats );

	/* Modified chances for stealing by Morpheus */
	percent = ch->level * 3/2; /* Base value */

	percent += number_range( -10, 10 ); /* Luck */

	percent -= victim->level * 2; /* Character level vs victim's */

	percent += sklv + sklv / 2 + (sklv/6)*4;

	if ( !IS_AWAKE( victim ) )
		percent += 20; /* Sleeping characters are easier */

	//percent += ch->pcdata->learned[gsn_steal]; /* Character ability */
	percent += get_skill_percent( ch, gsn_steal );

	if ( IS_AFFECTED( ch, AFF_SNEAK ) )
		percent += 5; /* Quiet characters steal better */

	if ( !can_see( ch, victim ) )
		percent += 10; /* Unseen characters steal better */

	percent = ( percent * weakness_percent( victim, gsn_steal)) / 100;
	if ( !str_prefix( arg1, "coins" ) || !str_cmp( arg1, "gold" ) )
		percent = (int) ( percent * 1.2 ); /* Gold is fairly easy to steal */
	else
	{
		number = number_argument( arg1, arg );
		count  = 0;
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( //obj->wear_loc == WEAR_NONE
				/*&&*/ can_see_obj( ch, obj )
				&& is_name( arg, obj->name ) )
			{
				if ( ++count == number )
				break;
			}
		}
  
		if ( !obj )
		{
			send_to_char( "�S���o�˪��~.\n\r", ch );
			return;
		}

		if ( obj->wear_loc == WEAR_NONE ) {
		/* Items wearing are harder */
			if( victim->level - ch->level < 3 )
				percent = (int) ( percent * .8 );
			else if (victim->level - ch->level < 8 )
				percent = (int) ( percent * .6 );
			else if (victim->level - ch->level < 15 )
				percent = (int) ( percent * .4 );
			else
				percent = (int) ( percent * .2 );
		}
		else
			percent = (int) ( percent * .05 );
	}
	if( IS_NPC(victim) && victim->pIndexData->pShop){
		percent = -1;
	}

	if ( ( !IS_NPC( victim )
		&& ( ( !IS_SET( victim->act, PLR_KILLER )
		&& !IS_SET( victim->act, PLR_THIEF )
		&& !IS_SET( victim->act, PLR_REGISTER )
		&& victim->race != race_lookup( "Vampire" ) )
		|| ch->level - victim->level < 5 ) )
		|| percent < number_percent( ) )
	{
	/* Failure.*/
		send_to_char( "�V�|�A�Q�o�{�F�I\n\r", ch );
		act( "�A�o�{ $n �Q�q�A���W���F��I\n\r", ch, NULL, victim, TO_VICT );
		act( "$n �Q�q $N ���W���F��A���O�Q�o�{�F�I \n\r",  ch, NULL, victim, TO_NOTVICT );
		sprintf( buf, "%s �O�ӥi�c���p���I", ch->name );
		do_shout( victim, buf );
		if ( IS_NPC( victim ) )
		{
			multi_hit( victim, ch, TYPE_UNDEFINED );
		}
		else
		{
			if ( !licensed( ch ) )
			{
				//  send_to_char(
				//       "�A�S�� PK ���ӡI  �A�l���F "HIG"400"NOR" �I�g��.\n\r", ch );
				//  gain_exp( ch, -400 );
				if ( registered( ch, victim ) && ch->level - victim->level < 6 )
					return;
			}

			if ( ch->level - victim->level > 5 || !registered( ch, victim ) )
			{
				log_string( buf );
				if ( !IS_SET( ch->act, PLR_THIEF ) )
				{
					SET_BIT( ch->act, PLR_THIEF );
					send_to_char( HIP "***  �A�O�Ӥp���I�I ***"NOR"\n\r", ch );
					save_char_obj( ch );
				}
			}
		}
		return;
	}

	if ( !str_prefix( arg1, "coins" ) || !str_cmp ( arg1, "gold" ) )
	{
		int amount;

		amount = victim->gold * number_range( 1, 10 ) / 20;
		if ( amount <= 0 )
		{
			send_to_char( "�A������b���.\n\r", ch );
			return;
		}

		ch->gold += amount;
		victim->gold -= amount;
		sprintf( buf, "�o��F�I  �A����F "HIY"%d"NOR" �T����.\n\r", amount );
		send_to_char( buf, ch );
		return;
	}

	if ( !can_drop_obj( ch, obj ) || IS_SET( obj->extra_flags, ITEM_INVENTORY ) || obj->level > ch->level )
	{
		send_to_char( "�A�N��F�A�i�O�o������.\n\r", ch );
		return;
	}

	if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
	{
		send_to_char( "�A�����U��h�F��F.\n\r", ch );
		return;
	}

	if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
	{
		send_to_char( "�A�a���ʧ�h���q�F.\n\r", ch );
		return;
	}

	//Modified by Razgriz 20050926
	if ( obj->wear_loc != WEAR_NONE )
	{
		send_to_char( "�A�N��F�A�i�O�o������.\n\r", ch );
		return;
		//unequip_char( victim, obj );
	}

	// �ԭz���� obj ���}�e 2022/02/12
	send_to_char( "�o��F�I\n\r", ch );
	send_to_char( "Ok.\n\r", ch );
	obj_from_char( obj );
	obj_to_char( obj, ch );

	// �s�W obj_act steal �P�w���Ѫ����� 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("steal", ch, obj);
	}

	// obj �i��b obj_act �ɳQ purge 2022/02/13
	if(!obj) return;

	// �s�W obj_act steal_target �P�w�Q�������� 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("steal_target", victim, obj);
	}
	
	//send_to_char( "�o��F�I\n\r", ch );
	//send_to_char( "Ok.\n\r", ch );
	return;
}



/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *keeper;
	SHOP_DATA *pShop;
	char       buf [ MAX_STRING_LENGTH ];

	pShop = NULL;

	for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
	{
			/*
			if ( IS_NPC( keeper ) && ( pShop = keeper->pIndexData->pShop )
				&& ( argument[0] == '\0' || is_name( argument, keeper->name ) ) )
			break;
			*/
		if ( !IS_NPC( keeper ) )
			continue;

		if ( IS_NPC( keeper ) && ( pShop = keeper->pIndexData->pShop ) && argument[0] == '\0' )
			break;
	}
		/*
		if ( !keeper || !pShop || ( keeper && IS_AFFECTED( keeper, AFF_CHARM ) ) )
		{
			send_to_char( "�A�L�k�o�� .\n\r", ch );
			return NULL;
		}
		*/
	/*
	if ( argument[0] != '\0' && !keeper ){
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		return NULL;
	}

	if ( !IS_NPC( keeper ) ){
		send_to_char( "���a�R��Х� bid �� auction ���O.\n\r", ch );
		return NULL;
	}
	*/
	if ( argument[0] != '\0' ){
		keeper = get_char_room( ch, argument );
		if ( keeper == NULL) {
			send_to_char( "�S���o�ӤH.\n\r", ch );
			return NULL;
		}
		if ( !IS_NPC( keeper ) ){
			send_to_char( "���a�R��Х� bid �� auction ���O.\n\r", ch );
			return NULL;
		}
		pShop = keeper->pIndexData->pShop;
	}

	if ( !keeper ){
		send_to_char( "�o�̨S���H��A�i��R��.\n\r", ch );
		return NULL;
	}

	if ( !pShop ){
		send_to_char( "�L����A�i��R��.\n\r", ch );
		return NULL;
	}

	if ( keeper && IS_AFFECTED( keeper, AFF_CHARM ) ){
		send_to_char( "�L�S���R�檺�N�@.\n\r", ch );
		return NULL;
	}

    /*
     * Undesirables.
     */
	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_KILLER ) )
	{
		do_say( keeper, "�o�̤��w�����I" );
		sprintf( buf, "�i�c������ %s �b�o�̡I\n\r", ch->name );
		do_shout( keeper, buf );
		return NULL;
	}

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_THIEF ) )
	{
		do_say( keeper, "�o�̤��w��p���I" );
		sprintf( buf, "�i�c���p�� %s �b�o�̡I \n\r", ch->name );
		do_shout( keeper, buf );
		return NULL;
	}

	/*
     * Shop hours.
	*/
		/*      old version
		if ( time_info.hour < pShop->open_hour )
		{
			do_say( keeper, "��p�A�����|���}�i�A�б��I�A��." );
			return NULL;
		}

		if ( time_info.hour > pShop->close_hour )
		{
			do_say( keeper, "���Ѥw�g���L�F�A���ѽЦ�." );
			return NULL;
		} */
	
	// �s�Wopen > close(�]����~ or ��]�A�˰ө�) �� open = close ���P�w , 20/02/24
	if ( pShop->close_hour > pShop->open_hour ){
		if ( time_info.hour < pShop->open_hour ){
			do_say( keeper, "�����|���}�i�A�б��I�A��." );
			return NULL;
		}

		if ( time_info.hour > pShop->close_hour ){
			do_say( keeper, "���Ѥw�g���L�F�A���ѽЦ�." );
			return NULL;
		}
	}else if( pShop->close_hour < pShop->open_hour ){
		if ( time_info.hour < pShop->open_hour && time_info.hour - pShop->close_hour > 0){
			do_say( keeper, "���Ѥw�g���L�F�A���ѽЦ�." );
			return NULL;
		}
	}else{
		if ( time_info.hour != pShop->open_hour ){
			do_say( keeper, "���Ѥw�g���L�F�A���ѽЦ�." );
			return NULL;
		}
	}


    /*
     * Invisible or hidden people.
     */
	if ( !can_see( keeper, ch ) )
	{
		do_say( keeper, "�֦b�M�ڻ��ܡH" );
		return NULL;
	}

	return keeper;
}



int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
	SHOP_DATA *pShop;
	int cost;

	if ( !obj || !( pShop = keeper->pIndexData->pShop ) )
	return 0;

	if ( fBuy )
	{
		cost = obj->cost * UMAX(pShop->profit_buy,100)  / 100;
	}
	else
	{
		OBJ_DATA *obj2;
		int itype;

		cost = 0;
		for ( itype = 0; itype < MAX_TRADE; itype++ )
		{
			if ( obj->item_type == pShop->buy_type[itype] )
			{
				cost = obj->cost * UMIN(pShop->profit_sell, 100) / 100;
				break;
			}
		}

		for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
		{
			if ( obj->pIndexData == obj2->pIndexData )
			cost /= 2;
		}
	}

	if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
		cost = cost * obj->value[2] / obj->value[1];

	return cost;
}

// �s�W�H qmark �P�w���D��~ 2022/05/05
bool not_for_sale( CHAR_DATA *ch, CHAR_DATA *keeper, OBJ_DATA *obj )
{
	
	/*  �ثe�� 5 �� qmark , �C�� value �N��@�� obj vnum
		�]���C�� mob �঳ 15 ��D��~
		�ӤH not_for_sale v0 v1 v2 �Ȥ��O���� �R�a for_sale �� v0 �� : 

							v0       v1       v3
		not_for_sale_1 :    1        2        4
		not_for_sale_2 :    8        16       32
		not_for_sale_3 :    64       128      256
		not_for_sale_4 :    512      1024     2048
		not_for_sale_5 :    4096     8192     16384

		�p�G�ȹ�����, �h not_for_sale �N������ (�R�a�i�H�ʶR�Ӫ��~)
		��: ���ξ�� list �� buy ���s��, �o�ǥ��ӴN�� char ���P�Ӥ��P */

	QMARK *qm;
	char saleqm [ MAX_INPUT_LENGTH ];

	// �P�w �R�a ���S�� for_sale_vnum( mob �ӤH�� vnum ) �� qmark
	sprintf( saleqm , "for_sale_%d", keeper->pIndexData->vnum );
	qm = is_qmark(ch, saleqm);

	// �P�w obj vnum �O�_���� �ӤH ���W�S�w qmark value
	if( is_qmark( keeper , "not_for_sale_1" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_1" )->v0 )
		{
			// �P�w �R�a ���W�O�_�������� qmark value
			if( qm && qm->v0 & 1 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_1" )->v1 )
		{
			if( qm && qm->v0 & 2 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_1" )->v2 )
		{
			if( qm && qm->v0 & 4 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_2" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_2" )->v0 )
		{
			if( qm && qm->v0 & 8 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_2" )->v1 )
		{
			if( qm && qm->v0 & 16 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_2" )->v2 )
		{
			if( qm && qm->v0 & 32 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_3" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_3" )->v0 )
		{
			if( qm && qm->v0 & 64 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_3" )->v1 )
		{
			if( qm && qm->v0 & 128 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_3" )->v2 )
		{
			if( qm && qm->v0 & 256 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_4" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_4" )->v0 )
		{
			if( qm && qm->v0 & 512 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_4" )->v1 )
		{
			if( qm && qm->v0 & 1024 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_4" )->v2 )
		{
			if( qm && qm->v0 & 2048 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_5" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_5" )->v0 )
		{
			if( qm && qm->v0 & 4096 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_5" )->v1 )
		{
			if( qm && qm->v0 & 8192 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_5" )->v2 )
		{
			if( qm && qm->v0 & 16384 )
				return FALSE;
			else
				return TRUE;
		}
	}

	// �p�G obj vnum �S�����ӯS�w�� qmark �h������
	return FALSE;

}

/*
 * Multiple object buy modifications by Erwin Andreasen
 * Obtained from Canth's snippets page at:
 * http://www.xs4all.nl/~phule/snippets/snippets.html
 */
void do_buy( CHAR_DATA *ch, char *argument )
{
	char arg  [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	char arg3 [ MAX_INPUT_LENGTH ];
	int type;
	QMARK *qmark;

	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg2);
	one_argument( argument, arg3 );

	if ( arg[0] == '\0' ){
		send_to_char( "�n�R����H\n\r", ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags, ROOM_PET_SHOP ) )
	{
		CHAR_DATA *pet;
		ROOM_INDEX_DATA *pRoomIndexNext;
		ROOM_INDEX_DATA *in_room;
		char buf [ MAX_STRING_LENGTH ];

		if ( IS_NPC( ch ) )
			return;

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

		if ( !pRoomIndexNext ){
			bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "�o�̨S���樺�˪F��.\n\r", ch );
			return;
		}

		in_room     = ch->in_room;
		ch->in_room = pRoomIndexNext;
		pet     = get_char_room( ch, arg );
		ch->in_room = in_room;

		if ( !pet || !IS_SET( pet->act, ACT_PET ) ){
			send_to_char( "�o�̨S���樺�˪F��.\n\r", ch );
			return;
		}
		/*
		  if ( IS_SET( ch->act, PLR_BOUGHT_PET ) && !IS_IMMORTAL( ch ) )
		  {
			  send_to_char( "�A�w�g���@���d���F.\n\r", ch );
			  return;
		  }
		*/
		if ( ch->gold < pet->level * pet->level * pet->level ){
			send_to_char( "�A�R���_.\n\r", ch );
			return;
		}

		if ( !can_see( ch, ch ) ){
			send_to_char( "�A�D���d���ݤ���A�I\n\r", ch );
			return;
		}
		
		if ( ch->level < pet->level ){
			send_to_char( "�A�����Ť���.\n\r", ch );
			return;
		}

		if ( ( qmark = ch->qmark ) )
		for( qmark = ch->qmark; qmark != NULL; qmark = qmark->next )
			if ( !str_cmp( qmark->mark_name, "bought_pet_timer" ) )
			{
				send_to_char( "�A���[�e�~��R�L�@���d���A�����L�@�}�l�~��A�R�C\n\r", ch );
				return;
			}
			
		ch->gold -= pet->level * pet->level * pet->level;
		pet   = create_mobile( pet->pIndexData );
		pet->level = pet->pIndexData->level;
		//  SET_BIT( ch->act,    PLR_BOUGHT_PET );
		SET_BIT( pet->act, ACT_PET );
		SET_BIT( pet->affected_by, AFF_CHARM );
		pet->clan = ch->clan;
		
		one_argument( argument, arg );

		if ( arg[0] != '\0' ){
			sprintf( buf, "%s %s", pet->name, arg );
			free_string( pet->name );
			pet->name = str_dup( buf );
		}

		sprintf( buf, "%s����W�g�ۡG�u�ڪ��D�H�O %s�v.\n\r",
		pet->description, /*ch->clan ? ch->clan->name : */ ch->name );
		free_string( pet->description );
		pet->description = str_dup( buf );

		char_to_room( pet, ch->in_room );
		mprog_repop_trigger(pet);
		//if ( !ch->clan )
		add_follower( pet, ch );
		send_to_char( "�Цn�n���ݧA���d��.\n\r", ch );
		act( "$n �R�U�F�@�� $N.", ch, NULL, pet, TO_ROOM );

		sprintf( arg, "%s ", ch->name );
		strcat( arg, "bought_pet_timer 30 0 0 0 0 0" );
		do_mpadd_qmark( pet, arg );
		return;
	}
    else if ( IS_SET( ch->in_room->room_flags, ROOM_HORSE_SHOP ) )
    {
		CHAR_DATA *horse;
		ROOM_INDEX_DATA *pRoomIndexNext;
		ROOM_INDEX_DATA *in_room; // temp for IN_ROOM
		char buf [ MAX_STRING_LENGTH ];

		if ( IS_NPC( ch ) )
			return;

		if ( get_skill_percent( ch, gsn_ride ) == 0 ){
			send_to_char("�A�S���|�M���I�R���M������I\n\r",ch);
			return;
		}

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
		if ( !pRoomIndexNext ){
			bug( "Do_buy: bad horse shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "�o�̨S����o�ا��M.\n\r", ch );
			return;
		}

		in_room     = ch->in_room;
		ch->in_room = pRoomIndexNext;
		horse       = get_char_room( ch, arg );
		ch->in_room = in_room;

		if ( !horse || !IS_SET( horse->act, ACT_HORSE ) ){
			send_to_char( "�o�̨S����o�ا��M.\n\r", ch );
			return;
		}

		if ( ch->gold < horse->level * horse->level * horse->level ){
			send_to_char( "�A�R���_.\n\r", ch );
			return;
		}

		if ( !can_see( horse, ch ) ){
			send_to_char( "�A�D�����M�ݤ���A�I\n\r", ch );
			return;
		}
    
		if ( ch->level < horse->level ){
			send_to_char( "�A�����Ť���.\n\r", ch );
			return;
		}

		if ( ( qmark = ch->qmark ) )
		for( qmark = ch->qmark; qmark != NULL; qmark = qmark->next )
			if ( !str_cmp( qmark->mark_name, "bought_horse_timer" ) )
			{
				send_to_char( "�A���O���[�e�~�R�L�@�ǰ��ܡH\n\r", ch );
				return;
			}

		ch->gold -= horse->level * horse->level * horse->level ;
		horse   = create_mobile( horse->pIndexData );
		horse->level = horse->pIndexData->level;
		//  SET_BIT( ch->act,    PLR_BOUGHT_PET );
		SET_BIT( horse->act,     ACT_HORSE    );
		SET_BIT( horse->act,  ACT_SENTINEL      );
		//  SET_BIT( horse->affected_by, AFF_CHARM    );
  
		one_argument( argument, arg );

		if ( arg[0] != '\0' ){
			sprintf( buf, "%s %s", horse->name, arg );
			free_string( horse->name );
			horse->name = str_dup( buf );
		}

		char_to_room( horse, ch->in_room );
		mprog_repop_trigger(horse);
		add_rember( horse , ch );
		//if ( !ch->clan )
		add_follower( horse, ch );
		send_to_char( "�Цn�n���ݧA�����M.\n\r", ch );
		act( "$n ���ΤF�@�� $N.", ch, NULL, horse, TO_ROOM );
		sprintf( arg, "%s ", ch->name );
		strcat( arg, "bought_horse_timer 30 0 0 0 0 1" );
		do_mpadd_qmark( horse, arg );
		return;
	}
	else
	{   // keeper ��ܤW�� 2022/05/05
		OBJ_DATA  *obj;
		CHAR_DATA *keeper;
		int cost;
		int item_count = 1; /* buy only one by default */

		if ( is_number( arg ) )
		{
			if( arg2[0] != '\0' ) {
				item_count = atoi( arg );
				strcpy( arg, arg2 );
				strcpy( arg2, arg3 );
			}
		}

		if ( !( keeper = find_keeper( ch, arg2 ) ) )
			return;

		if( is_number(arg) )
		{
			int i = 1;
			type = atoi(arg);
			for( obj = keeper->carrying; obj; obj = obj->next_content, i++ )
			{
				if ( obj->wear_loc != WEAR_NONE
					|| ( cost = get_cost( keeper, obj, TRUE ) ) < 0
					|| not_for_sale( ch, keeper, obj ) ) // �s�W qmark �P�w�D��~ 2022/05/05
				{
					--i;
					continue;
				}
				if(i == type) break;
			}
		}else{
			obj = get_obj_carry( keeper, arg );
		}

		cost = get_cost( keeper, obj, TRUE );

		if ( cost <= 0 || !can_see_obj( ch, obj ) || not_for_sale( ch, keeper, obj ) ) // �s�W qmark �P�w�D��~ 2022/05/05
		{
			act( "$n"HIG"�i�D�A�u�ڨS����o�˪F��A�Х� 'list' �ݬݧڪ��f���C�v"NOR"", keeper, NULL, ch, TO_VICT );
			ch->reply = keeper;
			return;
		}

		if ( item_count < 1 ){
			send_to_char( "�n�R�h�֡H\n\r", ch );
			return;
		}

		if ( ch->gold < ( cost * item_count ) )
		{
			if ( item_count == 1 ){
				act( "$n"HIG"�i�D�A�u�A���W�S�����������C�v"NOR"", keeper, obj, ch, TO_VICT );
				ch->reply = keeper;
				return;
			}else{
				char buf [ MAX_STRING_LENGTH ];

				if ( ( ch->gold / cost ) > 0 )
					sprintf( buf, "$n"HIG"�i�D�A�u�A�����u���R %d �ӡC�v"NOR"", ( ch->gold / cost ) );
				else
					sprintf( buf, "$n"HIG"�i�D�A�u�A�s�@�Ӥ]�R���_�C�v"NOR"" );

				act( buf, keeper, obj, ch, TO_VICT );
				ch->reply = keeper;
				return;
			}
		}
  
		if ( obj->level > ch->level ){
			act( "$n"HIG"�i�D�A�u�A�٤���ϥ� $p�C�v"NOR"",
			keeper, obj, ch, TO_VICT );
			ch->reply = keeper;
			return;
		}

		if ( ch->carry_number + ( item_count * get_obj_number( obj ) ) > can_carry_n( ch ) ){
			send_to_char( "�A�L�k�A���ʧ�h�F��F.\n\r", ch );
			return;
		}

		if ( ch->carry_weight + ( item_count * get_obj_weight( obj ) ) > can_carry_w( ch ) ){
			send_to_char( "�A�L�k�A���ʧ�h���q�F.\n\r", ch );
			return;
		}

		if ( ( item_count > 1 ) && !IS_SET( obj->extra_flags, ITEM_INVENTORY ) ){
			act( "$n"HIG"�i�D�A�u��p, $p �u�ѤU�@�ӤF�C�v"NOR"", keeper, obj, ch, TO_CHAR );
			ch->reply = keeper;
			return;
		}

		//Anti-duper by Chris
		if ( obj && keeper->pIndexData->pShop && keeper->pIndexData->pShop->profit_buy < 100){
			bugf("Dupe by Shops : %s try to buy %s to mob %d for %d percents cost!",
			ch->name, obj->short_descr, keeper->pIndexData->vnum, keeper->pIndexData->pShop->profit_buy );
		}


		if ( item_count == 1 ){
			act( "�A�R�U $p.", ch, obj, NULL, TO_CHAR );
			act( "$n �R�U $p.", ch, obj, NULL, TO_ROOM );
		}else{
			char buf [ MAX_STRING_LENGTH ];

			sprintf( buf, "$n �R�U %d * $p.", item_count );
			act( buf, ch, obj, NULL, TO_ROOM );
			sprintf( buf, "�A�R�U %d * $p.", item_count );
			act( buf, ch, obj, NULL, TO_CHAR );
		}

		ch->gold -= cost * item_count;

		if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
		for ( ; item_count > 0; item_count-- )
		{
			obj = fcreate_object( obj->pIndexData, obj->level );
			obj_to_char( obj, ch );
		}else{
			obj_from_char( obj );
			obj_to_char( obj, ch );
		}

		// �s�W obj_act buy �w��R���~������ 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("buy", ch, obj);
		}

		// obj �i��b obj_act �ɳQ purge 2022/02/13
		if(!obj) return;

		// �s�W obj_act buy_target �w��Q�R���~������(npc �ӤH) 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("buy_target", keeper, obj);
		}

		return;
    }
}



void do_list( CHAR_DATA *ch, char *argument )
{
	char buf  [ MAX_STRING_LENGTH   ];
	char buf1 [ MAX_STRING_LENGTH*4 ];
	int wear_filter = 0;
	//static const   WEAR_MAP wear_table[17]=
	static const   WEAR_MAP wear_table[18]=
	{
		{   "about",      ITEM_WEAR_ABOUT},
		{   "arm",        ITEM_WEAR_ARMS},
		{   "arrow",      ITEM_WEAR_ARROW},
		{   "body",       ITEM_WEAR_BODY },
		{   "feet",       ITEM_WEAR_FEET },
		{   "finger",     ITEM_WEAR_FINGER },
		{   "hand",       ITEM_WEAR_HANDS},
		{   "head",       ITEM_WEAR_HEAD },
		{   "hold",       ITEM_HOLD},
		{   "leg",        ITEM_WEAR_LEGS },
		{   "neck",       ITEM_WEAR_NECK },
		{   "shield",     ITEM_WEAR_SHIELD},
		{   "twohand",    ITEM_TWO_HAND},
		{   "waist",      ITEM_WEAR_WAIST},
		{   "weapon",     ITEM_WIELD},
		{   "wrist",      ITEM_WEAR_WRIST},
		{   "ammo",       ITEM_WEAR_AMMO}, // �u�Ħ�m 2022/04/30
		{   "ride",       ITEM_WEAR_RIDE},
	};

	buf1[0] = '\0';

	if ( IS_SET( ch->in_room->room_flags, ROOM_PET_SHOP ) )
	{
		CHAR_DATA *pet;
		ROOM_INDEX_DATA *pRoomIndexNext;
		bool     found;

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

		if ( !pRoomIndexNext ){
			bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "�A�L�k�o�� .\n\r", ch );
			return;
		}

		found = FALSE;

		for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
		{
			if ( IS_SET( pet->act, ACT_PET ) ){
				if ( !found ){
					found = TRUE;
					strcat( buf1, "�d���X��:\n\r" );
				}
				sprintf( buf, "[%2d] %8d - %s\n\r",
				pet->level,
				pet->level * pet->level * pet->level,
				pet->short_descr );
				strcat( buf1, buf );
			}
		}
		if ( !found )
			send_to_char( "��p�A�����ثe�ʳf.\n\r", ch );

		send_to_char( buf1, ch );
		return;
	}
	else if ( IS_SET( ch->in_room->room_flags, ROOM_HORSE_SHOP ) )
	{
		CHAR_DATA *horse;
		ROOM_INDEX_DATA *pRoomIndexNext;
		bool     found;

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
		if ( !pRoomIndexNext ){
			bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "�A�L�k�o�� .\n\r", ch );
			return;
		}

		found = FALSE;

		for ( horse = pRoomIndexNext->people; horse; horse = horse->next_in_room )
		{
			if ( IS_SET( horse->act, ACT_HORSE ) ){
				if ( !found ){
					found = TRUE;
					strcat( buf1, "���M�X��:\n\r" );
				}
				sprintf( buf, "[%2d] %8d - %s\n\r",
				horse->level,
				horse->level * horse->level * horse->level,
				horse->short_descr );
				strcat( buf1, buf );
			}
		}
		if ( !found )
			send_to_char( "��p�A�����ثe�ʳf.\n\r", ch );

		send_to_char( buf1, ch );
		return;
	}else{
		OBJ_DATA  *obj;
		CHAR_DATA *keeper;
		char arg  [ MAX_INPUT_LENGTH ];
		char arg2 [ MAX_INPUT_LENGTH ];
		int cost;
		bool found;
		int count = 0;
		int i;

		argument = one_argument( argument, arg );
		one_argument( argument, arg2 );

		if ( !( keeper = find_keeper( ch, arg2 ) ) )
			return;
		
		found = FALSE;
		if( arg[0] != '\0' ){
			// add wear loc ammo 2022/04/30
			//for( i = 0; i < 16 ; ++i )
			for( i = 0; i < 17 ; ++i )
			{
				if( !str_cmp( arg, wear_table[i].name) ){
					wear_filter = wear_table[i].place;
					break;
				}
			}
		}
		for ( i = 1,obj = keeper->carrying; obj; obj = obj->next_content,++i )
		{
			// obj �Q�ӤH��� �άO ����p�� 0  �h���|��ܦb list ��
			if ( obj->wear_loc != WEAR_NONE
				|| ( cost = get_cost( keeper, obj, TRUE ) ) < 0
				|| not_for_sale( ch, keeper, obj ) ) // �s�W qmark �P�w�D��~ 2022/05/05
			{
				--i;
				continue;
			}

			if ( can_see_obj( ch, obj )
				&& (arg[0] == '\0'
					|| !str_cmp( arg, "all" )
					|| (CAN_WEAR(obj, wear_filter))
					|| is_name( arg, obj->name )) )
			{
				if ( !found )
				{
					found = TRUE;
					strcat( buf1, "[�s��   Lv  �� �� ] �� �~ �W ��\n\r" );
				}

				sprintf( buf, "[%4d   %2d %7d] %s.\n\r", i ,obj->level, cost, capitalize( obj->short_descr ) );
				++count;
				strcat( buf1, buf );
			}

			if ( count >= 35 )
			{
				count = 0;
				send_to_char(buf1, ch);
				buf1[0] = '\0';
			}
		}

		if ( !found )
		{
			if ( arg[0] == '\0' )
				send_to_char( "�o�̶R����F��.\n\r", ch );
			else
				send_to_char( "�L���樺�˪F��.\n\r", ch );
			return;
		}

		send_to_char( buf1, ch );
		return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *keeper;
	char       buf  [ MAX_STRING_LENGTH ];
	char       arg  [ MAX_INPUT_LENGTH  ];
	char       arg2 [ MAX_INPUT_LENGTH  ];
	int        cost;

	argument = one_argument( argument, arg );
	one_argument( argument, arg2 );

	if ( arg[0] == '\0' )
	{
		send_to_char( "�n�椰��?\n\r", ch );
		return;
	}

	if ( !( keeper = find_keeper( ch, arg2 ) ) )
		return;

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		act( "$n �i�D�A, '�A�S�����˪F��'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
    }

	if ( !can_drop_obj( ch, obj ) )
	{
		send_to_char( "�A�L�k��U��.\n\r", ch );
		return;
	}

	if ( !can_see_obj( keeper, obj ) )
	{
		act( "$n �i�D�A, '�ڬݤ���A�Q�檺�F��'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	//Anti-duper by Chris
	if ( obj && keeper->pIndexData->pShop && keeper->pIndexData->pShop->profit_sell > 100)
	{
		bugf("Dupe by Shops : %s try to sell %s to mob %d for %d percents profit!",
			ch->name, obj->short_descr, keeper->pIndexData->vnum, keeper->pIndexData->pShop->profit_sell );
	}
  
  
    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0
		|| obj->level > LEVEL_IMMORTAL 
		|| IS_SET(obj->extra_flags, ITEM_CLAN))
	{
		act( "$n �n�n�Y�A�ݨӹ� $p �@�I����]�S��.", keeper, obj, ch, TO_VICT );
		return;
	}

	if ( IS_SET( obj->extra_flags, ITEM_POISONED ) )
	{
		act( "$n �i�D�A, '�ڤ��R���r���F��'", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	sprintf( buf, "�A�H %d �T�����������汼�F $p.", cost );
	act( buf, ch, obj, NULL, TO_CHAR );
	act( "$n �汼 $p.", ch, obj, NULL, TO_ROOM );
	ch->gold   += cost;

	if ( obj->item_type == ITEM_TRASH )
	{
		extract_obj( obj );
	}
	else
	{
		obj_from_char( obj );
		obj_to_char( obj, keeper );
	}

	// �s�W obj_act sell �w��檫�~������ 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("sell", ch, obj);
	}

	// obj �i��b obj_act �ɳQ purge 2022/02/13
	if(!obj) return;

	// �s�W obj_act sell_target �w�怜�ʪ��~������(npc �ӤH) 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("sell_target", keeper, obj);
	}

	return;
}



void do_value( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *keeper;
	char       buf  [ MAX_STRING_LENGTH ];
	char       arg  [ MAX_INPUT_LENGTH  ];
	char       arg2 [ MAX_INPUT_LENGTH  ];
	int        cost;

	argument = one_argument( argument, arg );
	one_argument( argument, arg2 );

	if ( arg[0] == '\0' )
	{
		send_to_char( "�n���������?\n\r", ch );
		return;
	}

	if ( !( keeper = find_keeper( ch, arg2 ) ) )
		return;

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		act( "$n �i�D�A, '�A�S�����˪F��'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
		send_to_char( "�A�L�k��U��.\n\r", ch );
		return;
	}

	if ( !can_see_obj( keeper, obj ) )
	{
		act( "$n �i�D�A, '�ڤ���o���Ų�w�Ů�'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
	{
		act( "$n �n�n�Y�A�ݨӹ� $p �@�I����]�S��.", keeper, obj, ch, TO_VICT );
		return;
	}

	if ( IS_SET( obj->extra_flags, ITEM_POISONED ) )
	{
		act( "$n �i�D�A, '�ڤ��R���r���F��'", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	sprintf( buf, "$n �i�D�A, '���@�N�X %d �T�����������R�U $p'.", cost );
	act( buf, keeper, obj, ch, TO_VICT );
	ch->reply = keeper;

	return;
}

/* Poison weapon by Thelonius for EnvyMud */
void do_poison_weapon( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *pobj;
	OBJ_DATA *wobj;
	AFFECT_DATA *paf;  // 2021/11/10
	char arg [ MAX_INPUT_LENGTH ];
	int sklv = get_skill_level(ch, gsn_poison_weapon);
	/* Don't allow mobs or unskilled pcs to do this */
	if ( IS_NPC( ch )
		|| ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_poison_weapon ) == 0 ) )
	{
		send_to_char( "����H\n\r", ch );
		return;
    }

	one_argument( argument, arg );

	if ( arg[0] == '\0' )                
	{
		send_to_char( "�A�n�b����F��W�٬r�H\n\r", ch );
		return;
	}
	if ( ch->fighting )               
	{
		send_to_char( "�A���b�԰��A�w���X���.\n\r", ch );
		return;
	}
	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "�A�S���o�˪F��.\n\r", ch );
		return;
	}
	/*
	if ( obj->item_type != ITEM_WEAPON  )
	{
		send_to_char( "�����O�Z��.\n\r", ch );
		return;
	}
	*/
	// �s�W arrow �� dart �٬r 2021/11/10
	if ( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARROW && obj->item_type != ITEM_DART )
	{
		send_to_char( "���󪫫~����٬r.\n\r", ch );
		return;
	}
	if ( IS_OBJ_STAT( obj, ITEM_POISONED ) )
	{
		send_to_char( "���w�g���r�F.\n\r", ch );
		return;
	}
    
	/* Now we have a valid weapon...check to see if we have the powder. */
	for ( pobj = ch->carrying; pobj; pobj = pobj->next_content )
	{
		if ( pobj->pIndexData->vnum == OBJ_VNUM_BLACK_POWDER )
			break;
	}
	if ( !pobj )
	{
		send_to_char( "�A�S���r��.\n\r", ch );
		return;
	}

	/* Okay, we have the powder...do we have water? */
	for ( wobj = ch->carrying; wobj; wobj = wobj->next_content )
	{
		// �קK�� �S����ë~ ���h�N��
		if ( IS_SET(wobj->pIndexData->souvenir, Z4) )
			continue;
		if ( wobj->item_type == ITEM_DRINK_CON 
			&& wobj->value[1] > 0
			&& wobj->value[2]  == 0 )
			break;
	}
	if ( !wobj )
	{
		send_to_char( "�A�o�n�����ӲV�X�r��.\n\r", ch );
		return;
	}

	/* Great, we have the ingredients...but is the thief smart enough? */
	if ( !IS_NPC( ch ) &&  get_curr_wis( ch )+sklv/2 < 11 && get_curr_int( ch )+sklv/2 < 11)
	{
		send_to_char( "�A�¤�¸}���A���G���ѤF...\n\r", ch );
		return;
	}
	/* And does the thief have steady enough hands? */
	if ( !IS_NPC( ch ) && ( get_curr_dex( ch )+sklv/2 < 17
		|| ch->pcdata->condition[COND_DRUNK] > 0 ) )
	{
		send_to_char( "�A�@�Ӥ��p�ߡA��@�n���r�G����½�F�I\n\r", ch );
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_poison_weapon].beats );

	/* Check the skill percentage */
	if ( !IS_NPC( ch )
	//&& number_percent( ) > ch->pcdata->learned[gsn_poison_weapon] )
		&& number_percent() > sklv*2 + get_skill_percent(ch, gsn_poison_weapon))
	{
		send_to_char( "�A�@�Ӥ��p�ߡA��r�G�q��F�ۤv���W�I\n\r", ch );
		//damage( ch, ch, ch->level, gsn_poison_weapon, WEAR_NONE );
		damage( ch, ch, ch->level, gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		act( "$n ��r�G�q�o��B���O�I", ch, NULL, NULL, TO_ROOM );
		extract_obj( pobj );
		extract_obj( wobj );
		return;
	}

    /* Well, I'm tired of waiting.  Are you? */
    act( "�A�� $p �˶i $P, �s���r�G�C", ch, pobj, wobj, TO_CHAR );
    act( "$n �� $p �˶i $P, �s���F�r�G�C",ch, pobj, wobj, TO_ROOM );
    act( "�A��r�G���æa�٦b $p �W�C",ch, obj, NULL, TO_CHAR  );
    act( "$n ��r�G���æa�٦b $p�W�C",ch, obj, NULL, TO_ROOM  );

	/*  �ް� timer �]�w�� item_poison_weapon
	if( obj->timer <= 0 )
		SET_BIT( obj->extra_flags, ITEM_POISONED_WEAPON );
	if( obj->timer < 0 )
	{
		obj->timer = 40 + ch->level + sklv * sklv * 8;

		// obj->cost *= ch->level;

		// Set an object timer.  Dont want proliferation of poisoned weapons
		//obj->timer = 40 + ch->level + sklv * sklv * 8;
	 
		if ( IS_OBJ_STAT( obj, ITEM_BLESS ) )
			obj->timer = obj->timer + ( obj->timer * ( sklv / 2 ) / 10 );

		if ( IS_OBJ_STAT( obj, ITEM_MAGIC ) )
			obj->timer = UMAX( 20, obj->timer - 60 );

		// WHAT?  All of that, just for that one bit?  How lame. ;)
	}
	*/

	SET_BIT( obj->extra_flags, ITEM_POISONED );
	// �[�J�٬r���~���r�ʧP�w 2021/11/11
	paf            = new_affect();
	paf->type      = obj->pIndexData->vnum;
	paf->duration  = 5 + sklv * sklv;
	paf->location  = APPLY_TOXICITY_POISON;
	paf->modifier  = pobj->level * ( 1 + sklv/6 );
	paf->bitvector = 0;
	paf->next      = obj->affected;
	obj->affected  = paf;

	act( "�ѤU���r�G�� $p ���G�k�F.", ch, wobj, NULL, TO_CHAR );
	act( "�ѤU���r�G�� $p ���G�k�F.", ch, wobj, NULL, TO_ROOM );
	extract_obj( pobj );
	extract_obj( wobj );

	return;
}

/* Contributed by BoneCrusher of EnvyMud. */
void do_donate( CHAR_DATA *ch, char *arg )
{
	OBJ_DATA *container;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	char      arg1[MAX_INPUT_LENGTH];

	arg = one_argument( arg, arg1 );

	if ( arg1[0] == '\0' )
	{
		send_to_char( "�A�n���m����H\n\r", ch );
		return;
	}

	if( !ch->in_room ) return;

    /*
		if ( ( container = get_obj_world( ch, "donation" ) ) == NULL )
		{
			send_to_char( "�o�ӥ@�ɤW�S���^�m�c.\n\r", ch );
			return;
		}
	*/

	container = NULL;
	for( container = object_list ; container ; container = container->next )
	{
		if( IS_SET( container->extra_flags, ITEM_DONATION_PIT)) {
			if( !container->in_room || container->in_room->area->recall != ch->in_room->area->recall)
				continue;
			else
				break;
		}
	}
	if( !container ) {
		send_to_char("�o�ӥ@�ɤW�S���^�m�c�C\n\r", ch);
		return;
	}
	if( get_obj_number(container) > 100 ) {
		send_to_char("�^�m�c���F�C\n\r", ch);
		return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
		if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
		{
			send_to_char( "�A�S���o�˪F��.\n\r", ch );
			return;
		}

		if ( !can_drop_obj( ch, obj ) || IS_SET(obj->extra_flags, ITEM_NOSAC))
		{
			send_to_char( "�A�L�k�߶}�o�˪F��.\n\r", ch );
			return;
		}
  
		if ( get_obj_weight( obj ) + get_obj_weight( container ) > container->value[0] )
		{
			send_to_char( "�o�˪F��ˤ��i�^�m�c.\n\r", ch );
			return;
		}

		act( "�A�^�m $p �i $P.", ch, obj, container, TO_CHAR );

		if ( obj->item_type == ITEM_TRASH
			|| obj->item_type == ITEM_FOOD
			|| obj->item_type == ITEM_KEY
			|| obj->item_type == ITEM_PILL )
		{
			// �s�W obj_act donate 2022/02/12
			if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
				oprog_act_trigger("donate", ch, obj);
			}

			// obj �i��b obj_act �ɳQ purge 2022/02/13
			if(!obj){
				save_char_obj( ch );
				return;
			}

			//act( "�A�^�m $p �i $P.", ch, obj, container, TO_CHAR );
			extract_obj( obj );
			return;
		}

		act( "$n �^�m $p �i $P.", ch, obj, container, TO_ROOM );
		send_to_room( "�^�m�c�̶ǨӤ@�}�T�n�I", container->in_room );
		obj_from_char( obj );
		obj_to_obj( obj, container );
		// �s�W obj_act donate 2022/02/12

		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("donate", ch, obj);
		}

		// obj �i��b obj_act �ɳQ purge 2022/02/13
		if(!obj){
			save_char_obj( ch );
			return;
		}

		// �ԭz���� obj �q ch ���W�����e 2022/02/12
		//act( "$n �^�m $p �i $P.", ch, obj, container, TO_ROOM );
		//act( "�A�^�m $p �i $P.", ch, obj, container, TO_CHAR );
		//send_to_room( "�^�m�c�̶ǨӤ@�}�T�n�I", container->in_room );
	}
	else
	{
		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
				&& can_see_obj( ch, obj )
				&& obj->wear_loc == WEAR_NONE
				&& obj != container
				&& can_drop_obj( ch, obj )
				&& get_obj_weight( obj ) + get_obj_weight( container )
				<= container->value[0] )
			{
				act( "�A�^�m $p �i $P.", ch, obj, container, TO_CHAR );

				if ( obj->item_type == ITEM_TRASH
					|| obj->item_type == ITEM_FOOD
					|| obj->item_type == ITEM_KEY
					|| obj->item_type == ITEM_PILL )
				{
					// �s�W obj_act donate 2022/02/12
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("donate", ch, obj);
					}

					// obj �i��b obj_act �ɳQ purge 2022/02/13
					if(!obj) continue;

					//act( "�A�^�m $p �i $P.", ch, obj, container, TO_CHAR );
					extract_obj( obj );
					continue;
				}

				act( "$n �^�m $p �i $P.", ch, obj, container, TO_ROOM );
				send_to_room( "�^�m�c�̶ǨӤ@�}�T�n�I", container->in_room );
				obj_from_char( obj);
				obj_to_obj( obj, container );

				// �s�W obj_act donate 2022/02/12
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("donate", ch, obj);
				}

				// obj �i��b obj_act �ɳQ purge 2022/02/13
				if(!obj) continue;

				// �ԭz���� obj �q ch ���W�����e 2022/02/12
				//act( "$n �^�m $p �i $P.", ch, obj, container, TO_ROOM );
				//act( "�A�^�m $p �i $P.", ch, obj, container, TO_CHAR );
				//send_to_room( "�^�m�c�̶ǨӤ@�}�n�T�I\n\r", container->in_room );
			}
		}
	}

	save_char_obj( ch );
	return;

}


void do_register( CHAR_DATA *ch, char *arg )
{
	CHAR_DATA *mob;
	int cost = ch->level * 5000;
	char buf[MAX_STRING_LENGTH];

	if ( IS_NPC( ch ) )
		return;

	mob = NULL;
	for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
	{
		if ( IS_NPC( mob ) && mob->pIndexData->vnum == MOB_VNUM_SECRETARY )
			break;
	}

	if ( !mob )
	{
		send_to_char( "�o�̤���i�� PK �n�O.\n\r", ch );
		return;
	}

	if ( ch->level < 25 )
	{
		do_say( mob, "�A�������٤ӧC�F." );
		return;
	}

	/*if ( ch->level > LEVEL_HERO + 6 )
	{
		do_say( mob, "�����i�H�ѥ[ PK." );
		return;
	} */

	if( IS_SET( ch->act, PLR_REGISTER ))
	{
		if( ch->gold < cost * 5 )
		{
			sprintf( buf, "�A�S�����������Ӱh�X PK." );
			do_say( mob, buf );
			return;
		}
		sprintf( buf, "�A�I�X�F %d �T�����A�h�X PK.", cost * 5 );
		//do_say( mob, buf );
		send_to_char( buf, ch);
		ch->gold -= 5 * cost;
		REMOVE_BIT( ch->act, PLR_REGISTER);
		return;
	}

	if ( ch->gold < cost )
	{
		sprintf( buf, "�A�S�����������ӥI�n�O�O." );
		do_say( mob, buf );
		return;
	}

	sprintf( buf, "�A�I�X�F %d �T�����A�n�O�ѥ[ PK.", cost );
	//do_say( mob, buf );
	send_to_char( buf, ch);

	ch->gold -= cost;

	SET_BIT( ch->act, PLR_REGISTER );

	return;

}

void do_deposit( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	char arg [ MAX_INPUT_LENGTH ];
	int num;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		if ( IS_NPC(vch) && IS_SET( vch->act, ACT_BANKER ) ) // �[�J npc ���w , ���K plr_thief �]�|�Q�� banker 2022/11/27
			break;

	one_argument( argument, arg );
	if ( is_number( arg ) )
		num = atoi( arg );
	else
	{
		send_to_char( "�Щ��T���X�@�ӼƦr�C\n\r", ch );
		return;
	}

	if( num < 1 ) {
		send_to_char("�Цܤ֦s�@���C\n\r", ch);
		return;
	}

	if ( vch )
	{
		if ( num <= ch->gold )
		{
			if(num > 2147483647 - ch->bank){ // MAXIMUM Integer 2147483647
				send_to_char("�A�����Y�s���F�o��h���C\n\r", ch);
				return;
			}
			ch->gold -= num;
			ch->bank += num;
			act( "$N�V�A�@���`���G���±z�����{�A�j���s��Ȧ�û��ܸ۬��z�A�ȡC", ch, NULL, vch, TO_CHAR );
			act( "$N�V$n�@���`���G���±z�����{�A�j���s��Ȧ�û��ܸ۬��z�A�ȡC", ch, NULL, vch, TO_ROOM );
			return;
		}
		else
		{
			send_to_char( "��I�A���W�S����h���C\n\r", ch );
			return; 
		}
	}
	else
		send_to_char( "�A�䤣��H���A�s���C\n\r", ch );
	return;
}

void do_withdraw( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	char arg [ MAX_INPUT_LENGTH ];
	int num;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		if ( IS_NPC(vch) && IS_SET( vch->act, ACT_BANKER ) ) // �[�J npc ���w , ���K plr_thief �]�|�Q�� banker 2022/11/27
			break;

	one_argument( argument, arg );
	if ( is_number( arg ) )
		num = atoi( arg );
	else
	{
		send_to_char( "�Щ��T���X�@�ӼƦr�C\n\r", ch );
		return;
	}
	if( num < 1 ) {
		send_to_char("�Цܤֻ�@���C\n\r", ch);
		return;
	}

	if ( vch )
	{
		if ( num <= ch->bank )
		{
			if(num > 2147483647 - ch->gold){ // MAXIMUM Integer 2147483647
				send_to_char("�A�����W�a���F�o��h���C\n\r", ch);
				return;
			}
			ch->gold += num;
			ch->bank -= num ;
			act( "$N�V�A�@���`���G�o�O�z�������A���I���C���±z�����{�C", ch, NULL, vch, TO_CHAR );
			act( "$N�V$n�@���`���G�o�O�z�������A���I���C���±z�����{�C", ch, NULL, vch, TO_ROOM );
			return;
		}
		else
		{
			send_to_char( "�m�T�ڡH�A���Ө���h���H\n\r", ch );
			return; 
		}
	}
	else
		send_to_char( "�A�䤣��H���A�����C\n\r", ch );
	return;
}

void do_store( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *vobj;
	CHAR_DATA *vch;
	char buf [10];
	int cost;
	bool fBank = TRUE;
	int number = 0;

	if ( IS_NPC( ch ) )
		return;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room ){
		// �[�J npc ���w, �íק� eqbanker ���P�w�� name �ܧ� qmark 2022/12/01
		if( !IS_NPC(vch) ) continue;
		//if ( is_name( "sharas-eqb", vch->name ) )
		if( is_qmark(vch, "is_eqbanker") )
			break;
	}
	if ( !vch )
	{
		send_to_char( "�˳ƻȦ���󤣦b�o��C\n\r", ch );
		return;
	}

	if( ch->in_room->area->recall != ROOM_VNUM_TEMPLE){
		send_to_char("�u���v�d�Ԩ����i�H�ϥ��¸˳ƻȦ�C\n\r", ch);
		return;
	}

	if ( argument[0] == '\0' )
	{
		send_to_char( "�˳ƻȦ����K�ܻ۬��G�u�z�����n�s����A�ڤ]�����W���C�v\n\r", ch );
		return;
	}

	for( obj = ch->eqbank; obj ; obj = obj->next_content ){
		++number;
	}

	if( number >= 50 ) {
		send_to_char( "�˳ƻȦ����K�ܻ۬��G�u��p�A�z�����Y�w�g���F�A�}�ӷs���ܮw�a�C�v\n\r", ch );
		return;
	}

	if ( ( obj = get_obj_carry( ch, argument ) ) )
	{
		cost = ( obj->level < 10 ) ? 10000 :
		( obj->level < 36 ) ? obj->level * 200 : obj->level * 500;
		if ( ch->bank < cost )
		{
			fBank = FALSE;
			if( ch->gold < cost ) {
				send_to_char( "�˳ƻȦ����W���ۻ��G�u�藍�_�A�S���������O�޶O�ڵL�k���ΫO���H���C�v\n\r", ch );
				return;
			}
		}
		if ( ( ch->level < ( obj->level - 5 ) && ch->class < 5 )
			|| obj->item_type == ITEM_KEY
			|| IS_SET( obj->extra_flags, ITEM_NOSAVE )
			|| IS_SET( obj->extra_flags, ITEM_UNIQUE)
			|| obj->deleted 
			|| obj->item_type == ITEM_CONTAINER
			|| obj->timer > 0 )
		{
			send_to_char( "�˳ƻȦ�����q�q�ӻ��G�u�藍�_�A�o���˳Ƨڭ̤����C�v\n\r", ch );
			return;
		}
		obj_from_char( obj );
		if ( obj == object_list )
		{
			object_list = obj->next;
			obj->next = NULL; 
		}
		else for ( vobj = object_list; vobj; vobj = vobj->next )
		{
			if ( obj == vobj->next )
			{
				vobj->next = obj->next;
				obj->next = NULL;
				break;
			}
		}
		obj->next_content = ch->eqbank;
		ch->eqbank = obj;
		if( fBank )
			ch->bank -= cost;
		else
			ch->gold -= cost;
		// ���� to_room �קK�j�q�s�����~�y���L�H�e����� 2022/02/01
		//act( "�˳ƻȦ����p�ߦa�N$n��$p����᭱���O�I�w�s��C", ch, obj, NULL, TO_ROOM );
		act( "�˳ƻȦ����p�ߦa�N�A��$p����᭱���O�I�w�s��C", ch, obj, NULL, TO_CHAR );
		if( fBank )
			send_to_char("�A�q�Ȧ���b�A�I�X�F "HIY, ch);
		else
			send_to_char("�A�I�X�F "HIY, ch);
		sprintf(buf, "%d", cost);
		send_to_char(buf, ch);
		send_to_char(NOR " �����O�޶O�C\n\r", ch);
		do_save( ch, "" );
		return;
	}
	send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�ڨS�ݨ�o���˳ưڡH�v\n\r", ch );
}

void do_receive( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	OBJ_DATA  *obj = NULL;
	OBJ_DATA  *vobj;
	// int number;
	int i = 0;
	int count = 0;
	// char  arg [ MAX_INPUT_LENGTH ];
	char  buf [ MAX_STRING_LENGTH ];
	char  no_ansi [ MAX_STRING_LENGTH ];

	if ( IS_NPC( ch ) )
		return;

	// number = number_argument( argument, arg );
	// count = atoi(argument);
	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
	{
		// �[�J npc ���w, �íק� eqbanker ���P�w�� name �ܧ� qmark 2022/12/01
		if( !IS_NPC(vch) ) continue;
		//if ( is_name( "sharas-eqb", vch->name ) )
		if( is_qmark(vch, "is_eqbanker") )
			break;
	}
	if ( !vch )
	{
		send_to_char( "�˳ƻȦ���󤣦b�o��C\n\r", ch );
		return;
	}


	if( ch->in_room->area->recall != ROOM_VNUM_TEMPLE){
		send_to_char("�u���v�d�Ԩ����i�H�ϥ��¸˳ƻȦ�C\n\r", ch);
		return;
	}

	if ( argument[0] == '\0' )
	{
		send_to_char( "�A�ثe�s�b�˳ƻȦ�O�I�w���˳Ʀ��G\n\r", ch );
		if ( !ch->eqbank )
		{
			send_to_char( "\n\r���򳣨S���I\n\r\n\r", ch );
			return; 
		}

		send_to_char("�i�s���j  ��  �~  �W  ��                                  ����\n\r", ch);
		for ( vobj = ch->eqbank; vobj; vobj = vobj->next_content )
		{
			i++;
			strip_ansi(vobj->short_descr, no_ansi);
			sprintf ( buf, "�i%4d�j%-48s  [%2d] \n\r", 
			i,  
			can_see_obj(ch, vobj) ? no_ansi :
			"�������~",
			vobj->level
			);
			send_to_char( buf, ch );
			//act( "$p", ch, vobj, NULL, TO_CHAR );
		}
		send_to_char( "\n\r", ch );
		return;
	}
	if ( !is_number( argument ) )
	{
		send_to_char( "�Х� 'receive �s��' �ӻ���z���˳ơC\n\r", ch );
		return;
	}
	count = atoi( argument );

	if( !ch->eqbank ) {
		send_to_char( "�˳ƻȦ���󪰺æa�ݵۧA���G�u�z���ܮw�̨S���F��ڡH�v\n\r", ch );
		return;
	}

	i = 2;
	if( ch->eqbank && count == 1 )
	{
		obj = ch->eqbank;
		if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) )
		{
			do_say(vch, "�藍�_�A�z�����ʧ�h�F��F�C");
			return;
		}

		if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) )
		{
			do_say(vch,"�藍�_�A�z���t�������F�C");
			return;
		}
		ch->eqbank = obj->next_content;
		obj->next_content = NULL;
	}
	else if ( ch->eqbank && ch->eqbank->next_content )
	{
		for( vobj = ch->eqbank ; vobj->next_content; vobj = vobj->next_content )
		{

			// sprintf(buf, "[%d][%d]\n\r", count, i );
			// send_to_char(buf, ch);
			if( count == i )
			{
				obj = vobj->next_content;
				if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) )
				{
					do_say(vch, "�藍�_�A�z�����ʧ�h�F��F�C");
					return;
				}

				if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) )
				{
					do_say(vch,"�藍�_�A�z���t�������F�C");
					return;
				}
				vobj->next_content = obj->next_content;
				obj->next_content = NULL;

				break;
			}
			i++;
		}
	}
	/*
	if ( ch->eqbank && is_pfxname( arg, ch->eqbank->name ) )
	{
	obj = ch->eqbank;
	ch->eqbank = obj->next_content;
	}
	else if ( ch->eqbank && ch->eqbank->next_content )
	for ( vobj = ch->eqbank; vobj->next_content; vobj = vobj->next_content )
	if ( is_pfxname( arg, vobj->next_content->name ) )
	{
	if ( ++count == number )
	{
	obj = vobj->next_content;
	vobj->next_content = obj->next_content;
	break;
	}
	}     
	*/
	if ( obj )
	{
		obj_to_char ( obj, ch );
		obj->next = object_list;
		object_list = obj;
		// ���� to_room �קK�j�q�s�����~�y���L�H�e����� 2022/02/01
		act( "�˳ƻȦ����۫᭱���w�бN$p���ӥ浹�A�C", ch, obj, NULL, TO_CHAR );
		//act( "�˳ƻȦ����۫᭱���w�бN$p���ӥ浹$n�C", ch, obj, NULL, TO_ROOM );
		//act( "�˳ƻȦ����@���`���G�u�бz�ˬd���L����l�ˡA���¥��{�C�v", ch, obj, NULL, TO_ROOM );
		act( "�˳ƻȦ����@���`���G�u�бz�ˬd���L����l�ˡA���¥��{�C�v", ch, obj, NULL, TO_CHAR );
		do_save( ch, "" );
		return;
	}
	send_to_char( "�˳ƻȦ����d�\\�F�d�O�W���n�Oï���G�_�ǡA�S���o���˳ƪ�������I\n\r", ch );
}


void do_destroy( CHAR_DATA *ch, char* argument )
{
	char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;


	one_argument(argument, arg);
    
	if ( arg[0] == '\0' )
	{
		send_to_char( "�п�J destroy <���~>\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "�A�S�����˪F��.\n\r", ch );
		return;
	}

	if( !IS_SET(obj->extra_flags, ITEM_PERSONAL) ) {
		send_to_char("�A�L�k�P���o�˪F��.\n\r", ch);
		return;
	}
	act( "�A���h $p.", ch, obj, NULL, TO_CHAR );
	act( "$n ���h $p.", ch, obj, NULL, TO_ROOM );
	// �s�W obj_act destroy 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("destroy", ch, obj);
	}
	obj_from_char(obj);

	// obj �i��b prog ���Q purge 2022/02/13
	//if(obj)
	if(obj->deleted)
		;
	else
		extract_obj(obj);
}

/*
void do_consent( CHAR_DATA *ch, char* argument)
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char qm[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *old_in = NULL;

    one_argument( argument, arg);

    if( IS_NPC(ch) ) return;
    if( arg[0] == '\0' ) {
        send_to_char("�A�n���v���֡H\n\r", ch);
        return;
    }
    if( !(victim = get_char_world( ch, arg) )
    ||  IS_NPC(victim) ){
        send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
        return;
    }

    if( victim == ch )
  {
    send_to_char("�A���ӴN�i�H�ۤv�ߦۤv������F�I\n\r",ch);
    return;
  }

    one_argument(ch->name, arg1);
    sprintf(qm, "consent_%s", arg1);
    if( is_qmark(victim, qm) ) {
        if( ch->in_room != victim->in_room ) {
            old_in = ch->in_room;
            char_from_room(ch);
            char_to_room(ch, victim->in_room);
        }
        sprintf(buf,"'%s' %s ", arg, qm);
        do_del_qmark(ch, buf);
        if( old_in ) {
            char_from_room(ch);
            char_to_room(ch, old_in);
        }
        act("�A���^��$N�����v�C", ch, NULL, victim, TO_CHAR);
        act("$n���A���v�A�ߨ�$s����C", ch, NULL, victim, TO_VICT);
        return;
    }

    sprintf(buf,"'%s' %s -1 0 0 0 0 0", arg, qm);
    if( ch->in_room != victim->in_room ) {
        old_in = ch->in_room;
        char_from_room(ch);
        char_to_room(ch, victim->in_room);
    }
    do_add_qmark(ch, buf);
    if( old_in ) {
        char_from_room(ch);
        char_to_room(ch, old_in);
    }
    act("�A���v$N�ߨ��A������C", ch, NULL, victim, TO_CHAR);
    act("$n���v�A�ߨ�$s����C", ch, NULL, victim, TO_VICT);
}
*/
void do_bandage( CHAR_DATA *ch, char* argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *pobj;
    CHAR_DATA *vch;
    int factor = 0;
    int sklv = get_skill_level(ch, gsn_bandage);


    if ( IS_NPC( ch ) || (  !IS_NPC( ch )
            && get_skill_percent( ch, gsn_bandage) == 0 ))
    {                
        send_to_char( "����H\n\r", ch );
        return;
    }

    if ( ch->fighting ){
        send_to_char( "�A���b�԰��A�w���X���.\n\r", ch ); 
        return; 
    }
    one_argument(argument, arg);

    if ( arg[0] == '\0') 
        vch = ch;
    else if( !(vch = get_char_room(ch, arg))) {
        send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
        return;
    }
    if( vch->fighting ) {
        send_to_char("��西�b�԰��A�A�S��k�J�Ӫ��]�ϡC\n\r", ch);
        return;
    }

    for ( pobj = ch->carrying; pobj; pobj = pobj->next_content )
    {
        if ( pobj->pIndexData->vnum == OBJ_VNUM_BANDAGE)
            break;
    }
    if ( !pobj )
    {
        send_to_char( "�A���W�S���^�a.\n\r", ch );
        return;
    }

    extract_obj(pobj);

    if( ch != vch ) {
        act( "�A�Ӥߪ���$N�]�϶˶�.", ch, NULL, vch, TO_CHAR );
        act( "$n�Ӥߪ���$N�]�϶˶�.", ch, NULL, vch, TO_NOTVICT );
        act( "$n�Ӥߪ����A�]�϶˶�.", ch, NULL, vch, TO_VICT);
        WAIT_STATE(vch, skill_table[gsn_bandage].beats);
        WAIT_STATE(ch, skill_table[gsn_bandage].beats);
    }

    factor = UMAX( ch->level /6 + ch->level / 5, 5);
    factor += sklv;
    if( ch->pcdata->prime_attr == APPLY_WIS) {
        int extra;
        extra = number_bits(2);
        factor += sklv*2;
        switch(extra) {
        case 0:
            factor += 5+sklv;
            break;
        case 1:
            factor += sklv*2;
            break;
        case 2:
            if( is_affected(vch, gsn_poison)){
                act( "�@�}�M�s���Pı�R���A����.", ch, NULL, vch, TO_VICT);
            }
            break;
        case 3:
            vch->move = UMIN(vch->move + sklv*4+(sklv/3)*10, vch->max_move);
            break;
        };
    }
    int nHeal = (vch->max_hit*factor)/(114-sklv*4);
    nHeal *= PPL_HP_MULTIPLYER;
    vch->hit = UMIN( vch->hit + nHeal, vch->max_hit );
    ch->move = UMAX(0, ch->move - (10 + factor/10));
    send_to_char("�n���C\n\r", ch);

    WAIT_STATE(ch, skill_table[gsn_bandage].beats);
}
