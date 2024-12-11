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
 *  such installation can be found in INSTALL.	Enjoy...	 N'Atas-Ha *
 ***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "merc.h"
#include "rprog.h"

/*
 * Room command table. Made by Amenda
 */
const	struct	rcmd_type	rcmd_table	[ ] =
{
    { "echo",				do_rrecho			},
    { "echoat",				do_rechoat			},
    { "echoaround",			do_rechoaround		},
    { "nonechoat",			do_rnonechoat		},
    { "nonechoaround",		do_rnonechoaround	},
    { "echotoall",			do_rechotoall		},
    { "echotoarea",			do_rechotoarea		},
    { "modrvt",				do_modrvt			},
    { "modhmv",				do_modhmv			},
	{ "modhmvgroup" ,		do_modhmvgroup		},
    { "at",					do_rat				},
    { "cast",				do_rcast			},
    { "callout",			do_rcallout			},
    { "rcall",				do_rcall			},
    { "oload",				do_roload			},
    { "mload",				do_rmload			},
    { "add_affect",			do_add_affect		},
    { "del_affect",			do_del_affect		},
    { "force",				do_rforce			},
    { "transfer",			do_rtransfer		},
	{ "transfergroup" ,		do_rtransfergroup	}, /* add at 2018/07/18 */
    { "wtransfer",			do_rwtransfer		}, /* add at 2022/11/21 */
	{ "wtransfergroup" ,	do_rwtransfergroup	}, /* add at 2022/11/21 */
    { "makedoor",			do_rpmakedoor		},
    { "deldoor",			do_rpdeldoor		},
    { "setdoor",			do_rpsetdoor		},
    { "purge",				do_rpurge			},
    { "log",				do_rlog				},
    { "def_rembch",			do_def_rembch		},
    { "add_qmark",			do_radd_qmark		},
	{ "rpexp" ,				do_rpexp			},
	{ "rpgold" ,			do_rpgold			},
	{ "rpresp" ,			do_rpresp			},
	{ "rpalign" ,			do_rpalign			}, /* add at 20.02.25 */
	{ "rpskillexp" ,		do_rpskillexp		}, /* add at 20.02.25 */
	{ "rpmerit" ,			do_rpmerit			}, /* add at 20.02.25 */
	{ "rppractice" ,		do_rppractice		}, /* add at 20.02.25 */
	{ "greward" ,			do_rgreward			},
    { "add_all_qmark",		do_radd_all_qmark	},
    { "del_qmark",			do_rdel_qmark		},
    { "del_all_qmark",		do_rdel_all_qmark	},
    { "mod_qmark",			do_rmod_qmark		},
    { "mod_all_qmark",		do_rmod_all_qmark	},
    { "stun",				do_rpstun			}, // by Amethyst
	{ "setroom",			do_set_room			}, // by Amethyst
    { "", 0 }
};

/***************************************************************************
 * Local function code and brief comments.
 */

/* if you dont have these functions, you damn well should... */

#ifdef DUNNO_STRSTR
char * strstr(s1,s2) const char *s1; const char *s2;
{
  char *cp;
  int i,j=strlen(s1)-strlen(s2),k=strlen(s2);
  if(j<0)
    return NULL;
  for(i=0; i<=j && strncmp(s1++,s2, k)!=0; i++);
  return (i>j) ? NULL : (s1-1);
}
#endif

int 
rprog_name_to_type ( char *name )
{
   if ( !str_cmp( name, "room_enter"	 ) )	return ROOM_ENTER;
   if ( !str_cmp( name, "room_leave"	 ) )	return ROOM_LEAVE;
   if ( !str_cmp( name, "room_rand"	 ) )	return ROOM_RAND;
   if ( !str_cmp( name, "room_time"	 ) )	return ROOM_TIME;
   if ( !str_cmp( name, "room_command"	))	return ROOM_COMMAND;
   if ( !str_cmp( name, "room_mask"	))	return ROOM_MASK;
   if ( !str_cmp( name, "room_trap"	))	return ROOM_TRAP;
   if ( !str_cmp( name, "room_defun"	))	return ROOM_DEFUN;


   
   return( ERROR_PROG );
}


/* 
 * This procedure is responsible for reading any in_file ROOMprograms.
 */

void rprog_read_programs( FILE *fp, ROOM_INDEX_DATA *pRoomIndex)
{
  RPROG_DATA *rprg;
  char	      letter;
  bool	      done = FALSE;
  int	      stat;

  if ( ( letter = fread_letter( fp ) ) != '>' )
  {
      bug( "Load_rooms: vnum %d ROOMPROG char", pRoomIndex->vnum );
      exit( 1 );
  }
  pRoomIndex->roomprogs = (RPROG_DATA *)alloc_mem( sizeof( RPROG_DATA ) );
  rprg = pRoomIndex->roomprogs;

  while ( !done )
  {
    rprg->type = rprog_name_to_type( fread_word( fp, &stat ) );
    if ( rprg->type == ERROR_PROG )
    {
	bug( "Load_rooms: vnum %d ROOMPROG type.", pRoomIndex->vnum );
	exit( 1 );
    }
    else
    {
	rprg->next_in_list = rprg_list;
	rprg_list = rprg;
	rprg->owner = pRoomIndex;
	pRoomIndex->progtypes = pRoomIndex->progtypes | rprg->type;
	rprg->arglist	     = fread_string( fp, &stat );
	fread_to_eol( fp );
	rprg->comlist	     = fread_string( fp, &stat );
	fread_to_eol( fp );
	switch ( letter = fread_letter( fp ) )
	{
	  case '>':
	     rprg->next = ( RPROG_DATA *)alloc_mem( sizeof( RPROG_DATA ) );
	     rprg	= rprg->next;
	     rprg->next = NULL;
	   break;
	  case '|':
	     rprg->next = NULL;
	     fread_to_eol( fp );
	     done = TRUE;
	   break;
	  default:
	     bug( "Load_rooms: vnum %d bad ROOMPROG.", pRoomIndex->vnum );
	     exit( 1 );
	   break;
	}
    }
  }

  return;

}
/* This routine reads in scripts of ROOMprograms from a file */

RPROG_DATA* rprog_file_read( char *f, RPROG_DATA *rprg, ROOM_INDEX_DATA *pRoomIndex )
{
	char ROOMProgfile[ MAX_INPUT_LENGTH ];
	RPROG_DATA *rprg2;
	FILE *progfile;
	char letter;
	bool done = FALSE;
	int	stat;

	sprintf( ROOMProgfile, "%s%s", ROOM_DIR, f );

	progfile = fopen( ROOMProgfile, "r" );
	if ( !progfile )
	{
		bug( "Room: %d couldnt open roomprog file", pRoomIndex->vnum );
		exit( 1 );
	}

	rprg2 = rprg;
	switch ( letter = fread_letter( progfile ) )
	{
		case '>':
			break;
		case '|':
			bug( "empty roomprog file.", 0 );
			exit( 1 );
			break;
		default:
			bug( "in roomprog file syntax error.", 0 );
			exit( 1 );
			break;
	}

	while ( !done )
	{
		rprg2->type = rprog_name_to_type( fread_word( progfile, &stat ) );
		if ( rprg2->type == ERROR_PROG )
		{
			bug( "mobprog file type error", 0 );
			exit( 1 );
		}
		else
		{
			pRoomIndex->progtypes = pRoomIndex->progtypes | rprg2->type;
			rprg2->arglist = fread_string( progfile, &stat );
			rprg2->comlist = fread_string( progfile, &stat );
			switch ( letter = fread_letter( progfile ) )
			{
				case '>':
					rprg2->next = (RPROG_DATA *)alloc_mem( sizeof( RPROG_DATA ) );
					rprg2 = rprg2->next;
					rprg2->next = NULL;
					break;
				case '|':
					done = TRUE;
					break;
				default:
					bug( "in roomprog file syntax error.", 0 );
					exit( 1 );
					break;
			}
		}
	}
	fclose( progfile );
	return rprg2;
}

/* Snarf a MOBprogram section from the area file.
 */
void load_roomprogs( FILE *fp )
{
  ROOM_INDEX_DATA *iRoom;
  RPROG_DATA	 *original;
  RPROG_DATA	 *working;
  char		  letter;
  int		  value;
  int		  stat;

  for ( ; ; )
    switch ( letter = fread_letter( fp ) )
    {
    default:
      bug( "Load_roomprogs: bad command '%c'.",letter);
      exit(1);
      break;
    case 'S':
    case 's':
      fread_to_eol( fp ); 
      return;
    case '*':
      fread_to_eol( fp ); 
      break;
    case 'M':
    case 'm':
    case 'R':
    case 'r':
      value = fread_number( fp, &stat );
      if ( ( iRoom = get_room_index( value ) ) == NULL )
      {
	bug( "Load_roomprogs: vnum %d doesnt exist", value );
	exit( 1 );
      }
    
      /* Go to the end of the prog command list if other commands
	 exist */

      if ( ( original = iRoom->roomprogs ) )
	for ( ; original->next != NULL; original = original->next );

      working = (RPROG_DATA *)alloc_mem( sizeof( RPROG_DATA ) );
      if ( original )
	original->next = working;
      else
	iRoom->roomprogs = working;
      working	    = rprog_file_read( fread_word( fp, &stat ), working, iRoom );
      working->next = NULL;
      fread_to_eol( fp );
      break;
    }

  return;

}

bool 
rprog_wordlist_check( char *arg, ROOM_INDEX_DATA *room, CHAR_DATA *actor, int type )
{

	char temp1[ MAX_STRING_LENGTH ];
	char temp2[ MAX_INPUT_LENGTH ];
	char word[ MAX_INPUT_LENGTH ];
	char cmd[MAX_INPUT_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	RPROG_DATA *rprg = NULL;
	char *list;
	char *start;
	char *dupl;
	char *end;
	char *tmp;
	int i;
	bool fChinese;


	arg = one_argument(arg, cmd);
	one_argument(arg, arg1);

	for ( rprg = room->roomprogs; rprg != NULL; rprg = rprg->next ){
		if ( rprg->type == type )
		{
			//strcpy( temp1, rprg->arglist );
			tmp = one_argument(rprg->arglist, temp1);
			if( str_cmp(temp1, cmd ) ) continue;
			strcpy(temp1, tmp);
			list = temp1;
			fChinese=FALSE;
			for ( i = 0; i < strlen( list ); i++ ) {
				//if ( fChinese )
				//fChinese=FALSE;
				//else {
				if ( fChinese == FALSE ) list[i] = LOWER( list[i] );
				fChinese = is_chinese_char( list[i], fChinese );
				//if ( list[i]>='\x80' && list[i]<='\xff' )
				//    fChinese=TRUE;
				//}
			}
			strcpy( temp2, arg1);
			dupl = temp2;
			fChinese=FALSE;
			for ( i = 0; i < strlen( dupl ); i++ ) {
				//if ( fChinese )
				//fChinese=FALSE;
				//else {
				if (fChinese == FALSE ) dupl[i] = LOWER( dupl[i] );
				fChinese = is_chinese_char( dupl[i], fChinese );
				//if ( dupl[i]>='\x80' && list[i]<='\xff' )
				//    fChinese=TRUE;
				//}
			}
			if ( ( list[0] == 'p' ) && ( list[1] == ' ' ) )
			{
				list += 2;
				while ( ( start = strstr( dupl, list ) ) )
					if ( (start == dupl || *(start-1) == ' ' )
						&& ( *(end = start + strlen( list ) ) == ' '
						|| *end == '\n'
						|| *end == '\r'
						|| *end == '\0' /*NULL*/ ) )
					{
						rprog_driver( rprg->comlist, room, actor );
						return TRUE;
					}
					else
						dupl = start+1;
			}
			else
			{
				list = one_argument( list, word );
				for( ; word[0] != '\0'; list = one_argument( list, word ) ){
					while ( ( start = strstr( dupl, word ) ) ){
						if ( ( start == dupl || *(start-1) == ' ' )
							&& ( *(end = start + strlen( word ) ) == ' '
							|| *end == '\n'
							|| *end == '\r'
							|| *end == '\0' /*NULL*/ ) )
						{
							rprog_driver( rprg->comlist, room, actor );
							return TRUE;
							break;
						}
						else
							dupl = start+1;
					}
				}
			}
		}
	}

	return FALSE;

}

/***************************************************************************
 * Global function code and brief comments.
 */


void 
rprog_defun_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch ,char *txt )
{
    RPROG_DATA *rprg;
    char arg[MAX_INPUT_LENGTH];
    bool notfound = 1;


    for ( rprg = room->roomprogs; rprg != NULL; rprg = rprg->next )
    {
	if( rprg->type != ROOM_DEFUN ) continue;
	one_argument(rprg->arglist, arg);
	if( !str_cmp(arg, txt ) ){
	    rprog_driver( rprg->comlist, room, ch  );
            return ;
        }
    }
    if( notfound ) {
	bugf("no function %s defined in room %d", txt, room->vnum);
    }
}


bool rprog_mask_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch ,char *txt )
{
	RPROG_DATA *rprg;
	char arg[MAX_INPUT_LENGTH];
	char cmd[MAX_INPUT_LENGTH];
	char *tmp;
	char arg2[MAX_INPUT_LENGTH]; // add at 2022/01/07
	QMARK *qm; // add at 2022/01/07
	int qm_number; // add at 2022/01/07
	int ch_qm_value; // add at 2022/01/07

	if( !txt )
		return FALSE;

	if( ch && !ch->deleted && ch->forced )
	{
		ch->forced = FALSE;
		return FALSE;
	}

	// 針對 wizinv 的情況 2022/11/20
	if( !IS_NPC(ch) && IS_SET( ch->act, PLR_WIZINVIS) )
		return FALSE;

	if( !(room->progtypes & ROOM_MASK))
		return FALSE;
	tmp = one_argument(txt, cmd);
	/*
	switch(cmd[0]){
		case '2': strcpy(cmd, "south"); break;
		case '4': strcpy(cmd, "west");  break;
		case '6': strcpy(cmd, "east");  break;
		case '8': strcpy(cmd, "north"); break;
		case '3': strcpy(cmd, "down");  break;
		case '9': strcpy(cmd, "up");    break;
		case '5': strcpy(cmd, "look");	break;
	};*/

	one_argument(tmp, arg);

	for ( rprg = room->roomprogs; rprg != NULL; rprg = rprg->next ){
		if( rprg->type != ROOM_MASK)
			continue;
		tmp = one_argument(rprg->arglist, arg); //strip cmd
		if( !str_cmp(cmd, "all")) {
			rprog_driver( rprg->comlist, room, ch );
			return TRUE;
		}
		if( str_cmp(arg, cmd ) )
			continue;
		one_argument(tmp, arg); //real arg
		if( arg[0] != '\0' && !str_cmp(arg,"default") ){
			rprog_driver( rprg->comlist, room, ch );
			return TRUE;
		}
		tmp = one_argument(tmp, arg);
		one_argument(tmp , arg2);
		// qmark check 2022/01/07
		if( arg[0] != '\0' && !str_prefix( "qmark:" , arg ) ){
			// ch 帶有 qmark: 後字串的 qmark 時
			if( qm = is_qmark( ch , &arg[6] ) ){
				// qmark 要判定 v0 v1 v2 時
				if( arg2[0] != '\0' )
				{
					// ch qmark 轉換
					if( !str_prefix( "v0" , arg2 ) )
						ch_qm_value = qm->v0;
					if( !str_prefix( "v1" , arg2 ) )
						ch_qm_value = qm->v1;
					if( !str_prefix( "v2" , arg2 ) )
						ch_qm_value = qm->v2;
					// 判定數值是否與 ch qmark 相同
					if ( !str_prefix( "==" , &arg2[2]) ){
						qm_number = atoi( &arg2[4] );
						if ( ch_qm_value == qm_number )
							return FALSE;
					}
					if ( !str_prefix( "!=" , &arg2[2]) ){
						qm_number = atoi( &arg2[4] );
						if ( ch_qm_value != qm_number )
							return FALSE;
					}
					if ( !str_prefix( ">" , &arg2[2]) ){
						qm_number = atoi( &arg2[3] );
						if ( ch_qm_value > qm_number )
							return FALSE;
					}
					if ( !str_prefix( "<" , &arg2[2] ) ){
						qm_number = atoi( &arg2[3] );
						if ( ch_qm_value < qm_number )
							return FALSE;
					}
					if ( !str_prefix( "<=" , &arg2[2] ) || !str_prefix( "=<" , &arg2[2] ) ){
						qm_number = atoi( &arg2[4] );
						if ( ( ch_qm_value < qm_number ) || ( ch_qm_value == qm_number ) )
							return FALSE;
					}
					if ( !str_prefix( ">=" , &arg2[2] ) || !str_prefix( "=>" , &arg2[2] ) ){
						qm_number = atoi( &arg2[4] );
						if ( ch_qm_value >= qm_number )
							return FALSE;
					}
					if ( !str_prefix( "&" , &arg2[2] ) ){
						qm_number = atoi( &arg2[3] );
						if ( ( ch_qm_value & qm_number ) != 0 )
							return FALSE;
					}
					if ( !str_prefix( "!&" , &arg2[2] ) ){
						qm_number = atoi( &arg2[4] );
						if ( !( ch_qm_value & qm_number ) )
							return FALSE;
					}
					// 沒跑到判定成立就執行 room_mask
					rprog_driver( rprg->comlist, room, ch );
					return TRUE;
				}else{
					// qmark 不判定 v0 v1 v2 等數值時, room_mask 就不成立
					return FALSE;
				}
			}else{
				rprog_driver( rprg->comlist, room, ch );
				return TRUE;
			}
		}
	}
	return rprog_wordlist_check( txt, room, ch, ROOM_MASK );
}

bool rprog_cmd_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch ,char *txt )
{
    RPROG_DATA *rprg;
    char arg[MAX_INPUT_LENGTH];
    char cmd[MAX_INPUT_LENGTH];
    char *tmp;

    if( !txt ) return FALSE;

    if( !(room->progtypes & ROOM_COMMAND)) return FALSE;

	if( (rprog_wordlist_check( txt, room, ch,   ROOM_COMMAND) ) )
		return TRUE;

	tmp = one_argument(txt, cmd);
    one_argument(tmp, arg);
    //if( arg[0] == '\0' || !str_cmp(arg, "default")){
        for ( rprg = room->roomprogs; rprg != NULL; rprg = rprg->next )
            {
		if( rprg->type != ROOM_COMMAND ) continue;
                tmp = one_argument(rprg->arglist, arg); //strip cmd
		if( str_cmp(arg, cmd ) )
		    continue;
		one_argument(tmp, arg); //real arg
                if( arg[0] != '\0' && !str_cmp(arg,"default") ){
                    rprog_driver( rprg->comlist, room, ch  );
		    return TRUE;
		}
            }
		return FALSE;
}


void rprog_driver ( char *com_list, ROOM_INDEX_DATA *room, CHAR_DATA *actor )
{
	char tmpcmndlst[ MAX_STRING_LENGTH*2 ];
	char buf[ MAX_INPUT_LENGTH ];
	char *morebuf;
	char *command_list;
	char *cmnd;
	CHAR_DATA *rndm = NULL;
	CHAR_DATA *vch = NULL;
	int count = 0;

	/* get a random visable mortal player who is in the room with the mob */
	// if ( !( room->people ) )
	//	bug ( "Warning: Room %d has no any people in it, but have room_trigger.", room->vnum );
	// else
	for ( vch = room->people; vch; vch = vch->next_in_room )
	{
		if ( !vch->in_room || vch->deleted )
			continue;
		if ( !IS_NPC( vch ) && !IS_SET( vch->act, PLR_WIZINVIS ) )
			{
				if ( number_range( 0, count ) == 0 )
				rndm = vch;
				count++;
			}
	}
	strcpy( tmpcmndlst, com_list );
	command_list = tmpcmndlst;
	cmnd = command_list;
	command_list = mprog_next_command( command_list );
	while ( *cmnd != '\0' )
	{
		morebuf = one_argument( cmnd, buf );
		if ( !str_cmp( buf, "if" ) )
			command_list = rprog_process_if( morebuf, command_list, room, actor, rndm );
		else
			rprog_process_cmnd( cmnd, room, actor, rndm );
			cmnd = command_list;
			command_list = mprog_next_command( command_list );
	}
	return;
}

char *rprog_process_if( char *ifchck, char *com_list, ROOM_INDEX_DATA *room, CHAR_DATA *actor, CHAR_DATA *rndm )
{

 static char null[ 1 ];
 char buf[ MAX_INPUT_LENGTH ];
 char *morebuf = '\0';
 char	 *cmnd = '\0';
 bool loopdone = FALSE;
 bool	  flag = FALSE;

 *null = '\0';

 if ( actor && actor->deleted )
 {
 	bug( "Rprog_process_if: Actor deleted!", 0 );
 	bug( actor->name, 0 );
 	return null;
 }

 /* check for trueness of the ifcheck */
 if ( mprog_do_ifchck( ifchck, NULL, actor, NULL, room, rndm, room->rrembch ) == 1 )
     flag = TRUE;

 while( loopdone == FALSE ) /*scan over any existing or statements */
 {
     cmnd     = com_list;
     com_list = mprog_next_command( com_list );
     while ( *cmnd == ' ' )
       cmnd++;
     if ( *cmnd == '\0' )
     {
	 bug ( "Room: %d no commands after IF/OR", room->vnum ); 
	 return null;
     }
     morebuf = one_argument( cmnd, buf );
     if ( !str_cmp( buf, "or" ) )
     {
	 if ( mprog_do_ifchck( morebuf, NULL, actor, NULL, room, rndm, room->rrembch ) == 1 )
	     flag = TRUE;
     }
     else
       loopdone = TRUE;
 }
 if ( flag )
   for ( ; ; ) /*ifcheck was true, do commands but ignore else to endif*/ 
   {
       if ( !str_cmp( buf, "if" ) )
       { 
	   com_list = rprog_process_if(morebuf,com_list,room,actor,rndm);
	   while ( *cmnd==' ' )
	     cmnd++;
	   if ( *com_list == '\0' )
	     return null;
	   cmnd	    = com_list;
	   com_list = mprog_next_command( com_list );
	   morebuf  = one_argument( cmnd,buf );
	   continue;
       }
       if ( !str_cmp( buf, "break" ) )
	 return null;
       if ( !str_cmp( buf, "endif" ) )
	 return com_list;
       if ( !str_cmp( buf, "else" ) ) 
       {
	   while ( str_cmp( buf, "endif" ) ) 
	   {
	       if ( !str_cmp( buf, "if" ) )
			com_list = rextract_if_endif( com_list, room );
	       cmnd	= com_list;
	       com_list = mprog_next_command( com_list );
	       while ( *cmnd == ' ' )
		 cmnd++;
	       if ( *cmnd == '\0' )
	       {
		   bug ( "Room: %d missing endif after else",
			room->vnum );
		   return null;
	       }
	       morebuf = one_argument( cmnd,buf );
	   }
	   return com_list; 
       }
       rprog_process_cmnd( cmnd, room, actor, rndm );
       cmnd	= com_list;
       com_list = mprog_next_command( com_list );
       while ( *cmnd == ' ' )
	 cmnd++;
       if ( *cmnd == '\0' )
       {
	   bug ( "Room: %d missing else or endif", room->vnum ); 
	   return null;
       }
       morebuf = one_argument( cmnd, buf );
   }
 else /*false ifcheck, find else and do existing commands or quit at endif*/
   {
     while ( ( str_cmp( buf, "else" ) ) && ( str_cmp( buf, "endif" ) ) )
       {
	 if ( !str_cmp( buf, "if" ) )
		com_list = rextract_if_endif( com_list, room );
	 cmnd	  = com_list;
	 com_list = mprog_next_command( com_list );
	 while ( *cmnd == ' ' )
	   cmnd++;
	 if ( *cmnd == '\0' )
	   {
	     bug ( "Room: %d missing an else or endif",
		  room->vnum ); 
	     return null;
	   }
	 morebuf = one_argument( cmnd, buf );
       }

     /* found either an else or an endif.. act accordingly */
     if ( !str_cmp( buf, "endif" ) )
       return com_list;

     cmnd     = com_list;
     com_list = mprog_next_command( com_list );
     while ( *cmnd == ' ' )
       cmnd++;
     if ( *cmnd == '\0' )
       { 
	 bug ( "Room: %d missing endif", room->vnum ); 
	 return null;
       }
     morebuf = one_argument( cmnd, buf );
     
     for ( ; ; ) /*process the post-else commands until an endif is found.*/
       {
	 if ( !str_cmp( buf, "if" ) )
	   { 
	     com_list = rprog_process_if( morebuf, com_list, room, actor, rndm );
	     while ( *cmnd == ' ' )
	       cmnd++;
	     if ( *com_list == '\0' )
	       return null;
	     cmnd     = com_list;
	     com_list = mprog_next_command( com_list );
	     morebuf  = one_argument( cmnd,buf );
	     continue;
	   }
	 if ( !str_cmp( buf, "else" ) ) 
	   {
	     bug ( "Room: %d found else in an else section",
		  room->vnum ); 
	     return null;
	   }
	 if ( !str_cmp( buf, "break" ) )
	   return null;
	 if ( !str_cmp( buf, "endif" ) )
	   return com_list; 
	 rprog_process_cmnd( cmnd, room, actor, rndm );
	 cmnd	  = com_list;
	 com_list = mprog_next_command( com_list );
	 while ( *cmnd == ' ' )
	   cmnd++;
	 if ( *cmnd == '\0' )
	   {
	     bug ( "Room:%d missing endif in else section", room->vnum ); 
	     return null;
	   }
	 morebuf = one_argument( cmnd, buf );
       }
   }
}

char * rextract_if_endif( char *com_list, ROOM_INDEX_DATA *room )
{
 static char null[ 1 ];
 char buf[ MAX_INPUT_LENGTH ] = "";
 char	 *cmnd = '\0';
 *null = '\0';
	while ( str_cmp( buf, "endif" ) )
	{
		cmnd	= com_list;
		com_list = mprog_next_command( com_list );
		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' )
		{
			bug ( "Room: %d missing endif after else",
			room->vnum );
			return null;
		}
		one_argument( cmnd, buf );
		if ( !str_cmp( buf, "if" ) )
		{
			com_list = rextract_if_endif( com_list, room );
			while ( *com_list == ' ' )
				com_list++;
			if ( *com_list == '\0' )
			{
				bug ( "Room: %d missing endif after else",
				room->vnum );
				return null;
			}
			one_argument( com_list, buf );
		}
	}
	return com_list;
}

void rprog_process_cmnd( char *cmnd, ROOM_INDEX_DATA *room, CHAR_DATA *actor, CHAR_DATA *rndm )
{
	char buf[ MAX_INPUT_LENGTH ];
	char tmp[ MAX_INPUT_LENGTH ];
	char *str;
	char *i;
	char *point;

	point = buf;
	str	  = cmnd;
	while ( *str != '\0' )
	{
		if ( *str != '$' )
		{
			*point++ = *str++;
			continue;
		}
		str++;
		rprog_translate( *str, tmp, room, actor, rndm );
		i = tmp;
		++str;
		while ( ( *point = *i ) != '\0' )
			++point, ++i;
	}
	*point = '\0';
	if ( actor && actor->deleted )
	{
		bug( "Rprog_process_cmnd: actor deleted!", 0 );
		bug( actor->name, 0);
		return;
	}
	rinterpret( room, actor, buf );

	return;

}

void rprog_translate( char ch, char *t, ROOM_INDEX_DATA *room, CHAR_DATA *actor, CHAR_DATA *rndm )
{
 static char *he_she	    [] = { "it",  "he",	 "she" };
 static char *him_her	    [] = { "it",  "him", "her" };
 static char *his_her	    [] = { "its", "his", "her" };
 //CHAR_DATA   *vict	       = (CHAR_DATA *) vo;
 //OBJ_DATA    *v_obj	       = (OBJ_DATA  *) vo;
 CHAR_DATA   *bch;

 *t = '\0';
 switch ( ch ) {
     case 'n':
	if ( actor )
	   if ( !IS_SET( actor->act, PLR_WIZINVIS ) || IS_NPC(actor) )
	   {
	     one_argument( actor->name, t );
	     if ( !IS_NPC( actor ) )
	   	*t = UPPER( *t );
	     break;
	   }
      strcpy( t, "someone" );
      break;

     case 'N':
	 if ( actor ) 
	   if ( !IS_SET( actor->act, PLR_WIZINVIS ) || IS_NPC(actor) )
	    {
		/*  remove at 2020.0701
	       if ( IS_NPC( actor ) )
		 strcpy( t, actor->short_descr );
	       else
	       {
			strcpy( t, actor->short_descr ); // change by Amethyst
		   // strcpy( t, actor->name );
		   // strcat( t, " " );
		   // strcat( t, actor->pcdata->title );
	       }
		*/
			strcpy( t, actor->short_descr );
			break;
	    }
	  strcpy( t, "someone" );
	  break;

     case 'b':
	if ( ( bch = room->rrembch ) )
	   if ( !IS_SET( bch->act, PLR_WIZINVIS ) || IS_NPC(bch) )
	   {
	     one_argument( bch->name, t );
	     if ( !IS_NPC( bch ) )
	   	*t = UPPER( *t );
	   	break;
	   }
      strcpy( t, "someone" );
      break;

	  case 'B':
		 if ( ( bch = room->rrembch) )
		 {
			 if ( !IS_SET( bch->act, PLR_WIZINVIS ) || IS_NPC(bch) )
			 {
				strcpy( t, bch->short_descr );
			 }
			 break;
		 }
		 strcpy( t, "someone" );
		 break;

     case 'r':
	if ( rndm )
	   if ( !IS_SET( rndm->act, PLR_WIZINVIS ) )
	   {
	     one_argument( rndm->name, t );
	     if ( !IS_NPC( rndm ) )
	   	*t = UPPER( *t );
	   	break;
	   }
      strcpy( t, "someone" );
      break;

     case 'R':
	 if ( rndm ) 
	    if ( !IS_SET( rndm->act, PLR_WIZINVIS ) )
	    {
		/*  remove at 2020.0701
	       if ( IS_NPC( rndm ) )
		 strcpy(t,rndm->short_descr);
	       else
	       {
			   strcpy( t, rndm->short_descr ); // change by Amethyst
		 // strcpy( t, rndm->name );
		 // strcat( t, " " );
		 // strcat( t, rndm->pcdata->title );
	       }
		*/
			strcpy( t, rndm->short_descr );
			break;
	     }
      strcpy( t, "someone" );
      break;

     case 'e':
		if ( actor )
			if ( IS_NPC(actor) || !IS_SET( actor->act, PLR_WIZINVIS ) )
			{
				strcpy( t, he_she[ actor->sex ] );
			} 
			else
			{ 
				strcpy( t, "someone" );
			}
	   //!IS_SET( actor->act, PLR_WIZINVIS ) ? strcpy( t, he_she[ actor->sex ] ) : strcpy( t, "someone" );
	 break;
  
     case 'm':
		if ( actor )
			if ( IS_NPC(actor) || !IS_SET( actor->act, PLR_WIZINVIS ) )
			{
				strcpy( t, him_her[ actor->sex ] );
			} 
			else
			{ 
				strcpy( t, "someone" );
			}
	   //!IS_SET( actor->act, PLR_WIZINVIS ) ? strcpy( t, him_her[ actor->sex ] ) : strcpy( t, "someone" );
	 break;
  
     case 's':
		if ( actor )
			if ( IS_NPC(actor) || !IS_SET( actor->act, PLR_WIZINVIS ) )
			{
				strcpy( t, his_her[ actor->sex ] );
			} 
			else
			{ 
				strcpy( t, "someone's" );
			}
	   //!IS_SET( actor->act, PLR_WIZINVIS ) ? strcpy( t, his_her[ actor->sex ] ) : strcpy( t, "someone's" );
	 break;
     
     case 'J':
	 if ( rndm )
	   !IS_SET( rndm->act, PLR_WIZINVIS ) ? strcpy( t, he_she[ rndm->sex ] ) : strcpy( t, "someone" );
	 break;
  
     case 'K':
	 if ( rndm )
	   !IS_SET( rndm->act, PLR_WIZINVIS ) ? strcpy( t, him_her[ rndm->sex ] ) : strcpy( t, "someone" );
	 break;
  
     case 'L':
	 if ( rndm )
	   !IS_SET( rndm->act, PLR_WIZINVIS ) ? strcpy( t, his_her[ rndm->sex ] ) : strcpy( t, "someone's" );
	 break;
/*
     case 'o':
	 if ( obj )
	   can_see_obj( mob, obj ) ? one_argument( obj->name, t )
				   : strcpy( t, "something" );
	 break;

     case 'O':
	 if ( obj )
	   can_see_obj( mob, obj ) ? strcpy( t, obj->short_descr )
				   : strcpy( t, "something" );
	 break;

     case 'p':
	 if ( v_obj )
	   can_see_obj( mob, v_obj ) ? one_argument( v_obj->name, t )
				     : strcpy( t, "something" );
	 break;

     case 'P':
	 if ( v_obj )
	   can_see_obj( mob, v_obj ) ? strcpy( t, v_obj->short_descr )
				     : strcpy( t, "something" );
      break;

     case 'a':
	 if ( obj ) 
	  switch ( *( obj->name ) )
	  {
	    case 'a': case 'e': case 'i':
	    case 'o': case 'u': strcpy( t, "an" );
	      break;
	    default: strcpy( t, "a" );
	  }
	 break;

     case 'A':
	 if ( v_obj ) 
	  switch ( *( v_obj->name ) )
	  {
	    case 'a': case 'e': case 'i':
	    case 'o': case 'u': strcpy( t, "an" );
	      break;
	    default: strcpy( t, "a" );
	  }
	 break;
*/
     case '$':
	 strcpy( t, "$" );
	 break;

     default:
	 bug( "Room: %d bad $var", room->vnum );
	 break;
       }

 return;

}


void rprog_trap_trigger( ROOM_INDEX_DATA *room, CHAR_DATA *ch, int door )
{
    RPROG_DATA *rprg;
    char *argument;
    char arg[MAX_STRING_LENGTH];
    int num;
    int check = (1 << door );

    for ( rprg = room->roomprogs; rprg ; rprg = rprg->next )
    {
        argument = one_argument( rprg->arglist, arg );
	num = atoi( arg );
	if ( ( rprg->type & ROOM_TRAP) && ( IS_SET(num, check)))
	{
		one_argument( argument, arg );
		num = atoi( arg );
		if ( number_percent() < num || num >= 100)
		{
                    rprog_driver( rprg->comlist, room, ch );
		    break;
		}
	}
    }
    return;
}

void rprog_entry_trigger( ROOM_INDEX_DATA *room, CHAR_DATA *ch, int door )
{
    RPROG_DATA *rprg;
    char *argument;
    char arg[MAX_STRING_LENGTH];
    int num;

    for ( rprg = room->roomprogs; rprg ; rprg = rprg->next )
    {
        argument = one_argument( rprg->arglist, arg );
		num = atoi( arg );
		if ( ( rprg->type & ROOM_ENTER ) &&
			  (	( door == 0 && IS_SET( num, 4 ) ) ||
			( door == 1 && IS_SET( num, 8 ) ) ||
			( door == 2 && IS_SET( num, 1 ) ) ||
			( door == 3 && IS_SET( num, 2 ) ) ||
			( door == 4 && IS_SET( num, 32 ) ) ||
			( door == 5 && IS_SET( num, 16 ) ) ||
			( door == 6 && IS_SET( num, 64 ) ) ) ) // Added by Amethyst
		{
			one_argument( argument, arg );
			num = atoi( arg );
			if ( number_percent() < num || num >= 100)
			{
						rprog_driver( rprg->comlist, room, ch );
				break;
			}
		}
    }
    return;
}

void rprog_leave_trigger( ROOM_INDEX_DATA *room, CHAR_DATA *ch, int door )
{
    RPROG_DATA *rprg;
    char *argument;
    char arg[MAX_STRING_LENGTH];
    int num;

    for ( rprg = room->roomprogs; rprg ; rprg = rprg->next )
    {
        argument = one_argument( rprg->arglist, arg );
		num = atoi( arg );

		if ( ( rprg->type & ROOM_LEAVE ) && IS_SET( NUM_BIT( door ), num ) )
		{
			one_argument( argument, arg );
			num = atoi( arg );
			if ( number_percent() < num || num >= 100)
			{
				rprog_driver( rprg->comlist, room, ch );
				break;
			}
		}
    }
    return;
}

void rprog_rand_trigger( RPROG_DATA *rprg )
{
	if ( number_percent() < atoi( rprg->arglist ) || (atoi(rprg->arglist) >= 100 ))
		rprog_driver( rprg->comlist, rprg->owner, NULL );
}



void rprog_time_trigger( RPROG_DATA *rprg )
{
	if ( number_percent() < atoi( rprg->arglist ) 
			|| ( atoi(rprg->arglist)>= 100))
                rprog_driver( rprg->comlist, rprg->owner, NULL );
}


