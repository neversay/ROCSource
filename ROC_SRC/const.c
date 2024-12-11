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
//*  PURPOSE     :         �ק�spell_gate ppl����ǲ�             *
//*              :         �ק�spell_change_sex ppl����ǲ�       *
//*              :         �ק�spell_destroy_cursed ppl����ǲ�   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940711 jye                                    *
//*  PURPOSE     :         �s�Wspell_flame_elemental              *
//*              :             spell_water_elemental              *
//*              :             spell_earth_elemental              *
//*              :             spell_wind_elemental               *
//*              :             spell_lightning_elemental          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940725 jye                                    *
//*  PURPOSE     :         �ק� Sor New Skill �i�ǲߵ��ŤΥ[�Wstun*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941002 Razgriz                                *
//*  PURPOSE     :         �ק�will fortress�� TAR_CHAR_DEFENSIVE *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941016 Razgriz                                *
//*  PURPOSE     :         �ק� �I�� ���ϥήɾ��� POS_FIGHTING    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050426 Jye                                    *
//*  PURPOSE     :         �ק� �ر��ݩ�                          *
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
   18, //     3                RACE_FLY                  4  �v�T DC ��
    0, //     4                RACE_SWIM                 8
    0, //     5                RACE_WATERWALK           16
    0, //     6                RACE_PASSDOOR            32
   13, //     7                RACE_INFRAVISION         64  ���ɩ]�ߤ��C tick �� HP/MP/MV ���^�_
    0, //     8                RACE_DETECT_ALIGN       128
    0, //     9                RACE_DETECT_INVIS       256
    0, //    10                RACE_DETECT_HIDDEN      512
   23, //    11                RACE_PROTECTION        1024  ��׶ˮ`����v����
    0, //    12                RACE_SANCT             2048
    0, //    13                RACE_WEAPON_WIELD      4096
    0, //    14                RACE_MUTE              8192
    0, //    15                RACE_NO_POISON        16384
   13, //    16                RACE_DETECT_MAGIC     32768  pp , md �B�~�[��
   55, //    17                RACE_REGENERACY       65536  �A�ͯ�O������
   27, //    18                RACE_BEHOLDER        131072  �v�T�� gaze ����O�Asklv ��@�w����(�i��O�ѤH)�i�h���I�˳�
   33, //    19                RACE_LIZARDMAN       262144  �v�T��R�ġB����������
   31, //    20                RACE_ORC             524288  �v�T�Y�ͪ��ĪG
   39, //    21                RACE_VAMPIRE        1048576  �v�T���]�Χl�尭���k
    0, //    22                RACE_INVIS          2097152
    0, //    23                RACE_ANTI_MAGIC     4194304  ���]�k�ˮ`������K
   23, //    24                RACE_UNDEAD         8388608  �v�T�ﯫ�t�]�k����ܤO
   45, //    25                RACE_WEREWOLF      16777216  ��u�դ��ܦ^�H�Ϊ��ɶ��B�z�ƮĪG�A�H�δ��ɤȩ]��O�C
   37, //    26                RACE_GARGOLYE      33554432  �֦��ۤƯ�O
   35, //    27                RACE_CENTAUR       67108864  �H��, �i���� boot ��� legging, ���خy�M
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
		"Mag", "Mage", "�k�v","�k  �v", APPLY_INT,  OBJ_VNUM_SCHOOL_DAGGER,
		3018,  85,  8,	 -1,   6,  8, TRUE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"Cle", "Cleric", "���v","��  �v", APPLY_WIS,  OBJ_VNUM_SCHOOL_MACE,
		3003,  80,  8,	 -8,  6, 10, TRUE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"Thi", "Thief", "����","��  ��", APPLY_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
		3028,  75,  8,	 -10,  8, 13, FALSE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"War", "Warrior", "�Ԥh", "��  �h",APPLY_STR,  OBJ_VNUM_SCHOOL_SWORD,
		3022,  75,  8,	 -12,  11, 15, FALSE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },

    {
		"Bar", "Barbarian", "���Z�H", "���Z�H",APPLY_CON,  OBJ_VNUM_SCHOOL_SWORD,
		3221,  70,  8,	 -12,  11, 20, FALSE, { L_APP, L_APP, L_APP, L_APP, L_APP}
    },
    
    {
		"Wiz", "Wizard", "�Ův", "��  �v",APPLY_WIS,  OBJ_VNUM_SCHOOL_DAGGER,
		3018,  99,  2,	 -4,   6,  9, TRUE, { 20, L_APP, L_APP, 36, L_APP}
    },

    {
		"Alc", "Alchemist", "����v", "����v",APPLY_STR,  OBJ_VNUM_SCHOOL_DAGGER,
		3018,  99,  0,	 -16,   8,  12, TRUE, { 20, L_APP, L_APP, 34, 36}
    },

    {
		"Sav", "Savant", "���", "��  ��",APPLY_DEX,  OBJ_VNUM_SCHOOL_MACE,
		3003,  99,  8,	 -1,  7, 10, TRUE, { L_APP, 20, L_APP, L_APP, L_APP}
    },

    {
		"Bis", "Bishop", "�D��", "�D  ��",APPLY_INT,  OBJ_VNUM_SCHOOL_DAGGER,
		3003,  99,  5,	 -12,  8, 13, TRUE, { 35, 20, L_APP, L_APP, L_APP}
    },


    {
		"Asn", "Assassin", "���","��  ��", APPLY_STR,  OBJ_VNUM_SCHOOL_SWORD,
		3028,  90,  0,	 -20,  11, 15, FALSE, { L_APP, L_APP, 20, 34, 36}
    },

    {
		"Nin", "Ninja", "�Ԫ�","��  ��", APPLY_INT,  OBJ_VNUM_SCHOOL_SWORD,
		3028,  95,  2,	 -18,  9, 13, TRUE, { L_APP, L_APP, 20, 34, 35}
    },
    
    {
		"Kni", "Knight", "�M�h","�M  �h", APPLY_CON,  OBJ_VNUM_SCHOOL_DAGGER,
		3022,  90,  0,	 -22,   13,  18, FALSE, { 35, 35, 35, 20, 34}
    },    

    {
		"Swd", "Swordsman", "�C��","�C  ��", APPLY_DEX,  OBJ_VNUM_SCHOOL_SWORD,
		3022,  95,  0,	 -22,  11, 15, TRUE, { 36, L_APP, 34, 20, 34}
    },

    {
		"Mon", "Monk", "���Q","��  �Q", APPLY_DEX,  OBJ_VNUM_SCHOOL_SWORD,
		3022,  95,  -2,	 -24,  11, 15, TRUE, { L_APP, 30, L_APP, 20, 30}
    },
    
    {
		"Shm", "Shaman", "����","��  ��", APPLY_WIS,  OBJ_VNUM_SCHOOL_DAGGER,
		3221,  90,  2,	 -17,   15,  20, TRUE, { 30, 25, L_APP, L_APP, 20}
    },

    {
		"Sor", "Sorcerer", "�۪k�v","�۪k�v", APPLY_DEX,  OBJ_VNUM_SCHOOL_SWORD,
		3018,  99,  5,	 -12,  6, 10, TRUE, { 20, 34, L_APP, L_APP, L_APP}
    },
    
    {
		"Lor", "Lord", "��D","��  �D", APPLY_STR,  OBJ_VNUM_SCHOOL_DAGGER,
		3003,  95,  0,	 -18,   11,  15, TRUE, { L_APP, 20, L_APP, 30, 34}
    },    

    {
		"Ran", "Ranger", "��L","��  �L", APPLY_WIS,  OBJ_VNUM_SCHOOL_SWORD,
		3028,  95,  0,	 -23,  8, 12, TRUE, { 35, 33, 20, 30, 35}
    },

    {
		"Wrd", "Warlord", "�Ԥ�","��  ��", APPLY_INT,  OBJ_VNUM_SCHOOL_SWORD,
		3221,  85,  -2,	 -14,  10, 20, FALSE, { L_APP, 36, 35, 30, 20}
    },
    
    {
		"Mar", "Martialman", "�g�Ԥh","�g�Ԥh", APPLY_STR,  OBJ_VNUM_SCHOOL_DAGGER,
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
   nature_svs{�k, ��, ��, ��, �g, �s, �p, �t, ��}, {nature_dodge, nature_ac}, dmg_message, 
   hate{ ... }, 
   glow, ghigh
*/
const struct race_type race_table [ MAX_RACE ] =
{
	{ "Human", "�H��",
		RACE_PC_AVAIL | RACE_WEAPON_WIELD,
			 /* Five ATR  ,  HP MP MV Gain ,  Drink Eat , */
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3,  0,  1,  0,  1,  0,  0,  0,  0, /* Drink Eat */  0,  0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -5,  -4,  -4,  -4,  -4,   0,  -4,   0,   0  },  -60,  -90, "�Ť�@��",
		"Githyanki Vampire Werewolf Mindflayer Ogre",
		100, 100
	},
	{ "Elf", "���F",
		RACE_PC_AVAIL | RACE_DETECT_MAGIC | RACE_DETECT_INVIS | RACE_WEAPON_WIELD,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3, -2,  2,  1,  2, -2,  0,  3,  1, /* Drink Eat */  -1,  0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{ -11,  -7,  -7,  -7,  -7,  -2,  -7, -10,   4  },  -80, -100, "�Ť�@��",
		"Demon Drow Ogre Orc Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin Half-kobold", 
		50, 80
	},
	{ "Dwarf", "�G�H",
		RACE_PC_AVAIL | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3,  3, -3,  1, -2,  4,  4,  0,  0, /* Drink Eat */  1,  0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{ -12, -12, -12, -12, -12, -12,   8,  -5,  -5  },   50, -120, "�Ť�@��",
		"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin Half-kobold",
		100, 200
	},
	{ "Orc", "�~�H",
		RACE_PC_AVAIL | RACE_INFRAVISION | RACE_WEAPON_WIELD | RACE_ORC,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_4,  4, -3, -4,  0,  2,  2,  0,  1, /* Drink Eat */  1, 3,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{   5,   0,   0,  -3,   0, -15,  -2,   0,  -4  },  -30, -100, "�Ť�@��",
		"Elf Dwarf Hobbit Vampire Werewolf Gnome",
		30, 30
	},
	{ "Hobbit", "�P��S",
		RACE_PC_AVAIL | RACE_DETECT_HIDDEN	| RACE_WEAPON_WIELD,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_2, -1, -1,  3,  4, -2,  0,  0,  5, /* Drink Eat */  0, 0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -6,  -6,  -5,  -6,  -5,  -5,  -6, -10, -15  }, -250,  -70, "�Ť�@��",
		"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin Half-kobold",
		50, 50
	},
	{ "Fairy", "����",
		RACE_PC_AVAIL | RACE_FLY | RACE_DETECT_ALIGN | RACE_WEAPON_WIELD | RACE_ANTI_MAGIC,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_1, -4,  4,  2,  2, -4,  0,  4,  1, /* Drink Eat */  0,-1,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -8,  -7,  -7,   8, -10,  -7,  -7,  -8,  -8  }, -170,  -30, "�Ť�@��",
		"Kobold Minotaur Angel Goblin Dwarf HalfDwarf Half-elf Half-kobold Beholder Ogre",
		50, 50
	},
	{ "Gnome", "�a��",
		RACE_PC_AVAIL | RACE_DETECT_ALIGN | RACE_PROTECTION | RACE_WEAPON_WIELD,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_2, -2,  2,  4, -4,  0,  1,  2,  0, /* Drink Eat */  0, 0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{ -10, -10, -10, -10, -10,   5, -10, -20,  20  },  300, -130, "�Ť�@��",
		"Drow Ogre Orc Kobold Troll Hobgoblin Vampire Werewolf Goblin",
		50, 50
	},
	{ "Drow", "�t�º��F",
		RACE_PC_AVAIL | RACE_DETECT_MAGIC | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3, -3,  3,  2,  2, -3,  0,  3,  0, /* Drink Eat */  -1, 0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{ -12,  -8,  -8,  -8,  -8,  -8,  -6,   4, -10  },  -80,  -80, "�Ť�@��",
		"Elf Gnome Githyanki Vampire Werewolf",
		50, 50
	},
	{ "Vampire", "�l�尭",
		RACE_PC_AVAIL | RACE_FLY | RACE_INFRAVISION | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_VAMPIRE | RACE_UNDEAD,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3,  0,  1, -1,  2, -2,  1,  1,  1, /* Drink Eat */  3,-1,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -7,  15,  -4,  -5,  -4,  -4,  -5,   8, -20  },  -70, -100, "�ϫl�@��",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Werewolf Goblin Angel Gnome Lizardman",
		30, 200
	},
	{ "Beholder", "���]",
		RACE_PC_AVAIL | RACE_FLY | RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_BEHOLDER,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3, -5,  6,  4,  0, -5,  0,  5,  0, /* Drink Eat */  1, 1,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{ -15,  -9,  -9,  -9,  -9,   0,  -9,   0,   0  },  -30,  -70, "�����@Ĳ", 
		"Lizardman",
		0, 200
	},
	{ "Lizardman", "�h�i�H",
		RACE_PC_AVAIL | RACE_SWIM | RACE_WEAPON_WIELD | RACE_LIZARDMAN,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_4,  3, -5, -4,  1,  3,  2,  0,  2, /* Drink Eat */  1, 2,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  10,   0, -15,   0,  -2, -25,   0,   0,   0  },  -50, -180, "�Ť�@��", 
		"Vampire Werewolf Ghost",
		10, 10
	},
	{ "Horse", "��",
		RACE_WEAPON_WIELD | RACE_MUTE,
		SIZE_3, 3,-5,-5, 3, 4, 0, 0, 0, 2, 3,
		{8,2,2,2,2,1,0,0,0},-20,-60, "�[�t�ļ�",
		"Kobold Half-kobold Ghost",
		0, 0
	},
	{ "God", "��",
		RACE_WATERBREATH | RACE_FLY | RACE_SWIM | RACE_WATERWALK | RACE_SANCT | RACE_DETECT_MAGIC 
		| RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_5, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0,
		{-15,-15,-15,-15,-15,-15,-15,-20,-20},-200, -200, "�p�^�@��",
		"",
		0, 0
	},
	{ "Demon", "�c�]",
		RACE_REGENERACY | RACE_WEAPON_WIELD | RACE_INFRAVISION | RACE_NO_POISON 
		| RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_UNDEAD,
		SIZE_5, 8, 8, 5, 6, 8, 0, 0, 0, 0, 0,
		{-20,-10,-10,-10,-10,-10,-10,20,-20}, -55, -500, "�Ť�@��",
		"Angel Elf Dwarf God",
		50, 200
    },
	{ "Angel", "�Ѩ�",
		RACE_FLY | RACE_PROTECTION | RACE_DETECT_INVIS | RACE_WEAPON_WIELD |RACE_DETECT_MAGIC,
		SIZE_4, 5, 5, 5, 8, 3 ,0, 0, 0, 0, 0,  {-10,-10,-10,-10,-10,-10,-10,-20,20},-150, -60, "�Ť�@��",
		"Demon",
		0, 0
	},
	{ "Dragon", "�s",
		RACE_FLY | RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_8, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,  {-9,-7,-7,-7,-7,-30,-3,0,0},  -10, -70, "�ϫl�@��",
		"",
		50 ,300
	},
	{ "Half-elf", "�b���F",
		RACE_INFRAVISION | RACE_DETECT_MAGIC | RACE_WEAPON_WIELD,
		SIZE_3,2, 3, 1, 5,1, 0, 0, 0, 0, 0,  {-6,-5,-5,-6,-5,-6,-5,-2,-2},-100, -100, "�Ť�@��",
		"Ogre Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin",
		50, 50
	},
	{ "Half-dwarf", "�b�G�H",
		RACE_WEAPON_WIELD,
		SIZE_3, 1,-1, 0,-1, 1, 0, 0, 0, 0, 0,  {-3,-2,1,1,-2,-1,1,-1,-1}, 10, -80, "�Ť�@��",
		"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin",
		50, 50
	},
	{ "Half-kobold", "�b�a�F",
		RACE_DETECT_ALIGN | RACE_WEAPON_WIELD,
		SIZE_2,-1, 1, 2,-2, 0, 0, 0, 0, 0, 0, {4,2,-1,2,-1,1,-2,-1,-1}, 10,110, "�Ť�@��", 
		"Ogre Orc Giant Troll Hobgoblin",
		50, 50
	},
	{ "Giant", "���H",
		RACE_WEAPON_WIELD,
		SIZE_7, 4,-2,-2,-3, 3, 0, 0, 0, 2, 4,  {8,3,3,3,3,1,4,0,0}, 30,-20, "�����@��",
		"Demon Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 200
	},
	{ "Ogre", "���]",
		RACE_WEAPON_WIELD,
		SIZE_6, 4,-3,-2,-1, 2, 0, 0, 0, 0, 2,  {4,1,-4,1,1,0,1,0,-2}, 15, -10, "�����@��",
		"Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 150
	},
	{ "Kobold", "�a�F",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_2,-1,-1, 0, 1, 1, 0, 0, 0, 0, 0, {8,4-2,4,-2,2,-1,3,-3},  0,130, "�Ť�@��",
		"Elf Imp Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome Half-kobold",
		80, 80
	},
	{ "Minotaur",  "���Y��",
		RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_6, 2, 0, 0,-1, 1, 0, 0, 0, 0, 3,  {4,-4,4,4,-4,-2,0,3,-3},  -10, -30, "�����@��",
		"Elf Imp Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		30, 120
	},
	{ "Troll", "���H��",
		RACE_WEAPON_WIELD,
		SIZE_6, 3,-1,-1,-2, 1, 0, 0, 0, 0, 8,  {-2,0,-2,-2,0,0,0,0,0}, -5,100, "�����@��",
		"Human Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		30, 150
	},
	{ "Hobgoblin", "�j�c��",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_4, 1, 0,-1, 0, 0, 0, 0, 0, 0, 0, {6,5,5,5,5,3,-2,0,0},-10, 90, "�Ť�@��",
		"Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 50
	},
	{ "Insect", "����",
		RACE_MUTE,
		SIZE_1,-4,-4,-4, 0,-4, 0, 0, 0, 0, 0, {12,8,8,8,8,5,-2,0,0},-60, 80, "�ΤO�@�r",
		"",
		0, 0
	},
	{ "Animal", "�ʪ�",
		RACE_DETECT_HIDDEN | RACE_MUTE,
		SIZE_3, 1,-3,-1, 1, 1, 0, 0, 0, 0, 0, {8,3,3,3,3,2,1,0,0},-20, 80, "�ΤO�@�r",
		"Kobold Half-kobold Ghost",
		0, 0
	},
	{ "Undead", "�����t",
		RACE_INFRAVISION | RACE_DETECT_INVIS | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_UNDEAD,
		SIZE_3, 0, 0, 0, 1,-1, 0, 0, 0,-1,-1,  {0,10,-10,0,-10,8,0,8,-8},15, 75, "�����@Ĳ",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Goblin Angel Gnome Animal",
		0, 200
	},
	{ "Harpy", "�H����",
		RACE_FLY | RACE_DETECT_INVIS | RACE_WEAPON_WIELD,
		SIZE_3, 0,-1,-1, 2, 0, 0, 0, 0, 0, 0,  {-3,5,5,5,5,2,-2,3,-3}, -5, 85, "�ϫl�@��",
		"Human Imp Elf Half-elf Dwarf Half-dwarf Hobbit Gnome",
		30, 30
	},
	{ "Bear", "��",
		RACE_WATERWALK | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_5, 1, 0, 0,-1, 1, 0, 0, 0, 0, 3,  {4,2,2,2,2,1,0,0,0},  0, 35, "���x�@��",
		"",
		0, 0
	},
	{ "Githyanki", "�]�F",
		RACE_WEAPON_WIELD,
		SIZE_3, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0,  {-6,6,-6,-6,6,2,-10,5,-5}, -5, 75, "�Ť�@��",
		"Mindflayer",
		50, 150
	},
	{ "Elemental", "����",
		RACE_NO_POISON,
		SIZE_4, 1, 0, 0, 0, 1, 0, 0, 0,-2,-2,  {0,-12,-12,-12,-12,0,-10,0,0},-30, 40, "�r�O�@��",
		"",
		0, 0
	},
	{ "Bat", "����",
		RACE_FLY | RACE_INFRAVISION | RACE_MUTE,
		SIZE_1,/* Five ATR */ -2, 0, -1, 4,-3, /*HP MP MV */ 0, 0, 4, /* Drink Eat */0, 0, 
		/* �k, ��, ��, ��, �g, �s, �p, �t, �� */
		{   4,  6,  6,  6,  6,  2,  0,  0,  0},-50,120, "�ΤO�@�r",
		"",
		0, 0
	},
	{ "Plant", "�Ӫ�",
		RACE_MUTE | RACE_WEAPON_WIELD,
		SIZE_2, 0, 0, 0,-2, 1, 0, 0, 0,-1,-1,  {-10,20,10,-10,-20,10,4,0,0}, 100,150, "�����@��",
		"",
		0, 0
	},
	{ "Rat", "��", RACE_PASSDOOR | RACE_MUTE,
		SIZE_1,-1, 0, 0, 2,-1, 0, 0, 0, 0, 0, {6,5,5,5,5,3,0,0,-2},-20, 95, "�ΤO�@�r",
		"",
		0, 0
	},
    { "Werewolf",  "�T�H",
		RACE_PC_AVAIL | RACE_INFRAVISION | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD | RACE_WEREWOLF,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_4,  2, -3,  0,  2,  3,  2,  0,  2, /* Drink Eat */  0, 3,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -3,   5,  -3,  -3,  -3,  -3,   5,   0,   0  },  -50, -100, "�ϫl�@��",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Goblin Angel Gnome",
		100, 100
	},
	{ "Goblin", "�p�c��",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_2,-1,-1,-1, 1, 0, 0, 0, 0, 0, 0, {6,5,5,5,5,3,-1,0,0}, -5,100, "�Ť�@��",
		"Elf Half-elf Dwarf Half-dwarf Hobbit Vampire Werewolf Gnome",
		50, 50
	},
	{ "Arachnid",  "�j��",
		RACE_NO_ABILITIES | RACE_WEAPON_WIELD | RACE_MUTE,
		SIZE_2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, {0,5,0,0,0,2,-2,1,-1},-20,100, "�ΤO�@�r",
		"",
		0, 0
	},
	{ "Mind-flayer","�ܤ��]",
		RACE_INFRAVISION | RACE_WEAPON_WIELD,
		SIZE_3, 1, 2, 1,-1, 0, 0, 6, 0, 0, 0, {-6,-6,6,6,-6,2,-3,3,-3},-30, 90, "�Ť�@��",
		"Githyanki",
		100, 100
	},
	{ "Object", "���~",
		RACE_WATERBREATH | RACE_MUTE | RACE_NO_POISON,
		SIZE_2, 3, 0, 0, 0, 3, 0, 0, 0,-2,-2, {-20,10,10,10,10,10,3,0,0} ,100,170, "�ΤO�@��",
		"",
		0, 0
	},
	{ "Mist", "��",
		RACE_FLY | RACE_PASSDOOR | RACE_NO_POISON | RACE_WEAPON_WIELD,
		SIZE_0, 0, 0, 0, 3, 0, 0, 0, 0,-2,-2,  {-5,-5,-5,10,-5,-5,-3,0,0},-50, 140, "����@�Q",
		"",
		0, 0
	},
	{ "Snake", "�D",
		RACE_MUTE,
		SIZE_2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  {0,3,3,3,3,2,0,0,0},-10, 80, "�ΤO�@�r",
		"",
		0, 0
	},
	{ "Worm", "��",
		RACE_PASSDOOR | RACE_MUTE | RACE_WEAPON_WIELD,
		SIZE_8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {6,3,3,-10,-10,5,-1,0,0},  5,115, "�ΤO�@�r",
		"",
		0, 0
	},
	{ "Fish", "��",
		RACE_WATERBREATH | RACE_SWIM | RACE_MUTE,
		SIZE_1, 0, 0, 0, 2, 0, 0, 0, 0,-2, 0,  {6,10,10,-10,-10,0,8,0,0},-10, 80, "�ΤO�@��",
		"",
		0, 0
	},
	{ "Hydra", "�E�Y���D��",
		RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_8, 2, 0, 0,-1, 2, 0, 0, 0, 0, 0,  {-5,-4,-4,-4,-4,-4,4,0,0}, -10, 90, "�ΤO�@�r",
		"",
		50, 200
	},
	{ "Lizard", "�h�i",
		RACE_MUTE,
		SIZE_1,-1, 0, 0, 1, 0, 0, 0, 0, 0, 0, {12,-3,-3,-3,-3,-1,0,0,0},-15, 85, "���ڤ@��",
		"",
		0, 0
	},
	{ "Pixie",     "�p����",
		RACE_FLY | RACE_WEAPON_WIELD,
		SIZE_1,-5, 4, 2, 3,-4, 0, 0, 0, 0,-1, {-4,2,2,2,2,1,-3,0,0},-45, 65, "�Ť�@��",
		"Ogre Kobold Minotaur Goblin Hobgoblin Dwarf HalfDwarf Half-elf Half-kobold Beholder",
		30, 30
	},
	{ "Elementx", "�����ͪ�", RACE_WEAPON_WIELD,
		SIZE_4, 1, 0, 0, 0, 1, 0, 0, 0,-2,-2,  {5,-8,-8,-8,-8,-8,0,0,0},-20, 50, "�Ť�@��",
		"",
		30, 30
	},
	{ "Phantom", "�ۼv",
		RACE_FLY | RACE_PASSDOOR | RACE_WEAPON_WIELD | RACE_NO_POISON,
		SIZE_0, 0, 0, 0, 3, 0, 0, 0, 0,-2,-2,  {-8,0,3,6,-10,-5,0,0,0},-50, 40, "����@�Q",
		"",
		0, 0
	},
	{ "Statue", "�J��",
		RACE_MUTE | RACE_WEAPON_WIELD | RACE_NO_POISON,
		SIZE_5, 1, 0, 0,-5, 4, 0, 0, 0,-2,-2,  {-10, 0, 0,-10,10,10,-10,0,0}, 50, 0, "�r�P�@��",
		"",
		0, 0
	},
	{ "Robot", "����",
		RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_ANTI_MAGIC,
		SIZE_4, 0, 0, 0, 0, 0, 0, 0, 0,-2,-2,  {-10,5,5,5,5,5,5,0,0}, 10, 70, "�r�P�@��",
		"",
		0, 0
	},
	{ "Ghost", "����",
		RACE_INVIS | RACE_PASSDOOR | RACE_FLY | RACE_INFRAVISION | RACE_DETECT_INVIS 
		| RACE_UNDEAD | RACE_WEAPON_WIELD,
		SIZE_0,-1, 0, 0, 2,-1, 0, 0, 0,-1,-1, {-5,3,3,3,3,2,0,8,-8},-30, 70, "�����@Ĳ",
		"Animal Drow Kobold Lizardman Mindflayer Human Gnome Hobbit",
		0, 0
	},

	{ "Halfling", "�b���H",
		RACE_WEAPON_WIELD,
		SIZE_2, 1,-1, 0, 0, 0, 0, 0, 0, 0, 0,  {0,0,0,0,0,0,1,-5,-5},-10, 80, "�Ť�@��",
		"",
		100, 100
	},

	{ "Beast", "�~��",
		RACE_INFRAVISION | RACE_DETECT_HIDDEN | RACE_REGENERACY | RACE_WEAPON_WIELD,
		SIZE_3, 3,0, 3, 5, 5, 3, 0, 0, 1, 3, {2,5,5,-5,-5,-2,-5,0,0},-30,  -50, "�Q���çS",
		"Kobold Half-kobold Ghost Orc Beholder",
		0, 0
	},

	{ "Centaur",   "�H��",
		RACE_PC_AVAIL | RACE_DETECT_INVIS | RACE_WEAPON_WIELD | RACE_INFRAVISION | RACE_CENTAUR,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_5,  1, -3, -2,  3,  1,  1,  0,  1, /* Drink Eat */  2, 1,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -4,  -4,   5,  -4,  10,   0,  -4,   0,   0  },  -70,  -70, "�K��@��",
		"Kobold Ghost Vampire Orc Troll Goblin",
		50, 50
	},
	{ "Dinosaur", "���s",
		RACE_WEAPON_WIELD|RACE_MUTE|RACE_INFRAVISION,
		SIZE_9, 3, -2, -2, 0, 3, 0, 0, 0, 0, 0,  {2,-3,-3,-3,-3,-10,0,0,0},  50, -100, "�ϫl�@��",
		"",
		0, 0
	},
	{ "Drake", "�s�~",
		RACE_FLY | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
		SIZE_7, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0,  {0,-2,-2,-2,-2,-4,-4,0,0},  0, 20, "�ϫl�@��",
		"",
		50, 50
	},
	{ "Wyrm", "���s",
		RACE_INFRAVISION|RACE_DETECT_HIDDEN|RACE_REGENERACY | RACE_WEAPON_WIELD,
		SIZE_8, 4, 1, 1, 0, 3, 0, 0, 0, 0, 0,  {0,-5,-5,-5,-5,-7,0,0,0},  20, -20, "�����@��",
		"",
		50, 200
	},
	{ "Wyvern", "���s",
		RACE_FLY |  RACE_WEAPON_WIELD,
		SIZE_8, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0,  {-2,-2,-2,-2,-2,-4,4,0,0}, -10, 50, "�ϫl�@��",
		"",
		50, 150
	},
	{ "Eagle", "�N",
		RACE_FLY,
		SIZE_3, 0,-1,-1, 2, 0, 0, 0, 0, 0, 0,  {-3,5,5,5,5,-2,0,0}, -5, 85, "�ϫl�@��",
		"",
		30, 30
	},
	{ "Imp", "����",
		RACE_FLY | RACE_WEAPON_WIELD,
		SIZE_2,-4, 4, 2, 2,-4, 0, 4, 0, 0,-1, {-8,5,5,5,5,4,-1,3,-3},-60, 120, "�Ť�@��",
		"Kobold Minotaur Angel Goblin Dwarf HalfDwarf Half-elf Half-kobold Beholder",
		50, 50
	},
	{ "Merfolk", "���H",
		RACE_WEAPON_WIELD | RACE_WATERBREATH | RACE_SWIM | RACE_WATERWALK,
		SIZE_3, 0, 1, 0, 1, 0, 0, 0, 0, -1, 0, {1, 5, -5, 5, -5, -5, 3,0,0}, -20, 0, "�Ť�@��",
		"Vampire Lizardman Drow Dinosaur Kuo-toa", 50, 50
	},
	{ "Kuo-toa", "�F�ܳ��H",
		RACE_WEAPON_WIELD | RACE_WATERBREATH | RACE_INFRAVISION | RACE_SWIM | RACE_WATERWALK,
		SIZE_4, 2, -1, -1, 0, 1, 0, 0, 0, -1, 0, {1, 5, -5, 5, -5, -5, 3,0,0}, 0, -20, "�Ť�@��",
		"Elf Dinosaur Merfolk Drow",
		50, 50
	},
	{ "Bachi", "�_�C",
		RACE_INVIS | RACE_INFRAVISION | RACE_NO_POISON | RACE_PASSDOOR | RACE_DETECT_INVIS
		| RACE_DETECT_HIDDEN | RACE_DETECT_ALIGN | RACE_FLY | RACE_SANCT | RACE_WEAPON_WIELD
		| RACE_PROTECTION | RACE_DETECT_MAGIC | RACE_REGENERACY | RACE_ANTI_MAGIC | RACE_WATERWALK
		| RACE_WATERBREATH,
		SIZE_9, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, {20,20,20,20,20,20,20,20,20},-500,-500, "�H�a�R��",
		"",
		100, 1000
	},
	{ "Mummy", "��D��",
		RACE_PC_AVAIL | RACE_REGENERACY | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_UNDEAD ,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3,  3, -1,  1, -1, -2,  3,  0,  0, /* Drink Eat */  -1, 0,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -15, 15,  -8,  -8, -15,  -10,  0,  10, -25  },  -40,  -80, "�ϫl�@��",
		"Human Elf Half-elf Drow Dwarf Half-dwarf Hobbit Giant Ogre Orc Minotaur Hobgoblin Goblin Angel Gnome",
		30, 200
	},

	{ "Gargoyle", "�۹���",
		RACE_PC_AVAIL | RACE_WEAPON_WIELD | RACE_NO_POISON | RACE_UNDEAD | RACE_FLY | RACE_GARGOYLE,
			 /* �O, ��, �z, ��, ��, HP, MP, MV,*/
		SIZE_3, -2,  1,  1,  3, -3,  1,  0,  1, /* Drink Eat */  -1, 2,
		/* �k,  ��,  ��,  ��,  �g,  �s,  �p,  �t,  ��       DC,   AC, */
		{  -6,  15,  -5,  10,  -3,  -5,  15,  10, -10  },  -60,  -30, "�ϫl�@��",
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

/* ac �q con 10 �}�l�W�[ 25 2022/02/23 */
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
  {  "�M��",       "water",             "�z���L�⪺",     {   0,    0,    5  } }, /* 0 */
  {  "��s",       "beer",              "�H�Ħ⪺",       {   4,    1,    3  } },
  {  "�s",         "wine",              "�����⪺",       {  10,    0,    3  } },
  {  "���s",       "ale",               "�Ħ⪺",         {   4,    1,    3  } },
  {  "�³��s",     "dark-ale",          "�t�Ħ⪺",       {   4,    1,    3  } },
  {  "�¤h��",     "whisky",            "�[�Ħ⪺",       {  20,    0,    0  } }, /* 5 */
  {  "�f�c��",     "lemonade",          "�B�զ⪺",       {   0,    0,    5  } },
  {  "�P�s",       "firebreather",      "������",         {  30,    0,    0  } },
  {  "��a�S��",   "local speciality",  "�M����",         {   9,    1,    2  } },
  {  "�C���",     "slime mold juice",  "�`��⪺",       {   0,    4,   -1  } },
  {  "����",       "milk",              "�զ⪺",         {   0,    1,    3  } }, /* 10 */
  {  "��",         "tea",               "��M�H����",     {   0,    0,    5  } },
  {  "�@��",       "coffee",            "�t���⪺",       {   0,    0,    4  } },
  {  "��",         "blood",             "�A���⪺",       {   0,    4,   -1  } },
  {  "�Q��",       "salt water",        "�z���L�⪺",     {   0,    0,   -5  } },
  {  "�i��",       "cola",              "���⪺",       {   0,    2,    1  } }, /* 15 */
  {  "�ï]����",   "bubble milk tea",   "���Ŧ��ƪ�",     {   0,    2,    1  } },
  {  "����",       "milk tea",          "���Ŧ�",         {   0,    2,    4  } },
  {  "���",       "green tea",         "����⪺",       {   0,    0,    5  } },
  {  "����",       "black tea",         "�`�Ŧ⪺",       {   0,    0,    5  } },
  {  "�i������",   "calpis",            "�ťզ⪺",       {   0,    2,    1  } }, /* 20 */
  {  "�@��",       "soup",              "�@�Y��",         {   0,    4,    1  } },
  {  "�h���",     "orange juice",      "��⪺",         {   0,    2,    2  } },
  {  "����",       "sweet soup",        "�����@����",     {   0,    3,    2  } },
  {  "�B��",       "ice",               "�B�D��",         {   0,    0,    5  } }, /* 24 */
  {  "���l��",     "coconut juice",     "�z���V�B��",     {   0,    1,    5  } },
  {  "����",       "porridge",          "�V�B�@�Y��",     {   0,    2,    3  } },
  {  "�G��",       "juice",             "������",         {   0,    1,    3  } },
  {  "�Żs���~",   "dairy beverage",    "�ťխ��@��",     {   0,    2,    2  } },
  {  "�C���",     "grass tea",         "�`�Ŧ⪺",       {   0,    1,    3  } }  /* 29 */
};


#define SKILL_APP 440
#define SKILL_LIMIT( level, learn , sklvlimit ) ( (learn) * 1000 + (level + 1)*10 + sklvlimit )

const struct skill_type skill_table [MAX_SKILL] =
{

    /*
     * Magic spells.
     */
    {
        "reserved", "�O�d", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            0,   TAR_IGNORE,  POS_STANDING,
            NULL,   0,  0,
            "",   "", ATTR_ALL, {0,0,0,0,0} // {STR,DEX,INT,WIS,CON}
    },
    
    {
        "acid blast", "���z�N", { SKILL_LIMIT(20,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(20,80,6), SKILL_LIMIT(25,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   20, 2,
            "���z�N",  "!Acid Blast!", ATTR_ALL, {5,3,1,2,5}
    },

    {
        "armor", "�@�ҳN", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(1,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP },
            spell_armor,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   25, 2,
            "",   "�A�Pı���@�O�U���F.", ATTR_ALL, {5,5,4,4,5}

    },

    {
        "bless", "���ֳN", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), SKILL_LIMIT(2,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_APP, SKILL_LIMIT(3,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_bless,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_bless,   25, 2,
            "",   "�Aı�o���W�����֮����F. ", ATTR_ALL, {8,7,7,7,10}

    },

    {
        "blindness", "���سN", { SKILL_LIMIT(8,85,6), SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_APP, 
            SKILL_LIMIT(30,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(3,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_blindness,  20, 2,
            "",   "�A���e��M�@�G, �A�S�i�H�ݨ��@���F!", ATTR_ALL, {7,6,6,6,7}

    },

    {
        "paralyze", "�·��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_paralyze, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_paralyze,  35, 4,
            "",   "�A���|�ϤS�ଡ�ʦۦp�F.", ATTR_ALL, {8,8,6,7,8}

    },

    {
        "breathe water", "�����I�l�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_breathe_water, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_breathe_water,  5, 4,
            "����",  "�A����b�����I�l�F.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "burning hands", "�`�����x", { SKILL_LIMIT(5,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(2,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_burning_hands, 25, 4,
            "�`�����x", "�A���W�U�N�����K��������", ATTR_ALL, {2,2,2,1,3}

    },

    {
        "call lightning", "�{�q�l��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_call_lightning, TAR_IGNORE,  POS_FIGHTING,
            NULL,   18, 8,
            "�{�q�l��N", "�A���Ǫ��p�������F.", ATTR_ALL, {3,2,1,2,3}

    },

    {
        "cause critical", "���׶ˮ`�N", { SKILL_APP, SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   200, 0,
            "���׶ˮ`�N",  "!Cause Critical!", ATTR_ALL, {3,2,2,1,3}

    },

    {
        "cause light", "���׶ˮ`�N", { SKILL_APP, SKILL_LIMIT(1,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   50, 0,
            "���׶ˮ`�N",  "!Cause Light!", ATTR_ALL//, {2,2,1,1,2}

    },

    {
        "cause serious", "���׶ˮ`�N", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), SKILL_LIMIT(2,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   100, 0,
            "���׶ˮ`�N",  "!Cause Serious!", ATTR_ALL, {3,2,1,1,1}

    },

    {
        "change sex", "�ʧO�ܧ�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 2,
            "",   "�Aı�o����_�F�ǲ��˪��ܤ�.", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "charm person", "�y�b�N", { SKILL_LIMIT(15,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(8,60,6), SKILL_LIMIT(15,80,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(6,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_charm_person, 35, 0,
            "",   "�A��ۤv�S���H�ߤF.", ATTR_ALL, {5,4,3,3,5}

    },

    {
        "chill touch", "�H����Ĳ", { SKILL_LIMIT(3,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(5,90,6), SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_chill_touch,   30, 2,
            "�H����Ĳ", "�Aı�o�ŷx�h�F.", ATTR_ALL, {5,4,4,3,4}

    },

    {
        "colour spray", "�B���N", { SKILL_LIMIT(12,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(7,99,6), SKILL_LIMIT(7,99,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(6,99,6), SKILL_APP, 
            SKILL_LIMIT(31,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(7,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_colour_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   45, 4,
            "�B���N",  "�Aı�o�ŷx�h�F.", ATTR_ALL, {5,5,4,4,5}

    },

    {
        "cone of silence", "�H�q�O��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cone_of_silence, TAR_IGNORE,  POS_FIGHTING,
            NULL,   35, 12,
            "",   "!Cone of Silence!", ATTR_ALL, {4,3,2,2,3}

    },

    {
        "continual light", "�]�k���y�N", { SKILL_LIMIT(4,85,6), SKILL_LIMIT(2,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP },
            spell_continual_light, TAR_IGNORE,  POS_STANDING,
            NULL,   7, 0,
            "",   "!Continual Light!", ATTR_ALL, {3,2,1,1,3}

    },

    {
        "control weather", "�I����B�N", { SKILL_LIMIT(10,85,6), SKILL_LIMIT(15,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(8,90,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(14,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_control_weather, TAR_IGNORE,  POS_STANDING,
            NULL,   25, 20,
            "",   "!Control Weather!", ATTR_ALL, {3,2,1,2,2}

    },

    {
        "create food", "�����s�y�N", { SKILL_APP, SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(1,90,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_create_food, TAR_IGNORE,  POS_STANDING,
            NULL,   5, 20,
            "",   "!Create Food!", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "create spring", "�y�u�N", { SKILL_LIMIT(10,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_create_spring, TAR_ARG,  POS_STANDING,
            NULL,   20, 20,
            "",   "!Create Spring!", ATTR_ALL, {2,1,1,1,2}

    },

    {
        "create water", "�y���N", { SKILL_APP, SKILL_LIMIT(2,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(1,90,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_create_water, TAR_OBJ_INV,  POS_STANDING,
            NULL,   5, 0,
            "",   "!Create Water!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "cure blindness", "���تv���N", { SKILL_APP, SKILL_LIMIT(4,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(9,90,6), 
            SKILL_APP, SKILL_LIMIT(2,95,6), SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP },
            spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   5, 4,
            "",   "!Cure Blindness!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "cure critical", "�������˳N", { SKILL_APP, SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(12,90,6), 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP },
            spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   25, 0,
            "�������˳N",   "�A���������ˮĪG�����F�I", ATTR_ALL, {3,2,2,1,3}

    },

    {
        "cure light", "������˳N", { SKILL_APP, SKILL_LIMIT(1,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   8, 0,
            "������˳N",   "!Cure Light!", ATTR_ALL, {2,1,1,1,1}

    },

    {
        "cure poison", "�Ѭr�N", { SKILL_APP, SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP },
            spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   5, 4,
            "",   "!Cure Poison!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "cure serious", "�������˳N", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), 
            SKILL_APP, SKILL_LIMIT(3,95,6), SKILL_LIMIT(9,95,6), SKILL_APP, SKILL_APP },
            spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 0,
            "�������˳N",   "!Cure Serious!", ATTR_ALL, {2,2,2,1,1}

    },

    {
        "curse", "�A�G�N", { SKILL_LIMIT(12,85,6), SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_APP, SKILL_LIMIT(8,99,6), SKILL_APP, 
            SKILL_LIMIT(28,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(9,90,6), 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_curse,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_curse,  25, 2,
            "�A�G�N",  "�Aı�o�A�G����ŵ�w�g���A�ӥh�F. ", ATTR_ALL, {7,6,5,5,7}

    },

    {
        "destroy cursed", "�A�G�Ѱ��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_destroy_cursed, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   20, 10,
            "",   "!destroy cursed!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "detect evil", "���c����N", { SKILL_APP, SKILL_LIMIT(4,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_detect_evil, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "�A���u�������⨸������F.", ATTR_ALL, {1,1,1,1,1}

    },

    {
        "detect hidden", "�ðΰ���N", { SKILL_APP, SKILL_LIMIT(7,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP },
            spell_detect_hidden, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "�A��P�D��ĵı�O�U���F. ", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "detect invis", "���ΰ���N", { SKILL_LIMIT(2,85,6), SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_detect_invis, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "�A�L�k�A�ݨ����Ϊ��ƪ��F.", ATTR_ALL, {2,2,1,1,1}

    },

    {
        "detect magic", "�]�k����N", { SKILL_LIMIT(2,85,6), SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_detect_magic, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   5, 0,
            "",   "�A����P���]�k�F.", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "detect poison", "�r������N", { SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(5,95,6), 
            SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_detect_poison, TAR_OBJ_INV,  POS_STANDING,
            NULL,   5, 0,
            "",   "�A��r������ı�O�U���F.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "dispel evil", "���]�N", { SKILL_APP, SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   30, 2,
            "���]�N",  "!Dispel Evil!", ATTR_ALL, {2,2,1,2,3}

    },

    {
        "dispel good", "���t�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dispel_good, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   30, 2,
            "���t�N",  "!Dispel Evil!", ATTR_ALL, {2,2,1,2,3}
                                                                                
    },

    {
        "dispel magic", "�Ѱ��]�k", { SKILL_LIMIT(20,85,6), SKILL_LIMIT(15,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(5,85,6), SKILL_LIMIT(10,90,6), SKILL_APP, 
            SKILL_LIMIT(36,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dispel_magic, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 8,
            "",   "!Dispel Magic!", ATTR_ALL, {4,3,4,3,4}

    },

    {
        "earthquake", "�a�_�N", { SKILL_APP, SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earthquake, TAR_IGNORE,  POS_FIGHTING,
            NULL,   65, 6,
            "�a�_�N",  "�A���|�ϤS�ଡ�ʦۦp�F.", ATTR_ALL, {4,4,3,3,4}

    },

    {
        "enchant weapon", "�Z���W�j�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_enchant_weapon, TAR_OBJ_INV,  POS_STANDING,
            NULL,   100, 12,
            "",   "�A���W���Z���W�j�N�����F.", ATTR_ALL, {8,8,7,7,9}

    },

    {
        "energy drain", "�l��N", { SKILL_LIMIT(14,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_LIMIT(22,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   60, 4,
            "�l��N",  "�A����O���A�ֳt�����F.", ATTR_ALL, {4,4,3,4,5}

    },

    {
        "exorcise", "�X���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_exorcise, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   35, 4,
            "",   "�A���W���X���N�����F.", ATTR_ALL, {3,3,2,1,3}

    },

    {
        "faerie fire", "�ѨϤ���", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_LIMIT(16,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_faerie_fire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_faerie_fire,   15, 2,
            "�ѨϤ���",  "�A���W����������~���������F.", ATTR_ALL, {6,5,4,4,6}

    },

    {
        "faerie fog", "�ѨϤ���", { SKILL_LIMIT(10,85,6), SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(7,99,6), SKILL_LIMIT(7,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(8,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_faerie_fog, TAR_IGNORE,  POS_STANDING,
            NULL,   25, 4,
            "�ѨϤ���",  "�A���Ǫ��ѨϤ��������F.", ATTR_ALL, {3,4,2,2,3}

    },

    {
        "fireball", "���y�N", { SKILL_LIMIT(20,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_LIMIT(20,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fireball,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   45, 4,
            "���y�N",  "�A���Ǫ����y�����F.", ATTR_ALL, {7,5,4,5,7}

    },

    {
        "powerball", "��q�y", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(22,99,6), SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_powerball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   175, 8,
            "��q�y",  "�A���Ǫ���q�y�����F.", ATTR_ALL, {7,5,4,6,7}

    },

    {
        "star fire", "�P�P����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_starfire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   175, 8,
            "�P�P����",  "�A���Ǫ��P�P���������F.", ATTR_ALL, {9,8,6,7,9}

    },
        
    {
        "flamestrike", "�t����", { SKILL_APP, SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(8,85,6), SKILL_LIMIT(10,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   40, 4,
            "�t����",  "�A���Ǫ��t�������F.", ATTR_ALL, {3,3,2,2,3}

    },

    {
        "fly",  "�����N", { SKILL_LIMIT(7,85,6), SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(8,85,6), SKILL_LIMIT(10,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fly,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_flying,   10, 0,
            "�P�U",   "�A�����}�C�C���^�F�a��.", ATTR_ALL, {3,3,2,2,3}

    },

    {
        "grave robber",  "�s�Ӫ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_grave_robber, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   65, 10,
            "",   "!Grave Robber!", ATTR_ALL, {8,7,6,5,8}

    },

    {
        "gate",  "�H���l��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_gate,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   50, 8,
            "",   "!Gate!", ATTR_ALL, {3,4,2,3,3}

    },

    {
        "giant strength", "���H���O", { SKILL_LIMIT(12,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(8,90,6), SKILL_LIMIT(10,90,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(9,90,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_giant,   35, 2,
            "",   "�Aı�o���ѤO�q�����F.", ATTR_ALL, {6,6,4,5,5}

    },

    {
        "harm",  "�׷��ˮ`�N", { SKILL_APP, SKILL_LIMIT(17,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), 
            SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_harm,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   40, 4,
            "�׷��ˮ`�N",  "!Harm!", ATTR_ALL, {3,2,1,1,3}

    },

    {
        "heal",  "�v���N", { SKILL_APP, SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,90,6), 
            SKILL_APP, SKILL_LIMIT(10,95,6), SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP },
            spell_heal,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 2,
            "�v���N",   "!Heal!", ATTR_ALL, {3,2,3,1,4}

    },

    {
        "identify", "���~Ų�w�N", { SKILL_LIMIT(10,85,6), SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_identify,  TAR_OBJ_INV,  POS_STANDING,
            NULL,   12, 0,
            "",   "�A�磌�~����Ų�O���C�F.", ATTR_ALL, {7,8,6,6,8}

    },

    {
        "infravision", "�]���N", { SKILL_LIMIT(6,85,6), SKILL_LIMIT(9,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(6,99,6), SKILL_APP, 
            SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(12,90,6), 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(6,95,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP },
            spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   5, 0,
            "",   "�A�L�k�A�ݨ��·t�����ƪ��F.", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "invis", "���γN", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(5,85,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(2,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_invis,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_invis,  5, 0,
            "",   "�A�����ήĪG�����F.", ATTR_ALL, {3,2,1,1,3}

    },

    {
        "know alignment", "�}����ѳN", { SKILL_LIMIT(8,85,6), SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(2,99,6), SKILL_LIMIT(3,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(12,99,6), 
            SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP },
            spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   9, 4,
            "",   "�A�L�k���ѥX�O�H���}��F.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "thunder strike", "�p���N", { SKILL_LIMIT(5,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_electrostatic_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_thunder_strike,   30, 2,
            "�p���N", "�A�q���C�ӵꪺ���A�U�^��.", ATTR_ALL, {5,4,3,3,4}

    },

    {
        "lightning bolt", "�z�����p", { SKILL_LIMIT(10,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(12,90,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   50, 4,
            "�z�����p", "�A���餣�A��Ĳ�q�P.", ATTR_ALL, {9,9,7,6,9}

    },

    {
        "locate object", "���~�w��N", { SKILL_LIMIT(6,85,6), SKILL_LIMIT(10,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,90,6), SKILL_LIMIT(5,85,6), SKILL_LIMIT(5,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_locate_object, TAR_IGNORE,  POS_STANDING,
            NULL,   20, 8,
            "",   "!Locate Object!", ATTR_ALL, {3,2,2,1,3}

    },

    {
        "magic missile", "�]�k���u", { SKILL_LIMIT(1,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(12,90,6), SKILL_LIMIT(4,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_magic_missile,   20, 0,
            "�]�k���u", "�A���Ǵo�H�����I�����F.", ATTR_ALL, {8,7,5,6,7}

    },

    {
        "mass heal", "�s��v���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(25,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,80,6), 
            SKILL_APP, SKILL_LIMIT(34,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mass_heal, TAR_IGNORE,  POS_STANDING,
            &gsn_mass_heal,  50, 3,
            "",   "!Mass Heal!", ATTR_ALL, {5,3,4,2,4}

    },

    {
        "mass invis", "�s�����γN", { SKILL_LIMIT(15,85,6), SKILL_LIMIT(17,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,85,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(15,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mass_invis, TAR_IGNORE,  POS_STANDING,
            &gsn_mass_invis, 20, 0,
            "",   "�A�����ήĪG�����F.", ATTR_ALL, {3,3,2,2,3}

    },

    {
        "mute",  "�T���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mute,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_mute,  30, 2,
            "",   "�A�S��}�f���ܤF.", ATTR_ALL, {8,7,7,7,8}

    },

    {
        "pass door", "����N", { SKILL_LIMIT(18,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,90,6), SKILL_APP, SKILL_LIMIT(18,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pass_door, TAR_CHAR_SELF,  POS_STANDING,
            NULL,   20, 6,
            "",   "�A�S�ܱo����ƤF.", ATTR_ALL, {4,2,1,3,3}

    },

    {
        "poison", "�I�r�N", { SKILL_APP, SKILL_LIMIT(8,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), 
            SKILL_LIMIT(27,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_poison,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_poison,  25, 4,
            "�I�r�N", "�Aı�o�ΪA�h�F.", ATTR_ALL, {3,3,2,2,4}

    },

    {
        "disease", "�e�f�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_disease,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_disease,  30, 6,
            "�e�f�N", "�Aı�o�ΪA�h�F.", ATTR_ALL, {3,3,2,2,4}

    },


	{
        "fear", "���߳N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fear,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_fear,  30, 6,
            "���߳N", "�Aı�o�w�w�h�F.", ATTR_ALL, {3,3,2,2,4}

    },

    {
        "polymorph other", "�رڲ��ܳN", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_polymorph_other, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_polymorph,   20,8,
            "�رڲ��ܳN",   "�A������S�ܦ^��ˤF." , ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "protection", "�Ѩϥ[�@�N", { SKILL_APP, SKILL_LIMIT(6,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP },
            spell_protection, TAR_CHAR_SELF,  POS_STANDING,
            NULL,   5, 2,
            "",   "�A���A����O�@.", ATTR_ALL, {6,5,4,4,6}

    },

    {           
        "recharge item", "���~�R��N",    { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(20,99,6), SKILL_LIMIT(25,90,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_recharge_item, TAR_OBJ_INV,  POS_STANDING,
            NULL,   25, 16,
            "���~�H��",  "!Recharge Item!", ATTR_ALL, {6,5,4,4,6}

    }, 

    {
        "refresh", "��O��_�N", { SKILL_LIMIT(5,85,6), SKILL_LIMIT(3,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(3,85,6), SKILL_LIMIT(3,90,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), 
            SKILL_APP, SKILL_LIMIT(1,95,6), SKILL_LIMIT(5,95,6), SKILL_APP, SKILL_APP },
            spell_refresh,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   20, 0,
            "refresh",  "!Refresh!", ATTR_ALL, {3,2,2,1,4}

    },

    {
        "remove alignment", "�}��Ѱ��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_alignment, TAR_OBJ_INV,  POS_STANDING,
            NULL,   200, 12,
            "",   "!Remove Alignment!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "remove curse", "�A�G�Ѱ��N", { SKILL_APP, SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(6,99,6), SKILL_LIMIT(8,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(8,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_curse, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   5, 8,
            "",   "!Remove Curse!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "remove silence", "�T���Ѱ��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_LIMIT(36,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_silence, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,   15, 8,
            "",   "!Remove Silence!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "sanctuary", "�t���N", { SKILL_APP, SKILL_LIMIT(20,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(15,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_sanctuary,   75, 4,
            "",   "�A���W���զ�t�������F.", ATTR_ALL, {5,4,5,3,4}

    },

    {
        "shield", "�]�k�@�޳N", { SKILL_LIMIT(13,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,90,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(22,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_shield,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   25, 2,
            "",   "�A���W���]�k���@�O���������h�F.", ATTR_ALL, {5,5,4,5,5}

    },

    {
        "shocking grasp", "�����i", { SKILL_LIMIT(8,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_LIMIT(7,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL,   25, 2,
            "�����i", "!Shocking Grasp!", ATTR_ALL, {3,2,1,2,3}

    },

    {
        "sleep", "�ʯv�N", { SKILL_LIMIT(14,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(20,90,6), SKILL_APP, 
            SKILL_LIMIT(30,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sleep,  TAR_CHAR_OFFENSIVE, POS_STANDING,
            &gsn_sleep,  35, 0,
            "",   "�Aı�o���|�ֱo�Q�ΤF.", ATTR_ALL, {3,2,1,1,2}

    },

    {
        "sober up", "���s�N", { SKILL_APP, SKILL_LIMIT(14,80,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(14,90,6),
            SKILL_APP, SKILL_LIMIT(12,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sober_up,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_soberup,  15, 4,
            "",   "!Sober!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },


    {
        "stone skin", "�ۥҳN", { SKILL_LIMIT(17,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_LIMIT(15,90,6), SKILL_LIMIT(20,99,6), SKILL_LIMIT(22,99,6), SKILL_APP, 
            SKILL_LIMIT(28,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_stone_skin, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   25, 2,
            "",   "�A���ֽ��S�ܦ^��Ӫ��ˤl�F.", ATTR_ALL, {6,5,4,4,6}

    },

    {
        "summon", "�l��N", { SKILL_APP, SKILL_LIMIT(25,60,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(5,99,6), SKILL_APP, SKILL_LIMIT(5,70,6), SKILL_LIMIT(8,80,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_summon,  TAR_IGNORE,  POS_STANDING,
            NULL,   50, 4,
            "",   "!Summon!", ATTR_ALL, {2,2,1,1,2}

    },

    {
        "teleport", "�H���ǰe�N",{ SKILL_LIMIT(15,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,90,6), SKILL_APP, SKILL_LIMIT(18,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_teleport,  TAR_CHAR_SELF,  POS_FIGHTING,
            NULL,   35, 4,
            "",   "!Teleport!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "turn undead", "�W�礣���ͪ�", { SKILL_APP, SKILL_LIMIT(30,60,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,99,6), SKILL_LIMIT(28,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_turn_undead, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_turn_undead, 45, 4,
            "�t���W��", "���t����������F,�Aı�o���c����O�S�^��A�����W.", ATTR_ALL, {3,1,2,1,3}

    },

    {
        "mass turn", "�s��W��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(37,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mass_turn, TAR_IGNORE, POS_FIGHTING,
            NULL, 80, 4,
            "�t���W��", "���t����������F,�Aı�o���c����O�S�^��A�����W.", ATTR_ALL, {5,3,4,3,5}
                                                                                
    },

    {
        "ventriloquate", "���y�N", { SKILL_LIMIT(1,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,90,6), SKILL_LIMIT(1,80,6), SKILL_LIMIT(1,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(3,90,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_ventriloquate, TAR_IGNORE,  POS_STANDING,
            NULL,   5, 0,
            "",   "!Ventriloquate!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "weaken", "�I�z�N", { SKILL_LIMIT(7,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_LIMIT(4,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_weaken,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_weaken,   15, 2,
            "�I�z�N", "�A������S�j���F�_��.", ATTR_ALL, {3,2,1,2,2}

    },

    {
        "word of recall", "���t���y", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_word_of_recall, TAR_CHAR_SELF,  POS_RESTING,
            NULL,   5, 8,
            "",   "�A����ǨӪ��W�Ҥ�����������.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "lore",  "���̤���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lore,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_lore,   20, 6,
            "",   "�A����ǨӪ��u�z���y��������.", ATTR_ALL, {3,3,2,3,3}

    },

    /*
     * Dragon breath
     */
    {
        "acid breath", "�s���j�ĦR��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_acid_breath,   175, 12,
            "�s���j�ĦR��", "�A���W���ĲG�]�o����.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "fire breath", "�s���`���R��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_fire_breath,   175, 12,
            "�s���`���R��", "�A�����餣�A�_�X����.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "frost breath", "�s���B��R��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_frost_breath,   175, 12,
            "�s���B��R��", "�A�ֽ��W���B����������.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "gas breath", "�s���r��R��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(38,99,6), SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_gas_breath, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_gas_breath,   200, 12,
            "�s���r��R��",  "�A���Ǫ��r������F.", ATTR_ALL, {6,6,6,6,6}

    },

    {
        "demon tremor", "�c�]�Ժq", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_demontremor, TAR_IGNORE,  POS_FIGHTING,
            NULL,   80, 8,
            "�c�]�Ժq",  "�A�����F�A�����R�U��!", ATTR_ALL, {5,4,3,5,6}

    },

    {
        "green breath", "���s�g�K", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_green_breath, TAR_IGNORE,  POS_FIGHTING,
            &gsn_green_breath,   120, 6,
            "���s�g�K",  "�A���A�P��`��!", ATTR_ALL, {4,3,2,4,4}

    },

    {
        "whirlwind", "�g�P�N", { SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_whirlwind, TAR_IGNORE,  POS_FIGHTING,
            NULL,   65, 6,
            "�g�P�N",  "!Whirlwind!", ATTR_ALL, {6,5,4,5,6}

    },

    {
        "lightning breath", "�s���{�q�R��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_lightning_breath,   175, 12,
            "�s���{�q�R��", "�A���W��¶���q�y�h�h.", ATTR_ALL, {6,6,6,6,6}

    },

    /*
     * Fighter and thief skills, as well as magic item skills.
     */
    {
        "backstab", "�I��", { SKILL_APP, SKILL_APP, SKILL_LIMIT(5,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), 
            SKILL_LIMIT(1,90,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(1,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_backstab,   0, 20,
            "�I��", "!Backstab!", ATTR_ALL, {7,5,8,8,7}

    },

    {
        "bash door", "����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,80,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_LIMIT(26,99,6), SKILL_LIMIT(32,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(18,99,6)},
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_bash,   0, 24,
            "�r�O�@��", "!Bash Door!", ATTR_ALL, {1,2,2,2,1}

    },

    {
        "berserk", "���O�r��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,90,6), 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(-1,90,6), 
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(18,99,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_berserk,   0,  1,
            "",   "�A���߱������q�E�Ԥ����R�U��.", ATTR_ALL, {8,9,9,9,8}

    },

    {
        "excited", "����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_excited,   0,  0,
            "",   "�A���A���򿳾�.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "circle", "¶��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(27,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_circle,   0, 6,
            "¶��",  "!Circle!", ATTR_ALL, {8,7,9,9,8}

    },

    {
        "disarm", "�����Z��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_LIMIT(20,70,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,95,6), SKILL_LIMIT(5,95,6), SKILL_LIMIT(10,85,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(8,90,6), SKILL_LIMIT(5 ,90,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_disarm,   0, 4,
            "",   "!Disarm!", ATTR_ALL, {2,3,4,4,3}

    },

    {
        "barehand", "�Ť�J�դb", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(16,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_barehand,   0, 6,
            "",   "!Barehand!", ATTR_ALL, {2,3,4,4,3}

    },

    {
        "dodge", "�{���ޥ�", { SKILL_APP, SKILL_APP, SKILL_LIMIT( 8,75,6), SKILL_LIMIT(10,70,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), 
            SKILL_LIMIT(35,99,6), SKILL_LIMIT(1,85,6), SKILL_LIMIT(1,95,6), SKILL_LIMIT(1,95,6), SKILL_LIMIT(32,85,6), 
            SKILL_APP, SKILL_LIMIT(35,85,6), SKILL_LIMIT(1, 99,6), SKILL_LIMIT(1, 95,6), SKILL_LIMIT(1, 80,6) },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_dodge,   0,  0,
            "",   "!Dodge!", ATTR_ALL, {3,1,3,3,4}

    },

    {
        "dual",  "����Z��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(20,99,6), SKILL_LIMIT(10,95,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(16,99,6), SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_dual,  0,  0,
            "",   "!Dual!", ATTR_ALL, {2,2,4,4,3}

    },

    {
        "enhanced damage", "�����O�W�j", { SKILL_APP, SKILL_LIMIT(30, 75,6), SKILL_LIMIT(25,50,6), SKILL_LIMIT(1,75,6), SKILL_LIMIT(3,70,6), 
            SKILL_APP, SKILL_LIMIT(5,85,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(3,99,6), 
            SKILL_LIMIT(15, 90,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(5,80,6), 
            SKILL_APP, SKILL_LIMIT(12,99,6), SKILL_LIMIT(15,85,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1 ,90,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_enhanced_damage, 0,  0,
            "",   "!Enhanced Damage!", ATTR_ALL, {4,5,6,6,4}

    },

    {
        "hide",  "�ðΦ���", { SKILL_APP, SKILL_APP, SKILL_LIMIT(12,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(1,99,6), 
            SKILL_LIMIT(2,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(34,85,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,95,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_RESTING,
            &gsn_hide,   0, 12,
            "",   "!Hide!", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "kick",  "���L", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,75,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(16,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,99,6), SKILL_LIMIT(4,95,6), SKILL_LIMIT(8 ,95,6)},
            spell_null,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_kick,   0,  4,
            "��ŭ���",  "!Kick!", ATTR_ALL, {5,5,8,8,6}

    },

    {
        "hunt",  "�y���ޥ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,90,6), SKILL_APP, 
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(-1,90,6), 
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_LIMIT(25,99,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6)},
            spell_null,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_hunt,   0,  8,
            "",   "!hunt!", ATTR_ALL, {-1,-1,-1,-1,-1}
    },

    {
        "assassination", "�t��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_ass,   0, 24,
            "�t��", "!Assassination!", ATTR_ALL, {7,6,8,8,7}

    },

    {
        "parry", "�Z������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(3,80,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(13,95,6), 
            SKILL_LIMIT(36,95,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(1,95,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(18,95,6), SKILL_LIMIT(15,95,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(2 ,95,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_parry,   0,  0,
            "",   "!Parry!", ATTR_ALL, {1,2,3,3,2}

    },

    {
        "critical", "�P�R�@��", { SKILL_APP, SKILL_APP, SKILL_LIMIT(15,75,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(13,99,6), 
            SKILL_LIMIT(18,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(23,95,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,50,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_critical,   0,  0,
            "",   "!Critical!", ATTR_ALL, {10, 9,11,10,11}

    },

    {
        "fool foe",  "���δ���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_fool,   0,  0,
            "",   "!Fool Foe!", ATTR_ALL, {2,2,3,3,5}

    },

    {
        "bodycontrol", "���饭�ű���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(29,70,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), 
            SKILL_LIMIT(30,99,6), SKILL_LIMIT(18,80,6), SKILL_LIMIT(18,95,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_LIMIT(22,85,6), SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_bodycontrol,   0,  0,
            "",   "!Bodycontrol!", ATTR_ALL, {6,4,7,7,5}

    },

    {
        "swordmaster", "�s�C�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,95,6), 
            SKILL_LIMIT(36,90,6), SKILL_LIMIT(25,90,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(25,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,90,6), SKILL_LIMIT(25,80,6) },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_swordmaster,   0,  0,
            "",   "!Swordmaster!", ATTR_ALL, {6,6,7,7,6}

    },

    {
        "spellmaster", "�]�k���t���y", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_spellmaster,   0,  0,
            "",   "!Spellmaster!", ATTR_ALL, {9,8,7,6,8}

    },

    {
        "spellcraft", "�k�N�j�v", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,85,6), SKILL_LIMIT(28,99,6), SKILL_LIMIT(31,85,6), SKILL_LIMIT(37,70,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,70,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_spellcraft,   0,  0,
            "",   "!Spellcraft!", ATTR_ALL, {9,5,7,8,9}

    },

    {
        "multi casting", "�h���I�k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_multicasting,   0,  0,
            "",   "!Multicasting!", ATTR_ALL, {6,6,5,5,6}

    },

    {
        "abstraction",  "�]��۴��ҳN", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_abstraction,   0,  18,
            "",   "!Abstraction!", ATTR_ALL, {7,6,5,5,6}

    },

    {
        "peek",  "�s��", { SKILL_APP, SKILL_APP, SKILL_LIMIT(8,85,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(4,90,6), 
            SKILL_LIMIT(4,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(4,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_peek,   0,  0,
            "",   "�A���A�Pı�����H���ۦb�����u.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "pick lock", "�}��", { SKILL_APP, SKILL_APP, SKILL_LIMIT(8,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(2,90,6), 
            SKILL_LIMIT(2,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(2,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_pick_lock,   0, 12,
            "",   "�A�P�쨭��Y�ǳ������G�Q���W�F.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "poison weapon", "�Z���٬r", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6), 
            SKILL_LIMIT(20,95,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_OBJ_INV,  POS_STANDING,
            &gsn_poison_weapon,  0, 12,
            "�Ĭr", "!Poison Weapon!", ATTR_ALL, {4,3,5,6,6}

    },

    {
        "rescue", "�ѱϧޥ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(15,99,6), 
            SKILL_APP, SKILL_LIMIT(25,99,6), SKILL_LIMIT(15,99,6), SKILL_LIMIT(10,99,6), SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_rescue,   0, 12,
            "",   "!Rescue!", ATTR_ALL, {1,3,3,3,1}

    },

    {
        "scrolls", "�Իw���b", { SKILL_LIMIT(4,90,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(15,75,6), SKILL_LIMIT(10,70,6), SKILL_LIMIT(16,60,6), 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(6,99,6), 
            SKILL_LIMIT(6,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(3,99,6), 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(12 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_scrolls,   0, 0,
            "�ۤ������b", "!Scrolls!", ATTR_ALL, {3,3,2,2,3}
    },

    {
        "second attack", "��������", { SKILL_APP, SKILL_APP, SKILL_LIMIT(1,75,6), SKILL_LIMIT(3,80,6), SKILL_LIMIT(5,70,6), 
            SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,80,6), SKILL_LIMIT(10,80,6), SKILL_LIMIT(2,99,6), 
            SKILL_LIMIT(2,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,80,6), 
            SKILL_APP, SKILL_LIMIT(6,99,6), SKILL_LIMIT(2,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1 ,99,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_second_attack,  0,  0,
            "",   "!Second Attack!", ATTR_ALL, {1,1,1,1,1}

    },

    {
        "snare", "�����N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), 
            SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_snare,   0, 0,
            "",   "�A���A���쳴��������.", ATTR_ALL, {6,4,5,6,7}

    },

    {
        "sneak", "���", { SKILL_APP, SKILL_APP, SKILL_LIMIT(6,75,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(2,99,6), 
            SKILL_LIMIT(3,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(34,85,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(8,95,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_sneak,   0, 12,
            "",   "�A������C", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "staves", "���R�j���k��", { SKILL_LIMIT(13,85,6), SKILL_LIMIT(13,85,6), SKILL_LIMIT(26,70,6), SKILL_LIMIT(27,60,6), SKILL_LIMIT(30,50,6),
            SKILL_LIMIT(7,99,6), SKILL_LIMIT(7,99,6), SKILL_LIMIT(17,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(14,99,6), 
            SKILL_LIMIT(14,99,6), SKILL_LIMIT(14,99,6), SKILL_LIMIT(14,99,6), SKILL_LIMIT(14,99,6), SKILL_LIMIT(10,99,6), 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(22,99,6), SKILL_LIMIT(22 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_staves,   0, 0,
            "�k����q", "!Staves!",ATTR_ALL, {3,3,2,2,3}
    },

    {
        "steal", "���ѧޥ�", { SKILL_APP, SKILL_APP, SKILL_LIMIT(8,80,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(4,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(4,90,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_steal,   0, 24,
            "",   "!Steal!",ATTR_ALL, {7,4,6,5,7}

    },

    {
        "third attack", "�T������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_LIMIT(12,70,6), 
            SKILL_APP, SKILL_LIMIT(10,85,6), SKILL_APP, SKILL_LIMIT(10,75,6), SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(10,95,6), SKILL_LIMIT(5,85,6), SKILL_LIMIT(11,99,6), SKILL_LIMIT(6,99,6), SKILL_LIMIT(12,70,6), 
            SKILL_APP, SKILL_LIMIT(15,90,6), SKILL_LIMIT(10,95,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(10 ,99,6)},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_third_attack,  0,  0,
            "",   "!Third Attack!",ATTR_ALL, {1,1,2,2,2}

    },

    {
        "fourth attack", "�|������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36, 85,6), SKILL_APP, SKILL_LIMIT(24,95,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,80,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_fourth_attack,  0,  0,
            "",   "!Fourth Attack!",ATTR_ALL, {2,1,3,3,3}

    },

    {
        "fifth attack", "��������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP,  SKILL_LIMIT(34,95,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_fifth_attack,  0,  0,
            "",   "!Fifth Attack!",ATTR_ALL, {2,2,4,4,3}

    },

    {
        "untangle", "�Ѱ�����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22,90,6), 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_STANDING,
            &gsn_untangle,   0, 24,
            "",   "!Untangle!",ATTR_ALL, {6,4,5,7,8}

    },

    {
        "wands", "���R�]��", { SKILL_LIMIT(8,90,6), SKILL_LIMIT(8,90,6), SKILL_LIMIT(18,75,6), SKILL_LIMIT(12,70,6), SKILL_LIMIT(16,60,6), 
            SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(12,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(8,99,6), 
            SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(8,99,6), SKILL_LIMIT(5,99,6), 
            SKILL_LIMIT(22,99,6), SKILL_LIMIT(3,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(12 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_wands,   0, 0,
            "�]����q", "!Wands!",ATTR_ALL, {3,3,2,2,3}

    },

    {
        "scan",  "�j���ޥ�",  { SKILL_LIMIT(18,50,6), SKILL_LIMIT(20,60,6), SKILL_LIMIT(10,80,6), SKILL_LIMIT(13,75,6), SKILL_LIMIT(16,70,6), 
            SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(34,90,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(5,99,6), 
            SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), SKILL_LIMIT(10,90,6), 
            SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6) },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_scan,   0, 4,
            "", "!Scan!",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "scout",  "����ޥ�",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30, 99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            NULL,   0, 14,
            "", "!Scout!",ATTR_ALL, {4,4,5,6,5}

    },

    {
        "frenzy", "�g�ɧ���",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,90,6),
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(-1,90,6),
            SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6),
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(-1,90,6), SKILL_LIMIT(18,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_frenzy,   0, 15,
            "�g�ɧ���", "!Frenzy!",ATTR_ALL, {8,9,8,9,8}

    },

    {
        "bashhead", "�Y���r��",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,75,6), SKILL_LIMIT(6,80,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_LIMIT(2,90,6), SKILL_LIMIT(5,99,6), SKILL_LIMIT(5,90,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(5,90,6), SKILL_LIMIT(5,99,6) },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_bashhead,   0, 8,
            "�Y���r��", "!Bashhead!",ATTR_ALL, {4,5,7,7,5}

    },
    {
        "general purpose", "�s�����f�P",  { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(29,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_general_purpose,   0, 0,
            "�s�����f�P", "!General Purpose Ammo!",ATTR_ALL, {7,9,8,6,8}

    },

    {
        "high explosive", "������z", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_high_explosive,   0, 15,
            "������z", "!High Explosive Ammo!",ATTR_ALL, {7,5,4,6,7}

    },


    /*
     * Psionicist spell/skill definitions
     * Psi class conceived, created, and coded by Thelonius (EnvyMud)
     */

    {
        "adrenaline control", "�ƵǸ��N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_adrenaline_control, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_adrenaline,    35, 2,
            "",    "�A�餺���İʮ����F.",ATTR_ALL, {5,6,7,4,6}

    },
    {
        
        "combat mind", "�Ԥ����N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_combat_mind, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   35, 2,
            "",   "�A���԰���ı�����F.",ATTR_ALL, {4,5,5,3,5}

    },

    {
        "full heal", "�����v¡�N", { SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(-1,99,6), SKILL_APP, SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), 
            SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), SKILL_LIMIT(-1,99,6), SKILL_APP },
            spell_full_heal, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            NULL,   200, 7,
            "",   "!Full Heal!",ATTR_ALL, {5,3,4,2,4}

    },

    {
        "gfull heal", "�s�駹���v¡�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(29,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_full_heal, TAR_IGNORE,  POS_STANDING,
            NULL,   500, 0,
            "",   "!Full Heal!",ATTR_ALL, {7,5,6,6,7}

    },
    {
        "barrier", "�@������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_barrier, TAR_CHAR_SELF,  POS_FIGHTING,
            NULL,   40,     2,
            "",   "�A���W���@�����ɺC�C�����F.",ATTR_ALL, {6,5,2,3,7}

    },

    {
        "vampiric bite", "�l�尭���k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_vampiric_bite, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_vampiric_bite, 0,     0,
            "�l�尭���k", "�A���W���l�尭�L�O�����F.",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "seal magic", "�]�k�ʦL", { SKILL_LIMIT(28,85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_seal_magic, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_seal_magic, 30,     2,
            "", "�Aı�o���W���ʦL�Ѷ}�F",ATTR_ALL, {3,2,1,2,3}

    },

    {
        "artifact", "�]�k���~�ϥ�", { SKILL_LIMIT(15,90,6), SKILL_LIMIT(15,90,6), SKILL_LIMIT(22,80,6), SKILL_LIMIT(20,70,6), SKILL_LIMIT(25,50,6), 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,99,6), SKILL_LIMIT(16,90,6),
            SKILL_LIMIT(16,95,6), SKILL_LIMIT(16,85,6), SKILL_LIMIT(12,85,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(15,90,6), 
            SKILL_LIMIT(10,99,6), SKILL_LIMIT(10,90,6), SKILL_LIMIT(12,90,6), SKILL_LIMIT(10,85,6), SKILL_LIMIT(18 ,85,6)},
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_artifact, 0,     0,
            "", "",ATTR_ALL, {10,10,7,7,10}

    },

    {
        "change fighting", "���ܧ����ؼ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(27,99,6), 
            SKILL_LIMIT(30,95,6), SKILL_LIMIT(20,90,6), SKILL_LIMIT(18,95,6), SKILL_LIMIT(25,95,6), SKILL_LIMIT(30,70,6), 
            SKILL_APP, SKILL_LIMIT(30,90,6), SKILL_LIMIT(25,90,6), SKILL_LIMIT(25,80,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_change_fighting, 0,     1,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "nightstalk", "�t�]����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_nightstalk, 0,     9,
            "", "�A�����γv���q��v���B�{",ATTR_ALL, {8,7,9,9,9}

    },

    {
        "reflexion", "�ϦV���k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_reflexion, 0,     0,
            "�ϦV���k", "",ATTR_ALL, {8,5,6,9,8}

    },

    {
        "fist fight", "�{�����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(10,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_fist_fight, 0,     0,
            "", "",ATTR_ALL, {3,5,8,5,18}

    },

    {
        "regeneracy", "�A�ͯ�O", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_regeneracy, 0,     0,
            "", "�A���A�ͯ�O��_���`.",ATTR_ALL, {6,6,7,7,5}

    },

    {
        "weapon master", "�Z���j�v", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_weapon_master, 0,     0,
            "", "",ATTR_ALL, {6,7,6,6,7}

    },

    {
        "tactic", "�ԳN", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(8,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(28,99,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_tactics, 0,     0,
            "��������", "",ATTR_ALL, {5,7,7,7,5}

    },

    {
        "track", "�l�ܧޥ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), 
            SKILL_LIMIT(26,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(28,95,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_track, 0,     6,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "burial ceremony", "�W�����", { SKILL_APP, SKILL_LIMIT(15,80,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_LIMIT(10,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_burial_ceremony, 0,     0,
            "", "", ATTR_ALL, {3,2,3,1,4}
    },

    {
        "combat howling", "�԰��g�z", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30 ,99,6)},
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_combat_howling, 0,     2,
            "�԰��g�z", "!�԰��g�z!",ATTR_ALL, {7,7,8,8,6}

    },


    {
        "blood thirsty", "�ݦ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30 ,99,6)},
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_blood_thirsty, 0,     0,
            "", "�Ů𤤪���{�������A���ݦ����A�A���U��Ť��A���o�o�g�C",ATTR_ALL, {8,8,10,10,7}

    },


    {
        "invoke", "���F����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6),
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_invoke, 0,     8,
            "", "�@�}�����P��W���Y",ATTR_ALL, {7,7,6,6,5}

    },


    {
        "full refresh", "��O������_�N", { SKILL_APP, SKILL_LIMIT(18,80,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(15,80,6), SKILL_LIMIT(15,80,6), SKILL_LIMIT(12,99,6), SKILL_LIMIT(12,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(15,90,6), SKILL_LIMIT(15,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_full_refresh, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 70,     7,
            "", "",ATTR_ALL, {5,4,4,3,5}

    },

    {
        "soulsteal", "�]�O���P", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_LIMIT(22,90,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_soulsteal, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_soulsteal, 1,     4,
            "�]�O���P", "!�]�O���P!", ATTR_ALL,{8,7,6,7,8}
    },

    {
        "lifesteal", "�ͩR���P", { SKILL_APP, SKILL_LIMIT(20,70,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(15,90,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lifesteal, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_lifesteal, 50,     4,
            "�ͩR���P", "!�ͩR���P!",ATTR_ALL, {4,4,2,3,4}

    },

    {
        "darkraid", "�]ŧ", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(32,99,6),
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_dark_raid, 0,     4,
            "", "�A�������O��������",ATTR_ALL, {9,8,10,10,9}

    },

    {
        "flaming armor", "�����Z��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flaming_armor, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_flaming_armor, 70,     6,
            "�@������", "��¶�b�A���䪺�]�K��������",ATTR_ALL, {10,10,7,8,10}

    },

    {
        "mana shield", "�]�O�̻�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mana_shield, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_mana_shield, 350,     4,
            "", "��¶�b�A���䪺�]�k���ɺ����H��",ATTR_ALL, {5,6,2,3,6}

    },

    {
        "armageddon", "����T�G", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(35,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_armageddon, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_armageddon, 150,     0,
            "����T�G", "",ATTR_ALL, {10,10,7,8,10}

    },

    {
        "block", "�޵P���", { SKILL_APP, SKILL_LIMIT(25,80,6), SKILL_APP, SKILL_LIMIT(8,75,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,90,6), SKILL_LIMIT(15,85,6), SKILL_APP,
            SKILL_APP, SKILL_LIMIT(3,99,6), SKILL_LIMIT(3,95,6), SKILL_APP, SKILL_LIMIT(20,90,6),
            SKILL_APP, SKILL_LIMIT(10,90,6), SKILL_APP, SKILL_LIMIT(20,95,6), SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_block, 0,     0,
            "", "",ATTR_ALL, {1,3,3,2,2}

    },

    //This skill is for sor
    {
        "glacier strike", "�B�e����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(12,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_glacier_strike, TAR_IGNORE, POS_FIGHTING,
            &gsn_glacier_strike, 120,     6,
            "�B�e����", "�A�Pı��@�ɺC�C�^�x.",ATTR_ALL, {5,4,2,3,5}

    },


    {
        "clay golem", "�l��g���H", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_clay_golem, TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL, 330,     8,
            "", "",ATTR_ALL, {8,4,2,3,8}

    },

    {
        "double heal", "�����v���N", { SKILL_APP, SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_LIMIT(18,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(27,90,6), SKILL_LIMIT(31,85,6), SKILL_APP, SKILL_APP },
            spell_double_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 30,     7,
            "�����v���N", "",ATTR_ALL, {4,3,3,2,4}

    },

    {
        "gdouble heal", "�s�������v���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_double_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 100,     7,
            "", "",ATTR_ALL, {4,3,3,3,4}

    },

    {
        "remove nodrop", "�A�G���~�}�ѳN", { SKILL_APP, SKILL_LIMIT(20,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_LIMIT(12,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(15,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_remove_nodrop, TAR_OBJ_INV, POS_STANDING,
            NULL, 50,     2,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },


    {
        "endless sight", "�L�ɵ���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_endless_sight, TAR_IGNORE, POS_STANDING,
            NULL, 50,     8,
            "", "",ATTR_ALL, {5,3,2,3,5}

    },


    {
        "divine light", "�����[�@", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_divine_light, TAR_IGNORE, POS_FIGHTING,
            &gsn_divine_light, 175,     8,
            "", "�A���W���զ�t�������F.",ATTR_ALL, {8,6,6,5,8}

    },

    {
        "gfull refresh", "�s����O���_�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,95,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_full_refresh, TAR_IGNORE, POS_FIGHTING,
            NULL, 250,     0,
            "", "",ATTR_ALL, {5,4,5,4,6}

    },

    {
        "gcure blindness", "�s�骼�تv���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_group_cure_blindness, TAR_IGNORE, POS_FIGHTING,
            NULL, 100,     10,
            "", "",ATTR_ALL, {4,5,4,3,4}

    },

    {
        "vivification", "�d¡�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(26,99,6), SKILL_LIMIT(35,80,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_vivification, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,  35, 0,
            "",   "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },


    {
        "water spirit", "�l������F", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_water_spirit, TAR_IGNORE, POS_STANDING,
            NULL, 400,     8,
            "", "",ATTR_ALL, {5,3,2,3,5}

    },

    {
        "fire elemental", "�l�������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fire_elemental, TAR_IGNORE, POS_STANDING,
            NULL, 500,     8,
            "", "",ATTR_ALL, {5,4,2,3,5}

    },

    {
        "archery", "�b�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(15,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_shot_arrow, 0,     0,
            "", "",ATTR_ALL, {7,9,7,6,7}

    },

    {
        "moon night", "��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_moon_night, 0,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "concentration", "�M�`", { SKILL_APP, SKILL_LIMIT(27,85,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(31,80,6), SKILL_APP, SKILL_LIMIT(12,99,6), SKILL_LIMIT(15,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(24, 90,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_concentration, 0,     0,
            "", "",ATTR_ALL, {5,4,4,3,5}

    },

    {
        "circular chop", "�j�۱�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(37,99,6), SKILL_LIMIT(31 ,90,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_circular, 0,     10,
            "�j�۱�", "",ATTR_ALL, {6,6,5,15,6}

    },

    {
        "flaming slash", "�P�K��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flaming_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_flaming_slash, 0,     9,
            "�P�K��", "",ATTR_ALL, {11,10,10,10,12}

    },

    {
        "earth slash", "�j�a��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earth_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_earth, 0,     12,
            "�j�a��", "�A���|�ϤS�ଡ�ʦۦp�F.",ATTR_ALL, {10,10,9,10,11}

    },

    {
        "storm slash", "�g����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_storm_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_storm_slash, 0,     12,
            "�g����", "",ATTR_ALL, {11,11,10,10,11}

    },

    {
        "sprinkle slash", "�i���C", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sprinkle_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_sprinkle_slash, 500,     12,
            "�i���C", "�A����ʤ��A����i���C�𪺧���",ATTR_ALL, {7,6,5,6,7}

    },

    {
        "adore", "�^�P��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(29,99,6), SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_adore, 0,     10,
            "�^�P��", "�A�E���������C�C�^�_���R",ATTR_ALL, {7,9,5,9,6}

    },

    {
        "meteoric arrow", "�y�P�b", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_meteoric_arrow, 0,     20,
            // "", "",ATTR_ALL, {12,14,12,8,11}
            "", "",ATTR_ALL, {11, 8,12, 9,11}

    },

    {
        "protect target", "�O�@�ؼ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(23,75,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(17,99,6), SKILL_LIMIT(15,90,6), SKILL_LIMIT(22,85,6), SKILL_LIMIT(32,90,6), 
            SKILL_APP, SKILL_LIMIT(17,85,6), SKILL_APP, SKILL_LIMIT(28,85,6), SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_protect, 0,     0,
            "", "",ATTR_ALL, {7,7,8,8,6}
    },

    {
        "break shield", "�}�޳N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_break_shield, 0,     0,
            "", "",ATTR_ALL, {6,5,7,6,7}

    },

    {
        "reposte", "���ؤ���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(33,99,6), SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_reposte, 0,     0,
            "���ؤ���", "",ATTR_ALL, {5,6,5,8,6}

    },

    {
        "ride", "�M�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(1,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(10,95,6), SKILL_LIMIT(32,80,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_ride, 0,     0,
            "", "",ATTR_ALL, {4,5,6,5,6}

    },

    {
        "charge", "�Ĩ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_charge, 0,     20,
            "�Ĩ�", "",ATTR_ALL,{5,6,8,7,6}

    },

    {
        "twin swords", "������C", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(25,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_twin, 0,     16,
            "������C", "",ATTR_ALL, {10,8,9,9,10}

    },

    {
        "lead", "����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            &gsn_lead, 0,     0,
            "", "",ATTR_ALL, {6,8,5,5,7}

    },

    {
        "battle array", "�}��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            NULL, 0,     20,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "repair armor", "�@��״_�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_repair_armor, TAR_OBJ_INV,  POS_STANDING,
            NULL,   200, 12,
            "",   "!Repair Armor!",ATTR_ALL, {7,8,8,8,9}

    },

    {
        "tardy", "��w�N", { SKILL_APP, SKILL_LIMIT(28,70,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(10,99,6), SKILL_LIMIT(20,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_tardy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_tardy, 40,     2,
            "", "�A�Sı�o���ְ_�ӡI",ATTR_ALL, {4,3,4,2,4}

    },
    {
        "autoparry", "���O���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_autoparry, 0,     0,
            "", "",ATTR_ALL, {5,6,7,7,5}

    },
    {
        "autododge", "���O�{��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,80,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_autododge, 0,     0,
            "", "",ATTR_ALL, {6,5,6,6,7}

    },
    {
        "wind screaming", "�����㸹", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wind_screaming, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_wind_screaming, 100,     4,
            "�����㸹", "!wind screaming!",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "angry fire storm", "�������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_angry_fire_storm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            NULL, 200,     8,
            "�������", "!angry fire storm!",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "sun flash", "�v�����z�N", { SKILL_LIMIT(35,60,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(25,99,6), SKILL_LIMIT(27,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sun_flash, TAR_IGNORE, POS_FIGHTING,
            &gsn_sun_flash, 75,     7,
            "�v�����z", "�@�ɤS�C�C�A���_�ӡI",ATTR_ALL, {4,5,3,4,5}

    },
    {
        "deadly strike", "�P�R����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(32,99,6), 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(32,80,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_deadly_strike, 0,     0,
            "", "",ATTR_ALL, {6,5,7,7,6}

    },

    {
        "calm", "���R�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(32,99,6), SKILL_LIMIT(34,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_calm, TAR_IGNORE, POS_FIGHTING,
            &gsn_calm, 80,     6,
            "���R�N", "",ATTR_ALL, {3,3,4,2,3}

    },

    {
        "virtual swords", "�ۼC�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_virtual_swords, TAR_IGNORE, POS_FIGHTING,
            &gsn_virtual_swords, 90,     8,
            "�ۼC�N", "",ATTR_ALL, {8,7,6,6,7}

    },
    {
        "powerblow", "�����N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22 ,99,6)},
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_powerblow, 0,     0,
            "�����N", "",ATTR_ALL, {10,10,11,11,8}

    },
    {
        "mana recharge", "�]�O�^�_", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mana_recharge, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 1,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "double recharge", "�����]�O�^�_", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_double_recharge, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 1,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "full recharge", "�]�O���_�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_full_recharge, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 1,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "meditate", "�߷Q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(24,99,6), SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_LIMIT(34,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,80,6), 
            SKILL_LIMIT(26,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_meditate, 0,     290,
            "", "�A���믫�C�C�P���魫�s���X�C",ATTR_ALL, {6,6,4,5,6}

    },

    {
        "fix item", "�˳ƭײz", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_OBJ_INV, POS_STANDING,
            &gsn_fix_item, 0,     24,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },
    {
        "escape", "�k�R�ޥ�", { SKILL_APP, SKILL_LIMIT(35,70,6), SKILL_LIMIT(30,85,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6),
            SKILL_LIMIT(20,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_escape, 0,     4,
            "", "",ATTR_ALL, {12, 10, 11, 11, 12}//{7,5,6,7,8}  //  // {STR,DEX,INT,WIS,CON}
    },
    {
        "mystic mirror", "�]�k����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(37,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(39,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mystic_mirror, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_mystic_mirror, 350,     6,
            "", "�A�P���]�k��������F�C",ATTR_ALL, {6,5,2,3,6}
    },
    {
        "mystic armor", "�]�O�@��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mystic_armor, TAR_CHAR_SELF, POS_FIGHTING,
            &gsn_mystic_armor, 800,     6,
            "", "�A�P���]�O�@���C�C�����C",ATTR_ALL, {6,6,2,3,6}
    },
    {
        "lifeshare", "�P�ͦ@��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lifeshare, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_lifeshare, 0,     0,
            "�P�ͦ@��", "",ATTR_ALL, {5,5,9,4,5}
    },
    {
        "fire wall", "����N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fire_wall, TAR_IGNORE, POS_STANDING,
            NULL, 60,     16,
            "����N", "����w�w�a�����F�C",ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "armor mastery", "�˥ұM�a", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(28,99,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_armor_mastery, 0,     0,
            "", "",ATTR_ALL, {10,12,12,12,10}
    },
    {
        "magic arrow", "�]�k�b", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(33,99,6), SKILL_APP, SKILL_APP },
            spell_magic_arrow, TAR_IGNORE, POS_STANDING,
            &gsn_magic_arrow, 0,     4,
            // "", "",ATTR_ALL, {12,11,10,10,12}
            "", "",ATTR_ALL, {12,9, 8,10,12}

    },
    {
        "prot. from fire", "���K����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pfire,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "�A�P�򪺤��K���������F�C", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "prot. from cold", "�H�B����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pcold,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "�A�P�򪺴H�B���������F�C", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "prot. from wind", "�e������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pwind,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "�A�P�򪺯e�����������F�C", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "prot. from earth", "�j�a����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_pearth,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "�A�P�򪺤j�a���������F�C", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "day of protection", "���t�[�@", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(30,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_day_of_prot,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   150, 12,
            "",   "�A�P�򪺯��t���������F�C", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "day of power", "�t�Ԥ���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_day_of_power,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   260, 12,
            "",   "�A�ߤ����t�Ա۫߮��u�F�C", ATTR_ALL, {4,4,0,3,5}

    },
    {
        "day of brightness", "��������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(22,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_day_of_brightness,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   75, 12,
            "",   "�A���W���������֮����F�C", ATTR_ALL, {-1,-1,5,4,-1}

    },
    {
        "resurrection", "�_���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_resurrection,  TAR_OBJ, POS_STANDING,
            &gsn_resurrection,   250, 12,
            "",   "���`�����v�v�����A���h�C", ATTR_ALL, {-1,-1,7,5,-1}

    },
    {
        "mental empower", "���F�W�T", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(33,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mental_empower, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_mental_empower,   40, 4,
            "���F�W�T",  "�A�����F�v����_���`!", ATTR_ALL, {5,5,4,6,5}

    },
    {
        "empslash", "�Q�ӼC", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_empslash,   0, 12,
            "�Q�ӼC",  "", ATTR_ALL, {8,9,9,10,9}

    },


    {
        "wizard rage", "�ԧŤѫ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wizard_rage, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_wizard_rage, 0,     0,
            "�ԧŤѫ�", "�A�P��������O�q�S��_�F�C", ATTR_ALL, { 8,-1,5,6,-1}
    },
    {
        "hasten", "��𬡨��k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_hasten,   0, 0,
            "",   "�A���������S�^�쥿�`���B�@�t�v�C", ATTR_ALL, {7,8,15,7,15}

    },
    {
        "morphing", "���e�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(34,99,6), 
            SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_STANDING,
            &gsn_morphing,   12, 0,
            "",   "�A���ܸˮĪG�����F�C", ATTR_ALL, {2,1,1,3,2}

    },
    {
        "crushing hand", "���H�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(34,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_crushing_hand,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_crushing_hand,   1000, 0,
            "",   "", ATTR_ALL, {8,9,7,10,9}

    },
    {
        "song of clenase", "�b�Ƥ��q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(35,99,6), SKILL_LIMIT(35,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_song_of_cleanse,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            &gsn_song_of_clenase,   200, 0,
            "�b�Ƥ��q",   "�b�Ƥ��q", ATTR_ALL, {4,4,4,4,4}

    },
    // mark and recall is for Sorcerer
    {
        "mark", "�аO�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36, 99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mark,  TAR_IGNORE, POS_STANDING,
            &gsn_mark,   150, 4,
            "�b�Ƥ��q",   "�b�Ƥ��q", ATTR_ALL, {7,5,6,7,7}

    },
    {
        "recall", "��^�N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_recall,  TAR_IGNORE, POS_FIGHTING,
            &gsn_recall,   300, 4,
            "��^�N",   "�b�Ƥ��q", ATTR_ALL, {7,5,6,7,7}

    },
    // this spell is for wizard
    {
        "evacuate", "�����N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37, 99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_evacuate,  TAR_IGNORE, POS_FIGHTING,
            &gsn_evacuate,   150, 8,
            "�b�Ƥ��q",   "�b�Ƥ��q", ATTR_ALL, {5,5,2,3,5}

    },

    // this skill is for war-kni
    {
        "defensive", "�@���\\��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE, POS_FIGHTING,
            &gsn_defensive,   150, 12,
            "�@���\\��",   "�A���@���\\������F�C", ATTR_ALL, {6,7,8,6,5}

    },

    { //passive skill for wiz/alc/sor
        "spell fury", "�]�k�g��", { SKILL_LIMIT( 34, 60,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(26, 99,6), SKILL_LIMIT( 30, 75,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(28, 85,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE, POS_STANDING,
            &gsn_spell_fury,   150, 12,
            "�]�k�g��",   "�]�k�g��", ATTR_ALL, {9,10,8,9,10}

    },

    {
        "continous heal", "����v���N", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP,SKILL_LIMIT(32,99,6), SKILL_LIMIT(33,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36, 70,6),
            SKILL_APP, SKILL_LIMIT(37,70,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_cont_heal,  TAR_CHAR_SELF, POS_STANDING,
            &gsn_cont_heal,   60, 6,
            "����v��",   "�A�餺���x�y�����F�C", ATTR_ALL, {6,5,6,4,6}

    },
    {
        "double refresh", "������O�^�_�N", { SKILL_APP, SKILL_LIMIT(35,80,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(32,90,6), SKILL_LIMIT(32,90,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(-1,90,6), SKILL_APP, SKILL_APP, SKILL_APP },
            spell_double_refresh, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL, 30,     7,
            "������O�^��", "double refresh",ATTR_ALL, {4,3,3,2,4}

    },
    {
        "blind fight", "�߲��y���k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(35,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_blind_fight,   0, 0,
            "",   "", ATTR_ALL, {1,2,3,3,5}

    },
    {
        "throwing", "���Y���~", { SKILL_LIMIT(20,70,6), SKILL_LIMIT(20,70,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,75,6), SKILL_LIMIT(20,75,6), 
            SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,95,6), 
            SKILL_LIMIT(20,99,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,95,6), SKILL_LIMIT(20,95,6), SKILL_LIMIT(20,85,6), 
            SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,85,6), SKILL_LIMIT(20,95,6), SKILL_LIMIT(20,90,6), SKILL_LIMIT(20,85,6) },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_throwing, 0,     9,
            "���Y���~", "���Y���~",ATTR_ALL, {2,1,3,3,2}

    },
    {
        "evade", "�j�קޥ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_evade, 0,     6,
            "�j�קޥ�", "�A���j�ׯ�O�U���F.",ATTR_ALL, {6,5,7,7,7}

    },
    {
        "meteor shower", "�f�۫B", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(35,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_meteor_shower,  TAR_IGNORE, POS_FIGHTING,
            &gsn_meteor_shower,   0, 8,
            "�f�۫B",   "meteor shower", ATTR_ALL, {11,10,7,7,11}

    },
    {
        "ice comet", "�B���k�P", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_ice_comet,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_ice_comet,   100, 8,
            "�B���k�P",   "ice comet", ATTR_ALL, {9,9,6,6,9}

    },
    {
        "holy crown", "�t���a��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
           SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP,
 	         SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
 	         SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
 	         spell_holy_crown,  TAR_CHAR_DEFENSIVE, POS_STANDING,
 	         NULL,   280, 12,
 	         "�j�j���F��w�w�o��......",   "�A�����¶���F������F", ATTR_ALL, {8,8,5,4,8}
                                                                                
    },

    {
        "powerup", "�o�l", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_powerup,   0, 0,
            "",   "�A�餺���l�O�����F�C", ATTR_ALL, {6,7,8,7,12}

    },

    {
        "bandage", "�]�϶˶�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_bandage,   0, 12,
            "",   "�A���������S�^�쥿�`���B�@�t�v�C", ATTR_ALL, {3,2,2,2,4}
    },
    {
        "trueshot", "��Ǯg��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_trueshot,   0, 8,
            "",   "�A�����������v����_���`�C", ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "powerbolt", "�]�u�g��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_CHAR_SELF,  POS_FIGHTING,
            &gsn_powerbolt,   0, 8,
            "",   "�A���[�b�b�W���O�q�����F�C", ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "cure disease", "�v���e�f�N", { SKILL_APP, SKILL_LIMIT(25,85,6), SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(23,99,6), SKILL_LIMIT(25,99,6), SKILL_APP,
            SKILL_APP, SKILL_LIMIT(-1,85,6), SKILL_APP, SKILL_LIMIT(-1,85,6), SKILL_LIMIT(20,90,6),
            SKILL_LIMIT(-1,85,6), SKILL_LIMIT(-1,85,6), SKILL_LIMIT(-1,85,6), SKILL_LIMIT(-1,85,6), SKILL_APP },
            spell_cure_disease,  TAR_CHAR_DEFENSIVE, POS_FIGHTING,
            NULL,  30, 6,
            "�e�f�N", "�Aı�o�ΪA�h�F.", ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    {
        "prot. from lightning", "�{�q����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20,99,6), SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_plightning,  TAR_CHAR_DEFENSIVE, POS_STANDING,
            NULL,   30, 0,
            "",   "�A�P�򪺰{�q���������F�C", ATTR_ALL, {-1,-1,5,4,-1}
    },
    // this skill is for sor
    {
        "sand tide", "�F��½��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sand_tide, TAR_IGNORE, POS_FIGHTING,
            &gsn_sand_tide, 120,     6,
            "�F��½��", "�A�Pı�ש�q�F��̱ò�X��.",ATTR_ALL, {-1,4,2,3,-1}
    },
    // this skill is for sor
    {
        "banshee wail", "�৯�F�z", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_banshee_wail, TAR_IGNORE, POS_FIGHTING,
            &gsn_banshee_wail, 120,     6,
            "�৯�F�z", "�A�Pı����k�������z�����F.",ATTR_ALL, {-1,4,2,3,-1}
    },
    // this skill is for sor
    {
        "succubus kiss", "�y�]���k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_succubus_kiss, TAR_IGNORE, POS_FIGHTING,
            &gsn_succubus_kiss, 120,     6,
            "�y�]���k", "�A�Pı�y�]�a���A���E����������.",ATTR_ALL, {-1,4,2,3,-1}
    },
    // this skill is for sor
    {
        "will fortress", "�N�ӭn��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_will_fortress, TAR_CHAR_DEFENSIVE,  POS_STANDING,
            &gsn_will_fortress, 150,     4,
            "�N�ӭn��", "�A�Pı��j���N�ӪZ�˺C�C�Y��.",ATTR_ALL, {-1,7,2,4,-1}
    },
    // this skill is for sor
    {
        "mind blast", "���F�z�_", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(10,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_mind_blast, TAR_IGNORE, POS_FIGHTING,
            &gsn_mind_blast, 30,     6,
            "���F�z�_", "�A���F�C�C�q�ҽk���^��.",ATTR_ALL, {-1,3,2,2,-1}
    },
    // this skill is for sor
    {
        "disenchant", "�۶H����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_disenchant, TAR_IGNORE, POS_FIGHTING,
            NULL, 100,     8,
            "�۶H����", "",ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    // this skill is for sor
    {
        "ghosts celebration", "�ʰ��]��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_LIMIT(32,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_ghosts_celebration, TAR_IGNORE, POS_FIGHTING,
            NULL, 80,     8,
            "�ʰ��]��", "�A�Pı�|�P���y������n�ש����.",ATTR_ALL, {-1,-1,-1,-1,-1}
    },
    
    {
        "chain lightning", "�s��{�q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_chain_lightning, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_chain_lightning,   200, 10,
            "�s��{�q",  "!chain lightning!", ATTR_ALL, {11,11,7,7,11}

    },
    {
        "icecore slash", "�B���", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_icecore_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_icecore_slash, 0,     12,
            "�B���", "",ATTR_ALL, {11,11,10,10,11}

    },
    {
        "fulgurant slash", "�~�p��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_fulgurant_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_fulgurant_slash, 0,     12,
            "�~�p��", "",ATTR_ALL, {11,11,10,10,11}

    },
    {
        "body building", "�j������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_body_building, 0,  0,
            "",   "�A���骺�j�Ʈ����F.", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "mana condense", "�]�O���E", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_mana_condense, 0,  0,
            "",   "���E�b�A���䪺�]�O���h.", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "poison explosion", "�r���z", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_poison_explosion, TAR_IGNORE,  POS_FIGHTING,
            &gsn_poison_explosion, 75,  4,
            "�r���z",   "poison explosion", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "finger of death", "���`����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_finger_of_death, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_finger_of_death, 120,  4,
            "���`����",   "finger of death", ATTR_ALL, {4,5,6,6,4}

    },    
    {
        "apocalypse", "�ҥܿ�����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_apocalypse, TAR_IGNORE,  POS_FIGHTING,
            &gsn_apocalypse, 350,  6,
            "�ҥܿ�����",   "Apocalypse", ATTR_ALL, {4,5,6,6,4}

    },
    {
        "dark containment", "��G����", { SKILL_APP, SKILL_LIMIT(30,60,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(18,99,6), SKILL_LIMIT(28,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dark_containment, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_dark_containment, 80,  4,
            "��G����",   "��¶�A���骺����Y�������C", ATTR_ALL, {4,1,3,2,3}

    },
    {
        "phlogistic verse", "���K�ֽg", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_phlogistic_verse, TAR_IGNORE,  POS_FIGHTING,
            &gsn_phlogistic_verse, 1000,  0,
            "���K�ֽg",   "���K����������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {15,15,12,12,15}
    },
    {
        "wail of tempest", "�^���s��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_wail_of_tempest, TAR_IGNORE,  POS_FIGHTING,
            &gsn_wail_of_tempest, 1000,  0,
            "�^���s��",   "�g������������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "fulgurant dance", "�ƹp¶�R", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_fulgurant_dance, TAR_IGNORE,  POS_FIGHTING,
            &gsn_fulgurant_dance, 1000,  0,
            "�ƹp¶�R",   "�{�q����������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "agglomerate descant", "�~�����q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_agglomerate_descant, TAR_IGNORE,  POS_FIGHTING,
            &gsn_agglomerate_descant, 1000,  0,
            "�~�����q",   "�H�B����������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "earth bind" , "�j�a����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_earth_bind, TAR_IGNORE,  POS_FIGHTING,
            &gsn_earth_bind, 1000,  0,
            "�j�a����",   "�j�a����������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "holy judgement", "�t���f�P", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_holy_judgement, TAR_IGNORE,  POS_FIGHTING,
            &gsn_holy_judgement, 1000,  0,
            "�t���f�P",   "���t����������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "hell southerncross", "�a���n�Q�r�P", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_hell_southencross, TAR_IGNORE,  POS_FIGHTING,
            &gsn_hell_southencross, 1400,  0,
            "�a���n�Q�r�P",   "�����������F�A�Aı�o����S�C�C��_���`�C", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "salvation", "�ϥ@���q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_salvation, TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
            &gsn_salvation, 350,  8,
            "�ϥ@���q", "salvation", ATTR_ALL, {5,5,5,4,5}

    },
    {
        "song of firestorm", "�a�����q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_song_of_firestorm, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_song_of_firestorm, 50,  4,
            "�a�����q",   "song of firestorm", ATTR_ALL, {4,5,6,6,4}

    }, 
    {
        "lore of frost", "��������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_lore_of_frost, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_lore_of_frost, 50,  4,
            "��������",   "lore of frost", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "soul of magma", "�J�����z", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_soul_of_magma, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_soul_of_magma, 50,  4,
            "�J�����z",   "soul of magma", ATTR_ALL, {4,5,6,6,4}
    },
    {
        "fang of slime", "�d�뤧��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP},
            spell_fang_of_slime, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
            &gsn_fang_of_slime, 50,  4,
            "�d�뤧��",   "fang of slime", ATTR_ALL, {4,5,6,6,4}
    },
    
    //Sor New Skill
    {
        "flame elemental", "�l�������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flame_elemental, TAR_ARG, POS_STANDING,
            &gsn_flame_elemental, 300,     8,
            "�l�������", "�A���A������������[��.",ATTR_ALL, {10,6,7,7,10}

    },

    {
        "water elemental", "�l�������", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_water_elemental, TAR_ARG, POS_STANDING,
            &gsn_water_elemental, 250,     8,
            "�l�������", "�A���A������������[��.",ATTR_ALL, {10,7,8,8,10}

    },
  
    {
        "earth elemental", "�l��g����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earth_elemental, TAR_ARG, POS_STANDING,
            &gsn_earth_elemental, 300,     8,
            "�l��g����", "�A���A����g�������[��.",ATTR_ALL, {10,6,6,6,10}

    },

    {
        "wind elemental", "�l�ꭷ����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wind_elemental, TAR_ARG, POS_STANDING,
            &gsn_wind_elemental, 200,     8,
            "�l�ꭷ����", "�A���A���쭷�������[��.",ATTR_ALL, {10,5,7,7,10}

    },

    {
        "lightning elemental", "�l��p����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(37,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_elemental, TAR_ARG, POS_STANDING,
            &gsn_lightning_elemental, 250,     8,
            "�l��p����", "�A���A����p�������[��.",ATTR_ALL, {10,6,7,7,10}

    },
		
    
    { // natural ability for all classes and races
        "inborn ability", "�ѽ�", { 
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6),
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6),
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6),
            SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6), SKILL_LIMIT(1,99,6) },
            spell_null,  TAR_IGNORE, POS_STANDING,
            &gsn_inborn_ability,   150, 12,
            "�ѽ�",   "�A����O��_���`.", ATTR_ALL, { 1, 1, 1, 1, 1}

    },

    {
        "werewolf", "�T�H", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_STANDING,
            &gsn_werewolf, 0,     0,
            "", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "elemental slash", "�����]�k��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_elemental_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_elemental_slash, 0,     12,
            "�����]�k��", "",ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "sun slash", "�P����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(38,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_sun_slash, TAR_IGNORE, POS_FIGHTING,
            &gsn_sun_slash, 0,     12,
            "�P����", "�@�ɤS�C�C�A���_�ӡI",ATTR_ALL, {11,11,10,10,12}

    },

    {
        "instant cast", "�����I�k", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_LIMIT(30,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_instant_cast,   0, 10,
            "�����I�k",  "", ATTR_ALL, {5,5,2,2,5}

    },

    {
        "stone form", "�ۤ�", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_stone_form,   0, 10,
            "�ۤ�",  "���骺�ۤƪ��A���������F.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "lance mastery", "�j�N", { 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(25, 99, 6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_lance_mastery,   0, 10,
            "�j�N",  "", ATTR_ALL, {4,5,8,8,5}
    },

    {
        "venomous strike", "�rŧ", { 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,70,6), SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20, 99, 6),
            SKILL_LIMIT(30, 90, 6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_venomous_strike,   0, 10,
            "�rŧ",  "", ATTR_ALL, {5,4,6,7,7}
    },

    {
        "satiety", "����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_satiety,   0,  0,
            "",   "�A�Pı�S���򦳹����P�F.", ATTR_ALL, {-1,-1,-1,-1,-1}

    },

    {
        "tricky pacing", "���ԨB��", { 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(20, 99, 6),
            SKILL_LIMIT(20, 95, 6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_LIMIT(20, 90, 6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_tricky_pacing,   0, 10,
            "���ԨB��",  "", ATTR_ALL, {4,3,6,6,6}
    },
	
    {
        "hit attack", "�Ť����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_hit_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "slice attack", "�U�Q����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_slice_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "stab attack", "�²r�@�W", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_stab_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "slash attack", "���R�A��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_slash_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "whip attack", "�`���@�@", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_whip_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "claw attack", "�ΤO�@��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_claw_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "blast attack", "�z�o�Q��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_blast_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "pound attack", "�U�v�Oݴ", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_pound_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "crush attack", "�����V��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_crush_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "grep attack", "�R�Q�@��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_grep_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "bite attack", "�i�f�r�r", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_bite_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "pierce attack", "�y�U����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_pierce_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "suction attack", "�r�P�l�m", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_suction_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "chop attack", "���s��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_chop_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "shot attack", "�g���ˮ`", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null,  TAR_IGNORE,  POS_FIGHTING,
            &gsn_shot_attack,   0,  0,
            "",   "", ATTR_ALL, {-1,-1,-1,-1,-1}

    },
	
    {
        "magic enchantment", "�k�N���]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_magic_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_magic_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "wind enchantment", "���ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_wind_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_wind_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "flame enchantment", "���ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_flame_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_flame_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "water enchantment", "���ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_water_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_water_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "earth enchantment", "�a�ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_earth_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_earth_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "lightning enchantment", "�p�ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_lightning_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_lightning_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {2,2,1,2,3}

    },
	
    {
        "saint enchantment", "�t�ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_saint_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_saint_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {3,3,2,1,3}

    },
	
    {
        "dark enchantment", "�t�ݩʪ��]", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_LIMIT(36,99,6), SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_dark_enchantment, TAR_IGNORE,  POS_STANDING,
            &gsn_dark_enchantment,   250, 0,
            "",   "�A���W�����]�����F.", ATTR_ALL, {3,3,2,1,3}

    },

    {  // �N dart ���}�s�@�����P skill 2022/04/29
        "dart mastery", "�t���M��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_LIMIT(36,90,6), 
            SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_dart_mastery, 0,     8,
            "�������R", "coin dart",ATTR_ALL, {2,1,2,3,2}

    },

    {  // �s�W alc �M�� skill 2022/04/29
        "artifacts mastery", "�]��M��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_artifacts_mastery, 0,     8,
            "�]�u���Y", "throw ammo",ATTR_ALL, {2,3,1,3,2}

    },

    {   // �s�W alc �M�� skill 2022/05/01
        "firearm mastery", "�j���q", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_LIMIT(36,99,6), SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
            spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
            &gsn_firearm_mastery, 0,     8,
            "�Y���g��", "head shot",ATTR_ALL, {4,4,4,5,5}

    },
	
    {
        "rainfall arrow", "�b�B", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, 
            SKILL_APP, SKILL_APP, SKILL_LIMIT(31,99,6), SKILL_APP, SKILL_APP },
            spell_null, TAR_IGNORE, POS_FIGHTING,
            &gsn_rainfall_arrow, 0,     20,
            "", "",ATTR_ALL, {-1, -1, -1, -1, -1}

    }

    /*
       {
       "extra heal", "�ͩR�O�W�V", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_extra_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },

       {
       "group extra heal", "�s��ͩR�O�W�V", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, 28, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_gextra_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },

       {
       "ogre power", "���]����", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       24, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_ogre_power, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },


       {
       "holy protection", "�t�F�[�@", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, 32, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_holy_protection, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       NULL, 0,     0,
       "", ""
       },

       {
       "cometslash", "�z�P��", { SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, 12, SKILL_APP, SKILL_APP,
       SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP, SKILL_APP },
       spell_null, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
       &gsn_cometslash, 0,     0,
       "", ""
       },

*/
};
