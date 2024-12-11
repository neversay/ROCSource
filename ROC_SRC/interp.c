/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.			   *
 *									   *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and	   *
 *  Mitchell Tse.							   *
 *									   *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.	   *
 *									   *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in	   *
 *  'license.txt', as well as the Envy license in 'license.nvy'.	   *
 *  In particular, you may not remove either of these copyright notices.   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

#if defined( macintosh )
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
#include "ansi.h"

extern	void send_to_all_char args( ( const char *text ) );
extern bool	check_social	
args( ( CHAR_DATA *ch, char *command, char *argument ) );
extern bool show_position_to_char( CHAR_DATA *ch );
void	do_chatemote	args( ( CHAR_DATA *ch, char *argument) );
void	set_fighting	     args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void do_clanchatemote( CHAR_DATA *ch, char *argument );
void do_clanrecall( CHAR_DATA *ch, char *argument );
void do_immtalkemote( CHAR_DATA *ch, char *argument); // add by jye 0940604    
bool immtalk_social( CHAR_DATA *ch, char *command, char *argument ); // add by jye 0940604    
void immact( const char *format, CHAR_DATA *ch, const void *arg1,const void *arg2, int type ); // add by jye 0940604
/*
 * Command logging types.
 */
#define LOG_NORMAL		0
#define LOG_ALWAYS		1
#define LOG_NEVER		2

/*
 * God Levels - Check them out in merc.h
*/

#define L_HER			LEVEL_HERO

extern bool	fLogAll;

extern const struct	cmd_type	cmd_table[]; 
extern const struct ocmd_type	ocmd_table[];
extern const struct	rcmd_type	rcmd_table[];


/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
	char command [ MAX_INPUT_LENGTH ];
	char logline [ MAX_INPUT_LENGTH ];
	int	 cmd;
	int	 trust;
	bool found;

	if( ch->deleted ) return;
	if( !ch->in_room ) return;
    /*
     * Strip leading spaces.
     */
	while ( isspace( *argument ) )
		argument++;
	if ( argument[0] == '\0' )
		return;

	if( ch->deleted ) return;
    /*
     * No hiding and remove AFK
     */
	// REMOVE_BIT( ch->affected_by, AFF_HIDE );
	if ( !IS_NPC( ch ) )
		REMOVE_BIT( ch->act, PLR_AFK );


    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *	 also no spaces needed after punctuation.
     */
	strcpy( logline, argument );
	if ( !isalpha( argument[0] ) && !isdigit( argument[0] ) )
	{
		command[0] = argument[0];
		command[1] = '\0';
		argument++;
		while ( isspace( *argument ) )
			argument++;
	}
	else
	{
		argument = one_argument( argument, command );
	}


    /*
     * Implement freeze command.
     */
	if ( str_cmp( command, "quit") && !IS_NPC( ch ) && IS_SET( ch->act, PLR_FREEZE ) )
	{
		send_to_char( "§A§¹¥þµLªk¦æ°Ê¡I\n\r", ch );
		return;
	}

    /*
     * Look for command in command table.
     */

	found = FALSE;
	trust = get_trust( ch );
    /* room command hack
     * by zcecil
     */
	/*
		if( ch->in_room &&( ch->in_room->progtypes & ROOM_MASK)){
			if( rprog_mask_trigger(ch->in_room, ch, logline))
				return;
		}
	*/
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( command[0] == cmd_table[cmd].name[0]
			&& !str_prefix( command, cmd_table[cmd].name )
			&& ( cmd_table[cmd].level <= trust ) )
		{

			//if( ch->ordered == TRUE && IS_NPC( ch ) && IS_AFFECTED( ch, AFF_SUMMONED )
			if( ch->ordered == TRUE && IS_NPC( ch ) )
			{
				// ªýÀÉ order ¹ï mob ¤U skilldrv 2022/10/27
				if( !str_cmp( cmd_table[cmd].name, "skilldrv" ) ){
					ch->ordered = FALSE;
					break;
				}
				// summoned mob ¤£¯à¥Îªº§Þ¯à
				if( IS_AFFECTED( ch, AFF_SUMMONED )
					&& (   !str_cmp( cmd_table[cmd].name, "backstab" )
						|| !str_cmp( cmd_table[cmd].name, "empslash" )
						|| !str_cmp( cmd_table[cmd].name, "circle" )
						|| !str_cmp( cmd_table[cmd].name, "berserk" )
						|| !str_cmp( cmd_table[cmd].name, "change fighting" )
						|| !str_cmp( cmd_table[cmd].name, "kick" )
						|| !str_cmp( cmd_table[cmd].name, "assassination" )
						|| !str_cmp( cmd_table[cmd].name, "frenzy" )
						|| !str_cmp( cmd_table[cmd].name, "bashhead" )
						|| !str_cmp( cmd_table[cmd].name, "disarm" )
						|| !str_cmp( cmd_table[cmd].name, "barehand" )
						|| !str_cmp( cmd_table[cmd].name, "circular chop" )
						|| !str_cmp( cmd_table[cmd].name, "flaming slash" )
						|| !str_cmp( cmd_table[cmd].name, "storm slash" )
						|| !str_cmp( cmd_table[cmd].name, "sprinkle slash" )
						|| !str_cmp( cmd_table[cmd].name, "earth slash" )
						|| !str_cmp( cmd_table[cmd].name, "cast" )
						|| !str_cmp( cmd_table[cmd].name, "snaek" )
						|| !str_cmp( cmd_table[cmd].name, "invoke" )
						|| !str_cmp( cmd_table[cmd].name, "pick" ) ) )
				{
					if (IsSorGateMobByChar(ch)) {
						int nMobIndex = ch->pIndexData->vnum;
						if (nMobIndex == MOB_VNUM_SOR_WATER2) {
							if (str_cmp( cmd_table[cmd].name, "cast") == 0) {
								// setting ch->ordered = FALSE in do_cast
								found = TRUE;
								break;
							}
						}else if (nMobIndex == MOB_VNUM_SOR_LIGHTNING2){
							if (str_cmp( cmd_table[cmd].name, "berserk") == 0) {
								found = TRUE;
								ch->ordered = FALSE;
							}
						}
					}
					ch->ordered = FALSE;
					break;
				}
			}

			ch->ordered = FALSE;
			found = TRUE;
			break;
		}
	}

    /*
     * Log and snoop.
     */
	if ( cmd_table[cmd].log == LOG_NEVER )
		strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXXXX" );

	//if ( ch->trust > 42 || IS_IMMORTAL( ch ) || ( ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_LOG ) ) )
	//	|| ( fLogAll && !IS_NPC( ch ) )
	//	|| cmd_table[cmd].log == LOG_ALWAYS )
	if ( ( !IS_NPC( ch ) && ( IS_SET( ch->act, PLR_LOG ) || fLogAll ) ) || cmd_table[cmd].log == LOG_ALWAYS )
	{
		sprintf( log_buf, "Log %s: %s", ch->name, logline );
		// ±N log ¤]Åã¥Ü©ó bug ¤¤ 2023/01/04
		if( !IS_NPC( ch ) && ( IS_SET( ch->act, PLR_LOG ) || fLogAll ) )
			bug( log_buf , 0 );
		log_string( log_buf );
	}

	if ( ch->desc && ch->desc->snoop_by )
	{
		// ±N¤U«ü¥Oªº¨¤¦â¦W¥N¤J snoop_by 2023/01/17
		sprintf( log_buf, "[36m%s: %s[m", ch->name, logline );
		write_to_buffer( ch->desc->snoop_by, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by, log_buf, 0 );
		write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
	}

	if ( !found )
	{
        /* obj command hack 
         * by Amethyst */
		char final [ MAX_INPUT_LENGTH ];
		char finalline [ MAX_INPUT_LENGTH ];
		char objname [ MAX_INPUT_LENGTH ];
		OBJ_DATA *obj;

		one_argument( argument, objname );

		if ( ch->in_room )
		{
			if ( ( objname[0] != '\0' ) && ( obj = get_obj_here( ch, objname ) ) && can_see_obj( ch, obj ) )
			{
				argument = one_argument( argument, objname );
				strcpy( finalline, argument );
				sprintf( final, "%s %s", command, finalline);
				// §â ch->in_room ²¾¨ì³Ì«e­± 2023/01/18
				//if( ch->in_room &&( obj->pIndexData->progtypes & OBJ_COMMAND) )
				if( obj->pIndexData->progtypes & OBJ_COMMAND )
				{
					if(  can_see_obj( ch, obj ) && oprog_cmd_trigger(obj, ch, final) )
						return;
				}
			}

			/* room command hack
			* by zcecil*/
			// §â ch->in_room ²¾¨ì³Ì«e­± 2023/01/18
			//if( ch->in_room &&( ch->in_room->progtypes & ROOM_COMMAND) )
			if( ch->in_room->progtypes & ROOM_COMMAND )
			{
				if( rprog_cmd_trigger(ch->in_room, ch, logline) )
					return;
			}
		}
    
        /*
         * Look for command in socials table.
         */
		if ( !check_social( ch, command, argument ) )
			send_to_char( "¤°»ò¡H\n\r", ch );
		return;
	}

	update_pos(ch);

	if( IS_AFFECTED(ch, AFF_PARALYZED ) )
		ch->position = POS_PARALYZED;

	one_argument(argument, command);
	if( command[0] == '-' && !str_cmp(command, "-?") ) {
		do_help(ch, cmd_table[cmd].name);
		return;
	}
    /*
     * Character not in position for command?
     */
	if ( ch->position < cmd_table[cmd].position )
	{
		show_position_to_char(ch);
		// ¦pªG npc ¦³ death_prog «h¦º«e¤U«ü¥O¤£¨ü¼vÅT 2022/12/29
		if ( IS_NPC(ch) && ch->position == POS_DEAD && ( ch->pIndexData->progtypes & DEATH_PROG ) )
			send_to_char("mob death_prog trigger.\n\r", ch);
		else
			return;
	}

	if( ch->in_room &&( ch->in_room->progtypes & ROOM_MASK)){
		char mask_buf[MAX_INPUT_LENGTH *2];
		sprintf(mask_buf, "%s %s", cmd_table[cmd].name, argument);
		if( rprog_mask_trigger(ch->in_room, ch, mask_buf) )
			return;
	}

    /*
     * Dispatch the command.
     */
	(*cmd_table[cmd].do_fun) ( ch, argument );

	// ²¾°£ forced ª¬ºA 2022/10/27
	if( ch->forced )
		ch->forced = FALSE;

	tail_chain( );
	return;
}



void ointerpret( OBJ_DATA *obj, CHAR_DATA *ch, char *argument )
{
    char command [ MAX_INPUT_LENGTH ];
    char logline [ MAX_INPUT_LENGTH ];
    int	 cmd;
    bool found;

    /*
     * Strip leading spaces.
     */
    while ( isspace( *argument ) )
		argument++;
    if ( argument[0] == '\0' )
		return;

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *	 also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha( argument[0] ) && !isdigit( argument[0] ) )
    {
		command[0] = argument[0];
		command[1] = '\0';
		argument++;
		while ( isspace( *argument ) )
			argument++;
    }
    else
    {
		argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    for ( cmd = 0; ocmd_table[cmd].name[0] != '\0'; cmd++ )
    {
		if ( command[0] == ocmd_table[cmd].name[0]
			&& !str_prefix( command, ocmd_table[cmd].name ) )
		{
			found = TRUE;
			break;
		}
    }

    if ( !found )
    {
		bug( "Obj command not exist. Obj: %d", obj->pIndexData->vnum );
		bugf( "Obj error command : %s", command );
		return;
    }
    /*
     * Dispatch the command.
     *
    if ( !ch )
    {
	if ( char_list )
	{
		ch = char_list;
	}
	else
		return;
    }
*/
   (*ocmd_table[cmd].do_ofun) ( obj, ch, argument );

    tail_chain( );
    return;
}

/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void rinterpret( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument )
{
    char command [ MAX_INPUT_LENGTH ];
    char logline [ MAX_INPUT_LENGTH ];
    int	 cmd;
    bool found;

    /*
     * Strip leading spaces.
     */
    while ( isspace( *argument ) )
		argument++;
    if ( argument[0] == '\0' )
		return;

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *	 also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha( argument[0] ) && !isdigit( argument[0] ) )
    {
		command[0] = argument[0];
		command[1] = '\0';
		argument++;
		while ( isspace( *argument ) )
			argument++;
    }
    else
    {
		argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    for ( cmd = 0; rcmd_table[cmd].name[0] != '\0'; cmd++ )
    {
		if ( command[0] == rcmd_table[cmd].name[0]
			&& !str_prefix( command, rcmd_table[cmd].name ) )
		{
			found = TRUE;
			break;
		}
    }

    if ( !found )
    {
		bug( " Room command not exist. Room: %d", room->vnum );
		bugf( " Room command not exist. command: %s", command );
		return;
    }
    /*
     * Dispatch the command.
     *
    if ( !ch )
    {
	if ( char_list )
	{
		ch = char_list;
	}
	else
		return;
    }
*/
	(*rcmd_table[cmd].do_rfun) ( room, ch, argument );

    tail_chain( );
    return;
}


void zact( const char *format, CHAR_DATA *ch, const void *arg1,
	 const void *arg2, int type )
{
	   OBJ_DATA	   *obj1	= (OBJ_DATA  *) arg1;
	   DESCRIPTOR_DATA *d;
	   OBJ_DATA	   *obj2	= (OBJ_DATA  *) arg2;
	   CHAR_DATA	   *to;
	   CHAR_DATA	   *vch		= (CHAR_DATA *) arg2;
    static char *    const  he_she  [ ] = { "¥¦",  "¥L",  "¦o" };
    static char *    const  him_her [ ] = { "¥¦",  "¥L",  "¦o" };
    static char *    const  his_her [ ] = { "¥¦ªº","¥Lªº","¦oªº" };
    const  char		   *str;
    const  char		   *i;
	   char		   *point;
	   char		    buf	    [ MAX_STRING_LENGTH * 2];
	   char		    buf1    [ MAX_STRING_LENGTH * 2];
	   char		    fname   [ MAX_INPUT_LENGTH	];
	   bool		    fChinese;

    /*
     * Discard null and zero-length messages.
     */
    if ( !format || format[0] == '\0' )
	return;

	// ­×¥¿ social_table µLªk§P©w != "" , ·|³y¦¨Åã¥Üchat µL¤º®eªº±¡ªp 2021/09/21
	if( strlen(format) <= 12 ) return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( !vch )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    sprintf( buf1, "Bad act string:  %s", format );
	    bug( buf1, 0 );
	    return;
	}
		if ( !vch->in_room )//amenda 6/2/01 for debug purpose
		{
			bug( "interp.c, act:vch not in any room", 0 );
			bug( vch->name, 0 );
			return;	
		}
	to = vch->in_room->people;
    }

    for ( d = descriptor_list; d; d = d->next ) 
    {
	to = (d->original) ? d->original : d->character;
	if( !to ) continue;
	if ( ( to->deleted )
	    || ( !to->desc && IS_NPC( to ) )
	    || !IS_AWAKE( to ) 
	    || IS_SET( to->deaf, CHANNEL_CHAT ) )
	    continue;

	point	     = buf;
	*point++ = '';
	*point++ = '[';
	*point++ = '1';
	*point++ = ';';
	*point++ = '3';
	*point++ = '6';
	*point++ = 'm';
	str	   = format;
	fChinese=FALSE;
	while ( *str != '\0' )
	{
	    if ( *str != '$' || fChinese )
	    {
			fChinese = is_chinese_char(*str, fChinese);
		//if ( fChinese )
		//    fChinese=FALSE;
		//else if ( *str>='\x80' && *str<='\xff' )
		//    fChinese=TRUE;
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( !arg2 && *str >= 'A' && *str <= 'Z' )
	    {
		bug( "ZAct: missing arg2 for code %d.", *str );
		sprintf( buf1, "ZAct Bad act string:	 %s", format );
		bug( buf1, 0 );
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "ZAct: bad code %d.", *str );
			  sprintf( buf1, "ZAct Bad act string:  %s", format );
			  bug( buf1, 0 );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				    break;
		case 'T': i = (char *) arg2;				      break;
		case 'n': i = PERS( ch,	 to  );				       break;
		case 'N': i = PERS( vch, to  );				       break;
		case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];	  break;
		case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];	  break;
		case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];	  break;
		case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];	  break;
		case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];	  break;
		case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];	  break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( strcpy(name_buf, obj1->short_descr) )
			    : "¤£©úª««~";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( strcpy(name_buf, obj2->short_descr) )
			    : "¤£©úª««~";
		    break;

		case 'd':
		    if ( !arg2 || ( (char *) arg2 )[0] == '\0' )
		    {
			i = "ªù";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		case '$':
		    i = "$";
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	*point++ = '';
	*point++ = '[';
	*point++ = '1';
	*point++ = ';';
	*point++ = '3';
	*point++ = '6';
	*point++ = 'm';
	}

	*point++ = '';
	*point++ = '[';
	*point++ = '0';
	*point++ = 'm';
	*point++ = '\n';
	*point++ = '\r';
	*point++ = '\0';
	buf[0]	 = UPPER( buf[0] );
	if ( to->desc )
	//    write_to_buffer( to->desc, buf, point - buf );
            send_to_char( buf, to );
	/*
	if (MOBtrigger)
	    mprog_act_trigger( buf, to, ch, obj1, vch );
	*/
    }

    //MOBtrigger = TRUE;
    return;
}


/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    if ( *arg == '+' || *arg == '-' )
	arg++;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit( *arg ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
	char *pdot;
	int	  number;

	for ( pdot = argument; *pdot != '\0'; pdot++ )
	{
		if ( *pdot == '.' )
		{
			*pdot = '\0';
			number = atoi( argument );
			*pdot = '.';
			strcpy( arg, pdot+1 );
			return number;
		}
	}

	strcpy( arg, argument );
	return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
	char cEnd;
	bool fChinese;
	fChinese=FALSE;

	while ( isspace( *argument ) )
		argument++;

	cEnd = ' ';
	if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

	while ( *argument != '\0' ){
		if ( *argument == cEnd ){
			argument++;
			break;
		}
		if ( fChinese ) {
			*arg_first = *argument;
			fChinese=FALSE;
		}else {	
			*arg_first = LOWER( *argument );
			fChinese = is_chinese_char(*argument, fChinese);
		}
		arg_first++;
		argument++;
	}
	*arg_first = '\0';

	while ( isspace( *argument ) )
	argument++;

	return argument;
}

bool chat_social( CHAR_DATA *ch, char *command, char *argument )
{
    CHAR_DATA *victim;
    char       arg [ MAX_INPUT_LENGTH ];
    char       arg1[ MAX_INPUT_LENGTH ];
	char       buf[ MAX_INPUT_LENGTH ];
    int	       cmd = 0;
    bool       found = FALSE;
    bool    fEmote = FALSE;
    strcpy(arg1, "¡i¶¢²á¡j");
	
	found  = FALSE;

	if( strlen(argument) > 130 ) {
		send_to_char("¿é¤J¹Lªø.\n\r", ch);
		return FALSE;
	}

	if( !str_cmp( "emote", command)){
		found = TRUE;
		fEmote = TRUE;
	}

	if( !found )
		for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
		{
			if ( command[0] == social_table[cmd].name[0]
				&& !str_prefix( command, social_table[cmd].name ) )
			{
				found = TRUE;
				break;
			}
		}

	if ( !found ) return FALSE;

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
	{
		send_to_char( "§AµLªk¨Ï¥Î¥ô¦óªí±¡«ü¥O¡I\n\r", ch );
		return TRUE;
	}

	switch ( ch->position )
	{
		case POS_DEAD:
			send_to_char( "ÀRÀR½öµÛ§a¡A§A¤w¸g¦º¤F.\n\r", ch );
			return TRUE;

		case POS_INCAP:
		case POS_MORTAL:
			send_to_char( "§Aªºª¬ªp¤ÓÁV¤F.\n\r", ch );
			return TRUE;

		case POS_STUNNED:
			send_to_char( "§AÀY©ü²´ªá.\n\r",    ch );
			return TRUE;

		case POS_SLEEPING:
			send_to_char( "§A¥¿¦bºÎ¹Ú¤¤¡A½Ð¥ý wake °_¨Ó¡C\n\r",	    ch );
			return TRUE;
	}

    if( fEmote ) {
		if( argument[0] == '\0' )
			zact("¡i¶¢²á¡j$n¬Ý°_¨Óªí±¡Â×´I¡C", ch, NULL, argument, TO_ROOM); // Changed by Amethyst
		else
			zact("¡i¶¢²á¡j$n$T¡C", ch, NULL, argument, TO_ROOM); // Changed by Amethyst
		return TRUE;
    }

	one_argument( argument, arg );
	victim = NULL;
	if ( arg[0] == '\0' )
	{
		zact( strcat(arg1, social_table[cmd].others_no_arg), ch, NULL, victim, TO_ROOM );
		/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
		// Changed by Amethyst
		if ( social_table[cmd].others_no_arg != "" )
			zact( strcat(arg1, social_table[cmd].others_no_arg), ch, NULL, victim, TO_ROOM	   );
		else
			send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		*/
		return TRUE;
	}
    else if ( !( victim = get_char_world( ch, arg ) ) )
    {
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",			  ch );
		return TRUE;
	}
/*
    if( victim->in_room->area->recall != ch->in_room->area->recall &&
     !IS_IMMORTAL(ch)
    && !IS_IMMORTAL(victim)){
	send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",			  ch );
	return TRUE;
    }
*/   
	if ( victim == ch ){
		zact( strcat(arg1, social_table[cmd].others_auto),   ch, NULL, victim, TO_ROOM );
		/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
		//Changed by Amethyst
		if ( social_table[cmd].others_auto != "" ){
			zact( strcat(arg1, social_table[cmd].others_auto),   ch, NULL, victim, TO_ROOM	   );
		}
		else
			send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		*/
    }else if ( !get_char_room( ch, arg ) && can_see( ch, victim ) ){
		if ( !IS_NPC( victim ) )
		{
			ROOM_INDEX_DATA *original;
			original = ch->in_room;
			zact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
			/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
			// Changed by Amethyst
			if ( social_table[cmd].others_found != "" ){
				zact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
			}
			else
				send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
			*/
		}else{
			send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",		  ch );
		}
    }else{
		zact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
		/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
		// Changed by Amethyst
		if ( social_table[cmd].others_found != "" )
			zact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
		else
			send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		*/
    }
    return TRUE;
}

void do_chatemote(CHAR_DATA *ch, char *argument)
{
    char command[ MAX_INPUT_LENGTH ];

    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_SUMMONED )
    || IS_AFFECTED( ch, AFF_CHARM ) ) )
        return;



    if( !argument || argument[0] == '\0') return;

    argument = one_argument(argument, command);
    
    if( !command || command[0] == '\0' ) return;
    
//    sprintf(buf,"[%s][%s]\n\r", command, argument);
    //send_to_char(buf, ch);
    chat_social(ch, command, argument);
    return;
}

// add by jye 0940604  immÀW¹D°Ê§@
void do_immtalkemote(CHAR_DATA *ch, char *argument)
{
    char command[ MAX_INPUT_LENGTH ];

    if( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_SUMMONED )
        || IS_AFFECTED( ch, AFF_CHARM ) ) )
        return;

    if( !IS_IMMORTAL( ch )) return; 

    if( !argument || argument[0] == '\0') return;

    argument = one_argument(argument, command);
    
    if( !command || command[0] == '\0' ) return;
    
    immtalk_social(ch, command, argument);
    return;
}

// add by jye 0940604  immÀW¹D°Ê§@
bool immtalk_social( CHAR_DATA *ch, char *command, char *argument )
{
    CHAR_DATA *victim;
    char       arg [ MAX_INPUT_LENGTH ];
	  char       arg1[ MAX_INPUT_LENGTH ];
    int	       cmd = 0;
    bool       found = FALSE;
    bool    fEmote = FALSE;
    strcpy(arg1, "¡i¯«¿Ù¡j");

    found  = FALSE;
    if( strlen(argument) > 130 ) {
        send_to_char("¿é¤J¹Lªø.\n\r", ch);
        return FALSE;
    }
    if( !str_cmp( "emote", command)){
	      found = TRUE;
	      fEmote = TRUE;
    }
    if( !found )
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	      if ( command[0] == social_table[cmd].name[0]
	          && !str_prefix( command, social_table[cmd].name ) )
	      {
	          found = TRUE;
	          break;
	      }
    }

    if ( !found ) return FALSE;

    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
    {
	    send_to_char( "§AµLªk¨Ï¥Î¥ô¦óªí±¡«ü¥O¡I\n\r", ch );
	    return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	         send_to_char( "ÀRÀR½öµÛ§a¡A§A¤w¸g¦º¤F.\n\r", ch );
	         return TRUE;
    case POS_INCAP:
    case POS_MORTAL:
           send_to_char( "§Aªºª¬ªp¤ÓÁV¤F.\n\r", ch );
           return TRUE;
    case POS_STUNNED:
	         send_to_char( "§AÀY©ü²´ªá.\n\r",    ch );
	         return TRUE;
    case POS_SLEEPING:
           send_to_char( "§A¥¿¦bºÎ¹Ú¤¤¡A½Ð¥ý wake °_¨Ó¡C\n\r",	    ch );
           return TRUE;
    }

    if( fEmote ) {
	      if( argument[0] == '\0' )
	      	immact("¡i¯«¿Ù¡j$n¬Ý°_¨Óªí±¡Â×´I¡C", ch, NULL, argument, TO_ROOM); // Changed by Amethyst
	      else
	      	immact("¡i¯«¿Ù¡j$n$T¡C", ch, NULL, argument, TO_ROOM); // Changed by Amethyst
	      return TRUE;
    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
		immact( strcat(arg1, social_table[cmd].others_no_arg), ch, NULL, victim, TO_ROOM );
		/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
		  // Changed by Amethyst
		  if ( social_table[cmd].others_no_arg != "" )
		  	immact( strcat(arg1, social_table[cmd].others_no_arg), ch, NULL, victim, TO_ROOM );
		  else
		  	send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		*/
		return TRUE;
    }
    else if ( !( victim = get_char_world( ch, arg ) ) )
    {
	    send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",			  ch );
	    return TRUE;
    }

    if ( victim == ch )
    {
		immact( strcat(arg1, social_table[cmd].others_auto),   ch, NULL, victim, TO_ROOM	   );
		/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
	    //Changed by Amethyst
		  if ( social_table[cmd].others_auto != "" )
		  	immact( strcat(arg1, social_table[cmd].others_auto),   ch, NULL, victim, TO_ROOM	   );
		  else
		  	send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		*/
    }
    else if ( !get_char_room( ch, arg ) && can_see( ch, victim ) )
    {
	    if ( !IS_NPC( victim ) )
	    {
	      
	      ROOM_INDEX_DATA *original;
      
	      original = ch->in_room;
		  
		  immact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
		  /*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
	      // Changed by Amethyst
	    	if ( social_table[cmd].others_found != "" )
	    		immact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
	    	else
	    		send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		  */
	    }
	    else
	    {
	      send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",		  ch );
	    }
    }
    else
    {
		immact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
		/*  != "" §P©w¤£§@¥Î , ²¾°£ 2021/09/21
	    // Changed by Amethyst
		  if ( social_table[cmd].others_found != "" )
		  	immact( strcat(arg1, social_table[cmd].others_found),  ch, NULL, victim, TO_NOTVICT );
		  else
		  	send_to_char( "§A¤£¯à³o¼Ë°µ.\n\r",    ch );
		*/
    }
    return TRUE;
}

// add by jye 0940604  immÀW¹D°Ê§@
void immact( const char *format, CHAR_DATA *ch, const void *arg1,
	 const void *arg2, int type )
{
	  OBJ_DATA	   *obj1	= (OBJ_DATA  *) arg1;
	  DESCRIPTOR_DATA *d;
	  OBJ_DATA	   *obj2	= (OBJ_DATA  *) arg2;
	  CHAR_DATA	   *to;
	  CHAR_DATA	   *vch		= (CHAR_DATA *) arg2;
    static char *    const  he_she  [ ] = { "¥¦",  "¥L",  "¦o" };
    static char *    const  him_her [ ] = { "¥¦",  "¥L",  "¦o" };
    static char *    const  his_her [ ] = { "¥¦ªº","¥Lªº","¦oªº" };
    const  char		   *str;
    const  char		   *i;
	  char		   *point;
	  char		    buf	    [ MAX_STRING_LENGTH * 2];
	  char		    buf1    [ MAX_STRING_LENGTH * 2];
	  char		    fname   [ MAX_INPUT_LENGTH	];
	  bool		    fChinese;

    /*
     * Discard null and zero-length messages.
     */
    if ( !format || format[0] == '\0' ) return;

	// ­×¥¿ social_table µLªk§P©w != "" , ·|³y¦¨Åã¥Üchat µL¤º®eªº±¡ªp 2021/09/21
	if( strlen(format) <= 12 ) return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	     if ( !vch )
	     {
	         bug( "Act: null vch with TO_VICT.", 0 );
	         sprintf( buf1, "Bad act string:  %s", format );
	         bug( buf1, 0 );
	         return;
	     }
		   if ( !vch->in_room )//amenda 6/2/01 for debug purpose
		   {
		   	 bug( "interp.c, act:vch not in any room", 0 );
		   	 bug( vch->name, 0 );
		   	 return;	
		   }
	     to = vch->in_room->people;
    }

    for ( d = descriptor_list; d; d = d->next ) 
    {
	    to = (d->original) ? d->original : d->character;
	    if( !to ) continue;
	    if( !IS_IMMORTAL( to )) continue; 
	    
	    point	     = buf;
	    *point++ = '';
	    *point++ = '[';
	    *point++ = '1';
	    *point++ = ';';
	    *point++ = '3';
	    *point++ = '1';
	    *point++ = 'm';
	    
	    str	   = format;
	    fChinese=FALSE;
	    while ( *str != '\0' )
	    {
	        if ( *str != '$' || fChinese )
	        {
	    		  fChinese = is_chinese_char(*str, fChinese);
	    	    //if ( fChinese )
	    	    //    fChinese=FALSE;
	    	    //else if ( *str>='\x80' && *str<='\xff' )
	    	    //    fChinese=TRUE;
	    	    *point++ = *str++;
	    	    continue;
	        }
	        ++str;
      
	        if ( !arg2 && *str >= 'A' && *str <= 'Z' )
	        {
	    	    bug( "Act: missing arg2 for code %d.", *str );
	    	    sprintf( buf1, "Bad act string:	 %s", format );
	    	    bug( buf1, 0 );
	    	    i = " <@@@> ";
	        }
	        else
	        {
	    	    switch ( *str )
	    	    {
	    	    default:  bug( "Act: bad code %d.", *str );
	    	    	  sprintf( buf1, "Bad act string:  %s", format );
	    	    	  bug( buf1, 0 );
	    	    	  i = " <@@@> ";				break;
	    	    /* Thx alex for 't' idea */
	    	    case 't': i = (char *) arg1;				    break;
	    	    case 'T': i = (char *) arg2;				      break;
	    	    case 'n': i = PERS( ch,	 to  );				       break;
	    	    case 'N': i = PERS( vch, to  );				       break;
	    	    case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];	  break;
	    	    case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];	  break;
	    	    case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];	  break;
	    	    case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];	  break;
	    	    case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];	  break;
	    	    case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];	  break;
            
	    	    case 'p':
	    	        i = can_see_obj( to, obj1 )
	    	    	    ? ( strcpy(name_buf, obj1->short_descr) )
	    	    	    : "¤£©úª««~";
	    	        break;
            
	    	    case 'P':
	    	        i = can_see_obj( to, obj2 )
	    	    	    ? ( strcpy(name_buf, obj2->short_descr) )
	    	    	    : "¤£©úª««~";
	    	        break;
            
	    	    case 'd':
	    	        if ( !arg2 || ( (char *) arg2 )[0] == '\0' )
	    	        {
	    	    	i = "ªù";
	    	        }
	    	        else
	    	        {
	    	    	one_argument( (char *) arg2, fname );
	    	    	i = fname;
	    	        }
	    	        break;
	    	    case '$':
	    	        i = "$";
	    	        break;
	    	    }
	        }
	    	
	        ++str;
	        while ( ( *point = *i ) != '\0' )
	    	++point, ++i;
	      *point++ = '';
	      *point++ = '[';
	      *point++ = '1';
	      *point++ = ';';
	      *point++ = '3';
	      *point++ = '1';
	      *point++ = 'm';
	    }
      
	    *point++ = '';
	    *point++ = '[';
	    *point++ = '0';
	    *point++ = 'm';
	    *point++ = '\n';
	    *point++ = '\r';
	    *point++ = '\0';
      
	    buf[0]	 = UPPER( buf[0] );
      if ( to->desc )
           send_to_char(buf , to );
	    /*
	    if (MOBtrigger)
	        mprog_act_trigger( buf, to, ch, obj1, vch );
	    */
    }

    //MOBtrigger = TRUE;
    return;
}
