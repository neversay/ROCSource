/***************************************************************************
 *  File: bit.c								   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 *									   *
 *  This code was written by Jason Dinkel and inspired by Russ Taylor,	   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.		   *
 *									   *
 ***************************************************************************/
/*
 The code below uses a table lookup system that is based on suggestions
 from Russ Taylor.  There are many routines in handler.c that would benefit
 with the use of tables.  You may consider simplifying your code base by
 implementing a system like below with such functions. -Jason Dinkel
*/



#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "olc.h"



struct flag_stat_type
{
    const struct flag_type *structure;
    bool stat;
};



/*****************************************************************************
 Name:		flag_stat_table
 Purpose:	This table catagorizes the tables following the lookup
		functions below into stats and flags.  Flags can be toggled
		but stats can only be assigned.	 Update this table when a
		new set of flags is installed.
 ****************************************************************************/
const struct flag_stat_type flag_stat_table[] =
{
/*  {	structure		stat	}, */
    {	area_flags,		FALSE	},
    {	sex_flags,		TRUE	},
    {	race_flags,		TRUE	},
    {	exit_flags,		FALSE	},
    {	door_resets,		TRUE	},
    {	room_flags,		FALSE	},
    {	sector_flags,		TRUE	},
    {	type_flags,		TRUE	},
    {	extra_flags,		FALSE	},
    {	wear_flags,		FALSE	},
    {	act_flags,		FALSE	},
    {	affect_flags,		FALSE	},
    {	apply_flags,		TRUE	},
    {	wear_loc_flags,		TRUE	},
    {	wear_loc_strings,	TRUE	},
    {	weapon_flags,		TRUE	},
    {	container_flags,	FALSE	},
    {	liquid_flags,		TRUE	},
    {	mprog_type_flags,	TRUE	},

    {	0,			0	}
};
    


/*****************************************************************************
 Name:		is_stat( table )
 Purpose:	Returns TRUE if the table is a stat table and FALSE if flag.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
bool is_stat( const struct flag_type *flag_table )
{
    int flag;

    for (flag = 0; flag_stat_table[flag].structure; flag++)
    {
	if ( flag_stat_table[flag].structure == flag_table
	  && flag_stat_table[flag].stat )
	    return TRUE;
    }
    return FALSE;
}



/*
 * This function is Russ Taylor's creation.  Thanks Russ!
 * All code copyright (C) Russ Taylor, permission to use and/or distribute
 * has NOT been granted.  Use only in this OLC package has been granted.
 */
/*****************************************************************************
 Name:		flag_lookup( flag, table )
 Purpose:	Returns the value of a single, settable flag from the table.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
long long int flag_lookup(const char *name, const struct flag_type *flag_table)
{
    int flag;
 
    for (flag = 0; *flag_table[flag].name; flag++)	/* OLC 1.1b */
    {
	if ( !str_cmp( name, flag_table[flag].name )
	  && flag_table[flag].settable )
	    return flag_table[flag].bit;
    }
 
    return NO_FLAG;
}



/*****************************************************************************
 Name:		flag_value( table, flag )
 Purpose:	Returns the value of the flags entered.	 Multi-flags accepted.
 Called by:	olc.c and olc_act.c.
 ****************************************************************************/
long long int flag_value( const struct flag_type *flag_table, char *argument)
{
    char word[MAX_INPUT_LENGTH];
    long long int	 bit;
    long long int	 marked = 0;
    bool found = FALSE;

    if ( is_stat( flag_table ) )
    {
	one_argument( argument, word );

	if ( ( bit = flag_lookup( word, flag_table ) ) != NO_FLAG )
	    return bit;
	else
	    return NO_FLAG;
    }

    /*
     * Accept multiple flags.
     */
    for (; ;)
    {
	argument = one_argument( argument, word );

	if ( word[0] == '\0' )
	    break;

	if ( ( bit = flag_lookup( word, flag_table ) ) != NO_FLAG )
	{
	    SET_BIT( marked, bit );
	    found = TRUE;
	}
    }

    if ( found )
	return marked;
    else
	return NO_FLAG;
}



/*****************************************************************************
 Name:		flag_string( table, flags/stat )
 Purpose:	Returns string with name(s) of the flags or stat entered.
 Called by:	act_olc.c, olc.c, and olc_save.c.
 ****************************************************************************/
char *flag_string( const struct flag_type *flag_table, long long int bits )
{
    static char buf[512];
    int	 flag;

    buf[0] = '\0';

    for (flag = 0; *flag_table[flag].name; flag++)	/* OLC 1.1b */
    {
	if ( !is_stat( flag_table ) && IS_SET(bits, flag_table[flag].bit) )
	{
	    strcat( buf, " " );
	    strcat( buf, flag_table[flag].name );
	}
	else
	if ( flag_table[flag].bit == bits )
	{
	    strcat( buf, " " );
	    strcat( buf, flag_table[flag].name );
	    break;
	}
    }
    return (buf[0] != '\0') ? buf+1 : "none";
}



const struct flag_type area_flags[] =
{
    {	"none",			AREA_NONE,		FALSE	},
    {	"changed",		AREA_CHANGED,		FALSE	},
    {	"added",		AREA_ADDED,		FALSE	},
    {	"loading",		AREA_LOADING,		FALSE	},
    {	"verbose",		AREA_VERBOSE,		FALSE	},
    {	"building",		AREA_BUILDING,		TRUE},
    {	"clan",		AREA_CLAN,		TRUE},
    {	"",			0,			0	}
};



const struct flag_type sex_flags[] =
{
    {	"male",			SEX_MALE,		TRUE	},
    {	"female",		SEX_FEMALE,		TRUE	},
    {	"neutral",		SEX_NEUTRAL,		TRUE	},
    {	"",			0,			0	}
};



const struct flag_type exit_flags[] =
{
    {	"door",			EX_ISDOOR,		TRUE	},
    {	"closed",		EX_CLOSED,		TRUE	},
    {	"locked",		EX_LOCKED,		TRUE	},
    {	"bashed",		EX_BASHED,		FALSE	},
    {	"bashproof",		EX_BASHPROOF,		TRUE	},
    {	"pickproof",		EX_PICKPROOF,		TRUE	},
    {	"passproof",		EX_PASSPROOF,		TRUE	},
    {	"",			0,			0	}
};

const struct flag_cost exit_cost[] = {
    {	"door",	    0,	10000 },
    {	"closed",   0,	0},
    {	"locked",   0,	50000 },
    {	"bashed",   99,-1},
    {	"bashproof",13,	500000},
    {	"pickproof",15,	300000},
    {	"passproof",17,	800000},
    {	"",			0,	0}
};



const struct flag_type door_resets[] =
{
    {	"open and unlocked",	0,		TRUE	},
    {	"closed and unlocked",	1,		TRUE	},
    {	"closed and locked",	2,		TRUE	},
    {	"",			0,		0	}
};



const struct flag_type room_flags[] =
{
    {	"dark",			ROOM_DARK,		TRUE	},
	{	"no_flee",		ROOM_NO_FLEE,	TRUE	},
    {	"no_mob",		ROOM_NO_MOB,		TRUE	},
    {	"indoors",		ROOM_INDOORS,		TRUE	},
    {	"underground",		ROOM_UNDERGROUND,	TRUE	},
	{	"scout_one",	ROOM_SCOUT_ONE,	TRUE	},
	{	"no_mark",	ROOM_NO_MARK,	TRUE	},          // room 無法施放mark 18/8/8
	{	"no_recall_in",	ROOM_NO_RECALL_IN,	TRUE	},  // 從其他的area無法recall進入mark的area 18/8/8
    {	"private",		ROOM_PRIVATE,		TRUE	},
    {	"safe",			ROOM_SAFE,		TRUE	},
    {	"solitary",		ROOM_SOLITARY,		TRUE	},
    {	"pet_shop",		ROOM_PET_SHOP,		TRUE	},
    {	"no_recall",		ROOM_NO_RECALL,		TRUE	},
    {	"cone_of_silence",	ROOM_CONE_OF_SILENCE,	TRUE	},
	{	"camp",			ROOM_CAMP,		TRUE	},      // 可以下evacuate標記的 flag 19/12/08
    {	"summon_bit",	ROOM_SUMMON_BIT,	TRUE	},
    {	"auction",	ROOM_AUCTION,	TRUE	},
    {	"save",	ROOM_SAVE,	TRUE	},
    {	"no_teleport_out",	ROOM_NO_TELEPORT,	TRUE	},
    {	"no_teleport_to",	ROOM_NO_TELEPORT_TO,	TRUE	},
    {	"pk",	ROOM_PK,	TRUE	},
    {	"magic_tower",   ROOM_MAGIC_TOWER, TRUE},
    {	"mil_department",   ROOM_MIL_DEPARTMENT, TRUE},
    {	"misc_research",    ROOM_MISC_RESEARCH, TRUE},
    {	"barrack",  ROOM_BARRACK, TRUE},
    {	"armory",   ROOM_ARMORY, TRUE},
    {	"tavern",   ROOM_TAVERN, TRUE},
    {	"library",  ROOM_LIBRARY, TRUE},
    {	"lv1",	    ROOM_CLAN_LV1, TRUE},
    {	"lv2",	    ROOM_CLAN_LV2, TRUE},
    {	"lv3",	    ROOM_CLAN_LV3,  TRUE},
    {   "horse_shop", ROOM_HORSE_SHOP, TRUE},
    {	"",			0,			0	}
};

const struct flag_cost room_cost[] = {
    { "dark",		0,		10000},
    { "no_mob",		17,		10000000},
    { "indoors",	0,		50000},
    { "underground",10,		100000},
    { "private",	99,		-1},
    { "safe",		20,		10000000},
    { "solitary",	99,		-1},
    { "pet_shop",	99,		-1},
    { "no_recall",	99,		-1},
    { "cone_of_silence",99,-1},
    { "summon_bit",	99,		-1},
    { "auction",	18,		20000000},
    { "save",		0,		0},
    { "no_teleport_out",0,	0},
    { "no_teleport_to", 0,	0},
    { "pk",			10,		500000},
    { "magic_tower",	16,	10000000},
    { "mil_department",	19,	30000000},
    { "misc_research",  17,	20000000},
    { "barrack",  	8,	5000000},
    { "armory",  	2,	4000000},
    { "tavern",  	13,	4000000},
    { "library",  	18,	10000000},
    { "lv1",	    3,	200000},
    { "lv2",	    4,	400000},
    { "lv3",	    5,	800000},
    { "horse_shop", 99, -1},
    { "", 			0,	0}
};



const struct flag_type sector_flags[] =
{
    {	"inside",	SECT_INSIDE,		TRUE	},
    {	"city",		SECT_CITY,		TRUE	},
    {	"field",	SECT_FIELD,		TRUE	},
    {	"forest",	SECT_FOREST,		TRUE	},
    {	"hills",	SECT_HILLS,		TRUE	},
    {	"mountain",	SECT_MOUNTAIN,		TRUE	},
    {	"swim",		SECT_WATER_SWIM,	TRUE	},
    {	"noswim",	SECT_WATER_NOSWIM,	TRUE	},
    {	"underwater",	SECT_UNDERWATER,	TRUE	},
    {	"air",		SECT_AIR,		TRUE	},
    {	"desert",	SECT_DESERT,		TRUE	},
    {	"",		0,			0	}
};

const struct flag_cost sector_cost[] = 
{
    { "inside",	    0,	50000},
    { "city",	    0,	50000},
    { "field",	    0,	30000},
    { "forest",	    0,	10000},
    { "hills",	    0,	10000},
    { "mountain",   0,	10000},
    { "swim",	    0,	10000},
    { "noswim",	    99,	-1},
    { "underwater", 99,	-1},
    { "air",	    99,	-1},
    { "desert",	    0,	10000},
    { "", 			0,	0}
};

const struct flag_type type_flags[] =
{
    {	"light",		ITEM_LIGHT,		TRUE	},
    {	"scroll",		ITEM_SCROLL,		TRUE	},
    {	"wand",			ITEM_WAND,		TRUE	},
    {	"staff",		ITEM_STAFF,		TRUE	},
    {	"weapon",		ITEM_WEAPON,		TRUE	},
    {	"treasure",		ITEM_TREASURE,		TRUE	},
    {	"armor",		ITEM_ARMOR,		TRUE	},
    {	"potion",		ITEM_POTION,		TRUE	},
    {	"furniture",		ITEM_FURNITURE,		TRUE	},
    {	"trash",		ITEM_TRASH,		TRUE	},
    {	"container",		ITEM_CONTAINER,		TRUE	},
    {	"drink-container",	ITEM_DRINK_CON,		TRUE	},
    {	"key",			ITEM_KEY,		TRUE	},
    {	"food",			ITEM_FOOD,		TRUE	},
    {	"money",		ITEM_MONEY,		TRUE	},
    {	"boat",			ITEM_BOAT,		TRUE	},
    {	"npc corpse",		ITEM_CORPSE_NPC,	TRUE	},
    {	"pc corpse",		ITEM_CORPSE_PC,		FALSE	},
    {	"fountain",		ITEM_FOUNTAIN,		TRUE	},
    {	"pill",			ITEM_PILL,		TRUE	},
    {	"arrow",			ITEM_ARROW,		TRUE	},
    {	"dart",			ITEM_DART,		TRUE    },
    {   "horse",		ITEM_HORSE,		TRUE},
    {	"ammo",			ITEM_AMMO,		TRUE    }, // 2022/04/29
    {	"",			0,			0	}
};

const struct flag_cost type_cost[] = {
    { "light",	    10, 	10000},
    { "scroll",	    99, 	-1},
    { "wand",	    99, 	-1},
    { "staff",	    99,	-1},
    { "weapon",	    6, 		50000},
    { "treasure",   99, 	-1},
    { "armor",	    8, 		-1},
    { "potion",   	99, 	-1},
    { "furniture",  2, 		50000},
    { "trash",		0, 		10000},
    { "container",  12,		-1},
    { "drink-container", 14,2000000},
    { "key",	    2, 		20000},
    { "food",     2,    2000000},
    { "money",		99, 	-1},
    { "boat", 		5,		100000},
    { "npc corpse", 99, 	-1},
    { "pc corpse", 	99, 	-1},
    { "fountain", 	4, 		1000000},
    { "pill", 		99,		-1},
    { "arrow", 		99,		-1},
    { "", 			0,		0}
};


const struct flag_type extra_flags[] =
{
    {	"glow",			ITEM_GLOW,		TRUE	},
    {	"hum",			ITEM_HUM,		TRUE	},
    {	"dark",			ITEM_DARK,		TRUE	},
    {	"lock",			ITEM_LOCK,		TRUE	},
    {	"evil",			ITEM_EVIL,		TRUE	},
    {	"invis",		ITEM_INVIS,		TRUE	},
    {	"magic",		ITEM_MAGIC,		TRUE	},
    {	"nodrop",		ITEM_NODROP,		TRUE	},
    {	"bless",		ITEM_BLESS,		TRUE	},
    {	"anti-good",		ITEM_ANTI_GOOD,		TRUE	},
    {	"anti-evil",		ITEM_ANTI_EVIL,		TRUE	},
    {	"anti-neutral",		ITEM_ANTI_NEUTRAL,	TRUE	},
    {	"noremove",		ITEM_NOREMOVE,		TRUE	},
    {	"inventory",		ITEM_INVENTORY,		TRUE	},
    {	"poisoned",		ITEM_POISONED,		TRUE	},
    {	"vampire-bane",		ITEM_VAMPIRE_BANE,	TRUE	},
    {	"holy",			ITEM_HOLY,		TRUE	},
    {	"remove_nodrop",	ITEM_REMOVE_NODROP,	TRUE	},
    {	"donation_pit",	ITEM_DONATION_PIT,	TRUE	},
    {	"unique",	ITEM_UNIQUE,		FALSE},
    {	"clan",	ITEM_CLAN,		TRUE	},
    {	"personal",	ITEM_PERSONAL,		TRUE	},
    {	"spell",	ITEM_SPELL,		TRUE	}, // ITEM_RACIAL change to ITEM_SPELL 2021/09/19
    {	"nosave",	ITEM_NOSAVE,		TRUE	},
    {	"lore",	ITEM_LORE,		TRUE	},
    {	"nomale",	ITEM_NO_MALE,		TRUE	},
    {	"nosac",	ITEM_NOSAC,		TRUE	},
    {	"nofemale",	ITEM_NO_FEMALE,		TRUE	},
    {	"noneutral",	ITEM_NO_NEUTRAL,		TRUE	},
	{	"special",  ITEM_SPECIAL,	TRUE		},
    {	"",			0,			0	}
};

const struct flag_cost extra_cost[] = {
    { "glow",		16,	100000},
    { "hum",		20,	200000},
    { "dark",		26,	7000000},
    { "lock",		15,	50000},
    { "evil",		22,	300000},
    { "invis",		21,	500000},
    { "magic",		99,-1},
    { "nodrop",		17,	300000},
    { "bless",		23, 4000000},
    { "anti-good", 	99,-1},
    { "anti-evil", 	99,-1},
    { "anti-neutral",99,-1},
    { "noremove", 	99,-1},
    { "inventory", 	99,-1},
    { "poisoned", 	24,	7000000},
    { "vampire-bane",99,-1},
    { "holy", 		99,-1},
    { "remove_nodrop",99,-1},
    { "donation_pit",99,-1},
    { "unique",		99,-1},
    { "clan", 		99,-1},
    { "personal", 	99,-1},
    { "nosave",	99,-1},
    {	"nomale",	99,		-1},
    {	"nosac",	99,		-1},
    {	"nofemale",	99,		-1},
    {	"noneutral",	99,		-1},
    { "", 			0,	0}
};



const struct flag_type wear_flags[] =
{
    {	"take",			ITEM_TAKE,			TRUE	},
    {	"finger",		ITEM_WEAR_FINGER,	TRUE	},
    {	"neck",			ITEM_WEAR_NECK,		TRUE	},
    {	"body",			ITEM_WEAR_BODY,		TRUE	},
    {	"head",			ITEM_WEAR_HEAD,		TRUE	},
    {	"legs",			ITEM_WEAR_LEGS,		TRUE	},
    {	"feet",			ITEM_WEAR_FEET,		TRUE	},
    {	"hands",		ITEM_WEAR_HANDS,	TRUE	},
    {	"arms",			ITEM_WEAR_ARMS,		TRUE	},
    {	"shield",		ITEM_WEAR_SHIELD,	TRUE	},
    {	"about",		ITEM_WEAR_ABOUT,	TRUE	},
    {	"waist",		ITEM_WEAR_WAIST,	TRUE	},
    {	"wrist",		ITEM_WEAR_WRIST,	TRUE	},
    {	"wield",		ITEM_WIELD,			TRUE	},
    {	"hold",			ITEM_HOLD,			TRUE	},
    {	"twohanded",	ITEM_TWO_HAND,		TRUE	},
    {	"arrow",		ITEM_WEAR_ARROW,	TRUE	},
    {   "ride",			ITEM_WEAR_RIDE,		FALSE	}, //add by coco 0410
    {   "ammo",			ITEM_WEAR_AMMO,		TRUE	}, //add at 2022/04/30
    {	"",			0,			0	}
};

const struct flag_cost wear_cost[] = {
    { "take",	1,	1000},
    { "finger", 11,	10000},
    { "neck",	11,	10000},
    { "body",	16,	30000},
    { "head",	12,	20000},
    { "legs",	12,	20000},
    { "feet",	12,	20000},
    { "hands",	12,	20000},
    { "arms",	13,	25000},
    { "shield",	17,	30000},
    { "about",	12,	15000},
    { "waist",	10,	10000},
    { "wrist",	12,	15000},
    { "wield",	6,	50000},
    { "hold",	11,	30000},
    { "twohanded",16,50000},
    { "arrow", 	17,	30000},
    { "",   	0,	0}
};



const struct flag_type act_flags[] =
{
    {	"npc",			ACT_IS_NPC,		FALSE	},
    {	"sentinel",		ACT_SENTINEL,		TRUE	},
    {	"scavenger",		ACT_SCAVENGER,		TRUE	},
    {	"aggressive",		ACT_AGGRESSIVE,		TRUE	},
    {	"stay_area",		ACT_STAY_AREA,		TRUE	},
    {	"wimpy",		ACT_WIMPY,		TRUE	},
    {	"pet",			ACT_PET,		TRUE	},
    {	"train",		ACT_TRAIN,		TRUE	},
    {	"practice",		ACT_PRACTICE,		TRUE	},
    {	"gamble",		ACT_GAMBLE,		TRUE	},
    {	"reflexion",		ACT_REFLEXION,		TRUE	},
    {	"critical",		ACT_CRITICAL,		TRUE	},
    {	"swordmaster",		ACT_HARD_DIS,		TRUE	},
    {	"hunt",			ACT_HUNT,		TRUE	},
    {	"dual_weapon",		ACT_DUAL,		TRUE	},
	{	"mob_no_flee",		ACT_NO_FLEE,		TRUE	}, //將 no_flee改為 mob_no_flee 避免 null bstr 2022/01/03
    {	"moved",		ACT_MOVED,		FALSE	},
    {	"tactics",		ACT_TACTICS,		TRUE	},
    {	"track",		ACT_TRACK,		TRUE	},
    {	"regeneracy",		ACT_REGENERACY,		TRUE	},
    {	"banker",		ACT_BANKER,		TRUE	},
    {	"mob_ai",		ACT_MOB_AI,		TRUE	},
    {	"reflexion",		ACT_REFLEXION,		TRUE	},
    {	"ignore_par",		ACT_NOPAR,		TRUE	},
    {	"ignore_nomob",		ACT_IGNORE_NOMOB,	TRUE	},
    {	"mprog_death",		ACT_MPROG_DEATH,	FALSE	},
    {   "clan",                 ACT_CLAN,               FALSE   },
    {	"smith",		ACT_SMITH,		TRUE	},
    {	"horse",		ACT_HORSE,		FALSE    },
    {	"",			0,			0	}
};



const struct flag_type affect_flags[] =
{
    {	"blind",		AFF_BLIND,		TRUE	},
    {	"invisible",		AFF_INVISIBLE,		TRUE	},
    {	"detect-evil",		AFF_DETECT_EVIL,	TRUE	},
    {	"detect-invis",		AFF_DETECT_INVIS,	TRUE	},
    {	"detect-magic",		AFF_DETECT_MAGIC,	TRUE	},
    {	"detect-hidden",	AFF_DETECT_HIDDEN,	TRUE	},
    {	"hold",			AFF_HOLD,		TRUE	},
    {	"sanctuary",		AFF_SANCTUARY,		TRUE	},
    {	"faerie-fire",		AFF_FAERIE_FIRE,	TRUE	},
    {	"tardy",		AFF_TARDY,	TRUE	},
    {	"infrared",		AFF_INFRARED,		TRUE	},
    {	"curse",		AFF_CURSE,		TRUE	},
    {	"change-sex",		AFF_CHANGE_SEX,		TRUE	},
    {	"poison",		AFF_POISON,		TRUE	},
    {	"protect",		AFF_PROTECT,		TRUE	},
    {	"polymorph",		AFF_POLYMORPH,		TRUE	},
    {	"sneak",		AFF_SNEAK,		TRUE	},
    {	"hide",			AFF_HIDE,		TRUE	},
    {	"sleep",		AFF_SLEEP,		TRUE	},
    {	"charm",		AFF_CHARM,		TRUE	},
    {	"flying",		AFF_FLYING,		TRUE	},
    {	"pass-door",		AFF_PASS_DOOR,		TRUE	},
    {	"waterwalk",		AFF_WATERWALK,		TRUE	},
    {	"summoned",		AFF_SUMMONED,		TRUE	},
    {	"mute",			AFF_MUTE,		TRUE	},
//    {	"gills",		AFF_GILLS,		TRUE	},
    {	"vamp-bite",		AFF_VAMP_BITE,		TRUE	},
    {	"ghoul",		AFF_GHOUL,		TRUE	},
    {	"flaming",		AFF_FLAMING,		TRUE	},
    {	"paralyzed",		AFF_PARALYZED,		TRUE	},
    {	"crazy",		AFF_CRAZY,		TRUE	},
    {   "weaken",		AFF_WEAKEN,		TRUE	},
    {	"nightstalk",		AFF_NIGHTSTALK,		TRUE	},
    {	"seal magic",		AFF_SEAL_MAGIC,		TRUE	},
    {	"dark raid",		AFF_DARK_RAID,		TRUE	},
    {	"mana shield",		AFF_MANA_SHIELD,	TRUE	},
    {	"mystic mirror",	AFF_MYSTIC_MIRROR,	TRUE	},
    {	"mystic armor",	AFF_MYSTIC_ARMOR,	TRUE	},
    {	"cast_acc",	AFF_CAST_ACC,	TRUE	},
	{   "disease", AFF_DISEASE, TRUE },
	{   "fear", AFF_FEAR, TRUE },
    {   "ride", AFF_RIDE, TRUE },
	{   "no_heal", AFF_NO_HEAL, TRUE }, //add at 2018.11.20
    {	"",			0,			0	}
};


/*
 * Used when adding an affect to tell where it goes.
 * See addaffect and delaffect in act_olc.c
 */
const struct flag_type apply_flags[] =
{
    {	"none",			APPLY_NONE,		TRUE	},
    {	"str",		APPLY_STR,		TRUE	},
    {	"dex",		APPLY_DEX,		TRUE	},
    {	"int",		APPLY_INT,		TRUE	},
    {	"wis",		APPLY_WIS,		TRUE	},
    {	"con",		APPLY_CON,		TRUE	},
    {	"sex",			APPLY_SEX,		TRUE	},
    {	"class",		APPLY_CLASS,		TRUE	},
    {	"level",		APPLY_LEVEL,		TRUE	},
    {	"age",			APPLY_AGE,		TRUE	},
    {	"height",		APPLY_HEIGHT,		TRUE	},
    {	"weight",		APPLY_WEIGHT,		TRUE	},
    {	"mana",			APPLY_MANA,		TRUE	},
    {	"hp",			APPLY_HIT,		TRUE	},
    {	"move",			APPLY_MOVE,		TRUE	},
    {	"gold",			APPLY_GOLD,		TRUE	},
    {	"exp",		APPLY_EXP,		TRUE	},
    {	"ac",			APPLY_AC,		TRUE	},
    {	"hr",		APPLY_HITROLL,		TRUE	},
    {	"dr",		APPLY_DAMROLL,		TRUE	},
    {	"saving-para",		APPLY_SAVING_PARA,	TRUE	},
    {	"svs",		APPLY_SAVING_SPELL,	TRUE	},
    {	"saving-life",		APPLY_SAVING_LIFE,	TRUE	},
    {	"saving-petri",		APPLY_SAVING_PETRI,	TRUE	},
    {	"saving-breath",	APPLY_SAVING_BREATH,	TRUE	},
    {	"saving-spell",		APPLY_SAVING_SPELL,	TRUE	},
    {	"saving-element",		APPLY_SAVING_ELEMENT,	TRUE	},
    {	"dodge",		APPLY_DODGE,		TRUE},
    {	"pp",			APPLY_POWER_POTENTIAL,	    TRUE},
    {	"physical_damage_res",	APPLY_RDAMAGE,		    TRUE},
    {	"magic_damage_res",		APPLY_RMAGIC,		TRUE},
    {	"magic_damage",		APPLY_AMAGIC,		TRUE},
    {	"physical_damage",		APPLY_ADAMAGE,		TRUE},
    {	"magic_damage_gate",		APPLY_GMAGIC,		FALSE},
    {	"physical_damage_gate",		APPLY_GPHYSICAL,		FALSE},
    {	"race",			APPLY_RACE,		TRUE	},
    {	"",			0,			0	}
};



/*
 * What is seen.
 */
const struct flag_type wear_loc_strings[] =
{
    {	"in the inventory",		WEAR_NONE,		TRUE	},
    {	"as a light",			WEAR_LIGHT,		TRUE	},
    {	"on the left finger",	WEAR_FINGER_L,	TRUE	},
    {	"on the right finger",	WEAR_FINGER_R,	TRUE	},
    {	"around the neck (1)",	WEAR_NECK_1,	TRUE	},
    {	"around the neck (2)",	WEAR_NECK_2,	TRUE	},
    {	"on the body",			WEAR_BODY,		TRUE	},
    {	"over the head",		WEAR_HEAD,		TRUE	},
    {	"on the legs",			WEAR_LEGS,		TRUE	},
    {	"on the feet",			WEAR_FEET,		TRUE	},
    {	"on the hands",			WEAR_HANDS,		TRUE	},
    {	"on the arms",			WEAR_ARMS,		TRUE	},
    {	"as a shield",			WEAR_SHIELD,	TRUE	},
    {	"about the shoulders",	WEAR_ABOUT,		TRUE	},
    {	"around the waist",		WEAR_WAIST,		TRUE	},
    {	"on the left wrist",	WEAR_WRIST_L,	TRUE	},
    {	"on the right wrist",	WEAR_WRIST_R,	TRUE	},
    {	"wielded",				WEAR_WIELD,		TRUE	},
    {	"held in the hands",	WEAR_HOLD,		TRUE	},
    {	"second wield",			WEAR_WIELD_2,	TRUE	},
    {	"third ring",			WEAR_FINGER_3,	TRUE	},
    {	"fourth ring",			WEAR_FINGER_4,	TRUE	},
    {	"twohanded",			WEAR_TWO_HAND ,	TRUE	},
    {	"wear as arrow",		WEAR_ARROW,		TRUE	},
    {   "ride horse",			WEAR_RIDE,		FALSE	}, // add by coco 0410
    {   "ammo",					WEAR_AMMO,		TRUE	}, // add at 2022/04/30
    {	"",			0			}
};


/*
 * What is typed.
 * Neck2 should not be settable for loaded mobiles.
 */
const struct flag_type wear_loc_flags[] =
{
    {	"none",		WEAR_NONE,	TRUE	},
    {	"light",	WEAR_LIGHT,	TRUE	},
    {	"lfinger",	WEAR_FINGER_L,	TRUE	},
    {	"rfinger",	WEAR_FINGER_R,	TRUE	},
    {	"neck1",	WEAR_NECK_1,	TRUE	},
    {	"neck2",	WEAR_NECK_2,	TRUE	},
    {	"body",		WEAR_BODY,	TRUE	},
    {	"head",		WEAR_HEAD,	TRUE	},
    {	"legs",		WEAR_LEGS,	TRUE	},
    {	"feet",		WEAR_FEET,	TRUE	},
    {	"hands",	WEAR_HANDS,	TRUE	},
    {	"arms",		WEAR_ARMS,	TRUE	},
    {	"shield",	WEAR_SHIELD,	TRUE	},
    {	"about",	WEAR_ABOUT,	TRUE	},
    {	"waist",	WEAR_WAIST,	TRUE	},
    {	"lwrist",	WEAR_WRIST_L,	TRUE	},
    {	"rwrist",	WEAR_WRIST_R,	TRUE	},
    {	"wielded",	WEAR_WIELD,	TRUE	},
    {	"hold",		WEAR_HOLD,	TRUE	},
    {	"sec_wield",	WEAR_WIELD_2,	TRUE	},
    {	"twohand",	WEAR_TWO_HAND,	TRUE	},
    {	"arrow",	WEAR_ARROW,	TRUE	},
    {	"finger3",	WEAR_FINGER_3,	TRUE	},
    {	"finger4",	WEAR_FINGER_4,	TRUE	},
    {   "ride",		WEAR_RIDE,	TRUE},//add by coco 0410
    {   "ammo",		WEAR_AMMO,	TRUE},//add at 2022/04/30
    {	"",		0,		0	}
};



const struct flag_type weapon_flags[] =
{
    {	"hit",		0,	TRUE	},
    {	"slice",	1,	TRUE	},
    {	"stab",		2,	TRUE	},
    {	"slash",	3,	TRUE	},
    {	"whip",		4,	TRUE	},
    {	"claw",		5,	TRUE	},
    {	"blast",	6,	TRUE	},
    {	"pound",	7,	TRUE	},
    {	"crush",	8,	TRUE	},
    {	"grep",		9,	TRUE	},
    {	"bite",		10,	TRUE	},
    {	"pierce",	11,	TRUE	},
    {	"suction",	12,	TRUE	},
    {	"chop",		13,	TRUE	},
    {	"shot",		14,	TRUE	},
    {	"",		0,	0	}
};


const struct flag_type container_flags[] =
{
    {	"closeable",		CONT_CLOSEABLE,	TRUE	},
    {	"pickproof",		CONT_PICKPROOF,	TRUE	},
    {	"closed",		CONT_CLOSED,	TRUE	},
    {	"locked",		CONT_LOCKED,	TRUE	},
    {	"",			0,		0	}
};



const struct flag_type liquid_flags[] =
{
    {	"water",		0,	TRUE	},
    {	"beer",			1,	TRUE	},
    {	"wine",			2,	TRUE	},
    {	"ale",			3,	TRUE	},
    {	"dark-ale",		4,	TRUE	},
    {	"whisky",		5,	TRUE	},
    {	"lemonade",		6,	TRUE	},
    {	"firebreather",		7,	TRUE	},
    {	"local-specialty",	8,	TRUE	},
    {	"slime-mold-juice",	9,	TRUE	},
    {	"milk",			10,	TRUE	},
    {	"tea",			11,	TRUE	},
    {	"coffee",		12,	TRUE	},
    {	"blood",		13,	TRUE	},
    {	"salt-water",		14,	TRUE	},
    {	"cola",			15,	TRUE	},
    {	"",			0,	0	}
};


/* 
 * Race types for medit - Envy 2.x version
 * New table by Stoked for OLC
 */
const	struct	  flag_type	  race_flags	  [ ]	 =
{
    { "Human",	    0,	   TRUE	   },
    { "Elf",	    1,	   TRUE	   },
    { "Halfelf",    2,	   TRUE	   },
    { "Drow",	    3,	   TRUE	   },
    { "Dwarf",	    4,	   TRUE	   },
    { "Halfdwarf",  5,	   TRUE	   },
    { "Hobbit",	    6,	   TRUE	   },
    { "Gnome",	    7,	   TRUE	   },
    { "Halfkobold", 8,	   TRUE	   },
    { "Giant",	    9,	   TRUE	   },
    { "Ogre",	   10,	   FALSE   },
    { "Orc",	   11,	   TRUE	   },
    { "Kobold",	   12,	   TRUE	   },
    { "Minotaur",  13,	   TRUE	   },
    { "Troll",	   14,	   TRUE	   },
    { "Hobgoblin", 15,	   TRUE	   },
    { "Insect",	   16,	   TRUE	   },
    { "Dragon",	   17,	   FALSE   },
    { "Animal",	   18,	   TRUE	   },
    { "God",	   19,	   FALSE   },
    { "Undead",	   20,	   TRUE	   },
    { "Harpy",	   21,	   TRUE	   },
    { "Bear",	   22,	   TRUE	   },
    { "Githyanki", 23,	   TRUE	   },
    { "Elemental", 24,	   TRUE	   },
    { "Bat",	   25,	   TRUE	   },
    { "Plant",	   26,	   TRUE	   },
    { "Rat",	   27,	   TRUE	   },
    { "Vampire",   28,	   TRUE	   },
    { "Werewolf",  29,	   TRUE	   },
    { "Goblin",	   30,	   TRUE	   },
    { "Faerie",	   31,	   TRUE	   },
    { "Arachnid",  32,	   TRUE	   },
    { "Mindflayer",33,	   TRUE	   },
    { "Object",	   34,	   TRUE	   },
    { "Mist",	   35,	   TRUE	   },
    { "Snake",	   36,	   TRUE	   },
    { "Worm",	   37,	   TRUE	   },
    { "Fish",	   38,	   TRUE	   },
    { "Hydra",	   39,	   TRUE	   },
    { "Lizard",	   40,	   TRUE	   },
    { "Centaur",   41,	   TRUE	   },
    { "Halfling",  42,	   TRUE	   },
    { "Statue",	   43,	   TRUE	   },
    { "Phantom",   44,	   TRUE	   },
    { "Pixie",	   45,	   TRUE	   },
    { "Beast",	   46,	   FALSE   },
    { "Beholder",  47,	   TRUE	   },
    { "Lizardman", 48,	   TRUE	   },
    { "Dinosaur",  49,	   FALSE   },
    { "Drake",	   50,	   FALSE   },
    //{ "Wurm",	   51,	   FALSE   },  誤字修正 2022/01/14
	{ "Wyrm",	   51,	   FALSE   },
    { "Wyvern",	   52,	   FALSE   },
    { "",	    0,	   0	   }
};

const struct flag_type mprog_type_flags[] =
{
    {	"in_file_prog",		IN_FILE_PROG,	TRUE	},
    {	"act_prog",		ACT_PROG,	TRUE	},
    {	"speech_prog",		SPEECH_PROG,	TRUE	},
    {	"rand_prog",		RAND_PROG,	TRUE	},
    {	"fight_prog",		FIGHT_PROG,	TRUE	},
    {	"hitprcnt_prog",	HITPRCNT_PROG,	TRUE	},
    {	"death_prog",		DEATH_PROG,	TRUE	},
    {	"entry_prog",		ENTRY_PROG,	TRUE	},
    {	"greet_prog",		GREET_PROG,	TRUE	},
    {	"all_greet_prog",	ALL_GREET_PROG, TRUE	},
    {	"give_prog",		GIVE_PROG,	TRUE	},
    {	"bribe_prog",		BRIBE_PROG,	TRUE	},
    {	"talk_prog",		TALK_PROG,	TRUE	},
    {	"entry_greet_prog", ENTRY_GREET_PROG,	TRUE	}, // add at 2021/12/18
    {	"",			0,		0	}
};

const struct flag_cost spec_cost[] = {
    { "spec_breath_any",	31,		50000000	},
    { "spec_breath_acid",	30,		30000000	},
    { "spec_breath_fire",	29,		20000000	},
    { "spec_breath_frost",	28,		40000000	},
    { "spec_breath_gas",	27,		10000000	},
    { "spec_breath_lightning",	26,		10000000	},
    { "spec_cast_adept",	20,		8000000		},
    { "spec_cast_cleric",	17,		4000000		},
    { "spec_cast_ghost",	15,		500000		},
    { "spec_cast_judge",	18,		5000000		},
    { "spec_cast_mage",		19,		6000000		},
    { "spec_cast_psionicist",	99,		-1		},
    { "spec_cast_undead",	20,		7000000		},
    { "spec_executioner",	22,		3000000		},
    { "spec_fido",		13,		10000		},
    { "spec_guard",		12,		800000		},
    { "spec_janitor",		10,		10000		},
    { "spec_mayor",		99,		-1		},
    { "spec_poison",		14,		2000000		},
    { "spec_repairman",		99,		-1		},
    { "spec_thief",		24,		5000000		},
    { "spec_cast_fire",		28,		8000000		},
    { "spec_cast_cold",		27,		7000000		},
    { "spec_cast_wind",		29,		9000000		},
    { "spec_cast_earth",	26,		6000000		},
    { "spec_assassin",		30,		10000000	},
    { "",	0,	0}
};

const struct flag_cost class_cost[] = {
    { "warrior",	22,		15000000	},
    { "mage",		17,		10000000	},
    { "thief",		19,		13000000	},
    { "swordsman",	25,		10000000	},
    { "ranger",		34,		80000000	},
    { "",	0,	0}
};
