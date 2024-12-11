
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.			   *
 *									   *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and	   *
 *  Mitchell Tse.							   *
 *									   *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.	   *
 *									   *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in	   *
 *  'license.txt', as well as the Envy license in 'license.nvy'.	   *
 *  In particular, you may not remove either of these copyright notices.   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

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

#define __ROC_CHINESE_C__
#include "merc.h"
#include "ansi.h"


bool 
is_chinese_char(unsigned char c, bool cFlag)
{
	if( cFlag )
		return FALSE;

	if ( c > '\x7F') // always true: && c <= '\xff' )
		return TRUE;
	
	return FALSE;
}

bool is_brief(CHAR_DATA* ch, CHAR_DATA* victim)
{
    if( !ch ) return ( IS_NPC(victim) ? FALSE : IS_SET(ch->act, PLR_BRIEF));
    if( !victim ) {
        if( IS_NPC(ch) ) 
            return TRUE;
        else {
	          return (IS_SET(ch->act, PLR_BRIEF));
        }
    }
    if( IS_NPC(ch) ) {
        if( IS_NPC(victim) ) 
            return TRUE;
        else{ 
            return (IS_SET(victim->act, PLR_BRIEF));
        }
    }else{
        return (IS_SET(ch->act, PLR_BRIEF));
    }

    return FALSE;
}
bool is_chinese(CHAR_DATA* ch, CHAR_DATA* victim)
{
    if( !ch ) return TRUE;
    if( !victim ) {
	if( IS_NPC(ch) ) return TRUE;
	else {
	    return (IS_SET(ch->act, PLR_CHINESE)) ? TRUE : FALSE;
	}
    }
    if( IS_NPC(ch) ) {
	if( IS_NPC(victim) ) return TRUE;
	else{ 
	    return (IS_SET(victim->act, PLR_CHINESE)) ? TRUE : FALSE;
	}
    }
    else {
	return (IS_SET(ch->act, PLR_CHINESE)) ? TRUE : FALSE;
    }

    return TRUE;
}

void strip_ansi(char* src, char* dup)
{
    int i = 0;
    int j = 0;
    while(src[i] != '\0')
    {
	if( src[i] == '' )
	{
	    while(src[i] != 'm' )
	    {
		i++;	
	    }
	    i++;
	}
	else
	{
	    dup[j++] = src[i++];
	}
    }
    dup[j] = '\0';
}



/*
 * Return ascii name of an item type.
 */
char *c_item_type_name( OBJ_DATA *obj )
{
    OBJ_DATA *in_obj;
    char      buf [ MAX_STRING_LENGTH ];

    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return "¥ú·½";
    case ITEM_SCROLL:		return "¨÷¶b";
    case ITEM_WAND:		return "Å]§ú";
    case ITEM_STAFF:		return "ªk§ú";
    case ITEM_WEAPON:		return "ªZ¾¹";
    case ITEM_TREASURE:		return "Ä_ÂÃ";
    case ITEM_ARMOR:		return "¸Ë¥Ò";
    case ITEM_POTION:		return "ÃÄ¾¯";
    case ITEM_FURNITURE:	return "®a­Ñ";
    case ITEM_TRASH:		return "©U§£";
    case ITEM_CONTAINER:	return "®e¾¹";
    case ITEM_DRINK_CON:	return "¶¼¤ô®e¾¹";
    case ITEM_KEY:		return "Æ_°Í";
    case ITEM_FOOD:		return "­¹ª«";
    case ITEM_MONEY:		return "¿ú¹ô";
    case ITEM_BOAT:		return "²î°¦";
    case ITEM_CORPSE_NPC:	return "NPC«ÍÅé";
    case ITEM_CORPSE_PC:	return "ª±ªÌ«ÍÅé";
    case ITEM_FOUNTAIN:		return "¬u¤ô";
    case ITEM_PILL:		return "ÃÄ¤Y";
    case ITEM_ARROW:		return "¤}¥Ú";
    case ITEM_DART:		return "·t¾¹";
    case ITEM_HORSE:		return "§¤ÃM";
	case ITEM_AMMO:		return "¼uÃÄ"; // 2022/04/29
    }

    for ( in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj )
      ;

    if ( in_obj->carried_by )
      sprintf( buf, "Item_type_name: unknown type %d from %s owned by %s.",
	      obj->item_type, obj->name, obj->carried_by->name );
    else
      sprintf( buf,
	      "Item_type_name: unknown type %d from %s owned by <¤£©ú>.",
	      obj->item_type, obj->name );

    bug( buf, 0 );
    return "<¤£©ú>";
}



/*
 * Return ascii name of an affect location.
 */
char *c_affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:            return "µL";
    case APPLY_STR:             return "¤O¶q";
    case APPLY_DEX:             return "±Ó±¶";
    case APPLY_INT:             return "´¼¤O";
    case APPLY_WIS:             return "´¼¼z";
    case APPLY_CON:             return "Åé½è";
    case APPLY_SEX:             return "©Ê§O";
    case APPLY_CLASS:           return "Â¾·~";
    case APPLY_LEVEL:           return "¯Å§O";
    case APPLY_AGE:             return "¦~ÄÖ";
    case APPLY_HEIGHT:          return "Åé«¬";
    case APPLY_WEIGHT:          return "­t­«";
    case APPLY_MANA:            return "ªk¤O";
    case APPLY_HIT:             return "¥Í©R¤O";
    case APPLY_MOVE:            return "²¾°Ê¤O";
    case APPLY_GOLD:            return "ª÷¿ú";
    case APPLY_EXP:             return "¸gÅç­È";
    case APPLY_AC:              return "¨¾¿m¤O";
    case APPLY_HITROLL:         return "©R¤¤²v";
    case APPLY_DAMROLL:         return "¶Ë®`¤O";
    case APPLY_SAVING_PARA:     return "³Â·ô¦^Á×²v";
    case APPLY_SAVING_LIFE:     return "¥Í©R¦^Á×²v";    // ¥Î©ó mar ¤£¦º 2020/10/28
    case APPLY_SAVING_PETRI:    return "¥Û¤Æ¦^Á×²v";
    case APPLY_SAVING_BREATH:   return "Às®§¦^Á×²v";
    case APPLY_SAVING_SPELL:    return "ªk³N¦^Á×²v";
    case APPLY_RACE:            return "ºØ±Ú";
    case APPLY_POWER_POTENTIAL: return "Å]ªk¼ç¤O";
    case APPLY_DODGE:           return "°{Á×²v";
    case APPLY_RDAMAGE:         return "§Üª«²z¶Ë®`";
    case APPLY_RMAGIC:          return "§ÜÅ]ªk¶Ë®`";
    case APPLY_ADAMAGE:         return "ª«²z¶Ë®`¤O";
    case APPLY_AMAGIC:          return "Å]ªk¶Ë®`¤O";
    case APPLY_SAVING_ANCIENT:  return "¤W¥jªk³N¦^Á×²v";
    case APPLY_SAVING_SPIRIT:   return "ÆF»îªk³N¦^Á×²v";
    case APPLY_SAVING_FIRE:     return "¤õª¢ªk³N¦^Á×²v";
    case APPLY_SAVING_COLD:     return "´H§Nªk³N¦^Á×²v";
    case APPLY_SAVING_LIGHTNING:return "°{¹qªk³N¦^Á×²v";
    case APPLY_SAVING_EARTH:    return "¤j¦aªk³N¦^Á×²v";
    case APPLY_SAVING_WIND:     return "­·ªk³N¦^Á×²v";
    case APPLY_SAVING_SAINT:    return "¯«¸tªk³N¦^Á×²v";
    case APPLY_SAVING_DARK:     return "¶Â·tªk³N¦^Á×²v";
    case APPLY_SAVING_POISON:   return "¼@¬r¶Ë®`";
    case APPLY_SAVING_ELEMENT:  return "¤¸¯À¦^Á×²v";
    case APPLY_SAVING_GOOD:     return "µ½¨}ªk³N¦^Á×²v";
    case APPLY_SAVING_EVIL:     return "¨¸´cªk³N¦^Á×²v";
    case APPLY_DOB_SAVING_FLASH:return "¥úÃz©è§Ü¤O";
    case APPLY_SAVING_ILLUSION: return "¤Û³N¦^Á×²v";
    case APPLY_ACTION_ABILITY:  return "¦æ°Ê¤O";
	case APPLY_WEAPON_SPELL:    return "ªk³NÄÝ©Ê¶Ë®`"; // ªZ¾¹ÃB¥~°l¥[ªk³N«Â¤O 2021/09/19
	case APPLY_WEAPON_WIND:     return "­·ÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[­·¨t«Â¤O
	case APPLY_WEAPON_FIRE:     return "¤õÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[¤õ¨t«Â¤O
	case APPLY_WEAPON_COLD:     return "¤ôÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[¤ô¨t«Â¤O
	case APPLY_WEAPON_EARTH:    return "¦aÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[¦a¨t«Â¤O
	case APPLY_WEAPON_LIGHTNING:return "¹pÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[¹p¨t«Â¤O
	case APPLY_WEAPON_SAINT:    return "¸tÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[¸t¨t«Â¤O
	case APPLY_WEAPON_DARK:     return "·tÄÝ©Ê¶Ë®`";   // ªZ¾¹ÃB¥~°l¥[·t¨t«Â¤O
	case APPLY_MAGIC_IMMUNE:    return "Å]ªk´î§K";     // ¥Î©ó mystic armor 2021/11/03
	case APPLY_TOXICITY_POISON: return "¬r©Ê±j«×";     // ¥Î©ó poison item ­pºâ 2021/11/10
	case APPLY_DAMAGE_COUNT:    return "²Ö­p¶Ë®`­È";   // ¥Î©ó blood thirsty ­pºâ 2022/02/21
	case APPLY_SHOOT_HITROLL:   return "®gÀ»©R¤¤²v";     // ½b¥Ú©Îºj¼uµ¥ª««~³Q®gÀ»®Éªº©R¤¤¥[¦¨ 2022/05/06
	case APPLY_SHOOT_DAMROLL:   return "®gÀ»¶Ë®`¤O";     // ½b¥Ú©Îºj¼uµ¥ª««~³Q®gÀ»®Éªº¶Ë®`¥[¦¨ 2022/05/06
	case APPLY_SHOOT_ADAMAGE:   return "®gÀ»ª«²z¶Ë®`¤O"; // ½b¥Ú©Îºj¼uµ¥ª««~³Q®gÀ»®Éªºª«²z¶Ë®`¥[¦¨ 2022/05/06
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "<¤£©ú>";
}


/*
 * Return ascii name of an affect bit vector.
 */
char *c_affect_bit_name( long long int vector )
{
    static char buf [ 1024 ];

    buf[0] = '\0';
    if ( vector & AFF_BLIND )           strcat( buf, " ª¼¥Ø" );
    if ( vector & AFF_INVISIBLE )       strcat( buf, " Áô§Î" );
    if ( vector & AFF_DETECT_EVIL )     strcat( buf, " °»´ú¨¸´c" );
    if ( vector & AFF_DETECT_INVIS )    strcat( buf, " °»´úÁô§Î" );
    if ( vector & AFF_DETECT_MAGIC )    strcat( buf, " °»´úÅ]ªk" );
    if ( vector & AFF_DETECT_HIDDEN )   strcat( buf, " °»´úÂÃ°Î" );
    if ( vector & AFF_HOLD )            strcat( buf, " HOLD" );
    if ( vector & AFF_SANCTUARY )       strcat( buf, " ¸t¥ú" );
    if ( vector & AFF_FAERIE_FIRE )     strcat( buf, " ¤Ñ¨Ï¤§¤õ" );
    if ( vector & AFF_INFRARED )        strcat( buf, " ©]µø" );
    if ( vector & AFF_CURSE )           strcat( buf, " ¶A©G" );
    if ( vector & AFF_TARDY )           strcat( buf, " ¿ð½w" );
    if ( vector & AFF_CHANGE_SEX )      strcat( buf, " ©Ê§OÅÜ§ó" );
    if ( vector & AFF_POISON )          strcat( buf, " ¤¤¬r" );
    if ( vector & AFF_PROTECT )         strcat( buf, " ¤Ñ¨Ï¥[Å@" );
    if ( vector & AFF_POLYMORPH )       strcat( buf, " ºØ±ÚÅÜ§ó" );
    if ( vector & AFF_SLEEP )           strcat( buf, " ¨IºÎ" );
    if ( vector & AFF_SNEAK )           strcat( buf, " ¼ç¦æ" );
    if ( vector & AFF_HIDE )            strcat( buf, " ÂÃ°Î" );
    if ( vector & AFF_CHARM )           strcat( buf, " °g¥¢" );
    if ( vector & AFF_FLYING )          strcat( buf, " ­¸¦æ" );
    if ( vector & AFF_WATERWALK)        strcat( buf, " ¤ô­±¦æ¨«" );
    if ( vector & AFF_PASS_DOOR )       strcat( buf, " ¥b³z©ú" );
    if ( vector & AFF_MUTE )            strcat( buf, " ¨IÀq" );
    if ( vector & AFF_SEAL_MAGIC )      strcat( buf, " «Ê©G" );
	// if ( vector & AFF_GILLS )        strcat( buf, " gills" );
    if ( vector & AFF_VAMP_BITE )       strcat( buf, " §l¦å°­¤§§k" );
    if ( vector & AFF_GHOUL )           strcat( buf, " ¤£¦º±Ú¶A©G" ); // 2022/01/16
    if ( vector & AFF_FLAMING )         strcat( buf, " ª¢¾À" );
    if ( vector & AFF_PARALYZED )       strcat( buf, " ³Â·ô" );
    if ( vector & AFF_CRAZY )           strcat( buf, " ºÆ¨g" );
    if ( vector & AFF_WEAKEN )          strcat( buf, " °I®z" );
    if ( vector & AFF_NIGHTSTALK )      strcat( buf, " ·t©]°ÎÂÜ" );
	if ( vector & AFF_DISEASE )         strcat( buf, " ¥Í¯f" );
	if ( vector & AFF_FEAR )            strcat( buf, " ®£Äß" );
    return ( buf[0] != '\0' ) ? buf+1 : "µL";
}



/*
 * Return ascii name of extra flags vector.
 */
char *c_extra_bit_name( int extra_flags )
{
    static char buf [ 1024 ];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW	 ) strcat( buf, " µo¥ú"		);
    if ( extra_flags & ITEM_HUM		 ) strcat( buf, " §C»ï"		);
    if ( extra_flags & ITEM_DARK	 ) strcat( buf, " îî¶Â"		);
    if ( extra_flags & ITEM_LOCK	 ) strcat( buf, " ¤WÂê"		);
    if ( extra_flags & ITEM_EVIL	 ) strcat( buf, " ¨¸´c"		);
    if ( extra_flags & ITEM_INVIS	 ) strcat( buf, " Áô§Î"	);
    if ( extra_flags & ITEM_MAGIC	 ) strcat( buf, " Å]¤O"	);
    if ( extra_flags & ITEM_NODROP	 ) strcat( buf, " µLªk¥á±ó"	);
    if ( extra_flags & ITEM_BLESS	 ) strcat( buf, " ¯¬ºÖ"	);
    if ( extra_flags & ITEM_ANTI_GOOD	 ) strcat( buf, " ¤Ïµ½¨}"	);
    if ( extra_flags & ITEM_ANTI_EVIL	 ) strcat( buf, " ¤Ï¨¸´c"	);
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " ¤Ï¤¤¥ß" );
    if ( extra_flags & ITEM_NOREMOVE	 ) strcat( buf, " µLªk²¾°£"	);
    if ( extra_flags & ITEM_INVENTORY	 ) strcat( buf, " ª««~"	);
    if ( extra_flags & ITEM_POISONED	 ) strcat( buf, " ¼@¬r"	);
    if ( extra_flags & ITEM_VAMPIRE_BANE ) strcat( buf, " BANE" );
    if ( extra_flags & ITEM_HOLY	 ) strcat( buf, " ¯«¸t"		);
    if ( extra_flags & ITEM_NOSAVE) strcat( buf, " µLªkÀx¦s"		);
    if ( extra_flags & ITEM_UNIQUE) strcat( buf, " ¿W¤@µL¤G"		);
    if ( extra_flags & ITEM_LORE) strcat( buf, " ³æ¤@"		);
    if ( extra_flags & ITEM_NOSAC) strcat( buf, " µLªk©^Äm"		);
    if ( extra_flags & ITEM_PERSONAL) strcat( buf, " ­Ó¤H±M¥Î"		);
    if ( extra_flags & ITEM_RECOVER) strcat( buf, " ·´·l­×´_"		);
    if ( extra_flags & ITEM_NO_MALE) strcat( buf, " ¨k©Ê¸T¥Î"		);
    if ( extra_flags & ITEM_NO_FEMALE) strcat( buf, " ¤k©Ê¸T¥Î"		);
    if ( extra_flags & ITEM_NO_NEUTRAL) strcat( buf, " ¤¤©Ê¸T¥Î"		);
	if ( extra_flags & ITEM_SPECIAL) strcat( buf, " Â¾·~­­¨î"	);
	if ( extra_flags & ITEM_SPELL) strcat( buf, " ªþÅ]"	); // ITEM_SPELL add at 2021/09/19
//	if ( extra_flags & ITEM_MALE) strcat( buf, "¨k©Ê±M¥Î"		);
//	if ( extra_flags & ITEM_FEMALE) strcat( buf, "¤k©Ê±M¥Î"		);
//	if ( extra_flags & ITEM_NEUTRAL) strcat( buf, "¤¤©Ê±M¥Î"	);
    return ( buf[0] != '\0' ) ? buf+1 : "µL";
}


char *	const			c_title_table [ MAX_CLASS ][ MAX_LEVEL+1 ][ 2 ] =
{
    {
	{ "Man",			"Woman"				},

	{ "Å]ªk¾Ç®{",	"Å]ªk¾Ç®{"		},
	{ "Å]ªk¾Ç®{",		"Å]ªk¾Ç®{"			},
	{ "Å]ªk¾Ç²ßªÌ",		"Å]ªk¾Ç²ßªÌ"		},
	{ "Å]ªk¬ã¨sªÌ",		"Å]ªk¬ã¨sªÌ"		},
	{ "Å]ªkÆF´C",		"Å]ªkÆF´C"		},

	{ "Å]ªk§Û¼g­û",		"Å]ªk§Û¼g­û"		},
	{ "¥ýª¾",			"¤k¥ýª¾"			},
	{ "´¼ªÌ",			"´¼ªÌ"				},
	{ "¤Û³N¤h",		"¤Û³N¤h"			},
	{ "©G³N¤h",			"©G³N¤h"			},

	{ "­°³N¤h",			"­°³N¤h"			},
	{ "Å]¾É¤h",			"Å]¾É¤h"			},
	{ "¥l³ê¤h",			"¥l³ê¤h"			},
	{ "Å]³N®v",			"Å]³N®v"				},
	{ "³Ð³yªÌ",			"³Ð³yªÌ"			},

	{ "½åªÌ",			"½åªÌ"			},
	{ "ªk®v",			"¤kªk®v"			},
	{ "§Å®v",			"¤k§Å"			},
	{ "¤jªk®v",			"¤jªk®v"			},
	{ "Å]ªk®v",			"Å]ªk®v"			},

	{ "Å]ªkªø¦Ñ",		"Å]ªkªø¦Ñ"		},
	{ "°ª¶¥Å]ªkªø¦Ñ",		"°ª¶¥Å]ªkªø¦Ñ"		},
	{ "¤jÅ]ªk®v",		"¤jÅ]ªk®v"		},
	{ "¥¨¹³»s³yªÌ",		"¥¨¹³»s³yªÌ"			},
	{ "¥¨¹³»s³y¤j®v",	"¥¨¹³»s³y¤j®v"		},

	{ "Å]¥Û»s³yªÌ",		"Å]¥Û»s³yªÌ",		},
	{ "Å]ÃÄ»s³yªÌ",		"Å]ÃÄ»s³yªÌ",		},
	{ "¨÷¶b»s³yªÌ",		"¨÷¶b»s³yªÌ",		},
	{ "ªk§ú»s³yªÌ",		"ªk§ú»s³yªÌ",		},
	{ "Åv§ú»s³yªÌ",		"Åv§ú»s³yªÌ",		},

	{ "´cÅ]¥l³ê¤h",		"´cÅ]¥l³ê¤h"		},
	{ "´cÅ]¥l³ê®v",	"´cÅ]¥l³ê®v"	},
	{ "¾yÀs³N¤h",		"¾yÀs³N¤h"		},
	{ "¾yÀs³N®v",	"¾yÀs³N®v"	},
	{ "Å]ªk¤j®v",	"Å]ªk¤j®v"		},

	{ "Å]ªk¤j®v",	"Å]ªk¤j®v"		},
	{ "Å]ªk¤j®v",	"Å]ªk¤j®v"		},
	{ "Å]ªk¤j®v",	"Å]ªk¤j®v"		},
	{ "ªk®v­^¶¯",			"¤kªk®v­^¶¯"			},
	{ "ªk®v­^¶¯",			"¤kªk®v­^¶¯"			},

	{ "ªk®v­^¶¯",			"¤kªk®v­^¶¯"			},
	{ "ªk®v­^¶¯",			"¤kªk®v­^¶¯"			},
	{ "Å]ªk¤Ñ¨Ï",		"Å]ªk¤Ñ¨Ï"		},
	{ "Å]ªk¤§¯«",		"Å]ªk¤§¯«"		},

	{ "Å]ªk¤j¯«",		"Å]ªk¤j¯«"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "«H®{",			"¤k«H®{"			},
	{ "¨Í±q",			"¨Í±q"			},
	{ "¨Í¹¬",			"¨Í¹¬"			},
	{ "·s¶i±Ð®{",			"·s¶i±Ð®{"			},
	{ "¶Ç±Ð¤h",			"¶Ç±Ð¤h"			},

	{ "±Ð¤hªø",			"±Ð¤hªø"				},
	{ "°õ¨Æ",			"¤k°õ¨Æ"			},
	{ "¥q±Ð",			"¥q±Ð"			},
	{ "²½¥q",			"¤k²½¥q"			},
	{ "ªª®v",			"ªª®v"			},

	{ "°Ñ¨Æ",			"°Ñ¨Æ"				},
	{ "°Ñ¨Æªø",			"°Ñ¨Æªø"			},
	{ "»²²½",			"»²²½"			},
	{ "­×¤h",			"­×¤k"				},
	{ "ÂåªÌ",			"¤kÂåªÌ"			},

	{ "°ª¶¥ªª®v",			"°ª¶¥ªª®v"			},
	{ "ÄÄ¹DªÌ",			"ÄÄ¹DªÌ"			},
	{ "¥D±Ð",			"¥D±Ð"			},
	{ "¤j¥D±Ð",		"¤j¥D±Ð"	},
	{ "Á`¥D±Ð",			"Á`¥D±Ð"			},

	{ "¥D±Ðªø¦Ñ",			"¥D±Ðªø¦Ñ"		},
	{ "¤j¥D±Ðªø¦Ñ",			"¤j¥D±Ðªø¦Ñ"		},
	{ "Á`¥D±Ðªø¦Ñ",			"Á`¥D±Ðªø¦Ñ"		},
	{ "°£Å]ªÌ",		"°£Å]ªÌ"			},
	{ "°£Å]¸tªÌ",	"°£Å]¸tªÌ"		},

	{ "®ü¶¥¼Ï¾÷¥D±Ð",	"®ü¶¥¼Ï¾÷¥D±Ð"		},
	{ "¤j¦a¶¥¼Ï¾÷¥D±Ð",	"¤j¦a¶¥¼Ï¾÷¥D±Ð"		},
	{ "­·¶¥¼Ï¾÷¥D±Ð",	"­·¶¥¼Ï¾÷¥D±Ð"		},
	{ "¤A¤Ó¶¥¼Ï¾÷¥D±Ð",	"¤A¤Ó¶¥¼Ï¾÷¥D±Ð"		},
	{ "¤Ñ¦ì¼Ï¾÷¥D±Ð",	"¤Ñ¦ì¼Ï¾÷¥D±Ð"	},

	{ "¤£¦´¸tªÌ",	"¤£¦´¸tªÌ"		},
	{ "¯«¸t¤Æ¨­",		"¯«¸t¤Æ¨­"		},
	{ "¯«¸t¤Æ¨­",		"¯«¸t¤Æ¨­"		},
	{ "¤Ñ¯«­°¥@",	"¤Ñ¯«­°¥@"	},
	{ "¯«¸tÅ]ªk¤j®v",	"¯«¸tÅ]ªk¤j®v"	},

	{ "¯«¸tÅ]ªk¤j®v",	"¯«¸tÅ]ªk¤j®v"	},
	{ "¯«¸tÅ]ªk¤j®v",	"¯«¸tÅ]ªk¤j®v"	},
	{ "¯«¸tÅ]ªk¤j®v",	"¯«¸tÅ]ªk¤j®v"	},
	{ "ªª®v­^¶¯",			"ªª®v¤k­^¶¯"			},
	{ "ªª®v­^¶¯",			"ªª®v¤k­^¶¯"			},

	{ "ªª®v­^¶¯",			"ªª®v¤k­^¶¯"			},
	{ "ªª®v­^¶¯",			"ªª®v¤k­^¶¯"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "¤ò¸é",			"¤ò¸é"			},
	{ "Äd¸ôµs",			"Äd¸ôµs"			},
	{ "¤p°½",			"¤k¤p°½"			},
	{ "¥¶¤â",		"¥¶¤â"			},
	{ "µs¸é",			"µs¸é"				},

	{ "ÅÑ¸é",			"ÅÑ¸é"			},
	{ "°ª¯Å¥¶¤â",			"°ª¯Å¥¶¤â"			},
	{ "±jµs",			"±jµs"			},
	{ "ÄF¤l",			"¤kÄF¤l"			},
	{ "¬yª]",			"¬yª]"			},
	{ "´c´Ò",			"´c´Ò"				},

	{ "µs­ê",			"µs­ê"			},
	{ "°¨¸é",			"°¨¸é"			},
	{ "©]¦æµs",			"©]¦æµs"			},
	{ "§®¸é",			"§®¸é"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "¥û¤â",			"¥û¤â"			},
	{ "¤s¸é",			"¤s¸é"			},
	{ "¤jµs",			"¤jµs"			},

	{ "¶¡¿Ò",			"¶¡¿Ò"				},
	{ "¦Ñ¤â¶¡¿Ò",			"¦Ñ¤â¶¡¿Ò"			},
	{ "¶¡¿Ò¤j®v",			"¶¡¿Ò¤j®v"			},
	{ "¨ë«È",			"¨ë«È"			},
	{ "°ª¯Å¨ë«È",			"°ª¯Å¨ë«È"			},

	{ "¿sµø±M®a",		"¿sµø±M®a"		},
	{ "ÅÑÅ¥±M®a",		"ÅÑÅ¥±M®a"		},
	{ "¬}¹î±M®a",		"¬}¹î±M®a"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "¥Ç¸o°ª¤â",	"¥Ç¸o°ª¤â"	},
	{ "¥Ç¸o±M®a",	"¥Ç¸o±M®a"	},
	{ "¥Ç¸o¤j®v",	"¥Ç¸o¤j®v"	},
	{ "¥Ç¸o¤§¤ý",			"¥Ç¸o¤§¦Z"		},
	{ "¶Âµó±Ð¤÷",			"¶Âµó±Ð¥À"			},
	{ "¶Âµó±Ð¤÷",			"¶Âµó±Ð¥À"			},
	{ "¶Âµó±Ð¤÷",			"¶Âµó±Ð¥À"			},
	{ "¶Âµó±Ð¤÷",			"¶Âµó±Ð¥À"			},

	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "¼C³N¨£²ß¥Í",			"¼C³N¨£²ß¥Í"			},
	{ "·s§L",			"·s§L"			},
	{ "¨B¨ò",			"¨B¨ò"			},
	{ "¾Ô¤h",			"¤k¾Ô¤h"			},
	{ "¤h§L",			"¤k§L"			},

	{ "ªZ¤h",			"¤kªZ¤h"			},
	{ "¦Ñ§L",			"¦Ñ§L"			},
	{ "¼C¤â",			"¼C¤â"			},
	{ "À»¼CªÌ",			"À»¼CªÌ"			},
	{ "°«¤h",			"¤k°«¤h"			},

	{ "­^¶¯",			"¤k­^¶¯"			},
	{ "«i¤h",			"¤k«i¤h"			},
	{ "¨g¼C¤h",		"¨g¼C¤h"		},
	{ "¶Ä§L",			"¤k¶Ä§L"			},
	{ "¼C®v",		"¤k¼C®v"			},

	{ "°Æ­º»â",			"°Æ­º»â"			},
	{ "­º»â",			"¤k­º»â"			},
	{ "ÀsÃM§L",			"ÀsÃM§L"			},
	{ "ÀsÃM±N",			"ÀsÃM±N"			},
	{ "¥¿ÃM¤h",			"¤kÃM¤h"			},

	{ "¤W¦ìÃM¤h",		"¤W¦ìÃM¤h"			},
	{ "ÃM¤hªø",		"ÃM¤hªø"			},
	{ "¸t·µÃM¤h",			"¸t·µÃM¤h"			},
	{ "¸t·µ¸T½Ã",		"¸t·µ¸T½Ã"			},
	{ "°£Å]ªÌ",		"°£Å]ªÌ"			},

	{ "¤W¦ì°£Å]ªÌ",	"¤W¦ì°£Å]ªÌ"		},
	{ "±OÀsªÌ",		"±OÀsªÌ"			},
	{ "±OÀs«iªÌ",	"±OÀs«iªÌ"		},
	{ "¾Ô±N",			"¾Ô±N"			},
	{ "±s«Ê¾Ô±N",			"±s«Ê¾Ô±N"			},

	{ "¹p»ï¨kÀï",		"¹p»ï¤kÀï"		},
	{ "­·¼É¤lÀï",		"­·¼É¤k¤lÀï"		},
	{ "¨g­·§BÀï",		"¨g­·¤k§BÀï"		},
	{ "Áü­·«JÀï",	"Áü­·¤k«JÀï"	},
	{ "¬y¬P¤½Àï",		"¬y¬P¤k¤½Àï"		},

	{ "ªF¯î¨F¤ýÀï","ªF¯î¨F¤k¤ýÀï"		},
	{ "¦è®üÀÜ¤ýÀï","¦è®üÀÜ¤k¤ýÀï"	},
	{ "¥_®mÀ­¤ýÀï","¥_®mÀ­¤k¤ýÀï"		},
	{ "«n·ÏªL¤ýÀï",	"«n·ÏªL¤k¤ýÀï"		},
	//{ "Knight of the Black Rose",	"Baroness of Meteors"		},

//	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
	//{ "Knight of the White Rose",	"Baroness of Meteors"		},
	{ "­^¶¯¾Ô¤h",   "­^¶¯¤k¾Ô¤h"},
	{ "­^¶¯¾Ô¤h",   "­^¶¯¤k¾Ô¤h"},
	{ "­^¶¯¾Ô¤h",   "­^¶¯¤k¾Ô¤h"},
	/*{ "Knight of the Silver Moon",	"Baroness of Meteors"		},
	{ "Knight of the Golden Sun",	"Baroness of Meteors"		},
	{ "Squire of the Realm",	"Baroness of Meteors"		},

	{ "Herald of the Realm",	"Baroness of Meteors"		},
	{ "Knight of the Realm",	"Baroness of Meteors"		},
	{ "Lord of the Realm",		"Baroness of Meteors"		},
	{ "Highlord of the Realm",	"Baroness of Meteors"		},
	{ "Knight Hero",		"Knight Heroine"		},
*/
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "¼C³N¨£²ß¥Í",			"¼C³N¨£²ß¥Í"			},
	{ "·s§L",			"·s§L"			},
	{ "¨B¨ò",			"¨B¨ò"			},
	{ "¾Ô¤h",			"¤k¾Ô¤h"			},
	{ "¤h§L",			"¤k§L"			},

	{ "ªZ¤h",			"¤kªZ¤h"			},
	{ "¦Ñ§L",			"¦Ñ§L"			},
	{ "¼C¤â",			"¼C¤â"			},
	{ "À»¼CªÌ",			"À»¼CªÌ"			},
	{ "°«¤h",			"¤k°«¤h"			},

	{ "­^¶¯",			"¤k­^¶¯"			},
	{ "«i¤h",			"¤k«i¤h"			},
	{ "¨g¼C¤h",		"¨g¼C¤h"		},
	{ "¶Ä§L",			"¤k¶Ä§L"			},
	{ "¼C®v",		"¤k¼C®v"			},

	{ "°Æ­º»â",			"°Æ­º»â"			},
	{ "­º»â",			"¤k­º»â"			},
	{ "ÀsÃM§L",			"ÀsÃM§L"			},
	{ "ÀsÃM±N",			"ÀsÃM±N"			},
	{ "¥¿ÃM¤h",			"¤kÃM¤h"			},

	{ "¤W¦ìÃM¤h",		"¤W¦ìÃM¤h"			},
	{ "ÃM¤hªø",		"ÃM¤hªø"			},
	{ "¸t·µÃM¤h",			"¸t·µÃM¤h"			},
	{ "¸t·µ¸T½Ã",		"¸t·µ¸T½Ã"			},
	{ "°£Å]ªÌ",		"°£Å]ªÌ"			},

	{ "¤W¦ì°£Å]ªÌ",	"¤W¦ì°£Å]ªÌ"		},
	{ "±OÀsªÌ",		"±OÀsªÌ"			},
	{ "±OÀs«iªÌ",	"±OÀs«iªÌ"		},
	{ "¾Ô±N",			"¾Ô±N"			},
	{ "±s«Ê¾Ô±N",			"±s«Ê¾Ô±N"			},

	{ "¹p»ï¨kÀï",		"¹p»ï¤kÀï"		},
	{ "­·¼É¤lÀï",		"­·¼É¤k¤lÀï"		},
	{ "¨g­·§BÀï",		"¨g­·¤k§BÀï"		},
	{ "Áü­·«JÀï",	"Áü­·¤k«JÀï"	},
	{ "¬y¬P¤½Àï",		"¬y¬P¤k¤½Àï"		},

	{ "ªF¯î¨F¤ýÀï","ªF¯î¨F¤k¤ýÀï"		},
	{ "¦è®üÀÜ¤ýÀï","¦è®üÀÜ¤k¤ýÀï"	},
	{ "¥_®mÀ­¤ýÀï","¥_®mÀ­¤k¤ýÀï"		},
	{ "«n·ÏªL¤ýÀï",	"«n·ÏªL¤k¤ýÀï"		},
	//{ "Knight of the Black Rose",	"Baroness of Meteors"		},

//	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
	//{ "Knight of the White Rose",	"Baroness of Meteors"		},
	{ "­^¶¯ÆZ«L",   "­^¶¯¤kÆZ«L"},
	{ "­^¶¯ÆZ«L",   "­^¶¯¤kÆZ«L"},
	{ "­^¶¯ÆZ«L",   "­^¶¯¤kÆZ«L"},
	/*{ "Knight of the Silver Moon",	"Baroness of Meteors"		},
	{ "Knight of the Golden Sun",	"Baroness of Meteors"		},
	{ "Squire of the Realm",	"Baroness of Meteors"		},

	{ "Herald of the Realm",	"Baroness of Meteors"		},
	{ "Knight of the Realm",	"Baroness of Meteors"		},
	{ "Lord of the Realm",		"Baroness of Meteors"		},
	{ "Highlord of the Realm",	"Baroness of Meteors"		},
	{ "Knight Hero",		"Knight Heroine"		},
*/
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Psychic",			"Psychic",			},
	{ "Medium",			"Medium",			},
	{ "Gypsy",			"Gypsy",			},
	{ "Meditator",			"Meditator",			},
	{ "Mind Prober",		"Mind Prober",			},

	{ "Soul Searcher",		"Soul Searcher",		},
	{ "Astral Voyager",		"Astral Voyager",		},
	{ "Seeker",			"Seeker",			},
	{ "Empath",			"Empath",			},
	{ "Mind Reader",		"Mind Reader"			},

	{ "Telepath",			"Telepath",			},
	{ "Mental Adept",		"Mental Adept",			},
	{ "Spoonbender",		"Spoonbender",			},
	{ "Perceptive",			"Perceptive",			},
	{ "Clever",			"Clever",			},

	{ "Wise",			"Wise",				},
	{ "Genius",			"Genius",			},
	{ "Oracle",			"Oracle",			},
	{ "Soothsayer",			"Soothsayer",			},
	{ "Truthteller",		"Truthteller",			},

	{ "Sage",			"Sage",				},
	{ "Master Psychic",		"Mistress Psychic",		},
	{ "Master Meditator",		"Mistress Meditator",		},
	{ "Master Empath",		"Mistress Empath",		},
	{ "Master Clairvoyant",		"Mistress Clairvoyant",		},

	{ "Master Mind Reader",		"Mistress Mind Reader",		},
	{ "Master Telepath",		"Mistress Telepath",		},
	{ "Master Spoonbender",		"Mistress Spoonbender",		},
	{ "Grand Master Psychic",	"Grand Mistress Psychic",	},
	{ "Grand Master Meditator",	"Grand Mistress Meditator",	},

	{ "Grand Master Empath",	"Grand Mistress Empath",	},
	{ "Grand Master Clairvoyant",	"Grand Mistress Clairvoyant",	},
	{ "Grand Master Mind Reader",	"Grand Mistress Mind Reader",	},
	{ "Grand Master Telepath",	"Grand Mistress Telepath",	},
	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},

	{ "¤jªk®v",	"¤k¤jªk®v",	},
	{ "¤Ñ¤~¤jªk®v",	"±öÂL©ÔÄ~ªÌ",	},
	{ "§Å®v",		"¤k§Å®v",		},
	{ "¤j§Å®v",		"¤k¤j§Å®v",		},
	{ "¾Ô§Å",		"¤k¾Ô§Å",		},
	{ "«Ò°ê¾Ô§Å",		"±öªk©ÔÄ~ªÌ",		},
	{ "¸Uª«¤Þ¾ÉªÌ",		"¸Uª«¤Þ¾ÉªÌ",		},

	{ "Psionicist Angel",		"Psionicist Angel"		},
	{ "Deity of Psionics",		"Deity of Psionics"		},

	{ "Supreme Psionicst",		"Supreme Psionicist"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Psychic",			"Psychic",			},
	{ "Medium",			"Medium",			},
	{ "Gypsy",			"Gypsy",			},
	{ "Meditator",			"Meditator",			},
	{ "Mind Prober",		"Mind Prober",			},

	{ "Soul Searcher",		"Soul Searcher",		},
	{ "Astral Voyager",		"Astral Voyager",		},
	{ "Seeker",			"Seeker",			},
	{ "Empath",			"Empath",			},
	{ "Mind Reader",		"Mind Reader"			},

	{ "Telepath",			"Telepath",			},
	{ "Mental Adept",		"Mental Adept",			},
	{ "Spoonbender",		"Spoonbender",			},
	{ "Perceptive",			"Perceptive",			},
	{ "Clever",			"Clever",			},

	{ "Wise",			"Wise",				},
	{ "Genius",			"Genius",			},
	{ "Oracle",			"Oracle",			},
	{ "Soothsayer",			"Soothsayer",			},
	{ "Truthteller",		"Truthteller",			},

	{ "Sage",			"Sage",				},
	{ "Master Psychic",		"Mistress Psychic",		},
	{ "Master Meditator",		"Mistress Meditator",		},
	{ "Master Empath",		"Mistress Empath",		},
	{ "Master Clairvoyant",		"Mistress Clairvoyant",		},

	{ "Master Mind Reader",		"Mistress Mind Reader",		},
	{ "Master Telepath",		"Mistress Telepath",		},
	{ "Master Spoonbender",		"Mistress Spoonbender",		},
	{ "Grand Master Psychic",	"Grand Mistress Psychic",	},
	{ "Grand Master Meditator",	"Grand Mistress Meditator",	},

	{ "Grand Master Empath",	"Grand Mistress Empath",	},
	{ "Grand Master Clairvoyant",	"Grand Mistress Clairvoyant",	},
	{ "Grand Master Mind Reader",	"Grand Mistress Mind Reader",	},
	{ "Grand Master Telepath",	"Grand Mistress Telepath",	},
	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},

	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},
	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "³Ð³yªÌ",		"³Ð³yªÌ",		},
	{ "Psionicist Angel",		"Psionicist Angel"		},
	{ "Deity of Psionics",		"Deity of Psionics"		},

	{ "Supreme Psionicst",		"Supreme Psionicist"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "¨k¤H",			"¤k¤H"				},

	{ "·M¥Á",			"·M¥Á"			},
	{ "¨Í±q",			"¨Í±q"			},
	{ "¨FÀ±",			"¤k¨FÀ±"			},
	{ "·s§L",			"¤k§L"			},
	{ "¶Ç±Ð¤h",			"¤k¶Ç±Ð¤h"			},

	{ "°ª¤â",			"¤k°ª¤â"				},
	{ "°Æ¥q²½",			"¤k°Æ¥q²½"			},
	{ "¥N²zªª®v",			"¥N²zªª®v"			},
	{ "ªª®v",			"¤kªª®v"			},
	{ "¦í«ù",			"¤k¦í«ù"			},

	{ "±Ð·|°Ñ¨Æ",			"±Ð·|°Ñ¨Æ"				},
	{ "¯«·µ°Æ¥D²½",			"¯«·µ¤k°Æ¥D²½"			},
	{ "§U²z¯«¤÷",			"§U²z¯«¥À"			},
	{ "¶Â¦ç­×¤h",			"¬õ¦ç­×¤k"				},
	{ "ÂåªÌ",			"¤kÂåªÌ"			},

	{ "ÀH­xªª®v",			"ÀH­x¤kªª®v"			},
	{ "¸Ñ¹DªÌ",			"¤k¸Ñ¹DªÌ"			},
	{ "¥D±Ð",			"¥D±Ð"			},
	{ "°ª¶¥¥D±Ð",		"¤k°ª¶¥¥D±Ð"	},
	{ "¤j¥D±Ð",			"¤k¤j¥D±Ð"			},

	{ "¬õ¦ç¤j¥D±Ð",		"¬õ¦ç¤k¤j¥D±Ð"		},
	{ "±Ð¥D",		"¤k±Ð¥D"		},
	{ "¤j±Ð¥D",		"¤k¤j±Ð¥D"		},
	{ "´cÅ]±þ¤â",		"´cÅ]±þ¤â"			},
	{ "¤j´cÅ]±þ¤â",	"¤j´cÅ]±þ¤â"		},

	{ "®ü¬v¼Ï¾÷¥D±Ð",	"®ü¬v¼Ï¾÷¥D±Ð"		},
	{ "¤j¦a¼Ï¾÷¥D±Ð",	"¤j¦a¼Ï¾÷¥D±Ð"		},
	{ "¤j®ð¼Ï¾÷¥D±Ð",	"¤j®ð¼Ï¾÷¥D±Ð"		},
	{ "¥H¤Ó¼Ï¾÷¥D±Ð",	"¥H¤Ó¼Ï¾÷¥D±Ð"		},
	{ "¤Ñ°ó¼Ï¾÷¥D±Ð",	"¤Ñ°ó¼Ï¾÷¥D±Ð"	},

	{ "¤£¦´ªÌ",	"¤£¦´ªÌ"		},
	{ "¯Â²b¤£¦´ªÌ",		"¯Â²b¤£¦´ªÌ"		},
	{ "¸tªÌ",	"¸tªÌ"		},
	{ "³yª«¸tªÌ",	"³yª«¸tªÌ"	},
	{ "¯Â²b¸tªÌ",	"¯Â²b¸tªÌ"	},

	{ "¥úºa¸tªÌ",	"¥úºa¸tªÌ"	},
	{ "¥ú©ú¸tªÌ",	"¥ú©ú¸tªÌ"	},
	{ "¤j¥ú©ú¸tªÌ",	"¤j¥ú©ú¸tªÌ"	},
	{ "¬P½÷¤§½åªÌ",			"¬P½÷¤§½åªÌ"			},
	{ "¤ë¬M¤§½åªÌ",			"¤ë¬M¤§½åªÌ"			},
	{ "¤é©ú¤§½åªÌ",			"¤é©ú¤§½åªÌ"			},
	{ "±Ï¥@¸t¥D",			"±Ï¥@¸t¥D"			},
	{ "¤Ñ¨Ï",			"¤Ñ¨Ï"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "¨k¤H",			"¤k¤H"				},

	{ "·M¥Á",			"·M¥Á"			},
	{ "¨Í±q",			"¨Í±q"			},
	{ "¨FÀ±",			"¤k¨FÀ±"			},
	{ "·s§L",			"¤k§L"			},
	{ "¶Ç±Ð¤h",			"¤k¶Ç±Ð¤h"			},

	{ "°ª¤â",			"¤k°ª¤â"				},
	{ "°Æ¥q²½",			"¤k°Æ¥q²½"			},
	{ "¥N²zªª®v",			"¥N²zªª®v"			},
	{ "ªª®v",			"¤kªª®v"			},
	{ "¦í«ù",			"¤k¦í«ù"			},

	{ "±Ð·|°Ñ¨Æ",			"±Ð·|°Ñ¨Æ"				},
	{ "¯«·µ°Æ¥D²½",			"¯«·µ¤k°Æ¥D²½"			},
	{ "§U²z¯«¤÷",			"§U²z¯«¥À"			},
	{ "¶Â¦ç­×¤h",			"¬õ¦ç­×¤k"				},
	{ "ÂåªÌ",			"¤kÂåªÌ"			},

	{ "ÀH­xªª®v",			"ÀH­x¤kªª®v"			},
	{ "¸Ñ¹DªÌ",			"¤k¸Ñ¹DªÌ"			},
	{ "¥D±Ð",			"¥D±Ð"			},
	{ "°ª¶¥¥D±Ð",		"¤k°ª¶¥¥D±Ð"	},
	{ "¤j¥D±Ð",			"¤k¤j¥D±Ð"			},

	{ "¬õ¦ç¤j¥D±Ð",		"¬õ¦ç¤k¤j¥D±Ð"		},
	{ "±Ð¥D",		"¤k±Ð¥D"		},
	{ "¤j±Ð¥D",		"¤k¤j±Ð¥D"		},
	{ "´cÅ]±þ¤â",		"´cÅ]±þ¤â"			},
	{ "¤j´cÅ]±þ¤â",	"¤j´cÅ]±þ¤â"		},

	{ "®ü¬v¼Ï¾÷¥D±Ð",	"®ü¬v¼Ï¾÷¥D±Ð"		},
	{ "¤j¦a¼Ï¾÷¥D±Ð",	"¤j¦a¼Ï¾÷¥D±Ð"		},
	{ "¤j®ð¼Ï¾÷¥D±Ð",	"¤j®ð¼Ï¾÷¥D±Ð"		},
	{ "¥H¤Ó¼Ï¾÷¥D±Ð",	"¥H¤Ó¼Ï¾÷¥D±Ð"		},
	{ "¤Ñ°ó¼Ï¾÷¥D±Ð",	"¤Ñ°ó¼Ï¾÷¥D±Ð"	},

	{ "¤£¦´ªÌ",	"¤£¦´ªÌ"		},
	{ "¯Â²b¤£¦´ªÌ",		"¯Â²b¤£¦´ªÌ"		},
	{ "¸tªÌ",	"¸tªÌ"		},
	{ "³yª«¸tªÌ",	"³yª«¸tªÌ"	},
	{ "¯Â²b¸tªÌ",	"¯Â²b¸tªÌ"	},

	{ "¥úºa¸tªÌ",	"¥úºa¸tªÌ"	},
	{ "¥ú©ú¸tªÌ",	"¥ú©ú¸tªÌ"	},
	{ "¤j¥ú©ú¸tªÌ",	"¤j¥ú©ú¸tªÌ"	},
	{ "¤Ó¤Ñ¦ì¼Ï¾÷¥D±Ð",			"¤Ó¤Ñ¦ì¼Ï¾÷¥D±Ð"			},
	{ "±Ð©v",			"±Ð©v"			},
	{ "°Æ±Ð¬Ó",			"°Æ±Ð¬Ó"			},
	{ "±Ð¬Ó",			"¤k±Ð¬Ó"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
	{ "Master Assassin",		"Master Assassin "		},
	{ "Master Assassin",		"Master Assassin "		},
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "¤Û¥@±þ¯«",		"¤Û¥@±þ¯«"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Lord of Shadow",		"Lady of Shadow"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "Å]ñZÃM¤h","­¸³·ÃM¤h"		},
	{ "¯«¬ÞÃM¤h","­YÄõÃM¤h"	},
	{ "¸t¼CÃM¤h","ª´ºÀÃM¤h"		},
	{ "¬Ó«aÃM¤h",	"Á¥Á¨ÃM¤h"		},
	{ "²V¨PÃM¤h",   "²V¨PÃM¤h"},
	{ "¤Ñ¦ì²V¨PÃM¤h",   "¤Ñ¦ì²V¨PÃM¤h"},
	{ "¤Ó¤Ñ¦ì²V¨PÃM¤h",   "¤Ó¤Ñ¦ì²V¨PÃM¤h"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "¤j¦a¼C¤h","¤j¦a¼C¤h"		},
	{ "¿Kª¢¼C¤h","¿Kª¢¼C¤h"	},
	{ "ÅQ¬Ó¼C¤h","ÅQ¬Ó¼C¤h"		},
	{ "¶Æ­·¼C¤h",	"¶Æ­·¼C¤h"		},
	{ "¼C¸t",   "¤k¼C¸t"},
	{ "¤Ñ¦ì¼C¸t",   "¤Ñ¦ì¤k¼C¸t"},
	{ "¤Ó¤Ñ¦ì¼C¸t",   "¤Ó¤Ñ¦ì¤k¼C¸t"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "¨FÀ±",			"¨FÀ±"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "ÂÃ¸g»ÕÅ@ªk","ÂÃ¸g»ÕÅ@ªk"		},
	{ "¹F¼¯°ó­º®y",	"¹F¼¯°ó­º®y"		},
	{ "§Ù«ß°|­º®y","§Ù«ß°|­º®y"	},
	{ "Ã¹º~°ó­º®y","Ã¹º~°ó­º®y"		},
	{ "¥ñªêÃ¹º~",   "¥ñªêÃ¹º~"},
	{ "­°ÀsÃ¹º~",   "­°ÀsÃ¹º~"},
	{ "¤jªüÃ¹º~",   "¤jªüÃ¹º~"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Áü­·§BÀï",	"Áü­·¤kÀï"	},
	{ "±k¬P§BÀï",		"±k¬P¤kÀï"		},

	{ "ªF¯î­ì§BÀï","ªF¯î­ì¤kÀï"		},
	{ "¦è¦B¤s§BÀï","¦è¦B¤s¤kÀï"	},
	{ "¥_¤jºz§BÀï","¥_¤jºz¤kÀï"		},
	{ "«n¤j¬v§BÀï",	"«n¤j¬v¤kÀï"		},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },
    
    {
	{ "Man",			"Woman"				},

	{ "Apprentice of Magic",	"Apprentice of Magic"		},
	{ "Spell Student",		"Spell Student"			},
	{ "Scholar of Magic",		"Scholar of Magic"		},
	{ "Delver in Spells",		"Delveress in Spells"		},
	{ "Medium of Magic",		"Medium of Magic"		},

	{ "Scribe of Magic",		"Scribess of Magic"		},
	{ "Seer",			"Seeress"			},
	{ "Sage",			"Sage"				},
	{ "Illusionist",		"Illusionist"			},
	{ "Abjurer",			"Abjuress"			},

	{ "Invoker",			"Invoker"			},
	{ "Enchanter",			"Enchantress"			},
	{ "Conjurer",			"Conjuress"			},
	{ "Magician",			"Witch"				},
	{ "Creator",			"Creator"			},

	{ "Savant",			"Savant"			},
	{ "Magus",			"Craftess"			},
	{ "Wizard",			"Wizard"			},
	{ "Warlock",			"War Witch"			},
	{ "Sorcerer",			"Sorceress"			},

	{ "Elder Sorcerer",		"Elder Sorceress"		},
	{ "Grand Sorcerer",		"Grand Sorceress"		},
	{ "Great Sorcerer",		"Great Sorceress"		},
	{ "Golem Maker",		"Golem Maker"			},
	{ "Greater Golem Maker",	"Greater Golem Maker"		},

	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Master of all Magic",	"Master of all Magic"		},

	{ "Master of all Magic",	"Master of all Magic"		},
	{ "Master of all Magic",	"Master of all Magic"		},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},

	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},

	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},

	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},
	{ "Monk",			"Nun"				},
	{ "Healer",			"Healess"			},

	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},

	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},

	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},

	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},

	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Champion of the Realm",			"Champion of the Realm"			},
	{ "Lord of the Realm",			"Lady of the Realm"			},
	{ "King of the Realm",			"Queen of the Realm"			},
	{ "Guadian of the Realm",			"Guadian of the Realm"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Áü­·§BÀï",	"Áü­·¤kÀï"	},
	{ "±k¬P§BÀï",		"±k¬P¤kÀï"		},

	{ "ªF¯î­ì§BÀï","ªF¯î­ì¤kÀï"		},
	{ "¦è¦B¤s§BÀï","¦è¦B¤s¤kÀï"	},
	{ "¥_¤jºz§BÀï","¥_¤jºz¤kÀï"		},
	{ "«n¤j¬v§BÀï",	"«n¤j¬v¤kÀï"		},
	{ "¾Ô¯«",   "¤k¾Ô¯«"},
	{ "¾Ô¯«",   "¤k¾Ô¯«"},
	{ "¾Ô¯«",   "¤k¾Ô¯«"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    },    

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Áü­·§BÀï",   "Áü­·¤kÀï"	},
	{ "±k¬P§BÀï",	    "±k¬P¤kÀï"	    },

	{ "ªF¯î­ì§BÀï","ªF¯î­ì¤kÀï"	    },
	{ "¦è¦B¤s§BÀï","¦è¦B¤s¤kÀï" },
	{ "¥_¤jºz§BÀï","¥_¤jºz¤kÀï"	    },
	{ "«n¤j¬v§BÀï",	"«n¤j¬v¤kÀï"	    },

	{ "ªZ¯«",   "¤kªZ¯«"},
	{ "ªZ¯«",   "¤kªZ¯«"},
	{ "ªZ¯«",   "¤kªZ¯«"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "³yª«¥D",		"³Ð¥@¤k¯«"			},
	{ "Implementor",		"Implementress"			}
    }
};
