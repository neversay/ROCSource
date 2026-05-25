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
//*  PURPOSE     :         修改Note Read時會多讀一次note          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         修改do_tell,do_reply增加【竊聽】功能   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         修改do_tell,do_reply【竊聽】           *
//*              :             增加all 的功能及NPC不接收          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940622 jye                                    *
//*  PURPOSE     :         修改do_group增加排序功能               *
//*              :             及Protection資訊                   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940811 Razgriz                                *
//*  PURPOSE     :         修改 stop_rember(),加入 time_stamp 判斷*
//*              :         以避免 pch_next->next_in_member 可能   *
//*              :         造成無窮迴圈的情形                     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941006 Razgriz                                *
//*  PURPOSE     :         修改 do_order, mob 不能 snare, untangle*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941006 jye                                    *
//*  PURPOSE     :         修改do_order不可使用公頻               *
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
           send_to_char( "[33m你沒有沒讀過的信。[m\n\r", ch );
           return;
       }
       else if ( is_number( argument ) )
       {
           fAll = FALSE;
           anum = atoi( argument );
       }
       else
       {
           send_to_char( "[33m要讀哪一號的信？[m\n\r", ch );
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
            send_to_char( "[33m沒有這封信。[m\n\r", ch );
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
          send_to_char( "[33m信太長了。[m\n\r", ch );
          return;
      }
      
      strcat( buf, argument );
      strcat( buf, "\n\r"   );
      free_string( ch->pnote->text );
      ch->pnote->text = str_dup( buf );
      send_to_char( "[36m沒問題。[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "subject" ) )
  {
      note_attach( ch );
      free_string( ch->pnote->subject );
      ch->pnote->subject = str_dup( argument );
      send_to_char( "[36m沒問題。[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "to" ) )
  {
       note_attach( ch );
       free_string( ch->pnote->to_list );
       ch->pnote->to_list = str_dup( argument );
       send_to_char( "[36m沒問題。[m\n\r", ch );
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
      
      send_to_char( "[36m沒問題。[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "show" ) )
  {
      if( !ch->pnote )
      {
          send_to_char( "[33m你沒有正在寫的信。[m\n\r", ch );
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
          send_to_char( "[33m你沒有正在寫的信。[m\n\r", ch );
          return;
      }
      
      if( !str_cmp( ch->pnote->to_list, "" ) )
      {
          send_to_char("[33m你得要寫個收件者（名字、all、或是immortal）。[m\n\r", ch );
          return;
      }
      
      if( !str_cmp( ch->pnote->subject, "" ) )
      {
          send_to_char( "[33m你得要寫個標題。[m\n\r", ch );
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
      
      send_to_char( "[36m沒問題。[m\n\r", ch );
      return;
  }

  if( !str_cmp( arg, "remove" ) )
  {
      if( !is_number( argument ) )
      {
          send_to_char( "[33m要砍哪一號的信？[m\n\r", ch );
          return;
      }

      anum = atoi( argument );
      vnum = 0;
      for( pnote = note_list; pnote; pnote = pnote->next )
      {
          if( is_note_to( ch, pnote ) && vnum++ == anum )
          {
              note_remove( ch, pnote );
              send_to_char( "[36m沒問題。[m\n\r", ch );
              return;
          }
      }
      
      send_to_char( "[33m沒有這封信。[m\n\r", ch );
      return;
  }

  send_to_char( "[33m啥？打「help note」看看怎麼用。[m\n\r", ch );
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
		sprintf( buf, "[33m要說什麼？[m\n\r" );
		send_to_char( buf, ch );
		return;
	}

	if( strlen(argument) > 150 ) {
		send_to_char("輸入過長.\n\r", ch);
		return;
	}

	if( !ch->in_room ) return;

	if ( !IS_NPC( ch ) && ( IS_SET(ch->act, PLR_SILENCE) || ( IS_SET(ch->act, PLR_HORSE) && ( channel != CHANNEL_CHAT ) ) ) )
	{
		sprintf( buf, "[33m你不能使用 %s 頻道。[m\n\r", verb );
		send_to_char( buf, ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| ( IS_SET( race_table[ch->race].race_abilities, RACE_MUTE ) && !IS_SET( ch->act, PLR_HORSE ) )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "[33m你的嘴唇在動，但是沒有聲音跑出來。[m\n\r", ch );
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
			sprintf( buf, "%s你%s：「%s%s」[m\n\r",color,verb,  argument,color );
			send_to_char( buf, ch );
			sprintf( buf, "%s$n%s%s著：「$t%s」"NOR"",color,color,verb,     color );
			break;
		default:
			sprintf( buf, "%s%s你說：「%s%s」[m\n\r",color,verb,  argument,color );
			send_to_char( buf, ch );
			sprintf( buf, "%s%s$n%s：「$t%s」"NOR"",color,verb, color,    color );
			break;

		case CHANNEL_SYSTEM:
			sprintf( buf, "%s【系統】%s 從 %s 連線進入."NOR"", color,ch->name, ch->desc->host );
			break;

		case CHANNEL_IMMTALK:
			sprintf( buf, "%s%s你說：$t[m", HIR,verb );
			position  = ch->position;
			ch->position      = POS_STANDING;
			act( buf, ch, argument, NULL, TO_CHAR );
			ch->position      = position;
			sprintf( buf, HIR"%s$n"HIR"：「$t"HIR"」"NOR"",     verb );
			break;
		case CHANNEL_RUMOR:
			fRecall = TRUE;
			sprintf( buf, "%s%s你說：「%s%s」[m\n\r",HIP, verb, argument,HIP );
			send_to_char( buf, ch );
			sprintf( buf, "%s%s：「$t%s」[m",  HIP, verb,  HIP);
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
			// 原 bid 喊價只在 auc room 能看得到, 變更為與 chat 相同 2020/11/08
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
				act( HIG"你發現是"NOR"$n"HIG"在散播謠言。"NOR, ch, NULL, vch, TO_VICT );  // 2021/10/04 使人物色碼不影響到後續字串
			vch->position = position;
		}
	}
	return;
}



void do_auction( CHAR_DATA *ch, char *argument )
{
    //talk_channel( ch, argument, CHANNEL_AUCTION, "拍賣",HIY );
    
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
			send_to_char(CYN"目前沒有東西在拍賣！"NOR"\n\r", ch);
			return;
		}
		for(auction = auction_list ; auction; auction = auction->next ){
			if( is_pfxname(arg2, auction->obj->name)) break;
		}
		if( !auction ) {
			send_to_char(CYN"找不到這樣東西！"NOR"\n\r", ch);
			return;
		}
		obj = auction->obj;

		if( ch == auction->owner )
		return;

		if( ch->gold < 2000 )
		{
			send_to_char(CYN"你付不起鑑定費用！"NOR"\n\r", ch );
			return;
		}
		ch->gold = ch->gold - 2000;

		obj_cast_spell(skill_lookup("identify"), ch->level+6, ch, NULL, obj);
		return;
	}

	if( !IS_SET(ch->in_room->room_flags, ROOM_AUCTION))
	{
		send_to_char(CYN"這裡不能進行拍賣！"NOR"\n\r", ch);
		return;
	}else if(arg1[0] == '\0'){
		send_to_char(CYN"你要賣什麼？"NOR"\n\r", ch);
		return;
	}

    if( auction_list && arg1[0] != '\0' && str_cmp( arg1, "id") 
      && str_cmp( arg1, "cancel") )
    {
        if( is_qmark(ch, "auction_cancel") ){
            send_to_char("你由於之前取消拍賣，因此需要一段時間才能再進行拍賣。\n\r", ch);
            return;
        }

  if( !( obj = get_obj_carry( ch, arg1 )))
  {
      if( (obj = get_obj_wear( ch, arg1 )))
    send_to_char("你得先脫下它。\n\r", ch);
      else
    send_to_char("你身上沒有那樣東西。\n\r", ch);
      return;
  }


  for( auction = auction_list ; auction ; auction = auction->next ){
      if( !str_cmp(obj->name, auction->obj->name )){
    send_to_char("抱歉，目前有一場同名的拍賣正在進行中，下次請早。\n\r", ch );
    return;
      }
      if( ch == auction->owner ) {
    send_to_char("你已經有一場拍賣在進行了。\n\r", ch);
    return;
      }
  }
    }
    if( arg1[0] == '\0' )
    {
      if( auction_list )
  {
      send_to_char("要拍賣什麼？\n\r", ch);
      return;
  }

  for( auction = auction_list ;auction; auction = auction->next ){
      obj = auction->obj;
      sprintf( buf, CYN"目前正在拍賣的物品是：%s,\n\r形式：%s(%s),\n\r特性：%s,\n\r等級：%d,\n\r重量是 %d 公斤, 目前的價位是 %d 枚金幣。"NOR"\n\r",
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
      send_to_char("目前沒有拍賣正在進行！\n\r", ch);
      return;
  }
  for( auction = auction_list; auction; auction = auction->next ){
      if( ch == auction->owner ) break;
      if( IS_IMMORTAL(ch) && is_pfxname( arg2, auction->obj->name )) break;
  }
  if( !auction )
  {
      send_to_char(CYN"只有賣方可以中止拍賣！"NOR"\n\r", ch);
      return;
  }
  if( ( !auction->buyer && auction->time < 1 ) || (ch != auction->owner && get_trust( ch ) < L_JUN) )
  {
      send_to_char(CYN"你不能中止拍賣！"NOR"\n\r", ch);
      return;
  }

  if( !auction->buyer )
    ;
  else if( auction->price && auction->owner->gold < auction->price / 10 )
  {
    send_to_char(CYN"你的錢不夠中止拍賣！"NOR"\n\r",ch);
    return;
  }
  else
    auction->owner->gold -= auction->price / 10;

  sprintf(buf, "本人決定，取消%s的拍賣！", auction->obj->short_descr );
  talk_channel( ch, buf, CHANNEL_AUCTION, "【拍賣】",HIY );
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
      send_to_char(CYN"這裡不能進行拍賣！"NOR"\n\r", ch);
      return;
    }
    if( !( obj = get_obj_carry( ch, arg1 )))
    {
  if( (obj = get_obj_wear( ch, arg1 )))
      send_to_char("你得先脫下它。\n\r", ch);
  else
      send_to_char("你身上沒有那樣東西。\n\r", ch);
  return;
    }

    if( !obj ) {
  bug("do_auction; No OBJ!!", 0 );
  return;
    }

    if( !can_drop_obj( ch, obj ))
    {
      send_to_char("你無法放開這樣東西。\n\r", ch );
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

    sprintf( buf,"%s，起價 %d 枚金幣，歡迎各界競標，地點在%s。",
        obj->short_descr,
        price,
        ch->in_room->name
    );
    talk_channel( ch, buf, CHANNEL_AUCTION, "【拍賣】",HIY );
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
      send_to_char( "你至少要達到第三級才能閒聊，試試 question 吧。\n\r", ch );
      return; 
    }
    talk_channel( ch, argument, CHANNEL_CHAT, "【閒聊】",HIC );
    return;
}


void do_bet( CHAR_DATA *ch, char *argument )
{
	//talk_channel( ch, argument, CHANNEL_AUCTION, "喊價",WHT );
    
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
		send_to_char(CYN"目前並沒有物品在拍賣中。"NOR"\n\r", ch);
		return;
	}

	if( arg1[0] == '\0')
	{
		for( auction = auction_list ; auction ; auction = auction->next )
		{
			if( ch == auction->owner )
			{
				if( auction->buyer )
					sprintf( buf, CYN"%s目前的價位是 %d 枚金幣。"NOR"\n\r",
								auction->obj->short_descr,
								auction->price );
				else
					sprintf( buf, CYN"%s目前沒有任何人願意出價，底價是 %d 枚金幣。"NOR"\n\r",
								auction->obj->short_descr,
								auction->price);

				send_to_char( buf, ch );
			}
			else
			{
				sprintf( buf, "目前正在拍賣 %s, 價位是 %d 枚金幣。",
								auction->obj->short_descr,
								auction->price);
				if( auction->buyer )
				{
					strcat( buf, "出價最高的是" );
					strcat( buf, ( ch == auction->buyer ) ? "你" : auction->buyer->name);
					strcat( buf, "。");
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
		send_to_char(CYN"沒有這一類的物品在拍賣。"NOR"\n\r", ch);
		return;
    }


	obj = auction->obj;
	if( !can_see_obj( ch, obj ))
	{
		send_to_char(CYN"你看不到這樣物品。"NOR"\n\r", ch);
		return;
	}
	if( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ))
	{
		send_to_char(CYN"你無法再拿更多東西了。"NOR"\n\r", ch );
		return;
	}
	if( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ))
	{
		send_to_char(CYN"你再也拿不動了。"NOR"\n\r", ch);
		return;
	}


	if( ch == auction->buyer )
	{
		send_to_char("目前出最高價的已經是你了，不需要再叫價。\n\r", ch);
		return;
	}

    if( !IS_SET(ch->in_room->room_flags, ROOM_AUCTION))
    {
		send_to_char(CYN"這裡不能進行拍賣！"NOR"\n\r", ch);
		return;
    }

	if( ch->in_room->area->recall != auction->recall ) {
		send_to_char(CYN"你正處在不同的時空之中，無法進行叫價！"NOR"\n\r", ch);
		return;
	}

	if( !is_number( arg2 ))
	{
		send_to_char("出價請用「數字」！\n\r", ch);
		return;
	}
	price = atoi( arg2 );
	if( price > ch->gold )
	{
		send_to_char("你沒有這麼多錢。\n\r", ch);
		return;
	}
	if( price > 2100000000 ) price = 2100000000;
	if( price < (auction->price + auction->price / 20 ))
	{
		send_to_char("你太小氣了，再出多一點吧。\n\r", ch);
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
		sprintf( buf, "我出價 %d 枚金幣購買 %s。", price, auction->obj->short_descr);
	}
	talk_channel( ch, buf, CHANNEL_BET, "【拍賣】",HIC );
	return;
}

/*
 * Alander's new channels.
 */
void do_music( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_MUSIC, "【心情點播】",WHT );
    return;
}



void do_question( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
      return;

    talk_channel( ch, argument, CHANNEL_QUESTION, "【新手發問】" ,HIG);
    return;
}



void do_answer( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC( ch ) )
      return;

    talk_channel( ch, argument, CHANNEL_QUESTION, "【老手回答】" ,HIG);
    return;
}



void do_shout( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_SHOUT, "喊叫",HIY );
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
    talk_channel( ch, argument, CHANNEL_YELL, "吶喊",HIY );
    return;
}


void do_herotalk( CHAR_DATA *ch, char *argument )
{
    if( IS_NPC(ch) ) return;

    talk_channel( ch, argument, CHANNEL_HEROTALK, "【英雄茶館】" , HIG);
    return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
  
    rch = get_char( ch );

    if ( !authorized( rch, "immtalk" ) )
  return;

    talk_channel( ch, argument, CHANNEL_IMMTALK, "【神諭】" ,HIR);
    return;
}

void do_rumor( CHAR_DATA *ch, char *argument )
{
  
    if ( ch->level < 10 )
    {
      send_to_char( "你至少要達到第十級才有人肯相信你的謠言。\n\r", ch );
      return; 
    }
    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
  return;
    talk_channel( ch, argument, CHANNEL_RUMOR, "【幻世謠傳】" ,HIP);
    return;
}


void do_say( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m要說什麼？[m\n\r", ch );
  return;
    }
    if ( IS_AFFECTED( ch, AFF_MUTE )
  || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
  || IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
  send_to_char( "[33m你的嘴唇在動，但是沒有聲音跑出來。[m\n\r", ch );
  return;
    }

    argument = makedrunk( argument, ch );

    act( "$n"HIG"說著「$T」"NOR, ch, NULL, argument, TO_ROOM );
    act( HIG "你說「$T」"NOR, ch, NULL, argument, TO_CHAR );

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
      send_to_char( "[33m你的嘴唇在動，但是沒有聲音跑出來。[m\n\r", ch );
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
      send_to_char( "[33m他不在這兒。[m\n\r", ch );
      return;
  }

  if( victim == ch ) 
  {
      send_to_char("你對著自己喃喃自語。\n\r", ch);
      return;
  }

  if( ( !IS_NPC( ch ) && ( IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE) 
           || IS_SET( ch->act, PLR_NO_TELL ) ) )
           || IS_SET( victim->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
  {
      send_to_char( "[33m你的聲音傳不過去。[m\n\r", ch );
      return;
  }

  if( !IS_NPC(victim) && !victim->desc && victim->pcdata->horse )
  {
      victim->reply  = ch;
      victim = victim->pcdata->horse;
  }


  if( !victim->desc )
  {
      act( "[33m$N斷線了。[m", ch, 0, victim, TO_CHAR );
      return;
  }

  argument = one_argument( argument, arg );

  if( arg[0] == '\0' || argument[0] == '\0' )
  {
      send_to_char( "[33m要告訴誰什麼事？[m\n\r", ch );
      return;
  }

  if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
  {
      act( "[33m$E聽不到你的聲音。[m", ch, 0, victim, TO_CHAR );
      return;
  }

  argument = makedrunk( argument, ch );

  act( ""HIY"你告訴 $N「$t」"NOR"", ch, argument, victim, TO_CHAR );
  position        = victim->position;
  victim->position      = POS_STANDING;
  act( "$n"HIY"告訴你「$t」"NOR"", ch, argument, victim, TO_VICT );
  victim->position      = position;
  victim->reply  = ch;
  
	if( IS_SET( victim->act, PLR_AFK ) )
		act( "[33m你也知道的嘛，$E現在不在電腦前面。[m", ch, NULL, victim, TO_CHAR );
      
	for ( d = descriptor_list; d; d = d->next ) 
	{
		to = (d->original) ? d->original : d->character;
      
		if( !to ) continue;
		if( !IS_IMMORTAL( to ) 
			|| IS_IMMORTAL( ch )
			|| IS_NPC( ch ))
			continue;
		/*
			避免傳入 str_cmp 後產生大量的 null bstr 2022/01/05 

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
			sprintf(buf, HIB"【竊聽】%s"HIB"傳送訊息給%s"HIB"「%s」"NOR"\n\r",ch->short_descr,victim->short_descr,argument);
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
      send_to_char( "[33m你的嘴唇在動，但是沒有聲音跑出來。[m\n\r", ch );
      return;
  }

  if( !( victim = ch->reply ) )
  {
      send_to_char( "[33m他不在這兒。[m\n\r", ch );
      return;
  }

  if( ( !IS_NPC( ch ) 
        && ( IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE)
            || IS_SET( ch->act, PLR_NO_TELL ) ) )
            || IS_SET( victim->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
  {
      send_to_char( "[33m你的聲音傳不過去。[m\n\r", ch );
      return;
  }

  if( argument[0] == '\0' )
  {
      send_to_char( "[33m要回應什麼？[m\n\r", ch );
      return;
  }

  if( !IS_NPC(victim) && !victim->desc && victim->pcdata->horse )
  {
      victim->reply  = ch;
      victim = victim->pcdata->horse;
  }

  if( !victim->desc )
  {
      act( "[33m$N斷線了。[m", ch, 0, victim, TO_CHAR );
      return;
  }

  if( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
  {
      act( "[33m$E聽不到你的聲音。[m", ch, 0, victim, TO_CHAR );
      return;
  }

  argument = makedrunk( argument, ch );

  act( ""HIY"你告訴$N「$t」"NOR"",  ch, argument, victim, TO_CHAR );
  position        = victim->position;
  victim->position      = POS_STANDING;
  act( HIY"$n告訴你「$t」"NOR"", ch, argument, victim, TO_VICT );
  victim->position      = position;
  victim->reply  = ch;

  if( IS_SET( victim->act, PLR_AFK ) )
      act( "[33m你也知道的嘛，$E現在不在電腦前面。[m", ch, NULL, victim, TO_CHAR );
        
	for ( d = descriptor_list; d; d = d->next ) 
	{
		to = (d->original) ? d->original : d->character;
      
		if( !to ) continue;
		if( !IS_IMMORTAL( to ) 
			|| IS_IMMORTAL( victim )
			|| IS_NPC( victim ))
			continue;
		/*
			避免傳入 str_cmp 後產生大量的 null bstr 2022/01/05 

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
			sprintf(buf, HIB"【竊聽】%s"HIB"傳送訊息給%s"HIB"「%s」"NOR"\n\r",ch->short_descr,victim->short_descr,argument);
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
  send_to_char( "[33m你沒辦法表達你的情感。[m\n\r", ch );
  return;
    }

    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m表達什麼情感？[m\n\r", ch );
  return;
    }

    for ( plast = argument; *plast != '\0'; plast++ )
  ;

    strcpy( buf, argument );
    if ( isalpha( plast[-1] ) )
  strcat( buf, "。" );

	act( "$n"NOR"$T。[m", ch, NULL, buf, TO_ROOM ); // 2021/10/04  使預設 emote 無顏色且名字顏色不受影響
	act( "$n"NOR"$T。[m", ch, NULL, buf, TO_CHAR );
	//act( "[36m$n$T。[m", ch, NULL, buf, TO_ROOM );
	//act( "[36m$n$T。[m", ch, NULL, buf, TO_CHAR );
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
      "[36m你散發出能量。[m",
      "[36m$n散發出能量。[m",
      "[36m你感到很聖潔。[m",
      "[36m$n看起來很聖潔。[m",
      "[36m你玩了個打牌的小技倆。[m",
      "[36m$n玩了個打牌的小技倆。[m",
      "[36m你現出你的大肌肉。[m",
      "[36m$n現出$s大肌肉。[m",
      "[36m你要用碟仙停下來嗎？[m",
      "[36m太好了，$n又在玩$s碟仙了。[m"
  }
    },

    {
  {
      "[36m你變成一隻蝴蝶，然後又變回你正常的樣子。[m",
      "[36m$n變成一隻蝴蝶，然後又變回$s正常樣子。[m",
      "[36m你無動於衷地把酒變成水。[m",
      "[36m$n無動於衷地把酒變成水。[m",
      "[36m你交替地動了動你的耳朵。[m",
      "[36m$n交替地動了動$s耳朵。[m",
      "[36m你用你的手指把堅果捏開。[m",
      "[36m$n用$s的手指把堅果捏開。[m",
      "[36m你讀了大家的內心...然後厭惡得發抖。[m",
      "[36m$n讀了你的內心...喲，你墮入了邪道！[m"
  }
    },

    {
  {
      "[36m藍色的閃光從你的指間冒出。[m",
      "[36m藍色的閃光從$n的指間冒出。[m",
      "[36m你的頭上出現了一道光環。[m",
      "[36m$n的頭上出現了一道光環。[m",
      "[36m你很迅速地給自己打了個蝴蝶結。[m",
      "[36m$n很迅速地給$m自己打了個蝴蝶結。[m",
      "[36mYou grizzle your teeth and look mean.[m",
      "[36m$n grizzles $s teeth and looks mean.[m",
      "[36m你把你的學校全勤獎現給大家看。[m",
      "[36m$n的學校全勤獎引不起你的注意。嘖。[m"
  }
    },

    {
  {
      "[36m紅色的光芒在你的眼睛裡舞動。[m",
      "[36m紅色的光芒在$n的眼睛裡舞動。[m",
      "[36m你朗誦著智慧之言。[m",
      "[36m$n朗誦著智慧之言。[m",
      "[36m你用匕首、蘋果和眼球耍把戲。[m",
      "[36m$n用匕首、蘋果和眼球耍把戲。[m",
      "[36m你打了打你的頭，然後轉了轉你的眼睛。[m",
      "[36m$n打了打$s頭，然後轉了轉$s眼睛。[m",
      "[36m一團鬼火跟著你的拖鞋來了。[m",
      "[36m一團鬼火跟著$n的扡鞋來了。[m"
  }
    },

    {
  {
      "[36m一個有綠色黏液的怪物在你面前出現，向你鞠躬。[m",
      "[36m一個有綠色黏液的怪物在$n面前出現，向$n鞠躬。[m",
      "[36m在冗長的禱告之下，你浮了起來。[m",
      "[36m在冗長的禱告之下，$n浮了起來。[m",
      "[36m你偷了這裡每個人的內衣。[m",
      "[36m你的內衣不見了！$n把它偷走了！[m",
      "[36m喀啦、喀啦...你正嚼著一個瓶子。[m",
      "[36m喀啦、喀啦...$n正嚼著一個瓶子。[m",
      "[36m多出來的那隻腳是幹嘛的呢？[m",
      "[36m為什麼剛剛$n多長出了一隻腳呢？[m"
  }
    },

    {
  {
      "[36m你把大家都變成了粉紅色的小象。[m",
      "[36m你被$n變成了一隻粉紅色小象。[m",
      "[36m有一個天使在請教你。[m",
      "[36m在一個天使在請教$n。[m",
      "[36m骰子轉動著...你又贏了。[m",
      "[36m骰子轉動著...$n又贏了。[m",
      "[36m...998、999、1000...你在做伏地挺身。[m",
      "[36m...998、999、1000...$n在做伏地挺身。[m",
      "[36m那只湯匙在你開始集中精神的時候逃跑了。[m",
      "[36m那只湯匙在$n開始集中精神的時候逃跑了。[m"
  }
    },

    {
  {
      "[36m一個小光球在你的指尖跳舞。[m",
      "[36m一個小光球在$n的指尖跳舞。[m",
      "[36m你的身體發出神秘的光芒。[m",
      "[36m$n的身體發出神秘的光芒。[m",
      "[36m你在算大家口袋裡的錢。[m",
      "[36m檢查你的錢，$n正在數。[m",
      "[36m阿諾史瓦辛格讚嘆你的體格。[m",
      "[36m阿諾史瓦辛格讚嘆$n的體格。[m",
      "[36m不要在別人面前搖動你的大腦。[m",
      "[36m叫$n不要在你的面前搖動$s大腦！[m"
  }
    },

    {
  {
      "[36m煙霧從你的鼻孔漏出來。[m",
      "[36m煙霧從$n的鼻孔漏出來。[m",
      "[36m聚光燈照向你。[m",
      "[36m聚光燈照向$n。[m",
      "[36m你把一把小刀平衡在舌頭上。[m",
      "[36m$n把一把小刀平衡在舌頭上。[m",
      "[36m小心你的腳，你正在耍玩花崗岩圓石。[m",
      "[36m小心你的腳，$n正在耍玩花崗岩圓石。[m",
      "[36mMENSA打電話來...他們要問你對某事的意見。[m",
      "[36mMENSA剛剛打電話向$n請教。[m"
  }
    },

    {
  {
      "[36m光線在你念咒文的時候閃爍不定。[m",
      "[36m光線在$n念咒文的時候閃爍不定。[m",
      "[36m你禱告的時候大家都浮了起來。[m",
      "[36m你在$n禱告的時候浮了起來。[m",
      "[36m你從每個人的耳朵裡變出一枚硬幣。[m",
      "[36m$n從你的耳朵裡變出一枚硬幣。[m",
      "[36m嗚～～你從花崗岩圓石裡擠出水來了。[m",
      "[36m嗚～～$n從花崗岩圓石裡擠出水來了。[m",
      "[36m椅子在你最微弱的意念之下在房裡飛舞。[m",
      "[36m椅子在$n最微弱的意念之下在房裡飛舞。[m"
  }
    },

    {
  {
      "[36m你的頭消失了。[m",
      "[36m$n的頭消失了。[m",
      "[36m一陣清風讓你恢復了精神。[m",
      "[36m一陣清風使$n恢復了精神。[m",
      "[36m你走在你的影子後面。[m",
      "[36m$n走在$s影子後面。[m",
      "[36m你用長矛剔牙。[m",
      "[36m$n用長矛剔牙。[m",
      "[36m噢喔...也許你不該再召喚更多的河馬了。[m",
      "[36m噢喔！$n應該有一陣子不會再召喚更多的河馬了。[m"
  }
    },

    {
  {
      "[36m一個火之元素燒了你的頭髮。[m",
      "[36m一個火之元素燒了$n的頭髮。[m",
      "[36m太陽穿過雲層來照亮你。[m",
      "[36m太陽穿過雲層來照亮$n。[m",
      "[36m你的眼睛貪婪地躍動著。[m",
      "[36m$n的眼睛貪婪地躍動著。[m",
      "[36m你一抱大家都和自己的腳分家了。[m",
      "[36m你在$n一抱之下和你自已的腳分家了。[m",
      "[36m噢喔...你的頭髮因為想得太用力而發出吱吱的聲音。[m",
      "[36m噢喔...$n的頭髮因為想得太用力而發出吱吱的聲音。[m"
  }
    },

    {
  {
      "[36m天空改變顏色來撘配你的眼睛。[m",
      "[36m天空改變顏色來撘配$n的眼睛。[m",
      "[36m大海在你面前分開了。[m",
      "[36m大海在$n面前分開了。[m",
      "[36m你熟練地偷走了每一個人的武器。[m",
      "[36m$n熟練地偷走了你的武器。[m",
      "[36m你用手刀把樹劈開了。[m",
      "[36m$n用手刀把樹劈開了。[m",
      "[36m什麼？你正忙著集中精神。[m",
      "[36m什麼？噢，$n「又」迷失在思考中了。[m"
  }
    },

    {
  {
      "[36m石頭依照你的命令起舞。[m",
      "[36m石頭依照$n的命令起舞。[m",
      "[36m一片雷雨雲在你面前跪下。[m",
      "[36m一片雷雨雲在$n面前跪下。[m",
      "[36mThe Grey Mouser替你買了杯啤酒。[m",
      "[36mThe Grey Mouser替$n買了杯啤酒。[m",
      "[36m你護甲上的帶子因你強大的鬥氣而斷裂了。[m",
      "[36m$n護甲的帶子因為$s強大鬥氣而斷裂了。[m",
      "[36m你會在你受傷之前下來這裡嗎？[m",
      "[36m快，找跟棍子來，$n又在doing$spinata impression了。[m"
  }
    },

    {
  {
      "[36m你微笑起來天地都改變了顏色。[m",
      "[36m$n微笑起來天地都改變了顏色。[m",
      "[36mThe Burning Man跟你說話。[m",
      "[36mThe Burning Man跟$n說話。[m",
      "[36m每個人的口袋都被你的煙火炸開了。[m",
      "[36m你的口袋被$n的煙火炸開了。[m",
      "[36m你一皺眉頭，一個大圓石就裂開了。[m",
      "[36m$n一皺眉頭，一個大圓石就裂開了。[m",
      "[36m小心...不要把誰給分解了！[m",
      "[36m小心！$n要把什麼東西給分解了。[m"
  }
    },

    {
  {
      "[36m每個人的衣服都是透明的，你在笑。[m",
      "[36m你的衣服是透明的，$n在笑。[m",
      "[36m一隻金字塔裡的眼睛對著你眨眼。[m",
      "[36m一隻金字塔裡的眼睛對著$n眨眼。[m",
      "[36m大家都發現到你的匕首距離他們的眼睛只有一公分。[m",
      "[36m你發現到$n的匕首距離你的眼睛只有一公分。[m",
      "[36m傭兵都來服從你的命令。[m",
      "[36m傭兵都來服從$n的命令。[m",
      "[36m你大肆宣揚「心勝於物」的道理。[m",
      "[36m是是是，心勝於物。閉嘴$n。[m"
  }
    },

    {
  {
      "[36m一個黑洞把你吞噬。[m",
      "[36m一個黑洞把$n吞噬。[m",
      "[36mValentine Michael Smith給你一杯水。[m",
      "[36mValentine Michael Smith給$n一杯水。[m",
      "[36m你去了哪了？[m",
      "[36m$n去了哪了？[m",
      "[36m四個相稱的 組成你的戰車。[m",
      "[36m四個相稱的Percherons組成$n的戰車。[m",
      "[36m轟。[m",
      "[36m轟。[m"
  }
    },

    {
  {
      "[36m整個世界跟著你吹口哨的節奏搖擺。[m",
      "[36m整個世界跟著$n吹口哨的節奏搖擺。[m",
      "[36m偉大的神Amenda給你一枝權杖。[m",
      "[36m偉大的神Amenda給$n一枝權杖。[m",
      "[36m喀啦。[m",
      "[36m喀啦。[m",
      "[36m擎天的亞特拉斯神請求你解救他。[m",
      "[36m擎天的亞特拉斯神請求$n解救他。[m",
      "[36m你用魔力把大家的褲子吸走了...不還給他們。[m",
      "[36m你的褲子被$n用魔力吸走了，$e不還給你。[m"
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
  send_to_char( "[33m作者們用懷疑的眼神看著你。[m\n\r", ch );
  return;
    }

    append_file( ch, BUG_FILE,  argument );
    send_to_char( "[36m沒問題。謝謝你。[m\n\r", ch );
    return;
}



void do_idea( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m作者們用懷疑的眼神看著你。[m\n\r", ch );
  return;
    }

    append_file( ch, IDEA_FILE, argument );
    send_to_char( "[36m沒問題。謝謝你。[m\n\r", ch );
    return;
}



void do_typo( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m作者們用懷疑的眼神看著你。[m\n\r", ch );
  return;
    }

    append_file( ch, TYPO_FILE, argument );
    send_to_char( "[36m沒問題。謝謝你。[m\n\r", ch );
    return;
}



void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "[36m這裡不用付租金，你就存檔就是了。[m\n\r", ch );
    return;
}



void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "[33m如果你要QUIT的話，你得把它打完全。[m\n\r", ch );
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
            act( ""HIR"你不被允許儲存"NOR"$p", ch, obj, NULL, TO_CHAR );
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
				send_to_char(CYN"不行！你還在進行拍賣呢。"NOR"\n\r", ch);
				return;
			}
		}
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "[33m不行！你還在打鬥。[m\n\r", ch );
		return;
	}

	if ( phorse && phorse->position == POS_FIGHTING )
	{
		send_to_char( "[33m不行！你的馬還在打鬥。[m\n\r", ch );
		return;
	}

	if ( ch->position  < POS_STUNNED  )
	{
		send_to_char( "[33m你還沒「死」哪。[m\n\r", ch );
		return;
	}

	if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
			&& str_cmp( ch->in_room->area->identity, "skarabrae" )
			&& !IS_IMMORTAL( ch) )
	{
		send_to_char(HIC"你並不在旅店或是其他可以安全休息的地方喔！"NOR"\n\r", ch);
	}

	if ( IS_SET( ch->act, PLR_HORSE ) )
	{
		// brute force
		send_to_char("請先下馬再進行離線。\n\r", ch);
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
	affect_strip( ch, gsn_lifeshare ); // quit 移除 link cast 效果 2021/11/22


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
			send_to_char( "夏瑞斯(Sharas)替你把未關閉的倉庫關上了。\n\r", ch );
		}
		else {
			bug("eqbank: file open error!!",0);
			send_to_char("發生嚴重問題，請向大神回報。\n\r", ch);
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

	send_to_char( "[36m憑藉著你對於原來的世界的牽掛，你又回到了原來的世界。[m\n\r", ch );
	send_to_char( "[36m但是，希望你不要忘了自己的使命", ch );
	send_to_char( "和你對這個世界的承諾。[m\n\r\n\r", ch );

	act( ""CYN"一道藍色的閃光從天而降，把"NOR" $n "CYN"帶離了這個世界。"NOR"", ch, NULL, NULL, TO_ROOM );
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
			act( ""HIR"你不被允許儲存"NOR"$p", ch, obj, NULL, TO_CHAR );
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
				send_to_char(CYN"不行！你還在進行拍賣呢。"NOR"\n\r", ch);
				return;
			}
		}
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "[33m不行！你還在打鬥。[m\n\r", ch );
		return;
	}

	if ( phorse && phorse->position == POS_FIGHTING )
	{
		send_to_char( "[33m不行！你的馬還在打鬥。[m\n\r", ch );
		return;
	}

	if ( ch->position  < POS_STUNNED  )
	{
		send_to_char( "[33m你還沒「死」哪。[m\n\r", ch );
		return;
	}

	if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
			&& str_cmp( ch->in_room->area->identity, "skarabrae" )
			&& !IS_IMMORTAL( ch) )
	{
		send_to_char(HIC"你並不在旅店或是其他可以安全休息的地方喔！"NOR"\n\r", ch);
	}

	if ( IS_SET( ch->act, PLR_HORSE ) )
	{
		// brute force
		send_to_char("請先下馬再進行離線。\n\r", ch);
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
	affect_strip( ch, gsn_lifeshare ); // quit 移除 link cast 效果 2021/11/22

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
			send_to_char( "夏瑞斯(Sharas)替你把未關閉的倉庫關上了。\n\r", ch );
		}
		else {
			bug("eqbank: file open error!!",0);
			send_to_char("發生嚴重問題，請向大神回報。\n\r", ch);
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

	send_to_char( "[36m憑藉著你對於原來的世界的牽掛，你又回到了原來的世界。[m\n\r", ch );
	send_to_char( "[36m但是，希望你不要忘了自己的使命", ch );
	send_to_char( "和你對這個世界的承諾。[m\n\r\n\r", ch );

	act( ""CYN"一道藍色的閃光從天而降，把"NOR" $n "CYN"帶離了這個世界。"NOR"", ch, NULL, NULL, TO_ROOM );
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
			act( ""HIR"你不被允許儲存"NOR"$p", ch, obj, NULL, TO_CHAR );
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
				act( ""HIR"你不被允許儲存"NOR"$p", phorse, obj, NULL, TO_CHAR );
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
    send_to_char( "[33m如果你要SUICIDE的話，你得把它打完全。[m\n\r", ch );
    return;
}



void do_suicide( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_STRING_LENGTH ];
    FILE *fp;

    if ( IS_NPC( ch ) )
  return;

    if( IS_IMMORTAL(ch )){
  send_to_char("神不可以自殺。\n\r", ch);
  return;
    }

    if( IS_SET(ch->act, PLR_HORSE)){
  send_to_char("請先下馬。\n\r", ch);
  return;
    }

    if ( argument[0] == '\0' ) {
  send_to_char( "[33m使用方法：suicide <password>[m\n\r", ch );
  send_to_char( "[33m請打「help suicide」看看說明。[m\n\r", ch ); 
  return;
    }
  
    if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) ) {
  send_to_char( "[33m你的密碼不對。[m\n\r", ch );
  return;
    }

    if ( ch->position == POS_FIGHTING )
    {
  send_to_char( "[33m不行！你還在打鬥。[m\n\r", ch );
  return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
  send_to_char( "[33m你還沒「死」哪。[m\n\r", ch );
  return;
    }

    sprintf( buf, "[36m%s抱著堅定的決心向這個世界告別。\n\r[m", ch->short_descr );
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
    send_to_char( "[33m如果你要HSUICIDE的話，你得把它打完全。[m\n\r", ch );
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
  send_to_char("神不可以自殺。\n\r", ch);
  return;
    }

    if( IS_SET(ch->act, PLR_HORSE)){
  send_to_char("請先下馬。\n\r", ch);
  return;
    }

  if ( !( phorse = ch->pcdata->horse ) )
  {
    send_to_char("沒看到你的馬喔。\n\r", ch);
    return;
  }
  
    if ( argument[0] == '\0' ) {
  send_to_char( "[33m使用方法：suicide <password>[m\n\r", ch );
  send_to_char( "[33m請打「help suicide」看看說明。[m\n\r", ch ); 
  return;
    }
  
    if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) ) {
  send_to_char( "[33m你的密碼不對。[m\n\r", ch );
  return;
    }

    if ( ch->position == POS_FIGHTING )
    {
  send_to_char( "[33m不行！你還在打鬥。[m\n\r", ch );
  return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
  send_to_char( "[33m你還沒「死」哪。[m\n\r", ch );
  return;
    }

    sprintf( buf, "[36m%s殘忍地殺掉他的坐騎！真是可憐的東西，跟錯主人了...\n\r[m", ch->short_descr );
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
  send_to_char( "[33m你最少要到第二級以上才能存檔。[m\n\r", ch );
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
  send_to_char(HIC"你並不在旅店或是其他可以安全休息的地方喔！"NOR"\n\r", ch);
    }

//  WAIT_STATE( ch, 6 );

    save_char_obj( ch );
        
    send_to_char( "[36m存檔完畢。[m\n\r", ch );
    return;
}

void do_save( CHAR_DATA *ch, char *argument )
{
	if ( IS_NPC( ch ) )
		return;

	if( !ch->desc ) return;

	if ( ch->level < 2 && ch->class <= BARBARIAN )
	{
		send_to_char( "[33m你最少要到第二級以上才能存檔。[m\n\r", ch );
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
		send_to_char(HIC"你並不在旅店或是其他可以安全休息的地方喔！"NOR"\n\r", ch);
	}

	//  WAIT_STATE( ch, 6 );

	save_char_obj( ch );

	send_to_char( "[36m存檔完畢。[m\n\r", ch );

	/* horse save 沒有作用 2024/01/29
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

    sprintf(buf, CYN"【$n擲了%d面的骰子-------------\n  結果是 %d 點】"NOR,dice, total);
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
		send_to_char( "[33m要跟隨誰？[m\n\r", ch );
		return;
	}

	if( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "[33m他不在這兒。[m\n\r", ch );
		return;
	}

	if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master )
	{
		act( "[33m可是你比較想跟隨$N！[m", ch, NULL, ch->master, TO_CHAR );
		return;
	}

	/*  移除aff_summoned 的生物無法 執行 follow 的判定
	if( IS_AFFECTED(ch, AFF_SUMMONED)){
	  send_to_char("你無法跟隨主人以外的目標。\n\r", ch);
	  return;
	}
	*/

	// 讓aff_summoned的生物(召喚物)  只能執行 follow 主人跟 self  19/11/27
	if( IS_AFFECTED(ch, AFF_SUMMONED) && victim != ch && victim != ch->rembch){
		send_to_char("你無法跟隨主人以外的目標。\n\r", ch);
		return;
	}

	// follow self
	if( victim == ch )
	{
		if( !ch->master )
		{
			send_to_char( "[33m你已經只跟隨你自己了。[m\n\r", ch );
			return;
		}
		stop_follower( ch );
		return;
	}

	// 已經在 follow ch 的情況
	if( victim == ch->master ) {
	   send_to_char("你已經在跟隨著他了\n\r",ch);
	   return;
	}

	// pk room 無法 follow
	if( victim->in_room && IS_SET(victim->in_room->room_flags, ROOM_PK)){
	   send_to_char("PK 場裡不能 follow。\n\r", ch);
	   return;
	}

    /*if ( ( ch->level - victim->level < -5
		|| ch->level - victim->level >  5 )
		&& !IS_HERO( ch ) )
		{
		send_to_char( "[33m你的能力和他相差太多了。[m\n\r", ch );
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
		// 移除不必要的 bug log 2022/11/14
		//bug( "Add_follower: non-null master.", 0 );
		//bugf( "Add_follower: non-null master. ch: %s ", ch->name );
		return;
	}

	ch->master        = master;
	//ch->leader        = NULL;

	if ( can_see( master, ch ) )
	act( "[36m$n開始跟隨著你。[m", ch, NULL, master, TO_VICT );

	act( "[36m你開始跟隨著$N。[m",  ch, NULL, master, TO_CHAR );

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

// 新增 obj rember 的指令 2022/11/10
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
    act( "你決定在戰鬥中小心地保護$N不受到傷害。", ch, NULL, pch, TO_CHAR );
    act( "$n決定在戰鬥中小心地保護$N不受到傷害。", ch, NULL, pch, TO_ROOM );
}

// 加入 ch 協助 pch 2021/11/20
void add_cooperator( CHAR_DATA *ch, CHAR_DATA *pch )
{
	if( ch->cooperating == pch ) {
		bugf("duplicated cooperating: [%s] -> [%s]",ch->name, pch->name);
		return;
	}
	ch->next_in_coop = pch->cooperator;
	pch->cooperator = ch;
	ch->cooperating = pch;
	act( "你決定從旁協助$N。", ch, NULL, pch, TO_CHAR );
	act( "$n決定從旁協助$N。", ch, NULL, pch, TO_ROOM );
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

// 新增 obj rember 的指令 2022/11/10
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

/** ch 停止保護 pch 
 * last update by zcecil 2004/07/04
 */
void stop_protecting( CHAR_DATA *ch, CHAR_DATA *pch)
{
	CHAR_DATA *protector, *protector_next;
	if( !ch || !pch ) return;
	if( ch->protecting != pch ) return; // sorry, i am not protecting you
	if( pch->protector == ch ) {
		pch->protector = ch->next_in_protect;
		act( "你不願意再保護$N了！", ch, NULL, pch, TO_CHAR );
		act( "$n不願意再保護你了！", ch, NULL, pch, TO_VICT );
	}
	else
	{
		for( protector = pch->protector ; protector ; protector = protector_next ){
			protector_next = protector->next_in_protect;
			if( protector_next == ch ) {
				protector->next_in_protect = ch->next_in_protect;
				act( "你不願意再保護$N了！", ch, NULL, pch, TO_CHAR );
				act( "$n不願意再保護你了！", ch, NULL, pch, TO_VICT );
				break;
			}
		}
	}
	ch->next_in_protect = NULL;
	ch->protecting = NULL;
}

// ch 停止協助 pch 2021/11/20
void stop_cooperating( CHAR_DATA *ch, CHAR_DATA *pch)
{
	CHAR_DATA *cooperator, *cooperator_next;
	if( !ch || !pch ) return;
	if( ch->cooperating != pch ) return; // sorry, i am not cooperating you
	if( pch->cooperator == ch ) {
		pch->cooperator = ch->next_in_coop;
		act( "你不願意再協助$N了！", ch, NULL, pch, TO_CHAR );
		act( "$n不願意再協助你了！", ch, NULL, pch, TO_VICT );
	}
	else
	{
		for( cooperator = pch->cooperator ; cooperator ; cooperator = cooperator_next ){
			cooperator_next = cooperator->next_in_coop;
			if( cooperator_next == ch ) {
				cooperator->next_in_coop = ch->next_in_coop;
				act( "你不願意再協助$N了！", ch, NULL, pch, TO_CHAR );
				act( "$n不願意再協助你了！", ch, NULL, pch, TO_VICT );
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
	act( "你與$N靈魂間的聯結中斷了。", ch, NULL, pch, TO_CHAR );
	act( "你與$n靈魂間的聯結中斷了。", ch, NULL, pch, TO_VICT );
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


/** 讓所有保護 ch 的人通通停止保護 ch
 *
 */
void remove_protector( CHAR_DATA *ch)
{
	CHAR_DATA *pch, *pch_next;

	for( pch = ch->protector ; pch ; pch = pch_next ){
		pch->protecting = NULL;
		pch_next = pch->next_in_protect;
		pch->next_in_protect = NULL;
		act( "你不願意再保護$N了！", pch, NULL, ch, TO_CHAR );
		act( "$n不願意再保護你了！", pch, NULL, ch, TO_VICT );
	}
	ch->protector = NULL;
}
// 移除所有協助的人 2021/11/20
void remove_cooperator( CHAR_DATA *ch)
{
	CHAR_DATA *pch, *pch_next;

	for( pch = ch->cooperator ; pch ; pch = pch_next ){
		pch->cooperating = NULL;
		pch_next = pch->next_in_coop;
		pch->next_in_coop = NULL;
		act( "你不願意再協助$N了！", pch, NULL, ch, TO_CHAR );
		act( "$n不願意再協助你了！", pch, NULL, ch, TO_VICT );
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
		act( "[36m$n不再跟隨你了。[m", ch, NULL, ch->master, TO_VICT );
	act( "[36m你不再跟隨$N了。[m", ch, NULL, ch->master, TO_CHAR );

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
		send_to_char( "[33m要命令誰做什麼？[m\n\r", ch );
		return;
	}

	if (IS_NPC(ch))
		return;  

	if ( IS_AFFECTED( ch, AFF_CHARM ) )
	{
		send_to_char( "[33m你應該是要受命令，而不是發命令吧！[m\n\r", ch );
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
			send_to_char( "[33m他不在這兒。[m\n\r", ch );
			return;
		}
		if( !victim ) {
			send_to_char( "[33m他不在這兒。[m\n\r", ch );
			return;
		}

		if ( victim == ch )
		{
			send_to_char( "[33m是是是，馬上辦！[m\n\r", ch );
			return;
		}
		if( IS_AFFECTED(victim, AFF_BATTLE)){
			send_to_char("你正在發動戰陣，不能接受領導。\n\t",victim);
			return;
		}

		if ( !( (IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch) || !IS_AFFECTED( victim, AFF_CHARM )
					//|| ( !IS_SET( victim->act, ACT_PET ) )
					|| ( !IS_NPC(ch) && !IS_NPC(victim) && ch == victim->pcdata->horse && victim->pcdata->horse == ch ) 
					|| ( victim->master != ch && victim->clan != ch->clan ) ) ) 
		{
			send_to_char( "[33m你自己不會去做呀？[m\n\r", ch );
			return;
		}
		//if( IS_AFFECTED( victim, AFF_SUMMONED ) )
		//{
		//  victim->ordered = TRUE;
		//}
	}
	if( !str_prefix(argument, "order")
			||  !str_prefix(argument, "lead") ){
		send_to_char("不可以要求別人下命令！\n\r", ch);
		return;
	}

	//0941006 by jye
	//can't order mob or ppl do "chat"
	if( !str_prefix("chat" , argument )
			||  !str_prefix(".",argument) ){
		send_to_char("不可以要求別人使用公頻！\n\r", ch);
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
			act( "[36m$n命令你「$t」。[m", ch, argument, och, TO_VICT );
			if ( och->wait > 14 )
			{
				send_to_char( "他已經手忙腳亂了！\n\r", ch );
				return;
			}
			interpret( och, argument );
		}
	}

	if ( found )
		send_to_char( "[36m沒問題。[m\n\r", ch );
	else
		send_to_char( "[33m這裡沒有人可以聽從命令。[m\n\r", ch );
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
		sprintf( buf, "\n\r%s"NOR"%s：\n\r"//
		// 1234567890123456789012345678901234567890123456789012345678901234567890   
		"[LV  職  業]        姓        名            生命力      正在保護       保護狀態   [m\n\r", //PERS( leader, ch ) );
			leader->group ? leader->group->name : ch->short_descr, leader->group ? "" :"的隊伍");
	}
	// group -k for cooperate 2021/11/23
	else if( !str_cmp(argument , "-k") )
	{
		sprintf( buf, "\n\r%s"NOR"%s：\n\r"//
		// 1234567890123456789012345678901234567890123456789012345678901234567890   
		"[LV  職  業]        姓        名            生命力      正在協助       協助狀態   [m\n\r", //PERS( leader, ch ) );
			leader->group ? leader->group->name : ch->short_descr, leader->group ? "" :"的隊伍");
	}
	else
	{
		sprintf( buf, "\n\r%s"NOR"%s：\n\r"//
		// 1234567890123456789012345678901234567890123456789012345678901234567890   
		"[LV  職  業]        姓        名            生命力      魔法力       移動力   [m\n\r", //PERS( leader, ch ) );
			leader->group ? leader->group->name : ch->short_descr, leader->group ? "" :"的隊伍");
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
					IS_NPC( wch[i] ) ? "非玩者"
					: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
					can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
					IS_NPC( wch[i] ) ? "非玩者"
					: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
					can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
						IS_NPC( wch[i] ) ? "非玩者"
						: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
						IS_NPC( wch[i] ) ? "非玩者"
						: fChinese ? (char *)class_table[wch[i]->class].cwho_name : (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "不明人物",
						gr_stat_color(wch[i]->hit, wch[i]->max_hit),
						wch[i]->hit,
						NOR, wch[i]->max_hit,
						HIW,
						(tch = wch[i]->protecting) ? tch->name : "----",
						intCharCnt
						);
				}
			}
			else if ( !str_cmp(argument , "+r") ) // 加入 +r 參數顯示同 group 中同 room 的角色 2022/11/21
			{
				if( SameRoom == 0 )
					continue;
				if( !IS_NPC(ch) && IS_SET(ch->act, PLR_GRAPHIC))
				{
					sprintf( buf,
						NOR"[%-3d %6s] %-27s  %s%s   %s%s   %s[m\n\r",
						wch[i]->level,
						IS_NPC( wch[i] ) ? "非玩者"
										: fChinese
											? (char *)class_table[wch[i]->class].cwho_name
											: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
						IS_NPC( wch[i] ) ? "非玩者"
										: fChinese
										? (char *)class_table[wch[i]->class].cwho_name
										: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
			else if ( !str_cmp(argument , "-r") ) // 加入 -r 參數顯示同 group 中不同 room 的角色 2022/11/21
			{
				if( SameRoom == 1 )
					continue;
				if( !IS_NPC(ch) && IS_SET(ch->act, PLR_GRAPHIC))
				{
					sprintf( buf,
						NOR"[%-3d %6s] %-27s  %s%s   %s%s   %s[m\n\r",
						wch[i]->level,
						IS_NPC( wch[i] ) ? "非玩者"
										: fChinese
											? (char *)class_table[wch[i]->class].cwho_name
											: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
						IS_NPC( wch[i] ) ? "非玩者"
										: fChinese
										? (char *)class_table[wch[i]->class].cwho_name
										: (char *)class_table[wch[i]->class].who_name,
						can_see( ch, wch[i] ) ? no_ansi : "不明人物",
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
		|| !str_cmp(arg , "-k") // 協助狀態 2021/11/23
		|| !str_cmp(arg , "+r") // 同格隊員 2021/11/23
		|| !str_cmp(arg , "-r") // 隊員是否同格 2021/11/23
		|| !str_cmp(arg , "-p"))
	{
		do_show_group_info( ch , arg );
		return;
	}

	if( !str_cmp( arg, "form" ) ) 
	{
		if( ch->group ) 
		{
			send_to_char("你已經有隊伍了。\n\r", ch);
			return;
		}

		if( *arg1 == '\0' ) 
		{
			sprintf(arg1, "%s的隊伍", ch->short_descr);
		}
		else if( strlen(arg1) > 30 ) 
		{
			send_to_char("你的隊名太長了。\n\r", ch);
			return;
		}


		pG = new_group();
		pG->leader = ch;
		pG->name = str_dup(arg1);
		act( "[36m你成立了一支冒險隊伍。[m", ch, NULL, NULL, TO_CHAR    );
		act( "[36m$n成立了一支冒險隊伍。[m", ch, NULL, NULL, TO_ROOM);
		ch->next_in_group = NULL;
		ch->group = pG;
		pG->members = ch;
		return;
	}

	if( !ch->group ) 
	{
		send_to_char("你並沒有在隊伍中。\n\r", ch);
		return;
	}


	if( !str_cmp(arg, "leave"))
	{
		act( "[36m$N脫離了你的隊伍。[m", ch->group->leader, NULL, ch, TO_CHAR    );
		act( "[36m你脫離了$n的隊伍。[m", ch->group->leader, NULL, ch, TO_VICT);
		remove_member( ch);
		return;
	}



	if( ch != ch->group->leader ) 
	{
		send_to_char("只有一隊之長才可以這麼做。\n\r", ch);
		return;
	}

	pG = ch->group;

	if( !str_cmp( arg, "disband" ) ) 
	{
		act( "[36m你解散了隊伍。[m", ch, NULL, NULL, TO_CHAR    );
		act( "[36m$n解散了隊伍。[m", ch, NULL, NULL, TO_ROOM);
		clear_member(ch);
		return;
	}

	if( !str_cmp(arg, "transfer"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) || IS_NPC(victim))
		{
			send_to_char( "[33m他不在這兒。[m\n\r", ch );
			return;
		}

		if( victim == ch ) 
		{
			send_to_char("你已經是隊長了。\n\r", ch);
			return;
		}

		if( !is_same_group(ch, victim))
		{
			send_to_char("先收他入隊吧。\n\r", ch);
			return;
		}

		act( "[36m你把隊長之職交接給$N。[m", ch, NULL, victim, TO_CHAR    );
		act( "[36m$n把隊長之職交接給你。[m", ch, NULL, victim, TO_VICT    );
		act( "[36m$n把隊長之職交接給$N。[m", ch, NULL, victim, TO_NOTVICT );
		ch->group->leader = victim;
		return;
	}

	if( !str_cmp(arg, "*"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) )
		{
			send_to_char( "[33m他不在這兒。[m\n\r", ch );
			return;
		}

		if( !victim->group || victim != victim->group->leader )
		{
			send_to_char("他並不是一隊之長。\n\r", ch);
			return;
		}

		if( victim->master != ch && ch != victim )
		{
			act( "[33m$N並沒有跟隨你。[m", ch, NULL, victim, TO_CHAR );
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
		act( "[36m你併吞了$N的隊伍。[m", ch, NULL, victim, TO_CHAR    );
		act( "[36m$n併吞了你的隊伍。[m", ch, NULL, victim, TO_VICT    );
		act( "[36m$n併吞了$N的隊伍。[m", ch, NULL, victim, TO_NOTVICT );
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
				act( "[36m$N加入你的隊伍。[m", ch, NULL, victim, TO_CHAR    );
				act( "[36m你加入$n的隊伍。[m", ch, NULL, victim, TO_VICT    );
				act( "[36m$N加入$n的隊伍。[m", ch, NULL, victim, TO_NOTVICT );
			}
		}
		return;
	}

	if( !str_cmp(arg, "+"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) )
		{
			send_to_char( "[33m他不在這兒。[m\n\r", ch );
			return;
		}

		if( victim == ch ) 
		{
			send_to_char(YEL "找個別的隊友比較實際點。"NOR"\n\r", ch);
			return;
		}

		if( victim->master != ch && ch != victim )
		{
			act( "[33m$N並沒有跟隨你。[m", ch, NULL, victim, TO_CHAR );
			return;
		}

		add_member(ch, victim);

		act( "[36m$N加入你的隊伍。[m", ch, NULL, victim, TO_CHAR    );
		act( "[36m你加入$n的隊伍。[m", ch, NULL, victim, TO_VICT    );
		act( "[36m$N加入$n的隊伍。[m", ch, NULL, victim, TO_NOTVICT );
		return;
	}
	if( !str_cmp(arg, "-"))
	{
		if( !( victim = get_char_room( ch, arg1 ) ) )
		{
			send_to_char( "[33m他不在這兒。[m\n\r", ch );
			return;
		}

		if( victim == ch ) 
		{
			send_to_char(YEL "請使用group leave。"NOR"\n\r", ch);
			return;
		}


		if( is_same_group( victim, ch ) )
		{
			CHAR_DATA *vch;

			if( ( vch = victim->protector ) && !IS_AFFECTED( vch, AFF_SUMMONED ) )
			{
				remove_protector(victim);//should remove before set victim->leader == null
			}
			// 停止被協助 2021/11/20
			if( ( vch = victim->cooperator ) && !IS_AFFECTED( vch, AFF_SUMMONED ) )
			{
				remove_cooperator(victim);
			}

			// victim->leader = NULL;
			remove_member( victim);
			act( "[36m你把$N移出你的隊伍。[m", ch, NULL, victim, TO_CHAR    );
			act( "[36m$n把你移出$s隊伍。[m",  ch, NULL, victim, TO_VICT  );
			act( "[36m$n把$N移出$s隊伍。[m",   ch, NULL, victim, TO_NOTVICT );

			// 停止協助目標 2021/11/20
			if( ( vch = victim->linking ) )
			{
				stop_linking(victim, vch);
			}

			// 停止協助目標 2021/11/20
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
			send_to_char("他不在你的隊伍裡。\n\r", ch);
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
  sprintf( buf, "\n\r%s"NOR"的隊伍：\n\r"//
 1234567890123456789012345678901234567890123456789012345678901234567890   
"[LV 職  業]        姓        名            生命力      魔法力       移動力   [m\n\r", PERS( leader, ch ) );
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
                            IS_NPC( gch ) ? "非玩者"
                            : (char *)class_table[gch->class].cwho_name,
                            can_see( ch, gch ) ? no_ansi : "不明人物",
                            gr_stat_bar(gch->hit, gch->max_hit),
                            NOR, gr_stat_bar(gch->mana, gch->max_mana),
                            NOR,gr_stat_bar(gch->move, gch->max_move)
                           );
                }
                else {
                    sprintf( buf,
                            NOR"[%-2d %s] %-27s %s%5d%s/%-5d  %s%5d%s/%-5d  %s%5d%s/%-5d [m\n\r",
                            gch->level,
                            IS_NPC( gch ) ? "非玩者"
                            : (char *)class_table[gch->class].cwho_name,
                            can_see( ch, gch ) ? no_ansi : "不明人物",
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
  send_to_char( "[33m可是你在跟隨別人耶。[m\n\r", ch );
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
          act( "[36m$N加入你的隊伍。[m", ch, NULL, victim, TO_CHAR    );
          act( "[36m你加入$n的隊伍。[m", ch, NULL, victim, TO_VICT    );
          act( "[36m$N加入$n的隊伍。[m", ch, NULL, victim, TO_NOTVICT );
        }
  return;
    }

    if ( !( victim = get_char_room( ch, arg ) ) )
    {
  send_to_char( "[33m他不在這兒。[m\n\r", ch );
  return;
    }

    if( victim == ch ) {
  send_to_char(YEL "找個別的隊友比較實際點。"NOR"\n\r", ch);
  return;
    }

    if ( victim->master != ch && ch != victim )
    {
  act( "[33m$N並沒有跟隨你。[m", ch, NULL, victim, TO_CHAR );
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
  act( "[36m你把$N移出你的隊伍。[m", ch, NULL, victim, TO_CHAR    );
  act( "[36m$n把你移出$s隊伍。[m",  ch, NULL, victim, TO_VICT  );
  act( "[36m$n把$N移出$s隊伍。[m",   ch, NULL, victim, TO_NOTVICT );

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
  act( "[33m$N不能加入你的隊伍。[m",  ch, NULL, victim, TO_CHAR   );
  act( "[33m你不能加入$n的隊伍。[m", ch, NULL, victim, TO_VICT   );
  act( "[33m$N不能加入$n的隊伍。[m",  ch, NULL, victim, TO_NOTVICT    );
  return;
    }

    victim->leader = ch;
    ch->leader = ch;
    act( "[36m$N加入你的隊伍。[m", ch, NULL, victim, TO_CHAR    );
    act( "[36m你加入$n的隊伍。[m", ch, NULL, victim, TO_VICT    );
    act( "[36m$N加入$n的隊伍。[m", ch, NULL, victim, TO_NOTVICT );
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
  send_to_char( "[33m分配多少錢？[m\n\r", ch );
  return;
    }
    
    amount = atoi( arg );

    if ( amount < 0 )
    {
  send_to_char( "[33m你的隊友不會想要這樣的。[m\n\r", ch );
  return;
    }

    if ( amount == 0 )
    {
  send_to_char( "[33m你拿出了零塊錢，但是沒有人注意到。[m\n\r", ch );
  return;
    }

    if ( ch->gold < amount )
    {
  send_to_char( "[33m你沒有那麼多金幣。[m\n\r", ch );
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
  //send_to_char( "[33m自己留著吧！[m\n\r", ch );
  return;
    }
      
    share = amount / members;
    extra = amount % members;

    if ( share == 0 )
    {
  send_to_char( "[33m別打擾人家，小氣鬼。[m\n\r", ch );
  return;
    }

    ch->gold -= amount;
    ch->gold += share + extra;

    sprintf( buf,
  "[36m你分出了%d枚金幣。你自己分到了%d枚金幣。[m\n\r",
  amount, share + extra );
    send_to_char( buf, ch );

    sprintf( buf, "[36m$n分出了%d枚金幣。你分到了%d枚金幣。[m\n\r",
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
		send_to_char( "[33m要告訴你的隊友什麼？[m\n\r", ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| ( IS_SET( race_table[ch->race].race_abilities, RACE_MUTE ) && !IS_SET( ch->act, PLR_HORSE ) )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "[33m你的嘴唇在動，但是沒有聲音跑出來。[m\n\r", ch );
		return;
	}

	if ( IS_SET( ch->act, PLR_NO_TELL ) )
	{
		send_to_char( "[33m你的聲音傳不過去！[m\n\r", ch );
		return;
	}

	argument = makedrunk( argument, ch );

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
	sprintf( buf, HIG"%s向隊友說「%s」。[m\n\r", ch->short_descr, argument );
    
	for ( gch = char_list; gch; gch = gch->next )
	{
		if ( is_same_group( gch, ch ) && !IS_SET( gch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
			// 移除目標 mute 的情況下不能收到訊息 2021/12/26
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
  send_to_char( "[33m要嗶誰？[m\n\r", ch );
  return;
    }

    if ( !( victim = get_char_world( ch, arg ) ) )
    {
  send_to_char( "[33m他不在這兒。[m\n\r", ch );
  return;
    }

    if ( IS_NPC( victim ) )
    {
  send_to_char( "[33m他不能被嗶。[m\n\r", ch );
  return;
    }

    sprintf( buf, "[36m你嗶了%s。[m\n\r", victim->short_descr );
    send_to_char( buf, ch );
    
    sprintf( buf, "[36m\a\a%s嗶了你。[m\n\r", ch->short_descr );
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
        send_to_char( "天神不淮你發出任何聲音!\n\r", ch );
        return;
    }
    if ( IS_AFFECTED( ch, AFF_MUTE )
  || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
  || IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
  send_to_char( "[33m你的嘴唇在動，但是沒有聲音跑出來。[m\n\r", ch );
  return;
    }

    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
  return;
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0'  )
    {
        send_to_char( "你要告訴什麼人有關什麼事呢?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( victim->in_room != ch->in_room ) )
    {
        send_to_char( "你談話的對象現在好像不在 MUD 世界中喔!\n\r", ch );
        return;
    }

    if( !IS_NPC(victim) )
    {
        send_to_char("用 tell 聊天吧...\n\r",ch);
        return;
    }

    if ( !IS_AWAKE(victim) )
    {
        act( "$E睡得很熟而聽不到你說什麼.", ch, 0, victim, TO_CHAR );
        return;
    }

    if( argument[0] != '\0' )
    {
        act( "你和 $N 交談 '$t'.", ch, argument, victim, TO_CHAR );
        position                = victim->position;
        victim->position        = POS_STANDING;
		/*   對 mob 顯示訊息沒有實質上的意義且會產生 comm.c Act: bad code 99.
        act( "$cfyw$n$cfwe 和你交談 '$t'.", ch, argument, victim, TO_VICT );
		*/
        victim->position        = position;
        victim->reply   = ch;
        mprog_talk_trigger(argument,ch,victim);
    }
    else
    {
        act( "你試著和 $N 交談....", ch, NULL, victim, TO_CHAR );
        position                = victim->position;
        victim->position        = POS_STANDING;
		/*    對 mob 顯示訊息沒有實質上的意義且會產生 comm.c Act: bad code 99.
        act( "$cfyw$n$cfwe 試著和你交談.", ch, NULL, victim, TO_VICT );
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
  send_to_char( "[33m要領導誰做什麼？[m\n\r", ch );
  return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
    send_to_char( "[33m你應該是要受命令，而不是發命令吧！[m\n\r", ch );
    return;
    }

  if ( ch->class != LORD || !ch->fighting || IS_NPC( ch ) )
  {
    send_to_char( "只有領主在戰鬥中才可以領導別人！\n\r", ch );
    return;
  }

  if ( //ch->pcdata->learned[gsn_lead] < number_percent() )
            get_skill_percent(ch, gsn_lead) < number_percent() )
  {
    send_to_char( "[33m你失敗了！[m\n\r", ch );
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
        send_to_char( "[33m他不在這兒。[m\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "[33m是是是，馬上辦！[m\n\r", ch );
        return;
    }
    
    if ( IS_AFFECTED( victim, AFF_SUMMONED ) )
    {
      send_to_char( "[33m你無法領導召喚生物！[m\n\r", ch );
      return;
    }

    if ( victim->wait > 11 + sklv *2  )
    {
        send_to_char( "[33m他已經手忙腳亂了！[m\n\r", ch );
        return;
    }
                if( IS_AFFECTED(victim, AFF_BATTLE)){
                    send_to_char("你正在發動戰陣，沒空領導別人", victim);
                    return;
                }
    }

    found = FALSE;
    one_argument(argument, cmd);
    
  if( !str_prefix(cmd, "battle") 
      ||  !str_prefix(cmd, "'battle array")) {
    send_to_char("不可以領導別人使用陣型！\n\r", ch);
    return;
  }

  if( !str_prefix(cmd, "order")
      ||  !str_prefix(cmd, "lead") ){
    send_to_char("不可以領導別人下命令！\n\r", ch);
    return;
  }

  if( !str_prefix(cmd, "quit") 
      || !str_prefix(cmd, "drop")
      || !str_prefix(cmd, "remove")
      || !str_prefix(cmd, "give"))
  {
    send_to_char("你想幹嘛？\n\r", ch);
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
        send_to_char( "[33m他不願服從你的領導。[m\n\r", ch );
        continue;
      }
      found = TRUE;
      sprintf( log_buf, "%s : lead %s", ch->name, och->name );
      strcat( log_buf, argument );
      log_string( log_buf );
      act( "[36m$n領導你 $t[m", ch, argument, och, TO_VICT );
      interpret( och, argument );
    }
  }

  if ( found ){
    send_to_char( "[36m沒問題。[m\n\r", ch );
    WAIT_STATE(ch, 6);
  }
  else
    send_to_char( "[33m這裡沒有人可以聽從領導。[m\n\r", ch );
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
        send_to_char( "只有領主才可以使用陣形！\n\r", ch );
        return;
    }

    if ( ch->level < 19 )
    {
        send_to_char( "你的等級不夠！\n\r", ch );
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
            send_to_char( "指令輸入錯誤。\n\r", ch );
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
         send_to_char("不可以battle battle！\n\r", ch);
         return;      
         }
         }
         */ 
        smash_tilde( argument );
        strcpy( buf, argument );
        one_argument(buf, cmd);
        if ( !str_infix( "battle",argument)){
//        if( !str_prefix(cmd, "battle") ) {
            send_to_char("不可以循環使用陣型！\n\r", ch);
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
            default : send_to_char( "第幾個陣形的第幾個命令？\n\r", ch );
        }
        send_to_char("好的。\n\r", ch);
        return;
    }
    else
    {
        if ( !is_number( arg ) )
        {
            send_to_char( "指令輸入錯誤。\n\r", ch );
            return;
        }

        if ( !ch->pcdata->ba )
        {
            send_to_char( "您還未設定陣形。\n\r", ch );
            return;
        }

        if( IS_AFFECTED(ch, AFF_BATTLE)){
            send_to_char("你正在發動戰陣，不能接受領導",ch);
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
            default : send_to_char( "你想使用第幾個陣形？\n\r", ch );
                      REMOVE_BIT(ch->affected_by, AFF_BATTLE);
                      return;
        }
        WAIT_STATE(ch, wait);
        WAIT_STATE( ch, gsn_battle_array );
        REMOVE_BIT(ch->affected_by, AFF_BATTLE);
    }
}
