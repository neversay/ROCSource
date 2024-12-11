/***************************************************************************
 *  File: olc.c								   *
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
#include "merc.h"
#include "olc.h"



/*
 * Local functions.
 */
AREA_DATA *get_area_data	args( ( int vnum ) );

bool check_cost(CHAR_DATA *ch, int cost)
{
    char buf[MAX_STRING_LENGTH];
    if( !ch->clan ) return FALSE;
    if( cost < 0 ) return FALSE;
    if( ch->clan->bank >= cost ) {
	ch->clan->bank -= cost;
	if( cost > 0 )
	    sprintf(buf, "你從幫費裡付出 %d 元。\n\r", cost);
	else if( cost < 0 )
	    sprintf(buf, "你的幫費增加了 %d 元。\n\r", cost);
	send_to_char(buf, ch);
	clan_update();
	return TRUE;
    }
    return FALSE;
}
/*1.檢查有沒有這個選項
  2.檢查幫派等級
  3.檢查幫派戶頭，如果夠，扣款
  4.檢查是否為 imm，如果是，無償完成
  */
bool check_clco( const struct flag_cost fc[], char* arg, CHAR_DATA* ch)
{
	char buf[MAX_STRING_LENGTH];
    int i;
    
    for( i=0; *(fc[i].name); ++i)
		if( !str_cmp( arg, fc[i].name ) )
			break;
	if ( !( *(fc[i].name) )	)
	{
		send_to_char( "沒有這個選項啊？\n\r", ch );
		return FALSE;
	}
	if ( IS_IMMORTAL( ch ) ) return TRUE;
	if ( fc[i].cclass > ch->clan->cclass )
	{
		sprintf( buf, "組織的發展程度還不夠！(需要 %d 級)\n\r", fc[i].cclass );
		send_to_char( buf, ch );
		return FALSE;
	}
	if ( fc[i].cost > ch->clan->bank )
	{
		sprintf( buf, "組織戶頭裡沒有足夠的經費！(需要 %d 金幣)\n\r", fc[i].cost );
		send_to_char( buf, ch );
		return FALSE;
	}
	ch->clan->bank -= fc[i].cost;
	clan_update();
    return TRUE;
}

int flag_cost( const struct flag_cost fc[], char* arg)
{
    int i;
    for( i=0; *(fc[i].name); ++i){
	if( !str_cmp( arg, fc[i].name )) return fc[i].cost;
    }
    return -1;
}

int flag_class( const struct flag_cost fc[], char* arg)
{
    int i;
    for( i=0; *(fc[i].name); ++i){
	if( !str_cmp( arg, fc[i].name )) return fc[i].cclass;
    }
    return -1;
}

/* Executed from comm.c.  Minimizes compiling when changes are made. */
bool run_olc_editor( DESCRIPTOR_DATA *d )
{
    switch ( d->editor )
    {
    case ED_AREA:
	aedit( d->character, d->incomm );
	break;
    case ED_ROOM:
	redit( d->character, d->incomm );
	break;
    case ED_OBJECT:
	oedit( d->character, d->incomm );
	break;
    case ED_MOBILE:
	medit( d->character, d->incomm );
	break;
    case ED_MPROG:
	mpedit( d->character, d->incomm );
	break;
    default:
	return FALSE;
    }
    return TRUE;
}

long long int flag_lookup(const char *name, const struct flag_type *flag_table);

bool is_stat( const struct flag_type *flag_table );
long long int llflag_value( const struct flag_type *flag_table, char *argument, long long int* v)
{
    char word[MAX_INPUT_LENGTH];
    long long int	 bit;
    long long int	 marked = 0;
    bool found = FALSE;

    if ( is_stat( flag_table ) )
    {
	one_argument( argument, word );

	if ( ( bit = flag_lookup( word, flag_table ) ) != NO_FLAG )
	    return bit;
	else
	    return NO_FLAG;
    }

    /*
     * Accept multiple flags.
     */
    for (; ;)
    {
	argument = one_argument( argument, word );

	if ( word[0] == '\0' )
	    break;

	if ( ( bit = flag_lookup( word, flag_table ) ) != NO_FLAG )
	{
	    SET_BIT( marked, bit );
	    found = TRUE;
	    *v = marked;
	}
    }

    if ( found )
	return marked;
    else
	return NO_FLAG;
}

char *olc_ed_name( CHAR_DATA *ch )
{
    static char buf[10];
    
    buf[0] = '\0';
    if( !ch->desc ) {
	return buf;
    }
    switch (ch->desc->editor)
    {
    case ED_AREA:
	sprintf( buf, "AEdit" );
	break;
    case ED_ROOM:
	sprintf( buf, "REdit" );
	break;
    case ED_OBJECT:
	sprintf( buf, "OEdit" );
	break;
    case ED_MOBILE:
	sprintf( buf, "MEdit" );
	break;
    case ED_MPROG:
	sprintf( buf, "MPEdit" );
	break;
    default:
	sprintf( buf, " " );
	break;
    }
    return buf;
}



char *olc_ed_vnum( CHAR_DATA *ch )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    OBJ_INDEX_DATA *pObj;
    MOB_INDEX_DATA *pMob;
    static char buf[10];
	
    buf[0] = '\0';
    if( !ch->desc ) {
	return buf;
    }
    switch ( ch->desc->editor )
    {
    case ED_AREA:
	pArea = (AREA_DATA *)ch->desc->pEdit;
	sprintf( buf, "%d", pArea ? pArea->vnum : 0 );
	break;
    case ED_ROOM:
	pRoom = ch->in_room;
	sprintf( buf, "%d", pRoom ? pRoom->vnum : 0 );
	break;
    case ED_OBJECT:
	pObj = (OBJ_INDEX_DATA *)ch->desc->pEdit;
	sprintf( buf, "%d", pObj ? pObj->vnum : 0 );
	break;
    case ED_MOBILE:
	pMob = (MOB_INDEX_DATA *)ch->desc->pEdit;
	sprintf( buf, "%d", pMob ? pMob->vnum : 0 );
	break;
    case ED_MPROG:
	pMob = (MOB_INDEX_DATA *)ch->desc->pEdit;
	sprintf( buf, "%d(%d)", pMob ? pMob->vnum : 0,
				ch->pcdata->mprog_edit + 1 );
	break;
    default:
	sprintf( buf, " " );
	break;
    }

    return buf;
}



/*****************************************************************************
 Name:		show_olc_cmds
 Purpose:	Format up the commands from given table.
 Called by:	show_commands(olc_act.c).
 ****************************************************************************/
void show_olc_cmds( CHAR_DATA *ch, const struct olc_cmd_type *olc_table )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int	 cmd;
    int	 col;
 
    buf1[0] = '\0';
    col = 0;
    for (cmd = 0; olc_table[cmd].name[0] != '\0'; cmd++)
    {
	if( olc_table[cmd].immonly && !IS_IMMORTAL(ch)) continue;
	sprintf( buf, "%-15.15s", olc_table[cmd].name );
	strcat( buf1, buf );
	if ( ++col % 5 == 0 )
	    strcat( buf1, "\n\r" );
    }
 
    if ( col % 5 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}



/*****************************************************************************
 Name:		show_commands
 Purpose:	Display all olc commands.
 Called by:	olc interpreters.
 ****************************************************************************/
bool show_commands( CHAR_DATA *ch, char *argument )
{
    switch (ch->desc->editor)
    {
	case ED_AREA:
	    show_olc_cmds( ch, aedit_table );
	    break;
	case ED_ROOM:
	    show_olc_cmds( ch, redit_table );
	    break;
	case ED_OBJECT:
	    show_olc_cmds( ch, oedit_table );
	    break;
	case ED_MOBILE:
	    show_olc_cmds( ch, medit_table );
	    break;
	case ED_MPROG:
	    show_olc_cmds( ch, mpedit_table );
	    break;
    }

    return FALSE;
}



/*****************************************************************************
 *			     Interpreter Tables.			     *
 *****************************************************************************/
const struct olc_cmd_type aedit_table[] =
{
/*  {	command		function		}, */

    {	"age",		aedit_age		, TRUE},
    {	"builders",	aedit_builder		, TRUE},
    {	"commands",	show_commands		, FALSE},
    {	"create",	aedit_create		, TRUE},
    {	"filename",	aedit_file		, TRUE},
    {	"name",		aedit_name		, TRUE},
    {	"identity",		aedit_identity, TRUE},
    {	"recall",	aedit_recall		, TRUE},
    {	"reset",	aedit_reset		, TRUE},
    {	"rlist",	aedit_rlist, FALSE},
    {	"mlist",	aedit_mlist, FALSE},
    {	"olist",	aedit_olist, FALSE},
    {	"save",	aedit_save, FALSE},
    {	"security",	aedit_security		, TRUE},
    {	"show",		aedit_show		, FALSE},
    {	"ftoggle",		aedit_ftoggle, TRUE},
    {	"vnum",		aedit_vnum		, TRUE},
    {	"lvnum",	aedit_lvnum		, TRUE},
    {	"uvnum",	aedit_uvnum		, TRUE},

    {	"?",		show_help		, FALSE},
    {	"version",	show_version		, FALSE},

    {	"",		0,			 TRUE}
};



const struct olc_cmd_type redit_table[] =
{
/*  {	command		function		}, */

    {	"commands",	show_commands		, FALSE},
    {	"create",	redit_create		, FALSE},
    {	"desc",		redit_desc		, FALSE},
    {	"ed",		redit_ed		, FALSE},
    {	"room",		redit_room, FALSE},
    {	"format",	redit_format		, FALSE},
    {	"name",		redit_name		, FALSE},
    {	"purge",	redit_purge		, FALSE},
    {	"recall",	redit_recall		, FALSE},
    {	"sector",		redit_sector, FALSE},
    {	"show",		redit_show		, FALSE},

    {	"north",	redit_north		, FALSE},
    {	"south",	redit_south		, FALSE},
    {	"east",		redit_east		, FALSE},
    {	"west",		redit_west		, FALSE},
    {	"up",		redit_up		, FALSE},
    {	"down",		redit_down		, FALSE},
    {	"walk",		redit_move		, FALSE},

    /* New reset commands. */
    {	"mreset",	redit_mreset		, FALSE},
    {	"oreset",	redit_oreset		, FALSE},
    {	"mlist",	redit_mlist		, FALSE},
    {	"olist",	redit_olist		, FALSE},
    {	"mshow",	redit_mshow		, TRUE},
    {	"oshow",	redit_oshow		, TRUE},

    {	"?",		show_help		, FALSE},
    {	"version",	show_version		, FALSE},

    {	"",		0,		 TRUE	}
};



const struct olc_cmd_type oedit_table[] =
{
/*  {	command		function		}, */

    {	"addaffect",	oedit_addaffect		, TRUE},
/*    {	  "cname",	oedit_cname		},*/
    {	"commands",	show_commands		, FALSE},
    {	"cost",		oedit_cost		, FALSE},
    {	"create",	oedit_create		, FALSE},
    {	"delaffect",	oedit_delaffect		, TRUE},
    {	"ed",		oedit_ed		, FALSE},
    {	"extra",		oedit_extra		, FALSE},
    {	"level",		oedit_level, FALSE},
    {	"long",		oedit_long		, FALSE},
    {	"material",		oedit_material, FALSE},
    {	"mtlist",		oedit_mtlist, FALSE},
    {	"name",		oedit_name		, FALSE},
    {	"oload",		oedit_oload, FALSE},
    {	"short",	oedit_short		, FALSE},
    {	"show",		oedit_show		, FALSE},
    {	"type",		oedit_type, FALSE},
    {	"v0",		oedit_value0		, FALSE},
    {	"v1",		oedit_value1		, FALSE},
    {	"v2",		oedit_value2		, FALSE},
    {	"v3",		oedit_value3		, FALSE},
    {	"wear",	oedit_wear, FALSE},
    {	"weight",	oedit_weight		, FALSE},

    {	"?",		show_help		, FALSE},
    {	"version",	show_version		, FALSE},

    {	"",		0,			 TRUE}
};



const struct olc_cmd_type medit_table[] =
{
/*  {	command		function		}, */

    {	"alignment",	medit_align		, FALSE},
/*    {	  "cname",	medit_cname		},*/
    {	"commands",	show_commands		, FALSE},
    {	"create",	medit_create		, FALSE},
    {	"desc",		medit_desc		, FALSE},
    {	"level",	medit_level		, FALSE},
    {	"long",		medit_long		, FALSE},
    {	"name",		medit_name		, FALSE},
    {	"shop",		medit_shop		, TRUE},
    {	"short",	medit_short		, FALSE},
    {	"show",		medit_show		, FALSE},
    {	"sex",		medit_sex		, FALSE},
    {	"spec",		medit_spec		, FALSE},
    {	"class",	medit_class		, FALSE},
    {	"race",		medit_race		, FALSE},
    
    {	"?",		show_help		, FALSE},
    {	"version",	show_version	,TRUE},

    {	"",		0,			 TRUE}
};

const struct olc_cmd_type mpedit_table[] =
{
/*  {	command		function	}, */

    {	"add",		mpedit_add	, TRUE},
    {	"commands",	show_commands	, TRUE},
    {	"copy",		mpedit_copy	, TRUE},
    {	"create",	mpedit_create	, TRUE},
    {	"delete",	mpedit_delete	, TRUE},
    {	"program",	mpedit_program	, TRUE},
    {	"show",		mpedit_show	, TRUE},
    {	"trigger",	mpedit_trigger	, TRUE},

    {	"?",		show_help	, TRUE},
    {	"version",	show_version	, TRUE},

    {	"",		0,		 TRUE}
};
/*****************************************************************************
 *			    End Interpreter Tables.			     *
 *****************************************************************************/



/*****************************************************************************
 Name:		get_area_data
 Purpose:	Returns pointer to area with given vnum.
 Called by:	do_aedit(olc.c).
 ****************************************************************************/
AREA_DATA *get_area_data( int vnum )
{
    AREA_DATA *pArea;

    for (pArea = area_first; pArea; pArea = pArea->next )
    {
	if (pArea->vnum == vnum)
	    return pArea;
    }

    return 0;
}



/*****************************************************************************
 Name:		edit_done
 Purpose:	Resets builder information on completion.
 Called by:	aedit, redit, oedit, medit(olc.c)
 ****************************************************************************/
bool edit_done( CHAR_DATA *ch )
{
    if( ch->desc->editor == ED_OBJECT ){
        OBJ_INDEX_DATA *pObj;
        EDIT_OBJ(ch, pObj);
        if( pObj->cost < 0 ) pObj->cost = 0;
    }
    ch->desc->pEdit = NULL;
    ch->desc->editor = 0;
    return FALSE;
}



/*****************************************************************************
 *				Interpreters.				     *
 *****************************************************************************/


/* Area Interpreter, called by do_aedit. */
void aedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char command[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int	 cmd;
    long long int	 value;

    EDIT_AREA(ch, pArea);
    smash_tilde( argument );
    strcpy( arg, argument );
    argument = one_argument( argument, command );

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
	send_to_char( "AEdit:  Insufficient security to modify area.\n\r", ch );


    if ( command[0] == '\0' )
    {
	aedit_show( ch, argument );
	return;
    }

    if ( !str_cmp(command, "done") )
    {
	edit_done( ch );
	return;
    }

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
    {
	interpret( ch, arg );
	return;
    }

    /* Search Table and Dispatch Command. */
    for ( cmd = 0; *aedit_table[cmd].name; cmd++ )
    {
	if ( !str_prefix( command, aedit_table[cmd].name ) )
	{
	    if( aedit_table[cmd].immonly && !IS_IMMORTAL(ch) ) continue;
	    if ( (*aedit_table[cmd].olc_fun) ( ch, argument ) )
		SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
    }

    if( IS_IMMORTAL(ch)){
	/* Take care of flags. */
	if ( ( value = flag_value( area_flags, arg ) ) != NO_FLAG )
	{
	    TOGGLE_BIT(pArea->area_flags, value);

	    send_to_char( "Flag toggled.\n\r", ch );
	    return;
	}
    }
    /* Default to Standard Interpreter. */
    interpret( ch, arg );
    return;
}



/* Room Interpreter, called by do_redit. */
void redit( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    AREA_DATA *pArea;
    char arg[MAX_STRING_LENGTH];
    char command[MAX_INPUT_LENGTH];
    int	 cmd;
    long long int	 value;

    EDIT_ROOM(ch, pRoom);
    pArea = pRoom->area;

    smash_tilde( argument );
    strcpy( arg, argument );
    argument = one_argument( argument, command );

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
	send_to_char( "REdit:  Insufficient security to modify room.\n\r", ch );

    if ( command[0] == '\0' )
    {
	redit_show( ch, argument );
	return;
    }

    if ( !str_cmp(command, "done") )
    {
	edit_done( ch );
	return;
    }

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
    {
	interpret( ch, arg );
	return;
    }

    /* Search Table and Dispatch Command. */
    for ( cmd = 0; *redit_table[cmd].name; cmd++ )
    {
	if( redit_table[cmd].immonly && !IS_IMMORTAL(ch) ) continue;
	if ( !str_prefix( command, redit_table[cmd].name ) )
	{
	    if ( (*redit_table[cmd].olc_fun) ( ch, argument ) )
		SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
    }

    /* Take care of flags. */
    if( IS_IMMORTAL(ch)){
	if ( ( value = flag_value( room_flags, arg ) ) != NO_FLAG )
	{
	    TOGGLE_BIT(pRoom->room_flags, value);

	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    send_to_char( "Room flag toggled.\n\r", ch );
	    return;
	}

	if ( ( value = flag_value( sector_flags, arg ) ) != NO_FLAG )
	{
	    pRoom->sector_type  = value;

	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    send_to_char( "Sector type set.\n\r", ch );
	    return;
	}
    }

    /* Default to Standard Interpreter. */
    interpret( ch, arg );
    return;
}



/* Object Interpreter, called by do_oedit. */
void oedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    OBJ_INDEX_DATA *pObj;
    char arg[MAX_STRING_LENGTH];
    char command[MAX_INPUT_LENGTH];
    int	 cmd;
    long long int	 value;

    smash_tilde( argument );
    strcpy( arg, argument );
    argument = one_argument( argument, command );

    EDIT_OBJ(ch, pObj);
    pArea = pObj->area;

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea) )
	send_to_char( "OEdit: Insufficient security to modify area.\n\r", ch );

    if ( command[0] == '\0' )
    {
	oedit_show( ch, argument );
	return;
    }

    if ( !str_cmp(command, "done") )
    {
	edit_done( ch );
	return;
    }

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea))
    {
	interpret( ch, arg );
	return;
    }

    /* Search Table and Dispatch Command. */
    for ( cmd = 0; *oedit_table[cmd].name; cmd++ )
    {
	if( oedit_table[cmd].immonly && !IS_IMMORTAL(ch) ) continue;
	if ( !str_prefix( command, oedit_table[cmd].name ) )
	{
	    if ( (*oedit_table[cmd].olc_fun) ( ch, argument ) )
		SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
    }

    if( IS_IMMORTAL(ch)){
    /* Take care of flags. */
	if ( ( value = flag_value( type_flags, arg ) ) != NO_FLAG )
	{
	    pObj->item_type = value;

	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    send_to_char( "Type set.\n\r", ch);

	    /*
	    * Clear the values.
	     */
	    pObj->value[0] = 0;
	    pObj->value[1] = 0;
	    pObj->value[2] = 0;
	    pObj->value[3] = 0;

	    return;
	}

	if ( ( value = flag_value( extra_flags, arg ) ) != NO_FLAG )
	{
	    TOGGLE_BIT(pObj->extra_flags, value);

	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    send_to_char( "Extra flag toggled.\n\r", ch);
	    return;
	}

	if ( ( value = flag_value( wear_flags, arg ) ) != NO_FLAG )
	{
	    TOGGLE_BIT(pObj->wear_flags, value);

	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    send_to_char( "Wear flag toggled.\n\r", ch);
	    return;
	}
    }

    /* Default to Standard Interpreter. */
    interpret( ch, arg );
    return;
}



/* Mobile Interpreter, called by do_medit. */
void medit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    MOB_INDEX_DATA *pMob;
    char command[MAX_INPUT_LENGTH];
    char arg[MAX_STRING_LENGTH];
    int	 cmd;
    long long int	 value;

    smash_tilde( argument );
    strcpy( arg, argument );
    argument = one_argument( argument, command );

    EDIT_MOB(ch, pMob);
    pArea = pMob->area;
    if( IS_NPC(ch) ) return;

    if ( !IS_BUILDER( ch, pArea ) && !IS_PPLBUILDER(ch, pArea) )
	send_to_char( "MEdit: Insufficient security to modify area.\n\r", ch );

    if ( command[0] == '\0' )
    {
	medit_show( ch, argument );
	return;
    }

    if ( !str_cmp(command, "done") )
    {
	edit_done( ch );
	return;
    }

    if ( !IS_BUILDER( ch, pArea )  && !IS_PPLBUILDER(ch, pArea) )
    {
	interpret( ch, arg );
	return;
    }

    /* Search Table and Dispatch Command. */
    for ( cmd = 0; *medit_table[cmd].name; cmd++ )
    {
	if ( !str_prefix( command, medit_table[cmd].name ) )
	{
	    if ( (*medit_table[cmd].olc_fun) ( ch, argument ) )
		SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
    }

if ( IS_IMMORTAL( ch ) )
{
    /* Take care of flags. */
    if ( ( value = flag_value( sex_flags, arg ) ) != NO_FLAG )
    {
	pMob->sex = value;

	SET_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char( "Sex set.\n\r", ch);
	return;
    }

/*
    if ( ( value = race_full_lookup( arg ) ) != NO_FLAG )
*/
    if ( ( value = flag_value( race_flags, arg ) ) != NO_FLAG )
    {
	pMob->race = value;

	SET_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char( "Race set.\n\r", ch);
	return;
    }

    if ( ( value = flag_value( act_flags, arg ) ) != NO_FLAG )
    {
	TOGGLE_BIT(pMob->act, value);

	SET_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char( "Act flag toggled.\n\r", ch);
	return;
    }

    llflag_value( affect_flags, arg, &value ) ;
    //value = flag_value(affect_flags, arg);
    if ( value != NO_FLAG )
    {
	TOGGLE_BIT(pMob->affected_by, value);

	SET_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char( "Affect flag toggled.\n\r", ch);
	return;
    }
}
    /* Default to Standard Interpreter. */
    interpret( ch, arg );
    return;
}


/* MobProg Interpreter, called by do_mpedit. */
void mpedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    MOB_INDEX_DATA *pMob;
    MPROG_DATA* pMobProg;
    char command[MAX_INPUT_LENGTH];
    char arg[MAX_STRING_LENGTH];
    int	 cmd;
    long long int	 value;

    smash_tilde( argument );
    strcpy( arg, argument );
    argument = one_argument( argument, command );

    EDIT_MOB(ch, pMob);
    pArea = pMob->area;
    pMobProg = edit_mprog( ch, pMob );

    if ( !IS_BUILDER( ch, pArea ) )
	send_to_char( "MPEdit: Insufficient security to modify area.\n\r", ch );

    if ( command[0] == '\0' )
    {
	mpedit_show( ch, "" );
	return;
    }

    if ( !str_cmp(command, "done") )
    {
	edit_done( ch );
	return;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	interpret( ch, arg );
	return;
    }

    /* Set mobprog type */
    if( ( value = flag_value( mprog_type_flags, arg ) ) != NO_FLAG )
    {
	if( pMobProg->type )
	    REMOVE_BIT( pMob->progtypes, pMobProg->type );
	pMobProg->type = value;
	SET_BIT( pMob->progtypes, pMobProg->type );

	SET_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char( "MOBProg type set.\n\r", ch);
	return;
    }

    /* Search Table and Dispatch Command. */
    for ( cmd = 0; mpedit_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( !str_prefix( command, mpedit_table[cmd].name ) )
	{
	    if ( (*mpedit_table[cmd].olc_fun) ( ch, argument ) )
	    {
		SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	    else
		return;
	}
    }

    /* Default to Standard Interpreter. */
    interpret( ch, arg );
    return;
}

void do_aedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char command[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;

    if ( !( rch = get_char( ch ) ) )
	return;
    
    if( IS_NPC(ch)) return;
    if( !ch->in_room ) return;
    if( IS_IMMORTAL(ch)){
	if ( !authorized( rch, "aedit" ) )
	    return;
    }
    else {
	pArea = ch->in_room->area;
	if( !IS_PPLBUILDER(ch, pArea) && !IS_BUILDER(ch, pArea)){
	    send_to_char("你不能查看這個區域的資料。\n\r", ch);
	    return;
	}
	if( ch->pcdata->clanlevel < 4 ) {
	    send_to_char("你的權限不夠。\n\r", ch);
	    return;
	}
	ch->desc->pEdit = (void *)pArea;
	ch->desc->editor = ED_AREA;
	aedit_show( ch, "" );
	return;
    }
    argument = one_argument( argument, command );
    pArea = ch->in_room->area;

    if ( command[0] == 'r' && !str_prefix( command, "reset" ) )
    {
	if ( ch->desc->editor == ED_AREA )
	    reset_area( (AREA_DATA *)ch->desc->pEdit );
	else
	    reset_area( pArea );
	send_to_char( "Area reset.\n\r", ch );
	return;
    }

    if ( command[0] == 'c' && !str_prefix( command, "create" ) )
    {
	if ( aedit_create( ch, argument ) )
	{
	    ch->desc->editor = ED_AREA;
	    pArea = (AREA_DATA *)ch->desc->pEdit;
	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    aedit_show( ch, "" );
	}
	return;
    }

    if ( is_number( command ) )
    {
	if ( !( pArea = get_area_data( atoi(command) ) ) )
	{
	    send_to_char( "No such area vnum exists.\n\r", ch );
	    return;
	}
    }

    /*
     * Builder defaults to editing current area.
     */
    ch->desc->pEdit = (void *)pArea;
    ch->desc->editor = ED_AREA;
    aedit_show( ch, "" );
    return;
}


/* Entry point for editing room_index_data. */
void do_redit( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    char command[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;

    if ( !( rch = get_char( ch ) ) )
	return;
    
    if( IS_NPC(ch)) return;
    if( IS_IMMORTAL(ch)){
	if ( !authorized( rch, "redit" ) && 
	( !ch->clan || ch->pcdata->clanlevel < 4 ) )
	    return;
	}
    else {
	if( !ch->clan ) {
	    send_to_char("你沒有參加任何幫派。\n\r", ch);
	    return;
	}
	if( ch->pcdata->clanlevel < 4 ) {
	    send_to_char("你在幫中的地位不夠。\n\r", ch);
	    return;
	}
    }

    argument = one_argument( argument, command );
    pRoom = ch->in_room;

    if( !IS_BUILDER(ch, pRoom->area) && !IS_PPLBUILDER(ch, pRoom->area)){
	send_to_char("你沒有足夠的權限。\n\r", ch);
	return;
    }

    if ( command[0] == 'r' && !str_prefix( command, "reset" ) )
    {
	reset_room( pRoom );
	send_to_char( "Room reset.\n\r", ch );
	return;
    }

    if ( command[0] == 'c' && !str_prefix( command, "create" ) )
    {
	if ( redit_create( ch, argument ) )
	{
	    char_from_room( ch );
	    char_to_room( ch, ch->desc->pEdit );
	    SET_BIT( pRoom->area->area_flags, AREA_CHANGED );
	}
    }

    /*
     * Builder defaults to editing current room.
     */
    ch->desc->editor = ED_ROOM;
    redit_show( ch, "" );
    return;
}



/* Entry point for editing obj_index_data. */
void do_oedit( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AREA_DATA *pArea;
    char command[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;

    if ( !( rch = get_char( ch ) ) )
	return;
    if( IS_NPC(ch)) return;
    if( IS_IMMORTAL(ch)){
	if ( !authorized( rch, "redit" ) && ( !ch->clan || ch->pcdata->clanlevel < 4 ) )
	    return;
    }
    else {
	if( !ch->clan ) {
	    send_to_char("你沒有參加任何幫派。\n\r", ch);
	    return;
	}
	if( ch->pcdata->clanlevel < 4 ) {
	    send_to_char("你在幫中的地位不夠。\n\r", ch);
	    return;
	}
		if( !IS_SET( ch->in_room->room_flags, ROOM_ARMORY ) )
		{
			send_to_char("必須在打鐵舖中才能進行製造作業。\n\r", ch);
	    	return;
		}
    }

    argument = one_argument( argument, command );

    if ( is_number( command ) )
    {
	if ( !( pObj = get_obj_index( atoi( command ) ) ) )
	{
	    send_to_char( "OEdit:  沒有那個 VNUM.\n\r", ch );
	    return;
	}

	if( !IS_BUILDER(ch,pObj->area) && !IS_PPLBUILDER(ch, pObj->area)){
	    send_to_char("你沒有足夠的權限。\n\r", ch);
	    return;
	}

	ch->desc->pEdit = (void *)pObj;
	ch->desc->editor = ED_OBJECT;
	oedit_show( ch, "" );
	return;
    }

    if ( command[0] == 'c' && !str_prefix( command, "create" ) )
    {
	if ( oedit_create( ch, argument ) )
	{
	    if( IS_IMMORTAL(ch)){
                pArea = get_vnum_area( atoi( argument ) );
                if( !pArea )
                    return;
                SET_BIT( pArea->area_flags, AREA_CHANGED );
	    }
	    ch->desc->editor = ED_OBJECT;
	    oedit_show( ch, "" );
	}
	return;
    }

    send_to_char( "OEdit:  沒有指定編輯的物品.\n\r", ch );
    return;
}



/* Entry point for editing mob_index_data. */
void do_medit( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    AREA_DATA *pArea;
    char command[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;

    if ( !( rch = get_char( ch ) ) )
	return;
    
    if( IS_IMMORTAL(ch) )
    {
	if ( !authorized( rch, "redit" ) && ( !ch->clan || ch->pcdata->clanlevel < 4 ) )
			return;
	}
    else
    {
		if( !ch->clan )
		{
	    	send_to_char("你沒有參加任何幫派。\n\r", ch);
	    	return;
		}
		if( ch->pcdata->clanlevel < 4 )
		{
	    	send_to_char("你在幫中的地位不夠。\n\r", ch);
	    	return;
		}
		if( !IS_SET( ch->in_room->room_flags, ROOM_BARRACK ) )
		{
			send_to_char("必須在軍營中才能展開招募作業。\n\r", ch);
	    	return;
		}
    }
    argument = one_argument( argument, command );

    if ( is_number( command ) )
    {
	if ( !( pMob = get_mob_index( atoi( command ) ) ))
	{
	    send_to_char( "MEdit:  That vnum does not exist.\n\r", ch );
	    return;
	}

	if( !IS_BUILDER( ch,pMob->area ) && !IS_PPLBUILDER( ch, pMob->area ) )
	{
	    send_to_char( "你沒有足夠的權限。\n\r", ch );
	    return;
	}

	ch->desc->pEdit = (void *)pMob;
	ch->desc->editor = ED_MOBILE;
	medit_show( ch, "" );
	return;
    }

    if ( command[0] == 'c' && !str_prefix( command, "create" ) )
    {
	if ( medit_create( ch, argument ) )
	{
	    if( argument[0] == '\0' )
		pArea = ch->in_room->area;
	    else
		pArea = get_vnum_area( atoi( argument ) );
            if( !pArea )
                return;
	    SET_BIT( pArea->area_flags, AREA_CHANGED );
	    ch->desc->editor = ED_MOBILE;
	    medit_show( ch, "" );
	}
	return;
    }

    send_to_char( "MEdit:  There is no default mobile to edit.\n\r", ch );
    return;
}

/* Entry point for editing mob_prog_data. */
void do_mpedit( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    AREA_DATA *pArea;
    MPROG_DATA* mprg, *cprg;
    int value;
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    CHAR_DATA *rch;

    if ( !( rch = get_char( ch ) ) )
	return;
    
    if ( !authorized( rch, "mpedit" ) )
	return;

    if ( IS_NPC(ch) )
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ch->in_room && !IS_BUILDER( ch, ch->in_room->area ) )
    {
	send_to_char( "MPEdit:	Insufficient security to edit mobprog.\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	value = atoi( arg1 );
	if ( !( pMob = get_mob_index( value ) ))
	{
	    send_to_char( "MPEdit:  That vnum does not exist.\n\r", ch );
	    return;
	}

	if( arg2[ 0 ] == '\0' || atoi( arg2 ) == 0 )
	{
	    if( !pMob->mobprogs )
	    {
		send_to_char( "MPEdit:	Mobile has no mobprogs.	 Use create.\n\r", ch );
		return;
	    }
	    else
	    {
		send_to_char( "MPEdit:	Editing first mobprog.\n\r", ch );
		value = 1;
	    }
	}
	else if( ( value = atoi( arg2 ) ) > mprog_count( pMob ) )
	{
	    send_to_char( "MPEdit:  Mobile does not have that many mobprogs.\n\r", ch );
	    return;
	}

	ch->desc->pEdit = (void *)pMob;
	ch->pcdata->mprog_edit = value - 1;
	ch->desc->editor = ED_MPROG;
	return;
    }
    else
    {
	if ( !str_cmp( arg1, "create" ) )
	{
	    value = atoi( arg2 );
	    if ( arg2[0] == '\0' || value == 0 )
	    {
		send_to_char( "Syntax:	edit mobprog create vnum [svnum]\n\r",
			ch );
		return;
	    }

	    pArea = get_vnum_area( value );

	    if ( mpedit_create( ch, arg2 ) )
	    {
		SET_BIT( pArea->area_flags, AREA_CHANGED );
		ch->desc->editor = ED_MPROG;
	    }
	    else
		return;

	    /* See if another argument was supplied, meaning COPY */
	    if( is_number( argument ) )
	    {
		MOB_INDEX_DATA *cMob, *pMob;

		/* Check destination mobile */
		if ( !( pMob = get_mob_index( value ) ) )
		{
		    send_to_char( "No destination mob exists.\n\r", ch );
		    return;
		}

		value = atoi( argument );
		if ( !( cMob = get_mob_index( value ) ) )
		{
		    send_to_char( "No such source mob exists.\n\r", ch );
		    return;
		}

		/* Start copying */
		mprg = pMob->mobprogs; /* allocated by create */

		for( cprg = cMob->mobprogs; cprg;
			    cprg = cprg->next, mprg = mprg->next )
		{
		    mprg->type = cprg->type;
		    mprg->arglist = str_dup( cprg->arglist );
		    mprg->comlist = str_dup( cprg->comlist );
		    if( cprg->next )
			mprg->next = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
		    else
			mprg->next = NULL;
		}

		send_to_char( "MOBProg copied.\n\r", ch );
	    }
	    return;
	}
    }

    send_to_char( "MPEdit:  There is no default mobile to edit.\n\r", ch );
    return;
}


void display_resets( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA	*pRoom;
    RESET_DATA		*pReset;
    MOB_INDEX_DATA	*pMob = NULL;
    char		buf   [ MAX_STRING_LENGTH ];
    char		final [ MAX_STRING_LENGTH ];
    int		iReset = 0;

    EDIT_ROOM(ch, pRoom);
    final[0]  = '\0';
    
    send_to_char ( 
  " No.	 Loads	  Description	    Location	     Vnum    Max  Description"
  "\n\r"
  "==== ======== ============= =================== ======== ===== ==========="
  "\n\r", ch );

    for ( pReset = pRoom->reset_first; pReset; pReset = pReset->next )
    {
	OBJ_INDEX_DATA	*pObj;
	MOB_INDEX_DATA	*pMobIndex;
	OBJ_INDEX_DATA	*pObjIndex;
	OBJ_INDEX_DATA	*pObjToIndex;
	ROOM_INDEX_DATA *pRoomIndex;

	final[0] = '\0';
	sprintf( final, "[%2d] ", ++iReset );

	switch ( pReset->command )
	{
	default:
	    sprintf( buf, "Bad reset command: %c.", pReset->command );
	    strcat( final, buf );
	    break;

	case 'M':
	    if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
	    {
		sprintf( buf, "Load Mobile - Bad Mob %d\n\r", pReset->arg1 );
		strcat( final, buf );
		continue;
	    }

	    if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
	    {
		sprintf( buf, "Load Mobile - Bad Room %d\n\r", pReset->arg3 );
		strcat( final, buf );
		continue;
	    }

	    pMob = pMobIndex;
	    sprintf( buf, "M[%5d] %s(%s) in room	     R[%5d] [%3d] %-15.15s\n\r",
		       pReset->arg1, pMob->short_descr, pMob->player_name, pReset->arg3,
		       pReset->arg2, pRoomIndex->name );
	    strcat( final, buf );

	    /*
	     * Check for pet shop.
	     * -------------------
	     */
	    {
		ROOM_INDEX_DATA *pRoomIndexPrev;

		pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );
		if ( pRoomIndexPrev
		    && IS_SET( pRoomIndexPrev->room_flags, ROOM_PET_SHOP ) )
		    final[5] = 'P';
	    }

	    break;

	case 'O':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
		sprintf( buf, "Load Object - Bad Object %d\n\r",
		    pReset->arg1 );
		strcat( final, buf );
		continue;
	    }

	    pObj       = pObjIndex;

	    if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
	    {
		sprintf( buf, "Load Object - Bad Room %d\n\r", pReset->arg3 );
		strcat( final, buf );
		continue;
	    }

	    sprintf( buf, "O[%5d] %s(%s) in room	     "
			  "R[%5d]	%-15.15s\n\r",
			  pReset->arg1, pObj->short_descr, pObj->name,
			  pReset->arg3, pRoomIndex->name );
	    strcat( final, buf );

	    break;

	case 'P':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
		sprintf( buf, "Put Object - Bad Object %d\n\r",
		    pReset->arg1 );
		strcat( final, buf );
		continue;
	    }

	    pObj       = pObjIndex;

	    if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
	    {
		sprintf( buf, "Put Object - Bad To Object %d\n\r",
		    pReset->arg3 );
		strcat( final, buf );
		continue;
	    }

	    sprintf( buf,
		"O[%5d] %s(%s) inside		   O[%5d]	%s(%s)\n\r",
		pReset->arg1,
		pObj->short_descr,
		pObj->name,
		pReset->arg3,
		pObjToIndex->short_descr,
		pObjToIndex->name );
	    strcat( final, buf );

	    break;

	case 'G':
	case 'E':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
		sprintf( buf, "Give/Equip Object - Bad Object %d\n\r",
		    pReset->arg1 );
		strcat( final, buf );
		continue;
	    }

	    pObj       = pObjIndex;

	    if ( !pMob )
	    {
		sprintf( buf, "Give/Equip Object - No Previous Mobile\n\r" );
		strcat( final, buf );
		break;
	    }

	    if ( pMob->pShop )
	    {
	    sprintf( buf,
		"O[%5d] %s(%s) in the inventory of S[%5d]	%s(%s)\n\r",
		pReset->arg1,
		pObj->short_descr,
		pObj->name,			      
		pMob->vnum,
		pMob->short_descr,
		pMob->player_name  );
	    }
	    else
	    sprintf( buf,
		"O[%5d] %s(%s) %-19.19s M[%5d]	     %s(%s)\n\r",
		pReset->arg1,
		pObj->short_descr,
		pObj->name,
		(pReset->command == 'G') ?
		    flag_string( wear_loc_strings, WEAR_NONE )
		  : flag_string( wear_loc_strings, pReset->arg3 ),
		  pMob->vnum,
		  pMob->short_descr,
		  pMob->player_name );
	    strcat( final, buf );

	    break;

	/*
	 * Doors are set in rs_flags don't need to be displayed.
	 * If you want to display them then uncomment the new_reset
	 * line in the case 'D' in load_resets in db.c and here.
	 *
	case 'D':
	    pRoomIndex = get_room_index( pReset->arg1 );
	    sprintf( buf, "R[%5d] %s door of %-19.19s reset to %s\n\r",
		pReset->arg1,
		capitalize( dir_name[ pReset->arg2 ] ),
		pRoomIndex->name,
		flag_string( door_resets, pReset->arg3 ) );
	    strcat( final, buf );

	    break;
	 *
	 * End Doors Comment.
	 */
	case 'R':
	    if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
	    {
		sprintf( buf, "Randomize Exits - Bad Room %d\n\r",
		    pReset->arg1 );
		strcat( final, buf );
		continue;
	    }

	    sprintf( buf, "R[%5d] Exits are randomized in %s\n\r",
		pReset->arg1, pRoomIndex->name );
	    strcat( final, buf );

	    break;
	}
	send_to_char( final, ch );
    }

    return;
}



/*****************************************************************************
 Name:		add_reset
 Purpose:	Inserts a new reset in the given index slot.
 Called by:	do_resets(olc.c).
 ****************************************************************************/
void add_reset( ROOM_INDEX_DATA *room, RESET_DATA *pReset, int index )
{
    RESET_DATA *reset;
    int iReset = 0;

    if ( !room->reset_first )
    {
	room->reset_first	= pReset;
	room->reset_last	= pReset;
	pReset->next		= NULL;
	return;
    }

    index--;

    if ( index == 0 )	/* First slot (1) selected. */
    {
	pReset->next = room->reset_first;
	room->reset_first = pReset;
	return;
    }

    /*
     * If negative slot( <= 0 selected) then this will find the last.
     */
    for ( reset = room->reset_first; reset->next; reset = reset->next )
    {
	if ( ++iReset == index )
	    break;
    }

    pReset->next	= reset->next;
    reset->next		= pReset;
    if ( !pReset->next )
	room->reset_last = pReset;
    return;
}



void do_resets( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    char arg5[MAX_INPUT_LENGTH];
    RESET_DATA *pReset = NULL;
    CHAR_DATA *rch;
    AREA_DATA *pArea;

    if ( !( rch = get_char( ch ) ) )
	return;
    
    if( IS_NPC(ch)) return;
    if( !ch->in_room ) return;
    if( IS_IMMORTAL(ch)){
	if ( !authorized( rch, "resets" ) )
	    return;
    }
    else {
	pArea = ch->in_room->area;
	if( !IS_PPLBUILDER(ch, pArea) && !IS_BUILDER(ch, pArea)){
	    send_to_char("你不能查看這個區域的資料。\n\r", ch);
	    return;
	}
	if( ch->pcdata->clanlevel < 4 ) {
	    send_to_char("你的權限不夠。\n\r", ch);
	    return;
	}
    }


    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );
    argument = one_argument( argument, arg5 );

    /*
     * Display resets in current room.
     * -------------------------------
     */
    if ( arg1[0] == '\0' )
    {
	if ( ch->in_room->reset_first )
	{
	    send_to_char(
		"Resets: M = 非玩者生物, R = 房間, O = 物品, "
		"P = 寵物, S = 店老闆\n\r", ch );
	    display_resets( ch );
	}
	else
	    send_to_char( "無.\n\r", ch );
    }

    if ( !IS_BUILDER( ch, ch->in_room->area ) && !IS_PPLBUILDER(ch, ch->in_room->area))
    {
	send_to_char( "Resets: Invalid security for editing this area.\n\r",
		      ch );
	return;
    }

    /*
     * Take index number and search for commands.
     * ------------------------------------------
     */
    if ( is_number( arg1 ) )
    {
	ROOM_INDEX_DATA *pRoom = ch->in_room;

	/*
	 * Delete a reset.
	 * ---------------
	 */
	if ( !str_cmp( arg2, "delete" ) )
	{
	    int insert_loc = atoi( arg1 );

	    if ( !ch->in_room->reset_first )
	    {
		send_to_char( "本區域無 RESETS.\n\r", ch );
		return;
	    }

	    if ( insert_loc-1 <= 0 )
	    {
		pReset = pRoom->reset_first;
		pRoom->reset_first = pRoom->reset_first->next;
		if ( !pRoom->reset_first )
		    pRoom->reset_last = NULL;
	    }
	    else
	    {
		int iReset = 0;
		RESET_DATA *prev = NULL;

		for ( pReset = pRoom->reset_first;
		  pReset;
		  pReset = pReset->next )
		{
		    if ( ++iReset == insert_loc )
			break;
		    prev = pReset;
		}

		if ( !pReset )
		{
		    send_to_char( "找不到 RESETS.\n\r", ch );
		    return;
		}

		if ( prev )
		    prev->next = prev->next->next;
		else
		    pRoom->reset_first = pRoom->reset_first->next;

		for ( pRoom->reset_last = pRoom->reset_first;
		  pRoom->reset_last->next;
		  pRoom->reset_last = pRoom->reset_last->next );
	    }

	    free_reset_data( pReset );
	    send_to_char( "RESETS 刪除成功\.\n\r", ch );
	}
	else if ( !IS_IMMORTAL(ch)) return;
	/*
	 * Add a reset.
	 * ------------
	 */
	if ( (!str_cmp( arg2, "mob" ) && is_number( arg3 ))
	  || (!str_cmp( arg2, "obj" ) && is_number( arg3 )) )
	{
	    /*
	     * Check for Mobile reset.
	     * -----------------------
	     */
	    if ( !str_cmp( arg2, "mob" ) && IS_IMMORTAL(ch))
	    {
		pReset = new_reset_data();
		pReset->command = 'M';
		pReset->arg1	= atoi( arg3 );
		pReset->arg2	= is_number( arg4 ) ? atoi( arg4 ) : 1; /* Max # */
		pReset->arg3	= ch->in_room->vnum;
	    }
	    else
	    /*
	     * Check for Object reset.
	     * -----------------------
	     */
	    if ( !str_cmp( arg2, "obj" ) )
	    {
		pReset = new_reset_data();
		pReset->arg1	= atoi( arg3 );
		if( !IS_IMMORTAL(ch) ) {
		    if( !check_cost(ch, COST_ADD_RESETOBJ)){
			send_to_char("你的幫費不夠了。\n\r", ch);
			return ;
		    }

		    if ( !str_cmp( arg4, "room" ) )
		    {
			/*pReset = new_reset_data();*/
			pReset->command = 'O';
			pReset->arg1    = atoi( arg3 );
			pReset->arg2     = 0; 
			pReset->arg3     = ch->in_room->vnum;
			add_reset( ch->in_room, pReset, atoi( arg1 ) );
			send_to_char( "Reset 增加成功\.\n\r", ch );
			return ;
		    }
		    else {
			send_to_char( "	       RESET <number> OBJ <vnum> room\n\r", ch );
			send_to_char( "	       RESET <number> DELETE\n\r", ch );
		    }
		}
		/*
		 * Inside another object.
		 * ----------------------
		 */
		if ( !str_prefix( arg4, "inside" )  )
		{
		    pReset->command = 'P';
		    pReset->arg2    = 0;
		    pReset->arg3    = is_number( arg5 ) ? atoi( arg5 ) : 1;
		}
		else
		/*
		 * Inside the room.
		 * ----------------
		 */
		if ( !str_cmp( arg4, "room" ) )
		{
		    /*pReset = new_reset_data();*/
		    pReset->command = 'O';
		    pReset->arg1    = atoi( arg3 );
		    pReset->arg2     = 0; 
		    pReset->arg3     = ch->in_room->vnum;
		}
		else
		/*
		 * Into a Mobile's inventory.
		 * --------------------------
		 */
		{
		    if ( flag_value( wear_loc_flags, arg4 ) == NO_FLAG )
		    {
			send_to_char( "Resets: '? wear-loc'\n\r", ch );
			return;
		    }
		    /*pReset = new_reset_data();*/
		    pReset->arg3 = flag_value( wear_loc_flags, arg4 );
		    if ( pReset->arg2 == WEAR_NONE )
			pReset->command = 'G';
		    else
			pReset->command = 'E';
		}
	    }

	    add_reset( ch->in_room, pReset, atoi( arg1 ) );
	    send_to_char( "Reset 增加成功\.\n\r", ch );
	}
	else
	{
	    send_to_char( "Syntax:     RESET <number> DELETE\n\r", ch );
	    send_to_char( "	       RESET <number> OBJ <vnum> room\n\r", ch );
	    if( IS_IMMORTAL(ch)){
	    send_to_char( "	       RESET <number> OBJ <vnum> <wear_loc>\n\r", ch );
	    send_to_char( "	       RESET <number> OBJ <vnum> in <vnum>\n\r", ch );
	    send_to_char( "	       RESET <number> MOB <vnum> [<max #>]\n\r", ch );
	    }
	}
    }

    return;
}



/*****************************************************************************
 Name:		do_alist
 Purpose:	Normal command to list areas and display area information.
 Called by:	interpreter(interp.c)
 ****************************************************************************/
void do_alist( CHAR_DATA *ch, char *argument )
{
    char buf	[ MAX_STRING_LENGTH ];
    char result [ MAX_STRING_LENGTH*3 ];	/* May need tweaking. */
    int count = 0;
    AREA_DATA *pArea;
    CHAR_DATA *rch;

    if ( !( rch = get_char( ch ) ) )
	return;
    
    if ( !authorized( rch, "alist" ) )
	return;

    sprintf( result, "[%3s] [%-27s] (%-5s-%5s) [%-10s] %3s [%-10s]\n\r",
       "Num", "Area Name", "lvnum", "uvnum", "Filename", "Sec", "Builders" );

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
	sprintf( buf, "[%3d] %-29.29s (%-5d-%5d) %-12.12s [%d] [%-10.10s]\n\r",
	     pArea->vnum,
	     &pArea->name[8],
	     pArea->lvnum,
	     pArea->uvnum,
	     pArea->filename,
	     pArea->security,
	     pArea->builders );
        ++count;
	     strcat( result, buf );
             if( count >= 50 ) {
                 send_to_char(result, ch);
                 count = 0;
                 result[0] = '\0';
             }
    }

    send_to_char( result, ch );
    return;
}
