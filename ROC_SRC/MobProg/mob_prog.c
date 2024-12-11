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
//*  PROGRAM ID  : mob_prog.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940707 jye                                    *
//*  PURPOSE     :         在mprog_percent_check中增加debug       *
//*              :         之online log                           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940714 Razgriz                                *
//*  PURPOSE     :         修正 mprog_do_ifchck() 的 learned 判斷 *
//*              :         中，最後的參數指定錯誤的問題           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 jye                                    *
//*  PURPOSE     :         將mprog_percent_check中debug移除       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Razgriz                                *
//*  PURPOSE     :         新增 obj ifchck  -  objextra           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941004 jye                                    *
//*  PURPOSE     :         修改pcnum  不記算imm char              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050705 jye                                    *
//*  PURPOSE     :         修改mobcount 增加obj                   *
//*---------------------------------------------------------------*
//*  LAST MODIFY DATE : 2022/07/16                                *
//*****************************************************************
/* $Id: mob_prog.c 3383 2005-10-04 14:05:32Z jye $ */
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "merc.h"
#include "ansi.h"

/*
 * Local function prototypes
 */

//bool  mprog_seval   args( ( char* lhs, char* opr, char* rhs ) );
//bool  mprog_veval   args( ( int lhs, char* opr, int rhs ) );
bool mprog_seval args( ( char* lhs, char* opr, char* rhs , char *fun_name ) );
bool mprog_veval args( ( int lhs, char* opr, int rhs , char *fun_name ) );
// 新增 bool 針對值過大的判定 2022/11/10
bool mprog_longval args( ( long long lhs, char* opr, long long rhs , char *fun_name ) );
int /* bool */  mprog_do_ifchck args( ( char* ifchck, CHAR_DATA* mob, CHAR_DATA* actor, OBJ_DATA* obj, void* vo, CHAR_DATA* rndm, CHAR_DATA* remb ) );
char *mprog_process_if args( ( char* ifchck, char* com_list, CHAR_DATA* mob, CHAR_DATA* actor, OBJ_DATA* obj, void* vo, CHAR_DATA* rndm ) );
char *mprog_next_command args( ( char* clist ) );
void  mprog_process_cmnd args( ( char* cmnd, CHAR_DATA* mob,  CHAR_DATA* actor, OBJ_DATA* obj, void* vo, CHAR_DATA* rndm ) );
void  mprog_translate args( ( char ch, char* t, CHAR_DATA* mob, CHAR_DATA* actor, OBJ_DATA* obj, void* vo, CHAR_DATA* rndm ) );
void  mprog_driver args( ( char* com_list, CHAR_DATA* mob, CHAR_DATA* actor, OBJ_DATA* obj, void* vo ) );
char *extract_if_endif args( ( char *com_list, CHAR_DATA *mob ) );

/***************************************************************************
 * Local function code and brief comments.
 */

/* if you dont have these functions, you damn well should... */
// gcc 3.4 have supported strstr, Keric, 2005/02/20
/*#ifdef DUNNO_STRSTR
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
*/
/* Used to get sequential lines of a multi line string (separated by "\n\r")
 * Thus its like one_argument(), but a trifle different. It is destructive
 * to the multi line string argument, and thus clist must not be shared.
 */
char *mprog_next_command( char *clist )
{

	char *pointer = clist;

	while ( *pointer != '\n' && *pointer != '\0' )
		pointer++;
	if ( *pointer == '\n' )
		*pointer++ = '\0';
	if ( *pointer == '\r' )
		*pointer++ = '\0';

	return ( pointer );

}

/* These two functions do the basic evaluation of ifcheck operators.
 *  It is important to note that the string operations are not what
 *  you probably expect.  Equality is exact and division is substring.
 *  remember that lhs has been stripped of leading space, but can
 *  still have trailing spaces so be careful when editing since:
 *  "guard" and "guard " are not equal.
 */

// 加入 fun name 顯示 bug 發生的 fun 2022/11/08
//bool mprog_seval( char *lhs, char *opr, char *rhs )
bool mprog_seval( char *lhs, char *opr, char *rhs , char *fun_name )
{

	if ( !str_cmp( opr, "==" ) )
		return ( bool )( !str_cmp( lhs, rhs ) );
	if ( !str_cmp( opr, "!=" ) )
		return ( bool )( str_cmp( lhs, rhs ) );
	if ( !str_cmp( opr, "/" ) )
		return ( bool )( !str_infix( rhs, lhs ) );
	if ( !str_cmp( opr, "!/" ) )
		return ( bool )( str_infix( rhs, lhs ) );

	// add at 2022/11/08
	char buf[ MAX_INPUT_LENGTH ];
	sprintf( buf, "Improper mprog_seval MOBprog operator: %s %s %s %s:", fun_name , lhs , opr , rhs );
	bug( buf , 0 );

	//bug ( "Improper mprog_seval MOBprog operator:", 0 );
	//bug(opr, 0);
	//bug(rhs, 0);
	return 0;

}

// 加入 fun name 顯示 bug 發生的 fun 2022/11/08
//bool mprog_veval( int lhs, char *opr, int rhs )
bool mprog_veval( int lhs, char *opr, int rhs , char *fun_name)
{

	if ( !str_cmp( opr, "==" ) )
		return ( lhs == rhs );
	if ( !str_cmp( opr, "!=" ) )
		return ( lhs != rhs );
	if ( !str_cmp( opr, ">" ) )
		return ( lhs > rhs );
	if ( !str_cmp( opr, "<" ) )
		return ( lhs < rhs );
	if ( !str_cmp( opr, "<=" ) || !str_cmp( opr, "=<" ) )
		return ( ( lhs < rhs ) || ( lhs == rhs ) );
	if ( !str_cmp( opr, ">=" ) || !str_cmp( opr, "=>" ) )
		return ( lhs >= rhs );
	if ( !str_cmp( opr, "&" ) )
		return ( ( lhs & rhs ) != 0 );
	if ( !str_cmp( opr, "|" ) )
		return ( lhs | rhs );
	if ( !str_cmp( opr, "!&" ) )
		return ( !( lhs & rhs ) );

	// add at 2022/11/08
	char buf[ MAX_INPUT_LENGTH ];
	sprintf( buf, "Improper mprog_veval MOBprog operator: %s %d %s %d:", fun_name , lhs , opr , rhs);
	bug( buf , 0 );

	//bug ( "Improper mprog_veval MOBprog operator : %d", lhs );
	//bug(opr, 0);
	//bug("%d", rhs);
	return 0;
}

// 針對值過大的判定, 如 isaffected 及 roomflag 用的判定 2022/11/10
bool mprog_longval( long long lhs, char *opr, long long rhs , char *fun_name)
{

	if ( !str_cmp( opr, "==" ) )
		return ( lhs == rhs );
	if ( !str_cmp( opr, "!=" ) )
		return ( lhs != rhs );
	if ( !str_cmp( opr, ">" ) )
		return ( lhs > rhs );
	if ( !str_cmp( opr, "<" ) )
		return ( lhs < rhs );
	if ( !str_cmp( opr, "<=" ) || !str_cmp( opr, "=<" ) )
		return ( ( lhs < rhs ) || ( lhs == rhs ) );
	if ( !str_cmp( opr, ">=" ) || !str_cmp( opr, "=>" ) )
		return ( lhs >= rhs );
	if ( !str_cmp( opr, "&" ) )
		return ( ( lhs & rhs ) != 0 );
	if ( !str_cmp( opr, "|" ) )
		return ( lhs | rhs );
	if ( !str_cmp( opr, "!&" ) )
		return ( !( lhs & rhs ) );

	// add at 2022/11/08
	char buf[ MAX_INPUT_LENGTH ];
	sprintf( buf, "Improper mprog_veval MOBprog operator: %s %d %s %d:", fun_name , lhs , opr , rhs);
	bug( buf , 0 );

	//bug ( "Improper mprog_veval MOBprog operator : %d", lhs );
	//bug(opr, 0);
	//bug("%d", rhs);
	return 0;
}

int FightWithVarCheck( CHAR_DATA *ch , CHAR_DATA *vch , char *opr )
{
	if ( !str_cmp( opr, "==" ) ){
		if( ch->fighting && ch->fighting == vch )
			return mprog_veval( 1 , "==" , 1 , "fightwith" );
	}
	return mprog_veval( 1 , "==" , 2 , "fightwith" );

}

// 加入 fun name 顯示 bug 發生的 fun 2022/11/08
//int GetMprogSeval_RaceByCharData(CHAR_DATA *ch, char opr[], char val[] )
int GetMprogSeval_RaceByCharData(CHAR_DATA *ch, char opr[], char val[] , char* fun_name)
{
	if( ch ){
		if(ch->temp_race){
			return mprog_seval( race_table[ch->temp_race - 1].name, opr, val, fun_name );
		}else if(is_affected(ch, gsn_werewolf)){
			if(is_affected(ch, gsn_polymorph)){
				return mprog_seval( race_table[ch->race].name, opr, val, fun_name );
			}else{
				return mprog_seval( "Werewolf", opr, val, fun_name );
			}
		}else{
			return mprog_seval( race_table[ch->race].name, opr, val, fun_name );
		}
	}
	
	return -1;

}


/* This function performs the evaluation of the if checks.  It is
 * here that you can add any ifchecks which you so desire. Hopefully
 * it is clear from what follows how one would go about adding your
 * own. The syntax for an if check is: ifchck ( arg ) [opr val]
 * where the parenthesis are required and the opr and val fields are
 * optional but if one is there then both must be. The spaces are all
 * optional. The evaluation of the opr expressions is farmed out
 * to reduce the redundancy of the mammoth if statement list.
 * If there are errors, then return -1 otherwise return boolean 1,0
 */
/* bool */ int mprog_do_ifchck( char *ifchck, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo, CHAR_DATA *rndm, CHAR_DATA *remb)
{

	char buf[ MAX_INPUT_LENGTH ];
	char arg[ MAX_INPUT_LENGTH ];
	char opr[ MAX_INPUT_LENGTH ];
	char val[ MAX_INPUT_LENGTH ];
	CHAR_DATA *vict = (CHAR_DATA *) vo;
	OBJ_DATA *v_obj = (OBJ_DATA  *) vo;
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	ROOM_INDEX_DATA *in_room = NULL;
	char *bufpt = buf;
	char *argpt = arg;
	char *oprpt = opr;
	char *valpt = val;
	char *point = ifchck;
	int lhsvl = 0;
	int rhsvl;
	AFFECT_DATA *paf;
	CHAR_DATA *victim = NULL, *vch = NULL;
	ROOM_INDEX_DATA *vroom;
	MOB_INDEX_DATA *vmob;
	OBJ_INDEX_DATA *cobj;
	OBJ_DATA *vobj;
	QMARK *qmark;
	char *vnumptr;
	char mark_name[ MAX_INPUT_LENGTH ];
	int mark_value;

	//if( !mob ) return -1;
  
	if ( *point == '\0' )
	{
		if( mob ) 
			bug ( "Mob: %d null ifchck", mob->pIndexData->vnum ); 
		return -1;
	} 
	/* skip leading spaces */
	while ( *point == ' ' )
		point++;

	/* get whatever comes before the left paren.. ignore spaces */
	while ( *point != '(' ) 
	if ( *point == '\0' ) 
	{
		if( mob )
			bug ( "Mob: %d ifchck syntax error", mob->pIndexData->vnum ); 
		return -1;
	}   
	else if ( *point == ' ' )
		point++;
	else 
		*bufpt++ = *point++; 

	*bufpt = '\0';
	point++;

	/* get whatever is in between the parens.. ignore spaces */
	while ( *point != ')' ) 
	if ( *point == '\0' ) 
	{
		if( mob )
			bug ( "Mob: %d ifchck syntax error", mob->pIndexData->vnum ); 
		return -1;
	}   
	else if ( *point == ' ' )
		point++;
	else 
		*argpt++ = *point++; 

	*argpt = '\0';
	point++;

	/* check to see if there is an operator */
	while ( *point == ' ' )
		point++;
	if ( *point == '\0' ) 
	{
		*opr = '\0';
		*val = '\0';
	} 
	else /* there should be an operator and value, so get them */
	{
		while ( ( *point != ' ' ) && ( !isalnum( *point ) ) ) 
			if ( *point == '\0' ) 
			{
				if( mob )
					bug ( "Mob: %d ifchck operator without value", mob->pIndexData->vnum ); 
				return -1;
			}
			else
				*oprpt++ = *point++; 

		*oprpt = '\0';

		/* finished with operator, skip spaces and then get the value */
		while ( *point == ' ' )
			point++;
		for( ; ; )
		{
			if ( ( *point != ' ' ) && ( *point == '\0' ) )
				break;
			else
				*valpt++ = *point++; 
		}

		*valpt = '\0';
	}
	bufpt = buf;
	argpt = arg;
	oprpt = opr;
	valpt = val;
  /* Ok... now buf contains the ifchck, arg contains the inside of the
   *  parentheses, opr contains an operator if one is present, and val
   *  has the value if an operator was present.
   *  So.. basically use if statements and run over all known ifchecks
   *  Once inside, use the argument and expand the lhs. Then if need be
   *  send the lhs,opr,rhs off to be evaluated.
   */

	if ( !str_cmp( buf, "rand" ) )
	{
		return ( number_percent() <= atoi(arg) );
	}

	if ( !str_cmp( buf, "ispc" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
		case 'i':
			return 0;
		case 'n':
			if ( actor )
				return ( !IS_NPC( actor ) && !IS_SET( actor->act, PLR_HORSE ) );
			else
				return -1;
		case 'v':  // 2022/10/08
			// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
			//if ( actor && actor->fighting )
			if ( mob && mob->fighting )
			{
				return ( !IS_NPC( mob->fighting ) && !IS_SET( mob->fighting->act, PLR_HORSE ) );
			}
			else if ( obj && obj->carried_by && actor && actor->fighting )
			{
				return ( !IS_NPC( actor->fighting ) && !IS_SET( actor->fighting->act, PLR_HORSE ) );
			}
			else
				return -1;
		case 't':
			if ( vict )
				return ( !IS_NPC( vict ) && !IS_SET( vict->act, PLR_HORSE ) );
			else
				return -1;
		case 'r':
			if ( rndm )
				return ( !IS_NPC( rndm ) && !IS_SET( rndm->act, PLR_HORSE ) );
			else
				return -1;
		case 'b':
			if ( remb )
				return ( !IS_NPC( remb ) && !IS_SET( remb->act, PLR_HORSE ) );
			else
				return -1;
		default:
			if( mob )
				bug ( "Mob: %d bad argument to 'ispc'", mob->pIndexData->vnum ); 

			return -1;
		}
	}



	if ( !str_cmp( buf, "isnpc" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return 1;
			case 'n':
				if ( actor )
					return ( IS_NPC( actor ) );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return ( IS_NPC( mob->fighting ) );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return ( IS_NPC( actor->fighting ) );
				else
					return -1;
			case 't':
				if ( vict )
					return ( IS_NPC( vict ) );
				else
					return -1;
			case 'r':
				if ( rndm )
					return ( IS_NPC( rndm ) );
				else
					return -1;
			case 'b':
				if ( remb )
					return ( IS_NPC( remb ) );
				else
					return -1;
			default:
				if( mob )
					bug ("Mob: %d bad argument to 'isnpc'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "isgood" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return IS_GOOD( mob );
			case 'n':
				if ( actor )
					return IS_GOOD( actor );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return IS_GOOD( mob->fighting );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return IS_GOOD( actor->fighting );
				else
					return -1;
			case 't':
				if ( vict )
					return IS_GOOD( vict );
				else
					return -1;
			case 'r':
				if ( rndm )
					return IS_GOOD( rndm );
				else
					return -1;
			case 'b':
				if ( remb )
					return IS_GOOD( remb );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'isgood'", mob->pIndexData->vnum ); 
					return -1;
		}
	}

	if ( !str_cmp( buf, "isevil" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return IS_EVIL( mob );
			case 'n':
				if ( actor )
					return IS_EVIL( actor );
				else
					return -1;
				/*{
					if(obj)
						bug ( "BUGIF:%d bad argument to isevil",obj->pIndexData->vnum);
					return -1;
				}*/
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return IS_EVIL( mob->fighting );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return IS_EVIL( actor->fighting );
				else
					return -1;
			case 't':
				if ( vict )
					return IS_EVIL( vict );
				else
					return -1;
			case 'r':
				if ( rndm )
					return IS_EVIL( rndm );
				else
					return -1;
			case 'b':
				if ( remb )
					return IS_EVIL( remb );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'isevil'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "isneutral" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return IS_NEUTRAL( mob );
			case 'n':
				if ( actor )
					return IS_NEUTRAL( actor );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return IS_NEUTRAL( mob->fighting );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return IS_NEUTRAL( actor->fighting );
				else
					return -1;
			case 't':
				if ( vict )
					return IS_NEUTRAL( vict );
				else
					return -1;
			case 'r':
				if ( rndm )
					return IS_NEUTRAL( rndm );
				else
					return -1;
			case 'b':
				if ( remb )
					return IS_NEUTRAL( remb );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'isneutral'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "isfight" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob )
						return ( mob->fighting ) ? 1 : 0;
					else
						return -1;
				case 'n':
					if ( actor )
						return ( actor->fighting ) ? 1 : 0;
					else if ( obj && obj->carried_by ) // add at 2022/05/08
						return ( obj->carried_by->fighting ) ? 1 : 0;
					else
						return -1;
				case 't':
					if ( vict )
						return ( vict->fighting ) ? 1 : 0;
					else
						return -1;
				case 'r':
					if ( rndm )
						return ( rndm->fighting ) ? 1 : 0;
					else
						return -1;
				case 'b':
					if ( remb )
						return ( remb->fighting ) ? 1 : 0;
					else
						return -1;

				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'isfight'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			if ( mob )
			{
				if ( ( victim = get_char_room( mob, arg ) ) )
					return ( victim->fighting ) ? 1 : 0;
				if( !victim )
					return 0;
			}
			else if ( room )
			{
				if ( ( victim = room_find_char( room, arg ) ) )
					return ( victim->fighting ) ? 1 : 0;
				if( !victim )
					return 0;
			}
			else if ( obj && obj->carried_by && !IS_NPC(obj->carried_by))
			{
				return ( obj->carried_by->fighting ) ? 1 : 0;
			}
			else
				return 0;
		}
	}

	if ( !str_cmp( buf, "isdrunk" ) ) /*add by coconet 20030806*/
	{
		switch ( arg[1] )
		{
			case 'i':
				if( mob ) 
					return ( !IS_NPC( mob ) && mob->pcdata->condition[COND_DRUNK ] > 10 );
				else
					return -1;
			case 'n':
				if ( actor )
					return ( !IS_NPC( actor ) && actor->pcdata->condition[COND_DRUNK ] > 10 );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return ( !IS_NPC( mob->fighting ) && mob->fighting->pcdata->condition[COND_DRUNK ] > 10);
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return ( !IS_NPC( actor->fighting ) && actor->fighting->pcdata->condition[COND_DRUNK ] > 10);
				else
					return -1;
			case 'r':
				if ( rndm )
					return ( !IS_NPC( rndm ) && rndm->pcdata->condition[COND_DRUNK ] > 10);
				else
					return -1;
			default: 
				if( mob )
					bug ( "Mob: %d bad argument to 'isdruken'", mob->pIndexData->vnum ); 
				return -1;
		}
	}
	
	if ( !str_cmp( buf, "isfly" ) ) /*add at 2022/06/15*/
	{
		switch ( arg[1] )
		{
			case 'i':
				if( mob ) 
					return ( is_flying( mob ) );
				else
					return -1;
			case 'n':
				if ( actor )
					return ( is_flying( actor ) );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return ( is_flying( mob->fighting ) );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return ( is_flying( actor->fighting ) );
				else
					return -1;
			case 'r':
				if ( rndm )
					return ( !IS_NPC( rndm ) && is_flying( rndm ) );
				else
					return -1;
			default: 
				if( mob )
					bug ( "Mob: %d bad argument to 'isfly'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "isimmort" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return ( get_trust( mob ) > LEVEL_IMMORTAL );
			case 'n':
				if ( actor )
					return ( get_trust( actor ) > LEVEL_IMMORTAL );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return ( get_trust( mob->fighting ) > LEVEL_IMMORTAL );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return ( get_trust( actor->fighting ) > LEVEL_IMMORTAL );
				else
					return -1;
			case 't':
				if ( vict )
					return ( get_trust( vict ) > LEVEL_IMMORTAL );
				else
					return -1;
			case 'r':
				if ( rndm )
					return ( get_trust( rndm ) > LEVEL_IMMORTAL );
				else
					return -1;
			case 'b':
				if ( remb )
					return ( get_trust( remb ) > LEVEL_IMMORTAL );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'isimmort'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "ischarmed" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return IS_AFFECTED( mob, AFF_CHARM );
			case 'n':
				if ( actor )
					return IS_AFFECTED( actor, AFF_CHARM );
				else
					return -1;
			case 'v': // 2022/10/08
				// 從 actor 修改為 mob 且加入 obj 判定 2022/11/10
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return IS_AFFECTED( mob->fighting, AFF_CHARM );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return IS_AFFECTED( actor->fighting, AFF_CHARM );
				else
					return -1;
			case 't':
				if ( vict )
					return IS_AFFECTED( vict, AFF_CHARM );
				else
					return -1;
			case 'r':
				if ( rndm )
					return IS_AFFECTED( rndm, AFF_CHARM );
				else
					return -1;
			case 'b':
				if ( remb )
					return IS_AFFECTED( remb, AFF_CHARM );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'ischarmed'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "isfollow" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
					return ( mob->master != NULL && mob->master->in_room == mob->in_room );
				else
					return -1;
			case 'n':
				if ( actor )
					return ( actor->master != NULL && actor->master->in_room == actor->in_room );
				else
					return -1;
			case 't':
				if ( vict )
					return ( vict->master != NULL && vict->master->in_room == vict->in_room );
				else
					return -1;
			case 'r':
				if ( rndm )
					return ( rndm->master != NULL && rndm->master->in_room == rndm->in_room );
				else
					return -1;
			case 'b':
				if ( remb )
					return ( remb->master != NULL && remb->master->in_room == rndm->in_room );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'isfollow'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "isgroup" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
		case 'i': 
			if( mob )
				return ( mob->group != NULL && mob->group->leader != NULL );
						//&& mob->group->leader->in_room == mob->in_room );  remove at 20/03/05
			else
				return -1;
		case 'n':
			if ( actor )
				return ( actor->group != NULL && actor->group->leader != NULL );
						//&& actor->group->leader->in_room == actor->in_room );
			else
				return -1;
		case 'v': // 2022/10/08
			//if ( actor && actor->fighting )
			if ( mob && mob->fighting )
				return ( mob->fighting->group != NULL && mob->fighting->group->leader != NULL );
			else if ( obj && obj->carried_by && actor && actor->fighting )
				return ( actor->fighting->group != NULL && actor->fighting->group->leader != NULL );
			else
				return -1;
		case 't':
			if ( vict )
				return ( vict->group != NULL && vict->group->leader != NULL );
						//&& vict->group->leader->in_room == vict->in_room );
			else
				return -1;
		case 'r':
			if ( rndm )
				return ( rndm->group != NULL && rndm->group->leader != NULL );
						//&& rndm->group->leader->in_room == rndm->in_room );
			else
				return -1;
		case 'b':
			if ( remb )
				return ( remb->group != NULL && remb->group->leader != NULL );
						//&& remb->group->leader->in_room == rndm->in_room );
			else
				return -1;
		default:
			if( mob )
				bug ( "Mob: %d bad argument to 'isgroup'", mob->pIndexData->vnum ); 
			return -1;
		}
	}

	if ( !str_cmp( buf, "is_affected" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
					return ( mob->affected_by & atoi( arg ) );
				else
					return -1;
			case 'n':
				if ( actor )
					return ( actor->affected_by & atoi( arg ) );
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
					return ( mob->fighting->affected_by & atoi( arg ) );
				else if ( obj && obj->carried_by && actor && actor->fighting )
					return ( actor->fighting->affected_by & atoi( arg ) );
				else
					return -1;
			case 't':
				if ( vict )
					return ( vict->affected_by & atoi( arg ) );
				else
					return -1;
			case 'r':
				if ( rndm )
					return ( rndm->affected_by & atoi( arg ) );
				else
					return -1;
			case 'b':
				if ( remb )
					return ( remb->affected_by & atoi( arg ) );
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'isaffected'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "hitprcnt" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob )
					{
						lhsvl = 100 * mob->hit / mob->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'n':
					if ( actor )
					{
						lhsvl = 100 * actor->hit / actor->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting ){
						lhsvl = 100 * mob->fighting->hit / mob->fighting->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else if ( obj && obj->carried_by && actor && actor->fighting ){
						lhsvl = 100 * actor->fighting->hit / actor->fighting->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						lhsvl = 100 * vict->hit / vict->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						lhsvl = 100 * rndm->hit / rndm->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						lhsvl = 100 * remb->hit / remb->max_hit;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'hitprcnt'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'hitprcnt'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			lhsvl = 100 * victim->hit / victim->max_hit;
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl , buf );
		}
	}

	// 2023/01/08
	if ( !str_cmp( buf, "manaprcnt" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob )
					{
						lhsvl = 100 * mob->mana / mob->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'n':
					if ( actor )
					{
						lhsvl = 100 * actor->mana / actor->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'v':
					if ( mob && mob->fighting ){
						lhsvl = 100 * mob->fighting->mana / mob->fighting->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else if ( obj && obj->carried_by && actor && actor->fighting ){
						lhsvl = 100 * actor->fighting->mana / actor->fighting->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						lhsvl = 100 * vict->mana / vict->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						lhsvl = 100 * rndm->mana / rndm->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						lhsvl = 100 * remb->mana / remb->max_mana;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'manaprcnt'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'manaprcnt'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			lhsvl = 100 * victim->mana / victim->max_mana;
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl , buf );
		}
	}

	// 2023/01/08
	if ( !str_cmp( buf, "moveprcnt" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob )
					{
						lhsvl = 100 * mob->move / mob->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'n':
					if ( actor )
					{
						lhsvl = 100 * actor->move / actor->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting ){
						lhsvl = 100 * mob->fighting->move / mob->fighting->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else if ( obj && obj->carried_by && actor && actor->fighting ){
						lhsvl = 100 * actor->fighting->move / actor->fighting->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						lhsvl = 100 * vict->move / vict->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						lhsvl = 100 * rndm->move / rndm->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						lhsvl = 100 * remb->move / remb->max_move;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl , buf );
					}
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'moveprcnt'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'moveprcnt'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			lhsvl = 100 * victim->move / victim->max_move;
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl , buf );
		}
	}

	if( !str_cmp(buf,"align"))
	{
		if( arg[0] == '$' )
		switch ( arg[1] )
		{
			case 'i':
			rhsvl = atoi(val);
				if ( mob )
					return mprog_veval(mob->alignment,opr,rhsvl,buf);
				else
					return -1;
			case 'n':
				if ( actor )
					return mprog_veval( actor->alignment,opr,atoi(val),buf);
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					return mprog_veval( mob->fighting->alignment,opr,atoi(val),buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					return mprog_veval( actor->fighting->alignment,opr,atoi(val),buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
					return mprog_veval( rndm->alignment,opr,atoi(val),buf);
				else
					return -1;
			case 'b':
				if (remb )
					return mprog_veval( remb->alignment,opr,atoi(val),buf);
			default:
				return -1;
		}
	}
	if ( !str_cmp( buf, "hpnum" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->hit, opr, rhsvl ,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->hit, opr, atoi( val ) ,buf);
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->hit, opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->hit, opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->hit, opr, atoi( val ) ,buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->hit, opr, atoi( val ) ,buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->hit, opr, atoi( val ) ,buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'hp num'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'hpnum'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->hit, opr, atoi( val ) ,buf);
		}
	}
	
	//add at 2018/10/24 check for max hp
	if ( !str_cmp( buf, "hpmax" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->max_hit, opr, rhsvl ,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->max_hit, opr, atoi( val ) ,buf);
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->max_hit, opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->max_hit, opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->max_hit, opr, atoi( val ),buf );
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->max_hit, opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->max_hit, opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'hp max'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'hpmax'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->max_hit, opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "weight") )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )
			{
				case 'n':
					if ( actor )
						return mprog_veval( actor->carry_weight, opr, atoi(val),buf);
					else
					{ 
						if( mob )
							bug (" Mob: %d bad argument to weight", mob->pIndexData->vnum );
						return -1;
					}
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->carry_weight, opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->carry_weight, opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 'i':
					if ( mob )
						return mprog_veval( mob->carry_weight, opr, atoi(val),buf);
					else
						return -1;
				case 'r':
					if ( rndm)
						return mprog_veval( rndm->carry_weight, opr,atoi(val),buf);
					else
						return -1;
				case 'b':
					if ( remb)
						return mprog_veval( rndm->carry_weight, opr,atoi(val),buf);
					else
						return -1;
				default:
					if( mob )
						bug (" Mob: %d bad argument to 'weight'",mob->pIndexData->vnum);
					return -1;
			}
	}

	//add at 2020/06/14
	if ( !str_cmp( buf, "carrynum") )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )
			{
				case 'n':
					if ( actor )
						return mprog_veval( actor->carry_number, opr, atoi(val),buf);
					else
					{ 
						if( mob )
							bug (" Mob: %d bad argument to carrynum", mob->pIndexData->vnum );
						return -1;
					}
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->carry_number, opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->carry_number, opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 'i':
					if ( mob )
						return mprog_veval( mob->carry_number, opr, atoi(val),buf);
					else
						return -1;
				case 'r':
					if ( rndm)
						return mprog_veval( rndm->carry_number, opr,atoi(val),buf);
					else
						return -1;
				case 'b':
					if ( remb)
						return mprog_veval( rndm->carry_number, opr,atoi(val),buf);
					else
						return -1;
				default:
					if( mob )
						bug (" Mob: %d bad argument to 'carrynum'",mob->pIndexData->vnum);
					return -1;
			}
	}
	
	//add at 2020/06/14
	if ( !str_cmp( buf, "maxweight") )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )
			{
				case 'n':
					if ( actor )
						return mprog_veval( can_carry_w(actor), opr, atoi(val),buf);
					else
					{ 
						if( mob )
							bug (" Mob: %d bad argument to maxweight", mob->pIndexData->vnum );
						return -1;
					}
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( can_carry_w(mob->fighting), opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( can_carry_w(actor->fighting), opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 'i':
					if ( mob )
						return mprog_veval( can_carry_w(mob) , opr, atoi(val),buf);
					else
						return -1;
				case 'r':
					if ( rndm)
						return mprog_veval( can_carry_w(rndm) , opr,atoi(val),buf);
					else
						return -1;
				case 'b':
					if ( remb)
						return mprog_veval( can_carry_w(rndm) , opr,atoi(val),buf);
					else
						return -1;
				default:
					if( mob )
						bug (" Mob: %d bad argument to 'maxweight'",mob->pIndexData->vnum);
					return -1;
			}
	}
	
	//add at 2020/06/14
	if ( !str_cmp( buf, "maxcarrynum") )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )
			{
				case 'n':
					if ( actor )
						return mprog_veval( can_carry_n(actor), opr, atoi(val),buf);
					else
					{ 
						if( mob )
							bug (" Mob: %d bad argument to maxcarrynum", mob->pIndexData->vnum );
						return -1;
					}
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( can_carry_n(mob->fighting), opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( can_carry_n(actor->fighting), opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 'i':
					if ( mob )
						return mprog_veval( can_carry_n(mob), opr, atoi(val),buf);
					else
						return -1;
				case 'r':
					if ( rndm)
						return mprog_veval( can_carry_n(rndm), opr,atoi(val),buf);
					else
						return -1;
				case 'b':
					if ( remb)
						return mprog_veval( can_carry_n(rndm), opr,atoi(val),buf);
					else
						return -1;
				default:
					if( mob )
						bug (" Mob: %d bad argument to 'maxcarrynum'",mob->pIndexData->vnum);
					return -1;
			}
	}
	
	//add at 2020/06/14
	if ( !str_cmp( buf, "cancarry") )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )
			{
				case 'n':
					if ( actor )
						return mprog_veval( can_carry_n(actor) - actor->carry_number, opr, atoi(val),buf);
					else
					{ 
						if( mob )
							bug (" Mob: %d bad argument to cancarry", mob->pIndexData->vnum );
						return -1;
					}
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( can_carry_n(mob->fighting) - mob->fighting->carry_number , opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( can_carry_n(actor->fighting) - actor->fighting->carry_number , opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 'i':
					if ( mob )
						return mprog_veval( can_carry_n(mob) - mob->carry_number, opr, atoi(val),buf);
					else
						return -1;
				case 'r':
					if ( rndm)
						return mprog_veval( can_carry_n(rndm) - rndm->carry_number, opr,atoi(val),buf);
					else
						return -1;
				case 'b':
					if ( remb)
						return mprog_veval( can_carry_n(rndm) - rndm->carry_number, opr,atoi(val),buf);
					else
						return -1;
				default:
					if( mob )
						bug (" Mob: %d bad argument to 'cancarry'",mob->pIndexData->vnum);
					return -1;
			}
	}
	
	//add at 2020/06/14
	if ( !str_cmp( buf, "canweight") )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )
			{
				case 'n':
					if ( actor )
						return mprog_veval( can_carry_w(actor) - actor->carry_weight, opr, atoi(val),buf);
					else
					{ 
						if( mob )
							bug (" Mob: %d bad argument to canweight", mob->pIndexData->vnum );
						return -1;
					}
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( can_carry_w(mob->fighting) - mob->fighting->carry_weight , opr, atoi( val ) ,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( can_carry_w(actor->fighting) - actor->fighting->carry_weight , opr, atoi( val ) ,buf);
					}
					else
						return -1;
				case 'i':
					if ( mob )
						return mprog_veval( can_carry_w(mob) - mob->carry_weight, opr, atoi(val),buf);
					else return -1;
				case 'r':
					if ( rndm)
						return mprog_veval( can_carry_w(rndm) - rndm->carry_weight, opr,atoi(val),buf);
					else return -1;
				case 'b':
					if ( remb)
						return mprog_veval( can_carry_w(rndm) - rndm->carry_weight, opr,atoi(val),buf);
					else return -1;
				default:
					if( mob )
						bug (" Mob: %d bad argument to 'canweight'",mob->pIndexData->vnum);
						return -1;
			}
	}


	if ( !str_cmp( buf, "pcweight") )
	{
		if ( arg[1] == '\0' )
		{
			lhsvl = 0;
			/* 改寫加入 obj 判定 2022/12/19
			if ( !mob )
				vch = room->people;
			else
				vch = mob->in_room->people; */

			if ( room )
				vch = room->people;
			else if ( mob )
				vch = mob->in_room->people;
			else if ( obj ){
				if( obj->carried_by )
					vch = obj->carried_by->in_room->people;
				else if( obj->in_room )
					vch = obj->in_room->people;
			}

			for( ; vch; vch = vch->next_in_room )
			{
				if ( !IS_NPC(vch) && !IS_IMMORTAL(vch))
				{
					lhsvl += vch->carry_weight;
					if ( ( obj = get_eq_char( vch, WEAR_RIDE ) ) )
					{
						lhsvl -= obj->weight;
						lhsvl += get_obj_weight( obj );
					}
					for ( vobj = vch->in_room->contents; vobj; vobj = vobj->next_content )
						lhsvl += get_obj_weight( vobj );
				}
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
		else
		{
			vnumptr = arg;
			vnumptr ++;

			if ( !is_number( vnumptr ) ){
				if( mob )
					bug ( "Mob: %d npcnum ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d npcnum ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d npcnum ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}
			if ( !( vroom = get_room_index( atoi( vnumptr ) ) ) ){
				if( mob )
					bug ( "Mob: %d npcnum ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d npcnum ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d npcnum ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}

			if( !vroom )
				return -1;

			lhsvl = 0;
			for( vch = vroom->people; vch; vch = vch->next_in_room )
			{
				if ( !IS_NPC(vch) && !IS_IMMORTAL(vch))
				{
					lhsvl += vch->carry_weight;
					if ( ( obj = get_eq_char( vch, WEAR_RIDE ) ) )
					{
						lhsvl -= obj->weight;
						lhsvl += get_obj_weight( obj );
					}
					for ( vobj = vch->in_room->contents; vobj; vobj = vobj->next_content )
						lhsvl += get_obj_weight( vobj );
				}
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
	}

	if ( !str_cmp( buf, "mpnum" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->mana, opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->mana, opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->mana, opr, atoi( val ),buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->mana, opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->mana, opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->mana, opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->mana, opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'mp num'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'mpnum'", mob->pIndexData->vnum ); 
					return TRUE;
				}
					return mprog_veval( victim->mana, opr, atoi( val ),buf);
		}
	}

     //add at 2018/10/24 check for max mana
  if ( !str_cmp( buf, "mpmax" ) )
    {
      if ( arg[0] == '$' )
            switch ( arg[1] )  /* arg should be "$*" so just get the letter */
      {
    case 'i': rhsvl = atoi( val );
    	  if( mob )
            return mprog_veval( mob->max_mana, opr, rhsvl,buf);
        else
            return -1;
    case 'n': if ( actor ) return mprog_veval( actor->max_mana, opr, atoi( val ),buf);
          else return -1;
    case 't': if ( vict ) return mprog_veval( vict->max_mana, opr, atoi( val ),buf);
          else return -1;
    case 'r': if ( rndm ) return mprog_veval( rndm->max_mana, opr, atoi( val ),buf);
          else return -1;
    case 'b': if ( remb ) return mprog_veval( remb->max_mana, opr, atoi( val ),buf);
          else return -1;
    default:
    	if( mob )
          bug ( "Mob: %d bad argument to 'mp max'", mob->pIndexData->vnum ); 
      return -1;
      }
          else
          {
        vnumptr = arg;
    if ( !( victim = get_char_room( mob, vnumptr ) ) )
          if ( !( victim = get_char_world( mob, vnumptr ) ) )
      {
      	if( mob )
            bug ( "Mob: %d bad argument to 'mpmax'", mob->pIndexData->vnum ); 
            return TRUE;
        }
    return mprog_veval( victim->max_mana, opr, atoi( val ),buf);
        }
    }

	if ( !str_cmp( buf, "mvnum" ) )
	{
		if ( arg[0] == '$' )
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				rhsvl = atoi( val );
				if( mob )
					return mprog_veval( mob->move, opr, rhsvl,buf);
				else
					return -1;
			case 'n':
				if ( actor )
					return mprog_veval( actor->move, opr, atoi( val ),buf);
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					return mprog_veval( mob->fighting->move, opr, atoi( val ),buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					return mprog_veval( actor->fighting->move, opr, atoi( val ),buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
					return mprog_veval( vict->move, opr, atoi( val ),buf);
				else
					return -1;
			case 'r':
				if ( rndm )
					return mprog_veval( rndm->move, opr, atoi( val ),buf);
				else
					return -1;
			case 'b':
				if ( remb )
					return mprog_veval( remb->move, opr, atoi( val ),buf);
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'mv num'", mob->pIndexData->vnum ); 
				return -1;
		}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'mvnum'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->move, opr, atoi( val ),buf);
		}
	}

	//add at 2018/10/24 check for max move
	if ( !str_cmp( buf, "mvmax" ) )
	{
		if ( arg[0] == '$' )
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				rhsvl = atoi( val );
				if( mob )
					return mprog_veval( mob->max_move, opr, rhsvl,buf);
				else
					return -1;
			case 'n':
				if ( actor )
					return mprog_veval( actor->max_move, opr, atoi( val ),buf);
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					return mprog_veval( mob->fighting->max_move, opr, atoi( val ),buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					return mprog_veval( actor->fighting->max_move, opr, atoi( val ),buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
					return mprog_veval( vict->max_move, opr, atoi( val ),buf);
				else
					return -1;
			case 'r':
				if ( rndm )
					return mprog_veval( rndm->max_move, opr, atoi( val ),buf);
				else
					return -1;
			case 'b':
				if ( remb )
					return mprog_veval( remb->max_move, opr, atoi( val ),buf);
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'mv max'", mob->pIndexData->vnum ); 
				return -1;
		}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'mvmax'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->max_move, opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "primeattr" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i':
					if( mob )
						bug("primeattr can't use on mob %d!\n\r", mob->pIndexData->vnum);
						return FALSE;
				case 'n':
					if ( actor && !IS_NPC(actor))
					{
						lhsvl = actor->pcdata->prime_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting && !IS_NPC(mob->fighting) )
					{
						lhsvl = mob->fighting->pcdata->prime_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC(actor->fighting) )
					{
						lhsvl = actor->fighting->pcdata->prime_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 't':
					if ( vict && !IS_NPC(vict))
					{
						lhsvl = vict->pcdata->prime_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 'r':
					if ( rndm && !IS_NPC(rndm))
					{
						lhsvl = rndm->pcdata->prime_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 'b':
					if ( remb && !IS_NPC(remb))
					{
						lhsvl = remb->pcdata->prime_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'primeattr'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			if ( !( victim = get_char_world( mob, arg ) ) || IS_NPC(victim))
				return FALSE;
			else
				return mprog_veval( victim->pcdata->prime_attr, opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "sndattr" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i':
					if( mob )
						bug("second attr can't use on mob %d!\n\r", mob->pIndexData->vnum);
					return FALSE;
				case 'n':
					if ( actor && !IS_NPC(actor))
					{
						lhsvl = actor->pcdata->snd_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting && !IS_NPC(mob->fighting) )
					{
						lhsvl = mob->fighting->pcdata->snd_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC(actor->fighting) )
					{
						lhsvl = actor->fighting->pcdata->snd_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 't':
					if ( vict && !IS_NPC(vict))
					{
						lhsvl = vict->pcdata->snd_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 'r':
					if ( rndm && !IS_NPC(rndm))
					{
						lhsvl = rndm->pcdata->snd_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				case 'b':
					if ( remb && !IS_NPC(remb))
					{
						lhsvl = remb->pcdata->snd_attr;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return FALSE;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'second attr'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			if ( !( victim = get_char_world( mob, arg ) ) || IS_NPC(victim))
				return FALSE;
			else
				return mprog_veval( victim->pcdata->snd_attr, opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "inroom" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i': 
					if( mob )
					{
						lhsvl = mob->in_room->vnum;
						rhsvl = atoi(val);
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'n':
					if ( actor )
					{
						lhsvl = actor->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && !IS_NPC(obj->carried_by) )
					{
						return mprog_veval( obj->carried_by->in_room->vnum, opr, atoi(val),buf);
					}
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						lhsvl = mob->fighting->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						lhsvl = actor->fighting->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						lhsvl = vict->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						lhsvl = rndm->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						lhsvl = remb->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'o':
					if ( obj )
					{
						if (obj->carried_by)
							lhsvl = obj->carried_by->in_room->vnum;
						else
							lhsvl = obj->in_room->vnum;
							//lhsvl = obj->in_room->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'inroom'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			if ( !( victim = get_char_world( mob, arg ) ) )
				return -1;
			else
				return mprog_veval( victim->in_room->vnum, opr, atoi( val ),buf);
		}
	}

	/* Added by Keric for count realexp for some area reentrance bound */
	if ( !str_cmp( buf, "realexp") )
	{
		switch ( arg[1] ) /* arg should be "$*" so just get the letter */
		{
			case 'i':
				return -1;
			case 'n':
				if ( actor && !IS_NPC(actor) ) // 加入 PC 判定 2022/10/08
				{
					lhsvl = actor->exp;
					lhsvl += actor->practice * 5000;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && !IS_NPC(mob->fighting) )
				{
					lhsvl = mob->fighting->exp;
					lhsvl += mob->fighting->practice * 5000;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC(actor->fighting) )
				{
					lhsvl = actor->fighting->exp;
					lhsvl += actor->fighting->practice * 5000;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
				}
				else
					return -1;
			case 'r':
				return -1;
			case 'b':
				if ( remb )
				{
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'realexp'", mob->pIndexData->vnum );
				return -1;
		}
	}

	
	// 計算持有的skexp 與上限skexp的差值
	if ( !str_cmp( buf, "skexp_dif" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				return -1; 
			case 'n':
				if ( actor && !IS_NPC(actor) ) // 加入 PC 判定 2022/10/08
				{
					lhsvl = get_skill_exp(actor) - get_max_skill_exp(actor);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && !IS_NPC(mob->fighting) )
				{
					lhsvl = get_skill_exp(mob->fighting) - get_max_skill_exp(mob->fighting);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC(actor->fighting) )
				{
					lhsvl = get_skill_exp(actor->fighting) - get_max_skill_exp(actor->fighting);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_skill_exp(vict) - get_max_skill_exp(vict);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r': 
				return -1;
			case 'b':
				if ( remb )
				{
				lhsvl = get_skill_exp(remb) - get_max_skill_exp(remb);
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'Skexp_dif'", mob->pIndexData->vnum ); 
				return -1;
		}
	}
	
	if ( !str_cmp( buf, "skexp" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				return -1; 
			case 'n':
				if ( actor && !IS_NPC(actor) ) // 加入 PC 判定 2022/10/08
				{
					lhsvl = get_skill_exp(actor);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && !IS_NPC(mob->fighting) )
				{
					lhsvl = get_skill_exp(mob->fighting);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC(actor->fighting) )
				{
					lhsvl = get_skill_exp(actor->fighting);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict && !IS_NPC(vict) )
				{
					lhsvl = get_skill_exp(vict);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r': // 加回 rndm 的 skexp 判定 2022/10/12 
				if ( rndm && !IS_NPC(rndm) )
				{
					lhsvl = get_skill_exp(rndm);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb && !IS_NPC(remb) )
				{
					lhsvl = get_skill_exp(remb);
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'skexp'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "sex" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
				{
					lhsvl = mob->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'n':
				if ( actor )
				{
					lhsvl = actor->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = mob->fighting->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = actor->fighting->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = vict->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = rndm->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = remb->sex;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'sex'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "position" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
		case 'i': 
			if( mob )
			{
				lhsvl = mob->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'n':
			if ( actor )
			{
				lhsvl = actor->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'v': // 2022/10/08
			//if ( actor && actor->fighting )
			if ( mob && mob->fighting )
			{
				lhsvl = mob->fighting->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else if ( obj && obj->carried_by && actor && actor->fighting )
			{
				lhsvl = actor->fighting->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 't':
			if ( vict )
			{
				lhsvl = vict->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'r':
			if ( rndm )
			{
				lhsvl = rndm->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'b':
			if ( remb )
			{
				lhsvl = remb->position;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		default:
			if( mob )
				bug ( "Mob: %d bad argument to 'position'", mob->pIndexData->vnum ); 
			return -1;
		}
	}

	// 判定 pc 的 autoc_combat 狀態 2022/10/12
	if ( !str_cmp( buf, "is_autocombat" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
		case 'i': 
			return -1;
		case 'n':
			if ( actor && !IS_NPC(actor) )
			{
				lhsvl = actor->pcdata->autocombat;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'v':
			//if ( actor && actor->fighting )
			if ( mob && mob->fighting && !IS_NPC(actor) )
			{
				lhsvl = mob->fighting->pcdata->autocombat;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC(actor) )
			{
				lhsvl = actor->fighting->pcdata->autocombat;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 't':
			if ( vict && !IS_NPC(vict) )
			{
				lhsvl = vict->pcdata->autocombat;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'r':
			if ( rndm && !IS_NPC(rndm) )
			{
				lhsvl = rndm->pcdata->autocombat;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		case 'b':
			if ( remb && !IS_NPC(remb) )
			{
				lhsvl = remb->pcdata->autocombat;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			}
			else
				return -1;
		default:
			if( mob )
				bug ( "Mob: %d bad argument to 'position'", mob->pIndexData->vnum ); 
			return -1;
		}
	}

	// 體型判定 2022/12/24
	if ( !str_cmp( buf, "size" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_size( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_size( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_size( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_size( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_curr_size( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_size( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_size( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'size'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	// race 體型判定 2022/12/24
	if ( !str_cmp( buf, "race_size" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_size( mob ) - mob->mod_size;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_size( actor ) - actor->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_size( mob->fighting ) - mob->fighting->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_size( actor->fighting ) - actor->fighting->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_curr_size( vict ) - vict->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_size( rndm ) - rndm->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_size( remb ) - remb->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'race_size'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	// 裝備體型判定 2022/12/24
	if ( !str_cmp( buf, "aff_size" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = mob->mod_size;
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = actor->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				if ( mob && mob->fighting )
				{
					lhsvl = mob->fighting->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = actor->fighting->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = vict->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = rndm->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = remb->mod_size;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'aff_size'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "dex" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_dex( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_dex( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_dex( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_dex( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_curr_dex( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_dex( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_dex( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'dex'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "int" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_int( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_int( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_int( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_int( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;

			case 't':
				if ( vict )
				{
					lhsvl = get_curr_int( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_int( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_int( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'int'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "wis" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_wis( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_wis( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_wis( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_wis( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_curr_wis( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_wis( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_wis( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'wis'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "con" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_con( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_con( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_con( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_con( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_curr_con( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_con( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_con( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'con'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "str" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_curr_str( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_curr_str( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_curr_str( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_curr_str( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_curr_str( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_curr_str( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_curr_str( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'str'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "max_str" ) )  // add at 2022/05/08
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_max_str( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_max_str( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_max_str( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_max_str( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_max_str( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_max_str( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_max_str( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'max_str'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "max_int" ) )   // add at 2022/05/08
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_max_int( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_max_int( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_max_int( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_max_int( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_max_int( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_max_int( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_max_int( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'max_int'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "max_wis" ) )   // add at 2022/05/08
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_max_wis( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_max_wis( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_max_wis( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_max_wis( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_max_wis( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_max_wis( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_max_wis( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'max_wis'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "max_dex" ) )  // add at 2022/05/08
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_max_dex( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_max_dex( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_max_dex( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_max_dex( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_max_dex( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_max_dex( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_max_dex( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'max_dex'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "max_con" ) )  // add at 2022/05/08
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_max_con( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_max_con( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_max_con( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_max_con( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_max_con( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_max_con( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_max_con( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'max_con'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "level" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				lhsvl = get_trust( mob );
				rhsvl = atoi( val );
				return mprog_veval( lhsvl, opr, rhsvl,buf);
			case 'n':
				if ( actor )
				{
					lhsvl = get_trust( actor );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = get_trust( mob->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = get_trust( actor->fighting );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = get_trust( vict );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = get_trust( rndm );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = get_trust( remb );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'o':
				if ( obj )
				{
					lhsvl = obj->level;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'level'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "class" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
				{
					lhsvl = mob->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'n':
				if ( actor )
				{
					lhsvl = actor->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = mob->fighting->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = actor->fighting->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = vict->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = rndm->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = remb->class;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'class'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "goldamt" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
				{
					lhsvl = mob->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'n':
				if ( actor )
				{
					lhsvl = actor->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					lhsvl = mob->fighting->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					lhsvl = actor->fighting->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					lhsvl = vict->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					lhsvl = rndm->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					lhsvl = remb->gold;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'goldamt'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "objtype" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj )
				{
					lhsvl = obj->item_type;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->item_type;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'objstat'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'objstat'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}
	
	if ( !str_cmp( buf, "is_equip" ) ) // add at 2022/05/07
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj ){
					if( obj->wear_loc == WEAR_NONE )
						return FALSE;
					else
						return TRUE;
				}
				else
					return -1;

			case 'p':
				if ( v_obj )
				{
					if( v_obj->wear_loc == WEAR_NONE )
						return FALSE;
					else
						return TRUE;
				}
				else 
					return -1;

			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'is_equip'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'is_equip'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}
	// 判定特定 obj 當前裝備的位置(wear_loc) 2022/11/02
	if ( !str_cmp( buf, "is_equ_loc" ) )
	{
		if ( arg[0] == '$' ){
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'o':
					if( obj )
					{
						lhsvl = obj->wear_loc;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else 
						return -1;

				case 'p':
					if ( v_obj )
					{
						lhsvl = v_obj->wear_loc;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else 
						return -1;

				default:
					if( mob )
						bug ("Mob: %d bad argument to 'is_equ_loc'", mob->pIndexData->vnum );
					return -1;
			}
		}
	}

	if ( !str_cmp( buf, "objweight" ) ) // 2022/12/20
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj )
				{
					lhsvl = get_obj_weight( obj );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = get_obj_weight( v_obj );
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else 
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'objweight'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'objweight'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "objflag" ) ) // add at 2022/12/12
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj )
				{
					lhsvl = obj->extra_flags;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->extra_flags;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else 
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'objflag'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'objflag'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "objval0" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj )
				{
					lhsvl = obj->value[0];
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->value[0];
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else 
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'objval0'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'objval0'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "objval1" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj )
				{
					lhsvl = obj->value[1];
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->value[1];
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'objval1'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'objval1'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "objval2" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'o':
				if ( obj )
				{
					lhsvl = obj->value[2];
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->value[2];
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl, buf);
				}
				else
					return -1;
			default:
				if( mob )
					bug ( "Mob: %d bad argument to 'objval2'", mob->pIndexData->vnum ); 
				else if( v_obj ) 
					bug ( "Obj: %d bad argument to 'objval2'", v_obj->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "objval3" ) )
	{
		if( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'o':
					if ( obj )
					{
						lhsvl = obj->value[3];
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl, buf);
					}
					else
						return -1;
				case 'p':
				if ( v_obj ) 
					{
						lhsvl = v_obj->value[3];
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl, buf);
					}
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'objval3'", mob->pIndexData->vnum ); 
					else if( v_obj ) 
						bug ( "Obj: %d bad argument to 'objval3'", v_obj->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			obj = NULL;

			if( actor )
				obj = get_obj_here( actor, arg );
			else if( mob )
				obj = get_obj_here( mob, arg );
			else
				return -1;

			if( !obj )
				return -1;

			lhsvl = obj->value[3];
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
	}

	/* Added by Razgriz 20050921
	syntax : objextra ( victim , object , field ) opr val
	victim : self , $o   (不需有object參數)
	         owner, $n   (obj攜帶者)
	         char_id     (obj/obj攜帶者 同房間的char)
	         room        (obj/obj攜帶者 房間地上)
	object : obj name, obj vnum
	field  : used v0 v1 t0 t1
	opr    : == != > < <= =< >= => & | !&
	val    : 數字
	*/
	if ( !str_cmp( buf, "objextra" ) )
	{
		OBJ_DATA *exobj = NULL;
		char *exarg, *tmparg;

		exobj = obj_find ( obj, obj->carried_by, arg );

		//skip front arguments
		exarg = tmparg = arg;
		while ( *tmparg )
		{
			//cause obj_find will exchange ',' to ' '
			if ( *tmparg == ' ' )
				exarg = tmparg + 1;
			tmparg++;
		}

		if ( !exobj )
		{
			bug ( "Obj ifchck objextra: %d can't find target", obj->pIndexData->vnum );
			return -1;
		}
		else
		{
			rhsvl = atoi( val );

			if ( !str_cmp ( exarg, "used" ) )
				return mprog_veval( ( exobj->extra_value ).used, opr, rhsvl,buf);
			else if ( !str_cmp ( exarg, "v0" ) )
				return mprog_veval( ( exobj->extra_value ).v[0], opr, rhsvl,buf);
			else if ( !str_cmp ( exarg, "v1" ) )
				return mprog_veval( ( exobj->extra_value ).v[1], opr, rhsvl,buf);
			else if ( !str_cmp ( exarg, "t0" ) )
				return mprog_veval( ( exobj->extra_value ).t[0], opr, rhsvl,buf);
			else if ( !str_cmp ( exarg, "t1" ) )
				return mprog_veval( ( exobj->extra_value ).t[1], opr, rhsvl,buf);
			else if ( obj )
				bug ( "Obj ifchck objextra: %d bad argument to 'field'", obj->pIndexData->vnum);

			return -1;
		}
	}

	if ( !str_cmp( buf, "number" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
				{
					lhsvl = mob->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'n':
				if ( actor )
				{
					if( IS_NPC( actor ) )
					{
						lhsvl = actor->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && IS_NPC( mob->fighting ) )
				{
					lhsvl = mob->fighting->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && IS_NPC( actor->fighting ) )
				{
					lhsvl = actor->fighting->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
				{
					if( IS_NPC( actor ) )
					{
						lhsvl = vict->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
				}
				else
					return -1;
			case 'r':
				if ( rndm )
				{
					if( IS_NPC( actor ) )
					{
						lhsvl = rndm->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
				}
				else
					return -1;
			case 'b':
				if ( remb )
				{
					if( IS_NPC( actor ) )
					{
						lhsvl = remb->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
				}
				else
					return -1;
			case 'o':
				if ( obj )
				{
					lhsvl = obj->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:                                         
				if( mob )
					bug ( "Mob: %d bad argument to 'number'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf , "roomvnum" ) ) // add by coconet
	{
		if ( obj )
		{
			if ( obj->carried_by )
				return mprog_veval( actor->in_room->vnum, opr, atoi( val ),buf);
			else
				return mprog_veval( obj->in_room->vnum, opr, atoi( val ),buf);
		}
		if ( actor )
		{
			if ( IS_NPC(actor))
				return mprog_veval( actor->in_room->vnum, opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "material" ) ) // added by Amethyst
	{
		switch ( arg[1] )
		{
			case 'o': 
				if ( obj )
					return mprog_seval( obj->material->name, opr, val , buf );
				else
					return -1;

			default:
				if ( room )
					bug ( "Room: %d bad argument to 'material'", room->vnum ); 
				if ( mob )
					bug ( "Mob: %d bad argument to 'material'", mob->pIndexData->vnum ); 
				return -1;
		}
	}
  
	if ( !str_cmp( buf, "sector" ) ) // added by Amethyst
	{
		if ( arg[1] == '\0' )
		{
			if ( room )
				lhsvl = room->sector_type;
			else if ( mob )
				lhsvl = mob->in_room->sector_type;
			else if ( obj ){
				if (obj->carried_by)
					lhsvl = obj->carried_by->in_room->sector_type;
				else 
					lhsvl = obj->in_room->sector_type;
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
	}

	if ( !str_cmp( buf, "ishorse" ) ) // added by Amethyst
	{
		switch ( arg[1] )
		{
			case 'n':
				if ( actor )
					return IS_SET( actor->act, PLR_HORSE ) ? TRUE : FALSE;
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					return IS_SET( mob->fighting->act, PLR_HORSE ) ? TRUE : FALSE;
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					return IS_SET( actor->fighting->act, PLR_HORSE ) ? TRUE : FALSE;
				}
				else
					return -1;
			case 't':
				if ( vict )
					return IS_SET( vict->act, PLR_HORSE ) ? TRUE : FALSE;
				else
					return -1;
			case 'r':
				if ( rndm )
					return IS_SET( rndm->act, PLR_HORSE ) ? TRUE : FALSE;
				else
					return -1;
			default:
				if ( room )
					bug ( "Room: %d bad argument to 'ishorse'", room->vnum ); 
				if ( mob )
					bug ( "Mob: %d bad argument to 'ishorse'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "name" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i': 
				if( mob )
					return mprog_seval( mob->name, opr, val, buf);
				else
					return -1;
			case 'n':
				if ( actor )
					return mprog_seval( actor->name, opr, val, buf);
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting )
				{
					return mprog_seval( mob->fighting->name, opr, val, buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					return mprog_seval( actor->fighting->name, opr, val, buf);
				}
				else
					return -1;
			case 't':
				if ( vict )
					return mprog_seval( vict->name, opr, val, buf);
				else
					return -1;
			case 'r':
				if ( rndm )
					return mprog_seval( rndm->name, opr, val, buf);
				else
					return -1;
			case 'b':
				if ( remb )
					return mprog_seval( remb->name, opr, val, buf);
				else
					return -1;
			case 'o':
				if ( obj )
					return mprog_seval( obj->name, opr, val, buf);
				else
					return -1;
			case 'p':
				if ( v_obj )
					return mprog_seval( v_obj->name, opr, val, buf);
				else
					return -1;
			default:                                       
				if( mob )
					bug ( "Mob: %d bad argument to 'name'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

/* The following ifcheck are added by Amenda */

	if ( !str_cmp( buf, "hour" ) )
	{
		lhsvl = time_info.hour;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "day" ) )
	{
		lhsvl = time_info.day;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "month" ) )
	{
		lhsvl = time_info.month;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "weather") )
	{
		lhsvl = weather_info.sky;
		rhsvl = atoi(val);
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
  
	if ( !str_cmp( buf, "nplayer" ) )
	{
		if( !mob ) return -1;
		victim = mob;
		if( !victim->in_room ) return -1;
		lhsvl = victim->in_room->area->nplayer;
		/*for ( d = descriptor_list; d; d = d->next )
		{
			if ( d->connected == CON_PLAYING
			&& ( victim = d->character )
			&& !IS_NPC( victim )
			&& victim->in_room
			&& victim->in_room->area == mob->in_room->area )
			lhsvl += 1;
		}*/
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	// add at 20/03/04
	if ( !str_cmp( buf, "oplayer" ) )
	{
		if( !obj ) return -1;
		if( obj->carried_by )
			lhsvl = obj->carried_by->in_room->area->nplayer;
		else
			lhsvl = obj->in_room->area->nplayer;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
    
	if ( !str_cmp( buf, "pcnum" ) )
	{
		if ( arg[1] == '\0' )
		{
			lhsvl = 0;
			/*  改寫加入 obj 判定 2022/12/19
			if ( !mob )
				vch = room->people;
			else
				vch = mob->in_room->people; */

			if ( room )
				vch = room->people;
			else if ( mob )
				vch = mob->in_room->people;
			else if ( obj ){
				if( obj->carried_by )
					vch = obj->carried_by->in_room->people;
				else if( obj->in_room )
					vch = obj->in_room->people;
			}

			for( ; vch; vch = vch->next_in_room )
			{
				if ( !IS_NPC( vch ) && !IS_IMMORTAL(vch))
					lhsvl += 1;
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
		else
		{
			vnumptr = arg;
			vnumptr ++;

			if ( !is_number( vnumptr ) ){
				if( mob )
					bug ( "Mob: %d pcnum ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d pcnum ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d pcnum ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}
			if ( !( vroom = get_room_index( atoi( vnumptr ) ) ) ){
				if( mob )
					bug ( "Mob: %d pcnum ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d pcnum ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d pcnum ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}

			if( !vroom )
				return -1;

			lhsvl = 0;
			for(vch = vroom->people; vch; vch = vch->next_in_room )
			{
				if ( !IS_NPC( vch ) && !IS_IMMORTAL(vch) )
					lhsvl += 1;
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
	}

	if ( !str_cmp( buf, "npcnum" ) )
	{
		if ( arg[1] == '\0' )
		{
			lhsvl = 0;
			/*  改寫加入 obj 判定 2022/12/19
			if ( !mob )
				vch = room->people;
			else
				vch = mob->in_room->people; */

			if ( room )
				vch = room->people;
			else if ( mob )
				vch = mob->in_room->people;
			else if ( obj ){
				if( obj->carried_by )
					vch = obj->carried_by->in_room->people;
				else if( obj->in_room )
					vch = obj->in_room->people;
			}

			for( ; vch; vch = vch->next_in_room )
			{
				if ( IS_NPC( vch ) )
					lhsvl += 1;
			}

			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
		else
		{
			vnumptr = arg;
			vnumptr ++;

			if ( !is_number( vnumptr ) ){
				if( mob )
					bug ( "Mob: %d npcnum ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d npcnum ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d npcnum ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}
			if ( !( vroom = get_room_index( atoi( vnumptr ) ) ) ){
				if( mob )
					bug ( "Mob: %d npcnum ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d npcnum ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d npcnum ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}

			if( !vroom )
				return -1;

			lhsvl = 0;
			for(vch = vroom->people; vch; vch = vch->next_in_room )
			{
				if ( IS_NPC( vch ) )
					lhsvl += 1;
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
	}

	if ( !str_cmp( buf, "roomchar" ) )
	{
		if ( arg[1] == '\0' )
		{
			lhsvl = 0;
			/*  改寫加入 obj 判定 2022/12/19
			if ( !mob )
				vch = room->people;
			else
				vch = mob->in_room->people; */

			if ( room )
				vch = room->people;
			else if ( mob )
				vch = mob->in_room->people;
			else if ( obj ){
				if( obj->carried_by )
					vch = obj->carried_by->in_room->people;
				else if( obj->in_room )
					vch = obj->in_room->people;
			}

			//for(vch = room->people; vch; vch = vch->next_in_room )
			for( ; vch; vch = vch->next_in_room )
			{
				lhsvl += 1;
			}

			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl, buf );
		}
		else
		{
			vnumptr = arg;
			vnumptr ++;

			if ( !is_number( vnumptr ) ){
				if( mob )
					bug ( "Mob: %d roomchar ifchck invalid room vnum.", mob->pIndexData->vnum );
				// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d roomchar ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d roomchar ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}
			if ( !( vroom = get_room_index( atoi( vnumptr ) ) ) ){
				if( mob )
					bug ( "Mob: %d roomchar ifchck invalid room vnum.", mob->pIndexData->vnum );
				/// 加入 room 及 obj bug log 及 直接 return -1 2022/12/19
				if( room )
					bug ( "Room: %d roomchar ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d roomchar ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}

			if( !vroom )
				return -1;

			lhsvl = 0;
			for(vch = vroom->people; vch; vch = vch->next_in_room )
				lhsvl ++;

			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
	}

	if ( !str_cmp( buf, "rmax_char" ) ) // add at 2022/12/18
	{
		if ( arg[1] == '\0' )
		{
			lhsvl = 0;
			if ( room )
				lhsvl = room->max_char;
			else if ( mob )
				lhsvl = mob->in_room->max_char;
			else if ( obj ){
				if( obj->carried_by )
					lhsvl = obj->carried_by->in_room->max_char;
				else if( obj->in_room )
					lhsvl = obj->in_room->max_char;
			}

			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl, buf );
		}
		else
		{
			vnumptr = arg;
			vnumptr ++;

			if ( !is_number( vnumptr ) ){
				if( mob )
					bug ( "Mob: %d rmax_char ifchck invalid room vnum.", mob->pIndexData->vnum );
				if( room )
					bug ( "Room: %d rmax_char ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d rmax_char ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}
			if ( !( vroom = get_room_index( atoi( vnumptr ) ) ) ){
				if( mob )
					bug ( "Mob: %d rmax_char ifchck invalid room vnum.", mob->pIndexData->vnum );
				if( room )
					bug ( "Room: %d rmax_char ifchck invalid room vnum.", room->vnum );
				if( obj )
					bug ( "Obj: %d rmax_char ifchck invalid room vnum.", obj->pIndexData->vnum );
				return -1;
			}

			if( !vroom )
				return -1;

			lhsvl = 0;

			lhsvl = vroom->max_char;

			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl, buf );
		}
	}

	if ( !str_cmp( buf, "rmobcount" ) )
	{
		int vnum;
		if( mob ){
			if ( !is_number( arg ) )
				bug ( "Mob: %d rmobcount invalid vnum or zero count.", mob->pIndexData->vnum );
			vnum = atoi(arg);
			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Mob: %d rmobcount invalid vnum or zero count.", mob->pIndexData->vnum );
			if( mob->in_room )
				in_room = mob->in_room;
			else{
				bug("Mob :rmobcount %d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if ( room ){
			if ( !is_number( arg ) )
				bug ( "Room: %d rmobcount invalid vnum or zero count.", room->vnum );
			vnum = atoi(arg);
			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Room: %d rmobcount invalid vnum or zero count.", room->vnum );
			in_room = room;
		}
		// obj 判定 2021/12/16
		else if ( obj ){
			if ( !is_number( arg ) )
				bug ( "Obj: %d rmobcount invalid vnum or zero count.", obj->pIndexData->vnum );
			vnum = atoi(arg);
			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Obj: %d rmobcount invalid vnum or zero count.", obj->pIndexData->vnum );
			if( obj->in_room )
				in_room = obj->in_room;
			else if ( obj->carried_by )
				in_room = obj->carried_by->in_room;
			else{
				bug("obj :rmobcount %d has no in_room.", obj->pIndexData->vnum);
				return FALSE;
			}
		}
		else
		{
			bug ( "Illegal use of rmobcount!",0 );
			return -1;
		}
		lhsvl = 0;
		for( vch = char_list; vch; vch = vch->next )
			if ( IS_NPC( vch ) && vch->pIndexData->vnum == vnum && !vch->deleted && vch->in_room == in_room )
				lhsvl ++;

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	// 新增 area mobcount 2022/10/06
	if ( !str_cmp( buf, "amobcount" ) )
	{
		int vnum;
		if( mob )
		{
			if ( !is_number( arg ) )
				bug ( "Mob: %d amobcount invalid vnum .", mob->pIndexData->vnum );

			vnum = atoi(arg);
			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Mob: %d amobcount invalid vnum .", mob->pIndexData->vnum );

			if( mob->in_room )
				in_room = mob->in_room->area;
			else{
				bug("Mob :amobcount %d has no area.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if (room)
		{
			if ( !is_number( arg ) )
				bug ( "Room: %d amobcount invalid vnum .", room->vnum );

			vnum = atoi(arg);

			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Room: %d amobcount invalid vnum .", room->vnum );
			in_room = room->area;
		}
		else if ( obj )
		{
			if ( !is_number( arg ) )
				bug ( "Obj: %d amobcount invalid vnum or zero count.", obj->pIndexData->vnum );
			vnum = atoi(arg);
			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Obj: %d amobcount invalid vnum or zero count.", obj->pIndexData->vnum );
			if ( obj->carried_by )
				in_room = obj->carried_by->in_room->area;
			else if( obj->in_room )
				in_room = obj->in_room->area;
			else{
				bug("obj :amobcount %d has no area.", obj->pIndexData->vnum);
				return FALSE;
			}
		}else{
			bug ( "Illegal use of amobcount!",0 );
			return -1;
		}
		lhsvl = 0;

		for( vch = char_list; vch; vch = vch->next )
			if ( IS_NPC( vch ) && vch->pIndexData->vnum == vnum && !vch->deleted && vch->in_room->area == in_room )
				lhsvl ++;

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "mobcount" ) )
	{
		int vnum;
		if( mob )
		{
			if ( !is_number( arg ) )
				bug ( "Mob: %d mobcount invalid vnum .", mob->pIndexData->vnum );

			vnum = atoi(arg);
			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Mob: %d mobcount invalid vnum .", mob->pIndexData->vnum );

			if( mob->in_room )
				in_room = mob->in_room;
			else{
				bug("Mob :%d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if (room)
		{
			if ( !is_number( arg ) )
				bug ( "Room: %d mobcount invalid vnum .", room->vnum );

			vnum = atoi(arg);

			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Room: %d mobcount invalid vnum .", room->vnum );
			in_room = room;
		}
		else if (obj)  //1050705 moidfy by jye
		{
			if ( !is_number( arg ) )
				bug ( "Obj: %d mobcount invalid vnum .", obj->pIndexData->vnum );

			vnum = atoi(arg);

			if ( !( vmob = get_mob_index( vnum) ) )
				bug ( "Obj: %d mobcount invalid vnum .", obj->pIndexData->vnum );
			in_room = obj->in_room;
		}else{
			bug ( "Illegal use of mobcount!",0 );
			return -1;
		}
				
		lhsvl = 0;

		for( vch = char_list; vch; vch = vch->next )
			if ( IS_NPC( vch ) && vch->pIndexData->vnum == vnum && !vch->deleted )
				lhsvl ++;

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	// robjcount 無法判定被持有的物品
	if ( !str_cmp( buf, "robjcount" ) )
	{
		/*if ( !is_number( arg )){
			if( mob )
				bug ( "Mob robjcount : %d invalid vnum.", mob->pIndexData->vnum );
			else if (room)
				bug( "Room robjcount : %d invalid vnum.", room->vnum);
			return FALSE;
		}
		if ( !( cobj = get_obj_index( atoi(arg) ) ) )
		{
			if( mob )
				bug ( "Mob robjcount : %d no object has such vnum.", mob->pIndexData->vnum );
			else if ( room )
				bug("Room robjcount : %d no ojbects has such vnum.", room->vnum);
			return FALSE;
		}
		if( mob ) {
			if( mob->in_room )
			{
				in_room = mob->in_room;
			}
			else
			{
				bug("Mod robjcount :%d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if ( room )
			in_room = room;
		// obj 判定 2021/12/16
		else if ( obj ) {
			if( obj->in_room )
				in_room = obj->in_room;
			else if ( obj->carried_by )
				in_room = obj->carried_by->in_room;
			else
			{
				bug("Obj robjcount :%d has no in_room.", obj->pIndexData->vnum);
				return FALSE;
			}
		} */

		int cobj_vnum;
		if( mob ){
			if ( !is_number( arg ) )
				bug ( "Mob robjcount : %d invalid vnum.", mob->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Mob: %d robjcount invalid vnum or zero count.", mob->pIndexData->vnum );
			if( mob->in_room )
				in_room = mob->in_room;
			else{
				bug("Mob :robjcount %d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if ( room ){
			if ( !is_number( arg ) )
				bug ( "Room robjcount : %d invalid vnum.", room->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Room: %d robjcount invalid vnum or zero count.", room->vnum );
			in_room = room;
		}
		// obj 判定 2021/12/16
		else if ( obj ){
			if ( !is_number( arg ) )
				bug ( "Obj robjcount : %d invalid vnum.", obj->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Obj: %d robjcount invalid vnum or zero count.", obj->pIndexData->vnum );
			if( obj->in_room )
				in_room = obj->in_room;
			else if ( obj->carried_by )
				in_room = obj->carried_by->in_room;
			else{
				bug("Obj :robjcount %d has no in_room.", obj->pIndexData->vnum);
				return FALSE;
			}
		}
		else
		{
			bug("Obj robjcount :%d has no in_room.", obj->pIndexData->vnum);
			return FALSE;
		}

		lhsvl = 0;
		for( vobj = object_list; vobj; vobj = vobj->next )
			if ( !vobj->deleted && vobj->pIndexData->vnum == cobj_vnum && vobj->in_room == in_room)
				lhsvl ++;

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	
	// 新增 robjcount_a 判定房間內特定物品數 2022/10/06
	if ( !str_cmp( buf, "robjcount_a" ) )
	{
		int cobj_vnum;
		int vobj_inroom;
		if( mob ){
			if ( !is_number( arg ) )
				bug ( "Mob robjcount_a : %d invalid vnum.", mob->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Mob: %d robjcount_a invalid vnum or zero count.", mob->pIndexData->vnum );
			if( mob->in_room )
				in_room = mob->in_room;
			else{
				bug("Mob :robjcount_a %d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if ( room ){
			if ( !is_number( arg ) )
				bug ( "Room robjcount_a : %d invalid vnum.", room->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Room: %d robjcount_a invalid vnum or zero count.", room->vnum );
			in_room = room;
		}
		// obj 判定 2021/12/16
		else if ( obj ){
			if ( !is_number( arg ) )
				bug ( "Obj robjcount_a : %d invalid vnum.", obj->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Obj: %d robjcount_a invalid vnum or zero count.", obj->pIndexData->vnum );
			if( obj->in_room )
				in_room = obj->in_room;
			else if ( obj->carried_by )
				in_room = obj->carried_by->in_room;
			else{
				bug("Obj :robjcount_a %d has no in_room.", obj->pIndexData->vnum);
				return FALSE;
			}
		}
		else
		{
			bug("Obj robjcount_a :%d has no in_room.", obj->pIndexData->vnum);
			return FALSE;
		}

		lhsvl = 0;
		for( vobj = object_list; vobj; vobj = vobj->next )
			if ( !vobj->deleted && vobj->pIndexData->vnum == cobj_vnum ){
				if ( vobj->carried_by )
					vobj_inroom = vobj->carried_by->in_room->vnum;
				else if ( vobj->in_room )
					vobj_inroom = vobj->in_room->vnum;
				if( !vobj_inroom )
					continue;
				if( vobj_inroom == in_room->vnum )
					lhsvl++;
			}

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	// 新增 aobjcount 判定 area 中非被持有的特定物品數 2022/10/06
	if ( !str_cmp( buf, "aobjcount" ) )
	{
		int cobj_vnum;
		int vobj_lvnum;
		int cobj_lvnum;
		if( mob ){
			if ( !is_number( arg ) )
				bug ( "Mob aobjcount : %d invalid vnum.", mob->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Mob: %d aobjcount invalid vnum or zero count.", mob->pIndexData->vnum );
			if( mob->in_room )
				in_room = mob->in_room;
			else{
				bug("Mob :aobjcount %d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if ( room ){
			if ( !is_number( arg ) )
				bug ( "Room aobjcount : %d invalid vnum.", room->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Room: %d aobjcount invalid vnum or zero count.", room->vnum );
			in_room = room;
		}
		// obj 判定 2021/12/16
		else if ( obj ){
			if ( !is_number( arg ) )
				bug ( "Obj aobjcount : %d invalid vnum.", obj->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Obj: %d aobjcount invalid vnum or zero count.", obj->pIndexData->vnum );
			if( obj->in_room )
				in_room = obj->in_room;
			else if ( obj->carried_by )
				in_room = obj->carried_by->in_room;
			else{
				bug("Obj :aobjcount %d has no in_room.", obj->pIndexData->vnum);
				return FALSE;
			}
		}
		else
		{
			bug("Obj aobjcount :%d has no in_room.", obj->pIndexData->vnum);
			return FALSE;
		}

		cobj_lvnum = in_room->area->lvnum;

		lhsvl = 0;
		for( vobj = object_list; vobj; vobj = vobj->next ){
			if ( vobj->deleted )
				continue;
			if ( vobj->pIndexData->vnum != cobj_vnum )
				continue;
			if ( vobj->carried_by )
				vobj_lvnum = NULL;
			else if ( vobj->in_room )
				vobj_lvnum = vobj->in_room->area->lvnum;
			if ( !vobj_lvnum )
				continue;
			if( vobj_lvnum == cobj_lvnum )
				lhsvl++;
		}

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	// 新增 aobjcount_a 判定 area 中特定物品數 2022/10/06
	if ( !str_cmp( buf, "aobjcount_a" ) )
	{
		int cobj_vnum;
		int vobj_lvnum;
		int cobj_lvnum;
		if( mob ){
			if ( !is_number( arg ) )
				bug ( "Mob aobjcount_a : %d invalid vnum.", mob->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Mob: %d aobjcount_a invalid vnum or zero count.", mob->pIndexData->vnum );
			if( mob->in_room )
				in_room = mob->in_room;
			else{
				bug("Mob :aobjcount_a %d has no in_room.", mob->pIndexData->vnum);
				return FALSE;
			}
		}
		else if ( room ){
			if ( !is_number( arg ) )
				bug ( "Room aobjcount_a : %d invalid vnum.", room->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Room: %d aobjcount_a invalid vnum or zero count.", room->vnum );
			in_room = room;
		}
		// obj 判定 2021/12/16
		else if ( obj ){
			if ( !is_number( arg ) )
				bug ( "Obj aobjcount_a : %d invalid vnum.", obj->pIndexData->vnum );
			cobj_vnum = atoi(arg);
			if ( !( cobj = get_obj_index( cobj_vnum) ) )
				bug ( "Obj: %d aobjcount_a invalid vnum or zero count.", obj->pIndexData->vnum );
			if( obj->in_room )
				in_room = obj->in_room;
			else if ( obj->carried_by )
				in_room = obj->carried_by->in_room;
			else{
				bug("Obj :aobjcount_a %d has no in_room.", obj->pIndexData->vnum);
				return FALSE;
			}
		}
		else
		{
			bug("Obj aobjcount_a :%d has no in_room.", obj->pIndexData->vnum);
			return FALSE;
		}

		cobj_lvnum = in_room->area->lvnum;

		lhsvl = 0;
		for( vobj = object_list; vobj; vobj = vobj->next ){
			if ( vobj->deleted )
				continue;
			if ( vobj->pIndexData->vnum != cobj_vnum )
				continue;
			if ( vobj->carried_by )
				vobj_lvnum = vobj->carried_by->in_room->area->lvnum;
			else if ( vobj->in_room )
				vobj_lvnum = vobj->in_room->area->lvnum;
			if ( !vobj_lvnum )
				continue;
			if( vobj_lvnum == cobj_lvnum )
				lhsvl++;
		}

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "objcount" ) )
	{
		if ( !is_number( arg )){
			if( mob )
				bug ( "Mob: %d objcount invalid vnum.", mob->pIndexData->vnum );
			else if (room)
				bug( "Room: %d objcount invalid vnum.", room->vnum);
			return FALSE;
		}
		if ( !( cobj = get_obj_index( atoi( arg ) ) ) ){
			if( mob )
				bug ( "Mob: %d no object has such vnum.", mob->pIndexData->vnum );
			else if ( room )
				bug("Room: %d no ojbects has such vnum.", room->vnum);
			return FALSE;
		}

		lhsvl = 0;
		for( vobj = object_list; vobj; vobj = vobj->next )
			if ( vobj->pIndexData == cobj && !vobj->deleted )
				lhsvl ++;

		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "insameroom" ) )
	{
		if ( arg[0] == '$' )
		{                
			switch ( arg[1] )
			{
				case 'i': return TRUE;
				case 'n': return TRUE;
				case 'r':
					if ( rndm &&  mob )
						return ( mob->in_room == rndm->in_room );
					else
						return -1;
				case 'b':
					if ( !( remb ) || !mob )
						return -1;
					else
						return ( mob->in_room == remb->in_room );
				default :
					if( mob )
						bug ( "Mob: %d rembch invalid var.", mob->pIndexData->vnum );
					return -1;
			}
		}

		vnumptr = arg;
		if ( mob )
		{
			if ( get_char_room( mob, vnumptr ) )
				return TRUE;
		}
		else if ( room_find_char( room, vnumptr ) )
			return TRUE;

		return FALSE;
	}

	if ( !str_cmp( buf, "notbeing" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i': return FALSE;
				case 'n': return FALSE;
				case 'v':  // 2022/10/08
					if ( mob )
					{
						if ( mob->fighting )
							return FALSE;
						else
							return TRUE;
					}
					else if ( obj && obj->carried_by && actor )
					{
						if ( actor->fighting )
							return FALSE;
						else
							return TRUE;
					}
					else
						return -1;
				case 'r':
					if ( rndm && !rndm->deleted)
						return FALSE;
					else
						return -1;
				case 'b':
					if ( remb && !remb->deleted)
						return FALSE;
					else
						return TRUE;
				default : 
					if( mob )
						bug ( "Mob: %d rembch invalid var.", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( get_char_room( mob, vnumptr ) )
					return FALSE;
				else if ( get_char_world( mob, vnumptr ) )
					return FALSE;
			}
			else if ( room_find_char( room, vnumptr ) )
				return FALSE;
			else if ( world_find_char( room, vnumptr ) )
				return FALSE;

		return TRUE;
		}
	}
	if ( !str_cmp( buf, "being" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i': return TRUE;
				case 'n': return TRUE;
				case 'v':  // 2022/10/08
					if ( mob )
					{
						if ( mob->fighting )
							return TRUE;
						else
							return FALSE;
					}
					else if ( obj && obj->carried_by && actor )
					{
						if ( actor->fighting )
							return TRUE;
						else
							return FALSE;
					}
					else
						return -1;
				case 'r':
					if ( rndm )
						return TRUE;
					else
						return -1;
				case 'b':
					if ( remb )
						return TRUE;
					else
						return FALSE;
				default : 
					if( mob )
						bug ( "Mob: %d rembch invalid var.", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( get_char_room( mob, vnumptr ) )
					return TRUE;
				else if ( get_char_world( mob, vnumptr ) )
					return TRUE;
			}
			else if ( room_find_char( room, vnumptr ) )
				return TRUE;
			else if ( world_find_char( room, vnumptr ) )
				return TRUE;

			return FALSE;
		}
	}
	// 判定是否能看見目標 2022/11/14
	if ( !str_cmp( buf, "can_see" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i':
					if( mob && actor ){
						if( can_see( actor , mob ) )
							return TRUE;
						else
							return FALSE;
					}else
						return -1;
				case 'n':
					if( actor ){
						if( mob ){
							if( can_see( mob , actor ) )
								return TRUE;
							else
								return FALSE;
						}else if( obj ){
							if( can_see_obj( actor, obj ) )
								return TRUE;
							else
								return FALSE;
						}else
							return TRUE; // room always true
					}else
						return -1;
				case 'v':
					if ( mob )
					{
						if ( mob->fighting && can_see( mob , mob->fighting ) )
							return TRUE;
						else
							return FALSE;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting && can_see( actor , actor->fighting ) )
					{
						return TRUE;
					}
					else
						return -1;
				case 'r':
					if ( rndm ){
						if ( mob ){ // mob
							if( can_see( mob , rndm ) )
								return TRUE;
							else
								return FALSE;
						}else if( obj ){ // obj
							if( obj->carried_by ){ // obj 被持有
								if( can_see( obj->carried_by , rndm ) )
									return TRUE;
								else
									return FALSE;
							}else{ // obj 未被持有
								if( can_see_obj( rndm, obj ) )
									return TRUE;
								else
									return FALSE;
							}
						}else if( room && actor ){ // room
							if( can_see( actor , rndm ) )
								return TRUE;
							else
								return FALSE;
						}
					}else
						return -1;
				case 'b':
					if ( remb ){
						if ( mob ){ // mob
							if( can_see( mob , remb ) )
								return TRUE;
							else
								return FALSE;
						}else if( obj ){ // obj
							if( obj->carried_by ){ // obj 被持有
								if( can_see( obj->carried_by , remb ) )
									return TRUE;
								else
									return FALSE;
							}else{ // obj 未被持有
								if( can_see_obj( remb, obj ) )
									return TRUE;
								else
									return FALSE;
							}
						}else if( room && actor ){ // room
							if( can_see( actor , remb ) )
								return TRUE;
							else
								return FALSE;
						}
					}else
						return -1;
				default : 
					if( mob )
						bug ( "Mob: %d can_see invalid var.", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( get_char_room( mob, vnumptr ) )
					return TRUE;
				else
					return FALSE;
			}
			else if( room && actor ){
				if ( room_find_char( room, vnumptr ) && can_see( actor , vnumptr ) )
					return TRUE;
				else
					return FALSE;
			}
			return -1;
		}
	}

	if ( !str_cmp( buf, "fightwith" ) )
	{
		int fight_target = 0;
		// 新增變數 val 判定 , 讓 figthwith 的 == 可以用於 $i $n $b $r 2022/11/09
		if ( val[0] == '$' )
		{
			switch ( val[1] )
			{
				default : 
					break;
				case 'i':
					if( mob )
						fight_target = 1;
					break;
				case 'n':
					if ( actor )
						fight_target = 2;
					break;
				case 'r':
					if ( rndm )
						fight_target = 3;
					break;
				case 'b':
					if ( remb )
						fight_target = 4;
					break;
			}
		}
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i': 
					if ( mob ){
						if ( mob->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 1 )
									return FightWithVarCheck(mob , mob , opr );
								if ( fight_target == 2 )
									return FightWithVarCheck(mob , actor , opr );
								if ( fight_target == 3 )
									return FightWithVarCheck(mob , rndm , opr );
								if ( fight_target == 4 )
									return FightWithVarCheck(mob , remb , opr );
								else
									return -1;
							}else
								return mprog_seval( mob->fighting->name, opr, val, buf);
						}
					}
					return -1;
				case 'n':
					if ( actor ){
						if (actor->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 1 )
									return FightWithVarCheck(actor , mob , opr  );
								if ( fight_target == 2 )
									return FightWithVarCheck(actor , actor , opr  );
								if ( fight_target == 3 )
									return FightWithVarCheck(actor , rndm , opr  );
								if ( fight_target == 4 )
									return FightWithVarCheck(actor , remb , opr  );
								else
									return -1;
							}else
								return mprog_seval( actor->fighting->name, opr, val, buf);
						}
					}else if ( obj && obj->carried_by){ // obj 判定 add at 20/03/04
						if ( obj->carried_by->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 2 )
									return FightWithVarCheck(obj->carried_by , actor , opr );
								if ( fight_target == 3 )
									return FightWithVarCheck(obj->carried_by , rndm , opr );
								if ( fight_target == 4 )
									return FightWithVarCheck(obj->carried_by , remb , opr  );
								else
									return -1;
							}else
								return mprog_seval( obj->carried_by->fighting->name, opr, val, buf);
						}
					}
					return -1;
				case 'v':
					if ( mob && mob->fighting ){
						vch = mob->fighting;
						if( vch->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 1 )
									return FightWithVarCheck( vch , mob , opr  );
								if ( fight_target == 2 )
									return FightWithVarCheck( vch , actor , opr  );
								if ( fight_target == 3 )
									return FightWithVarCheck( vch , rndm , opr  );
								if ( fight_target == 4 )
									return FightWithVarCheck( vch , remb , opr  );
								else
									return -1;
							}else
								return mprog_seval( vch->fighting->name, opr, val, buf);
						}
					}else if ( obj && obj->carried_by && actor && obj->carried_by->fighting ){
						vch = obj->carried_by->fighting;
						if ( vch->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 2 )
									return FightWithVarCheck( vch , obj->carried_by , opr );
								if ( fight_target == 3 )
									return FightWithVarCheck( vch , rndm , opr );
								if ( fight_target == 4 )
									return FightWithVarCheck( vch , remb , opr  );
								else
									return -1;
							}else
								return mprog_seval( vch->fighting->name, opr, val, buf);
						}
					}
					return -1;
				case 'r':
					if ( rndm ){
						if ( rndm->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 1 )
									return FightWithVarCheck(rndm , mob , opr );
								if ( fight_target == 2 )
									return FightWithVarCheck(rndm , actor , opr );
								if ( fight_target == 3 )
									return FightWithVarCheck(rndm , rndm , opr );
								if ( fight_target == 4 )
									return FightWithVarCheck(rndm , remb , opr );
								else
									return -1;
							}else
								return mprog_seval( rndm->fighting->name, opr, val, buf);
						}
					}
					return -1;
				case 'b':
					if ( remb ){
						if ( remb->fighting ){
							if ( val[0] == '$' ){
								if ( fight_target == 1 )
									return FightWithVarCheck(remb , mob , opr );
								if ( fight_target == 2 )
									return FightWithVarCheck(remb , actor , opr );
								if ( fight_target == 3 )
									return FightWithVarCheck(remb , rndm , opr );
								if ( fight_target == 4 )
									return FightWithVarCheck(remb , remb , opr );
								else
									return -1;
							}else
								return mprog_seval( remb->fighting->name, opr, val, buf);
						}
					}
					return -1;
				default : 
					if( mob )
						bug ( "Mob: %d fightwith invalid var.", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( !( vch = get_char_room( mob, vnumptr ) ) )
				if ( !( vch = get_char_world( mob, vnumptr ) ) )
					return -1;
			if ( vch->fighting )
				return mprog_seval( vch->fighting->name, opr, val, buf);
			return -1;
		}
	}

	// 2022/11/10
	if ( !str_cmp( buf, "fightvnum" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob && mob->fighting && IS_NPC(mob->fighting) )
					{
						lhsvl = mob->fighting->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'n':
					if ( actor && actor->fighting && IS_NPC(actor->fighting) )
					{
						lhsvl = actor->fighting->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && obj->carried_by->fighting && IS_NPC(obj->carried_by->fighting) )
					{
						lhsvl = obj->carried_by->fighting->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'v':
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
						if( vch->fighting && IS_NPC(vch->fighting) ){
							lhsvl = vch->fighting->pIndexData->vnum;
							rhsvl = atoi( val );
							return mprog_veval( lhsvl, opr, rhsvl,buf);
						}
					}
					else if ( obj && obj->carried_by && obj->carried_by->fighting )
					{
						vch = obj->carried_by->fighting;
						if( vch->fighting && IS_NPC(vch->fighting) ){
							lhsvl = vch->fighting->pIndexData->vnum;
							rhsvl = atoi( val );
							return mprog_veval( lhsvl, opr, rhsvl,buf);
						}
					}
					else
						return -1;
				case 't':
					if ( vict && vict->fighting && IS_NPC(vict->fighting) )
					{
						lhsvl = vict->fighting->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'r':
					if ( rndm && rndm->fighting && IS_NPC(rndm->fighting) )
					{
						lhsvl = rndm->fighting->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'b':
					if ( remb && remb->fighting && IS_NPC(remb->fighting) )
					{
						lhsvl = remb->fighting->pIndexData->vnum;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				default:                                      
					if( mob )
						bug ( "Mob: %d bad argument to 'fightvnum'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		vnumptr = arg;
		if ( mob )
		{
			if ( !( vch = get_char_room( mob, vnumptr ) ) )
			{
				if ( !( vch = get_char_world( mob, vnumptr ) ) )
				{
					bug ( "Mob: %d 'fightvnum' target not exist", mob->pIndexData->vnum ); 
					return -1;
				}
			}
		}
		if ( vch && vch->fighting && IS_NPC(vch->fighting) ){
			lhsvl = vch->fighting->pIndexData->vnum;
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
		return -1;
	}

	if ( !str_cmp( buf, "followwith" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
			case 'i': 
				if ( mob && mob->master )
					return mprog_seval( mob->master->name, opr, val, buf);
			case 'n':
				if ( actor && actor->master )
					return mprog_seval( actor->master->name, opr, val, buf);
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && mob->fighting->master )
				{
					return mprog_seval( mob->fighting->master->name, opr, val, buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && actor->fighting->master)
				{
					return mprog_seval( actor->fighting->master->name, opr, val, buf);
				}
				else
					return -1;
			case 't':
				if ( vict && vict->master )
					return mprog_seval( vict->master->name, opr, val, buf);
				else
					return -1;
			case 'r':
				if ( rndm && rndm->master )
					return mprog_seval( rndm->master->name, opr, val, buf);
				else
					return -1;
			case 'b':
				if ( remb && remb->master )
					return mprog_seval( remb->master->name, opr, val, buf);
				else
					return -1;
			default:                                             
				if( mob )
					bug ( "Mob: %d bad argument to 'followwith'", mob->pIndexData->vnum ); 
				return -1;
			}
		}
		vnumptr = arg;
		if ( !( victim = get_char_room( mob, vnumptr ) ) )
			if ( !( victim = get_char_world( mob, vnumptr ) ) )
			{                                                  
				if( mob )
					bug ( "Mob: %d 'followwith' val not exist", mob->pIndexData->vnum ); 
				return -1;    
			}
		if ( victim->master )
			return mprog_seval( victim->master->name, opr, val, buf);

		return FALSE;
	}

	if ( !str_cmp( buf, "groupwith" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
			case 'i': 
				if ( mob && mob->group && mob->group->leader )
					return mprog_seval( mob->group->leader->name, opr, val, buf);
			case 'n':
				if ( actor && actor->group && actor->group->leader)
					return mprog_seval( actor->group->leader->name, opr, val, buf);
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && mob->fighting->group && mob->fighting->group->leader )
				{
					return mprog_seval( mob->fighting->group->leader->name, opr, val, buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && actor->fighting->group && actor->fighting->group->leader )
				{
					return mprog_seval( actor->fighting->group->leader->name, opr, val, buf);
				}
				else
					return -1;
			case 't':
				if ( vict && vict->group && vict->group->leader )
					return mprog_seval( vict->group->leader->name, opr, val, buf);
				else
					return -1;
			case 'r':
				if ( rndm && rndm->group && rndm->group->leader )
					return mprog_seval( rndm->group->leader->name, opr, val, buf);
				else
					return -1;
			case 'b':
				if ( remb && remb->group && remb->group->leader )
					return mprog_seval( remb->group->leader->name, opr, val, buf);
				else
					return -1;
			default:                                             
				if( mob )
					bug ( "Mob: %d bad argument to 'followwith'", mob->pIndexData->vnum ); 
				return -1;
			}
		}
		vnumptr = arg;
		if ( !( victim = get_char_room( mob, vnumptr ) ) )
			if ( !( victim = get_char_world( mob, vnumptr ) ) )
			{                                                 
				if( mob )
					bug ( "Mob: %d 'groupwith' val not exist", mob->pIndexData->vnum ); 
				return -1;    
			}
		if ( victim->group && victim->group->leader )
			return mprog_seval( victim->group->leader->name, opr, val, buf);

		return FALSE;
	}

	if ( !str_cmp( buf, "exp" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob )
					{
						lhsvl = mob->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'n':
					if ( actor )
					{
						lhsvl = actor->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						lhsvl = mob->fighting->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						lhsvl = actor->fighting->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						lhsvl = vict->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						lhsvl = rndm->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						lhsvl = remb->exp;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				default:                                      
					if( mob )
						bug ( "Mob: %d bad argument to 'exp'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		vnumptr = arg;
		if ( mob )
		{
			if ( !( vch = get_char_room( mob, vnumptr ) ) )
			{
				if ( !( vch = get_char_world( mob, vnumptr ) ) )
				{
					bug ( "Mob: %d 'exp' target not exist", mob->pIndexData->vnum ); 
					return -1;
				}
			}
		}
		else if ( !( vch = room_find_char( room, vnumptr ) ) )
		{
			if ( !( vch = world_find_char( room, vnumptr ) ) )
			{
				bug ( "Room: %d 'exp' target not exist", room->vnum ); 
				return -1;
			}
		}
		lhsvl = vch->exp;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	
	//add at 2018/10/24 check for practice point
	if ( !str_cmp( buf, "practice" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i': 
					if( mob )
					{
						lhsvl = mob->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'n':
					if ( actor )
					{
						lhsvl = actor->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						lhsvl = mob->fighting->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						lhsvl = actor->fighting->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						lhsvl = vict->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						lhsvl = rndm->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						lhsvl = remb->practice;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				default:                                      
					if( mob )
						bug ( "Mob: %d bad argument to 'practice'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		vnumptr = arg;
		if ( mob )
		{
			if ( !( vch = get_char_room( mob, vnumptr ) ) )
			{
				if ( !( vch = get_char_world( mob, vnumptr ) ) )
				{
					bug ( "Mob: %d 'practice' target not exist", mob->pIndexData->vnum ); 
					return -1;
				}
			}
		}
		else if ( !( vch = room_find_char( room, vnumptr ) ) )
		{
			if ( !( vch = world_find_char( room, vnumptr ) ) )
			{
				bug ( "Room: %d 'practice' target not exist", room->vnum ); 
				return -1;
			}
		}
		lhsvl = vch->practice;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "resp" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'n':
					if ( actor )
					{
						if ( IS_NPC( actor ) )
							return -1;
						lhsvl = actor->pcdata->respect;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting && !IS_NPC( mob->fighting ) )
					{
						lhsvl = mob->fighting->pcdata->respect;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else if ( obj && obj->carried_by && actor && actor->fighting && !IS_NPC( actor->fighting ) )
					{
						lhsvl = actor->fighting->pcdata->respect;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
					{
						if ( IS_NPC( vict ) )
							return -1;
						lhsvl = vict->pcdata->respect;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'r':
					if ( rndm )
					{
						if ( IS_NPC( rndm ) )
							return -1;
						lhsvl = rndm->pcdata->respect;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				case 'b':
					if ( remb )
					{
						if ( IS_NPC( remb ) )
							return -1;
						lhsvl = remb->pcdata->respect;
						rhsvl = atoi( val );
						return mprog_veval( lhsvl, opr, rhsvl,buf);
					}
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'resp'", mob->pIndexData->vnum ); 
					else if ( room )
						bug ("Room %d bad argument to 'resp'", room->vnum);
					return -1;
			}
		}
		vnumptr = arg;
		if ( mob )
		{
			if ( !( vch = get_char_room( mob, vnumptr ) ) )
				if ( !( vch = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d 'pcdata->respect' target not exist", mob->pIndexData->vnum ); 
					return -1;
				}
		}
		else if ( !( vch = room_find_char( room, vnumptr ) ) )
			if ( !( vch = world_find_char( room, vnumptr ) ) )
			{
				bug ( "Room: %d 'pcdata->respect' target not exist", room->vnum ); 
				return -1;
			}
		if ( IS_NPC( vch ) )
			return -1;
		lhsvl = vch->pcdata->respect;
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "vnum" ) )
	{
		switch ( arg[1] )  /* arg should be "$*" so just get the letter */
		{
			case 'i':
				if( mob ) 
				{
					lhsvl = mob->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else 
					return -1;
			case 'n':
				if ( actor && IS_NPC( actor ) )
				{
					lhsvl = actor->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting )
				if ( mob && mob->fighting && IS_NPC( mob->fighting ) )
				{
					lhsvl = mob->fighting->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else if ( obj && obj->carried_by && actor && actor->fighting && IS_NPC( actor->fighting ) )
				{
					lhsvl = actor->fighting->pIndexData->vnum;
					rhsvl = atoi( val );
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 't':
				if ( vict && IS_NPC( vict ) )
				{
					lhsvl = vict->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'r':
				if ( rndm && IS_NPC( rndm ) )
				{
					lhsvl = rndm->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'b':
				if ( remb && IS_NPC( remb ) )
				{
					lhsvl = remb->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'o':
				if ( obj )
				{
					lhsvl = obj->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			case 'p':
				if ( v_obj )
				{
					lhsvl = v_obj->pIndexData->vnum;
					rhsvl = atoi( val ); 
					return mprog_veval( lhsvl, opr, rhsvl,buf);
				}
				else
					return -1;
			default:                                       
				if( mob )
					bug ( "Mob: %d bad argument to 'vnum'", mob->pIndexData->vnum ); 
				return -1;
		}
	}

	if ( !str_cmp( buf, "skillexp" ) )
	{
		char* argument;
		int sn = 0;
		vch = NULL;
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					if ( mob )
						vch = mob;
					else
						return -1;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
					vch = remb;
					else
						return -1;
					break;
				default:  
					if( mob )
						bug ( "Mob: %d bad argument to 'skillexp'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						if( mob )
							bug ( "Mob: %d 'skillexp' val not exist", mob->pIndexData->vnum );
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'skillexp' val not exist", room->vnum );
					return -1;
				} 
		}

		vnumptr = val;

		sn = atoi( val );
		if( !skill_table[sn].name ) {
			if( room ) 
				bug(" Room: %d 'skillexp' no such sn", room->vnum );
			else if( mob )
				bug("Mob: %d 'skillexp' no such sn", mob->pIndexData->vnum);
			return -1;
		}

		argument = one_argument( val, vnumptr ); //modified by coconet 0512
		rhsvl = atoi( argument );
		lhsvl = get_skill_level( vch, sn ); // Modified by Amethyst
		// bug("Mob: sn %d ", sn);
		// bug("Mob: sklv %d", lhsvl);
		// bug("Mob: want %d", rhsvl);
		// bug("Mob: opr %s", opr);
		// lhsvl = get_skill_exp( vch);

		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "learned" ) )
	{
		char * argument;	//Added by Razgriz	20050714
		int sn = 0;
		vch = NULL;
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					if ( mob )
						vch = mob;
					else
						return -1;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
						vch = remb;
					else
						return -1;
					break;
				default:  
					if( mob )
						bug ( "Mob: %d bad argument to 'learned'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{                                               
						if( mob )
							bug ( "Mob: %d 'learned' val not exist", mob->pIndexData->vnum );
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'learned' val not exist", room->vnum );
					return -1;
				}
		}

		vnumptr = val;
        
		sn = atoi( val );
		if( !skill_table[sn].name ) {
			if( room ) 
				bug(" Room: %d 'learned' no such sn", room->vnum );
			else if( mob )
				bug("Mob: %d 'learned' no such sn", mob->pIndexData->vnum);
			return -1;
		}

		argument = one_argument( val, vnumptr ); //Modified by Razgriz 20050714
		rhsvl = atoi( argument );                //Modified by Razgriz 20050714
		lhsvl = get_skill_percent( vch, sn );

		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	// groupsize 只能計算與目標同房間的成員
	if ( !str_cmp( buf, "groupsize" ) )
	{
		vch = NULL;
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					if ( mob )
						vch = mob;
					else
						return -1;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
						vch = remb;
					else
						return -1;
					break;
				default:  
					if( mob )
						bug ( "Mob: %d bad argument to 'groupsize'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{                                                 
						if( mob )
						bug ( "Mob: %d 'groupsize' val not exist", mob->pIndexData->vnum );
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'groupsize' val not exist", room->vnum );
					return -1;
				}
		}

		rhsvl = atoi( val );
		lhsvl = 0;
		if( !vch->in_room ) return FALSE;
		for( vict = vch->in_room->people ; vict; vict = vict->next_in_room ){
			if( !(IS_NPC(vict)^IS_NPC(vch)) && is_same_group( vict, vch ))
				++ lhsvl;
		}

		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	// 新增 groupnum 計算目標團隊的總人數, 不管 pc 還 npc 都計算並且就算在不同 room 也會算 2022/11/28
	if ( !str_cmp( buf, "group_num" ) )
	{
		vch = NULL;
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					if ( mob )
						vch = mob;
					else
						return -1;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v':
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
						vch = remb;
					else
						return -1;
					break;
				default:  
					if( mob )
						bug ( "Mob: %d bad argument to 'group_num'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						if( mob )
						bug ( "Mob: %d 'group_num' val not exist", mob->pIndexData->vnum );
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'group_num' val not exist", room->vnum );
					return -1;
				}
		}

		rhsvl = atoi( val );
		lhsvl = 0;
		if( !vch->in_room ) return FALSE;
		// 目標沒隊伍等同於團隊只有一人
		if( !vch->group )
			return mprog_veval( 1, opr, rhsvl,buf);

		for( vict = char_list; vict; vict = vict->next ){
			if( is_same_group( vict, vch ) )
				++ lhsvl;
		}

		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	// 新增 grouppcnum 計算目標團隊中 pc 的總人數並且就算在不同 room 也會算 2022/11/28
	if ( !str_cmp( buf, "group_pcnum" ) )
	{
		vch = NULL;
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					if ( mob )
						vch = mob;
					else
						return -1;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v':
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
						vch = remb;
					else
						return -1;
					break;
				default:  
					if( mob )
						bug ( "Mob: %d bad argument to 'group_pcnum'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						if( mob )
						bug ( "Mob: %d 'group_pcnum' val not exist", mob->pIndexData->vnum );
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'group_pcnum' val not exist", room->vnum );
					return -1;
				}
		}

		rhsvl = atoi( val );
		lhsvl = 0;
		if( !vch->in_room ) return FALSE;
		// 目標沒隊伍等同於團隊只有一人
		if( !vch->group )
			return mprog_veval( 1, opr, rhsvl,buf);

		for( vict = char_list; vict; vict = vict->next ){
			if( !(IS_NPC(vict)^IS_NPC(vch)) && is_same_group( vict, vch ))
				++ lhsvl;
		}

		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
	if ( !str_cmp( buf, "is_qmark" ) )
	{
		vch = NULL;
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					if ( mob )
						vch = mob;
					else
						return -1;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v': // 2022/10/08
					//if ( actor && actor->fighting )
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
						vch = remb;
					else
						return -1;
					break;
				default: 
					if( mob )
						bug ( "Mob: %d bad argument to 'is_qmark'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{                                                
						if( mob )
							bug ( "Mob: %d 'is_qmark' val not exist", mob->pIndexData->vnum );
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'is_qmark' val not exist", room->vnum );
					return -1;
				}
		}


		vnumptr = val;
		vnumptr = one_argument( vnumptr, mark_name );
		if ( ( qmark = vch->qmark ) )
			for( qmark = vch->qmark; qmark != NULL; qmark = qmark->next )
				if ( !str_cmp( qmark->mark_name, mark_name ) )
					break;

		if ( !qmark )
		{
			if ( !str_cmp( opr, "!=" ) )
				return TRUE;
			else
				return FALSE;
		}

		if ( *vnumptr == '\0' )
		{
			if ( !str_cmp( opr, "!=" ) )
				return FALSE;
			else
				return TRUE;
		}

		mark_value = atoi( vnumptr );
		// 加入 qmark 死亡消失 及 離線保存 判定 2022/10/23
		if ( mark_value <= 10 ) // qmark 10 以下 , death 判定為假
		{
			if( qmark->fDeath ){
				lhsvl = 1;
				rhsvl = 0;
			}else{
				lhsvl = 0;
				rhsvl = 0;
			}
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		if ( mark_value < 20 ) // qmark 11 到 19 時 , death 判定為真
		{
			if( qmark->fDeath ){
				lhsvl = 1;
				rhsvl = 1;
			}else{
				lhsvl = 0;
				rhsvl = 1;
			}
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		if ( mark_value == 20 )  // qmark 20 , save 判定為假
		{
			if( qmark->fSave ){
				lhsvl = 1;
				rhsvl = 0;
			}else{
				lhsvl = 0;
				rhsvl = 0;
			}
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		if ( mark_value < 30 )  // qmark 21 ~ 29 , save 判定為真
		{
			if( qmark->fSave ){
				lhsvl = 1;
				rhsvl = 1;
			}else{
				lhsvl = 0;
				rhsvl = 1;
			}
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		if ( mark_value < 2000 )
		{
			lhsvl = qmark->timer;
			rhsvl = mark_value - 1000;
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		if ( mark_value < 200000 )
		{
			lhsvl = qmark->v0;
			rhsvl = mark_value - 100000;
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		if ( mark_value < 300000 )
		{
			lhsvl = qmark->v1;
			rhsvl = mark_value - 200000;
			return mprog_veval( lhsvl, opr, rhsvl, buf);
		}
		lhsvl = qmark->v2;
		rhsvl = mark_value - 300000;
		return mprog_veval( lhsvl, opr, rhsvl, buf);
	}

	if ( !str_cmp( buf, "is_inv_obj" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i':
					vch = mob;
					break;
				case 'n':
					vch = actor;
					break;
				case 'v': // 2022/10/08
					// vch = actor->fighting;
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					vch = vict;
					break;
				case 'r':
					vch = rndm;
					break;
				case 'b':
					vch = remb;
					break;
				default:                                            
					if( mob )
						bug ("Mob: %d bad argument to 'is_inv_obj'", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
				{
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						if( mob )
							bug ( "Mob: %d 'is_inv_obj' val not exist", mob->pIndexData->vnum ); 
						return -1;
					}
				}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
			{
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'is_inv_obj' val not exist", room->vnum ); 
					return -1;
				}
			}
		}

		if ( get_obj_carry( vch, val ) )
		{
			if ( !str_cmp( opr, "==" ) )
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if ( !str_cmp( opr, "==" ) )
				return FALSE;
			else
				return TRUE;
		}
	}

	if ( !str_cmp( buf, "is_equ_obj" ) )
	{
		if ( arg[0] == '$' ){
			switch ( arg[1] )
			{
				case 'i':
					vch = mob;
					break;
				case 'n':
					vch = actor;
					break;
				case 'v': // 2022/10/08
					//vch = actor->fighting;
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					vch = vict;
					break;
				case 'r':
					vch = rndm;
					break;
				case 'b':
					vch = remb;
					break;
				default:
					if( mob )
						bug ("Mob: %d bad argument to 'is_equ_obj'", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
				{
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						if( mob )
							bug ( "Mob: %d 'is_equ_obj' val not exist", mob->pIndexData->vnum ); 
						return -1;
					}
				}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
			{
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'is_equ_obj' val not exist", room->vnum ); 
					return -1;
				}
			}
		}
		if (!vch)
		{
		//    if (mob)
		//    {
		//            bug ("Mob: %d bad argument to 'is_equ_obj'", mob->pIndexData->vnum );
			return -1;
		//    }
		//    else
		//    {
		//      bug ( "Room: %d 'is_equ_obj' val not exist", 
		//        room->vnum ); 
		//      return -1;
		//    }
		}
		if ( get_obj_wear2( vch, val ) )
		{
			if ( !str_cmp( opr, "==" ) )
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if ( !str_cmp( opr, "==" ) )
				return FALSE;
			else
				return TRUE;
		}
	}
	// add at 20/03/05
	if ( !str_cmp( buf, "is_wear_loc" ) )
	{
		if ( arg[0] == '$' ){
			switch ( arg[1] )
			{
				case 'i':
					vch = mob;
					break;
				case 'n':
					vch = actor;
					break;
				case 'v': // 2022/10/08
					//vch = actor->fighting;
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					vch = vict;
					break;
				case 'r':
					vch = rndm;
					break;
				case 'b':
					vch = remb;
					break;
				default:
					if( mob )
						bug ("Mob: %d bad argument to 'is_wear_loc'", mob->pIndexData->vnum );
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
				{
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						if( mob )
							bug ( "Mob: %d 'is_wear_loc' val not exist", mob->pIndexData->vnum ); 
						return -1;
					}
				}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
			{
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'is_wear_loc' val not exist", room->vnum ); 
					return -1;
				}
			}
		}

		if (!vch) return -1;

		if( !is_number( val ) )
		{
			bug("Vnum: %d 'is_wear_loc' -> wear_loc should be proper defined. ", room->vnum);
			return -1;
		}
		else
		{
			int	loc	= 0;
			loc = atoi ( val );
			for ( obj = vch->carrying; obj; obj = obj->next_content )
			{
				if ( !obj ) continue;
				if( obj->wear_loc != WEAR_NONE && obj->wear_loc == loc) {
					if ( !str_cmp( opr, "==" ) )
						return TRUE;
					else
						return FALSE;
				}
			}
			if ( !str_cmp( opr, "==" ) )
				return FALSE;
			else
				return TRUE;
		}
	}

    /* Added by tale 20120502
       	syntax : is_wear_obj ( victim , wear_loc ) opr val
    	victim : $i   (mob)
    	         $n   (ppl)
    	wear_loc : Equpiment wear locations. (defined in parameters.h)
    	opr    : == != > < <= =< >= => & | !&
    	val    : obj vnum
    */
	if ( !str_cmp( buf, "is_wear_obj" ))
	{
		char arg1 [ MAX_INPUT_LENGTH ];
		char arg2 [ MAX_INPUT_LENGTH ];
		char *argument = arg;
		char *pchar	= '\0';
		int loc	= -999;

		smash_tilde ( argument );

		//exchange ',' to ' '
		for ( pchar = argument; *pchar; pchar++ )
		{
			if ( *pchar == ',' )
				*pchar = ' ';
		}
		argument = one_argument ( argument, arg1 );

		if ( arg1[0] == '\0' || argument[0] == '\0' )
		{
			bug("Mob: %d bad argument to 'is_wear_obj'", mob->pIndexData->vnum );
		}
		if (arg1[0] == '$')
		{
			switch (arg1[1])
			{
				case 'i':
					vch = mob;
					break;
				case 'n':
					vch = actor;
					break;
				case 'v': // 2022/10/08
					// vch = actor->fighting;
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if ( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					vch = vict;
					break;
				case 'r':
					vch = rndm;
					break;
				case 'b':
					vch = remb;
					break;
				default:
					if (mob)
					{
						bug ("Mob: %d bad argument to 'is_wear_obj'", mob->pIndexData->vnum );
						return -1;
					}
			}
		}
		else
		{
			vnumptr = arg1;
			if ( mob )
			{
				if (!( vch = get_char_room( mob, vnumptr )))
				{
					if (!( vch = get_char_world( mob, vnumptr )))
					{
						if ( mob )
						{
							bug ( "Mob: %d 'is_wear_obj' val not exist", mob->pIndexData->vnum ); 
							return -1;
						}
					}                         
				}
			}
			else if (!( vch = room_find_char( room, vnumptr )))
			{
				if (!( vch = world_find_char( room, vnumptr )))
				{
					bug ( "Room: %d 'is_wear_obj' val not exist", room->vnum ); 
					return -1;
				}
			}
		}
		argument = one_argument ( argument, arg2 );

		if ( is_number ( arg2 ) )
		{
			loc = atoi ( arg2 );
		}
		else
		{
			bug("Vnum: %d 'is_wear_obj' -> wear_loc should be proper defined. ", room->vnum);
			return FALSE;
		}

		if (!vch) return -1;

		OBJ_DATA * obj = get_obj_wear2( vch, val);

		if (obj)
		{
			if ( loc == obj->wear_loc)
			{
				if (!str_cmp( opr, "==")) return TRUE;
				else return FALSE;
			}
			else
				return FALSE;
		}
		else
		{
			if (!str_cmp( opr, "==")) return FALSE;
			else return TRUE;
		}
	}
	
	// add at 20/03/05
	if ( !str_cmp( buf, "carrycount" ))
	{
		char   arg1 [ MAX_INPUT_LENGTH ];
		char   arg2 [ MAX_INPUT_LENGTH ];
		char * argument = arg;
		char * pchar = '\0';
		int    number;
		int    count;
		int    countnum = 0;
		int    objnum = 0;

		smash_tilde ( argument );
         
		//exchange ',' to ' '
		for ( pchar = argument; *pchar; pchar++ )
		{
			if ( *pchar == ',' )
				*pchar = ' ';
		}
		argument = one_argument ( argument, arg1 );

		if ( arg1[0] == '\0' || argument[0] == '\0' )
		{
			bug("Mob: %d bad argument to 'carrycount'", mob->pIndexData->vnum );
		}
		if (arg1[0] == '$')
		{
			switch (arg1[1])
			{
			case 'i':
				vch = mob;
				break;
			case 'n':
				vch = actor;
				break;
			case 'v': // 2022/10/08
				//if( actor && actor->fighting)
				if ( mob && mob->fighting )
				{
					vch = mob->fighting;
				}
				else if ( obj && obj->carried_by && actor && actor->fighting )
				{
					vch = actor->fighting;
				}
				else
					return -1;
			case 't':
				vch = vict;
				break;
			case 'r':
				vch = rndm;
				break;
			case 'b':
				vch = remb;
				break;
			default:
				if (mob)
				{
					bug ("Mob: %d bad argument to 'carrycount'", mob->pIndexData->vnum );
					return -1;
				}
			}
		}
		else
		{
			vnumptr = arg1;
			if ( mob )
			{
				if (!( vch = get_char_room( mob, vnumptr )))
				{
					if (!( vch = get_char_world( mob, vnumptr )))
					{
						if ( mob )
						{
							bug ( "Mob: %d 'carrycount' val not exist", mob->pIndexData->vnum ); 
							return -1;
						}
					}                         
				}
			}
			else if (!( vch = room_find_char( room, vnumptr )))
			{
				if (!( vch = world_find_char( room, vnumptr )))
				{
					bug ( "Room: %d 'carrycount' val not exist", room->vnum ); 
					return -1;
				}
			}
		}

		argument = one_argument ( argument, arg2 );

		if (!vch) return -1;

		if ( !is_number( arg2 ) )
		{
			number = number_argument( arg2, arg );
			count  = 0;
			for ( obj = vch->carrying; obj; obj = obj->next_content )
			{
				if ( obj->wear_loc == WEAR_NONE
					&& can_see_obj( vch, obj )
					&& is_pfxname( arg , obj->name ) )
				{
					//if ( ++count == number )
						countnum++;
				}
			}
		}
		else
		{
			number = atoi( arg2 );
			for ( obj = vch->carrying; obj; obj = obj->next_content )
			{
				if ( obj->pIndexData->vnum == number 
					&& obj->wear_loc == WEAR_NONE
					&& can_see_obj( vch, obj ))
				{
					countnum++;
				}
			}
		}

		objnum = atoi( val );
		
		return mprog_veval( countnum, opr, objnum,buf);
		/*
				if (!str_cmp( opr, "=="))
				{
					if (countnum == objnum)
						return TRUE;
					else
						return FALSE;
				}
				else if (!str_cmp( opr, ">"))
				{
					if (countnum > objnum)
						return TRUE;
					else
						return FALSE;
				}
				else if (!str_cmp( opr, "<"))
				{
					if (countnum < objnum)
						return TRUE;
					else
						return FALSE;
				}
				else
				{
						return FALSE;
					else
						return TRUE;
				} 
		*/
	}
     
	if ( !str_cmp( buf, "race" ) ){
		if ( arg[0] == '$' ){
			switch ( arg[1] )
			{
				case 'i':
					return(GetMprogSeval_RaceByCharData(mob, opr, val , buf ));
				case 'n':
					return(GetMprogSeval_RaceByCharData(actor, opr, val , buf ));
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						if( mob->fighting->temp_race ){
							return mprog_seval( race_table[mob->fighting->temp_race - 1].name, opr, val, buf);
						}else if(is_affected(mob->fighting, gsn_werewolf)){
							if(is_affected(mob->fighting, gsn_polymorph)){
								return mprog_seval( race_table[mob->fighting->race].name, opr, val, buf);
							}else{
								return mprog_seval( "Werewolf", opr, val, buf);
							}
						}else{
							return mprog_seval( race_table[mob->fighting->race].name, opr, val, buf);
						}
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						if( actor->fighting->temp_race ){
							return mprog_seval( race_table[actor->fighting->temp_race - 1].name, opr, val, buf);
						}else if(is_affected(actor->fighting, gsn_werewolf)){
							if(is_affected(actor->fighting, gsn_polymorph)){
								return mprog_seval( race_table[actor->fighting->race].name, opr, val, buf);
							}else{
								return mprog_seval( "Werewolf", opr, val, buf);
							}
						}else{
							return mprog_seval( race_table[actor->fighting->race].name, opr, val, buf);
						}
					}
					else
						return -1;
				case 't':
					return(GetMprogSeval_RaceByCharData(vict, opr, val, buf ));
				case 'r':
					return(GetMprogSeval_RaceByCharData(rndm, opr, val, buf ));
				case 'b':
					return(GetMprogSeval_RaceByCharData(remb, opr, val, buf ));
				default:
					if( mob ) bug ("Mob: %d bad argument to 'race'", mob->pIndexData->vnum );
					return -1;
				/* Old Version *
					case 'i':
						if( mob )
							return mprog_seval( race_table[( ( mob->temp_race ) ? mob->temp_race - 1 : mob->race )].name, opr, val );
						else
							return -1;
					case 'n': 
						if ( actor )
							return mprog_seval(race_table[( ( actor->temp_race ) ? actor->temp_race - 1 : actor->race )].name, opr, val);
						else 
							return -1;
					case 't': 
						if ( vict )
							return mprog_seval( race_table[( ( vict->temp_race ) ? vict->temp_race - 1 : vict->race )].name, opr, val );
						else 
							return -1;
					case 'r': 
						if ( rndm )
							return mprog_seval( race_table[( ( rndm->temp_race ) ? rndm->temp_race - 1 : rndm->race )].name, opr, val );
						else 
							return -1;
					case 'b': 
						if ( remb )
							return mprog_seval( race_table[( ( remb->temp_race ) ? remb->temp_race - 1 : remb->race )].name, opr, val );
						else 
							return -1;
					default:                                      
						if( mob ) bug ("Mob: %d bad argument to 'race'", mob->pIndexData->vnum );
						return -1;
				***************/
			}
		}else{
			vnumptr = arg;
			if ( !( vch = get_char_world( mob, vnumptr ) ) ){
				if( mob )
					bug ( "Mob: %d 'race' val not exist", mob->pIndexData->vnum ); 
				return -1;
			}
			return mprog_seval( race_table[( ( vch->temp_race ) ? vch->temp_race - 1 : vch->race )].name, opr, val, buf);
		}
	}

	if ( !str_cmp( buf, "rembch" ) )
	{
		if ( arg[0] == '$' )
		switch ( arg[1] )
		{
		case 'i': 
			if( mob )
				if ( mob->rembch && !str_cmp( mob->name, mob->rembch->name ) )
					return TRUE;
				else
					return -1;
			else
				return -1;
		case 'n': 
			if( mob )
				if ( actor && mob->rembch && !str_cmp( actor->name, mob->rembch->name ) )
					return TRUE;
				else
					return -1;
			else
				return -1;
		case 'v': // 2022/10/08
			if ( mob && mob->fighting )
			{
				if ( mob->rembch && !str_cmp( mob->fighting->name, mob->rembch->name ) )
					return TRUE;
				else
					return -1;
			}
			else if( obj && obj->carried_by && actor && actor->fighting ) // obj rember 2022/11/10
			{
				if ( obj->orembch && !str_cmp( actor->fighting->name, obj->orembch->name ) )
					return TRUE;
				else
					return -1;
			}
			else
				return -1;
		case 't':
			if( mob )
				if ( vict && mob->rembch && !str_cmp( vict->name, mob->rembch->name ) )
					return TRUE;
                else
					return -1;
			else
				return -1;
		case 'r': 
			if( mob )
				if ( rndm && mob->rembch && !str_cmp( rndm->name, mob->rembch->name ) )
					return TRUE;
				else
					return -1;
			else
				return -1;
		case 'b':
			if ( remb )
				return TRUE;
			else
				return -1;
		default:                                      
			if( mob )
				bug ("Mob: %d bad argument to 'rembch'", mob->pIndexData->vnum );
			return -1;
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{ 
						if( mob )
							bug ( "Mob: %d 'rembch' val not exist", mob->pIndexData->vnum ); 
						return -1;
					}
				if ( mob->rembch && !str_cmp( mob->rembch->name, vch->name ) )
					return TRUE;
				else
					return FALSE;
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
					if ( !( vch = world_find_char( room, vnumptr ) ) )
					{
						bug ( "Room: %d 'rembch' val not exist", room->vnum ); 
						return -1;
					}
			if ( room->rrembch && !str_cmp( room->rrembch->name, vch->name ) )
				return TRUE;
			else
				return FALSE;
		}
    }

	if ( !str_cmp( buf, "area" ) )
	{
		if ( arg[0] == '$' )
		switch ( arg[1] )
		{
			case 'i':
				if ( mob && mob->in_room )
				{
					if ( mprog_seval( mob->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				return -1;
			case 'n':
				if ( actor && actor->in_room )
				{
					if ( mprog_seval( actor->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				return -1; // 加入 return -1 避免跑到 default 的 bug log 2022/11/08
			case 'v': // 2022/10/08
				//if ( actor && actor->fighting && actor->fighting->in_room )
				if ( mob && mob->fighting && mob->fighting->in_room )
				{
					if ( mprog_seval( mob->fighting->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				else if( obj && obj->carried_by && actor && actor->fighting && actor->fighting->in_room )
				{
					if ( mprog_seval( actor->fighting->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				return -1; // add at 2022/11/08
			case 't':
				if ( vict && vict->in_room )
				{
					if ( mprog_seval( vict->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				return -1; // add at 2022/11/08
			case 'r':
				if ( rndm && rndm->in_room )
				{
					if ( mprog_seval( rndm->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				return -1; // add at 2022/11/08
			case 'b':
				if ( remb && remb->in_room )
				{
					if ( mprog_seval( remb->in_room->area->identity, opr, val, buf) )
						return TRUE;
					else
						return FALSE;
				}
				return -1; // add at 2022/11/08
			default:
				if( mob )
					bug ("Mob: %d bad argument to 'area'", mob->pIndexData->vnum );
				return -1;
		}
        else
        {
			vnumptr = arg;
			if ( !( vch = get_char_world( mob, vnumptr ) ) )
			{
				if( mob )
					bug ( "Mob: %d 'area' val not exist", mob->pIndexData->vnum ); 
				return -1;
			}
			if ( vch->in_room && mprog_seval( vch->in_room->area->identity, opr, val, buf) )
				return TRUE;
			else
				return FALSE;
		}
	}

	if ( !str_cmp( buf, "roomvalue" ) )
	{
		int i;
		int rvnum=0;

		//by Chris, 05/07/31
		if(room)
		{
			if(!room->roomprogs )
			{
				bug( "Roomvalue - Room does not have room_prog:  Room: %d.", room->vnum );
				return -1;
			}
		}
		else if(!vict)
		{
			if(actor)
			{
				//bug( "Error roomvalue in Mob_prog : actor vnum : %d ", actor->pIndexData->vnum );
			}
			return -1;
		}
        /* 
		if(TRUE)
		{
			char chrisbuf[BUF_STRING_LENGTH];
			sprintf(chrisbuf,"by Chris on roomvalue usage : rvnum = %d\n\r",rvnum);
			send_to_all_char(arg);
		}
		*/

		rhsvl = atoi( val );
    
		switch ( arg[1] )
		{
			case '0':
				return mprog_veval( room->roomprogs->v0, opr, rhsvl,buf);
			case '1':
				return mprog_veval( room->roomprogs->v1, opr, rhsvl,buf);
			case 'n':
			{
				for(i=2;i<20;i++)
				{
					if ( arg[i] == 'v' )
					{
						if ( !( vroom = get_room_index( rvnum ) ) )
						{
							if( mob )
								bug ("Room or Mob: %d roomvalue check.", (mob) ? mob->pIndexData->vnum : room->vnum );
								bug ("room %d doesn't exist in the world.", rvnum );
							return -1;
						};
						if ( !( vroom->roomprogs ) )
						{
							bug ("Room: %d doesn't have room_prog but want to use roomvalue", vroom->vnum );
							return -1;
						};
						if ( arg[i+1] == '0' )
							return mprog_veval( vroom->roomprogs->v0, opr, rhsvl,buf);
						else
							return mprog_veval( vroom->roomprogs->v1, opr, rhsvl,buf);
					}
					else
						rvnum = rvnum * 10 + ( (int ) arg[i] - 48 );
				}
			}
			default : 
				bug ("Room or Mob: %d bad argument to roomvalue.", (mob) ? mob->pIndexData->vnum : room->vnum );
				return -1;
		}
	}
	
	if ( !str_cmp( buf, "roomflag" ) ) // add at 2022/01/17
	{
		if ( arg[1] == '\0' )
		{
			if ( room )
				lhsvl = room->room_flags;
			else if ( mob )
				lhsvl = mob->in_room->room_flags;
			else if ( obj )
			{
				if (obj->carried_by)
					lhsvl = obj->carried_by->in_room->room_flags;
				else 
					lhsvl = obj->in_room->room_flags;
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
	}

	if ( !str_cmp( buf, "lightnum" ) ) // add at 2022/01/17
	{
		if ( arg[1] == '\0' )
		{
			if ( room )
				lhsvl = room->light;
			else if ( mob )
				lhsvl = mob->in_room->light;
			else if ( obj )
			{
				if (obj->carried_by)
					lhsvl = obj->carried_by->in_room->light;
				else 
					lhsvl = obj->in_room->light;
			}
			rhsvl = atoi( val );
			return mprog_veval( lhsvl, opr, rhsvl,buf);
		}
	}

	if ( !str_cmp( buf, "islight" ) ) // add at 2022/01/17
	{
		if ( arg[0] == '$' )
		{
			if ( room )
				if ( room_is_dark( room ) )
					return FALSE;
				else
					return TRUE;
			else if ( mob )
				if ( room_is_dark( mob->in_room ) )
					return FALSE;
				else
					return TRUE;
			else if ( obj )
			{
				if ( obj->carried_by )
					if ( room_is_dark( obj->carried_by->in_room ) )
						return FALSE;
					else
						return TRUE;
				else
					if ( room_is_dark( obj->in_room ) )
						return FALSE;
					else
						return TRUE;
			}
			return -1;
		}
		bug ("Islight: first arg should be $.", 0 );
		return -1;
	}
	
	if ( !str_cmp( buf, "isdark" ) ) // add at 2022/01/17
	{
		if ( arg[0] == '$' )
		{
			if ( room )
				if ( room_is_dark( room ) )
					return TRUE;
				else
					return FALSE;
			else if ( mob )
				if ( room_is_dark( mob->in_room ) )
					return TRUE;
				else
					return FALSE;
			else if ( obj )
			{
				if ( obj->carried_by )
					if ( room_is_dark( obj->carried_by->in_room ) )
						return TRUE;
					else
						return FALSE;
				else
					if ( room_is_dark( obj->in_room ) )
						return TRUE;
					else
						return FALSE;
			}
			return -1;
		}
		bug ("Isdark: first arg should be $.", 0 );
		return -1;
	}

	if ( !str_cmp( buf, "rnplayer" ) )
	{
		if( !room ) return -1;
		lhsvl = room->area->nplayer;
		/*for ( d = descriptor_list; d; d = d->next )
		{
			if ( d->connected == CON_PLAYING
			&& ( victim = d->character )
			&& !IS_NPC( victim )
			&& victim->in_room
			&& victim->in_room->area == mob->in_room->area )
			lhsvl += 1;
		}*/
		rhsvl = atoi( val );
		return mprog_veval( lhsvl, opr, rhsvl,buf);
	}
    
	if( !str_cmp(buf, "hasdoor" ))
	{
		int door = atoi(arg);
		if( door < 0 || door > 5 ) {
			bug("Hasdoor - invalid door number", 0);
		return -1;
		}
		//modify by coconet 20050425
		if( mob ) return ( mob->in_room->exit[door] ) ? TRUE : FALSE;
		if( obj )  // add at 20/03/04
		{
			if (obj->carried_by)
				return ( obj->carried_by->in_room->exit[door] ) ? TRUE : FALSE;
			else 
				return ( obj->in_room->exit[door] ) ? TRUE : FALSE;
		}
		if( !room ) return -1;

		return ( room->exit[door] ) ? TRUE : FALSE;
	}

	if ( !str_cmp( buf, "roomtimer" ) )
	{
		int rt = atoi(arg);
		if( !room ) return -1;

		if( rt < 0 || rt > 1 ) {
			bug( "Roomtimer - timer only 0 and 1:  Room: %d.", room->vnum );
			return -1;
		}
		rhsvl = atoi( val );
		if ( !( room->roomprogs ) )
		{
		bug( "Roomtimer - Room does not have room_prog:  Room: %d.", room->vnum );
		return -1;
		}
		//  bug("test roomtimer rt %d", rt );
		return mprog_veval( room->roomprogs->timer[rt], opr, rhsvl,buf);
	}
    
	if ( !str_cmp( buf, "exitinfo" ) )
	{
		if( !room ) return -1;

		if ( !( room->roomprogs ) )
		{
			bug( "Exitinfo - Room does not have room_prog:  Room: %d.", room->vnum );
			return -1;
		}

		vnumptr = arg;
		vnumptr++;
		lhsvl = atoi( vnumptr );
		rhsvl = atoi( val );
		if ( lhsvl > 5 || lhsvl < 0 )
		{
			bug( "Exitinfo - bad val:  Room: %d.", room->vnum );
			return -1;
		}

		return mprog_veval( room->exit[lhsvl]->exit_info, opr, rhsvl,buf);
	}

	if ( !str_cmp( buf, "isaffected" ) )
	{
		if ( arg[0] == '$' )
		{
			switch ( arg[1] )
			{
				case 'i':
					vch = mob;
					break;
				case 'n':
					if ( actor )
						vch = actor;
					else
						return -1;
					break;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						vch = mob->fighting;
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						vch = actor->fighting;
					}
					else
						return -1;
					break;
				case 't':
					if ( vict )
						vch = vict;
					else
						return -1;
					break;
				case 'r':
					if ( rndm )
						vch = rndm;
					else
						return -1;
					break;
				case 'b':
					if ( remb )
						vch = remb;
					else
						return -1;
					break;
				default:  
					if( mob )
						bug ( "Mob: %d bad argument to 'isaffected'", mob->pIndexData->vnum ); 
					return -1;
			}
		}
		else
		{
			vnumptr = arg;
			if ( mob )
			{
				if ( !( vch = get_char_room( mob, vnumptr ) ) )
					if ( !( vch = get_char_world( mob, vnumptr ) ) )
					{
						bug ( "Mob: %d 'isaffected' val not exist", mob->pIndexData->vnum ); 
						return -1;
					}
			}
			else if ( !( vch = room_find_char( room, vnumptr ) ) )
			{
				if ( !( vch = world_find_char( room, vnumptr ) ) )
				{
					bug ( "Room: %d 'isaffected' val not exist", room->vnum ); 
					return -1;
				}
			}
		}
		if ( is_number( val ) )
		{
			/* 避免值過大, 更換 atoi 為 atoll 直接代入 long long 的判定 2022/11/10
			rhsvl = atoi( val );
			return mprog_veval( vch->affected_by, opr, rhsvl,buf); */
			return mprog_longval( vch->affected_by, opr, atoll( val ) , buf);
		}
		rhsvl = skill_lookup( val );
		for ( paf = vch->affected; paf; paf = paf->next )
		{
			if ( paf->deleted )
				continue;
			if ( paf->type == rhsvl )
				return TRUE;
		}
		return FALSE;
	}

	// 新增數個角色數值判定 2022/11/08
	if ( !str_cmp( buf, "sv_spell" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_SPELL), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_SPELL), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_SPELL), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_SPELL), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_SPELL), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_SPELL), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_SPELL), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_spell'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_spell'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_SPELL), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_wind" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_WIND), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_WIND), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_WIND), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_WIND), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_WIND), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_WIND), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_WIND), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_wind'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_wind'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_WIND), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_fire" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_FIRE), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_FIRE), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_FIRE), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_FIRE), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_FIRE), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_FIRE), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_FIRE), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_fire'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_fire'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_FIRE), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_cold" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_COLD), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_COLD), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_COLD), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_COLD), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_COLD), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_COLD), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_COLD), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_cold'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_cold'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_COLD), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_earth" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_EARTH), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_EARTH), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_EARTH), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_EARTH), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_EARTH), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_EARTH), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_EARTH), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_earth'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_earth'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_EARTH), opr, atoi( val ),buf);
		}
	}


	if ( !str_cmp( buf, "sv_breath" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_BREATH), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_BREATH), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_BREATH), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_BREATH), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_BREATH), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_BREATH), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_BREATH), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_breath'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_breath'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_BREATH), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_poison" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_POISON), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_POISON), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_POISON), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_POISON), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_POISON), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_POISON), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_POISON), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_poison'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_poison'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_POISON), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_lightning" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_LIGHTNING), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_LIGHTNING), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_LIGHTNING), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_LIGHTNING), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_LIGHTNING), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_LIGHTNING), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_LIGHTNING), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_lightning'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_lightning'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_LIGHTNING), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_saint" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_SAINT), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_SAINT), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_SAINT), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_SAINT), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_SAINT), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_SAINT), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_SAINT), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_saint'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_saint'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_SAINT), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "sv_dark" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( NEW_GET_SVS(mob, MAGIC_DARK), opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( NEW_GET_SVS(actor, MAGIC_DARK), opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( NEW_GET_SVS(mob->fighting, MAGIC_DARK), opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( NEW_GET_SVS(actor->fighting, MAGIC_DARK), opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( NEW_GET_SVS(vict, MAGIC_DARK), opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( NEW_GET_SVS(rndm, MAGIC_DARK), opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( NEW_GET_SVS(remb, MAGIC_DARK), opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_dark'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'sv_dark'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( NEW_GET_SVS(victim, MAGIC_DARK), opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "pp" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->pp , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->pp , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->pp , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->pp , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->pp , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->pp , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->pp , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'pp'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'pp'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->pp , opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "apdr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->apdr , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->apdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->apdr , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->apdr , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->apdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->apdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->apdr , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'apdr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'apdr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->apdr , opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "pdr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->pdr , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->pdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->pdr , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->pdr , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->pdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->pdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->pdr , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'pdr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'pdr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->pdr , opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "amdr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->amdr , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->amdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->amdr , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->amdr , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->amdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->amdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->amdr , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'amdr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'amdr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->amdr , opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "mdr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( mob->mdr , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( actor->mdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( mob->fighting->mdr , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( actor->fighting->mdr , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( vict->mdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( rndm->mdr , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( remb->mdr , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'mdr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'mdr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( victim->mdr , opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "armor" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( GET_AC( mob ) , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( GET_AC( actor ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( GET_AC( mob->fighting ) , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( GET_AC( actor->fighting ) , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( GET_AC( vict ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( GET_AC( rndm ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( GET_AC( remb ) , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'armor'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'armor'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( GET_AC( victim ) , opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "dodge" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( GET_DODGE( mob ) , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( GET_DODGE( actor ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( GET_DODGE( mob->fighting ) , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( GET_DODGE( actor->fighting ) , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( GET_DODGE( vict ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( GET_DODGE( rndm ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( GET_DODGE( remb ) , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'dodge'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'dodge'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( GET_DODGE( victim ) , opr, atoi( val ),buf);
		}
	}
	
	if ( !str_cmp( buf, "wp_dr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( get_damroll( mob, ( get_eq_char(mob, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( get_damroll( actor, ( get_eq_char(actor, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( get_damroll( mob->fighting, ( get_eq_char(mob->fighting, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( get_damroll( actor->fighting, ( get_eq_char(actor->fighting, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( get_damroll( vict, ( get_eq_char(vict, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( get_damroll( rndm, ( get_eq_char(rndm, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( get_damroll( remb, ( get_eq_char(remb, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'wp_dr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'wp_dr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( get_damroll( victim, ( get_eq_char(victim, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) ) , opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "wp_hr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob )
						return mprog_veval( get_hitroll( mob, ( get_eq_char(mob, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor )
						return mprog_veval( get_hitroll( actor, ( get_eq_char(actor, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting )
					{
						return mprog_veval( get_hitroll( mob->fighting, ( get_eq_char(mob->fighting, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting )
					{
						return mprog_veval( get_hitroll( actor->fighting, ( get_eq_char(actor->fighting, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict )
						return mprog_veval( get_hitroll( vict, ( get_eq_char(vict, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm )
						return mprog_veval( get_hitroll( rndm, ( get_eq_char(rndm, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb )
						return mprog_veval( get_hitroll( remb, ( get_eq_char(remb, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) )
											, opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'wp_hr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'wp_hr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( get_hitroll( victim, ( get_eq_char(victim, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) ) , opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "wp2_dr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob && get_eq_char( mob, WEAR_WIELD_2 ) )
						return mprog_veval( get_damroll( mob, WEAR_WIELD_2 ) , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor && get_eq_char( actor, WEAR_WIELD_2 ) )
						return mprog_veval( get_damroll( actor, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting && get_eq_char( mob->fighting, WEAR_WIELD_2 ) )
					{
						return mprog_veval( get_damroll( mob->fighting , WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting && get_eq_char( actor->fighting, WEAR_WIELD_2 ) )
					{
						return mprog_veval( get_damroll( actor->fighting , WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict && get_eq_char( vict, WEAR_WIELD_2 ) )
						return mprog_veval( get_damroll( vict, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm && get_eq_char( rndm, WEAR_WIELD_2 ) )
						return mprog_veval( get_damroll( rndm, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb && get_eq_char( remb, WEAR_WIELD_2 ) )
						return mprog_veval( get_damroll( remb, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'wp2_dr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'wp2_dr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( get_damroll( victim, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
		}
	}

	if ( !str_cmp( buf, "wp2_hr" ) )
	{
		if ( arg[0] == '$' )
			switch ( arg[1] )  /* arg should be "$*" so just get the letter */
			{
				case 'i':
					rhsvl = atoi( val );
					if( mob && get_eq_char( mob, WEAR_WIELD_2 ) )
						return mprog_veval( get_hitroll( mob, WEAR_WIELD_2 ) , opr, rhsvl,buf);
					else
						return -1;
				case 'n':
					if ( actor && get_eq_char( actor, WEAR_WIELD_2 ) )
						return mprog_veval( get_hitroll( actor, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'v': // 2022/10/08
					if ( mob && mob->fighting && get_eq_char( mob->fighting, WEAR_WIELD_2 ) )
					{
						return mprog_veval( get_hitroll( mob->fighting , WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					}
					else if( obj && obj->carried_by && actor && actor->fighting && get_eq_char( actor->fighting, WEAR_WIELD_2 ) )
					{
						return mprog_veval( get_hitroll( actor->fighting , WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					}
					else
						return -1;
				case 't':
					if ( vict && get_eq_char( vict, WEAR_WIELD_2 ) )
						return mprog_veval( get_hitroll( vict, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'r':
					if ( rndm && get_eq_char( rndm, WEAR_WIELD_2 ) )
						return mprog_veval( get_hitroll( rndm, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				case 'b':
					if ( remb && get_eq_char( remb, WEAR_WIELD_2 ) )
						return mprog_veval( get_hitroll( remb, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
					else
						return -1;
				default:
					if( mob )
						bug ( "Mob: %d bad argument to 'wp2_hr'", mob->pIndexData->vnum ); 
					return -1;
			}
		else
		{
			vnumptr = arg;
			if ( !( victim = get_char_room( mob, vnumptr ) ) )
				if ( !( victim = get_char_world( mob, vnumptr ) ) )
				{
					if( mob )
						bug ( "Mob: %d bad argument to 'wp2_hr'", mob->pIndexData->vnum ); 
					return TRUE;
				}
			return mprog_veval( get_hitroll( victim, WEAR_WIELD_2 ) , opr, atoi( val ),buf);
		}
	}


	/* Ok... all the ifchcks are done, so if we didnt find ours then something
	* odd happened.  So report the bug and abort the MOBprogram (return error)
	*/
	if ( mob )
	{
		//bug ("buf: %s.", buf);
		bug ( "Mob: %d unknown ifchck", mob->pIndexData->vnum );
	}
	else if (room)
		bug ( "Room: %d unknown ifchck", room->vnum );
	else if (obj) // add at 2022/11/08
		bug ( "Obj: %d unknown ifchck", obj->pIndexData->vnum );
	else 
		bug("mprog_if_check: unknown ifchch",0);
	return FALSE;

}
/* Quite a long and arduous function, this guy handles the control
 * flow part of MOBprograms.  Basicially once the driver sees an
 * 'if' attention shifts to here.  While many syntax errors are
 * caught, some will still get through due to the handling of break
 * and errors in the same fashion.  The desire to break out of the
 * recursion without catastrophe in the event of a mis-parse was
 * believed to be high. Thus, if an error is found, it is bugged and
 * the parser acts as though a break were issued and just bails out
 * at that point. I havent tested all the possibilites, so I'm speaking
 * in theory, but it is 'guaranteed' to work on syntactically correct
 * MOBprograms, so if the mud crashes here, check the mob carefully!
 */
char *mprog_process_if( char *ifchck, char *com_list, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo, CHAR_DATA *rndm )
{

	static char null[ 1 ];
	char buf[ MAX_INPUT_LENGTH ];
	char *morebuf = '\0';
	char  *cmnd = '\0';
	bool loopdone = FALSE;
	bool   flag = FALSE;

	*null = '\0';

	if ( mob->deleted )
		return null;  

	/* check for trueness of the ifcheck */
	if ( mprog_do_ifchck( ifchck, mob, actor, obj, vo, rndm, mob->rembch ) == 1 )
		flag = TRUE;

	while( loopdone == FALSE ) /*scan over any existing or statements */
	{
		cmnd     = com_list;
		com_list = mprog_next_command( com_list );
		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' )
		{
			bug ( "Mob: %d no commands after IF/OR", mob->pIndexData->vnum ); 
			return null;
		}
		morebuf = one_argument( cmnd, buf );
		if ( !str_cmp( buf, "or" ) )
		{
			if ( mprog_do_ifchck( morebuf,mob,actor,obj,vo,rndm,mob->rembch ) == 1 )
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
				com_list = mprog_process_if(morebuf,com_list,mob,actor,obj,vo,rndm);
				while ( *cmnd==' ' )
					cmnd++;
				if ( *com_list == '\0' )
					return null;
				cmnd     = com_list;
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
						com_list = extract_if_endif( com_list, mob );
					cmnd = com_list;
					com_list = mprog_next_command( com_list );
					while ( *cmnd == ' ' )
						cmnd++;
					if ( *cmnd == '\0' )
					{
						bug ( "Mob: %d missing endif after else", mob->pIndexData->vnum );
						return null;
					}
					morebuf = one_argument( cmnd,buf );
				}
				return com_list; 
			}
			mprog_process_cmnd( cmnd, mob, actor, obj, vo, rndm );
			cmnd = com_list;
			com_list = mprog_next_command( com_list );
			while ( *cmnd == ' ' )
				cmnd++;
			if ( *cmnd == '\0' )
			{
				bug ( "Mob: %d missing else or endif", mob->pIndexData->vnum ); 
				return null;
			}
			morebuf = one_argument( cmnd, buf );
		}
	else /*false ifcheck, find else and do existing commands or quit at endif*/
	{
		while ( str_cmp( buf, "else" ) && str_cmp( buf, "endif" ) )
		{
			if ( !str_cmp( buf, "if" ) )
				com_list = extract_if_endif( com_list, mob );
			cmnd   = com_list;
			com_list = mprog_next_command( com_list );
			while ( *cmnd == ' ' )
				cmnd++;
			if ( *cmnd == '\0' )
			{
				bug ( "Mob: %d missing an else or endif", mob->pIndexData->vnum ); 
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
			bug ( "Mob: %d missing endif", mob->pIndexData->vnum ); 
			return null;
		}
		morebuf = one_argument( cmnd, buf );

		for ( ; ; ) /*process the post-else commands until an endif is found.*/
		{
			if ( !str_cmp( buf, "if" ) )
			{ 
				com_list = mprog_process_if( morebuf, com_list, mob, actor, obj, vo, rndm );
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
				bug ( "Mob: %d found else in an else section", mob->pIndexData->vnum ); 
				return null;
			}
			if ( !str_cmp( buf, "break" ) )
				return null;
			if ( !str_cmp( buf, "endif" ) )
				return com_list; 
			mprog_process_cmnd( cmnd, mob, actor, obj, vo, rndm );
			cmnd   = com_list;
			com_list = mprog_next_command( com_list );
			while ( *cmnd == ' ' )
				cmnd++;
			if ( *cmnd == '\0' )
			{
				bug ( "Mob:%d missing endif in else section", mob->pIndexData->vnum ); 
				return null;
			}
			morebuf = one_argument( cmnd, buf );
		}
	}
}

/* to fixed the nested if-else bug -- Amenda 1/14/01 */
char * extract_if_endif( char *com_list, CHAR_DATA *mob )
{
	static char null[ 1 ];
	char buf1[ MAX_INPUT_LENGTH ] = "NULL";
	char  *cmnd = '\0';
	*null = '\0';

	while ( str_cmp( buf1, "endif" ) )
	{
		cmnd  = com_list;
		com_list = mprog_next_command( com_list );
		while ( *cmnd == ' ' )
			cmnd++;
		if ( *cmnd == '\0' )
		{
			bug ( "Mob: %d missing endif after else", mob->pIndexData->vnum );
			return null;
		}
		one_argument( cmnd, buf1 );
		if ( !str_cmp( buf1, "if" ) )
		{
			com_list = extract_if_endif( com_list, mob );
			while ( *com_list == ' ' )
				com_list++;
			if ( *com_list == '\0' )
			{
				bug ( "Mob: %d missing endif after else", mob->pIndexData->vnum );
				return null;
			}
			one_argument( com_list, buf1 );
		}
	}
		return com_list;
}

/* This routine handles the variables for command expansion.
 * If you want to add any go right ahead, it should be fairly
 * clear how it is done and they are quite easy to do, so you
 * can be as creative as you want. The only catch is to check
 * that your variables exist before you use them. At the moment,
 * using $t when the secondary target refers to an object 
 * i.e. >prog_act drops~<nl>if ispc($t)<nl>sigh<nl>endif<nl>~<nl>
 * probably makes the mud crash (vice versa as well) The cure
 * would be to change act() so that vo becomes vict & v_obj.
 * but this would require a lot of small changes all over the code.
 */
void mprog_translate( char ch, char *t, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo, CHAR_DATA *rndm )
{
	static char *he_she      [] = { "it",  "he",  "she" };
	static char *him_her     [] = { "it",  "him", "her" };
	static char *his_her     [] = { "its", "his", "her" };
	CHAR_DATA   *vict         = (CHAR_DATA *) vo;
	OBJ_DATA    *v_obj        = (OBJ_DATA  *) vo;
	CHAR_DATA   *bch;

	*t = '\0';
	switch ( ch )
	{
		case 'i':
			one_argument( mob->name, t );
			break;

		case 'I':
			strcpy( t, mob->short_descr );
			break;

		case 'n':
			if ( actor )
				if ( can_see( mob,actor ) )
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
				if ( can_see( mob, actor ) )
				{
					if ( IS_NPC( actor ) )
						strcpy( t, actor->short_descr );
					else
					{
						strcpy( t, actor->short_descr ); //change by Amethyst
						// strcpy( t, actor->name );
						// strcat( t, " - " );
						// strcat( t, actor->pcdata->title );
					}
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'b':
			if ( ( bch = mob->rembch ) ){
				if ( can_see( mob, bch ) )
				{
					one_argument( bch->name, t );
					if ( !IS_NPC( bch ) )
						*t = UPPER( *t );
					break;
				}
			}
			strcpy( t, "someone" );
			break;

		case 'B':
			if ( ( bch = mob->rembch) )
			{
				if ( can_see( mob, bch ) )
				{
					strcpy( t, bch->short_descr );
				}
				break;
			}
			strcpy( t, "someone" );
			break;

		case 't':
			if ( vict )
				if ( can_see( mob, vict ) )
				{
					one_argument( vict->name, t );
					if ( !IS_NPC( vict ) )
						*t = UPPER( *t );
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'T':
			if ( vict ) 
				if ( can_see( mob, vict ) )
				{
					if ( IS_NPC( vict ) )
						strcpy( t, vict->short_descr );
					else
					{
						strcpy( t, vict->name );
						strcat( t, " " );
						strcat( t, vict->pcdata->title );
					}
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'r':
			if ( rndm )
				// 加入 wizinv 判定 2022/11/20
				if ( can_see( mob, rndm ) && ( !IS_NPC(rndm) && !IS_SET( rndm->act, PLR_WIZINVIS ) ) )
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
				// 加入 wizinv 判定 2022/11/20
				if ( can_see( mob, rndm ) && ( !IS_NPC(rndm) && !IS_SET( rndm->act, PLR_WIZINVIS ) ) )
				{
					if ( IS_NPC( rndm ) )
						strcpy(t,rndm->short_descr);
					else
					{
						strcpy(t, rndm->short_descr); // change by Amethyst
						// strcpy( t, rndm->name );
						// strcat( t, " " );
						// strcat( t, rndm->pcdata->title );
					}
					break;
				}
			strcpy( t, "someone" );
			break;
		
		// 新增目標參數 mob 正在攻擊的目標 2022/07/16
		case 'v':
			if ( mob->fighting )
				if ( can_see( mob,mob->fighting ) )
				{
					one_argument( mob->fighting->name, t );
					if ( !IS_NPC( mob->fighting ) )
						*t = UPPER( *t );
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'V':
			if ( mob->fighting ) 
				if ( can_see( mob, mob->fighting ) )
				{
					strcpy( t, mob->fighting->short_descr );
					break;
				}
			strcpy( t, "someone" );
			break;

		case 'e':
			if ( actor )
				can_see( mob, actor ) ? strcpy( t, he_she[ actor->sex ] ) : strcpy( t, "someone" );
			break;

		case 'm':
			if ( actor )
				can_see( mob, actor ) ? strcpy( t, him_her[ actor->sex ] ) : strcpy( t, "someone" );
			break;

		case 's':
			if ( actor )
				can_see( mob, actor ) ? strcpy( t, his_her[ actor->sex ] ) : strcpy( t, "someone's" );
			break;

		case 'E':
			if ( vict )
				can_see( mob, vict ) ? strcpy( t, he_she[ vict->sex ] ) : strcpy( t, "someone" );
			break;

		case 'M':
			if ( vict )
			can_see( mob, vict ) ? strcpy( t, him_her[ vict->sex ] ) : strcpy( t, "someone" );
			break;

		case 'S':
			if ( vict )
				can_see( mob, vict ) ? strcpy( t, his_her[ vict->sex ] ) : strcpy( t, "someone's" ); 
			break;

		case 'j':
			strcpy( t, he_she[ mob->sex ] );
			break;

		case 'k':
			strcpy( t, him_her[ mob->sex ] );
			break;

		case 'l':
			strcpy( t, his_her[ mob->sex ] );
			break;

		case 'J':
			if ( rndm )
				can_see( mob, rndm ) ? strcpy( t, he_she[ rndm->sex ] ) : strcpy( t, "someone" );
			break;

		case 'K':
			if ( rndm )
				can_see( mob, rndm ) ? strcpy( t, him_her[ rndm->sex ] ) : strcpy( t, "someone" );
			break;

		case 'L':
			if ( rndm )
				can_see( mob, rndm ) ? strcpy( t, his_her[ rndm->sex ] ) : strcpy( t, "someone's" );
			break;

		case 'o':
			if ( obj )
				can_see_obj( mob, obj ) ? one_argument( obj->name, t ) : strcpy( t, "something" );
			break;

		case 'O':
			if ( obj )
				can_see_obj( mob, obj ) ? strcpy( t, obj->short_descr ) : strcpy( t, "something" );
			break;

		case 'p':
			if ( v_obj )
				can_see_obj( mob, v_obj ) ? one_argument( v_obj->name, t ) : strcpy( t, "something" );
			break;

		case 'P':
			if ( v_obj )
				can_see_obj( mob, v_obj ) ? strcpy( t, v_obj->short_descr ) : strcpy( t, "something" );
			break;

		case 'a':
			if ( obj ) 
				switch ( *( obj->name ) )
				{
					case 'a':
					case 'e':
					case 'i':
					case 'o':
					case 'u':
						strcpy( t, "an" );
						break;
			
					default: strcpy( t, "a" );
				}
			break;

		case 'A':
			if ( v_obj ) 
				switch ( *( v_obj->name ) )
				{
					case 'a':
					case 'e':
					case 'i':
					case 'o':
					case 'u':
						strcpy( t, "an" );
						break;
					default: strcpy( t, "a" );
				}
			break;

		case '$':
			strcpy( t, "$" );
			break;

		default:
			bug( "Mob: %d bad $var", mob->pIndexData->vnum );
			break;
	}

	return;

}

/* This procedure simply copies the cmnd to a buffer while expanding
 * any variables by calling the translate procedure.  The observant
 * code scrutinizer will notice that this is taken from act()
 */
void mprog_process_cmnd( char *cmnd, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo, CHAR_DATA *rndm )
{
	char buf[ MAX_INPUT_LENGTH ];
	char tmp[ MAX_INPUT_LENGTH ];
	char *str;
	char *i;
	char *point;

	point = buf;
	str   = cmnd;

	if ( mob->deleted )
	{
		// remove at 2020/05/31  減少 bug log
		//bug( "Mprog_process_cmnd: mob deleted!", 0);
		//bug( mob->name, 0);
		return;
	}

	while ( *str != '\0' )
	{
		if ( *str != '$' )
		{
			*point++ = *str++;
			continue;
		}
		str++;
		mprog_translate( *str, tmp, mob, actor, obj, vo, rndm );
		i = tmp;
		++str;
		while ( ( *point = *i ) != '\0' )
			++point, ++i;
	}
	*point = '\0';
	interpret( mob, buf );
	return;

}

/* The main focus of the MOBprograms.  This routine is called 
 *  whenever a trigger is successful.  It is responsible for parsing
 *  the command list and figuring out what to do. However, like all
 *  complex procedures, everything is farmed out to the other guys.
 */
void mprog_driver ( char *com_list, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo)
{

	char tmpcmndlst[ MAX_STRING_LENGTH*3 ];
	char buf [ MAX_INPUT_LENGTH ];
	char *morebuf;
	char *command_list;
	char *cmnd;
	CHAR_DATA *rndm  = NULL;
	CHAR_DATA *vch   = NULL;
	int      count = 0;

	if ( !IS_SET( mob->act, ACT_PET ) || IS_AFFECTED( mob, AFF_SUMMONED ) )
		if ( IS_AFFECTED( mob, AFF_CHARM ) )
			return;

	/* get a random visable mortal player who is in the room with the mob */
	if( !mob->in_room )
		return;

	for ( vch = mob->in_room->people; vch; vch = vch->next_in_room )
	{
		if ( !vch->in_room || vch->deleted )
			continue;
		if ( !IS_NPC( vch ) &&  can_see( mob, vch ) )
		{
			if ( number_range( 0, count ) == 0 )
				rndm = vch;
			count++;
		}
	}

	strcpy( tmpcmndlst, com_list );
	command_list = tmpcmndlst;
	cmnd       = command_list;
	command_list = mprog_next_command( command_list );
	while ( *cmnd != '\0' )
	{
		mob->wait = 0;
		morebuf = one_argument( cmnd, buf );
		if ( !str_cmp( buf, "if" ) )
			command_list = mprog_process_if( morebuf, command_list, mob, actor, obj, vo, rndm );
		else
			mprog_process_cmnd( cmnd, mob, actor, obj, vo, rndm );
		cmnd   = command_list;
		command_list = mprog_next_command( command_list );
	}

	return;
}


/***************************************************************************
 * Global function code and brief comments.
 */

/* The next two routines are the basic trigger types. Either trigger
 *  on a certain percent, or trigger on a keyword or word phrase.
 *  To see how this works, look at the various trigger routines..
 */
void mprog_wordlist_check ( char *arg, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo, int type )
{

	char temp1[ MAX_STRING_LENGTH ];
	char temp2[ MAX_INPUT_LENGTH ];
	char word[ MAX_INPUT_LENGTH ];
	MPROG_DATA *mprg;
	char *list;
	char *start;
	char *dupl;
	char *end;
	int i;
	bool fChinese;

	for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next ){
		if ( mprg->type & type )
		{
			strcpy( temp1, mprg->arglist );
			list = temp1;
			fChinese=FALSE;
			for ( i = 0; i < strlen( list ); i++ ) {
				if ( fChinese )
					fChinese=FALSE;
				else{
					list[i] = LOWER( list[i] );
					fChinese = is_chinese_char( list[i], fChinese);
					//if ( list[i]>='\x80' && list[i]<='\xff' )
					//  fChinese=TRUE;
				}
			}
			strcpy( temp2, arg );
			dupl = temp2;
			fChinese=FALSE;
			for ( i = 0; i < strlen( dupl ); i++ ) {
				if ( fChinese )
					fChinese=FALSE;
				else {
					dupl[i] = LOWER( dupl[i] );
					fChinese = is_chinese_char( dupl[i], fChinese);
					//if ( dupl[i]>='\x80' && list[i]<='\xff' )
					//  fChinese=TRUE;
				}
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
						mprog_driver( mprg->comlist, mob, actor, obj, vo );
						break;
					}
					else
						dupl = start+1;
			}
			else
			{
				list = one_argument( list, word );
				for( ; word[0] != '\0'; list = one_argument( list, word ) )
					while ( ( start = strstr( dupl, word ) ) )
					if ( ( start == dupl || *(start-1) == ' ' )
						&& ( *(end = start + strlen( word ) ) == ' '
						|| *end == '\n'
						|| *end == '\r'
						|| *end == '\0' /*NULL*/ ) )
					{
						mprog_driver( mprg->comlist, mob, actor, obj, vo );
						break;
					}
					else
						dupl = start+1;
			}
		}
	}

	return;

}

void mprog_percent_check( CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *obj, void *vo, int type)
{
	MPROG_DATA * mprg;
	// int tmp_number_percent = number_percent();
	CHAR_DATA *to;
   
	if(!mob->deleted)
		to = mob->in_room->people;
   
	for( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
	{
		// 機率放入中間能使每個 fight_prog 變為獨立事件 2019/08/06
		int tmp_number_percent = number_percent();
		if( ( mprg->type & type )
			&& ( (atoi(mprg->arglist) >= 100)
				|| (tmp_number_percent < atoi( mprg->arglist )) ) )
		{
			mprog_driver( mprg->comlist, mob, actor, obj, vo );
			// if ( type != GREET_PROG && type != ALL_GREET_PROG )
			// break;
		}
	}
	return;
}

bool mprog_act_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob )
{
    bool returnValue = FALSE;
    MPROG_DATA *mprg;
    char arg[MAX_STRING_LENGTH];

    if ( !IS_NPC( mob ) || !(mob->pIndexData->progtypes & ACT_PROG ) )
    return returnValue;

 /*  避免被測 mob prog , 移到act_social 內判斷 , 
     讓不管有無 act_prog 的 mob 在看不見的情況都不作social反應
    if ( !can_see( mob, ch ) )
    {
      send_to_char( "他看不到你！\n\r", ch );
      return;
    }
 */
 
    if( txt != NULL )
    {
        for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
            {
                one_argument(mprg->arglist, arg);
                if( arg[0] != '\0' && !str_cmp(arg, txt)){
                    returnValue = TRUE;
                    mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
                }
            }
    }


    return returnValue;
}

void mprog_bribe_trigger( CHAR_DATA *mob, CHAR_DATA *ch, int amount )
{

  ///char       buf[ MAX_STRING_LENGTH ];
  MPROG_DATA *mprg;
  //OBJ_DATA   *obj;

  if( !can_see(mob, ch)){
      send_to_char("他看不到你。\n\r", ch);
      return;
  }
  if ( IS_NPC( mob )
      && ( mob->pIndexData->progtypes & BRIBE_PROG ) )
    {
    //  obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ), 0 );
     // sprintf( buf, obj->short_descr, amount );
     // free_string( obj->short_descr );
     // obj->short_descr = str_dup( buf );
     // obj->value[0]    = amount;
     // obj_to_char( obj, mob );
     // mob->gold -= amount;

      for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
  if ( ( mprg->type & BRIBE_PROG )
      && ( amount >= atoi( mprg->arglist ) ) )
    {
      mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
      break;
    }
    }
  
  return;

}

void mprog_death_trigger( CHAR_DATA *actor, CHAR_DATA *mob )
{
	AFFECT_DATA *paf;
	if( !IS_NPC(mob)) return;
	if(mob->deleted) return;
	if ( ( mob->pIndexData->progtypes & DEATH_PROG ) )
	{
		if ( mob->affected )
			for ( paf = mob->affected; paf; paf = paf->next )
				affect_strip( mob, paf->type );
		spell_detect_invis( skill_lookup("detect inv"), mob->level, mob, mob);
		spell_detect_hidden( skill_lookup("detect hid"), mob->level, mob, mob);
		spell_infravision( skill_lookup("infravision"), mob->level, mob, mob);
		// 跑入 death_prog 就把 hit 轉換成 -21 , 並加入 aff_no_heal 避免後面判定造成 hp 變為正值 2022/12/30
		SET_BIT( mob->affected_by, AFF_NO_HEAL );
		mob->hit = -21;
		mprog_percent_check( mob, actor, NULL, NULL, DEATH_PROG );
	}
	if ( !mob->deleted )//may mppurge self
		death_cry( mob );
	return;
}

void mprog_entry_trigger( CHAR_DATA *mob )
{
	
	if ( IS_NPC( mob ) && ( mob->pIndexData->progtypes & ENTRY_PROG ) )
		mprog_percent_check( mob, NULL, NULL, NULL, ENTRY_PROG );

	return;

}

// 新增 entry_greet_prog 判定進入房間內是否存在生物 2021/12/18
void mprog_entry_greet_trigger( CHAR_DATA *mob )
{

	CHAR_DATA *vch;
	CHAR_DATA *vch_next_in_room;

	if ( IS_NPC( mob ) && ( mob->pIndexData->progtypes & ENTRY_GREET_PROG ) )
	{
		for ( vch = mob->in_room->people; vch != NULL; vch = vch_next_in_room )
		{
			vch_next_in_room = vch->next_in_room;
			if( !mob || mob->deleted )
				return;
			if( !mob->in_room )
				return;
			if( !vch || vch->deleted )
				continue;
			if( !vch->in_room )
				continue;
			if( mob == vch )
				continue;
			if( !can_see( mob , vch ) )
				continue;
			if( mob->fighting )
				return;
			mprog_percent_check( mob, vch, NULL, NULL, ENTRY_GREET_PROG );
		}
	}
	return;
}

void mprog_fight_trigger( CHAR_DATA *mob, CHAR_DATA *ch )
{
	if ( !mob || !ch ) return;

	// 如果 mob 或目標消失則不跑 fight_prog 2022/07/14
	if ( mob->deleted ) return;
	if ( ch->deleted ) return;

	if ( IS_NPC( mob ) && ( mob->pIndexData->progtypes & FIGHT_PROG ) )
	{
		// 阻擋死前一擊跑 fight_prog 2022/12/29
		if( mob->hit <= 0 )
			return;
		if( mob->position == POS_DEAD )
			return;

		mprog_percent_check( mob, ch, NULL, NULL, FIGHT_PROG );
	}

	return;
}

void mprog_give_trigger( CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj )
{

	char buf[MAX_INPUT_LENGTH];
	MPROG_DATA *mprg;

	if( !can_see(mob, ch)){
		send_to_char("他看不到你。\n\r", ch);
		return;
	}
	if ( IS_NPC( mob ) && ( mob->pIndexData->progtypes & GIVE_PROG ) )
	for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
	{
		one_argument( mprg->arglist, buf );
		if ( ( mprg->type & GIVE_PROG )
			&& ( ( !str_cmp( obj->name, mprg->arglist ) )
			|| ( obj->pIndexData->vnum == atoi( mprg->arglist ) )
			|| ( !str_cmp( "all", buf ) ) ) )
		{
			mprog_driver( mprg->comlist, mob, ch, obj, NULL );
			break;
		}
	}

	return;

}

// modified by coconet 0520 for let ALL_GREET_PROG can get when NIGHTSTALK
void old_mprog_greet_trigger( CHAR_DATA *ch )
{

	CHAR_DATA *vmob;
	CHAR_DATA *vmob_next_in_room;

	for ( vmob = ch->in_room->people; vmob != NULL; vmob = vmob_next_in_room )
	{
		vmob_next_in_room = vmob->next_in_room;
		if ( !vmob->in_room || vmob->deleted )
			continue;
		if ( IS_NPC( vmob )
			&& ch != vmob
			&& can_see( vmob, ch )
			// 新增 mob 有 enable_fight_greet 的 qmark 時 能在戰鬥中判定 greet_prog 2021/12/19
			//&& ( vmob->fighting == NULL )
			&& ( vmob->fighting == NULL || (is_qmark(vmob,"enable_fight_greet" )) )
			&& IS_AWAKE( vmob )
			&& ( vmob->pIndexData->progtypes & GREET_PROG) 
			&& !IS_AFFECTED(ch, AFF_NIGHTSTALK) )
			mprog_percent_check( vmob, ch, NULL, NULL, GREET_PROG );
		else
			if ( IS_NPC( vmob )
				// 新增 mob 有 enable_fight_greet 的 qmark 時 能在戰鬥中判定 all_greet_prog 2021/12/19
				//&& ( vmob->fighting == NULL )
				&& ( vmob->fighting == NULL || (is_qmark(vmob,"enable_fight_greet" )) )
				&& IS_AWAKE( vmob )
				&& ( vmob->pIndexData->progtypes & ALL_GREET_PROG ) )
				mprog_percent_check(vmob,ch,NULL,NULL,ALL_GREET_PROG);
	}

	return;

}


void mprog_greet_trigger( CHAR_DATA *mob )
{

	static int depth = 0;
	if(depth++ < 3 )
		old_mprog_greet_trigger(mob);
	depth--;
	return;

}



void mprog_kill_trigger( CHAR_DATA *mob ,CHAR_DATA *ch)
{

	if ( mob->pIndexData->progtypes & KILL_PROG)
		mprog_percent_check(mob,ch,NULL,NULL,KILL_PROG);

	return;

}

void mprog_hurt_trigger( CHAR_DATA *mob, CHAR_DATA *ch, int dt)
{

	MPROG_DATA *mprg;

	if ( IS_NPC( mob ) && ( mob->pIndexData->progtypes & HURT_PROG ) )
	{
		for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
		{
				/* 改寫 hurt_prog 追加 hurt_prog all 的效果 2022/05/03
					if ( ( mprg->type & HURT_PROG ) && ( dt == atoi( mprg->arglist ) ) )
					{
						mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
				*/
			if( mprg->type & HURT_PROG )
			{
				// 特定 元素傷害 判定 , 不希望此項作用可加入此 qmark 2022/12/25
				if( !(is_qmark(mob, "disable_hurt_element"))
					&& (   dt == 314     // gsn_magic_enchantment
						|| dt == 315     // gsn_wind_enchantment
						|| dt == 316     // gsn_flame_enchantment
						|| dt == 317     // gsn_water_enchantment
						|| dt == 318     // gsn_earth_enchantment
						|| dt == 319     // gsn_lightning_enchantment
						|| dt == 320     // gsn_saint_enchantment
						|| dt == 321 ) ) // gsn_dark_enchantment
				{
					// 每種類可另外加入 disable_hurt 的 qmark 阻止作用
					if( !str_cmp( mprg->arglist ,"spell") && dt == 314 ){
						if( !(is_qmark(mob, "disable_hurt_spell")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"wind") && dt == 315 ){
						if( !(is_qmark(mob, "disable_hurt_wind")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"fire") && dt == 316 ){
						if( !(is_qmark(mob, "disable_hurt_fire")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"water") && dt == 317 ){
						if( !(is_qmark(mob, "disable_hurt_water")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"earth") && dt == 318 ){
						if( !(is_qmark(mob, "disable_hurt_earth")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"lightning") && dt == 319 ){
						if( !(is_qmark(mob, "disable_hurt_lightning")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"saint") && dt == 320 ){
						if( !(is_qmark(mob, "disable_hurt_saint")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
					if( !str_cmp( mprg->arglist ,"dark") && dt == 321 ){
						if( !(is_qmark(mob, "disable_hurt_dark")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
				}else{
					if( dt == atoi( mprg->arglist ) )
					{
						// 特定 dt 值判定(原始判定) , 不希望此項作用則加入此 qmark 2022/12/25
						if( !(is_qmark(mob, "disable_hurt_dt")) )
							mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
					}
				}

				// 所有傷害 判定 , 不希望此項作用則加入此 qmark
				if( !(is_qmark(mob, "disable_hurt_all")) )
				{
					// 元素傷害不會跑到 所有傷害 的判定 2022/12/25
					if (   dt == 314
						|| dt == 315
						|| dt == 316
						|| dt == 317
						|| dt == 318
						|| dt == 319
						|| dt == 320
						|| dt == 321 )
					{
						return;
					}
					if( !str_cmp( mprg->arglist ,"all") )
						mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
				}
			}
		}
	}
	return;
}


void mprog_hitprcnt_trigger( CHAR_DATA *mob, CHAR_DATA *ch)
{

	MPROG_DATA *mprg;

	if ( IS_NPC( mob ) && ( mob->pIndexData->progtypes & HITPRCNT_PROG ) )
	for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
		if ( ( mprg->type & HITPRCNT_PROG ) && ( ( 100*mob->hit / mob->max_hit ) < atoi( mprg->arglist ) ) )
		{
			mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
			// break;
		}
	return;

}

void mprog_random_trigger( CHAR_DATA *mob )
{
	//log_string("[RAND_PROG] CHECK.\n");
	if( mob->pIndexData->progtypes & RAND_PROG)
		mprog_percent_check(mob,NULL,NULL,NULL,RAND_PROG);
	return;

}


void mprog_repop_trigger( CHAR_DATA *mob )
{

	if ( mob->pIndexData->progtypes & REPOP_PROG)
		mprog_percent_check(mob,NULL,NULL,NULL,REPOP_PROG);

	return;

}

void mprog_tick_trigger( CHAR_DATA *mob )
{

	if ( mob->pIndexData->progtypes & TICK_PROG)
		mprog_percent_check(mob,NULL,NULL,NULL,TICK_PROG);
	return;

}

void old_mprog_speech_trigger( char *txt, CHAR_DATA *mob )
{

  CHAR_DATA *vmob;

	for ( vmob = mob->in_room->people; vmob != NULL; vmob = vmob->next_in_room )
		if ( IS_NPC( vmob ) && ( vmob->pIndexData->progtypes & SPEECH_PROG ) )
			mprog_wordlist_check( txt, vmob, mob, NULL, NULL, SPEECH_PROG );

	return;

}


void check_arrival(CHAR_DATA *ch, bool f)
{
	if ( (f || !IS_AFFECTED(ch, AFF_SNEAK)  )//|| !IS_AFFECTED(ch, AFF_NIGHTSTALK))
		// wizinv 必須為 pc 才成立 2022/10/11
		//&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) )
		&& ( IS_NPC(ch) || (!IS_NPC(ch) && !IS_SET(ch->act, PLR_WIZINVIS) ) ) )
	{
		mprog_entry_trigger(ch);
		mprog_greet_trigger(ch);
		mprog_entry_greet_trigger(ch); // add at 2021/12/18
	}
}


void mprog_speech_trigger( char *txt, CHAR_DATA *mob )
{

	static int depth = 0;
	if(depth++ < 3 )
		old_mprog_speech_trigger(txt, mob);
	depth--;
	return;

}



void mprog_defun_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob )
{
	MPROG_DATA *mprg;
	char arg[MAX_INPUT_LENGTH];
	bool fFound = FALSE;

	for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
	{
		one_argument(mprg->arglist, arg);
		if( arg[0] != '\0' && !str_cmp(arg,txt) ){
			mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
			return;
		}
	}

	if( !fFound ) {
		bugf("no such function %s in %d", txt, mob->pIndexData->vnum);
	}

	return;
}

void mprog_talk_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob )
{
    MPROG_DATA *mprg;
    char arg[MAX_STRING_LENGTH];

    if( txt != NULL && !str_cmp(txt,"topic"))
    {
        for ( mprg = mob->pIndexData->mobprogs; mprg != NULL; mprg = mprg->next )
        {
            one_argument(mprg->arglist, arg);
            if( arg[0] != '\0' && !str_cmp(arg,"topic") )
                mprog_driver( mprg->comlist, mob, ch, NULL, NULL );
        }
        return;
    }

    if ( !can_see( mob, ch ) )
    {
      send_to_char( "他看不到你！\n\r", ch );
      return;
    }

    mprog_wordlist_check( txt, mob, ch, NULL, NULL, TALK_PROG );
    return;
}

