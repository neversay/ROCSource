/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael    *
 *  Chastain, Michael Quan, and Mitchell Tse.          *
 *                     *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc     *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.               *
 *                     *
 *  Much time and thought has gone into this software and you are    *
 *  benefitting.  We hope that you share your changes too.  What goes    *
 *  around, comes around.              *
 ***************************************************************************/

/***************************************************************************
 *  The MOBprograms have been contributed by N'Atas-ha.  Any support for   *
 *  these routines should not be expected from Merc Industries.  However,  *
 *  under no circumstances should the blame for bugs, etc be placed on     *
 *  Merc Industries.  They are not guaranteed to work on all systems due   *
 *  to their frequent use of strxxx functions.  They are also not the most *
 *  efficient way to perform their tasks, but hopefully should be in the   *
 *  easiest possible way to install and begin using. Documentation for     *
 *  such installation can be found in INSTALL.  Enjoy........  N'Atas-Ha *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : room_commands.c                                *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         ­×§ïdo_modhmv §PÂ_imm¤§charÂêhp 1      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jacquex                                *
//*  PURPOSE     :         ­×§ïpurge ªº¦a¤è¡AÁ×§K¤@­Ócrash bug    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         ­×§ïdo_roload¡AÁ×§K¤@­Ócrash bug       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940616 jye                                    *
//*  PURPOSE     :         ­×§ïdo_roload¡A¤£¥X¨Ó¤§bug             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940707 Razgriz                                *
//*  PURPOSE     :         ­×§ï do_rat¡A¦b¥H¨¤¦â¦WºÙ§PÂ_¦aÂI®É¡A  *
//*              :         ¥[¤J¨¤¦â¬O§_³Q deleted ¤§§PÂ_¡A¥HÁ×§K  *
//*              :         room ÅÜ NULL«á¡A³Q¨ä¥L¨ç¼Æ¨Ï¥Î¨ì¦Ó³y¦¨ *
//*              :         crash.                                 *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940723 Jye                                    *
//*  PURPOSE     :         ­×§ï do_roload ³Ì°ªµ¥¯Å½Õ¬°41          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940907 Razgriz                                *
//*  PURPOSE     :         ­×§ï do_rforce Åý mob ¤]¯à³]©w forced  *
//*              :         ÅÜ¼Æ, ¥HÁ×§K rprog_mask_trigger ¤¤ªº¬Y *
//*              :         ºØ¼gªk³y¦¨µL½a¦^°é, ¾É­P±j¨î crash     *
//*              :         ex: Å]²´´ËªL                           *
//*****************************************************************
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "ansi.h"

//extern int qmark_opr args( ( int lhs, char *opr, int rhs ) );
extern int qmark_opr args( ( int lhs, char *opr, int rhs, char *mod_name ) );
extern long long ll_qmark_opr args( ( long long lhs, char *opr, long long rhs, char *mod_name ) ); // ¼W¥[¼Æ­È¸û¤jªº§P©w 2023/01/10
extern bool IsSorGateMob(int nMobIndex);

void do_rpresp( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;
    
	if( IS_NPC(ch) ){
		// send_to_char("¤°»ò?\n\r", ch);
		return;
	}
	/*
	if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	{
		send_to_char( "¤°»ò?\n\r", ch );
		return;
	}
    */
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
			bugf("Room : %d rpresp %s %d", room->vnum, target->name, value);
			return;
		}
	}
	bug ( "Room: %d 'rpresp' bad syntax", room->vnum );
	return;
}

void do_rpgold( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;

	if( IS_NPC(ch) ){
	//  send_to_char("¤°»ò?\n\r", ch);
		return;
	}
	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	//{
	//  send_to_char( "¤°»ò?\n\r", ch );
	//  return;
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
				sprintf( arg, "§A±o¨ì %d ª÷¹ô¡C\n\r", value );
				send_to_char( arg, target );
			}
			else
			{
				int temp=0-value;
				sprintf( arg, "§A¥¢¥h %d ª÷¹ô¡C\n\r", temp );
				send_to_char( arg, target ); 
			}
			//act( "$n µ¹¤F $N ¤@¨Çª÷¹ô¡C", ch, NULL, target, TO_NOTVICT );  remove at 20.02.25
			//bugf("Room : %d rpgold %s %d", room->vnum, target->name, value);
			return;
		}
	}
	bug ( "Room: %d 'rpgold' bad syntax", room->vnum );
	return;
}

void do_rpexp( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;
    
	if( IS_NPC(ch) ){
	//  send_to_char("¤°»ò?\n\r", ch);
		return;
	}
	// if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	// {
	//  send_to_char( "¤°»ò?\n\r", ch );
	//  return;
	// }

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			if ( value > 0 )
			{
				sprintf( arg, "§A±o¨ì %d ÂI¸gÅç¡C\n\r", value );
			}
			else
			{
				if ( target->exp < abs( value ) )
				{
					value = 1000 - target->exp;
				}
				sprintf( arg, "§A¥¢¥h %d ÂI¸gÅç¡C\n\r", abs( value ) );
			}
			send_to_char( arg, target );
			gain_exp( target, value );
			bugf("Room : %d rpexp %s %d", room->vnum, target->name, value);
			return;
		}
	}
	bug ( "Room: %d 'rpexp' bad syntax", room->vnum );
	return;
}

// rpalign add at 20.02.25
void do_rpalign( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;
	OBJ_DATA *obj, *obj_next;

	if( IS_NPC(ch) ){
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
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

			for( obj = target->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ( obj->deleted ) continue;
				if ( obj->wear_loc == WEAR_NONE ) continue;
				if (
					  ( IS_OBJ_STAT ( obj, ITEM_ANTI_EVIL    ) && IS_EVIL    ( target ) )
					||( IS_OBJ_STAT ( obj, ITEM_ANTI_GOOD    ) && IS_GOOD    ( target ) )
					||( IS_OBJ_STAT ( obj, ITEM_ANTI_NEUTRAL ) && IS_NEUTRAL ( target ) ) 
				) {
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
			return;
		}
	}
	bug ( "Room: %d 'rpalign' bad syntax", room->vnum );
	return;
}

// rpskillexp add at 20.02.25
void do_rpskillexp( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;
    
	if( IS_NPC(ch) ){
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		one_argument( argument, arg );
		if ( is_number( arg ) )
		{
			value = atoi( arg );
			if ( value > 0 )
			{
				sprintf( arg, "§A±o¨ì %d ÂI§Þ¯à¸gÅç¡C\n\r", value );
			}
			else
			{
				if ( target->exp < abs( value ) )
				{
					value = 1000 - target->exp;
				}
				sprintf( arg, "§A¥¢¥h %d ÂI§Þ¯à¸gÅç¡C\n\r", abs( value ) );
			}
			send_to_char( arg, target );
			gain_skill_exp( target, value );
			bugf("Room : %d rpskillexp %s %d", room->vnum, target->name, value);
			return;
		}
	}
	bug ( "Room: %d 'rpskillexp' bad syntax", room->vnum );
	return;
}

// rpmerit add at 20.02.25
void do_rpmerit( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int    value;

	if( IS_NPC(ch) ){
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
		one_argument(argument, arg);
		if ( is_number( arg ) ) {
			value = atoi( arg );
			gain_merit( target, value );
			return;
		}
	}
	bug ( "Room: %d 'rpmerit' bad syntax", room->vnum );
	return;
}

// rppractice add at 20.02.25
void do_rppractice( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
	int value;
    
	if( IS_NPC(ch) ){
		return;
	}

	argument = one_argument( argument, arg );

	if ( ( target = get_char_world( ch, arg ) ) )
	{
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
	bug ( "Room: %d 'rppractice' bad syntax", room->vnum );
	return;
}

void do_radd_qmark( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;
    
	smash_tilde(argument);
	argument = one_argument( argument, arg );
	if ( !( target = room_find_char( room, arg ) ) )
	{
		bug( "radd_qmark - no(can't see) target: vnum %d.", room->vnum );
		return;
	}
    
	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		bug( "radd_qmark - no mark_name string: vnum %d.", room->vnum );
		return;
	}
	//smash_tilde( arg );
    
	if ( is_qmark( target, arg ) )
	{
		bug( "radd_qmark - qmark exist already: vnum %d.", room->vnum );
			return;
	}

	qmark = set_qmark( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		bug( "radd_qmark - Bad syntax: vnum %d.", room->vnum );
		qmark->timer = -1;
	}
	else
		qmark->timer = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		bug( "radd_qmark - Bad syntax: vnum %d.", room->vnum );
		qmark->v0 = 0;
	}
	else
		qmark->v0 = atoi( arg );

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		bug( "radd_qmark - Bad syntax: vnum %d.", room->vnum );
		qmark->v1 = 0;
	}
	else
		qmark->v1 = atoi( arg );
    
	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		bug( "radd_qmark - Bad syntax: vnum %d.", room->vnum );
		qmark->v2 = 0;
	}
	else
		qmark->v2 = atoi( arg );
    
	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		bug( "radd_qmark - Bad syntax: vnum %d.", room->vnum );
		qmark->fDeath = TRUE;
	}
	else
		qmark->fDeath = ( atoi( arg ) == 0 ) ? FALSE : TRUE;

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
		bug( "radd_qmark - Bad syntax: vnum %d.", room->vnum );
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

void do_rdel_qmark( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH ];
    QMARK *qmark;
    CHAR_DATA *target;

    smash_tilde(argument);
    argument = one_argument( argument, arg );
    if ( !( target = room_find_char( room, arg ) ) )
    {
  bug( "rdel_qmark - no(can't see) target: vnum %d.", room->vnum );
  return;
    }
    
    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
  bug( "rdel_qmark - no mark_name string: vnum %d.", room->vnum );
  return;
    }

    //smash_tilde( arg );
    if ( !target->qmark )
    {
        bug( "rdel_qmark - target has no any qmark to delete: vnum %d.", 
          room->vnum );
        return;
    }

    for( qmark = target->qmark; qmark; qmark = qmark->next )
      if ( !str_cmp( qmark->mark_name, arg ) )
  {
    del_qmark( qmark );
    return;
  }

    bug( "rdel_qmark - target has no this qmark: vnum %d.", room->vnum );
    return;
}

void do_rmod_qmark( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	char val[ MAX_INPUT_LENGTH ];
	QMARK *qmark;
	CHAR_DATA *target;
	int value;

	smash_tilde(argument);
	argument = one_argument( argument, arg );
	if ( !( target = room_find_char( room, arg ) ) )
	{
		bug( "rmod_qmark - no(can't see) target: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		bug( "rmod_qmark - no mark_name string: vnum %d.", room->vnum );
		return;
	}

	//smash_tilde( arg );
	if ( !target->qmark )
	{
		bug( "rdel_qmark - target has no any qmark: vnum %d.", room->vnum );
		return;
	}
	if( !( qmark = is_qmark(target, arg) )) {
		bug( "rmod_qmark - target has no this qmark: vnum %d.", room->vnum );
		return;
	}


	argument = one_argument( argument, arg );
	argument = one_argument( argument, opr );
	one_argument( argument, val );
	// ­×¥¿¦r¦ê±q 0 ¦pªG¬°ªÅªº±¡ªp¤~·|¶] bug log , §_«h¦b qmark ¦³­Èªº±¡ªp¤U·|³y¦¨¶]¤£¨ì return ¦Ó crash 2023/01/09
	//if ( arg == '\0' || opr == '\0' || val == '\0' )
	if ( arg[0] == '\0' || opr[0] == '\0' || val[0] == '\0' || argument[0] == '\0' )
	{
		bug( "rmod_qmark - illegal operator: vnum %d.", room->vnum );
		return;
	}

	value = atoi( val );

	if ( !str_cmp( arg, "timer" ) )
	{
		qmark->timer = qmark_opr( qmark->timer, opr, value, "rmod_qmark_timer" );
		return;
	}
	if ( !str_cmp( arg, "v0" ) )
	{
		qmark->v0 = qmark_opr( qmark->v0, opr, value, "rmod_qmark_v0" );
		return;
	}
	if ( !str_cmp( arg, "v1" ) )
	{
		qmark->v1 = qmark_opr( qmark->v1, opr, value, "rmod_qmark_v1" );
		return;
	}
	if ( !str_cmp( arg, "v2" ) )
	{
		qmark->v2 = qmark_opr( qmark->v2, opr, value, "rmod_qmark_v2" );
		return;
	}
	if ( !str_cmp( arg, "death" ) )
	{
		qmark->fDeath = qmark_opr( qmark->fDeath, opr, value, "rmod_qmark_death" );
		return;
	}
	if ( !str_cmp( arg, "save" ) )
	{
		qmark->fSave = qmark_opr( qmark->fSave, opr, value, "rmod_qmark_save" );
		return;
	}
	bug( "rmod_qmark - illegal operator: vnum %d.", room->vnum );
	return;
}

void do_radd_all_qmark( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
  char  arg[ MAX_INPUT_LENGTH ];
  char  buf[ MAX_INPUT_LENGTH ];
  CHAR_DATA *lch, *vch;

        smash_tilde(argument);
  argument = one_argument( argument, arg );

/*   old version
    if ( !( lch = room_find_char( room, arg ) ) )
    {
  bug( "radd_all_qmark - no(can't see) target: vnum %d.", room->vnum );
  return;
    }

  for( vch = lch->in_room->people; vch; vch = vch->next_in_room )
    if ( is_same_group( lch, vch ) )
    {
      sprintf( buf, "%s ", vch->name );
      strcat( buf, argument );
      do_radd_qmark( room, ch, buf );
    } 
*/
	if ( !str_cmp( arg, "all" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				sprintf( buf, "%s ", lch->name );
				strcat( buf, argument );
				do_radd_qmark( room, ch, buf );
			}
		return;
	}else if ( !str_cmp( arg, "npc" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				if( IS_NPC( lch ) ){
					sprintf( buf, "%s ", lch->name );
					strcat( buf, argument );
					do_radd_qmark( room, ch, buf );
				}
			}
		return;
	}else if ( !str_cmp( arg, "pc" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				if( !IS_NPC( lch ) ){
					sprintf( buf, "%s ", lch->name );
					strcat( buf, argument );
					do_radd_qmark( room, ch, buf );
				}
			}
		return;
	}else{
		if ( !( lch = room_find_char( room, arg ) ) )
		{
		  bug( "radd_all_qmark - no(can't see) target: vnum %d.", room->vnum );
		  return;
		}
		for( vch = lch->in_room->people; vch; vch = vch->next_in_room ){
			if ( is_same_group( lch, vch ) ){
				sprintf( buf, "%s ", vch->name );
				strcat( buf, argument );
				do_radd_qmark( room, ch, buf );
			}
		}
	}
}

void do_rdel_all_qmark( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char buf[ MAX_INPUT_LENGTH ];
	CHAR_DATA *lch, *vch;

	smash_tilde(argument);
	argument = one_argument( argument, arg );

	/*   old version
		if ( !( lch = room_find_char( room, arg ) ) )
		{
	  bug( "rdel_all_qmark - no(can't see) target: vnum %d.", room->vnum );
	  return;
		}

	  for( vch = lch->in_room->people; vch; vch = vch->next_in_room )
		if ( is_same_group( lch, vch ) )
		{
		  sprintf( buf, "%s ", vch->name );
		  strcat( buf, argument );
		  do_rdel_qmark( room, ch, buf );
		}
	*/
  
	if ( !str_cmp( arg, "all" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				sprintf( buf, "%s ", lch->name );
				strcat( buf, argument );
				do_rdel_qmark( room, ch, buf );
			}
		return;
	}else if ( !str_cmp( arg, "npc" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				if( IS_NPC( lch ) ){
					sprintf( buf, "%s ", lch->name );
					strcat( buf, argument );
					do_rdel_qmark( room, ch, buf );
				}
			}
		return;
	}else if ( !str_cmp( arg, "pc" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				if( !IS_NPC( lch ) ){
					sprintf( buf, "%s ", lch->name );
					strcat( buf, argument );
					do_rdel_qmark( room, ch, buf );
				}
			}
		return;
	}else{
		if ( !( lch = room_find_char( room, arg ) ) )
		{
		  bug( "rdel_all_qmark - no(can't see) target: vnum %d.", room->vnum );
		  return;
		}
		for( vch = lch->in_room->people; vch; vch = vch->next_in_room ){
			if ( is_same_group( lch, vch ) ){
				sprintf( buf, "%s ", vch->name );
				strcat( buf, argument );
				do_rdel_qmark( room, ch, buf );
			}
		}
	}
}

void do_rmod_all_qmark( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	  char arg[ MAX_INPUT_LENGTH ];
	  char buf[ MAX_INPUT_LENGTH ];
	  CHAR_DATA *lch, *vch;

	smash_tilde(argument);
	argument = one_argument( argument, arg );

	/*   old version
		if ( !( lch = room_find_char( room, arg ) ) )
		{
	  bug( "rmod_all_qmark - no(can't see) target: vnum %d.", room->vnum );
	  return;
		}

	  for( vch = lch->in_room->people; vch; vch = vch->next_in_room )
		if ( is_same_group( lch, vch ) )
		{
		  sprintf( buf, "%s ", vch->name );
		  strcat( buf, argument );
		  do_rmod_qmark( room, ch, buf );
		}
	*/

	if ( !str_cmp( arg, "all" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				sprintf( buf, "%s ", lch->name );
				strcat( buf, argument );
				do_rmod_qmark( room, ch, buf );
			}
		return;
	}else if ( !str_cmp( arg, "npc" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				if( IS_NPC( lch ) ){
					sprintf( buf, "%s ", lch->name );
					strcat( buf, argument );
					do_rmod_qmark( room, ch, buf );
				}
			}
		return;
	}else if ( !str_cmp( arg, "pc" ) ){
		if ( room->people )
			for ( lch = room->people; lch; lch = vch){
				vch = lch->next_in_room;
				if( !IS_NPC( lch ) ){
					sprintf( buf, "%s ", lch->name );
					strcat( buf, argument );
					do_rmod_qmark( room, ch, buf );
				}
			}
		return;
	}else{
		if ( !( lch = room_find_char( room, arg ) ) )
		{
		  bug( "rmod_all_qmark - no(can't see) target: vnum %d.", room->vnum );
		  return;
		}
		for( vch = lch->in_room->people; vch; vch = vch->next_in_room ){
			if ( is_same_group( lch, vch ) ){
				sprintf( buf, "%s ", vch->name );
				strcat( buf, argument );
				do_rmod_qmark( room, ch, buf );
			}
		}
	}
}


void do_rpstun( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	CHAR_DATA *target, *target_next;
	int stun_time;

	argument = one_argument( argument, arg );
	one_argument( argument, arg1 );
    
	if ( arg[0] == '\0' || arg1[0] == '\0' )
	{
		bug( "room stun - No argument:  Room: %d.", room->vnum );
		return;
	}

	if( !is_number(arg1) ) {
		bug( "room stun - stun time must be integer: Room: %d.", room->vnum );
		return;
    }
	stun_time = atoi(arg1);
    
	if ( !str_cmp( arg, "pc" ) )
	{
		if ( room->people )
		{
			for ( target = room->people; target; target = target_next )
			{
				target_next = target->next_in_room;
				if ( !target )
				{
					bug( "room stun - target does not exist PC: Room: %d.", room->vnum );
					return;
				}
				if ( IS_NPC( target ) )
					continue;
				target->wait = target->wait + stun_time;
			}
		}
		return;
	}
	else if ( !str_cmp( arg, "npc" ) )
	{
		if ( room->people )
		{
			for ( target = room->people; target; target = target_next )
			{
				target_next = target->next_in_room;
				if ( !target )
				{
					bug( "room stun - target does not exist NPC: Room: %d.", room->vnum );
					return;
				}
				if ( !IS_NPC( target ) )
					continue;
				target->wait = target->wait + stun_time;
			}
		}
		return;
	}
	else
	{
		target = room_find_char( room, arg );
	}

	if ( !target )
	{
		bug( "room stun - target does not exist: Room: %d.", room->vnum );
		//bugf( "room stun - target does not exist: Room: %d. Actor: %s.",room->vnum , actor->name );
		return;
	}
	/*
	if( IS_NPC( target ) )
		return;
	*/
	target->wait = target->wait + stun_time ;
	return;
}

void do_rechoat( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
	char       arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;


	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Echoat - No argument:  Room: %d.", room->vnum );
		return;
	}

	target = room_find_char( room, arg );
	//target = world_find_char(room,arg);

	if ( !target )
	{
		bug( "Echoat - target does not exist: Room: %d.", room->vnum );
		//bugf( "Echoat - target does not exist: Room: %d. Actor: %s.",room->vnum ,  actor->name );
		return;
	}

	act( argument, target, NULL, target, TO_CHAR );
	return;
}

void do_rgreward( ROOM_INDEX_DATA *room, CHAR_DATA *actor,char *argument )
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

    //if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
   // {
//  send_to_char( "¤°»ò?\n\r", ch );
//  return;
  //  }

    argument = one_argument( argument, type);
    argument = one_argument( argument, ppl);
    argument = one_argument( argument, level);
    argument = one_argument( argument, value);

    if( !is_number(level) || !is_number(value) ) {
  bug("Rgreward: bad level or value : %d", room->vnum);
  return;
    }

    if( !( victim = room_find_char( room, ppl ))) {
  bug("Rgreward: victim not found :%d", room->vnum);
  return;
    }

    lv = atoi(level);
    v = atoi(value);

    if( !victim->in_room ) {
  bug("Rgreward: victim no in_room !", 0);
  bug("victim is",0);
  bug( ppl,0);
  return;
    }

    if( !str_cmp(type, "exp")){
    
  for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
      if( IS_NPC(gch) ) continue;
      if( is_same_group( victim, gch )){
    if( (lv < 0 && gch->level + lv < 1 )
    ||  (lv > 0 && gch->level - lv > -1 )){
        gain_exp(gch, v);
        sprintf(buf,"§Aªº¸gÅç­È%s¤F %d ÂI¡I\n\r",
          (v < 0) ? "´î¤Ö" : "¼W¥[",
          v);
        send_to_char(buf, gch);
        //bugf("Room: %d rgreward exp %s %d", room->vnum, gch->name, v);
    }
      }
  }
    }
    else if( !str_cmp(type, "skexp")){
    
  for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
      if( IS_NPC(gch) ) continue;
      if( is_same_group( victim, gch )){
    if( (lv < 0 && gch->level + lv < 1 )
    ||  (lv > 0 && gch->level - lv > -1 )){
        gain_skill_exp(gch, v);
        sprintf(buf,"§Aªº§Þ¯à¸gÅç­È%s¤F %d ÂI¡I\n\r",
          (v < 0) ? "´î¤Ö" : "¼W¥[",
          v);
        send_to_char(buf, gch);
    }
      }
  }
    }
    else if ( !str_cmp(type, "gold")){
  for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
      if( IS_NPC(gch) ) continue;
      if( is_same_group( victim, gch )){
    if( (lv < 0 && gch->level + lv < 1 )
    ||  (lv > 0 && gch->level - lv > -1 )){
        gch->gold += v;
        sprintf(buf,"§Aªºª÷¿ú%s¤F %d ¤¸¡I\n\r",
          (v < 0) ? "´î¤Ö" : "¼W¥[",
          v);
        send_to_char(buf, gch);
        //bugf("Room: %d rgreward gold %s %d", room->vnum, gch->name, v);
    }
      }
  }
    }
    else if ( !str_cmp(type, "resp")){
  for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
      if( IS_NPC(gch) ) continue;
      if( is_same_group( victim, gch )){
    if( (lv < 0 && gch->level + lv < 1 )
    ||  (lv > 0 && gch->level - lv > -1 )){
        gch->pcdata->respect += v;
        sprintf(buf,"§Aªº«Â±æ%s¤F %d ÂI¡I\n\r",
          (v < 0) ? "¤U­°" : "´£°ª",
          v);
        send_to_char(buf, gch);
        //bugf("Room: %d rgreward resp %s %d", room->vnum, gch->name, v);
    }
      }
  }
    }
    else if (!str_cmp(type, "align")){
  for( gch = victim->in_room->people; gch; gch = gch->next_in_room ){
      if( IS_NPC(gch) ) continue;
      if( is_same_group( victim, gch )){
    if( (lv < 0 && gch->level + lv < 1 )
    ||  (lv > 0 && gch->level - lv > -1 )){
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
  bug("Rgreward: unknown type : %d", room->vnum);
  return;
    }
    return;
}

void do_rrecho( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
    if ( argument[0] == '\0' )
    {
       bug( "Echoat - No argument:  Room: %d.",
     room->vnum );
       return;
    }
    if ( !room->people )
  return;
    send_to_room( argument, room );
    return;
}


void do_rechoaround( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
	char       arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Echoaround - No argument:  Room: %d.", room->vnum );
		return;
	}

	target = room_find_char( room, arg );
	//target = world_find_char(room,arg);

	if ( !target )
	{
		bug( "Echoaround - target does not exist: Room: %d.",room->vnum );
		//bugf( "Echoaround - target does not exist: Room: %d. , Actor: %s.",room->vnum ,  actor->name );
		return;
	}

	act( argument, target, NULL, target, TO_NOTVICT );
	return;
}


void do_rechotoarea( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
  CHAR_DATA *cha;
    char buf [MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
  bug( "Rechotoarea - called w/o argument: vnum %d.",
      room->vnum );
  return;
    }
    sprintf(buf, "\n\r%s\n\r", argument);

  for(cha=char_list;cha;cha=cha->next)
        {
            if ( !IS_NPC(cha) && cha->in_room&& room->area == cha->in_room->area)
            {
                send_to_char(buf, cha);
            }
        }
}


void do_rechotoall( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
  CHAR_DATA *cha;
    char buf [MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
  bug( "Rechotoall - called w/o argument: vnum %d.",
      room->vnum );
  return;
    }
    sprintf(buf, "\n\r%s\n\r", argument);

  for(cha=char_list;cha;cha=cha->next)
  {
    if ( !IS_NPC(cha) && room != cha->in_room )
    {
      send_to_char(buf, cha);
    }
  }
}

void do_rnonechoat( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
	char       arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;


	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "NonEchoat - No argument:  Room: %d.", room->vnum );
		return;
	}

    target = room_find_char( room, arg );
    //target = world_find_char(room,arg);

	if ( !target )
	{
		bug( "NonEchoat - target does not exist: Room: %d.", room->vnum );
		//bugf( "NonEchoat - target does not exist: Room: %d. Actor: %s.",room->vnum ,  actor->name );
		return;
	}

    act_nonline( argument, target, NULL, target, TO_CHAR );
    return;
}

void do_rnonechoaround( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
	char       arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *target;
  
	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "NonEchoaround - No argument:  Room: %d.", room->vnum );
		return;
	}

	target = room_find_char( room, arg );
	//target = world_find_char(room,arg);

	if ( !target )
	{
		bug( "Echoaround - target does not exist: Room: %d.", room->vnum );
		//bugf( "NonEchoaround - target does not exist: Room: %d. Actor: %s.",room->vnum , actor->name );
		return;
	}
    
	act_nonline( argument, target, NULL, target, TO_NOTVICT );
	return;
}

void do_modrvt( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	char val[ MAX_INPUT_LENGTH ];
	int value;
	char *ptr;
	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Modrvt - No argument:  Room: %d.", room->vnum );
		return;
	}

	if (   arg[0] != 'v'
		&& arg[0] != 't'
		&& arg[0] != 'd'
		&& arg[0] != 'x' // ³]©w door ªº size 2022/12/20
		&& arg[0] != 'c' // ³]©w©Ð¶¡³Ì¤j¤H¼Æ 2022/12/17
		&& arg[0] != 's' // ³]©w©Ð¶¡char size 2022/12/20
		&& arg[0] != 'p' // ³]©w©Ð¶¡room size 2022/12/20
		&& str_cmp(arg, "self") )
	{
		if ( !( room = get_room_index( atoi( arg ) ) ) ){
			bug( "Modrvt - Room not exist:  Room: %d.", atoi( arg ) );
			return;
		}
		argument = one_argument( argument, arg );
	}
		/*   ·|³y¦¨ room to room force §ïªù­È¤£§@¥Î, ³y¦¨ room ³]©wµL prog room ¤£§@¥Î
		if ( !( room->roomprogs ) )
		{
			bug( "Modrvt - Room does not have room_prog:  Room: %d.", room->vnum );
			return;
		} */
	argument = one_argument( argument, opr );
	one_argument( argument, val );
	value = atoi( val );
	ptr = arg;
	ptr++;
	switch( arg[0] )
	{
		case 'v' : 
			// room ¥²¶·³]¦³room prog ¤~¦³ v ,t ­È
			if ( !( room->roomprogs ) )
			{
				bug( "Modrvt v-value - Room does not have room_prog:  Room: %d.", room->vnum );
				return;
			}
			if ( arg[1] == '0' )
				room->roomprogs->v0 = qmark_opr( room->roomprogs->v0, opr, value, "modrvt_v0" );
			else 
				room->roomprogs->v1 = qmark_opr( room->roomprogs->v1, opr, value, "modrvt_v1" );
			break;

		case 't' : 
			// room ¥²¶·³]¦³room prog ¤~¦³ v ,t ­È
			if ( !( room->roomprogs ) )
			{
				bug( "Modrvt t-value - Room does not have room_prog:  Room: %d.", room->vnum );
				return;
			}
			if( arg[1] == '0' || arg[1] == 'i' )
				room->roomprogs->timer[0] = qmark_opr( room->roomprogs->timer[0], opr, value, "modrvt_t0" );
			else if( arg[1] == '1' )
				room->roomprogs->timer[1] = qmark_opr( room->roomprogs->timer[1], opr, value, "modrvt_t1" );
			else
				bug( "Modrvt - bad loc:  Room: %d.", room->vnum );
			break;

		case 'd' :
			// ¦b¨S¦³©Ðªùªº±¡ªp¤UµLªk§@ modrvt , Á×§K crash 2022/12/22
			if( room->exit[atoi( ptr )] )
				room->exit[atoi( ptr )]->exit_info = qmark_opr( room->exit[atoi( ptr )]->exit_info, opr, value, "modrvt_d" );
			else
				return;
			break;

		case 'x' : // ªùªº¤j¤p­­¨î 2022/12/22
			if( room->exit[atoi( ptr )] )
				room->exit[atoi( ptr )]->exit_size = qmark_opr( room->exit[atoi( ptr )]->exit_size, opr, value, "modrvt_x" );
			else
				return;
			break;

		case 'c' : // ©Ð¶¡³Ì¤j¤H¼Æ­­¨î 2022/12/17
			room->max_char = qmark_opr( room->max_char, opr, value, "modrvt_c" );
			break;

		case 's' : // ¯à¶i¤J©Ð¶¡ªº char size ³Ì¤j­È 2022/12/20
			room->room_size = qmark_opr( room->room_size, opr, value, "modrvt_s" );
			break;

		case 'p' : // ©Ð¶¡¯à®e¯Çªº size Á`­È ( ©Ò¦³ char size ¥[Á` ) 2022/12/20
			room->room_space = qmark_opr( room->room_space, opr, value, "modrvt_p" );
			break;

		default : bug( "Modrvt - bad loc:  Room: %d.", room->vnum );
			return;
	}
}

// do_modhmv modified by jye 6.12.2005
void do_modhmv( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	char  arg[ MAX_INPUT_LENGTH ];
	char  min[ MAX_INPUT_LENGTH ];
	int hmv;
	int con;
	int num;
	int max;
	int type;
	int dam;
	int a;
	int bc;
	int d;
	int e;
	int fg;
	int h;
	char  *ptr;
	argument = one_argument( argument, arg );
	argument = one_argument( argument, min );
  
	if( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Modhmv - No target:  Room: %d.", room->vnum );
		return;
	}

	ptr = min;
	hmv = (int ) min[0] - 48;
	con = (int ) min[1] - 48;
	num = atoi( ptr + 3 );
	argument = one_argument( argument, min );
	max = atoi( min );
	argument = one_argument( argument, min );
	type = atoi( min );
	h = type % 10;
	fg = ( type % 1000 ) / 10;
	e = ( type % 10000 ) / 1000;
	d = ( type % 100000 ) / 10000;
	bc = ( type % 10000000 ) / 100000;
	a = ( type % 100000000 ) / 10000000;

	if( room->people )
		for( vch = room->people; vch; vch = vch->next_in_room )
		{
			// ¥[¤J§P©w npc ªº¥Ø¼Ð 2022/01/06
			//if( str_cmp( arg, "all" ) && str_cmp( arg, "pc" ) )
			if( str_cmp( arg, "all" ) && str_cmp( arg, "pc" ) && str_cmp( arg, "npc" ) )
				if( !is_name( arg, vch->name ) || ( IS_SET( vch->act, PLR_WIZINVIS )  && !IS_NPC(vch) ) )// ¥[¤J wizinv pc §P©w2022/10/07
					continue;

			if( !str_cmp( arg, "pc" ) )
				if( IS_NPC( vch ) )
					continue;

			// ¥Ø¼Ð¬° npc 2022/01/06
			if( !str_cmp( arg, "npc" ) )
				if( !IS_NPC( vch ) )
					continue;

			switch( con )
			{
				case 1:
						dam = -num;
						break;
				case 2:
						dam = num;
						break;
				case 3:
						dam =   ( hmv == 0 ) ? -vch->hit * num / 100 :
								( hmv == 1 ) ? -vch->mana * num / 100 :
								-vch->move * num / 100;
						break;
				case 4:
						dam =   ( hmv == 0 ) ? vch->hit * num / 100 :
								( hmv == 1 ) ? vch->mana * num / 100 :
								vch->move * num / 100;
						break;
				case 5:  // 2023/01/15
						dam =   ( hmv == 0 ) ? -vch->max_hit * num / 100 :
								( hmv == 1 ) ? -vch->max_mana * num / 100 :
								-vch->max_move * num / 100;
						break;
				case 6:  // 2023/01/15
						dam =   ( hmv == 0 ) ? vch->max_hit * num / 100 :
								( hmv == 1 ) ? vch->max_mana * num / 100 :
								vch->max_move * num / 100;
						break;
				default:
						bug( "Modhmv - min syntax error:  Room: %d.", room->vnum );
						return;
			}

			if ( a == 1 )
				dam = dam - UMIN( 0, GET_AC( vch ) * UMAX( 8, number_bits( 4 ) + 1 ) / 64 ) / UMIN( 1, bc );
			else if ( d == 1 )
				dam = dam * new_saves_spell( fg, vch, e ) / 70;
			if ( h == 1 && IS_AFFECTED( vch, AFF_SANCTUARY ) )
				dam = dam / 2;

			switch( hmv )
			{
				case 0 :
					vch->hit =   vch->hit + dam ;
					if( IS_NPC(vch) || IS_IMMORTAL( vch ))
						vch->hit = URANGE( 1, vch->hit , (vch->max_hit + (vch->max_hit / 10 )));
					else
						vch->hit = URANGE( -20, vch->hit , (vch->max_hit + (vch->max_hit / 10 )));
					update_pos( vch );

				// ¹ï¶Ë®`«á¨¤¦âª¬ºA¥[¤J¤¤¤å±Ô­z 2022/01/06
				switch( vch->position )
				{
					case POS_MORTAL:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", vch );
							act( "$n is mortally wounded, and will die soon, if not aided.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A¨ü¤F·¥ÄY­«ªº¶Ë¡A¦pªG¦A¤£ªvÀø¡A«Ü§Ö´N·|¦º¥h.\n\r", vch );
							act( "$n ¨ü¤F·¥ÄY­«ªº¶Ë¡A¦pªG¦A¤£ªvÀø¡A«Ü§Ö´N·|¦º¥h.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_INCAP:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You are incapacitated and will slowly die, if not aided.\n\r", vch );
							act( "$n is incapacitated and will slowly die, if not aided.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A¦]¶Ë­«¦ÓµLªk¦æ°Ê¡A¦pªG±o¤£¨ìªvÀø¡A´N¥u¦³µ¥¦º¤F.\n\r", vch );
							act( "$n ¦]¶Ë­«¦ÓµLªk¦æ°Ê¡A¦pªG±o¤£¨ìªvÀø¡A´N¥u¦³µ¥¦º¤F.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_STUNNED:
						if( !is_chinese(vch, NULL)){
							send_to_char("You are stunned, but will probably recover.\n\r", vch );
							act( "$n is stunned, but will probably recover.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A©ü­Ë¤F¡A¹L¤@·|¤]³\\·|¦^´_.\n\r", vch );
							act( "$n ©ü­Ë¤F¡A¹L¤@·|¤]³\\·|¦^´_.", vch, NULL, NULL, TO_ROOM );
						}
						break;
          
					case POS_DEAD:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You have been KILLED!!\n\r\n\r", vch );
							act( "$n is DEAD!!", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A¦º¤F¡I\n\r\n\r", vch );
							act( "$n ¦º¤F¡I", vch, NULL, NULL, TO_ROOM );
						}
						break;
          
					default:
						// dam ¦©¦å¬°­t­È , ¦¹³B§P©w¼g¤Ï 2022/01/16
						//if( dam > vch->max_hit / 4 )
						if( dam + (vch->max_hit / 4) < 0 )
							send_to_char( (is_chinese(vch, NULL)) ?
							HIY"¤Ñ­þ! µh¦º¤H¤F!"NOR"\n\r" :
							HIY"That really did HURT!"NOR"\n\r",
							vch );
						if( vch->hit < vch->max_hit / 4 )
							send_to_char( (is_chinese(vch, NULL)) ?
							HIR"§A¥¿¦b¼Q¦å!"NOR"\n\r" :
							HIR"You sure are BLEEDING!"NOR"\n\r",
							vch );
						break;
				}
				break;
				case 1 :
						vch->mana = vch->mana+dam ;
						vch->mana = URANGE(-100, vch->mana, (vch->max_mana + (vch->max_mana / 10)));
						break;
				case 2 :
						vch->move = vch->move + dam ;
						vch->move = URANGE(-100, vch->move, (vch->max_move + (vch->max_move / 10)));
						break;
				default :
						bug( "Modhmv - min syntax error:  Room: %d.", room->vnum );
			}
			update_pos(vch);
		}

	return;
}

void do_modhmvgroup( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	char  arg[ MAX_INPUT_LENGTH ];
	char  min[ MAX_INPUT_LENGTH ];
	int hmv;
	int con;
	int num;
	int max;
	int type;
	int dam;
	int a;
	int bc;
	int d;
	//  int e;
	int fg;
	int h;
	char  *ptr;
	argument = one_argument( argument, arg );
	argument = one_argument( argument, min );
  
	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Modhmv - No target:  Room: %d.", room->vnum );
		return;
	}

	ptr = min;
	hmv = (int ) min[0] - 48;
	con = (int ) min[1] - 48;
	num = atoi( ptr + 3 );
	argument = one_argument( argument, min );
	max = atoi( min );
	argument = one_argument( argument, min );
	type = atoi( min );
	h = type % 10;
	fg = ( type % 1000 ) / 10;
	//  e = ( type % 10000 ) / 1000;
	d = ( type % 100000 ) / 10000;
	bc = ( type % 10000000 ) / 100000;
	a = ( type % 100000000 ) / 10000000;

	if ( room->people )
		for ( vch = room->people; vch; vch = vch->next_in_room )
		{
			if ( !is_same_group( vch, ch ) )
				continue;
			switch( con )
			{
				case 1: dam = -num; break;
				case 2: dam = num; break;
				case 3: dam = ( hmv == 0 ) ?  -vch->hit * num / 100 :
								( hmv == 1 ) ? -vch->mana * num / 100 :
							-vch->move * num / 100;
						break;
				case 4: dam = ( hmv == 0 ) ? vch->hit * num / 100 :
								( hmv == 1 ) ? vch->mana * num / 100 :
								vch->move * num / 100;
						break;
				default:bug( "Modhmv - min syntax error:  Room: %d.", room->vnum );
						return;
			}
			if ( a == 1 )
				dam = dam - UMIN( 0, GET_AC( vch ) * UMAX( 8, number_bits( 4 ) + 1 ) / 64 ) / UMIN( 1, bc );
			else if ( d == 1 )
				dam = dam * saves_spell( fg, vch ) / 70;
			if ( h == 1 && IS_AFFECTED( vch, AFF_SANCTUARY ) )
				dam = dam / 2;
			switch ( hmv )
			{
				case 0 : vch->hit =   vch->hit + dam ;
						vch->hit = URANGE( 0, vch->hit , (vch->max_hit + (vch->max_hit / 10 )));
						update_pos( vch );

				// ¹ï¶Ë®`«á¨¤¦âª¬ºA¥[¤J¤¤¤å±Ô­z 2022/01/06
				switch( vch->position )
				{
					case POS_MORTAL:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", vch );
							act( "$n is mortally wounded, and will die soon, if not aided.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A¨ü¤F·¥ÄY­«ªº¶Ë¡A¦pªG¦A¤£ªvÀø¡A«Ü§Ö´N·|¦º¥h.\n\r", vch );
							act( "$n ¨ü¤F·¥ÄY­«ªº¶Ë¡A¦pªG¦A¤£ªvÀø¡A«Ü§Ö´N·|¦º¥h.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_INCAP:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You are incapacitated and will slowly die, if not aided.\n\r", vch );
							act( "$n is incapacitated and will slowly die, if not aided.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A¦]¶Ë­«¦ÓµLªk¦æ°Ê¡A¦pªG±o¤£¨ìªvÀø¡A´N¥u¦³µ¥¦º¤F.\n\r", vch );
							act( "$n ¦]¶Ë­«¦ÓµLªk¦æ°Ê¡A¦pªG±o¤£¨ìªvÀø¡A´N¥u¦³µ¥¦º¤F.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_STUNNED:
						if( !is_chinese(vch, NULL)){
							send_to_char("You are stunned, but will probably recover.\n\r", vch );
							act( "$n is stunned, but will probably recover.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A©ü­Ë¤F¡A¹L¤@·|¤]³\\·|¦^´_.\n\r", vch );
							act( "$n ©ü­Ë¤F¡A¹L¤@·|¤]³\\·|¦^´_.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_DEAD:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You have been KILLED!!\n\r\n\r", vch );
							act( "$n is DEAD!!", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "§A¦º¤F¡I\n\r\n\r", vch );
							act( "$n ¦º¤F¡I", vch, NULL, NULL, TO_ROOM );
						}
						break;

					default:
						// dam ¦©¦å¬°­t­È , ¦¹³B§P©w¼g¤Ï 2022/01/16
						//if( dam > vch->max_hit / 4 )
						if( dam + (vch->max_hit / 4) < 0 )
							send_to_char( (is_chinese(vch, NULL)) ?
							HIY"¤Ñ­þ! µh¦º¤H¤F!"NOR"\n\r" :
							HIY"That really did HURT!"NOR"\n\r",
							vch );
						if( vch->hit < vch->max_hit / 4 )
							send_to_char( (is_chinese(vch, NULL)) ?
							HIR"§A¥¿¦b¼Q¦å!"NOR"\n\r" :
							HIR"You sure are BLEEDING!"NOR"\n\r",
							vch );
						break;
				}
				break;
				case 1 : vch->mana = vch->mana+dam  ;
						vch->mana = URANGE(0, vch->mana, (vch->max_mana + (vch->max_mana / 10)));
						break;
				case 2 : vch->move = vch->move ;
						vch->move = URANGE(0, vch->move, (vch->max_move + (vch->max_move / 10)));
						break;
				default :
					bug( "Modhmv - min syntax error:  Room: %d.",room->vnum );
			}
			update_pos(vch);
		}

	return;
}

void do_rat( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	ROOM_INDEX_DATA	*location;
	CHAR_DATA *wch = NULL;
 
	argument = one_argument ( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug ( "rat - Bad argument: vnum %d.", room->vnum );
		return;
	}

	if ( is_number ( arg ) )
	{
		if ( ( location = get_room_index( atoi (arg) ) ) == NULL )
		{
			bug ( "Rat - No such location: vnum %d.", room->vnum );
			return;
		}
	}
	else
	{
		if ( char_list )
			for ( wch = char_list; wch; wch = wch->next )
				// ¥[¤J IS_NPC(wch) Á×§K mob act hunt ÅÜ¬° PLR_WIZINVIS ¤£§P©w 2022/10/07
				if ( is_name ( arg, wch->name ) && ( !IS_SET ( wch->act, PLR_WIZINVIS ) || IS_NPC(wch) ) )
					break;

		if ( !wch )
		{ 
			bug ( "Rat - No such location: vnum %d.", room->vnum );
			return;
		}

		//Added by Razgriz	20050707
		if ( wch->deleted )
		{
			bug ( "Rat - wch was deleted: room vnum %d.", room->vnum );
			return;
		}

			location = wch->in_room;
	}

	rinterpret ( location, ch, argument );

	return;
}
 
void do_def_rembch( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *vch;

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		bug( "Def_rembch - Bad syntax: vnum %d.", room->vnum );
		return;
	}

	if ( !str_cmp( "null-tar", arg ) )
	{
		if ( ( vch = room->rrembch ) )
			stop_rrember(room, ch);
		room->rrembch = NULL;
	}
	else
	{
		// §â room rember char ªº½d³ò°l¥[¦¨¥þ mud ¥Ø¼Ð 2022/10/04
		//vch = room_find_char( room, arg );
		vch = get_char_world( ch , arg);
		if ( vch )
		{
			add_rrember(room, vch);
		}
	}
}

void do_roload( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
  char arg1[ MAX_INPUT_LENGTH ];
  char arg2[ MAX_INPUT_LENGTH ];
  char arg3[ MAX_INPUT_LENGTH ];
  char arg4[ MAX_INPUT_LENGTH ];
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA     *obj;
  CHAR_DATA *target;
  int       level;
  int   site = 0;


  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  argument = one_argument( argument, arg3 );

  if( arg1[0] == '\0' || !is_number( arg1 ) )
  {
      bug( "Roload - Bad syntax: vnum %d.", room->vnum );
      return;
  }
    
  if( arg2[0] == '\0' )
  {
      bug( "Roload - level?: vnum %d.", room->vnum );
      return;
  }

  if( arg3[0] == '\0' )
  {
      target = NULL;
      site = 0;
  }
  else
  {
     if( !str_cmp( "target", arg3 ) )
     {
         target = NULL;
         site = 1;
     }
     else if ( !str_cmp( "floor", arg3 ) )
     {
         target = NULL;
         site = 0; 
     }
     else
     {
         target = room_find_char( room, arg3 );
         if( !target )
         {
             bug( "Roload - cannot find target in room.: vnum %d.", room->vnum );
             return;
         }
         
         one_argument( argument, arg4 );
         
         if( arg4 != '\0' )
         {
         	   if( !str_cmp("target", arg4))
                 site = 1;
         }
         else 
             site = 0;
     }
  }
  

         
  if( is_number( arg2 ) )
      level = atoi( arg2 );
  else
  {
      if( !target )
      {
          bug( "Roload - cannot find target in room.: vnum %d.", room->vnum );
          return;
      }
  
      if( !str_cmp( "viclv", arg2 ) )
      {
          level = target->level;
      }
      else
          level = number_fuzzy( target->level - 1 );
  }

  level = UMIN( number_fuzzy(LEVEL_IMMORTAL - 3 ) , level);

  if( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
  {
      bug( "Roload - Bad vnum arg: vnum %d.", room->vnum );
      return;
  }

  obj = fcreate_object( pObjIndex, level ) ;
    
  if( CAN_WEAR(obj, ITEM_TAKE) && site == 1)
  {
      if( !target ) 
      {
          bug( "Roload - cannot find target in room.: vnum %d.", room->vnum );
          return;
      }
      else
      {
          obj_to_char( obj, target );
          oprog_repop_trigger(obj);
      }
      return;
   }
   
   obj_to_room( obj, room );
   oprog_repop_trigger(obj);
}

void do_rmload( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char      arg[ MAX_INPUT_LENGTH ];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA    *target;

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
  bug( "Rmload - Bad vnum as arg: vnum %d.", room->vnum );
  return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
  bug( "Rmload - Bad mob vnum: vnum %d.", room->vnum );
  return;
    }

    target = create_mobile( pMobIndex );
    char_to_room( target, room );
    mprog_repop_trigger(target);
    return;
}

void do_set_room( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char type[ MAX_INPUT_LENGTH ];
	char buf [ MAX_INPUT_LENGTH ];
	char opr [ MAX_INPUT_LENGTH ];
	char keyword [ MAX_INPUT_LENGTH ];
	EXTRA_DESCR_DATA *key;
	int value;
	long long value_long; // ¼W¥[ long long value ­È 2023/01/10
	if ( !room )
		return;
	argument = one_argument( argument, type );
	if ( type[0] == '\0' || argument[0] == '\0' )
	{
		bug( "setroom - missing argument type: vnum %d.", room->vnum );
		return;
	}
	if ( !str_cmp( "sector", type ) )
	{
		if ( is_number( argument ) )
		{
			value = atoi( argument );
			room->sector_type = value;
			return;
		}
		else
		{
			bug( "setroom - Sector value is not an integer: vnum %d.", room->vnum );
			return;
		}
	}
	if ( !str_cmp( "flag", type ) )
	{
		argument = one_argument( argument, opr );
		if ( is_number( argument ) )
		{
			// °w¹ï­×§ï¼Æ­È·|¶W¹L int ªº¼W¥[¥H long long ¨Ó§P©w¼Æ­È 2023/01/10
			//value = atoi( argument );
			//room->room_flags = qmark_opr( room->room_flags, opr, value );
			value_long = atoi( argument );
			room->room_flags = ll_qmark_opr( room->room_flags, opr, value_long, "set_room_flag" );
			return;
		}
		else
		{
			bug( "setroom - Flag value is not an integer: vnum %d.", room->vnum );
			return;
		}
	}
	if ( !str_cmp( "name", type ) )
	{
		free_string( room->name );
		room->name = str_dup( argument );
		return;
	}
	buf[0] = '\0';
	if ( !str_cmp( "extra", type ) )
	{
		argument = one_argument( argument, keyword );
		if ( keyword[0] == '\0' )
		{
			bug( "setroom - Setting extra without keyword: vnum %d.", room->vnum );
			return;
		}
		if ( argument[0] == 'N' )
		{
			EXTRA_DESCR_DATA *ed;
			ed = new_extra_descr();
			argument++;
			argument++;
			ed->keyword = str_dup( keyword );
			ed->description = str_dup( argument );
			ed->deleted = FALSE;
			ed->next = room->extra_descr;
			room->extra_descr = ed;
			return;
		}
		if ( argument[0] != 'D' || argument[0] != 'A' )
			return;
		for ( key = room->extra_descr; key; key = key->next )
		{
			if ( !str_cmp( keyword, key->keyword ) )
			{
				if ( argument[0] == 'D' )
				{
					//free_string( key->keyword );
					//free_string( key->description );
					//key->deleted = TRUE;
					return;
				}
				else if ( argument[0] == 'A' )
				{
					argument++;
					argument++;
					if ( key->description )
					{
						strcat( buf, key->description );
						strcat( buf, "\n\r" );
					}
					strcat( buf, argument );
					key->description = str_dup( buf );
					return;
				}
			}
			else
			{
				bug( "setroom - Not such extra_desc keyword for editing: vnum %d.", room->vnum );
				return;
			}
		}
	}
	if ( !str_cmp( "desc", type ) )
	{
		if ( argument[0] == 'N' )
		{
			argument++;
			argument++;
		}
		else if ( argument[0] == 'A' )
		{
			argument++;
			argument++;
			if ( room->description )
				strcat( buf, room->description );
		}
		strcat( buf, argument );
		strcat( buf, "\n\r" );
		free_string( room->description );
		room->description = str_dup( buf );
		return;
	}
	else
	{
		bug( "setroom - Incorrect input type: vnum %d.", room->vnum );
		return;
	}
}

void do_add_affect( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	char arg3[ MAX_INPUT_LENGTH ];
	char arg4[ MAX_INPUT_LENGTH ];
	char arg5[ MAX_INPUT_LENGTH ];
	char arg6[ MAX_INPUT_LENGTH ];
	char arg7[ MAX_INPUT_LENGTH ];

	CHAR_DATA *target;
	int sn;
	int level = 0;
	int time;
	int loc;
	int mod;
	long long int bit;
	//int aff_exist = 0;  marked at 2023/01/25
	AFFECT_DATA af;

	argument = one_argument( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Add_affect - No target name: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Add_affect - No spellname: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' || !is_number( arg3) )
	{
		bug( "Add_affect - Bad syntax at level: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg4 );
	if ( arg4[0] == '\0' || argument[0] == '\0' || !is_number( arg4) )
	{
		bug( "Add_affect - Bad syntax at time: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg5 );
	if ( arg5[0] == '\0' || argument[0] == '\0' || !is_number( arg5) )
	{
		bug( "Add_affect - Bad syntax at location: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg6 );
	if ( arg6[0] == '\0' || argument[0] == '\0' || !is_number( arg6) )
	{
		bug( "Add_affect - Bad syntax at mod: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg7 );
	if ( arg7[0] == '\0' || !is_number( arg7) )
	{
		bug( "Add_affect - Bad syntax at bit: vnum %d.", room->vnum );
		return;
	}

	target = room_find_char( room, arg1 );
	if ( !target )
	{
		// Á×§K $r §ì¤£¨ì, ¬G²¾°£ 2022/12/18
		//bug( "Add_affect - target not in room: vnum %d ", room->vnum );
		return;
	}

	if ( ( sn = skill_lookup( arg2 ) ) == -1 )
	{
		bug( "Add_affect - strange spellname: vnum %d.", room->vnum );
		return;
	}

	level = atoi ( arg3 );

	/*  ­×§ï¬°ª½±µ§P©w level ¬O§_¦b 0 ¥H¤U 2023/01/25
		aff_exist = level / 1000;

		if ( aff_exist == 0 && is_affected( target, sn ) )
			return; */

	if ( level <= 0 && is_affected( target, sn ) )
		return;

	time  = atoi ( arg4 );
	loc   = atoi ( arg5 );
	mod   = atoi ( arg6 );
	bit   = atoll ( arg7 );

	//bit = strtoll(arg7, &tmp, 10);

	//bug(arg7,0);

	af.type       = sn;
	af.duration  = time;
	af.location  = loc;
	af.modifier  = mod;
	af.bitvector = bit;
	affect_join( target, &af );
}

void do_del_affect( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	int sn;
	AFFECT_DATA *paf;
	CHAR_DATA *target;
	CHAR_DATA *next_target;

	argument = one_argument( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "Del_affect - No target name: vnum %d.", room->vnum );
		return;
	}

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
		bug( "Add_affect - No spellname: vnum %d.", room->vnum );
		return;
	}
	/*   ­×§ï¦r¦ê§P©w, ¥H¥[¤J pc ¤Î npc §P©w 2022/07/12

	if (str_cmp( arg1, "all" ))
	{
		target = room_find_char( room, arg1 );
		if ( !target )
		{
			bug( "Del_affect - target not in room: vnum %d.", room->vnum );
			return;
		}

		if ( is_number( arg2 ) )
		{
			sn = atoi( arg2 );
			if ( target->affected )
				for ( paf = target->affected; paf; paf = paf->next )
					if ( paf->bitvector == sn )
						affect_remove( target, paf ); 
		}
		else if ( !str_cmp( arg2, "all" ) )
			for ( paf = target->affected; paf; paf = paf->next )
			{
				if ( paf->deleted )
					continue;

				affect_strip( target, paf->type );
			}
		else
		{
			if ( ( sn = skill_lookup( arg2 ) ) == -1 )
			{
				bug( "Del_affect - strange spellname: vnum %d.", room->vnum );
				return;
			}
			affect_strip( target, sn );
		}
	}
	else
	{
		for ( target = room->people; target ; target = next_target )
		{
			next_target = target->next_in_room;
			if ( !target->in_room || target->deleted )
				continue;

			if ( is_number( arg2 ) )
			{
				sn = atoi( arg2 );
				if ( target->affected )
					for ( paf = target->affected; paf; paf = paf->next )
						if ( paf->bitvector == sn )
							affect_remove( target, paf ); 
			}
			else if ( !str_cmp( arg2, "all" ) )
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;

					affect_strip( target, paf->type );
				}
			else
			{
				if ( ( sn = skill_lookup( arg2 ) ) == -1 )
				{
					bug( "Del_affect - strange spellname: vnum %d.", room->vnum );
					return;
				}
				affect_strip( target, sn );
			}
		}
	} */

	if (!str_cmp( arg1, "all" ))
	{
		for ( target = room->people; target ; target = next_target )
		{
			next_target = target->next_in_room;
			if ( !target->in_room || target->deleted )
				continue;

			// wizinv ¤£¨ü¨ì del_affect ¼vÅT 2022/07/13
			if( !IS_NPC(target) && IS_SET( target->act, PLR_WIZINVIS ) )
				continue;

			if ( is_number( arg2 ) )
			{
				sn = atoi( arg2 );
				if ( target->affected )
					for ( paf = target->affected; paf; paf = paf->next )
						if ( paf->bitvector == sn )
							affect_remove( target, paf ); 
			}
			else if ( !str_cmp( arg2, "all" ) )
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;

					affect_strip( target, paf->type );
				}
			else if ( !str_cmp( arg2, "random" ) ) // ÀH¾÷®ø°£¥Ø¼Ð¨­¤W affect 2022/07/13
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;
					if ( number_percent() >= 25 )
						continue;

					affect_strip( target, paf->type );
				}
			else
			{
				if ( ( sn = skill_lookup( arg2 ) ) == -1 )
				{
					bug( "Del_affect - strange spellname: vnum %d.", room->vnum );
					return;
				}
				affect_strip( target, sn );
			}
		}
	}
	else if( !str_cmp( arg1, "pc" ) ){  // ¥Ø¼Ð¬°¥þ room pc 2022/07/12
		for ( target = room->people; target ; target = next_target )
		{
			next_target = target->next_in_room;
			if ( !target->in_room || target->deleted )
				continue;

			if( IS_NPC(target) )
				continue;

			// wizinv ¤£¨ü¨ì del_affect ¼vÅT 2022/07/13
			// ¥[¤J !IS_NPC §P©wÁ×§K mob act hunt ÅÜ¬° PLR_WIZINVIS 2022/10/07
			if( IS_SET( target->act, PLR_WIZINVIS ) && !IS_NPC(target) )
				continue;

			if ( is_number( arg2 ) )
			{
				sn = atoi( arg2 );
				if ( target->affected )
					for ( paf = target->affected; paf; paf = paf->next )
						if ( paf->bitvector == sn )
							affect_remove( target, paf ); 
			}
			else if ( !str_cmp( arg2, "all" ) )
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;

					affect_strip( target, paf->type );
				}
			else if ( !str_cmp( arg2, "random" ) )
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;
					if ( number_percent() >= 25 )
						continue;

					affect_strip( target, paf->type );
				}
			else
			{
				if ( ( sn = skill_lookup( arg2 ) ) == -1 )
				{
					bug( "Del_affect - strange spellname: vnum %d.", room->vnum );
					return;
				}
				affect_strip( target, sn );
			}
		}
	}
	else if( !str_cmp( arg1, "npc" ) ){  // ¥Ø¼Ð¬°¥þ room npc 2022/07/12
		for ( target = room->people; target ; target = next_target )
		{
			next_target = target->next_in_room;
			if ( !target->in_room || target->deleted )
				continue;

			if( !IS_NPC(target) )
				continue;

			if ( is_number( arg2 ) )
			{
				sn = atoi( arg2 );
				if ( target->affected )
					for ( paf = target->affected; paf; paf = paf->next )
						if ( paf->bitvector == sn )
							affect_remove( target, paf ); 
			}
			else if ( !str_cmp( arg2, "all" ) )
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;

					affect_strip( target, paf->type );
				}
			else if ( !str_cmp( arg2, "random" ) )
				for ( paf = target->affected; paf; paf = paf->next )
				{
					if ( paf->deleted )
						continue;
					if ( number_percent() >= 25 )
						continue;

					affect_strip( target, paf->type );
				}
			else
			{
				if ( ( sn = skill_lookup( arg2 ) ) == -1 )
				{
					bug( "Del_affect - strange spellname: vnum %d.", room->vnum );
					return;
				}
				affect_strip( target, sn );
			}
		}
	}
	else
	{
		target = room_find_char( room, arg1 );
		if ( !target )
		{
			bug( "Del_affect - target not in room: vnum %d.", room->vnum );
			return;
		}

		if ( is_number( arg2 ) )
		{
			sn = atoi( arg2 );
			if ( target->affected )
				for ( paf = target->affected; paf; paf = paf->next )
					if ( paf->bitvector == sn )
						affect_remove( target, paf ); 
		}
		else if ( !str_cmp( arg2, "all" ) )
			for ( paf = target->affected; paf; paf = paf->next )
			{
				if ( paf->deleted )
					continue;

				affect_strip( target, paf->type );
			}
		else if ( !str_cmp( arg2, "random" ) )
			for ( paf = target->affected; paf; paf = paf->next )
			{
				if ( paf->deleted )
					continue;
				if ( number_percent() >= 50 )
					continue;

				affect_strip( target, paf->type );
			}
		else
		{
			if ( ( sn = skill_lookup( arg2 ) ) == -1 )
			{
				bug( "Del_affect - strange spellname: vnum %d.", room->vnum );
				return;
			}
			affect_strip( target, sn );
		}
	}
}

CHAR_DATA* room_find_char( ROOM_INDEX_DATA *room, char *name)
{
	CHAR_DATA *target = NULL;

	if ( room->people )
	{
		for ( target = room->people; target; target = target->next_in_room )
			if ( is_name( name, target->name )
				&& ( IS_NPC( target )
				|| ( !IS_NPC( target ) && !IS_SET( target->act, PLR_WIZINVIS ) ) ) )
			break;
		return target;
	}
	/*   remove at 2020/05/31 ÁY´î bug log
	bug(name,0);
	bug("%d",room->vnum);
	bug("room no people",0);
	*/
	return NULL;
}

CHAR_DATA* world_find_char( ROOM_INDEX_DATA *room, char *name)
{
	CHAR_DATA *target;

	for ( target = char_list; target; target = target->next )
	{
		if ( is_name( name, target->name ) && ( IS_NPC( target )
			|| ( !IS_NPC( target ) && !IS_SET( target->act, PLR_WIZINVIS ) ) ) )
		break;
	}
	return target;
}

void do_rforce( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH ];
    ROOM_INDEX_DATA *troom;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
        bug( "Rforce - Bad syntax: vnum %d.", room->vnum );
        return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
        CHAR_DATA *vch;

        for ( vch = room->people; vch; vch = vch->next_in_room )
        {
            if ( vch->deleted ) continue;
            /*if( !IS_NPC(vch) )*/ vch->forced = TRUE;        //Modified by Razgriz        20050907
            interpret( vch, argument );
        }
    }else if ( !is_number( arg ) ){
        CHAR_DATA *target;

        if ( !( target = room_find_char( room, arg ) ) )
        {
            bug( "Rforce - No such target: room vnum %d.", room->vnum );
			bugf( "Rforce - No such target: arg %s.", arg );
            return;
        }
		/*Amenda 90,9,23
		  if ( target == ch )
		  {
			  bug( "Rforce - Forcing oneself: vnum %d.",
			room->vnum );
			  return;
		  }
		*/
		/*if( !IS_NPC(target) )*/ target->forced = TRUE;	//Modified by Razgriz	20050907
        interpret( target, argument );
	}else{
		if ( !( troom = get_room_index( atoi( arg ) ) ) ) {
            bug( "Rforce - No such room: vnum %d.", room->vnum );
            return;
		}
		//ch->forced = TRUE;
		rinterpret( troom, ch, argument );
	}
}

CHAR_DATA *rget_char_room( ROOM_INDEX_DATA *room, char *argument )
{
	CHAR_DATA *rch;
	char arg [ MAX_INPUT_LENGTH ];
	int number;
	int count;

	number = number_argument( argument, arg );
	count  = 0;
	for ( rch = room->people; rch; rch = rch->next_in_room )
	{
		if ( !is_name( arg, rch->name ) )
			continue;
		if ( ++count == number )
			return rch;
	}
	for ( rch = room->people; rch; rch = rch->next_in_room )
	{
		if ( !is_pfxname( arg, rch->name ) )
			continue;
		if ( ++count == number )
			return rch;
	}

	return NULL;
}



void do_rcallout( ROOM_INDEX_DATA *ch,CHAR_DATA *cch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	int timer;
	RCALL_OUT_DATA *cod;
	argument = one_argument(argument, arg);
	if( !is_number(arg)) {
		bugf("rcallout - argument 1 must be timer. vnum %d", ch->vnum);
		return;
	}
	timer = atoi(arg);

	if( timer < 1 ) {
		bugf("rcallout - timer must > 0. vnum %d", ch->vnum);
		return;
	}

	cod = new_rcall_out();
	cod->owner = ch;
	cod->timer = timer;
	cod->cmd = str_dup(argument);
	cod->next  = rcall_out_list;
	rcall_out_list = cod;

	return;
}

void do_rcall( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char fun[MAX_STRING_LENGTH];
	char percent[MAX_STRING_LENGTH];
	CHAR_DATA *vch;
	CHAR_DATA *vch_next; // add at 2022/01/17
	int per = 100;

	argument = one_argument(argument , fun);
	argument = one_argument(argument , arg);
	argument = one_argument(argument , percent);

	if( fun[0] == '\0' ) {
		bugf("rcall : no function name in %d", room->vnum);
		return;
	}

	// ·s¼W rcall °w¹ï©Ð¶¡¤º pc , npc , all ªº®æ¦¡ 2022/01/17
	if( !str_cmp( arg, "all" ) )
	{
		for ( vch = room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( room->vnum != vch->in_room->vnum ) continue;
			rprog_defun_trigger( room, vch, fun );
		}
		return;
	}
	else if( !str_cmp( arg, "pc" ) )
	{
		for ( vch = room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( room->vnum != vch->in_room->vnum ) continue;
			if( IS_NPC(vch) ) continue;
			rprog_defun_trigger( room, vch, fun );
		}
		return;
	}
	else if( !str_cmp( arg, "npc" ) )
	{
		for ( vch = room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( room->vnum != vch->in_room->vnum ) continue;
			if( !IS_NPC(vch) ) continue;
			rprog_defun_trigger( room, vch, fun );
		}
		return;
	}
	// if( !( vch = rget_char_room( room, arg) )) 2022/01/17
	else if( !( vch = rget_char_room( room, arg) ))
	{
		bugf("rcall: no such char here in %d", room->vnum);
		return;
	}

	if( is_number( percent ) ){
		per = atoi(percent);
		per = URANGE( 0, per, 100);
	}
	if( per == 100 || number_percent() < per ) {
		rprog_defun_trigger( room, vch, fun );
	}
}

void do_rcast( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	char arg3[ MAX_INPUT_LENGTH ];
	char arg4[ MAX_INPUT_LENGTH ];
	CHAR_DATA * victim;
	int times;
	int sn, level;

	argument = one_argument( argument , arg1);
	if( is_number(arg1) )
	{
		times = atoi(arg1);
		argument = one_argument( argument, arg2);
		argument = one_argument( argument, arg3);
		argument = one_argument( argument, arg4);
		if( !(victim = rget_char_room( room, arg3 ) )){
			log_string(arg3);
			bugf("Rcast: no such target in %d, %s", room->vnum, ch->name);
			return;
		}
		sn = skill_lookup( arg2 );
		if( !str_cmp( arg4, "viclv"))
			level = victim->level;
		else if ( !is_number(arg4)){
			//bug("Rcast : level must be number in %d", room->vnum);
			bugf("Rcast : level must be number in %d, %s", room->vnum, ch->name); // 2022/12/29
			bug("Rcast sn: %d", sn); // 2022/12/29
			return;
		}
		else
			level = atoi(arg4);
	}else{
		times = 1;
		argument = one_argument( argument, arg2);
		argument = one_argument( argument, arg3);
		if( !(victim = rget_char_room( room, arg2 ) )){
			log_string(arg2);
			//bug("Rcast: no target in %d", room->vnum);
			bugf("Rcast: no such target in %d, %s", room->vnum, ch->name); // 2022/12/29
			return;
		}
		sn = skill_lookup( arg1 );
		if( !str_cmp( arg3, "viclv"))
			level = victim->level;
		else if ( !is_number(arg3)){
			//bug("Rcast : level must be number in %d", room->vnum);
			bugf("Rcast : level must be number in %d, %s", room->vnum, ch->name); // 2022/12/29
			bug("Rcast sn: %d", sn); // 2022/12/29
			return;
		}
		else
			level = atoi(arg3);
	}

	while( times-- ){
		room_cast_spell( sn, level, victim, ch, room);
	}

}

void do_rtransfer( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	OBJ_DATA *obj, *obj_next;
	ROOM_INDEX_DATA *location;
	CHAR_DATA *target, *target_next;
	int dir = -1;
	EXIT_DATA *pexit;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( !arg1 || !arg2 || arg1[0] == '\0' || arg2[0] == '\0' )// || // !is_number( arg2 ) )
	{
		bug( "Rtransfer - Bad syntax: vnum %d.", room->vnum );
		return;
	}

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
	else if( !str_prefix(arg2,"here")) // ·s¼W§â¯S©wªF¦è²¾¨ì¥Ø«e©Ð¶¡¤ºªº¦ì¸m 2022/11/20
	{
		dir = 100;
	}

	if ( dir == -1 && !is_number(arg2) )
	{
		bug( "Rtransfer - Bad syntax: vnum %d.", room->vnum );
		return;
	}

	if ( dir != -1 )
	{
		// dir 100 ³]©w¬°§â¥Ø¼Ð°e¨ì¥Ø«e©Ð¶¡ 2022/11/20
		if ( dir == 100 )
		{
			location = get_room_index( room->vnum );
		}
		else if ( !( pexit = room->exit[dir] ) || !( location = pexit->to_room ) )
		{
			// send_to_char( "[33m«u§r¡A¦¹¸ô¤£³q¡C[m\n\r", ch );
			return;
		}
	}

	if ( dir == -1 )
		location = get_room_index( atoi( arg2 ) );

	if( !location ) {
		bug("Rtransfer - No such room: %d." , room->vnum );
		return;
	}

	if ( !str_cmp( arg1, "all" ) )
	{
		if ( room->people )
		for ( target = room->people; target; target = target_next )
		{
			target_next = target->next_in_room;
			if( room->progtypes & ROOM_LEAVE)
				rprog_leave_trigger(room, target, 8);
			char_from_room( target );
			char_to_room( target, location );
			if( location->progtypes & ROOM_ENTER)
				rprog_entry_trigger(location, target, 6);
			if ( target->position > POS_DEAD ) // Added by Amethyst
				mprog_greet_trigger( target ); // Added by Amethyst
		}
		return;
	}
	else if ( !str_cmp( arg1, "pc" ))
	{
		if ( room->people )
			for ( target = room->people; target; target = target_next )
			{
				target_next = target->next_in_room;
				if (!IS_NPC(target))
				{
					if( room->progtypes & ROOM_LEAVE)
						rprog_leave_trigger(room, target, 8);
					char_from_room( target );
					char_to_room( target, location );
					if( location->progtypes & ROOM_ENTER)
						rprog_entry_trigger(location, target, 6);
					if ( target->position > POS_DEAD ) // Added by Amethyst
						mprog_greet_trigger( target ); // Added by Amethyst
				}
			}
		return;
	}
	else if ( !str_cmp( arg1, "npc" ))
	{
		if ( room->people )
			for ( target = room->people; target; target = target_next )
			{
				target_next = target->next_in_room;
				if (IS_NPC(target))
				{
					if( room->progtypes & ROOM_LEAVE)
						rprog_leave_trigger(room, target, 8);
					char_from_room( target );
					char_to_room( target, location );
					if( location->progtypes & ROOM_ENTER)
						rprog_entry_trigger(location, target, 6);
					if ( target->position > POS_DEAD ) // Added by Amethyst
						mprog_greet_trigger( target ); // Added by Amethyst
				}
			}
		return;
	}
	else if ( !str_cmp( arg1, "obj" ))
	{
		if( room->contents )
		{
			for ( obj = room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				obj_from_room( obj );
				obj_to_room( obj, location );
			}
		}
	}
	else
	{
		if ( !( target = room_find_char( room, arg1 ) ) )
		{
			bug( "Rtransfer - No such target in room: vnum %d.", room->vnum );
			return;
		}

		// ¦pªG target ¤£¦s¦b©ó¥ô¦ó©Ð¶¡¤¤ 2022/11/20
		if ( target->in_room == NULL )
		{
			bug( "Rtransfer - Victim in Limbo: vnum %d.", room->vnum );
			return;
		}

		if( room->progtypes & ROOM_LEAVE)
			rprog_leave_trigger(room, target, 8);
		char_from_room( target );
		char_to_room( target, location );
		if( location->progtypes & ROOM_ENTER)
			rprog_entry_trigger(location, target, 6);
		if ( target->position > POS_DEAD ) // Added by Amethyst
			mprog_greet_trigger( target ); // Added by Amethyst
	}
}


void do_rtransfergroup( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char       arg1[ MAX_INPUT_LENGTH ];
    char       arg2[ MAX_INPUT_LENGTH ];
    ROOM_INDEX_DATA *location;
    CHAR_DATA     *gtarget, *gtarget_next;
    int        dir = -1;
    EXIT_DATA     *pexit;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( !arg1 || !arg2 || arg1[0] == '\0' || arg2[0] == '\0' ){
		bug( "Rtransfergroup - Bad syntax: vnum %d.", room->vnum );
		return;
    }

    if ( !str_prefix(arg2,"up")){
		dir = 4;
    }
    else if( !str_prefix(arg2,"down")){
		dir = 5;
    }
    else if( !str_prefix(arg2,"north")){
        dir = 0;
    }
    else if( !str_prefix(arg2,"east")){
        dir = 1;
    }
    else if( !str_prefix(arg2,"south")){
        dir = 2;
    }
    else if( !str_prefix(arg2,"west")){
        dir = 3;
    }
	else if( !str_prefix(arg2,"here")) // ·s¼W§â¯S©wªF¦è²¾¨ì¥Ø«e©Ð¶¡¤ºªº¦ì¸m 2022/11/20
	{
		dir = 100;
	}


    if ( dir == -1 && !is_number(arg2) ){
		bug( "Rtransfergroup - Bad syntax: vnum %d.", room->vnum );
		return;
    }

    if ( dir != -1 ){
		// dir 100 ³]©w¬°§â¥Ø¼Ð°e¨ì¥Ø«e©Ð¶¡ 2022/11/20
		if ( dir == 100 )
		{
			location = get_room_index( room->vnum );
		}
		else if ( !( pexit = room->exit[dir] ) || !( location = pexit->to_room ) ){
			return;
		}
	}

    if ( dir == -1 )
		location = get_room_index( atoi( arg2 ) );

    if( !location ) {
		bug("Rtransfergroup - No such room: %d." , room->vnum );
		return;
    }

	if ( !( ch = room_find_char( room, arg1 ) ) )
	{
		bug( "Rtransfergroup - No such target in room: vnum %d.", room->vnum );
		return;
	}

	// ¦pªG ch ¤£¦s¦b©ó¥ô¦ó©Ð¶¡¤¤ 2022/11/20
	if ( ch->in_room == NULL )
	{
		bug( "Rtransfergroup - Victim in Limbo: vnum %d.", room->vnum );
		return;
	}

	for( gtarget = ch->in_room->people; gtarget; gtarget = gtarget_next ){
		gtarget_next = gtarget->next_in_room;
		
		if( gtarget == ch ) continue;
		if ( is_same_group( ch , gtarget ) ){
			if( gtarget->in_room->progtypes & ROOM_LEAVE)
				rprog_leave_trigger(gtarget->in_room, gtarget, 8);
				char_from_room( gtarget );
				char_to_room( gtarget, location );
			if( location->progtypes & ROOM_ENTER)
				rprog_entry_trigger(location, gtarget, 6);
			if( gtarget->position > POS_DEAD )
				mprog_greet_trigger( gtarget );
		}
	}
	
	if( ch->in_room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger(ch->in_room, ch, 8);
		char_from_room( ch );
		char_to_room( ch, location ); 
	if( location->progtypes & ROOM_ENTER)
		rprog_entry_trigger(location, ch, 6); 
	if( ch->position > POS_DEAD )
		mprog_greet_trigger( ch );
  
	return;
}

// ·s¼W¨âºØ wtransfer °w¹ï¥þ mud char §@ transfer 2022/11/21
void do_rwtransfer( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	OBJ_DATA *obj, *obj_next;
	ROOM_INDEX_DATA *location;
	CHAR_DATA *target, *target_next;
	int dir = -1;
	EXIT_DATA *pexit;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( !arg1 || !arg2 || arg1[0] == '\0' || arg2[0] == '\0' )// || // !is_number( arg2 ) )
	{
		bug( "RWtransfer - Bad syntax: vnum %d.", room->vnum );
		return;
	}

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
	else if( !str_prefix(arg2,"here")) // §â¯S©wªF¦è²¾¨ì¥Ø«e©Ð¶¡¤ºªº¦ì¸m
	{
		dir = 100;
	}

	if ( dir == -1 && !is_number(arg2) )
	{
		bug( "RWtransfer - Bad syntax: vnum %d.", room->vnum );
		return;
	}

	if ( dir != -1 )
	{
		// dir 100 ³]©w¬°§â¥Ø¼Ð°e¨ì¥Ø«e©Ð¶¡
		if ( dir == 100 )
		{
			location = get_room_index( room->vnum );
		}
		else if ( !( pexit = room->exit[dir] ) || !( location = pexit->to_room ) )
		{
			return;
		}
	}

	if ( dir == -1 )
		location = get_room_index( atoi( arg2 ) );

	if( !location ) {
		bug("RWtransfer - No such room: %d." , room->vnum );
		return;
	}

	// wtransfer ·|Àu¥ý§P©w©Ð¶¡¤¤ªº char ¦pªG¨S§ì¨ì¤~¥h§ä¨ä¥L room
	if ( !( target = room_find_char( room, arg1 ) ) )
	{
		if ( !( target = world_find_char( room, arg1 ) ) )
		{
			bug( "RWtransfer - No such target in world: vnum %d.", room->vnum );
			return;
		}
	}

	// ¦pªG target ¤£¦s¦b©ó¥ô¦ó©Ð¶¡¤¤
	if ( target->in_room == NULL )
	{
		bug( "RWtransfer - Victim in Limbo: vnum %d.", room->vnum );
		return;
	}

	if( room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger(room, target, 8);
	char_from_room( target );
	char_to_room( target, location );
	if( location->progtypes & ROOM_ENTER)
		rprog_entry_trigger(location, target, 6);
	if ( target->position > POS_DEAD )
		mprog_greet_trigger( target );
}


void do_rwtransfergroup( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char       arg1[ MAX_INPUT_LENGTH ];
    char       arg2[ MAX_INPUT_LENGTH ];
    ROOM_INDEX_DATA *location;
    CHAR_DATA     *gtarget, *gtarget_next;
    int        dir = -1;
    EXIT_DATA     *pexit;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( !arg1 || !arg2 || arg1[0] == '\0' || arg2[0] == '\0' ){
		bug( "RWtransfergroup - Bad syntax: vnum %d.", room->vnum );
		return;
    }

    if ( !str_prefix(arg2,"up")){
		dir = 4;
    }
    else if( !str_prefix(arg2,"down")){
		dir = 5;
    }
    else if( !str_prefix(arg2,"north")){
        dir = 0;
    }
    else if( !str_prefix(arg2,"east")){
        dir = 1;
    }
    else if( !str_prefix(arg2,"south")){
        dir = 2;
    }
    else if( !str_prefix(arg2,"west")){
        dir = 3;
    }
	else if( !str_prefix(arg2,"here")) // §â¯S©wªF¦è²¾¨ì¥Ø«e©Ð¶¡¤ºªº¦ì¸m
	{
		dir = 100;
	}


    if ( dir == -1 && !is_number(arg2) ){
		bug( "RWtransfergroup - Bad syntax: vnum %d.", room->vnum );
		return;
    }

    if ( dir != -1 ){
		// dir 100 ³]©w¬°§â¥Ø¼Ð°e¨ì¥Ø«e©Ð¶¡
		if ( dir == 100 )
		{
			location = get_room_index( room->vnum );
		}
		else if ( !( pexit = room->exit[dir] ) || !( location = pexit->to_room ) ){
			return;
		}
	}

    if ( dir == -1 )
		location = get_room_index( atoi( arg2 ) );

    if( !location ) {
		bug("RWtransfergroup - No such room: %d." , room->vnum );
		return;
    }

	// wtransfergroup ·|Àu¥ý§P©w©Ð¶¡¤¤ªº char ¦pªG¨S§ì¨ì¤~¥h§ä¨ä¥L room
	if ( !( ch = room_find_char( room, arg1 ) ) )
	{
		if ( !( ch = world_find_char( room, arg1 ) ) )
		{
			bug( "RWtransfergroup - No such target in world: vnum %d.", room->vnum );
			return;
		}
	}

	// ¦pªG ch ¤£¦s¦b©ó¥ô¦ó©Ð¶¡¤¤
	if ( ch->in_room == NULL )
	{
		bug( "RWtransfergroup - Victim in Limbo: vnum %d.", room->vnum );
		return;
	}

	for( gtarget = ch->in_room->people; gtarget; gtarget = gtarget_next ){
		gtarget_next = gtarget->next_in_room;
		
		if( gtarget == ch ) continue;
		if ( is_same_group( ch , gtarget ) ){
			if( gtarget->in_room->progtypes & ROOM_LEAVE)
				rprog_leave_trigger(gtarget->in_room, gtarget, 8);
				char_from_room( gtarget );
				char_to_room( gtarget, location );
			if( location->progtypes & ROOM_ENTER)
				rprog_entry_trigger(location, gtarget, 6);
			if( gtarget->position > POS_DEAD )
				mprog_greet_trigger( gtarget );
		}
	}
	
	if( ch->in_room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger(ch->in_room, ch, 8);
		char_from_room( ch );
		char_to_room( ch, location ); 
	if( location->progtypes & ROOM_ENTER)
		rprog_entry_trigger(location, ch, 6); 
	if( ch->position > POS_DEAD )
		mprog_greet_trigger( ch );
  
	return;
}


void do_rpurge( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ]; // add at 2020/10/24
	CHAR_DATA *target = NULL, *target_next;
	OBJ_DATA *obj, *obj_next;
    
	argument = one_argument( argument, arg );

	if( arg[0] == '\0' || !str_cmp( "all", arg ) )
	{
	/* 'purge' */

		if( room->people )
		{
			for( target = room->people; target != NULL; target = target_next )
			{
				target_next = target->next_in_room;
				if( !target->in_room || target->deleted )
					continue;
				if( IS_NPC( target ) ){
					if(target->pIndexData != NULL && IsSorGateMob(target->pIndexData->vnum)){
						if(target->master != NULL && !IS_NPC(target->master)){
						}else{
							extract_char( target, TRUE );
						}
					}else{
						extract_char( target, TRUE );
					}
				}
			}
		}

		if( room->contents )
			for( obj = room->contents; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if( obj->deleted )
					continue;
				extract_obj( obj );
			}
	}
	// purge ±¼©Ð¶¡¤¤©Ò¦³¯S©w¦W¦rªº obj ¤Î mob 2022/01/06
	else if ( !str_prefix( "all." , arg ) )
	{
		if ( room->people )
		{
			for ( target = room->people; target != NULL; target = target_next )
			{
				target_next = target->next_in_room;
				if ( !target->in_room || target->deleted )
					continue;
				if ( !IS_NPC( target ) )
					continue;
				//if ( ! (target = room_find_char( room, &arg[4] ) ) )
				//	continue;
				if( is_name( &arg[4], target->name ) )
					extract_char( target, TRUE );
			}
		}
		if( room->contents )
		{
			for( obj = room->contents; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if( obj->deleted )
					continue;
				if( is_name( &arg[4], obj->name ) )
				{
					extract_obj( obj );
				}
			}
		}
	}
	else if ( !str_cmp( arg, "allmob" ) )
	{
		if ( room->people )
			for ( target = room->people; target != NULL; target = target_next )
			{
				target_next = target->next_in_room;
				if ( !target->in_room || target->deleted )
					continue;
				if ( IS_NPC( target ) )
					extract_char( target, TRUE );
			}
	}
	// purge ±¼©Ð¶¡¤¤©Ò¦³¯S©w¦W¦rªº mob 2022/01/06
	else if ( !str_prefix( "allmob.", arg ) )
	{
		if ( room->people )
		{
			for ( target = room->people; target != NULL; target = target_next )
			{
				target_next = target->next_in_room;
				if ( !target->in_room || target->deleted )
					continue;
				if ( !IS_NPC( target ) )
					continue;
				//if ( ! (target = room_find_char( room, &arg[7] ) ) )
				//	continue;
				if( is_name( &arg[7], target->name ) )
					extract_char( target, TRUE );
			}
		}
	}
	// purge ±¼©Ð¶¡¤¤©Ò¦³ obj 2022/01/06
	else if ( !str_cmp( arg, "allobj" ) )
	{
		if( room->contents )
			for( obj = room->contents; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if( obj->deleted )
					continue;
				extract_obj( obj );
			}
	}
	// purge ±¼©Ð¶¡¤¤©Ò¦³¯S©w¦W¦rªº obj 2022/01/06
	else if ( !str_prefix( "allobj.", arg ) )
	{
		if( room->contents )
		{
			for( obj = room->contents; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if( obj->deleted )
					continue;
				if( is_name( &arg[7], obj->name ) )
				{
					extract_obj( obj );
				}
			}
		}
	}
	// °w¹ï©Ð¶¡¤¤¯S©w¦W mob §@¤@¦¸ purge 2022/01/06
	else if ( !str_prefix( "mob.", arg ) )
	{
		if ( room->people )
		{
			for ( target = room->people; target != NULL; target = target->next_in_room )
			{
				if ( !target->in_room || target->deleted )
					continue;
				if ( !IS_NPC( target ) )
					continue;
				//if ( ! (target = room_find_char( room, &arg[7] ) ) )
				//	continue;
				if( is_name( &arg[4], target->name ) )
					extract_char( target, TRUE );
			}
		}
	}
	// °w¹ï©Ð¶¡¤¤¯S©w¦W obj §@¤@¦¸ purge 2022/01/06
	else if ( !str_prefix( "obj.", arg ) )
	{
		if( room->contents )
		{
			for( obj = room->contents; obj != NULL; obj = obj->next_content )
			{
				if( obj->deleted )
					continue;
				if( is_name( &arg[4], obj->name ) )
				{
					extract_obj( obj );
				}
			}
		}
	}
	else
	{
        argument = one_argument( argument, arg1 );
		// °w¹ï¯S©w¥Ø¼Ð¨­¤W«ù¦³ªºª««~ vnum §@ purge 2020/03/05
		if ( !str_cmp( arg1, "vnum" ) )
		{
			argument = one_argument( argument, arg2 );
			int objvnum;
			if ( arg2[0] != '\0' )
			{
				target = room_find_char( room, arg );
				if ( !is_number( arg2 ) )
				{
					bug( "Rpurge - vnum must be number: room %d.", room->vnum );
					return;
				}
				if ( !target )
				{
					bug( "Rpurge - vnum target not found: vnum %d.", room->vnum );
					return;
				}
				for( obj = target->carrying; obj; obj = obj->next_content )
				{
					if ( obj->deleted )
						continue;
					objvnum = atoi( arg2 );
					if(obj->pIndexData->vnum == objvnum)
					{
						obj_from_char( obj );
						extract_obj( obj );
					}
				}
			}
			else
			{
				bug( "Rpurge - vnum arg2 not found: room %d.", room->vnum );
				return;
			}
		}
		// ¤@¦¸©Ê purge ±¼¥Ø¼Ð¯S©w vnum ªº obj 2022/01/06
		else if ( !str_cmp( arg1, "all.vnum" ) )
		{
			argument = one_argument( argument, arg2 );
			int objvnum;
			if ( arg2[0] != '\0' )
			{
				target = room_find_char( room, arg );
				if ( !is_number( arg2 ) )
				{
					bug( "Rpurge - all.vnum must be number: room %d.", room->vnum );
					return;
				}
				if ( !target )
				{
					bug( "Rpurge - all.vnum target not found: vnum %d.", room->vnum );
					return;
				}
				for( obj = target->carrying; obj; obj = obj_next )
				{
					obj_next = obj->next_content;
					if( obj->deleted )
						continue;
					objvnum = atoi( arg2 );
					if(obj->pIndexData->vnum == objvnum)
					{
						obj_from_char( obj );
						extract_obj( obj );
					}
				}
			}
			else
			{
				bug( "Rpurge - all.vnum arg2 not found: room %d.", room->vnum );
				return;
			}
		}
		else if ( arg1[0] != '\0' )
		{
			target = room_find_char( room, arg );
			if ( !target )
			{
				bug( "Rpurge - target not found: vnum %d.", room->vnum );
				return;
			}
			for( obj = target->carrying; obj; obj = obj->next_content )
			{
				if ( obj->deleted )
					continue;
				// ¥Ø¼Ð¨­¤Wª««~¦W¬°¼Æ¦r®É, ªý¾× purge ¨ì clan eq 2020/10/24
				if( is_number( arg1 ) && IS_SET(obj->extra_flags, ITEM_CLAN))
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
			target = room_find_char( room, arg );
			if ( target && IS_NPC(target) )
				extract_char( target, TRUE );
			else if ( room->contents )
			{
				for ( obj = room->contents; obj; obj = obj->next_content )
				{
					if ( obj->deleted )
						continue;
					if ( is_name( arg, obj->name ) )
						extract_obj( obj );
				}
			}
		}
    }
}

void do_rlog( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char  buf [ MAX_STRING_LENGTH ];

    sprintf( buf, "[*****] ROOMLOG: " );
    strcat( buf, argument );
    log_string( buf );
}

void do_rpdeldoor( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument)
{
    char arg [MAX_INPUT_LENGTH];
    EXIT_DATA * pexit;
    int exit;
    one_argument( argument, arg );
    if( arg[0] == '\0' || !is_number(arg) ){
  bug("[ROOMPROG] rpdeldoor: no argument or invalid argument in vnum %d", room->vnum);
  return;
    }
    exit = atoi(arg);
    if( exit < 0 || exit > 5 ) {
  bug("[ROOMPROG] rpdeldoor: no such door in vnum %d.", room->vnum);
  return;
    }

    if( (pexit = room->exit[exit] )) {
  free_mem( pexit/*, sizeof( *pexit ) */);
    }

    room->exit[exit] = NULL;
    --top_exit;
    return;
}

void do_rpsetdoor( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int exit;
    EXIT_DATA * pexit;

    argument = one_argument( argument, arg);
    if( arg[0] == '\0' || !is_number(arg)) {
  bug("rpsetdoor: invalid exit number in %d.", room->vnum);
  return;
    }

    exit = atoi(arg);
    if( exit < 0 || exit > 5 ) {
  bug("rpsetdoor: invalid exit number in %d.", room->vnum);
  return;
    }

    if( ! (pexit = room->exit[exit])) {
  bug("rpsetdoor: no such door in %d.", room->vnum);
  return;
    }

    argument = one_argument(argument, arg);
    if( !str_cmp(arg, "vnum") ){
  argument = one_argument(argument, arg);

  if( arg[0] == '\0' || !is_number(arg)) {
      bug("rpsetdoor: invalid to vnum in %d.", room->vnum);
      return;
  }
  pexit->vnum = atoi(arg);

  pexit->to_room = get_room_index(pexit->vnum);
    }
    else if ( !str_cmp(arg, "key") ) {
  argument = one_argument(argument, arg);

  if( arg[0] == '\0' || !is_number(arg)) {
      bug("rpsetdoor: invalid key in %d.", room->vnum);
      return;
  }
  pexit->key = atoi(arg);
    }
    else if (!str_cmp(arg, "flags")){
  argument = one_argument(argument, arg);

  if( arg[0] == '\0' || !is_number(arg)) {
      bug("[*****] rpsetdoor: invalid locks in %d.", room->vnum);
      return;
  }
  pexit->exit_info = atoi(arg);
  pexit->rs_flags = pexit->exit_info;

  switch ( pexit->exit_info )        /* OLC exit_info to rs_flags. */
  {
      case 1: pexit->rs_flags = EX_ISDOOR;                    break;
      case 2: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF;     break;
      case 3: pexit->rs_flags = EX_ISDOOR | EX_PASSPROOF;     break;
      case 4: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF
        | EX_PASSPROOF;                 break
      ;
      case 5: pexit->rs_flags = EX_ISDOOR | EX_BASHPROOF;     break;
      case 6: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF
        | EX_BASHPROOF;                 break
      ;
      case 7: pexit->rs_flags = EX_ISDOOR | EX_PASSPROOF
        | EX_BASHPROOF;                 break
      ;
      case 8: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF
        | EX_PASSPROOF | EX_BASHPROOF;  break
      ;
  }
    }
    else {
  bug("rpsetdoor: invalid option in %d.", room->vnum);
  return;
    }

    room->exit[exit] = pexit;
    return;
}

void do_rpmakedoor( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char desc[MAX_INPUT_LENGTH];
    char keyw[MAX_INPUT_LENGTH];
    int exit;
    EXIT_DATA * pexit;

    argument = one_argument( argument, arg );

    if( arg[0] == '\0' || !is_number(arg) ){
  bug("[*****] rpmakedoor: argument error in vnum %d.", room->vnum);
  return;
    }

    exit = atoi(arg);
    if( exit < 0 || exit > 5 ) {
  bug("[*****] rpmakedoor: invalid exit number in vnum %d.", room->vnum);
  return;
    }

    if( room->exit[exit] ) {
  bug("[*****] rpmakedoor: already has door in vnum %d.", room->vnum);
  bug("[*****] rpmakedoor: already has door %d.", exit);
  return;
    }

    pexit = alloc_mem( sizeof( *pexit ) );
    argument = one_argument(argument, desc);

    if( arg[0] == '\0' ) {
  bug("[*****] rpmakedoor: no description in %d.", room->vnum);
  return;
    }

    pexit->description = str_dup(desc);


    argument = one_argument(argument, keyw);

    if( arg[0] == '\0' ) {
  bug("[*****] rpmakedoor: no keyword in %d.", room->vnum);
  return;
    }
    pexit->keyword = str_dup(keyw);


    argument = one_argument(argument, arg);

    if( arg[0] == '\0' || !is_number(arg)) {
  bug("[*****] rpmakedoor: invalid locks in %d.", room->vnum);
  return;
    }
    pexit->exit_info = atoi(arg);
    pexit->rs_flags = pexit->exit_info;

    switch ( pexit->exit_info )        /* OLC exit_info to rs_flags. */
    {
  case 1: pexit->rs_flags = EX_ISDOOR;                    break;
  case 2: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF;     break;
  case 3: pexit->rs_flags = EX_ISDOOR | EX_PASSPROOF;     break;
  case 4: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF
    | EX_PASSPROOF;                 break
        ;
  case 5: pexit->rs_flags = EX_ISDOOR | EX_BASHPROOF;     break;
  case 6: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF
    | EX_BASHPROOF;                 break
        ;
  case 7: pexit->rs_flags = EX_ISDOOR | EX_PASSPROOF
    | EX_BASHPROOF;                 break
        ;
  case 8: pexit->rs_flags = EX_ISDOOR | EX_PICKPROOF
    | EX_PASSPROOF | EX_BASHPROOF;  break
        ;
    }



    argument = one_argument(argument, arg);

    if( arg[0] == '\0' || !is_number(arg)) {
  bug("[*****] rpmakedoor: invalid key in %d.", room->vnum);
  return;
    }
    pexit->key = atoi(arg);



    argument = one_argument(argument, arg);

    if( arg[0] == '\0' || !is_number(arg)) {
  bug("[*****] rpmakedoor: invalid to vnum in %d.", room->vnum);
  return;
    }
    pexit->vnum = atoi(arg);

    pexit->to_room = get_room_index(pexit->vnum);
    room->exit[exit] = pexit;
    ++top_exit;
    return;
}

