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
//*  PROGRAM ID  : act_info.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940607 jye                                    *
//*  PURPOSE     :         ±Npractice¤§+¤W¦â                      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         ­×§ïlv44¥H¤W¬Ý¤£¨ìclan_channel¤§bug    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         ­×§ïlv45¥H¤W¤~¥i¨Ï¥Îtell¡iÅÑÅ¥¡j¥\¯à   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         ­×§ïwho imm®Étrust 43¤]·|¥X²{          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         ­×§ïtell¡iÅÑÅ¥¡j¼W¥[all ªº¥\¯à         *
//*              :             tell¡iÅÑÅ¥¡j¼W¥[close ªº¥\¯à       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 jye                                    *
//*  PURPOSE     :         ­×§ïwho ¤W¤U­­¤§bug¤Î¼W¥[who¥i¬d¸ßrace *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         ­×§ï§PÂ_souvenir­È¨Ï§ï¥ÎÅÜ¼Æ           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         ·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050214 jye                                    *
//*  PURPOSE     :         ­×§ïdo_exitsÅã¥Ü¦r«¬"¡V"§ï¥Î"¡Ð"       *
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
#include "olc.h"
extern bool IsSorGateMob(int nMobIndex);
extern bool IsUnderFistFight(CHAR_DATA *ch);
bool can_scout_check( CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room, int sklv );


extern const struct social_type social_table[];
char *        const    where_name      [] =
{
/*  "<used as light>     ",
    "<worn on finger>  ",
    "<worn on finger>  ",
    "<worn around neck>  ",
    "<worn around neck>  ",
    "<worn on body>  ",
    "<worn on head>  ",
    "<worn on legs>  ",
    "<worn on feet>  ",
    "<worn on hands>   ",
    "<worn on arms>  ",
    "<worn as shield>  ",
    "<worn about body>   ",
    "<worn about waist>  ",
    "<worn around wrist> ",
    "<worn around wrist> ",
    "<wielded, primary>  ",
    "<held>    ",
    "<wielded, second>   " */
    "< ¥ú  ·½ > ",
    "< ¤â  «ü > ",
    "< ¤â  «ü > ",
    "< ÀV  ³¡ > ",
    "< ÀV  ³¡ > ",
    "< ¨­  Åé > ",
    "< ÀY  ³¡ > ",
    "< »L  ³¡ > ",
    "< ¸}  ³¡ > ",
    "< ¤â  ³¡ > ",
    "< ¤â  Áu > ",
    "< ¬Þ  µP > ",
    "<³òÂ¶¨­Åé> ",
    "< ¸y  ³¡ > ",
    "< µÃ  ³¡ > ",
    "< µÃ  ³¡ > ",
    "<¥D­nªZ¾¹> ",
    "< ¤â  ´¤ > ",
    "<¦¸­nªZ¾¹> ",
    "<Âù¤âªZ¾¹> ",
    "< ­I  ±¾ > ",
    "",                    //  MAX_WEAR
    "< Ä²  ¤â > ",
    "< Ä²  ¤â > ",
    "< ÃM  ­¼ > ",
    "<¦¸ ¤â Áu> ",
    "<¦¸ ¤â ³¡> ",
    "<¦¸ »L ³¡> ",
    "<¦¸ ¸} ³¡> ",
    "< ¼u  ÃÄ > "         // wear loc ammo add at 2022/04/30
};

TOP10_DATA tmudage[TOP_COUNT + 1],
           texp[TOP_COUNT + 1],
           tgold[TOP_COUNT + 1],
           thp[TOP_COUNT + 1],
           t_mp[TOP_COUNT + 1],
           tmv[TOP_COUNT + 1],
           tpk[TOP_COUNT + 1],
           tresp[TOP_COUNT + 1],
           tmerit[TOP_COUNT + 1];
/*
 * Local functions.
 */
char *        format_obj_to_char  args( ( OBJ_DATA *obj, CHAR_DATA *ch,
               bool fShort ) );
void      show_list_to_char      args( ( OBJ_DATA *list, CHAR_DATA *ch,
               bool fShort, bool fShowNothing ) );
void      show_char_to_char_0        args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void      show_char_to_char_1        args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void      show_char_to_char      args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
int       is_skname     args( ( const char *str, char *namelist ) );
int       class_lookup    args( ( const char *name ) );
int       str_length    args( ( char *string) );

char *skill_exp_desc(int exp);

char *format_obj_to_char_b( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf [ BUF_STRING_LENGTH ];

    buf[0] = '\0';

    if( IS_IMMORTAL(ch) ) sprintf(buf, "["HIG"%5d"NOR"]", obj->pIndexData->vnum );
    if( IS_SET(obj->pIndexData->souvenir, Z1) ) strcat( buf, "(" HIW "¬ö" NOR  ")"   );
    if ( IS_OBJ_STAT( obj, ITEM_INVIS)     )   strcat( buf, "(" YEL "Áô" NOR  ")"   );
    if ( ( IS_AFFECTED( ch, AFF_DETECT_EVIL  )
    || ( IS_SET( race_table[ ch->race ].race_abilities,
          RACE_DETECT_ALIGN )
        && !IS_EVIL( ch ) ) )
  && IS_OBJ_STAT( obj, ITEM_EVIL )   )   strcat( buf, "("HIR"¨¸"NOR")"  );
    if ( ( IS_AFFECTED( ch, AFF_DETECT_MAGIC ) || IS_SET( race_table[ch->race].race_abilities,RACE_DETECT_MAGIC))
  && IS_OBJ_STAT( obj, ITEM_MAGIC )  )   strcat( buf, "("HIC"Å]"NOR")"   );
    if ( IS_OBJ_STAT( obj, ITEM_GLOW )     )   strcat( buf, "("HIY"¥ú"NOR")"   );
    if ( IS_OBJ_STAT( obj, ITEM_HUM )    )   strcat( buf, "("HIP"»ï"NOR")"   );
    if ( IS_OBJ_STAT( obj, ITEM_POISONED ) )   strcat( buf, "("HIG"¬r"NOR")"  );
    if(!indestructible(obj) && ( get_curr_dura(obj) * 3 < get_max_dura(obj))) strcat( buf, "("HIP"·l"NOR")" );
    if( is_broken(obj) )strcat( buf, "("HIB"Ãa"NOR")" );




    if ( fShort )
    {
  if( obj->short_descr)
    strcat( buf, obj->short_descr );
  else
  {
    if(obj->name){
      strcat(buf,"(");
      strcat(buf, obj->name );
      strcat(buf, ")");
    }
    else
    {
      strcat(buf, "("HIB"¤£©úª««~"NOR")");
    }
  }
/*
  if ( obj->name ) {
      strcat( buf, "(" );
      strcat( buf, obj->name );
      strcat( buf, ")" );
  }
*/
    }
    else
    {
  if ( obj->description )
      strcat( buf, obj->description );
  else
      strcat( buf, obj->short_descr );
    }

    return buf;
}

char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf [ BUF_STRING_LENGTH ];
	AFFECT_DATA *paf;  // 2021/09/28
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	int e = 0;
	int f = 0;
	int g = 0;
	int h = 0;
	int enc = 0;
	
	buf[0] = '\0';

	if( IS_IMMORTAL(ch) ) sprintf(buf, "["HIG"%5d"NOR"]", obj->pIndexData->vnum );
	if( IS_SET(obj->pIndexData->souvenir, Z1) ) strcat( buf, "(" HIW "¬ö©À" NOR ")" );
	if ( IS_OBJ_STAT( obj, ITEM_INVIS) )   strcat( buf, "(" YEL "Áô§Î" NOR  ")" );
	if ( IS_OBJ_STAT( obj, ITEM_SPELL) )
	{
		for ( paf = obj->affected; paf; paf = paf->next )
		{
			if ( paf->location == APPLY_WEAPON_SPELL )
				a = a + paf->modifier;
			if ( paf->location == APPLY_WEAPON_WIND )
				b = b + paf->modifier;
			if ( paf->location == APPLY_WEAPON_FIRE )
				c = c + paf->modifier;
			if ( paf->location == APPLY_WEAPON_COLD )
				d = d + paf->modifier;
			if ( paf->location == APPLY_WEAPON_EARTH )
				e = e + paf->modifier;
			if ( paf->location == APPLY_WEAPON_LIGHTNING )
				f = f + paf->modifier;
			if ( paf->location == APPLY_WEAPON_SAINT )
				g = g + paf->modifier;
			if ( paf->location == APPLY_WEAPON_DARK )
				h = h + paf->modifier;
		}
		for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
		{
			if ( paf->location == APPLY_WEAPON_SPELL )
				a = a + paf->modifier;
			if ( paf->location == APPLY_WEAPON_WIND )
				b = b + paf->modifier;
			if ( paf->location == APPLY_WEAPON_FIRE )
				c = c + paf->modifier;
			if ( paf->location == APPLY_WEAPON_COLD )
				d = d + paf->modifier;
			if ( paf->location == APPLY_WEAPON_EARTH )
				e = e + paf->modifier;
			if ( paf->location == APPLY_WEAPON_LIGHTNING )
				f = f + paf->modifier;
			if ( paf->location == APPLY_WEAPON_SAINT )
				g = g + paf->modifier;
			if ( paf->location == APPLY_WEAPON_DARK )
				h = h + paf->modifier;
		}
		for ( paf = obj->material->affected; paf; paf = paf->next )
		{
			if ( paf->location == APPLY_WEAPON_SPELL )
				a = a + paf->modifier;
			if ( paf->location == APPLY_WEAPON_WIND )
				b = b + paf->modifier;
			if ( paf->location == APPLY_WEAPON_FIRE )
				c = c + paf->modifier;
			if ( paf->location == APPLY_WEAPON_COLD )
				d = d + paf->modifier;
			if ( paf->location == APPLY_WEAPON_EARTH )
				e = e + paf->modifier;
			if ( paf->location == APPLY_WEAPON_LIGHTNING )
				f = f + paf->modifier;
			if ( paf->location == APPLY_WEAPON_SAINT )
				g = g + paf->modifier;
			if ( paf->location == APPLY_WEAPON_DARK )
				h = h + paf->modifier;
		}
		/*if( a > 0 || b > 0 || c > 0 || d > 0 || e > 0 || f > 0 || g > 0 || h > 0 ){
			enc = UMAX( h , UMAX( g , UMAX( f , UMAX( e , UMAX( d , UMAX( c , UMAX( b , a ) ))))));
			if( enc == a )
				strcat( buf, "("HIU"ªþÅ]"NOR")" );
			else if( enc == b )
				strcat( buf, "("HIG"Äñ­·"NOR")" );
			else if( enc == c )
				strcat( buf, "("HIR"¨`¼ö"NOR")" );
			else if( enc == d )
				strcat( buf, "("HIC"´HÁ÷"NOR")" );
			else if( enc == e )
				strcat( buf, "("YEL"ÂÐ©¥"NOR")" );
			else if( enc == f )
				strcat( buf, "("HIY"¹q®ð"NOR")" );
			else if( enc == g )
				strcat( buf, "("HIW"°{Ä£"NOR")" );
			else if( enc == h )
				strcat( buf, "("HIB"±â·t"NOR")" );
		} */
		if( a > 0 )
			enc += 1;
		if( b > 0 )
			enc += 1;
		if( c > 0 )
			enc += 1;
		if( d > 0 )
			enc += 1;
		if( e > 0 )
			enc += 1;
		if( f > 0 )
			enc += 1;
		if( g > 0 )
			enc += 1;
		if( h > 0 )
			enc += 1;
		// ­×§ïªþÅ]Åã¥Ü 2022/12/10
		if( enc > 0 ){
			strcat(buf,"(");
			if( enc > 1 ){ // ¸Ë³Æ¦³¨â­Ó¥H¤W¥¿­ÈªþÅ], Åã¥Ü§Ü©Ê³æ¤@¦r
				if( a > 0 )
					strcat( buf, HIU"Å]"NOR );
				if( b > 0 )
					strcat( buf, HIG"­·"NOR );
				if( c > 0 )
					strcat( buf, HIR"¤õ"NOR );
				if( d > 0 )
					strcat( buf, HIC"¦B"NOR );
				if( e > 0 )
					strcat( buf, YEL"¦a"NOR );
				if( f > 0 )
					strcat( buf, HIY"¹p"NOR );
				if( g > 0 )
					strcat( buf, HIW"¥ú"NOR );
				if( h > 0 )
					strcat( buf, HIB"îî"NOR );
			}else{         // ¸Ë³Æ¥u¦³¤@ºØ¥¿­ÈªþÅ]
				if( a > 0 )
					strcat( buf, HIU"ªþÅ]"NOR );
				if( b > 0 )
					strcat( buf, HIG"Äñ­·"NOR );
				if( c > 0 )
					strcat( buf, HIR"¨`¼ö"NOR );
				if( d > 0 )
					strcat( buf, HIC"´HÁ÷"NOR );
				if( e > 0 )
					strcat( buf, YEL"ÂÐ©¥"NOR );
				if( f > 0 )
					strcat( buf, HIY"¹q®ð"NOR );
				if( g > 0 )
					strcat( buf, HIW"°{Ä£"NOR );
				if( h > 0 )
					strcat( buf, HIB"±â·t"NOR );
			}
			strcat(buf, ")");
		}
	}
	if (( IS_AFFECTED( ch, AFF_DETECT_EVIL )
		|| ( IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN )
		&& !IS_EVIL( ch ) ) )
		&& IS_OBJ_STAT( obj, ITEM_EVIL ) )
			strcat( buf, "("HIR"¨¸®ð"NOR")" );
	if (( IS_AFFECTED( ch, AFF_DETECT_MAGIC )
		|| IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_MAGIC))
		&& IS_OBJ_STAT( obj, ITEM_MAGIC )  )
			strcat( buf, "("HIC"Å]¤O"NOR")" );
	if ( IS_OBJ_STAT( obj, ITEM_GLOW ) )
		strcat( buf, "("HIY"µo¥ú"NOR")" );
	if ( IS_OBJ_STAT( obj, ITEM_HUM ) )
		strcat( buf, "("HIP"§C»ï"NOR")" );
	if ( IS_OBJ_STAT( obj, ITEM_POISONED ) )
		strcat( buf, "("HIG"¼@¬r"NOR")" );
	if(!indestructible(obj) && !is_broken(obj)&& ( get_curr_dura(obj) * 3 < get_max_dura(obj))) strcat( buf, "("HIP"¿i·l"NOR")" );
	if( is_broken(obj) || IS_OBJ_STAT( obj, ITEM_BROKEN ) )strcat( buf, "("HIB"·lÃa"NOR")" );




	if ( fShort )
	{
		if( obj->short_descr)
			strcat( buf, obj->short_descr );
		else
		{
			if(obj->name){
				strcat(buf,"(");
				strcat(buf, obj->name );
				strcat(buf, ")");
			}
			else
			{
				strcat(buf, "("HIB"¤£©úª««~"NOR")");
			}
		}
		/*
			if ( obj->name ) {
				strcat( buf, "(" );
				strcat( buf, obj->name );
				strcat( buf, ")" );
		  }
		*/
	}
	else
	{
		if ( obj->description )
			strcat( buf, obj->description );
		else
			strcat( buf, obj->short_descr );
	}

	return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    OBJ_DATA  *obj;
    char       buf [ BUF_STRING_LENGTH ];
    char       buf1[ MAX_STRING_LENGTH * 7 ];
    char     **prgpstrShow;
    char      *pstrShow;
    int       *prgnShow;
    int        nShow;
    int        iShow;
    int        count;
    int iLine = 0;
    bool       fCombine;

    if ( !ch->desc || !list )
  return;

    buf1[0] = '\0';
    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj; obj = obj->next_content )
    {
  if ( obj->deleted )
      continue;
  count++;
    }

    prgpstrShow        = alloc_mem( count * sizeof( char * ) );
    prgnShow  = alloc_mem( count * sizeof( int )    );
    nShow  = 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj; obj = obj->next_content )
    { 
  if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
  {
      pstrShow = format_obj_to_char( obj, ch, fShort );
      fCombine = FALSE;

      if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
      {
    /*
     * Look for duplicates, case sensitive.
     * Matches tend to be near end so run loop backwords.
     */
    for ( iShow = nShow - 1; iShow >= 0; iShow-- )
    {
        if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
        {
      prgnShow[iShow]++;
      fCombine = TRUE;
      break;
        }
    }
      }

      /*
       * Couldn't combine, or didn't want to.
       */
      if ( !fCombine )
      {
    prgpstrShow [nShow] = str_dup( pstrShow );
    prgnShow    [nShow] = 1;
    nShow++;
      }
  }
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
  if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
  {
      if ( prgnShow[iShow] != 1 )
      {
    sprintf( buf, "(%2d)", prgnShow[iShow] );
    strcat( buf1, buf );
      }
      else
      {
    strcat( buf1, "    " );
      }
  }
  strcat( buf1, prgpstrShow[iShow] );
  strcat( buf1, "\n\r" );
  free_string( prgpstrShow[iShow] );
        ++iLine;
		// ­×§ï³Ì¤j pagelength , ¬G¥iÅã¥Ü¤W­­´£°ª 2023/01/12
		if( iLine > 80 ) {
        //if( iLine > 50 ) {
            send_to_char(buf1, ch);
            buf1[0] = '\0';
            iLine = 0;
        }
    }

    if ( fShowNothing && nShow == 0 )
    {
  if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
      strcat( buf1, "    " );
  strcat( buf1, "¤°»ò³£¨S¦³¡C\n\r" );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow/*, count * sizeof( char * ) */);
    free_mem( prgnShow/*,    count * sizeof( int )   */ );

    send_to_char( buf1, ch );
    return;
}

void show_list_to_char1( char *out, OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    OBJ_DATA  *obj;
    char       buf [ BUF_STRING_LENGTH * 2];
    char       buf1[ MAX_STRING_LENGTH * 9 ];
    char     **prgpstrShow;
    char      *pstrShow;
    int       *prgnShow;
    int        nShow;
    int        iShow;
    int        count;
    bool       fCombine;

    if ( !ch->desc || !list )
  return;

    send_to_char("¨­¤Wªºª««~¦³¡G\n\r\n\r", ch);
    buf1[0] = '\0';
    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj; obj = obj->next_content )
    {
  if ( obj->deleted )
      continue;
  count++;
    }

    prgpstrShow        = alloc_mem( count * sizeof( char * ) );
    prgnShow  = alloc_mem( count * sizeof( int )    );
    nShow  = 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj; obj = obj->next_content )
    { 
  if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
  {
      pstrShow = format_obj_to_char( obj, ch, fShort );
      fCombine = FALSE;

      if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
      {
    /*
     * Look for duplicates, case sensitive.
     * Matches tend to be near end so run loop backwords.
     */
    for ( iShow = nShow - 1; iShow >= 0; iShow-- )
    {
        if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
        {
      prgnShow[iShow]++;
      fCombine = TRUE;
      break;
        }
    }
      }

      /*
       * Couldn't combine, or didn't want to.
       */
      if ( !fCombine )
      {
    prgpstrShow [nShow] = str_dup( pstrShow );
    prgnShow    [nShow] = 1;
    nShow++;
      }
  }
    }

    /*
     * Output the formatted list.
     */
    count = 0;
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
  if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
  {
      if ( prgnShow[iShow] != 1 )
      {
    sprintf( buf, "(%2d)", prgnShow[iShow] );
    strcat( buf1, buf );
      }
      else
      {
    strcat( buf1, "    " );
      }
  }
  strcat( buf1, prgpstrShow[iShow] );
  strcat( buf1, "\n\r" );
  free_string( prgpstrShow[iShow] );
        ++count;
		// ­×§ï³Ì¤j pagelength , ¬G¥iÅã¥Ü¤W­­´£°ª 2023/01/12
		//if( count >= 50 ) {
		if( count >= 80 ) {
            buf1[0] = '\0';
            send_to_char(buf1, ch);
            count = 0;
        }
    }

    if ( fShowNothing && nShow == 0 )
    {
  if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
      strcat( buf1, " " );
  strcat( buf1, "¤°»ò³£¨S¦³¡C\n\r" );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow/*, count * sizeof( char * )*/ );
    free_mem( prgnShow/*,    count * sizeof( int )    */);

    send_to_char(buf1, ch);
//    sprintf( out, buf1 );
//    ª½±µ°e¥Xµ¹player¡A³o¼Ë¥i¥H´î¤Öcrashªº°ÝÃD
    return;
}

void show_char_to_char_0b( CHAR_DATA *victim, CHAR_DATA *ch )
{
	char buf [ BUF_STRING_LENGTH * 4];
	OBJ_DATA* pObj;

	buf[0] = '\0';

	if( IS_NPC(victim) && IS_IMMORTAL(ch)){
		sprintf(buf, "["HIG"%5d"NOR"]", victim->pIndexData->vnum);
	}

	// ¨¤¦â¦b¦P­Ó©Ð¶¡¤¤ªø®É¶¡¥¼²¾°Ê 2023/01/17
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_IDLE ) && IS_IMMORTAL(ch) )
		strcat( buf, "("HIB"¶¢¸m"NOR")" );

	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && can_see(ch, victim) )
		strcat( buf, "("HIY"Áô"NOR")" );
	if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
		strcat( buf, "("YEL "Áô"NOR")" );
	if ( IS_AFFECTED( victim, AFF_HIDE ) )
		strcat( buf, "("HIB"ÂÃ"NOR")" );
	if ( IS_AFFECTED( victim, AFF_NIGHTSTALK))
		strcat( buf, "("HIU"·t"NOR")" );
	if ( IS_AFFECTED( victim, AFF_CHARM ) )
		strcat( buf, "("HIP"´b"NOR")" );
	if ( IS_AFFECTED( victim, AFF_CRAZY ) )
		strcat( buf, "("CYN"ºÆ"NOR")" );
	if ( IS_AFFECTED( victim, AFF_PASS_DOOR )
		|| ( IS_SET( race_table[ victim->race ].race_abilities, RACE_PASSDOOR )
		&& ( !str_cmp( race_table[ victim->race ].name, "Undead" )
		|| !str_cmp( race_table[ victim->race ].name, "Vampire" ) ) ) )
		strcat( buf, "("CYN"³z"NOR")");
	if ( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
		strcat( buf, "("HIP"Ãú"NOR")"  );
	if ( IS_AFFECTED( victim, AFF_TARDY ) )
		strcat( buf, "("HIY"Ãú"NOR")"  );
	if ( IS_EVIL( victim )
		&& ( IS_AFFECTED( ch, AFF_DETECT_EVIL )
		|| ( IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN )
		&& !IS_EVIL( ch ) ) ) )
		strcat( buf, "("HIR"®ð"NOR")" );
	if ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities, RACE_SANCT ) )
		strcat( buf, "("HIW"¥ú"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FLAMING ) ) strcat( buf, "("HIP"¥Ò"NOR")" );
	if ( is_affected( victim, gsn_invoke ) )  strcat( buf, "("HIB"®ð"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_ARMOR) )strcat( buf, "("HIB"¾À"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_MIRROR) )strcat( buf, "("HIW"Ãè"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MANA_SHIELD ) )strcat( buf, "("HIU"¬Þ"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FEAR ) )strcat( buf, "("CYN"®£"NOR")" );
	if( is_affected(victim, gsn_meditate)) strcat( buf, "("CYN"­ß"NOR")");
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER ) )
		strcat( buf, "("RED"¥û"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF ) )
		strcat( buf, "("RED"°½"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_REGISTER ) )
		strcat( buf, "("HIG"PK"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AFK ) )
		strcat( buf, "("HIP"§b"NOR")" );

	if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' )
	{
		strcat( buf, victim->long_descr );
		send_to_char( buf, ch );
		return;
	}

	strcat( buf, PERS( victim, ch ) );

	if ( !IS_NPC( victim ) && !IS_SET( ch->act, PLR_BRIEF ) )
	{
		strcat(buf, " ");
		strcat( buf, victim->pcdata->title );
		strcat(buf, " ");
	}

	//add by coconet for horse 0517
	for( pObj = victim->carrying ; pObj ; pObj = pObj->next_content )
	{
		if( pObj->wear_loc == WEAR_RIDE )
		{
			/* ¥[¤J switch §P©w ride ·í«e position ª¬ºA 2022/06/16
				strcat( buf, "ÃM¦b");
				strcat( buf, pObj->description);
				strcat( buf, "¤W¡C ");
				strcat( buf, "\n\r" );
				buf[0] = UPPER( buf[0] );
				send_to_char( buf, ch );
				return;
			*/
			strcat( buf, "ÃM¦b");
			strcat( buf, pObj->description);
			strcat( buf, "¤W");
			switch ( victim->position )
			{
				case POS_DEAD:
					strcat( buf, "¦º¤F¡I¡I" ); break;
				case POS_MORTAL:
					strcat( buf, "¨ü¤F­P©R¶Ë¡C" ); break;
				case POS_INCAP:
					strcat( buf, "¥¢¥h¦æ°Ê¯à¤O¤F¡C" ); break;
				case POS_STUNNED:
					strcat( buf, "©ü­Ë¡C" ); break;
				case POS_SLEEPING: 
					strcat( buf, "ºÎÄ±¡C" ); 
					break;
				case POS_RESTING:
					strcat( buf, "¥ð®§¡C" ); break;
				case POS_STANDING: 
					if( is_flying( victim ) )
						strcat(buf, "¯B¦b¥bªÅ¤¤¡C");
					else
						strcat( buf, "¡C" ); 
					break;
				case POS_FIGHTING:
					strcat( buf, "©M" );
					if ( !victim->fighting )
						strcat( buf, "ªÅ®ð(¡H)¥´¬[¡C" );
					else if ( victim->fighting == ch )
						strcat( buf, "§A(¡I)¥´¬[¡C" );
					else if ( victim->in_room == victim->fighting->in_room )
					{
						strcat( buf, PERS( victim->fighting, ch ) );
						strcat( buf, "¥´¬[¡C" );
					}
					else
					{
						strcat( buf, "¤£¦b³o¸Ìªº½Ö¥´¬[¡H" );
						if( victim->fighting ) {
							stop_fighting( victim, TRUE);
						}
						else
							stop_fighting( victim, FALSE);
					}
					break;
			}
			strcat( buf, "\n\r" );
			buf[0] = UPPER( buf[0] );
			send_to_char( buf, ch );
			return;
		}
	}

	switch ( victim->position )
	{
		case POS_DEAD:     strcat( buf, "¦º¤F¡I¡I" ); break;
		case POS_MORTAL:   strcat( buf, "¨ü¤F­P©R¶Ë¡C" ); break;
		case POS_INCAP:    strcat( buf, "¥¢¥h¦æ°Ê¯à¤O¤F¡C" ); break;
		case POS_STUNNED:  strcat( buf, "©ü­Ë¦b¦a¡C" ); break;
		case POS_SLEEPING: 
			//if( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_MEDITATING))
			//  strcat( buf, "¦b³o¸Ì­ß·Q¡C"        ); 
			//else
			strcat( buf, "¦b³o¸ÌºÎÄ±¡C" ); 
			break;
		case POS_RESTING:
			strcat( buf, "¦b³o¸Ì¥ð®§¡C" ); break;
		case POS_STANDING: 
			if( is_flying( victim ) )
				strcat(buf, "¯B¦b¥bªÅ¤¤¡C");
			else
				strcat( buf, "¯¸¦b³o¨à¡C"         ); 
			break;
		case POS_FIGHTING:
			strcat( buf, "¥¿¦b³o¸Ì©M" );
		if ( !victim->fighting )
			strcat( buf, "ªÅ®ð(¡H)¥´¬[¡C" );
		else if ( victim->fighting == ch )
			strcat( buf, "§A(¡I)¥´¬[¡C" );
		else if ( victim->in_room == victim->fighting->in_room )
		{
			strcat( buf, PERS( victim->fighting, ch ) );
			strcat( buf, "¥´¬[¡C" );
		}
		else
		{
			strcat( buf, "¤£¦b³o¸Ìªº½Ö¥´¬[¡H" );
			if( victim->fighting ) {
				stop_fighting( victim, TRUE);
			}
			else
				stop_fighting( victim, FALSE);
		}
		break;
	}

	strcat( buf, "\n\r" );
	buf[0] = UPPER( buf[0] );
	send_to_char( buf, ch );
	return;
}
/* called by show_char_to_char */
void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
	char buf [ BUF_STRING_LENGTH * 4];
	OBJ_DATA *pObj;

	buf[0] = '\0';
	if( !IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF)){
		show_char_to_char_0b(victim, ch);
		return;
	}

	if( IS_NPC(victim) && IS_IMMORTAL(ch) ){
		sprintf(buf, "["HIG"%5d"NOR"]", victim->pIndexData->vnum);
	}

	// ¨¤¦â¦b¦P­Ó©Ð¶¡¤¤ªø®É¶¡¥¼²¾°Ê 2023/01/17
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_IDLE ) && IS_IMMORTAL(ch) )
		strcat( buf, "("HIB"¶¢¸m"NOR")" );

	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && can_see(ch, victim) )
		strcat( buf, "("HIY"¯«Áô"NOR")" );
	if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
		strcat( buf, "("YEL "Áô§Î"NOR")" );
	if ( IS_AFFECTED( victim, AFF_HIDE )  )
		strcat( buf, "("HIB"ÂÃ°Î"NOR")" );
	if ( IS_AFFECTED( victim, AFF_NIGHTSTALK))
		strcat( buf, "("HIU"·t°Î"NOR")" );
	if ( IS_SET( victim->act, ACT_PET ) && IS_AFFECTED( victim, AFF_CHARM ) )
		strcat( buf, "("HIP"Ãdª«"NOR")" );
	else if ( IS_AFFECTED( victim, AFF_CHARM ) )
		strcat( buf, "("HIP"°g¥¢"NOR")" );

	if ( IS_AFFECTED( victim, AFF_CRAZY ) )
		strcat( buf, "("CYN"ºÆ¨g"NOR")" );
	if ( IS_AFFECTED( victim, AFF_PASS_DOOR ) || ( IS_SET( race_table[ victim->race ].race_abilities, RACE_PASSDOOR )
		&& ( !str_cmp( race_table[ victim->race ].name, "Undead" ) || !str_cmp( race_table[ victim->race ].name, "Vampire" ) ) ) ){
		strcat( buf, "("CYN"¥b³z©ú"NOR")");
	}
	if ( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
		strcat( buf, "("HIP"µµÃú"NOR")" );
	if ( IS_AFFECTED( victim, AFF_TARDY ) )
		strcat( buf, "("HIY"¶ÀÃú"NOR")" );
	if ( IS_EVIL( victim ) && ( IS_AFFECTED( ch, AFF_DETECT_EVIL )
		|| ( IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ) && !IS_EVIL( ch ) ) ) ){
		strcat( buf, "("HIR"¨¸®ð"NOR")" );
	}
	if ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities, RACE_SANCT ) )
		strcat( buf, "("HIW"¸t¥ú"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FLAMING ) )
		strcat( buf, "("HIP"µK¥Ò"NOR")" );
	if ( is_affected( victim, gsn_invoke ) )
		strcat( buf, "("HIB"Ó\\®ð"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_ARMOR) )
		strcat( buf, "("HIB"Å]¾À"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_MIRROR) )
		strcat( buf, "("HIW"Å]Ãè"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MANA_SHIELD ) )
		strcat( buf, "("HIU"Å]¬Þ"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FEAR ) )
		strcat( buf, "("CYN"®£Äß"NOR")" );
	if( is_affected(victim, gsn_meditate))
		strcat( buf, "("CYN"­ß·Q"NOR")");
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER )  )
		strcat( buf, "("RED"¥û¤â"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF  )  )
		strcat( buf, "("RED"¤p°½"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_REGISTER  )  )
		strcat( buf, "("HIG"PK"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AFK )  )
		strcat( buf, "("HIP"µo§b"NOR")" );

	if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' )
	{
		strcat( buf, victim->long_descr );
		send_to_char( buf, ch );
		return;
	}
	/*
	bug( "show_char_to_char_0 log: 1.ch->name, 2.vic->name", 0 );
	bug( ch->name, 0 );
	bug( victim->name, 0 );
	*/
	strcat( buf, PERS( victim, ch ) );

	if ( !IS_NPC( victim ) && !IS_SET( ch->act, PLR_BRIEF ) )
	{
		strcat(buf, " ");
		strcat( buf, victim->pcdata->title );
		strcat(buf, " ");
	}

	//add by coconet for horse 0517
	for( pObj = victim->carrying ; pObj ; pObj = pObj->next_content )
	{
		if( pObj->wear_loc == WEAR_RIDE )
		{
			/* ¥[¤J switch §P©w ride ·í«e position ª¬ºA 2022/06/16
				strcat( buf, "ÃM¦b");
				strcat( buf, pObj->description);
				strcat( buf, "¤W¡C ");
				strcat( buf, "\n\r" );
				buf[0] = UPPER( buf[0] );
				send_to_char( buf, ch );
				return;
			*/
			strcat( buf, "ÃM¦b");
			strcat( buf, pObj->description);
			strcat( buf, "¤W");
			switch ( victim->position )
			{
				case POS_DEAD:
					strcat( buf, "¦º¤F¡I¡I" ); break;
				case POS_MORTAL:
					strcat( buf, "¨ü¤F­P©R¶Ë¡C" ); break;
				case POS_INCAP:
					strcat( buf, "¥¢¥h¦æ°Ê¯à¤O¤F¡C" ); break;
				case POS_STUNNED:
					strcat( buf, "©ü­Ë¡C" ); break;
				case POS_SLEEPING: 
					strcat( buf, "ºÎÄ±¡C" ); 
					break;
				case POS_RESTING:
					strcat( buf, "¥ð®§¡C" ); break;
				case POS_STANDING: 
					if( is_flying( victim ) )
						strcat(buf, "¯B¦b¥bªÅ¤¤¡C");
					else
						strcat( buf, "¡C" ); 
					break;
				case POS_FIGHTING:
					strcat( buf, "©M" );
					if ( !victim->fighting )
						strcat( buf, "ªÅ®ð(¡H)¥´¬[¡C" );
					else if ( victim->fighting == ch )
						strcat( buf, "§A(¡I)¥´¬[¡C" );
					else if ( victim->in_room == victim->fighting->in_room )
					{
						strcat( buf, PERS( victim->fighting, ch ) );
						strcat( buf, "¥´¬[¡C" );
					}
					else
					{
						strcat( buf, "¤£¦b³o¸Ìªº½Ö¥´¬[¡H" );
						if( victim->fighting ) {
							stop_fighting( victim, TRUE);
						}
						else
							stop_fighting( victim, FALSE);
					}
					break;
			}
			strcat( buf, "\n\r" );
			buf[0] = UPPER( buf[0] );
			send_to_char( buf, ch );
			return;
		}
	}


	switch ( victim->position )
	{
		case POS_DEAD: strcat( buf, "¦º¤F¡I¡I" ); break;
		case POS_MORTAL: strcat( buf, "¨ü¤F­P©R¶Ë¡C" ); break;
		case POS_INCAP: strcat( buf, "¥¢¥h¦æ°Ê¯à¤O¤F¡C" ); break;
		case POS_STUNNED: strcat( buf, "©ü­Ë¦b¦a¡C" ); break;
		case POS_SLEEPING: strcat( buf, "¦b³o¸ÌºÎÄ±¡C" ); break;
		case POS_RESTING: strcat( buf, "¦b³o¸Ì¥ð®§¡C" ); break;
		case POS_STANDING:
			if( is_flying(victim) )
				strcat(buf, "¯B¦b¥bªÅ¤¤¡C");
			else
				strcat( buf, "¯¸¦b³o¨à¡C" ); 
			break;

		case POS_FIGHTING:
			strcat( buf, "¥¿¦b³o¸Ì©M" );
			if ( !victim->fighting )
				strcat( buf, "ªÅ®ð(¡H)¥´¬[¡C" );
			else if ( victim->fighting == ch )
				strcat( buf, "§A(¡I)¥´¬[¡C" );
			else if ( victim->in_room == victim->fighting->in_room )
			{
				strcat( buf, PERS( victim->fighting, ch ) );
				strcat( buf, "¥´¬[¡C" );
			}
			else
			{
				strcat( buf, "¤£¦b³o¸Ìªº½Ö¥´¬[¡H" );
				if( victim->fighting ) {
					stop_fighting( victim, TRUE);
				}
				else
					stop_fighting( victim, FALSE);
			}
			break;
	}

	strcat( buf, "\n\r" );
	buf[0] = UPPER( buf[0] );
	send_to_char( buf, ch );
	return;
}


/* called when look someone */
void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    char      buf [ MAX_STRING_LENGTH*15 ];
    char      buf1 [ MAX_STRING_LENGTH*16 ];
    int       iWear;
    int       percent;
    bool      found;
    int     count = 0;
    int   race;

    if ( can_see( victim, ch ) ){
        act( "[36m$n¥J²Ó¥´¶qµÛ§A¡C[m", ch, NULL, victim, TO_VICT    );
        act( "[36m$n¥J²Ó¥´¶qµÛ$N¡C[m",  ch, NULL, victim, TO_NOTVICT );
    }

    race = ( ( victim->temp_race ) ? victim->temp_race - 1 : victim->race );

	// ·s¼W aff_ghoul ¤Î stone form ªº±¡ªp¤U¦bºØ±Ú¦W«eÅã¥Ü¤£¦º¨t 2022/01/16
	//sprintf( buf1, "$N - %s(%s):\n\r", race_table[ race ].cname, race_table[ race ].name );
	if( is_affected(victim, gsn_stone_form)
		|| (  !IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD )
			&& IS_AFFECTED( victim, AFF_GHOUL ) ) )
		sprintf( buf1, "$N - ¤£¦º¨t%s(%s):\n\r", race_table[ race ].cname, race_table[ race ].name );
	else
		sprintf( buf1, "$N - %s(%s):\n\r", race_table[ race ].cname, race_table[ race ].name );


    if ( victim->description[0] != '\0' ){
        send_to_char(victim->description, ch);
        send_to_char("\n\r", ch);
    }else{
        strcat( buf1, "$E¬Ý°_¨Ó¨S¦³¤°»ò¯S¦â¡C" );
    }

    if ( victim->max_hit > 0 )
        percent = ( 100 * victim->hit ) / victim->max_hit;
    else
        percent = -1;

    sprintf( buf, PERS( victim, ch ) );

    if ( percent >= 100 ) strcat( buf, HIG"°·±dª¬ªp¨}¦n¡C"NOR"\n\r"  );
    else if ( percent >=  90 ) strcat( buf, HIG"¨ü¤F»´·LªºÀ¿¶Ë¡C"NOR"\n\r" );
    else if ( percent >=  80 ) strcat( buf, HIG"¨­¤W¦³¤@ÂI¤p·ï¦å¡C"NOR"\n\r"   );
    else if ( percent >=  70 ) strcat( buf, HIY"¨­¤W¦³¤@¨Ç¶Ë²ª¡C"NOR"\n\r"     );
    else if ( percent >=  60 ) strcat( buf, HIY"¨ü¤F¤p¶Ë¡C"NOR"\n\r"    );
    else if ( percent >=  50 ) strcat( buf, HIY"¬Ý¨Óª¬ªp¨Ã¤£¤Ó¦n¡C"NOR"\n\r" );
    else if ( percent >=  40 ) strcat( buf, HIR"¶Ë²ª²Ö²Ö¡C"NOR"\n\r"    );
    else if ( percent >=  30 ) strcat( buf, HIR"´ý¨­¬O¦å¡C"NOR"\n\r"   );
    else if ( percent >=  20 ) strcat( buf, HIR"¨ü¤F¤Q¤ÀÄY­«ªº¶Ë¡AÀH®É³£¥i¯à·w­Ë¡C"NOR"\n\r"   );
    else if ( percent >=  10 ) strcat( buf, RED"¤w¸g¥i¥H¬Ý¨ì¦º¯«¹ï¥L·L¯º¡C"NOR"\n\r"  );
    else           strcat( buf, RED"¤w¬O¶i®ð¤Ö¡A¥X®ð¦h¡A²´¬Ý´N­n¦º¥h¡C"NOR"\n\r"      );

    buf[0] = UPPER( buf[0] );
    strcat( buf1, buf );
    buf[0] = '\0';

    found = FALSE;
    if ( !IS_SET( race_table[ victim->race ].race_abilities, RACE_BEHOLDER ) ){
        for ( iWear = 0; iWear < MAX_WEAR; iWear++ ){
            if ( ( obj = get_eq_char( victim, iWear ) )
                && can_see_obj( ch, obj ) ){
                if ( !found ){
                    strcat( buf1, "\n\r" );
                    strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                    found = TRUE;
                }
                strcat( buf1, where_name[iWear] );
                strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                strcat( buf1, "\n\r" );
                ++count;
            }
            if (iWear == WEAR_LEGS) {
              if ( ( obj = get_eq_char( victim, WEAR_LEGS_2 ) )
                  && can_see_obj( ch, obj ) ){
                  if ( !found ){
                      strcat( buf1, "\n\r" );
                      strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                      found = TRUE;
                  }
                  strcat( buf1, where_name[WEAR_LEGS_2] );
                  strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                  strcat( buf1, "\n\r" );
                  ++count;
              }
            }
            if (iWear == WEAR_FEET) {
              if ( ( obj = get_eq_char( victim, WEAR_FEET_2 ) )
                  && can_see_obj( ch, obj ) ){
                  if ( !found ){
                      strcat( buf1, "\n\r" );
                      strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                      found = TRUE;
                  }
                  strcat( buf1, where_name[WEAR_FEET_2] );
                  strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                  strcat( buf1, "\n\r" );
                  ++count;
              }
            }
		}
		// ·s¼W¼uÃÄ¸Ë³Æ 2022/04/30
		if ( ( obj = get_eq_char( victim, WEAR_AMMO ) ) && can_see_obj( ch, obj ) ){
			if ( !found ){
				strcat( buf1, "\n\r" );
				strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
				found = TRUE;
			}
			strcat( buf1, where_name[WEAR_AMMO] );
			strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
			strcat( buf1, "\n\r" );
			++count;
		}
        if ( ( obj = get_eq_char( victim, WEAR_RIDE ) ) && can_see_obj(ch, obj) ){
            if ( !found ) {
                strcat( buf1, "\n\r" );
                strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                found = TRUE;
            }
            strcat( buf1, where_name[WEAR_RIDE] );
            strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
            strcat( buf1, "\n\r" );
            ++count;
        }
    }else{ // Code for beholder's wearing
        for ( iWear = 0; iWear <= WEAR_FINGER_R; iWear++ ){ // from light source to the second ring.
            if ( ( obj = get_eq_char( victim, iWear ) )
                && can_see_obj( ch, obj ) ){
                if ( !found ){
                    strcat( buf1, "\n\r" );
                    strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                    found = TRUE;
                }
                strcat( buf1, where_name[iWear] );
                strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                strcat( buf1, "\n\r" );
                ++count;
            }
        }
        // Finger 3 and Finger 4 for beholders
        for ( iWear = WEAR_FINGER_3; iWear <= WEAR_FINGER_4; iWear++ ){
            if ( ( obj = get_eq_char( victim, iWear ) )
                && can_see_obj( ch, obj ) ){
                if ( !found ){
                    strcat( buf1, "\n\r" );
                    strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                    found = TRUE;
                }
                strcat( buf1, where_name[iWear] );
                strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                strcat( buf1, "\n\r" );
                ++count;
            }
        }

        for ( iWear = WEAR_NECK_1; iWear <= WEAR_HANDS; iWear++ ){ // from necklace to the end
            if ( ( obj = get_eq_char( victim, iWear ) )
                && can_see_obj( ch, obj ) ){
                 if ( !found ){
                     strcat( buf1, "\n\r" );
                     strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                     found = TRUE;
                 }
                 strcat( buf1, where_name[iWear] );
                 strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                 strcat( buf1, "\n\r" );
                 ++count;
            }
        }
        if ( ( obj = get_eq_char( victim, WEAR_HANDS_2 ) ) && can_see_obj(ch, obj) ){
            if ( !found ){
                strcat( buf1, "\n\r" );
                strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                found = TRUE;
            }
            strcat( buf1, where_name[WEAR_HANDS_2] );
            strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
            strcat( buf1, "\n\r" );
            ++count;
        }
        if ( ( obj = get_eq_char( victim, WEAR_ARMS ) ) && can_see_obj(ch, obj) ){
            if ( !found ){
                strcat( buf1, "\n\r" );
                strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                found = TRUE;
            }
            strcat( buf1, where_name[WEAR_ARMS] );
            strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
            strcat( buf1, "\n\r" );
            ++count;
        }
        if ( ( obj = get_eq_char( victim, WEAR_ARMS_2 ) ) && can_see_obj(ch, obj) ){
            if ( !found ){
                strcat( buf1, "\n\r" );
                strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                found = TRUE;
            }
            strcat( buf1, where_name[WEAR_ARMS_2] );
            strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
            strcat( buf1, "\n\r" );
            ++count;
        }
        for ( iWear = WEAR_SHIELD; iWear < MAX_WEAR; iWear++ ){ // from necklace to the end
            if ( ( obj = get_eq_char( victim, iWear ) ) // MAX_WEAR  excludes ride, additional rings and sleeves
                && can_see_obj( ch, obj ) ){
                 if ( !found ){
                     strcat( buf1, "\n\r" );
                     strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                     found = TRUE;
                 }
                 strcat( buf1, where_name[iWear] );
                 strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                 strcat( buf1, "\n\r" );
                 ++count;
            }
        }
		// ·s¼W¼uÃÄ¸Ë³Æ 2022/04/30
		if ( ( obj = get_eq_char( victim, WEAR_AMMO ) ) && can_see_obj( ch, obj ) ){
			if ( !found ){
				strcat( buf1, "\n\r" );
				strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
				found = TRUE;
			}
			strcat( buf1, where_name[WEAR_AMMO] );
			strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
			strcat( buf1, "\n\r" );
			++count;
		}
        if ( ( obj = get_eq_char( victim, WEAR_RIDE ) ) && can_see_obj(ch, obj) ){
            if ( !found ){
                strcat( buf1, "\n\r" );
                strcat( buf1, "$E¨­¤W¬ïµÛ¡G\n\r" );
                found = TRUE;
            }
            strcat( buf1, where_name[WEAR_RIDE] );
            strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
            strcat( buf1, "\n\r" );
            ++count;
        }
    }

    act( buf1, ch, NULL, victim, TO_CHAR );
    buf1[0] = '\0';
    count = 0;
    if ( victim != ch && !IS_NPC( ch ) &&  number_percent() < get_skill_percent(ch,gsn_peek)){
        if ( IS_NPC(victim) && victim->pIndexData->pShop )
            strcat( buf1, "\n\r¤@ºØ©_©Çªº¤O¶qªýµ´§Aªº¿sµø¡I\n\r" );
        else
            show_list_to_char1( buf, victim->carrying, ch, TRUE, TRUE );
    }

    act( buf1, ch, NULL, victim, TO_CHAR );
    return;
}


/* called when PC look environment or enter a room */
void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
	CHAR_DATA *rch;

	if ( !list )
		return;

	for ( rch = list; rch; rch = rch->next_in_room )
	{
		if ( rch->deleted || rch == ch )
			continue;

		if( !can_see(ch, rch))
			continue;
		//if ( !IS_NPC( rch )
		//  && IS_SET( rch->act, PLR_WIZINVIS ))
			//continue;

		if ( can_see( ch, rch ) )
		{
			show_char_to_char_0( rch, ch );
		}
		else if ( room_is_dark( ch->in_room )
				&& ( IS_AFFECTED( rch, AFF_INFRARED )
				|| IS_SET( race_table[ rch->race ].race_abilities, RACE_INFRAVISION ) ) )
		{
			send_to_char( "§A·P¨ì¦³¤H¥¿ª`µøµÛ§A¡I\n\r", ch );
		}
	}

	return;
} 



bool check_blind( CHAR_DATA *ch )
{
    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_HOLYLIGHT ) && IS_IMMORTAL( ch ) )
  return TRUE;

    if ( IS_AFFECTED( ch, AFF_BLIND ) )
    {
  send_to_char( "§A¤°»ò³£¬Ý¤£¨ì¡I\n\r", ch );
  return FALSE;
    }

    return TRUE;
}

void top10_remove( TOP10_DATA * td, CHAR_DATA * ch, int value, bool fIMM ){
    int i,j;
    //int fFound = FALSE;
    char * id = str_dup( ch->name);
    char * none = "n / a";

    if( !fIMM && IS_IMMORTAL( ch ) ) return;
    
    for( i = 0; i < TOP_COUNT; ++i ){
      if( td[i].id && td[i].id[0] != '\0' && !str_cmp( td[i].id, id ) ) {
        for( j = i; j < TOP_COUNT - 1; ++j ){
          td[j].id = td[j+1].id;
          td[j].value = td[j+1].value;
        }
        td[TOP_COUNT].id = none;
        td[TOP_COUNT].value = 0;
        break;
      }
    }

    if (value <= td[TOP_COUNT].value) return;

    for( i = 0; i< TOP_COUNT; ++i ) {
      if( td[i].value < value ) {
        for( j=TOP_COUNT; j>i; --j ) {
          td[j].id = td[j-1].id;
          td[j].value = td[j-1].value;
        }
        td[i].id = id;
        td[i].value = value;
        break;
      }
    }
    
    return;
}

void top10_check( CHAR_DATA * ch )
{
  if( IS_SET(ch->act, PLR_HORSE)) return;
  top10_remove(tmudage, ch, ch->played / 3600, FALSE);
  top10_remove(tgold, ch, ch->bank + ch->gold, FALSE);
  top10_remove(texp, ch, ch->exp + ch->practice*5000, FALSE);
  top10_remove(thp, ch, ch->max_hit, FALSE);
  top10_remove(t_mp, ch,ch->max_mana, FALSE);
  top10_remove(tmv, ch, ch->max_move, FALSE);
  top10_remove(tpk, ch, ch->pcdata->pkwin, FALSE);
  top10_remove(tresp, ch, ch->pcdata->respect, FALSE);
  top10_remove(tmerit, ch, ch->pcdata->merit, FALSE);
  top10_save();
}

void show_top10(CHAR_DATA *ch, TOP10_DATA *td,
                char *title, char *prefix, char *posfix) {
  int i;
  char buf[MAX_STRING_LENGTH];
  char show[MAX_STRING_LENGTH];
  send_to_char(title, ch);
  send_to_char("\n\r==============================="
               "=========================================\n\r", ch);
  show[0] = 0;
  for (i = 0; i < TOP_COUNT; ++i) {
    if (i < 10) {
      sprintf(buf, "%s[%2d] %-20s %s %5d %s¡C"NOR"\n\r",
          ( str_cmp(td[i].id, ch->name) ) ? HIW: HIY,
          i + 1,
          td[i].id,
          prefix,
          td[i].value,
          posfix);
    } else {
      sprintf(buf, "%s[%2d] %-20s %s %5d %s¡C"NOR"\n\r",
          ( str_cmp(td[i].id, ch->name) ) ? CYN: HIC,
          i + 1,
          td[i].id,
          prefix,
          td[i].value,
          posfix);
    }
    strcat(show, buf);
  }
  send_to_char(show, ch);
  send_to_char("\n\r==============================="
               "=========================================\n\r", ch);
}

void do_top10( CHAR_DATA *ch, char *argument )
{
  //char buf [MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int i, j;
  char * none = "n / a"; 
  TOP10_DATA * ranks[9] = { tmudage, tgold, texp, thp, t_mp, tmv, tpk ,tresp, tmerit};
  argument = one_argument(argument, arg);
  if(arg[0]=='\0') {
    send_to_char("½Ð¿é¤J·Q¬Ýªº±Æ¦WºØÃþ¡C\n\r", ch );
    send_to_char("¥i¿ï¾ÜªººØÃþ¦³¡Ghp, mana, move, resp, merit, gold, "
                 "mudage, pk\n\r", ch);
    return;
  }

  if( !str_cmp( arg, "reset" )){
    if( !IS_IMMORTAL( ch ) ) {
      send_to_char("¤°»ò¡H\n\r", ch);
      return;
    }
    if( IS_NPC(ch) || get_trust(ch) < 45 ) {
      send_to_char("§AªºÅv­­¤£°÷¡C\n\r", ch);
      return;
    }
    for( j = 0; j < 9; ++j) {
      for( i = 0; i < TOP_COUNT + 1; ++i ) { 
        ranks[j][i].id = none;
        ranks[j][i].value = 0;
      }
    }
    
    return;
  }

  if( !str_cmp( arg, "pk" )){
    show_top10(ch, tpk, "[¤Û¥@PK¤ý±Æ¦æ]", "PK ¬ö¿ý", "³Ó");
  } else if(!str_cmp(arg, "mudage")) {
    show_top10(ch, tmudage, "[¤Û¥@¤W½u®É¶¡±Æ¦æ]", "¤W½u®É¶¡", "¤p®É");
  } else if(!str_cmp(arg, "gold")) {
    show_top10(ch, tgold, "[¤Û¥@ 50 ¤j´I»¨±Æ¦æ]", "°]²£¦@¦³", "ªT¥v¹ô");
  } else if(!str_cmp(arg, "exp")) {
    show_top10(ch, texp, "[¤Û¥@ 50 ¤j¦ÌÂÎ±Æ¦æ]", "¸gÅçÂI¼Æ", "ÂI");
  } else if(!str_cmp(arg, "hp")) {
    show_top10(ch, thp, "[¤Û¥@ 50 ¤j¦å¤û±Æ¦æ]", "¥Í©R¤O", "ÂI");
  } else if(!str_cmp(arg, "mana")) {
    show_top10(ch, t_mp, "[¤Û¥@ 50 ¤jªk¤O±Æ¦æ]", "ªk¤O", "ÂI");
  } else if(!str_cmp(arg, "move")) {
    show_top10(ch, tmv, "[¤Û¥@ 50 ¤jÅé¯à±Æ¦æ]", "Åé¤O", "ÂI");
  } else if(!str_cmp(arg, "resp")) {
    show_top10(ch, tresp, "[¤Û¥@ 50 ¤j¦W¤H±Æ¦æ]", "«Â±æ­È", "ÂI");
  } else if(!str_cmp(arg, "merit")) {
    show_top10(ch, tmerit, "[¤Û¥@ 50 ¤j¥\\¾±±Æ¦æ]", "¥\\¾±­È", "ÂI");
  }

  /* OLD TOP10

  if( !str_cmp( arg, "pk" )){
  send_to_char("[¤Û¥@¤Q¤jPK¤ý±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s PK ¬ö¿ý %4d ³Ó¡C%s\n\r",
        ( str_cmp(tpk[i].id, ch->name) ) ?
    PUR: HIP,
    i+1,
    tpk[i].id,
    tpk[i].value ,
    NOR);

    send_to_char(buf, ch);
  }

  send_to_char("========================================================================\n\r", ch);
  return;
    }
    if( !str_cmp( arg, "mudage" )){
  send_to_char("[¤Û¥@¤Q¤j¤W½u®É¶¡±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s ¤W½u®É¶¡ %4d ¤p®É¡C%s\n\r",
        ( str_cmp(tmudage[i].id, ch->name) ) ?
    WHT: HIW,
    i+1,
    tmudage[i].id,
    tmudage[i].value / 3600,
    NOR);

    send_to_char(buf, ch);
  }

  send_to_char("========================================================================\n\r", ch);
  return;
    }
    if( !str_cmp( arg, "gold" )){
  send_to_char("[¤Û¥@¤Q¤j´I»¨±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s °]²£¦@¦³ %d ªTª÷¹ô¡C%s\n\r",
        ( str_cmp(tgold[i].id, ch->name) ) ?
    YEL : HIY,
    i+1,
    tgold[i].id,
    tgold[i].value ,
    NOR);

    send_to_char(buf, ch);
  }

  send_to_char("========================================================================\n\r", ch);
  return;
    }
    if( !str_cmp( arg, "exp" )){
  send_to_char("[¤Û¥@¤Q¤j°ª¤â±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s ¸gÅçÂI¼Æ %10d ÂI¡C%s\n\r",
        ( str_cmp(texp[i].id, ch->name) ) ?
    GRN : HIG,
    i+1,
    texp[i].id,
    texp[i].value ,
    NOR);

    send_to_char(buf, ch);
  }

  send_to_char("========================================================================\n\r", ch);
  return;
    }
    if( !str_cmp( arg, "hp" )){
  send_to_char("[¤Û¥@¤Q¤j¦å¤û±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s ¥Í©RÂI¼Æ %5d ÂI¡C%s\n\r",
        ( str_cmp(thp[i].id, ch->name) ) ?
    CYN : HIC,
    i+1,
    thp[i].id,
    thp[i].value ,
    NOR);

    send_to_char(buf, ch);
  }

  send_to_char("========================================================================\n\r", ch);
  return;
    }
    if( !str_cmp( arg, "mana" )){
  send_to_char("[¤Û¥@¤Q¤jÅ]ªk°ª¤â±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s Å]¤OÂI¼Æ %5d ÂI¡C%s\n\r",
        ( str_cmp(t_mp[i].id, ch->name) ) ?
    CYN : HIC,
    i+1,
  t_mp[i].id,
    t_mp[i].value ,
    NOR);

    send_to_char(buf, ch);
  }

  send_to_char("========================================================================\n\r", ch);
  return;
    }
    if( !str_cmp( arg, "move" )){
  send_to_char("[¤Û¥@¤Q¤jÅé¤O°ª¤â±Æ¦æ]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s ²¾°Ê¤O %5d ÂI¡C%s\n\r",
        ( str_cmp(tmv[i].id, ch->name) ) ?
    CYN : HIC,
    i+1,
    tmv[i].id,
    tmv[i].value ,
    NOR);

    send_to_char(buf, ch);
  }
  send_to_char("========================================================================\n\r", ch);
  return;
    }

    if( !str_cmp( arg, "resp" )){
      send_to_char("[¤Û¥@¤Q¤j¦W¤H±Æ¦æ]\n\r========================================================================\n\r", ch);
       for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s «Â±æ­È %5d ÂI¡C%s\n\r",
            ( str_cmp(tresp[i].id, ch->name) ) ?
            CYN : HIC,
            i+1,
            tresp[i].id,
            tresp[i].value ,
            NOR);
        send_to_char(buf, ch);
      }
      send_to_char("========================================================================\n\r", ch);
      return;
    }
    
    if( !str_cmp( arg, "merit" )){
      send_to_char("[¤Û¥@¤Q¤j¥\\¾±±Æ¦æ]\n\r========================================================================\n\r", ch);
       for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s ¥\\¾±­È %5d ÂI¡C%s\n\r",
            ( str_cmp(tmerit[i].id, ch->name) ) ?
            CYN : HIC,
            i+1,
            tmerit[i].id,
            tmerit[i].value ,
            NOR);
        send_to_char(buf, ch);
      }
      send_to_char("========================================================================\n\r", ch);
      return;
    }
    return;
  */
}


void do_look( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	CHAR_DATA *victim, *rch;
	EXIT_DATA *pexit;
	char buf  [ MAX_STRING_LENGTH ];
	char arg1 [ MAX_INPUT_LENGTH  ];
	char arg2 [ MAX_INPUT_LENGTH  ];
	char *pdesc;
	int door;

	if ( !IS_NPC( ch ) && !ch->desc ) 
		return;

	if ( ch->position < POS_SLEEPING )
	{
		send_to_char( "[33m§A¥u¯à¬Ý¨ì¬P¬P¦Ó¤w¡I¦h¬üÄR°Ú¡I[m\n\r",ch );
		return;
	}

	if ( ch->position == POS_SLEEPING )
	{
		send_to_char( "[33m§A¥u¯à¬Ý¨ì¹Ú¹Ò¦Ó¤w¡IZzz...[m\n\r", ch );
		return;
	}

	if ( !check_blind( ch ) )
		return;

	if ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_HOLYLIGHT ) && room_is_dark( ch->in_room ) )
	{
		send_to_char( "¤@¤ùº£¶Â....\n\r", ch );
		show_char_to_char( ch->in_room->people, ch );
		return;
	}

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) || !str_cmp( arg1, "move" ) )
	{
		/* 'look' or 'look auto' */
		if( IS_IMMORTAL(ch) || ( !IS_NPC( ch ) && ch->clan && IS_PPLBUILDER(ch, ch->in_room->area)))
		{
			sprintf(buf, "["HIY"%d"NOR"]", ch->in_room->vnum);
			send_to_char(buf, ch);
		}

		send_to_char( ch->in_room->name, ch );
		send_to_char(" - ",ch);
		if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_AUTOEXIT ) )
			do_exits( ch, "auto" );

		if ( arg1[0] == '\0' || ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_BRIEF ) ) )
			send_to_char( "    ", ch );
		
		send_to_char( ch->in_room->description, ch );


		if ( IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
			send_to_char( "³o¸Ì²§±`¦a¦wÀR¡C\n\r", ch );

		// evacuate mark
		if ( (get_skill_level(ch,gsn_evacuate) > 2) && IS_SET( ch->in_room->room_flags, ROOM_CAMP ) )
			send_to_char(HIU"³o¸Ìªº¦a­±¬yÂ«µÛÅ]¤O¡C"NOR"\n\r", ch );

	#ifdef HAS_ROOM_AFFECT
	  
		if( IS_AFFECTED(ch->in_room, R_AFF_FIREWALL))
		{
		  send_to_char(HIR"\n\r\t¤@¹D¤õÀð¥¿¦b³o¸Ì¿U¿NµÛ¡C"NOR"\n\r", ch);
		}
	#endif

		if( ch->in_room->people )
		{
			send_to_char("\n\r",ch);
			if ( str_cmp( arg1, "move" ) || !IS_SET( ch->act, PLR_BRIEF ) )
				show_char_to_char( ch->in_room->people, ch );
			else//in big group, grep the redundant message.
			{
				for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
				{
					if ( rch->deleted || rch == ch )
						continue;

					if ( !IS_NPC( rch ) && IS_SET( rch->act, PLR_WIZINVIS ))
						continue;
					if( !can_see(ch, rch))
						continue;

					if ( can_see( ch, rch ) && ( !is_same_group( ch, rch ) || (ch->group && ch->group->leader == rch )) )
					{ 
						show_char_to_char_0( rch, ch );
					}
					else if ( room_is_dark( ch->in_room )
							&& (   IS_AFFECTED( rch, AFF_INFRARED )
								|| IS_SET( race_table[ rch->race ].race_abilities, RACE_INFRAVISION ) ) )
					{
						send_to_char( "§A·P¨ì¦³¤H¥¿ª`µøµÛ§A¡I\n\r", ch );
					}
				}
			}
		}
		if( ch->in_room->contents )
		{
			send_to_char("\n\r",ch);
			show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
		}
		return;
	}

	if ( !str_cmp( arg1, "in" ) || ( !str_prefix( arg1, "in" ) && arg2[0] ) )
	{
		/* 'look in' */
		if ( arg2[0] == '\0' )
		{
			send_to_char( "[33m­n©¹¤°»ò¸Ì­±¬Ý¡H[m\n\r", ch );
			return;
		}

		if ( !( obj = get_obj_here( ch, arg2 ) ) )
		{
			send_to_char( "[33m§A¦b³o¸Ì¨S¬Ý¨ì¨ºª±·N¨à¡C[m\n\r", ch );
			return;
		}

		// ¹ï¯à³Q look in ªº obj type ·s¼W obj_act look_in 2022/02/12
		switch ( obj->item_type )
		{
			default:
				send_to_char( "[33m¨º¤£¬O­Ó®e¾¹¡C[m\n\r", ch );
				break;

			case ITEM_DRINK_CON:
				if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
				{
					bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
					obj->value[2] = 0;
				}
				if ( obj->value[1] <= 0 && obj->value[1] != -1 )
				{
					send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}
				sprintf( buf, "¨º¸Ì­±¦³%s%s²GÅé¡C\n\r",
				(obj->value[1] == -1) 
				? "¦p¦P¤p¦t©z¯ë¥¨¶qªº" : 
				obj->value[1] <     obj->value[0] / 4 
				? "¤£¨ì¤@¥bªº" : 
				obj->value[1] < 3 * obj->value[0] / 4 
				? "¤j¬ù¤@¥bªº" :       "¶W¹L¤@¥bªº",
				liq_table[obj->value[2]].liq_color);
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;

			case ITEM_FOUNTAIN:
				if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
				{
					bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
					obj->value[2] = 0;
				}
				if ( obj->value[1] <= 0 && obj->value[1] != -1 )
				{
					send_to_char( "[36m¨º¬O°®ªº¡C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}
				sprintf( buf, "¨º¸Ì­±¬O%s%s¡C\n\r",
				(obj->value[1] == -1) 
				? "·½·½¤£µ´ªº" : 
				obj->value[1] <     obj->value[0] / 4 
				? "®þ®þªº" : 
				obj->value[1] < 3 * obj->value[0] / 4 
				? "½w½w¬y°Êªº" :       "²F²Fªº",
				liq_table[obj->value[2]].liq_name);
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_CORPSE_PC:
				if ( IS_SET( obj->value[1], CONT_CLOSED ) )
				{
					send_to_char( "[33m¥¦¬OÃöµÛªº¡C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}

				act( "$p¸Ì­±¦³¡G", ch, obj, NULL, TO_CHAR );
				show_list_to_char( obj->contains, ch, TRUE, TRUE );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
			case ITEM_HORSE:
				if ( obj->value[0] == -1 )
				{
					send_to_char( "[33m¨e¨­¤W¨S¦³­t¸ü¥ô¦óªF¦è¡C[m\n\r", ch);
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break; 
				}

				act( "$p¨­¤W­t¸üµÛ¡G", ch, obj, NULL, TO_CHAR );
				show_list_to_char( obj->contains, ch, TRUE, TRUE );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
			case ITEM_ARROW:
				if ( obj->value[0] <= 0 )
				{
					// ¼W¥[ -1 ªº±¡ªp 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m¸Ì­±½b¥Úªº¼Æ¶q¦³¦p¯EÃvªº¤p¦t©z¡C[m\n\r", ch );
					else
						send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}

				sprintf( buf, "¨º¸Ì­±ÁÙ¦³%d¤ä½b¡C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
				
			case ITEM_AMMO:   // add at 2022/05/06
				if ( obj->value[0] <= 0 )
				{
					// ¼W¥[ -1 ªº±¡ªp 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m¸Ì­±¼uÃÄªº¼Æ¶q¦³¦p¯EÃvªº¤p¦t©z¡C[m\n\r", ch );
					else
						send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}

				sprintf( buf, "ÁÙ¦³%dµo¼uÃÄ¡C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;


			case ITEM_DART:

				sprintf( buf, "ÁÙ¦³%d¥ó¹D¨ã¡C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
		}
		return;
	}

	if ( ( victim = get_char_room( ch, arg1 ) ) )
	{
		show_char_to_char_1( victim, ch );
		return;
	}

	pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
	if ( pdesc )
	{
		send_to_char( pdesc, ch );
		return;
	}

	for ( obj = ch->carrying; obj; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) )
		{
			pdesc = get_extra_descr( arg1, obj->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}

			pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}
		}

		if ( is_name( arg1, obj->name ) )
		{
			send_to_char( obj->short_descr , ch);
			send_to_char( " - ", ch);
			send_to_char( obj->material->short_descr, ch);
			send_to_char("\n\r", ch);
			send_to_char( obj->description, ch );
			send_to_char( "\n\r", ch );
			return;
		}
	}

	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) )
		{
			pdesc = get_extra_descr( arg1, obj->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}

			pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}
		}

		if ( is_name( arg1, obj->name ) )
		{
			send_to_char( obj->short_descr , ch);
			send_to_char( " - ", ch);
			send_to_char( obj->material->short_descr, ch);
			send_to_char("\n\r", ch);
			send_to_char( obj->description, ch );
			send_to_char( "\n\r", ch );
			return;
		}
	}

	pdesc = get_pfxextra_descr( arg1, ch->in_room->extra_descr );
	if ( pdesc )
	{
		//if( obj ) {
		//	send_to_char( obj->short_descr , ch);
		//	send_to_char( " - ", ch);
		//	send_to_char( obj->material->short_descr, ch);
		//	send_to_char("\n\r", ch);
		//}
		send_to_char( pdesc, ch );
		return;
	}


	//	2022/02/02
	//  ¥Î get_obj_here ¨ú¥N obj ªº pfx §P©w
	// ¥H¦¹Á×§K¯S©w¦ì¸m§ì¤£¨ì obj name ªº¤£Åã¥Ü¦WºÙ ( eg: look 2.sword , look 3.wa )
	/*
	for ( obj = ch->carrying; obj; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) )
		{
			pdesc = get_pfxextra_descr( arg1, obj->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}

			pdesc = get_pfxextra_descr( arg1, obj->pIndexData->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}
		}

		if ( is_pfxname( arg1, obj->name ) )
		{
			send_to_char( obj->short_descr , ch);
			send_to_char( " - ", ch);
			send_to_char( obj->material->short_descr, ch);
			send_to_char("\n\r", ch);
			send_to_char( obj->description, ch );
			send_to_char( "\n\r", ch );
			return;
		}
	}

	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) )
		{
			pdesc = get_pfxextra_descr( arg1, obj->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}

			pdesc = get_pfxextra_descr( arg1, obj->pIndexData->extra_descr );
			if ( pdesc )
			{
				send_to_char( obj->short_descr , ch);
				send_to_char( " - ", ch);
				send_to_char( obj->material->short_descr, ch);
				send_to_char("\n\r", ch);
				send_to_char( pdesc, ch );
				return;
			}
		}

		if ( is_pfxname( arg1, obj->name ) )
		{
			send_to_char( obj->short_descr , ch);
			send_to_char( " - ", ch);
			send_to_char( obj->material->short_descr, ch);
			send_to_char("\n\r", ch);
			send_to_char( obj->description, ch );
			send_to_char( "\n\r", ch );
			return;
		}
	}*/
	if ( ( obj = get_obj_here( ch, arg1 ) ) ){

		pdesc = get_pfxextra_descr( arg1, obj->extra_descr );
		if ( pdesc )
		{
			send_to_char( obj->short_descr , ch);
			send_to_char( " - ", ch);
			send_to_char( obj->material->short_descr, ch);
			send_to_char("\n\r", ch);
			send_to_char( pdesc, ch );
			return;
		}

		pdesc = get_pfxextra_descr( arg1, obj->pIndexData->extra_descr );
		if ( pdesc )
		{
			send_to_char( obj->short_descr , ch);
			send_to_char( " - ", ch);
			send_to_char( obj->material->short_descr, ch);
			send_to_char("\n\r", ch);
			send_to_char( pdesc, ch );
			return;
		}

		send_to_char( obj->short_descr , ch);
		send_to_char( " - ", ch);
		send_to_char( obj->material->short_descr, ch);
		send_to_char("\n\r", ch);
		send_to_char( obj->description, ch );
		send_to_char( "\n\r", ch );
		return;
	}

	if ( !str_prefix( arg1, "north" ) ) door = 0;
	else if ( !str_prefix( arg1, "east"  ) ) door = 1;
	else if ( !str_prefix( arg1, "south" ) ) door = 2;
	else if ( !str_prefix( arg1, "west"  ) ) door = 3;
	else if ( !str_prefix( arg1, "up"  ) ) door = 4;
	else if ( !str_prefix( arg1, "down"  ) ) door = 5;
	else
	{
		send_to_char( "[33m§A¦b³o¸Ì¨S¬Ý¨ì¨ºª±·N¨à¡C[m\n\r", ch );
		return;
	}

	/* 'look direction' */
	if ( !( pexit = ch->in_room->exit[door] ) )
	{
		send_to_char( "[33m¨º¸Ì¨S¤°»ò¯S§OªºªF¦è¡C[m\n\r", ch );
		return;
	}

	if ( pexit->description && pexit->description[0] != '\0' )
		send_to_char( pexit->description, ch );
	else
		send_to_char( "[33m¨º¸Ì¨S¤°»ò¯S§OªºªF¦è¡C[m\n\r", ch );

	if (   pexit->keyword
		&& pexit->keyword[0] != '\0'
		&& pexit->keyword[0] != ' ' )
	{
		if ( IS_SET( pexit->exit_info, EX_BASHED ) )
			act( "[36m$d³Q¼²Ãa¤F¡C[m", ch, NULL, pexit->keyword, TO_CHAR );
		else if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
			act( "[36m$dÃö¤F¡C[m", ch, NULL, pexit->keyword, TO_CHAR );
		else if ( IS_SET( pexit->exit_info, EX_ISDOOR ) )
			act( "[36m$d¬O¶}µÛªº¡C[m",   ch, NULL, pexit->keyword, TO_CHAR );
	}

	return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char buf [ MAX_STRING_LENGTH ];
	char arg [ MAX_INPUT_LENGTH  ];

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m­nÀË¬d¤°»òª««~¡H[m\n\r", ch );
		return;
	}


	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		if ( !obj ) return;
		int diff;
		// ²¾°£¥H do_look ¨ÓÅã¥Üª««~¦W¤Î§÷½è 2022/02/01
		//do_look( ch, argument );
		send_to_char( obj->short_descr , ch);
		send_to_char( " - ", ch);
		send_to_char( obj->material->short_descr, ch);
		send_to_char("\n\r", ch);
		send_to_char( obj->description, ch );
		send_to_char( "\n\r", ch );
		// ²¾°£ serial Åã¥Ü 2022/02/02
		//sprintf(buf,"ª««~§Ç¸¹¡G%ld\n\r", (long)obj->serial);
		//send_to_char(buf, ch);
		// ·s¼W obj_act examine , examine ¥Î¦b³¡¤À item type ¤W¥i¯à·|»P look_in ­«ÂÐ 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("examine", ch, obj);
		}
		switch ( obj->item_type )
		{
			default:
				break;

			case ITEM_LIGHT:
				if( obj->value[2] != -1 ){
					sprintf(buf, "¬Ý¨ÓÁÙ¯à¨Ï¥Î %d ¤p®É¡C\n\r", obj->value[2] );
					send_to_char(buf, ch);
				}
				break;

			/* ±Ncase armor ¤Î weapon ¤À¶}§P©w 2021/12/13 */
			case ITEM_ARMOR:
				send_to_char("§A¥J²Ó¥´¶q¤§«á¡Aµo²{¥¦", ch);
				if( obj->value[1] != 0)
					send_to_char("¨ü¨ì¤F»G»k¡A¥~Æ[¬Ý°_¨Ó", ch);

				if( indestructible(obj) ) {
					send_to_char(HIW"¥¿³B¦b§¹¬üªºª¬ªp¤U¡C"NOR"\n\r", ch);
					break;
				}else if ( get_max_dura(obj) == 0 ) {
					send_to_char(RED"¦n¹³¤@¥¹¥ÎÃa´NµLªk­×´_¡C"NOR"\n\r", ch);
					break;
				}
				diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);
				if( diff == 100 ) send_to_char(HIW"¥¿³B¦b§¹¬üªºª¬ªp¤U"NOR"¡C\n\r", ch);
				else if( diff > 70) send_to_char(HIC"¬Ý°_¨Ó¬Û·í¨}¦n"NOR"¡C\n\r", ch);
				else if( diff > 50) send_to_char(HIY"µy·L¦³¨Ç¨í²ª"NOR"¡C\n\r", ch);
				else if( diff > 33) send_to_char(YEL"ª¬ªp¤£¤Ó¦n"NOR"¡C\n\r", ch);
				else if( diff > 20) send_to_char(HIR"®t¤£¦h§ÖÃa¤F"NOR"¡C\n\r", ch);
				else if( diff > 10) send_to_char(RED"¤WÀYªº³¡¥ó¼@¯P®Ì°ÊµÛ"NOR"¡C\n\r", ch);
				else if( diff > 0 ) send_to_char(RED"¦n¹³»´»´¤@¸I´N·|¸ÑÅé"NOR"¡C\n\r", ch);
				else send_to_char(HIB"¤w¸g§¹¥þ·lÃa¡A¤£³ô¨Ï¥Î"NOR"¡C\n\r", ch);
				break;

			case ITEM_WEAPON:
				send_to_char("§A¥J²Ó¥´¶q¤§«á¡Aµo²{¥¦", ch);
				/* ·s¼W weapon type Åã¥Ü 2021/12/13 */
				if( obj->value[3] == 1 )
					send_to_char("¯à§@¬°§Q¤b¤Á³Î¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 2 )
					send_to_char("¯à§@¬°ªøºj¬ð¨ë¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 3 )
					send_to_char("¯à§@¬°¤M¼C¼A¬å¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 4 )
					send_to_char("¯à§@¬°Ã@¤l´§»R¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 5 )
					send_to_char("¯à¥Î¨Ó§ìÀ»¹ï¤â¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 6 )
					send_to_char("¯à¼Q¥XÅ]ªk¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 7 )
					send_to_char("¯à¥Î¨ÓºVÀ»¹ï¤â¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 8 )
					send_to_char("¯à§@¬°­«Âñ¯»¸H¹ï¤â¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 9 )
					send_to_char("¯à§@¬°­««¬§Q¾¹¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 10 )
					send_to_char("¯à«r¦í¥Íª«¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 11 )
					send_to_char("¯à§@¬°¤P­º¨ëÀ»¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 12 )
					send_to_char("¯à§l¦í¥Íª«¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 13 )
					send_to_char("¯à§@¬°®ã¤M³s±Ù¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 14 )
					send_to_char("¯à§@¬°¤}¨Ï¥Î¡A¥~Æ[¬Ý°_¨Ó", ch);
				else if( obj->value[3] == 16 )  // add at 2022/05/06
					send_to_char("¯à§@¬°ºj±ñ¨Ï¥Î¡A¥~Æ[¬Ý°_¨Ó", ch);

				if( indestructible(obj) ) {
					send_to_char(HIW"¥¿³B¦b§¹¬üªºª¬ªp¤U"NOR"¡C\n\r", ch);
					break;
				}else if ( get_max_dura(obj) == 0 ) {
					send_to_char(RED"¦n¹³¤@¥¹¥ÎÃa´NµLªk­×´_"NOR"¡C\n\r", ch);
					break;
				}
				diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);
				if( diff == 100 ) send_to_char(HIW"¥¿³B¦b§¹¬üªºª¬ªp¤U"NOR"¡C\n\r", ch);
				else if( diff > 70) send_to_char(HIC"¬Ý°_¨Óª¬ªp¦nªº«Ü"NOR"¡C\n\r", ch);
				else if( diff > 50) send_to_char(HIY"µy·L¦³¨Ç¨í²ª"NOR"¡C\n\r", ch);
				else if( diff > 33) send_to_char(YEL"ª¬ªp¤£¤Ó¦n"NOR"¡C\n\r", ch);
				else if( diff > 20) send_to_char(HIR"®t¤£¦h§ÖÃa¤F"NOR"¡C\n\r", ch);
				else if( diff > 10) send_to_char(RED"¤WÀYªº³¡¥ó¼@¯P®Ì°ÊµÛ"NOR"¡C\n\r", ch);
				else if( diff > 0 ) send_to_char(RED"¦n¹³»´»´¤@¸I´N·|¸ÑÅé"NOR"¡C\n\r", ch);
				else send_to_char(HIB"¤w¸g§¹¥þ·lÃa¡A¤£³ô¨Ï¥Î"NOR"¡C\n\r", ch);
				break;

			case ITEM_DRINK_CON:
			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_CORPSE_PC:
			case ITEM_FOUNTAIN:
				send_to_char( "§A©¹¸Ì­±¬Ýªº®É­Ô¡A§A¬Ý¨ì¡G\n\r", ch );
				sprintf( buf, "in %s", arg );
				do_look( ch, buf );
				break;

			case ITEM_ARROW:
				if ( obj->value[0] <= 0 ){
					// ¼W¥[ -1 ªº±¡ªp 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m¸Ì­±½b¥Úªº¼Æ¶q¦³¦p¯EÃvªº¤p¦t©z¡C[m\n\r", ch );
					else
						send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
					break;
				}
				sprintf( buf, "¨º¸Ì­±ÁÙ¦³%d¤ä½b¡C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				break;
			case ITEM_AMMO:  // add at 2022/05/06
				if ( obj->value[0] <= 0 ){
					// ¼W¥[ -1 ªº±¡ªp 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m¸Ì­±¼uÃÄªº¼Æ¶q¦³¦p¯EÃvªº¤p¦t©z¡C[m\n\r", ch );
					else
						send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
					break;
				}
				sprintf( buf, "ÁÙ¦³%dµo¼uÃÄ¡C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				break;
			case ITEM_DART:
				sprintf( buf, "ÁÙ¦³%d¥ó¹D¨ã¡C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				break;
			case ITEM_HORSE:
				send_to_char( "§Aªº°¨¨­¤W©ñµÛ¡G\n\r",ch);
				sprintf( buf, "in %s",arg);
				do_look(ch,buf);
				break;
		}
	}
	
	// ¨S¦³¥s§@ arg ªºªF¦è¦b©Ð¶¡¤¤ 2022/02/02
	if(!obj)
		send_to_char( "[33m§A¦b³o¸Ì¨S¬Ý¨ì¨ºª±·N¨à¡C[m\n\r", ch );

	return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
	EXIT_DATA     *pexit;
	extern char *    const  dir_name [ ];
	char       buf      [ MAX_STRING_LENGTH ];
	int        door;
	bool       found;
	bool       fAuto;

	buf[0] = '\0';
	fAuto  = !str_cmp( argument, "auto" );

	if ( !check_blind( ch ) )
		return;

  /*If (!fAuto)
  {
    if ( in_room->roomprogs && ( in_room->progtypes & ROOM_MASK )
    && !IS_SET( ch->act, PLR_WIZINVIS ) )
    rprog_leave_trigger( in_room, ch, door );
  }*/

	strcpy( buf, fAuto ? "[" HIW"¥X¤f"NOR":" : HIW"©úÅãªº¥X¤f"NOR":\n\r" );

	found = FALSE;
	for ( door = 0; door <= 5; door++ )
	{
		if ( ( pexit = ch->in_room->exit[door] )
			&& pexit->to_room
			&& !IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			found = TRUE;
			if ( fAuto )
			{
				if( !IS_SET(pexit->exit_info, EX_INVIS)){
					strcat( buf, " "HIC );
					strcat( buf, dir_name[door] );
					strcat( buf, NOR);
				}
				else if ( IS_IMMORTAL(ch)){
					strcat( buf, " "HIB );
					strcat( buf, dir_name[door] );
					strcat( buf, NOR);
				}
			}
			else
			{
				sprintf( buf + strlen( buf ), "%-5s ¡Ð %s\n\r",
				capitalize( dir_name[door] ),
				room_is_dark( pexit->to_room )
				?  "³oÃä¤Ó·t¤F"
				: pexit->to_room->name
				);
			}
		}
	}

	if ( !found )
		strcat( buf, fAuto ? " "HIC"µL"NOR : HIC"µL"NOR"¡C\n\r" );

	if ( fAuto )
		strcat( buf, "]\n\r" );

	send_to_char( buf, ch );
	return;
}

char* stat_color(int cur, int max)
{
  int ratio;  
  if( max == 0 ) return HIR;
  ratio = cur * 100 / max;
  if( ratio < 30 ) return HIR;
  else if(ratio < 60) return HIY;
  else if(ratio <= 100) return HIG;
  else return HIW;
}

char* gift_color(int cur, int ori)
{
  if( cur > ori ) return HIC;
  else if( cur < ori ) return HIR;
  else return HIW;
}

char* alignment_color(int align)
{
	// ­×§ï align color °Ï¶¡»P¦r¦êÅÜ¤Æ¬Û²Å 2023/01/16
	if( align > 799 ) return HIW;
	else if( align > 299) return HIC;
	else if( align > -300) return HIG;
	else if( align > -600) return HIY;
	else return HIR;
}


void do_elescore( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char       arg [ MAX_INPUT_LENGTH ];

  one_argument( argument, arg );

  if ( !( victim = get_char_room( ch, arg ) ) )
  {
    send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
    return;
  }
	if(!(victim->pIndexData != NULL && IsSorGateMob(victim->pIndexData->vnum))){
		send_to_char( "¥L¤£¬O¥l³êª«.\n\r", ch);
		return;
	}
	// ­×§ï¬°¥l³êª« remb ªº¨¤¦â 2022/12/20
	//if(victim->master != ch){
	if( victim->rembch != ch ){
		send_to_char( "§O·Q°½Á«§O¤H¥l³êª«ªº¸ê®Æ\n\r", ch);
		return;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	MOB_WEAKNESS *pWeak  = NULL;

	char          rgLore[MAX_STRING_LENGTH];
	char          rgBuf[MAX_STRING_LENGTH];
	char          rgSec1[200], rgSec2[200], rgSec3[200];
	char          rgSec4[200], rgSec5[200], rgSec6[500];

	rgLore[0] = 0;
  sprintf(rgBuf, "§Aªº¥l³êª«¦V§A³ø§i¥Lªºª¬ºA¡C\n\n\r"); 
	strcat(rgLore, rgBuf);

  // Mob basic Info
	sprintf(rgBuf, "¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸¡¸\n\r" );
	strcat(rgLore, rgBuf);
	sprintf(rgBuf, "%s , %s:\n\rºØ±Ú¡G%s(%s)  µ¥¯Å¡G%2d  §Þ¯àµ¥¯Å¡G%d  ©Ê§O¡G%s  °}Àç¡G%d\n\r", 
	               victim->short_descr, victim->name,
	               race_table[ victim->race ].cname, race_table[ victim->race ].name,
	               victim->level, GetGeneralMobSklv(victim) ,
	               victim->sex == SEX_MALE ? "¨k" :
	               victim->sex == SEX_FEMALE ? "¤k" : "¤¤",
								 victim->alignment);
	strcat(rgLore, rgBuf);

	// Five Basic Attributes
	sprintf(rgBuf, "¤O¶q¡G%2d/%2d  ´¼¤O¡G%2d/%2d  ´¼¼z¡G%2d/%2d   ±Ó±¶¡G%2d/%2d   Åé½è¡G%2d/%2d \n\r",
	               get_curr_str( victim ), get_max_str( victim ), 
								 get_curr_int( victim ), get_max_int( victim ),
								 get_curr_wis( victim ), get_max_wis( victim ),
								 get_curr_dex( victim ), get_max_dex( victim ),
	               get_curr_con( victim ), get_max_con( victim ));
	strcat(rgLore, rgBuf);

	// HP/MP/MV Info
	sprintf(rgBuf, "¥Í©R¤O¡G%4d/%4d   Å]ªk¤O¡G%4d/%4d  ²¾°Ê¤O¡G%4d/%4d\n\r",
								 victim->hit,  victim->max_hit,
								 victim->mana, victim->max_mana,
								 victim->move, victim->max_move);

	strcat(rgLore, rgBuf);

	// Fighting Related
	strcat(rgLore, "\n\r¾Ô°«¯à¤O¸ê®Æ¡G\n\r");
	sprintf(rgSec1, "%5d", get_hitroll(victim, WEAR_WIELD));
	sprintf(rgSec3, "%4d", get_damroll(victim, WEAR_WIELD));
	sprintf(rgBuf, "¥D­nªZ¾¹©R¤¤¡G%s   ¥D­nªZ¾¹¶Ë®`¡G%s\n\r", 
	               rgSec1, rgSec3);
	strcat(rgLore, rgBuf);

	if(get_eq_char( victim, WEAR_WIELD_2 )){
		sprintf(rgSec2, "%5d", get_hitroll(victim, WEAR_WIELD_2));
		sprintf(rgSec4, "%4d", get_damroll(victim, WEAR_WIELD_2));
 		sprintf(rgBuf, "¦¸­nªZ¾¹©R¤¤¡G%s   ¦¸­nªZ¾¹¶Ë®`¡G%s\n\r",
		             rgSec2, rgSec4);
		strcat(rgLore, rgBuf);
	}

	sprintf(rgSec4, "%5d", victim->pp);
	sprintf(rgSec5, "%4d", victim->apdr);
	sprintf(rgSec6, "%4d", victim->amdr);
	sprintf(rgBuf, "    Å]ªk¼ç¤O¡G%s   ª«²z¶Ë®`¡G%s  Å]ªk¶Ë®`¡G%s\n\r", 
	               rgSec4, rgSec5, rgSec6);
	strcat(rgLore, rgBuf);

	// Physical Resistance 
	strcat(rgLore, "\n\r¨¾¿m°ò¥»¸ê®Æ¡G\n\r");
	sprintf(rgSec4, "%4d", victim->pdr);
	sprintf(rgSec5, "%4d", victim->pad);
	
	sprintf(rgBuf, "    Å@¥Ò¨¾½Ã¡G%5d   ª«²z§Ü©Ê¡G%s  ª«²z»Ù¾À¡G%s\n\r",
			            GET_AC( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Dodge and Magical Resistance 
	sprintf(rgSec4, "%4d", victim->mdr);
	sprintf(rgSec5, "%4d", victim->mad);
	
	sprintf(rgBuf, "    °{Á×¯à¤O¡G%5d   Å]ªk§Ü©Ê¡G%s  Å]ªk»Ù¾À¡G%s\n\r",
	                GET_DODGE( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Magical Resistance in Elemental
	strcat(rgLore, "\n\r¤¸¯ÀÅ]ªk§Ü©Ê¡G\n\r");
	sprintf(rgSec6, "    §ÜÅ]ªk¡G%4d   §Üª¢¿V¡G%4d   §Üµ¾­·¡G%4d\n\r",
									GET_SVS(victim),
									NEW_GET_SVS(victim, MAGIC_FIRE),
									NEW_GET_SVS(victim, MAGIC_WIND));
	strcat(rgLore, rgSec6);

	sprintf(rgSec6, "    §Ü¹p°{¡G%4d   §Ü¦BÁ÷¡G%4d   §Ü¤j¦a¡G%4d\n\r", 
									NEW_GET_SVS(victim, MAGIC_LIGHTNING),
									NEW_GET_SVS(victim, MAGIC_COLD),
									NEW_GET_SVS(victim, MAGIC_EARTH));
	strcat(rgLore, rgSec6);

	strcat(rgLore, "\n\r¯S®íÅ]ªk§Ü©Ê¡G\n\r");
	sprintf(rgSec4, "    §ÜÀs®§¡G%4d   §Ü¯«¸t¡G%4d   §Üîî¶Â¡G%4d\n\r",
									NEW_GET_SVS(victim, MAGIC_BREATH), 
									NEW_GET_SVS(victim, MAGIC_SAINT), 
									NEW_GET_SVS(victim, MAGIC_DARK));
	strcat(rgLore, rgSec4);

	// Weakness
  strcat(rgLore, "\n\r¹ï¯S®í§Þ¯àªº§Ü©Ê±Mªø¤Î®zÂI¡G\n\r");
	for(pWeak = victim->weakness, rgBuf[0] = 0; 
			pWeak != NULL; 
			pWeak = pWeak->next){
		sprintf(rgSec6, "[%20s] §Þ¯à©R¤¤¥[¦¨ %3d%%¡A¶Ë®`¥[¦¨ %3d%%¡C\n\r",
										pWeak->name, 
										pWeak->percent - 100, 
										pWeak->damage  - 100);
		strcat(rgBuf, rgSec6);
	}
	strcat(rgLore, rgBuf);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
        
	if ( victim->affected ){

		AFFECT_DATA *paf;
		int nAffectCnt = 0;

		for ( paf = victim->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;

			if(nAffectCnt == 0) strcat( rgLore, "¥¿¨ü¨ì¥H¤U¯S®í¼vÅT¡G\n\r" );
			nAffectCnt++;

			sprintf( rgBuf, "¯S®Ä¡G¡u"HIC"%s"NOR"(%s)¡v", skill_table[paf->type].cname, skill_table[paf->type].name );
			strcat( rgLore, rgBuf );

			strcat( rgLore, "(");
			if( strcmp(c_affect_loc_name(paf->location), "µL")){
				sprintf( rgBuf, "¼vÅT"HIY"%s"NOR, c_affect_loc_name( paf->location ) );
				strcat( rgLore, rgBuf );
			}
			if( paf->modifier ){
				sprintf( rgBuf, " %s%d "NOR"ÂI  ",
				         paf->modifier<0? HIU"-":( paf->modifier == 0 ? HIW:HIC"+"),
				         paf->modifier<0?-paf->modifier:paf->modifier
			         );
				strcat( rgLore, rgBuf );
			}
			if( paf->duration >= 0 ) {
				sprintf( rgBuf, "©|¦³ "HIG"%d"NOR" ¤p®Éªº®Ä¥Î¡C", paf->duration );
				strcat( rgLore, rgBuf );
			} else {
				sprintf(rgBuf, "«ùÄò§@¥Î¤¤¡C");
				strcat(rgLore, rgBuf);
			}
			strcat(rgLore, ")");

			strcat(rgLore, "\n\r" );
		}
	}

	strcat(rgLore, "\n\r");
	send_to_char(rgLore, ch);
}

void do_score( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA *paf;
	char   buf  [ BUF_STRING_LENGTH ];
	char   buf1 [ MAX_STRING_LENGTH *8];
	char   z_buf [ MAX_INPUT_LENGTH ];
	char   key [ MAX_INPUT_LENGTH ];
	CHAR_DATA   *vch = NULL;
	CHAR_DATA   *victim = NULL;
	int race;
	int z_t=0;
	buf1[0] = '\0';
	z_buf[0]='\0';

	argument = one_argument( argument, z_buf );
	if( IS_NPC(ch) ) return;
	if( !z_buf || z_buf[0] == '\0' )
	{
		victim = ch;
		sprintf( key, "all" );
	}
	else if ( !str_cmp( z_buf, "b" ) || !str_cmp( z_buf, "f" ) || !str_cmp( z_buf, "a" ) )
	{
		victim = ch;
		sprintf( key, z_buf );
	}
	else
	{
		if( ! (victim = get_char_world(ch, z_buf ) ) )
		{
			send_to_char("³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch);
			return;
		}

		if( (!IS_SET( victim->act, PLR_HORSE )
			|| victim != ch->pcdata->horse
			|| victim->in_room != ch->in_room)
			&& ( ch->desc && ch->desc->original != victim )
			&& victim != ch
			&& !IS_IMMORTAL( ch ))
		{
			send_to_char("§A¤£¯à¬Ý§O¤Hªº¸ê®Æ¡C\n\r", ch);
			return;
		}
		one_argument( argument, key );
		if( !key || key[0] == '\0' )
			sprintf( key, "all" );
	}
/*
¢b¢b¢c¢c¢d¢d¢e¢e¢f¢f¢g¢g¢h¢h¢i¶Ç »¡ ¤¤ ªº ¤@ ­¶¢i¢h¢h¢g¢g¢f¢f¢e¢e¢d¢d¢c¢c¢b¢b
<PKer> <¥û¤â> <¤p°½> Zcecil, Raidiant Spirit  Â½Ä¶¤§¯«
©Ê§O¡G¨k©Ê    ºØ±Ú¡G¯«          PK¬ö¿ý¡G  12³Ó / 129±Ñ
µ¥¯Å¡G39      Â¾·~¡GÁåª÷®v      ¦~ÄÖ¡G 39·³¡]¯u¹ê®É¶¡ 178¤p®É¡^
¤O¶q¡G30  ´¼¤O¡G30  ´¼¼z¡G30  ±Ó±¶¡G30  Åé®æ¡G30
ª««~¡G1000/1000 ¥ó, ­«¶q¡G1000000/1000000 ¤½¤ç    ¤À­¶¡G20 ¦æ
¥Í©R¤O¡G99999/99999 ÂI, Å]¤O¡G99999/99999 ÂI, Åé¤O¡G99999/99999 ÂI
Wimpy ¡G99999 ÂI,       ´_¬¡ÂI¼Æ¡G 50 ÂI
ª÷¿ú  ¡G2147283647 ¤¸,  ¦s´Ú¡G2147483647 ¤¸
Autogold: ¬O    Autoloot: ¬O      Autosac: ¬O    Autoexit: ¬O
Wizinvis: §_    Holylight: ¬O
°}  Àç¡G990, §A¦³¦p¤Ñ¨Ï¯ëµ½¨}.
¸gÅç­È¡G12345678 / MAX       ½m²ßÂI¼Æ¡G1234 ÂI
©R¤¤²v¡G1222+22 ÂI       ¶Ë®`¤O¡G1231+21 ÂI      §ÜÅ]¤O¡G-123 ÂI
«i  ®ð¡G999/299 ÂI, ¼vÅT©R¤¤²v 10 ÂI, ¶Ë®`¤O 10 ÂI.
¨¾¿m¤O¡G-9999 ÂI, §AªºªZ¸Ë¯u¤£¥i«äÄ³¡I
§A¥¿¯¸µÛ.§A²{¦b¤£¾A¦X§@®{¤â¾Ô°«.
§A¥¿³Q«°¥«¦u½Ã(Cityguard)Ây±þ.
§A¥¿Ây±þµÛ¥«ªø(Mayor).
§A¨­¤W¥¿§@¥ÎµÛ¥H¤Uªk³N¡G
§Þ¯à¡G¥þ¤O¨¾¿m(defence), ¼vÅT©R¤¤²v -3 ÂI, ÁÙ¦³ 1111 ¤p®Éªº®Ä¥Î.
¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹
                    IS_SET( wch->act, PLR_KILLER   ) ? "¡]"RED"¥û¤â"NOR"¡^" : "",
                            IS_SET( wch->act, PLR_THIEF    ) ? "¡]"RED"¤p°½"NOR") "  : "",
                              IS_SET( wch->act, PLR_REGISTER ) ? "¡]"HIG"PK"NOR"¡^"    
*/
  /*
<PK>Reticent, ª±®a§O¥s§ÚÀ°¦£®³¸Ë³Æ QQ  Reticent(Reticent)
§A¬O¤@­Ó 20 ·³ªº 42 ¯Å¨k©Ê¤HÃþ±Ú¨g¾Ô¤h¶Ç©_¡C(¤W½u®É¶¡ 50 ¤p®É)
§AªºÀ°¬£¬O: ¥ú©ú¸t¨Ï¹Î¡C        «Â±æ:10, §A¶}©lºCºC¹ñÅSÀY¨¤¡C
¤O¶q: 17/16  ´¼¤O: 14/17  ´¼¼z: 16/16  ±Ó±¶: 15/17  Åé½è: 17/22
¥Í©R¤O: 589/1856      Å]ªk¤O: 200/1000      ²¾°Ê¤O: 1061/1061
PK ¬ö¿ý:  0 ³Ó/ 0 ±Ñ    ¸gÅç­È¡G6311435/MAX        ½m²ßÂI¼Æ: 351
°}   Àç: -1000, §A¬°²³¤H©Ò¶A©G¡AÂ²ª½´N¬O¼»¥¹ªº¨«ª¯¡C
                                                                                
§A¥Ø«eªº§@¾Ô«¬ºA¬°: °±¤î§ðÀ»¡C    ¥Í©R¤O¤Ö©ó   0 ÂI®É¦Û°Ê°k¶]¡C
°}«¬: µL      ²{¦bª¬ºA: ¤f´÷¡B¨{¤l¾j¤F¡B¥ð®§¡B®{¤â·i°«
ª««~: 56/72   ­t­«: 414/500 ¤½¤ç  ª÷¿ú: 1000000 ¤¸  ¦s´Ú: 1000000 ¤¸
¤À­¶: 20 ¦æ   Autoexit: ¬O   Autoloot: §_   Autogold: ¬O Autosac: §_
                                                                                
¥D­nªZ¾¹©R¤¤ÂI¼Æ: 52   °{  Á×  ²v:   14, §A¤ñ½½¤û§Ö¤£¤F¦h¤Ö¡C
¥D­nªZ¾¹§ðÀ»ÂI¼Æ: 69   Å@¥Ò¨¾½Ã¤O:  -96, §A¸Ë³Æ°_¨Ó¡C
¦¸­nªZ¾¹©R¤¤ÂI¼Æ: µL   ÁÅ  §K  ²v: ªk³N¡G-14  ­·¨t¡G-11  ¤õ¨t¡G-9
¦¸­nªZ¾¹§ðÀ»ÂI¼Æ: µL               ¤ô¨t¡G-9   ¦a¨t¡G-9   ¦R®§¡G-9
Å]  ªk    ¼ç  ¤O:  0

§A¬O¤@­Ó 120 ·³ªº 43 ¯Å¤¤©Ê¯«±Ú§Å®v¡C(¤W½u®É¶¡ 1163 ¤p®É)¡A³Q¤j¯««H¥ô¬°²Ä 46¯Å¡C
§AªºÀ°¬£¬O¡G¤Z¯S¿ü¾Ç°|¡C      «Â±æ¡G4, §A¬O­ÓµL¦W¤p¨ò¡C
¤O¶q: 25/25  ´¼¤O: 21/21  ´¼¼z: 21/21  ±Ó±¶: 24/24  Åé½è: 21/21
¥Í©R¤O: 1515/1515  Å]ªk¤O: 2025/2025  ²¾°Ê¤O: 1624/1624
¸gÅç­È¡G1244880/MAX ½m²ßÂI¼Æ: 424
§Þ¯à¸gÅç­È¡G145499/145500/1400000   
°}Àç¡G60, §AÂÔ¦u¤¤¥ß¥»¤À¡C

§A¦b¥ð®§¡C
§A¥Ø«eªº§@¾Ô«¬ºA¬°¨³±¶¬Iªk.     ¥Í©R¤O¤Ö©ó 0 ÂI®É¦Û°Ê°k¶]
²{¦bª¬ºA¡G
ª««~: 60/1000  ­t­« 83/1000000 ¤½¤ç  ª÷¿ú¡G207457814 ¤¸  ¦s´Ú¡G106011205 ¤¸
¤À­¶: 23 ¦æ¡C Autoexit: ¬O  Autoloot: §_  Autogold: ¬O  Autosac: §_
Wizinvis: §_(0)  Holylight: ¬O  
¥D­nªZ¾¹©R¤¤ÂI¼Æ¡G  44¡@°{  Á×  ²v¡G-1531,¡@§AºC¤@ÂI! §OÅý§Aªº¼v¤l¸ò¥á¤F¡I¡I¡I
¥D­nªZ¾¹§ðÀ»ÂI¼Æ¡G  44  Å@¥Ò¨¾½Ã¤O¡G -274,¡@§A¬ïµÛ­««¬¸Ë³Æ¡C
¦¸­nªZ¾¹©R¤¤ÂI¼Æ¡G¡@µL¡@ªk³N¡G-441¡@­·¨t¡G-227¡@¤õ¨t¡G-230
¦¸­nªZ¾¹§ðÀ»ÂI¼Æ¡G¡@µL¡@¤ô¨t¡G-128¡@¦a¨t¡G-131¡@¦R®§¡G -30
Å]  ªk    ¼ç  ¤O¡G1015¡@¹p¨t¡G -22¡@
[¿é¤J«ü¥O (c)Ä~Äò, (r)­«¦L¥»­¶, (b)¤W­¶, (h)»¡©ú, (q)Â÷¶}, ©Î«ö ENTER]:  
ª«¡@²z ¶Ë ®`¡@¤O¡G   5¡@§Ü¡@ª« ²z ¶Ë¡@®`¡G1969
Å]¡@ªk ¶Ë ®`¡@¤O¡G   0¡@§Ü¡@Å] ªk ¶Ë¡@®`¡G1985

*/


	race = ( ( victim->temp_race ) ? victim->temp_race - 1 : victim->race );
	if ( IS_NPC( victim ) )
		return;

	if ( !str_cmp( key, "all" ) || !str_cmp( key, "b" ) )
	{
		sprintf( buf, "\n\r"HIW"¢b¢b¢c¢c¢d¢d¢e¢e¢f¢f¢g¢g¢h¢h¢i "HIC"¶Ç "HIP"»¡ "HIR"¤¤ "HIY"ªº "HIU"¤@ "HIG"­¶"HIW" ¢i¢h¢h¢g¢g¢f¢f¢e¢e¢d¢d¢c¢c¢b¢b"NOR"\n\r");
	//¨¤¦â¦WºÙ
		strcat( buf1, buf );
		sprintf( buf, HIW"%s%s%s"HIY"%s"NOR", "NOR"%s"NOR" %s\n\r",
			IS_SET( victim->act, PLR_KILLER ) ? HIR"<¥û¤â>"NOR : "",
			IS_SET( victim->act, PLR_THIEF ) ? HIP"<¤p°½>"NOR : "",
			IS_SET( victim->act, PLR_REGISTER ) ? HIG"<PK>"NOR : "",
			victim->name, IS_NPC( victim ) ? "" : 
			victim->short_descr, victim->pcdata->title);
			strcat( buf1, buf );

	//«H¥ôµ¥¯Å , ²¾¨ì¨¤¦â¦W¤U¤è 2022/12/22
		if ( get_trust( victim ) != victim->level )
		{
			sprintf( buf, "¤j¯««H¥ô¬°²Ä "HIC"%d"NOR" ¯Å¡C\n\r", get_trust( victim ) );
			strcat( buf1, buf );
		}else
			strcat(buf1, "\n\r");

		sprintf( buf, "§A¬O¤@­Ó "HIC"%d"NOR" ·³ªº "HIY"%d"NOR" ¯Å"CYN"%s"NOR"©Ê"HIW"%s%s"NOR"(%s %s) - (¤W½u"HIC" %d "NOR"¤p®É)\n\r", 
		get_age( victim ), victim->level,
		(victim->sex == SEX_MALE) ? "¨k" : (victim->sex == SEX_FEMALE) ? "¤k" : "¤¤",
		race_table[ race ].cname,
		class_table[ victim->class ].cname,
		race_table[ race ].name,
		class_table[ victim->class ].name,
		//( victim->level == 41 ? "­^¶¯" : victim->level == 42 ? "¶Ç©_" : ""),
		( victim->played / 3600 ));

		strcat( buf1, buf );

	//À°¬£
		if ( victim->clan )
		{
			sprintf( buf, "§AªºÀ°¬£¬O¡G%s¡C", victim->clan->name );
			strcat( buf1, buf );
		}
		else 
		{
			sprintf( buf, "§A²{¦b¨S¦³¥[¤JÀ°¬£¡C");
			strcat( buf1, buf );
		}
	//«Â±æ
		if( victim->level >= LEVEL_HERO )
		{
			sprintf(buf,"      «Â±æ¡G"HIW"%d"NOR", ", victim->pcdata->respect);
			strcat( buf1, buf );
		}
		//strcat(buf1, "§A");
		if ( victim->pcdata->respect <  0 ) strcat( buf1, HIC"§A´Nºâ¦º¦b¸ôÃä¤]¨S¦³¤H·|µo²{¡C"NOR"\n\r" );
		else if ( victim->pcdata->respect < 20 ) strcat( buf1,HIW"§A¬O­ÓµL¦W¤p¨ò¡C"NOR"\n\r" );
		else if ( victim->pcdata->respect < 40 ) strcat( buf1,HIG "§AºCºC¶}©l¹ñÅSÀY¨¤¡C"NOR"\n\r"    );
		else if ( victim->pcdata->respect < 60 ) strcat( buf1, "§A¬O¸¹¤Hª«¡C\n\r"    );
		else if ( victim->pcdata->respect < 80 ) strcat( buf1, "§A«Â¾_¤@¤è¡C\n\r" );
		else if ( victim->pcdata->respect < 100 ) strcat( buf1, "§Aªº¨Æ¸ñ³Q§u¹C¸Ö¤H¦b°s§a¶Ç»w¡C\n\r"    );
		else if ( victim->pcdata->respect < 150 ) strcat( buf1,HIP "§AªºÁn¦WµL¤H¤£ª¾¡AµL¤H¤£¾å¡C"NOR"\n\r"    );
		else if ( victim->pcdata->respect < 300 ) strcat( buf1,HIR "§A¬O­Ó¤w³Q¯«¤Æªº¶Ç©_¤Hª«¡C"NOR"\n\r" );
		else strcat( buf1, RED"§A¬O¦Û¤Ñ¯«¥H­°ªº²Ä¤@¤H¡C"NOR"\n\r" );

	//¥\¾±
		sprintf(buf, "§Aªº¥\\¾±¡G"HIG"%d"NOR"  ", victim->pcdata->merit);
		strcat(buf1, buf);
		if ( victim->pcdata->merit <=  0 ) strcat( buf1, HIC"§AÁÙ¨S«Ø¥ß¥ô¦ó¾Ô¥\\¡C"NOR"\n\r" );
		else if ( victim->pcdata->merit < 20000 ) strcat( buf1,"§A¶}©l«Ø¥ß¾Ô¥\\¡C"NOR"\n\r"    );
		else if ( victim->pcdata->merit < 40000 ) strcat( buf1, "§A¬O¸¹¤Hª«¡C\n\r"    );
		else if ( victim->pcdata->merit < 80000 ) strcat( buf1, "§Aªº¾ÔªGÂ×ºÓ¡C\n\r" );
		else if ( victim->pcdata->merit < 100000 ) strcat( buf1, HIY "§Aªº¾Ô¥\\³Q§u¹C¸Ö¤H¦b°s§a¶Ç»w¡C"NOR"\n\r" );
		else if ( victim->pcdata->merit < 200000 ) strcat( buf1,HIP "§Aªº¾Ô¥\\µL¤H¤£ª¾¡AµL¤H¤£¾å¡C"NOR"\n\r"    );
		else if ( victim->pcdata->merit < 500000 ) strcat( buf1,HIR "§Aªº¨ÆÂÝ¦³¦p¾Ô¯«²Ä¤G¡C"NOR"\n\r" );
		else strcat( buf1, HIW "§A¬O¦Û¤Ñ¯«¥H­°ªº²Ä¤@¤H¡C"NOR"\n\r" );

	//ÄÝ©Ê , ¨¤¦âÅé«¬ 2022/12/23
		sprintf( buf,
			"¤O¶q: %s%d"HIW"/%d%s  ´¼¤O: %s%d"HIW"/%d%s  ´¼¼z: %s%d"HIW"/%d%s  ±Ó±¶: %s%d"HIW"/%d%s  Åé½è: %s%d"HIW"/%d%s  Åé«¬: "HIY"%d"NOR"\n\r",
			gift_color( get_eff_str(victim), get_max_str(victim)),
			get_curr_str( victim ),
			get_max_str( victim ),
			NOR,
			gift_color( get_eff_int(victim), get_max_int(victim)),
			get_curr_int( victim ),
			get_max_int( victim ),
			NOR,
			gift_color( get_eff_wis(victim), get_max_wis(victim)),
			get_curr_wis( victim ),
			get_max_wis( victim ),
			NOR,
			gift_color( get_eff_dex(victim), get_max_dex(victim)),
			get_curr_dex( victim ),
			get_max_dex( victim ),
			NOR,
			gift_color( get_eff_con(victim), get_max_con(victim)),
			get_curr_con( victim ),
			get_max_con( victim ),
			NOR,
			get_curr_size( victim ) // ¥[¤JÅé«¬ 2022/12/22
			);
		strcat( buf1, buf );

	//hp mana move
			/*sprintf( buf,
			"§A¦³[1m%d[m/[1m%d[mÂI¥Í©R¤O¡B[1m%d[m/[1m%d[mÂIÅ]ªk¤O¡B[1m%d[m/[1m%d[mÂI²¾°Ê¤O¡B[1m%d[mÂI½m²ßÂI¼Æ¡C\n\r",
				  ch->hit,  ch->max_hit,
				  ch->mana, ch->max_mana,
				  ch->move, ch->max_move,
				  ch->practice );*/
		sprintf( buf, "¥Í©R¤O: %s%d%s/"HIG"%d"NOR"    Å]ªk¤O: %s%d%s/"HIG"%d"NOR"    ²¾°Ê¤O: %s%d%s/"HIG"%d"NOR"\n\r",
				stat_color(victim->hit, victim->max_hit),
				victim->hit,
				NOR,
				victim->max_hit,
				stat_color(victim->mana, victim->max_mana),
				victim->mana,
				NOR,
				victim->max_mana,
				stat_color(victim->move, victim->max_move),
				victim->move,
				NOR,
				victim->max_move
		);
		strcat( buf1, buf );



		/*sprintf( buf,
		  "§A±aµÛ¦³[1m%d[m/[1m%d[m¶µª««~¡A¦@­«[1m%d[m/[1m%d[m¤½¤ç¡C\n\r",
		  ch->carry_number, can_carry_n( ch ),
		  ch->carry_weight, can_carry_w( ch ) ); */
		if( victim->level >= (LEVEL_IMMORTAL -1 ))
		{
			sprintf( z_buf, "%s", "MAX");
		}
		else
		{
			//20060722 modify by jye
			//·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ
			z_t = EXP_PER_LEVEL( victim->level , victim->class) * (victim->level + 1 );
			sprintf(z_buf, "%d",z_t); 
		}
		sprintf(buf, "¸gÅç­È¡G%s%d"NOR"/"HIG"%s"NOR"   ½m²ßÂI¼Æ: "HIG"%d"NOR"",
				(victim->exp >= z_t) ? CYN : HIC,
				victim->exp,
				z_buf,
				victim->practice
		);
		strcat(buf1,buf);

	//pk¬ö¿ý
		if( !IS_NPC( victim ) && IS_SET(victim->act, PLR_REGISTER)){
			sprintf(buf, "   PK ¬ö¿ý¡G "HIC" %d ³Ó/"HIR" %d ±Ñ"NOR"\n\r",
					victim->pcdata->pkwin,
					victim->pcdata->pklose);
			strcat(buf1, buf);
		}
		else
			strcat(buf1, "\n\r");

	//§Þ¯à¸gÅç
		if( victim->class >= WIZARD ) {
			sprintf(buf,"§Þ¯à¸gÅç­È¡G"HIC"%d"NOR"/"HIY"%d"NOR"/"HIW"%d"NOR"  (¥\\¾±´£¤É§A"HIG" %d "NOR"¤W­­ÂI)\n\r",
					get_eff_skill_exp(victim),
					get_skill_exp(victim),
					get_max_skill_exp(victim),
					get_merit_bonus_max_skexp(victim));
			strcat(buf1, buf);
		}
	//°}Àç
		if ( !IS_NPC(victim) && victim->level >= 10 )
		{
			sprintf( buf, "°}Àç¡G%s%d"NOR"  ", alignment_color(victim->alignment), victim->alignment );
			strcat( buf1, buf );
		}

		//strcat(buf1,"§A");
		// ­×§ï°Ï¶¡¼Æ­È»P¦â½X, ¼W¥[±q¤¤¥ßÂà´««e 20 align ÅÜ¦â 2023/01/16
		if ( victim->alignment      >  799 ) strcat( buf1,HIC "§A¦³¦p¤Ñ¨Ï¯ëµ½¨}¡C"NOR"                   \n\r" );
		else if ( victim->alignment >  599 ) strcat( buf1,HIW "§Aªº«~¼w¦³¦p¸t¤H¯ë¤½¥¿¡C"NOR"             \n\r" );
		else if ( victim->alignment >  315 ) strcat( buf1,HIG "§A©^¤½¦uªk¡A¦æ¨Æ¥¿¸q¡C"NOR"               \n\r" );
		else if ( victim->alignment >  284 ) strcat( buf1,CYN "§A¹ï¤HÁÙºâ¤¯·O¡C"NOR"                     \n\r" );
		else if ( victim->alignment >  149 ) strcat( buf1,    "§A¹ï¤HÁÙºâ¤¯·O¡C                          \n\r" );
		else if ( victim->alignment > -150 ) strcat( buf1,    "§AÂÔ¦u¤¤¥ß¥»¤À¡C                          \n\r" );
		else if ( victim->alignment > -285 ) strcat( buf1,    "§A¦³¤@ÂI¨õ»À¡C                            \n\r" );
		else if ( victim->alignment > -300 ) strcat( buf1,PUR "§A¦³¤@ÂI¨õ»À¡C"NOR"                       \n\r" );
		else if ( victim->alignment > -316 ) strcat( buf1,PUR "§A¥ØµLªk¬ö¡A¦æ¬°¨¸´c¡C"NOR"               \n\r" );
		else if ( victim->alignment > -600 ) strcat( buf1,HIP "§A¥ØµLªk¬ö¡A¦æ¬°¨¸´c¡C"NOR"               \n\r" );
		else if ( victim->alignment > -800 ) strcat( buf1,HIR "§A¬O­Ó¤Q´c¤£³jªº´cÅ]¡C"NOR"               \n\r" );
		else                                 strcat( buf1,RED "§A¬°²³¤H©Ò¶A©G¡AÂ²ª½´N¬O¼»¥¹ªº¨«ª¯¡C"NOR" \n\r" );
		strcat( buf1, "\n\r");
	//²{¦bªº¦æ¬°
		switch( victim->position )
		{
			case POS_DEAD:
				strcat( buf1, "§A"RED"¦º"NOR"¤F¡I¡I");
				break;
			case POS_MORTAL:
				strcat( buf1, "§A"HIR"¨ü¤F­P©R"NOR"¶Ë¡C");
				break;
			case POS_INCAP:
				strcat( buf1, "§A"HIB"¥¢¥h¦æ°Ê¯à¤O"NOR"¤F¡C");
				break;
			case POS_STUNNED:
				strcat( buf1, "§A"HIB"¤£¬Ù¤H¨Æ"NOR"¤F¡C");
				break;
			case POS_SLEEPING:
				strcat( buf1, "§A"HIU"ºÎµÛ"NOR"¤F¡C");
				break;
			case POS_RESTING:
				strcat( buf1, "§A¦b"HIG"¥ð®§"NOR"¡C");
				break;
			case POS_STANDING:
				if( is_flying( victim ) ) // ¥[¤J aff_flying ªº§P©w 2021/11/05
					strcat( buf1, "§Aº}¯B¦b³o¸Ì¡C");
				else
					strcat( buf1, "§A¯¸¥ß¦b³o¸Ì¡C");
				break;
			case POS_FIGHTING:
				if( is_flying( victim ) ) // ¥[¤J aff_flying ªº§P©w 2021/11/05
					strcat( buf1, "§Aº}¯B¦b³o¸Ì"HIY"¾Ô°«"NOR"µÛ¡C");
				else
					strcat( buf1, "§A¯¸¦b³o¸Ì"HIY"¾Ô°«"NOR"µÛ¡C");
				break;
			default:
				break;
		}
	// ·s¼W link char Åã¥Ü 2021/11/20 
		if ( ( vch = victim->linking) )
		{
			sprintf(buf, HIU "§AªºÆF»î»P"NOR"%s"HIU"²£¥Í¦@»ï"NOR".\n\r", vch->short_descr );
			strcat(buf1, buf);
		}else{
			strcat( buf1, "\n\r");
		}
	//§@¾Ô«¬ºA
		if( !IS_NPC( victim ) /*&& victim->pcdata->autocombat != AUTOC_NORMAL*/)
		{
			strcat(buf1, "¥Ø«eªº§@¾Ô«¬ºA");
			switch( victim->pcdata->autocombat ) 
			{
				case AUTOC_NORMAL:
					strcat(buf1, "  ´¶  ³q.");
					break;
				case AUTOC_STOP:
					strcat(buf1, HIR"°±¤î§ðÀ»."NOR);
					break;
				case AUTOC_DODGE:
					strcat(buf1, HIC"¥þ¤O°{¸ú."NOR);
					break;
				case AUTOC_PARRY:
					strcat(buf1, HIG"¥þ¤O¦u¿m."NOR);
					break;
				case AUTOC_CONCENTRATION:
					strcat(buf1, HIY"±Mª`¬èÃ«."NOR);
					break;
				case AUTOC_CRITICAL:
					strcat(buf1, HIP"­P©R§ðÀ»."NOR);
					break;
				case AUTOC_COOPERATE:
					strcat(buf1, HIW"¸sÅé§@¾Ô."NOR);
					break;
				case AUTOC_POWER:
					strcat(buf1, HIC"¤ÞµoÅ]¤O."NOR);
					break;
				case AUTOC_INSTANT:
					strcat(buf1, HIY"¨³±¶¬Iªk."NOR);
					break;
				case AUTOC_CHARGE:
					strcat(buf1, HIY"½Ä¨ë¬ðÀ»."NOR);
					break;
				case AUTOC_RELOAD:  // ¥Îºj±ñ¾Ô°«®É ¦Û°Ê¤W¼u 2022/05/01
					strcat(buf1, HIB"§Ö³t¶ñ¼u."NOR);
					break;
				default:
					strcat(buf1, HIU"  ¤£  ©ú."NOR);
				break;
			}
		}


		//sprintf( buf, "¦b¥Í©R­È³Ñ¤U[1m%d[m¥H¤Uªº®É­Ô·|¦Û°Ê°k¶]¡C\n\r", ch->wimpy );
		/*sprintf( buf,
		  "§A¦³[1m%d[mÂI¸gÅç­È¡A¦³[1m%d[mªTª÷¹ô¡C\n\r",
		  ch->exp,  ch->gold );
		  */
	//¦Û°Ê°k¶]
		sprintf(buf, "            ¥Í©R¤O¤Ö©ó "CYN"%d"NOR" ÂI®É¦Û°Ê°k¶]\n\r", victim->wimpy);
		strcat( buf1, buf );

	//²{¦bª¬ºA
		// ­pºâª¬ºAÅã¥Üªº­Ó¼Æ, ¤Ó¦h«h´«¦æ 2021/11/05
		int affect_count = 0;;
		strcat( buf1, "²{¦bª¬ºA¡G");
		// ±Nfist ¤ÎÂÃ°Î²¾¦Ü³Ì«e¤è 2021/11/05
		if ( IS_SET( victim->act, PLR_FIST_FIGHT ) )
		{
			sprintf( buf, "®{¤â·i°« " );
			strcat( buf1, buf );
			affect_count += 2;
		}
		if ( IS_AFFECTED( victim, AFF_HIDE ) )
		{
			sprintf( buf, HIB"ÂÃ°Î"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
		{
			sprintf( buf, YEL"Áô§Î"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		// ­×§ï Cond ªº³Ì¤j¼Æ­È ±q 48 ÅÜ¬° 100 , ÅÜ§ó±Ô­zªº¼Æ­È°Ï¶¡¤Î¼W¥[±Ô­z 2023/01/13
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] >= 70)
		{
			strcat( buf1, HIR"°sºë¤¤¬r"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] < 70
			&& victim->pcdata->condition[COND_DRUNK] >= 50 /*36*/ )
		{
			strcat( buf1, HIR"Äê¾K"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] < 50 /*50*/
			&& victim->pcdata->condition[COND_DRUNK] >= 20 /*10*/)
		{
			strcat( buf1, HIR"³Ü¾K"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] < 20/*10*/
			&& victim->pcdata->condition[COND_DRUNK] > 0 ){
			strcat( buf1, RED"·LÅH"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_THIRST] < 40 /*15*/
			&& victim->pcdata->condition[COND_THIRST] > 10 /*0*/)
		{
			strcat( buf1, HIY"¤f´÷"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_FULL] < 40 /*15*/
			&& victim->pcdata->condition[COND_FULL] > 10 /*0*/)
		{
			strcat( buf1, HIP"¨{¤l¾j¤F"NOR" "  );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_THIRST] <= 10
			&& victim->pcdata->condition[COND_THIRST] >= 0 /* == 0*/ )
		{
			strcat( buf1, YEL"²æ¤ô"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_FULL] <= 10
			&& victim->pcdata->condition[COND_FULL] >= 0 /* == 0*/ )
		{
			strcat( buf1, PUR"°§¾j"NOR" "  );
			affect_count += 1;
		}
		// ·s¼W ¤¤¬r , ¥Í¯f , ®£Äß , ºÆ¨g , ³Â·ôµ¥­t­±ª¬ºA 2021/11/05
		if ( IS_AFFECTED( victim, AFF_POISON ) )
		{
			sprintf( buf, HIG"¤¤¬r"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_DISEASE ) )
		{
			sprintf( buf, HIU"¥Í¯f"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_PARALYZED ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, YEL"³Â·ô"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_FEAR ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, CYN"®£Äß"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_CRAZY ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, PUR"ºÆ¨g"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_CURSE ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, HIB"¶A©G"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_MUTE ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, HIY"¨IÀq"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_SEAL_MAGIC ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, HIC"«Ê©G"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_SET( victim->act, PLR_RIDE ) )
		{
			sprintf( buf, "®yÃM " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		strcat( buf1, "\n\r" );
		sprintf( buf, "ª««~ "CYN"%d"NOR"/"HIC"%d"NOR"  ­t­« "CYN"%d"NOR"/"HIC"%d"NOR" ¤½¤ç    ",

		  victim->carry_number, can_carry_n( victim ),
		  victim->carry_weight, can_carry_w( victim ) ); 
		strcat( buf1, buf );

		sprintf( buf, "ª÷¿ú¡G"HIY"%d"NOR" ¤¸  ¦s´Ú¡G"HIY"%d"NOR" ¤¸\n\r",
			victim->gold,
			victim->bank
			);
		strcat( buf1, buf );

	//¤À­¶ pagelength
		if ( !IS_NPC( victim ) )
			sprintf( buf, "¤À­¶: "HIW"%d"NOR" ¦æ¡C  ", victim->pcdata->pagelen );
		else
			sprintf(buf, "¡C\n\r");
		strcat( buf1, buf );
		buf[0] = '\0';

	//¦Û°Ê¦æ¬°
		sprintf( buf, "Autoexit: %s  Autoloot: %s  Autogold: %s  Autosac: %s\n\r",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOEXIT ) ) ? HIW"¬O"NOR : "§_",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOLOOT ) ) ? HIW"¬O"NOR : "§_",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOGOLD ) ) ? HIW"¬O"NOR : "§_",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOSAC  ) ) ? HIW"¬O"NOR : "§_"
				);
		strcat( buf1, buf );
	//¯«Áô
		if( !IS_NPC(victim) && victim->level >= L_APP ) 
		{
			QMARK *qm = is_qmark(victim, "wizinvis");
			sprintf( buf, "Wizinvis: %s(%d)  Holylight: %s  \n\r",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS  ) ) ? HIW"¬O"NOR : "§_",
				( qm ? qm->v0 : 0 ),
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_HOLYLIGHT ) ) ? HIW"¬O"NOR : "§_" );
			strcat( buf1, buf );
		}
		else
			strcat( buf1, "\n\r");
	}

	// ¾Ô°«¼Æ­È , score f
	if ( !str_cmp( key, "all" ) || !str_cmp( key, "f" ) )
	{
		if ( !str_cmp( key, "f" ) )
		{
			strcat( buf1, "§Aªº¾Ô°«¼Æ­È¦p¤U¡G\n\r" );
		}

		if ( ch->level >= 15 )
		{
			// wp hr
			strcat ( buf1, "¥D­nªZ¾¹" );
			sprintf( buf, "©R¤¤ÂI¼Æ¡G"HIG"%4d"NOR"¡@",get_hitroll( victim, ( get_eq_char(victim, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD)));
			strcat( buf1, buf );

			// dc
			strcat( buf1, "°{  Á×  ²v¡G" );
			if ( ch->level >= LEVEL_HERO )
			{
				sprintf( buf, HIW"%5d"NOR",¡@§A", GET_DODGE( victim ) );
				strcat( buf1, buf );
				buf[0] = '\0';
			}
			else
				strcat(buf1,"§A");
			if ( GET_DODGE( victim )      >=  200  ) strcat( buf1, "®Ú¥»´N¬O¤@¶ô©¥¥Û¡C" );
			else if ( GET_DODGE( victim ) >=  100  ) strcat( buf1, "ÁÙ¯à°Ê¯u¬O©_ÂÝ¡I" );
			else if ( GET_DODGE( victim ) >=   0   ) strcat( buf1, "½wºC¦Óµh­W¦aª¦¦æµÛ¡C" );
			else if ( GET_DODGE( victim ) >= -50   ) strcat( buf1, "¤ñ½½¤û§Ö¤£¤F¦h¤Ö¡C" );
			else if ( GET_DODGE( victim ) >= -150  ) strcat( buf1, "²×©ó¾Ç·|¨«¸ô¡C" );
			else if ( GET_DODGE( victim ) >= -200  ) strcat( buf1, "©ïÀY®¼¯Ý¡A¾¹¦t°a©ù¡C" );
			else if ( GET_DODGE( victim ) >= -400  ) strcat( buf1, "ªº³t«×¤£¿ù¡C" );
			else if ( GET_DODGE( victim ) >= -500  ) strcat( buf1, "¦æ°Ê«D±`±Ó±¶¡C" );
			else if ( GET_DODGE( victim ) >= -750  ) strcat( buf1, "°Ê¦p²æ¨ß¡A¤@¬ª¤d¨½¡C" );
			else if ( GET_DODGE( victim ) >= -900  ) strcat( buf1, "¤@©w¬O¥@¬ÉÁÉ¶]«a­x¡C" );
			else if ( GET_DODGE( victim ) >= -1000 ) strcat( buf1, "³t«×§Ö±o¨S¤H¬Ý±o¨ì¡C");
			else if ( GET_DODGE( victim ) >= -1200 ) strcat( buf1, "®Ú¥»´N¹³¤@°}±Û­·¡C");
			else if ( GET_DODGE( victim ) >= -1300 ) strcat( buf1, "§Ö¨ì¥i¥H°l¤W¦Û¤vªº¼v¤l¡I");
			else                                     strcat( buf1, "ºC¤@ÂI! §OÅý§Aªº¼v¤l¸ò¥á¤F¡I¡I¡I");
			strcat( buf1, NOR"\n\r" );

			// wp dr
			sprintf( buf, "¥D­nªZ¾¹§ðÀ»ÂI¼Æ¡G"HIG"%4d"NOR"  ",get_damroll( victim, ( get_eq_char(victim, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD)));
			strcat( buf1, buf );

			// ac
			strcat( buf1, "Å@¥Ò¨¾½Ã¤O¡G" );
			if ( ch->level > 25 )
			{
				sprintf( buf, HIW"%5d"NOR",¡@§A", GET_AC( victim ) );
				strcat( buf1, buf );  
				buf[0] = '\0';
			}
			else
				strcat(buf1,"§A");
			if ( GET_AC( victim )      >=  200  ) strcat( buf1, "¤ñ»rÅéªºÁÙ¸Ø±i¡I" );
			else if ( GET_AC( victim ) >=  100  ) strcat( buf1, "¸òÞóÅé¨S¤°»ò¨â¼Ë¡C" );
			else if ( GET_AC( victim ) >=   0   ) strcat( buf1, "¥u¬ïµÛ¤º¦ç¤º¿Ç¡C" );
			else if ( GET_AC( victim ) >= -50   ) strcat( buf1, "¬ïµÛ´¶³qªº¦çªA¡C" );
			else if ( GET_AC( victim ) >= -200  ) strcat( buf1, "¦h¤Ö¦³ÂI¸Ë³Æ¡C" );
			else if ( GET_AC( victim ) >= -400  ) strcat( buf1, "¸Ë³Æ°_¨Ó¡C" );
			else if ( GET_AC( victim ) >= -600  ) strcat( buf1, "ªº¸Ë³Æ¤£¿ù¡C" );
			else if ( GET_AC( victim ) >= -800  ) strcat( buf1, "ªº¸Ë³Æ«Ü¦n¡C" );
			else if ( GET_AC( victim ) >= -1000 ) strcat( buf1, "¬ïµÛ­««¬¸Ë³Æ¡C" );
			else if ( GET_AC( victim ) >= -1100 ) strcat( buf1, "¬ïµÛ¶W¯Åªº¸Ë³Æ¡C" );
			else if ( GET_AC( victim ) >= -1200 ) strcat( buf1, "ªº¸Ë³Æ¥uÀ³¤Ñ¤W¦³¡C");
			else if ( GET_AC( victim ) >= -1400 ) strcat( buf1, "³o¼Ëªº¸Ë³Æ¤Ñ¤UµL¼Ä¡C");
			else if ( GET_AC( victim ) >= -1600 ) strcat( buf1, "¬ïµÛ¶W¯ÅµL¼Ä¶Àª÷ÅRÆEªº¸Ë³Æ¡C");
			else                                  strcat( buf1, "ªº¸Ë³Æ¤Ó¸Ø±i¤F¡A¬O¥Î bug ¶Ü¡H");
			strcat( buf1, NOR"\n\r" );

			if ( get_eq_char( victim, WEAR_WIELD_2 ) )
			{
			// wp2 hr
				sprintf( buf, "¦¸­nªZ¾¹©R¤¤ÂI¼Æ¡G"HIG"%4d"NOR"¡@",  get_hitroll( victim, WEAR_WIELD_2 ));
				strcat( buf1, buf );

			// svs , wind , fire , poison
				if ( ch->level > 36)
				{
					sprintf( buf, "ªk³N¡G"HIW"%4d"NOR"¡@­·¨t¡G"HIW"%4d"NOR"¡@¤õ¨t¡G"HIW"%4d"NOR"¡@¬r¯À¡G"HIW"%4d"NOR"\n\r",
					NEW_GET_SVS(victim, MAGIC_SPELL),
					NEW_GET_SVS(victim, MAGIC_WIND),
					NEW_GET_SVS(victim, MAGIC_FIRE),
					NEW_GET_SVS(victim,MAGIC_POISON)); // ¼W¥[¤¤¬r¨Ctick¨ü¨ì¶Ë®`Åã¥Ü 2021/11/03
					strcat( buf1, buf );
				}
				else
					strcat( buf1, "\n\r");

			// wp2 dr
				sprintf( buf, "¦¸­nªZ¾¹§ðÀ»ÂI¼Æ¡G"HIG"%4d"NOR"¡@",get_damroll( victim, WEAR_WIELD_2 ) );
				strcat( buf1, buf );
				
			// water , earth, breath
				if (ch->level > 36)
				{
					sprintf( buf, NOR"¤ô¨t¡G"HIW"%4d"NOR"¡@¦a¨t¡G"HIW"%4d"NOR"¡@¦R®§¡G"HIW"%4d"NOR"\n\r",
					NEW_GET_SVS(victim, MAGIC_COLD),
					NEW_GET_SVS(victim, MAGIC_EARTH),
					NEW_GET_SVS(victim, MAGIC_BREATH));
					strcat( buf1, buf );
				}
				else
					strcat( buf1, "\n\r");
			}
			else
			{
				sprintf( buf, "¦¸­nªZ¾¹©R¤¤ÂI¼Æ¡G"HIG"¡@µL"NOR"¡@");
				strcat( buf1, buf );

				if ( ch->level > 36)
				{
					sprintf( buf, "ªk³N¡G"HIW"%4d"NOR"¡@­·¨t¡G"HIW"%4d"NOR"¡@¤õ¨t¡G"HIW"%4d"NOR"¡@¬r¯À¡G"HIW"%4d"NOR"\n\r",
					NEW_GET_SVS(victim, MAGIC_SPELL),
					NEW_GET_SVS(victim, MAGIC_WIND),
					NEW_GET_SVS(victim, MAGIC_FIRE),
					NEW_GET_SVS(victim,MAGIC_POISON)); // ¼W¥[¤¤¬r¨Ctick¨ü¨ì¶Ë®`Åã¥Ü 2021/11/03
					strcat( buf1, buf );
				}
				else
					strcat( buf1, "\n\r");

				sprintf( buf, "¦¸­nªZ¾¹§ðÀ»ÂI¼Æ¡G"HIG"¡@µL"NOR"¡@");
				strcat( buf1, buf );

				if (ch->level > 36)
				{
					sprintf( buf, NOR"¤ô¨t¡G"HIW"%4d"NOR"¡@¦a¨t¡G"HIW"%4d"NOR"¡@¦R®§¡G"HIW"%4d"NOR"\n\r",
					NEW_GET_SVS(victim, MAGIC_COLD),
					NEW_GET_SVS(victim, MAGIC_EARTH), NEW_GET_SVS(victim, MAGIC_BREATH));
					strcat( buf1, buf );
				}
				else
					strcat( buf1, "\n\r");
			}
		}
    
		if ( ch->level > 36 )
		{
			sprintf( buf, "Å]  ªk    ¼ç  ¤O¡G"HIP"%4d"NOR"¡@",  victim->pp );
			strcat( buf1, buf );

			sprintf( buf, NOR"¹p¨t¡G"HIW"%4d"NOR"¡@¥ú¨t¡G"HIW"%4d"NOR"¡@îî¨t¡G"HIW"%4d"NOR"\n\r",
				NEW_GET_SVS(victim, MAGIC_LIGHTNING), NEW_GET_SVS(victim, MAGIC_SAINT), NEW_GET_SVS(victim, MAGIC_DARK));
			strcat( buf1, buf );

			sprintf( buf, "ª«¡@²z ¶Ë ®`¡@¤O¡G"HIP"%4d"NOR"¡@",   victim->apdr );
			strcat( buf1, buf );

			sprintf( buf, "§Ü¡@ª« ²z ¶Ë¡@®`¡G"HIP"%4d"NOR"\n\r", victim->pdr );
			strcat( buf1, buf );

			sprintf( buf, "Å]¡@ªk ¶Ë ®`¡@¤O¡G"HIP"%4d"NOR"¡@",   victim->amdr );
			strcat( buf1, buf );

			sprintf( buf, "§Ü¡@Å] ªk ¶Ë¡@®`¡G"HIP"%4d"NOR"\n\r", victim->mdr );
			strcat( buf1, buf );
		}
		// ±N¾Ô°«°O¿ý§ï¨ì score f ¤§¤U, ¨Ã¥[¤J§¹¾ãÅã¥Ü 2021/11/21
		//if ( !str_cmp( key, "all" ) || !str_cmp( key, "a" ) )
		if ( !str_cmp( key, "f" ) )
		{
			strcat( buf1, "§Aªº¾Ô°«°O¿ý¦p¤U¡G\n\r" );
			if ( ( vch = victim->tracking ) )
			{
				sprintf( buf, YEL "§A¥¿¦b°lÂÜ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->assassinating ) )
			{
				sprintf( buf, RED "§A¥¿¦b·t±þ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->hunting ) )
			{
				sprintf( buf, HIR "§A¥¿¦bÂy±þ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->cooperating) )
			{
				sprintf( buf, HIW "§A¥¿¦b¨ó§U "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->protecting) )
			{
				sprintf( buf, HIY "§A¥¿¦b«OÅ@ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
				strcat( buf1,"\n\r");
			}
			if ( ( victim->hunted ) ){
				int hunt_num = 0;
				strcat( buf1, ""HIR"¥¿¦bÂy±þ§A¡G"NOR"\n\r" );
				for( vch = victim->hunted; vch; vch = vch->next_in_hunt ){
					hunt_num++;
					if( hunt_num > 3 ){
						hunt_num = 0;
						strcat( buf1, "\n\r");
						sprintf( buf, "%s ", vch->short_descr );
						strcat( buf1, buf );
					}else{
						sprintf( buf, "%s ", vch->short_descr );
						strcat( buf1, buf );
					}
				}
				strcat( buf1,".\n\r");
				strcat( buf1,"\n\r");
			}
			if( ( vch = victim->cooperator ) ) {
				int coo_num = 0;
				strcat( buf1, ""HIW"¥¿¦b¨ó§U§A¡G"NOR"\n\r" );
				for( vch = victim->cooperator; vch; vch = vch->next_in_coop ) {
					coo_num++;
					if(coo_num > 3){
						coo_num = 0;
						strcat( buf1, "\n\r");
						sprintf(buf, "%s ", vch->short_descr);
						strcat(buf1, buf);
					}else{
						sprintf(buf, "%s ", vch->short_descr);
						strcat(buf1, buf);
					}
				}
				strcat( buf1,".\n\r");
				strcat( buf1,"\n\r");
			}
			if( ( vch = victim->protector ) ) {
				int pro_num = 0;
				strcat( buf1, ""HIY"¥¿¦b«OÅ@§A¡G"NOR"\n\r" );
				for( vch = victim->protector; vch; vch = vch->next_in_protect ) {
					pro_num++;
					if(pro_num > 3){
						pro_num = 0;
						strcat( buf1, "\n\r");
						sprintf(buf, "%s ", vch->short_descr);
						strcat(buf1, buf);
					}else{
						sprintf(buf, "%s ", vch->short_descr);
						strcat(buf1, buf);
					}
				}
				strcat( buf1,".\n\r");
			}
		}else{
			if ( ( vch = victim->tracking ) )
			{
				sprintf( buf, YEL "§A¥¿¦b°lÂÜ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
			}
			if ( ( vch = victim->assassinating ) )
			{
				sprintf( buf, RED "§A¥¿¦b·t±þ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
			}
			if ( ( vch = victim->hunting ) )
			{
				sprintf( buf, HIR "§A¥¿¦bÂy±þ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
			}
			// ·s¼W cooperator Åã¥Ü 2021/11/20 
			if ( ( vch = victim->cooperating) )
			{
				sprintf( buf, HIW "§A¥¿¦b¨ó§U "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
			}
			if ( ( vch = victim->protecting) )
			{
				sprintf( buf, HIY "§A¥¿¦b«OÅ@ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
			}
			// ·s¼W hunted ­p¼Æ¦b config +COMBINE ®ÉÁY´îÅã¥Ü 2021/11/21
			if ( ( victim->hunted ) ){
				int hunt_num = 0;
				for( vch = victim->hunted; vch; vch = vch->next_in_hunt ){
					hunt_num++;
				}
				for( vch = victim->hunted; vch; vch = vch->next_in_hunt ){
					if( hunt_num > 3 && IS_SET( ch->act, PLR_COMBINE ) ){
						sprintf( buf, "("HIR"%d"NOR")%s "HIR"¥¿¦bÂy±þ§A"NOR".\n\r", hunt_num, vch->short_descr);
						strcat( buf1, buf );
						break;
					}else{
						sprintf( buf, "%s "HIR"¥¿¦bÂy±þ§A"NOR".\n\r", vch->short_descr );
						strcat( buf1, buf );
					}
				}
			}
			// ·s¼W cooperator Åã¥Ü 2021/11/20 
			if( ( vch = victim->cooperator ) ) {
				int coo_num = 0;
				for( vch = victim->cooperator; vch; vch = vch->next_in_coop ) {
					coo_num++;
				}
				for( vch = victim->cooperator; vch; vch = vch->next_in_coop ) {
					if( coo_num > 3 && IS_SET( ch->act, PLR_COMBINE ) ){
						sprintf( buf, "("HIW"%d"NOR")%s "HIW"¥¿¦b¨ó§U§A"NOR".\n\r", coo_num, vch->short_descr);
						strcat( buf1, buf );
						break;
					}else{
						sprintf( buf, "%s "HIW"¥¿¦b¨ó§U§A"NOR".\n\r", vch->short_descr );
						strcat( buf1, buf );
					}
				}
			}
			// ·s¼W protected ­p¼Æ¦b config +COMBINE ®ÉÁY´îÅã¥Ü 2021/11/21
			if( ( vch = victim->protector ) ) {
				int pro_num = 0;
				for( vch = victim->protector; vch; vch = vch->next_in_protect ) {
					pro_num++;
				}
				for( vch = victim->protector; vch; vch = vch->next_in_protect ) {
					if( pro_num > 3 && IS_SET( ch->act, PLR_COMBINE ) ){
						sprintf( buf, "("HIY"%d"NOR")%s "HIY"¥¿¦b«OÅ@§A"NOR".\n\r", pro_num, vch->short_descr);
						strcat( buf1, buf );
						break;
					}else{
						sprintf( buf, "%s "HIY"¥¿¦b«OÅ@§A"NOR".\n\r", vch->short_descr );
						strcat( buf1, buf );
					}
				}
			}
		}
	}
	
	// affected_by ¯S®í¼vÅT,  score a
	if ( !str_cmp( key, "all" ) || !str_cmp( key, "a" ) )
	{
		if ( !str_cmp( key, "a" ) )
		{
		strcat( buf1, "§A¥¿¨ü¨ì¥H¤U¯S®í¼vÅT¡G\n\r" );
		}

		if ( victim->affected )
		{
			bool printed = FALSE;

			for ( paf = victim->affected; paf; paf = paf->next )
			{
				if ( paf->deleted )
					continue;

				if ( !printed && str_cmp( key, "a" ) )
				{
					strcat( buf1, "§A¥¿¨ü¨ì¥H¤U¯S®í¼vÅT¡G\n\r" );
					printed = TRUE;
				}

				sprintf( buf, "¯S®Ä¡G¡u"HIC"%s"NOR"(%s)¡v", skill_table[paf->type].cname, skill_table[paf->type].name );
				strcat( buf1, buf );

				if ( ch->level >= 20 )
				{
					strcat( buf1, "(");
					if( strcmp(c_affect_loc_name(paf->location), "µL"))
					{
						sprintf( buf, "¼vÅT"HIY"%s"NOR, c_affect_loc_name( paf->location ) );
						strcat( buf1, buf );
					}
					if( paf->modifier )
					{
						sprintf( buf, " %s%d "NOR"ÂI  ",
								paf->modifier<0? HIU"-":( paf->modifier == 0 ? HIW:HIC"+"),
								paf->modifier<0?-paf->modifier:paf->modifier
								);
						strcat( buf1, buf );
					}
					if( paf->duration >= 0 )
					{
						sprintf( buf, "©|¦³ "HIG"%d"NOR" ¤p®Éªº®Ä¥Î¡C", paf->duration );
						strcat( buf1, buf );
					}
					else
					{
						sprintf(buf, "«ùÄò§@¥Î¤¤¡C");
						strcat(buf1, buf);
					}
					strcat(buf1, ")");
				}

				strcat( buf1, "\n\r" );
			}
		}
	}

	if ( !str_cmp( key, "all" ) )
	{
		strcat(buf1, NOR"¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹¡¸¡¹"NOR"\n\r");
	}

	send_to_char( buf1, ch );
	return;
}



char *        const    day_name    [] =
{
    "¤ë", "­·", "»a", "¹p", "¦Û¥Ñ",
    "¸t¯«", "¬P"
};

char *        const    month_name      [] =
{
    "¥V", "¥V¯T", "Á÷¤§¥¨¤H", "»·¥j",
    "¤j¾Ô", "¬K", "¦ÛµM", "¯î¼o", "Às",
    "¤Ó¶§", "´»®ð", "§Å³N", "·t¼v", "«Õ­ß",
    "ªø©]", "¶Â·t", "¤j´cÅ]"
};

void do_time( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];
	extern char str_boot_time[];
	extern time_t down_time; //, boot_time;
	char *suf;
	int day;

	// day = time_info.day + 1;
	day = time_info.day;

	if ( day > 4 && day < 20 ) suf = "th";
	else if ( day % 10 ==  1 ) suf = "st";
	else if ( day % 10 ==  2 ) suf = "nd";
	else if ( day % 10 ==  3 ) suf = "rd";
	else suf = "th";

	if( IS_IMMORTAL(ch) )
		sprintf( buf,
		"[36m²{¦b¬O%s%d®É¡A%s¤§¤é¡A%d%s¡A%s(%d)¤§¤ë¡C[m\n\r",
		//time_info.hour >= 12 ? "¤U¤È" : "¤W¤È",
		"",
		//( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
		time_info.hour % 24,
		day_name[day % 7], 
		day, suf,
		month_name[time_info.month], time_info.month );
	else
		sprintf( buf,
		"[36m²{¦b¬O%s%d®É¡A%s¤§¤é¡A%d%s¡A%s¤§¤ë¡C[m\n\r",
		//time_info.hour >= 12 ? "¤U¤È" : "¤W¤È",
		"",
		//( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
		time_info.hour % 24,
		day_name[day % 7],
		day, suf,
		month_name[time_info.month] );
	send_to_char( buf, ch );
	sprintf( buf, "[36m²V¨P¤Û¥@±Ò°Ê©ó %s\n\r¨t²Î®É¶¡¬O     %s[m\n\r", str_boot_time, (char *) ctime( &current_time ) );
	send_to_char( buf, ch );

	if ( down_time > 0 )
	{
		sprintf( buf, "[36m¤U¦¸%s±N©ó %s[m\r", Reboot ? "Reboot" : "Shutdown", (char *) ctime( &down_time ) );
		send_to_char( buf, ch );
	}
	return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];


	//winter check add at 2018.11.06
	static char * const sky_look[ 6 ] =
	{
		"¸U¨½µL¶³",
		"¦h¶³",
		"­°µÛ¤p«B",
		"¥´µÛ°{¹q¤UµÛ¼É«B",
		"ÄÆµÛ¤p³·",
		"¨í°_°}°}¼É­·³·"
	};

	if ( !IS_OUTSIDE( ch ) )
	{
		send_to_char( "[33m¦b³o¸Ì§AÆ[¹î¤£¥X¤ÑªÅªºÅÜ¤Æ.[m\n\r", ch );
		return;
	}

	//monsoon change add at 2018.11.06
	sprintf( buf, "¤ÑªÅ%s, ¦Ó¥B%s.\n\r",
		sky_look[weather_info.sky],
		time_info.month <= 4 
			? "¦³¥_¤è»þ¨æªº´H­·§j¹L" 
			: ((time_info.month >= 5 && time_info.month <= 8)
			? "¦³ªF¤è·Å·xªº©M­·§j¹L" 
			: ((time_info.month >= 9 && time_info.month <= 12)
			? "¦³«n¤è¼éÀãªº¼ö­·§j¹L" 
			: "¦³¦è¤è²M²nªº²D­·§j¹L")) );
	send_to_char( buf, ch );
	return;
}


void do_sector( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];
	ROOM_INDEX_DATA  *in_room = NULL;
	int RoomSector = ch->in_room->sector_type;

	if ( !check_blind( ch ) )
		return;

	if ( room_is_dark( ch->in_room ) ){
		send_to_char( "¥|©P¤@¤ùº£¶ÂµLªk±oª¾©P³òÀô¹Ò....\n\r",ch);
		return;
	}


	static char * const SectorType[ 11 ] =
	{
		"©P³òÀô¹Ò¨S¤°»ò¯S§Oªº",
		"©P³òÀô¹Ò¦³µÛ³\\¦h«Ø¿vª«ªº«°¥«",
		"©P³òÀô¹Ò¬O¤@¤ù¥­©Zªº­ì³¥",
		"©P³òÀô¹Ò¬O¦³µÛ³\\¦h¾ð¤ìªº´ËªL",
		"©P³òÀô¹Ò¬O¦a­±°ª§C¤£¥­ªº¥C³®",
		"©P³òÀô¹Ò¬OªÅ®ðµ}Á¡ªº°ª¤s",
		"§A¦ì¦b¥­ÀRªº¤ô­±¤W",
		"§A¨­³B¦b´ø«æªº¤ô¬y",
		"§A¨­³B¦b¤ô¤¤",
		"§A¨­³B¦b¥bªÅ¤¤",
		"©P³òÀô¹Ò¬O°®§òªº¯îºz"
	};
	sprintf( buf, "%s", SectorType[RoomSector]);
	
	if( IS_SET(ch->in_room->room_flags, ROOM_DARK)
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
		|| IS_SET(ch->in_room->room_flags, ROOM_PK)
		|| IS_SET(ch->in_room->room_flags, ROOM_UNDERGROUND))
	{
		sprintf( buf, "%s.\r\n\r\n³o¸Ì", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_PK))
			sprintf( buf, "%s¬O°«§Þ³õ", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
			sprintf( buf, "%s¨ü¨ì¶A©G", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_UNDERGROUND))
			sprintf( buf, "%s¶§¥ú³Q¾B½ª", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_DARK))
			sprintf( buf, "%s²§±`¶Â·t", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_CAMP)
			|| IS_SET(ch->in_room->room_flags, ROOM_SAFE))
		{
			sprintf( buf, "%s, ", buf );
			if( IS_SET(ch->in_room->room_flags, ROOM_CAMP))
				sprintf( buf, "%s¦a­±¶Ç¨ÓÅ]¤O", buf );
			if( IS_SET(ch->in_room->room_flags, ROOM_SAFE))
				sprintf( buf, "%s¬Û·í¦w¥þ", buf );
		}
	}else if( IS_SET(ch->in_room->room_flags, ROOM_CAMP)
			|| IS_SET(ch->in_room->room_flags, ROOM_SAFE))
	{
		sprintf( buf, "%s.\r\n\r\n³o¸Ì", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_CAMP))
			sprintf( buf, "%sªº¦a­±¶Ç¨ÓÅ]¤O", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_SAFE))
			sprintf( buf, "%s¬Û·í¦w¥þ", buf );
	}
	sprintf( buf, "%s.\r\n", buf );
	send_to_char( buf, ch );
    return;
}


void do_help( CHAR_DATA *ch, char *argument )
{
	HELP_DATA *pHelp;
	bool hfound = FALSE; /* modification to show multiple helps
	by Robin Goodall from merc mailing list */

	if ( argument[0] == '\0' )
		argument = "summary";

	for ( pHelp = help_first; pHelp; pHelp = pHelp->next )
	{
		if ( pHelp->level > get_trust( ch ) )
			continue;

		if ( is_name( argument, pHelp->keyword ) )
		{
			if ( hfound )
				send_to_char( "\n\r----------------------------------------------------------------------------\n\r\n\r", ch );

			if ( pHelp->level >= 0 && str_cmp( argument, "imotd" ) )
			{
				send_to_char( pHelp->keyword, ch );
				send_to_char( "\n\r", ch );
			}

			/*
			* Strip leading '.' to allow initial blanks.
			*/
			if ( pHelp->text[0] == '.' )
				send_to_char( pHelp->text+1, ch );
			else
				send_to_char( pHelp->text  , ch );
			hfound = TRUE;
		}
	}

	if ( !hfound )
		send_to_char( "[33m¨S¦³Ãö©ó³o­Ó¦rªº»¡©ú³á¡I[m\n\r", ch );
	return;
}



/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
// do_who modified by jye 6.18.2005
int MAX_PPL;
void do_who( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA *d;
  //CLAN_DATA *cn;
  char       buf      [ MAX_STRING_LENGTH*7 ];
  char       buf2     [ BUF_STRING_LENGTH*2   ];
  //char     name_clan [ MAX_INPUT_LENGTH ];
  char       arg1 [ MAX_INPUT_LENGTH ];
  int        iClass;
  int        iRace;
  int        iLevelLower;
  int        iLevelUpper;
  int        nNumber;
  int        nMatch;
  int        current_ppl = 0;
  int        curr_lv, i, comp_lv;
  bool       rgfClass [ MAX_CLASS ];
  bool       rgfRace [ MAX_RACE ];
  bool       fRaceRestrict;
  bool       fClassRestrict;
  bool       fImmortalOnly;
  bool       fChinese = FALSE;
  bool       fVerbose = FALSE;
  bool       fGroup = FALSE;
  bool       fPK = FALSE;
  int        count = 0;
  bool       fListGroup = FALSE;
  bool       fRecall = FALSE;
  bool       who_clan = FALSE;
  // bool   cn_exist = FALSE;
  CHAR_DATA  *wch[ 500 ];
  CHAR_DATA  *leader = NULL;
  char const *class[ 500 ];

  /*
   * Set default arguments.
   */
  iLevelLower    = 0;
  iLevelUpper    = L_ACG; /*Used to be Max_level */
  fClassRestrict = FALSE;
  fImmortalOnly  = FALSE;
  fRaceRestrict = FALSE;
  
  for( iClass = 0; iClass < MAX_CLASS; iClass++ )
       rgfClass[iClass] = FALSE;

  for( iRace = 0; iRace < MAX_RACE; iRace++ )
       rgfRace[iRace] = FALSE;
       
  if( IS_SET( ch->act, PLR_CHINESE ) )
      fChinese = TRUE;
  /*
   * Parse arguments.
   */
  nNumber = 0;
  for( ;; )
  {  
       char arg [ MAX_INPUT_LENGTH ];
     
       argument = one_argument( argument, arg );
       argument = one_argument( argument, arg1 );
       
       if( arg[0] == '\0' )
           break;
     
       if( is_number( arg ) )
       {
       	   //modify by jye 0940618
       	   iLevelLower = atoi( arg );
       	   iLevelUpper = ( 0 < atoi( arg1 ) ? atoi( arg1 ) : iLevelUpper );
           //switch ( ++nNumber )
           //{
           //   case 1: iLevelLower = atoi( arg ); break;
           //   case 2: iLevelUpper = atoi( arg1 ); break;
           //   default:
           //        send_to_char( "[33m¥u¯à¥´¨â­Óµ¥¯Å¡C[m\n\r", ch );
           //        return;
           //}
       }
       else if( arg[0] == '-' )
       {
           switch( arg[1] )
           {
              case 'c' : fChinese = TRUE;
                   break;
              case 'e' :
                   fChinese = FALSE;
                   break;
              case 'v' :
                   fVerbose = TRUE;
                   break;
              case 'r' :
                   fRecall = TRUE;
                   break;
              
              default:
                   send_to_char(YEL "¨S¦³³o­Ó°Ñ¼Æ¡C"NOR"\n\r", ch);
                   return;
           }
       }
       else
       { 
           int iClass;
              
           if( !str_cmp(arg, "group") )
           {
               fGroup = TRUE;
//              one_argument( argument, buf2);
               if( arg1[0] == '\0')
                   break;
          
//              log_string( "who group test 1" );
//              log_string( arg1);
               leader = get_char_world(ch,arg1);
               
               if( !leader || IS_NPC(leader))
               {
                   send_to_char(YEL "¨S¦³³o­Óª±®a¡C"NOR"\n\r", ch);
                   return;
               }
               fGroup = FALSE;
               fListGroup = TRUE;
               break;
           }
/*        
            if ( !str_cmp( arg, "clan" ) )
            {
              cn = clan_list;
              who_clan = TRUE;
              if( arg1[0] != '\0' )
              {
                for( cn; cn; cn = cn->next )
                {
                  if ( !str_cmp( cn->leader, arg1 ) )
                  {
                    sprintf( name_clan, cn->name );
                    cn_exist = TRUE;
                    break;
                  }
                  else
                  {
                    send_to_char( "³o¤H¬OÀ°¥D¶Ü¡H\n\r", ch );
                    return;
                  }
                }
              }
              send_to_char( cn->name, ch );
              break;
            }
*/        
            if( !str_cmp(arg, "pk"))
            {
                fPK = TRUE;
                break;
            }
          
              /*
               * Look for classes to turn on.
               */
            //arg[3]    = '\0';
            if( !str_cmp( arg, "imm" ) )
            {
                fImmortalOnly = TRUE;
            }
            else
            {
                if( strlen( arg ) < 3 )
                {
                    send_to_char( "[33mÂ¾·~©ÎºØ±Ú¦WºÙ­n¥´±o§óªø¨Ç¡C[m\n\r", ch );
                    return;
                }
                
                for( iClass = 0; iClass < MAX_CLASS; iClass++ )
                {
                     if( !str_prefix( arg, class_table[iClass].who_name ) 
                         || !str_prefix(arg, class_table[iClass].name))
                     {
                         rgfClass[iClass] = TRUE;
                         fClassRestrict = TRUE;
                         break;
                     }
                }
                
                //add by jye 0940618
                if( !fClassRestrict)
                { 
                    for( iRace = 0; iRace < MAX_RACE; iRace++ )
                    {
                         if( !str_prefix( arg, race_table[iRace].name ) ) 
                         {
                             rgfRace[iRace] = TRUE;
                             fRaceRestrict = TRUE;
                             break;
                         }
                    }
                }
                
                if( !fClassRestrict && !fRaceRestrict )
                {
                  send_to_char( "[33m¨º¤£¬O¤@­ÓÂ¾·~©ÎºØ±Ú¡I¡C[m\n\r", ch );
                  return;
                }
             }
       }
  }


    /*
     * Now show matching chars.
     */
  nMatch = 0;
  buf[0] = '\0';
  sprintf( buf , "[ "HIC"²V¨P¤Û¥@"NOR" ]%s%s%s\n\r""========================================================================"NOR"\n\r",
            ( fPK || fImmortalOnly || fListGroup || who_clan ) ? " - " : "",
             fListGroup ? ( (leader->group) ? leader->group->name: leader->short_descr ): 
            (fPK ? "°Ñ¥[ PK ª±®a¦Cªí" : (fImmortalOnly ? "¯«±Ú¦Cªí" :(who_clan ? "À°­û¦Cªí" : ""))),
             fListGroup ? (leader->group ? "" : "ªº¶¤¥î"): "");
  for( d = descriptor_list, i=0; d; d = d->next, i++ )
  {
       current_ppl += 1;

       wch[i] = ( d->original ) ? d->original : d->character;

       /*
        * Check for match against restrictions.
        * Don't use trust as that exposes trusted mortals.
        */
       if( d->connected != CON_PLAYING || !can_see( ch, wch[i] ) )
       {
                 i--;
           continue;
       }
       
       if( IS_NPC(wch[i]) ) 
       {
           i--;
           continue;
       }

       if( !( wch[i]->level >= iLevelLower && wch[i]->level <= iLevelUpper )
           || ( fImmortalOnly  && ( wch[i]->level < L_APP && wch[i]->trust < L_APP) )
           || ( fPK && !IS_SET( wch[i]->act, PLR_REGISTER ))
           || ( fClassRestrict && !rgfClass[wch[i]->class] ) 
           || ( fRaceRestrict && !rgfRace[wch[i]->race] ) 
           || ( fListGroup && !is_same_group(wch[i], leader))
           || ( fGroup && (!wch[i]->group || wch[i] != wch[i]->group->leader ))
           || ( fRecall && wch[i]->in_room->area->recall != ch->in_room->area->recall )
           //|| ( fGroup && wch[i]->leader != wch[i] && wch[i]->leader->leader != wch[i] )
           )
       {
           i--;
           continue;
       }

       nMatch++;

       /*
        * Figure out what to print for class.
        */
        
       if( fChinese )
           class[i] = class_table[wch[i]->class].cwho_name;
       else
           class[i] = class_table[wch[i]->class].who_name;
           
       if( wch[i]->level >= 41)//LEVEL_IMMORTAL )
           switch ( wch[i]->level )
           {
              default: break;
              case L_ACG: class[i] = (fChinese) ? HIW"ÅU  °Ý"NOR:HIW"ANC"NOR; break;
              case L_DIR: class[i] = (fChinese) ? HIC"³Ð¥@¯«"NOR:HIC"ADM"NOR; break;
              case L_SEN: class[i] = (fChinese) ? HIY"¤j  ¯«"NOR:HIY"GOD"NOR; break;
              case L_JUN: class[i] = (fChinese) ? HIU"  ¯«  "NOR:HIU"GOD"NOR; break;
              case L_APP: class[i] = (fChinese) ? HIG"¤Ñ  ¨Ï"NOR:HIG"ANG"NOR; break;
              case 42: class[i] = (fChinese) ? HIR"¶Ç  ©_"NOR:HIR"LGD"NOR; break;
       
           }
  }
  /*
   * Format it up.
   */

  curr_lv = ( who_clan ? 5 : MAX_LEVEL );
  comp_lv = ( who_clan ? 0 : 1 );
  	
  for( ; curr_lv >= comp_lv ; curr_lv-- )
  { //sort lv
       for( i=0; i<nMatch; i++ ) 
       {//for each lv, we search for every match
            if( curr_lv != ( who_clan ? wch[i]->pcdata->clanlevel : wch[i]->level ) )
                continue;
        /*if( cn_exist )
        {
          if( wch[i]->clan )
          {
            if ( str_cmp( name_clan, wch[i]->clan->whoname ) == FALSE )
              continue;
          }
          else
            continue;
        }*/
           ++count;
           if( fChinese )
           {
               if( wch[i]->level < ( LEVEL_IMMORTAL - 2 ) )
               {
                   sprintf( buf + strlen( buf ), "¡i"HIW" %2d"NOR" "HIW"%6s"NOR" ¡j %s%s%s%s%s("HIY"%s"NOR") %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"µLÀ°¬£"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"¥û¤â"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF    ) ? "("RED"¤p°½"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"     : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK    ) )  ? "("HIP"µo§b"NOR")"   : "",
                            //wch[i]->short_descr,
                            wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? wch[i]->temp_race - 1 : wch[i]->race )].cname);
                            //race_table[ wch[i]->race ].cname );
               }
               else if( wch[i]->level == ( LEVEL_IMMORTAL - 2 ) )
               {
                   sprintf( buf + strlen( buf ), "¡i"HIW" %2d"NOR" "HIP"%6s"NOR" ¡j %s%s%s%s%s("HIY"%s"NOR") %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"µLÀ°¬£"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"¥û¤â"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF    ) ? "("RED"¤p°½"NOR")"  : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"     : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK    ) )  ? "("HIP"µo§b"NOR")"   : "",
                            //wch[i]->short_descr,
                            wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].cname);
                            //race_table[ wch[i]->race ].cname );
               }
               else
               {
                   sprintf( buf + strlen( buf ), "¡i"HIW" %2d"NOR" %6s"NOR" ¡j %s%s%s%s%s%s("HIY"%s"NOR") %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"µLÀ°¬£"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_WIZINVIS ) ? "("HIY"¯«Áô"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_KILLER  ) ) ? "("RED"¥û¤â"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_THIEF     )) ? "("RED"¤p°½"NOR")"  : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER) ) ? "("HIG"PK"NOR")"     : "",
                            (  IS_SET( wch[i]->act, PLR_AFK    )) ? "("HIP"µo§b"NOR")"    : "",
                            //wch[i]->short_descr,
                            wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].cname);
                            //race_table[ wch[i]->race ].cname );
               }
           }
           else
           {
               if( wch[i]->level < ( LEVEL_IMMORTAL - 2 ) )
               {
                   sprintf( buf + strlen( buf ), "¡i"HIW" %2d"NOR" "HIW"%3s"NOR" ¡j %s%s%s%s"HIY"%s"NOR" %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"µLÀ°¬£"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"¥û¤â"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF     ) ? "("RED"¤p°½"NOR")"  : "",
                            (fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"      : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK     )) ? "("HIP"µo§b"NOR")"    : "",
                            //wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].name);
                            //race_table[ wch[i]->race ].name );
               }
               else if( wch[i]->level == ( LEVEL_IMMORTAL - 2 ) )
               {
                   sprintf( buf + strlen( buf ), "¡i"HIW" %2d"NOR" "HIP"%3s"NOR" ¡j %s%s%s%s"HIY"%s"NOR" %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"µLÀ°¬£"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"¥û¤â"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF     ) ? "("RED"¤p°½"NOR")"  : "",
                            (fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"      : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK     )) ? "("HIP"µo§b"NOR")"    : "",
                            //wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? wch[i]->temp_race - 1 : wch[i]->race )].name);
                            //race_table[ wch[i]->race ].name );
               }
               else
               {
                   sprintf( buf + strlen( buf ), "¡i"HIW" %2d"NOR" %s"NOR" ¡j %s%s%s%s%s"HIY"%s"NOR" %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"µLÀ°¬£"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_WIZINVIS ) ? "("HIY"¯«Áô"NOR")" : "", 
                            ( fVerbose && IS_SET( wch[i]->act, PLR_KILLER   )) ? "("RED"¥û¤â"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_THIEF     )) ? "("RED"¤p°½"NOR")"  : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"     : "",
                            IS_SET( wch[i]->act, PLR_AFK     ) ? "("HIP"µo§b"NOR")"    : "",
                            //wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].name);
                            //race_table[ wch[i]->race ].name );
               }
           }
       } //j end
       
		// ­×§ï³Ì¤j pagelength , ¬G¥iÅã¥Ü¤W­­´£°ª 2023/01/12
		//if( count >= 50 ) {
		if( count >= 80 ) {
			send_to_char(buf, ch);
			buf[0] = '\0';
			count = 0;
		}
  } //sort lv end    
  
  sprintf( buf2, "%s¦³ "HIW"%d / %d"NOR" ¦W«iªÌ¥¿¦b³Ð³y·sªº¶Ç»¡¡C\n\r", 
           "========================================================================"NOR"\n\r",
           nMatch, current_ppl );
  strcat( buf, buf2 );

  if( current_ppl > MAX_PPL )
      MAX_PPL = current_ppl;
  sprintf( buf2, "³Ì¦h´¿¦³ "HIW"%d"NOR" ¦W«iªÌ¦P®É¦b³o­Ó¥@¬É¹C¾ú¡C\n\r", MAX_PPL );
  strcat( buf, buf2 );

  send_to_char( buf, ch );
  return;
}

/* Contributed by Kaneda */
void do_whois( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char       buf  [ MAX_STRING_LENGTH  ];
    char       name [ MAX_INPUT_LENGTH   ];
 
    one_argument( argument, name );

    if( name[0] == '\0' )
    {
  send_to_char( "[33m¨Ï¥Î¤èªk¡Gwhois <name>[m\n\r", ch );
  return;
    }

    name[0] = UPPER( name[0] );

    buf[0] = '\0';
    for( d = descriptor_list ; d ; d = d->next )
    {
  CHAR_DATA *wch;
  char    const *class;
  
  wch = ( d->original ) ? d->original : d->character; 

  if( d->connected != CON_PLAYING || !can_see( ch, wch ) )
      continue;
  
  if( str_prefix( name, wch->name ) )
      continue;

  class = class_table[ wch->class ].cname;
  if( wch->level >= LEVEL_IMMORTAL )
      switch( wch->level )
      {
        case L_ACG: class = HIW"ÅU°Ý"NOR; break;
        case L_DIR: class = HIW"¤Ñ¯«"NOR; break;
        case L_SEN: class = HIY"¤j¯«"NOR; break;
        case L_JUN: class = HIG" ¯« "NOR; break;
        case L_APP: class = HIC"¤Ñ¨Ï"NOR; break;
/*
        case L_DIR: class = "[1;36m¤j¤j¯«[m"; break;
        case L_SEN: class = "[1;36m ¤j¯« [m"; break;
        case L_JUN: class = "[1;36m  ¯«  [m"; break;
        case L_APP: class = "[1;36m¨£²ß¯«[m"; break;
*/
      }
    
  /*
   * Format it up.
   */
  if ( wch->level < LEVEL_IMMORTAL )
      sprintf( buf + strlen( buf ), "(%2d %s %-6s(%-8s))%s%s%s%s%s%s %s\n\r",
        wch->level,
        class,
        race_table[wch->race].cname,
        race_table[wch->race].name,
        IS_SET( wch->act, PLR_KILLER   ) ? "("RED"¥û¤â"NOR")" : "",
        IS_SET( wch->act, PLR_THIEF    ) ? "("RED"¤p°½"NOR")"  : "",
        IS_SET( wch->act, PLR_REGISTER ) ? "("HIG"PK"NOR")"     : "",
        IS_SET( wch->act, PLR_AFK    ) ? "("HIP"µo§b"NOR")"    : "",
        wch->short_descr,
        wch->pcdata->title , "");
  else
      sprintf( buf + strlen( buf ), "(        %s        )%s%s%s%s%s%s%s %s\n\r",
        class,
        IS_SET( wch->act, PLR_WIZINVIS ) ? "("HIY"¯«Áô"NOR")" : "",
        IS_SET( wch->act, PLR_KILLER   ) ? "("RED"¥û¤â"NOR")" : "",
        IS_SET( wch->act, PLR_THIEF    ) ? "("RED"¤p°½"NOR")"  : "",
        IS_SET( wch->act, PLR_REGISTER ) ? "("HIG"PK"NOR")"     : "",
        IS_SET( wch->act, PLR_AFK    ) ? "("HIP"µo§b"NOR")"    : "",
        wch->short_descr,
        wch->pcdata->title , "");
      }

    if ( buf[0] == '\0' )
  send_to_char( "[33m¨S¦³¤H¦X§Aªº±ø¥ó¡C[m\n\r", ch );
    else
  send_to_char( buf, ch );
    return;
}


void do_inventory( CHAR_DATA *ch, char *argument )
{
    if( IS_IMMORTAL(ch) ) {
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  one_argument(argument, arg);
  if( arg[0] == '\0' ) victim = ch;
  else if( ! (victim = get_char_world(ch, arg ) ) ){
      send_to_char("³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch);
      return;
  }
  send_to_char( "§A¨­¤W±aµÛ¦³¡G\n\r\n\r", ch );
  show_list_to_char( victim->carrying, ch, TRUE, TRUE );
  return;
    }
    send_to_char( "§A¨­¤W±aµÛ¦³¡G\n\r\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int       iWear;
    bool      found;
    CHAR_DATA *victim;
    if( IS_IMMORTAL(ch) ) {
      char arg[MAX_INPUT_LENGTH];
      one_argument(argument, arg);
      if( arg[0] == '\0' ) victim = ch;
      else if( ! (victim = get_char_world(ch, arg ) ) ){
          send_to_char("³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch);
          return;
      }
    } else {
      victim = ch;
    }

    send_to_char( "§A¨­¤W¸Ë³ÆµÛ¡G\n\r", ch );
    found = FALSE;
    if ( !IS_SET( race_table[ victim->race ].race_abilities, RACE_BEHOLDER ) ){
        for ( iWear = 0; iWear < MAX_WEAR; iWear++ ){
          if ( obj = get_eq_char( victim, iWear ) ) {
            send_to_char( where_name[iWear], ch );
            if ( can_see_obj( ch, obj ) ){
              send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
              send_to_char( "\n\r", ch );
            }else{
              send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
          }
          if (iWear == WEAR_LEGS) {
            if ( obj = get_eq_char( victim, WEAR_LEGS_2 ) ) {
              send_to_char( where_name[WEAR_LEGS_2], ch );
              if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch );
              }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
              }
              found = TRUE;
            } 
          } else if (iWear == WEAR_FEET) {
            if ( obj = get_eq_char( victim, WEAR_FEET_2 ) ) {
              send_to_char( where_name[WEAR_FEET_2], ch );
              if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch );
              }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
              }
              found = TRUE;
            } 
          }
        }
		// ¼uÃÄ¸Ë³Æ¦ì¸m add at 2022/04/30
		if ( obj = get_eq_char( victim, WEAR_AMMO ) ) {
			send_to_char( where_name[WEAR_AMMO], ch );
			if ( can_see_obj( ch, obj ) ){
				send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
				send_to_char( "\n\r", ch );
			}else{
				send_to_char( "¤£©úª««~¡C\n\r", ch );
			}
			found = TRUE;
		}
        if ( !( obj = get_eq_char( victim, WEAR_RIDE ) ) )
            ;
        else{
            send_to_char( where_name[WEAR_RIDE],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
    }else{ // Code for Beholder
        for ( iWear = 0; iWear <= WEAR_FINGER_R; iWear++ ){
            if ( !( obj = get_eq_char( victim, iWear ) ) )
                continue;

            send_to_char( where_name[iWear], ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch );
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
        for ( iWear = WEAR_FINGER_3; iWear <= WEAR_FINGER_4; iWear++ ){
            if ( !( obj = get_eq_char( victim, iWear ) ) )
                continue;

            send_to_char( where_name[iWear], ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch );
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
        for ( iWear = WEAR_NECK_1; iWear <= WEAR_HANDS; iWear++ ){
            if ( !( obj = get_eq_char( victim, iWear ) ) )
                continue;

            send_to_char( where_name[iWear], ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch );
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
        if ( ( obj = get_eq_char( victim, WEAR_HANDS_2 ) ) ){
            send_to_char( where_name[WEAR_HANDS_2],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
        if ( ( obj = get_eq_char( victim, WEAR_ARMS ) ) ){
            send_to_char( where_name[WEAR_ARMS],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
        if ( ( obj = get_eq_char( victim, WEAR_ARMS_2 ) ) ){
            send_to_char( where_name[WEAR_ARMS_2],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
        for ( iWear = WEAR_SHIELD; iWear < MAX_WEAR; iWear++ ){
            if ( !( obj = get_eq_char( victim, iWear ) ) )
                continue;

            send_to_char( where_name[iWear], ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch );
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
		// ¼uÃÄ¸Ë³Æ¦ì¸m add at 2022/04/30
		if ( obj = get_eq_char( victim, WEAR_AMMO ) ) {
			send_to_char( where_name[WEAR_AMMO], ch );
			if ( can_see_obj( ch, obj ) ){
				send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
				send_to_char( "\n\r", ch );
			}else{
				send_to_char( "¤£©úª««~¡C\n\r", ch );
			}
			found = TRUE;
		}
        //check WEAR_RIDE
        if ( ( obj = get_eq_char( victim, WEAR_RIDE ) ) ){
            send_to_char( where_name[WEAR_RIDE],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "¤£©úª««~¡C\n\r", ch );
            }
            found = TRUE;
        }
    }
    if ( !found )
        send_to_char( "¤°»ò¤]¨S¦³¡C\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    char     *msg;
    char      arg1 [ MAX_INPUT_LENGTH ];
    char      arg2 [ MAX_INPUT_LENGTH ];
    int       value1;
    int       value2;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
  send_to_char( "[33m­n¤ñ¸û¤°»ò¸ò¤°»ò¡H[m\n\r", ch );
  return;
    }

    if ( !( obj1 = get_obj_carry( ch, arg1 ) ) )
    {
  send_to_char( "[33m§A¨S¦³¨º¼ËªF¦è¡C[m\n\r", ch );
  return;
    }

    if ( arg2[0] == '\0' )
    {
  for ( obj2 = ch->carrying; obj2; obj2 = obj2->next_content )
  {
      if ( obj2->wear_loc != WEAR_NONE
    && can_see_obj( ch, obj2 )
    && obj1->item_type == obj2->item_type
    && ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
    break;
  }

  if ( !obj2 )
  {
      send_to_char( "[33m§A¨S¦³¬ïµÛ¥i¥H®³¨Ó°µ¬°¤ñ¸ûªºªF¦è¡C[m\n\r", ch );
      return;
  }
    }
    else
    {
  if ( !( obj2 = get_obj_carry( ch, arg2 ) ) )
  {
      char new_arg2 [ MAX_INPUT_LENGTH ];
      int  number;

      /*  Strip off number argument, subtract one, paste it together */
      number = number_argument( arg2, arg2 );
      if ( number > 1 )  number--;
      sprintf( new_arg2, "%d.%s", number, arg2 );

      if ( !( obj2 = get_obj_wear( ch, new_arg2 ) ) )
      {
    send_to_char( "[33m§A¨S¦³¨º¼ËªF¦è¡C[m\n\r", ch );
    return;
      }
  
      if ( ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE ) == 0 )
      {
    send_to_char( "[33m³o¤£¬O¥i¥H®³¨Ó¤ñ¸ûªºªF¦è¡C[m\n\r", ch );
    return;
      }

  }
    }
      
    msg          = NULL;
    value1    = 0;
    value2    = 0;

    if ( obj1 == obj2 )
    {
  msg = "[33m§A§â$p©M¥¦¦Û¤v®³¨Ó¤ñ¸û¡C¥¦­Ì¬Ý°_¨Ó®t¤£¦h¬O¤@¼Ëªº¡C[m";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
  msg = "[33m§A¤£¯à®³$P¨Ó¸ò$p¤ñ¸û¡C[m";
    }
    else
    {
  switch ( obj1->item_type )
  {
  default:
      msg = "[33m$p©M$P¤£¯à®³¨Ó¤ñ¸û¡C[m";
      break;

  case ITEM_ARMOR:
      value1 = obj1->value[0];
      value2 = obj2->value[0];
      break;

  case ITEM_WEAPON:
      value1 = obj1->value[1] + obj1->value[2];
      value2 = obj2->value[1] + obj2->value[2];
      break;
  }
    }

    if ( !msg )
    {
  if ( obj2->wear_loc != WEAR_NONE )
  {
     if ( value1 == value2 )
         msg = "[36m$p©M$P¡]¸Ë³Æ¦b¨­¤Wªº¡^¬Ý°_¨Ó®t¤£¦h¬O¤@¼Ëªº¡C[m";
     else if ( value1  > value2 )
         msg = "[36m$p¬Ý°_¨Ó¤ñ$P¡]¸Ë³Æ¦b¨­¤Wªº¡^­n¨Ó±o¦n¡C[m";
      else
         msg = "[36m$p¬Ý°_¨Ó¤ñ$P¡]¸Ë³Æ¦b¨­¤Wªº¡^ÁÙ¨Ó±oÄê¡C[m";
  }
  else
  {
       if ( value1 == value2 ) msg = "[36m$p©M$P¬Ý°_¨Ó®t¤£¦h¬O¤@¼Ëªº¡C[m";
  else if ( value1  > value2 ) msg = "[36m$p¬Ý°_¨Ó¤ñ$P­n¨Ó±o¦n¡C[m";
  else           msg = "[36m$p¬Ý°_¨Ó¤ñ$PÁÙ¨Ó±oÄê¡C[m";
  }

    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA     *victim;
    DESCRIPTOR_DATA *d;
    char       buf [ MAX_STRING_LENGTH ];
    char       arg [ MAX_INPUT_LENGTH  ];
    bool       found;

    if ( !check_blind( ch ) )
  return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
  send_to_char( "[36m¦b§Aªþªñªº¦³¡G[m\n\r", ch );
  found = FALSE;
  for ( d = descriptor_list; d; d = d->next )
  {
      if ( d->connected == CON_PLAYING
    && ( victim = d->character )
    && !IS_NPC( victim )
    && victim->in_room
    && victim->in_room->area == ch->in_room->area
    && can_see( ch, victim ) )
      {
    found = TRUE;
    sprintf( buf, "%-18s %s\n\r",
      victim->short_descr, victim->in_room->name );
    send_to_char( buf, ch );
      }
  }
  if ( !found )
      send_to_char( "[33mµL[m\n\r", ch );
    }
    else
    {
  found = FALSE;
  for ( victim = char_list; victim; victim = victim->next )
  {
      if ( !victim->in_room
    || IS_AFFECTED( victim, AFF_HIDE ) 
    || IS_AFFECTED( victim, AFF_SNEAK ) )
    continue;

      if ( victim->in_room->area == ch->in_room->area
    && can_see( ch, victim )
    && is_pfxname( arg, victim->name ) )
      {
    found = TRUE;
    sprintf( buf, "%-28s %s\n\r", PERS( victim, ch ),
      victim->in_room->name );
    send_to_char( buf, ch );
    break;
      }
  }
  if ( !found )
      act( "[33m§A§ä¤£¨ì¤°»ò$T¡C[m", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char *buf = '\0';
	char *msg;
	char arg [ MAX_INPUT_LENGTH ];
	char buf1[ MAX_INPUT_LENGTH ];
	int diff;
	int hpdiff;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "[33m­n¦Ò¶q±þ½Ö¡H[m\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "[33m¥L¤£¦b³o¨à¡C[m\n\r", ch );
		return;
	}

	if ( !IS_NPC( victim ) && !IS_SET( victim->act, PLR_KILLER ) )
	{
		send_to_char( "[33m·Q·í±þ¤H¥Ç¡H¦Ò¼{³o»ò¦h·F¹À¡A·F¤U¥h(murder)´N¹ï¤F¡I[m\n\r", ch );
		return;
	}

	diff = victim->level - ch->level;

	if (      diff <= -20 ) msg = HIG"§A¤£¬ï¨¾¨ã¤]¥i¥HªÅ¤â°µ±¼$N¡C"NOR;
	else if ( diff <= -10 ) msg = HIG"$N¤£­È±o§A¥h±þ¥L¡C"NOR;
	else if ( diff <=  -5 ) msg = HIG"$N¬Ý°_¨Ó«Ü¦n¸Ñ¨M¡C"NOR;
	else if ( diff <=   5 ) msg = HIY"´Ñ³{¼Ä¤â¡I"NOR;
	else if ( diff <=  10 ) msg = HIY"§A»Ý­n¤@ÂI¹B®ð¤~¯à¥´±Ñ$N"NOR;
	else if ( diff <=  20 ) msg = HIR"$N²@¤£¯d±¡¦a¹ï§A¤j¯º¡C"NOR;
	else                    msg = HIR"¦º¯«·|·PÁÂ§AªºÂ§ª«¡C"NOR;

	act( msg, ch, NULL, victim, TO_CHAR );

	/* additions by amenda at 4/20/02 */
	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r¥L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r¦o" );
	else
		sprintf( buf1, "\n\r¥¦" );

	if (      get_curr_size( victim ) >= 9 ) strcat( buf1, "ªºÅé«¬¸ò®£Às®t¤£¦h¤j¡C" );
	else if ( get_curr_size( victim ) >= 8 ) strcat( buf1, "ªºÅé«¬¹³¤@ÀY­¸Às¡C" );
	else if ( get_curr_size( victim ) >= 7 ) strcat( buf1, "ªºÅé«¬¯à»P¥¨¤H·B¬ü¡C" );
	else if ( get_curr_size( victim ) >= 6 ) strcat( buf1, "ªº¨­Åé»P¥¨Å]®t¤£¦h¤j¡C¡A" );
	else if ( get_curr_size( victim ) >= 5 ) strcat( buf1, "¸ò¦«´µ°¨§JªºÀJ¹³®t¤£¦h¤j¡C" );
	else if ( get_curr_size( victim ) >= 4 ) strcat( buf1, "¤ñ¤@¯ë¦¨¦~¤H°ª¤j¤@¨Ç¡C" );
	else if ( get_curr_size( victim ) >= 3 ) strcat( buf1, "¨S¯S§O°ª¤j¡A»P¤HÃþ®t¤£¦h¡C" );
	else if ( get_curr_size( victim ) >= 2 ) strcat( buf1, "¸G¤pªº¨­Åé¡A¹³­Ó¥b¨­¤H¡C" );
	else if ( get_curr_size( victim ) >= 1 ) strcat( buf1, "¦p¦P¤p§¯ºë¤@¼Ë¡A¼b¤p¬ÂÄn¡C" );
	else                                     strcat( buf1, "¦ü¥G¤£¦s¦b¥bÂI½è¶q¡A¸òªÅ®ð®t¤£¦h¡I" );
	send_to_char( buf1, ch );

	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r¥L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r¦o" );
	else
		sprintf( buf1, "\n\r¥¦" );

	if (      GET_DODGE( victim ) >=   200 ) strcat( buf1, "¦æ°Ê°_¨Ó®Ú¥»¹³¤@¶ô©¥¥Û¡C" );
	else if ( GET_DODGE( victim ) >=     0 ) strcat( buf1, "°Ê§@¹³½wºC¦Óµh­W¦aª¦¦æ¡C" );
	else if ( GET_DODGE( victim ) >=   -50 ) strcat( buf1, "°Ê§@¤ñ½½¤û§Ö¤£¤F¦h¤Ö¡C" );
	else if ( GET_DODGE( victim ) >=  -150 ) strcat( buf1, "°Ê§@²×©ó¾Ç·|¨«¸ô¡C" );
	else if ( GET_DODGE( victim ) >=  -300 ) strcat( buf1, "¦æ°Ê³t«×¤£¿ù¡C" );
	else if ( GET_DODGE( victim ) >=  -500 ) strcat( buf1, "¦æ°Ê«D±`±Ó±¶¡C" );
	else if ( GET_DODGE( victim ) >=  -700 ) strcat( buf1, "°Ê¦p²æ¨ß¡A¤@¬ª¤d¨½¡C" );
	else if ( GET_DODGE( victim ) >=  -950 ) strcat( buf1, "±Ó±¶±o´N¹³¤@°¦ÂG³À¡C" );
	else if ( GET_DODGE( victim ) >= -1000 ) strcat( buf1, "³t«×§Ö±o§A¬Ý¤£¨ì¡C" );
	else if ( GET_DODGE( victim ) >= -1200 ) strcat( buf1, "³t«×®Ú¥»´N¹³¤@°}±Û­·¡C" );
	else if ( GET_DODGE( victim ) >= -1400 ) strcat( buf1, "³t«×§Ö¨ì¥i¥H°l¤W¦Û¤vªº¼v¤l¡C" );
	else                                     strcat( buf1, ""HIR"³t«×§Ö¨ì¤w¸g°l¤W¦Û¤vªº¼v¤l¤F¡I¡I"NOR"");
	send_to_char( buf1, ch );


	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r¥L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r¦o" );
	else
		sprintf( buf1, "\n\r¥¦" );

	if (      GET_AC( victim ) >=    300 ) strcat( buf1, "¨­¤Wªº¬ïµÛ¤ñ»rÅéªºÁÙ¸Ø±i¡I"  );
	else if ( GET_AC( victim ) >=    100 ) strcat( buf1, "¸òÞóÅé¨S¤°»ò¨â¼Ë¡C" );
	else if ( GET_AC( victim ) >=      0 ) strcat( buf1, "¥u¬ïµÛ¤º¦ç¤º¿Ç¡C" );
	else if ( GET_AC( victim ) >=   -400 ) strcat( buf1, "¬ïµÛ´¶³qªº¦çªA¡C" );
	else if ( GET_AC( victim ) >=   -500 ) strcat( buf1, "¦h¤Ö¦³ÂI¸Ë³Æ¡C" );
	else if ( GET_AC( victim ) >=  -1000 ) strcat( buf1, "¸Ë³Æ°_¨Ó¡C" );
	else if ( GET_AC( victim ) >=  -1500 ) strcat( buf1, "ªº¸Ë³Æ¤£¿ù¡C" );
	else if ( GET_AC( victim ) >=  -2000 ) strcat( buf1, "ªº¸Ë³Æ«Ü¦n¡C" );
	else if ( GET_AC( victim ) >=  -2500 ) strcat( buf1, "¬ïµÛ­««¬¸Ë³Æ¡C" );
	else if ( GET_AC( victim ) >=  -3000 ) strcat( buf1, "¬ïµÛ¶W¯Åªº¸Ë³Æ¡C" );
	else if ( GET_AC( victim ) >=  -3500 ) strcat( buf1, "ªº¸Ë³Æ¥uÀ³¤Ñ¤W¦³¡C" );
	else if ( GET_AC( victim ) >=  -4000 ) strcat( buf1, "ªº¸Ë³Æ¤Ñ¤UµL¼Ä¡C" );
	else if ( GET_AC( victim ) >=  -5000 ) strcat( buf1, "¬ïµÛ¶W¯ÅµL¼Ä¶Àª÷ÅRÆEªº¸Ë³Æ¡C" );
	else                                   strcat( buf1, ""HIR"ªº¸Ë³Æ¤Ó¸Ø±i¤F¡A¤p¤ßÂI¡A©Î³\¬O¥Î bug¡I"NOR""  );
	send_to_char( buf1, ch );

	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r¥L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r¦o" );
	else
		sprintf( buf1, "\n\r¥¦" );

	if (      get_damroll( victim, WEAR_WIELD ) <  50 ) strcat( buf1, "¬Ý¨Ó²@µL¯}Ãa¤O¡A" );
	else if ( get_damroll( victim, WEAR_WIELD ) < 100 ) strcat( buf1, "ªºªZ¸Ë«j±j¯à³y¦¨¤@¨Ç¶Ë®`¡A" );
	else if ( get_damroll( victim, WEAR_WIELD ) < 150 ) strcat( buf1, "¦ü¥GÆZ±j§§¡A" );
	else if ( get_damroll( victim, WEAR_WIELD ) < 200 ) strcat( buf1, "¯à³y¦¨«Ü¤j¶Ë®`¡A" );
	else                                               strcat( buf1, ""HIR"¦ü¥G¯à¶}¸Oµõ¥Û¡I"NOR"" );

	send_to_char( buf1, ch );
	/* additions by king@tinuviel.cs.wcu.edu */
	hpdiff = ( ch->hit - victim->hit );

	if ( ( ( diff >= 0) && ( hpdiff <= 0 ) ) || ( ( diff <= 0 ) && ( hpdiff >= 0 ) ) )
	{
		send_to_char( "¦Ó¥B", ch );
	}
	else
	{
		send_to_char( "¤£¹L", ch );
	}

	if ( hpdiff >= 101 )
		buf = "§Aªº¥Í©R¤O¤ñ$E¦hº¡¦hªº¡C[m";
	if ( hpdiff <= 100 )
		buf = "§Aªº¥Í©R¤O¤ñ$E¦h¡C[m";
	if ( hpdiff <= 50 ) 
		buf = "§Aªº¥Í©R¤O¤ñ$Eµy¦h¡C[m";
	if ( hpdiff <= 25 )
		buf = "§Aªº¥Í©R¤O¤ñ$E¦h¤@ÂIÂI¡C[m";
	if ( hpdiff <= 0 )
		buf = "$Eªº¥Í©R¤O¤ñ§A¦h¤@ÂIÂI¡C[m";
	if ( hpdiff <= -25 )
		buf = "$Eªº¥Í©R¤O¤ñ§Aµy¦h¡C[m";
	if ( hpdiff <= -50 )
		buf = "$Eªº¥Í©R¤O¤ñ§A¦h¡C[m";
	if ( hpdiff <= -100 )
		buf = "$Eªº¥Í©R¤O¤ñ§A¦hº¡¦hªº¡C[m";

	act( buf, ch, NULL, victim, TO_CHAR );
	return;
}


void set_hname( CHAR_DATA *ch, char *cname )
{
    char buf [ BUF_STRING_LENGTH ];
    char buf1 [ BUF_STRING_LENGTH ];

    buf[0] = '\0';

    strcpy( buf, cname );
    free_string( ch->pcdata->horse->name );
    ch->pcdata->horse->name = str_dup( buf );

    sprintf( buf1, "%s(%s)", ch->pcdata->horse->cname, ch->pcdata->horse->name );
    free_string( ch->pcdata->horse->short_descr );
    ch->pcdata->horse->short_descr = str_dup( buf1 );

    sprintf( buf1, "%s(%s)¨Ä¨Ä¦a¯¸¦b³o¸Ì¡C\n\r", ch->pcdata->horse->cname, ch->pcdata->horse->name );
    free_string(ch->pcdata->horse->long_descr);
    ch->pcdata->horse->long_descr = str_dup(buf1);
    /*
    if ( IS_NPC( ch ) ) {
  free_string( ch->pIndexData->short_descr );
  ch->pIndexData->short_descr=str_dup( buf );
    }
    */
    return;
}

void set_cname( CHAR_DATA *ch, char *cname )
{
    char buf [ BUF_STRING_LENGTH ];
    char buf1 [ BUF_STRING_LENGTH ];

    buf[0] = '\0';

    strcpy( buf, cname );
    free_string( ch->cname );
    ch->cname = str_dup( buf );

    sprintf( buf1, "%s(%s)", cname, ch->name );
    free_string( ch->short_descr );
    ch->short_descr = str_dup( buf1 );

  if ( IS_SET( ch->act, PLR_HORSE ) )
  {
    sprintf( buf, "%s(%s)¨Ä¨Ä¦a¯¸¦b³o¸Ì¡C\n\r", ch->cname, ch->name );
    free_string(ch->long_descr);
    ch->long_descr = str_dup(buf);
  }

    if ( IS_NPC( ch ) ) {
  free_string( ch->pIndexData->short_descr );
  ch->pIndexData->short_descr=str_dup( buf );
    }
    return;
}


void do_hname( CHAR_DATA *ch, char *arg )
{
    char argument[MAX_INPUT_LENGTH];
    if ( IS_NPC( ch ) )
  return;
    one_argument(arg, argument);

    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m­n§â°¨ªº¦W¦r§ï¦¨¤°»ò¡H[m\n\r", ch );
  return;
    }

    if( !ch->pcdata->horse ) {
  send_to_char(YEL "§A¨S¦³°¨¡C"NOR"\n\r", ch);
  return;
    }

    if( IS_SET(ch->act, PLR_HORSE ) ){
  send_to_char(YEL "°¨¤£¯àÀ°¦Û¤v¨ú¦W¦r¡C"NOR"\n\r", ch);
  return;
    }

    if ( strlen( argument ) > 21 )
    {
  argument[21] = '\0';
  send_to_char(YEL "§Aªº°¨¦W¦r¤Óªø¤F¡A½Ð¨ú¤@­Óµu¤@ÂIªº¡C"NOR"\n\r", ch);
  return;
    }
     if( !check_parse_name( argument ) ) {
         send_to_char("¨º¬O¤£¦Xªkªº¦W¦r¡A½Ð­«·s¨ú¤@­Ó¡C\n\r", ch);
         return;
     }

    if ( !str_prefix( argument, ch->name ) || !str_prefix( ch->name, argument) )
    {
        send_to_char( "°¨ªº¦W¦r¤£¯à©M¥D¤H¤Ó¹³¡C\r\n", ch );
        return;
    }

    if ( !str_infix( argument, ch->name ) || !str_infix( ch->name, argument) )
    {
        send_to_char( "°¨ªº¦W¦r¤£¯à©M¥D¤H¤Ó¹³¡C\r\n", ch );
        return;
    }
    


    if( has_player( argument ) ){
  send_to_char(YEL "¤£¯à¥Î§Oªºª±ªÌªº¦W¦r¡C"NOR"\n\r", ch);
  return;
    }


    smash_tilde( argument );
    set_hname( ch, argument );
    send_to_char( "[36mOK.[m\n\r", ch );
}

void do_cname( CHAR_DATA *ch, char *argument )
{
    char no_ansi [ MAX_STRING_LENGTH ];
    int len, len2;

    if ( IS_NPC( ch ) )
       return;

    strip_ansi( argument, no_ansi );

    for(len = 0;len < 120;len++)
    {
       if(no_ansi[len]=='\0' )
    	   break;
    }
 
    for(len2 = 0; len2 <120;len2++)
    {
       if(argument[len2]=='\0' )
    	   break;
    }

    if (len == 0 )
    {
        send_to_char( "[33m­n§â§Aªº¤¤¤å¦W¦r§ï¦¨¤°»ò¡H[m\n\r", ch );
        return;
    }
    else if(len > 12 || len2 > 100)
    {
  	argument[12] = '\0';
	send_to_char(YEL "§Aªº¦W¦r¤Óªø¤F¡A½Ð¨ú¤@­Óµu¤@ÂIªº¡C"NOR"\n\r", ch);
	return;
       
    }

    smash_tilde( argument );
    set_cname( ch, argument );
    send_to_char( "[36mOK.[m\n\r", ch );
}

int str_length(char *str)
{
    int len = 0;
    int i, k;
    len = strlen( str );
    k = len;
    for( i = 0; i < len; i++ )
    {
        if( str[i] == '' ||
            str[i] == '[' ||
            str[i] == 'm' ||
            str[i] == ';' ||
            str[i] == '0' ||
            str[i] == '1' ||
            str[i] == '2' ||
            str[i] == '3' ||
            str[i] == '4' ||
            str[i] == '5' ||
            str[i] == '6' ||
            str[i] == '7' ||
            str[i] == '8' ||
            str[i] == '9' )
        {
            k--;
        }
    }
    return k;
}

void set_title( CHAR_DATA *ch, char *title )
{
    char buf [ BUF_STRING_LENGTH ];

    if ( IS_NPC( ch ) )
    {
  bug( "Set_title: NPC.", 0 );
  return;
    }

    buf[0] = '\0';

    /*
    if ( isalpha( title[0] ) || isdigit( title[0] ) )
    {
  buf[0] = ' ';
  strcpy( buf+1, title );
    }
    else
    {*/
  strcpy( buf, title );
   // }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
  return;

    if ( argument[0] == '\0' )
    {
  send_to_char( "[33m­n§â§AªººÙ¸¹§ï¦¨¤°»ò¡H[m\n\r", ch );
  return;
    }

    if ( str_length( argument ) > 30 )
    {
  argument[30] = '\0';
  send_to_char(YEL "§AªººÙ¸¹¤Óªø¤F¡A½Ð·Q¤@­Óµu¤@ÂIªº¡AÅT«G¤@ÂIªº¡C"NOR"\n\r", ch);
    }

    smash_tilde( argument );
    set_title( ch, argument );
    send_to_char( "[36mOK.[m\n\r", ch );
}



void do_description( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];

	if ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || IS_SET( ch->act, ACT_PET ) )
	return;

	if ( argument[0] != '\0' )
	{
		buf[0] = '\0';
		smash_tilde( argument );
		if ( argument[0] == '+' )
		{
			if ( strlen( buf ) + strlen( argument ) >=  500 )
			{
				send_to_char( "[33m´y­z¤Óªø¤F¡C[m\n\r", ch );
				return;
			}
			if ( ch->description )
				strcat( buf, ch->description );
			argument++;
			while ( isspace( *argument ) )
			argument++;
		}

		if ( strlen( buf ) + strlen( argument ) >=  500 )
		{
			send_to_char( "[33m´y­z¤Óªø¤F¡C[m\n\r", ch );
			return;
		}

		strcat( buf, argument );
		strcat( buf, "\n\r" );
		free_string( ch->description );
		ch->description = str_dup( buf );
	}

    send_to_char( "§Aªº´y­z¬O¡G\n\r", ch );
    send_to_char( ch->description ? ch->description : "¡]µL¡^¡C\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    char buf [ MAX_INPUT_LENGTH ];

    sprintf( buf,
      "[36m§Aªºª¬ºA¡G%d/%dÂI¥Í©R¤O¡B%d/%dÂIÅ]ªk¤O¡B%d/%dÂI²¾°Ê¤O¡B%dÂI¸gÅç­È¡C[m\n\r",
      ch->hit,  ch->max_hit,
      ch->mana, ch->max_mana,
      ch->move, ch->max_move,
      ch->exp );

    send_to_char( buf, ch );

    sprintf( buf,
      "[36m$n³ø§iª¬ºA¡G%d/%dÂI¥Í©R¤O¡B%d/%dÂIÅ]ªk¤O¡B%d/%dÂI²¾°Ê¤O¡B%dÂI¸gÅç­È¡C[m\n\r",
      ch->hit,  ch->max_hit,
      ch->mana, ch->max_mana,
      ch->move, ch->max_move,
      ch->exp );

    act( buf, ch, NULL, NULL, TO_ROOM );

    return;
}


// practice modified by jye 6.07.2005
void do_practice( CHAR_DATA *ch, char *argument )
{
	char buf  [ BUF_STRING_LENGTH   ];
	char buf1 [ MAX_STRING_LENGTH*4 ];
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	char *temp;
	int req_lv;

	int  sn;
	int  money = ch->level * ch->level * 20;
	bool can_learn;

	if( IS_NPC( ch ) )
		return;

	if( ch->class < 5 ) 
		money /= 2;

	temp = argument;
	temp = one_argument(temp, arg1);
	one_argument(temp, arg2);

	buf1[0] = '\0';

	if( argument[0] == '\0' )
	{
		CHAR_DATA *mob;
		int col;

		for( mob = ch->in_room->people; mob; mob = mob->next_in_room )
		{
			if( mob->deleted )
				continue;

			if( IS_NPC( mob ) && IS_SET( mob->act, ACT_PRACTICE ) &&( mob->psk ) )
				break;
		}

		if( !mob || !mob->psk)
		{
			send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
			return;
		}

		col    = 0;

		for( sn = 0; sn < MAX_SKILL; sn++ )
		{
			can_learn = FALSE;
			if( !skill_table[sn].name )
				break;
			// if ( ch->pcdata->learned[sn] == 0 )
			// continue;
			if( skill_req_level( ch, sn ) == L_APP )
				continue;
			if( skill_req_level( ch, sn ) == -1 && skill_preq_level( ch, sn ) == L_APP )
				continue;

			if( is_skname( skill_table[sn].name, mob->psk) != -1 )
				can_learn = TRUE;

			sprintf( buf, HIW"%c"NOR"%14s(%16s) %3d%% ", can_learn ? '+' : ' ', skill_table[sn].cname, skill_table[sn].name, get_skill_percent(ch, sn));
			strcat( buf1, buf );
			if( ++col % 2 == 0 )
				strcat( buf1, "\n\r" );
		}

		if( col % 3 != 0 )
			strcat( buf1, "\n\r" );

		sprintf( buf, "§AÁÙ¦³%dÂI½m²ßÂI¼Æ¡C\n\r",ch->practice );
		strcat( buf1, buf );
		sprintf( buf, "½m²ßªº¶O¥Î¬O%dªTª÷¹ô¡C\n\r", money );
		strcat( buf1, buf );
		send_to_char( buf1, ch );
	}
	else if (arg2[0] == '\0')
	{
		CHAR_DATA *mob;
		QMARK *qmark;
		int adept;

		if( ch->level < 3 )
		{
			send_to_char("[33m§A±o­n¦b¤T¯Å¥H¤W¤~¯à½m²ß(practice)¡A¥ý¥h°V½m(train)§a¡I[m\n\r", ch );
			return;
		}

		if( !IS_AWAKE( ch ) )
		{
			send_to_char( "[33m¬O­n¦b¹Ú¸Ì½m²ß¶Ü¡H[m\n\r", ch );
			return;
		}

		for( mob = ch->in_room->people; mob; mob = mob->next_in_room )
		{
			if( mob->deleted )
				continue;

			if( IS_NPC( mob ) && IS_SET( mob->act, ACT_PRACTICE ) )
				break;
		}

		if( !mob || !mob->psk)
		{
			send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
			return;
		}

		if( ch->practice <= 0 )
		{
			send_to_char( "[33m§A¨S¦³½m²ßÂI¼Æ¤F¡C[m\n\r", ch );
			return;
		}
		else if ( money > ch->gold && ch->class > 4)
		{
			send_to_char( "[33m§A¥I¤£°_½m²ßªº¶O¥Î¡C[m\n\r", ch );
			return;
		}

		if(( sn = skill_lookup( argument ) ) < 0 || ( ( skill_req_level( ch, sn ) > -1 && ch->level < skill_req_level(ch, sn))))
		{
			send_to_char( "[33m§A¤£¯à½m²ß¨º­Ó¡C[m\n\r", ch );
			return;
		}

		if( skill_req_level(ch, sn) == -1 )
		{
			//req_lv = skill_table[sn].skill_level[apply_class[ch->pcdata->prime_attr]];
			req_lv = skill_preq_level(ch, sn);
			if( req_lv == L_APP ) 
			{
				;
			}
			else 
				req_lv = req_lv / 2 + 1;

			if( ch->level < req_lv ) 
			{
				send_to_char( "[33m§A¤£¯à½m²ß¨º­Ó¡C[m\n\r", ch );
				return;
			}
		}

		switch( is_skname( argument, mob->psk ) )
		{
			case -1 :
				act( "$n »¡¡G¹ï¤£°_¡A§Ú¨Ã¤£·|¨º¶µ§Þ¥©¡C", mob, NULL, ch, TO_VICT );
				return;
			case 1 :
				for( qmark = ch->qmark; qmark; qmark = qmark->next )
					if ( !str_cmp( qmark->mark_name, skill_table[sn].name ) )
						break;
				if ( !qmark )
				{
					act( "$n »¡¡G¹ï¤£°_¡A§AÁÙ¤£°÷¸ê®æ¾Ç²ß³o¶µ§Þ¥©¡C", mob, NULL, ch, TO_VICT );
					return;
				}
				break;
			default :
				break;
		}

		//adept = IS_NPC( ch ) ? 100 : class_table[ch->class].skill_adept;
		adept = skill_max_percent(ch, sn);

		if( //ch->pcdata->learned[sn] >= adept )
		get_skill_percent(ch, sn) >= adept )
		{
			sprintf( buf, "[33m§A¤w¸gµLªk´£¤É%s(%s)ªº¼ô½m«×¤F¡C[m\n\r", skill_table[sn].cname, skill_table[sn].name );
			send_to_char( buf, ch );
		}
		else
		{
			ch->practice--;
			if( ch->class > 4 )
				ch->gold -= money;
			gain_skill_percent(ch, sn, int_app[get_curr_int( ch )].learn);

			if( get_skill_percent( ch, sn ) < adept )
			{
				act( "[36m§A½m²ß¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n½m²ß¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
			}
			else
			{
				act( "[36m§A²{¦b¬O$t($T)ªº±M®a¤F¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n²{¦b¬O$t($T)ªº±M®a¤F¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
			}
		}
	}
	else  //prac skill tutor
	{
		CHAR_DATA *mob;
		QMARK *qmark;
		int    adept;

		for( mob = ch->in_room->people; mob; mob = mob->next_in_room )
		{
			if( mob->deleted )
				continue;

			if( IS_NPC( mob ) && IS_SET( mob->act, ACT_PRACTICE ) && is_pfxname(arg2, mob->name))
				break;
		}

		if( !mob || !IS_SET( mob->act, ACT_PRACTICE ))
		{
			send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
			return;
		}

		if( !mob->psk ) 
		{
			send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
			return;
		}
		if( ch->practice <= 0 )
		{
			send_to_char( "[33m§A¨S¦³½m²ßÂI¼Æ¤F¡C[m\n\r", ch );
			return;
		}

		if( !str_cmp( arg1, "+") ) 
		{
			int col;

			col = 0;

			for( sn = 0; sn < MAX_SKILL; sn++ )
			{
				can_learn = FALSE;
				if( !skill_table[sn].name )
					break;
				if( skill_req_level(ch, sn) == L_APP )
					continue;
				if( skill_req_level( ch, sn) == -1 && skill_preq_level(ch, sn) == L_APP)
					continue;

				if( is_skname( skill_table[sn].name, mob->psk) != -1 )
					can_learn = TRUE;

				sprintf( buf, HIW"%c"NOR"%14s(%16s) %3d%% ", can_learn ? '+' : ' ', skill_table[sn].cname, skill_table[sn].name, get_skill_percent(ch, sn));
				strcat( buf1, buf );
				if ( ++col % 2 == 0 )
					strcat( buf1, "\n\r" );
			}

			if( col % 3 != 0 )
				strcat( buf1, "\n\r" );

			sprintf( buf, "§AÁÙ¦³%dÂI½m²ßÂI¼Æ¡C\n\r", ch->practice );
			strcat( buf1, buf );
			sprintf( buf, "½m²ßªº¶O¥Î¬O%dªTª÷¹ô¡C\n\r", money );
			strcat( buf1, buf );
			send_to_char( buf1, ch );
			return;
		}
		else if ( money > ch->gold && ch->class > 4 )
		{
			send_to_char( "[33m§A¥I¤£°_½m²ßªº¶O¥Î¡C[m\n\r", ch );
			return;
		}

		if(( sn = skill_lookup( arg1 ) ) < 0 || ( ( skill_req_level(ch, sn) > -1 && ch->level < skill_req_level(ch, sn))))
		{
			send_to_char( "[33m§A¤£¯à½m²ß¨º­Ó¡C[m\n\r", ch );
			return;
		}
		if( skill_req_level(ch, sn) == -1 ) 
		{
			req_lv = skill_preq_level(ch, sn);
			if( req_lv == L_APP ) 
			{
				;
			}
			else 
				req_lv = req_lv / 2 + 1;

			if( ch->level < req_lv ) 
			{
				send_to_char( "[33m§A¤£¯à½m²ß¨º­Ó¡C[m\n\r", ch );
				return;
			}
		}

		switch( is_skname( arg1, mob->psk ) )
		{
			case -1 :
				act( "$n »¡¡G¹ï¤£°_¡A§Ú¨Ã¤£·|¨º¶µ§Þ¥©¡C", mob, NULL, ch, TO_VICT );
				return;
			case 1 :
				for( qmark = ch->qmark; qmark; qmark = qmark->next )
					if ( !str_cmp( qmark->mark_name, skill_table[sn].name ) )
						break;
				if ( !qmark )
				{
					act( "$n »¡¡G¹ï¤£°_¡A§AÁÙ¤£°÷¸ê®æ¾Ç²ß³o¶µ§Þ¥©¡C", mob, NULL, ch, TO_VICT );
					return;
				}
				break;
			default :
				break;
		}

		//adept = IS_NPC( ch ) ? 100 : class_table[ch->class].skill_adept;
		adept = skill_max_percent(ch, sn);

		if ( get_skill_percent(ch, sn) >= adept )
		{
			sprintf( buf, "[33m§A¤w¸gµLªk´£¤É%s(%s)ªº¼ô½m«×¤F¡C[m\n\r", skill_table[sn].cname, skill_table[sn].name );
			send_to_char( buf, ch );
		}
		else
		{
			ch->practice--;
			if( ch->class > 4 )
				ch->gold -= money;
			gain_skill_percent( ch, sn, int_app[get_curr_int( ch )].learn);
			if( get_skill_percent(ch, sn) < adept )
			{
				act( "[36m§A½m²ß¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n½m²ß¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
			}
			else
			{
				act( "[36m§A²{¦b¬O$t($T)ªº±M®a¤F¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n²{¦b¬O$t($T)ªº±M®a¤F¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
			}
		}
	}
	return;
}
/* return -1 : mob don't teach this skill
 * return  0 : mob teach this skill, and no need to check pc's qmark
 * return  1 : mob teach this skill, but need pc has corresponding qmark
 */
int is_skname( const char *str, char *namelist )
{
    char name [ MAX_INPUT_LENGTH ];
    char *nameptr;
    bool qualify;
    
    if( !namelist ) return FALSE;
    for ( ; ; )
    {
  qualify = FALSE;
  namelist = one_argument( namelist, name );
  if ( name[0] == '\0' )
      return -1;

  nameptr = name;
  if ( name[0] == '*' )
  {
    nameptr += 1;
    qualify = TRUE; 
  }

  if ( !str_prefix( str, nameptr ) )
      return qualify ? 1 : 0 ;
    }
}

void do_join( CHAR_DATA *ch, char *argument )
{
  char buf  [ BUF_STRING_LENGTH   ];
  char buf1 [ MAX_STRING_LENGTH*2 ];
  int complement [MAX_CLASS] = {
      3,4,2,1,5,
      3,3,4,4,2,
      2,1,1,1,5,
      3,4,2,5,5};
  int  cn;
  int  money = ch->level * ch->level * 40;

  if ( IS_NPC( ch ) || ch->class > 4 ){
    send_to_char("§A¤£»Ý­n¦AÂàÂ¾¤F¡C\n\r", ch);
    return;
  }

  buf1[0] = '\0';

  if ( ch->level < 20 )
  {
    send_to_char(
        "[33m§A±o­n¦b¤G¤Q¯Å¥H¤W¤~¯àÂàÂ¾(join)¡A¥ý³]ªk¤É¯Å§a¡I[m\n\r",
        ch );
    return;
  }

  if ( argument[0] == '\0' )
  {
    CHAR_DATA *mob;
    int    col;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
      if ( mob->deleted )
        continue;
      if ( IS_NPC( mob ) && ( mob->jcl ) )
        break;
    }

    if ( !mob )
    {
      send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
      return;
    }

    col = 0;    

    strcat( buf1, "§A¯à°÷¥[¤Jªº¶i¶¥Â¾·~¦³¡G\n\r" );
    for ( cn = 5; cn < MAX_CLASS; cn++ )
    {
      if ( !class_table[cn].name )
        break;
      if ( class_table[cn].bclass[ch->class] == L_APP )
        continue;

      if ( is_name( class_table[cn].name, mob->jcl) ) 
      {
        sprintf( buf, "%s ( %s ), ",
          class_table[cn].cname, class_table[cn].name );
        strcat( buf1, buf );
        if ( ++col % 4 == 0 )
          strcat( buf1, "\n\r" );
      }
    }

    if ( col % 4 != 0 )
      strcat( buf1, "\n\r" );

    sprintf( buf, "ÂàÂ¾©Ò»Ýªº¶O¥Î¬O%dªTª÷¹ô¡C\n\r", money );
    strcat( buf1, buf );
    send_to_char( buf1, ch );
  }
  else
  {
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    if ( !IS_AWAKE( ch ) )
    {
      send_to_char( "[33m¬O­n¦b¹Ú¸ÌÂàÂ¾¶Ü¡H[m\n\r", ch );
      return;
    }

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
      if ( mob->deleted )
        continue;
      if ( IS_NPC( mob ) && ( mob->jcl ) )
        break;
    }

    if ( !mob )
    {
      send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
      return;
    }

    if ( money > ch->gold )
    {
      send_to_char( "[33m§A¥I¤£°_ÂàÂ¾ªº¶O¥Î¡C[m\n\r", ch );
      return;
    }

    if ( ( cn = class_lookup( argument ) ) < 0
        || !is_name( class_table[cn].name, mob->jcl) )

    {
      send_to_char( "[33m§AµLªk¦b¦¹¥[¤J¨º­ÓÂ¾·~¡C[m\n\r", ch );
      return;
    }

    if ( !IS_NPC( ch ) &&
          class_table[cn].bclass[ch->class] > ch->level )
    {
      sprintf( buf, "[33m§A¦Ü¤Ö­n¨ì%d¯Å¤~¯à¥[¤J¨ºªùÂ¾·~¡C[m\n\r", class_table[cn].bclass[ch->class] );
      send_to_char( buf, ch );
      return;
    }

    do_remove( ch, "all" );

    for ( obj = ch->carrying; obj; obj = obj->next_content )
    {
      if ( obj->wear_loc != WEAR_NONE )
      {
        unequip_char( ch, obj );
            act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
            act( "You stop using $p.", ch, obj, NULL, TO_CHAR );
      }
    }

    die_follower( ch, "" );

    ch->gold        -= money;
    ch->max_hit = UMAX( 100, ch->max_hit / 2 );
    ch->max_mana  = UMAX( 100, ch->max_mana / 2 );
    ch->max_move  = UMAX( 100, ch->max_move / 2 );
    ch->pcdata->snd_attr  = class_table[cn].attr_prime;
    ch->level = 1;
    ch->exp   = 1000;
    ch->dodge += race_table[ ch->race ].nature_dodge;
    ch->armor += race_table[ ch->race ].nature_ac;
    //for(int i=0; i< 6; ++i)
      //  ch->saving_throw[i] += race_table[ ch->race ].nature_svs[i];
    if( ch->pcdata->prime_attr == ch->pcdata->snd_attr ){
        ch->pcdata->snd_attr = complement[cn];
    }
    ch->class = cn;

    do_wimpy(ch, "");
    do_save( ch, "" );

    sprintf( buf, HIW"%s ¤jÁn¦a«Å¥¬¡GÅwªï %s ¦¨¬° %s ªº¦¨­û¡I\n\r"NOR, 
        mob->short_descr, ch->name, class_table[cn].cname );
    send_to_all_char( buf );

    sprintf(buf,
     HIP"®¥³ß§A¡I§A²{¦b¥i¥H¶}©l´£¤É§Þ¯àµ¥¯Å¤F¡I\n\r"
        "¦b¸Ñ¹L¥ô°È©ÎÀ»±Ñ¯S©w¥Ø¼Ð¡A°O±o¦V§Þ¯à¤p¤Ñ¨Ï(Angel of Skill)»â¨ú§Þ¯à¸gÅçÂI³á¡I\n\r"
        "\n\r"
        "Ãö©ó§Þ¯àµ¥¯Å¨t²Î¡A½Ð¿é¤J help skexp, help skadv ¤Î help skdemote °Ñ¦Ò»¡©ú¤å¥ó\n\r"NOR);
    send_to_char(buf, ch);
    gain_skill_exp(ch, 100000);
    send_to_char("\n\r§A±o¨ì¤F 10 ¸UÂI§Þ¯à¸gÅçÂI¡I\n\r", ch);
  }
  return;
}

/*
 * Lookup a class by name.
 */
int class_lookup( const char *name )
{
    int cn;

    for ( cn = 0; cn < MAX_CLASS; cn++ )
    {
  if ( !class_table[cn].name )
      break;
  if ( LOWER( name[0] ) == LOWER( class_table[cn].name[0] )
      && !str_prefix( name, class_table[cn].name ) )
      return cn;
    }

    return -1;
}

/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf [ BUF_STRING_LENGTH ];
    char arg [ MAX_INPUT_LENGTH  ];
    int  wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
  wimpy = ch->max_hit / 5;
    else
  wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
  send_to_char( "[33m§Aªº«i®ð­n¤ñ§Aªº´¼¼z¨Ó±o°ª¡C[m\n\r", ch );
  return;
    }

    if ( wimpy > ch->max_hit / 2 )
    {
  send_to_char( "[33m§A¬OÁx¤p°­§r¡H[m\n\r", ch );
  return;
    }

    ch->wimpy      = wimpy;
    sprintf( buf, "[36mWimpy³]©w¨ì%dÂI¥Í©R­È¥H¤U·|¦Û°Ê°k¶]¡C[m\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char *pArg;
    char *pwdnew;
    char *p;
    char  arg1 [ MAX_INPUT_LENGTH ];
    char  arg2 [ MAX_INPUT_LENGTH ];
    char  cEnd;

    if ( IS_NPC( ch ) )
  return;

  if ( IS_SET( ch->act, PLR_HORSE ) )
  {
    send_to_char( "°¨¬O¤£¥Î password ªº¡C\n\r", ch );
    return;
  }
    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace( *argument ) )
  argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
  cEnd = *argument++;

    while ( *argument != '\0' )
    {
  if ( *argument == cEnd )
  {
      argument++;
      break;
  }
  *pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace( *argument ) )
  argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
  cEnd = *argument++;

    while ( *argument != '\0' )
    {
  if ( *argument == cEnd )
  {
      argument++;
      break;
  }
  *pArg++ = *argument++;
    }
    *pArg = '\0';
    *argument = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
  send_to_char( "[33m®æ¦¡¡Gpassword <ÂÂ±K½X> <·s±K½X>¡C[m\n\r", ch );
  return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
  WAIT_STATE( ch, 40 );
  send_to_char( "[33m±K½X¿ù»~¡Cµ¥¤Q¬íÄÁ¡C[m\n\r", ch );
  return;
    }

    if ( strlen( arg2 ) < 5 )
    {
  send_to_char(
      "[33m·sªº±K½X³Ì¤Ö±o­n¦³¤­­Ó¦r¡C[m\n\r", ch );
  return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
  if ( *p == '~' )
  {
      send_to_char(
    "[33m·sªº±K½X¤£¯à¥Î¡A¸Õ¸Õ§Oªº¡C[m\n\r", ch );
      return;
  }
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "[36mOK.[m\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH * 4];
    int  iSocial;
    int  col;

    buf1[0] = '\0';
    col = 0;
    for ( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
    {
  sprintf( buf, "%-12s", social_table[iSocial].name );
  strcat( buf1, buf );
  if ( ++col % 6 == 0 )
      strcat( buf1, "\n\r" );
    }
 
    if ( col % 6 != 0 )
  strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH * 4];
    int  cmd;
    int  col;
 
    buf1[0] = '\0';
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
  if (   cmd_table[cmd].level <  LEVEL_HERO
      && cmd_table[cmd].level <= get_trust( ch ) && cmd_table[cmd].toPPL )
  {
      sprintf( buf, "%-16s", cmd_table[cmd].name );
      strcat( buf1, buf );
      if ( ++col % 5 == 0 )
    strcat( buf1, "\n\r" );
  }
    }
 
    if ( col % 5 != 0 )
  strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}


// do_channels modified by jye 6.12.2005
void do_channels( CHAR_DATA *ch, char *argument )
{
  char arg1[ MAX_INPUT_LENGTH  ];
  char arg2[ MAX_INPUT_LENGTH  ];
  //char bug [ MAX_INPUT_LENGTH  ];
  char buf [ MAX_INPUT_LENGTH  ];
  CLAN_DATA *cn = clan_list;
  CHAR_DATA *vch = char_list;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if( arg1[0] == '\0' )
  {
      if( !IS_NPC( ch ) && ( IS_SET(ch->act, PLR_SILENCE) || IS_SET(ch->act, PLR_HORSE) ) )
      {
          send_to_char( "[33m§A³Q¸T¤î¨Ï¥ÎÀW¹D¤F¡C[m\n\r", ch );
          return;
      }
      
      send_to_char( "[36mÀW¹D¡G", ch );
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_AUCTION  )
             ? " +AUCTION"
             : " -auction",
             ch );
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_CHAT   )
             ? " +CHAT"
             : " -chat",
             ch );
      
      if( IS_IMMORTAL( ch ) )
      {
          send_to_char( !IS_SET( ch->deaf, CHANNEL_IMMTALK )
           ? " +IMMTALK"
           : " -immtalk",
           ch );
      }
      if( IS_HERO( ch ) )
      {
          send_to_char( !IS_SET( ch->deaf, CHANNEL_HEROTALK )
           ? " +HEROTALK"
           : " -herotalk",
           ch );
      }
      if( IS_IMMORTAL( ch ))
      {
          send_to_char( !IS_SET( ch->deaf, CHANNEL_SYSTEM )
           ? " +SYSTEM"
           : " -system",
           ch );
      }
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_MUSIC    )
             ? " +MUSIC"
             : " -music",
             ch );
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_QUESTION )
             ? " +QUESTION"
             : " -question",
             ch );
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_SHOUT    )
             ? " +SHOUT"
             : " -shout",
             ch );
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_YELL   )
             ? " +YELL"
             : " -yell",
             ch );
      
      send_to_char( !IS_SET( ch->deaf, CHANNEL_RUMOR    )
             ? " +RUMOR"
             : " -rumor",
             ch );
      
      send_to_char( "¡C[m\n\r", ch );
      
      if( ch->trust >= 44 || ch->level >=44)
      {
          send_to_char( "§A©Ò±µ¦¬ªºÀ°·|ÀW¹D¡G", ch );
          
          if( !str_cmp( ch->clan_channel, "all" ) )
          {
              send_to_char( "©Ò¦³À°·|", ch );
          }
          else if ( ch->clan_channel )
          {
              send_to_char( ch->clan_channel, ch );
          }
          else
          {
              send_to_char( "µL", ch );
          }
          
          send_to_char( "¡I\n\r", ch );
          send_to_char( "À°·|¦WºÙ¦Cªí¡G\n\r", ch );
          
          for( cn = clan_list; cn; cn = cn->next )
          {
               sprintf( buf, "%s (%s     ) ", cn->whoname, cn->name );
               send_to_char( buf, ch );
               send_to_char( "\n\r", ch );
          }
      }
  }
  else
  {
      int  bit;
      bool fClear;
      
      if( ch->trust >= 44 || ch->level >=44 )
      {

          if( !str_cmp( arg1, "tell" ) )
          {
              if( arg2[0] == '\0' )
                  return;
              if( ch->trust >= 45 || ch->level >=45 )
              {
              	  if( !str_cmp( arg2, "all"  ) )
              	  {
                      ch->tell_channel = str_dup( arg2 );
                      sprintf( buf, HIW"§A±Ò°Ê¤F½Ã¬PÅÑÅ¥¨t²Î¡I"NOR"\n\r");
                      send_to_char( buf, ch );
                      return;
              	  }
              	  else if( !str_cmp( arg2, "close"  ) )
              	  {
                      ch->tell_channel = NULL;
                      sprintf( buf, HIW"§AÃö³¬¤F½Ã¬PÅÑÅ¥¨t²Î¡I"NOR"\n\r");
                      send_to_char( buf, ch );
                      return;
              	  }
              	  else
              	  {
                      for( vch = char_list; vch; vch = vch->next )
                      {
                        if( !str_cmp( vch->name, arg2 ) )
                        {
                            if( IS_IMMORTAL( vch ) )
                            {
                                sprintf( buf, HIB"§A·Q°½°½¦b%s¨­¤W¸ËÅÑÅ¥¾¹¡I"NOR"\n\r", arg2 );
                                send_to_char( buf, ch );
                                sprintf( buf, HIW"°Ú³Qµo²{¤F¡I¡I"NOR"\n\r");
                                send_to_char( buf, ch );
                                return;
                            }
                            else
                            {
                                ch->tell_channel = str_dup( arg2 );
                                sprintf( buf, HIB"§A°½°½¦b%s¨­¤W¸Ë¤FÅÑÅ¥¾¹¡I"NOR"\n\r", arg2 );
                                send_to_char( buf, ch );
                                return;
                            }
                        }
                      }
                  }
              }
          }
          else if( !str_cmp( arg1, "all"  ) )
          {
              ch->clan_channel = str_dup( arg1 );
              send_to_char( "§A¶}©l±µ¦¬©Ò¦³À°·|ÀW¹D¡I\n\r", ch );
              return;
          }
          else if( !str_cmp( arg1, "close"  ) )
          {
              ch->clan_channel = NULL;
              send_to_char( "§AÃö³¬À°·|ÀW¹D¡I\n\r", ch );
              send_to_char( "\n\r", ch );
              return;
          }
          else
          {
              for( cn = clan_list; cn; cn = cn->next )
              {
                   if( !str_cmp( cn->name, arg1 ) || !str_cmp( cn->whoname, arg1 ) )
                   {
                       sprintf( buf, "§A¶}©l±µ¦¬%sªºÀ°·|ÀW¹D¡I\n\r", cn->whoname );
                       send_to_char( buf, ch );
                       ch->clan_channel = cn->whoname;
                       return; 
                   }
              }
          }
      }
      
      if( arg1[0] == '+' ) 
          fClear = TRUE;
      else if ( arg1[0] == '-' ) 
          fClear = FALSE;
      else
      {
          send_to_char( "[33mChannels -channel or +channel?[m\n\r", ch );
          return;
      }
      
           if( !str_cmp( arg1+1, "auction"  ) ) bit = CHANNEL_AUCTION;
      else if( !str_cmp( arg1+1, "chat"     ) ) bit = CHANNEL_CHAT;
      else if( !str_cmp( arg1+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
      else if( !str_cmp( arg1+1, "music"    ) ) bit = CHANNEL_MUSIC;
      else if( !str_cmp( arg1+1, "question" ) ) bit = CHANNEL_QUESTION;
      else if( !str_cmp( arg1+1, "shout"    ) ) bit = CHANNEL_SHOUT;
      else if( !str_cmp( arg1+1, "yell"     ) ) bit = CHANNEL_YELL;
      else if( !str_cmp( arg1+1, "rumor"    ) ) bit = CHANNEL_RUMOR;
      else if( !str_cmp( arg1+1, "system" )) bit = CHANNEL_SYSTEM;
      else if( !str_cmp( arg1+1, "herotalk" )) bit = CHANNEL_HEROTALK;
      else
      {
          send_to_char( "[33m­n¶}Ãö­þ¤@­ÓÀW¹D¡H[m\n\r", ch );
          return;
      }
      
      if( fClear )
          REMOVE_BIT ( ch->deaf, bit );
      else
          SET_BIT    ( ch->deaf, bit );
      
      send_to_char( "OK¡I\n\r", ch );
  }
  
  return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char arg [ MAX_INPUT_LENGTH ];

    if ( IS_NPC( ch ) )
  return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
  send_to_char( "[36m[  ÃöÁä¦r  ] ¿ï¶µ\n\r[m", ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOEXIT  )
      ? "[+AUTOEXIT ] §A¦Û°Ê§ä¥X¤f¡C\n\r"
      : "[-autoexit ] §A¤£¦Û°Ê§ä¥X¤f¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOLOOT  )
      ? "[+AUTOLOOT ] §A¦Û°Ê±q«ÍÅé¸Ì±°¨ú¾Ô§Q«~¡C\n\r"
      : "[-autoloot ] §A¤£¦Û°Ê±q«ÍÅé¸Ì±°¨ú¾Ô§Q«~¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOGOLD  )
      ? "[+AUTOGOLD ] §A¦Û°Ê±q«ÍÅé¸Ì®³ª÷¹ô¡C\n\r"
      : "[-autogold ] §A¤£¦Û°Ê±q«ÍÅé¸Ì®³ª÷¹ô¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOSAC   )
      ? "[+AUTOSAC  ] §A¦Û°Ê©^Äm«ÍÅé¡C\n\r"
      : "[-autosac  ] §A¤£¦Û°Ê©^Äm«ÍÅé¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOSPLIT  )
      ? "[+AUTOSPLIT] §A¦Û°Ê¤À¨Éª÷¹ô¡C\n\r"
      : "[-autosplit] §A¤£¦Û°Ê¤À¨Éª÷¹ô¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_BLANK     )
      ? "[+BLANK    ] §A¦b´£¥Ü¦r¦ê«e¦³¤@¦æªÅ¥Õ¡C\n\r"
      : "[-blank    ] §A¦b´£¥Ü¦r¦ê«e¨S¦³¤@¦æªÅ¥Õ¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_BRIEF     )
      ? "[+BRIEF    ] §A­n¬ÝÂ²µuªº´y­z¡C\n\r"
      : "[-brief    ] §A­n¬Ý§¹¾ãªº´y­z¡C\n\r"
      , ch );
   
  send_to_char(  IS_SET( ch->act, PLR_COMBINE   )
      ? "[+COMBINE  ] §A¨Ï¥Îµ²¦Xªº¤è¦¡¬Ýª««~²M³æ¡C\n\r"
      : "[-combine  ] §A¨Ï¥Î³æ¿Wªº¤è¦¡¬Ýª««~²M³æ¡C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_PROMPT    )
      ? "[+PROMPT   ] §A¦³´£¥Ü¦r¦ê¡C\n\r"
      : "[-prompt   ] §A¨S¦³´£¥Ü¦r¦ê¡C\n\r"
      , ch );
/*
  send_to_char(  IS_SET( ch->act, PLR_TELNET_GA )
      ? "[+TELNETGA ] §A±µ¨ütelnet GA¦r¦ê¡C\n\r"
      : "[-telnetga ] §A¤£±µ¨ütelnet GA¦r¦ê¡C\n\r"
      , ch );
*/      
  send_to_char(  IS_SET( ch->act, PLR_ARTIFACT )
      ? "[+ARTIFACT ] §A¨Ï¥Î¸Ë³ÆªºÁôÂÃ¤O¶q¡C\n\r"
      : "[-artifact ] §A¤£¨Ï¥Î¸Ë³ÆªºÁôÂÃ¤O¶q¡C\n\r"
      , ch );
      
  send_to_char(  IS_SET( ch->act, PLR_SILENCE   )
      ? "[+SILENCE  ] §A³Q¨HÀq¤Æ¤F¡C\n\r"
      : ""
      , ch );

  send_to_char( !IS_SET( ch->act, PLR_NO_EMOTE  )
      ? ""
      : "[-emote    ] §A¤£¥i¥Hªí¹F±¡·P¡C\n\r"
      , ch );

  send_to_char( !IS_SET( ch->act, PLR_NO_TELL   )
      ? ""
      : "[-tell     ] §A¤£¯à¥Î¡utell¡v¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_COMBAT )
      ? "[+COMBAT   ] §A±µ¦¬§¹¾ã¾Ô°«°T®§¡C\n\r"
      : "[-combat   ] §A¶È±µ¨üºëÂ²¾Ô°«°T®§¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_CHINESE )
      ? "[+CHINESE  ] §A±Ò°Ê¤¤¤å¼Ò¦¡¡C\n\r"
      : "[-chinese  ] §A¤£±Ò°Ê¤¤¤å¼Ò¦¡¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_STATUS )
      ? "[+STATUS   ] §A±Ò°Ê¾Ô°«¦å¶q­p¤Àªí¡C\n\r"
      : "[-status   ] §AÃö³¬¾Ô°«¦å¶q­p¤Àªí¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_TITLE    )
      ? "[+TITLE    ] §AªººÙ¸¹¤£·|ÅÜ°Ê¡C\n\r"
      : "[-title    ] §AªººÙ¸¹·|¦Û°Ê§ïÅÜ¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_ASSIST    )
      ? "[+ASSIST   ] §A¦Û°Ê¨ó§U¶¤¤Í§@¾Ô¡C\n\r"
      : "[-assist   ] §A¤£¨ó§U¶¤¤Í§@¾Ô¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_NOTIFY )
      ? "[+NOTIFY   ] §A±µ¦¬¨t²Î´£¥Ü°T®§¡C\n\r"
      : "[-notify   ] §A¤£·Q±µ¦¬¨t²Î´£¥Ü°T®§¡C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_DEAD )
    ? "[+DEAD     ] §A¦º¤`¤§«á±N¦Û°ÊÂ÷¶}¶¤¥î¡C\n\r"
    : "[-dead     ] §A¦º¤`¤§«á±N¤£·|¦Û°ÊÂ÷¶}¶¤¥î¡C\n\r"
    , ch );
  send_to_char(  IS_SET( ch->act, PLR_GRAPHIC )
    ? "[+GRAPHIC  ] §A¨Ï¥Î¹Ï§Î¤Æ¶¤¥î¸ê®Æ¡C\n\r"
    : "[-graphic  ] §A¤£¨Ï¥Î¹Ï§Î¤Æ¶¤¥î¸ê®Æ¡C\n\r"
    , ch );
  if( IS_IMMORTAL( ch ) ) {
      send_to_char(  IS_SET( ch->act, PLR_VERBOSEALL )
        ? "[+VERBOSEALL] §AÅã¥Ü¥þ³¡¸Ô²Ó¸ê®Æ¡C\n\r"
        : "[-verboseall] §A¤£·QÅã¥Ü¥þ³¡¸Ô²Ó¸ê®Æ¡C\n\r"
        , ch );
  }
    }
    else
    {
  char buf [ BUF_STRING_LENGTH ];
  long long int  bit;
  bool fSet;

       if ( arg[0] == '+' ) fSet = TRUE;
  else if ( arg[0] == '-' ) fSet = FALSE;
  else
  {
      send_to_char( "[33mConfig -option or +option?[m\n\r", ch );
      return;
  }

       if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
  else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
  else if ( !str_cmp( arg+1, "autogold" ) ) bit = PLR_AUTOGOLD;
  else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
  else if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
  else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
  else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
  else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
  else if ( !str_cmp( arg+1, "notify"   ) ) bit = PLR_NOTIFY;
  else if ( !str_cmp( arg+1, "artifact" ) ) bit = PLR_ARTIFACT;
  else if ( !str_cmp( arg+1, "combat" ) ) bit = PLR_COMBAT;
  else if ( !str_cmp( arg+1, "chinese" ) ) bit = PLR_CHINESE;
  else if ( !str_cmp( arg+1, "status" ) ) bit = PLR_STATUS;
  else if ( !str_cmp( arg+1, "title" ) ) bit = PLR_TITLE;
  else if ( !str_cmp( arg+1, "assist" ) ) bit = PLR_ASSIST;
  else if ( !str_cmp( arg+1, "autosplit")) bit = PLR_AUTOSPLIT;
  else if ( !str_cmp( arg+1, "dead")) bit=PLR_DEAD;
  else if ( !str_cmp( arg+1, "graphic")) bit=PLR_GRAPHIC;
  else if ( !str_cmp( arg+1, "verboseall")) {
      if( !IS_IMMORTAL(ch) ) { return; }
      bit = PLR_VERBOSEALL;
  }
  else
  {
      send_to_char( "[33m­n³]­þ¤@­Ó¿ï¶µ¡H[m\n\r", ch );
      return;
  }

  if ( fSet )
  {
      SET_BIT    ( ch->act, bit );
      sprintf( buf, "[36m%s²{¦b¥´¶}¤F¡C[m\n\r", arg+1 );
      buf[5] = UPPER( buf[5] );
      send_to_char( buf, ch );
  }
  else
  {
      REMOVE_BIT ( ch->act, bit );
      sprintf( buf, "[36m%s²{¦bÃö°_¨Ó¤F¡C[m\n\r", arg+1 );
      buf[5] = UPPER( buf[5] );
      send_to_char( buf, ch );
  }

    }

    return;
}

void do_wizlist ( CHAR_DATA *ch, char *argument )
{

    do_help ( ch, "wizlist" );
    return;

}

void do_spells ( CHAR_DATA *ch, char *argument )
{
	char buf  [ BUF_STRING_LENGTH ];
	char buf1 [ MAX_STRING_LENGTH * 4];
	int  sn;
	int  col;

	if ( IS_NPC( ch ))
	//|| ( !IS_NPC( ch ) && !class_table[ch->class].fMana ) )
	{  
		send_to_char ( "[33m§A¤£»Ý­n¨Ï¥Î©G¤å¡C[m\n\r", ch );
		return;
	}

	buf1[0] = '\0';

	col = 0;
	strcat(buf, "©G   ¤å   ¦C   ªí¡G\n\r");
	for ( sn = 1; sn < MAX_SKILL; sn++ )
	{
		if ( !skill_table[sn].name )
			break;

		//|| ( skill_table[sn].skill_level[ch->class] > LEVEL_HERO ) )
		if ( (  skill_table[sn].spell_fun == spell_null ))
		continue;

		//if(ch->pcdata->learned[sn] == 0) continue; 
        if( !get_skill_percent(ch, sn) ) continue;

		sprintf ( buf, "%15s(%17s) %3dÂI",
		skill_table[sn].cname,skill_table[sn].name, MANA_COST( ch, sn ) );
		strcat( buf1, buf );

		if ( ++col % 2 == 0 )
			strcat( buf1, "\n\r" );
	}

	if ( col % 2 != 0 )
		strcat( buf1, "\n\r" );

	send_to_char ( buf1, ch );
	return;

}

int skill_eff_percent( CHAR_DATA *ch, int sn ) 
{
	if( IS_NPC(ch) ) return 0;
	if( ch->class < WIZARD ){
		return skill_max_percent( ch, sn);
	}
    else
		return UMAX( skill_max_percent(ch, sn),( ( skill_table[sn].skill_level[apply_class[ch->pcdata->prime_attr]] ) / 1000 ));
}

void do_slist ( CHAR_DATA *ch, char *argument )
{
	char buf  [ BUF_STRING_LENGTH *2];
	char buf1 [ MAX_STRING_LENGTH * 8];
	char arg [MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int  sn;
	int  col;
	int  level;
	int  max_level;
	bool pSpell;
	bool fFound;
	int count = 0;
	int skdif = 0;


	one_argument(argument, arg);

	if( arg[0] == '\0' ) victim = ch;
	else{
		if( ! (victim = get_char_world(ch, arg ) ) )
		{
			send_to_char("³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch);
			return;
		}

		if( (!IS_SET( victim->act, PLR_HORSE )
			|| victim != ch->pcdata->horse
			|| victim->in_room != ch->in_room)
			&& !IS_IMMORTAL( ch ))
			{
				send_to_char("§A¤£¯à¬Ý§O¤Hªº¸ê®Æ¡C\n\r", ch);
				return;
			}
	}


	if ( IS_NPC( victim ) )
    {
		send_to_char ( "[33m§A¤£»Ý­n¥Î¨ì§Þ¯à¡C[m\n\r", ch );
		return;
    }

	buf1[0] = '\0';

	strcat ( buf1, "§AªºÂ¾·~¥i¥H¥Îªº§Þ¯à¦p¤U¡G\n\r\n\r" );
	strcat ( buf1, "µ¥¯Å          §Þ¯à/Å]ªk¦WºÙ                   ¼ô½m«×     §Þ¯àµ¥¯Å  ©Ò»Ý¸gÅç\n\r\n\r" );
    //              [ 1]  123456789012345678(123456789012345678) 123%

	max_level = ( victim->level < L_APP ) ? 42 : L_APP;
	fFound = FALSE;
	level = 0;
	// Á×§K¤U«ü¥O¤@ª½¶]¥Xbug log overflow[0] ±N sn ­×§ï¬°±q 1 ¶}©lºâ 2021/12/20
	//for ( sn = 0; sn < MAX_SKILL; sn++ )
	for ( sn = 1; sn < MAX_SKILL; sn++ )
	{
		if ( !skill_table[sn].name )
			break;
		if (skill_req_level( victim, sn ) != level
			&& !( level == 0
				&& get_skill_percent(victim, sn) != 0
				&& skill_req_level(victim, sn) == L_APP ))
				// skill_table[sn].skill_level[victim->class] != level &&
				// victim->pcdata->learned[sn] != 0 &&
			// all skills which basic class can learn but adv class can't will show behind [ 1]
				// skill_table[sn].skill_level[victim->class] == L_APP ) )
			continue;

		fFound = TRUE;
		break;
	}
    for ( level = (fFound) ? 0 : 1; level <= max_level; level++ )
	{

		col = 0;
		pSpell = TRUE;
		// Á×§K¤U«ü¥O¤@ª½¶]¥Xbug log overflow[0] ±N sn ­×§ï¬°±q 1 ¶}©lºâ 2021/12/20
		//for ( sn = 0; sn < MAX_SKILL; sn++ )
		for ( sn = 1; sn < MAX_SKILL; sn++ )
		{
			if( !skill_table[sn].name )
				break;
			if( skill_req_level(victim, sn) > -1
				&& skill_req_level(victim, sn) != level
				&& !( level == 0
				&& get_skill_percent( victim, sn) != 0
				&& skill_req_level(victim, sn) == L_APP ))
				//skill_table[sn].skill_level[victim->class] > -1
				//&& skill_table[sn].skill_level[victim->class] != level&&
				//victim->pcdata->learned[sn] != 0 &&
			//all skills which basic class can learn but adv class can't will show behind [ 1]
				//skill_table[sn].skill_level[victim->class] == L_APP ) )
				continue;
			// if(skill_table[sn].skill_level[victim->class] == -1){
			if( skill_req_level(victim, sn) == -1 ) {
				// if(sskill_table[sn].skill_level[apply_class[victim->pcdata->prime_attr]] < L_APP) {
				if ( skill_preq_level(victim, sn) < L_APP ) {
					// if((skill_table[sn].skill_level[apply_class[victim->pcdata->prime_attr]] / 2 +1  )!= level) {
					if ( ( skill_preq_level(victim, sn) / 2 + 1 ) != level ) {
						continue;
					}
				}else
					continue;
			}

			if ( pSpell )
			{
				sprintf ( buf, "[%2d]  ", level );
				strcat ( buf1, buf );
				pSpell = FALSE;
			}else{
				strcat( buf1, "      ");
			}
			//else if ( col++ % 4 == 0 )
			//  strcat ( buf1, "    " );

			++count;
			if( victim->class < WIZARD ) {
				//sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s \n\r", skill_table[sn].cname,
				sprintf ( buf, "%16s(%25s)  %3d%%/%3d%%   %8s \n\r", skill_table[sn].cname, // ³¡¤À§Þ¯à¦W¹Lªø,­×¬° 25 ¦r 2021/10/10
				skill_table[sn].name,
				//victim->pcdata->learned[sn]
				get_skill_percent(victim, sn),
				skill_eff_percent(victim, sn),
				skill_exp_desc(get_skill_level(victim, sn)));
			}else{
				skdif =  skill_difficulty(victim,sn);
				if( skdif >= 0 ) {
					//sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s  [%6d]\n\r", skill_table[sn].cname,
					sprintf ( buf, "%16s(%25s)  %3d%%/%3d%%   %8s  [%6d]\n\r", skill_table[sn].cname,
					skill_table[sn].name,
					//victim->pcdata->learned[sn]
					get_skill_percent(victim, sn),
					skill_eff_percent(victim, sn),
					skill_exp_desc(get_skill_level(victim, sn)),
					skill_difficulty(victim,sn));
				}else{
					//sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s  [¢Û¢Ï¢æ]\n\r", skill_table[sn].cname,
					sprintf ( buf, "%16s(%25s)  %3d%%/%3d%%   %8s  [¢Û¢Ï¢æ]\n\r", skill_table[sn].cname,
							skill_table[sn].name,
							//victim->pcdata->learned[sn]
							get_skill_percent(victim, sn),
							skill_eff_percent(victim, sn),
							skill_exp_desc(get_skill_level(victim, sn)));
							//,skill_difficulty(victim,sn) );
				}
			}
			strcat ( buf1, buf );
			// ­×§ï³Ì¤j pagelength ¬° 70, ¬G¥iÅã¥Ü¤W­­´£°ª 2023/01/12
			//if( count >= 50 ) {
			if( count >= 80 ) {
				send_to_char(buf1, ch);
				count = 0;
				buf1[0] = '\0';
			}
		}

	//if ( col % 4 != 0 )
	//	strcat ( buf1, "\n\r" );

	}

    send_to_char( buf1, ch );
    return;

}

/* bypassing the config command - Kahn */

void do_autoexit ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH ];

    ( IS_SET ( ch->act, PLR_AUTOEXIT )
     ? sprintf( buf, "-autoexit" )
     : sprintf( buf, "+autoexit" ) );

    do_config( ch, buf );

    return;

}

void do_autoloot ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH];

    ( IS_SET ( ch->act, PLR_AUTOLOOT )
     ? sprintf( buf, "-autoloot" )
     : sprintf( buf, "+autoloot" ) );

    do_config( ch, buf );

    return;
}

void do_autogold ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH];

    ( IS_SET ( ch->act, PLR_AUTOGOLD )
     ? sprintf( buf, "-autogold" )
     : sprintf( buf, "+autogold" ) );

    do_config( ch, buf );

    return;

}



void do_autosac ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH ];

    ( IS_SET ( ch->act, PLR_AUTOSAC )
     ? sprintf( buf, "-autosac" )
     : sprintf( buf, "+autosac" ) );

    do_config( ch, buf );

    return;

}

void do_blank ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH ];

    ( IS_SET ( ch->act, PLR_BLANK )
     ? sprintf( buf, "-blank" )
     : sprintf( buf, "+blank" ) );

    do_config( ch, buf );

    return;

}

void do_brief ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH ];

    ( IS_SET ( ch->act, PLR_BRIEF )
     ? sprintf( buf, "-brief" )
     : sprintf( buf, "+brief" ) ) ; 

    do_config( ch, buf );

    return;

}

void do_combine ( CHAR_DATA *ch, char *argument )
{
    char buf[ MAX_INPUT_LENGTH ];

    ( IS_SET ( ch->act, PLR_COMBINE )
     ? sprintf( buf, "-combine" )
     : sprintf( buf, "+combine" ) );

    do_config( ch, buf );

    return;

}
 
void do_pagelen ( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];
	char arg [ MAX_INPUT_LENGTH  ];
	int  lines;

	if ( IS_NPC( ch ) )
		return;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
		lines = 40;
	else
		lines = atoi( arg );

	if ( lines < 19 )
	{
		send_to_char( "[33m¨C­¶¤£¯à¤Ö©ó¤Q¤E¦æ¡C[m", ch );
		return;
	}

	if ( lines > 70 )
	{
		send_to_char( "[33m½Ð¤Å³]©w¦Ü¶W¹L¤C¤Q¦æ¡I[m\n\r", ch );
		lines = 70;
    }

	ch->pcdata->pagelen = lines;
	sprintf( buf, "[36m¤À­¶³]©w³]¨ì¤@­¶%d¦æ¡C[m\n\r", lines );
	send_to_char( buf, ch );
	return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
	char buf [ MAX_STRING_LENGTH ];

	buf[0] = '\0';

	/* Unswitched NPC's get kicked out */
	if ( !ch->desc )
		return;

	/* Will always have a pc ch after this */
	if( IS_NPC(ch) )
		ch = ( ch->desc->original ? ch->desc->original : ch->desc->character );

	if ( argument[0] == '\0' )
	{
		( IS_SET ( ch->act, PLR_PROMPT )
		? sprintf( buf, "-prompt" )
		: sprintf( buf, "+prompt" ) );

		do_config( ch, buf );
		return;
	}

	if( !strcmp( argument, "all" ) )
		strcat( buf, "[m<¥Í©R[1;32m%h[mÅ]ªk[1;33m%m[m²¾°Ê[1;35m%v[m>");
	else if( !strcmp( argument, "max" ) )
	{
		strcat(buf, NOR"< %h/"HIC"%H "NOR"%m/"HIC"%M "NOR"%v/"HIC"%V"NOR" >");
	}
	else
	{
		if ( strlen( argument ) > 200 )
			argument[200] = '\0';
			smash_tilde( argument );
			strcat( buf, argument );
	}
	strcat( buf, " " );

	free_string( ch->pcdata->prompt );
	ch->pcdata->prompt = str_dup( buf );
	do_config(ch,"+prompt");
	send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
	return;
} 

void do_auto( CHAR_DATA *ch, char *argument )
{

    do_config( ch, "" );
    return;

}

void do_afk( CHAR_DATA *ch, char *argument )
{
	if ( IS_NPC( ch ) )
		return;

	if ( IS_SET( ch->act, PLR_AFK ) )
	{
		REMOVE_BIT( ch->act, PLR_AFK );
		send_to_char( "[36m§A¦^¨ì¤F¶Ç»¡¤¤¡C[m\n\r", ch );
		act( "[36m$n¦^¨ì¤F¶Ç»¡¤¤¡C[m", ch, NULL, ch, TO_ROOM );
	}
	else
	{
		SET_BIT( ch->act, PLR_AFK );
		send_to_char( "[36m§A¼È®ÉÂ÷¶}¶Ç»¡¡C[m\n\r", ch   );
		act( "[36m$n¼È®ÉÂ÷¶}¶Ç»¡¡C[m", ch, NULL, ch, TO_ROOM );
	}

	return;
}

/* scan skill added by Amenda 00/12/6 */
void do_scan( CHAR_DATA *ch, char *argument )
{
	EXIT_DATA *pexit;
	int i;

	if ( !IS_NPC( ch )
		&& ( ch->race != race_lookup( "Hobbit" ) ) // hobbit scan ¤£·|¥¢±Ñ 2022/02/01
		&& number_percent() > get_skill_percent(ch, gsn_scan))
	{
		send_to_char( "§A¥¢±Ñ¤F!\n\r", ch );
		return;
	}
	// affect stone form ­×¥¿ 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§A¦b¥Û¤Æª¬ºA¤U¤£¯à²¾°Ê.\n\r", ch);
		return;
	}

	if ( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare ±Ô­z­×§ï 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m§A³Q§x¦b³´¨À¸Ì¤F¡I§A¤£¯à²¾°Ê¡I[m\n\r", ch );
		}else{
			send_to_char( "[33m§Aªº¨­Åé°Ê¼u¤£±o¡I§A¤£¯à²¾°Ê¡I[m\n\r", ch );
		}
		return;
	}

	if ( IS_AFFECTED( ch, AFF_PARALYZED ) ) 
	{
		send_to_char( "§A¦]³Â·ô¦Ó°Ê¼u¤£±o¡I\n\r", ch );
		return;
	}

	/*  ²¾°£¥Ø«e©Ð¶¡¶Â·t«hµLªk¨Ï¥Îªº³]©w 2022/02/01
	if ( room_is_dark( ch->in_room ) || !check_blind( ch ) )
	{
		send_to_char( "¥|©P¤@¤ùº£¶Â¡A§A¤°»ò³£¬Ý¤£¨ì!\n\r", ch );
		return;
	}
	*/
	for( i = 0; i < 6 ; i ++ )
	{
		if ( ( pexit = ch->in_room->exit[i] ) && !IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			scan_print( ch, pexit, i, 1 );
			if ( pexit->to_room && ( pexit = pexit->to_room->exit[i] ) && !IS_SET( pexit->exit_info, EX_CLOSED ) )
			{
				scan_print( ch, pexit, i, 2 );
				if ( pexit->to_room && ( pexit = pexit->to_room->exit[i] ) && !IS_SET( pexit->exit_info, EX_CLOSED ) )
					scan_print( ch, pexit, i, 3 );
			}
		}
	}
}

void scan_print(CHAR_DATA *ch, EXIT_DATA *pexit, int door, int level)
{
  CHAR_DATA *pch;
      char buf[ MAX_STRING_LENGTH * 2];
      
      if ( room_is_dark( pexit->to_room ) )
        return;
        
  for ( pch = pexit->to_room->people; pch; pch = pch->next_in_room )
  {
    if ( !IS_NPC( ch ) && !IS_IMMORTAL( ch ) && (
    //number_percent() > UMIN( ch->pcdata->learned[gsn_scan], 95)
                number_percent() > UMIN( get_skill_percent(ch, gsn_scan), 95)
    || !can_see( ch, pch) ) )
      continue;
      
    strcpy( buf, "$N ¦b§Aªº ");
    switch ( door )
    {
    case 0 : strcat( buf, "¥_Ãä" ); break;
    case 1 : strcat( buf, "ªFÃä" ); break;
    case 2 : strcat( buf, "«nÃä" ); break;
    case 3 : strcat( buf, "¦èÃä" ); break;
    case 4 : strcat( buf, "¤W­±" ); break;
    case 5 : strcat( buf, "¤U­±" ); break;
    }
    switch (level)
    {
    case 1 : break;
    case 2 : strcat( buf, " ¤£»·" ); break;
    case 3 : strcat( buf, " ¸û»·³B" ); break;
    }
  act( buf, ch, NULL, pch, TO_CHAR );
  }
}

/* scout skill added by Amenda 2002/6/28 */
void do_scout( CHAR_DATA *ch, char *argument )
{
	int i;
	int sklv = get_skill_level(ch, skill_lookup("scout"));
	//int nMoveCR = sklv * (sklv + 1) * 5;
	int nMoveCR = 0;
	ROOM_INDEX_DATA *in_room, *to_room;
	EXIT_DATA *pexit;

	if ( ch->fighting || IS_NPC( ch ) ||
		number_percent() > get_skill_percent(ch, skill_lookup("scout")))
	{
		send_to_char( "§A¥¢±Ñ¤F!\n\r", ch );
		return;
	}
	// affect stone form ­×¥¿ 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("§A¦b¥Û¤Æª¬ºA¤U¤£¯à²¾°Ê.\n\r", ch);
		return;
	}

	if ( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare ±Ô­z­×§ï 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m§A³Q§x¦b³´¨À¸Ì¤F¡I§A¤£¯à²¾°Ê¡I[m\n\r", ch );
		}else{
			send_to_char( "[33m§Aªº¨­Åé°Ê¼u¤£±o¡I§A¤£¯à²¾°Ê¡I[m\n\r", ch );
		}
		return;
	}

	if ( IS_AFFECTED( ch, AFF_PARALYZED ) ) 
	{
		send_to_char( "§A¦]³Â·ô¦Ó°Ê¼u¤£±o!\n\r", ch );
		return;
	}

	if( auction_list )
	{
		AUC_DATA *auction;
		for( auction = auction_list ; auction; auction = auction->next )
		{
			if ( ch == auction->owner || ch == auction->buyer )
			{
				send_to_char("§A¥¿¦b¶i¦æ©ç½æ¡A¤£¯àÂ÷¶}³o¸Ì¡C\n\r", ch);
				return;
			}
		}
	}

	/*  ²¾°£¥Ø«e©Ð¶¡¶Â·t«hµLªk¨Ï¥Îªº³]©w 2022/02/01
	if ( room_is_dark( ch->in_room ) || !check_blind( ch ) )
	{
		send_to_char( "¥|©P¤@¤ùº£¶Â¡A§A¤°»ò³£¬Ý¤£¨ì!\n\r", ch );
		return;
	}
	*/

	// hobbit ¦³ÃB¥~ sklv ¥[¦¨ , ¨Ï sklv ³Ì°ª¥i¬° 7  2022/02/01
	if( ch->race == race_lookup( "Hobbit" ) )
		sklv += 1;

	// mv ³Ì§C®ø¯Ó 1  2022/02/01
	nMoveCR = UMIN( 299 , sklv * (sklv + 1) * 5 );

	// ­×¥¿¤£´÷ºØ±Ú¤£·|§P©w´÷­È 2021/10/02
	if ( race_table[ch->race].thirst_mod >= 0 && ch->pcdata->condition[COND_THIRST] <= 15 && sklv < 4)
	{
		send_to_char( "§A¤Ó´÷¤F¡C\n\r", ch );
		return;
	}
	// ­×¥¿¤£ÄÈ¾jºØ±Ú¤£·|§P©wÄÈ¾j­È 2021/10/02
	if( race_table[ch->race].hunger_mod >= 0 && ch->pcdata->condition[COND_FULL] <= 15 && sklv < 5)
	{
		send_to_char( "§A¤Ó¾j¤F¡C\n\r", ch );
		return;
	}
	if(ch->move < 300 - nMoveCR)
	{
		send_to_char( "§A¨S¦³¨¬°÷ªºÅé¤O¡C\n\r", ch );
		return;
	}
	else
	{
		// ­×¥¿¤£ÄÈ¾jºØ±Ú¤£·|¦©°£ÄÈ¾j­È  2021/10/02
		if( sklv < 4 && race_table[ch->race].thirst_mod >= 0 )
			ch->pcdata->condition[COND_THIRST] -= 12 - sklv*2;
		// ¹¡¨¬ª¬ºA¤U¤£·|´î¤ÖÄÈ¾j­È¡A¥B¤£ÄÈ¾jºØ±Ú¤£·|¦©°£ÄÈ¾j­È 2021/10/02
		if(!is_affected(ch, gsn_satiety)){
			if( sklv < 5 && race_table[ch->race].hunger_mod >= 0 )
				ch->pcdata->condition[COND_FULL] -= 12 - sklv*2;
		}
		ch->move -= 300 - nMoveCR;
	}
  
	if ( !str_prefix(argument, "north" ) )
		i = 0;
	else if ( !str_prefix(argument, "east" ) )
		i = 1;
	else if ( !str_prefix(argument, "south" ) )
		i = 2;
	else if ( !str_prefix(argument, "west" ) )
		i = 3;
	else if ( !str_prefix(argument, "up" ) )
		i = 4;
	else if ( !str_prefix(argument, "down" ) )
		i = 5;
	else{
		send_to_char( "§A¨ì©³¦³¨S¦³¤è¦V·P°Ú¡H\n\r", ch );
		return;
	}
  
	in_room = ch->in_room;

	// ²Ä¤@¦¸ scout , ­×§ï±Ô­z¥[¤J¤è¦V 2022/12/22
	//send_to_char( ""HIB"§A¸ÕµÛ°»¹î¾aªñ§Aªº¦a¤è¡G"NOR"\n\r", ch );
	if( i == 0 )
		send_to_char( ""HIB"§A¸ÕµÛ©¹"HIC"¥_"HIB"Ãä¶i¦æ°»¹î¡G"NOR"\n\r", ch );
	if( i == 1 )
		send_to_char( ""HIB"§A¸ÕµÛ©¹"HIC"ªF"HIB"Ãä¶i¦æ°»¹î¡G"NOR"\n\r", ch );
	if( i == 2 )
		send_to_char( ""HIB"§A¸ÕµÛ©¹"HIC"«n"HIB"Ãä¶i¦æ°»¹î¡G"NOR"\n\r", ch );
	if( i == 3 )
		send_to_char( ""HIB"§A¸ÕµÛ©¹"HIC"¦è"HIB"Ãä¶i¦æ°»¹î¡G"NOR"\n\r", ch );
	if( i == 4 )
		send_to_char( ""HIB"§A¸ÕµÛ©¹"HIC"¤W"HIB"¤è¶i¦æ°»¹î¡G"NOR"\n\r", ch );
	if( i == 5 )
		send_to_char( ""HIB"§A¸ÕµÛ©¹"HIC"¤U"HIB"¤è¶i¦æ°»¹î¡G"NOR"\n\r", ch );
	if ( !( pexit = ch->in_room->exit[i] ) || !( to_room = pexit->to_room ) ){
		send_to_char( "[33m«u§r¡A¦¹¸ô¤£³q¡C[m\n\r", ch );
		return;
	}else{
		if( can_scout_check( ch, pexit, to_room, sklv ) )
			scout( ch, pexit, to_room, sklv );
		else
			return;
	}

	// ©Ò³B©Ð¶¡¦³ ROOM_SCOUT_ONE ªº room flag «h¥u¯à¬d¤@®æ
	if ( IS_SET( in_room->room_flags, ROOM_SCOUT_ONE ) ){
		send_to_char( ""HIB"§A¸ÕµÛ°»¹î¤£»·³B¡A¦ý§A¥¢±Ñ¤F¡I"NOR"\n\r", ch );
	}
	else
	{
		// ²Ä¤G¦¸ scout
		if( i == 0 )
			send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¥_"HIB"Ãä¤£»·³B¡G"NOR"\n\r", ch );
		if( i == 1 )
			send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"ªF"HIB"Ãä¤£»·³B¡G"NOR"\n\r", ch );
		if( i == 2 )
			send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"«n"HIB"Ãä¤£»·³B¡G"NOR"\n\r", ch );
		if( i == 3 )
			send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¦è"HIB"Ãä¤£»·³B¡G"NOR"\n\r", ch );
		if( i == 4 )
			send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¤W"HIB"¤è¤£»·³B¡G"NOR"\n\r", ch );
		if( i == 5 )
			send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¤U"HIB"¤è¤£»·³B¡G"NOR"\n\r", ch );
		//send_to_char( ""HIB"§A¸ÕµÛ°»¹î¤£»·³B¡G"NOR"\n\r", ch );
		if ( !( pexit = to_room->exit[i] ) || !( to_room = pexit->to_room ) ){
			send_to_char( "[33m«u§r¡A¦¹¸ô¤£³q¡C[m\n\r", ch );
		}else{
			if( can_scout_check( ch, pexit, to_room, sklv ) )
			{
				scout( ch, pexit, to_room , sklv );
				// ²Ä¤T¦¸ scout
				if( sklv > 5 ){
					if( i == 0 )
						send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¥_"HIB"Ãä¸û»·³B¡G"NOR"\n\r", ch );
					if( i == 1 )
						send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"ªF"HIB"Ãä¸û»·³B¡G"NOR"\n\r", ch );
					if( i == 2 )
						send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"«n"HIB"Ãä¸û»·³B¡G"NOR"\n\r", ch );
					if( i == 3 )
						send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¦è"HIB"Ãä¸û»·³B¡G"NOR"\n\r", ch );
					if( i == 4 )
						send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¤W"HIB"¤è¸û»·³B¡G"NOR"\n\r", ch );
					if( i == 5 )
						send_to_char( ""HIB"§A¸ÕµÛ°»¹î"HIC"¤U"HIB"¤è¸û»·³B¡G"NOR"\n\r", ch );
					if ( !( pexit = to_room->exit[i] ) || !( to_room = pexit->to_room ) ){
						send_to_char( "[33m«u§r¡A¦¹¸ô¤£³q¡C[m\n\r", ch );
					}else{
						if( can_scout_check( ch, pexit, to_room, sklv ) )
							scout( ch, pexit, to_room , sklv );
					}
				}
			}
		}
	}

	char_from_room( ch );
	char_to_room( ch, in_room );
	act( "§Aµo²{ $n ­·¹Ð¹²¹²¡Aº¡¨­¤j¦½¦a±q³±·t¨¤¸¨¥X²{¡C", ch, NULL, NULL, TO_ROOM );
}

// §P©w scout ªº©Ð¶¡¬O§_¯à¶i¤J 2022/12/22
bool can_scout_check( CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room, int sklv )
{

	if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
	{
		if ( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) )
		{
			act( "[33m$d¬OÃöµÛªº¡C[m",ch, NULL, pexit->keyword, TO_CHAR );
			return FALSE;
		}
		if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
		{
			act( "[33m§A¬ï¤£¹L$d¡C¦nµh¡I[m",ch, NULL, pexit->keyword, TO_CHAR );
			return FALSE;
		}
	}
	if ( to_room->sector_type == SECT_AIR )
	{
		if( !is_flying(ch))
		{
			send_to_char( "[33m§A­¸¤£°_¨Ó¡C[m\n\r", ch );
			return FALSE;
		}
	}

	if (   to_room->sector_type != SECT_WATER_NOSWIM
		&& to_room->sector_type != SECT_UNDERWATER
		&& strcmp( race_table[ ch->race ].name, "God" )
		&& strcmp( race_table[ ch->race ].name, "Bear" )
		&& strcmp( race_table[ ch->race ].name, "Lizardman" )
		&& IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ))
	{
		send_to_char( "[33m§A¦b­ì¦aÂ¶¨ÓÂ¶¥h«o´å¤£¹L¥h¡I[m\n\r", ch );
		return FALSE;
	}

	if ( to_room->sector_type == SECT_WATER_NOSWIM )
	{
		OBJ_DATA *obj;
		bool found; // Look for a boat.

		found = FALSE;


		// Suggestion for flying above water by Sludge
		if(    is_flying( ch )
			|| IS_SET( race_table[ ch->race ].race_abilities, RACE_WATERWALK )
			|| IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ) )
		found = TRUE;

		for ( obj = ch->carrying; obj; obj = obj->next_content )
		{
			if ( obj->item_type == ITEM_BOAT )
			{
				found = TRUE;
				break;
			}
		}
		if ( !found )
		{
			send_to_char( "[33m§A±o­n¦³¤@¿´²î¤~¹L±o¥h¡C[m\n\r", ch );
			return FALSE;
		}
	}

	// door size §P©w 2022/12/22
	if( sklv < 2 )
	{
		//if ( pexit->exit_size > 0 && ( race_table[ch->race].size > pexit->exit_size ) )
		if ( pexit->exit_size > 0 && ( get_curr_size( ch ) > pexit->exit_size ) ) // ´À´«Åé«¬ size ¤½¦¡ 2022/12/23
		{
			send_to_char( "[33m§AÅé«¬¤Ó¤j¡A¶i¤J¤£¤F³q¹D¡C[m\n\r", ch );
			return FALSE;
		}
	}

	// room size ¤H¼Æ§P©w 2022/12/22
	if( sklv < 4 )
	{
		if ( room_is_private( ch, to_room ) )
		{
			send_to_char( "[33m¨º¸ÌªºªÅ¶¡¤j¤p¡A®e¯Ç¤£¤U§AªºÅé¿n¡C[m\n\r", ch );
			return FALSE;
		}
	}

	return TRUE;
}
void scout( CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room, int sklv )
{
	int room_flags = 0; // 2022/12/22

	// remove at 2022/12/22
	// ¦¹¬q§P©w¶È¯à³B²z³Q scout ªº¨º®æ©Ð¶¡, ·í²Ä¤@®æ©Ð¶¡µLªk¶i¤J®É, µLªkªý¾×¶i¤J²Ä¤G®æ©Ð¶¡
	// ¬G²¾¦Ü¤W¤è§@¦¨ bool can_scout_check ¨Ó¿W¥ß§P©w¬O§_¯à¹ï©Ð¶¡§@ scout
		/*if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			if ( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) )
			{
				act( "[33m$d¬OÃöµÛªº¡C[m",ch, NULL, pexit->keyword, TO_CHAR );
				return;
			}
			if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
			{
				act( "[33m§A¬ï¤£¹L$d¡C¦nµh¡I[m",ch, NULL, pexit->keyword, TO_CHAR );
				return;
			}
		}
		if ( to_room->sector_type == SECT_AIR )
		{
			if( !is_flying(ch))
			{
				send_to_char( "[33m§A­¸¤£°_¨Ó¡C[m\n\r", ch );
				return;
			}
		}

		if (   to_room->sector_type != SECT_WATER_NOSWIM
			&& to_room->sector_type != SECT_UNDERWATER
			&& strcmp( race_table[ ch->race ].name, "God" )
			&& strcmp( race_table[ ch->race ].name, "Bear" )
			&& strcmp( race_table[ ch->race ].name, "Lizardman" )
			&& IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ))
		{
			send_to_char( "[33m§A¦b­ì¦aÂ¶¨ÓÂ¶¥h«o´å¤£¹L¥h¡I[m\n\r", ch );
			return;
		}

		if ( to_room->sector_type == SECT_WATER_NOSWIM )
		{
			OBJ_DATA *obj;
			bool found; // Look for a boat.

			found = FALSE;

			// Suggestion for flying above water by Sludge
			if(    is_flying( ch )
				|| IS_SET( race_table[ ch->race ].race_abilities, RACE_WATERWALK )
				|| IS_SET( race_table[ ch->race ].race_abilities, RACE_SWIM ) )
			found = TRUE;

			for ( obj = ch->carrying; obj; obj = obj->next_content )
			{
				if ( obj->item_type == ITEM_BOAT )
				{
					found = TRUE;
					break;
				}
			}
			if ( !found )
			{
				send_to_char( "[33m§A±o­n¦³¤@¿´²î¤~¹L±o¥h¡C[m\n\r", ch );
				return;
			}
		} */

	char_from_room( ch );
	char_to_room( ch, to_room );
	do_look( ch, "" );
	// ¥[¤J sklv room flag ±Ô­z 2022/12/22
	send_to_char( "³o¸Ì", ch );
	if( sklv >= 3 )
	{
		if( IS_SET( to_room->room_flags, ROOM_NO_RECALL) )
		{
			if( IS_SET( to_room->room_flags, ROOM_PK) )
				send_to_char( "¬O¨üªý©Gªº", ch );
			else
				send_to_char( "¨ü¨ìªý©G", ch );
			room_flags += 1;
		}

		if ( room_flags < 0 ) send_to_char( "¬O", ch );

		if( IS_SET( to_room->room_flags, ROOM_PK) )
		{
			send_to_char( "°«§Þ³õ", ch );
			room_flags += 1;
		}
	}
	if( sklv >= 1 )
	{
		if ( room_flags > 0 ) send_to_char( "¡A", ch );

		if( IS_SET( to_room->room_flags, ROOM_INDOORS) )
		{
			send_to_char( "¦ì©ó«Ç¤º", ch );
			room_flags += 1;
		}
		if( IS_SET( to_room->room_flags, ROOM_DARK) )
		{
			send_to_char( "¥|©P¬Û·í¶Â·t", ch );
			room_flags += 1;
		}

		if( IS_SET( to_room->room_flags, ROOM_SAFE) )
		{
			if ( room_flags > 0 )
				send_to_char( "¡A¤£¹L¬Ý°_¨Ó«Ü¦w¥þ", ch );
			else
				send_to_char( "¬Ý°_¨Ó«Ü¦w¥þ", ch );
			room_flags += 1;
		}
		if( sklv >= 4 )
		{
			if( IS_SET( to_room->room_flags, ROOM_CAMP) ){
				if ( room_flags > 0 )
					send_to_char( "¡A¨Ã¥B¦a­±¬yÂ«µÛÅ]¤O", ch );
				else
					send_to_char( "¦a­±¬yÂ«µÛÅ]¤O", ch );
				room_flags += 1;
			}
		}
	}
	if( room_flags > 0 ){
		send_to_char( "¡C\n\r", ch );
	}else{
		send_to_char( "¨S¤°»ò¯S§Oªº¡C\n\r", ch );
	}

	// ¥[¤J sklv §P©w 2022/12/22
	if( sklv > 5 )
	{
		if( IS_SET( to_room->room_flags, ROOM_CAMP) )
		if ( to_room->roomprogs )
			send_to_char( "¦¹¥~¡A§A±Ó¾UªºÆ[¹î¤Oµo²{³o¸Ì¦ü¥G¦³¤°»ò¤£¹ï«l¡C\n\n\r", ch );
	}
}

int skill_exp_level( int exp )
{
    return 0;
}

char * skill_exp_desc( int exp )
{
    switch(exp) {
        case 6:
            return HIW"¤Ñ¤H¦X¤@"NOR;
            break;
        case 5:
            return HIC"²`¤£¥i´ú"NOR;
            break;
        case 4:
            return HIY"¶W¤Z¤J¸t"NOR;
            break;
        case 3: 
            return HIG"¯«¥G¨ä§Þ"NOR;
            break;
        case 2:
            return HIR"±o¤ßÀ³¤â"NOR;
            break;
        case 1:
            return HIP"²¤ª¾¤@¤G"NOR;
            break;
        case 0:
            return HIU"·s¾Ç¥E½m"NOR;
            break;
        default:
            return "BUG CHAR";
            break;
    }
}

void do_skadv( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[BUF_STRING_LENGTH];
	int sn = 0;
	int cost;
	CHAR_DATA *mob;

	if( IS_NPC(ch) )
		return;

	if( ch->class < WIZARD ) {
		send_to_char("§A¥²¶·ÂàÂ¾«á¤~¯à´£¤É§Þ¯àµ¥¯Å¡C\n\r", ch);
		return;
	}
	argument = one_argument(argument, arg);
	argument = one_argument(argument, arg2);

	if( arg[0] == '\0' ) {
		send_to_char("§A­n´£¤É¤°»ò§Þ¯à¡H\n\r", ch);
		return;
	}

	sn = skill_lookup(arg);

	if( sn < 0 ) {
		send_to_char("¨S¦³³o­Ó§Þ¯à¡C\n\r", ch);
		return;
	}

	if( get_skill_percent(ch,sn) < 1 ) {
		send_to_char("§A¨Ã¤£·|³o¶µ§Þ¯à¡C\n\r", ch);
		return;
	}

	if ( sn == gsn_battle_array )
		return;

	if( sn == gsn_inborn_ability)
	{
		AFFECT_DATA *paf = NULL;
		for ( paf = ch->affected; paf; paf = paf->next )
		{
			if ( paf->deleted )
				continue;
			if ( paf->type == gsn_polymorph){
				send_to_char("§A¤£¯à¦bÅÜ±Úªº±¡ªp´£¤É§Aªº¤Ñ½á¡C\n\r", ch);
				return;
			}
		}
	}


	// °w¹ï¯S®í room ªº±¡ªp, ¤§«á¥i¥H¥[¤J¨ä¥L room_flag §@ room skadv ªº§P©w
	if( ch->in_room->vnum == 11000 )
		;
	else
	{
		if( arg2[0] == '\0' )
		{
			for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
			{
				if ( mob->deleted )
					continue;
				if ( IS_NPC( mob ) && IS_SET( mob->act, ACT_PRACTICE ) )
					break;
			}
		}
		else
		{
			for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
			{
				if ( mob->deleted )
					continue;
				if ( IS_NPC( mob ) && IS_SET( mob->act, ACT_PRACTICE ) && is_pfxname(arg2, mob->name))
					break;
			}
		}

		if ( !mob || !IS_SET( mob->act, ACT_PRACTICE ))
		{
			send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
			return;
		}

		if( !mob->psk ) {
			send_to_char( "[33m§A§ä¤£¨ì¾É®v¡C[m\n\r", ch );
			return;
		}

		switch( is_skname( arg, mob->psk ) )
		{
			case -1 :
				act( "$n »¡¡G¹ï¤£°_¡A§Ú¨Ã¤£·|¨º¶µ§Þ¥©¡C", mob, NULL, ch, TO_VICT );
				return;
			case 1 :
				if ( !is_qmark(ch, skill_table[sn].name) )
				{
					act( "$n »¡¡G¹ï¤£°_¡A§AÁÙ¤£°÷¸ê®æ¾Ç²ß³o¶µ§Þ¥©¡C", mob, NULL, ch, TO_VICT );
					return;
				}
				break;
			default :
				break;
		}
	}

	/* ±N sn §P©w²¾¨ì³Ì«e¤è, ¬O is_skname ¯à°÷²£¥Í§@¥Î 2023/02/20

		if( arg[0] == '\0' ) {
			send_to_char("§A­n´£¤É¤°»ò§Þ¯à¡H\n\r", ch);
			return;
		}
		sn = skill_lookup(arg);
		if( sn < 0 ) {
			send_to_char("¨S¦³³o­Ó§Þ¯à¡C\n\r", ch);
			return;
		}
		if(get_skill_percent(ch,sn) < 1 ) {
			send_to_char("§A¨Ã¤£·|³o¶µ§Þ¯à¡C\n\r", ch);
			return;
		}

		if ( sn == gsn_battle_array )
			return;

		if( sn == gsn_inborn_ability)
		{
			AFFECT_DATA *paf = NULL;
			for ( paf = ch->affected; paf; paf = paf->next )
			{
				if ( paf->deleted )
					continue;
				if ( paf->type == gsn_polymorph){
					send_to_char("§A¤£¯à¦bÅÜ±Úªº±¡ªp´£¤É§Aªº¤Ñ½á¡C\n\r", ch);
					return;
				}
			}
		}
	*/


	if( (cost = skill_difficulty(ch,sn)) < 0 ) {
		send_to_char("§A¦b³o¶µ§Þ¯à¤W¤w¸gµLªk¦A¶i¨B¤F¡C\n\r", ch);
		return;
	}
	if( get_eff_skill_exp(ch) < cost ) {
		send_to_char("§Aªº¥i¥Î§Þ¯à¸gÅç­È¤£°÷¤F¡C\n\r", ch);
		return;
	}

	sprintf(buf, "§A¥Î¥h¤F %d ÂI§Þ¯à¸gÅç­È¡C\n\r", skill_difficulty(ch,sn));
	send_to_char(buf,ch);

	act( "[36m§A´£¤É¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
	act( "[36m$n´£¤É¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );

	use_skill_exp(ch, skill_difficulty(ch,sn));
	gain_skill_level(ch,sn,1);

	if (IsUnderFistFight(ch)){
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	}else{
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}
}

void do_skdemote( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	//char buf[BUF_STRING_LENGTH];
	int sn;
	//int cost;

	if( IS_NPC(ch) ) return;
	if( ch->class < WIZARD ) {
		send_to_char("§A¥²¶·ÂàÂ¾«á¤~¯à­°§C§Þ¯àµ¥¯Å¡C\n\r", ch);
		return;
	}
	one_argument(argument, arg);

	if( arg[0] == '\0' ) {
		send_to_char("§A­n­°§C¤°»ò§Þ¯à¡H\n\r", ch);
		return;
	}
	sn = skill_lookup(arg);
	if( sn < 0 ) {
		send_to_char("¨S¦³³o­Ó§Þ¯à¡C\n\r", ch);
		return;
	}
	if(get_skill_percent(ch,sn) < 1 ) {
		send_to_char("§A¨Ã¤£·|³o¶µ§Þ¯à¡C\n\r", ch);
		return;
	}

	if( get_skill_level(ch,sn) < 1 ) {
		send_to_char("§A¦b³o¶µ§Þ¯à¤W¤w¸gµLªk¦A­°§C¤F¡C\n\r", ch);
		return;
	}

	if( sn == gsn_inborn_ability){
		AFFECT_DATA *paf = NULL;
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->type == gsn_polymorph){
				send_to_char("§A¤£¯à¦bÅÜ±Úªº±¡ªp­°§C§Aªº¤Ñ½á¡C\n\r", ch);
				return;
			}
		}
	}

	act( "[36m§A­°§C¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
	act( "[36m$n­°§C¤F$t($T)¡C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );

	gain_skill_level(ch,sn,-1);
	regain_skill_exp( ch, skill_difficulty(ch,sn)  );

	if (IsUnderFistFight(ch)) {
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	} else {
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}
}
