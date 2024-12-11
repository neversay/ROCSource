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
 *  such installation can be found in INSTALL.  Enjoy...   N'Atas-Ha *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : obj_prog.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         新增 do_opset_obj_affect    變數宣告   *
//*              :         新增 do_opremove_obj_affect 變數宣告   *
//*              :         新增 do_opmodify_obj_affect 變數宣告   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         新增 do_opmodvt    變數宣告            *
//*****************************************************************
/*
 * obj_prog.c is created, moved, modified by Keric, 2005/2/16.
 * It's wrote for obj program.
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "merc.h"
#include "oprog.h"

const struct ocmd_type  ocmd_table[] = 
{
    { "oset",					do_ooset				},
    { "echo",					do_ooecho				},
    { "echoat",					do_oechoat				},
    { "echoaround",				do_oechoaround			},
    { "echotoall",				do_oechotoall			},
    { "nonechoat",				do_ononechoat			},
    { "nonechoaround",			do_ononechoaround		},
    { "oforce",					do_oforce				},
    { "modhmv",					do_omodhmv				},
    { "oforcer",				do_oforcer				},
    { "add_affect",				do_oadd_affect			},
    { "del_affect",				do_odel_affect			},
    { "purge",					do_opurge				},
    { "oteleport",				do_oteleport			},
    { "ogoto",					do_ogoto				},
    { "odef_rembch",			do_odef_rembch			}, // add at 2022/11/10
    { "cast",					do_ocast				}, // add at 2022/11/10
    { "opset_obj_affect",		do_opset_obj_affect		}, // Added by Razgriz 20050916
    { "opremove_obj_affect",	do_opremove_obj_affect	}, // Added by Razgriz 20050916
    { "opmodify_obj_affect",	do_opmodify_obj_affect	}, // Added by Razgriz 20050916
    { "opmodvt",				do_opmodvt				}, // Added by Razgriz 20050918
    { "", 0 }
};


/*
 * Local function prototypes
 */

/***************************************************************************
 * Local function code and brief comments.
 */

/* if you dont have these functions, you damn well should... */

#ifdef DUNNO_STRSTR
char *strstr(s1,s2) const char *s1; const char *s2;
{
	char *cp;
	int i,j=strlen(s1)-strlen(s2),k=strlen(s2);
	if(j<0)
		return NULL;
	for(i=0; i<=j && strncmp(s1++,s2, k)!=0; i++);
		return (i>j) ? NULL : (s1-1);
}
#endif

int oprog_name_to_type ( char *name )
{
	if ( !str_cmp( name, "obj_act"  ) )     return OBJ_ACT;
	if ( !str_cmp( name, "obj_repop"  ) )   return OBJ_REPOP;
	if ( !str_cmp( name, "obj_break" ))     return OBJ_BREAK;
	if ( !str_cmp( name, "obj_rand"  ))     return OBJ_RAND;
	if ( !str_cmp( name, "obj_hit" ))       return OBJ_HIT;
	if ( !str_cmp( name, "obj_block" ))     return OBJ_BLOCK;
	if ( !str_cmp( name, "obj_fight" ))     return OBJ_FIGHT;
	if ( !str_cmp( name, "obj_defun" ))     return OBJ_DEFUN;
	if ( !str_cmp( name, "obj_load"  ))     return OBJ_LOAD;
	if ( !str_cmp( name, "obj_disappear" )) return OBJ_DISAPPEAR;
	if ( !str_cmp( name, "obj_command" ))   return OBJ_COMMAND;
	if ( !str_cmp( name, "obj_tick" ))      return OBJ_TICK;
	if ( !str_cmp( name, "obj_kill" ))      return OBJ_KILL; // add at 2022/05/08

	return( ERROR_PROG );
}



void oprog_read_programs( FILE *fp, OBJ_INDEX_DATA *pIndex)
{
	OPROG_DATA *rprg;
	char letter;
	bool done = FALSE;
	int stat;

	if ( ( letter = fread_letter( fp ) ) != '>' )
	{
		bug( "Load_obj: vnum %d OBJprog char", pIndex->vnum );
		exit( 1 );
	}
	pIndex->objprogs = (OPROG_DATA *)alloc_mem( sizeof( OPROG_DATA ) );
	rprg = pIndex->objprogs;

	while ( !done )
	{
		rprg->type = oprog_name_to_type( fread_word( fp, &stat ) );
		if ( rprg->type == ERROR_PROG )
		{
			bug( "Load_obj: vnum %d OBJPROG type.", pIndex->vnum );

			exit( 1 );
		}
		else
		{
			rprg->next_in_list = oprg_list;
			oprg_list = rprg;
			rprg->owner = pIndex;
			pIndex->progtypes = pIndex->progtypes | rprg->type;
			rprg->arglist = fread_string( fp, &stat );
			fread_to_eol( fp );
			rprg->comlist = fread_string( fp, &stat );
			fread_to_eol( fp );
			switch ( letter = fread_letter( fp ) )
			{
				case '>':
					rprg->next = ( OPROG_DATA *)alloc_mem( sizeof( OPROG_DATA ) );
					rprg = rprg->next;
					rprg->next = NULL;
					break;

				case '|':
					rprg->next = NULL;
					fread_to_eol( fp );
					done = TRUE;
					break;

				default:
					bug( "Load_obj: vnum %d bad OBJPROG.", pIndex->vnum );
					exit( 1 );
					break;
			}
		}
	}

	return;

}


/* The main focus of the MOBprograms.  This routine is called 
 *  whenever a trigger is successful.  It is responsible for parsing
 *  the command list and figuring out what to do. However, like all
 *  complex procedures, everything is farmed out to the other guys.
 */
void oprog_driver( char *com_list, OBJ_DATA *obj, CHAR_DATA *actor, CHAR_DATA *victim, void *vo)
{

	char tmpcmndlst[ MAX_STRING_LENGTH*3 ];
	char buf [ MAX_INPUT_LENGTH ];
	char *morebuf;
	char *command_list;
	char *cmnd;
	CHAR_DATA *rndm  = NULL;
	CHAR_DATA *vch   = NULL;
	int count = 0;

	/* get a random visable mortal player who is in the room with the mob */
	if( !obj->in_room && !obj->carried_by && !obj->in_obj )
		return;

	if( obj->in_room) {
		for ( vch = obj->in_room->people; vch; vch = vch->next_in_room )
		{
			if ( !vch->in_room || vch->deleted )
				continue;
			if ( !IS_NPC( vch ) && !IS_SET(vch->act, PLR_WIZINVIS) )
			{
				if ( number_range( 0, count ) == 0 )
					rndm = vch;
				count++;
			}
		}
	}else if( obj->carried_by ){
		if( !obj->carried_by->in_room ) {
			bug("object %d carried_by has no in_room!", obj->pIndexData->vnum);
			return;
		}
		for ( vch = obj->carried_by->in_room->people; vch; vch = vch->next_in_room )
		{
			if ( !vch->in_room || vch->deleted )
				continue;
			if ( !IS_NPC( vch ) && can_see( obj->carried_by, vch ) )
			{
				if ( number_range( 0, count ) == 0 )
					rndm = vch;
				count++;
			}
		}
	}else if( obj->in_obj ){
		if( !obj->in_obj->in_room ) {
			bug("object %d in_obj has no in_room!", obj->pIndexData->vnum);
			return;
		}
		for ( vch = obj->in_obj->in_room->people; vch; vch = vch->next_in_room )
		{
			if ( !vch->in_room || vch->deleted )
				continue;
			if ( !IS_NPC( vch ) && !IS_SET(vch->act, PLR_WIZINVIS) )
			{
				if ( number_range( 0, count ) == 0 )
					rndm = vch;
				count++;
			}
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
			command_list = oprog_process_if( morebuf, command_list, obj, actor,  vo, rndm);
		else
			oprog_process_cmnd( cmnd, obj, actor, vo, rndm );
		cmnd = command_list;
		command_list = mprog_next_command( command_list );
	}

	return;

}

/* Obj prog wordlist check, 
added by Amethyst to improve the performance of OBJ_PROG.*/

bool oprog_wordlist_check( char *arg, OBJ_DATA *obj, CHAR_DATA *actor, int type )
{
	char temp1[ MAX_STRING_LENGTH ];
	char temp2[ MAX_INPUT_LENGTH ];
	char word[ MAX_INPUT_LENGTH ];
	char cmd[MAX_INPUT_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	OPROG_DATA *oprg = NULL;
	char *list;
	char *start;
	char *dupl;
	char *end;
	char *tmp;
	int i;
	bool fChinese;


	arg = one_argument(arg, cmd);
	one_argument(arg, arg1);

	for ( oprg = obj->pIndexData->objprogs; oprg != NULL; oprg = oprg->next ){
		if ( oprg->type == type )
		{
			//strcpy( temp1, oprg->arglist );
			tmp = one_argument(oprg->arglist, temp1);
			if( str_cmp(temp1, cmd ) ) continue;
			strcpy(temp1, tmp);
			list = temp1;
			fChinese=FALSE;
			for ( i = 0; i < strlen( list ); i++ ) {
				if ( fChinese == FALSE) list[i] = LOWER( list[i] );
					fChinese = is_chinese_char( list[i], fChinese );
			}
			strcpy( temp2, arg1);
			dupl = temp2;
			fChinese=FALSE;
			for ( i = 0; i < strlen( dupl ); i++ ) {
				if ( fChinese == FALSE) dupl[i] = LOWER( dupl[i] );
					fChinese = is_chinese_char( dupl[i], fChinese );
			}
			if ( ( list[0] == 'p' ) && ( list[1] == ' ' ) ){
				list += 2;
				while ( ( start = strstr( dupl, list ) ) )
					if ( (start == dupl || *(start-1) == ' ' )
						&& ( *(end = start + strlen( list ) ) == ' '
						|| *end == '\n'
						|| *end == '\r'
						|| *end == '\0'/*NULL*/ ) )
					{
						oprog_driver( oprg->comlist, obj, actor, NULL, NULL );
						return TRUE;
					}
					else
						dupl = start+1;
			}else{
				list = one_argument( list, word );
				for( ; word[0] != '\0'; list = one_argument( list, word ) )
				{
					while ( ( start = strstr( dupl, word ) ) )
					{
						if ( ( start == dupl || *(start-1) == ' ' )
							&& ( *(end = start + strlen( word ) ) == ' '
							|| *end == '\n'
							|| *end == '\r'
							|| *end == '\0' /*NULL */) )
						{
							oprog_driver( oprg->comlist, obj, actor, NULL, NULL );
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

void oprog_percent_check(OBJ_DATA* object, CHAR_DATA *actor, CHAR_DATA *victim,void *vo, int type)
{
	OPROG_DATA* oprg;
	for( oprg = object->pIndexData->objprogs;oprg != NULL;oprg = oprg->next)
	{
		if((oprg->type & type) && ((atoi(oprg->arglist)>= 100)|| (number_percent()<atoi(oprg->arglist))))
		{
			oprog_driver(oprg->comlist, object,actor,victim,vo);
		}
	}
	return;
}

void oprog_act_trigger(char *txt, CHAR_DATA *ch, OBJ_DATA *mob )
{
	OPROG_DATA *mprg;
	char arg[MAX_STRING_LENGTH];

	if( !mob->pIndexData->progtypes & OBJ_ACT) 
		return;
	if( txt != NULL )
	{
		for( mprg = mob->pIndexData->objprogs; mprg != NULL; mprg = mprg->next )
		{
			one_argument(mprg->arglist, arg);

			if( arg[0] != '\0' && !str_cmp(arg, txt))
				oprog_driver( mprg->comlist, mob, ch, NULL, NULL );
		}
		return;
	}

	return;
}

void oprog_tick_trigger( OBJ_DATA *obj)
{
	if ( obj->pIndexData->progtypes & OBJ_TICK)
		oprog_percent_check(obj,NULL,NULL,NULL,OBJ_TICK);
	return;
}

void oprog_rand_trigger( OBJ_DATA *obj)
{
	if ( obj->pIndexData->progtypes & OBJ_RAND)
		oprog_percent_check(obj,obj->carried_by,NULL,NULL,OBJ_RAND);
	return; 
}

// obj_fight, obj 攜帶者 戰鬥中每 round 判定 2022/05/07
void oprog_fight_trigger( OBJ_DATA *obj)
{
	if ( obj->pIndexData->progtypes & OBJ_FIGHT)
		// obj 必須有攜帶者
		if ( obj->carried_by )
			oprog_percent_check(obj,obj->carried_by,NULL,NULL,OBJ_FIGHT);
	return;
}

// obj_hit, obj 命中目標 2022/10/23
void oprog_hit_trigger( OBJ_DATA *obj)
{
	if ( obj->pIndexData->progtypes & OBJ_HIT)
		// obj 必須有攜帶者
		if ( obj->carried_by )
			oprog_percent_check(obj,obj->carried_by,NULL,NULL,OBJ_HIT);
	return;
}

// obj_block, obj parry 或 block 時觸發 2022/10/23
void oprog_block_trigger( OBJ_DATA *obj)
{
	if ( obj->pIndexData->progtypes & OBJ_BLOCK )
		// obj 必須有攜帶者
		if ( obj->carried_by )
			oprog_percent_check(obj,obj->carried_by,NULL,NULL,OBJ_BLOCK);
	return;
}

// obj_kill, obj 攜帶者 殺死目標時判定 2022/05/08
void oprog_kill_trigger( OBJ_DATA *obj )
{
	if ( obj->pIndexData->progtypes & OBJ_KILL)
		// obj 必須有攜帶者
		if ( obj->carried_by )
			oprog_percent_check(obj,obj->carried_by,NULL,NULL,OBJ_KILL);
	return;
}

void oprog_disappear_trigger( OBJ_DATA *obj)
{
	if ( obj->pIndexData->progtypes & OBJ_DISAPPEAR)
		oprog_percent_check(obj,obj->carried_by,NULL,NULL,OBJ_DISAPPEAR);
	return;   
}

void oprog_repop_trigger( OBJ_DATA *obj)
{
	if( obj->pIndexData->progtypes & OBJ_REPOP)
		oprog_percent_check(obj,NULL,NULL,NULL,OBJ_REPOP);
	return;
}

//obj_command prog by Amethyst
bool oprog_cmd_trigger(OBJ_DATA *obj, CHAR_DATA *ch ,char *txt )
{
	OPROG_DATA *oprg;
	char arg[MAX_INPUT_LENGTH];
	char cmd[MAX_INPUT_LENGTH];
	char *tmp;

	if( !txt )
		return FALSE;

	if( !(obj->pIndexData->progtypes & OBJ_COMMAND) )
		return FALSE;

	if( oprog_wordlist_check( txt, obj, ch, OBJ_COMMAND ) )
		return TRUE;

	tmp = one_argument(txt, cmd);
	one_argument(tmp, arg);

	for ( oprg = obj->pIndexData->objprogs; oprg != NULL; oprg = oprg->next )
	{
		if( oprg->type != OBJ_COMMAND ) continue;

		tmp = one_argument(oprg->arglist, arg); //strip cmd

		if( str_cmp(arg, cmd ) ) continue;

		one_argument(tmp, arg); //real arg

		if( arg[0] != '\0' && !str_cmp(arg,"default") )
		{
			oprog_driver( oprg->comlist, obj, ch, NULL, NULL );
			return TRUE;
		}
	}
	return FALSE;
}


char *oprog_process_if( char *ifchck, char *com_list, OBJ_DATA *obj, CHAR_DATA *actor, void* vo,CHAR_DATA *rndm )
{

	static char null[ 1 ];
	char buf[ MAX_INPUT_LENGTH ];
	char *morebuf = '\0';
	char  *cmnd = '\0';
	bool loopdone = FALSE;
	bool   flag = FALSE;

	*null = '\0';

	if ( actor && actor->deleted )
	{
		bug( "Oprog_process_if: Actor deleted!", 0 );
		bug( actor->name, 0 );
		return null;
	}

	/* check for trueness of the ifcheck */
	if ( mprog_do_ifchck( ifchck, NULL, actor, obj, vo, rndm, obj->orembch ) == 1 )
		flag = TRUE;

	while( loopdone == FALSE ) /*scan over any existing or statements */
	{
		cmnd = com_list;
		com_list = mprog_next_command( com_list );
		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' )
		{
			bug ( "Obj: %d no commands after IF/OR", obj->pIndexData->vnum ); 
			return null;
		}
		morebuf = one_argument( cmnd, buf );
		if ( !str_cmp( buf, "or" ) )
		{
			if ( mprog_do_ifchck( morebuf, NULL, actor, obj,vo, rndm, obj->orembch ) == 1 )
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
				com_list = oprog_process_if(morebuf,com_list,obj,actor,vo,rndm);
				while ( *cmnd==' ' )
					cmnd++;
				if ( *com_list == '\0' )
					return null;
				cmnd = com_list;
				com_list = mprog_next_command( com_list );
				morebuf = one_argument( cmnd,buf );
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
						com_list = oextract_if_endif( com_list, obj);
					cmnd = com_list;
					com_list = mprog_next_command( com_list );
					while ( *cmnd == ' ' )
						cmnd++;
					if ( *cmnd == '\0' )
					{
						bug ( "Obj: %d missing endif after else",
						obj->pIndexData->vnum );
						return null;
					}
					morebuf = one_argument( cmnd,buf );
				}
				return com_list; 
			}
			oprog_process_cmnd( cmnd, obj, actor, vo,rndm );
			cmnd = com_list;
			com_list = mprog_next_command( com_list );
			while ( *cmnd == ' ' )
				cmnd++;
			if ( *cmnd == '\0' )
			{
				bug ( "Obj: %d missing else or endif", obj->pIndexData->vnum ); 
				return null;
			}
			morebuf = one_argument( cmnd, buf );
		}
	else /*false ifcheck, find else and do existing commands or quit at endif*/
	{
		while ( ( str_cmp( buf, "else" ) ) && ( str_cmp( buf, "endif" ) ) )
		{
			if ( !str_cmp( buf, "if" ) )
				com_list = oextract_if_endif( com_list, obj);
			cmnd = com_list;
			com_list = mprog_next_command( com_list );
			while ( *cmnd == ' ' )
				cmnd++;
			if ( *cmnd == '\0' )
			{
				bug ( "Obj: %d missing an else or endif", obj->pIndexData->vnum ); 
				return null;
			}
			morebuf = one_argument( cmnd, buf );
		}

		/* found either an else or an endif.. act accordingly */
		if ( !str_cmp( buf, "endif" ) )
			return com_list;

		cmnd = com_list;
		com_list = mprog_next_command( com_list );

		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' )
		{
			bug ( "Obj: %d missing endif", obj->pIndexData->vnum ); 
			return null;
		}
		morebuf = one_argument( cmnd, buf );

		for ( ; ; ) /*process the post-else commands until an endif is found.*/
		{
			if ( !str_cmp( buf, "if" ) )
			{ 
				com_list = oprog_process_if( morebuf, com_list, obj, actor, vo, rndm );

				while ( *cmnd == ' ' )
					cmnd++;

				if ( *com_list == '\0' )
					return null;

				cmnd = com_list;
				com_list = mprog_next_command( com_list );
				morebuf = one_argument( cmnd,buf );
				continue;
			}
			if ( !str_cmp( buf, "else" ) ) 
			{
				bug ( "Obj: %d found else in an else section",
				obj->pIndexData->vnum ); 
				return null;
			}

			if ( !str_cmp( buf, "break" ) )
				return null;

			if ( !str_cmp( buf, "endif" ) )
				return com_list;

			oprog_process_cmnd( cmnd, obj, actor,vo, rndm );
			cmnd = com_list;
			com_list = mprog_next_command( com_list );

			while ( *cmnd == ' ' )
				cmnd++;

			if ( *cmnd == '\0' )
			{
				bug ( "Obj: %d missing endif in else section", obj->pIndexData->vnum ); 
				return null;
			}
			morebuf = one_argument( cmnd, buf );
		}
	}
}


char *oextract_if_endif( char *com_list, OBJ_DATA *obj)
{
	static char null[ 1 ];
	char buf[ MAX_INPUT_LENGTH ] = "";
	char  *cmnd = '\0';
	*null = '\0';
	while ( str_cmp( buf, "endif" ) )
	{
		cmnd  = com_list;
		com_list = mprog_next_command( com_list );
		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' )
		{
			bug ( "Obj: %d missing endif after else",
			obj->pIndexData->vnum );
			return null;
		}
		one_argument( cmnd, buf );
		if ( !str_cmp( buf, "if" ) )
		{
			com_list = oextract_if_endif( com_list, obj);
			while ( *com_list == ' ' )
				com_list++;
			if ( *com_list == '\0' )
			{
				bug ( "Obj: %d missing endif after else",
				obj->pIndexData->vnum );
				return null;
			}
			one_argument( com_list, buf );
		}
	}
	return com_list;
}



void oprog_process_cmnd( char *cmnd, OBJ_DATA *obj, CHAR_DATA *actor, void* vo,CHAR_DATA *rndm )
{
	char buf[ MAX_INPUT_LENGTH ];
	char tmp[ MAX_INPUT_LENGTH ];
	char *str;
	char *i;
	char *point;

	point   = buf;
	str   = cmnd;
	while ( *str != '\0' )
	{
		if ( *str != '$' )
		{
			*point++ = *str++;
			continue;
		}
		str++;
		oprog_translate( *str, tmp, obj, actor, rndm );
		i = tmp;
		++str;
		while ( ( *point = *i ) != '\0' )
			++point, ++i;
	}
	*point = '\0';
	if ( actor && actor->deleted )
	{
		bug( "Oprog_process_cmnd: actor deleted!", 0 );
		bug( actor->name, 0);
		return;
	}
	ointerpret( obj, actor, buf );

	return;
}


void oprog_translate( char ch, char *t, OBJ_DATA *obj, CHAR_DATA *actor, CHAR_DATA *rndm )
{
	static char *he_she      [] = { "it",  "he",  "she" };
	static char *him_her     [] = { "it",  "him", "her" };
	static char *his_her     [] = { "its", "his", "her" };
	//OBJ_DATA *v_obj = (OBJ_DATA  *) vo;
	//CHAR_DATA *vict = (CHAR_DATA *) vo;
	CHAR_DATA *bch;

	// 修正 PLR_WIZINVIS 等同於 NPC act hunt 造成不作用的問題, 加入 IS_NPC 強制成立 2022/10/07
	*t = '\0';
	switch ( ch )
	{
		case 'o':
			if ( actor && obj )
				can_see_obj( actor, obj )
					? one_argument( obj->name, t )
					: strcpy( t, "something" );
			break;

		case 'O':
			if ( actor && obj )
				can_see_obj( actor, obj )
					? strcpy( t, obj->short_descr )
					: strcpy( t, "something" );
			break;

		case 'i':
			if ( obj->carried_by )
				if ( !IS_SET( obj->carried_by->act, PLR_WIZINVIS ) || IS_NPC(obj->carried_by) )
				{
					one_argument( obj->carried_by->name, t );
					if ( !IS_NPC( obj->carried_by) )
						*t = UPPER( *t );
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'I':
			if ( obj->carried_by) 
				if ( !IS_SET( obj->carried_by->act, PLR_WIZINVIS ) || IS_NPC(obj->carried_by) )
				{
					if ( IS_NPC( obj->carried_by) )
						strcpy( t, obj->carried_by->short_descr );
					else
					{
						strcpy( t, obj->carried_by->short_descr );
						//strcpy( t, obj->carried_by->name );
						//strcat( t, " " );
						//strcat( t, obj->carried_by->pcdata->title );
					}
					break;
				}
			strcpy( t, "someone" );
			break;

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
					if ( IS_NPC( actor ) )
						strcpy( t, actor->short_descr );
					else
					{
						strcpy( t, actor->short_descr );
						//strcpy( t, actor->name );
						//strcat( t, " " );
						//strcat( t, actor->pcdata->title );
					}
					break;
				}
				strcpy( t, "someone" );
			break;

		case 'b':
			if ( ( bch = obj->orembch ) )
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
			if ( ( bch = obj->orembch ) )
				if ( !IS_SET( bch->act, PLR_WIZINVIS ) || IS_NPC(bch) )
				{
					strcpy(t,bch->short_descr);
					break;
				}
			strcpy( t, "someone" );
			break;

		// 被攻擊者 2022/05/08
		/*
		case 'v':
			if ( obj->carried_by)
				if ( (vict = obj->carried_by->fighting ) )
					if ( !IS_SET( vict->act, PLR_WIZINVIS ) )
					{
						one_argument( vict->name, t );
						if ( !IS_NPC( vict ) )
							*t = UPPER( *t );
						break;
					}
			strcpy( t, "someone" );
			break;

		case 'V':
			if ( obj->carried_by)
				if ( (vict = obj->carried_by->fighting ) )
					if ( !IS_SET( vict->act, PLR_WIZINVIS ) )
					{
						if ( IS_NPC( vict ) )
							strcpy( t, vict->short_descr );
						else
						{
							strcpy( t, vict->name );
						}
						break;
					}
			strcpy( t, "someone" );
			break;
		*/
		case 'v':
			if ( obj->carried_by && actor )
			{
				if ( actor->fighting )
					if ( !IS_SET( actor->fighting->act, PLR_WIZINVIS ) || IS_NPC(actor->fighting) )
					{
						one_argument( actor->fighting->name, t );
						if ( !IS_NPC( actor->fighting ) )
							*t = UPPER( *t );
						break;
					}
				strcpy( t, "someone" );
			}
			break;

		case 'V':
			if ( obj->carried_by && actor )
			{
				if ( actor->fighting )
					if ( !IS_SET( actor->fighting->act, PLR_WIZINVIS ) || IS_NPC(actor->fighting) )
					{
						if ( IS_NPC( actor->fighting ) )
							strcpy( t, actor->fighting->short_descr );
						else
						{
							strcpy( t, actor->fighting->name );
						}
						break;
					}
				strcpy( t, "someone" );
			}
			break;
		case 'r':
			if ( rndm )
				if ( !IS_SET( rndm->act, PLR_WIZINVIS ) || IS_NPC(rndm) )
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
				if ( !IS_SET( rndm->act, PLR_WIZINVIS ) || IS_NPC(rndm) )
				{
					if ( IS_NPC( rndm ) )
						strcpy(t,rndm->short_descr);
					else
					{
						strcpy(t,rndm->short_descr);
						//strcpy( t, rndm->name );
						//strcat( t, " " );
						//strcat( t, rndm->pcdata->title );
					}
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'e':
			if ( actor )
				// !IS_SET( actor->act, PLR_WIZINVIS )
				( !IS_SET( actor->act, PLR_WIZINVIS ) || IS_NPC(actor) )
					? strcpy( t, he_she[ actor->sex ] )
					: strcpy( t, "someone" );
			break;

		case 'm':
			if ( actor )
				//!IS_SET( actor->act, PLR_WIZINVIS )
				( !IS_SET( actor->act, PLR_WIZINVIS ) || IS_NPC(actor) )
					? strcpy( t, him_her[ actor->sex ] )
					: strcpy( t, "someone" );
			break;

		case 's':
			if ( actor )
				//!IS_SET( actor->act, PLR_WIZINVIS )
				( !IS_SET( actor->act, PLR_WIZINVIS ) || IS_NPC(actor) )
					? strcpy( t, his_her[ actor->sex ] )
					: strcpy( t, "someone's" );
			break;

		case 'J':
			if ( rndm )
				//!IS_SET( rndm->act, PLR_WIZINVIS )
				( !IS_SET( rndm->act, PLR_WIZINVIS ) || IS_NPC(rndm) )
					? strcpy( t, he_she[ rndm->sex ] )
					: strcpy( t, "someone" );
			break;

		case 'K':
			if ( rndm )
				//!IS_SET( rndm->act, PLR_WIZINVIS )
				( !IS_SET( rndm->act, PLR_WIZINVIS ) || IS_NPC(rndm) )
					? strcpy( t, him_her[ rndm->sex ] )
					: strcpy( t, "someone" );
			break;

		case 'L':
			if ( rndm )
				//!IS_SET( rndm->act, PLR_WIZINVIS )
				( !IS_SET( rndm->act, PLR_WIZINVIS ) || IS_NPC(rndm) )
					? strcpy( t, his_her[ rndm->sex ] )
					: strcpy( t, "someone's" );
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
			bug( "Obj: %d bad $var", obj->pIndexData->vnum );
			break;
	}

 return;

}
