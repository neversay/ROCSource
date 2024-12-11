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
//*  PROGRAM ID  : save.c                                         *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940617 Razgriz                                *
//*  PURPOSE     :         ­×¥¿ ·íª±®aÀÉ®×¿ù»~®É·|µo¥Í¤GÂàªº BUG  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940725 Jye                                    *
//*  PURPOSE     :         ReSet Sor Skill ¨Ã ®³±¼ÂÂ§Þ¯à          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         ¥[¤J extra_value ªº³B²z¨ì¤U­±¨ç¼Æ¤¤    *
//*              :         fread_obj          fwrite_obj          *
//*              :         fread_bank_obj     fwrite_bank_obj     *
//*              :         fread_newbank_obj  fwrite_newbank_obj  *
//*****************************************************************
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
#include <string.h>
#include <time.h>
#include "merc.h"

#if defined( sun )
#include <memory.h>
#endif

#if !defined( macintosh )
extern  int _filbuf   args( (FILE *) );
#endif

#if defined( sun )
int system    args( ( const char *string ) );
#endif


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST  100
static  OBJ_DATA *  rgObjNest [ MAX_NEST ];


QMARK * qmark_list;
int stat;

/*
 * Local functions.
 */
void  fwrite_char args( ( CHAR_DATA *ch,  FILE *fp ) );
void  fwrite_obj  args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
             FILE *fp, int iNest ) );
void  fwrite_bank_obj args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
             FILE *fp, int iNest ) );
void  fwrite_newbank_obj  args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
             FILE *fp, int iNest ) );
int fread_char  args( ( CHAR_DATA *ch,  FILE *fp ) );
int fread_obj args( ( CHAR_DATA *ch,  FILE *fp ) );
int fread_bank_obj  args( ( CHAR_DATA *ch,  FILE *fp ) );
int fread_newbank_obj args( ( BANK_DATA *ch,  FILE *fp ) );

void HpMnMvDeMultiply_Char(CHAR_DATA *ch)
{
    ch->hit     /= PPL_HP_MULTIPLYER;
    ch->max_hit /= PPL_HP_MULTIPLYER;
    ch->mana    /= PPL_HP_MULTIPLYER;
    ch->max_mana/= PPL_HP_MULTIPLYER;
    ch->move    /= PPL_HP_MULTIPLYER;
    ch->max_move/= PPL_HP_MULTIPLYER;
}

void HpMnMvMultiply_Char(CHAR_DATA *ch)
{
    ch->hit     *= PPL_HP_MULTIPLYER;
    ch->max_hit *= PPL_HP_MULTIPLYER;
    ch->mana    *= PPL_HP_MULTIPLYER;
    ch->max_mana*= PPL_HP_MULTIPLYER;
    ch->move    *= PPL_HP_MULTIPLYER;
    ch->max_move*= PPL_HP_MULTIPLYER;
}

#define MAXIMUM_QMARK_SKEXP 20000

void AbnormalSkexpCheck(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;
  if (ch->level >= 43) return;
  int additional_max_skexp = 0;
  int sn;
  int skill_abnormal = 0;

  QMARK *qm = is_qmark(ch, "max_skill_exp");

  if (qm) {
    additional_max_skexp = qm->v0;
  }
  

  if (additional_max_skexp > MAXIMUM_QMARK_SKEXP) {
    // then this char is abnormal
    send_to_char("Your char has been reset due to your "
                 "abnormal skexp limit\n\r", ch);

    // reset resp
    ch->pcdata->respect = 0;

    // throw away all qmarks
    QMARK *qm = NULL;
    for (qm = ch->qmark; qm; qm = qm->next) {
      del_qmark(qm);
    }

    ch->qmark = NULL;

    // reset all skills
    for (sn = 0; sn < MAX_SKILL; ++sn) {
      ch->pcdata->learned[sn] %= 1000;
    }
    // reset skexp usage
    ch->pcdata->skexp_used = 0;
    ch->pcdata->skexp_earned = get_max_skill_exp(ch);
  }
 
  for (sn = 0; sn < MAX_SKILL; ++sn) {
    if ((ch->pcdata->learned[sn] % 1000) > skill_eff_percent(ch, sn)) {
      char buf[2000];
      sprintf(buf, "%16s(%19s) abnormal, learned %d%% but should be %d%%\n\r",
          skill_table[sn].cname, skill_table[sn].name,
          ch->pcdata->learned[sn] % 1000, skill_eff_percent(ch, sn));
      send_to_char(buf, ch);
      ch->pcdata->learned[sn] = skill_eff_percent(ch, sn);
      skill_abnormal = 1;
    }
  }

  if (skill_abnormal) {
    send_to_char("Your skill exp usage has been reset due to "
                 "abnormal skill percentage\n\r", ch);
    // reset all skills
    for (sn = 0; sn < MAX_SKILL; ++sn) {
      ch->pcdata->learned[sn] %= 1000;
    }
    // reset skexp usage
    ch->pcdata->skexp_used = 0;
    ch->in_room = get_room_index(11000);
  }

  int max_skexp = get_max_skill_exp(ch);
  if (ch->pcdata->skexp_used > max_skexp) {
    ch->pcdata->skexp_used = 0;
    if (ch->pcdata->skexp_earned > max_skexp * 2) {
      ch->pcdata->skexp_earned = max_skexp * 2;
    }
    send_to_char("Your skill level has been reset due to "
                 "your abnormal skexp usage\n\r", ch);

    // reset all skills
    int sn = 0;
    for (sn = 0; sn < MAX_SKILL; ++sn) {
      ch->pcdata->learned[sn] %= 1000;
    }
  }
}

/* Courtesy of Yaz of 4th Realm */
char *initial( const char *str )
{
    static char strint [ MAX_STRING_LENGTH ];

    strint[0] = LOWER( str[ 0 ] );
    return strint;

}

/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj( CHAR_DATA *ch )
{
	FILE *fp;
	CHAR_DATA *phorse;
	char  buf   [ MAX_STRING_LENGTH ];
	char  strsave [ MAX_INPUT_LENGTH  ];

	if ( IS_NPC( ch ) || ( ch->level < 2 && ch->class <= BARBARIAN ) )
		return;

	if ( ch->desc && ch->desc->original )
		ch = ch->desc->original;

	if ( IS_SET( ch->act, PLR_HORSE ) )
	{
		save_horse_obj( ch );
		return;
	}
	else if ( ( phorse = ch->pcdata->horse ) )
	{
		if ( phorse->in_room != ch->in_room )
		{
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

	ch->save_time = current_time;
	fflush( NULL );
	if (fpReserve) {
		fflush( fpReserve );
		fclose( fpReserve );
		fpReserve = NULL;
	}

	/* player files parsed directories by Yaz 4th Realm */
#if !defined( macintosh ) && !defined( WIN32 )
	sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial( ch->name ), "/", capitalize( ch->name ) );
#else
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
#endif
	if ( !( fp = fopen( strsave, "w" ) ) )
	{
		sprintf( buf, "Save_char_obj: fopen %s: ", ch->name );
		bug( buf, 0 );
		perror( strsave );
	}
	else
	{
		fwrite_char( ch, fp );
		if ( ch->carrying )
			fwrite_obj( ch, ch->carrying, fp, 0 );
		if ( ch->eqbank )
			fwrite_bank_obj( ch, ch->eqbank, fp, 0 );
		fprintf( fp, "#END\n" );
	}
	fflush( NULL );
	if( fp ) {
		fflush( fp );
		fclose( fp );
	}
	fpReserve = fopen( NULL_FILE, "r" );
	return;
}



/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
	AFFECT_DATA *paf;
	QMARK *qmark;
	int sn;
	int i;

	fprintf( fp, "#%s\n", IS_NPC( ch ) ? "MOB" : "PLAYER" );

	if( !IS_NPC(ch))
		fprintf(fp, "Ver  %d\n", LATEST_VERSION);
	fprintf( fp, "Nm          %s~\n", ch->name    );
	fprintf( fp, "Cname %s~\n", ch->cname   );
	fprintf( fp, "ShtDsc      %s~\n", ch->short_descr   );
	fprintf( fp, "LngDsc      %s~\n", ch->long_descr    );
	fprintf( fp, "Dscr        %s~\n", ch->description   );
	fprintf( fp, "Prmpt       %s~\n", ch->pcdata->prompt  );
	fprintf( fp, "CTitle   %s~\n",  ch->pcdata->clantitle );
	fprintf( fp, "Sx          %d\n",  ch->sex     );
	fprintf( fp, "Cla         %d\n",  ch->class   );

	fprintf( fp, "Race        %s~\n", race_table[ ch->race ].name );

	fprintf( fp, "Lvl         %d\n",  ch->level   );
	fprintf( fp, "PKWin       %d\n",  ch->pcdata->pkwin);
	fprintf( fp, "PKLose  %d\n",  ch->pcdata->pklose);
	fprintf( fp, "Trst        %d\n",  ch->trust   );
	fprintf( fp, "Security    %d\n",  ch->pcdata->security  );  /* OLC */
	fprintf( fp, "Playd       %d\n",
	ch->played + (int) ( current_time - ch->logon )   );
	fprintf( fp, "Note        %ld\n", (unsigned long)ch->last_note );
	if ( (ch->in_room && IS_SET( ch->in_room->room_flags, ROOM_SAVE ))
		|| (ch->in_room && !str_cmp( ch->in_room->area->identity, "skarabrae" ))
		|| IS_IMMORTAL( ch) )
	{
		fprintf( fp, "Room        %d\n", ch->in_room->vnum );
		HpMnMvDeMultiply_Char(ch);
		fprintf( fp, "HpMnMv      %d %d %d %d %d %d\n",
		ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move );
		fprintf( fp, "Gold        %d\n",  ch->gold);
		HpMnMvMultiply_Char(ch);
	}
	else
	{
		//fprintf( fp, "Room        %d\n", ROOM_VNUM_RUBY_INN );
		if( ch->in_room )
			fprintf( fp, "Room        %d\n", ch->in_room->area->recall );
		else
			fprintf( fp, "Room        %d\n", ROOM_VNUM_TEMPLE );
		HpMnMvDeMultiply_Char(ch);
		fprintf( fp, "HpMnMv      %d %d %d %d %d %d\n",
		UMAX(1,ch->hit/2),
		ch->max_hit, UMAX(1,ch->mana/2),
		ch->max_mana,UMAX(1, ch->move/2), ch->max_move );
		fprintf( fp, "Gold        %d\n",  ch->gold);
		HpMnMvMultiply_Char(ch);
	}

	fprintf( fp, "Bank        %d\n",  ch->bank    );
	fprintf( fp, "Exp         %d\n",  ch->exp     );
	fprintf( fp, "Act         %lld\n",  ch->act     );
	//REMOVE_BIT(ch->affected_by, AFF_DELAYED_CAST);
	fprintf( fp, "AffdBy      %lld\n",  ch->affected_by   );
	//    fprintf( fp, "AffdBy2     %d\n",  ch->affected_by2  );
	/* Bug fix from Alander */
	fprintf( fp, "Pos         %d\n",
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position );

	fprintf( fp, "Prac        %d\n",  ch->practice    );
	fprintf( fp, "SavThr    %d\n" ,ch->saving_throw[0]);
	fprintf( fp, "ExSavThr ");
	for(i=1; i< MAX_SAVING ; ++i){
		fprintf(fp, " %d", ch->saving_throw[i]);
	}
	fprintf(fp, "\n");
	fprintf( fp, "Align       %d\n",  ch->alignment   );
	fprintf( fp, "Hit         %d\n",  ch->hitroll   );
	fprintf( fp, "Dam         %d\n",  ch->damroll   );
	fprintf( fp, "Armr        %d\n",  ch->armor   );
	fprintf( fp, "PPotent     %d\n",  ch->pp      );
	fprintf( fp, "Mdr       %d\n",  ch->mdr     );
	fprintf( fp, "Pdr       %d\n",  ch->pdr     );
	fprintf( fp, "AMdr        %d\n",  ch->amdr      );
	fprintf( fp, "APdr        %d\n",  ch->apdr      );
	fprintf( fp, "Wimp        %d\n",  ch->wimpy   );
	fprintf( fp, "Deaf        %d\n",  ch->deaf    );

	if ( IS_NPC( ch ) )
	{
		fprintf( fp, "Vnum        %d\n",  ch->pIndexData->vnum  );
	}
	else
	{
		fprintf( fp, "Paswd       %s~\n", ch->pcdata->pwd   );
		fprintf( fp, "Bmfin       %s~\n", ch->pcdata->bamfin  );
		fprintf( fp, "Bmfout      %s~\n", ch->pcdata->bamfout );
		fprintf( fp, "Immskll     %s~\n", ch->pcdata->immskll );
		fprintf( fp, "Ttle        %s~\n", ch->pcdata->title );
		fprintf( fp, "Rank      %d\n",  ch->pcdata->rank);
		fprintf( fp, "AtrPrm      %d %d %d %d %d\n",
			ch->perm_str,
			ch->perm_int,
			ch->perm_wis,
			ch->perm_dex,
			ch->perm_con);

		fprintf( fp, "AtrMd       %d %d %d %d %d \n",
			ch->mod_str, 
			ch->mod_int, 
			ch->mod_wis,
			ch->mod_dex, 
			ch->mod_con );

		fprintf( fp, "Cond        %d %d %d\n",
			ch->pcdata->condition[0],
			ch->pcdata->condition[1],
			ch->pcdata->condition[2] );

		fprintf( fp, "Pglen       %d\n",	ch->pcdata->pagelen );
		fprintf( fp, "Respect     %d\n",	ch->pcdata->respect );
		fprintf( fp, "Merit     %d\n",		ch->pcdata->merit );
		fprintf( fp, "MeritSpent     %d\n",	ch->pcdata->merit_spent );
		fprintf( fp, "CLevel     %d\n",		ch->pcdata->clanlevel );
		fprintf( fp, "CStat     %d\n",		ch->pcdata->clanstatistics );
		fprintf( fp, "CExp     %d\n",		ch->pcdata->clanexp );
		fprintf( fp, "Ride     %d\n",		ch->pcdata->ride == 0 ? 0 : 1 );
		fprintf( fp, "AutoC %d\n",			ch->pcdata->autocombat );
		fprintf( fp, "SkExp     %d  %d \n",	ch->pcdata->skexp_used, ch->pcdata->skexp_earned );
		fprintf( fp, "Dodge %d\n",			ch->dodge );
		fprintf( fp, "ChSize %d\n",			ch->mod_size ); // Åé«¬ 2022/12/23
		fprintf( fp, "Denied  %ld\n",		(long)ch->pcdata->denied );
		fprintf( fp, "Freezd  %ld\n",		(long)ch->pcdata->freezed );

		if ( !ch->pcdata->snd_attr )
			ch->pcdata->snd_attr = 0;

		fprintf( fp, "PS_ATTR %d %d\n", ch->pcdata->prime_attr, ch->pcdata->snd_attr );
		for ( sn = 0; sn < MAX_SKILL; sn++ )
		{
			if ( skill_table[sn].name && ch->pcdata->learned[sn] > 0 )
			{
				fprintf( fp, "Skll        %d '%s'\n",
				ch->pcdata->learned[sn], skill_table[sn].name );
			}
		}
	}

	//   if ( !ch->pcdata->prime_attr )
	//  ch->pcdata->prime_attr = ch->class + 1;

	for ( paf = ch->affected; paf; paf = paf->next )
	{
		if ( paf->deleted )
			continue;

		fprintf( fp, "Afft        %18s~ %3d %3d %3d %14lld\n",
			skill_table[ paf->type ].name,
			paf->duration,
			paf->modifier,
			paf->location,
			paf->bitvector );
	}

	if ( ch->qmark )
	{
		for( qmark = ch->qmark; qmark; qmark = qmark->next )
		{
			if ( qmark->fSave )
				fprintf( fp, "QMARK %s~ %d %d %d %d %d\n",
			qmark->mark_name,
			qmark->timer,
			qmark->v0,
			qmark->v1,
			qmark->v2,
			qmark->fDeath );
		} 
	}

	if ( ch->clan )
		fprintf( fp, "CLAN %s~\n", ch->clan->name );

	fprintf( fp, "End\n\n" );
	fflush( fp );
	return;
}



/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    AFFECT_DATA      *paf;
    EXTRA_DESCR_DATA *ed;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content )
    fwrite_obj( ch, obj->next_content, fp, iNest );

    /*
     * Castrate storage characters.
     */
    if( obj->deleted ) return;
    if ( !(IS_SET(obj->extra_flags, ITEM_PERSONAL && !IS_SET( obj->extra_flags, ITEM_NOSAVE )))  // personal item is always saveable
    && (( ch->level < ( obj->level - 5 ) && ch->class < 5 ) || obj->item_type == ITEM_KEY))
  return;
/*
    if ( IS_SET( obj->extra_flags, ITEM_NOSAVE )
  || IS_SET( obj->extra_flags, ITEM_UNIQUE )){
  unequip_char( ch, obj );
  return;
    }
*/

    fprintf( fp, "#OBJECT\n" );
    //REMOVE_BIT(obj->extra_flags, ITEM_RECOVER);
    fprintf( fp, "Nest         %d\n", iNest          );
    fprintf( fp, "Name         %s~\n",  obj->name        );
    fprintf( fp, "ShortDescr   %s~\n",  obj->short_descr       );
/*    fprintf( fp, "Cname  %s~\n",  obj->cname         );*/
    fprintf( fp, "Description  %s~\n",  obj->description       );
    fprintf( fp, "Vnum         %d\n", obj->pIndexData->vnum      );
    fprintf( fp, "ExtraFlags   %d\n", obj->extra_flags       );
    fprintf( fp, "WearFlags    %d\n", obj->wear_flags        );
    fprintf( fp, "WearLoc      %d\n", obj->wear_loc        );
    fprintf( fp, "ItemType     %d\n", obj->item_type         );
    fprintf( fp, "Weight       %d\n", obj->weight        );
    fprintf( fp, "Level        %d\n", obj->level         );
    fprintf( fp, "Timer        %d\n", obj->timer         );
    fprintf( fp, "Cost         %d\n", obj->cost        );
    fprintf( fp, "Class        %d\n", obj->class         );
    fprintf( fp, "Race         %d\n", obj->race        );
    fprintf( fp, "Serial    %d\n", (int)obj->serial);
    fprintf( fp, "Values       %d %d %d %d\n",
  obj->value[0], obj->value[1], obj->value[2], obj->value[3]   );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
  if ( obj->value[1] > 0 )
  {
      fprintf( fp, "Spell 1      '%s'\n", 
    skill_table[obj->value[1]].name );
  }

  if ( obj->value[2] > 0 )
  {
      fprintf( fp, "Spell 2      '%s'\n", 
    skill_table[obj->value[2]].name );
  }

  if ( obj->value[3] > 0 )
  {
      fprintf( fp, "Spell 3      '%s'\n", 
    skill_table[obj->value[3]].name );
  }

  break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
  if ( obj->value[3] > 0 )
  {
      fprintf( fp, "Spell 3      '%s'\n", 
    skill_table[obj->value[3]].name );
  }

  break;
    }

    for ( ed = obj->extra_descr; ed; ed = ed->next )
    {
  fprintf( fp, "ExtraDescr   %s~ %s~\n",
    ed->keyword, ed->description );
    }

    for ( paf = obj->affected; paf; paf = paf->next )
    {
  fprintf( fp, "Affect       %d %d %d %d %lld\n",
    paf->type,
    paf->duration,
    paf->modifier,
    paf->location,
    paf->bitvector );
    }
    if ( obj->hp ) {
  OBJ_HP * ohp = obj->hp;
  for( ; ohp; ohp = ohp->next ) {
  fprintf( fp, "Hidden %d %s~ %d %d %d\n", ohp->vnum,
    ohp->hp_name, ohp->percent, 
    ohp->level, ohp->event );
  }
    }

    /* Added by Razgriz 20050918
     * save obj_extra_value if used is TRUE
     */
    if ( ( obj->extra_value ).used )
    {
        fprintf ( fp, "ExValue      %d %d %d %d\n",
                ( obj->extra_value ).v[0],
                ( obj->extra_value ).v[1],
                ( obj->extra_value ).t[0],
                ( obj->extra_value ).t[1] );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->contains )
  fwrite_obj( ch, obj->contains, fp, iNest + 1 );

  fflush( fp );
    tail_chain();
    return;
}


void fwrite_newbank_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    AFFECT_DATA      *paf;
    EXTRA_DESCR_DATA *ed;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content )
  fwrite_newbank_obj( ch, obj->next_content, fp, iNest );

    /*
     * Castrate storage characters.
     */

    fprintf( fp, "#EQBANK_OBJECT\n" );
    //REMOVE_BIT(obj->extra_flags, ITEM_RECOVER);
    fprintf( fp, "Nest         %d\n", iNest          );
    fprintf( fp, "Name         %s~\n",  obj->name        );
    fprintf( fp, "ShortDescr   %s~\n",  obj->short_descr       );
/*    fprintf( fp, "Cname  %s~\n",  obj->cname         );*/
    fprintf( fp, "Description  %s~\n",  obj->description       );
    fprintf( fp, "Vnum         %d\n", obj->pIndexData->vnum      );
    fprintf( fp, "ExtraFlags   %d\n", obj->extra_flags       );
    fprintf( fp, "WearFlags    %d\n", obj->wear_flags        );
    fprintf( fp, "WearLoc      %d\n", obj->wear_loc        );
    fprintf( fp, "ItemType     %d\n", obj->item_type         );
    fprintf( fp, "Weight       %d\n", obj->weight        );
    fprintf( fp, "Level        %d\n", obj->level         );
    fprintf( fp, "Timer        %d\n", obj->timer         );
    fprintf( fp, "Cost         %d\n", obj->cost        );
    fprintf( fp, "Class        %d\n", obj->class         );
    fprintf( fp, "Race         %d\n", obj->race        );
    fprintf( fp, "Serial    %d\n", (int)obj->serial);
    fprintf( fp, "Values       %d %d %d %d\n",
  obj->value[0], obj->value[1], obj->value[2], obj->value[3]   );
    fprintf( fp, "Recall    %d\n", obj->value[4]);

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
  if ( obj->value[1] > 0 )
  {
      fprintf( fp, "Spell 1      '%s'\n", 
    skill_table[obj->value[1]].name );
  }

  if ( obj->value[2] > 0 )
  {
      fprintf( fp, "Spell 2      '%s'\n", 
    skill_table[obj->value[2]].name );
  }

  if ( obj->value[3] > 0 )
  {
      fprintf( fp, "Spell 3      '%s'\n", 
    skill_table[obj->value[3]].name );
  }

  break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
  if ( obj->value[3] > 0 )
  {
      fprintf( fp, "Spell 3      '%s'\n", 
    skill_table[obj->value[3]].name );
  }

  break;
    }

    for ( ed = obj->extra_descr; ed; ed = ed->next )
    {
  fprintf( fp, "ExtraDescr   %s~ %s~\n",
    ed->keyword, ed->description );
    }

    for ( paf = obj->affected; paf; paf = paf->next )
    {
  fprintf( fp, "Affect       %d %d %d %d %lld\n",
    paf->type,
    paf->duration,
    paf->modifier,
    paf->location,
    paf->bitvector );
    }
    if ( obj->hp ){
  OBJ_HP * ohp;
  for( ohp = obj->hp; ohp; ohp = ohp->next ) {
  fprintf( fp, "Hidden %d %s~ %d %d %d\n", ohp->vnum,
    ohp->hp_name, ohp->percent, 
    ohp->level, ohp->event );
  }
    }

    /* Added by Razgriz 20050918
     * save obj_extra_value if used is TRUE
     */
    if ( ( obj->extra_value ).used )
    {
        fprintf ( fp, "ExValue      %d %d %d %d\n",
                ( obj->extra_value ).v[0],
                ( obj->extra_value ).v[1],
                ( obj->extra_value ).t[0],
                ( obj->extra_value ).t[1] );
    }

  fprintf( fp, "End\n\n" );

    if ( obj->contains )
  fwrite_newbank_obj( ch, obj->contains, fp, iNest + 1 );

  fflush( fp );
    tail_chain();
    return;
}

/*
 * Write an eqbank object.
 */
void fwrite_bank_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    AFFECT_DATA      *paf;
    EXTRA_DESCR_DATA *ed;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content )
  fwrite_bank_obj( ch, obj->next_content, fp, iNest );

    /*
     * Castrate storage characters.
     */

    fprintf( fp, "#EQBANK_OBJECT\n" );
    //REMOVE_BIT(obj->extra_flags, ITEM_RECOVER);
    fprintf( fp, "Nest         %d\n", iNest          );
    fprintf( fp, "Name         %s~\n",  obj->name        );
    fprintf( fp, "ShortDescr   %s~\n",  obj->short_descr       );
/*    fprintf( fp, "Cname  %s~\n",  obj->cname         );*/
    fprintf( fp, "Description  %s~\n",  obj->description       );
    fprintf( fp, "Vnum         %d\n", obj->pIndexData->vnum      );
    fprintf( fp, "ExtraFlags   %d\n", obj->extra_flags       );
    fprintf( fp, "WearFlags    %d\n", obj->wear_flags        );
    fprintf( fp, "WearLoc      %d\n", obj->wear_loc        );
    fprintf( fp, "ItemType     %d\n", obj->item_type         );
    fprintf( fp, "Weight       %d\n", obj->weight        );
    fprintf( fp, "Level        %d\n", obj->level         );
    fprintf( fp, "Timer        %d\n", obj->timer         );
    fprintf( fp, "Cost         %d\n", obj->cost        );
    fprintf( fp, "Class        %d\n", obj->class         );
    fprintf( fp, "Race         %d\n", obj->race        );
    fprintf( fp, "Serial    %d\n", (int)obj->serial);
    fprintf( fp, "Values       %d %d %d %d\n",
  obj->value[0], obj->value[1], obj->value[2], obj->value[3]   );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
  if ( obj->value[1] > 0 )
  {
      fprintf( fp, "Spell 1      '%s'\n", 
    skill_table[obj->value[1]].name );
  }

  if ( obj->value[2] > 0 )
  {
      fprintf( fp, "Spell 2      '%s'\n", 
    skill_table[obj->value[2]].name );
  }

  if ( obj->value[3] > 0 )
  {
      fprintf( fp, "Spell 3      '%s'\n", 
    skill_table[obj->value[3]].name );
  }

  break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
  if ( obj->value[3] > 0 )
  {
      fprintf( fp, "Spell 3      '%s'\n", 
    skill_table[obj->value[3]].name );
  }

  break;
    }

    for ( ed = obj->extra_descr; ed; ed = ed->next )
    {
  fprintf( fp, "ExtraDescr   %s~ %s~\n",
    ed->keyword, ed->description );
    }

    for ( paf = obj->affected; paf; paf = paf->next )
    {
  fprintf( fp, "Affect       %d %d %d %d %lld\n",
    paf->type,
    paf->duration,
    paf->modifier,
    paf->location,
    paf->bitvector );
    }
    if ( obj->hp ){
  OBJ_HP * ohp;
  for( ohp = obj->hp; ohp; ohp = ohp->next ) {
  fprintf( fp, "Hidden %d %s~ %d %d %d\n", ohp->vnum,
    ohp->hp_name, ohp->percent, 
    ohp->level, ohp->event );
  }
    }

    /* Added by Razgriz 20050918
     * save obj_extra_value if used is TRUE
     */
    if ( ( obj->extra_value ).used )
    {
        fprintf ( fp, "ExValue      %d %d %d %d\n",
                ( obj->extra_value ).v[0],
                ( obj->extra_value ).v[1],
                ( obj->extra_value ).t[0],
                ( obj->extra_value ).t[1] );
    }

    fprintf( fp, "End\n\n" );
/*
    if ( obj->contains )
  fwrite_obj( ch, obj->contains, fp, iNest + 1 );
*/
  fflush( fp );
    tail_chain();
    return;
}


/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
  extern char      *daPrompt;
  FILE       *fp;
  CHAR_DATA *ch;
  char        strsave [ MAX_INPUT_LENGTH ];
  bool        found;
  char        sorry_player [] =
    "********************************************************\n\r"
    "** ±zªº¨¤¦â¦sÀÉµo¥Í·lÃa, ½Ð©ó BBS ¤W¦^³ø°ÝÃD          **\n\r"
    "** ©Î¨Ï¥Î¨ä¥L¨¤¦â note to IMMORTAL ¦^³ø¦^³ø°ÝÃD,      **\n\r"
    "** ¨ÃÀR­Ô­×¥¿. ³y¦¨±zªº¤£«K, ²`·P©êºp.                **\n\r"
    "**                                ROC ¥þÅé¤u§@¤H­û    **\n\r"
    "********************************************************\n\r";
  char        sorry_object [] =
    "********************************************************\n\r"
    "** ±z¨¤¦âªº«ù¦³ª««~µo¥Í°ÝÃD, ¥i¯à³y¦¨ª««~·l¥¢.        **\n\r"
    "** ½Ð¨Ï¥Î¨ä¥L¨¤¦â note to IMMORTAL ¦^³ø¦^³ø°ÝÃD.      **\n\r"
    "** ©Î¦V½u¤W IMM ¦^³ø. ÁÂÁÂ.                           **\n\r"
    "********************************************************\n\r";


  ch          = new_character( TRUE );

  d->character      = ch;
  ch->desc        = d;
  ch->name        = str_dup( name );
  sprintf(pipe_buf, "|N %d %s \n", (int)d, ch->name);
  write(fd0[1], pipe_buf, strlen(pipe_buf));
  ch->pcdata->prompt      = str_dup( daPrompt );
  ch->last_note     = 0;
  ch->act       = PLR_BLANK
    | PLR_COMBINE
    | PLR_PROMPT
    | PLR_AUTOEXIT
    //| PLR_REPLACE
    | PLR_COMBAT
    | PLR_CHINESE
    | PLR_STATUS
    | PLR_ARTIFACT
    | PLR_ASSIST
    | PLR_NOTIFY
    | PLR_AUTOGOLD;
  ch->pcdata->pwd     = str_dup( "" );
  ch->pcdata->bamfin      = str_dup( "" );
  ch->pcdata->bamfout     = str_dup( "" );
  ch->pcdata->immskll     = str_dup( "" );
  ch->pcdata->title     = str_dup( "" );
  ch->perm_str    = 13;
  ch->perm_int    = 13; 
  ch->perm_wis    = 13;
  ch->perm_dex    = 13;
  ch->perm_con    = 13;
  // 2023/01/13
  //ch->pcdata->condition[COND_THIRST]  = 48;
  //ch->pcdata->condition[COND_FULL]  = 48;
  ch->pcdata->condition[COND_THIRST]  = 100;
  ch->pcdata->condition[COND_FULL]  = 100;
  ch->pcdata->pagelen     = 40;
  ch->pcdata->security    = 0;  /* OLC */

  ch->pcdata->switched    = FALSE;
  ch->pcdata->horse     = NULL;
  ch->pcdata->ride      = 0;
  ch->pcdata->version     = 0;
  ch->dodge       = 0;
  // ch->pcdata->skexp_earned_max = 0;//add by jacquex
  ch->pcdata->ba        = NULL;

  found = FALSE;
  if (fpReserve) {
    fflush( fpReserve );
    fclose( fpReserve );
    fpReserve = NULL;
  }
  fflush( NULL );

  /* parsed player file directories by Yaz of 4th Realm */
  /* decompress if .gz file exists - Thx Alander */
#if !defined( macintosh ) && !defined( WIN32 )
  sprintf( strsave, "%s%s%s%s%s", PLAYER_DIR, initial( ch->name ),
      "/", capitalize( name ), ".gz" );
  if ( ( fp = fopen( strsave, "r" ) ) )
  {
    char     buf     [ MAX_STRING_LENGTH ];
    
    fflush( fp );
    fclose( fp );
    sprintf( buf, "gzip -dfq %s", strsave );
    system( buf );
  }
#endif

#if !defined( macintosh ) && !defined( WIN32 )
  sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial( ch->name ),
      "/", capitalize( name ) );
#else
  sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
#endif
  if ( ( fp = fopen( strsave, "r" ) ) )
  {
    char buf[ MAX_STRING_LENGTH ];
    int iNest;

    for ( iNest = 0; iNest < MAX_NEST; iNest++ )
      rgObjNest[iNest] = NULL;

    found = TRUE;
    for ( ; ; )
    {
      char *word;
      char  letter;
      int   status;

      letter = fread_letter( fp );
      if ( letter == '*' )
      {
        fread_to_eol( fp );
        continue;
      }

      if ( letter != '#' )
      {
        bug( "Load_char_obj: # not found.", 0 );
        break;
      }

      word = fread_word( fp, &status );

      if( !word ) {
        bug("load_char_obj, error2",0);
        fread_to_eol(fp);
        break;
      }
      if ( !str_cmp( word, "PLAYER" ) )
      {
        if ( fread_char ( ch, fp ) )
        {
          sprintf( buf,
              "Load_char_obj:  %s section PLAYER corrupt.\n\r",
              name );
          bug( buf, 0 );
          write_to_buffer( d, sorry_player, 0 );

          /* 
           * In case you are curious,
           * it is ok to leave ch alone for close_socket
           * to free.
           * We want to now kick the bad character out as
           * what we are missing are MANDATORY fields.  -Kahn
           */
          SET_BIT( ch->act, PLR_DENY );
          return TRUE;
        }
      }
      else if ( !str_cmp( word, "OBJECT" ) )
      {
        if ( !fread_obj  ( ch, fp ) )
        {
          sprintf( buf,
              "Load_char_obj:  %s section OBJECT corrupt.\n\r",
              name );
          bug( buf, 0 );
          write_to_buffer( d, sorry_object, 0 );
          return TRUE;
        }
      }
      else if ( !str_cmp( word, "EQBANK_OBJECT" ) )
      {
        if ( !fread_bank_obj( ch, fp ) )
        {
          sprintf( buf,
              "Load_char_obj:  %s section OBJECT corrupt.\n\r",
              name );
          bug( buf, 0 );
          write_to_buffer( d, sorry_object, 0 );
          return TRUE;
        }
      }
      else if ( !str_cmp( word, "END"    ) ) break;
      else
      {
        bug( "Load_char_obj: bad section.", 0 );
        break;
      }
    } /* for */
  
    fflush( fp );
    fclose( fp );
  }

  fpReserve = fopen( NULL_FILE, "r" );
  return found;
}



/*
 * Read in a char.
 */


int fread_char( CHAR_DATA *ch, FILE *fp )
{
  int   nRoomNum = 3214;
  char  *word;
  char  buf [ MAX_STRING_LENGTH ];
  AFFECT_DATA *paf;
  QMARK *qmark;
  CLAN_DATA *cn;
  int   sn;
  int   i;
  int   j;
  int   error_count = 0;
  int   status;
  int   status1;
  char  *p;
  long long int   tmpi;
  int   num_keys;
  int   last_key = 0;

  char  def_prompt [] = "[m<¥Í©R[1;32m%h[mÅ]ªk[1;33m%m[m²¾°Ê[1;35m%v[m> ";
  char  *def_sdesc  /*[]*/ = ch->name;/*"Your short description was corrupted.";*/
  char  def_ldesc  [] = "Your long description was corrupted.";
  char  def_desc   [] = "Your description was corrupted.";
  char  def_title  [] = "Your title was corrupted.";
  char  def_clantitle  [] = "None";
  char  *def_name   = ch->name;
  char  *def_cname    = ch->name;

	struct key_data key_tab [] = 
	{
		{ "Ver",		FALSE,		LATEST_VERSION,		{ &ch->pcdata->version,	NULL } },
		{ "Nm",			TRUE,		(int) &def_name, { &ch->name,	NULL } },
		{ "Cname",		TRUE,		(int) &def_cname,  { &ch->cname,       NULL } },
		{ "ShtDsc",		TRUE,		(int) def_sdesc, { &ch->short_descr,   NULL } },
		{ "LngDsc",		TRUE,		(int) &def_ldesc,  { &ch->long_descr,    NULL } },
		{ "Dscr",		TRUE,		(int) &def_desc, { &ch->description,   NULL } },
		{ "Prmpt",		TRUE,		(int) &def_prompt, { &ch->pcdata->prompt,NULL } },
		{ "CTitle",		TRUE,		(int) &def_clantitle,  { &ch->pcdata->clantitle,   NULL } },
		{ "Sx",			FALSE,		SEX_MALE,		{ &ch->sex,       NULL } },
		{ "Security",	FALSE,		0,     { &ch->pcdata->security,NULL } },
		/* OLC */           
		{ "Cla",		FALSE,		MAND,      { &ch->class,       NULL } },
		{ "Lvl",		FALSE,		MAND,      { &ch->level,       NULL } },
		{ "PKWin",		FALSE,		0,     { &ch->pcdata->pkwin,       NULL } },
		{ "PKLose",		FALSE,		0,     { &ch->pcdata->pklose,        NULL } },
		{ "Trst",		FALSE,		0,     { &ch->trust,       NULL } },
		{ "Playd",		FALSE,		0,     { &ch->played,        NULL } },
		{ "Note",		FALSE,		0,     { &ch->last_note,     NULL } },
		{ "HpMnMv",		FALSE,		MAND,		{	&ch->hit,     
													&ch->max_hit,           
													&ch->mana,           
													&ch->max_mana,           
													&ch->move,           
													&ch->max_move,	NULL } },
		{ "Gold",   FALSE, 0,     { &ch->gold,        NULL } },
		{ "Bank",   FALSE, 0,     { &ch->bank,        NULL } },
		{ "Exp",    FALSE, MAND,      { &ch->exp,       NULL } },
		{ "Act",    FALSE, 3344558LL,     { &ch->act,         NULL } },
		{ "AffdBy", FALSE, 0LL,     { &ch->affected_by,   NULL } },
		{ "AffdBy2",FALSE, 0,     { &ch->affected_by2,  NULL } },
		{ "Pos",    FALSE, POS_STANDING,    { &ch->position,      NULL } },
		{ "Prac",   FALSE, MAND,      { &ch->practice,      NULL } },
		{ "SavThr", FALSE, 0/*MAND*/,     { &(ch->saving_throw[0]), NULL}},           
			/*{ "ExSavThr", FALSE, 0,     {            
				&(ch->saving_throw[1]),
				&(ch->saving_throw[2]),
				&(ch->saving_throw[3]),
				&(ch->saving_throw[4]),
				&(ch->saving_throw[5]),
				&(ch->saving_throw[6]),
				&(ch->saving_throw[7]),
				&(ch->saving_throw[8]),
				&(ch->saving_throw[9]),
				&(ch->saving_throw[10]),
				&(ch->saving_throw[11]),
				&(ch->saving_throw[12]),
				&(ch->saving_throw[13]),
				&(ch->saving_throw[14]),
				&(ch->saving_throw[15]),
				&(ch->saving_throw[16]),
			NULL } },*/
		{ "Align",  FALSE, 0,     { &ch->alignment,     NULL } },
		{ "Hit",    FALSE, MAND,      { &ch->hitroll,         NULL } },
		{ "Dam",    FALSE, MAND,      { &ch->damroll,         NULL } },
		{ "Armr",   FALSE, MAND,      { &ch->armor,       NULL } },
		{ "PPotent", FALSE, 0,      { &ch->pp,        NULL } },
		{ "Mdr", FALSE, 0,      { &ch->mdr,       NULL } },
		{ "Pdr", FALSE, 0,      { &ch->pdr,       NULL } },
		{ "AMdr", FALSE, 0,     { &ch->amdr,        NULL } },
		{ "APdr", FALSE, 0,     { &ch->apdr,        NULL } },
		{ "Wimp",   FALSE, 10,      { &ch->wimpy,       NULL } },
		{ "Deaf",   FALSE, 0,     { &ch->deaf,        NULL } },
		{ "Paswd",  TRUE,  MAND,      { &ch->pcdata->pwd,   NULL } },
		{ "Bmfin",  TRUE,  DEFLT,     { &ch->pcdata->bamfin,
		NULL } },           
		{ "Bmfout", TRUE,  DEFLT,     { &ch->pcdata->bamfout,
		NULL } },           
		{ "Immskll",TRUE,  DEFLT,     { &ch->pcdata->immskll,
		NULL } },           
		{ "Ttle",   TRUE,  (int) &def_title,  { &ch->pcdata->title, NULL } },
		{ "Rank",   FALSE,   0, { &ch->pcdata->rank, NULL } },
		{ "AtrPrm", FALSE, MAND,      { &ch->perm_str,
										&ch->perm_int,           
										&ch->perm_wis,           
										&ch->perm_dex,           
										&ch->perm_con,           
										NULL } },
		{ "AtrMd",  FALSE, MAND,      { &ch->mod_str, 
										&ch->mod_int,           
										&ch->mod_wis,           
										&ch->mod_dex,           
										&ch->mod_con,           
										NULL } },           
		{ "Cond",   FALSE, DEFLT,     { &ch->pcdata->condition [0],
										&ch->pcdata->condition [1],           
										&ch->pcdata->condition [2],           
										NULL } },           
		{ "Pglen",  FALSE, 20,      { &ch->pcdata->pagelen, NULL } },           
		{ "Respect",FALSE,  0,      { &ch->pcdata->respect, NULL } },           
		{ "Merit",FALSE,  0,      { &ch->pcdata->merit, NULL } },           
		{ "MeritSpent",FALSE,  0,      { &ch->pcdata->merit_spent, NULL } },
		{ "CLevel",FALSE,  0,     { &ch->pcdata->clanlevel, NULL } },
		{ "CStat",FALSE,  0,      { &ch->pcdata->clanstatistics, NULL } },
		{ "CExp",FALSE,  0,     { &ch->pcdata->clanexp, NULL } },
		{ "Ride",FALSE,  0,     { &ch->pcdata->ride, NULL } },
		{ "SkExp",FALSE,  0,      { &ch->pcdata->skexp_used, &ch->pcdata->skexp_earned ,NULL } },//&ch->pcdata->skexp_earned_max, NULL } },
		{ "AutoC",FALSE,  0,      { &ch->pcdata->autocombat, NULL } },
		{ "Dodge",      FALSE,  0,  { &ch->dodge,       NULL } },
		{ "ChSize",FALSE,  0,      { &ch->mod_size, NULL } }, // Åé«¬ 2022/12/23
		{ "Denied",     FALSE,  0,  { &ch->pcdata->denied,        NULL } },
		{ "Freezd",     FALSE,  0,  { &ch->pcdata->freezed,       NULL } },
		{ "\0",   FALSE, 0               }
	};


  for( num_keys = 0; *key_tab [num_keys].key; )
       num_keys++;

  for( ; !feof (fp) ; )
  {
      
       word = fread_word( fp, &status );
      
       if( !word )
       {
           bug( "fread_char:  Error reading key.  EOF?", 0 );
           fread_to_eol( fp );
           break;
       }
      
       /* This little diddy searches for the keyword
          from the last keyword found */
      
       for( i = last_key;
            i < last_key + num_keys &&
            str_cmp (key_tab [i % num_keys].key, word); )
            i++;
      
       i = i % num_keys;
      
       if( !str_cmp (key_tab [i].key, word) )
           last_key = i;
       else
           i = num_keys;
      
       if( *key_tab [i].key )   /* Key entry found in key_tab */
       {
           if( key_tab [i].string == SPECIFIED )
               bug ("Key already specified.", 0);
            /* Entry is a string */
           else
               if( key_tab [i].string )
               {
                   if( ( p = fread_string( fp, &status ) ) && !status )
                   {
                       free_string ( *(char **)key_tab [i].ptrs [0] );
                       *(char **)key_tab [i].ptrs [0] = p;
                   }
               }
            /* Entry is an integer */
           else
               for( j = 0; key_tab [i].ptrs [j]; j++ )
               {
                    tmpi = fread_number ( fp, &status );
                 
                    if( !status )
                    {
                        if( str_cmp( key_tab[i].key, "AffdBy" ) 
                            && ( str_cmp( key_tab[i].key, "Act" ) ))
                            *(int *)key_tab [i].ptrs [j] = tmpi;
                        else if ( !str_cmp( key_tab[i].key, "AffdBy" ) )
                            ch->affected_by = tmpi;
                        else if ( !str_cmp( key_tab[i].key, "Act" ) )
                            ch->act = tmpi;
                    }
               }
      
           if( status )
           { 
               fread_to_eol( fp );
               continue;
           } 
           else
               key_tab [i].string = SPECIFIED;
       }  
       else if ( *word == '*' || !str_cmp( word, "Nm" ) )
           fread_to_eol( fp );
       else if ( !str_cmp( word, "End" ) )
           break;
       else if ( !str_cmp( word, "Room" ) )
       {
           nRoomNum =  fread_number( fp, &status );

           ch->in_room = get_room_index( nRoomNum );
           
           if( !ch->in_room )
               ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
           
           if( ch->pcdata->version < 13 && ch->class == KNIGHT)
               ch->in_room = get_room_index(11000);
           if( ch->pcdata->version < 15)
               ch->in_room = get_room_index(11000);
           if( ch->pcdata->version < 17 && (ch->class == KNIGHT || ch->class == WARLORD || ch->class == MARTIALMAN))
               ch->in_room = get_room_index(11000);
           if( ch->pcdata->version < 19 && ch->class == MARTIALMAN)
               ch->in_room = get_room_index(11000);
           if( ch->pcdata->version < 21 && ch->class == WIZARD)
               ch->in_room = get_room_index(11000);
           if( ch->pcdata->version < 23 && ch->class == KNIGHT)
               ch->in_room = get_room_index(11000);
       }
       else if ( !str_cmp( word, "Race" ) )
       {
           i  = race_lookup( p=fread_string( fp, &status ) );
           free_string( p );
           
           if( status )
               bug( "Fread_char: Unknown Race.", 0 );
           else
           {
               if( i == 60)
                   ch ->race = 5;
               else
                   ch->race = i;
           }
           
           ch->race = URANGE( 0, ch->race, MAX_RACE);
       }
       else if ( !str_cmp(word, "ExSavThr")) 
       {
           if( ch->pcdata->version < 1 ) 
           {
               for( i=1; i<16; ++i )
                    ch->saving_throw[i] = fread_number(fp, &status);
 
               ch->saving_throw[16] = 0;
           }
           else 
           {
               for( i=1; i<17; ++i )
                    ch->saving_throw[i] = fread_number(fp, &status);

           }
       }
       else if ( !str_cmp( word, "Skll" ) )
       {
           char * sbuf;
           i  = fread_number( fp, &status );
           sbuf = fread_word(fp, &status1);
           sn = skill_lookup(sbuf);
           
           if( ch->pcdata->version < 2 ) 
           {
               if( !str_cmp(sbuf, "shot arrow")) 
                    sn = skill_lookup("archery");
           }

           if( ch->pcdata->version < 9 )
               i %= 1000;
            
           if( ch->pcdata->version < 10 && ch->class == SWORDSMAN )
               i %= 1000;
            
           //add by jye 0940725
           if( ch->pcdata->version < 11 && ch->class == SORCERER )
           {
           	   //®³±¼old sor skill
           	   if( !str_cmp(sbuf, "gate")
           	       || !str_cmp(sbuf, "water spirit")
           	       || !str_cmp(sbuf, "fire elemental")
           	       || !str_cmp(sbuf, "clay golem")) 
           	       i = 0;
           	   else
                   i %= 1000;
           }
              
           if( ch->pcdata->version < 12 && (ch->class == SHAMAN || ch->class == LORD))
           {
               i %= 1000;
           }

           if( ch->pcdata->version < 13 && ch->class == KNIGHT)
           {
               i %= 1000;
           }
           if( ch->pcdata->version < 16 && ch->class == WIZARD){
               i %= 1000;  
           }
           if( ch->pcdata->version < 17 && (ch->class == KNIGHT || ch->class == WARLORD || ch->class == MARTIALMAN)){
               i %= 1000;  
           }
           if( ch->pcdata->version < 21 && ch->class == WIZARD){
               i %= 1000;  
           }
           
           if (ch->pcdata->version < 28) {
             if( !str_cmp(sbuf, "convalesce")) {
               sn = gsn_salvation;
             }
           }

           if( ch->pcdata->version < 30) {
             if (ch->pcdata->prime_attr == APPLY_DEX ||
                 ch->class == RANGER || ch->class == MONK ||
                 ch->class == NINJA || ch->class == ASSASSIN){
               i %= 1000;
             }
           }

           if( status || status1 )
           {
               bug( "Fread_char: Error reading skill.", 0 );
               fread_to_eol( fp );
               continue;
           }
      
           if( sn < 0 )
               bug( "Fread_char: unknown skill.", 0 );
           else
               ch->pcdata->learned[sn] = i;
       }
       else if ( !str_cmp ( word, "PS_ATTR" ) )
       {
            ch->pcdata->prime_attr = fread_number( fp, &status );
            ch->pcdata->snd_attr = fread_number( fp, &status );
            ch->pcdata->prime_attr = URANGE( 0, ch->pcdata->prime_attr, 5);
            ch->pcdata->snd_attr = URANGE( 0, ch->pcdata->snd_attr, 5);
            if( ch->pcdata->version < 20 && (ch->class == NINJA || ch->class == WARLORD || ch->class == KNIGHT)
                && ch->pcdata->prime_attr == APPLY_CON)
                ch->in_room = get_room_index(11000);

           if( ch->pcdata->version < 22 && ch->pcdata->prime_attr == APPLY_INT){
               if(nRoomNum >= 2950 && nRoomNum <= 3399){
                   ch->in_room = get_room_index(11000);
               }
           }
           if (ch->pcdata->version < 24 && ch->pcdata->prime_attr == APPLY_DEX){
               sn = skill_lookup("venomous strike");
               ch->pcdata->learned[sn] = 70;
           }
           if (ch->pcdata->version < 27 && ch->pcdata->prime_attr == APPLY_WIS){
				  if (ch->class != BISHOP && ch->class != SAVANT) {
					sn = skill_lookup("turn undead");
					ch->pcdata->learned[sn] = 60;
				  }
           }
           
           if (ch->pcdata->version < 30) {
				 if (ch->pcdata->prime_attr == APPLY_DEX ||
					 ch->class == RANGER || ch->class == MONK ||
					 ch->class == NINJA || ch->class == ASSASSIN){
				   ch->pcdata->skexp_used = 0;
				 }
           }
           
           if (ch->pcdata->version < 31 && ch->pcdata->prime_attr == APPLY_INT){
				  if (ch->class != ALCHEMIST && ch->class != WIZARD) {
					sn = skill_lookup("thunder strike");
					ch->pcdata->learned[sn] = 90;
				 }
           }
		   
           if (ch->pcdata->version < 31 && ch->pcdata->prime_attr == APPLY_WIS){
				  if (ch->class != SORCERER && ch->class != SAVANT) {
					sn = skill_lookup("invis");
					ch->pcdata->learned[sn] = 85;
				 }
           }
		   
           if (ch->pcdata->version < 32 && ch->pcdata->prime_attr == APPLY_WIS){
				  if (ch->class != BISHOP || ch->class != SAVANT) {
					sn = skill_lookup("dark containment");
					ch->pcdata->learned[sn] = 60;
				 }
			}
       }
       else if ( !str_cmp ( word, "Afft" ) )
       {
      
           int status;
      
           paf     = new_affect();
      
           paf->type = affect_lookup( p=fread_string( fp, &status ) );
           free_string( p );
           paf->duration   = fread_number( fp, &status );
           paf->modifier   = fread_number( fp, &status );
           paf->location   = fread_number( fp, &status );
           paf->bitvector    = fread_number( fp, &status );
           paf->deleted    = FALSE;
           paf->next     = ch->affected;
           ch->affected    = paf;
       }
       else if( !str_cmp ( word, "QMARK" ) )
       {
           p = fread_string(fp, &status);
           if( is_qmark(ch, p) != NULL)
           {
               //already has this qmark, so we skip it and keep log
               /*qmark->timer    = */fread_number( fp, &status );
               /*qmark->v0     = */fread_number( fp, &status );
               /*qmark->v1     =*/ fread_number( fp, &status );
               /*qmark->v2     =*/ fread_number( fp, &status );
               /*qmark->fDeath     = */fread_number( fp, &status );
               sprintf( buf, "fread_char: %s has duplicated qmark '%s' in pfile.", ch->name, p);
               bug( buf, 0 );
               fread_to_eol( fp );
           }
           else 
           {
               qmark = set_qmark(p);
               //qmark = set_qmark( p = fread_string( fp, &status ) );
               free_string( p );
               qmark->timer    = fread_number( fp, &status );
               qmark->v0     = fread_number( fp, &status );
               qmark->v1     = fread_number( fp, &status );
               qmark->v2     = fread_number( fp, &status );
               qmark->fDeath     = fread_number( fp, &status );
               qmark->fSave    = TRUE;
               qmark->owner    = ch;
               qmark->next   = ch->qmark;
               if( ch->pcdata->version < 5 ) 
               {
                   if( !str_cmp(p, "immhall_for_skillexp")
                       || !str_cmp(p, "immhall_for_skillexp2"))
                   {
                       qmark->v0 = 0;
                       qmark->v1 = 0;
                       qmark->v2 = 0;
                   }
               } 
               
               if( ch->qmark )
                   ch->qmark->prev = qmark;
                   
               ch->qmark   = qmark;
               
               if( qmark->timer > 0 )
               {
                   qmark->next_in_list = qmark_list;
                   if( qmark_list )
                       qmark_list->prev_in_list = qmark;
                       
                   qmark_list    = qmark;
               }
           }
       }
       else if( !str_cmp( word, "CLAN" ) )
       {
           p = fread_string( fp, &status );
           //    strcpy( buf, p );
           for( cn = clan_list; cn; cn = cn->next )
                if( !str_cmp( p, cn->name ) )
                {
                  ch->clan = cn;
                  break;
                }
                
           free_string( p );
               
           if( !cn )
           {
               if(!ch->desc)
                   bug( ch->name, 0 );
               else
               {
                   write_to_buffer( ch->desc, "±zªºÀ°¬£¤w¸g¤£¦s¦b¤F¡C\n\r", 0 );
                   //ch->pcdata->respect --;
                   ch->pcdata->clantitle = NULL;
                   ch->pcdata->clanlevel = 0;
                   ch->pcdata->clanstatistics = 0;
               }
           }
       }
       else
       {
           sprintf( buf, "fread_char: Unknown key '%s' in pfile.", word );
           bug( buf, 0 );
           fread_to_eol( fp );
       }
        
  }

  /* Require all manditory fields, set defaults */
  for( i = 0; *key_tab [i].key; i++ )
  {
       if( key_tab [i].string == SPECIFIED || key_tab [i].deflt == DEFLT )
           continue;
       
       if( key_tab [i].deflt == MAND )
       {
           sprintf( buf, "Manditory field '%s' missing from pfile.",
             key_tab [i].key );
           bug( buf, 0 );
           error_count++;
           continue;
       }
       
       /* This if/else sets default strings and numbers */
       
       if( key_tab [i].string && key_tab [i].deflt )
       {
           free_string( *(char **)key_tab [i].ptrs [0] );
           *(char **)key_tab [i].ptrs [0] = str_dup( (char *)(key_tab [i].deflt) );
       }
       else
           for( j = 0; key_tab [i].ptrs [j]; j++ )
                *(int *)key_tab [i].ptrs [j] = key_tab [i].deflt;
  }

  /* Fixups */
  ch->protector = NULL;
  ch->protecting = NULL;
  ch->next_in_protect = NULL;
  ch->neweqbank = NULL;
  ch->act |= PLR_COMBINE;
    
  //skill expÂk¹s ver 2-> 3
  if( ch->pcdata->version < 5 ) 
  {
      ch->pcdata->skexp_earned = 0;
      ch->pcdata->skexp_used = 0;
      //ch->pcdata->skexp_earned_max = 0;
  }
  
  if( ch->pcdata->version < 9 )
      ch->pcdata->skexp_used = 0;

  if( ch->pcdata->version < 10 && ch->class == SWORDSMAN )
      ch->pcdata->skexp_used = 0;

  //add by jye 0940725
  if( ch->pcdata->version < 11 && ch->class == SORCERER )
      ch->pcdata->skexp_used = 0;

  if( ch->pcdata->version < 12 && (ch->class == SHAMAN || ch->class == LORD))
      ch->pcdata->skexp_used = 0;

  if( ch->pcdata->version < 13 && ch->class == KNIGHT)
      ch->pcdata->skexp_used = 0;

  if( ch->pcdata->version < 16 && ch->class == WIZARD)
      ch->pcdata->skexp_used = 0;

  if( ch->pcdata->version < 17 && (ch->class == KNIGHT || ch->class == WARLORD || ch->class == MARTIALMAN))
      ch->pcdata->skexp_used = 0;

  if( ch->pcdata->version < 18)
      ch->max_mana += ch->level * 10;

  if( ch->pcdata->version < 21 && ch->class == WIZARD)
      ch->pcdata->skexp_used = 0;
  
  if( ch->pcdata->version < 30) {
    if (ch->pcdata->prime_attr == APPLY_DEX ||
        ch->class == RANGER || ch->class == MONK ||
        ch->class == NINJA || ch->class == ASSASSIN){
      int sn = 0;
      for (sn = 0; sn < MAX_SKILL; ++sn) {
        ch->pcdata->learned[sn] %= 1000;
      }

      ch->pcdata->skexp_used = 0;
      ch->in_room = get_room_index(11000);
    }
  }
	if( ch->pcdata->version < 36
		&& (   (ch->pcdata->learned[gsn_dual] > 1000)
			|| (ch->pcdata->learned[gsn_dodge] > 1000)
			|| (ch->pcdata->learned[gsn_block] > 1000)
			|| (ch->pcdata->learned[gsn_parry] > 1000)
			|| (ch->pcdata->learned[gsn_second_attack] > 1000)
			|| (ch->pcdata->learned[gsn_third_attack] > 1000)
			|| (ch->pcdata->learned[gsn_fourth_attack] > 1000)
			|| (ch->pcdata->learned[gsn_fifth_attack] > 1000)
			|| (ch->pcdata->learned[gsn_disarm] > 1000)
			|| (ch->pcdata->learned[gsn_barehand] > 1000)
			|| (ch->pcdata->learned[gsn_bash] > 1000)
			|| (ch->pcdata->learned[gsn_fool] > 1000)
			|| (ch->pcdata->learned[gsn_blind_fight] > 1000)
			|| (ch->pcdata->learned[gsn_bandage] > 1000)
			|| (ch->pcdata->learned[gsn_rescue] > 1000)
			|| (ch->pcdata->learned[gsn_morphing] > 1000)
			|| (ch->pcdata->learned[gsn_scrolls] > 1000)
			|| (ch->pcdata->learned[gsn_staves] > 1000)
			|| (ch->pcdata->learned[gsn_wands] > 1000) ))
	{
		int sn = 0;
		for (sn = 0; sn < MAX_SKILL; ++sn) {
			ch->pcdata->learned[sn] %= 1000;
		}
		ch->pcdata->skexp_used = 0;
		ch->in_room = get_room_index(11000);
	}

	if( ch->pcdata->version < 37
		&& (   (ch->pcdata->learned[gsn_throwing] > 1000)
			|| (ch->pcdata->learned[gsn_acid_breath] > 1000)
			|| (ch->pcdata->learned[gsn_fire_breath] > 1000)
			|| (ch->pcdata->learned[gsn_frost_breath] > 1000)
			|| (ch->pcdata->learned[gsn_gas_breath] > 1000)
			|| (ch->pcdata->learned[gsn_lightning_breath] > 1000)) )
	{
		int sn = 0;
		for (sn = 0; sn < MAX_SKILL; ++sn) {
			ch->pcdata->learned[sn] %= 1000;
		}
		ch->pcdata->skexp_used = 0;
		ch->in_room = get_room_index(11000);
	}

  if( ch->pcdata->version < 14){
      // affect_strip(ch, gsn_polymorph);      
      AFFECT_DATA *paf;

      for ( paf = ch->affected; paf; paf = paf->next ){
          if ( paf->deleted ) continue;

          if ( paf->type == gsn_polymorph ){
              //  affect_remove( ch, paf );
              //  Check for natural_fly and inborn ability skill -- added by keelar
              REMOVE_BIT( ch->affected_by, paf->bitvector );
              int  mod         = -paf->modifier;
              int  nNewRace    = ch->race + mod;
              int  nRaceSklv   = get_skill_level(ch, gsn_inborn_ability);
              bool bOrgFlyStat, bNewFlyStat;

              while(nNewRace >= 64){ // previous MAX_RACE is 64
                  nNewRace -= 64;
              }
              while(nNewRace < 0){
                  nNewRace += 64;
              }

              bOrgFlyStat = IS_SET(race_table[ ch->race ].race_abilities, RACE_FLY);
              bNewFlyStat = IS_SET(race_table[ nNewRace ].race_abilities, RACE_FLY);
              if(bOrgFlyStat){
                  ch->dodge += nRaceSklv * 2 + (nRaceSklv * nRaceSklv) / 2;
                  ch->dodge -= race_table[ch->race].size * 5;
              }
              if(bNewFlyStat){
                  ch->dodge -= nRaceSklv * 2 + (nRaceSklv * nRaceSklv) / 2;
                  ch->dodge += race_table[nNewRace].size * 5;
              }
              ch->race  = nNewRace;
              // end of restoring race              
              paf->deleted = TRUE;
          }
      }
  }

  if (ch->pcdata->version < 25) {
    ch->pcdata->merit *= 10;
    ch->pcdata->merit_spent *= 10;
  }
  
  if (ch->pcdata->version < 26) {
    if (ch->class >= WIZARD) {
      gain_skill_exp(ch, 100000);
      send_to_char("§A¦]¬°ÂàÂ¾±o¨ì 10 ¸UÂI§Þ¯à¸gÅçÂI¡I\n\r", ch);
    }
  }

  REMOVE_BIT(ch->affected_by, AFF_BATTLE);
  // if ( ch->pcdata->skexp_earned_max == 0)
  //    {
  //      ch->pcdata->skexp_earned_max = ch->pcdata->skexp_earned;
  //    }

  if( strlen(ch->description) > 500 )
  {
      char debuf [501];
      strncpy(debuf, ch->description, 500);
      free_string(ch->description);
      ch->description = str_dup(debuf);
  }

  if( ch->pcdata->title && isalnum ( *ch->pcdata->title ) )
  {
      sprintf( buf, "%s", ch->pcdata->title );
      free_string( ch->pcdata->title );
      ch->pcdata->title = str_dup( buf );
  }

  HpMnMvMultiply_Char(ch);

  AbnormalSkexpCheck(ch);

  return error_count;

}

void recover( FILE *fp, long fpos )
{
    char  buf[ MAX_STRING_LENGTH ];

    fseek( fp, fpos, 0 );

    while ( !feof (fp) )
    {
  fpos = ftell( fp );

  if ( !fgets( buf, MAX_STRING_LENGTH, fp ) )
      return;

  if ( !strncmp( buf, "#OBJECT", 7 ) ||
       !strncmp( buf, "#END", 4 ) )
  {
      fseek( fp, fpos, 0 );
      return;
  }
    }
}

//OBJ_HP * obj_hp_list;

int fread_obj( CHAR_DATA *ch, FILE *fp )
{
	EXTRA_DESCR_DATA *ed;
	OBJ_DATA       obj;
	OBJ_DATA       *new_obj;
	AFFECT_DATA      *paf;
	char        buf[ MAX_STRING_LENGTH ];
	char       *spell_name = NULL;
	char       *p    = NULL;
	char       *word;
	char       *tmp_ptr;
	bool        fNest;
	bool        fVnum;
	long        fpos;
	int         iNest;
	int         iValue;
	int         status;
	int         sn;
	int         vnum;
	int         num_keys;
	int         last_key   = 0;
	int         i, j, tmpi;
	char        *cor_obj  = "This object was corrupted. ";
	char        **corobj  = &cor_obj;

	struct key_data key_tab [] =
	{
		{ "Name",  TRUE,  MAND,     { &obj.name,        NULL } },
		{ "ShortDescr",  TRUE,  (int) corobj,   { &obj.short_descr, NULL } },
		{ "Description", TRUE,  (int) corobj,   { &obj.description, NULL } },
		//{ "ExtraFlags",  FALSE, MAND,     { &obj.extra_flags, NULL } },
		{ "WearFlags",   FALSE, MAND,     { &obj.wear_flags,  NULL } },
		{ "WearLoc",   FALSE, MAND,     { &obj.wear_loc,    NULL } },
		{ "ItemType",  FALSE, MAND,     { &obj.item_type,   NULL } },
		{ "Weight",  FALSE, 10,     { &obj.weight,      NULL } },
		{ "Level",   FALSE, ch->level,    { &obj.level,       NULL } },
		{ "Timer",   FALSE, 0,      { &obj.timer,       NULL } },
		{ "Cost",  FALSE, 300,      { &obj.cost,        NULL } },
		{ "Class",   FALSE, 1048576,    { &obj.class,       NULL } },
		{ "Race",  FALSE, 1048576,    { &obj.race,        NULL } },
		{ "Serial", FALSE,  time(NULL), {&obj.serial, NULL}},
		{ "Values",  FALSE, MAND,     { &obj.value [0],
										&obj.value [1],
										&obj.value [2],
										&obj.value [3],
										NULL } },
		{ "\0",    FALSE, 0            } };

	memset( &obj, 0, sizeof( OBJ_DATA ) );

	obj.name      = str_dup( "" );
	obj.short_descr = str_dup( "" );
/*    obj.cname     = str_dup( "" );*/
	obj.description = str_dup( "" );
	obj.deleted     = FALSE;

	fNest     = FALSE;
	fVnum     = TRUE;
	iNest     = 0;

	new_obj = new_object ();

	for ( num_keys = 0; *key_tab [num_keys].key; )
		num_keys++;

	for ( fpos = ftell( fp ) ; !feof( fp ) ; )
	{

		word = fread_word( fp, &status );

		if( !word ) {
			bug("Fread_obj error",0);
			fread_to_eol(fp);
			break;
		}

		for ( i = last_key; i < last_key + num_keys && str_cmp( key_tab [i % num_keys].key, word ); )
			i++;

		i = i % num_keys;

		if ( !str_cmp( key_tab [i].key, word ) )
			last_key = i + 1;
		else
			i = num_keys;

		if ( !str_cmp( word, "Nest" ) )
		{

			iNest = fread_number( fp, &status );

			if ( status ) /* Losing track of nest level is bad */
				iNest = 0;  /* This makes objs go to inventory */
			else if ( iNest < 0 || iNest >= MAX_NEST )
				bug( "Fread_obj: bad nest %d.", iNest );
			else
			{
				rgObjNest[iNest] = new_obj;
				fNest = TRUE;
			}
		}
		else if ( *key_tab [i].key )    /* Key entry found in key_tab */
		{
			if ( key_tab [i].string == SPECIFIED )
				bug( "Key already specified.", 0 );

			/* Entry is a string */
			else if ( key_tab [i].string )
			{
				if ( ( p = fread_string( fp, &status ) ) && !status ) 
				{
					free_string ( * (char **) key_tab [i].ptrs [0] );
					* (char **) key_tab [i].ptrs [0] = p;
				}
			}

			/* Entry is an integer */
			else
				for ( j = 0; key_tab [i].ptrs [j]; j++ )
				{
					tmpi = fread_number( fp, &status );
					if ( !status )
						* (int *) key_tab [i].ptrs [j] = tmpi;
				}

			if ( status )
			{
				fread_to_eol( fp );
				continue;
			}
			else
				key_tab [i].string = SPECIFIED;
		}

		else if ( *word == '*' )
			fread_to_eol( fp );
		else if ( !str_cmp( word, "End" ) )
		{
			if ( !fNest || !fVnum )
			{
				bug( "Fread_obj: incomplete object.", 0 );

				recover    ( fp, fpos      );
				free_string( obj.name      );
				free_string( obj.short_descr );
				//free_string( obj.cname       );
				free_string( obj.description );
				extract_obj( new_obj       );

				return FALSE;
			}
			break;
		}
		else if ( !str_cmp( word, "ExtraFlags")){
			//char ebuf[MAX_STRING_LENGTH];
			obj.extra_flags = fread_number( fp, &status );
			/*if( obj.extra_flags != obj.pIndexData->extra_flags )
			{
				obj.extra_flags |= obj.pIndexData->extra_flags;
				obj.extra_flags |= ITEM_RECOVER;
				sprintf(ebuf,"%s has one %d.", ch->name, obj.pIndexData->vnum);
				bug(ebuf, 0);
				recover    ( fp, fpos      );
			} */
		}
		else if ( !str_cmp( word, "Spell" ) )
		{

			iValue = fread_number( fp, &status );

			if ( !status )
				spell_name = fread_word( fp, &status );

			if ( status ) /* Recover is to skip spell */
			{
				fread_to_eol( fp );
				continue;
			}

			sn = skill_lookup( spell_name );

			if ( iValue < 0 || iValue > 3 )
				bug( "Fread_obj: bad iValue %d.", iValue );
			else if ( sn < 0 )
				bug( "Fread_obj: unknown skill.", 0 );
			else
				obj.value [iValue] = sn;
		}
		else if ( !str_cmp( word, "Vnum" ) )
		{

			vnum = fread_number( fp, &status );

			if ( status )   /* Can't live without vnum */
			{
				recover ( fp, fpos );
				free_string( obj.name );
				free_string( obj.short_descr );
				///free_string( obj.cname       );
				free_string( obj.description );
				extract_obj( new_obj );
				return FALSE;
			}

			if ( !( obj.pIndexData = get_obj_index( vnum ) ) )
			{
				obj.pIndexData = get_obj_index( OBJ_VNUM_DUMMY );
				bug( "Fread_obj: bad vnum %d.", vnum );
			}
			else
				fVnum = TRUE;

			obj.vrace = obj.pIndexData->vrace;
			obj.material = obj.pIndexData->material;
		}

		/* The following keys require extra processing */

		if ( !str_cmp( word, "Affect" ) )
		{
			paf = new_affect ();

			paf->type     = fread_number( fp, &status );
			paf->duration   = fread_number( fp, &status );
			paf->modifier   = fread_number( fp, &status );
			paf->location   = fread_number( fp, &status );
			paf->bitvector  = fread_number( fp, &status );

			paf->next = obj.affected;
			obj.affected = paf;
		}
		else if ( !str_cmp( word, "Material" ))
		{
			char * tmp;
			tmp = fread_string( fp, &status );
			obj.material  = material_lookup( buf );
		}

		if ( !str_cmp( word, "Hidden" ) )
		{
			int vnum_tmp;
			OBJ_HP * hp;
    
			vnum_tmp  = fread_number( fp, &status );
    
		/*
			for( hp = obj_hp_list; hp; hp = hp->next )
			if ( hp->vnum == vnum_tmp )
				break; */
			//if ( hp )
			//  obj.hp = hp;
			//else
			//{
			hp = new_hiddenpower();
			hp->vnum  = vnum_tmp;
			hp->hp_name = fread_string( fp, &stat );
			hp->percent = fread_number( fp, &stat );
			hp->level = fread_number( fp, &stat );
			hp->event = fread_number( fp, &stat );
			//hp->next  = obj_hp_list;
			//obj_hp_list = hp;
			hp->next = obj.hp;
			obj.hp = hp;
			//}
		}
		else if ( !str_cmp( word, "ExtraDescr" ) )
		{
			tmp_ptr = fread_string( fp, &status );

			if ( !status )
				p = fread_string( fp, &status );

			if ( status )
			{
				recover ( fp, fpos );
				free_string( obj.name );
				//free_string( obj.cname );
				free_string( obj.short_descr );
				free_string( obj.description );
				extract_obj( new_obj );
				return FALSE;
			}

			ed = new_extra_descr ();

			ed->keyword = tmp_ptr;
			ed->description = p;
			ed->next = obj.extra_descr;
			obj.extra_descr = ed;
		}

       /* Added by Razgriz 20050918
        * Read obj_extra_value if exist
        */
		if ( !str_cmp ( word, "ExValue" ) )
		{
			obj.extra_value.used = TRUE;
			obj.extra_value.v[0] = fread_number( fp, &status );
			obj.extra_value.v[1] = fread_number( fp, &status );
			obj.extra_value.t[0] = fread_number( fp, &status );
			obj.extra_value.t[1] = fread_number( fp, &status );
		}
	}
    /* Require all manditory fields, set defaults */

	for ( i = 0; *key_tab [i].key; i++ )
	{

		if ( key_tab [i].string == SPECIFIED || key_tab [i].deflt == DEFLT )
			continue;

		if ( key_tab [i].deflt == MAND )
		{
			sprintf( buf, "Manditory obj field '%s' missing from pfile.",
			key_tab [i].key );
			bug( buf, 0 );

			recover ( fp, fpos  );
			free_string( obj.name  );
			free_string( obj.short_descr );
			//free_string( obj.cname );
			free_string( obj.description );
			extract_obj( new_obj   );

			return FALSE;
		}

		/* This if/else sets default strings and numbers */

		if ( key_tab [i].string && key_tab [i].deflt ) {
			* (char **) key_tab [i].ptrs [0] =
				str_dup ( * (char **) key_tab [i].deflt );
		}
		else
			for ( j = 0; key_tab [i].ptrs [j]; j++ )
				* (int *) key_tab [i].ptrs [j] = key_tab [i].deflt;
	}

	memcpy( new_obj, &obj, sizeof( OBJ_DATA ) );

	new_obj->next = object_list;
	object_list   = new_obj;

	new_obj->pIndexData->count++;
	if ( iNest == 0 || !rgObjNest[iNest] )
		obj_to_char( new_obj, ch );
	else
		obj_to_obj( new_obj, rgObjNest[iNest-1] );

	return TRUE;
}


int fread_newbank_obj( BANK_DATA *ch, FILE *fp )
{
    EXTRA_DESCR_DATA *ed;
    OBJ_DATA       obj;
    OBJ_DATA       *new_obj;
    AFFECT_DATA      *paf;
    char        buf[ MAX_STRING_LENGTH ];
    char       *spell_name = NULL;
    char       *p    = NULL;
    char       *word;
    char      *tmp = NULL;
    char       *tmp_ptr;
    bool        fVnum;
    bool    fNest;
    int       iNest;
    long        fpos;
    int         iValue;
    int         status;
    int         sn;
    int         vnum;
    int         num_keys;
    int         last_key   = 0;
    int         i, j, tmpi;
    char        *cor_obj  = "This object was corrupted. ";
    char        **corobj  = &cor_obj;
    char        *coor_obj = "This object was corrupted 2. ";
    char        **coorobj = &coor_obj;



    struct key_data key_tab [] =
      {
  { "Name",  TRUE,  MAND,     { &obj.name,        NULL } },
  { "ShortDescr",  TRUE,  (int) corobj,   { &obj.short_descr, NULL } },
/*  { "Cname",   TRUE,  (int) &obj.short_descr,{ &obj.cname,        NULL } },*/
  { "Description", TRUE,  (int) coorobj,    { &obj.description, NULL } },
  //{ "ExtraFlags",  FALSE, MAND,     { &obj.extra_flags, NULL } },
  { "WearFlags",   FALSE, MAND,     { &obj.wear_flags,  NULL } },
  { "WearLoc",   FALSE, MAND,     { &obj.wear_loc,    NULL } },
  { "ItemType",  FALSE, MAND,     { &obj.item_type,   NULL } },
  { "Weight",  FALSE, 10,     { &obj.weight,      NULL } },
  { "Level",   FALSE, 42,   { &obj.level,       NULL } },
  { "Timer",   FALSE, 0,      { &obj.timer,       NULL } },
  { "Cost",  FALSE, 300,      { &obj.cost,        NULL } },
  { "Class",   FALSE, 1048576,    { &obj.class,       NULL } },
  { "Race",  FALSE, 1048576,    { &obj.race,        NULL } },
  { "Serial", FALSE,  time(NULL), {&obj.serial, NULL}},
  { "Values",  FALSE, MAND,     { &obj.value [0],
                &obj.value [1],
                &obj.value [2],
                &obj.value [3],   NULL } },
  { "Recall",  FALSE, ROOM_VNUM_TEMPLE,     { &obj.value[4],        NULL } },
  { "\0",    FALSE, 0            } };

    memset( &obj, 0, sizeof( OBJ_DATA ) );

    obj.name      = str_dup( "" );
    obj.short_descr = str_dup( "" );
/*    obj.cname     = str_dup( "" );*/
    obj.description = str_dup( "" );
    obj.deleted     = FALSE;

    fNest     = FALSE;
    fVnum     = TRUE;
    iNest     = 0;

    new_obj = new_object ();

    for ( num_keys = 0; *key_tab [num_keys].key; )
  num_keys++;

    for ( fpos = ftell( fp ) ; !feof( fp ) ; )
    {

  word = fread_word( fp, &status );

  if( !word ) {
      bug("fread_neweqbank_obj error",0);
      fread_to_eol(fp);
      break;
  }
  for ( i = last_key;
        i < last_key + num_keys &&
    str_cmp( key_tab [i % num_keys].key, word ); )
      i++;

  i = i % num_keys;

  if ( !str_cmp( key_tab [i].key, word ) )
      last_key = i + 1;
  else
      i = num_keys;
  

  if ( !str_cmp( word, "Nest" ) )
  {

      iNest = fread_number( fp, &status );

      if ( status ) /* Losing track of nest level is bad */
    iNest = 0;  /* This makes objs go to inventory */

      else if ( iNest < 0 || iNest >= MAX_NEST )
    bug( "Fread_obj: bad nest %d.", iNest );

      else
      {
    rgObjNest[iNest] = new_obj;
    fNest = TRUE;
      }
  }
  else if ( *key_tab [i].key )    /* Key entry found in key_tab */
  {
      if ( key_tab [i].string == SPECIFIED )
    bug( "Key already specified.", 0 );

      /* Entry is a string */

      else if ( key_tab [i].string )
      {
    if ( ( p = fread_string( fp, &status ) ) && !status )
    {
       free_string ( * (char **) key_tab [i].ptrs [0] );
       * (char **) key_tab [i].ptrs [0] = p;
    }
      }

      /* Entry is an integer */
      else
    for ( j = 0; key_tab [i].ptrs [j]; j++ )
    {
        tmpi = fread_number( fp, &status );
        if ( !status )
      * (int *) key_tab [i].ptrs [j] = tmpi;
    }

      if ( status )
      {
    fread_to_eol( fp );
    continue;
      }
      else
    key_tab [i].string = SPECIFIED;
  }

  else if ( *word == '*' )
      fread_to_eol( fp );

  else if ( !str_cmp( word, "End" ) )
  {
      if ( !fVnum )
      {
    bug( "Fread_obj: incomplete object.", 0 );

    recover    ( fp, fpos      );
    free_string( obj.name      );
    free_string( obj.short_descr );
/*    free_string( obj.cname       );*/
    free_string( obj.description );
    extract_obj( new_obj       );

    return FALSE;
      }
      break;
  }
  else if ( !str_cmp( word, "ExtraFlags")){
      //char ebuf[MAX_STRING_LENGTH];
      obj.extra_flags = fread_number( fp, &status );
     //if( obj.extra_flags != obj.pIndexData->extra_flags )
     //{
         //obj.extra_flags |= obj.pIndexData->extra_flags;
         //obj.extra_flags |= ITEM_RECOVER;
        // sprintf(ebuf,"%s has one %d.", ch->name, obj.pIndexData->vnum);
         //bug(ebuf, 0);
    //recover    ( fp, fpos      );
     //}

  }
  else if ( !str_cmp( word, "Spell" ) )
  {

      iValue = fread_number( fp, &status );

      if ( !status )
    spell_name = fread_word( fp, &status );

      if ( status ) /* Recover is to skip spell */
      {
    fread_to_eol( fp );
    continue;
      }

      sn = skill_lookup( spell_name );

      if ( iValue < 0 || iValue > 3 )
    bug( "Fread_obj: bad iValue %d.", iValue );

      else if ( sn < 0 )
    bug( "Fread_obj: unknown skill.", 0 );

      else
    obj.value [iValue] = sn;
  }

  else if ( !str_cmp( word, "Material" ))
  {
      tmp = fread_string( fp, &status );
      obj.material  = material_lookup( buf );
  }

  else if ( !str_cmp( word, "Vnum" ) )
  {

      vnum = fread_number( fp, &status );

      if ( status )   /* Can't live without vnum */
      {
    recover    ( fp, fpos      );
    free_string( obj.name      );
    free_string( obj.short_descr );
/*    free_string( obj.cname       );*/
    free_string( obj.description );
    extract_obj( new_obj       );
    return FALSE;
      }

      if ( !( obj.pIndexData = get_obj_index( vnum ) ) )
      {
    obj.pIndexData = get_obj_index( OBJ_VNUM_DUMMY );
    bug( "Fread_obj: bad vnum %d.", vnum );
      }
      else
    fVnum = TRUE;
      obj.vrace = obj.pIndexData->vrace;
      obj.material = obj.pIndexData->material;
  }

    /* The following keys require extra processing */

  if ( !str_cmp( word, "Affect" ) )
  {
      paf = new_affect ();

      paf->type     = fread_number( fp, &status );
      paf->duration   = fread_number( fp, &status );
      paf->modifier   = fread_number( fp, &status );
      paf->location   = fread_number( fp, &status );
      paf->bitvector  = fread_number( fp, &status );

      paf->next = obj.affected;
      obj.affected = paf;
  }

  if ( !str_cmp( word, "Hidden" ) )
  {
    int vnum_tmp;
    OBJ_HP * hp;
    
    vnum_tmp  = fread_number( fp, &status );
    
    /*
    for( hp = obj_hp_list; hp; hp = hp->next )
      if ( hp->vnum == vnum_tmp )
        break; */
    //if ( hp )
    //  obj.hp = hp;
    //else
    //{
          hp    = new_hiddenpower();
          hp->vnum  = vnum_tmp;
          hp->hp_name = fread_string( fp, &stat );
          hp->percent = fread_number( fp, &stat );
          hp->level = fread_number( fp, &stat );
          hp->event = fread_number( fp, &stat );
          //hp->next  = obj_hp_list;
        //obj_hp_list = hp;
      hp->next  = obj.hp;
      obj.hp    = hp;
    //}
  }
  
  else if ( !str_cmp( word, "ExtraDescr" ) )
  {
      tmp_ptr = fread_string( fp, &status );

      if ( !status )
    p = fread_string( fp, &status );

      if ( status )
      {
    recover    ( fp, fpos      );
    free_string( obj.name      );
/*    free_string( obj.cname       );*/
    free_string( obj.short_descr );
    free_string( obj.description );
    extract_obj( new_obj       );
    return FALSE;
      }

      ed = new_extra_descr ();

      ed->keyword     = tmp_ptr;
      ed->description = p;
      ed->next      = obj.extra_descr;
      obj.extra_descr = ed;
  }

    /* Added by Razgriz 20050918
     * Read obj_extra_value if exist
     */
    if ( !str_cmp ( word, "ExValue" ) )
    {
    	obj.extra_value.used = TRUE;
    	obj.extra_value.v[0] = fread_number( fp, &status );
        obj.extra_value.v[1] = fread_number( fp, &status );
        obj.extra_value.t[0] = fread_number( fp, &status );
        obj.extra_value.t[1] = fread_number( fp, &status );
    }

  }
    /* Require all manditory fields, set defaults */

    for ( i = 0; *key_tab [i].key; i++ )
    {

  if ( key_tab [i].string == SPECIFIED ||
       key_tab [i].deflt == DEFLT )
      continue;

  if ( key_tab [i].deflt == MAND )
  {
      sprintf( buf, "Manditory obj field '%s' missing from pfile.",
        key_tab [i].key );
      bug( buf, 0 );

      recover    ( fp, fpos  );
      free_string( obj.name  );
      free_string( obj.short_descr );
/*      free_string( obj.cname   );*/
      free_string( obj.description );
      extract_obj( new_obj   );

      return FALSE;
  }

    /* This if/else sets default strings and numbers */

  if ( key_tab [i].string && key_tab [i].deflt ) {
      * (char **) key_tab [i].ptrs [0] =
    str_dup ( * (char **) key_tab [i].deflt );
  }
  else
      for ( j = 0; key_tab [i].ptrs [j]; j++ )
    * (int *) key_tab [i].ptrs [j] = key_tab [i].deflt;
    }

    memcpy( new_obj, &obj, sizeof( OBJ_DATA ) );
/*
    new_obj->next = object_list;
    object_list   = new_obj;
*/
    new_obj->pIndexData->count++;


    if ( iNest == 0 || !rgObjNest[iNest] ){
  new_obj->next_content = ch->objs;
  ch->objs = new_obj;
    }
    else{
  //obj_to_obj( new_obj, rgObjNest[iNest-1] );
  new_obj->next_content = rgObjNest[iNest-1]->contains;
  new_obj->in_obj = rgObjNest[iNest-1];
  new_obj->in_room = NULL;
  new_obj->carried_by = NULL;
  rgObjNest[iNest-1]->contains = new_obj;
    }
    return TRUE;
}

int fread_bank_obj( CHAR_DATA *ch, FILE *fp )
{
    EXTRA_DESCR_DATA *ed;
    OBJ_DATA       obj;
    OBJ_DATA       *new_obj;
    AFFECT_DATA      *paf;
    char        buf[ MAX_STRING_LENGTH ];
    char       *spell_name = NULL;
    char       *p    = NULL;
    char       *word;
    char      *tmp = NULL;
    char       *tmp_ptr;
    bool        fVnum;
    long        fpos;
    int         iValue;
    int         status;
    int         sn;
    int         vnum;
    int         num_keys;
    int         last_key   = 0;
    int         i, j, tmpi;
    char        *cor_obj  = "This object was corrupted. ";
    char        **corobj  = &cor_obj;
    char        *coor_obj = "This object was corrupted 2. ";
    char        **coorobj = &coor_obj;

    struct key_data key_tab [] =
      {
  { "Name",  TRUE,  MAND,     { &obj.name,        NULL } },
  { "ShortDescr",  TRUE,  (int) corobj,   { &obj.short_descr, NULL } },
/*  { "Cname",   TRUE,  (int) &obj.short_descr,{ &obj.cname,        NULL } },*/
  { "Description", TRUE,  (int) coorobj,    { &obj.description, NULL } },
  //{ "ExtraFlags",  FALSE, MAND,     { &obj.extra_flags, NULL } },
  { "WearFlags",   FALSE, MAND,     { &obj.wear_flags,  NULL } },
  { "WearLoc",   FALSE, MAND,     { &obj.wear_loc,    NULL } },
  { "ItemType",  FALSE, MAND,     { &obj.item_type,   NULL } },
  { "Weight",  FALSE, 10,     { &obj.weight,      NULL } },
  { "Level",   FALSE, ch->level,    { &obj.level,       NULL } },
  { "Timer",   FALSE, 0,      { &obj.timer,       NULL } },
  { "Cost",  FALSE, 300,      { &obj.cost,        NULL } },
  { "Class",   FALSE, 1048576,    { &obj.class,       NULL } },
  { "Race",  FALSE, 1048576,    { &obj.race,        NULL } },
  { "Serial", FALSE,  time(NULL), {&obj.serial, NULL}},
  { "Values",  FALSE, MAND,     { &obj.value [0],
                &obj.value [1],
                &obj.value [2],
                &obj.value [3],   NULL } },
  { "\0",    FALSE, 0            } };

    memset( &obj, 0, sizeof( OBJ_DATA ) );

    obj.name      = str_dup( "" );
    obj.short_descr = str_dup( "" );
/*    obj.cname     = str_dup( "" );*/
    obj.description = str_dup( "" );
    obj.deleted     = FALSE;

    fVnum     = TRUE;

    new_obj = new_object ();

    for ( num_keys = 0; *key_tab [num_keys].key; )
  num_keys++;

    for ( fpos = ftell( fp ) ; !feof( fp ) ; )
    {

  word = fread_word( fp, &status );

  if( !word ) {
      bug("fread_bank_obj error",0);
      fread_to_eol(fp);
      break;
  }

  for ( i = last_key;
        i < last_key + num_keys &&
    str_cmp( key_tab [i % num_keys].key, word ); )
      i++;

  i = i % num_keys;

  if ( !str_cmp( key_tab [i].key, word ) )
      last_key = i + 1;
  else
      i = num_keys;

  if ( *key_tab [i].key )   /* Key entry found in key_tab */
  {
      if ( key_tab [i].string == SPECIFIED )
    bug( "Key already specified.", 0 );

      /* Entry is a string */

      else if ( key_tab [i].string )
      {
    if ( ( p = fread_string( fp, &status ) ) && !status )
    {
       free_string ( * (char **) key_tab [i].ptrs [0] );
       * (char **) key_tab [i].ptrs [0] = p;
    }
      }

      /* Entry is an integer */
      else
    for ( j = 0; key_tab [i].ptrs [j]; j++ )
    {
        tmpi = fread_number( fp, &status );
        if ( !status )
      * (int *) key_tab [i].ptrs [j] = tmpi;
    }

      if ( status )
      {
    fread_to_eol( fp );
    continue;
      }
      else
    key_tab [i].string = SPECIFIED;
  }

  else if ( *word == '*' )
      fread_to_eol( fp );

  else if ( !str_cmp( word, "End" ) )
  {
      if ( !fVnum )
      {
    bug( "Fread_obj: incomplete object.", 0 );

    recover    ( fp, fpos      );
    free_string( obj.name      );
    free_string( obj.short_descr );
/*    free_string( obj.cname       );*/
    free_string( obj.description );
    extract_obj( new_obj       );

    return FALSE;
      }
      break;
  }
  else if ( !str_cmp( word, "ExtraFlags")){
      //char ebuf[MAX_STRING_LENGTH];
      obj.extra_flags = fread_number( fp, &status );
     if( obj.extra_flags != obj.pIndexData->extra_flags )
     {
         //obj.extra_flags |= obj.pIndexData->extra_flags;
         //obj.extra_flags |= ITEM_RECOVER;
        // sprintf(ebuf,"%s has one %d.", ch->name, obj.pIndexData->vnum);
         //bug(ebuf, 0);
         //recover     ( fp, fpos      );
     }

  }
  else if ( !str_cmp( word, "Spell" ) )
  {

      iValue = fread_number( fp, &status );

      if ( !status )
    spell_name = fread_word( fp, &status );

      if ( status ) /* Recover is to skip spell */
      {
    fread_to_eol( fp );
    continue;
      }

      sn = skill_lookup( spell_name );

      if ( iValue < 0 || iValue > 3 )
    bug( "Fread_obj: bad iValue %d.", iValue );

      else if ( sn < 0 )
    bug( "Fread_obj: unknown skill.", 0 );

      else
    obj.value [iValue] = sn;
  }

  else if ( !str_cmp( word, "Material" ))
  {
      tmp = fread_string( fp, &status );
      obj.material  = material_lookup( buf );
  }

  else if ( !str_cmp( word, "Vnum" ) )
  {

      vnum = fread_number( fp, &status );

      if ( status )   /* Can't live without vnum */
      {
    recover    ( fp, fpos      );
    free_string( obj.name      );
    free_string( obj.short_descr );
/*    free_string( obj.cname       );*/
    free_string( obj.description );
    extract_obj( new_obj       );
    return FALSE;
      }

      if ( !( obj.pIndexData = get_obj_index( vnum ) ) )
      {
    obj.pIndexData = get_obj_index( OBJ_VNUM_DUMMY );
    bug( "Fread_obj: bad vnum %d.", vnum );
      }
      else
    fVnum = TRUE;
      obj.vrace = obj.pIndexData->vrace;
      obj.material = obj.pIndexData->material;
  }

    /* The following keys require extra processing */

  if ( !str_cmp( word, "Affect" ) )
  {
      paf = new_affect ();

      paf->type     = fread_number( fp, &status );
      paf->duration   = fread_number( fp, &status );
      paf->modifier   = fread_number( fp, &status );
      paf->location   = fread_number( fp, &status );
      paf->bitvector  = fread_number( fp, &status );

      paf->next = obj.affected;
      obj.affected = paf;
  }

  if ( !str_cmp( word, "Hidden" ) )
  {
    int vnum_tmp;
    OBJ_HP * hp;
    
    vnum_tmp  = fread_number( fp, &status );
    
    /*
    for( hp = obj_hp_list; hp; hp = hp->next )
      if ( hp->vnum == vnum_tmp )
        break; */
    //if ( hp )
    //  obj.hp = hp;
    //else
    //{
          hp    = new_hiddenpower();
          hp->vnum  = vnum_tmp;
          hp->hp_name = fread_string( fp, &stat );
          hp->percent = fread_number( fp, &stat );
          hp->level = fread_number( fp, &stat );
          hp->event = fread_number( fp, &stat );
          //hp->next  = obj_hp_list;
        //obj_hp_list = hp;
      hp->next  = obj.hp;
      obj.hp    = hp;
    //}
  }
  
  else if ( !str_cmp( word, "ExtraDescr" ) )
  {
      tmp_ptr = fread_string( fp, &status );

      if ( !status )
    p = fread_string( fp, &status );

      if ( status )
      {
    recover    ( fp, fpos      );
    free_string( obj.name      );
/*    free_string( obj.cname       );*/
    free_string( obj.short_descr );
    free_string( obj.description );
    extract_obj( new_obj       );
    return FALSE;
      }

      ed = new_extra_descr ();

      ed->keyword     = tmp_ptr;
      ed->description = p;
      ed->next      = obj.extra_descr;
      obj.extra_descr = ed;
  }

    /* Added by Razgriz 20050918
     * Read obj_extra_value if exist
     */
    if ( !str_cmp ( word, "ExValue" ) )
    {
    	obj.extra_value.used = TRUE;
    	obj.extra_value.v[0] = fread_number( fp, &status );
        obj.extra_value.v[1] = fread_number( fp, &status );
        obj.extra_value.t[0] = fread_number( fp, &status );
        obj.extra_value.t[1] = fread_number( fp, &status );
    }

  }
    /* Require all manditory fields, set defaults */

    for ( i = 0; *key_tab [i].key; i++ )
    {

  if ( key_tab [i].string == SPECIFIED ||
       key_tab [i].deflt == DEFLT )
      continue;

  if ( key_tab [i].deflt == MAND )
  {
      sprintf( buf, "Manditory obj field '%s' missing from pfile.",
        key_tab [i].key );
      bug( buf, 0 );

      recover    ( fp, fpos  );
      free_string( obj.name  );
      free_string( obj.short_descr );
/*      free_string( obj.cname   );*/
      free_string( obj.description );
      extract_obj( new_obj   );

      return FALSE;
  }

    /* This if/else sets default strings and numbers */

  if ( key_tab [i].string && key_tab [i].deflt ) {
      * (char **) key_tab [i].ptrs [0] =
    str_dup ( * (char **) key_tab [i].deflt );
  }
  else
      for ( j = 0; key_tab [i].ptrs [j]; j++ )
    * (int *) key_tab [i].ptrs [j] = key_tab [i].deflt;
    }

    memcpy( new_obj, &obj, sizeof( OBJ_DATA ) );
/*
    new_obj->next = object_list;
    object_list   = new_obj;
*/
//    new_obj->pIndexData->count++;

    new_obj->next_content = ch->eqbank;
    ch->eqbank      = new_obj;

    return TRUE;
}

void do_horse_backup( CHAR_DATA *ch, char *argument )
{
    FILE *fp;
    char  buf   [ MAX_STRING_LENGTH ];
    char  strsave [ MAX_INPUT_LENGTH  ];

    if ( ch->desc && ch->desc->original )
  ch = ch->desc->original;

    ch->save_time = current_time;
    if (fpReserve) {
      fclose( fpReserve );
      fpReserve = NULL;
    }

    sprintf( strsave, "%s%s%s%s", HORSE_BACKUP_DIR, initial( ch->pcdata->horse->name ),
      "/", capitalize( ch->pcdata->horse->name ) );

    if ( !( fp = fopen( strsave, "w" ) ) )
    {
  sprintf( buf, "Save_char_obj: fopen %s: ", ch->name );
  bug( buf, 0 );
  perror( strsave );
    }
    else
    {
  fwrite_char( ch, fp );
  if ( ch->carrying )
      fwrite_obj( ch, ch->carrying, fp, 0 );
  fprintf( fp, "#END\n" );
    }
    if( fp ) {
    fflush( fp );
      fclose( fp );
  }
    fpReserve = fopen( NULL_FILE, "r" );
    send_to_char( "[36m¨S°ÝÃD¡C[m\n\r", ch );
    return;
}

void fwrite_neweqbank( CHAR_DATA * ch, FILE* fp );
void free_neweqbank( BANK_DATA * bank );

void do_backup( CHAR_DATA *ch, char *argument )
{
	FILE *fp;
	char buf   [ MAX_STRING_LENGTH ];
	char strsave [ MAX_INPUT_LENGTH  ];
	bool switched =  FALSE;

	if ( IS_NPC( ch ) )
		return;

	if( !ch->desc ) return;

	if ( IS_SET( ch->act, PLR_HORSE ) )
	{
		if ( ch->pcdata->horse )
		{
			ch->pcdata->horse->desc = ch->desc;
			ch = ch->pcdata->horse;
			ch->pcdata->horse->desc = NULL;
			switched = TRUE;
		}
		else
			return;
	}

	if ( ch->level < 5 )
	{
		send_to_char( "[33m§A³Ì¤Ö­n¨ì²Ä¤­¯Å¥H¤W¤~¯à³Æ¥÷¡C[m\n\r", ch );
		if ( switched )
			ch = ch->pcdata->horse;
		return;
	}

	// ·s¼W¦º¤`«á§ä¦^«ÍÅé«e¤@¬q®É¶¡¤ºµLªk backup 2022/10/10
	if ( is_qmark(ch, "death_stop_backup") )
	{
		send_to_char("»°ºò§ä¦^§Aªº«ÍÅé©Mª««~¡A¤~¯à¶i¦æ backup ªº°Ê§@¡C\n\r", ch);
		return;
	}

	if ( ch->desc && ch->desc->original )
		ch = ch->desc->original;

	ch->save_time = current_time;
	fflush( NULL );
	if (fpReserve) {
		fclose( fpReserve );
		fpReserve = NULL;
	}

	sprintf( strsave, "%s%s%s%s", BACKUP_DIR, initial( ch->name ), "/", capitalize( ch->name ) );

	if ( !( fp = fopen( strsave, "w" ) ) )
	{
		sprintf( buf, "Save_char_obj: fopen %s: ", ch->name );
		bug( buf, 0 );
		perror( strsave );
	}
	else
	{
		fwrite_char( ch, fp );
		if ( ch->carrying )
			fwrite_obj( ch, ch->carrying, fp, 0 );
		if ( ch->eqbank )
			fwrite_bank_obj( ch, ch->eqbank, fp, 0 );
		fprintf( fp, "#END\n" );
	}
	if( fp) {
		fflush( fp );
		fclose( fp );
	}
	fpReserve = fopen( NULL_FILE, "r" );
	if ( ch->pcdata->horse )
		do_horse_backup( ch->pcdata->horse, "" );
		send_to_char( "[36m³Æ¥÷§¹¦¨¡C[m\n\r", ch );
	if ( switched )
	{
		ch->pcdata->horse->desc = ch->desc;
		ch = ch->pcdata->horse;
		ch->pcdata->horse->desc = NULL;
	}
		return;
	}

/*
 * Load a char and inventory into a new ch structure.
 */
CHAR_DATA * load_horse_obj( DESCRIPTOR_DATA *d, char *name )
{
    extern char      *daPrompt;
     FILE      *fp;
     CHAR_DATA *ch;
     char       strsave [ MAX_INPUT_LENGTH ];
     bool       found;
     char       sorry_player [] =
       "********************************************************\n\r"
       "** One or more of the critical fields in your player  **\n\r"
       "** file were corrupted since you last played.  Please **\n\r"
       "** contact an administrator or programmer to      **\n\r"
       "** investigate the recovery of your characters.     **\n\r"
       "********************************************************\n\r";
     char       sorry_object [] =
       "********************************************************\n\r"
       "** One or more of the critical fields in your player  **\n\r"
       "** file were corrupted leading to the loss of one or  **\n\r"
       "** more of your possessions.          **\n\r"
       "********************************************************\n\r";


    ch          = new_character( TRUE );

//    ch->desc        = d;
    ch->name        = str_dup( name );
//    sprintf(pipe_buf, "|N %d %s \n", (int)d, ch->name);
//    write(fd0[1], pipe_buf, strlen(pipe_buf));
    ch->pcdata->prompt      = str_dup( daPrompt );
    ch->last_note     = 0;
    ch->act       = PLR_BLANK
          | PLR_COMBINE
          | PLR_PROMPT
          | PLR_AUTOEXIT
          | PLR_ASSIST
          //| PLR_REPLACE
          | PLR_CHINESE
          | PLR_AUTOGOLD
          | PLR_HORSE;
    ch->pcdata->pwd     = str_dup( "" );
    ch->pcdata->bamfin      = str_dup( "" );
    ch->pcdata->bamfout     = str_dup( "" );
    ch->pcdata->immskll     = str_dup( "" );
    ch->pcdata->title     = str_dup( "" );
    ch->perm_str    = 13;
    ch->perm_int    = 13; 
    ch->perm_wis    = 13;
    ch->perm_dex    = 13;
    ch->perm_con    = 13;
    ch->pcdata->condition[COND_THIRST]  = 48;
    ch->pcdata->condition[COND_FULL]  = 48;
    ch->pcdata->pagelen     = 40;
    ch->pcdata->security    = 0;  /* OLC */
  ch->pcdata->ride      = 0;
    ch->pcdata->switched    = FALSE;

    ch->pcdata->horse     = d->character;
  d->character->pcdata->horse = d->original;

  found = FALSE;
  fflush( NULL );
  if (fpReserve) {
    fclose( fpReserve );
    fpReserve = NULL;
  }

    /* parsed player file directories by Yaz of 4th Realm */
    /* decompress if .gz file exists - Thx Alander */
#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( strsave, "%s%s%s%s%s", HORSE_DIR, initial( ch->name ),
      "/", capitalize( name ), ".gz" );
    if ( ( fp = fopen( strsave, "r" ) ) )
    {
  char     buf     [ MAX_STRING_LENGTH ];

  fflush( NULL );
  fclose( fp );
  sprintf( buf, "gzip -dfq %s", strsave );
  system( buf );
    }
#endif

#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( strsave, "%s%s%s%s", HORSE_DIR, initial( ch->name ),
      "/", capitalize( name ) );
#else
    sprintf( strsave, "%s%s", HORSE_DIR, capitalize( name ) );
#endif
    if ( ( fp = fopen( strsave, "r" ) ) )
    {
  char buf[ MAX_STRING_LENGTH ];
  int iNest;

  for ( iNest = 0; iNest < MAX_NEST; iNest++ )
      rgObjNest[iNest] = NULL;

  found = TRUE;
  for ( ; ; )
  {
      char *word;
      char  letter;
      int   status;

      letter = fread_letter( fp );
      if ( letter == '*' )
      {
    fread_to_eol( fp );
    continue;
      }

      if ( letter != '#' )
      {
    bug( "Load_char_obj: # not found.", 0 );
    break;
      }

      word = fread_word( fp, &status );

      if(!word) {
    bug("load_char_obj error.",0);
    fread_to_eol(fp);
    break;
      }

      if ( !str_cmp( word, "PLAYER" ) )
      {
    if ( fread_char ( ch, fp ) )
    {
        sprintf( buf,
          "Load_char_obj:  %s section PLAYER corrupt.\n\r",
          name );
        bug( buf, 0 );
        write_to_buffer( d, sorry_player, 0 );

        SET_BIT( ch->act, PLR_DENY );
        return FALSE;
    }
      }
      else if ( !str_cmp( word, "OBJECT" ) )
      {
    if ( !fread_obj  ( ch, fp ) )
    {
        sprintf( buf,
          "Load_char_obj:  %s section OBJECT corrupt.\n\r",
          name );
        bug( buf, 0 );
        write_to_buffer( d, sorry_object, 0 );
        return FALSE;
    }
      }
      else if ( !str_cmp( word, "EQBANK_OBJECT" ) )
      {
    if ( !fread_bank_obj( ch, fp ) )
    {
        sprintf( buf,
          "Load_char_obj:  %s section OBJECT corrupt.\n\r",
          name );
        bug( buf, 0 );
        write_to_buffer( d, sorry_object, 0 );
        return FALSE;
    }
      }
      else if ( !str_cmp( word, "END"    ) ) break;
      else
      {
    bug( "Load_char_obj: bad section.", 0 );
    break;
      }
  } /* for */

  if( fp ) {
    fflush( fp );
      fclose( fp );
  }
    }

    fpReserve = fopen( NULL_FILE, "r" );
//    sprintf( buf, "%s horse", ch->name );
//    free_string( ch->name );
//    ch->name = str_dup( buf );
    ch->pcdata->ride      = 0;
    return ch;
}

void save_horse_obj( CHAR_DATA *ch )
{
    FILE *fp;
    char  buf   [ MAX_STRING_LENGTH ];
    char  strsave [ MAX_INPUT_LENGTH  ];

    if ( IS_NPC( ch ) || ( ch->level < 2 && ch->class <= BARBARIAN ) )
  return;
    if( !ch->pcdata->horse ) return;

    if ( ch->desc && ch->desc->original )
  ch = ch->desc->original;

    ch->save_time = current_time;
    fflush( NULL );
  if (fpReserve) {
    fclose( fpReserve );
    fpReserve = NULL;
  }

    /* player files parsed directories by Yaz 4th Realm */
#if !defined( macintosh ) && !defined( WIN32 )
    sprintf( strsave, "%s%s%s%s", HORSE_DIR, initial( ch->pcdata->horse->name ),
      "/", capitalize( ch->pcdata->horse->name ) );
#else
    sprintf( strsave, "%s%s", HORSE_DIR, capitalize( ch->pcdata->horse->name ) );
#endif
    if ( !( fp = fopen( strsave, "w" ) ) )
    {
  sprintf( buf, "Save_char_obj: fopen %s: ", ch->name );
  bug( buf, 0 );
  perror( strsave );
    }
    else
    {
  //ch->in_room = ch->master->in_room;
  fwrite_char( ch, fp );
  if ( ch->carrying )
      fwrite_obj( ch, ch->carrying, fp, 0 );
  if ( ch->eqbank )
    fwrite_bank_obj( ch, ch->eqbank, fp, 0 );
  fprintf( fp, "#END\n" );
    }
    if( fp ) {
    fflush( fp );
  fclose( fp );
  }
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}
