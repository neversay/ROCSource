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
//*****************************************************************
//*  PROGRAM ID  : obj_commands.c                                 *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jacquex                                *
//*  PURPOSE     :         增加opurge的功能和Room的purge功能一樣  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         新增 do_opset_obj_affect ()            *
//*              :         新增 do_opremove_obj_affect ()         *
//*              :         新增 do_opmodify_obj_affect ()         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         新增 do_opmodvt ()                     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Razgriz                                *
//*  PURPOSE     :         新增 obj_find ()                       *
//*****************************************************************
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "ansi.h"

//extern int qmark_opr args( ( int lhs, char *opr, int rhs ));
extern int qmark_opr args( ( int lhs, char *opr, int rhs, char *mod_name ));
extern long long ll_qmark_opr args( ( long long lhs, char *opr, long long rhs, char *mod_name )); // 增加數值較大的判定 2023/01/10

void do_ogoto( OBJ_DATA* obj, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	ROOM_INDEX_DATA *location;

	if( !obj ) return;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		bug( "ogoto - No argument: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	if (arg[0] >= '0' && arg[0] <= '9')
	{
		if ( ( location = find_location( ch, arg ) ) == NULL )
		{
			bug( "ogoto - No such location: vnum %d.", obj->pIndexData->vnum );
			return;
		}

		if( obj->carried_by )
			obj_from_char( obj );
		else
			obj_from_room( obj );
		obj_to_room( obj, location );
	}
	else if(strcmp(arg, "self") == 0)
	{
		if(ch == NULL || ch->in_room == NULL)
			return;
		if( obj->carried_by )
			obj_from_char( obj );
		else
			obj_from_room( obj );
		obj_to_room( obj, ch->in_room );
	}
	else
	{
		CHAR_DATA *pTargetChar = NULL;
		if ( (pTargetChar = world_find_char( NULL, arg ) ) == NULL )
		{
			return;
		}
		if( obj->carried_by )
			obj_from_char( obj );
		else
			obj_from_room( obj );
		obj_to_char( obj, pTargetChar );
	}
	return;
}

// 新增 ocast 2022/11/28
void do_ocast( OBJ_DATA* obj, CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	char arg3[ MAX_INPUT_LENGTH ];
	char arg4[ MAX_INPUT_LENGTH ];
	CHAR_DATA *victim;
	int times;
	int sn, level;

	if( !obj )
		return;

	if( !obj->carried_by )
		return;

	ch = obj->carried_by;

	argument = one_argument( argument , arg1);
	if( is_number(arg1) )
	{
		times = atoi(arg1);
		argument = one_argument( argument, arg2);
		argument = one_argument( argument, arg3);
		argument = one_argument( argument, arg4);
		if( !(victim = get_char_room( ch, arg3 ) ) )
		{
			//bugf("Ocast: no such target in %d, %s", obj->pIndexData->vnum, ch->name);
			return;
		}
		sn = skill_lookup( arg2 );
		if( !str_cmp( arg4, "viclv"))
			level = victim->level;
		else if ( !is_number(arg4))
		{
			bug("Ocast : level must be number in %d", obj->pIndexData->vnum);
			return;
		}
		else
			level = atoi(arg4);
	}else{
		times = 1;
		argument = one_argument( argument, arg2);
		argument = one_argument( argument, arg3);
		if( !(victim = get_char_room( ch, arg2 ) ) )
		{
			//bug("Ocast: no target in %d", obj->pIndexData->vnum);
			return;
		}
		sn = skill_lookup( arg1 );
		if( !str_cmp( arg3, "viclv"))
			level = victim->level;
		else if ( !is_number(arg3))
		{
			bug("Ocast : level must be number in %d", obj->pIndexData->vnum);
			return;
		}
		else
			level = atoi(arg3);
	}

	while( times-- ){
		obj_cast_spell( sn, level, ch, victim, obj );
	}
}

// 新增 obj rember 的指令 2022/11/10
void do_odef_rembch( OBJ_DATA* obj, CHAR_DATA *ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];
	CHAR_DATA *vch;

	if( !obj )
		return;

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		bug( "oDef_rembch - Bad syntax: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	if ( !str_cmp( "null-tar", arg ) )
	{
		if ( ( vch = obj->orembch ) )
			stop_orember(obj, ch);
		obj->orembch = NULL;
	}
	else
	{
		// obj rember char 的範圍為全 mud 的目標
		vch = get_char_world( ch , arg);
		if ( vch )
		{
			add_orember(obj, vch);
		}
	}
}

void do_oteleport( OBJ_DATA* obj, CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *pRoomIndex = NULL;
	int room1, room2, count = 0;
	const int times = 100;
	char arg1[MAX_INPUT_LENGTH];

	if( !obj )
		return;

	argument = one_argument( argument, arg1);
	if( arg1[0] == '\0' )
	{
		bug("[do_oteleport] : missing location", 0);
		return;
	}

	if( !str_cmp( arg1, "all" ) )
	{
		while( !pRoomIndex )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
		}
	}
	else if ( !str_cmp( arg1, "noclan" ) )
	{
		while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) )
				pRoomIndex = NULL;
			count++;
		}
	}
	else if ( !str_cmp( arg1, "recall1" ) )
	{
		while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) ||
				pRoomIndex->area->recall != 3001 )
				pRoomIndex = NULL;
			count++;
		}
	}
	else if ( !str_cmp( arg1, "recall2" ) )
	{
		while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) ||
				pRoomIndex->area->recall != 4580 )
				pRoomIndex = NULL;
			count++;
		}
	}
	else if ( !str_cmp( arg1, "recall3" ) )
	{
		while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
			if( IS_SET( pRoomIndex->area->area_flags, AREA_CLAN) ||
				pRoomIndex->area->recall != 14229 )
				pRoomIndex = NULL;
			count++;
		}
	}
	else if ( !str_cmp( arg1, "area" ) ) // 新增當前區域內 oteleport 2022/10/06
	{
		while( !pRoomIndex )
		{
			// 針對 obj 是否被人拿著的情況
			if( obj->carried_by )
				pRoomIndex = get_room_index( number_range( obj->carried_by->in_room->area->lvnum , obj->carried_by->in_room->area->uvnum  ) );
			else
				pRoomIndex = get_room_index( number_range( obj->in_room->area->lvnum , obj->in_room->area->uvnum  ) );
			count++;
		}
	}
	else if( !str_cmp( arg1, "vnum" ) )
	{
		argument = one_argument( argument, arg1);
		if( argument[0] == '\0' || arg1[0] == '\0' )
		{
			bug("[do_oteleport] : missing location range", 0);
			return;
		}
		if( ( is_number( arg1 ) ) )
			room1 = atoi(arg1);
		else
		{
			bug ( "Mob: %d 'oteleport' incorrect room number", ch->pIndexData->vnum );
			return;
		}
		if( ( is_number( argument ) ) )
			room2 = atoi(argument);
		else
		{
			bug ( "Mob: %d 'oteleport' incorrect room number", ch->pIndexData->vnum );
			return;
		}
		if ( room1 > room2 )
		{
			bug ( "Mob: %d 'oteleport' value 1 need to be smaller than value 2", ch->pIndexData->vnum );
			return;
		}

		while( !pRoomIndex )
		// while( !pRoomIndex || times != count )
		{
			pRoomIndex = get_room_index( number_range( room1, room2  ) );
			count++;
		}
	}
	else
	{
		bug ( "Mob: %d 'oteleport' Incorrect type.", ch->pIndexData->vnum );
		return;
	}

	if( !pRoomIndex )
	{
		//bug ( "Mob: %d 'mpteleport' Room cannot be found", ch->pIndexData->vnum );
		return;
	}
	if ( pRoomIndex == NULL ) pRoomIndex = 11002;
	if( obj->carried_by )
		obj_from_char( obj );
	else
		obj_from_room( obj );
	obj_to_room( obj, pRoomIndex );
    return;
}

void do_oforcer( OBJ_DATA *obj,CHAR_DATA* ch, char*argument)
{
    ROOM_INDEX_DATA *room = NULL;
    char arg[ MAX_INPUT_LENGTH ];

	//if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) )
	//{
	//	send_to_char( "什麼?\n\r", ch );
	//	return;
	//  }

    //if( !IS_NPC(ch) ){
	//send_to_char("什麼?\n\r", ch);
	//return;
    //}

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	bug( "Oforcer - Bad syntax: vnum %d.", obj->pIndexData->vnum );
	return;
    }

    if( !str_cmp( arg, "here" )) 
    {
    	if (obj->carried_by)
    		room = obj->carried_by->in_room;
    	else
    		room = obj->in_room;
    }
    else if( !is_number(arg))
    {
	    bug( "Oforcer - Force which room to do what?", obj->pIndexData->vnum );
		    return;
    }
    else
    {
	    room = get_room_index( atoi(arg));
    }
    if( !room )
    {
	    bug("Oforcer - No such location.", obj->pIndexData->vnum);;
	    return;
    }
    rinterpret( room, ch,argument );

    return;
}

void do_oforce( OBJ_DATA *obj,CHAR_DATA* ch, char *argument )
{
	char arg[ MAX_INPUT_LENGTH ];

	//  if( !IS_NPC(ch) ){
	//	send_to_char("什麼?\n\r", ch);
	//	return;
	//  }
	if(!obj)
		return;
	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "oforce - Bad syntax: vnum %d.", obj->pIndexData->vnum );
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
			if ( obj->carried_by && vch->in_room == obj->carried_by->in_room && get_trust(vch) < 43)
			{
				vch->forced = TRUE; // oforce 能成立 forced 2022/10/27
				interpret(vch,argument);
			}
			else if ( vch->in_room == obj->in_room && get_trust(vch)< 43) 
			// && get_trust( vch ) < get_trust( ch ) 
			// && can_see( ch, vch ) )
			{
				vch->forced = TRUE; // oforce 能成立 forced 2022/10/27
				interpret(vch, argument );
			}
		}
    }
    else
    {
		CHAR_DATA *target;
		if (obj->carried_by == NULL )
		{
			if ( ( target = room_find_char( obj->in_room, arg ) ) == NULL )
			{
				//bug( "oforce - No such target in room: vnum %d.",obj->pIndexData->vnum );
				return;
			}
		}else if ( ( target = get_char_room( obj->carried_by, arg ) ) == NULL ){
			//bug( "oforce - No such target: vnum %d.",obj->pIndexData->vnum );
			return;
		}

		// if ( target == ch )
		// {
		//    bug( "oforce - Forcing oneself: vnum %d.",
		//    ch->pIndexData->vnum );
		//    return;
		// }
		target->forced = TRUE; // oforce 能成立 forced 2022/10/27
		interpret(target, argument );
    }

    return;

}

void do_oechoat( OBJ_DATA *room, CHAR_DATA *actor, char *argument )
{
  char arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;


    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Echoat - No argument:  Obj: %d.",
	   room->pIndexData->vnum );
       return;
    }

    if( !actor->in_room ) return;
    target = room_find_char( actor->in_room, arg );
    //target = world_find_char(room,arg);

    if ( !target )
    {
	bug( "Echoat - target does not exist: Obj: %d.",
	    room->pIndexData->vnum );
	return;
    }

    act( argument, target, NULL, target, TO_CHAR );
    return;
}


void do_ooecho(OBJ_DATA *obj, CHAR_DATA *actor, char *argument )
{
    if ( argument[0] == '\0' )
    {
       bug( "Echoat - No argument:  Obj: %d.",
	   obj->pIndexData->vnum);
       return;
    }
	if( !actor ) return;
    if( !actor->in_room ) return;
    send_to_room( argument, actor->in_room );
    return;
}

void do_oechoaround( OBJ_DATA *obj, CHAR_DATA *actor, char *argument )
{
  char	     arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;
  
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Echoat - No argument:  obj: %d.",
	   obj->pIndexData->vnum );
       return;
    }

    if( !actor->in_room ) return;
    target = room_find_char(actor->in_room, arg );
    //target = world_find_char(room,arg);

    if ( !target )
    {
	bug( "Echoaround - target does not exist: Obj: %d.",
	    obj->pIndexData->vnum );
	return;
    }
    
    act( argument, target, NULL, target, TO_NOTVICT );
    return;
}

void do_oechotoall( OBJ_DATA *room, CHAR_DATA *actor, char *argument )
{
	CHAR_DATA *cha;
    char buf [MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
		bug( "Oechotoall - called w/o argument: vnum %d.", room->pIndexData->vnum );
		return;
    }
    sprintf(buf, "\n\r%s\n\r", argument);

	for(cha=char_list;cha;cha=cha->next)
	{
		if ( !IS_NPC(cha) )
		{
			send_to_char(buf, cha);
		}
	}
}

void do_ononechoat( OBJ_DATA *room, CHAR_DATA *actor, char *argument )
{
  char arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;


    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Echoat - No argument:  Obj: %d.", room->pIndexData->vnum );
       return;
    }

    if( !actor->in_room ) return;
    target = room_find_char( actor->in_room, arg );
    //target = world_find_char(room,arg);

    if ( !target )
    {
		bug( "Echoat - target does not exist: Obj: %d.", room->pIndexData->vnum );
		return;
    }

    act_nonline( argument, target, NULL, target, TO_CHAR );
    return;
}

void do_ononechoaround( OBJ_DATA *obj, CHAR_DATA *actor, char *argument )
{
  char arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *target;
  
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Echoat - No argument:  obj: %d.", obj->pIndexData->vnum );
       return;
    }

    if( !actor->in_room ) return;
    target = room_find_char(actor->in_room, arg );
    //target = world_find_char(room,arg);

    if ( !target )
    {
		bug( "Echoaround - target does not exist: Obj: %d.", obj->pIndexData->vnum );
		return;
    }
    
    act_nonline( argument, target, NULL, target, TO_NOTVICT );
    return;
}


/* Added by Razgriz 20050918
功能:
  改變 obj.extra_value 的 used v0 v1 t0 t1

語法:
  opmodvt victim object field opr modifier

  victim   (字串): self,  $o	//obj 本身, 不必再設 object
                   owner, $n	//obj->carried_by
                   char_id	//跟 obj 同房間的 char id
                   room		//房間中的 obj
  object   (數字): object vnum
           (字串): object name
  field    (字串): used v0 v1 t0 t1
  opr      (符號): + - & | r =
  modifier (數字): 配合 field 填入適當數值
*/
void do_opmodvt ( OBJ_DATA *pobj, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *	victim	= NULL;
	OBJ_DATA *	obj	= NULL;
	ROOM_INDEX_DATA *	room	= NULL;
	char		arg1	[ MAX_INPUT_LENGTH ];
	char		arg2	[ MAX_INPUT_LENGTH ];
	char		arg3	[ MAX_INPUT_LENGTH ];
	char		opr	[ MAX_INPUT_LENGTH ];
	char		arg5	[ MAX_INPUT_LENGTH ];
	int		vnum	= -999;
	int		mod;
	bool		fSelf	= FALSE;
	bool		fFind	= FALSE;

	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opmodvt - No victim name: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//if victim = self, we do not need to read object
	if ( str_cmp ( arg1 , "self" ) && str_cmp ( arg1 , "$o" ))
	{
		argument = one_argument ( argument, arg2 );
		if ( arg2[0] == '\0' || argument[0] == '\0' )
		{
			bug( "opmodvt - No object name: vnum %d.", pobj->pIndexData->vnum );
			return;
		}
	}
	else
	{
		fSelf = TRUE;
		arg2[0] = '\0';
		obj = pobj;
	}

	argument = one_argument ( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opmodvt - No field: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, opr );
	if ( opr[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opmodvt - No operator: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg5 );
	if ( arg5[0] == '\0' || !is_number ( arg5 ) )
	{
		bug( "opmodvt - Bad syntax at modifier: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//we need a room
	if ( pobj->carried_by && pobj->carried_by->in_room )
		room = pobj->carried_by->in_room;
	else if ( pobj->in_room )
		room = pobj->in_room;
	else
	{
		bug( "opmodvt - no room can be find: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//transform args to local variable
	if ( is_number ( arg2 ) )
		vnum = atoi ( arg2 );
	mod  = atoi ( arg5 );

	//find obj from different victim
	if ( fSelf )
	{
		fFind = TRUE;
	}
	else if ( !str_cmp ( arg1, "room" ) )
	{
		//find obj in room ,obj can't be in container
		for ( obj = room->contents; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}
	else
	{
		//find victim
		victim = ( !str_cmp ( arg1, "owner" ) || !str_cmp ( arg1, "$n" ) ) ? pobj->carried_by : room_find_char ( room , arg1 );

		if ( !victim )
		{
			bug( "opmodvt - no victim can be find: vnum %d.", pobj->pIndexData->vnum );
			return;
		}

		//find obj carried by victim , obj can't be in container
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}

	//modify new field value to object
	if ( fFind )
	{
		//parse field
		if ( !strcmp ( arg3, "used" ) )
		{
			if ( qmark_opr ( ( obj->extra_value ).used , opr , mod, "opmodvt_used" ) > 0 )
				( obj->extra_value ).used = TRUE;
			else
				( obj->extra_value ).used = FALSE;
			return;
		}
		else if ( !strcmp ( arg3, "v0" ) )
			( obj->extra_value ).v[0] = qmark_opr ( ( obj->extra_value ).v[0] , opr , mod, "opmodvt_usedv0" );
		else if ( !strcmp ( arg3, "v1" ) )
			( obj->extra_value ).v[1] = qmark_opr ( ( obj->extra_value ).v[1] , opr , mod, "opmodvt_usedv0" );
		else if ( !strcmp ( arg3, "t0" ) )
			( obj->extra_value ).t[0] = qmark_opr ( ( obj->extra_value ).t[0] , opr , mod, "opmodvt_usedv0" );
		else if ( !strcmp ( arg3, "t1" ) )
			( obj->extra_value ).t[1] = qmark_opr ( ( obj->extra_value ).t[1] , opr , mod, "opmodvt_usedv0" );
		else
		{
			bug( "opmodvt - Bad syntax at field: vnum %d.", pobj->pIndexData->vnum );
			return;
		}

		//auto set used = true, when v0 v1 t0 t1 be modified
		( obj->extra_value ).used = TRUE;
		return;
	}

	bug( "opmodvt - no item can be find: vnum %d.", pobj->pIndexData->vnum );
	return;
}

/* Added by Razgriz
   find an object depend on argument with the object viewpoint.
   now this function is used in mprog_do_ifchck - "objextra"
   argument will be :
   ---------------------------------
   self / $o                         , XXXX
   ---------------------------------
   owner / $n ,  obj_vnum / obj_name , XXXX
   char_id    ,  obj_vnum / obj_name , XXXX
   room       ,  obj_vnum / obj_name , XXXX
   ---------------------------------

   ex: ( $o, XXXX ) or ( room, sword, XXXX ) .....
*/
OBJ_DATA * obj_find ( OBJ_DATA *pobj, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *	victim	= NULL;
	OBJ_DATA *	obj	= NULL;
	ROOM_INDEX_DATA *	room	= NULL;
	char		arg1	[ MAX_INPUT_LENGTH ];
	char		arg2	[ MAX_INPUT_LENGTH ];
	int		vnum	= -999;
	bool		fSelf	= FALSE;
	bool		fFind	= FALSE;
	char *		pchar	= '\0';

	smash_tilde ( argument );

	//exchange ',' to ' '
	for ( pchar = argument; *pchar; pchar++ )
	{
		if ( *pchar == ',' )
			*pchar = ' ';
	}

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "obj_find - No victim name: vnum %d.", pobj->pIndexData->vnum );
		return NULL;
	}

	//if victim = self, we do not need to read object
	if ( str_cmp ( arg1 , "self" ) && str_cmp ( arg1 , "$o" ))
	{
		argument = one_argument ( argument, arg2 );
		if ( arg2[0] == '\0' || argument[0] == '\0' )
		{
			bug( "obj_find - No object name: vnum %d.", pobj->pIndexData->vnum );
			return NULL;
		}
	}
	else
	{
		fSelf = TRUE;
		arg2[0] = '\0';
		obj = pobj;
	}

	//we need a room
	if ( pobj->carried_by && pobj->carried_by->in_room )
		room = pobj->carried_by->in_room;
	else if ( pobj->in_room )
		room = pobj->in_room;
	else
	{
		bug( "obj_find - no room can be find: vnum %d.", pobj->pIndexData->vnum );
		return NULL;
	}

	//transform args to local variable
	if ( is_number ( arg2 ) )
		vnum = atoi ( arg2 );

	//find obj from different victim
	if ( fSelf )
	{
		fFind = TRUE;
	}
	else if ( !str_cmp ( arg1, "room" ) )
	{
		//find obj in room ,obj can't be in container
		for ( obj = room->contents; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}
	else
	{
		//find victim
		victim = ( !str_cmp ( arg1, "owner" ) || !str_cmp ( arg1, "$n" ) ) ? pobj->carried_by : room_find_char ( room , arg1 );

		if ( !victim )
		{
			bug( "obj_find - no victim can be find: vnum %d.", pobj->pIndexData->vnum );
			return NULL;
		}

		//find obj carried by victim , obj can't be in container
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}

	//if find object return it
	if ( fFind )
	{
		return obj;
	}

	bug( "obj_find - no item can be find: vnum %d.", pobj->pIndexData->vnum );
	return NULL;
}


void do_omodhmv(OBJ_DATA *obj,CHAR_DATA *actor,char *argument)
{
	CHAR_DATA *vch;
	char arg[ MAX_INPUT_LENGTH ];
	char min[ MAX_INPUT_LENGTH ];
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
	char *ptr;
	argument = one_argument( argument, arg );
	argument = one_argument( argument, min );
  
	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		bug( "oModhmv - No target:  Obj: %d.", obj->pIndexData->vnum );
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

	if ( obj->carried_by && obj->carried_by->in_room->people )
		for ( vch = obj->carried_by->in_room->people; vch; vch = vch->next_in_room )
		{
			// 加入判定 npc 的目標 2022/01/06
			if ( str_cmp( arg, "all" ) && str_cmp( arg, "pc" ) && str_cmp( arg, "npc" ) )
				if ( !is_name( arg, vch->name ) || ( IS_SET( vch->act, PLR_WIZINVIS ) && !IS_NPC(vch) ) )// 加入 wizinv pc 判定2022/10/07
					continue;

			if ( !str_cmp( arg, "pc" ) )
				if ( IS_NPC( vch ) )
					continue;
				
			if ( !str_cmp( arg, "npc" ) )
				if ( !IS_NPC( vch ) )
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
					dam = ( hmv == 0 ) ?  -vch->hit * num / 100 :
							( hmv == 1 ) ? -vch->mana * num / 100 :
							-vch->move * num / 100;
					break;
			case 4:
					dam = ( hmv == 0 ) ?  vch->hit * num / 100 :
							( hmv == 1 ) ? vch->mana * num / 100 :
							vch->move * num / 100;
					break;
			default:
					bug( "oModhmv - min syntax error:  Obj: %d.", obj->pIndexData->vnum );
					return;
		}
		if ( a == 1 )
			dam = dam - UMIN( 0, GET_AC( vch ) * UMAX( 8, number_bits( 4 ) + 1 ) / 64 ) / UMIN( 1, bc );
		else if ( d == 1 )
			dam = dam * new_saves_spell( fg, vch, e ) / 70;
		if ( h == 1 && IS_AFFECTED( vch, AFF_SANCTUARY ) )
			dam = dam / 2;
		switch ( hmv )
		{
			case 0 : vch->hit =   vch->hit + dam ;
				if ( IS_NPC(vch) )
					vch->hit = URANGE( 1, vch->hit , (vch->max_hit + (vch->max_hit / 10 )));
				else
					vch->hit = URANGE( -20, vch->hit , (vch->max_hit + (vch->max_hit / 10 )));
				update_pos( vch );
				// 對傷害後角色狀態加入中文敘述 2022/01/06
				switch( vch->position )
				{
					case POS_MORTAL:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", vch );
							act( "$n is mortally wounded, and will die soon, if not aided.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "你受了極嚴重的傷，如果再不治療，很快就會死去.\n\r", vch );
							act( "$n 受了極嚴重的傷，如果再不治療，很快就會死去.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_INCAP:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You are incapacitated and will slowly die, if not aided.\n\r", vch );
							act( "$n is incapacitated and will slowly die, if not aided.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "你因傷重而無法行動，如果得不到治療，就只有等死了.\n\r", vch );
							act( "$n 因傷重而無法行動，如果得不到治療，就只有等死了.", vch, NULL, NULL, TO_ROOM );
						}
						break;

					case POS_STUNNED:
						if( !is_chinese(vch, NULL)){
							send_to_char("You are stunned, but will probably recover.\n\r", vch );
							act( "$n is stunned, but will probably recover.", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "你昏倒了，過一會也許\會回復.\n\r", vch );
							act( "$n 昏倒了，過一會也許\會回復.", vch, NULL, NULL, TO_ROOM );
						}
						break;
          
					case POS_DEAD:
						if( !is_chinese(vch, NULL)){
							send_to_char( "You have been KILLED!!\n\r\n\r", vch );
							act( "$n is DEAD!!", vch, NULL, NULL, TO_ROOM );
						}else{
							send_to_char( "你死了！\n\r\n\r", vch );
							act( "$n 死了！", vch, NULL, NULL, TO_ROOM );
						}
						break;
          
					default:
						// dam 扣血為負值 , 此處判定寫反 2022/01/16
						//if( dam > vch->max_hit / 4 )
						if( dam + (vch->max_hit / 4) < 0 )
							send_to_char( (is_chinese(vch, NULL)) ?
							HIY"天哪! 痛死人了!"NOR"\n\r" :
							HIY"That really did HURT!"NOR"\n\r",
							vch );
						if( vch->hit < vch->max_hit / 4 )
							send_to_char( (is_chinese(vch, NULL)) ?
							HIR"你正在噴血!"NOR"\n\r" :
							HIR"You sure are BLEEDING!"NOR"\n\r",
							vch );
						break;
				}
				break;
				case 1 :
						vch->mana =   vch->mana+dam  ;
						vch->mana = URANGE(-100, vch->mana, (vch->max_mana + (vch->max_mana / 10)));
						break;
				case 2 :
						vch->move =   vch->move + dam ;
						vch->move = URANGE(-100, vch->move, (vch->max_move + (vch->max_move / 10)));
						break;
				default :
						bug( "oModhmv - min syntax error:  Obj: %d.", obj->pIndexData->vnum );
		}
		update_pos(vch);
	}

	return;
}

void do_oadd_affect( OBJ_DATA *obj, CHAR_DATA *ch, char *argument )
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
		bug( "oAdd_affect - No target name: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' || argument[0] == '\0' )
	{
		bug( "oAdd_affect - No spellname: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' || !is_number( arg3) )
	{
		bug( "oAdd_affect - Bad syntax at level: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg4 );
	if ( arg4[0] == '\0' || argument[0] == '\0' || !is_number( arg4) )
	{
		bug( "oAdd_affect - Bad syntax at time: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg5 );
	if ( arg5[0] == '\0' || argument[0] == '\0' || !is_number( arg5) )
	{
		bug( "oAdd_affect - Bad syntax at location: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg6 );
	if ( arg6[0] == '\0' || argument[0] == '\0' || !is_number( arg6) )
	{
		bug( "oAdd_affect - Bad syntax at mod: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	argument = one_argument( argument, arg7 );
	if ( arg7[0] == '\0' || !is_number( arg7) )
	{
		bug( "oAdd_affect - Bad syntax at bit: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	if ( !obj->carried_by || !obj->carried_by->in_room )
		return;
	target = room_find_char( obj->carried_by->in_room, arg1 );
	if ( !target )
	{
		bug( "oAdd_affect - target not in room: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	if ( ( sn = skill_lookup( arg2 ) ) == -1 )
	{
		bug( "oAdd_affect - strange spellname: vnum %d.", obj->pIndexData->vnum );
		return;
	}

	// if ( is_affected( target, sn ) )
	//	return;

	level = atoi ( arg3 );

	/*  修改為直接判定 level 是否在 0 以下 2023/01/25
		// 與 room command 相同 level 的部分為 0 則有法術不作用 2022/11/08
		aff_exist = level / 1000;
		if ( aff_exist == 0 && is_affected( target, sn ) )
			return;
	*/

	if ( level <= 0 && is_affected( target, sn ) )
		return;

	time  = atoi ( arg4 );
	loc   = atoi ( arg5 );
	mod	  = atoi ( arg6 );
	bit   = atoll ( arg7 );

	//bit = strtoll(arg7, &tmp, 10);

	//bug(arg7,0);

	af.type	 = sn;
	af.duration	 = time;
	af.location	 = loc;
	af.modifier	 = mod;
	af.bitvector = bit;
	affect_join( target, &af );
}


void do_odel_affect( OBJ_DATA *obj, CHAR_DATA *ch, char *argument )
{
    char arg1[ MAX_INPUT_LENGTH ];
    char arg2[ MAX_INPUT_LENGTH ];
    int		sn;
    AFFECT_DATA *paf;
    CHAR_DATA	*target;

    argument = one_argument( argument, arg1 );
    if ( arg1[0] == '\0' || argument[0] == '\0' )
    {
	bug( "oDel_affect - No target name: vnum %d.",
	    obj->pIndexData->vnum );
	return;
    }

    argument = one_argument( argument, arg2 );
    if ( arg2[0] == '\0' )
    {
	bug( "oDel_affect - No spellname: vnum %d.",
	    obj->pIndexData->vnum );
	return;
    }

    if ( !obj->carried_by || !obj->carried_by->in_room)
    {
    	bug( "obj no carried %d",obj->pIndexData->vnum);
    	return;
    }

    target = room_find_char( obj->carried_by->in_room, arg1 );
    if ( !target )
    {
	bug( "oDel_affect - target not in room: vnum %d.",
	    obj->pIndexData->vnum );
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
		bug( "oDel_affect - strange spellname: vnum %d.",
	    		obj->pIndexData->vnum );
		return;
    	}
    	affect_strip( target, sn );
    }
}


void do_opurge( OBJ_DATA *obj, CHAR_DATA *ch, char *argument )
{
    char       arg[ MAX_INPUT_LENGTH ];
    char       arg1[ MAX_INPUT_LENGTH ];
	char       arg2[ MAX_INPUT_LENGTH ];  // add at 2020/10/24
    CHAR_DATA *target = NULL, *target_next;
    OBJ_DATA  *pobj, *obj_next;
    ROOM_INDEX_DATA *room;

	argument = one_argument( argument, arg );

	if( !obj )
		return;
    
	if ( !str_cmp( "self", arg ) )
	{
	/* 'purge' */
		if ( !obj->carried_by )
			extract_obj( obj );
		else
		{
			obj_from_char(obj);
			extract_obj(obj);
		}
	}
    
	if ( !obj->carried_by || !obj->carried_by->in_room )
		return;
    
	room = obj->carried_by->in_room;
    
	if ( arg[0] == '\0' || !str_cmp( "all", arg ) )
	{
		/* 'purge' */
		if ( room->people )
			for ( target = room->people; target != NULL; target = target_next )
			{
				target_next = target->next_in_room;
				if ( !target->in_room || target->deleted )
					continue;
				if ( IS_NPC( target ) )
					extract_char( target, TRUE );
			}

		if ( room->contents )
			for ( pobj = room->contents; pobj != NULL; pobj = obj_next )
			{
				obj_next = pobj->next_content;
				if ( pobj->deleted )
					continue;
				extract_obj( pobj );
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
	else
	{
		argument = one_argument( argument, arg1 );
		// 針對特定目標身上持有的物品 vnum 作 purge 2020/03/05
		if ( !str_cmp( arg1, "vnum" ) )
		{
			argument = one_argument( argument, arg2 );
			int objvnum;
			if ( !str_cmp("$n",arg ) )
				target = obj->carried_by;
			else
				target = room_find_char( room, arg );
			if ( arg2[0] != '\0' )
			{
				if ( !is_number( arg2 ) )
				{
					bug( "Opurge - vnum must be number: obj %d.", obj->pIndexData->vnum );
					return;
				}
				if ( !target )
				{
					bug( "Opurge - vnum target not found: obj %d.", obj->pIndexData->vnum );
					return;
				}
				for( pobj = target->carrying; pobj; pobj = pobj->next_content )
				{
					if ( pobj->deleted )
						continue;
					objvnum = atoi( arg2 );
					if( pobj->pIndexData->vnum == objvnum )
					{
						obj_from_char( pobj );
						extract_obj( pobj );
					}
				}
			}
			else
			{
				bug( "Opurge - vnum arg2 not found: obj %d.", obj->pIndexData->vnum );
				return;
			}
			
		}
		if ( arg1[0] != '\0' )
		{
			if ( !str_cmp("$n",arg ) )
				target = obj->carried_by;
			else
				target = room_find_char( room, arg );
			if ( !target )
			{
				bug( "Opurge - target not found: obj %d.", obj->pIndexData->vnum );
				return;
			}
			for( pobj = target->carrying; pobj; pobj = pobj->next_content )
			{
				if ( pobj->deleted )
					continue;
				// 目標身上物品名為數字時, 阻擋 purge 到 clan eq 2020/10/24
				if ( is_number( arg1 ) && IS_SET(pobj->extra_flags, ITEM_CLAN))
					continue;
				if ( is_name( arg1, pobj->name ) )
				{
					obj_from_char( pobj );
					extract_obj( pobj );
				}
			}
		}
		else
		{
			target = room_find_char( room, arg );
			if ( target && IS_NPC(target) )
				extract_char( target, TRUE );
			else if ( room->contents )
				for ( pobj = room->contents; pobj; pobj = pobj->next_content )
				{
					if ( pobj->deleted )
						continue;
					if ( is_name( arg, pobj->name ) )
						extract_obj( pobj );
				}
		}
    }
}

void do_ooset(OBJ_DATA * pobj, CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj = NULL;
	//CHAR_DATA *pch;
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	char arg3 [ MAX_INPUT_LENGTH ];
	// char logline [ MAX_INPUT_LENGTH ];	//Mark by Razgriz	20050916
	char opr  [ MAX_INPUT_LENGTH ];
	int value = 0;
	long long value_long = 0; // 增加 long long value 值 2023/01/10

	// 移除 ooset 的 ch 2023/01/08
	//ch = pobj->carried_by;

	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, opr  );
	// 修改 arg3 為 strcpy 2023/01/08
	//one_argument( argument, arg3 );
	strcpy( arg3, argument );

	if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
	{
		return;
	}

	/*  修改判定, 並移除 ch 2023/01/08
	if ( str_cmp( arg1, "self" ) && !( obj = get_obj_here( pobj->carried_by, arg1 ) ) )
	{
		send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
		bug ( "Obj prog bug1 %d" , pobj->pIndexData->vnum );
		return;
	} */

	// 依續判定 self, 被攜帶, 房間地上 的情況 2023/01/08
	if ( !str_cmp(arg1, "self" ) )
		obj = pobj;
	else if ( pobj->carried_by )
		obj = get_obj_here( pobj->carried_by, arg1 );
	else if ( pobj->in_room )
		obj = get_room_obj( pobj->in_room , arg1 );

	// 如果都沒抓到則 return 跑 bug log 2023/01/08
	if( !obj ){
		bug ( "Obj prog bug1 %d" , pobj->pIndexData->vnum );
		return;
	}

	/*
	* Snarf the value (which need not be numeric).
	*/

	value = atoi( arg3 );


	/*
	* Set something numeric value.
	*/

	if ( !str_cmp( arg2, "extra" ) )
	{
		// 針對修改數值會超過 int 的增加以 long long 來判定數值 2023/01/10
		//obj->extra_flags = qmark_opr( obj->extra_flags, opr, value );
		//return;
		value_long = atoll( arg3 );
		obj->extra_flags = ll_qmark_opr( obj->extra_flags, opr, value_long, "ooset_extra" );
		return;
	}


	if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
	{
		obj->value[0] = qmark_opr( obj->value[0] , opr ,value, "ooset_v0" );
		return;
	}

	if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
	{
		obj->value[1] = qmark_opr( obj->value[1], opr, value, "ooset_v1" );
		return;
	}

	if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
	{
		obj->value[2] = qmark_opr( obj->value[2], opr, value, "ooset_v2" );
		return;
	}

	if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
	{
		obj->value[3] = qmark_opr( obj->value[3], opr, value, "ooset_v3" );
		return;
	}

	if ( !str_cmp( arg2, "wear" ) )
	{
		obj->wear_flags = qmark_opr( obj->wear_flags, opr, value, "ooset_wear" );
		return;
	}

	if ( !str_cmp( arg2, "level" ) )
	{
		obj->level = qmark_opr( obj->level, opr, value, "ooset_level" );
		return;
	}

	if ( !str_cmp( arg2, "weight" ) )
	{
		if ( obj->carried_by != NULL && !IS_NPC( obj->carried_by ) )
		{
			return;
		}
		obj->weight = qmark_opr( obj->weight, opr, value, "ooset_weight" );
		return;
	}

	if ( !str_cmp( arg2, "cost" ) )
	{
		obj->cost = qmark_opr( obj->cost, opr, value, "ooset_cost" );
		return;
	}

	if ( !str_cmp( arg2, "timer" ) )
	{
		obj->timer = qmark_opr( obj->timer, opr, value, "ooset_timer" );
		return;
	}

	/* 
	 * set obj_extra_value  2022/11/29
	 */
	if ( !str_cmp( arg2, "exused" ) )
	{
		if ( value > 0 )
			( obj->extra_value ).used = TRUE;
		else
			( obj->extra_value ).used = FALSE;
		return;
	}

	if ( !str_cmp( arg2, "exvalue0" ) || !str_cmp( arg2, "ev0" ) )
	{
		( obj->extra_value ).used = TRUE;
		( obj->extra_value ).v[0] = value;
		return;
	}

	if ( !str_cmp( arg2, "exvalue1" ) || !str_cmp( arg2, "ev1" ) )
	{
		( obj->extra_value ).used = TRUE;
		( obj->extra_value ).v[1] = value;
		return;
	}

	if ( !str_cmp( arg2, "extimer0" ) || !str_cmp( arg2, "et0" ) )
	{
		( obj->extra_value ).used = TRUE;
		( obj->extra_value ).t[0] = value;
		return;
	}

	if ( !str_cmp( arg2, "extimer1" ) || !str_cmp( arg2, "et1" ) )
	{
		( obj->extra_value ).used = TRUE;
		( obj->extra_value ).t[1] = value;
		return;
	}

	/*
	*  設定字串 2022/11/29
	*/

	if ( !str_cmp( arg2, "name" ) )
	{
		//if ( longstring( ch, arg3 ) )
		//	return;

		free_string( obj->name );
		obj->name = str_dup( arg3 );
		return;
	}

	if ( !str_cmp( arg2, "short" ) )
	{
		//if ( longstring( ch, arg3 ) )
		//	return;

		free_string( obj->short_descr );
		obj->short_descr = str_dup( arg3 );
		return;
	}

	if ( !str_cmp( arg2, "long" ) )
	{
		//if ( longstring( ch, arg3 ) )
		//	return;

		free_string( obj->description );
		obj->description = str_dup( arg3 );
		return;
	}


	/*
	* Generate usage message.
	*/
	//do_oset( ch, "" );
	return;
}

/* Added by Razgriz 20050916
語法:
  opset_obj_affect victim object duration location modifier

  victim   (字串): self,  $o	//obj 本身, 不必再設 object
                   owner, $n	//obj->carried_by
                   char_id	//跟 obj 同房間的 char id
                   room		//房間中的 obj
  object   (數字): object vnum
           (字串): object name
  duration (數字): 時間, -1 則為永久存在
  location (數字): 參考 parameter.h - Apply types (for affects)
  modifier (數字): 配合 location 填入適當數值
*/
void do_opset_obj_affect ( OBJ_DATA *pobj, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim = NULL;
	AFFECT_DATA *paf = NULL;
	OBJ_DATA *obj = NULL;
	ROOM_INDEX_DATA *room = NULL;
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	char arg3 [ MAX_INPUT_LENGTH ];
	char arg4 [ MAX_INPUT_LENGTH ];
	char arg5 [ MAX_INPUT_LENGTH ];
	int vnum = -999;
	int time;
	int loc;
	int mod;
	bool fSelf = FALSE;
	bool fFind = FALSE;
	// 新增如果物品被裝備時, 會自己重新裝備, 並且不會被目標發現 2022/11/27
	CHAR_DATA *wear_ch = NULL;
	int ch_iwear = 0;

	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opset_obj_affect - No victim name: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//if victim = self, we do not need to read object
	if ( str_cmp ( arg1 , "self" ) && str_cmp ( arg1 , "$o" ) )
	{
		argument = one_argument ( argument, arg2 );
		if ( arg2[0] == '\0' || argument[0] == '\0' )
		{
			bug( "opset_obj_affect - No object name: vnum %d.", pobj->pIndexData->vnum );
			return;
		}
	}
	else
	{
		fSelf = TRUE;
		arg2[0] = '\0';
		obj = pobj;
	}

	argument = one_argument ( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' || !is_number ( arg3 ) )
	{
		bug( "opset_obj_affect - Bad syntax at duration: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg4 );
	if ( arg4[0] == '\0' || argument[0] == '\0' || !is_number ( arg4 ) )
	{
		bug( "opset_obj_affect - Bad syntax at location: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg5 );
	if ( arg5[0] == '\0' || !is_number ( arg5 ) )
	{
		bug( "opset_obj_affect - Bad syntax at modifier: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//we need a room
	if ( pobj->carried_by && pobj->carried_by->in_room )
		room = pobj->carried_by->in_room;
	else if ( pobj->in_room )
		room = pobj->in_room;
	else
	{
		bug( "opset_obj_affect - no room can be find: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//transform args to local variable
	if ( is_number ( arg2 ) )
		vnum = atoi ( arg2 );
	time = atoi ( arg3 );
	loc  = atoi ( arg4 );
	mod  = atoi ( arg5 );

	//find obj from different victim
	if ( fSelf )
	{
		if ( obj->carried_by && obj->wear_loc != WEAR_NONE ){
			// 記錄裝備者及裝備位置 2022/11/27
			wear_ch = obj->carried_by;
			ch_iwear = obj->wear_loc;
			// 修改為以無聲的方式脫掉裝備 2022/11/27
			//remove_obj ( obj->carried_by, obj->wear_loc, TRUE );
			unequip_char2( wear_ch , obj );
		}
		fFind = TRUE;
	}
	else if ( !str_cmp ( arg1, "room" ) )
	{
		//find obj in room ,obj can't be in container
		for ( obj = room->contents; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}
	else
	{
		//find victim
		victim = ( !str_cmp ( arg1, "owner" ) || !str_cmp ( arg1, "$n" ) ) ? pobj->carried_by : room_find_char ( room , arg1 );

		if ( !victim )
		{
			bug( "opset_obj_affect - no victim can be find: vnum %d.", pobj->pIndexData->vnum );
			return;
		}

		//find obj carried by victim , obj can't be in container
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) )
				continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum )
				continue;
			if ( obj->wear_loc != WEAR_NONE )
			{
				// 記錄裝備者及裝備位置 2022/11/27
				wear_ch = victim;
				ch_iwear = obj->wear_loc;
				// 修改為以無聲的方式脫掉裝備 2022/11/27
				//remove_obj ( victim, obj->wear_loc, TRUE );
				unequip_char2( wear_ch , obj );
			}
			fFind = TRUE;
			break;
		}
	}

	//set affect to object
	if ( fFind )
	{
		paf		= new_affect ();
		paf->type	= obj->pIndexData->vnum;
		paf->duration	= UMAX ( -1, time );
		paf->location	= loc;
		paf->modifier	= mod;
		paf->bitvector	= 0;
		paf->next	= obj->affected;
		obj->affected	= paf;

		// 重新裝備物品 2022/11/27
		if( wear_ch && obj ){
			equip_char2( wear_ch , obj , ch_iwear );
		}

		return;
	}

	bug( "opset_obj_affect - no item can be find: vnum %d.", pobj->pIndexData->vnum );
	return;
}

/* Added by Razgriz 20050916
語法:
  opremove_obj_affect victim object location

  victim   (字串): self,  $o	//obj 本身, 不必再設 object
                   owner, $n	//obj->carried_by
                   char_id	//跟 obj 同房間的 char id
                   room		//房間中的 obj
  object   (數字): object vnum
           (字串): object name
  location (數字): 參考 parameter.h - Apply types (for affects)
           (字串): all, 表示移除該 obj 所有 affect
*/
void do_opremove_obj_affect ( OBJ_DATA *pobj, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim = NULL;
	AFFECT_DATA *paf = NULL;
	AFFECT_DATA *pafPre	= NULL;
	AFFECT_DATA *pafNext = NULL;
	OBJ_DATA *obj = NULL;
	ROOM_INDEX_DATA *room = NULL;
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	char arg3[ MAX_INPUT_LENGTH ];
	int vnum= -999;
	int loc	= -999;
	bool fSelf	= FALSE;
	bool fAll	= FALSE;
	bool fFind	= FALSE;
	// 新增如果物品被裝備時, 會自己重新裝備, 並且不會被目標發現 2022/11/27
	CHAR_DATA *wear_ch = NULL;
	int ch_iwear = 0;


	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opremove_obj_affect - No victim name: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//if victim = self, we do not need to read object
	if ( str_cmp ( arg1 , "self" ) && str_cmp ( arg1 , "$o" ) )
	{
		argument = one_argument ( argument, arg2 );
		if ( arg2[0] == '\0' || argument[0] == '\0' )
		{
			bug( "opremove_obj_affect - No object name: vnum %d.", pobj->pIndexData->vnum );
			return;
		}
	}
	else
	{
		fSelf = TRUE;
		arg2[0] = '\0';
		obj = pobj;
	}

	argument = one_argument ( argument, arg3 );
	fAll = !str_cmp ( arg3, "all" );
	if ( arg3[0] == '\0' || ( !is_number ( arg3 ) && !fAll ) )
	{
		bug( "opremove_obj_affect - Bad syntax at location: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//we need a room
	if ( pobj->carried_by && pobj->carried_by->in_room )
		room = pobj->carried_by->in_room;
	else if ( pobj->in_room )
		room = pobj->in_room;
	else
	{
		bug( "opremove_obj_affect - no room can be find: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//transform args to local variable
	if ( is_number ( arg2 ) ) vnum = atoi ( arg2 );
	if ( is_number ( arg3 ) ) loc  = atoi ( arg3 );

	//find obj from different victim
	if ( fSelf )
	{
		if ( obj->carried_by && obj->wear_loc != WEAR_NONE )
		{
			// 記錄裝備者及裝備位置 2022/11/27
			wear_ch = obj->carried_by;
			ch_iwear = obj->wear_loc;
			// 修改為以無聲的方式脫掉裝備 2022/11/27
			//remove_obj ( obj->carried_by, obj->wear_loc, TRUE );
			unequip_char2( wear_ch , obj );
		}
		fFind = TRUE;
	}
	else if ( !str_cmp ( arg1, "room" ) )
	{
		//find obj in room ,obj can't be in container
		for ( obj = room->contents; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}
	else
	{
		//find victim
		victim = ( !str_cmp ( arg1, "owner" ) || !str_cmp ( arg1, "$n" ) ) ? pobj->carried_by : room_find_char ( room , arg1 );

		if ( !victim )
		{
			bug( "opremove_obj_affect - no victim can be find: vnum %d.", pobj->pIndexData->vnum );
			return;
		}

		//find obj carried by victim , obj can't be in container
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) )
				continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum )
				continue;
			if ( obj->wear_loc != WEAR_NONE )
			{
				// 記錄裝備者及裝備位置 2022/11/27
				wear_ch = victim;
				ch_iwear = obj->wear_loc;
				// 修改為以無聲的方式脫掉裝備 2022/11/27
				//remove_obj ( victim, obj->wear_loc, TRUE );
				unequip_char2( wear_ch , obj );
			}
			fFind = TRUE;
			break;
		}
	}

	//remove affect from object
	if ( fFind )
	{
		if ( fAll )
		{
			for ( paf = obj->affected; paf; paf = pafNext )
			{
				pafNext = paf->next;
				free_affect ( paf );
			}
			obj->affected = NULL;
			//return;
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
			//return;
		}
		// 重新裝備物品 2022/11/27
		if( wear_ch && obj ){
			equip_char2( wear_ch , obj , ch_iwear );
		}
		return;
	}

	bug( "opremove_obj_affect - no item can be find: vnum %d.", pobj->pIndexData->vnum );
	return;
}

/* Added by Razgriz 20050916
語法:
  opmodify_obj_affect victim object location opr modifier [duration]

  victim   (字串): self,  $o	//obj 本身, 不必再設 object
                   owner, $n	//obj->carried_by
                   char_id	//跟 obj 同房間的 char id
                   room		//房間中的 obj
  object   (數字): object vnum
           (字串): object name
  location (數字): 參考 parameter.h - Apply types (for affects)
  opr      (符號): + - & | r =
  modifier (數字): 配合 location 填入適當數值
  duration (數字): 有值的話, 當找不到 affect 時, 會自動呼叫 set 並把 duration 設成此值
                   沒值的話, 當找不到 affect 時, 什麼都不做
*/
void do_opmodify_obj_affect ( OBJ_DATA *pobj, CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim = NULL;
	AFFECT_DATA *paf = NULL;
	OBJ_DATA *obj = NULL;
	ROOM_INDEX_DATA *room = NULL;
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	char arg3[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	char arg5[ MAX_INPUT_LENGTH ];
	char arg6[ MAX_INPUT_LENGTH ];
	char buf[ MAX_INPUT_LENGTH ];
	char name[ MAX_INPUT_LENGTH ];
	char *blank = " ";
	int vnum = -999;
	int loc;
	int mod;
	bool fSelf = FALSE;
	bool fFind = FALSE;
	// 新增如果物品被裝備時, 會自己重新裝備, 並且不會被目標發現 2022/11/27
	CHAR_DATA *wear_ch = NULL;
	int ch_iwear = 0;


	smash_tilde ( argument );

	//read arguments
	argument = one_argument ( argument, arg1 );
	if ( arg1[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opmodify_obj_affect - No victim name: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//if victim = self, we do not need to read object
	if ( str_cmp ( arg1 , "self" ) && str_cmp ( arg1 , "$o" ) )
	{
		argument = one_argument ( argument, arg2 );
		if ( arg2[0] == '\0' || argument[0] == '\0' )
		{
			bug( "opmodify_obj_affect - No object name: vnum %d.", pobj->pIndexData->vnum );
			return;
		}
	}
	else
	{
		fSelf = TRUE;
		arg2[0] = '\0';
		obj = pobj;
	}

	argument = one_argument ( argument, arg3 );
	if ( arg3[0] == '\0' || argument[0] == '\0' || !is_number ( arg3 ) )
	{
		bug( "opmodify_obj_affect - Bad syntax at location: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, opr );
	if ( opr[0] == '\0' || argument[0] == '\0' )
	{
		bug( "opmodify_obj_affect - Bad syntax at operator: vnum %d.", pobj->pIndexData->vnum );
		return;
	}
	// 阻擋不適用的 opr 2022/11/30
	if ( str_cmp( opr, "+" ) && str_cmp( opr, "-" ) && str_cmp( opr, "&" ) && str_cmp( opr, "|" ) && str_cmp( opr, "r" ) && str_cmp( opr, "=" ) )
	{
		bug( "opmodify_obj_affect - illegal operator: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	argument = one_argument ( argument, arg5 );
	if ( arg5[0] == '\0' || !is_number ( arg5 ) )
	{
		bug( "opmodify_obj_affect - Bad syntax at modifier: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//we need a room
	if ( pobj->carried_by && pobj->carried_by->in_room )
		room = pobj->carried_by->in_room;
	else if ( pobj->in_room )
		room = pobj->in_room;
	else
	{
		bug( "opmodify_obj_affect - no room can be find: vnum %d.", pobj->pIndexData->vnum );
		return;
	}

	//transform args to local variable
	if ( !fSelf ){ // 阻檔 self 或 $o 的情況下跑到 arg2  2022/11/30
		if ( is_number ( arg2 ) )
			vnum = atoi ( arg2 );
	}
	loc  = atoi ( arg3 );
	mod  = atoi ( arg5 );

	//find obj from different victim
	if ( fSelf )
	{
		if ( obj->carried_by && obj->wear_loc != WEAR_NONE )
		{
			// 記錄裝備者及裝備位置 2022/11/27
			wear_ch = obj->carried_by;
			ch_iwear = obj->wear_loc;
			// 修改為以無聲的方式脫掉裝備 2022/11/27
			//remove_obj ( obj->carried_by, obj->wear_loc, TRUE );
			unequip_char2( wear_ch , obj );
		}
		fFind = TRUE;
	}
	else if ( !str_cmp ( arg1, "room" ) )
	{
		//find obj in room ,obj can't be in container
		for ( obj = room->contents; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) ) continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum ) continue;
			fFind = TRUE;
			break;
		}
	}
	else
	{
		//find victim
		victim = ( !str_cmp ( arg1, "owner" ) || !str_cmp ( arg1, "$n" ) ) ? pobj->carried_by : room_find_char ( room , arg1 );

		if ( !victim )
		{
			bug( "opmodify_obj_affect - no victim can be find: vnum %d.", pobj->pIndexData->vnum );
			return;
		}

		//find obj carried by victim , obj can't be in container
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( vnum == -999 && !is_name ( arg2, obj->name ) )
				continue;
			if ( vnum != -999 && obj->pIndexData->vnum != vnum )
				continue;
			if ( obj->wear_loc != WEAR_NONE )
			{
				// 記錄裝備者及裝備位置 2022/11/27
				wear_ch = victim;
				ch_iwear = obj->wear_loc;
				// 修改為以無聲的方式脫掉裝備 2022/11/27
				//remove_obj ( victim, obj->wear_loc, TRUE );
				unequip_char2( wear_ch , obj );
			}
			fFind = TRUE;
			break;
		}
	}

	//modify affect to object
	if ( fFind )
	{
		//find affect depend on location
		for ( paf = obj->affected; paf; paf = paf->next )
		{
			if ( paf->location != loc )
				continue;
			paf->modifier = qmark_opr ( paf->modifier , opr , mod, "opmodify" );
			// 重新裝備物品 2022/11/27
			if( wear_ch && obj ){
				equip_char2( wear_ch , obj , ch_iwear );
			}
			return;
		}

		// 沒有找到能 modify 的值會進行重新裝備的行為 2022/11/27
		// 就算之後跑入 opset_obj_affect 也會再次依此裝備重新跑一次
		if( wear_ch && obj ){
			equip_char2( wear_ch , obj , ch_iwear );
		}

		//no find affect, if duration have value call opset_obj_affect
		argument = one_argument ( argument, arg6 );
		if ( arg6[0] == '\0' || !is_number ( arg6 ) )
		{
			bug( "opmodify_obj_affect - no affect can be find: vnum %d.", pobj->pIndexData->vnum );
			return;
		}
		else
		{
			buf[0] = '\0';
			strcat ( buf, arg1 ); strcat ( buf, blank );
			if( !fSelf ){ // 阻檔 self 或 $o 的情況下跑到 arg2  2022/11/30
				strcat ( buf, arg2 );
				strcat ( buf, blank );
			}
			strcat ( buf, arg6 ); strcat ( buf, blank );
			strcat ( buf, arg3 ); strcat ( buf, blank );
			strcat ( buf, arg5 );
			do_opset_obj_affect ( pobj, ch, buf );
		}
		return;
	}

	bug( "opmodify_obj_affect - no item can be find: vnum %d.", pobj->pIndexData->vnum );
	return;
}
