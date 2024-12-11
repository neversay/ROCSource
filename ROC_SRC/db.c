 /***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael    *
 *  Chastain, Michael Quan, and Mitchell Tse.          *
 *                     *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.         *
 *                     *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and    *
 *  Mitchell Tse.                *
 *                     *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.     *
 *                     *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in    *
 *  'license.txt', as well as the Envy license in 'license.nvy'.     *
 *  In particular, you may not remove either of these copyright notices.   *
 *                     *
 *  Much time and thought has gone into this software and you are    *
 *  benefitting.  We hope that you share your changes too.  What goes    *
 *  around, comes around.              *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : db.c                                           *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 jye                                    *
//*  PURPOSE     :         ­×§ï±Nstrcpy§ï¥Îstrncpy                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940712 jye                                    *
//*  PURPOSE     :         ­×§ï±Nload_down_time ®É¶¡©T©w¹j¤Ñ20:00 *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940717 jye                                    *
//*  PURPOSE     :         ­×§ïget_mob_index Crash Bug            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940720 Razgriz                                *
//*  PURPOSE     :         ­×§ï load_mobiles() ¼W¥[ fAutoset ªº¹B *
//*              :         ºâ¦¡¡A­×§ï create_mobile() ¥H¤½¦¡°t¦X  *
//*              :         fAutoset ªº§PÂ_¡AÅý lv > 35 ªº mob     *
//*              :         °²¦p¥¼¦b°Ï°ìÀÉ¤¤¥H S ³]©w¨ä pp pd pdr  *
//*              :         md mdr ªº¸Ü¡A±N¥Ñ¨t²Î¨Ó¦Û°Ê¨M©w.       *
//*              :         (¼È®É¨ú®ø¦¹¥\¯à)                       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         ­×§ï new_object() ¼W¥[ extra_value ªì­È*
//*****************************************************************

#define ENABLE_BUG_MESSAGE 1

#if defined( macintosh )
#include <types.h>
#else
#if defined( unix )
#include <unistd.h>
#endif
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "merc.h"
#include "ansi.h"
#include "olc.h"
#include "gsn.h"

#if !defined( macintosh )
extern  int _filbuf   args( (FILE *) );
#endif

#if !defined( ultrix ) && !defined( apollo )
#include <memory.h>
#endif



/*
 * Globals.
 */
HELP_DATA *   help_first = NULL;
HELP_DATA *   help_last = NULL;
DO_FUN*     yn_dofun[2] = { NULL, NULL};
SHOP_DATA *   shop_first = NULL;
SHOP_DATA *   shop_last = NULL;

FILE * fpBug = NULL;
OBJ_HP *    obj_hp_free = NULL;
BANK_DATA   * eqbank_list = NULL, *eqbank_free = NULL;
GROUP_DATA  * group_list = NULL, *group_free=NULL;
CHAR_DATA *   char_free = NULL;
EXTRA_DESCR_DATA *  extra_descr_free = NULL;
AFFECT_DATA *   affect_free = NULL;
#ifdef HAS_ROOM_AFFECT
R_AFFECT_DATA *   r_affect_free = NULL;
R_AFFECT_DATA *   r_affect_list = NULL;
#endif
NOTE_DATA *   note_free = NULL;
OBJ_DATA *    obj_free = NULL;
PC_DATA *   pcdata_free = NULL;
ASKILL_DATA *   askill_list = NULL;
CALL_OUT_DATA *   call_out_list =NULL;
CALL_OUT_DATA *   call_out_free =NULL;
RCALL_OUT_DATA *    rcall_out_list =NULL;
RCALL_OUT_DATA *    rcall_out_free =NULL;

char      bug_buf     [ MAX_INPUT_LENGTH*2 ];
CHAR_DATA *   char_list = NULL;
char *      help_greeting   [ MAX_GREETING ];
int     num_greeting;
char      log_buf     [ MAX_INPUT_LENGTH*2 ];
char      pipe_buf    [ MAX_STRING_LENGTH  ];
char      read_pipe_buf   [ MAX_STRING_LENGTH  ];
char      name_buf    [ MAX_STRING_LENGTH  ];
char *      read_pipe_ptr   = read_pipe_buf;
KILL_DATA   kill_table    [ MAX_LEVEL      ];
NOTE_DATA *   note_list = NULL;
OBJ_DATA *    object_list = NULL;
TIME_INFO_DATA    time_info;
WEATHER_DATA    weather_info;
OBJ_INDEX_DATA      *   basic_types[MAX_TYPES];
extern time_t     down_time;
time_t      warning1;
time_t      warning0;
time_t      warning2;
bool      Reboot = TRUE;
int   mpact[24];

/*
 * Locals.
 */
MOB_INDEX_DATA *  mob_index_hash    [ MAX_KEY_HASH       ];
OBJ_INDEX_DATA *  obj_index_hash    [ MAX_KEY_HASH       ];
ROOM_INDEX_DATA * room_index_hash   [ MAX_KEY_HASH       ];
SYMBOL_DATA * symbol_hash [26];

AREA_DATA *   area_first = NULL;
AREA_DATA *   area_last = NULL;

extern char   str_empty   [ 1        ];

int     top_affect;
int     top_errmem;
int     top_area;
int     top_ed;
int     top_exit;
int     top_help;
int     top_desc;
int     top_ch;
int     top_obj;
int     top_mob_index;
int     top_obj_index;
int     top_reset;
int     top_room;
int     top_shop;
int     top_callout;
int     top_act;//act_prog
int     top_action;
int     top_qmark;
int     top_hiddenpower;
int     top_bank;
int     top_vnum_room;  /* OLC */
int     top_vnum_mob; /* OLC */
int     top_vnum_obj; /* OLC */
int     fd0[2];
int     fd1[2];

/*
 * Memory management.
 * Increase MAX_STRING from 1500000 if you have too.
 * Tune the others only if you understand what you're doing.
 * MAX_STRING is now in ssm.c
 */
extern int    MAX_STRING;


extern int    nAllocString;
extern int    sAllocString;
extern int    nOverFlowString;
extern int    sOverFlowString;
extern bool   Full;
extern int      nAllocPerm;
extern int      sAllocPerm;



/*
 * Semi-locals.
 */
bool      fBootDb;
FILE *      fpArea;
char      strArea     [ MAX_INPUT_LENGTH   ];

void  init_string_space( void );
void  boot_done( );
char *  daPrompt;

int DIW_AREA;

/*
 * Local booting procedures.
 */
void  init_mm   args( ( void ) );

void  load_area args( ( FILE *fp ) );
void  load_basic_types ();
void load_material( FILE *fp );
void  load_symbol args( (void ));
void  load_helps  args( ( FILE *fp ) );
void  load_recall args( ( FILE *fp ) );
void  load_mobiles  args( ( FILE *fp ) );
void  load_objects  args( ( FILE *fp ) );
void  load_resets args( ( FILE *fp ) );
void  load_rooms  args( ( FILE *fp ) );
void  load_shops  args( ( FILE *fp ) );
void  load_specials args( ( FILE *fp ) );
void  load_notes  args( ( void ) );
void  load_ban  args( ( void ) );
void  load_down_time  args( ( void ) );
void  fix_exits args( ( void ) );

void  load_diw_area args( ( FILE *fp ) );
void  load_diw_objects  args( ( FILE *fp ) );

/* void reset_area  args( ( AREA_DATA * pArea ) ); */

int     interpolate1  args ( ( int level1, int value1, int level2, int value2, int level ) );

/*
 * MOBprogram locals
 */

int   mprog_name_to_type  args ( ( char* name ) );
MPROG_DATA *  mprog_file_read args ( ( char* f, MPROG_DATA* mprg, 
            MOB_INDEX_DATA *pMobIndex ) );
void    load_mobprogs   args ( ( FILE* fp ) );
void    mprog_read_programs args ( ( FILE* fp,
            MOB_INDEX_DATA *pMobIndex ) );
void    action_read_programs  args ( ( FILE* fp,
            MOB_INDEX_DATA *pMobIndex ) );
/*
 * ROOMprogram locals
 */

int   rprog_name_to_type  args ( ( char* name ) );
int   oprog_name_to_type  args ( ( char* name ) );
RPROG_DATA *  rprog_file_read args ( ( char* f, RPROG_DATA* rprg, 
            ROOM_INDEX_DATA *pRoomIndex ) );
void    load_roomprogs    args ( ( FILE* fp ) );
void    rprog_read_programs args ( ( FILE* fp,
            ROOM_INDEX_DATA *pRoomIndex ) );
//void    load_objprogs   args ( ( FILE* fp ) );
void    oprog_read_programs args ( ( FILE* fp,
            OBJ_INDEX_DATA *pObjIndex) );
/*
 * Non-Envy Loading procedures.
 * Put any new loading function in this section.
 */
void  new_load_area args( ( FILE *fp ) ); /* OLC */
void  new_load_rooms  args( ( FILE *fp ) ); /* OLC 1.1b */

OBJ_INDEX_DATA * material_list;

/*
 * Big mama top level function.
 */
void boot_db( void )
{
  /*
   * Init some data space stuff.
   */
  {
    init_string_space( );
    fBootDb   = TRUE;
  }

  /*
   * Init random number generator.
   */
  {
    init_mm( );
  }

  load_symbol();
  /* init hashes to null */
  {
    int i;
    for( i=0; i< MAX_KEY_HASH; ++i ) {
      mob_index_hash[i] = NULL;
      obj_index_hash[i] = NULL;
      room_index_hash[i] = NULL;
    }
  }
  /*
   * Set time and weather.
   */
  {
    int i;
    long lhour, lday, lmonth;
    for( i=0 ; i < 24 ; ++i )
      mpact[i] = 0;

    lhour   = ( current_time - 650336715 )
           / ( PULSE_TICK / PULSE_PER_SECOND );
    time_info.hour  = lhour  % 24;
    lday    = lhour  / 24;
    time_info.day = lday   % 35;
    lmonth    = lday   / 35;
    time_info.month = lmonth % 17;
    time_info.year  = lmonth / 17;

    if      ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;
    else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;
    else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;
    else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;
    else        weather_info.sunlight = SUN_DARK;

    weather_info.change = 0;
    weather_info.mmhg = 960;
    if ( time_info.month >= 7 && time_info.month <=12 )
      weather_info.mmhg += number_range( 1, 50 );
    else
      weather_info.mmhg += number_range( 1, 80 );

    //winter snowing and snowstorm check add at 2018.11.06
    if ( weather_info.mmhg <=  980 ) {
		if ( time_info.month <= 4)
			weather_info.sky = SKY_SNOWSTORM;
		else
			weather_info.sky = SKY_LIGHTNING;
	}else if ( weather_info.mmhg <= 1000 ) {
		if ( time_info.month <= 4)
			weather_info.sky = SKY_SNOWING;
		else
			weather_info.sky = SKY_RAINING;
	}else if ( weather_info.mmhg <= 1020 ) 
			weather_info.sky = SKY_CLOUDY;
    else	weather_info.sky = SKY_CLOUDLESS;

  }

  /*
   * Assign gsn's for skills which have them.
   */
  {
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
        if ( skill_table[sn].pgsn )
      *skill_table[sn].pgsn = sn;
    }
  }

  load_clans( );//clan should load before area
  
  /*
   * Read in all the area files.
   */
  num_greeting = 0;
  {
    FILE *fpList;

    if ( !( fpList = fopen( AREA_LIST, "r" ) ) )
    {
      perror( AREA_LIST );
      exit( 1 );
    }

    for ( ; ; )
    {
      int stat;

      strncpy( strArea, fread_word( fpList, &stat ),MAX_INPUT_LENGTH );
      if ( strArea[0] == '$' )
        break;

      if ( strArea[0] == '-' )
      {
        fpArea = stdin;
      }
      else
      {
        if ( !( fpArea = fopen( strArea, "r" ) ) )
        {
            perror( strArea );
            exit( 1 );
        }
      }

      for ( ; ; )
      {
        char *word;
    /*
        if ( fread_letter( fpArea ) != '#' )
        {
            bug( "Boot_db: # not found.", 0 );
            exit( 1 );
        }
    */
        char letter;
        letter = fread_letter( fpArea );

        if( letter != '#' && letter != '@' )
        {
            bug( "Boot_db: # not found.", 0 );
            exit( 1 );
        }

        word = fread_word( fpArea, &stat );

        if ( word[0] == '$'         )
          break;
        else if ( !str_cmp( word, "AREA"     ) )
          load_area  ( fpArea );
        else if ( !str_cmp( word, "HELPS"    ) ) 
          load_helps   ( fpArea );
        else if ( !str_cmp( word, "RECALL"   ) )
          load_recall  ( fpArea );
        else if ( !str_cmp( word, "MOBILES"  ) )
          load_mobiles ( fpArea );
        else if ( !str_cmp( word, "MOBPROGS" ) )
          load_mobprogs( fpArea );
        else if ( !str_cmp( word, "ROOMPROGS" ) )
          load_roomprogs( fpArea );
        else if ( !str_cmp( word, "OBJECTS"  ) )
          load_objects ( fpArea );
        else if ( !str_cmp( word, "RESETS"   ) )
          load_resets  ( fpArea );
        else if ( !str_cmp( word, "ROOMS"    ) )
          load_rooms   ( fpArea );
        else if ( !str_cmp( word, "SHOPS"    ) )
          load_shops   ( fpArea );
        else if ( !str_cmp( word, "SPECIALS" ) )
          load_specials( fpArea );
        else if ( !str_cmp( word, "MATERIAL" ) )
          load_material( fpArea );
        else if ( !str_cmp( word, "AREADATA" ) )  /* OLC */
        {
          new_load_area( fpArea );
          DIW_AREA = 0;
        } else if ( !str_cmp( word, "ROOMDATA" ) )  /* OLC 1.1b */
            new_load_rooms( fpArea );
        else if ( !str_cmp( word, "MERC" ) )    /* DIW area */
        {
            load_diw_area( fpArea );
            DIW_AREA = 1;
        }
        else
        {
            bug( "Boot_db: bad section name.", 0 );
            exit( 1 );
        }
      }

      fflush(NULL);
      if ( fpArea != stdin )
        fclose( fpArea );
      fpArea = NULL;
    }
    fflush(NULL);
    fclose( fpList );
  }

  /*
   * Fix up exits.
   * Declare db booting over.
   * Reset all areas once.
   * Load up the notes file.
   */
  {
    fix_exits( );
    fBootDb  = FALSE;
    daPrompt = str_dup(
        "[m<¥Í©R[31m%h[mÅ]ªk[1;34m%m[m²¾°Ê[35m%v[m> " );
    boot_done( );
    area_update( );
    load_notes( );
    MOBtrigger = TRUE;
    load_ban( );
    load_down_time( );
    top10_load();
    load_basic_types();
    qmark_list = NULL;
    qmark_free = NULL;
    qmark_string_list = NULL;
  }

  return;
}

void load_basic_types()
{
  OBJ_INDEX_DATA *oid;
  int i;

  for( i = 0; i < MAX_TYPES ; ++i ){
    oid = get_obj_index( OBJ_VNUM_HIT + i );
    if( !oid ) {
      bugf("Basic weapon %d not found.\n\r", OBJ_VNUM_HIT + i );
      exit(0);
    }
    basic_types[i] = oid ;
  }
}


void load_symbol()
{
    FILE *fp;
    int status;
    SYMBOL_DATA *pSymbol;
    int letter;
    char *word;

    if( !( fp = fopen(SYMBOL_FILE, "r")) ){
  bug("symbol file not found!",0);
  return;
    }
    for( ; ; )
    {
  word = fread_word( fp , &status);
  if( !word ) break;
  if( feof( fp ) )
      break;
  if( word[0] == '*' )
  {
      fread_to_eol( fp );
      continue;
  }
  if( !str_cmp(word,"#EndSymbol") )
      break;
  pSymbol = alloc_mem( sizeof(*pSymbol) );
  pSymbol->str = str_dup(word);
  pSymbol->num = fread_number( fp , &status);
  if( !isalpha(pSymbol->str[0]) )
  {
      log_string(pSymbol->str);
      bug("Load_Symbol: Not Alpha Symbol!",0);
      exit(0);
  }
  letter = LOWER(pSymbol->str[0])-'a';
  pSymbol->next = symbol_hash[letter];
  symbol_hash[letter] = pSymbol;
    }
    fflush( NULL );
  fclose( fp );


}


/*
 * Snarf an 'area' header line.
 */
void load_area( FILE *fp )
{
	AREA_DATA *pArea;
	int stat;

	pArea   = alloc_mem( sizeof( *pArea ) );  /* pArea->reset_first  = NULL; pArea->reset_last = NULL; OLC-Removed */
	pArea->name   = fread_string( fp, &stat );
	pArea->recall = ROOM_VNUM_TEMPLE;
	pArea->area_flags = AREA_LOADING;   /* OLC */
	pArea->security = 1;      /* OLC */
	pArea->builders = str_dup( "None" );  /* OLC */
	pArea->lvnum  = 0;      /* OLC */
	pArea->uvnum  = 0;      /* OLC */
	pArea->vnum   = top_area;   /* OLC */
	pArea->filename = str_dup( strArea ); /* OLC */
	pArea->age    = 15;
	pArea->nplayer  = 0;

	if ( !area_first )
		area_first = pArea;
	if (  area_last  )
	{
		area_last->next = pArea;
		REMOVE_BIT(area_last->area_flags, AREA_LOADING);  /* OLC */
	}
	area_last = pArea;
	pArea->next = NULL;

	top_area++;
	return;
}

void load_diw_area( FILE *fp )
{
	AREA_DATA *pArea;
	int stat;
	char *word;
	char letter;
	pArea = alloc_mem( sizeof( *pArea ) );  /* pArea->reset_first  = NULL; pArea->reset_last = NULL; OLC-Removed  */ 
	// pArea->name = fread_string( fp, &stat );
	pArea->recall = ROOM_VNUM_TEMPLE;
	pArea->area_flags = AREA_LOADING;     /* OLC */
	pArea->security = 1;                  /* OLC */
	pArea->builders = str_dup( "None" );  /* OLC */
	pArea->lvnum  = 0;                    /* OLC */
	pArea->uvnum  = 0;                    /* OLC */
	pArea->vnum   = top_area;             /* OLC */
	pArea->filename = str_dup( strArea ); /* OLC */
	pArea->age    = 15;
	pArea->nplayer  = 0;
	pArea->f_toggle = 0;

	word = fread_word( fpArea, &stat );
	if ( !str_cmp( word, "#AREA" ) )
	{
		pArea->name   = fread_string( fp, &stat );
		pArea->builders   = diw_fread_string( fp, &stat );
		pArea->translator = diw_fread_string( fp, &stat );
		pArea->modifier   = diw_fread_string( fp, &stat );
		fread_string( fp, &stat );//remove Wind
		pArea->identity   = fread_string( fp, &stat );
	}

	letter = fread_letter( fp );
	if ( letter == 'F' )
		pArea->f_toggle = fread_number( fp, &stat );
	else
		ungetc( letter, fp );

	if ( !area_first )
		area_first = pArea;
	if (  area_last  )
	{
		area_last->next = pArea;
		REMOVE_BIT(area_last->area_flags, AREA_LOADING);  /* OLC */
	}
	area_last = pArea;
	pArea->next = NULL;

	top_area++;
	return;
}

/*
 * OLC
 * Use these macros to load any new area formats that you choose to
 * support on your MUD.  See the new_load_area format below for
 * a short example.
 */
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )        \
    if ( !str_cmp( word, literal ) )    \
    {           \
        field  = value;       \
        fMatch = TRUE;        \
        break;          \
    }

#define SKEY( string, field )         \
    if ( !str_cmp( word, string ) )     \
    {           \
        free_string( field );     \
        field = fread_string( fp, &stat );     \
        fMatch = TRUE;        \
    }



/* OLC
 * Snarf an 'area' header line.   Check this format.  MUCH better.  Add fields
 * too.
 *
 * #AREAFILE
 * Name   { All } Locke    Newbie School~
 * Repop  A teacher pops in the room and says, 'Repop coming!'~
 * Recall 3001
 * End
 */
void new_load_area( FILE *fp )
{
	AREA_DATA *pArea;
	CLAN_DATA *cn;
	char *word;
	bool fMatch;

	pArea   = alloc_mem( sizeof(*pArea) );
	pArea->age    = 15;
	pArea->nplayer  = 0;
	pArea->filename = str_dup( strArea );
	pArea->vnum   = top_area;
	pArea->name   = str_dup( "New Area" );
	pArea->builders = str_dup( "" );
	pArea->security = 1;
	pArea->lvnum  = 0;
	pArea->uvnum  = 0;
	pArea->area_flags = 0;
	pArea->recall = ROOM_VNUM_TEMPLE;
	pArea->death  = 0;
	pArea->f_toggle = 0;
	pArea->reborn       = 0;

	for ( ; ; )
	{
		int  stat;
		word = feof( fp ) ? "End" : fread_word( fp, &stat );
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
			case 'N':
				SKEY( "Name", pArea->name );
				// bug(pArea->name,0);
				break;
			case 'I':
				SKEY( "Identity", pArea->identity );
				for( cn = clan_list; cn; cn = cn->next )
					if ( !str_cmp( pArea->identity, cn->founder ) )
						pArea->clan = cn;
				break;
			case 'B':
				SKEY( "Builders", pArea->builders );
				break;
			case 'T':
				SKEY( "Translator", pArea->translator );
				break;
			case 'M':
				SKEY( "Modifier", pArea->modifier );
				break;
			case 'S':
				KEY( "Security", pArea->security, fread_number( fp, &stat ) );
				break;
			case 'V':
				if ( !str_cmp( word, "VNUMs" ) )
				{
					pArea->lvnum = fread_number( fp, &stat );
					pArea->uvnum = fread_number( fp, &stat );
				}
				break;
			case 'E':
				if ( !str_cmp( word, "End" ) )
				{
					fMatch = TRUE;
					if ( area_first == NULL )
						area_first = pArea;
					if ( area_last  != NULL )
						area_last->next = pArea;
					area_last   = pArea;
					pArea->next = NULL;
					top_area++;
					if( pArea->death < 1 ) {
						if( pArea->recall == ROOM_VNUM_TEMPLE)
							pArea->death = ROOM_VNUM_ALTAR;
						else
							pArea->death = pArea->recall;
					}
					return;
				}
				break;
			case 'R':
				KEY( "Recall", pArea->recall, fread_number( fp, &stat ) );
				break;
			case 'A':
				KEY( "Areborn", pArea->reborn, fread_number( fp, &stat) );
				break;
			case 'D':
				KEY( "Death", pArea->death, fread_number( fp, &stat ) );
				break;
			case 'F':
				KEY( "F_toggle", pArea->f_toggle, fread_number( fp, &stat ) );
				break;
		}
	}
}



/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum( int vnum )
{
	if ( area_last->lvnum == 0 || area_last->uvnum == 0 )
		area_last->lvnum = area_last->uvnum = vnum;
	if ( vnum != URANGE( area_last->lvnum, vnum, area_last->uvnum ) )
	{
		if ( vnum < area_last->lvnum )
			area_last->lvnum = vnum;
		else
			area_last->uvnum = vnum;
	}
	return;
}



/*
 * Snarf a help section.
 */
void load_helps( FILE *fp )
{
	HELP_DATA *pHelp;
	char *keyword;
	int level;
	int stat;

	for ( ; ; )
	{
		level   = fread_number( fp, &stat );
		keyword   = fread_string( fp, &stat );
		if ( keyword[0] == '$' ) {
			free_string( keyword );
			break;
		}
		pHelp = alloc_mem( sizeof( *pHelp ) );
		pHelp->level = level;
		pHelp->keyword = keyword;
		pHelp->text = fread_string( fp, &stat );

		if ( !str_cmp( pHelp->keyword, "GREETING" ) && num_greeting < MAX_GREETING)
			help_greeting[num_greeting++] = pHelp->text;

		if ( !help_first )
			help_first = pHelp;
		if (  help_last  )
			help_last->next = pHelp;

		help_last = pHelp;
		pHelp->next = NULL;
		top_help++;
	}

	return;
}



/*
 * Snarf a recall point.
 */
void load_recall( FILE *fp )
{
	AREA_DATA *pArea;
	char buf [ MAX_STRING_LENGTH ];
	int stat;

	pArea   = area_last;
	pArea->recall = fread_number( fp, &stat );

	if ( pArea->recall < 1 )
	{
		sprintf( buf, "Load_recall:  %s invalid recall point", pArea->name );
		bug( buf, 0 );
		pArea->recall = ROOM_VNUM_TEMPLE;
	}

	return;

}



/*
 * Snarf a mob section.
 */
void load_mobiles( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;
    MOB_WEAKNESS *weak;
    int type, mod;
    bool fAutoset;   //Added by Razgriz    20050720

    if ( !area_last ) /* OLC */
    {
        bug( "Load_mobiles: no #AREA seen yet.", 0 );
        exit( 1 );
    }

    for ( ; ; )
    {
        char *race;
        char  letter;
        int   vnum;
        int   iHash;
        int   stat;

        fAutoset = TRUE;   //Added by Razgriz    20050720

        letter = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_mobiles: # not found.", 0 );
            exit( 1 );
        }

        vnum = fread_number( fp, &stat );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_mob_index( vnum ) )
        {
            bug( "Load_mobiles: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;

        pMobIndex     = new_mob_index();
        pMobIndex->vnum     = vnum;
        pMobIndex->area     = area_last;    /* OLC */
        pMobIndex->player_name    = fread_string( fp, &stat );
        pMobIndex->short_descr    = fread_string( fp, &stat );
        /*pMobIndex->cname    = fread_string( fp, &stat );*/
        pMobIndex->long_descr   = fread_string( fp, &stat );
        pMobIndex->description    = fread_string( fp, &stat );

        pMobIndex->long_descr[0]  = UPPER( pMobIndex->long_descr[0]  );
        pMobIndex->description[0] = UPPER( pMobIndex->description[0] );

        pMobIndex->act      = fread_number( fp, &stat ) | ACT_IS_NPC;
        pMobIndex->affected_by    = fread_number( fp, &stat );
        pMobIndex->pShop    = NULL;
        pMobIndex->alignment    = fread_number( fp, &stat );
        letter        = fread_letter( fp );
        pMobIndex->level    = number_fuzzy( fread_number( fp, &stat ) );

        if ( pMobIndex->level < 36 ) fAutoset = FALSE;   //added by Razgriz    20050720

        /*
         * The unused stuff is for imps who want to use the old-style
         * stats-in-files method.
         */
        /*
           pMobIndex->hitroll   = fread_number( fp, &stat );   
           pMobIndex->ac      = fread_number( fp, &stat );   
           pMobIndex->hitnodice   = fread_number( fp, &stat );  
           'd'          fread_letter( fp ); 
           pMobIndex->hitsizedice   = fread_number( fp, &stat );  
           '+'          fread_letter( fp ); 
           pMobIndex->hitplus   = fread_number( fp, &stat ); 
           pMobIndex->damnodice   = fread_number( fp, &stat );   
           'd'          fread_letter( fp ); 
           pMobIndex->damsizedice   = fread_number( fp, &stat );   
           '+'          fread_letter( fp ); 
           pMobIndex->damplus   = fread_number( fp, &stat );  
           pMobIndex->gold      = fread_number( fp, &stat );   
           */
        pMobIndex->mod_hitplus    = fread_number( fp, &stat );  
        pMobIndex->mod_mana   = fread_number( fp, &stat );  
        pMobIndex->mod_hitroll    = fread_number( fp, &stat );  
        /* 'd'    */      fread_letter( fp );
        pMobIndex->mod_damroll    = fread_number( fp, &stat );   
        /* '+'    */      fread_letter( fp );
        pMobIndex->mod_ac   = fread_number( fp, &stat );  
        pMobIndex->mod_svs[0]   = fread_number( fp, &stat );   
        /* 'd'    */      fread_letter( fp ); 
        pMobIndex->dodge    = fread_number( fp, &stat );   
        /* '+'    */      fread_letter( fp ); 
        pMobIndex->mod_pp   = fread_number( fp, &stat );
        pMobIndex->gold     = fread_number( fp, &stat );  

        /* xp   */      fread_number( fp, &stat );   /* Unused */
        /* position */      fread_number( fp, &stat );   /* Unused */
        //  race        = fread_string( fp, &stat ); 
        /* position     */      fread_number( fp, &stat );   /* Unused */
        pMobIndex->sex      = fread_number( fp, &stat );
        /*
           pMobIndex->race = race_lookup( race );
           free_string( race );
           if ( pMobIndex->race < 0 )
           {
           bug( "Load_mobiles: vnum %d bad race.", vnum );
           pMobIndex->race = 0;
           }
           */
        if ( letter != 'S' )
        {
            bug( "Load_mobiles: vnum %d non-S.", vnum );
            exit( 1 );
        }

        for ( ; ; )
        {
            char letter;
            bool finished = FALSE;

            letter = fread_letter( fp );

            switch( letter )
            {
                case 'S':
                    type = fread_number(fp, &stat);
                    mod = fread_number(fp, &stat);
                    switch( type ) {
                        case APPLY_NONE:
                            break;
                        case APPLY_STR:
                            //if ( !IS_NPC( pMobIndex ) )
                            pMobIndex->mod_str += mod;
                            //  else
                            //  pMobIndex->str += mod;
                            break;
                        case APPLY_DEX:
                            //if ( !IS_NPC( pMobIndex ) )
                            pMobIndex->mod_dex += mod;
                            //  else
                            //      pMobIndex->dex += mod;
                            break;
                        case APPLY_INT:
                            //if ( !IS_NPC( pMobIndex ) )
                            pMobIndex->mod_int += mod;    
                            //  else 
                            //      pMobIndex->int += mod;
                            break;
                        case APPLY_WIS:
                            //if ( !IS_NPC( pMobIndex ) )
                            pMobIndex->mod_wis += mod;    
                            //  else 
                            //      pMobIndex->wis += mod;
                            break;
                        case APPLY_CON:
                            //if ( !IS_NPC( pMobIndex ) )
                            pMobIndex->mod_con += mod;
                            //  else
                            //      pMobIndex->con += mod;
                            break;
                        case APPLY_SEX:       
                            pMobIndex->sex+= mod; 
                            break;
                        case APPLY_RACE:        
                            pMobIndex->race = abs( pMobIndex->race + mod); 
                            break;
                        case APPLY_CLASS:
                            break;
                        case APPLY_LEVEL:
                            break;
                        case APPLY_AGE:
                            break;
                        case APPLY_HEIGHT:
							pMobIndex->mod_size += mod;  // Åé«¬ 2022/12/23
                            break;
                        case APPLY_WEIGHT:
                            break;
                        case APPLY_MANA:        
                            pMobIndex->mod_mana += mod; 
                            break;
                        case APPLY_HIT:       
                            pMobIndex->mod_hitplus += mod; 
                            break;
                        case APPLY_MOVE:       
                            /* pMobIndex->mod_move    += mod;*/ 
                            break;
                        case APPLY_GOLD:
                            break;
                        case APPLY_EXP: 
                            pMobIndex->mod_exp += mod;
                            break;
                        case APPLY_AC:        
                            pMobIndex->mod_ac += mod; 
                            break;
                        case APPLY_HITROLL:       
                            pMobIndex->mod_hitroll += mod; 
                            break;
                        case APPLY_DAMROLL:       
                            pMobIndex->mod_damroll += mod; 
                            break;
                        case APPLY_SAVING_PARA:   
                            pMobIndex->mod_svs[0] += mod; 
                            break;
                        case APPLY_SAVING_LIFE:    // ¥Î©ó mar ¤£¦º§P©w 2020/10/28
                            //pMobIndex->mod_svs[MAGIC_BREATH] += mod; 
                            break;
                        case APPLY_SAVING_SPELL:  
                            pMobIndex->mod_svs[0] += mod; 
                            break;
                        case APPLY_SAVING_PETRI:  
                        case APPLY_SAVING_EARTH:
                            pMobIndex->mod_svs[MAGIC_EARTH] += mod;
                            break;
                        case APPLY_SAVING_SAINT:
						case APPLY_SAVING_GOOD:
                            pMobIndex->mod_svs[MAGIC_SAINT] += mod;
                            break;
                        case APPLY_SAVING_FIRE:
                            pMobIndex->mod_svs[MAGIC_FIRE] += mod; 
                            break;
                        case APPLY_SAVING_COLD:
                            pMobIndex->mod_svs[MAGIC_COLD] += mod; 
                            break;
                        case APPLY_SAVING_LIGHTNING:
                            pMobIndex->mod_svs[MAGIC_LIGHTNING] += mod;
                            break;
                        case APPLY_SAVING_WIND:
                            pMobIndex->mod_svs[MAGIC_WIND] += mod; 
                            break;
                        case APPLY_SAVING_DARK:
						case APPLY_SAVING_EVIL:
                            pMobIndex->mod_svs[MAGIC_DARK] += mod;
                            break;
						/* 
						// ·s¼W mob sv poison ¶Ë®`§P©w  2021/11/03 
						// ±N evil , good ²¾¦Ü sv dark ¤Î sv saint ¤U, ancient ¤À¶}¬°¤£¦P¶µ¥Ø
						case APPLY_SAVING_POISON:
						case APPLY_SAVING_ANCIENT:
                        case APPLY_SAVING_EVIL:
                        case APPLY_SAVING_GOOD:
							pMobIndex->mod_svs[0] += mod;
						break;
						*/
						case APPLY_SAVING_ANCIENT:
							break;
                        case APPLY_SAVING_POISON:
							pMobIndex->mod_svs[MAGIC_POISON] += mod;
                            break;
                        case APPLY_SAVING_ELEMENT:
                            pMobIndex->mod_svs[MAGIC_FIRE] += mod; 
                            pMobIndex->mod_svs[MAGIC_WIND] += mod; 
                            pMobIndex->mod_svs[MAGIC_COLD] += mod; 
                            pMobIndex->mod_svs[MAGIC_EARTH]+= mod;
                            pMobIndex->mod_svs[MAGIC_LIGHTNING] += mod; 
                            break;
                        case APPLY_SAVING_BREATH: 
                            pMobIndex->mod_svs[MAGIC_BREATH] += mod; break;
                        /*  case APPLY_SAVING_SPELL:  
                         *  pMobIndex->mod_svs[MAGIC_SPELL] += mod; 
                         *  break;
                         *  case APPLY_SAVING_ANCIENT:  
                         *  pMobIndex->mod_svs[MAGIC_ANCIENT] += mod; 
                         *  break;
                         *  case APPLY_SAVING_SPIRIT:  
                         *  pMobIndex->mod_svs[MAGIC_SPIRIT] += mod; 
                         *  break;
                         *  case APPLY_SAVING_FIRE:  
                         *  pMobIndex->mod_svs[MAGIC_FIRE] += mod; 
                         *  break;
                         *  case APPLY_SAVING_COLD:  
                         *  pMobIndex->mod_svs[MAGIC_COLD] += mod; 
                         *  break;
                         *  case APPLY_SAVING_LIGHTNING:  
                         *  pMobIndex->mod_svs[MAGIC_LIGHTNING] += mod; 
                         *  break;
                         *  case APPLY_SAVING_EARTH:  
                         *  pMobIndex->mod_svs[MAGIC_EARTH] += mod; 
                         *  break;
                         *  case APPLY_SAVING_WIND:  
                         *  pMobIndex->mod_svs[MAGIC_WIND] += mod; 
                         *  break;
                         *  case APPLY_SAVING_SAINT:  
                         *  pMobIndex->mod_svs[MAGIC_SAINT] += mod; 
                         *  break;
                         *  case APPLY_SAVING_DARK:  
                         *  pMobIndex->mod_svs[MAGIC_DARK] += mod; 
                         *  break;
                         *  case APPLY_SAVING_POISON: 
                         *  pMobIndex->mod_svs[MAGIC_POISON] += mod; 
                         *  break;
                         *  case APPLY_SAVING_NATURE:  
                         *  pMobIndex->mod_svs[MAGIC_NATURE] += mod; 
                         *  break;
                         *  case APPLY_SAVING_GOOD:  
                         *  pMobIndex->mod_svs[MAGIC_GOOD] += mod; 
                         *  break;
                         *  case APPLY_SAVING_EVIL:  
                         *  pMobIndex->mod_svs[MAGIC_EVIL] += mod; 
                         *  break;
                         *  */
                        case APPLY_RDAMAGE: 
                            pMobIndex->pdr += mod;
                            fAutoset = FALSE;   //Added by Razgriz    20050720 
                            break;
                        case APPLY_RMAGIC:  
                            pMobIndex->mdr += mod; 
                            fAutoset = FALSE;   //Added by Razgriz    20050720
                            break;
                        case APPLY_ADAMAGE:     
                            pMobIndex->apdr += mod;
                            fAutoset = FALSE;   //Added by Razgriz    20050720
                            break;
                        case APPLY_AMAGIC:      
                            pMobIndex->amdr += mod;
                            fAutoset = FALSE;   //Added by Razgriz    20050720
                            break;
                        case APPLY_GPHYSICAL: 
                            pMobIndex->pad += mod; 
                            break;
                        case APPLY_GMAGIC:  
                            pMobIndex->mad += mod; 
                            break;
                        case APPLY_POWER_POTENTIAL:  
                            pMobIndex->mod_pp += mod;
                            fAutoset = FALSE;   //Added by Razgriz    20050720
                            break;
                        case APPLY_DODGE:         
                            pMobIndex->dodge += mod; 
                            break;
                        default:
                            bugf("No such apply %d on mob %d", 
                                    type, pMobIndex->vnum);
                            break;
                    };
                    break;
                case 'R':
                    race = fread_string( fp, &stat );
                    pMobIndex->race = race_lookup( race );
                    free_string( race );
                    if ( pMobIndex->race < 0 )
                    {
                        bug( "Load_mobiles: vnum %d bad race.", vnum );
                        pMobIndex->race = 0;
                    }
                    break;
                case 'C':
                    pMobIndex->class = fread_number( fp, &stat );
                    break;
                case 'H':
                    pMobIndex->replica = fread_number(fp,&stat );
                    break;
                case 'P':
                    pMobIndex->special = fread_number( fp, &stat );
                    break;
                case 'K':
                    pMobIndex->psk = fread_string( fp, &stat );
                    break;
                case 'J':
                    pMobIndex->jcl = fread_string( fp, &stat );
                    break;
                case 'W':
                    weak = alloc_mem( sizeof ( *weak ) );
                    weak->name  = fread_string( fp, &stat );
                    weak->percent = fread_number( fp, &stat );
                    weak->damage = fread_number( fp, &stat );
                    if( pMobIndex->weakness ) 
                        weak->next = pMobIndex->weakness;
                    pMobIndex->weakness = weak;
                    break;
                case '>':
                    ungetc( letter, fp );
                    mprog_read_programs( fp, pMobIndex );
                    break;
                case 'A':
                    ungetc( letter, fp );
                    action_read_programs( fp, pMobIndex );
                    break;
                default :
                    ungetc( letter, fp );
                    finished = TRUE;
                    break;
            }
            if ( finished )
                break;
        }

        pMobIndex->fAutoset = fAutoset;   //Added by Razgriz    20050720
        iHash = vnum % MAX_KEY_HASH;
        pMobIndex->next = mob_index_hash[iHash];
        mob_index_hash[iHash] = pMobIndex;
        top_mob_index++;
        top_vnum_mob = top_vnum_mob < vnum ? vnum : top_vnum_mob;  /* OLC */
        assign_area_vnum( vnum );          /* OLC */
        kill_table[URANGE( 0, pMobIndex->level, MAX_LEVEL-1 )].number++;
    }

    return;
}

void load_material( FILE *fp )
{
	OBJ_INDEX_DATA *pObjIndex;
	AFFECT_DATA *paf;
	OBJ_HP *hp;
	OBJ_VRACE *vrace;
	char * buf;

	if ( !area_last ) /* OLC */
	{
		bug( "Load_objects: no #AREA seen yet.", 0 );
		exit( 1 );
	}

	for ( ; ; )
	{
		char *value [ 4 ];
		char  letter;
		int   vnum;
		int   stat;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
			bug( "Load_objects: # not found.", 0 );
			exit( 1 );
		}

		vnum = fread_number( fp, &stat );
		if ( vnum == 0 )
			break;

		fBootDb = FALSE;
		/*if ( get_obj_index( vnum ) )
		{
		bug( "Load_objects: vnum %d duplicated.", vnum );
		exit( 1 );
		}*/
		fBootDb = TRUE;

		pObjIndex = new_obj_index();
		pObjIndex->vnum = vnum;
		pObjIndex->area = area_last;    /* OLC */
		pObjIndex->name = fread_string( fp, &stat );
		pObjIndex->short_descr = fread_string( fp, &stat );
		/*pObjIndex->cname = fread_string( fp, &stat );*/
		pObjIndex->description = fread_string( fp, &stat );
		fread_string( fp, &stat ); /* Action description */

		pObjIndex->short_descr[0] = LOWER( pObjIndex->short_descr[0] );
		pObjIndex->description[0] = UPPER( pObjIndex->description[0] );

		pObjIndex->item_type = fread_number( fp, &stat );
		pObjIndex->extra_flags = fread_number( fp, &stat );
		pObjIndex->wear_flags = fread_number( fp, &stat );

		if ( DIW_AREA == 1 )
		{
			value[0] = diw_fread_string( fp, &stat );
			value[1] = diw_fread_string( fp, &stat );
			value[2] = diw_fread_string( fp, &stat );
			value[3] = diw_fread_string( fp, &stat );
		}
		else
		{
			value[0] = fread_string( fp, &stat );
			value[1] = fread_string( fp, &stat );
			value[2] = fread_string( fp, &stat );
			value[3] = fread_string( fp, &stat );
		}
		pObjIndex->weight = fread_number( fp, &stat );
		pObjIndex->cost = fread_number( fp, &stat );

		/* Cost per day */  fread_number( fp, &stat );   /* Unused */

		/*
		* Check here for the redundancy of invisible light sources - Kahn
		*/
		if ( pObjIndex->item_type == ITEM_LIGHT && IS_SET( pObjIndex->extra_flags, ITEM_INVIS ) )
		{
			bug( "Vnum %d : light source with ITEM_INVIS set", vnum );
			REMOVE_BIT( pObjIndex->extra_flags, ITEM_INVIS );
		}

		for ( ; ; )
		{
			char letter;
			bool finished = FALSE;
			letter = fread_letter( fp );

			switch( letter )
			{
				case 'A':
					paf = new_affect();
					paf->type = -1;
					paf->duration = -1;
					paf->location = fread_number( fp, &stat );
					paf->modifier = fread_number( fp, &stat );
					paf->bitvector = 0;
					paf->next = pObjIndex->affected;
					pObjIndex->affected = paf;
					top_affect++;
					break;
				case 'F':
					paf = new_affect();
					paf->type = -1;
					paf->duration = -1;
					paf->location = 0;
					paf->modifier = 0;
					paf->bitvector = fread_number( fp, &stat );
					paf->next = pObjIndex->affected;
					pObjIndex->affected = paf;
					top_affect++;
					break;
				case 'H':
					hp = new_hiddenpower();
					hp->vnum = vnum;
					if ( DIW_AREA == 1 )
						// hp->hp_name = skill_table[fread_number( fp, &stat )].name;
						hp->hp_name = diw_fread_string( fp, &stat );
					else
						hp->hp_name = fread_string( fp, &stat );
					hp->percent = fread_number( fp, &stat );
					hp->level = fread_number( fp, &stat );
					hp->event = fread_number( fp, &stat );
					//hp->next = obj_hp_list;
					//obj_hp_list = hp;
					hp->next = pObjIndex->hp;
					pObjIndex->hp = hp;
					break;
				case 'L':
					pObjIndex->level = fread_number( fp, &stat );
					break;
				case 'T':
					pObjIndex->timer = fread_number( fp, &stat );
					break;
				case 'C':
					pObjIndex->cost = fread_number( fp, &stat );
					if( pObjIndex->cost < 0 ) {
						bug("Obj's cost < 0, vnum = %d", pObjIndex->vnum);
						pObjIndex->cost = 0;
					}
					break;
				case 'P':
					pObjIndex->class  = fread_number( fp, &stat );
					break;
				case 'V':
					vrace = new_obj_vrace();
					buf = fread_string( fp ,&stat);
					vrace->race = race_lookup( buf );
					vrace->hr_mod_q = fread_number( fp, &stat);
					vrace->hr_mod_p = fread_number( fp, &stat);
					vrace->dr_mod_q = fread_number( fp, &stat);
					vrace->dr_mod_p = fread_number( fp, &stat);
					vrace->next = pObjIndex->vrace;
					pObjIndex->vrace = vrace;
					break;
				case 'R':
					pObjIndex->race   = fread_number( fp, &stat );
					break;
				default:
					ungetc( letter, fp );
					finished = TRUE;
					break;
			}
			if (finished)
				break;
		}

		/*
		* Translate character strings *value[] into integers:  sn's for
		* items with spells, or straight conversion for other items.
		* - Thelonius
		*/
		switch ( pObjIndex->item_type )
		{
			default:
				pObjIndex->value[0] = atoi( value[0] );
				pObjIndex->value[1] = atoi( value[1] );
				pObjIndex->value[2] = atoi( value[2] );
				pObjIndex->value[3] = atoi( value[3] );
				break;

			case ITEM_PILL:
			case ITEM_POTION:
			case ITEM_SCROLL:
				pObjIndex->value[0] = atoi( value[0] );
				pObjIndex->value[1] = skill_lookup( value[1] );
				pObjIndex->value[2] = skill_lookup( value[2] );
				pObjIndex->value[3] = skill_lookup( value[3] );
				break;

			case ITEM_STAFF:
			case ITEM_WAND:
				pObjIndex->value[0] = atoi( value[0] );
				pObjIndex->value[1] = atoi( value[1] );
				pObjIndex->value[2] = atoi( value[2] );
				pObjIndex->value[3] = skill_lookup( value[3] );
				break;
		}

		if( !material_list ) {
			pObjIndex->vnum = 1;
			material_list = pObjIndex;
		}else{
			pObjIndex->vnum = material_list->vnum + 1;
			pObjIndex->next = material_list;
			material_list = pObjIndex;
		}
		top_obj_index++;
		free_string( value[0] );
		free_string( value[1] );
		free_string( value[2] );
		free_string( value[3] );
	}

	return;
}


/*
 * Snarf an obj section.
 */
void load_objects( FILE *fp )
{
	OBJ_INDEX_DATA *pObjIndex;
	AFFECT_DATA *paf;
	// ALIGN_AFFECT_DATA *align_af;
	EXTRA_DESCR_DATA *ed;
	OBJ_HP *hp;
	OBJ_VRACE *vrace;
	char * buf;
	int m_time;

	if ( !area_last ) /* OLC */
	{
		bug( "Load_objects: no #AREA seen yet.", 0 );
		exit( 1 );
	}

	for ( ; ; )
	{
		char *value [ 4 ];
		char  letter;
		int   vnum;
		int   iHash;
		int   stat;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
			bug( "Load_objects: # not found.", 0 );
			exit( 1 );
		}

		vnum = fread_number( fp, &stat );
		if ( vnum == 0 )
		break;

		fBootDb = FALSE;
		if ( get_obj_index( vnum ) )
		{
			bug( "Load_objects: vnum %d duplicated.", vnum );
			exit( 1 );
		}
		fBootDb = TRUE;

		pObjIndex = new_obj_index();
		pObjIndex->vnum = vnum;
		pObjIndex->area = area_last;    /* OLC */
		free_string( pObjIndex->name );
		pObjIndex->name = fread_string( fp, &stat );
		free_string( pObjIndex->short_descr );
		pObjIndex->short_descr = fread_string( fp, &stat );
		//pObjIndex->cname = fread_string( fp, &stat );
		free_string( pObjIndex->description );
		pObjIndex->description = fread_string( fp, &stat );
		fread_string( fp, &stat ); /* Action description */ 

		pObjIndex->short_descr[0] = LOWER( pObjIndex->short_descr[0] );
		pObjIndex->description[0] = UPPER( pObjIndex->description[0] );

		pObjIndex->item_type = fread_number( fp, &stat );
		pObjIndex->extra_flags = fread_number( fp, &stat );
		pObjIndex->wear_flags = fread_number( fp, &stat );

		if ( DIW_AREA == 1 )
		{
			value[0] = diw_fread_string( fp, &stat );
			value[1] = diw_fread_string( fp, &stat );
			value[2] = diw_fread_string( fp, &stat );
			value[3] = diw_fread_string( fp, &stat );
		}
		else
		{
			value[0] = fread_string( fp, &stat );
			value[1] = fread_string( fp, &stat );
			value[2] = fread_string( fp, &stat );
			value[3] = fread_string( fp, &stat );
		}
		pObjIndex->weight = fread_number( fp, &stat );
		pObjIndex->cost = fread_number( fp, &stat );
		if( pObjIndex->cost < 0 ) {
			//bug("Obj's cost < 0, vnum = %d", pObjIndex->vnum);
			pObjIndex->cost = 0;
		}

		pObjIndex->max_level = fread_number( fp, &stat );   /* Unused */

		/*
		* Check here for the redundancy of invisible light sources - Kahn
		*/
		if ( pObjIndex->item_type == ITEM_LIGHT && IS_SET( pObjIndex->extra_flags, ITEM_INVIS ) )
		{
			bug( "Vnum %d : light source with ITEM_INVIS set", vnum );
			REMOVE_BIT( pObjIndex->extra_flags, ITEM_INVIS );
		}

		for ( ; ; )
		{
			char letter;
			//char *str;
			//char *str2;
			//char *str1;
			bool finished = FALSE;
			letter = fread_letter( fp );

			switch( letter )
			{
				case '>':
					ungetc( letter, fp );
					oprog_read_programs( fp, pObjIndex );
					break;
				case 'A':
					paf = new_affect();
					paf->type = -1;
					paf->duration = -1;
					paf->location = fread_number( fp, &stat );
					paf->modifier = fread_number( fp, &stat );
					paf->bitvector = 0;
					paf->next = pObjIndex->affected;
					pObjIndex->affected = paf;
					top_affect++;
					break;
				case 'E':
					ed = alloc_mem( sizeof( *ed ) );
					ed->keyword = fread_string( fp, &stat );
					ed->description = fread_string( fp, &stat );
					ed->next = pObjIndex->extra_descr;
					pObjIndex->extra_descr = ed;
					top_ed++;
					break;
				case 'H':
					hp = new_hiddenpower();
					hp->vnum = vnum;
					if ( DIW_AREA == 1 )
						// hp->hp_name = skill_table[fread_number( fp, &stat )].name;
						hp->hp_name = diw_fread_string( fp, &stat );
					else
						hp->hp_name = fread_string( fp, &stat );
					hp->percent = fread_number( fp, &stat );
					hp->level = fread_number( fp, &stat );
					hp->event = fread_number( fp, &stat );
					//hp->next = obj_hp_list;
					//obj_hp_list = hp;
					hp->next = pObjIndex->hp;
					pObjIndex->hp = hp;
					break;
				case 'L':
					pObjIndex->level  = fread_number( fp, &stat );
					break;
				case 'T':
					pObjIndex->timer  = fread_number( fp, &stat );
					break;
				case 'O':
					pObjIndex->horse  = fread_number( fp, &stat );
					break;
				case 'C':
					pObjIndex->cost   = fread_number( fp, &stat );
					if( pObjIndex->cost < 0 ) {
						//bug("Obj's cost < 0, vnum = %d", pObjIndex->vnum);
						pObjIndex->cost = 0;
					}
					break;
				case 'P':
					pObjIndex->class  = fread_number( fp, &stat );
					break;
				case 'V':
					vrace = new_obj_vrace();
					buf = fread_string( fp ,&stat);
					vrace->race = race_lookup( buf );
					vrace->hr_mod_q = fread_number( fp, &stat);
					vrace->hr_mod_p = fread_number( fp, &stat);
					vrace->dr_mod_q = fread_number( fp, &stat);
					vrace->dr_mod_p = fread_number( fp, &stat);
					vrace->next = pObjIndex->vrace;
					pObjIndex->vrace = vrace;
					break;
					/*case 'B':
					hp = new_hiddenpower();
					hp->vnum = vnum;
					hp->hp_name = fread_string( fp, &stat );
					hp->percent = fread_number( fp, &stat );
					hp->level = fread_number( fp, &stat );
					hp->event = fread_number( fp, &stat );
					for( str = fread_string( fp, &stat ); str_cmp( str, "endb" ) ; str = fread_string( fp, &stat ) )
					{
					if ( !str_cmp( str, "pr" )
					{
					for( str1 = fread_string( fp, &stat ); str_cmp( str1, "endb" ); str = fread_string( fp, &stat ) )
					{
					hp->p_race = NUM_BIT( material_lookup( str1 ) );
					}
					}
					}
					//hp->next = obj_hp_list;
					//obj_hp_list = hp;
					hp->next = pObjIndex->hp;
					pObjIndex->hp = hp;
					break;*/

					/*case 'B':
					str = fread_string( fp, &stat );
					if( str[0] == 'a' )
					pObjIndex->hp_use_align = fread_number( fp, &stat );
					else if( str[0] == 'r' )
					pObjIndex->hp_use_race = fread_number( fp, &stat );
					else if( str[0] == 'c' )
					pObjIndex->hp_use_class = fread_number( fp, &stat );
					else if( str[0] == 's' )
					pObjIndex->hp_use_sex = fread_number( fp, &stat );
					break;*/
				case 'Z':
					pObjIndex->souvenir = fread_number( fp, &stat );
					break;
				case 'S':
					//pObjIndex->special_limitation = fread_number( fp, &stat );
					break;
					/* case 'D':
					str = fread_string( fp, &stat );
					if( str[0] == 'a' )
					{
					str1 = fread_string( fp, &stat );
					if( str[0] == 'a' )
					pObjIndex->hp_affect_align = ALLOW_BIT + fread_number( fp, &stat );
					else if( str[0] == 's' )
					pObjIndex->hp_affect_sex = fread_number( fp, &stat );
					else if( str[0] == 'r' )
					pObjIndex->hp_affect_race += NUM_BIT( race_lookup( fread_string( fp, &stat ) ) );
					else if( str[0] == 'c' )
					pObjIndex->hp_affect_class += NUM_BIT( class_lookup( fread_string( fp, &stat ) ) );
					}
					else if( str[0] == 'n' )
					{
					str1 = fread_string( fp, &stat );
					if( str[0] == 'r' )
					pObjIndex->hp_noeffect_race += NUM_BIT( race_lookup( fread_string( fp, &stat ) ) );
					else if( str[0] == 'c' )
					pObjIndex->hp_noeffect_class += NUM_BIT( class_lookup( fread_string( fp, &stat ) ) );
					}
					break; */
					/*case 'D':
					str = fread_string( fp, &stat );
					if( str[0] == 'a' )
					{
					align_af      = new_affect();
					align_af->type    = -1;
					align_af->duration    = -1;
					align_af->location    = fread_number( fp, &stat );
					align_af->modifier    = fread_number( fp, &stat );
					align_af->bitvector   = 0;
					align_af->next    = pObjIndex->align_affected;
					pObjIndex->affected = align_af;
					top_affect++;
					}
					if( str[0] == 'c' )
					{
					}
					if( str[0] == 'r' )
					{
					}
					if( str[0] == 's' )
					{
					}
					break;
					*/
					/*case 'B':
					pObjIndex->hp_use_align = fread_number( fp, &stat );
					break;
					case 'I':
					pObjIndex->hp_use_race = fread_number( fp, &stat );
					break;
					case 'J':
					pObjIndex->hp_use_class = fread_number( fp, &stat );
					break;
					case 'K':
					pObjIndex->hp_use_sex = fread_number( fp, &stat );
					break; */
				case 'X':
					buf = fread_string( fp, &stat );
					pObjIndex->material = material_lookup(buf);
					break;
				case 'R':
					pObjIndex->race = fread_number( fp, &stat );
					break;
				case 'F':
					paf = new_affect();
					paf->type = -1;
					paf->duration = -1;
					paf->location = 0;
					paf->modifier = 0;
					paf->bitvector = fread_number( fp, &stat );
					paf->next = pObjIndex->affected;
					pObjIndex->affected = paf;
					top_affect++;
					break;
				case 'U':
					pObjIndex->replica = fread_number(fp, &stat);
					SET_BIT(pObjIndex->extra_flags, ITEM_UNIQUE);
					SET_BIT(pObjIndex->extra_flags, ITEM_NOSAVE);
					break;
				case 'M':
					m_time      = fread_number( fp, &stat );
					if ( m_time == 0 )
						pObjIndex->on_message = fread_string( fp, &stat );
					else if( m_time == 1 )
						pObjIndex->off_message  = fread_string( fp, &stat );
					else if( m_time == 2 )
						pObjIndex->hit_message = fread_string(fp, &stat);
					break;
				default:
					ungetc( letter, fp );
					finished = TRUE;
					break;
			}
			if (finished)
				break;
		}

		if( !pObjIndex->material )
			pObjIndex->material = material_lookup( "Generic");
		/*
		* Translate character strings *value[] into integers:  sn's for
		* items with spells, or straight conversion for other items.
		* - Thelonius
		*/
		switch ( pObjIndex->item_type )
		{
			default:
				pObjIndex->value[0] = atoi( value[0] );
				pObjIndex->value[1] = atoi( value[1] );
				pObjIndex->value[2] = atoi( value[2] );
				pObjIndex->value[3] = atoi( value[3] );
				break;

			case ITEM_PILL:
			case ITEM_POTION:
			case ITEM_SCROLL:
				pObjIndex->value[0] = atoi( value[0] );
				pObjIndex->value[1] = skill_lookup( value[1] );
				pObjIndex->value[2] = skill_lookup( value[2] );
				pObjIndex->value[3] = skill_lookup( value[3] );
				break;

			case ITEM_STAFF:
			case ITEM_WAND:
				pObjIndex->value[0] = atoi( value[0] );
				pObjIndex->value[1] = atoi( value[1] );
				pObjIndex->value[2] = atoi( value[2] );
				pObjIndex->value[3] = skill_lookup( value[3] );
				break;
		}

		iHash = vnum % MAX_KEY_HASH;
		pObjIndex->next = obj_index_hash[iHash];
		obj_index_hash[iHash] = pObjIndex;
		top_obj_index++;
		top_vnum_obj = top_vnum_obj < vnum ? vnum : top_vnum_obj;  /* OLC */
		assign_area_vnum( vnum );          /* OLC */
		free_string( value[0] );
		free_string( value[1] );
		free_string( value[2] );
		free_string( value[3] );
	}

	return;
}



/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset( ROOM_INDEX_DATA *pR, RESET_DATA *pReset )
{
    RESET_DATA *pr;

    if ( !pR )
       return;

    pr = pR->reset_last;

    if ( !pr )
    {
  pR->reset_first = pReset;
  pR->reset_last  = pReset;
    }
    else
    {
  pR->reset_last->next = pReset;
  pR->reset_last       = pReset;
  pR->reset_last->next = NULL;
    }

    top_reset++;
    return;
}



/*
 * Snarf a reset section. Changed for OLC.
 */
void load_resets( FILE *fp )
{
	RESET_DATA *pReset;
	int iLastRoom = 0;
	int iLastObj  = 0;

	if ( !area_last )
	{
		bug( "Load_resets: no #AREA seen yet.", 0 );
		exit( 1 );
	}

	for ( ; ; )
	{
		EXIT_DATA *pexit = NULL;
		ROOM_INDEX_DATA *pRoomIndex = NULL;
		char letter;
		int stat;

		if ( ( letter = fread_letter( fp ) ) == 'S' )
			break;

		if ( letter == '*' )
		{
			fread_to_eol( fp );
			continue;
		}

		pReset = alloc_mem( sizeof( *pReset ) );
		pReset->command = letter;
		fread_number( fp, &stat ); /* if_flag */ 
		pReset->arg1 = fread_number( fp, &stat );
		pReset->arg2 = fread_number( fp, &stat );
		pReset->arg3 = ( letter == 'G' || letter == 'R' ) ? 0 : fread_number( fp, &stat );
		fread_to_eol( fp );

		/*
		* Validate parameters.
		* We're calling the index functions for the side effect.
		*/
		switch ( letter )
		{
			default:
				bug( "Load_resets: bad command '%c'.", letter );
				exit( 1 );
				break;

			case 'M':
				get_mob_index ( pReset->arg1 );
				if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
				{
					new_reset( pRoomIndex, pReset );
					iLastRoom = pReset->arg3;
				}
				break;

			case 'O':
				get_obj_index ( pReset->arg1 );
				if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
				{
					new_reset( pRoomIndex, pReset );
					iLastObj = pReset->arg3;
				}
				break;

			case 'P':
				get_obj_index ( pReset->arg1 );
				if ( ( pRoomIndex = get_room_index ( iLastObj ) ) )
				{
					new_reset( pRoomIndex, pReset );
				}
				break;
			case 'F':
				get_mob_index ( abs( pReset->arg1 ) );
				get_mob_index ( abs( pReset->arg3 ) );
				if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
				{
					new_reset( pRoomIndex, pReset );
				}
				break;

			case 'G':
			case 'E':
				get_obj_index ( pReset->arg1 );
				if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
				{
					new_reset( pRoomIndex, pReset );
					iLastObj = iLastRoom;
				}
				break;

			case 'D':
				pRoomIndex = get_room_index( pReset->arg1 );

				if (   pReset->arg2 < 0
					|| pReset->arg2 > 5
					|| !pRoomIndex
					|| !( pexit = pRoomIndex->exit[pReset->arg2] )
					|| !IS_SET( pexit->rs_flags, EX_ISDOOR ) )
				{
					bug( "Load_resets: 'D': exit %d not door.", pReset->arg2 );
					if( pexit ) {
						char temp[MAX_STRING_LENGTH];
						bug( temp, 0);
					}
					exit( 1 );
				}

				switch ( pReset->arg3 ) /* OLC 1.1b */
				{
					default:
						bug( "Load_resets: 'D': bad 'locks': %d." , pReset->arg3);
					case 0:
						break;
					case 1: SET_BIT( pexit->rs_flags, EX_CLOSED );
						break;
					case 2: SET_BIT( pexit->rs_flags, EX_CLOSED | EX_LOCKED );
						break;
				}

				/* if ( pReset->arg3 < 0 || pReset->arg3 > 2 )
				{
				bug( "Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );
				exit( 1 );
				}

				break; */
			case 'R':
				/* pRoomIndex = get_room_index( pReset->arg1 ); */
				if ( pReset->arg2 < 0 || pReset->arg2 > 6 ) /* Last Door. */
				{
					bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );
					exit( 1 );
				}

				if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) )
					new_reset( pRoomIndex, pReset );

				break;
		}

		/*  if ( !area_last->reset_first )
		area_last->reset_first  = pReset;
		if (  area_last->reset_last  )
		area_last->reset_last->next = pReset;

		area_last->reset_last = pReset;
		pReset->next    = NULL;
		top_reset++;
		*/
	}

	return;
}



/*
 * Snarf a room section.
 */
void load_rooms( FILE *fp )
{
	ROOM_INDEX_DATA *pRoomIndex;

	if ( !area_last )
	{
		bug( "Load_resets: no #AREA seen yet.", 0 );
		exit( 1 );
	}

	for ( ; ; )
	{
		char letter;
		int  vnum;
		int  door;
		int  iHash;
		int  stat;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
			bug( "Load_rooms: # not found.", 0 );
			exit( 1 );
		}

		vnum = fread_number( fp, &stat );
		if ( vnum == 0 )
			break;

		fBootDb = FALSE;
		if ( get_room_index( vnum ) )
		{
			bug( "Load_rooms: vnum %d duplicated.", vnum );
			exit( 1 );
		}
		fBootDb = TRUE;

		pRoomIndex      = alloc_mem( sizeof( *pRoomIndex ) );
		pRoomIndex->people    = NULL;
		pRoomIndex->contents    = NULL;
		pRoomIndex->extra_descr   = NULL;
		pRoomIndex->area    = area_last;
		pRoomIndex->vnum    = vnum;
		pRoomIndex->name    = fread_string( fp, &stat );
		pRoomIndex->description   = fread_string( fp, &stat );
		/* Area number */     fread_number( fp, &stat );   /* Unused */
		pRoomIndex->room_flags    = fread_number( fp, &stat ) ;
		pRoomIndex->room_flags ^= area_last->f_toggle;
		pRoomIndex->sector_type   = fread_number( fp, &stat );
		pRoomIndex->light   = 0;
		pRoomIndex->max_char = 0; // 2022/12/17
		pRoomIndex->room_size = 0; // 2022/12/20
		pRoomIndex->room_space = 0; // 2022/12/20
		pRoomIndex->fall = 0;       // load room ©T©w±N fall ªº­È³]¬° 0 2023/01/14
		for ( door = 0; door <= 5; door++ )
			pRoomIndex->exit[door] = NULL;

		for ( ; ; )
		{
			letter = fread_letter( fp );

			if ( letter == 'S' || letter == 's' )
			{
				if ( letter == 's' )
					bug( "Load_rooms: vnum %d has lowercase 's'", vnum );
				break;
			}

			if ( letter == 'F' )
			{
				pRoomIndex->fall = fread_number( fp, &stat );
			}
			else if ( letter == 'D' )
			{
				EXIT_DATA *pexit;
				int    locks;

				door = fread_number( fp, &stat );
				if ( door < 0 || door > 9 )
				{
					bug( "Fread_rooms: vnum %d has bad door number.", vnum );
					exit( 1 );
				}

				pexit     = alloc_mem( sizeof( *pexit ) );
				pexit->description  = fread_string( fp, &stat );
				pexit->keyword    = fread_string( fp, &stat );
				pexit->exit_info  = 0;
				pexit->rs_flags   = 0;      /* OLC */
				pexit->exit_size = 0; // ªùªº¤j¤p 2022/12/22
				locks     = fread_number( fp, &stat );
				pexit->key    = fread_number( fp, &stat );
				pexit->vnum   = fread_number( fp, &stat );

				switch ( locks )  /* OLC exit_info to rs_flags. */
				{
					case 1:
						pexit->rs_flags = EX_ISDOOR; 
						break;
					case 2:
						pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF;
						break;
					case 3:
						pexit->rs_flags = EX_ISDOOR + EX_PASSPROOF;
						break;
					case 4:
						pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF + EX_PASSPROOF;
						break;
					case 5:
						pexit->rs_flags = EX_ISDOOR + EX_BASHPROOF;
						break;
					case 6:
						pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF + EX_BASHPROOF;
						break;
					case 7:
						pexit->rs_flags = EX_ISDOOR + EX_PASSPROOF + EX_BASHPROOF;
						break;
					case 8:
						pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF + EX_PASSPROOF + EX_BASHPROOF;
						break;
				}

				pRoomIndex->exit[door]   = pexit;
				top_exit++;
			}
			else if ( letter == 'E' )
			{
				EXTRA_DESCR_DATA *ed;

				ed      = alloc_mem( sizeof( *ed ) );
				ed->keyword   = fread_string( fp, &stat );
				ed->description   = fread_string( fp, &stat );
				ed->next    = pRoomIndex->extra_descr;
				pRoomIndex->extra_descr = ed;
				top_ed++;
			}
			else if ( letter == '>' )
			{
				ungetc( letter, fp );
				rprog_read_programs( fp, pRoomIndex );
			}
			else
			{
				bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
				exit( 1 );
			}
		}

		iHash     = vnum % MAX_KEY_HASH;
		pRoomIndex->next  = room_index_hash[iHash];
		room_index_hash[iHash]  = pRoomIndex;
		top_room++;
		top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room; /* OLC */
		assign_area_vnum( vnum );            /* OLC */
	}

	return;
}



/* OLC 1.1b */
void new_load_rooms( FILE *fp )
{
	ROOM_INDEX_DATA *pRoomIndex;

	if ( !area_last )
	{
		bug( "Load_rooms: no #AREA seen yet.", 0 );
		exit( 1 );
	}

	for ( ; ; )
	{
		char letter;
		int  vnum;
		int  door;
		int  iHash;
		int  stat;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
			bug( "Load_rooms: # not found.", 0 );
			exit( 1 );
		}

		vnum = fread_number( fp, &stat );
		if ( vnum == 0 )
			break;

		fBootDb = FALSE;
		if ( get_room_index( vnum ) )
		{
			bug( "Load_rooms: vnum %d duplicated.", vnum );
			exit( 1 );
		}
		fBootDb = TRUE;

		pRoomIndex = alloc_mem( sizeof( *pRoomIndex ) );
		pRoomIndex->people = NULL;
		pRoomIndex->contents = NULL;
		pRoomIndex->extra_descr = NULL;
		pRoomIndex->area = area_last;
		pRoomIndex->vnum = vnum;
		pRoomIndex->name = fread_string( fp, &stat );
		pRoomIndex->description = fread_string( fp, &stat );
		fread_number( fp, &stat ); /* Area number Unused */
		pRoomIndex->room_flags = fread_number( fp, &stat ) ;
		pRoomIndex->room_flags ^= area_last->f_toggle;
		pRoomIndex->sector_type = fread_number( fp, &stat );
		pRoomIndex->light = 0;
		pRoomIndex->max_char = 0; // 2022/12/17
		pRoomIndex->room_size = 0; // 2022/12/20
		pRoomIndex->room_space = 0; // 2022/12/20
		pRoomIndex->fall = 0;       // load room ©T©w±N fall ªº­È³]¬° 0 2023/01/14
		for ( door = 0; door <= 5; door++ )
			pRoomIndex->exit[door] = NULL;

		for ( ; ; )
		{
			int stat;
			letter = fread_letter( fp );

			if ( letter == 'S' || letter == 's' )
			{
				if ( letter == 's' )
					bug( "Load_rooms: vnum %d has lowercase 's'", vnum );
				break;
			}

			if ( letter == 'F' )
			{
				pRoomIndex->fall = fread_number( fp, &stat );
			}
			else if ( letter == 'D' )
			{
				EXIT_DATA *pexit;
				int locks;

				door = fread_number( fp, &stat );
				if ( door < 0 || door > 9 )
				{
					bug( "Fread_rooms: vnum %d has bad door number.", vnum );
					exit( 1 );
				}

				pexit = alloc_mem( sizeof( *pexit ) );
				pexit->description = fread_string( fp, &stat );
				pexit->keyword = fread_string( fp, &stat );
				locks = fread_number( fp, &stat );
				pexit->exit_info = locks;
				pexit->rs_flags = locks;
				pexit->exit_size = 0; // ªùªº¤j¤p 2022/12/22
				pexit->key = fread_number( fp, &stat );
				pexit->vnum = fread_number( fp, &stat );
				/*
					#ifdef 0
					switch ( locks )  // OLC exit_info to rs_flags. 
					{
					case 1: pexit->rs_flags = EX_ISDOOR;      break;
					case 2: pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF; break;
					case 3: pexit->rs_flags = EX_ISDOOR + EX_PASSPROOF; break;
					case 4: pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF + EX_PASSPROOF;     break;
					case 5: pexit->rs_flags = EX_ISDOOR + EX_BASHPROOF; break;
					case 6: pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF
					+ EX_BASHPROOF;     break;
					case 7: pexit->rs_flags = EX_ISDOOR + EX_PASSPROOF
					+ EX_BASHPROOF;     break;
					case 8: pexit->rs_flags = EX_ISDOOR + EX_PICKPROOF
					+ EX_PASSPROOF + EX_BASHPROOF;  break;
					}
					#endif
				*/

				pRoomIndex->exit[door] = pexit;
				++top_exit;
			}
			else if ( letter == 'E' )
			{
				EXTRA_DESCR_DATA *ed;

				ed = alloc_mem( sizeof( *ed ) );
				ed->keyword = fread_string( fp, &stat );
				ed->description = fread_string( fp, &stat );
				ed->next = pRoomIndex->extra_descr;
				pRoomIndex->extra_descr = ed;
				top_ed++;
			}
			else if ( letter == '>' )
			{
				ungetc( letter, fp );
				rprog_read_programs( fp, pRoomIndex );
			}
			else
			{
				bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
				exit( 1 );
			}
		}

		iHash = vnum % MAX_KEY_HASH;
		pRoomIndex->next  = room_index_hash[iHash];
		room_index_hash[iHash]  = pRoomIndex;
		top_room++;
		top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room;
		assign_area_vnum( vnum );
	}

	return;
}



/*
 * Snarf a shop section.
 */
void load_shops( FILE *fp )
{
    SHOP_DATA *pShop;
    int        keeper = 0;

    for ( ; ; )
    {
  MOB_INDEX_DATA *pMobIndex;
  int   iTrade;
  int   stat;

  keeper      = fread_number( fp, &stat );
  if ( keeper == 0 )
      break;
  pShop     = alloc_mem( sizeof( *pShop ) );
  pShop->keeper   = keeper;
  for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
      pShop->buy_type[iTrade] = fread_number( fp, &stat );
  pShop->profit_buy = fread_number( fp, &stat );
  pShop->profit_sell  = fread_number( fp, &stat );
  pShop->open_hour  = fread_number( fp, &stat );
  pShop->close_hour = fread_number( fp, &stat );
          fread_to_eol( fp );
  pMobIndex   = get_mob_index( pShop->keeper );
  pMobIndex->pShop  = pShop;

  if( pShop->profit_buy < 100 ){
      //bugf("Shop profit_buy < 100, keeper = %d", keeper);
      pShop->profit_buy = 120;
  }
  if( pShop->profit_sell > 100 ){
      //bugf("Shop profit_sell > 100, keeper = %d", keeper);
      pShop->profit_sell = 80;
  }
  if ( !shop_first )
      shop_first = pShop;
  if (  shop_last  )
      shop_last->next = pShop;

  shop_last = pShop;
  pShop->next = NULL;
  top_shop++;
    }

    return;
}



/*
 * Snarf spec proc declarations.
 */
void load_specials( FILE *fp )
{
    for ( ; ; )
    {
  MOB_INDEX_DATA *pMobIndex;
  char    letter;
  int   stat;

  switch ( letter = fread_letter( fp ) )
  {
  default:
      bug( "Load_specials: letter '%c' not *MS.", letter );
      exit( 1 );

  case 'S':
      return;

  case '*':
      break;

  case 'M':
      pMobIndex   = get_mob_index ( fread_number ( fp, &stat ) );
      pMobIndex->spec_fun = spec_lookup ( fread_word   ( fp, &stat ) );
      if ( pMobIndex->spec_fun == 0 )
      {
    bug( "Load_specials: 'M': vnum %d.", pMobIndex->vnum );
    exit( 1 );
      }
      break;
  }

  fread_to_eol( fp );
    }
}



/*
 * Snarf notes file.
 */
void load_notes( void )
{
    FILE      *fp;
    NOTE_DATA *pnotelast;

    if ( !( fp = fopen( NOTE_FILE, "r" ) ) )
  return;

    pnotelast = NULL;
    for ( ; ; )
    {
  NOTE_DATA *pnote;
  char     letter;
  int    stat;

  do
  {
      letter = getc( fp );
      if ( feof(fp) )
      {
      fflush( NULL );
      fclose( fp );
    return;
      }
  }
  while ( isspace( letter ) );
  ungetc( letter, fp );

  pnote     = alloc_mem( sizeof( *pnote ) );

  if ( str_cmp( fread_word( fp, &stat ), "sender" ) )
      break;
  pnote->sender   = fread_string( fp, &stat );

  if ( str_cmp( fread_word( fp, &stat ), "csender" ) )
      break;
  pnote->csender    = fread_string( fp, &stat );

  if ( str_cmp( fread_word( fp, &stat ), "date" ) )
      break;
  pnote->date   = fread_string( fp, &stat );

  if ( str_cmp( fread_word( fp, &stat ), "stamp" ) )
      break;
  pnote->date_stamp = fread_number( fp, &stat );

  if ( str_cmp( fread_word( fp, &stat ), "to" ) )
      break;
  pnote->to_list    = fread_string( fp, &stat );

  if ( str_cmp( fread_word( fp, &stat ), "subject" ) )
      break;
  pnote->subject    = fread_string( fp, &stat );

  if ( str_cmp( fread_word( fp, &stat ), "text" ) )
      break;
  pnote->text   = fread_string( fp, &stat );

  if ( !note_list )
      note_list   = pnote;
  else
      pnotelast->next = pnote;

  pnotelast   = pnote;
    }

    strncpy( strArea, NOTE_FILE ,MAX_INPUT_LENGTH);
    fpArea = fp;
    bug( "Load_notes: bad key word.", 0 );
    exit( 1 );
    return;
}


void load_down_time( void )
{
	
	struct tm *check_time;
	
  //FILE *fp;
  //int   number = 0;
  
  //down_time = 0; //boot_time + 86400;
  //warning1  = 0;
  //warning2  = 0;
  //Reboot = FALSE;
  Reboot    = TRUE;
  /* Keric modified */
  if( !current_time ) {
    struct timeval         last_time;
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;
  }
  
  check_time = localtime(&current_time);
  check_time->tm_sec = 59 ;
  check_time->tm_min = 59 ;
  check_time->tm_hour = 23 ;

  if( check_time->tm_mon == 1
      || check_time->tm_mon == 3
      || check_time->tm_mon == 5
      || check_time->tm_mon == 7
      || check_time->tm_mon == 8
      || check_time->tm_mon == 10)
  {
  	  if( check_time->tm_mday == 31)
  	  {
  	  	  check_time->tm_mday = 1;
  	  	  check_time->tm_mon += 1;
  	  }
  	  else
  	  {
  	  	  check_time->tm_mday += 1 ;
  	  }
  }
  else if( check_time->tm_mon == 2 )
  {
  	  if( check_time->tm_yday == 365 )
  	  {
  	      if( check_time->tm_mday == 29)
  	      {
  	      	  check_time->tm_mday = 1;
  	      	  check_time->tm_mon += 1;
  	      }
  	      else
  	      {
  	      	  check_time->tm_mday += 1 ;
  	      }
  	  }
  	  else
  	  {
  	      if( check_time->tm_mday == 28)
  	      {
  	      	  check_time->tm_mday = 1;
  	      	  check_time->tm_mon += 1;
  	      }
  	      else
  	      {
  	      	  check_time->tm_mday += 1 ;
  	      }
  	  }
  }
  else if( check_time->tm_mon == 12 )
  {
  	  if( check_time->tm_mday == 31)
  	  {
  	  	  check_time->tm_mday = 1;
  	  	  check_time->tm_mon = 0;
  	  	  check_time->tm_year += 1;
  	  }
  	  else
  	  {
  	  	  check_time->tm_mday += 1 ;
  	  }
  }
  else
  {
  	  if( check_time->tm_mday == 30)
  	  {
  	  	  check_time->tm_mday = 1;
  	  	  check_time->tm_mon = 1;
  	  	  check_time->tm_year += 1;
  	  }
  	  else
  	  {
  	  	  check_time->tm_mday += 1 ;
  	  }
  }
  
  down_time = mktime(check_time);
 
  //down_time = current_time + 86400 ;
  warning2   = down_time - 150;
  warning1   = warning2  - 150;
  warning0   = warning2 - 1800;
  /* /Keric modified */

  //if ( !( fp = fopen( DOWN_TIME_FILE, "r" ) ) )
  //  return;

  /*for ( ; ; )
  {
    char *word;
    char  letter;
    int   stat;

    do
    {
      letter = getc( fp );
      if ( feof( fp ) )
      {
        fflush( fp );
        fclose( fp );
        if ( down_time > 0 && down_time < 301 )
        {
          down_time  = current_time + 300;
          warning2   = down_time - 150;
          warning1   = warning2  - 75;
          warning0   = warning2 - 1800;
        }
        else 
        {
          if ( down_time > 0 )
          {
            down_time += current_time;
            warning2   = down_time - 150;
            warning1   = warning2  - 150;
            warning0   = warning2 - 1800;
          }
        }
        return;
      }
    }
    while ( isspace( letter ) );
    ungetc( letter, fp );

    word = fread_word( fp, &stat );

    if ( !str_cmp( word, "HOUR" ) )
    {
      number = 24; //fread_number( fp, &stat );
      if ( number > 0 )
        down_time += (time_t) ( number * 3600 );
    }
    if ( !str_cmp( word, "MINUTE" ) )
    {
      number = fread_number( fp, &stat );
      if ( number > 0 )
        down_time += (time_t) ( number * 60 );
    }
    if ( !str_cmp( word, "REBOOT" ) )
    {
      Reboot = TRUE;
    }
  }*/
}

/*
 * Load up the ban file
 */
void load_ban( void )
{
    BAN_DATA  *pban;
    FILE      *fp;

    if ( !( fp = fopen( BAN_FILE, "r" ) ) )
      return;

    for ( ; ; )
    {
  char   letter;
  int    stat;

  do
  {
      letter = getc( fp );
      if ( feof( fp ) )
      {
      fflush( fp );
      fclose( fp );
    return;
      }
  }
  while ( isspace( letter ) );
  ungetc( letter, fp );

  if ( ban_free == NULL )
  {
      pban     = alloc_mem( sizeof( *pban ) );
  }
  else
  {
      pban     = ban_free;
      ban_free = ban_free->next;
  }

  pban->name   = fread_string( fp, &stat );

  pban->next   = ban_list;
  ban_list     = pban;

    }

}

/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad or suspicious reverse exits.
 */
void fix_exits( void )
{
     EXIT_DATA   *pexit;
     EXIT_DATA   *pexit_rev;
     ROOM_INDEX_DATA *pRoomIndex;
     ROOM_INDEX_DATA *to_room;
     char     buf   [ MAX_STRING_LENGTH ];
    extern const int      rev_dir [ ];
     int      iHash;
     int      door;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
  for ( pRoomIndex  = room_index_hash[iHash];
        pRoomIndex;
        pRoomIndex  = pRoomIndex->next )
  {
      bool fexit;

      fexit = FALSE;
      for ( door = 0; door <= 5; door++ )
      {
    if ( ( pexit = pRoomIndex->exit[door] ) )
    {
        fexit = TRUE;
        if ( pexit->vnum <= 0 )
      pexit->to_room = NULL;
        else
      pexit->to_room = get_room_index( pexit->vnum );
    }
      }

      if ( !fexit )
    SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );
  }
    }

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
  for ( pRoomIndex  = room_index_hash[iHash];
        pRoomIndex;
        pRoomIndex  = pRoomIndex->next )
  {
      for ( door = 0; door <= 5; door++ )
      {
    if (   ( pexit     = pRoomIndex->exit[door]   )
        && ( to_room   = pexit->to_room     )
        && ( pexit_rev = to_room->exit[rev_dir[door]] )
        &&   pexit_rev->to_room != pRoomIndex )
    {
        sprintf( buf, "Fix_exits: %d:%d -> %d:%d -> %d.",
          pRoomIndex->vnum, door,
          to_room->vnum,    rev_dir[door],
          ( !pexit_rev->to_room ) ? 0
          :  pexit_rev->to_room->vnum );
        //bug( buf, 0 );
    }
      }
  }
    }

    return;
}



/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
      CHAR_DATA *pch;

      if ( ++pArea->age < 3 )
          continue;

      /*
       * Check for PC's.
       */
      if ( pArea->reborn != 0)
      {
        if ( pArea->nplayer > 0 && pArea->age == pArea->reborn -1)
        {
            for ( pch = char_list; pch; pch = pch->next )
            {
                  if ( !IS_NPC( pch )
                      && IS_AWAKE( pch )
                      && pch->in_room
                      && pch->in_room->area == pArea )
                  {
                      send_to_char( "§AÅ¥¨ì»´·Lªº¸}¨BÁn.\n\r", pch );
                  }
              }
          }
        if ( pArea->nplayer == 0 || pArea->age >= pArea->reborn )
        {
           //ROOM_INDEX_DATA *pRoomIndex;
      
           reset_area( pArea );
           pArea->age = 0;
        }         
      } 
      else
      { 
        if ( pArea->nplayer > 0 && pArea->age == 15 - 1 )
        {
            for ( pch = char_list; pch; pch = pch->next )
            {
          if ( !IS_NPC( pch )
              && IS_AWAKE( pch )
              && pch->in_room
              && pch->in_room->area == pArea )
          {
              send_to_char( "§AÅ¥¨ì»´·Lªº¸}¨BÁn.\n\r",
            pch );
          }
           }
        }

        /*
         * Check age and reset.
         * Note: Mud School resets every 3 minutes (not 15).
         */
        if ( pArea->nplayer == 0 || pArea->age >= 15 )
        {
           ROOM_INDEX_DATA *pRoomIndex;
      
           reset_area( pArea );
           pArea->age = number_range( 0, 3 );
           pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
           if ( pRoomIndex && pArea == pRoomIndex->area )
          pArea->age = 15 - 3;
        }
      }
    }

    return;
}

//use this to create eqs from lv31 - 37
int number_fuzzy2( int number )
{
    switch ( number_bits( 3 ) )
    {
    case 0:  number -= 5; break;
    case 1:  number -= 4; break;
    case 2:  number -= 3; break;
    case 3:  number -= 2; break;
    case 4:  number -= 1; break;
    case 5:  number += 1; break;
    }

    return UMAX( 1, number );
}

/* OLC
 * Reset one room.  Called by reset_area and olc.
 */
void reset_room( ROOM_INDEX_DATA *pRoom )
{
    RESET_DATA  *pReset;
    CHAR_DATA *pMob;
    OBJ_DATA  *pObj;
    EXIT_DATA *pexit;
    CHAR_DATA *LastMob = NULL;
    OBJ_DATA  *LastObj = NULL;
    int iExit;
    int level = 0;
    bool last;

    if ( !pRoom )
  return;

    pMob  = NULL;
    last  = FALSE;

    for ( iExit = 0;  iExit < MAX_DIR;  iExit++ )
    {
  EXIT_DATA *pExit;
  if ( ( pExit = pRoom->exit[iExit] ) )
//    && !IS_SET( pExit->exit_info, EX_BASHED ) ) /* Skip Bashed. */
  {
      pExit->exit_info = pExit->rs_flags;
      if ( ( pExit->to_room != NULL )
        && ( ( pExit = pExit->to_room->exit[rev_dir[iExit]] ) ) )
      {
    /* nail the other side */
    pExit->exit_info = pExit->rs_flags;
      }
  }
    }

    for ( pReset = pRoom->reset_first; pReset != NULL; pReset = pReset->next )
    {
  MOB_INDEX_DATA  *pMobIndex;
  OBJ_INDEX_DATA  *pObjIndex;
  OBJ_INDEX_DATA  *pObjToIndex;
  ROOM_INDEX_DATA *pRoomIndex;
  CHAR_DATA   *lch;
  CHAR_DATA   *vch;

  switch ( pReset->command )
  {
  default:
    bug( "Reset_room: bad command %c.", pReset->command );
    break;

  case 'M':
      if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
      {
    bug( "Reset_room: 'M': bad vnum %d.", pReset->arg1 );
    continue;
      }

      /*
       * Some hard coding.
       */
      if ( ( pMobIndex->spec_fun == spec_lookup( "spec_cast_ghost" ) &&
     ( weather_info.sunlight != SUN_DARK ) ) ) continue;

      if ( pMobIndex->count >= pReset->arg2 )
      {
    last = FALSE;
    break;
      }

      pMob = create_mobile( pMobIndex );

      /*
       * Some more hard coding.
       */
      if ( room_is_dark( pRoom ) )
    SET_BIT(pMob->affected_by, AFF_INFRARED);

      /*
       * Pet shop mobiles get ACT_PET set.
       */
      {
    ROOM_INDEX_DATA *pRoomIndexPrev;

    pRoomIndexPrev = get_room_index( pRoom->vnum - 1 );
    if ( pRoomIndexPrev
        && IS_SET( pRoomIndexPrev->room_flags, ROOM_PET_SHOP ) )
        SET_BIT( pMob->act, ACT_PET);
      }

      char_to_room( pMob, pRoom );
      mprog_repop_trigger(pMob);

      LastMob = pMob;
      level  = URANGE( 1, pMob->level - 2, 41 );
      last = TRUE;
      break;

  case 'O':
      if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
      {
    bug( "Reset_room: 'O': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
      {
    bug( "Reset_room: 'O': bad vnum %d.", pReset->arg3 );
    continue;
      }

      if ( pRoom->area->nplayer > 0
        || count_obj_list( pObjIndex, pRoom->contents ) > 0 )
    break;

    if ( pRoom->area->clan && pRoom->area->clan->construct < 100 )
      break;

      pObj = create_object( pObjIndex, number_fuzzy( level ) );
      LastObj = pObj;
            if( IS_SET(pObj->extra_flags, ITEM_CLAN)){
                REMOVE_BIT(pObj->wear_flags, ITEM_TAKE);
                SET_BIT(pObj->extra_flags, ITEM_NOSAC);
            }
      pObj->cost = 0;
    if ( pRoom->area->clan )
      switch( pObj->item_type )
      {
        case ITEM_ARMOR : pRoom->area->clan->construct -=3; break;
        case ITEM_WEAPON : pRoom->area->clan->construct -=8; break;
        default : pRoom->area->clan->construct--; break;
      }
      obj_to_room( pObj, pRoom );
      break;

  case 'P':
      if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
      {
    bug( "Reset_room: 'P': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
      {
    bug( "Reset_room: 'P': bad vnum %d.", pReset->arg3 );
    continue;
      }


      if ( pRoom->area->nplayer > 0
      //  || !( LastObj = get_obj_type( pObjToIndex ) )
        || ( !LastObj && !( LastObj = get_obj_type( pObjToIndex )))
        ||( LastObj->in_room != pRoom ) 
        || ( LastObj->carried_by && !IS_NPC(LastObj->carried_by))
        || count_obj_list( pObjIndex, LastObj->contains ) > 0 )
    break;

    if ( pRoom->area->clan && pRoom->area->clan->construct < 100 )
      break;

      pObj = create_object( pObjIndex, number_fuzzy( level ) );
    if ( pRoom->area->clan )
      switch( pObj->item_type )
      {
        case ITEM_ARMOR : pRoom->area->clan->construct -=3; break;
        case ITEM_WEAPON : pRoom->area->clan->construct -=8; break;
        default : pRoom->area->clan->construct--; break;
      }
      obj_to_obj( pObj, LastObj );

      /*
             * Ensure that the container gets reset.  OLC 1.1b
             */
            if ( LastObj->item_type == ITEM_CONTAINER )
            {
                LastObj->value[1] = LastObj->pIndexData->value[1];
            }
            else
            {
                /* THIS SPACE INTENTIONALLY LEFT BLANK */
            }
            break;

        case 'G':
        case 'E':
            if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
            {
                bug( "Reset_room: 'E' or 'G': bad vnum %d.", pReset->arg1 );
                continue;
            }

            if ( !last )
                break;

            if ( !LastMob )
            {
                bug( "Reset_room: 'E' or 'G': null mob for vnum %d.",
                        pReset->arg1 );
                last = FALSE;
                break;
            }

            if ( pRoom->area->clan && pRoom->area->clan->construct < 100 )
                break;

            if ( LastMob->pIndexData->pShop ) /* Shop-keeper? */
            {
                int olevel;

                switch ( pObjIndex->item_type )
                {
                    default:    olevel = 0;      break;
                    case ITEM_PILL:   olevel = number_range(  0, 10 ); break;
                    case ITEM_POTION: olevel = number_range(  0, 10 ); break;
                    case ITEM_SCROLL: olevel = number_range(  5, 15 ); break;
                    case ITEM_WAND:   olevel = number_range( 10, 20 ); break;
                    case ITEM_STAFF:  olevel = number_range( 15, 25 ); break;
                    case ITEM_ARMOR:  olevel = number_range(  5, 15 ); break;
                    case ITEM_WEAPON: 
                                        if( pObjIndex->level )
                                            olevel = pObjIndex->level;
                                        else if ( pReset->command == 'G' )
                                            olevel = number_range( 5, 15 );
                                        else
                                            olevel = number_fuzzy( level );
                                        break;
                }

                pObj = create_object( pObjIndex, olevel );
                if ( pReset->command == 'G' )
                    SET_BIT( pObj->extra_flags, ITEM_INVENTORY );
            }
            else
            {
                //if ( level > LEVEL_HERO )
                 //   pObj = create_object( pObjIndex, number_fuzzy2( 37 ) );
                //else
                //¹ïlevel°µ­×¥¿¡A¨Sexpertªºmob eqµ¥¯Å½Õ¾ã
                if( !IS_SET( LastMob->special,EXPERT ) ) {
                    if( level > 29 && level < 38) {
                        level = UMIN(level, 35);
                        level = number_range( 25,level); // mid-level mob
                    }
                }
                pObj = create_object( pObjIndex, number_fuzzy( level ) );
            }
            if ( pRoom->area->clan ){
                switch( pObj->item_type )
                {
                    case ITEM_ARMOR : pRoom->area->clan->construct -=3; break;
                    case ITEM_WEAPON : pRoom->area->clan->construct -=8; break;
                    default : pRoom->area->clan->construct--; break;
                }
            }

            obj_to_char( pObj, LastMob );

            if( IS_SET(LastMob->act, ACT_CLAN)){
                SET_BIT( pObj->extra_flags, ITEM_INVENTORY );
            }

            if ( pReset->command == 'E' )
                equip_char( LastMob, pObj, pReset->arg3 );

            LastObj = pObj;
            last = TRUE;
            break;

  case 'F':
      if ( !( pMobIndex = get_mob_index( abs( pReset->arg1 ) ) ) )
      {
    bug( "Reset_area: 'F': bad vnum %d.", pReset->arg1 );
    continue;
      }
      for( lch = char_list; lch; lch = lch->next )
      if ( lch->pIndexData == pMobIndex )
      break;
      if ( !( pMobIndex = get_mob_index( abs( pReset->arg3 ) ) ) )
      {
    bug( "Reset_area: 'F': bad vnum %d.", pReset->arg3 );
    continue;
      }
      for( vch = char_list; vch; vch = vch->next )
      if ( vch->pIndexData == pMobIndex )
      break;
     if( !lch || !vch)
    continue;

      if ( vch->master )
    continue;

      add_follower( vch, lch );
            if( !lch->group ) {
                do_group(lch, "form");
            }
            //victim->leader = ch;
            add_member(lch, vch);
      //vch->leader = lch;
      
      if ( pReset->arg2 != 0 )
      {
        /*vch->protecting = lch;
    vch->next_in_protect = lch->protector;
        lch->protector = vch;*/
    add_protector(vch, lch);
      }
      break;


  case 'D':
      if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'D': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pexit = pRoomIndex->exit[pReset->arg2] ) )
    break;

       /* Bashed doors persist across resets */ 
      if ( !IS_SET( pexit->exit_info, EX_BASHED ) )
    switch ( pReset->arg3 )
    {
    case 0:
        REMOVE_BIT( pexit->exit_info, EX_CLOSED );
        REMOVE_BIT( pexit->exit_info, EX_LOCKED );
        break;

    case 1:
        SET_BIT(  pexit->exit_info, EX_CLOSED );
        REMOVE_BIT( pexit->exit_info, EX_LOCKED );
        break;

    case 2:
        SET_BIT(  pexit->exit_info, EX_CLOSED );
        SET_BIT(  pexit->exit_info, EX_LOCKED );
        break;
    }

      last = TRUE;
      break;
  case 'R':
///* OLC 1.1b
      if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
      {
    bug( "Reset_room: 'R': bad vnum %d.", pReset->arg1 );
    continue;
      }

      {
    EXIT_DATA *pExit;
    int d0;
    int d1;

    for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
    {
        d1       = number_range( d0, pReset->arg2-1 );
        pExit    = pRoomIndex->exit[d0];
        pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
        pRoomIndex->exit[d1] = pExit;
    }
      }
//*/
      break;
  }
    }

    return;
}



/*
 * Reset one area.
 */
/*
void reset_area( AREA_DATA *pArea )
{
    CHAR_DATA  *mob;
    RESET_DATA *pReset;
    int   level;
    bool  last;

    mob = NULL;
    last  = TRUE;
    level = 0;
    for ( pReset = pArea->reset_first; pReset; pReset = pReset->next )
    {
  OBJ_DATA  *obj;
  OBJ_DATA  *obj_to;
  EXIT_DATA *pexit;
  MOB_INDEX_DATA  *pMobIndex;
  OBJ_INDEX_DATA  *pObjIndex;
  OBJ_INDEX_DATA  *pObjToIndex;
  ROOM_INDEX_DATA *pRoomIndex;
  CHAR_DATA *lch, *vch;

  switch ( pReset->command )
  {
  default:
      bug( "Reset_area: bad command %c.", pReset->command );
      break;

  case 'M':
      if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'M': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
      {
    bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
    continue;
      }

      if ( ( pMobIndex->spec_fun == spec_lookup( "spec_cast_ghost" )
      && ( weather_info.sunlight != SUN_DARK ) ) )
      {
    last = FALSE;
    continue;
      }

      level = URANGE( 0, pMobIndex->level - 2, LEVEL_HERO );

      */ /* If sentinel, then maximum number indicates in room instead
         of in world. -Kahn */ /*
      */ /* Midgaard mayor is special case as it has a spec proc which
         moves it.  Dislike such specific coding.  Shrug.  -Kahn */ /*
      if ( IS_SET( pMobIndex->act, ACT_SENTINEL )
    && pMobIndex->vnum != MOB_VNUM_MIDGAARD_MAYOR )
      {
    CHAR_DATA *ch;
    int    count = 0;
    
    for ( ch = pRoomIndex->people; ch; ch = ch->next_in_room )
        if ( IS_NPC( ch ) && ch->pIndexData == pMobIndex )
      count++;
    if ( count >= pReset->arg2 )
    {
        last = FALSE;
        break;
    }
      }
      else
    if ( pMobIndex->count >= pReset->arg2 )
    {
        last = FALSE;
        break;
    }

      mob = create_mobile( pMobIndex );

      */ /*
       * Check for pet shop.
       */ /*
      {
    ROOM_INDEX_DATA *pRoomIndexPrev;

    pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );
    if ( pRoomIndexPrev
        && IS_SET( pRoomIndexPrev->room_flags, ROOM_PET_SHOP ) )
        SET_BIT( mob->act, ACT_PET);
      }

      if ( room_is_dark( pRoomIndex ) )
    SET_BIT( mob->affected_by, AFF_INFRARED );

      char_to_room( mob, pRoomIndex );
      level = URANGE( 0, mob->level - 2, LEVEL_HERO );
      last  = TRUE;
      break;

  case 'O':
      if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'O': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
      {
    bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
    continue;
      }

      if ( pArea->nplayer > 0
    || count_obj_list( pObjIndex, pRoomIndex->contents ) > 0 )
      {
    last = FALSE;
    break;
      }

      obj       = create_object( pObjIndex, number_fuzzy( level ) );
      obj->cost = 0;
      obj_to_room( obj, pRoomIndex );
      last = TRUE;
      break;

  case 'P':
      if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'P': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
      {
    bug( "Reset_area: 'P': bad vnum %d.", pReset->arg3 );
    continue;
      }

      if ( pArea->nplayer > 0
    || !( obj_to = get_obj_type( pObjToIndex ) )
    || count_obj_list( pObjIndex, obj_to->contains ) > 0 )
      {
    last = FALSE;
    break;
      }
      
      obj = create_object( pObjIndex, number_fuzzy( obj_to->level ) );
      obj_to_obj( obj, obj_to );
      last = TRUE;
      break;

  case 'G':
  case 'E':
      if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'E' or 'G': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !last )
    break;

      if ( !mob )
      {
    bug( "Reset_area: 'E' or 'G': null mob for vnum %d.",
        pReset->arg1 );
    last = FALSE;
    break;
      }

      if ( mob->pIndexData->pShop )
      {
    int olevel;

    switch ( pObjIndex->item_type )
    {
    default:    olevel = 0;      break;
    case ITEM_PILL:   olevel = number_range(  0, 10 ); break;
    case ITEM_POTION: olevel = number_range(  0, 10 ); break;
    case ITEM_SCROLL: olevel = number_range(  5, 15 ); break;
    case ITEM_WAND:   olevel = number_range( 10, 20 ); break;
    case ITEM_STAFF:  olevel = number_range( 15, 25 ); break;
    case ITEM_ARMOR:  olevel = number_range(  5, 15 ); break;
    case ITEM_FURNITURE:  olevel = number_range(  5, 15 ); break;
    case ITEM_WEAPON: if ( pReset->command == 'G' )
              olevel = number_range( 5, 15 );
          else
              olevel = number_fuzzy( level );
      break;
    }

    obj = create_object( pObjIndex, olevel );
    if ( pReset->command == 'G' )
        SET_BIT( obj->extra_flags, ITEM_INVENTORY );
      }
      else
      {
    obj = create_object( pObjIndex, number_fuzzy( level ) );
      }
      obj_to_char( obj, mob );
      if ( pReset->command == 'E' )
    equip_char( mob, obj, pReset->arg3 );
      last = TRUE;
      break;

  case 'F':
      if ( !( pMobIndex = get_mob_index( abs( pReset->arg1 ) ) ) )
      {
    bug( "Reset_area: 'F': bad vnum %d.", pReset->arg1 );
    continue;
      }
      for( lch = char_list; lch; lch = lch->next )
      if ( lch->pIndexData == pMobIndex )
      break;
      if ( !( pMobIndex = get_mob_index( abs( pReset->arg3 ) ) ) )
      {
    bug( "Reset_area: 'F': bad vnum %d.", pReset->arg3 );
    continue;
      }
      for( vch = char_list; vch; vch = vch->next )
      if ( vch->pIndexData == pMobIndex )
      break;
     if( !lch || !vch)
    continue;

      if ( vch->master )
    continue;

      add_follower( vch, lch );
      vch->leader = lch;
      break;

  case 'D':
      if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'D': bad vnum %d.", pReset->arg1 );
    continue;
      }

      if ( !( pexit = pRoomIndex->exit[pReset->arg2] ) )
    break;

      */ /* Bashed doors persist across resets */ /*
      if ( !IS_SET( pexit->exit_info, EX_BASHED ) )
    switch ( pReset->arg3 )
    {
    case 0:
        REMOVE_BIT( pexit->exit_info, EX_CLOSED );
        REMOVE_BIT( pexit->exit_info, EX_LOCKED );
        break;

    case 1:
        SET_BIT(  pexit->exit_info, EX_CLOSED );
        REMOVE_BIT( pexit->exit_info, EX_LOCKED );
        break;

    case 2:
        SET_BIT(  pexit->exit_info, EX_CLOSED );
        SET_BIT(  pexit->exit_info, EX_LOCKED );
        break;
    }

      last = TRUE;
      break;

  case 'R':
      if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
      {
    bug( "Reset_area: 'R': bad vnum %d.", pReset->arg1 );
    continue;
      }

      {
    int d0;
    int d1;

    for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
    {
        d1       = number_range( d0, pReset->arg2-1 );
        pexit    = pRoomIndex->exit[d0];
        pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
        pRoomIndex->exit[d1] = pexit;
    }
      }
      break;
  }
    }

    return;
}
*/
/* OLC
 * Reset one area.
 */
void reset_area( AREA_DATA *pArea )
{
    ROOM_INDEX_DATA *pRoom;
    int  vnum;

    for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
  if ( ( pRoom = get_room_index(vnum) ) )
      reset_room(pRoom);
    }

    return;
}



/*
 * Take a character data from the free list and clean it out.
 */
CHAR_DATA *new_character( bool player )
{
	static PC_DATA pcdata_zero;
	CHAR_DATA *ch;
	int i;

	if ( !char_free )
	{
		++top_ch;
		ch = alloc_mem( sizeof( CHAR_DATA ) );
	}
	else
	{
		ch = char_free;
		char_free = char_free->next;
	}

	clear_char( ch );

	if ( player )
	{
		if ( !pcdata_free )
		{
			ch->pcdata = alloc_mem( sizeof( *ch->pcdata ) );
		}
		else
		{
			ch->pcdata = pcdata_free;
			pcdata_free = pcdata_free->next;
		}

		*ch->pcdata = pcdata_zero;
	}
	for( i=0; i< MAX_SAVING; ++i )
		ch->saving_throw[i] = 0;


	ch->deleted			=   FALSE;
	ch->mdr				=   0;
	ch->dodge			=   0;
	ch->pdr				=   0;
	ch->pp				=   0;
	ch->protector		=   NULL;
	ch->protecting		=   NULL;
	ch->next_in_protect	=   NULL;
	ch->tracking		=   NULL;
	ch->tracked			=   NULL;
	ch->next_in_track	=   NULL;
	ch->hunting			=   NULL;
	ch->hunted			=   NULL;
	ch->next_in_hunt	=   NULL;
	ch->rembch			=   NULL;
	ch->rembed			=   NULL;
	ch->rrembed			=   NULL;
	ch->next_in_rember	=   NULL;
	ch->assassinating	=   NULL;
	ch->assassinated	=   NULL;
	ch->next_in_ass		=   NULL;
	ch->perm_str		=   0;
	ch->perm_dex		=   0;
	ch->perm_int		=   0;
	ch->perm_wis		=   0;
	ch->perm_con		=   0;
	ch->pad				=   0;
	ch->mad				=   0;
	ch->amdr			=   0;
	ch->apdr			=   0;
	ch->mod_str			=   0;
	ch->mod_dex			=   0;
	ch->mod_int			=   0;
	ch->mod_wis			=   0;
	ch->mod_con			=   0;
	ch->mod_size		=   0; // Åé«¬ 2022/12/23
	ch->clan			=   NULL;
	ch->group			=   NULL;

    return ch;
}

/*
AFFECT_DATA *new_affect()
{
    AFFECT_DATA *paf;

    if ( !affect_free )
  return alloc_mem( sizeof( AFFECT_DATA ) );

    paf   = affect_free;
    affect_free = affect_free->next;

    return paf;
}
*/

OBJ_DATA *new_object()
{
    OBJ_DATA *obj;

    if ( !obj_free )
    {
  ++top_obj;
  obj  = alloc_mem( sizeof( *obj ) );
    }
    else
    {
  obj  = obj_free;
  obj_free = obj_free->next;
    }

    memset( obj, 0, sizeof( OBJ_DATA ) );

    obj->deleted = FALSE;
    obj->material = NULL;
    obj->vrace = NULL;
    obj->next = NULL;
    obj->next_content = NULL;
    obj->in_eqbank = FALSE;
    obj->in_room = NULL;
    obj->carried_by = NULL;
    obj->contains = NULL;
    obj->in_obj = NULL;
    obj->extra_descr = NULL;
    obj->affected = NULL;
    obj->pIndexData = NULL;
    obj->hp = NULL;
    obj->name = NULL;
    obj->short_descr = NULL;
    obj->description = NULL;
    obj->orembch = NULL;
    obj->next_in_orember = NULL;
    obj->value[0] = 0;
    obj->value[1] = 0;
    obj->value[2] = 0;
    obj->value[3] = 0;
    obj->value[4] = 0;
    ( obj->extra_value ).used = FALSE;   //Added by Razgriz 20050918
    ( obj->extra_value ).v[0] = 0;       //Added by Razgriz 20050918
    ( obj->extra_value ).v[1] = 0;       //Added by Razgriz 20050918
    ( obj->extra_value ).t[0] = -1;      //Added by Razgriz 20050918
    ( obj->extra_value ).t[1] = -1;      //Added by Razgriz 20050918

    return obj;
}

/*
EXTRA_DESCR_DATA *new_extra_descr()
{
    EXTRA_DESCR_DATA *ed;

    if ( !extra_descr_free )
  return alloc_mem( sizeof( EXTRA_DESCR_DATA ) );
    
    ed         = extra_descr_free;
    extra_descr_free = extra_descr_free->next;

    return ed;

}
*/
  


/*
 * Create an instance of a mobile.
 */
MOB_ACTION *mob_action_list;
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )
{
	CHAR_DATA *mob;
	int i;
	int value;
	MOB_ACTION *mob_action, *pmob_action;

	if ( !pMobIndex )
	{
		bug( "Create_mobile: NULL pMobIndex.", 0 );
		exit( 1 );
	}

	mob     = new_character( FALSE );

	mob->pIndexData = pMobIndex;

	mob->name   = str_dup( pMobIndex->player_name );
	mob->short_descr  = str_dup( pMobIndex->short_descr );
/*    mob->cname    = str_dup( pMobIndex->short_descr );*/
	mob->long_descr = str_dup( pMobIndex->long_descr  );
	mob->description  = str_dup( pMobIndex->description );
	mob->spec_fun = pMobIndex->spec_fun;

	mob->level    = number_fuzzy( pMobIndex->level );
	mob->act    = pMobIndex->act;
	mob->affected_by  = pMobIndex->affected_by;
	mob->alignment  = pMobIndex->alignment;
	mob->sex    = pMobIndex->sex;
	mob->race   = pMobIndex->race;
	mob->exp            = pMobIndex->mod_exp;

	mob->protecting = NULL;
	mob->special  = pMobIndex->special;
	mob->protector = NULL;
	mob->next_in_protect = NULL;
	mob->next = NULL;
	mob->rembch = NULL;
	mob->rembed = NULL;
	mob->next_in_rember = NULL;
	mob->rrembed = NULL;
	mob->clan = pMobIndex->area->clan;
	mob->weakness = pMobIndex->weakness;
	mob->pdr = pMobIndex->pdr;
	mob->mdr = pMobIndex->mdr;
	mob->pad = pMobIndex->pad;
	mob->mad = pMobIndex->mad;
	mob->apdr = pMobIndex->apdr;
	mob->amdr = pMobIndex->amdr;
	mob->track_times = 0;
	mob->attacked = NULL;
	if( IS_SET(mob->special, EXPERT ) ){
		value = 13 + race_table[mob->race].str_mod;
		mob->perm_str = UMIN( 25 /*MAX_STR*/, value + mob->level/5 );
		value = 13 + race_table[mob->race].int_mod;
		mob->perm_int = UMIN( 25 /*MAX_STR*/, value + mob->level/5 );
		value = 13 + race_table[mob->race].dex_mod;
		mob->perm_dex = UMIN( 25 /*MAX_STR*/, value + mob->level/5 );
		value = 13 + race_table[mob->race].wis_mod;
		mob->perm_wis = UMIN( 25 /*MAX_STR*/, value + mob->level/5 );
		value = 13 + race_table[mob->race].con_mod;
		mob->perm_con = UMIN( 25 /*MAX_STR*/, value + mob->level/5 );
		if ( ( mob->gold = pMobIndex->gold ) < 0 )
			mob->gold = number_fuzzy( 50 )
						* number_fuzzy( pMobIndex->level )
						* UMAX( (mob->level / 7 ), 1 )
						* UMAX( (mob->level / 17 ), 1 )
						* number_range( race_table[ mob->race ].glow, race_table[ mob->race ].ghigh ) / 100;
		mob->armor    = pMobIndex->mod_ac + ( (mob->level>36) ? interpolate1(36, -40, 45, -290, mob->level-36) :
						interpolate( mob->level, 100, -80 ) )
						+ ( ( mob->level > 30 ) ? race_table[ mob->race ].nature_ac : 40 );
		if( mob->level > 45 ) mob->armor -= mob->level * 4;
		if( mob->level > 55 ) mob->armor -= mob->level * 2;

		mob->dodge    = pMobIndex->dodge + ( (mob->level>36) ? 
		interpolate1(36, 0, 44, -140, mob->level-36) :
		interpolate( mob->level, 250,-90) )
					+ ( ( mob->level > 30 ) ? race_table[ mob->race ].nature_dodge : 0 );
		if( mob->level > 54 ) mob->dodge = mob->dodge - (mob->level/6);
		if( mob->level > 59 ) mob->dodge = mob->dodge - (mob->level/5);

		mob->max_hit  = pMobIndex->mod_hitplus + 
						number_range( mob->level * mob->level / 2, 
						mob->level * (mob->level + mob->level /2 ) ) +
						( ( mob->level > 30 ) ? mob->level * 60 : mob->level * 10 );
		if( mob->level > 49 )
			mob->max_hit += mob->level * 15;
		if( mob->level > 54 )
			mob->max_hit += mob->level * 20;
		mob->hit    = mob->max_hit;
		mob->max_mana = mob->max_hit / 2;
		mob->mana = mob->max_mana;
		mob->hitroll  = pMobIndex->mod_hitroll + ( (mob->level>28) ? interpolate1(28, 30, 45, 40, mob->level-28) :
						interpolate(mob->level, 5, 25) );
		mob->damroll  = pMobIndex->mod_damroll + ( (mob->level > 30) ? interpolate1(30, 40, 40, 65, mob->level-30) : 
						interpolate(mob->level, 3, 50) );
		for( i = 0 ; i < MAX_SAVING; ++i ){
			mob->saving_throw[i]  = pMobIndex->mod_svs[i] + ( (mob->level>42) ? interpolate1(42, -15, 60, -20, mob->level-42) :
			interpolate(mob->level, 5, -15) );
		;
		}
		for( i = 0; i < 9 ; i++ )
			mob->saving_throw[i] += race_table[pMobIndex->race].nature_svs[i];
	}
	else
	{
		value = 13 + race_table[mob->race].str_mod;
				mob->perm_str = UMIN( 25 /*MAX_STR*/, value + mob->level/8 );
		value = 13 + race_table[mob->race].int_mod;
				mob->perm_int = UMIN( 25 /*MAX_STR*/, value + mob->level/8 );
		value = 13 + race_table[mob->race].dex_mod;
				mob->perm_dex = UMIN( 25 /*MAX_STR*/, value + mob->level/8 );
		value = 13 + race_table[mob->race].wis_mod;
				mob->perm_wis = UMIN( 25 /*MAX_STR*/, value + mob->level/8 );
		value = 13 + race_table[mob->race].con_mod;
				mob->perm_con = UMIN( 25 /*MAX_STR*/, value + mob->level/8 );
		if ( ( mob->gold = pMobIndex->gold ) < 0 )
			mob->gold  = number_fuzzy( 50 )
						* number_fuzzy( pMobIndex->level )
						* UMAX( (mob->level / 9 ), 1 )
						* UMAX( (mob->level / 20 ), 1 )
						* number_range( race_table[ mob->race ].glow, race_table[ mob->race ].ghigh ) / 100;
			mob->armor = pMobIndex->mod_ac + ( (mob->level>36) ? interpolate1(36, -20, 45, -200, mob->level-36) :
						interpolate( mob->level, 110, -60 ) )
						+ ( ( mob->level > 36 ) ? race_table[ mob->race ].nature_ac : 60 );
		if( mob->level > 45 ) mob->armor -= mob->level * 4;

			mob->dodge = pMobIndex->dodge + ( (mob->level>34) ? interpolate1(34, 60, 44, -90, mob->level-34) :
						interpolate( mob->level, 300, 0 ) )
						+ ( ( mob->level > 36 ) ? race_table[ mob->race ].nature_dodge : 0 );

		mob->max_hit  = pMobIndex->mod_hitplus + 
						number_range( mob->level * mob->level / 2, 
						mob->level * mob->level ) +
						( ( mob->level > 34 ) ? mob->level * 45 : mob->level * 10 );
		mob->hit      = mob->max_hit;
		mob->max_mana = UMAX( 50, mob->max_hit / 5);
		mob->mana     = mob->max_mana;
		mob->hitroll  = pMobIndex->mod_hitroll + ( (mob->level>28) ? interpolate1(28, 20, 45, 30, mob->level-28) :
						interpolate(mob->level, 5, 25) );
		mob->damroll  = pMobIndex->mod_damroll + ( (mob->level > 33) ? interpolate1(33, 35, 40, 47, mob->level-33) : 
						interpolate(mob->level, 3, 50) );
		for( i = 0 ; i < MAX_SAVING; ++i ){
			mob->saving_throw[i]  = pMobIndex->mod_svs[i] + ( (mob->level>42) ? interpolate1(42, -8, 60, -20, mob->level-42) :
			interpolate(mob->level, 5, -15) );
			;
		}
		for( i = 0; i < 9 ; ++ i )
			mob->saving_throw[i] += race_table[pMobIndex->race].nature_svs[i];
	}
	mob->max_move = mob->max_mana;
	mob->move     = mob->mana;
	mob->pp       = pMobIndex->mod_pp;
	mob->class    = pMobIndex->class;
	mob->psk      = pMobIndex->psk;
	mob->jcl      = pMobIndex->jcl;
	mob->armor    = ( mob->armor * 6) / 5;
	// mob ³]¦³ mystic armor ¦b reborn ®É·|§@¥Î®ÄªG 2020/10/27
	if( IS_AFFECTED( mob , AFF_MYSTIC_ARMOR) ){
		AFFECT_DATA af;
		af.type  = gsn_mystic_armor;
		af.duration  = -1;
		//af.location  = APPLY_SAVING_SPIRIT;
		af.location  = APPLY_MAGIC_IMMUNE;
		af.modifier  = UMIN(150000 , mob->max_mana * 3);
		af.bitvector = AFF_MYSTIC_ARMOR;
		affect_join( mob, &af );
	}
	if ( ( pmob_action = pMobIndex->mob_action ) )
		while( pmob_action)
		{
			if( !mob_action_free )
			{
				++top_action;
				mob_action = (MOB_ACTION *)alloc_mem( sizeof( MOB_ACTION ) );
			}
			else
			{
				mob_action = mob_action_free;
				mob_action_free = mob_action_free->next_in_list;
			}
			if ( !mob->mob_action )/* used in free_char() */
				mob->mob_action      = mob_action;
			mob_action->owner        = mob;
			mob_action->com_list     = pmob_action->com_list;
			mob_action->old_com_list = pmob_action->com_list;
			mob_action->start_time   = pmob_action->start_time;
			mob_action->mode         = 0;
			mob_action->next         = NULL;
			if ( mob_action_list )
			{
				mob_action_list->prev_in_list = mob_action;
				if ( mob_action_list->owner == mob )
					mob_action_list->next = mob_action;
			}
			mob_action->next_in_list = mob_action_list;
			mob_action_list = mob_action;
			pmob_action = pmob_action->next;
		}

       /*
        * Added by Razgriz 20050720
        * Set [pp, pd, pdr, md, mdr] if fAutoset is TRUE
        *
        if ( mob->pIndexData->fAutoset )
        {
           mob->pdr    = (int) ( mob->perm_con * 8 + mob->level * 2 ) / 3; //pdr
           mob->mdr    = (int) ( mob->perm_wis * 4 +
                                 mob->perm_con * 4 + mob->level * 2 ) / 3; //mdr
           mob->apdr   = (int) ( mob->perm_str * 6 + mob->level * 4 ) / 4; //pd
           mob->amdr   = (int) ( mob->perm_int * 6 + mob->level * 4 ) / 4; //md
           mob->pp     = (int) ( mob->perm_wis * 2 + mob->level     ) / 2; //pp
        }
        */ //Mark by Razgriz    20050722
    /*
     * Insert in list.
     */
	mob->next   = char_list;
	char_list   = mob;
	pMobIndex->count++;
	return mob;
}



/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )
{
	OBJ_DATA *obj;
	OBJ_HP *php, *hp;
	int dmgbase;
	int avgdif;
	int weight;

	if ( !pObjIndex )
	{
		bug( "Create_object: NULL pObjIndex.", 0 );
		exit( 1 );
	}

	if( IS_SET(pObjIndex->extra_flags, ITEM_UNIQUE)) {
		OBJ_INDEX_DATA *tmp;
		if( ( obj = get_obj_index_vnum( pObjIndex->vnum ) )) {
			tmp = get_obj_index( pObjIndex->replica );
			if( !tmp ) {
				bug( "Create_object: Unique weapon without replica. %d", pObjIndex->vnum);
				exit(1);
			}
			return create_object( tmp, level );
		}
	}

	obj = new_object();

	obj->pIndexData = pObjIndex;
	obj->in_room  = NULL;
	//if ( pObjIndex->level )
	//	obj->level = pObjIndex->level;
	//else
	//	obj->level = level;
	obj->wear_loc = -1;

	if( !pObjIndex->material )
		pObjIndex->material = material_lookup( "Generic");

	obj->material = pObjIndex->material;
	obj->level = UMAX( level, pObjIndex->level );
	obj->level = UMAX( obj->level, obj->material->level );
	//obj->level    = UMIN( 38, obj->level );
	if( level >= 36 ){
		switch( number_bits(4) ){
			case 5:
				obj->level = 40;
				break;
			case 7:
			case 3:
			case 12:
			case 9:
				obj->level = number_range( 36, 39 );//LEVEL_IMMORTAL - 2, LEVEL_HERO );
			default:
				obj->level = number_range( 36, 38 );//LEVEL_IMMORTAL - 2, LEVEL_HERO );
		};
	}
	obj->name = str_dup( pObjIndex->name    );
	obj->short_descr = str_dup( pObjIndex->short_descr );
	/* obj->cname = str_dup( pObjIndex->cname );*/
	obj->description = str_dup( pObjIndex->description );
	obj->item_type = pObjIndex->item_type;
	obj->extra_flags = pObjIndex->extra_flags;
	obj->wear_flags = pObjIndex->wear_flags;
	obj->value[0] = pObjIndex->value[0];
	obj->value[1] = pObjIndex->value[1];
	obj->value[2] = pObjIndex->value[2];
	obj->value[3] = pObjIndex->value[3];
	obj->weight = pObjIndex->weight;
	obj->serial = time(NULL);
	obj->vrace = pObjIndex->vrace;
	if ( pObjIndex->cost )
		obj->cost = pObjIndex->cost;
	else
		obj->cost = number_fuzzy( 4 ) * number_fuzzy( level ) * number_fuzzy( level );
	obj->timer = pObjIndex->timer;
	obj->class = pObjIndex->class;
	obj->race = pObjIndex->race;

	if ( pObjIndex->hp ){
	for( php = pObjIndex->hp; php; php = php->next )
	{
	hp      = new_hiddenpower();
	hp->vnum  = pObjIndex->vnum;
	hp->hp_name = str_dup( php->hp_name );
	hp->percent = php->percent;
	hp->level = php->level;
	hp->event = php->event;
	hp->next  = obj->hp;
	obj->hp   = hp;
	}
	}

	obj->deleted  = FALSE;

	// material-depedent hack
	obj->cost   = (obj->cost * obj->material->cost) / 100;
	obj->extra_flags  |= obj->material->extra_flags;
	/*
	* Mess with object properties.
	*/
	switch ( obj->item_type )
	{
		default:
			bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
			break;

		case ITEM_LIGHT:
		case ITEM_TREASURE:
		case ITEM_FURNITURE:
		case ITEM_TRASH:
		case ITEM_CONTAINER:
		case ITEM_DRINK_CON:
		case ITEM_KEY:
		case ITEM_FOOD:
		case ITEM_BOAT:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
			break; // add at 2022/05/06

		// ¬u¤ô®e¶q 2020/01/23
		case ITEM_FOUNTAIN:
			if( obj->value[0] == 0 && obj->value[1] == 0){
				if( obj->weight > 0)
					obj->value[0] = obj->weight*10;
				else
					obj->value[0] = 1000;
					obj->value[1] = obj->value[0];
			}
			break;

		case ITEM_HORSE:
			break;

		case ITEM_AMMO: // ºj¼u2022/04/29
			dmgbase = level + obj->weight / 3 ;
			if( obj->value[1] == 0 ){
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
			}else{
				dmgbase = obj->value[1];
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) + 2);
			}
			if( IS_SET(obj->extra_flags, ITEM_EVIL)){
				dmgbase = (obj->level/9) + 1;
				obj->value[1] += dmgbase;
			}
			break;

		case ITEM_DART:   // §â dart ¤Î arrow ªº create ¤À¦¨¦PÃþ 2022/05/06
		case ITEM_ARROW:
			dmgbase = level + obj->weight / 3 ;
			if( obj->value[1] == 0 )
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
			else{
				//bugf("arrow [%d] has value1 != 0 ", pObjIndex->vnum);
				dmgbase = obj->value[1];
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) - 2);
			}
			if( obj->value[2] == 0 )
				obj->value[2] = number_fuzzy( number_fuzzy( dmgbase ) + 6);
			else{
				//bugf("arrow [%d] has value2 != 0 ", pObjIndex->vnum);
				dmgbase = obj->value[2];
				//obj->value[2] = number_fuzzy( number_fuzzy( dmgbase + 6 ) );
			}
			if( IS_SET(obj->extra_flags, ITEM_EVIL)){
				dmgbase = (obj->level/9) + 1;
				obj->value[1] += dmgbase;
				obj->value[2] += dmgbase;
			}
			break;

		case ITEM_SCROLL:
			obj->value[0] = number_fuzzy( obj->value[0] );
			break;

		case ITEM_WAND:
		case ITEM_STAFF:
			obj->value[0] = number_fuzzy( obj->value[0] );
			obj->value[1] = number_fuzzy( obj->value[1] );
			obj->value[2] = obj->value[1];
			break;

		case ITEM_WEAPON:
			weight = obj->weight * 2;
			dmgbase = level / 3 + weight / 5;
			avgdif = 6;
			switch( obj->value[3] ) {
				case 0: //hit
					break;
				case 1: //slice
					break;
				case 2: //stab
					break;
				case 3: //slash
					break;
				case 4: //whip
					dmgbase = level / 3 + weight / 4 ;
					break;
				case 5: //claw
					break;
				case 6: //blast
					break;
				case 7: //pound
					dmgbase = level / 3 + weight / 5 + 2;
					break;
				case 8: //crush
					dmgbase = level / 3 + weight / 5 + 4;
					break;
				case 9: //grep
					break;
				case 10: //bite
					break;
				case 11: //pierce
					dmgbase = level / 3 + weight / 6 + 1;
					break;
				case 12: //suction
					break;
				case 13: //chop
					break;
				case 14: //shot
					dmgbase = level / 3 + weight / 4;
					avgdif = 3;
					break;
				case 16: //ammo ¤@©w­n³]¦³­È 2022/05/06
					dmgbase = 0;
					avgdif = 0;
					break;
			}

			// ·í¦³³]©w­È®É value ­È¤£¦A·|¶]ÀH¾÷­È 2022/05/08
			if( obj->value[1] == 0 )
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
			/*else{
				//bugf("weapon [%d] has value1 != 0 ", pObjIndex->vnum);
				dmgbase = obj->value[1];
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
			} */

			if( obj->value[2] == 0 )
				obj->value[2] = number_fuzzy( number_fuzzy( dmgbase + avgdif ) );
			/*else {
				//bugf("weapon [%d] has value2 != 0 ", pObjIndex->vnum);
				dmgbase = obj->value[2];
				obj->value[2] = number_fuzzy( number_fuzzy( dmgbase + avgdif ) );
			} */

			switch( obj->value[3] ) {
				case 0: //hit
					break;
				case 1: //slice
					obj->value[1] += 3;
					obj->value[2] -= 1;
					break;
				case 2: //stab
					obj->value[1] -= 2;
					obj->value[2] += 2;
					break;
				case 3: //slash
					obj->value[1] -= 1;
					obj->value[2] += 1;
					break;
				case 4: //whip
					obj->value[1] -= 3;
					obj->value[2] += 1;
					break;
				case 5: //claw
					obj->value[1] += 2;
					obj->value[2] -= 2;
					break;
				case 6: //blast
					obj->value[1] += 2;
					obj->value[2] -= 1;
					break;
				case 7: //pound
					obj->value[1] -= 1;
					obj->value[2] += 2;
					break;
				case 8: //crush
					obj->value[1] += 3;
					obj->value[2] += 1;
					break;
				case 9: //grep
					obj->value[1] -= 3;
					obj->value[2] += 3;
					break;
				case 10: //bite
					obj->value[1] += 2;
					obj->value[2] -= 1;
					break;
				case 11: //pierce
					obj->value[1] += 3;
					obj->value[2] -= 1;
					break;
				case 12: //suction
					obj->value[1] -= 2;
					obj->value[2] += 1;
					break;
				case 13: //chop
					obj->value[1] -= 2;
					obj->value[2] += 2;
					break;
				case 14: //shot
					break;
				case 16: //ammo 2022/05/06
					break;
			}
			// ammo ¤£¨ü evil ¼vÅT 2022/05/06
			if( IS_SET(obj->extra_flags, ITEM_EVIL) && obj->value[3] != 16 ){
				dmgbase = (obj->level/9) + 1;
				obj->value[1] += dmgbase;
				obj->value[2] += dmgbase;
			}
			if( obj->value[0] == DURA_AUTO ){
				obj->value[0] = obj->material->value[3];
			}
			else if( obj->value[0] >= DURA_MAX ) {
				int du = obj->value[0];
				du -= ( du % 10000 );
				du /= 10000;
				obj->value[0] = du ;
			}
			break;

		case ITEM_ARMOR:
			if( obj->value[0] == 0 ){
				obj->value[0] = number_fuzzy( level / 4 + 2 );
				if( obj->level >= 38 && obj->value[0] == 12){ 
					if( number_bits(4) == 1)
						obj->value[0] = 13;
					else if ( number_bits(9) == 1 )
						obj->value[0] = 14;
				}
			}else{
				//bug("armor [%d] has value0 != 0 ", pObjIndex->vnum);
				//obj->value[0] = number_fuzzy( level / 4 + 2 );
				if( obj->level >= 38 && obj->value[0] == 12){ 
					if( number_bits(4) == 1)
						obj->value[0] = 13;
					else if ( number_bits(9) == 1 )
						obj->value[0] = 14;
				}
			}
			if( IS_SET(obj->extra_flags, ITEM_HOLY)){
				obj->value[0] += (obj->level / 13) + 1;
				if( obj->level >= 38 && number_bits( 13 ) == 1)
					obj->value[0] = UMIN( obj->value[0], 15);
				else if ( obj->level >= 36 && number_bits( 5 ) == 1)
					obj->value[0] = UMIN( obj->value[0], 14);
				else
					obj->value[0] = UMIN( obj->value[0], 13);
			}
			if( obj->value[2] == DURA_AUTO ) {
				obj->value[2] = obj->material->value[3];
			}else if( obj->value[2] >= DURA_MAX ) {
				int du = obj->value[2];
				du -= ( du % 10000 );
				du /= 10000;
				obj->value[2] = du ;
			}
			break;

		case ITEM_POTION:
		case ITEM_PILL:
			obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
			break;

		case ITEM_MONEY:
			obj->value[0] = obj->cost;
			break;
	}


	obj->weight = (obj->weight * obj->material->weight) / 100;
	obj->next = object_list;
	object_list = obj;
	pObjIndex->count++;

	return obj;
}

OBJ_DATA *fcreate_object( OBJ_INDEX_DATA *pObjIndex, int level )
{
	OBJ_DATA *obj;
	OBJ_HP  *php, *hp;
	int dmgbase;
	int avgdif;
	int weight;

	if ( !pObjIndex )
	{
		bug( "Create_object: NULL pObjIndex.", 0 );
		exit( 1 );
	}

	if( IS_SET(pObjIndex->extra_flags, ITEM_UNIQUE)) {
		OBJ_INDEX_DATA *tmp;
		if( ( obj = get_obj_index_vnum( pObjIndex->vnum ) )) {
			tmp = get_obj_index( pObjIndex->replica );
			if( !tmp ) {
				bug( "Create_object: Unique weapon without replica. %d", pObjIndex->vnum);
				exit(1);
			}
			return create_object( tmp, level );
		}
	}

	obj = new_object();

	obj->pIndexData = pObjIndex;
	obj->in_room = NULL;
	//if ( pObjIndex->level )
	//  obj->level = pObjIndex->level;
	//else
	//obj->level = level;
	//obj->level = UMAX( level, pObjIndex->level );
	obj->level = level;

	if (pObjIndex->level) {
		if (pObjIndex->level > obj->level) {
			obj->level = pObjIndex->level;
		}
	}
	obj->wear_loc = -1;

	if( !pObjIndex->material )
	pObjIndex->material = material_lookup( "Generic");

	obj->material = pObjIndex->material;
	// obj->level    = UMAX( obj->level, obj->material->level );
	//obj->level    = UMIN( 38, obj->level );
	//if( obj->level < LEVEL_HERO + 1 && obj->level > LEVEL_HERO - 3) {
	//  obj->level = number_range( LEVEL_HERO - 5, LEVEL_HERO );
	//  }
	obj->name = str_dup( pObjIndex->name    );
	obj->short_descr = str_dup( pObjIndex->short_descr );
	/* obj->cname = str_dup( pObjIndex->cname );*/
	obj->description = str_dup( pObjIndex->description );
	obj->item_type = pObjIndex->item_type;
	obj->extra_flags = pObjIndex->extra_flags;
	obj->wear_flags = pObjIndex->wear_flags;
	obj->value[0] = pObjIndex->value[0];
	obj->value[1] = pObjIndex->value[1];
	obj->value[2] = pObjIndex->value[2];
	obj->value[3] = pObjIndex->value[3];
	obj->weight = pObjIndex->weight;
	obj->serial = time(NULL);
	obj->vrace = pObjIndex->vrace;
	if ( pObjIndex->cost )
		obj->cost = pObjIndex->cost;
	else
		obj->cost = number_fuzzy( 4 ) * number_fuzzy( level ) * number_fuzzy( level );
	obj->timer = pObjIndex->timer;
	obj->class = pObjIndex->class;
	obj->race = pObjIndex->race;
	if ( pObjIndex->hp ){
		for( php = pObjIndex->hp; php; php = php->next )
		{
			hp = new_hiddenpower();
			hp->vnum = pObjIndex->vnum;
			hp->hp_name = str_dup( php->hp_name );
			hp->percent = php->percent;
			hp->level = php->level;
			hp->event = php->event;
			hp->next = obj->hp;
			obj->hp = hp;
		}
	}

	obj->deleted = FALSE;

	// material-depedent hack
	obj->cost = (obj->cost * obj->material->cost) / 100;
	obj->extra_flags |= obj->material->extra_flags;
	/*
	* Mess with object properties.
	*/
	switch ( obj->item_type )
	{
		default:
			bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
			break;

		case ITEM_LIGHT:
		case ITEM_TREASURE:
		case ITEM_FURNITURE:
		case ITEM_TRASH:
		case ITEM_CONTAINER:
		case ITEM_DRINK_CON:
		case ITEM_KEY:
		case ITEM_FOOD:
		case ITEM_BOAT:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
			break; // add at 2022/05/06

		// fcreate ªº¬u¤ô®e¶q 2022/05/06
		case ITEM_FOUNTAIN:
			if( obj->value[0] == 0 && obj->value[1] == 0){
				if( obj->weight > 0)
					obj->value[0] = obj->weight*10;
				else
					obj->value[0] = 1000;
					obj->value[1] = obj->value[0];
			}
			break;


		case ITEM_AMMO: // ºj¼u2022/04/29
			dmgbase = level + obj->weight / 3 ;
			if( obj->value[1] == 0 ){
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
			}else{
				dmgbase = obj->value[1];
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) + 2);
			}
			if( IS_SET(obj->extra_flags, ITEM_EVIL)){
				dmgbase = (obj->level/9) + 1;
				obj->value[1] += dmgbase;
			}
			break;

		case ITEM_DART:   // §â dart ¤Î arrow ªº create ¤À¦¨¦PÃþ 2022/05/06
		case ITEM_ARROW:
			dmgbase = level  + obj->weight / 3 ;
			if( obj->value[1] == 0 )
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
			else{
				//bugf("arrow [%d] has value1 != 0 ", pObjIndex->vnum);
				dmgbase = obj->value[1];
				obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) - 2);
			}
			if( obj->value[2] == 0 )
				obj->value[2] = number_fuzzy( number_fuzzy( dmgbase  ) + 6);
			else {
				//bugf("arrow [%d] has value2 != 0 ", pObjIndex->vnum);
				dmgbase = obj->value[2];
				//obj->value[2] = number_fuzzy( number_fuzzy( dmgbase + 6 ) );
			}
			if( IS_SET(obj->extra_flags, ITEM_EVIL)){
				dmgbase = (obj->level/9) + 1;
				obj->value[1] += dmgbase;
				obj->value[2] += dmgbase;
			}
			break;

		case ITEM_SCROLL:
			obj->value[0] = number_fuzzy( obj->value[0] );
			break;

		case ITEM_WAND:
		case ITEM_STAFF:
			obj->value[0] = number_fuzzy( obj->value[0] );
			obj->value[1] = number_fuzzy( obj->value[1] );
			obj->value[2] = obj->value[1];
			break;

		case ITEM_WEAPON:
			weight = obj->weight * 2;
			dmgbase = level / 3 + weight / 5;
			avgdif = 6;
			switch( obj->value[3] ) {
			case 0: //hit
				break;
			case 1: //slice
				break;
			case 2: //stab
				break;
			case 3: //slash
				break;
			case 4: //whip
				dmgbase = level / 3 + weight / 4 ;
				break;
			case 5: //claw
				break;
			case 6: //blast
				break;
			case 7: //pound
				dmgbase = level / 3 + weight / 5 + 2;
				break;
			case 8: //crush
				dmgbase = level / 3 + weight / 5 + 4;
				break;
			case 9: //grep
				break;
			case 10: //bite
				break;
			case 11: //pierce
				dmgbase = level / 3 + weight / 6 + 1;
				break;
			case 12: //suction
				break;
			case 13: //chop
				break;
			case 14: //shot
				dmgbase = level / 3 + weight / 4;
				avgdif = 3;
				break;
			case 16: //ammo ¤@©w­n³]¦³­È 2022/05/06
				dmgbase = 0;
				avgdif = 0;
				break;
		}

		// ·í¦³³]©w­È®É value ­È¤£·|¶]ÀH¾÷­È 2022/05/08
		if( obj->value[1] == 0 )
			obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
		/*else{
			//bugf("weapon [%d] has value1 != 0 ", pObjIndex->vnum);
			dmgbase = obj->value[1];
			obj->value[1] = number_fuzzy( number_fuzzy( dmgbase ) );
		} */

		if( obj->value[2] == 0 )
			obj->value[2] = number_fuzzy( number_fuzzy( dmgbase + avgdif ) );
		/*else {
			//bugf("weapon [%d] has value2 != 0 ", pObjIndex->vnum);
			dmgbase = obj->value[2];
			obj->value[2] = number_fuzzy( number_fuzzy( dmgbase + avgdif ) );
		}*/

		switch( obj->value[3] ) {
			case 0: //hit
				break;
			case 1: //slice
				obj->value[1] += 3;
				obj->value[2] -= 1;
				break;
			case 2: //stab
				obj->value[1] -= 2;
				obj->value[2] += 2;
				break;
			case 3: //slash
				obj->value[1] -= 1;
				obj->value[2] += 1;
				break;
			case 4: //whip
				obj->value[1] -= 3;
				obj->value[2] += 1;
				break;
			case 5: //claw
				obj->value[1] += 2;
				obj->value[2] -= 2;
				break;
			case 6: //blast
				obj->value[1] += 2;
				obj->value[2] -= 1;
				break;
			case 7: //pound
				obj->value[1] -= 1;
				obj->value[2] += 2;
				break;
			case 8: //crush
				obj->value[1] += 3;
				obj->value[2] += 1;
				break;
			case 9: //grep
				obj->value[1] -= 3;
				obj->value[2] += 3;
				break;
			case 10: //bite
				obj->value[1] += 2;
				obj->value[2] -= 1;
				break;
			case 11: //pierce
				obj->value[1] += 3;
				obj->value[2] -= 1;
				break;
			case 12: //suction
				obj->value[1] -= 2;
				obj->value[2] += 1;
				break;
			case 13: //chop
				obj->value[1] -= 2;
				obj->value[2] += 2;
				break;
			case 14: //shot
				break;
			case 16: //ammo 2022/05/06
				break;
		}
		if( IS_SET(obj->extra_flags, ITEM_EVIL) && obj->value[3] != 0){
			dmgbase = (obj->level/9) + 1;
			obj->value[1] += dmgbase;
			obj->value[2] += dmgbase;
		}
		if( obj->value[0] == DURA_AUTO ){
			obj->value[0] = obj->material->value[3];
		}
		else if( obj->value[0] > DURA_MAX ) {
			int du = obj->value[0];
			du -= ( du % 10000 );
			du /= 10000;
			obj->value[0] = du ;
		}
		break;

		case ITEM_ARMOR:
		if( obj->value[0] == 0 ){
			obj->value[0] = number_fuzzy( level / 4 + 2 );
			if( obj->level >= 38 && obj->value[0] == 12){ 
				if( number_bits(4) == 1)
					obj->value[0] = 13;
				else if ( number_bits(9) == 1 )
					obj->value[0] = 14;
			}
		}else{
			//bug("armor [%d] has value0 != 0 ", pObjIndex->vnum);
			//obj->value[0] = number_fuzzy( level / 4 + 2 );
			if( obj->level >= 38 && obj->value[0] == 12){ 
				if( number_bits(4) == 1)
					obj->value[0] = 13;
				else if ( number_bits(9) == 1 )
					obj->value[0] = 14;
			}
		}
		if( IS_SET(obj->extra_flags, ITEM_HOLY)){
			obj->value[0] += (obj->level / 13) +1;
			if( obj->level >= 38 && number_bits( 10 ) == 1)
				obj->value[0] = UMIN( obj->value[0], 15);
			else if ( obj->level >= 38 )
				obj->value[0] = UMIN( obj->value[0], 14);
			else
				obj->value[0] = UMIN( obj->value[0], 13);
		}
		if( obj->value[2] == DURA_AUTO ) {
			obj->value[2] = obj->material->value[3];
		}else if( obj->value[2] > DURA_MAX ){
			int du = obj->value[2];
			du -= ( du % 10000 );
			du /= 10000;
			obj->value[2] = du ;
		}
		break;

		case ITEM_POTION:
		case ITEM_PILL:
			obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
			break;

		case ITEM_MONEY:
			obj->value[0] = obj->cost;
			break;
	}


	obj->weight = (obj->weight * obj->material->weight) / 100;
	obj->next = object_list;
	object_list = obj;
	pObjIndex->count++;

	return obj;
}


/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;

    *ch       = ch_zero;
    ch->name      = &str_empty[0];
/*    ch->cname     = &str_empty[0];*/
    ch->short_descr   = &str_empty[0];
    ch->long_descr    = &str_empty[0];
    ch->description   = &str_empty[0];
    ch->last_note   = 0;
    ch->logon     = current_time;
    ch->armor     = 100;
    ch->position    = POS_STANDING;
    ch->level     = 0;
    ch->race      = 0;
    ch->practice    = 21;
    ch->hit     = 20;
    ch->max_hit     = 20;
    ch->mana      = 100;
    ch->max_mana    = 100;
    ch->move      = 100;
    ch->max_move    = 100;
    ch->leader      = NULL;
    ch->master      = NULL;
    ch->deleted     = FALSE;
    ch->protecting = NULL;
    ch->protector = NULL;
    ch->next_in_protect = NULL;
    ch->hunting = NULL;
    ch->hunted = NULL;
    ch->next_in_hunt = NULL;
    ch->tracking = NULL;
    ch->tracked = NULL;
    ch->next_in_track = NULL;
    ch->assassinating = NULL;
    ch->assassinated = NULL;
    ch->next_in_ass = NULL;
    ch->rembch = NULL;
    ch->rembed = NULL;
    ch->rrembed = NULL;
    return;
}



/*
 * Free a character.
 */
MOB_ACTION *mob_action_list, *mob_action_free;
BATTLE_ARRAY *ba_free;
void free_char( CHAR_DATA *ch )
{
    OBJ_DATA  *obj;
    OBJ_DATA  *obj_next;
    AFFECT_DATA *paf;
    QMARK   *qmark, *qmark_next;
    MOB_ACTION  *mob_action;

    /*
     * This places the character in a place we know exists and allows for
     * more controled removal of mud effects and such.  Suggested by Erwin.
     */

    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );

    for ( obj = ch->carrying; obj; obj = obj_next ){
        obj_next = obj->next_content;
        if ( obj->deleted ) continue;
        extract_obj( obj );
    }

    for ( paf = ch->affected; paf; paf = paf->next ){
        if ( paf->deleted ) continue;
        affect_remove( ch, paf );
    }

    if ( qmark = ch->qmark ){
      while( qmark ){
          qmark_next = qmark->next;
          del_qmark( qmark );
          qmark = qmark_next;
      }
    }
    free_string( ch->name   );
    free_string( ch->short_descr  );
    free_string( ch->cname    );
    free_string( ch->long_descr   );
    free_string( ch->description  );

    if ( ch->pcdata ){
        free_string( ch->pcdata->pwd    );
        free_string( ch->pcdata->bamfin   );
        free_string( ch->pcdata->bamfout  );
        free_string( ch->pcdata->immskll  );
        free_string( ch->pcdata->title    );
        free_string( ch->pcdata->prompt   );
        free_string( ch->pcdata->clantitle  );
        ch->pcdata->ride = 0;
        ch->pcdata->horse = NULL;
        if ( ch->pcdata->ba ){
            free_string( ch->pcdata->ba->ba11 );
            free_string( ch->pcdata->ba->ba12 );
            free_string( ch->pcdata->ba->ba13 );
            free_string( ch->pcdata->ba->ba21 );
            free_string( ch->pcdata->ba->ba22 );
            free_string( ch->pcdata->ba->ba23 );
            free_string( ch->pcdata->ba->ba31 );
            free_string( ch->pcdata->ba->ba32 );
            free_string( ch->pcdata->ba->ba33 );
            ch->pcdata->ba->next = ba_free;
            ba_free = ch->pcdata->ba;
            ch->pcdata->ba = NULL;      
        }
          
        ch->pcdata->next = pcdata_free;
        pcdata_free  = ch->pcdata;
    }
    if ( ( mob_action = ch->mob_action ) ){
//  for ( mob_action = mob_action_list; mob_action; mob_action = mob_action->next_in_list )
//      while( mob_action )
//          if ( mob_action->owner == ch )
        while( mob_action ){
            // if current mob_action is not the first one
            if ( mob_action->prev_in_list ){
                mob_action->prev_in_list->next_in_list = mob_action->next_in_list;
                if ( mob_action->next_in_list )
                    mob_action->next_in_list->prev_in_list = mob_action->prev_in_list;
            }else{
            // mob_action_list stores the first element of the list
                mob_action_list = mob_action->next_in_list;
                if ( mob_action->next_in_list )
                    mob_action->next_in_list->prev_in_list = NULL;
            }
            mob_action->owner = NULL;
            mob_action->prev_in_list = NULL;
            mob_action->next_in_list = mob_action_free;
            mob_action_free = mob_action;
            mob_action = mob_action->next;
        }
		}
    ch->next       = char_free;
    char_free      = ch;
    return;
}



/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed; ed = ed->next )
    {
  if ( is_name( name, ed->keyword ) )
      return ed->description;
    }
    return NULL;
}



char *get_pfxextra_descr( const char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed; ed = ed->next )
    {
  if ( is_pfxname( name, ed->keyword ) )
      return ed->description;
    }
    return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
  MOB_INDEX_DATA *pMobIndex;

  /*
   * There is a possibility of vnum passed is negative.
   * Trapping suggested by Erwin Andreasen  -Kahn
   */

  if( vnum < 0 )
     return NULL;

  for( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
       pMobIndex;
       pMobIndex  = pMobIndex->next )
  {
  	 //add by jye 0940617
  	 if( !pMobIndex)
  	     continue;
  	     
     if( pMobIndex->vnum == vnum )
         return pMobIndex;
  }

  if( fBootDb )
  {
      bug( "Get_mob_index: bad vnum %d.", vnum );
      exit( 1 );
  }

  return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
    OBJ_INDEX_DATA *pObjIndex;

    /*
     * There is a possibility of vnum passed is negative.
     * Trapping suggested by Erwin Andreasen  -Kahn
     */

    if ( vnum < 0 )
  return NULL;

    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
    pObjIndex;
    pObjIndex  = pObjIndex->next )
    {
  if ( pObjIndex->vnum == vnum )
      return pObjIndex;
    }

    if ( fBootDb )
    {
  bug( "Get_obj_index: bad vnum %d.", vnum );
  exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
	ROOM_INDEX_DATA *pRoomIndex;

	/*
	*  There is a possibility of vnum passed is negative.
	*  Trapping suggested by Erwin Andreasen  -Kahn
	*/

	if ( vnum < 0 )
		return NULL;

	for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH]; pRoomIndex; pRoomIndex  = pRoomIndex->next )
	{
		if ( pRoomIndex->vnum == vnum )
			return pRoomIndex;
	}

	if ( fBootDb )
	{
		bug( "Get_room_index: bad vnum %d.", vnum );
		exit( 1 );
	}

	return NULL;
}




int rgRecall[5];

void do_areas( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea1;
    AREA_DATA *pArea2;
    char buf  [ MAX_STRING_LENGTH*2   ];
    char buf1 [ MAX_STRING_LENGTH*8 ];
    char buf2 [ MAX_STRING_LENGTH ];
    char buf3 [ MAX_STRING_LENGTH ];
    char z_buf[ MAX_INPUT_LENGTH ];
    char buf_an[ 33 ];
    int iArea;
    int iAreaHalf;
    int type;

    z_buf[0]='\0';
    one_argument( argument, z_buf );
    buf1[0] = '\0';
    if( !z_buf || z_buf[0] == '\0' )
    {
        send_to_char("Areas«ü¥O¥i¥HÅý§A¬d¸ß¥»Mud²{¦³ªº°Ï°ì¦Cªí\n\r",ch);
        send_to_char("Areas«á­±ªº¦³®Ä°Ñ¼Æ¦p¤U¡G\n\r\n\r",ch);
        send_to_char("(1) °Ï°ì­ìµÛ \n\r", ch);
        send_to_char("(2) °Ï°ìÂ½Ä¶ \n\r", ch);
        send_to_char("(3) °Ï°ì§ï½s \n\r", ch);
        return;
    }

    iAreaHalf = ( top_area + 1 ) / 2;
    pArea1    = area_first;
    pArea2    = area_first;

    for ( iArea = 0; iArea < iAreaHalf; iArea++ )
        pArea2 = pArea2->next;

    if ( !is_number( z_buf ) )
        return;
    type = atoi( z_buf );
    if ( type > 3  || type < 1)
    { bug( ch->name, 0 );
        bug( "use area bug to make crash!!!", 0 );
        return;
    }

    //¶}©l°Q½×¤TºØ°Ñ¼Æ

	if(type==1) strcat(buf1,
		"  °Ï  °ì  ¦W  ºÙ           §@ ªÌ             °Ï  °ì  ¦W  ºÙ           §@ ªÌ   \n\r"

	);
	else if(type==2) strcat(buf1,
		"  °Ï  °ì  ¦W  ºÙ           Â½Ä¶ªÌ            °Ï  °ì  ¦W  ºÙ           Â½Ä¶ªÌ  \n\r"

	);
	else if(type==3) strcat(buf1,
		"  °Ï  °ì  ¦W  ºÙ           §ï½sªÌ            °Ï  °ì  ¦W  ºÙ           §ï½sªÌ  \n\r"

	);
	strcat(buf1,
		"-----------------------------------------+---------------------------------\n\r");
	for ( iArea = 0; iArea < iAreaHalf; iArea++ )
	{
	if(pArea1)
		strncpy( buf_an, pArea1->name, 26);
	else
		buf_an[0]='\0';
	buf_an[26] = '\0';

	if(type==1)
		sprintf( buf2, "%-27s%-7s", buf_an, (pArea1->clan) ? pArea1->translator : pArea1->builders);
	else if(type==2)
		sprintf( buf2, "%-27s%-7s", buf_an, pArea1->translator);
	else if(type==3)
		sprintf( buf2, "%-27s%-7s", buf_an, pArea1->modifier);
	if( pArea2 )
	{
		strncpy( buf_an, pArea2->name, 26);
		buf_an[26] = '\0';
		if(type==1)
			sprintf( buf3, "%-27s%-7s", buf_an, (pArea2->clan) ? pArea2->translator : pArea2->builders);
		else if(type==2)
			sprintf( buf3, "%-27s%-7s", buf_an, pArea2->translator);
		else if(type==3)
			sprintf( buf3, "%-27s%-7s", buf_an, pArea2->modifier);
	}
	else
	{
		buf_an[0]='\0';
		buf3[0]='\0';
	}
	// ­×¥¿ | ªº¦ì¸m 2022/10/21
	//sprintf( buf, "%-37s|%-37s\n\r", buf2, buf3 );
	sprintf( buf, "%-41s|%-41s\n\r", buf2, buf3 );
	strcat( buf1, buf );
	pArea1 = pArea1->next;
	if ( pArea2 )
		pArea2 = pArea2->next;
	}
    strcat(buf1,
		"-----------------------------------------+----------------------------------\n\r");
	send_to_char(buf1,ch);
	return;
}



void do_memory( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *rch;
	char buf [ MAX_STRING_LENGTH ];

	rch = get_char( ch );

	if ( !authorized( rch, "memory" ) )
		return;

	sprintf( buf, "Affects %5d\n\r", top_affect ); send_to_char( buf, ch );
	sprintf( buf, "Areas   %5d\n\r", top_area ); send_to_char( buf, ch );
	sprintf( buf, "NewBank %5d\n\r", top_bank ); send_to_char( buf, ch );
	sprintf( buf, "ExDes   %5d\n\r", top_ed ); send_to_char( buf, ch );
	sprintf( buf, "Exits   %5d\n\r", top_exit ); send_to_char( buf, ch );
	sprintf( buf, "Helps   %5d\n\r", top_help ); send_to_char( buf, ch );
	sprintf( buf, "Descs   %5d\n\r", top_desc ); send_to_char( buf, ch );
	sprintf( buf, "Chars   %5d\n\r", top_ch ); send_to_char( buf, ch );
	sprintf( buf, "Objs    %5d\n\r", top_obj ); send_to_char( buf, ch );
	sprintf( buf, "MobIndx %5d\n\r", top_mob_index ); send_to_char( buf, ch );
	sprintf( buf, "ObjIndx %5d\n\r", top_obj_index ); send_to_char( buf, ch );
	sprintf( buf, "Resets  %5d\n\r", top_reset ); send_to_char( buf, ch );
	sprintf( buf, "Rooms   %5d\n\r", top_room ); send_to_char( buf, ch );
	sprintf( buf, "Shops   %5d\n\r", top_shop ); send_to_char( buf, ch );
	sprintf( buf, "Qmarks  %5d\n\r", top_qmark ); send_to_char( buf, ch );
	sprintf( buf, "EqBanks %5d\n\r", top_bank ); send_to_char( buf, ch );
	sprintf( buf, "HiddenP %5d\n\r", top_hiddenpower); send_to_char( buf, ch );
	sprintf( buf, "CallOut %5d\n\r", top_callout); send_to_char( buf, ch );
	sprintf( buf, "Mob_Act %5d\n\r", top_action ); send_to_char( buf, ch );

	sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r\n\r", nAllocPerm, sAllocPerm );
	send_to_char( buf, ch );

	sprintf( buf, "Shared String Info:\n\r\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "Shared Strings   %5d strings of %7d bytes (max %d).\n\r", nAllocString, sAllocString, MAX_STRING );
	send_to_char( buf, ch );
	sprintf( buf, "Overflow Strings %5d strings of %7d bytes.\n\r", nOverFlowString, sOverFlowString );
	send_to_char( buf, ch );
	sprintf( buf, "ErrMem Blocks %d\n\r", top_errmem);
	send_to_char(buf, ch);
	if ( Full )
	{
		sprintf( buf, "Shared String Heap is full, increase MAX_STRING.\n\r" );
		send_to_char( buf, ch );
	}

	return;
}



/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
	switch ( number_bits( 2 ) )
	{
		case 0:  number -= 1; break;
		case 3:  number += 1; break;
	}

	return UMAX( 1, number );
}

/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
    int power;
    int number;

    if ( ( to = to - from + 1 ) <= 1 )
        return from;

    for ( power = 2; power < to; power <<= 1 ) {
        if (power < 0) return from;
    }

    while ( ( number = number_mm( ) & ( power - 1 ) ) >= to )
        ;

    return from + number;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    // use rand() in stdlib, modified by keelar
    return (1 + ( rand() % 100 ));
}



/*
 * Generate a random door.
 */
int number_door( void )
{
    int door;

    while ( ( door = number_mm( ) & ( 8-1 ) ) > 5 )
  ;

    return door;
}



int number_bits( int width )
{
    return number_mm( ) & ( ( 1 << width ) - 1 );
}



/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */
static  int rgiState[2+55];

void init_mm( )
{
	srand(time(NULL));
    int *piState;
    int  iState;

    piState = &rgiState[2];

    piState[-2] = 55 - 55;
    piState[-1] = 55 - 24;

    piState[0]  = ( (int) current_time ) & ( ( 1 << 30 ) - 1 );
    piState[1]  = 1;
    for ( iState = 2; iState < 55; iState++ )
    {
        piState[iState] = (  piState[iState-1] + piState[iState-2] )
                           & ( ( 1 << 30 ) - 1 );
    }
    return;
}



int number_mm( void )
{
	return rand();
/*  §ó§ï¶Ã¼Æªí 19/08/06
    int *piState;
    int  iState1;
    int  iState2;
    int  iRand;

    piState   = &rgiState[2];
    iState1   = piState[-2];
    iState2   = piState[-1];
    iRand   = ( piState[iState1] + piState[iState2] )
      & ( ( 1 << 30 ) - 1 );
    piState[iState1]  = iRand;
    if ( ++iState1 == 55 )
        iState1 = 0;
    if ( ++iState2 == 55 )
        iState2 = 0;
    piState[-2]   = iState1;
    piState[-1]   = iState2;
    return iRand >> 6;
*/
}



/*
 * Roll some dice.
 */
int dice( int number, int size )
{
    int idice;
    int sum;

    switch ( size )
    {
    case 0: return 0;
    case 1: return number;
    }

    for ( idice = 0, sum = 0; idice < number; idice++ )
  sum += number_range( 1, size );

    return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_42 )
{
    return value_00 + level * ( value_42 - value_00 ) / 42;
}

int interpolate1( int level1, int value1, int level2, int value2, int level )
{
    return value1 + level * ( value2 - value1 ) / ( level2 - level1 );
}



/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
  FILE *fp;

  if ( IS_NPC( ch ) || str[0] == '\0' )
    return;

  fflush( NULL );
  if (fpReserve) {
    fclose( fpReserve );
    fpReserve = NULL;
  }

  if ( !( fp = fopen( file, "a" ) ) )
  {
    perror( file );
    send_to_char( "Could not open the file!\n\r", ch );
  }
  else
  {
    fprintf( fp, "[%5d] %s: %s\n",
      ch->in_room ? ch->in_room->vnum : 0, ch->name, str );
    fflush( fp );
    fclose( fp );
  }

  fpReserve = fopen( NULL_FILE, "r" );
  return;
}



/*
 * Reports a bug.
 */
void bug( const char *str, int param )
{
	FILE *fp;
	char buf [ MAX_STRING_LENGTH * 36];

#if ENABLE_BUG_MESSAGE
	if( fpArea )
	{
		int iLine;
		long int iChar;

		if( fpArea == stdin )
		{
			iLine = 0;
		}
		else
		{
			iChar = ftell( fpArea );
			fseek( fpArea, 0, 0 );

			for( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
			{
				while( getc( fpArea ) != '\n' )
					;
			}

			fseek( fpArea, iChar, 0 );
		}

		sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
		log_string( buf );

		if ( ( fp = fopen( "SHUTDOWN.TXT", "a" ) ) )
		{
			fprintf( fp, "[*****] %s\n", buf );
			fflush( fp );
			fclose( fp );
		}
	}

	strncpy( buf, "[*****] BUG: " ,MAX_STRING_LENGTH * 36);
	sprintf( buf + strlen( buf ), str, param );
	log_string( buf );

	fflush( NULL );
	if (fpReserve) {
		fclose( fpReserve );
		fpReserve = NULL;
	}

	if( ( fp= fopen( BUG_FILE, "a" ) ) )
	{
		fprintf( fp, "%s\n", buf );
		fflush( fp );
		fclose( fp );
	}
	fpReserve = fopen( NULL_FILE, "r" );
#endif
	return;
}



/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
	char *strtime;

	strtime = ctime( &current_time );
	strtime[strlen( strtime )-1] = '\0';
	sprintf( pipe_buf, "%s :: %s\n", strtime, str );
	write(fd0[1], pipe_buf, strlen(pipe_buf));
	return;
}



/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}



/* the functions */

/* This routine transfers between alpha and numeric forms of the
 *  mob_prog bitvector types. This allows the use of the words in the
 *  mob/script files.
 */

RPROG_DATA * rprg_list;
OPROG_DATA *oprg_list;

void top10_load()
{
  FILE *fp;
  int i,j;
  int stat;
  char * none = "n / a";
  TOP10_DATA * ranks[9] = { tmudage, tgold, texp, thp, t_mp, tmv, tpk ,tresp, tmerit};

  fflush( NULL );
  if (fpReserve) {
    fclose( fpReserve );
    fpReserve = NULL;
  }
  
  for( j = 0; j < 9; ++j) {
    for( i = 0; i < TOP_COUNT + 1; ++i ) { 
      ranks[j][i].id = none;
      ranks[j][i].value = 0;
    }
  }
  
  if( ! (fp = fopen(TOP10_FILE, "r"))) {  
    return;
  }

  while(1) {
    char   letter;
    do {
      letter = getc( fp );
      if ( feof( fp ) ) {
        fflush( fp );
        fclose( fp );
        fpReserve = fopen( NULL_FILE, "r" );
        return;
      }
    } while ( isspace( letter ) );
    ungetc( letter, fp );
    for( j = 0; j < 9; ++j) {
      for( i = 0; i < TOP_COUNT + 1; ++i ) {
        ranks[j][i].id= fread_string( fp, &stat );
        ranks[j][i].value = fread_number(fp, &stat);
      }
    }
  }
  fflush( fp );
  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );
}

void top10_save()
{
  FILE *fp;
  int i,j;
  TOP10_DATA * ranks[9] = { tmudage, tgold, texp, thp, t_mp, tmv, tpk ,tresp, tmerit};

  fflush( NULL );
  if (fpReserve) {
    fclose( fpReserve );
    fpReserve = NULL;
  }

  if( ! (fp = fopen(TOP10_FILE, "w"))) {
    bug("fopen top10_file error.", 0);
    return;
  }

  for( j = 0; j < 9; ++j) {
    for( i = 0; i < TOP_COUNT + 1; ++i ) { 
      if( !(ranks[j][i].id )) {
        fprintf( fp, "%s~\n", "" );
        fprintf( fp, "%d\n", 0);
      } else {
        fprintf( fp, "%s~\n", ranks[j][i].id );
        fprintf( fp, "%d\n", ranks[j][i].value);
      }
    }
  }
  fflush( fp );
  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );
}

void bugf( char * fmt, ... )
{
    char        buf [ MAX_STRING_LENGTH * 4];
    va_list     args;

    va_start( args, fmt );
    vsprintf( buf, fmt, args );
    va_end( args );

    bug ( buf, 0 );
    return;
}



/*
*  * Writes a string to the log.
*   * Got it from Erwin S.A. source - Zen
*    */
void logf_envy( char * fmt, ... )
{
	char buf [ 2 * MAX_STRING_LENGTH ];
	va_list args;

	va_start( args, fmt );
	vsprintf( buf, fmt, args );
	va_end( args );

	log_string( buf );
	return;
}

/** zcecil
 * ÀË¬d mob ¬O¤£¬O¦b«D¦Û¥Dª¬ºA 
 */
bool is_real_npc( CHAR_DATA * ch )
{
    /* ¥Î±ø¦C¦¡¬O¬°¤F¥[³treturn false*/
    if( !IS_NPC(ch) ) return FALSE;
    if( IS_AFFECTED(ch, AFF_CHARM) ) return FALSE;
    if( IS_AFFECTED(ch, AFF_SUMMONED ) ) return FALSE;
    if( ch->clan ) return FALSE;

    return TRUE;
}
