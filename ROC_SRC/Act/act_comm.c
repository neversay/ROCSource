/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael     *
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
//*  PROGRAM ID  : act_comm.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940606 jye                                    *
//*  PURPOSE     :         ­×§ïNote Read®É·|¦hÅª¤@¦¸note          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         ­×§ïdo_tell,do_reply¼W¥[¡iÅÑÅ¥¡j¥\¯à   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         ­×§ïdo_tell,do_reply¡iÅÑÅ¥¡j           *
//*              :             ¼W¥[all ªº¥\¯à¤ÎNPC¤£±µ¦¬          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940622 jye                                    *
//*  PURPOSE     :         ­×§ïdo_group¼W¥[±Æ§Ç¥\¯à               *
//*              :             ¤ÎProtection¸ê°T                   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940811 Razgriz                                *
//*  PURPOSE     :         ­×§ï stop_rember(),¥[¤J time_stamp §PÂ_*
//*              :         ¥HÁ×§K pch_next->next_in_member ¥i¯à   *
//*              :         ³y¦¨µL½a°j°éªº±¡§Î                     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941006 Razgriz                                *
//*  PURPOSE     :         ­×§ï do_order, mob ¤£¯à snare, untangle*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941006 jye                                    *
//*  PURPOSE     :         ­×§ïdo_order¤£¥i¨Ï¥Î¤½ÀW               *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#if defined( unix )
#include <unistd.h>
#endif
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"


/*
 * Local functions.
 */
bool      is_note_to        args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void      note_attach        args( ( CHAR_DATA *ch ) );
void      note_remove        args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void      talk_channel  args( ( CHAR_DATA *ch, char *argument,
          int channel, const char *verb, const char *color) );
void do_show_group_info ( CHAR_DATA *ch, char *argument );
void chang_char ( CHAR_DATA *ch, CHAR_DATA *victim);

AUCTION_DATA  * auction_list = NULL, *auc_free = NULL;


bool is_note_to( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    if ( !str_cmp( ch->name, pnote->sender ) )
  return TRUE;

    if ( is_name( "all", pnote->to_list ) )
  return TRUE;

    if ( IS_IMMORTAL( ch ) && (   is_name( "immortal",  pnote->to_list )
         || is_name( "immortals", pnote->to_list )
         || is_name( "imm",     pnote->to_list )
         || is_name( "immort",    pnote->to_list ) ) )
  
  return TRUE;

    if ( is_name( ch->name, pnote->to_list ) )
  return TRUE;

    return FALSE;
}



void note_attach( CHAR_DATA *ch )
{
    NOTE_DATA *pnote;

    if ( ch->pnote )
  return;

    if ( !note_free )
    {
  pnote        = alloc_mem( sizeof( *ch->pnote ) );
    }
    else
    {
  pnote        = note_free;
  note_free = note_free->next;
    }

    pnote->next          = NULL;
    pnote->sender  = str_dup( ch->name );
    pnote->csender    = str_dup( ch->short_descr );
    pnote->date          = str_dup( "" );
    pnote->to_list    = str_dup( "" );
    pnote->subject    = str_dup( "" );
    pnote->text          = str_dup( "" );
    ch->pnote        = pnote;
    return;
}



void note_remove( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    FILE      *fp;
    NOTE_DATA *prev;
    char      *to_list;
    char       to_new [ MAX_INPUT_LENGTH ];
    char       to_one [ MAX_INPUT_LENGTH ];

    /*
     * Build a new to_list.
     * Strip out this recipient.
     */
    to_new[0]      = '\0';
    to_list    = pnote->to_list;
    while ( *to_list != '\0' )
    {
  to_list        = one_argument( to_list, to_one );
  if ( to_one[0] != '\0' && str_cmp( ch->name, to_one ) )
  {
      strcat( to_new, " "    );
      strcat( to_new, to_one );
  }
    }

    /*
     * Just a simple recipient removal?
     */
    if ( str_cmp( ch->name, pnote->sender ) && to_new[0] != '\0' )
    {
  free_string( pnote->to_list );
  pnote->to_list = str_dup( to_new + 1 );
  return;
    }

    /*
     * Remove note from linked list.
     */
    if ( pnote == note_list )
    {
  note_list = pnote->next;
    }
    else
    {
  for ( prev = note_list; prev; prev = prev->next )
  {
      if ( prev->next == pnote )
    break;
  }

  if ( !prev )
  {
      bug( "Note_remove: pnote not found.", 0 );
      return;
  }

  prev->next = pnote->next;
    }

    free_string( pnote->text  );
    free_string( pnote->subject );
    free_string( pnote->to_list );
    free_string( pnote->date  );
    free_string( pnote->sender  );
    free_string( pnote->csender );
    pnote->next        = note_free;
    note_free      = pnote;

    /*
     * Rewrite entire list.
     */
    fflush( NULL );
  fclose( fpReserve );
    if ( !( fp = fopen( NOTE_FILE, "w" ) ) )
    {
  perror( NOTE_FILE );
    }
    else
    {
  for ( pnote = note_list; pnote; pnote = pnote->next )
  {
      fprintf( fp, "Sender  %s~\n", pnote->sender     );
      fprintf( fp, "CSender %s~\n", pnote->csender    );
      fprintf( fp, "Date    %s~\n", pnote->date     );
      fprintf( fp, "Stamp   %ld\n", (unsigned long)pnote->date_stamp );
      fprintf( fp, "To    %s~\n", pnote->to_list    );
      fprintf( fp, "Subject %s~\n", pnote->subject    );
      fprintf( fp, "Text\n%s~\n\n", pnote->text     );
  }
  fflush( NULL );
  fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


/* Date stamp idea comes from Alander of ROM */
// note modified by jye 6.06.2005
void do_note( CHAR_DATA *ch, char *argument )
{
  NOTE_DATA *pnote;
  char       buf  [ MAX_STRING_LENGTH   ];
  char       buf1 [ MAX_STRING_LENGTH*7 ];
  char       arg  [ MAX_INPUT_LENGTH    ];
  int        vnum;
  int        anum;

  if( IS_NPC( ch ) )
      return;

  argument = one_argument( argument, arg );
  smash_tilde( argument );

  if( arg[0] == '\0' )
  {
      do_note( ch, "read" );
      return;
  }

  if( !str_cmp( arg, "list" ) )
  {
      vnum  = 0;
      buf1[0] = '\0';
      
      for( pnote = note_list; pnote; pnote = pnote->next )
      {
           if( is_note_to( ch, pnote ) )
           {
               sprintf( buf, "[%3d%s] %s: %s\n\r",
                        vnum,
                        ( pnote->date_stamp > ch->last_note
                         && str_cmp( pnote->sender, ch->name ) ) ? "[1;33mN[m" : " ",
                        pnote->csender, /*pnote->sender,*/ pnote->subject );
               strcat( buf1, buf );
               vnum++;
           }
      }
      send_to_char( buf1, ch );
      return;
  }

  if( !str_cmp( arg, "read" ) )
  {
       bool fAll;
       
       if( !str_cmp( argument, "all" ) )
       {
            fAll = TRUE;
            anum = 0;
            return;
       }
       else if ( argument[0] == '\0' || !str_prefix( argument, "next" ) )
         /* read next unread note */
       {
           vnum    = 0;
           buf1[0] = '\0';
           
           for( pnote = note_list; pnote; pnote = pnote->next )
           {
                if( is_note_to( ch, pnote )
                    && str_cmp( ch->name, pnote->sender )
                    && ch->last_note < pnote->date_stamp )
                {
                    break;
                }
                else
                {
                     if ( is_note_to( ch, pnote ) )
                     vnum++;
                }
           }
           
           if( pnote )
           {
               sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                        vnum,
                        pnote->csender,
                        //pnote->sender,
                        pnote->subject,
                        pnote->date,
                        pnote->to_list );
                        
               strcat( buf1, buf );
               strcat( buf1, pnote->text );
               ch->last_note = UMAX( ch->last_note, pnote->date_stamp );
               send_to_char( buf1, ch );
               return;
           }
           send_to_char( "[33m§A¨S¦³¨SÅª¹Lªº«H¡C[m\n\r", ch );
           return;
       }
       else if ( is_number( argument ) )
       {
           fAll = FALSE;
           anum = atoi( argument );
       }
       else
       {
           send_to_char( "[33m­nÅª­þ¤@¸¹ªº«H¡H[m\n\r", ch );
           return;
       }
       
       vnum  = 0;
       buf1[0] = '\0';
       
       for( pnote = note_list; pnote; pnote = pnote->next )
       {
            if( is_note_to( ch, pnote ) )
            {
                if( vnum == anum || fAll )
                {
                    sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                             vnum,
                             pnote->csender,
                             //pnote->sender,
                             pnote->subject,
                             pnote->date,
                             pnote->to_list );
                             
                    strcat( buf1, buf );
                    strcat( buf1, pnote->text );
                    
                    if( !fAll )
                        send_to_char( buf1, ch );
                    else
                        strcat( buf1, "\n\r" );

                    buf1[0] = '\0';
                    ch->last_note = UMAX( ch->last_note, pnote->date_stamp );
                    
                    if( !fAll )
                        return;
                }
                vnum++;
            }
       }
       
       if( !fAll )
            send_to_char( "[33m¨S¦³³o«Ê«H¡C[m\n\r", ch );
       else
            send_to_char( buf1, ch );
       return;
  }

  if( !str_cmp( arg, "+" ) )
  {
      note_attach( ch );
      strcpy( buf, ch->pnote->text );
      
      if( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH - 200 )
      {
          send_to_char( "[33m«H¤Óªø¤F¡C[m\n\r", ch );
          return;
      }
      
      strcat( buf, argument );
      strcat( buf, "\n\r"   );
      free_string( ch->pnote->text );
      ch->pnote->text = str_dup( buf );
      send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "subject" ) )
  {
      note_attach( ch );
      free_string( ch->pnote->subject );
      ch->pnote->subject = str_dup( argument );
      send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "to" ) )
  {
       note_attach( ch );
       free_string( ch->pnote->to_list );
       ch->pnote->to_list = str_dup( argument );
       send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
       return;
  }

  if( !str_cmp( arg, "clear" ) )
  {
      if( ch->pnote )
      {
          free_string( ch->pnote->text    );
          free_string( ch->pnote->subject );
          free_string( ch->pnote->to_list );
          free_string( ch->pnote->date    );
          free_string( ch->pnote->sender  );
          free_string( ch->pnote->csender );
          ch->pnote->next = note_free;
          note_free = ch->pnote;
          ch->pnote = NULL;
      }
      
      send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "show" ) )
  {
      if( !ch->pnote )
      {
          send_to_char( "[33m§A¨S¦³¥¿¦b¼gªº«H¡C[m\n\r", ch );
          return;
      }
      
      sprintf( buf, "%s: %s\n\rTo: %s\n\r",
               ch->pnote->csender,
               //ch->pnote->sender,
               ch->pnote->subject,
               ch->pnote->to_list );
               
      send_to_char( buf, ch );
      send_to_char( ch->pnote->text, ch );
      return;
  }

  if( !str_cmp( arg, "post" ) || !str_prefix( arg, "send" ) )
  {
      FILE *fp;
      char *strtime;
      
      //CHAR_DATA *rcvr = NULL;
      if( !ch->pnote )
      {
          send_to_char( "[33m§A¨S¦³¥¿¦b¼gªº«H¡C[m\n\r", ch );
          return;
      }
      
      if( !str_cmp( ch->pnote->to_list, "" ) )
      {
          send_to_char("[33m§A±o­n¼g­Ó¦¬¥óªÌ¡]¦W¦r¡Ball¡B©Î¬Oimmortal¡^¡C[m\n\r", ch );
          return;
      }
      
      if( !str_cmp( ch->pnote->subject, "" ) )
      {
          send_to_char( "[33m§A±o­n¼g­Ó¼ÐÃD¡C[m\n\r", ch );
          return;
      }
      
      ch->pnote->next = NULL;
      strtime = ctime( &current_time );
      strtime[strlen(strtime)-1] = '\0';
      free_string( ch->pnote->date );
      ch->pnote->date = str_dup( strtime );
      ch->pnote->date_stamp = current_time;
      
      if( !note_list )
      {
           note_list = ch->pnote;
      }
      else
      {
          for( pnote = note_list; pnote->next; pnote = pnote->next )
               ;
          pnote->next = ch->pnote;
      }
      pnote = ch->pnote;
      ch->pnote = NULL;
      
      fflush( NULL );
      fclose( fpReserve );
      if( !( fp = fopen( NOTE_FILE, "a" ) ) )
      {
          perror( NOTE_FILE );
      }
      else
      {
          fprintf( fp, "Sender  %s~\n", pnote->sender     );
          fprintf( fp, "CSender %s~\n", pnote->csender    );
          fprintf( fp, "Date    %s~\n", pnote->date     );
          fprintf( fp, "Stamp   %ld\n", (unsigned long)pnote->date_stamp );
          fprintf( fp, "To    %s~\n", pnote->to_list    );
          fprintf( fp, "Subject %s~\n", pnote->subject    );
          fprintf( fp, "Text\n%s~\n\n", pnote->text     );
          fflush( NULL );
          fclose( fp );
      }
      fpReserve = fopen( NULL_FILE, "r" );
      
      send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "remove" ) )
  {
      if( !is_number( argument ) )
      {
          send_to_char( "[33m­n¬å­þ¤@¸¹ªº«H¡H[m\n\r", ch );
          return;
      }

      anum = atoi( argument );
      vnum = 0;
      for( pnote = note_list; pnote; pnote = pnote->next )
      {
          if( is_note_to( ch, pnote ) && vnum++ == anum )
          {
              note_remove( ch, pnote );
              send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
              return;
          }
      }
      
      send_to_char( "[33m¨S¦³³o«Ê«H¡C[m\n\r", ch );
      return;
  }

  send_to_char( "[33mÔ£¡H¥´¡uhelp note¡v¬Ý¬Ý«ç»ò¥Î¡C[m\n\r", ch );
  return;
}

/*
 * How to make a string look drunk by Apex <robink@htsa.hva.nl>
 * Modified and enhanced by Maniac from Mythran
 *
 * Modify by Amenda 4/8/01.
 *  1. Fixed buffer reused bug.( c gate when drunken )
 *  2. Chinese words fuzzy also.
 */
char drunkwords [ 255 ];
char *makedrunk( char *string, CHAR_DATA *ch )
{
    char buf[ MAX_STRING_LENGTH ];
    char temp;
    char temp1;
    int  randomnum;
    int  drunkpos;
    int  drunklevel = 0;
    int  pos      = 0;
    int  i;
    bool fChinese;
    fChinese=FALSE;

    strcpy( drunkwords, string );
    /* Check how drunk a person is... */
    if ( !IS_NPC( ch ) )
    {
  if ( ( drunklevel = ch->pcdata->condition[ COND_DRUNK ] ) > 0 )
  {
      do {
        if ( fChinese ) {
    buf[ pos++ ] = *string;
    fChinese=FALSE;
        }
        else {
    temp   = UPPER( *string );
    drunkpos = temp - 'A';

    if ( ( temp >= 'A' ) && ( temp <= 'Z' ) )
    {
        if ( drunklevel > drunk[ drunkpos ].min_drunk_level )
        {
      randomnum =
        number_range( 0, drunk[ drunkpos ].number_of_rep );
      strcpy( &buf[ pos ],
             drunk[ drunkpos ].replacement[ randomnum ] );
      pos +=
        strlen( drunk[ drunkpos ].replacement[ randomnum ] );
        }
        else
      buf[ pos++ ] = *string;
    }
    else
    {
        if ( ( temp >= '0' ) && ( temp <= '9' ) )
        {
      temp       = '0' + number_range( 0, 9 );
      buf[ pos++ ] = temp;
        }
        else {
      buf[ pos++ ] = *string;
      //if ( *string >= '\x80' && *string <= '\xff' )
      if( (fChinese = is_chinese_char( *string, fChinese)) == TRUE )
      {
          //fChinese=TRUE;
          if ( ch->pcdata->condition[ COND_DRUNK ] > number_range( 1, 40 ) )
          {
        drunkpos = number_range( 0, ch->pcdata->condition[ COND_DRUNK ] / 10 );
        temp = *string;
        temp1 = *string + 1;
        i = 0;
        while( i < drunkpos )
        {
          buf[ pos++ ] = temp1;
          buf[ pos++ ] = temp;
          i++;
        }
          }
      }
        }
    }
        }
      }
      while ( *string++ );
      buf[ pos ] = '\0';

      strncpy( drunkwords, buf , 255);
  }
    }
    return( drunkwords );
}

/*
 * Generic channel function.
 */
void talk_channel( CHAR_DATA *ch, char *argument, int channel, const char *verb, const char *color )
{
	DESCRIPTOR_DATA *d;
	char       buf [ MAX_STRING_LENGTH ];
	int        position;
	bool      fRecall = FALSE;

	if ( argument[0] == '\0' )
	{
		sprintf( buf, "[33m­n»¡¤°»ò¡H[m\n\r" );
		send_to_char( buf, ch );
		return;
	}

	if( strlen(argument) > 150 ) {
		send_to_char("¿é¤J¹Lªø.\n\r", ch);
		return;
	}

	if( !ch->in_room ) return;

	if ( !IS_NPC( ch ) && ( IS_SET(ch->act, PLR_SILENCE) || ( IS_SET(ch->act, PLR_HORSE) && ( channel != CHANNEL_CHAT ) ) ) )
	{
		sprintf( buf, "[33m§A¤£¯à¨Ï¥Î %s ÀW¹D¡C[m\n\r", verb );
		send_to_char( buf, ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| ( IS_SET( race_table[ch->race].race_abilities, RACE_MUTE ) && !IS_SET( ch->act, PLR_HORSE ) )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "[33m§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µ¶]¥X¨Ó¡C[m\n\r", ch );
		return;
	}

	REMOVE_BIT( ch->deaf, channel );
	if( channel == CHANNEL_CHAT || channel == CHANNEL_RUMOR || channel == CHANNEL_MUSIC )
		argument = makedrunk( argument, ch );
	// mark by coco  
	// if( channel == CHANNEL_CHAT ) fRecall = TRUE;
	switch ( channel )
	{
		case CHANNEL_YELL:
		case CHANNEL_SHOUT:
			sprintf( buf, "%s§A%s¡G¡u%s%s¡v[m\n\r",color,verb,  argument,color );
			send_to_char( buf, ch );
			sprintf( buf, "%s$n%s%sµÛ¡G¡u$t%s¡v"NOR"",color,color,verb,     color );
			break;
		default:
			sprintf( buf, "%s%s§A»¡¡G¡u%s%s¡v[m\n\r",color,verb,  argument,color );
			send_to_char( buf, ch );
			sprintf( buf, "%s%s$n%s¡G¡u$t%s¡v"NOR"",color,verb, color,    color );
			break;

		case CHANNEL_SYSTEM:
			sprintf( buf, "%s¡i¨t²Î¡j%s ±q %s ³s½u¶i¤J."NOR"", color,ch->name, ch->desc->host );
			break;

		case CHANNEL_IMMTALK:
			sprintf( buf, "%s%s§A»¡¡G$t[m", HIR,verb );
			position  = ch->position;
			ch->position      = POS_STANDING;
			act( buf, ch, argument, NULL, TO_CHAR );
			ch->position      = position;
			sprintf( buf, HIR"%s$n"HIR"¡G¡u$t"HIR"¡v"NOR"",     verb );
			break;
		case CHANNEL_RUMOR:
			fRecall = TRUE;
			sprintf( buf, "%s%s§A»¡¡G¡u%s%s¡v[m\n\r",HIP, verb, argument,HIP );
			send_to_char( buf, ch );
			sprintf( buf, "%s%s¡G¡u$t%s¡v[m",  HIP, verb,  HIP);
			break;
	}

	/*
	* Make the words look drunk if needed...
	*/
	// if( channel == CHANNEL_CHAT || channel == CHANNEL_RUMOR || channel == CHANNEL_MUSIC )
	// argument = makedrunk( argument, ch );

	for ( d = descriptor_list; d; d = d->next )
	{
		CHAR_DATA *och;
		CHAR_DATA *vch;

		och = d->original ? d->original : d->character;
		vch = d->character;
		if( !och || !vch ) continue;

		if ( d->connected == CON_PLAYING
			&& vch != ch
			&& !IS_SET( och->deaf, channel )
			&& och->in_room
			&& vch->in_room
			&& !IS_SET( och->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
		{
			if( fRecall && !IS_IMMORTAL(ch) && !IS_IMMORTAL( och ) && vch->in_room->area->recall != ch->in_room->area->recall )
				continue;
			if ( channel == CHANNEL_IMMTALK && !IS_IMMORTAL( och ) )
				continue;
			if( channel == CHANNEL_SYSTEM   && !IS_IMMORTAL( och ))
				continue;
			if( channel == CHANNEL_HEROTALK && !IS_HERO(och) )
				continue;
			if( channel == CHANNEL_SYSTEM   && !can_see( och, ch))
				continue;
			if( channel == CHANNEL_CHAT     && och->level < 3 )
				continue;
			if ( channel == CHANNEL_YELL    && vch->in_room->area != ch->in_room->area )
				continue;
			// ­ì bid ³Û»ù¥u¦b auc room ¯à¬Ý±o¨ì, ÅÜ§ó¬°»P chat ¬Û¦P 2020/11/08
			//if ( channel == CHANNEL_BET && !IS_SET( vch->in_room->room_flags, ROOM_AUCTION))
			if ( channel == CHANNEL_BET     && och->level < 3 )
				continue;

			position = vch->position;

			if ( channel != CHANNEL_SHOUT && channel != CHANNEL_YELL )
				vch->position = POS_STANDING;
			act( buf, ch, argument, vch, TO_VICT );

			if ( channel == CHANNEL_RUMOR
				&& ( IS_IMMORTAL( vch )
				|| ( !IS_NPC( ch ) && vch->level > ch->level ) ) )
				act( HIG"§Aµo²{¬O"NOR"$n"HIG"¦b´²¼½ÁÁ¨¥¡C"NOR, ch, NULL, vch, TO_VICT );  // 2021/10/04 ¨Ï¤Hª«¦â½X¤£¼vÅT¨ì«áÄò¦r¦ê
			vch->position = position;
		}
	}
	return;
}



void do_auction( CHAR_DATA *ch, char *argument )
{
    //talk_channel( ch, argument, CHANNEL_AUCTION, "©ç½æ",HIY );
    
    char  arg1 [ MAX_INPUT_LENGTH ];
    char  arg2 [ MAX_INPUT_LENGTH ];
    char  buf  [ MAX_STRING_LENGTH ];
    OBJ_DATA  *obj = NULL;
    int   price = 0;
    AUC_DATA  *auction, *auc_next;

    if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_HORSE ) || !ch->in_room)
  return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if( !str_cmp( arg1, "id" ) )
	{
		if( !auction_list )
		{
			send_to_char(CYN"¥Ø«e¨S¦³ªF¦è¦b©ç½æ¡I"NOR"\n\r", ch);
			return;
		}
		for(auction = auction_list ; auction; auction = auction->next ){
			if( is_pfxname(arg2, auction->obj->name)) break;
		}
		if( !auction ) {
			send_to_char(CYN"§ä¤£¨ì³o¼ËªF¦è¡I"NOR"\n\r", ch);
			return;
		}
		obj = auction->obj;

		if( ch == auction->owner )
		return;

		if( ch->gold < 2000 )
		{
			send_to_char(CYN"§A¥I¤£°_Å²©w¶O¥Î¡I"NOR"\n\r", ch );
			return;
		}
		ch->gold = ch->gold - 2000;

		obj_cast_spell(skill_lookup("identify"), ch->level+6, ch, NULL, obj);
		return;
	}

	if( !IS_SET(ch->in_room->room_flags, ROOM_AUCTION))
	{
		send_to_char(CYN"³o¸Ì¤£¯à¶i¦æ©ç½æ¡I"NOR"\n\r", ch);
		return;
	}else if(arg1[0] == '\0'){
		send_to_char(CYN"§A­n½æ¤°»ò¡H"NOR"\n\r", ch);
		return;
	}

    if( auction_list && arg1[0] != '\0' && str_cmp( arg1, "id") 
      && str_cmp( arg1, "cancel") )
    {
        if( is_qmark(ch, "auction_cancel") ){
            send_to_char("§A¥Ñ©ó¤§«e¨ú®ø©ç½æ¡A¦]¦¹»Ý­n¤@¬q®É¶¡¤~¯à¦A¶i¦æ©ç½æ¡C\n\r", ch);
            return;
        }

  if( !( obj = get_obj_carry( ch, arg1 )))
  {
      if( (obj = get_obj_wear( ch, arg1 )))
    send_to_char("§A±o¥ý²æ¤U¥¦¡C\n\r", ch);
      else
    send_to_char("§A¨­¤W¨S¦³¨º¼ËªF¦è¡C\n\r", ch);
      return;
  }


  for( auction = auction_list ; auction ; auction = auction->next ){
      if( !str_cmp(obj->name, auction->obj->name )){
    send_to_char("©êºp¡A¥Ø«e¦³¤@³õ¦P¦Wªº©ç½æ¥¿¦b¶i¦æ¤¤¡A¤U¦¸½Ð¦­¡C\n\r", ch );
    return;
      }
      if( ch == auction->owner ) {
    send_to_char("§A¤w¸g¦³¤@³õ©ç½æ¦b¶i¦æ¤F¡C\n\r", ch);
    return;
      }
  }
    }
    if( arg1[0] == '\0' )
    {
      if( auction_list )
  {
      send_to_char("­n©ç½æ¤°»ò¡H\n\r", ch);
      return;
  }

  for( auction = auction_list ;auction; auction = auction->next ){
      obj = auction->obj;
      sprintf( buf, CYN"¥Ø«e¥¿¦b©ç½æªºª««~¬O¡G%s,\n\r§Î¦¡¡G%s(%s),\n\r¯S©Ê¡G%s,\n\rµ¥¯Å¡G%d,\n\r­«¶q¬O %d ¤½¤ç, ¥Ø«eªº»ù¦ì¬O %d ªTª÷¹ô¡C"NOR"\n\r",
        obj->short_descr,
        c_item_type_name( obj ),
        item_type_name( obj ),
        c_extra_bit_name( obj->extra_flags ),
        obj->level,
        obj->weight,
        auction->price
       );
      send_to_char( buf, ch );
  }
      return;
    }
    if( !str_cmp( arg1, "cancel") )
    {
  if( !auction_list ) {
      send_to_char("¥Ø«e¨S¦³©ç½æ¥¿¦b¶i¦æ¡I\n\r", ch);
      return;
  }
  for( auction = auction_list; auction; auction = auction->next ){
      if( ch == auction->owner ) break;
      if( IS_IMMORTAL(ch) && is_pfxname( arg2, auction->obj->name )) break;
  }
  if( !auction )
  {
      send_to_char(CYN"¥u¦³½æ¤è¥i¥H¤¤¤î©ç½æ¡I"NOR"\n\r", ch);
      return;
  }
  if( ( !auction->buyer && auction->time < 1 ) || (ch != auction->owner && get_trust( ch ) < L_JUN) )
  {
      send_to_char(CYN"§A¤£¯à¤¤¤î©ç½æ¡I"NOR"\n\r", ch);
      return;
  }

  if( !auction->buyer )
    ;
  else if( auction->price && auction->owner->gold < auction->price / 10 )
  {
    send_to_char(CYN"§Aªº¿ú¤£°÷¤¤¤î©ç½æ¡I"NOR"\n\r",ch);
    return;
  }
  else
    auction->owner->gold -= auction->price / 10;

  sprintf(buf, "¥»¤H¨M©w¡A¨ú®ø%sªº©ç½æ¡I", auction->obj->short_descr );
  talk_channel( ch, buf, CHANNEL_AUCTION, "¡i©ç½æ¡j",HIY );
  obj_to_char( auction->obj, auction->owner );
  if( auction == auction_list ) {
      auction_list = auction_list->next;
  }
  else {
      for( auc_next = auction_list ; auc_next; auc_next = auc_next->next ){
    if( auc_next->next == auction ){
        auc_next->next = auction->next;
        auc_next = auction->next;
        break;
    }
      }
  }
        if( auction->buyer ) {
            auction->buyer->gold += auction->price;
        }
        if( auction->buyer && !IS_IMMORTAL(ch)) {
            sprintf(arg2, "self auction_cancel 6 0 0 0 0 1");
            do_add_qmark(auction->owner, arg2);
        }
  free_auction_data(auction);
  return;
    }

    if( !IS_SET(ch->in_room->room_flags, ROOM_AUCTION))
    {
      send_to_char(CYN"³o¸Ì¤£¯à¶i¦æ©ç½æ¡I"NOR"\n\r", ch);
      return;
    }
    if( !( obj = get_obj_carry( ch, arg1 )))
    {
  if( (obj = get_obj_wear( ch, arg1 )))
      send_to_char("§A±o¥ý²æ¤U¥¦¡C\n\r", ch);
  else
      send_to_char("§A¨­¤W¨S¦³¨º¼ËªF¦è¡C\n\r", ch);
  return;
    }

    if( !obj ) {
  bug("do_auction; No OBJ!!", 0 );
  return;
    }

    if( !can_drop_obj( ch, obj ))
    {
      send_to_char("§AµLªk©ñ¶}³o¼ËªF¦è¡C\n\r", ch );
      return;
    }

    if( arg2[0] == '\0' || !is_number( arg2 ))
    {
  price = obj->cost;
        if( price <= 0 ) price = 1000;
    }
    else
    {
  price = atoi( arg2 );
  if( price < obj->cost ) price = obj->cost;
        if( price > 2000000000 ) price = 2000000000;
    }

    auction = new_auction_data();

    auction->obj = obj;
    auction->owner = ch;
    auction->buyer = NULL;
    auction->price = price;
    auction->time = 0;
    auction->recall = ch->in_room->area->recall;
    auction->next = auction_list;
    auction_list = auction;
    obj_from_char( obj );

    sprintf( buf,"%s¡A°_»ù %d ªTª÷¹ô¡AÅwªï¦U¬ÉÄv¼Ð¡A¦aÂI¦b%s¡C",
        obj->short_descr,
        price,
        ch->in_room->name
    );
    talk_channel( ch, buf, CHANNEL_AUCTION, "¡i©ç½æ¡j",HIY );
    return;
}



void do_chat( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_SUMMONED )
    || IS_AFFECTED( ch, AFF_CHARM ) ) )
      return;

    if ( IS_NPC( ch ) )
        return;
  
    if ( ch->level < 3 )
    {
      send_to_char( "§A¦Ü¤Ö­n¹F¨ì²Ä¤T¯Å¤~¯à¶¢²á¡A¸Õ¸Õ question §a¡C\n\r", ch );
      return; 
    }
    talk_channel( ch, argument, CHANNEL_CHAT, "¡i¶¢²á¡j",HIC );
    return;
}


void do_bet( CHAR_DATA *ch, char *argument )
{
	//talk_channel( ch, argument, CHANNEL_AUCTION, "³Û»ù",WHT );
    
	char  arg1 [ MAX_INPUT_LENGTH ];
	char  arg2 [ MAX_INPUT_LENGTH ];
	char  buf  [ MAX_STRING_LENGTH ];
	OBJ_DATA  *obj;
	int   price = 0;
	AUC_DATA  *auction;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	if( IS_NPC( ch ) || IS_SET( ch->act, PLR_HORSE ) || !ch->in_room) return;

	if( !auction_list )
	{
		send_to_char(CYN"¥Ø«e¨Ã¨S¦³ª««~¦b©ç½æ¤¤¡C"NOR"\n\r", ch);
		return;
	}

	if( arg1[0] == '\0')
	{
		for( auction = auction_list ; auction ; auction = auction->next )
		{
			if( ch == auction->owner )
			{
				if( auction->buyer )
					sprintf( buf, CYN"%s¥Ø«eªº»ù¦ì¬O %d ªTª÷¹ô¡C"NOR"\n\r",
								auction->obj->short_descr,
								auction->price );
				else
					sprintf( buf, CYN"%s¥Ø«e¨S¦³¥ô¦ó¤HÄ@·N¥X»ù¡A©³»ù¬O %d ªTª÷¹ô¡C"NOR"\n\r",
								auction->obj->short_descr,
								auction->price);

				send_to_char( buf, ch );
			}
			else
			{
				sprintf( buf, "¥Ø«e¥¿¦b©ç½æ %s, »ù¦ì¬O %d ªTª÷¹ô¡C",
								auction->obj->short_descr,
								auction->price);
				if( auction->buyer )
				{
					strcat( buf, "¥X»ù³Ì°ªªº¬O" );
					strcat( buf, ( ch == auction->buyer ) ? "§A" : auction->buyer->name);
					strcat( buf, "¡C");
				}
				strcat( buf, "\n\r" );
				send_to_char( buf, ch );
			}
		}
		return;
	}
    
	for( auction = auction_list ;auction; auction = auction->next ) {
		if( is_pfxname( arg1, auction->obj->name ) )
			break;
	}
    
	if( !auction ) {
		send_to_char(CYN"¨S¦³³o¤@Ãþªºª««~¦b©ç½æ¡C"NOR"\n\r", ch);
		return;
    }


	obj = auction->obj;
	if( !can_see_obj( ch, obj ))
	{
		send_to_char(CYN"§A¬Ý¤£¨ì³o¼Ëª««~¡C"NOR"\n\r", ch);
		return;
	}
	if( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ))
	{
		send_to_char(CYN"§AµLªk¦A®³§ó¦hªF¦è¤F¡C"NOR"\n\r", ch );
		return;
	}
	if( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ))
	{
		send_to_char(CYN"§A¦A¤]®³¤£°Ê¤F¡C"NOR"\n\r", ch);
		return;
	}


	if( ch == auction->buyer )
	{
		send_to_char("¥Ø«e¥X³Ì°ª»ùªº¤w¸g¬O§A¤F¡A¤£»Ý­n¦A¥s»ù¡C\n\r", ch);
		return;
	}

    if( !IS_SET(ch->in_room->room_flags, ROOM_AUCTION))
    {
		send_to_char(CYN"³o¸Ì¤£¯à¶i¦æ©ç½æ¡I"NOR"\n\r", ch);
		return;
    }

	if( ch->in_room->area->recall != auction->recall ) {
		send_to_char(CYN"§A¥¿³B¦b¤£¦Pªº®ÉªÅ¤§¤¤¡AµLªk¶i¦æ¥s»ù¡I"NOR"\n\r", ch);
		return;
	}

	if( !is_number( arg2 ))
	{
		send_to_char("¥X»ù½Ð¥Î¡u¼Æ¦r¡v¡I\n\r", ch);
		return;
	}
	price = atoi( arg2 );
	if( price > ch->gold )
	{
		send_to_char("§A¨S¦³³o»ò¦h¿ú¡C\n\r", ch);
		return;
	}
	if( price > 2100000000 ) price = 2100000000;
	if( price < (auction->price + auction->price / 20 ))
	{
		send_to_char("§A¤Ó¤p®ð¤F¡A¦A¥X¦h¤@ÂI§a¡C\n\r", ch);
		return;
	}
	ch->gold -= price;
	if( auction->buyer ) {
		auction->buyer->gold += auction->price;
	}
	if( (price == auction->price && !auction->buyer) || price > auction->price )
	{
		auction->buyer = ch;
		auction->price = price;
		auction->time = 0;
		sprintf( buf, "§Ú¥X»ù %d ªTª÷¹ôÁÊ¶R %s¡C", price, auction->obj->short_descr);
	}
	talk_channel( ch, buf, CHANNEL_BET, "¡i©ç½æ¡j",HIC );
	return;
}

/*
 * Alander's new channels.
 */
void do_music( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_MUSIC, "¡i¤ß±¡ÂI¼½¡j",WHT );
    return;
}



void do_question( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
      return;

    talk_channel( ch, argument, CHANNEL_QUESTION, "¡i·s¤âµo°Ý¡j" ,HIG);
    return;
}



void do_answer( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
      return;

    talk_channel( ch, argument, CHANNEL_QUESTION, "¡i¦Ñ¤â¦^µª¡j" ,HIG);
    return;
}



void do_shout( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_SHOUT, "³Û¥s",HIY );
    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
  return;
    if ( !IS_NPC(ch) && ch->fighting && !IS_NPC(ch->fighting))
  return;
    if( !IS_NPC(ch) )
        WAIT_STATE( ch, 12 );
    return;
}



void do_yell( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_YELL, "§o³Û",HIY );
    return;
}


void do_herotalk( CHAR_DATA *ch, char *argument )
{
    if( IS_NPC(ch) ) return;

    talk_channel( ch, argument, CHANNEL_HEROTALK, "¡i­^¶¯¯ùÀ]¡j" , HIG);
    return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
  
    rch = get_char( ch );

    if ( !authorized( rch, "immtalk" ) )
  return;

    talk_channel( ch, argument, CHANNEL_IMMTALK, "¡i¯«¿Ù¡j" ,HIR);
    return;
}

void do_rumor( CHAR_DATA *ch, char *argument )
{
  
    if ( ch->level < 10 )
    {
      send_to_char( "§A¦Ü¤Ö­n¹F¨ì²Ä¤Q¯Å¤~¦³¤HªÖ¬Û«H§AªºÁÁ¨¥¡C\n\r", ch );
      return; 
    }
    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
  return;
    talk_channel( ch, argument, CHANNEL_RUMOR, "¡i¤Û¥@ÁÁ¶Ç¡j" ,HIP);
    return;
}


void do_say( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m­n»¡¤°»ò¡H[m\n\r", ch );
  return;
    }
    if ( IS_AFFECTED( ch, AFF_MUTE )
  || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
  || IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
  send_to_char( "[33m§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µ¶]¥X¨Ó¡C[m\n\r", ch );
  return;
    }

    argument = makedrunk( argument, ch );

    act( "$n"HIG"»¡µÛ¡u$T¡v"NOR, ch, NULL, argument, TO_ROOM );
    act( HIG "§A»¡¡u$T¡v"NOR, ch, NULL, argument, TO_CHAR );

    mprog_speech_trigger( argument, ch );

    return;
}


// do_tell modified by jye 6.13.2005
void do_tell( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *d;
  CHAR_DATA    *to;
  char       arg [ MAX_INPUT_LENGTH ];
  char       buf [ MAX_STRING_LENGTH ];
  int        position;
  
  if( IS_NPC( ch ) 
      && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
      return;

  if( IS_AFFECTED( ch, AFF_MUTE )
      || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
      || IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
  {
      send_to_char( "[33m§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µ¶]¥X¨Ó¡C[m\n\r", ch );
      return;
  }

  one_argument( argument, arg );

  /*
   * Can tell to PC's anywhere, but NPC's only in same room.
   * -- Furey
   */
  if( !( victim = get_char_world( ch, arg ) )
      || ( IS_NPC( victim ) && victim->in_room != ch->in_room ) 
      || !victim->in_room )
  {
      send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
      return;
  }

  if( victim == ch ) 
  {
      send_to_char("§A¹ïµÛ¦Û¤v³ä³ä¦Û»y¡C\n\r", ch);
      return;
  }

  if( ( !IS_NPC( ch ) && ( IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE) 
           || IS_SET( ch->act, PLR_NO_TELL ) ) )
           || IS_SET( victim->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
  {
      send_to_char( "[33m§AªºÁn­µ¶Ç¤£¹L¥h¡C[m\n\r", ch );
      return;
  }

  if( !IS_NPC(victim) && !victim->desc && victim->pcdata->horse )
  {
      victim->reply  = ch;
      victim = victim->pcdata->horse;
  }


  if( !victim->desc )
  {
      act( "[33m$NÂ_½u¤F¡C[m", ch, 0, victim, TO_CHAR );
      return;
  }

  argument = one_argument( argument, arg );

  if( arg[0] == '\0' || argument[0] == '\0' )
  {
      send_to_char( "[33m­n§i¶D½Ö¤°»ò¨Æ¡H[m\n\r", ch );
      return;
  }

  if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
  {
      act( "[33m$EÅ¥¤£¨ì§AªºÁn­µ¡C[m", ch, 0, victim, TO_CHAR );
      return;
  }

  argument = makedrunk( argument, ch );

  act( ""HIY"§A§i¶D $N¡u$t¡v"NOR"", ch, argument, victim, TO_CHAR );
  position        = victim->position;
  victim->position      = POS_STANDING;
  act( "$n"HIY"§i¶D§A¡u$t¡v"NOR"", ch, argument, victim, TO_VICT );
  victim->position      = position;
  victim->reply  = ch;
  
	if( IS_SET( victim->act, PLR_AFK ) )
		act( "[33m§A¤]ª¾¹Dªº¹À¡A$E²{¦b¤£¦b¹q¸£«e­±¡C[m", ch, NULL, victim, TO_CHAR );
      
	for ( d = descriptor_list; d; d = d->next ) 
	{
		to = (d->original) ? d->original : d->character;
      
		if( !to ) continue;
		if( !IS_IMMORTAL( to ) 
			|| IS_IMMORTAL( ch )
			|| IS_NPC( ch ))
			continue;
		/*
			Á×§K¶Ç¤J str_cmp «á²£¥Í¤j¶qªº null bstr 2022/01/05 

			if( str_cmp( ch->name , to->tell_channel )
				&& str_cmp( "all" , to->tell_channel ) )
				continue;
		*/
		if ( to->tell_channel == NULL )
			continue;
		if( !str_cmp( "all" , to->tell_channel ) )
			;
		else if( !str_cmp( ch->name , to->tell_channel ) )
			;
		else
			continue;
		if( to->desc )
		{
			sprintf(buf, HIB"¡iÅÑÅ¥¡j%s"HIB"¶Ç°e°T®§µ¹%s"HIB"¡u%s¡v"NOR"\n\r",ch->short_descr,victim->short_descr,argument);
			send_to_char( buf, to );
		}
	}

  return;
}


// do_reply modified by jye 6.13.2005
void do_reply( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *d;
  CHAR_DATA    *to;
  char       buf [ MAX_STRING_LENGTH ];
  int        position;
  
  if( IS_AFFECTED( ch, AFF_MUTE )
      || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
      || IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
  {
      send_to_char( "[33m§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µ¶]¥X¨Ó¡C[m\n\r", ch );
      return;
  }

  if( !( victim = ch->reply ) )
  {
      send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
      return;
  }

  if( ( !IS_NPC( ch ) 
        && ( IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE)
            || IS_SET( ch->act, PLR_NO_TELL ) ) )
            || IS_SET( victim->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
  {
      send_to_char( "[33m§AªºÁn­µ¶Ç¤£¹L¥h¡C[m\n\r", ch );
      return;
  }

  if( argument[0] == '\0' )
  {
      send_to_char( "[33m­n¦^À³¤°»ò¡H[m\n\r", ch );
      return;
  }

  if( !IS_NPC(victim) && !victim->desc && victim->pcdata->horse )
  {
      victim->reply  = ch;
      victim = victim->pcdata->horse;
  }

  if( !victim->desc )
  {
      act( "[33m$NÂ_½u¤F¡C[m", ch, 0, victim, TO_CHAR );
      return;
  }

  if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
  {
      act( "[33m$EÅ¥¤£¨ì§AªºÁn­µ¡C[m", ch, 0, victim, TO_CHAR );
      return;
  }

  argument = makedrunk( argument, ch );

  act( ""HIY"§A§i¶D$N¡u$t¡v"NOR"",  ch, argument, victim, TO_CHAR );
  position        = victim->position;
  victim->position      = POS_STANDING;
  act( HIY"$n§i¶D§A¡u$t¡v"NOR"", ch, argument, victim, TO_VICT );
  victim->position      = position;
  victim->reply  = ch;

  if( IS_SET( victim->act, PLR_AFK ) )
      act( "[33m§A¤]ª¾¹Dªº¹À¡A$E²{¦b¤£¦b¹q¸£«e­±¡C[m", ch, NULL, victim, TO_CHAR );
        
	for ( d = descriptor_list; d; d = d->next ) 
	{
		to = (d->original) ? d->original : d->character;
      
		if( !to ) continue;
		if( !IS_IMMORTAL( to ) 
			|| IS_IMMORTAL( victim )
			|| IS_NPC( victim ))
			continue;
		/*
			Á×§K¶Ç¤J str_cmp «á²£¥Í¤j¶qªº null bstr 2022/01/05 

			if( str_cmp( victim->name , to->tell_channel )
				&& str_cmp( "all" , to->tell_channel ))
				continue;
		*/
		if ( to->tell_channel == NULL )
			continue;
		if( !str_cmp( "all" , to->tell_channel ) )
			;
		else if( !str_cmp( victim->name , to->tell_channel ) )
			;
		else
			continue;
		if( to->desc )
		{
			sprintf(buf, HIB"¡iÅÑÅ¥¡j%s"HIB"¶Ç°e°T®§µ¹%s"HIB"¡u%s¡v"NOR"\n\r",ch->short_descr,victim->short_descr,argument);
			send_to_char( buf, to );
		}
	}

  return;
}



void do_emote( CHAR_DATA *ch, char *argument )
{
    char  buf [ MAX_STRING_LENGTH ];
    char *plast;

    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
    {
  send_to_char( "[33m§A¨S¿ìªkªí¹F§Aªº±¡·P¡C[m\n\r", ch );
  return;
    }

    if ( argument[0] == '\0' )
    {
  send_to_char( "[33mªí¹F¤°»ò±¡·P¡H[m\n\r", ch );
  return;
    }

    for ( plast = argument; *plast != '\0'; plast++ )
  ;

    strcpy( buf, argument );
    if ( isalpha( plast[-1] ) )
  strcat( buf, "¡C" );

	act( "$n"NOR"$T¡C[m", ch, NULL, buf, TO_ROOM ); // 2021/10/04  ¨Ï¹w³] emote µLÃC¦â¥B¦W¦rÃC¦â¤£¨ü¼vÅT
	act( "$n"NOR"$T¡C[m", ch, NULL, buf, TO_CHAR );
	//act( "[36m$n$T¡C[m", ch, NULL, buf, TO_ROOM );
	//act( "[36m$n$T¡C[m", ch, NULL, buf, TO_CHAR );
    return;
}



/*
 * All the posing stuff.
 */
struct        pose_table_type
{
    char * message[ 2*MAX_CLASS ];
};

const      struct    pose_table_type    pose_table      []        =
{
    {
  {
      "[36m§A´²µo¥X¯à¶q¡C[m",
      "[36m$n´²µo¥X¯à¶q¡C[m",
      "[36m§A·P¨ì«Ü¸t¼ä¡C[m",
      "[36m$n¬Ý°_¨Ó«Ü¸t¼ä¡C[m",
      "[36m§Aª±¤F­Ó¥´µPªº¤p§Þ­Ç¡C[m",
      "[36m$nª±¤F­Ó¥´µPªº¤p§Þ­Ç¡C[m",
      "[36m§A²{¥X§Aªº¤j¦Ù¦×¡C[m",
      "[36m$n²{¥X$s¤j¦Ù¦×¡C[m",
      "[36m§A­n¥ÎºÐ¥P°±¤U¨Ó¶Ü¡H[m",
      "[36m¤Ó¦n¤F¡A$n¤S¦bª±$sºÐ¥P¤F¡C[m"
  }
    },

    {
  {
      "[36m§AÅÜ¦¨¤@°¦½¹½º¡AµM«á¤SÅÜ¦^§A¥¿±`ªº¼Ë¤l¡C[m",
      "[36m$nÅÜ¦¨¤@°¦½¹½º¡AµM«á¤SÅÜ¦^$s¥¿±`¼Ë¤l¡C[m",
      "[36m§AµL°Ê©ó°J¦a§â°sÅÜ¦¨¤ô¡C[m",
      "[36m$nµL°Ê©ó°J¦a§â°sÅÜ¦¨¤ô¡C[m",
      "[36m§A¥æ´À¦a°Ê¤F°Ê§Aªº¦Õ¦·¡C[m",
      "[36m$n¥æ´À¦a°Ê¤F°Ê$s¦Õ¦·¡C[m",
      "[36m§A¥Î§Aªº¤â«ü§â°íªG®º¶}¡C[m",
      "[36m$n¥Î$sªº¤â«ü§â°íªG®º¶}¡C[m",
      "[36m§AÅª¤F¤j®aªº¤º¤ß...µM«á¹½´c±oµo§Ý¡C[m",
      "[36m$nÅª¤F§Aªº¤º¤ß...³é¡A§A¼Z¤J¤F¨¸¹D¡I[m"
  }
    },

    {
  {
      "[36mÂÅ¦âªº°{¥ú±q§Aªº«ü¶¡«_¥X¡C[m",
      "[36mÂÅ¦âªº°{¥ú±q$nªº«ü¶¡«_¥X¡C[m",
      "[36m§AªºÀY¤W¥X²{¤F¤@¹D¥úÀô¡C[m",
      "[36m$nªºÀY¤W¥X²{¤F¤@¹D¥úÀô¡C[m",
      "[36m§A«Ü¨³³t¦aµ¹¦Û¤v¥´¤F­Ó½¹½ºµ²¡C[m",
      "[36m$n«Ü¨³³t¦aµ¹$m¦Û¤v¥´¤F­Ó½¹½ºµ²¡C[m",
      "[36mYou grizzle your teeth and look mean.[m",
      "[36m$n grizzles $s teeth and looks mean.[m",
      "[36m§A§â§Aªº¾Ç®Õ¥þ¶Ô¼ú²{µ¹¤j®a¬Ý¡C[m",
      "[36m$nªº¾Ç®Õ¥þ¶Ô¼ú¤Þ¤£°_§Aªºª`·N¡C¹É¡C[m"
  }
    },

    {
  {
      "[36m¬õ¦âªº¥ú¨~¦b§Aªº²´·ú¸Ì»R°Ê¡C[m",
      "[36m¬õ¦âªº¥ú¨~¦b$nªº²´·ú¸Ì»R°Ê¡C[m",
      "[36m§A®Ô»wµÛ´¼¼z¤§¨¥¡C[m",
      "[36m$n®Ô»wµÛ´¼¼z¤§¨¥¡C[m",
      "[36m§A¥Î¤P­º¡BÄ«ªG©M²´²y­A§âÀ¸¡C[m",
      "[36m$n¥Î¤P­º¡BÄ«ªG©M²´²y­A§âÀ¸¡C[m",
      "[36m§A¥´¤F¥´§AªºÀY¡AµM«áÂà¤FÂà§Aªº²´·ú¡C[m",
      "[36m$n¥´¤F¥´$sÀY¡AµM«áÂà¤FÂà$s²´·ú¡C[m",
      "[36m¤@¹Î°­¤õ¸òµÛ§Aªº©ì¾c¨Ó¤F¡C[m",
      "[36m¤@¹Î°­¤õ¸òµÛ$nªºÉæ¾c¨Ó¤F¡C[m"
  }
    },

    {
  {
      "[36m¤@­Ó¦³ºñ¦âÂH²Gªº©Çª«¦b§A­±«e¥X²{¡A¦V§AÁù°`¡C[m",
      "[36m¤@­Ó¦³ºñ¦âÂH²Gªº©Çª«¦b$n­±«e¥X²{¡A¦V$nÁù°`¡C[m",
      "[36m¦b¤¾ªøªºÃ«§i¤§¤U¡A§A¯B¤F°_¨Ó¡C[m",
      "[36m¦b¤¾ªøªºÃ«§i¤§¤U¡A$n¯B¤F°_¨Ó¡C[m",
      "[36m§A°½¤F³o¸Ì¨C­Ó¤Hªº¤º¦ç¡C[m",
      "[36m§Aªº¤º¦ç¤£¨£¤F¡I$n§â¥¦°½¨«¤F¡I[m",
      "[36m³Ø°Õ¡B³Ø°Õ...§A¥¿ÄZµÛ¤@­Ó²~¤l¡C[m",
      "[36m³Ø°Õ¡B³Ø°Õ...$n¥¿ÄZµÛ¤@­Ó²~¤l¡C[m",
      "[36m¦h¥X¨Óªº¨º°¦¸}¬O·F¹Àªº©O¡H[m",
      "[36m¬°¤°»ò­è­è$n¦hªø¥X¤F¤@°¦¸}©O¡H[m"
  }
    },

    {
  {
      "[36m§A§â¤j®a³£ÅÜ¦¨¤F¯»¬õ¦âªº¤p¶H¡C[m",
      "[36m§A³Q$nÅÜ¦¨¤F¤@°¦¯»¬õ¦â¤p¶H¡C[m",
      "[36m¦³¤@­Ó¤Ñ¨Ï¦b½Ð±Ð§A¡C[m",
      "[36m¦b¤@­Ó¤Ñ¨Ï¦b½Ð±Ð$n¡C[m",
      "[36m»ë¤lÂà°ÊµÛ...§A¤SÄ¹¤F¡C[m",
      "[36m»ë¤lÂà°ÊµÛ...$n¤SÄ¹¤F¡C[m",
      "[36m...998¡B999¡B1000...§A¦b°µ¥ñ¦a®¼¨­¡C[m",
      "[36m...998¡B999¡B1000...$n¦b°µ¥ñ¦a®¼¨­¡C[m",
      "[36m¨º¥u´ö°Í¦b§A¶}©l¶°¤¤ºë¯«ªº®É­Ô°k¶]¤F¡C[m",
      "[36m¨º¥u´ö°Í¦b$n¶}©l¶°¤¤ºë¯«ªº®É­Ô°k¶]¤F¡C[m"
  }
    },

    {
  {
      "[36m¤@­Ó¤p¥ú²y¦b§Aªº«ü¦y¸õ»R¡C[m",
      "[36m¤@­Ó¤p¥ú²y¦b$nªº«ü¦y¸õ»R¡C[m",
      "[36m§Aªº¨­Åéµo¥X¯«¯µªº¥ú¨~¡C[m",
      "[36m$nªº¨­Åéµo¥X¯«¯µªº¥ú¨~¡C[m",
      "[36m§A¦bºâ¤j®a¤f³U¸Ìªº¿ú¡C[m",
      "[36mÀË¬d§Aªº¿ú¡A$n¥¿¦b¼Æ¡C[m",
      "[36mªü¿Õ¥v¥Ë¨¯®æÆg¹Ä§AªºÅé®æ¡C[m",
      "[36mªü¿Õ¥v¥Ë¨¯®æÆg¹Ä$nªºÅé®æ¡C[m",
      "[36m¤£­n¦b§O¤H­±«e·n°Ê§Aªº¤j¸£¡C[m",
      "[36m¥s$n¤£­n¦b§Aªº­±«e·n°Ê$s¤j¸£¡I[m"
  }
    },

    {
  {
      "[36m·ÏÃú±q§Aªº»ó¤Õº|¥X¨Ó¡C[m",
      "[36m·ÏÃú±q$nªº»ó¤Õº|¥X¨Ó¡C[m",
      "[36m»E¥ú¿O·Ó¦V§A¡C[m",
      "[36m»E¥ú¿O·Ó¦V$n¡C[m",
      "[36m§A§â¤@§â¤p¤M¥­¿Å¦b¦ÞÀY¤W¡C[m",
      "[36m$n§â¤@§â¤p¤M¥­¿Å¦b¦ÞÀY¤W¡C[m",
      "[36m¤p¤ß§Aªº¸}¡A§A¥¿¦b­Aª±ªá±^©¥¶ê¥Û¡C[m",
      "[36m¤p¤ß§Aªº¸}¡A$n¥¿¦b­Aª±ªá±^©¥¶ê¥Û¡C[m",
      "[36mMENSA¥´¹q¸Ü¨Ó...¥L­Ì­n°Ý§A¹ï¬Y¨Æªº·N¨£¡C[m",
      "[36mMENSA­è­è¥´¹q¸Ü¦V$n½Ð±Ð¡C[m"
  }
    },

    {
  {
      "[36m¥ú½u¦b§A©À©G¤åªº®É­Ô°{Ã{¤£©w¡C[m",
      "[36m¥ú½u¦b$n©À©G¤åªº®É­Ô°{Ã{¤£©w¡C[m",
      "[36m§AÃ«§iªº®É­Ô¤j®a³£¯B¤F°_¨Ó¡C[m",
      "[36m§A¦b$nÃ«§iªº®É­Ô¯B¤F°_¨Ó¡C[m",
      "[36m§A±q¨C­Ó¤Hªº¦Õ¦·¸ÌÅÜ¥X¤@ªTµw¹ô¡C[m",
      "[36m$n±q§Aªº¦Õ¦·¸ÌÅÜ¥X¤@ªTµw¹ô¡C[m",
      "[36m¶ã¡ã¡ã§A±qªá±^©¥¶ê¥Û¸ÌÀ½¥X¤ô¨Ó¤F¡C[m",
      "[36m¶ã¡ã¡ã$n±qªá±^©¥¶ê¥Û¸ÌÀ½¥X¤ô¨Ó¤F¡C[m",
      "[36m´È¤l¦b§A³Ì·L®zªº·N©À¤§¤U¦b©Ð¸Ì­¸»R¡C[m",
      "[36m´È¤l¦b$n³Ì·L®zªº·N©À¤§¤U¦b©Ð¸Ì­¸»R¡C[m"
  }
    },

    {
  {
      "[36m§AªºÀY®ø¥¢¤F¡C[m",
      "[36m$nªºÀY®ø¥¢¤F¡C[m",
      "[36m¤@°}²M­·Åý§A«ì´_¤Fºë¯«¡C[m",
      "[36m¤@°}²M­·¨Ï$n«ì´_¤Fºë¯«¡C[m",
      "[36m§A¨«¦b§Aªº¼v¤l«á­±¡C[m",
      "[36m$n¨«¦b$s¼v¤l«á­±¡C[m",
      "[36m§A¥Îªø¥Ù­ç¤ú¡C[m",
      "[36m$n¥Îªø¥Ù­ç¤ú¡C[m",
      "[36m¾¾³á...¤]³\\§A¤£¸Ó¦A¥l³ê§ó¦hªºªe°¨¤F¡C[m",
      "[36m¾¾³á¡I$nÀ³¸Ó¦³¤@°}¤l¤£·|¦A¥l³ê§ó¦hªºªe°¨¤F¡C[m"
  }
    },

    {
  {
      "[36m¤@­Ó¤õ¤§¤¸¯À¿N¤F§AªºÀY¾v¡C[m",
      "[36m¤@­Ó¤õ¤§¤¸¯À¿N¤F$nªºÀY¾v¡C[m",
      "[36m¤Ó¶§¬ï¹L¶³¼h¨Ó·Ó«G§A¡C[m",
      "[36m¤Ó¶§¬ï¹L¶³¼h¨Ó·Ó«G$n¡C[m",
      "[36m§Aªº²´·ú³g°ý¦aÅD°ÊµÛ¡C[m",
      "[36m$nªº²´·ú³g°ý¦aÅD°ÊµÛ¡C[m",
      "[36m§A¤@©ê¤j®a³£©M¦Û¤vªº¸}¤À®a¤F¡C[m",
      "[36m§A¦b$n¤@©ê¤§¤U©M§A¦Û¤wªº¸}¤À®a¤F¡C[m",
      "[36m¾¾³á...§AªºÀY¾v¦]¬°·Q±o¤Ó¥Î¤O¦Óµo¥X§s§sªºÁn­µ¡C[m",
      "[36m¾¾³á...$nªºÀY¾v¦]¬°·Q±o¤Ó¥Î¤O¦Óµo¥X§s§sªºÁn­µ¡C[m"
  }
    },

    {
  {
      "[36m¤ÑªÅ§ïÅÜÃC¦â¨Óå¼°t§Aªº²´·ú¡C[m",
      "[36m¤ÑªÅ§ïÅÜÃC¦â¨Óå¼°t$nªº²´·ú¡C[m",
      "[36m¤j®ü¦b§A­±«e¤À¶}¤F¡C[m",
      "[36m¤j®ü¦b$n­±«e¤À¶}¤F¡C[m",
      "[36m§A¼ô½m¦a°½¨«¤F¨C¤@­Ó¤HªºªZ¾¹¡C[m",
      "[36m$n¼ô½m¦a°½¨«¤F§AªºªZ¾¹¡C[m",
      "[36m§A¥Î¤â¤M§â¾ð¼A¶}¤F¡C[m",
      "[36m$n¥Î¤â¤M§â¾ð¼A¶}¤F¡C[m",
      "[36m¤°»ò¡H§A¥¿¦£µÛ¶°¤¤ºë¯«¡C[m",
      "[36m¤°»ò¡H¾¾¡A$n¡u¤S¡v°g¥¢¦b«ä¦Ò¤¤¤F¡C[m"
  }
    },

    {
  {
      "[36m¥ÛÀY¨Ì·Ó§Aªº©R¥O°_»R¡C[m",
      "[36m¥ÛÀY¨Ì·Ó$nªº©R¥O°_»R¡C[m",
      "[36m¤@¤ù¹p«B¶³¦b§A­±«e¸÷¤U¡C[m",
      "[36m¤@¤ù¹p«B¶³¦b$n­±«e¸÷¤U¡C[m",
      "[36mThe Grey Mouser´À§A¶R¤FªM°à°s¡C[m",
      "[36mThe Grey Mouser´À$n¶R¤FªM°à°s¡C[m",
      "[36m§AÅ@¥Ò¤Wªº±a¤l¦]§A±j¤jªº°«®ð¦ÓÂ_µõ¤F¡C[m",
      "[36m$nÅ@¥Òªº±a¤l¦]¬°$s±j¤j°«®ð¦ÓÂ_µõ¤F¡C[m",
      "[36m§A·|¦b§A¨ü¶Ë¤§«e¤U¨Ó³o¸Ì¶Ü¡H[m",
      "[36m§Ö¡A§ä¸ò´Ò¤l¨Ó¡A$n¤S¦bdoing$spinata impression¤F¡C[m"
  }
    },

    {
  {
      "[36m§A·L¯º°_¨Ó¤Ñ¦a³£§ïÅÜ¤FÃC¦â¡C[m",
      "[36m$n·L¯º°_¨Ó¤Ñ¦a³£§ïÅÜ¤FÃC¦â¡C[m",
      "[36mThe Burning Man¸ò§A»¡¸Ü¡C[m",
      "[36mThe Burning Man¸ò$n»¡¸Ü¡C[m",
      "[36m¨C­Ó¤Hªº¤f³U³£³Q§Aªº·Ï¤õ¬µ¶}¤F¡C[m",
      "[36m§Aªº¤f³U³Q$nªº·Ï¤õ¬µ¶}¤F¡C[m",
      "[36m§A¤@½K¬ÜÀY¡A¤@­Ó¤j¶ê¥Û´Nµõ¶}¤F¡C[m",
      "[36m$n¤@½K¬ÜÀY¡A¤@­Ó¤j¶ê¥Û´Nµõ¶}¤F¡C[m",
      "[36m¤p¤ß...¤£­n§â½Öµ¹¤À¸Ñ¤F¡I[m",
      "[36m¤p¤ß¡I$n­n§â¤°»òªF¦èµ¹¤À¸Ñ¤F¡C[m"
  }
    },

    {
  {
      "[36m¨C­Ó¤Hªº¦çªA³£¬O³z©úªº¡A§A¦b¯º¡C[m",
      "[36m§Aªº¦çªA¬O³z©úªº¡A$n¦b¯º¡C[m",
      "[36m¤@°¦ª÷¦r¶ð¸Ìªº²´·ú¹ïµÛ§A¯w²´¡C[m",
      "[36m¤@°¦ª÷¦r¶ð¸Ìªº²´·ú¹ïµÛ$n¯w²´¡C[m",
      "[36m¤j®a³£µo²{¨ì§Aªº¤P­º¶ZÂ÷¥L­Ìªº²´·ú¥u¦³¤@¤½¤À¡C[m",
      "[36m§Aµo²{¨ì$nªº¤P­º¶ZÂ÷§Aªº²´·ú¥u¦³¤@¤½¤À¡C[m",
      "[36m¶Ä§L³£¨ÓªA±q§Aªº©R¥O¡C[m",
      "[36m¶Ä§L³£¨ÓªA±q$nªº©R¥O¡C[m",
      "[36m§A¤j¸v«Å´­¡u¤ß³Ó©óª«¡vªº¹D²z¡C[m",
      "[36m¬O¬O¬O¡A¤ß³Ó©óª«¡C³¬¼L$n¡C[m"
  }
    },

    {
  {
      "[36m¤@­Ó¶Â¬}§â§A§]¾½¡C[m",
      "[36m¤@­Ó¶Â¬}§â$n§]¾½¡C[m",
      "[36mValentine Michael Smithµ¹§A¤@ªM¤ô¡C[m",
      "[36mValentine Michael Smithµ¹$n¤@ªM¤ô¡C[m",
      "[36m§A¥h¤F­þ¤F¡H[m",
      "[36m$n¥h¤F­þ¤F¡H[m",
      "[36m¥|­Ó¬ÛºÙªº ²Õ¦¨§Aªº¾Ô¨®¡C[m",
      "[36m¥|­Ó¬ÛºÙªºPercherons²Õ¦¨$nªº¾Ô¨®¡C[m",
      "[36mÅF¡C[m",
      "[36mÅF¡C[m"
  }
    },

    {
  {
      "[36m¾ã­Ó¥@¬É¸òµÛ§A§j¤f­ïªº¸`«µ·nÂ\\¡C[m",
      "[36m¾ã­Ó¥@¬É¸òµÛ$n§j¤f­ïªº¸`«µ·nÂ\\¡C[m",
      "[36m°¶¤jªº¯«Amendaµ¹§A¤@ªKÅv§ú¡C[m",
      "[36m°¶¤jªº¯«Amendaµ¹$n¤@ªKÅv§ú¡C[m",
      "[36m³Ø°Õ¡C[m",
      "[36m³Ø°Õ¡C[m",
      "[36mÀº¤Ñªº¨È¯S©Ô´µ¯«½Ð¨D§A¸Ñ±Ï¥L¡C[m",
      "[36mÀº¤Ñªº¨È¯S©Ô´µ¯«½Ð¨D$n¸Ñ±Ï¥L¡C[m",
      "[36m§A¥ÎÅ]¤O§â¤j®aªº¿Ç¤l§l¨«¤F...¤£ÁÙµ¹¥L­Ì¡C[m",
      "[36m§Aªº¿Ç¤l³Q$n¥ÎÅ]¤O§l¨«¤F¡A$e¤£ÁÙµ¹§A¡C[m"
  }
    }
};

void do_pose( CHAR_DATA *ch, char *argument )
{
    int level;
    int pose;
    int class = ch->class;

    if ( IS_NPC( ch ) )
  return;
    if( ch->class > 4 )
  class = apply_class[ch->pcdata->prime_attr];  
    class = URANGE( 0, class, 4);

    level = UMIN( ch->level,
     sizeof( pose_table ) / sizeof( pose_table[0] ) - 1 );
    pose  = number_range( 0, level );

    act( pose_table[pose].message[2*class+0], ch, NULL, NULL, TO_CHAR );
    act( pose_table[pose].message[2*class+1], ch, NULL, NULL, TO_ROOM );

    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m§@ªÌ­Ì¥ÎÃhºÃªº²´¯«¬ÝµÛ§A¡C[m\n\r", ch );
  return;
    }

    append_file( ch, BUG_FILE,  argument );
    send_to_char( "[36m¨S°ÝÃD¡CÁÂÁÂ§A¡C[m\n\r", ch );
    return;
}



void do_idea( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m§@ªÌ­Ì¥ÎÃhºÃªº²´¯«¬ÝµÛ§A¡C[m\n\r", ch );
  return;
    }

    append_file( ch, IDEA_FILE, argument );
    send_to_char( "[36m¨S°ÝÃD¡CÁÂÁÂ§A¡C[m\n\r", ch );
    return;
}



void do_typo( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m§@ªÌ­Ì¥ÎÃhºÃªº²´¯«¬ÝµÛ§A¡C[m\n\r", ch );
  return;
    }

    append_file( ch, TYPO_FILE, argument );
    send_to_char( "[36m¨S°ÝÃD¡CÁÂÁÂ§A¡C[m\n\r", ch );
    return;
}



void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "[36m³o¸Ì¤£¥Î¥I¯²ª÷¡A§A´N¦sÀÉ´N¬O¤F¡C[m\n\r", ch );
    return;
}



void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "[33m¦pªG§A­nQUITªº¸Ü¡A§A±o§â¥¦¥´§¹¥þ¡C[m\n\r", ch );
    return;
}


void fwrite_neweqbank( CHAR_DATA * ch, FILE* fp );
void free_neweqbank( BANK_DATA * bank );
void check_container_save( CHAR_DATA *ch, OBJ_DATA *container )
{
  OBJ_DATA *obj, *obj_next_content;
  
    for ( obj = container->contains; obj; obj = obj_next_content )
    {
        obj_next_content = obj->next_content;
        if ( obj->deleted )
            continue;
        if( IS_SET( obj->extra_flags, ITEM_PERSONAL) && !IS_SET( obj->extra_flags, ITEM_NOSAVE ) ) continue;

        if ( IS_SET( obj->extra_flags, ITEM_NOSAVE )
                || IS_SET( obj->extra_flags, ITEM_UNIQUE )
                || ( ch->level < ( obj->level - 5 ) && ch->class < 5 ) )
        {
            act( ""HIR"§A¤£³Q¤¹³\\Àx¦s"NOR"$p", ch, obj, NULL, TO_CHAR );
            //        unequip_char( ch, obj);
            obj_from_obj( obj );
            obj_to_room( obj, ch->in_room );
        }
    else if ( obj->item_type == ITEM_CONTAINER )
      check_container_save( ch, obj );
  }
}

void do_idlequit( CHAR_DATA *ch, char *argument )
{
	DESCRIPTOR_DATA *d;
	OBJ_DATA *obj, *obj_next_content;
	CHAR_DATA *phorse;

	if ( IS_NPC( ch ))
		return;

	phorse = ch->pcdata->horse;

	if( auction_list ){
		AUC_DATA *auction;
		for( auction = auction_list ; auction; auction = auction->next ){
			if( auction->owner == ch || auction->buyer == ch )
			{
				send_to_char(CYN"¤£¦æ¡I§AÁÙ¦b¶i¦æ©ç½æ©O¡C"NOR"\n\r", ch);
				return;
			}
		}
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "[33m¤£¦æ¡I§AÁÙ¦b¥´°«¡C[m\n\r", ch );
		return;
	}

	if ( phorse && phorse->position == POS_FIGHTING )
	{
		send_to_char( "[33m¤£¦æ¡I§Aªº°¨ÁÙ¦b¥´°«¡C[m\n\r", ch );
		return;
	}

	if ( ch->position  < POS_STUNNED  )
	{
		send_to_char( "[33m§AÁÙ¨S¡u¦º¡v­þ¡C[m\n\r", ch );
		return;
	}

	if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
			&& str_cmp( ch->in_room->area->identity, "skarabrae" )
			&& !IS_IMMORTAL( ch) )
	{
		send_to_char(HIC"§A¨Ã¤£¦b®È©±©Î¬O¨ä¥L¥i¥H¦w¥þ¥ð®§ªº¦a¤è³á¡I"NOR"\n\r", ch);
	}

	if ( IS_SET( ch->act, PLR_HORSE ) )
	{
		// brute force
		send_to_char("½Ð¥ý¤U°¨¦A¶i¦æÂ÷½u¡C\n\r", ch);
		return;

		if ( !ch->desc )
			return;
		if ( !ch->desc->original )
		{
			bug( "do_quit : Horse without master!", 0 );
			bug( ch->name, 0 );
			return;
		}
		ch->desc->original->pcdata->switched = FALSE;
		ch->desc->character      = ch->desc->original;
		ch->desc->original       = NULL;
		ch->desc->character->desc    = ch->desc; 
		ch->desc         = NULL;
		ch = ch->pcdata->horse;
		phorse = ch->pcdata->horse;
	}
	affect_strip( ch, gsn_ride );
	affect_strip( ch, gsn_morphing );
	affect_strip( ch, gsn_lifeshare ); // quit ²¾°£ link cast ®ÄªG 2021/11/22


	if( ch->neweqbank ) {
		FILE * fp;
		char tmp[255];

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
		ch->neweqbank->opened = FALSE;
		if(( fp = fopen(tmp, "w"))){
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fflush( fp );
	  fclose(fp);
			send_to_char( "®L·ç´µ(Sharas)´À§A§â¥¼Ãö³¬ªº­Ü®wÃö¤W¤F¡C\n\r", ch );
		}
		else {
			bug("eqbank: file open error!!",0);
			send_to_char("µo¥ÍÄY­«°ÝÃD¡A½Ð¦V¤j¯«¦^³ø¡C\n\r", ch);
			return;
		}
		free_neweqbank(ch->neweqbank);
		ch->neweqbank = NULL;
	}


	if ( ch->in_room->roomprogs && ( ch->in_room->roomprogs->type & ROOM_LEAVE ) && !IS_SET( ch->act, PLR_WIZINVIS ) )
		rprog_leave_trigger( ch->in_room, ch, 7 );
	else
		if( ch->in_room->progtypes & ROOM_LEAVE) // Added by Amethyst
			rprog_leave_trigger(ch->in_room, ch, 7); // Added by Amethyst

	//    if ( !IS_NPC( ch ) && !ch->clan )
	//  REMOVE_BIT( ch->act, PLR_BOUGHT_PET );
	//add by coconet 0702 for let ppl quit will auto ride
	if ( get_skill_percent( ch,gsn_ride ) > 0 || IS_IMMORTAL(ch) )
	{
		if( !get_eq_char( ch , WEAR_RIDE ) )
			do_ride(ch,"");
	}

	send_to_char( "[36m¾ÌÂÇµÛ§A¹ï©ó­ì¨Óªº¥@¬Éªº²o±¾¡A§A¤S¦^¨ì¤F­ì¨Óªº¥@¬É¡C[m\n\r", ch );
	send_to_char( "[36m¦ý¬O¡A§Æ±æ§A¤£­n§Ñ¤F¦Û¤vªº¨Ï©R", ch );
	send_to_char( "©M§A¹ï³o­Ó¥@¬Éªº©Ó¿Õ¡C[m\n\r\n\r", ch );

	act( ""CYN"¤@¹DÂÅ¦âªº°{¥ú±q¤Ñ¦Ó­°¡A§â"NOR" $n "CYN"±aÂ÷¤F³o­Ó¥@¬É¡C"NOR"", ch, NULL, NULL, TO_ROOM );
	sprintf( log_buf, "%s has quit.", ch->name );
	top10_check(ch);
	log_string( log_buf );

	/*
	 * After extract_char the ch is no longer valid!
	 */

	for ( obj = ch->carrying; obj; obj = obj_next_content )
	{
		obj_next_content = obj->next_content;
		if ( obj->deleted )
			continue;
		/*           
			if ( obj->item_type == ITEM_HORSE )
			{
				horse = create_mobile( get_mob_index( obj->pIndexData->horse) );


				for( obj_horse = obj->contains; obj_horse ; obj_horse = obj_horse_next )
				{
					obj_horse_next = obj_horse->next_content;
					obj_from_obj( obj_horse );
					obj_to_char( obj_horse, horse );
				}

				obj_from_char( obj );
				extract_obj( obj );
				char_to_room( horse, ch->in_room);
				SET_BIT( horse->act,     ACT_HORSE    );
				SET_BIT( horse->act,  ACT_SENTINEL      );          
			}
		*/      
		if( IS_SET( obj->extra_flags, ITEM_PERSONAL) && obj->item_type != ITEM_HORSE && !IS_SET( obj->extra_flags, ITEM_NOSAVE )) continue;
		if ( IS_SET( obj->extra_flags, ITEM_NOSAVE )
			|| IS_SET( obj->extra_flags, ITEM_UNIQUE )
			|| ( ch->level < ( obj->level - 5 ) && ch->class < 5 ) )
		{
			act( ""HIR"§A¤£³Q¤¹³\\Àx¦s"NOR"$p", ch, obj, NULL, TO_CHAR );
			// unequip_char( ch, obj);
			obj_from_char( obj );
			obj_to_room( obj, ch->in_room );
		}
		else if ( obj->item_type == ITEM_CONTAINER )
			check_container_save( ch, obj );
	}


	save_char_obj( ch );



	d = ch->desc;
	if ( phorse )
		extract_char( phorse, TRUE );
	extract_char( ch, TRUE );
	if ( d )
		close_socket( d );

	return;
}
void do_quit( CHAR_DATA *ch, char *argument )
{
	DESCRIPTOR_DATA *d;
	OBJ_DATA *obj, *obj_next_content;
	CHAR_DATA *phorse;

	if ( IS_NPC( ch ) || !ch->desc )
		return;

	phorse = ch->pcdata->horse;

	if( auction_list ){
		AUC_DATA *auction;
		for( auction = auction_list ; auction; auction = auction->next ){
			if( auction->owner == ch || auction->buyer == ch )
			{
				send_to_char(CYN"¤£¦æ¡I§AÁÙ¦b¶i¦æ©ç½æ©O¡C"NOR"\n\r", ch);
				return;
			}
		}
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "[33m¤£¦æ¡I§AÁÙ¦b¥´°«¡C[m\n\r", ch );
		return;
	}

	if ( phorse && phorse->position == POS_FIGHTING )
	{
		send_to_char( "[33m¤£¦æ¡I§Aªº°¨ÁÙ¦b¥´°«¡C[m\n\r", ch );
		return;
	}

	if ( ch->position  < POS_STUNNED  )
	{
		send_to_char( "[33m§AÁÙ¨S¡u¦º¡v­þ¡C[m\n\r", ch );
		return;
	}

	if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
			&& str_cmp( ch->in_room->area->identity, "skarabrae" )
			&& !IS_IMMORTAL( ch) )
	{
		send_to_char(HIC"§A¨Ã¤£¦b®È©±©Î¬O¨ä¥L¥i¥H¦w¥þ¥ð®§ªº¦a¤è³á¡I"NOR"\n\r", ch);
	}

	if ( IS_SET( ch->act, PLR_HORSE ) )
	{
		// brute force
		send_to_char("½Ð¥ý¤U°¨¦A¶i¦æÂ÷½u¡C\n\r", ch);
		return;

		if ( !ch->desc )
			return;
		if ( !ch->desc->original )
		{
			bug( "do_quit : Horse without master!", 0 );
			bug( ch->name, 0 );
			return;
		}
		ch->desc->original->pcdata->switched = FALSE;
		ch->desc->character      = ch->desc->original;
		ch->desc->original       = NULL;
		ch->desc->character->desc    = ch->desc; 
		ch->desc         = NULL;
		ch = ch->pcdata->horse;
		phorse = ch->pcdata->horse;
	}
	affect_strip( ch, gsn_ride );
	affect_strip( ch, gsn_morphing );
	affect_strip( ch, gsn_lifeshare ); // quit ²¾°£ link cast ®ÄªG 2021/11/22

	if( ch->neweqbank ) {
		FILE * fp;
		char tmp[255];

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
		ch->neweqbank->opened = FALSE;
		if(( fp = fopen(tmp, "w"))){
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fflush( fp );
			fclose(fp);
			send_to_char( "®L·ç´µ(Sharas)´À§A§â¥¼Ãö³¬ªº­Ü®wÃö¤W¤F¡C\n\r", ch );
		}
		else {
			bug("eqbank: file open error!!",0);
			send_to_char("µo¥ÍÄY­«°ÝÃD¡A½Ð¦V¤j¯«¦^³ø¡C\n\r", ch);
			return;
		}
		free_neweqbank(ch->neweqbank);
		ch->neweqbank = NULL;
	}


	if ( ch->in_room->roomprogs && ( ch->in_room->roomprogs->type & ROOM_LEAVE ) && !IS_SET( ch->act, PLR_WIZINVIS ) )
		rprog_leave_trigger( ch->in_room, ch, 7 );
	else
		if( ch->in_room->progtypes & ROOM_LEAVE) // Added by Amethyst
			rprog_leave_trigger(ch->in_room, ch, 7); // Added by Amethyst

    // if ( !IS_NPC( ch ) && !ch->clan )
    // REMOVE_BIT( ch->act, PLR_BOUGHT_PET );

	//add by coconet 0702 for let ppl quit auto ride
	if ( get_skill_percent( ch,gsn_ride ) > 0 || IS_IMMORTAL(ch) )
	{
		if( !get_eq_char( ch , WEAR_RIDE ) )
			do_ride(ch,"");
	}

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ))
	{
		if( get_eq_char( ch , WEAR_RIDE ) )
			do_ride(ch,"");
	}

	send_to_char( "[36m¾ÌÂÇµÛ§A¹ï©ó­ì¨Óªº¥@¬Éªº²o±¾¡A§A¤S¦^¨ì¤F­ì¨Óªº¥@¬É¡C[m\n\r", ch );
	send_to_char( "[36m¦ý¬O¡A§Æ±æ§A¤£­n§Ñ¤F¦Û¤vªº¨Ï©R", ch );
	send_to_char( "©M§A¹ï³o­Ó¥@¬Éªº©Ó¿Õ¡C[m\n\r\n\r", ch );

	act( ""CYN"¤@¹DÂÅ¦âªº°{¥ú±q¤Ñ¦Ó­°¡A§â"NOR" $n "CYN"±aÂ÷¤F³o­Ó¥@¬É¡C"NOR"", ch, NULL, NULL, TO_ROOM );
	sprintf( log_buf, "%s has quit.", ch->name );
	top10_check(ch);
	log_string( log_buf );

    /*
     * After extract_char the ch is no longer valid!
     */

    for ( obj = ch->carrying; obj; obj = obj_next_content )
    {
        obj_next_content = obj->next_content;
        if ( obj->deleted )
            continue;

		/*
			if ( obj->item_type == ITEM_HORSE )
			{
				horse = create_mobile( get_mob_index( obj->pIndexData->horse) );


				for( obj_horse = obj->contains; obj_horse ; obj_horse = obj_horse_next )
				{
					obj_horse_next = obj_horse->next_content;
					obj_from_obj( obj_horse );
					obj_to_char( obj_horse, horse );
				}

				obj_from_char( obj );
				extract_obj( obj );
				char_to_room( horse, ch->in_room);
				SET_BIT( horse->act,     ACT_HORSE    );
				SET_BIT( horse->act,  ACT_SENTINEL      );          
			}
		*/        
		if( IS_SET( obj->extra_flags, ITEM_PERSONAL) && obj->item_type != ITEM_HORSE && !IS_SET( obj->extra_flags, ITEM_NOSAVE ))
			continue;
		if ( IS_SET( obj->extra_flags, ITEM_NOSAVE )
			|| IS_SET( obj->extra_flags, ITEM_UNIQUE )
			|| ( ch->level < ( obj->level - 5 ) && ch->class < 5 ) )
		{
			act( ""HIR"§A¤£³Q¤¹³\\Àx¦s"NOR"$p", ch, obj, NULL, TO_CHAR );
			// unequip_char( ch, obj);
			obj_from_char( obj );
			obj_to_room( obj, ch->in_room );
		}
		else if ( obj->item_type == ITEM_CONTAINER )
			check_container_save( ch, obj );

	}
	fflush( NULL );
	save_char_obj( ch );

	if (phorse)
	{
		for ( obj = phorse->carrying; obj; obj = obj_next_content )
		{
			obj_next_content = obj->next_content;
			if ( obj->deleted )
				continue;
			if( IS_SET( obj->extra_flags, ITEM_PERSONAL) && !IS_SET( obj->extra_flags, ITEM_NOSAVE ))
				continue;
			if ( IS_SET( obj->extra_flags, ITEM_NOSAVE )
				|| IS_SET( obj->extra_flags, ITEM_UNIQUE )
				|| ( phorse->level < ( obj->level - 5 ) && ch->class < 5 ) )
			{
				act( ""HIR"§A¤£³Q¤¹³\\Àx¦s"NOR"$p", phorse, obj, NULL, TO_CHAR );
				// unequip_char( ch, obj);
				obj_from_char( obj );
				obj_to_room( obj, phorse->in_room );
			}
			else if ( obj->item_type == ITEM_CONTAINER )
				check_container_save( phorse, obj );
		}
		save_char_obj(phorse);
	}

	d = ch->desc;
	if ( phorse )
		extract_char( phorse, TRUE );
	extract_char( ch, TRUE );
	if ( d )
		close_socket( d );

	return;
}



void do_suicid( CHAR_DATA *ch, char *argument )
{
    send_to_char( "[33m¦pªG§A­nSUICIDEªº¸Ü¡A§A±o§â¥¦¥´§¹¥þ¡C[m\n\r", ch );
    return;
}



void do_suicide( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_STRING_LENGTH ];
    FILE *fp;

    if ( IS_NPC( ch ) )
  return;

    if( IS_IMMORTAL(ch )){
  send_to_char("¯«¤£¥i¥H¦Û±þ¡C\n\r", ch);
  return;
    }

    if( IS_SET(ch->act, PLR_HORSE)){
  send_to_char("½Ð¥ý¤U°¨¡C\n\r", ch);
  return;
    }

    if ( argument[0] == '\0' ) {
  send_to_char( "[33m¨Ï¥Î¤èªk¡Gsuicide <password>[m\n\r", ch );
  send_to_char( "[33m½Ð¥´¡uhelp suicide¡v¬Ý¬Ý»¡©ú¡C[m\n\r", ch ); 
  return;
    }
  
    if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) ) {
  send_to_char( "[33m§Aªº±K½X¤£¹ï¡C[m\n\r", ch );
  return;
    }

    if ( ch->position == POS_FIGHTING )
    {
  send_to_char( "[33m¤£¦æ¡I§AÁÙ¦b¥´°«¡C[m\n\r", ch );
  return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
  send_to_char( "[33m§AÁÙ¨S¡u¦º¡v­þ¡C[m\n\r", ch );
  return;
    }

    sprintf( buf, "[36m%s©êµÛ°í©wªº¨M¤ß¦V³o­Ó¥@¬É§i§O¡C\n\r[m", ch->short_descr );
    send_to_all_char( buf );
    
    sprintf( log_buf, "%s has committed suicide.", ch->name );
    log_string( log_buf );

    /*
     * After do_quit the ch is no longer valid!
     */
    do_backup( ch, "");
    do_quit( ch, "" );

#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( buf, "%s%s%s%s%s", PLAYER_DIR, initial( ch->name ),
      "/", capitalize( ch->name ), ".gz" );
    if ( ( fp = fopen( buf, "r" ) ) )
    {
    fflush( fp );
    fclose( fp );
  sprintf( buf, "gzip -dfq %s", buf );
  system( buf );
    }
#endif

#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( buf, "%s%s%s%s", PLAYER_DIR, initial( ch->name ),
      "/", capitalize( ch->name ) );
#else
    sprintf( buf, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
#endif
    remove( buf );

    return;
}

void do_hsuicid( CHAR_DATA *ch, char *argument )
{
    send_to_char( "[33m¦pªG§A­nHSUICIDEªº¸Ü¡A§A±o§â¥¦¥´§¹¥þ¡C[m\n\r", ch );
    return;
}

void do_hsuicide( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_STRING_LENGTH ];
    FILE *fp;
    CHAR_DATA *phorse;

    if ( IS_NPC( ch ) )
  return;

    if( IS_IMMORTAL(ch )){
  send_to_char("¯«¤£¥i¥H¦Û±þ¡C\n\r", ch);
  return;
    }

    if( IS_SET(ch->act, PLR_HORSE)){
  send_to_char("½Ð¥ý¤U°¨¡C\n\r", ch);
  return;
    }

  if ( !( phorse = ch->pcdata->horse ) )
  {
    send_to_char("¨S¬Ý¨ì§Aªº°¨³á¡C\n\r", ch);
    return;
  }
  
    if ( argument[0] == '\0' ) {
  send_to_char( "[33m¨Ï¥Î¤èªk¡Gsuicide <password>[m\n\r", ch );
  send_to_char( "[33m½Ð¥´¡uhelp suicide¡v¬Ý¬Ý»¡©ú¡C[m\n\r", ch ); 
  return;
    }
  
    if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) ) {
  send_to_char( "[33m§Aªº±K½X¤£¹ï¡C[m\n\r", ch );
  return;
    }

    if ( ch->position == POS_FIGHTING )
    {
  send_to_char( "[33m¤£¦æ¡I§AÁÙ¦b¥´°«¡C[m\n\r", ch );
  return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
  send_to_char( "[33m§AÁÙ¨S¡u¦º¡v­þ¡C[m\n\r", ch );
  return;
    }

    sprintf( buf, "[36m%s´Ý§Ô¦a±þ±¼¥Lªº§¤ÃM¡I¯u¬O¥i¼¦ªºªF¦è¡A¸ò¿ù¥D¤H¤F...\n\r[m", ch->short_descr );
    send_to_all_char( buf );
    
    sprintf( log_buf, "%s has killed his horse! Oh, my god, so cruel....", ch->name );
    log_string( log_buf );

    /*
     * After do_quit the ch is no longer valid!
     */
    do_backup( ch, "");
    do_quit( ch, "" );

#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( buf, "%s%s%s%s%s", HORSE_DIR, initial( ch->name ),
      "/", capitalize( ch->name ), ".gz" );
    if ( ( fp = fopen( buf, "r" ) ) )
    {
    fflush( fp );
    fclose( fp );
  sprintf( buf, "gzip -dfq %s", buf );
  system( buf );
    }
#endif

#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( buf, "%s%s%s%s", HORSE_DIR, initial( ch->name ),
      "/", capitalize( ch->name ) );
#else
    sprintf( buf, "%s%s", HORSE_DIR, capitalize( ch->name ) );
#endif
    remove( buf );

    return;
}

void do_silentsave( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
  return;

    if( !ch->desc ) return;

    if ( ch->level < 2 && ch->class <= BARBARIAN )
    {
  return;
    }
    if( !ch->in_room ) {
  ROOM_INDEX_DATA * limbo = get_room_index(ROOM_VNUM_ALTAR);
  if( ch->was_in_room ) {
      char_to_room(ch, ch->was_in_room );
  }
  else char_to_room( ch, limbo);

    }
    if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
      && str_cmp( ch->in_room->area->identity, "skarabrae" )
      && !IS_IMMORTAL( ch) )
    {
    }

//  WAIT_STATE( ch, 6 );
/*
  if ( !IS_SET( ch->act, PLR_HORSE ) )
  {
      save_char_obj( ch );
      if ( (phorse = ch->pcdata->horse ) && phorse->in_room != ch->in_room) {
    ROOM_INDEX_DATA *old;
    old = phorse->in_room;
    char_from_room( phorse );
    char_to_room( phorse, ch->in_room );
        save_horse_obj( ch->pcdata->horse );
    char_from_room( phorse );
    char_to_room( phorse, old );
    
      }
      else
        save_horse_obj( ch->pcdata->horse );
  }
  else
  {
      if ( !ch->pcdata->horse )
      {
        bug( "do_save:horse without master", 0 );
        bug( ch->name, 0 );
        return;
      }
    save_horse_obj( ch );
      save_char_obj( ch->pcdata->horse );
    }
*/        
  save_char_obj( ch );
    return;
}


void do_hsave( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
  return;

    if( !ch->desc ) return;

    if ( ch->level < 2 && ch->class <= BARBARIAN )
    {
  send_to_char( "[33m§A³Ì¤Ö­n¨ì²Ä¤G¯Å¥H¤W¤~¯à¦sÀÉ¡C[m\n\r", ch );
  return;
    }
    if( !ch->in_room ) {
  ROOM_INDEX_DATA * limbo = get_room_index(ROOM_VNUM_ALTAR);
  if( ch->was_in_room ) {
      char_to_room(ch, ch->was_in_room );
  }
  else char_to_room( ch, limbo);

    }
    if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
      && str_cmp( ch->in_room->area->identity, "skarabrae" )
      && !IS_IMMORTAL( ch) )
    {
  send_to_char(HIC"§A¨Ã¤£¦b®È©±©Î¬O¨ä¥L¥i¥H¦w¥þ¥ð®§ªº¦a¤è³á¡I"NOR"\n\r", ch);
    }

//  WAIT_STATE( ch, 6 );

    save_char_obj( ch );
        
    send_to_char( "[36m¦sÀÉ§¹²¦¡C[m\n\r", ch );
    return;
}

void do_save( CHAR_DATA *ch, char *argument )
{
	if ( IS_NPC( ch ) )
		return;

	if( !ch->desc ) return;

	if ( ch->level < 2 && ch->class <= BARBARIAN )
	{
		send_to_char( "[33m§A³Ì¤Ö­n¨ì²Ä¤G¯Å¥H¤W¤~¯à¦sÀÉ¡C[m\n\r", ch );
		return;
	}

	if( !ch->in_room ) {
		ROOM_INDEX_DATA * limbo = get_room_index(ROOM_VNUM_ALTAR);
		if( ch->was_in_room ) {
			char_to_room(ch, ch->was_in_room );
		}
		else
			char_to_room( ch, limbo);
	}
	if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
		&& str_cmp( ch->in_room->area->identity, "skarabrae" )
		&& !IS_IMMORTAL( ch) )
	{
		send_to_char(HIC"§A¨Ã¤£¦b®È©±©Î¬O¨ä¥L¥i¥H¦w¥þ¥ð®§ªº¦a¤è³á¡I"NOR"\n\r", ch);
	}

	//  WAIT_STATE( ch, 6 );

	save_char_obj( ch );

	send_to_char( "[36m¦sÀÉ§¹²¦¡C[m\n\r", ch );

	/* horse save ¨S¦³§@¥Î 2024/01/29
	if( !IS_NPC(ch) && ch->pcdata->horse )
		do_hsave( ch->pcdata->horse, ""); */

	return;
}

void do_dice( CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH];
    char buf[256];
    int dice = 21;
    int total = 0;

    argument = one_argument( argument, arg);
    if( arg[0] == '\0' ) { dice = 21;}
/*  else if( !str_cmp( arg, "bless" ) )
  {
    argument = one_argument( argument, arg);
    if( arg[0] == '\0' || argument[0] == '\0' )
      return;
    if( !is_number( arg ) || !is_number( argument ) )
      return;
    dice = atoi(arg);
    total = atoi(argument);
    if ( total > dice )
      total = dice;
  }
 */ 
    else  dice = atoi(arg);
    dice = URANGE( 1 , dice, 1000 );

    if( !total ) total += number_range( 1, dice );

    sprintf(buf, CYN"¡i$nÂY¤F%d­±ªº»ë¤l-------------\n  µ²ªG¬O %d ÂI¡j"NOR,dice, total);
    act( buf, ch, NULL, ch, TO_CHAR );
    act( buf, ch, NULL, ch, TO_ROOM );
    return;
}

void do_follow( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	if( !ch->in_room ) return;

	one_argument( argument, arg );

	if( arg[0] == '\0' )
	{
		send_to_char( "[33m­n¸òÀH½Ö¡H[m\n\r", ch );
		return;
	}

	if( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
		return;
	}

	if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master )
	{
		act( "[33m¥i¬O§A¤ñ¸û·Q¸òÀH$N¡I[m", ch, NULL, ch->master, TO_CHAR );
		return;
	}

	/*  ²¾°£aff_summoned ªº¥Íª«µLªk °õ¦æ follow ªº§P©w
	if( IS_AFFECTED(ch, AFF_SUMMONED)){
	  send_to_char("§AµLªk¸òÀH¥D¤H¥H¥~ªº¥Ø¼Ð¡C\n\r", ch);
	  return;
	}
	*/

	// Åýaff_summonedªº¥Íª«(¥l³êª«)  ¥u¯à°õ¦æ follow ¥D¤H¸ò self  19/11/27
	if( IS_AFFECTED(ch, AFF_SUMMONED) && victim != ch && victim != ch->rembch){
		send_to_char("§AµLªk¸òÀH¥D¤H¥H¥~ªº¥Ø¼Ð¡C\n\r", ch);
		return;
	}

	// follow self
	if( victim == ch )
	{
		if( !ch->master )
		{
			send_to_char( "[33m§A¤w¸g¥u¸òÀH§A¦Û¤v¤F¡C[m\n\r", ch );
			return;
		}
		stop_follower( ch );
		return;
	}

	// ¤w¸g¦b follow ch ªº±¡ªp
	if( victim == ch->master ) {
	   send_to_char("§A¤w¸g¦b¸òÀHµÛ¥L¤F\n\r",ch);
	   return;
	}

	// pk room µLªk follow
	if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_PK)){
	   send_to_char("PK ³õ¸Ì¤£¯à follow¡C\n\r", ch);
	   return;
	}

    /*if ( ( ch->level - victim->level < -5
		|| ch->level - victim->level >  5 )
		&& !IS_HERO( ch ) )
		{
		send_to_char( "[33m§Aªº¯à¤O©M¥L¬Û®t¤Ó¦h¤F¡C[m\n\r", ch );
		return;
    }*/

	if ( ch->master )
		stop_follower( ch );

	add_follower( ch, victim );
	return;
}



void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
  
	if ( ch->master )
	{
		// ²¾°£¤£¥²­nªº bug log 2022/11/14
		//bug( "Add_follower: non-null master.", 0 );
		//bugf( "Add_follower: non-null master. ch: %s ", ch->name );
		return;
	}

	ch->master        = master;
	//ch->leader        = NULL;

	if ( can_see( master, ch ) )
	act( "[36m$n¶}©l¸òÀHµÛ§A¡C[m", ch, NULL, master, TO_VICT );

	act( "[36m§A¶}©l¸òÀHµÛ$N¡C[m",  ch, NULL, master, TO_CHAR );

	return;
}

void add_rember( CHAR_DATA *ch, CHAR_DATA *pch )
{
    ch->next_in_rember = pch->rembed;
    pch->rembed = ch;
    ch->rembch = pch;
}

void add_rrember( ROOM_INDEX_DATA *room, CHAR_DATA *pch)
{
    room->next_in_rrember = pch->rrembed;
    pch->rrembed = room;
    room->rrembch = pch;
}

// ·s¼W obj rember ªº«ü¥O 2022/11/10
void add_orember( OBJ_DATA* obj , CHAR_DATA *pch)
{
    obj->next_in_orember = pch->orembed;
    pch->orembed = obj;
    obj->orembch = pch;
}

void add_protector( CHAR_DATA *ch, CHAR_DATA *pch )
{
    if( ch->protecting == pch ) {
        bugf("duplicated protecting: [%s] -> [%s]",ch->name, pch->name);
        return;
    }
    ch->next_in_protect = pch->protector;
    pch->protector = ch;
    ch->protecting = pch;
    act( "§A¨M©w¦b¾Ô°«¤¤¤p¤ß¦a«OÅ@$N¤£¨ü¨ì¶Ë®`¡C", ch, NULL, pch, TO_CHAR );
    act( "$n¨M©w¦b¾Ô°«¤¤¤p¤ß¦a«OÅ@$N¤£¨ü¨ì¶Ë®`¡C", ch, NULL, pch, TO_ROOM );
}

// ¥[¤J ch ¨ó§U pch 2021/11/20
void add_cooperator( CHAR_DATA *ch, CHAR_DATA *pch )
{
	if( ch->cooperating == pch ) {
		bugf("duplicated cooperating: [%s] -> [%s]",ch->name, pch->name);
		return;
	}
	ch->next_in_coop = pch->cooperator;
	pch->cooperator = ch;
	ch->cooperating = pch;
	act( "§A¨M©w±q®Ç¨ó§U$N¡C", ch, NULL, pch, TO_CHAR );
	act( "$n¨M©w±q®Ç¨ó§U$N¡C", ch, NULL, pch, TO_ROOM );
}

// link chars 2021/11/20
void add_linking( CHAR_DATA *ch, CHAR_DATA *pch)
{
	if( !ch ) {
		bugf("there is no linking ch: [%s]",ch->name );
		return;
	}
	if( !pch ) {
		bugf("there is no linking vch: [%s]",pch->name );
		return;
	}
	ch->linking = NULL;
	pch->linking = NULL;
	ch->linking = pch;
	pch->linking = ch;
}

void stop_rrember( ROOM_INDEX_DATA *ch, CHAR_DATA *pch)
{
	ROOM_INDEX_DATA *pch_next;
	if( pch->rrembed == ch ) {
		pch->rrembed = ch->next_in_rrember;
	}
	else {
		for( pch_next = pch->rrembed ; pch_next ; pch_next = pch_next->next_in_rrember  ){
			if( pch_next->next_in_rrember == pch_next ) {
				pch_next->next_in_rrember = NULL;
				break;
			}
			if( pch_next->next_in_rrember == ch ){
				pch_next->next_in_rrember = ch->next_in_rrember;
				break;
			}
		}
	}
	ch->next_in_rrember= NULL;
	ch->rrembch = NULL;
}

// ·s¼W obj rember ªº«ü¥O 2022/11/10
void stop_orember( OBJ_DATA *ch, CHAR_DATA *pch)
{
	OBJ_DATA *pch_next;
	if( pch->orembed == ch ) {
		pch->orembed = ch->next_in_orember;
	}
	else {
		for( pch_next = pch->orembed ; pch_next ; pch_next = pch_next->next_in_orember  ){
			if( pch_next->next_in_orember == pch_next ) {
				pch_next->next_in_orember = NULL;
				break;
			}
			if( pch_next->next_in_orember == ch ){
				pch_next->next_in_orember = ch->next_in_orember;
				break;
			}
		}
	}
	ch->next_in_orember= NULL;
	ch->orembch = NULL;
}

void stop_rember( CHAR_DATA *ch, CHAR_DATA *pch)
{
	CHAR_DATA *pch_next;
	time_t lt;             //Added by Razgriz 20050811

	if( pch->rembed == ch ) {
		pch->rembed = ch->next_in_rember;
	}
	else {
		lt = time ( NULL );			//Added by Razgriz	20050811
		for( pch_next = pch->rembed ; pch_next ; pch_next = pch_next->next_in_rember  )
		{
			pch_next->time_stamp = lt;	//Added by Razgriz	20050811
			if ( pch_next->next_in_rember == pch_next ) {
				pch_next->next_in_rember = NULL;
				break;
			}
			if ( pch_next->next_in_rember == ch ) {
				pch_next->next_in_rember = ch->next_in_rember;
				break;
			}
			
			//Added by Razgriz	20050811
			if ( (pch_next->next_in_rember) &&
			     (pch_next->time_stamp == pch_next->next_in_rember->time_stamp) ) {
				pch_next->next_in_rember = NULL;
				break;
			}
		}
	}
	
	ch->next_in_rember= NULL;
	ch->rembch = NULL;
}

/** ch °±¤î«OÅ@ pch 
 * last update by zcecil 2004/07/04
 */
void stop_protecting( CHAR_DATA *ch, CHAR_DATA *pch)
{
	CHAR_DATA *protector, *protector_next;
	if( !ch || !pch ) return;
	if( ch->protecting != pch ) return; // sorry, i am not protecting you
	if( pch->protector == ch ) {
		pch->protector = ch->next_in_protect;
		act( "§A¤£Ä@·N¦A«OÅ@$N¤F¡I", ch, NULL, pch, TO_CHAR );
		act( "$n¤£Ä@·N¦A«OÅ@§A¤F¡I", ch, NULL, pch, TO_VICT );
	}
	else
	{
		for( protector = pch->protector ; protector ; protector = protector_next ){
			protector_next = protector->next_in_protect;
			if( protector_next == ch ) {
				protector->next_in_protect = ch->next_in_protect;
				act( "§A¤£Ä@·N¦A«OÅ@$N¤F¡I", ch, NULL, pch, TO_CHAR );
				act( "$n¤£Ä@·N¦A«OÅ@§A¤F¡I", ch, NULL, pch, TO_VICT );
				break;
			}
		}
	}
	ch->next_in_protect = NULL;
	ch->protecting = NULL;
}

// ch °±¤î¨ó§U pch 2021/11/20
void stop_cooperating( CHAR_DATA *ch, CHAR_DATA *pch)
{
	CHAR_DATA *cooperator, *cooperator_next;
	if( !ch || !pch ) return;
	if( ch->cooperating != pch ) return; // sorry, i am not cooperating you
	if( pch->cooperator == ch ) {
		pch->cooperator = ch->next_in_coop;
		act( "§A¤£Ä@·N¦A¨ó§U$N¤F¡I", ch, NULL, pch, TO_CHAR );
		act( "$n¤£Ä@·N¦A¨ó§U§A¤F¡I", ch, NULL, pch, TO_VICT );
	}
	else
	{
		for( cooperator = pch->cooperator ; cooperator ; cooperator = cooperator_next ){
			cooperator_next = cooperator->next_in_coop;
			if( cooperator_next == ch ) {
				cooperator->next_in_coop = ch->next_in_coop;
				act( "§A¤£Ä@·N¦A¨ó§U$N¤F¡I", ch, NULL, pch, TO_CHAR );
				act( "$n¤£Ä@·N¦A¨ó§U§A¤F¡I", ch, NULL, pch, TO_VICT );
				break;
			}
		}
	}
	ch->next_in_coop = NULL;
	ch->cooperating = NULL;
}

// link chars 2021/11/20
void stop_linking( CHAR_DATA *ch , CHAR_DATA *pch)
{
	if( !ch || !pch ) return;
	if( ch->linking != pch ) return;
	if( ch->linking == NULL ||  pch->linking == NULL ) return;
	affect_strip( ch, gsn_lifeshare );
	affect_strip( pch, gsn_lifeshare );
	act( "§A»P$NÆF»î¶¡ªºÁpµ²¤¤Â_¤F¡C", ch, NULL, pch, TO_CHAR );
	act( "§A»P$nÆF»î¶¡ªºÁpµ²¤¤Â_¤F¡C", ch, NULL, pch, TO_VICT );
	ch->linking = NULL;
	pch->linking = NULL;
}

// link chars 2021/11/20
void remove_linking( CHAR_DATA *ch )
{
	if( !ch ) return;
	ch->linking = NULL;
}

void remove_rrember( CHAR_DATA *ch)
{
	ROOM_INDEX_DATA *pch, *pch_next;

	for( pch = ch->rrembed ; pch ; pch = pch_next ){
		pch->rrembch = NULL;
		pch_next = pch->next_in_rrember;
		pch->next_in_rrember= NULL;
	}
	ch->rrembed = NULL;
}

void remove_rember( CHAR_DATA *ch)
{
	CHAR_DATA *pch, *pch_next;

	for( pch = ch->rembed ; pch ; pch = pch_next ){
		pch->rembch = NULL;
		pch_next = pch->next_in_rember;
		pch->next_in_rember= NULL;
	}
	ch->rembed = NULL;
}


/** Åý©Ò¦³«OÅ@ ch ªº¤H³q³q°±¤î«OÅ@ ch
 *
 */
void remove_protector( CHAR_DATA *ch)
{
	CHAR_DATA *pch, *pch_next;

	for( pch = ch->protector ; pch ; pch = pch_next ){
		pch->protecting = NULL;
		pch_next = pch->next_in_protect;
		pch->next_in_protect = NULL;
		act( "§A¤£Ä@·N¦A«OÅ@$N¤F¡I", pch, NULL, ch, TO_CHAR );
		act( "$n¤£Ä@·N¦A«OÅ@§A¤F¡I", pch, NULL, ch, TO_VICT );
	}
	ch->protector = NULL;
}
// ²¾°£©Ò¦³¨ó§Uªº¤H 2021/11/20
void remove_cooperator( CHAR_DATA *ch)
{
	CHAR_DATA *pch, *pch_next;

	for( pch = ch->cooperator ; pch ; pch = pch_next ){
		pch->cooperating = NULL;
		pch_next = pch->next_in_coop;
		pch->next_in_coop = NULL;
		act( "§A¤£Ä@·N¦A¨ó§U$N¤F¡I", pch, NULL, ch, TO_CHAR );
		act( "$n¤£Ä@·N¦A¨ó§U§A¤F¡I", pch, NULL, ch, TO_VICT );
	}
	ch->cooperator = NULL;
}

//ch stop follow xxx
void stop_follower( CHAR_DATA *ch )
{
	if ( !ch->master )
	{
		bug( "Stop_follower: null master.", 0 );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_CHARM ) )
	{
		REMOVE_BIT( ch->affected_by, AFF_CHARM );
		affect_strip( ch, gsn_charm_person );
		affect_strip( ch, gsn_domination   );
	}

	/*if ( ( vch = ch->protector ) && ch->leader )
	{
	remove_protector( ch );
	}*/


	/*if ( ( vch = ch->protecting ) )
	{
	stop_protecting(ch, vch);
	}*/

	if ( can_see( ch->master, ch ) )
		act( "[36m$n¤£¦A¸òÀH§A¤F¡C[m", ch, NULL, ch->master, TO_VICT );
	act( "[36m§A¤£¦A¸òÀH$N¤F¡C[m", ch, NULL, ch->master, TO_CHAR );

	ch->master = NULL;
	if ( IS_SET( ch->act, ACT_PET ) ) ch->clan = NULL;
	//ch->leader = NULL;
	return;
}

//all who follow ch stop following
void die_follower( CHAR_DATA *ch, char *name )
{
    CHAR_DATA *fch;

    if ( ch->master )
  stop_follower( ch );

    //ch->leader = NULL;

    for ( fch = char_list; fch; fch = fch->next )
    {
  if ( fch->deleted )
      continue;
  if ( fch->master == ch )
      stop_follower( fch );
  //if ( fch->leader == ch )
  //    fch->leader = NULL;
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	CHAR_DATA *och;
	CHAR_DATA *och_next;
	char arg [ MAX_INPUT_LENGTH ];
	bool found;
	bool fAll;

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || argument[0] == '\0' )
	{
		send_to_char( "[33m­n©R¥O½Ö°µ¤°»ò¡H[m\n\r", ch );
		return;
	}

	if (IS_NPC(ch))
		return;  

	if ( IS_AFFECTED( ch, AFF_CHARM ) )
	{
		send_to_char( "[33m§AÀ³¸Ó¬O­n¨ü©R¥O¡A¦Ó¤£¬Oµo©R¥O§a¡I[m\n\r", ch );
		return;
	}

	if ( !str_cmp( arg, "all" ) )
	{
		fAll   = TRUE;
		victim = NULL;
	}
	else
	{
		fAll   = FALSE;
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}
		if( !victim ) {
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}

		if ( victim == ch )
		{
			send_to_char( "[33m¬O¬O¬O¡A°¨¤W¿ì¡I[m\n\r", ch );
			return;
		}
		if( IS_AFFECTED(victim, AFF_BATTLE)){
			send_to_char("§A¥¿¦bµo°Ê¾Ô°}¡A¤£¯à±µ¨ü»â¾É¡C\n\t",victim);
			return;
		}

		if ( !( (IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch) || !IS_AFFECTED( victim, AFF_CHARM )
					//|| ( !IS_SET( victim->act, ACT_PET ) )
					|| ( !IS_NPC(ch) && !IS_NPC(victim) && ch == victim->pcdata->horse && victim->pcdata->horse == ch ) 
					|| ( victim->master != ch && victim->clan != ch->clan ) ) ) 
		{
			send_to_char( "[33m§A¦Û¤v¤£·|¥h°µ§r¡H[m\n\r", ch );
			return;
		}
		//if( IS_AFFECTED( victim, AFF_SUMMONED ) )
		//{
		//  victim->ordered = TRUE;
		//}
	}
	if( !str_prefix(argument, "order")
			||  !str_prefix(argument, "lead") ){
		send_to_char("¤£¥i¥H­n¨D§O¤H¤U©R¥O¡I\n\r", ch);
		return;
	}

	//0941006 by jye
	//can't order mob or ppl do "chat"
	if( !str_prefix("chat" , argument )
			||  !str_prefix(".",argument) ){
		send_to_char("¤£¥i¥H­n¨D§O¤H¨Ï¥Î¤½ÀW¡I\n\r", ch);
		return;
	}

	found = FALSE;
	for ( och = ch->in_room->people; och; och = och_next )
	{
		och_next = och->next_in_room;

		if ( och->deleted )
			continue;
		if( IS_AFFECTED(och, AFF_BATTLE)){
			continue;
		}

		if( IS_NPC(och) && !str_prefix( "mp", argument )) continue;

		//Added by Razgriz 20051006
		//can't order mob do "snare" "untangle"
		if (    IS_NPC ( och )
			 && ( !str_prefix( "sn", argument ) || !str_prefix( "unt", argument ) ) )
			 continue;

		if ( ( ( IS_AFFECTED( och, AFF_CHARM ) && och->master==ch )
					|| (IS_AFFECTED( och, AFF_SUMMONED )&&och->rembch == ch)
					//       || IS_SET( och->act, PLR_HORSE ) ||
					|| (och  == ch->pcdata->horse )
					//       || ( IS_NPC( och ) && och->clan && och->clan == ch->clan )
					//      && ( ( och->master == ch ) || ( IS_NPC( och ) && och->clan == ch->clan ) )
					|| ( och == ch->pcdata->horse && IS_SET(ch->act, PLR_HORSE) ))
				&& IS_AWAKE( och ) && ( fAll || och == victim ) )
		{
			och->ordered = TRUE;
			found = TRUE;
			act( "[36m$n©R¥O§A¡u$t¡v¡C[m", ch, argument, och, TO_VICT );
			if ( och->wait > 14 )
			{
				send_to_char( "¥L¤w¸g¤â¦£¸}¶Ã¤F¡I\n\r", ch );
				return;
			}
			interpret( och, argument );
		}
	}

	if ( found )
		send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
	else
		send_to_char( "[33m³o¸Ì¨S¦³¤H¥i¥HÅ¥±q©R¥O¡C[m\n\r", ch );
	return;
}

const char* stat_bars[12] = 
{
    BRED"_"BWHT"_________", // 0-9
    BHIR"_"BRED"_"BWHT"________", // 10-19
    BHIY"__"BYEL "_"BWHT"_______", // 20-29
    BHIY"____"BWHT"______", // 30-39
    BHIG"_____"BGRN"_"BWHT"____",//40-49
    BHIG"______"BWHT"____",//50-59
    BHIC"_______"BCYN"_"BWHT"__",//60-69
    BHIC"________"BWHT"__",//70-79
    BHIU"________"BBLU"_"BWHT"_",//80-89
    BHIU"_________"BWHT"_", //90-99
    BHIU"__________" ,// 100
    BHIW"__________" // 100+
};

const char* gr_stat_bar(int cur, int max)
{
  int ratio ;
  if( !cur || ! max ) ratio = 0;
  else ratio = cur * 100 /max;

        return stat_bars[URANGE(0,ratio / 10,11)];
}


char* gr_stat_color(int cur, int max)
{
	int ratio ;
	if( !cur || ! max ) ratio = 0;
	else ratio = cur * 100 /max;
	if( ratio < 30 ) return HIR;
	else if(ratio < 60) return HIY;
	else if(ratio <= 100) return WHT;
	else return WHT;
}

//add by jye 0940622
void do_show_group_info( CHAR_DATA *ch, char *argument )
{
	char buf [ BUF_STRING_LENGTH * 5];
	char no_ansi [ MAX_STRING_LENGTH ];
	CHAR_DATA *gch,*tch;
	CHAR_DATA *leader;
	bool fChinese = FALSE;
	CHAR_DATA  *wch[ 500 ];
	int i,j;
	int intCharCnt,nMatch;

	if( IS_SET( ch->act, PLR_CHINESE ) )
		fChinese = TRUE;

	leader =  (ch->group) ? ch->group->leader : ch;

	if( !str_cmp(argument , "-p") )
	{
		sprintf( buf, "\n\r%s"NOR"%s¡G\n\r"//
		// 1234567890123456789012345678901234567890123456789012345678901234567890   
		"[LV  Â¾  ·~]        ©m        ¦W            ¥Í©R¤O      ¥¿¦b«OÅ@       «OÅ@ª¬ºA   [m\n\r", //PERS( leader, ch ) );
			leader->group ? leader->group->name : ch->short_descr, leader->group ? "" :"ªº¶¤¥î");
	}
	// group -k for cooperate 2021/11/23
	else if( !str_cmp(argument , "-k") )
	{
		sprintf( buf, "\n\r%s"NOR"%s¡G\n\r"//
		// 1234567890123456789012345678901234567890123456789012345678901234567890   
		"[LV  Â¾  ·~]        ©m        ¦W            ¥Í©R¤O      ¥¿¦b¨ó§U       ¨ó§Uª¬ºA   [m\n\r", //PERS( leader, ch ) );
			leader->group ? leader->group->name : ch->short_descr, leader->group ? "" :"ªº¶¤¥î");
	}
	else
	{
		sprintf( buf, "\n\r%s"NOR"%s¡G\n\r"//
		// 1234567890123456789012345678901234567890123456789012345678901234567890   
		"[LV  Â¾  ·~]        ©m        ¦W            ¥Í©R¤O      Å]ªk¤O       ²¾°Ê¤O   [m\n\r", //PERS( leader, ch ) );
			leader->group ? leader->group->name : ch->short_descr, leader->group ? "" :"ªº¶¤¥î");
	}
	send_to_char( buf, ch );
	nMatch = 0;
	i = 0;
	for( gch = char_list ; gch; gch = gch->next)
	{
		if( gch->deleted )
			continue;
		if( is_same_group( gch, ch ) )
		{
			wch[i] = gch;
			nMatch++;
			i++;
		}
	}
	if( argument != '\0' || (  str_cmp(argument , "-p")
							&& str_cmp(argument , "-k")
							&& str_cmp(argument , "+r")
							&& str_cmp(argument , "-r")) )
	{
		for( i = 0; i < nMatch; i++ ) 
		{
			for( j = i + 1; j < nMatch ; j++ )
			{
				if( (    !str_cmp(argument , "-l") && wch[i]->level > wch[j]->level )
					|| ( !str_cmp(argument , "+l") && wch[i]->level < wch[j]->level )
					|| ( !str_cmp(argument , "-h") && wch[i]->max_hit > wch[j]->max_hit )
					|| ( !str_cmp(argument , "+h") && wch[i]->max_hit < wch[j]->max_hit )
					|| ( !str_cmp(argument , "-m") && wch[i]->max_mana > wch[j]->max_mana )
					|| ( !str_cmp(argument , "+m") && wch[i]->max_mana < wch[j]->max_mana )
					|| ( !str_cmp(argument , "-v") && wch[i]->max_move > wch[j]->max_move )
					|| ( !str_cmp(argument , "+v") && wch[i]->max_move < wch[j]->max_move )
					|| ( !str_cmp(argument , "-c") && wch[i]->class > wch[j]->class )
					|| ( !str_cmp(argument , "+c") && wch[i]->class < wch[j]->class ))
				{
					tch = wch[i];
					wch[i] = wch[j];
					wch[j] = tch;
				}
			}
		}
	}
	for( i = 0; i < nMatch; i++ ) 
	{
		int SameRoom = 0;

		strip_ansi( wch[i]->short_descr, no_ansi );

		if( ch->in_room == wch[i]->in_room )
			SameRoom = 1;

		// group -k for cooperate  2021/11/23
		//if( str_cmp(argument , "-p") )
		if( str_cmp(argument , "-p") && str_cmp(argument , "-k") && str_cmp(argument , "+r") && str_cmp(argument , "-r"))
		{
			if( !IS_NPC(ch) && IS_SET(ch->act, PLR_GRAPHIC))
			{
				sprintf( buf,
					//NOR"[%-2d %6s] %-27s  %s%s   %s%s   %s[m\n\r",
					NOR"%s%-3d %6s] "NOR"%-27s  %s%s   %s%s   %s[m\n\r",
					( SameRoom == 1 ) ? "[" : ""CYN"[" ,
					wch[i]->level,
					IS_NPC( wch[i] ) ? "«Dª±ªÌ"
					: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
					can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
					gr_stat_bar(wch[i]->hit, wch[i]->max_hit),
					NOR, gr_stat_bar(wch[i]->mana, wch[i]->max_mana),
					NOR,gr_stat_bar(wch[i]->move, wch[i]->max_move)
					);
			}
			else
			{
				sprintf( buf,
					NOR"%s%-3d %6s] %-27s"NOR" %s%5d%s/%-5d  %s%5d%s/%-5d  %s%5d%s/%-5d [m\n\r",
					( SameRoom == 1 ) ? "[" : ""CYN"[" ,
					wch[i]->level,
					IS_NPC( wch[i] ) ? "«Dª±ªÌ"
					: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
					can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
					gr_stat_color(wch[i]->hit, wch[i]->max_hit),
					wch[i]->hit,
					NOR, wch[i]->max_hit, gr_stat_color(wch[i]->mana, wch[i]->max_mana),
					wch[i]->mana,  NOR,wch[i]->max_mana,gr_stat_color(wch[i]->move, wch[i]->max_move),
					wch[i]->move, NOR, wch[i]->max_move
					);
			}
		}
		else
		{
			// group -k for cooperate  2021/11/23
			if ( !str_cmp(argument , "-k") ){
				if( !IS_NPC(ch) )
				{
					intCharCnt = 0;
					if( ( tch = wch[i]->cooperator ) ) 
					{   
						for( ; tch && tch != tch->next_in_coop ; tch = tch->next_in_coop ) 
						intCharCnt++;
					}

					sprintf( buf,
						//NOR"[%-2d %6s] %-27s"NOR" %s%5d%s/%-5d    %s%-12s       %d [m\n\r",
						NOR"%s%-3d %6s] %-27s"NOR" %s%5d%s/%-5d    %s%-12s       %d [m\n\r",
						( SameRoom == 1 ) ? "[" : ""CYN"[" ,
						wch[i]->level,
						IS_NPC( wch[i] ) ? "«Dª±ªÌ"
						: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
						gr_stat_color(wch[i]->hit, wch[i]->max_hit),
						wch[i]->hit,
						NOR, wch[i]->max_hit,
						HIW,
						(tch = wch[i]->cooperating) ? tch->name : "----",
						intCharCnt
						);
				}
			}
			else if ( !str_cmp(argument , "-p") )
			{
				if( !IS_NPC(ch) )
				{
					intCharCnt = 0;
					if( ( tch = wch[i]->protector ) ) 
					{   
						for( ; tch && tch != tch->next_in_protect ; tch = tch->next_in_protect ) 
						intCharCnt++;
					}

					sprintf( buf,
						//NOR"[%-2d %6s] %-27s"NOR" %s%5d%s/%-5d    %s%-12s       %d [m\n\r",
						NOR"%s%-3d %6s] %-27s"NOR" %s%5d%s/%-5d    %s%-12s       %d [m\n\r",
						( SameRoom == 1 ) ? "[" : ""CYN"[" ,
						wch[i]->level,
						IS_NPC( wch[i] ) ? "«Dª±ªÌ"
						: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
						gr_stat_color(wch[i]->hit, wch[i]->max_hit),
						wch[i]->hit,
						NOR, wch[i]->max_hit,
						HIW,
						(tch = wch[i]->protecting) ? tch->name : "----",
						intCharCnt
						);
				}
			}
			else if ( !str_cmp(argument , "+r") ) // ¥[¤J +r °Ñ¼ÆÅã¥Ü¦P group ¤¤¦P room ªº¨¤¦â 2022/11/21
			{
				if( SameRoom == 0 )
					continue;
				if( !IS_NPC(ch) && IS_SET(ch->act, PLR_GRAPHIC))
				{
					sprintf( buf,
						NOR"[%-3d %6s] %-27s  %s%s   %s%s   %s[m\n\r",
						wch[i]->level,
						IS_NPC( wch[i] ) ? "«Dª±ªÌ"
										: fChinese
											? (char *)class_table[wch[i]->class].cwho_name
											: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
						gr_stat_bar(wch[i]->hit, wch[i]->max_hit),
						NOR,
						gr_stat_bar(wch[i]->mana, wch[i]->max_mana),
						NOR,
						gr_stat_bar(wch[i]->move, wch[i]->max_move)
						);
				}
				else
				{
					sprintf( buf,
						NOR"[%-3d %6s] %-27s"NOR" %s%5d%s/%-5d  %s%5d%s/%-5d  %s%5d%s/%-5d [m\n\r",
						wch[i]->level,
						IS_NPC( wch[i] ) ? "«Dª±ªÌ"
										: fChinese
										? (char *)class_table[wch[i]->class].cwho_name
										: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
						gr_stat_color(wch[i]->hit, wch[i]->max_hit),
						wch[i]->hit,
						NOR,
						wch[i]->max_hit, gr_stat_color(wch[i]->mana, wch[i]->max_mana),
						wch[i]->mana,
						NOR,
						wch[i]->max_mana,gr_stat_color(wch[i]->move, wch[i]->max_move),
						wch[i]->move,
						NOR,
						wch[i]->max_move
						);
				}
			}
			else if ( !str_cmp(argument , "-r") ) // ¥[¤J -r °Ñ¼ÆÅã¥Ü¦P group ¤¤¤£¦P room ªº¨¤¦â 2022/11/21
			{
				if( SameRoom == 1 )
					continue;
				if( !IS_NPC(ch) && IS_SET(ch->act, PLR_GRAPHIC))
				{
					sprintf( buf,
						NOR"[%-3d %6s] %-27s  %s%s   %s%s   %s[m\n\r",
						wch[i]->level,
						IS_NPC( wch[i] ) ? "«Dª±ªÌ"
										: fChinese
											? (char *)class_table[wch[i]->class].cwho_name
											: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
						gr_stat_bar(wch[i]->hit, wch[i]->max_hit),
						NOR,
						gr_stat_bar(wch[i]->mana, wch[i]->max_mana),
						NOR,
						gr_stat_bar(wch[i]->move, wch[i]->max_move)
						);
				}
				else
				{
					sprintf( buf,
						NOR"[%-3d %6s] %-27s"NOR" %s%5d%s/%-5d  %s%5d%s/%-5d  %s%5d%s/%-5d [m\n\r",
						wch[i]->level,
						IS_NPC( wch[i] ) ? "«Dª±ªÌ"
										: fChinese
										? (char *)class_table[wch[i]->class].cwho_name
										: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "¤£©ú¤Hª«",
						gr_stat_color(wch[i]->hit, wch[i]->max_hit),
						wch[i]->hit,
						NOR,
						wch[i]->max_hit, gr_stat_color(wch[i]->mana, wch[i]->max_mana),
						wch[i]->mana,
						NOR,
						wch[i]->max_mana,gr_stat_color(wch[i]->move, wch[i]->max_move),
						wch[i]->move,
						NOR,
						wch[i]->max_move
						);
				}
			}
		}
		send_to_char( buf, ch );
	}
	return;  
}

void do_group( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH  ];
	char arg1 [ MAX_INPUT_LENGTH  ];
	GROUP_DATA *pG = NULL, *oldpG= NULL;
	CHAR_DATA *gch, *gch_next;

	argument = one_argument( argument, arg );
	one_argument( argument, arg1 );

	//modify by jye 0940622
	if( arg[0] == '\0' 
		|| !str_cmp(arg , "-l") 
		|| !str_cmp(arg , "+l") 
		|| !str_cmp(arg , "-h") 
		|| !str_cmp(arg , "+h") 
		|| !str_cmp(arg , "-m") 
		|| !str_cmp(arg , "+m") 
		|| !str_cmp(arg , "-v") 
		|| !str_cmp(arg , "+v") 
		|| !str_cmp(arg , "-c")
		|| !str_cmp(arg , "+c")
		|| !str_cmp(arg , "-k") // ¨ó§Uª¬ºA 2021/11/23
		|| !str_cmp(arg , "+r") // ¦P®æ¶¤­û 2021/11/23
		|| !str_cmp(arg , "-r") // ¶¤­û¬O§_¦P®æ 2021/11/23
		|| !str_cmp(arg , "-p"))
	{
		do_show_group_info( ch , arg );
		return;
	}

	if( !str_cmp( arg, "form" ) ) 
	{
		if( ch->group ) 
		{
			send_to_char("§A¤w¸g¦³¶¤¥î¤F¡C\n\r", ch);
			return;
		}

		if( *arg1 == '\0' ) 
		{
			sprintf(arg1, "%sªº¶¤¥î", ch->short_descr);
		}
		else if( strlen(arg1) > 30 ) 
		{
			send_to_char("§Aªº¶¤¦W¤Óªø¤F¡C\n\r", ch);
			return;
		}


		pG = new_group();
		pG->leader = ch;
		pG->name = str_dup(arg1);
		act( "[36m§A¦¨¥ß¤F¤@¤ä«_ÀI¶¤¥î¡C[m", ch, NULL, NULL, TO_CHAR    );
		act( "[36m$n¦¨¥ß¤F¤@¤ä«_ÀI¶¤¥î¡C[m", ch, NULL, NULL, TO_ROOM);
		ch->next_in_group = NULL;
		ch->group = pG;
		pG->members = ch;
		return;
	}

	if( !ch->group ) 
	{
		send_to_char("§A¨Ã¨S¦³¦b¶¤¥î¤¤¡C\n\r", ch);
		return;
	}


	if( !str_cmp(arg, "leave"))
	{
		act( "[36m$N²æÂ÷¤F§Aªº¶¤¥î¡C[m", ch->group->leader, NULL, ch, TO_CHAR    );
		act( "[36m§A²æÂ÷¤F$nªº¶¤¥î¡C[m", ch->group->leader, NULL, ch, TO_VICT);
		remove_member( ch);
		return;
	}



	if( ch != ch->group->leader ) 
	{
		send_to_char("¥u¦³¤@¶¤¤§ªø¤~¥i¥H³o»ò°µ¡C\n\r", ch);
		return;
	}

	pG = ch->group;

	if( !str_cmp( arg, "disband" ) ) 
	{
		act( "[36m§A¸Ñ´²¤F¶¤¥î¡C[m", ch, NULL, NULL, TO_CHAR    );
		act( "[36m$n¸Ñ´²¤F¶¤¥î¡C[m", ch, NULL, NULL, TO_ROOM);
		clear_member(ch);
		return;
	}

	if( !str_cmp(arg, "transfer"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) || IS_NPC(victim))
		{
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}

		if( victim == ch ) 
		{
			send_to_char("§A¤w¸g¬O¶¤ªø¤F¡C\n\r", ch);
			return;
		}

		if( !is_same_group(ch, victim))
		{
			send_to_char("¥ý¦¬¥L¤J¶¤§a¡C\n\r", ch);
			return;
		}

		act( "[36m§A§â¶¤ªø¤§Â¾¥æ±µµ¹$N¡C[m", ch, NULL, victim, TO_CHAR    );
		act( "[36m$n§â¶¤ªø¤§Â¾¥æ±µµ¹§A¡C[m", ch, NULL, victim, TO_VICT    );
		act( "[36m$n§â¶¤ªø¤§Â¾¥æ±µµ¹$N¡C[m", ch, NULL, victim, TO_NOTVICT );
		ch->group->leader = victim;
		return;
	}

	if( !str_cmp(arg, "*"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) )
		{
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}

		if( !victim->group || victim != victim->group->leader )
		{
			send_to_char("¥L¨Ã¤£¬O¤@¶¤¤§ªø¡C\n\r", ch);
			return;
		}

		if( victim->master != ch && ch != victim )
		{
			act( "[33m$N¨Ã¨S¦³¸òÀH§A¡C[m", ch, NULL, victim, TO_CHAR );
			return;
		}

		if( is_same_group(ch, victim))
		{
			return;
		}

		oldpG = victim->group;

		for( gch = victim->group->members ; gch ; gch = gch_next )
		{
			gch_next = gch->next_in_group;
			gch->next_in_group = pG->members;
			pG->members = gch;
			gch->group = pG;
		}

		oldpG->members = NULL;
		oldpG->leader = NULL;
		free_group(oldpG);
		act( "[36m§A¨Ö§]¤F$Nªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
		act( "[36m$n¨Ö§]¤F§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_VICT    );
		act( "[36m$n¨Ö§]¤F$Nªº¶¤¥î¡C[m", ch, NULL, victim, TO_NOTVICT );
		//ch->group->leader = victim;
		return;
	}

	if ( !str_cmp( arg, "all" ) )
	{
		for( victim = ch->in_room->people; victim; victim = victim->next_in_room )
		{
			if( victim->group == NULL 
				&& victim->master == ch
				&& victim != ch )
			{
				//victim->leader = ch;
				//ch->leader = ch;
				add_member(ch, victim);
				act( "[36m$N¥[¤J§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
				act( "[36m§A¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_VICT    );
				act( "[36m$N¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_NOTVICT );
			}
		}
		return;
	}

	if( !str_cmp(arg, "+"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) )
		{
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}

		if( victim == ch ) 
		{
			send_to_char(YEL "§ä­Ó§Oªº¶¤¤Í¤ñ¸û¹ê»ÚÂI¡C"NOR"\n\r", ch);
			return;
		}

		if( victim->master != ch && ch != victim )
		{
			act( "[33m$N¨Ã¨S¦³¸òÀH§A¡C[m", ch, NULL, victim, TO_CHAR );
			return;
		}

		add_member(ch, victim);

		act( "[36m$N¥[¤J§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
		act( "[36m§A¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_VICT    );
		act( "[36m$N¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_NOTVICT );
		return;
	}
	if( !str_cmp(arg, "-"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) )
		{
			send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
			return;
		}

		if( victim == ch ) 
		{
			send_to_char(YEL "½Ð¨Ï¥Îgroup leave¡C"NOR"\n\r", ch);
			return;
		}


		if( is_same_group( victim, ch ) )
		{
			CHAR_DATA *vch;

			if( ( vch = victim->protector ) && !IS_AFFECTED( vch, AFF_SUMMONED ) )
			{
				remove_protector(victim);//should remove before set victim->leader == null
			}
			// °±¤î³Q¨ó§U 2021/11/20
			if( ( vch = victim->cooperator ) && !IS_AFFECTED( vch, AFF_SUMMONED ) )
			{
				remove_cooperator(victim);
			}

			// victim->leader = NULL;
			remove_member( victim);
			act( "[36m§A§â$N²¾¥X§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
			act( "[36m$n§â§A²¾¥X$s¶¤¥î¡C[m",  ch, NULL, victim, TO_VICT  );
			act( "[36m$n§â$N²¾¥X$s¶¤¥î¡C[m",   ch, NULL, victim, TO_NOTVICT );

			// °±¤î¨ó§U¥Ø¼Ð 2021/11/20
			if( ( vch = victim->linking ) )
			{
				stop_linking(victim, vch);
			}

			// °±¤î¨ó§U¥Ø¼Ð 2021/11/20
			if( ( vch = victim->cooperating ) )
			{
				stop_cooperating(victim, vch);
			}

			if( ( vch = victim->protecting ) )
			{
				stop_protecting(victim, vch);
			}

			return;
		}
		else 
		{
			send_to_char("¥L¤£¦b§Aªº¶¤¥î¸Ì¡C\n\r", ch);
			return;
		}

		return;
	}
	return;
}

/*void do_group( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char       buf [ BUF_STRING_LENGTH * 5];
    char      no_ansi [ MAX_STRING_LENGTH ];
    char       arg [ MAX_INPUT_LENGTH  ];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
  CHAR_DATA *gch;
  CHAR_DATA *leader;

  leader = ( ch->leader ) ? ch->leader : ch;
  sprintf( buf, "\n\r%s"NOR"ªº¶¤¥î¡G\n\r"//
 1234567890123456789012345678901234567890123456789012345678901234567890   
"[LV Â¾  ·~]        ©m        ¦W            ¥Í©R¤O      Å]ªk¤O       ²¾°Ê¤O   [m\n\r", PERS( leader, ch ) );
  send_to_char( buf, ch );

  for ( gch = char_list; gch; gch = gch->next )
  {
      if ( gch->deleted )
    continue;
      if ( is_same_group( gch, ch ) )
      {

    strip_ansi( gch->short_descr, no_ansi );
                if( !IS_NPC(ch) && IS_SET(ch->act, PLR_GRAPHIC)){
                    sprintf( buf,
                            NOR"[%-2d %s] %-27s  %s%s   %s%s   %s[m\n\r",
                            gch->level,
                            IS_NPC( gch ) ? "«Dª±ªÌ"
                            : (char *)class_table[gch->class].cwho_name,
                            can_see( ch, gch ) ? no_ansi : "¤£©ú¤Hª«",
                            gr_stat_bar(gch->hit, gch->max_hit),
                            NOR, gr_stat_bar(gch->mana, gch->max_mana),
                            NOR,gr_stat_bar(gch->move, gch->max_move)
                           );
                }
                else {
                    sprintf( buf,
                            NOR"[%-2d %s] %-27s %s%5d%s/%-5d  %s%5d%s/%-5d  %s%5d%s/%-5d [m\n\r",
                            gch->level,
                            IS_NPC( gch ) ? "«Dª±ªÌ"
                            : (char *)class_table[gch->class].cwho_name,
                            can_see( ch, gch ) ? no_ansi : "¤£©ú¤Hª«",
                            gr_stat_color(gch->hit, gch->max_hit),
                            gch->hit,
                            NOR, gch->max_hit, gr_stat_color(gch->mana, gch->max_mana),
                            gch->mana,  NOR,gch->max_mana,gr_stat_color(gch->move, gch->max_move),
                            gch->move, NOR, gch->max_move
                           );
                }
    send_to_char( buf, ch );
      }
  }
  return;
    }
    
    if( !str_cmp( arg, "form" ) ) {
    }

    if ( ch->master || ( ch->leader && ch->leader != ch ) )
    {
  send_to_char( "[33m¥i¬O§A¦b¸òÀH§O¤H­C¡C[m\n\r", ch );
  return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
      for( victim = ch->in_room->people; victim; victim = victim->next_in_room )
        if ( victim->leader == NULL && victim->master == ch
      && victim != ch
      && ch->level - victim->level < 6 
          && ch->level -victim->level > -6 )
        {
          victim->leader = ch;
      ch->leader = ch;
          act( "[36m$N¥[¤J§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
          act( "[36m§A¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_VICT    );
          act( "[36m$N¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_NOTVICT );
        }
  return;
    }

    if ( !( victim = get_char_room( ch, arg ) ) )
    {
  send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
  return;
    }

    if( victim == ch ) {
  send_to_char(YEL "§ä­Ó§Oªº¶¤¤Í¤ñ¸û¹ê»ÚÂI¡C"NOR"\n\r", ch);
  return;
    }

    if ( victim->master != ch && ch != victim )
    {
  act( "[33m$N¨Ã¨S¦³¸òÀH§A¡C[m", ch, NULL, victim, TO_CHAR );
  return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
  CHAR_DATA *vch;

    if ( ( vch = victim->protector ) && !IS_AFFECTED( vch, AFF_SUMMONED ) )
    {
  remove_protector(victim);//should remove before set victim->leader == null
    }

  victim->leader = NULL;
  act( "[36m§A§â$N²¾¥X§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
  act( "[36m$n§â§A²¾¥X$s¶¤¥î¡C[m",  ch, NULL, victim, TO_VICT  );
  act( "[36m$n§â$N²¾¥X$s¶¤¥î¡C[m",   ch, NULL, victim, TO_NOTVICT );

    if ( ( vch = victim->protecting ) )
    {
  stop_protecting(victim, vch);
    }

  return;
    }

    if ( ch->level < LEVEL_IMMORTAL
  && ( ch->level - victim->level < -5
  || ch->level - victim->level >  5 ) )
    {
  act( "[33m$N¤£¯à¥[¤J§Aªº¶¤¥î¡C[m",  ch, NULL, victim, TO_CHAR   );
  act( "[33m§A¤£¯à¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_VICT   );
  act( "[33m$N¤£¯à¥[¤J$nªº¶¤¥î¡C[m",  ch, NULL, victim, TO_NOTVICT    );
  return;
    }

    victim->leader = ch;
    ch->leader = ch;
    act( "[36m$N¥[¤J§Aªº¶¤¥î¡C[m", ch, NULL, victim, TO_CHAR    );
    act( "[36m§A¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_VICT    );
    act( "[36m$N¥[¤J$nªº¶¤¥î¡C[m", ch, NULL, victim, TO_NOTVICT );
    return;
}*/



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    char       buf [ MAX_STRING_LENGTH ];
    char       arg [ MAX_INPUT_LENGTH  ];
    int        members;
    int        amount;
    int        share;
    int        extra;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
  send_to_char( "[33m¤À°t¦h¤Ö¿ú¡H[m\n\r", ch );
  return;
    }
    
    amount = atoi( arg );

    if ( amount < 0 )
    {
  send_to_char( "[33m§Aªº¶¤¤Í¤£·|·Q­n³o¼Ëªº¡C[m\n\r", ch );
  return;
    }

    if ( amount == 0 )
    {
  send_to_char( "[33m§A®³¥X¤F¹s¶ô¿ú¡A¦ý¬O¨S¦³¤Hª`·N¨ì¡C[m\n\r", ch );
  return;
    }

    if ( ch->gold < amount )
    {
  send_to_char( "[33m§A¨S¦³¨º»ò¦hª÷¹ô¡C[m\n\r", ch );
  return;
    }
  
    members = 0;
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
  if ( gch->deleted )
      continue;
  if ( is_same_group( gch, ch ) && !IS_NPC( gch ))
      members++;
    }

    if ( members < 2 )
    {
  //send_to_char( "[33m¦Û¤v¯dµÛ§a¡I[m\n\r", ch );
  return;
    }
      
    share = amount / members;
    extra = amount % members;

    if ( share == 0 )
    {
  send_to_char( "[33m§O¥´ÂZ¤H®a¡A¤p®ð°­¡C[m\n\r", ch );
  return;
    }

    ch->gold -= amount;
    ch->gold += share + extra;

    sprintf( buf,
  "[36m§A¤À¥X¤F%dªTª÷¹ô¡C§A¦Û¤v¤À¨ì¤F%dªTª÷¹ô¡C[m\n\r",
  amount, share + extra );
    send_to_char( buf, ch );

    sprintf( buf, "[36m$n¤À¥X¤F%dªTª÷¹ô¡C§A¤À¨ì¤F%dªTª÷¹ô¡C[m\n\r",
  amount, share );

    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
  if ( gch->deleted )
      continue;
  if ( gch != ch && is_same_group( gch, ch ) && !IS_NPC(gch) )
  {
      act( buf, ch, NULL, gch, TO_VICT );
      gch->gold += share;
  }
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *gch;
	char       buf [ MAX_STRING_LENGTH ];

	if ( argument[0] == '\0' )
	{
		send_to_char( "[33m­n§i¶D§Aªº¶¤¤Í¤°»ò¡H[m\n\r", ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| ( IS_SET( race_table[ch->race].race_abilities, RACE_MUTE ) && !IS_SET( ch->act, PLR_HORSE ) )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "[33m§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µ¶]¥X¨Ó¡C[m\n\r", ch );
		return;
	}

	if ( IS_SET( ch->act, PLR_NO_TELL ) )
	{
		send_to_char( "[33m§AªºÁn­µ¶Ç¤£¹L¥h¡I[m\n\r", ch );
		return;
	}

	argument = makedrunk( argument, ch );

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
	sprintf( buf, HIG"%s¦V¶¤¤Í»¡¡u%s¡v¡C[m\n\r", ch->short_descr, argument );
    
	for ( gch = char_list; gch; gch = gch->next )
	{
		if ( is_same_group( gch, ch ) && !IS_SET( gch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
			// ²¾°£¥Ø¼Ð mute ªº±¡ªp¤U¤£¯à¦¬¨ì°T®§ 2021/12/26
			//&& !IS_AFFECTED( gch, AFF_MUTE ) )
			send_to_char( buf, gch );
	}

	return;
}

/* Sent in by Judson Knott <jek@conga.oit.unc.edu> */
void do_beep( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) )
  return;

    argument = one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
  send_to_char( "[33m­n¹Í½Ö¡H[m\n\r", ch );
  return;
    }

    if ( !( victim = get_char_world( ch, arg ) ) )
    {
  send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
  return;
    }

    if ( IS_NPC( victim ) )
    {
  send_to_char( "[33m¥L¤£¯à³Q¹Í¡C[m\n\r", ch );
  return;
    }

    sprintf( buf, "[36m§A¹Í¤F%s¡C[m\n\r", victim->short_descr );
    send_to_char( buf, ch );
    
    sprintf( buf, "[36m\a\a%s¹Í¤F§A¡C[m\n\r", ch->short_descr );
    send_to_char( buf, victim );

    return;

}

void do_talk( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC(ch) && ( IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE) ) )
    {
        send_to_char( "¤Ñ¯«¤£²a§Aµo¥X¥ô¦óÁn­µ!\n\r", ch );
        return;
    }
    if ( IS_AFFECTED( ch, AFF_MUTE )
  || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
  || IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
  send_to_char( "[33m§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µ¶]¥X¨Ó¡C[m\n\r", ch );
  return;
    }

    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
  return;
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0'  )
    {
        send_to_char( "§A­n§i¶D¤°»ò¤H¦³Ãö¤°»ò¨Æ©O?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( victim->in_room != ch->in_room ) )
    {
        send_to_char( "§A½Í¸Üªº¹ï¶H²{¦b¦n¹³¤£¦b MUD ¥@¬É¤¤³á!\n\r", ch );
        return;
    }

    if( !IS_NPC(victim) )
    {
        send_to_char("¥Î tell ²á¤Ñ§a...\n\r",ch);
        return;
    }

    if ( !IS_AWAKE(victim) )
    {
        act( "$EºÎ±o«Ü¼ô¦ÓÅ¥¤£¨ì§A»¡¤°»ò.", ch, 0, victim, TO_CHAR );
        return;
    }

    if( argument[0] != '\0' )
    {
        act( "§A©M $N ¥æ½Í '$t'.", ch, argument, victim, TO_CHAR );
        position                = victim->position;
        victim->position        = POS_STANDING;
		/*   ¹ï mob Åã¥Ü°T®§¨S¦³¹ê½è¤Wªº·N¸q¥B·|²£¥Í comm.c Act: bad code 99.
        act( "$cfyw$n$cfwe ©M§A¥æ½Í '$t'.", ch, argument, victim, TO_VICT );
		*/
        victim->position        = position;
        victim->reply   = ch;
        mprog_talk_trigger(argument,ch,victim);
    }
    else
    {
        act( "§A¸ÕµÛ©M $N ¥æ½Í....", ch, NULL, victim, TO_CHAR );
        position                = victim->position;
        victim->position        = POS_STANDING;
		/*    ¹ï mob Åã¥Ü°T®§¨S¦³¹ê½è¤Wªº·N¸q¥B·|²£¥Í comm.c Act: bad code 99.
        act( "$cfyw$n$cfwe ¸ÕµÛ©M§A¥æ½Í.", ch, NULL, victim, TO_VICT );
		*/
        victim->position        = position;
        victim->reply   = ch;
        mprog_talk_trigger("topic",ch,victim);
    }
    return;
}

/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if( !ach || !bch ) return FALSE;
    if ( ach->deleted || bch->deleted )
  return FALSE;
    //if( ach == bch ) return TRUE;
    if( !ach->group ) return ach == bch;
    if( !IS_NPC(ach) && !IS_NPC(bch) ) 
    {
  if( ach->level - bch->level > 5 ) return FALSE;
  if( ach->level - bch->level < -5 ) return FALSE;
    }
    return (ach->group == bch->group);
    

    if ( ach->leader ) ach = ach->leader;
    if( ach == bch ) return TRUE;
    if( !IS_NPC(ach) && !IS_NPC(bch) ) {
  if( ach->level - bch->level > 5 ) return FALSE;
  if( ach->level - bch->level < -5 ) return FALSE;
    }
    if ( bch->leader ) bch = bch->leader;
    if( ach == bch ) return TRUE;
    if( !IS_NPC(ach) && !IS_NPC(bch) ) {
  if( ach->level - bch->level > 5 ) return FALSE;
  if( ach->level - bch->level < -5 ) return FALSE;
    }
    if ( ach != ach->leader ) return is_same_group( ach->leader, bch );
    if( bch != bch->leader ) return is_same_group( ach, bch->leader );
    return FALSE;
}


void do_lead( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    char       arg [ MAX_INPUT_LENGTH ];
    char       cmd [ MAX_INPUT_LENGTH ];
    char       log_buf [ MAX_INPUT_LENGTH ];
    bool       found;
    bool       fAll;
    int sklv = get_skill_level(ch, gsn_lead);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
  send_to_char( "[33m­n»â¾É½Ö°µ¤°»ò¡H[m\n\r", ch );
  return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
    send_to_char( "[33m§AÀ³¸Ó¬O­n¨ü©R¥O¡A¦Ó¤£¬Oµo©R¥O§a¡I[m\n\r", ch );
    return;
    }

  if ( ch->class != LORD || !ch->fighting || IS_NPC( ch ) )
  {
    send_to_char( "¥u¦³»â¥D¦b¾Ô°«¤¤¤~¥i¥H»â¾É§O¤H¡I\n\r", ch );
    return;
  }

  if ( //ch->pcdata->learned[gsn_lead] < number_percent() )
            get_skill_percent(ch, gsn_lead) < number_percent() )
  {
    send_to_char( "[33m§A¥¢±Ñ¤F¡I[m\n\r", ch );
    return;
  }

    if ( !str_cmp( arg, "all" ) )
    {
    fAll   = TRUE;
    victim = NULL;
    }
    else
    {
    fAll   = FALSE;
    if ( !( victim = get_char_room( ch, arg ) ) )
    {
        send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "[33m¬O¬O¬O¡A°¨¤W¿ì¡I[m\n\r", ch );
        return;
    }
    
    if ( IS_AFFECTED( victim, AFF_SUMMONED ) )
    {
      send_to_char( "[33m§AµLªk»â¾É¥l³ê¥Íª«¡I[m\n\r", ch );
      return;
    }

    if ( victim->wait > 11 + sklv *2  )
    {
        send_to_char( "[33m¥L¤w¸g¤â¦£¸}¶Ã¤F¡I[m\n\r", ch );
        return;
    }
                if( IS_AFFECTED(victim, AFF_BATTLE)){
                    send_to_char("§A¥¿¦bµo°Ê¾Ô°}¡A¨SªÅ»â¾É§O¤H", victim);
                    return;
                }
    }

    found = FALSE;
    one_argument(argument, cmd);
    
  if( !str_prefix(cmd, "battle") 
      ||  !str_prefix(cmd, "'battle array")) {
    send_to_char("¤£¥i¥H»â¾É§O¤H¨Ï¥Î°}«¬¡I\n\r", ch);
    return;
  }

  if( !str_prefix(cmd, "order")
      ||  !str_prefix(cmd, "lead") ){
    send_to_char("¤£¥i¥H»â¾É§O¤H¤U©R¥O¡I\n\r", ch);
    return;
  }

  if( !str_prefix(cmd, "quit") 
      || !str_prefix(cmd, "drop")
      || !str_prefix(cmd, "remove")
      || !str_prefix(cmd, "give"))
  {
    send_to_char("§A·Q·F¹À¡H\n\r", ch);
    return;
  }
  
  for ( och = ch->in_room->people; och; och = och_next )
  {
    och_next = och->next_in_room;

    if ( och->deleted )
      continue;
    if( IS_AFFECTED(och, AFF_BATTLE)){
      continue;
    }

    if( och == ch ) continue;
    if( !och->group || !is_same_group(ch, och))continue;
    if( IS_NPC(och) && !str_prefix( "mp", argument )) continue;

    if ( och->group->leader == ch && IS_AWAKE( och ) 
        && ( fAll || och == victim ) && och->wait < 12 + sklv*2)
    {
      if ( IS_NPC( och ) )
      {
        send_to_char( "[33m¥L¤£Ä@ªA±q§Aªº»â¾É¡C[m\n\r", ch );
        continue;
      }
      found = TRUE;
      sprintf( log_buf, "%s : lead %s", ch->name, och->name );
      strcat( log_buf, argument );
      log_string( log_buf );
      act( "[36m$n»â¾É§A $t[m", ch, argument, och, TO_VICT );
      interpret( och, argument );
    }
  }

  if ( found ){
    send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
    WAIT_STATE(ch, 6);
  }
  else
    send_to_char( "[33m³o¸Ì¨S¦³¤H¥i¥HÅ¥±q»â¾É¡C[m\n\r", ch );
  return;
}

BATTLE_ARRAY * ba_free;
void do_battle_array( CHAR_DATA *ch, char *argument )
{
    char    arg [ MAX_INPUT_LENGTH ];
    char    cmd [ MAX_INPUT_LENGTH ];
    char    buf [ MAX_INPUT_LENGTH ];
    /*char    check1 [ MAX_INPUT_LENGTH ];
    char    check2 [ MAX_INPUT_LENGTH ];
    char    *argument1;*/

    //    argument1=(char*)malloc(sizeof(argument)); 
    //   *argument1=*argument;

    int wait = 0;

    argument = one_argument( argument, arg );

    if ( ch->class != LORD || IS_NPC( ch ) )
    {
        send_to_char( "¥u¦³»â¥D¤~¥i¥H¨Ï¥Î°}§Î¡I\n\r", ch );
        return;
    }

    if ( ch->level < 19 )
    {
        send_to_char( "§Aªºµ¥¯Å¤£°÷¡I\n\r", ch );
        return;
    }

    if ( !ch->fighting )
    {
        if ( !ch->pcdata->ba )
        {
            if ( !ba_free )
                ch->pcdata->ba = alloc_mem( sizeof( BATTLE_ARRAY ) );
            else
            {
                ch->pcdata->ba = ba_free;
                ba_free = ba_free->next;
            }
            ch->pcdata->ba->ba11 = NULL;
            ch->pcdata->ba->ba12 = NULL;
            ch->pcdata->ba->ba13 = NULL;
            ch->pcdata->ba->ba21 = NULL;
            ch->pcdata->ba->ba22 = NULL;
            ch->pcdata->ba->ba23 = NULL;
            ch->pcdata->ba->ba31 = NULL;
            ch->pcdata->ba->ba32 = NULL;
            ch->pcdata->ba->ba33 = NULL;
        }

        if ( !is_number( arg ) || argument[0] == '\0' )
        {
            send_to_char( "«ü¥O¿é¤J¿ù»~¡C\n\r", ch );
            return;
        }

        /*    argument1=(char*)malloc(sizeof(argument));
         *argument1=*argument;
         one_argument(argument1, check1);
         one_argument(argument1, check2);
         if( !str_prefix(cmd, "order") )
         {
         if( !str_prefix(check2,"battle") )
         {
         send_to_char("¤£¥i¥Hbattle battle¡I\n\r", ch);
         return;      
         }
         }
         */ 
        smash_tilde( argument );
        strcpy( buf, argument );
        one_argument(buf, cmd);
        if ( !str_infix( "battle",argument)){
//        if( !str_prefix(cmd, "battle") ) {
            send_to_char("¤£¥i¥H´`Àô¨Ï¥Î°}«¬¡I\n\r", ch);
            return;
        }

        switch( atoi( arg ) )
        {
            case 11 : if ( ch->pcdata->ba->ba11 )
                          free_string( ch->pcdata->ba->ba11 );
                      ch->pcdata->ba->ba11 = str_dup( buf );
                      break;
            case 12 : if ( ch->pcdata->ba->ba12 )
                          free_string( ch->pcdata->ba->ba12 );
                      ch->pcdata->ba->ba12 = str_dup( buf );
                      break;
            case 13 : if ( ch->pcdata->ba->ba13 )
                          free_string( ch->pcdata->ba->ba13 );
                      ch->pcdata->ba->ba13 = str_dup( buf );
                      break;
            case 21 : if ( ch->pcdata->ba->ba21 )
                          free_string( ch->pcdata->ba->ba21 );
                      ch->pcdata->ba->ba21 = str_dup( buf );
                      break;
            case 22 : if ( ch->pcdata->ba->ba22 )
                          free_string( ch->pcdata->ba->ba22 );
                      ch->pcdata->ba->ba22 = str_dup( buf );
                      break;
            case 23 : if ( ch->pcdata->ba->ba23 )
                          free_string( ch->pcdata->ba->ba23 );
                      ch->pcdata->ba->ba23 = str_dup( buf );
                      break;
            case 31 : if ( ch->pcdata->ba->ba31 )
                          free_string( ch->pcdata->ba->ba31 );
                      ch->pcdata->ba->ba31 = str_dup( buf );
                      break;
            case 32 : if ( ch->pcdata->ba->ba32 )
                          free_string( ch->pcdata->ba->ba32 );
                      ch->pcdata->ba->ba32 = str_dup( buf );
                      break;
            case 33 : if ( ch->pcdata->ba->ba33 )
                          free_string( ch->pcdata->ba->ba33 );
                      ch->pcdata->ba->ba33 = str_dup( buf );
                      break;
            default : send_to_char( "²Ä´X­Ó°}§Îªº²Ä´X­Ó©R¥O¡H\n\r", ch );
        }
        send_to_char("¦nªº¡C\n\r", ch);
        return;
    }
    else
    {
        if ( !is_number( arg ) )
        {
            send_to_char( "«ü¥O¿é¤J¿ù»~¡C\n\r", ch );
            return;
        }

        if ( !ch->pcdata->ba )
        {
            send_to_char( "±zÁÙ¥¼³]©w°}§Î¡C\n\r", ch );
            return;
        }

        if( IS_AFFECTED(ch, AFF_BATTLE)){
            send_to_char("§A¥¿¦bµo°Ê¾Ô°}¡A¤£¯à±µ¨ü»â¾É",ch);
            return;
        }
        SET_BIT(ch->affected_by, AFF_BATTLE) ;
        switch( atoi( arg ) )
        {
            case 1 : if ( ch->pcdata->ba->ba11 ){
                         interpret( ch, ch->pcdata->ba->ba11 );
                     }
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     if ( ch->pcdata->ba->ba12 )
                         interpret( ch, ch->pcdata->ba->ba12 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     if ( ch->pcdata->ba->ba13 )
                         interpret( ch, ch->pcdata->ba->ba13 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     break;
            case 2 : if ( ch->pcdata->ba->ba21 )
                         interpret( ch, ch->pcdata->ba->ba21 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     if ( ch->pcdata->ba->ba22 )
                         interpret( ch, ch->pcdata->ba->ba22 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     if ( ch->pcdata->ba->ba23 )
                         interpret( ch, ch->pcdata->ba->ba23 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     break;
            case 3 : if ( ch->pcdata->ba->ba31 )
                         interpret( ch, ch->pcdata->ba->ba31 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     if ( ch->pcdata->ba->ba32 )
                         interpret( ch, ch->pcdata->ba->ba32 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     if ( ch->pcdata->ba->ba33 )
                         interpret( ch, ch->pcdata->ba->ba33 );
                     wait = UMAX(ch->wait, wait);
                     ch->wait = 0;
                     break;
            default : send_to_char( "§A·Q¨Ï¥Î²Ä´X­Ó°}§Î¡H\n\r", ch );
                      REMOVE_BIT(ch->affected_by, AFF_BATTLE);
                      return;
        }
        WAIT_STATE(ch, wait);
        WAIT_STATE( ch, gsn_battle_array );
        REMOVE_BIT(ch->affected_by, AFF_BATTLE);
    }
}
