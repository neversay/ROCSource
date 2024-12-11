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
//*  PURPOSE     :         �Npractice��+�W��                      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         �ק�lv44�H�W�ݤ���clan_channel��bug    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         �ק�lv45�H�W�~�i�ϥ�tell�i��ť�j�\��   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         �ק�who imm��trust 43�]�|�X�{          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         �ק�tell�i��ť�j�W�[all ���\��         *
//*              :             tell�i��ť�j�W�[close ���\��       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 jye                                    *
//*  PURPOSE     :         �ק�who �W�U����bug�μW�[who�i�d��race *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         �ק�P�_souvenir�Ȩϧ���ܼ�           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         �s�WEXP_PER_LEVEL ppl_class�ܼ�        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050214 jye                                    *
//*  PURPOSE     :         �ק�do_exits��ܦr��"�V"���"��"       *
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
    "< ��  �� > ",
    "< ��  �� > ",
    "< ��  �� > ",
    "< �V  �� > ",
    "< �V  �� > ",
    "< ��  �� > ",
    "< �Y  �� > ",
    "< �L  �� > ",
    "< �}  �� > ",
    "< ��  �� > ",
    "< ��  �u > ",
    "< ��  �P > ",
    "<��¶����> ",
    "< �y  �� > ",
    "< ��  �� > ",
    "< ��  �� > ",
    "<�D�n�Z��> ",
    "< ��  �� > ",
    "<���n�Z��> ",
    "<����Z��> ",
    "< �I  �� > ",
    "",                    //  MAX_WEAR
    "< Ĳ  �� > ",
    "< Ĳ  �� > ",
    "< �M  �� > ",
    "<�� �� �u> ",
    "<�� �� ��> ",
    "<�� �L ��> ",
    "<�� �} ��> ",
    "< �u  �� > "         // wear loc ammo add at 2022/04/30
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
    if( IS_SET(obj->pIndexData->souvenir, Z1) ) strcat( buf, "(" HIW "��" NOR  ")"   );
    if ( IS_OBJ_STAT( obj, ITEM_INVIS)     )   strcat( buf, "(" YEL "��" NOR  ")"   );
    if ( ( IS_AFFECTED( ch, AFF_DETECT_EVIL  )
    || ( IS_SET( race_table[ ch->race ].race_abilities,
          RACE_DETECT_ALIGN )
        && !IS_EVIL( ch ) ) )
  && IS_OBJ_STAT( obj, ITEM_EVIL )   )   strcat( buf, "("HIR"��"NOR")"  );
    if ( ( IS_AFFECTED( ch, AFF_DETECT_MAGIC ) || IS_SET( race_table[ch->race].race_abilities,RACE_DETECT_MAGIC))
  && IS_OBJ_STAT( obj, ITEM_MAGIC )  )   strcat( buf, "("HIC"�]"NOR")"   );
    if ( IS_OBJ_STAT( obj, ITEM_GLOW )     )   strcat( buf, "("HIY"��"NOR")"   );
    if ( IS_OBJ_STAT( obj, ITEM_HUM )    )   strcat( buf, "("HIP"��"NOR")"   );
    if ( IS_OBJ_STAT( obj, ITEM_POISONED ) )   strcat( buf, "("HIG"�r"NOR")"  );
    if(!indestructible(obj) && ( get_curr_dura(obj) * 3 < get_max_dura(obj))) strcat( buf, "("HIP"�l"NOR")" );
    if( is_broken(obj) )strcat( buf, "("HIB"�a"NOR")" );




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
      strcat(buf, "("HIB"�������~"NOR")");
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
	if( IS_SET(obj->pIndexData->souvenir, Z1) ) strcat( buf, "(" HIW "����" NOR ")" );
	if ( IS_OBJ_STAT( obj, ITEM_INVIS) )   strcat( buf, "(" YEL "����" NOR  ")" );
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
				strcat( buf, "("HIU"���]"NOR")" );
			else if( enc == b )
				strcat( buf, "("HIG"��"NOR")" );
			else if( enc == c )
				strcat( buf, "("HIR"�`��"NOR")" );
			else if( enc == d )
				strcat( buf, "("HIC"�H��"NOR")" );
			else if( enc == e )
				strcat( buf, "("YEL"�Щ�"NOR")" );
			else if( enc == f )
				strcat( buf, "("HIY"�q��"NOR")" );
			else if( enc == g )
				strcat( buf, "("HIW"�{ģ"NOR")" );
			else if( enc == h )
				strcat( buf, "("HIB"��t"NOR")" );
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
		// �ק���]��� 2022/12/10
		if( enc > 0 ){
			strcat(buf,"(");
			if( enc > 1 ){ // �˳Ʀ���ӥH�W���Ȫ��], ��ܧܩʳ�@�r
				if( a > 0 )
					strcat( buf, HIU"�]"NOR );
				if( b > 0 )
					strcat( buf, HIG"��"NOR );
				if( c > 0 )
					strcat( buf, HIR"��"NOR );
				if( d > 0 )
					strcat( buf, HIC"�B"NOR );
				if( e > 0 )
					strcat( buf, YEL"�a"NOR );
				if( f > 0 )
					strcat( buf, HIY"�p"NOR );
				if( g > 0 )
					strcat( buf, HIW"��"NOR );
				if( h > 0 )
					strcat( buf, HIB"��"NOR );
			}else{         // �˳ƥu���@�إ��Ȫ��]
				if( a > 0 )
					strcat( buf, HIU"���]"NOR );
				if( b > 0 )
					strcat( buf, HIG"��"NOR );
				if( c > 0 )
					strcat( buf, HIR"�`��"NOR );
				if( d > 0 )
					strcat( buf, HIC"�H��"NOR );
				if( e > 0 )
					strcat( buf, YEL"�Щ�"NOR );
				if( f > 0 )
					strcat( buf, HIY"�q��"NOR );
				if( g > 0 )
					strcat( buf, HIW"�{ģ"NOR );
				if( h > 0 )
					strcat( buf, HIB"��t"NOR );
			}
			strcat(buf, ")");
		}
	}
	if (( IS_AFFECTED( ch, AFF_DETECT_EVIL )
		|| ( IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN )
		&& !IS_EVIL( ch ) ) )
		&& IS_OBJ_STAT( obj, ITEM_EVIL ) )
			strcat( buf, "("HIR"����"NOR")" );
	if (( IS_AFFECTED( ch, AFF_DETECT_MAGIC )
		|| IS_SET(race_table[ch->race].race_abilities, RACE_DETECT_MAGIC))
		&& IS_OBJ_STAT( obj, ITEM_MAGIC )  )
			strcat( buf, "("HIC"�]�O"NOR")" );
	if ( IS_OBJ_STAT( obj, ITEM_GLOW ) )
		strcat( buf, "("HIY"�o��"NOR")" );
	if ( IS_OBJ_STAT( obj, ITEM_HUM ) )
		strcat( buf, "("HIP"�C��"NOR")" );
	if ( IS_OBJ_STAT( obj, ITEM_POISONED ) )
		strcat( buf, "("HIG"�@�r"NOR")" );
	if(!indestructible(obj) && !is_broken(obj)&& ( get_curr_dura(obj) * 3 < get_max_dura(obj))) strcat( buf, "("HIP"�i�l"NOR")" );
	if( is_broken(obj) || IS_OBJ_STAT( obj, ITEM_BROKEN ) )strcat( buf, "("HIB"�l�a"NOR")" );




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
				strcat(buf, "("HIB"�������~"NOR")");
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
		// �ק�̤j pagelength , �G�i��ܤW������ 2023/01/12
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
  strcat( buf1, "���򳣨S���C\n\r" );
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

    send_to_char("���W�����~���G\n\r\n\r", ch);
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
		// �ק�̤j pagelength , �G�i��ܤW������ 2023/01/12
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
  strcat( buf1, "���򳣨S���C\n\r" );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow/*, count * sizeof( char * )*/ );
    free_mem( prgnShow/*,    count * sizeof( int )    */);

    send_to_char(buf1, ch);
//    sprintf( out, buf1 );
//    �����e�X��player�A�o�˥i�H���crash�����D
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

	// ����b�P�өж������ɶ������� 2023/01/17
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_IDLE ) && IS_IMMORTAL(ch) )
		strcat( buf, "("HIB"���m"NOR")" );

	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && can_see(ch, victim) )
		strcat( buf, "("HIY"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
		strcat( buf, "("YEL "��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_HIDE ) )
		strcat( buf, "("HIB"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_NIGHTSTALK))
		strcat( buf, "("HIU"�t"NOR")" );
	if ( IS_AFFECTED( victim, AFF_CHARM ) )
		strcat( buf, "("HIP"�b"NOR")" );
	if ( IS_AFFECTED( victim, AFF_CRAZY ) )
		strcat( buf, "("CYN"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_PASS_DOOR )
		|| ( IS_SET( race_table[ victim->race ].race_abilities, RACE_PASSDOOR )
		&& ( !str_cmp( race_table[ victim->race ].name, "Undead" )
		|| !str_cmp( race_table[ victim->race ].name, "Vampire" ) ) ) )
		strcat( buf, "("CYN"�z"NOR")");
	if ( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
		strcat( buf, "("HIP"��"NOR")"  );
	if ( IS_AFFECTED( victim, AFF_TARDY ) )
		strcat( buf, "("HIY"��"NOR")"  );
	if ( IS_EVIL( victim )
		&& ( IS_AFFECTED( ch, AFF_DETECT_EVIL )
		|| ( IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN )
		&& !IS_EVIL( ch ) ) ) )
		strcat( buf, "("HIR"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities, RACE_SANCT ) )
		strcat( buf, "("HIW"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FLAMING ) ) strcat( buf, "("HIP"��"NOR")" );
	if ( is_affected( victim, gsn_invoke ) )  strcat( buf, "("HIB"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_ARMOR) )strcat( buf, "("HIB"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_MIRROR) )strcat( buf, "("HIW"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MANA_SHIELD ) )strcat( buf, "("HIU"��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FEAR ) )strcat( buf, "("CYN"��"NOR")" );
	if( is_affected(victim, gsn_meditate)) strcat( buf, "("CYN"��"NOR")");
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER ) )
		strcat( buf, "("RED"��"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF ) )
		strcat( buf, "("RED"��"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_REGISTER ) )
		strcat( buf, "("HIG"PK"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AFK ) )
		strcat( buf, "("HIP"�b"NOR")" );

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
			/* �[�J switch �P�w ride ��e position ���A 2022/06/16
				strcat( buf, "�M�b");
				strcat( buf, pObj->description);
				strcat( buf, "�W�C ");
				strcat( buf, "\n\r" );
				buf[0] = UPPER( buf[0] );
				send_to_char( buf, ch );
				return;
			*/
			strcat( buf, "�M�b");
			strcat( buf, pObj->description);
			strcat( buf, "�W");
			switch ( victim->position )
			{
				case POS_DEAD:
					strcat( buf, "���F�I�I" ); break;
				case POS_MORTAL:
					strcat( buf, "���F�P�R�ˡC" ); break;
				case POS_INCAP:
					strcat( buf, "���h��ʯ�O�F�C" ); break;
				case POS_STUNNED:
					strcat( buf, "���ˡC" ); break;
				case POS_SLEEPING: 
					strcat( buf, "��ı�C" ); 
					break;
				case POS_RESTING:
					strcat( buf, "�𮧡C" ); break;
				case POS_STANDING: 
					if( is_flying( victim ) )
						strcat(buf, "�B�b�b�Ť��C");
					else
						strcat( buf, "�C" ); 
					break;
				case POS_FIGHTING:
					strcat( buf, "�M" );
					if ( !victim->fighting )
						strcat( buf, "�Ů�(�H)���[�C" );
					else if ( victim->fighting == ch )
						strcat( buf, "�A(�I)���[�C" );
					else if ( victim->in_room == victim->fighting->in_room )
					{
						strcat( buf, PERS( victim->fighting, ch ) );
						strcat( buf, "���[�C" );
					}
					else
					{
						strcat( buf, "���b�o�̪��֥��[�H" );
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
		case POS_DEAD:     strcat( buf, "���F�I�I" ); break;
		case POS_MORTAL:   strcat( buf, "���F�P�R�ˡC" ); break;
		case POS_INCAP:    strcat( buf, "���h��ʯ�O�F�C" ); break;
		case POS_STUNNED:  strcat( buf, "���˦b�a�C" ); break;
		case POS_SLEEPING: 
			//if( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_MEDITATING))
			//  strcat( buf, "�b�o�̭߷Q�C"        ); 
			//else
			strcat( buf, "�b�o�̺�ı�C" ); 
			break;
		case POS_RESTING:
			strcat( buf, "�b�o�̥𮧡C" ); break;
		case POS_STANDING: 
			if( is_flying( victim ) )
				strcat(buf, "�B�b�b�Ť��C");
			else
				strcat( buf, "���b�o��C"         ); 
			break;
		case POS_FIGHTING:
			strcat( buf, "���b�o�̩M" );
		if ( !victim->fighting )
			strcat( buf, "�Ů�(�H)���[�C" );
		else if ( victim->fighting == ch )
			strcat( buf, "�A(�I)���[�C" );
		else if ( victim->in_room == victim->fighting->in_room )
		{
			strcat( buf, PERS( victim->fighting, ch ) );
			strcat( buf, "���[�C" );
		}
		else
		{
			strcat( buf, "���b�o�̪��֥��[�H" );
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

	// ����b�P�өж������ɶ������� 2023/01/17
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_IDLE ) && IS_IMMORTAL(ch) )
		strcat( buf, "("HIB"���m"NOR")" );

	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS ) && can_see(ch, victim) )
		strcat( buf, "("HIY"����"NOR")" );
	if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
		strcat( buf, "("YEL "����"NOR")" );
	if ( IS_AFFECTED( victim, AFF_HIDE )  )
		strcat( buf, "("HIB"�ð�"NOR")" );
	if ( IS_AFFECTED( victim, AFF_NIGHTSTALK))
		strcat( buf, "("HIU"�t��"NOR")" );
	if ( IS_SET( victim->act, ACT_PET ) && IS_AFFECTED( victim, AFF_CHARM ) )
		strcat( buf, "("HIP"�d��"NOR")" );
	else if ( IS_AFFECTED( victim, AFF_CHARM ) )
		strcat( buf, "("HIP"�g��"NOR")" );

	if ( IS_AFFECTED( victim, AFF_CRAZY ) )
		strcat( buf, "("CYN"�ƨg"NOR")" );
	if ( IS_AFFECTED( victim, AFF_PASS_DOOR ) || ( IS_SET( race_table[ victim->race ].race_abilities, RACE_PASSDOOR )
		&& ( !str_cmp( race_table[ victim->race ].name, "Undead" ) || !str_cmp( race_table[ victim->race ].name, "Vampire" ) ) ) ){
		strcat( buf, "("CYN"�b�z��"NOR")");
	}
	if ( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
		strcat( buf, "("HIP"����"NOR")" );
	if ( IS_AFFECTED( victim, AFF_TARDY ) )
		strcat( buf, "("HIY"����"NOR")" );
	if ( IS_EVIL( victim ) && ( IS_AFFECTED( ch, AFF_DETECT_EVIL )
		|| ( IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ) && !IS_EVIL( ch ) ) ) ){
		strcat( buf, "("HIR"����"NOR")" );
	}
	if ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET( race_table[ victim->race ].race_abilities, RACE_SANCT ) )
		strcat( buf, "("HIW"�t��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FLAMING ) )
		strcat( buf, "("HIP"�K��"NOR")" );
	if ( is_affected( victim, gsn_invoke ) )
		strcat( buf, "("HIB"�\\��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_ARMOR) )
		strcat( buf, "("HIB"�]��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MYSTIC_MIRROR) )
		strcat( buf, "("HIW"�]��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_MANA_SHIELD ) )
		strcat( buf, "("HIU"�]��"NOR")" );
	if ( IS_AFFECTED( victim, AFF_FEAR ) )
		strcat( buf, "("CYN"����"NOR")" );
	if( is_affected(victim, gsn_meditate))
		strcat( buf, "("CYN"�߷Q"NOR")");
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER )  )
		strcat( buf, "("RED"����"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF  )  )
		strcat( buf, "("RED"�p��"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_REGISTER  )  )
		strcat( buf, "("HIG"PK"NOR")" );
	if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AFK )  )
		strcat( buf, "("HIP"�o�b"NOR")" );

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
			/* �[�J switch �P�w ride ��e position ���A 2022/06/16
				strcat( buf, "�M�b");
				strcat( buf, pObj->description);
				strcat( buf, "�W�C ");
				strcat( buf, "\n\r" );
				buf[0] = UPPER( buf[0] );
				send_to_char( buf, ch );
				return;
			*/
			strcat( buf, "�M�b");
			strcat( buf, pObj->description);
			strcat( buf, "�W");
			switch ( victim->position )
			{
				case POS_DEAD:
					strcat( buf, "���F�I�I" ); break;
				case POS_MORTAL:
					strcat( buf, "���F�P�R�ˡC" ); break;
				case POS_INCAP:
					strcat( buf, "���h��ʯ�O�F�C" ); break;
				case POS_STUNNED:
					strcat( buf, "���ˡC" ); break;
				case POS_SLEEPING: 
					strcat( buf, "��ı�C" ); 
					break;
				case POS_RESTING:
					strcat( buf, "�𮧡C" ); break;
				case POS_STANDING: 
					if( is_flying( victim ) )
						strcat(buf, "�B�b�b�Ť��C");
					else
						strcat( buf, "�C" ); 
					break;
				case POS_FIGHTING:
					strcat( buf, "�M" );
					if ( !victim->fighting )
						strcat( buf, "�Ů�(�H)���[�C" );
					else if ( victim->fighting == ch )
						strcat( buf, "�A(�I)���[�C" );
					else if ( victim->in_room == victim->fighting->in_room )
					{
						strcat( buf, PERS( victim->fighting, ch ) );
						strcat( buf, "���[�C" );
					}
					else
					{
						strcat( buf, "���b�o�̪��֥��[�H" );
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
		case POS_DEAD: strcat( buf, "���F�I�I" ); break;
		case POS_MORTAL: strcat( buf, "���F�P�R�ˡC" ); break;
		case POS_INCAP: strcat( buf, "���h��ʯ�O�F�C" ); break;
		case POS_STUNNED: strcat( buf, "���˦b�a�C" ); break;
		case POS_SLEEPING: strcat( buf, "�b�o�̺�ı�C" ); break;
		case POS_RESTING: strcat( buf, "�b�o�̥𮧡C" ); break;
		case POS_STANDING:
			if( is_flying(victim) )
				strcat(buf, "�B�b�b�Ť��C");
			else
				strcat( buf, "���b�o��C" ); 
			break;

		case POS_FIGHTING:
			strcat( buf, "���b�o�̩M" );
			if ( !victim->fighting )
				strcat( buf, "�Ů�(�H)���[�C" );
			else if ( victim->fighting == ch )
				strcat( buf, "�A(�I)���[�C" );
			else if ( victim->in_room == victim->fighting->in_room )
			{
				strcat( buf, PERS( victim->fighting, ch ) );
				strcat( buf, "���[�C" );
			}
			else
			{
				strcat( buf, "���b�o�̪��֥��[�H" );
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
        act( "[36m$n�J�ӥ��q�ۧA�C[m", ch, NULL, victim, TO_VICT    );
        act( "[36m$n�J�ӥ��q��$N�C[m",  ch, NULL, victim, TO_NOTVICT );
    }

    race = ( ( victim->temp_race ) ? victim->temp_race - 1 : victim->race );

	// �s�W aff_ghoul �� stone form �����p�U�b�رڦW�e��ܤ����t 2022/01/16
	//sprintf( buf1, "$N - %s(%s):\n\r", race_table[ race ].cname, race_table[ race ].name );
	if( is_affected(victim, gsn_stone_form)
		|| (  !IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD )
			&& IS_AFFECTED( victim, AFF_GHOUL ) ) )
		sprintf( buf1, "$N - �����t%s(%s):\n\r", race_table[ race ].cname, race_table[ race ].name );
	else
		sprintf( buf1, "$N - %s(%s):\n\r", race_table[ race ].cname, race_table[ race ].name );


    if ( victim->description[0] != '\0' ){
        send_to_char(victim->description, ch);
        send_to_char("\n\r", ch);
    }else{
        strcat( buf1, "$E�ݰ_�ӨS������S��C" );
    }

    if ( victim->max_hit > 0 )
        percent = ( 100 * victim->hit ) / victim->max_hit;
    else
        percent = -1;

    sprintf( buf, PERS( victim, ch ) );

    if ( percent >= 100 ) strcat( buf, HIG"���d���p�}�n�C"NOR"\n\r"  );
    else if ( percent >=  90 ) strcat( buf, HIG"���F���L�����ˡC"NOR"\n\r" );
    else if ( percent >=  80 ) strcat( buf, HIG"���W���@�I�p���C"NOR"\n\r"   );
    else if ( percent >=  70 ) strcat( buf, HIY"���W���@�Ƕ˲��C"NOR"\n\r"     );
    else if ( percent >=  60 ) strcat( buf, HIY"���F�p�ˡC"NOR"\n\r"    );
    else if ( percent >=  50 ) strcat( buf, HIY"�ݨӪ��p�ä��Ӧn�C"NOR"\n\r" );
    else if ( percent >=  40 ) strcat( buf, HIR"�˲��ֲ֡C"NOR"\n\r"    );
    else if ( percent >=  30 ) strcat( buf, HIR"�����O��C"NOR"\n\r"   );
    else if ( percent >=  20 ) strcat( buf, HIR"���F�Q���Y�����ˡA�H�ɳ��i��w�ˡC"NOR"\n\r"   );
    else if ( percent >=  10 ) strcat( buf, RED"�w�g�i�H�ݨ즺����L�L���C"NOR"\n\r"  );
    else           strcat( buf, RED"�w�O�i��֡A�X��h�A���ݴN�n���h�C"NOR"\n\r"      );

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
                    strcat( buf1, "$E���W��ۡG\n\r" );
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
                      strcat( buf1, "$E���W��ۡG\n\r" );
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
                      strcat( buf1, "$E���W��ۡG\n\r" );
                      found = TRUE;
                  }
                  strcat( buf1, where_name[WEAR_FEET_2] );
                  strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                  strcat( buf1, "\n\r" );
                  ++count;
              }
            }
		}
		// �s�W�u�ĸ˳� 2022/04/30
		if ( ( obj = get_eq_char( victim, WEAR_AMMO ) ) && can_see_obj( ch, obj ) ){
			if ( !found ){
				strcat( buf1, "\n\r" );
				strcat( buf1, "$E���W��ۡG\n\r" );
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
                strcat( buf1, "$E���W��ۡG\n\r" );
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
                    strcat( buf1, "$E���W��ۡG\n\r" );
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
                    strcat( buf1, "$E���W��ۡG\n\r" );
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
                     strcat( buf1, "$E���W��ۡG\n\r" );
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
                strcat( buf1, "$E���W��ۡG\n\r" );
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
                strcat( buf1, "$E���W��ۡG\n\r" );
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
                strcat( buf1, "$E���W��ۡG\n\r" );
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
                     strcat( buf1, "$E���W��ۡG\n\r" );
                     found = TRUE;
                 }
                 strcat( buf1, where_name[iWear] );
                 strcat( buf1, format_obj_to_char( obj, ch, TRUE ) );
                 strcat( buf1, "\n\r" );
                 ++count;
            }
        }
		// �s�W�u�ĸ˳� 2022/04/30
		if ( ( obj = get_eq_char( victim, WEAR_AMMO ) ) && can_see_obj( ch, obj ) ){
			if ( !found ){
				strcat( buf1, "\n\r" );
				strcat( buf1, "$E���W��ۡG\n\r" );
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
                strcat( buf1, "$E���W��ۡG\n\r" );
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
            strcat( buf1, "\n\r�@�ة_�Ǫ��O�q�����A���s���I\n\r" );
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
			send_to_char( "�A�P�즳�H���`���ۧA�I\n\r", ch );
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
  send_to_char( "�A���򳣬ݤ���I\n\r", ch );
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
      sprintf(buf, "%s[%2d] %-20s %s %5d %s�C"NOR"\n\r",
          ( str_cmp(td[i].id, ch->name) ) ? HIW: HIY,
          i + 1,
          td[i].id,
          prefix,
          td[i].value,
          posfix);
    } else {
      sprintf(buf, "%s[%2d] %-20s %s %5d %s�C"NOR"\n\r",
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
    send_to_char("�п�J�Q�ݪ��ƦW�����C\n\r", ch );
    send_to_char("�i��ܪ��������Ghp, mana, move, resp, merit, gold, "
                 "mudage, pk\n\r", ch);
    return;
  }

  if( !str_cmp( arg, "reset" )){
    if( !IS_IMMORTAL( ch ) ) {
      send_to_char("����H\n\r", ch);
      return;
    }
    if( IS_NPC(ch) || get_trust(ch) < 45 ) {
      send_to_char("�A���v�������C\n\r", ch);
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
    show_top10(ch, tpk, "[�ۥ@PK���Ʀ�]", "PK ����", "��");
  } else if(!str_cmp(arg, "mudage")) {
    show_top10(ch, tmudage, "[�ۥ@�W�u�ɶ��Ʀ�]", "�W�u�ɶ�", "�p��");
  } else if(!str_cmp(arg, "gold")) {
    show_top10(ch, tgold, "[�ۥ@ 50 �j�I���Ʀ�]", "�]���@��", "�T�v��");
  } else if(!str_cmp(arg, "exp")) {
    show_top10(ch, texp, "[�ۥ@ 50 �j���αƦ�]", "�g���I��", "�I");
  } else if(!str_cmp(arg, "hp")) {
    show_top10(ch, thp, "[�ۥ@ 50 �j����Ʀ�]", "�ͩR�O", "�I");
  } else if(!str_cmp(arg, "mana")) {
    show_top10(ch, t_mp, "[�ۥ@ 50 �j�k�O�Ʀ�]", "�k�O", "�I");
  } else if(!str_cmp(arg, "move")) {
    show_top10(ch, tmv, "[�ۥ@ 50 �j���Ʀ�]", "��O", "�I");
  } else if(!str_cmp(arg, "resp")) {
    show_top10(ch, tresp, "[�ۥ@ 50 �j�W�H�Ʀ�]", "�±��", "�I");
  } else if(!str_cmp(arg, "merit")) {
    show_top10(ch, tmerit, "[�ۥ@ 50 �j�\\���Ʀ�]", "�\\����", "�I");
  }

  /* OLD TOP10

  if( !str_cmp( arg, "pk" )){
  send_to_char("[�ۥ@�Q�jPK���Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s PK ���� %4d �ӡC%s\n\r",
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
  send_to_char("[�ۥ@�Q�j�W�u�ɶ��Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s �W�u�ɶ� %4d �p�ɡC%s\n\r",
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
  send_to_char("[�ۥ@�Q�j�I���Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s �]���@�� %d �T�����C%s\n\r",
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
  send_to_char("[�ۥ@�Q�j����Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s �g���I�� %10d �I�C%s\n\r",
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
  send_to_char("[�ۥ@�Q�j����Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s �ͩR�I�� %5d �I�C%s\n\r",
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
  send_to_char("[�ۥ@�Q�j�]�k����Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s �]�O�I�� %5d �I�C%s\n\r",
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
  send_to_char("[�ۥ@�Q�j��O����Ʀ�]\n\r========================================================================\n\r", ch);

  for( i=0; i< 10; ++i){
      sprintf(buf, "%s[%2d] %-20s ���ʤO %5d �I�C%s\n\r",
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
      send_to_char("[�ۥ@�Q�j�W�H�Ʀ�]\n\r========================================================================\n\r", ch);
       for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s �±�� %5d �I�C%s\n\r",
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
      send_to_char("[�ۥ@�Q�j�\\���Ʀ�]\n\r========================================================================\n\r", ch);
       for( i=0; i< 10; ++i){
        sprintf(buf, "%s[%2d] %-20s �\\���� %5d �I�C%s\n\r",
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
		send_to_char( "[33m�A�u��ݨ�P�P�Ӥw�I�h���R�ڡI[m\n\r",ch );
		return;
	}

	if ( ch->position == POS_SLEEPING )
	{
		send_to_char( "[33m�A�u��ݨ�ڹҦӤw�IZzz...[m\n\r", ch );
		return;
	}

	if ( !check_blind( ch ) )
		return;

	if ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_HOLYLIGHT ) && room_is_dark( ch->in_room ) )
	{
		send_to_char( "�@������....\n\r", ch );
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
			send_to_char( "�o�̲��`�a�w�R�C\n\r", ch );

		// evacuate mark
		if ( (get_skill_level(ch,gsn_evacuate) > 2) && IS_SET( ch->in_room->room_flags, ROOM_CAMP ) )
			send_to_char(HIU"�o�̪��a���y«���]�O�C"NOR"\n\r", ch );

	#ifdef HAS_ROOM_AFFECT
	  
		if( IS_AFFECTED(ch->in_room, R_AFF_FIREWALL))
		{
		  send_to_char(HIR"\n\r\t�@�D���𥿦b�o�̿U�N�ۡC"NOR"\n\r", ch);
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
						send_to_char( "�A�P�즳�H���`���ۧA�I\n\r", ch );
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
			send_to_char( "[33m�n������̭��ݡH[m\n\r", ch );
			return;
		}

		if ( !( obj = get_obj_here( ch, arg2 ) ) )
		{
			send_to_char( "[33m�A�b�o�̨S�ݨ쨺���N��C[m\n\r", ch );
			return;
		}

		// ���Q look in �� obj type �s�W obj_act look_in 2022/02/12
		switch ( obj->item_type )
		{
			default:
				send_to_char( "[33m�����O�Ӯe���C[m\n\r", ch );
				break;

			case ITEM_DRINK_CON:
				if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
				{
					bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
					obj->value[2] = 0;
				}
				if ( obj->value[1] <= 0 && obj->value[1] != -1 )
				{
					send_to_char( "[36m���O�Ū��C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}
				sprintf( buf, "���̭���%s%s�G��C\n\r",
				(obj->value[1] == -1) 
				? "�p�P�p�t�z�륨�q��" : 
				obj->value[1] <     obj->value[0] / 4 
				? "����@�b��" : 
				obj->value[1] < 3 * obj->value[0] / 4 
				? "�j���@�b��" :       "�W�L�@�b��",
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
					send_to_char( "[36m���O�����C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}
				sprintf( buf, "���̭��O%s%s�C\n\r",
				(obj->value[1] == -1) 
				? "����������" : 
				obj->value[1] <     obj->value[0] / 4 
				? "������" : 
				obj->value[1] < 3 * obj->value[0] / 4 
				? "�w�w�y�ʪ�" :       "�F�F��",
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
					send_to_char( "[33m���O���۪��C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}

				act( "$p�̭����G", ch, obj, NULL, TO_CHAR );
				show_list_to_char( obj->contains, ch, TRUE, TRUE );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
			case ITEM_HORSE:
				if ( obj->value[0] == -1 )
				{
					send_to_char( "[33m�e���W�S���t������F��C[m\n\r", ch);
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break; 
				}

				act( "$p���W�t���ۡG", ch, obj, NULL, TO_CHAR );
				show_list_to_char( obj->contains, ch, TRUE, TRUE );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
			case ITEM_ARROW:
				if ( obj->value[0] <= 0 )
				{
					// �W�[ -1 �����p 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m�̭��b�ڪ��ƶq���p�E�v���p�t�z�C[m\n\r", ch );
					else
						send_to_char( "[36m���O�Ū��C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}

				sprintf( buf, "���̭��٦�%d��b�C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;
				
			case ITEM_AMMO:   // add at 2022/05/06
				if ( obj->value[0] <= 0 )
				{
					// �W�[ -1 �����p 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m�̭��u�Ī��ƶq���p�E�v���p�t�z�C[m\n\r", ch );
					else
						send_to_char( "[36m���O�Ū��C[m\n\r", ch );
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("look_in", ch, obj);
					}
					break;
				}

				sprintf( buf, "�٦�%d�o�u�ġC\n\r", obj->value[0] );
				send_to_char( buf, ch );
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("look_in", ch, obj);
				}
				break;


			case ITEM_DART:

				sprintf( buf, "�٦�%d��D��C\n\r", obj->value[0] );
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
	//  �� get_obj_here ���N obj �� pfx �P�w
	// �H���קK�S�w��m�줣�� obj name ������ܦW�� ( eg: look 2.sword , look 3.wa )
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
		send_to_char( "[33m�A�b�o�̨S�ݨ쨺���N��C[m\n\r", ch );
		return;
	}

	/* 'look direction' */
	if ( !( pexit = ch->in_room->exit[door] ) )
	{
		send_to_char( "[33m���̨S����S�O���F��C[m\n\r", ch );
		return;
	}

	if ( pexit->description && pexit->description[0] != '\0' )
		send_to_char( pexit->description, ch );
	else
		send_to_char( "[33m���̨S����S�O���F��C[m\n\r", ch );

	if (   pexit->keyword
		&& pexit->keyword[0] != '\0'
		&& pexit->keyword[0] != ' ' )
	{
		if ( IS_SET( pexit->exit_info, EX_BASHED ) )
			act( "[36m$d�Q���a�F�C[m", ch, NULL, pexit->keyword, TO_CHAR );
		else if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
			act( "[36m$d���F�C[m", ch, NULL, pexit->keyword, TO_CHAR );
		else if ( IS_SET( pexit->exit_info, EX_ISDOOR ) )
			act( "[36m$d�O�}�۪��C[m",   ch, NULL, pexit->keyword, TO_CHAR );
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
		send_to_char( "[33m�n�ˬd���򪫫~�H[m\n\r", ch );
		return;
	}


	if ( ( obj = get_obj_here( ch, arg ) ) )
	{
		if ( !obj ) return;
		int diff;
		// �����H do_look ����ܪ��~�W�Χ��� 2022/02/01
		//do_look( ch, argument );
		send_to_char( obj->short_descr , ch);
		send_to_char( " - ", ch);
		send_to_char( obj->material->short_descr, ch);
		send_to_char("\n\r", ch);
		send_to_char( obj->description, ch );
		send_to_char( "\n\r", ch );
		// ���� serial ��� 2022/02/02
		//sprintf(buf,"���~�Ǹ��G%ld\n\r", (long)obj->serial);
		//send_to_char(buf, ch);
		// �s�W obj_act examine , examine �Φb���� item type �W�i��|�P look_in ���� 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("examine", ch, obj);
		}
		switch ( obj->item_type )
		{
			default:
				break;

			case ITEM_LIGHT:
				if( obj->value[2] != -1 ){
					sprintf(buf, "�ݨ��ٯ�ϥ� %d �p�ɡC\n\r", obj->value[2] );
					send_to_char(buf, ch);
				}
				break;

			/* �Ncase armor �� weapon ���}�P�w 2021/12/13 */
			case ITEM_ARMOR:
				send_to_char("�A�J�ӥ��q����A�o�{��", ch);
				if( obj->value[1] != 0)
					send_to_char("����F�G�k�A�~�[�ݰ_��", ch);

				if( indestructible(obj) ) {
					send_to_char(HIW"���B�b���������p�U�C"NOR"\n\r", ch);
					break;
				}else if ( get_max_dura(obj) == 0 ) {
					send_to_char(RED"�n���@�����a�N�L�k�״_�C"NOR"\n\r", ch);
					break;
				}
				diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);
				if( diff == 100 ) send_to_char(HIW"���B�b���������p�U"NOR"�C\n\r", ch);
				else if( diff > 70) send_to_char(HIC"�ݰ_�Ӭ۷�}�n"NOR"�C\n\r", ch);
				else if( diff > 50) send_to_char(HIY"�y�L���Ǩ�"NOR"�C\n\r", ch);
				else if( diff > 33) send_to_char(YEL"���p���Ӧn"NOR"�C\n\r", ch);
				else if( diff > 20) send_to_char(HIR"�t���h���a�F"NOR"�C\n\r", ch);
				else if( diff > 10) send_to_char(RED"�W�Y������@�P�̰ʵ�"NOR"�C\n\r", ch);
				else if( diff > 0 ) send_to_char(RED"�n�������@�I�N�|����"NOR"�C\n\r", ch);
				else send_to_char(HIB"�w�g�����l�a�A�����ϥ�"NOR"�C\n\r", ch);
				break;

			case ITEM_WEAPON:
				send_to_char("�A�J�ӥ��q����A�o�{��", ch);
				/* �s�W weapon type ��� 2021/12/13 */
				if( obj->value[3] == 1 )
					send_to_char("��@���Q�b���ΡA�~�[�ݰ_��", ch);
				else if( obj->value[3] == 2 )
					send_to_char("��@�����j���A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 3 )
					send_to_char("��@���M�C�A��A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 4 )
					send_to_char("��@���@�l���R�A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 5 )
					send_to_char("��Ψӧ������A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 6 )
					send_to_char("��Q�X�]�k�A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 7 )
					send_to_char("��ΨӺV�����A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 8 )
					send_to_char("��@�����񯻸H���A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 9 )
					send_to_char("��@�������Q���A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 10 )
					send_to_char("��r��ͪ��A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 11 )
					send_to_char("��@���P�������A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 12 )
					send_to_char("��l��ͪ��A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 13 )
					send_to_char("��@����M�s�١A�~�[�ݰ_��", ch);
				else if( obj->value[3] == 14 )
					send_to_char("��@���}�ϥΡA�~�[�ݰ_��", ch);
				else if( obj->value[3] == 16 )  // add at 2022/05/06
					send_to_char("��@���j��ϥΡA�~�[�ݰ_��", ch);

				if( indestructible(obj) ) {
					send_to_char(HIW"���B�b���������p�U"NOR"�C\n\r", ch);
					break;
				}else if ( get_max_dura(obj) == 0 ) {
					send_to_char(RED"�n���@�����a�N�L�k�״_"NOR"�C\n\r", ch);
					break;
				}
				diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);
				if( diff == 100 ) send_to_char(HIW"���B�b���������p�U"NOR"�C\n\r", ch);
				else if( diff > 70) send_to_char(HIC"�ݰ_�Ӫ��p�n����"NOR"�C\n\r", ch);
				else if( diff > 50) send_to_char(HIY"�y�L���Ǩ�"NOR"�C\n\r", ch);
				else if( diff > 33) send_to_char(YEL"���p���Ӧn"NOR"�C\n\r", ch);
				else if( diff > 20) send_to_char(HIR"�t���h���a�F"NOR"�C\n\r", ch);
				else if( diff > 10) send_to_char(RED"�W�Y������@�P�̰ʵ�"NOR"�C\n\r", ch);
				else if( diff > 0 ) send_to_char(RED"�n�������@�I�N�|����"NOR"�C\n\r", ch);
				else send_to_char(HIB"�w�g�����l�a�A�����ϥ�"NOR"�C\n\r", ch);
				break;

			case ITEM_DRINK_CON:
			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_CORPSE_PC:
			case ITEM_FOUNTAIN:
				send_to_char( "�A���̭��ݪ��ɭԡA�A�ݨ�G\n\r", ch );
				sprintf( buf, "in %s", arg );
				do_look( ch, buf );
				break;

			case ITEM_ARROW:
				if ( obj->value[0] <= 0 ){
					// �W�[ -1 �����p 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m�̭��b�ڪ��ƶq���p�E�v���p�t�z�C[m\n\r", ch );
					else
						send_to_char( "[36m���O�Ū��C[m\n\r", ch );
					break;
				}
				sprintf( buf, "���̭��٦�%d��b�C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				break;
			case ITEM_AMMO:  // add at 2022/05/06
				if ( obj->value[0] <= 0 ){
					// �W�[ -1 �����p 2023/01/05
					if ( obj->value[0] == -1 )
						send_to_char( "[36m�̭��u�Ī��ƶq���p�E�v���p�t�z�C[m\n\r", ch );
					else
						send_to_char( "[36m���O�Ū��C[m\n\r", ch );
					break;
				}
				sprintf( buf, "�٦�%d�o�u�ġC\n\r", obj->value[0] );
				send_to_char( buf, ch );
				break;
			case ITEM_DART:
				sprintf( buf, "�٦�%d��D��C\n\r", obj->value[0] );
				send_to_char( buf, ch );
				break;
			case ITEM_HORSE:
				send_to_char( "�A�������W��ۡG\n\r",ch);
				sprintf( buf, "in %s",arg);
				do_look(ch,buf);
				break;
		}
	}
	
	// �S���s�@ arg ���F��b�ж��� 2022/02/02
	if(!obj)
		send_to_char( "[33m�A�b�o�̨S�ݨ쨺���N��C[m\n\r", ch );

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

	strcpy( buf, fAuto ? "[" HIW"�X�f"NOR":" : HIW"���㪺�X�f"NOR":\n\r" );

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
				sprintf( buf + strlen( buf ), "%-5s �� %s\n\r",
				capitalize( dir_name[door] ),
				room_is_dark( pexit->to_room )
				?  "�o��ӷt�F"
				: pexit->to_room->name
				);
			}
		}
	}

	if ( !found )
		strcat( buf, fAuto ? " "HIC"�L"NOR : HIC"�L"NOR"�C\n\r" );

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
	// �ק� align color �϶��P�r���ܤƬ۲� 2023/01/16
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
    send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
    return;
  }
	if(!(victim->pIndexData != NULL && IsSorGateMob(victim->pIndexData->vnum))){
		send_to_char( "�L���O�l�ꪫ.\n\r", ch);
		return;
	}
	// �קאּ�l�ꪫ remb ������ 2022/12/20
	//if(victim->master != ch){
	if( victim->rembch != ch ){
		send_to_char( "�O�Q�����O�H�l�ꪫ�����\n\r", ch);
		return;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	MOB_WEAKNESS *pWeak  = NULL;

	char          rgLore[MAX_STRING_LENGTH];
	char          rgBuf[MAX_STRING_LENGTH];
	char          rgSec1[200], rgSec2[200], rgSec3[200];
	char          rgSec4[200], rgSec5[200], rgSec6[500];

	rgLore[0] = 0;
  sprintf(rgBuf, "�A���l�ꪫ�V�A���i�L�����A�C\n\n\r"); 
	strcat(rgLore, rgBuf);

  // Mob basic Info
	sprintf(rgBuf, "������������������������������������������������������������\n\r" );
	strcat(rgLore, rgBuf);
	sprintf(rgBuf, "%s , %s:\n\r�رڡG%s(%s)  ���šG%2d  �ޯ൥�šG%d  �ʧO�G%s  �}��G%d\n\r", 
	               victim->short_descr, victim->name,
	               race_table[ victim->race ].cname, race_table[ victim->race ].name,
	               victim->level, GetGeneralMobSklv(victim) ,
	               victim->sex == SEX_MALE ? "�k" :
	               victim->sex == SEX_FEMALE ? "�k" : "��",
								 victim->alignment);
	strcat(rgLore, rgBuf);

	// Five Basic Attributes
	sprintf(rgBuf, "�O�q�G%2d/%2d  ���O�G%2d/%2d  ���z�G%2d/%2d   �ӱ��G%2d/%2d   ���G%2d/%2d \n\r",
	               get_curr_str( victim ), get_max_str( victim ), 
								 get_curr_int( victim ), get_max_int( victim ),
								 get_curr_wis( victim ), get_max_wis( victim ),
								 get_curr_dex( victim ), get_max_dex( victim ),
	               get_curr_con( victim ), get_max_con( victim ));
	strcat(rgLore, rgBuf);

	// HP/MP/MV Info
	sprintf(rgBuf, "�ͩR�O�G%4d/%4d   �]�k�O�G%4d/%4d  ���ʤO�G%4d/%4d\n\r",
								 victim->hit,  victim->max_hit,
								 victim->mana, victim->max_mana,
								 victim->move, victim->max_move);

	strcat(rgLore, rgBuf);

	// Fighting Related
	strcat(rgLore, "\n\r�԰���O��ơG\n\r");
	sprintf(rgSec1, "%5d", get_hitroll(victim, WEAR_WIELD));
	sprintf(rgSec3, "%4d", get_damroll(victim, WEAR_WIELD));
	sprintf(rgBuf, "�D�n�Z���R���G%s   �D�n�Z���ˮ`�G%s\n\r", 
	               rgSec1, rgSec3);
	strcat(rgLore, rgBuf);

	if(get_eq_char( victim, WEAR_WIELD_2 )){
		sprintf(rgSec2, "%5d", get_hitroll(victim, WEAR_WIELD_2));
		sprintf(rgSec4, "%4d", get_damroll(victim, WEAR_WIELD_2));
 		sprintf(rgBuf, "���n�Z���R���G%s   ���n�Z���ˮ`�G%s\n\r",
		             rgSec2, rgSec4);
		strcat(rgLore, rgBuf);
	}

	sprintf(rgSec4, "%5d", victim->pp);
	sprintf(rgSec5, "%4d", victim->apdr);
	sprintf(rgSec6, "%4d", victim->amdr);
	sprintf(rgBuf, "    �]�k��O�G%s   ���z�ˮ`�G%s  �]�k�ˮ`�G%s\n\r", 
	               rgSec4, rgSec5, rgSec6);
	strcat(rgLore, rgBuf);

	// Physical Resistance 
	strcat(rgLore, "\n\r���m�򥻸�ơG\n\r");
	sprintf(rgSec4, "%4d", victim->pdr);
	sprintf(rgSec5, "%4d", victim->pad);
	
	sprintf(rgBuf, "    �@�Ҩ��áG%5d   ���z�ܩʡG%s  ���z�پ��G%s\n\r",
			            GET_AC( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Dodge and Magical Resistance 
	sprintf(rgSec4, "%4d", victim->mdr);
	sprintf(rgSec5, "%4d", victim->mad);
	
	sprintf(rgBuf, "    �{�ׯ�O�G%5d   �]�k�ܩʡG%s  �]�k�پ��G%s\n\r",
	                GET_DODGE( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Magical Resistance in Elemental
	strcat(rgLore, "\n\r�����]�k�ܩʡG\n\r");
	sprintf(rgSec6, "    ���]�k�G%4d   �ܪ��V�G%4d   �ܵ����G%4d\n\r",
									GET_SVS(victim),
									NEW_GET_SVS(victim, MAGIC_FIRE),
									NEW_GET_SVS(victim, MAGIC_WIND));
	strcat(rgLore, rgSec6);

	sprintf(rgSec6, "    �ܹp�{�G%4d   �ܦB���G%4d   �ܤj�a�G%4d\n\r", 
									NEW_GET_SVS(victim, MAGIC_LIGHTNING),
									NEW_GET_SVS(victim, MAGIC_COLD),
									NEW_GET_SVS(victim, MAGIC_EARTH));
	strcat(rgLore, rgSec6);

	strcat(rgLore, "\n\r�S���]�k�ܩʡG\n\r");
	sprintf(rgSec4, "    ���s���G%4d   �ܯ��t�G%4d   ����¡G%4d\n\r",
									NEW_GET_SVS(victim, MAGIC_BREATH), 
									NEW_GET_SVS(victim, MAGIC_SAINT), 
									NEW_GET_SVS(victim, MAGIC_DARK));
	strcat(rgLore, rgSec4);

	// Weakness
  strcat(rgLore, "\n\r��S��ޯ઺�ܩʱM���ήz�I�G\n\r");
	for(pWeak = victim->weakness, rgBuf[0] = 0; 
			pWeak != NULL; 
			pWeak = pWeak->next){
		sprintf(rgSec6, "[%20s] �ޯ�R���[�� %3d%%�A�ˮ`�[�� %3d%%�C\n\r",
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

			if(nAffectCnt == 0) strcat( rgLore, "������H�U�S��v�T�G\n\r" );
			nAffectCnt++;

			sprintf( rgBuf, "�S�ġG�u"HIC"%s"NOR"(%s)�v", skill_table[paf->type].cname, skill_table[paf->type].name );
			strcat( rgLore, rgBuf );

			strcat( rgLore, "(");
			if( strcmp(c_affect_loc_name(paf->location), "�L")){
				sprintf( rgBuf, "�v�T"HIY"%s"NOR, c_affect_loc_name( paf->location ) );
				strcat( rgLore, rgBuf );
			}
			if( paf->modifier ){
				sprintf( rgBuf, " %s%d "NOR"�I  ",
				         paf->modifier<0? HIU"-":( paf->modifier == 0 ? HIW:HIC"+"),
				         paf->modifier<0?-paf->modifier:paf->modifier
			         );
				strcat( rgLore, rgBuf );
			}
			if( paf->duration >= 0 ) {
				sprintf( rgBuf, "�|�� "HIG"%d"NOR" �p�ɪ��ĥΡC", paf->duration );
				strcat( rgLore, rgBuf );
			} else {
				sprintf(rgBuf, "����@�Τ��C");
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
			send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
			return;
		}

		if( (!IS_SET( victim->act, PLR_HORSE )
			|| victim != ch->pcdata->horse
			|| victim->in_room != ch->in_room)
			&& ( ch->desc && ch->desc->original != victim )
			&& victim != ch
			&& !IS_IMMORTAL( ch ))
		{
			send_to_char("�A����ݧO�H����ơC\n\r", ch);
			return;
		}
		one_argument( argument, key );
		if( !key || key[0] == '\0' )
			sprintf( key, "all" );
	}
/*
�b�b�c�c�d�d�e�e�f�f�g�g�h�h�i�� �� �� �� �@ ���i�h�h�g�g�f�f�e�e�d�d�c�c�b�b
<PKer> <����> <�p��> Zcecil, Raidiant Spirit  ½Ķ����
�ʧO�G�k��    �رڡG��          PK�����G  12�� / 129��
���šG39      ¾�~�G����v      �~�֡G 39���]�u��ɶ� 178�p�ɡ^
�O�q�G30  ���O�G30  ���z�G30  �ӱ��G30  ���G30
���~�G1000/1000 ��, ���q�G1000000/1000000 ����    �����G20 ��
�ͩR�O�G99999/99999 �I, �]�O�G99999/99999 �I, ��O�G99999/99999 �I
Wimpy �G99999 �I,       �_���I�ơG 50 �I
����  �G2147283647 ��,  �s�ڡG2147483647 ��
Autogold: �O    Autoloot: �O      Autosac: �O    Autoexit: �O
Wizinvis: �_    Holylight: �O
�}  ��G990, �A���p�Ѩϯ뵽�}.
�g��ȡG12345678 / MAX       �m���I�ơG1234 �I
�R���v�G1222+22 �I       �ˮ`�O�G1231+21 �I      ���]�O�G-123 �I
�i  ��G999/299 �I, �v�T�R���v 10 �I, �ˮ`�O 10 �I.
���m�O�G-9999 �I, �A���Z�˯u���i��ĳ�I
�A������.�A�{�b���A�X�@�{��԰�.
�A���Q�����u��(Cityguard)�y��.
�A���y���ۥ���(Mayor).
�A���W���@�εۥH�U�k�N�G
�ޯ�G���O���m(defence), �v�T�R���v -3 �I, �٦� 1111 �p�ɪ��ĥ�.
������������������������������������������������������������������������
                    IS_SET( wch->act, PLR_KILLER   ) ? "�]"RED"����"NOR"�^" : "",
                            IS_SET( wch->act, PLR_THIEF    ) ? "�]"RED"�p��"NOR") "  : "",
                              IS_SET( wch->act, PLR_REGISTER ) ? "�]"HIG"PK"NOR"�^"    
*/
  /*
<PK>Reticent, ���a�O�s���������˳� QQ  Reticent(Reticent)
�A�O�@�� 20 ���� 42 �Ũk�ʤH���ڨg�Ԥh�ǩ_�C(�W�u�ɶ� 50 �p��)
�A�������O: �����t�ϹΡC        �±�:10, �A�}�l�C�C���S�Y���C
�O�q: 17/16  ���O: 14/17  ���z: 16/16  �ӱ�: 15/17  ���: 17/22
�ͩR�O: 589/1856      �]�k�O: 200/1000      ���ʤO: 1061/1061
PK ����:  0 ��/ 0 ��    �g��ȡG6311435/MAX        �m���I��: 351
�}   ��: -1000, �A�����H�ҶA�G�A²���N�O�����������C
                                                                                
�A�ثe���@�ԫ��A��: ��������C    �ͩR�O�֩�   0 �I�ɦ۰ʰk�]�C
�}��: �L      �{�b���A: �f���B�{�l�j�F�B�𮧡B�{��i��
���~: 56/72   �t��: 414/500 ����  ����: 1000000 ��  �s��: 1000000 ��
����: 20 ��   Autoexit: �O   Autoloot: �_   Autogold: �O Autosac: �_
                                                                                
�D�n�Z���R���I��: 52   �{  ��  �v:   14, �A�񽽤��֤��F�h�֡C
�D�n�Z�������I��: 69   �@�Ҩ��äO:  -96, �A�˳ư_�ӡC
���n�Z���R���I��: �L   ��  �K  �v: �k�N�G-14  ���t�G-11  ���t�G-9
���n�Z�������I��: �L               ���t�G-9   �a�t�G-9   �R���G-9
�]  �k    ��  �O:  0

�A�O�@�� 120 ���� 43 �Ť��ʯ��ڧŮv�C(�W�u�ɶ� 1163 �p��)�A�Q�j���H������ 46�šC
�A�������O�G�Z�S���ǰ|�C      �±�G4, �A�O�ӵL�W�p��C
�O�q: 25/25  ���O: 21/21  ���z: 21/21  �ӱ�: 24/24  ���: 21/21
�ͩR�O: 1515/1515  �]�k�O: 2025/2025  ���ʤO: 1624/1624
�g��ȡG1244880/MAX �m���I��: 424
�ޯ�g��ȡG145499/145500/1400000   
�}��G60, �A�Ԧu���ߥ����C

�A�b�𮧡C
�A�ثe���@�ԫ��A�������I�k.     �ͩR�O�֩� 0 �I�ɦ۰ʰk�]
�{�b���A�G
���~: 60/1000  �t�� 83/1000000 ����  �����G207457814 ��  �s�ڡG106011205 ��
����: 23 ��C Autoexit: �O  Autoloot: �_  Autogold: �O  Autosac: �_
Wizinvis: �_(0)  Holylight: �O  
�D�n�Z���R���I�ơG  44�@�{  ��  �v�G-1531,�@�A�C�@�I! �O���A���v�l���F�I�I�I
�D�n�Z�������I�ơG  44  �@�Ҩ��äO�G -274,�@�A��ۭ����˳ơC
���n�Z���R���I�ơG�@�L�@�k�N�G-441�@���t�G-227�@���t�G-230
���n�Z�������I�ơG�@�L�@���t�G-128�@�a�t�G-131�@�R���G -30
�]  �k    ��  �O�G1015�@�p�t�G -22�@
[��J���O (c)�~��, (r)���L����, (b)�W��, (h)����, (q)���}, �Ϋ� ENTER]:  
���@�z �� �`�@�O�G   5�@�ܡ@�� �z �ˡ@�`�G1969
�]�@�k �� �`�@�O�G   0�@�ܡ@�] �k �ˡ@�`�G1985

*/


	race = ( ( victim->temp_race ) ? victim->temp_race - 1 : victim->race );
	if ( IS_NPC( victim ) )
		return;

	if ( !str_cmp( key, "all" ) || !str_cmp( key, "b" ) )
	{
		sprintf( buf, "\n\r"HIW"�b�b�c�c�d�d�e�e�f�f�g�g�h�h�i "HIC"�� "HIP"�� "HIR"�� "HIY"�� "HIU"�@ "HIG"��"HIW" �i�h�h�g�g�f�f�e�e�d�d�c�c�b�b"NOR"\n\r");
	//����W��
		strcat( buf1, buf );
		sprintf( buf, HIW"%s%s%s"HIY"%s"NOR", "NOR"%s"NOR" %s\n\r",
			IS_SET( victim->act, PLR_KILLER ) ? HIR"<����>"NOR : "",
			IS_SET( victim->act, PLR_THIEF ) ? HIP"<�p��>"NOR : "",
			IS_SET( victim->act, PLR_REGISTER ) ? HIG"<PK>"NOR : "",
			victim->name, IS_NPC( victim ) ? "" : 
			victim->short_descr, victim->pcdata->title);
			strcat( buf1, buf );

	//�H������ , ���쨤��W�U�� 2022/12/22
		if ( get_trust( victim ) != victim->level )
		{
			sprintf( buf, "�j���H������ "HIC"%d"NOR" �šC\n\r", get_trust( victim ) );
			strcat( buf1, buf );
		}else
			strcat(buf1, "\n\r");

		sprintf( buf, "�A�O�@�� "HIC"%d"NOR" ���� "HIY"%d"NOR" ��"CYN"%s"NOR"��"HIW"%s%s"NOR"(%s %s) - (�W�u"HIC" %d "NOR"�p��)\n\r", 
		get_age( victim ), victim->level,
		(victim->sex == SEX_MALE) ? "�k" : (victim->sex == SEX_FEMALE) ? "�k" : "��",
		race_table[ race ].cname,
		class_table[ victim->class ].cname,
		race_table[ race ].name,
		class_table[ victim->class ].name,
		//( victim->level == 41 ? "�^��" : victim->level == 42 ? "�ǩ_" : ""),
		( victim->played / 3600 ));

		strcat( buf1, buf );

	//����
		if ( victim->clan )
		{
			sprintf( buf, "�A�������O�G%s�C", victim->clan->name );
			strcat( buf1, buf );
		}
		else 
		{
			sprintf( buf, "�A�{�b�S���[�J�����C");
			strcat( buf1, buf );
		}
	//�±�
		if( victim->level >= LEVEL_HERO )
		{
			sprintf(buf,"      �±�G"HIW"%d"NOR", ", victim->pcdata->respect);
			strcat( buf1, buf );
		}
		//strcat(buf1, "�A");
		if ( victim->pcdata->respect <  0 ) strcat( buf1, HIC"�A�N�⦺�b����]�S���H�|�o�{�C"NOR"\n\r" );
		else if ( victim->pcdata->respect < 20 ) strcat( buf1,HIW"�A�O�ӵL�W�p��C"NOR"\n\r" );
		else if ( victim->pcdata->respect < 40 ) strcat( buf1,HIG "�A�C�C�}�l���S�Y���C"NOR"\n\r"    );
		else if ( victim->pcdata->respect < 60 ) strcat( buf1, "�A�O���H���C\n\r"    );
		else if ( victim->pcdata->respect < 80 ) strcat( buf1, "�A�¾_�@��C\n\r" );
		else if ( victim->pcdata->respect < 100 ) strcat( buf1, "�A���Ƹ�Q�u�C�֤H�b�s�a�ǻw�C\n\r"    );
		else if ( victim->pcdata->respect < 150 ) strcat( buf1,HIP "�A���n�W�L�H�����A�L�H����C"NOR"\n\r"    );
		else if ( victim->pcdata->respect < 300 ) strcat( buf1,HIR "�A�O�Ӥw�Q���ƪ��ǩ_�H���C"NOR"\n\r" );
		else strcat( buf1, RED"�A�O�ۤѯ��H�����Ĥ@�H�C"NOR"\n\r" );

	//�\��
		sprintf(buf, "�A���\\���G"HIG"%d"NOR"  ", victim->pcdata->merit);
		strcat(buf1, buf);
		if ( victim->pcdata->merit <=  0 ) strcat( buf1, HIC"�A�٨S�إߥ���ԥ\\�C"NOR"\n\r" );
		else if ( victim->pcdata->merit < 20000 ) strcat( buf1,"�A�}�l�إ߾ԥ\\�C"NOR"\n\r"    );
		else if ( victim->pcdata->merit < 40000 ) strcat( buf1, "�A�O���H���C\n\r"    );
		else if ( victim->pcdata->merit < 80000 ) strcat( buf1, "�A���ԪG�׺ӡC\n\r" );
		else if ( victim->pcdata->merit < 100000 ) strcat( buf1, HIY "�A���ԥ\\�Q�u�C�֤H�b�s�a�ǻw�C"NOR"\n\r" );
		else if ( victim->pcdata->merit < 200000 ) strcat( buf1,HIP "�A���ԥ\\�L�H�����A�L�H����C"NOR"\n\r"    );
		else if ( victim->pcdata->merit < 500000 ) strcat( buf1,HIR "�A�����ݦ��p�ԯ��ĤG�C"NOR"\n\r" );
		else strcat( buf1, HIW "�A�O�ۤѯ��H�����Ĥ@�H�C"NOR"\n\r" );

	//�ݩ� , �����髬 2022/12/23
		sprintf( buf,
			"�O�q: %s%d"HIW"/%d%s  ���O: %s%d"HIW"/%d%s  ���z: %s%d"HIW"/%d%s  �ӱ�: %s%d"HIW"/%d%s  ���: %s%d"HIW"/%d%s  �髬: "HIY"%d"NOR"\n\r",
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
			get_curr_size( victim ) // �[�J�髬 2022/12/22
			);
		strcat( buf1, buf );

	//hp mana move
			/*sprintf( buf,
			"�A��[1m%d[m/[1m%d[m�I�ͩR�O�B[1m%d[m/[1m%d[m�I�]�k�O�B[1m%d[m/[1m%d[m�I���ʤO�B[1m%d[m�I�m���I�ơC\n\r",
				  ch->hit,  ch->max_hit,
				  ch->mana, ch->max_mana,
				  ch->move, ch->max_move,
				  ch->practice );*/
		sprintf( buf, "�ͩR�O: %s%d%s/"HIG"%d"NOR"    �]�k�O: %s%d%s/"HIG"%d"NOR"    ���ʤO: %s%d%s/"HIG"%d"NOR"\n\r",
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
		  "�A�a�ۦ�[1m%d[m/[1m%d[m�����~�A�@��[1m%d[m/[1m%d[m����C\n\r",
		  ch->carry_number, can_carry_n( ch ),
		  ch->carry_weight, can_carry_w( ch ) ); */
		if( victim->level >= (LEVEL_IMMORTAL -1 ))
		{
			sprintf( z_buf, "%s", "MAX");
		}
		else
		{
			//20060722 modify by jye
			//�s�WEXP_PER_LEVEL ppl_class�ܼ�
			z_t = EXP_PER_LEVEL( victim->level , victim->class) * (victim->level + 1 );
			sprintf(z_buf, "%d",z_t); 
		}
		sprintf(buf, "�g��ȡG%s%d"NOR"/"HIG"%s"NOR"   �m���I��: "HIG"%d"NOR"",
				(victim->exp >= z_t) ? CYN : HIC,
				victim->exp,
				z_buf,
				victim->practice
		);
		strcat(buf1,buf);

	//pk����
		if( !IS_NPC( victim ) && IS_SET(victim->act, PLR_REGISTER)){
			sprintf(buf, "   PK �����G "HIC" %d ��/"HIR" %d ��"NOR"\n\r",
					victim->pcdata->pkwin,
					victim->pcdata->pklose);
			strcat(buf1, buf);
		}
		else
			strcat(buf1, "\n\r");

	//�ޯ�g��
		if( victim->class >= WIZARD ) {
			sprintf(buf,"�ޯ�g��ȡG"HIC"%d"NOR"/"HIY"%d"NOR"/"HIW"%d"NOR"  (�\\�����ɧA"HIG" %d "NOR"�W���I)\n\r",
					get_eff_skill_exp(victim),
					get_skill_exp(victim),
					get_max_skill_exp(victim),
					get_merit_bonus_max_skexp(victim));
			strcat(buf1, buf);
		}
	//�}��
		if ( !IS_NPC(victim) && victim->level >= 10 )
		{
			sprintf( buf, "�}��G%s%d"NOR"  ", alignment_color(victim->alignment), victim->alignment );
			strcat( buf1, buf );
		}

		//strcat(buf1,"�A");
		// �ק�϶��ƭȻP��X, �W�[�q�����ഫ�e 20 align �ܦ� 2023/01/16
		if ( victim->alignment      >  799 ) strcat( buf1,HIC "�A���p�Ѩϯ뵽�}�C"NOR"                   \n\r" );
		else if ( victim->alignment >  599 ) strcat( buf1,HIW "�A���~�w���p�t�H�뤽���C"NOR"             \n\r" );
		else if ( victim->alignment >  315 ) strcat( buf1,HIG "�A�^���u�k�A��ƥ��q�C"NOR"               \n\r" );
		else if ( victim->alignment >  284 ) strcat( buf1,CYN "�A��H�ٺ⤯�O�C"NOR"                     \n\r" );
		else if ( victim->alignment >  149 ) strcat( buf1,    "�A��H�ٺ⤯�O�C                          \n\r" );
		else if ( victim->alignment > -150 ) strcat( buf1,    "�A�Ԧu���ߥ����C                          \n\r" );
		else if ( victim->alignment > -285 ) strcat( buf1,    "�A���@�I�����C                            \n\r" );
		else if ( victim->alignment > -300 ) strcat( buf1,PUR "�A���@�I�����C"NOR"                       \n\r" );
		else if ( victim->alignment > -316 ) strcat( buf1,PUR "�A�صL�k���A�欰���c�C"NOR"               \n\r" );
		else if ( victim->alignment > -600 ) strcat( buf1,HIP "�A�صL�k���A�欰���c�C"NOR"               \n\r" );
		else if ( victim->alignment > -800 ) strcat( buf1,HIR "�A�O�ӤQ�c���j���c�]�C"NOR"               \n\r" );
		else                                 strcat( buf1,RED "�A�����H�ҶA�G�A²���N�O�����������C"NOR" \n\r" );
		strcat( buf1, "\n\r");
	//�{�b���欰
		switch( victim->position )
		{
			case POS_DEAD:
				strcat( buf1, "�A"RED"��"NOR"�F�I�I");
				break;
			case POS_MORTAL:
				strcat( buf1, "�A"HIR"���F�P�R"NOR"�ˡC");
				break;
			case POS_INCAP:
				strcat( buf1, "�A"HIB"���h��ʯ�O"NOR"�F�C");
				break;
			case POS_STUNNED:
				strcat( buf1, "�A"HIB"���٤H��"NOR"�F�C");
				break;
			case POS_SLEEPING:
				strcat( buf1, "�A"HIU"�ε�"NOR"�F�C");
				break;
			case POS_RESTING:
				strcat( buf1, "�A�b"HIG"��"NOR"�C");
				break;
			case POS_STANDING:
				if( is_flying( victim ) ) // �[�J aff_flying ���P�w 2021/11/05
					strcat( buf1, "�A�}�B�b�o�̡C");
				else
					strcat( buf1, "�A���ߦb�o�̡C");
				break;
			case POS_FIGHTING:
				if( is_flying( victim ) ) // �[�J aff_flying ���P�w 2021/11/05
					strcat( buf1, "�A�}�B�b�o��"HIY"�԰�"NOR"�ۡC");
				else
					strcat( buf1, "�A���b�o��"HIY"�԰�"NOR"�ۡC");
				break;
			default:
				break;
		}
	// �s�W link char ��� 2021/11/20 
		if ( ( vch = victim->linking) )
		{
			sprintf(buf, HIU "�A���F��P"NOR"%s"HIU"���ͦ@��"NOR".\n\r", vch->short_descr );
			strcat(buf1, buf);
		}else{
			strcat( buf1, "\n\r");
		}
	//�@�ԫ��A
		if( !IS_NPC( victim ) /*&& victim->pcdata->autocombat != AUTOC_NORMAL*/)
		{
			strcat(buf1, "�ثe���@�ԫ��A");
			switch( victim->pcdata->autocombat ) 
			{
				case AUTOC_NORMAL:
					strcat(buf1, "  ��  �q.");
					break;
				case AUTOC_STOP:
					strcat(buf1, HIR"�������."NOR);
					break;
				case AUTOC_DODGE:
					strcat(buf1, HIC"���O�{��."NOR);
					break;
				case AUTOC_PARRY:
					strcat(buf1, HIG"���O�u�m."NOR);
					break;
				case AUTOC_CONCENTRATION:
					strcat(buf1, HIY"�M�`��ë."NOR);
					break;
				case AUTOC_CRITICAL:
					strcat(buf1, HIP"�P�R����."NOR);
					break;
				case AUTOC_COOPERATE:
					strcat(buf1, HIW"�s��@��."NOR);
					break;
				case AUTOC_POWER:
					strcat(buf1, HIC"�޵o�]�O."NOR);
					break;
				case AUTOC_INSTANT:
					strcat(buf1, HIY"�����I�k."NOR);
					break;
				case AUTOC_CHARGE:
					strcat(buf1, HIY"�Ĩ����."NOR);
					break;
				case AUTOC_RELOAD:  // �κj��԰��� �۰ʤW�u 2022/05/01
					strcat(buf1, HIB"�ֳt��u."NOR);
					break;
				default:
					strcat(buf1, HIU"  ��  ��."NOR);
				break;
			}
		}


		//sprintf( buf, "�b�ͩR�ȳѤU[1m%d[m�H�U���ɭԷ|�۰ʰk�]�C\n\r", ch->wimpy );
		/*sprintf( buf,
		  "�A��[1m%d[m�I�g��ȡA��[1m%d[m�T�����C\n\r",
		  ch->exp,  ch->gold );
		  */
	//�۰ʰk�]
		sprintf(buf, "            �ͩR�O�֩� "CYN"%d"NOR" �I�ɦ۰ʰk�]\n\r", victim->wimpy);
		strcat( buf1, buf );

	//�{�b���A
		// �p�⪬�A��ܪ��Ӽ�, �Ӧh�h���� 2021/11/05
		int affect_count = 0;;
		strcat( buf1, "�{�b���A�G");
		// �Nfist ���ðβ��̫ܳe�� 2021/11/05
		if ( IS_SET( victim->act, PLR_FIST_FIGHT ) )
		{
			sprintf( buf, "�{��i�� " );
			strcat( buf1, buf );
			affect_count += 2;
		}
		if ( IS_AFFECTED( victim, AFF_HIDE ) )
		{
			sprintf( buf, HIB"�ð�"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
		{
			sprintf( buf, YEL"����"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		// �ק� Cond ���̤j�ƭ� �q 48 �ܬ� 100 , �ܧ�ԭz���ƭȰ϶��μW�[�ԭz 2023/01/13
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] >= 70)
		{
			strcat( buf1, HIR"�s�뤤�r"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] < 70
			&& victim->pcdata->condition[COND_DRUNK] >= 50 /*36*/ )
		{
			strcat( buf1, HIR"��K"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] < 50 /*50*/
			&& victim->pcdata->condition[COND_DRUNK] >= 20 /*10*/)
		{
			strcat( buf1, HIR"�ܾK"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) 
			&& victim->pcdata->condition[COND_DRUNK] < 20/*10*/
			&& victim->pcdata->condition[COND_DRUNK] > 0 ){
			strcat( buf1, RED"�L�H"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_THIRST] < 40 /*15*/
			&& victim->pcdata->condition[COND_THIRST] > 10 /*0*/)
		{
			strcat( buf1, HIY"�f��"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_FULL] < 40 /*15*/
			&& victim->pcdata->condition[COND_FULL] > 10 /*0*/)
		{
			strcat( buf1, HIP"�{�l�j�F"NOR" "  );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_THIRST] <= 10
			&& victim->pcdata->condition[COND_THIRST] >= 0 /* == 0*/ )
		{
			strcat( buf1, YEL"���"NOR" " );
			affect_count += 1;
		}
		if( !IS_NPC( victim ) && victim->level < LEVEL_IMMORTAL 
			&& victim->pcdata->condition[COND_FULL] <= 10
			&& victim->pcdata->condition[COND_FULL] >= 0 /* == 0*/ )
		{
			strcat( buf1, PUR"���j"NOR" "  );
			affect_count += 1;
		}
		// �s�W ���r , �ͯf , ���� , �ƨg , �·����t�����A 2021/11/05
		if ( IS_AFFECTED( victim, AFF_POISON ) )
		{
			sprintf( buf, HIG"���r"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_DISEASE ) )
		{
			sprintf( buf, HIU"�ͯf"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_PARALYZED ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, YEL"�·�"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_FEAR ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, CYN"����"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_CRAZY ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, PUR"�ƨg"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_CURSE ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, HIB"�A�G"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_MUTE ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, HIY"�I�q"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_AFFECTED( victim, AFF_SEAL_MAGIC ) )
		{
			if ( affect_count >= 8){
				strcat( buf1, "\n\r          " );
				affect_count = 0;
			}
			sprintf( buf, HIC"�ʩG"NOR" " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		if ( IS_SET( victim->act, PLR_RIDE ) )
		{
			sprintf( buf, "�y�M " );
			strcat( buf1, buf );
			affect_count += 1;
		}
		strcat( buf1, "\n\r" );
		sprintf( buf, "���~ "CYN"%d"NOR"/"HIC"%d"NOR"  �t�� "CYN"%d"NOR"/"HIC"%d"NOR" ����    ",

		  victim->carry_number, can_carry_n( victim ),
		  victim->carry_weight, can_carry_w( victim ) ); 
		strcat( buf1, buf );

		sprintf( buf, "�����G"HIY"%d"NOR" ��  �s�ڡG"HIY"%d"NOR" ��\n\r",
			victim->gold,
			victim->bank
			);
		strcat( buf1, buf );

	//���� pagelength
		if ( !IS_NPC( victim ) )
			sprintf( buf, "����: "HIW"%d"NOR" ��C  ", victim->pcdata->pagelen );
		else
			sprintf(buf, "�C\n\r");
		strcat( buf1, buf );
		buf[0] = '\0';

	//�۰ʦ欰
		sprintf( buf, "Autoexit: %s  Autoloot: %s  Autogold: %s  Autosac: %s\n\r",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOEXIT ) ) ? HIW"�O"NOR : "�_",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOLOOT ) ) ? HIW"�O"NOR : "�_",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOGOLD ) ) ? HIW"�O"NOR : "�_",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_AUTOSAC  ) ) ? HIW"�O"NOR : "�_"
				);
		strcat( buf1, buf );
	//����
		if( !IS_NPC(victim) && victim->level >= L_APP ) 
		{
			QMARK *qm = is_qmark(victim, "wizinvis");
			sprintf( buf, "Wizinvis: %s(%d)  Holylight: %s  \n\r",
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_WIZINVIS  ) ) ? HIW"�O"NOR : "�_",
				( qm ? qm->v0 : 0 ),
				( !IS_NPC( victim ) && IS_SET( victim->act, PLR_HOLYLIGHT ) ) ? HIW"�O"NOR : "�_" );
			strcat( buf1, buf );
		}
		else
			strcat( buf1, "\n\r");
	}

	// �԰��ƭ� , score f
	if ( !str_cmp( key, "all" ) || !str_cmp( key, "f" ) )
	{
		if ( !str_cmp( key, "f" ) )
		{
			strcat( buf1, "�A���԰��ƭȦp�U�G\n\r" );
		}

		if ( ch->level >= 15 )
		{
			// wp hr
			strcat ( buf1, "�D�n�Z��" );
			sprintf( buf, "�R���I�ơG"HIG"%4d"NOR"�@",get_hitroll( victim, ( get_eq_char(victim, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD)));
			strcat( buf1, buf );

			// dc
			strcat( buf1, "�{  ��  �v�G" );
			if ( ch->level >= LEVEL_HERO )
			{
				sprintf( buf, HIW"%5d"NOR",�@�A", GET_DODGE( victim ) );
				strcat( buf1, buf );
				buf[0] = '\0';
			}
			else
				strcat(buf1,"�A");
			if ( GET_DODGE( victim )      >=  200  ) strcat( buf1, "�ڥ��N�O�@�����ۡC" );
			else if ( GET_DODGE( victim ) >=  100  ) strcat( buf1, "�ٯ�ʯu�O�_�ݡI" );
			else if ( GET_DODGE( victim ) >=   0   ) strcat( buf1, "�w�C�ӵh�W�a����ۡC" );
			else if ( GET_DODGE( victim ) >= -50   ) strcat( buf1, "�񽽤��֤��F�h�֡C" );
			else if ( GET_DODGE( victim ) >= -150  ) strcat( buf1, "�ש�Ƿ|�����C" );
			else if ( GET_DODGE( victim ) >= -200  ) strcat( buf1, "���Y���ݡA���t�a���C" );
			else if ( GET_DODGE( victim ) >= -400  ) strcat( buf1, "���t�פ����C" );
			else if ( GET_DODGE( victim ) >= -500  ) strcat( buf1, "��ʫD�`�ӱ��C" );
			else if ( GET_DODGE( victim ) >= -750  ) strcat( buf1, "�ʦp��ߡA�@���d���C" );
			else if ( GET_DODGE( victim ) >= -900  ) strcat( buf1, "�@�w�O�@���ɶ]�a�x�C" );
			else if ( GET_DODGE( victim ) >= -1000 ) strcat( buf1, "�t�קֱo�S�H�ݱo��C");
			else if ( GET_DODGE( victim ) >= -1200 ) strcat( buf1, "�ڥ��N���@�}�ۭ��C");
			else if ( GET_DODGE( victim ) >= -1300 ) strcat( buf1, "�֨�i�H�l�W�ۤv���v�l�I");
			else                                     strcat( buf1, "�C�@�I! �O���A���v�l���F�I�I�I");
			strcat( buf1, NOR"\n\r" );

			// wp dr
			sprintf( buf, "�D�n�Z�������I�ơG"HIG"%4d"NOR"  ",get_damroll( victim, ( get_eq_char(victim, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD)));
			strcat( buf1, buf );

			// ac
			strcat( buf1, "�@�Ҩ��äO�G" );
			if ( ch->level > 25 )
			{
				sprintf( buf, HIW"%5d"NOR",�@�A", GET_AC( victim ) );
				strcat( buf1, buf );  
				buf[0] = '\0';
			}
			else
				strcat(buf1,"�A");
			if ( GET_AC( victim )      >=  200  ) strcat( buf1, "��r�骺�ٸرi�I" );
			else if ( GET_AC( victim ) >=  100  ) strcat( buf1, "������S�����ˡC" );
			else if ( GET_AC( victim ) >=   0   ) strcat( buf1, "�u��ۤ��示�ǡC" );
			else if ( GET_AC( victim ) >= -50   ) strcat( buf1, "��۴��q����A�C" );
			else if ( GET_AC( victim ) >= -200  ) strcat( buf1, "�h�֦��I�˳ơC" );
			else if ( GET_AC( victim ) >= -400  ) strcat( buf1, "�˳ư_�ӡC" );
			else if ( GET_AC( victim ) >= -600  ) strcat( buf1, "���˳Ƥ����C" );
			else if ( GET_AC( victim ) >= -800  ) strcat( buf1, "���˳ƫܦn�C" );
			else if ( GET_AC( victim ) >= -1000 ) strcat( buf1, "��ۭ����˳ơC" );
			else if ( GET_AC( victim ) >= -1100 ) strcat( buf1, "��۶W�Ū��˳ơC" );
			else if ( GET_AC( victim ) >= -1200 ) strcat( buf1, "���˳ƥu���ѤW���C");
			else if ( GET_AC( victim ) >= -1400 ) strcat( buf1, "�o�˪��˳ƤѤU�L�ġC");
			else if ( GET_AC( victim ) >= -1600 ) strcat( buf1, "��۶W�ŵL�Ķ����R�E���˳ơC");
			else                                  strcat( buf1, "���˳ƤӸرi�F�A�O�� bug �ܡH");
			strcat( buf1, NOR"\n\r" );

			if ( get_eq_char( victim, WEAR_WIELD_2 ) )
			{
			// wp2 hr
				sprintf( buf, "���n�Z���R���I�ơG"HIG"%4d"NOR"�@",  get_hitroll( victim, WEAR_WIELD_2 ));
				strcat( buf1, buf );

			// svs , wind , fire , poison
				if ( ch->level > 36)
				{
					sprintf( buf, "�k�N�G"HIW"%4d"NOR"�@���t�G"HIW"%4d"NOR"�@���t�G"HIW"%4d"NOR"�@�r���G"HIW"%4d"NOR"\n\r",
					NEW_GET_SVS(victim, MAGIC_SPELL),
					NEW_GET_SVS(victim, MAGIC_WIND),
					NEW_GET_SVS(victim, MAGIC_FIRE),
					NEW_GET_SVS(victim,MAGIC_POISON)); // �W�[���r�Ctick����ˮ`��� 2021/11/03
					strcat( buf1, buf );
				}
				else
					strcat( buf1, "\n\r");

			// wp2 dr
				sprintf( buf, "���n�Z�������I�ơG"HIG"%4d"NOR"�@",get_damroll( victim, WEAR_WIELD_2 ) );
				strcat( buf1, buf );
				
			// water , earth, breath
				if (ch->level > 36)
				{
					sprintf( buf, NOR"���t�G"HIW"%4d"NOR"�@�a�t�G"HIW"%4d"NOR"�@�R���G"HIW"%4d"NOR"\n\r",
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
				sprintf( buf, "���n�Z���R���I�ơG"HIG"�@�L"NOR"�@");
				strcat( buf1, buf );

				if ( ch->level > 36)
				{
					sprintf( buf, "�k�N�G"HIW"%4d"NOR"�@���t�G"HIW"%4d"NOR"�@���t�G"HIW"%4d"NOR"�@�r���G"HIW"%4d"NOR"\n\r",
					NEW_GET_SVS(victim, MAGIC_SPELL),
					NEW_GET_SVS(victim, MAGIC_WIND),
					NEW_GET_SVS(victim, MAGIC_FIRE),
					NEW_GET_SVS(victim,MAGIC_POISON)); // �W�[���r�Ctick����ˮ`��� 2021/11/03
					strcat( buf1, buf );
				}
				else
					strcat( buf1, "\n\r");

				sprintf( buf, "���n�Z�������I�ơG"HIG"�@�L"NOR"�@");
				strcat( buf1, buf );

				if (ch->level > 36)
				{
					sprintf( buf, NOR"���t�G"HIW"%4d"NOR"�@�a�t�G"HIW"%4d"NOR"�@�R���G"HIW"%4d"NOR"\n\r",
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
			sprintf( buf, "�]  �k    ��  �O�G"HIP"%4d"NOR"�@",  victim->pp );
			strcat( buf1, buf );

			sprintf( buf, NOR"�p�t�G"HIW"%4d"NOR"�@���t�G"HIW"%4d"NOR"�@��t�G"HIW"%4d"NOR"\n\r",
				NEW_GET_SVS(victim, MAGIC_LIGHTNING), NEW_GET_SVS(victim, MAGIC_SAINT), NEW_GET_SVS(victim, MAGIC_DARK));
			strcat( buf1, buf );

			sprintf( buf, "���@�z �� �`�@�O�G"HIP"%4d"NOR"�@",   victim->apdr );
			strcat( buf1, buf );

			sprintf( buf, "�ܡ@�� �z �ˡ@�`�G"HIP"%4d"NOR"\n\r", victim->pdr );
			strcat( buf1, buf );

			sprintf( buf, "�]�@�k �� �`�@�O�G"HIP"%4d"NOR"�@",   victim->amdr );
			strcat( buf1, buf );

			sprintf( buf, "�ܡ@�] �k �ˡ@�`�G"HIP"%4d"NOR"\n\r", victim->mdr );
			strcat( buf1, buf );
		}
		// �N�԰��O����� score f ���U, �å[�J������� 2021/11/21
		//if ( !str_cmp( key, "all" ) || !str_cmp( key, "a" ) )
		if ( !str_cmp( key, "f" ) )
		{
			strcat( buf1, "�A���԰��O���p�U�G\n\r" );
			if ( ( vch = victim->tracking ) )
			{
				sprintf( buf, YEL "�A���b�l�� "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->assassinating ) )
			{
				sprintf( buf, RED "�A���b�t�� "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->hunting ) )
			{
				sprintf( buf, HIR "�A���b�y�� "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->cooperating) )
			{
				sprintf( buf, HIW "�A���b��U "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
				strcat( buf1,"\n\r");
			}
			if ( ( vch = victim->protecting) )
			{
				sprintf( buf, HIY "�A���b�O�@ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
				strcat( buf1,"\n\r");
			}
			if ( ( victim->hunted ) ){
				int hunt_num = 0;
				strcat( buf1, ""HIR"���b�y���A�G"NOR"\n\r" );
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
				strcat( buf1, ""HIW"���b��U�A�G"NOR"\n\r" );
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
				strcat( buf1, ""HIY"���b�O�@�A�G"NOR"\n\r" );
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
				sprintf( buf, YEL "�A���b�l�� "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
			}
			if ( ( vch = victim->assassinating ) )
			{
				sprintf( buf, RED "�A���b�t�� "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
			}
			if ( ( vch = victim->hunting ) )
			{
				sprintf( buf, HIR "�A���b�y�� "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat( buf1, buf );
			}
			// �s�W cooperator ��� 2021/11/20 
			if ( ( vch = victim->cooperating) )
			{
				sprintf( buf, HIW "�A���b��U "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
			}
			if ( ( vch = victim->protecting) )
			{
				sprintf( buf, HIY "�A���b�O�@ "NOR"%s"NOR".\n\r", vch->short_descr );
				strcat(buf1, buf);
			}
			// �s�W hunted �p�Ʀb config +COMBINE ���Y����� 2021/11/21
			if ( ( victim->hunted ) ){
				int hunt_num = 0;
				for( vch = victim->hunted; vch; vch = vch->next_in_hunt ){
					hunt_num++;
				}
				for( vch = victim->hunted; vch; vch = vch->next_in_hunt ){
					if( hunt_num > 3 && IS_SET( ch->act, PLR_COMBINE ) ){
						sprintf( buf, "("HIR"%d"NOR")%s "HIR"���b�y���A"NOR".\n\r", hunt_num, vch->short_descr);
						strcat( buf1, buf );
						break;
					}else{
						sprintf( buf, "%s "HIR"���b�y���A"NOR".\n\r", vch->short_descr );
						strcat( buf1, buf );
					}
				}
			}
			// �s�W cooperator ��� 2021/11/20 
			if( ( vch = victim->cooperator ) ) {
				int coo_num = 0;
				for( vch = victim->cooperator; vch; vch = vch->next_in_coop ) {
					coo_num++;
				}
				for( vch = victim->cooperator; vch; vch = vch->next_in_coop ) {
					if( coo_num > 3 && IS_SET( ch->act, PLR_COMBINE ) ){
						sprintf( buf, "("HIW"%d"NOR")%s "HIW"���b��U�A"NOR".\n\r", coo_num, vch->short_descr);
						strcat( buf1, buf );
						break;
					}else{
						sprintf( buf, "%s "HIW"���b��U�A"NOR".\n\r", vch->short_descr );
						strcat( buf1, buf );
					}
				}
			}
			// �s�W protected �p�Ʀb config +COMBINE ���Y����� 2021/11/21
			if( ( vch = victim->protector ) ) {
				int pro_num = 0;
				for( vch = victim->protector; vch; vch = vch->next_in_protect ) {
					pro_num++;
				}
				for( vch = victim->protector; vch; vch = vch->next_in_protect ) {
					if( pro_num > 3 && IS_SET( ch->act, PLR_COMBINE ) ){
						sprintf( buf, "("HIY"%d"NOR")%s "HIY"���b�O�@�A"NOR".\n\r", pro_num, vch->short_descr);
						strcat( buf1, buf );
						break;
					}else{
						sprintf( buf, "%s "HIY"���b�O�@�A"NOR".\n\r", vch->short_descr );
						strcat( buf1, buf );
					}
				}
			}
		}
	}
	
	// affected_by �S��v�T,  score a
	if ( !str_cmp( key, "all" ) || !str_cmp( key, "a" ) )
	{
		if ( !str_cmp( key, "a" ) )
		{
		strcat( buf1, "�A������H�U�S��v�T�G\n\r" );
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
					strcat( buf1, "�A������H�U�S��v�T�G\n\r" );
					printed = TRUE;
				}

				sprintf( buf, "�S�ġG�u"HIC"%s"NOR"(%s)�v", skill_table[paf->type].cname, skill_table[paf->type].name );
				strcat( buf1, buf );

				if ( ch->level >= 20 )
				{
					strcat( buf1, "(");
					if( strcmp(c_affect_loc_name(paf->location), "�L"))
					{
						sprintf( buf, "�v�T"HIY"%s"NOR, c_affect_loc_name( paf->location ) );
						strcat( buf1, buf );
					}
					if( paf->modifier )
					{
						sprintf( buf, " %s%d "NOR"�I  ",
								paf->modifier<0? HIU"-":( paf->modifier == 0 ? HIW:HIC"+"),
								paf->modifier<0?-paf->modifier:paf->modifier
								);
						strcat( buf1, buf );
					}
					if( paf->duration >= 0 )
					{
						sprintf( buf, "�|�� "HIG"%d"NOR" �p�ɪ��ĥΡC", paf->duration );
						strcat( buf1, buf );
					}
					else
					{
						sprintf(buf, "����@�Τ��C");
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
		strcat(buf1, NOR"��������������������������������������������������������������������������������"NOR"\n\r");
	}

	send_to_char( buf1, ch );
	return;
}



char *        const    day_name    [] =
{
    "��", "��", "�a", "�p", "�ۥ�",
    "�t��", "�P"
};

char *        const    month_name      [] =
{
    "�V", "�V�T", "�������H", "���j",
    "�j��", "�K", "�۵M", "��o", "�s",
    "�Ӷ�", "����", "�ųN", "�t�v", "�խ�",
    "���]", "�·t", "�j�c�]"
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
		"[36m�{�b�O%s%d�ɡA%s����A%d%s�A%s(%d)����C[m\n\r",
		//time_info.hour >= 12 ? "�U��" : "�W��",
		"",
		//( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
		time_info.hour % 24,
		day_name[day % 7], 
		day, suf,
		month_name[time_info.month], time_info.month );
	else
		sprintf( buf,
		"[36m�{�b�O%s%d�ɡA%s����A%d%s�A%s����C[m\n\r",
		//time_info.hour >= 12 ? "�U��" : "�W��",
		"",
		//( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
		time_info.hour % 24,
		day_name[day % 7],
		day, suf,
		month_name[time_info.month] );
	send_to_char( buf, ch );
	sprintf( buf, "[36m�V�P�ۥ@�Ұʩ� %s\n\r�t�ήɶ��O     %s[m\n\r", str_boot_time, (char *) ctime( &current_time ) );
	send_to_char( buf, ch );

	if ( down_time > 0 )
	{
		sprintf( buf, "[36m�U��%s�N�� %s[m\r", Reboot ? "Reboot" : "Shutdown", (char *) ctime( &down_time ) );
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
		"�U���L��",
		"�h��",
		"���ۤp�B",
		"���۰{�q�U�ۼɫB",
		"�Ƶۤp��",
		"��_�}�}�ɭ���"
	};

	if ( !IS_OUTSIDE( ch ) )
	{
		send_to_char( "[33m�b�o�̧A�[��X�ѪŪ��ܤ�.[m\n\r", ch );
		return;
	}

	//monsoon change add at 2018.11.06
	sprintf( buf, "�Ѫ�%s, �ӥB%s.\n\r",
		sky_look[weather_info.sky],
		time_info.month <= 4 
			? "���_����檺�H���j�L" 
			: ((time_info.month >= 5 && time_info.month <= 8)
			? "���F��ŷx���M���j�L" 
			: ((time_info.month >= 9 && time_info.month <= 12)
			? "���n����㪺�����j�L" 
			: "�����M�n���D���j�L")) );
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
		send_to_char( "�|�P�@�����µL�k�o���P������....\n\r",ch);
		return;
	}


	static char * const SectorType[ 11 ] =
	{
		"�P�����ҨS����S�O��",
		"�P�����Ҧ��۳\\�h�ؿv��������",
		"�P�����ҬO�@�����Z���쳥",
		"�P�����ҬO���۳\\�h��쪺�˪L",
		"�P�����ҬO�a�����C�������C��",
		"�P�����ҬO�Ů�}�������s",
		"�A��b���R�������W",
		"�A���B�b���檺���y",
		"�A���B�b����",
		"�A���B�b�b�Ť�",
		"�P�����ҬO���򪺯�z"
	};
	sprintf( buf, "%s", SectorType[RoomSector]);
	
	if( IS_SET(ch->in_room->room_flags, ROOM_DARK)
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
		|| IS_SET(ch->in_room->room_flags, ROOM_PK)
		|| IS_SET(ch->in_room->room_flags, ROOM_UNDERGROUND))
	{
		sprintf( buf, "%s.\r\n\r\n�o��", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_PK))
			sprintf( buf, "%s�O���޳�", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
			sprintf( buf, "%s����A�G", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_UNDERGROUND))
			sprintf( buf, "%s�����Q�B��", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_DARK))
			sprintf( buf, "%s���`�·t", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_CAMP)
			|| IS_SET(ch->in_room->room_flags, ROOM_SAFE))
		{
			sprintf( buf, "%s, ", buf );
			if( IS_SET(ch->in_room->room_flags, ROOM_CAMP))
				sprintf( buf, "%s�a���Ǩ��]�O", buf );
			if( IS_SET(ch->in_room->room_flags, ROOM_SAFE))
				sprintf( buf, "%s�۷�w��", buf );
		}
	}else if( IS_SET(ch->in_room->room_flags, ROOM_CAMP)
			|| IS_SET(ch->in_room->room_flags, ROOM_SAFE))
	{
		sprintf( buf, "%s.\r\n\r\n�o��", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_CAMP))
			sprintf( buf, "%s���a���Ǩ��]�O", buf );
		if( IS_SET(ch->in_room->room_flags, ROOM_SAFE))
			sprintf( buf, "%s�۷�w��", buf );
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
		send_to_char( "[33m�S������o�Ӧr��������I[m\n\r", ch );
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
           //        send_to_char( "[33m�u�ॴ��ӵ��šC[m\n\r", ch );
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
                   send_to_char(YEL "�S���o�ӰѼơC"NOR"\n\r", ch);
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
                   send_to_char(YEL "�S���o�Ӫ��a�C"NOR"\n\r", ch);
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
                    send_to_char( "�o�H�O���D�ܡH\n\r", ch );
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
                    send_to_char( "[33m¾�~�κرڦW�٭n���o����ǡC[m\n\r", ch );
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
                  send_to_char( "[33m�����O�@��¾�~�κرڡI�C[m\n\r", ch );
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
  sprintf( buf , "[ "HIC"�V�P�ۥ@"NOR" ]%s%s%s\n\r""========================================================================"NOR"\n\r",
            ( fPK || fImmortalOnly || fListGroup || who_clan ) ? " - " : "",
             fListGroup ? ( (leader->group) ? leader->group->name: leader->short_descr ): 
            (fPK ? "�ѥ[ PK ���a�C��" : (fImmortalOnly ? "���ڦC��" :(who_clan ? "�����C��" : ""))),
             fListGroup ? (leader->group ? "" : "������"): "");
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
              case L_ACG: class[i] = (fChinese) ? HIW"�U  ��"NOR:HIW"ANC"NOR; break;
              case L_DIR: class[i] = (fChinese) ? HIC"�Х@��"NOR:HIC"ADM"NOR; break;
              case L_SEN: class[i] = (fChinese) ? HIY"�j  ��"NOR:HIY"GOD"NOR; break;
              case L_JUN: class[i] = (fChinese) ? HIU"  ��  "NOR:HIU"GOD"NOR; break;
              case L_APP: class[i] = (fChinese) ? HIG"��  ��"NOR:HIG"ANG"NOR; break;
              case 42: class[i] = (fChinese) ? HIR"��  �_"NOR:HIR"LGD"NOR; break;
       
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
                   sprintf( buf + strlen( buf ), "�i"HIW" %2d"NOR" "HIW"%6s"NOR" �j %s%s%s%s%s("HIY"%s"NOR") %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"�L����"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"����"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF    ) ? "("RED"�p��"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"     : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK    ) )  ? "("HIP"�o�b"NOR")"   : "",
                            //wch[i]->short_descr,
                            wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? wch[i]->temp_race - 1 : wch[i]->race )].cname);
                            //race_table[ wch[i]->race ].cname );
               }
               else if( wch[i]->level == ( LEVEL_IMMORTAL - 2 ) )
               {
                   sprintf( buf + strlen( buf ), "�i"HIW" %2d"NOR" "HIP"%6s"NOR" �j %s%s%s%s%s("HIY"%s"NOR") %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"�L����"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"����"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF    ) ? "("RED"�p��"NOR")"  : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"     : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK    ) )  ? "("HIP"�o�b"NOR")"   : "",
                            //wch[i]->short_descr,
                            wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].cname);
                            //race_table[ wch[i]->race ].cname );
               }
               else
               {
                   sprintf( buf + strlen( buf ), "�i"HIW" %2d"NOR" %6s"NOR" �j %s%s%s%s%s%s("HIY"%s"NOR") %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"�L����"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_WIZINVIS ) ? "("HIY"����"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_KILLER  ) ) ? "("RED"����"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_THIEF     )) ? "("RED"�p��"NOR")"  : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER) ) ? "("HIG"PK"NOR")"     : "",
                            (  IS_SET( wch[i]->act, PLR_AFK    )) ? "("HIP"�o�b"NOR")"    : "",
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
                   sprintf( buf + strlen( buf ), "�i"HIW" %2d"NOR" "HIW"%3s"NOR" �j %s%s%s%s"HIY"%s"NOR" %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"�L����"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"����"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF     ) ? "("RED"�p��"NOR")"  : "",
                            (fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"      : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK     )) ? "("HIP"�o�b"NOR")"    : "",
                            //wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].name);
                            //race_table[ wch[i]->race ].name );
               }
               else if( wch[i]->level == ( LEVEL_IMMORTAL - 2 ) )
               {
                   sprintf( buf + strlen( buf ), "�i"HIW" %2d"NOR" "HIP"%3s"NOR" �j %s%s%s%s"HIY"%s"NOR" %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"�L����"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_KILLER   ) ? "("RED"����"NOR")" : "",
                            IS_SET( wch[i]->act, PLR_THIEF     ) ? "("RED"�p��"NOR")"  : "",
                            (fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"      : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_AFK     )) ? "("HIP"�o�b"NOR")"    : "",
                            //wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? wch[i]->temp_race - 1 : wch[i]->race )].name);
                            //race_table[ wch[i]->race ].name );
               }
               else
               {
                   sprintf( buf + strlen( buf ), "�i"HIW" %2d"NOR" %s"NOR" �j %s%s%s%s%s"HIY"%s"NOR" %s ("HIG"%s"NOR")\n\r",
                            ( who_clan ? wch[i]->pcdata->clanlevel:wch[i]->level ),
                            ( who_clan ? (wch[i]->clan ? wch[i]->clan->whoname: HIW"�L����"NOR):class[i] ),
                            IS_SET( wch[i]->act, PLR_WIZINVIS ) ? "("HIY"����"NOR")" : "", 
                            ( fVerbose && IS_SET( wch[i]->act, PLR_KILLER   )) ? "("RED"����"NOR")" : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_THIEF     )) ? "("RED"�p��"NOR")"  : "",
                            ( fVerbose && IS_SET( wch[i]->act, PLR_REGISTER )) ? "("HIG"PK"NOR")"     : "",
                            IS_SET( wch[i]->act, PLR_AFK     ) ? "("HIP"�o�b"NOR")"    : "",
                            //wch[i]->cname,
                            wch[i]->name,
                            wch[i]->pcdata->title,
                            race_table[( ( wch[i]->temp_race ) ? (wch[i]->temp_race - 1) : (wch[i]->race) )].name);
                            //race_table[ wch[i]->race ].name );
               }
           }
       } //j end
       
		// �ק�̤j pagelength , �G�i��ܤW������ 2023/01/12
		//if( count >= 50 ) {
		if( count >= 80 ) {
			send_to_char(buf, ch);
			buf[0] = '\0';
			count = 0;
		}
  } //sort lv end    
  
  sprintf( buf2, "%s�� "HIW"%d / %d"NOR" �W�i�̥��b�гy�s���ǻ��C\n\r", 
           "========================================================================"NOR"\n\r",
           nMatch, current_ppl );
  strcat( buf, buf2 );

  if( current_ppl > MAX_PPL )
      MAX_PPL = current_ppl;
  sprintf( buf2, "�̦h���� "HIW"%d"NOR" �W�i�̦P�ɦb�o�ӥ@�ɹC���C\n\r", MAX_PPL );
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
  send_to_char( "[33m�ϥΤ�k�Gwhois <name>[m\n\r", ch );
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
        case L_ACG: class = HIW"�U��"NOR; break;
        case L_DIR: class = HIW"�ѯ�"NOR; break;
        case L_SEN: class = HIY"�j��"NOR; break;
        case L_JUN: class = HIG" �� "NOR; break;
        case L_APP: class = HIC"�Ѩ�"NOR; break;
/*
        case L_DIR: class = "[1;36m�j�j��[m"; break;
        case L_SEN: class = "[1;36m �j�� [m"; break;
        case L_JUN: class = "[1;36m  ��  [m"; break;
        case L_APP: class = "[1;36m���߯�[m"; break;
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
        IS_SET( wch->act, PLR_KILLER   ) ? "("RED"����"NOR")" : "",
        IS_SET( wch->act, PLR_THIEF    ) ? "("RED"�p��"NOR")"  : "",
        IS_SET( wch->act, PLR_REGISTER ) ? "("HIG"PK"NOR")"     : "",
        IS_SET( wch->act, PLR_AFK    ) ? "("HIP"�o�b"NOR")"    : "",
        wch->short_descr,
        wch->pcdata->title , "");
  else
      sprintf( buf + strlen( buf ), "(        %s        )%s%s%s%s%s%s%s %s\n\r",
        class,
        IS_SET( wch->act, PLR_WIZINVIS ) ? "("HIY"����"NOR")" : "",
        IS_SET( wch->act, PLR_KILLER   ) ? "("RED"����"NOR")" : "",
        IS_SET( wch->act, PLR_THIEF    ) ? "("RED"�p��"NOR")"  : "",
        IS_SET( wch->act, PLR_REGISTER ) ? "("HIG"PK"NOR")"     : "",
        IS_SET( wch->act, PLR_AFK    ) ? "("HIP"�o�b"NOR")"    : "",
        wch->short_descr,
        wch->pcdata->title , "");
      }

    if ( buf[0] == '\0' )
  send_to_char( "[33m�S���H�X�A������C[m\n\r", ch );
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
      send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
      return;
  }
  send_to_char( "�A���W�a�ۦ��G\n\r\n\r", ch );
  show_list_to_char( victim->carrying, ch, TRUE, TRUE );
  return;
    }
    send_to_char( "�A���W�a�ۦ��G\n\r\n\r", ch );
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
          send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
          return;
      }
    } else {
      victim = ch;
    }

    send_to_char( "�A���W�˳ƵۡG\n\r", ch );
    found = FALSE;
    if ( !IS_SET( race_table[ victim->race ].race_abilities, RACE_BEHOLDER ) ){
        for ( iWear = 0; iWear < MAX_WEAR; iWear++ ){
          if ( obj = get_eq_char( victim, iWear ) ) {
            send_to_char( where_name[iWear], ch );
            if ( can_see_obj( ch, obj ) ){
              send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
              send_to_char( "\n\r", ch );
            }else{
              send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
              }
              found = TRUE;
            } 
          }
        }
		// �u�ĸ˳Ʀ�m add at 2022/04/30
		if ( obj = get_eq_char( victim, WEAR_AMMO ) ) {
			send_to_char( where_name[WEAR_AMMO], ch );
			if ( can_see_obj( ch, obj ) ){
				send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
				send_to_char( "\n\r", ch );
			}else{
				send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
            }
            found = TRUE;
        }
        if ( ( obj = get_eq_char( victim, WEAR_HANDS_2 ) ) ){
            send_to_char( where_name[WEAR_HANDS_2],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "�������~�C\n\r", ch );
            }
            found = TRUE;
        }
        if ( ( obj = get_eq_char( victim, WEAR_ARMS ) ) ){
            send_to_char( where_name[WEAR_ARMS],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "�������~�C\n\r", ch );
            }
            found = TRUE;
        }
        if ( ( obj = get_eq_char( victim, WEAR_ARMS_2 ) ) ){
            send_to_char( where_name[WEAR_ARMS_2],ch );
            if ( can_see_obj( ch, obj ) ){
                send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
                send_to_char( "\n\r", ch ); 
            }else{
                send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
            }
            found = TRUE;
        }
		// �u�ĸ˳Ʀ�m add at 2022/04/30
		if ( obj = get_eq_char( victim, WEAR_AMMO ) ) {
			send_to_char( where_name[WEAR_AMMO], ch );
			if ( can_see_obj( ch, obj ) ){
				send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
				send_to_char( "\n\r", ch );
			}else{
				send_to_char( "�������~�C\n\r", ch );
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
                send_to_char( "�������~�C\n\r", ch );
            }
            found = TRUE;
        }
    }
    if ( !found )
        send_to_char( "����]�S���C\n\r", ch );

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
  send_to_char( "[33m�n�������򤰻�H[m\n\r", ch );
  return;
    }

    if ( !( obj1 = get_obj_carry( ch, arg1 ) ) )
    {
  send_to_char( "[33m�A�S�����˪F��C[m\n\r", ch );
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
      send_to_char( "[33m�A�S����ۥi�H���Ӱ���������F��C[m\n\r", ch );
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
    send_to_char( "[33m�A�S�����˪F��C[m\n\r", ch );
    return;
      }
  
      if ( ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE ) == 0 )
      {
    send_to_char( "[33m�o���O�i�H���Ӥ�����F��C[m\n\r", ch );
    return;
      }

  }
    }
      
    msg          = NULL;
    value1    = 0;
    value2    = 0;

    if ( obj1 == obj2 )
    {
  msg = "[33m�A��$p�M���ۤv���Ӥ���C���̬ݰ_�Ӯt���h�O�@�˪��C[m";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
  msg = "[33m�A���ள$P�Ӹ�$p����C[m";
    }
    else
    {
  switch ( obj1->item_type )
  {
  default:
      msg = "[33m$p�M$P���ள�Ӥ���C[m";
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
         msg = "[36m$p�M$P�]�˳Ʀb���W���^�ݰ_�Ӯt���h�O�@�˪��C[m";
     else if ( value1  > value2 )
         msg = "[36m$p�ݰ_�Ӥ�$P�]�˳Ʀb���W���^�n�ӱo�n�C[m";
      else
         msg = "[36m$p�ݰ_�Ӥ�$P�]�˳Ʀb���W���^�٨ӱo��C[m";
  }
  else
  {
       if ( value1 == value2 ) msg = "[36m$p�M$P�ݰ_�Ӯt���h�O�@�˪��C[m";
  else if ( value1  > value2 ) msg = "[36m$p�ݰ_�Ӥ�$P�n�ӱo�n�C[m";
  else           msg = "[36m$p�ݰ_�Ӥ�$P�٨ӱo��C[m";
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
  send_to_char( "[36m�b�A���񪺦��G[m\n\r", ch );
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
      send_to_char( "[33m�L[m\n\r", ch );
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
      act( "[33m�A�䤣�줰��$T�C[m", ch, NULL, arg, TO_CHAR );
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
		send_to_char( "[33m�n�Ҷq���֡H[m\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "[33m�L���b�o��C[m\n\r", ch );
		return;
	}

	if ( !IS_NPC( victim ) && !IS_SET( victim->act, PLR_KILLER ) )
	{
		send_to_char( "[33m�Q����H�ǡH�Ҽ{�o��h�F���A�F�U�h(murder)�N��F�I[m\n\r", ch );
		return;
	}

	diff = victim->level - ch->level;

	if (      diff <= -20 ) msg = HIG"�A���慨��]�i�H�Ťⰵ��$N�C"NOR;
	else if ( diff <= -10 ) msg = HIG"$N���ȱo�A�h���L�C"NOR;
	else if ( diff <=  -5 ) msg = HIG"$N�ݰ_�ӫܦn�ѨM�C"NOR;
	else if ( diff <=   5 ) msg = HIY"�ѳ{�Ĥ�I"NOR;
	else if ( diff <=  10 ) msg = HIY"�A�ݭn�@�I�B��~�ॴ��$N"NOR;
	else if ( diff <=  20 ) msg = HIR"$N�@���d���a��A�j���C"NOR;
	else                    msg = HIR"�����|�P�§A��§���C"NOR;

	act( msg, ch, NULL, victim, TO_CHAR );

	/* additions by amenda at 4/20/02 */
	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r�L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r�o" );
	else
		sprintf( buf1, "\n\r��" );

	if (      get_curr_size( victim ) >= 9 ) strcat( buf1, "���髬���s�t���h�j�C" );
	else if ( get_curr_size( victim ) >= 8 ) strcat( buf1, "���髬���@�Y���s�C" );
	else if ( get_curr_size( victim ) >= 7 ) strcat( buf1, "���髬��P���H�B���C" );
	else if ( get_curr_size( victim ) >= 6 ) strcat( buf1, "������P���]�t���h�j�C�A" );
	else if ( get_curr_size( victim ) >= 5 ) strcat( buf1, "�򦫴����J���J���t���h�j�C" );
	else if ( get_curr_size( victim ) >= 4 ) strcat( buf1, "��@�릨�~�H���j�@�ǡC" );
	else if ( get_curr_size( victim ) >= 3 ) strcat( buf1, "�S�S�O���j�A�P�H���t���h�C" );
	else if ( get_curr_size( victim ) >= 2 ) strcat( buf1, "�G�p������A���ӥb���H�C" );
	else if ( get_curr_size( victim ) >= 1 ) strcat( buf1, "�p�P�p����@�ˡA�b�p���n�C" );
	else                                     strcat( buf1, "���G���s�b�b�I��q�A��Ů�t���h�I" );
	send_to_char( buf1, ch );

	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r�L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r�o" );
	else
		sprintf( buf1, "\n\r��" );

	if (      GET_DODGE( victim ) >=   200 ) strcat( buf1, "��ʰ_�Ӯڥ����@�����ۡC" );
	else if ( GET_DODGE( victim ) >=     0 ) strcat( buf1, "�ʧ@���w�C�ӵh�W�a����C" );
	else if ( GET_DODGE( victim ) >=   -50 ) strcat( buf1, "�ʧ@�񽽤��֤��F�h�֡C" );
	else if ( GET_DODGE( victim ) >=  -150 ) strcat( buf1, "�ʧ@�ש�Ƿ|�����C" );
	else if ( GET_DODGE( victim ) >=  -300 ) strcat( buf1, "��ʳt�פ����C" );
	else if ( GET_DODGE( victim ) >=  -500 ) strcat( buf1, "��ʫD�`�ӱ��C" );
	else if ( GET_DODGE( victim ) >=  -700 ) strcat( buf1, "�ʦp��ߡA�@���d���C" );
	else if ( GET_DODGE( victim ) >=  -950 ) strcat( buf1, "�ӱ��o�N���@���G���C" );
	else if ( GET_DODGE( victim ) >= -1000 ) strcat( buf1, "�t�קֱo�A�ݤ���C" );
	else if ( GET_DODGE( victim ) >= -1200 ) strcat( buf1, "�t�׮ڥ��N���@�}�ۭ��C" );
	else if ( GET_DODGE( victim ) >= -1400 ) strcat( buf1, "�t�ק֨�i�H�l�W�ۤv���v�l�C" );
	else                                     strcat( buf1, ""HIR"�t�ק֨�w�g�l�W�ۤv���v�l�F�I�I"NOR"");
	send_to_char( buf1, ch );


	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r�L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r�o" );
	else
		sprintf( buf1, "\n\r��" );

	if (      GET_AC( victim ) >=    300 ) strcat( buf1, "���W����ۤ�r�骺�ٸرi�I"  );
	else if ( GET_AC( victim ) >=    100 ) strcat( buf1, "������S�����ˡC" );
	else if ( GET_AC( victim ) >=      0 ) strcat( buf1, "�u��ۤ��示�ǡC" );
	else if ( GET_AC( victim ) >=   -400 ) strcat( buf1, "��۴��q����A�C" );
	else if ( GET_AC( victim ) >=   -500 ) strcat( buf1, "�h�֦��I�˳ơC" );
	else if ( GET_AC( victim ) >=  -1000 ) strcat( buf1, "�˳ư_�ӡC" );
	else if ( GET_AC( victim ) >=  -1500 ) strcat( buf1, "���˳Ƥ����C" );
	else if ( GET_AC( victim ) >=  -2000 ) strcat( buf1, "���˳ƫܦn�C" );
	else if ( GET_AC( victim ) >=  -2500 ) strcat( buf1, "��ۭ����˳ơC" );
	else if ( GET_AC( victim ) >=  -3000 ) strcat( buf1, "��۶W�Ū��˳ơC" );
	else if ( GET_AC( victim ) >=  -3500 ) strcat( buf1, "���˳ƥu���ѤW���C" );
	else if ( GET_AC( victim ) >=  -4000 ) strcat( buf1, "���˳ƤѤU�L�ġC" );
	else if ( GET_AC( victim ) >=  -5000 ) strcat( buf1, "��۶W�ŵL�Ķ����R�E���˳ơC" );
	else                                   strcat( buf1, ""HIR"���˳ƤӸرi�F�A�p���I�A�γ\�O�� bug�I"NOR""  );
	send_to_char( buf1, ch );

	if ( victim->sex ==1 )
		sprintf( buf1, "\n\r�L" );
	else if ( victim->sex == 2 )
		sprintf( buf1, "\n\r�o" );
	else
		sprintf( buf1, "\n\r��" );

	if (      get_damroll( victim, WEAR_WIELD ) <  50 ) strcat( buf1, "�ݨӲ@�L�}�a�O�A" );
	else if ( get_damroll( victim, WEAR_WIELD ) < 100 ) strcat( buf1, "���Z�˫j�j��y���@�Ƕˮ`�A" );
	else if ( get_damroll( victim, WEAR_WIELD ) < 150 ) strcat( buf1, "���G�Z�j���A" );
	else if ( get_damroll( victim, WEAR_WIELD ) < 200 ) strcat( buf1, "��y���ܤj�ˮ`�A" );
	else                                               strcat( buf1, ""HIR"���G��}�O���ۡI"NOR"" );

	send_to_char( buf1, ch );
	/* additions by king@tinuviel.cs.wcu.edu */
	hpdiff = ( ch->hit - victim->hit );

	if ( ( ( diff >= 0) && ( hpdiff <= 0 ) ) || ( ( diff <= 0 ) && ( hpdiff >= 0 ) ) )
	{
		send_to_char( "�ӥB", ch );
	}
	else
	{
		send_to_char( "���L", ch );
	}

	if ( hpdiff >= 101 )
		buf = "�A���ͩR�O��$E�h���h���C[m";
	if ( hpdiff <= 100 )
		buf = "�A���ͩR�O��$E�h�C[m";
	if ( hpdiff <= 50 ) 
		buf = "�A���ͩR�O��$E�y�h�C[m";
	if ( hpdiff <= 25 )
		buf = "�A���ͩR�O��$E�h�@�I�I�C[m";
	if ( hpdiff <= 0 )
		buf = "$E���ͩR�O��A�h�@�I�I�C[m";
	if ( hpdiff <= -25 )
		buf = "$E���ͩR�O��A�y�h�C[m";
	if ( hpdiff <= -50 )
		buf = "$E���ͩR�O��A�h�C[m";
	if ( hpdiff <= -100 )
		buf = "$E���ͩR�O��A�h���h���C[m";

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

    sprintf( buf1, "%s(%s)�ĨĦa���b�o�̡C\n\r", ch->pcdata->horse->cname, ch->pcdata->horse->name );
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
    sprintf( buf, "%s(%s)�ĨĦa���b�o�̡C\n\r", ch->cname, ch->name );
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
  send_to_char( "[33m�n�Ⱘ���W�r�令����H[m\n\r", ch );
  return;
    }

    if( !ch->pcdata->horse ) {
  send_to_char(YEL "�A�S�����C"NOR"\n\r", ch);
  return;
    }

    if( IS_SET(ch->act, PLR_HORSE ) ){
  send_to_char(YEL "���������ۤv���W�r�C"NOR"\n\r", ch);
  return;
    }

    if ( strlen( argument ) > 21 )
    {
  argument[21] = '\0';
  send_to_char(YEL "�A�����W�r�Ӫ��F�A�Ш��@�ӵu�@�I���C"NOR"\n\r", ch);
  return;
    }
     if( !check_parse_name( argument ) ) {
         send_to_char("���O���X�k���W�r�A�Э��s���@�ӡC\n\r", ch);
         return;
     }

    if ( !str_prefix( argument, ch->name ) || !str_prefix( ch->name, argument) )
    {
        send_to_char( "�����W�r����M�D�H�ӹ��C\r\n", ch );
        return;
    }

    if ( !str_infix( argument, ch->name ) || !str_infix( ch->name, argument) )
    {
        send_to_char( "�����W�r����M�D�H�ӹ��C\r\n", ch );
        return;
    }
    


    if( has_player( argument ) ){
  send_to_char(YEL "����ΧO�����̪��W�r�C"NOR"\n\r", ch);
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
        send_to_char( "[33m�n��A������W�r�令����H[m\n\r", ch );
        return;
    }
    else if(len > 12 || len2 > 100)
    {
  	argument[12] = '\0';
	send_to_char(YEL "�A���W�r�Ӫ��F�A�Ш��@�ӵu�@�I���C"NOR"\n\r", ch);
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
  send_to_char( "[33m�n��A���ٸ��令����H[m\n\r", ch );
  return;
    }

    if ( str_length( argument ) > 30 )
    {
  argument[30] = '\0';
  send_to_char(YEL "�A���ٸ��Ӫ��F�A�зQ�@�ӵu�@�I���A�T�G�@�I���C"NOR"\n\r", ch);
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
				send_to_char( "[33m�y�z�Ӫ��F�C[m\n\r", ch );
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
			send_to_char( "[33m�y�z�Ӫ��F�C[m\n\r", ch );
			return;
		}

		strcat( buf, argument );
		strcat( buf, "\n\r" );
		free_string( ch->description );
		ch->description = str_dup( buf );
	}

    send_to_char( "�A���y�z�O�G\n\r", ch );
    send_to_char( ch->description ? ch->description : "�]�L�^�C\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    char buf [ MAX_INPUT_LENGTH ];

    sprintf( buf,
      "[36m�A�����A�G%d/%d�I�ͩR�O�B%d/%d�I�]�k�O�B%d/%d�I���ʤO�B%d�I�g��ȡC[m\n\r",
      ch->hit,  ch->max_hit,
      ch->mana, ch->max_mana,
      ch->move, ch->max_move,
      ch->exp );

    send_to_char( buf, ch );

    sprintf( buf,
      "[36m$n���i���A�G%d/%d�I�ͩR�O�B%d/%d�I�]�k�O�B%d/%d�I���ʤO�B%d�I�g��ȡC[m\n\r",
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
			send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
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

		sprintf( buf, "�A�٦�%d�I�m���I�ơC\n\r",ch->practice );
		strcat( buf1, buf );
		sprintf( buf, "�m�ߪ��O�άO%d�T�����C\n\r", money );
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
			send_to_char("[33m�A�o�n�b�T�ťH�W�~��m��(practice)�A���h�V�m(train)�a�I[m\n\r", ch );
			return;
		}

		if( !IS_AWAKE( ch ) )
		{
			send_to_char( "[33m�O�n�b�ڸ̽m�߶ܡH[m\n\r", ch );
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
			send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
			return;
		}

		if( ch->practice <= 0 )
		{
			send_to_char( "[33m�A�S���m���I�ƤF�C[m\n\r", ch );
			return;
		}
		else if ( money > ch->gold && ch->class > 4)
		{
			send_to_char( "[33m�A�I���_�m�ߪ��O�ΡC[m\n\r", ch );
			return;
		}

		if(( sn = skill_lookup( argument ) ) < 0 || ( ( skill_req_level( ch, sn ) > -1 && ch->level < skill_req_level(ch, sn))))
		{
			send_to_char( "[33m�A����m�ߨ��ӡC[m\n\r", ch );
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
				send_to_char( "[33m�A����m�ߨ��ӡC[m\n\r", ch );
				return;
			}
		}

		switch( is_skname( argument, mob->psk ) )
		{
			case -1 :
				act( "$n ���G�藍�_�A�ڨä��|�����ޥ��C", mob, NULL, ch, TO_VICT );
				return;
			case 1 :
				for( qmark = ch->qmark; qmark; qmark = qmark->next )
					if ( !str_cmp( qmark->mark_name, skill_table[sn].name ) )
						break;
				if ( !qmark )
				{
					act( "$n ���G�藍�_�A�A�٤������ǲ߳o���ޥ��C", mob, NULL, ch, TO_VICT );
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
			sprintf( buf, "[33m�A�w�g�L�k����%s(%s)�����m�פF�C[m\n\r", skill_table[sn].cname, skill_table[sn].name );
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
				act( "[36m�A�m�ߤF$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n�m�ߤF$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
			}
			else
			{
				act( "[36m�A�{�b�O$t($T)���M�a�F�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n�{�b�O$t($T)���M�a�F�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
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
			send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
			return;
		}

		if( !mob->psk ) 
		{
			send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
			return;
		}
		if( ch->practice <= 0 )
		{
			send_to_char( "[33m�A�S���m���I�ƤF�C[m\n\r", ch );
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

			sprintf( buf, "�A�٦�%d�I�m���I�ơC\n\r", ch->practice );
			strcat( buf1, buf );
			sprintf( buf, "�m�ߪ��O�άO%d�T�����C\n\r", money );
			strcat( buf1, buf );
			send_to_char( buf1, ch );
			return;
		}
		else if ( money > ch->gold && ch->class > 4 )
		{
			send_to_char( "[33m�A�I���_�m�ߪ��O�ΡC[m\n\r", ch );
			return;
		}

		if(( sn = skill_lookup( arg1 ) ) < 0 || ( ( skill_req_level(ch, sn) > -1 && ch->level < skill_req_level(ch, sn))))
		{
			send_to_char( "[33m�A����m�ߨ��ӡC[m\n\r", ch );
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
				send_to_char( "[33m�A����m�ߨ��ӡC[m\n\r", ch );
				return;
			}
		}

		switch( is_skname( arg1, mob->psk ) )
		{
			case -1 :
				act( "$n ���G�藍�_�A�ڨä��|�����ޥ��C", mob, NULL, ch, TO_VICT );
				return;
			case 1 :
				for( qmark = ch->qmark; qmark; qmark = qmark->next )
					if ( !str_cmp( qmark->mark_name, skill_table[sn].name ) )
						break;
				if ( !qmark )
				{
					act( "$n ���G�藍�_�A�A�٤������ǲ߳o���ޥ��C", mob, NULL, ch, TO_VICT );
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
			sprintf( buf, "[33m�A�w�g�L�k����%s(%s)�����m�פF�C[m\n\r", skill_table[sn].cname, skill_table[sn].name );
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
				act( "[36m�A�m�ߤF$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n�m�ߤF$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
			}
			else
			{
				act( "[36m�A�{�b�O$t($T)���M�a�F�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
				act( "[36m$n�{�b�O$t($T)���M�a�F�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );
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
    send_to_char("�A���ݭn�A��¾�F�C\n\r", ch);
    return;
  }

  buf1[0] = '\0';

  if ( ch->level < 20 )
  {
    send_to_char(
        "[33m�A�o�n�b�G�Q�ťH�W�~����¾(join)�A���]�k�ɯŧa�I[m\n\r",
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
      send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
      return;
    }

    col = 0;    

    strcat( buf1, "�A����[�J���i��¾�~���G\n\r" );
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

    sprintf( buf, "��¾�һݪ��O�άO%d�T�����C\n\r", money );
    strcat( buf1, buf );
    send_to_char( buf1, ch );
  }
  else
  {
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    if ( !IS_AWAKE( ch ) )
    {
      send_to_char( "[33m�O�n�b�ڸ���¾�ܡH[m\n\r", ch );
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
      send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
      return;
    }

    if ( money > ch->gold )
    {
      send_to_char( "[33m�A�I���_��¾���O�ΡC[m\n\r", ch );
      return;
    }

    if ( ( cn = class_lookup( argument ) ) < 0
        || !is_name( class_table[cn].name, mob->jcl) )

    {
      send_to_char( "[33m�A�L�k�b���[�J����¾�~�C[m\n\r", ch );
      return;
    }

    if ( !IS_NPC( ch ) &&
          class_table[cn].bclass[ch->class] > ch->level )
    {
      sprintf( buf, "[33m�A�ܤ֭n��%d�Ť~��[�J����¾�~�C[m\n\r", class_table[cn].bclass[ch->class] );
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

    sprintf( buf, HIW"%s �j�n�a�ť��G�w�� %s ���� %s �������I\n\r"NOR, 
        mob->short_descr, ch->name, class_table[cn].cname );
    send_to_all_char( buf );

    sprintf(buf,
     HIP"���ߧA�I�A�{�b�i�H�}�l���ɧޯ൥�ŤF�I\n\r"
        "�b�ѹL���ȩ����ѯS�w�ؼСA�O�o�V�ޯ�p�Ѩ�(Angel of Skill)����ޯ�g���I��I\n\r"
        "\n\r"
        "����ޯ൥�Ũt�ΡA�п�J help skexp, help skadv �� help skdemote �Ѧһ������\n\r"NOR);
    send_to_char(buf, ch);
    gain_skill_exp(ch, 100000);
    send_to_char("\n\r�A�o��F 10 �U�I�ޯ�g���I�I\n\r", ch);
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
  send_to_char( "[33m�A���i��n��A�����z�ӱo���C[m\n\r", ch );
  return;
    }

    if ( wimpy > ch->max_hit / 2 )
    {
  send_to_char( "[33m�A�O�x�p���r�H[m\n\r", ch );
  return;
    }

    ch->wimpy      = wimpy;
    sprintf( buf, "[36mWimpy�]�w��%d�I�ͩR�ȥH�U�|�۰ʰk�]�C[m\n\r", wimpy );
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
    send_to_char( "���O���� password ���C\n\r", ch );
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
  send_to_char( "[33m�榡�Gpassword <�±K�X> <�s�K�X>�C[m\n\r", ch );
  return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
  WAIT_STATE( ch, 40 );
  send_to_char( "[33m�K�X���~�C���Q�����C[m\n\r", ch );
  return;
    }

    if ( strlen( arg2 ) < 5 )
    {
  send_to_char(
      "[33m�s���K�X�ֱ̤o�n�����Ӧr�C[m\n\r", ch );
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
    "[33m�s���K�X����ΡA�ոէO���C[m\n\r", ch );
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
          send_to_char( "[33m�A�Q�T��ϥ��W�D�F�C[m\n\r", ch );
          return;
      }
      
      send_to_char( "[36m�W�D�G", ch );
      
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
      
      send_to_char( "�C[m\n\r", ch );
      
      if( ch->trust >= 44 || ch->level >=44)
      {
          send_to_char( "�A�ұ��������|�W�D�G", ch );
          
          if( !str_cmp( ch->clan_channel, "all" ) )
          {
              send_to_char( "�Ҧ����|", ch );
          }
          else if ( ch->clan_channel )
          {
              send_to_char( ch->clan_channel, ch );
          }
          else
          {
              send_to_char( "�L", ch );
          }
          
          send_to_char( "�I\n\r", ch );
          send_to_char( "���|�W�٦C��G\n\r", ch );
          
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
                      sprintf( buf, HIW"�A�ҰʤF�ìP��ť�t�ΡI"NOR"\n\r");
                      send_to_char( buf, ch );
                      return;
              	  }
              	  else if( !str_cmp( arg2, "close"  ) )
              	  {
                      ch->tell_channel = NULL;
                      sprintf( buf, HIW"�A�����F�ìP��ť�t�ΡI"NOR"\n\r");
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
                                sprintf( buf, HIB"�A�Q�����b%s���W����ť���I"NOR"\n\r", arg2 );
                                send_to_char( buf, ch );
                                sprintf( buf, HIW"�ڳQ�o�{�F�I�I"NOR"\n\r");
                                send_to_char( buf, ch );
                                return;
                            }
                            else
                            {
                                ch->tell_channel = str_dup( arg2 );
                                sprintf( buf, HIB"�A�����b%s���W�ˤF��ť���I"NOR"\n\r", arg2 );
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
              send_to_char( "�A�}�l�����Ҧ����|�W�D�I\n\r", ch );
              return;
          }
          else if( !str_cmp( arg1, "close"  ) )
          {
              ch->clan_channel = NULL;
              send_to_char( "�A�������|�W�D�I\n\r", ch );
              send_to_char( "\n\r", ch );
              return;
          }
          else
          {
              for( cn = clan_list; cn; cn = cn->next )
              {
                   if( !str_cmp( cn->name, arg1 ) || !str_cmp( cn->whoname, arg1 ) )
                   {
                       sprintf( buf, "�A�}�l����%s�����|�W�D�I\n\r", cn->whoname );
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
          send_to_char( "[33m�n�}�����@���W�D�H[m\n\r", ch );
          return;
      }
      
      if( fClear )
          REMOVE_BIT ( ch->deaf, bit );
      else
          SET_BIT    ( ch->deaf, bit );
      
      send_to_char( "OK�I\n\r", ch );
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
  send_to_char( "[36m[  ����r  ] �ﶵ\n\r[m", ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOEXIT  )
      ? "[+AUTOEXIT ] �A�۰ʧ�X�f�C\n\r"
      : "[-autoexit ] �A���۰ʧ�X�f�C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOLOOT  )
      ? "[+AUTOLOOT ] �A�۰ʱq����̱����ԧQ�~�C\n\r"
      : "[-autoloot ] �A���۰ʱq����̱����ԧQ�~�C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOGOLD  )
      ? "[+AUTOGOLD ] �A�۰ʱq����̮������C\n\r"
      : "[-autogold ] �A���۰ʱq����̮������C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOSAC   )
      ? "[+AUTOSAC  ] �A�۰ʩ^�m����C\n\r"
      : "[-autosac  ] �A���۰ʩ^�m����C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_AUTOSPLIT  )
      ? "[+AUTOSPLIT] �A�۰ʤ��ɪ����C\n\r"
      : "[-autosplit] �A���۰ʤ��ɪ����C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_BLANK     )
      ? "[+BLANK    ] �A�b���ܦr��e���@��ťաC\n\r"
      : "[-blank    ] �A�b���ܦr��e�S���@��ťաC\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_BRIEF     )
      ? "[+BRIEF    ] �A�n��²�u���y�z�C\n\r"
      : "[-brief    ] �A�n�ݧ��㪺�y�z�C\n\r"
      , ch );
   
  send_to_char(  IS_SET( ch->act, PLR_COMBINE   )
      ? "[+COMBINE  ] �A�ϥε��X���覡�ݪ��~�M��C\n\r"
      : "[-combine  ] �A�ϥγ�W���覡�ݪ��~�M��C\n\r"
      , ch );

  send_to_char(  IS_SET( ch->act, PLR_PROMPT    )
      ? "[+PROMPT   ] �A�����ܦr��C\n\r"
      : "[-prompt   ] �A�S�����ܦr��C\n\r"
      , ch );
/*
  send_to_char(  IS_SET( ch->act, PLR_TELNET_GA )
      ? "[+TELNETGA ] �A����telnet GA�r��C\n\r"
      : "[-telnetga ] �A������telnet GA�r��C\n\r"
      , ch );
*/      
  send_to_char(  IS_SET( ch->act, PLR_ARTIFACT )
      ? "[+ARTIFACT ] �A�ϥθ˳ƪ����äO�q�C\n\r"
      : "[-artifact ] �A���ϥθ˳ƪ����äO�q�C\n\r"
      , ch );
      
  send_to_char(  IS_SET( ch->act, PLR_SILENCE   )
      ? "[+SILENCE  ] �A�Q�H�q�ƤF�C\n\r"
      : ""
      , ch );

  send_to_char( !IS_SET( ch->act, PLR_NO_EMOTE  )
      ? ""
      : "[-emote    ] �A���i�H��F���P�C\n\r"
      , ch );

  send_to_char( !IS_SET( ch->act, PLR_NO_TELL   )
      ? ""
      : "[-tell     ] �A����Ρutell�v�C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_COMBAT )
      ? "[+COMBAT   ] �A��������԰��T���C\n\r"
      : "[-combat   ] �A�ȱ�����²�԰��T���C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_CHINESE )
      ? "[+CHINESE  ] �A�Ұʤ���Ҧ��C\n\r"
      : "[-chinese  ] �A���Ұʤ���Ҧ��C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_STATUS )
      ? "[+STATUS   ] �A�Ұʾ԰���q�p����C\n\r"
      : "[-status   ] �A�����԰���q�p����C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_TITLE    )
      ? "[+TITLE    ] �A���ٸ����|�ܰʡC\n\r"
      : "[-title    ] �A���ٸ��|�۰ʧ��ܡC\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_ASSIST    )
      ? "[+ASSIST   ] �A�۰ʨ�U���ͧ@�ԡC\n\r"
      : "[-assist   ] �A����U���ͧ@�ԡC\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_NOTIFY )
      ? "[+NOTIFY   ] �A�����t�δ��ܰT���C\n\r"
      : "[-notify   ] �A���Q�����t�δ��ܰT���C\n\r"
      , ch );
  send_to_char(  IS_SET( ch->act, PLR_DEAD )
    ? "[+DEAD     ] �A���`����N�۰����}����C\n\r"
    : "[-dead     ] �A���`����N���|�۰����}����C\n\r"
    , ch );
  send_to_char(  IS_SET( ch->act, PLR_GRAPHIC )
    ? "[+GRAPHIC  ] �A�ϥιϧΤƶ����ơC\n\r"
    : "[-graphic  ] �A���ϥιϧΤƶ����ơC\n\r"
    , ch );
  if( IS_IMMORTAL( ch ) ) {
      send_to_char(  IS_SET( ch->act, PLR_VERBOSEALL )
        ? "[+VERBOSEALL] �A��ܥ����ԲӸ�ơC\n\r"
        : "[-verboseall] �A���Q��ܥ����ԲӸ�ơC\n\r"
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
      send_to_char( "[33m�n�]���@�ӿﶵ�H[m\n\r", ch );
      return;
  }

  if ( fSet )
  {
      SET_BIT    ( ch->act, bit );
      sprintf( buf, "[36m%s�{�b���}�F�C[m\n\r", arg+1 );
      buf[5] = UPPER( buf[5] );
      send_to_char( buf, ch );
  }
  else
  {
      REMOVE_BIT ( ch->act, bit );
      sprintf( buf, "[36m%s�{�b���_�ӤF�C[m\n\r", arg+1 );
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
		send_to_char ( "[33m�A���ݭn�ϥΩG��C[m\n\r", ch );
		return;
	}

	buf1[0] = '\0';

	col = 0;
	strcat(buf, "�G   ��   �C   ��G\n\r");
	for ( sn = 1; sn < MAX_SKILL; sn++ )
	{
		if ( !skill_table[sn].name )
			break;

		//|| ( skill_table[sn].skill_level[ch->class] > LEVEL_HERO ) )
		if ( (  skill_table[sn].spell_fun == spell_null ))
		continue;

		//if(ch->pcdata->learned[sn] == 0) continue; 
        if( !get_skill_percent(ch, sn) ) continue;

		sprintf ( buf, "%15s(%17s) %3d�I",
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
			send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
			return;
		}

		if( (!IS_SET( victim->act, PLR_HORSE )
			|| victim != ch->pcdata->horse
			|| victim->in_room != ch->in_room)
			&& !IS_IMMORTAL( ch ))
			{
				send_to_char("�A����ݧO�H����ơC\n\r", ch);
				return;
			}
	}


	if ( IS_NPC( victim ) )
    {
		send_to_char ( "[33m�A���ݭn�Ψ�ޯ�C[m\n\r", ch );
		return;
    }

	buf1[0] = '\0';

	strcat ( buf1, "�A��¾�~�i�H�Ϊ��ޯ�p�U�G\n\r\n\r" );
	strcat ( buf1, "����          �ޯ�/�]�k�W��                   ���m��     �ޯ൥��  �һݸg��\n\r\n\r" );
    //              [ 1]  123456789012345678(123456789012345678) 123%

	max_level = ( victim->level < L_APP ) ? 42 : L_APP;
	fFound = FALSE;
	level = 0;
	// �קK�U���O�@���]�Xbug log overflow[0] �N sn �קאּ�q 1 �}�l�� 2021/12/20
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
		// �קK�U���O�@���]�Xbug log overflow[0] �N sn �קאּ�q 1 �}�l�� 2021/12/20
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
				sprintf ( buf, "%16s(%25s)  %3d%%/%3d%%   %8s \n\r", skill_table[sn].cname, // �����ޯ�W�L��,�׬� 25 �r 2021/10/10
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
					//sprintf ( buf, "%16s(%19s)  %3d%%/%3d%%   %8s  [�ۢϢ�]\n\r", skill_table[sn].cname,
					sprintf ( buf, "%16s(%25s)  %3d%%/%3d%%   %8s  [�ۢϢ�]\n\r", skill_table[sn].cname,
							skill_table[sn].name,
							//victim->pcdata->learned[sn]
							get_skill_percent(victim, sn),
							skill_eff_percent(victim, sn),
							skill_exp_desc(get_skill_level(victim, sn)));
							//,skill_difficulty(victim,sn) );
				}
			}
			strcat ( buf1, buf );
			// �ק�̤j pagelength �� 70, �G�i��ܤW������ 2023/01/12
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
		send_to_char( "[33m�C������֩�Q�E��C[m", ch );
		return;
	}

	if ( lines > 70 )
	{
		send_to_char( "[33m�Фų]�w�ܶW�L�C�Q��I[m\n\r", ch );
		lines = 70;
    }

	ch->pcdata->pagelen = lines;
	sprintf( buf, "[36m�����]�w�]��@��%d��C[m\n\r", lines );
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
		strcat( buf, "[m<�ͩR[1;32m%h[m�]�k[1;33m%m[m����[1;35m%v[m>");
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
	send_to_char( "[36m�S���D�C[m\n\r", ch );
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
		send_to_char( "[36m�A�^��F�ǻ����C[m\n\r", ch );
		act( "[36m$n�^��F�ǻ����C[m", ch, NULL, ch, TO_ROOM );
	}
	else
	{
		SET_BIT( ch->act, PLR_AFK );
		send_to_char( "[36m�A�Ȯ����}�ǻ��C[m\n\r", ch   );
		act( "[36m$n�Ȯ����}�ǻ��C[m", ch, NULL, ch, TO_ROOM );
	}

	return;
}

/* scan skill added by Amenda 00/12/6 */
void do_scan( CHAR_DATA *ch, char *argument )
{
	EXIT_DATA *pexit;
	int i;

	if ( !IS_NPC( ch )
		&& ( ch->race != race_lookup( "Hobbit" ) ) // hobbit scan ���|���� 2022/02/01
		&& number_percent() > get_skill_percent(ch, gsn_scan))
	{
		send_to_char( "�A���ѤF!\n\r", ch );
		return;
	}
	// affect stone form �ץ� 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�b�ۤƪ��A�U���ಾ��.\n\r", ch);
		return;
	}

	if ( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare �ԭz�ק� 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m�A�Q�x�b�����̤F�I�A���ಾ�ʡI[m\n\r", ch );
		}else{
			send_to_char( "[33m�A������ʼu���o�I�A���ಾ�ʡI[m\n\r", ch );
		}
		return;
	}

	if ( IS_AFFECTED( ch, AFF_PARALYZED ) ) 
	{
		send_to_char( "�A�]�·��Ӱʼu���o�I\n\r", ch );
		return;
	}

	/*  �����ثe�ж��·t�h�L�k�ϥΪ��]�w 2022/02/01
	if ( room_is_dark( ch->in_room ) || !check_blind( ch ) )
	{
		send_to_char( "�|�P�@�����¡A�A���򳣬ݤ���!\n\r", ch );
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
      
    strcpy( buf, "$N �b�A�� ");
    switch ( door )
    {
    case 0 : strcat( buf, "�_��" ); break;
    case 1 : strcat( buf, "�F��" ); break;
    case 2 : strcat( buf, "�n��" ); break;
    case 3 : strcat( buf, "����" ); break;
    case 4 : strcat( buf, "�W��" ); break;
    case 5 : strcat( buf, "�U��" ); break;
    }
    switch (level)
    {
    case 1 : break;
    case 2 : strcat( buf, " ����" ); break;
    case 3 : strcat( buf, " �����B" ); break;
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
		send_to_char( "�A���ѤF!\n\r", ch );
		return;
	}
	// affect stone form �ץ� 2022/12/04
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�b�ۤƪ��A�U���ಾ��.\n\r", ch);
		return;
	}

	if ( IS_AFFECTED( ch, AFF_HOLD ) ) 
	{
		// snare �ԭz�ק� 2022/12/04
		if( is_affected(ch, gsn_snare) ){
			send_to_char( "[33m�A�Q�x�b�����̤F�I�A���ಾ�ʡI[m\n\r", ch );
		}else{
			send_to_char( "[33m�A������ʼu���o�I�A���ಾ�ʡI[m\n\r", ch );
		}
		return;
	}

	if ( IS_AFFECTED( ch, AFF_PARALYZED ) ) 
	{
		send_to_char( "�A�]�·��Ӱʼu���o!\n\r", ch );
		return;
	}

	if( auction_list )
	{
		AUC_DATA *auction;
		for( auction = auction_list ; auction; auction = auction->next )
		{
			if ( ch == auction->owner || ch == auction->buyer )
			{
				send_to_char("�A���b�i����A�������}�o�̡C\n\r", ch);
				return;
			}
		}
	}

	/*  �����ثe�ж��·t�h�L�k�ϥΪ��]�w 2022/02/01
	if ( room_is_dark( ch->in_room ) || !check_blind( ch ) )
	{
		send_to_char( "�|�P�@�����¡A�A���򳣬ݤ���!\n\r", ch );
		return;
	}
	*/

	// hobbit ���B�~ sklv �[�� , �� sklv �̰��i�� 7  2022/02/01
	if( ch->race == race_lookup( "Hobbit" ) )
		sklv += 1;

	// mv �̧C���� 1  2022/02/01
	nMoveCR = UMIN( 299 , sklv * (sklv + 1) * 5 );

	// �ץ������رڤ��|�P�w���� 2021/10/02
	if ( race_table[ch->race].thirst_mod >= 0 && ch->pcdata->condition[COND_THIRST] <= 15 && sklv < 4)
	{
		send_to_char( "�A�Ӵ��F�C\n\r", ch );
		return;
	}
	// �ץ����Ⱦj�رڤ��|�P�w�Ⱦj�� 2021/10/02
	if( race_table[ch->race].hunger_mod >= 0 && ch->pcdata->condition[COND_FULL] <= 15 && sklv < 5)
	{
		send_to_char( "�A�Ӿj�F�C\n\r", ch );
		return;
	}
	if(ch->move < 300 - nMoveCR)
	{
		send_to_char( "�A�S����������O�C\n\r", ch );
		return;
	}
	else
	{
		// �ץ����Ⱦj�رڤ��|�����Ⱦj��  2021/10/02
		if( sklv < 4 && race_table[ch->race].thirst_mod >= 0 )
			ch->pcdata->condition[COND_THIRST] -= 12 - sklv*2;
		// �������A�U���|����Ⱦj�ȡA�B���Ⱦj�رڤ��|�����Ⱦj�� 2021/10/02
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
		send_to_char( "�A�쩳���S����V�P�ڡH\n\r", ch );
		return;
	}
  
	in_room = ch->in_room;

	// �Ĥ@�� scout , �ק�ԭz�[�J��V 2022/12/22
	//send_to_char( ""HIB"�A�յ۰���a��A���a��G"NOR"\n\r", ch );
	if( i == 0 )
		send_to_char( ""HIB"�A�յ۩�"HIC"�_"HIB"��i�氻��G"NOR"\n\r", ch );
	if( i == 1 )
		send_to_char( ""HIB"�A�յ۩�"HIC"�F"HIB"��i�氻��G"NOR"\n\r", ch );
	if( i == 2 )
		send_to_char( ""HIB"�A�յ۩�"HIC"�n"HIB"��i�氻��G"NOR"\n\r", ch );
	if( i == 3 )
		send_to_char( ""HIB"�A�յ۩�"HIC"��"HIB"��i�氻��G"NOR"\n\r", ch );
	if( i == 4 )
		send_to_char( ""HIB"�A�յ۩�"HIC"�W"HIB"��i�氻��G"NOR"\n\r", ch );
	if( i == 5 )
		send_to_char( ""HIB"�A�յ۩�"HIC"�U"HIB"��i�氻��G"NOR"\n\r", ch );
	if ( !( pexit = ch->in_room->exit[i] ) || !( to_room = pexit->to_room ) ){
		send_to_char( "[33m�u�r�A�������q�C[m\n\r", ch );
		return;
	}else{
		if( can_scout_check( ch, pexit, to_room, sklv ) )
			scout( ch, pexit, to_room, sklv );
		else
			return;
	}

	// �ҳB�ж��� ROOM_SCOUT_ONE �� room flag �h�u��d�@��
	if ( IS_SET( in_room->room_flags, ROOM_SCOUT_ONE ) ){
		send_to_char( ""HIB"�A�յ۰�����B�A���A���ѤF�I"NOR"\n\r", ch );
	}
	else
	{
		// �ĤG�� scout
		if( i == 0 )
			send_to_char( ""HIB"�A�յ۰���"HIC"�_"HIB"�䤣���B�G"NOR"\n\r", ch );
		if( i == 1 )
			send_to_char( ""HIB"�A�յ۰���"HIC"�F"HIB"�䤣���B�G"NOR"\n\r", ch );
		if( i == 2 )
			send_to_char( ""HIB"�A�յ۰���"HIC"�n"HIB"�䤣���B�G"NOR"\n\r", ch );
		if( i == 3 )
			send_to_char( ""HIB"�A�յ۰���"HIC"��"HIB"�䤣���B�G"NOR"\n\r", ch );
		if( i == 4 )
			send_to_char( ""HIB"�A�յ۰���"HIC"�W"HIB"�褣���B�G"NOR"\n\r", ch );
		if( i == 5 )
			send_to_char( ""HIB"�A�յ۰���"HIC"�U"HIB"�褣���B�G"NOR"\n\r", ch );
		//send_to_char( ""HIB"�A�յ۰�����B�G"NOR"\n\r", ch );
		if ( !( pexit = to_room->exit[i] ) || !( to_room = pexit->to_room ) ){
			send_to_char( "[33m�u�r�A�������q�C[m\n\r", ch );
		}else{
			if( can_scout_check( ch, pexit, to_room, sklv ) )
			{
				scout( ch, pexit, to_room , sklv );
				// �ĤT�� scout
				if( sklv > 5 ){
					if( i == 0 )
						send_to_char( ""HIB"�A�յ۰���"HIC"�_"HIB"������B�G"NOR"\n\r", ch );
					if( i == 1 )
						send_to_char( ""HIB"�A�յ۰���"HIC"�F"HIB"������B�G"NOR"\n\r", ch );
					if( i == 2 )
						send_to_char( ""HIB"�A�յ۰���"HIC"�n"HIB"������B�G"NOR"\n\r", ch );
					if( i == 3 )
						send_to_char( ""HIB"�A�յ۰���"HIC"��"HIB"������B�G"NOR"\n\r", ch );
					if( i == 4 )
						send_to_char( ""HIB"�A�յ۰���"HIC"�W"HIB"������B�G"NOR"\n\r", ch );
					if( i == 5 )
						send_to_char( ""HIB"�A�յ۰���"HIC"�U"HIB"������B�G"NOR"\n\r", ch );
					if ( !( pexit = to_room->exit[i] ) || !( to_room = pexit->to_room ) ){
						send_to_char( "[33m�u�r�A�������q�C[m\n\r", ch );
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
	act( "�A�o�{ $n ���й����A�����j���a�q���t�����X�{�C", ch, NULL, NULL, TO_ROOM );
}

// �P�w scout ���ж��O�_��i�J 2022/12/22
bool can_scout_check( CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room, int sklv )
{

	if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
	{
		if ( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) )
		{
			act( "[33m$d�O���۪��C[m",ch, NULL, pexit->keyword, TO_CHAR );
			return FALSE;
		}
		if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
		{
			act( "[33m�A�藍�L$d�C�n�h�I[m",ch, NULL, pexit->keyword, TO_CHAR );
			return FALSE;
		}
	}
	if ( to_room->sector_type == SECT_AIR )
	{
		if( !is_flying(ch))
		{
			send_to_char( "[33m�A�����_�ӡC[m\n\r", ch );
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
		send_to_char( "[33m�A�b��a¶��¶�h�o�夣�L�h�I[m\n\r", ch );
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
			send_to_char( "[33m�A�o�n���@����~�L�o�h�C[m\n\r", ch );
			return FALSE;
		}
	}

	// door size �P�w 2022/12/22
	if( sklv < 2 )
	{
		//if ( pexit->exit_size > 0 && ( race_table[ch->race].size > pexit->exit_size ) )
		if ( pexit->exit_size > 0 && ( get_curr_size( ch ) > pexit->exit_size ) ) // �����髬 size ���� 2022/12/23
		{
			send_to_char( "[33m�A�髬�Ӥj�A�i�J���F�q�D�C[m\n\r", ch );
			return FALSE;
		}
	}

	// room size �H�ƧP�w 2022/12/22
	if( sklv < 4 )
	{
		if ( room_is_private( ch, to_room ) )
		{
			send_to_char( "[33m���̪��Ŷ��j�p�A�e�Ǥ��U�A����n�C[m\n\r", ch );
			return FALSE;
		}
	}

	return TRUE;
}
void scout( CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room, int sklv )
{
	int room_flags = 0; // 2022/12/22

	// remove at 2022/12/22
	// ���q�P�w�ȯ�B�z�Q scout ������ж�, ��Ĥ@��ж��L�k�i�J��, �L�k���׶i�J�ĤG��ж�
	// �G���ܤW��@�� bool can_scout_check �ӿW�ߧP�w�O�_���ж��@ scout
		/*if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
		{
			if ( !IS_AFFECTED( ch, AFF_PASS_DOOR ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_PASSDOOR ) )
			{
				act( "[33m$d�O���۪��C[m",ch, NULL, pexit->keyword, TO_CHAR );
				return;
			}
			if ( IS_SET( pexit->exit_info, EX_PASSPROOF ) )
			{
				act( "[33m�A�藍�L$d�C�n�h�I[m",ch, NULL, pexit->keyword, TO_CHAR );
				return;
			}
		}
		if ( to_room->sector_type == SECT_AIR )
		{
			if( !is_flying(ch))
			{
				send_to_char( "[33m�A�����_�ӡC[m\n\r", ch );
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
			send_to_char( "[33m�A�b��a¶��¶�h�o�夣�L�h�I[m\n\r", ch );
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
				send_to_char( "[33m�A�o�n���@����~�L�o�h�C[m\n\r", ch );
				return;
			}
		} */

	char_from_room( ch );
	char_to_room( ch, to_room );
	do_look( ch, "" );
	// �[�J sklv room flag �ԭz 2022/12/22
	send_to_char( "�o��", ch );
	if( sklv >= 3 )
	{
		if( IS_SET( to_room->room_flags, ROOM_NO_RECALL) )
		{
			if( IS_SET( to_room->room_flags, ROOM_PK) )
				send_to_char( "�O�����G��", ch );
			else
				send_to_char( "������G", ch );
			room_flags += 1;
		}

		if ( room_flags < 0 ) send_to_char( "�O", ch );

		if( IS_SET( to_room->room_flags, ROOM_PK) )
		{
			send_to_char( "���޳�", ch );
			room_flags += 1;
		}
	}
	if( sklv >= 1 )
	{
		if ( room_flags > 0 ) send_to_char( "�A", ch );

		if( IS_SET( to_room->room_flags, ROOM_INDOORS) )
		{
			send_to_char( "���Ǥ�", ch );
			room_flags += 1;
		}
		if( IS_SET( to_room->room_flags, ROOM_DARK) )
		{
			send_to_char( "�|�P�۷�·t", ch );
			room_flags += 1;
		}

		if( IS_SET( to_room->room_flags, ROOM_SAFE) )
		{
			if ( room_flags > 0 )
				send_to_char( "�A���L�ݰ_�ӫܦw��", ch );
			else
				send_to_char( "�ݰ_�ӫܦw��", ch );
			room_flags += 1;
		}
		if( sklv >= 4 )
		{
			if( IS_SET( to_room->room_flags, ROOM_CAMP) ){
				if ( room_flags > 0 )
					send_to_char( "�A�åB�a���y«���]�O", ch );
				else
					send_to_char( "�a���y«���]�O", ch );
				room_flags += 1;
			}
		}
	}
	if( room_flags > 0 ){
		send_to_char( "�C\n\r", ch );
	}else{
		send_to_char( "�S����S�O���C\n\r", ch );
	}

	// �[�J sklv �P�w 2022/12/22
	if( sklv > 5 )
	{
		if( IS_SET( to_room->room_flags, ROOM_CAMP) )
		if ( to_room->roomprogs )
			send_to_char( "���~�A�A�ӾU���[��O�o�{�o�̦��G�����򤣹�l�C\n\n\r", ch );
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
            return HIW"�ѤH�X�@"NOR;
            break;
        case 5:
            return HIC"�`���i��"NOR;
            break;
        case 4:
            return HIY"�W�Z�J�t"NOR;
            break;
        case 3: 
            return HIG"���G���"NOR;
            break;
        case 2:
            return HIR"�o������"NOR;
            break;
        case 1:
            return HIP"�����@�G"NOR;
            break;
        case 0:
            return HIU"�s�ǥE�m"NOR;
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
		send_to_char("�A������¾��~�ണ�ɧޯ൥�šC\n\r", ch);
		return;
	}
	argument = one_argument(argument, arg);
	argument = one_argument(argument, arg2);

	if( arg[0] == '\0' ) {
		send_to_char("�A�n���ɤ���ޯ�H\n\r", ch);
		return;
	}

	sn = skill_lookup(arg);

	if( sn < 0 ) {
		send_to_char("�S���o�ӧޯ�C\n\r", ch);
		return;
	}

	if( get_skill_percent(ch,sn) < 1 ) {
		send_to_char("�A�ä��|�o���ޯ�C\n\r", ch);
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
				send_to_char("�A����b�ܱڪ����p���ɧA���ѽ�C\n\r", ch);
				return;
			}
		}
	}


	// �w��S�� room �����p, ����i�H�[�J��L room_flag �@ room skadv ���P�w
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
			send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
			return;
		}

		if( !mob->psk ) {
			send_to_char( "[33m�A�䤣��ɮv�C[m\n\r", ch );
			return;
		}

		switch( is_skname( arg, mob->psk ) )
		{
			case -1 :
				act( "$n ���G�藍�_�A�ڨä��|�����ޥ��C", mob, NULL, ch, TO_VICT );
				return;
			case 1 :
				if ( !is_qmark(ch, skill_table[sn].name) )
				{
					act( "$n ���G�藍�_�A�A�٤������ǲ߳o���ޥ��C", mob, NULL, ch, TO_VICT );
					return;
				}
				break;
			default :
				break;
		}
	}

	/* �N sn �P�w����̫e��, �O is_skname ������ͧ@�� 2023/02/20

		if( arg[0] == '\0' ) {
			send_to_char("�A�n���ɤ���ޯ�H\n\r", ch);
			return;
		}
		sn = skill_lookup(arg);
		if( sn < 0 ) {
			send_to_char("�S���o�ӧޯ�C\n\r", ch);
			return;
		}
		if(get_skill_percent(ch,sn) < 1 ) {
			send_to_char("�A�ä��|�o���ޯ�C\n\r", ch);
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
					send_to_char("�A����b�ܱڪ����p���ɧA���ѽ�C\n\r", ch);
					return;
				}
			}
		}
	*/


	if( (cost = skill_difficulty(ch,sn)) < 0 ) {
		send_to_char("�A�b�o���ޯ�W�w�g�L�k�A�i�B�F�C\n\r", ch);
		return;
	}
	if( get_eff_skill_exp(ch) < cost ) {
		send_to_char("�A���i�Χޯ�g��Ȥ����F�C\n\r", ch);
		return;
	}

	sprintf(buf, "�A�Υh�F %d �I�ޯ�g��ȡC\n\r", skill_difficulty(ch,sn));
	send_to_char(buf,ch);

	act( "[36m�A���ɤF$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
	act( "[36m$n���ɤF$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );

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
		send_to_char("�A������¾��~�୰�C�ޯ൥�šC\n\r", ch);
		return;
	}
	one_argument(argument, arg);

	if( arg[0] == '\0' ) {
		send_to_char("�A�n���C����ޯ�H\n\r", ch);
		return;
	}
	sn = skill_lookup(arg);
	if( sn < 0 ) {
		send_to_char("�S���o�ӧޯ�C\n\r", ch);
		return;
	}
	if(get_skill_percent(ch,sn) < 1 ) {
		send_to_char("�A�ä��|�o���ޯ�C\n\r", ch);
		return;
	}

	if( get_skill_level(ch,sn) < 1 ) {
		send_to_char("�A�b�o���ޯ�W�w�g�L�k�A���C�F�C\n\r", ch);
		return;
	}

	if( sn == gsn_inborn_ability){
		AFFECT_DATA *paf = NULL;
		for ( paf = ch->affected; paf; paf = paf->next ){
			if ( paf->deleted ) continue;
			if ( paf->type == gsn_polymorph){
				send_to_char("�A����b�ܱڪ����p���C�A���ѽ�C\n\r", ch);
				return;
			}
		}
	}

	act( "[36m�A���C�F$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_CHAR );
	act( "[36m$n���C�F$t($T)�C[m", ch, skill_table[sn].cname, skill_table[sn].name, TO_ROOM );

	gain_skill_level(ch,sn,-1);
	regain_skill_exp( ch, skill_difficulty(ch,sn)  );

	if (IsUnderFistFight(ch)) {
		SET_BIT( ch->act, PLR_FIST_FIGHT );
	} else {
		REMOVE_BIT( ch->act, PLR_FIST_FIGHT );
	}
}
