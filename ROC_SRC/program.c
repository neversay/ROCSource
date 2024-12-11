/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "ansi.h"

char *	mprog_type_to_name	args( ( int type ) );
char *	rprog_type_to_name	args( ( int type ) );

/* This routine transfers between alpha and numeric forms of the
 *  mob_prog bitvector types. It allows the words to show up in mpstat to
 *  make it just a hair bit easier to see what a mob should be doing.
 */

char *mprog_type_to_name( int type )
{
    switch ( type )
    {
    case IN_FILE_PROG:		return "in_file_prog";
    case ACT_PROG:		return "act_prog";
    case SPEECH_PROG:		return "speech_prog";
    case RAND_PROG:		return "rand_prog";
    case FIGHT_PROG:		return "fight_prog";
    case HITPRCNT_PROG:		return "hitprcnt_prog";
    case DEATH_PROG:		return "death_prog";
    case ENTRY_PROG:		return "entry_prog";
    case GREET_PROG:		return "greet_prog";
    case ALL_GREET_PROG:	return "all_greet_prog";
    case GIVE_PROG:		return "give_prog";
    case BRIBE_PROG:		return "bribe_prog";
    case TALK_PROG:		return "talk_prog";
    case TICK_PROG:		return "tick_prog";
    case REPOP_PROG:		return "repop_prog";
    case DEFUN_PROG:		return "defun_prog";
    case HURT_PROG:		return "hurt_prog";
    case KILL_PROG:             return "kill_prog";
    case ENTRY_GREET_PROG:     return "entry_greet_prog"; // add at 2021/12/18
    default:			return "ERROR_PROG";
    }
}

char *oprog_type_to_name( int type )
{
    switch ( type )
    {
		case OBJ_DEFUN:	    return "obj_defun";
		case OBJ_FIGHT:	    return "obj_fight";
		case OBJ_ACT:	    return "obj_act";
		case OBJ_REPOP:	    return "obj_repop";
		case OBJ_HIT:	    return "obj_hit";
		case OBJ_BLOCK:	    return "obj_block";
		case OBJ_RAND:	    return "obj_rand";
		case OBJ_BREAK:	    return "obj_break";
		case OBJ_LOAD:	    return "obj_load";
		case OBJ_DISAPPEAR: return "obj_disappear";
		case OBJ_TICK:      return "obj_tick";
		case OBJ_COMMAND:   return "obj_command";
		case OBJ_KILL:      return "obj_kill";  // add at 2022/05/08
		default:			return "ERROR_PROG";
    }
}
char *rprog_type_to_name( int type )
{
    switch ( type )
    {
    case ROOM_ENTER:		return "room_enter";
    case ROOM_LEAVE:		return "room_leave";
    case ROOM_RAND:		return "room_rand";
    case ROOM_TIME:		return "room_time";
    case ROOM_COMMAND:		return "room_command";
    case ROOM_MASK:		return "room_mask";
    case ROOM_TRAP:		return "room_trap";
    case ROOM_DEFUN:		return "room_defun";
    default:			return "ERROR_PROG";
    }
}

void check_position( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
	update_pos( victim );

	switch( victim->position )
	{
		case POS_DEAD:
			if( IS_NPC(victim) && IS_SET(victim->act, ACT_MPROG_DEATH ))
				break;
			if( !is_chinese(victim, NULL))
			{
				send_to_char( "You have been KILLED!!\n\r\n\r", victim );
				act( "$n is DEAD!!", victim, NULL, NULL, TO_ROOM );
			}
			else
			{
				send_to_char( "你死了！\n\r\n\r", victim );
				act( "$n 死了！", victim, NULL, NULL, TO_ROOM );
			}
			// Don't do raw_kill() here. It'll make double death check.
			// Keric. 2005/07/22
			//raw_kill( ch, victim );
			break;

		case POS_STUNNED:
			if( !is_chinese(victim, NULL))
			{
				send_to_char("You are stunned, but will probably recover.\n\r", victim );
				act( "$n is stunned, but will probably recover.", victim, NULL, NULL, TO_ROOM );
			}
			else
			{
				send_to_char("你昏倒了，過一會也許\會回復.\n\r", victim );
				act( "$n 昏倒了，過一會也許\會回復.", victim, NULL, NULL, TO_ROOM );
			}
			break;

		case POS_INCAP:
			if( !is_chinese(victim, NULL))
			{
				send_to_char( "You are incapacitated and will slowly die, if not aided.\n\r", victim );
				act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
			}
			else
			{
				send_to_char( "你因傷重而無法行動，如果得不到治療，就只有等死了.\n\r", victim );
				act( "$n 因傷重而無法行動，如果得不到治療，就只有等死了.", victim, NULL, NULL, TO_ROOM );
			}
			break;

		case POS_MORTAL:
			if( !is_chinese(victim, NULL))
			{
				send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", victim );
				act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
			}
			else
			{
				send_to_char(  "你受了極嚴重的傷，如果再不治療，很快就會死去.\n\r", victim );
				act( "$n 受了極嚴重的傷，如果再不治療，很快就會死去.", victim, NULL, NULL, TO_ROOM );
			}
			break;

		default:
			if ( dam > victim->max_hit / 4 )
				send_to_char( (is_chinese(victim, NULL)) ?
				HIY"天哪! 痛死人了!"NOR"\n\r" :
				HIY"That really did HURT!"NOR"\n\r",
				victim );
			if ( victim->hit < victim->max_hit / 4 )
				send_to_char( (is_chinese(victim, NULL)) ?
				HIR"你正在噴血!"NOR"\n\r" :
				HIR"You sure are BLEEDING!"NOR"\n\r",
				victim );
			break;
	}
	return;
}

void do_rpstat( CHAR_DATA *ch, char *argument )
{
	char buf[ MAX_STRING_LENGTH *25 ]; // 讓最大字數 *25 避免字數過多 crash 2021/10/17
	RPROG_DATA *rprg;

	if ( !authorized( ch, "rpstat" ) )
		return;


	if( !ch->in_room ) return;
	if( !ch->in_room->roomprogs ) {
		send_to_char("There is no roomprog in this room.\n\r", ch);
		return;
	}
	for ( rprg = ch->in_room->roomprogs; rprg ; rprg = rprg->next )
	{
		sprintf( buf, ">%s %s\n\r%s\n\r",
		rprog_type_to_name( rprg->type ),
		rprg->arglist,
		rprg->comlist );
		send_to_char( buf, ch );
	}
	return;

}

void do_opstat( CHAR_DATA *ch, char *argument )
{
	char buf[ MAX_STRING_LENGTH * 25];
	char arg[ MAX_INPUT_LENGTH  ];
	OPROG_DATA *oprg;
	OBJ_DATA *target;

	if ( !authorized( ch, "opstat" ) )
		return;
	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "ObjProg stat whom?\n\r", ch );
		return;
	}

	if ( ( target = get_obj_world( ch, arg ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if ( !( target->pIndexData->progtypes ) )
	{
		send_to_char( "That Obj has no Programs set.\n\r", ch);
		return;
	}

	for ( oprg = target->pIndexData->objprogs; oprg != NULL; oprg = oprg->next )
	{
		sprintf( buf, ">%s %s\n\r%s\n\r",
		oprog_type_to_name( oprg->type ),
		oprg->arglist,
		oprg->comlist );
		send_to_char( buf, ch );
	}

	return;

}

void do_mastat( CHAR_DATA *ch, char *argument )
{
    char	buf[ MAX_STRING_LENGTH * 25];
    char	arg[ MAX_INPUT_LENGTH  ];
    MOB_ACTION  *mprg;
    CHAR_DATA  *target;

    if ( !authorized( ch, "mastat" ) )
	return;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "MobAction stat whom?\n\r", ch );
	return;
    }

    if ( ( target = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC( target ) )
    {
	send_to_char( "Only Mobiles can have Actions!\n\r", ch);
	return;
    }

    if ( !( target->pIndexData->mob_action) )
    {
	send_to_char( "That Mobile has no Actions set.\n\r", ch);
	return;
    }

    sprintf( buf, "Name: %s.  Vnum: %d.\n\r",
	target->name, target->pIndexData->vnum );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong  description: %s",
	    target->short_descr,
	    target->long_descr[0] != '\0' ?
	    target->long_descr : "(none).\n\r" );
    send_to_char( buf, ch );

    sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d. \n\r",
	target->hit,	     target->max_hit,
	target->mana,	     target->max_mana,
	target->move,	     target->max_move );
    send_to_char( buf, ch );

    sprintf( buf,
	"Lv: %d.  Class: %d.  Align: %d.  AC: %d.  Gold: %d.  Exp: %d.\n\r",
	target->level,	     target->class,	   target->alignment,
	GET_AC( target ),    target->gold,	   target->exp );
    send_to_char( buf, ch );

    for ( mprg = mob_action_list ; mprg != NULL;
	 mprg = mprg->next_in_list )
    {
	if( mprg->owner && mprg->owner->pIndexData == target->pIndexData ){
	    sprintf( buf, "(%d)A %d\n %s\n\r",
		    mprg->mode,
		    mprg->start_time,
		    mprg->com_list );
	    send_to_char( buf, ch );
	}
    }

    return;

}

/* A trivial rehack of do_mstat.  This doesnt show all the data, but just
 * enough to identify the mob and give its basic condition.  It does however,
 * show the MOBprograms which are set.
 */

void do_mpstat( CHAR_DATA *ch, char *argument )
{
    char	buf[ MAX_STRING_LENGTH * 25];
    char	arg[ MAX_INPUT_LENGTH  ];
    MPROG_DATA *mprg;
    CHAR_DATA  *target;

    if ( !authorized( ch, "mpstat" ) )
	return;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "MobProg stat whom?\n\r", ch );
	return;
    }

    if ( ( target = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC( target ) )
    {
	send_to_char( "Only Mobiles can have Programs!\n\r", ch);
	return;
    }

    if ( !( target->pIndexData->progtypes ) )
    {
	send_to_char( "That Mobile has no Programs set.\n\r", ch);
	return;
    }

    sprintf( buf, "Name: %s.  Vnum: %d.\n\r",
	target->name, target->pIndexData->vnum );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong  description: %s",
	    target->short_descr,
	    target->long_descr[0] != '\0' ?
	    target->long_descr : "(none).\n\r" );
    send_to_char( buf, ch );

    sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d. \n\r",
	target->hit,	     target->max_hit,
	target->mana,	     target->max_mana,
	target->move,	     target->max_move );
    send_to_char( buf, ch );

    sprintf( buf,
	"Lv: %d.  Class: %d.  Align: %d.  AC: %d.  Gold: %d.  Exp: %d.\n\r",
	target->level,	     target->class,	   target->alignment,
	GET_AC( target ),    target->gold,	   target->exp );
    send_to_char( buf, ch );

    for ( mprg = target->pIndexData->mobprogs; mprg != NULL;
	 mprg = mprg->next )
    {
      sprintf( buf, ">%s %s\n\r%s\n\r",
	      mprog_type_to_name( mprg->type ),
	      mprg->arglist,
	      mprg->comlist );
      send_to_char( buf, ch );
    }

    return;

}

