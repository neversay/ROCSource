/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	       *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *  The MOBprograms have been contributed by N'Atas-ha.	 Any support for   *
 *  these routines should not be expected from Merc Industries.	 However,  *
 *  under no circumstances should the blame for bugs, etc be placed on	   *
 *  Merc Industries.  They are not guaranteed to work on all systems due   *
 *  to their frequent use of strxxx functions.	They are also not the most *
 *  efficient way to perform their tasks, but hopefully should be in the   *
 *  easiest possible way to install and begin using. Documentation for	   *
 *  such installation can be found in INSTALL.	Enjoy........	 N'Atas-Ha *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : mob_commands.c                                 *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940617 Razgriz                                *
//*  PURPOSE     :         ­×¥¿ do_skilldrv() ¤¤ heavyhit         *
//*              :         ¥i¯à¾É­P ac µL­­¼W¥[ªº bug             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940626 Razgriz                                *
//*  PURPOSE     :         ·s¼W do_mpmset() «ü¥O                  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940701 Razgriz                                *
//*  PURPOSE     :         §â check_qmark() «Å§i©ñ¨ì merc.h ¥h¡A  *
//*              :         ¥HÁ×§K make ®É·|²£¥Í¤@­Ó               *
//*              :         implicit declaration ªº warning        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940723 Jye                                    *
//*  PURPOSE     :         ­×§ï do_mpoload ³Ì°ªµ¥¯Å½Õ¬°41         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940914 Jye                                    *
//*  PURPOSE     :         ·s¼W do_mpget mob_commands             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940915 Razgriz                                *
//*  PURPOSE     :         ·s¼W do_mpset_obj_affect               *
//*              :         ·s¼W do_mpremove_obj_affect            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         ·s¼W do_mpmodify_obj_affect            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         ­×§ï do_mpget ¯S®íª««~¤£®³             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         ­×§ï§PÂ_souvenir­È¨Ï§ï¥ÎÅÜ¼Æ           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941005 Razgriz                                *
//*  PURPOSE     :         ­×§ï do_mpalign ¥[¤W¸Ë³Æ°}Àç§PÂ_       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941010 Razgriz                                *
//*  PURPOSE     :         ·s¼W do_mpforceo                       *
//*****************************************************************
/* $Id: mob_commands.c 3437 2005-10-14 15:55:00Z akira $ */
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "ansi.h"

extern int one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
//extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode );
/*
 * Local functions.
 */

char *	mprog_type_to_name	args( ( int type ) );
/*bool 	check_qmark        args( ( CHAR_DATA *target, char *arg ) );*/		//Modified by Razgriz	20050701
//int qmark_opr args( ( int lhs, char *opr, int rhs ) );
int qmark_opr args( ( int lhs, char *opr, int rhs, char *mod_name ) );
long long ll_qmark_opr args( ( long long lhs, char *opr, long long rhs, char *mod_name ) ); // ¼W¥[¼Æ­È¸û¤jªº§P©w 2023/01/10

char *	rprog_type_to_name	args( ( int type ) );
void check_position	args( (CHAR_DATA *ch, CHAR_DATA *victim, int dam) );

/* This routine transfers between alpha and numeric forms of the
 *  mob_prog bitvector types. It allows the words to show up in mpstat to
 *  make it just a hair bit easier to see what a mob should be doing.
 */

void do_self_skill( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *target, *next_target;
	char arg0[ MAX_INPUT_LENGTH ];
	char skill_name[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	int  times;
	int  min_times;
	int  max_times;
	int  min_dam;
	int  max_dam;
	int  miss_check;
	int  miss_result;
	int  dam_check;
	int  dam_result;
	int  ac;
	int  ac_effect;
	int  pdr;
	int  pdr_effect;
	int  sanc;
	int  type;
	//int  dt;
	int  dam = 0;
	int  total_dam = 0;
	int  miss_times = 0;
	int  no_dam_times = 0;
	int  dam_times = 0;
	bool miss = FALSE;
	int  i;

    if( !IS_NPC(ch) ){
	send_to_char("What?\n\r", ch);
	return;
    }
	argument = one_argument( argument, arg0 );
	argument = one_argument( argument, skill_name );
	if ( arg0[0] == '\0' || skill_name[0] == '\0' || argument[0] == '\0' )
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		type = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	max_times = type % 100;
	min_times = ( type % 100000 ) / 1000;
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		min_dam = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		max_dam = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		type = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	dam_result = type % 100;
	dam_check = ( type % 1000 ) / 100;
	miss_result = ( type % 100000 ) / 1000;
	miss_check = ( type % 1000000 ) / 100000;

	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		type = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	sanc = type % 10;
	pdr_effect = ( type % 10000 ) / 10;
	pdr = ( type % 100000 ) / 10000;
	ac_effect = ( type % 100000000 ) / 100000;
	ac = ( type % 1000000000 ) / 100000000;

	//abcd efgh i
	times = number_range( min_times, max_times );
	//bug ( "'selfskill' min_times: %s", min_times );
	//bug ( "'selfskill' max_times: %d", max_times );
	//bug ( "'selfskill' times: %d", times );
	/*
	bug ( "'selfskill' show_skill: %s", skill_name );
	bug ( "'selfskill' show_times: %d", times );
	bug ( "'selfskill' show_min_dam: %d", min_dam );
	bug ( "'selfskill' show_max_dam: %d", max_dam );
	bug ( "'selfskill' show_miss_check: %d", miss_check );
	bug ( "'selfskill' show_miss_result: %d", miss_result );
	bug ( "'selfskill' show_dam_check: %d", dam_check );
	bug ( "'selfskill' show_dam_result: %d", dam_result );
	bug ( "'selfskill' show_ac: %d", ac );
	bug ( "'selfskill' show_ac_effect: %d", ac_effect );
	bug ( "'selfskill' show_pdr: %d", pdr );
	bug ( "'selfskill' show_pdr_effect: %d", pdr_effect );
	bug ( "'selfskill' show_sanc: %d", sanc );*/

	if( !str_cmp( arg0, "all" ) )
	{
		bug ( "Mob: %d 'selfskill' target is all", ch->pIndexData->vnum );
		for ( target = ch->in_room->people; target; target = next_target )
		{
			next_target = target->next_in_room;
			if( target == ch ) continue;
			if( IS_NPC( target ) && target->fighting != ch ) continue;
			
			if ( miss_check == 2 )
			{
				if( number_range( 1, 100 ) < miss_result )
				{
					miss_times += 1;
					miss = TRUE;
				}
			}
			if( !miss )
			{
				dam = number_range( min_dam, max_dam );
				if ( dam_check == 2 )
				{
					if( number_range( 1, 100 ) < dam_result )
					{
						no_dam_times += 1;
					}
					else
					{
						if( ac == 2 )
							dam = dam * ( 1 + ( ( target->armor /ac_effect ) * 10 * 0.01 ) );
						if( pdr == 2 )
							dam = dam - target->pdr + pdr_effect + ch->apdr;
						if( sanc == 2 )
							dam = dam/2;
						if( dam <= 0 )
							no_dam_times += 1;
						else
							dam_times += 1;
					}
				}
			}
			/*
			bug ( "Mob: %d 'selfskill' before self_damage", ch->pIndexData->vnum );
			bug ( "'selfskill' show_dam: %d", dam );
			bug ( "'selfskill' show_miss_times: %d", miss_times );
			bug ( "'selfskill' show_no_dam_times: %d", no_dam_times );
			bug ( "'selfskill' show_dam_times: %d", dam_times );*/
			target->hit = target->hit - dam;
			self_damage( ch, target, miss_times, no_dam_times, dam_times, 1, dam, skill_name );
			check_position( ch, target, dam );
			if( target->position == POS_DEAD) raw_kill(ch, target);
		}
	}
	else
	{
		target = get_char_room(ch, arg0);
		if( !target ) 
		{
			bug("selfskill : target does not in room", 0);
			return;
		}
		for ( i = 0; i < times; i++ )
		{
			if ( miss_check == 2 )
			{
				if( number_range( 1, 100 ) < miss_result )
				{
					miss_times += 1;
					miss = TRUE;
				}
			}
			if( !miss )
			{
				dam = number_range( min_dam, max_dam );
				if ( dam_check == 2 && !miss )
				{
					if( number_range( 1, 100 ) < dam_result )
					{
						no_dam_times += 1;
					}
					else
					{
						if( ac == 2 )
							dam = dam * ( 1 + ( ( target->armor /ac_effect ) * 10 * 0.01 ) );
						if( pdr == 2 )
							dam = dam - target->pdr + pdr_effect + ch->apdr;
						if( sanc == 2 )
							dam = dam/2;
						if( dam <= 0 )
							no_dam_times += 1;
						else
							dam_times += 1;
					}
				}
				//bug ( "'selfskill' show_dam: %d", dam );
				total_dam += dam;
			}
			miss = FALSE;
		}
		//bug ( "'selfskill' show_total: %d", total_dam );
		target->hit = target->hit - total_dam;
		self_damage( ch, target, miss_times, no_dam_times, dam_times, times, total_dam, skill_name );
		check_position( ch, target, total_dam );
		if( target->position == POS_DEAD) raw_kill(ch, target);
	}
	return;
}

void do_self_magic( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *target, *next_target;
	char arg0[ MAX_INPUT_LENGTH ];
	char magic_name[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	int  times;
	int  min_times;
	int  max_times;
	int  min_dam;
	int  max_dam;
	int  fail_check;
	int  fail_chance;
	int  null_check;
	int  null_damage;
	int  svs_check;
	int  svs_type;
	int  svs_factor;
	int  mdr_check;
	int  mdr_factor;
	int  sanc;
	int  type;
	int  multi_times;
	int  dam = 0;
	int  total_dam = 0;
	int  miss_times = 0;
	int  no_dam_times = 0;
	int  dam_times = 0;
	//bool miss = FALSE;
	int  i;

	argument = one_argument( argument, arg0 );
	argument = one_argument( argument, magic_name );
	if ( arg0[0] == '\0' || magic_name[0] == '\0' || argument[0] == '\0' )
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		type = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	max_times = type % 100;
	min_times = ( type % 100000 ) / 1000;
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		min_dam = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		max_dam = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		type = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	null_damage = type % 100;
	null_check = ( type % 1000 ) / 100;
	fail_chance = ( type % 100000 ) / 1000;
	fail_check = ( type % 1000000 ) / 100000;

	argument = one_argument( argument, arg2 );
	if( ( is_number( arg2 ) ) )
		type = atoi( arg2 );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	sanc = type % 10;
	mdr_factor = ( type % 10000 ) / 10;
	mdr_check = ( type % 100000 ) / 10000;
	svs_factor = ( type % 10000000 ) / 100000;
	svs_type = ( type % 1000000000 ) / 10000000;
	svs_check = (( type / 1000000000 ) % 10);


	if( ( is_number( argument ) ) )
		multi_times = atoi( argument );
	else
	{
		bug ( "Mob: %d 'selfskill' bad syntax", ch->pIndexData->vnum );
		return;
	}
	//abcd efgh i
	times = number_range( min_times, max_times );
	//bug ( "'selfskill' min_times: %s", min_times );
	//bug ( "'selfskill' max_times: %d", max_times );
	//bug ( "'selfskill' times: %d", times );
	/*
	bug ( "'selfskill' show_skill: %s", skill_name );
	bug ( "'selfskill' show_times: %d", times );
	bug ( "'selfskill' show_min_dam: %d", min_dam );
	bug ( "'selfskill' show_max_dam: %d", max_dam );
	bug ( "'selfskill' show_miss_check: %d", miss_check );
	bug ( "'selfskill' show_miss_result: %d", miss_result );
	bug ( "'selfskill' show_dam_check: %d", dam_check );
	bug ( "'selfskill' show_dam_result: %d", dam_result );
	bug ( "'selfskill' show_ac: %d", ac );
	bug ( "'selfskill' show_ac_effect: %d", ac_effect );
	bug ( "'selfskill' show_pdr: %d", pdr );
	bug ( "'selfskill' show_pdr_effect: %d", pdr_effect );
	bug ( "'selfskill' show_sanc: %d", sanc );*/

	if( !str_cmp( arg0, "all" ) )
	{
		bug ( "Mob: %d 'selfskill' target is all", ch->pIndexData->vnum );
		for ( target = ch->in_room->people; target; target = next_target )
		{
			next_target = target->next_in_room;
			if( target == ch ) continue;
			if( IS_NPC( target ) && target->fighting != ch ) continue;
			
			if ( fail_check == 2 )
			{
				if( number_range( 1, 100 ) < fail_chance )
				{
					break;
				}
			}
			dam = number_range( min_dam, max_dam );
			if ( null_check == 2 )
			{
				if( number_range( 1, 100 ) < null_damage )
				{
					no_dam_times += 1;
				}
				else
				{
					if( svs_check == 2 )
						dam = dam * ( 1 + ( ( NEW_GET_SVS( ch, svs_type ) / svs_factor ) * 10 ) );
					if( mdr_check == 2 )
						dam = dam - target->mdr + mdr_factor + ch->amdr;
					if( sanc == 2 )
						dam = dam/2;
					if( dam <= 0 )
						no_dam_times += 1;
					else
						dam_times += 1;
				}
			}
			/*
			bug ( "Mob: %d 'selfskill' before self_damage", ch->pIndexData->vnum );
			bug ( "'selfskill' show_dam: %d", dam );
			bug ( "'selfskill' show_miss_times: %d", miss_times );
			bug ( "'selfskill' show_no_dam_times: %d", no_dam_times );
			bug ( "'selfskill' show_dam_times: %d", dam_times );*/
			target->hit = target->hit - dam;
			self_damage( ch, target, miss_times, no_dam_times, dam_times, 1, dam, magic_name );
			check_position( ch, target, dam );
			if( target->position == POS_DEAD) raw_kill(ch, target);
		}
	}
	else
	{
		target = get_char_room(ch, arg0);
		if( !target ) 
		{
			bug("selfskill : target does not in room", 0);
			return;
		}
		for ( i = 0; i < times; i++ )
		{
			if ( fail_check == 2 )
			{
				if( number_range( 1, 100 ) < fail_chance )
				{
					break;
				}
			}
			dam = number_range( min_dam, max_dam );
			if ( null_check == 2 )
			{
				if( number_range( 1, 100 ) < null_damage )
				{
					no_dam_times += 1;
				}
				else
				{
					if( svs_check == 2 )
						dam = dam * ( 1 + ( ( NEW_GET_SVS( ch, svs_type ) / svs_factor ) * 10 ) );
					if( mdr_check == 2 )
						dam = dam - target->mdr + mdr_factor + ch->amdr;
					if( sanc == 2 )
						dam = dam/2;
					if( dam <= 0 )
						no_dam_times += 1;
					else
						dam_times += 1;
				}
			}
			bug ( "'selfskill' show_dam: %d", dam );
			total_dam += dam;
		}
		bug ( "'selfskill' show_total: %d", total_dam );
		target->hit = target->hit - total_dam;
		self_damage( ch, target, miss_times, no_dam_times, dam_times, times, total_dam, magic_name );
		check_position( ch, target, total_dam );
		if( target->position == POS_DEAD) raw_kill(ch, target);
	}
	return;
}

void do_mpstun( CHAR_DATA *ch, char *argument )
{
	//char arg[MAX_INPUT_LENGTH];

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
}

void do_mpteleport( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *pRoomIndex = NULL;
	//int room1, room2, count = 0;
	//const int times = 100;
	int room1 ,room2;
	char arg1[MAX_INPUT_LENGTH];

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
    }

	argument = one_argument( argument, arg1);

	if( arg1[0] == '\0' )
	{
		bug("[do_mpteleport] : missing location", 0);
		return;
	}

	if( !str_cmp( arg1, "all" ) )
	{
		while( !pRoomIndex )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
		}
	}
	else if( !str_cmp( arg1, "noclan" ) )
	{
		while( !pRoomIndex )		
		// while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if(    pRoomIndex
			    && pRoomIndex->area
			    && IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) )
				pRoomIndex = NULL;
			//count++;
		}
	}
	else if( !str_cmp( arg1, "recall1" ) )
	{
		while( !pRoomIndex )
		// while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if(    pRoomIndex
			    && pRoomIndex->area
			    && ( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) || (pRoomIndex->area->recall != 3001 ) ) )
				pRoomIndex = NULL;
			//count++;
		}
	}
	else if( !str_cmp( arg1, "recall2" ) )
	{
		while( !pRoomIndex )
		// while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if(    pRoomIndex
			    && pRoomIndex->area
			    && ( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) || pRoomIndex->area->recall != 4580 ) )
				pRoomIndex = NULL;
			//count++;
		}
	}
	else if( !str_cmp( arg1, "recall3" ) )
	{
		while( !pRoomIndex )
		// while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if(    pRoomIndex
			    && pRoomIndex->area
			    && ( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) || pRoomIndex->area->recall != 14229 ) )
				pRoomIndex = NULL;
			//count++;
		}
	}
	else if( !str_cmp( arg1, "area" ) )  // ·s¼W·í«e°Ï°ì¤º mpteleport 2022/10/06
	{
		while( !pRoomIndex )
		{
			pRoomIndex = get_room_index( number_range( ch->in_room->area->lvnum, ch->in_room->area->uvnum ) );
		}
	}
	else if( !str_cmp( arg1, "vnum" ) )
	{
		argument = one_argument( argument, arg1);
		if( argument[0] == '\0' || arg1[0] == '\0' )
		{
			bug("[do_mpteleport] : missing location range", 0);
			return;
		}
		if( ( is_number( arg1 ) ) )
			room1 = atoi(arg1);
		else
		{
			bug ( "Mob: %d 'mpteleport' incorrect room number", ch->pIndexData->vnum );
			return;
		}
		if( ( is_number( argument ) ) )
			room2 = atoi(argument);
		else
		{
			bug ( "Mob: %d 'mpteleport' incorrect room number", ch->pIndexData->vnum );
			return;
		}
		if ( room1 > room2 )
		{
			bug ( "Mob: %d 'mpteleport' value 1 need to be smaller than value 2", ch->pIndexData->vnum );
			return;
		}

		while( !pRoomIndex )
		// while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( room1, room2  ) );
			//count++;
		}
	}
	else if( !str_cmp( arg1, "self" ) )  // ¹ï¦Û¤v·í«eªº¦ì¸m§@ mpteleport »P mpgoto ¬Û¦ü ¦ý¯àÄ²µo greet entry enter_room ¤§Ãþªº prog 2022/10/11
	{
		if( ch->in_room )
			pRoomIndex = get_room_index( ch->in_room->vnum );
	}
	// ­×§ï¬°¹ï©Ò¦³¥Ø¼Ð§@ mpteleport 2023/01/09
	/*else if( is_number( arg1 ) )  // ¹ï¯S©w®æ§@ mpteleport 2022/10/11
	{
		if ( find_location( ch, arg1 ) == NULL )
		{
			bug( "Mpteleport - No such location: vnum %d.", ch->pIndexData->vnum );
			return;
		}
		pRoomIndex = get_room_index( atoi(arg1) );
	} */
	else if( !( find_location( ch, arg1 ) == NULL ) )  // ¹ï¯S©w®æ§@ mpteleport 2022/10/11
	{
		pRoomIndex = find_location( ch, arg1 );
	}
	else
	{
		bug ( "Mob: %d 'mpteleport' Incorrect type.", ch->pIndexData->vnum );
		return;
	}

	if( !pRoomIndex )
	{
		bug ( "Mob: %d 'mpteleport' Room cannot be found", ch->pIndexData->vnum );
		return;
	}

	if ( pRoomIndex == NULL ) return;

	if ( ch->fighting )
		stop_fighting( ch, TRUE );
	// ¼W¥[ mpteleport ¹ï room_leave §@§P©w 2022/10/11
	if( ch->in_room->roomprogs && ch->in_room->progtypes & ROOM_LEAVE )
		rprog_leave_trigger( ch->in_room, ch , 8);
	char_from_room( ch );
	char_to_room( ch, pRoomIndex );
	// ¼W¥[ mpteleport ¹ï room_enter , entry_prog , greet_prog §P©w 2022/10/11
	if ( pRoomIndex->roomprogs && ( pRoomIndex->progtypes & ROOM_ENTER ) && ch->position > POS_DEAD )
		rprog_entry_trigger( pRoomIndex, ch, 6 );
	if ( ch->position > POS_DEAD )
		mprog_entry_trigger( ch );
	if ( ch->position > POS_DEAD )
		mprog_greet_trigger( ch );
	if ( ch->position > POS_DEAD )
		mprog_entry_greet_trigger( ch );
    return;
}

// added by Amethyst
// allow NPC to use trip directly
void do_mptrip( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *target, *next_target;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
    }

	if( argument[0] == '\0' )
	{
		bug("[do_mptrip] : missing target", 0);
		return;
	}

	if( !str_cmp( argument, "all" ) )
	{
		for ( target = ch->in_room->people; target; target = next_target )
		{
			next_target = target->next_in_room;
			if( IS_NPC(target) ) continue;
			if ( target != ch && can_see( ch, target ) )
			{
				if ( target->in_room == NULL )
				{
					bug("[do_mpstun] : target does not in room", 0);
					return;
				}
				if ( target->fighting != ch )
					return;
				trip( ch, target );
			}
		}
	}
	else
	{
		target = get_char_room(ch, argument);
		if( !target ) 
		{
			// ²¾°£ bug log 2022/11/21
			//bug("[do_mptrip] : target does not in room", 0);
			return;
		}
		trip( ch, target );
	}
	return;
}

void do_skilldrv( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	CHAR_DATA *vch = NULL;
	int dt_undefined = TYPE_UNDEFINED;

	// ­×§ï skilldrv ¨Ï¥Î±ø¥ó 2022/10/27
	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	if( !ch->forced ) // ¥u­n¬O¥H force ´N¥i¥H¶i¦æ skilldrv ¥]§t pc ¤]¯à¦¨¥ß
	{
		if( !IS_NPC( ch ) ) // pc ¦pªG¤£¬O force µLªk¨Ï¥Î , summoned mob »P order ªº³¡¤À©ó interp ¤¤ªý¾×
		{
			send_to_char( "¤°»ò¡H\n\r", ch );
			return;
		}
	}

	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);
	one_argument( argument, arg3);

	if( !str_cmp( arg1, "multihit" ) )
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				//bug("[skilldrv] : multihit no target, vnum %d", ch->pIndexData->vnum);
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				//bug("[skilldrv] : multihit no target, vnum %d", ch->pIndexData->vnum);
				return;
			}
		}
		// add at 2022/10/27
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		multi_hit( ch, vch, TYPE_UNDEFINED );
		//return;
	}
	else if( !str_cmp(arg1, "monehit"))
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				//bug("[skilldrv] : monehit: no such target", 0);
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if( !vch ){
				//bug("[skilldrv] : monehit no such target", 0);
				return;
			}
		}
		// add at 2022/10/27
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		mone_hit( ch, vch, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
	}
	else if ( !str_cmp(arg1, "heavyhit"))
	{
		int ac;
		int ac_before_onehit, ac_after_onehit;				//Added by Razgriz 20050617

		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				//bug("[skilldrv] : heavyhit: no such target", 0);
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				//bug("[skilldrv] : heavyhit no such target", 0);
				return;
			}
		}
		// add at 2022/10/27
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		do_add_qmark(vch, "self heavyhit_preset -1 1 0 0 1 0");
		ac = vch->armor ;
		vch->armor -= GET_AC(vch);
		ac_before_onehit = vch->armor;					//Added by Razgriz 20050617

		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD );
		if( is_qmark( vch, "heavyhit_preset")){
			// npc ¥u¯à¥H mpdel_qmark ¨Ó²¾°£ qmark 2022/11/02
			if( IS_NPC(vch) )
				do_mpdel_qmark(vch, "self heavyhit_preset");
			else
				do_del_qmark(vch, "self heavyhit_preset");
			ac_after_onehit = vch->armor;				//Added by Razgriz 20050617
			vch->armor = ac + (ac_after_onehit - ac_before_onehit);	//Modified by Razgriz 20050617
		}
	}
	else if ( !str_cmp(arg1, "speedhit"))
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				//bug("[skilldrv] : speedhit: no such target", 0);
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ) {
				//bug("[skilldrv] : speedhit no such target", 0);
				return;
			}
		}
		// add at 2022/10/27
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		SET_BIT( ch->act, ACT_SPEEDHIT );
		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD );
		REMOVE_BIT( ch->act, ACT_SPEEDHIT );
	}
	else if ( !str_cmp(arg1, "onehit"))
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				//bug("[skilldrv] : onehit no target", 0);
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				//bug("[skilldrv] : onehit no such target", 0);
				return;
			}
		}
		// add at 2022/10/27
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD );
	}
	else if ( !str_cmp(arg1, "heavyspeedhit"))  // 2022/11/29
	{
		int ac;
		int ac_before_onehit, ac_after_onehit;

		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				return;
			}
		}
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		do_add_qmark(vch, "self heavyhit_preset -1 1 0 0 1 0");
		ac = vch->armor ;
		vch->armor -= GET_AC(vch);
		ac_before_onehit = vch->armor;

		SET_BIT( ch->act, ACT_SPEEDHIT );
		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD );
		REMOVE_BIT( ch->act, ACT_SPEEDHIT );

		if( is_qmark( vch, "heavyhit_preset")){
			if( IS_NPC(vch) )
				do_mpdel_qmark(vch, "self heavyhit_preset");
			else
				do_del_qmark(vch, "self heavyhit_preset");
			ac_after_onehit = vch->armor;
			vch->armor = ac + (ac_after_onehit - ac_before_onehit);
		}
	}
	else if( !str_cmp(arg1, "sec_monehit") && get_eq_char( ch, WEAR_WIELD_2 ) ) // ·s¼W¦¸­nªZ¾¹°l¥[§ðÀ» 2022/11/02
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if( !vch ){
				return;
			}
		}
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		mone_hit ( ch, vch, &dt_undefined, WEAR_WIELD_2 , MSG_NORMAL, DT_NOCHANGE );
	}
	else if ( !str_cmp(arg1, "sec_heavyhit") && get_eq_char( ch, WEAR_WIELD_2 ) )
	{
		int ac;
		int ac_before_onehit, ac_after_onehit;

		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				return;
			}
		}
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		do_add_qmark(vch, "self heavyhit_preset -1 1 0 0 1 0");
		ac = vch->armor ;
		vch->armor -= GET_AC(vch);
		ac_before_onehit = vch->armor;

		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD_2 );
		if( is_qmark( vch, "heavyhit_preset")){
			// npc ¥u¯à¥H mpdel_qmark ¨Ó²¾°£ qmark 2022/11/02
			if( IS_NPC(vch) )
				do_mpdel_qmark(vch, "self heavyhit_preset");
			else
				do_del_qmark(vch, "self heavyhit_preset");
			ac_after_onehit = vch->armor;
			vch->armor = ac + (ac_after_onehit - ac_before_onehit);
		}
	}
	else if ( !str_cmp(arg1, "sec_speedhit") && get_eq_char( ch, WEAR_WIELD_2 ) )
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ) {
				return;
			}
		}
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		SET_BIT( ch->act, ACT_SPEEDHIT );
		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD_2 );
		REMOVE_BIT( ch->act, ACT_SPEEDHIT );
	}
	else if ( !str_cmp(arg1, "sec_onehit") && get_eq_char( ch, WEAR_WIELD_2 ) )
	{
		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				return;
			}
		}
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD_2 );
	}
	else if ( !str_cmp(arg1, "sec_heavyspeedhit") && get_eq_char( ch, WEAR_WIELD_2 ) )
	{
		int ac;
		int ac_before_onehit, ac_after_onehit;

		if( arg2[0] == '\0' ){
			if( ch->fighting ){
				vch = ch->fighting;
			}else{
				return;
			}
		}else{
			vch = get_char_room(ch, arg2);
			if ( !vch ){
				return;
			}
		}
		if( ch->deleted || !ch || vch->deleted || !vch )
			return;
		do_add_qmark(vch, "self heavyhit_preset -1 1 0 0 1 0");
		ac = vch->armor ;
		vch->armor -= GET_AC(vch);
		ac_before_onehit = vch->armor;

		SET_BIT( ch->act, ACT_SPEEDHIT );
		one_hit( ch, vch, TYPE_UNDEFINED, WEAR_WIELD_2 );
		REMOVE_BIT( ch->act, ACT_SPEEDHIT );

		if( is_qmark( vch, "heavyhit_preset")){
			if( IS_NPC(vch) )
				do_mpdel_qmark(vch, "self heavyhit_preset");
			else
				do_del_qmark(vch, "self heavyhit_preset");
			ac_after_onehit = vch->armor;
			vch->armor = ac + (ac_after_onehit - ac_before_onehit);
		}
	}
	/*  use to add new skilldrv
		else if ( !str_cmp(arg1, "")) 
		{
		}
	*/
	//else
	return;
}


/* prints the argument to all the rooms aroud the mobile */

void do_mpasound( CHAR_DATA *ch, char *argument )
{

  ROOM_INDEX_DATA *was_in_room;
  int		   door;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
//	send_to_char( "¤°»ò¡H\n\r", ch );
//	return;
  //  }

    if ( argument[0] == '\0' )
    {
	bug( "Mpasound - No argument: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
      EXIT_DATA	      *pexit;
      
      if ( ( pexit = was_in_room->exit[door] ) != NULL
	  &&   pexit->to_room != NULL
	  &&   pexit->to_room != was_in_room )
      {
	ch->in_room = pexit->to_room;
	MOBtrigger  = FALSE;
	act( argument, ch, NULL, NULL, TO_ROOM );
      }
    }

  ch->in_room = was_in_room;
  return;

}

/* lets the mobile kill any player or mobile without murder*/

void do_mpkill( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	/*if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    {
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
	} */

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		bug( "MpKill - no argument: vnum %d.", ch->pIndexData->vnum );
		return;
	}

    if ( ( target = get_char_room( ch, arg ) ) == NULL )
    {
		// bug( "MpKill - Victim not in room: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	if( target->in_room && IS_SET(target->in_room->room_flags, ROOM_SAFE)){
		send_to_char("³o¸Ì¸T¤î¾Ô°«¡C\n\r", ch);
		return ;
	}

	if ( target == ch )
	{
		bug( "MpKill - Bad target to attack: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == target )
	{
		bug( "MpKill - Charmed mob attacking master: vnum %d.", ch->pIndexData->vnum );
		return;
	}

    /* ED Fix
	if ( ch->position == POS_FIGHTING )
	{	
		bug( "MpKill - Already fighting: vnum %d", ch->pIndexData->vnum );
		return;
	}*/

	multi_hit( ch, target, TYPE_UNDEFINED );

	return;
}


/* lets the mobile destroy an object in its inventory
   it can also destroy a worn object and it can destroy 
   items using all.xxxxx or just plain all of them */

void do_mpjunk( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	if( !IS_NPC(ch) ) return;
	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//	send_to_char( "¤°»ò¡H\n\r", ch );
	//	return;
	//  }

	one_argument( argument, arg );

	if ( arg[0] == '\0')
	{
		bug( "Mpjunk - No argument: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
	{
		if ( ( obj = get_obj_wear( ch, arg ) ) != NULL )
		{
			unequip_char( ch, obj );
			extract_obj( obj );
			return;
		}
		if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
			return; 
		extract_obj( obj );
	}
	else
	{
		for ( obj = ch->carrying; obj != NULL; obj = obj_next )
		{
			obj_next = obj->next_content;
			if ( obj->deleted )
				continue;
			if ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
			{
				if ( obj->wear_loc != WEAR_NONE)
					unequip_char( ch, obj );
				extract_obj( obj );
			}
		}
	}

	return;

}

/* prints the message to everyone in the room other than the mob and target */

void do_mpechoaround( CHAR_DATA *ch, char *argument )
{
  char	     arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    if( !IS_NPC(ch) ) return;
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
      // send_to_char( "¤°»ò¡H\n\r", ch );
       //return;
   // }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       bug( "Mpechoaround - No argument:  vnum %d.", ch->pIndexData->vnum );
       return;
    }

    if ( !( target=get_char_room( ch, arg ) ) )
    {
//	bug( "Mpechoaround - target does not exist: vnum %d.",
//	    ch->pIndexData->vnum );
	return;
    }

    act( argument, ch, NULL, target, TO_NOTVICT );
    return;
}

/* prints the message to only the target */

void do_mpechoat( CHAR_DATA *ch, char *argument )
{
  char	     arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    if( !IS_NPC(ch) ) return;
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
      // send_to_char( "¤°»ò¡H\n\r", ch );
      // return;
   // }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Mpechoat - No argument:  vnum %d.",
	   ch->pIndexData->vnum );
       return;
    }

    if ( !( target = get_char_room( ch, arg ) ) )
    {
//	bug( "Mpechoat - target does not exist: vnum %d.",
//	    ch->pIndexData->vnum );
	return;
    }

    act( argument, ch, NULL, target, TO_VICT );
    return;
}

/* prints the message to the room at large */

void do_mpecho( CHAR_DATA *ch, char *argument )
{
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	//  {
	//		send_to_char( "¤°»ò¡H\n\r", ch );
	//		return;
	// }

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	if ( argument[0] == '\0' )
	{
		bug( "Mpecho - called w/o argument: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	act( argument, ch, NULL, NULL, TO_ROOM );
	return;

}


void do_mpechotoall( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *cha;
    char buf [MAX_STRING_LENGTH];
    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
//	send_to_char( "¤°»ò¡H\n\r", ch );
//	return;
  //  }

    if ( argument[0] == '\0' )
    {
	bug( "Mpechotoall - called w/o argument: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }
    sprintf(buf, "\n\r%s\n\r", argument);

	for(cha=char_list;cha;cha=cha->next)
	{
		if ( !IS_NPC(cha) && ch->in_room != cha->in_room )
		{
			send_to_char(buf, cha);
		}
	}
}

/* prints the non newline message to everyone in the room other than the mob and target*/

void do_mpnonechoaround( CHAR_DATA *ch, char *argument )
{
  char	     arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    if( !IS_NPC(ch) ) return;
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
      // send_to_char( "¤°»ò¡H\n\r", ch );
       //return;
   // }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       bug( "Mpechoaround - No argument:  vnum %d.", ch->pIndexData->vnum );
       return;
    }

    if ( !( target=get_char_room( ch, arg ) ) )
    {
//	bug( "Mpechoaround - target does not exist: vnum %d.",
//	    ch->pIndexData->vnum );
	return;
    }

    act_nonline( argument, ch, NULL, target, TO_NOTVICT );
    return;
}

/* prints the message to only the target with non newline change*/

void do_mpnonechoat( CHAR_DATA *ch, char *argument )
{
  char	     arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    if( !IS_NPC(ch) ) return;
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
      // send_to_char( "¤°»ò¡H\n\r", ch );
      // return;
   // }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Mpechoat - No argument:  vnum %d.",
	   ch->pIndexData->vnum );
       return;
    }

    if ( !( target = get_char_room( ch, arg ) ) )
    {
//	bug( "Mpechoat - target does not exist: vnum %d.",
//	    ch->pIndexData->vnum );
	return;
    }

    act_nonline( argument, ch, NULL, target, TO_VICT );
    return;
}

/* prints the non newline message to the room at large*/

void do_mpnonecho( CHAR_DATA *ch, char *argument )
{
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
   // {
//	send_to_char( "¤°»ò¡H\n\r", ch );
//	return;
  //  }

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    if ( argument[0] == '\0' )
    {
	bug( "Mpecho - called w/o argument: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    act_nonline( argument, ch, NULL, NULL, TO_ROOM );
    return;

}


void do_mpnowait( CHAR_DATA *ch, char *argument )
{
	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }
	ch->wait = 0;
}

void do_mpdesc( CHAR_DATA *ch, char *argument )
{
	char type[ MAX_INPUT_LENGTH ];
	char buf[ MAX_INPUT_LENGTH ];

	if( !IS_NPC (ch) ) {
		send_to_char ( "¤°»ò¡H\n\r", ch );
		return;
	}

	if ( !ch )
		return;

	argument = one_argument( argument, type );

	buf[0] = '\0';

	if ( !str_cmp( "N", type ) )
	{
		strcat( buf, argument );
		strcat( buf, "\n\r" );
		free_string( ch->description );
		ch->description = str_dup( buf );
		return;
	}else if ( !str_cmp( "A", type ) ){
		if ( ch->description )
			strcat( buf, ch->description );
		strcat( buf, argument );
		strcat( buf, "\n\r" );
		free_string( ch->description );
		ch->description = str_dup( buf );
		return;
	}else{
		bug( "Mpdesc - mob vnum %d no desc Tyep.", ch->pIndexData->vnum );
	}
	return;
}

/*
 * Mob mset command	//Added by Razgriz	20050626
 */
void do_mpmset ( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char buf [ MAX_STRING_LENGTH ];
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	char arg3 [ MAX_INPUT_LENGTH ];
	int value;
	int max;

	if( !IS_NPC (ch) ) {
		send_to_char ( "¤°»ò¡H\n\r", ch );
		return;
	}

	smash_tilde ( argument );
	argument = one_argument ( argument, arg1 );
	argument = one_argument ( argument, arg2 );
	strcpy ( arg3, argument );

	if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
	{
		send_to_char ( "Syntax: mset <victim> <field>  <value>\n\r", ch );
		send_to_char ( "or:     mset <victim> <string> <value>\n\r", ch );
		send_to_char ( "\n\r", ch );
		send_to_char ( "Field being one of:\n\r", ch );
		send_to_char ( "     str int wis dex con class sex race level\n\r",	ch );
		send_to_char ( "     gold hp mana move practice align\n\r", ch );
		send_to_char ( "     thirst drunk full security respect special\n\r", ch );	/* OLC */
		send_to_char ( "     hitroll damroll armor dodge pp size ", ch );	/* OLC */
		send_to_char ( "\n\r", ch );
		send_to_char ( "String being one of:\n\r", ch );
		send_to_char ( "     name cname short long title spec\n\r", ch );
		return;
	}

	if ( !( victim = get_char_world ( ch, arg1 ) ) ) {
		send_to_char ( "They aren't here.\n\r", ch );
		return;
	}

	/*
	 * Snarf the value (which need not be numeric).
	 */

	value = is_number ( arg3 ) ? atoi ( arg3 ) : -1;

	/*
	 * Set something.
	 */

	/*  ²¾°£¤j¶q°w¹ï pc ¥B·|¥Ã¤[§ïÅÜ¼Æ­Èªº¶µ¥Ø 2022/11/05
		¨ÃÅÜ§ó¤j¦h¶µ¥Ø¶È¯à¹ï npc ¨Ï¥Î, ¥B¦b¶µ¥Ø¤¤¥[¤J bug log
	
		if( !str_cmp (arg2, "playd") )
		{
			if ( IS_NPC ( victim ) ) {
				send_to_char ( "Not on NPC's.\n\r", ch );
				return;
			}
			victim->played = value;
			return;
		}
		if ( !str_cmp ( arg2, "class" ) )
		{
			if ( value < 0 || value >= MAX_CLASS ) {
				sprintf ( buf, "Class range is 0 to %d.\n", MAX_CLASS-1 );
				send_to_char ( buf, ch );
				return;
			}
			victim->class = value;
			return;
		}
		if ( !str_cmp ( arg2, "practice" ) )
		{

			if ( value < 0 || value > 1000 ) {
				send_to_char ( "Practice range is 0 to 1000 sessions.\n\r", ch );
				return;
			}

			victim->practice = value;
			return;
		}
		if ( !str_cmp ( arg2, "respect" ) )
		{
			if ( IS_NPC ( victim ) ) {
				send_to_char ( "Not on NPC's.\n\r", ch );
				return;
			}

			if ( value < -1000 || value > 1000 ) {
				send_to_char ( "Respect range is -1000 to 1000.\n\r", ch );
				return;
			}

			victim->pcdata->respect = value;
			return;
		}
		if ( !str_cmp ( arg2, "title" ) )
		{
			if ( IS_NPC ( victim ) ) {
				send_to_char ( "Not on NPC's.\n\r", ch );
				return;
			}

			set_title ( victim, arg3 );
			return;
		}
		if ( !str_cmp ( arg2, "cname" ) )
		{
			set_cname ( victim, arg3 );
			return;
		}
		if ( !str_cmp ( arg2, "security" ) )	// OLC
		{
			if ( IS_NPC ( victim ) ) {
				send_to_char ( "Not on NPC's.\n\r", ch );
				return;
			}
		
			if ( value > ch->pcdata->security || value < 0 )
			{
				if ( ch->pcdata->security != 0 ) {
					sprintf( buf, "Valid security is 0-%d.\n\r", ch->pcdata->security );
					send_to_char ( buf, ch );
				}
				else {
					send_to_char ( "Valid security is 0 only.\n\r", ch );
				}
				
				return;
			}
			
			victim->pcdata->security = value;
			return;
		}
	*/

	if ( !str_cmp ( arg2, "str" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : str.", ch->pIndexData->vnum );
			return;
		}

		max = 25;

		if ( value < 3 || value > max ) {
			sprintf ( buf, "Strength range is 3 to %d.\n\r", max );
			send_to_char ( buf, ch );
			return;
		}

		victim->perm_str = value;
		return;
	}

	if ( !str_cmp ( arg2, "int" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : int.", ch->pIndexData->vnum );
			return;
		}

		max = 25;

		if ( value < 3 || value > max ) {
			sprintf ( buf, "Intelligence range is 3 to %d.\n\r", max );
			send_to_char ( buf, ch );
			return;
		}

		victim->perm_int = value;
		return;
	}

	if ( !str_cmp ( arg2, "wis" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : wis.", ch->pIndexData->vnum );
			return;
		}

		max = 25;

		if ( value < 3 || value > max ) {
			sprintf ( buf, "Wisdom range is 3 to %d.\n\r", max );
			send_to_char ( buf, ch );
			return;
		}

		victim->perm_wis = value;
		return;
	}

	if ( !str_cmp ( arg2, "dex" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : dex.", ch->pIndexData->vnum );
			return;
		}

		max = 25;

		if ( value < 3 || value > max ) {
			sprintf ( buf, "Dexterity range is 3 to %d.\n\r", max );
			send_to_char ( buf, ch );
			return;
		}

		victim->perm_dex = value;
		return;
	}

	if ( !str_cmp ( arg2, "con" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : con.", ch->pIndexData->vnum );
			return;
		}
		max = 25;

		if ( value < 3 || value > max )
		{
			sprintf ( buf, "Constitution range is 3 to %d.\n\r", max );
			send_to_char ( buf, ch );
			return;
		}

		victim->perm_con = value;
		return;
	}

	// ¥[¤J size 2022/12/24
	if ( !str_cmp ( arg2, "size" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : size.", ch->pIndexData->vnum );
			return;
		}

		victim->mod_size = value;
		return;
	}

	if ( !str_cmp ( arg2, "sex" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : sex.", ch->pIndexData->vnum );
			return;
		}
		if ( IS_AFFECTED ( victim, AFF_CHANGE_SEX ) ) {
			send_to_char ( "This person is affect by change sex.\n\r", ch );
			send_to_char ( "Try again later.\n\r", ch );
			return;
		}

		if ( value < 0 || value > 2 ) {
			send_to_char ( "Sex range is 0 to 2.\n\r", ch );
			return;
		}

		victim->sex = value;
		return;
	}

	if ( !str_cmp ( arg2, "race" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : race.", ch->pIndexData->vnum );
			return;
		}
		//OBJ_DATA *wield;
		//OBJ_DATA *wield2;
		int race;

		if ( IS_AFFECTED( victim, AFF_POLYMORPH ) ) {
			send_to_char ( "This person is affected by polymorph other.\n\r", ch );
			send_to_char ( "Try again later.\n\r", ch );
			return;
		}

		race = race_lookup ( arg3 );

		if ( race < 0 ) {
			send_to_char ( "Invalid race.\n\r", ch );
			return;
		}

		/*if (!IS_SET ( race_table[race].race_abilities, RACE_PC_AVAIL ) && get_trust ( ch ) < L_SEN) {
			send_to_char ( "You may not set a race not available to PC's.\n\r", ch );
			return;
		}*/

		victim->race = race;

		// °w¹ï mob ¨S¦³¥²­n±j¨î remove weapon , ¦pªG­n remove ¦b area ÀÉ¤¤§@´N¦æ¤F 2022/11/05
		/*if ( ( wield = get_eq_char ( victim, WEAR_WIELD ) ) && !IS_SET ( race_table[ victim->race ].race_abilities, RACE_WEAPON_WIELD ))
		{
			act ( "You drop $p.", victim, wield, NULL, TO_CHAR );
			act ( "$n drops $p.", victim, wield, NULL, TO_ROOM );
			obj_from_char ( wield );
			obj_to_room	( wield, victim->in_room );
		}

		if ( ( wield2 = get_eq_char ( victim, WEAR_WIELD_2 ) ) && !IS_SET ( race_table[ victim->race ].race_abilities, RACE_WEAPON_WIELD ))
		{
			act ( "You drop $p.", victim, wield2, NULL, TO_CHAR );
			act ( "$n drops $p.", victim, wield2, NULL, TO_ROOM );
			obj_from_char ( wield2 );
			obj_to_room	( wield2, victim->in_room );
		}

		if ( ( wield2 = get_eq_char ( victim, WEAR_TWO_HAND ) ) && !IS_SET( race_table[ victim->race ].race_abilities, RACE_WEAPON_WIELD ))
		{
			act ( "You drop $p.", victim, wield2, NULL, TO_CHAR );
			act ( "$n drops $p.", victim, wield2, NULL, TO_ROOM );
			obj_from_char ( wield2 );
			obj_to_room	( wield2, victim->in_room );
		} */

		return;
	}

	if ( !str_cmp ( arg2, "level" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : level.", ch->pIndexData->vnum );
			return;
		}

		if ( value < 1 || value > 300 ) {
			send_to_char ( "Level range is 1 to 300.\n\r", ch );
			return;
		}

		victim->level = value;
		return;
	}

	if ( !str_cmp ( arg2, "gold" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : gold.", ch->pIndexData->vnum );
			return;
		}
		victim->gold = value;
		return;
	}

	if ( !str_cmp ( arg2, "hp" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : hp.", ch->pIndexData->vnum );
			return;
		}
		if ( value < 1 || value > 5000000 ) {
			send_to_char ( "Hp range is 1 to 5,000,000 hit points.\n\r", ch );
			return;
		}

		victim->max_hit = value;
		return;
	}

	if ( !str_cmp ( arg2, "mana" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : mana.", ch->pIndexData->vnum );
			return;
		}
		if ( value < 1 || value > 5000000 ) {
			send_to_char ( "Mana range is 1 to 5,000,000 mana points.\n\r", ch );
			return;
		}

		victim->max_mana = value;
		return;
	}

	if ( !str_cmp ( arg2, "move" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : move.", ch->pIndexData->vnum );
			return;
		}

		if ( value < 1 || value > 5000000 ) {
			send_to_char ( "Move range is 1 to 5,000,000 move points.\n\r", ch );
			return;
		}

		victim->max_move = value;
		return;
	}

	if ( !str_cmp ( arg2, "align" ) )
	{
		if ( value < -1000 || value > 1000 ) {
			send_to_char ( "Alignment range is -1000 to 1000.\n\r", ch );
			return;
		}

		victim->alignment = value;
		return;
	}

	if ( !str_cmp ( arg2, "damroll" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : damroll.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->damroll ) <= -1000 || ( value + victim->damroll ) >= 3000 ) {
			send_to_char ( "You cannot set a person's damroll over 3000 or below -1000.\n\r", ch );
			return;
		}

		victim->damroll += value;
		return;
	}

	if ( !str_cmp ( arg2, "hitroll" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : hitroll.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->hitroll ) <= -1000 || ( value + victim->hitroll ) >= 3000 ) {
			send_to_char ( "You cannot set a person's hitroll over 3000 or below -1000.\n\r", ch );
			return;
		}

		victim->hitroll += value;
		return;
	}

	if ( !str_cmp ( arg2, "armor" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : armor.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->armor ) <= -50000 || ( value + victim->armor ) >= 5000 ) {
			send_to_char ( "You cannot set a person's armor over 5000 or below -50000.\n\r", ch );
			return;
		}

		victim->armor += value;
		return;
	}

	if ( !str_cmp ( arg2, "dodge" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : dodge.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->dodge ) <= -50000 || ( value + victim->dodge ) >= 5000 ) {
			send_to_char ( "You cannot set a person's dodge over 5000 or below -50000.\n\r", ch );
			return;
		}

		victim->dodge += value;
		return;
	}

	if ( !str_cmp ( arg2, "pp" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : pp.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->pp ) <= -100 || ( value + victim->pp ) >= 500 ) {
			send_to_char ( "You cannot set a person's pp over 300 or below -100.\n\r", ch );
			return;
		}

		victim->pp += value;
		return;
	}

	if ( !str_cmp ( arg2, "special" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : special.", ch->pIndexData->vnum );
			return;
		}

		victim->special = value;
		return;
	}

	// ­×§ï Cond ªº³Ì¤j­È±q 48 ¬° 100  2023/01/13
	if ( !str_cmp ( arg2, "thirst" ) )
	{
		if ( IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on NPC : thirst.", ch->pIndexData->vnum );
			return;
		}

		if ( value < 0 || value > 100 /*48*/ ) {
			send_to_char ( "Thirst range is 0 to 100.\n\r", ch );
			return;
		}

		victim->pcdata->condition[COND_THIRST] = value;
		return;
	}

	if ( !str_cmp ( arg2, "drunk" ) )
	{
		if ( IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on NPC : drunk.", ch->pIndexData->vnum );
			return;
		}

		if ( value < 0 || value > 100 /*48*/  ) {
			send_to_char ( "Drunk range is 0 to 100.\n\r", ch );
			return;
		}

		victim->pcdata->condition[COND_DRUNK] = value;
		return;
	}

	if ( !str_cmp ( arg2, "full" ) )
	{
		if ( IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on NPC : full.", ch->pIndexData->vnum );
			return;
		}

		if ( value < 0 || value > 100 /*48*/  ) {
			send_to_char( "Full range is 0 to 100.\n\r", ch );
			return;
		}

		victim->pcdata->condition[COND_FULL] = value;
		return;
	}

	if ( !str_cmp ( arg2, "name" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : name.", ch->pIndexData->vnum );
			return;
		}

		if ( longstring ( ch, arg3 ) )
			return;

		free_string ( victim->name );
		victim->name = str_dup ( arg3 );
		return;
	}

	if ( !str_cmp ( arg2, "short" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : short.", ch->pIndexData->vnum );
			return;
		}
		if ( longstring ( ch, arg3 ) )
			return;

		free_string ( victim->short_descr );
		victim->short_descr = str_dup ( arg3 );
		return;
	}

	if ( !str_cmp ( arg2, "long" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : long.", ch->pIndexData->vnum );
			return;
		}

		if ( longstring ( ch, arg3 ) )
			return;

		free_string ( victim->long_descr );
		strcat ( arg3, "\n\r" );
		victim->long_descr = str_dup ( arg3 );
		return;
	}

	if ( !str_cmp ( arg2, "spec" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : spec.", ch->pIndexData->vnum );
			return;
		}

		if ( ( victim->spec_fun = spec_lookup ( arg3 ) ) == 0 ) {
			send_to_char ( "No such spec fun.\n\r", ch );
			return;
		}

		return;
	}
	// ·s¼W¼Æ­Ó°Ñ¼Æ 2022/11/05
	if ( !str_cmp ( arg2, "apdr" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : apdr.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->apdr ) <= -10000 || ( value + victim->apdr ) >= 10000 ) {
			send_to_char ( "You cannot set a person's apdr over 10000 or below -10000.\n\r", ch );
			return;
		}

		victim->apdr += value;
		return;
	}
	
	if ( !str_cmp ( arg2, "amdr" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : amdr.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->amdr ) <= -10000 || ( value + victim->amdr ) >= 10000 ) {
			send_to_char ( "You cannot set a person's amdr over 10000 or below -10000.\n\r", ch );
			return;
		}

		victim->amdr += value;
		return;
	}
	
	if ( !str_cmp ( arg2, "pdr" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : pdr.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->pdr ) <= -10000 || ( value + victim->pdr ) >= 200000 ) {
			send_to_char ( "You cannot set a person's pdr over 200000 or below -10000.\n\r", ch );
			return;
		}

		victim->pdr += value;
		return;
	}
	
	if ( !str_cmp ( arg2, "mdr" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : mdr.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->mdr ) <= -10000 || ( value + victim->mdr ) >= 200000 ) {
			send_to_char ( "You cannot set a person's mdr over 200000 or below -10000.\n\r", ch );
			return;
		}

		victim->mdr += value;
		return;
	}
	
	if ( !str_cmp ( arg2, "pad" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : pad.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->pad ) <= 0 || ( value + victim->pad ) >= 200000 ) {
			send_to_char ( "You cannot set a person's pad over 200000 or below 0.\n\r", ch );
			return;
		}

		victim->pad += value;
		return;
	}
	
	if ( !str_cmp ( arg2, "mad" ) )
	{
		if ( !IS_NPC ( victim ) ) {
			bug( "Mpmset - mob vnum %d illegal arg on PC : mad.", ch->pIndexData->vnum );
			return;
		}

		if ( ( value + victim->mad ) <= 0 || ( value + victim->mad ) >= 200000 ) {
			send_to_char ( "You cannot set a person's mad over 200000 or below 0.\n\r", ch );
			return;
		}

		victim->mad += value;
		return;
	}
	
	if ( !str_cmp ( arg2, "svs" ) )
	{

		if ( ( value + victim->saving_throw[0] ) <= -500 || ( value + victim->saving_throw[0] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svs over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[0] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svfire" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_FIRE] ) <= -500 || ( value + victim->saving_throw[MAGIC_FIRE] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svfire over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_FIRE] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svcold" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_COLD] ) <= -500 || ( value + victim->saving_throw[MAGIC_COLD] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svcold over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_COLD] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svwind" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_WIND] ) <= -500 || ( value + victim->saving_throw[MAGIC_WIND] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svwind over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_WIND] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svlight" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_LIGHTNING] ) <= -500 || ( value + victim->saving_throw[MAGIC_LIGHTNING] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svlight over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_LIGHTNING] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svearth" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_EARTH] ) <= -500 || ( value + victim->saving_throw[MAGIC_EARTH] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svearth over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_EARTH] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svsaint" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_SAINT] ) <= -500 || ( value + victim->saving_throw[MAGIC_SAINT] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svsaint over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_SAINT] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svdark" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_DARK] ) <= -500 || ( value + victim->saving_throw[MAGIC_DARK] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svdark over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_DARK] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svbreath" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_BREATH] ) <= -500 || ( value + victim->saving_throw[MAGIC_BREATH] ) >= 500 ) {
			send_to_char ( "You cannot set a person's svbreath over 500 or below -500.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_BREATH] += value;
		return;
	}

	if ( !str_cmp ( arg2, "svpoison" ) )
	{

		if ( ( value + victim->saving_throw[MAGIC_POISON] ) <= -100000 || ( value + victim->saving_throw[MAGIC_POISON] ) >= 100000 ) {
			send_to_char ( "You cannot set a person's svpoison over 100000 or below -100000.\n\r", ch );
			return;
		}

		victim->saving_throw[MAGIC_POISON] += value;
		return;
	}

	/*
	 * Generate usage message.
	 */
	//do_mset ( ch, "" );  2022/11/05
	do_mpmset ( ch, "" );
	return;
}


/* lets the mobile load an item or mobile.  All items
are loaded into inventory.  you can specify a level with
the load object portion as well. */

void do_mpmload( CHAR_DATA *ch, char *argument )
{
    char	    arg[ MAX_INPUT_LENGTH ];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA	   *target;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
//	send_to_char( "¤°»ò¡H\n\r", ch );
//	return;
  //  }

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	bug( "Mpmload - Bad vnum as arg: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	bug( "Mpmload - Bad mob vnum: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    target = create_mobile( pMobIndex );
    char_to_room( target, ch->in_room );
    mprog_repop_trigger(target);
    return;
}

void do_mpoload( CHAR_DATA *ch, char *argument )
{
    char arg1[ MAX_INPUT_LENGTH ];
    char arg2[ MAX_INPUT_LENGTH ];
    char arg3[ MAX_INPUT_LENGTH ];
    char arg4[ MAX_INPUT_LENGTH ];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA	   *obj;
    CHAR_DATA	   *victim;
    int		    level = LEVEL_IMMORTAL;
    bool        fLevel = FALSE;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
//	send_to_char( "¤°»ò¡H\n\r", ch );
//	return;
  //  }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );

    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
      bug( "Mpoload - Bad syntax: vnum %d.",
          ch->pIndexData->vnum );
      return;
    }
 
    if ( arg3[0] == '\0' )
    {
      if ( arg2[0] == '\0' ) {
        level = get_trust( ch );
      } else if ( is_number( arg2 ) ){
        level = atoi( arg2 );
        fLevel = TRUE;
      } else {
        level = get_trust( ch );
      }

      if (!fLevel) level = UMIN( number_fuzzy( LEVEL_IMMORTAL - 3 ), level);
       if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
      {
          bug( "Mpoload - Obj not found: vnum %d.", ch->pIndexData->vnum );
          bug( "Mpoload - Obj not found: obj vnum %d.", atoi(arg1));
          return;
      }
      obj = fLevel ? fcreate_object( pObjIndex, level ) : create_object( pObjIndex, level );
      if ( arg2[0] == '\0' || is_number( arg2 ) )
      {
//            if ( CAN_WEAR(obj, ITEM_TAKE) )
          obj_to_char( obj, ch );
//            else
//            obj_to_room( obj, ch->in_room );
      }
      else
      {
          if ( !str_cmp( arg2, "floor" ) )
              obj_to_room( obj, ch->in_room );
          else    obj_to_char( obj, ch );
      }
      oprog_repop_trigger(obj);
      return;
    }
    else if ( arg4[0] == '\0' )
    {
        if ( !str_cmp( arg3, "floor" ) )
        {
            if ( !is_number( arg2 ) )
            {
                if ( !str_cmp( arg2, "standard" ) )
                    level = number_fuzzy( ch->level - 1 );
                else
                {
                    bug( "Mpoload - arg2 is ambiguous: vnum %d.",
                           ch->pIndexData->vnum );
                    return;
                }
                level = UMIN( number_fuzzy( LEVEL_IMMORTAL - 3 ), level);
            }
            else{
                level = atoi( arg2 );
                        fLevel = TRUE;
                }
 			if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    		{
				bug( "Mpoload - obj not found: vnum %d.", ch->pIndexData->vnum );
				bug( "Mpoload - obj not found: obj's vnum %d.", atoi(arg1) );
				return;
    		}
                        obj = fLevel ? fcreate_object( pObjIndex, level ) : create_object( pObjIndex, level );
			obj_to_room( obj, ch->in_room );
			oprog_repop_trigger(obj);
			return;
		}
    	else if ( !str_cmp( arg3, "target" ) )
		{
			if ( !( victim = get_char_room( ch, arg2 ) ) )
    		{
				bug( "Mpoload - victim not found: vnum %d.",
	    			ch->pIndexData->vnum );
				return;
    		}	
 			if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    		{
				bug( "Mpoload - obj not found: vnum %d.", ch->pIndexData->vnum );
				bug( "Mpoload - obj not found: vnum of obj %d.", atoi(arg1));
				return;
    		}
                        obj = fLevel ? fcreate_object( pObjIndex, level ) : create_object( pObjIndex, level );
			//obj_to_room( obj, ch->in_room );
			obj_to_char(obj, victim);
			oprog_repop_trigger(obj);
			return;
		}
	}
	if ( !( victim = get_char_room( ch, arg2 ) ) )
    	{
		bug( "Mpoload - victim not found: vnum %d.",
	    	ch->pIndexData->vnum );
		return;
    	}	
	if ( is_number( arg3 ) ){
		level = atoi( arg3 );
                fLevel = TRUE;
        }
	else if ( !str_cmp( arg3, "viclv" ) ){
		level = victim->level;
                fLevel = TRUE;
        }
	else if ( !str_cmp( arg3, "standard" ) ){
    		level = number_fuzzy( ch->level - 1 );
		level = UMIN( number_fuzzy( LEVEL_IMMORTAL - 3 ), level);
	}
	else	{
	    level = get_trust( ch );
	    level = UMIN( number_fuzzy( LEVEL_IMMORTAL - 3 ), level);
	}

 	if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    	{
		bug( "Mpoload - obj not found: vnum %d.", ch->pIndexData->vnum );
		return;
    	}

        obj = fLevel ? fcreate_object( pObjIndex, level ) : create_object( pObjIndex, level );
/*
    	if ( !CAN_WEAR(obj, ITEM_TAKE) )
    	{
		obj_to_room( obj, ch->in_room );
		return;
    	}
*/
	if ( arg4[0] == '\0' )
		obj_to_char( obj, ch );
	else if ( !str_cmp( arg4, "target" ) )
		obj_to_char( obj, victim );
	else if ( !str_cmp( arg4, "floor" ) )
		obj_to_room( obj, ch->in_room );
	else	obj_to_char( obj, ch );
	oprog_repop_trigger(obj);
    return;
}


/* lets the mobile purge all objects and other npcs in the room,
   or purge a specified object or mob in the room.  It can purge
   itself, but this had best be the last command in the MOBprogram
   otherwise ugly stuff will happen */

void do_mppurge( CHAR_DATA *ch, char *argument )
{
	char       arg[ MAX_INPUT_LENGTH ];
	char       arg1[ MAX_INPUT_LENGTH ];
	char       arg2[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	OBJ_DATA  *obj;
	int objvnum = 0;
	
	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
    //if ( !IS_NPC( ch ) || (IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED(ch, AFF_SUMMONED)))
	// {
	//	send_to_char( "¤°»ò¡H\n\r", ch );
	//	return;
	//  }

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		/* 'purge' */
		CHAR_DATA *vnext;
		OBJ_DATA  *obj_next;

		for ( target = ch->in_room->people; target != NULL; target = vnext )
		{
		  vnext = target->next_in_room;
		  if ( !target->in_room || target->deleted )
			  continue;
		  if ( IS_NPC( target ) && target != ch )
			extract_char( target, TRUE );
		}

		for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
		{
		  obj_next = obj->next_content;
		  if ( obj->deleted )
			continue;
		  extract_obj( obj );
		}

		return;
    }

    argument = one_argument( argument, arg1 );
    if ( arg1[0] == '\0' )
    {
		if ( str_cmp(arg, "all") && str_prefix( "all.", arg ) )
		{
			if ( ( target = get_char_room( ch, arg ) ) == NULL )
			{
				if( !(obj = get_obj_here(ch, arg)))
				{
					//bug( "Mppurge - Target not found: vnum %d.", ch->pIndexData->vnum );
					return;
				}
				else
				{
					//obj_from_char(obj);
					extract_obj( obj );
					return;
				}
			}

			if ( !IS_NPC( target ) )
			{
				//bug( "Mppurge - Purging a PC: vnum %d.", ch->pIndexData->vnum );
				return;
			}
			extract_char( target, TRUE );
		}
		else
		{
			OBJ_DATA *obj_next;
			for ( obj = ch->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& can_drop_obj( ch, obj ) )
				{
					obj_from_char(obj);
					extract_obj(obj);
				}
			}
		}
		return;
	}
	if ( ( target = get_char_room( ch, arg ) ) == NULL )
	{
		//bug( "Mppurge - Target not found: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg2 );
	objvnum = atoi( arg2 );

	// add at 20/03/05
	if ( is_number( arg2 ) ){
		if ( !str_prefix( "all.vnum", arg1 ) )
		{
			OBJ_DATA *obj_next;

			for ( obj = target->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( obj->pIndexData->vnum == objvnum
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& can_drop_obj( ch, obj ) )
				{
					obj_from_char(obj);
					extract_obj(obj);
				}
			}
		}
		else if ( !str_prefix( "vnum", arg1 ) )
		{
			for( obj = target->carrying; obj; obj = obj->next_content )
			{
				if ( obj->deleted )
					continue;
				if(obj->pIndexData->vnum == objvnum)
				{
					obj_from_char( obj );
					extract_obj( obj );
				}
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		if ( str_cmp(arg1, "all") && str_prefix( "all.", arg1 ) )
		{
			for( obj = target->carrying; obj; obj = obj->next_content )
			{
				if ( obj->deleted )
					continue;
				if ( is_name( arg1, obj->name ) )
				{
					obj_from_char( obj );
					extract_obj( obj );
				}
			}
		}
		else
		{
			OBJ_DATA *obj_next;
			for ( obj = target->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& can_drop_obj( ch, obj ) )
				{
					obj_from_char(obj);
					extract_obj(obj);
				}
			}
		}
	}
	return;
}


/* lets the mobile goto any location it wishes that is not private */

void do_mpgoto( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	ROOM_INDEX_DATA *location;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	//{
	//	send_to_char( "¤°»ò¡H\n\r", ch );
	//	return;
	//  }

	one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		bug( "Mpgoto - No argument: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( ( location = find_location( ch, arg ) ) == NULL )
	{
		// ·|³y¦¨¬Ý¤£¨£¥Ø¼Ð®É(³Ì±`µo¥Í¦b $b )²£¥Í¤@°ï bug log 2022/11/14
		//bug( "Mpgoto - No such location: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( ch->fighting != NULL )
		stop_fighting( ch, TRUE );

	char_from_room( ch );
	char_to_room( ch, location );

	return;
}

/* lets the mobile do a command at another location. Very useful */

void do_mpat( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	ROOM_INDEX_DATA *location;
	ROOM_INDEX_DATA *original;
	CHAR_DATA *victim, *wch;
	int ori_pos;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Mpat - Bad argument: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( is_number( arg ) )
	{
		if ( ( location = find_location( ch, arg ) ) == NULL )
		{
			bug( "Mpat is_number - No such location: vnum %d.", ch->pIndexData->vnum );
			return;
		}
	}
	else
	{
		if ( ( wch = get_char_world( ch, arg ) ) )
			location = wch->in_room;
		else
		{
			bug( "Mpat get_char_world - No such location: vnum %d.", ch->pIndexData->vnum );
			return;
		}
	}

	original = ch->in_room;
	ori_pos  = ch->position;
	ch->position = POS_STANDING;
	victim = ch->fighting;
	char_from_room2( ch );
	char_to_room( ch, location );
	interpret( ch, argument );

	/*
	* See if 'ch' still exists before continuing!
	* Handles 'at XXXX quit' case.
	*/
	if( ch->deleted ) return;
	char_from_room2( ch );
	char_to_room( ch, original );
	ch->fighting = victim;
	ch->position = ori_pos;
	update_pos( ch );

	return;
}
 
/* lets the mobile transfer people.  the all argument transfers
   everyone in the current room to the specified location */

void do_mptransfer( CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *target, *target_next_in_room;
	int dir = -1;
	EXIT_DATA *pexit;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' )
	{
		bug( "Mptransfer - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	/*
	* Thanks to Grodyn for the optional location parameter.
	*/
	if ( arg2[0] == '\0' )
	{
		location = ch->in_room;
	}
	else
	{
		if ( !str_prefix(arg2,"up"))
		{
			dir = 4;
		}
		else if( !str_prefix(arg2,"down"))
		{
			dir = 5;
		}
		else if( !str_prefix(arg2,"north"))
		{
			dir = 0;
		}
		else if( !str_prefix(arg2,"east"))
		{
			dir = 1;
		}
		else if( !str_prefix(arg2,"south"))
		{
			dir = 2;
		}
		else if( !str_prefix(arg2,"west"))
		{
			dir = 3;
		}

		if ( dir == -1 && ( location = find_location( ch, arg2 ) ) == NULL )
		{
			bug( "Mptransfer - No such location: vnum %d.", ch->pIndexData->vnum );
			return;
		}

		if ( dir != -1 )
		if ( !( pexit = ch->in_room->exit[dir] ) || !( location = pexit->to_room ) )
		{
			// send_to_char( "[33m«u§r¡A¦¹¸ô¤£³q¡C[m\n\r", ch );
			return;
		}


		if ( location == ch->in_room )
		{
			//bug( "Mptransfer - Transfer to the same room: vnum %d.", ch->pIndexData->vnum );
			return;
		}

		/* ²¾°£ mptransfer private ³]©w 2022/12/20
		if ( room_is_private( ch , location ) )
		{
			bug( "Mptransfer - Private room: vnum %d.", ch->pIndexData->vnum );
			return;
		} */
	}

	if ( !str_cmp( arg1, "pc" ) )
	{
		for ( target = ch->in_room->people; target; target = target_next_in_room )
		{
			target_next_in_room = target->next_in_room;
			if( IS_NPC(target) ) continue;
			if ( target != ch && can_see( ch, target ) )
			{
				if ( target->in_room == NULL )
				{
					bug( "Mptransfer - Victim in Limbo: vnum %d.", ch->pIndexData->vnum );
					return;
				}
				if( target->in_room->progtypes & ROOM_LEAVE)
					rprog_leave_trigger(target->in_room, target, 8);
				char_from_room( target );
				char_to_room( target, location );
				if( location->progtypes & ROOM_ENTER)
					rprog_entry_trigger(location, target, 6);
				if ( target->position > POS_DEAD ) // Added by Amethyst
					mprog_greet_trigger( target );
			}
		}
		return;
	}

	if ( !str_cmp( arg1, "all" ) )
	{
		for ( target = ch->in_room->people; target; target = target_next_in_room )
		{
			target_next_in_room = target->next_in_room;
			if ( target != ch && can_see( ch, target ) )
			{
				if ( target->in_room == NULL )
				{
					bug( "Mptransfer - Victim in Limbo: vnum %d.", ch->pIndexData->vnum );
					return;
				}
				if( target->in_room->progtypes & ROOM_LEAVE)
					rprog_leave_trigger(target->in_room, target, 8);
				char_from_room( target );
				char_to_room( target, location );
				if( location->progtypes & ROOM_ENTER)
					rprog_entry_trigger(location, target, 6);
				if ( target->position > POS_DEAD ) // Added by Amethyst
					mprog_greet_trigger( target );
			}
		}
		return;
	}

	if ( ( target = get_char_world( ch, arg1 ) ) == NULL )
	{
		bug( "Mptransfer - No such person: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( target->in_room == NULL )
	{
		bug( "Mptransfer - Victim in Limbo: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if( target->in_room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger(target->in_room, target, 8);
	char_from_room( target );
	char_to_room( target, location );
	if( location->progtypes & ROOM_ENTER)
		rprog_entry_trigger(location, target, 6);
	if ( target->position > POS_DEAD ) // Added by Amethyst
		mprog_greet_trigger( target );

	return;
}


//use to modify npc
void do_mpmodify( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	unsigned long int value;
	unsigned long long value_long; // ¼W¥[ long long value ­È 2023/01/10

	if( !IS_NPC(ch) )
	{
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );
	argument = one_argument( argument, opr );
	if ( arg[0] == '\0' || opr[0] == '\0' || opr[1] != '\0' || argument[0] == '\0' )
	{
		bug( "Mpmodified - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	value = strtoul(argument, (char **)NULL, 10);


	// °w¹ï­×§ï¼Æ­È·|¶W¹L int ªº¼W¥[¥H long long ¨Ó§P©w¼Æ­È 2023/01/10
	if ( !str_cmp( arg, "act" ) )
	{
		//ch->act = qmark_opr( ch->act, opr, value );
		value_long = strtoul(argument, (char **)NULL, 10);
		ch->act = ll_qmark_opr( ch->act, opr, value_long, "mpmodify_act" );
	}
	if ( !str_cmp( arg, "aff" ) )
	{
		//ch->affected_by = qmark_opr( ch->affected_by, opr, value );
		value_long = strtoul(argument, (char **)NULL, 10);
		ch->affected_by = ll_qmark_opr( ch->affected_by, opr, value_long, "mpmodify_aff" );
	}
	if ( !str_cmp( arg, "special" ) )
	{
		//ch->special = qmark_opr( ch->special, opr, value );
		value_long = strtoul(argument, (char **)NULL, 10);
		ch->special = ll_qmark_opr( ch->special, opr, value_long, "mpmodify_special" );
	}

	if ( !str_cmp( arg, "hp" ) )
	{
		ch->max_hit = qmark_opr( ch->max_hit, opr, value, "mpmodify_hp" );
	}
	if ( !str_cmp( arg, "mana" ) )
	{
		ch->max_mana = qmark_opr( ch->max_mana, opr, value, "mpmodify_mana" );
	}
	if ( !str_cmp( arg, "move" ) )
	{
		ch->max_move = qmark_opr( ch->max_move, opr, value, "mpmodify_move" );
	}
	if ( !str_cmp( arg, "class" ) )
	{
		ch->class = qmark_opr( ch->class, opr, value, "mpmodify_class" );
	}
	if ( !str_cmp( arg, "hr" ) )
	{
		ch->hitroll = qmark_opr( ch->hitroll, opr, value, "mpmodify_hr" );
	}
	if ( !str_cmp( arg, "dr" ) )
	{
		ch->damroll = qmark_opr( ch->damroll, opr, value, "mpmodify_dr" );
	}
	if ( !str_cmp( arg, "ac" ) )
	{
		ch->armor = qmark_opr( ch->armor, opr, value, "mpmodify_ac" );
	}
	if ( !str_cmp( arg, "dc" ) )
	{
		ch->dodge = qmark_opr( ch->dodge, opr, value, "mpmodify_dc" );
	}
	if ( !str_cmp( arg, "pp" ) )
	{
		ch->pp = qmark_opr( ch->pp, opr, value, "mpmodify_pp" );
	}
	if ( !str_cmp( arg, "mdr" ) )
	{
		ch->mdr = qmark_opr( ch->mdr, opr, value, "mpmodify_mdr" );
	}
	if ( !str_cmp( arg, "pdr" ) )
	{
		ch->pdr = qmark_opr( ch->pdr, opr, value, "mpmodify_pdr" );
	}
	if ( !str_cmp( arg, "amdr" ) )
	{
		ch->amdr = qmark_opr( ch->amdr, opr, value, "mpmodify_amdr" );
	}
	if ( !str_cmp( arg, "apdr" ) )
	{
		ch->apdr = qmark_opr( ch->apdr, opr, value, "mpmodify_apdr" );
	}
	if ( !str_cmp( arg, "align") )
	{
		ch->alignment = qmark_opr( ch->alignment, opr, value, "mpmodify_align" );
		if ( ch->alignment > 1000 )
			ch->alignment = 1000;
		else if (ch->alignment < -1000 )
			ch->alignment = -1000;
	}
	if ( !str_cmp( arg, "level") )
	{
		ch->level = qmark_opr( ch->level, opr, value, "mpmodify_level" );
		if ( ch->level > 300 )
			ch->level = 300;
		else if (ch->level < 1 )
			ch->level = 1;
	}

	// °w¹ï max hp mana mv ¤p©ó 0 ªº±¡ªp­×§ï¬° 1, °O¿ý bug log 2022/12/30
	if( ch->max_hit <= 0 ){
		ch->max_hit = 1;
		bug( "Mpmodify : Max_hit <= 0 vnum %d.", ch->pIndexData->vnum );
	}
	if( ch->max_mana <= 0 ){
		ch->max_mana = 1;
		bug( "Mpmodify : Max_mana <= 0 vnum %d.", ch->pIndexData->vnum );
	}
	if( ch->max_move <= 0 ){
		ch->max_move = 1;
		bug( "Mpmodify : Max_move <= 0 vnum %d.", ch->pIndexData->vnum );
	}

	/*
	   if ( !str_cmp( arg, "pad" ) )
	   {
	   if ( arg1 == '+' )
	   ch->pad += value;
	   if ( arg1 == '-' )
	   ch->pad -= value;
	   if ( arg1 == '=' )
	   ch->pad = value;
	   }
	   if ( !str_cmp( arg, "mad" ) )
	   {
	   if ( arg1 == '+' )
	   ch->mad += value;
	   if ( arg1 == '-' )
	   ch->mad -= value;
	   if ( arg1 == '=' )
	   ch->mad = value;
	   }
	*/
}

void do_mpforce( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];

	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Mpforce - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( !str_cmp( arg, "all" ) )
	{
		CHAR_DATA *vch;
		CHAR_DATA *vch_next;

		for ( vch = char_list; vch != NULL; vch = vch_next )
		{
			vch_next = vch->next;
			if ( !vch->in_room || vch->deleted )
				continue;

			if ( vch->in_room == ch->in_room
				&& get_trust( vch ) < get_trust( ch ) 
				&& can_see( ch, vch ) )
			{
				vch->forced = TRUE; // mpforce ¯à¦¨¥ß forced 2022/10/27
				interpret( vch, argument );
			}
		}
	}
	else
	{
		CHAR_DATA *target;

		if ( ( target = get_char_room( ch, arg ) ) == NULL )
		{
			/*bug( "Mpforce - No such target: vnum %d.",
			ch->pIndexData->vnum );
			*/
			return;
		}

		if ( target == ch )
		{
			bug( "Mpforce - Forcing oneself: vnum %d.",
			ch->pIndexData->vnum );
			return;
		}
		target->forced = TRUE; // mpforce ¯à¦¨¥ß forced 2022/10/27
		interpret( target, argument );
	}

	return;
}

void do_mpforcer( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *room = NULL;
	char arg[ MAX_INPUT_LENGTH ];

	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	// 	send_to_char( "¤°»ò¡H\n\r", ch );
	// 	return;
	// }

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Mpforcer - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if( !str_cmp( arg, "here" ))
		room = ch->in_room;
	else if( !is_number(arg)) {
		// bug log add mob vnum  2020/11/05
		bug( "Mpforcer - Force which room to do what? mob : %d ", ch->pIndexData->vnum );
		return;
	}
	else
	{
		room = get_room_index( atoi(arg));
		if( !room ) {
			// bug log add mob vnum  2020/11/05
			bug("Mpforcer - No such location. mob : %d", ch->pIndexData->vnum);;
			return;
		}
	}
	rinterpret( room, ch,argument );

	return;
}

void do_mpcallout( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	int timer;
	CALL_OUT_DATA *cod;
	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	argument = one_argument(argument, arg);
	if( !is_number(arg)) {
		bugf("mpcallout - argument 1 must be timer. vnum %d", ch->pIndexData->vnum);
		return;
	}
	timer = atoi(arg);

	if( timer < 1 ) {
		bugf("mpcallout - timer must > 0. vnum %d", ch->pIndexData->vnum);
		return;
	}

	cod = new_call_out();
	cod->owner = ch;
	cod->timer = timer;
	cod->cmd = str_dup(argument);
	add_call_out( cod);

	return;
}

void do_mpcall( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char fun[MAX_STRING_LENGTH];
	char percent[MAX_STRING_LENGTH];
	CHAR_DATA *vch;
	CHAR_DATA *vch_next; // add at 2022/01/17
	int per = 100;

	if( !IS_NPC(ch)){
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
	}
	argument = one_argument(argument , fun);
	argument = one_argument(argument , arg);
	argument = one_argument(argument , percent);

	if( fun[0] == '\0' ) {
		bugf("mpcall : no function name in %d", ch->pIndexData->vnum);
		return;
	}

	// ·s¼W mpcall °w¹ï©Ð¶¡¤º pc , npc , all ªº®æ¦¡ 2022/01/17
	if( !str_cmp( arg, "all" ) )
	{
		for ( vch = ch->in_room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( vch == ch ) continue; // mob ¤£·|¹ï¦Û¤v§@ mpcall
			if( vch->in_room != ch->in_room ) continue;
			if( !can_see(ch, vch) ) continue;
			mprog_defun_trigger( fun, vch, ch);
		}
		return;
	}
	else if( !str_cmp( arg, "pc" ) )
	{
		for ( vch = ch->in_room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( vch == ch ) continue; // mob ¤£·|¹ï¦Û¤v§@ mpcall
			if( vch->in_room != ch->in_room ) continue;
			if( !can_see(ch, vch) ) continue;
			if( IS_NPC(vch) ) continue;
			mprog_defun_trigger( fun, vch, ch);
		}
		return;
	}
	else if( !str_cmp( arg, "npc" ) )
	{
		for ( vch = ch->in_room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( vch == ch ) continue; // mob ¤£·|¹ï¦Û¤v§@ mpcall
			if( vch->in_room != ch->in_room ) continue;
			if( !can_see(ch, vch) ) continue;
			if( !IS_NPC(vch) ) continue;
			mprog_defun_trigger( fun, vch, ch);
		}
		return;
	}
	//if( !( vch = get_char_room( ch, arg) ) || !can_see(ch, vch)) {
	else if( !( vch = get_char_room( ch, arg) ) || !can_see(ch, vch)) {
		bugf("mpcall: no such char %s here with %s", arg,ch->name);
		return;
	}

	if( is_number( percent ) ){
		per = atoi(percent);
		per = URANGE( 0, per, 100);
	}
	if( per == 100 || number_percent() < per ) {
		mprog_defun_trigger( fun, vch, ch);
	}
}

void do_mplog( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	sprintf( buf, "MPLOG %s:", ch->name );
	strcat( buf, argument );
	log_string( buf );
}

void do_mpdef_rembch( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *vch;

	if ( !IS_NPC( ch ) )
	{
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
	}

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		bug( "Mpdef_rembch - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	if ( !str_cmp( "null-tar", arg ) )
	{
		if ( ( vch = ch->rembch ) )
			stop_rember(ch, vch);
		ch->rembch = NULL;
	}
	else
	{
		if ( !( vch = get_char_room( ch, arg ) ) )
		{
			// ¥i¯à³y¦¨ mob ¤H³]½Ä¬ð¡A¬G²¾°£±Ô­z 2022/07/12
			//do_say( ch, "½Ö¦b§ä§Ú¡H§Ú²{¦b¤°»ò³£¬Ý¤£¨ì¡I" );
			return;
		}
		add_rember(ch, vch);
	}
}

void do_mpdis_magic( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	CHAR_DATA *vch;
	AFFECT_DATA *paf;
	int sn;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	/*
		if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
		{
			send_to_char( "¤°»ò¡H\n\r", ch );
			return;
		}
	*/

	argument = one_argument( argument, arg );
	one_argument( argument, arg1 );


	if( !str_cmp(arg, "self") )
	{
		vch = ch;
	}
	else if ( !( vch = get_char_room( ch, arg ) ) )
	{
		// ²¾°£¾Ô°« bug log 2022/11/21
		//bug( "mpdis_magic - victim not in room: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if ( !str_cmp( arg1, "all" ) )
	{
		if ( vch->affected ){
			for ( paf = vch->affected; paf; paf = paf->next )
				affect_strip( vch, paf->type );
		}
	}else if( !str_cmp( arg1, "random" ) ){  // ÀH¾÷¥h°£¥Ø¼Ð¨­¤Wªº buff 2022/07/12
		if ( vch->affected ){
			for ( paf = vch->affected; paf; paf = paf->next ){
				if ( number_percent() >= 25 )
					continue;
				affect_strip( vch, paf->type );
			}
		}
	}
	else
	{
		sn = skill_lookup( arg1 );
		if ( vch->affected )
			affect_strip( vch, sn );
	}
}


void do_mpgreward( CHAR_DATA *ch, char *argument )
{
	char type[MAX_INPUT_LENGTH];
	char ppl[MAX_INPUT_LENGTH];
	char level[MAX_INPUT_LENGTH];
	char value[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int v;
	int lv;
	//int sn;
	CHAR_DATA *gch = NULL, *victim;

	if( !ch->in_room ) return;
	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	argument = one_argument( argument, type);
	argument = one_argument( argument, ppl);
	argument = one_argument( argument, level);
	argument = one_argument( argument, value);

	if( (!is_number(level) &&  str_cmp(level, "basic") ) || !is_number(value) ) {
		bug("Mpgreward: bad level or value : %d", ch->pIndexData->vnum);
		return;
	}

	if( !( victim = get_char_room( ch, ppl ))) {
		bug("Mpgreward: victim not found :%d", ch->pIndexData->vnum);
		return;
	}

	lv = atoi(level);
	v = atoi(value);

	if( !victim->in_room ) {
		bug("Mpgreward: victim no in_room !", 0);
		bug("victim is",0);
		bug( ppl,0);
		return;
	}

	if( !str_cmp(type, "exp")){
		for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
			if( IS_NPC(gch) ) continue;
			if( is_same_group( victim, gch )){
				if( ((!str_cmp(level, "basic")) && gch->class < 5 ) 
					|| (lv < 0 && gch->level + lv < 1 )
					|| (lv > 0 && gch->level - lv > -1 ))
				{
					gain_exp(gch, v);
					sprintf(buf,"§Aªº¸gÅç­È%s¤F %d ÂI¡I\n\r",
							(v < 0) ? "´î¤Ö" : "¼W¥[",
							v);
					send_to_char(buf, gch);
				}
			}
		}
	}else if( !str_cmp(type, "skexp")){
		for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
			if( IS_NPC(gch) ) continue;
			if( is_same_group( victim, gch )){
				if( ((!str_cmp(level, "basic")) && gch->class < 5 ) 
					|| (lv < 0 && gch->level + lv < 1 )
					|| (lv > 0 && gch->level - lv > -1 ))
				{
					gain_skill_exp(gch, v);
					sprintf(buf,"§Aªº§Þ¯à¸gÅç­È%s¤F %d ÂI¡I\n\r",
							(v < 0) ? "´î¤Ö" : "¼W¥[",
							v);
					send_to_char(buf, gch);
				}
			}
		}
	}else if( !str_cmp(type, "merit")){
		for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
			if( IS_NPC(gch) ) continue;
			if( is_same_group( victim, gch )){
				if( ((!str_cmp(level, "basic")) && gch->class < 5 ) 
					|| (lv < 0 && gch->level + lv < 1 )
					|| (lv > 0 && gch->level - lv > -1 ))
				{
					gain_merit(gch, v);
				}
			}
		}
	}else if ( !str_cmp(type, "gold")) {
		for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
			if( IS_NPC(gch) ) continue;
			if( is_same_group( victim, gch )){
				if( ((!str_cmp(level, "basic")) && gch->class < 5 ) 
					||  (lv < 0 && gch->level + lv < 1 )
					||  (lv > 0 && gch->level - lv > -1 ))
				{
					gch->gold += v;
					sprintf(buf,"§Aªºª÷¿ú%s¤F %d ¤¸¡I\n\r",
							(v < 0) ? "´î¤Ö" : "¼W¥[",
							v);
					send_to_char(buf, gch);
				}
			}
		}
	}else if ( !str_cmp(type, "resp")){
		for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
			if( IS_NPC(gch) ) continue;
			if( is_same_group( victim, gch )){
				if( ((!str_cmp(level, "basic")) && gch->class < 5 ) 
					|| (lv < 0 && gch->level + lv < 1 )
					|| (lv > 0 && gch->level - lv > -1 ))
				{
					gch->pcdata->respect += v;
					sprintf(buf,"§Aªº«Â±æ%s¤F %d ÂI¡I\n\r",
							(v < 0) ? "¤U­°" : "´£°ª",
							v);
					send_to_char(buf, gch);
				}
			}
		}
	}else if (!str_cmp(type, "align")) {
		for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
			if( IS_NPC(gch) ) continue;
			if( is_same_group( victim, gch )){
				if( ((!str_cmp(level, "basic")) && gch->class < 5 ) 
					|| (lv < 0 && gch->level + lv < 1 )
					|| (lv > 0 && gch->level - lv > -1 ))
				{
					if( gch->alignment + v > 1000 )
					{
						  v = 1000 - gch->alignment;
					}
					if( gch->alignment + v < -1000 )
					{
						  v = -1000 - gch->alignment;
					}
					gch->alignment += v;
					sprintf(buf,"§Aªº°}Àç%s¤F %d ÂI¡I\n\r",
							(v < 0) ? "¤U­°" : "¤W¤É",
							v);
					send_to_char(buf, gch);
				}
			}
		}
	}
	else
	{
		bug("Mpgreward: unknown type : %d", ch->pIndexData->vnum);
		return;
	}
	return;
}

void do_mpgroup( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	CHAR_DATA *lch, *vch;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	if ( arg[0] == '\0' || arg1[0] == '\0' )
	{
		bug( "Mpgroup - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	if ( !( vch = get_char_room( ch, arg1 ) ) || !( lch = get_char_room( ch, arg ) ) )
	{
		bug( "Mpgroup - target not exist: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	if ( IS_AFFECTED( vch, AFF_CHARM ) && vch->master )
	{
		bug( "Mpgroup - target has been charmed: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	if( vch == lch ) return;
	if ( vch->group)
	{
		remove_member(vch);
	}
	// if ( lch->leader )
	//     lch->leader = NULL;
	if( !lch->group ){
		do_group(lch, "form");
	}

	add_follower( vch, lch );
	//vch->leader = lch;
	act( "[36m$N¥[¤J§Aªº¶¤¥î¡C[m", lch, NULL, vch, TO_CHAR    );
	act( "[36m§A¥[¤J$nªº¶¤¥î¡C[m", lch, NULL, vch, TO_VICT    );
	act( "[36m$N¥[¤J$nªº¶¤¥î¡C[m", lch, NULL, vch, TO_NOTVICT );
	add_member(lch, vch);

	if ( is_number( arg2 ) && atoi( arg2 ) != 0 )
	{
		//vch->next_in_protect = lch->protector;
		//lch->protector = vch;
		//vch->protecting = lch;
		add_protector(vch, lch);
	}
}

/*
void do_mpgroup( CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH ];
    char arg1[ MAX_INPUT_LENGTH ];
    char arg2[ MAX_INPUT_LENGTH ];
    CHAR_DATA *lch, *vch;

    if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
    }
    
    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
    //{
//	send_to_char( "¤°»ò¡H\n\r", ch );
//	return;
  //  }

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

    if ( arg[0] == '\0' || arg1[0] == '\0' )
    {
	bug( "Mpgroup - Bad syntax: vnum %d.", ch->pIndexData->vnum );
	return;
    }
    if ( !( vch = get_char_room( ch, arg1 ) ) || !( lch = get_char_room( ch, arg ) ) )
    {
	bug( "Mpgroup - target not exist: vnum %d.", ch->pIndexData->vnum );
	return;
    }
    if ( IS_AFFECTED( vch, AFF_CHARM ) && vch->master )
    {
	bug( "Mpgroup - target has been charmed: vnum %d.", ch->pIndexData->vnum );
	return;
    }
    if( vch == lch ) return;
    if ( vch->master )
    {
	if ( vch->master == lch )
		return;
	else
		stop_follower( vch );
    }
    if ( lch->leader )//== vch )
	lch->leader = NULL;

    add_follower( vch, lch );
    vch->leader = lch;
    act( "[36m$N¥[¤J§Aªº¶¤¥î¡C[m", lch, NULL, vch, TO_CHAR    );
    act( "[36m§A¥[¤J$nªº¶¤¥î¡C[m", lch, NULL, vch, TO_VICT    );
    act( "[36m$N¥[¤J$nªº¶¤¥î¡C[m", lch, NULL, vch, TO_NOTVICT );

	if ( is_number( arg2 ) && atoi( arg2 ) != 0 )
	{
		//vch->next_in_protect = lch->protector;
		//lch->protector = vch;
		//vch->protecting = lch;	
		add_protector(vch, lch);
	}
}*/

QMARK * qmark_list, * qmark_free;

void do_add_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;

	//¥l³êÃ~·|¦Û¤v¥[ qmark
	//Ãdª«¤]·|¦Û¤v¥[ qmark¡A¦Ó¥BÃdª«¬O charmed ªº    
	//¬°¤F spell armageddon, ³s ppl ³£¥i¥H¦Û¤v¤W qmark.
	/*
		if ( !IS_NPC( ch ) )
		{
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
		}
	*/

	smash_tilde(argument);
	argument = one_argument( argument, arg );

	if( !str_cmp(arg, "self") )
		target = ch;
	else if ( !( target = get_char_room( ch, arg ) ) )
	{
		bugf( "add_qmark - no(can't see) target: name %s.", ch->name );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' && ch->pIndexData )
	{
		// bug( "mpadd_qmark - no mark_name string: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	// smash_tilde( arg );

	if ( is_qmark( target, arg ) )
	{
		// bug( "mpadd_qmark - qmark exist already: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	qmark = set_qmark( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		// bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->timer = -1;
	}
	else
		qmark->timer = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		// bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->v0 = 0;
	}
	else
		qmark->v0 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		// bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->v1 = 0;
	}
	else
		qmark->v1 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		// bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->v2 = 0;
	}
	else
		qmark->v2 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		// bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->fDeath = TRUE;
	}
	else
		qmark->fDeath = ( atoi( arg ) == 0 ) ? FALSE : TRUE;

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		// bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->fSave = FALSE;
	}
	else
		qmark->fSave = ( atoi( arg ) == 0 ) ? FALSE : TRUE;

	qmark->owner = target;
	qmark->next = target->qmark;
	if ( target->qmark )
		target->qmark->prev = qmark;
	qmark->next_in_list = qmark_list;
	if ( qmark_list )
		qmark_list->prev_in_list = qmark;
	qmark_list = qmark;
	target->qmark = qmark;

	return;
}

void do_mpadd_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;

	//¥l³êÃ~·|¦Û¤v¥[ qmark
	//Ãdª«¤]·|¦Û¤v¥[ qmark¡A¦Ó¥BÃdª«¬O charmed ªº    
	//¬°¤F spell armageddon, ³s ppl ³£¥i¥H¦Û¤v¤W qmark.
	//// spell armageddon ¸Ì¥Îªº¬O do_add_qmark
	//«D npc ½Ð¤£­n¨Ï¥Î³o­Ó¨ç¦¡¡I¡I¡I¡I¡I¡I¡I¡I¡I¡I¡I

	if ( !IS_NPC( ch ) )
	{
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
	}

	smash_tilde(argument);
	argument = one_argument( argument, arg );
	if( !str_cmp(arg, "self") )
		target = ch;
	else if ( !( target = get_char_room( ch, arg ) ) )
	{
		bug( "mpadd_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' && ch->pIndexData )
	{
		bug( "mpadd_qmark - no mark_name string: vnum %d.", ch->pIndexData->vnum );
		return;
	}
	// smash_tilde( arg );

	if ( is_qmark( target, arg ) )
	{
		//bug( "mpadd_qmark - qmark exist already: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	qmark = set_qmark( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		//bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->timer = -1;
	}
	else
		qmark->timer = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		//bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->v0 = 0;
	}
	else
		qmark->v0 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		//bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->v1 = 0;
	}
	else
		qmark->v1 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		//bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->v2 = 0;
	}
	else
		qmark->v2 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		//bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->fDeath = TRUE;
	}
	else
		qmark->fDeath = ( atoi( arg ) == 0 ) ? FALSE : TRUE;

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		//bug( "mpadd_qmark - Bad syntax: vnum %d.", ch->pIndexData->vnum );
		qmark->fSave = FALSE;
	}
	else
		qmark->fSave = ( atoi( arg ) == 0 ) ? FALSE : TRUE;

	qmark->owner = target;
	qmark->next = target->qmark;
	if ( target->qmark )
		target->qmark->prev = qmark;
	qmark->next_in_list = qmark_list;
	if ( qmark_list )
		qmark_list->prev_in_list = qmark;
	qmark_list = qmark;
	target->qmark = qmark;

	return;
}

QMARK *set_qmark( char *arg )
{
	QMARK *qmark;
	char buf[MAX_INPUT_LENGTH];

	//if( qmark_free == NULL )
	qmark = new_qmark();
	/*
	else
	{
		qmark = qmark_free;
		qmark_free = qmark_free->next_in_list;
	}
	*/
	smash_tilde( arg );
	sprintf(buf, "%s",arg);
	//qmark->mark_name = str_dup( qmark_lookup(buf) );
	//  qmark->mark_name = qmark_lookup(buf);
	qmark->mark_name = str_dup( arg );
	return qmark;
}

bool check_qmark( CHAR_DATA *target, char *arg )
{
	QMARK *qmark;

	if ( !target->qmark )
		return FALSE;
		
	for( qmark = target->qmark ; qmark; qmark = qmark->next )
	{
		if( !str_cmp( qmark->mark_name , arg ) )
			return TRUE;
	}
	return FALSE;
}


void do_del_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;

	if(!ch)
	   return;  

	if(IS_NPC(ch))
	   return;


	smash_tilde(argument);
	argument = one_argument( argument, arg );

	if( !str_cmp(arg, "self") ) target = ch;
	else if ( !( target = get_char_room( ch, arg ) ) )
	{
		//bug( "mpdel_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		//bug( "mpdel_qmark - no mark_name string: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	smash_tilde( arg );
	if ( !target->qmark )
	{
		//bug( "mpdel_qmark - target has no any qmark to delete: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	for( qmark = target->qmark; qmark; qmark = qmark->next )
		if ( !str_cmp( qmark->mark_name, arg ) )
	{
		del_qmark( qmark );
		return;
	}

	/*
		if ( !str_cmp( qmark->mark_name, arg ) )
		{
			target->qmark = qmark->next;
			target->qmark->next_in_list = qmark->next->next_in_list;
			if ( qmark == qmark_list)
				qmark_list = qmark_list->next_in_list;
			qmark->next = qmark_free;
			qmark_free = qmark;
			return;
		}
		for( qmark = target->qmark; qmark->next; qmark = qmark->next )
			if ( qmark->next && !str_cmp( qmark->next->mark_name, arg ) )
			{
			qmark_tmp = qmark->next;
				qmark->next = qmark->next->next;
			qmark->next_in_list = qmark->next->next_in_list;
				qmark_tmp->next = qmark_free;
				qmark_free = qmark_tmp;	
				return;
			}
	*/

	//bug( "mpdel_qmark - target has no this qmark: vnum %d.", ch->pIndexData->vnum );
	return;
}

void do_mpdel_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;

	if ( !IS_NPC( ch ) )
	{
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
	}

	smash_tilde(argument);
	argument = one_argument( argument, arg );
	if ( !( target = get_char_room( ch, arg ) ) )
	{
		bug( "mpdel_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		bug( "mpdel_qmark - no mark_name string: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	smash_tilde( arg );
	if ( !target->qmark )
	{
		//bug( "mpdel_qmark - target has no any qmark to delete: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	for( qmark = target->qmark; qmark; qmark = qmark->next )
		if ( !str_cmp( qmark->mark_name, arg ) )
	{
		del_qmark( qmark );
		return;
	}

	/*
	if ( !str_cmp( qmark->mark_name, arg ) )
	{
		target->qmark = qmark->next;
		target->qmark->next_in_list = qmark->next->next_in_list;
		if ( qmark == qmark_list)
			qmark_list = qmark_list->next_in_list;
		qmark->next = qmark_free;
		qmark_free = qmark;
		return;
	}
	for( qmark = target->qmark; qmark->next; qmark = qmark->next )
		if ( qmark->next && !str_cmp( qmark->next->mark_name, arg ) )
		{
		qmark_tmp = qmark->next;
			qmark->next = qmark->next->next;
		qmark->next_in_list = qmark->next->next_in_list;
			qmark_tmp->next = qmark_free;
			qmark_free = qmark_tmp;	
			return;
		}
	*/
	//bug( "mpdel_qmark - target has no this qmark: vnum %d.", ch->pIndexData->vnum );
	return;
}

void del_qmark( QMARK *qmark )
{
	//free_string( qmark->mark_name ); intentionally...don't free it !!
	free_string( qmark->mark_name );
        qmark->mark_name = str_dup(""); //¥H§K¤£¤p¤ßaccess¨ì±¾
	if ( qmark->prev )
	{
		qmark->prev->next = qmark->next;
		if ( qmark->next )
			qmark->next->prev = qmark->prev;
	}
	else
	{
		if( qmark->owner )
			qmark->owner->qmark = qmark->next;
		if ( qmark->next )
			qmark->next->prev = NULL;
	}
	if ( qmark->prev_in_list )
	{
		qmark->prev_in_list->next_in_list = qmark->next_in_list;
		if ( qmark->next_in_list )
			qmark->next_in_list->prev_in_list = qmark->prev_in_list;
	}
	else if ( qmark_list == qmark )//qmark may not in qmark_list if its timer = -1
	{
		qmark_list = qmark->next_in_list;
		if ( qmark->next_in_list )
			qmark->next_in_list->prev_in_list = NULL;
	}
	qmark->owner = NULL;
	qmark->prev = NULL;
	qmark->next = NULL;
	qmark->next_in_list = qmark_free;
	qmark->prev_in_list = NULL;
	qmark_free = qmark;
}


void do_mod_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	char val[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;
	int	value;

	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	//{
	//	send_to_char( "¤°»ò¡H\n\r", ch );
	//	return;
	//  }

	if( !ch ) return;

	if(IS_NPC(ch)) return;

	smash_tilde(argument);
	argument = one_argument( argument, arg );
	if( !str_cmp(arg, "self") )
		target = ch;
	else if ( !( target = get_char_room( ch, arg ) ) )
	{
		// bug( "mpmod_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		// bug( "mpmod_qmark - no mark_name string: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	// smash_tilde( arg );
	if ( !target->qmark )
	{
		// bug( "mpdel_qmark - target has no any qmark: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if( !( qmark = is_qmark(target, arg) )) {
		return;
	}

	argument = one_argument( argument, arg );
	argument = one_argument( argument, opr );
	one_argument( argument, val );
	// ­×¥¿¦r¦ê±q 0 ¦pªG¬°ªÅªº±¡ªp¤~·|¶] bug log , §_«h¦b qmark ¦³­Èªº±¡ªp¤U·|³y¦¨¶]¤£¨ì return ¦Ó crash 2023/01/10
	//if ( arg == '\0' || opr == '\0' || val == '\0' )
	if ( arg[0] == '\0' || opr[0] == '\0' || val[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpmod_qmark - illegal operator: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	value = atoi( val );

	if ( !str_cmp( arg, "timer" ) )
	{
		qmark->timer = qmark_opr( qmark->timer, opr, value, "mod_qmark_timer" );
		return;
	}
	if ( !str_cmp( arg, "v0" ) )
	{
		qmark->v0 = qmark_opr( qmark->v0, opr, value, "mod_qmark_v0" );
		return;
	}
	if ( !str_cmp( arg, "v1" ) )
	{
		qmark->v1 = qmark_opr( qmark->v1, opr, value, "mod_qmark_v1" );
		return;
	}
	if ( !str_cmp( arg, "v2" ) )
	{
		qmark->v2 = qmark_opr( qmark->v2, opr, value, "mod_qmark_v2" );
		return;
	}
	// mod_qmark ­×§ï qmark death ¤Î save flag 2022/10/18
	if ( !str_cmp( arg, "death" ) )
	{
		qmark->fDeath = qmark_opr( qmark->fDeath, opr, value, "mod_qmark_death" );
		return;
	}
	if ( !str_cmp( arg, "save" ) )
	{
		qmark->fSave = qmark_opr( qmark->fSave, opr, value, "mod_qmark_save" );
		return;
	}
	return;
}

void do_mpmod_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	char val[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;
	int	value;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//	send_to_char( "¤°»ò¡H\n\r", ch );
	//	return;
	// }

	smash_tilde(argument);
	argument = one_argument( argument, arg );
	if( !str_cmp(arg, "self") )
		target = ch;
	else if ( !( target = get_char_room( ch, arg ) ) )
	{
		bug( "mpmod_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		bug( "mpmod_qmark - no mark_name string: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	//smash_tilde( arg );
	if ( !target->qmark )
	{
		bug( "mpmod_qmark - target has no any qmark: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	if( !( qmark = is_qmark(target, arg) )) {
		//bug( "mpmod_qmark - target has no this qmark: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	argument = one_argument( argument, opr );
	one_argument( argument, val );
	// ­×¥¿¦r¦ê±q 0 ¦pªG¬°ªÅªº±¡ªp¤~·|¶] bug log , §_«h¦b qmark ¦³­Èªº±¡ªp¤U·|³y¦¨¶]¤£¨ì return ¦Ó crash 2023/01/10
	//if ( arg == '\0' || opr == '\0' || val == '\0' )
	if ( arg[0] == '\0' || opr[0] == '\0' || val[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpmod_qmark - illegal operator: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	value = atoi( val );

	if ( !str_cmp( arg, "timer" ) )
	{
		qmark->timer = qmark_opr( qmark->timer, opr, value, "mpmod_qmark_timer" );
		return;
	}
	if ( !str_cmp( arg, "v0" ) )
	{
		qmark->v0 = qmark_opr( qmark->v0, opr, value, "mpmod_qmark_v0" );
		return;
	}
	if ( !str_cmp( arg, "v1" ) )
	{
		qmark->v1 = qmark_opr( qmark->v1, opr, value, "mpmod_qmark_v1" );
		return;
	}
	if ( !str_cmp( arg, "v2" ) )
	{
		qmark->v2 = qmark_opr( qmark->v2, opr, value, "mpmod_qmark_v2" );
		return;
	}
	// mpmod_qmark ­×§ï qmark death ¤Î save flag 2022/10/18
	if ( !str_cmp( arg, "death" ) )
	{
		qmark->fDeath = qmark_opr( qmark->fDeath, opr, value, "mpmod_qmark_death" );
		return;
	}
	if ( !str_cmp( arg, "save" ) )
	{
		qmark->fSave = qmark_opr( qmark->fSave, opr, value, "mpmod_qmark_save" );
		return;
	}
	bug( "mpmod_qmark - illegal operator: vnum %d.", ch->pIndexData->vnum );
	return;
}

//int qmark_opr( int lhs, char *opr, int rhs )
int qmark_opr( int lhs, char *opr, int rhs, char *mod_name )
{

	if ( !str_cmp( opr, "+" ) )
		return ( lhs + rhs );
	if ( !str_cmp( opr, "-" ) )
		return ( lhs - rhs );
	if ( !str_cmp( opr, "&" ) )
		return ( lhs & rhs );
	if ( !str_cmp( opr, "|" ) )
		return ( lhs | rhs );
	if ( !str_cmp( opr, "r" ) )
		return ( lhs & ~rhs );
	if ( !str_cmp( opr, "=" ) )
		return rhs;

	// ­×§ï bug log Åã¥Ü¤º®e 2022/11/14
	char buf[ MAX_INPUT_LENGTH ];
	sprintf( buf, "Improper qmark operator: lhs:%d opr:%s rhs:%d type:%s.",lhs , opr , rhs , mod_name );
	bug( buf , 0 );

	//bug ( "Improper qmark operator\n\r", 0 );
	return lhs;

}

// ¼W¥[¼Æ­È¸û¤jªº§P©w 2023/01/10
long long ll_qmark_opr( long long lhs, char *opr, long long rhs, char *mod_name )
{

	if ( !str_cmp( opr, "+" ) )
		return ( lhs + rhs );
	if ( !str_cmp( opr, "-" ) )
		return ( lhs - rhs );
	if ( !str_cmp( opr, "&" ) )
		return ( lhs & rhs );
	if ( !str_cmp( opr, "|" ) )
		return ( lhs | rhs );
	if ( !str_cmp( opr, "r" ) )
		return ( lhs & ~rhs );
	if ( !str_cmp( opr, "=" ) )
		return rhs;

	char buf[ MAX_INPUT_LENGTH ];
	sprintf( buf, "Improper long long qmark operator: lhs:%d opr:%s rhs:%d type:%s.",lhs , opr , rhs , mod_name );
	bug( buf , 0 );

	return lhs;
}


void do_mpadd_all_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char buf[ MAX_INPUT_LENGTH ];
	CHAR_DATA *lch, *vch;

    if( !IS_NPC(ch) )
    {
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
    }

	argument = one_argument( argument, arg );

    smash_tilde(argument);
    if( !str_cmp(arg, "self") ) lch = ch;
    else if ( !( lch = get_char_room( ch, arg ) ) )
    {
		bug( "mpadd_all_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
    }

	for( vch = lch->in_room->people; vch; vch = vch->next_in_room )
		if ( is_same_group( lch, vch ) )
		{
			sprintf( buf, "'%s' ", vch->name );
			strcat( buf, argument );
			do_mpadd_qmark( ch, buf );
		}
}

void do_mpmod_all_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char buf[ MAX_INPUT_LENGTH ];
	CHAR_DATA *lch, *vch;
	//char opr[ MAX_INPUT_LENGTH ];
	//char val[ MAX_INPUT_LENGTH ];
	//QMARK *qmark;
	//int	value;

    if( !IS_NPC(ch) )
    {
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
    }

    smash_tilde(argument);
	argument = one_argument( argument, arg );

    if( !str_cmp(arg, "self") ) lch = ch;
    else if ( !( lch = get_char_room( ch, arg ) ) )
    {
		bug( "mpmod_all_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
    }

	for( vch = lch->in_room->people; vch; vch = vch->next_in_room )
		if ( is_same_group( lch, vch ) )
		{
			sprintf( buf, "'%s' ", vch->name );
			strcat( buf, argument );
			do_mpmod_qmark( ch, buf );
 		}
}

void do_mpdel_all_qmark( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char buf[ MAX_INPUT_LENGTH ];
	CHAR_DATA *lch, *vch;

    if( !IS_NPC(ch) )
    {
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
    }

    smash_tilde(argument);
	argument = one_argument( argument, arg );

    if( !str_cmp(arg, "self") ) lch = ch;
    else if ( !( lch = get_char_room( ch, arg ) ) )
    {
		bug( "mpadd_all_qmark - no(can't see) target: vnum %d.", ch->pIndexData->vnum );
		return;
    }

	for( vch = lch->in_room->people; vch; vch = vch->next_in_room )
		if ( is_same_group( lch, vch ) )
		{
			sprintf( buf, "'%s' ", vch->name );
			strcat( buf, argument );
			do_mpdel_qmark( ch, buf );
		}
}


void do_mpgold( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int	value;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	//{
	//	send_to_char( "¤°»ò¡H\n\r", ch );
	//	return;
	//  }

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			target->gold += value;
			if (value >=0 )
			{
				sprintf( arg, "%s µ¹§A %d ª÷¹ô¡C\n\r", ch->short_descr, value );
				act( "$n µ¹¤F $N ¤@¨Çª÷¹ô¡C", ch, NULL, target, TO_NOTVICT );
				send_to_char( arg, target );
			}
			else
			{
				int temp=0-value;
				sprintf( arg, "%s ®³¨«§A %d ª÷¹ô¡C\n\r", ch->short_descr, temp );
				act( "$n ®³¨«¤F $N ¤@¨Çª÷¹ô¡C", ch, NULL, target, TO_NOTVICT );
				send_to_char( arg, target ); 
			}
			// act( "$n µ¹¤F $N ¤@¨Çª÷¹ô¡C", ch, NULL, target, TO_NOTVICT );
			return;
		}
	}
	bug ( "Mob: %d 'mpgold' bad syntax", ch->pIndexData->vnum );
	return;
}

/**
 * The mob prog for giving merit to ppl.
 *
 * @param ch
 *     the pointer to ppl who will receive the merit.
 * @param argument
 *     the command argument used to specify the amount of merit.
 *
 * @author keelar
 */
void do_mpmerit( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_room( ch, arg ) ) ) {
		//argument = one_argument( argument, arg );
		one_argument(argument, arg);
		if ( is_number( arg ) ) {
			value = atoi( arg );
			gain_merit( target, value );
			return;
		}
	}
	bug ( "Mob: %d 'mpmerit' bad syntax", ch->pIndexData->vnum );
	return;
}

void do_mpskillexp( CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH ];
    CHAR_DATA *target;
    int	value;
    
    if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
    }
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }
    
    argument = one_argument( argument, arg );

    if ( ( target = get_char_room( ch, arg ) ) )
    {
		//argument = one_argument( argument, arg );
        one_argument(argument, arg);
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			if ( value > 0 )
					sprintf( arg, "§A±o¨ì %d ÂI§Þ¯à¸gÅç¡C\n\r", value );
				else
			{
				if ( target->exp < abs( value ) ){
					value = 1000 - target->exp;
				}
					sprintf( arg, "§A¥¢¥h %d ÂI§Þ¯à¸gÅç¡C\n\r",abs( value ) );
				}
				send_to_char( arg, target );
				gain_skill_exp( target, value );
				return;
			}
		}
    bug ( "Mob: %d 'mpskillexp' bad syntax", ch->pIndexData->vnum );
    return;
}

void do_mpexp( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int	value;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			if ( value > 0 )
				sprintf( arg, "§A±o¨ì %d ÂI¸gÅç¡C\n\r", value );
			else
			{
				if ( target->exp < abs( value ) ){
				value = 1000 - target->exp;
			}
				sprintf( arg, "§A¥¢¥h %d ÂI¸gÅç¡C\n\r", abs( value ) );
			}
			send_to_char( arg, target );
			gain_exp( target, value );
			return;
		}
	}
	bug ( "Mob: %d 'mpexp' bad syntax", ch->pIndexData->vnum );
	return;
}

void do_mppractice( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int	value;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		if ( IS_NPC( target ) )
			return;
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			target->practice += value;
			if ( value > 0 )
				sprintf( arg, "§Aªº½m²ßÂI¼Æ´£¤É¤F %d ÂI¡C\n\r", value );
			else
				sprintf( arg, "§Aªº½m²ßÂI¼Æ­°§C %d ÂI¡C\n\r", abs( value ) );
			send_to_char( arg, target );
			return;
		}
	}
	bug ( "Mob: %d 'mppractice' bad syntax", ch->pIndexData->vnum );
	return;
}

void do_mpcexp( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int	value;

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		if( !target->clan )
			return;

		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			if ( value > 0 )
					sprintf( arg, "§AÀ°§Aªº²ÕÂ´±o¨ì %d ÂIµo®iÂI¼Æ¡C\n\r", value );
				else
					sprintf( arg, "§Aªº²ÕÂ´¥¢¥h %d ÂIµo®iÂI¼Æ¡C\n\r", abs( value ) );
			send_to_char( arg, target );
			target->clan->score += value;
			return;
		}
	}
	bug ( "Mob: %d 'mpexp' bad syntax", ch->pIndexData->vnum );
	return;
}

void do_mpalign( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int	value;
	OBJ_DATA *obj, *obj_next; //Added by Razgriz 20051005

	if( !IS_NPC(ch) ){
		send_to_char("¤°»ò¡H\n\r", ch);
		return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		if ( IS_NPC( target ) )
			return;
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			target->alignment += value;
			if ( value < 1000 && value > -1000 )
			{
				if ( value > 0 )
						sprintf( arg, "§Aªº°}Àç´£¤É¤F %d ÂI¡C\n\r", value );
					else
						sprintf( arg, "§Aªº°}Àç­°§C %d ÂI¡C\n\r", abs( value ) );
				send_to_char( arg,target );
			}
			
			if ( IS_SET( race_table[ target->race ].race_abilities, RACE_VAMPIRE ) )
					target->alignment = UMIN( 0, target->alignment );

			target->alignment = URANGE( -1000, target->alignment, 1000);

			//Added by Razgriz 20051005
			for( obj = target->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ( obj->deleted ) continue;
				if ( obj->wear_loc == WEAR_NONE ) continue;
				if (  ( IS_OBJ_STAT ( obj, ITEM_ANTI_EVIL    ) && IS_EVIL    ( target ) )
					||( IS_OBJ_STAT ( obj, ITEM_ANTI_GOOD    ) && IS_GOOD    ( target ) )
					||( IS_OBJ_STAT ( obj, ITEM_ANTI_NEUTRAL ) && IS_NEUTRAL ( target ) ) )
				{
					act ( "§A³Q $p µ¹±Æ¥¸¤F.", target, obj, NULL, TO_CHAR );
					act ( "$n ³Q $p ±Æ¥¸¤F.",  target, obj, NULL, TO_ROOM );
					unequip_char  ( target, obj );
					obj_from_char ( obj );
			 
					if( IS_SET ( obj->extra_flags , ITEM_PERSONAL ) )
						obj_to_char ( obj, target );
					else
						obj_to_room ( obj, target->in_room );
					continue;
				}
			}
				// send_to_char( arg, target );
				return;
		}
	}
	bug ( "Mob: %d 'mpalign' bad syntax", ch->pIndexData->vnum );
	return;
}

void do_mpresp( CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int	value;

	if( !IS_NPC(ch) ){
	send_to_char("¤°»ò¡H\n\r", ch);
	return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//    send_to_char( "¤°»ò¡H\n\r", ch );
	//    return;
	// }

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		if ( IS_NPC( target ) )
			return;
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			target->pcdata->respect += value;
			if ( value > 0 )
				sprintf( arg, "§Aªº«Â±æ´£¤É¤F %d ÂI¡C\n\r", value );
			else
				sprintf( arg, "§Aªº«Â±æ­°§C %d ÂI¡C\n\r", abs( value ) );
			send_to_char( arg, target );
			return;
		}
	}
	bug ( "Mob: %d 'mpresp' bad syntax", ch->pIndexData->vnum );
	return;
}

void do_mpmake_horse( CHAR_DATA *ch, char *argument )
{
	return;
#if 0
    char	     arg[ MAX_INPUT_LENGTH ];
    CHAR_DATA	    *mob, *master;
    MOB_INDEX_DATA *pMobIndex;
	// MOB_ACTION *mob_action, *pmob_action;
    int			prime_attr;
    int i;
    int			snd_attr;

    if ( !IS_NPC( ch ) && !IS_IMMORTAL( ch ) )
    {
	send_to_char( "¤°»ò¡H\n\r", ch );
	return;
	}
	if ( !check_blind( ch ) )
	{
		do_say( ch, "§Ú¤°»ò³£¬Ý¤£¨£¡I" );
		return;
	}
    argument = one_argument( argument, arg );
	if ( !( master = get_char_room( ch, arg ) ) )
	{
		bug( "Mpmake_horse - Not found horse's master: vnum %d.",
			 ch->pIndexData->vnum );
		return;
	}
	if ( IS_SET( master->pcdata->ride, 1 ) )
	{
		do_say( ch, "§A¤w¸g¦³¤@¤Ç°¨¤F¡I" );
		return;
	}

    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' || !is_number(arg) )
    {
	bug( "Mpmake_horse - Bad vnum as arg: vnum %d.", ch->pIndexData->vnum );
	return;
    }
    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	bug( "Mpload_horse - Bad mob vnum: vnum %d.", ch->pIndexData->vnum );
	return;
	}
    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' || !is_number(arg) || 
    	( prime_attr = atoi(arg) ) > 5 || prime_attr < 1 )
    {
	bug( "Mpload_horse - Bad prime_attr: vnum %d.", ch->pIndexData->vnum );
	return;
    }
    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' || !is_number(arg) || 
    	( snd_attr = atoi(arg) ) > 5 || snd_attr < 1 )
    {
	bug( "Mpload_horse - Bad snd_attr: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    mob			= new_character( TRUE );

    mob->pIndexData	= pMobIndex;
    mob->name		= str_dup( pMobIndex->player_name );
    mob->short_descr	= str_dup( pMobIndex->short_descr );
    mob->long_descr	= str_dup( pMobIndex->long_descr  );
    mob->description	= str_dup( pMobIndex->description );
    mob->spec_fun	= pMobIndex->spec_fun;

	mob->pcdata->prime_attr = prime_attr;
	mob->pcdata->snd_attr = snd_attr;
	mob->pcdata->prompt = str_dup( "[m<¥Í©R[1;32m%h[mÅ]ªk[1;34m%m[m²¾°Ê[1;35m%v[m> " );
    mob->last_note			= 0;
    mob->act		= PLR_BLANK
					| PLR_COMBINE
					| PLR_PROMPT
					| PLR_AUTOEXIT
					| PLR_CHINESE
					| PLR_AUTOGOLD
					| PLR_HORSE;
    mob->pcdata->pwd			= str_dup( "" );
    mob->pcdata->bamfin			= str_dup( "" );
    mob->pcdata->bamfout			= str_dup( "" );
    mob->pcdata->immskll			= str_dup( "" );
    mob->pcdata->title			= str_dup( "" );
    mob->perm_str		= 14;
    mob->perm_int		= 10; 
    mob->perm_wis		= 10;
    mob->perm_dex		= 14;
    mob->perm_con		= 15;
    mob->pcdata->condition[COND_THIRST]	= 48;
    mob->pcdata->condition[COND_FULL]	= 48;
    mob->pcdata->pagelen			= 20;
    mob->pcdata->security		= 0;	
    mob->pcdata->ride			= 0;
    mob->pcdata->switched		= FALSE;
    mob->pcdata->horse			= master;

    mob->level		= pMobIndex->level;
    mob->exp		= 1000 * ( mob->level - 1 );
    mob->affected_by	= pMobIndex->affected_by;
    mob->alignment	= pMobIndex->alignment;
    mob->sex		= pMobIndex->sex;
    mob->race		= pMobIndex->race;
	// mob->gold = number_fuzzy( 50 )
	// * number_fuzzy( pMobIndex->level );

    mob->armor		= pMobIndex->mod_ac + race_table[ mob->race ].nature_ac;

    mob->dodge		= pMobIndex->dodge + race_table[ mob->race ].nature_dodge;

    mob->max_hit	= pMobIndex->mod_hitplus + 300;
    mob->hit		= mob->max_hit + 300;
    mob->hitroll	= pMobIndex->mod_hitroll;
    mob->damroll	= pMobIndex->mod_damroll;
    for( i = 0; i < MAX_SAVING; ++i ){
	mob->saving_throw[i]	= pMobIndex->saving_throw[i];
    }
    mob->class		= pMobIndex->class;
    mob->special	= pMobIndex->special;
    mob->psk		= pMobIndex->psk;
    mob->jcl		= pMobIndex->jcl;

    if ( ( pmob_action = pMobIndex->mob_action ) )
    	while( pmob_action)
    	{
		if( !mob_action_free )
			mob_action = (MOB_ACTION *)alloc_mem( sizeof( MOB_ACTION ) );
		else
		{
			mob_action = mob_action_free;
			mob_action_free = mob_action_free->next_in_list;
		}
		if ( !mob->mob_action )
			mob->mob_action = mob_action;
		mob_action->owner = mob;
		mob_action->com_list = pmob_action->com_list;
		mob_action->start_time = pmob_action->start_time;
		mob_action->mode = 0;
		mob_action->next = NULL;
		if ( mob_action_list )
		{
			mob_action_list->prev_in_list = mob_action;
			if ( mob_action_list->owner == mob )
				mob_action_list->next = mob_action;
		}
		mob_action->next_in_list = mob_action_list;
		mob_action_list = mob_action;
		pmob_action = pmob_action->next;
    	}

     * Insert in list.

    mob->next		= char_list;
    char_list		= mob;
    pMobIndex->count++;

    char_to_room( mob, ch->in_room );
    
    master->pcdata->horse = mob;
    master->pcdata->ride  = 1;
    add_follower( mob, master );
//    mob->leader = master;
#endif
}

void do_mpget( CHAR_DATA *ch, char *argument )
{
 	char		buf	[ MAX_STRING_LENGTH	];
 	CHAR_DATA *rch;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
 	char		arg1	[ MAX_INPUT_LENGTH	];
	char		arg2	[ MAX_INPUT_LENGTH	];
	char		arg3	[ MAX_INPUT_LENGTH	];
	int   count = 0;
  int		max = 20;
  int   int_eq = 2;
  int   int_type = 0;
  int   value1;
  int   value2;
  
  
	if( !ch ) 
      return;

	smash_tilde ( argument );
	argument = one_argument ( argument, arg1 );
	argument = one_argument ( argument, arg2 );
	strcpy ( arg3, argument );


	if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
  	  return;
	
	
	rch = get_char_world ( ch,arg1 );
	value1 = UMIN(max,is_number ( arg2 ) ? atoi ( arg2 ) : int_eq);
	value2 = is_number ( arg3 ) ? atoi ( arg3 ) : int_type;

  send_to_char(buf,rch);
       
  switch ( value2 )
  {
    case ITEM_TAKE		    : break;
    case ITEM_WEAR_FINGER : break;
    case ITEM_WEAR_NECK	  : break;
    case ITEM_WEAR_BODY	  : break;
    case ITEM_WEAR_HEAD	  : break;
    case ITEM_WEAR_LEGS	  : break;
    case ITEM_WEAR_FEET	  : break;
    case ITEM_WEAR_HANDS	: break;
    case ITEM_WEAR_ARMS	  : break;
    case ITEM_WEAR_SHIELD : break;
    case ITEM_WEAR_ABOUT	: break;
    case ITEM_WEAR_WAIST	: break;
    case ITEM_WEAR_WRIST	: break;
    case ITEM_WIELD		    : break;
    case ITEM_HOLD		    : break;
    case ITEM_TWO_HAND		: break;
    case ITEM_WEAR_ARROW	: break;
    default:
      value2 = int_type;
      break;

  }
  
  for( obj = rch->carrying; obj; obj = obj_next )
  {
       obj_next = obj->next_content;

       if( count >= value1) 
           break;

       if( obj->deleted )
           continue;
           
       //­Ó¤Hª««~,,¯S®íª««~,°¨,NPC_Corpse,²î°¦¤£®³
       if( IS_SET( obj->extra_flags, ITEM_PERSONAL ) 
           || IS_SET( obj->pIndexData->souvenir, Z4 )
           || IS_SET( obj->wear_flags , ITEM_WEAR_RIDE )
           || obj->item_type == ITEM_CORPSE_NPC
           || obj->item_type == ITEM_BOAT) 
           continue;
       
       //·í¦³«ü©wª««~®É¦Óobj¤£¬O©Ò«ü©wª««~¤£®³
       //·í¨S¦³«ü©wª««~®É¥Ñ¾÷²v¨Ó¬Dª««~
       if( (obj->wear_flags != value2 && value2 != int_type) 
           || ( value2 == int_type && (number_percent() >= number_percent() )))
           continue;
       
       sprintf(buf, HIW"%s±q§A¨­¤W¨ú¨«¤F%s "NOR"\n\r", ch->short_descr,obj->short_descr);
       send_to_char(buf,rch);
       obj_from_char( obj );
       obj_to_char(obj, ch);
       count++;
  }
  
  return;
}

/* Added by Razgriz 20050915
»yªk:
  mpset_obj_affect victim object duration location modifier

  victim   (¦r¦ê): self, $i	//mob ¥»¨­
                   char_id	//¸ò mob ¦P©Ð¶¡ªº char id
  object   (¼Æ¦r): object vnum
           (¦r¦ê): object name
  duration (¼Æ¦r): ®É¶¡, -1 «h¬°¥Ã¤[¦s¦b
  location (¼Æ¦r): °Ñ¦Ò parameter.h - Apply types (for affects)
  modifier (¼Æ¦r): °t¦X location ¶ñ¤J¾A·í¼Æ­È
*/
void do_mpset_obj_affect ( CHAR_DATA * ch, char * argument )
{
	CHAR_DATA *	victim	= NULL;
	AFFECT_DATA *	paf	= NULL;
	OBJ_DATA *	obj	= NULL;
	char		arg1	[ MAX_INPUT_LENGTH ];
	char		arg2	[ MAX_INPUT_LENGTH ];
	char		arg3	[ MAX_INPUT_LENGTH ];
	char		arg4	[ MAX_INPUT_LENGTH ];
	char		arg5	[ MAX_INPUT_LENGTH ];
	int		vnum	= -999;
	int		time;
	int		loc;
	int		mod;

	if ( !IS_NPC ( ch ) )
	{
		send_to_char ( "¤°»ò¡H\n\r", ch );
		return;
	}

	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpset_obj_affect - No victim name: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg2 );
	if ( arg2[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpset_obj_affect - No object name: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' || !is_number ( arg3 ) )
	{
		bug( "mpset_obj_affect - Bad syntax at duration: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg4 );
	if ( arg4[0] == '\0' || argument[0] == '\0' || !is_number ( arg4 ) )
	{
		bug( "mpset_obj_affect - Bad syntax at location: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg5 );
	if ( arg5[0] == '\0' || !is_number ( arg5 ) )
	{
		bug( "mpset_obj_affect - Bad syntax at modifier: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	//find victim
	if ( !ch->in_room ) return;
	victim = ( !str_cmp ( arg1, "self" ) || !str_cmp ( arg1, "$i" ) ) ? ch : room_find_char ( ch->in_room, arg1 );

	if ( !victim )
	{
		send_to_char ( "Victim is not here.\n\r", ch );
		return;
	}

	if ( is_number ( arg2 ) )
		vnum = atoi ( arg2 );
	time = atoi ( arg3 );
	loc  = atoi ( arg4 );
	mod  = atoi ( arg5 );

	//find obj carryed by victim ,obj can't be in container
	for ( obj = victim->carrying; obj; obj = obj->next_content )
	{
		if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
		if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
		if ( obj->wear_loc != WEAR_NONE )
			remove_obj ( victim, obj->wear_loc, TRUE );

		//set affect to object
		paf		= new_affect ();
		paf->type	= obj->pIndexData->vnum;
		paf->duration	= UMAX ( -1, time );
		paf->location	= loc;
		paf->modifier	= mod;
		paf->bitvector	= 0;
		paf->next	= obj->affected;
		obj->affected	= paf;

		return;
	}

	send_to_char ( "Item is not here.\n\r", ch );
	return;
}

/* Added by Razgriz 20050915
»yªk:
  mpremove_obj_affect victim object location

  victim   (¦r¦ê): self, $i	//mob ¥»¨­
                   char_id	//¸ò mob ¦P©Ð¶¡ªº char id
  object   (¼Æ¦r): object vnum
           (¦r¦ê): object name
  location (¼Æ¦r): °Ñ¦Ò parameter.h - Apply types (for affects)
           (¦r¦ê): all, ªí¥Ü²¾°£¸Ó obj ©Ò¦³ affect
*/
void do_mpremove_obj_affect ( CHAR_DATA * ch, char * argument )
{
	CHAR_DATA *	victim	= NULL;
	AFFECT_DATA *	paf	= NULL;
	AFFECT_DATA *	pafPre	= NULL;
	AFFECT_DATA *	pafNext	= NULL;
	OBJ_DATA *	obj	= NULL;
	char		arg1	[ MAX_INPUT_LENGTH ];
	char		arg2	[ MAX_INPUT_LENGTH ];
	char		arg3	[ MAX_INPUT_LENGTH ];
	int		vnum	= -999;
	int		loc	= -999;
	bool		fAll	= FALSE;

	if ( !IS_NPC ( ch ) )
	{
		send_to_char ( "¤°»ò¡H\n\r", ch );
		return;
	}

	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpremove_obj_affect - No victim name: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg2 );
	if ( arg2[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpremove_obj_affect - No object name: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg3 );
	fAll = !str_cmp ( arg3, "all" );
	if ( arg3[0] == '\0'
		|| ( !is_number ( arg3 ) && !fAll ) )
	{
		bug( "mpremove_obj_affect - Bad syntax at location: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	//find victim
	if ( !ch->in_room ) return;
	victim = ( !str_cmp ( arg1, "self" ) || !str_cmp ( arg1, "$i" ) ) ? ch : room_find_char ( ch->in_room, arg1 );

	if ( !victim )
	{
		send_to_char ( "Victim is not here.\n\r", ch );
		return;
	}

	if ( is_number ( arg2 ) ) vnum = atoi ( arg2 );
	if ( is_number ( arg3 ) ) loc  = atoi ( arg3 );

	//find obj carryed by victim ,obj can't be in container
	for ( obj = victim->carrying; obj; obj = obj->next_content )
	{
		if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
		if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
		if ( obj->wear_loc != WEAR_NONE )
			remove_obj ( victim, obj->wear_loc, TRUE );

		//remove affect from object
		if ( fAll )
		{
			for ( paf = obj->affected; paf; paf = pafNext )
			{
				pafNext = paf->next;
				free_affect ( paf );
			}
			obj->affected = NULL;
			return;
		}
		else
		{
			pafPre = obj->affected;
			for ( paf = obj->affected; paf; paf = pafNext )
			{
				pafNext = paf->next;
				if ( paf->location != loc )
				{
					pafPre = paf;
					continue;
				}

				if ( paf == obj->affected )
					obj->affected = pafPre = paf->next;
				else
					pafPre->next = paf->next;

				free_affect ( paf );
			}
			return;
		}
	}// end for obj

	send_to_char ( "Item is not here.\n\r", ch );
	return;
}

/* Added by Razgriz 20050916
»yªk:
  mpmodify_obj_affect victim object location opr modifier [duration]

  victim   (¦r¦ê): self, $i	//mob ¥»¨­
                   char_id	//¸ò mob ¦P©Ð¶¡ªº char id
  object   (¼Æ¦r): object vnum
           (¦r¦ê): object name
  location (¼Æ¦r): °Ñ¦Ò parameter.h - Apply types (for affects)
  opr      (²Å¸¹): + - & | r =
  modifier (¼Æ¦r): °t¦X location ¶ñ¤J¾A·í¼Æ­È
  duration (¼Æ¦r): ¦³­Èªº¸Ü, ·í§ä¤£¨ì affect ®É, ·|¦Û°Ê©I¥s set ¨Ã§â duration ³]¦¨¦¹­È
                   ¨S­Èªº¸Ü, ·í§ä¤£¨ì affect ®É, ¤°»ò³£¤£°µ
*/
void do_mpmodify_obj_affect ( CHAR_DATA * ch, char * argument )
{
	CHAR_DATA *	victim	= NULL;
	AFFECT_DATA *	paf	= NULL;
	OBJ_DATA *	obj	= NULL;
	char		arg1	[ MAX_INPUT_LENGTH ];
	char		arg2	[ MAX_INPUT_LENGTH ];
	char		arg3	[ MAX_INPUT_LENGTH ];
	char		opr	[ MAX_INPUT_LENGTH ];
	char		arg5	[ MAX_INPUT_LENGTH ];
	char		arg6	[ MAX_INPUT_LENGTH ];
	char		buf	[ MAX_INPUT_LENGTH ];
	char *		blank	= " ";
	int		vnum	= -999;
	int		loc;
	int		mod;

	if ( !IS_NPC ( ch ) )
	{
		send_to_char ( "¤°»ò¡H\n\r", ch );
		return;
	}

	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpmodify_obj_affect - No victim name: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg2 );
	if ( arg2[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpmodify_obj_affect - No object name: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' || !is_number ( arg3 ) )
	{
		bug( "mpmodify_obj_affect - Bad syntax at location: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, opr );
	if ( opr[0] == '\0' || argument[0] == '\0' )
	{
		bug( "mpmodify_obj_affect - Bad syntax at operator: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg5 );
	if ( arg5[0] == '\0' || !is_number ( arg5 ) )
	{
		bug( "mpmodify_obj_affect - Bad syntax at modifier: vnum %d.", ch->pIndexData->vnum );
		return;
	}

	//find victim
	if ( !ch->in_room ) return;
	victim = ( !str_cmp ( arg1, "self" ) || !str_cmp ( arg1, "$i" ) ) ? ch : room_find_char ( ch->in_room, arg1 );

	if ( !victim )
	{
		send_to_char ( "Victim is not here.\n\r", ch );
		return;
	}

	if ( is_number ( arg2 ) )
		vnum = atoi ( arg2 );
	loc  = atoi ( arg3 );
	mod  = atoi ( arg5 );

	//find obj carryed by victim ,obj can't be in container
	for ( obj = victim->carrying; obj; obj = obj->next_content )
	{
		if ( vnum == -999 && !is_name ( arg2, obj->name )  ) continue;
		if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
		if ( obj->wear_loc != WEAR_NONE )
			remove_obj ( victim, obj->wear_loc, TRUE );

		//find affect depend on location
		for ( paf = obj->affected; paf; paf = paf->next )
		{
			if ( paf->location != loc ) continue;
			paf->modifier = qmark_opr ( paf->modifier , opr , mod, "mpmodify_obj_affect" );
			return;
		}
		
		//no find affect, if duration have value call mpset_obj_affect
		argument = one_argument ( argument, arg6 );
		if ( arg6[0] == '\0' || !is_number ( arg6 ) )	return;
		else
		{
			buf[0] = '\0';
			strcat ( buf, arg1 ); strcat ( buf, blank );
			strcat ( buf, arg2 ); strcat ( buf, blank );
			strcat ( buf, arg6 ); strcat ( buf, blank );
			strcat ( buf, arg3 ); strcat ( buf, blank );
			strcat ( buf, arg5 );
			do_mpset_obj_affect ( ch, buf );
		}
		return;
	}

	send_to_char ( "Item is not here.\n\r", ch );
	return;
}

/* Added by Razgriz 20051010
 * force object do something
 */
void do_mpforceo ( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj = NULL;
	char arg[ MAX_INPUT_LENGTH ];

	// ³Q force ªº±¡ªp¤U, ©Ò¦³¤H³£¥i¥H¨Ï¥Î mpforceo 2022/12/01
	if ( !ch->forced ){
		if ( !IS_NPC ( ch ) )
		{
			send_to_char ( "¤°»ò¡H\n\r", ch );
			return;
		}
	}

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		//send_to_char( "Force which object to do what?\n\r", ch );
		return;
	}

	obj = get_obj_here ( ch , arg );

	if ( !obj )
	{
		//send_to_char("No such object in here.\n\r", ch );
		return;
	}

	/*
	 * Look for command in command table.
	 */

	// ²¾°£ ch force ª¬ºA 2022/12/01
	if( ch->forced )
		ch->forced = FALSE;

	ointerpret( obj , ch , argument );

	//send_to_char( "Ok.\n\r", ch );
	return;
}