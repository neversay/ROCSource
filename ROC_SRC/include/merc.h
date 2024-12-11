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
//*  PROGRAM ID  : merc.h                                         *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940711 jye                                    *
//*  PURPOSE     :         穝糤spell_flame_elemental              *
//*              :             spell_water_elemental              *
//*              :             spell_earth_elemental              *
//*              :             spell_wind_elemental               *
//*              :             spell_lightning_elemental          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940811 Razgriz                                *
//*  PURPOSE     :          struct char_data い跑计         *
//*              :         time_t time_stamp                      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940908 Razgriz                                *
//*  PURPOSE     :         穝糤 check_exit() ㄧ计             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940914 jye                                    *
//*  PURPOSE     :         穝糤do_mpget mob_commands              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940915 Razgriz                                *
//*  PURPOSE     :         穝糤 do_mpset_obj_affect    ㄧ计   *
//*              :         穝糤 do_mpremove_obj_affect ㄧ计   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         穝糤 do_mpmodify_obj_affect ㄧ计   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         穝糤 do_opset_obj_affect    ㄧ计   *
//*              :         穝糤 do_opremove_obj_affect ㄧ计   *
//*              :         穝糤 do_opmodify_obj_affect ㄧ计   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         穝糤 do_forceo ㄧ计                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         穝糤 struct obj_extra_value            *
//*              :          obj_data い extra_value 跑计    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         穝糤 do_opmodvt ㄧ计               *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Razgriz                                *
//*  PURPOSE     :         穝糤 obj_find  ㄧ计                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 Razgriz                                *
//*  PURPOSE     :         穝糤 do_mpforceo  ㄧ计             *
//*****************************************************************
/* $Id: merc.h,v 1.42 2005/02/13 08:08:55 mudadm Exp $ */

/*
 * Accommodate old non-Ansi compilers.
 */
#ifndef __MERC_H__
#define __MERC_H__

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Game settings. -- Keric */
#include "parameters.h"

#if defined( TRADITIONAL )
#define const
#define args( list )			    ( )
#define DECLARE_DO_FUN( fun )		     void fun( )
#define DECLARE_SPEC_FUN( fun )		       bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#define DECLARE_GAME_FUN( fun )		void fun( )
#else
#define args( list )			    list
#define DECLARE_DO_FUN( fun )		     DO_FUN    fun
#define DECLARE_DO_RFUN( fun )		     DO_RFUN    fun
#define DECLARE_DO_OFUN( fun )		     DO_OFUN    fun
#define DECLARE_SPEC_FUN( fun )		       SPEC_FUN	 fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#define DECLARE_GAME_FUN( fun )		       GAME_FUN	 fun
#endif

#define LATEST_VERSION      37
/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	   !defined( FALSE )
#define FALSE	      0
#endif

#if	   !defined( TRUE )
#define TRUE	     1
#endif

#if	   defined( _AIX )
#if	   !defined( const )
#define const
#endif
typedef int bool;
#define unix
#else
typedef unsigned char bool;
#endif


/*
 * Ok here we define strdup so it can no longer be confused
 * with str_dup.  Suggested by erwin@pip.dknet.dk - Kahn.
 */
//#define strdup	STRDUP_ERROR__USE_STR_DUP!


#include "memwatch.h"
#define	    BV00	    (1 <<  0)	/*          1 */
#define	    BV01	    (1 <<  1)	/*          2 */
#define	    BV02	    (1 <<  2)	/*          4 */
#define	    BV03	    (1 <<  3)	/*          8 */
#define	    BV04	    (1 <<  4)	/*         16 */
#define	    BV05	    (1 <<  5)	/*         32 */
#define	    BV06	    (1 <<  6)	/*         64 */
#define	    BV07	    (1 <<  7)	/*        128 */
#define	    BV08	    (1 <<  8)	/*        256 */
#define	    BV09	    (1 <<  9)	/*        512 */
#define	    BV10	    (1 << 10)	/*       1024 */
#define	    BV11	    (1 << 11)	/*       2048 */
#define	    BV12	    (1 << 12)	/*       4096 */
#define	    BV13	    (1 << 13)	/*       8192 */
#define	    BV14	    (1 << 14)	/*      16384 */
#define	    BV15	    (1 << 15)	/*      32768 */
#define	    BV16	    (1 << 16)	/*      65536 */
#define	    BV17	    (1 << 17)	/*     131072 */
#define	    BV18	    (1 << 18)	/*     262144 */
#define	    BV19	    (1 << 19)	/*     524288 */
#define	    BV20	    (1 << 20)	/*    1048576 */
#define	    BV21	    (1 << 21)	/*    2097152 */
#define	    BV22	    (1 << 22)	/*    4194304 */
#define	    BV23	    (1 << 23)	/*    8388608 */
#define	    BV24	    (1 << 24)	/*   16777216 */
#define	    BV25	    (1 << 25)	/*   33554432 */
#define	    BV26	    (1 << 26)	/*   67108864 */
#define	    BV27	    (1 << 27)	/*  134217728 */
#define	    BV28	    (1 << 28)	/*  268435456 */
#define	    BV29	    (1 << 29)	/*  536870912 */
#define	    BV30	    (1 << 30)	/* 1073741824 */

/*
 * Structure types.
 */
typedef struct affect_data			AFFECT_DATA;
typedef struct area_data			AREA_DATA;
typedef struct auction_data			AUC_DATA;
typedef struct auction_data			AUCTION_DATA;
typedef struct ban_data				BAN_DATA;
typedef struct call_out_data		CALL_OUT_DATA;
typedef struct char_data			CHAR_DATA;
typedef struct clan_data			CLAN_DATA;
typedef struct descriptor_data		DESCRIPTOR_DATA;
typedef struct exit_data			EXIT_DATA;
typedef struct extra_descr_data		EXTRA_DESCR_DATA;
typedef struct group_data			GROUP_DATA;
typedef struct help_data			HELP_DATA;
typedef struct kill_data			KILL_DATA;
typedef struct mob_action_data		MOB_ACTION;
typedef struct mob_index_data		MOB_INDEX_DATA;
typedef struct mob_prog_act_list	MPROG_ACT_LIST;
typedef struct mob_prog_data		MPROG_DATA;
typedef struct note_data			NOTE_DATA;
typedef struct obj_data				OBJ_DATA;
typedef struct obj_hidden_power		OBJ_HP;
typedef struct obj_index_data		OBJ_INDEX_DATA;
typedef struct obj_prog_data		OPROG_DATA;
typedef struct obj_extra_value		OBJ_EXTRA_VALUE;	/* Razgriz 20050918 */
typedef struct pc_data				PC_DATA;
typedef struct quest_mark_data		QMARK;
typedef struct qmark_name			QMARK_STRING;
typedef struct rcall_out_data		RCALL_OUT_DATA;
typedef struct reset_data			RESET_DATA;
typedef struct room_index_data		ROOM_INDEX_DATA;
typedef struct room_prog_data		RPROG_DATA;
typedef struct shop_data			SHOP_DATA;
typedef struct symbol_data			SYMBOL_DATA;
typedef struct time_info_data		TIME_INFO_DATA;
typedef struct top10_data			TOP10_DATA;
typedef struct weather_data			WEATHER_DATA;
typedef struct mob_weakness_data	MOB_WEAKNESS;
typedef struct battle_array			BATTLE_ARRAY;
typedef struct obj_vs_race			OBJ_VRACE;
typedef	struct area_skill_data		ASKILL_DATA;
typedef struct bank_account_data	BANK_DATA;
typedef struct clan_war_data		CLAN_WAR_DATA;

/*
 * Function types.
 */
typedef void DO_FUN args( ( CHAR_DATA *ch, char *argument ) );
typedef void DO_RFUN args( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *argument ) );
typedef void DO_OFUN args( ( OBJ_DATA *obj, CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN args( ( CHAR_DATA *ch ) );
typedef void SPELL_FUN args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
typedef void GAME_FUN args( ( CHAR_DATA *ch, CHAR_DATA *croupier, char *argument ) );


/*
 * Player character key data struct
 * Stuff for new error trapping of corrupt pfiles.
 */
struct key_data
{
	char key[12];		/* Increase if you make a key > 11 chars */
	int string;			/* TRUE for string, FALSE for int	*/
	int deflt;			/* Default value or pointer	       */
	void *ptrs[20];		/* Increase if you have > 6 parms/line	 */
};


/*
 * Site ban structure.
 */
struct ban_data
{
    BAN_DATA *next;
    char *name;
};


/*
 * Time and weather stuff.
 */

struct time_info_data
{
    int hour;
    int day;
    int month;
    int year;
};

struct weather_data
{
    int mmhg;
    int change;
    int sky;
    int sunlight;
};



/*
 * Race structures
 */
struct race_type
{
    char *name;
    char *cname;
    int race_abilities;
    int size;
    int str_mod;
    int int_mod;
    int wis_mod;
    int dex_mod;
    int con_mod;
    int hp_gain;
    int mana_gain;
    int move_gain;
    int thirst_mod;
    int hunger_mod;
    int nature_svs[ 9];
    int nature_dodge;
    int nature_ac;
    char *dmg_message;
    char *hate;
    int glow;
    int ghigh;
};

/*
 * Drunkeness communication structure.
 */
struct struckdrunk
{
    int min_drunk_level;
    int number_of_rep;
    char *replacement[11];
};


/*
 * Descriptor (channel) structure.
 */
struct descriptor_data
{
    DESCRIPTOR_DATA *next;
    DESCRIPTOR_DATA *snoop_by;
    CHAR_DATA *character;
    CHAR_DATA *original;
    char *host;
    char *remoteuser;
    unsigned int descriptor;
    int connected;
    bool fcommand;
    char inbuf[ MAX_INPUT_LENGTH*4 ];
    char incomm[ MAX_INPUT_LENGTH   ];
    char inlast[ MAX_INPUT_LENGTH   ];
    int repeat;
    char *showstr_head;
    char *showstr_point;
    char *outbuf;
    int outsize;
    int outtop;
    void *pEdit;		/* OLC */
    char **pString;		/* OLC */
    int editor;			/* OLC */
	int trytimes;		/* Keric Added for try password times*/
};



/*
 * Attribute bonus structures.
 */
struct str_app_type
{
    int tohit;
    int todam;
    int carry;
    int wield;
    int wield2;
    int twohand;
};

struct int_app_type
{
    int learn;
};

struct wis_app_type
{
    int practice;
    int tohit;
};

struct dex_app_type
{
    int defensive;
    int tohit;
};

struct con_app_type
{
    int hitp;
    int shock;
    int armor;
};



/*
 * TO types for act.
 */
#define TO_ROOM			    0
#define TO_NOTVICT		    1
#define TO_VICT			    2
#define TO_CHAR			    3
#define TO_ROOM_IMM			4//1050619 add TO_ROOM_IMM 倒roomいimm



/*
 * Help table types.
 */
struct help_data
{
    HELP_DATA *next;
    int level;
    char *keyword;
    char *text;
    AREA_DATA *area;		/* OLC */
};



/*
 * Shop types.
 */
struct shop_data
{
    SHOP_DATA *next;			/* Next shop in list */
    int keeper;					/* Vnum of shop keeper mob */
    int buy_type[ MAX_TRADE ];	/* Item types shop will buy */
    int profit_buy;				/* Cost multiplier for buying */
    int profit_sell;			/* Cost multiplier for selling */
    int open_hour;				/* First opening hour */
    int close_hour;				/* First closing hour */
};



/*
 * Per-class stuff.
 */
struct class_type
{
    char who_name[ 4 ];		/* Three-letter name for 'who' */
    char name[11];
    char cname[10];
    char cwho_name[10];
    int attr_prime;			/* Prime attribute */
    int weapon;				/* First weapon */
    int guild;				/* Vnum of guild room */
    int skill_adept;		/* Maximum skill level */
    int thac0_00;			/* Thac0 for level  0 */
    int thac0_42;			/* Thac0 for level 42 */
    int hp_min;				/* Min hp gained on leveling */
    int hp_max;				/* Max hp gained on leveling */
    bool fMana;				/* Class gains mana on level */
    int bclass[5];			/* basic class */
};



/*
 * Data structure for notes.
 */
struct note_data
{
    NOTE_DATA *next;
    char *sender;
    char *csender;
    char *date;
    char *to_list;
    char *subject;
    char *text;
    time_t date_stamp;
};



/*
 * An affect.
 */
struct affect_data
{
    AFFECT_DATA *next;
    int type;
    int duration;
    int location;
    int modifier;
    long long int bitvector;
    bool deleted;
};

#ifdef HAS_ROOM_AFFECT
struct r_affect_data
{
    R_AFFECT_DATA *next;
    R_AFFECT_DATA *next_in_list;
    R_AFFECT_DATA *prev_in_list;
    ROOM_INDEX_DATA *owner;
    int type;
    int duration;
    int modifier;
    long long int bitvector;
    bool deleted;
};
#endif


/*
 * A kill structure (indexed by level).
 */
struct kill_data
{
    int number;/*total number of mobs of this level*/
    int killed;/*total number of mobs of this level being killed*/
};

/*
 * A quest mark structure
 */
struct quest_mark_data
{
    CHAR_DATA *owner;
    QMARK *prev_in_list;
    QMARK *next_in_list;
    QMARK *prev;
    QMARK *next;
    char *mark_name;
    int timer;
    int v0;
    int v1;
    int v2;
    bool fDeath;
    bool fSave;
};

struct qmark_name
{
    char *name;
    QMARK_STRING *next;
};

/*
 * An eq hidden power structure
 */
struct obj_hidden_power
{
    OBJ_HP *next;
    int vnum;
    char *hp_name;
    int percent;
    int event;
    int level;
	int p_race;
	int p_align;
	int p_class;
	int p_sex;
	int l_race;
	int l_class;
	int l_sex;
	int l_align;
	int a_race;
	int a_class;
	int a_sex;
	int a_align;
	int n_sex;
	int n_align;
	int n_class;
	int n_race;
};


/*
 * mob weakness data
 * by Zcecil
 */
struct mob_weakness_data
{
    MOB_WEAKNESS *next;
    int percent;
    int damage;
    char *name;
};

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct mob_index_data
{
    MOB_INDEX_DATA *next;
    SPEC_FUN *spec_fun;
    SHOP_DATA *pShop;
    MOB_WEAKNESS *weakness;
    char *player_name;
/*    char *		    cname;*/
    char *short_descr;
    char *long_descr;
    char *description;
    int vnum;
    int count;
    int killed;
    int sex;
    int level;
    long long int act;
    long long int affected_by;
    int affected_by2;
    int alignment;
/*
    int			hitroll;		
    int			ac;		
    int			hitnodice;
    int			hitsizedice;		  
    int			hitplus;		
    int			damnodice;
    int			damsizedice;		    
    int			damplus;		
    int			       gold;		
*/
    int mod_str;
    int mod_int;
    int mod_wis;
    int mod_dex;
    int mod_con;
	int mod_size; // 砰 2022/12/23
    int mod_hitplus;	
    int mod_mana;	
    int mod_hitroll;	
    int mod_damroll;			  
    int mod_ac;		 
    int mod_svs[MAX_SAVING];		   
    int pdr;
    int mod_exp;
    int mdr;
    int pad;
    int mad;
    int apdr;
    int amdr;
    int dodge;		
    int mod_pp;
    int gold;			
    int race;
    int class;
    int special;
	int replica;//means for ACT_HORSE to know what obj to oload
    char *psk;
    char *jcl;
    MPROG_DATA *mobprogs;
    int progtypes;
    MOB_ACTION *mob_action;
    AREA_DATA *area;		/* OLC */
    bool fAutoset;	//Added by Razgriz	20050720
};


struct symbol_data //from is82219@cis.nctu.edu.tw
{
    SYMBOL_DATA *next;
    char *str;
    long long int num;
};

struct group_data
{
    char *name;
    CHAR_DATA *leader;
    CHAR_DATA *members;
    GROUP_DATA *next;
};

/*
 * One character (PC or NPC).
 */
struct char_data
{
    CHAR_DATA *next;
    CHAR_DATA *next_in_room;
    CHAR_DATA *master;
    CHAR_DATA *leader;
    CHAR_DATA *fighting;
    CHAR_DATA *hunting;
    CHAR_DATA *hunted;
    CHAR_DATA *next_in_hunt;
    CHAR_DATA *spec_target;
    CHAR_DATA *assassinating;
    CHAR_DATA *assassinated;
	CHAR_DATA *cooperating; /* cooperate for skill/spell  2021/11/20 */
	CHAR_DATA *cooperator;
	CHAR_DATA *next_in_coop;
	CHAR_DATA *linking; /* link two chars for some skill/spell 2021/11/20 */
    CHAR_DATA *next_in_ass;
    CHAR_DATA *tracking;
    CHAR_DATA *tracked;
    CHAR_DATA *next_in_track;
    CHAR_DATA *protector;
    CHAR_DATA *next_in_protect;
    CHAR_DATA *protecting;
    CHAR_DATA *reply;
    CHAR_DATA *attacked; /* rember who is killer */
    CHAR_DATA *temp_char;
    CHAR_DATA *rembch;	/* rember someone */
    CHAR_DATA *rembed;	/* be rembered by some mob */
    CHAR_DATA *next_in_rember;
    CHAR_DATA *next_in_group;
    GROUP_DATA *group;
    ROOM_INDEX_DATA *rrembed;	/* be rembered by room */
    OBJ_DATA *orembed;
    SPEC_FUN *spec_fun;
    MOB_INDEX_DATA *pIndexData;
    DESCRIPTOR_DATA *desc;
    AFFECT_DATA *affected;
    NOTE_DATA *pnote;
    OBJ_DATA *carrying;
    OBJ_DATA *eqbank;
    BANK_DATA *neweqbank;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *was_in_room;
    PC_DATA *pcdata;
    QMARK *qmark;
    CLAN_DATA *clan;
    char *clan_channel;
    char *tell_channel;
    char *clan_join;
    MOB_WEAKNESS *weakness;
    char *psk;
    char *jcl;
    char *name;
    char *cname;
    char *short_descr;
    char *long_descr;
    char *description;
    char *stun_desc;
    // char *		attacked;
    int sex;
    int class;
    int race;
    int temp_race;
    int level;
    int trust;
    int played;
    int perm_str;
    int perm_int;
    int perm_wis;
    int perm_dex;
    int perm_con;
    int mod_str;
    int mod_int;
    int mod_wis;
    int mod_dex;
    int mod_con;
	int mod_size; // 砰 2022/12/23
    time_t logon;
    time_t save_time;
    time_t last_note;
	int stay_time; // ﹚┬丁氨痙丁 2023/01/15
    int timer;
    int wait;
    int hit;
    int max_hit;
    int mana;
    int max_mana;
    int move;
    int max_move;
    int gold;
    int bank;
    int exp;
    long long int act;
    int affected_by2;
    long long int affected_by;
    int summoned;
    int charmed;
    int position;
    int practice;
    int carry_weight;
    int carry_number;
    int saving_throw [ MAX_SAVING ];
    int alignment;
    int hitroll;
    int damroll;
    int armor;
    int dodge;
    int pp;
    int mdr; //magic damage reduced
    int pdr; //physical damage reduced
    int amdr; //add magic damage
    int apdr; //add physical damage
    int pad; // physical gate damage
    int mad; // magical gate damage
    int wimpy;
    int deaf;
    int special;
    bool deleted;
    bool ordered;
    bool forced;
    MPROG_ACT_LIST *mpact;
    MOB_ACTION *mob_action;
    int mpactnum;
    int track_times;
    CHAR_DATA *gater;		/* remember who gate him 	Added by Razgriz	20050701*/
    time_t time_stamp;	/* used in stop_rember() to prevent infinite loop	Added by Razgriz	20050811*/
};



/*
 * Data which only PC's have.
 */
struct pc_data
{
    PC_DATA *next;
    CHAR_DATA *horse;
	BATTLE_ARRAY *ba;
    char *pwd;
    char *bamfin;
    char *bamfout;
    char *immskll;
    char *title;
    char *prompt;
    char *clantitle;
    char *spouse;
    int ride;//1:has horse, 2:ride
    int rank;
    int prime_attr;
    int snd_attr;
    int condition	 [ 3 ];
    int pagelen;
    int respect;
    int skexp_earned;
    int skexp_used;
    int skexp_earned_max;
    int learned[ MAX_SKILL ];
    int clanlevel;
    int clanstatistics;
    int clanexp;
    int clanpk;
    int pkwin;
    int pklose;
    bool switched;
    int autocombat;
    int security;	/* OLC - Builder security */
    int mprog_edit;	  /* MOBProg currently editing.	 Walker */    
    int version;
    int merit;
    int merit_spent;
    time_t denied;
    time_t freezed;
};


/*
 * Liquids.
 */
#define LIQ_WATER	 0
#define LIQ_MAX		       30

struct liq_type
{
    char *liq_name;
    char *liq_ename;
    char *liq_color;
    int liq_affect [ 3 ];
};



/*
 * Extra description data for a room or object.
 */
struct extra_descr_data
{
    EXTRA_DESCR_DATA *next;	      /* Next in list			  */
    char *keyword;	   /* Keyword in look/examine	       */
    char *description; /* What to see		       */
    bool deleted;
};



/*
 * Prototype for an object.
 */
struct obj_index_data
{
    OBJ_INDEX_DATA *next;
    EXTRA_DESCR_DATA *extra_descr;
    AFFECT_DATA *affected;
    OBJ_HP *hp;
    OPROG_DATA *objprogs;
	// SPECIAL_AFFECT_DATA *			special_affected;
    char *name;
    // char *cname;
    char *short_descr;
    char *description;
    int vnum;
    int progtypes;
    int item_type;
    int extra_flags;
    int wear_flags;
    int count;
    int replica;
    int horse;
    int weight;
    int level;
    int max_level;
    int cost;
    int timer;
    int class;
	int hp_use_race;
	int hp_use_class;
	int hp_use_sex;
	int hp_use_align;
	long long int hp_affect_race;
	int hp_affect_class;
	int hp_affect_sex;
	int hp_affect_align;
	long long int hp_noeffect_race;
	int souvenir;
    int race;
    int value[ 4 ];
    char *on_message;
    char *hit_message;
    char *off_message;
    OBJ_VRACE *vrace;
    OBJ_INDEX_DATA *material;
    AREA_DATA *area;		/* OLC */
};

/*
 * Record of special affects
 * for race/class/sex/alignment.
 */
struct special_affect_data
{
	AFFECT_DATA *next;
    int type;
    int duration;
    int location;
    int modifier;
    long long int bitvector;
    bool deleted;
};

/* Added by Razgriz 20050918
 * Object's extra value
 */
struct obj_extra_value
{
    bool used;
    int v[2];	//v0 v1
    int t[2];	//timer
};

/*
 * One object.
 */
struct obj_data
{
    OBJ_DATA *next; /*obj global link-list*/
    OBJ_DATA *next_content; /*obj link-list in the current container, like room/char,etc.*/
    OBJ_DATA *contains;
    OBJ_DATA *in_obj;
    CHAR_DATA *carried_by;
    CHAR_DATA *orembch;
    OBJ_DATA *next_in_orember;
    EXTRA_DESCR_DATA *extra_descr;
    AFFECT_DATA *affected;
    OBJ_INDEX_DATA *pIndexData;
    ROOM_INDEX_DATA *in_room;
    OBJ_HP *hp;
    OBJ_VRACE *vrace;
    OBJ_INDEX_DATA *material;
    char *name;
/*    char *		    cname;*/
    char *short_descr;
    char *description;
    int in_eqbank;
    int item_type;
    int extra_flags;
    int wear_flags;
    int wear_loc;
    int weight;
    int cost;
    int level;
    int timer;
    int class;
    int race;
    int value[5];
    bool deleted;
    bool update_checked; 	//Added by Razgriz	20050625
    OBJ_EXTRA_VALUE	extra_value;	/* Razgriz 20050918 */
    time_t serial;
    
};



/*
 * Exit data.
 */
struct exit_data
{
    ROOM_INDEX_DATA *to_room;
    int vnum;
    int exit_info;
    int key;
	int exit_size; // 2022/12/22
    char *keyword;
    char *description;
    EXIT_DATA *next;		/* OLC */
    int rs_flags;	/* OLC */
    int orig_door;	/* OLC */
};



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct reset_data
{
    RESET_DATA *next;
    char command;
    int arg1;
    int arg2;
    int arg3;
};



/*
 * Area definition.
 */
struct area_data
{
    AREA_DATA *next;
    CLAN_DATA *clan;
	WEATHER_DATA *weather;
/*
    RESET_DATA *	reset_first;
    RESET_DATA *	reset_last;
*/
    char *name;
    char *identity;
    int recall;
    int death;
    int age;
    int nplayer;
    char *filename;	/* OLC */
    char *builders;	/* OLC - Listing of builders */
    int security;	/* OLC - Value 0-infinity  */
    int lvnum;		/* OLC - Lower vnum */
    int uvnum;		/* OLC - Upper vnum */
    int vnum;		/* OLC - Area vnum  */
    int area_flags;	/* OLC */
    char *translator;
    char *modifier;
    int f_toggle;	/* room flag toggle bits */
    int reborn;		//add by coconet
};



/*
 * Room type.
 */
struct room_index_data
{
    ROOM_INDEX_DATA *next;
    CHAR_DATA *people;
    CHAR_DATA *rrembch;
    OBJ_DATA *contents;
    EXTRA_DESCR_DATA *extra_descr;
    AREA_DATA *area;
    EXIT_DATA *exit[ 6 ];
    ROOM_INDEX_DATA *next_in_rrember;
    char *name;
    char *description;
    int vnum;
	int fall;
	int max_char; // 2022/12/17
	int room_size; // 2022/12/20
	int room_space; // 2022/12/20
    int room_flags;
    int light;
    int sector_type;
#ifdef HAS_ROOM_AFFECT
    R_AFFECT_DATA *affected;
    long long int affected_by;
#endif
    RPROG_DATA *roomprogs;
    int progtypes;
    RESET_DATA *reset_first;	/* OLC */
    RESET_DATA *reset_last;	/* OLC */
	char *temp_description;
};


struct mob_action_data
{
    MOB_ACTION *next_in_list;
    MOB_ACTION *prev_in_list;
    MOB_ACTION *next;
    CHAR_DATA *owner;
    char *com_list;
    char *old_com_list;
    int mode;
    int start_time;
};

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED		   -1
#define TYPE_HIT		 1000

/*
 * Added by Razgriz 20051014
 * - ㄏノ mone_hit() 把计い, ㄓ跋琌璶㊣ dam_message()
 * - ㄏノ damage()   把计い, ㄓ跋琌璶㊣ dam_message()
 */
#define MSG_NORMAL	0
#define MSG_BRIEF	1

/*
 * Added by Razgriz 20051014
 * ㄏノ damage() 把计い, ノㄓ跋 端甡块Τぃ耞:
 * - м琌砆 parry dodge block statics
 * - 端甡琌礚跌竧, 礚跌ぱㄏ臔砃
 */
#define DMG_NORMAL	0
#define DMG_ADJUST	1

/*
 * Added by Razgriz 20051014
 * ㄏノ mone_hit() 把计い, ∕﹚琌璶э跑㊣狠 bt 
 */
#define DT_NOCHANGE	0
#define DT_CHANGE	1

/*
 *  Target types.
 */
#define TAR_IGNORE		      0
#define TAR_CHAR_OFFENSIVE	      1
#define TAR_CHAR_DEFENSIVE	      2
#define TAR_CHAR_SELF			 3
#define TAR_OBJ_INV		       4
#define TAR_OBJ			    5
#define TAR_ARG				6



/*
 * Skills include spells as a particular case.
 */
struct skill_type
{
    char *name;				  /* Name of skill		   */
    char *cname;
    int skill_level[ MAX_CLASS ]; /* Level needed by class	    */
    SPELL_FUN *spell_fun;		    /* Spell pointer (for spells) */
    int target;				  /* Legal targets		   */
    int minimum_position;	    /* Position for caster / user */
    int *pgsn;				 /* Pointer to associated gsn	      */
    int min_mana;		    /* Minimum mana used		 */
    int beats;				 /* Waiting time after use	   */
    char *noun_damage;			 /* Damage message		   */
    char *msg_off;		     /* Wear off message		 */
    int require_attr;
    int apply_attr[5];              /*  For skillexp five attr*/
    //int		type;
};

/*
 * Top 10 rank data
 */
struct top10_data
{
    char *id;
    int value;
};

/*
 * Battle array commands for LORD.
 */
struct battle_array
{
	BATTLE_ARRAY *next;
    char *ba11;
    char *ba12;
    char *ba13;
    char *ba21;
    char *ba22;
    char *ba23;
    char *ba31;
    char *ba32;
    char *ba33;
};

struct clan_war_data
{
	int fighting;
	int clan_a_num;
	int clan_b_num;
	CLAN_DATA *clan_a;
	CLAN_DATA *clan_b;
	CHAR_DATA *clan_a_member[11];
	CHAR_DATA *clan_b_member[11];
};

#define ALLOW_BIT			1048576
#define	DENY_BIT			2097152
#define NEW_BIT				4194304

#define PRIME_STR   BV00
#define PRIME_DEX   BV01
#define PRIME_INT   BV02
#define PRIME_WIS   BV03
#define PRIME_CON   BV04
#define PRIME_ALL   (BV05 - 1)
#define SECOND_STR  BV05
#define SECOND_DEX  BV06
#define SECOND_INT  BV07
#define SECOND_WIS  BV08
#define SECOND_CON  BV09
#define SECOND_ALL (BV10-BV05)
#define ATTR_ALL    (BV10-1)

/*
 * Structure for a command in the command lookup table.
 */
struct cmd_type
{
    char *const name;
    DO_FUN *do_fun;
    int position;
    int level;
    int log;
    bool toPPL;
};


/*
 * Structure for a room command in the command lookup table.
 */
struct rcmd_type
{
    char *const name;
    DO_RFUN *do_rfun;
};


struct ocmd_type
{
    char *const name;
    DO_OFUN *do_ofun;
};

/*
 * Structure for a social in the socials table.
 */
struct social_type
{
    char *const name;
    char *const char_no_arg;
    char *const others_no_arg;
    char *const char_found;
    char *const others_found;
    char *const vict_found;
    char *const char_auto;
    char *const others_auto;
};



struct mob_prog_act_list
{
    MPROG_ACT_LIST *next;
    char *buf;
    CHAR_DATA *ch;
    OBJ_DATA *obj;
    void *vo;
};

struct mob_prog_data
{
    MPROG_DATA *next;
    int type;
    char *arglist;
    char *comlist;
};

bool MOBtrigger;

struct obj_prog_data
{
    OPROG_DATA *next;
    OPROG_DATA *next_in_list;
    int type;
    char *arglist;
    char *comlist;
    OBJ_INDEX_DATA *owner;
};

struct room_prog_data
{
    RPROG_DATA *next;
    RPROG_DATA *next_in_list;
    ROOM_INDEX_DATA *owner;
    int type;
    char *arglist;
    char *comlist;
    int v0;//these parameters should belong to room, however,
    int v1;//to save the memory space, i put them in rprog_data.
    int timer[2];//cause only few rooms have rprog.....
};
/*****************************************************************************
 *				      OLC				     *
 *****************************************************************************/

/*
 * This structure is used in special.c to lookup spec funcs and
 * also in olc_act.c to display listings of spec funcs.
 */
struct spec_type
{
    char *spec_name;
    SPEC_FUN *spec_fun;
};



/*
 * This structure is used in bit.c to lookup flags and stats.
 */
struct flag_type
{
    char *name;
    long long int bit;
    bool settable;
};

#define ERROR_PROG		  -1
#define IN_FILE_PROG		    0
#define ACT_PROG		1
#define SPEECH_PROG		   2
#define RAND_PROG		 4
#define FIGHT_PROG		  8
#define DEATH_PROG		  16
#define HITPRCNT_PROG		     32
#define ENTRY_PROG		  64
#define GREET_PROG		  128
#define ALL_GREET_PROG		      256
#define GIVE_PROG		 512
#define BRIBE_PROG		  1024
#define TALK_PROG		  2048
#define TICK_PROG		 4096
#define REPOP_PROG		8192
#define DEFUN_PROG		16384
#define HURT_PROG               32768
#define KILL_PROG               65536
#define ENTRY_GREET_PROG        131072  // add at 2021/12/18

#define ROOM_LEAVE      BV00
#define ROOM_ENTER      BV01
#define ROOM_RAND       BV02
#define ROOM_TIME       BV03 
#define ROOM_COMMAND    BV04	
#define ROOM_TRAP       BV05
#define ROOM_DEFUN      BV06
#define ROOM_MASK       BV07

#define OBJ_ACT         BV00
#define OBJ_BREAK	    BV01
#define OBJ_RAND	    BV02
#define OBJ_HIT		    BV03
#define OBJ_REPOP	    BV04
#define OBJ_BLOCK	    BV05
#define OBJ_FIGHT	    BV06
#define OBJ_DEFUN	    BV07
#define OBJ_LOAD        BV08
#define OBJ_DISAPPEAR   BV09
#define OBJ_COMMAND     BV10
#define OBJ_TICK        BV11
#define OBJ_KILL        BV12 // add at 2022/05/08
/*
 * Area flags.
 */
#define		AREA_NONE	0
#define		AREA_CHANGED	1	/* Area has been modified. */
#define		AREA_ADDED	2	/* Area has been added to. */
#define		AREA_LOADING	4	/* Used for counting in db.c */
#define		       AREA_VERBOSE	   8	    /* verbose */
#define		AREA_BUILDING	16
#define		AREA_CLAN   32



#define MAX_DIR 6
#define NO_FLAG -99LL	/* Must not be used in flags or stats. */



/*
 * Global constants.
 */
extern        const    int     inborn_ability_cost[RACE_ABILITY_COUNT];
extern	      const	   struct	 str_app_type	     str_app		    [ 32 ];
extern	      const	   struct	 int_app_type	     int_app		    [ 31 ];
extern	      const	   struct	 wis_app_type	     wis_app		    [ 31 ];
extern	      const	   struct	 dex_app_type	     dex_app		    [ 31 ];
extern	      const	   struct	 con_app_type	     con_app		    [ 31 ];

extern	      const	   struct	 class_type	   class_table	      [ MAX_CLASS   ];
extern	      const	   int		 apply_class	[6];
extern	      const	   struct	 cmd_type	 cmd_table	  [ ];
extern	      const	   struct	 rcmd_type	 rcmd_table	  [ ];
extern	      const	   struct	 liq_type	 liq_table	  [ LIQ_MAX	];
extern	      const	   struct	 skill_type	   skill_table	      [ MAX_SKILL   ];
extern	      const	   struct	 social_type	    social_table	[ ];
extern	      char *	    const			 title_table	    [ MAX_CLASS	  ]
							[ MAX_LEVEL+1 ]
							[ 2 ];
extern	      char *	    const			 c_title_table	    [ MAX_CLASS	  ]
							[ MAX_LEVEL+1 ]
							[ 2 ];
extern	const	struct	race_type	race_table	[ MAX_RACE ];
extern	const	struct	struckdrunk	drunk		[ ];

extern	char *	const	dir_name	[];
extern	const	int	rev_dir		[];
extern	const	struct	spec_type	spec_table	[];
extern	DO_FUN *    yn_dofun[2];



/*
 * Global variables.
 */
extern ASKILL_DATA *askill_list;
extern HELP_DATA *help_first;
extern SHOP_DATA *shop_first;

extern BAN_DATA *ban_list;
extern CHAR_DATA *char_list;
extern DESCRIPTOR_DATA *descriptor_list;
extern NOTE_DATA *note_list;
extern OBJ_DATA *object_list;
extern BANK_DATA *eqbank_list;
extern BANK_DATA *eqbank_free;
extern OBJ_INDEX_DATA *material_list;

extern AFFECT_DATA *affect_free;
#ifdef HAS_ROOM_AFFECT
typedef struct r_affect_data R_AFFECT_DATA;
extern R_AFFECT_DATA *r_affect_free;
extern R_AFFECT_DATA *r_affect_list;
#endif
extern QMARK *qmark_list;
extern GROUP_DATA *group_list;
extern GROUP_DATA *group_free;
extern QMARK *qmark_free;
extern QMARK_STRING *qmark_string_list;
extern MOB_ACTION *mob_action_list;
extern MOB_ACTION *mob_action_free;
extern OBJ_HP *obj_hp_free;
extern RPROG_DATA *rprg_list;
extern OPROG_DATA *oprg_list;
extern MOB_ACTION *mob_action_list;
extern BATTLE_ARRAY *ba_free;
extern BAN_DATA *ban_free;
extern CHAR_DATA *char_free;
extern DESCRIPTOR_DATA *descriptor_free;
extern EXTRA_DESCR_DATA *extra_descr_free;
extern NOTE_DATA *note_free;
extern OBJ_DATA *obj_free;
extern PC_DATA *pcdata_free;
extern CLAN_DATA *clan_list;
extern CLAN_DATA *clan_free;
extern CLAN_WAR_DATA *clan_war;
extern char bug_buf[ ];
extern time_t current_time;
extern bool fLogAll;
extern FILE *fpReserve;
extern FILE *fpBug;
extern KILL_DATA kill_table[ ];
extern char log_buf[ ];
extern char pipe_buf[ ];
extern char read_pipe_buf[ ];
extern char *read_pipe_ptr;
extern char name_buf[ ];
extern char drunkwords[ 255 ];
extern int fd0[];
extern int fd1[];
extern TIME_INFO_DATA time_info;
extern WEATHER_DATA weather_info;
extern time_t down_time;
extern time_t warning1;
extern time_t warning2;
extern bool Reboot;

extern AREA_DATA *area_first;
extern AREA_DATA *area_last;
extern SHOP_DATA *shop_last;

extern int top_affect;
extern int top_area;
extern int top_ed;
extern int top_exit;
extern int top_help;
extern int top_mob_index;
extern int top_obj_index;
extern int top_reset;
extern int top_room;
extern int top_shop;
extern int top_qmark;
extern int top_hiddenpower;
extern int top_errmem;

extern int top_vnum_mob;
extern int top_vnum_obj;
extern int top_vnum_room;
extern int MAX_PPL;
extern char str_empty[1];

extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern SYMBOL_DATA *symbol_hash[26];
extern int mpact[24];
extern TOP10_DATA tmudage[TOP_COUNT + 1], texp[TOP_COUNT + 1], tgold[TOP_COUNT + 1], thp[TOP_COUNT + 1], t_mp[TOP_COUNT + 1], tmv[TOP_COUNT + 1], tpk[TOP_COUNT + 1], tresp[TOP_COUNT + 1], tmerit[TOP_COUNT + 1];
extern AUCTION_DATA *auction_list, *auc_free;
extern CALL_OUT_DATA *call_out_list, *call_out_free;
extern RCALL_OUT_DATA *rcall_out_list, *rcall_out_free;
#define MAX_TYPES 15
extern OBJ_INDEX_DATA *basic_types[MAX_TYPES];

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN(	       do_adore		 );		/* by Amenda */
DECLARE_DO_FUN(		do_repair);  /* zcecil */
DECLARE_DO_FUN(         do_personal_recover); /* Keric */
DECLARE_DO_FUN(		do_unbackup);  /* zcecil */
DECLARE_DO_FUN(		do_perform);  /* zcecil */
DECLARE_DO_FUN(		do_skdemote);  /* zcecil */
DECLARE_DO_FUN(		do_skadv);  /* zcecil */
DECLARE_DO_FUN(	       do_advance	 );
DECLARE_DO_FUN(	       do_opstat ); /* zcecil */
DECLARE_DO_FUN(	       do_eqbank);  /* zcecil */
DECLARE_DO_FUN(	       do_skilldrv); /* zcecil */
DECLARE_DO_FUN(	       do_mtlist);/* zcecil */
DECLARE_DO_FUN(	       do_mtstat);/* zcecil */
DECLARE_DO_FUN(	       do_mtset);/* zcecil */
DECLARE_DO_FUN(	       do_afk	       );
DECLARE_DO_FUN(	       do_dice	       ); /* zcecil */
DECLARE_DO_FUN(	       do_allow	       );
DECLARE_DO_FUN(	       do_medi);	/* zcecil */
DECLARE_DO_FUN(	       do_meditate);	/* zcecil */
DECLARE_DO_FUN(	       do_adv);	/* zcecil */
DECLARE_DO_FUN(	       do_answer	);
DECLARE_DO_FUN(	       do_areas	       );
DECLARE_DO_FUN(	       do_at		    );
DECLARE_DO_FUN(	       do_auction	 );
DECLARE_DO_FUN( do_auto		);
DECLARE_DO_FUN( do_autocombat		);
DECLARE_DO_FUN( do_autoexit	);
DECLARE_DO_FUN( do_autoloot	);
DECLARE_DO_FUN( do_autogold	);
DECLARE_DO_FUN( do_autosac	);
DECLARE_DO_FUN(	       do_battle_array );	/* by Amenda */
DECLARE_DO_FUN(	       do_backstab	  );
DECLARE_DO_FUN(	       do_bamfin	);
DECLARE_DO_FUN(	       do_bamfout	 );
DECLARE_DO_FUN(	       do_bandage);
DECLARE_DO_FUN(	       do_ban		     );
DECLARE_DO_FUN(	       do_bash	       );
DECLARE_DO_FUN(	       do_beep	       );
DECLARE_DO_FUN(	       do_berserk	  );		    /* by Thelonius */
DECLARE_DO_FUN(	       do_bet	       );		 /* by Thelonius */
DECLARE_DO_FUN(	       do_backup       );		 /* by Amenda */
DECLARE_DO_FUN( do_blank	);
DECLARE_DO_FUN(	       do_brandish	  );
DECLARE_DO_FUN( do_brief	);
DECLARE_DO_FUN(	       do_bug		     );
DECLARE_DO_FUN(	       do_buy		     );
DECLARE_DO_FUN(	       do_cast		      );
DECLARE_DO_FUN(	       delayed_do_cast		      );
DECLARE_DO_FUN(	       do_cast_delay);
DECLARE_DO_FUN( do_chameleon	);		  /* by Thelonius */
DECLARE_DO_FUN(	       do_channels	  );
DECLARE_DO_FUN(	       do_charge	  );	/* by Amenda */
DECLARE_DO_FUN(	       do_chat		      );
DECLARE_DO_FUN(	       do_circle	 );		   /* by Thelonius */
DECLARE_DO_FUN(	       do_circular	 );		   /* by Amenda */
DECLARE_DO_FUN(	       do_clan	      );	/* by Amenda */
DECLARE_DO_FUN(	       do_clanchat	      );	/* by Amenda */
DECLARE_DO_FUN(	       do_close	       );
DECLARE_DO_FUN(	       do_cname	       );
DECLARE_DO_FUN(	       do_hname	       );
DECLARE_DO_FUN(	       do_hsummon);
DECLARE_DO_FUN( do_combine	);
DECLARE_DO_FUN(	       do_commands	  );
DECLARE_DO_FUN(	       do_compare	 );
DECLARE_DO_FUN(	       do_config	);
DECLARE_DO_FUN(	       do_consent	); // zcecil
DECLARE_DO_FUN(	       do_consider	  );
DECLARE_DO_FUN(	       do_credits	 );
DECLARE_DO_FUN(	       do_deny		      );
DECLARE_DO_FUN(	       do_description	     );
DECLARE_DO_FUN(	       do_disarm	);
DECLARE_DO_FUN(	       do_barehand	);		/* by Amenda */
DECLARE_DO_FUN(	       do_disconnect	    );
DECLARE_DO_FUN(	       do_donate	);
DECLARE_DO_FUN(	       do_down		      );
DECLARE_DO_FUN(	       do_drink	       );
DECLARE_DO_FUN(	       do_drop		      );
DECLARE_DO_FUN(        do_destro ); /*by coconet*/
DECLARE_DO_FUN(	       do_destroy );
DECLARE_DO_FUN(	       do_earth		      );	/* by Amenda */
DECLARE_DO_FUN(	       do_east		      );
DECLARE_DO_FUN(	       do_eat		     );
DECLARE_DO_FUN(	       do_echo		      );
DECLARE_DO_FUN(	       do_elemental); /*by keelar */
DECLARE_DO_FUN(	       do_emote	       );
DECLARE_DO_FUN(	       do_equipment	   );
DECLARE_DO_FUN(	       do_examine	 );
DECLARE_DO_FUN(	       do_exits	       );
DECLARE_DO_FUN(	       do_fee		     );
DECLARE_DO_FUN(	       do_feed		      );
DECLARE_DO_FUN(	       do_fill		      );
DECLARE_DO_FUN(	       do_flaming	      );	/* by Amenda */
DECLARE_DO_FUN(	       do_storm	      );	/* by Pluto */
DECLARE_DO_FUN(	       do_stone_form	);	/* by Keelar */
DECLARE_DO_FUN(	       do_sprinkle	      );	/* by Pluto */
DECLARE_DO_FUN(	       do_icecore);/*by coconet*/
DECLARE_DO_FUN(	       do_fulgurant); /*by coconet */
DECLARE_DO_FUN(	       do_flee		      );
DECLARE_DO_FUN(	       do_follow );
DECLARE_DO_FUN(	       do_force	 );
DECLARE_DO_FUN(	       do_forceo );	/* Razgriz 20050916 */
DECLARE_DO_FUN(	       do_forcer );
DECLARE_DO_FUN(	       do_addexp );
DECLARE_DO_FUN(	       do_freeze );
DECLARE_DO_FUN(	       do_get	 );
DECLARE_DO_FUN(	       do_get_silence		     );
DECLARE_DO_FUN(	       do_give		      );
DECLARE_DO_FUN(	       do_goto		      );
DECLARE_DO_FUN(	       do_group	       );
DECLARE_DO_FUN(	       do_gtell	       );
DECLARE_DO_FUN( do_heighten	);		  /* by Thelonius */
DECLARE_DO_FUN(	       do_help		      );
DECLARE_DO_FUN(	       do_hide		      );
DECLARE_DO_FUN(	       do_holylight	   );
DECLARE_DO_FUN(	       do_idea		      );
DECLARE_DO_FUN(	       do_immtalk	 );
DECLARE_DO_FUN(        do_check_bit	 );  // by Amethyst
DECLARE_DO_FUN(	       do_herotalk);
DECLARE_DO_FUN(	       do_imtlset	 );
DECLARE_DO_FUN(	       do_inventory	   );
DECLARE_DO_FUN(	       do_invis	       );
DECLARE_DO_FUN(	       do_kick		      );
DECLARE_DO_FUN(	       do_hunt		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_ass		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_scan		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_frenzy	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_bashhead	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_abstraction	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_ammo             ); // add at 2022/04/30
DECLARE_DO_FUN(	       do_power	      	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_reflexion	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_combat_howl	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_track		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_invoke	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_darkraid	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_stare		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_deposit	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_withdraw	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_store		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_receive	      ); /* by Amenda */
DECLARE_DO_FUN(	       do_scout		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_kill		      );
DECLARE_DO_FUN(	       do_throw             );
DECLARE_DO_FUN(	       do_lead		      ); /* by Amenda */
DECLARE_DO_FUN(	       do_list		      );
DECLARE_DO_FUN(	       do_lock		      );
DECLARE_DO_FUN(	       do_log		     );
DECLARE_DO_FUN(	       do_look		      );
DECLARE_DO_FUN(	       do_memory	);
DECLARE_DO_FUN(	       do_meteoric_arrow );	/* by Amenda */
DECLARE_DO_FUN(	       do_rainfall_arrow );	// add at 2022/05/03
DECLARE_DO_FUN(	       do_mfind	       );
DECLARE_DO_FUN(	       do_mload	       );
DECLARE_DO_FUN(	       do_mset		      );
DECLARE_DO_FUN(	       do_mstat	       );
DECLARE_DO_FUN(	       do_cras);
DECLARE_DO_FUN(	       do_crash);
DECLARE_DO_FUN(	       do_idlequit		      );
DECLARE_DO_FUN(	       do_mwhere	);
DECLARE_DO_FUN(	       do_murder	);
DECLARE_DO_FUN(	       do_music	       );
DECLARE_DO_FUN(	       do_newloc	 );
DECLARE_DO_FUN(	       do_newlock	 );
DECLARE_DO_FUN(	       do_noemote	 );
DECLARE_DO_FUN(	       do_north	       );
DECLARE_DO_FUN(	       do_note		      );
DECLARE_DO_FUN(	       do_notell	);
DECLARE_DO_FUN(	       do_numloc	 );
DECLARE_DO_FUN(	       do_numlock	 );
DECLARE_DO_FUN(	       do_ofind	       );
DECLARE_DO_FUN(	       do_oload	       );
DECLARE_DO_FUN(	       do_open		      );
DECLARE_DO_FUN(	       do_order	       );
DECLARE_DO_FUN(	       do_oset		      );
DECLARE_DO_FUN(	       do_ostat	       );
DECLARE_DO_FUN(	       do_owhere	);
DECLARE_DO_FUN( do_pagelen	);
DECLARE_DO_FUN(	       do_pardon	);
DECLARE_DO_FUN(	       do_password	  );
DECLARE_DO_FUN(	       do_peace	       );
DECLARE_DO_FUN(	       do_pick		      );
DECLARE_DO_FUN(	       do_poison_weapon);		 /* by Thelonius */
DECLARE_DO_FUN(	       do_pose		      );
DECLARE_DO_FUN(	       do_practice	  );
DECLARE_DO_FUN(	       do_cooperate	  );        /* 2021/11/20 */
DECLARE_DO_FUN(	       do_protect	  );		/* by Amenda */
DECLARE_DO_FUN(	       do_join	  );			/* by Amenda */
DECLARE_DO_FUN( do_prompt	);
DECLARE_DO_FUN(	       do_purge	       );
DECLARE_DO_FUN(	       do_put		     );
DECLARE_DO_FUN(	       do_quaff	       );
DECLARE_DO_FUN(	       do_question	  );
DECLARE_DO_FUN(	       do_qui		     );
DECLARE_DO_FUN(	       do_quit		      );
DECLARE_DO_FUN(	       do_reboo	       );
DECLARE_DO_FUN(	       do_reboot	);
DECLARE_DO_FUN(	       do_recall	);
DECLARE_DO_FUN(	       do_recho	       );
DECLARE_DO_FUN(	       do_recite	);
DECLARE_DO_FUN(	       do_register	  );
DECLARE_DO_FUN(	       do_remove	);
DECLARE_DO_FUN(	       do_rent		      );
DECLARE_DO_FUN(	       do_reply	       );
DECLARE_DO_FUN(	       do_report	);
DECLARE_DO_FUN(	       do_rescue	);
DECLARE_DO_FUN(	       do_change	);		/* by Amenda */
DECLARE_DO_FUN(	       do_rest		      );
DECLARE_DO_FUN(	       do_restore	 );
DECLARE_DO_FUN(	       do_return	);
DECLARE_DO_FUN(	       do_ride	);	/* by Amenda */
DECLARE_DO_FUN(	       do_rset		      );
DECLARE_DO_FUN(	       do_rstat	       );
DECLARE_DO_FUN(	       do_sacrifice	   );
DECLARE_DO_FUN(	       do_save		      );
DECLARE_DO_FUN(	       do_silentsave		      );
DECLARE_DO_FUN(	       do_say		     );
DECLARE_DO_FUN(	       do_score	       );
DECLARE_DO_FUN(	       do_elescore	       );
DECLARE_DO_FUN(	       do_sell		      );
DECLARE_DO_FUN( do_shadow	);		  /* by Thelonius */
DECLARE_DO_FUN(	       do_shout	       );
DECLARE_DO_FUN(	       do_shutdow	 );
DECLARE_DO_FUN(	       do_shutdown	  );
DECLARE_DO_FUN(	       do_silence	 );
DECLARE_DO_FUN(	       do_sla		     );
DECLARE_DO_FUN(	       do_slay		      );
DECLARE_DO_FUN(	       do_sleep	       );
DECLARE_DO_FUN( do_slist	);
DECLARE_DO_FUN(	       do_slookup	 );
DECLARE_DO_FUN(	       do_snare	       );		 /* by Binky / Thelonius */
DECLARE_DO_FUN(	       do_sneak	       );
DECLARE_DO_FUN(	       do_nightstalk       );		/* by Amenda */
DECLARE_DO_FUN(	       do_snoop	       );
DECLARE_DO_FUN(	       do_socials	 );
DECLARE_DO_FUN(	       do_south	       );
DECLARE_DO_FUN( do_spells	);
DECLARE_DO_FUN(	       do_split	       );
DECLARE_DO_FUN(	       do_sset		      );
DECLARE_DO_FUN(	       do_sstime	);
DECLARE_DO_FUN(	       do_sstim	);
DECLARE_DO_FUN(	       do_stake	       );
DECLARE_DO_FUN(	       do_stand	       );
DECLARE_DO_FUN(	       do_steal	       );
DECLARE_DO_FUN(	       do_sun	       );
DECLARE_DO_FUN( do_suicid	 );
DECLARE_DO_FUN( do_suicide	  );
DECLARE_DO_FUN( do_hsuicid	 );
DECLARE_DO_FUN( do_hsuicide	  );
DECLARE_DO_FUN(	       do_switch	);
DECLARE_DO_FUN(	       do_tell		      );
DECLARE_DO_FUN(	       do_test		      );
DECLARE_DO_FUN(	       do_time		      );
DECLARE_DO_FUN(	       do_title	       );
DECLARE_DO_FUN(	       do_top10		);
DECLARE_DO_FUN(	       do_train	       );
DECLARE_DO_FUN(	       do_transfer	  );
DECLARE_DO_FUN(	       do_trust	       );
DECLARE_DO_FUN(	       do_typo		      );
DECLARE_DO_FUN(	       do_twin		      );	/* by Amenda */
DECLARE_DO_FUN(	       do_unlock	);
DECLARE_DO_FUN( do_untangle	);		  /* by Thelonius */
DECLARE_DO_FUN(	       do_up		    );
DECLARE_DO_FUN(	       do_users	       );
DECLARE_DO_FUN(	       do_value	       );
DECLARE_DO_FUN(	       do_visible	 );
DECLARE_DO_FUN(	       do_disperse	);//add by coconet 1118,2004
DECLARE_DO_FUN(	       do_wager		      );
DECLARE_DO_FUN(	       do_wake		      );
DECLARE_DO_FUN(	       do_wear		      );
DECLARE_DO_FUN(	       do_weather	 );
DECLARE_DO_FUN(	       do_sector	 );// add at 200201
DECLARE_DO_FUN(	       do_west		      );
DECLARE_DO_FUN(	       do_where	       );
DECLARE_DO_FUN(	       do_who		     );
DECLARE_DO_FUN(	       do_whois	       );
DECLARE_DO_FUN(	       do_wimpy	       );
DECLARE_DO_FUN(	       do_wizhelp	 );
DECLARE_DO_FUN( do_wizify	);
DECLARE_DO_FUN( do_wizlist	);
DECLARE_DO_FUN(	       do_wizlock	 );
DECLARE_DO_FUN(	       do_yell		      );
DECLARE_DO_FUN(	       do_zap		     );

DECLARE_DO_FUN( do_mpget	); //add by jye 0940913
DECLARE_DO_FUN( do_mpasound	);
DECLARE_DO_FUN( do_mplog	);
DECLARE_DO_FUN( do_mpat		);
DECLARE_DO_FUN( do_mpecho	);
DECLARE_DO_FUN( do_mpechoaround );
DECLARE_DO_FUN( do_mpechoat	);
DECLARE_DO_FUN( do_mpnonecho	);
DECLARE_DO_FUN( do_mpnonechoaround );
DECLARE_DO_FUN( do_mpnonechoat	);
DECLARE_DO_FUN( do_mpechotoall	);
DECLARE_DO_FUN( do_mpforce	);
DECLARE_DO_FUN( do_mpforceo	);	/* Razgriz 20051010 */
DECLARE_DO_FUN( do_mpforcer );
DECLARE_DO_FUN( do_mpgoto	);
DECLARE_DO_FUN( do_mpgreward); /* zcecil */
DECLARE_DO_FUN( do_mpalign);
DECLARE_DO_FUN( do_mpskillexp); /* zcecil */
DECLARE_DO_FUN( do_mpmerit); /* keelar */
DECLARE_DO_FUN( do_mpjunk	);
DECLARE_DO_FUN( do_mpkill	);
DECLARE_DO_FUN( do_mpmake_horse ); /* by Amenda */
DECLARE_DO_FUN( do_mpmload	);
DECLARE_DO_FUN( do_mpdesc	);	/* 2022/11/05 */
DECLARE_DO_FUN( do_mpmset	);	/* Razgriz 20050626 */
DECLARE_DO_FUN( do_mpnowait);
DECLARE_DO_FUN( do_mpoload	);
DECLARE_DO_FUN( do_mppurge	);
DECLARE_DO_FUN( do_mpstat	);
DECLARE_DO_FUN( do_mastat	);
DECLARE_DO_FUN( do_mpset_obj_affect	);	/* Razgriz 20050915 */
DECLARE_DO_FUN( do_mpremove_obj_affect	);	/* Razgriz 20050915 */
DECLARE_DO_FUN( do_mpmodify_obj_affect	);	/* Razgriz 20050916 */
DECLARE_DO_FUN( do_mpcall);	/* zcecil */
DECLARE_DO_FUN( do_mpcallout);	/* zcecil */
DECLARE_DO_FUN( do_rpstat );
DECLARE_DO_FUN( do_opstat );/*coconet0603*/
DECLARE_DO_FUN( do_mptransfer	);
DECLARE_DO_FUN( do_mpdef_rembch	);	/* by Amenda */
DECLARE_DO_FUN( do_mpadd_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_mpadd_all_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_add_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_del_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_mod_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_mpdel_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_mpdel_all_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_mpmod_qmark  );	/* by Amenda */
DECLARE_DO_FUN( do_mpmod_all_qmark  );  /* by Amenda*/ 
DECLARE_DO_FUN( do_talk  	);	/* by Amenda */
DECLARE_DO_FUN( do_mpgold  	);	/* by Amenda */
DECLARE_DO_FUN( do_mpexp  	);	/* by Amenda */
DECLARE_DO_FUN( do_mppractice  	);	/* add at 20181024 */
DECLARE_DO_FUN( do_mpcexp  	);	/* by Amenda */
DECLARE_DO_FUN( do_mpresp  	);	/* by Amenda */
DECLARE_DO_FUN( do_mpgroup  	);	/* by Amenda */
DECLARE_DO_FUN(	do_rumor        );	/* by Amenda */
DECLARE_DO_FUN(	do_mpdis_magic  );	/* by Amenda */
//DECLARE_DO_FUN( do_mpstun		);  /* by Amethyst */
DECLARE_DO_FUN( do_mpteleport	);  /* by Amethyst */
DECLARE_DO_FUN( do_mptrip		);  /* by Amethyst */
DECLARE_DO_FUN( do_mpmodify		);  /* by Amethyst */
DECLARE_DO_FUN( do_self_skill	);	/* by Amethyst */
DECLARE_DO_FUN( do_northeast	);	/* by Amethyst */
DECLARE_DO_FUN( do_northwest	);	/* by Amethyst */
DECLARE_DO_FUN( do_southeast	);	/* by Amethyst */
DECLARE_DO_FUN( do_southwest	);	/* by Amethyst */

/*
 * Room commands
 */
DECLARE_DO_RFUN( do_rrecho	);	/* by Amenda */
DECLARE_DO_RFUN( do_rechoat	);	/* by Amenda */
DECLARE_DO_RFUN( do_rnonechoaround	);
DECLARE_DO_RFUN( do_rnonechoat	);
DECLARE_DO_RFUN( do_rechoaround	);	/* by Amenda */
DECLARE_DO_RFUN( do_rechotoall	);	/* by Amenda */
DECLARE_DO_RFUN( do_rechotoarea	);	/* by Amenda */
DECLARE_DO_RFUN( do_modrvt	);	/* by Amenda */
DECLARE_DO_RFUN( do_modhmv	);	/* by Amenda */
DECLARE_DO_RFUN( do_modhmvgroup );/*by Coconet*/
DECLARE_DO_RFUN( do_rat		);	/* by Amenda */
DECLARE_DO_RFUN( do_rcall		);	/* by zcecil*/
DECLARE_DO_RFUN( do_rcast		);	/* by zcecil */
DECLARE_DO_RFUN( do_def_rembch	);	/* by Amenda */
DECLARE_DO_RFUN( do_roload	);	/* by Amenda */
DECLARE_DO_RFUN( do_rmload	);	/* by Amenda */
DECLARE_DO_RFUN( do_add_affect	);	/* by Amenda */
DECLARE_DO_RFUN( do_del_affect	);	/* by Amenda */
DECLARE_DO_RFUN( do_rforce	);	/* by Amenda */
DECLARE_DO_RFUN( do_rtransfer	);	/* by Amenda */
DECLARE_DO_RFUN( do_rtransfergroup	);	/* 2018/07/18 */
DECLARE_DO_RFUN( do_rwtransfer	);	    /* 2022/11/21 */
DECLARE_DO_RFUN( do_rwtransfergroup	);	/* 2022/11/21 */
DECLARE_DO_RFUN( do_rpurge	);	/* by Amenda */
DECLARE_DO_RFUN( do_rlog	);	/* by Amenda */
DECLARE_DO_RFUN( do_radd_qmark	);	/* by Amenda */
DECLARE_DO_RFUN( do_rpexp ); /*by Coconet*/
DECLARE_DO_RFUN( do_rpgold ); /*by Coconet*/
DECLARE_DO_RFUN( do_rpresp ); /*by Coconet*/
DECLARE_DO_RFUN( do_rpalign ); /* add at 20.02.25 */
DECLARE_DO_RFUN( do_rpskillexp ); /* add at 20.02.25 */
DECLARE_DO_RFUN( do_rpmerit ); /* add at 20.02.25 */
DECLARE_DO_RFUN( do_rppractice ); /* add at 20.02.25 */
DECLARE_DO_RFUN( do_rgreward); /*by zcecil*/
DECLARE_DO_RFUN( do_radd_all_qmark	);/* by Amenda */
DECLARE_DO_RFUN( do_rdel_qmark	);	/* by Amenda */
DECLARE_DO_RFUN( do_rdel_all_qmark	);	/* by Amenda */
DECLARE_DO_RFUN( do_rmod_qmark	);	/* by Amenda */
DECLARE_DO_RFUN( do_rmod_all_qmark	);	/* by Amenda */
DECLARE_DO_RFUN( do_rpmakedoor	);	/* by zcecil */
DECLARE_DO_RFUN( do_rpdeldoor	);	/* by zcecil */
DECLARE_DO_RFUN( do_rpsetdoor	);	/* by zcecil */
DECLARE_DO_RFUN( do_rcallout);	/* by zcecil */
DECLARE_DO_RFUN( do_set_room	);  /* by Amethyst */
DECLARE_DO_RFUN( do_rpstun	);  /* by Amethyst */

DECLARE_DO_OFUN( do_ooset	);	/* zcecil */
DECLARE_DO_OFUN( do_ooecho);	/* zcecil */
DECLARE_DO_OFUN( do_oechotoall);	/* zcecil */
DECLARE_DO_OFUN( do_oechoat);	/* zcecil */
DECLARE_DO_OFUN( do_oechoaround);	/* zcecil */
DECLARE_DO_OFUN( do_ononechoat);
DECLARE_DO_OFUN( do_ononechoaround);
DECLARE_DO_OFUN( do_oteleport );   /* by Amethyst */
DECLARE_DO_OFUN( do_ogoto ); /* by Amethyst */
DECLARE_DO_OFUN( do_oforce); /* coconet */
DECLARE_DO_OFUN( do_omodhmv);/*coconet*/
DECLARE_DO_OFUN( do_oforcer);/*coconet*/
DECLARE_DO_OFUN( do_oadd_affect	);	/* by coconet */
DECLARE_DO_OFUN( do_odel_affect );
DECLARE_DO_OFUN( do_odef_rembch ); /*2022/11/10*/
DECLARE_DO_OFUN( do_ocast		); /*2022/11/10*/
DECLARE_DO_OFUN( do_opurge );
DECLARE_DO_OFUN( do_opset_obj_affect    );	//Added by Razgriz 20050916
DECLARE_DO_OFUN( do_opremove_obj_affect );	//Added by Razgriz 20050916
DECLARE_DO_OFUN( do_opmodify_obj_affect );	//Added by Razgriz 20050916
DECLARE_DO_OFUN( do_opmodvt );			//Added by Razgriz 20050918
/*
 * Interp.c
 */
DECLARE_DO_FUN( do_aedit	);	/* OLC 1.1b */
DECLARE_DO_FUN( do_redit	);	/* OLC 1.1b */
DECLARE_DO_FUN( do_ppl_redit	);	/* OLC 1.1b */
DECLARE_DO_FUN( do_oedit	);	/* OLC 1.1b */
DECLARE_DO_FUN( do_medit	);	/* OLC 1.1b */
DECLARE_DO_FUN( do_asave	);
DECLARE_DO_FUN( do_alist	);
DECLARE_DO_FUN( do_resets	);
DECLARE_DO_FUN(	       do_mpedit	);



/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	  spell_null		    );
DECLARE_SPELL_FUN(	  spell_fire_wall);
DECLARE_SPELL_FUN(	  spell_meteor_shower);
DECLARE_SPELL_FUN(	  spell_ice_comet);
DECLARE_SPELL_FUN(	  spell_magic_arrow);
DECLARE_SPELL_FUN(	  spell_mental_empower);
DECLARE_SPELL_FUN(	  spell_resurrection);
DECLARE_SPELL_FUN(	  spell_mark);
DECLARE_SPELL_FUN(	  spell_recall);
DECLARE_SPELL_FUN(	  spell_evacuate);
DECLARE_SPELL_FUN(	  spell_earth_slash  );
DECLARE_SPELL_FUN(	  spell_mana_recharge);
DECLARE_SPELL_FUN(	  spell_double_recharge);
DECLARE_SPELL_FUN(	  spell_full_recharge);
DECLARE_SPELL_FUN(	  spell_flaming_slash);
DECLARE_SPELL_FUN(	  spell_flaming_slash2); // add by whisperwind 2.18.2005
DECLARE_SPELL_FUN(	  spell_storm_slash);
DECLARE_SPELL_FUN(	  spell_icecore_slash);
DECLARE_SPELL_FUN(	  spell_fulgurant_slash);
DECLARE_SPELL_FUN(	  spell_elemental_slash);
DECLARE_SPELL_FUN(	  spell_sun_slash);
DECLARE_SPELL_FUN(	  spell_sprinkle_slash);
DECLARE_SPELL_FUN(	  spell_calm);
DECLARE_SPELL_FUN(	  spell_lifeshare);
DECLARE_SPELL_FUN(	  spell_mystic_mirror);
DECLARE_SPELL_FUN(	  spell_mystic_armor);
DECLARE_SPELL_FUN(	  spell_virtual_swords);
DECLARE_SPELL_FUN(	  spell_sober_up);
DECLARE_SPELL_FUN(	  spell_acid_blast	  );
DECLARE_SPELL_FUN(	  spell_armor		     );
DECLARE_SPELL_FUN(	  spell_bless		     );
DECLARE_SPELL_FUN(	  spell_wizard_rage);
DECLARE_SPELL_FUN(	  spell_hasten);
DECLARE_SPELL_FUN(	  spell_powerup);
DECLARE_SPELL_FUN(	  spell_pfire ); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_pcold); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_pwind); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_pearth); /* Zcecil */
DECLARE_SPELL_FUN(        spell_plightning); /*coconet*/
DECLARE_SPELL_FUN(	  spell_day_of_prot); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_day_of_power); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_crushing_hand); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_song_of_cleanse); /* Zcecil */
DECLARE_SPELL_FUN(	  spell_day_of_brightness); /* Amenda */
DECLARE_SPELL_FUN(	  spell_blindness		 );
DECLARE_SPELL_FUN(	  spell_paralyze		 );	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_demontremor		 );	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_powerball		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_starfire		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_whirlwind		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_grave_robber		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_full_refresh		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_seal_magic		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_group_full_refresh	);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_group_double_heal	);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_vivification		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_soulsteal		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_lifesteal		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_flaming_armor		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_mana_shield		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_armageddon		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_wizard_rage		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_remove_nodrop		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_extra_heal		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_gextra_heal		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_divine_light		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_glacier_strike		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_clay_golem		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_ogre_power		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_endless_sight		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_water_spirit		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_greendragon_breath	);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_fire_elemental		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_double_heal		);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_group_cure_blindness	);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_wind_screaming	);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_angry_fire_storm	);	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_sun_flash	);		/* by Amenda */
DECLARE_SPELL_FUN(	  spell_green_breath		 );	/* by Amenda */
DECLARE_SPELL_FUN(	  spell_burning_hands	     );
DECLARE_SPELL_FUN(	  spell_breathe_water	     );	       /* by Thelonius */
DECLARE_SPELL_FUN(	  spell_call_lightning	      );
DECLARE_SPELL_FUN(	  spell_cause_critical	      );
DECLARE_SPELL_FUN(	  spell_cause_light	   );
DECLARE_SPELL_FUN(	  spell_cause_serious	     );
DECLARE_SPELL_FUN(	  spell_change_sex	  );
DECLARE_SPELL_FUN(	  spell_charm_person	    );
DECLARE_SPELL_FUN(	  spell_chill_touch	   );
DECLARE_SPELL_FUN(	  spell_colour_spray	    );
DECLARE_SPELL_FUN(	  spell_cone_of_silence	       );	 /* by Thelonius */
DECLARE_SPELL_FUN(	  spell_continual_light	       );
DECLARE_SPELL_FUN(	  spell_control_weather	       );
DECLARE_SPELL_FUN(	  spell_create_food	   );
DECLARE_SPELL_FUN(	  spell_create_spring	     );
DECLARE_SPELL_FUN(	  spell_create_water	    );
DECLARE_SPELL_FUN(	  spell_cure_blindness	      );
DECLARE_SPELL_FUN(	  spell_cure_critical	     );
DECLARE_SPELL_FUN(	  spell_cure_light	  );
DECLARE_SPELL_FUN(	  spell_cure_poison	   );
DECLARE_SPELL_FUN(	  spell_cure_serious	    );
DECLARE_SPELL_FUN(	  spell_curse		     );
DECLARE_SPELL_FUN(	  spell_destroy_cursed	  );
DECLARE_SPELL_FUN(	  spell_detect_evil	   );
DECLARE_SPELL_FUN(	  spell_detect_hidden	     );
DECLARE_SPELL_FUN(	  spell_detect_invis	    );
DECLARE_SPELL_FUN(	  spell_detect_magic	    );
DECLARE_SPELL_FUN(	  spell_detect_poison	     );
DECLARE_SPELL_FUN(	  spell_dispel_evil	   );
DECLARE_SPELL_FUN(	  spell_dispel_magic	    );	      /* by Thelonius */
DECLARE_SPELL_FUN(	  spell_earthquake	  );
DECLARE_SPELL_FUN(	  spell_enchant_weapon	      );
DECLARE_SPELL_FUN(	  spell_enchant_armor);
DECLARE_SPELL_FUN(	  spell_energy_drain	    );
DECLARE_SPELL_FUN(	  spell_exorcise	  );
DECLARE_SPELL_FUN(	  spell_faerie_fire	   );
DECLARE_SPELL_FUN(	  spell_faerie_fog	  );
DECLARE_SPELL_FUN(	  spell_fireball		);
DECLARE_SPELL_FUN(	  spell_flamestrike	   );
DECLARE_SPELL_FUN(	  spell_flaming		  );
DECLARE_SPELL_FUN(	  spell_storm		  );
DECLARE_SPELL_FUN(	  spell_sprinkle		  );
DECLARE_SPELL_FUN(	  spell_fly		   );
DECLARE_SPELL_FUN(	  spell_gate		    );
DECLARE_SPELL_FUN(	  spell_general_purpose	       );
DECLARE_SPELL_FUN(	  spell_giant_strength	      );
DECLARE_SPELL_FUN(	  spell_harm		    );
DECLARE_SPELL_FUN(	  spell_heal		    );
DECLARE_SPELL_FUN(	  spell_cont_heal);
DECLARE_SPELL_FUN(	  spell_high_explosive	      );
DECLARE_SPELL_FUN(	  spell_identify		);
DECLARE_SPELL_FUN(	  spell_infravision	   );
DECLARE_SPELL_FUN(	  spell_invis		     );
DECLARE_SPELL_FUN(	  spell_know_alignment	      );
DECLARE_SPELL_FUN(	  spell_lightning_bolt	      );
DECLARE_SPELL_FUN(	  spell_locate_object	     );
DECLARE_SPELL_FUN(	  spell_magic_missile	     );
DECLARE_SPELL_FUN(	  spell_mass_heal	  );
DECLARE_SPELL_FUN(	  spell_mass_invis	  );
DECLARE_SPELL_FUN(	  spell_mute		    );	      /* by Thelonius */
DECLARE_SPELL_FUN(	  spell_pass_door		 );
DECLARE_SPELL_FUN(	  spell_poison		      );
DECLARE_SPELL_FUN(	  spell_poison_explosion );//add by coconet 20041014
DECLARE_SPELL_FUN(	  spell_polymorph_other	       );
DECLARE_SPELL_FUN(	  spell_protection	  );
DECLARE_SPELL_FUN(	  spell_recharge_item	     );	       /* by Thelonius */
DECLARE_SPELL_FUN(	  spell_refresh		       );
DECLARE_SPELL_FUN(	  spell_double_refresh		       );
DECLARE_SPELL_FUN(	  spell_remove_alignment	);	  /* by Thelonius */
DECLARE_SPELL_FUN(	  spell_remove_curse	    );
DECLARE_SPELL_FUN(	  spell_remove_silence	      );	/* by Thelonius */
DECLARE_SPELL_FUN(	  spell_sanctuary		 );
DECLARE_SPELL_FUN(	  spell_shocking_grasp	      );
DECLARE_SPELL_FUN(	  spell_shield		      );
DECLARE_SPELL_FUN(	  spell_sleep		     );
DECLARE_SPELL_FUN(	  spell_stone_skin	  );
DECLARE_SPELL_FUN(	  spell_summon		      );
DECLARE_SPELL_FUN(	  spell_tardy		);		/* Amenda */
DECLARE_SPELL_FUN(	  spell_teleport		);
DECLARE_SPELL_FUN(	  spell_turn_undead	   );
DECLARE_SPELL_FUN(	  spell_ventriloquate	     );
DECLARE_SPELL_FUN(	  spell_weaken		      );
DECLARE_SPELL_FUN(	  spell_word_of_recall	      );
DECLARE_SPELL_FUN(	  spell_acid_breath	   );
DECLARE_SPELL_FUN(	  spell_fire_breath	   );
DECLARE_SPELL_FUN(	  spell_frost_breath	    );
DECLARE_SPELL_FUN(	  spell_gas_breath	  );
DECLARE_SPELL_FUN(	  spell_lightning_breath	);
DECLARE_SPELL_FUN(	  spell_vampiric_bite	     );
DECLARE_SPELL_FUN(	  spell_repair_armor	    );
DECLARE_SPELL_FUN(        spell_holy_crown                );
DECLARE_SPELL_FUN(        spell_mass_turn                );
DECLARE_SPELL_FUN(        spell_disease                );  /*by Coconet */
DECLARE_SPELL_FUN(        spell_cure_disease         );  /*by coconet 20030730*/
DECLARE_SPELL_FUN(        spell_fear                );  /*by coconet */
DECLARE_SPELL_FUN(        spell_dispel_good        );/*by Neversay*/
DECLARE_SPELL_FUN(        spell_electrostatic_bolt	);/*by Neversay*/
DECLARE_SPELL_FUN(        spell_sand_tide	);/*by Neversay 2004/June/25*/
DECLARE_SPELL_FUN(        spell_banshee_wail	);/*by Neversay 2004/June/25*/
DECLARE_SPELL_FUN(        spell_succubus_kiss	);/*by Neversay 2004/June/25*/
DECLARE_SPELL_FUN(	  spell_will_fortress	);/*by Neversay 2004/June/26*/
DECLARE_SPELL_FUN(        spell_mind_blast      );/*by Neversay 2004/July/3*/
DECLARE_SPELL_FUN(        spell_disenchant      );/*by Neversay 2004/July/3*/
DECLARE_SPELL_FUN(	  spell_ghosts_celebration);/*by Neversay 2004/July/12*/
DECLARE_SPELL_FUN(	  spell_chain_lightning ); // by Whisperwind 10.1.2004
DECLARE_SPELL_FUN(	  spell_finger_of_death ); // by coconet 10/20/2004 
DECLARE_SPELL_FUN(	  spell_apocalypse	); // by coconet 10.20.2004
DECLARE_SPELL_FUN(	  spell_dark_containment); // by coconet 10.20.2004
DECLARE_SPELL_FUN(	  spell_phlogistic_verse); // by coconet 10.20.2004
DECLARE_SPELL_FUN( 	  spell_wail_of_tempest ); // by coconet 10.20.2004
DECLARE_SPELL_FUN(	  spell_fulgurant_dance ); // by coconet 10.20.2004
DECLARE_SPELL_FUN(	  spell_salvation 	);
DECLARE_SPELL_FUN(	  spell_agglomerate_descant);
DECLARE_SPELL_FUN(	  spell_earth_bind	);
DECLARE_SPELL_FUN(	  spell_holy_judgement	);
DECLARE_SPELL_FUN(	  spell_hell_southencross);
DECLARE_SPELL_FUN(	  spell_song_of_firestorm);
DECLARE_SPELL_FUN(	  spell_lore_of_frost	);
DECLARE_SPELL_FUN(	  spell_soul_of_magma	);
DECLARE_SPELL_FUN(	  spell_fang_of_slime	);
DECLARE_SPELL_FUN(	  spell_magic_enchantment	);
DECLARE_SPELL_FUN(	  spell_wind_enchantment	);
DECLARE_SPELL_FUN(	  spell_flame_enchantment	);
DECLARE_SPELL_FUN(	  spell_water_enchantment	);
DECLARE_SPELL_FUN(	  spell_earth_enchantment	);
DECLARE_SPELL_FUN(	  spell_lightning_enchantment	);
DECLARE_SPELL_FUN(	  spell_saint_enchantment	);
DECLARE_SPELL_FUN(	  spell_dark_enchantment	);

//added by jye 0940711
//Sor New Skill
DECLARE_SPELL_FUN(	  spell_flame_elemental	);
DECLARE_SPELL_FUN(	  spell_water_elemental	);
DECLARE_SPELL_FUN(	  spell_earth_elemental	);
DECLARE_SPELL_FUN(	  spell_wind_elemental	);
DECLARE_SPELL_FUN(	  spell_lightning_elemental	);
/*
 * Psi spell_functions, in magic.c (by Thelonius).
 */
DECLARE_SPELL_FUN(	spell_adrenaline_control);
DECLARE_SPELL_FUN(	spell_agitation		);
DECLARE_SPELL_FUN(	spell_aura_sight	);
DECLARE_SPELL_FUN(	spell_awe		);
DECLARE_SPELL_FUN(	spell_ballistic_attack	);
DECLARE_SPELL_FUN(	spell_biofeedback	);
DECLARE_SPELL_FUN(	spell_cell_adjustment	);
DECLARE_SPELL_FUN(	spell_combat_mind	);
DECLARE_SPELL_FUN(	spell_full_heal		);
DECLARE_SPELL_FUN(	spell_group_full_heal	);
DECLARE_SPELL_FUN(	spell_control_flames	);
DECLARE_SPELL_FUN(	spell_create_sound	);
DECLARE_SPELL_FUN(	spell_death_field	);
DECLARE_SPELL_FUN(	spell_detonate		);
DECLARE_SPELL_FUN(	spell_disintegrate	);
DECLARE_SPELL_FUN(	spell_displacement	);
DECLARE_SPELL_FUN(	spell_domination	);
DECLARE_SPELL_FUN(	spell_ectoplasmic_form	);
DECLARE_SPELL_FUN(	spell_ego_whip		);
DECLARE_SPELL_FUN(	spell_energy_containment);
DECLARE_SPELL_FUN(	spell_enhance_armor	);
DECLARE_SPELL_FUN(	spell_enhanced_strength );
DECLARE_SPELL_FUN(	spell_flesh_armor	);
DECLARE_SPELL_FUN(	spell_inertial_barrier	);
DECLARE_SPELL_FUN(	spell_inflict_pain	);
DECLARE_SPELL_FUN(	spell_lend_health	);
DECLARE_SPELL_FUN(	spell_levitation	);
DECLARE_SPELL_FUN(	spell_barrier	);
DECLARE_SPELL_FUN(	spell_mind_thrust	);
DECLARE_SPELL_FUN(	spell_project_force	);
DECLARE_SPELL_FUN(	spell_psionic_blast	);
DECLARE_SPELL_FUN(	spell_psychic_crush	);
DECLARE_SPELL_FUN(	spell_psychic_drain	);
DECLARE_SPELL_FUN(	spell_psychic_healing	);
DECLARE_SPELL_FUN(	spell_share_strength	);
DECLARE_SPELL_FUN(	spell_thought_shield	);
DECLARE_SPELL_FUN(	spell_ultrablast	);
DECLARE_SPELL_FUN(	spell_lore		);

/*
 * Global spec_fun for water spirit spell
 */
DECLARE_SPEC_FUN(	spec_cast_water		);
DECLARE_SPEC_FUN(	spec_cast_fire		);
/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	   defined( _AIX )
char *crypt args( ( const char *key, const char *salt ) );
#endif

#if	defined( amiga )
#define AmigaTCP
#endif

#if	   defined( apollo )
int atoi args( ( const char *string ) );
void *calloc args( ( unsigned nelem, size_t size ) );
char *crypt args( ( const char *key, const char *salt ) );
#endif

#if	   defined( hpux )
char *crypt args( ( const char *key, const char *salt ) );
#endif

#if	   defined( linux )
char *crypt args( ( const char *key, const char *salt ) );
#endif

#if	   defined( macintosh ) || defined( WIN32 )
#define NOCRYPT
#if	   defined( unix )
#undef	      unix
#endif
#endif

#if	   defined( MIPS_OS )
char *crypt args( ( const char *key, const char *salt ) );
#endif

#if	   defined( NeXT )
char *crypt args( ( const char *key, const char *salt ) );
#endif

#if	   defined( sequent )
char *crypt args( ( const char *key, const char *salt ) );
int fclose args( ( FILE *stream ) );
int fprintf args( ( FILE *stream, const char *format, ... ) );
int fread args( ( void *ptr, int size, int n, FILE *stream ) );
int fseek args( ( FILE *stream, long offset, int ptrname ) );
void perror args( ( const char *s ) );
int ungetc args( ( int c, FILE *stream ) );
#endif

#if	   defined( sun )
char *crypt args( ( const char *key, const char *salt ) );
int fclose args( ( FILE *stream ) );
int fprintf args( ( FILE *stream, const char *format, ... ) );
size_t fread args( ( void *ptr, size_t size, size_t nitems, FILE *stream ) );
int fseek args( ( FILE *stream, long offset, int ptrname ) );
void perror args( ( const char *s ) );
int ungetc args( ( int c, FILE *stream ) );
#endif

#if	   defined( ultrix )
char *crypt args( ( const char *key, const char *salt ) );
#endif

/*
 * Stuff for DEC UNIX on Alpha (OSF3.2C)
 * Fusion
 */
#if defined( _OSF_SOURCE )
char *crypt args( ( const char *key, const char *salt ) );
int system args( ( const char *string ) );
ssize_t read args( ( int fd, void *buf, size_t nbyte ) );
ssize_t write args( ( int fd, const void *buf, size_t nbyte ) );
int close args( ( int fd ) );
#endif


/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	   defined( NOCRYPT )
#define crypt( s1, s2 )	       ( s1 )
#endif

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	  CHAR_DATA
#define MID	   MOB_INDEX_DATA
#define OD	  OBJ_DATA
#define AD	  AFFECT_DATA
#define ED	  EXTRA_DESCR_DATA
#define OID	   OBJ_INDEX_DATA
#define RID	   ROOM_INDEX_DATA
#define SF	  SPEC_FUN

/* act_comm.c */
void add_member args( ( CHAR_DATA *ch, CHAR_DATA *member) );
void clear_member args( ( CHAR_DATA *ch ) );
void remove_member args( ( CHAR_DATA *member ) );
void add_follower args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void stop_follower args( ( CHAR_DATA *ch ) );
void die_follower args( ( CHAR_DATA *ch, char *name ) );
void add_protector args( (CHAR_DATA *ch, CHAR_DATA *pch));
void stop_protecting args( (CHAR_DATA *ch, CHAR_DATA *pch));
void add_cooperator args( ( CHAR_DATA *ch, CHAR_DATA *pch ) );  // 2021/11/20
void stop_cooperating args( ( CHAR_DATA *ch, CHAR_DATA *pch) ); // 2021/11/20
void stop_linking args( ( CHAR_DATA *ch , CHAR_DATA *pch) ); // 2021/11/21
void add_linking args( ( CHAR_DATA *ch, CHAR_DATA *pch) ); // 2021/11/20
void add_rember args( (CHAR_DATA *ch, CHAR_DATA *pch));
void stop_rember args( (CHAR_DATA *ch, CHAR_DATA *pch));
void add_rrember args( (ROOM_INDEX_DATA *ch, CHAR_DATA *pch));
void stop_rrember args( (ROOM_INDEX_DATA *ch, CHAR_DATA *pch));
void add_orember args( (OBJ_DATA *ch, CHAR_DATA *pch));  // 2022/11/10
void stop_orember args( (OBJ_DATA *ch, CHAR_DATA *pch));  // 2022/11/10
void remove_protector args( (CHAR_DATA *ch) );
void remove_cooperator args( ( CHAR_DATA *ch) );  // 2021/11/20
void remove_linking args( ( CHAR_DATA *ch  ) );  // 2021/11/21
void remove_rember args( (CHAR_DATA *ch) );
void remove_rrember args( (CHAR_DATA *ch) );
bool is_same_group args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
bool is_note_to args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );

/* act_info.c */
int skill_exp_level args( ( int exp ) );
void set_title args( ( CHAR_DATA *ch, char *title ) );
void set_cname args( ( CHAR_DATA *ch, char *cname ) );
bool check_blind args( ( CHAR_DATA *ch ) );
void scan_print args( ( CHAR_DATA *ch, EXIT_DATA *pexit, int door, int level ) );
void scout args( ( CHAR_DATA *ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room , int sklv ) ); // 加入 sklv 2022/12/22
void top10_check args(( CHAR_DATA *ch ));

/* act_move.c */
void move_char args( ( CHAR_DATA *ch, int door ) );

/* act_obj.c */
bool remove_obj args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );

int do_autoget( CHAR_DATA *ch, char *argument );
/* act_wiz.c */
ROOM_INDEX_DATA *find_location args( ( CHAR_DATA *ch, char *arg ) );

/* clan.c */
void load_clans args( ( void ) );
void clan_update args( ( void ) );

/* chinese.c */

char *c_item_type_name args( ( OBJ_DATA *obj ) );
char *c_affect_loc_name args( ( int location ) );
char *c_affect_bit_name args( ( long long int vector ) );
char *c_extra_bit_name args( ( int extra_flags ) );
void strip_ansi args( ( char *src, char *dup ) );

/* comm.c */
void close_socket args( ( DESCRIPTOR_DATA *dclose ) );
void write_to_buffer args( ( DESCRIPTOR_DATA *d, const char *txt, int length ) );
void send_to_room args( ( const char *txt, ROOM_INDEX_DATA *room ) );
void send_to_all_char args( ( const char *text ) );
void send_to_char args( ( const char *txt, CHAR_DATA *ch ) );
void show_string args( ( DESCRIPTOR_DATA *d, char *input ) );
void act args( ( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type ) );
bool check_parse_name args( ( char *name ) );
void combat_info args( ( const char *format, CHAR_DATA *ch, 
				const void *arg, int type ) );
/* db.c */
void boot_db args( ( void ) );
int interpolate1( int level1, int value1, int level2, int value2, int level );
int interpolate( int level, int value_00, int value_42 );

//long long int	atoll(char*);
void area_update args( ( void ) );
CD *new_character args( ( bool player ) );
AD *new_affect args( ( void ) );
CALL_OUT_DATA *new_call_out args((void));
RCALL_OUT_DATA *new_rcall_out args((void));
void free_call_out args((CALL_OUT_DATA *rm));
void free_rcall_out args((RCALL_OUT_DATA *rm));
#ifdef HAS_ROOM_AFFECT
R_AFFECT_DATA *new_r_affect args( ( void ) );
#endif
OD *new_object args( ( void ) );
ED *new_extra_descr args( ( void ) );
CD *create_mobile args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *create_object args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
OD *fcreate_object args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void clear_char args( ( CHAR_DATA *ch ) );
void free_char args( ( CHAR_DATA *ch ) );
char *get_extra_descr args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
char *get_pfxextra_descr args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
MID *get_mob_index args( ( int vnum ) );
OID *get_obj_index args( ( int vnum ) );
RID *get_room_index args( ( int vnum ) );
char fread_letter args( ( FILE *fp ) );
long long int fread_number args( ( FILE *fp, int *status ) );
char *fread_string args( ( FILE *fp, int *status ) );
char *diw_fread_string args( ( FILE *fp, int *status ) );
void fread_to_eol args( ( FILE *fp ) );
char *fread_word args( ( FILE *fp, int *status ) );
char *fread_alpha_word args( ( FILE *fp, int *status ) );
void *alloc_mem args( ( int sMem ) );
void *alloc_perm args( ( int sMem ) );
void free_mem args( ( void *pMem) );
char *str_dup args( ( const char *str ) );
void free_string args( ( char *pstr ) );
int number_fuzzy args( ( int number ) );
int number_range args( ( int from, int to ) );
int number_percent args( ( void ) );
int number_door args( ( void ) );
int number_bits args( ( int width ) );
int number_mm args( ( void ) );
int dice args( ( int number, int size ) );
int interpolate args( ( int level, int value_00, int value_32 ) );
void smash_tilde args( ( char *str ) );
bool str_cmp args( ( const char *astr, const char *bstr ) );
bool ip_str_cmp args( ( const char *astr, const char *bstr ) );
bool str_prefix args( ( const char *astr, const char *bstr ) );
bool str_infix args( ( const char *astr, const char *bstr ) );
bool str_suffix args( ( const char *astr, const char *bstr ) );
char *capitalize args( ( const char *str ) );
void append_file args( ( CHAR_DATA *ch, char *file, char *str ) );
void bug args( ( const char *str, int param ) );
void bugf(char* fmt, ...);
void logf_envy(char* fmt, ...);
void log_string args( ( const char *str ) );
void tail_chain args( ( void ) );

void string_edit args( ( CHAR_DATA *ch, char **pString ) );
void string_append args( ( CHAR_DATA *ch, char **pString ) );
char *string_replace args( ( char * orig, char * old, char * new ) );
void string_add args( ( CHAR_DATA *ch, char *argument ) );
char *format_string args( ( char *oldstring /*, bool fSpace */ ) );
char *first_arg args( ( char *argument, char *arg_first, bool fCase ) );
char *string_unpad args( ( char * argument ) );
char *string_proper args( ( char * argument ) );

void reset_area args( ( AREA_DATA * pArea ) );
void reset_room args( ( ROOM_INDEX_DATA * pRoom ) );

void top10_load args((void));
void top10_save args((void));

/* fight.c */
bool raw_kill( CHAR_DATA *ch, CHAR_DATA *victim );
void set_fighting args( ( CHAR_DATA *ch, CHAR_DATA *victim ));

void self_damage( CHAR_DATA *ch, CHAR_DATA *victim, int miss_times, int no_dam_times, int dam_times, int times, int dam, char *skill_name );
void disarm args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void trip args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
CHAR_DATA *check_mob_ai( CHAR_DATA * mob );
void trigger_obj_hp( OBJ_DATA *obj, int sn, int level,CHAR_DATA *ch, void* vo);
void violence_update args( ( void ) );
void multi_hit args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
//int brief__damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
//int brief_damage  args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
//void _damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
int	damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn, int msg_mode, int dmg_mode ) ); //Modified by Razgriz 20051014
void magic__damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
int magic_damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
bool is_real_npc args( ( CHAR_DATA *ch ) ); /** zcecil, 浪琩mob琌ぃ琌獶篈 */
void stop_fighting args( ( CHAR_DATA *ch, bool fBoth ) );
void update_pos args( ( CHAR_DATA *victim ) );
void check_killer args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool is_safe args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool licensed args( ( CHAR_DATA *ch ) );
bool registered args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void death_cry args( ( CHAR_DATA *ch ) );
void check_asshunt args( ( CHAR_DATA *ch ) );
void brief_dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn,int total, int hit ) );
void dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
/* handler.c */
OBJ_DATA *get_obj_wear2( CHAR_DATA *ch, char *argument );
bool check_skill(CHAR_DATA *ch,int sn, int chance); /* by zcecil */
int use_skill_exp(CHAR_DATA *ch, int value);
void regain_skill_exp( CHAR_DATA *ch, int xp);
int skill_difficulty( CHAR_DATA *ch, int sn);
bool check_int( int i);
bool consented( CHAR_DATA *ch, OBJ_DATA *corpse);
bool IS_HORSE( CHAR_DATA *ch);
int GET_AC( CHAR_DATA * ch);
int get_skill_exp( CHAR_DATA *ch);
int get_skill_level( CHAR_DATA *ch, int sn);
int skill_difficuty(CHAR_DATA *ch, int sn);
int skill_max_percent( CHAR_DATA *ch, int sn);
int skill_req_level( CHAR_DATA *ch, int sn);
int skill_preq_level( CHAR_DATA *ch, int sn);
int get_skill_percent( CHAR_DATA *ch, int sn);
int gain_skill_level( CHAR_DATA* ch, int sn, int value);
int gain_skill_percent( CHAR_DATA* ch, int sn, int value);
void WAIT_STATE( CHAR_DATA *ch, int wait);
void WAIT_MAGIC_STATE( CHAR_DATA *ch, int wait);
void add_call_out( CALL_OUT_DATA *call);
QMARK *is_qmark(CHAR_DATA *ch, char* qmark);
QMARK *is_prefix_qmark(CHAR_DATA *ch, char* qmark);
bool is_flying(CHAR_DATA *ch);
int	room_clanlv( ROOM_INDEX_DATA *pROOM);
bool is_chinese(CHAR_DATA* ch, CHAR_DATA* victim);
bool is_brief(CHAR_DATA* ch, CHAR_DATA* victim);
#ifdef  __ROC_CHINESE_C__
bool is_chinese_char(unsigned char c, bool flag);
#else
extern bool is_chinese_char(unsigned char c, bool flag);
#endif
void waste_dura(OBJ_DATA *obj);
void waste_duraz(OBJ_DATA *obj);
bool has_player(char *name);
void repair_dura(OBJ_DATA *obj,int value);
long long int symbol_lookup(char *name);
bool meet_req_prime args((CHAR_DATA * ch,int sn, int attr));
bool meet_req_second args((CHAR_DATA * ch,int sn, int attr));
int GET_DODGE args ((CHAR_DATA * ch ));
bool is_broken( OBJ_DATA *obj);
bool indestructible( OBJ_DATA *obj);
int GET_SVS args ((CHAR_DATA * ch ));
int NEW_GET_SVS args ((CHAR_DATA * ch, int type ));
int get_trust args( ( CHAR_DATA *ch ) );
int get_age args( ( CHAR_DATA *ch ) );
int get_curr_dura args((OBJ_DATA * obj));
int get_max_dura args((OBJ_DATA * obj));
int get_max_skill_exp args(( CHAR_DATA *ch));
int get_eff_skill_exp args(( CHAR_DATA *ch));
void gain_skill_exp args(( CHAR_DATA *ch, int xp ));
int get_curr_str args( ( CHAR_DATA *ch ) );
int get_curr_int args( ( CHAR_DATA *ch ) );
int get_curr_wis args( ( CHAR_DATA *ch ) );
int get_curr_dex args( ( CHAR_DATA *ch ) );
int get_curr_con args( ( CHAR_DATA *ch ) );
int get_curr_size args( ( CHAR_DATA *ch ) ); // 砰 2022/12/23
int get_eff_str args( ( CHAR_DATA *ch ) );
int get_eff_int args( ( CHAR_DATA *ch ) );
int get_eff_wis args( ( CHAR_DATA *ch ) );
int get_eff_dex args( ( CHAR_DATA *ch ) );
int get_eff_con args( ( CHAR_DATA *ch ) );
int get_max_str args( ( CHAR_DATA *ch ) );
int get_max_int args( ( CHAR_DATA *ch ) );
int get_max_wis args( ( CHAR_DATA *ch ) );
int get_max_dex args( ( CHAR_DATA *ch ) );
int get_max_con args( ( CHAR_DATA *ch ) );
int	get_hitroll args( ( CHAR_DATA *ch, int wpn ) );
int	get_damroll args( ( CHAR_DATA *ch, int wpn ) );
int	get_vrace_hrq args( ( CHAR_DATA *victim, OBJ_DATA * wpn ) );
int	get_vrace_hrp args( ( CHAR_DATA *victim, OBJ_DATA *wpn ) );
int	get_vrace_drq args( ( CHAR_DATA *victim, OBJ_DATA *wpn ) );
int	get_vrace_drp args( ( CHAR_DATA *victim, OBJ_DATA *wpn ) );
int can_carry_n args( ( CHAR_DATA *ch ) );
int can_carry_w args( ( CHAR_DATA *ch ) );
bool is_name args( ( const char *str, char *namelist ) );
bool is_pfxname args( ( const char *str, char *namelist ) );
#ifdef HAS_ROOM_AFFECT
void affect_to_room args( ( ROOM_INDEX_DATA *ch, R_AFFECT_DATA *paf ) );
void r_affect_remove args( ( ROOM_INDEX_DATA *ch, R_AFFECT_DATA *paf ) );
void r_affect_strip args( ( ROOM_INDEX_DATA *ch, int sn ) );
bool r_is_affected args( ( ROOM_INDEX_DATA *ch, int sn ) );
void r_affect_join args( ( ROOM_INDEX_DATA *ch, R_AFFECT_DATA *paf ) );
#endif
void affect_to_char args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void affect_remove args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void affect_strip args( ( CHAR_DATA *ch, int sn ) );
bool is_affected args( ( CHAR_DATA *ch, int sn ) );
int affected_value args( ( CHAR_DATA *ch, int sn, int apply_type ) );
void affect_join args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void char_from_room args( ( CHAR_DATA *ch ) );
void char_from_room2 args( ( CHAR_DATA *ch ) );
void char_to_room args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void obj_to_char args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void obj_from_char args( ( OBJ_DATA *obj ) );
int apply_ac args( ( OBJ_DATA *obj, int iWear ) );
OD *get_eq_char args( ( CHAR_DATA *ch, int iWear ) );
void equip_char args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void unequip_char args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
void equip_char2 args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) ); // add at 2022/11/29
void unequip_char2 args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );          // add at 2022/11/29
int count_obj_list args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void obj_from_room args( ( OBJ_DATA *obj ) );
void obj_to_room args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void obj_to_obj args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void obj_from_obj args( ( OBJ_DATA *obj ) );
void extract_obj args( ( OBJ_DATA *obj ) );
void extract_char args( ( CHAR_DATA *ch, bool fPull ) );
CD *get_char_room args( ( CHAR_DATA *ch, char *argument ) );
CD *get_char_world args( ( CHAR_DATA *ch, char *argument ) );
CD *get_char_world2 args( ( CHAR_DATA *ch, char *argument ) );	//Added by Razgriz	20050831
OD *get_obj_type args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *get_obj_list args( ( CHAR_DATA *ch, char *argument, OBJ_DATA *list ) );
OD *get_obj_carry args( ( CHAR_DATA *ch, char *argument ) );
OD *get_obj_wear args( ( CHAR_DATA *ch, char *argument ) );
OD *get_char_obj args( ( CHAR_DATA *ch, char *argument ) ); // 抓出 ch 身上所有的 obj 2023/01/08
OD *get_obj_here args( ( CHAR_DATA *ch, char *argument ) );
OD *get_room_obj args( ( ROOM_INDEX_DATA *pRoomIndex, char *argument ) ); // 抓出房間中所有的 obj 2023/01/08
OD *get_obj_world args( ( CHAR_DATA *ch, char *argument ) );
OD *get_obj_index_vnum args( (int vnum) );
int count_obj_index_vnum args( (int vnum) );
OD *check_using_wp args( ( CHAR_DATA *ch, int dt , int wield ) ); // 命中裝備判定 2022/12/25
OD *check_using_wp2 args( ( CHAR_DATA *ch, int dt ) ); // 命中裝備判定 2022/12/25
OD *create_money args( ( int amount ) );
int get_obj_number args( ( OBJ_DATA *obj ) );
int get_obj_weight args( ( OBJ_DATA *obj ) );
bool room_is_dark args( ( ROOM_INDEX_DATA *pRoomIndex ) );
//bool room_is_private args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool room_is_private args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) ); // 增加參數 2022/12/20
bool can_see args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool can_see_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool can_drop_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *item_type_name args( ( OBJ_DATA *obj ) );
char *affect_loc_name args( ( int location ) );
char *affect_bit_name args( ( long long int vector ) );
char *r_affect_bit_name args( ( long long int vector ) );
char *wear_bit_name args( (int vector));
char *act_bit_name args((long long int vector));
char *special_bit_name args((int vector));
char *extra_bit_name args( ( int extra_flags ) );
CD *get_char args( ( CHAR_DATA *ch ) );
bool longstring args( ( CHAR_DATA *ch, char *argument ) );
bool authorized args( ( CHAR_DATA *ch, char *skllnm ) );
void end_of_game args( ( void ) );
int	race_lookup args( ( const char *race ) );
OBJ_INDEX_DATA * material_lookup args( ( const char *race ) );
int	affect_lookup args( ( const char *race ) );
int check_exit args ( ( CHAR_DATA * ch, int check ) );	//Added by Razgriz	20050908

/* interp.c */
void interpret args( ( CHAR_DATA *ch, char *argument ) );
void rinterpret args( ( ROOM_INDEX_DATA *room, CHAR_DATA *actor, char *argument ) );
void ointerpret args( ( OBJ_DATA *room, CHAR_DATA *actor, char *argument ) );
bool is_number args( ( char *arg ) );
int number_argument args( ( char *argument, char *arg ) );
char *one_argument args( ( char *argument, char *arg_first ) );
bool IS_SWITCHED args( ( CHAR_DATA *ch ) );
bool check_social args( ( CHAR_DATA *ch, char *command, char *argument ) );

/* mem.c */
void free_affect( AFFECT_DATA* pAf );
QMARK *new_qmark args( (void) );
GROUP_DATA *new_group args((void));
void free_group( GROUP_DATA * g);
QMARK_STRING *new_qmark_string args(( void ));
char *qmark_lookup args( ( const char * str));
OBJ_HP *new_hiddenpower args( (void) );
OBJ_VRACE *new_obj_vrace args((void));
AUC_DATA *new_auction_data (void);
void free_auction_data (AUC_DATA *pAuc);
#ifdef HAS_ROOM_AFFECT
void free_r_affect( R_AFFECT_DATA* pAf );
#endif

/* magic.c */
int new_saves_spell( int pp, CHAR_DATA *victim, int type );
int skill_lookup args( ( const char *name ) );
int weakness_percent args( (CHAR_DATA *ch, int sn) );
int weakness_damage args( (CHAR_DATA *ch, int sn) );
int saves_spell args( ( int level, CHAR_DATA *victim ) );
int old_saves_spell( int pp, CHAR_DATA *victim, int type );
void dm_effect( CHAR_DATA *ch );
void fall_effect( CHAR_DATA *ch );

bool do_autopower (CHAR_DATA * ch, char* arg);
void obj_cast_spell args( ( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj ) );
void room_cast_spell args( ( int sn, int level, CHAR_DATA *victim, CHAR_DATA *ch, ROOM_INDEX_DATA *obj ) );
OD *get_powergem_carry args( ( CHAR_DATA *ch, char *argument ) );

/* mob_prog.c */
#ifdef DUNNO_STRSTR
char *strstr args ( (const char *s1, const char *s2 ) );
#endif

void check_arrival( CHAR_DATA* ch, bool f);
void mprog_wordlist_check args ( ( char * arg, CHAR_DATA *mob, CHAR_DATA* actor, OBJ_DATA* object, void* vo, int type ) );
void mprog_percent_check args ( ( CHAR_DATA *mob, CHAR_DATA* actor, OBJ_DATA* object, void* vo, int type ) );
void oprog_percent_check args ( (OBJ_DATA* object, CHAR_DATA *actor, CHAR_DATA *victim, void *vo, int type));
//void	    mprog_act_trigger	     args ( ( char* buf, CHAR_DATA* mob,
					//CHAR_DATA* ch, OBJ_DATA* obj,
					//void* vo ) );
//void mprog_act_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob );
bool mprog_act_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob );
void oprog_act_trigger(char *txt, CHAR_DATA *ch, OBJ_DATA *mob );

void mprog_bribe_trigger args ( ( CHAR_DATA* mob, CHAR_DATA* ch, int amount ) );
void mprog_entry_trigger args ( ( CHAR_DATA* mob ) );
void mprog_entry_greet_trigger args ( ( CHAR_DATA* mob ) ); // add at 2021/12/18
void mprog_give_trigger args ( ( CHAR_DATA* mob, CHAR_DATA* ch, OBJ_DATA* obj ) );
void mprog_greet_trigger args ( ( CHAR_DATA* mob ) );
void mprog_fight_trigger args ( ( CHAR_DATA* mob, CHAR_DATA* ch ) );
void mprog_hurt_trigger( CHAR_DATA *mob, CHAR_DATA *ch, int dt);
void mprog_hitprcnt_trigger args ( ( CHAR_DATA* mob, CHAR_DATA* ch ) );
void mprog_death_trigger args ( ( CHAR_DATA* actor, CHAR_DATA* mob ) );
void mprog_random_trigger args ( ( CHAR_DATA* mob ) );
void oprog_rand_trigger ( OBJ_DATA *obj);
void oprog_tick_trigger ( OBJ_DATA *obj);
void mprog_tick_trigger args ( ( CHAR_DATA* mob ) );
void mprog_kill_trigger args ( ( CHAR_DATA* mob, CHAR_DATA *ch ) );
void mprog_repop_trigger args ( ( CHAR_DATA* mob ) );
void oprog_repop_trigger args ( ( OBJ_DATA* obj ) ); //add by coconet
void oprog_fight_trigger args ( ( OBJ_DATA* obj ) ); //add at 2022/05/07
void oprog_kill_trigger args ( ( OBJ_DATA* obj ) ); //add at 2022/05/08
bool oprog_cmd_trigger(OBJ_DATA *obj, CHAR_DATA *ch ,char *txt );
void oprog_disappear_trigger args ( ( OBJ_DATA* obj ) ); //add by coconet
void mprog_speech_trigger args ( ( char* txt, CHAR_DATA* mob ) );
void mprog_talk_trigger args ( ( char* txt, CHAR_DATA* ch, CHAR_DATA* mob ) );
void mprog_defun_trigger args ( ( char* txt, CHAR_DATA* ch, CHAR_DATA* mob ) );

void rprog_entry_trigger args ( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch, int door ) );
void rprog_trap_trigger args ( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch, int door ) );
void rprog_leave_trigger args ( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch, int door ) );
void rprog_rand_trigger args ( ( RPROG_DATA *room ) );
void rprog_time_trigger args ( ( RPROG_DATA *room ) );

bool rprog_cmd_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch ,char *txt );
bool rprog_mask_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch ,char *txt );
void rprog_defun_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch ,char *txt );
CHAR_DATA *rget_char_room( ROOM_INDEX_DATA *room, char *argument );
/* mob_commands.c */
QMARK *set_qmark args( ( char *arg ) );
void del_qmark args( ( QMARK *qmark ) );
CHAR_DATA *room_find_char args( ( ROOM_INDEX_DATA *room, char *name) );
CHAR_DATA *world_find_char args( ( ROOM_INDEX_DATA *room, char *name) );
void do_mpgroup args( ( CHAR_DATA *ch, char *argument ) );
bool check_qmark args( ( CHAR_DATA *target, char *arg ) );		//Added by Razgriz	20050701

/* obj_commands.c */
OBJ_DATA *obj_find args( ( OBJ_DATA *pobj, CHAR_DATA *ch, char *argument ) );  //Added by Razgriz	20050921

/* save.c */
void save_char_obj args( ( CHAR_DATA *ch ) );
bool load_char_obj args( ( DESCRIPTOR_DATA *d, char *name ) );
CHAR_DATA *load_horse_obj args( ( DESCRIPTOR_DATA *d, char *name ) );
void save_horse_obj args( ( CHAR_DATA *ch ) );
char *initial args( ( const char *str ) );
void fwrite_bank_obj args(( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest ));
int fread_bank_obj args(( CHAR_DATA *ch, FILE *fp ));
void fwrite_newbank_obj args(( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest ));
int fread_newbank_obj args(( BANK_DATA*ch, FILE *fp ));

/* special.c */
SF *spec_lookup args( ( const char *name ) );
char *spec_string	args( ( SPEC_FUN *fun ) );	/* OLC */

/* update.c */
void advance_level args( ( CHAR_DATA *ch ) );
void demote_level args( ( CHAR_DATA *ch ) );
void gain_exp args( ( CHAR_DATA *ch, int gain ) );
void gain_condition args( ( CHAR_DATA *ch, int iCond, int value ) );
void update_handler args( ( void ) );
void ban_update args( ( void ) );

/* olc.c */
bool run_olc_editor args( ( DESCRIPTOR_DATA *d ) );
char *olc_ed_name args( ( CHAR_DATA *ch ) );
char *olc_ed_vnum args( ( CHAR_DATA *ch ) );

/* bit.c */
extern const struct flag_type	area_flags[];
extern const struct flag_type	sex_flags[];
extern const struct flag_type	     race_flags[];
extern const struct flag_type	exit_flags[];
extern const struct flag_type	door_resets[];
extern const struct flag_type	room_flags[];
extern const struct flag_type	sector_flags[];
extern const struct flag_type	type_flags[];
extern const struct flag_type	extra_flags[];
extern const struct flag_type	wear_flags[];
extern const struct flag_type	act_flags[];
extern const struct flag_type	affect_flags[];
extern const struct flag_type	apply_flags[];
extern const struct flag_type	wear_loc_strings[];
extern const struct flag_type	wear_loc_flags[];
extern const struct flag_type	weapon_flags[];
extern const struct flag_type	container_flags[];
extern const struct flag_type	liquid_flags[];
extern const struct flag_type	mprog_type_flags[];

/* string.c */
void string_edit	args( ( CHAR_DATA *ch, char **pString ) );
void string_append	args( ( CHAR_DATA *ch, char **pString ) );
char *string_replace	args( ( char * orig, char * old, char * new ) );
void string_add	args( ( CHAR_DATA *ch, char *argument ) );
char *format_string	args( ( char *oldstring /*, bool fSpace */ ) );
char *first_arg	args( ( char *argument, char *arg_first, bool fCase ) );
char *string_unpad	args( ( char * argument ) );
char *string_proper	args( ( char * argument ) );
char *all_capitalize	args( ( const char *str ) );

/* olc_save.c */
char *mprog_type_to_name	args ( ( int type ) );
char *rprog_type_to_name	args ( ( int type ) );

#define CLAN_LEVEL 5
#define CLAN_KILL 20
struct clan_data
{
    CLAN_DATA *next;
	char *name;
	char *whoname;
	char *filename;
    char *leader;
    char *founder;
    char *desc;
    char *feud;
    char *ally;
    int dirty;
    int warwin, warlose;
    int score;
    int cclass;
    int hire;
    int construct;
    long long int bank;
    int member[CLAN_LEVEL];
    int crecall;
    int cobj[CLAN_LEVEL];
    int kill[CLAN_KILL];
};

/* call_out_data by zcecil@ROC */
struct call_out_data
{
    CHAR_DATA *owner;
    CALL_OUT_DATA *next;
    char *cmd;
    int timer;
};

/* call_out_data for room by zcecil@ROC */
struct rcall_out_data
{
    ROOM_INDEX_DATA *owner;
    RCALL_OUT_DATA *next;
    char *cmd;
    int timer;
};

/* auction data by zcecil@ROC */
struct auction_data
{
    AUC_DATA *next;
    CHAR_DATA *owner;
    CHAR_DATA *buyer;
    OBJ_DATA *obj;
    int price;
    int time;
    int recall;
};

/*
 * bank_account_data
 * EQ Bank眀腹戈
 * by zcecil@ROC
 */
struct bank_account_data
{
    BANK_DATA *next;
    char *name;
    char *password;
    int amount;
    int recall;
    bool opened;
    OBJ_DATA *objs;
    OBJ_DATA *obj_list;
};

/* area_skill_data
 * 跋办ず﹚м戈
 * by zcecil@ROC
 */
struct area_skill_data
{
    ASKILL_DATA *next;
    int target;
    char *name;
    char *cname;
    char *msg_to_vict;
    char *msg_to_room;
    char *msg_to_char;
};

/* obj_vs_race
 * 猌竟癸疭﹚ race 秸俱
 * by zcecil@ROC
 */
struct obj_vs_race
{
    OBJ_VRACE *next;
    int race;
    int hr_mod_q;
    int hr_mod_p;
    int dr_mod_q;
    int dr_mod_p;
};

/* Do include local header file -- Keric */
#include "skill.h"
#include "gsn.h"
#include "macros.h"

#undef	      CD
#undef	      MID
#undef	      OD
#undef	      OID
#undef	      RID
#undef	      SF

#endif
