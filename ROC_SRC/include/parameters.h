//*****************************************************************
//*  PROGRAM ID  : parameters.h                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 jye                                    *
//*  PURPOSE     :         新增Z1,Z2,Z4四個變數                   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         新增EXP_PER_LEVEL ppl_class變數        *
//*              :         判斷未轉職前及lv38前每級升級只而1000exp*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1000124 tale                                    *
//*  PURPOSE     :         解決因output memory而導致crash的問題       *
//*              :         增加buffer memory (default 為 1024)*
//*****************************************************************
/*
 * Game's Parameters.'
 * 遊戲中的各樣參數設定.		-- Keric
 */

#ifndef __ROC_PARA_H__
#define __ROC_PARA_H__

#define PPL_HP_MULTIPLYER    2

#define MERIT_BONUS_MAX_SKEXP 1000000
#define BASE_MERIT 100000

#define TOP_COUNT 50

/* 
 * ENV variables.  -- Keric
 */

#define PLAYER_DIR	  "../player/"	      /* Player files			     */
#define BACKUP_DIR	  "../player/backup/"	      /* BACKUP files	*/
#define EQBANK_DIR	    "../eqbank/"
#define EQBANK_BACKUPDIR	    "../eqbank/backup/"
#define HORSE_DIR	  "../horse/"	      /* Horse files			     */
#define CLAN_DIR	    "../clan/"
#define HORSE_BACKUP_DIR	  "../horse/backup/"	      /* HORSE files	*/
#define NULL_FILE	 "/dev/null"	    /* To reserve one stream	    */
#define MOB_DIR		       "MOBProgs/"	  /* MOBProg files		  */
#define ROOM_DIR	       "ROOMProgs/"	  /* ROOMProg files		  */

/* 
 * ENV files list.  -- Keric 
 */

#define AREA_LIST	 "AREA.LST"	   /* List of areas		   */
#define CLAN_LIST	"../clan/CLAN.LST"
#define BUG_FILE	"BUGS.TXT"	/* For 'bug' and bug( )		       */
#define IDEA_FILE	 "IDEAS.TXT"	    /* For 'idea'			 */
#define TYPO_FILE	 "TYPOS.TXT"	 /* For 'typo'			      */
#define NOTE_FILE	 "NOTES.TXT"	    /* For 'notes'			  */
#define SHUTDOWN_FILE	     "SHUTDOWN.TXT"	   /* For 'shutdown'		    */
//#define DOWN_TIME_FILE	"TIME.TXT"	/* For automatic shutdown	*/
#define BAN_FILE	"BAN.TXT"	/* For banned site save		*/
#define CLAN_FILE	CLAN_LIST	/* For player clan */
#define TOP10_FILE	"TOP50.TXT" /* For top 50 ranking */
#define MAX_VNUM    131072
#define	SYMBOL_FILE	"SYMBOL.LST"


/*
 * String and memory management parameters.
 */
#define MAX_KEY_HASH              2048 
#define MAX_STRING_LENGTH         7132
#define BUF_STRING_LENGTH	        20480 /* modified by tale */
#define MAX_INPUT_LENGTH          255


/*
 * Max greeting number
 */
#define 		MAX_GREETING	7	

/*
 * PK 傷害分母
 */
#define     PK_DMG_DENOMINATOR  4

/*
 * Game parameters.
 * Increase the max'es if you add more of something.'
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_CHUNKS		   600			 /* Used in ssm.c  change 300 to 600 2020/10/21 */
//20060722 modify by jye
//新增EXP_PER_LEVEL ppl_class變數
//判斷未轉職前及lv38前每級升級只而1000exp
#define EXP_PER_LEVEL( level ,ppl_class) \
					( ( level  < 28 ) ? 1000 : 	\
				  	( level == 28 ) ? 1050 :	\
				  	( level == 29 ) ? 1100 :	\
				  	( level == 30 ) ? 1200 :	\
				  	( level == 31 ) ? 1300 :	\
				  	( level == 32 ) ? 1450 :	\
				  	( level == 33 ) ? 1600 :	\
				  	( level == 34 ) ? 1800 :	\
				  	( level == 35 ) ? 2100 :	\
				  	( level == 36 ) ? 4000 :	\
				  	( level == 37 ) ? 6300 :	\
				  	( level == 38 ) ? 12000 :	\
				  	( level == 39 ) ? 19200 :	\
				  	( level == 40 ) ? 28000 :	\
				  	( level < 43  ) ? 29000 : 0)
#define MAX_SKILL		        900
#define MAX_CLASS		        20
#define MAX_RACE		        66
#define RACE_ABILITY_COUNT  32
#define MAX_MATERIAL		    1
#define MAX_LEVEL		    47
#define L_ACG                       MAX_LEVEL
#define L_DIR			    (MAX_LEVEL -1)
#define L_SEN			    ( L_DIR - 1 )
#define L_JUN			    ( L_SEN - 1 )
#define L_APP			    ( L_JUN - 1 )
#define LEVEL_IMMORTAL		    L_APP
#define LEVEL_HERO		    ( LEVEL_IMMORTAL - 7 )

#define MAX_STR			    25
#define MAX_INT			    25
#define MAX_WIS			    25
#define MAX_DEX			    25
#define MAX_CON			    25

#define PULSE_PER_SECOND	    4
// duration of a combat round
#define PULSE_VIOLENCE		    ( 2 * PULSE_PER_SECOND )
#define PULSE_MOBILE		    ( 6 * PULSE_PER_SECOND )
#define PULSE_CALL_OUT		    ( 3 * PULSE_PER_SECOND )
// duratoin of one tick
#define PULSE_TICK		    ( 30 * PULSE_PER_SECOND * PPL_HP_MULTIPLYER)
// duration of area reborn ?
#define PULSE_AREA		    ( 60 * PULSE_PER_SECOND * PPL_HP_MULTIPLYER)



/* Save the database - OLC 1.1b */
#define PULSE_DB_DUMP		  (1800* PULSE_PER_SECOND ) /* 30 minutes  */

#define MAND		    3344556	   /* Magic # for manditory field	    */
#define SPECIFIED	 3344557 /* Key was used already.		  */
#define DEFLT		     3344558 /* Use default from fread_char_obj	      */

#define DEFLT_LL		     3344558LL /* Use default from fread_char_obj	      */


/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3
#define SKY_SNOWING		    4 //winter check add at 2018.11.06
#define SKY_SNOWSTORM		    5 //winter check add at 2018.11.06

/*
 * Connected state for a channel.
 */
#define CON_PLAYING			   0
#define CON_GET_NAME			    1
#define CON_GET_OLD_PASSWORD		    2
#define CON_CONFIRM_NEW_NAME		    3
#define CON_GET_NEW_PASSWORD		    4
#define CON_CONFIRM_NEW_PASSWORD	5
#define CON_DISPLAY_RACE		6
#define CON_GET_NEW_RACE		7
#define CON_CONFIRM_NEW_RACE		8
#define CON_GET_NEW_SEX			9
#define CON_DISPLAY_CLASS	       10
#define CON_GET_NEW_CLASS	       11
#define CON_CONFIRM_CLASS	       12
#define CON_READ_MOTD		       13
#define CON_YES_NO			14

/* Race ability bits */
#define RACE_NO_ABILITIES         0
#define RACE_PC_AVAIL             1
#define RACE_WATERBREATH          2
#define RACE_FLY                  4
#define RACE_SWIM                 8
#define RACE_WATERWALK           16
#define RACE_PASSDOOR            32
#define RACE_INFRAVISION         64
#define RACE_DETECT_ALIGN       128
#define RACE_DETECT_INVIS       256
#define RACE_DETECT_HIDDEN      512
#define RACE_PROTECTION        1024
#define RACE_SANCT             2048
#define RACE_WEAPON_WIELD      4096
#define RACE_MUTE              8192
#define RACE_NO_POISON        16384 
#define RACE_DETECT_MAGIC     32768 
#define RACE_REGENERACY       65536 
#define RACE_BEHOLDER        131072
#define RACE_LIZARDMAN       262144
#define RACE_ORC             524288
#define RACE_VAMPIRE        1048576
#define RACE_INVIS          2097152
#define RACE_ANTI_MAGIC     4194304
#define RACE_UNDEAD         8388608
#define RACE_WEREWOLF      16777216
#define RACE_GARGOYLE      33554432
#define RACE_CENTAUR       67108864


#define SIZE_0	0
#define SIZE_1	1
#define SIZE_2	2
#define SIZE_3  3
#define SIZE_4	4
#define SIZE_5	5
#define SIZE_6	6
#define SIZE_7	7
#define SIZE_8	8
#define SIZE_9	9


/*
 * Shop types.
 */
#define MAX_TRADE	  5


/***************************************************************************
 *									   *
 *		     VALUES OF INTEREST TO AREA BUILDERS		   *
 *		     (Start of section ... start here)			   *
 *									   *
 ***************************************************************************/

/*
 * Class number
 */
#define  MAGE                  0 
#define  CLERIC                1       
#define  THIEF                 2       
#define  WARRIOR               3       
#define  BARBARIAN             4       
#define  WIZARD                5       
#define  ALCHEMIST             6       
#define  SAVANT                7       
#define  BISHOP                8       
#define  ASSASSIN              9       
#define  NINJA                 10      
#define  KNIGHT                11      
#define  SWORDSMAN             12      
#define  MONK                  13      
#define  SHAMAN                14      
#define  SORCERER              15      
#define  LORD                  16      
#define  RANGER                17      
#define  WARLORD               18      
#define  MARTIALMAN            19      

/*
 * Race allow/deny bit
 */
#define  HUMAN                 1
#define  ELF                   2
#define  DWARF                 4
#define  ORC                   8
#define  HOBBIT                16
#define  FAIRY                 32
#define  GNOME                 64
#define  DROW                  128
#define  VAMPIRE               256
#define  BEHOLDER              512
#define  LIZARDMAN             1024
#define  WEREWOLF              2048
#define  MUMMY                 4096
#define  GARGOYLE              8192
#define  CENTAUR              16384

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	     3060
#define MOB_VNUM_VAMPIRE	   3404
#define MOB_VNUM_ULT		   3160
#define MOB_VNUM_SECRETARY	   3142
#define MOB_VNUM_MIDGAARD_MAYOR	   3143

#define MOB_VNUM_AIR_ELEMENTAL	   8914
#define MOB_VNUM_EARTH_ELEMENTAL   8915
#define MOB_VNUM_WATER_ELEMENTAL   8916
#define MOB_VNUM_FIRE_ELEMENTAL	   8917
#define MOB_VNUM_DUST_ELEMENTAL	   8918

//flame_elemental modify by jye 0940714 
#define MOB_VNUM_SOR_FLAME0	   14
#define MOB_VNUM_SOR_FLAME1	   15
#define MOB_VNUM_SOR_FLAME2	   16
//water_elemental modify by jye 0940714 
#define MOB_VNUM_SOR_WATER0	   9
#define MOB_VNUM_SOR_WATER1	   10
#define MOB_VNUM_SOR_WATER2	   11
//earth_elemental modify by jye 0940714 
#define MOB_VNUM_SOR_EARTH0	   6
#define MOB_VNUM_SOR_EARTH1	   7
#define MOB_VNUM_SOR_EARTH2    8
//wind_elemental modify by jye 0940714 
#define MOB_VNUM_SOR_WIND0 20
#define MOB_VNUM_SOR_WIND1 21
#define MOB_VNUM_SOR_WIND2 22
//lightning_elemental modify by jye 0940714 
#define MOB_VNUM_SOR_LIGHTNING0 17
#define MOB_VNUM_SOR_LIGHTNING1 18
#define MOB_VNUM_SOR_LIGHTNING2 19

#define MOB_VNUM_FIRE_ELEMENT1	   12
#define MOB_VNUM_EARTH0 23 
#define MOB_VNUM_EARTH1 24
#define MOB_VNUM_EARTH2 27
#define MOB_VNUM_EARTH3 25
#define MOB_VNUM_EARTH4 28
#define MOB_VNUM_EARTH5 26
#define MOB_VNUM_EARTH6 29
#define MOB_VNUM_WATER0 30
#define MOB_VNUM_WATER1 31
#define MOB_VNUM_WATER2 34
#define MOB_VNUM_WATER3 32
#define MOB_VNUM_WATER4 35
#define MOB_VNUM_WATER5 33
#define MOB_VNUM_WATER6 36
/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC			1		 /* Auto set for mobs	     */
#define ACT_SENTINEL			  2		   /* Stays in one room	       */
#define ACT_SCAVENGER			   4		    /* Picks up objects	       */
#define ACT_CLAN			   8	/* clan mob */
#define ACT_SPEEDHIT            16  /* mob 攻擊必然命中 */ 
#define ACT_AGGRESSIVE			   32		     /* Attacks PC's		    */
#define ACT_STAY_AREA			  64		    /* Won't leave area	       */
#define ACT_WIMPY		     128		/* Flees when hurt	  */
#define ACT_PET				   256		      /* Auto set for pets	  */
#define ACT_TRAIN		     512		/* Can train PC's	 */
#define ACT_PRACTICE		       1024		   /* Can practice PC's	       */
#define ACT_CRITICAL		   2048		/* Mob have chance to have critical hit */
#define ACT_HARD_DIS		   4096		/* Mob hard disarm */
#define ACT_HUNT		   8192		/* Mob can hunt */
#define ACT_DUAL		 16384		/* Mob can use dual weapon	*/
#define ACT_NO_FLEE		 32768		/* Mob don't do_flee  add 2018/8/10 */
#define ACT_MOVED		 131072		/* Dont ever set!	*/
#define ACT_TACTICS		 262144		/* Mob can use tactics	*/
#define ACT_TRACK		 524288		/* Mob can track enemy	*/
#define ACT_REGENERACY		 1048576	/* Mob can regenerate	*/
#define ACT_GAMBLE		 2097152 	/* Runs a gambling game */
#define ACT_BANKER     		 4194304	/* Banker */
#define ACT_MOB_AI			8388608		/* Mob have AI in fight */
#define ACT_REFLEXION		 16777216	/* Mob can reflexion	*/
#define ACT_NOPAR	    33554432 /* no reduce exp */
#define ACT_IGNORE_NOMOB    67108864/* no reduce exp */
#define ACT_MPROG_DEATH	    134217728	/* don't ever set that */
#define ACT_SMITH	    268435456
#define ACT_HORSE           536870912
#define ACT_CORPSE_COUNT   1073741824
/*
 * Bits fot auto combat
 *
 */
#define AUTOC_NORMAL        0
#define AUTOC_STOP          1
#define	AUTOC_DODGE         2
#define AUTOC_PARRY         4
#define AUTOC_CONCENTRATION 8
#define AUTOC_CRITICAL	    16
#define AUTOC_COOPERATE	    32
#define AUTOC_POWER         64
#define AUTOC_INSTANT       128
#define AUTOC_CHARGE        256
#define AUTOC_RELOAD        512 // 自動裝備彈藥 2022/05/01
/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		       1LL
#define AFF_INVISIBLE			   2LL
#define AFF_DETECT_EVIL			     4LL
#define AFF_DETECT_INVIS	      8LL
#define AFF_DETECT_MAGIC	     16LL
#define AFF_DETECT_HIDDEN	      32LL
#define AFF_HOLD		     64LL
#define AFF_SANCTUARY			 128LL
#define AFF_FAERIE_FIRE			   256LL
#define AFF_INFRARED			512LL
#define AFF_CURSE		    1024LL
#define AFF_CHANGE_SEX			 2048LL
#define AFF_POISON		     4096LL
#define AFF_PROTECT		      8192LL
#define AFF_POLYMORPH		       16384LL
#define AFF_SNEAK		   32768LL
#define AFF_HIDE		  65536LL
#define AFF_SLEEP		  131072LL
#define AFF_CHARM		  262144LL
#define AFF_FLYING		   524288LL
#define AFF_PASS_DOOR		     1048576LL
#define AFF_WATERWALK		2097152LL
#define AFF_SUMMONED		4194304LL
#define AFF_MUTE		8388608LL
#define AFF_SEAL_MAGIC		16777216LL
#define AFF_VAMP_BITE	       33554432LL
#define AFF_GHOUL	         67108864LL
#define AFF_FLAMING	        134217728LL
#define AFF_PARALYZED	    268435456LL
#define AFF_CRAZY	        536870912LL
#define AFF_WEAKEN		    1073741824LL
#define AFF_NIGHTSTALK		2147483648LL
#define AFF_DARK_RAID		4294967296LL
#define AFF_MANA_SHIELD		8589934592LL
#define AFF_GILLS	        17179869184LL
#define AFF_TARDY	        34359738368LL
#define AFF_MYSTIC_MIRROR	68719476736LL
#define AFF_MYSTIC_ARMOR	137438953472LL
#define AFF_CAST_ACC        274877906944LL
#define AFF_CONTINUE_CURE	549755813888LL
#define AFF_DISEASE         1099511627776LL
#define AFF_FEAR            2199023255552LL
#define AFF_RIDE            4398046511104LL
#define AFF_BATTLE          8796093022208LL
#define AFF_NO_HEAL         17592186044416LL
/*
 * Mob special attributes
 */
#define NO_DM		1LL
#define NO_LORE		2LL
#define NO_BLIND	4LL
#define NO_CHARM	8LL
#define NO_CURSE	16LL
#define NO_FAEFIRE	32LL
#define NO_POISON	64LL
#define NO_SLEEP	128LL
#define DAM_HALF	256LL
#define ANTI_MAGIC	512LL
#define REF_MAGIC	1024LL
#define ALY_SAN		2048LL
#define ALY_INVIS	4096LL
#define ALY_HIDE	8192LL
#define ALY_SNEAK	16384LL
#define ALY_DI		32768LL
#define ALY_DH		65536LL
#define ALY_INF		131072LL
#define ALY_BAR		262144LL
#define NO_TARDY	524288LL
#define NO_MUTE		1048576LL
#define NO_SEALMAGIC	2097152LL
#define EXPERT		4194304LL
#define	NO_CRAZY	8388608LL
#define MOBAI_LV2	16777216LL
#define MOBAI_LV3	33554432LL
#define NO_PARA		67108864LL
#define NO_EXP		134217728LL
#define SPELL_FURY  268435456LL
#define NO_FEAR     536870912LL
#define NO_DISARM   1073741824LL
#define NO_TRIP     2147483648LL
/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL			 0
#define SEX_MALE		     1
#define SEX_FEMALE			 2

/*
 * special.
 */
#define SPECIAL_EFFECT_SMART		1
#define SPECIAL_EFFECT_BEAUTY		2
/*
 * alignment
 */
#define ALIGN_EVIL			0
#define ALIGN_GOOD			1
#define ALIGN_NEUTRAL		2

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE		2
#define OBJ_VNUM_MONEY_SOME		 3

#define OBJ_VNUM_CORPSE_NPC		10
#define OBJ_VNUM_CORPSE_PC	       11
#define OBJ_VNUM_SEVERED_HEAD		  12
#define OBJ_VNUM_TORN_HEART		13
#define OBJ_VNUM_SLICED_ARM		14
#define OBJ_VNUM_SLICED_LEG		15
#define OBJ_VNUM_FINAL_TURD		16

#define OBJ_VNUM_MUSHROOM	      20
#define OBJ_VNUM_LIGHT_BALL		21
#define OBJ_VNUM_SPRING			    22
#define OBJ_VNUM_POWER_GEM	      23
#define OBJ_VNUM_HIT		    49
#define OBJ_VNUM_MAGIC_ARROW	    65
#define OBJ_VNUM_MAGIC_AMMO	    66  // add at 2022/04/29

#define OBJ_VNUM_SCHOOL_MACE	       3700
#define OBJ_VNUM_SCHOOL_DAGGER		 3701
#define OBJ_VNUM_SCHOOL_SWORD		3702
#define OBJ_VNUM_SCHOOL_VEST	       3703
#define OBJ_VNUM_SCHOOL_SHIELD		 3704
#define OBJ_VNUM_SCHOOL_BANNER	   3716
#define OBJ_VNUM_SCHOOL_CLUB	   3717
#define OBJ_VNUM_SCHOOL_BOOK	   3733
#define OBJ_VNUM_BANDAGE            3146
#define OBJ_VNUM_BLACK_POWDER	   8903
#define OBJ_VNUM_FLAMEBLADE	   8920
#define OBJ_VNUM_STAKE		   3811
//#define OBJ_VNUM_LICENSE	   3011
#define OBJ_VNUM_LICENSE	   11000 //亂設的

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.'
 */
#define OBJ_VNUM_DUMMY	1
/*
 * obj->pIndexData->souvenir.
 * Used in #OBJECTS.
 */
#define Z1      1 //紀念品
#define Z2      2 //Mob 專用物品
#define Z4      4 //特殊物品

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT            1
#define ITEM_SCROLL           2
#define ITEM_WAND             3
#define ITEM_STAFF            4
#define ITEM_WEAPON           5
#define ITEM_TREASURE         8
#define ITEM_ARMOR            9
#define ITEM_POTION          10
#define ITEM_FURNITURE       12
#define ITEM_TRASH           13
#define ITEM_CONTAINER       15
#define ITEM_DRINK_CON       17
#define ITEM_KEY             18
#define ITEM_FOOD            19
#define ITEM_MONEY           20
#define ITEM_BOAT            22
#define ITEM_CORPSE_NPC      23
#define ITEM_CORPSE_PC       24
#define ITEM_FOUNTAIN        25
#define ITEM_PILL            26
#define ITEM_ARROW           27
#define ITEM_DART            28
#define ITEM_HORSE           29
#define ITEM_AMMO            30  // add at 2022/04/29

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		       1LL
#define ITEM_HUM		      2LL
#define ITEM_DARK		       4LL
#define ITEM_LOCK		       8LL
#define ITEM_EVIL		      16LL
#define ITEM_INVIS		       32LL
#define ITEM_MAGIC		       64LL
#define ITEM_NODROP		       128LL
#define ITEM_BLESS		      256LL
#define ITEM_ANTI_GOOD			  512LL
#define ITEM_ANTI_EVIL			 1024LL
#define ITEM_ANTI_NEUTRAL	    2048LL
#define ITEM_NOREMOVE			4096LL
#define ITEM_INVENTORY			 8192LL
#define ITEM_NOSAVE		  16384LL
#define ITEM_VAMPIRE_BANE	  32768LL
#define ITEM_HOLY		  65536LL
#define ITEM_POISONED		 131072LL
#define ITEM_REMOVE_NODROP	 262144LL
#define ITEM_DONATION_PIT	 524288LL
#define ITEM_UNIQUE		1048576LL
#define ITEM_CLAN	    2097152LL
#define ITEM_PERSONAL	    4194304LL
#define ITEM_SPELL	    8388608LL // ITEM_RACIAL change to ITEM_SPELL 2021/09/19
#define ITEM_LORE	    16777216LL
#define ITEM_NOSAC	    33554432LL
#define ITEM_RECOVER	    67108864LL
#define ITEM_SPECIAL           134217728LL
#define ITEM_BROKEN			   268435456LL
#define ITEM_POISONED_WEAPON	536870912LL
#define ITEM_NO_MALE           1073741824LL
#define ITEM_NO_NEUTRAL        2147483648LL
#define ITEM_NO_FEMALE         4294967296LL

/* item durabilities
 */
#define DURA_AUTO		    (-2)
#define DURA_INDESTRUCTIBLE	    (-1)
#define DURA_MAX		    1000
/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE               1
#define ITEM_WEAR_FINGER        2
#define ITEM_WEAR_NECK          4
#define ITEM_WEAR_BODY          8
#define ITEM_WEAR_HEAD         16
#define ITEM_WEAR_LEGS         32
#define ITEM_WEAR_FEET         64
#define ITEM_WEAR_HANDS       128 
#define ITEM_WEAR_ARMS        256
#define ITEM_WEAR_SHIELD      512
#define ITEM_WEAR_ABOUT      1024 
#define ITEM_WEAR_WAIST      2048
#define ITEM_WEAR_WRIST      4096
#define ITEM_WIELD           8192
#define ITEM_HOLD           16384
#define ITEM_TWO_HAND       32768
#define ITEM_WEAR_ARROW     65536
#define ITEM_WEAR_RIDE     131072
#define ITEM_WEAR_AMMO     262144  // add at 2022/04/29

/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE                 0
#define APPLY_STR                  1
#define APPLY_DEX                  2
#define APPLY_INT                  3
#define APPLY_WIS                  4
#define APPLY_CON                  5
#define APPLY_SEX                  6
#define APPLY_CLASS                7
#define APPLY_LEVEL                8
#define APPLY_AGE                  9
#define APPLY_HEIGHT              10
#define APPLY_WEIGHT              11
#define APPLY_MANA                12
#define APPLY_HIT                 13
#define APPLY_MOVE                14
#define APPLY_GOLD                15
#define APPLY_EXP                 16
#define APPLY_AC                  17
#define APPLY_HITROLL             18
#define APPLY_DAMROLL             19
#define APPLY_SAVING_PARA         20
#define APPLY_SAVING_LIFE         21
#define APPLY_SAVING_PETRI        22
#define APPLY_SAVING_BREATH       23
#define APPLY_SAVING_SPELL        24
#define APPLY_SAVING_ANCIENT      25
#define APPLY_SAVING_SPIRIT       26
#define APPLY_SAVING_FIRE         27
#define APPLY_SAVING_COLD         28
#define APPLY_SAVING_LIGHTNING    29
#define APPLY_SAVING_EARTH        30
#define APPLY_SAVING_WIND         31
#define APPLY_SAVING_SAINT        32
#define APPLY_SAVING_DARK         33
#define APPLY_SAVING_POISON       34
#define APPLY_SAVING_ELEMENT      35
#define APPLY_SAVING_GOOD         36
#define APPLY_SAVING_EVIL         37
#define APPLY_RACE                38
#define APPLY_DODGE               39
#define APPLY_POWER_POTENTIAL     40
#define APPLY_RDAMAGE             41
#define APPLY_RMAGIC              42
#define APPLY_ADAMAGE             43
#define APPLY_AMAGIC              44
#define APPLY_GPHYSICAL           45 // mob only, Psycical Damage Gate
#define APPLY_GMAGIC              46 // mob only, Magical Damage Gate
#define APPLY_DOB_SAVING_FLASH    47 // for dob Sun Flash resistance
#define APPLY_SAVING_ILLUSION     48 // for illusion resistance
#define APPLY_ACTION_ABILITY      49 // for action ability
#define APPLY_WEAPON_SPELL        50 // spell damage for weapon, 21/09/19
#define APPLY_WEAPON_WIND         51 // wind damage
#define APPLY_WEAPON_FIRE         52 // fire damage
#define APPLY_WEAPON_COLD         53 // cold damage
#define APPLY_WEAPON_EARTH        54 // earth damage
#define APPLY_WEAPON_LIGHTNING    55 // lightning damage
#define APPLY_WEAPON_SAINT        56 // saint damage
#define APPLY_WEAPON_DARK         57 // dark damage
#define APPLY_MAGIC_IMMUNE        58 // 用於 mystic armor 計算值 2021/11/03
#define APPLY_TOXICITY_POISON     59 // 用於 poison item 計算 2021/11/10
#define APPLY_DAMAGE_COUNT        60 // 計算 blood thirsty 戰鬥中的累計值 2022/02/20
#define APPLY_SHOOT_HITROLL       61 // 飛行物品( arrow , ammo) 被射擊時的 hr 加成 2022/05/06
#define APPLY_SHOOT_DAMROLL       62 // 飛行物品( arrow , ammo) 被射擊時的 dr 加成 2022/02/20
#define APPLY_SHOOT_ADAMAGE       63 // 飛行物品( arrow , ammo) 被射擊時的 apdr 加成 2022/02/20

/* for Hidden Power
 */
#define	HP_WEAR	    1
#define	HP_FIGHT    2
#define	HP_WEAPON   4
#define HP_BLOCK    8
#define HP_ARROW    16
#define HP_REMOVE   32
#define HP_DART	    64

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE			    1
#define CONT_PICKPROOF			    2
#define CONT_CLOSED			 4
#define CONT_LOCKED			 8

/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO			     2
#define ROOM_VNUM_CHAT			 1200
#define ROOM_VNUM_TEMPLE	   3001
#define ROOM_VNUM_ALTAR			  3054
#define ROOM_VNUM_SCHOOL	   3700
#define ROOM_VNUM_GRAVEYARD_A	    427
#define ROOM_VNUM_PURGATORY_A	    401
#define ROOM_VNUM_RUBY_INN	   3048

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK                 1
#define ROOM_NO_FLEE              2
#define ROOM_NO_MOB               4
#define ROOM_INDOORS              8
#define ROOM_UNDERGROUND          16
#define ROOM_SCOUT_ONE            32
#define ROOM_NO_RECALL_IN         128  // 從其他的area無法recall進入mark的area 18/8/8
#define ROOM_NO_MARK              256   // room 無法施放mark 18/8/8
#define ROOM_PRIVATE              512
#define ROOM_SAFE                 1024
#define ROOM_SOLITARY             2048
#define ROOM_PET_SHOP             4096
#define ROOM_NO_RECALL            8192
#define ROOM_CONE_OF_SILENCE      16384
#define ROOM_CAMP                 32768
#define ROOM_SUMMON_BIT           65536
#define ROOM_AUCTION              131072 
#define ROOM_SAVE                 262144
#define ROOM_NO_TELEPORT          524288
#define ROOM_NO_TELEPORT_TO       1048576
#define ROOM_PK                   2097152
#define ROOM_MAGIC_TOWER          4194304
#define ROOM_MIL_DEPARTMENT       8388608
#define ROOM_MISC_RESEARCH        16777216
#define ROOM_CLAN_LV1             33554432
#define ROOM_CLAN_LV2             67108864
#define ROOM_CLAN_LV3             134217728
#define ROOM_BARRACK              268435456
#define ROOM_ARMORY               536870912
#define ROOM_HORSE_SHOP           1073741824
#define ROOM_TAVERN               32
#define ROOM_LIBRARY              64

/* 
 * Room affect flags
 *
 */
#define R_AFF_FIREWALL		1LL

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH			0
#define DIR_EAST			1
#define DIR_SOUTH			2
#define DIR_WEST			3
#define DIR_UP				4
#define DIR_DOWN			5
#define DIR_NORTHEAST		6
#define DIR_NORTHWEST		7
#define DIR_SOUTHEAST		8
#define DIR_SOUTHWEST		9


/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		       1
#define EX_CLOSED		       2
#define EX_LOCKED		       4
#define EX_BASHED		      8
#define EX_BASHPROOF		     16
#define EX_PICKPROOF			 32
#define EX_PASSPROOF		     64
#define EX_INVIS		    128

/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE			 0
#define SECT_CITY		       1
#define SECT_FIELD			2
#define SECT_FOREST			 3
#define SECT_HILLS			4
#define SECT_MOUNTAIN			   5
#define SECT_WATER_SWIM			     6
#define SECT_WATER_NOSWIM	       7
#define SECT_UNDERWATER		       8
#define SECT_AIR		      9
#define SECT_DESERT			10
#define SECT_MAX		     11



/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE          -1
#define WEAR_LIGHT          0
#define WEAR_FINGER_L       1
#define WEAR_FINGER_R       2
#define WEAR_NECK_1         3
#define WEAR_NECK_2         4
#define WEAR_BODY           5
#define WEAR_HEAD           6
#define WEAR_LEGS           7
#define WEAR_FEET           8
#define WEAR_HANDS          9
#define WEAR_ARMS          10
#define WEAR_SHIELD        11
#define WEAR_ABOUT         12
#define WEAR_WAIST         13
#define WEAR_WRIST_L       14
#define WEAR_WRIST_R       15
#define WEAR_WIELD         16
#define WEAR_HOLD          17
#define WEAR_WIELD_2       18   /* by Thelonius */
#define WEAR_TWO_HAND      19   /* by Amenda */
#define WEAR_ARROW         20   /* by Amenda */
#define MAX_WEAR           21
#define WEAR_FINGER_3      22   /* by Amenda */
#define WEAR_FINGER_4      23   /* by Amenda */
#define WEAR_RIDE          24   /* by Zcecil */
#define WEAR_ARMS_2        25   /* by Keelar */
#define WEAR_HANDS_2       26   /* by Keelar */
#define WEAR_LEGS_2        27   /* by Keelar */
#define WEAR_FEET_2        28   /* by Keelar */
#define WEAR_AMMO          29   /* add at 20220430 */

/***************************************************************************
 *									   *
 *		     VALUES OF INTEREST TO AREA BUILDERS		   *
 *		     (End of this section ... stop here)		   *
 *									   *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK			0
#define COND_FULL		       1
#define COND_THIRST			 2

/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL			1
#define POS_INCAP		       2
#define POS_STUNNED			 3
#define POS_SLEEPING			  4
#define POS_PARALYZED			 5
#define POS_RESTING			 6
#define POS_FIGHTING			  7
#define POS_STANDING			  8

/*
 * ACT bits for players.
 */
#define PLR_IS_NPC			1LL		 /* Don't EVER set.	   */
#define PLR_STATUS			    2LL
#define PLR_REGISTER		      4LL		/* Registering for Pkill */
#define PLR_AUTOEXIT			  8LL
#define PLR_AUTOLOOT			 16LL
#define PLR_AUTOSAC		     32LL
#define PLR_BLANK		      64LL
#define PLR_BRIEF		     128LL
#define PLR_COMBINE		       512LL
#define PLR_PROMPT		     1024LL
#define PLR_AUTOSPLIT	    2048LL

#define PLR_HOLYLIGHT			4096LL
#define PLR_WIZINVIS		       8192LL
#define PLR_WIZBIT		    16384LL
#define	PLR_SILENCE		    32768LL
#define PLR_NO_EMOTE		      65536LL
#define PLR_MOVED		  131072LL
#define PLR_NO_TELL		    262144LL
#define PLR_LOG				524288LL
#define PLR_DENY		1048576LL
#define PLR_FREEZE		  2097152LL
#define PLR_THIEF		 4194304LL
#define PLR_KILLER		  8388608LL
#define PLR_AUTOGOLD	       16777216LL
#define PLR_AFK		       33554432LL
#define PLR_ARTIFACT		67108864LL
#define PLR_FIST_FIGHT		134217728LL
#define PLR_COMBAT		268435456LL
#define PLR_CHINESE		536870912LL
#define PLR_HORSE	   1073741824LL
#define PLR_NOTIFY	   2147483648LL
#define PLR_VERBOSEALL	   4294967296LL
#define PLR_TITLE	    8589934592LL
#define PLR_ASSIST	17179869184LL
#define PLR_DEAD    34359738368LL
#define PLR_GRAPHIC 68719476736LL
#define PLR_IDLE    137438953472LL
#define PLR_RIDE    274877906944LL
// 2^64 = 4611686018427387904


#define MAGIC_SPELL	    0	    //pure magic
#define MAGIC_FIRE	    1
#define MAGIC_COLD	    2
#define MAGIC_WIND	    3
#define MAGIC_EARTH	    4
#define MAGIC_BREATH	    5
#define MAGIC_LIGHTNING	    6
#define MAGIC_SAINT	    7
#define MAGIC_DARK	    8
#define MAGIC_SPIRIT	    9
#define MAGIC_ANCIENT	    10
#define MAGIC_POISON	    11
#define MAGIC_NATURE	    12
#define MAGIC_GOOD	    13
#define MAGIC_EVIL	    14
#define	SKILL_PASSIVE	    15	    //normal skills
#define SKILL_SLASH	    16	    //劍術
#define MAX_SAVING	    17


/*
 * Obsolete bits.
 */
#if 0
#define PLR_CHAT		    256	       /* Obsolete	  */
#endif

/*
 * Channel bits.
 */
#define	       CHANNEL_AUCTION			    1
#define	       CHANNEL_CHAT			 2

#define	       CHANNEL_IMMTALK			    8
#define	       CHANNEL_MUSIC			 16
#define	       CHANNEL_QUESTION		    32
#define	       CHANNEL_SHOUT			 64
#define	       CHANNEL_YELL		       128
#define	       CHANNEL_RUMOR		       256
#define		CHANNEL_BET			512
#define		CHANNEL_SYSTEM		    1024
#define		CHANNEL_HEROTALK	2048

#endif
