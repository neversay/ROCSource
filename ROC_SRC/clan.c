 
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
//*  PROGRAM ID  : clan.c                                         *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         ­×§ï¦³«ü©wclan_channel®É               *
//*              :             ¬Ý¤£¨ì¦Û¤vclanªºÀW¹D¤§bug          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940617 jye                                    *
//*  PURPOSE     :         ­×§ïclan who§OÀ°®É¥»¨­lv4              *
//*              :             ¥H¤W·|¬Ý¨£exp gold¤§bug            *
//*****************************************************************
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
#include "olc.h"

/*
 * A clan structure
 */

extern const struct social_type social_table[];

CLAN_DATA *clan_free = NULL, *clan_list = NULL;

void free_clan_data( CLAN_DATA * pClan )
{
    free_string( pClan->name );
    free_string( pClan->whoname );
    free_string( pClan->filename );
    free_string( pClan->leader );
    free_string( pClan->founder );
    free_string( pClan->desc );
    free_string( pClan->feud );
    free_string( pClan->ally );

    pClan->next = clan_free;
    clan_free = pClan;
}


void do_clanrecall( CHAR_DATA *ch, char *argument );

char* bank_desc(long long int bank)
{
    if( bank <= 1000000 ) return "³h§x«×¤é";
    else if(bank <= 10000000) return "´¶´¶³q³q";
    else if ( bank <= 50000000 ) return "¤p¦³°]¤O";
    else if ( bank <= 100000000 ) return "°]¤j¶Õ¤j";
    else if ( bank <= 300000000 ) return "´I¥i¼Ä°ê";
    else if ( bank <= 600000000 ) return "¶¯ÅQ¤@¤è";
    else if ( bank <= 1000000000 ) return "¤ñº¸»\\¤Ñ";
    else return "¦h¨ìº¡¥X¨Ó¤F¡C";
}

struct clan_class
{
    char * name;
    int exp;
};

const struct    clan_class    cc[ ]  =
{
    { "§Î³æ¼v°¦",     10000   },
    { "¤T¤­¦n¤Í",     20000   },
    { "¦P¦n·|",       30000   },
    { "¤p«¬®a±Ú",     40000   },
    { "¤j«¬®a±Ú",     50000   },
    { "¶°·|",       60000   },
    { "µ²ªÀ",       70000   },
    { "©v±Ú",       90000   },
    { "¤ó±Ú",       110000   },
    { "²ÕÂ´",       140000   },
    { "¦a¤U²ÕÂ´",     170000   },
    { "¯«¯µ²ÕÂ´",     210000   },
    { "°í¹ê²ÕÂ´",     250000   },
    { "¤p¤sÀY",       300000   },
    { "¤sÀY",       360000   },
    { "¤j¤sÀY",       430000   },
    { "¨ó·|",       510000   },//start jump
    { "Áp¦X",       650000   },
    { "·|ªÀ",       800000   },
    { "À°·|",       970000   },
    { "±j¤jÀ°·|",     1200000   },
    { "²Ä¤@¤jÀ°",     1400000   },
    { "±j²±»â¦a",     1800000   },
    { "¶Õ¦p½Ñ«J",     2200000   },
    { "¶Õ¦p¤p°ê",     3000000   },
    { "¶Õ¦p¤j°ê",     3800000   },
    { "¨¹°ê",         5000000   },
    { "±j¤j¨¹°ê",     6600000   },
    { "¦U¨¹·ù¥D",     8000000   },
    { "¦U¨¹¦@¥D",     10000000   },
    { "¦U°ê·ù¥D",     12000000   },
    { "¦U°ê¦@¥D",     14000000   },
    { "¦U±Ú·ù¥D",     16000000   },
    { "¦U±Ú¦@¥D",     18000000   },
    { "¤j³°·ù¥D",     20000000   },
    { "¤j³°¦@¥D",     40000000   },
    { "¥@¬É·ù¥D",     60000000   },
    { "¥@¬É¦@¥D",     100000000   },
    { "¸¹¥O¤Ñ¤U",     150000000   },
    { "²ö´±¤£±q",     220000000   },
    { "½Ö»Pª§¾W",     350000000   },
    { "²V¨P¤Û¥@",     1000000000   },
    { "¯}ªí¤F",         2147480000   },
    { "",         0   }
};

// clanact modified by jye 6.13.2005
void clanact( const char *format, CHAR_DATA *ch, const void *arg1,
   const void *arg2, int type )
{
  OBJ_DATA     *obj1  = (OBJ_DATA  *) arg1;
  DESCRIPTOR_DATA *d;
  OBJ_DATA     *obj2  = (OBJ_DATA  *) arg2;
  CHAR_DATA    *to;
  CHAR_DATA    *vch   = (CHAR_DATA *) arg2;
  static char *    const  he_she  [ ] = { "¥¦",  "¥L",  "¦o" };
  static char *    const  him_her [ ] = { "¥¦",  "¥L",  "¦o" };
  static char *    const  his_her [ ] = { "¥¦ªº","¥Lªº","¦oªº" };
  const  char      *str;
  const  char      *i;
  char      *point;
  char       buf     [ MAX_STRING_LENGTH ];
  char       buf1    [ MAX_STRING_LENGTH ];
  char       fname   [ MAX_INPUT_LENGTH  ];
  bool       fChinese;

  /*
   * Discard null and zero-length messages.
   */
  if( !format || format[0] == '\0' )
       return;

  to = ch->in_room->people;
  if( type == TO_VICT )
  {
      if( !vch )
      {
          bug( "Act: null vch with TO_VICT.", 0 );
          sprintf( buf1, "Bad act string:  %s", format );
          bug( buf1, 0 );
          return;
      }
      
      if( !vch->in_room )//amenda 6/2/01 for debug purpose
      {
          bug( "clan.c, act:vch not in any room", 0 );
          bug( vch->name, 0 );
          return; 
      }
      to = vch->in_room->people;
  }

  for( d = descriptor_list; d; d = d->next ) 
  {
       to = (d->original) ? d->original : d->character;
       	
       if( !to ) continue;
       // if( (to && ( to->clan != ch->clan && to->clan_channel != ch->clan->whoname 
       //  && !str_cmp( to->clan_channel, "all" ) ) ) ) continue;
       if( to->clan != ch->clan && !to->clan_channel ) continue;
       if( to->clan_channel )
       {
           if( to->clan_channel != ch->clan->whoname 
               && to->clan != ch->clan )
           {
               if( str_cmp( to->clan_channel, "all" ) ) continue;
           }
       }

       sprintf(buf, "%s¡i%s¡j", HIY, ch->clan->whoname);
       point      = buf;
       point += strlen(buf);
       /**point++ = '';
       *point++ = '[';
       *point++ = '1';
       *point++ = ';';
       *point++ = '3';
       *point++ = '3';
       *point++ = 'm';*/
       str    = format;
       fChinese=FALSE;
       while ( *str != '\0' )
       {
          if( *str != '$' || fChinese )
          {
            //if ( fChinese )
            //    fChinese=FALSE;
            //else
              fChinese = is_chinese_char( *str, fChinese );
              //if ( *str>='\x80' && *str<='\xff' )
                //fChinese=TRUE;
              *point++ = *str++;
              continue;
          }

          ++str;
          
          if( !arg2 && *str >= 'A' && *str <= 'Z' )
          {
              bug( "Act: missing arg2 for code %d.", *str );
              sprintf( buf1, "Bad act string:  %s", format );
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
                     i = " <@@@> ";        break;
                 /* Thx alex for 't' idea */
                 case 't': i = (char *) arg1;            break;
                 case 'T': i = (char *) arg2;              break;
                 case 'n': i = PERS( ch,  to  );              break;
                 case 'N': i = PERS( vch, to  );              break;
                 case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];    break;
                 case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];    break;
                 case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];    break;
                 case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];    break;
                 case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];    break;
                 case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];    break;
                 
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
          while( ( *point = *i ) != '\0' )
                 ++point, ++i;
                 
          *point++ = '';
          *point++ = '[';
          *point++ = '1';
          *point++ = ';';
          *point++ = '3';
          *point++ = '3';
          *point++ = 'm';
       }
       
       *point++ = '';
       *point++ = '[';
       *point++ = '0';
       *point++ = 'm';
       *point++ = '\n';
       *point++ = '\r';
       buf[0]   = UPPER( buf[0] );
       if( to->desc )
           write_to_buffer( to->desc, buf, point - buf );
       /*
       if (MOBtrigger)
           mprog_act_trigger( buf, to, ch, obj1, vch );
       */
    }

  //MOBtrigger = TRUE;
  return;
}
bool clan_social( CHAR_DATA *ch, char *command, char *argument )
{
    CHAR_DATA *victim;
    char       arg [ MAX_INPUT_LENGTH ];
    int        cmd;
    bool       found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
    if ( command[0] == social_table[cmd].name[0]
      && !str_prefix( command, social_table[cmd].name ) )
    {
      found = TRUE;
      break;
    }
    }

    if ( !found )
  return FALSE;

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
  /*
   * I just know this is the path to a 12" 'if' statement.  :(
   * But two players asked for it already!  -- Furey
   */
  if ( !str_cmp( social_table[cmd].name, "snore" ) )
      break;
  send_to_char( "§A¬O¦b§@¹Ú¶Ü¡H\n\r",     ch );
  return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
  //clanact( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
  clanact( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
    }
    else if ( !( victim = get_char_world( ch, arg ) ) )
    {
  send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",        ch );
    }
    else if ( victim == ch )
    {
  //clanact( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
  clanact( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
    }
    else if ( !get_char_room( ch, arg ) && can_see( ch, victim ) )
    {
  if ( !IS_NPC( victim ) )
  {
      
      ROOM_INDEX_DATA *original;
      /*
      char      *ldbase      = "From far away, ";
      char       ldmsg [ MAX_STRING_LENGTH ];
      */

      original = ch->in_room;
      /*
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      */
      clanact( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
      /*
      char_from_room( ch );
      char_to_room( ch, original );
      strcpy( ldmsg, ldbase );
      strcat( ldmsg, social_table[cmd].char_found );
      act( ldmsg,           ch, NULL, victim, TO_CHAR    );

      strcpy( ldmsg, ldbase );
      strcat( ldmsg, social_table[cmd].vict_found );
      act( ldmsg,           ch, NULL, victim, TO_VICT    );

      char_from_room( ch );
      char_to_room( ch, original );
      */
  }
  else
  {
      send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r",      ch );
  }
    }
    else
    {
  //clanact( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
  //clanact( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );
  clanact( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );

    }

    return TRUE;
}
int clan_members( CLAN_DATA *pClan )
{
    int i, m = 0;
    for( i = 0; i < CLAN_LEVEL-1; ++i ) m+=pClan->member[i];

    return m+1;
}

CLAN_DATA *new_clan_data(void)
{
    CLAN_DATA *pClan = NULL;

    if( !clan_free ) {
  pClan = alloc_mem (sizeof(*pClan));
    
    }
    else {
  pClan = clan_free;
  clan_free = pClan->next;
    }

    pClan->dirty = 0;
    pClan->filename = NULL;
    
    return pClan;
}

void clan_update( void )
{
    FILE      *fp;
    CLAN_DATA  *cn;
    int i;
    //char buf [MAX_STRING_LENGTH];

    fclose( fpReserve );



    for ( cn = clan_list; cn; cn = cn->next )
    {
  //sprintf(buf, "%s%s", CLAN_DIR, cn->filename );
  if ( !( fp = fopen ( cn->filename, "w" ) ) )
  {
      bug( "Clan_update:  fopen of CLAN_FILE failed", 0 );
      return;
  }
  fprintf( fp, "#CLAN\n");
  fprintf( fp, "Nm  %s~\n", cn->name );
  fprintf( fp, "Wnm %s~\n", cn->whoname );
  fprintf( fp, "Fndr  %s~\n", cn->founder);
  fprintf( fp, "Ldr %s~\n", cn->leader );
  fprintf( fp, "Desc  %s~\n", cn->desc ? cn->desc : "");
  fprintf( fp, "Bank  %lld\n", cn->bank );
  fprintf( fp, "Mmbr      %d", cn->member[0] );
  for( i = 1; i < CLAN_LEVEL -1 ; ++i ) fprintf(fp, " %d", cn->member[i]);
  fprintf(fp, "\n");
  fprintf( fp, "CRecl %d\n", cn->crecall);
  fprintf( fp, "Ally  %s~\n", cn->ally);
  fprintf( fp, "Feud   %s~\n", cn->feud);
  fprintf( fp, "War   %d %d\n", cn->warwin, cn->warlose);
  fprintf( fp, "CScore  %d\n", cn->score);
  fprintf( fp, "CClass  %d\n", cn->cclass);
  fprintf( fp, "HIRE  %d\n", cn->hire);
  fprintf( fp, "Construct %d\n", cn->construct);
  fprintf( fp, "CObj  %d", cn->cobj[0]);
  for( i = 1; i < CLAN_LEVEL ; ++i ) fprintf(fp, " %d", cn->cobj[i]);
  fprintf( fp, "\n");
  fprintf( fp, "Killed  %d", cn->kill[0]);
  for( i = 1; i < CLAN_KILL ; ++i ) fprintf(fp, " %d", cn->kill[i]);
  fprintf( fp, "\n");
  fprintf(fp, "End\n");
  fprintf( fp, "#End\n" );
  fclose( fp );
    }

    if ( !( fp = fopen ( CLAN_FILE, "w" ) ) )
    {
  bug( "Clan_update:  fopen of CLAN_FILE failed", 0 );
  return;
    }
    for ( cn = clan_list; cn; cn = cn->next ){
  fprintf(fp, "%s\n", cn->filename);
    }
    fprintf(fp, "$~\n");
    fclose(fp);
    fpReserve = fopen( NULL_FILE, "r" );
    return;

}

/* code original from Greed Mud */
bool fread_clan( CLAN_DATA *pClan, FILE *fp)
{
    const char * word;
    bool fMatch = FALSE;
    int stat;

    for( ;; ){
  word   = feof( fp ) ? "End" : fread_word( fp, &stat );
  fMatch = FALSE;

  switch( UPPER( word[0])){
      case '*':
    fMatch = TRUE;
    fread_to_eol(fp);
    break;

      case 'A':
    if( !str_cmp(word, "Ally")){
        fMatch = TRUE;
        if( pClan->ally ) free_string( pClan->ally );
        pClan->ally = fread_string(fp, &stat);
    }
    break;

      case 'B':
    if( !str_cmp(word, "Bank")){
        fMatch = TRUE;
        pClan->bank = fread_number(fp, &stat);
    }
    break;

      case 'C':
    if( !str_cmp(word, "CRecl")){
        pClan->crecall = fread_number(fp, &stat);
        fMatch = TRUE;
    }
    else if ( !str_cmp(word, "CScore")){
        fMatch = TRUE;
        pClan->score = fread_number(fp, &stat);
    }
    else if ( !str_cmp(word, "CClass")){
        fMatch = TRUE;
        pClan->cclass = fread_number(fp, &stat);
    }
    else if ( !str_cmp(word, "CObj")){
        int i ;
        for( i=0; i< CLAN_LEVEL; ++i){
      pClan->cobj[i] = fread_number(fp, &stat);
      fMatch = TRUE;
      }
    }
    else if ( !str_cmp(word, "Construct") ){
        pClan->construct = fread_number(fp, &stat);
      fMatch = TRUE;
    }
    break;
    
      case 'D':
    if( !str_cmp(word, "Desc")){
        fMatch = TRUE;
        if( pClan->desc ) free_string( pClan->desc );
        pClan->desc = fread_string(fp, &stat);
    }
    break;

      case 'E':
    if( !str_cmp(word, "End")) return TRUE;
    break;
    
    
      case 'F':
    if( !str_cmp( word, "Feud")){
        fMatch = TRUE;
        if( pClan->feud ) free_string( pClan->feud);
        pClan->feud = fread_string( fp, &stat);
    }
    else if ( !str_cmp( word, "Fndr")){
        fMatch = TRUE;
        if( pClan->founder ) free_string( pClan->founder );
        pClan->founder = fread_string( fp, &stat);
    }
    break;

      case 'H':
    if( !str_cmp(word, "Hire")){
        fMatch = TRUE;
        pClan->hire = fread_number(fp, &stat);
    }
    break;

    case 'K':
    if( !str_cmp(word, "Killed")){
      int i;
      for( i=0; i< CLAN_KILL; ++i){
      pClan->kill[i] = fread_number(fp, &stat);
      fMatch = TRUE;
      }
    }
    break;
    
      case 'L':
    if( !str_cmp(word, "Ldr")){
        if( pClan->leader ) free_string(pClan->leader);
        pClan->leader = fread_string(fp, &stat);
        fMatch = TRUE;
    }
    break;

      case 'M':
    if( !str_cmp(word, "Mmbr")){
        int i;
        fMatch = TRUE;
        for( i = 0; i < CLAN_LEVEL-1; ++i){
      pClan->member[i] = fread_number(fp, &stat);
        }

    }
    break;

      case 'N':
    if(!str_cmp(word, "Nm")){
        if( pClan->name ) free_string(pClan->name);
        pClan->name = fread_string(fp, &stat);
//bug( pClan->name, 0 );
        fMatch = TRUE;
    }
    break;

      case 'W':
    if( !str_cmp(word, "War")){
        pClan->warwin = fread_number(fp, &stat);
        pClan->warlose = fread_number(fp, &stat);
        fMatch = TRUE;
    }
    else if ( !str_cmp(word,"Wnm")){
        fMatch = TRUE;
        if( pClan->whoname ) free_string(pClan->whoname);
        pClan->whoname = fread_string(fp, &stat);
    }
    break;
  }

  if( !fMatch ) {
      bugf("fread_clan: unknown key [%s].", word);
  }
  
    }
    return FALSE;
}


void old_load_clan( void )
{
    CLAN_DATA  *cn;
    FILE      *fp;

    if ( !( fp = fopen( "CLAN.TXT", "r" ) ) )
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
    fclose( fp );
    return;
      }
  }
  while ( isspace( letter ) );
  ungetc( letter, fp );

  if ( clan_free == NULL )
  {
      cn     = alloc_mem( sizeof( *cn ) );
  }
  else
  {
      cn     = clan_free;
      clan_free = clan_free->next;
  }

  cn->name   = fread_string( fp, &stat );
  cn->founder= fread_string( fp, &stat );
  cn->leader = fread_string( fp, &stat );
  cn->desc   = fread_string( fp, &stat );
  cn->bank   = fread_number( fp, &stat );
  cn->member[0] = fread_number( fp, &stat );
  if ( fread_letter( fp ) != '|' )
  {
      bug( "clan_load error", 0 );
      return;
  }

  cn->next   = clan_list;
  clan_list   = cn;
    }
}


bool load_clan_file( char* fn )
{
    FILE* fp;
    CLAN_DATA * pClan = NULL;
    char clanfile[MAX_STRING_LENGTH];
    int stat;

    sprintf(clanfile, "%s",  fn);
    if( !( fp = fopen( clanfile, "r" ))) {
  bugf("clan file not found : %s", clanfile);
  return FALSE;
    }

    pClan = new_clan_data();
    pClan->filename = str_dup(clanfile);

    for( ; !feof(fp); )
    {
  char *word;
  int letter;

  letter = fread_letter(fp);

  if( letter == '*' ) {
      fread_to_eol(fp);
      continue;
  }
  if ( letter != '#' )
  {
      bug( "Load_clan_file: # not found.", 0 );
      free_clan_data(pClan);
      break;
  }

  word = fread_word( fp, &stat );
  if( !word ) {
      bug("load_clan_file, no word!",0);
      return FALSE;
  }

  if ( !str_cmp( word, "CLAN" ) )
  {
      fread_clan( pClan, fp );

      pClan->next = clan_list;
      clan_list = pClan;

      break;
  }
  else if ( !str_cmp( word, "END"  ) )        break;
  else
  {
      bugf( "Load_clan_file: bad section: %s.", word );
      free_clan_data(pClan);
      break;
  }
    }
    fclose( fp );

    return TRUE;

}

void load_clans( void )
{
    FILE      *fp;
    char fn[MAX_INPUT_LENGTH];
    char *fname;
    int stat;

    /*if ( !( fp = fopen( "CLAN.TXT", "r" ) ) )
  return;

  old_load_clan();
  return;*/

    if( !(fp = fopen(CLAN_LIST, "r"))){
  bug("clan list not found!",0);
  return;
    }
    for ( ; ; )
    {
  if( feof(fp)) break;
  fname = fread_word(fp, &stat);
  strcpy(fn, fname);
  
  if( fn[0] == '$' ) break;
  bugf("Clan [%s] loaded.", fn);

  if( !load_clan_file(fn) ){
      bugf("cannot load clan file : %s", fn);
      return;
  }
    }
    fclose(fp);

    clan_war     = alloc_mem( sizeof( *clan_war ) );
    return;
}



void clan_title( CHAR_DATA *ch, char *title )
{
    char buf [ MAX_STRING_LENGTH ];

    if ( IS_NPC( ch ) )
    {
  bug( "Set_title: NPC.", 0 );
  return;
    }

    buf[0] = '\0';

  strcpy( buf, title );

    free_string( ch->pcdata->clantitle );
    ch->pcdata->clantitle = str_dup( buf );
    return;
}


void do_clanchatemote( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_INPUT_LENGTH];
    sprintf(buf, "chat* %s", argument);
    do_clan(ch, buf);
    return;
}

void do_clanchat( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_INPUT_LENGTH];
    sprintf(buf, "chat %s", argument);
    do_clan(ch, buf);
    return;
}

void do_clan( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA *d = descriptor_list;
  CHAR_DATA *victim = NULL;
  CHAR_DATA *vch;
  CLAN_DATA *cn = clan_list;
  CLAN_DATA *vclan = NULL;
  char       arg  [ MAX_INPUT_LENGTH    ];
  char    buf [ MAX_STRING_LENGTH * 2];
  char    buf1 [ MAX_STRING_LENGTH * 6];
  char    no_ansi [ MAX_INPUT_LENGTH ];
  char      clan_name[40];
  int     level = 0;
  int members = 0;
        int num=0;
  CHAR_DATA       *wch[ 500 ];
  char      const *class[ 500 ];
  int i=0;
  int rnum=0;
  int mnum=0;
  int onum=0;
  
    if ( IS_NPC( ch ) || ( ch->desc && ch->desc->original ) )
  return;

    argument = one_argument( argument, arg );
    if( arg[0] == '\0' ) strcpy(arg, "who");
    
    if ( !str_cmp( arg, "setup" ) )
    {
  char       arg2 [ MAX_INPUT_LENGTH    ];
  char       arg3 [ MAX_INPUT_LENGTH    ];
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  
    if ( !ch->in_room || ch->in_room->vnum != 11000)
      {
        send_to_char("¥Ø«eÀ°·|ªº³Ð³y»Ý­n¥ý¸g¥ÑImm ¦P·N³á¡I\n\r", ch);
        return; 
      }
  if( arg2[0] == '\0' || arg3[0] == '\0') {
      send_to_char("®æ¦¡¡G clan setup §¹¾ãÀ°¦W Â²ºÙ¡]¤G¨ì¥|¦r¡^\n\r", ch);
      return;
  }
  for( cn = clan_list; cn; cn = cn->next )
      if ( !str_cmp( cn->founder, ch->name) )
      {
    send_to_char("§A¤w¸g³Ð¹LÀ°¤F¡I\n\r", ch);
    return; 
      }
      if ( ch->bank < 8000000 )
      {
        send_to_char( "­n¦¨¥ß¤@­ÓÀ°·|¬O«Üªá¿úªº¡I§Aªº¦s´Ú¤£°÷\n\r", ch );
        return;
      }
      if ( ch->pcdata->respect < 2 || ch->level < 38 )
      {
        send_to_char( "µL¦W¤p¤l¤]·Q¶}©v¥ß¬£¡H\n\r", ch );
        return; 
      }
    if ( ch->clan )
    {
      send_to_char( "§A¤w¥[¤J§OªºÀ°¬£¡C\n\r", ch );
      return;
    }
      if ( strlen( arg2 ) > 31 || strlen(arg3) > 11)
      {
    send_to_char(YEL "À°¬£ªº¦W¦r¤Óªø¤F¡A½Ð¨ú¤@­Óµu¤@ÂIªº¡C"NOR"\n\r", ch);
    return;
      }
    for( cn = clan_list; cn; cn = cn->next )
      if ( !str_cmp( cn->name, arg2 ) || !str_cmp(cn->whoname, arg3) )
      {
        send_to_char( "¹ï¤£°_¡A³o­ÓÀ°¬£¤w¸g¦s¦b¡A½Ð¥t¨ú¦WºÙ¡C\n\r", ch );
        return; 
      }

      ch->bank -= 8000000;
      ch->pcdata->respect += 5;
      ch->pcdata->clanlevel = 5;
  clan_title(ch, "À°¥D");
      send_to_char( "¦]¬°¶}©v¥ß¬£¡A§Aªº«Â±æ¼W¥[¤F¡I", ch );
      if ( clan_free )
      {
        cn = clan_free;
        clan_free = clan_free->next;
      }
      else
      cn = new_clan_data();
    smash_tilde( arg2);
    smash_tilde(arg3);
    strcpy( buf, arg2);
    cn->name = str_dup( buf );
    strcpy(buf, arg3);
    cn->whoname = str_dup(buf);
    strcpy( buf, ch->name );
    cn->leader = str_dup( buf );
    cn->founder = str_dup( buf );
    sprintf(buf, "%s%s.clan", CLAN_DIR, ch->name);
    cn->filename = str_dup(buf);
    cn->bank = 1000000;
    cn->member[0] = 0;
    cn->member[1] = 0;
    cn->member[2] = 0;
    cn->member[3] = 0;
    cn->member[4] = 1;
    cn->next = clan_list;
    clan_list = cn;

    ch->clan = cn;

    sprintf( buf, "\n\r"HIY"¤@ªÑ·sªº¶Õ¤O"NOR" %s, "HIY"¥Ñ­^©úªº"NOR" %s "HIY"©Ò»â¾É¡AºCºC±U°_¤F¡I"NOR"\n\r",
        cn->name, ch->name );
    send_to_all_char( buf );
    clan_update();
    do_save(ch, "");
    return;
    }
    else if ( !str_cmp( arg, "transfer" ) )
    {
  if( IS_NPC(ch) || !ch->clan ) {return;}
  one_argument( argument,arg);
  if( arg[0] == '\0' 
  || !( victim = get_char_room( ch, arg ) )
  || IS_NPC(victim) ) {
      send_to_char("§A­n¶Ç¦ìµ¹½Ö¡H\n\r", ch);
      return;
  }
  //strcpy(clan_name, ch->clan->name);

  if( !victim->clan 
  || victim->clan != ch->clan ){
      send_to_char("¤£¯à¶Ç¦ìµ¹À°¥~¤§¤H¡I\n\r", ch);
      return;
  }
  if( victim->level < 38 ) {
      send_to_char("¥Lªº¯à¤O¤£¨¬¥H±µ¥ôÀ°¥D¤@Â¾¡C\n\r", ch);
      return;
  }
  if ( ch->pcdata->clanlevel < 5 )
  {
      send_to_char("§O¾x¤F¡A§A®Ú¥»¤£¬OÀ°¥D¡C\n\r", ch);
      return;
  }
  if ( ch == victim )
  {
      send_to_char("³o¼Ë¦³¤°»ò·N¸q¡H\n\r", ch);
      return;
  }
  free_string(ch->clan->leader);
  ch->clan->leader = str_dup( victim->name );
  ch->clan->score -= ch->clan->cclass * 5000;
  ch->pcdata->clanlevel = 4;
  victim->pcdata->clanlevel = 5;
  ch->pcdata->respect -= 2;
  victim->pcdata->respect += 2;
  clan_title(ch, "«eÀ°¥D");
  clan_title(victim, "À°¥D");

  sprintf(buf, "\n\r%s%s(%s)%s«Å§G¡G¥»¤H±N%s %s %sÀ°¥D¤§¦ì¶Çµ¹ %s%s %s¡I%s\n\r\n\r",
    HIY,
    ch->cname,
    ch->name,
    HIY,
    NOR,
    ch->clan->name,
    HIY,
    NOR,
    victim->name,
    HIY,
    NOR);
  send_to_all_char( buf );
  send_to_char( "§Aªº«Â±æ¤U­°¤F¡I\n\r", ch);
  send_to_char( "ªÓ­t°_¤@À°­«¥ô¡A§Aªº«Â±æ¤W¤É¤F¡I\n\r", victim);

  
    }
  else if ( !str_cmp( arg, "join") )
  {
    if ( ch->clan )
    {
      send_to_char("§A¤w¸g¥[¤J§OªºÀ°¬£¤F¡C\n\r", ch);
      return;
    }
    one_argument(argument, arg);
    if ( !str_cmp( arg, "cancel") )
    {
      if ( !ch->clan_join )
      {
        send_to_char( "§A¨ì²{¦b¬°¤îÁÙ¨S¦³¥[¤JÀ°¬£ªº©ÀÀY¡C\n\r", ch );
        return;
      }
      sprintf( buf, "§A¥´®ø¥[¤J%sªº©ÀÀY¡I\n\r", ch->clan_join );
      send_to_char( buf, ch );
      /*sprintf(buf, "%s¡i%s¤½§i¡j%s%s¥´®ø¥[¤J¶QÀ°ªº©ÀÀY¡I%s\n\r",
          HIY,
          ch->clan_join,
          ch->short_descr,
          HIY,
          NOR);
          for ( d = descriptor_list; d; d = d->next )
          {
            if( d->connected != CON_PLAYING ) continue;
            victim = (d->character) ? d->character : d->original;
            if( !victim ) continue;
            if( victim->clan->whoname != ch->clan_join && !victim->clan_channel ) continue;
            if( victim->clan_channel )
            {
              if ( victim->clan_channel != ch->clan_join )
              {
                if ( str_cmp( victim->clan_channel, "all" ) ) continue;
              }
            }
              send_to_char(buf, victim);
          }*/
      ch->clan_join = NULL;
      return;
    }
    sprintf(clan_name, "%s", arg);
    for( cn = clan_list; cn; cn = cn->next ) {
      if(!str_cmp(clan_name, cn->leader)) {
        ch->clan_join = cn->whoname;
        sprintf( buf, "§A§Æ±æ¥[¤JªºÀ°¬£¬O¡G%s¡I\n\r", ch->clan_join );
        send_to_char( buf, ch );
        /*sprintf(buf, "%s¡i%s¤½§i¡j%s%s§Æ±æ¥[¤J¶QÀ°¡A½ÐÀ°¤¤¤¸¦Ñ©Û¦¬¡I%s\n\r",
          HIY,
          ch->clan_join,
          ch->short_descr,
          HIY,
          NOR);
          for ( d = descriptor_list; d; d = d->next )
          {
            if( d->connected != CON_PLAYING ) continue;
            victim = (d->character) ? d->character : d->original;
            if( !victim ) continue;
            if( victim->clan->whoname != ch->clan_join && !victim->clan_channel ) continue;
            if( victim->clan_channel )
            {
              if ( victim->clan_channel != ch->clan_join )
              {
                if ( str_cmp( victim->clan_channel, "all" ) ) continue;
              }
            }
              send_to_char(buf, victim);
          }*/
        break;
      }
    }
    if( !ch->clan_join )
    {
      send_to_char( "¹ï¤£°_¡A§ä¤£¨ì§A­n§äªºÀ°¬£¡C\n\r", ch );
      return;
    }
    return;
  }
    else if ( !str_cmp( arg, "recall") )
    {
  do_clanrecall(ch, "");
  return;
    }
    else if ( !str_cmp(arg, "goto")){
  ROOM_INDEX_DATA *pRoom;
  if( !ch->clan ) return;
  if( ch->pcdata->clanlevel < 3 ) {
      send_to_char("§A¦bÀ°¤¤ªº¦a¦ì¤£°÷¡C\n\r", ch);
      return;
  }
  one_argument(argument, arg);
  if( !( pRoom = get_room_index( atoi(arg)))){
      send_to_char("¨S¦³³o­Ó¦a¤è¡C\n\r", ch);
      return;
  }
  if( !IS_PPLBUILDER(ch, pRoom->area)|| !IS_PPLBUILDER(ch, ch->in_room->area)){
      send_to_char("§AµLªk²¾°Ê¨ì¨º¸Ì¥h¡C\n\r", ch);
      return;
  }
  act("$nªº¨­¼v¬ðµM±q§A­±«e®ø¥¢¡C\n\r", ch, NULL, NULL, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch, pRoom);
  act("$n¬ðµM¥X²{¦b³o¸Ì¡C\n\r", ch, NULL, NULL, TO_ROOM);
  do_look(ch, "auto");
  return;
    }
    else if ( !str_cmp( arg, "chat") )
    {
  if( !( ch->clan ) ) return;
  one_argument( argument, arg);
  if( arg[0] == '\0' ) {
      send_to_char("§A­n»¡¤°»ò¡H\n\r", ch);
      return;
  }
  if( strlen(arg) > 100 )
  {
     send_to_char("¿é¤J¹Lªø.\n\r", ch);
         return;
    }
  //strcpy(clan_name, ch->clan->name);
  sprintf(buf, "%s¡i%s¡j%s%s¶¢²áµÛ¡u%s¡v%s\n\r",
    HIY,
    ch->clan->whoname,
    ch->short_descr,
    //ch->name,
    HIY,
    argument,
    NOR);
strcpy( buf1, "[*****]  CC: " );
strcat( buf1, buf );
log_string( buf1 );

  for ( d = descriptor_list; d; d = d->next )
  {
      if( d->connected != CON_PLAYING ) continue;
//      victim = (d->original) ? d->original : d->character;
      victim = (d->character) ? d->character : d->original;
      if( !victim ) continue; //&& !( victim->clan && victim->clan_channel ) ) continue;
      if( victim->clan != ch->clan && !victim->clan_channel ) continue;
    if( victim->clan_channel )
    {
      if ( victim->clan_channel != ch->clan->whoname 
           && victim->clan != ch->clan )
      {
        if ( str_cmp( victim->clan_channel, "all" ) ) continue;
      }
    }
      send_to_char(buf, victim);
  }
  return;
    }
    else if ( !str_cmp( arg, "chat*") )
    {
  if( !( ch->clan ) ) return;
  argument = one_argument( argument, arg);
  if( !arg|| arg[0] == '\0' ) return;

  //    sprintf(buf,"[%s][%s]\n\r", command, argument);
  //send_to_char(buf, ch);
  clan_social(ch, arg, argument);
  return;
    }
    else if ( !str_cmp( arg, "accept" ) )
    {
  if( !ch->clan ) return;
    one_argument( argument, arg );
      if ( arg[0] == '\0' || !( victim = get_char_room( ch, arg ) ) || IS_NPC(ch))
      {
        send_to_char( "³o­Ó¤H¤£¦b³o¸Ì°Ú¡H\n\r", ch );
        return;
      }
    if ( victim->clan )
    {
      send_to_char( "«Ü¥i±¤¡A§AºC¤F¤@¨B¡A¥L(¦o)¤w¥[¤J¨ä¥LÀ°¬£¤F¡C\n\r", ch );
      return;
    }
    if ( !victim->clan_join )
    {
      send_to_char( "«Ü¥i±¤¡A¥L(¦o)ÁÙ¨S¦³¿ï¾Ü­n¥[¤JªºÀ°¬£¡C\n\r", ch );
      return;
    }
    if ( ch->clan->whoname != victim->clan_join )
    {
      send_to_char( "«Ü¥i±¤¡A¥L(¦o)¨S¦³¥[¤J§AÀ°¬£ªº¥´ºâ¡C\n\r", ch );
      return;
    }
    if ( IS_NPC( victim ) || ch->pcdata->clanlevel < 3 )
    {
      send_to_char( "§Aªº¥÷¶q¤£°÷¡C\n\r", ch );
      return;
    }
    if ( victim->level < 10 )
    {
      send_to_char( "§AªºÀ°¬£¤£·|·Q­n³oºØ¤p°­ªº¡C\n\r", ch );
      return;
    }
    if ( victim->gold < 30000 )
    {
      send_to_char( "¥Lªº¨­¤W¨S¦³±a¨¬°÷ªºª÷¹ô¡C\n\r", ch );
      send_to_char( "§Aªº¨­¤W¨S¦³¨¬°÷ªºª÷¹ôÃº¥æ·|¶O¡C\n\r", victim );
      return;
    }
    if ( ch->clan->bank < 100000 )
    {
      send_to_char( "À°¤¤ªº±b¤á¨S¦³¨¬°÷ªºª÷¹ôÃº¥æ§l¦¬¶O¡C\n\r", ch );
      send_to_char( "³o­ÓÀ°¬£¤Ó½a¤F¡A¾i¤£°_§A¡C\n\r", victim );
      return;
    }
    victim->gold -= 30000;
    ch->clan->bank -= 100000;
                if( victim->level <= 20 )
                    ch->clan->score += victim->level * 10;
                else
                    ch->clan->score += UMIN( victim->level * (victim->level + victim->level / 3), 1000);
    ch->clan->member[0] +=1;
    victim->pcdata->respect += 2;
    victim->pcdata->clanstatistics = 0;
    victim->pcdata->clanexp = 0;
    victim->pcdata->clanlevel = 0;
    sprintf( buf, "¦]¬°¥[¤J %s, §Aªº«Â±æ¤W¤É¤F¡I\n\r",
        ch->clan->name );
    send_to_char( buf, victim );
    victim->clan = ch->clan;
    clan_title(victim, "À°²³");
    sprintf( buf, "\n\r"HIY"%s(%s)"HIY"¥Ñ%s"HIY"¤Þ»â¥[¤J%s"HIY"¡A¨Ï±o%s"HIY"Án¶Õ¤j®¶¡I"NOR"\n\r\n\r",
      victim->cname,victim->name, ch->cname, ch->clan->name, ch->clan->name );
    send_to_all_char( buf );
  }
  else if ( !str_cmp( arg, "retrieve" ) )
  {
  if( !ch->clan ) return;
    one_argument( argument, arg );
      if ( arg[0] == '\0' || !( victim = get_char_world( ch, arg ) ) || ch == victim || IS_NPC(victim))
      {
        send_to_char( "³o­Ó¤H¤£¦b³o¸Ì°Ú¡H\n\r", ch );
        return;
      }
    if ( ch->pcdata->clanlevel < 3 )
    {
        send_to_char("§A¨S¦³¨¬°÷ªºÅv¤O¡I\n\r", ch);
      return;
    }
    if( ch->pcdata->clanlevel <= victim->pcdata->clanlevel )
    {
        send_to_char("§A¤£¯àÅX³v§Aªº¤W¥q¡I\n\r", ch);
        return;
    }
    if( ch->clan != victim->clan ) {
        send_to_char("§A­Ì¤£¬O¦P¤@­ÓÀ°·|ªº¡C\n\r", ch);
        return;
    }
      if ( ch->clan->bank < 200000 )
      {
      send_to_char( "À°¬£ªº±b¤á¨S¦³¨¬°÷ªºª÷¹ô¥I»º´²¶O¡C\n\r", ch );
      return;
      }
      if ( !victim->clan || IS_NPC( victim ) )
      {
        send_to_char( "¥L(¦o)®Ú¥»¨S¦³¥[¤J¥ô¦óÀ°¬£¡A¦ó°h¤§¦³¡H\n\r", ch );
        return;
      }
      ch->clan->bank -= 200000;
      ch->clan->score -= 1000;
    victim->pcdata->respect -= 3;
    sprintf( buf, "¦]¬°³Q½ð¥X %s, $s«Â±æ¤U­°¤F¡I\n\r",
        victim->clan->name );
    send_to_char( buf, victim );
    victim->clan = NULL;
    ch->clan->member[0] -= 1;
    sprintf( buf, "\n\r%s"HIY"²M²zªù¤á¡A±N"NOR"%s"HIY"¥Ñ"NOR"%s"HIY"¶}°£¡I"NOR"\n\r\n\r",
      ch->cname, victim->cname, ch->clan->name );
    send_to_all_char( buf );
  }
  else if ( !str_cmp( arg, "leave" ) )
  {
  if( !ch->clan ) return;
    if ( ch->pcdata->clanlevel > 4 )
    {
      send_to_char( "¥@¤W°Z¦³À°¥D°hÀ°¤§²z¡H\n\r", ch );
      return;
    }
    if( ch->pcdata->clanlevel > 2 ) {
        send_to_char("§A¨­©~­nÂ¾¡A¤£¯à¥ô·N°hÀ°¡C\n\r", ch);
        return;
    }
      if ( ch->gold < 100000 )
      {
      send_to_char( "§Aªº¨­¤W¨S¦³¨¬°÷ªºª÷¹ô¤ä¥I°h·|¶O¡C\n\r", ch );
      return;
      }
      if ( !ch->clan )
      {
        send_to_char( "§A®Ú¥»¨S¦³¥[¤J¥ô¦óÀ°¬£¡A¦ó°h¤§¦³¡H\n\r", ch );
        return;
      }
      ch->gold -= 100000;
    ch->pcdata->respect -= 2;
    sprintf( buf, "¦]¬°°h¥X %s, §Aªº«Â±æ¤U­°¤F¡I\n\r",
        ch->clan->name );
    send_to_char( buf, ch );
    sprintf( buf, "\n\r"HIY"¹D¤£¦P¤£¬Û¬°¿Ñ¡A"NOR"%s(%s)"HIY"¼ÝµM¦a°h¥X"NOR"%s"HIY"¡I"NOR"\n\r\n\r",
      ch->cname,ch->name, ch->clan->name );
    send_to_all_char( buf );
    ch->clan->score -= 1000;
    ch->clan->member [0]-=1;
    ch->clan = NULL;
  }
  else if ( !str_cmp( arg, "all" ) )
  {
      int clv;
      int lv;
            int count = 0;
    sprintf(buf1, "[ "HIC"½u¤W©Ò¦³²ÕÂ´¦¨­û¦Cªí"NOR" ]\n\r" );
    strcat(buf1,"========================================================================"NOR"\n\r");

    for( clv = 5; clv >= 0; --clv) {
                    for( lv = MAX_LEVEL; lv > 0; --lv ) {
                        for ( d = descriptor_list; d; d = d->next )
                        {
                            if( d->connected != CON_PLAYING ) continue;
                            victim = (d->original) ? d->original : d->character;
                            if( victim->level != lv ) continue;
                            if( !can_see(ch, victim)) continue;
                            if( !victim->clan ) continue;
                            if( victim->pcdata->clanlevel != clv ) continue;
                            ++count;
                            sprintf( buf , "¡i"HIW"%8s %1d %2d"NOR" ¡j "HIG"%s"NOR"  -  %s("HIY"%s"NOR") %s \n\r",
                                    victim->clan->whoname,
                                    victim->pcdata->clanlevel,
                                    victim->level,
                                    //    class_table[ victim->class ].cwho_name,
                                    victim->pcdata->clantitle,
                                    victim->cname,
                                    victim->name,
                                    victim->pcdata->title
                                   );
                            strcat( buf1, buf );
                            if( count >= 30 ) {
                                send_to_char(buf1, ch);
                                buf1[0] = '\0';
                                count = 0;
                            }
                        }
                    }
    }
    strcat( buf1, "========================================================================"NOR"\n\r" );
    send_to_char(buf1, ch);
    return;
  }
  else if ( !str_cmp( arg, "who" ) )
  {
    one_argument(argument, arg);
    if( arg[0] == '\0' )
    {
      if ( !ch->clan )
      {
        send_to_char( "¹ï¤£°_¡A§A¨Ã¨S¦³¥[¤J¥ô¦óÀ°¬£¡C", ch );
        return;
      }
        //strcpy(clan_name, ch->clan->name);
        vclan = ch->clan;
    }
    else{
        //strcpy(clan_name, arg);
        sprintf(clan_name, "%s", arg);
        //vclan = ch->clan;
        for( cn = clan_list; cn; cn = cn->next ) {
      if(!str_cmp(clan_name, cn->leader)) {
          vclan = cn;
          break;
      }
        }
      if( !vclan )
      {
        send_to_char( "¹ï¤£°_¡A§ä¤£¨ì§A­n§äªºÀ°¬£¡C", ch );
        return;
      }
    }
    sprintf( buf , "[ "HIC"%sÀ°­û¦Cªí"NOR" ]",vclan->whoname);
    send_to_char(buf, ch);
    if( ch->pcdata->clanlevel > 0 && ch->clan == vclan ) {
        sprintf(buf, " ¥Ø«eÀ°¶O¡G"HIY"%lld"NOR", µo®iµ¥¯Å¡G"HIY"%2d"NOR", ¾Ô§Ð¡G"HIB"%d ³Ó"NOR" "HIR"%d ±Ñ"NOR"\n\r",
          vclan->bank, vclan->cclass, vclan->warwin, vclan->warlose);
        send_to_char(buf, ch);
    }
    else {send_to_char("\n\r", ch);}
    sprintf(buf,"========================================================================"NOR"\n\r");
    send_to_char(buf, ch);
    for ( d = descriptor_list; d; d = d->next )
    {
        if( d->connected != CON_PLAYING ) continue;
        victim = (d->original) ? d->original : d->character;
        if( !can_see(ch, victim)) continue;
        if( victim->clan 
        //&& !str_cmp( clan_name, victim->clan->name))
        && victim->clan == vclan )
        {
      wch[members] = victim;
      class[members] = class_table[ wch[members]->class ].cwho_name;
      ++members;
        }
    }
    for( level = 5; level >=0; --level) {
        for( i = 0; i<members;++i){
      if ( wch[i]->pcdata->clanlevel == level )
      {
          cn = wch[i]->clan;
          if( (ch->pcdata->clanlevel > 3 && ch->clan == vclan) || IS_IMMORTAL(ch))
          {
        strip_ansi( wch[i]->name, no_ansi );  
        sprintf( buf , "¡i"HIW"%1d %2d"NOR" "HIW"%6s"NOR" ¡j %s - %s("HIY"%s"NOR")  ("HIY"%d, %d"NOR")\n\r",
          wch[i]->pcdata->clanlevel,
          wch[i]->level,
          class[i],
          wch[i]->pcdata->clantitle,
          wch[i]->cname,
          wch[i]->name,
        //  wch[i]->pcdata->title,
          wch[i]->pcdata->clanexp,
          wch[i]->pcdata->clanstatistics);
          }
          else {
        strip_ansi( wch[i]->name, no_ansi );  
        sprintf( buf , "¡i"HIW"%1d %2d"NOR" "HIW"%6s"NOR" ¡j %s - %s("HIY"%s"NOR") %s\n\r",
          wch[i]->pcdata->clanlevel,
          wch[i]->level,
          class[i],
          wch[i]->pcdata->clantitle,
          wch[i]->cname,
          wch[i]->name,
          wch[i]->pcdata->title);
        //sprintf( buf, "%20s%20s%22s %d\n\r",
        //      cn->name, d->character->pcdata->clantitle, 
        //      d->character->cname, d->character->pcdata->clanstatistics );
          }
          send_to_char( buf, ch );
      }
        }
    }
    sprintf( buf, "%s¦³ "HIW"%d"NOR" ¦WÀ°²³¥¿¦b½u¤W¡C\n\r", 
      "========================================================================"NOR"\n\r",
      members);
    send_to_char(buf, ch);
      return;
  }
    else if ( !str_cmp( arg, "withdraw" ) )
    {
    CHAR_DATA *vch;

    if ( !ch->clan )
    {
      send_to_char( "§A¨Ã¨S¦³¥[¤J¥ô¦óÀ°¬£¡C\n\r", ch );
      return;
    }

    if ( ch->pcdata->clanlevel < 3 )
    {
      send_to_char( "§Aªº¥÷¶q¤£°÷¡C", ch );
      return;     
    }

    for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
      if ( IS_SET( vch->act, ACT_BANKER ) )
        break;

    one_argument( argument, arg );
    if ( is_number( arg ) )
      num = atoi( arg );
    else
    {
      send_to_char( "½Ð©ú½T»¡¥X¤@­Ó¼Æ¦r¡C\n\r", ch );
      return;
    }

    if( num < 1 ) {
        send_to_char("½Ð¦Ü¤Ö»â¤@¤¸¡C\n\r", ch);
        return;
    }
                if( num > 2000000000 ) {
                    send_to_char("¤£¯à»â¥X³o»ò¦h¿ú¡C\n\r", ch);
                    return;
                }
                num += num / 100;

    if ( vch )
    {
      if ( num < ch->clan->bank )
      {
        ch->gold += num;
        ch->clan->bank -= ( num );
        ch->pcdata->clanstatistics -= num;
        act( "$N¦V§A¤@Áù°`»¡¡G³o¬O±zªºª÷¹ô¡A½ÐÂI¦¬¡CÁÂÁÂ±zªº¥úÁ{¡C",
           ch, NULL, vch, TO_CHAR );
        act( "$N¦V$n¤@Áù°`»¡¡G³o¬O±zªºª÷¹ô¡A½ÐÂI¦¬¡CÁÂÁÂ±zªº¥úÁ{¡C",
           ch, NULL, vch, TO_ROOM );
                                sprintf(buf, "chat ¥»¤H¦ÛÀ°®w¤¤´£»â¤F %d ªTª÷¹ô¡C", num);
                                do_clan(ch, buf);
      }
      else
      {
        send_to_char( "§A¥H¬°§A¥[¤Jªº¬O'¤j³°³sÂê»È¦æÀ°'¶Ü¡H\n\r", ch );
        return; 
      }
    }
    else
    {
      send_to_char( "§A§ä¤£¨ì¤HÀ°§A´£¿ú¡C\n\r", ch );
      return;
      }
    }
    else if ( !str_cmp( arg, "deposit" ) )
    {
    CHAR_DATA *vch;

    if ( !ch->clan )
    {
      send_to_char( "§A¨Ã¨S¦³¥[¤J¥ô¦óÀ°¬£¡C\n\r", ch );
      return;
    }

    for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
      if ( IS_SET( vch->act, ACT_BANKER ) )
        break;

    one_argument( argument, arg );
    if ( is_number( arg ) )
      num = atoi( arg );
    else
    {
      send_to_char( "½Ð©ú½T»¡¥X¤@­Ó¼Æ¦r¡C\n\r", ch );
      return;
    }
    if( num < 1 ) {
        send_to_char("½Ð¦Ü¤Ö¦s¤@¤¸¡C\n\r", ch);
        return;
    }
                if( num > 2000000000 ) {
                    send_to_char("¤£¯à¦s¤J³o»ò¦h¿ú¡C\n\r", ch);
                    return;
                }

    if ( vch )
    {
      if ( num < ch->gold )
      {
        ch->gold -= num;
        ch->clan->bank += num;
        ch->pcdata->clanstatistics += num;
        act( "$N¦V§A¤@Áù°`»¡¡GÁÂÁÂ±zªº¥úÁ{¡A¤j³°³sÂê»È¦æ¥Ã»·ºÜ¸Û¬°±zªA°È¡C",
           ch, NULL, vch, TO_CHAR );
        act( "$N¦V$n¤@Áù°`»¡¡GÁÂÁÂ±zªº¥úÁ{¡A¤j³°³sÂê»È¦æ¥Ã»·ºÜ¸Û¬°±zªA°È¡C",
           ch, NULL, vch, TO_ROOM );
      }
      else
      {
        send_to_char( "³á¡I§A¨­¤W¨S¨º»ò¦h¿ú¡C\n\r", ch );
        return;
      }
    }
    else
    {
      send_to_char( "§A§ä¤£¨ì¤HÀ°§A¦s¿ú¡C\n\r", ch );
      return;
    }
    }
    else if ( !str_cmp( arg, "level" ) )
    {
  if( !ch->clan ) return;
    argument = one_argument( argument, arg );
      if ( arg[0] == '\0' || !( victim = get_char_room( ch, arg ) ) || ch == victim || IS_NPC(victim))
      {
        send_to_char( "³o­Ó¤H¤£¦b³o¸Ì°Ú¡H\n\r", ch );
        return;
      }
    one_argument( argument, arg );
    if ( !is_number( arg ) || ( level = atoi( arg ) ) > 4 || level < 0 )
    {
        send_to_char( "µ¥¯Å¥²¶·¬O 0-4 ªº¼Æ¦r¡C\n\r", ch );
        return;
    }
    if ( ch->pcdata->clanlevel < 4 )
    {
      send_to_char( "§A¦bÀ°¤¤ªº¦a¦ì¤£°÷¡C\n\r", ch );
      return;
    }
    if ( victim->clan != ch->clan || IS_NPC( victim ) )
    {
      send_to_char( "§A·F¹ÀºÞ¨ì§OÀ°ªº®a°È¨Æ¡H¡C\n\r", ch );
      return;
    }
                if( ch->pcdata->clanlevel <= victim->pcdata->clanlevel 
                ||  ch->pcdata->clanlevel <= level ) {
                    send_to_char("§A¨S¦³³o»ò¤jªºÅv­­¡C\n\r", ch);
                    return;
                }
    if ( ch->clan->bank < level * level * 10000 )
    {
      send_to_char( "§AªºÀ°¬£¦b»È¦æ¤¤¨S¦³¨¬°÷ªº¦s´Ú¡C", ch );
      return;
    }
    if ( level > victim->pcdata->clanlevel )
    {
      send_to_char( "§A¦bÀ°¬£¸Ìªº¦a¦ì¤É¯Å¤F¡I\n\r", victim );
      send_to_char( "§AÀÂª@¤F¥L(¦o)¦bÀ°¬£¸Ìªº¦a¦ì¡I\n\r", ch );
    }
    else
    {
      send_to_char( "§A¦bÀ°¬£¸Ìªº¦a¦ì­°§C¤F¡I\n\r", victim );
      send_to_char( "§A½Õ­°¥L(¦o)¦bÀ°¬£¸Ìªº¦a¦ì¡I\n\r", ch );
    }
    ch->clan->member[victim->pcdata->clanlevel] -=1;
    ch->clan->member[level] +=1;
    victim->pcdata->clanlevel = level;
    ch->clan->bank -= level * level * 10000;
  }
  else if ( !str_cmp( arg, "title" ) )
  {
  if( !ch->clan ) return;
    argument = one_argument( argument, arg );
    if ( ch->pcdata->clanlevel == 0 )
    {
      send_to_char("§Aªº¥÷¶q¤£°÷¡C\n\r",ch);
      return;
    }
      if ( arg[0] == '\0' || !( victim = get_char_room( ch, arg ) ) || IS_NPC(victim))
      {
        send_to_char( "³o­Ó¤H¤£¦b³o¸Ì°Ú¡H\n\r", ch );
        return;
      }
    if ( !ch->clan || !victim->clan )
    {
      send_to_char( "¦n¤ï¥ý¤JÀ°§a¡H", ch );
      return;
    }
    if ( victim != ch && victim->pcdata->clanlevel >= ch->pcdata->clanlevel )
    {
      send_to_char( "§Aªº¥÷¶q¤£°÷¡C\n\r", ch );
      return;
    }
    if ( ch->clan != victim->clan )
    {
      send_to_char( "§AºÞ¨ä¥LÀ°·|ªº®a°È¨Æ·F¤°»ò¡H\n\r", ch );
      return;
    }
    one_argument( argument, arg );
      if ( argument[0] == '\0' )
      {
    send_to_char( "[33m­n§âºÙ¸¹§ï¦¨¤°»ò¡H[m\n\r", ch );
    return;
      }

      if ( strlen( arg ) > 21 )
      {
    arg[21] = '\0';
    send_to_char(YEL "ºÙ¸¹¤Óªø¤F¡A½Ð·Q¤@­Óµu¤@ÂIªº¡AÅT«G¤@ÂIªº¡C"NOR"\n\r", ch);
      }

      smash_tilde( arg );
      clan_title( victim, arg );
      if( victim != ch ){
      sprintf(buf, "chat ¥»¤H«Å¥¬¡G«Ê %s ¬° %s¡I", victim->short_descr, arg);
      do_clan(ch, buf);
      }
      send_to_char( "[36mOK.[m\n\r", ch );
    }
    else if ( !str_cmp( arg, "status" ) )
    {
      send_to_char( "²{¦³À°¬£¤§¸ê®Æ¡G\n\r", ch );
      send_to_char(
//   1234567890123456789012345678901234567890123456789012345678901234567890
    "[     À°     ¦W      ][ ³Ð ©l ¤H  ][²{ ¥ô À° ¥D][À°²³][ µo®iª¬ªp ][   À°¶O   ]\n\r", ch);
      for( cn = clan_list; cn; cn = cn->next )
      {
        if( IS_IMMORTAL(ch) 
        || ( ch->clan == cn && ch->pcdata->clanlevel > 3 ))
        {
          sprintf( buf, " %-20s %12s %12s  %4d%10s(%2d) %lld\n\r",
            cn->name, cn->founder,cn->leader, clan_members(cn), cc[cn->cclass].name, cn->cclass, cn->bank );
          send_to_char( buf, ch );
        }
        else
        {
          sprintf( buf, " %-20s %12s %12s  %4d %10s  %s\n\r",
            cn->name, cn->founder,cn->leader, clan_members(cn), cc[cn->cclass].name, bank_desc(cn->bank));
          send_to_char( buf, ch );
        }
      }
    if ( clan_war->fighting == 2 )
    {
        sprintf( buf, "%s ©M %s ªº¾Ô§ÐÁÙ¦b¶i¦æ¤¤¡I\n\r",
        clan_war->clan_a->name, clan_war->clan_b->name );
        send_to_char( buf, ch );
        sprintf( buf, "%s ¤è­±¤´µM¦z¥ß¦b¾Ô³õªº¤H­û¦³¡G", clan_war->clan_a->name );
        for( i=0;i<10;i++ )
        if ( clan_war->clan_a_member[i] )
          {
            sprintf( buf1, "%s, ", clan_war->clan_a_member[i]->name );
            strcat( buf, buf1 );
          }
        strcat( buf, "\n\r" );
        send_to_char( buf, ch );
        sprintf( buf, "%s ¤è­±¤´µM¦z¥ß¦b¾Ô³õªº¤H­û¦³¡G", clan_war->clan_b->name );
        for( i=0;i<10;i++ )
          if ( clan_war->clan_b_member[i] )
          {
            sprintf( buf1, "%s, ", clan_war->clan_b_member[i]->name );
            strcat( buf, buf1 );
          }
        strcat( buf, "\n\r" );
        send_to_char( buf, ch );
      }
      return;
    }
    else if ( !str_cmp( arg, "+" ) )
    {
    
    if ( !ch->clan || ch->pcdata->clanlevel < 4 )
    {
      send_to_char( "§Aªº¥÷¶q¤£°÷¡C\n\r", ch );
      return;
    }
    if ( ch->clan->desc )
      strcpy( buf, ch->clan->desc );
    if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH - 200 )
    {
        send_to_char( "[33mÀ°¬£´y­z¤Óªø¤F¡C[m\n\r", ch );
        return;
    }
    smash_tilde( argument );
    strcat( buf, argument );
    strcat( buf, "\n\r" );
    if(ch->clan->desc)
        free_string( ch->clan->desc );
    ch->clan->desc = str_dup( buf );
  }
    else if ( !str_cmp( arg, "clear" ) )
    {
    if ( !ch->clan || ch->pcdata->clanlevel < 4 )
    {
      send_to_char( "§Aªº¥÷¶q¤£°÷¡C\n\r", ch );
      return;
    }
    if( ch->clan->desc )
        free_string( ch->clan->desc );
    ch->clan->desc = NULL;
  }
    else if ( !str_cmp( arg, "history" ) )
    {
      one_argument( argument, arg );
      for( cn = clan_list; cn; cn = cn->next )
        if ( !str_cmp( cn->leader, arg ) )
          break;
    if ( !cn )
      send_to_char( "§ä¤£¨ì³o­ÓÀ°¬£¡C\n\r", ch );
    else
      send_to_char( cn->desc, ch );
    return;
    }
    else if ( !str_cmp( arg, "upgrade" ) )
    {
      if ( !ch->clan || ch->pcdata->clanlevel < 4 )
      {
        send_to_char( "§A¨S¨º­Ó¥÷¶q¡C\n\r", ch );
        return;
      }
      if ( ch->clan->bank < 1000000 )
      {
        send_to_char( "²ÕÂ´ªº°]¤O¤£°÷³á¡I\n\r", ch );
        return;
      }
      for ( i = 0; *(cc[i].name); i++ )
        if ( ch->clan->score < cc[i].exp )
          break;
      if ( ch->clan->cclass < i )
      {
        ch->clan->cclass++;
        ch->clan->bank -= 1000000;
      sprintf( buf, "\n\r"HIY"¸g¹L²³¤Hªø¤[ªº§V¤O¡A"NOR"%s"HIY"ªº¶Õ¤O§ó¶i¤@¨B¦aÂX±i¤F¡I"NOR"\n\r",
        ch->clan->name );
      send_to_all_char( buf );
      sprintf( buf, "%s"HIY"²{¦b¤w¬O¤@­Ó"NOR" %s "HIY"ªº²ÕÂ´¡I"NOR"\n\r", ch->clan->name, cc[ch->clan->cclass].name );
      send_to_all_char( buf );
      send_to_char( "®¥³ß§A¡I\n\r", ch );
      }
      else
      {
        sprintf( buf, "§Aªº²ÕÂ´ÁÙ»Ý­n %d ÂIµo®iÂI¼Æ¤~¯à¤É¯Å¡C\n\r", cc[i].exp - ch->clan->score );
        send_to_char( buf, ch );
        return;
      }
    }
    else if ( !str_cmp( arg, "dispatch" ) )
    {
      CHAR_DATA *mch, *lch;
    argument = one_argument( argument, arg );
    if ( !( mch = get_char_room( ch, arg ) ) || !( lch = get_char_room( ch, argument ) ) )
    {
      send_to_char( "¥Îªk¡Gclan dispatch ¥Íª«¦W »â¾ÉªÌ¦W\n\r", ch );
      return;
    }
    if ( !ch->clan || ch->pcdata->clanlevel < 3 || mch->clan != ch->clan
      || lch->clan != ch->clan || !IS_NPC( mch ) )
    {
      send_to_char( "§AµLªk³o»ò°µ¡C\n\r", ch );
      return;
    }
    if ( mch->level > lch->level + 5 )
    {
      send_to_char( "¹ï¤£°_¡A¤GªÌµ¥¯Å®t¶Z¤Ó¤j¡C\n\r", ch );
      return;
    }
    mch->master = lch;
    act( "$n ±Nµø $N ¬°¥D¤H¡I", mch, NULL, lch, TO_ROOM );
    send_to_char( "¨S°ÝÃD¡C\n\r", ch );
    return;
    }
    else if ( !str_cmp( arg, "clanpk" ) )
    {
      if ( ch->pcdata->clanlevel < 3 )
      {
        send_to_char( "§A¦bÀ°¤ºªº¥÷¶q¤£°÷¡AµLªk¦V§OªºÀ°·|«Å¾Ô¡C\n\r", ch );
        return;
      }
      if ( ch->clan == clan_war->clan_a || ch->clan == clan_war->clan_b )
      {
        send_to_char( "§A¤]¤Ó¦n¾Ô¤F§a¡H\n\r", ch );
        return;
      }
      if ( clan_war->fighting == 2 )
      {
        sprintf( "%s ©M %s ªº¾Ô§ÐÁÙ¦b¶i¦æ¤¤¡A½Ðµy­Ô¡C\n\r", clan_war->clan_a->name,
          clan_war->clan_b->name );
        send_to_char( buf, ch );
        return;
      }
      if ( !IS_SET( ch->act, PLR_REGISTER ) )
      {
        send_to_char( "§A¨S¦³¥[¤J PK¡I\n\r", ch );
        return;
      }
      if ( ch->clan->bank < 500000 )
      {
        send_to_char( "§AªºÀ°·|¤Ó½a¤F¡A¦Ü¤Ö­n¦³¤­¤Q¸Uª÷¹ô¤~¯à§¹¦¨³Æ¾Ô·Ç³Æ¡C\n\r", ch );
        return;
      }
      ch->clan->bank -= 500000;
      if ( clan_war->fighting == 0 )
      {
      clan_war->fighting = 1;
        clan_war->clan_a = ch->clan;
        for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
        {
          if ( !IS_NPC( vch ) && vch->desc && IS_SET( vch->act, PLR_REGISTER )
            && vch->clan == ch->clan )
          {
            clan_war->clan_a_member[clan_war->clan_a_num] = vch;
            vch->pcdata->clanpk = 1;
            act( "$n ¤w¸g¥[¤J³o³õ¾Ô§Ð¡I", vch, NULL, NULL, TO_ROOM );
            send_to_char( "§A¥[¤J¤F³o³õ¾Ô§Ð¡I\n\r", vch );
            clan_war->clan_a_num ++;
            if ( clan_war->clan_a_num == 10 ) break;
          }
            
        }
      }
    else
    {
      clan_war->fighting = 2;
        clan_war->clan_b = ch->clan;
        for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
        {
          if ( !IS_NPC( vch ) && vch->desc && IS_SET( vch->act, PLR_REGISTER )
            && vch->clan == ch->clan )
          {
            clan_war->clan_b_member[clan_war->clan_b_num] = vch;
            vch->pcdata->clanpk = 1;
            act( "$n ¤w¸g¥[¤J³o³õ¾Ô§Ð¡I", vch, NULL, NULL, TO_ROOM );
            send_to_char( "§A¥[¤J¤F³o³õ¾Ô§Ð¡I\n\r", vch );
            clan_war->clan_b_num ++;
            if ( clan_war->clan_b_num == 10 ) break;
          }
            
        }
        sprintf( buf, ""HIY"²V¨P¤Û¥@¨â¤j±jÅv -- "NOR"%s "HIY"©M"NOR" %s "HIY"Ãzµo¿E¯P¾Ô°«¡I"NOR"\n\r",
        clan_war->clan_a->name, clan_war->clan_b->name );
        send_to_all_char( buf );
        sprintf( buf, "%s ¤è­±¥D­n°Ñ¾Ô¤H­û¦³¡G", clan_war->clan_a->name );
        for( i=0;i<10;i++ )
        if ( clan_war->clan_a_member[i] )
          {
            sprintf( buf1, "%s, ", clan_war->clan_a_member[i]->name );
            strcat( buf, buf1 );
          }
        strcat( buf, "\n\r" );
        send_to_all_char( buf );
        sprintf( buf, "%s ¤è­±¥D­n°Ñ¾Ô¤H­û¦³¡G", clan_war->clan_b->name );
        for( i=0;i<10;i++ )
          if ( clan_war->clan_b_member[i] )
          {
            sprintf( buf1, "%s, ", clan_war->clan_b_member[i]->name );
            strcat( buf, buf1 );
          }
        strcat( buf, "\n\r" );
        send_to_all_char( buf );
    }
    }
    else if ( !str_cmp( arg, "cancelpk" ) )
    {
    if ( clan_war->fighting < 2 )
    {
      send_to_char( "®Ú¥»´NÁÙ¨S¶}¥´­ù¡I\n\r", ch );
      return;
    }
      if ( ch->pcdata->clanlevel < 3 )
      {
        send_to_char( "§A¦bÀ°¤ºªº¥÷¶q¤£°÷¿Ñ©M¡C\n\r", ch );
        return;
      }
      if ( ch->clan == clan_war->clan_a )
      {
      sprintf( buf, "¾Ô§Ðµ²§ô¡A%s ¦V %s Ãº¥æ½ß´Ú¡A¥H¨D¤Q¦~¥Í»E±Ð°V¡I\n\r",
        clan_war->clan_a->name, clan_war->clan_b->name );
      send_to_all_char( buf );
      clan_war->clan_b->bank += 1000000;
      for(i=0; i < 10; i++)
      {
        if ( clan_war->clan_a_member[i] )
        {
          clan_war->clan_a_member[i]->pcdata->clanpk = 0;
          clan_war->clan_a_member[i] = NULL;
        }
        if ( clan_war->clan_b_member[i] )
        {
          clan_war->clan_b_member[i]->pcdata->clanpk = 0;
          clan_war->clan_b_member[i] = NULL;
        }
      }
      clan_war->fighting = 0;
      clan_war->clan_a = NULL;
      clan_war->clan_b = NULL;
      clan_war->clan_a_num = 0;
      clan_war->clan_b_num = 0;
      }
      else
      {
      sprintf( buf, "¾Ô§Ðµ²§ô¡A%s ¦V %s Ãº¥æ½ß´Ú¡A¥H¨D¤Q¦~¥Í»E±Ð°V¡I\n\r",
        clan_war->clan_b->name, clan_war->clan_a->name );
      send_to_all_char( buf );
      clan_war->clan_a->bank += 1000000;
      for(i=0; i < 10; i++)
      {
        if ( clan_war->clan_a_member[i] )
        {
          clan_war->clan_a_member[i]->pcdata->clanpk = 0;
          clan_war->clan_a_member[i] = NULL;
        }
        if ( clan_war->clan_b_member[i] )
        {
          clan_war->clan_b_member[i]->pcdata->clanpk = 0;
          clan_war->clan_b_member[i] = NULL;
        }
      }
      clan_war->fighting = 0;
      clan_war->clan_a = NULL;
      clan_war->clan_b = NULL;
      clan_war->clan_a_num = 0;
      clan_war->clan_b_num = 0;
      }
    }
  else if ( !str_cmp(arg, "score"))
        {
            one_argument(argument, arg);
            cn = ch->clan;
            if( arg[0] == '\0' && !cn ){
                send_to_char("½Ð¥Î clan score <À°¥Did> ¨Ó¬d¸ß¡C\n\r", ch);
                return;
            }
            else
            {
                for ( cn = clan_list; cn; cn = cn->next )
                    if ( !str_cmp( arg, cn->leader ) )
                        break;
            }
            if ( !cn )
            {
                send_to_char( "¨S³o­ÓÀ°¬£°Ú¡H\n\r", ch );
                return;
            }
            if ( IS_IMMORTAL( ch ) || ( ch->pcdata->clanlevel > 2 && ch->clan == cn ) )
            {
                AREA_DATA *pArea;
                int vnum;
                for ( pArea = area_first; pArea; pArea = pArea->next )
                    if ( pArea->clan == cn )
                    {
                        for( vnum = pArea->lvnum ; vnum <= pArea->uvnum; ++vnum )
                        {
                            if( get_room_index( vnum ) )
                                rnum++;
                            if( get_mob_index( vnum ) )
                                mnum++;
                            if( get_obj_index( vnum ) )
                                onum++;
                        }
                        break;
                    }
            }
            sprintf( buf, "\n\r********²ÕÂ´¥Ø«eµo®i·§ªp¬°********\n\r" );
            send_to_char( buf, ch );
            sprintf( buf, "²ÕÂ´Á`¤H¼Æ¡G%d\n\r", clan_members(cn) );
            send_to_char( buf, ch );
            sprintf( buf, "Á`¸g¶O¡G%lld\n\r", cn->bank );
            send_to_char( buf, ch );
            sprintf( buf, "²ÕÂ´µ¥¯Å¡G%d, %s\n\r", cn->cclass, cc[cn->cclass].name );
            send_to_char( buf, ch );
            sprintf( buf, "²Ö¿nµo®iÂI¼Æ/¤É¯Å©Ò»ÝÂI¼Æ¡G%d/%d\n\r", cn->score, cc[cn->cclass].exp - cn->score );
            send_to_char( buf, ch );
            sprintf( buf, "¶±¥ÎÂI¼Æ¡G%d\n\r", cn->hire );
            send_to_char( buf, ch );
            sprintf( buf, "»s³yÂI¼Æ¡G%d\n\r", cn->construct );
            send_to_char( buf, ch );
            sprintf( buf, "»â¦a¤j¤p¡G%d\n\r", rnum );
            send_to_char( buf, ch );
            sprintf( buf, "¥Íª«Á`¼Æ¡G%d\n\r", mnum );
            send_to_char( buf, ch );
            sprintf( buf, "ª««~Á`¼Æ¡G%d\n\r", onum );
            send_to_char( buf, ch );
            sprintf( buf, "·ù¤Í¡G\n\r" );
            send_to_char( buf, ch );
            sprintf( buf, "¾Ô§Ð°O¿ý¡G%d ³Ó %d ±Ñ\n\r", cn->warwin, cn->warlose );
            send_to_char( buf, ch );
            return;
        }

/*
    else 
    {
  return;
  if( !ch->clan ) return;
  if( argument[0] == '\0' ) {
      send_to_char("§A­n»¡¤°»ò¡H\n\r", ch);
      return;
  }
  sprintf(buf, "%s¡i%s¡j%s(%s)%s¶¢²áµÛ¡u%s¡v%s\n\r",
    HIY,
    ch->clan->name,
    ch->cname,
    ch->name,
    HIY,
    argument,
    NOR);
  for ( d = descriptor_list; d; d = d->next )
  {
      victim = (d->original) ? d->original : d->character;
      if( !victim ) continue;
      if( victim && !victim->clan ) continue;
      //if( !str_cmp( clan_name, victim->clan->name)){
      if( victim && victim->clan == ch->clan ){
    send_to_char(buf, victim);
      }
  }
  return;
    }
*/
  clan_update( );
  if ( ch )
    do_save( ch, "" );
  if ( victim && victim != ch )
    do_save( victim, "" );
}


void do_clanrecall( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA     *victim,*horse;
    ROOM_INDEX_DATA *location;
    char       buf [ MAX_STRING_LENGTH ];
    int        place;

    if( !ch->in_room || !ch->clan) return;
    
    if( auction_list ) {
  AUC_DATA * auction;
  for( auction = auction_list ; auction; auction = auction->next ){
      if( ch == auction->buyer || ch == auction->owner )
      {
    send_to_char("§A¥¿¦b¶i¦æ©ç½æ¡A¤£¯à¬è¨D¶Ç°e¡C\n\r", ch);
    return;
      }
  }
    }

    act( "[36m$n¬è¨D¶Ç°e¡I[m", ch, NULL, NULL, TO_ROOM );

    if ( IS_SET( ch->in_room->room_flags, ROOM_NO_RECALL )
  || IS_AFFECTED( ch, AFF_CURSE ) )
    {
  send_to_char( "[33m¯«¿ò±ó¤F§A¡C[m\n\r", ch );
  return;
    }

    place = ch->clan->crecall;
    if ( !( location = get_room_index( place ) ) )
    {
  send_to_char( "[33m§A§¹§¹¥þ¥þ¦a°g¸ô¤F¡C[m\n\r", ch );
  return;
    }


    if( !(ch->in_room->area->recall == location->area->recall ))
    {
      send_to_char("§A¶ZÂ÷À°·|¤Ó»·¤F³á¡C\n\r", ch);
  return; 
    }
    
    if ( ch->in_room == location )
  return;

    if ( ( victim = ch->fighting ) )
    {
  int lose;

  if ( number_bits( 1 ) == 0 )
  {
      WAIT_STATE( ch, 4 );
      lose = ( ch->desc ) ? 12 : 25;
      gain_exp( ch, 0 - lose );
      sprintf( buf, "[33m§A¨S¯à¦¨¥\\¡I§A¥¢¥h¤F"HIC"%d"CYN"ÂI¸gÅç­È¡C[m\n\r", lose );
      send_to_char( buf, ch );
      return;
  }

  lose = ( ch->desc ) ? 25 : 50;
  gain_exp( ch, 0 - lose );
  sprintf( buf, "[36m§A±q¾Ô°«¤¤¶Ç°eºM¦^¡I§A¥¢¥h¤F"HIC"%d"CYN"ÂI¸gÅç­È¡C[m\n\r", lose );
  send_to_char( buf, ch );
  stop_fighting( ch, TRUE );
    }

    ch->move /= 2;
    
    if ( ch->in_room->roomprogs && ( ch->in_room->progtypes & ROOM_LEAVE )
  && !IS_SET( ch->act, PLR_WIZINVIS ) )
  rprog_leave_trigger( ch->in_room, ch, 6 );
    
    act( "[36m$n®ø¥¢¤F¡C[m", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "[36m$n©¿µM¦b³o¸Ì¥X²{¡C[m", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
/*
  if ( IS_SET( ch->act, PLR_HORSE ) && ch->desc )
  {
    horse = ch->pcdata->horse;
    stop_fighting( horse, TRUE );
      if ( horse->in_room->roomprogs && ( horse->in_room->progtypes & ROOM_LEAVE )
    && !IS_SET( horse->act, PLR_WIZINVIS ) )
    rprog_leave_trigger( horse->in_room, horse, 6 );
    
      act( "[36m$n®ø¥¢¤F¡C[m", horse, NULL, NULL, TO_ROOM );
      char_from_room( horse );
      char_to_room( horse, location );
      act( "[36m$n©¿µM¦b³o¸Ì¥X²{¡C[m", horse, NULL, NULL, TO_ROOM );
  }
*/

  if ( ch->pcdata && ch->pcdata->horse && ch->pcdata->horse )
  {
    if ( ch->in_room != ( horse = ch->pcdata->horse )->in_room )
    {
      char_from_room( horse );
        char_to_room( horse, location);
      send_to_char("§Aªº¾Ô°¨¦^¨ì¤F§Aªº¨­Ãä¡C\n\r", victim);
    }
  }
    
    dm_effect(ch);

    return;
}
