/***************************************************************************
 *  File: olc_act.c							   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 *									   *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.		   *
 *									   *
 ***************************************************************************/



#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>	/* OLC 1.1b */
#include "merc.h"
#include "olc.h"

#define MPEDIT( fun )		bool fun( CHAR_DATA *ch, char *argument )

struct olc_help_type
{
    char *command;
    const void *structure;
    char *desc;
};

bool aedit_mlist( CHAR_DATA *ch, char * arg)
{
    AREA_DATA *pArea;
    MOB_INDEX_DATA *pMob;
    char buf [MAX_STRING_LENGTH];
    int vnum;

    EDIT_AREA(ch, pArea);

    send_to_char("這個區域內的生物共有：\n\r", ch);

    for( vnum = pArea->lvnum ; vnum <= pArea->uvnum; ++vnum ) {
	if( ( pMob = get_mob_index( vnum ) )) {
	    sprintf(buf,"[%5d] %s [%s]\n\r", pMob->vnum,pMob->short_descr, pMob->player_name);
	    send_to_char(buf, ch);
	}
    }
    return FALSE;

}

bool aedit_olist( CHAR_DATA *ch, char * arg)
{
    AREA_DATA *pArea;
    OBJ_INDEX_DATA *pObj;
    char buf [MAX_STRING_LENGTH];
    int vnum;

    EDIT_AREA(ch, pArea);

    send_to_char("這個區域內的物品共有：\n\r", ch);

    for( vnum = pArea->lvnum ; vnum <= pArea->uvnum; ++vnum ) {
	if( ( pObj = get_obj_index( vnum ) )) {
	    sprintf(buf,"[%5d] %s [%s]\n\r", pObj->vnum,pObj->short_descr, pObj->name);
	    send_to_char(buf, ch);
	}
    }
    return FALSE;

}

bool aedit_rlist( CHAR_DATA *ch, char * arg)
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    char buf [MAX_STRING_LENGTH];
    int vnum;

    EDIT_AREA(ch, pArea);

    send_to_char("這個區域內的房間共有：\n\r", ch);

    for( vnum = pArea->lvnum ; vnum <= pArea->uvnum; ++vnum ) {
	if( ( pRoom = get_room_index( vnum ) )) {
	    sprintf(buf,"[%5d] %s\n\r", pRoom->vnum, pRoom->name);
	    send_to_char(buf, ch);
	}
    }
    return FALSE;

}

bool show_version( CHAR_DATA *ch, char *argument )
{
    send_to_char( VERSION, ch );
    send_to_char( "\n\r", ch );
    send_to_char( AUTHOR, ch );
    send_to_char( "\n\r", ch );
    send_to_char( DATE, ch );
    send_to_char( "\n\r", ch );
    send_to_char( CREDITS, ch );
    send_to_char( "\n\r", ch );

    return FALSE;
}    

/*
 * This table contains help commands and a brief description of each.
 * ------------------------------------------------------------------
 */
const struct olc_help_type help_table[] =
{
    {	"area",		area_flags,	 "Area attributes."		},
    {	"room",		room_flags,	 "Room attributes."		},
    {	"sector",	sector_flags,	 "Sector types, terrain."	},
    {	"exit",		exit_flags,	 "Exit types."			},
    {	"type",		type_flags,	 "Types of objects."		},
    {	"extra",	extra_flags,	 "Object attributes."		},
    {	"wear",		wear_flags,	 "Where to wear object."	},
    {	"spec",		spec_table,	 "Available special programs."	},
    {	"class",	class_table, "Available mob class."	},
    {	"sex",		sex_flags,	 "Sexes."			},
    {	"race",		race_flags,	 "Mobile races."		},
    {	"act",		act_flags,	 "Mobile attributes."		},
    {	"affect",	affect_flags,	 "Mobile affects."		},
    {	"wear-loc",	wear_loc_flags,	 "Where mobile wears object."	},
    {	"spells",	skill_table,	 "Names of current spells."	},
    {	"weapon",	weapon_flags,	 "Type of weapon."		},
    {	"container",	container_flags, "Container status."		},
    {	"liquid",	liquid_flags,	 "Types of liquids."		},
    {	"mobprogs",	mprog_type_flags, "Types of Mob Programs."	},
    {	"",		0,		 ""				}
};



/*****************************************************************************
 Name:		show_flag_cmds
 Purpose:	Displays settable flags and stats.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_flag_cmds( CHAR_DATA *ch, const struct flag_type *flag_table )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int	 flag;
    int	 col;
 
    buf1[0] = '\0';
    col = 0;
    for (flag = 0; *flag_table[flag].name; flag++)
    {
	if ( flag_table[flag].settable )
	{
	    sprintf( buf, "%-19.18s", flag_table[flag].name );
	    strcat( buf1, buf );
	    if ( ++col % 4 == 0 )
		strcat( buf1, "\n\r" );
	}
    }
 
    if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}



/*****************************************************************************
 Name:		show_skill_cmds
 Purpose:	Displays all skill functions.
		Does remove those damn immortal commands from the list.
		Could be improved by:
		(1) Adding a check for a particular class.
		(2) Adding a check for a level range.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_skill_cmds( CHAR_DATA *ch, int tar )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH*2 ];
    int	 sn;
    int	 col;
 
    buf1[0] = '\0';
    col = 0;
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
	if ( !skill_table[sn].name )
	    break;

	if ( !str_cmp( skill_table[sn].name, "reserved" )
	  || skill_table[sn].spell_fun == spell_null )
	    continue;

	if ( tar == -1 || skill_table[sn].target == tar )
	{
	    sprintf( buf, "%-19.18s", skill_table[sn].name );
	    strcat( buf1, buf );
	    if ( ++col % 4 == 0 )
		strcat( buf1, "\n\r" );
	}
    }
 
    if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}



/*****************************************************************************
 Name:		show_spec_cmds
 Purpose:	Displays settable special functions.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_spec_cmds( CHAR_DATA *ch )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int	 spec;
    int	 col;
 
    buf1[0] = '\0';
    col = 0;
    send_to_char( "請用 'spec spec_xxxxx' 來設定特殊能力。\n\r\n\r", ch );
    for (spec = 0; *spec_cost[spec].name; spec++)
    {
	sprintf( buf, "%-17.16s%4d    ", &spec_cost[spec].name[5], spec_cost[spec].cclass );
	strcat( buf1, buf );
	if ( ++col % 3 == 0 )
	    strcat( buf1, "\n\r" );
    }
 
    if ( col % 3 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}


/*****************************************************************************
 Name:		show_class_cmds
 Purpose:	Displays settable mob class.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_class_cmds( CHAR_DATA *ch )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int	 class;
    int	 col;
 
    buf1[0] = '\0';
    col = 0;
    send_to_char( "目前可以設定的職業有：\n\r\n\r", ch );
    for (class = 0; *class_cost[class].name; class++)
    {
	sprintf( buf, "%-11.10s %d   ", class_cost[class].name, class_cost[class].cclass );
	strcat( buf1, buf );
	if ( ++col % 4 == 0 )
	    strcat( buf1, "\n\r" );
    }
 
    if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}


/*****************************************************************************
 Name:		show_help
 Purpose:	Displays help for many tables used in OLC.
 Called by:	olc interpreters.
 ****************************************************************************/
bool show_help( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char spell[MAX_INPUT_LENGTH];
    int cnt;

    argument = one_argument( argument, arg );
    one_argument( argument, spell );

    /*
     * Display syntax.
     */
    if ( arg[0] == '\0' )
    {
	send_to_char( "用法：	? [command]\n\r\n\r", ch );
	send_to_char( "[command]  [description]\n\r", ch );
	for (cnt = 0; help_table[cnt].command[0] != '\0'; cnt++)
	{
	    sprintf( buf, "%-10.10s -%s\n\r",
		capitalize( help_table[cnt].command ),
		help_table[cnt].desc );
	    send_to_char( buf, ch );
	}
	return FALSE;
    }

    /*
     * Find the command, show changeable data.
     * ---------------------------------------
     */
    for (cnt = 0; *help_table[cnt].command; cnt++)
    {
	if (  arg[0] == help_table[cnt].command[0]
	  && !str_prefix( arg, help_table[cnt].command ) )
	{
	    if ( help_table[cnt].structure == spec_table )
	    {
		show_spec_cmds( ch );
		return FALSE;
	    }
	    else if ( help_table[cnt].structure == class_table )
	    {
		show_class_cmds( ch );
		return FALSE;
	    }
	    else
	    if ( help_table[cnt].structure == skill_table )
	    {

		if ( spell[0] == '\0' )
		{
		    send_to_char( "用法：  ? spells "
			"[ignore/attack/defend/self/object/all]\n\r", ch );
		    return FALSE;
		}

		if ( !str_prefix( spell, "all" ) )
		    show_skill_cmds( ch, -1 );
		else if ( !str_prefix( spell, "ignore" ) )
		    show_skill_cmds( ch, TAR_IGNORE );
		else if ( !str_prefix( spell, "attack" ) )
		    show_skill_cmds( ch, TAR_CHAR_OFFENSIVE );
		else if ( !str_prefix( spell, "defend" ) )
		    show_skill_cmds( ch, TAR_CHAR_DEFENSIVE );
		else if ( !str_prefix( spell, "self" ) )
		    show_skill_cmds( ch, TAR_CHAR_SELF );
		else if ( !str_prefix( spell, "object" ) )
		    show_skill_cmds( ch, TAR_OBJ_INV );
		else
		    send_to_char( "用法：  ? spell "
			"[ignore/attack/defend/self/object/all]\n\r", ch );
		    
		return FALSE;
	    }
	    else
	    {
		show_flag_cmds( ch, help_table[cnt].structure );
		return FALSE;
	    }
	}
    }

    show_help( ch, "" );
    return FALSE;
}

bool check_facility ( AREA_DATA* area, int flag )
{
	int	room;
	ROOM_INDEX_DATA *proom;
	
	for( room = area->lvnum; room <= area->uvnum; ++room)
		if( ( proom = get_room_index(room) ) )
		{
			if ( IS_SET( proom->room_flags, flag ) )
				return TRUE;
		}
	return FALSE;
}

bool redit_recall( CHAR_DATA *ch, char *argument )
{

    ROOM_INDEX_DATA	*pRoom;
    EDIT_ROOM(ch, pRoom);
    if( !ch->clan ) {
	send_to_char("你沒有加入任何幫會。\n\r", ch);
	return FALSE;
    }
    if( !check_cost(ch, COST_SET_CRECALL)){
	send_to_char("你的幫費不夠了。\n\r", ch);
	return FALSE;
    }
    ch->clan->crecall = pRoom->vnum;
    clan_update();
    send_to_char("幫會 Recall 點設定成功\。\n\r", ch);
    return TRUE;
}

bool redit_mlist( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA	*pMobIndex;
    AREA_DATA		*pArea;
    char		buf  [ MAX_STRING_LENGTH   ];
    char		buf1 [ MAX_STRING_LENGTH*2 ];
    char		arg  [ MAX_INPUT_LENGTH	   ];
    bool fAll, found;
    int vnum;
    //int	 col = 0;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "用法：	mlist <all/name>\n\r", ch );
	return FALSE;
    }

    pArea = ch->in_room->area;
    buf1[0] = '\0';
    fAll    = !str_cmp( arg, "all" );
    found   = FALSE;

    for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) )
	{
	    if ( fAll || is_name( arg, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s(%s)\n",
		    pMobIndex->vnum, capitalize( pMobIndex->short_descr ),
		    pMobIndex->player_name );
		strcat( buf1, buf );
		//if ( ++col % 3 == 0 )
		//    strcat( buf1, "\n\r" );
	    }
	}
    }

    if ( !found )
    {
	send_to_char( "Mobile(s) not found in this area.\n\r", ch);
	return FALSE;
    }

    //if ( col % 3 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return FALSE;
}



bool redit_olist( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA	*pObjIndex;
    AREA_DATA		*pArea;
    char		buf  [ MAX_STRING_LENGTH   ];
    char		buf1 [ MAX_STRING_LENGTH*2 ];
    char		arg  [ MAX_INPUT_LENGTH	   ];
    bool fAll, found;
    int vnum;
    //int	 col = 0;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "用法：	olist <all/name/item_type>\n\r", ch );
	return FALSE;
    }

    pArea = ch->in_room->area;
    buf1[0] = '\0';
    fAll    = !str_cmp( arg, "all" );
    found   = FALSE;

    for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) )
	{
	    if ( fAll || is_name( arg, pObjIndex->name )
	    || flag_value( type_flags, arg ) == pObjIndex->item_type )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s(%s)\n",
		    pObjIndex->vnum, capitalize( pObjIndex->short_descr ),
		    pObjIndex->name );
		strcat( buf1, buf );
		//if ( ++col % 3 == 0 )
		//    strcat( buf1, "\n\r" );
	    }
	}
    }

    if ( !found )
    {
	send_to_char( "Object(s) not found in this area.\n\r", ch);
	return FALSE;
    }

    //if ( col % 3 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return FALSE;
}



bool redit_mshow( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    int value;

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	mshow <vnum>\n\r", ch );
	return FALSE;
    }

    if ( is_number( argument ) )
    {
	value = atoi( argument );
	if ( !( pMob = get_mob_index( value ) ))
	{
	    send_to_char( "REdit:  That mobile does not exist.\n\r", ch );
	    return FALSE;
	}

	ch->desc->pEdit = (void *)pMob;
    }
 
    medit_show( ch, argument );
    ch->desc->pEdit = (void *)ch->in_room;
    return FALSE; 
}



bool redit_oshow( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int value;

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	oshow <vnum>\n\r", ch );
	return FALSE;
    }

    if ( is_number( argument ) )
    {
	value = atoi( argument );
	if ( !( pObj = get_obj_index( value ) ))
	{
	    send_to_char( "REdit:  That object does not exist.\n\r", ch );
	    return FALSE;
	}

	ch->desc->pEdit = (void *)pObj;
    }
 
    oedit_show( ch, argument );
    ch->desc->pEdit = (void *)ch->in_room;
    return FALSE; 
}



/*****************************************************************************
 Name:		check_range( lower vnum, upper vnum )
 Purpose:	Ensures the range spans only one area.
 Called by:	aedit_vnum(olc_act.c).
 ****************************************************************************/
bool check_range( int lower, int upper )
{
    AREA_DATA *pArea;
    int cnt = 0;

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
	/*
	 * lower < area < upper
	 */
	if ( ( lower <= pArea->lvnum && upper >= pArea->lvnum )
	||   ( upper >= pArea->uvnum && lower <= pArea->uvnum ) )
	    cnt++;

	if ( cnt > 1 )
	    return FALSE;
    }
    return TRUE;
}



AREA_DATA *get_vnum_area( int vnum )
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
	if ( vnum >= pArea->lvnum
	  && vnum <= pArea->uvnum )
	    return pArea;
    }

    return 0;
}



/*
 * Area Editor Functions.
 */
bool aedit_show( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char buf  [MAX_STRING_LENGTH];

    EDIT_AREA(ch, pArea);

    sprintf( buf, "Name:     [%5d] %s\n\r", pArea->vnum, pArea->name );
    send_to_char( buf, ch );

    sprintf( buf, "Identity:     %s\n\r",  pArea->identity );
    send_to_char( buf, ch );
    sprintf( buf, "Recall:   [%5d] %s\n\r", pArea->recall,
	get_room_index( pArea->recall )
	? get_room_index( pArea->recall )->name : "none" );
    send_to_char( buf, ch );
    sprintf( buf, "Death:   [%5d] %s\n\r", pArea->death,
	get_room_index( pArea->death)
	? get_room_index( pArea->death )->name : "none" );
    send_to_char( buf, ch );

    sprintf( buf, "File:     %s\n\r", pArea->filename );
    send_to_char( buf, ch );

    sprintf( buf, "Vnums:    [%d-%d]\n\r", pArea->lvnum, pArea->uvnum );
    send_to_char( buf, ch );

    sprintf( buf, "Age:	     [%d]\n\r",	pArea->age );
    send_to_char( buf, ch );

    sprintf( buf, "Players:  [%d]\n\r", pArea->nplayer );
    send_to_char( buf, ch );

    sprintf( buf, "Security: [%d]\n\r", pArea->security );
    send_to_char( buf, ch );

    sprintf( buf, "Builders: [%s]\n\r", pArea->builders );
    send_to_char( buf, ch );

    sprintf( buf, "Flags:    [%s]\n\r", flag_string( area_flags, pArea->area_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "F_toggle:    [%s]\n\r", flag_string( room_flags, pArea->f_toggle) );
    send_to_char( buf, ch );
    return FALSE;
}



bool aedit_reset( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    EDIT_AREA(ch, pArea);

    reset_area( pArea );
    send_to_char( "Area reset.\n\r", ch );
    bugf("%s reset %s", ch->name, pArea->name);

    return FALSE;
}



bool aedit_create( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    if ( top_area >= INT_MAX )	/* OLC 1.1b */
    {
	send_to_char( "We're out of vnums for new areas.\n\r", ch );
	return FALSE;
    }

    pArea		=   new_area();
    area_last->next	=   pArea;
    area_last		=   pArea;	/* Thanks, Walker. */
    ch->desc->pEdit	=   (void *)pArea;

    SET_BIT( pArea->area_flags, AREA_ADDED );
    send_to_char( "Area Created.\n\r", ch );
    return TRUE;	/* OLC 1.1b */
}


bool aedit_identity( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    EDIT_AREA(ch, pArea);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	 identity [$name]\n\r", ch );
	return FALSE;
    }

    free_string( pArea->identity);
    pArea->identity = str_dup( argument);

    send_to_char( "Identity set.\n\r", ch );
    return TRUE;
}

bool aedit_name( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    EDIT_AREA(ch, pArea);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	 name [$name]\n\r", ch );
	return FALSE;
    }

    free_string( pArea->name );
    pArea->name = str_dup( argument );

    send_to_char( "Name set.\n\r", ch );
    return TRUE;
}



bool aedit_file( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char file[MAX_STRING_LENGTH];
    int i, length;

    EDIT_AREA(ch, pArea);

    one_argument( argument, file );	/* Forces Lowercase */

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	filename [$file]\n\r", ch );
	return FALSE;
    }

    /*
     * Simple Syntax Check.
     */
    length = strlen( argument );
    if ( length > 8 )
    {
	send_to_char( "No more than eight characters allowed.\n\r", ch );
	return FALSE;
    }

    /*
     * Allow only letters and numbers.
     */
    for ( i = 0; i < length; i++ )
    {
	if ( !isalnum( file[i] ) )
	{
	    send_to_char( "Only letters and numbers are valid.\n\r", ch );
	    return FALSE;
	}
    }	 

    free_string( pArea->filename );
    strcat( file, ".are" );
    pArea->filename = str_dup( file );

    send_to_char( "Filename set.\n\r", ch );
    return TRUE;
}



bool aedit_age( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char age[MAX_STRING_LENGTH];

    EDIT_AREA(ch, pArea);

    one_argument( argument, age );

    if ( !is_number( age ) || age[0] == '\0' )
    {
	send_to_char( "用法：	age [#age]\n\r", ch );
	return FALSE;
    }

    pArea->age = atoi( age );

    send_to_char( "Age set.\n\r", ch );
    return TRUE;
}



bool aedit_recall( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char room[MAX_STRING_LENGTH];
    int	 value;

    EDIT_AREA(ch, pArea);

    one_argument( argument, room );

    if ( !is_number( argument ) || argument[0] == '\0' )
    {
	send_to_char( "用法：	recall [#rvnum]\n\r", ch );
	return FALSE;
    }

    value = atoi( room );

    if ( !get_room_index( value ) )
    {
	send_to_char( "AEdit:  Room vnum does not exist.\n\r", ch );
	return FALSE;
    }

    pArea->recall = value;

    send_to_char( "Recall set.\n\r", ch );
    return TRUE;
}



bool aedit_security( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char sec[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int	 value;

    EDIT_AREA(ch, pArea);

    one_argument( argument, sec );

    if ( !is_number( sec ) || sec[0] == '\0' )
    {
	send_to_char( "用法：	security [#level]\n\r", ch );
	return FALSE;
    }

    value = atoi( sec );

    if ( value > ch->pcdata->security || value < 0 )
    {
	if ( ch->pcdata->security != 0 )
	{
	    sprintf( buf, "Security is 0-%d.\n\r", ch->pcdata->security );
	    send_to_char( buf, ch );
	}
	else
	    send_to_char( "Security is 0 only.\n\r", ch );
	return FALSE;
    }

    pArea->security = value;

    send_to_char( "Security set.\n\r", ch );
    return TRUE;
}



bool aedit_builder( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char name[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    EDIT_AREA(ch, pArea);

    one_argument( argument, name );

    if ( name[0] == '\0' )
    {
	send_to_char( "用法：	builder [$name]	 -toggles builder\n\r", ch );
	send_to_char( "用法：	builder All	 -allows everyone\n\r", ch );
	return FALSE;
    }

    name[0] = UPPER( name[0] );

    if ( strstr( pArea->builders, name ) != '\0' )
    {
	pArea->builders = string_replace( pArea->builders, name, "\0" );
	pArea->builders = string_unpad( pArea->builders );

	if ( pArea->builders[0] == '\0' )
	{
	    free_string( pArea->builders );
	    pArea->builders = str_dup( "None" );
	}
	send_to_char( "Builder removed.\n\r", ch );
	return TRUE;
    }
    else
    {
	buf[0] = '\0';
	if ( strstr( pArea->builders, "None" ) != '\0' )
	{
	    pArea->builders = string_replace( pArea->builders, "None", "\0" );
	    pArea->builders = string_unpad( pArea->builders );
	}

	if (pArea->builders[0] != '\0' )
	{
	    strcat( buf, pArea->builders );
	    strcat( buf, " " );
	}
	strcat( buf, name );
	free_string( pArea->builders );
	pArea->builders = string_proper( str_dup( buf ) );

	send_to_char( "Builder added.\n\r", ch );
	return TRUE;
    }

    return FALSE;
}



bool aedit_vnum( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char lower[MAX_STRING_LENGTH];
    char upper[MAX_STRING_LENGTH];
    int	 ilower;
    int	 iupper;

    EDIT_AREA(ch, pArea);

    argument = one_argument( argument, lower );
    one_argument( argument, upper );

    if ( !is_number( lower ) || lower[0] == '\0'
    || !is_number( upper ) || upper[0] == '\0' )
    {
	send_to_char( "用法：	vnum [#lower] [#upper]\n\r", ch );
	return FALSE;
    }

    if ( ( ilower = atoi( lower ) ) > ( iupper = atoi( upper ) ) )
    {
	send_to_char( "AEdit:  Upper must be larger then lower.\n\r", ch );
	return FALSE;
    }
    
    /* OLC 1.1b */
    if ( ilower <= 0 || ilower >= INT_MAX || iupper <= 0 || iupper >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	sprintf( output, "AEdit: vnum must be between 0 and %d.\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( !check_range( ilower, iupper ) )
    {
	send_to_char( "AEdit:  Range must include only this area.\n\r", ch );
	return FALSE;
    }

    if ( get_vnum_area( ilower )
    && get_vnum_area( ilower ) != pArea )
    {
	send_to_char( "AEdit:  Lower vnum already assigned.\n\r", ch );
	return FALSE;
    }

    pArea->lvnum = ilower;
    send_to_char( "Lower vnum set.\n\r", ch );

    if ( get_vnum_area( iupper )
    && get_vnum_area( iupper ) != pArea )
    {
	send_to_char( "AEdit:  Upper vnum already assigned.\n\r", ch );
	return TRUE;	/* The lower value has been set. */
    }

    pArea->uvnum = iupper;
    send_to_char( "Upper vnum set.\n\r", ch );

    return TRUE;
}



bool aedit_lvnum( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char lower[MAX_STRING_LENGTH];
    int	 ilower;
    int	 iupper;

    EDIT_AREA(ch, pArea);

    one_argument( argument, lower );

    if ( !is_number( lower ) || lower[0] == '\0' )
    {
	send_to_char( "用法：	lvnum [#lower]\n\r", ch );
	return FALSE;
    }

    if ( ( ilower = atoi( lower ) ) > ( iupper = pArea->uvnum ) )
    {
	send_to_char( "AEdit:  Value must be less than the uvnum.\n\r", ch );
	return FALSE;
    }
    
    /* OLC 1.1b */
    if ( ilower <= 0 || ilower >= INT_MAX || iupper <= 0 || iupper >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	sprintf( output, "AEdit: vnum must be between 0 and %d.\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( !check_range( ilower, iupper ) )
    {
	send_to_char( "AEdit:  Range must include only this area.\n\r", ch );
	return FALSE;
    }

    if ( get_vnum_area( ilower )
    && get_vnum_area( ilower ) != pArea )
    {
	send_to_char( "AEdit:  Lower vnum already assigned.\n\r", ch );
	return FALSE;
    }

    pArea->lvnum = ilower;
    send_to_char( "Lower vnum set.\n\r", ch );
    return TRUE;
}



bool aedit_uvnum( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char upper[MAX_STRING_LENGTH];
    int	 ilower;
    int	 iupper;

    EDIT_AREA(ch, pArea);

    one_argument( argument, upper );

    if ( !is_number( upper ) || upper[0] == '\0' )
    {
	send_to_char( "用法：	uvnum [#upper]\n\r", ch );
	return FALSE;
    }

    if ( ( ilower = pArea->lvnum ) > ( iupper = atoi( upper ) ) )
    {
	send_to_char( "AEdit:  Upper must be larger then lower.\n\r", ch );
	return FALSE;
    }
    
    /* OLC 1.1b */
    if ( ilower <= 0 || ilower >= INT_MAX || iupper <= 0 || iupper >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	sprintf( output, "AEdit: vnum must be between 0 and %d.\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( !check_range( ilower, iupper ) )
    {
	send_to_char( "AEdit:  Range must include only this area.\n\r", ch );
	return FALSE;
    }

    if ( get_vnum_area( iupper )
    && get_vnum_area( iupper ) != pArea )
    {
	send_to_char( "AEdit:  Upper vnum already assigned.\n\r", ch );
	return FALSE;
    }

    pArea->uvnum = iupper;
    send_to_char( "Upper vnum set.\n\r", ch );

    return TRUE;
}



/*
 * Room Editor Functions.
 */
bool redit_show( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*pRoom;
    char		buf  [MAX_STRING_LENGTH];
    char		buf1 [2*MAX_STRING_LENGTH];
    OBJ_DATA		*obj;
    CHAR_DATA		*rch;
    int			door;
    bool		fcnt;
    
    EDIT_ROOM(ch, pRoom);

    buf1[0] = '\0';
    

    sprintf( buf, "房間名稱:       [%s]\n\r所在區域:	    [%5d] %s\n\r",
	    pRoom->name, pRoom->area->vnum, pRoom->area->name );
    strcat( buf1, buf );

    sprintf( buf, "Vnum:       [%5d]\n\rSector:	    [%s]\n\r",
	    pRoom->vnum, flag_string( sector_flags, pRoom->sector_type ) );
    strcat( buf1, buf );
    sprintf( buf, "描述:\n\r%s", pRoom->description );
    strcat( buf1, buf );

    sprintf( buf, "Room flags: [%s]\n\r",
	    flag_string( room_flags, pRoom->room_flags ) );
    strcat( buf1, buf );

    if ( pRoom->extra_descr )
    {
	EXTRA_DESCR_DATA *ed;

	strcat( buf1, "額外描述:  [" );
	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    strcat( buf1, ed->keyword );
	    if ( ed->next )
		strcat( buf1, " " );
	}
	strcat( buf1, "]\n\r" );
    }

    strcat( buf1, "房內人物: [" );
    fcnt = FALSE;
    for ( rch = pRoom->people; rch; rch = rch->next_in_room )
    {
	if ( rch->deleted )
	    continue;
	if( !IS_NPC(rch) && IS_SET(rch->act, PLR_WIZINVIS))
            continue;
	one_argument( rch->name, buf );
	strcat( buf1, buf );
	strcat( buf1, " " );
	fcnt = TRUE;
    }

    if ( fcnt )
    {
	int end;

	end = strlen(buf1) - 1;
	buf1[end] = ']';
	strcat( buf1, "\n\r" );
    }
    else
	strcat( buf1, "none]\n\r" );

    strcat( buf1, "房內物品:    [" );
    fcnt = FALSE;
    for ( obj = pRoom->contents; obj; obj = obj->next_content )
    {
	if ( obj->deleted )
	    continue;
	one_argument( obj->name, buf );
	strcat( buf1, buf );
	strcat( buf1, " " );
	fcnt = TRUE;
    }

    if ( fcnt )
    {
	int end;

	end = strlen(buf1) - 1;
	buf1[end] = ']';
	strcat( buf1, "\n\r" );
    }
    else
	strcat( buf1, "none]\n\r" );

    for ( door = 0; door < MAX_DIR; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = pRoom->exit[door] ) )
	{
	    char word[MAX_INPUT_LENGTH];
	    char reset_state[MAX_STRING_LENGTH];
	    char *state;
	    int i, length;

	    sprintf( buf, "-%-5s to [%5d] Key: [%5d]",
		capitalize(dir_name[door]),
		pexit->to_room ? pexit->to_room->vnum : 0,
		pexit->key );
	    strcat( buf1, buf );

	    /*
	     * Format up the exit info.
	     * Capitalize all flags that are not part of the reset info.
	     */
	    strcpy( reset_state, flag_string( exit_flags, pexit->rs_flags ) );
	    state = flag_string( exit_flags, pexit->exit_info );
	    strcat( buf1, " Exit flags: [" );
	    for (; ;)
	    {
		state = one_argument( state, word );

		if ( word[0] == '\0' )
		{
		    int end;

		    end = strlen(buf1) - 1;
		    buf1[end] = ']';
		    strcat( buf1, "\n\r" );
		    break;
		}

		if ( str_infix( word, reset_state ) )
		{
		    length = strlen(word);
		    for (i = 0; i < length; i++)
			word[i] = toupper(word[i]);
		}
		strcat( buf1, word );
		strcat( buf1, " " );
	    }

	    if ( pexit->keyword && pexit->keyword[0] != '\0' )
	    {
		sprintf( buf, "Kwds: [%s]\n\r", pexit->keyword );
		strcat( buf1, buf );
	    }
	    if ( pexit->description && pexit->description[0] != '\0' )
	    {
		sprintf( buf, "%s", pexit->description );
		strcat( buf1, buf );
	    }
	}
    }

    send_to_char( buf1, ch );
    return FALSE;
}


/* OLC 1.1b */
/*****************************************************************************
 Name:		change_exit
 Purpose:	Command interpreter for changing exits.
 Called by:	redit_<dir>.  This is a local function.
 ****************************************************************************/
bool change_exit( CHAR_DATA *ch, char *argument, int door )
{
    ROOM_INDEX_DATA *pRoom;
    AREA_DATA *pArea;
    char command[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char total_arg[MAX_STRING_LENGTH];
    int	 rev;
    int	 value=0;

    EDIT_ROOM(ch, pRoom);

    /* Often used data. */
    rev = rev_dir[door];
    
    if ( argument[0] == '\0' )
    {
	do_help( ch, "EXIT" );
	return FALSE;
    }

    /*
     * Now parse the arguments.
     */
    strcpy( total_arg, argument );
    argument = one_argument( argument, command );
    one_argument( argument, arg );

    if ( !str_cmp( command, "delete" ) )
    {	
	if ( !pRoom->exit[door] )
	{
	    send_to_char( "沒有這個出口  .\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_DELETE_EXIT)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	/*
	 * Remove To Room Exit.
	 */
	if ( pRoom->exit[door]->to_room && pRoom->exit[door]->to_room->exit[rev] )
	{
	    free_exit( pRoom->exit[door]->to_room->exit[rev] );
	    pRoom->exit[door]->to_room->exit[rev] = NULL;
	}

	/*
	 * Remove this exit.
	 */
	free_exit( pRoom->exit[door] );
	pRoom->exit[door] = NULL;

	send_to_char( "出口刪除成功\.\n\r", ch );
	return TRUE;
    }

    /*
     * Create a two-way exit.
     */
    if ( !str_cmp( command, "link" ) )
    {
	EXIT_DATA	*pExit;
	ROOM_INDEX_DATA	*pLinkRoom;

	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "格式:  [方向] link [vnum]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_TWOWAY_EXIT)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	if ( !( pLinkRoom = get_room_index( atoi(arg) ) ) )
	{
	    send_to_char( "REdit:  目標房間不存在.\n\r", ch );
	    return FALSE;
	}

	if ( !IS_BUILDER( ch, pLinkRoom->area ) && !IS_PPLBUILDER(ch, pLinkRoom->area))
	{
	    send_to_char( "REdit:  不能連到那個區域.\n\r", ch );
	    return FALSE;
	}

	if ( pLinkRoom->exit[rev] )
	{
	    send_to_char( "REdit:  那裡已經有對應方向的出口了.\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )		/* No exit.		*/
	    pRoom->exit[door] = new_exit();

	pRoom->exit[door]->to_room = pLinkRoom;	/* Assign data.		*/
	pRoom->exit[door]->vnum = value;

	pExit			= new_exit();	/* No remote exit.	*/

	pExit->to_room		= ch->in_room;	/* Assign data.		*/
	pExit->vnum		= ch->in_room->vnum;

	pLinkRoom->exit[rev]	= pExit;	/* Link exit to room.	*/

	send_to_char( "雙向出入口建立成功\。\n\r", ch );
	return TRUE;
    }

    /*
     * Create room and make two-way exit.
     */
    if ( !str_cmp( command, "dig" ) )
    {
	char buf[MAX_INPUT_LENGTH];
	
	    if ( arg[0] == '\0' || !is_number( arg ) )
	    {
		send_to_char( "格式: [方向] dig <vnum>\n\r", ch );
		return FALSE;
	    }


	    redit_create( ch, arg );		/* Create the room.	*/
	    sprintf( buf, "link %s", arg );
	    change_exit( ch, buf, door);		/* Create the exits.	*/
	    return TRUE;
    }

    /*
     * Create one-way exit.
     */
    if ( !str_cmp( command, "room" ) )
    {
	ROOM_INDEX_DATA *pLinkRoom;

	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "格式:  [方向] room [vnum]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_ONEWAY_EXIT)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	if ( !( pLinkRoom = get_room_index( atoi( arg ) ) ) )
	{
	    send_to_char( "REdit:  房間不存在.\n\r", ch );
	    return FALSE;
	}

	if ( !IS_BUILDER( ch, pLinkRoom->area ) && !IS_PPLBUILDER(ch, pLinkRoom->area))
	{
	    send_to_char( "REdit:  不能連到那個區域.\n\r", ch );
	    return FALSE;
	}


	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	pRoom->exit[door]->to_room = pLinkRoom;
	pRoom->exit[door]->vnum = value;

	send_to_char( "單向出口建立成功\.\n\r", ch );
	return TRUE;
    }

    if ( !str_cmp( command, "remove" ) )
    {
	if ( arg[0] == '\0' )
	{
	    send_to_char( "格式:  [方向] remove [key/name/desc]\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	{
	    send_to_char( "REdit:  出口不存在.\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_REMOVE_KEY)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	if ( !str_cmp( argument, "key" ) )
	{
	    pRoom->exit[door]->key = 0;
	    send_to_char( "鑰匙刪除成功\。.\n\r", ch );			
	    return TRUE;
	}

	if ( !str_cmp( argument, "name" ) )
	{
	    free_string( pRoom->exit[door]->keyword );
	    pRoom->exit[door]->keyword = &str_empty[0];
	    send_to_char( "名稱刪除成功\.\n\r", ch );			 
	    return TRUE;
	}

	if ( argument[0] == 'd' && !str_prefix( argument, "description" ) )
	{
	    free_string( pRoom->exit[door]->description );
	    pRoom->exit[door]->description = &str_empty[0];
	    send_to_char( "描述刪除成功\.\n\r", ch );			
	    return TRUE;
	}

	send_to_char( "格式:	[方向] remove [key/name/desc]\n\r", ch );
	return FALSE;
    }

    if ( !str_cmp( command, "key" ) )
    {
	OBJ_INDEX_DATA *pObjIndex;

	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "格式:  [方向] key [vnum]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_SET_KEY)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	if ( !( pObjIndex = get_obj_index( atoi( arg ) ) ) )
	{
	    send_to_char( "REdit:  物品不存在.\n\r", ch );
	    return FALSE;
	}

	pArea = get_vnum_area( atoi(arg));
	if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
	{
	    send_to_char( "REdit:  不能指定那樣物品.\n\r", ch );
	    return FALSE;
	}

	if ( pObjIndex->item_type != ITEM_KEY )
	{
	    send_to_char( "REdit:  那不是鑰匙。\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	pRoom->exit[door]->key = pObjIndex->vnum;

	send_to_char( "鑰匙設定完成.\n\r", ch );
	return TRUE;
    }

    if ( !str_cmp( command, "name" ) )
    {
	if ( arg[0] == '\0' )
	{
	    send_to_char( "格式:  [方向] name [字串]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_SET_DOORNAME)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	free_string( pRoom->exit[door]->keyword );
	pRoom->exit[door]->keyword = str_dup( argument );

	send_to_char( "名稱設定完成.\n\r", ch );
	return TRUE;
    }

    if ( command[0] == 'd' && !str_prefix( command, "description" ) )
    {
	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_SET_DOORDESC)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
	
	if ( arg[0] == '\0' )
	{
	    if ( !pRoom->exit[door] )
		pRoom->exit[door] = new_exit();

	    string_append( ch, &pRoom->exit[door]->description );
	    return TRUE;
	}

	send_to_char( "格式:	[方向] desc\n\r", ch );
	return FALSE;
    }

    /*
     * Set the exit flags, needs full argument.
     * ----------------------------------------
     */
    //if ( ( value = flag_value( exit_flags, total_arg ) ) != NO_FLAG )
    if ( ( value = flag_value( exit_flags, command) ) != NO_FLAG )
    {
	ROOM_INDEX_DATA *pToRoom;
	int exit_c = flag_cost( exit_cost, command );
	if( !IS_IMMORTAL(ch) && exit_c < 0 ) {
	    send_to_char("沒有這個exit flags或是你不能設定。\n\r", ch);
	    return FALSE;
	}

	//if( pRoom->exit[door] && IS_SET(pRoom->exit[door]->rs_flags, value))
	  //  exit_c *= -1;

	if( !IS_IMMORTAL(ch) && !check_cost(ch, exit_c)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	/*
	 * Create an exit if none exists.
	 */
	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	/*
	 * Set door bits for this room.
	 */
	TOGGLE_BIT(pRoom->exit[door]->rs_flags, value);
	pRoom->exit[door]->exit_info = pRoom->exit[door]->rs_flags;

	/*
	 * Set door bits of connected room.
	 * Skip one-way exits and non-existant rooms.
	 */
	if ( ( pToRoom = pRoom->exit[door]->to_room ) && pToRoom->exit[rev] )
	{
	    TOGGLE_BIT(pToRoom->exit[rev]->rs_flags, value);
	    pToRoom->exit[rev]->exit_info =  pToRoom->exit[rev]->rs_flags;
	}

	send_to_char( "Exit flag toggled.\n\r", ch );
	return TRUE;
    }

    return FALSE;
}



bool redit_north( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_NORTH ) )
	return TRUE;

    return FALSE;
}



bool redit_south( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_SOUTH ) )
	return TRUE;

    return FALSE;
}



bool redit_east( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_EAST ) )
	return TRUE;

    return FALSE;
}



bool redit_west( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_WEST ) )
	return TRUE;

    return FALSE;
}



bool redit_up( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_UP ) )
	return TRUE;

    return FALSE;
}



bool redit_down( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_DOWN ) )
	return TRUE;

    return FALSE;
}


/* OLC 1.1b */
bool redit_move( CHAR_DATA *ch, char *argument )
{
    interpret( ch, argument );
    return FALSE;
}



bool redit_ed( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    EXTRA_DESCR_DATA *ed;
    char command[MAX_INPUT_LENGTH];
    char keyword[MAX_INPUT_LENGTH];

    EDIT_ROOM(ch, pRoom);

    argument = one_argument( argument, command );
    one_argument( argument, keyword );

    if ( command[0] == '\0' || keyword[0] == '\0' )
    {
	send_to_char( "格式:	ed add [keyword]\n\r", ch );
	send_to_char( "		ed edit [keyword]\n\r", ch );
	send_to_char( "		ed delete [keyword]\n\r", ch );
	send_to_char( "		ed format [keyword]\n\r", ch );
	return FALSE;
    }

    if ( !str_cmp( command, "add" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "用法：  ed add [keyword]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_SET_ROOMED)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}

	ed			=   new_extra_descr();
	ed->keyword		=   str_dup( keyword );
	ed->description		=   str_dup( "" );
	ed->next		=   pRoom->extra_descr;
	pRoom->extra_descr	=   ed;

	string_append( ch, &ed->description );

	return TRUE;
    }


    if ( !str_cmp( command, "edit" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed edit [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	}

	if ( !ed )
	{
	    send_to_char( "REdit:  沒有這個關鍵字.\n\r", ch );
	    return FALSE;
	}

	string_append( ch, &ed->description );

	return TRUE;
    }


    if ( !str_cmp( command, "delete" ) )
    {
	EXTRA_DESCR_DATA *ped = NULL;

	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed delete [keyword]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_DEL_ROOMED)){
	    send_to_char("你的幫費不夠了。\n\r", ch) ;
	    return FALSE;
	}

	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	    ped = ed;
	}

	if ( !ed )
	{
	    send_to_char( "REdit:  沒有這個關鍵字.\n\r", ch );
	    return FALSE;
	}

	if ( !ped )
	    pRoom->extra_descr = ed->next;
	else
	    ped->next = ed->next;

	free_extra_descr( ed );

	send_to_char( "額外敘述刪除完成.\n\r", ch );
	return TRUE;
    }


    if ( !str_cmp( command, "format" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed format [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	}

	if ( !ed )
	{
	    send_to_char( "REdit:  沒有這個關鍵字.\n\r", ch );
	    return FALSE;
	}

	/* OLC 1.1b */
	if ( strlen(ed->description) >= (MAX_STRING_LENGTH - 4) )
	{
	    send_to_char( "敘述太長了.\n\r", ch );
	    return FALSE;
	}

	ed->description = format_string( ed->description );

	send_to_char( "自動格式化完成.\n\r", ch );
	return TRUE;
    }

    redit_ed( ch, "" );
    return FALSE;
}


bool pplredit_create( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    int value;
    int iHash;
    
    EDIT_ROOM(ch, pRoom);

    for( value = ch->in_room->area->lvnum; value <= ch->in_room->area->uvnum; ++value){
	if( !get_room_index(value)) break;
    }
    if( value > ch->in_room->area->uvnum ) {
	send_to_char("你沒有足夠的編號再建新房間了，請向大神申請。\n\r", ch);
	return FALSE;
    }

    pArea = ch->in_room->area;

    if ( !IS_PPLBUILDER( ch, pArea ) )
    {
	send_to_char("你不能在這裡建立房間。\n\r", ch);
	return FALSE;
    }

    if ( get_room_index( value ) )
    {
	send_to_char( "REdit:  Room vnum already exists.\n\r", ch );
	return FALSE;
    }

    if( !check_cost(ch, COST_CREATE_ROOM)){
	send_to_char("你的幫費不夠了。 \n\r", ch);
	return FALSE;
    }

    pRoom			= new_room_index();
    pRoom->area			= pArea;
    pRoom->vnum			= value;

    if ( value > top_vnum_room )
	top_vnum_room = value;

    iHash			= value % MAX_KEY_HASH;
    pRoom->next			= room_index_hash[iHash];
    room_index_hash[iHash]	= pRoom;
    ch->desc->pEdit		= (void *)pRoom;

    send_to_char( "房間建立成功\.\n\r", ch );
    return TRUE;
}

bool redit_create( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    int value;
    int iHash;
    
    if( !IS_IMMORTAL(ch) ) return pplredit_create(ch, argument);
    EDIT_ROOM(ch, pRoom);

    value = atoi( argument );

    /* OLC 1.1b */
    if ( argument[0] == '\0' || value <= 0 || value >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	sprintf( output, "用法：  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( argument[0] == '\0' || value <= 0 )
    {
	send_to_char( "用法：	create [vnum > 0]\n\r", ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );
    if ( !pArea )
    {
	send_to_char( "REdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	send_to_char( "REdit:  Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if ( get_room_index( value ) )
    {
	send_to_char( "REdit:  Room vnum already exists.\n\r", ch );
	return FALSE;
    }

    pRoom			= new_room_index();
    pRoom->area			= pArea;
    pRoom->vnum			= value;

    if ( value > top_vnum_room )
	top_vnum_room = value;

    iHash			= value % MAX_KEY_HASH;
    pRoom->next			= room_index_hash[iHash];
    room_index_hash[iHash]	= pRoom;
    ch->desc->pEdit		= (void *)pRoom;

    send_to_char( "Room created.\n\r", ch );
    return TRUE;
}



bool redit_name( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    EDIT_ROOM(ch, pRoom);

    if ( argument[0] == '\0' )
    {
	send_to_char( "格式:	name [name]\n\r", ch );
	return FALSE;
    }

    free_string( pRoom->name );
    pRoom->name = str_dup( argument );

    send_to_char( "設定完成.\n\r", ch );
    return TRUE;
}


bool aedit_ftoggle( CHAR_DATA *ch, char *argument )
{

    AREA_DATA *pArea;
    char arg[MAX_INPUT_LENGTH];
    long long int value;

    EDIT_AREA(ch, pArea);

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	send_to_char("格式： ftoggle [room flags]\n\r請用 ? room 來取得列表。\n\r", ch);
	return FALSE;
    }

    if ( ( value = flag_value( room_flags, arg ) ) != NO_FLAG )
    {
	int cost = flag_cost(room_cost , arg);
	bool fSet = TRUE;
	if(!IS_IMMORTAL(ch)){
	    if( cost < 0 ) {
		send_to_char("沒有這個 room flag，或是你沒有權力設定。\n\r", ch);
		return FALSE;
	    }
	    if( IS_SET(pArea->f_toggle, value)) {
		fSet = FALSE;
		//cost *= -1;
		check_cost(ch, cost);
	    }
	    else {
		if( !check_cost(ch, cost)){
		    send_to_char("你幫裡的錢不夠了。\n\r", ch);
		    return FALSE;
		}
	    }

	}
	TOGGLE_BIT(pArea->f_toggle, value);

	send_to_char( "Room flag toggled.\n\r", ch );
	return TRUE;
    }

    send_to_char("無此 room flags，請用 ? room 取得列表。\n\r", ch);
    return FALSE;
}

bool redit_room( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    AREA_DATA *pArea;
    char arg[MAX_INPUT_LENGTH];

    EDIT_ROOM(ch, pRoom);
    pArea = pRoom->area;

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	send_to_char("格式： room [room flags]\n\r請用 ? room 來取得列表。\n\r", ch);
	return FALSE;
    }

	if ( !check_clco( room_cost, arg, ch ) )
		return FALSE;

		TOGGLE_BIT(pRoom->room_flags, flag_value( room_flags, arg ));

	send_to_char( "好的，房間特定建築已遵照指示完成或取消。\n\r", ch );
	return TRUE;
}

bool redit_sector( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    AREA_DATA *pArea;
    char arg[MAX_INPUT_LENGTH];

    EDIT_ROOM(ch, pRoom);
    pArea = pRoom->area;

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	send_to_char("格式： sector [sector type]\n\r請用 ? sector來取得列表。\n\r", ch);
	return FALSE;
    }

	if ( !check_clco( sector_cost, arg, ch ) )
		return FALSE;

	pRoom->sector_type  = flag_value( sector_flags, arg );

	send_to_char( "地形設定完成.\n\r", ch );
	return TRUE;
}

bool redit_desc( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    EDIT_ROOM(ch, pRoom);

    if ( argument[0] == '\0' )
    {
	string_append( ch, &pRoom->description );
	return TRUE;
    }

    send_to_char( "用法：  desc\n\r", ch );
    return FALSE;
}




bool redit_format( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    EDIT_ROOM(ch, pRoom);

    /* OLC 1.1b */
    if ( strlen(pRoom->description) >= (MAX_STRING_LENGTH - 4) )
    {
	send_to_char( "String too long to be formatted.\n\r", ch );
	return FALSE;
    }

    pRoom->description = format_string( pRoom->description );

    send_to_char( "String formatted.\n\r", ch );
    return TRUE;
}



bool redit_mreset( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*pRoom;
    MOB_INDEX_DATA	*pMobIndex;
    CHAR_DATA		*newmob;
    char		arg [ MAX_INPUT_LENGTH ];

    RESET_DATA		*pReset;
    char		output [ MAX_STRING_LENGTH ];

    EDIT_ROOM(ch, pRoom);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number( arg ) )
    {
	send_to_char ( "用法：	 mreset <vnum> <max #>\n\r", ch );
	return FALSE;
    }

    if ( !( pMobIndex = get_mob_index( atoi( arg ) ) ) )
    {
	send_to_char( "REdit: No mobile has that vnum.\n\r", ch );
	return FALSE;
    }

    if ( pMobIndex->area != pRoom->area )
    {
	send_to_char( "REdit: No such mobile in this area.\n\r", ch );
	return FALSE;
    }

    /*
     * Create the mobile reset.
     */
    pReset = new_reset_data();
    pReset->command	= 'M';
    pReset->arg1	= pMobIndex->vnum;
    pReset->arg2	= is_number( argument ) ? atoi( argument ) : MAX_MOB;
    pReset->arg3	= pRoom->vnum;
    add_reset( pRoom, pReset, 0/* Last slot*/ );

    /*
     * Create the mobile.
     */
    newmob = create_mobile( pMobIndex );
    char_to_room( newmob, pRoom );
    mprog_repop_trigger(newmob);

    sprintf( output, "%s(%s) (%d) 已經被加入到 resets 裡。\n\r"
	"將會有 %d 隻生物在 reset 時被放到這個房間。\n\r",
	capitalize( pMobIndex->short_descr ),
	pMobIndex->player_name,
	pMobIndex->vnum,
	pReset->arg2 );
    send_to_char( output, ch );
    act( "$n has created $N!", ch, NULL, newmob, TO_ROOM );
    return TRUE;
}

struct wear_type
{
    int	wear_loc;
    int	wear_bit;
};



const struct wear_type wear_table_olc[] =
{
    {	WEAR_NONE,	ITEM_TAKE		},
    {	WEAR_LIGHT,	ITEM_LIGHT		},
    {	WEAR_FINGER_L,	ITEM_WEAR_FINGER	},
    {	WEAR_FINGER_R,	ITEM_WEAR_FINGER	},
    {	WEAR_NECK_1,	ITEM_WEAR_NECK		},
    {	WEAR_NECK_2,	ITEM_WEAR_NECK		},
    {	WEAR_BODY,	ITEM_WEAR_BODY		},
    {	WEAR_HEAD,	ITEM_WEAR_HEAD		},
    {	WEAR_LEGS,	ITEM_WEAR_LEGS		},
    {	WEAR_FEET,	ITEM_WEAR_FEET		},
    {	WEAR_HANDS,	ITEM_WEAR_HANDS		},
    {	WEAR_ARMS,	ITEM_WEAR_ARMS		},
    {	WEAR_SHIELD,	ITEM_WEAR_SHIELD	},
    {	WEAR_ABOUT,	ITEM_WEAR_ABOUT		},
    {	WEAR_WAIST,	ITEM_WEAR_WAIST		},
    {	WEAR_WRIST_L,	ITEM_WEAR_WRIST		},
    {	WEAR_WRIST_R,	ITEM_WEAR_WRIST		},
    {	WEAR_WIELD,	ITEM_WIELD		},
    {	WEAR_HOLD,	ITEM_HOLD		},
    {	WEAR_TWO_HAND,	ITEM_TWO_HAND	    },
    {	WEAR_ARROW  ,	ITEM_ARROW  },
    {   WEAR_RIDE   ,   ITEM_WEAR_RIDE},
    {	NO_FLAG,	NO_FLAG			}
};



/*****************************************************************************
 Name:		wear_loc
 Purpose:	Returns the location of the bit that matches the count.
		1 = first match, 2 = second match etc.
 Called by:	oedit_reset(olc_act.c).
 ****************************************************************************/
int wear_loc(int bits, int count)
{
    int flag;
 
    for (flag = 0; wear_table_olc[flag].wear_bit != NO_FLAG; flag++)
    {
	if ( IS_SET(bits, wear_table_olc[flag].wear_bit) && --count < 1)
	    return wear_table_olc[flag].wear_loc;
    }
 
    return NO_FLAG;
}



/*****************************************************************************
 Name:		wear_bit
 Purpose:	Converts a wear_loc into a bit.
 Called by:	redit_oreset(olc_act.c).
 ****************************************************************************/
int wear_bit(int loc)
{
    int flag;
 
    for (flag = 0; wear_table_olc[flag].wear_loc != NO_FLAG; flag++)
    {
	if ( loc == wear_table_olc[flag].wear_loc )
	    return wear_table_olc[flag].wear_bit;
    }
 
    return 0;
}


bool redit_purge( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    OBJ_DATA *obj, *obj_next;
    CHAR_DATA *victim, *vnext;

    EDIT_ROOM(ch, pRoom);

    if( !IS_BUILDER(ch, pRoom->area ) && !IS_PPLBUILDER(ch, pRoom->area)){
	send_to_char("你的權限不夠。\n\r", ch);
	return FALSE;
    }
    for ( victim = ch->in_room->people; victim; victim = vnext )
    {
	vnext = victim->next_in_room;
	if ( victim->deleted )
	    continue;

	if ( IS_NPC( victim ) && victim != ch )
	    extract_char( victim, TRUE );
    }

    for ( obj = ch->in_room->contents; obj; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->deleted )
	    continue;
	extract_obj( obj );
    }

    send_to_char( "Ok.\n\r", ch );
    act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
    return FALSE;
}

bool redit_oreset( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*pRoom;
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*newobj;
    OBJ_DATA		*to_obj;
    CHAR_DATA		*to_mob;
    char		arg1 [ MAX_INPUT_LENGTH ];
    char		arg2 [ MAX_INPUT_LENGTH ];
    int			olevel = 0;

    RESET_DATA		*pReset;
    char		output [ MAX_STRING_LENGTH ];

    EDIT_ROOM(ch, pRoom);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char ( "格式:	 oreset <vnum> <參數>\n\r", ch );
	send_to_char ( "<參數>： 不加參數	= 	放入房內\n\r", ch );
	if( !IS_IMMORTAL(ch)){
	send_to_char ( "<參數>： <物品名稱>  = 	放入物品中\n\r", ch );
	send_to_char ( "<參數>： <mob名稱> <穿著位置> 	= 	穿在 mob 身上\n\r", ch );
	}
	return FALSE;
    }

    if ( !( pObjIndex = get_obj_index( atoi( arg1 ) ) ) )
    {
	send_to_char( "REdit: 沒有那樣物品.\n\r", ch );
	return FALSE;
    }

    if ( pObjIndex->area != pRoom->area )
    {
	send_to_char( "REdit: 這個區域沒有那樣物品.\n\r", ch );
	return FALSE;
    }

//    if( !IS_IMMORTAL(ch)){
	if( !check_cost(ch, COST_ADD_RESETOBJ)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
//	arg2[0] = '\0';
//    }

    /*
     * Load into room.
     */
    if ( arg2[0] == '\0' )
    {
	pReset		= new_reset_data();
	pReset->command	= 'O';
	pReset->arg1	= pObjIndex->vnum;
	pReset->arg2	= 0;
	pReset->arg3	= pRoom->vnum;
	add_reset( pRoom, pReset, 0/* Last slot*/ );

	newobj = create_object( pObjIndex, number_fuzzy( olevel ) );
	obj_to_room( newobj, pRoom );

	sprintf( output, "%s(%s) (%d) has been loaded and added to resets.\n\r",
	    capitalize( pObjIndex->short_descr ),
	    pObjIndex->name,
	    pObjIndex->vnum );
	send_to_char( output, ch );
    }
    else
    /*
     * Load into object's inventory.
     */
    if ( argument[0] == '\0'
    && ( ( to_obj = get_obj_list( ch, arg2, pRoom->contents ) ) != NULL ) )
    {
	pReset		= new_reset_data();
	pReset->command	= 'P';
	pReset->arg1	= pObjIndex->vnum;
	pReset->arg2	= 0;
	pReset->arg3	= to_obj->pIndexData->vnum;
	add_reset( pRoom, pReset, 0/* Last slot*/ );

	newobj = create_object( pObjIndex, number_fuzzy( olevel ) );
	newobj->cost = 0;
	obj_to_obj( newobj, to_obj );

	sprintf( output, "%s(%s) (%d) has been loaded into "
	    "%s(%s) (%d) and added to resets.\n\r",
	    capitalize( newobj->short_descr ),
	    newobj->name,
	    newobj->pIndexData->vnum,
	    to_obj->short_descr,
	    to_obj->name,
	    to_obj->pIndexData->vnum );
	send_to_char( output, ch );
    }
    else
    /*
     * Load into mobile's inventory.
     */
    if ( ( to_mob = get_char_room( ch, arg2 ) ) != NULL )
    {
	int	wear_loc;

	/*
	 * Make sure the location on mobile is valid.
	 */
	if ( (wear_loc = flag_value( wear_loc_flags, argument )) == NO_FLAG )
	{
	    send_to_char( "REdit: Invalid wear_loc.  '? wear-loc'\n\r", ch );
	    return FALSE;
	}

	/*
	 * Disallow loading a sword(WEAR_WIELD) into WEAR_HEAD.
	 */
	if ( !IS_SET( pObjIndex->wear_flags, wear_bit(wear_loc) ) )
	{
	    sprintf( output,
		"%s(%s) (%d) has wear flags: [%s]\n\r",
		capitalize( pObjIndex->short_descr ),
		pObjIndex->name,
		pObjIndex->vnum,
		flag_string( wear_flags, pObjIndex->wear_flags ) );
	    send_to_char( output, ch );
	    return FALSE;
	}

	/*
	 * Can't load into same position.
	 */
	if ( get_eq_char( to_mob, wear_loc ) )
	{
	    send_to_char( "REdit:  Object already equipped.\n\r", ch );
	    return FALSE;
	}

	pReset		= new_reset_data();
	pReset->arg1	= pObjIndex->vnum;
	pReset->arg2	= wear_loc;
	if ( pReset->arg2 == WEAR_NONE )
	    pReset->command = 'G';
	else
	    pReset->command = 'E';
	pReset->arg3	= wear_loc;

	add_reset( pRoom, pReset, 0/* Last slot*/ );

	olevel	= URANGE( 0, to_mob->level - 2, LEVEL_HERO );
	newobj = create_object( pObjIndex, number_fuzzy( olevel ) );

	if ( to_mob->pIndexData->pShop )	/* Shop-keeper? */
	{
	    switch ( pObjIndex->item_type )
	    {
	    default:		olevel = 0;				break;
	    case ITEM_PILL:	olevel = number_range(	0, 10 );	break;
	    case ITEM_POTION:	olevel = number_range(	0, 10 );	break;
	    case ITEM_SCROLL:	olevel = number_range(	5, 15 );	break;
	    case ITEM_WAND:	olevel = number_range( 10, 20 );	break;
	    case ITEM_STAFF:	olevel = number_range( 15, 25 );	break;
	    case ITEM_ARMOR:	olevel = number_range(	5, 15 );	break;
	    case ITEM_WEAPON:	if ( pReset->command == 'G' )
				    olevel = number_range( 5, 15 );
				else
				    olevel = number_fuzzy( olevel );
		break;
	    }

	    newobj = create_object( pObjIndex, olevel );
	    if ( pReset->arg2 == WEAR_NONE )
		SET_BIT( newobj->extra_flags, ITEM_INVENTORY );
	}
	else
	    newobj = create_object( pObjIndex, number_fuzzy( olevel ) );

	obj_to_char( newobj, to_mob );
	if ( pReset->command == 'E' )
	    equip_char( to_mob, newobj, pReset->arg3 );

	sprintf( output, "%s(%s) (%d) has been loaded "
	    "%s of %s(%s) (%d) and added to resets.\n\r",
	    capitalize( pObjIndex->short_descr ),
	    pObjIndex->name,
	    pObjIndex->vnum,
	    flag_string( wear_loc_strings, pReset->arg3 ),
	    to_mob->short_descr,
	    to_mob->name,
	    to_mob->pIndexData->vnum );
	send_to_char( output, ch );
    }
    else	/* Display Syntax */
    {
	send_to_char( "REdit:  That mobile isn't here.\n\r", ch );
	return FALSE;
    }

    act( "$n 伸手一指，變出了 $p!", ch, newobj, NULL, TO_ROOM );
    return TRUE;
}



/*
 * Object Editor Functions.
 */
void show_obj_values( CHAR_DATA *ch, OBJ_INDEX_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];

    switch( obj->item_type )
    {
	default:	/* No values. */
	    break;
	    
	case ITEM_LIGHT:
	    if ( obj->value[2] == -1 )
		sprintf( buf, "[v2] Light:  Infinite[-1]\n\r" );
	    else
		sprintf( buf, "[v2] Light:  [%d]\n\r", obj->value[2] );
	    send_to_char( buf, ch );
	    break;

	case ITEM_WAND:
	case ITEM_STAFF:
	    sprintf( buf,
		"[v0] Level:	      [%d]\n\r"
		"[v1] Charges Total:  [%d]\n\r"
		"[v2] Charges Left:   [%d]\n\r"
		"[v3] Spell:	      %s\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2],
		obj->value[3] != -1 ? skill_table[obj->value[3]].name
				    : "none" );
	    send_to_char( buf, ch );
	    break;

	case ITEM_SCROLL:
	case ITEM_POTION:
	case ITEM_PILL:
	    sprintf( buf,
		"[v0] Level:  [%d]\n\r"
		"[v1] Spell:  %s\n\r"
		"[v2] Spell:  %s\n\r"
		"[v3] Spell:  %s\n\r",
		obj->value[0],
		obj->value[1] != -1 ? skill_table[obj->value[1]].name
				    : "none",
		obj->value[2] != -1 ? skill_table[obj->value[2]].name
				    : "none",
		obj->value[3] != -1 ? skill_table[obj->value[3]].name
				    : "none" );
	    send_to_char( buf, ch );
	    break;
	case ITEM_ARMOR:
	    sprintf(buf,
		"[v0] Armor class  : [%d]\n\r"
		"[v1] Acid-breathed: [%d]\n\r"
		"[v2] Durability   : [%d]\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2]);
	    send_to_char(buf, ch);
	    break;

	case ITEM_ARROW:
	    sprintf( buf,
		
		"[v0] Arrow amount  : [%d]\n\r"
		"[v1] Damage minimum: [%d]\n\r"
		"[v2] Damage maximum: [%d]\n\r"
		"[v3] Extra damage  : [%d]\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2],
		obj->value[3]);
	    send_to_char( buf, ch );
	    break;
	case ITEM_WEAPON:
	    sprintf( buf,
		
		"[v0] Durability    : [%d]\n\r"
		"[v1] Damage minimum: [%d]\n\r"
		"[v2] Damage maximum: [%d]\n\r"
		"[v3] Type:	      %s\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2],
		flag_string( weapon_flags, obj->value[3] ) );
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	    sprintf( buf,
		"[v0] Weight: [%d kg]\n\r"
		"[v1] Flags:  [%s]\n\r"
		"[v2] Key:    %s [%d]\n\r",
		obj->value[0],
		flag_string( container_flags, obj->value[1] ),
		get_obj_index(obj->value[2])
		    ? get_obj_index(obj->value[2])->short_descr
		    : "none", obj->value[2]);
	    send_to_char( buf, ch );
	    break;

	case ITEM_DRINK_CON:
	    sprintf( buf,
		"[v0] Liquid Total: [%d]\n\r"
		"[v1] Liquid Left:  [%d]\n\r"
		"[v2] Liquid:	    %s\n\r"
		"[v3] Poisoned:	    %s\n\r",
		obj->value[0],
		obj->value[1],
		flag_string( liquid_flags, obj->value[2] ),
		obj->value[3] != 0 ? "Yes" : "No" );
	    send_to_char( buf, ch );
	    break;

	case ITEM_FOOD:
	    sprintf( buf,
		"[v0] Food hours: [%d]\n\r"
		"[v3] Poisoned:	  %s\n\r",
		obj->value[0],
		obj->value[3] != 0 ? "Yes" : "No" );
	    send_to_char( buf, ch );
	    break;

	case ITEM_MONEY:
	    sprintf( buf, "[v0] Gold:	[%d]\n\r", obj->value[0] );
	    send_to_char( buf, ch );
	    break;
    }

    return;
}



bool set_obj_values( CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, int value_num, char *argument)
{
    switch( pObj->item_type )
    {
	default:
	    break;
	    
	case ITEM_LIGHT:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_LIGHT" );
		    return FALSE;
		case 2:
		    send_to_char( "光源時間設定完成.\n\r\n\r", ch );
		    pObj->value[2] = atoi( argument );
		    break;
	    }
	    break;

	case ITEM_WAND:
	case ITEM_STAFF:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_STAFF_WAND" );
		    return FALSE;
		case 0:
		    send_to_char( "SPELL LEVEL SET.\n\r\n\r", ch );
		    pObj->value[0] = atoi( argument );
		    break;
		case 1:
		    send_to_char( "TOTAL NUMBER OF CHARGES SET.\n\r\n\r", ch );
		    pObj->value[1] = atoi( argument );
		    break;
		case 2:
		    send_to_char( "CURRENT NUMBER OF CHARGES SET.\n\r\n\r", ch );
		    pObj->value[2] = atoi( argument );
		    break;
		case 3:
		    send_to_char( "SPELL TYPE SET.\n\r", ch );
		    pObj->value[3] = skill_lookup( argument );
		    break;
	    }
	    break;

	case ITEM_SCROLL:
	case ITEM_POTION:
	case ITEM_PILL:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_SCROLL_POTION_PILL" );
		    return FALSE;
		case 0:
		    send_to_char( "SPELL LEVEL SET.\n\r\n\r", ch );
		    pObj->value[0] = atoi( argument );
		    break;
		case 1:
		    send_to_char( "SPELL TYPE 1 SET.\n\r\n\r", ch );
		    pObj->value[1] = skill_lookup( argument );
		    break;
		case 2:
		    send_to_char( "SPELL TYPE 2 SET.\n\r\n\r", ch );
		    pObj->value[2] = skill_lookup( argument );
		    break;
		case 3:
		    send_to_char( "SPELL TYPE 3 SET.\n\r\n\r", ch );
		    pObj->value[3] = skill_lookup( argument );
		    break;
	    }
	    break;

	case ITEM_WEAPON:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_WEAPON" );
		    return FALSE;
		case 0:
		    if( !IS_IMMORTAL(ch)) {
			int dura = atoi(argument);
			int cost = 10;
			if( dura < 10 || dura > 500 ) {
			    send_to_char("只能設定在 100 到 500 之間。\n\r", ch);
			    return FALSE;
			}
			cost *= ( dura - pObj->value[0]); 
			if( cost > 0 ){
			    if( !check_cost(ch, cost)){
				send_to_char("你的幫費不夠了。\n\r", ch);
				return FALSE;
			    }
			}
		    }
		    send_to_char("DURABILITY SET.\n\r\n\r", ch);
		    pObj->value[0] = atoi( argument );
		    break;
		case 1:
		    if( !IS_IMMORTAL(ch)){
			send_to_char("你無法設定這個欄位。\n\r", ch);
			pObj->value[0] = 0;
			return FALSE;
		    }
		    send_to_char( "MINIMUM DAMAGE SET.\n\r\n\r", ch );
		    pObj->value[1] = atoi( argument );
		    break;
		case 2:
		    if( !IS_IMMORTAL(ch)){
			send_to_char("你無法設定這個欄位。\n\r", ch);
			pObj->value[0] = 0;
			return FALSE;
		    }
		    send_to_char( "MAXIMUM DAMAGE SET.\n\r\n\r", ch );
		    pObj->value[2] = atoi( argument );
		    break;
		case 3:
		    if( !IS_IMMORTAL(ch)){
			if( !check_cost(ch, 5000)){
			    send_to_char("你的幫費不夠了。\n\r", ch);
			    return FALSE;
			}
		    }
		    send_to_char( "WEAPON TYPE SET.\n\r\n\r", ch );
		    pObj->value[3] = flag_value( weapon_flags, argument );
		    break;
	    }
	    break;

	case ITEM_CONTAINER:
	    switch ( value_num )
	    {
		int value;
		
		default:
		    do_help( ch, "ITEM_CONTAINER" );
		    return FALSE;
		case 0:
			//if ( atoi( argument ) > 50 )
			//{
			//	send_to_char( "Max capacity is 50.\n\r", ch );
			//	return FALSE;
			//}
                    if( !IS_IMMORTAL(ch) ) {
                        if( atoi(argument) > 100 ) {
                            send_to_char("Max capacity is 100.\n\r", ch);
                            return FALSE;
                        }
                        pObj->cost -= pObj->value[0] * 1000;
                        pObj->weight -= pObj->value[0] / 5;
                    }
		    send_to_char( "WEIGHT CAPACITY SET.\n\r\n\r", ch );
		    pObj->value[0] = atoi( argument );
                    if( !IS_IMMORTAL(ch) ) {
                       pObj->cost += pObj->value[0] * 1000;
                       pObj->weight += pObj->value[0] / 5;
                    }
		    break;
		case 1:
		    if ( ( value = flag_value( container_flags, argument ) )
		      != NO_FLAG )
			TOGGLE_BIT(pObj->value[1], value);
		    else
		    {
			do_help ( ch, "ITEM_CONTAINER" );
			return FALSE;
		    }
		    send_to_char( "CONTAINER TYPE SET.\n\r\n\r", ch );
		    break;
		case 2:
		    if ( atoi(argument) != 0 )
		    {
			if ( !get_obj_index( atoi( argument ) ) )
			{
			    send_to_char( "THERE IS NO SUCH ITEM.\n\r\n\r", ch );
			    return FALSE;
			}

			if ( get_obj_index( atoi( argument ) )->item_type != ITEM_KEY )
			{
			    send_to_char( "THAT ITEM IS NOT A KEY.\n\r\n\r", ch );
			    return FALSE;
			}
		    }
		    send_to_char( "CONTAINER KEY SET.\n\r\n\r", ch );
		    pObj->value[2] = atoi( argument );
		    break;
	    }
	    break;

	case ITEM_DRINK_CON:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_DRINK" );
		    return FALSE;
		case 0:
		    send_to_char( "容量設定完成.\n\r\n\r", ch );
            pObj->weight -= ( pObj->value[0] / 10 );
            pObj->cost -= pObj->value[0] * 5;
            pObj->value[0] = atoi( argument );
            pObj->weight += ( pObj->value[0] / 10 );
            pObj->cost += 1000 * 5;
            send_to_char( "盛裝量設定完成.\n\r\n\r", ch );
            pObj->value[1] = atoi(argument);
            if( pObj->value[0] < 0 ) {
                pObj->value[0] = 0;
                pObj->value[1] = 0;
                send_to_char("容量不能設定負值.\n\r", ch);
                break;
            }
            if( pObj->value[0] > 120 ) {
                pObj->value[0] = 120;
                pObj->value[1] = 120;
                send_to_char("容量最大值為 120.\n\r", ch);
                break;
            }
                    /*if( !IS_IMMORTAL(ch) ) {
                        if( pObj->value[0] < 0 ) {
                            pObj->value[0] = 0;
                            pObj->value[1] = 0;
                            send_to_char("You cannot set negative amount.\n\r", ch);
                            break;
                        }
                        if( pObj->value[0] > 200 ) {
                            pObj->value[0] = 200;
                            pObj->value[1] = 200;
                            send_to_char("Max capacity is 200.\n\r", ch);
                            break;
                        }
                        pObj->weight -= ( pObj->value[0] / 10 );
                        pObj->cost -= pObj->value[0] * 5;
                        pObj->value[0] = atoi( argument );
                        pObj->weight += ( pObj->value[0] / 10 );
                        pObj->cost += 1000 * 5;
                        send_to_char( "CURRENT AMOUNT OF LIQUID HOURS SET.\n\r\n\r", ch );
                        pObj->value[1] = atoi(argument);
                    }
                    else
                        pObj->value[0] = atoi( argument );
					*/
		    break;
		case 1:
		    send_to_char( "盛裝量設定完成.\n\r\n\r", ch );
		    pObj->value[1] = atoi( argument );
            if( pObj->value[1] > pObj->value[0] ) {
                send_to_char( "容量設定完成.\n\r\n\r", ch );
                pObj->weight -= ( pObj->value[0] / 10 );
                pObj->cost -= pObj->value[0] * 5;
                pObj->value[0] = atoi( argument );
                pObj->weight += ( pObj->value[0] / 10 );
                pObj->cost += pObj->value[0] * 5;
            }
            if( pObj->value[1] < 0 ) {
                pObj->value[0] = 0;
                pObj->value[1] = 0;
                send_to_char("盛裝量不能設定負值.\n\r", ch);
                break;
            }
            if( pObj->value[0] > 120 ) {
                pObj->value[0] = 120;
                pObj->value[1] = 120;
                send_to_char("盛裝量最大值為 120.\n\r", ch);
                break;
            }
                   /* if( !IS_IMMORTAL(ch) ) {
                        if( pObj->value[1] < 0 ) {
                            pObj->value[0] = 0;
                            pObj->value[1] = 0;
                            send_to_char("You cannot set negative amount.\n\r", ch);
                            break;
                        }
                        if( pObj->value[0] > 200 ) {
                            pObj->value[0] = 200;
                            pObj->value[1] = 200;
                            send_to_char("Max capacity is 200.\n\r", ch);
                            break;
                        }
                        if( pObj->value[1] > pObj->value[0] ) {
                            send_to_char( "MAXIMUM AMOUT OF LIQUID HOURS SET.\n\r\n\r", ch );
                            pObj->weight -= ( pObj->value[0] / 10 );
                            pObj->cost -= pObj->value[0] * 5;
                            pObj->value[0] = atoi( argument );
                            pObj->weight += ( pObj->value[0] / 10 );
                            pObj->cost += pObj->value[0] * 5;
                        }
                    }  */
		    break;
		case 2:
		    send_to_char( "種類設定完成.\n\r\n\r", ch );
		    pObj->value[2] = flag_value( liquid_flags, argument );
		    break;
		case 3:
		    send_to_char( "是否有毒設定完成.\n\r\n\r", ch );
		    pObj->value[3] = ( pObj->value[3] == 0 ) ? 1 : 0;
		    break;
	    }
	    break;

	case ITEM_FOOD:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_FOOD" );
		    return FALSE;
		case 0:
		    send_to_char( "飽足度設定完成.\n\r\n\r", ch );
		    pObj->value[0] = atoi( argument );
            if( pObj->value[0] < 0 ) {
                pObj->value[0] = 0;
                send_to_char("飽足度不能設定負值.\n\r", ch);
                break;
            }
            if( pObj->value[0] > 30 ) {
                pObj->value[0] = 30;
                send_to_char("最大飽足度為 30.\n\r", ch);
                break;
            }
		    break;
		case 3:
		    send_to_char( "是否有毒設定完成.\n\r\n\r", ch );
		    pObj->value[3] = ( pObj->value[3] == 0 ) ? 1 : 0;
		    break;
	    }
	    break;

	case ITEM_MONEY:
	    switch ( value_num )
	    {
		default:
		    do_help( ch, "ITEM_MONEY" );
		    return FALSE;
		case 0:
		    send_to_char( "GOLD AMOUNT SET.\n\r\n\r", ch );
		    pObj->value[0] = atoi( argument );
		    break;
	    }
	    break;
    }

    show_obj_values( ch, pObj );

    return TRUE;
}



bool oedit_show( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int cnt;

    EDIT_OBJ(ch, pObj);

    sprintf( buf, "英文名稱:	[%s]\n\r區域:	     [%5d] %s\n\r",
	pObj->name,
	!pObj->area ? -1	: pObj->area->vnum,
	!pObj->area ? "No Area" : pObj->area->name );
    send_to_char( buf, ch );


    sprintf( buf, "Vnum:	[%5d]\n\r類別:	      [%s]\n\r",
	pObj->vnum,
	flag_string( type_flags, pObj->item_type ) );
    send_to_char( buf, ch );

    sprintf( buf, "Wear flags:	[%s]\n\r",
	flag_string( wear_flags, pObj->wear_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Extra flags: [%s]\n\r",
	flag_string( extra_flags, pObj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "重量:	[%d]\n\r價格:	     [%d]\n\r",
	pObj->weight, pObj->cost );
    send_to_char( buf, ch );

    sprintf( buf, "材質:    [%s]\n\r等級:	[%d]\n\r",
	    pObj->material->name, pObj->level );
    send_to_char(buf, ch);

    if ( pObj->extra_descr )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Ex desc kwd: ", ch );

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( "[", ch );
	    send_to_char( ed->keyword, ch );
	    send_to_char( "]", ch );
	}

	send_to_char( "\n\r", ch );
    }

    sprintf( buf, "名稱:	%s\n\r長敘述:\n\r	 %s\n\r",
	pObj->short_descr, pObj->description );
    send_to_char( buf, ch );

    for ( cnt = 0, paf = pObj->affected; paf; paf = paf->next )
    {
	if ( paf->deleted )
	    continue;
	if ( cnt == 0 )
	{
	    send_to_char( "編 號   修正值   影 響\n\r", ch );
	    send_to_char( "------ -------- -------\n\r", ch );
	}
	sprintf( buf, "[%4d] %-8d %s\n\r", cnt,
	    paf->modifier,
	    flag_string( apply_flags, paf->location ) );
	send_to_char( buf, ch );
	cnt++;
    }

    show_obj_values( ch, pObj );

    return FALSE;
}


/*
 * Need to issue warning if flag isn't valid.
 */
bool oedit_addaffect( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AFFECT_DATA *pAf;
    char loc[MAX_STRING_LENGTH];
    char mod[MAX_STRING_LENGTH];

    EDIT_OBJ(ch, pObj);

    argument = one_argument( argument, loc );
    one_argument( argument, mod );

    if ( loc[0] == '\0' || mod[0] == '\0' || !is_number( mod ) )
    {
	send_to_char( "用法：	addaffect [location] [#mod]\n\r", ch );
	return FALSE;
    }

    /*if ( ( value = flag_value( affect_flags, argument ) ) == NO_FLAG )
    {
	send_to_char( "OEdit: Invalid value.  '? addaffect'\n\r", ch );
	return FALSE;
    }*/

    pAf		    =	new_affect();
    pAf->location   =	flag_value( apply_flags, loc );
    pAf->modifier   =	atoi( mod );
    pAf->type	    =	-1;
    pAf->duration   =	-1;
    pAf->bitvector  =	0;
    pAf->next	    =	pObj->affected;
    pObj->affected  =	pAf;

    send_to_char( "Affect added.\n\r", ch);
    return TRUE;
}



/*
 * My thanks to Hans Hvidsten Birkeland and Noam Krendel(Walker)
 * for really teaching me how to manipulate pointers.
 */
bool oedit_delaffect( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AFFECT_DATA *pAf;
    AFFECT_DATA *pAf_next;
    char affect[MAX_STRING_LENGTH];
    int	 value;
    int	 cnt = 0;

    EDIT_OBJ(ch, pObj);

    one_argument( argument, affect );

    if ( !is_number( affect ) || affect[0] == '\0' )
    {
	send_to_char( "用法：	delaffect [#affect]\n\r", ch );
	return FALSE;
    }

    value = atoi( affect );

    if ( value < 0 )
    {
	send_to_char( "Only non-negative affect-numbers allowed.\n\r", ch );
	return FALSE;
    }

    if ( !( pAf = pObj->affected ) )
    {
	send_to_char( "OEdit:  Non-existant affect.\n\r", ch );
	return FALSE;
    }

    if( value == 0 )	/* First case: Remove first affect */
    {
	pAf = pObj->affected;
	pObj->affected = pAf->next;
	free_affect( pAf );
    }
    else		/* Affect to remove is not the first */
    {
	while ( ( pAf_next = pAf->next ) && ( ++cnt < value ) )
	     pAf = pAf_next;

	if( pAf_next )		/* See if it's the next affect */
	{
	    pAf->next = pAf_next->next;
	    free_affect( pAf_next );
	}
	else				     /* Doesn't exist */
	{
	     send_to_char( "No such affect.\n\r", ch );
	     return FALSE;
	}
    }

    send_to_char( "Affect removed.\n\r", ch);
    return TRUE;
}


bool oedit_material( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    OBJ_INDEX_DATA *pMat;
    char arg[ MAX_INPUT_LENGTH];
    EDIT_OBJ(ch, pObj);

    one_argument(argument, arg);
    if( arg[0] == '\0' ){
	send_to_char("格式： material [材質名稱]\n\r", ch);
	return FALSE;
    }

    if( ! (pMat = material_lookup(arg))){
	send_to_char("沒有這個材質。\n\r", ch);
	return FALSE;
    }

    if( !IS_IMMORTAL(ch) ){
	if( pMat->item_type < 0 || pMat->item_type > ch->clan->cclass ) {
	    send_to_char("你無法使用這個材質。\n\r", ch);
	    return FALSE;
	}
	if( !check_cost(ch, pMat->wear_flags)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
	pObj->cost += pMat->wear_flags;
    }

    pObj->material = pMat;
    send_to_char("材質設定成功\。\n\r", ch);
    return TRUE;


}

bool oedit_name( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "格式:	name [字串]\n\r", ch );
	return FALSE;
    }

    if( !IS_IMMORTAL(ch) ){
	if( !check_cost(ch, COST_SET_OBJNAME)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
    }

    free_string( pObj->name );
    pObj->name = str_dup( argument );

    send_to_char( "英文名稱設定完成.\n\r", ch);
    return TRUE;
}



/*bool oedit_cname( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	cname [string]\n\r", ch );
	return FALSE;
    }

    free_string( pObj->cname );
    pObj->cname = str_dup( argument );

    send_to_char( "Chinese Name set.\n\r", ch);
    return TRUE;
}*/



bool oedit_short( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "格式:	short [字串]\n\r", ch );
	return FALSE;
    }

    if( !IS_IMMORTAL(ch) ){
	if( !check_cost(ch, COST_SET_OBJSHORT)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
    }
    free_string( pObj->short_descr );
    pObj->short_descr = str_dup( argument );
    pObj->short_descr[0] = LOWER( pObj->short_descr[0] );

    send_to_char( "名稱設定完成.\n\r", ch);
    return TRUE;
}



bool oedit_long( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "格式:	long [string]\n\r", ch );
	return FALSE;
    }
    if( !IS_IMMORTAL(ch) ){
	if( !check_cost(ch, COST_SET_OBJLONG)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
    }

    free_string( pObj->description );
    pObj->description = str_dup( argument );
    pObj->description[0] = UPPER( pObj->description[0] );

    send_to_char( "長敘述設定完成.\n\r", ch);
    return TRUE;
}



bool set_value( CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, char *argument, int value )
{
    if ( argument[0] == '\0' )
    {
	set_obj_values( ch, pObj, -1, '\0' );
	return FALSE;
    }

    if ( set_obj_values( ch, pObj, value, argument ) )
	return TRUE;

    return FALSE;
}



/*****************************************************************************
 Name:		oedit_values
 Purpose:	Finds the object and sets its value.
 Called by:	The four valueX functions below.
 ****************************************************************************/
bool oedit_values( CHAR_DATA *ch, char *argument, int value )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( set_value( ch, pObj, argument, value ) )
	return TRUE;

    return FALSE;
}

bool oedit_level( CHAR_DATA *ch, char *argument )
{
    int level;
    int pcost;
    int cost;
    OBJ_INDEX_DATA *pObj;
    char arg [MAX_INPUT_LENGTH];

    EDIT_OBJ(ch, pObj);

    one_argument(argument, arg);
    if( arg[0] == '\0'){
	send_to_char("格式： level [等級]\n\r", ch);
	return FALSE;
    }
    level = atoi(arg);
    if( level < 0 ) {
	send_to_char("等級必須至少為 0！\n\r", ch);
	return FALSE;
    }
    if( !IS_IMMORTAL(ch)){
    if ( abs( level - 20 ) > ch->clan->cclass )
    {
    	send_to_char("幫派目前的技術等級還不夠。\n\r", ch);
    	return FALSE;
    }
    pcost = UMAX( 1, ( level - 20 ) );
	cost = pcost * pcost * 400;

	if( !check_cost(ch, cost)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
	    pObj->cost += pcost * 1000;
    }
    pObj->level = level;
    send_to_char("等級設定成功\.\n\r", ch);

    return TRUE;
}

bool oedit_extra( CHAR_DATA *ch, char *argument )
{
    char arg [ MAX_INPUT_LENGTH];
    int value;
    int cost;
    OBJ_INDEX_DATA *pObj;
    
    EDIT_OBJ(ch, pObj);

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	send_to_char("格式： extra <extra-flags>，請用 ? extra 取得說明。\n\r", ch);
	return FALSE;
    }


    if ( ( value = flag_value( extra_flags, arg ) ) != NO_FLAG )
    {
	if( !IS_IMMORTAL(ch) ){
	    cost = flag_cost( extra_cost, arg);
	    if( cost < 0 ) {
		send_to_char("沒有這個 extra flag，或是你不能設定。\n\r", ch);
		return FALSE;
	    }
/*	    if( IS_SET(pObj->extra_flags, value)) cost *= -1;
	    if( !check_cost(ch, cost)){
		send_to_char("你的幫費不夠了。\n\r", ch);
		return FALSE;
		}
*/
		if ( !check_facility( ch->in_room->area, ROOM_LIBRARY ) )
		{
			send_to_char( "沒有大圖書館，幫會無法附加裝備魔法屬性！\n\r", ch );
			return FALSE;
		}
		if ( !check_clco( extra_cost, arg, ch ) ) return FALSE;
	    pObj->cost += cost;
	}
	TOGGLE_BIT(pObj->extra_flags, value);

	send_to_char( "Extra flag toggled.\n\r", ch);
	return TRUE;
    }
    return FALSE;
}

bool oedit_type( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int value;
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	send_to_char("格式： type <item-type>，請用 ? type取得說明。\n\r", ch);
	return FALSE;
    }

    if ( ( value = flag_value( type_flags, arg ) ) != NO_FLAG )
    {
	if( !IS_IMMORTAL(ch)){
	    int cost = flag_cost(type_cost, arg);
	
	    if( ch->clan->cclass < flag_class( type_cost, arg ) ) {
		send_to_char("幫會的技藝等級不足以製造這個類別。\n\r", ch);
		return FALSE;
	    }
	    if( !check_cost(ch, cost)){
		send_to_char("你的幫費不夠了。\n\r", ch);
		return FALSE;
	    }
	    pObj->cost += cost;
	}
	pObj->item_type = value;

	send_to_char( "類別設定完成.\n\r", ch );
	switch( value ) {
	    case ITEM_FOUNTAIN:
		pObj->weight = 1000;
		REMOVE_BIT(pObj->wear_flags, ITEM_TAKE);
		break;
	    case ITEM_WEAPON:
		pObj->wear_flags = ITEM_TAKE;
		break;
            case ITEM_CONTAINER:
            case ITEM_DRINK_CON:
                if( ch->clan->cclass < 15 )
                    pObj->weight = 50;
                else if ( ch->clan->cclass < 22 )
                    pObj->weight = 30;
                else if ( ch->clan->cclass < 30 )
                    pObj->weight = 15;
                else if( ch->clan->cclass < 40 )
                    pObj->weight = 10;
                else pObj->weight = 5;
                pObj->value[0] = 5;
                break;
	    default:
		pObj->weight = 10;
		break;
	};

	/*
	* Clear the values.
	 */
	pObj->value[0] = 0;
	pObj->value[1] = 0;
	pObj->value[2] = 0;
	pObj->value[3] = 0;

	return TRUE;
    }
    return FALSE;
}

bool oedit_wear( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int value;
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	send_to_char("格式： wear <wear flags>，請用 ? wear取得說明。\n\r", ch);
	return FALSE;
    }
    if ( ( value = flag_value( wear_flags, arg ) ) != NO_FLAG )
    {

	if( !IS_IMMORTAL(ch) && str_cmp(arg, "take")){
            int cost = flag_cost( wear_cost, arg);
            if( pObj->item_type == ITEM_WEAPON ) {
		if( str_cmp( arg, "wield")
		&& str_cmp(arg, "twohanded")){
		    send_to_char("武器類只能設定wield 或twohanded\n\r", ch);
		    return FALSE;
		} 

	    }
	    if( cost < 0 ) {
		send_to_char("沒有這個 wear flag，或是你不能設定。\n\r", ch);
		return FALSE;
		}
		if ( pObj->wear_flags > 1 )
		{
			send_to_char("只有神族才能創造多重部位穿著裝備。\n\r", ch );
			return FALSE;
		}
	    if( IS_SET(pObj->wear_flags, value)) cost *= -1;
	    if( !check_cost(ch, cost)){
		send_to_char("你的幫費不夠了。\n\r", ch);
		return FALSE;
	    }

	    pObj->cost += cost;
	}
	TOGGLE_BIT(pObj->wear_flags, value);

	send_to_char( "Wear flag toggled.\n\r", ch);
	return TRUE;
    }
    return FALSE;
}



bool oedit_value0( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 0 ) )
	return TRUE;

    return FALSE;
}



bool oedit_value1( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 1 ) )
	return TRUE;

    return FALSE;
}



bool oedit_value2( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 2 ) )
	return TRUE;

    return FALSE;
}



bool oedit_value3( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 3 ) )
	return TRUE;

    return FALSE;
}



bool oedit_weight( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int weight;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "用法：	weight [number]\n\r", ch );
	return FALSE;
    }

    weight = atoi( argument );
    if( !IS_IMMORTAL(ch)){
	if( pObj->item_type == ITEM_FOUNTAIN
        ||  pObj->item_type == ITEM_CONTAINER
        ||  pObj->item_type == ITEM_DRINK_CON ) {
	    send_to_char("這樣東西不能改重量。\n\r", ch);
	    return FALSE;
	}
	if( weight < 10 || weight > 1000 ) {
	    send_to_char("重量的範圍是 10 到 1000。\n\r", ch);
	    return FALSE;
	}
    }

    if ( weight < 0 )
    {
	send_to_char( "You may not set negative weight.\n\r", ch );
	return FALSE;
    }

    pObj->weight = weight;

    send_to_char( "重量設定完成.\n\r", ch);
    return TRUE;
}

bool pploedit_create( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AREA_DATA *pArea;
    int	 value;
    int	 iHash;

    if( !ch->in_room || !ch->in_room->area ) return FALSE;

    for( value = ch->in_room->area->lvnum; value <= ch->in_room->area->uvnum; ++value){
	if( !get_obj_index(value))break;
    }
    if( value > ch->in_room->area->uvnum){
	send_to_char("你沒有多餘的 vnum 了，請向大神申請。\n\r", ch);
	return FALSE;
    }
    

    /* OLC 1.1b */

    pArea = get_vnum_area( value );
    if ( !pArea )
    {
	send_to_char( "OEdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }
    SET_BIT( pArea->area_flags, AREA_CHANGED );

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
    {
	send_to_char( "OEdit:  你無法在這個區域建立物品.\n\r", ch );
	return FALSE;
    }

    if ( get_obj_index( value ) )
    {
	send_to_char( "OEdit:  已經有這個 Vnum 了.\n\r", ch );
	return FALSE;
    }

    if( !IS_IMMORTAL(ch) && !check_cost(ch, COST_CREATE_OBJ)){
	send_to_char("你的幫費不夠了。\n\r", ch);
	return FALSE;
    }
	
    pObj			= new_obj_index();
    pObj->vnum			= value;
    pObj->area			= pArea;
    pObj->level			= 20;
    pObj->weight		= 10;
    pObj->cost			= 100;
    pObj->material		= material_lookup("Generic");
	
    if ( value > top_vnum_obj )
	top_vnum_obj = value;

    iHash			= value % MAX_KEY_HASH;
    pObj->next			= obj_index_hash[iHash];
    obj_index_hash[iHash]	= pObj;
    ch->desc->pEdit		= (void *)pObj;
    SET_BIT(pObj->extra_flags, ITEM_CLAN);

    send_to_char( "物品建立完成.\n\r", ch );
    return TRUE;
}

bool oedit_oload( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA	   *obj;
    OBJ_INDEX_DATA *pObjIndex;
    AREA_DATA *pArea;
    char	    arg1 [ MAX_INPUT_LENGTH ];
    int		    level;


    argument = one_argument( argument, arg1 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char( "格式: oload <vnum> .\n\r", ch );
	return FALSE;
    }
 

    if ( !( pObjIndex = get_obj_index( atoi( arg1 ) ) ) )
    {
	send_to_char( "沒有這個 vnum 的物品.\n\r", ch );
	return FALSE;
    }

    pArea = get_vnum_area( atoi(arg1));
    if( !pArea ||( !IS_BUILDER(ch, pArea) &&!IS_PPLBUILDER(ch, pArea))){
	send_to_char("你無法創造這樣物品。\n\r", ch);
	return FALSE;
    }

    if( !IS_IMMORTAL(ch) ){
	int cost = UMAX( pObjIndex->cost , COST_OLOAD_OBJ);
	if( !check_cost(ch, cost)){
	    send_to_char("你的幫費不夠了。\n\r", ch);
	    return FALSE;
	}
    }

    if( pObjIndex->level == 0 ) level = 35;

    level = number_fuzzy( pObjIndex->level );

    obj = create_object( pObjIndex, level );
    if( !IS_IMMORTAL(ch) ) {
        switch( obj->item_type ) {
            case ITEM_WEAPON:
                obj->value[1] = UMIN( obj->value[1], ch->clan->cclass / 2+1);
                obj->value[2] = UMIN( obj->value[2], ch->clan->cclass + ch->clan->cclass / 4 );
                break;
            default:
                break;
        };
    }
    if ( CAN_WEAR( obj, ITEM_TAKE ) )
    {
	obj_to_char( obj, ch );
    }
    else
    {
	obj_to_room( obj, ch->in_room );
	act( "$n 伸手一指，變出了 $p。", ch, obj, NULL, TO_ROOM );
    }
    send_to_char( "Ok.\n\r", ch );
    return FALSE;
}

bool oedit_cost( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int cost;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "用法：	cost [number]\n\r", ch );
	return FALSE;
    }

    cost = atoi( argument );

    if( !IS_IMMORTAL(ch) ) {
        if( pObj->item_type == ITEM_CONTAINER
        ||  pObj->item_type == ITEM_DRINK_CON ){
            send_to_char("You may not set its cost.\n\r", ch);
            return FALSE;
        }
    }

    if ( cost < 0 )
    {
	send_to_char( "You may not set negative cost.\n\r", ch );
	return FALSE;
    }

    pObj->cost = cost;

    send_to_char( "Cost set.\n\r", ch);
    return TRUE;
}



bool oedit_create( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AREA_DATA *pArea;
    int	 value = 0;
    int	 iHash;
    char arg[MAX_INPUT_LENGTH];
    char output[BUF_STRING_LENGTH];

    if( !IS_IMMORTAL(ch)) return pploedit_create(ch, argument);

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	sprintf( output, "用法：  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }
    value = atoi( arg );

    /* OLC 1.1b */
    if ( arg[0] == '\0' || value <= 0 || value >= INT_MAX )
    {

	sprintf( output, "用法：  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );
    if ( !pArea )
    {
	send_to_char( "OEdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	send_to_char( "OEdit:  Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if ( get_obj_index( value ) )
    {
	send_to_char( "OEdit:  Object vnum already exists.\n\r", ch );
	return FALSE;
    }
	
    pObj			= new_obj_index();
    pObj->vnum			= value;
    pObj->area			= pArea;
    pObj->level			= 0;
    pObj->material		= material_lookup("Generic");
	
    if ( value > top_vnum_obj )
	top_vnum_obj = value;

    iHash			= value % MAX_KEY_HASH;
    pObj->next			= obj_index_hash[iHash];
    obj_index_hash[iHash]	= pObj;
    ch->desc->pEdit		= (void *)pObj;

    send_to_char( "Object Created.\n\r", ch );
    return TRUE;
}



bool oedit_ed( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    EXTRA_DESCR_DATA *ed;
    char command[MAX_INPUT_LENGTH];
    char keyword[MAX_INPUT_LENGTH];

    EDIT_OBJ(ch, pObj);

    argument = one_argument( argument, command );
    one_argument( argument, keyword );

    if ( command[0] == '\0' )
    {
	send_to_char( "格式:	ed add [keyword]\n\r", ch );
	send_to_char( "		ed delete [keyword]\n\r", ch );
	send_to_char( "		ed edit [keyword]\n\r", ch );
	send_to_char( "		ed format [keyword]\n\r", ch );
	return FALSE;
    }

    if ( !str_cmp( command, "add" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed add [keyword]\n\r", ch );
	    return FALSE;
	}
	if( !IS_IMMORTAL(ch) ){
	    if( !check_cost(ch, COST_SET_OBJED)){
		send_to_char("你的幫費不夠了。\n\r", ch);
		return FALSE;
	    }
	}

	ed		    =	new_extra_descr();
	ed->keyword	    =	str_dup( keyword );
	ed->next	    =	pObj->extra_descr;
	pObj->extra_descr   =	ed;

	string_append( ch, &ed->description );

	return TRUE;
    }

    if ( !str_cmp( command, "edit" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed edit [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	}

	if ( !ed )
	{
	    send_to_char( "OEdit:   無此關鍵字.\n\r", ch );
	    return FALSE;
	}

	string_append( ch, &ed->description );

	return TRUE;
    }

    if ( !str_cmp( command, "delete" ) )
    {
	EXTRA_DESCR_DATA *ped = NULL;

	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed delete [keyword]\n\r", ch );
	    return FALSE;
	}

	if( !IS_IMMORTAL(ch) ){
	    if( !check_cost(ch, COST_DEL_OBJED)){
		send_to_char("你的幫費不夠了。\n\r", ch);
		return FALSE;
	    }
	}
	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	    ped = ed;
	}

	if ( !ed )
	{
	    send_to_char( "OEdit:  無此關鍵字.\n\r", ch );
	    return FALSE;
	}

	if ( !ped )
	    pObj->extra_descr = ed->next;
	else
	    ped->next = ed->next;

	free_extra_descr( ed );

	send_to_char( "刪除成功\.\n\r", ch );
	return TRUE;
    }


    if ( !str_cmp( command, "format" ) )
    {
	EXTRA_DESCR_DATA *ped = NULL;

	if ( keyword[0] == '\0' )
	{
	    send_to_char( "格式:  ed format [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	    ped = ed;
	}

	if ( !ed )
	{
		send_to_char( "OEdit:  無此關鍵字.\n\r", ch );
		return FALSE;
	}

	/* OLC 1.1b */
	if ( strlen(ed->description) >= (MAX_STRING_LENGTH - 4) )
	{
	    send_to_char( "字串過長.\n\r", ch );
	    return FALSE;
	}

	ed->description = format_string( ed->description );

	send_to_char( "格式化完成.\n\r", ch );
	return TRUE;
    }

    oedit_ed( ch, "" );
    return FALSE;
}



/*
 * Mobile Editor Functions.
 */
bool medit_show( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    char buf[MAX_STRING_LENGTH];

    EDIT_MOB(ch, pMob);

    sprintf( buf, "Name:	[%s]\n\rArea:	     [%5d] %s\n\r",
	pMob->player_name,
	!pMob->area ? -1	: pMob->area->vnum,
	!pMob->area ? "No Area" : pMob->area->name );
    send_to_char( buf, ch );

    sprintf( buf, "Act:		[%s]\n\r",
	flag_string( act_flags, pMob->act ) );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum:	[%5d]\n\rSex:	      [%s]\n\r",
	pMob->vnum,
	pMob->sex == SEX_MALE	 ? "male"   :
	pMob->sex == SEX_FEMALE	 ? "female" : "neutral" );
    send_to_char( buf, ch );

    sprintf( buf, "Race:	[%s]\n\r", race_table[ pMob->race ].name );
    send_to_char( buf, ch );

    sprintf( buf,
	"Level:	      [%2d]\n\rAlign:	    [%4d]\n\r",
	pMob->level,	   pMob->alignment );
    send_to_char( buf, ch );

    sprintf( buf, "Affected by: [%s]\n\r",
	flag_string( affect_flags, pMob->affected_by ) );
    send_to_char( buf, ch );

    if ( pMob->spec_fun )
    {
	sprintf( buf, "Spec fun:    [%s]\n\r",	spec_string( pMob->spec_fun ) );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Short descr: %s\n\rLong descr:\n\r%s",
	pMob->short_descr,
	pMob->long_descr );
    send_to_char( buf, ch );

    sprintf( buf, "Description:\n\r%s", pMob->description );
    send_to_char( buf, ch );

    if ( pMob->pShop )
    {
	SHOP_DATA *pShop;
	int iTrade;

	pShop = pMob->pShop;

	sprintf( buf,
	  "Shop data for [%5d]:\n\r"
	  "  Markup for purchaser: %d%%\n\r"
	  "  Markdown for seller:  %d%%\n\r",
	    pShop->keeper, pShop->profit_buy, pShop->profit_sell );
	send_to_char( buf, ch );
	sprintf( buf, "	 Hours: %d to %d.\n\r",
	    pShop->open_hour, pShop->close_hour );
	send_to_char( buf, ch );

	for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
	{
	    if ( pShop->buy_type[iTrade] != 0 )
	    {
		if ( iTrade == 0 ) {
		    send_to_char( "  Number Trades Type\n\r", ch );
		    send_to_char( "  ------ -----------\n\r", ch );
		}
		sprintf( buf, "	 [%4d] %s\n\r", iTrade,
		    flag_string( type_flags, pShop->buy_type[iTrade] ) );
		send_to_char( buf, ch );
	    }
	}
    }

    return FALSE;
}

bool pplmedit_create( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    int value;
    int iHash;
    

    for( value = ch->in_room->area->lvnum; value <= ch->in_room->area->uvnum; ++value)
    {
	if( !get_mob_index(value)) break;
    }
    if( value > ch->in_room->area->uvnum ) {
	send_to_char("你沒有足夠的編號再建新房間了，請向大神申請。\n\r", ch);
	return FALSE;
    }

    if( !check_cost(ch, COST_CREATE_MOB)){
	send_to_char("你的幫費不夠了。 \n\r", ch);
	return FALSE;
    }

    pMob			= new_mob_index();
    pMob->vnum			= value;
    pMob->area			= ch->in_room->area;
	
    if ( value > top_vnum_mob )
	top_vnum_mob = value;	     

    pMob->act			= ACT_IS_NPC + ACT_SENTINEL + ACT_STAY_AREA;
    iHash			= value % MAX_KEY_HASH;
    pMob->next			= mob_index_hash[iHash];
    mob_index_hash[iHash]	= pMob;
    ch->desc->pEdit		= (void *)pMob;

    send_to_char( "招募前置作業完成，請繼續設定其他要求。\n\r", ch );
    return TRUE;
}

bool medit_create( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    AREA_DATA *pArea;
    int	 value = 0;
    int	 iHash;
    char arg[MAX_INPUT_LENGTH];
	char output[MAX_STRING_LENGTH];

	if ( !IS_IMMORTAL( ch ) ) return pplmedit_create( ch, argument );
    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
	sprintf( output, "用法：  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }
    if( argument[0] == '\0'){
	int i;
	pArea = ch->in_room->area;
	value = 0;
	for( i = pArea->lvnum; i <= pArea->uvnum; ++i ){
	    if( !get_mob_index(i) ) {
		value = i;
		break;
	    }
	}
    }

    /* OLC 1.1b */
    else if ( value <= 0 || value >= INT_MAX )
    {

	sprintf( output, "用法：  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );

    if ( !pArea )
    {
	send_to_char( "MEdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	send_to_char( "MEdit:  Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if ( get_mob_index( value ) )
    {
	send_to_char( "MEdit:  Mobile vnum already exists.\n\r", ch );
	return FALSE;
    }

    pMob			= new_mob_index();
    pMob->vnum			= value;
    pMob->area			= pArea;
	
    if ( value > top_vnum_mob )
	top_vnum_mob = value;	     

    pMob->act			= ACT_IS_NPC | ACT_SENTINEL | ACT_STAY_AREA;
    iHash			= value % MAX_KEY_HASH;
    pMob->next			= mob_index_hash[iHash];
    mob_index_hash[iHash]	= pMob;
    ch->desc->pEdit		= (void *)pMob;

    send_to_char( "Mobile Created.\n\r", ch );
    return TRUE;
}



bool medit_spec( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	spec [special function]\n\r", ch );
	send_to_char( "取消所有特殊能力：	spec none\n\r", ch );
	return FALSE;
    }


    if ( !str_cmp( argument, "none" ) )
    {
	pMob->spec_fun = NULL;

	send_to_char( "Spec removed.\n\r", ch);
	return TRUE;
    }

	if ( !check_clco( spec_cost, argument, ch ) )
		return FALSE;

	if ( !str_prefix( "spec_cast", argument ) ||
		 !str_prefix( "spec_breath", argument ) )
		if ( !IS_SET( pMob->class, 2 ) )
		{
			send_to_char( "這個生物的職業並非法師，無法施法。\n\r", ch );
			return FALSE;
		}
	
	if ( !strcmp( "spec_thief", argument ) ||
		 !strcmp( "spec_assassin", argument ) )
		if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_MISC_RESEARCH ) )
		{
			send_to_char( "沒有研究廳，無法召募殺手、弓箭手及神偷。\n\r", ch );
			return FALSE;
		}

    if ( spec_lookup( argument ) )
    {
	pMob->spec_fun = spec_lookup( argument );
	send_to_char( "Spec set.\n\r", ch);
	return TRUE;
    }

    send_to_char( "MEdit: No such special function.\n\r", ch );
    return FALSE;
}


bool medit_class( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	class [class name]\n\r", ch );
	send_to_char( "取消所有職業：	class none\n\r", ch );
	return FALSE;
    }


    if ( !str_cmp( argument, "none" ) )
    {
	pMob->class = '\0'; //NULL;

	send_to_char( "Class removed.\n\r", ch);
	return TRUE;
    }

	if ( !check_clco( class_cost, argument, ch ) )
		return FALSE;

	if ( !strcmp( "warrior", argument ) )
	{
		if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_MIL_DEPARTMENT ) )
		{
			send_to_char( "沒有軍事部門，無法召募戰士系生物。\n\r", ch );
			return FALSE;
		}
		pMob->class += 1;
	}
	else if ( !strcmp( "mage", argument ) )
	{
		if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_MAGIC_TOWER ) )
		{
			send_to_char( "沒有魔法塔，無法召募法師系生物。\n\r", ch );
			return FALSE;
		}
		pMob->class += 2;
	}
	else if ( !strcmp( "thief", argument ) )
	{
		if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_MISC_RESEARCH ) )
		{
			send_to_char( "沒有研究廳，無法召募神偷。\n\r", ch );
			return FALSE;
		}
		pMob->class += 4;
	}
	else if ( !strcmp( "swordsman", argument ) )
	{
		if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_MIL_DEPARTMENT ) )
		{
			send_to_char( "沒有軍事部門，無法召募戰士系生物。\n\r", ch );
			return FALSE;
		}
		pMob->class += 8;
	}
	else if ( !strcmp( "ranger", argument ) )
	{
		if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_MISC_RESEARCH ) )
		{
			send_to_char( "沒有研究廳，無法召募弓箭手。\n\r", ch );
			return FALSE;
		}
		pMob->class += 16;
	}
	else
	{
    	send_to_char( "MEdit: 沒這門職業。\n\r", ch );
    	return FALSE;
    }
    send_to_char( "MEdit: 職業設定完成。\n\r", ch );
    return TRUE;
}

bool medit_align( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "用法：	alignment [number]\n\r", ch );
	return FALSE;
    }

    pMob->alignment = atoi( argument );

    send_to_char( "Alignment set.\n\r", ch);
    return TRUE;
}

bool medit_sex( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	sex [male/female/neutral]\n\r", ch );
	return FALSE;
    }

	if ( !strcmp( "male", argument ) )
		pMob->sex = SEX_MALE;
	else if ( !strcmp( "female", argument ) )
		pMob->sex = SEX_FEMALE;
	else if ( !strcmp( "neutral", argument ) )
		pMob->sex = SEX_NEUTRAL;
	else
	{
		send_to_char( "用法：	sex [male/female/neutral]\n\r", ch );
		return FALSE;
    }
    send_to_char( "MEdit: 性別設定完成。\n\r", ch );
    return TRUE;
}

bool medit_race( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
	int race;
	
    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	race [race name]\n\r", ch );
	return FALSE;
    }

	if ( !IS_IMMORTAL( ch ) && !check_facility( ch->in_room->area, ROOM_TAVERN ) )
	{
		send_to_char( "沒有酒店，召募不到非人類種族。\n\r", ch );
		return FALSE;
	}

    for (race = 0; *race_flags[race].name; race++)	/* OLC 1.1b */
		if ( !str_cmp( argument, race_flags[race].name ) )
	    	break;

	if ( !(*race_flags[race].name) )
	{
		send_to_char( "找不到這個種族。\n\r", ch );
		return FALSE;
	}
	if ( !IS_IMMORTAL( ch ) && !race_flags[race].settable )
	{
		send_to_char( "只有天神才能召募這個種族。\n\r", ch );
		return FALSE;
	}

	pMob->race = URANGE( 0, race_lookup( argument ), MAX_RACE - 1 );

    send_to_char( "MEdit: 種族設定完成。\n\r", ch );
    return TRUE;
}

bool medit_level( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
	int level;
	char	buf [MAX_STRING_LENGTH];
		
    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "用法：	level [number]\n\r", ch );
	return FALSE;
    }
	if ( !IS_IMMORTAL( ch ) )
	{
		if ( ( level = atoi( argument ) ) > 18 + ch->clan->cclass )
		{
			sprintf( buf, "組織的發展狀況目前只能夠招募到等級%d的生物\n\r",
				18 + ch->clan->cclass );
			send_to_char ( buf, ch );
			return FALSE;
		}
		else if ( level > 45 )
		{
			send_to_char( "只有天神才能創造這麼強的生物。\n\r", ch );
			return FALSE;
		}
	}
	
    pMob->level = atoi( argument );

    send_to_char( "Level set.\n\r", ch);
    return TRUE;
}



bool medit_desc( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	string_append( ch, &pMob->description );
	return TRUE;
    }

    send_to_char( "用法：  desc    - line edit\n\r", ch );
    return FALSE;
}




bool medit_long( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	long [string]\n\r", ch );
	return FALSE;
    }

    free_string( pMob->long_descr );
    strcat( argument, "\n\r" );
    pMob->long_descr = str_dup( argument );
    pMob->long_descr[0] = UPPER( pMob->long_descr[0]  );

    send_to_char( "Long description set.\n\r", ch);
    return TRUE;
}



bool medit_short( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	short [string]\n\r", ch );
	return FALSE;
    }

    free_string( pMob->short_descr );
    pMob->short_descr = str_dup( argument );

    send_to_char( "Short description set.\n\r", ch);
    return TRUE;
}



bool medit_name( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
	char	buf[MAX_STRING_LENGTH];
	
    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	name [string]\n\r", ch );
	return FALSE;
    }

	strcpy( buf, argument );
	if ( !IS_IMMORTAL( ch ) )
	{
		if ( !check_parse_name( argument ) )
		{
	    	send_to_char( "那是不合法的名字，請重新輸入。\n\r", ch );
	    	return FALSE;
		}
		strcat( buf, "-cn" );
	}
    free_string( pMob->player_name );
    pMob->player_name = str_dup( buf );

    send_to_char( "Name set.\n\r", ch);
    return TRUE;
}




/*bool medit_cname( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	cname [string]\n\r", ch );
	return FALSE;
    }

    free_string( pMob->cname );
    pMob->cname = str_dup( argument );

    send_to_char( "Chinese Name set.\n\r", ch);
    return TRUE;
}*/




bool medit_shop( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    char command[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];

    argument = one_argument( argument, command );
    argument = one_argument( argument, arg1 );

    EDIT_MOB(ch, pMob);

    if ( command[0] == '\0' )
    {
	send_to_char( "用法：	shop hours [#opening] [#closing]\n\r", ch );
	send_to_char( "		shop profit [#buying%] [#selling%]\n\r", ch );
	send_to_char( "		shop type [#0-4] [item type]\n\r", ch );
	send_to_char( "		shop delete [#0-4]\n\r", ch );
	return FALSE;
    }

	if ( !IS_IMMORTAL( ch ) && ch->clan->cclass < 18 )
	{
		send_to_char( "組織發展等級要到 17 級以後才能建立商店。\n\r", ch );
		return FALSE;
	}
    if ( !str_cmp( command, "hours" ) )
    {
	if ( arg1[0] == '\0' || !is_number( arg1 )
	|| argument[0] == '\0' || !is_number( argument ) )
	{
	    send_to_char( "用法：  shop hours [#opening] [#closing]\n\r", ch );
	    return FALSE;
	}

	if ( !pMob->pShop )
	{
	    pMob->pShop		= new_shop();
	    pMob->pShop->keeper = pMob->vnum;
	    shop_last->next	= pMob->pShop;
	}

	pMob->pShop->open_hour = atoi( arg1 );
	pMob->pShop->close_hour = atoi( argument );

	send_to_char( "Shop hours set.\n\r", ch);
	return TRUE;
    }


    if ( !str_cmp( command, "profit" ) )
    {
	if ( arg1[0] == '\0' || !is_number( arg1 )
	|| argument[0] == '\0' || !is_number( argument ) )
	{
	    send_to_char( "用法：  shop profit [#buying%] [#selling%]\n\r", ch );
	    return FALSE;
	}

	if ( !pMob->pShop )
	{
	    pMob->pShop		= new_shop();
	    pMob->pShop->keeper = pMob->vnum;
	    shop_last->next	= pMob->pShop;
	}

	pMob->pShop->profit_buy	    = atoi( arg1 );
	pMob->pShop->profit_sell    = atoi( argument );

	send_to_char( "Shop profit set.\n\r", ch);
	return TRUE;
    }


    if ( !str_cmp( command, "type" ) )
    {
	char buf[MAX_INPUT_LENGTH];
	int value;

	if ( arg1[0] == '\0' || !is_number( arg1 )
	|| argument[0] == '\0' )
	{
	    send_to_char( "用法：  shop type [#0-4] [item type]\n\r", ch );
	    return FALSE;
	}

	if ( atoi( arg1 ) >= MAX_TRADE )
	{
	    sprintf( buf, "REdit:  May sell %d items max.\n\r", MAX_TRADE );
	    send_to_char( buf, ch );
	    return FALSE;
	}

	if ( ( value = flag_value( type_flags, argument ) ) == NO_FLAG )
	{
	    send_to_char( "REdit:  That type of item is not known.\n\r", ch );
	    return FALSE;
	}

	if ( !pMob->pShop )
	{
	    pMob->pShop		= new_shop();
	    pMob->pShop->keeper = pMob->vnum;
	    shop_last->next	= pMob->pShop;
	}

	pMob->pShop->buy_type[atoi( arg1 )] = value;

	send_to_char( "Shop type set.\n\r", ch);
	return TRUE;
    }


    if ( !str_cmp( command, "delete" ) )
    {
	SHOP_DATA *pShop;

	if (!pMob->pShop)
	{
	    send_to_char ("REdit:  Non-existant shop.\n\r", ch);
	    return FALSE;
	}

	pShop = pMob->pShop;
	free_shop (pShop);
	pMob->pShop = NULL;

	send_to_char ("Shop deleted.\n\r", ch);
	return TRUE;
    }

    medit_shop( ch, "" );
    return FALSE;
}

/*
 * MobProg Editor Functions.
 */
 MPEDIT( mpedit_show )
 {
    MOB_INDEX_DATA *pMob;
    MPROG_DATA* pMobProg;
    char buf[MAX_STRING_LENGTH];

    EDIT_MOB(ch, pMob);
    pMobProg = edit_mprog( ch, pMob );

    if ( argument[0] == '\0' )	/* Show current mobprog */
	show_mprog( ch, pMobProg );
    else if( is_number( argument ) )	/* show a specific mobprog */
    {
	MPROG_DATA* mprg;
	int prg = atoi( argument );
	int cnt = mprog_count( pMob );

	if( prg < 1 || prg > cnt )
	{
	    sprintf( buf, "MPEdit:  Valid range is 1 to %d.\n\r", cnt );
	    send_to_char( buf, ch );
	    return FALSE;
	}
	for( mprg = pMob->mobprogs; mprg && prg-- > 1; mprg = mprg->next )
	    ;
	show_mprog( ch, mprg );
    }
    else if( !str_cmp( argument, "all" ) )
    {
	for( pMobProg = pMob->mobprogs; pMobProg;
			pMobProg = pMobProg->next )
	    show_mprog( ch, pMobProg );
	send_to_char( "|\n\r", ch );
    }
    else
	send_to_char( "用法： show [all]\n\r", ch );

    return FALSE;
}


MPEDIT( mpedit_create )
{
    MOB_INDEX_DATA *pMob;
    AREA_DATA *pArea;
    MPROG_DATA* mprg;
    int	 value;

    value = atoi( argument );
    if ( argument[0] == '\0' || value == 0 )
    {
	send_to_char( "用法：	mpedit create vnum [svnum]\n\r", ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );

    if ( !pArea )
    {
	send_to_char( "MPEdit:	That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	send_to_char( "MPEdit:	Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if( !( pMob = get_mob_index( value ) ) )
    {
	send_to_char( "MPEdit:	Mobile vnum does not exist!\n\r", ch );
	return FALSE;
    }

    if( pMob->mobprogs )
    {
	send_to_char( "MPEdit:	Mobile already has mob programs!\n\r", ch );
	return FALSE;
    }

    pMob->mobprogs = (MPROG_DATA *) alloc_mem( sizeof( MPROG_DATA ) );
    mprg = pMob->mobprogs;

    /* empty mob-program */
    mprg->type = 0;
    mprg->comlist = str_dup( "break\n" );
    mprg->arglist = str_dup( "0" );

    ch->desc->pEdit = (void *)pMob;
    ch->pcdata->mprog_edit = 0;

    send_to_char( "MOBProg Created.\n\r", ch );
    return TRUE;
}


MPEDIT( mpedit_add )
{
    MOB_INDEX_DATA *pMob;
    MPROG_DATA* mprg, *mprg2;
    char buf[ MAX_INPUT_LENGTH ];
    int count;

    EDIT_MOB(ch, pMob);

    if( !pMob->mobprogs )
    {
	send_to_char( "Mobile doesn't have mobprogs.  Use create.\n\r", ch );
	return FALSE;
    }

    count = mprog_count( pMob );

    /* find last mobprog and add after it */
    for( mprg = mprg2 = pMob->mobprogs; mprg; mprg2 = mprg, mprg = mprg->next )
	;

    mprg2->next = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
    mprg = mprg2->next;
    mprg->type = 0;
    mprg->arglist = str_dup( "0" );
    mprg->comlist = str_dup( "break\n" );
    count++;

    ch->pcdata->mprog_edit = count - 1;

    sprintf( buf, "MOBProg %d Added.\n\r", count );
    send_to_char( buf, ch );
    return TRUE;
}


MPEDIT( mpedit_delete )
{
    MOB_INDEX_DATA *pMob;
    char arg[ MAX_INPUT_LENGTH ];
    int count, pnum;

    EDIT_MOB(ch, pMob);
    count = mprog_count( pMob );

    argument = one_argument( argument, arg );

    if( arg[ 0 ] == '\0' )	/* No argument - delete current program */
    {
	delete_mprog( ch, ch->pcdata->mprog_edit );
    }
    else if( is_number( arg ) )
    {
	if( ( pnum = atoi( arg ) ) > count )
	{
	    send_to_char( "Mobile does not have that many programs.\n\r", ch );
	    return FALSE;
	}
	delete_mprog( ch, pnum - 1 );
    }
    else if( !str_cmp( arg, "all" ) )
    {
	for( pnum = count - 1; pnum >= 0; pnum-- )
	    delete_mprog( ch, pnum );
    }
    else
    {
	send_to_char( "用法：	delete [#pnum/all]\n\r", ch );
	return FALSE;
    }

    count = mprog_count( pMob );	/* Get new count */
    if( ch->pcdata->mprog_edit >= count )
    {
	ch->pcdata->mprog_edit = count - 1;
	if( count == 0 )
	    edit_done( ch );
    }
    send_to_char( "Ok.\n\r", ch );
    return TRUE;
}


MPEDIT( mpedit_copy )
{
    MOB_INDEX_DATA *pMob, *cMob;
    MPROG_DATA* mprg, *mprg_next, *cprg;
    int value;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "用法：	copy [vnum]\n\r", ch );
	return FALSE;
    }

    value = atoi( argument );
    if ( !( cMob = get_mob_index( value ) ) )
    {
	send_to_char( "No such mobile exists.\n\r", ch );
	return FALSE;
    }

    if( cMob == pMob )
    {
	send_to_char( "You can't copy from yourself, sorry.\n\r", ch );
	return FALSE;
    }

    if( !cMob->mobprogs )
    {
	send_to_char( "That mobile doesn't have mobprogs!\n\r", ch );
	return FALSE;
    }

    /* free existing mobprog list */
    for( mprg = pMob->mobprogs; mprg; mprg = mprg_next )
    {
	mprg_next = mprg->next;
	free_string( mprg->arglist );
	free_string( mprg->comlist );
	free_mem( mprg/*, sizeof( MPROG_DATA )*/ );
    }

    mprg = pMob->mobprogs = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );

    /* Start copying */
    for( cprg = cMob->mobprogs; cprg;
		cprg = cprg->next, mprg = mprg->next )
    {
	mprg->type = cprg->type;
	SET_BIT( pMob->progtypes, cprg->type );
	mprg->arglist = str_dup( cprg->arglist );
	mprg->comlist = str_dup( cprg->comlist );
	if( cprg->next )
	    mprg->next = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
	else
	    mprg->next = NULL;
    }

    ch->pcdata->mprog_edit = mprog_count( pMob ) - 1;

    send_to_char( "MOBProg copied.\n\r", ch );
    return TRUE;
}



MPEDIT( mpedit_trigger )
{
    MOB_INDEX_DATA *pMob;
    MPROG_DATA* pMobProg;

    if ( argument[0] == '\0' )
    {
	send_to_char( "用法：	trigger [trigger value(s)]\n\r", ch );
	return FALSE;
    }

    EDIT_MOB(ch, pMob);
    pMobProg = edit_mprog( ch, pMob );

    free_string( pMobProg->arglist );
    pMobProg->arglist = str_dup( argument );
    return TRUE;
}



MPEDIT( mpedit_program )
{
    MOB_INDEX_DATA *pMob;
    MPROG_DATA* pMobProg;

    EDIT_MOB(ch, pMob);
    pMobProg = edit_mprog( ch, pMob );

    if ( argument[0] == '\0' )
    {
	string_append( ch, &pMobProg->comlist );
	return TRUE;
    }

    send_to_char( "用法：  program\n\r", ch );
    return FALSE;
}


int mprog_count( MOB_INDEX_DATA* pMob )
{
    MPROG_DATA* mprg;
    int count;

    for( count = 0, mprg = pMob->mobprogs;
		mprg;
		mprg = mprg->next, count++ )
	;
    return count;
}


MPROG_DATA* edit_mprog( CHAR_DATA* ch, MOB_INDEX_DATA* pMob )
{
    MPROG_DATA* mprg;
    int mprog_num;
    int count = 0;

    if( IS_NPC( ch ) )
	return NULL;

    mprog_num = ch->pcdata->mprog_edit;
    for( mprg = pMob->mobprogs; mprg && count < mprog_num; mprg = mprg->next )
	count++;

    return mprg;
}


void show_mprog( CHAR_DATA* ch, MPROG_DATA* pMobProg )
{
    char buf[ MAX_STRING_LENGTH ];

    sprintf( buf, ">%s %s~\n\r",
	    mprog_type_to_name( pMobProg->type ),
	    pMobProg->arglist ? pMobProg->arglist : "NULL" );
    send_to_char( buf, ch );

    sprintf( buf, "%s~\n\r", pMobProg->comlist
				    ? pMobProg->comlist
				    : "NULL\n\r" );
    send_to_char( buf, ch );
}


void delete_mprog( CHAR_DATA* ch, int pnum )
{
    MPROG_DATA* mprg, *mprg_prev;
    MOB_INDEX_DATA* pMob;
    char buf[ MAX_INPUT_LENGTH ];
    int count;

    EDIT_MOB( ch, pMob );

    if( pnum < 0 ) /* sanity check */
	return;

    if( pnum == 0 )
    {
	free_string( pMob->mobprogs->arglist );
	free_string( pMob->mobprogs->comlist );
	mprg = pMob->mobprogs->next;
	/* Here is where we would recycle the memory of pMob->mobprogs...
	   no such mechanism yet so this actually IS a sort of memory leak
	   since memory allocated with alloc_mem cannot be freed.  Walker */
	pMob->mobprogs = mprg;
    }
    else
    {
	mprg_prev = pMob->mobprogs;
	mprg = mprg_prev->next;
	for( count = 1; mprg && count < pnum; count++ )
	{
	    mprg_prev = mprg;
	    mprg = mprg->next;
	}
	if( mprg )
	{
	    free_string( mprg->arglist );
	    free_string( mprg->comlist );
	    mprg_prev->next = mprg->next;
	    /* Here is where we would recycle the memory of mprg...
	       no such mechanism yet so this actually IS a sort of memory leak
	       since memory allocated with alloc_mem cannot be freed.	Walker */
	}
    }

    sprintf( buf, "MOBProg %d Deleted.\n\r", pnum + 1 );
    send_to_char( buf, ch );
    return;
}



bool oedit_mtlist( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *mat;
    char       buf  [ MAX_STRING_LENGTH ];


    if( !material_list ){
	send_to_char("No materials.\n\r", ch);
	return FALSE;
    }

    send_to_char("材 質 名 稱\n\r", ch);

    for( mat = material_list ; mat ; mat = mat->next ) {
	sprintf(buf,"%s\n\r",  mat->short_descr);
	send_to_char(buf, ch);
    }
    return FALSE;
}
