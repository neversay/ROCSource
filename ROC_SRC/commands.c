/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,       *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                       *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael       *
 *  Chastain, Michael Quan, and Mitchell Tse.                   *
 *                                       *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.               *
 *                                       *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and       *
 *  Mitchell Tse.                               *
 *                                       *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.       *
 *                                       *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in       *
 *  'license.txt', as well as the Envy license in 'license.nvy'.       *
 *  In particular, you may not remove either of these copyright notices.   *
 *                                       *
 *  Much time and thought has gone into this software and you are       *
 *  benefitting.  We hope that you share your changes too.  What goes       *
 *  around, comes around.                           *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : commands.c                                     *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940704 Razgriz                                *
//*  PURPOSE     :         開放 disconnect 指令給 lv 大於 3 的玩家*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940914 jye                                    *
//*  PURPOSE     :         新增do_mpget mob_commands              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940915 Razgriz                                *
//*  PURPOSE     :         新增 do_mpset_obj_affect               *
//*              :         新增 do_mpremove_obj_affect            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         新增 do_mpmodify_obj_affect            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         新增 do_forceo                         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941010 Razgriz                                *
//*  PURPOSE     :         新增 do_mpforceo                       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941016 Razgriz                                *
//*  PURPOSE     :         修改 backstab 跟 bs 的使用時機         *
//*              :         為 POS_FIGHTING                        *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "ansi.h"

#define LOG_NORMAL        0
#define LOG_ALWAYS        1
#define LOG_NEVER        2

/*
 * God Levels - Check them out in merc.h
*/

#define L_HER            LEVEL_HERO

/*
 * Log-all switch.
 */
bool fLogAll = FALSE;

extern void    do_chatemote    args( ( CHAR_DATA *ch, char *argument) );
//extern void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
extern void do_clanchatemote( CHAR_DATA *ch, char *argument );
extern void do_clanrecall( CHAR_DATA *ch, char *argument );
extern void do_immtalkemote( CHAR_DATA *ch, char *argument);

const struct cmd_type cmd_table[]=
{
    /*
     * Common movement commands.
     */
    { "north",              do_north,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "east",               do_east,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "south",              do_south,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "west",               do_west,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "up",                 do_up,                POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "down",               do_down,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
        /* Adopt mapping to these num-pad commands
    { "8",        do_north,    POS_STANDING,     0,  LOG_NORMAL    },
    { "6",        do_east,    POS_STANDING,     0,  LOG_NORMAL    },
    { "2",        do_south,    POS_STANDING,     0,  LOG_NORMAL    },
    { "4",        do_west,    POS_STANDING,     0,  LOG_NORMAL },
    { "9",        do_up,        POS_STANDING,     0,  LOG_NORMAL },
    { "3",        do_down,    POS_STANDING,    0,  LOG_NORMAL },
    { "5",        do_look,    POS_PARALYZED,   0,  LOG_NORMAL },
        */
    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "buy",                do_buy,             POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "cast",               do_cast,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "cast_delay",         do_cast_delay,      POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "cast_delay猜猜看",   delayed_do_cast,    POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "dice",               do_dice,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "equipment",          do_equipment,       POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "eqbank",             do_eqbank,          POS_RESTING,      0,  LOG_NORMAL, TRUE },
    { "exits",              do_exits,           POS_RESTING,      0,  LOG_NORMAL, TRUE },
    { "get",                do_get,             POS_RESTING,      0,  LOG_NORMAL, TRUE },
    { "inventory",          do_inventory,       POS_DEAD,          0,  LOG_NORMAL, TRUE },
    { "kill",               do_kill,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "look",               do_look,            POS_PARALYZED,     0,  LOG_NORMAL, TRUE },
    { "medi",               do_medi,            POS_STANDING,     0,  LOG_NORMAL, FALSE },
    { "meditate",           do_meditate,        POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "order",              do_order,           POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "quaff",              do_quaff,           POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "rest",               do_rest,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "sleep",              do_sleep,           POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "stand",              do_stand,           POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "talk",               do_talk,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "tell",               do_tell,            POS_DEAD,          0,  LOG_NORMAL, TRUE },
    { "wake",               do_wake,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "wield",              do_wear,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "wizhelp",            do_wizhelp,         POS_DEAD,    L_HER,  LOG_NORMAL, FALSE },

    /*
     * Informational commands.
     */
    { "areas",              do_areas,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "bug",                do_bug,                POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "commands",           do_commands,        POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "compare",            do_compare,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "consider",           do_consider,        POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "credits",            do_credits,            POS_DEAD,          0,  LOG_NORMAL, TRUE },
    { "elescore",           do_elescore,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "examine",            do_examine,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "help",               do_help,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "idea",               do_idea,            POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "report",             do_report,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "pagelength",         do_pagelen,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "read",               do_look,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "score",              do_score,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "skills",             do_slist,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "skadv",              do_skadv,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "skdemote",           do_skdemote,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "slist",              do_slist,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "socials",            do_socials,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "time",               do_time,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "top10",              do_top10,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "typo",               do_typo,            POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "weather",            do_weather,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "who",                do_who,                POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "whois",              do_whois,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "wizlist",            do_wizlist,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "scan",               do_scan,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "scout",              do_scout,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "sector",             do_sector,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    /*
     * Combat commands.
     */
    { "backstab",           do_backstab,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "barehand",           do_barehand,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "bs",                 do_backstab,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "berserk",            do_berserk,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "charge",             do_charge,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "circular",           do_circular,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "circle",             do_circle,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "combat howling",     do_combat_howl,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "disarm",             do_disarm,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "earth slash",        do_earth,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "fee",                do_fee,                POS_FIGHTING,     0,  LOG_NORMAL, FALSE },    
    { "feed",               do_feed,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "flee",               do_flee,            POS_DEAD,          0,  LOG_NORMAL, TRUE },
    { "flaming slash",      do_flaming,         POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "stone form",         do_stone_form,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "storm slash",        do_storm,             POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "sprinkle slash",     do_sprinkle,          POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "fulgurant slash",    do_fulgurant,       POS_FIGHTING,    0,  LOG_NORMAL, TRUE },
    { "icecore slash",      do_icecore,         POS_FIGHTING,    0,  LOG_NORMAL, TRUE },
    { "elemental slash",    do_elemental,         POS_FIGHTING,    0,  LOG_NORMAL, TRUE },
    { "sun slash",          do_sun,             POS_FIGHTING,    0,  LOG_NORMAL, TRUE },
    { "kick",               do_kick,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "hunt",               do_hunt,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "assassination",      do_ass,                POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "frenzy",             do_frenzy,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "lead",               do_lead,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "meteoric arrow",     do_meteoric_arrow,    POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
	{ "rainfall arrow",     do_rainfall_arrow,    POS_FIGHTING,     0,  LOG_NORMAL, TRUE }, // add at 2022/05/02
    { "murder",             do_murder,            POS_FIGHTING,     0,  LOG_ALWAYS, TRUE },
    { "perform",            do_perform,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "rescue",             do_rescue,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "change fighting",    do_change,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "throw",              do_throw,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "stake",              do_stake,            POS_STANDING,     0,  LOG_NORMAL, FALSE },
    { "bashhead",           do_bashhead,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "track",              do_track,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "twin",               do_twin,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "invoke",             do_invoke,            POS_STUNNED,     0,  LOG_NORMAL, TRUE },
    { "dark raid",          do_darkraid,        POS_RESTING,     0,  LOG_NORMAL, TRUE }, // changed to RESTING at 2020/09/14

    /*
     * Configuration commands.
     */
    { "auto",               do_auto,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "autocombat",         do_autocombat,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "autoexit",           do_autoexit,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "autogold",           do_autogold,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "autoloot",           do_autoloot,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "autosac",            do_autosac,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "blank",              do_blank,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "brief",              do_brief,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "channels",           do_channels,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "combine",            do_combine,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "config",             do_config,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
//  { "consent",  do_consent,    POS_DEAD,     0,  LOG_NORMAL    },
    { "description",        do_description,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "password",           do_password,        POS_DEAD,         0,  LOG_NEVER, TRUE },
    { "protect target",     do_protect,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
	{ "cooperate",     do_cooperate,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "prompt",             do_prompt,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "title",              do_title,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "wimpy",              do_wimpy,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "cname",              do_cname,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "hname",              do_hname,            POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "hsummon",            do_hsummon,            POS_STANDING,     0,  LOG_NORMAL, FALSE },

    /*
     * Communication commands.
     */
    { "answer",             do_answer,            POS_SLEEPING,     3,  LOG_NORMAL, TRUE },
    { "auction",            do_auction,            POS_SLEEPING,     3,  LOG_NORMAL, TRUE },
    { "beep",               do_beep,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "chat",               do_chat,            POS_DEAD,         1,  LOG_NORMAL, TRUE },
    { "chat*",              do_chatemote,        POS_DEAD,         3,  LOG_NORMAL, TRUE },
    { ".",                  do_chat,            POS_DEAD,     0,  LOG_NORMAL, TRUE },  // pos_resting 變更為 pos_dead 2022/02/05
    { "emote",              do_emote,            POS_DEAD,     0,  LOG_NORMAL, TRUE }, // pos_resting 變更為 pos_dead 2022/02/05
    { ",",                  do_emote,            POS_DEAD,     0,  LOG_NORMAL, TRUE }, // pos_resting 變更為 pos_dead 2022/02/05
    { "gtell",              do_gtell,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { ";",                  do_gtell,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "herotalk",           do_herotalk,        POS_DEAD,    LEVEL_HERO,  LOG_NORMAL, TRUE },
    { ":",                  do_immtalk,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { ":",                  do_herotalk,        POS_DEAD,    LEVEL_HERO,  LOG_NORMAL, FALSE },
    { "music",              do_music,            POS_SLEEPING,     3,  LOG_NORMAL, TRUE },
    { "note",               do_note,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "pose",               do_pose,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "question",           do_question,        POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "reply",              do_reply,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "say",                do_say,                POS_DEAD,     0,  LOG_NORMAL, TRUE }, // pos_resting 變更為 pos_dead 2022/02/05
    { "'",                  do_say,                POS_DEAD,     0,  LOG_NORMAL, TRUE }, // pos_resting 變更為 pos_dead 2022/02/05
    { "shout",              do_shout,            POS_DEAD,         3,  LOG_NORMAL, TRUE },
    { "yell",               do_yell,            POS_DEAD,         3,  LOG_NORMAL, TRUE },

    /*
     * Object manipulation commands.
     */
    { "adore",              do_adore,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "bandage",            do_bandage,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "brandish",           do_brandish,        POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "close",              do_close,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "donate",             do_donate,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "drink",              do_drink,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "drop",               do_drop,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "destro",             do_destro,            POS_RESTING,     0,  LOG_NORMAL, FALSE },
    { "destroy",            do_destroy,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "eat",                do_eat,                POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "fill",               do_fill,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "give",               do_give,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "hold",               do_wear,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "list",               do_list,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "lock",               do_lock,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "open",               do_open,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "pick",               do_pick,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "put",                do_put,                POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "quaff",              do_quaff,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "recite",             do_recite,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "recover",            do_personal_recover,POS_STANDING,    0,  LOG_NORMAL, TRUE },
    { "remove",             do_remove,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "repair",             do_repair,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "sell",               do_sell,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "take",               do_get,                POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "sacrifice",          do_sacrifice,        POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "unlock",             do_unlock,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "value",              do_value,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "wear",               do_wear,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "zap",                do_zap,                POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "abstraction",        do_abstraction,        POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "power",              do_power,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
	{ "ammo",               do_ammo,             POS_RESTING,     0,  LOG_NORMAL, TRUE }, // add at 2022/04/30
        
    /*
     * Miscellaneous commands.
     */
    { "afk",                do_afk,                POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "bashdoor",           do_bash,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "battle",             do_battle_array,    POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "bid",                do_bet,                POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "backup",             do_backup,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "clan",               do_clan,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "cc",                 do_clanchat,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "cc*",                do_clanchatemote,    POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "\\",                 do_clanrecall,        POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "wager",              do_wager,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "chameleon power",    do_chameleon,        POS_STANDING,     0,  LOG_NORMAL, FALSE },
    { "follow",             do_follow,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "group",              do_group,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "heighten senses",    do_heighten,        POS_STANDING,     0,  LOG_NORMAL, FALSE },
    { "hide",               do_hide,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "poison weapon",      do_poison_weapon,    POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "practice",           do_practice,        POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "ride",               do_ride,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "join",               do_join,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "qui",                do_qui,                POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "quit",               do_quit,            POS_DEAD,         0,  LOG_ALWAYS, TRUE },
    { "recall",             do_recall,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "/",                  do_recall,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "register",           do_register,        POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "rent",               do_rent,            POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "save",               do_save,            POS_DEAD,         0,  LOG_NORMAL, TRUE },
    { "shadow form",        do_shadow,            POS_STANDING,     0,  LOG_NORMAL, FALSE },
    { "sleep",              do_sleep,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "snare",              do_snare,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "sneak",              do_sneak,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "gaze",               do_stare,            POS_FIGHTING,     0,  LOG_NORMAL, TRUE }, // changed to FIGHTING at 19/02/03
    { "nightstalk",         do_nightstalk,        POS_RESTING,    30,  LOG_NORMAL, TRUE }, // changed to RESTING by keelar
    { "spells",             do_spells,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "split",              do_split,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "steal",              do_steal,            POS_STANDING,     0,  LOG_NORMAL, TRUE },
    { "suicid",             do_suicid,            POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "suicide",            do_suicide,            POS_DEAD,         0,  LOG_ALWAYS, TRUE },
    { "hsuicid",            do_hsuicid,            POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "hsuicide",           do_hsuicide,        POS_DEAD,         0,  LOG_NORMAL, FALSE },
    { "train",              do_train,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "untangle",           do_untangle,        POS_FIGHTING,     0,  LOG_NORMAL, TRUE },
    { "visible",            do_visible,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "disperse",           do_disperse,        POS_SLEEPING,     0,  LOG_NORMAL, TRUE },    
    { "wake",               do_wake,            POS_SLEEPING,     0,  LOG_NORMAL, TRUE },
    { "where",              do_where,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "deposit",            do_deposit,            POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "withdraw",           do_withdraw,        POS_RESTING,     0,  LOG_NORMAL, TRUE },
    { "receive",            do_receive,            POS_RESTING,     3,  LOG_NORMAL, TRUE },
    { "store",              do_store,            POS_RESTING,     3,  LOG_NORMAL, TRUE },
    /*
     * Immortal commands.
     */
/*    { "disconnect",    do_disconnect,    POS_DEAD,    L_APP,  LOG_ALWAYS    },    */
    { "disconnect",         do_disconnect,        POS_DEAD,        3,  LOG_ALWAYS, TRUE },    //Modified by Razgriz    20050704
    { "immtalk",            do_immtalk,            POS_DEAD,    L_HER,  LOG_NORMAL, FALSE },
    { "imm*",               do_immtalkemote,    POS_DEAD,     L_HER,  LOG_NORMAL, FALSE },      //add by jye 0940604 imm頻道動作
    { "advance",            do_advance,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "trust",              do_trust,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "addexp",             do_addexp,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "allow",              do_allow,            POS_DEAD,    L_SEN,  LOG_ALWAYS, FALSE },
    { "bansite",            do_ban,                POS_DEAD,    L_SEN,  LOG_ALWAYS, FALSE },
    { "deny",               do_deny,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "force",              do_force,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "forceo",             do_forceo,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },    //Added by Razgriz 20050916
    { "forcer",             do_forcer,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "freeze",             do_freeze,            POS_DEAD,    L_APP,  LOG_ALWAYS, FALSE },
    { "imtlset",            do_imtlset,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "log",                do_log,                POS_DEAD,    L_APP,  LOG_ALWAYS, FALSE },
    { "mset",               do_mset,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "mtlist",             do_mtlist,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "mtstat",             do_mtstat,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "mtset",              do_mtset,            POS_DEAD,    L_SEN,  LOG_ALWAYS, FALSE },
    { "noemote",            do_noemote,            POS_DEAD,    L_APP,  LOG_ALWAYS, FALSE },
    { "notell",             do_notell,            POS_DEAD,    L_APP,  LOG_ALWAYS, FALSE },
    { "numloc",             do_numloc,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "numlock",            do_numlock,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "oload",              do_oload,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "oset",               do_oset,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "pardon",             do_pardon,            POS_DEAD,    L_APP,  LOG_ALWAYS, FALSE },
    { "purge",              do_purge,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "reboo",              do_reboo,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "reboot",             do_reboot,            POS_DEAD,    L_SEN,  LOG_ALWAYS, FALSE },
    { "restore",            do_restore,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "shutdow",            do_shutdow,            POS_DEAD,    L_DIR,  LOG_NORMAL, FALSE },
    { "shutdown",           do_shutdown,        POS_DEAD,    L_DIR,  LOG_ALWAYS, FALSE },
    { "silence",            do_silence,            POS_DEAD,    L_APP,  LOG_ALWAYS, FALSE },
    { "sla",                do_sla,                POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "slay",               do_slay,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "sset",               do_sset,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "sstim",              do_sstim,            POS_DEAD,    L_SEN,  LOG_ALWAYS, FALSE },
    { "sstime",             do_sstime,            POS_DEAD,    L_SEN,  LOG_ALWAYS, FALSE },
    { "transfer",           do_transfer,        POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "users",              do_users,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "wizify",             do_wizify,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "wizlock",            do_wizlock,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "unflag",             do_check_bit,        POS_DEAD,    L_APP,  LOG_NORMAL, FALSE }, // add by amethyst
    { "echo",               do_echo,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "memory",             do_memory,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "mload",              do_mload,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "mfind",              do_mfind,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "mstat",              do_mstat,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "mwhere",             do_mwhere,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "newloc",             do_newloc,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "newlock",            do_newlock,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "ofind",              do_ofind,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "ostat",              do_ostat,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "owhere",             do_owhere,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "peace",              do_peace,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "recho",              do_recho,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "return",             do_return,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "rset",               do_rset,            POS_DEAD,    L_JUN,  LOG_ALWAYS, FALSE },
    { "rstat",              do_rstat,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "slookup",            do_slookup,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "snoop",              do_snoop,            POS_DEAD,    L_JUN,  LOG_NORMAL, FALSE },
    { "switch",             do_switch,            POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "wizinvis",           do_invis,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "at",                 do_at,                POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "bamfin",             do_bamfin,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "bamfout",            do_bamfout,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "goto",               do_goto,            POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "holylight",          do_holylight,        POS_DEAD,    L_APP,  LOG_NORMAL, FALSE },
    { "unbackup",           do_unbackup,        POS_DEAD,    L_SEN,  LOG_NORMAL, FALSE },
    { "cras",               do_cras,            POS_DEAD,    L_DIR,  LOG_NORMAL, FALSE },
    { "crash",              do_crash,            POS_DEAD,    L_DIR,  LOG_NORMAL, FALSE },


    /*
     * MOBprogram commands.
     */
    { "mplog",              do_mplog,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpget",              do_mpget,            POS_DEAD,       0,  LOG_NORMAL, FALSE },//add by jye 0940913
    { "mpcall",             do_mpcall,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpcallout",          do_mpcallout,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mastat",             do_mastat,            POS_DEAD,   L_APP,  LOG_NORMAL, FALSE },
    { "mpstat",             do_mpstat,            POS_DEAD,   L_APP,  LOG_NORMAL, FALSE },
    { "mpasound",           do_mpasound,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpjunk",             do_mpjunk,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpecho",             do_mpecho,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpechoat",           do_mpechoat,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpechoaround",       do_mpechoaround,    POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpnonecho",          do_mpnonecho,    POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpnonechoat",        do_mpnonechoat,    POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpnonechoaround",    do_mpnonechoaround,    POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpechotoall",        do_mpechotoall,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpkill",             do_mpkill,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpmake_horse",       do_mpmake_horse,    POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpmload",            do_mpmload,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpdesc",             do_mpdesc,          POS_DEAD,        0,  LOG_NORMAL },    // add at 2022/11/05
    { "mpmset",             do_mpmset,          POS_DEAD,        0,  LOG_NORMAL },    //for OpenTest only, Added by Razgriz 20050626 , added at 200330
    { "mpnowait",           do_mpnowait,        POS_DEAD,       0,  LOG_NORMAL },
    { "mpoload",            do_mpoload,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mppurge",            do_mppurge,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpgoto",             do_mpgoto,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpat",               do_mpat,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mptransfer",         do_mptransfer,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpforce",            do_mpforce,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpforceo",           do_mpforceo,        POS_DEAD,       0,  LOG_NORMAL, FALSE },    //Added by Razgriz 20051010
    { "mpforcer",           do_mpforcer,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpdef_rembch",       do_mpdef_rembch,    POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpadd_qmark",        do_mpadd_qmark,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpadd_all_qmark",    do_mpadd_all_qmark, POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpdel_qmark",        do_mpdel_qmark,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpdel_all_qmark",    do_mpdel_all_qmark, POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpmod_qmark",        do_mpmod_qmark,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpmod_all_qmark",    do_mpmod_all_qmark, POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpgold",             do_mpgold,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpgreward",          do_mpgreward,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpexp",              do_mpexp,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mppractice",         do_mppractice,        POS_DEAD,       0,  LOG_NORMAL, FALSE }, // add at 20181024 
    { "mpcexp",             do_mpcexp,            POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpmodify",           do_mpmodify,         POS_DEAD,       0,  LOG_NORMAL, FALSE }, // by Amethyst
    { "mpset_obj_affect",   do_mpset_obj_affect,    POS_DEAD,       0,  LOG_NORMAL, FALSE },    //Added by Razgriz 20050915
    { "mpremove_obj_affect",do_mpremove_obj_affect, POS_DEAD,    0,  LOG_NORMAL, FALSE },    //Added by Razgriz 20050915
    { "mpmodify_obj_affect",do_mpmodify_obj_affect,  POS_DEAD,    0,  LOG_NORMAL, FALSE },    //Added by Razgriz 20050916
    { "mpskillexp",         do_mpskillexp,      POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpmerit",            do_mpmerit,         POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpresp",             do_mpresp,          POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpalign",            do_mpalign,         POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "skilldrv",           do_skilldrv,        POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "mpgroup",            do_mpgroup,         POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "rumor",              do_rumor,           POS_DEAD,       0,  LOG_NORMAL, TRUE },
    { "mpdis_magic",        do_mpdis_magic,     POS_DEAD,       0,  LOG_NORMAL, FALSE },
    { "rpstat",             do_rpstat,          POS_DEAD,   L_APP,  LOG_NORMAL, FALSE },
    { "opstat",             do_opstat,          POS_DEAD,   L_APP,  LOG_NORMAL, FALSE },
//    { "stun",       do_mpstun,  POS_DEAD,       0,       LOG_NORMAL }, // by Amethyst
//    { "mpstun",     do_mpfstun, POS_FIGHTING,   0,       LOG_NORMAL }, // by Amethyst
    { "mptrip",             do_mptrip,          POS_DEAD,       0,  LOG_NORMAL, FALSE }, // by Amethyst
    { "selfskill",          do_self_skill,      POS_DEAD,       0,  LOG_NORMAL, FALSE }, // by Amethyst
    { "mpteleport",         do_mpteleport,      POS_DEAD,       0,  LOG_NORMAL, FALSE }, // by Amethyst

    /*
     * OLC 1.1b
     */
    { "aedit",              do_aedit,           POS_DEAD,      31,  LOG_NORMAL, TRUE },
    { "redit",              do_redit,           POS_DEAD,      31,  LOG_NORMAL, TRUE },
    { "oedit",              do_oedit,           POS_DEAD,      31,  LOG_NORMAL, TRUE },
    { "mmedit",             do_medit,           POS_DEAD,      31,  LOG_NORMAL, TRUE },
    { "asave",              do_asave,           POS_DEAD,      31,  LOG_NORMAL, TRUE },
    { "alist",              do_alist,           POS_DEAD,   L_JUN,  LOG_NORMAL, FALSE },
    { "resets",             do_resets,          POS_DEAD,      31,  LOG_NORMAL, TRUE },
    { "mpedit",             do_mpedit,          POS_DEAD,   L_JUN,  LOG_NORMAL, FALSE },

    /*
     * End of list.
     */
    { "",        0,        POS_DEAD,     0,  LOG_NORMAL, FALSE }
};

