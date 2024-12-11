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
#include "fight_const.h"

extern int check_item_magic_flag(OBJ_DATA *, int, int);
extern bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff);
extern bool check_race_special args( ( CHAR_DATA *ch ) );
extern bool check_evade args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
//extern bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff ) );
extern bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn ) ); // 2022/12/04
extern bool check_block args( ( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff ) );
extern bool check_tactics args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

extern void death_cry args( ( CHAR_DATA *ch ) );
extern void group_gain args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
extern int  xp_compute args( ( CHAR_DATA *gch, CHAR_DATA *victim ) );
extern bool is_wielding_poisoned args( ( CHAR_DATA *ch, int wpn ) );
extern void make_corpse args( ( CHAR_DATA *killer, CHAR_DATA *ch ) );
extern int artifact( CHAR_DATA *ch);

extern void check_position( CHAR_DATA *ch, CHAR_DATA *victim, int dam );

/*
 * Local functions.
 */
void dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
void c_dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
int one_hit args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn ) );
////extern int   mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
extern int mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode ); //Modified by Razgriz 20051014
////extern int   brief_mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int* dt, int wpn );

extern void use_magical_item args( ( CHAR_DATA *ch ) );
extern void track_hunt args( (CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door ) );
extern int interpolate1( int level1, int value1, int level2, int value2, int level );
extern void check_corpse_count( CHAR_DATA * ch, CHAR_DATA *victim);
extern int get_align( CHAR_DATA *ch );
extern void show_vs_graphy(CHAR_DATA *ch, CHAR_DATA *victim);
extern void death_declare(CHAR_DATA *ch, CHAR_DATA *victim);
extern void pk_kill(CHAR_DATA *ch, CHAR_DATA *victim);

extern QMARK *qmark_list, *qmark_free;
extern QMARK_STRING *qmark_string_list;
extern CLAN_WAR_DATA *clan_war;

extern char* stat_color1(int cur, int max);

void self_damage( CHAR_DATA *ch, CHAR_DATA *victim, int miss_times,int no_dam_times, int dam_times, int times, int dam, char *skill_name )
{
    const  char   *vp;
    //const  char   *attack;
    //char     buf    [ MAX_STRING_LENGTH ];
    char     buf1   [ 1256 ];
    char     buf2   [ 1256 ];
    char     buf3   [ 1256 ];
    char     buf4   [ 1256 ];
    char     buf5   [ 1256 ];
    char     buf6   [ 256  ];
    char     buf7   [ 256  ];
    char     punct  [ 10   ] = "\0";
    int i=0;
    
    buf6[0] = '\0';
    buf7[0] = '\0';

    for (i=1;;++i)
    {
      	if ( dam <= cedmt[i].min_dmg )
    	{
      	    vp = cedmt[i].desc;
      	    break;
	}
    }

    if( i > 2 )
    {
	sprintf(buf6,"(%d-%d)", ((cedmt[i-1].min_dmg)+1),((cedmt[i].min_dmg)));
    }
    else
    {
	sprintf(buf6,"(%d)", cedmt[i].min_dmg);
    }

    if( miss_times > 0 )
    {
	sprintf( buf1, "§Aªº %d/%d ¦¸%s[1;37m¨S¦³¥´¤¤[m$N.", miss_times, times, skill_name );
      	sprintf( buf2, "$nªº %d/%d ¦¸%s[1;37m¨S¦³¥´¤¤[m§A.", miss_times, times, skill_name );
      	sprintf( buf3, "$nªº %d/%d ¦¸%s[1;37m¨S¦³¥´¤¤[m%s$N"NOR".", miss_times, times, skill_name, stat_color1(victim->hit, victim->max_hit) );
      	sprintf( buf4, "§Aªº %d/%d ¦¸%s[1;37m¨S¦³¥´¤¤[m§A¦Û¤v.", miss_times, times, skill_name );
      	sprintf( buf5, "$nªº %d/%d ¦¸%s[1;37m¨S¦³¥´¤¤[m$m.", miss_times, times, skill_name );

	if ( victim != ch )
	{
	    act( buf1, ch, NULL, victim, TO_CHAR  );
    	    act( buf2, ch, NULL, victim, TO_VICT  );
    	    act( buf3, ch, NULL, victim, TO_NOTVICT );
	}
    	else
    	{
    	    act( buf4, ch, NULL, victim, TO_CHAR  );
    	    act( buf5, ch, NULL, victim, TO_ROOM  );
    	}
    }

    if( no_dam_times > 0 )
    {
    	sprintf(buf7,"(%d)", cedmt[0].min_dmg);
    	sprintf( buf1, "§Aªº %d/%d ¦¸%s[1;37mµLªk¶Ë®`[m$N.", no_dam_times, times, skill_name );
    	sprintf( buf2, "$nªº %d/%d ¦¸%s[1;37mµLªk¶Ë®`[m§A.", no_dam_times, times, skill_name );
    	sprintf( buf3, "$nªº %d/%d ¦¸%s[1;37mµLªk¶Ë®`[m%s$N"NOR".", no_dam_times, times, skill_name, stat_color1(victim->hit, victim->max_hit) );
    	sprintf( buf4, "§Aªº %d/%d ¦¸%s[1;37mµLªk¶Ë®`[m§A.", no_dam_times, times, skill_name );
    	sprintf( buf5, "$nªº %d/%d ¦¸%s[1;37mµLªk¶Ë®`[m$m.", no_dam_times, times, skill_name );

    	if( ch->level > 34 )
    	{
    	    strcat(buf1, buf7);
    	    strcat(buf4, buf7);
    	}

    	if(victim->level > 34)
    	{
    	    strcat(buf2, buf7);
   	}

    	if ( victim != ch )
    	{
    	    act( buf1, ch, NULL, victim, TO_CHAR  );
    	    act( buf2, ch, NULL, victim, TO_VICT  );
    	    act( buf3, ch, NULL, victim, TO_NOTVICT );
    	}
	else
    	{
    	    act( buf4, ch, NULL, victim, TO_CHAR  );
    	   act( buf5, ch, NULL, victim, TO_ROOM  );
    	}
    }

    if( dam_times > 0 )
    {
    	if ( dam <= 24 )
	{
      	    strcat( punct, "." );
      	    sprintf( buf1, "§Aªº %d/%d ¦¸%s%s$N%s", dam_times, times, skill_name, vp, punct );
      	    sprintf( buf2, "$nªº %d/%d ¦¸%s%s§A%s", dam_times, times, skill_name, vp, punct );
      	    sprintf( buf3, "$nªº %d/%d ¦¸%s%s%s$N"NOR"%s", dam_times, times, skill_name, stat_color1(victim->hit, victim->max_hit), vp, punct );
      	    sprintf( buf4, "§Aªº %d/%d ¦¸%s%s§A¦Û¤v%s", dam_times, times, skill_name, vp, punct );
      	    sprintf( buf5, "$nªº %d/%d ¦¸%s%s$m%s", dam_times, times, skill_name, vp, punct );
	}
    	else if ( dam <= 230 )
	{
      	    strcat( punct, "!" );
      	    sprintf( buf1, "§Aªº %d/%d ¦¸%sÅý$N%s%s", dam_times, times, skill_name, vp, punct );
      	    sprintf( buf2, "$nªº %d/%d ¦¸%sÅý§A %s%s", dam_times, times, skill_name, vp, punct );
      	    sprintf( buf3, "$nªº %d/%d ¦¸%sÅý%s$N"NOR"%s%s", dam_times, times, skill_name, 
            IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
      	    sprintf( buf4, "§Aªº %d/%d ¦¸%sÅý§A¦Û¤v %s%s", dam_times, times, skill_name, vp, punct );
      	    sprintf( buf5, "$nªº %d/%d ¦¸%sÅý$m %s%s", dam_times, times, skill_name, vp, punct );
	}
	else
    	{
      	    strcat( punct, "." );
      	    sprintf( buf1, "§Aªº %d/%d ¦¸%s¥´±o$N%s%s", dam_times, times, skill_name, vp, punct );
            sprintf( buf2, "$nªº %d/%d ¦¸%s¥´±o§A %s%s", dam_times, times, skill_name, vp, punct );
            sprintf( buf3, "$nªº %d/%d ¦¸%s¥´±o%s$N"NOR"%s%s", dam_times, times, skill_name,   
            IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
            sprintf( buf4, "§Aªº %d/%d ¦¸%s¥´±o§A¦Û¤v %s%s", dam_times, times, skill_name, vp, punct );
            sprintf( buf5, "$nªº %d/%d ¦¸%s¥´±o$m %s%s", dam_times, times, skill_name, vp, punct );
	}

	if( ch->level > 34 )
	{
	    strcat(buf1, buf6);
	    strcat(buf4, buf6);
	}

	if(victim->level > 34)
	{
	    strcat(buf2, buf6);
	}

	if ( victim != ch )
	{
    	    act( buf1, ch, NULL, victim, TO_CHAR  );
    	    act( buf2, ch, NULL, victim, TO_VICT  );
    	    act( buf3, ch, NULL, victim, TO_NOTVICT );
	}
	else
	{
	    act( buf4, ch, NULL, victim, TO_CHAR  );
	    act( buf5, ch, NULL, victim, TO_ROOM  );
	}
    }

    /*
    }
    if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
     send_to_char(ibuf, ch);
    if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
    send_to_char(ibuf, victim);*/

    return;
}


void brief_e_dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn, int total, int hit )
{
    const  char   *vp;
    const  char   *attack;
    char     buf    [ MAX_STRING_LENGTH ];
    char     buf1   [ 1256 ];
    char     buf2   [ 1256 ];
    char     buf3   [ 1256 ];
    char     buf4   [ 1256 ];
    char     buf5   [ 1256 ];
    char     buf6   [ 256 ];
    char    ibuf    [ 256 ];
    char     punct    [10] = "\0";

    int i=0;

	//char		    buf9	    [ MAX_STRING_LENGTH * 2];
	
    sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR", wpn="HIY"%d"NOR")\n\r", dt, dam, wpn);
    buf6[0] = '\0';

    if ( dam ==   -1 )
    {
    	vp = "[1;37mmisses[m";
    }
    else
    {
	/*mark by Amenda, 00/12/5, the damage message directly reveal the
	*damage number inflect.
	  dam *= 100;
	  if ( victim->hit > 0 )
	  dam /= victim->hit;
	   */

	for(i=0;;++i)
    	{
    	    if( dam <= edmt[i].min_dmg )
    	    {
	    	vp = edmt[i].desc;
	    	break;
	    }
        }

    	if( i > 2 )
    	{
	    sprintf(buf6,"(%d-%d)", ((edmt[i-1].min_dmg)+1),((edmt[i].min_dmg)));
    	}
    	else
    	{
      	    sprintf(buf6,"(%d)", edmt[i].min_dmg);
    	}

    }

    ( dam <= 40 ) ? strcat( punct, "." ) : ( dam <= 220 ) ? strcat( punct, "!" ) : strcat( punct, "!!!" );

    if ( dt == TYPE_HIT )
    {
	if ( ch->race > MAX_RACE )
	{
	    bug( "Dam_message:  %d invalid race", ch->race );
	    ch->race = 0;
	}

	attack = race_table[ ch->race ].dmg_message;

	if( ch->level > 34 )
	{
	    sprintf( buf1, "You %s %d/%d times and %s $N%s%s",attack,hit,total, vp, punct, buf6 );
	    sprintf( buf4, "You %s %d/%d times and %s yourself%s%s", attack,hit,total, vp, punct,buf6 );
	}
	else
	{
      	    sprintf( buf1, "You %s %d/%d times and %s $N%s",      attack,hit,total, vp, punct );
      	    sprintf( buf4, "You %s %d/%d times and %s yourself%s", attack,hit,total, vp, punct );
	}
	
	if( victim->level > 34 )
	{
	    sprintf( buf2, "$n %s %d/%d times and %s you%s%s",   attack,hit,total, vp, punct, buf6 );
	}
	else
	{
	    sprintf( buf2, "$n %s %d/%d times and %s you%s",   attack,hit, total, vp, punct );
	}
	
	sprintf( buf3, "$n %s %d/%d times and %s %s$N"NOR"%s",   attack,hit,total, vp, 
	IS_NPC( victim ) ? HIW : stat_color1(victim->hit, victim->max_hit), punct );
	sprintf( buf5, "$n %s %d/%d times and %s $m%s",  attack,hit,total, vp, punct );
    }
    else
    {
	if ( dt >= 0 && dt < MAX_SKILL )
	{
	    attack  = skill_table[dt].noun_damage;
	}
	else if (    dt >= TYPE_HIT
                  && dt < TYPE_HIT + sizeof( attack_table ) / sizeof( attack_table[0] ) )
        {
            attack  = attack_table[dt - TYPE_HIT];
        }
	else
	{
	    sprintf( buf, "Dam_message: bad dt %d caused by %s.", dt,ch->name );
	    bug( buf, 0 );
            dt = TYPE_HIT;
	    attack  = attack_table[0];
	}

	if ( dt > TYPE_HIT && is_wielding_poisoned( ch, wpn ) )
	{
	    if( ch->level > 34 )
	    {
		sprintf( buf1, "Your poisoned %s hits $N %d/%d times and %s $M%s%s", attack,hit,total, vp, punct, buf6 );
		sprintf( buf4, "Your poisoned %s hits $N %d/%d times and %s you%s%s",attack,hit,total, vp, punct,buf6 );
      	    }
	    else
	    {
		sprintf( buf1, "Your poisoned %s hits $N %d/%d times and %s $M%s",  attack,hit,total, vp, punct );
 		sprintf( buf4, "Your poisoned %s hits $N %d/%d times and %s you%s", attack,hit,total, vp, punct );
	    }

	    if( victim->level > 34 )
	    {
		sprintf( buf2, "$n's poisoned %s hits you %d/%d times and %s you%s%s", attack,hit,total, vp, punct, buf6 );
	    }
	    else
	    {
	    	sprintf( buf2, "$n's poisoned %s hits you %d/%d times and %s you%s", attack,hit,total, vp, punct );
	    }

      	    sprintf( buf3, "$n's poisoned %s hits $N %d/%d times and %s %s$M"NOR"%s", attack,hit,total, vp, 
            IS_NPC( victim ) ? HIW : stat_color1(victim->hit, victim->max_hit), punct );
            sprintf( buf5, "$n's poisoned %s hits $n %d/%d times and %s $m%s",  attack, hit,total,vp, punct );
	}
	else
	{
	    if( ch->level > 34 )
	    {
		sprintf( buf1, "You %s %d/%d times and %s $N%s%s",  attack,hit,total, vp, punct, buf6 );
		sprintf( buf4, "You %s %d/%d times and %s you%s%s", attack,hit,total, vp, punct, buf6 );
	    }
	    else
	    {
		sprintf( buf1, "You %s %d/%d times and %s $N%s",  attack,hit, total, vp, punct );
		sprintf( buf4, "You %s %d/%d times and %s you%s", attack,hit,total, vp, punct );
	    }

      	    if( victim->level > 34 )
      	    {
    		sprintf( buf2, "$n %s %d/%d times and %s you%s%s", attack,hit,total, vp, punct, buf6 );
      	    }
      	    else
      	    {
    		sprintf( buf2, "$n %s %d/%d times and %s you%s", attack,hit,total, vp, punct );
    	    }
            sprintf( buf3, "$n %s %d/%d times and %s %s$N"NOR"%s",  attack,hit,total, vp, 
            IS_NPC( victim ) ? HIW : stat_color1(victim->hit, victim->max_hit), punct );
      	    sprintf( buf5, "$n %s %d/%d times and %s $m%s",  attack,hit,total, vp, punct );
  	}
    }

    if ( victim != ch )
    {
		combat_info( buf1, ch, victim, TO_CHAR  );
		combat_info( buf2, ch, victim, TO_VICT  );
		combat_info( buf3, ch, victim, TO_NOTVICT );
    }
    else
    {
		combat_info( buf4, ch, victim, TO_CHAR  );
		combat_info( buf5, ch, victim, TO_ROOM  );
    }
    if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
    {
    	send_to_char(ibuf, ch);
    }
    if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
    {
    	send_to_char(ibuf, victim);
    }
    return;
}



/* Chinese damage message added by Amenda, 2/25/01 */
void brief_c_dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn, int total, int hit )
{
	const  char   *vp;
	const  char   *attack;
	char     buf    [ MAX_STRING_LENGTH ];
	char     buf1   [ 1256 ];
	char     buf2   [ 1256 ];
	char     buf3   [ 1256 ];
	char     buf4   [ 1256 ];
	char     buf5   [ 1256 ];
	char     buf6   [ 256 ];
	//char     wps    [ 256 ] = "\0";
	char     punct    [10] = "\0";
	char    ibuf[256];
	int i=0;
	//OBJ_DATA  *weapon = NULL;

	sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR", wpn="HIY"%d"NOR")\n\r", dt, dam, wpn);
	buf6[0] = '\0';
	if ( dam == -1 )
	{
		vp = "[1;37m¨S¦³¥´¤¤[m";
	}
	else 
	{
		for(i=0;;++i)
		{
				if( dam <= cedmt[i].min_dmg )
				{
			vp = cedmt[i].desc;
			break;
			}
		}
		if( i > 2 )
		{
			sprintf(buf6,"(%d-%d)", ((cedmt[i-1].min_dmg)+1),( (cedmt[i].min_dmg)));
		}
		else
		{
			sprintf(buf6,"(%d)", cedmt[i].min_dmg);
		}
	}

    if ( dam <= 40 )
    {
		strcat( punct, "." );

		if ( dt == TYPE_HIT )
		{
			if ( ch->race > MAX_RACE )
			{
				bug( "Dam_message:  %d invalid race", ch->race );
				ch->race = 0;
			}
			attack = race_table[ ch->race ].dmg_message;
			sprintf( buf1, "§Aªº %d/%d ¦¸%s%s$N%s",      hit,total,attack, vp, punct );
			sprintf( buf2, "$nªº %d/%d ¦¸%s%s§A%s",  hit,total,attack, vp, punct );
			sprintf( buf3, "$nªº %d/%d ¦¸%s%s%s$N"NOR"%s",   hit,total,attack, stat_color1(victim->hit, victim->max_hit), vp, punct );
			sprintf( buf4, "§Aªº %d/%d ¦¸%s%s§A¦Û¤v%s", hit,total,attack, vp, punct );
			sprintf( buf5, "$nªº %d/%d ¦¸%s%s$m%s",  hit,total,attack, vp, punct );
		}
		else
		{
			if ( dt >= 0 && dt < MAX_SKILL )
			{
				attack  = skill_table[dt].noun_damage;
				sprintf( buf1, "§Aªº %d/%d ¦¸%s%s$N%s",  hit, total, attack, vp, punct );
				sprintf( buf2, "$nªº %d/%d ¦¸%s%s§A %s", hit, total, attack, vp, punct );
				sprintf( buf3, "$nªº %d/%d ¦¸%s%s%s$N"NOR"%s",  hit, total, attack, stat_color1(victim->hit, victim->max_hit), vp, punct );
				sprintf( buf4, "§Aªº %d/%d ¦¸%s%s§A %s", hit, total, attack, vp, punct );
				sprintf( buf5, "$nªº %d/%d ¦¸%s%s$m %s",  hit, total, attack, vp, punct );
			}
			else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof( cattack_table )/ sizeof( cattack_table[0] ) )
			{
				attack  = cattack_table[dt - TYPE_HIT];
				if ( is_wielding_poisoned( ch, wpn ) ){
					sprintf( buf1, "§Aªº %d/%d ¦¸¼@¬r%s%s$N%s", hit, total, attack, vp, punct );
					sprintf( buf2, "$nªº %d/%d ¦¸¼@¬r%s%s§A %s", hit, total, attack, vp, punct );
					sprintf( buf3, "$nªº %d/%d ¦¸¼@¬r%s%s%s$N"NOR"%s", hit, total, attack, 
									IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº %d/%d ¦¸¼@¬r%s%s§A %s", hit, total, attack, vp, punct );
					sprintf( buf5, "$nªº %d/%d ¦¸¼@¬r%s%s$m %s", hit, total, attack, vp, punct );
				}else{
					sprintf( buf1, "§Aªº %d/%d ¦¸%s%s$N%s", hit, total, attack, vp, punct );
					sprintf( buf2, "$nªº %d/%d ¦¸%s%s§A %s", hit, total, attack, vp, punct );
					sprintf( buf3, "$nªº %d/%d ¦¸%s%s%s$N"NOR"%s", hit, total, attack, 
									IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº %d/%d ¦¸%s%s§A %s", hit, total, attack, vp, punct );
					sprintf( buf5, "$nªº %d/%d ¦¸%s%s$m %s", hit, total, attack, vp, punct );
				}
			}else{
				sprintf( buf, "Dam_message: bad dt %d caused by %s.", dt,ch->name );
				bug( buf, 0 );
				dt = TYPE_HIT;
				attack  = cattack_table[0];
			}
		}
	}
	else if ( dam <= 220 )
    {
		strcat( punct, "!" );
		if ( dt == TYPE_HIT )
		{
			if ( ch->race > MAX_RACE ){
				bug( "Dam_message:  %d invalid race", ch->race );
				ch->race = 0;
			}

			attack = race_table[ ch->race ].dmg_message;

			sprintf( buf1, "§Aªº %d/%d ¦¸%sÅý$N%s%s",      hit, total, attack, vp, punct );
			sprintf( buf2, "$nªº %d/%d ¦¸%sÅý§A %s%s",   hit, total, attack, vp, punct );
			sprintf( buf3, "$nªº %d/%d ¦¸%sÅý%s$N"NOR"%s%s",   hit, total, attack, 
							IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
			sprintf( buf4, "§Aªº %d/%d ¦¸%sÅý§A¦Û¤v %s%s", hit, total, attack, vp, punct );
			sprintf( buf5, "$nªº %d/%d ¦¸%sÅý$m %s%s",   hit, total, attack, vp, punct );
		}else{
			if ( dt >= 0 && dt < MAX_SKILL ){
				attack  = skill_table[dt].noun_damage;
				sprintf( buf1, "§Aªº %d/%d ¦¸%sÅý$N%s%s",  hit, total, attack, vp, punct );
				sprintf( buf2, "$nªº %d/%d ¦¸%sÅý§A %s%s", hit, total, attack, vp, punct );
				sprintf( buf3, "$nªº %d/%d ¦¸%sÅý%s$N"NOR"%s%s",  hit, total, attack, 
								IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
				sprintf( buf4, "§Aªº %d/%d ¦¸%sÅý§A¦Û¤v %s%s", hit, total, attack, vp, punct );
				sprintf( buf5, "$nªº %d/%d ¦¸%sÅý$m %s%s",  hit, total, attack, vp, punct );
			}else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof( cattack_table )/sizeof( cattack_table[0] ) ){
				attack  = cattack_table[dt - TYPE_HIT];
				if ( is_wielding_poisoned( ch, wpn ) )
				{
					sprintf( buf1, "§Aªº %d/%d ¦¸¼@¬r%sÅý$N%s%s", hit, total, attack, vp, punct );
					sprintf( buf2, "$nªº %d/%d ¦¸¼@¬r%sÅý§A %s%s", hit, total, attack, vp, punct );
					sprintf( buf3, "$nªº %d/%d ¦¸¼@¬r%sÅý%s$N"NOR"%s%s", hit, total, attack, 
									IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº %d/%d ¦¸¼@¬r%sÅý§A¦Û¤v %s%s", hit, total, attack, vp, punct );
					sprintf( buf5, "$nªº %d/%d ¦¸¼@¬r%sÅý$m %s%s",  hit, total, attack, vp, punct );
				}else{
					sprintf( buf1, "§Aªº %d/%d ¦¸%sÅý$N%s%s", hit, total, attack, vp, punct );
					sprintf( buf2, "$nªº %d/%d ¦¸%sÅý§A %s%s", hit, total, attack, vp, punct );
					sprintf( buf3, "$nªº %d/%d ¦¸%sÅý%s$N"NOR"%s%s", hit, total, attack, 
									IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº %d/%d ¦¸%sÅý§A¦Û¤v %s%s", hit, total, attack, vp, punct );
					sprintf( buf5, "$nªº %d/%d ¦¸%sÅý$m %s%s", hit, total, attack, vp, punct );
				}
			}else{
				sprintf( buf, "Dam_message: bad dt %d caused by %s.", dt,ch->name );
				bug( buf, 0 );
				dt = TYPE_HIT;
				attack  = cattack_table[0];
			}
		}
	}else{
		strcat( punct, "." );
		if ( dt == TYPE_HIT )
		{
			if ( ch->race > MAX_RACE )
			{
			bug( "Dam_message:  %d invalid race", ch->race );
			ch->race = 0;
			}

			attack = race_table[ ch->race ].dmg_message;

			sprintf( buf1, "§Aªº %d/%d ¦¸%s¥´±o$N%s%s",      hit, total, attack, vp, punct );
			sprintf( buf2, "$nªº %d/%d ¦¸%s¥´±o§A %s%s",   hit, total, attack, vp, punct );
			sprintf( buf3, "$nªº %d/%d ¦¸%s¥´±o%s$N"NOR"%s%s", hit, total, attack,   
							IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
			sprintf( buf4, "§Aªº %d/%d ¦¸%s¥´±o§A¦Û¤v %s%s", hit, total, attack, vp, punct );
			sprintf( buf5, "$nªº %d/%d ¦¸%s¥´±o$m %s%s",   hit, total, attack, vp, punct );
		}
		else
		{
			if ( dt >= 0 && dt < MAX_SKILL )
			{
				attack  = skill_table[dt].noun_damage;
				sprintf( buf1, "§Aªº %d/%d ¦¸%s¥´±o$N%s%s",  hit, total, attack, vp, punct );
				sprintf( buf2, "$nªº %d/%d ¦¸%s¥´±o§A %s%s", hit, total, attack, vp, punct );
				sprintf( buf3, "$nªº %d/%d ¦¸%s¥´±o%s$N"NOR"%s%s",  hit, total, attack,
								IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
				sprintf( buf4, "§Aªº %d/%d ¦¸%s¥´±o§A¦Û¤v %s%s", hit, total, attack, vp, punct );
				sprintf( buf5, "$nªº %d/%d ¦¸%s¥´±o$m %s%s",  hit, total, attack, vp, punct );
			}else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof( cattack_table )/sizeof( cattack_table[0] ) ){
				attack  = cattack_table[dt - TYPE_HIT];
				if ( is_wielding_poisoned( ch, wpn ) )
				{
					sprintf( buf1, "§Aªº %d/%d ¦¸¼@¬r%s¥´±o$N%s%s", hit, total, attack, vp, punct );
					sprintf( buf2, "$nªº %d/%d ¦¸¼@¬r%s¥´±o§A %s%s", hit, total, attack, vp, punct );
					sprintf( buf3, "$nªº %d/%d ¦¸¼@¬r%s¥´±o%s$N"NOR"%s%s", hit, total, attack, 
									IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº %d/%d ¦¸¼@¬r%s¥´±o§A¦Û¤v %s%s", hit, total, attack, vp, punct );
					sprintf( buf5, "$nªº %d/%d ¦¸¼@¬r%s¥´±o$m %s%s",  hit, total, attack, vp, punct );
				}else{
					sprintf( buf1, "§Aªº %d/%d ¦¸%s¥´±o$N%s%s", hit, total, attack, vp, punct );
					sprintf( buf2, "$nªº %d/%d ¦¸%s¥´±o§A %s%s", hit, total, attack, vp, punct );
					sprintf( buf3, "$nªº %d/%d ¦¸%s¥´±o%s$N"NOR"%s%s", hit, total, attack, 
									IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº %d/%d ¦¸%s¥´±o§A¦Û¤v %s%s", hit, total, attack, vp, punct );
					sprintf( buf5, "$nªº %d/%d ¦¸%s¥´±o$m %s%s", hit, total, attack, vp, punct );
				}
			}
		}
	}

	if( ch->level > 34 ){
		strcat(buf1, buf6);
		strcat(buf4, buf6);
	}

	if( victim->level > 34 ) {
		strcat(buf2, buf6);
	}

	if ( victim != ch ){
		combat_info( buf1, ch, victim, TO_CHAR );
		combat_info( buf2, ch, victim, TO_VICT );
		combat_info( buf3, ch, victim, TO_NOTVICT );
	}else{
		combat_info( buf4, ch, victim, TO_CHAR );
		combat_info( buf5, ch, victim, TO_ROOM );
	}

	/*
		²¾°£¦¹¬q¾Ô°«±Ô­z 2022/05/04

	weapon = get_eq_char( ch, wpn );

	if ( dt > MAX_SKILL && number_bits( 3 ) == 1 && weapon ){
		strcat( wps, weapon->short_descr );

		if ( is_wielding_poisoned( ch, wpn ) )
		{
				sprintf( buf1, "§A¹B¥Î¼@¬r%s¦V$N²r¯P§ðÀ»%s", wps, punct );
				sprintf( buf2, "$n¹B¥Î¼@¬r%s¦V§A§ðÀ»%s", wps, punct );
				sprintf( buf3, "$n¹B¥Î¼@¬r%s¦V$N²r¯P§ðÀ»%s", wps, punct );
				sprintf( buf4, "§A¹B¥Î¼@¬r%s¦V§A¦Û¤v§ðÀ» %s", wps, punct );
				sprintf( buf5, "$n¹B¥Î¼@¬r%s¦V$m§ðÀ»%s", wps, punct );
		}
		else
		{
				sprintf( buf1, "§A¹B¥Î%s¦V$N²r¯P§ðÀ»%s", wps, punct );
				sprintf( buf2, "$n¹B¥Î%s¦V§A§ðÀ»%s", wps, punct );
				sprintf( buf3, "$n¹B¥Î%s¦V$N²r§ð¦Ó¥h%s", wps, punct );
				sprintf( buf4, "§A¹B¥Î%s¦V§A¦Û¤v§ðÀ» %s", wps, punct );
				sprintf( buf5, "$n¹B¥Î%s¦V$m§ðÀ»%s", wps, punct );
		}
		if ( victim != ch ){
			combat_info( buf1, ch, victim, TO_CHAR );
			combat_info( buf2, ch, victim, TO_VICT );
			combat_info( buf3, ch, victim, TO_NOTVICT );
		}else{
			combat_info( buf4, ch, victim, TO_CHAR  );
			combat_info( buf5, ch, victim, TO_ROOM  );
		}
	}  */

    if (IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL) )
		send_to_char(ibuf, ch);

    if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
		send_to_char(ibuf, victim);

	return;
}




void e_dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn )
{
	const  char   *vp;
	const  char   *attack;
	char     buf    [ MAX_STRING_LENGTH ];
	char     buf1   [ 1256 ];
	char     buf2   [ 1256 ];
	char     buf3   [ 1256 ];
	char     buf4   [ 1256 ];
	char     buf5   [ 1256 ];
	char     buf6   [ 256 ];
	char    ibuf    [ 256 ];
	char     punct    [10] = "\0";
	int i=0;

	sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR", wpn="HIY"%d"NOR")\n\r", dt, dam, wpn);
	buf6[0] = '\0';

	if ( dam == -1 )
	{
		vp = "[1;37mmisses[m";
	}
	else
	{
		for(i=0;;++i){
			if( dam <= edmt[i].min_dmg ){
				vp = edmt[i].desc;
				break;
			}
		}

		if( i > 2 )
			sprintf(buf6,"(%d-%d)", ((edmt[i-1].min_dmg)+1), ((edmt[i].min_dmg)));
		else
			sprintf(buf6,"(%d)", edmt[i].min_dmg);
	}

	( dam <= 40 ) ? strcat( punct, "." ) : ( dam <= 220 ) ? strcat( punct, "!" ) : strcat( punct, "!!!" );

	if ( dt == TYPE_HIT )
	{
		if ( ch->race > MAX_RACE ){
			bug( "Dam_message:  %d invalid race", ch->race );
			ch->race = 0;
		}

		attack = race_table[ ch->race ].dmg_message;

		if ( ch->level > 34 ){
			sprintf( buf1, "Your %s %s $N%s%s",      attack, vp, punct, buf6 );
			sprintf( buf4, "You %s %s yourself%s%s", attack, vp, punct,buf6 );
		}else{
			sprintf( buf1, "Your %s %s $N%s",      attack, vp, punct );
			sprintf( buf4, "You %s %s yourself%s", attack, vp, punct );
		}

		if ( victim->level > 34 )
			sprintf( buf2, "$n's %s %s you%s%s",   attack, vp, punct, buf6 );
		else
			sprintf( buf2, "$n's %s %s you%s",   attack, vp, punct );

		sprintf( buf3, "$n's %s %s %s$N"NOR"%s",   attack, vp, 
		IS_NPC( victim ) ? HIW : stat_color1(victim->hit, victim->max_hit), punct );
		sprintf( buf5, "$n's %s %s $m%s",  attack, vp, punct );
	}
	else
	{
		if ( dt >= 0 && dt < MAX_SKILL ){
			attack  = skill_table[dt].noun_damage;
		}else if(dt >= TYPE_HIT && dt < TYPE_HIT + sizeof( attack_table )/sizeof( attack_table[0] ) ){
			attack  = attack_table[dt - TYPE_HIT];
		}else{
			sprintf( buf, "Dam_message: bad dt %d caused by %s.", dt,ch->name );
			bug( buf, 0 );
			dt = TYPE_HIT;
			attack  = attack_table[0];
		}

		if ( dt > TYPE_HIT && is_wielding_poisoned( ch, wpn ) )
		{
			if( ch->level > 34 )
			{
				sprintf( buf1, "Your poisoned %s %s $N%s%s",  attack, vp, punct, buf6 );
				sprintf( buf4, "Your poisoned %s %s you%s%s", attack, vp, punct,buf6 );
			}else{
				sprintf( buf1, "Your poisoned %s %s $N%s",  attack, vp, punct );
				sprintf( buf4, "Your poisoned %s %s you%s", attack, vp, punct );
			}

			if( victim->level > 34 )
				sprintf( buf2, "$n's poisoned %s %s you%s%s", attack, vp, punct, buf6 );
			else
				sprintf( buf2, "$n's poisoned %s %s you%s", attack, vp, punct );

			sprintf( buf3, "$n's poisoned %s %s %s$N"NOR"%s", attack, vp, 
			IS_NPC( victim ) ? HIW : stat_color1(victim->hit, victim->max_hit), punct );
			sprintf( buf5, "$n's poisoned %s %s $m%s",  attack, vp, punct );
		}
		else
		{
			if( ch->level > 34 )
			{
				sprintf( buf1, "Your %s %s $N%s%s",  attack, vp, punct, buf6 );
				sprintf( buf4, "Your %s %s you%s%s", attack, vp, punct, buf6 );
			}else{
				sprintf( buf1, "Your %s %s $N%s",  attack, vp, punct );
				sprintf( buf4, "Your %s %s you%s", attack, vp, punct );
			}

			if( victim->level > 34 )
				sprintf( buf2, "$n's %s %s you%s%s", attack, vp, punct, buf6 );
			else
				sprintf( buf2, "$n's %s %s you%s", attack, vp, punct );

			sprintf( buf3, "$n's %s %s %s$N"NOR"%s",  attack, vp, 
			IS_NPC( victim ) ? HIW : stat_color1(victim->hit, victim->max_hit), punct );
			sprintf( buf5, "$n's %s %s $m%s",  attack, vp, punct );
		}
	}

	if ( victim != ch ){
		combat_info( buf1, ch, victim, TO_CHAR  );
		combat_info( buf2, ch, victim, TO_VICT  );
		combat_info( buf3, ch, victim, TO_NOTVICT );
	}else{
		combat_info( buf4, ch, victim, TO_CHAR  );
		combat_info( buf5, ch, victim, TO_ROOM  );
	}

	if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
		send_to_char(ibuf, ch);
	if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
		send_to_char(ibuf, victim);
	return;
}

/* Chinese damage message added by Amenda, 2/25/01 */
void c_dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt,int wpn )
{
	const  char   *vp;
	const  char   *attack;
	char     buf    [ MAX_STRING_LENGTH ];
	char     buf1   [ 1256 ];
	char     buf2   [ 1256 ];
	char     buf3   [ 1256 ];
	char     buf4   [ 1256 ];
	char     buf5   [ 1256 ];
	char     buf6   [ 256 ];
	//char     wps    [ 256 ] = "\0";
	char     punct    [10] = "\0";
	char    ibuf[256];
	int i=0;
	//OBJ_DATA  *weapon = NULL;

	sprintf(ibuf, "(dt="HIC"%d"NOR", dam="HIR"%d"NOR", wpn="HIY"%d"NOR")\n\r", dt, dam, wpn);
	buf6[0] = '\0';
	if ( dam ==   -1 )
	{
		vp = "[1;37m¨S¦³¥´¤¤[m";
	}
	else 
	{
		for(i=0;;++i)
		{
			if( dam <= cedmt[i].min_dmg )
			{
				vp = cedmt[i].desc;
				break;
			}
		}

		if( i > 2 )
		{
			sprintf(buf6,"(%d-%d)", ((cedmt[i-1].min_dmg)+1),((cedmt[i].min_dmg)));
		}
		else
		{
			sprintf(buf6,"(%d)", cedmt[i].min_dmg);
		}
	}

	if ( dam <= 40 )
	{
		strcat( punct, "." );

		if ( dt == TYPE_HIT )
		{
			if ( ch->race > MAX_RACE )
			{
				bug( "Dam_message:  %d invalid race", ch->race );
				ch->race = 0;
			}

				attack = race_table[ ch->race ].dmg_message;
				sprintf( buf1, "§Aªº%s%s$N%s",      attack, vp, punct );
				sprintf( buf2, "$nªº%s%s§A%s",   attack, vp, punct );
				sprintf( buf3, "$nªº%s%s%s$N"NOR"%s",  attack, stat_color1(victim->hit, victim->max_hit), vp, punct );
				sprintf( buf4, "§Aªº%s%s§A¦Û¤v%s", attack, vp, punct );
				sprintf( buf5, "$nªº%s%s$m%s",   attack, vp, punct );
		}
		else
		{
			if ( dt >= 0 && dt < MAX_SKILL )
			{
				attack  = skill_table[dt].noun_damage;
				sprintf( buf1, "§Aªº%s%s$N%s",  attack, vp, punct );
				sprintf( buf2, "$nªº%s%s§A %s", attack, vp, punct );
				sprintf( buf3, "$nªº%s%s%s$N"NOR"%s",  attack, stat_color1(victim->hit, victim->max_hit), vp, punct );
				sprintf( buf4, "§Aªº%s%s§A %s", attack, vp, punct );
				sprintf( buf5, "$nªº%s%s$m %s",  attack, vp, punct );
			}
			else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof( cattack_table )/sizeof( cattack_table[0] ) )
			{
				attack  = cattack_table[dt - TYPE_HIT];

				if ( is_wielding_poisoned( ch, wpn ) )
				{
						sprintf( buf1, "§Aªº¼@¬r%s%s$N%s", attack, vp, punct );
						sprintf( buf2, "$nªº¼@¬r%s%s§A %s", attack, vp, punct );
						sprintf( buf3, "$nªº¼@¬r%s%s%s$N"NOR"%s", attack, 
					IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº¼@¬r%s%s§A %s", attack, vp, punct );
					sprintf( buf5, "$nªº¼@¬r%s%s$m %s", attack, vp, punct );
				}
				else
				{
					sprintf( buf1, "§Aªº%s%s$N%s", attack, vp, punct );
						sprintf( buf2, "$nªº%s%s§A %s", attack, vp, punct );
						sprintf( buf3, "$nªº%s%s%s$N"NOR"%s", attack, 
						IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
						sprintf( buf4, "§Aªº%s%s§A %s", attack, vp, punct );
						sprintf( buf5, "$nªº%s%s$m %s", attack, vp, punct );
				}
			}
			else
			{
				sprintf( buf, "Dam_message: bad dt %d caused by %s.", dt,ch->name );
				bug( buf, 0 );
				dt = TYPE_HIT;
				attack  = cattack_table[0];
			}
		}
	}
	else if ( dam <= 220 )
	{
		strcat( punct, "!" );
		if ( dt == TYPE_HIT )
		{
			if ( ch->race > MAX_RACE )
			{
			bug( "Dam_message:  %d invalid race", ch->race );
			ch->race = 0;
				}

			attack = race_table[ ch->race ].dmg_message;

				sprintf( buf1, "§Aªº%sÅý$N%s%s",      attack, vp, punct );
				sprintf( buf2, "$nªº%sÅý§A %s%s",  attack, vp, punct );
				sprintf( buf3, "$nªº%sÅý%s$N"NOR"%s%s",  attack, 
				IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
				sprintf( buf4, "§Aªº%sÅý§A¦Û¤v %s%s", attack, vp, punct );
				sprintf( buf5, "$nªº%sÅý$m %s%s",  attack, vp, punct );
		}
		else
		{
			if ( dt >= 0 && dt < MAX_SKILL )
			{
				attack  = skill_table[dt].noun_damage;
			sprintf( buf1, "§Aªº%sÅý$N%s%s",  attack, vp, punct );
			sprintf( buf2, "$nªº%sÅý§A %s%s", attack, vp, punct );
			sprintf( buf3, "$nªº%sÅý%s$N"NOR"%s%s",  attack,
					 IS_NPC( victim )? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
			sprintf( buf4, "§Aªº%sÅý§A¦Û¤v %s%s", attack, vp, punct );
			sprintf( buf5, "$nªº%sÅý$m %s%s",  attack, vp, punct );
			}
			else if (    dt >= TYPE_HIT
				  && dt < TYPE_HIT + sizeof( cattack_table )/sizeof( cattack_table[0] ) )
			{
				attack  = cattack_table[dt - TYPE_HIT];
				if ( is_wielding_poisoned( ch, wpn ) )
				{
					sprintf( buf1, "§Aªº¼@¬r%sÅý$N%s%s", attack, vp, punct );
					sprintf( buf2, "$nªº¼@¬r%sÅý§A %s%s", attack, vp, punct );
					sprintf( buf3, "$nªº¼@¬r%sÅý%s$N"NOR"%s%s", attack,
							 IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº¼@¬r%sÅý§A¦Û¤v %s%s", attack, vp, punct );
					sprintf( buf5, "$nªº¼@¬r%sÅý$m %s%s", attack, vp, punct );
				}
				else
				{
					sprintf( buf1, "§Aªº%sÅý$N%s%s", attack, vp, punct );
					sprintf( buf2, "$nªº%sÅý§A %s%s", attack, vp, punct );
					sprintf( buf3, "$nªº%sÅý%s$N"NOR"%s%s", attack, 
						 IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
					sprintf( buf4, "§Aªº%sÅý§A¦Û¤v %s%s", attack, vp, punct );
					sprintf( buf5, "$nªº%sÅý$m %s%s", attack, vp, punct );
				}
			}
			else
			{
				sprintf( buf, "Dam_message: bad dt %d caused by %s.", dt,ch->name );
				bug( buf, 0 );
				dt = TYPE_HIT;
				attack  = cattack_table[0];
			}
		}
    }
    else
    {
	strcat( punct, "." );
	if ( dt == TYPE_HIT )
	{
	    if ( ch->race > MAX_RACE )
	    {
		bug( "Dam_message:  %d invalid race", ch->race );
    		ch->race = 0;
      	    }

	    attack = race_table[ ch->race ].dmg_message;

      	    sprintf( buf1, "§Aªº%s¥´±o$N%s%s",      attack, vp, punct );
      	    sprintf( buf2, "$nªº%s¥´±o§A %s%s",  attack, vp, punct );
      	    sprintf( buf3, "$nªº%s¥´±o%s$N"NOR"%s%s", attack,  
        	     IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
      	    sprintf( buf4, "§Aªº%s¥´±o§A¦Û¤v %s%s", attack, vp, punct );
      	    sprintf( buf5, "$nªº%s¥´±o$m %s%s",  attack, vp, punct );
	}
	else
	{
      	    if ( dt >= 0 && dt < MAX_SKILL )
      	    {
    	    	attack  = skill_table[dt].noun_damage;
    		sprintf( buf1, "§Aªº%s¥´±o$N%s%s",  attack, vp, punct );
    		sprintf( buf2, "$nªº%s¥´±o§A %s%s", attack, vp, punct );
    		sprintf( buf3, "$nªº%s¥´±o%s$N"NOR"%s%s",  attack,
      			 IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
		sprintf( buf4, "§Aªº%s¥´±o§A¦Û¤v %s%s", attack, vp, punct );
		sprintf( buf5, "$nªº%s¥´±o$m %s%s",  attack, vp, punct );
	    }
	    else if (    dt >= TYPE_HIT
		      && dt < TYPE_HIT + sizeof( cattack_table )/sizeof( cattack_table[0] ) )
	    {
		attack  = cattack_table[dt - TYPE_HIT];
		if ( is_wielding_poisoned( ch, wpn ) )
		{
		    sprintf( buf1, "§Aªº¼@¬r%s¥´±o$N%s%s", attack, vp, punct );
		    sprintf( buf2, "$nªº¼@¬r%s¥´±o§A %s%s", attack, vp, punct );
		    sprintf( buf3, "$nªº¼@¬r%s¥´±o%s$N"NOR"%s%s", attack, 
		    IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
		    sprintf( buf4, "§Aªº¼@¬r%s¥´±o§A¦Û¤v %s%s", attack, vp, punct );
		    sprintf( buf5, "$nªº¼@¬r%s¥´±o$m %s%s", attack, vp, punct );
		}
		else
		{
		    sprintf( buf1, "§Aªº%s¥´±o$N%s%s", attack, vp, punct );
		    sprintf( buf2, "$nªº%s¥´±o§A %s%s", attack, vp, punct );
		    sprintf( buf3, "$nªº%s¥´±o%s$N"NOR"%s%s", attack, 
		    IS_NPC( victim ) ? WHT : stat_color1(victim->hit, victim->max_hit), vp, punct );
		    sprintf( buf4, "§Aªº%s¥´±o§A¦Û¤v %s%s", attack, vp, punct );
		    sprintf( buf5, "$nªº%s¥´±o$m %s%s", attack, vp, punct );
		}
	    }
	} 
    }
    if( ch->level > 34 ){
		strcat(buf1, buf6);
		strcat(buf4, buf6);
    }
    if(victim->level > 34)
    {
		strcat(buf2, buf6);
    }
    if ( victim != ch )
    {
		combat_info( buf1, ch, victim, TO_CHAR  );
		combat_info( buf2, ch, victim, TO_VICT  );
		combat_info( buf3, ch, victim, TO_NOTVICT );
    }
    else
    {
		combat_info( buf4, ch, victim, TO_CHAR  );
		combat_info( buf5, ch, victim, TO_ROOM  );
    }

	/*
		²¾°£¦¹¬q¾Ô°«±Ô­z 2022/05/04

    weapon = get_eq_char( ch, wpn );

    if ( dt > MAX_SKILL && number_bits( 3 ) == 1 && weapon )
    {
		strcat( wps, weapon->short_descr );

		if ( is_wielding_poisoned( ch, wpn ) )
		{
				sprintf( buf1, "§A¹B¥Î¼@¬r%s¦V$N²r¯P§ðÀ»%s", wps, punct );
				sprintf( buf2, "$n¹B¥Î¼@¬r%s¦V§A§ðÀ»%s", wps, punct );
				sprintf( buf3, "$n¹B¥Î¼@¬r%s¦V$N²r¯P§ðÀ»%s", wps, punct );
				sprintf( buf4, "§A¹B¥Î¼@¬r%s¦V§A¦Û¤v§ðÀ» %s", wps, punct );
				sprintf( buf5, "$n¹B¥Î¼@¬r%s¦V$m§ðÀ»%s", wps, punct );
		}
		else
		{
				sprintf( buf1, "§A¹B¥Î%s¦V$N²r¯P§ðÀ»%s", wps, punct );
				sprintf( buf2, "$n¹B¥Î%s¦V§A§ðÀ»%s", wps, punct );
				sprintf( buf3, "$n¹B¥Î%s¦V$N²r§ð¦Ó¥h%s", wps, punct );
				sprintf( buf4, "§A¹B¥Î%s¦V§A¦Û¤v§ðÀ» %s", wps, punct );
				sprintf( buf5, "$n¹B¥Î%s¦V$m§ðÀ»%s", wps, punct );
		}

		if ( victim != ch )
		{
				combat_info( buf1, ch, victim, TO_CHAR  );
				combat_info( buf2, ch, victim, TO_VICT  );
				combat_info( buf3, ch, victim, TO_NOTVICT );
		}
		else
		{
				combat_info( buf4, ch, victim, TO_CHAR  );
				combat_info( buf5, ch, victim, TO_ROOM  );
		}
    }  */
	
	if( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_VERBOSEALL))
		send_to_char(ibuf, ch);

	if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_VERBOSEALL))
		send_to_char(ibuf, victim);
	return;

}




void brief_dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt,int wpn, int total, int hit )
{
	int todam = dam;
	if( hit > 0 ) todam /= hit;
	if( !is_chinese(ch, victim))
		brief_e_dam_message(ch, victim, todam, dt, wpn, total, hit);
    else
		brief_c_dam_message(ch, victim, todam, dt, wpn, total, hit);
}




void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt,int wpn )
{
	if( !is_chinese(ch, victim))
		e_dam_message(ch, victim, dam, dt, wpn);
	else
		c_dam_message(ch, victim, dam, dt, wpn);
}
