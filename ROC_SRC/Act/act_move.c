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
//*  PROGRAM ID  : act_move.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940625 jye                                    *
//*  PURPOSE     :         �bdo_disperse�W�[�i����Powerup��Hasten *
//*              :         �ק�do_stand�԰����Qtripe��stand��     *
//*              :             �i�H�L���Ukill���O��bug            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940907 Razgriz                                *
//*  PURPOSE     :         �ק� instant �u�ର wiz �Ҩϥ�         *
//*              :         �������e sav �]��ϥΪ� bug            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940924 jye                                    *
//*  PURPOSE     :         �ק�move_char ��evacuate QMARK         *
//*              :         �@�q�ɶ��L�k����                       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950523 jye                                    *
//*  PURPOSE     :         �ק�move_char �W�[�P�_master��follower *
//*              :         �b���Proom��follower�~move             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         �s�WEXP_PER_LEVEL ppl_class�ܼ�        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1060107 jye                                    *
//*  PURPOSE     :         �ק� do_hunt ()                        *
//*              :         ����STUN�ĪG                           *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"
#include "olc.h"

char *        const    dir_name    [ ]          =
{
    "�_", "�F", "�n", "��", "�W", "�U", "�F�_", "��_", "�F�n", "��n"
};

const      int  rev_dir          [ ]      =
{
    2, 3, 0, 1, 5, 4
};

const      int  movement_loss      [ SECT_MAX ]  =
{
    1, 2, 2, 3, 4, 6, 4, 1, 5, 10, 6
};

extern int IsSorGateMob(int nMobIndex);

/*
 * Local functions.
 */
int find_door args( ( CHAR_DATA *ch, char *arg ) );
bool has_key args( ( CHAR_DATA *ch, int key ) );

/*
 *  Local game functions.
 */
DECLARE_GAME_FUN( game_u_l_t );


void aggr_check( CHAR_DATA *ch, ROOM_INDEX_DATA * to_room)
{
	CHAR_DATA *inp;

	if( ch->deleted )
		return;

	// safe room �����|���� 2022/07/11
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
		return;

	if( to_room->people ) {
		for( inp = to_room->people; inp; inp = inp->next_in_room ){
			// safe room �����|���� 2022/07/11
			if( IS_SET(inp->in_room->room_flags, ROOM_SAFE) )
				return;
			if( inp == ch
				|| !IS_AWAKE(inp)
				|| !can_see(inp, ch)
				|| inp->deleted
				//|| IS_IMMORTAL(ch))  �קאּ�P�w wizinv 2022/07/15
				|| ( !IS_NPC(ch) && IS_SET( ch->act, PLR_WIZINVIS ) ) )
				continue;
			if( is_same_group(ch, inp)&& !IS_AFFECTED(inp, AFF_CRAZY)) continue;

			if( (IS_AFFECTED(inp, AFF_CHARM) || IS_AFFECTED(inp, AFF_SUMMONED)) && !IS_AFFECTED(inp, AFF_CRAZY)) continue;

			if( IS_NPC(ch) && IS_NPC(inp) && !IS_AFFECTED(inp, AFF_CRAZY)) continue;

			if( IS_AFFECTED(inp, AFF_CRAZY) || (IS_NPC(inp) && IS_SET(inp->act, ACT_AGGRESSIVE)))
			{
				if( IS_NPC(inp) && IS_NPC(ch) && number_bits(4) == 1 && !inp->fighting ){
					multi_hit(inp, ch, TYPE_UNDEFINED);
				}else if ( !inp->fighting && number_bits(3) == 1 ){
					multi_hit(inp, ch, TYPE_UNDEFINED);
				}
			}
		}
	}

	if( !ch->fighting && ( IS_AFFECTED(ch, AFF_CRAZY) || (IS_NPC(ch) && IS_SET(ch->act, ACT_AGGRESSIVE)))){
		for( inp = to_room->people ; inp; inp = inp->next_in_room ){
			// safe room �����|���� 2022/07/11
			if( IS_SET(inp->in_room->room_flags, ROOM_SAFE) )
				return;
			if( inp == ch
				|| !can_see(ch, inp)
				|| inp->deleted
				//|| IS_IMMORTAL(inp))  �קאּ�P�w wizinv 2022/07/15
				|| ( !IS_NPC(inp) && IS_SET( inp->act, PLR_WIZINVIS ) ) )
				continue;
			if( is_same_group(ch, inp) && !IS_AFFECTED(inp, AFF_CRAZY)) continue;
			if( (IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED)) && !IS_AFFECTED(ch, AFF_CRAZY)) continue;
			if( IS_NPC(ch) && IS_NPC(inp) && !IS_AFFECTED(ch, AFF_CRAZY)) continue;
			if( IS_NPC(ch) && IS_NPC(inp) && number_bits(4) == 1 ){
				multi_hit(ch, inp, TYPE_UNDEFINED);
				break;
			}else if( number_bits(3) == 1 ){
				multi_hit(ch, inp, TYPE_UNDEFINED);
				break;
			}
		}
	}
}

void move_char( CHAR_DATA *ch, int door )
{
	CHAR_DATA *fch, *rch;
	CHAR_DATA *fch_next;
	OBJ_DATA *horse;
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA *in_room;
	ROOM_INDEX_DATA *to_room;
	int moved = 131072; /* Matches ACT & PLR bits */
	char buf [ MAX_STRING_LENGTH ];
	QMARK *qmark = NULL;

	if ( door < 0 || door > 5 )
	{
		bug( "Do_move: bad door %d.", door );
		return;
	}

	/*
	* Prevents infinite move loop in
	* maze zone when group has 2 leaders - Kahn
	*/
	if( IS_SET( ch->act, moved ) )
		return;

	if( (qmark = is_qmark( ch, "cast_evacuate_mark" ))!= NULL ) 
	{
		send_to_char( "�A�ثe�B���檬�A�L�k���ʡI\n\r", ch );
		return;
	}

	// affect stone form �ץ� 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�b�ۤƪ��A�U���ಾ��.\n\r", ch);
		return;
	}

	if ( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare �ԭz�ק� 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m�A�Q�x�b�����̤F�I�A���ಾ�ʡI[m\n\r", ch );
		}else{
			send_to_char( "[33m�A������ʼu���o�I�A���ಾ�ʡI[m\n\r", ch );
		}
		return;
	}

	if ( IS_AFFECTED( ch, AFF_PARALYZED ) ) 
	{
		send_to_char( "�A�]�·��Ӱʼu���o�I\n\r", ch );
		return;
	}

	if( auction_list )
	{
		AUC_DATA *auction;
		for( auction = auction_list ; auction; auction = auction->next )
		{
			if ( ch == auction->owner || ch == auction->buyer )
			{
				send_to_char("�A���b�i����A�������}�o�̡C\n\r", ch);
				return;
			}
		}
	}



	if( !(in_room = ch->in_room)) 
	{
		return;
	}
	if ( !( pexit = in_room->exit[door] ) || !( to_room = pexit->to_room ) )
	{
		send_to_char( "[33m�u�r�A�������q�C[m\n\r", ch );
		return;
	}

	if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
	{
		if ( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) )
		{
			act( "[33m$d�O���۪��C[m",ch, NULL, pexit->keyword, TO_CHAR );
			return;
		}
		if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
		{
			act( "[33m�A�藍�L$d�C�n�h�I[m",ch, NULL, pexit->keyword, TO_CHAR );
			return;
		}
	}
	// �Ъ��� size , �j�� 0 �~�|�@�� 2022/12/22
	//if ( pexit->exit_size > 0 && ( race_table[ch->race].size > pexit->exit_size ) )
	if ( pexit->exit_size > 0 && ( get_curr_size( ch ) > pexit->exit_size ) ) // �����髬 size ���� 2022/12/23
	{
		send_to_char( "[33m�A�髬�Ӥj�A�i�J���F�q�D�C[m\n\r", ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master && in_room == ch->master->in_room )
	{
		send_to_char( "[33m����H�n��A�˷R���D�H�ӥh�ܡH[m\n\r", ch );
		return;
	}

	// �ק� room_is_private �N�J�� 2022/12/20
	//if ( room_is_private( to_room ) && !IS_IMMORTAL( ch ))
	if ( room_is_private( ch, to_room ) && !IS_IMMORTAL( ch ) )
	{
		//send_to_char( "[33m���̲{�b�w�g�Q�����F�C[m\n\r", ch );
		send_to_char( "[33m���̪��Ŷ��j�p�A�e�Ǥ��U�A����n�C[m\n\r", ch );
		return;
	}

	if( (IS_NPC(ch) && (!IS_AFFECTED(ch, AFF_CHARM) && !IS_AFFECTED(ch, AFF_SUMMONED))) && !IS_SET(ch->act, ACT_IGNORE_NOMOB && !IS_SET(ch->act,ACT_PET))&& IS_SET(to_room->room_flags, ROOM_NO_MOB))
	{
		return;
	}

	//add by coconet 0602 for horse
	horse = get_eq_char( ch, WEAR_RIDE );

	// �� npc �]�|���a�β��ʭ��� 2023/01/14
	//if ( !IS_NPC( ch ) )
	if( !IS_IMMORTAL(ch) )
	{
		//  int iClass;
		// remove at 2023/01/14
		/*if( to_room->area && IS_SET(to_room->area->area_flags, AREA_CLAN) && !IS_IMMORTAL(ch) && (ch->clan && (!IS_PPLBUILDER(ch, to_room->area) || room_clanlv(to_room) > ch->pcdata->clanlevel ))) 
		{
			send_to_char("�A�b���̪��v�������C\n\r", ch);
			return;
		}*/

		/*
			for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
			{
				if ( iClass != ch->class && to_room->vnum == class_table[iClass].guild )
				{
					send_to_char( "[33m�A���Q���\\�쨺�̥h�C[m\n\r", ch );
					return;
				}
			}
		*/

		int move;

		if ( in_room->sector_type == SECT_AIR || to_room->sector_type == SECT_AIR )
		{
			if( !is_flying(ch))
			{
				send_to_char( "[33m�A�����_�ӡC[m\n\r", ch );
				return;
			}
		}

		if (   to_room->sector_type != SECT_WATER_NOSWIM
			&& to_room->sector_type != SECT_UNDERWATER
			&& strcmp( race_table[ ch->race ].name, "God" )
			&& strcmp( race_table[ ch->race ].name, "Bear" )
			&& strcmp( race_table[ ch->race ].name, "Lizardman" )
			&& strcmp( race_table[ ch->race ].name, "Merfolk" )
			&& strcmp( race_table[ ch->race ].name, "Kuo-toa" )
			&& IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ))
		{
			send_to_char( "[33m�A�b��a¶��¶�h�o�夣�L�h�I[m\n\r", ch );
			return;
		}

		if (   in_room->sector_type == SECT_WATER_NOSWIM
			|| to_room->sector_type == SECT_WATER_NOSWIM )
		{
			OBJ_DATA *obj;
			bool found;
			/*
			* Look for a boat.
			*/
			found = FALSE;

			/*
			* Suggestion for flying above water by Sludge
			*/

			if(    is_flying( ch )
				|| IS_SET( race_table[ ch->race ].race_abilities,RACE_WATERWALK )
				|| IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ) )
				found = TRUE;

			// �ץ� horse obj �ƭȨϥο��~ 2022/06/16
			//if( horse && horse->value[2] & 16 )
			if( horse && horse->value[1] & 1024 )
				found = TRUE;

			for ( obj = ch->carrying; obj; obj = obj->next_content )
			{
				if ( obj->item_type == ITEM_BOAT )
				{
					found = TRUE;
					break;
				}
			}

			if ( !found )
			{
				send_to_char( "[33m�A�o�n���@����~�L�o�h�C[m\n\r", ch );
				return;
			}
		}

		if( (  in_room->sector_type == SECT_UNDERWATER
			|| to_room->sector_type == SECT_UNDERWATER )
			&& ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM )
			&& !IS_AFFECTED( ch, AFF_GILLS ) ) )
		{
			// �ץ� horse obj �ƭȨϥο��~ 2022/06/16
			//if( horse && ( horse->value[2] & 16 ) )
			if( horse && ( horse->value[1] & 1024 ) )
				;
			else
			{
				send_to_char( "[33m�A�o�n�|��a�~��L�h�C[m\n\r", ch );
				return;
			}
		}
		// �ק� wizinv ������ pc �~�P�w 2022/10/11
		if ( in_room->roomprogs && ( in_room->progtypes & ROOM_LEAVE ) && ( IS_NPC(ch) || ( !IS_NPC(ch) && !IS_SET( ch->act, PLR_WIZINVIS ) ) ) )
			rprog_leave_trigger( in_room, ch, door );

		// pc ���ʤ~�|�� mv 2023/01/14
		if( !IS_NPC(ch) )
		{
			move = movement_loss[UMIN( SECT_MAX-1, in_room->sector_type )] + movement_loss[UMIN( SECT_MAX-1, to_room->sector_type )];
			/* Flying persons lose constant minimum movement. */
			if( is_flying(ch) )
				move = move/2;

			if( horse )
				move = 0;

			if ( ch->move < move )
			{
				send_to_char( "[33m�A�Ӳ֤F�C[m\n\r", ch );
				return;
			}

			//WAIT_STATE( ch, 1 );
			ch->move -= move;
		}
	}

	// �ק� wizinv ������ pc �~�P�w 2022/10/11
	//if ( !IS_AFFECTED( ch, AFF_SNEAK ) && ( IS_NPC( ch ) || !IS_SET( ch->act, PLR_WIZINVIS ) ) )
	if ( !IS_AFFECTED( ch, AFF_SNEAK ) && ( IS_NPC( ch ) || ( !IS_NPC(ch) && !IS_SET( ch->act, PLR_WIZINVIS ) ) ) )
	{
		if ( ( ( in_room->sector_type == SECT_WATER_SWIM ) || ( in_room->sector_type == SECT_UNDERWATER ) )
			&& ( ( to_room->sector_type == SECT_WATER_SWIM )
			|| ( to_room->sector_type == SECT_UNDERWATER ) ) )
			for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
			{
				if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
					continue;
				sprintf( buf, ""GRN"%s"GRN"��%s��h�C"NOR"\n\r", PERS( ch, rch), dir_name[door] );
				send_to_char( buf, rch );
			}
			else if ( IS_SET( ch->act, PLR_HORSE ) && ch->desc )
			{
				if( is_flying(ch) )
					for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
					{
						if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
							continue;
						sprintf( buf, ""GRN"%s"GRN"�M��%s"GRN"��%s���L�h�C"NOR"\n\r", PERS( ch->pcdata->horse, rch), can_see(rch, ch) ? ch->short_descr : "�����H��", dir_name[door] );
						send_to_char( buf, rch );
					}
				else
					for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
					{
						if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
							continue;
						sprintf( buf, ""GRN"%s"GRN"�M��%s"GRN"�𵪽𵪦a��%s���h�C"NOR"\n\r", PERS( ch->pcdata->horse, rch), can_see(rch, ch) ? ch->short_descr : "�����H��", dir_name[door] );
						send_to_char( buf, rch );
					}
			}
			else
				for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
				{
					if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
						continue;

					if ( (horse = get_eq_char( ch, WEAR_RIDE )) )
						sprintf( buf, ""GRN"%s"GRN"�M��%s"GRN"�𵪽𵪦a��%s���h�C"NOR"\n\r", ch->short_descr , horse->description , dir_name[door] );
					else
						sprintf( buf, ""GRN"%s"GRN"��%s���h�C"NOR"\n\r", PERS( ch, rch), dir_name[door] );

					send_to_char( buf, rch );
				}
	}

	char_from_room( ch );
	char_to_room( ch, to_room );
	// �ק� wizinv ������ pc �~�P�w 2022/10/11
	//if ( !IS_AFFECTED( ch, AFF_SNEAK ) && ( IS_NPC( ch ) || !IS_SET( ch->act, PLR_WIZINVIS ) ) )
	if ( !IS_AFFECTED( ch, AFF_SNEAK ) && ( IS_NPC( ch ) || ( !IS_NPC(ch) && !IS_SET( ch->act, PLR_WIZINVIS ) ) ) )
	{
		if ( IS_SET( ch->act, PLR_HORSE ) && ch->desc )
		{
			if( is_flying(ch) )
				for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
				{
					if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
						continue;
					sprintf( buf, ""GRN"%s"GRN"�M��%s���L�ӡC"NOR"\n\r", PERS( ch->pcdata->horse, rch), ( can_see(rch, ch) ? ch->short_descr : "�����H��") );
					send_to_char( buf, rch );
				}
			else
				for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
				{
					if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
						continue;
					sprintf( buf, ""GRN"%s"GRN"�M��%s�𵪽𵪦a���L�ӡC"NOR"\n\r", PERS( ch->pcdata->horse, rch), can_see(rch, ch) ? ch->short_descr : "�����H��" );
					send_to_char( buf, rch );
				}
		}
		else
			for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
			{
				if ( rch == ch || !can_see( rch, ch ) || !IS_AWAKE( rch ) || ( IS_SET( rch->act, PLR_BRIEF ) && is_same_group( ch, rch ) && rch->master != ch ) )
					continue;

				if ( (horse = get_eq_char( ch, WEAR_RIDE )) )
					sprintf( buf, ""GRN"%s"GRN"�M��%s"GRN"�𵪽𵪦a�L�ӤF�C"NOR"\n\r", ch->short_descr , horse->description );
				else
					sprintf( buf, ""GRN"%s"GRN"�ӤF�C"NOR"\n\r", PERS( ch, rch) );
				send_to_char( buf, rch );
			}
	}

	/* Because of the addition of the deleted flag, we can do this -Kahn */
	if ( !IS_IMMORTAL( ch ) && ch->race == race_lookup( "vampire" ) && to_room->sector_type == SECT_UNDERWATER )
	{
		send_to_char( "[33m�ڡI�����O���I[m\n\r", ch );
		act( "[36m$n�b���̱ä�C[m", ch, NULL, NULL, TO_ROOM );
		////damage( ch, ch, 20, TYPE_UNDEFINED, WEAR_NONE );
		damage( ch, ch, 20, TYPE_UNDEFINED, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}
	else if ( !IS_IMMORTAL( ch )
			&& ( to_room->sector_type == SECT_UNDERWATER
			&& !IS_AFFECTED( ch, AFF_GILLS )
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_WATERBREATH ) ) )
	{
		send_to_char( "[33m�A�S�k�I�l�I[m\n\r", ch );
		act( "[36m$n����F�I[m", ch, NULL, NULL, TO_ROOM );
		////damage( ch, ch, 2, TYPE_UNDEFINED, WEAR_NONE );
		damage( ch, ch, 2, TYPE_UNDEFINED, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}

	/*
	* Suggested by D'Sai from A Moment in Tyme Mud.  Why have mobiles
	* see the room?  -Kahn
	*/
	if ( ch->desc )
	{
		do_look( ch, "move" );
	}

	SET_BIT( ch->act, moved );

	for ( fch = in_room->people; fch; fch = fch_next )
	{
		fch_next = fch->next_in_room;
		if ( fch->deleted )
			continue;

		//if( !can_see(fch, ch) ) continue; // can't follow those u can't see
		//0950523 add by jye �b���Proom�ɤ~move

		if ( (fch->master == ch && fch->in_room != ch->in_room) || ( !IS_NPC( fch ) && fch->tracking == ch && fch->position == POS_STANDING ) )
		{
			if ( fch->position == POS_STANDING && !IS_AFFECTED(fch, AFF_PARALYZED) )
			{
				act( "[36m�A���$N���C[m", fch, NULL, ch, TO_CHAR );
				move_char( fch, door );
			}
		}
	}

	REMOVE_BIT( ch->act, moved );

	// �ק� wizinv ������ pc �~�P�w 2022/10/11
	//if ( to_room->roomprogs && ( to_room->progtypes & ROOM_ENTER ) && !IS_SET( ch->act, PLR_WIZINVIS ) && ch->position > POS_DEAD )
	if ( to_room->roomprogs && ( to_room->progtypes & ROOM_ENTER )
		&& ( IS_NPC(ch) || (!IS_NPC(ch) && !IS_SET( ch->act, PLR_WIZINVIS ) ) )
		&& ch->position > POS_DEAD )
		rprog_entry_trigger( to_room, ch, door );
	if ( ch->position > POS_DEAD )
		mprog_entry_trigger( ch );
	if ( ch->position > POS_DEAD )
		mprog_greet_trigger( ch );
	// add at 2021/12/18
	if ( ch->position > POS_DEAD )
		mprog_entry_greet_trigger( ch );

	if ( ch->hunting || ch->assassinating || ch->hunted || ch->assassinated )
		check_asshunt( ch );

	if( !ch->fighting )
		aggr_check( ch , to_room);
	return;
}



void do_north( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_NORTH );
	return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_EAST );
	return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_SOUTH );
	return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_WEST );
	return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_UP );
	return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_DOWN );
	return;
}



void do_northeast( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_NORTHEAST );
	return;
}



void do_nortwest( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_NORTHWEST );
	return;
}



void do_southeast( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_SOUTHWEST );
	return;
}



void do_southwest( CHAR_DATA *ch, char *argument )
{
	if ( ch->carry_number  > can_carry_n( ch ) )
	{
		send_to_char("�A���W�F��h�o�A�ʼu���o�I\n\r", ch);
		return;
	}

	if ( ch->carry_weight > can_carry_w( ch ) )
	{
		send_to_char("�A���W�F�譫�o�A�ʼu���o�I\n\r", ch);
		return;
	}
	move_char( ch, DIR_SOUTHWEST );
	return;
}



int find_door( CHAR_DATA *ch, char *arg )
{
	EXIT_DATA *pexit;
	int door;

	if( !ch->in_room ) return -1;
	if ( !str_prefix( arg, "north" ) ) door = 0;
	else if ( !str_prefix( arg, "east"  ) ) door = 1;
	else if ( !str_prefix( arg, "south" ) ) door = 2;
	else if ( !str_prefix( arg, "west"  ) ) door = 3;
	else if ( !str_prefix( arg, "up"  ) ) door = 4;
	else if ( !str_prefix( arg, "down"  ) ) door = 5;
	else return -1;
	/*
		2021/11/23
		�]��o�䥲�M���O��, ���P�w�S������N�q, �G�����אּ reutrn -1 ��^�U�P�w��

		else
		{
			for ( door = 0; door <= 5; door++ )
			{
				if ( ( pexit = ch->in_room->exit[door] )
					&& IS_SET( pexit->exit_info, EX_ISDOOR )
					&& pexit->keyword
					&& is_name( arg, pexit->keyword ) )
					return door;
			}
			for ( door = 0; door <= 5; door++ )
			{
				if ( ( pexit = ch->in_room->exit[door] )
					&& IS_SET( pexit->exit_info, EX_ISDOOR )
					&& pexit->keyword
					&& is_pfxname( arg, pexit->keyword ) )
					return door;
			}
			//act( "[33m�ڦb�o�̨S�ݨ줰��$T�C[m", ch, NULL, arg, TO_CHAR );
			return -1;
		}
	*/

	if ( !( pexit = ch->in_room->exit[door] ) )
	{
		//act( "[33m�ڨS�ݨ�$T�����C[m", ch, NULL, arg, TO_CHAR );
		send_to_char( "[33m�u�r�A�������q�C[m\n\r", ch );
		// �ܧ� return -2 �N���P�w���V�άO��, ���O���� 2021/11/23
		//return -1;
		return -2;
	}

	if ( !IS_SET( pexit->exit_info, EX_ISDOOR ) )
	{
		//send_to_char( "[33m�A����o�򰵡C[m\n\r", ch );
		send_to_char( "[33m�o��V�A�S�ݨ���C[m\n\r", ch );
		//return -1;
		return -2;
	}

	return door;
}

void do_open( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	char      door_name [ MAX_INPUT_LENGTH ];
	int       door;

	one_argument( argument, arg );

	if( !ch->in_room ) return;

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m�n�⤰�򥴶}�H[m\n\r", ch );
		return;
	}

	if ( ( door = find_door( ch, arg ) ) >= 0 )
	{
		/* 'open door' */
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		ROOM_INDEX_DATA *to_room;

		pexit = ch->in_room->exit[door];
		if ( !IS_SET( pexit->exit_info, EX_CLOSED )  )
			{ send_to_char( "[33m�o�D���w�g�O�}�۪��F�C[m\n\r",   ch ); return; }
		if (  IS_SET( pexit->exit_info, EX_LOCKED )  )
			{ send_to_char( "[33m�o�D���Q��W�F�C[m\n\r",     ch ); return; }

		REMOVE_BIT( pexit->exit_info, EX_CLOSED );
		// �l�[��V�P�w 2021/11/23
		//send_to_char( "[36m�S���D�C[m\n\r", ch );
		switch( door )
		{
			default:
				act( "$n"NOR"��$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A��$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
			case 0:
				act( "$n"NOR"��_�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A��_�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
			case 1:
				act( "$n"NOR"��F�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A��F�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
			case 2:
				act( "$n"NOR"��n�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A��n�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
			case 3:
				act( "$n"NOR"���誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A���誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
			case 4:
				act( "$n"NOR"��W�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A��W�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
			case 5:
				act( "$n"NOR"��U�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				act( "�A��U�誺$d"NOR"�����}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				break;
		}

		/* open the other side */
		if (   ( to_room   = pexit->to_room     )
			&& ( pexit_rev = to_room->exit[rev_dir[door]] )
			&& pexit_rev->to_room == ch->in_room )
		{
			CHAR_DATA *rch;

			REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
			for ( rch = to_room->people; rch; rch = rch->next_in_room )
			{
				if ( rch->deleted )
				continue;
				//act( "$d"NOR"�}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
				switch( door )
				{
					default:
						act( "$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 0:
						act( "�n�誺$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 1:
						act( "��誺$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 2:
						act( "�_�誺$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 3:
						act( "�F�誺$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 4:
						act( "�U�誺$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 5:
						act( "�W�誺$d"NOR"���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
				}
			}
		}
		if( ch->in_room->progtypes & ROOM_TRAP ) {
			rprog_trap_trigger(ch->in_room, ch, door+6);
		}
		return;
	}
	
	/* �P�w�L�S�w��V����, ���O���Ѫ����p 2021/11/23 */
	if ( door <= -2 )
		return;

	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		/* 'open object' */
		if ( obj->item_type != ITEM_CONTAINER )
			{ send_to_char( "[33m�o�F�褣�O�Ӯe���C[m\n\r", ch ); return; }
		if ( !IS_SET( obj->value[1], CONT_CLOSED )    )
			{ send_to_char( "[33m�o�F��w�g�O�}�۪��F�C[m\n\r",    ch ); return; }
		//if ( !IS_SET( obj->value[1], CONT_CLOSEABLE ) )
		//    { send_to_char( "[33m�o�F�������_�ӡC[m\n\r",      ch ); return; }
		if (  IS_SET( obj->value[1], CONT_LOCKED )    )
			{ send_to_char( "[33m�o�F��Q��W�F�C[m\n\r",      ch ); return; }

		REMOVE_BIT( obj->value[1], CONT_CLOSED );
		//send_to_char( "[36m�S���D�C[m\n\r", ch );
		act( "�A"NOR"��$p"NOR"�����}�F�C", ch, obj, NULL, TO_CHAR );
		act( "$n"NOR"��$p"NOR"�����}�F�C", ch, obj, NULL, TO_ROOM );
		return;
	}

	return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	int       door;

	one_argument( argument, arg );

	if( !ch->in_room ) return;

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m�n�⤰�����_�ӡH[m\n\r", ch );
		return;
	}

	if ( ( door = find_door( ch, arg ) ) >= 0 )
	{
		/* 'close door' */
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		ROOM_INDEX_DATA *to_room;

		pexit      = ch->in_room->exit[door];
		if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			//send_to_char( "[33m�o�D���w�g�O���۪��F�C[m\n\r",    ch );
			act( "�o��V��$d"NOR"�w�g�O���۪��F�C",ch, NULL, pexit->keyword, TO_CHAR );
			return;
		}
		if ( IS_SET( pexit->exit_info, EX_BASHED ) )
		{
			act( "�o��$d"NOR"�O�Q���}���ҥH�����_�ӡC",ch, NULL, pexit->keyword, TO_CHAR );
			return;
		}
		SET_BIT( pexit->exit_info, EX_CLOSED );
		// �l�[��V�P�w 2021/11/23
		//send_to_char( "[36m�S���D�C[m\n\r", ch );
		switch( door )
		{
			default:
				act( "�A��$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 0:
				act( "�A��_�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��_�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 1:
				act( "�A��F�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��F�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 2:
				act( "�A��n�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��n�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 3:
				act( "�A���誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"���誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 4:
				act( "�A��W�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��W�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 5:
				act( "�A��U�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��U�誺$d"NOR"���_�ӤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
		}

		/* close the other side */
		if (   ( to_room   = pexit->to_room     )
			&& ( pexit_rev = to_room->exit[rev_dir[door]] )
			&& pexit_rev->to_room == ch->in_room )
		{
			CHAR_DATA *rch;

			SET_BIT( pexit_rev->exit_info, EX_CLOSED );
			for ( rch = to_room->people; rch; rch = rch->next_in_room )
			{
				if ( rch->deleted )
				continue;
				//act( "$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
				switch( door )
				{
					default:
						act( "$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 0:
						act( "�n�誺$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 1:
						act( "��誺$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 2:
						act( "�_�誺$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 3:
						act( "�F�誺$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 4:
						act( "�U�誺$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
					case 5:
						act( "�W�誺$d"NOR"�Q���W�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
						break;
				}
			}
		}
		if( ch->in_room->progtypes & ROOM_TRAP ) {
			rprog_trap_trigger(ch->in_room, ch, door+12);
		}
		return;
	}

	/* �P�w�L�S�w��V����, ���O���Ѫ����p 2021/11/23 */
	if ( door <= -2 )
		return;

	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		/* 'close object' */
		if ( obj->item_type != ITEM_CONTAINER )
			{ send_to_char( "[33m�o�F�褣�O�Ӯe���C[m\n\r", ch ); return; }
		if (  IS_SET( obj->value[1], CONT_CLOSED )    )
			{ send_to_char( "[33m�o�F��w�g�O���۪��F�C[m\n\r", ch ); return; }
		if ( !IS_SET( obj->value[1], CONT_CLOSEABLE ) )
			{ send_to_char( "[33m�o�F�������_�ӡC[m\n\r", ch ); return; }

		SET_BIT( obj->value[1], CONT_CLOSED );
		//send_to_char( "[36m�S���D�C[m\n\r", ch );
		act( "�A��$p"NOR"���_�ӤF�C", ch, obj, NULL, TO_CHAR );
		act( "$n"NOR"��$p"NOR"���_�ӤF�C", ch, obj, NULL, TO_ROOM );
		return;
	}

	return;
}



bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj; obj = obj->next_content )
    {
  if ( obj->pIndexData->vnum == key )
      return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	int       door;

	one_argument( argument, arg );

	if( !ch->in_room ) return;

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m�n�⤰����_�ӡH[m\n\r", ch );
		return;
	}

	if ( ( door = find_door( ch, arg ) ) >= 0 )
	{
		/* 'lock door' */
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		ROOM_INDEX_DATA *to_room;

		pexit      = ch->in_room->exit[door];
		if ( !IS_SET( pexit->exit_info, EX_CLOSED ) )
			{ send_to_char( "[33m�o�D�����O���۪��C[m\n\r",  ch ); return; }
		if ( pexit->key < 0 )
			{ send_to_char( "[33m�o�D������W��C[m\n\r",     ch ); return; }
		if ( !has_key( ch, pexit->key ) )
			{ send_to_char( "[33m�A�S���o�D�����_�͡C[m\n\r",       ch ); return; }
		if (  IS_SET( pexit->exit_info, EX_LOCKED ) )
			{ send_to_char( "[33m�o�D���w�g�Q��W�F�C[m\n\r",    ch ); return; }

		SET_BIT( pexit->exit_info, EX_LOCKED );
		// �l�[��V�P�w 2021/11/23
		//act( "$n"NOR"��$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
		send_to_char( "[36m���ذա�[m\n\r", ch );
		switch( door )
		{
			default:
				act( "�A��$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 0:
				act( "�A��_�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��_�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 1:
				act( "�A��F�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��F�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 2:
				act( "�A��n�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��n�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 3:
				act( "�A���誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"���誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 4:
				act( "�A��W�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��W�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 5:
				act( "�A��U�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��U�誺$d"NOR"����W�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
		}

		/* lock the other side */
		if (   ( to_room   = pexit->to_room     )
			&& ( pexit_rev = to_room->exit[rev_dir[door]] )
			&& pexit_rev->to_room == ch->in_room )
		{
			SET_BIT( pexit_rev->exit_info, EX_LOCKED );
		}
		if( ch->in_room->progtypes & ROOM_TRAP ) {
			rprog_trap_trigger(ch->in_room, ch, door+18);
		}
		return;
	}

	/* �P�w�L�S�w��V����, ���O���Ѫ����p 2021/11/23 */
	if ( door <= -2 )
		return;

	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		/* 'lock object' */
		if ( obj->item_type != ITEM_CONTAINER )
			{ send_to_char( "[33m�o�F�褣�O�Ӯe���C[m\n\r", ch ); return; }
		if ( !IS_SET( obj->value[1], CONT_CLOSED ) )
			{ send_to_char( "[33m�o�F�褣�O���۪��C[m\n\r",  ch ); return; }
		if ( obj->value[2] < 0 )
			{ send_to_char( "[33m�o�F�褣��W��C[m\n\r",     ch ); return; }
		if ( !has_key( ch, obj->value[2] ) )
			{ send_to_char( "[33m�A�S���o�F�誺�_�͡C[m\n\r",       ch ); return; }
		if (  IS_SET( obj->value[1], CONT_LOCKED ) )
			{ send_to_char( "[33m�o�F��w�g�Q��W�F�C[m\n\r",    ch ); return; }

		SET_BIT( obj->value[1], CONT_LOCKED );
		send_to_char( "[36m���ذա�[m\n\r", ch );
		act( "$n"NOR"��$p"NOR"����W�F�C", ch, obj, NULL, TO_ROOM );
		return;
	}

	return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	int       door;

	one_argument( argument, arg );

	if( !ch->in_room ) return;

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m�n�⤰���ꥴ�}�H[m\n\r", ch );
		return;
	}

	if ( ( door = find_door( ch, arg ) ) >= 0 )
	{
		/* 'unlock door' */
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		ROOM_INDEX_DATA *to_room;

		pexit = ch->in_room->exit[door];
		if ( !IS_SET( pexit->exit_info, EX_CLOSED ) )
			{ send_to_char( "[33m�o�D�����O���۪��C[m\n\r",  ch ); return; }
		if ( pexit->key < 0 )
			{ send_to_char( "[33m�o�D���S����A�άO�S���_�͡C[m\n\r",   ch ); return; }
		if ( !has_key( ch, pexit->key ) )
			{ send_to_char( "[33m�A�S���o�D�����_�͡C[m\n\r",       ch ); return; }
		if ( !IS_SET( pexit->exit_info, EX_LOCKED ) )
			{ send_to_char( "[33m�o�D������w�g�}�F�C[m\n\r",  ch ); return; }

		REMOVE_BIT( pexit->exit_info, EX_LOCKED );
		// �l�[��V�P�w 2021/11/23
		//act( "$n"NOR"��$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
		send_to_char( "[36m���ذա�[m\n\r", ch );
		switch( door )
		{
			default:
				act( "�A��$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 0:
				act( "�A��_�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��_�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 1:
				act( "�A��F�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��F�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 2:
				act( "�A��n�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��n�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 3:
				act( "�A���誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"���誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 4:
				act( "�A��W�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��W�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 5:
				act( "�A��U�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��U�誺$d"NOR"���ꥴ�}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
		}

		/* unlock the other side */
		if (   ( to_room   = pexit->to_room     )
			&& ( pexit_rev = to_room->exit[rev_dir[door]] )
			&& pexit_rev->to_room == ch->in_room )
		{
			REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
		}
		if( ch->in_room->progtypes & ROOM_TRAP ) {
			rprog_trap_trigger(ch->in_room, ch, door);
		}
		return;
	}
	
	/* �P�w�L�S�w��V����, ���O���Ѫ����p 2021/11/23 */
	if ( door <= -2 )
		return;

	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		/* 'unlock object' */
		if ( obj->item_type != ITEM_CONTAINER )
			{ send_to_char( "[33m�o�F�褣�O�Ӯe���C[m\n\r", ch ); return; }
		if ( !IS_SET( obj->value[1], CONT_CLOSED ) )
			{ send_to_char( "[33m�o�F�褣�O���۪��C[m\n\r",  ch ); return; }
		if ( obj->value[2] < 0 )
			{ send_to_char( "[33m�o�F�誺�ꥴ���}�C[m\n\r",   ch ); return; }
		if ( !has_key( ch, obj->value[2] ) )
			{ send_to_char( "[33m�A�S���o�F�誺�_�͡C[m\n\r",       ch ); return; }
		if ( !IS_SET( obj->value[1], CONT_LOCKED ) )
			{ send_to_char( "[33m�o�F�誺��w�g�}�F�C[m\n\r",  ch ); return; }

		REMOVE_BIT( obj->value[1], CONT_LOCKED );
		send_to_char( "[36m���ذա�[m\n\r", ch );
		act( "$n"NOR"��$p"NOR"���ꥴ�}�F�C", ch, obj, NULL, TO_ROOM );
		return;
	}

	return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *gch;
	char       arg [ MAX_INPUT_LENGTH ];
	int        door;

	one_argument( argument, arg );

	if( !ch->in_room ) return;

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m�n�⤰������}�H[m\n\r", ch );
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

	/* look for guards */
	// �N pick guard ���P�w�ò���door �� obj ���P�w 2021/11/18

	/* Check skill roll for player-char, make sure mob isn't charmed */
	/*
		�N���\�v���ܪ��Ϊ��~���P�w , �ñN pc �P npc ���}�P�w 2021/11/18

		if( (  !IS_NPC( ch ) && number_percent() > get_skill_percent(ch, gsn_pick_lock) )
			|| ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) )
		{
			send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch);
			return;
		}
	*/

	if ( ( door = find_door( ch, arg ) ) >= 0 )
	{
		/* 'pick door' */
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		ROOM_INDEX_DATA *to_room;

		pexit = ch->in_room->exit[door];
		if ( !IS_SET( pexit->exit_info, EX_CLOSED ) ){
			send_to_char( "[33m�o�D�����O���۪��C[m\n\r", ch );
			return;
		}
		if ( pexit->key < 0 ){
			send_to_char( "[33m�o�D���S����A�άO�����}�C[m\n\r", ch );
			return;
		}
		if ( !IS_SET( pexit->exit_info, EX_LOCKED ) ){
			send_to_char( "[33m�o�D������w�g�}�F�C[m\n\r", ch );
			return;
		}
		// door guard ���P�w 2021/11/18
		for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
		{
			if( !gch )
				continue;
			if( !IS_NPC( gch ) )
				continue;
			if( gch == ch)
				continue;
			if( gch->deleted )
				continue;
			if( IS_SET( gch->act, ACT_PET ) || IS_AFFECTED( gch, AFF_CHARM ) || IS_AFFECTED( gch, AFF_SUMMONED ))
				continue;
			if( is_same_group( ch, gch ) )
				continue;
			// npc �b pc leader group ��
			if( gch->group && !IS_NPC( gch->group->leader ) )
				continue;
			//if ( IS_NPC( gch ) && IS_AWAKE( gch )&& ch->level + 5 < gch->level )
			if( IS_AWAKE( gch ) && can_see(gch , ch) && IS_SET( gch->act, ACT_SENTINEL ) && IS_SET( gch->act, ACT_STAY_AREA ) )
			{
				act( "$N[33m���o���o�����b�Ӫ�F�C[m", ch, NULL, gch, TO_CHAR );
				return;
			}
		}
		if (  IS_SET( pexit->exit_info, EX_PICKPROOF ) ){
			act( "$n"NOR"�յۼ��}$d"NOR"����A�����ѤF�C", ch, NULL, pexit->keyword, TO_ROOM );
			send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch );
			return;
		}

		// �����\�v�P�w 2021/11/18
		if( !IS_NPC( ch ) ){
			if( number_percent() > get_skill_percent(ch, gsn_pick_lock) ){
				act( "$n"NOR"�յۼ��}$d"NOR"����A�����ѤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch);
				return;
			}
		}else{
			if( !IS_SET( ch->class, 4 ) ){
				act( "$n"NOR"�յۼ��}$d"NOR"����A�����ѤF�C", ch, NULL, pexit->keyword, TO_ROOM );
				send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch);
				return;
			}
		}

		REMOVE_BIT( pexit->exit_info, EX_LOCKED );
		// �l�[��V�P�w 2021/11/23
		//act( "$n"NOR"��$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
		send_to_char( "[36m���ذա�[m\n\r", ch );
		switch( door )
		{
			default:
				act( "�A��$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 0:
				act( "�A��_�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��_�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 1:
				act( "�A��F�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��F�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 2:
				act( "�A��n�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��n�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 3:
				act( "�A���誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"���誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 4:
				act( "�A��W�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��W�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
			case 5:
				act( "�A��U�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
				act( "$n"NOR"��U�誺$d"NOR"���굹���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
				break;
		}

		/* pick the other side */
		if ( ( to_room = pexit->to_room )
			&& ( pexit_rev = to_room->exit[rev_dir[door]] )
			&& pexit_rev->to_room == ch->in_room )
		{
			REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
		}
		if( ch->in_room->progtypes & ROOM_TRAP ) {
			rprog_trap_trigger(ch->in_room, ch, door);
		}

		return;
	}
	
	/* �P�w�L�S�w��V����, ���O���Ѫ����p 2021/11/23 */
	if ( door <= -2 )
		return;

	// �P�w�ж��������~
	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		/* 'pick object' */
		if ( obj->item_type != ITEM_CONTAINER ){
			send_to_char( "[33m�o�F�褣�O�Ӯe���C[m\n\r", ch );
			return;
		}
		if ( !IS_SET( obj->value[1], CONT_CLOSED ) ){
			send_to_char( "[33m�o�F�褣�O���۪��C[m\n\r", ch );
			return;
		}
		if ( obj->value[2] < 0 ){
			send_to_char( "[33m�o�F�誺�ꥴ���}�C[m\n\r", ch );
			return;
		}
		if ( !IS_SET( obj->value[1], CONT_LOCKED ) ){
			send_to_char( "[33m�o�F�誺��w�g�}�F�C[m\n\r", ch );
			return;
		}
		/*
		// obj guard ���P�w 2021/11/18
		for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
		{
			// �P�w�� ch ���W���������~
			if ( ( obj = get_obj_carry( ch, arg ) ) || ( obj = get_obj_wear( ch, arg ) ) )
				break;
			if( !gch )
				continue;
			if( !IS_NPC( gch ) )
				continue;
			if( gch == ch)
				continue;
			if( gch->deleted )
				continue;
			if( IS_SET( gch->act, ACT_PET ) || IS_AFFECTED( gch, AFF_CHARM ) || IS_AFFECTED( gch, AFF_SUMMONED ))
				continue;
			if( is_same_group( ch, gch ) )
				continue;
			// npc �b pc leader group ��
			if( gch->group && !IS_NPC( gch->group->leader ) )
				continue;
			//if ( IS_NPC( gch ) && IS_AWAKE( gch )&& ch->level + 5 < gch->level )
			if( IS_AWAKE( gch ) && can_see(gch , ch) && IS_SET( gch->act, ACT_SENTINEL ) && IS_SET( gch->act, ACT_STAY_AREA ) )
			{
				act( "$N[33m���o���o���~�����b�Ӫ�F�C[m", ch, NULL, gch, TO_CHAR );
				return;
			}
		}
		*/

		if (  IS_SET( obj->value[1], CONT_PICKPROOF ) ){
			send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch );
			return;
		}

		// ���~���\�v�P�w 2021/11/18
		if( !IS_NPC( ch ) ){
			if( number_percent() > get_skill_percent(ch, gsn_pick_lock) ){
				act( "$n"NOR"�յۼ��}$p"NOR"����A�����ѤF�C", ch, obj, NULL, TO_ROOM );
				send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch);
				return;
			}
		}else{
			if( !IS_SET( ch->class, 4 ) ){
				act( "$n"NOR"�յۼ��}$p"NOR"����A�����ѤF�C", ch, obj, NULL, TO_ROOM );
				send_to_char( "[33m�A�S�ন�\\�C[m\n\r", ch);
				return;
			}
		}

		REMOVE_BIT( obj->value[1], CONT_LOCKED );
		send_to_char( "[36m���ذա�[m\n\r", ch );
		act( "$n"NOR"��$p"NOR"���굹���}�F�C", ch, obj, NULL, TO_ROOM );
		return;
	}
	// find nothing 2021/11/23
	send_to_char( "�o�̨S���o����C\n\r", ch );
	return;
}

//modify by jye 0940625
void do_stand( CHAR_DATA *ch, char *argument )
{
  if( !ch->in_room ) return;
  
  switch ( ch->position )
  {
     case POS_SLEEPING:
         if( IS_AFFECTED( ch, AFF_SLEEP ) )
         { 
         	   send_to_char( "[33m�A�����L�ӡI[m\n\r", ch ); return; 
         }

         send_to_char( "[36m�A���F�ï��F�_�ӡC[m\n\r", ch );
         act( "[36m$n���F�ï��F�_�ӡC[m", ch, NULL, NULL, TO_ROOM );
         ch->position = POS_STANDING;
//  if( !IS_NPC(ch) && IS_SET(ch->act, PLR_MEDITATING)) {
//      REMOVE_BIT(ch->act, PLR_MEDITATING);
//  }
         break;

     case POS_RESTING:
         send_to_char( "[36m�A���F�_�ӡC[m\n\r", ch );
         act( "[36m$n���F�_�ӡC[m", ch, NULL, NULL, TO_ROOM );
         if( !ch->fighting)
              ch->position = POS_STANDING;
         else 
              ch->position = POS_FIGHTING;
         break;

     case POS_FIGHTING:
         send_to_char( "[33m�A�w�g�b���[�F�I[m\n\r",  ch );
         break;
     case POS_STANDING:
         send_to_char( "[33m�A�w�g�O���۪��F�C[m\n\r",  ch );
         break;
  }

  return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    if( !ch->in_room ) return;
    switch ( ch->position )
    {
    case POS_SLEEPING:
  send_to_char( "[33m�A�w�g�b��ı�F�C[m\n\r",  ch );
  break;

    case POS_RESTING:
  send_to_char( "[33m�A�w�g���ۥ𮧤F�C[m\n\r",   ch );
  break;

    case POS_FIGHTING:
  send_to_char( "[33m�b���[���ɭԤ���I[m\n\r", ch );
  break;

    case POS_STANDING:
  send_to_char( "[36m�A���U�ӥ𮧡C[m\n\r", ch );
  act( "[36m$n���U�ӥ𮧡C[m", ch, NULL, NULL, TO_ROOM );
  ch->position = POS_RESTING;
  break;
    }

    return;
}

void do_medi( CHAR_DATA *ch, char *argument )
{
  send_to_char( "�p�G�A�Q�n�߷Q�A�A�o�⥦������(meditate)�C\n\r", ch );
  return;
}

void do_destro( CHAR_DATA *ch, char* argument )
{
  send_to_char( "If you want to destroy, you need to type correctly(destroy).\n\r",ch);
  return;
} 

void do_meditate( CHAR_DATA *ch, char *argument )
{
	//  CHAR_DATA *vch;
	AFFECT_DATA af;
	int sklv = 0;
	//  int   number = 0;

	if( !ch->in_room ) return;
	if( IS_NPC(ch) ) return;
	if( !get_skill_percent( ch, gsn_meditate ) ){
		send_to_char("�A�S�ǹL���߷Q�I\n\r", ch);
		return;
	}
	sklv = get_skill_level(ch, gsn_meditate);
	if( is_affected(ch, gsn_meditate) ){
		send_to_char("�A�w�g�b�߷Q�F�I\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	send_to_char( "[36m�A���ؾ����A�}�l�߷Q�C[m\n\r", ch );
	act( "[36m$n�����믫�A�}�l�߷Q�C[m", ch, NULL, NULL, TO_ROOM );
	af.type  = skill_lookup( "meditate" );
	af.duration  = number_fuzzy( get_skill_percent(ch, gsn_meditate) / 12 );
	af.location  = APPLY_DODGE;
	af.bitvector = 0;//AFF_MEDITATING;
	af.modifier  = 200 - sklv*25;
	affect_join( ch, &af );
	af.location  = APPLY_AC;
	af.modifier  = 240 - sklv*35;
	affect_join( ch, &af );
	WAIT_STATE( ch, skill_table[gsn_meditate].beats * (15 - sklv) / 20);
	af.location  = APPLY_ACTION_ABILITY;
	af.modifier  = -50 + sklv*7;
	affect_join( ch, &af );
	return;
}

void do_sleep( CHAR_DATA *ch, char *argument )
{
	if( !ch->in_room ) return;
	switch ( ch->position )
	{
		case POS_SLEEPING:
			send_to_char( "[33m�A�w�g�b��ı�F�C[m\n\r",  ch );
			break;

		case POS_RESTING:
		case POS_STANDING: 
			send_to_char( "[36m�A���U�Ӻ�ı�C[m\n\r", ch );
			act( "[36m$n���U�Ӻ�ı�C[m", ch, NULL, NULL, TO_ROOM );
			ch->position = POS_SLEEPING;
			break;

		case POS_FIGHTING:
			send_to_char( "[33m�A���[���ɭԤ���I[m\n\r", ch );
			break;
	}

	return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );
	if( !ch->in_room ) return;
	if ( arg[0] == '\0' )
		{ do_stand( ch, argument ); return; }

	if ( !IS_AWAKE( ch ) )
		{ send_to_char( "[33m�A�ۤv�]���O���۪��I[m\n\r", ch ); return; }

	if ( !( victim = get_char_room( ch, arg ) ) )
		{ send_to_char( "[33m�L���b�o��C[m\n\r", ch ); return; }

	if ( IS_AWAKE( victim ) )
		{ act( "[33m$N���ӴN�O���۪��C[m", ch, NULL, victim, TO_CHAR ); return; }

	if ( IS_AFFECTED( victim, AFF_SLEEP ) )
		{ act( "[33m�A�s����$M�I[m", ch, NULL, victim, TO_CHAR ); return; }

	victim->position = POS_STANDING;
	act( "[36m�A��$M�s���C[m", ch, NULL, victim, TO_CHAR );
	act( "[36m$n��A�s���C[m", ch, NULL, victim, TO_VICT );
	// if( !IS_NPC(ch) && IS_SET(ch->act, PLR_MEDITATING)) {
	// REMOVE_BIT(ch->act, PLR_MEDITATING);
	// }
	return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;

	if( !ch->in_room ) return;
	/* Don't allow charmed mobs to do this, check player's skill */
	if ( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) || ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_sneak) == 0 ))
	{
		send_to_char( "[33mԣ�H[m\n\r", ch );
		return;
	}

	send_to_char( "[36m�A�յۮ����a���ʡC[m\n\r", ch );
	affect_strip( ch, gsn_sneak );

	if ( IS_NPC( ch ) || number_percent( ) < get_skill_percent(ch, gsn_sneak))//ch->pcdata->learned[gsn_sneak] )
	{
		af.type      = gsn_sneak;
		af.duration  = ch->level + get_skill_level(ch, gsn_sneak)*3;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_SNEAK;
		affect_to_char( ch, &af );
	}

	return;
}

void do_nightstalk( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	CHAR_DATA *rch;
	int nSuccess = 100;
	int nNightSklv = get_skill_level(ch, gsn_nightstalk);
	int nWait = skill_table[gsn_nightstalk].beats;
	int nTimeBonus = nNightSklv - 4;

	if (nTimeBonus < 0) nTimeBonus = 0;
	if (nNightSklv >= 1) nWait -= nNightSklv + 3;

	if( !ch->in_room ) return;
	WAIT_STATE( ch, nWait);
    
	/* Don't allow charmed mobs to do this, check player's skill */
	if ( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
		|| ( !IS_NPC( ch )
		&& number_percent() > get_skill_percent( ch, gsn_nightstalk)))
	{
		send_to_char( "[33m�A���ѤF�C[m\n\r", ch );
		return;
	}

	if ( ch->move < 300 - get_skill_level(ch, gsn_nightstalk) * 20)
	{
		send_to_char( "[36m�A�S����������O�I[m\n\r", ch );
		return;
	}

	// ��O���Ӳ��s���ѩΦ��\���񤧤�
	//ch->move -= 300 - get_skill_level(ch, gsn_nightstalk) * 20;

	if ( time_info.hour > 4 + nTimeBonus && time_info.hour < 20 - nTimeBonus)
	{
		send_to_char( "[36m�Ѧ��A�Q�������G�A�A���ѤF�I[m\n\r", ch );
		return;
	}

	if (ch->position == POS_FIGHTING) {
		int nSameGroupLv = 0;
		int nDiffGroupLv = 0;
		int nAdjustedLv = 0;
		for ( rch = ch->in_room->people; rch; rch = rch->next_in_room ) {
			if ( is_same_group( ch, rch ) ){
				nAdjustedLv = 1 + ( rch->level * nNightSklv)/40;
				if (nAdjustedLv < 0) nAdjustedLv = 0;
				nSameGroupLv += nAdjustedLv;
			} else {
				//nDiffGroupLv += rch->level;
				// �ж����D�԰��ͪ� , �̬O�_�P�}��P�w���\�v�W�ɩΤU��
				if ( !rch->fighting
					&& can_see( rch , ch)
					&& !IS_AFFECTED( rch, AFF_CHARM )
					&& !IS_AFFECTED( rch, AFF_SLEEP )
					&& !IS_AFFECTED( rch, AFF_CRAZY ))
				{
					if ( rch->level <= ch->level
						|| ( rch->level > ch->level && rch->level - ch->level <= nNightSklv ) )
					{
						if ( IS_GOOD(rch) ){
							if( IS_GOOD(ch) )    nSameGroupLv += rch->level/(20 - nNightSklv);
							if( IS_EVIL(ch) )    nDiffGroupLv += rch->level/(1 + nNightSklv);
						}
						if ( IS_NEUTRAL(rch) ){
							if( IS_NEUTRAL(ch) ) nSameGroupLv += rch->level/(20 - nNightSklv);
						}
						if ( IS_EVIL(rch) ){
							if( IS_GOOD(ch) )    nDiffGroupLv += rch->level/(1 + nNightSklv);
							if( IS_EVIL(ch) )    nSameGroupLv += rch->level/(20 - nNightSklv);
						}
					}
				}
			}
			if (rch->fighting == ch) {
				//nDiffGroupLv += rch->level - 31;
				nDiffGroupLv += rch->level - ch->level;
			}
		}

		if( nDiffGroupLv <= -100 ) nDiffGroupLv = -99;

		//nSuccess = 100 * (nSameGroupLv + nNightSklv * nNightSklv * 2.5) / (nDiffGroupLv + 100 - nNightSklv * nNightSklv);
		nSuccess = 100 * (nSameGroupLv + nNightSklv * 16) / (nDiffGroupLv + 100);

		// �a���򥻦��\�v 2020/09/16
		if (ch->in_room->sector_type == SECT_CITY)     nSuccess += 2 * nNightSklv;
		if (ch->in_room->sector_type == SECT_FOREST)   nSuccess += 2 * nNightSklv;
		if (ch->in_room->sector_type == SECT_HILLS)    nSuccess += nNightSklv;
		if (ch->in_room->sector_type == SECT_MOUNTAIN) nSuccess += nNightSklv;
	}

	if (number_percent() > nSuccess) {
		ch->move -= 300 - get_skill_level(ch, gsn_nightstalk) * 20;
		send_to_char("�A�յۦb�԰����ðΧA�����v�A�i�����ѤF�I\n\r", ch);
		return;
	}

	//send_to_char( "[36m�A�V�J���Ҫ��t�v�����A�����԰��C[m\n\r", ch );
	//affect_strip( ch, gsn_nightstalk );

	if (ch->position == POS_FIGHTING) {
		send_to_char( "[36m�A�V�J���Ҫ��t�v�����A�����԰��C[m\n\r", ch );
		stop_fighting(ch, FALSE);
		for ( rch = ch->in_room->people; rch; rch = rch->next_in_room ) {
			if (rch->fighting == ch) {
				stop_fighting(rch, FALSE);
			}
		}
	}else if ( IS_AFFECTED( ch, AFF_NIGHTSTALK)){
		send_to_char( "�A�w�g���æb�t�v�����F�C\n\r", ch );
		return;
	}else{
		send_to_char( "[36m�ǵ۶©]���t�v�A�A�յۮ����a���ʡC[m\n\r", ch );
	}
	ch->move -= 300 - get_skill_level(ch, gsn_nightstalk) * 20;

	affect_strip( ch, gsn_nightstalk );

	af.type      = gsn_nightstalk;
	af.duration  = 12;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_NIGHTSTALK;
	affect_to_char( ch, &af );

	return;
}


void do_hide( CHAR_DATA *ch, char *argument )
{
	/* Dont allow charmed mobiles to do this, check player's skill */
	if( !ch->in_room ) return;
	if ( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
		|| ( !IS_NPC( ch )
		//&& ch->pcdata->learned[gsn_hide] == 0 ) )
		&& get_skill_percent( ch, gsn_hide) == 0 ))
	{
		send_to_char( "[33mԣ�H[m\n\r", ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_HIDE ) )
	{
		REMOVE_BIT( ch->affected_by, AFF_HIDE);
		send_to_char( "�A�����ðΡC\n\r", ch );
		return;
	}
	if ( IS_NPC( ch ) || number_percent( ) < get_skill_percent(ch, gsn_hide))//ch->pcdata->learned[gsn_hide] )
	{
		SET_BIT( ch->affected_by, AFF_HIDE);
		send_to_char( "[36m�A�ǵۦa�Ϊ����@�A���ð_�ӡC[m\n\r", ch );
	}
	else
		send_to_char( "[36m�A���ѤF�C[m\n\r", ch );
	return;
}



/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
	char arg  [MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg);
	if( arg[0] == '\0' || !str_cmp(arg, "all")) {
		affect_strip ( ch, gsn_invis );
		affect_strip ( ch, gsn_mass_invis );
		affect_strip ( ch, gsn_sneak );
		affect_strip ( ch, gsn_shadow );
		affect_strip ( ch, gsn_nightstalk );
		REMOVE_BIT   ( ch->affected_by, AFF_HIDE );
		REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE );
		REMOVE_BIT   ( ch->affected_by, AFF_SNEAK );
		REMOVE_BIT   ( ch->affected_by, AFF_NIGHTSTALK ); 
	}
	else if ( !str_cmp(arg, "invis")) {
		affect_strip ( ch, gsn_invis );
		REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE );
		affect_strip ( ch, gsn_mass_invis );
	}
	else if ( !str_cmp(arg, "hide")){
		REMOVE_BIT   ( ch->affected_by, AFF_HIDE );
	}
	else if( !str_cmp(arg, "sneak")){
		affect_strip ( ch, gsn_sneak );
		affect_strip ( ch, gsn_shadow );
		REMOVE_BIT   ( ch->affected_by, AFF_SNEAK );
	}
	else if ( !str_cmp("nightstalk", arg)){
		affect_strip ( ch, gsn_nightstalk );
		REMOVE_BIT   ( ch->affected_by, AFF_NIGHTSTALK );
	}
	else {
		send_to_char("�S���o�����O�C\n\r", ch);
		return;
	}
	send_to_char( "[36m�S���D�C[m\n\r", ch );
	return;
}

// disperse add by whisperwind 11.18.2004
void do_disperse( CHAR_DATA *ch, char *argument )
{
	char arg  [MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg);

	if( arg[0] == '\0' || !str_cmp(arg, "all")) 
	{
		if(is_affected ( ch, gsn_mana_shield) ) WAIT_MAGIC_STATE(ch, 0);
		if(is_affected ( ch, gsn_mystic_mirror) ) WAIT_MAGIC_STATE(ch, 0);
		if(is_affected ( ch, gsn_mystic_armor) ) WAIT_MAGIC_STATE(ch, 0);
		if(is_affected ( ch, gsn_flaming_armor) ) WAIT_MAGIC_STATE(ch, 0);
		if(is_affected ( ch, gsn_powerup) ) WAIT_MAGIC_STATE(ch, 0);
		if(is_affected ( ch, gsn_hasten) ) WAIT_MAGIC_STATE(ch, 0);

		affect_strip ( ch, gsn_mana_shield);
		affect_strip ( ch, gsn_mystic_mirror);
		affect_strip ( ch, gsn_mystic_armor);
		affect_strip ( ch, gsn_flaming_armor);
		affect_strip ( ch, gsn_powerup);
		affect_strip ( ch, gsn_hasten);
	}
	else if( !str_cmp(arg, "trueshot")) 
	{
		affect_strip ( ch, gsn_trueshot);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "powerbolt")) 
	{
		affect_strip ( ch, gsn_powerbolt);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "fly"))
	{
		affect_strip ( ch, gsn_flying);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "mana shield"))
	{
		affect_strip ( ch, gsn_mana_shield);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if(!str_cmp(arg, "shield"))
	{
		affect_strip ( ch, gsn_mana_shield);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "mystic armor"))
	{
		affect_strip ( ch, gsn_mystic_armor);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "armor")) 
	{
		affect_strip ( ch, gsn_mystic_armor);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "mystic mirror"))
	{
		affect_strip ( ch, gsn_mystic_mirror);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "mirror")) 
	{
		affect_strip ( ch, gsn_mystic_mirror);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "flaming armor"))
	{
		affect_strip ( ch, gsn_flaming_armor);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "flaming")) 
	{
		affect_strip ( ch, gsn_flaming_armor);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "powerup"))
	{ 
		affect_strip ( ch, gsn_powerup);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "hasten"))
	{ 
		affect_strip ( ch, gsn_hasten);
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "day of power") ||
	!str_cmp(arg, "day"))
	{ 
		affect_strip( ch, skill_lookup("day of power"));
		WAIT_MAGIC_STATE(ch, 0);
	}
	else if( !str_cmp(arg, "holy crown") ||
		!str_cmp(arg, "holy") ||
		!str_cmp(arg, "crown"))
	{ 
		affect_strip( ch, skill_lookup("holy crown"));
		WAIT_MAGIC_STATE(ch, 0);
	}
	else 
	{
		send_to_char("�S���o�����O�C\n\r", ch);
		return;
	}

	send_to_char( "[36m�A�Ѱ��F���W�������C[m\n\r", ch );
	return;
}

void do_recall( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA     *victim, *horse;
    ROOM_INDEX_DATA *location;
    char       buf [ MAX_STRING_LENGTH ];
    int        place;

    if( !ch->in_room ) return;
    if( auction_list ) {
        AUC_DATA * auction;
        for( auction = auction_list ; auction; auction = auction->next ){
            if( ch == auction->buyer || ch == auction->owner )
            {
                send_to_char("�A���b�i����A�����D�ǰe�C\n\r", ch);
                return;
            }
        }
    }

    act( "[36m$n��D�ǰe�I[m", ch, NULL, NULL, TO_ROOM );

    if ( IS_SET( ch->in_room->room_flags, ROOM_NO_RECALL )
         || IS_AFFECTED( ch, AFF_CURSE ) )
    {
        send_to_char( "[33m�����F�A�C[m\n\r", ch );
        return;
    }

    place = ch->in_room->area->recall;
    if ( !( location = get_room_index( place ) ) )
    {
        send_to_char( "[33m�A���������a�g���F�C[m\n\r", ch );
        return;
    }

    if ( ch->in_room == location )
        return;

    if ( ( victim = ch->fighting ) )
    {
        int lose;

        if ( number_bits( 1 ) == 0 )
        {
            WAIT_STATE( ch, 4 );
            lose = ( ch->desc ) ? 12 : 25;
            gain_exp( ch, 0 - lose );
            sprintf( buf, "[33m�A�S�ন�\\�I�A���h�F"HIC"%d"CYN"�I�g��ȡC[m\n\r", lose );
            send_to_char( buf, ch );
            return;
        }

        lose = ( ch->desc ) ? 25 : 50;
        gain_exp( ch, 0 - lose );
        sprintf( buf, "[36m�A�q�԰����ǰe�M�^�I�A���h�F"HIC"%d"CYN"�I�g��ȡC[m\n\r", lose );
        send_to_char( buf, ch );
        stop_fighting( ch, TRUE );
    }

    ch->move /= 2;

	// �ץ� wizinv ������ pc �~�|�P�w 2022/10/11
    if ( ch->in_room->roomprogs
		&& ( ch->in_room->progtypes & ROOM_LEAVE )
		&& ( IS_NPC(ch) || (!IS_NPC(ch) && !IS_SET( ch->act, PLR_WIZINVIS ) ) ) )
        rprog_leave_trigger( ch->in_room, ch, 6 );
    
    act( "[36m$n�����F�C[m", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "[36m$n���M�b�o�̥X�{�C[m", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if ( ch->position > POS_DEAD ) // Added by Amethyst
        mprog_greet_trigger( ch ); // Added by Amethyst

    if ( ch->pcdata && ch->pcdata->horse && ch->pcdata->horse && ch->pcdata->horse->in_room )
    {
        if ( ch->in_room != ( horse = ch->pcdata->horse )->in_room )
        {
          char_from_room( horse );
            char_to_room( horse, location);
          send_to_char("�A���԰��^��F�A������C\n\r", victim);
        }
    }

    dm_effect(ch);
  
    return;
}



void do_train( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *mob;
	AFFECT_DATA *paf;
	char *pOutput;
	char buf [ MAX_STRING_LENGTH ];
	bool ok = FALSE;
	int *pAbility;
	int cost;
	int money;
	int scale = ch->level * 5;
	int hp_f = 30, mp_f = 10, mv_f = 10;
	int newbie = 800, oldbie = 600;
	int hp_b = 1200, mp_b = 800, mv_b = 900;
	int bone_flag = 0; /*Added for training of hp ma mv */
	int nHpFee = 0;
	int nMnFee = 0;
	int nMvFee = 0;
	int times = 0; // add at 2022/02/08

	if ( IS_NPC( ch ) )
		return;


	if( !ch->in_room ) return;
    /*
     * Check for trainer.
     */
	for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
	{
		if ( IS_NPC( mob ) && IS_SET( mob->act, ACT_TRAIN ) )
			break;
	}

	if ( !mob )
	{
		send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
		return;
	}

	if ( argument[0] == '\0' )
	{
		sprintf( buf, "[36m�A��%d�I�m���I�ơC[m\n\r", ch->practice );
		send_to_char( buf, ch );
		argument = "foo";
	}

	/* Calculate all factors */
	switch( ch->pcdata->prime_attr ){
	case APPLY_CON:
		hp_f += 30;
		mp_f += 5;
		mv_f += 15;
		break;
	case APPLY_STR:
		hp_f += 20;
		mv_f += 25;
		mp_f += 5;
		break;
	case APPLY_INT:
		hp_f -= 10;
		mp_f += 45;
		mv_f += 15;
		break;
	case APPLY_DEX:
		mv_f += 35;
		mp_f += 5;
		hp_f += 10;
		break;
	case APPLY_WIS:
		mp_f += 35;
		hp_f -= 5;
		mv_f += 20;
		break;
	}
	if( ch->class > 4 ) {
		hp_f += 20;
		mp_f += 20;
		mv_f += 20;
		// �[�J�i��¾�~���ݩ��B�~�[�� 2022/04/23
		switch( ch->pcdata->snd_attr ){
		case APPLY_CON:
			hp_f += 10;
			mp_f += 2;
			mv_f += 3;
			break;
		case APPLY_STR:
			hp_f += 5;
			mp_f += 5;
			mv_f += 5;
			break;
		case APPLY_INT:
			hp_f += 2;
			mp_f += 11;
			mv_f += 2;
			break;
		case APPLY_DEX:
			hp_f += 4;
			mp_f += 3;
			mv_f += 8;
			break;
		case APPLY_WIS:
			hp_f += 3;
			mp_f += 8;
			mv_f += 4;
			break;
		}
	}


	hp_f *= PPL_HP_MULTIPLYER;
	mp_f *= PPL_HP_MULTIPLYER;
	mv_f *= PPL_HP_MULTIPLYER;

	cost  = 5;
	money = ch->level * ch->level * 100;

	hp_b = ((ch->level * hp_f ) )/2;
	mp_b = ((ch->level * mp_f ) )/2;
	mv_b = ((ch->level * mv_f ) )/2;
	nHpFee = (ch->class > 4 ) ? 10000 * ((ch->max_hit / oldbie/2)+1): 8000 * ((ch->max_hit/ newbie / 2)+1);
	nMnFee = (ch->class > 4 ) ? 15000 * ((ch->max_mana / oldbie)+1): 12000 * ((ch->max_mana / newbie)+1);
	nMvFee = (ch->class > 4 ) ? 10000 * ((ch->max_move/ oldbie)+1): 8000 * ((ch->max_move/ newbie)+1);
	nHpFee /= PPL_HP_MULTIPLYER;
	nMnFee /= PPL_HP_MULTIPLYER;
	nMvFee /= PPL_HP_MULTIPLYER;

	if ( !str_cmp( argument, "str" ) )
	{
		// �P�w���L�@�κر��ܤ�( �ܱڦ��L polymorph flag�����p)
		if( !is_affected(ch, gsn_werewolf) ){
			for ( paf = ch->affected; paf; paf = paf->next ){
				if ( paf->deleted )
					continue;
				if ( paf->location == APPLY_RACE ){
					send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
					return;
				}
			}
		}else if( ch->race != race_lookup( "Werewolf" )
				  && ch->race != race_lookup( "Human" ) ){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if( IS_AFFECTED( ch, AFF_POLYMORPH )){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if ( class_table[ch->class].attr_prime == APPLY_STR )
			cost = 3;
		pAbility = &ch->perm_str;
		pOutput = "�O�q";
	}
	else if ( !str_cmp( argument, "int" ) )
	{
		// �P�w���L�@�κر��ܤ�( �ܱڦ��L polymorph flag�����p)
		if( !is_affected(ch, gsn_werewolf) ){
			for ( paf = ch->affected; paf; paf = paf->next ){
				if ( paf->deleted ) continue;
				if ( paf->location == APPLY_RACE ){
					send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
					return;
				}
			}
		}else if( ch->race != race_lookup( "Werewolf" )
				  && ch->race != race_lookup( "Human" ) ){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if( IS_AFFECTED( ch, AFF_POLYMORPH )){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if ( class_table[ch->class].attr_prime == APPLY_INT )
			cost = 3;
		pAbility = &ch->perm_int;
		pOutput = "����";
	}
	else if ( !str_cmp( argument, "wis" ) )
	{
		// �P�w���L�@�κر��ܤ�( �ܱڦ��L polymorph flag�����p)
		if( !is_affected(ch, gsn_werewolf) ){
			for ( paf = ch->affected; paf; paf = paf->next ){
				if ( paf->deleted ) continue;
				if ( paf->location == APPLY_RACE ){
					send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
					return;
				}
			}
		}else if( ch->race != race_lookup( "Werewolf" )
				  && ch->race != race_lookup( "Human" ) ){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if( IS_AFFECTED( ch, AFF_POLYMORPH )){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if ( class_table[ch->class].attr_prime == APPLY_WIS )
			cost = 3;
		pAbility = &ch->perm_wis;
		pOutput = "���z";
	}
	else if ( !str_cmp( argument, "dex" ) )
	{
		// �P�w���L�@�κر��ܤ�( �ܱڦ��L polymorph flag�����p)
		if( !is_affected(ch, gsn_werewolf) ){
			for ( paf = ch->affected; paf; paf = paf->next ){
				if ( paf->deleted ) continue;
				if ( paf->location == APPLY_RACE ){
					send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
					return;
				}
			}
		}else if( ch->race != race_lookup( "Werewolf" )
				  && ch->race != race_lookup( "Human" ) ){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if( IS_AFFECTED( ch, AFF_POLYMORPH )){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if ( class_table[ch->class].attr_prime == APPLY_DEX )
			cost = 3;
		pAbility = &ch->perm_dex;
		pOutput = "�ӱ�";
	}
	else if ( !str_cmp( argument, "con" ) )
	{
		// �P�w���L�@�κر��ܤ�( �ܱڦ��L polymorph flag�����p)
		if( !is_affected(ch, gsn_werewolf) ){
			for ( paf = ch->affected; paf; paf = paf->next ){
				if ( paf->deleted ) continue;
				if ( paf->location == APPLY_RACE ){
					send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
					return;
				}
			}
		}else if( ch->race != race_lookup( "Werewolf" )
				  && ch->race != race_lookup( "Human" ) ){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if( IS_AFFECTED( ch, AFF_POLYMORPH )){
			send_to_char("�ر��ܲ�������V�m�ݩʡC\n\r", ch);
			return;
		}
		if ( class_table[ch->class].attr_prime == APPLY_CON )
			cost = 3;
		pAbility = &ch->perm_con;
		pOutput = "���";
	}
	else if ( !str_cmp( argument, "hp" ) )
	{
		/*
		if( is_affected ( ch, gsn_hell_southencross ) )
		{
			return;
		}
		*/
		// �u�n���W�a���v�T hp �� affect �N�L�k train hp , add at 200131
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->location == APPLY_HIT ){
				send_to_char("�A�{�b�����A�L�k���ɥͩR�O�C\n\r", ch);
				return;
			}
		}
		scale = ch->level * 5 * PPL_HP_MULTIPLYER;

		if( ch->max_hit >= ch->level * hp_f) {
		  send_to_char("�A���ͩR�O�w�g�W�L�W���F�C���h���I���ŧa�C\n\r", ch);
		  return;
		}
		cost = (ch->max_hit > hp_b ) ? ((ch->max_hit- hp_b) / scale)+1 : 1;
		cost = UMAX(1, cost);
		money = nHpFee;
		  
		bone_flag = 1;
		pAbility = &ch->max_hit;
		pOutput = "�ͩR�O";
	}
	  
	else if ( !str_cmp( argument, "mana" ) )
	{
		/*
		if( is_affected ( ch, gsn_hell_southencross ) )
		{
			return;
		}
		*/
		// �u�n���W�a���v�T mp �� affect �N�L�k train mp , add at 200131
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->location == APPLY_MANA ){
				send_to_char("�A�{�b�����A�L�k�����]�O�C\n\r", ch);
				return;
			}
		}
		if( ch->max_mana >= ch->level * mp_f) {
			send_to_char("�A���]�O�w�g�W�L�W���F�C���h���I���ŧa�C\n\r", ch);
			return;
		}
		scale = ch->level * 3 * PPL_HP_MULTIPLYER;
		cost = (ch->max_mana > mp_b ) ? ((ch->max_mana -mp_b)/ scale)+1 :  1;
		cost = UMAX(1, cost);
		money = nMnFee;
		bone_flag = 1;
		pAbility = &ch->max_mana;
		pOutput = "�]�k�O";
	}

	else if ( !str_cmp( argument, "move" ) )
	{
		/*
		if( is_affected ( ch, gsn_hell_southencross ) )
		{
			return;
		}
		*/
		// �u�n���W�a���v�T mv �� affect �N�L�k train mv , add at 200131
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->location == APPLY_MOVE ){
				send_to_char("�A�{�b�����A�L�k���ɲ��ʤO�C\n\r", ch);
				return;
			}
		}
		if( ch->max_move >= ch->level * mv_f ) {
			send_to_char("�A�����ʤO�w�g�W�L�W���F�C���h���I���ŧa�C\n\r", ch);
			return;
		}
		scale = ch->level * 3 * PPL_HP_MULTIPLYER;
		cost = (ch->max_move > mv_b) ? ((ch->max_move-mv_b) / scale)+1 : 1;
		cost = UMAX(1, cost);
		money = nMvFee;
		bone_flag = 2;
		pAbility = &ch->max_move;
		pOutput = "���ʤO";
	}
	else if ( !str_cmp( argument, "practice" ) )
	{
		cost = 0;
		money = ch->level * ch->level * 10;
		bone_flag = 1;
		pAbility = &ch->practice;
		pOutput = "�m���I";
	}
	// �s�W�s�� train 2022/02/09
	else if ( !str_cmp( argument, "hp-r" ) )
	{
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->location == APPLY_HIT ){
				send_to_char("�A�{�b�����A�L�k���ɥͩR�O�C\n\r", ch);
				return;
			}
		}
		scale = ch->level * 5 * PPL_HP_MULTIPLYER;

		if( ch->max_hit >= ch->level * hp_f) {
		  send_to_char("�A���ͩR�O�w�g�W�L�W���F�C���h���I���ŧa�C\n\r", ch);
		  return;
		}
		times = 1;
		cost = (ch->max_hit > hp_b ) ? ((ch->max_hit- hp_b) / scale)+1 : 1;
		cost = UMAX(1, cost);
		money = nHpFee;
		  
		bone_flag = 1;
		pAbility = &ch->max_hit;
		pOutput = "�ͩR�O";
	}
	else if ( !str_cmp( argument, "mana-r" ) )
	{
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->location == APPLY_MANA ){
				send_to_char("�A�{�b�����A�L�k�����]�O�C\n\r", ch);
				return;
			}
		}
		if( ch->max_mana >= ch->level * mp_f) {
			send_to_char("�A���]�O�w�g�W�L�W���F�C���h���I���ŧa�C\n\r", ch);
			return;
		}
		times = 1;
		scale = ch->level * 3 * PPL_HP_MULTIPLYER;
		cost = (ch->max_mana > mp_b ) ? ((ch->max_mana -mp_b)/ scale)+1 :  1;
		cost = UMAX(1, cost);
		money = nMnFee;
		bone_flag = 1;
		pAbility = &ch->max_mana;
		pOutput = "�]�k�O";
	}
	else if ( !str_cmp( argument, "move-r" ) )
	{
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->location == APPLY_MOVE ){
				send_to_char("�A�{�b�����A�L�k���ɲ��ʤO�C\n\r", ch);
				return;
			}
		}
		if( ch->max_move >= ch->level * mv_f ) {
			send_to_char("�A�����ʤO�w�g�W�L�W���F�C���h���I���ŧa�C\n\r", ch);
			return;
		}
		times = 1;
		scale = ch->level * 3 * PPL_HP_MULTIPLYER;
		cost = (ch->max_move > mv_b) ? ((ch->max_move-mv_b) / scale)+1 : 1;
		cost = UMAX(1, cost);
		money = nMvFee;
		bone_flag = 2;
		pAbility = &ch->max_move;
		pOutput = "���ʤO";
	}
	else if ( !str_cmp( argument, "practice-r" ) )
	{
		times = 1;
		cost = 0;
		money = ch->level * ch->level * 10;
		bone_flag = 1;
		pAbility = &ch->practice;
		pOutput = "�m���I";
	}
	else
	{
		strcpy( buf, "[36m�A�i�H�V�m:" );
		if( ! IS_AFFECTED( ch, AFF_POLYMORPH)){
			if ( ch->perm_str < 13 + race_table[ ch->race ].str_mod )
				strcat( buf, " str" );
			if ( ch->perm_int < 13 + race_table[ ch->race ].int_mod )
				strcat( buf, " int" );
			if ( ch->perm_wis < 13 + race_table[ ch->race ].wis_mod )
				strcat( buf, " wis" );
			if ( ch->perm_dex < 13 + race_table[ ch->race ].dex_mod )
				strcat( buf, " dex" );
			if ( ch->perm_con < 13 + race_table[ ch->race ].con_mod )
				strcat( buf, " con" );
		}
		//20060722 modify by jye
		//�s�WEXP_PER_LEVEL ppl_class�ܼ�
		if ( ch->exp > EXP_PER_LEVEL( ch->level - 1 ,ch->class) * ( ch->level ) + 5000 )
			strcat( buf, " practice" );
		if( ch->max_hit < ( ch->level * hp_f ))
			strcat(buf, " hp");
		if( ch->max_mana < ( ch->level * mp_f ))
			strcat(buf, " mana");
		if( ch->max_move < ( ch->level * mv_f ))
			strcat(buf, " move");
		// �s�W�h�� train hp mana mv practice 2022/02/09
		if( ( ch->exp > EXP_PER_LEVEL( ch->level - 1 ,ch->class) * ( ch->level ) + 5000 )
			|| ( ch->max_hit < ( ch->level * hp_f ) )
			|| ( ch->max_mana < ( ch->level * mp_f ) )
			|| ( ch->max_move < ( ch->level * mv_f ) ) )
		{
			strcat( buf, ".\n\r" );
			send_to_char( buf, ch );
			strcpy( buf, "[36m�ɥi�઺�h�V�m:" );
			if ( ch->exp > EXP_PER_LEVEL( ch->level - 1 ,ch->class) * ( ch->level ) + 5000 )
				strcat( buf, " practice-r" );
			if( ch->max_hit < ( ch->level * hp_f ))
				strcat(buf, " hp-r");
			if( ch->max_mana < ( ch->level * mp_f ))
				strcat(buf, " mana-r");
			if( ch->max_move < ( ch->level * mv_f ))
				strcat(buf, " move-r");
			strcat( buf, ".\n\r" );
			send_to_char( buf, ch );
		}
		if ( buf[strlen( buf )-1] != ':' )
		{
			//strcat( buf, ".\n\r" );
			//send_to_char( buf, ch );
			sprintf( buf, "�V�m�@���ݩʪ��O�άO%d�T�����A\n\r", money );
			send_to_char( buf, ch );
			money   = ch->level * ch->level * 10;
			sprintf( buf, "�ͩR�O�O%d�T�����C\n\r�]�k�O�O%d�T�����C\n\r���ʤO�O%d�T�����C\n\r", nHpFee, nMnFee, nMvFee);
			send_to_char( buf, ch );
			sprintf( buf, "�m���I�O%d�T�����C[m\n\r", money );
			send_to_char( buf, ch );
		}

		return;
	}

	if ( bone_flag == 0 )
	{
		if ( !str_cmp( argument, "str" ) )
		{
		  if ( *pAbility < 13 + race_table[ ch->race ].str_mod )
			{
				ok = TRUE;
			}
		}
		else if ( !str_cmp( argument, "int" ) )
		{ 
		  if ( *pAbility < 13 + race_table[ ch->race ].int_mod )
			{
				ok = TRUE;
			}
		}
		else if ( !str_cmp( argument, "wis" ) )
		{
		  if ( *pAbility < 13 + race_table[ ch->race ].wis_mod )
			{
				ok = TRUE;
			}
		}
		else if ( !str_cmp( argument, "dex" ) )
		{
		  if ( *pAbility < 13 + race_table[ ch->race ].dex_mod )
			{
				ok = TRUE;
			}
		}
		else if ( !str_cmp( argument, "con" ) )
		{
		  if ( *pAbility < 13 + race_table[ ch->race ].con_mod )
			{
				ok = TRUE;
			}
		}

		if ( !ok )
		{
			act( "[33m�A��$T�w�g��F�W���F�C[m", ch, NULL, pOutput, TO_CHAR );
			return;
		}
	}

	// �s�W practice-r �h�� train practice �����O 2022/02/08
	if ( cost > ch->practice && str_cmp( argument, "practice" ) && str_cmp( argument, "practice-r" ) )
	{//if demote level, ppl's prac point may become minus
		send_to_char( "[33m�A���m���I�Ƥ����C[m\n\r", ch );
		return;
	}
	else if ( money > ch->gold )
	{
		send_to_char( "[33m�A���������F�C[m\n\r", ch );
		return;
	}
	//20060722 modify by jye
		//�s�WEXP_PER_LEVEL ppl_class�ܼ�
		// �s�W practice-r �h�� train practice �����O 2022/02/08
	else if ( ( !str_cmp( argument, "practice" ) || !str_cmp( argument, "practice-r" ) )
	  && ch->exp < EXP_PER_LEVEL( ch->level -1,ch->class ) * ( ch->level ) + 5000 )
	{
		send_to_char( "[33m�A���g�礣���C[m\n\r", ch );
		return;
	}
	// �s�W�@�����O�ɥi�� train �ƭ� ( �̦h 100 �� ) 2022/02/08
	if ( times > 0 )
	{
		for ( ; times <= 100; times++ ){
			if ( cost > 0 && ( cost > ch->practice ) )
				break;
			if ( cost <= 0 && ( ch->exp < EXP_PER_LEVEL( ch->level -1,ch->class ) * ( ch->level ) + 5000 ) )
				break;
			if ( money > ch->gold )
				break;

			ch->practice -= cost;
			ch->gold -= money;
			if ( cost <= 0 ){
				ch->exp -= 5000;
			}

			if ( bone_flag == 0 ){
				*pAbility += 1;
			}else if ( bone_flag == 1 ){
				if(pAbility == &ch->max_hit){ // hp
					if( ch->max_hit >= ch->level * hp_f) {
					  break;
					}else{
						*pAbility += dice( 1, 2 ) * PPL_HP_MULTIPLYER;
					}
				}
				else if (pAbility == &ch->max_mana){ // mana
					if( ch->max_mana >= ch->level * mp_f){
						break;
					}else{
						*pAbility += dice( 1, 2 ) * PPL_HP_MULTIPLYER;
					}
				}
				else{ // practice
					*pAbility += dice( 1, 2 );
				}
			}else{
				if(pAbility == &ch->max_move){ // move
					if( ch->max_move >= ch->level * mv_f ){
						break;
					}else{
						*pAbility += dice( 1, 5 ) * PPL_HP_MULTIPLYER;
					}
				}
			}
		}
	}
	else
	{
		ch->practice -= cost;
		ch->gold -= money;
		if ( cost <= 0 )
			ch->exp -= 5000;

		if ( bone_flag == 0 ){
			*pAbility += 1;
		}else if ( bone_flag == 1 ){
			if(pAbility == &ch->max_hit || pAbility == &ch->max_mana)
				*pAbility += dice( 1, 2 ) * PPL_HP_MULTIPLYER;
			else
				*pAbility += dice( 1, 2 );
		}else{
		   if(pAbility == &ch->max_move)
			   *pAbility += dice( 1, 5 ) * PPL_HP_MULTIPLYER;
		   else
			   *pAbility += dice( 1, 5 );
		}
	}
	// remove at 2020/05/31 �Y�� bug log
	//bugf("%s trained 1 %s.", ch->name, argument);
	if ( bone_flag == 0 )
	{
		// �h�� train ���ԭz 2022/02/09
		if( times > 2 ){
			act( "[36m�A��$T�j�q���ɤF�I[m", ch, NULL, pOutput, TO_CHAR );
			act( "[36m$n��$T�j�q���ɤF�I[m", ch, NULL, pOutput, TO_ROOM );
		}else{
			act( "[36m�A��$T���ɤF�I[m", ch, NULL, pOutput, TO_CHAR );
			act( "[36m$n��$T���ɤF�I[m", ch, NULL, pOutput, TO_ROOM );
		}
		return;
	}
	// �h�� train ���ԭz 2022/02/09
	if( times > 2 ){
		act( "[36m�A��$T�j�q���ɤF�I[m", ch, NULL, pOutput, TO_CHAR );
		act( "[36m$n��$T�j�q���ɤF�I[m", ch, NULL, pOutput, TO_ROOM );
	}else{
		act( "[36m�A��$T���ɤF�I[m", ch, NULL, pOutput, TO_CHAR );
		act( "[36m$n��$T���ɤF�I[m", ch, NULL, pOutput, TO_ROOM );
	}

	return;
}

void do_chameleon ( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC( ch )
  && ch->pcdata->learned[gsn_chameleon] == 0 )
    {
  send_to_char( "[33mԣ�H[m\n\r", ch );
  return;
    }

    if( !ch->in_room ) return;
    send_to_char( "[36m�A�յ۩M�|�P�����ҿĦX�C[m\n\r", ch);

    if ( IS_AFFECTED( ch, AFF_HIDE ) )
  REMOVE_BIT( ch->affected_by, AFF_HIDE );

    if ( IS_NPC( ch ) || number_percent( ) < ch->pcdata->learned[gsn_chameleon] )
  SET_BIT( ch->affected_by, AFF_HIDE );

    return;
}

void do_heighten ( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    if( !ch->in_room ) return;
    if ( !IS_NPC( ch )
  && ch->level < skill_table[gsn_heighten].skill_level[ch->class] )
    {
  send_to_char( "[33mԣ�H[m\n\r", ch );
  return;
    }

    if ( is_affected( ch, gsn_heighten ) )
  return;

    if ( IS_NPC( ch ) || number_percent( ) < ch->pcdata->learned[gsn_heighten] )
    {
  af.type      = gsn_heighten;
  af.duration  = 24;
  af.modifier  = 0;
  af.location  = APPLY_NONE;
  af.bitvector = AFF_DETECT_INVIS;
  affect_to_char( ch, &af );

  af.bitvector = AFF_DETECT_HIDDEN;
  affect_to_char( ch, &af );
  
  af.bitvector = AFF_INFRARED;
  affect_to_char( ch, &af );
  
  send_to_char( "[36m�A���Pı��O�ܱӾU�F�C[m\n\r", ch );
    }
    return;

}

void do_shadow ( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    if( !ch->in_room ) return;
    if ( !IS_NPC( ch )
  && ch->level < skill_table[gsn_shadow].skill_level[ch->class] )
    {
  send_to_char( "[33mԣ�H[m\n\r", ch );
  return;
    }

    send_to_char( "[36m�A�յۧQ�ηt�v���ʡC[m\n\r", ch );
    affect_strip( ch, gsn_shadow );

    if ( IS_NPC( ch ) || number_percent( ) < ch->pcdata->learned[gsn_shadow] )
    {
  af.type      = gsn_shadow;
  af.duration  = ch->level;
  af.modifier  = APPLY_NONE;
  af.location  = 0;
  af.bitvector = AFF_SNEAK;
  affect_to_char( ch, &af );
    }
    return;

}

/*
 * Bash code by Thelonius for EnvyMud (originally bash_door)
 * Damage modified using Morpheus's code
 * Message for bashproof doors by that wacky guy Kahn
 */
void do_bash( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *gch;
	char       arg [ MAX_INPUT_LENGTH ];
	int        door;
	int sklv = get_skill_level(ch, gsn_bash);

	if( !ch->in_room ) return;
	if ( IS_NPC( ch ) || ( !IS_NPC( ch )
		//&& ch->pcdata->learned[gsn_bash] == 0 ) )
		&& get_skill_percent(ch, gsn_bash) == 0 ))
	{
		send_to_char( "[33m�A���O�@�ӱj��i�H���}�����Ԥh�I[m\n\r", ch );
		return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' ){
		send_to_char( "[33m�n���}����H[m\n\r", ch );
		return;
	}

	if ( ch->fighting ){
		send_to_char( "[33m�A���त�_�A�������C[m\n\r", ch );
		return;
	}

	if( ch->hit * 3 < ch->max_hit ){
		send_to_char( "[33m�A�ӵ�z�F�C[m\n\r", ch );
		return;
	}

	if ( ( door = find_door( ch, arg ) ) >= 0 )
	{
		ROOM_INDEX_DATA *to_room;
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		int    chance;

		pexit = ch->in_room->exit[door];
		if ( !IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			send_to_char( "[33m�N�R�I�C���w�g�}�F�C[m\n\r", ch );
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_bash].beats );

		if ( !IS_NPC( ch ) )
			//chance = ch->pcdata->learned[gsn_bash]/2;
			chance = get_skill_percent(ch, gsn_bash) / 2 ;
		else
			chance = 0;

		if ( IS_SET( pexit->exit_info, EX_LOCKED ) )
			chance /= 2;

		if ( IS_SET( pexit->exit_info, EX_BASHPROOF ) ){
			act( "[33m�y�I�I�A�ΤO�a��$d�W���A���O���ʤ]���ʡC[m", ch, NULL, pexit->keyword, TO_CHAR );
			act( "[36m�y�I�I�I$n�ΤO�a��$d�W���A���O���̵M�z�ߤ��n�C[m", ch, NULL, pexit->keyword, TO_ROOM );
			////damage( ch, ch, ( ch->max_hit /  3 ), gsn_bash, WEAR_NONE );
			damage( ch, ch, ( ch->max_hit /  3 ), gsn_bash, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			// ���� return �������}�����̵M�|Ĳ�o guard ���H 2021/11/20
			//return;
		}else{
			if ( ( get_curr_str( ch ) + sklv*4 >= 20 )
				&& number_percent( ) < ( chance + 4 * ( get_curr_str( ch ) + sklv*4 - 20 ) ) )
			{
				/* Success */

				REMOVE_BIT( pexit->exit_info, EX_CLOSED );
				if ( IS_SET( pexit->exit_info, EX_LOCKED ) )
					REMOVE_BIT( pexit->exit_info, EX_LOCKED );

				SET_BIT( pexit->exit_info, EX_BASHED );
				// �l�[��V�P�w 2021/11/23
				//act( "[36m�A���԰աI�A��$d���}�F�I[m", ch, NULL, pexit->keyword, TO_CHAR );
				//act( "[36m$n��$d���}�F�I[m", ch, NULL, pexit->keyword, TO_ROOM );
				switch( door )
				{
					default:
						act( "�A���԰աI�A��$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"��$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
					case 0:
						act( "�A���԰աI�A��_�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"��_�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
					case 1:
						act( "�A���԰աI�A��F�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"��F�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
					case 2:
						act( "�A���԰աI�A��n�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"��n�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
					case 3:
						act( "�A���԰աI�A���誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"���誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
					case 4:
						act( "�A���԰աI�A��W�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"��W�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
					case 5:
						act( "�A���԰աI�A��U�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_CHAR );
						act( "�A���԰աI$n"NOR"��U�誺$d"NOR"���}�F�C", ch, NULL, pexit->keyword, TO_ROOM );
						break;
				}
				////damage( ch, ch, ( ch->max_hit / 20 ), gsn_bash, WEAR_NONE );
				damage( ch, ch, ( ch->max_hit /  20 ), gsn_bash, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );

				/* Bash through the other side */
				if ( ( to_room   = pexit->to_room )
					&& ( pexit_rev = to_room->exit[rev_dir[door]] )
					&& pexit_rev->to_room == ch->in_room )
				{
					CHAR_DATA *rch;

					REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
					if ( IS_SET( pexit_rev->exit_info, EX_LOCKED ) )
						REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );

					SET_BIT( pexit_rev->exit_info, EX_BASHED );

					for ( rch = to_room->people; rch; rch = rch->next_in_room )
					{
						if ( rch->deleted )
							continue;
						// �l�[��V�P�w 2021/11/23
						//act( "[36m$d�Q���}�F�I[m", rch, NULL, pexit_rev->keyword, TO_CHAR );
						switch( door )
						{
							default:
								act( "�A���԰աI$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
							case 0:
								act( "�A���԰աI�n�誺$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
							case 1:
								act( "�A���԰աI��誺$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
							case 2:
								act( "�A���԰աI�_�誺$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
							case 3:
								act( "�A���԰աI�F�誺$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
							case 4:
								act( "�A���԰աI�U�誺$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
							case 5:
								act( "�A���԰աI�W�誺$d"NOR"�Q���}�F�C", rch, NULL, pexit_rev->keyword, TO_CHAR );
								break;
						}
					}
				}
				if( ch->in_room->progtypes & ROOM_TRAP ) {
					rprog_trap_trigger(ch->in_room, ch, door+24);
				}
			}
			else
			{
				/* Failure */
			  
				act( "[33m���I�A�ΤO�a��$d�W���A���O���ʤ]���ʡC[m", ch, NULL, pexit->keyword, TO_CHAR );
				act( "[36m$n�ΤO�a��$d�W���A���O���̵M�z�ߤ��n�C[m", ch, NULL, pexit->keyword, TO_ROOM );
				////damage( ch, ch, ( ch->max_hit / 10 ), gsn_bash, WEAR_NONE );
				damage( ch, ch, ( ch->max_hit /  10 ), gsn_bash, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			}
		}
	}
	/* �ۼ���P�w 2021/11/23  */
	if ( door == -2 ){
		act( "[33m���I�I�A�o�Ʀ����ΤO�a����w�������h�C[m", ch, NULL, NULL, TO_CHAR );
		act( "[36m���I�I$n�o�Ʀ����ΤO�a����w�������h�C[m", ch, NULL, NULL, TO_ROOM );
		damage( ch, ch, ( ch->max_hit /  5 ), gsn_bash, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		return;
	}
	/* �P�w�L�S�w��V����, ���O�S���������p 2021/11/23 */
	if ( door <= -2 )
		return;
    /*
     * Check for "guards"... anyone bashing a door is considered as
     * a potential aggressor, and there's a 25% chance that mobs
     * will do unto before being done unto.
     * But first...let's make sure ch ain't dead?  That'd be a pain.
     */

	if ( ch->deleted || ch->hit <= 1 )
		return;
	//if ( !IS_SET( ch->in_room->room_flags, ROOM_SAFE ))
	if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		return;

	for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		/*
			�ק� bash door �ɪ� guard �P�w 2021/11/18
			if ( !gch->deleted
				&& gch != ch
				&& IS_AWAKE( gch )
				&& can_see( gch, ch )
				&& !gch->fighting
				&& ( IS_NPC( gch ) && !IS_AFFECTED( gch, AFF_CHARM ) && !IS_AFFECTED(gch, AFF_SUMMONED))
				&& ( ch->level - gch->level <= 4 )
				&& number_bits( 2 ) >= 2 )
				if ( !is_same_group(gch,ch))
					multi_hit( gch, ch, TYPE_UNDEFINED );
		*/
		if( gch->deleted )
			continue;

		if( !IS_NPC( gch ) )
			continue;

		if( IS_SET( gch->act, ACT_PET ) || IS_AFFECTED( gch, AFF_CHARM ) || IS_AFFECTED( gch, AFF_SUMMONED ) )
			continue;

		if( is_same_group( ch, gch ) )
			continue;
		// npc �b pc leader group ��
		if( gch->group && !IS_NPC( gch->group->leader ) )
			continue;

		if( gch->fighting )
			continue;

		if( IS_AWAKE( gch )
			&& can_see(gch , ch)
			&& IS_SET( gch->act, ACT_SENTINEL )
			&& IS_SET( gch->act, ACT_STAY_AREA )
			&& number_bits( 2 ) >= 2 )
		{
			multi_hit( gch, ch, TYPE_UNDEFINED );
		}
	}

	return;
}

/* Snare skill by Binky for EnvyMud */
void do_snare( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	AFFECT_DATA  af;
	int sklv = get_skill_level(ch, gsn_snare);
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );
	if( !ch->in_room ) return;

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	/*
	*  First, this checks for case of no second argument (valid only
	*  while fighting already).  Later, if an argument is given, it 
	*  checks validity of argument.  Unsuccessful snares flow through
	*  and receive messages at the end of the function.
	*/

	if ( arg[0] == '\0' )
	{
		if ( !( victim = ch->fighting ) )
		{
			send_to_char( "[33m�n��֧x�b�����̡H[m\n\r", ch );
			return;
		}
		/* No argument, but already fighting: valid use of snare */
		WAIT_STATE( ch, skill_table[gsn_snare].beats );

		//if ( sklv + get_curr_int( ch ) + get_curr_dex( ch ) + number_bits( 4 ) < race_table[victim->race].size * 6 )
		if ( sklv + get_curr_int( ch ) + get_curr_dex( ch ) + number_bits( 4 ) < get_curr_size( victim ) * 6 ) // �����髬 size ���� 2022/12/23
		{
			send_to_char( "�A���y���ò�F�I\n\r", ch );
			return;
		}

		/* Only appropriately skilled PCs and uncharmed mobs */
		if (  ( IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CHARM ) && number_percent()<50)
			|| (!IS_NPC( ch ) && number_percent() * 100 / weakness_percent( victim, gsn_snare) < sklv*3 + get_skill_percent( ch, gsn_snare)*4/5))
		{
			affect_strip( victim, gsn_snare );  

			af.type  = gsn_snare;
			af.duration  = 1 + ( ( ch->level ) / 8 ) + sklv;
			af.location  = APPLY_NONE;
			af.modifier  = 0;
			af.bitvector = AFF_HOLD;

			affect_to_char( victim, &af );

			act( "[36m�A��$N�x�b�����̤F�I[m", ch, NULL, victim, TO_CHAR );
			act( "[36m$n��A�x�b�����̤F�I[m",  ch, NULL, victim, TO_VICT );
			act( "[36m$n��$N�x�b�����̤F�C[m",   ch, NULL, victim, TO_NOTVICT );
		}
		else
		{
			act( "[33m����I�A�S���$N�γ����x��C"NOR, ch, NULL, victim, TO_CHAR );
			act( "[33m$n�n��A�x�b�����̡I���$m"NOR, ch, NULL, victim, TO_VICT );
			act( "[36m$n�Q�γ����x��$N�A���O�S�ন�\\�I"NOR, ch, NULL, victim, TO_NOTVICT );
		}
	}
	else        /* argument supplied */
	{
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "[33m�L���b�o��C[m\n\r", ch );
			return;
		}

		if ( !IS_NPC( ch ) && !IS_NPC( victim ) )
		{
			send_to_char( "[33m�A�����O���_�I�̧x�b�����̡C[m\n\r", ch );
			return;
		}

		if( is_same_group(ch, victim) ) {
			send_to_char("[33m�A����x��ۤv�����͡C[0m\n\r", ch);
			return;
		}

		if( IS_NPC(ch) && ( IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED)))
		{
			if( ch->master )
				check_killer(ch->master , victim);
		}

		if ( victim != ch->fighting ) /* TRUE if not fighting, or fighting  */
		{                             /* if person other than victim      */
			if ( ch->fighting )       /* TRUE if fighting other than vict.  */ 
			{
				send_to_char( "[33m���`�N�n�b�M�A�������ӤH�a�I[m\n\r", ch );
				return;
			}
			WAIT_STATE( ch, 13 - sklv*2 );

			/* here, arg supplied, ch not fighting */
			/* only appropriately skilled PCs and uncharmed mobs */
			if (    ( IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CHARM ) && number_percent()<50 )
				|| ( !IS_NPC( ch ) && number_percent() < sklv + get_skill_percent( ch, gsn_snare)*4/5))
			{
				affect_strip( victim, gsn_snare );  

				af.type      = gsn_snare;
				af.duration  = 1 + ( (ch->level ) / 8 ) + sklv*2;
				af.location  = APPLY_NONE;
				af.modifier  = 0;
				af.bitvector = AFF_HOLD;

				affect_to_char( victim, &af );

				act( "[36m�A��$M�x�b�����̤F�I[m", ch, NULL, victim, TO_CHAR );
				act( "[36m$n��A�x�b�����̤F�I[m", ch, NULL, victim, TO_VICT );
				act( "[36m$n��$N�x�b�����̤F�C[m", ch, NULL, victim, TO_NOTVICT );
			}
			else
			{
				act( "[33m����I�A�S���$M�γ����x��C[m", ch, NULL, victim, TO_CHAR );
				act( "[33m$n�n��A�x�b�����̡I���$m�I[m", ch, NULL, victim, TO_VICT );
				act( "[36m$n�Q�γ����x��$N�A���O�S�ন�\\�C[m", ch, NULL, victim, TO_NOTVICT );
			}
			if ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) && ch->master )
			{
				/* go for the one who wanted to fight :) */
				multi_hit( victim, ch->master, TYPE_UNDEFINED );
			}
			else
			{
				multi_hit( victim, ch, TYPE_UNDEFINED );
			}
		}
		else
		{
			/* we are already fighting the intended victim */
			WAIT_STATE( ch, 13 - sklv*2 );

			/* charmed mobs not allowed to do this */
			if (   ( IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CHARM )&& number_percent( )<50 )
				|| ( !IS_NPC( ch ) && number_percent() < sklv*3 + get_skill_percent( ch, gsn_snare)*4/5))
			{
				affect_strip( victim, gsn_snare );  

				af.type      = gsn_snare;
				af.duration  = 1 + ( ( ch->level ) / 8 )+sklv*2;
				af.location  = APPLY_NONE;
				af.modifier  = 0;
				af.bitvector = AFF_HOLD;

				affect_to_char( victim, &af );

				act( "[36m�A��$M�x�b�����̤F�I[m", ch, NULL, victim, TO_CHAR );
				act( "[36m$n��A�x�b�����̤F�I[m", ch, NULL, victim, TO_VICT );
				act( "[36m$n��$N�x�b�����̤F�C[m", ch, NULL, victim, TO_NOTVICT );
			}
			else
			{
				act( "[33m����I�A�S���$M�γ����x��C[m", ch, NULL, victim, TO_CHAR );
				act( "[33m$n�n��A�x�b�����̡I���$m�I[m", ch, NULL, victim, TO_VICT );
				act( "[36m$n�Q�γ����x��$N�A���O�S�ন�\\�C[m", ch, NULL, victim, TO_NOTVICT );
			}
		}
	}

	mprog_hurt_trigger(victim, ch, gsn_snare);

	return;
}



/* Untangle by Thelonius for EnvyMud */
void do_untangle( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	if( !ch->in_room ) return;

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	if ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_untangle ) == 0 )
	{
		send_to_char( "[33m�A�����F���C[m\n\r", ch );
		return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
		victim = ch;
	else if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "[33m�L���b�o��C[m\n\r", ch );
		return;
	}

	if ( !is_affected( victim, gsn_snare ) )
		return;

	if ( ( get_curr_int( ch ) + get_curr_dex( ch ) * 4 ) < number_percent( ) )
	{
		send_to_char( "�A�V��V��I\n\r", ch );
		return;
	}

	if ( ( IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CHARM ) ) || ( !IS_NPC( ch ) && number_percent() < get_skill_percent( ch, gsn_untangle)))
	{
		affect_strip( victim, gsn_snare );

		if ( victim != ch )
		{
			act( "[36m�A��$N�Ѷ}�����C[m", ch, NULL, victim, TO_CHAR );
			act( "[36m$n���A�Ѷ}�����C[m", ch, NULL, victim, TO_VICT );
			act( "[36m$n��$N�Ѷ}�����C[m", ch, NULL, victim, TO_NOTVICT );
		}
		else
		{
			send_to_char( "[36m�A���A�ۤv�Ѷ}�����C[m\n\r", ch );
			act( "[36m$n��$m�ۤv�Ѷ}�����C[m", ch, NULL, NULL, TO_ROOM );
		}

		mprog_hurt_trigger(victim, ch, gsn_untangle);

		return;
	}
}



/*
 *  Menu for all game functions.
 *  Thelonius (Monk)  5/94
 */
void do_wager( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *croupier;

    if( !ch->in_room ) return;
    if (   IS_AFFECTED( ch, AFF_MUTE )
		|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "[33m�A���L�B�b�ʡA���O�S���n���]�X�ӡC[m\n\r", ch );
		return;
	}

    /*
     *  The following searches for a valid croupier.  It allows existing
     *  ACT_GAMBLE mobs to attempt to gamble with other croupiers, but
     *  will not allow them to gamble with themselves (i.e., switched
     *  imms).  This takes care of ch == croupier in later act()'s
     */
	for( croupier = ch->in_room->people; croupier; croupier = croupier->next_in_room )
	{
		if (   IS_NPC( croupier )
			&& IS_SET( croupier->act, ACT_GAMBLE )
			&& !IS_AFFECTED( croupier, AFF_MUTE )
			&& !IS_SET( race_table[croupier->race].race_abilities, RACE_MUTE )
			&& croupier != ch )
		break;
	}

	if ( !croupier )
	{
		send_to_char( "[33m�A����b�o���աC[m\n\r", ch );
		return;
	}

    //switch( croupier->pIndexData->vnum )
    //{
//  default:
///     bug( "ACT_GAMBLE set on undefined game; vnum = %d",
//    croupier->pIndexData->vnum );
//      break;
//  case MOB_VNUM_ULT:
	game_u_l_t( ch, croupier, argument );
//      break;
  //  }

	return;
}



/*
 * Upper-Lower-Triple
 * Game idea by Partan
 * Coded by Thelonius
 */
void game_u_l_t( CHAR_DATA *ch, CHAR_DATA *croupier, char *argument )
{
	char msg  [ MAX_STRING_LENGTH ];
	char buf  [ MAX_STRING_LENGTH ];
	char limit  [ MAX_STRING_LENGTH ] = "50000";
	char wager  [ MAX_INPUT_LENGTH  ];
	char choice [ MAX_INPUT_LENGTH  ];
	int  ichoice;
	int  amount;
	int  die1;
	int  die2;
	int  die3;
	int  total;

	argument = one_argument( argument, wager );
	one_argument( argument, choice );

	if ( wager[0] == '\0' || !is_number( wager ) )
	{
		send_to_char( "[33m�A�n�U�h�ֿ��H[m\n\r", ch );
		return;
	}

	amount = atoi( wager );

	if ( amount > ch->gold )
	{
		send_to_char( "[33m�A�S������h�����I[m\n\r", ch );
		return;
	}

	if ( amount < 0 )
	{
		send_to_char( "[33m�A����Ρu����v�U�`�C[m\n\r", ch );
		return;
	}
  
	if ( amount > atoi( limit ) )
	{
		act( "[33m$n�i�D�A�G�u��p�A�o�̽�`���W���O$t�C�v[m", croupier, limit, ch, TO_VICT );
		ch->reply = croupier;
		return;
    }
/*
 *  At the moment, the winnings (and losses) do not actually go through
 *  the croupier.  They could do so, if each croupier is loaded with a 
 *  certain bankroll.  Unfortunately, they would probably be popular
 *  (and rich) targets.
 */

	if ( !str_cmp( choice, "lower"  ) ) ichoice = 1;
	else if ( !str_cmp( choice, "upper"  ) ) ichoice = 2;
	else if ( !str_cmp( choice, "triple" ) ) ichoice = 3;
	else
	{
		send_to_char( "[33m�A�n�䤰��GUpper�BLower�٬OTriple�H[m\n\r", ch );
		return;
	}
/*
 *  Now we have a wagering amount, and a choice.
 *  Let's place the bets and roll the dice, shall we?
 */
	//act( "[36m$n�M�A�U�`�C[m", ch, NULL,  croupier, TO_VICT  );
    //act( "[36m$n���F�@�D��l�C[m", ch, NULL,  croupier, TO_NOTVICT );
	act( "[36m�A��$t�T������b��W�A��u$T�v�C[m", ch, wager, choice, TO_CHAR  );
	ch->gold -= amount;

	die1 = number_range( 1, 6 );
	die2 = number_range( 1, 6 );
	die3 = number_range( 1, 6 );
	total = die1 + die2 + die3;

	sprintf( msg, "[36m$n�Y�F��l�A���G�O%d�I�B%d�I�B%d�I", die1, die2, die3 );

	if( die1 == die2 && die2 == die3 )
	{
		strcat( msg, "�C[m" );
		act( msg, croupier, NULL, ch, TO_VICT );

		if ( ichoice == 3 )
		{
			char haul [ MAX_STRING_LENGTH ];

			amount *= 19;
			sprintf( haul, "%d", amount );
			act( "[36m�OTRIPLE�I�AĹ�F$t�T�����I[m", ch, haul, NULL, TO_CHAR );
			ch->gold += amount;
		}
	else
		send_to_char( "[33m�OTRIPLE�I�A��F�I[m\n\r", ch );
		return;
	}

	sprintf( buf, "�A�`�M�O%d�I�C[m", total );
	strcat( msg, buf );
	act( msg, croupier, NULL, ch, TO_VICT );

	if( ( ( total <= 10 ) && ( ichoice == 1 ) )
	|| ( ( total >= 11 ) && ( ichoice == 2 ) ) )
	{
		char haul [ MAX_STRING_LENGTH ];

		amount *= 2;
		sprintf( haul, "%d", amount );
		act( "[36m�AĹ�F$t�T�����I[m", ch, haul, NULL, TO_CHAR );
		ch->gold += amount;
	}
	else
		send_to_char( "[33m��p�F�A�Ʊ�A�U���B��|����n�I[m\n\r", ch );

	WAIT_STATE( ch, 4);
	return;
}

/* check_asshunt function added by Amenda, 00/12/22 */
void check_asshunt( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int assd = 8, huntd = 6, faild = 16;
	// assd = 24, huntd = 12, faild = 16;

	if( !ch->in_room ) {
		bug("[check_asshunt]: No ch->in_room!", 0);
		return;
	}

	// bug ("%d", ch->in_room->vnum );
	// bug(ch->in_room->people->name, 0 );
	// bug(ch->name, 0 );
	// if (ch->hunting) bug(ch->hunting->name, 0 );
	// if ( !IS_NPC(ch) && ch->pcdata->autocombat != AUTOC_NORMAL && ch->pcdata->autocombat != AUTOC_CRITICAL )
	// return;

	// ch ��� safe room ���h���|�@ hunt �� assassinate 2022/07/11
    if( ch->in_room && IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
		return;

	// �L�k�ϥδ����� autocombat ���|Ĳ�o hunt 2022/07/14
	if ( !IS_NPC( ch )
		&& (   ch->pcdata->autocombat == AUTOC_STOP
			|| ch->pcdata->autocombat == AUTOC_DODGE
			|| ch->pcdata->autocombat == AUTOC_PARRY
			|| ch->pcdata->autocombat == AUTOC_CONCENTRATION
			|| ch->pcdata->autocombat == AUTOC_INSTANT ) )
	{
		return;
	}

	for( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( !( victim->hunting == ch || ch->hunting == victim || victim->assassinating == ch || ch->assassinating == victim ) )
			continue;

		// victim ��� safe room ���h���|�Q hunt �� assassinate 2022/07/11
		if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_SAFE) )
			return;

		/*if ch and victim assassinate/hunt each other, the one who reach the room
		earlier(victim) should assassinate/hunt firstly*/
		if ( !victim->deleted
			&& IS_AWAKE(victim)
			&& (!victim->fighting)
			&& can_see(victim, ch)
			&& !IS_AFFECTED( ch, AFF_SNEAK ) )
		{
			// autocombat �O�_���߲��m�e��P�w 2022/07/14
			//if ( victim->hunting == ch && ( IS_NPC(victim) || victim->pcdata->autocombat == AUTOC_NORMAL || victim->pcdata->autocombat == AUTOC_CRITICAL) )
			if ( victim->hunting == ch )
			{
				act( "�A�}�l�y�� $N.", victim, NULL, ch, TO_CHAR );
				act( "$n �}�l�y���A.", victim, NULL, ch, TO_VICT );
				// �קאּ TO_NOTVICT  2022/12/12
				//act( "$n �}�l�y�� $N.", victim, NULL, ch, TO_ROOM );
				act( "$n �}�l�y�� $N.", victim, NULL, ch, TO_NOTVICT );
				multi_hit( victim, ch, TYPE_UNDEFINED );

				//1060107 modify by jye ����STUN�ĪG
				//WAIT_STATE( ch, huntd );
				continue;
			}
			if ( victim->assassinating == ch )
			{
				if ( !( obj = get_eq_char( victim, WEAR_WIELD ) ) || obj->value[3] != 11 )
				{
					if ( !( obj = get_eq_char( victim, WEAR_TWO_HAND ) ) || obj->value[3] != 11 )
					{
						send_to_char( "�A�o�n�˳ƤP�����Z���~��.\n\r", ch );
						continue;
					}
				}
				act( "�A�}�l�t�� $N.", victim, NULL, ch, TO_CHAR );
				act( "$n �}�l�t���A.", victim, NULL, ch, TO_VICT );
				// �קאּ room ���S�H�|�ݨ� 2022/12/12
				//act( "$n �}�l�t�� $N.", victim, NULL, ch, TO_ROOM );
				if ( IS_NPC( victim ) || number_percent() < get_skill_percent(victim, gsn_backstab))
				{
					multi_hit( victim, ch, gsn_backstab );
					WAIT_STATE( ch, assd );
				}
				else
				{
					damage( victim, ch, 0, gsn_backstab, WEAR_WIELD, MSG_NORMAL, DMG_NORMAL );
					WAIT_STATE( victim, faild );
				}
				continue;
			}
		}

		if ( can_see(ch, victim) && IS_AWAKE(ch) )
		{
			// autocombat �O�_���߲��m�e��P�w 2022/07/14
			//if ( ch->hunting == victim && (IS_NPC(ch) || ch->pcdata->autocombat == AUTOC_NORMAL || ch->pcdata->autocombat == AUTOC_CRITICAL || ch->pcdata->autocombat == AUTOC_CHARGE))
			if ( ch->hunting == victim )
			{
				// �[�J hunt charge stun �P�w�ζi�J�԰��Ĥ@�^�XĲ�o mob fight_prog 2022/12/07
				int charge_stun = 0;
				int vch_prog = 0;
				act( "�A�}�l�y�� $N.", ch, NULL, victim, TO_CHAR );
				act( "$n �}�l�y���A.", ch, NULL, victim, TO_VICT );
				// �קאּ TO_NOTVICT  2022/12/12
				//act( "$n �}�l�y�� $N.", ch, NULL, victim, TO_ROOM );
				act( "$n �}�l�y�� $N.", ch, NULL, victim, TO_NOTVICT );
				if(!IS_NPC(ch)){
					if(ch->pcdata->autocombat == AUTOC_CHARGE){
						// hunt charge ���߫h�����]�w�}�ԥؼ� 2022/12/07
						if( victim->position > POS_INCAP )
						{
							if( !victim->fighting ){
								set_fighting( victim, ch );
							}
							if( IS_NPC(victim) ) vch_prog = 1;
						}
						if( !ch->fighting )
						{
							set_fighting( ch, victim );
							ch->position = POS_FIGHTING;
						}
						do_charge(ch, "ultra");
						charge_stun = ch->wait; // �O�U ch stun �ƭ�
						ch->wait = 0; // charge �� stun �� 0
						charge_stun = UMAX( 3 , charge_stun/2 ); // hunt charge �����C�� stun
					}
				}
				multi_hit( ch, victim, TYPE_UNDEFINED );
				ch->wait += charge_stun; // multi hit ������[�^ stun
				if( vch_prog == 1 ) mprog_fight_trigger( victim , ch); // npc �Q�}�ԫhĲ�o fight_prog
				//WAIT_STATE( victim, huntd );   1060107 modify by jye ����STUN�ĪG
				continue;
			}
			if ( ch->assassinating == victim )
			{
				if ( !( obj = get_eq_char( ch, WEAR_WIELD ) ) || obj->value[3] != 11 )
				{
					if ( !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) || obj->value[3] != 11 )
					{
						send_to_char( "�A�o�n�˳ƤP�����Z��.\n\r", ch );
						continue;
					}
				}
				if ( victim->fighting )
				{
					send_to_char( "�A���ఽŧ�԰������ؼ�.\n\r", ch );
					continue;
				}
				act( "�A�}�l�t�� $N.", ch, NULL, victim, TO_CHAR );
				act( "$n �}�l�t���A.", ch, NULL, victim, TO_VICT );
				// �t�����|�Q�H�ݨ� 2022/12/12
				//act( "$n �}�l�t�� $N.", ch, NULL, victim, TO_ROOM );
				if ( IS_NPC( ch ) || !ch->pcdata || ( number_percent( ) < get_skill_percent(ch, gsn_backstab)))//ch->pcdata->learned[gsn_backstab] ))
				{
					multi_hit( ch, victim, gsn_backstab );
					WAIT_STATE( victim, assd );
				}
				else
				{
					damage( ch, victim, 0, gsn_backstab, WEAR_WIELD, MSG_NORMAL, DMG_NORMAL );
					WAIT_STATE( ch, faild );
				}
				continue;
			}
		}
	}
	return;
}

void do_ride( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *horse;
	//AFFECT_DATA af;
	OBJ_DATA *pObj;
	char buf [ BUF_STRING_LENGTH * 4];
	char arg  [ MAX_INPUT_LENGTH ];
	AFFECT_DATA *paf = NULL; // 2022/12/23
	int horse_size = 0;  // 2022/12/23

	//test by coconet for ride modified
	pObj = get_eq_char( ch , WEAR_RIDE );
	if ( pObj )
	{
		OBJ_DATA *qObj, *obj_next;

		if ( !pObj->pIndexData->horse )
		{
			bug("Ride_horse: Horse ride no horse vnum %d", pObj->pIndexData->vnum);
			return;
		}

		unequip_char( ch , pObj );
		obj_from_char( pObj);

		if (  get_mob_index(pObj->pIndexData->horse)  == NULL )
		{
			bug( "Ride - Bad mob vnum: vnum %d.", pObj->pIndexData->vnum );
			return;
		}

		horse = create_mobile( get_mob_index(pObj->pIndexData->horse) );


		for( qObj = pObj->contains; qObj ; qObj = obj_next )
		{
			obj_next = qObj->next_content;
			obj_from_obj( qObj );
			obj_to_char( qObj, horse );
		}   

		char_to_room( horse, ch->in_room);
		SET_BIT( horse->act, ACT_HORSE );
		SET_BIT( horse->act, ACT_SENTINEL );
		add_follower( horse, ch );
		add_rember( horse , ch );

		extract_obj( pObj );
		send_to_char( "�A�@½�����U�F���M�C\n\r", ch );
		act( "$n�t�x�a½�����U$N�C", ch, NULL, horse, TO_ROOM );

		return; 
	}

	argument = one_argument( argument, arg ); 

	if ( arg[0] != '\0' ) 
	{

		OBJ_DATA *qObj, *obj_next;
		OBJ_INDEX_DATA *tmp;
		int sklv;
		int temp = 1,i;

		if ( get_skill_percent( ch,gsn_ride ) == 0 && !IS_IMMORTAL(ch) && (!IS_NPC(ch) && !(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum))))
		{
			send_to_char( "�A�S���|�M���C\n\r",ch);
			return;
		}

		//add by coconet 0524 for avoid crashing
		if ( IS_SET( ch->act, PLR_HORSE ) || IS_NPC(ch) || ch->pcdata->switched == TRUE )
			return;

		if ( arg[0] == '\0' )
		{
			send_to_char( "�A�n�M���ǰ����H�I\n\r",ch);
			return;
		}

		horse = get_char_room( ch, arg );

		if ( !horse || !IS_SET( horse->act, ACT_HORSE ) )
		{
			send_to_char( "�o�̨S���o�ǧ��M.\n\r", ch );
			return;
		}

		if ( !horse->pIndexData )
			return; 
		if ( !horse->pIndexData->replica )
		{
			bug( "Ride_Horse: Horse ride no replica vnum %d",horse->pIndexData->vnum);
			return;
		}

		if ( horse->rembch && horse->rembch != ch )
		{
			send_to_char("�O���M�O�H���y�M�I\n\r",ch);
			return;
		}

		for( pObj = ch->carrying ; pObj ; pObj = pObj->next_content )
			if( pObj->wear_loc == WEAR_RIDE )
				return;

		tmp = get_obj_index( horse->pIndexData->replica );
		qObj = create_object( tmp, horse->level );
		sklv = get_skill_level( ch, gsn_ride );

		for( i = sklv ; i > 1 ; i-- )
			temp = temp*2;

		if ( !(qObj->value[1] & 4096) && ( sklv == 0 || !(qObj->value[1] & temp) )   )
		{
			sprintf( buf, "check ride %d",temp );
			log_string(buf);
			send_to_char("�L�קA���X�O�A���L�k���A�o�姤�M�I\n\r",ch);
			extract_obj( qObj );
			ch->move -= 50;
			return;
		}

		for( pObj = horse->carrying; pObj ; pObj = obj_next )
		{
			obj_next = pObj->next_content;
			// bug("test for horse  %s ", pObj->name );
			// if ( obj_next )
			//		bug("test for horse2 %s ", obj_next->name);
			obj_from_char( pObj );

			if ( get_obj_weight( pObj ) + get_obj_weight( qObj ) - qObj->weight > qObj->value[0] || get_obj_number( qObj ) >= qObj->value[1] )
				obj_to_room( pObj , ch->in_room );
			else
				obj_to_obj( pObj, qObj );
		}

		if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
			send_to_char("�A�U�b���w�g�O���F�A�ٷQ�A�M�@�ǡH�I\n\r", ch);
			return;
		}    

		// �M���髬�W�[ 2022/12/23
		horse_size = get_curr_size( horse );

		if( qObj->value[2] == 0 )
		{
			paf = new_affect ();
			paf->type = qObj->pIndexData->vnum;
			paf->duration = -1;
			paf->location = APPLY_HEIGHT;
			paf->modifier = horse_size;
			paf->bitvector = 0;
			paf->next = qObj->affected;
			qObj->affected = paf;
		}

		obj_to_char( qObj, ch );        
		equip_char( ch, qObj, WEAR_RIDE );
		send_to_char( "�A�@½�����W�F���M�C\n\r", ch );
		act( "$n�@�a�����W$N", ch, NULL, horse, TO_ROOM );
		extract_char( horse, TRUE );
		return;

	}
	else
	{
		//add by coconet to let ppl when quit will auto ride
		OBJ_DATA *qObj, *obj_next;
		OBJ_INDEX_DATA *tmp;
		int sklv;
		int temp = 1,i;

		if ( get_skill_percent( ch,gsn_ride ) == 0 && !IS_IMMORTAL(ch) )
		{
			return;
		}

		for ( horse = ch->in_room->people ; horse ; horse = horse->next_in_room )
		{
			if( IS_NPC(horse) && IS_SET( horse->act, ACT_HORSE ) && horse->rembch && horse->rembch == ch  )
				break;
		}

		if ( !horse || !horse->pIndexData )
			return; 

		if ( !horse->pIndexData->replica )
		{
			bug( "Ride_Horse: Horse ride no replica vnum %d",horse->pIndexData->vnum);
			return;
		}

		for( pObj = ch->carrying ; pObj ; pObj = pObj->next_content )
			if( pObj->wear_loc == WEAR_RIDE )
				return;

		tmp = get_obj_index( horse->pIndexData->replica );
		qObj = create_object( tmp, horse->level );
		sklv = get_skill_level( ch, gsn_ride );

		for( i = sklv ; i > 1 ; i-- )
			temp = temp*2;

		if ( !(qObj->value[1] & 4096) && ( sklv == 0 || !(qObj->value[1] & temp) )   )
		{
			extract_obj( qObj );
			return;
		}

		for( pObj = horse->carrying; pObj ; pObj = obj_next )
		{
			obj_next = pObj->next_content;
			obj_from_char( pObj );

			if ( get_obj_weight( pObj ) + get_obj_weight( qObj ) - qObj->weight > qObj->value[0] || get_obj_number( qObj ) >= qObj->value[1] )
			{
				obj_to_room( pObj , ch->in_room );
			}
			else
				obj_to_obj( pObj, qObj );
		}

		if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
			send_to_char("�A�U�b���w�g�O���F�A�ٷQ�A�M�@�ǡH�I\n\r", ch);
			return;
		}

		// �M���髬�W�[ 2022/12/23
		horse_size = get_curr_size( horse );

		if( qObj->value[2] == 0 )
		{
			paf = new_affect ();
			paf->type = qObj->pIndexData->vnum;
			paf->duration = -1;
			paf->location = APPLY_HEIGHT;
			paf->modifier = horse_size;
			paf->bitvector = 0;
			paf->next = qObj->affected;
			qObj->affected = paf;
		}

		obj_to_char( qObj, ch );        
		equip_char( ch, qObj, WEAR_RIDE );
		send_to_char( "�A�@½�����W�F���M�C\n\r", ch );
		act( "$n�@�a�����W$N", ch, NULL, horse, TO_ROOM );
		extract_char( horse, TRUE );
		return;
	}
}



void do_autocombat( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];

	if( IS_NPC( ch ) ) return;

	one_argument( argument, arg);

	if( *arg == '\0' ) {
		strcpy( arg, "normal" );
	}

	if( !str_cmp( arg, "normal" )){
		ch->pcdata->autocombat = AUTOC_NORMAL;
	}else if ( !str_cmp( arg, "stop") ){
		ch->pcdata->autocombat = AUTOC_STOP;
	}else if ( !str_cmp( arg, "dodge" )){
		//if( ch->pcdata->learned[gsn_autododge] < 1 ) {
		if( get_skill_percent(ch, gsn_autododge) < 1 ) {
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_DODGE;
	}else if (!str_cmp(arg, "parry")){
		//if( ch->pcdata->learned[gsn_autoparry] < 1 ) {
		if( get_skill_percent(ch, gsn_autoparry) < 1 ) {
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_PARRY;
	}else if (!str_cmp(arg, "critical")){
		//if( ch->pcdata->learned[gsn_deadly_strike] < 1 ) {
		if( get_skill_percent(ch, gsn_deadly_strike) < 1 ) {
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_CRITICAL;
	}else if ( !str_cmp(arg, "cast")) {
		//if( ch->pcdata->learned[gsn_concentration] < 1 ) {
		if( get_skill_percent(ch, gsn_concentration) < 1 ) {
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_CONCENTRATION;
	}else if (!str_cmp(arg, "coop")){
		ch->pcdata->autocombat = AUTOC_COOPERATE;
	}else if ( !str_cmp(arg, "power")){
		if( get_skill_percent(ch, gsn_abstraction) < 1 ) {
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_POWER;
	}else if ( !str_cmp(arg, "instant")){
		if( get_skill_percent(ch, gsn_instant_cast) < 1 ){
			send_to_char("�A���I�k�t���٤��������C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_INSTANT;
	}else if ( !str_cmp(arg, "charge")){
		if( get_skill_percent(ch, gsn_charge) < 1 ){
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		if ( !get_eq_char( ch , WEAR_RIDE ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) )
		{
			send_to_char( "�A�����M�b���M�W���C\n\r",ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_CHARGE;
	}else if ( !str_cmp(arg, "reload")){  // �ֳt��u 2022/05/01
		if( get_skill_percent(ch, gsn_firearm_mastery) < 1 ) {
			send_to_char("�A���|�o�˧ޯ�C\n\r", ch);
			return;
		}
		ch->pcdata->autocombat = AUTOC_RELOAD;
	}else{
		send_to_char("����H\n\r", ch);
		return;
	}

	send_to_char("�S���D.\n\r", ch);
	return;
}



void do_hsummon( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA     *horse;
    int sklv = get_skill_level(ch, gsn_ride);


    if( !ch->in_room ) return;
    if ( ( /*ch->pcdata->learned[gsn_ride]*/ get_skill_percent(ch, gsn_ride) == 0 ) 
            || !ch->pcdata->horse || !ch->desc )
    {
        send_to_char( "�A�S�����C\n\r", ch );
        return;
    }
    if( IS_SET(ch->act, PLR_HORSE)){
        send_to_char("������l��D�H�C\n\r", ch);
        return;
    }

    if ( ch->in_room == ( horse = ch->pcdata->horse )->in_room )
    {
        send_to_char( "�e�w�g�b�o��F�C\n\r", ch );
        return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
        return; 


    if ( !horse->in_room )
    {
        send_to_char( "�����b�o�ӥ@�ɤW�C\n\r", ch );
        return;
    }

    
    if( sklv < 6 && horse->in_room->area != ch->in_room->area ) {
        send_to_char("�Z���ӻ��A�eť����A���l��C\n\r", ch);
        return;
    }
    else if( sklv > 5 && horse->in_room->area->recall != ch->in_room->area->recall ){
        send_to_char("�Z���ӻ��A�eť����A���l��C\n\r", ch);
        return;
    }

    if( ch->fighting || horse->fighting ) {
        send_to_char("�b�԰����ɭԤ���I\n\r", ch);
        return;
    }
    if( horse->position < POS_STANDING || horse->move < 1) {
        send_to_char("�e�εۤF�άO�L�k���ʡC\n\r", ch);
        return;
    }


    act( "$n�I���D�H���l��ӥh.", horse, NULL, NULL,   TO_ROOM );
    char_from_room( horse );
    char_to_room( horse, ch->in_room);
    act( "$n���D�H���l��Ө�.",   horse, NULL, NULL,   TO_ROOM );
    act( "$N�ӤF.",   ch, NULL, horse,   TO_CHAR);
    horse->move /= 2;
    horse->position = POS_STANDING;
}

// ��U�ؼ� 2021/11/20
void do_cooperate( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		if( !ch->cooperating ) {
			send_to_char( "�n��U�֡H\n\r", ch );
		}
		else
		{
			act( "�A���b��U����H�O$N�C", ch, NULL, ch->cooperating, TO_CHAR );
		}
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		return;
	}

	if( ch->cooperating ){
		if( victim != ch->cooperating ) {
			stop_cooperating(ch, ch->cooperating);
		}
		else
		{
			act( "�A�w�g�b��U$N�F�C", ch, NULL, ch->cooperating, TO_CHAR );
			return;
		}
	}

	if( victim == ch ){
		return;
    }

	if( ( !is_same_group( ch, victim))) {
		send_to_char("�A�F���o��h�ơI\n\r", ch);
		return;
	}

    add_cooperator(ch, victim);
}

void do_adv( CHAR_DATA *ch, char *argument )
{
}
