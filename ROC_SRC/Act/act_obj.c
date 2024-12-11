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
//*  PROGRAM ID  : act_obj.c                                      *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940614 jye                                    *
//*  PURPOSE     :         ­×§ïeqbank open¦p¤wbank->open®É        *
//*              :             ÀË¬d½u¤Wppl¬O§_¦³¤H¨Ï¥Î            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940619 jye                                    *
//*  PURPOSE     :         ­×§ïeqbank list¥i´M§ì¬ÛÃö¦r¤§eq name   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940825 Razgriz                                *
//*  PURPOSE     :         §â fwrite_neweqbank ¸ò free_neweqbank  *
//*              :         «Å§i¦bÀÉ®×¶}ÀY¥H«KÅý close_socket ¨Ï¥Î *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 jye                                    *
//*  PURPOSE     :         ­×§ïdo_give ®ÉimmµLµøITEM_PERSONAL     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Jye                                    *
//*  PURPOSE     :         ­×§ï§PÂ_souvenir­È¨Ï§ï¥ÎÅÜ¼Æ           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940926 Razgriz                                *
//*  PURPOSE     :         ­×§ï do_steal ¤£¯à°½¬ï¦b¨­¤WªºªF¦è     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941003 Jye                                    *
//*  PURPOSE     :         ­×§ïdo_brandish ±Æ°£imm char           *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"


/*
 * Local functions.
 */
#define CD CHAR_DATA
void get_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container ) );
void wear_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace, int iWear ) );
CD * find_keeper args( ( CHAR_DATA *ch, char *argument ) );
int get_cost args( ( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
#undef  CD
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing );

/*
 * Global functions and variables.  -- Razgriz
 */
//declaration for close_socket()	Added by Razgriz 20050825
void fwrite_neweqbank	args( ( CHAR_DATA * ch, FILE* fp ) );
void free_neweqbank	args( ( BANK_DATA * bank ) );

extern  int top_bank;
extern bool IsSorGateMob(int nMobIndex);

void free_neweqbank_obj( OBJ_DATA *obj )
{
    AFFECT_DATA    *paf;
    AFFECT_DATA    *paf_next;
    EXTRA_DESCR_DATA *ed;
    EXTRA_DESCR_DATA *ed_next;
    OBJ_HP *ohp, *ohp_next;

    for ( ed = obj->extra_descr; ed; ed = ed_next )
    {
  ed_next = ed->next;

  free_string( ed->description );
  free_string( ed->keyword     );
  ed->next     = extra_descr_free;
  extra_descr_free = ed;
    }
    for( ohp = obj->hp ; ohp; ohp = ohp_next ) {
  ohp_next = ohp->next;
  free_string(ohp->hp_name);
  ohp->next = obj_hp_free;
  obj_hp_free = ohp;
    }

    for ( paf = obj->affected; paf; paf = paf_next )
    {
  paf_next = paf->next;

  if ( obj->affected == paf )
  {
      obj->affected = paf->next;
  }
  else
  {
      AFFECT_DATA *prev;

      for ( prev = obj->affected; prev; prev = prev->next )
      {
    if ( prev->next == paf )
    {
        prev->next = paf->next;
        break;
    }
      }

      if ( !prev )
      {
    bug( "[void free_neweqbank_obj]List_update: cannot find paf on obj.", 0 );
    continue;
      }
  }

  paf->next   = affect_free;
  affect_free = paf;
    }


    free_string( obj->name      );
    free_string( obj->description );
    free_string( obj->short_descr );
    /*        free_string( obj->cname     );*/
    --obj->pIndexData->count;

    obj->next = obj_free;
    obj_free  = obj;
}

// §¹¦¨ eqb_do_examine ªº«ü¥O, ¨Ã²¾°£ eqb_do_look ±N¨âªÌ²Î¦X¬°³æ¤@«ü¥O 2022/12/01
void eqb_do_examine( CHAR_DATA *ch, OBJ_DATA *obj )
{
	char buf[ MAX_STRING_LENGTH ];
	int diff;

	if ( !obj )
	{
		send_to_char( "[33m­nÀË¬d¤°»ò¡H[m\n\r", ch );
		return;
	}

	send_to_char( obj->short_descr , ch);
	send_to_char( " - ", ch);
	send_to_char( obj->material->short_descr, ch);
	send_to_char("\n\r", ch);
	send_to_char( obj->description, ch );
	send_to_char( "\n\r", ch );
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
			else if( obj->value[3] == 16 )
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
			if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ){
				bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
				obj->value[2] = 0;
			}
			if ( obj->value[1] <= 0 && obj->value[1] != -1 ){
				send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
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
			break;

		case ITEM_FOUNTAIN:
			if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ){
				bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
				obj->value[2] = 0;
			}
			if ( obj->value[1] <= 0 && obj->value[1] != -1 ){
				send_to_char( "[36m¨º¬O°®ªº¡C[m\n\r", ch );
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
			break;

		case ITEM_CONTAINER:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
			if ( IS_SET( obj->value[1], CONT_CLOSED ) ){
				send_to_char( "[33m¥¦¬OÃöµÛªº¡C[m\n\r", ch );
				break;
			}
			act( "$p¸Ì­±¦³¡G", ch, obj, NULL, TO_CHAR );
			show_list_to_char( obj->contains, ch, TRUE, TRUE );
			break;

		case ITEM_ARROW:
			if ( obj->value[0] <= 0 ){
				send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
				break;
			}
			sprintf( buf, "¨º¸Ì­±ÁÙ¦³%d¤ä½b¡C\n\r", obj->value[0] );
			send_to_char( buf, ch );
			break;

		case ITEM_AMMO:
			if ( obj->value[0] <= 0 ){
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
	}

	return;
}

/*  ­«·s½s¼g eqb_do_examine ¨Ã²¾°£ eqb_do_look 2022/12/01
void eqb_do_examine( CHAR_DATA *ch, OBJ_DATA *obj )
{
	int diff;

	if ( !obj )
	{
		send_to_char( "[33m­nÀË¬d¤°»ò¡H[m\n\r", ch );
		return;
	}


	switch ( obj->item_type )
	{
		default:
			break;

		case ITEM_ARMOR:
			eqb_do_look( ch, obj );
			if( obj->value[1]  != 0)
				send_to_char("¬Ý¨Ó¦n¹³¨ü¨ì»G»k¤F¡C\n\r", ch);
			break;

		case ITEM_WEAPON:
			eqb_do_look( ch, obj );
			send_to_char("§A¥J²Ó¥´¶q¤§«á¡Aµo²{¥¦", ch);
			if( indestructible(obj) ) {
				send_to_char(HIW"¥¿³B¦b§¹¬üªºª¬ªp¤U¡C"NOR"\n\r", ch);
				break;
			}
			else if ( get_max_dura(obj) == 0 ) {
				send_to_char(RED"¦n¹³¤@¥¹¥ÎÃa´NµLªk­×´_¡C"NOR"\n\r", ch);
				break;
			}
			diff = (get_curr_dura(obj) * 100) / get_max_dura(obj);

			if( diff == 100 ) send_to_char(HIW"¥¿³B¦b§¹¬üªºª¬ªp¤U¡C"NOR"\n\r", ch);
			else if( diff > 90) send_to_char(HIC"¬Ý°_¨Óª¬ªp¦nªº«Ü¡C"NOR"\n\r", ch);
			else if( diff > 70) send_to_char(HIY"µy·L¦³¨Ç¨í²ª¡C"NOR"\n\r", ch);
			else if( diff > 40) send_to_char(YEL "ª¬ªp¤£¤Ó¦n¡C"NOR"\n\r", ch);
			else if( diff > 10) send_to_char(HIR"®t¤£¦h§ÖÃa¤F¡C"NOR"\n\r", ch);
			else if( diff > 0 ) send_to_char(RED"¦n¹³»´»´¤@¸I´N·|¸ÑÅé¡C"NOR"\n\r", ch);
			else send_to_char(HIB"§¹¥þ·lÃa¡A¤£³ô¨Ï¥Î¡C"NOR"\n\r", ch);
			break;

		case ITEM_DRINK_CON:
		case ITEM_CONTAINER:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
		eqb_do_look( ch, obj );
	}

    return;
}

void eqb_do_look( CHAR_DATA *ch, OBJ_DATA *obj )
{
	char buf  [ MAX_STRING_LENGTH ];


	if ( !( obj  ) )
	{
		send_to_char( "[33m§A¦b³o¸Ì¨S¬Ý¨ì¨ºª±·N¨à¡C[m\n\r", ch );
		return;
	}


	send_to_char( obj->short_descr , ch);
	send_to_char( " - ", ch);
	send_to_char( obj->material->short_descr, ch);
	send_to_char("\n\r", ch);
	send_to_char( obj->description, ch );
	send_to_char( "\n\r", ch );
	switch ( obj->item_type )
	{
		default:
			break;

		case ITEM_DRINK_CON:
			send_to_char( "§A©¹¸Ì­±¬Ýªº®É­Ô¡A§A¬Ý¨ì¡G\n\r", ch );
			if ( obj->value[1] <= 0 )
			{
				send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
				break;
			}

			sprintf( buf, "¨º¸Ì­±¦³%s%s²GÅé¡C\n\r",
			obj->value[1] <     obj->value[0] / 4
			? "¤£¨ì¤@¥bªº" :
			obj->value[1] < 3 * obj->value[0] / 4
			? "¤j¬ù¤@¥bªº"     : "¶W¹L¤@¥bªº",
			liq_table[obj->value[2]].liq_color
			);

			send_to_char( buf, ch );
			break;

		case ITEM_CONTAINER:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
			if ( IS_SET( obj->value[1], CONT_CLOSED ) )
			{
				send_to_char( "[33m¥¦¬OÃöµÛªº¡C[m\n\r", ch );
				break;
			}

			send_to_char( "§A©¹¸Ì­±¬Ýªº®É­Ô¡A§A¬Ý¨ì¡G\n\r", ch );
			act( "$p¸Ì­±¦³¡G", ch, obj, NULL, TO_CHAR );
			show_list_to_char( obj->contains, ch, TRUE, TRUE );
			break;

		case ITEM_ARROW:
			send_to_char( "§A©¹¸Ì­±¬Ýªº®É­Ô¡A§A¬Ý¨ì¡G\n\r", ch );
			if ( obj->value[0] <= 0 )
			{
				send_to_char( "[36m¨º¬OªÅªº¡C[m\n\r", ch );
				break;
			}

			sprintf( buf, "¨º¸Ì­±ÁÙ¦³%d¤ä½b¡C\n\r", obj->value[0] );

			send_to_char( buf, ch );
			break;
	}
	return;
} */

void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
	if ( !CAN_WEAR( obj, ITEM_TAKE ) && obj->item_type != ITEM_CORPSE_PC)
	{
		send_to_char( "§AµLªk®³°_.\n\r", ch );
		return;
	}
	else if ( obj->item_type == ITEM_CORPSE_PC ) {
			if( !consented(ch, obj)){
				send_to_char( "§AµLªk®³°_.\n\r", ch );
				return;
			}
			/*  if( obj->in_room) //add by coconet for consent
			  {
				obj->area = obj->in_room->area;
				bug( "test consent: obj %d " ,obj->area->name);
			  } 
			  else
				return;*/
	}
	if( IS_SET(obj->extra_flags, ITEM_UNIQUE) && IS_NPC(ch)){
		send_to_char("§AµLªk®³°_.\n\r", ch);
		return;
	}

	if ( !( obj->item_type == ITEM_MONEY ) && ( !container || container->carried_by != ch ))
	{
		if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
		{
			act( "$d: §AµLªk®³¨º»ò¦hªF¦è.", ch, NULL, obj->name, TO_CHAR );
			return;
		}

		if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
		{
			act( "$d: §A®³¤£°Ê¤F.", ch, NULL, obj->name, TO_CHAR );
			return;
		}
	}

	// §â±Ô­z»Pª««~²¾°Ê¤À¶} , ¦] obj_act ¥i¯à¥]§t purge 2022/02/12
	if ( container )
	{
		//modify by coco 0511 for horse
		if( container->item_type == ITEM_HORSE )
		{
			act( "§A±q $P ¨ø¸ü¤U $p.", ch, obj, container, TO_CHAR );
			act( "$n ±q $P ¨ø¸ü¤U $p.", ch, obj, container, TO_ROOM );
			//obj_from_obj( obj );
		}
		else 
		{
			act( "§A±q $P ®³¥X $p.", ch, obj, container, TO_CHAR );
			act( "$n ±q $P ®³¥X $p.", ch, obj, container, TO_ROOM );
			//obj_from_obj( obj );
		}
	}
	else
	{
		act( "§A®³°_ $p.", ch, obj, container, TO_CHAR );
		act( "$n ®³°_ $p.", ch, obj, container, TO_ROOM );
		//obj_from_room( obj );
	}

	if ( container ){
		obj_from_obj( obj );
	}else{
		obj_from_room( obj ); //remove obj from in_room->contents list
	}

	// holy evil ¿N¶Ë§P©w
	if ( IS_OBJ_STAT( obj, ITEM_HOLY  ) && IS_EVIL(ch))
	{
		act( "§A³Q $p ¤Wªº¯«¸t¤O¶qµ¹¨`¶Ë¤F¡I", ch, obj, NULL, TO_CHAR );
		act( "$n ³Q $p ¤Wªº¯«¸t¤O¶qµ¹¨`¶Ë¤F¡I", ch, obj, NULL, TO_ROOM );
		damage( ch, ch, 50, gsn_burning_hands, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}
	else if ( IS_OBJ_STAT( obj, ITEM_EVIL) && IS_GOOD(ch))
	{
		act( "§A³Q $p ¤Wªº¨¸´c¤O¶qµ¹¨`¶Ë¤F¡I", ch, obj, NULL, TO_CHAR );
		act( "$n ³Q $p ¤Wªº¨¸´c¤O¶qµ¹¨`¶Ë¤F¡I", ch, obj, NULL, TO_ROOM );
		damage( ch, ch, 50, gsn_burning_hands, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}

	// obj type ¬° item_moeny ®Éªº§P©w
	if ( obj->item_type == ITEM_MONEY )
	{
		char buf [ MAX_STRING_LENGTH ];
		int  amount;

		amount = obj->value[0];
		ch->gold += amount;

		if ( amount > 1 )
		{
			sprintf( buf, "§A®³¥X¤F %d ªTª÷¹ô.\n\r", amount );
			send_to_char( buf, ch );
		}
		// add at 2022/02/13
		obj_to_char( obj, ch );
		// obj_act get 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("get", ch, obj);
		}
		if( obj->deleted )
			;
		else
			extract_obj( obj );/*remove money obj from room/pc/container...*/
	}
	else
	{
		obj_to_char( obj, ch );/*add obj to char->carrying list*/
	}

	// pc corpse
	if( container && container->item_type == ITEM_CORPSE_PC && !container->contains )
	{
		act( "$p ®ø¥¢¤F.", ch, container, container, TO_CHAR );
		act( "$p ®ø¥¢¤F.", ch, container, container, TO_ROOM );
		extract_obj(container);
		save_char_obj(ch);
		//container->timer /= 3;
	}

	// obj_act get 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("get", ch, obj);
	}

	return;
}


int do_autoget( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *container;
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	bool found;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	/* Get type. */
	if ( arg1[0] == '\0' )
	{
		send_to_char( "­n®³¤°»ò?\n\r", ch );
		return 0;
	}

	if ( arg2[0] == '\0' )
	{
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj' */
			obj = get_obj_list( ch, arg1, ch->in_room->contents );
			if ( !obj )
			{
				act( "³o¸Ì¨S¦³ $T.", ch, NULL, arg1, TO_CHAR );
				return 0;
			}

			get_obj( ch, obj, NULL );
		}
		else
		{
			/* 'get all' or 'get all.obj' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = ch->in_room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, NULL );
				}
			}

			if ( !found ) 
			{
				if ( arg1[3] == '\0' )
					send_to_char( "³o¸Ì¨S¦³¥ô¦óª««~.\n\r", ch );
				else
					act( "³o¸Ì¨S¦³ $T.", ch, NULL, &arg1[4], TO_CHAR );
			}
		}
	}
	else
	{
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		{
			send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
			return 0;
		}

		if ( !( container = get_obj_here( ch, arg2 ) ) )
		{
			act( "³o¸Ì¨S¦³ $T.", ch, NULL, arg2, TO_CHAR );
			return 0;
		}

		switch ( container->item_type )
		{
			default:
				send_to_char( "¨º¤£¬O­Ó®e¾¹.\n\r", ch );
				return 0;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
				break ;

			case ITEM_CORPSE_PC:
			{
				CHAR_DATA *gch;
				char  *pd, *cd;
				char   name [ MAX_INPUT_LENGTH ];
				char   chname [ MAX_INPUT_LENGTH ];

				if ( IS_NPC( ch ) )
				{
					send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
					return 0;
				}

				pd = container->short_descr;/*short_descr;
				pd = one_argument( pd, name );
				pd = one_argument( pd, name );*/
				cd = ch->name;
				cd = one_argument( cd, chname );

				if ( str_cmp( name, chname ) && !IS_IMMORTAL( ch ) )
				{
					bool fGroup;

					fGroup = FALSE;
					for ( gch = char_list; gch; gch = gch->next )
					{
						if ( !IS_NPC( gch ) && is_same_group( ch, gch ) && !str_cmp( name, gch->name ) )
						{
							fGroup = TRUE;
							break;
						}
					}
					if ( !fGroup )
					{
						send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
						return 0;
					}
				}
			}
		}

		if ( container->item_type == ITEM_CONTAINER && IS_SET( container->value[1], CONT_CLOSED ) )
		{
			act( "$d ¬OÃöµÛªº.", ch, NULL, container->name, TO_CHAR );
			return 0;
		}

		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			int value;
			/* 'get obj container' */
			obj = get_obj_list( ch, arg1, container->contains );
			if ( !obj )
			{
				// auto get corpse ¨SªF¦è¤£·|Åã¥Ü¦r¦ê 2022/05/08
				//act( "$T ¸Ì¨S¦³³o¼ËªF¦è.", ch, NULL, arg2, TO_CHAR );
				return 0;
			}
			value = obj->value[0];
			get_obj( ch, obj, container );
			return value;
		}
		else
		{
			/* 'get all container' or 'get all.obj container' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = container->contains; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, container );
				}
			}
			/*  auto get corpse ¨SªF¦è¤£·|Åã¥Ü¦r¦ê 2022/05/08
			if ( !found )
			{
				if ( arg1[3] == '\0' )
					act( "$T ¸Ì¨S¦³³o¼ËªF¦è.", ch, NULL, arg2, TO_CHAR );
				else
					act( "$T ¸Ì¨S¦³³o¼ËªF¦è.", ch, NULL, arg2, TO_CHAR );
			} */
		}
	}

	return 0;
}

BANK_DATA * new_bank(){
    BANK_DATA * bank = NULL;

    if( !eqbank_free ) {
        bank = alloc_mem( sizeof( *bank));

        ++top_bank;
    }
    else {
        bank = eqbank_free;
        eqbank_free = bank->next;
    }
    bank->name = NULL;
    bank->next = NULL;
    bank->password = NULL;
    bank->objs = NULL;
    bank->amount = 0;
    bank->opened = TRUE;
    return bank;
}

void fwrite_neweqbank( CHAR_DATA * ch, FILE* fp )
{
    
    if( !ch->neweqbank ) return;



    fprintf(fp,"#EQBANK_DATA\n");
    fprintf(fp, "%s~\n", ch->neweqbank->name);
    fprintf(fp, "%s~\n", ch->neweqbank->password);
    fprintf(fp, "%d\t", ch->neweqbank->amount);
    fprintf(fp, "%d\n", ch->neweqbank->opened);
    if( ch->neweqbank->objs )
  fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
    fprintf(fp,"#END\n");
    return;
}

int fread_neweqbank( BANK_DATA * ch, FILE* fp){

    char letter;
    char *word;
    int status;
    for ( ;; ) {
  letter = fread_letter(fp);
  if( letter != '#' ) {
      bug("[read_neweqbank] : # not found", 0);
      return FALSE;
  }
  word = fread_word(fp, &status);

  if( !str_cmp(word, "EQBANK_DATA")){
      ch->name = fread_string(fp, &status);
      ch->password = fread_string(fp, &status);
      ch->amount = fread_number(fp, &status);
      ch->opened = fread_number(fp, &status);
      continue;
  }
  else if( !str_cmp(word, "EQBANK_OBJECT")){

      if( !fread_newbank_obj( ch, fp )) return FALSE;
      continue;
  }
  else if( !str_cmp(word, "END")){
      break;
  }
  else
  {
      bug("[read_neweqbank]: unknown section name.",0);
      return FALSE;
  }
  
    }

    return TRUE;
}

bool check_parse_name( char *name );
void free_neweqbank( BANK_DATA * bank )
{
    OBJ_DATA * obj, *nobj;
    free_string(bank->name);
    free_string(bank->password);
    for( obj = bank->objs ; obj ; obj = nobj ){
        nobj = obj->next_content;
        obj->deleted = TRUE;
        obj->next = object_list;
        object_list = obj;
        obj->next_content = NULL;
    }
    /*
    for( obj = bank->objs ; obj; obj = nobj){
  if( obj->contains ) {
      OBJ_DATA * nnobj;
      for(nobj = obj->contains; nobj; nobj = nnobj ){
    nnobj = nobj->next_content;
    nobj->deleted = TRUE;
    nobj->next_content = NULL;
    free_neweqbank_obj( nobj );
      }
  }
  nobj = obj->next_content;
  obj->deleted = TRUE;
  obj->next_content = NULL;
  free_neweqbank_obj( obj );
    }*/
    bank->name = NULL;
    bank->password = NULL;
    bank->objs = NULL;
    bank->next = eqbank_free;
    eqbank_free = bank;
    return;
}

bool check_container( CHAR_DATA *ch, OBJ_DATA *container)
{
	OBJ_DATA *obj;
	int O_Weight = 0; // 2022/12/01
	int O_Number = 0; // 2022/12/01


	for( obj = container->contains; obj ; obj = obj->next_content)
	{
		// ¤º®eª«ªº container ¥]¦³ container ±NµLªk«O¦s 2022/12/01
		if( obj->item_type == ITEM_CONTAINER && !( obj->contains == NULL ) )
		{
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A§Ú­Ì¤£¦¬®e¾¹¤ºªº®e¾¹¸Ì­±ÁÙ©ñµÛª««~ªº®e¾¹¡C¡v\n\r", ch );
			return FALSE;
		}
		// container ªº¤º®eª«¥i¥H«O¦s key ¤Î¦³ timer ªº eq , ¥[¤JµLªk¦s¤J horse 2022/12/01
		if ( ( ch->level < ( obj->level - 5 ) && ch->class < 5 )
			//|| obj->item_type == ITEM_KEY
			|| obj->item_type == ITEM_HORSE
			|| IS_SET( obj->extra_flags, ITEM_NOSAVE )
			|| IS_SET( obj->extra_flags, ITEM_PERSONAL )
			|| IS_SET( obj->extra_flags, ITEM_UNIQUE )
			|| obj->deleted )
			//|| obj->timer > 0 )
		{
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A³o®e¾¹¤º©ñµÛ§Ú­Ì¤£¦¬ªºª««~¡C¡v\n\r", ch );
			return FALSE;
		}
		if( obj->weight > 0 )
			O_Weight += obj->weight;
		++O_Number;
	}

	// Á×§KµLªk¨ú¥X, ¼W¥[Á`­«¶q¤Îª««~¼Æ­pºâ 2022/12/01
	if( container->value[3] > 100 )
		O_Weight = (O_Weight * container->value[3]) / 100;
	else if( container->value[3] == 100 )
		O_Weight = O_Weight - 1;
	else if( container->value[3] > 0 )
		O_Weight = ( O_Weight * ( 100 - container->value[3] ) ) / 100;
	else if( container->value[3] < 0 )
		O_Number = O_Number + container->value[3];

	if( O_Weight > 800 ){
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A³o¥ó®e¾¹ªºÁ`­«¶q¶W¹L 800 ¤½¤ç¡C¡v\n\r", ch );
		return FALSE;
	}

	if( O_Number > 90 ){
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A³o¥ó®e¾¹ªºª««~¼Æ¶q¶W¹L 90 ¥ó¡C¡v\n\r", ch );
		return FALSE;
	}

	return TRUE;
}

bool check_eqbank( BANK_DATA * bank )
{
	DESCRIPTOR_DATA *d;
	CHAR_DATA *to;

	for( d = descriptor_list; d; d = d->next ) 
	{
		to = (d->original) ? d->original : d->character;
		if( !to ) continue;
		if( to->neweqbank )
		{
			if( !str_cmp( to->neweqbank->name , bank->name ))
				return TRUE;
		}
	}

	return FALSE;
}
void do_eqbank( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	char out_buf [ MAX_STRING_LENGTH * 12 ];
	char tmp[255];
	char no_ansi[255];
	CHAR_DATA * vch = NULL;
	BANK_DATA * bank = NULL;
	OBJ_DATA * obj=NULL;
	OBJ_DATA * vobj=NULL;
	bool fBank = TRUE;
	int count = 0;
	FILE * fp = NULL;
	int i = 0;
	int cost = 0;

	argument = one_argument( argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);

	if(IS_NPC(ch)) return;

	if( !ch->in_room || !ch->in_room->area ) return;

	if( IS_SET(ch->act, PLR_HORSE)) return;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
	{
		// ¥[¤J npc ­­©w, ¨Ã­×§ï eqbanker ªº§P©w¥Ñ name ÅÜ§ó¬° qmark 2022/12/01
		if( !IS_NPC(vch) ) continue;
		//if ( is_name( "sharas-eqb", vch->name ) )
		if( is_qmark(vch, "is_eqbanker") )
			break;
	}

	if ( !vch ){
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¤£¦b³o¨à¡C\n\r", ch );
		return;
	}

	if( arg1[0] == '\0' ){
		send_to_char("½Ð help eqbank ¥H¨ú±o¥Îªk»¡©ú¡C\n\r", ch);
		return;
	}

	if( !str_cmp(arg1, "passwd" ))
	{
		if( !ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u±z±o¥ý¥´¶}­Ü®w¡C¡v\n\r", ch );
			return;
		}

		if( str_cmp(arg2, arg3) ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u±z¨â¦¸ªº±K½X¤£¤@¼Ë¡C¡v\n\r", ch );
			return;
		}

		free_string( ch->neweqbank->password );
		ch->neweqbank->password = str_dup( crypt(arg3,ch->neweqbank->name));
		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));

		if(( fp = fopen(tmp, "w"))){
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}

		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u±K½X³]©w§¹¦¨¡C¡v\n\r", ch );
		return;
	}

	if( !str_cmp( arg1, "new"))
	{
		if( ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u±z±o¥ý§â¨Ï¥Î¤¤ªº­Ü®wµ¹Ãö³¬(close)¡C¡v\n\r", ch );
			return;
		}

		if( strlen( arg1 ) > 20 ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u±z·s­Ü®wªº¦W¦r¤Óªø¤F¡C¡v\n\r", ch );
			return;
		}

		if( !check_parse_name(arg2)){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u¨º¬O¤£¦Xªkªº¦W¦r¡C¡v\n\r", ch );
			return;
		}

		if( arg2[0] == '\0' || arg3[0] == '\0'){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u±z±o§i¶D§Ú·s­Ü®wªº¦W¦r©M±K½X°Ú¡C¡v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2 ), capitalize(arg2 ));

		if( ( fp = fopen( tmp, "r" ))){
			fclose(fp);
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u¯u©êºp¡A³o­Ó¦W¦r¤w¸g¦³¤H¥Î¤F¡C¡v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s.gz", PLAYER_DIR,initial( arg2 ), capitalize(arg2 ));

		if( ( fp = fopen( tmp, "r")))
		{
			fclose(fp);
			if( str_cmp( arg2, ch->name )){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u©êºp¡A½Ð¤£­n¥Î¨ä¥Lª±®aªº¦W¦r¡C¡v\n\r", ch );
				return;
			}
		}

		sprintf(tmp, "%s%s/%s", PLAYER_DIR,initial( arg2 ), capitalize(arg2 ));

		if( ( fp = fopen( tmp, "r")))
		{
			fclose(fp);
			if( str_cmp( arg2, ch->name )){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u©êºp¡A½Ð¤£­n¥Î¨ä¥Lª±®aªº¦W¦r¡C¡v\n\r", ch );
				return;
			}
		}

		cost = 500000;
		fBank = TRUE;

		if( ch->bank < cost ){
			fBank = FALSE;
			if( ch->gold < cost){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u¯u©êºp¡A§Aªº¿ú¤£°÷¡C¡v\n\r", ch );
				return;
			}
			ch->gold -= cost;
		}else{
			if( ch->bank < cost){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u¯u©êºp¡A§Aªº¿ú¤£°÷¡C¡v\n\r", ch );
				return;
			}
			ch->bank -= cost;
		}

		bank = new_bank();
		bank->name = str_dup(arg2);
		bank->password = str_dup( crypt(arg3,arg2));
		bank->opened = TRUE;
		ch->neweqbank = bank;

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));

		if(( fp = fopen(tmp, "w"))){
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}

		sprintf(tmp, "%s opened new storage '%s'", ch->name, arg2);
		bug(tmp, 0);
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u¶}¤á§¹¦¨¡C¡v\n\r", ch );
		if( fBank )
			send_to_char("§A±q»È¦æÂà±b¡A¥I¥X¤F "HIY, ch);
		else
			send_to_char("§A¥I¥X¤F "HIY, ch);
		sprintf(tmp, "%d", cost);
		send_to_char(tmp, ch);
		send_to_char(NOR " ¤¸ªº¶}¤á¶O¡C\n\r", ch);
		return;
	}

	if( !str_cmp(arg1, "backup"))
	{
		if( !ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u³Â·Ð½Ð¥ý¥´¶}­Ó­Ü®w¡C¡v\n\r", ch );
			return;
		}
		sprintf(tmp, "%s%s/%s", EQBANK_BACKUPDIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
		ch->neweqbank->opened = FALSE;
		if(( fp = fopen(tmp, "w"))){
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u­Ü®w³Æ¥÷§¹¦¨¡C¡v\n\r", ch );
		}else{
			bug("eqbank: file open error!!",0);
			send_to_char("µo¥ÍÄY­«°ÝÃD¡A½Ð¦V¤j¯«¦^³ø¡C\n\r", ch);
			return;
		}
		ch->neweqbank->opened = TRUE;
		return;
	}

	if( !str_prefix(arg1, "store"))
	{
		if( !ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z±o¥ý¥´¶}­Ó­Ü®w¡C¡v\n\r", ch );
			return;
		}
		if( arg2[0] == '\0' ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z­n¦s©ñ¤°»ò¸Ë³Æ¡H¡v\n\r", ch );
			return;
		}

		if ( ( obj = get_obj_carry( ch, arg2 ) ) )
		{
			cost = ( obj->level < 5 ) ? 8000 : ( obj->level < 36 ) ? obj->level * 200 : obj->level * 500;

			if( obj->item_type == ITEM_CONTAINER )
			{
				cost = (10000 * (get_obj_number(obj) + 1 ));
			}

			if( obj->timer > 0) cost *= 10;

			if ( ch->bank < cost )
			{
				fBank = FALSE;
				if( ch->gold < cost ){
					send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó­W¯ºµÛ»¡¡G¡u¹ï¤£°_¡A¨S¦³¨¬°÷ªº«OºÞ¶O§ÚµLªk¶±¥Î«O¥þ¤H­û¡C¡v\n\r", ch );
					return;
				}
			}

			if (ch->neweqbank->amount > 150){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó­W¯ºµÛ»¡¡G¡u¹ï¤£°_¡A±z®w¦sªº¸Ë³Æ¼Æ¶q¤w¹F 150 ¥óªº¤W­­¡C¡v\n\r", ch );
				return;
			}

			// ³æ¤@ª««~­«¶q¤W­­ 2022/12/01
			if( obj->weight > 800 ){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A¨C¥ó¸Ë³Æªº­«¶q¤£¯à¶W¹L 800 ¤½¤ç¡C¡v\n\r", ch );
				return;
			}

			// ²¾°£ item_key ¤£¯à¦s, ¥[¤Jitem_horse ¤£¯à¦s 2022/12/01
			if ( ( ch->level < ( obj->level - 5 ) && ch->class < 5 )
				//|| obj->item_type == ITEM_KEY
				|| obj->item_type == ITEM_HORSE
				|| IS_SET( obj->extra_flags, ITEM_NOSAVE )
				|| IS_SET( obj->extra_flags, ITEM_PERSONAL)
				|| IS_SET( obj->extra_flags, ITEM_UNIQUE)
				|| obj->deleted )
				//|| obj->timer > 0 )
			{
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A³oÃþ¸Ë³Æ§Ú­Ì¤£¦¬¡C¡v\n\r", ch );
				return;
			}

			if( obj->item_type == ITEM_CONTAINER && !check_container(ch, obj))
				return;

			obj_from_char( obj );
			obj->value[4] = ch->in_room->area->recall;
			if( obj->item_type == ITEM_CONTAINER )
			{
				OBJ_DATA * vobj_next = NULL;
				for( vobj = object_list ; vobj ; vobj = vobj_next )
				{
					if( !object_list->next || !vobj->next ) break;
					//if( object_list->next == object_list ) break;
					if( vobj->next == vobj ) break;

					if( object_list == obj ){
						object_list->value[4] = ch->in_room->area->recall;
						vobj_next = obj->next;
						object_list = obj->next;
						obj->next = NULL;
						continue;
					}
					if( object_list->in_obj == obj ){
						object_list->value[4] = ch->in_room->area->recall;
						vobj_next = object_list->next;
						object_list->next = NULL;
						object_list = vobj_next;
						continue;
					}
					if( vobj->next == obj ){
						vobj->next->value[4] = ch->in_room->area->recall;
						vobj_next = vobj;
						vobj->next = obj->next;
						obj->next = NULL;
						continue;
					}
					if( vobj->next->in_obj == obj ){
						vobj->next->value[4] = ch->in_room->area->recall;
						vobj_next = vobj->next;
						vobj->next = vobj->next->next;
						vobj_next->next = NULL;
						vobj_next = vobj;
						continue;
					}
					vobj_next = vobj->next;
				}
			}
			else
			{
				if ( obj == object_list )
				{
					object_list = obj->next;
					obj->next = NULL; 
				}
				else
				{
					for ( vobj = object_list; vobj; vobj = vobj->next )
					{
						if ( obj == vobj->next )
						{
							vobj->next = obj->next;
							obj->next = NULL;
							break;
						}
					}
				}
			}

			obj->next_content = ch->neweqbank->objs;
			ch->neweqbank->objs = obj;

			if( fBank )
				ch->bank -= cost;
			else
				ch->gold -= cost;

			// ²¾°£ to_room Á×§K¤j¶q¦s¨úª««~³y¦¨¥L¤Hµe­±­â¶Ã 2022/02/01
			//act( "¸Ë³Æ»È¦æ¦ÑÁó¤p¤ß¦a±N$nªº$p®³¨ì«á­±ªº«OÀI®w¦s©ñ¡C", ch, obj, NULL, TO_ROOM );
			act( "¸Ë³Æ»È¦æ¦ÑÁó¤p¤ß¦a±N§Aªº$p®³¨ì«á­±ªº«OÀI®w¦s©ñ¡C", ch, obj, NULL, TO_CHAR );

			if( fBank )
				send_to_char("§A±q»È¦æÂà±b¡A¥I¥X¤F "HIY, ch);
			else
				send_to_char("§A¥I¥X¤F "HIY, ch);
			sprintf(tmp, "%d", cost);
			send_to_char(tmp, ch);
			send_to_char(NOR " ¤¸ªº«OºÞ¶O¡C\n\r", ch);
			do_save( ch, "" );
			ch->neweqbank->amount += 1;
			sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
			if(( fp = fopen(tmp, "w"))){
				//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
				fwrite_neweqbank(ch, fp);
				fclose(fp);
				return;
			}else{
				bug("eqbank: file open error.",0);
				return;
			}
			return;
		}
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u§Ú¨S¬Ý¨ì³o¶µ¸Ë³Æ°Ú¡H¡v\n\r", ch );
		return;
	}

	if( !str_cmp(arg1, "fopen"))
	{
		if( ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u³Â·Ð½Ð¥ýÃö¤W¨Ï¥Î¤¤ªº­Ü®w¡C¡v\n\r", ch );
			return;
		}
		if( get_trust(ch) < 43 ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u¹ï¤£°_¡A±zªºµ¥¯Å¤£°÷¡C¡v\n\r", ch );
			return;
		}
		if( arg2[0] == '\0' ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u½Ð§i¶D§Ú­Ü®w¦WºÙ¡C¡v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
		bank = new_bank();

		if( (fp= fopen(tmp, "r")))
		{
			fread_neweqbank(bank, fp); 
			fclose(fp);
		}else{
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u¨S¦³³o­Ó­Ü®w®@¡C¡v\n\r", ch );
			return;
		}
		ch->neweqbank = bank;
		bank->opened = TRUE;
		if(( fp = fopen(tmp, "w")))
		{
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}else{
			bug("eqbank: file open error.",0);
		}
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u¶}±Ò¦¨¥\\¡C¡v\n\r", ch );
		return;
	}

	/*if( !str_cmp(arg1, "forceopen")){
	if( ch->neweqbank ) {
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u³Â·Ð½Ð¥ýÃö¤W¨Ï¥Î¤¤ªº­Ü®w¡C¡v\n\r", ch );
	return;
	}
	if( arg2[0] == '\0' || arg3[0] == '\0' ){
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u½Ð§i¶D§Ú­Ü®w¦WºÙ©M±K½X¡C¡v\n\r", ch );
	return;
	}
	sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
	bank = new_bank();
	if( (fp= fopen(tmp, "r"))){
	fread_neweqbank(bank, fp); 
	fclose(fp);
	}
	else {
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u¨S¦³³o­Ó­Ü®w®@¡C¡v\n\r", ch );
	return;
	}
	if( str_cmp( bank->password, crypt(arg3, arg2))){
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z½T©w±K½X¬O¹ïªº¶Ü¡H¡C¡v\n\r", ch );
	return;
	}
	ch->neweqbank = bank;
	bank->opened = TRUE;
	bugf("%s opened bank [%s]", ch->name, arg2);
	bank->recall = ch->in_room->area->recall;
	if(( fp = fopen(tmp, "w"))){
	//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
	fwrite_neweqbank(ch, fp);
	fclose(fp);
	}
	else{
	bug("eqbank: file open error.",0);
	}
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u¶}±Ò¦¨¥\\¡C¡v\n\r", ch );
	return;

	}*/

	if( !str_cmp(arg1, "open"))
	{
		if( ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u³Â·Ð½Ð¥ýÃö¤W¨Ï¥Î¤¤ªº­Ü®w¡C¡v\n\r", ch );
			return;
		}
		if( arg2[0] == '\0' || arg3[0] == '\0' ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u½Ð§i¶D§Ú­Ü®w¦WºÙ©M±K½X¡C¡v\n\r", ch );
			return;
		}
		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( arg2), capitalize(arg2));
		bank = new_bank();
		if( (fp= fopen(tmp, "r"))){
			fread_neweqbank(bank, fp); 
			fclose(fp);
		}else{
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u¨S¦³³o­Ó­Ü®w®@¡C¡v\n\r", ch );
			return;
		}
		if( str_cmp( bank->password, crypt(arg3, arg2))){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z½T©w±K½X¬O¹ïªº¶Ü¡H¡C¡v\n\r", ch );
			free_neweqbank(bank);
			return;
		}
		if( bank->opened ) 
		{
			if( check_eqbank( bank ) ){
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u©êºp¡A³o­Ó­Ü®w¥Ø«e¦³§O¤H¦b¨Ï¥Î¤¤¡C¡v\n\r", ch );
				free_neweqbank(bank);
				return;
			}
		}
		ch->neweqbank = bank;
		bank->opened = TRUE;
		bank->recall = ch->in_room->area->recall;
		if(( fp = fopen(tmp, "w"))){
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
		}else{
			bug("eqbank: file open error.",0);
		}
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u¶}±Ò¦¨¥\\¡C¡v\n\r", ch );
		return;
	}

	if( !str_prefix(arg1, "exam")) // §¹¦¨ eqbank exam ªº«ü¥O 2022/12/01
	{
		if( !ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z±o¥ý¥´¶}­Ó­Ü®w¡C¡v\n\r", ch );
			return;
		}
		if ( !is_number( arg2 ) ){
			send_to_char( "½Ð¥Î 'eqbank exam ½s¸¹' ¨ÓÀËµø±zªº¸Ë³Æ¡C\n\r", ch );
			return;
		}
		if( !ch->neweqbank->objs ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±zªº­Ü®w¸Ì¨S¦³ªF¦è°Ú¡H¡v\n\r", ch );
			return;
		}

		count = atoi( arg2);

		i = 1;

		if( count == 1 && ch->neweqbank->objs->value[4] == ch->in_room->area->recall )
		{
			obj = ch->neweqbank->objs;
		}
		else if ( ch->neweqbank->objs->next_content )//&& ch->neweqbank->objs->next_content )
		{
			if( ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
				++i;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj->next_content )
			{
				if( !vobj->next_content ) break;
				if( vobj->next_content->value[4] != ch->in_room->area->recall ) continue;
				if( count == i )
				{
					if( vobj->next_content->value[4] == ch->in_room->area->recall )
					{
						obj = vobj->next_content;
						break;
					}
					else
						continue;
				}
				i++;
			}
		}
		if ( obj )
		{
			eqb_do_examine(ch, obj);
			return;
		}
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¬d¾\\¤FÂd»O¤Wªºµn°OÃ¯»¡¡G©_©Ç¡A¨S¦³³o¶µ¸Ë³Æªº¬ö¿ý³á¡I\n\r", ch );
		return;
	}

	if( !str_prefix(arg1, "id"))
	{
		if( !ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z±o¥ý¥´¶}­Ó­Ü®w¡C¡v\n\r", ch );
			return;
		}
		if ( !is_number( arg2 ) ){
			send_to_char( "½Ð¥Î 'eqbank id ½s¸¹' ¨ÓÅ²©w±zªº¸Ë³Æ¡C\n\r", ch );
			return;
		}
		if( !ch->neweqbank->objs ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±zªº­Ü®w¸Ì¨S¦³ªF¦è°Ú¡H¡v\n\r", ch );
			return;
		}

		count = atoi( arg2);

		i = 1;
		if( count == 1 && ch->neweqbank->objs->value[4] == ch->in_room->area->recall )
		{
			obj = ch->neweqbank->objs;
		}
		else if ( ch->neweqbank->objs->next_content )//&& ch->neweqbank->objs->next_content )
		{
			if( ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
				++i;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj->next_content )
			{
				if( !vobj->next_content ) break;
				if( vobj->next_content->value[4] != ch->in_room->area->recall ) continue;
				if( count == i )
				{
					if( vobj->next_content->value[4] == ch->in_room->area->recall )
					{
						obj = vobj->next_content;
						break;
					}
					else
						continue;
				}
				i++;
			}
		}
		if ( obj )
		{
			cost = URANGE( 300, obj->cost / 10 , 3000);
			if ( ch->bank < cost ){
				fBank = FALSE;
				if( ch->gold < cost ){
					send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó­W¯ºµÛ»¡¡G¡u¹ï¤£°_¡A¨S¦³¨¬°÷ªºÅ²©w¶O§ÚµLªk¶±¥Î«O¥þ¤H­û¡C¡v\n\r", ch );
					return;
				}
			}
			if( fBank )
				ch->bank -= cost;
			else
				ch->gold -= cost;
			if( fBank )
				send_to_char("§A±q»È¦æÂà±b¡A¥I¥X¤F "HIY, ch);
			else
				send_to_char("§A¥I¥X¤F "HIY, ch);
			sprintf(tmp, "%d", cost);
			send_to_char(tmp, ch);
			send_to_char(NOR " ¤¸ªºÅ²©w¶O¡C\n\r", ch);
			spell_identify(gsn_invoke, ch->level,ch, obj);
			return;
		}
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¬d¾\\¤FÂd»O¤Wªºµn°OÃ¯»¡¡G©_©Ç¡A¨S¦³³o¶µ¸Ë³Æªº¬ö¿ý³á¡I\n\r", ch );
		return;
	}

	if( !str_prefix(arg1, "receive"))
	{
		if( !ch->neweqbank ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z±o¥ý¥´¶}­Ó­Ü®w¡C¡v\n\r", ch );
			return;
		}
		if ( !is_number( arg2 ) ){
			send_to_char( "½Ð¥Î 'eqbank receive ½s¸¹' ¨Ó»â¨ú±zªº¸Ë³Æ¡C\n\r", ch );
			return;
		}
		if( !ch->neweqbank->objs ){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±zªº­Ü®w¸Ì¨S¦³ªF¦è°Ú¡H¡v\n\r", ch );
			return;
		}

		count = atoi( arg2);

		i = 1;
		if( count == 1 && ch->neweqbank->objs->value[4] == ch->in_room->area->recall )
		{
			obj = ch->neweqbank->objs;
			if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) ){
				do_say(vch, "¹ï¤£°_¡A±z®³¤£°Ê§ó¦hªF¦è¤F¡C");
				return;
			}

			if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) ){
				do_say(vch,"¹ï¤£°_¡A±zªº­t­«¤£°÷¤F¡C");
				return;
			}
			ch->neweqbank->objs = obj->next_content;
			obj->next_content = NULL;
		}
		else if ( ch->neweqbank->objs->next_content )//&& ch->neweqbank->objs->next_content )
		{
			if( ch->neweqbank->objs->value[4] == ch->in_room->area->recall)
				++i;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj->next_content )
			{
				if( !vobj->next_content ) break;
				if( vobj->next_content->value[4] != ch->in_room->area->recall ) continue;
				if( count == i )
				{
					if( vobj->next_content->value[4] == ch->in_room->area->recall )
					{
						obj = vobj->next_content;
						if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) )
						{
							do_say(vch, "¹ï¤£°_¡A±z®³¤£°Ê§ó¦hªF¦è¤F¡C");
							return;
						}

						if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) )
						{
							do_say(vch,"¹ï¤£°_¡A±zªº­t­«¤£°÷¤F¡C");
							return;
						}
						vobj->next_content = obj->next_content;
						obj->next_content = NULL;
						break;
					}
					else
						continue;
				}
				i++;
			}
		}
		if ( obj )
		{
			/*
			if( obj->item_type == ITEM_CONTAINER ) {
			OBJ_DATA * vobj_next = NULL;
			for( vobj = ch->neweqbank->objs ; vobj; vobj = vobj_next ){
			if( ch->neweqbank->objs->in_obj == obj ) {
			vobj_next = ch->neweqbank->objs->next_content;
			ch->neweqbank->objs->next = object_list;
			object_list = ch->neweqbank->objs;
			object_list->next_content = obj->contains;
			obj->contains = object_list;
			ch->neweqbank->objs = vobj_next;
			continue;
			}
			if( vobj->next_content->in_obj == obj ) {
			vobj_next = vobj;
			vobj->next_content->next = object_list;
			object_list = vobj->next_content;
			vobj->next_content = vobj->next_content->next_content;
			object_list->next_content = obj->contains;
			obj->contains = object_list;
			continue;
			}
			}
			}*/

			if( obj->item_type == ITEM_CONTAINER)
			{
				for( vobj = obj->contains; vobj ; vobj = vobj->next_content )
				{
					vobj->next = object_list;
					object_list = vobj;
				}
			}
			obj_to_char ( obj, ch );
			obj->next = object_list;
			object_list = obj;
			// ²¾°£ to_room Á×§K¤j¶q¦s¨úª««~³y¦¨¥L¤Hµe­±­â¶Ã 2022/02/01
			//act( "¸Ë³Æ»È¦æ¦ÑÁó¦Û«á­±ªº®w©Ð±N$p±·¨Ó¥æµ¹$n¡C", ch, obj, NULL, TO_ROOM );
			//act( "¸Ë³Æ»È¦æ¦ÑÁó¤@Áù°`»¡¡G¡u½Ð±zÀË¬d¦³µL¥ô¦ó·l¶Ë¡AÁÂÁÂ¥úÁ{¡C¡v", ch, obj, NULL, TO_ROOM );
			act( "¸Ë³Æ»È¦æ¦ÑÁó¦Û«á­±ªº®w©Ð±N$p±·¨Ó¥æµ¹§A¡C", ch, obj, NULL, TO_CHAR );
			act( "¸Ë³Æ»È¦æ¦ÑÁó¤@Áù°`»¡¡G¡u½Ð±zÀË¬d¦³µL¥ô¦ó·l¶Ë¡AÁÂÁÂ¥úÁ{¡C¡v", ch, obj, NULL, TO_CHAR );
			ch->neweqbank->amount -= 1;
			sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
			if(( fp = fopen(tmp, "w"))){
				//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
				fwrite_neweqbank( ch, fp);
				fclose(fp);
				do_save( ch, "" );
				return;
			}else{
				bug("eqbank: file open error.",0);
				do_save( ch, "" );
				return;
			}
			return;
		}
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¬d¾\\¤FÂd»O¤Wªºµn°OÃ¯»¡¡G©_©Ç¡A¨S¦³³o¶µ¸Ë³Æªº¬ö¿ý³á¡I\n\r", ch );
		return;
	}

	if( !str_cmp(arg1, "list"))
	{
		if(!ch->neweqbank){
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u±zÁÙ¨S¥´¶}­Ü®w©O¡C¡v\n\r", ch );
			return;
		}

		send_to_char( "§A¥Ø«e¦s¦b¸Ë³Æ»È¦æ«OÀI®wªº¸Ë³Æ¦³¡G\n\r", ch );

		if ( !ch->neweqbank->objs ){
			send_to_char( "\n\r¤°»ò³£¨S¦³¡I\n\r\n\r", ch );
			return; 
		}

		send_to_char("¡i½s¸¹¡j  ª«  «~  ¦W  ºÙ                                  µ¥¯Å\n\r", ch);

		for ( vobj = ch->neweqbank->objs; vobj;)
		{
			out_buf[0] = 0;

			/* ¦³ Buffer ª©¥» *

				for(count = 0; count < 50 && vobj; count++, vobj = vobj->next_content){
				if( vobj->in_obj ) continue;
				if( vobj->value[4] != ch->in_room->area->recall ) continue;
				i++;
				//add by jye 0940619
				if( arg2[0] != '\0')
				{
				if( !is_pfxname( arg2, vobj->name )) continue;
				}
				strip_ansi(vobj->short_descr, no_ansi);
				sprintf ( tmp, "¡i%4d¡j%-48s  [%2d] \n\r", 
				i,  
				can_see_obj(ch, vobj) ? no_ansi :
				"¤£©úª««~",
				vobj->level
				);
				strcat(out_buf, tmp);
				}
				send_to_char(out_buf, ch);

			/**/

			/* µL Buffer ª©¥» */

			for(;vobj; vobj = vobj->next_content)
			{
				if( vobj->in_obj )
					continue;
				if( vobj->value[4] != ch->in_room->area->recall )
					continue;

				i++;

				if( arg2[0] != '\0')  //add by jye 0940619
				{
					if( !is_pfxname( arg2, vobj->name ) )
						continue;
				}
				strip_ansi(vobj->short_descr, no_ansi);
				sprintf ( tmp, "¡i%4d¡j%-48s  [%2d] \n\r",
								i,
								can_see_obj(ch, vobj) ? no_ansi : "¤£©úª««~",
								vobj->level );
				send_to_char(tmp, ch);
			}

			/**/
		}
		send_to_char( "\n\r", ch );
		return;
	}

	if( !str_cmp(arg1, "close"))
	{
		if( !ch->neweqbank ) {
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±z±o¥ý¥´¶}­Ó­Ü®w¤~¯àÃö¤W¡C¡v\n\r", ch );
			return;
		}

		sprintf(tmp, "%s%s/%s", EQBANK_DIR,initial( ch->neweqbank->name), capitalize(ch->neweqbank->name));
		ch->neweqbank->opened = FALSE;

		if(( fp = fopen(tmp, "w")))
		{
			//fwrite_newbank_obj( ch, ch->neweqbank->objs, fp, 0);
			fwrite_neweqbank(ch, fp);
			fclose(fp);
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó»¡µÛ¡G¡u­Ü®wÃö³¬§¹¦¨¡C¡v\n\r", ch );
		}
		else
		{
			bug("eqbank: file open error!!",0);
			send_to_char("µo¥ÍÄY­«°ÝÃD¡A½Ð¦V¤j¯«¦^³ø¡C\n\r", ch);
			return;
		}
		free_neweqbank(ch->neweqbank);
		ch->neweqbank = NULL;
		return;
	}
	send_to_char("½Ð help eqbank ¥H¨ú±o¥Îªk»¡©ú¡C\n\r", ch);
	return;
}

void do_get_silence( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *container;
	//char buf [ MAX_INPUT_LENGTH ];
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	bool found;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	/* Get type. */
	if ( arg1[0] == '\0' )
	{
		send_to_char( "­n®³¤°»ò?\n\r", ch );
		return;
	}

	if ( arg2[0] == '\0' )
	{
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj' */
			obj = get_obj_list( ch, arg1, ch->in_room->contents );
			if ( !obj )
			{
				return;
			}

			get_obj( ch, obj, NULL );
		}
		else
		{
			/* 'get all' or 'get all.obj' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = ch->in_room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, NULL );
				}
			}
		}
	}
	else
	{
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		{
			return;
		}
		/*
		sprintf( buf, "%s ªº«ÍÅé(corpse)", ch->name );
		for ( obj = object_list; obj; obj = obj->next )
		{
		if( !str_cmp( buf, obj->short_descr ) )
		{
		send_to_char( "§A¥ý³B²z¦n§A¦Û¤v§a¡I\n\r", ch );
		return;
		}
		}
		*/
		if ( !(container = get_obj_list( ch, arg2, ch->in_room->contents )) ){
			if ( !( container = get_obj_here( ch, arg2 ) ) )
			{
				return;
			}
		}

		switch ( container->item_type )
		{
			default:
			return;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			break;

			case ITEM_CORPSE_PC:
			{
				CHAR_DATA *gch;
				char  *pd, *cd;
				char   name [ MAX_INPUT_LENGTH ];
				char   chname [ MAX_INPUT_LENGTH ];

				if ( IS_NPC( ch ) )
				{
					return;
				}

				pd = container->short_descr;/*short_descr;
				pd = one_argument( pd, name );
				pd = one_argument( pd, name );*/
				pd = one_argument( pd, name );
				cd = ch->name;
				cd = one_argument( cd, chname );

				if ( str_cmp( name, chname ) && !IS_IMMORTAL( ch ) )
				{
					bool fGroup;

					fGroup = FALSE;
					for ( gch = char_list; gch; gch = gch->next )
					{
						if ( !IS_NPC( gch ) && is_same_group( ch, gch ) && !str_cmp( name, gch->name ) )
						{
							fGroup = TRUE;
							break;
						}
					}
					if ( !fGroup )
					{
						return;
					}
				}
			}
		}

		if ( IS_SET( container->value[1], CONT_CLOSED ) )
		{
			return;
		}

		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj container' */
			obj = get_obj_list( ch, arg1, container->contains );
			if ( !obj )
			{
				return;
			}
			get_obj( ch, obj, container );
		}
		else
		{
			/* 'get all container' or 'get all.obj container' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = container->contains; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) ) && can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, container );
				}
			}
		}
	}

	return;
}
void do_get( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *container;
	//char      buf [ MAX_INPUT_LENGTH ];
	char      arg1 [ MAX_INPUT_LENGTH ];
	char      arg2 [ MAX_INPUT_LENGTH ];
	bool      found;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

    /* Get type. */
	if ( arg1[0] == '\0' )
	{
		send_to_char( "­n®³¤°»ò?\n\r", ch );
		return;
	}

	if ( arg2[0] == '\0' )
	{
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj' */
			obj = get_obj_list( ch, arg1, ch->in_room->contents );
			if ( !obj )
			{
				act( "³o¸Ì¨S¦³ $T.", ch, NULL, arg1, TO_CHAR );
				return;
			}
    
			get_obj( ch, obj, NULL );
		}
		else
		{
			/* 'get all' or 'get all.obj' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = ch->in_room->contents; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
				//if( obj->item_type == ITEM_CORPSE_PC && !IS_IMMORTAL(ch) )
				//  continue;
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, NULL );
				}
			}

			if ( !found ) 
			{
				if ( arg1[3] == '\0' )
					send_to_char( "³o¸Ì¨S¦³¥ô¦óª««~.\n\r", ch );
				else
					act( "³o¸Ì¨S¦³ $T.", ch, NULL, &arg1[4], TO_CHAR );
			}
		}
	}
	else
	{
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		{
			send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
			return;
		}
		/*
			sprintf( buf, "%s ªº«ÍÅé(corpse)", ch->name );
			for ( obj = object_list; obj; obj = obj->next )
			{
			if( !str_cmp( buf, obj->short_descr ) )
			{
			  send_to_char( "§A¥ý³B²z¦n§A¦Û¤v§a¡I\n\r", ch );
			  return;
			}
			}
		*/
		if ( !(container = get_obj_list( ch, arg2, ch->in_room->contents )) ){
			if ( !( container = get_obj_here( ch, arg2 ) ) )
			{
				act( "³o¸Ì¨S¦³ $T.", ch, NULL, arg2, TO_CHAR );
				return;
			}
		}

		switch ( container->item_type )
		{
			default:
				send_to_char( "¨º¤£¬O­Ó®e¾¹.\n\r", ch );
				return;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_HORSE:
				break;

			case ITEM_CORPSE_PC:
			{
				//CHAR_DATA *gch;
				char  *pd, *cd;
				char   name [ MAX_INPUT_LENGTH ];
				char   chname [ MAX_INPUT_LENGTH ];

				if ( IS_NPC( ch ) )
				{
					send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
					return;
				}

				pd = container->short_descr;/*short_descr;
				pd = one_argument( pd, name );
				pd = one_argument( pd, name );*/
				pd = one_argument( pd, name );
				cd = ch->name;
				cd = one_argument( cd, chname );

				if ( str_cmp( name, chname ) && !IS_IMMORTAL( ch ) )
				{
					bool fGroup;
      
					fGroup = FALSE;
					/*for ( gch = char_list; gch; gch = gch->next )
					{
					  if ( !IS_NPC( gch )
					&& is_same_group( ch, gch )
									  &&  !consented(ch, obj)
					&& !str_cmp( name, gch->name ) )
					  {
					fGroup = TRUE;
					break;
					  }
					}*/
					if ( !consented(ch, container))
					{
						send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
						return;
					}
				}
				// ¾ß¦^«ÍÅé«á¥i¥H¦A¦¸¶i¦æ backup ªº°Ê§@ 2022/10/10
				if( is_qmark(ch, "death_stop_backup") )
					do_del_qmark( ch, "self death_stop_backup" );
			}
		}

		if ( container->item_type == ITEM_CONTAINER && IS_SET( container->value[1], CONT_CLOSED ) )
		{
			act( "$d ¬OÃöµÛªº.", ch, NULL, container->name, TO_CHAR );
			return;
		}

		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
		{
			/* 'get obj container' */
			obj = get_obj_list( ch, arg1, container->contains );
			if ( !obj )
			{
				act( "$T ¸Ì¨S¦³³o¼ËªF¦è.", ch, NULL, arg2, TO_CHAR );
				return;
			}
			get_obj( ch, obj, container );
		}
		else
		{
			/* 'get all container' or 'get all.obj container' */
			OBJ_DATA *obj_next;

			found = FALSE;
			for ( obj = container->contains; obj; obj = obj_next )
			{
				obj_next = obj->next_content;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj ) )
				{
					found = TRUE;
					get_obj( ch, obj, container );
				}
			}

			if ( !found )
			{
				if ( arg1[3] == '\0' )
					act( "$T ¸Ì¨S¦³³o¼ËªF¦è.", ch, NULL, arg2, TO_CHAR );
				else
					act( "$T ¸Ì¨S¦³³o¼ËªF¦è.", ch, NULL, arg2, TO_CHAR );
			}
		}
	}
	save_char_obj( ch );
	return;
}



void do_put( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *container;
	OBJ_DATA *obj;
	char arg1 [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];

	argument = one_argument( argument, arg1 );//get the first parameter from argument
	one_argument( argument, arg2 );           //and remove it from argument, then get
	//the second parameter from argument.
	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "§â¤°»ò©ñ¶i¤°»ò¡H\n\r", ch );
		return;
	}

	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
		send_to_char( "§AµLªk³o»ò°µ.\n\r", ch );
		return;
	}

	if ( !( container = get_obj_here( ch, arg2 ) ) )
	{
		act( "³o¸Ì¨S¦³ $T.", ch, NULL, arg2, TO_CHAR );
		return;
	}

	if ( container->item_type != ITEM_CONTAINER && container->item_type != ITEM_HORSE)
	{
		send_to_char( "¨º¤£¬O­Ó®e¾¹.\n\r", ch );
		return;
	}

	if ( container->item_type == ITEM_CONTAINER && IS_SET( container->value[1], CONT_CLOSED ) )
	{
		act( "$d ¬OÃöµÛªº.", ch, NULL, container->name, TO_CHAR );
		return;
	}
    
	//add by coconet 0511 for check whether the horse can carry things or not,
	//if v0 = -1, means can't
	if ( container->item_type == ITEM_HORSE && container->value[0] == -1 )
	{
		send_to_char("¨eµLªk­t¸ü¥ô¦óªF¦è³á¡C\n\r",ch);
		return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
		/* 'put obj container' */
		if ( !( obj = get_obj_carry( ch, arg1 ) ) )
		{
			send_to_char( "§A¨S¦³³o­ÓªF¦è.\n\r", ch );
			return;
		}

		if ( obj == container )
		{
			send_to_char( "§AµLªk§â¥¦¶ë¶i¥¦¦Û¤v¸Ì.\n\r", ch );
			return;
		}

		if ( !can_drop_obj( ch, obj ) )
		{
			send_to_char( "§A¥á¤£±¼.\n\r", ch );
			return;
		}

		if ( container->item_type == ITEM_CONTAINER )
		{
			if (  get_obj_weight( obj ) + get_obj_weight( container ) - container->weight > container->value[0] )
			{
				if ( container->value[0] != -1 )
				{
					send_to_char( "©ñ¤£¶i¥h.\n\r", ch );
					return;
				}
			}
		}
		else
		{

			if ( get_obj_weight( obj ) + get_obj_weight( container ) - container->weight > container->value[0]  )
			{
				send_to_char(" °¨¤Ç¤w¸g­t¸ü¤Ó¦hªF¦è¤F¡C\n\r",ch);
				return;
			}
		}

		if( container->item_type != ITEM_HORSE )
		{
			act( "§A§â $p ©ñ¤J $P.", ch, obj, container, TO_CHAR );
			act( "$n §â $p ©ñ¤J $P.", ch, obj, container, TO_ROOM );
		}
		else
		{
			act( "§A§â $p ©ñ¦b $P­I¤W.", ch, obj, container, TO_CHAR );
			act( "$n §â $p ©ñ¦b $P­I¤W.", ch, obj, container, TO_ROOM );    
		}

		// 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("put", ch, obj);
		}

		// add at 2022/02/12
		if (!obj){
			save_char_obj( ch );
			return;
		}
		
		obj_from_char( obj );
		obj_to_obj( obj, container );
	}
	else
	{
		/* 'put all container' or 'put all.obj container' */
		OBJ_DATA *obj_next;
  
		if ( container->item_type == ITEM_CONTAINER )
		{
			for ( obj = ch->carrying; obj; obj = obj_next )
			{
				obj_next = obj->next_content;
      
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& obj != container
					&& can_drop_obj( ch, obj )
					&& ( get_obj_weight( obj ) + get_obj_weight( container ) <= container->value[0] || container->value[0] == -1 ) )
				{
					act( "§A§â $p ©ñ¤J $P.", ch, obj, container, TO_CHAR );
					act( "$n §â $p ©ñ¤J $P.", ch, obj, container, TO_ROOM );

					// 2022/02/12
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("put", ch, obj);
					}

					// add at 2022/02/12
					if (!obj) continue;

					obj_from_char( obj );
					obj_to_obj( obj, container );
				}
			}
		}
		else
		{
			for( obj = ch->carrying; obj ; obj = obj_next )
			{
				obj_next = obj->next_content;
      
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&& can_see_obj( ch, obj )
					&& obj->wear_loc == WEAR_NONE
					&& obj != container
					&& can_drop_obj( ch, obj )
					&& ( get_obj_weight( obj ) + get_obj_weight( container ) <= container->value[0] ))
				{
					act( "§A§â $p ©ñ¦b $P­I¤W.", ch, obj, container, TO_CHAR );
					act( "$n §â $p ©ñ¦b $P­I¤W.", ch, obj, container, TO_ROOM );

					// 2022/02/12
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("put", ch, obj);
					}

					// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/12
					if (!obj) continue;

					obj_from_char( obj );
					obj_to_obj( obj, container );
				}
			}   
		}
	}
	save_char_obj( ch );
	return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	bool      found;

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "·FÔ£?\n\r", ch );
		return;
	}

	if ( is_number( arg ) )
	{
		/* 'drop NNNN coins' */
		OBJ_DATA *obj_next;
		int   amount;

		amount   = atoi( arg );
		one_argument( argument, arg );

		if ( amount <= 0 || str_prefix( arg, "coins" ) )
		{
		  send_to_char( "¹ï¤£°_, §AµLªk³o»ò°µ.\n\r", ch );
		  return;
		}

		if( ch->class <= BARBARIAN && ch->level < 3 ) {
		  send_to_char("§A¦~¬öÁÙ¤p¡A§O¶Ã¥áª÷¿ú¡C\n\r", ch);
		  return;
		}

		if ( ch->gold < amount )
		{
		  send_to_char( "§A¨S¦³¨º»ò¦h¿ú.\n\r", ch );
		  return;
		}

		ch->gold -= amount;

		for ( obj = ch->in_room->contents; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( obj->deleted )
				continue;

			switch ( obj->pIndexData->vnum )
			{
				case OBJ_VNUM_MONEY_ONE:
					amount += 1;
					extract_obj( obj );
				break;

				case OBJ_VNUM_MONEY_SOME:
					amount += obj->value[0];
					extract_obj( obj );
				break;
			}
		}

		obj_to_room( create_money( amount ), ch->in_room );
		send_to_char( "OK.\n\r", ch );  
		act( "$n ¥á¤U¤@¨Ç¿ú.", ch, NULL, NULL, TO_ROOM );
		return;
	}

	if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
	{
		/* 'drop obj' */
		if ( !( obj = get_obj_carry( ch, arg ) ) )
		{
			send_to_char( "§A¨S¦³¨º¼ËªF¦è.\n\r", ch );
			return;
		}

		if ( !can_drop_obj( ch, obj ) // && !IS_SET(obj->extra_flags, ITEM_PERSONAL))
			&& obj->pIndexData->vnum != OBJ_VNUM_MAGIC_ARROW
			&& obj->pIndexData->vnum != OBJ_VNUM_MAGIC_AMMO ) // ¥[¤JÅ]ªk¼u¥i¥H³Q¥á±ó 2022/04/30
		{
			send_to_char( "§A¥á¤£±¼¥¦.\n\r", ch );
			return;
		}
		act( "§A¥á¤U $p.", ch, obj, NULL, TO_CHAR );
		act( "$n ¥á¤U $p.", ch, obj, NULL, TO_ROOM );
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
		// ·s¼W obj_act drop 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("drop", ch, obj);
		}
		// ±Ô­z²¾¨ì obj Â÷¶} ch «e 2022/02/12
		//act( "§A¥á¤U $p.", ch, obj, NULL, TO_CHAR );
		//act( "$n ¥á¤U $p.", ch, obj, NULL, TO_ROOM );
		if( IS_SET(obj->extra_flags, ITEM_PERSONAL))
			extract_obj(obj);
	}
	else
	{
		/* 'drop all' or 'drop all.obj' */
		OBJ_DATA *obj_next;

		found = FALSE;
		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
				&& can_see_obj( ch, obj )
				&& obj->wear_loc == WEAR_NONE
				&& can_drop_obj( ch, obj ) )
			{
				act( "§A¥á¤U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¥á¤U $p.", ch, obj, NULL, TO_ROOM );
				found = TRUE;
				obj_from_char( obj );
				obj_to_room( obj, ch->in_room );
				// ·s¼W obj_act drop 2022/02/12
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("drop", ch, obj);
				}
				// ±Ô­z²¾¨ì obj Â÷¶} ch «e 2022/02/12
				//act( "§A¥á¤U $p.", ch, obj, NULL, TO_CHAR );
				//act( "$n ¥á¤U $p.", ch, obj, NULL, TO_ROOM );
			}
		}

		if ( !found )
		{
			if ( arg[3] == '\0' )
				send_to_char( "§A¨­¤W¨S¥bÂIªF¦è.", ch );
			else
				act( "§A¨­¤W¨S¦³ $T.", ch, NULL, &arg[4], TO_CHAR );
		}
	}
	save_char_obj( ch );
	return;
}



void do_give( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA  *obj;
	char       arg1 [ MAX_INPUT_LENGTH ];
	char       arg2 [ MAX_INPUT_LENGTH ];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "­nµ¹½Ö¤°»ò¡H\n\r", ch );
		return;
	}

	if ( is_number( arg1 ) )
	{
		/* 'give NNNN coins victim' */
		char buf[ MAX_STRING_LENGTH ];
		int  amount;

		amount   = atoi( arg1 );
		if ( amount <= 0 || str_prefix( arg2, "coins" ) )
		{
			send_to_char( "Sorry, §AµLªk³o»ò°µ.\n\r", ch );
			return;
		}

		one_argument( argument, arg2 );

		if ( arg2[0] == '\0' )
		{
			send_to_char( "­nµ¹½Ö¤°»ò¡H\n\r", ch );
			return;
		}
		if( ch->class <= BARBARIAN && ch->level < 3 ) {
			send_to_char("§A¦~¬öÁÙ¤p¡A§O¶Ã¥áª÷¿ú¡C\n\r", ch);
			return;
		}

		if ( !( victim = get_char_room( ch, arg2 ) ) )
		{
			send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
			return;
		}

		if( victim == ch ) {
			send_to_char("§A§â¤f³U¸Ìªº¿ú®³¥X¨Ó§âª±¤@¤U¡C\n\r", ch);
			return;
		}

		if ( ch->gold < amount )
		{
			send_to_char( "§A¨S¦³³o»ò¦h¿ú.\n\r", ch );
			return;
		}

		ch->gold     -= amount;
		if( !IS_NPC(victim) )
			victim->gold += amount;
		act( "§Aµ¹¤F$N¤@¨Ç¿ú.", ch, NULL, victim, TO_CHAR    );

		sprintf( buf, "$nµ¹¤F§A %d ªTª÷¹ô.", amount );
		act( buf, ch, NULL, victim, TO_VICT    );

		act( "$nµ¹¤F$N¤@¨Ç¿ú.", ch, NULL, victim, TO_NOTVICT );
		mprog_bribe_trigger( victim, ch, amount );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg1 ) ) )
	{
		send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
		return;
	}

	if ( obj->wear_loc != WEAR_NONE )
	{
		send_to_char( "§A±o¥ý§â¥¦²æ¤U¨Ó.\n\r", ch );
		return;
	}

	if( IS_SET(obj->extra_flags, ITEM_PERSONAL ) && !IS_IMMORTAL( ch ))
	{
		send_to_char("³o¼ËªF¦è¤£¯àµ¹¤H.\n\r", ch);
		return;
	}


	if ( !( victim = get_char_room( ch, arg2 ) ) )
	{
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
	}

	// if( victim == ch ) {
	// 	send_to_char("§A¤£»Ý­nµ¹§A¦Û¤vªF¦è¡C\n\r", ch );
	// 	return;
	// }
	if( IS_SET(obj->extra_flags, ITEM_UNIQUE) && IS_NPC(victim)){
		send_to_char("¤£¯à¥æµ¹«Dª±®a¨¤¦â.\n\r", ch);
		return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
		send_to_char( "§AµLªk©ñ¶}¥¦.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_HOLY ) && victim->race == race_lookup( "Vampire" ) )
	{
		act( "$N ©Úµ´¦¬¤U $p.", ch, obj, victim, TO_CHAR );
		act( "$n µ¹¤F $N $p ¦ý¬O³Q©Úµ´¤F.",
		ch, obj, victim, TO_ROOM );
		act( "§A©Úµ´¦¬¤U $n µ¹ªº $p.",
		ch, obj, victim, TO_VICT );
		return;
	}

	if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
	{
		act( "$N®³¤£°Ê§ó¦hªF¦è¤F.", ch, NULL, victim, TO_CHAR );
		act( "$n¸ÕµÛ­nµ¹§A¤@¨ÇªF¦è¡A¥i¬O§A¨­¤W¤w¸g©ñº¡ªF¦è¤F.", ch, NULL, victim, TO_VICT );
		return;
	}

	if ( victim->carry_weight + get_obj_weight( obj ) > can_carry_w( victim ) )
	{
		act( "$N®³¤£°Ê§ó¦h­«¶q¤F.", ch, NULL, victim, TO_CHAR );
		act( "$n¸ÕµÛ­nµ¹§A¤@¨ÇªF¦è¡A¥i¬O§A¤w¸g®³¤£°Ê¤F.",ch, NULL, victim, TO_VICT );
		return;
	}

	if ( !can_see_obj( victim, obj ) )
	{
		act( "$N ¬Ý¤£¨£³o¼ËªF¦è.", ch, NULL, victim, TO_CHAR );
		return;
	}

	act( "§Aµ¹¤F$N$p.", ch, obj, victim, TO_CHAR );
	act( "$nµ¹¤F§A$p.",   ch, obj, victim, TO_VICT );
	act( "$nµ¹¤F$N$p.", ch, obj, victim, TO_NOTVICT );
	obj_from_char( obj );
	obj_to_char( obj, victim );
	// ±Ô­z²¾¨ì obj Â÷¶} ch «e 2022/02/12
	//act( "§Aµ¹¤F$N$p.", ch, obj, victim, TO_CHAR );
	//act( "$nµ¹¤F§A$p.",   ch, obj, victim, TO_VICT );
	//act( "$nµ¹¤F$N$p.", ch, obj, victim, TO_NOTVICT );

	// °w¹ï ch §@¥Îªº obj_act give 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("give", ch, obj);
	}
	// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
	if (!obj){
		save_char_obj( ch );
		return;
	}
	// °w¹ï victim §@¥Îªº obj_act give_target 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("give_target", victim, obj);
	}
	if (!obj){
		save_char_obj( ch );
		return;
	}
	
	mprog_give_trigger( victim, ch, obj );
	
	// add at 2022/02/12
	if(!obj){
		save_char_obj( ch );
		return;
	}

	if ( IS_NPC( victim ) && ( victim->pIndexData->pShop )
		&& obj && !obj->deleted )
	{
		// Á×§K shop Ä²µo give_prog drop ©Î give ª««~«á®ø¥¢ add at 2020/10/30
		if( !(obj = get_obj_carry( victim,  arg1) ) ){
			save_char_obj( ch );
			return;
		}
		obj_from_char( obj );
		extract_obj( obj );
	}
	save_char_obj( ch );
	return;
}




void do_fill( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    char      arg [ MAX_INPUT_LENGTH ];
    char      arg1[ MAX_INPUT_LENGTH ];
    bool      found;

    if ( !check_blind( ch ) )
		return;

    argument = one_argument( argument, arg  );//get the first parameter from argument
    one_argument( argument, arg1 );       //and remove it from argument, then get
                //the second parameter from argument.

	if ( arg[0] == '\0' )
	{
		send_to_char( "Fill ¤°»ò¡H\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
		return;
	}
    
	found = FALSE;
	if ( arg1[0] == '\0' )
	{
		for ( fountain = ch->in_room->contents; fountain; fountain = fountain->next_content )
		{
			if ( fountain->item_type == ITEM_FOUNTAIN )
			{
				found = TRUE;
				break;
			}
		}
	}
	else
    {
		fountain = get_obj_list( ch, arg1, ch->in_room->contents );
		if ( !fountain )
		{
			act( "³o¸Ì¨S¦³ $T.", ch, NULL, arg1, TO_CHAR );
			return;
		}
		if ( fountain->item_type == ITEM_FOUNTAIN )
		{
			found = TRUE;
		}
		else
		{
			send_to_char( "³oªF¦è¤£¬O¤ô·½!\n\r" ,ch);
			return;   
		} 
	}

	if ( !found )
	{
		send_to_char( "³o¸Ì¨S¦³¤ô·½¡I\n\r", ch );
		return;
	}

	if ( obj->item_type != ITEM_DRINK_CON )
	{
		send_to_char( "¨º¼ËªF¦è¤£¯à¸Ë¤ô.\n\r", ch );
		return;
	}

	if ( obj->value[0] < 1 || obj->value[1] == -1)
	{
		send_to_char( "³o¼ËªF¦è¸Ë¤£¤F¤ô.\n\r", ch );
		return;
	}
	
	if ( fountain->value[1] < 1 )
	{
		send_to_char( "³o­Ó¤ô·½°®²U¤F.\n\r", ch );
		return;
	}

	if ( obj->value[1] >= obj->value[0] )
	{
		send_to_char( "¨º¤w¸g¸Ëº¡¤F.\n\r", ch );
		return;
    }

	act("§A§â $p ­Ë¥ú¡C", ch, obj, NULL, TO_CHAR);
	act( "§A­«·s¸Ëº¡ $p.", ch, obj, NULL, TO_CHAR );
	obj->value[2] = fountain->value[2];
	obj->value[1] = UMIN(obj->value[0] , fountain->value[1]);
	obj->value[3] = fountain->value[3];
	fountain->value[1] -= obj->value[1];
	// ¥[¤J obj_act fill 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("fill", ch, obj);
	}
	// add at 2022/02/12
	if(!obj)
		return;
	// ¥[¤J obj_act filled §P©w¬u¤ô³Qfilled ªº·í¤U 2022/02/12
	if( IS_SET(fountain->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("filled", ch, fountain);
	}

	return;
}



void do_drink( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg [ MAX_INPUT_LENGTH ];
    int amount;
    int liquid;

    if ( !check_blind( ch ) ) return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
        {
            if ( obj->item_type == ITEM_FOUNTAIN ) break;
        }

        if ( !obj )
        {
            send_to_char( "³Ü¤°»ò¡H\n\r", ch );
            return;
        }
    }
    else
    {
        if ( !( obj = get_obj_here( ch, arg ) ) )
        {
            send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
            return;
        }
    }

    switch ( obj->item_type )
    {
    default:
        send_to_char( "¨º¤£¯à³Ü.\n\r", ch );
        break;

    case ITEM_FOUNTAIN:

		if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
		{
			bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
			obj->value[2] = 0;
		}
		liquid = obj->value[2];

			/*
				if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
				{
					bug( "Do_drink: bad liquid number %d.", liquid );
					liquid = obj->value[2] = 0;
				}
			*/

        if ( obj->value[1] <= 0 && obj->value[1] != -1)
        {
            act( "$p°®²U¤F.", ch, obj, NULL, TO_CHAR );
            return;
        }

		// ­×§ï°s¾Kªº±ø¥ó 2023/01/13
		//if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 10 && liq_table[liquid].liq_affect[COND_DRUNK] > 0)
		if ( !IS_NPC( ch ) )
		{
			// °sºë¤¤¬rªº±¡ªp¤UµLªk¶i­¹ 2023/01/13
			if( ch->pcdata->condition[COND_DRUNK] >= 70 )
			{
				send_to_char( "§A·P¨ì­G¦bªÈµ²¡A®Ú¥»µLªk¶i­¹...\n\r", ch );
				return;
			}
			// ¶V¾K¶V³Ü¤£¨ì°s
			if( ch->pcdata->condition[COND_DRUNK] >= 20 && liq_table[liquid].liq_affect[COND_DRUNK] > 0 )
			{
				if( ch->pcdata->condition[COND_DRUNK] < number_percent() )
				{
					send_to_char( "«x¡H«ç»ò¦Ñ¬O³Ü¤£¨ì°Ú...  *§c*\n\r", ch );
					return;
				}
			}
		}

			/*   remove at 19/12/22
				if ( ch->race == race_lookup( "vampire" ) && obj->value[2] != 13)
				{
					act( "§A¤£³Ü$T.", ch, NULL, liq_table[liquid].liq_name, TO_CHAR );
					break;
				}
				
				²¾°£³Ü fountain ª½±µ§â thirst ¸É¨ìº¡ªº³]©w remove at 19/12/22
				if ( !IS_NPC( ch ) ) ch->pcdata->condition[COND_THIRST] = 48;

				act( "§A±q $p ¸Ì³Ü¤F´X¤f$T.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );
				act( "$n ±q $p ¸Ì³Ü¤F´X¤f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
			*/
		// §â©Ò¦³ Condition ±q 48 ­×§ï¬°³Ì¤j­È 100  2023/01/13
		if( !IS_NPC(ch) 
			&& liq_table[liquid].liq_affect[COND_DRUNK] == 0 
			&& ch->race != race_lookup( "vampire" ) )
		{
			// §P©w¤ñ¸û¸Ñ´÷ªº¶¼®Æ
			if( liq_table[liquid].liq_affect[COND_THIRST] > liq_table[liquid].liq_affect[COND_FULL]
				&& liq_table[liquid].liq_affect[COND_THIRST] > 0 )
			{
				// ¤£´÷¥B¶¼®Æ¥i¥H¹¡¨¬ªº±¡ªp
				if( ch->pcdata->condition[COND_THIRST] >= 40 && liq_table[liquid].liq_affect[COND_FULL] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			// §P©w¤ñ¸û¹¡¨¬ªº¶¼®Æ
			else if( liq_table[liquid].liq_affect[COND_THIRST] < liq_table[liquid].liq_affect[COND_FULL]
					&& liq_table[liquid].liq_affect[COND_FULL] > 0)
			{
				// ¤£¾j¥B¶¼®Æ¥i¥H¸Ñ´÷ªº±¡ªp
				if( ch->pcdata->condition[COND_FULL] >= 40 && liq_table[liquid].liq_affect[COND_THIRST] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
			}
			// ¸Ñ´÷¹¡¨¬¬Û¦Pªº¶¼®Æ
			else if( liq_table[liquid].liq_affect[COND_THIRST] > 0 && liq_table[liquid].liq_affect[COND_FULL] > 0 )
			{
				if( ch->pcdata->condition[COND_THIRST] > ch->pcdata->condition[COND_FULL] )
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			else
			{
				// ¨ã¦³¤f´÷©ÎÄÈ¾j®ÄªGªº¶¼®Æ
				amount = number_range( 1, 2 );
			}
		}
		else
		{
			// °sºë¶¼®Æ©Î§l¦å°­
			amount = number_range( 1, 2 );
		}

		if( obj->value[1] != -1 )
			amount = UMIN( amount, obj->value[1] );


        if( amount > 1){
            act( "§A±q $p ¸Ì³Ü¤F´X¤f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n ±q $p ¸Ì³Ü¤F´X¤f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }else{
            act( "§A±q $p ¸Ì³Ü¤F¤f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n ±q $p ¸Ì³Ü¤F¤f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }

		// ±o¨ì°sºëªºª¬ªp
        gain_condition( ch, COND_DRUNK, amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );

		// ±o¨ì¹¡¨¬¤Î¸Ñ´÷ªº±¡ªp
        if ( ch->race != race_lookup( "vampire" ) )
        {
            gain_condition( ch, COND_FULL,   amount * liq_table[liquid].liq_affect[COND_FULL] );
            gain_condition( ch, COND_THIRST, amount * liq_table[liquid].liq_affect[COND_THIRST] );
        }
        else if ( liquid == 13 ) // blood
        {
			// §P©w vampire ³Ü¦å
            if( !IS_NPC(ch) )
            {
                amount = UMAX( 1 , /*48*/ 100 - (ch->pcdata->condition[COND_THIRST]) );
                if( obj->value[1] != -1 )
                    amount = UMIN( amount, obj->value[1] );
            }
			amount = amount * 4;
            gain_condition( ch, COND_FULL, amount );
            gain_condition( ch, COND_THIRST, amount );
        }

		/* ±Ô­z§ï¥Ñ gain_condition Åã¥Ü 2023/01/13

		if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK ] > 10 )
			send_to_char( "§A·P¨ì¦³¨Ç¾K¤F.\n\r", ch );

		if ( ch->race != race_lookup( "vampire" ) )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "§A¹¡¤F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "§A¤£¦A·P¨ì¤f´÷.\n\r", ch );
        }
		else if ( liquid == 13 )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "§A¹¡¤F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "§A¤£¦A·P¨ì¤f´÷.\n\r", ch );
		} */

		/* ¥[¤J mob no_poison §P©w¤Î race_undead ¤¤¬r 2021/11/04
        if (obj->value[3] != 0
            && (   ch->race != race_lookup( "Vampire" )
                && ch->race != race_lookup( "Undead" ) 
                && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ))
		*/

		if( obj->value[3] != 0 ){
			/* The shit was poisoned! */
			AFFECT_DATA af;

			if( IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
				|| IS_SET( ch->special, NO_POISON ) )
			{
				// undead ¤¤¬r§P©w 2021/11/04
				// §@¥Î AFF_GHOUL µ¥¦P©óÅÜ¬° undead 2022/01/16
				if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( ch, AFF_GHOUL ) ){
					send_to_char( "¶¼®Æ¤¤ªº¬r¯À«I¤J¤F§AªºÅé¤º¡C\n\r", ch );
					af.type  = gsn_poison;
					af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
					af.location  = APPLY_SAVING_POISON;
					af.modifier  = amount;
					af.bitvector = AFF_POISON;
					affect_join( ch, &af );
				}
			}else{
				send_to_char( "§A·P¨ì¤£¤Ó¹ï«l¡A¬ðµM¼@¯P¦a«y¤F°_¨Ó¡I\n\r", ch );
				act( "$n ªí±¡¤£¤Ó¹ï«l¡A¬ðµM¼@¯P¦a«y¤F°_¨Ó¡I", ch, NULL, NULL, TO_ROOM );
				af.type  = gsn_poison;
				af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
				af.location  = APPLY_SAVING_POISON;
				af.modifier  = amount;
				af.bitvector = AFF_POISON;
				affect_join( ch, &af );
			}
		}
		if( obj->value[1] != -1 )
		{
			obj->value[1] -= amount;
			obj->value[1] = UMAX( 0, obj->value[1]);
		}

        if ( obj->value[1] == 0 )
        {
            act( "§A§â $p ³Ü°®¤F.", ch, obj, NULL, TO_CHAR );
        }

		// add at 19/12/22
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("drink", ch, obj);
		}
        break;

	// §â©Ò¦³ Condition ±q 48 ­×§ï¬°³Ì¤j­È 100  2023/01/13
    case ITEM_DRINK_CON:

        if ( obj->value[1] <= 0 && obj->value[1] != -1)
        {
            send_to_char( "¨º¤w¸g³Q³Ü°®¤F.\n\r", ch );
            return;
        }

		if ( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 )
		{
			bug( "Do_drink: bad liquid number %d.", obj->pIndexData->vnum );
			obj->value[2] = 0;
		}
		liquid = obj->value[2];
			/*
				if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
				{
					bug( "Do_drink: bad liquid number %d.", liquid );
					liquid = obj->value[2] = 0;
				}
			*/

		// ­×§ï°s¾Kªº±ø¥ó 2023/01/13
		//if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK] > 36 && liq_table[liquid].liq_affect[COND_DRUNK] > 0)
		if ( !IS_NPC( ch ) )
		{
			// °sºë¤¤¬rªº±¡ªp¤UµLªk¶i­¹ 2023/01/13
			if( ch->pcdata->condition[COND_DRUNK] >= 70 )
			{
				send_to_char( "§A·P¨ì­G¦bªÈµ²¡A®Ú¥»µLªk¶i­¹...\n\r", ch );
				return;
			}
			// ¶V¾K¶V³Ü¤£¨ì°s
			if( ch->pcdata->condition[COND_DRUNK] >= 20 && liq_table[liquid].liq_affect[COND_DRUNK] > 0 )
			{
				if( ch->pcdata->condition[COND_DRUNK] < number_percent() )
				{
					send_to_char( "«x¡H«ç»ò¦Ñ¬O³Ü¤£¨ì°Ú...  *§c*\n\r", ch );
					return;
				}
			}
		}

		if( !IS_NPC(ch) 
			&& liq_table[liquid].liq_affect[COND_DRUNK] == 0 
			&& ch->race != race_lookup( "vampire" ) )
		{
			if( liq_table[liquid].liq_affect[COND_THIRST] > liq_table[liquid].liq_affect[COND_FULL]
				&& liq_table[liquid].liq_affect[COND_THIRST] > 0 )
			{
				if( ch->pcdata->condition[COND_THIRST] >= 40 && liq_table[liquid].liq_affect[COND_FULL] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			else if( liq_table[liquid].liq_affect[COND_THIRST] < liq_table[liquid].liq_affect[COND_FULL]
					&& liq_table[liquid].liq_affect[COND_FULL] > 0)
			{
				if( ch->pcdata->condition[COND_FULL] >= 40 && liq_table[liquid].liq_affect[COND_THIRST] > 0)
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
			}
			else if( liq_table[liquid].liq_affect[COND_THIRST] > 0 && liq_table[liquid].liq_affect[COND_FULL] > 0 )
			{
				if( ch->pcdata->condition[COND_THIRST] > ch->pcdata->condition[COND_FULL] )
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_FULL])) / liq_table[liquid].liq_affect[COND_FULL] );
				else
					amount = UMAX( 1 , (/*48*/ 100 - (ch->pcdata->condition[COND_THIRST])) / liq_table[liquid].liq_affect[COND_THIRST] );
			}
			else
			{
				amount = number_range( 1, 2 );
			}
		}
		else
		{
			amount = number_range( 1, 2 );
		}

        if( obj->value[1] != -1 )
            amount = UMIN( amount, obj->value[1] );

        if( amount > 1){
            act( "§A±q $p ¸Ì³Ü¤F´X¤f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n ±q $p ¸Ì³Ü¤F´X¤f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }else{
            act( "§A±q $p ¸Ì³Ü¤F¤f$T.",  ch, obj, liq_table[liquid].liq_name, TO_CHAR );
            act( "$n ±q $p ¸Ì³Ü¤F¤f$T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
        }

        gain_condition( ch, COND_DRUNK, amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
        if ( ch->race != race_lookup( "vampire" ) )
        {
            gain_condition( ch, COND_FULL,   amount * liq_table[liquid].liq_affect[COND_FULL   ] );
            gain_condition( ch, COND_THIRST, amount * liq_table[liquid].liq_affect[COND_THIRST ] );
        }
        else if ( liquid == 13 )
        {
            if( !IS_NPC(ch) )
            {
                amount = UMAX( 1 , /*48*/ 100 - (ch->pcdata->condition[COND_THIRST]) );
                if( obj->value[1] != -1 )
                    amount = UMIN( amount, obj->value[1] );
            }
            gain_condition( ch, COND_FULL, amount );
            gain_condition( ch, COND_THIRST, amount );
        }

		/* ±Ô­z§ï¥Ñ gain_condition Åã¥Ü 2023/01/13

		if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_DRUNK ] > 10 )
			send_to_char( "§A·P¨ì¦³¨Ç¾K¤F.\n\r", ch );

		if ( ch->race != race_lookup( "vampire" ) )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "§A¹¡¤F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "§A¤£¦A·P¨ì¤f´÷.\n\r", ch );
        }
		else if ( liquid == 13 )
		{
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_FULL  ] > 99 )
				send_to_char( "§A¹¡¤F.\n\r", ch );
			if ( !IS_NPC( ch ) && ch->pcdata->condition[COND_THIRST] > 99 )
				send_to_char( "§A¤£¦A·P¨ì¤f´÷.\n\r", ch );
		} */

		/* ¥[¤J mob no_poison §P©w¤Î race_undead ¤¤¬r 2021/11/04
        if (obj->value[3] != 0
            && (   ch->race != race_lookup( "Vampire" )
                && ch->race != race_lookup( "Undead" ) 
                && !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ))
		*/
		if( obj->value[3] != 0 ){
			/* The shit was poisoned! */
			AFFECT_DATA af;

			if( IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
				|| IS_SET( ch->special, NO_POISON ) )
			{
				// undead ¤¤¬r§P©w 2021/11/04
				// §@¥Î AFF_GHOUL µ¥¦P©óÅÜ¬° undead 2022/01/16
				if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( ch, AFF_GHOUL ) ){
					send_to_char( "¶¼®Æ¤¤ªº¬r¯À«I¤J¤F§AªºÅé¤º¡C\n\r", ch );
					af.type  = gsn_poison;
					af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
					af.location  = APPLY_SAVING_POISON;
					af.modifier  = amount;
					af.bitvector = AFF_POISON;
					affect_join( ch, &af );
				}
			}else{
				send_to_char( "§A·P¨ì¤£¤Ó¹ï«l¡A¬ðµM¼@¯P¦a«y¤F°_¨Ó¡I\n\r", ch );
				act( "$n ªí±¡¤£¤Ó¹ï«l¡A¬ðµM¼@¯P¦a«y¤F°_¨Ó¡I", ch, NULL, NULL, TO_ROOM );
				af.type  = gsn_poison;
				af.duration  = UMAX( 0 , amount - get_curr_con(ch) );
				af.location  = APPLY_SAVING_POISON;
				af.modifier  = amount;
				af.bitvector = AFF_POISON;
				affect_join( ch, &af );
			}
		}
		// add at 19/12/22
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("drink", ch, obj);
		}

        if( obj->value[1] != -1 ) {
            obj->value[1] -= amount;
            obj->value[1] = UMAX( 0, obj->value[1]);
        }
		if ( obj->value[1] == 0 )
		{
			// add at 19/12/22
			if( IS_SET(obj->pIndexData->souvenir, Z4)
				|| obj->value[0] >= 50 )
			{
				send_to_char( "§A§â¥¦³Ü¥ú¤F.\n\r", ch );
			}
			else
			{
				send_to_char( "§A³Ü¥ú¤§«á¡A¶¶¤â§â¥¦µ¹¥á¤F.\n\r", ch );
				extract_obj( obj );
			}
		}
		break;
    }

    return;
}



void do_eat( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );
	if ( arg[0] == '\0' ){
		send_to_char( "­n¦Y¤°»ò¡H\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) ){
		send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
		return;
	}

	if ( !IS_IMMORTAL( ch ) )
	{
		if( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL
			&& !( IS_SET( race_table[ ch->race ].race_abilities, RACE_ORC )
			&& obj->item_type == ITEM_CORPSE_NPC ) )
		{
			send_to_char( "¨º¬Ý°_¨Ó¤£¯à¦Y.\n\r", ch );
			return;
		}

		if( !IS_NPC( ch )
			&& obj->item_type != ITEM_PILL
			&& obj->item_type != ITEM_CORPSE_NPC
			&& ch->pcdata->condition[COND_FULL] >= 100 )
		{
			send_to_char( "§A¦A¤]¦Y¤£¤U¤F.\n\r", ch );
			return;
		}
	}

	switch ( obj->item_type )
	{

		case ITEM_FOOD:
			if ( !IS_NPC( ch ) )
			{
				// °sºë¤¤¬rªº±¡ªp¤UµLªk¶i­¹ 2023/01/13
				if( ch->pcdata->condition[COND_DRUNK] >= 70 )
				{
					send_to_char( "§A·P¨ì­G¦bªÈµ²¡A®Ú¥»µLªk¶i­¹...\n\r", ch );
					return;
				}
				act( "§A¦Y¤U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¦Y¤U $p.", ch, obj, NULL, TO_ROOM );
				int condition;

				condition = ch->pcdata->condition[COND_FULL];
				if ( ch->race != race_lookup( "vampire" ) )
					gain_condition( ch, COND_FULL, obj->value[0] );

				/* ±Ô­z§ï¥Ñ gain_condition Åã¥Ü 2023/01/13

				if ( ch->pcdata->condition[COND_FULL] > 40 )
					send_to_char( "§A¦Y¹¡¤F.\n\r", ch );
				else if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
					send_to_char( "§A¤£¦A·P¨ì°§¾j.\n\r", ch ); */
			}

			if( IS_NPC(ch) )
			{
				act( "§A¦Y¤U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¦Y¤U $p.", ch, obj, NULL, TO_ROOM );
			}

			/* ¥[¤J mob no_poison §P©w¤Î race_undead ¤¤¬r 2021/11/04
			if (   obj->value[3] != 0
				&& ch->race != race_lookup( "Vampire" )
				&& ch->race != race_lookup( "Undead" ) 
				&& !IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON ) ){
			*/
			if( obj->value[3] != 0 ){
				/* The shit was poisoned! */

				if( IS_SET( race_table[ch->race].race_abilities, RACE_NO_POISON )
					|| IS_SET( ch->special, NO_POISON ) )
				{
					// undead ¤¤¬r§P©w 2021/11/04
					// §@¥Î AFF_GHOUL µ¥¦P©óÅÜ¬° undead 2022/01/16
					if( IS_SET( race_table[ch->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( ch, AFF_GHOUL ) ){
						send_to_char( "­¹ª«¤¤ªº¬r¯À«I¤J¤F§AªºÅé¤º¡C\n\r", ch );
						af.type  = gsn_poison;
						af.duration  = UMAX( 0 , obj->value[0] - get_curr_con(ch)*2 );
						af.location  = APPLY_SAVING_POISON;
						af.modifier  = obj->value[0];
						af.bitvector = AFF_POISON;
						affect_join( ch, &af );
					}
				}else{
					send_to_char( "§A·P¨ì¤£¤Ó¹ï«l¡A¬ðµM¼@¯P¦a«y¤F°_¨Ó¡I\n\r", ch );
					act( "$n ªí±¡¤£¤Ó¹ï«l¡A¬ðµM¼@¯P¦a«y¤F°_¨Ó¡I", ch, NULL, NULL, TO_ROOM );
					af.type  = gsn_poison;
					af.duration  = UMAX( 0 , obj->value[0] - get_curr_con(ch)*2 );
					af.location  = APPLY_SAVING_POISON;
					af.modifier  = obj->value[0];
					af.bitvector = AFF_POISON;
					affect_join( ch, &af );
				}
			}
			if( obj->value[1] > 0 && obj->value[1] <= 49 && ch->level >= obj->level){
				affect_strip(ch, gsn_satiety);
				send_to_char("«~¹Á¬ü¨ýªº­¹ª«Åý§A¤ß¹Ò²£¥Í¤FÅÜ¤Æ¡C\n\r", ch);
				af.type      = gsn_satiety;
				af.duration  = UMAX(1 , obj->value[0]/2);
				af.location  = obj->value[1];
				af.modifier  = obj->value[2];
				af.bitvector = 0;
				affect_to_char( ch, &af );
			}
			break;
		case ITEM_PILL:
			if ( obj->level > ch->level ){
				act( "§A¤£¯à¦Yµ¥¯Å¤ñ§A°ªªºÃÄ¤Y.", ch, obj, NULL, TO_CHAR );
				return;
			}else{
				act( "§A¦Y¤U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¦Y¤U $p.", ch, obj, NULL, TO_ROOM );
				obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
				obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
				obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
				if (    !IS_NPC( ch )
					|| ( IS_NPC( ch ) && is_affected( ch, gsn_charm_person ) ) ){
					if( ch->fighting )
						WAIT_STATE( ch, 1 );
					else
						WAIT_STATE( ch, 2);
				}else{
					WAIT_STATE(ch, 2);
				}
			}
			break;
		case ITEM_CORPSE_NPC:
			if ( !IS_NPC( ch ) )
			{
				// °sºë¤¤¬rªº±¡ªp¤UµLªk¶i­¹ 2023/01/13
				if( ch->pcdata->condition[COND_DRUNK] >= 70 )
				{
					send_to_char( "§A·P¨ì­G¦bªÈµ²¡A®Ú¥»µLªk¶i­¹...\n\r", ch );
					return;
				}

				act( "§A¦Y¤U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¦Y¤U $p.", ch, obj, NULL, TO_ROOM );

				int condition = ch->pcdata->condition[COND_FULL];
				int nSklv     = get_skill_level(ch, gsn_inborn_ability);

				if ( ch->race != race_lookup( "vampire" ) )
					gain_condition( ch, COND_FULL, obj->value[0] );

				/* ±Ô­z§ï¥Ñ gain_condition Åã¥Ü 2023/01/13

				if ( ch->pcdata->condition[COND_FULL] > 40 )
					send_to_char( "§A¦Y¹¡¤F.\n\r", ch );
				else if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
					send_to_char( "§A¤£¦A·P¨ì°§¾j.\n\r", ch ); */

				if ( (obj->level >= ch->level + 2 - nSklv || obj->level > 37 - nSklv / 3) && obj->level < ch->level * 4){
					int nBonusHR = ( obj->level + nSklv - ch->level ) / (11 - nSklv);
					int nBonusDR = ( obj->level + nSklv - ch->level ) / ( 9 - nSklv);
					if(nBonusHR < 1) nBonusHR = 1;
					if(nBonusDR < 2) nBonusDR = 2;

					nBonusHR += nSklv * 2 / 3;
					nBonusDR += nSklv;

					if (nSklv > 3) {
					  nBonusDR += (nSklv - 1) * 2;
					  nBonusHR += (nSklv - 1);
					}

					if(nBonusHR > 2 + nSklv * (nSklv + 4) / 6) nBonusHR = 2 + nSklv * (nSklv + 4) / 6;
					if(nBonusDR > 5 + nSklv * (nSklv + 4) / 3) nBonusDR = 5 + nSklv * (nSklv + 4) / 3;

					affect_strip(ch, gsn_excited);
					send_to_char("§A·P¨ì¥þ¨­³£¥Rº¡¤F¤O¶q¡C\n\r", ch);
					af.type      = gsn_excited;
					af.duration  = 2 + nSklv * nSklv / 6;
					af.location  = APPLY_HITROLL;
					af.modifier  = nBonusHR;
					af.bitvector = 0;
					affect_to_char( ch, &af );

					af.location  = APPLY_DAMROLL;
					af.modifier  = nBonusDR;
					affect_to_char( ch, &af );
				}
			}
			break;
		default:
			if ( IS_IMMORTAL( ch ) ){
				act( "§A¦Y¤U $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¦Y¤U $p.", ch, obj, NULL, TO_ROOM );
			}
	}

	// if ( !IS_NPC( ch )
	// || ( IS_NPC( ch ) && is_affected( ch, gsn_charm_person ) ) )
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("eat", ch, obj);
	}
	if( obj ) // obj may dest self in prog
		extract_obj( obj );
	return;
}


/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
	OBJ_DATA *obj;

	if( !( obj = get_eq_char( ch, iWear ) ) )
		return TRUE;

	if( !fReplace )
		return FALSE;

	if( iWear == WEAR_RIDE && !IS_IMMORTAL(ch) )
		return FALSE;

	if( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) && !IS_IMMORTAL(ch) )
	{
		act( "§AµLªk²æ¤U $p.", ch, obj, NULL, TO_CHAR );
		return FALSE;
	}

	unequip_char( ch, obj );

	//modified by coco 0513
	if( iWear == WEAR_RIDE )
	{
		act( "$n ¸õ¤U $p.", ch, obj, NULL, TO_ROOM );
		act( "§A¸õ¤U $p.", ch, obj, NULL, TO_CHAR );
	}
	else
	{
		act( "$n ²æ¤U $p.", ch, obj, NULL, TO_ROOM );
		act( "§A²æ¤U $p.", ch, obj, NULL, TO_CHAR );
	}
	// ·s¼W§@¥Î¦b½T¹ê²æ¤U¸Ë³Æ®Éªº remove 2020/10/24
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("bremove", ch, obj);
	}
	return TRUE;
}

bool fremove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
	OBJ_DATA *obj;

	if ( !( obj = get_eq_char( ch, iWear ) ) )
		return TRUE;

	if ( !fReplace )
		return FALSE;

	if ( iWear == WEAR_RIDE && !IS_IMMORTAL(ch))
		return FALSE;

	unequip_char( ch, obj );
	//modified by coco 0513
	if( iWear == WEAR_RIDE )
	{
		act( "$n ¸õ¤U $p.", ch, obj, NULL, TO_ROOM );
		act( "§A¸õ¤U $p.", ch, obj, NULL, TO_CHAR );
	}
	else
	{
		act( "$n ²æ¤U $p.", ch, obj, NULL, TO_ROOM );
		act( "§A²æ¤U $p.", ch, obj, NULL, TO_CHAR );
	}
	// ·s¼W§@¥Î¦b½T¹ê²æ¤U¸Ë³Æ®Éªº remove 2020/10/24
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("bremove", ch, obj);
	}
	return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
typedef struct wear_map {
    char *name;
    int place;
} WEAR_MAP;



void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace, int iWear )
{
	char buf [ MAX_STRING_LENGTH ];
	bool    fForce = FALSE;
	int     nSklv  = get_skill_level(ch, gsn_inborn_ability);
	int     min_str, str_diff;

	if( iWear != 0 ) fForce = TRUE;

	if ( iWear == WEAR_RIDE && !IS_IMMORTAL(ch) )
		return;
	/*
	static const   WEAR_MAP wear_table[17]= {
	{   "about",      ITEM_WEAR_ABOUT},
	{   "arm",      ITEM_WEAR_ARMS},
	{   "arrow",      ITEM_WEAR_ARROW},
	{   "body",     ITEM_WEAR_BODY },
	{   "feet",     ITEM_WEAR_FEET },
	{   "finger",     ITEM_WEAR_FINGER },
	{   "hand",     ITEM_WEAR_HANDS},
	{   "head",     ITEM_WEAR_HEAD },
	{   "hold",     ITEM_HOLD},
	{   "leg",      ITEM_WEAR_LEGS },
	{   "neck",     ITEM_WEAR_NECK },
	{   "shield",     ITEM_WEAR_SHIELD},
	{   "twohand",      ITEM_TWO_HAND},
	{   "waist",      ITEM_WEAR_WAIST},
	{   "weapon",     ITEM_WIELD},
	{   "wrist",      ITEM_WEAR_WRIST},
	{   "ride",         ITEM_WEAR_RIDE},
	};
	*/
	if ( obj->item_type != ITEM_LIGHT && !CAN_WEAR(obj, ITEM_WEAR_BODY) && 
		 !CAN_WEAR(obj, ITEM_WEAR_SHIELD) && !CAN_WEAR(obj, ITEM_WIELD) && 
		 !CAN_WEAR(obj, ITEM_TWO_HAND) && !CAN_WEAR(obj, ITEM_WEAR_HEAD)){
		if(ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)){    
			act( "$n ¸ÕµÛ¬ï¤W $p, ¥i±¤¬Ý¨Ó¨Ã¤£¦X¾A.", ch, obj, NULL, TO_ROOM );
			return;
		}
	}

	if ( ch->level < obj->level && !(IS_NPC(ch) && obj->level < 42)){
		sprintf( buf, "§A¥²¶·¨ìµ¥¯Å %d ¤~¯à¨Ï¥Î%s.\n\r",
		obj->level , obj->short_descr);
		send_to_char( buf, ch );
		act( "$n ¸ÕµÛ¬ï¤W $p, ¥i±¤¬Ý¨Ó¸gÅç¤£¨¬.",
			 ch, obj, NULL, TO_ROOM );
		return;
	}
	if ( obj->pIndexData->max_level > 0 && ch->level > obj->pIndexData->max_level ){
		sprintf( buf, "§A¥²¶·¦bµ¥¯Å %d ¥H¤º¤~¯à¨Ï¥Î³o¼Ëª««~.\n\r",
		obj->pIndexData->max_level );
		send_to_char( buf, ch );
		return;
	}

	if( !IS_NPC( ch ) || (ch->pIndexData != NULL && IsSorGateMob(ch->pIndexData->vnum)) ){
		if( IS_SET(obj->pIndexData->souvenir, Z1) && !IS_IMMORTAL( ch ) ){
			send_to_char( "¦¹ª««~¤D¬ö©À«~¡A¥u¨Ñ¦¬ÂÃ¡A¤£¯à¬ïÀ¹¡I\n\r", ch );
			return;
		}
		if( IS_SET(obj->pIndexData->souvenir, Z2) && !IS_IMMORTAL( ch ) ){
			send_to_char( "¦¹ª««~¬°Mob ±M¥Î¡I\n\r", ch );
			act( "$n ¸ÕµÛ¬ï¤W $p, ¥i±¤¬Ý¨Ó¨Ã¤£¦X¾A.", ch, obj, NULL, TO_ROOM );
			return;
		}
	}

	if( IS_SET(obj->extra_flags, ITEM_LORE)){
		OBJ_DATA *pObj;
		int vnum = obj->pIndexData->vnum;
		for( pObj = ch->carrying ; pObj ; pObj = pObj->next_content ) {
			if( pObj->pIndexData->vnum == vnum && pObj->wear_loc != WEAR_NONE){
				send_to_char("§A¨­¤W¤w¸g¬ïµÛ¤@¥ó¤F¡C\n\r", ch);
				return;
			}
		}
	}

	if( !fForce ) {
		if ( obj->item_type == ITEM_LIGHT ){
			if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
				return;
			act( "§AÂI«G¤F $p ¨Ã¥B¥Î¤â´¤¦í¥¦.",  ch, obj, NULL, TO_CHAR );
			act( "$n ÂI«G¤F $p ¨Ã¥B¥Î¤â´¤¦í¥¦.", ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_LIGHT );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
				if (    get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
					 return;
			}else{
				if (    get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&&  get_eq_char( ch, WEAR_FINGER_3 )
					&&  get_eq_char( ch, WEAR_FINGER_4 )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_3, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_4, fReplace ) )
					return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_L ) ){
				act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_R ) ){
				act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_R );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_3 ) ){
				act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_3 );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_4 ) ){
				act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_4 );
				return;
			}

			bug( "Wear_obj: no free finger.", 0 );
			send_to_char( "§A¤w¸gÀ¹º¡¨â¥u§Ù«ü¤F.\n\r", ch );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) ){
			if (    get_eq_char( ch, WEAR_NECK_1 )
				&&  get_eq_char( ch, WEAR_NECK_2 )
				&& !remove_obj( ch, WEAR_NECK_1, fReplace )
				&& !remove_obj( ch, WEAR_NECK_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_NECK_1 ) ){
				act( "§A ¬ïÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¬ïÀ¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_1 );
				return;
			}

			if ( !get_eq_char( ch, WEAR_NECK_2 ) ){
				act( "§A ¬ïÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¬ïÀ¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_2 );
				return;
			}

			bug( "Wear_obj: no free neck.", 0 );
			send_to_char( "§AªºÀV¤l¤W¬ïÀ¹¤£¤U§ó¦hªF¦è¤F.\n\r", ch );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WEAR_BODY ) 
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ) {
			  if (strstr(obj->name, "robe") ||
				  strstr(obj->name, "cloth") ||
				  strstr(obj->name, "skirt")) {
				send_to_char("³o³s¨­ªø³T¬ï¦b§A¨­¤W¨Ã¤£¦X¨­.\n\r", ch);
				act("$n ¸ÕµÛ¬ï¤W $p, ¥i¬O«o®M¤£¤U¥h.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			  if (get_skill_level(ch, gsn_inborn_ability) < 6) {
				act("§A¸ÕµÛ¬ï¤W$p¦ý¬O«o¬ï¤£¤W¥h, ¥i¯à¬O§A¤Ñ½á¤Ó®t¤F.", ch, obj, NULL, TO_CHAR);
				act("$n ¸ÕµÛ¬ï¤W $p, ¥i¬O«o·PÄ±´ý¨­¤£ÆF¬¡¦Ó²æ¤U¤F.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			}
			if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
				return;
			act( "§A¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_BODY );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) ){
			if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
				return;
			act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_HEAD );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WEAR_LEGS )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )){
			  if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
				  return;
			  act( "§A¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
			  act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
			  equip_char( ch, obj, WEAR_LEGS );
			  return;
			} else {
			  if (    get_eq_char( ch, WEAR_LEGS )
				  &&  get_eq_char( ch, WEAR_LEGS_2 )
				  && !remove_obj( ch, WEAR_LEGS, fReplace )
				  && !remove_obj( ch, WEAR_LEGS_2, fReplace ) )
				return;

			  if ( !get_eq_char( ch, WEAR_LEGS ) ) {
				  act( "§A ¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
				  act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
				  equip_char( ch, obj, WEAR_LEGS );
				return;
			  }

			  if ( !get_eq_char( ch, WEAR_LEGS_2 ) ) {
				  act( "§A ¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
				  act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
				  equip_char( ch, obj, WEAR_LEGS_2 );
				return;
			  }

			  send_to_char( "§Aªº»L¤w¸g¬ïº¡»L¥Ò¤F.\n\r", ch );
			  return;
			}
		}

		if (     CAN_WEAR( obj, ITEM_WEAR_FEET )  
			 && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )){
			  if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
				  return;
			  act( "§A¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
			  act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
			  equip_char( ch, obj, WEAR_FEET );
			  return;
			} else {
				if (    get_eq_char( ch, WEAR_FEET )
					&&  get_eq_char( ch, WEAR_FEET_2 )
					&& !remove_obj( ch, WEAR_FEET, fReplace )
					&& !remove_obj( ch, WEAR_FEET_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_FEET ) ){
					act( "§A ¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FEET );
					return;
				}

				if ( !get_eq_char( ch, WEAR_FEET_2 ) ){
					act( "§A ¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FEET_2 );
					return;
				}

				send_to_char( "§Aªº¸}¤w¸g¬ïº¡¹u¤l¤F.\n\r", ch );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 5){
				if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
					return;
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS );
				return;
			}else{ // Beholder with Inborn Ability higher or equal to Sklv 5, added by Keelar
				if (    get_eq_char( ch, WEAR_HANDS )
					&&  get_eq_char( ch, WEAR_HANDS_2 )
					&& !remove_obj( ch, WEAR_HANDS, fReplace )
					&& !remove_obj( ch, WEAR_HANDS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_HANDS ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_HANDS_2 ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS_2 );
					return;
				}

				bug( "Wear_obj: no free gloves for beholder with sklv higher than 4.", 0 );
				send_to_char( "§Aªº¤â¤w¸gÀ¹º¡¤â®M¤F.\n\r", ch );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) ){
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 6){
				if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
					return;
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_ARMS );
				return;
			}else{ // for beholder with inborn ability higher than or equal to 6, added by Keelar
				if (    get_eq_char( ch, WEAR_ARMS )
					&&  get_eq_char( ch, WEAR_ARMS_2 )
					&& !remove_obj( ch, WEAR_ARMS, fReplace )
					&& !remove_obj( ch, WEAR_ARMS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_ARMS ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_ARMS_2 ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS_2 );
					return;
				}

				bug( "Wear_obj: no free sleeves for beholder with sklv higher than 5.", 0 );
				send_to_char( "§Aªº¤âÁu¤w¸g¬ïº¡¤F.\n\r", ch );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) ) {
			if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
				return;
			if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ) {
			  if (strstr(obj->name, "robe") ||
				  strstr(obj->name, "cloth") ||
				  strstr(obj->name, "skirt")) {
				send_to_char("³o³s¨­ªø³T¬ï¦b§A¨­¤W¨Ã¤£¦X¨­.\n\r", ch);
				act("$n ¸ÕµÛ¬ï¤W $p, ¥i¬O«o®M¤£¤U¥h.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			  if (get_skill_level(ch, gsn_inborn_ability) < 5) {
				act("§A¤£ª¾¹D«ç»ò§å¤W$p¤~Åý§A·P¨ì¦æ°Ê¦Û¦p, ¥i¯à¬O§A¤Ñ½á¤Ó®t¤F.", ch, obj, NULL, TO_CHAR);
				act("$n ¸ÕµÛ§å¤W $p, ¥i¬O«o·PÄ±´ý¨­¤£ÆF¬¡¦Ó²æ¤U¤F.", ch, obj, NULL, TO_ROOM);
				return;
			  }
			}
			act( "§A©Ü¤W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ©Ü¤W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ABOUT );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WEAR_WAIST )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
			  if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
				  return;
			  act( "§AÃ´¤W $p.", ch, obj, NULL, TO_CHAR );
			  act( "$n Ã´¤W $p.",   ch, obj, NULL, TO_ROOM );
			  equip_char( ch, obj, WEAR_WAIST );
			  return;
			} else {
			  send_to_char("§A§ä¤£¨ì¦a¤èÃ´¤W¸y±a.\n\r", ch);
			  act("$n ¸ÕµÛÃ´¤W $p, ¥i¬O«o§ä¤£¨ì¦X¾Aªº¦a¤è.", ch, obj, NULL, TO_ROOM);
			  return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) ){
			if (    get_eq_char( ch, WEAR_WRIST_L )
				&&  get_eq_char( ch, WEAR_WRIST_R )
				&& !remove_obj( ch, WEAR_WRIST_L, fReplace )
				&& !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_WRIST_L ) ){
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_WRIST_R ) ){
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_R );
				return;
			}

			bug( "Wear_obj: no free wrist.", 0 );
			send_to_char( "§AÂù¤â¤âµÃ³£À¹µÛªF¦è.\n\r", ch );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) ){
			// ¼W¥[ Shield ´î¶Ë­«¶q±Ô­z 2018.11.27
			int ShieldWt = get_obj_weight( obj );
			int RequireStr = 1 + get_curr_str( ch ) + 2 * get_skill_level(ch, gsn_block );

			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			act( "$n ®³°_ $p §@¬°¬ÞµP.", ch, obj, NULL, TO_ROOM );
			act( "§A®³°_ $p §@¬°¬ÞµP.", ch, obj, NULL, TO_CHAR );
			if( RequireStr < ShieldWt / 4 ) {
				act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«, ¨Ï¥Î¤WµLªk±o¤ßÀ³¤â.", ch, obj, NULL, TO_CHAR );
			}
			equip_char( ch, obj, WEAR_SHIELD );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_WIELD )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			if( IS_NPC( ch ) || get_skill_percent( ch, gsn_dual ) != 0 )
			{
			  /*
			   * If you think this looks ugly now, just imagine how
			   * I felt trying to write it!  --- Thelonius (Monk)
			   * Now, it will attempt to wear in 1 if open, then 2 if
			   * open, else replace 2, else replace 1.
			   */
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
					|| ( get_eq_char( ch, WEAR_WIELD  )
					&& ( ( get_eq_char( ch, WEAR_WIELD_2 )
					&& !remove_obj( ch, WEAR_WIELD_2, fReplace ) ) //WEAR_HOLD & WEAR_SHIELD
					|| !remove_obj( ch, WEAR_HOLD,    fReplace )     //is at the same position of
					|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) )   //of WEAR_WIELD_2
					&&  !remove_obj( ch, WEAR_WIELD, fReplace ) ) )
					return;

				if ( !get_eq_char( ch, WEAR_WIELD ) ){
		
					int weight = get_obj_weight( obj );
		
					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
					act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
					}
					equip_char( ch, obj, WEAR_WIELD );
					return;
				}
		
				if ( !get_eq_char( ch, WEAR_WIELD_2 ) ){

					int weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield2 >= weight)
						  break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
					act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
					}
					equip_char( ch, obj, WEAR_WIELD_2 );
					return;
				}
				bug( "Wear_obj: no free weapon slot.", 0 );
				send_to_char( "§AÂù¤â³£´¤µÛªZ¾¹¤F.\n\r", ch );
				return;
			}else{  /* can only wield one weapon */
				int weight = get_obj_weight( obj );
				if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
					return;
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace ) )
					return;
		
				for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
					// ¥N¤Jªº¦ì¸m¤£¥¿½T , ¦¹³BÀ³¬° .wield
					//if(str_app[min_str].twohand >= weight)
					if(str_app[min_str].wield >= weight)
						break;
				}
				str_diff = get_curr_str( ch ) - min_str;

				int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
				int nPbSklv  = get_skill_level(ch, gsn_powerblow);
				if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

				act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
				act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
					else
						act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
				}
				equip_char( ch, obj, WEAR_WIELD );
				return;
			}
		}

		if ( CAN_WEAR( obj, ITEM_HOLD ) ){
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_HOLD,    fReplace ) )
				return;
			act( "§A´¤¦í $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ´¤¦í $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_HOLD );
			return;
		}

		if (    CAN_WEAR( obj, ITEM_TWO_HAND )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
			int weight = get_obj_weight( obj );
			if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;

			for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
				if(str_app[min_str].twohand >= weight)
					break;
			}
			str_diff = get_curr_str( ch ) - min_str;

			int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
			int nPbSklv  = get_skill_level(ch, gsn_powerblow);
			if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

			act( "§A¥ÎÂù¤â´¤¦í $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ¥ÎÂù¤â´¤¦í $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_TWO_HAND );
			if( str_diff < 0 ) {
				if( str_diff < -5 )
					act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
				else
					act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
			}
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WEAR_ARROW ) ){
			if(obj->pIndexData->vnum == OBJ_VNUM_MAGIC_ARROW){
				if(obj->value[3] > 30 + get_skill_level(ch, gsn_magic_arrow) * 20){
				//if(obj->value[3] > get_skill_level(ch, gsn_magic_arrow) * 20){
					act( "§AµLªk±±¨î³o®¹Å]ªk½bªº¤O¶q.", ch, obj, NULL, TO_CHAR );
					return;
				}
			}
			if ( !remove_obj( ch, WEAR_ARROW, fReplace ) )
				return;
			act( "§A§â $p ±×±¾¦b­I¤W.", ch, obj, NULL, TO_CHAR );
			act( "$n §â $p ±×±¾¦b­I¤W.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ARROW );
			return;
		}
		// ¼uÃÄ¸Ë³Æ add at 2022/04/30
		if ( CAN_WEAR( obj, ITEM_WEAR_AMMO ) ){
			if ( !remove_obj( ch, WEAR_AMMO, fReplace ) )
				return;
			act( "§A§â $p ¬ïÀ¹¦b®e©ö¶ñ¸Ëªº¦ì¸m.", ch, obj, NULL, TO_CHAR );
			act( "$n §â $p ¬ïÀ¹¦b®e©ö¶ñ¸Ëªº¦ì¸m.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_AMMO );
			return;
		}
	}else{
		/*
			if ( obj->item_type == ITEM_LIGHT )
			{
				if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
					return;
				act( "§AÂI«G¤F $p ¨Ã¥B¥Î¤â´¤¦í¥¦.",  ch, obj, NULL, TO_CHAR );
				act( "$n ÂI«G¤F $p ¨Ã¥B¥Î¤â´¤¦í¥¦.", ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_LIGHT );
				return;
			}
		*/

		if ( iWear == ITEM_WEAR_FINGER && CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ){
				if (    get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
					return;
			}else{
				if (   get_eq_char( ch, WEAR_FINGER_L )
					&&  get_eq_char( ch, WEAR_FINGER_R )
					&&  get_eq_char( ch, WEAR_FINGER_3 )
					&&  get_eq_char( ch, WEAR_FINGER_4 )
					&& !remove_obj( ch, WEAR_FINGER_L, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_R, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_3, fReplace )
					&& !remove_obj( ch, WEAR_FINGER_4, fReplace ) )
					return;
				if ( !get_eq_char( ch, WEAR_FINGER_3 ) ){
					act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FINGER_3 );
					return;
				}

				if ( !get_eq_char( ch, WEAR_FINGER_4 ) ){
					act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_FINGER_4 );
					return;
				}
			}

			if ( !get_eq_char( ch, WEAR_FINGER_L ) ){
				act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_FINGER_R ) ){ 
				act( "§AÀ¹¤W $p.",  ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",    ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FINGER_R );
				return;
			}


			bug( "Wear_obj: no free finger.", 0 );
			send_to_char( "§A¤w¸gÀ¹º¡§Ù«ü¤F.\n\r", ch );
			return;
		}

		if ( iWear == ITEM_WEAR_NECK && CAN_WEAR( obj, ITEM_WEAR_NECK ) )
		{
			if (    get_eq_char( ch, WEAR_NECK_1 )
				&&  get_eq_char( ch, WEAR_NECK_2 )
				&& !remove_obj( ch, WEAR_NECK_1, fReplace )
				&& !remove_obj( ch, WEAR_NECK_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_NECK_1 ) ){
				act( "§A ¬ïÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¬ïÀ¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_1 );
				return;
			}

			if ( !get_eq_char( ch, WEAR_NECK_2 ) ){
				act( "§A ¬ïÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¬ïÀ¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_NECK_2 );
				return;
			}

			bug( "Wear_obj: no free neck.", 0 );
			send_to_char( "§AªºÀV¤l¤W¬ïÀ¹¤£¤U§ó¦hªF¦è¤F.\n\r", ch );
			return;
		}

		if ( iWear == ITEM_WEAR_BODY && CAN_WEAR( obj, ITEM_WEAR_BODY ) 
            && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
            if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
                return;
            act( "§A¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_BODY );
            return;
        }

		if (iWear == ITEM_WEAR_HEAD && CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
		{
            if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
                return;
            act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_HEAD );
            return;
        }

		if ( iWear == ITEM_WEAR_LEGS && CAN_WEAR( obj, ITEM_WEAR_LEGS )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
            if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
                return;
            act( "§A¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_LEGS );
            return;
        }

		if ( iWear == ITEM_WEAR_FEET && CAN_WEAR( obj, ITEM_WEAR_FEET )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
            if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
                return;
            act( "§A¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
            act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
            equip_char( ch, obj, WEAR_FEET );
            return;
        }

		/*
			//add by coco for WEAR_RIDE
			if ( iWear == ITEM_WEAR_RIDE && CAN_WEAR( obj, ITEM_WEAR_RIDE )   )
			{
				if ( !remove_obj( ch, WEAR_RIDE, fReplace ) )
					return;

				if ( !IS_NPC(ch) && ch->class != KNIGHT && ch->class != LORD )
					return;

				act( "§AÃM¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ÃM¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_RIDE );
				return;
			}   
		*/

		if ( iWear == ITEM_WEAR_HANDS && CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 5){
				if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
					return;
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS );
				return;
			}else{ // Beholder with Inborn Ability higher or equal to Sklv 5, added by Keelar
			
				if (    get_eq_char( ch, WEAR_HANDS )
					&&  get_eq_char( ch, WEAR_HANDS_2 )
					&& !remove_obj( ch, WEAR_HANDS, fReplace )
					&& !remove_obj( ch, WEAR_HANDS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_HANDS ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_HANDS_2 ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_HANDS_2 );
					return;
				}

				bug( "Wear_obj: no free gloves for beholder with sklv higher than 4.", 0 );
				send_to_char( "§Aªº¤â¤w¸gÀ¹º¡¤â®M¤F.\n\r", ch );
				return;
			}
		}

		if ( iWear == ITEM_WEAR_ARMS && CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 6){
				if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
					return;
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_ARMS );
				return;
			}else{ // for beholder with inborn ability higher than or equal to 6, added by Keelar
				if (    get_eq_char( ch, WEAR_ARMS )
					&&  get_eq_char( ch, WEAR_ARMS_2 )
					&& !remove_obj( ch, WEAR_ARMS, fReplace )
					&& !remove_obj( ch, WEAR_ARMS_2, fReplace ) )
					return;

				if ( !get_eq_char( ch, WEAR_ARMS ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS );
					return;
				}

				if ( !get_eq_char( ch, WEAR_ARMS_2 ) ){
					act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
					act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
					equip_char( ch, obj, WEAR_ARMS_2 );
					return;
				}

				bug( "Wear_obj: no free sleeves for beholder with sklv higher than 5.", 0 );
				send_to_char( "§Aªº¤âÁu¤w¸g¬ïº¡¤F.\n\r", ch );
				return;
			}
		}

		if ( iWear == ITEM_WEAR_ABOUT && CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
		{
			if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
				return;
			act( "§A©Ü¤W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ©Ü¤W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ABOUT );
			return;
		}

		if ( iWear == ITEM_WEAR_WAIST && CAN_WEAR( obj, ITEM_WEAR_WAIST )  
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
				return;
			act( "§AÃ´¤W $p.", ch, obj, NULL, TO_CHAR );
			act( "$n Ã´¤W $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_WAIST );
			return;
		}

		if ( iWear == ITEM_WEAR_WRIST && CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
		{
			if ( get_eq_char( ch, WEAR_WRIST_L )
			&&   get_eq_char( ch, WEAR_WRIST_R )
			&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
			&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_WRIST_L )) {
				act( "§AÀ¹¤W $p.",ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_L );
				return;
			}

			if ( !get_eq_char( ch, WEAR_WRIST_R ) ){
				act( "§AÀ¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_WRIST_R );
				return;
			}

			bug( "Wear_obj: no free wrist.", 0 );
			send_to_char( "§AÂù¤â¤âµÃ³£À¹µÛªF¦è.\n\r", ch );
			return;
		}

		if ( iWear == ITEM_WEAR_SHIELD && CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
		{
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			act( "$n ®³°_ $p §@¬°¬ÞµP.", ch, obj, NULL, TO_ROOM );
			act( "§A®³°_ $p §@¬°¬ÞµP.", ch, obj, NULL, TO_CHAR );
			equip_char( ch, obj, WEAR_SHIELD );
			return;
		}
		// ¼uÃÄ¸Ë³Æ add at 2022/04/30
		if ( iWear == ITEM_WEAR_AMMO && CAN_WEAR( obj, ITEM_WEAR_AMMO ) ){
			if ( !remove_obj( ch, WEAR_AMMO, fReplace ) )
				return;
			act( "§A§â $p ¬ïÀ¹¦b®e©ö¶ñ¸Ëªº¦ì¸m.", ch, obj, NULL, TO_CHAR );
			act( "$n §â $p ¬ïÀ¹¦b®e©ö¶ñ¸Ëªº¦ì¸m.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_AMMO );
			return;
		}

		// iWear weapon
		if ( iWear == ITEM_WIELD && CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			if( IS_NPC( ch ) || get_skill_percent(ch, gsn_dual ) != 0 )
			{
			  /*
			   * If you think this looks ugly now, just imagine how
			   * I felt trying to write it!  --- Thelonius (Monk)
			   * Now, it will attempt to wear in 1 if open, then 2 if
			   * open, else replace 2, else replace 1.
			   */
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
					|| ( get_eq_char( ch, WEAR_WIELD  )
					&& ( ( get_eq_char( ch, WEAR_WIELD_2 )
					&& !remove_obj( ch, WEAR_WIELD_2, fReplace ) ) //WEAR_HOLD & WEAR_SHIELD
					|| !remove_obj( ch, WEAR_HOLD,    fReplace )     //is at the same position of
					|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) )   //of WEAR_WIELD_2
					&& !remove_obj( ch, WEAR_WIELD, fReplace ) ) )
					return;
				if ( !get_eq_char( ch, WEAR_WIELD ) )
				{
					int   weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
					act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
					}
					equip_char( ch, obj, WEAR_WIELD );
					return;
				}

				if ( !get_eq_char( ch, WEAR_WIELD_2 ) )
				{
					int   weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield2 >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
					act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
					}
						equip_char( ch, obj, WEAR_WIELD_2 );
						return;
				}
				bug( "Wear_obj: no free weapon slot.", 0 );
				send_to_char( "§AÂù¤â³£´¤µÛªZ¾¹¤F.\n\r", ch );
				return;
			}
			else  /* can only wield one weapon */
			{
				int   weight = get_obj_weight( obj );
				if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
					return;
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace ) )
					return;

				for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
					if(str_app[min_str].wield >= weight)
						break;
				}
				str_diff = get_curr_str( ch ) - min_str;

				int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
				int nPbSklv  = get_skill_level(ch, gsn_powerblow);
				if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

				act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
				act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
					else
						act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
				}
				equip_char( ch, obj, WEAR_WIELD );
				return;
			}
		}

		if ( iWear == ITEM_HOLD && CAN_WEAR( obj, ITEM_HOLD ) )
		{
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_HOLD,    fReplace ) )
				return;
			act( "§A´¤¦í $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ´¤¦í $p.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_HOLD );
			return;
		}

		// iWear twohand
		if ( iWear == ITEM_TWO_HAND //&& (CAN_WEAR( obj, ITEM_TWO_HAND ) || ch->class == MARTIALMAN)
			&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			int   weight = get_obj_weight( obj );
			int min_str;
			if( !CAN_WEAR(obj, ITEM_WIELD) && !CAN_WEAR(obj, ITEM_TWO_HAND)){
				send_to_char("³o¥óª««~¤£¯à§@¬°ªZ¾¹¡C\n\r", ch);
				return;
			}
			if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND,fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
				return;
			for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
				if(str_app[min_str].twohand >= weight)
					break;
			}
			str_diff = get_curr_str( ch ) - min_str;

			int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
			int nPbSklv  = get_skill_level(ch, gsn_powerblow);
			if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

			act( "§A¥ÎÂù¤â´¤¦í $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ¥ÎÂù¤â´¤¦í $p.",   ch, obj, NULL, TO_ROOM );
			if( str_diff < 0 ) {
				if( str_diff < -5 )
					act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
				else
					act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
			}
			equip_char( ch, obj, WEAR_TWO_HAND );
			return;
		}

		if ( iWear == ITEM_WEAR_ARROW && CAN_WEAR( obj, ITEM_WEAR_ARROW ) )
		{
			if ( !remove_obj( ch, WEAR_ARROW, fReplace ) )
				return;
			act( "§A§â $p ±×±¾¦b­I¤W.", ch, obj, NULL, TO_CHAR );
			act( "$n §â $p ±×±¾¦b­I¤W.",   ch, obj, NULL, TO_ROOM );
			equip_char( ch, obj, WEAR_ARROW );
			return;
		}

		// ·s¼WÃöÁä¦r wp1 ¥Î¨Ó¸Ë³Æ¥D­nªZ¾¹ 2022/04/05
		if ( iWear == WEAR_WIELD && CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			/*  ¦¹¬q§P©w¿ù»~ 2022/12/06
			if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
				|| ( ( ( get_eq_char( ch, WEAR_WIELD ) && !remove_obj( ch, WEAR_WIELD, fReplace ) )
					|| !remove_obj( ch, WEAR_HOLD, fReplace )
					|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) ) ) )
				return; */

			if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
				return;
			if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_WIELD ) )
			{
				int   weight = get_obj_weight( obj );

				for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
					if(str_app[min_str].wield >= weight)
						break;
				}
				str_diff = get_curr_str( ch ) - min_str;

				int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
				int nPbSklv  = get_skill_level(ch, gsn_powerblow);
				if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

				act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
				act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
				if( str_diff < 0 ) {
					if( str_diff < -5 )
						act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
					else
						act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
				}
				equip_char( ch, obj, WEAR_WIELD );
				return;
			}
			bug( "Wear_obj: when wear wp , no free weapon slot.", 0 );
			send_to_char( "§AÂù¤â³£´¤µÛªZ¾¹¤F.\n\r", ch );
			return;
		}

		// ·s¼W wp2 ¸Ë³Æ¦¸­nªZ¾¹ 2022/04/05
		if ( iWear == WEAR_WIELD_2 && CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
		{
			if( IS_NPC( ch ) || get_skill_percent(ch, gsn_dual ) != 0 )
			{
				if ( !remove_obj( ch, WEAR_TWO_HAND, fReplace )
					|| ( ( ( get_eq_char( ch, WEAR_WIELD_2 ) && !remove_obj( ch, WEAR_WIELD_2, fReplace ) )
						|| !remove_obj( ch, WEAR_HOLD, fReplace )
						|| !remove_obj( ch, WEAR_SHIELD,  fReplace ) ) ) )
					return;

				if ( !get_eq_char( ch, WEAR_WIELD_2 ) )
				{
					int   weight = get_obj_weight( obj );

					for( min_str = 0; min_str < MAX_STR + 6; ++min_str ){
						if(str_app[min_str].wield2 >= weight)
							break;
					}
					str_diff = get_curr_str( ch ) - min_str;

					int nPbPerc  = get_skill_percent(ch, gsn_powerblow);
					int nPbSklv  = get_skill_level(ch, gsn_powerblow);
					if(nPbPerc > 0) str_diff += 1 + nPbSklv * nPbSklv / 4;

					act( "§A¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_CHAR );
					act( "$n ¸Ë³Æ $p §@ªZ¾¹.", ch, obj, NULL, TO_ROOM );
					if( str_diff < 0 ) {
						if( str_diff < -5 )
							act( "¤Ó­«¤F¡I§A®³¤£°Ê¡I", ch, obj, NULL, TO_CHAR );
						else
							act( "$p ¹ï§A¨Ó»¡¦³ÂI¨I­«.", ch, obj, NULL, TO_CHAR );
					}
						equip_char( ch, obj, WEAR_WIELD_2 );
						return;
				}
				bug( "Wear_obj: when wear wp2 , no free weapon slot.", 0 );
				send_to_char( "§AÂù¤â³£´¤µÛªZ¾¹¤F.\n\r", ch );
				return;
			}else{
				send_to_char( "§AµLªk¸Ë³Æ¦¸­nªZ¾¹.\n\r", ch );
				return;
			}
		}

		// ·s¼W hand1 ¸Ë³Æ¦¸¤â®M 2022/04/05
		if ( iWear == WEAR_HANDS && CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
		{

			if ( get_eq_char( ch, WEAR_HANDS ) && !remove_obj( ch, WEAR_HANDS, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_HANDS ) ){
				act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS );
				return;
			}

			bug( "Wear_obj: hand2 change no free gloves for beholder with sklv higher than 4.", 0 );
			send_to_char( "§Aªº¤â¤w¸gÀ¹º¡¤â®M¤F.\n\r", ch );
			return;
		}

		// ·s¼W hand2 ¸Ë³Æ¦¸¤â®M 2022/04/05
		if ( iWear == WEAR_HANDS_2 && CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) || nSklv < 5)
				return;

			if ( get_eq_char( ch, WEAR_HANDS_2 ) && !remove_obj( ch, WEAR_HANDS_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_HANDS_2 ) ){
				act( "§A À¹¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n À¹¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_HANDS_2 );
				return;
			}

			bug( "Wear_obj: hand2 change no free gloves for beholder with sklv higher than 4.", 0 );
			send_to_char( "§Aªº¤â¤w¸gÀ¹º¡¤â®M¤F.\n\r", ch );
			return;
		}

		// ·s¼W feet1 ¸Ë³Æ¦¸¤â®M 2022/04/05
		if ( iWear == WEAR_FEET && CAN_WEAR( obj, ITEM_WEAR_FEET ) )
		{

			if ( get_eq_char( ch, WEAR_FEET ) && !remove_obj( ch, WEAR_FEET, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_FEET ) ){
				act( "§A ¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FEET );
				return;
			}

			bug( "Wear_obj: feet2 change no free feet for centaur.", 0 );
			send_to_char( "§Aªº¸}¤w¸g¬ïº¡¾c¤l¤F.\n\r", ch );
			return;
		}

		// ·s¼W feet2 ¸Ë³Æ¦¸¸}³¡ 2022/04/05
		if ( iWear == WEAR_FEET_2 && CAN_WEAR( obj, ITEM_WEAR_FEET ) )
		{
			if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) )
				return;

			if ( get_eq_char( ch, WEAR_FEET_2 ) && !remove_obj( ch, WEAR_FEET_2, fReplace ) )
				return;

			if ( !get_eq_char( ch, WEAR_FEET_2 ) ){
				act( "§A ¬ï¤W $p.", ch, obj, NULL, TO_CHAR );
				act( "$n ¬ï¤W $p.",   ch, obj, NULL, TO_ROOM );
				equip_char( ch, obj, WEAR_FEET_2 );
				return;
			}

			bug( "Wear_obj: feet2 change no free feet for centaur.", 0 );
			send_to_char( "§Aªº¸}¤w¸g¬ïº¡¾c¤l¤F.\n\r", ch );
			return;
		}
	}

	if ( fReplace ) //some objs just cannot be wear/hold/wield...
		send_to_char( "§AµLªk¸Ë³Æ³o¼ËªF¦è.\n\r", ch );

	return;
}

/* Added by Keric for personal eq AC recover */
void do_personal_recover( CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *mob;
	OBJ_DATA *obj;
	float adj = 1.0;
	int price;
	bool fBank = FALSE;
	char arg[MAX_STRING_LENGTH];
    
	if( !ch->in_room ) return;

	for( mob = ch->in_room->people ; mob; mob = mob->next_in_room )
	{
		if( IS_NPC(mob) && IS_SET(mob->act, ACT_SMITH)) break;
	}

	if( !mob )
	{
		send_to_char("¤°»ò¡H§A·Q·F¤°»ò¡H\n\r", ch);
		return;
	}

	one_argument(argument, arg);
    
	if( arg[0] == '\0') {
		send_to_char("§A­n­×¤°»ò¡H\n\r", ch);
		return;
	}

	if( !str_cmp("all", arg)) {
		for( obj = ch->carrying ; obj ; obj = obj->next_content )
		{
			if( obj->wear_loc != WEAR_NONE ) continue;
			if( !(obj->item_type == ITEM_ARMOR) ) continue;
			if( obj->item_type == ITEM_ARMOR && obj->value[1] < 1 ) continue;
			/*
			if( !(obj->extra_flags & ITEM_PERSONAL) ) continue;

			if( obj->item_type == ITEM_ARMOR && obj->value[1] < 0 )    
			{ 
				send_to_char("§A¦b´M§Ú¶}¤ß¶Ü¡H\n\r", ch);
				continue;
			}
			*/

			if( obj->level < 5 ) price = 5;
			else if (obj->level < 15) price = 10;
			else if ( obj->level < 30) price = 50;
			else if ( obj->level <= LEVEL_HERO ) price = 100;
			else price = 150;
			price = URANGE(1,price, ((obj->cost * 2) ));

			price *= obj->value[1]*100;
			if( IS_SET(obj->extra_flags, ITEM_BLESS)) adj += 1.5;
			if( IS_SET(obj->extra_flags, ITEM_HUM)) adj += 1.5;
			if( !(obj->extra_flags & ITEM_PERSONAL)) price *= 30;

			price = (int)(price * adj);
			price = UMAX( 100, price);

			sprintf(arg, "­×²z %s »Ý­n %d ªTª÷¹ô¡C\n\r", obj->short_descr, price);

			send_to_char(arg, ch);
			fBank = FALSE;

			if( ch->gold < price ) 
			{
				fBank = TRUE;
				if( ch->bank < price ) 
				{
					act("$N·n¤F·nÀY¡A»¡¡G¡u±zªº¿ú¤£°÷¡C¡v", ch, NULL, mob, TO_CHAR);
					continue;
				}
			}

			if( fBank ) 
			{
				sprintf(arg, "§A±q»È¦æ¥I¥X %d ªTª÷¹ô¡C \n\r", price);
				ch->bank -= price;
			}
			else 
			{
				sprintf(arg, "§A¥I¥X %d ªTª÷¹ô¡C \n\r", price);
				ch->gold -= price;
			}

			send_to_char(arg, ch);

			act("§A§â $p ¥æµ¹$N¡C", ch, obj, mob, TO_CHAR);
			act("$N»Eºë·|¯«¡A$pªx°_¤F¤@¹D¥ú¨~¡C", ch, obj, mob, TO_CHAR);
			act("$N©Ù©ÙÃBÀY¤Wªº¦½¯]¡A§â$p¥æÁÙµ¹§A¡C", ch, obj, mob, TO_CHAR);

			if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) )
			{
				obj->value[0] += obj->value[1];
				obj->value[1] = 0;
			}
		}
		return;
	}

	if( !(obj = get_obj_carry(ch, arg))){
		send_to_char("§A¨­¤W¨S¦³³o¼ËªF¦è¡C\n\r", ch);
		return;
	}

	//if( !(obj->extra_flags & ITEM_PERSONAL) ) 
	if( !(obj->item_type == ITEM_ARMOR) )
	{
		act("$N·n¤F·nÀY¡A»¡¡G¡u³o¼ËªF¦è§Ú¤£¯à­×¡C¡v", ch, NULL, mob, TO_CHAR);
		return;
	}
  
	if( obj->item_type == ITEM_ARMOR && obj->value[1] < 1 )
	{
		act("$N·n¤F·nÀY¡A»¡¡G¡u³o¼ËªF¦è®Ú¥»¨SÃa¡I¡v", ch, NULL, mob, TO_CHAR);
		return;
    }

	if( obj->level < 5 ) price = 5;
	else if (obj->level < 15) price = 10;
	else if ( obj->level < 30) price = 50;
	else if ( obj->level <= LEVEL_HERO ) price = 100;
	else price = 150;
	price = UMIN(price, ((obj->cost * 2) ));

	price *= obj->value[1]*100;
	if( IS_SET(obj->extra_flags, ITEM_BLESS)) adj += 1.5;
	if( IS_SET(obj->extra_flags, ITEM_HUM)) adj += 1.5;
	if( !(obj->extra_flags & ITEM_PERSONAL)) price *= 30;

	price = (int)(price * adj);
	price = UMAX( 100, price);

	sprintf(arg, "­×²z %s »Ý­n %d ªTª÷¹ô¡C\n\r", obj->short_descr, price);
	send_to_char(arg, ch);
	fBank = FALSE;
	if( ch->gold < price )
	{
		fBank = TRUE;
		if( ch->bank < price ) {
			act("$N·n¤F·nÀY¡A»¡¡G¡u±zªº¿ú¤£°÷¡C¡v", ch, NULL, mob, TO_CHAR);
			return;
		}
	}

	if( fBank ){
		sprintf(arg, "§A±q»È¦æ¥I¥X %d ªTª÷¹ô¡C \n\r", price);
		ch->bank -= price;
	}else{
		sprintf(arg, "§A¥I¥X %d ªTª÷¹ô¡C \n\r", price);
		ch->gold -= price;
	}

	send_to_char(arg, ch);

	act("§A§â $p ¥æµ¹$N¡C", ch, obj, mob, TO_CHAR);
	act("$N»Eºë·|¯«¡A$pªx°_¤F¤@°}¥ú¨~¡C", ch, obj, mob, TO_CHAR);
	act("$N©Ù©ÙÃBÀY¤Wªº¦½¯]¡A§â$p¥æÁÙµ¹§A¡C", ch, obj, mob, TO_CHAR);

	if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) ){
		obj->value[0] += obj->value[1];
		obj->value[1] = 0;
	}
	return;
}

void do_repair( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	CHAR_DATA *mob;
	char arg[ MAX_STRING_LENGTH];
	int curr_d, max_d, diff, price;
	float adj = 1.0;
	bool fBank = FALSE;
	if( !ch->in_room ) return;

	for( mob = ch->in_room->people ; mob; mob = mob->next_in_room){
		if( IS_NPC(mob) && IS_SET(mob->act, ACT_SMITH)) break;
	}

	if( !mob ) {
		send_to_char("§A§ä¤£¨ì¤HÀ°§A­×´_¸Ë³Æ¡C\n\r", ch);
		return;
	}

	one_argument(argument, arg);

	if( arg[0] == '\0') {
		send_to_char("§A­n­×¤°»ò¡H\n\r", ch);
		return;
	}

	if( !str_cmp("all", arg)) {
		for( obj = ch->carrying ; obj ; obj = obj->next_content )
		{
			if( obj->wear_loc != WEAR_NONE ) continue;

			if( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
				continue;

			curr_d = get_curr_dura(obj);
			max_d = get_max_dura(obj);
			diff = max_d - curr_d;

			if( diff < 1 || indestructible(obj) )    
			{ 
				continue;
			}

			if( obj->level < 5 )
				price = 1;
			else if (obj->level < 15)
				price = 2;
			else if ( obj->level < 30)
				price = 5;
			else if ( obj->level <= LEVEL_HERO )
				price = 10;
			else
				price = 30;

			price = URANGE(1,price, ((obj->cost * 2) ));

			price *= diff;
			if( IS_SET(obj->extra_flags, ITEM_BLESS))
				adj += 0.5;
			if( IS_SET(obj->extra_flags, ITEM_HUM))
				adj += 0.2;


			price = (int)(price * adj);
			price = UMAX( 100, price);
			// if( obj->item_type == ITEM_ARMOR && obj->value[1] > 0 )
			// price += obj-> level * 1000 * obj->value[1];

			sprintf(arg, "­×²z %s »Ý­n %d ªTª÷¹ô¡C\n\r", obj->short_descr, price);
			send_to_char(arg, ch);
			fBank = FALSE;
			if( ch->gold < price ) 
			{
				fBank = TRUE;
				if( ch->bank < price ) 
				{
					act("$N·n¤F·nÀY¡A»¡¡G¡u±zªº¿ú¤£°÷¡C¡v", ch, NULL, mob, TO_CHAR);
					continue;
				}
			}

			if( fBank ) 
			{
				sprintf(arg, "§A±q»È¦æ¥I¥X %d ªTª÷¹ô¡C \n\r", price);
				ch->bank -= price;
			}
			else 
			{
				sprintf(arg, "§A¥I¥X %d ªTª÷¹ô¡C \n\r", price);
				ch->gold -= price;
			}
			send_to_char(arg, ch);

			act("§A§â $p ¥æµ¹$N¡C", ch, obj, mob, TO_CHAR);
			act("$N¤@°}ºVºV¥´¥´¡A$pºCºC¦^´_¤F¥¿±`ªºª¬ºA¡C", ch, obj, mob, TO_CHAR);
			act("$N©Ù©ÙÃBÀY¤Wªº¦½¯]¡A§â$p¥æÁÙµ¹§A¡C", ch, obj, mob, TO_CHAR);

			repair_dura(obj, diff);
				/*if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) )
					{
					price += obj-> level * 10 * obj->value[1];
					obj->value[0] += obj->value[1];
					obj->value[1] = 0;
					}
				*/
		}
		return;
	}

	if( !(obj = get_obj_carry(ch, arg))){
		send_to_char("§A¨­¤W¨S¦³³o¼ËªF¦è¡C\n\r", ch);
		return;
	}

	//if( !can_drop_obj(ch, obj)){
	//  send_to_char("§A¨S¿ìªk§â¥¦µ¹¤H¡C\n\r", ch);
	//  return;
	//  }

	curr_d = get_curr_dura(obj);
	max_d = get_max_dura(obj);
	diff = max_d - curr_d;

	if( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
	{
		act("$N·n¤F·nÀY¡A»¡¡G¡u³o¼ËªF¦è§Ú¨S¿ìªk­×¡C¡v", ch, NULL, mob, TO_CHAR);
		return;
	}
	if( diff < 1 || indestructible(obj))
	{
		act("$N·n¤F·nÀY¡A»¡¡G¡u³o¼ËªF¦è®Ú¥»¨SÃa¡I¡v", ch, NULL, mob, TO_CHAR);
		return;
	}

	if( obj->level < 5 )
		price = 1;
	else if (obj->level < 15)
		price = 2;
	else if ( obj->level < 30)
		price = 5;
	else if ( obj->level <= LEVEL_HERO )
		price = 10;
	else
		price = 30;

	price *= diff;
	price = UMIN(price, ((obj->cost * 2) ));
	if( IS_SET(obj->extra_flags, ITEM_BLESS))
		adj += 0.5;
	if( IS_SET(obj->extra_flags, ITEM_HUM))
		adj += 0.2;


	price = (int)(price * adj);
	price = UMAX( 100, price);
	// if( obj->item_type == ITEM_ARMOR && obj->value[1] > 0 )
	// price += obj-> level * 1000 * obj->value[1];

	sprintf(arg, "­×²z %s »Ý­n %d ªTª÷¹ô¡C\n\r", obj->short_descr, price);
	send_to_char(arg, ch);
	fBank = FALSE;
	if( ch->gold < price )
	{
		fBank = TRUE;
		if( ch->bank < price ) {
			act("$N·n¤F·nÀY¡A»¡¡G¡u±zªº¿ú¤£°÷¡C¡v", ch, NULL, mob, TO_CHAR);
			return;
		}
	}

	if( fBank )
	{
		sprintf(arg, "§A±q»È¦æ¥I¥X %d ªTª÷¹ô¡C \n\r", price);
		ch->bank -= price;
	}else{
		sprintf(arg, "§A¥I¥X %d ªTª÷¹ô¡C \n\r", price);
		ch->gold -= price;
	}
	send_to_char(arg, ch);

	act("§A§â $p ¥æµ¹$N¡C", ch, obj, mob, TO_CHAR);
	act("$N¤@°}ºVºV¥´¥´¡A$pºCºC¦^´_¤F¥¿±`ªºª¬ºA¡C", ch, obj, mob, TO_CHAR);
	act("$N©Ù©ÙÃBÀY¤Wªº¦½¯]¡A§â$p¥æÁÙµ¹§A¡C", ch, obj, mob, TO_CHAR);

	repair_dura(obj, diff);
	/*if( (obj->item_type == ITEM_ARMOR) && (obj->value[1] > 0) ){
	obj->value[0] += obj->value[1];
	obj->value[1] = 0;
	}*/

	return;
}

void do_wear( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];
	char place [MAX_INPUT_LENGTH];
	int iWear = 0;
	//static const   WEAR_MAP wear_table[17]= {
	static const   WEAR_MAP wear_table[24]= {
	{   "about",      ITEM_WEAR_ABOUT},
	{   "arm",        ITEM_WEAR_ARMS},
	{   "arrow",      ITEM_WEAR_ARROW},
	{   "body",       ITEM_WEAR_BODY },
	{   "feet",       ITEM_WEAR_FEET },
	{   "finger",     ITEM_WEAR_FINGER },
	{   "hand",       ITEM_WEAR_HANDS},
	{   "head",       ITEM_WEAR_HEAD },
	{   "hold",       ITEM_HOLD},
	{   "leg",        ITEM_WEAR_LEGS },
	{   "neck",       ITEM_WEAR_NECK },
	{   "shield",     ITEM_WEAR_SHIELD},
	{   "twohand",    ITEM_TWO_HAND},
	{   "waist",      ITEM_WEAR_WAIST},
	{   "weapon",     ITEM_WIELD},
	{   "wrist",      ITEM_WEAR_WRIST},
	//{   "ride",       ITEM_WEAR_RIDE}, // ride ²¾¨ì³Ì«á 2022/04/05
	{   "wp1",        WEAR_WIELD},       // add at 2022/04/05
	{   "wp2",        WEAR_WIELD_2},     // add at 2022/04/05
	{   "hand1",      WEAR_HANDS},       // add at 2022/04/05
	{   "hand2",      WEAR_HANDS_2},     // add at 2022/04/05
	{   "feet1",      WEAR_FEET},        // add at 2022/04/05
	{   "feet2",      WEAR_FEET_2},      // add at 2022/04/05
	{   "ammo",       WEAR_AMMO},        // add at 2022/04/30  alc ¼uÃÄ
	{   "ride",       ITEM_WEAR_RIDE},
	};

	argument = one_argument( argument, arg );
	one_argument(argument, place);
	if( place[0] != '\0' ){
		int i;
		for( i = 0; i < 23 ; ++i ){
			if( !str_prefix(place, wear_table[i].name)){
				iWear = wear_table[i].place;
				break;
			}
		}
	}

	if ( arg[0] == '\0' )
	{
		send_to_char( "­n¸Ë³Æ¤°»ò¡H\n\r", ch );
		return;
	}

	if ( !str_cmp( arg, "all" ) )
	{
		OBJ_DATA *obj_next;

		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( obj->wear_loc != WEAR_NONE || !can_see_obj( ch, obj ) )
			continue;

			if ( CAN_WEAR( obj, ITEM_WIELD ) //some race cannot wield weapon
				&& !IS_SET( race_table[ ch->race ].race_abilities, RACE_WEAPON_WIELD ) )//this check seem could put in
			continue;     //wear_obj()?

			wear_obj( ch, obj, FALSE, 0);
			if ( !IS_NPC( ch ) && ch->position == POS_DEAD )
			{
				raw_kill( ch, ch );
				break;
			}
		}
		return;
	}else{
		if ( !( obj = get_obj_carry( ch, arg ) ) )
		{
			send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
			return;
		}

		if ( CAN_WEAR( obj, ITEM_WIELD ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_WEAPON_WIELD ) )
		{
			send_to_char( "§AµLªk¸Ë³ÆªZ¾¹.\n\r", ch );
			return;
		}

		wear_obj( ch, obj, TRUE , iWear);
		if ( !IS_NPC( ch ) && ch->position == POS_DEAD )
			raw_kill( ch, ch );

	}

	return;
}


void do_fremove( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [ MAX_INPUT_LENGTH ];
    int   wear_loc;

    one_argument( argument, arg );

    if ( arg[0] == '\0' ){
        send_to_char( "­n²æ¤U¤°»ò¡H\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "all" ) ){
        for( wear_loc = 0; wear_loc <= 20; wear_loc++ )
            fremove_obj( ch, wear_loc, TRUE );
        fremove_obj( ch, WEAR_FINGER_3, TRUE );
        fremove_obj( ch, WEAR_FINGER_4, TRUE );
        fremove_obj( ch, WEAR_ARMS_2, TRUE );
        fremove_obj( ch, WEAR_HANDS_2, TRUE );
        return;
    }

    if ( !( obj = get_obj_wear( ch, arg ) ) ){
        send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
        return;
    }

    fremove_obj( ch, obj->wear_loc, TRUE );
    return;
}

void do_remove( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [ MAX_INPUT_LENGTH ];
    int   wear_loc;

    one_argument( argument, arg );

    if ( arg[0] == '\0' ){
        send_to_char( "­n²æ¤U¤°»ò¡H\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "all" ) ){
        for( wear_loc = 0; wear_loc <= 20; wear_loc++ )
			remove_obj( ch, wear_loc, TRUE );
        remove_obj( ch, WEAR_FINGER_3, TRUE );
        remove_obj( ch, WEAR_FINGER_4, TRUE );
        remove_obj( ch, WEAR_HANDS_2, TRUE );
        remove_obj( ch, WEAR_ARMS_2, TRUE );
        remove_obj( ch, WEAR_FEET_2, TRUE );
        remove_obj( ch, WEAR_LEGS_2, TRUE );
		remove_obj( ch, WEAR_AMMO, TRUE ); // ¼uÃÄ¸Ë³Æ¦ì¸m add at 2022/04/30
        return;
    }

    if ( !( obj = get_obj_wear( ch, arg ) ) ){
        send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
        return;
    }

    remove_obj( ch, obj->wear_loc, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
	{
		send_to_char( "¤Ñ¯««Ü·PÁÂ§Aªº©^Äm¡A¤]³\\Í¢¥H«á·|±µ¨ü¡C", ch );
		act( "$n §â¦Û¤v©^Ämµ¹¤Ñ¯«¡A³o¬O¦h»ò°¶¤jªº±¡¾Þ¡C", ch, NULL, NULL, TO_ROOM );
		return;
	}

	obj = get_obj_list( ch, arg, ch->in_room->contents );
	if ( !obj )
	{
		send_to_char( "§A§ä¤£¨ì³o­ÓªF¦è¡C\n\r", ch );
		return;
	}

	/* ²¾¦Ü obj_act «á§P©w, ÅýµLªk sac ªºª««~¤]¯àÄ²µo 2022/11/27
		if ( !CAN_WEAR( obj, ITEM_TAKE ) || IS_SET(obj->extra_flags, ITEM_NOSAC))
		{
			act( "$p µLªk³Q©^Äm¡C", ch, obj, NULL, TO_CHAR );
			return;
		} */

	// ­×§ï obj_act sac ¤]¯àÅý mob §@¥Î 2022/02/12
	//if( !IS_NPC(ch) && IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT) ){
		oprog_act_trigger("sac", ch, obj);
	}
	// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
	if( !obj || obj->deleted ) return;

	if ( !CAN_WEAR( obj, ITEM_TAKE ) || IS_SET(obj->extra_flags, ITEM_NOSAC))
	{
		act( "$p µLªk³Q©^Äm¡C", ch, obj, NULL, TO_CHAR );
		return;
	}

	if( !IS_SET(obj->extra_flags, ITEM_CLAN)) {
		// Åý³Q sac obj ¯à³Q ch ¬Ý¨£ 2022/11/27 
		//send_to_char( "¬°¤F·PÁÂ§Aªº©^Äm¡A¤Ñ¯«µ¹¤F§A¤@¨Ç¼úÀy¡C\n\r", ch );
		act( "§A§â $p ©^Ämµ¹¤Ñ¯«¡AÀò±o¤F¤@¨Ç¼úÀy¡C", ch, obj, NULL, TO_CHAR );
		if( !IS_NPC( ch ) )
			ch->gold += obj->level;
	}else{
		act( "§A§â $p ©^Ämµ¹¤Ñ¯«¡C", ch, obj, NULL, TO_CHAR );
	}

	if ( obj->pIndexData->vnum == OBJ_VNUM_CORPSE_NPC && !IS_NPC( ch ) && get_skill_percent(ch, gsn_burial_ceremony ) > number_percent())
	{
		int nBurySklv = get_skill_level(ch, gsn_burial_ceremony);
		int nManaBonus= nBurySklv * nBurySklv / 10;
		ch->mana += obj->level + UMAX( 0, obj->level + get_skill_level(ch, gsn_burial_ceremony) - ch->level - 5 ) * 3
								+ (get_skill_level(ch, gsn_burial_ceremony)<<1);
		ch->mana += obj->level + UMAX( 0, obj->level + nBurySklv * 2 - ch->level - 5 ) * (3 + nManaBonus)
								+ nBurySklv * (nBurySklv + 4);
		send_to_char( "§A±N¦ºªÌ¶W´ç¡A¤Ñ¯«µ¹§A¤@¨Çªk¤O¥H¬°¦^³ø¡C\n\r", ch );
	}

	act( "$n §â $p ©^Ämµ¹¤Ñ¯«¡C", ch, obj, NULL, TO_ROOM );

	// ·s¼W a_sac ¥Î©ó¨t²Î¦r¦êÅã¥Ü«á 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT) ){
		oprog_act_trigger("a_sac", ch, obj);
	}
	if( !obj || obj->deleted ) return;

	extract_obj( obj );
	return;
}

void do_adore( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;
	char arg[MAX_INPUT_LENGTH];
	int sklv = get_skill_level(ch, gsn_adore);

	if (   ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) )
		|| ( !IS_NPC( ch )
	  //&& ch->pcdata->learned[gsn_adore] < number_percent( ) ) )
			&& get_skill_percent( ch, gsn_adore ) < number_percent()))
	{
		send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
		return;
	}

	one_argument(argument, arg);
	if( arg[0] == '\0' ) sprintf( arg, "corpse");

	obj = get_obj_list( ch, arg, ch->in_room->contents );
	if ( !obj || obj->item_type != ITEM_CORPSE_NPC )
	{
		send_to_char( "§A§ä¤£¨ì«ÍÅé¡C\n\r", ch );
		return;
	}

	if ( obj->item_type == ITEM_CORPSE_PC )
	{
		act( "¨ä¥L«_ÀIªÌªº«ÍÅé¬O¤£­ã¸Iªº¡C", ch, obj, NULL, TO_CHAR );
		return;
	}

	if ( obj->level >= ch->level || obj->level > 36 )
	{
		send_to_char( "ÂÇµÛ²øÄYªº»ö¦¡¡A§A¹ï¾Ô³õ¤W°}¤`ªº¾Ô¤hµ¹¤©³Ì±R°ªªº·q·N¡C\n\r", ch );
		act( ""HIP"$n±R·q¦a¹ï°}¤`¾Ô¤hÁ|¦æ²øÄYªº²½Â§¡A$s²´¤¤¦P®É°{°_²§¼Ëªº¥ú½÷¡C"NOR"", ch, NULL, NULL, TO_ROOM );
		if ( !is_affected( ch, gsn_adore ) && ch->hitroll > 2){

			// ¥[¤J obj_act adore 2022/02/12
			if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
				oprog_act_trigger("adore", ch, obj);
			}

			// corpse ¥i¯à¦b prog ¤¤ ³Q purge 2022/02/12
			if(!obj) return;

			af.type  = gsn_adore;
			if( obj->level < ch->level )
				af.duration  = obj->level / 4;
			else
				af.duration  = obj->level / 3;
			af.location  = APPLY_HITROLL;
			af.bitvector = 0;
			af.modifier  = - (obj->level / 6) + (sklv > 4 ? 1 : 0 );
			affect_to_char( ch, &af );

			if( obj->level < ch->level )
				af.duration  = obj->level / 4;
			else
				af.duration  = obj->level / 3;
			af.location  = APPLY_DAMROLL;
			af.bitvector = 0;
			af.modifier  = obj->level / 3 + sklv * 2 / 3;
			affect_to_char( ch, &af );
			// corpse ²¾°£±N¥u¦³¦b adore ¦¨¥\®É¦¨¥ß 2022/02/12
			extract_obj( obj );
		}else{
			send_to_char( "§Aªº¤ß¤w¸g¥Rº¡¤Ó¦h´º¥õ¤F¡C\n\r", ch );
		}
	}else{
		send_to_char( "³o¨ãÁx¤p°­ªº«Í­º¤£­È±o§Aªº´L·q¡C\n\r", ch );
	}
	// remove at 2022/02/12
	//extract_obj( obj );
	return;
}

void do_quaff( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg [ MAX_INPUT_LENGTH ];

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "­n¶¼¥Î¤°»òÃÄ¾¯¡H\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "§A¨­¤W¨S¦³³o¼ËªF¦è.\n\r", ch );
		return;
	}

	if ( obj->item_type != ITEM_POTION )
	{
		send_to_char( "¨º¤£¬OÅ]ªkÃÄ¾¯.\n\r", ch );
		return;
	}

	if( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > ch->level * 5 )
	{
		send_to_char("§A³Ü¤£¤U¤F.\n\r", ch);
	}

	act( "§A³Ü¤U $p.", ch, obj, NULL ,TO_CHAR );
	act( "$n ³Ü¤U $p.", ch, obj, NULL, TO_ROOM );

	if ( obj->level > ch->level )
	{
		act( "§AµLªk¶¼¥Îµ¥¯Å¤ñ§AÁÙ°ªªºÅ]ªkÃÄ¾¯.", ch, obj, NULL, TO_CHAR );
	}else{
		/* obj->value[0] is not used for potions */
		obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
		obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
		obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	}

	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT))
		oprog_act_trigger("quaff", ch, obj);

	if (    !IS_NPC( ch )
		|| ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
	{
		// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
		extract_obj( obj );
		if( ch->fighting )
			WAIT_STATE(ch, 8);
		else
			WAIT_STATE(ch, 2);
		// cond thirst ­×§ï¦¨ obj weight 2023/01/13
		//gain_condition( ch, COND_THIRST, 8);
		gain_condition( ch, COND_THIRST, obj->weight );
	}
	else
		WAIT_STATE(ch, 2);
	return;
}



void do_recite( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *scroll;
	OBJ_DATA  *obj;
	CHAR_DATA *victim;
	char       arg1 [ MAX_INPUT_LENGTH ];
	char       arg2 [ MAX_INPUT_LENGTH ];
	int sklv = get_skill_level(ch, gsn_scrolls);
	int obj_lv = 0;

	obj_lv += sklv;

	//if(sklv > 3) obj_lv += 1;
	//if(sklv > 4) obj_lv += 3;
	//if(sklv > 5) obj_lv += 5;

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	if ( !( scroll = get_obj_carry( ch, arg1 ) ) ){
		send_to_char( "§A¨­¤W¨S¦³¨º¼ËªF¦è.\n\r", ch );
		return;
	}

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	{
		send_to_char( "§Aªº¼L®B¦b°Ê¡A¦ý¬O¨S¦³Án­µµo¥X¨Ó.\n\r", ch );
		return;
	}
	
	if ( scroll->item_type != ITEM_SCROLL )
	{
		act( "§A¤jÁn°á»wµÛ$p.", ch, scroll, NULL, TO_CHAR );
		// ¤£¬O scroll ¤]¯àÄ²µo recite obj_act 2022/02/13
		if( IS_SET(scroll->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("recite", ch, scroll);
		}
		return;
	}

	/*check obj_act when recite scroll , add by coconet*/
	if( IS_SET(scroll->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("recite", ch, scroll);
	}

	// scroll ¥i¯à¦b obj_act ®É®ø¥¢ 2022/02/13
	if(!scroll) return;

	/*
	if ( scroll->item_type != ITEM_SCROLL )
	{
		send_to_char( "¨º¤£¬OÅ]ªk¨÷¶b.\n\r", ch );
		return;
	}
	*/

	obj = NULL;
	if ( arg2[0] == '\0' )
	{
		victim = ch;
		if ( ch->fighting )
			victim = ch->fighting;
	}
	else
	{
		if ( !( victim = get_char_room ( ch, arg2 ) )
			&& !( obj  = get_obj_here  ( ch, arg2 ) ) )
		{
			send_to_char( "§A§ä¤£¨ì¥Ø¼Ð.\n\r", ch );
			return;
		}
	}

	if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) ) || IS_AFFECTED( ch, AFF_CHARM ) )
	{
		act( "§A²{¦b¤£¯à¦Û¥D.", ch, scroll, NULL, TO_CHAR );
		return;
	}
	
	if ( scroll->level > ch->level + sklv * 2 ){
		act( "§AµLªk®Ô»w¤ñ§A°ª¯ÅªºÅ]ªk¨÷¶b.", ch, scroll, NULL, TO_CHAR );
		return;
	}


	// Åý recite stun ¨ü¨ì sklv ¼vÅT 2022/01/13
	//WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	WAIT_STATE( ch, 16 - sklv);

	act( "§A®Ô»w¤F $p.", ch, scroll, NULL, TO_CHAR );
	act( "$n ®Ô»w¤F $p.", ch, scroll, NULL, TO_ROOM );

	/* Scrolls skill by Binky for EnvyMud, modified by Thelonius */
	if ( !IS_NPC( ch ) && !( number_percent( ) < get_skill_percent(ch, gsn_scrolls)+ sklv * 10))
	{
		switch ( number_bits( 3 ) )
		{
			case 0: 
			case 1:          
			case 2:
			case 3: 
				act( "§A®Ú¥»¬Ý¤£À´ $p ªº¤º®e.", ch, scroll, NULL, TO_CHAR );
				return;          
			case 4:          
			case 5:          
			case 6:          
				send_to_char( "§A¤@©w¬O¦³¨º¸Ì©À¿ù¤F.\n\r", ch );
				act( "$p ¬ðµM°_¤õ¿U¿N¡AÂà²´¶¡¿N­Óºë¥ú.", ch, scroll, NULL, TO_CHAR );
				act( "$p ¬ðµM°_¤õ¿U¿N¡AÂà²´¶¡¿N­Óºë¥ú.", ch, scroll, NULL, TO_ROOM );
				extract_obj( scroll );
				return;
			case 7:
				act( "§A¶¶§Q¦a®Ô»w§¹²¦¡A $p µo¥X¤@°}Ä£²´ªº¥ú¨~¡A¦Ó«á®ø¥¢¤F¡C", ch, scroll, NULL, TO_CHAR );
				act( "$p µo¥X¤@°}Ä£²´ªº¥ú¨~¡AµM«á®ø¥¢¦b§A²´«e¡C", ch, scroll, NULL, TO_ROOM );
			  /*
			   * damage( ) call after extract_obj in case the damage would
			   * have extracted ch.  This is okay because we merely mark
			   * obj->deleted; it still retains all values until list_update.
			   * Sloppy?  Okay, create another integer variable. ---Thelonius
			   */
				extract_obj( scroll );
				////damage( ch, ch, scroll->level, gsn_scrolls, WEAR_NONE );
				damage( ch, ch, scroll->level, gsn_scrolls, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
				return;
		}
	}

	/* scroll->value[0] is now used for scrolls */

	// ¨¾¤î TAR_CHAR_SELF ªº spell ¹ï obj ¨Ï¥Î¦¨¥\«á³y¦¨crash add at 2021/09/15
	// ¨¾¤î TAR_IGNORE ¤Î TAR_ARG ¹ï spell ¨Ï¥Î§ì¤£¨ì¥Ø¼Ð³y¦¨ crash add at 2021/09/22
	
	// first spell
	if ( skill_table[scroll->value[1]].target == TAR_CHAR_SELF && victim != ch ){
		obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[1]].target == TAR_IGNORE ){
		// ¸Ë³ÆªþÅ] 2021/10/02
		if (   scroll->value[1] == 314
			|| scroll->value[1] == 315
			|| scroll->value[1] == 316
			|| scroll->value[1] == 317
			|| scroll->value[1] == 318
			|| scroll->value[1] == 319
			|| scroll->value[1] == 320
			|| scroll->value[1] == 321 )
			obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
		else
			obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[1]].target == TAR_ARG ){
		obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, ch, obj );
	}else{
		obj_cast_spell( scroll->value[1], scroll->value[0] + obj_lv, ch, victim, obj );
	}
	// second spell
	if ( skill_table[scroll->value[2]].target == TAR_CHAR_SELF && victim != ch ){
		obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[2]].target == TAR_IGNORE ){
		if (   scroll->value[2] == 314
			|| scroll->value[2] == 315
			|| scroll->value[2] == 316
			|| scroll->value[2] == 317
			|| scroll->value[2] == 318
			|| scroll->value[2] == 319
			|| scroll->value[2] == 320
			|| scroll->value[2] == 321 )
			obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
		else
			obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[2]].target == TAR_ARG ){
		obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, ch, obj );
	}else{
		obj_cast_spell( scroll->value[2], scroll->value[0] + obj_lv, ch, victim, obj );
	}
	// third spell
	if ( skill_table[scroll->value[3]].target == TAR_CHAR_SELF && victim != ch ){
		obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[3]].target == TAR_IGNORE ){
		if (   scroll->value[3] == 314
			|| scroll->value[3] == 315
			|| scroll->value[3] == 316
			|| scroll->value[3] == 317
			|| scroll->value[3] == 318
			|| scroll->value[3] == 319
			|| scroll->value[3] == 320
			|| scroll->value[3] == 321 )
			obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
		else
			obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
	}else if( skill_table[scroll->value[3]].target == TAR_ARG ){
		obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, ch, obj );
	}else{
		obj_cast_spell( scroll->value[3], scroll->value[0] + obj_lv, ch, victim, obj );
	}
/*
	obj_cast_spell( scroll->value[1], scroll->value[0] , ch, victim, obj );
	obj_cast_spell( scroll->value[2], scroll->value[0] , ch, victim, obj );
	obj_cast_spell( scroll->value[3], scroll->value[0] , ch, victim, obj );
*/
	// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
	if ( !IS_NPC( ch )
		|| ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
		extract_obj( scroll );
	return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *staff;
	CHAR_DATA *vch, *victim;
	int        sn;
	int   chance;
	int sklv = get_skill_level(ch, gsn_staves);
	int obj_lv = 0;

	obj_lv += sklv;
	//if(sklv > 3) obj_lv += 1;
	//if(sklv > 4) obj_lv += 3;
	//if(sklv > 5) obj_lv += 5;
    
	if ( !( staff = get_eq_char( ch, WEAR_HOLD ) ) )
	{
		send_to_char( "§A´§¤F´§§AªºÂù¤â¡A¬Ý¨Ó¤°»ò¨Æ¤]¨Sµo¥Í¡C\n\r", ch );
		return;
	}
 
 	if ( staff->item_type != ITEM_STAFF )
	{
		act( "§A´§»RµÛ$p.", ch, staff, NULL, TO_CHAR );
		// ¤£¬O item_staff ¤]¥i¥HÄ²µo brandish obj_act 2022/02/13
		if( IS_SET(staff->pIndexData->progtypes , OBJ_ACT))
		{
			oprog_act_trigger("brandish", ch, staff);
		}
		return;
	}
 
	if( IS_SET(staff->pIndexData->progtypes , OBJ_ACT))
	{
		oprog_act_trigger("brandish", ch, staff);
	}

	// staff ¥i¯à¦b obj_act ®É³Q prog 2022/02/13
	if(!staff) return;

	/*
	if ( staff->item_type != ITEM_STAFF )
	{
		send_to_char( "¥u¦³ªk§ú¥i¥H®³¨Ó´§»R.\n\r", ch );
		return;
	}
	*/

	// if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
	// || IS_AFFECTED( ch, AFF_CHARM ) )
	if ( IS_AFFECTED( ch, AFF_CHARM ) )
	{
		act( "§A¸ÕµÛ´§»R $p, ¦ý¬O§AªºÂù¤â¬Ý¨Ó¤£Å¥«ü´§.", ch, staff, NULL, TO_CHAR );
		act( "$n ¸ÕµÛ´§»R $p, ¦ý¬OÂù¤â¦n¹³¤£¤ÓÅ¥¸Ü.",  ch, staff, NULL, TO_ROOM );
		return;
	}

	if ( ( sn = staff->value[3] ) < 0
		|| sn >= MAX_SKILL
		|| skill_table[sn].spell_fun == 0 )
	{
		bug( "Do_brandish: bad sn %d.", sn );
		return;
	}

	//WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	WAIT_STATE( ch, 20 - sklv);

	if ( staff->value[2] > 0 )
	{
		CHAR_DATA *vch_next;

		act( "§A´§°Ê $p¡A¤Þµo¤º§tªºÅ]¤O.",  ch, staff, NULL, TO_CHAR );
		act( "$n ´§»R $p.", ch, staff, NULL, TO_ROOM );

		/* Staves skill by Binky for EnvyMud, modified by Thelonius */
		/* NPC can use staff by Amenda */

		//ch->pcdata->learned[gsn_staves];
		// if ( !IS_NPC( ch ) && !( number_percent( ) < ch->pcdata->learned[gsn_staves] ) )
		chance = ( IS_NPC( ch ) ) ? ch->level * 2 + 10 : get_skill_percent( ch, gsn_staves) + sklv * 10;
		if ( chance < number_percent( ) )
		{
			switch ( number_bits( 3 ) )
			{
				case 0: 
				case 1:      
				case 2:      
				case 3: 
					act( "§AµLªk¤Þµo $p ªºÅ]¤O.", ch, staff, NULL, TO_CHAR );
					act( "$n µLªk¤Þµo $p ªºÅ]¤O.", ch, staff, NULL, TO_ROOM );
					return;        
				case 4:      
				case 5:      
				case 6:      
					act( "§A¤Þµo¤F $p ªºÅ]¤O, ¦ý¬O¥¦¥ß¨è®ø´²¤F.", ch, staff, NULL, TO_CHAR );
					act( "$n ¤Þµo¤F $p ªºÅ]¤O, ¦ý¬O¥¦¥ß¨è®ø´²¤F.", ch, staff, NULL, TO_ROOM );
					if ( !IS_NPC( ch ) || ( IS_NPC( ch )
						&& ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
					{
						if ( --staff->value[2] <= 0 )
						{
							// ¦³ z4 ªº staff ¤£·|®ø¥¢ add at 2020/10/18
							if( IS_SET(staff->pIndexData->souvenir, Z4) )
							{
								act( HIY"$p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, staff, NULL, TO_CHAR );
								act( HIY"$n ¤â¤¤ªº $p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, staff, NULL, TO_ROOM );
								staff->value[2] = 0;
							}
							else
							{
								act( HIY"$p µo¥X¤@°}±j¥ú¡A±µµÛ¤Æ¬°¦ÇÂu."NOR, ch, staff, NULL, TO_CHAR );
								act( HIY"$n ¤â¤¤ªº $p µo¥X¤@°}±j¥ú¡A±µµÛ¤Æ¬°¦ÇÂu."NOR, ch, staff, NULL, TO_ROOM );
								extract_obj( staff );
							}
						}
					}
					return;
				case 7:
					/*
					 * damage( ) call after extract_obj in case the damage would
					 * have extracted ch.  This is okay because we merely mark
					 * obj->deleted; it still retains all values until list_update.
					 * Sloppy?  Okay, create another integer variable. ---Thelonius
					 */
					// ¦³ z4 ªº staff ¤£·|®ø¥¢ add at 2020/10/18
					if( IS_SET(staff->pIndexData->souvenir, Z4) )
					{
						act( HIR"¤£§®¡I§A±±¨î¤£¤F $p¡A¯à¶q±q¤¤¼Q´é¦Ó¥X¡I"NOR, ch, staff, NULL, TO_CHAR );
						act( HIR"$n ±±¨î¤£¤F $p¡A³QÅ]¤O¬µ¶Ë¤F¡I"NOR, ch, staff, NULL, TO_ROOM );
						staff->value[2] = 0;
					}
					else
					{
						act( HIR"¤£§®¡I§A±±¨î¤£¤F $p ªºÅ]¤O¡A²´¸C¸C¦a¬ÝµÛ¥¦¦b§A¤â¤¤¤Æ¬°¸H¤ù¡I"NOR, ch, staff, NULL, TO_CHAR );
						act( HIR"$n ¤â¤¤ªº $p ¬ðµM¤Æ¦¨¸H¤ù¡I"NOR, ch, staff, NULL, TO_ROOM );
						extract_obj( staff );
					}
					//damage( ch, ch, staff->level, gsn_staves, WEAR_NONE );
					damage( ch, ch, staff->level, gsn_staves, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					return;
			}
		}
		// ²¾°£¥¿±`µ¥¯Å¤U staff ¹ï³æÅé¨Ï¥Îªº³]©w 2022/01/13
		//if ( sklv * 4 + staff->value[0] < 25 )
		if ( staff->value[0] < 0 )
		{
			if ( argument[0] == '\0' )
			{
				if ( ch->fighting )
					victim = ch->fighting;
				else
					victim = ch;
			}
			else if ( !( victim = get_char_room( ch, argument ) ) )
			{
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch );
				return;
			}
			if ( skill_table[sn].target == TAR_CHAR_SELF && victim != ch )
			return;
			obj_cast_spell( staff->value[3], staff->value[0] + obj_lv, ch, victim, NULL );
			staff->value[0] - obj_lv;
		}
		else for ( vch = ch->in_room->people; vch; vch = vch_next )
		{
			vch_next = vch->next_in_room;

			if ( vch->deleted )
				continue;
    			
			if( !IS_NPC(ch) && IS_SET(ch->act, PLR_WIZINVIS))
				continue;

			if( !IS_NPC(vch) && IS_SET(vch->act, PLR_WIZINVIS))//0941003 by jye ±Æ°£imm char
				continue;

			switch ( skill_table[sn].target )
			{
				default:
					bug( "Do_brandish: bad target for sn %d.", sn );
					return;

				case TAR_IGNORE:
					if ( vch != ch )
						continue;
					break;

				case TAR_CHAR_OFFENSIVE:
					if ( is_same_group( ch, vch ) )
						continue;
					break;
				/*    
				case TAR_CHAR_DEFENSIVE:
					if ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
						continue;
						break;
				*/
				case TAR_CHAR_SELF:
					if ( vch != ch )
						continue;
					break;

				case TAR_CHAR_DEFENSIVE:
					break;
			}

			/* staff->value[0] is now used for staves */
			obj_cast_spell( staff->value[3], staff->value[0] + obj_lv, ch, vch, NULL );
		}
	}else{
		// staff v2 <= 0 ªº±¡ªp¤U add at 2020/10/18
		act( "§A¸ÕµÛ¤Þµo $p ªºÅ]¤O, ¦ý²@µL¥ô¦ó¤ÏÀ³.", ch, staff, NULL, TO_CHAR );
		act( "$n ¸ÕµÛ¤Þµo $p ªºÅ]¤O, ¦ý²@µL¥ô¦ó¤ÏÀ³.", ch, staff, NULL, TO_ROOM );
		return;
	}

	if ( !IS_NPC( ch ) || ( IS_NPC( ch )
		&& ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
	{
		// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) )
		if( number_percent() < 2 * sklv * sklv)
			return;

		if ( --staff->value[2] <= 0 )
		{
			// ¦³ z4 ªº staff ¤£·|®ø¥¢ add at 2020/10/18
			if( IS_SET(staff->pIndexData->souvenir, Z4) )
			{
				act( HIY"$p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, staff, NULL, TO_CHAR );
				act( HIY"$n ¤â¤¤ªº $p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, staff, NULL, TO_ROOM );
				staff->value[2] = 0;
			}
			else
			{
				act( HIY"$p µo¥X¤@°}±j¥ú¡A±µµÛ¤Æ¬°¦ÇÂu."NOR, ch, staff, NULL, TO_CHAR );
				act( HIY"$n ¤â¤¤ªº $p µo¥X¤@°}±j¥ú¡A±µµÛ¤Æ¬°¦ÇÂu."NOR, ch, staff, NULL, TO_ROOM );
				extract_obj( staff );
			}
		}
	}

	return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *wand;
	OBJ_DATA  *obj;
	CHAR_DATA *victim;
	char       arg [ MAX_INPUT_LENGTH ];
	int        sn, chance;
	int        sklv = get_skill_level(ch, gsn_wands);
	int obj_lv = 0;

	obj_lv += sklv;
	//if(sklv > 3) obj_lv += 1;
	//if(sklv > 4) obj_lv += 3;
	//if(sklv > 5) obj_lv += 5;

	one_argument( argument, arg );
	if ( arg[0] == '\0' && !ch->fighting )
	{
		send_to_char( "Zap ¤°»ò?\n\r", ch );
		return;
	}

	if ( !( wand = get_eq_char( ch, WEAR_HOLD ) ) )
	{
		send_to_char( "§A¼u¤F¼u¤â«ü¡A¬Ý¨Ó¤°»ò¨Æ¤]¨Sµo¥Í.\n\r", ch );
		return;
	}
	
	if ( wand->item_type != ITEM_WAND )
	{
		act( "§A´§°ÊµÛ$p.", ch, wand, NULL, TO_CHAR );
		// ¤£¬O item_wand ªº¤â´¤¤]¯àÄ²µo 2022/02/13
		if( IS_SET(wand->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("zap", ch, wand);
		}
		return;
	}

	if( IS_SET(wand->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("zap", ch, wand);
	}

	// wand ¥i¯à¦b obj_act ³Q purge 2022/02/13
	if(!wand) return;

	/* 
		if ( wand->item_type != ITEM_WAND )
		{
			send_to_char( "¨º¤£¬OÅ]§ú.\n\r", ch );
			return;
		}
	*/

	obj = NULL;
	if ( arg[0] == '\0' ){
		if ( ch->fighting ){
			victim = ch->fighting;
		}else{
			send_to_char( "Zap ¤°»ò?\n\r", ch );
			return;
		}
    }else{
		if ( !( victim = get_char_room ( ch, arg ) ) && !( obj  = get_obj_here  ( ch, arg ) ) )
		{
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
			return;
		}
	}

	//    if ( ( IS_NPC( ch ) && IS_SET( ch->act, ACT_PET ) )
	//  || IS_AFFECTED( ch, AFF_CHARM ) )
	if ( IS_AFFECTED( ch, AFF_CHARM ) ){
		act( "§A·Q­n¨Ï¥Î $p, ¦ý¬O§AªºÂù¤â¬Ý¨Ó¤£Å¥«ü´§.", ch, wand, NULL, TO_CHAR );
		act( "$n ¸ÕµÛ¨Ï¥Î $p, ¦ý¬OÂù¤â¤£Å¥«ü´§.", ch, wand, NULL, TO_ROOM );
		return;
	}

	if ( ( sn = wand->value[3] ) < 0 || sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 ){
		bug( "Do_zap: bad sn %d.", sn );
		return;
    }

	//WAIT_STATE( ch, 2 * PULSE_VIOLENCE);
	WAIT_STATE( ch, 16 - sklv);

	if ( wand->value[2] > 0 )
	{
		if ( victim )
		{
			if ( victim == ch ){
				act( "§A¹ï¦Û¤v¨Ï¥Î¤F $p.", ch, wand, NULL,   TO_CHAR );
				act( "$n ¹ï¦Û¤v¨Ï¥Î¤F $p.",   ch, wand, NULL,   TO_ROOM );
			}else{
				act( "§A¹ï $N ¨Ï¥Î¤F $p.",    ch, wand, victim, TO_CHAR );
				act( "$n ¹ï $N ¨Ï¥Î¤F $p.",   ch, wand, victim, TO_ROOM );
			}
		}else{
			act( "§A¹ï $P ¨Ï¥Î¤F $p.",      ch, wand, obj,    TO_CHAR );
			act( "$n ¹ï $P ¨Ï¥Î¤F $p.",     ch, wand, obj,    TO_ROOM );
		}

		/* Wands skill by Binky for EnvyMud, modified by Thelonius */
		//ch->pcdata->learned[gsn_wands];
		//  if ( !IS_NPC( ch ) 
		//      && !( number_percent( ) < ch->pcdata->learned[gsn_wands] ) )
		chance = ( IS_NPC( ch ) ) ? ch->level * 2 + 10 : get_skill_percent(ch, gsn_wands)+sklv*2;
		if ( chance < number_percent( ) )
		{
			switch ( number_bits( 3 ) )
			{
				case 0: 
				case 1:      
				case 2:      
				case 3: 
					act( "§AµLªk¤Þµo $p ªºÅ]¤O.", ch, wand, NULL, TO_CHAR );
					act( "$n µLªk¤Þµo $p ªºÅ]¤O.", ch, wand, NULL, TO_ROOM );
					return;        
				case 4:      
				case 5:      
				case 6:      
					act( "§A¤Þµo¤F $p ªºÅ]¤O, ¦ý¬O¥¦¥ß¨è®ø´²¤F.", ch, wand, NULL, TO_CHAR );
					act( "$n ¤Þµo¤F $p ªºÅ]¤O, ¦ý¬O¥¦¥ß¨è®ø´²¤F.", ch, wand, NULL, TO_ROOM );
					if ( !IS_NPC( ch ) || ( IS_NPC( ch )
						&& ( IS_AFFECTED( ch, AFF_CHARM )
						|| IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
					{
						if ( --wand->value[2] <= 0 )
						{
							// ¦³ z4 ªº wand ¤£·|®ø¥¢ add at 2020/10/18
							if( IS_SET(wand->pIndexData->souvenir, Z4) )
							{
								act( HIY"$p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, wand, NULL, TO_CHAR );
								act( HIY"$n ¤â¤¤ªº $p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, wand, NULL, TO_ROOM );
								wand->value[2] = 0;
							}
							else
							{
								act( HIY"$p µo¥X¤@°}±j¥ú¡AµM«á¤Æ¬°¦ÇÂu."NOR, ch, wand, NULL, TO_CHAR );
								act( HIY"$n ¤â¤¤ªº $p µo¥X¤@°}±j¥ú¡AµM«á¤Æ¬°¦ÇÂu."NOR, ch, wand, NULL, TO_ROOM );
								extract_obj( wand );
							}
						}
					}
					return;
				case 7:
				/*
					* damage( ) call after extract_obj in case the damage would
					* have extracted ch.  This is okay because we merely mark
					* obj->deleted; it still retains all values until list_update.
					* Sloppy?  Okay, create another integer variable. ---Thelonius
				*/
					// ¦³ z4 ªº wand ¤£·|®ø¥¢ add at 2020/10/18
					if( IS_SET(wand->pIndexData->souvenir, Z4) )
					{
						act( HIR"¤£§®¡I§A±±¨î¤£¤F $p¡A¯à¶q±q¤¤¼Q´é¦Ó¥X¡I"NOR, ch, wand, NULL, TO_CHAR );
						act( HIR"$n ±±¨î¤£¤F $p¡A³QÅ]¤O¬µ¶Ë¤F¡I"NOR, ch, wand, NULL, TO_ROOM );
						wand->value[2] = 0;
					}
					else
					{
						act( HIR"¤£§®¡I§A±±¨î¤£¤F $p ªºÅ]¤O¡A²´¸C¸C¦a¬ÝµÛ¥¦¦b§A¤â¤¤¤Æ¬°¸H¤ù¡I"NOR, ch, wand, NULL, TO_CHAR );
						act( HIR"$n ¤â¤¤ªº $p ¬ðµM¤Æ¦¨¸H¤ù¡I"NOR, ch, wand, NULL, TO_ROOM );
						extract_obj( wand );
					}
					//damage( ch, ch, wand->level, gsn_wands, WEAR_NONE );
					damage( ch, ch, wand->level, gsn_wands, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					return;
			}
		}
		
		/* wand->value[0] is now used for wands */
		
		// ¨¾¤î TAR_CHAR_SELF ªº spell ¹ï obj ¨Ï¥Î¦¨¥\«á³y¦¨crash add at 2021/09/15
		// ¨¾¤î TAR_IGNORE ¤Î TAR_ARG ¹ï spell ¨Ï¥Î§ì¤£¨ì¥Ø¼Ð³y¦¨ crash add at 2021/09/22
		
		if ( skill_table[wand->value[3]].target == TAR_CHAR_SELF && victim != ch ){
			obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
		}else if ( skill_table[wand->value[3]].target == TAR_IGNORE ){
			if (   wand->value[3] == 314
				|| wand->value[3] == 315
				|| wand->value[3] == 316
				|| wand->value[3] == 317
				|| wand->value[3] == 318
				|| wand->value[3] == 319
				|| wand->value[3] == 320
				|| wand->value[3] == 321 )
				obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
			else
				obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
		}else if ( skill_table[wand->value[3]].target == TAR_ARG ){
			obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, ch, obj );
		}else{
			obj_cast_spell( wand->value[3], wand->value[0] + obj_lv, ch, victim, obj );
		}
    }
	else
	{
		// wand v2 <= 0 ªº±¡ªp¤U add at 2020/10/18
		act( "§A¸ÕµÛ¤Þµo $p ªºÅ]¤O, ¦ý²@µL¥ô¦ó¤ÏÀ³.", ch, wand, NULL, TO_CHAR );
		act( "$n ¸ÕµÛ¤Þµo $p ªºÅ]¤O, ¦ý²@µL¥ô¦ó¤ÏÀ³.", ch, wand, NULL, TO_ROOM );
		return;
	}

	// if ( !( IS_NPC( ch ) && ch->pIndexData->pShop ) ){
	if ( !IS_NPC( ch ) || ( IS_NPC( ch )
		&& ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) )
	{
		if( number_percent() < 2 * sklv * sklv) return;
		if ( --wand->value[2] <= 0 )
		{
			// ¦³ z4 ªº wand ¤£·|®ø¥¢ add at 2020/10/18
			if( IS_SET(wand->pIndexData->souvenir, Z4) )
			{
				act( HIY"$p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, wand, NULL, TO_CHAR );
				act( HIY"$n ¤â¤¤ªº $p ¯ÓºÉÅ]¤Oµo¥X¤@°}±j¥ú«á³vº¥·t²H¤U¨Ó."NOR, ch, wand, NULL, TO_ROOM );
				wand->value[2] = 0;
			}
			else
			{
				act( "$p ¯ÓºÉÅ]¤O¡A¤Æ¦¨¸H¤ù.", ch, wand, NULL, TO_CHAR );
				act( "$n ¤â¤¤ªº $p ¯ÓºÉÅ]¤O¡A¤Æ¦¨¸H¤ù.", ch, wand, NULL, TO_ROOM );
				extract_obj( wand );
			}
		}
	}

	return;
}



void do_steal( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *victim;
	char       buf  [ MAX_STRING_LENGTH ];
	char       arg1 [ MAX_INPUT_LENGTH  ];
	char       arg2 [ MAX_INPUT_LENGTH  ];
	char       arg [ MAX_INPUT_LENGTH ];
	int        number;
	int        count;
	int        percent;
	int sklv = get_skill_level(ch, gsn_steal);

	if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_HORSE ) )
	{
		send_to_char( "¤£¦æ.\n\r", ch );
		return;
	}

	argument = one_argument( argument, arg1 );
	one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "±q½Ö¨­¤W°½¤°»ò?\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg2 ) ) )
	{
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return;
	}

	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_HORSE ) )
	{
		send_to_char( "§A¥¢±Ñ¤F¡C\n\r",ch);
		return;
	}

	if ( victim == ch )
	{
		send_to_char( "§AºN¤FºN¦Û¤vªº¤f³U.\n\r", ch );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;

	obj = NULL;

	WAIT_STATE( ch, skill_table[gsn_steal].beats );

	/* Modified chances for stealing by Morpheus */
	percent = ch->level * 3/2; /* Base value */

	percent += number_range( -10, 10 ); /* Luck */

	percent -= victim->level * 2; /* Character level vs victim's */

	percent += sklv + sklv / 2 + (sklv/6)*4;

	if ( !IS_AWAKE( victim ) )
		percent += 20; /* Sleeping characters are easier */

	//percent += ch->pcdata->learned[gsn_steal]; /* Character ability */
	percent += get_skill_percent( ch, gsn_steal );

	if ( IS_AFFECTED( ch, AFF_SNEAK ) )
		percent += 5; /* Quiet characters steal better */

	if ( !can_see( ch, victim ) )
		percent += 10; /* Unseen characters steal better */

	percent = ( percent * weakness_percent( victim, gsn_steal)) / 100;
	if ( !str_prefix( arg1, "coins" ) || !str_cmp( arg1, "gold" ) )
		percent = (int) ( percent * 1.2 ); /* Gold is fairly easy to steal */
	else
	{
		number = number_argument( arg1, arg );
		count  = 0;
		for ( obj = victim->carrying; obj; obj = obj->next_content )
		{
			if ( //obj->wear_loc == WEAR_NONE
				/*&&*/ can_see_obj( ch, obj )
				&& is_name( arg, obj->name ) )
			{
				if ( ++count == number )
				break;
			}
		}
  
		if ( !obj )
		{
			send_to_char( "¨S¦³³o¼Ëª««~.\n\r", ch );
			return;
		}

		if ( obj->wear_loc == WEAR_NONE ) {
		/* Items wearing are harder */
			if( victim->level - ch->level < 3 )
				percent = (int) ( percent * .8 );
			else if (victim->level - ch->level < 8 )
				percent = (int) ( percent * .6 );
			else if (victim->level - ch->level < 15 )
				percent = (int) ( percent * .4 );
			else
				percent = (int) ( percent * .2 );
		}
		else
			percent = (int) ( percent * .05 );
	}
	if( IS_NPC(victim) && victim->pIndexData->pShop){
		percent = -1;
	}

	if ( ( !IS_NPC( victim )
		&& ( ( !IS_SET( victim->act, PLR_KILLER )
		&& !IS_SET( victim->act, PLR_THIEF )
		&& !IS_SET( victim->act, PLR_REGISTER )
		&& victim->race != race_lookup( "Vampire" ) )
		|| ch->level - victim->level < 5 ) )
		|| percent < number_percent( ) )
	{
	/* Failure.*/
		send_to_char( "ÁV¿|¡A³Qµo²{¤F¡I\n\r", ch );
		act( "§Aµo²{ $n ·Q±q§A¨­¤W°½ªF¦è¡I\n\r", ch, NULL, victim, TO_VICT );
		act( "$n ·Q±q $N ¨­¤W°½ªF¦è¡A¦ý¬O³Qµo²{¤F¡I \n\r",  ch, NULL, victim, TO_NOTVICT );
		sprintf( buf, "%s ¬O­Ó¥i´cªº¤p°½¡I", ch->name );
		do_shout( victim, buf );
		if ( IS_NPC( victim ) )
		{
			multi_hit( victim, ch, TYPE_UNDEFINED );
		}
		else
		{
			if ( !licensed( ch ) )
			{
				//  send_to_char(
				//       "§A¨S¦³ PK °õ·Ó¡I  §A·l¥¢¤F "HIG"400"NOR" ÂI¸gÅç.\n\r", ch );
				//  gain_exp( ch, -400 );
				if ( registered( ch, victim ) && ch->level - victim->level < 6 )
					return;
			}

			if ( ch->level - victim->level > 5 || !registered( ch, victim ) )
			{
				log_string( buf );
				if ( !IS_SET( ch->act, PLR_THIEF ) )
				{
					SET_BIT( ch->act, PLR_THIEF );
					send_to_char( HIP "***  §A¬O­Ó¤p°½¡I¡I ***"NOR"\n\r", ch );
					save_char_obj( ch );
				}
			}
		}
		return;
	}

	if ( !str_prefix( arg1, "coins" ) || !str_cmp ( arg1, "gold" ) )
	{
		int amount;

		amount = victim->gold * number_range( 1, 10 ) / 20;
		if ( amount <= 0 )
		{
			send_to_char( "§A°½¤£¨ì¥b¤ò¿ú.\n\r", ch );
			return;
		}

		ch->gold += amount;
		victim->gold -= amount;
		sprintf( buf, "±o¤â¤F¡I  §A°½¨ì¤F "HIY"%d"NOR" ªTª÷¹ô.\n\r", amount );
		send_to_char( buf, ch );
		return;
	}

	if ( !can_drop_obj( ch, obj ) || IS_SET( obj->extra_flags, ITEM_INVENTORY ) || obj->level > ch->level )
	{
		send_to_char( "§AºN¨ì¤F¡A¥i¬O«o®³¤£¨«.\n\r", ch );
		return;
	}

	if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
	{
		send_to_char( "§A®³¤£¤U§ó¦hªF¦è¤F.\n\r", ch );
		return;
	}

	if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
	{
		send_to_char( "§A±a¤£°Ê§ó¦h­«¶q¤F.\n\r", ch );
		return;
	}

	//Modified by Razgriz 20050926
	if ( obj->wear_loc != WEAR_NONE )
	{
		send_to_char( "§AºN¨ì¤F¡A¥i¬O«o®³¤£¨«.\n\r", ch );
		return;
		//unequip_char( victim, obj );
	}

	// ±Ô­z²¾¨ì obj Â÷¶}«e 2022/02/12
	send_to_char( "±o¤â¤F¡I\n\r", ch );
	send_to_char( "Ok.\n\r", ch );
	obj_from_char( obj );
	obj_to_char( obj, ch );

	// ·s¼W obj_act steal §P©w°½ÅÑªº¨¤¦â 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("steal", ch, obj);
	}

	// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
	if(!obj) return;

	// ·s¼W obj_act steal_target §P©w³Q°½ªº¨¤¦â 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("steal_target", victim, obj);
	}
	
	//send_to_char( "±o¤â¤F¡I\n\r", ch );
	//send_to_char( "Ok.\n\r", ch );
	return;
}



/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *keeper;
	SHOP_DATA *pShop;
	char       buf [ MAX_STRING_LENGTH ];

	pShop = NULL;

	for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
	{
			/*
			if ( IS_NPC( keeper ) && ( pShop = keeper->pIndexData->pShop )
				&& ( argument[0] == '\0' || is_name( argument, keeper->name ) ) )
			break;
			*/
		if ( !IS_NPC( keeper ) )
			continue;

		if ( IS_NPC( keeper ) && ( pShop = keeper->pIndexData->pShop ) && argument[0] == '\0' )
			break;
	}
		/*
		if ( !keeper || !pShop || ( keeper && IS_AFFECTED( keeper, AFF_CHARM ) ) )
		{
			send_to_char( "§AµLªk³o»ò°µ .\n\r", ch );
			return NULL;
		}
		*/
	/*
	if ( argument[0] != '\0' && !keeper ){
		send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
		return NULL;
	}

	if ( !IS_NPC( keeper ) ){
		send_to_char( "ª±®a¶R½æ½Ð¥Î bid ¤Î auction «ü¥O.\n\r", ch );
		return NULL;
	}
	*/
	if ( argument[0] != '\0' ){
		keeper = get_char_room( ch, argument );
		if ( keeper == NULL) {
			send_to_char( "¨S¦³³o­Ó¤H.\n\r", ch );
			return NULL;
		}
		if ( !IS_NPC( keeper ) ){
			send_to_char( "ª±®a¶R½æ½Ð¥Î bid ¤Î auction «ü¥O.\n\r", ch );
			return NULL;
		}
		pShop = keeper->pIndexData->pShop;
	}

	if ( !keeper ){
		send_to_char( "³o¸Ì¨S¦³¤H¸ò§A¶i¦æ¶R½æ.\n\r", ch );
		return NULL;
	}

	if ( !pShop ){
		send_to_char( "¥L¤£¸ò§A¶i¦æ¶R½æ.\n\r", ch );
		return NULL;
	}

	if ( keeper && IS_AFFECTED( keeper, AFF_CHARM ) ){
		send_to_char( "¥L¨S¦³¶R½æªº·NÄ@.\n\r", ch );
		return NULL;
	}

    /*
     * Undesirables.
     */
	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_KILLER ) )
	{
		do_say( keeper, "³o¸Ì¤£Åwªï¥û¤â¡I" );
		sprintf( buf, "¥i´cªº¥û¤â %s ¦b³o¸Ì¡I\n\r", ch->name );
		do_shout( keeper, buf );
		return NULL;
	}

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_THIEF ) )
	{
		do_say( keeper, "³o¸Ì¤£Åwªï¤p°½¡I" );
		sprintf( buf, "¥i´cªº¤p°½ %s ¦b³o¸Ì¡I \n\r", ch->name );
		do_shout( keeper, buf );
		return NULL;
	}

	/*
     * Shop hours.
	*/
		/*      old version
		if ( time_info.hour < pShop->open_hour )
		{
			do_say( keeper, "©êºp¡A¥»©±©|¥¼¶}±i¡A½Ð±ßÂI¦A¨Ó." );
			return NULL;
		}

		if ( time_info.hour > pShop->close_hour )
		{
			do_say( keeper, "¤µ¤Ñ¤w¸g¥´¯L¤F¡A©ú¤Ñ½Ð¦­." );
			return NULL;
		} */
	
	// ·s¼Wopen > close(©]¶¡Àç·~ or ¤é©]ÄA­Ë°Ó©±) ¤Î open = close ªº§P©w , 20/02/24
	if ( pShop->close_hour > pShop->open_hour ){
		if ( time_info.hour < pShop->open_hour ){
			do_say( keeper, "¥»©±©|¥¼¶}±i¡A½Ð±ßÂI¦A¨Ó." );
			return NULL;
		}

		if ( time_info.hour > pShop->close_hour ){
			do_say( keeper, "¤µ¤Ñ¤w¸g¥´¯L¤F¡A©ú¤Ñ½Ð¦­." );
			return NULL;
		}
	}else if( pShop->close_hour < pShop->open_hour ){
		if ( time_info.hour < pShop->open_hour && time_info.hour - pShop->close_hour > 0){
			do_say( keeper, "¤µ¤Ñ¤w¸g¥´¯L¤F¡A©ú¤Ñ½Ð¦­." );
			return NULL;
		}
	}else{
		if ( time_info.hour != pShop->open_hour ){
			do_say( keeper, "¤µ¤Ñ¤w¸g¥´¯L¤F¡A©ú¤Ñ½Ð¦­." );
			return NULL;
		}
	}


    /*
     * Invisible or hidden people.
     */
	if ( !can_see( keeper, ch ) )
	{
		do_say( keeper, "½Ö¦b©M§Ú»¡¸Ü¡H" );
		return NULL;
	}

	return keeper;
}



int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
	SHOP_DATA *pShop;
	int cost;

	if ( !obj || !( pShop = keeper->pIndexData->pShop ) )
	return 0;

	if ( fBuy )
	{
		cost = obj->cost * UMAX(pShop->profit_buy,100)  / 100;
	}
	else
	{
		OBJ_DATA *obj2;
		int itype;

		cost = 0;
		for ( itype = 0; itype < MAX_TRADE; itype++ )
		{
			if ( obj->item_type == pShop->buy_type[itype] )
			{
				cost = obj->cost * UMIN(pShop->profit_sell, 100) / 100;
				break;
			}
		}

		for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
		{
			if ( obj->pIndexData == obj2->pIndexData )
			cost /= 2;
		}
	}

	if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
		cost = cost * obj->value[2] / obj->value[1];

	return cost;
}

// ·s¼W¥H qmark §P©wªº«D½æ«~ 2022/05/05
bool not_for_sale( CHAR_DATA *ch, CHAR_DATA *keeper, OBJ_DATA *obj )
{
	
	/*  ¥Ø«e¦³ 5 ºØ qmark , ¨C­Ó value ¥Nªí¤@­Ó obj vnum
		¦]¦¹¨C­Ó mob ¯à¦³ 15 ¥ó«D½æ«~
		°Ó¤H not_for_sale v0 v1 v2 ­È¤À§O¹ïÀ³ ¶R®a for_sale ªº v0 ­È : 

							v0       v1       v3
		not_for_sale_1 :    1        2        4
		not_for_sale_2 :    8        16       32
		not_for_sale_3 :    64       128      256
		not_for_sale_4 :    512      1024     2048
		not_for_sale_5 :    4096     8192     16384

		¦pªG­È¹ïÀ³¨ì, «h not_for_sale ´N¤£¦¨¥ß (¶R®a¥i¥HÁÊ¶R¸Óª««~)
		µù: ¤£¥Î¾á¤ß list ¸ò buy ªº½s¸¹, ³o¨Ç¥»¨Ó´N¨Ì char ¤£¦P¦Ó¤£¦P */

	QMARK *qm;
	char saleqm [ MAX_INPUT_LENGTH ];

	// §P©w ¶R®a ¦³¨S¦³ for_sale_vnum( mob °Ó¤Hªº vnum ) ªº qmark
	sprintf( saleqm , "for_sale_%d", keeper->pIndexData->vnum );
	qm = is_qmark(ch, saleqm);

	// §P©w obj vnum ¬O§_¹ïÀ³ °Ó¤H ¨­¤W¯S©w qmark value
	if( is_qmark( keeper , "not_for_sale_1" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_1" )->v0 )
		{
			// §P©w ¶R®a ¨­¤W¬O§_¦³¹ïÀ³ªº qmark value
			if( qm && qm->v0 & 1 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_1" )->v1 )
		{
			if( qm && qm->v0 & 2 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_1" )->v2 )
		{
			if( qm && qm->v0 & 4 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_2" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_2" )->v0 )
		{
			if( qm && qm->v0 & 8 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_2" )->v1 )
		{
			if( qm && qm->v0 & 16 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_2" )->v2 )
		{
			if( qm && qm->v0 & 32 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_3" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_3" )->v0 )
		{
			if( qm && qm->v0 & 64 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_3" )->v1 )
		{
			if( qm && qm->v0 & 128 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_3" )->v2 )
		{
			if( qm && qm->v0 & 256 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_4" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_4" )->v0 )
		{
			if( qm && qm->v0 & 512 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_4" )->v1 )
		{
			if( qm && qm->v0 & 1024 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_4" )->v2 )
		{
			if( qm && qm->v0 & 2048 )
				return FALSE;
			else
				return TRUE;
		}
	}
	if( is_qmark( keeper , "not_for_sale_5" ) ){
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_5" )->v0 )
		{
			if( qm && qm->v0 & 4096 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_5" )->v1 )
		{
			if( qm && qm->v0 & 8192 )
				return FALSE;
			else
				return TRUE;
		}
		if( obj->pIndexData->vnum == is_qmark( keeper , "not_for_sale_5" )->v2 )
		{
			if( qm && qm->v0 & 16384 )
				return FALSE;
			else
				return TRUE;
		}
	}

	// ¦pªG obj vnum ¨S¹ïÀ³¸Ó¯S©wªº qmark «h¤£¦¨¥ß
	return FALSE;

}

/*
 * Multiple object buy modifications by Erwin Andreasen
 * Obtained from Canth's snippets page at:
 * http://www.xs4all.nl/~phule/snippets/snippets.html
 */
void do_buy( CHAR_DATA *ch, char *argument )
{
	char arg  [ MAX_INPUT_LENGTH ];
	char arg2 [ MAX_INPUT_LENGTH ];
	char arg3 [ MAX_INPUT_LENGTH ];
	int type;
	QMARK *qmark;

	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg2);
	one_argument( argument, arg3 );

	if ( arg[0] == '\0' ){
		send_to_char( "­n¶R¤°»ò¡H\n\r", ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags, ROOM_PET_SHOP ) )
	{
		CHAR_DATA *pet;
		ROOM_INDEX_DATA *pRoomIndexNext;
		ROOM_INDEX_DATA *in_room;
		char buf [ MAX_STRING_LENGTH ];

		if ( IS_NPC( ch ) )
			return;

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

		if ( !pRoomIndexNext ){
			bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "³o¸Ì¨S¦³½æ¨º¼ËªF¦è.\n\r", ch );
			return;
		}

		in_room     = ch->in_room;
		ch->in_room = pRoomIndexNext;
		pet     = get_char_room( ch, arg );
		ch->in_room = in_room;

		if ( !pet || !IS_SET( pet->act, ACT_PET ) ){
			send_to_char( "³o¸Ì¨S¦³½æ¨º¼ËªF¦è.\n\r", ch );
			return;
		}
		/*
		  if ( IS_SET( ch->act, PLR_BOUGHT_PET ) && !IS_IMMORTAL( ch ) )
		  {
			  send_to_char( "§A¤w¸g¦³¤@°¦Ãdª«¤F.\n\r", ch );
			  return;
		  }
		*/
		if ( ch->gold < pet->level * pet->level * pet->level ){
			send_to_char( "§A¶R¤£°_.\n\r", ch );
			return;
		}

		if ( !can_see( ch, ch ) ){
			send_to_char( "§A¬DªºÃdª«¬Ý¤£¨ì§A¡I\n\r", ch );
			return;
		}
		
		if ( ch->level < pet->level ){
			send_to_char( "§Aªºµ¥¯Å¤£°÷.\n\r", ch );
			return;
		}

		if ( ( qmark = ch->qmark ) )
		for( qmark = ch->qmark; qmark != NULL; qmark = qmark->next )
			if ( !str_cmp( qmark->mark_name, "bought_pet_timer" ) )
			{
				send_to_char( "§A¤£¤[«e¤~­è¶R¹L¤@°¦Ãdª«¡A¥²¶·¹L¤@°}¤l¤~¯à¦A¶R¡C\n\r", ch );
				return;
			}
			
		ch->gold -= pet->level * pet->level * pet->level;
		pet   = create_mobile( pet->pIndexData );
		pet->level = pet->pIndexData->level;
		//  SET_BIT( ch->act,    PLR_BOUGHT_PET );
		SET_BIT( pet->act, ACT_PET );
		SET_BIT( pet->affected_by, AFF_CHARM );
		pet->clan = ch->clan;
		
		one_argument( argument, arg );

		if ( arg[0] != '\0' ){
			sprintf( buf, "%s %s", pet->name, arg );
			free_string( pet->name );
			pet->name = str_dup( buf );
		}

		sprintf( buf, "%s¶µ°é¤W¼gµÛ¡G¡u§Úªº¥D¤H¬O %s¡v.\n\r",
		pet->description, /*ch->clan ? ch->clan->name : */ ch->name );
		free_string( pet->description );
		pet->description = str_dup( buf );

		char_to_room( pet, ch->in_room );
		mprog_repop_trigger(pet);
		//if ( !ch->clan )
		add_follower( pet, ch );
		send_to_char( "½Ð¦n¦nµ½«Ý§AªºÃdª«.\n\r", ch );
		act( "$n ¶R¤U¤F¤@°¦ $N.", ch, NULL, pet, TO_ROOM );

		sprintf( arg, "%s ", ch->name );
		strcat( arg, "bought_pet_timer 30 0 0 0 0 0" );
		do_mpadd_qmark( pet, arg );
		return;
	}
    else if ( IS_SET( ch->in_room->room_flags, ROOM_HORSE_SHOP ) )
    {
		CHAR_DATA *horse;
		ROOM_INDEX_DATA *pRoomIndexNext;
		ROOM_INDEX_DATA *in_room; // temp for IN_ROOM
		char buf [ MAX_STRING_LENGTH ];

		if ( IS_NPC( ch ) )
			return;

		if ( get_skill_percent( ch, gsn_ride ) == 0 ){
			send_to_char("§A¤S¤£·|ÃM°¨¡I¶R§¤ÃM°µ¤°»ò¡I\n\r",ch);
			return;
		}

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
		if ( !pRoomIndexNext ){
			bug( "Do_buy: bad horse shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "³o¸Ì¨S¦³½æ³oºØ§¤ÃM.\n\r", ch );
			return;
		}

		in_room     = ch->in_room;
		ch->in_room = pRoomIndexNext;
		horse       = get_char_room( ch, arg );
		ch->in_room = in_room;

		if ( !horse || !IS_SET( horse->act, ACT_HORSE ) ){
			send_to_char( "³o¸Ì¨S¦³½æ³oºØ§¤ÃM.\n\r", ch );
			return;
		}

		if ( ch->gold < horse->level * horse->level * horse->level ){
			send_to_char( "§A¶R¤£°_.\n\r", ch );
			return;
		}

		if ( !can_see( horse, ch ) ){
			send_to_char( "§A¬Dªº§¤ÃM¬Ý¤£¨ì§A¡I\n\r", ch );
			return;
		}
    
		if ( ch->level < horse->level ){
			send_to_char( "§Aªºµ¥¯Å¤£°÷.\n\r", ch );
			return;
		}

		if ( ( qmark = ch->qmark ) )
		for( qmark = ch->qmark; qmark != NULL; qmark = qmark->next )
			if ( !str_cmp( qmark->mark_name, "bought_horse_timer" ) )
			{
				send_to_char( "§A¤£¬O¤£¤[«e¤~¶R¹L¤@¤Ç°¨¶Ü¡H\n\r", ch );
				return;
			}

		ch->gold -= horse->level * horse->level * horse->level ;
		horse   = create_mobile( horse->pIndexData );
		horse->level = horse->pIndexData->level;
		//  SET_BIT( ch->act,    PLR_BOUGHT_PET );
		SET_BIT( horse->act,     ACT_HORSE    );
		SET_BIT( horse->act,  ACT_SENTINEL      );
		//  SET_BIT( horse->affected_by, AFF_CHARM    );
  
		one_argument( argument, arg );

		if ( arg[0] != '\0' ){
			sprintf( buf, "%s %s", horse->name, arg );
			free_string( horse->name );
			horse->name = str_dup( buf );
		}

		char_to_room( horse, ch->in_room );
		mprog_repop_trigger(horse);
		add_rember( horse , ch );
		//if ( !ch->clan )
		add_follower( horse, ch );
		send_to_char( "½Ð¦n¦nµ½«Ý§Aªº§¤ÃM.\n\r", ch );
		act( "$n ¶±¥Î¤F¤@°¦ $N.", ch, NULL, horse, TO_ROOM );
		sprintf( arg, "%s ", ch->name );
		strcat( arg, "bought_horse_timer 30 0 0 0 0 1" );
		do_mpadd_qmark( horse, arg );
		return;
	}
	else
	{   // keeper ¹ï¸Ü¤W¦â 2022/05/05
		OBJ_DATA  *obj;
		CHAR_DATA *keeper;
		int cost;
		int item_count = 1; /* buy only one by default */

		if ( is_number( arg ) )
		{
			if( arg2[0] != '\0' ) {
				item_count = atoi( arg );
				strcpy( arg, arg2 );
				strcpy( arg2, arg3 );
			}
		}

		if ( !( keeper = find_keeper( ch, arg2 ) ) )
			return;

		if( is_number(arg) )
		{
			int i = 1;
			type = atoi(arg);
			for( obj = keeper->carrying; obj; obj = obj->next_content, i++ )
			{
				if ( obj->wear_loc != WEAR_NONE
					|| ( cost = get_cost( keeper, obj, TRUE ) ) < 0
					|| not_for_sale( ch, keeper, obj ) ) // ·s¼W qmark §P©w«D½æ«~ 2022/05/05
				{
					--i;
					continue;
				}
				if(i == type) break;
			}
		}else{
			obj = get_obj_carry( keeper, arg );
		}

		cost = get_cost( keeper, obj, TRUE );

		if ( cost <= 0 || !can_see_obj( ch, obj ) || not_for_sale( ch, keeper, obj ) ) // ·s¼W qmark §P©w«D½æ«~ 2022/05/05
		{
			act( "$n"HIG"§i¶D§A¡u§Ú¨S¦³½æ³o¼ËªF¦è¡A½Ð¥Î 'list' ¬Ý¬Ý§Úªº³fª«¡C¡v"NOR"", keeper, NULL, ch, TO_VICT );
			ch->reply = keeper;
			return;
		}

		if ( item_count < 1 ){
			send_to_char( "­n¶R¦h¤Ö¡H\n\r", ch );
			return;
		}

		if ( ch->gold < ( cost * item_count ) )
		{
			if ( item_count == 1 ){
				act( "$n"HIG"§i¶D§A¡u§A¨­¤W¨S¦³¨¬°÷ªº¿ú¡C¡v"NOR"", keeper, obj, ch, TO_VICT );
				ch->reply = keeper;
				return;
			}else{
				char buf [ MAX_STRING_LENGTH ];

				if ( ( ch->gold / cost ) > 0 )
					sprintf( buf, "$n"HIG"§i¶D§A¡u§Aªº¿ú¥u°÷¶R %d ­Ó¡C¡v"NOR"", ( ch->gold / cost ) );
				else
					sprintf( buf, "$n"HIG"§i¶D§A¡u§A³s¤@­Ó¤]¶R¤£°_¡C¡v"NOR"" );

				act( buf, keeper, obj, ch, TO_VICT );
				ch->reply = keeper;
				return;
			}
		}
  
		if ( obj->level > ch->level ){
			act( "$n"HIG"§i¶D§A¡u§AÁÙ¤£¯à¨Ï¥Î $p¡C¡v"NOR"",
			keeper, obj, ch, TO_VICT );
			ch->reply = keeper;
			return;
		}

		if ( ch->carry_number + ( item_count * get_obj_number( obj ) ) > can_carry_n( ch ) ){
			send_to_char( "§AµLªk¦A®³°Ê§ó¦hªF¦è¤F.\n\r", ch );
			return;
		}

		if ( ch->carry_weight + ( item_count * get_obj_weight( obj ) ) > can_carry_w( ch ) ){
			send_to_char( "§AµLªk¦A®³°Ê§ó¦h­«¶q¤F.\n\r", ch );
			return;
		}

		if ( ( item_count > 1 ) && !IS_SET( obj->extra_flags, ITEM_INVENTORY ) ){
			act( "$n"HIG"§i¶D§A¡u©êºp, $p ¥u³Ñ¤U¤@­Ó¤F¡C¡v"NOR"", keeper, obj, ch, TO_CHAR );
			ch->reply = keeper;
			return;
		}

		//Anti-duper by Chris
		if ( obj && keeper->pIndexData->pShop && keeper->pIndexData->pShop->profit_buy < 100){
			bugf("Dupe by Shops : %s try to buy %s to mob %d for %d percents cost!",
			ch->name, obj->short_descr, keeper->pIndexData->vnum, keeper->pIndexData->pShop->profit_buy );
		}


		if ( item_count == 1 ){
			act( "§A¶R¤U $p.", ch, obj, NULL, TO_CHAR );
			act( "$n ¶R¤U $p.", ch, obj, NULL, TO_ROOM );
		}else{
			char buf [ MAX_STRING_LENGTH ];

			sprintf( buf, "$n ¶R¤U %d * $p.", item_count );
			act( buf, ch, obj, NULL, TO_ROOM );
			sprintf( buf, "§A¶R¤U %d * $p.", item_count );
			act( buf, ch, obj, NULL, TO_CHAR );
		}

		ch->gold -= cost * item_count;

		if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
		for ( ; item_count > 0; item_count-- )
		{
			obj = fcreate_object( obj->pIndexData, obj->level );
			obj_to_char( obj, ch );
		}else{
			obj_from_char( obj );
			obj_to_char( obj, ch );
		}

		// ·s¼W obj_act buy °w¹ï¶Rª««~ªº¨¤¦â 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("buy", ch, obj);
		}

		// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
		if(!obj) return;

		// ·s¼W obj_act buy_target °w¹ï³Q¶Rª««~ªº¨¤¦â(npc °Ó¤H) 2022/02/12
		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("buy_target", keeper, obj);
		}

		return;
    }
}



void do_list( CHAR_DATA *ch, char *argument )
{
	char buf  [ MAX_STRING_LENGTH   ];
	char buf1 [ MAX_STRING_LENGTH*4 ];
	int wear_filter = 0;
	//static const   WEAR_MAP wear_table[17]=
	static const   WEAR_MAP wear_table[18]=
	{
		{   "about",      ITEM_WEAR_ABOUT},
		{   "arm",        ITEM_WEAR_ARMS},
		{   "arrow",      ITEM_WEAR_ARROW},
		{   "body",       ITEM_WEAR_BODY },
		{   "feet",       ITEM_WEAR_FEET },
		{   "finger",     ITEM_WEAR_FINGER },
		{   "hand",       ITEM_WEAR_HANDS},
		{   "head",       ITEM_WEAR_HEAD },
		{   "hold",       ITEM_HOLD},
		{   "leg",        ITEM_WEAR_LEGS },
		{   "neck",       ITEM_WEAR_NECK },
		{   "shield",     ITEM_WEAR_SHIELD},
		{   "twohand",    ITEM_TWO_HAND},
		{   "waist",      ITEM_WEAR_WAIST},
		{   "weapon",     ITEM_WIELD},
		{   "wrist",      ITEM_WEAR_WRIST},
		{   "ammo",       ITEM_WEAR_AMMO}, // ¼uÃÄ¦ì¸m 2022/04/30
		{   "ride",       ITEM_WEAR_RIDE},
	};

	buf1[0] = '\0';

	if ( IS_SET( ch->in_room->room_flags, ROOM_PET_SHOP ) )
	{
		CHAR_DATA *pet;
		ROOM_INDEX_DATA *pRoomIndexNext;
		bool     found;

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

		if ( !pRoomIndexNext ){
			bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "§AµLªk³o»ò°µ .\n\r", ch );
			return;
		}

		found = FALSE;

		for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
		{
			if ( IS_SET( pet->act, ACT_PET ) ){
				if ( !found ){
					found = TRUE;
					strcat( buf1, "Ãdª«¥X°â:\n\r" );
				}
				sprintf( buf, "[%2d] %8d - %s\n\r",
				pet->level,
				pet->level * pet->level * pet->level,
				pet->short_descr );
				strcat( buf1, buf );
			}
		}
		if ( !found )
			send_to_char( "©êºp¡A¥»©±¥Ø«e¯Ê³f.\n\r", ch );

		send_to_char( buf1, ch );
		return;
	}
	else if ( IS_SET( ch->in_room->room_flags, ROOM_HORSE_SHOP ) )
	{
		CHAR_DATA *horse;
		ROOM_INDEX_DATA *pRoomIndexNext;
		bool     found;

		pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
		if ( !pRoomIndexNext ){
			bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
			send_to_char( "§AµLªk³o»ò°µ .\n\r", ch );
			return;
		}

		found = FALSE;

		for ( horse = pRoomIndexNext->people; horse; horse = horse->next_in_room )
		{
			if ( IS_SET( horse->act, ACT_HORSE ) ){
				if ( !found ){
					found = TRUE;
					strcat( buf1, "§¤ÃM¥X°â:\n\r" );
				}
				sprintf( buf, "[%2d] %8d - %s\n\r",
				horse->level,
				horse->level * horse->level * horse->level,
				horse->short_descr );
				strcat( buf1, buf );
			}
		}
		if ( !found )
			send_to_char( "©êºp¡A¥»©±¥Ø«e¯Ê³f.\n\r", ch );

		send_to_char( buf1, ch );
		return;
	}else{
		OBJ_DATA  *obj;
		CHAR_DATA *keeper;
		char arg  [ MAX_INPUT_LENGTH ];
		char arg2 [ MAX_INPUT_LENGTH ];
		int cost;
		bool found;
		int count = 0;
		int i;

		argument = one_argument( argument, arg );
		one_argument( argument, arg2 );

		if ( !( keeper = find_keeper( ch, arg2 ) ) )
			return;
		
		found = FALSE;
		if( arg[0] != '\0' ){
			// add wear loc ammo 2022/04/30
			//for( i = 0; i < 16 ; ++i )
			for( i = 0; i < 17 ; ++i )
			{
				if( !str_cmp( arg, wear_table[i].name) ){
					wear_filter = wear_table[i].place;
					break;
				}
			}
		}
		for ( i = 1,obj = keeper->carrying; obj; obj = obj->next_content,++i )
		{
			// obj ³Q°Ó¤H¬ïµÛ ©Î¬O ½æ»ù¤p©ó 0  «h¤£·|Åã¥Ü¦b list ¤¤
			if ( obj->wear_loc != WEAR_NONE
				|| ( cost = get_cost( keeper, obj, TRUE ) ) < 0
				|| not_for_sale( ch, keeper, obj ) ) // ·s¼W qmark §P©w«D½æ«~ 2022/05/05
			{
				--i;
				continue;
			}

			if ( can_see_obj( ch, obj )
				&& (arg[0] == '\0'
					|| !str_cmp( arg, "all" )
					|| (CAN_WEAR(obj, wear_filter))
					|| is_name( arg, obj->name )) )
			{
				if ( !found )
				{
					found = TRUE;
					strcat( buf1, "[½s¸¹   Lv  »ù ¿ú ] ª« «~ ¦W ºÙ\n\r" );
				}

				sprintf( buf, "[%4d   %2d %7d] %s.\n\r", i ,obj->level, cost, capitalize( obj->short_descr ) );
				++count;
				strcat( buf1, buf );
			}

			if ( count >= 35 )
			{
				count = 0;
				send_to_char(buf1, ch);
				buf1[0] = '\0';
			}
		}

		if ( !found )
		{
			if ( arg[0] == '\0' )
				send_to_char( "³o¸Ì¶R¤£¨ìªF¦è.\n\r", ch );
			else
				send_to_char( "¥L¤£½æ¨º¼ËªF¦è.\n\r", ch );
			return;
		}

		send_to_char( buf1, ch );
		return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *keeper;
	char       buf  [ MAX_STRING_LENGTH ];
	char       arg  [ MAX_INPUT_LENGTH  ];
	char       arg2 [ MAX_INPUT_LENGTH  ];
	int        cost;

	argument = one_argument( argument, arg );
	one_argument( argument, arg2 );

	if ( arg[0] == '\0' )
	{
		send_to_char( "­n½æ¤°»ò?\n\r", ch );
		return;
	}

	if ( !( keeper = find_keeper( ch, arg2 ) ) )
		return;

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		act( "$n §i¶D§A, '§A¨S¦³¨º¼ËªF¦è'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
    }

	if ( !can_drop_obj( ch, obj ) )
	{
		send_to_char( "§AµLªk©ñ¤U¥¦.\n\r", ch );
		return;
	}

	if ( !can_see_obj( keeper, obj ) )
	{
		act( "$n §i¶D§A, '§Ú¬Ý¤£¨ì§A·Q½æªºªF¦è'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	//Anti-duper by Chris
	if ( obj && keeper->pIndexData->pShop && keeper->pIndexData->pShop->profit_sell > 100)
	{
		bugf("Dupe by Shops : %s try to sell %s to mob %d for %d percents profit!",
			ch->name, obj->short_descr, keeper->pIndexData->vnum, keeper->pIndexData->pShop->profit_sell );
	}
  
  
    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0
		|| obj->level > LEVEL_IMMORTAL 
		|| IS_SET(obj->extra_flags, ITEM_CLAN))
	{
		act( "$n ·n·nÀY¡A¬Ý¨Ó¹ï $p ¤@ÂI¿³½ì¤]¨S¦³.", keeper, obj, ch, TO_VICT );
		return;
	}

	if ( IS_SET( obj->extra_flags, ITEM_POISONED ) )
	{
		act( "$n §i¶D§A, '§Ú¤£¶R¦³¬rªºªF¦è'", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	sprintf( buf, "§A¥H %d ªTª÷¹ôªº»ù¿ú½æ±¼¤F $p.", cost );
	act( buf, ch, obj, NULL, TO_CHAR );
	act( "$n ½æ±¼ $p.", ch, obj, NULL, TO_ROOM );
	ch->gold   += cost;

	if ( obj->item_type == ITEM_TRASH )
	{
		extract_obj( obj );
	}
	else
	{
		obj_from_char( obj );
		obj_to_char( obj, keeper );
	}

	// ·s¼W obj_act sell °w¹ï½æª««~ªº¨¤¦â 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("sell", ch, obj);
	}

	// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
	if(!obj) return;

	// ·s¼W obj_act sell_target °w¹ï¦¬ÁÊª««~ªº¨¤¦â(npc °Ó¤H) 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("sell_target", keeper, obj);
	}

	return;
}



void do_value( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA  *obj;
	CHAR_DATA *keeper;
	char       buf  [ MAX_STRING_LENGTH ];
	char       arg  [ MAX_INPUT_LENGTH  ];
	char       arg2 [ MAX_INPUT_LENGTH  ];
	int        cost;

	argument = one_argument( argument, arg );
	one_argument( argument, arg2 );

	if ( arg[0] == '\0' )
	{
		send_to_char( "­n¬°¤°»ò¦ô»ù?\n\r", ch );
		return;
	}

	if ( !( keeper = find_keeper( ch, arg2 ) ) )
		return;

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		act( "$n §i¶D§A, '§A¨S¦³¨º¼ËªF¦è'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
		send_to_char( "§AµLªk©ñ¤U¥¦.\n\r", ch );
		return;
	}

	if ( !can_see_obj( keeper, obj ) )
	{
		act( "$n §i¶D§A, '§Ú¤£¾å±o«ç»òÅ²©wªÅ®ð'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
	{
		act( "$n ·n·nÀY¡A¬Ý¨Ó¹ï $p ¤@ÂI¿³½ì¤]¨S¦³.", keeper, obj, ch, TO_VICT );
		return;
	}

	if ( IS_SET( obj->extra_flags, ITEM_POISONED ) )
	{
		act( "$n §i¶D§A, '§Ú¤£¶R¦³¬rªºªF¦è'", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	}

	sprintf( buf, "$n §i¶D§A, '§ÚÄ@·N¥X %d ªTª÷¹ôªº»ù¿ú¶R¤U $p'.", cost );
	act( buf, keeper, obj, ch, TO_VICT );
	ch->reply = keeper;

	return;
}

/* Poison weapon by Thelonius for EnvyMud */
void do_poison_weapon( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *pobj;
	OBJ_DATA *wobj;
	AFFECT_DATA *paf;  // 2021/11/10
	char arg [ MAX_INPUT_LENGTH ];
	int sklv = get_skill_level(ch, gsn_poison_weapon);
	/* Don't allow mobs or unskilled pcs to do this */
	if ( IS_NPC( ch )
		|| ( !IS_NPC( ch ) && get_skill_percent( ch, gsn_poison_weapon ) == 0 ) )
	{
		send_to_char( "¤°»ò¡H\n\r", ch );
		return;
    }

	one_argument( argument, arg );

	if ( arg[0] == '\0' )                
	{
		send_to_char( "§A­n¦b¤°»òªF¦è¤W©Ù¬r¡H\n\r", ch );
		return;
	}
	if ( ch->fighting )               
	{
		send_to_char( "§A¥¿¦b¾Ô°«¡A½w¤£¥X¤â¨Ó.\n\r", ch );
		return;
	}
	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "§A¨S¦³³o¼ËªF¦è.\n\r", ch );
		return;
	}
	/*
	if ( obj->item_type != ITEM_WEAPON  )
	{
		send_to_char( "¨º¤£¬OªZ¾¹.\n\r", ch );
		return;
	}
	*/
	// ·s¼W arrow ¤Î dart ©Ù¬r 2021/11/10
	if ( obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARROW && obj->item_type != ITEM_DART )
	{
		send_to_char( "¨º¥óª««~¤£¯à©Ù¬r.\n\r", ch );
		return;
	}
	if ( IS_OBJ_STAT( obj, ITEM_POISONED ) )
	{
		send_to_char( "¨º¤w¸g¦³¬r¤F.\n\r", ch );
		return;
	}
    
	/* Now we have a valid weapon...check to see if we have the powder. */
	for ( pobj = ch->carrying; pobj; pobj = pobj->next_content )
	{
		if ( pobj->pIndexData->vnum == OBJ_VNUM_BLACK_POWDER )
			break;
	}
	if ( !pobj )
	{
		send_to_char( "§A¨S¦³¬r¯».\n\r", ch );
		return;
	}

	/* Okay, we have the powder...do we have water? */
	for ( wobj = ch->carrying; wobj; wobj = wobj->next_content )
	{
		// Á×§K¨Ï ¯S®í¬ÃÂÃ«~ ®³¥h¿N±¼
		if ( IS_SET(wobj->pIndexData->souvenir, Z4) )
			continue;
		if ( wobj->item_type == ITEM_DRINK_CON 
			&& wobj->value[1] > 0
			&& wobj->value[2]  == 0 )
			break;
	}
	if ( !wobj )
	{
		send_to_char( "§A±o­n¦³¤ô¨Ó²V¦X¬r¯».\n\r", ch );
		return;
	}

	/* Great, we have the ingredients...but is the thief smart enough? */
	if ( !IS_NPC( ch ) &&  get_curr_wis( ch )+sklv/2 < 11 && get_curr_int( ch )+sklv/2 < 11)
	{
		send_to_char( "§A²Â¤â²Â¸}ªº¡Aµ²ªG¥¢±Ñ¤F...\n\r", ch );
		return;
	}
	/* And does the thief have steady enough hands? */
	if ( !IS_NPC( ch ) && ( get_curr_dex( ch )+sklv/2 < 17
		|| ch->pcdata->condition[COND_DRUNK] > 0 ) )
	{
		send_to_char( "§A¤@­Ó¤£¤p¤ß¡A§â§@¦nªº¬r²Gµ¹¥´Â½¤F¡I\n\r", ch );
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_poison_weapon].beats );

	/* Check the skill percentage */
	if ( !IS_NPC( ch )
	//&& number_percent( ) > ch->pcdata->learned[gsn_poison_weapon] )
		&& number_percent() > sklv*2 + get_skill_percent(ch, gsn_poison_weapon))
	{
		send_to_char( "§A¤@­Ó¤£¤p¤ß¡A§â¬r²GÂq¨ì¤F¦Û¤v¨­¤W¡I\n\r", ch );
		//damage( ch, ch, ch->level, gsn_poison_weapon, WEAR_NONE );
		damage( ch, ch, ch->level, gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		act( "$n §â¬r²GÂq±o¨ì³B³£¬O¡I", ch, NULL, NULL, TO_ROOM );
		extract_obj( pobj );
		extract_obj( wobj );
		return;
	}

    /* Well, I'm tired of waiting.  Are you? */
    act( "§A§â $p ­Ë¶i $P, »s¦¨¬r²G¡C", ch, pobj, wobj, TO_CHAR );
    act( "$n §â $p ­Ë¶i $P, »s¦¨¤F¬r²G¡C",ch, pobj, wobj, TO_ROOM );
    act( "§A§â¬r²G§¡¤Ã¦a©Ù¦b $p ¤W¡C",ch, obj, NULL, TO_CHAR  );
    act( "$n §â¬r²G§¡¤Ã¦a©Ù¦b $p¤W¡C",ch, obj, NULL, TO_ROOM  );

	/*  ©Þ°£ timer ³]©w¤Î item_poison_weapon
	if( obj->timer <= 0 )
		SET_BIT( obj->extra_flags, ITEM_POISONED_WEAPON );
	if( obj->timer < 0 )
	{
		obj->timer = 40 + ch->level + sklv * sklv * 8;

		// obj->cost *= ch->level;

		// Set an object timer.  Dont want proliferation of poisoned weapons
		//obj->timer = 40 + ch->level + sklv * sklv * 8;
	 
		if ( IS_OBJ_STAT( obj, ITEM_BLESS ) )
			obj->timer = obj->timer + ( obj->timer * ( sklv / 2 ) / 10 );

		if ( IS_OBJ_STAT( obj, ITEM_MAGIC ) )
			obj->timer = UMAX( 20, obj->timer - 60 );

		// WHAT?  All of that, just for that one bit?  How lame. ;)
	}
	*/

	SET_BIT( obj->extra_flags, ITEM_POISONED );
	// ¥[¤J©Ù¬rª««~ªº¬r©Ê§P©w 2021/11/11
	paf            = new_affect();
	paf->type      = obj->pIndexData->vnum;
	paf->duration  = 5 + sklv * sklv;
	paf->location  = APPLY_TOXICITY_POISON;
	paf->modifier  = pobj->level * ( 1 + sklv/6 );
	paf->bitvector = 0;
	paf->next      = obj->affected;
	obj->affected  = paf;

	act( "³Ñ¤Uªº¬r²G§â $p µ¹»G»k¤F.", ch, wobj, NULL, TO_CHAR );
	act( "³Ñ¤Uªº¬r²G§â $p µ¹»G»k¤F.", ch, wobj, NULL, TO_ROOM );
	extract_obj( pobj );
	extract_obj( wobj );

	return;
}

/* Contributed by BoneCrusher of EnvyMud. */
void do_donate( CHAR_DATA *ch, char *arg )
{
	OBJ_DATA *container;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	char      arg1[MAX_INPUT_LENGTH];

	arg = one_argument( arg, arg1 );

	if ( arg1[0] == '\0' )
	{
		send_to_char( "§A­n®½Äm¤°»ò¡H\n\r", ch );
		return;
	}

	if( !ch->in_room ) return;

    /*
		if ( ( container = get_obj_world( ch, "donation" ) ) == NULL )
		{
			send_to_char( "³o­Ó¥@¬É¤W¨S¦³©^Äm½c.\n\r", ch );
			return;
		}
	*/

	container = NULL;
	for( container = object_list ; container ; container = container->next )
	{
		if( IS_SET( container->extra_flags, ITEM_DONATION_PIT)) {
			if( !container->in_room || container->in_room->area->recall != ch->in_room->area->recall)
				continue;
			else
				break;
		}
	}
	if( !container ) {
		send_to_char("³o­Ó¥@¬É¤W¨S¦³©^Äm½c¡C\n\r", ch);
		return;
	}
	if( get_obj_number(container) > 100 ) {
		send_to_char("©^Äm½cº¡¤F¡C\n\r", ch);
		return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
		if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
		{
			send_to_char( "§A¨S¦³³o¼ËªF¦è.\n\r", ch );
			return;
		}

		if ( !can_drop_obj( ch, obj ) || IS_SET(obj->extra_flags, ITEM_NOSAC))
		{
			send_to_char( "§AµLªk©ß¶}³o¼ËªF¦è.\n\r", ch );
			return;
		}
  
		if ( get_obj_weight( obj ) + get_obj_weight( container ) > container->value[0] )
		{
			send_to_char( "³o¼ËªF¦è¸Ë¤£¶i©^Äm½c.\n\r", ch );
			return;
		}

		act( "§A©^Äm $p ¶i $P.", ch, obj, container, TO_CHAR );

		if ( obj->item_type == ITEM_TRASH
			|| obj->item_type == ITEM_FOOD
			|| obj->item_type == ITEM_KEY
			|| obj->item_type == ITEM_PILL )
		{
			// ·s¼W obj_act donate 2022/02/12
			if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
				oprog_act_trigger("donate", ch, obj);
			}

			// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
			if(!obj){
				save_char_obj( ch );
				return;
			}

			//act( "§A©^Äm $p ¶i $P.", ch, obj, container, TO_CHAR );
			extract_obj( obj );
			return;
		}

		act( "$n ©^Äm $p ¶i $P.", ch, obj, container, TO_ROOM );
		send_to_room( "©^Äm½c¸Ì¶Ç¨Ó¤@°}ÅTÁn¡I", container->in_room );
		obj_from_char( obj );
		obj_to_obj( obj, container );
		// ·s¼W obj_act donate 2022/02/12

		if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
			oprog_act_trigger("donate", ch, obj);
		}

		// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
		if(!obj){
			save_char_obj( ch );
			return;
		}

		// ±Ô­z²¾¦Ü obj ±q ch ¨­¤W²¾°£«e 2022/02/12
		//act( "$n ©^Äm $p ¶i $P.", ch, obj, container, TO_ROOM );
		//act( "§A©^Äm $p ¶i $P.", ch, obj, container, TO_CHAR );
		//send_to_room( "©^Äm½c¸Ì¶Ç¨Ó¤@°}ÅTÁn¡I", container->in_room );
	}
	else
	{
		for ( obj = ch->carrying; obj; obj = obj_next )
		{
			obj_next = obj->next_content;

			if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
				&& can_see_obj( ch, obj )
				&& obj->wear_loc == WEAR_NONE
				&& obj != container
				&& can_drop_obj( ch, obj )
				&& get_obj_weight( obj ) + get_obj_weight( container )
				<= container->value[0] )
			{
				act( "§A©^Äm $p ¶i $P.", ch, obj, container, TO_CHAR );

				if ( obj->item_type == ITEM_TRASH
					|| obj->item_type == ITEM_FOOD
					|| obj->item_type == ITEM_KEY
					|| obj->item_type == ITEM_PILL )
				{
					// ·s¼W obj_act donate 2022/02/12
					if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
						oprog_act_trigger("donate", ch, obj);
					}

					// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
					if(!obj) continue;

					//act( "§A©^Äm $p ¶i $P.", ch, obj, container, TO_CHAR );
					extract_obj( obj );
					continue;
				}

				act( "$n ©^Äm $p ¶i $P.", ch, obj, container, TO_ROOM );
				send_to_room( "©^Äm½c¸Ì¶Ç¨Ó¤@°}ÅTÁn¡I", container->in_room );
				obj_from_char( obj);
				obj_to_obj( obj, container );

				// ·s¼W obj_act donate 2022/02/12
				if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
					oprog_act_trigger("donate", ch, obj);
				}

				// obj ¥i¯à¦b obj_act ®É³Q purge 2022/02/13
				if(!obj) continue;

				// ±Ô­z²¾¦Ü obj ±q ch ¨­¤W²¾°£«e 2022/02/12
				//act( "$n ©^Äm $p ¶i $P.", ch, obj, container, TO_ROOM );
				//act( "§A©^Äm $p ¶i $P.", ch, obj, container, TO_CHAR );
				//send_to_room( "©^Äm½c¸Ì¶Ç¨Ó¤@°}ÁnÅT¡I\n\r", container->in_room );
			}
		}
	}

	save_char_obj( ch );
	return;

}


void do_register( CHAR_DATA *ch, char *arg )
{
	CHAR_DATA *mob;
	int cost = ch->level * 5000;
	char buf[MAX_STRING_LENGTH];

	if ( IS_NPC( ch ) )
		return;

	mob = NULL;
	for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
	{
		if ( IS_NPC( mob ) && mob->pIndexData->vnum == MOB_VNUM_SECRETARY )
			break;
	}

	if ( !mob )
	{
		send_to_char( "³o¸Ì¤£¯à¶i¦æ PK µn°O.\n\r", ch );
		return;
	}

	if ( ch->level < 25 )
	{
		do_say( mob, "§Aªºµ¥¯ÅÁÙ¤Ó§C¤F." );
		return;
	}

	/*if ( ch->level > LEVEL_HERO + 6 )
	{
		do_say( mob, "¯«¤£¥i¥H°Ñ¥[ PK." );
		return;
	} */

	if( IS_SET( ch->act, PLR_REGISTER ))
	{
		if( ch->gold < cost * 5 )
		{
			sprintf( buf, "§A¨S¦³¨¬°÷ªº¿ú¨Ó°h¥X PK." );
			do_say( mob, buf );
			return;
		}
		sprintf( buf, "§A¥I¥X¤F %d ªTª÷¹ô¡A°h¥X PK.", cost * 5 );
		//do_say( mob, buf );
		send_to_char( buf, ch);
		ch->gold -= 5 * cost;
		REMOVE_BIT( ch->act, PLR_REGISTER);
		return;
	}

	if ( ch->gold < cost )
	{
		sprintf( buf, "§A¨S¦³¨¬°÷ªº¿ú¨Ó¥Iµn°O¶O." );
		do_say( mob, buf );
		return;
	}

	sprintf( buf, "§A¥I¥X¤F %d ªTª÷¹ô¡Aµn°O°Ñ¥[ PK.", cost );
	//do_say( mob, buf );
	send_to_char( buf, ch);

	ch->gold -= cost;

	SET_BIT( ch->act, PLR_REGISTER );

	return;

}

void do_deposit( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	char arg [ MAX_INPUT_LENGTH ];
	int num;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		if ( IS_NPC(vch) && IS_SET( vch->act, ACT_BANKER ) ) // ¥[¤J npc ­­©w , ¥²§K plr_thief ¤]·|³Q·í banker 2022/11/27
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

	if ( vch )
	{
		if ( num <= ch->gold )
		{
			if(num > 2147483647 - ch->bank){ // MAXIMUM Integer 2147483647
				send_to_char("§Aªº¤áÀY¦s¤£¤F³o»ò¦h¿ú¡C\n\r", ch);
				return;
			}
			ch->gold -= num;
			ch->bank += num;
			act( "$N¦V§A¤@Áù°`»¡¡GÁÂÁÂ±zªº¥úÁ{¡A¤j³°³sÂê»È¦æ¥Ã»·ºÜ¸Û¬°±zªA°È¡C", ch, NULL, vch, TO_CHAR );
			act( "$N¦V$n¤@Áù°`»¡¡GÁÂÁÂ±zªº¥úÁ{¡A¤j³°³sÂê»È¦æ¥Ã»·ºÜ¸Û¬°±zªA°È¡C", ch, NULL, vch, TO_ROOM );
			return;
		}
		else
		{
			send_to_char( "³á¡I§A¨­¤W¨S¨º»ò¦h¿ú¡C\n\r", ch );
			return; 
		}
	}
	else
		send_to_char( "§A§ä¤£¨ì¤HÀ°§A¦s¿ú¡C\n\r", ch );
	return;
}

void do_withdraw( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	char arg [ MAX_INPUT_LENGTH ];
	int num;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		if ( IS_NPC(vch) && IS_SET( vch->act, ACT_BANKER ) ) // ¥[¤J npc ­­©w , ¥²§K plr_thief ¤]·|³Q·í banker 2022/11/27
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

	if ( vch )
	{
		if ( num <= ch->bank )
		{
			if(num > 2147483647 - ch->gold){ // MAXIMUM Integer 2147483647
				send_to_char("§Aªº¨­¤W±a¤£¤F³o»ò¦h¿ú¡C\n\r", ch);
				return;
			}
			ch->gold += num;
			ch->bank -= num ;
			act( "$N¦V§A¤@Áù°`»¡¡G³o¬O±zªºª÷¹ô¡A½ÐÂI¦¬¡CÁÂÁÂ±zªº¥úÁ{¡C", ch, NULL, vch, TO_CHAR );
			act( "$N¦V$n¤@Áù°`»¡¡G³o¬O±zªºª÷¹ô¡A½ÐÂI¦¬¡CÁÂÁÂ±zªº¥úÁ{¡C", ch, NULL, vch, TO_ROOM );
			return;
		}
		else
		{
			send_to_char( "·m§T°Ú¡H§A­þ¨Ó¨º»ò¦h¿ú¡H\n\r", ch );
			return; 
		}
	}
	else
		send_to_char( "§A§ä¤£¨ì¤HÀ°§A´£¿ú¡C\n\r", ch );
	return;
}

void do_store( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *vobj;
	CHAR_DATA *vch;
	char buf [10];
	int cost;
	bool fBank = TRUE;
	int number = 0;

	if ( IS_NPC( ch ) )
		return;

	for( vch = ch->in_room->people; vch; vch = vch->next_in_room ){
		// ¥[¤J npc ­­©w, ¨Ã­×§ï eqbanker ªº§P©w¥Ñ name ÅÜ§ó¬° qmark 2022/12/01
		if( !IS_NPC(vch) ) continue;
		//if ( is_name( "sharas-eqb", vch->name ) )
		if( is_qmark(vch, "is_eqbanker") )
			break;
	}
	if ( !vch )
	{
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¤£¦b³o¨à¡C\n\r", ch );
		return;
	}

	if( ch->in_room->area->recall != ROOM_VNUM_TEMPLE){
		send_to_char("¥u¦³¥v¥d©Ô¨©«°¥i¥H¨Ï¥ÎÂÂ¸Ë³Æ»È¦æ¡C\n\r", ch);
		return;
	}

	if ( argument[0] == '\0' )
	{
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u±z¤£»¡­n¦s¤°»ò¡A§Ú¤]À°¤£¤W¦£¡C¡v\n\r", ch );
		return;
	}

	for( obj = ch->eqbank; obj ; obj = obj->next_content ){
		++number;
	}

	if( number >= 50 ) {
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó½KµÛ¬Ü»¡¡G¡u©êºp¡A±zªº¤áÀY¤w¸gº¡¤F¡A¶}­Ó·sªº­Ü®w§a¡C¡v\n\r", ch );
		return;
	}

	if ( ( obj = get_obj_carry( ch, argument ) ) )
	{
		cost = ( obj->level < 10 ) ? 10000 :
		( obj->level < 36 ) ? obj->level * 200 : obj->level * 500;
		if ( ch->bank < cost )
		{
			fBank = FALSE;
			if( ch->gold < cost ) {
				send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó­W¯ºµÛ»¡¡G¡u¹ï¤£°_¡A¨S¦³¨¬°÷ªº«OºÞ¶O§ÚµLªk¶±¥Î«O¥þ¤H­û¡C¡v\n\r", ch );
				return;
			}
		}
		if ( ( ch->level < ( obj->level - 5 ) && ch->class < 5 )
			|| obj->item_type == ITEM_KEY
			|| IS_SET( obj->extra_flags, ITEM_NOSAVE )
			|| IS_SET( obj->extra_flags, ITEM_UNIQUE)
			|| obj->deleted 
			|| obj->item_type == ITEM_CONTAINER
			|| obj->timer > 0 )
		{
			send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóÁqÁqªÓ»¡¡G¡u¹ï¤£°_¡A³oÃþ¸Ë³Æ§Ú­Ì¤£¦¬¡C¡v\n\r", ch );
			return;
		}
		obj_from_char( obj );
		if ( obj == object_list )
		{
			object_list = obj->next;
			obj->next = NULL; 
		}
		else for ( vobj = object_list; vobj; vobj = vobj->next )
		{
			if ( obj == vobj->next )
			{
				vobj->next = obj->next;
				obj->next = NULL;
				break;
			}
		}
		obj->next_content = ch->eqbank;
		ch->eqbank = obj;
		if( fBank )
			ch->bank -= cost;
		else
			ch->gold -= cost;
		// ²¾°£ to_room Á×§K¤j¶q¦s¨úª««~³y¦¨¥L¤Hµe­±­â¶Ã 2022/02/01
		//act( "¸Ë³Æ»È¦æ¦ÑÁó¤p¤ß¦a±N$nªº$p®³¨ì«á­±ªº«OÀI®w¦s©ñ¡C", ch, obj, NULL, TO_ROOM );
		act( "¸Ë³Æ»È¦æ¦ÑÁó¤p¤ß¦a±N§Aªº$p®³¨ì«á­±ªº«OÀI®w¦s©ñ¡C", ch, obj, NULL, TO_CHAR );
		if( fBank )
			send_to_char("§A±q»È¦æÂà±b¡A¥I¥X¤F "HIY, ch);
		else
			send_to_char("§A¥I¥X¤F "HIY, ch);
		sprintf(buf, "%d", cost);
		send_to_char(buf, ch);
		send_to_char(NOR " ¤¸ªº«OºÞ¶O¡C\n\r", ch);
		do_save( ch, "" );
		return;
	}
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u§Ú¨S¬Ý¨ì³o¶µ¸Ë³Æ°Ú¡H¡v\n\r", ch );
}

void do_receive( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
	OBJ_DATA  *obj = NULL;
	OBJ_DATA  *vobj;
	// int number;
	int i = 0;
	int count = 0;
	// char  arg [ MAX_INPUT_LENGTH ];
	char  buf [ MAX_STRING_LENGTH ];
	char  no_ansi [ MAX_STRING_LENGTH ];

	if ( IS_NPC( ch ) )
		return;

	// number = number_argument( argument, arg );
	// count = atoi(argument);
	for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
	{
		// ¥[¤J npc ­­©w, ¨Ã­×§ï eqbanker ªº§P©w¥Ñ name ÅÜ§ó¬° qmark 2022/12/01
		if( !IS_NPC(vch) ) continue;
		//if ( is_name( "sharas-eqb", vch->name ) )
		if( is_qmark(vch, "is_eqbanker") )
			break;
	}
	if ( !vch )
	{
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¤£¦b³o¨à¡C\n\r", ch );
		return;
	}


	if( ch->in_room->area->recall != ROOM_VNUM_TEMPLE){
		send_to_char("¥u¦³¥v¥d©Ô¨©«°¥i¥H¨Ï¥ÎÂÂ¸Ë³Æ»È¦æ¡C\n\r", ch);
		return;
	}

	if ( argument[0] == '\0' )
	{
		send_to_char( "§A¥Ø«e¦s¦b¸Ë³Æ»È¦æ«OÀI®wªº¸Ë³Æ¦³¡G\n\r", ch );
		if ( !ch->eqbank )
		{
			send_to_char( "\n\r¤°»ò³£¨S¦³¡I\n\r\n\r", ch );
			return; 
		}

		send_to_char("¡i½s¸¹¡j  ª«  «~  ¦W  ºÙ                                  µ¥¯Å\n\r", ch);
		for ( vobj = ch->eqbank; vobj; vobj = vobj->next_content )
		{
			i++;
			strip_ansi(vobj->short_descr, no_ansi);
			sprintf ( buf, "¡i%4d¡j%-48s  [%2d] \n\r", 
			i,  
			can_see_obj(ch, vobj) ? no_ansi :
			"¤£©úª««~",
			vobj->level
			);
			send_to_char( buf, ch );
			//act( "$p", ch, vobj, NULL, TO_CHAR );
		}
		send_to_char( "\n\r", ch );
		return;
	}
	if ( !is_number( argument ) )
	{
		send_to_char( "½Ð¥Î 'receive ½s¸¹' ¨Ó»â¨ú±zªº¸Ë³Æ¡C\n\r", ch );
		return;
	}
	count = atoi( argument );

	if( !ch->eqbank ) {
		send_to_char( "¸Ë³Æ»È¦æ¦ÑÁóª°ºÃ¦a¬ÝµÛ§A»¡¡G¡u±zªº­Ü®w¸Ì¨S¦³ªF¦è°Ú¡H¡v\n\r", ch );
		return;
	}

	i = 2;
	if( ch->eqbank && count == 1 )
	{
		obj = ch->eqbank;
		if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) )
		{
			do_say(vch, "¹ï¤£°_¡A±z®³¤£°Ê§ó¦hªF¦è¤F¡C");
			return;
		}

		if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) )
		{
			do_say(vch,"¹ï¤£°_¡A±zªº­t­«¤£°÷¤F¡C");
			return;
		}
		ch->eqbank = obj->next_content;
		obj->next_content = NULL;
	}
	else if ( ch->eqbank && ch->eqbank->next_content )
	{
		for( vobj = ch->eqbank ; vobj->next_content; vobj = vobj->next_content )
		{

			// sprintf(buf, "[%d][%d]\n\r", count, i );
			// send_to_char(buf, ch);
			if( count == i )
			{
				obj = vobj->next_content;
				if ( ch->carry_number + get_obj_number(obj)  > can_carry_n( ch ) )
				{
					do_say(vch, "¹ï¤£°_¡A±z®³¤£°Ê§ó¦hªF¦è¤F¡C");
					return;
				}

				if ( ch->carry_weight + get_obj_weight(obj) > can_carry_w( ch ) )
				{
					do_say(vch,"¹ï¤£°_¡A±zªº­t­«¤£°÷¤F¡C");
					return;
				}
				vobj->next_content = obj->next_content;
				obj->next_content = NULL;

				break;
			}
			i++;
		}
	}
	/*
	if ( ch->eqbank && is_pfxname( arg, ch->eqbank->name ) )
	{
	obj = ch->eqbank;
	ch->eqbank = obj->next_content;
	}
	else if ( ch->eqbank && ch->eqbank->next_content )
	for ( vobj = ch->eqbank; vobj->next_content; vobj = vobj->next_content )
	if ( is_pfxname( arg, vobj->next_content->name ) )
	{
	if ( ++count == number )
	{
	obj = vobj->next_content;
	vobj->next_content = obj->next_content;
	break;
	}
	}     
	*/
	if ( obj )
	{
		obj_to_char ( obj, ch );
		obj->next = object_list;
		object_list = obj;
		// ²¾°£ to_room Á×§K¤j¶q¦s¨úª««~³y¦¨¥L¤Hµe­±­â¶Ã 2022/02/01
		act( "¸Ë³Æ»È¦æ¦ÑÁó¦Û«á­±ªº®w©Ð±N$p±·¨Ó¥æµ¹§A¡C", ch, obj, NULL, TO_CHAR );
		//act( "¸Ë³Æ»È¦æ¦ÑÁó¦Û«á­±ªº®w©Ð±N$p±·¨Ó¥æµ¹$n¡C", ch, obj, NULL, TO_ROOM );
		//act( "¸Ë³Æ»È¦æ¦ÑÁó¤@Áù°`»¡¡G¡u½Ð±zÀË¬d¦³µL¥ô¦ó·l¶Ë¡AÁÂÁÂ¥úÁ{¡C¡v", ch, obj, NULL, TO_ROOM );
		act( "¸Ë³Æ»È¦æ¦ÑÁó¤@Áù°`»¡¡G¡u½Ð±zÀË¬d¦³µL¥ô¦ó·l¶Ë¡AÁÂÁÂ¥úÁ{¡C¡v", ch, obj, NULL, TO_CHAR );
		do_save( ch, "" );
		return;
	}
	send_to_char( "¸Ë³Æ»È¦æ¦ÑÁó¬d¾\\¤FÂd»O¤Wªºµn°OÃ¯»¡¡G©_©Ç¡A¨S¦³³o¶µ¸Ë³Æªº¬ö¿ý³á¡I\n\r", ch );
}


void do_destroy( CHAR_DATA *ch, char* argument )
{
	char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;


	one_argument(argument, arg);
    
	if ( arg[0] == '\0' )
	{
		send_to_char( "½Ð¿é¤J destroy <ª««~>\n\r", ch );
		return;
	}

	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "§A¨S¦³¨º¼ËªF¦è.\n\r", ch );
		return;
	}

	if( !IS_SET(obj->extra_flags, ITEM_PERSONAL) ) {
		send_to_char("§AµLªk¾P·´³o¼ËªF¦è.\n\r", ch);
		return;
	}
	act( "§A·´¥h $p.", ch, obj, NULL, TO_CHAR );
	act( "$n ·´¥h $p.", ch, obj, NULL, TO_ROOM );
	// ·s¼W obj_act destroy 2022/02/12
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("destroy", ch, obj);
	}
	obj_from_char(obj);

	// obj ¥i¯à¦b prog ¤¤³Q purge 2022/02/13
	//if(obj)
	if(obj->deleted)
		;
	else
		extract_obj(obj);
}

/*
void do_consent( CHAR_DATA *ch, char* argument)
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char qm[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *old_in = NULL;

    one_argument( argument, arg);

    if( IS_NPC(ch) ) return;
    if( arg[0] == '\0' ) {
        send_to_char("§A­n±ÂÅvµ¹½Ö¡H\n\r", ch);
        return;
    }
    if( !(victim = get_char_world( ch, arg) )
    ||  IS_NPC(victim) ){
        send_to_char("³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch);
        return;
    }

    if( victim == ch )
  {
    send_to_char("§A¥»¨Ó´N¥i¥H¦Û¤v¾ß¦Û¤vªº«ÍÅé¤F¡I\n\r",ch);
    return;
  }

    one_argument(ch->name, arg1);
    sprintf(qm, "consent_%s", arg1);
    if( is_qmark(victim, qm) ) {
        if( ch->in_room != victim->in_room ) {
            old_in = ch->in_room;
            char_from_room(ch);
            char_to_room(ch, victim->in_room);
        }
        sprintf(buf,"'%s' %s ", arg, qm);
        do_del_qmark(ch, buf);
        if( old_in ) {
            char_from_room(ch);
            char_to_room(ch, old_in);
        }
        act("§A¦¬¦^¹ï$Nªº±ÂÅv¡C", ch, NULL, victim, TO_CHAR);
        act("$n¤£¦A±ÂÅv§A¾ß¨ú$s«ÍÅé¡C", ch, NULL, victim, TO_VICT);
        return;
    }

    sprintf(buf,"'%s' %s -1 0 0 0 0 0", arg, qm);
    if( ch->in_room != victim->in_room ) {
        old_in = ch->in_room;
        char_from_room(ch);
        char_to_room(ch, victim->in_room);
    }
    do_add_qmark(ch, buf);
    if( old_in ) {
        char_from_room(ch);
        char_to_room(ch, old_in);
    }
    act("§A±ÂÅv$N¾ß¨ú§Aªº«ÍÅé¡C", ch, NULL, victim, TO_CHAR);
    act("$n±ÂÅv§A¾ß¨ú$s«ÍÅé¡C", ch, NULL, victim, TO_VICT);
}
*/
void do_bandage( CHAR_DATA *ch, char* argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *pobj;
    CHAR_DATA *vch;
    int factor = 0;
    int sklv = get_skill_level(ch, gsn_bandage);


    if ( IS_NPC( ch ) || (  !IS_NPC( ch )
            && get_skill_percent( ch, gsn_bandage) == 0 ))
    {                
        send_to_char( "¤°»ò¡H\n\r", ch );
        return;
    }

    if ( ch->fighting ){
        send_to_char( "§A¥¿¦b¾Ô°«¡A½w¤£¥X¤â¨Ó.\n\r", ch ); 
        return; 
    }
    one_argument(argument, arg);

    if ( arg[0] == '\0') 
        vch = ch;
    else if( !(vch = get_char_room(ch, arg))) {
        send_to_char("³o¸Ì¨S¦³³o­Ó¤H¡C\n\r", ch);
        return;
    }
    if( vch->fighting ) {
        send_to_char("¹ï¤è¥¿¦b¾Ô°«¡A§A¨S¿ìªk¥J²Óªº¥]²Ï¡C\n\r", ch);
        return;
    }

    for ( pobj = ch->carrying; pobj; pobj = pobj->next_content )
    {
        if ( pobj->pIndexData->vnum == OBJ_VNUM_BANDAGE)
            break;
    }
    if ( !pobj )
    {
        send_to_char( "§A¨­¤W¨S¦³Á^±a.\n\r", ch );
        return;
    }

    extract_obj(pobj);

    if( ch != vch ) {
        act( "§A²Ó¤ßªº¬°$N¥]²Ï¶Ë¶Õ.", ch, NULL, vch, TO_CHAR );
        act( "$n²Ó¤ßªº¬°$N¥]²Ï¶Ë¶Õ.", ch, NULL, vch, TO_NOTVICT );
        act( "$n²Ó¤ßªº¬°§A¥]²Ï¶Ë¶Õ.", ch, NULL, vch, TO_VICT);
        WAIT_STATE(vch, skill_table[gsn_bandage].beats);
        WAIT_STATE(ch, skill_table[gsn_bandage].beats);
    }

    factor = UMAX( ch->level /6 + ch->level / 5, 5);
    factor += sklv;
    if( ch->pcdata->prime_attr == APPLY_WIS) {
        int extra;
        extra = number_bits(2);
        factor += sklv*2;
        switch(extra) {
        case 0:
            factor += 5+sklv;
            break;
        case 1:
            factor += sklv*2;
            break;
        case 2:
            if( is_affected(vch, gsn_poison)){
                act( "¤@°}²M·sªº·PÄ±¥Rº¡§A¥þ¨­.", ch, NULL, vch, TO_VICT);
            }
            break;
        case 3:
            vch->move = UMIN(vch->move + sklv*4+(sklv/3)*10, vch->max_move);
            break;
        };
    }
    int nHeal = (vch->max_hit*factor)/(114-sklv*4);
    nHeal *= PPL_HP_MULTIPLYER;
    vch->hit = UMIN( vch->hit + nHeal, vch->max_hit );
    ch->move = UMAX(0, ch->move - (10 + factor/10));
    send_to_char("¦nªº¡C\n\r", ch);

    WAIT_STATE(ch, skill_table[gsn_bandage].beats);
}
