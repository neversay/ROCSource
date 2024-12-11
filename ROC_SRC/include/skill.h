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
#ifndef __ROC_SKILL_H__
#define __ROC_SKILL_H__
/* $Id$ */
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"


/*
 * Local functions.
 */
extern bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff );
//extern bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff );
extern bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn ); // 2022/12/04
extern bool check_block( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff );
extern bool check_tactics( CHAR_DATA *ch, CHAR_DATA *victim );
extern void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn );
extern void	c_dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn );
extern void	death_cry( CHAR_DATA *ch );
extern void	group_gain( CHAR_DATA *ch, CHAR_DATA *victim );
extern int	xp_compute( CHAR_DATA *gch, CHAR_DATA *victim );
extern bool	is_wielding_poisoned( CHAR_DATA *ch, int wpn  );
extern void	make_corpse( CHAR_DATA *killer, CHAR_DATA *ch );
extern int	one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
//extern int 	mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
extern int      mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode ); //Modified by Razgriz 20051014
//extern int 	brief_mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int* dt, int wpn );
extern bool	raw_kill( CHAR_DATA *ch, CHAR_DATA *victim );
extern void	set_fighting( CHAR_DATA *ch, CHAR_DATA *victim );
extern void	disarm( CHAR_DATA *ch, CHAR_DATA *victim );
extern void	trip( CHAR_DATA *ch, CHAR_DATA *victim );
extern bool	check_race_special( CHAR_DATA *ch );
extern void	use_magical_item( CHAR_DATA *ch );
extern void	track_hunt(CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door );
extern void do_lead_assistance(CHAR_DATA *ch, CHAR_DATA *protector);
extern void do_lead_assault(CHAR_DATA *ch, char* argument);
extern void do_lead_ucharge(CHAR_DATA *ch, char* argument);
extern void do_lead_ufire(CHAR_DATA *ch, char* argument);
extern void do_lead_hidemarch(CHAR_DATA *ch, char* argument);
extern void do_lead_envelopment(CHAR_DATA *ch, char* argument);

extern void say_spell	args( ( CHAR_DATA *ch, int sn ) );
extern void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim );
extern void fall_effect( CHAR_DATA *ch );
extern void spell_flaming_armor_strike( int sn, int level, CHAR_DATA *ch, void *vo );
extern int skill_lookup( const char *name );
extern int saves_spell( int pp, CHAR_DATA *victim );
extern int new_saves_spell( int pp, CHAR_DATA *victim, int type );
extern int old_saves_spell( int pp, CHAR_DATA *victim, int type );
extern int new_saves_timer( int pp, CHAR_DATA *victim, int type );
extern double int_effect( CHAR_DATA* ch );
extern double wis_effect( CHAR_DATA* ch );
extern char *target_name;
extern int weakness_percent( CHAR_DATA *ch, int sn);
extern int weakness_damage( CHAR_DATA *ch, int sn);
extern void check_fire_container( OBJ_DATA *con, CHAR_DATA* victim );
#endif
