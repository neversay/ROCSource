/* $Id: mprog_load.c 916 2005-05-10 03:56:16Z zcecil $ */
#include <stdio.h>
#include "merc.h"

int 
mprog_name_to_type ( char *name )
{
   if ( !str_cmp( name, "in_file_prog"	 ) )	return IN_FILE_PROG;
   if ( !str_cmp( name, "act_prog"	 ) )	return ACT_PROG;
   if ( !str_cmp( name, "speech_prog"	 ) )	return SPEECH_PROG;
   if ( !str_cmp( name, "rand_prog"	 ) )	return RAND_PROG;
   if ( !str_cmp( name, "fight_prog"	 ) )	return FIGHT_PROG;
   if ( !str_cmp( name, "hitprcnt_prog"	 ) )	return HITPRCNT_PROG;
   if ( !str_cmp( name, "death_prog"	 ) )	return DEATH_PROG;
   if ( !str_cmp( name, "entry_prog"	 ) )	return ENTRY_PROG;
   if ( !str_cmp( name, "greet_prog"	 ) )	return GREET_PROG;
   if ( !str_cmp( name, "all_greet_prog" ) )	return ALL_GREET_PROG;
   if ( !str_cmp( name, "give_prog"	 ) )	return GIVE_PROG;
   if ( !str_cmp( name, "bribe_prog"	 ) )	return BRIBE_PROG;
   if ( !str_cmp( name, "talk_prog"	 ) )	return TALK_PROG;
   if ( !str_cmp( name, "tick_prog"	 ) )	return TICK_PROG;
   if ( !str_cmp( name, "repop_prog"	 ) )	return REPOP_PROG;
   if ( !str_cmp( name, "defun_prog"	 ) )	return DEFUN_PROG;
   if ( !str_cmp( name, "hurt_prog"	 ) )	return HURT_PROG;
   if ( !str_cmp( name, "kill_prog"	 ) )	return KILL_PROG;
   if ( !str_cmp( name, "entry_greet_prog"	 ) )	return ENTRY_GREET_PROG; // add at 2021/12/18

   return( ERROR_PROG );
}

/* This routine reads in scripts of MOBprograms from a file */

MPROG_DATA* mprog_file_read( char *f, MPROG_DATA *mprg, MOB_INDEX_DATA *pMobIndex )
{

	char MOBProgfile[ MAX_INPUT_LENGTH ];
	MPROG_DATA *mprg2;
	FILE *progfile;
	char letter;
	bool done = FALSE;
	int stat;

	sprintf( MOBProgfile, "%s%s", MOB_DIR, f );

	progfile = fopen( MOBProgfile, "r" );
	if ( !progfile )
	{
		bug( "Mob: %d couldnt open mobprog file", pMobIndex->vnum );
		exit( 1 );
	}

	mprg2 = mprg;
	switch ( letter = fread_letter( progfile ) )
	{
		case '>':
			break;
		case '|':
			bug( "empty mobprog file.", 0 );
			exit( 1 );
			break;
		default:
			bug( "in mobprog file syntax error.", 0 );
			exit( 1 );
			break;
	}

	while ( !done )
	{
		mprg2->type = mprog_name_to_type( fread_word( progfile, &stat ) );
		switch ( mprg2->type )
		{
			case ERROR_PROG:
				bug( "mobprog file type error", 0 );
				exit( 1 );
				break;
			case IN_FILE_PROG:
				bug( "mprog file contains a call to file.", 0 );
				exit( 1 );
				break;
			default:
				pMobIndex->progtypes = pMobIndex->progtypes | mprg2->type;
				mprg2->arglist = fread_string( progfile, &stat );
				mprg2->comlist = fread_string( progfile, &stat );
				switch ( letter = fread_letter( progfile ) )
				{
					case '>':
						mprg2->next = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
						mprg2 = mprg2->next;
						mprg2->next = NULL;
						break;
					case '|':
						done = TRUE;
						break;
					default:
						bug( "in mobprog file syntax error.", 0 );
						exit( 1 );
						break;
				}
				break;
		}
	}
	fclose( progfile );
	return mprg2;
}

/* Snarf a MOBprogram section from the area file.
 */
void load_mobprogs( FILE *fp )
{
  MOB_INDEX_DATA *iMob;
  MPROG_DATA	 *original;
  MPROG_DATA	 *working;
  char		  letter;
  int		  value;
  int		  stat;

  for ( ; ; )
    switch ( letter = fread_letter( fp ) )
    {
    default:
      bug( "Load_mobprogs: bad command '%c'.",letter);
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
      value = fread_number( fp, &stat );
      if ( ( iMob = get_mob_index( value ) ) == NULL )
      {
	bug( "Load_mobprogs: vnum %d doesnt exist", value );
	exit( 1 );
      }
    
      /* Go to the end of the prog command list if other commands
	 exist */

      if ( ( original = iMob->mobprogs ) )
	for ( ; original->next != NULL; original = original->next );

      working = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
      if ( original )
	original->next = working;
      else
	iMob->mobprogs = working;
      working	    = mprog_file_read( fread_word( fp, &stat ), working, iMob );
      working->next = NULL;
      fread_to_eol( fp );
      break;
    }

  return;

} 

/* This procedure is responsible for reading any in_file MOBprograms.
 */

void mprog_read_programs( FILE *fp, MOB_INDEX_DATA *pMobIndex)
{
  MPROG_DATA *mprg;
  char	      letter;
  bool	      done = FALSE;
  int	      stat;
  char	*tempstr;

  if ( ( letter = fread_letter( fp ) ) != '>' )
  {
      bug( "Load_mobiles: vnum %d MOBPROG char", pMobIndex->vnum );
      exit( 1 );
  }
  pMobIndex->mobprogs = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
  mprg = pMobIndex->mobprogs;

  while ( !done )
  {
    mprg->type = mprog_name_to_type( fread_word( fp, &stat ) );
    switch ( mprg->type )
    {
     case ERROR_PROG:
	bug( "Load_mobiles: vnum %d MOBPROG type.", pMobIndex->vnum );
	exit( 1 );
      break;
     case IN_FILE_PROG:
	tempstr=fread_string( fp, &stat );
	mprg = mprog_file_read( tempstr, mprg,pMobIndex );
	free_string( tempstr );
	fread_to_eol( fp );
	switch ( letter = fread_letter( fp ) )
	{
	  case '>':
	     mprg->next = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
	     mprg	= mprg->next;
	     mprg->next = NULL;
	   break;
	  case '|':
	     mprg->next = NULL;
	     fread_to_eol( fp );
	     done = TRUE;
	   break;
	  default:
	     bug( "Load_mobiles: vnum %d bad MOBPROG.", pMobIndex->vnum );
	     exit( 1 );
	   break;
	}
      break;
     default:
	pMobIndex->progtypes = pMobIndex->progtypes | mprg->type;
	mprg->arglist	     = fread_string( fp, &stat );
	fread_to_eol( fp );
	mprg->comlist	     = fread_string( fp, &stat );
	fread_to_eol( fp );
	switch ( letter = fread_letter( fp ) )
	{
	  case '>':
	     mprg->next = (MPROG_DATA *)alloc_mem( sizeof( MPROG_DATA ) );
	     mprg	= mprg->next;
	     mprg->next = NULL;
	   break;
	  case '|':
	     mprg->next = NULL;
	     fread_to_eol( fp );
	     done = TRUE;
	   break;
	  default:
	     bug( "Load_mobiles: vnum %d bad MOBPROG.", pMobIndex->vnum );
	     exit( 1 );
	   break;
	}
      break;
    }
  }

  return;

}

void action_read_programs( FILE *fp, MOB_INDEX_DATA *pMobIndex )
{
  MOB_ACTION *mob_action;
  char	      letter;
  bool	      done = FALSE;
  int	      stat;

  if ( ( letter = fread_letter( fp ) ) != 'A' )
  {
      bug( "Load_mobiles: vnum %d MOB_ACTION char", pMobIndex->vnum );
      exit( 1 );
  }
  pMobIndex->mob_action = (MOB_ACTION *)alloc_mem( sizeof( MOB_ACTION ) );
  mob_action = pMobIndex->mob_action;

  while ( !done )
  {
	mob_action->start_time = fread_number( fp, &stat );
	fread_to_eol( fp );
	mob_action->com_list	     = fread_string( fp, &stat );
	fread_to_eol( fp );
	switch ( letter = fread_letter( fp ) )
	{
	  case 'A':
	     mob_action->next = (MOB_ACTION *)alloc_mem( sizeof( MOB_ACTION ) );
	     mob_action	= mob_action->next;
	   break;
	  case '|':
	     mob_action->next = NULL;
	     fread_to_eol( fp );
	     done = TRUE;
	   break;
	  default:
	     bug( "Load_mobiles: vnum %d bad MOB_ACTION.", pMobIndex->vnum );
	     exit( 1 );
	   break;
	}
  }

  return;

}

