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
//*****************************************************************
//*  PROGRAM ID  : const.c                                        *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940710 jye                                    *
//*  PURPOSE     :         修改spell_gate ppl不能學習             *
//*              :         修改spell_change_sex ppl不能學習       *
//*              :         修改spell_destroy_cursed ppl不能學習   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940711 jye                                    *
//*  PURPOSE     :         新增spell_flame_elemental              *
//*              :             spell_water_elemental              *
//*              :             spell_earth_elemental              *
//*              :             spell_wind_elemental               *
//*              :             spell_lightning_elemental          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940725 jye                                    *
//*  PURPOSE     :         修改 Sor New Skill 可學習等級及加上stun*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941002 Razgriz                                *
//*  PURPOSE     :         修改will fortress為 TAR_CHAR_DEFENSIVE *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941016 Razgriz                                *
//*  PURPOSE     :         修改 背刺 的使用時機為 POS_FIGHTING    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050426 Jye                                    *
//*  PURPOSE     :         修改 種族屬性                          *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"

const int inborn_ability_cost[RACE_ABILITY_COUNT] =
{
    0, //     0                RACE_NO_ABILITIES         0
    0, //     1                RACE_PC_AVAIL             1
    0, //     2                RACE_WATERBREATH          2
   18, //     3                RACE_FLY                  4  影響 DC 值
    0, //     4                RACE_SWIM                 8
    0, //     5                RACE_WATERWALK           16
    0, //     6                RACE_PASSDOOR            32
   13, //     7                RACE_INFRAVISION         64  提升夜晚中每 tick 的 HP/MP/MV 的回復
    0, //     8                RACE_DETECT_ALIGN       128
    0, //     9                RACE_DETECT_INVIS       256
    0, //    10                RACE_DETECT_HIDDEN      512
   23, //    11                RACE_PROTECTION        1024  抵擋傷害的比率提升
    0, //    12                RACE_SANCT             2048
    0, //    13                RACE_WEAPON_WIELD      4096
    0, //    14                RACE_MUTE              8192
    0, //    15                RACE_NO_POISON        16384
   13, //    16                RACE_DETECT_MAGIC     32768  pp , md 額外加成
   55, //    17                RACE_REGENERACY       65536  再生能力的提升
   27, //    18                RACE_BEHOLDER        131072  影響到 gaze 的能力，sklv 到一定水準(可能是天人)可多穿點裝備
   33, //    19                RACE_LIZARDMAN       262144  影響到吐酸、掃尾的攻擊
   31, //    20                RACE_ORC             524288  影響吃屍的效果
   39, //    21                RACE_VAMPIRE        1048576  影響到月夜及吸血鬼之吻
    0, //    22                RACE_INVIS          2097152
    0, //    23                RACE_ANTI_MAGIC     4194304  對魔法傷害部分減免
   23, //    24                RACE_UNDEAD         8388608  影響對神聖魔法的抵抗力
   45, //    25                RACE_WEREWOLF      16777216  減短白天變回人形的時間、弱化效果，以及提升午夜能力。
   37, //    26                RACE_GARGOLYE      33554432  擁有石化能力
   35, //    27                RACE_CENTAUR       67108864  人馬, 可穿兩個 boot 兩個 legging, 內建座騎
    0, //    28
    0, //    29
    0, //    30
    0  //    31
};

const int apply_class[6] =
{
	0,
	/* STR */ 3,
	/* DEX */ 2,
	/* INT */ 0,
	/* WIS */ 1,
	/* CON */ 4
};
/*
 * Class table.
 */
const struct class_type class_table [MAX_CLASS]	=
{
    {
		"Mag", "Mage", "法師","法  師", APPLY_INT,  OBJ_VNUM_SCHOOL_DAGGER,
		3018,  85,  8,	 -1,   6,  8, TRUE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"Cle", "Cleric", "牧師","牧  師", APPLY_WIS,  OBJ_VNUM_SCHOOL_MACE,
		3003,  80,  8,	 -8,  6, 10, TRUE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"Thi", "Thief", "妙賊","妙  賊", APPLY_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
		3028,  75,  8,	 -10,  8, 13, FALSE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"War", "Warrior", "戰士", "戰  士",APPLY_STR,  OBJ_VNUM_SCHOOL_SWORD,
		3022,  75,  8,	 -12,  11, 15, FALSE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"Bar", "Barbarian", "野蠻人", "野蠻人",APPLY_CON,  OBJ_VNUM_SCHOOL_SWORD,
		3221,  70,  8,	 -12,  11, 20, FALSE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },
    
    {
		"Wiz", "Wizard", "巫師", "巫  師",APPLY_WIS,  OBJ_VNUM_SCHOOL_DAGGER,
		3018,  99,  2,	 -4,   6,  9, TRUE, { 20, L_APP, L_APP, 36, L_APP}
    },

    {
		"Alc", "Alchemist", "鍊金師", "鍊金師",APPLY_STR,  OBJ_VNUM_SCHOOL_DAGGER,
		3018,  99,  0,	 -16,   8,  12, TRUE, { 20, L_APP, L_APP, 34, 36}
    },

    {
		"Sav", "Savant", "賢者", "賢  者",APPLY_DEX,  OBJ_VNUM_SCHOOL_MACE,
		3003,  99,  8,	 -1,  7, 10, TRUE, { L_APP, 20, L_APP, L_APP, L_APP}
    },

    {
		"Bis", "Bishop", "主教", "主  教",APPLY_INT,  OBJ_VNUM_SCHOOL_DAGGER,
		3003,  99,  5,	 -12,  8, 13, TRUE, { 35, 20, L_APP, L_APP, L_APP}
    },


    {
		"Asn", "Assassin", "刺客","刺  客", APPLY_STR,  OBJ_VNUM_SCHOOL_SWORD,
		3028,  90,  0,	 -20,  11, 15, FALSE, { L_APP, L_APP, 20, 34, 36}
    },

    {
		"Nin", "Ninja", "忍者","忍  者", APPLY_INT,  OBJ_VNUM_SCHOOL_SWORD,
		3028,  95,  2,	 -18,  9, 13, TRUE, { L_APP, L_APP, 20, 34, 35}
    },
    
    {
		"Kni", "Knight", "騎士","騎  士", APPLY_CON,  OBJ_VNUM_SCHOOL_DAGGER,
		3022,  90,  0,	 -22,   13,  18, FALSE, { 35, 35, 35, 20, 34}
    },    

    {
		"Swd", "Swordsman", "劍客","劍  客", APPLY_DEX,  OBJ_VNUM_SCHOOL_SWORD,
		3022,  95,  0,	 -22,  11, 15, TRUE, { 36, L_APP, 34, 20, 34}
    },

    {
		"Mon", "Monk", "僧侶","僧  侶", APPLY_DEX,  OBJ_VNUM_SCHOOL_SWORD,
		3022,  95,  -2,	 -24,  11, 15, TRUE, { L_APP, 30, L_APP, 20, 30}
    },
    
    {
		"Shm", "Shaman", "巫醫","巫  醫", APPLY_WIS,  OBJ_VNUM_SCHOOL_DAGGER,
		3221,  90,  2,	 -17,   15,  20, TRUE, { 30, 25, L_APP, L_APP, 20}
    },

    {
		"Sor", "Sorcerer", "幻法師","幻法師", APPLY_DEX,  OBJ_VNUM_SCHOOL_SWORD,
		3018,  99,  5,	 -12,  6, 10, TRUE, { 20, 34, L_APP, L_APP, L_APP}
    },
    
    {
		"Lor", "Lord", "領主","領  主", APPLY_STR,  OBJ_VNUM_SCHOOL_DAGGER,
		3003,  95,  0,	 -18,   11,  15, TRUE, { L_APP, 20, L_APP, 30, 34}
    },    

    {
		"Ran", "Ranger", "游俠","游  俠", APPLY_WIS,  OBJ_VNUM_SCHOOL_SWORD,
		3028,  95,  0,	 -23,  8, 12, TRUE, { 35, 33, 20, 30, 35}
    },

    {
		"Wrd", "Warlord", "戰王","戰  王", APPLY_INT,  OBJ_VNUM_SCHOOL_SWORD,
		3221,  85,  -2,	 -14,  10, 20, FALSE, { L_APP, 36, 35, 30, 20}
    },
    
    {
		"Mar", "Martialman", "狂戰士","狂戰士", APPLY_STR,  OBJ_VNUM_SCHOOL_DAGGER,
		3221,  80,  0,	 -17,   16,  24, FALSE, { L_APP, L_APP, 36, 30, 20}
    }
};

#define CLASS_MAGE       0
#define CLASS_CLERIC     1
#define CLASS_THIEF      2
#define CLASS_WARRIOR    3
#define CLASS_PSIONICIST 4

/*
 * Immort Levels
*/
#define L_HER LEVEL_HERO

/*
 * Titles.
 */
char *const title_table [ MAX_CLASS ][ MAX_LEVEL+1 ][ 2 ] =
{
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
	{ "Master of all Magic",	"Master of all Magic"		},
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Legendary Mage Hero",			"Legendary Mage Heroine"			},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},

	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Holy Hero",			"Holy Heroine"			},
	{ "Holy Hero",			"Holy Heroine"			},
	{ "Holy Hero",			"Holy Heroine"			},
	{ "Legendary Holy Hero",			"Legendary Holy Heroine"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Godfather",			"Godmother"			},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},

	{ "Thief Hero",		"Thief Heroine"		},
	{ "Legendary Thief Hero",		"Legendary Thief Heroine"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	//{ "Knight of the Black Rose",	"Baroness of Meteors"		},

//	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
	//{ "Knight of the White Rose",	"Baroness of Meteors"		},
	{ "Warrior Hero",   "Warrior Heroine"},
	{ "Warrior Hero",   "Warrior Heroine"},
	{ "Legendary Warrior Hero",   "Legendary Warrior Heroine"},
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
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Barbarian Hero",   "Barbarian Heroine"},
	{ "Barbarian Hero",   "Barbarian Heroine"},
	{ "Legendary Barbarian Hero",   "Legendary Barbarian Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"			},
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
	{ "Wizard Hero",		"Wizard Herione",		},
	{ "Wizard Hero",		"Wizard Herione",		},
	{ "Wizard Hero",		"Wizard Herione",		},
	{ "Wizard Hero",		"Wizard Herione",		},
	{ "Legendary Wizard Hero",		"Legendary Wizard Herione",		},

	{ "Psionicist Angel",		"Psionicist Angel"		},
	{ "Deity of Psionics",		"Deity of Psionics"		},

	{ "Supreme Psionicst",		"Supreme Psionicist"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Creater",		"Creatress",		},
	{ "Master Creater",		"Mistress Creater",		},
	{ "Psionicist Angel",		"Psionicist Angel"		},
	{ "Deity of Psionics",		"Deity of Psionics"		},

	{ "Supreme Psionicst",		"Supreme Psionicist"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Holy Savant Hero",			"Holy Savant Heroine"			},
	{ "Holy Savant Hero",			"Holy Savant Heroine"			},
	{ "Holy Savant Hero",			"Holy Savant Heroine"			},
	{ "Holy Savant Hero",			"Holy Savant Heroine"			},
	{ "Legendary Holy Savant Hero",			"Legendary Holy Savant Heroine"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Holy Bishop Hero",			"Holy Bishop Heroine"			},
	{ "Holy Bishop Hero",			"Holy Bishop Heroine"			},
	{ "Holy Bishop Hero",			"Holy Bishop Heroine"			},
	{ "Holy Bishop Hero",			"Holy Bishop Heroine"			},
	{ "Legendary Holy Bishop Hero",			"Legendary Holy Bishop Heroine"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Lord of Death",		"Lady of Death"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Knight Hero",   "Knight Heroine"},
	{ "Knight Hero",   "Knight Heroine"},
	{ "Legendary Knight Hero",   "Legendary Knight Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Lord of Swordmaster",   "Lady of Swordmaster"},
	{ "Great Lord of Swordmaster",   "Great Lady of Swordmaster"},
	{ "Grand Swordmaster",   "Grand Swordmaster"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Monk Hero",   "Monk Heroine"},
	{ "Monk Hero",   "Monk Heroine"},
	{ "Legendary Monk Hero",   "Legendary Monk Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Legendary Shaman Hero",   "Legendary Shaman Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"			},
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
	{ "Sorcerer Hero",			"Sorceress Heroine"			},
	{ "Master Sorcerer",			"Master Sorceress"			},
	{ "Greater Master Sorcerer",			"Greater Master Sorcerss"			},
	{ "Grand Sorcerer",			"Grand Sorceress"			},
	{ "Grand Master",			"Grand Mistress"			},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},

	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Guardian of the Realm",			"Guardian of the Realm"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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
	{ "Legendary Ranger Hero",		"Legendary Ranger Heroine"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Warlord Hero",   "Warlord Heroine"},
	{ "Warlord Hero",   "Warlord Heroine"},
	{ "Legendary Warlord Hero",   "Legendary Warlord Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"			},
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

	{ "Baron of the Eastern Plains","Baroness of the Eastern Plains"		},
	{ "Baron of the Western Mountains","Baroness of Western Mountains"	},
	{ "Baron of the Northern Reaches","Baroness of Northern Reaches"		},
	{ "Baron of the Southern Seas",	"Baroness of Southern Seas"		},
	{ "Martialman Hero",   "Martialman Heroine"},
	{ "Martialman Hero",   "Martialman Heroine"},
	{ "Legendary Martialman Hero",   "Legendary Martialman Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "Implementor",		"Implementress"				},
	{ "Implementor",		"Implementress"				}
    }
};

/* 
 * Race types
    size,str,int,wis,dex,con,hp_gain,mana_gain,move_gain,thirst,hunger,nature_def,
    nature_dog,nature_ac,dmg_message,hate,courage,spell,ancient,spirit,fire,cold,
    lightning,earth,wind,saint,dark,poison,nature,good,evil
 */
/*  MAGIC_SPELL,MAGIC_FIRE,MAGIC_COLD, MAGIC_WIND, MAGIC_EARTH, MAGIC_BREATH
    MAGIC_LIGHTNING,MAGIC_SAINT,MAGIC_DARK
  */
/* 
 { name, cname, race_abilities,
   size, {str_mod, int_mod, wis_mod, dex_mod, con_mod}, {hp_gain, mana_gain, move_gain}, {thrist_mod, hunger_mod}, 
   nature_svs{法, 火, 水, 風, 土, 龍, 雷, 聖, 闇}, {nature_dodge, nature_ac}, dmg_message, 
   hate{ ... }, 
   glow, ghigh
*/
const struct race_type race_table [ MAX_RACE ] =
{
	{ "Human", "人類",
		RACE_PC_AVAIL | RACE_WEAPON_WIELD,
			 /* Five ATR  ,  HP MP MV Gain ,  Drink Eat , */
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3,  0,  1,  0,  1,  0,  0,  0,  0, /* Drink Eat */  0,  0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -5,  -4,  -4,  -4,  -4,   0,  -4,   0,   0  },  -60,  -90, "空手一擊",
		"Githyanki Vampire Werewolf Mindflayer Ogre",
		100, 100
	},
	{ "Elf", "精靈",
		RACE_PC_AVAIL | RACE_DETECT_MAGIC | RACE_DETECT_INVIS | RACE_WEAPON_WIELD,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3, -2,  2,  1,  2, -2,  0,  3,  1, /* Drink Eat */  -1,  0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{ -11,  -7,  -7,  -7,  -7,  -2,  -7, -10,   4  },  -80, -100, "空手一擊",
		"Demon Drow Ogre Orc Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin Half-kobold", 
		50, 80
	},
	{ "Dwarf", "矮人",
		RACE_PC_AVAIL | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3,  3, -3,  1, -2,  4,  4,  0,  0, /* Drink Eat */  1,  0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{ -12, -12, -12, -12, -12, -12,   8,  -5,  -5  },   50, -120, "空手一擊",
		"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin Half-kobold",
		100, 200
	},
	{ "Orc", "獸人",
		RACE_PC_AVAIL | RACE_INFRAVISION | RACE_WEAPON_WIELD | RACE_ORC,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_4,  4, -3, -4,  0,  2,  2,  0,  1, /* Drink Eat */  1, 3,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{   5,   0,   0,  -3,   0, -15,  -2,   0,  -4  },  -30, -100, "空手一擊",
		"Elf Dwarf Hobbit Vampire Werewolf Gnome",
		30, 30
	},
	{ "Hobbit", "賀比特",
		RACE_PC_AVAIL | RACE_DETECT_HIDDEN	| RACE_WEAPON_WIELD,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_2, -1, -1,  3,  4, -2,  0,  0,  5, /* Drink Eat */  0, 0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -6,  -6,  -5,  -6,  -5,  -5,  -6, -10, -15  }, -250,  -70, "空手一擊",
		"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin Half-kobold",
		50, 50
	},
	{ "Fairy", "妖精",
		RACE_PC_AVAIL | RACE_FLY | RACE_DETECT_ALIGN | RACE_WEAPON_WIELD | RACE_ANTI_MAGIC,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_1, -4,  4,  2,  2, -4,  0,  4,  1, /* Drink Eat */  0,-1,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -8,  -7,  -7,   8, -10,  -7,  -7,  -8,  -8  }, -170,  -30, "空手一擊",
		"Kobold Minotaur Angel Goblin Dwarf HalfDwarf Half-elf Half-kobold Beholder Ogre",
		50, 50
	},
	{ "Gnome", "地精",
		RACE_PC_AVAIL | RACE_DETECT_ALIGN | RACE_PROTECTION | RACE_WEAPON_WIELD,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_2, -2,  2,  4, -4,  0,  1,  2,  0, /* Drink Eat */  0, 0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{ -10, -10, -10, -10, -10,   5, -10, -20,  20  },  300, -130, "空手一擊",
		"Drow Ogre Orc Kobold Troll Hobgoblin Vampire Werewolf Goblin",
		50, 50
	},
	{ "Drow", "暗黑精靈",
		RACE_PC_AVAIL | RACE_DETECT_MAGIC | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3, -3,  3,  2,  2, -3,  0,  3,  0, /* Drink Eat */  -1, 0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{ -12,  -8,  -8,  -8,  -8,  -8,  -6,   4, -10  },  -80,  -80, "空手一擊",
		"Elf Gnome Githyanki Vampire Werewolf",
		50, 50
	},
	{ "Vampire", "吸血鬼",
		RACE_PC_AVAIL | RACE_FLY | RACE_INFRAVISION | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_VAMPIRE | RACE_UNDEAD,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3,  0,  1, -1,  2, -2,  1,  1,  1, /* Drink Eat */  3,-1,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -7,  15,  -4,  -5,  -4,  -4,  -5,   8, -20  },  -70, -100, "使勁一抓",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Werewolf Goblin Angel Gnome Lizardman",
		30, 200
	},
	{ "Beholder", "眼魔",
		RACE_PC_AVAIL | RACE_FLY | RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_BEHOLDER,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3, -5,  6,  4,  0, -5,  0,  5,  0, /* Drink Eat */  1, 1,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{ -15,  -9,  -9,  -9,  -9,   0,  -9,   0,   0  },  -30,  -70, "輕輕一觸", 
		"Lizardman",
		0, 200
	},
	{ "Lizardman", "蜥蜴人",
		RACE_PC_AVAIL | RACE_SWIM | RACE_WEAPON_WIELD | RACE_LIZARDMAN,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_4,  3, -5, -4,  1,  3,  2,  0,  2, /* Drink Eat */  1, 2,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  10,   0, -15,   0,  -2, -25,   0,   0,   0  },  -50, -180, "空手一擊", 
		"Vampire Werewolf Ghost",
		10, 10
	},
	{ "Horse", "馬",
		RACE_WEAPON_WIELD | RACE_MUTE,
		SIZE_3, 3,-5,-5, 3, 4, 0, 0, 0, 2, 3,
		{8,2,2,2,2,1,0,0,0},-20,-60, "加速衝撞",
		"Kobold Half-kobold Ghost",
		0, 0
	},
	{ "God", "神",
		RACE_WATERBREATH | RACE_FLY | RACE_SWIM | RACE_WATERWALK | RACE_SANCT | RACE_DETECT_MAGIC 
		| RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_5, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0,
		{-15,-15,-15,-15,-15,-15,-15,-20,-20},-200, -200, "雷霆一擊",
		"",
		0, 0
	},
	{ "Demon", "惡魔",
		RACE_REGENERACY | RACE_WEAPON_WIELD | RACE_INFRAVISION | RACE_NO_POISON 
		| RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_UNDEAD,
		SIZE_5, 8, 8, 5, 6, 8, 0, 0, 0, 0, 0,
		{-20,-10,-10,-10,-10,-10,-10,20,-20}, -55, -500, "空手一擊",
		"Angel Elf Dwarf God",
		50, 200
    },
	{ "Angel", "天使",
		RACE_FLY | RACE_PROTECTION | RACE_DETECT_INVIS | RACE_WEAPON_WIELD |RACE_DETECT_MAGIC,
		SIZE_4, 5, 5, 5, 8, 3 ,0, 0, 0, 0, 0,  {-10,-10,-10,-10,-10,-10,-10,-20,20},-150, -60, "空手一擊",
		"Demon",
		0, 0
	},
	{ "Dragon", "龍",
		RACE_FLY | RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_8, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,  {-9,-7,-7,-7,-7,-30,-3,0,0},  -10, -70, "使勁一抓",
		"",
		50 ,300
	},
	{ "Half-elf", "半精靈",
		RACE_INFRAVISION | RACE_DETECT_MAGIC | RACE_WEAPON_WIELD,
		SIZE_3,2, 3, 1, 5,1, 0, 0, 0, 0, 0,  {-6,-5,-5,-6,-5,-6,-5,-2,-2},-100, -100, "空手一擊",
		"Ogre Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin",
		50, 50
	},
	{ "Half-dwarf", "半矮人",
		RACE_WEAPON_WIELD,
		SIZE_3, 1,-1, 0,-1, 1, 0, 0, 0, 0, 0,  {-3,-2,1,1,-2,-1,1,-1,-1}, 10, -80, "空手一擊",
		"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin",
		50, 50
	},
	{ "Half-kobold", "半地靈",
		RACE_DETECT_ALIGN | RACE_WEAPON_WIELD,
		SIZE_2,-1, 1, 2,-2, 0, 0, 0, 0, 0, 0, {4,2,-1,2,-1,1,-2,-1,-1}, 10,110, "空手一擊", 
		"Ogre Orc Giant Troll Hobgoblin",
		50, 50
	},
	{ "Giant", "巨人",
		RACE_WEAPON_WIELD,
		SIZE_7, 4,-2,-2,-3, 3, 0, 0, 0, 2, 4,  {8,3,3,3,3,1,4,0,0}, 30,-20, "重重一拳",
		"Demon Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 200
	},
	{ "Ogre", "巨魔",
		RACE_WEAPON_WIELD,
		SIZE_6, 4,-3,-2,-1, 2, 0, 0, 0, 0, 2,  {4,1,-4,1,1,0,1,0,-2}, 15, -10, "重重一拳",
		"Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 150
	},
	{ "Kobold", "地靈",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_2,-1,-1, 0, 1, 1, 0, 0, 0, 0, 0, {8,4-2,4,-2,2,-1,3,-3},  0,130, "空手一擊",
		"Elf Imp Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome Half-kobold",
		80, 80
	},
	{ "Minotaur",  "牛頭怪",
		RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_6, 2, 0, 0,-1, 1, 0, 0, 0, 0, 3,  {4,-4,4,4,-4,-2,0,3,-3},  -10, -30, "重重一拳",
		"Elf Imp Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		30, 120
	},
	{ "Troll", "食人妖",
		RACE_WEAPON_WIELD,
		SIZE_6, 3,-1,-1,-2, 1, 0, 0, 0, 0, 8,  {-2,0,-2,-2,0,0,0,0,0}, -5,100, "重重一拳",
		"Human Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		30, 150
	},
	{ "Hobgoblin", "大惡鬼",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_4, 1, 0,-1, 0, 0, 0, 0, 0, 0, 0, {6,5,5,5,5,3,-2,0,0},-10, 90, "空手一擊",
		"Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 50
	},
	{ "Insect", "昆蟲",
		RACE_MUTE,
		SIZE_1,-4,-4,-4, 0,-4, 0, 0, 0, 0, 0, {12,8,8,8,8,5,-2,0,0},-60, 80, "用力一咬",
		"",
		0, 0
	},
	{ "Animal", "動物",
		RACE_DETECT_HIDDEN | RACE_MUTE,
		SIZE_3, 1,-3,-1, 1, 1, 0, 0, 0, 0, 0, {8,3,3,3,3,2,1,0,0},-20, 80, "用力一咬",
		"Kobold Half-kobold Ghost",
		0, 0
	},
	{ "Undead", "不死系",
		RACE_INFRAVISION | RACE_DETECT_INVIS | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_UNDEAD,
		SIZE_3, 0, 0, 0, 1,-1, 0, 0, 0,-1,-1,  {0,10,-10,0,-10,8,0,8,-8},15, 75, "輕輕一觸",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Goblin Angel Gnome Animal",
		0, 200
	},
	{ "Harpy", "人面鳥",
		RACE_FLY | RACE_DETECT_INVIS | RACE_WEAPON_WIELD,
		SIZE_3, 0,-1,-1, 2, 0, 0, 0, 0, 0, 0,  {-3,5,5,5,5,2,-2,3,-3}, -5, 85, "使勁一抓",
		"Human Imp Elf Half-elf Dwarf Half-dwarf Hobbit Gnome",
		30, 30
	},
	{ "Bear", "熊",
		RACE_WATERWALK | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_5, 1, 0, 0,-1, 1, 0, 0, 0, 0, 3,  {4,2,2,2,2,1,0,0,0},  0, 35, "熊掌一抓",
		"",
		0, 0
	},
	{ "Githyanki", "夜靈",
		RACE_WEAPON_WIELD,
		SIZE_3, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0,  {-6,6,-6,-6,6,2,-10,5,-5}, -5, 75, "空手一擊",
		"Mindflayer",
		50, 150
	},
	{ "Elemental", "元素",
		RACE_NO_POISON,
		SIZE_4, 1, 0, 0, 0, 1, 0, 0, 0,-2,-2,  {0,-12,-12,-12,-12,0,-10,0,0},-30, 40, "猛力一撞",
		"",
		0, 0
	},
	{ "Bat", "蝙蝠",
		RACE_FLY | RACE_INFRAVISION | RACE_MUTE,
		SIZE_1,/* Five ATR */ -2, 0, -1, 4,-3, /*HP MP MV */ 0, 0, 4, /* Drink Eat */0, 0, 
		/* 法, 火, 水, 風, 土, 龍, 雷, 聖, 闇 */
		{   4,  6,  6,  6,  6,  2,  0,  0,  0},-50,120, "用力一咬",
		"",
		0, 0
	},
	{ "Plant", "植物",
		RACE_MUTE | RACE_WEAPON_WIELD,
		SIZE_2, 0, 0, 0,-2, 1, 0, 0, 0,-1,-1,  {-10,20,10,-10,-20,10,4,0,0}, 100,150, "莖葉一揮",
		"",
		0, 0
	},
	{ "Rat", "鼠", RACE_PASSDOOR | RACE_MUTE,
		SIZE_1,-1, 0, 0, 2,-1, 0, 0, 0, 0, 0, {6,5,5,5,5,3,0,0,-2},-20, 95, "用力一咬",
		"",
		0, 0
	},
    { "Werewolf",  "狼人",
		RACE_PC_AVAIL | RACE_INFRAVISION | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD | RACE_WEREWOLF,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_4,  2, -3,  0,  2,  3,  2,  0,  2, /* Drink Eat */  0, 3,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -3,   5,  -3,  -3,  -3,  -3,   5,   0,   0  },  -50, -100, "使勁一抓",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Goblin Angel Gnome",
		100, 100
	},
	{ "Goblin", "小惡鬼",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_2,-1,-1,-1, 1, 0, 0, 0, 0, 0, 0, {6,5,5,5,5,3,-1,0,0}, -5,100, "空手一擊",
		"Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 50
	},
	{ "Arachnid",  "蜘蛛",
		RACE_NO_ABILITIES | RACE_WEAPON_WIELD | RACE_MUTE,
		SIZE_2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, {0,5,0,0,0,2,-2,1,-1},-20,100, "用力一咬",
		"",
		0, 0
	},
	{ "Mind-flayer","奪心魔",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_3, 1, 2, 1,-1, 0, 0, 6, 0, 0, 0, {-6,-6,6,6,-6,2,-3,3,-3},-30, 90, "空手一擊",
		"Githyanki",
		100, 100
	},
	{ "Object", "物品",
		RACE_WATERBREATH | RACE_MUTE | RACE_NO_POISON,
		SIZE_2, 3, 0, 0, 0, 3, 0, 0, 0,-2,-2, {-20,10,10,10,10,10,3,0,0} ,100,170, "用力一撞",
		"",
		0, 0
	},
	{ "Mist", "霧",
		RACE_FLY | RACE_PASSDOOR | RACE_NO_POISON | RACE_WEAPON_WIELD,
		SIZE_0, 0, 0, 0, 3, 0, 0, 0, 0,-2,-2,  {-5,-5,-5,10,-5,-5,-3,0,0},-50, 140, "霧氣一噴",
		"",
		0, 0
	},
	{ "Snake", "蛇",
		RACE_MUTE,
		SIZE_2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  {0,3,3,3,3,2,0,0,0},-10, 80, "用力一咬",
		"",
		0, 0
	},
	{ "Worm", "蟲",
		RACE_PASSDOOR | RACE_MUTE | RACE_WEAPON_WIELD,
		SIZE_8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {6,3,3,-10,-10,5,-1,0,0},  5,115, "用力一咬",
		"",
		0, 0
	},
	{ "Fish", "魚",
		RACE_WATERBREATH | RACE_SWIM | RACE_MUTE,
		SIZE_1, 0, 0, 0, 2, 0, 0, 0, 0,-2, 0,  {6,10,10,-10,-10,0,8,0,0},-10, 80, "用力一掃",
		"",
		0, 0
	},
	{ "Hydra", "九頭海蛇怪",
		RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_8, 2, 0, 0,-1, 2, 0, 0, 0, 0, 0,  {-5,-4,-4,-4,-4,-4,4,0,0}, -10, 90, "用力一咬",
		"",
		50, 200
	},
	{ "Lizard", "蜥蜴",
		RACE_MUTE,
		SIZE_1,-1, 0, 0, 1, 0, 0, 0, 0, 0, 0, {12,-3,-3,-3,-3,-1,0,0,0},-15, 85, "尾巴一掃",
		"",
		0, 0
	},
	{ "Pixie",     "小妖精",
		RACE_FLY | RACE_WEAPON_WIELD,
		SIZE_1,-5, 4, 2, 3,-4, 0, 0, 0, 0,-1, {-4,2,2,2,2,1,-3,0,0},-45, 65, "空手一擊",
		"Ogre Kobold Minotaur Goblin Hobgoblin Dwarf HalfDwarf Half-elf Half-kobold Beholder",
		30, 30
	},
	{ "Elementx", "元素生物", RACE_WEAPON_WIELD,
		SIZE_4, 1, 0, 0, 0, 1, 0, 0, 0,-2,-2,  {5,-8,-8,-8,-8,-8,0,0,0},-20, 50, "空手一擊",
		"",
		30, 30
	},
	{ "Phantom", "幻影",
		RACE_FLY | RACE_PASSDOOR | RACE_WEAPON_WIELD | RACE_NO_POISON,
		SIZE_0, 0, 0, 0, 3, 0, 0, 0, 0,-2,-2,  {-8,0,3,6,-10,-5,0,0,0},-50, 40, "霧氣一噴",
		"",
		0, 0
	},
	{ "Statue", "雕像",
		RACE_MUTE | RACE_WEAPON_WIELD | RACE_NO_POISON,
		SIZE_5, 1, 0, 0,-5, 4, 0, 0, 0,-2,-2,  {-10, 0, 0,-10,10,10,-10,0,0}, 50, 0, "猛烈一撞",
		"",
		0, 0
	},
	{ "Robot", "機械",
		RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_ANTI_MAGIC,
		SIZE_4, 0, 0, 0, 0, 0, 0, 0, 0,-2,-2,  {-10,5,5,5,5,5,5,0,0}, 10, 70, "猛烈一擊",
		"",
		0, 0
	},
	{ "Ghost", "鬼魂",
		RACE_INVIS | RACE_PASSDOOR | RACE_FLY | RACE_INFRAVISION | RACE_DETECT_INVIS 
		| RACE_UNDEAD | RACE_WEAPON_WIELD,
		SIZE_0,-1, 0, 0, 2,-1, 0, 0, 0,-1,-1, {-5,3,3,3,3,2,0,8,-8},-30, 70, "輕輕一觸",
		"Animal Drow Kobold Lizardman Mindflayer Human Gnome Hobbit",
		0, 0
	},

	{ "Halfling", "半身人",
		RACE_WEAPON_WIELD,
		SIZE_2, 1,-1, 0, 0, 0, 0, 0, 0, 0, 0,  {0,0,0,0,0,0,1,-5,-5},-10, 80, "空手一擊",
		"",
		100, 100
	},

	{ "Beast", "獸族",
		RACE_INFRAVISION | RACE_DETECT_HIDDEN | RACE_REGENERACY | RACE_WEAPON_WIELD,
		SIZE_3, 3,0, 3, 5, 5, 3, 0, 0, 1, 3, {2,5,5,-5,-5,-2,-5,0,0},-30,  -50, "利爪亂刨",
		"Kobold Half-kobold Ghost Orc Beholder",
		0, 0
	},

	{ "Centaur",   "人馬",
		RACE_PC_AVAIL | RACE_DETECT_INVIS | RACE_WEAPON_WIELD | RACE_INFRAVISION | RACE_CENTAUR,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_5,  1, -3, -2,  3,  1,  1,  0,  1, /* Drink Eat */  2, 1,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -4,  -4,   5,  -4,  10,   0,  -4,   0,   0  },  -70,  -70, "鐵蹄一蹬",
		"Kobold Ghost Vampire Orc Troll Goblin",
		50, 50
	},
	{ "Dinosaur", "恐龍",
		RACE_WEAPON_WIELD|RACE_MUTE|RACE_INFRAVISION,
		SIZE_9, 3, -2, -2, 0, 3, 0, 0, 0, 0, 0,  {2,-3,-3,-3,-3,-10,0,0,0},  50, -100, "使勁一抓",
		"",
		0, 0
	},
	{ "Drake", "龍獸",
		RACE_FLY | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_7, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0,  {0,-2,-2,-2,-2,-4,-4,0,0},  0, 20, "使勁一抓",
		"",
		50, 50
	},
	{ "Wyrm", "蛟龍",
		RACE_INFRAVISION|RACE_DETECT_HIDDEN|RACE_REGENERACY | RACE_WEAPON_WIELD,
		SIZE_8, 4, 1, 1, 0, 3, 0, 0, 0, 0, 0,  {0,-5,-5,-5,-5,-7,0,0,0},  20, -20, "巨尾一掃",
		"",
		50, 200
	},
	{ "Wyvern", "飛龍",
		RACE_FLY |  RACE_WEAPON_WIELD,
		SIZE_8, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0,  {-2,-2,-2,-2,-2,-4,4,0,0}, -10, 50, "使勁一抓",
		"",
		50, 150
	},
	{ "Eagle", "鷹",
		RACE_FLY,
		SIZE_3, 0,-1,-1, 2, 0, 0, 0, 0, 0, 0,  {-3,5,5,5,5,-2,0,0}, -5, 85, "使勁一啄",
		"",
		30, 30
	},
	{ "Imp", "妖精",
		RACE_FLY | RACE_WEAPON_WIELD,
		SIZE_2,-4, 4, 2, 2,-4, 0, 4, 0, 0,-1, {-8,5,5,5,5,4,-1,3,-3},-60, 120, "空手一擊",
		"Kobold Minotaur Angel Goblin Dwarf HalfDwarf Half-elf Half-kobold Beholder",
		50, 50
	},
	{ "Merfolk", "魚人",
		RACE_WEAPON_WIELD | RACE_WATERBREATH | RACE_SWIM | RACE_WATERWALK,
		SIZE_3, 0, 1, 0, 1, 0, 0, 0, 0, -1, 0, {1, 5, -5, 5, -5, -5, 3,0,0}, -20, 0, "空手一擊",
		"Vampire Lizardman Drow Dinosaur Kuo-toa", 50, 50
	},
	{ "Kuo-toa", "寇濤魚人",
		RACE_WEAPON_WIELD | RACE_WATERBREATH | RACE_INFRAVISION | RACE_SWIM | RACE_WATERWALK,
		SIZE_4, 2, -1, -1, 0, 1, 0, 0, 0, -1, 0, {1, 5, -5, 5, -5, -5, 3,0,0}, 0, -20, "空手一擊",
		"Elf Dinosaur Merfolk Drow",
		50, 50
	},
	{ "Bachi", "北七",
		RACE_INVIS | RACE_INFRAVISION | RACE_NO_POISON | RACE_PASSDOOR | RACE_DETECT_INVIS
		| RACE_DETECT_HIDDEN | RACE_DETECT_ALIGN | RACE_FLY | RACE_SANCT | RACE_WEAPON_WIELD
		| RACE_PROTECTION | RACE_DETECT_MAGIC | RACE_REGENERACY | RACE_ANTI_MAGIC | RACE_WATERWALK
		| RACE_WATERBREATH,
		SIZE_9, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, {20,20,20,20,20,20,20,20,20},-500,-500, "隨地吐痰",
		"",
		100, 1000
	},
	{ "Mummy", "木乃伊",
		RACE_PC_AVAIL | RACE_REGENERACY | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_UNDEAD ,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3,  3, -1,  1, -1, -2,  3,  0,  0, /* Drink Eat */  -1, 0,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -15, 15,  -8,  -8, -15,  -10,  0,  10, -25  },  -40,  -80, "使勁一抓",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Minotaur Hobgoblin Goblin Angel Gnome",
		30, 200
	},

	{ "Gargoyle", "石像鬼",
		RACE_PC_AVAIL | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_UNDEAD | RACE_FLY | RACE_GARGOYLE,
			 /* 力, 智, 慧, 敏, 體, HP, MP, MV,*/
		SIZE_3, -2,  1,  1,  3, -3,  1,  0,  1, /* Drink Eat */  -1, 2,
		/* 法,  火,  水,  風,  土,  龍,  雷,  聖,  闇       DC,   AC, */
		{  -6,  15,  -5,  10,  -3,  -5,  15,  10, -10  },  -60,  -30, "使勁一抓",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Minotaur Hobgoblin Goblin Angel Gnome",
		30, 200
	}
};


/*
 * Drunkeness string changes.
 */
const struct struckdrunk drunk [ ] =
{
	{ 3, 10,
	{ "a", "a", "a", "A", "aa", "ah", "Ah", "ao", "aw", "oa", "ahhhh" }	},
	{ 8, 5,
	{ "b", "b", "b", "B", "B", "vb" }									},
	{ 3, 5,
	{ "c", "c", "C", "ch", "sj", "zj" }									},
	{ 5, 2,
	{ "d", "d", "D" }													},
	{ 3, 3,
	{ "e", "e", "eh", "E" }												},
	{ 4, 5,
	{ "f", "f", "ff", "fff", "fFf", "F" }								},
	{ 8, 3,
	{ "g", "g", "G", "jeh" }											},
	{ 9, 6,
	{ "h", "h", "hh", "hhh", "Hhh", "HhH", "H" }						},
	{ 7, 6,
	{ "i", "i", "Iii", "ii", "iI", "Ii", "I" }							},
	{ 9, 5,
	{ "j", "j", "jj", "Jj", "jJ", "J" }									},
	{ 7, 3,
	{ "k", "k", "K", "kah" }											},
	{ 3, 2, 
	{ "l", "l", "L" }													},
	{ 5, 8,
	{ "m", "m", "mm", "mmm", "mmmm", "mmmmm", "MmM", "mM", "M" }		},
	{ 6, 6,
	{ "n", "n", "nn", "Nn", "nnn", "nNn", "N" }							},
	{ 3, 6,
	{ "o", "o", "ooo", "ao", "aOoo", "Ooo", "ooOo" }					},
	{ 3, 2,
	{ "p", "p", "P" }													},
	{ 5, 5,
	{ "q", "q", "Q", "ku", "ququ", "kukeleku" }							},
	{ 4, 2,
	{ "r", "r", "R" }													},
	{ 2, 5,
	{ "s", "ss", "zzZzssZ", "ZSssS", "sSzzsss", "sSss" }				},
	{ 5, 2,
	{ "t", "t", "T" }													},
	{ 3, 6,
	{ "u", "u", "uh", "Uh", "Uhuhhuh", "uhU", "uhhu" }					},
	{ 4, 2,
	{ "v", "v", "V" }													},
	{ 4, 2,
	{ "w", "w", "W" }													},
	{ 5, 6,
	{ "x", "x", "X", "ks", "iks", "kz", "xz" }							},
	{ 3, 2,
	{ "y", "y", "Y" }													},
	{ 2, 8,
	{ "z", "z", "ZzzZz", "Zzz", "Zsszzsz", "szz", "sZZz", "ZSz", "zZ"}	}
};


/*
 * Attribute bonus tables.
 */
const struct str_app_type str_app [32] =
{
  /*  tohit  todam   carry  wield  wield2  twohand  */
    {  -60,   -5,      0,     0,     0,      0   },     /* 0 */
    {  -50,   -5,      3,     1,     0,      2   },     /* 1 */
    {  -40,   -4,      3,     2,     1,      4   },
  /*----------------------------------------------*/
    {  -30,   -3,     10,     3,     1,      5   },     /* 3  min str */
    {  -25,   -2,     25,     4,     2,      6   },
    {  -20,   -1,     55,     5,     2,      8   },     /* 5 */
    {  -15,   -1,     80,     6,     3,      9   },
    {  -10,    0,    100,     7,     4,     11   },     /* 7 */
    {    0,    0,    150,     8,     5,     12   },
    {   10,    0,    200,     9,     6,     14   },     /* 9 */
    {   20,    1,    250,    10,     7,     15   },
    {   25,    1,    300,    15,     8,     25   },     /* 11*/
    {   30,    2,    350,    20,     9,     30   },
    {   35,    2,    400,    25,    10,     38   },     /* 13 */
    {   40,    3,    430,    30,    15,     45   },
    {   45,    4,    460,    35,    20,     55   },     /* 15 */
    {   50,    5,    500,    40,    25,     65   },
    {   55,    6,    550,    45,    30,     75   },     /* 17 */
    {   60,    7,    600,    50,    35,     85   },
    {   65,    8,    650,    55,    40,     95   },     /* 19 */
    {   70,   10,    700,    60,    45,    100   },
    {   75,   12,    750,    65,    50,    120   },     /* 21 */
    {   80,   14,    800,    70,    55,    130   },
    {   85,   16,    850,    75,    60,    140   },     /* 23 */
    {   90,   18,    900,    80,    65,    150   },
    {  100,   21,    999,    90,    70,    160   },     /* 25  max str */
  /*----------------------------------------------*/
    {  110,   23,   1100,   100,    75,    180   },
    {   21,   24,   1200,   110,    85,    200   },
    {   24,   27,   1300,   120,    95,    230   },
    {   27,   30,   1400,   130,   105,    260   },
    {   31,   35,   1600,   150,   115,    300   },     /* 30 */
    {   21,   40,   1200, 999999, 999999, 999999 }//,  comma marked at 2022/12/06
};

const struct int_app_type int_app [31] =
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 85 },
    { 99 },/* 25 */
    { 99},
    { 99},
    { 99},
    { 99},
    { 99} /* 30 */
};

const struct wis_app_type wis_app [31] =
{
    { 0,-12 },	/*  0 */
    { 0,-11 },	/*  1 */
    { 0 ,-10},
    { 0,-9 },	/*  3 */
    { 0,-8 },
    { 1,-7 },	/*  5 */
    { 1 ,-6},
    { 1 ,-5},
    { 1 ,-4},
    { 2 ,-3},
    { 2 ,-2},	/* 10 */
    { 3 ,-1},
    { 3 ,0},
    { 4 ,1},
    { 4 ,2},
    { 5 ,3},	/* 15 */
    { 5 ,4},
    { 5 ,5},
    { 6 ,6},	/* 18 */
    { 6 ,7},
    { 6 ,8},	/* 20 */
    { 7 ,9},
    { 7 ,10},
    { 7 ,11},
    { 7 ,13},
    { 8 ,15},	/* 25 */
    { 8 ,110},
    { 8 ,120},
    { 8 ,130},
    { 8 ,140},
    { 9 ,150}	/* 30 */
};

const struct dex_app_type dex_app [31] =
{
    {   50, -12 },    /* 0 */
    {   40, -11 },
    {   30, -10 },
    {   20,  -9 },    /* 3 */
    {   10,  -8 },
    {    0,  -7 },    /* 5 */
    {  -10,  -6 },
    {  -20,  -5 },
    {  -30,  -4 },
    {  -40,  -3 },
    {  -50,  -2 },    /* 10 */
    {  -70,  -1 },
    {  -90,   0 },
    { -110,   1 },
    { -130,   2 },
    { -150,   3 },    /* 15 */
    { -170,   4 },
    { -195,   5 },
    { -220,   6 },
    { -245,   7 },
    { -270,   8 },    /* 20 */
    { -295,   9 },
    { -320,  10 },
    { -350,  11 },
    { -380,  13 },
    { -410,  15 },    /* 25 */
    { -450, 110 },
    { -490, 120 },
    { -530, 130 },
    { -570, 140 },
    { -610, 150 }     /* 30 */
};

/* ac 從 con 10 開始增加 25 2022/02/23 */
const struct con_app_type con_app [31] =
{
    { -4, 20,   20 },    /* Con  0 , AC Increased by 10 */ 
    { -3, 25,   10 },
    { -3, 30,    0 },    /* Con  3 */
    { -3, 35,  -10 },
    { -2, 40,  -20 },
    { -2, 45,  -35 },    /* Con  5 , AC Increased by 15 */
    { -1, 50,  -50 },
    { -1, 55,  -65 },
    {  0, 60,  -80 },
    {  0, 65,  -95 },
    {  0, 70, -115 },	  /* Con 10 , AC Increased by 20 */
    {  0, 75, -135 },
    {  1, 80, -155 },
    {  2, 85, -175 },
    {  3, 88, -195 },
    {  4, 90, -210 },    /* Con 15 , AC Increased by 25 */ 
    {  5, 95, -235 },
    {  6, 97, -260 },
    {  7, 99, -285 },
    {  8, 99, -310 },
    {  9, 99, -340 },    /* Con 20 , AC Increased by 30 */ 
    { 10, 99, -370 },
    { 11, 99, -400 },
    { 12, 99, -430 },
    { 13, 99, -460 },
    { 14, 99, -500 },    /* Con 25 , AC Increased by 40 */ 
    { 15, 99, -540 },
    { 16, 99, -580 },
    { 17, 99, -620 },
    { 18, 99, -660 },
    { 19, 99, -700 }
};


/*
 * Liquid properties.
 * Used in world.obj.
 */
/* This liquid table is replaced by chinese shape table.
#ifdef 0
const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
    { "water",			"clear",	{  0, 0, 10 }	},  //	0
    { "beer",			"amber",	{  3, 2,  5 }	},
    { "wine",			"rose",		{  5, 2,  5 }	},
    { "ale",			"brown",	{  2, 2,  5 }	},
    { "dark ale",		"dark",		{  1, 2,  5 }	},

    { "whisky",			"golden",	{  6, 1,  4 }	},  //	5 
    { "lemonade",		"pink",		{  0, 1,  8 }	},
    { "firebreather",		"boiling",	{ 10, 0,  0 }	},
    { "local specialty",	"everclear",	{  3, 3,  3 }	},
    { "slime mold juice",	"green",	{  0, 4, -8 }	},

    { "milk",			"white",	{  0, 3,  6 }	},  // 10 
    { "tea",			"tan",		{  0, 1,  6 }	},
    { "coffee",			"black",	{  0, 1,  6 }	},
    { "blood",			"red",		{  0, 2, -1 }	},
    { "salt water",		"clear",	{  0, 1, -2 }	},

    { "cola",			"cherry",	{  0, 1,  5 }	}   // 15 
};
#endif
*/
const struct liq_type liq_table [LIQ_MAX] =
{
	                                                    /* drunk hungry thirsty */
  {  "清水",       "water",             "透明無色的",     {   0,    0,    5  } }, /* 0 */
  {  "啤酒",       "beer",              "淡棕色的",       {   4,    1,    3  } },
  {  "酒",         "wine",              "玫瑰色的",       {  10,    0,    3  } },
  {  "麥酒",       "ale",               "棕色的",         {   4,    1,    3  } },
  {  "黑麥酒",     "dark-ale",          "暗棕色的",       {   4,    1,    3  } },
  {  "威士忌",     "whisky",            "琥珀色的",       {  20,    0,    0  } }, /* 5 */
  {  "檸檬水",     "lemonade",          "濁白色的",       {   0,    0,    5  } },
  {  "烈酒",       "firebreather",      "辛辣的",         {  30,    0,    0  } },
  {  "當地特產",   "local speciality",  "清澈的",         {   9,    1,    2  } },
  {  "青草汁",     "slime mold juice",  "深綠色的",       {   0,    4,   -1  } },
  {  "牛奶",       "milk",              "白色的",         {   0,    1,    3  } }, /* 10 */
  {  "茶",         "tea",               "澄清淡黃的",     {   0,    0,    5  } },
  {  "咖啡",       "coffee",            "暗紅色的",       {   0,    0,    4  } },
  {  "血",         "blood",             "鮮紅色的",       {   0,    4,   -1  } },
  {  "鹽水",       "salt water",        "透明無色的",     {   0,    0,   -5  } },
  {  "可樂",       "cola",              "櫻桃色的",       {   0,    2,    1  } }, /* 15 */
  {  "珍珠奶茶",   "bubble milk tea",   "黃褐有料的",     {   0,    2,    1  } },
  {  "奶茶",       "milk tea",          "黃褐色",         {   0,    2,    4  } },
  {  "綠茶",       "green tea",         "黃綠色的",       {   0,    0,    5  } },
  {  "紅茶",       "black tea",         "深褐色的",       {   0,    0,    5  } },
  {  "可爾必斯",   "calpis",            "乳白色的",       {   0,    2,    1  } }, /* 20 */
  {  "濃湯",       "soup",              "濃稠的",         {   0,    4,    1  } },
  {  "柳橙汁",     "orange juice",      "橙色的",         {   0,    2,    2  } },
  {  "甜湯",       "sweet soup",        "香甜濃郁的",     {   0,    3,    2  } },
  {  "冰砂",       "ice",               "冰涼的",         {   0,    0,    5  } }, /* 24 */
  {  "椰子汁",     "coconut juice",     "透明混濁的",     {   0,    1,    5  } },
  {  "粥水",       "porridge",          "混濁濃稠的",     {   0,    2,    3  } },
  {  "果汁",       "juice",             "香甜的",         {   0,    1,    3  } },
  {  "乳製飲品",   "dairy beverage",    "乳白香濃的",     {   0,    2,    2  } },
  {  "青草茶",     "grass tea",         "深褐色的",       {   0,    1,    3  } }  /* 29 */
};


#define SKILL_APP 440
#define SKILL_LIMIT( level, learn , sklvlimit ) ( (learn) * 1000 + (level + 1)*10 + sklvlimit )

const struct skill_type skill_table [MAX_SKILL] =
{

    /*
     * Magic spells.
     */
    {
        "reserved", "保留", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            0,   TAR_IGNORE,  POS_STANDING,
            NULL,   0,  0,
            "",   "", ATTR_ALL, {0,0,0,0,0} // {STR,DEX,INT,WIS,CON}
    },
    
    {
        "acid blast", "酸爆術", { SKILL_LIMIT(20,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(20,80,6), SKILL_LIMIT(25,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   20, 2,
            "酸爆術",  "!Acid Blast!", ATTR_ALL, {5,3,1,2,5}
    },

    {
        "armor", "護甲術", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(1,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP },
            spell_armor,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   25, 2,
            "",   "你感覺防護力下降了.", ATTR_ALL, {5,5,4,4,5}

    },

    {
        "bless", "祝福術", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), SKILL_LIMIT(2,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_APP, SKILL_LIMIT(3,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_bless,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_bless,   25, 2,
            "",   "你覺得身上的祝福消失了. ", ATTR_ALL, {8,7,7,7,10}

    },

    {
        "blindness", "盲目術", { SKILL_LIMIT(8,85,6), SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_APP, 
            SKILL_LIMIT(30,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(3,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_blindness,  20, 2,
            "",   "你眼前突然一亮, 你又可以看見一切了!", ATTR_ALL, {7,6,6,6,7}

    },

    {
        "paralyze", "麻痺術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_paralyze, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_paralyze,  35, 4,
            "",   "你的四肢又能活動自如了.", ATTR_ALL, {8,8,6,7,8}

    },

    {
        "breathe water", "水中呼吸術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_breathe_water, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_breathe_water,  5, 4,
            "窒息",  "你不能在水中呼吸了.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "burning hands", "灼熱之掌", { SKILL_LIMIT(5,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(2,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_burning_hands, 25, 4,
            "灼熱之掌", "你身上燃燒的火焰漸漸熄滅", ATTR_ALL, {2,2,2,1,3}

    },

    {
        "call lightning", "閃電召喚術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_call_lightning, TAR_IGNORE,  POS_FIGHTING,
            NULL,   18, 8,
            "閃電召喚術", "你身旁的雷光消散了.", ATTR_ALL, {3,2,1,2,3}

    },

    {
        "cause critical", "高度傷害術", { SKILL_APP, SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   200, 0,
            "高度傷害術",  "!Cause Critical!", ATTR_ALL, {3,2,2,1,3}

    },

    {
        "cause light", "輕度傷害術", { SKILL_APP, SKILL_LIMIT(1,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   50, 0,
            "輕度傷害術",  "!Cause Light!", ATTR_ALL//, {2,2,1,1,2}

    },

    {
        "cause serious", "中度傷害術", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), SKILL_LIMIT(2,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   100, 0,
            "中度傷害術",  "!Cause Serious!", ATTR_ALL, {3,2,1,1,1}

    },

    {
        "change sex", "性別變更術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 2,
            "",   "你覺得身體起了些異樣的變化.", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "charm person", "魅惑術", { SKILL_LIMIT(15,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(8,60,6), SKILL_LIMIT(15,80,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(6,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_charm_person, 35, 0,
            "",   "你對自己又有信心了.", ATTR_ALL, {5,4,3,3,5}

    },

    {
        "chill touch", "寒霜之觸", { SKILL_LIMIT(3,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(5,90,6), SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_chill_touch,   30, 2,
            "寒霜之觸", "你覺得溫暖多了.", ATTR_ALL, {5,4,4,3,4}

    },

    {
        "colour spray", "冰霧術", { SKILL_LIMIT(12,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(7,99,6), SKILL_LIMIT(7,99,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(6,99,6), SKILL_APP, 
            SKILL_LIMIT(31,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(7,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_colour_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   45, 4,
            "冰霧術",  "你覺得溫暖多了.", ATTR_ALL, {5,5,4,4,5}

    },

    {
        "cone of silence", "沈默力場", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cone_of_silence, TAR_IGNORE,  POS_FIGHTING,
            NULL,   35, 12,
            "",   "!Cone of Silence!", ATTR_ALL, {4,3,2,2,3}

    },

    {
        "continual light", "魔法光球術", { SKILL_LIMIT(4,85,6), SKILL_LIMIT(2,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP },
            spell_continual_light, TAR_IGNORE,  POS_STANDING,
            NULL,   7, 0,
            "",   "!Continual Light!", ATTR_ALL, {3,2,1,1,3}

    },

    {
        "control weather", "呼風喚雨術", { SKILL_LIMIT(10,85,6), SKILL_LIMIT(15,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(8,90,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(14,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_control_weather, TAR_IGNORE,  POS_STANDING,
            NULL,   25, 20,
            "",   "!Control Weather!", ATTR_ALL, {3,2,1,2,2}

    },

    {
        "create food", "食物製造術", { SKILL_APP, SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(1,90,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_create_food, TAR_IGNORE,  POS_STANDING,
            NULL,   5, 20,
            "",   "!Create Food!", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "create spring", "造泉術", { SKILL_LIMIT(10,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_create_spring, TAR_ARG,  POS_STANDING,
            NULL,   20, 20,
            "",   "!Create Spring!", ATTR_ALL, {2,1,1,1,2}

    },

    {
        "create water", "造水術", { SKILL_APP, SKILL_LIMIT(2,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(1,90,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_create_water, TAR_OBJ_INV,  POS_STANDING,
            NULL,   5, 0,
            "",   "!Create Water!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "cure blindness", "盲目治療術", { SKILL_APP, SKILL_LIMIT(4,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(9,90,6), 
            SKILL_APP, SKILL_LIMIT(2,95,6), SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP },
            spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   5, 4,
            "",   "!Cure Blindness!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "cure critical", "高級療傷術", { SKILL_APP, SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(12,90,6), 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP },
            spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   25, 0,
            "高級療傷術",   "你的高級療傷效果消失了！", ATTR_ALL, {3,2,2,1,3}

    },

    {
        "cure light", "初級療傷術", { SKILL_APP, SKILL_LIMIT(1,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   8, 0,
            "初級療傷術",   "!Cure Light!", ATTR_ALL, {2,1,1,1,1}

    },

    {
        "cure poison", "解毒術", { SKILL_APP, SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP },
            spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   5, 4,
            "",   "!Cure Poison!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "cure serious", "中級療傷術", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_APP, SKILL_LIMIT(3,95,6), SKILL_LIMIT(9,95,6), SKILL_APP, SKILL_APP },
            spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 0,
            "中級療傷術",   "!Cure Serious!", ATTR_ALL, {2,2,2,1,1}

    },

    {
        "curse", "詛咒術", { SKILL_LIMIT(12,85,6), SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_APP, SKILL_LIMIT(8,99,6), SKILL_APP, 
            SKILL_LIMIT(28,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(9,90,6), 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_curse,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_curse,  25, 2,
            "詛咒術",  "你覺得詛咒的陰霾已經離你而去了. ", ATTR_ALL, {7,6,5,5,7}

    },

    {
        "destroy cursed", "詛咒解除術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_destroy_cursed, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   20, 10,
            "",   "!destroy cursed!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "detect evil", "邪惡偵察術", { SKILL_APP, SKILL_LIMIT(4,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_detect_evil, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "你視線內的紅色邪氣消失了.", ATTR_ALL, {1,1,1,1,1}

    },

    {
        "detect hidden", "藏匿偵察術", { SKILL_APP, SKILL_LIMIT(7,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP },
            spell_detect_hidden, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "你對周遭的警覺力下降了. ", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "detect invis", "隱形偵察術", { SKILL_LIMIT(2,85,6), SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_detect_invis, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "你無法再看見隱形的事物了.", ATTR_ALL, {2,2,1,1,1}

    },

    {
        "detect magic", "魔法偵察術", { SKILL_LIMIT(2,85,6), SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_detect_magic, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "你不能感應魔法了.", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "detect poison", "毒物偵察術", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(5,95,6), 
            SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_detect_poison, TAR_OBJ_INV,  POS_STANDING,
            NULL,   5, 0,
            "",   "你對毒物的察覺力下降了.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "dispel evil", "除魔術", { SKILL_APP, SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   30, 2,
            "除魔術",  "!Dispel Evil!", ATTR_ALL, {2,2,1,2,3}

    },

    {
        "dispel good", "除聖術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dispel_good, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   30, 2,
            "除聖術",  "!Dispel Evil!", ATTR_ALL, {2,2,1,2,3}
                                                                                
    },

    {
        "dispel magic", "解除魔法", { SKILL_LIMIT(20,85,6), SKILL_LIMIT(15,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(5,85,6), SKILL_LIMIT(10,90,6), SKILL_APP, 
            SKILL_LIMIT(36,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dispel_magic, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 8,
            "",   "!Dispel Magic!", ATTR_ALL, {4,3,4,3,4}

    },

    {
        "earthquake", "地震術", { SKILL_APP, SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earthquake, TAR_IGNORE,  POS_FIGHTING,
            NULL,   65, 6,
            "地震術",  "你的四肢又能活動自如了.", ATTR_ALL, {4,4,3,3,4}

    },

    {
        "enchant weapon", "武器增強術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_enchant_weapon, TAR_OBJ_INV,  POS_STANDING,
            NULL,   100, 12,
            "",   "你身上的武器增強術消失了.", ATTR_ALL, {8,8,7,7,9}

    },

    {
        "energy drain", "吸精術", { SKILL_LIMIT(14,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_LIMIT(22,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   60, 4,
            "吸精術",  "你的精力不再快速消散了.", ATTR_ALL, {4,4,3,4,5}

    },

    {
        "exorcise", "驅邪術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_exorcise, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   35, 4,
            "",   "你身上的驅邪術消失了.", ATTR_ALL, {3,3,2,1,3}

    },

    {
        "faerie fire", "天使之火", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_LIMIT(16,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_faerie_fire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_faerie_fire,   15, 2,
            "天使之火",  "你身上的粉紅色光芒漸漸消失了.", ATTR_ALL, {6,5,4,4,6}

    },

    {
        "faerie fog", "天使之霧", { SKILL_LIMIT(10,85,6), SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(7,99,6), SKILL_LIMIT(7,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(8,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_faerie_fog, TAR_IGNORE,  POS_STANDING,
            NULL,   25, 4,
            "天使之霧",  "你身旁的天使之霧消失了.", ATTR_ALL, {3,4,2,2,3}

    },

    {
        "fireball", "火球術", { SKILL_LIMIT(20,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_LIMIT(20,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fireball,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   45, 4,
            "火球術",  "你身旁的火球消失了.", ATTR_ALL, {7,5,4,5,7}

    },

    {
        "powerball", "能量球", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(22,99,6), SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_powerball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   175, 8,
            "能量球",  "你身旁的能量球消失了.", ATTR_ALL, {7,5,4,6,7}

    },

    {
        "star fire", "星星之火", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_starfire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   175, 8,
            "星星之火",  "你身旁的星星之火消失了.", ATTR_ALL, {9,8,6,7,9}

    },
        
    {
        "flamestrike", "聖火擊", { SKILL_APP, SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(8,85,6), SKILL_LIMIT(10,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   40, 4,
            "聖火擊",  "你身旁的聖火消失了.", ATTR_ALL, {3,3,2,2,3}

    },

    {
        "fly",  "飛翔術", { SKILL_LIMIT(7,85,6), SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(8,85,6), SKILL_LIMIT(10,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fly,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_flying,   10, 0,
            "墬下",   "你的雙腳慢慢降回了地面.", ATTR_ALL, {3,3,2,2,3}

    },

    {
        "grave robber",  "盜墓者", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_grave_robber, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   65, 10,
            "",   "!Grave Robber!", ATTR_ALL, {8,7,6,5,8}

    },

    {
        "gate",  "隨機召喚", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_gate,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   50, 8,
            "",   "!Gate!", ATTR_ALL, {3,4,2,3,3}

    },

    {
        "giant strength", "巨人之力", { SKILL_LIMIT(12,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(8,90,6), SKILL_LIMIT(10,90,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(9,90,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_giant,   35, 2,
            "",   "你覺得那股力量消失了.", ATTR_ALL, {6,6,4,5,5}

    },

    {
        "harm",  "終極傷害術", { SKILL_APP, SKILL_LIMIT(17,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), 
            SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_harm,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   40, 4,
            "終極傷害術",  "!Harm!", ATTR_ALL, {3,2,1,1,3}

    },

    {
        "heal",  "治療術", { SKILL_APP, SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,90,6), 
            SKILL_APP, SKILL_LIMIT(10,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_heal,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 2,
            "治療術",   "!Heal!", ATTR_ALL, {3,2,3,1,4}

    },

    {
        "identify", "物品鑑定術", { SKILL_LIMIT(10,85,6), SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_identify,  TAR_OBJ_INV,  POS_STANDING,
            NULL,   12, 0,
            "",   "你對物品的評鑑力降低了.", ATTR_ALL, {7,8,6,6,8}

    },

    {
        "infravision", "夜視術", { SKILL_LIMIT(6,85,6), SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(6,99,6), SKILL_APP, 
            SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(12,90,6), 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(6,95,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP },
            spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   5, 0,
            "",   "你無法再看見黑暗中的事物了.", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "invis", "隱形術", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(5,85,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(2,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_invis,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_invis,  5, 0,
            "",   "你的隱形效果消失了.", ATTR_ALL, {3,2,1,1,3}

    },

    {
        "know alignment", "陣營辨識術", { SKILL_LIMIT(8,85,6), SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(2,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(12,99,6), 
            SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP },
            spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   9, 4,
            "",   "你無法辨識出別人的陣營了.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "thunder strike", "雷擊術", { SKILL_LIMIT(5,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_electrostatic_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_thunder_strike,   30, 2,
            "雷擊術", "你從神遊太虛的狀態下回來.", ATTR_ALL, {5,4,3,3,4}

    },

    {
        "lightning bolt", "宙斯之雷", { SKILL_LIMIT(10,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(12,90,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   50, 4,
            "宙斯之雷", "你身體不再有觸電感.", ATTR_ALL, {9,9,7,6,9}

    },

    {
        "locate object", "物品定位術", { SKILL_LIMIT(6,85,6), SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,90,6), SKILL_LIMIT(5,85,6), SKILL_LIMIT(5,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_locate_object, TAR_IGNORE,  POS_STANDING,
            NULL,   20, 8,
            "",   "!Locate Object!", ATTR_ALL, {3,2,2,1,3}

    },

    {
        "magic missile", "魔法飛彈", { SKILL_LIMIT(1,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(12,90,6), SKILL_LIMIT(4,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_magic_missile,   20, 0,
            "魔法飛彈", "你身旁惱人的光點消散了.", ATTR_ALL, {8,7,5,6,7}

    },

    {
        "mass heal", "群體治療術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(25,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,80,6), 
            SKILL_APP, SKILL_LIMIT(34,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mass_heal, TAR_IGNORE,  POS_STANDING,
            &gsn_mass_heal,  50, 3,
            "",   "!Mass Heal!", ATTR_ALL, {5,3,4,2,4}

    },

    {
        "mass invis", "群體隱形術", { SKILL_LIMIT(15,85,6), SKILL_LIMIT(17,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,85,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(15,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mass_invis, TAR_IGNORE,  POS_STANDING,
            &gsn_mass_invis, 20, 0,
            "",   "你的隱形效果消失了.", ATTR_ALL, {3,3,2,2,3}

    },

    {
        "mute",  "禁言術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mute,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_mute,  30, 2,
            "",   "你又能開口說話了.", ATTR_ALL, {8,7,7,7,8}

    },

    {
        "pass door", "穿牆術", { SKILL_LIMIT(18,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,90,6), SKILL_APP, SKILL_LIMIT(18,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pass_door, TAR_CHAR_SELF,  POS_STANDING,
            NULL,   20, 6,
            "",   "你又變得實體化了.", ATTR_ALL, {4,2,1,3,3}

    },

    {
        "poison", "施毒術", { SKILL_APP, SKILL_LIMIT(8,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), 
            SKILL_LIMIT(27,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_poison,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_poison,  25, 4,
            "施毒術", "你覺得舒服多了.", ATTR_ALL, {3,3,2,2,4}

    },

    {
        "disease", "疾病術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_disease,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_disease,  30, 6,
            "疾病術", "你覺得舒服多了.", ATTR_ALL, {3,3,2,2,4}

    },


	{
        "fear", "恐懼術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fear,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_fear,  30, 6,
            "恐懼術", "你覺得安定多了.", ATTR_ALL, {3,3,2,2,4}

    },

    {
        "polymorph other", "種族異變術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_polymorph_other, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_polymorph,   20,8,
            "種族異變術",   "你的身體又變回原樣了." , ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "protection", "天使加護術", { SKILL_APP, SKILL_LIMIT(6,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP },
            spell_protection, TAR_CHAR_SELF,  POS_STANDING,
            NULL,   5, 2,
            "",   "你不再受到保護.", ATTR_ALL, {6,5,4,4,6}

    },

    {           
        "recharge item", "物品充能術",    { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(20,99,6), SKILL_LIMIT(25,90,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_recharge_item, TAR_OBJ_INV,  POS_STANDING,
            NULL,   25, 16,
            "物品碎片",  "!Recharge Item!", ATTR_ALL, {6,5,4,4,6}

    }, 

    {
        "refresh", "體力恢復術", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,85,6), SKILL_LIMIT(3,90,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_refresh,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   20, 0,
            "refresh",  "!Refresh!", ATTR_ALL, {3,2,2,1,4}

    },

    {
        "remove alignment", "陣營解除術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_alignment, TAR_OBJ_INV,  POS_STANDING,
            NULL,   200, 12,
            "",   "!Remove Alignment!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "remove curse", "詛咒解除術", { SKILL_APP, SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(6,99,6), SKILL_LIMIT(8,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(8,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_curse, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   5, 8,
            "",   "!Remove Curse!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "remove silence", "禁言解除術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_LIMIT(36,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_silence, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 8,
            "",   "!Remove Silence!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "sanctuary", "聖光術", { SKILL_APP, SKILL_LIMIT(20,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_sanctuary,   75, 4,
            "",   "你身上的白色聖光消失了.", ATTR_ALL, {5,4,5,3,4}

    },

    {
        "shield", "魔法護盾術", { SKILL_LIMIT(13,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,90,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(22,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_shield,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   25, 2,
            "",   "你身上的魔法防護力場漸漸散去了.", ATTR_ALL, {5,5,4,5,5}

    },

    {
        "shocking grasp", "衝擊波", { SKILL_LIMIT(8,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_LIMIT(7,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   25, 2,
            "衝擊波", "!Shocking Grasp!", ATTR_ALL, {3,2,1,2,3}

    },

    {
        "sleep", "催眠術", { SKILL_LIMIT(14,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(20,90,6), SKILL_APP, 
            SKILL_LIMIT(30,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sleep,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_sleep,  35, 0,
            "",   "你覺得不會累得想睡了.", ATTR_ALL, {3,2,1,1,2}

    },

    {
        "sober up", "醒酒術", { SKILL_APP, SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(14,90,6),
            SKILL_APP, SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sober_up,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_soberup,  15, 4,
            "",   "!Sober!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },


    {
        "stone skin", "石甲術", { SKILL_LIMIT(17,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(15,90,6), SKILL_LIMIT(20,99,6), SKILL_LIMIT(22,99,6), SKILL_APP, 
            SKILL_LIMIT(28,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_stone_skin, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   25, 2,
            "",   "你的皮膚又變回原來的樣子了.", ATTR_ALL, {6,5,4,4,6}

    },

    {
        "summon", "召喚術", { SKILL_APP, SKILL_LIMIT(25,60,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_LIMIT(5,70,6), SKILL_LIMIT(8,80,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_summon,  TAR_IGNORE,  POS_STANDING,
            NULL,   50, 4,
            "",   "!Summon!", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "teleport", "隨機傳送術",{ SKILL_LIMIT(15,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,90,6), SKILL_APP, SKILL_LIMIT(18,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_teleport,  TAR_CHAR_SELF,  POS_FIGHTING,
            NULL,   35, 4,
            "",   "!Teleport!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "turn undead", "超渡不死生物", { SKILL_APP, SKILL_LIMIT(30,60,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,99,6), SKILL_LIMIT(28,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_turn_undead, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_turn_undead, 45, 4,
            "聖光超渡", "神聖的壓制消失了,你覺得邪惡的精力又回到你的身上.", ATTR_ALL, {3,1,2,1,3}

    },

    {
        "mass turn", "群體超渡術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(37,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mass_turn, TAR_IGNORE, POS_FIGHTING,
            NULL, 80, 4,
            "聖光超渡", "神聖的壓制消失了,你覺得邪惡的精力又回到你的身上.", ATTR_ALL, {5,3,4,3,5}
                                                                                
    },

    {
        "ventriloquate", "腹語術", { SKILL_LIMIT(1,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,90,6), SKILL_LIMIT(1,80,6), SKILL_LIMIT(1,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(3,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_ventriloquate, TAR_IGNORE,  POS_STANDING,
            NULL,   5, 0,
            "",   "!Ventriloquate!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "weaken", "衰弱術", { SKILL_LIMIT(7,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_LIMIT(4,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_weaken,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_weaken,   15, 2,
            "衰弱術", "你的身體又強壯了起來.", ATTR_ALL, {3,2,1,2,2}

    },

    {
        "word of recall", "神聖之語", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_word_of_recall, TAR_CHAR_SELF,  POS_RESTING,
            NULL,   5, 8,
            "",   "你耳邊傳來的上帝之音漸漸消失.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "lore",  "智者之言", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lore,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_lore,   20, 6,
            "",   "你耳邊傳來的真理之語漸漸消失.", ATTR_ALL, {3,3,2,3,3}

    },

    /*
     * Dragon breath
     */
    {
        "acid breath", "龍之強酸吐息", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_acid_breath,   175, 12,
            "龍之強酸吐息", "你身上的酸液蒸發消散.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "fire breath", "龍之灼熱吐息", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_fire_breath,   175, 12,
            "龍之灼熱吐息", "你的身體不再冒出熱煙.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "frost breath", "龍之冰凍吐息", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_frost_breath,   175, 12,
            "龍之冰凍吐息", "你皮膚上的冰霜漸漸溶化.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "gas breath", "龍之毒氣吐息", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_gas_breath, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_gas_breath,   200, 12,
            "龍之毒氣吐息",  "你身旁的毒氣消散了.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "demon tremor", "惡魔戰歌", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_demontremor, TAR_IGNORE,  POS_FIGHTING,
            NULL,   80, 8,
            "惡魔戰歌",  "你的心靈再次平靜下來!", ATTR_ALL, {5,4,3,5,6}

    },

    {
        "green breath", "綠龍狂焰", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_green_breath, TAR_IGNORE,  POS_FIGHTING,
            &gsn_green_breath,   120, 6,
            "綠龍狂焰",  "你不再感到灼熱!", ATTR_ALL, {4,3,2,4,4}

    },

    {
        "whirlwind", "狂嵐術", { SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_whirlwind, TAR_IGNORE,  POS_FIGHTING,
            NULL,   65, 6,
            "狂嵐術",  "!Whirlwind!", ATTR_ALL, {6,5,4,5,6}

    },

    {
        "lightning breath", "龍之閃電吐息", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_lightning_breath,   175, 12,
            "龍之閃電吐息", "你身上纏繞的電流退去.", ATTR_ALL, {6,6,6,6,6}

    },

    /*
     * Fighter and thief skills, as well as magic item skills.
     */
    {
        "backstab", "背刺", { SKILL_APP, SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), 
            SKILL_LIMIT(1,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_backstab,   0, 20,
            "背刺", "!Backstab!", ATTR_ALL, {7,5,8,8,7}

    },

    {
        "bash door", "撞門", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,80,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_LIMIT(26,99,6), SKILL_LIMIT(32,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(18,99,6)},
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_bash,   0, 24,
            "猛力一撞", "!Bash Door!", ATTR_ALL, {1,2,2,2,1}

    },

    {
        "berserk", "全力猛攻", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,90,6), 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(-1,90,6), 
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(18,99,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_berserk,   0,  1,
            "",   "你的心情漸漸從激戰中平靜下來.", ATTR_ALL, {8,9,9,9,8}

    },

    {
        "excited", "亢奮", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_excited,   0,  0,
            "",   "你不再那麼興奮.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "circle", "繞刺", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(27,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_circle,   0, 6,
            "繞刺",  "!Circle!", ATTR_ALL, {8,7,9,9,8}

    },

    {
        "disarm", "擊落武器", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_LIMIT(20,70,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(5,95,6), SKILL_LIMIT(10,85,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), SKILL_LIMIT(5 ,90,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_disarm,   0, 4,
            "",   "!Disarm!", ATTR_ALL, {2,3,4,4,3}

    },

    {
        "barehand", "空手入白刃", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(16,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_barehand,   0, 6,
            "",   "!Barehand!", ATTR_ALL, {2,3,4,4,3}

    },

    {
        "dodge", "閃躲技巧", { SKILL_APP, SKILL_APP, SKILL_LIMIT( 8,75,6), SKILL_LIMIT(10,70,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), 
            SKILL_LIMIT(35,99,6), SKILL_LIMIT(1,85,6), SKILL_LIMIT(1,95,6), SKILL_LIMIT(1,95,6), SKILL_LIMIT(32,85,6), 
            SKILL_APP, SKILL_LIMIT(35,85,6), SKILL_LIMIT(1, 99,6), SKILL_LIMIT(1, 95,6), SKILL_LIMIT(1, 80,6) },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_dodge,   0,  0,
            "",   "!Dodge!", ATTR_ALL, {3,1,3,3,4}

    },

    {
        "dual",  "雙手武器", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(20,99,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(16,99,6), SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_dual,  0,  0,
            "",   "!Dual!", ATTR_ALL, {2,2,4,4,3}

    },

    {
        "enhanced damage", "攻擊力增強", { SKILL_APP, SKILL_LIMIT(30, 75,6), SKILL_LIMIT(25,50,6), SKILL_LIMIT(1,75,6), SKILL_LIMIT(3,70,6), 
            SKILL_APP, SKILL_LIMIT(5,85,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), 
            SKILL_LIMIT(15, 90,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(5,80,6), 
            SKILL_APP, SKILL_LIMIT(12,99,6), SKILL_LIMIT(15,85,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1 ,90,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_enhanced_damage, 0,  0,
            "",   "!Enhanced Damage!", ATTR_ALL, {4,5,6,6,4}

    },

    {
        "hide",  "藏匿行蹤", { SKILL_APP, SKILL_APP, SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), 
            SKILL_LIMIT(2,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(34,85,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,95,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_RESTING,
            &gsn_hide,   0, 12,
            "",   "!Hide!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "kick",  "飛腿", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,75,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(16,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,99,6), SKILL_LIMIT(4,95,6), SKILL_LIMIT(8 ,95,6)},
            spell_null,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_kick,   0,  4,
            "凌空飛踢",  "!Kick!", ATTR_ALL, {5,5,8,8,6}

    },

    {
        "hunt",  "獵殺技巧", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,90,6), SKILL_APP, 
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(-1,90,6), 
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_LIMIT(25,99,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6)},
            spell_null,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_hunt,   0,  8,
            "",   "!hunt!", ATTR_ALL, {-1,-1,-1,-1,-1}
    },

    {
        "assassination", "暗殺", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_ass,   0, 24,
            "暗殺", "!Assassination!", ATTR_ALL, {7,6,8,8,7}

    },

    {
        "parry", "武器格檔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(3,80,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(13,95,6), 
            SKILL_LIMIT(36,95,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(18,95,6), SKILL_LIMIT(15,95,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(2 ,95,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_parry,   0,  0,
            "",   "!Parry!", ATTR_ALL, {1,2,3,3,2}

    },

    {
        "critical", "致命一擊", { SKILL_APP, SKILL_APP, SKILL_LIMIT(15,75,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(13,99,6), 
            SKILL_LIMIT(18,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(23,95,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,50,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_critical,   0,  0,
            "",   "!Critical!", ATTR_ALL, {10, 9,11,10,11}

    },

    {
        "fool foe",  "移形換位", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_fool,   0,  0,
            "",   "!Fool Foe!", ATTR_ALL, {2,2,3,3,5}

    },

    {
        "bodycontrol", "身體平衡控制", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(29,70,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), 
            SKILL_LIMIT(30,99,6), SKILL_LIMIT(18,80,6), SKILL_LIMIT(18,95,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_LIMIT(22,85,6), SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_bodycontrol,   0,  0,
            "",   "!Bodycontrol!", ATTR_ALL, {6,4,7,7,5}

    },

    {
        "swordmaster", "御劍術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,95,6), 
            SKILL_LIMIT(36,90,6), SKILL_LIMIT(25,90,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(25,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,90,6), SKILL_LIMIT(25,80,6) },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_swordmaster,   0,  0,
            "",   "!Swordmaster!", ATTR_ALL, {6,6,7,7,6}

    },

    {
        "spellmaster", "魔法神聖之語", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_spellmaster,   0,  0,
            "",   "!Spellmaster!", ATTR_ALL, {9,8,7,6,8}

    },

    {
        "spellcraft", "法術大師", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,85,6), SKILL_LIMIT(28,99,6), SKILL_LIMIT(31,85,6), SKILL_LIMIT(37,70,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,70,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_spellcraft,   0,  0,
            "",   "!Spellcraft!", ATTR_ALL, {9,5,7,8,9}

    },

    {
        "multi casting", "多重施法", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_multicasting,   0,  0,
            "",   "!Multicasting!", ATTR_ALL, {6,6,5,5,6}

    },

    {
        "abstraction",  "魔精石提煉術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_abstraction,   0,  18,
            "",   "!Abstraction!", ATTR_ALL, {7,6,5,5,6}

    },

    {
        "peek",  "窺視", { SKILL_APP, SKILL_APP, SKILL_LIMIT(8,85,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(4,90,6), 
            SKILL_LIMIT(4,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(4,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_peek,   0,  0,
            "",   "你不再感覺到讓人不自在的視線.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "pick lock", "開鎖", { SKILL_APP, SKILL_APP, SKILL_LIMIT(8,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(2,90,6), 
            SKILL_LIMIT(2,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_pick_lock,   0, 12,
            "",   "你感到身體某些部分似乎被關上了.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "poison weapon", "武器抹毒", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6), 
            SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_OBJ_INV,  POS_STANDING,
            &gsn_poison_weapon,  0, 12,
            "酸毒", "!Poison Weapon!", ATTR_ALL, {4,3,5,6,6}

    },

    {
        "rescue", "解救技巧", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), 
            SKILL_APP, SKILL_LIMIT(25,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(10,99,6), SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_rescue,   0, 12,
            "",   "!Rescue!", ATTR_ALL, {1,3,3,3,1}

    },

    {
        "scrolls", "朗誦捲軸", { SKILL_LIMIT(4,90,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(15,75,6), SKILL_LIMIT(10,70,6), SKILL_LIMIT(16,60,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(6,99,6), 
            SKILL_LIMIT(6,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(3,99,6), 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(12 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_scrolls,   0, 0,
            "著火的捲軸", "!Scrolls!", ATTR_ALL, {3,3,2,2,3}
    },

    {
        "second attack", "雙重攻擊", { SKILL_APP, SKILL_APP, SKILL_LIMIT(1,75,6), SKILL_LIMIT(3,80,6), SKILL_LIMIT(5,70,6), 
            SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,80,6), SKILL_LIMIT(10,80,6), SKILL_LIMIT(2,99,6), 
            SKILL_LIMIT(2,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,80,6), 
            SKILL_APP, SKILL_LIMIT(6,99,6), SKILL_LIMIT(2,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1 ,99,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_second_attack,  0,  0,
            "",   "!Second Attack!", ATTR_ALL, {1,1,1,1,1}

    },

    {
        "snare", "陷阱術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), 
            SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_snare,   0, 0,
            "",   "你不再受到陷阱的限制.", ATTR_ALL, {6,4,5,6,7}

    },

    {
        "sneak", "潛行", { SKILL_APP, SKILL_APP, SKILL_LIMIT(6,75,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), 
            SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(34,85,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(8,95,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_sneak,   0, 12,
            "",   "你停止潛行。", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "staves", "揮舞大型法杖", { SKILL_LIMIT(13,85,6), SKILL_LIMIT(13,85,6), SKILL_LIMIT(26,70,6), SKILL_LIMIT(27,60,6), SKILL_LIMIT(30,50,6),
            SKILL_LIMIT(7,99,6), SKILL_LIMIT(7,99,6), SKILL_LIMIT(17,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(14,99,6), 
            SKILL_LIMIT(14,99,6), SKILL_LIMIT(14,99,6), SKILL_LIMIT(14,99,6), SKILL_LIMIT(14,99,6), SKILL_LIMIT(10,99,6), 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(22,99,6), SKILL_LIMIT(22 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_staves,   0, 0,
            "法杖能量", "!Staves!",ATTR_ALL, {3,3,2,2,3}
    },

    {
        "steal", "偷竊技巧", { SKILL_APP, SKILL_APP, SKILL_LIMIT(8,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(4,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_steal,   0, 24,
            "",   "!Steal!",ATTR_ALL, {7,4,6,5,7}

    },

    {
        "third attack", "三重攻擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_LIMIT(12,70,6), 
            SKILL_APP, SKILL_LIMIT(10,85,6), SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(10,95,6), SKILL_LIMIT(5,85,6), SKILL_LIMIT(11,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(12,70,6), 
            SKILL_APP, SKILL_LIMIT(15,90,6), SKILL_LIMIT(10,95,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(10 ,99,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_third_attack,  0,  0,
            "",   "!Third Attack!",ATTR_ALL, {1,1,2,2,2}

    },

    {
        "fourth attack", "四重攻擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36, 85,6), SKILL_APP, SKILL_LIMIT(24,95,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,80,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_fourth_attack,  0,  0,
            "",   "!Fourth Attack!",ATTR_ALL, {2,1,3,3,3}

    },

    {
        "fifth attack", "五重攻擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP,  SKILL_LIMIT(34,95,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_fifth_attack,  0,  0,
            "",   "!Fifth Attack!",ATTR_ALL, {2,2,4,4,3}

    },

    {
        "untangle", "解除陷阱", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22,90,6), 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_untangle,   0, 24,
            "",   "!Untangle!",ATTR_ALL, {6,4,5,7,8}

    },

    {
        "wands", "揮舞魔杖", { SKILL_LIMIT(8,90,6), SKILL_LIMIT(8,90,6), SKILL_LIMIT(18,75,6), SKILL_LIMIT(12,70,6), SKILL_LIMIT(16,60,6), 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(12,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(8,99,6), 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(5,99,6), 
            SKILL_LIMIT(22,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(12 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_wands,   0, 0,
            "魔杖能量", "!Wands!",ATTR_ALL, {3,3,2,2,3}

    },

    {
        "scan",  "搜索技巧",  { SKILL_LIMIT(18,50,6), SKILL_LIMIT(20,60,6), SKILL_LIMIT(10,80,6), SKILL_LIMIT(13,75,6), SKILL_LIMIT(16,70,6), 
            SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(34,90,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(5,99,6), 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6) },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_scan,   0, 4,
            "", "!Scan!",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "scout",  "偵察技巧",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30, 99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            NULL,   0, 14,
            "", "!Scout!",ATTR_ALL, {4,4,5,6,5}

    },

    {
        "frenzy", "狂暴攻擊",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,90,6),
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(-1,90,6),
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6),
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(18,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_frenzy,   0, 15,
            "狂暴攻擊", "!Frenzy!",ATTR_ALL, {8,9,8,9,8}

    },

    {
        "bashhead", "頭部猛擊",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,75,6), SKILL_LIMIT(6,80,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(2,90,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), SKILL_LIMIT(5,99,6) },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_bashhead,   0, 8,
            "頭部猛擊", "!Bashhead!",ATTR_ALL, {4,5,7,7,5}

    },
    {
        "general purpose", "群眾之審判",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(29,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_general_purpose,   0, 0,
            "群眾之審判", "!General Purpose Ammo!",ATTR_ALL, {7,9,8,6,8}

    },

    {
        "high explosive", "高能核爆", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_high_explosive,   0, 15,
            "高能核爆", "!High Explosive Ammo!",ATTR_ALL, {7,5,4,6,7}

    },


    /*
     * Psionicist spell/skill definitions
     * Psi class conceived, created, and coded by Thelonius (EnvyMud)
     */

    {
        "adrenaline control", "副腎腺術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_adrenaline_control, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_adrenaline,    35, 2,
            "",    "你體內的衝動消失了.",ATTR_ALL, {5,6,7,4,6}

    },
    {
        
        "combat mind", "戰之藝術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_combat_mind, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   35, 2,
            "",   "你的戰鬥直覺消失了.",ATTR_ALL, {4,5,5,3,5}

    },

    {
        "full heal", "完全治癒術", { SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(-1,99,6), SKILL_APP, SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), 
            SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), SKILL_APP },
            spell_full_heal, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   200, 7,
            "",   "!Full Heal!",ATTR_ALL, {5,3,4,2,4}

    },

    {
        "gfull heal", "群體完全治癒術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(29,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_full_heal, TAR_IGNORE,  POS_STANDING,
            NULL,   500, 0,
            "",   "!Full Heal!",ATTR_ALL, {7,5,6,6,7}

    },
    {
        "barrier", "護身結界", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_barrier, TAR_CHAR_SELF,  POS_FIGHTING,
            NULL,   40,     2,
            "",   "你身上的護身結界慢慢消失了.",ATTR_ALL, {6,5,2,3,7}

    },

    {
        "vampiric bite", "吸血鬼之吻", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_vampiric_bite, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_vampiric_bite, 0,     0,
            "吸血鬼之吻", "你身上的吸血鬼印記消失了.",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "seal magic", "魔法封印", { SKILL_LIMIT(28,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_seal_magic, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_seal_magic, 30,     2,
            "", "你覺得身上的封印解開了",ATTR_ALL, {3,2,1,2,3}

    },

    {
        "artifact", "魔法物品使用", { SKILL_LIMIT(15,90,6), SKILL_LIMIT(15,90,6), SKILL_LIMIT(22,80,6), SKILL_LIMIT(20,70,6), SKILL_LIMIT(25,50,6), 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(16,90,6),
            SKILL_LIMIT(16,95,6), SKILL_LIMIT(16,85,6), SKILL_LIMIT(12,85,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(15,90,6), 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(12,90,6), SKILL_LIMIT(10,85,6), SKILL_LIMIT(18 ,85,6)},
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_artifact, 0,     0,
            "", "",ATTR_ALL, {10,10,7,7,10}

    },

    {
        "change fighting", "改變攻擊目標", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(27,99,6), 
            SKILL_LIMIT(30,95,6), SKILL_LIMIT(20,90,6), SKILL_LIMIT(18,95,6), SKILL_LIMIT(25,95,6), SKILL_LIMIT(30,70,6), 
            SKILL_APP, SKILL_LIMIT(30,90,6), SKILL_LIMIT(25,90,6), SKILL_LIMIT(25,80,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_change_fighting, 0,     1,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "nightstalk", "暗夜匿蹤", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_nightstalk, 0,     9,
            "", "你的身形逐漸從闇影中浮現",ATTR_ALL, {8,7,9,9,9}

    },

    {
        "reflexion", "反向殺法", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_reflexion, 0,     0,
            "反向殺法", "",ATTR_ALL, {8,5,6,9,8}

    },

    {
        "fist fight", "徒手攻擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_fist_fight, 0,     0,
            "", "",ATTR_ALL, {3,5,8,5,18}

    },

    {
        "regeneracy", "再生能力", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_regeneracy, 0,     0,
            "", "你的再生能力恢復正常.",ATTR_ALL, {6,6,7,7,5}

    },

    {
        "weapon master", "武器大師", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_weapon_master, 0,     0,
            "", "",ATTR_ALL, {6,7,6,6,7}

    },

    {
        "tactic", "戰術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(8,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,99,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_tactics, 0,     0,
            "掃尾攻擊", "",ATTR_ALL, {5,7,7,7,5}

    },

    {
        "track", "追蹤技巧", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), 
            SKILL_LIMIT(26,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(28,95,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_track, 0,     6,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "burial ceremony", "超渡儀式", { SKILL_APP, SKILL_LIMIT(15,80,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_burial_ceremony, 0,     0,
            "", "", ATTR_ALL, {3,2,3,1,4}
    },

    {
        "combat howling", "戰鬥狂嚎", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30 ,99,6)},
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_combat_howling, 0,     2,
            "戰鬥狂嚎", "!戰鬥狂嚎!",ATTR_ALL, {7,7,8,8,6}

    },


    {
        "blood thirsty", "嗜血", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30 ,99,6)},
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_blood_thirsty, 0,     0,
            "", "空氣中的血腥味滿足你的嗜血欲望，你腦袋放空不再殺得發狂。",ATTR_ALL, {8,8,10,10,7}

    },


    {
        "invoke", "神靈附體", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6),
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_invoke, 0,     8,
            "", "一陣失落感湧上心頭",ATTR_ALL, {7,7,6,6,5}

    },


    {
        "full refresh", "體力完全恢復術", { SKILL_APP, SKILL_LIMIT(18,80,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(15,80,6), SKILL_LIMIT(15,80,6), SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(15,90,6), SKILL_LIMIT(15,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_full_refresh, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 70,     7,
            "", "",ATTR_ALL, {5,4,4,3,5}

    },

    {
        "soulsteal", "魔力之嵐", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(22,90,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_soulsteal, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_soulsteal, 1,     4,
            "魔力之嵐", "!魔力之嵐!", ATTR_ALL,{8,7,6,7,8}
    },

    {
        "lifesteal", "生命之嵐", { SKILL_APP, SKILL_LIMIT(20,70,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lifesteal, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_lifesteal, 50,     4,
            "生命之嵐", "!生命之嵐!",ATTR_ALL, {4,4,2,3,4}

    },

    {
        "darkraid", "夜襲", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(32,99,6),
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_dark_raid, 0,     4,
            "", "你的集中力漸漸散亂",ATTR_ALL, {9,8,10,10,9}

    },

    {
        "flaming armor", "炎之鎧甲", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flaming_armor, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_flaming_armor, 70,     6,
            "護身炎甲", "圍繞在你身邊的魔焰漸漸熄滅",ATTR_ALL, {10,10,7,8,10}

    },

    {
        "mana shield", "魔力屏障", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mana_shield, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_mana_shield, 350,     4,
            "", "圍繞在你身邊的魔法結界漸漸淡化",ATTR_ALL, {5,6,2,3,6}

    },

    {
        "armageddon", "末日禁咒", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(35,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_armageddon, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_armageddon, 150,     0,
            "末日禁咒", "",ATTR_ALL, {10,10,7,8,10}

    },

    {
        "block", "盾牌抵擋", { SKILL_APP, SKILL_LIMIT(25,80,6), SKILL_APP, SKILL_LIMIT(8,75,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,90,6), SKILL_LIMIT(15,85,6), SKILL_APP,
            SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,95,6), SKILL_APP, SKILL_LIMIT(20,90,6),
            SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_APP, SKILL_LIMIT(20,95,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_block, 0,     0,
            "", "",ATTR_ALL, {1,3,3,2,2}

    },

    //This skill is for sor
    {
        "glacier strike", "冰河刺擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_glacier_strike, TAR_IGNORE, POS_FIGHTING,
            &gsn_glacier_strike, 120,     6,
            "冰河刺擊", "你感覺到世界慢慢回暖.",ATTR_ALL, {5,4,2,3,5}

    },


    {
        "clay golem", "召喚土巨人", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_clay_golem, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL, 330,     8,
            "", "",ATTR_ALL, {8,4,2,3,8}

    },

    {
        "double heal", "雙重治療術", { SKILL_APP, SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_LIMIT(18,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(27,90,6), SKILL_LIMIT(31,85,6), SKILL_APP, SKILL_APP },
            spell_double_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 30,     7,
            "雙重治療術", "",ATTR_ALL, {4,3,3,2,4}

    },

    {
        "gdouble heal", "群體雙重治療術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_double_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 100,     7,
            "", "",ATTR_ALL, {4,3,3,3,4}

    },

    {
        "remove nodrop", "詛咒物品破解術", { SKILL_APP, SKILL_LIMIT(20,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(15,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_nodrop, TAR_OBJ_INV, POS_STANDING,
            NULL, 50,     2,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },


    {
        "endless sight", "無盡視界", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_endless_sight, TAR_IGNORE, POS_STANDING,
            NULL, 50,     8,
            "", "",ATTR_ALL, {5,3,2,3,5}

    },


    {
        "divine light", "神光加護", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_divine_light, TAR_IGNORE, POS_FIGHTING,
            &gsn_divine_light, 175,     8,
            "", "你身上的白色聖光消失了.",ATTR_ALL, {8,6,6,5,8}

    },

    {
        "gfull refresh", "群體體力完復術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_full_refresh, TAR_IGNORE, POS_FIGHTING,
            NULL, 250,     0,
            "", "",ATTR_ALL, {5,4,5,4,6}

    },

    {
        "gcure blindness", "群體盲目治療術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_cure_blindness, TAR_IGNORE, POS_FIGHTING,
            NULL, 100,     10,
            "", "",ATTR_ALL, {4,5,4,3,4}

    },

    {
        "vivification", "甦癒術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(26,99,6), SKILL_LIMIT(35,80,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_vivification, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,  35, 0,
            "",   "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },


    {
        "water spirit", "召喚水精靈", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_water_spirit, TAR_IGNORE, POS_STANDING,
            NULL, 400,     8,
            "", "",ATTR_ALL, {5,3,2,3,5}

    },

    {
        "fire elemental", "召喚火元素", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fire_elemental, TAR_IGNORE, POS_STANDING,
            NULL, 500,     8,
            "", "",ATTR_ALL, {5,4,2,3,5}

    },

    {
        "archery", "箭術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_shot_arrow, 0,     0,
            "", "",ATTR_ALL, {7,9,7,6,7}

    },

    {
        "moon night", "月夜", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_moon_night, 0,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "concentration", "專注", { SKILL_APP, SKILL_LIMIT(27,85,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(31,80,6), SKILL_APP, SKILL_LIMIT(12,99,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(24, 90,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_concentration, 0,     0,
            "", "",ATTR_ALL, {5,4,4,3,5}

    },

    {
        "circular chop", "迴旋斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(37,99,6), SKILL_LIMIT(31 ,90,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_circular, 0,     10,
            "迴旋斬", "",ATTR_ALL, {6,6,5,15,6}

    },

    {
        "flaming slash", "烈焰斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flaming_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_flaming_slash, 0,     9,
            "烈焰斬", "",ATTR_ALL, {11,10,10,10,12}

    },

    {
        "earth slash", "大地斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earth_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_earth, 0,     12,
            "大地斬", "你的四肢又能活動自如了.",ATTR_ALL, {10,10,9,10,11}

    },

    {
        "storm slash", "狂風斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_storm_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_storm_slash, 0,     12,
            "狂風斬", "",ATTR_ALL, {11,11,10,10,11}

    },

    {
        "sprinkle slash", "波紋劍", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sprinkle_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_sprinkle_slash, 500,     12,
            "波紋劍", "你的行動不再受到波紋劍氣的束縛",ATTR_ALL, {7,6,5,6,7}

    },

    {
        "adore", "英烈魂", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(29,99,6), SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_adore, 0,     10,
            "英烈魂", "你激昂的情緒慢慢回復平靜",ATTR_ALL, {7,9,5,9,6}

    },

    {
        "meteoric arrow", "流星箭", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_meteoric_arrow, 0,     20,
            // "", "",ATTR_ALL, {12,14,12,8,11}
            "", "",ATTR_ALL, {11, 8,12, 9,11}

    },

    {
        "protect target", "保護目標", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(23,75,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(17,99,6), SKILL_LIMIT(15,90,6), SKILL_LIMIT(22,85,6), SKILL_LIMIT(32,90,6), 
            SKILL_APP, SKILL_LIMIT(17,85,6), SKILL_APP, SKILL_LIMIT(28,85,6), SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_protect, 0,     0,
            "", "",ATTR_ALL, {7,7,8,8,6}
    },

    {
        "break shield", "破盾術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_break_shield, 0,     0,
            "", "",ATTR_ALL, {6,5,7,6,7}

    },

    {
        "reposte", "間隙反擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(33,99,6), SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_reposte, 0,     0,
            "間隙反擊", "",ATTR_ALL, {5,6,5,8,6}

    },

    {
        "ride", "騎術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(10,95,6), SKILL_LIMIT(32,80,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_ride, 0,     0,
            "", "",ATTR_ALL, {4,5,6,5,6}

    },

    {
        "charge", "衝刺", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_charge, 0,     20,
            "衝刺", "",ATTR_ALL,{5,6,8,7,6}

    },

    {
        "twin swords", "日月雙劍", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_twin, 0,     16,
            "日月雙劍", "",ATTR_ALL, {10,8,9,9,10}

    },

    {
        "lead", "指揮", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_lead, 0,     0,
            "", "",ATTR_ALL, {6,8,5,5,7}

    },

    {
        "battle array", "陣形", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            NULL, 0,     20,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "repair armor", "護具修復術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_repair_armor, TAR_OBJ_INV,  POS_STANDING,
            NULL,   200, 12,
            "",   "!Repair Armor!",ATTR_ALL, {7,8,8,8,9}

    },

    {
        "tardy", "遲緩術", { SKILL_APP, SKILL_LIMIT(28,70,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(10,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_tardy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_tardy, 40,     2,
            "", "你又覺得輕快起來！",ATTR_ALL, {4,3,4,2,4}

    },
    {
        "autoparry", "全力格擋", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_autoparry, 0,     0,
            "", "",ATTR_ALL, {5,6,7,7,5}

    },
    {
        "autododge", "全力閃躲", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,80,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_autododge, 0,     0,
            "", "",ATTR_ALL, {6,5,6,6,7}

    },
    {
        "wind screaming", "陰風怒號", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wind_screaming, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_wind_screaming, 100,     4,
            "陰風怒號", "!wind screaming!",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "angry fire storm", "怒火風暴", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_angry_fire_storm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL, 200,     8,
            "怒火風暴", "!angry fire storm!",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "sun flash", "豔陽光爆術", { SKILL_LIMIT(35,60,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(27,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sun_flash, TAR_IGNORE, POS_FIGHTING,
            &gsn_sun_flash, 75,     7,
            "豔陽光爆", "世界又慢慢鮮明起來！",ATTR_ALL, {4,5,3,4,5}

    },
    {
        "deadly strike", "致命攻擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(32,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(32,80,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_deadly_strike, 0,     0,
            "", "",ATTR_ALL, {6,5,7,7,6}

    },

    {
        "calm", "鎮靜術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(32,99,6), SKILL_LIMIT(34,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_calm, TAR_IGNORE, POS_FIGHTING,
            &gsn_calm, 80,     6,
            "鎮靜術", "",ATTR_ALL, {3,3,4,2,3}

    },

    {
        "virtual swords", "幻劍術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_virtual_swords, TAR_IGNORE, POS_FIGHTING,
            &gsn_virtual_swords, 90,     8,
            "幻劍術", "",ATTR_ALL, {8,7,6,6,7}

    },
    {
        "powerblow", "重擊術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_powerblow, 0,     0,
            "重擊術", "",ATTR_ALL, {10,10,11,11,8}

    },
    {
        "mana recharge", "魔力回復", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mana_recharge, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 1,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "double recharge", "雙重魔力回復", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_double_recharge, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 1,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "full recharge", "魔力全復術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_full_recharge, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 1,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "meditate", "冥想", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_LIMIT(34,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,80,6), 
            SKILL_LIMIT(26,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_meditate, 0,     290,
            "", "你的精神慢慢與身體重新結合。",ATTR_ALL, {6,6,4,5,6}

    },

    {
        "fix item", "裝備修理", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_OBJ_INV, POS_STANDING,
            &gsn_fix_item, 0,     24,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "escape", "逃命技巧", { SKILL_APP, SKILL_LIMIT(35,70,6), SKILL_LIMIT(30,85,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6),
            SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_escape, 0,     4,
            "", "",ATTR_ALL, {12, 10, 11, 11, 12}//{7,5,6,7,8}  //  // {STR,DEX,INT,WIS,CON}
    },
    {
        "mystic mirror", "魔法之鏡", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(37,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(39,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mystic_mirror, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_mystic_mirror, 350,     6,
            "", "你周圍的魔法之鏡消失了。",ATTR_ALL, {6,5,2,3,6}
    },
    {
        "mystic armor", "魔力護壁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mystic_armor, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_mystic_armor, 800,     6,
            "", "你周圍的魔力護壁慢慢消失。",ATTR_ALL, {6,6,2,3,6}
    },
    {
        "lifeshare", "同生共死", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lifeshare, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_lifeshare, 0,     0,
            "同生共死", "",ATTR_ALL, {5,5,9,4,5}
    },
    {
        "fire wall", "火牆術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fire_wall, TAR_IGNORE, POS_STANDING,
            NULL, 60,     16,
            "火牆術", "火牆緩緩地熄滅了。",ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "armor mastery", "裝甲專家", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(28,99,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_armor_mastery, 0,     0,
            "", "",ATTR_ALL, {10,12,12,12,10}
    },
    {
        "magic arrow", "魔法箭", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(33,99,6), SKILL_APP, SKILL_APP },
            spell_magic_arrow, TAR_IGNORE, POS_STANDING,
            &gsn_magic_arrow, 0,     4,
            // "", "",ATTR_ALL, {12,11,10,10,12}
            "", "",ATTR_ALL, {12,9, 8,10,12}

    },
    {
        "prot. from fire", "火焰防壁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pfire,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "你周圍的火焰防壁消失了。", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "prot. from cold", "寒冰防壁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pcold,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "你周圍的寒冰防壁消失了。", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "prot. from wind", "疾風防壁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pwind,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "你周圍的疾風防壁消失了。", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "prot. from earth", "大地防壁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pearth,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "你周圍的大地防壁消失了。", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "day of protection", "神聖加護", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_day_of_prot,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   150, 12,
            "",   "你周圍的神聖防壁消失了。", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "day of power", "聖戰之詩", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_day_of_power,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   260, 12,
            "",   "你心中的聖戰旋律消逝了。", ATTR_ALL, {4,4,0,3,5}

    },
    {
        "day of brightness", "明光之境", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_day_of_brightness,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   75, 12,
            "",   "你身上的水神祝福消失了。", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "resurrection", "復活術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_resurrection,  TAR_OBJ, POS_STANDING,
            &gsn_resurrection,   250, 12,
            "",   "死亡的陰影逐漸離你遠去。", ATTR_ALL, {-1,-1,7,5,-1}

    },
    {
        "mental empower", "心靈增幅", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(33,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mental_empower, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_mental_empower,   40, 4,
            "心靈增幅",  "你的心靈逐漸恢復正常!", ATTR_ALL, {5,5,4,6,5}

    },
    {
        "empslash", "霸皇劍", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_empslash,   0, 12,
            "霸皇劍",  "", ATTR_ALL, {8,9,9,10,9}

    },


    {
        "wizard rage", "戰巫天怒", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wizard_rage, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_wizard_rage, 0,     0,
            "戰巫天怒", "你感到全身的力量又恢復了。", ATTR_ALL, { 8,-1,5,6,-1}
    },
    {
        "hasten", "行氣活身法", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_hasten,   0, 0,
            "",   "你的身體機能又回到正常的運作速率。", ATTR_ALL, {7,8,15,7,15}

    },
    {
        "morphing", "易容術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), 
            SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_STANDING,
            &gsn_morphing,   12, 0,
            "",   "你的變裝效果消失了。", ATTR_ALL, {2,1,1,3,2}

    },
    {
        "crushing hand", "粉碎術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_crushing_hand,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_crushing_hand,   1000, 0,
            "",   "", ATTR_ALL, {8,9,7,10,9}

    },
    {
        "song of clenase", "淨化之歌", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(35,99,6), SKILL_LIMIT(35,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_song_of_cleanse,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_song_of_clenase,   200, 0,
            "淨化之歌",   "淨化之歌", ATTR_ALL, {4,4,4,4,4}

    },
    // mark and recall is for Sorcerer
    {
        "mark", "標記術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36, 99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mark,  TAR_IGNORE, POS_STANDING,
            &gsn_mark,   150, 4,
            "淨化之歌",   "淨化之歌", ATTR_ALL, {7,5,6,7,7}

    },
    {
        "recall", "返回術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_recall,  TAR_IGNORE, POS_FIGHTING,
            &gsn_recall,   300, 4,
            "返回術",   "淨化之歌", ATTR_ALL, {7,5,6,7,7}

    },
    // this spell is for wizard
    {
        "evacuate", "脫離術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37, 99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_evacuate,  TAR_IGNORE, POS_FIGHTING,
            &gsn_evacuate,   150, 8,
            "淨化之歌",   "淨化之歌", ATTR_ALL, {5,5,2,3,5}

    },

    // this skill is for war-kni
    {
        "defensive", "護身罡\氣", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE, POS_FIGHTING,
            &gsn_defensive,   150, 12,
            "護身罡\氣",   "你的護身罡\氣消失了。", ATTR_ALL, {6,7,8,6,5}

    },

    { //passive skill for wiz/alc/sor
        "spell fury", "魔法狂擊", { SKILL_LIMIT( 34, 60,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(26, 99,6), SKILL_LIMIT( 30, 75,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(28, 85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE, POS_STANDING,
            &gsn_spell_fury,   150, 12,
            "魔法狂擊",   "魔法狂擊", ATTR_ALL, {9,10,8,9,10}

    },

    {
        "continous heal", "持續治療術", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP,SKILL_LIMIT(32,99,6), SKILL_LIMIT(33,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36, 70,6),
            SKILL_APP, SKILL_LIMIT(37,70,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cont_heal,  TAR_CHAR_SELF, POS_STANDING,
            &gsn_cont_heal,   60, 6,
            "持續治療",   "你體內的暖流消失了。", ATTR_ALL, {6,5,6,4,6}

    },
    {
        "double refresh", "雙重體力回復術", { SKILL_APP, SKILL_LIMIT(35,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_LIMIT(32,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_double_refresh, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 30,     7,
            "雙重體力回覆", "double refresh",ATTR_ALL, {4,3,3,2,4}

    },
    {
        "blind fight", "心眼流拳法", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(35,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_blind_fight,   0, 0,
            "",   "", ATTR_ALL, {1,2,3,3,5}

    },
    {
        "throwing", "投擲物品", { SKILL_LIMIT(20,70,6), SKILL_LIMIT(20,70,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,75,6), SKILL_LIMIT(20,75,6), 
            SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,95,6), 
            SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,95,6), SKILL_LIMIT(20,95,6), SKILL_LIMIT(20,85,6), 
            SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,95,6), SKILL_LIMIT(20,90,6), SKILL_LIMIT(20,85,6) },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_throwing, 0,     9,
            "投擲物品", "投擲物品",ATTR_ALL, {2,1,3,3,2}

    },
    {
        "evade", "迴避技巧", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_evade, 0,     6,
            "迴避技巧", "你的迴避能力下降了.",ATTR_ALL, {6,5,7,7,7}

    },
    {
        "meteor shower", "殞石雨", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(35,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_meteor_shower,  TAR_IGNORE, POS_FIGHTING,
            &gsn_meteor_shower,   0, 8,
            "殞石雨",   "meteor shower", ATTR_ALL, {11,10,7,7,11}

    },
    {
        "ice comet", "冰之彗星", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_ice_comet,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_ice_comet,   100, 8,
            "冰之彗星",   "ice comet", ATTR_ALL, {9,9,6,6,9}

    },
    {
        "holy crown", "聖之冠冕", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
           SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP,
 	         SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
 	         SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
 	         spell_holy_crown,  TAR_CHAR_DEFENSIVE, POS_STANDING,
 	         NULL,   280, 12,
 	         "強大的靈氣緩緩發散......",   "你身邊圍繞的靈氣消失了", ATTR_ALL, {8,8,5,4,8}
                                                                                
    },

    {
        "powerup", "發勁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_powerup,   0, 0,
            "",   "你體內的勁力消失了。", ATTR_ALL, {6,7,8,7,12}

    },

    {
        "bandage", "包紮傷勢", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_bandage,   0, 12,
            "",   "你的身體機能又回到正常的運作速率。", ATTR_ALL, {3,2,2,2,4}
    },
    {
        "trueshot", "精準射擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_trueshot,   0, 8,
            "",   "你眼中的神光逐漸恢復正常。", ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "powerbolt", "魔彈射擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_powerbolt,   0, 8,
            "",   "你附加在箭上的力量消失了。", ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "cure disease", "治療疾病術", { SKILL_APP, SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(23,99,6), SKILL_LIMIT(25,99,6), SKILL_APP,
            SKILL_APP, SKILL_LIMIT(-1,85,6), SKILL_APP, SKILL_LIMIT(-1,85,6), SKILL_LIMIT(20,90,6),
            SKILL_LIMIT(-1,85,6), SKILL_LIMIT(-1,85,6), SKILL_LIMIT(-1,85,6), SKILL_LIMIT(-1,85,6), SKILL_APP },
            spell_cure_disease,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,  30, 6,
            "疾病術", "你覺得舒服多了.", ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "prot. from lightning", "閃電防壁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_plightning,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "你周圍的閃電防壁消失了。", ATTR_ALL, {-1,-1,5,4,-1}
    },
    // this skill is for sor
    {
        "sand tide", "沙浪翻騰", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sand_tide, TAR_IGNORE, POS_FIGHTING,
            &gsn_sand_tide, 120,     6,
            "沙浪翻騰", "你感覺終於從沙堆裡掙脫出來.",ATTR_ALL, {-1,4,2,3,-1}
    },
    // this skill is for sor
    {
        "banshee wail", "喪妖厲嚎", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_banshee_wail, TAR_IGNORE, POS_FIGHTING,
            &gsn_banshee_wail, 120,     6,
            "喪妖厲嚎", "你感覺報喪女妖的哭嚎消失了.",ATTR_ALL, {-1,4,2,3,-1}
    },
    // this skill is for sor
    {
        "succubus kiss", "魅魔香吻", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_succubus_kiss, TAR_IGNORE, POS_FIGHTING,
            &gsn_succubus_kiss, 120,     6,
            "魅魔香吻", "你感覺魅魔帶給你的激情漸漸遠離.",ATTR_ALL, {-1,4,2,3,-1}
    },
    // this skill is for sor
    {
        "will fortress", "意志要塞", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_will_fortress, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_will_fortress, 150,     4,
            "意志要塞", "你感覺堅強的意志武裝慢慢崩落.",ATTR_ALL, {-1,7,2,4,-1}
    },
    // this skill is for sor
    {
        "mind blast", "心靈爆震", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mind_blast, TAR_IGNORE, POS_FIGHTING,
            &gsn_mind_blast, 30,     6,
            "心靈爆震", "你心靈慢慢從模糊中回神.",ATTR_ALL, {-1,3,2,2,-1}
    },
    // this skill is for sor
    {
        "disenchant", "幻象除滅", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_disenchant, TAR_IGNORE, POS_FIGHTING,
            NULL, 100,     8,
            "幻象除滅", "",ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    // this skill is for sor
    {
        "ghosts celebration", "百鬼夜行", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_ghosts_celebration, TAR_IGNORE, POS_FIGHTING,
            NULL, 80,     8,
            "百鬼夜行", "你感覺四周鬼魅的交談聲終於消失.",ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    
    {
        "chain lightning", "連鎖閃電", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_chain_lightning, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_chain_lightning,   200, 10,
            "連鎖閃電",  "!chain lightning!", ATTR_ALL, {11,11,7,7,11}

    },
    {
        "icecore slash", "冰髓斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_icecore_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_icecore_slash, 0,     12,
            "冰髓斬", "",ATTR_ALL, {11,11,10,10,11}

    },
    {
        "fulgurant slash", "悽雷斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fulgurant_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_fulgurant_slash, 0,     12,
            "悽雷斬", "",ATTR_ALL, {11,11,10,10,11}

    },
    {
        "body building", "強身健體", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_body_building, 0,  0,
            "",   "你身體的強化消失了.", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "mana condense", "魔力凝聚", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_mana_condense, 0,  0,
            "",   "凝聚在你身邊的魔力散去.", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "poison explosion", "毒霧爆", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_poison_explosion, TAR_IGNORE,  POS_FIGHTING,
            &gsn_poison_explosion, 75,  4,
            "毒霧爆",   "poison explosion", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "finger of death", "死亡之指", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_finger_of_death, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_finger_of_death, 120,  4,
            "死亡之指",   "finger of death", ATTR_ALL, {4,5,6,6,4}

    },    
    {
        "apocalypse", "啟示錄之炎", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_apocalypse, TAR_IGNORE,  POS_FIGHTING,
            &gsn_apocalypse, 350,  6,
            "啟示錄之炎",   "Apocalypse", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "dark containment", "闇咒封鎖", { SKILL_APP, SKILL_LIMIT(30,60,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,99,6), SKILL_LIMIT(28,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dark_containment, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_dark_containment, 80,  4,
            "闇咒封鎖",   "纏繞你身體的邪氣崩落消失。", ATTR_ALL, {4,1,3,2,3}

    },
    {
        "phlogistic verse", "殛焰詩篇", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_phlogistic_verse, TAR_IGNORE,  POS_FIGHTING,
            &gsn_phlogistic_verse, 1000,  0,
            "殛焰詩篇",   "火焰的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {15,15,12,12,15}
    },
    {
        "wail of tempest", "霆風哀鳴", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_wail_of_tempest, TAR_IGNORE,  POS_FIGHTING,
            &gsn_wail_of_tempest, 1000,  0,
            "霆風哀鳴",   "狂風的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "fulgurant dance", "曙雷繞舞", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_fulgurant_dance, TAR_IGNORE,  POS_FIGHTING,
            &gsn_fulgurant_dance, 1000,  0,
            "曙雷繞舞",   "閃電的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "agglomerate descant", "泠晶弦歌", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_agglomerate_descant, TAR_IGNORE,  POS_FIGHTING,
            &gsn_agglomerate_descant, 1000,  0,
            "泠晶弦歌",   "寒冰的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "earth bind" , "大地束縛", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_earth_bind, TAR_IGNORE,  POS_FIGHTING,
            &gsn_earth_bind, 1000,  0,
            "大地束縛",   "大地的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "holy judgement", "聖之審判", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_holy_judgement, TAR_IGNORE,  POS_FIGHTING,
            &gsn_holy_judgement, 1000,  0,
            "聖之審判",   "神聖的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "hell southerncross", "地獄南十字星", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_hell_southencross, TAR_IGNORE,  POS_FIGHTING,
            &gsn_hell_southencross, 1400,  0,
            "地獄南十字星",   "黑闇的壓制消失了，你覺得身體又慢慢恢復正常。", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "salvation", "救世之歌", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_salvation, TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
            &gsn_salvation, 350,  8,
            "救世之歌", "salvation", ATTR_ALL, {5,5,5,4,5}

    },
    {
        "song of firestorm", "蒼炎之歌", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_song_of_firestorm, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_song_of_firestorm, 50,  4,
            "蒼炎之歌",   "song of firestorm", ATTR_ALL, {4,5,6,6,4}

    }, 
    {
        "lore of frost", "霰霜之律", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_lore_of_frost, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_lore_of_frost, 50,  4,
            "霰霜之律",   "lore of frost", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "soul of magma", "焦炎之魄", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_soul_of_magma, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_soul_of_magma, 50,  4,
            "焦炎之魄",   "soul of magma", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "fang of slime", "泥月之牙", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_fang_of_slime, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_fang_of_slime, 50,  4,
            "泥月之牙",   "fang of slime", ATTR_ALL, {4,5,6,6,4}
    },
    
    //Sor New Skill
    {
        "flame elemental", "召喚火元素", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flame_elemental, TAR_ARG, POS_STANDING,
            &gsn_flame_elemental, 300,     8,
            "召喚火元素", "你不再受到火元素的加持.",ATTR_ALL, {10,6,7,7,10}

    },

    {
        "water elemental", "召喚水元素", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_water_elemental, TAR_ARG, POS_STANDING,
            &gsn_water_elemental, 250,     8,
            "召喚水元素", "你不再受到水元素的加持.",ATTR_ALL, {10,7,8,8,10}

    },
  
    {
        "earth elemental", "召喚土元素", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earth_elemental, TAR_ARG, POS_STANDING,
            &gsn_earth_elemental, 300,     8,
            "召喚土元素", "你不再受到土元素的加持.",ATTR_ALL, {10,6,6,6,10}

    },

    {
        "wind elemental", "召喚風元素", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wind_elemental, TAR_ARG, POS_STANDING,
            &gsn_wind_elemental, 200,     8,
            "召喚風元素", "你不再受到風元素的加持.",ATTR_ALL, {10,5,7,7,10}

    },

    {
        "lightning elemental", "召喚雷元素", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_elemental, TAR_ARG, POS_STANDING,
            &gsn_lightning_elemental, 250,     8,
            "召喚雷元素", "你不再受到雷元素的加持.",ATTR_ALL, {10,6,7,7,10}

    },
		
    
    { // natural ability for all classes and races
        "inborn ability", "天賦", { 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6),
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6),
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6),
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6) },
            spell_null,  TAR_IGNORE, POS_STANDING,
            &gsn_inborn_ability,   150, 12,
            "天賦",   "你的潛力恢復正常.", ATTR_ALL, { 1, 1, 1, 1, 1}

    },

    {
        "werewolf", "狼人", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_werewolf, 0,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "elemental slash", "元素魔法斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_elemental_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_elemental_slash, 0,     12,
            "元素魔法斬", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "sun slash", "烈光斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sun_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_sun_slash, 0,     12,
            "烈光斬", "世界又慢慢鮮明起來！",ATTR_ALL, {11,11,10,10,12}

    },

    {
        "instant cast", "迅捷施法", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_instant_cast,   0, 10,
            "迅捷施法",  "", ATTR_ALL, {5,5,2,2,5}

    },

    {
        "stone form", "石化", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_stone_form,   0, 10,
            "石化",  "身體的石化狀態漸漸消失了.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "lance mastery", "槍術", { 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(25, 99, 6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_lance_mastery,   0, 10,
            "槍術",  "", ATTR_ALL, {4,5,8,8,5}
    },

    {
        "venomous strike", "毒襲", { 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,70,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20, 99, 6),
            SKILL_LIMIT(30, 90, 6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_venomous_strike,   0, 10,
            "毒襲",  "", ATTR_ALL, {5,4,6,7,7}
    },

    {
        "satiety", "飽足", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_satiety,   0,  0,
            "",   "你感覺沒那麼有飽足感了.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "tricky pacing", "詭譎步調", { 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20, 99, 6),
            SKILL_LIMIT(20, 95, 6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20, 90, 6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_tricky_pacing,   0, 10,
            "詭譎步調",  "", ATTR_ALL, {4,3,6,6,6}
    },
	
    {
        "hit attack", "空手攻擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_hit_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "slice attack", "銳利切割", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_slice_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "stab attack", "威猛一戳", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_stab_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "slash attack", "揮舞劈砍", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_slash_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "whip attack", "灼熱一鞭", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_whip_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "claw attack", "用力一抓", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_claw_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "blast attack", "爆發噴擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_blast_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "pound attack", "萬鈞力搥", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_pound_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "crush attack", "狠狠碾壓", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_crush_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "grep attack", "犀利一剁", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_grep_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "bite attack", "張口猛咬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_bite_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "pierce attack", "尖銳刺擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_pierce_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "suction attack", "猛烈吸吮", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_suction_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "chop attack", "剁菜連斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_chop_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "shot attack", "射擊傷害", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_shot_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "magic enchantment", "法術附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_magic_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_magic_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "wind enchantment", "風屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wind_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_wind_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "flame enchantment", "火屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flame_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_flame_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "water enchantment", "水屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_water_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_water_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "earth enchantment", "地屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earth_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_earth_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "lightning enchantment", "雷屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_lightning_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "saint enchantment", "聖屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_saint_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_saint_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {3,3,2,1,3}

    },
	
    {
        "dark enchantment", "暗屬性附魔", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dark_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_dark_enchantment,   250, 0,
            "",   "你身上的附魔消失了.", ATTR_ALL, {3,3,2,1,3}

    },

    {  // 將 dart 分開製作成不同 skill 2022/04/29
        "dart mastery", "暗器專精", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,90,6), 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_dart_mastery, 0,     8,
            "金幣飛舞", "coin dart",ATTR_ALL, {2,1,2,3,2}

    },

    {  // 新增 alc 專用 skill 2022/04/29
        "artifacts mastery", "魔具專精", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_artifacts_mastery, 0,     8,
            "魔彈投擲", "throw ammo",ATTR_ALL, {2,3,1,3,2}

    },

    {   // 新增 alc 專用 skill 2022/05/01
        "firearm mastery", "槍械精通", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_firearm_mastery, 0,     8,
            "頭部射擊", "head shot",ATTR_ALL, {4,4,4,5,5}

    },
	
    {
        "rainfall arrow", "箭雨", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_rainfall_arrow, 0,     20,
            "", "",ATTR_ALL, {-1, -1, -1, -1, -1}

    }

    /*
       {
       "extra heal", "生命力超越", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_extra_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },

       {
       "group extra heal", "群體生命力超越", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, 28, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_gextra_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },

       {
       "ogre power", "巨魔戰擊", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       24, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_ogre_power, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },


       {
       "holy protection", "聖靈加護", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, 32, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_holy_protection, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },

       {
       "cometslash", "慧星斬", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, 12, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       &gsn_cometslash, 0,     0,
       "", ""
       },

*/
};
