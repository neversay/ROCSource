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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_ghost		);
DECLARE_SPEC_FUN(	spec_cast_judge		);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_psionicist	);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_cast_fire		);
DECLARE_SPEC_FUN(	spec_cast_cold		);
DECLARE_SPEC_FUN(	spec_cast_wind		);
DECLARE_SPEC_FUN(	spec_cast_earth		);
DECLARE_SPEC_FUN(	spec_executioner	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_repairman		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_assassin);



/*
 * Given a name, return the appropriate spec fun.
 */


void special_cast_spell( CHAR_DATA *ch, int sn )
{
    ;
}


/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
	CHAR_DATA *victim;
	int	       sn;

	if ( ch->position != POS_FIGHTING ) return FALSE;

	for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( victim->deleted )
			continue;
		if ( victim->fighting == ch && number_bits( 2 ) == 0 )
			break;
	}

	if ( !victim ) return FALSE;

	if ( ( sn = skill_lookup( spell_name ) ) < 0 ) return FALSE;

	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
	return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
	if ( ch->position != POS_FIGHTING )
		return FALSE;

	switch ( number_bits( 3 ) )
	{
		case 0: return spec_breath_fire ( ch );
		case 1:
		case 2: return spec_breath_lightning ( ch );
		case 3: return spec_breath_gas ( ch );
		case 4: return spec_breath_acid ( ch );
		case 5:
		case 6:
		case 7: return spec_breath_frost ( ch );
	}

	return FALSE;
}



bool spec_breath_acid( CHAR_DATA *ch )
{
    return dragon( ch, "acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, "fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch )
{
    return dragon( ch, "frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch )
{
/*   因毒息修改為指定目標, 所以修正毒息目標 2022/04/29
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL );
    return TRUE;
*/
    return dragon( ch, "gas breath" );
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, "lightning breath" );
}


bool spec_assassin( CHAR_DATA *ch )
{
	int percent;
	char arg[BUF_STRING_LENGTH];
	CHAR_DATA *victim;
	if ( ch->position != POS_FIGHTING ) return FALSE;

	if( !IS_AWAKE(ch) || ch->deleted ) return FALSE;

	if( !(victim = ch->fighting) || victim->deleted || victim->in_room != ch->in_room ) return FALSE;
    
	percent = ch->level + 20;

	if( number_percent() > percent ) return FALSE;

	sprintf(arg, "'%s'", victim->name);
	switch( number_bits(2) )
	{
		case 1:
			stop_fighting(ch, FALSE);
			do_backstab(ch, arg);
			break;
		case 2:
		case 3:
			do_circle(ch, "");
			break;
	}
	return FALSE;
}

bool spec_cast_adept( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	int level;

	if ( !IS_AWAKE( ch ) || ch->fighting )
		return FALSE;

    //if (// IS_AFFECTED( ch, AFF_MUTE )
	//|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	//|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	//|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
//	return FALSE;

	level = UMIN( ch->level, 42 );

	for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
			break;
	}

    if ( !victim || IS_NPC( victim ) )
	return FALSE;
/*
    if ( victim->level > 10 )
    {
	return FALSE;
    }
*/
    switch ( number_bits( 3 ) )
    {
		case 0:
			act( "$n 垂頭喃喃地念起咒語 'tehctah'.", ch, NULL, NULL, TO_ROOM );
			spell_armor( skill_lookup( "armor" ), level, ch, victim );
			return TRUE;

		case 1:
			act( "$n 垂頭喃喃地念起咒語 'nhak'.",    ch, NULL, NULL, TO_ROOM );
			spell_bless( skill_lookup( "bless" ), level, ch, victim );
			return TRUE;

		case 2:
			act( "$n 垂頭喃喃地念起咒語 'yeruf'.",   ch, NULL, NULL, TO_ROOM );
			spell_cure_blindness( skill_lookup( "cure blindness" ),level, ch, victim );
			return TRUE;

		case 3:
			act( "$n 垂頭喃喃地念起咒語 'garf'.",    ch, NULL, NULL, TO_ROOM );
			spell_cure_light( skill_lookup( "cure critical" ),level, ch, victim );
			return TRUE;

		case 4:
			act( "$n 垂頭喃喃地念起咒語 'rozar'.",  ch, NULL, NULL, TO_ROOM );
			spell_cure_poison( skill_lookup( "cure poison" ), level, ch, victim );
			return TRUE;

		case 5:
			if( number_bits(2) == 1 ) {
				act( "$n 垂頭喃喃地念起咒語 'poreny nadroj'.", ch, NULL, NULL, TO_ROOM );
				spell_double_refresh( skill_lookup( "double refresh" ), level, ch, victim );
			}
			else {
				act( "$n 垂頭喃喃地念起咒語 'nadroj'.", ch, NULL, NULL, TO_ROOM );
				spell_refresh( skill_lookup( "refresh" ), level, ch, victim );
			}
			return TRUE;

		case 6:
			act( "$n 垂頭喃喃地念起咒語 'suinoleht'.", ch, NULL, NULL, TO_ROOM );
			spell_combat_mind( skill_lookup( "combat mind" ), level, ch,victim );
			return TRUE;

		case 7:
			act( "$n 垂頭喃喃地念起咒語 'shular'.", ch, NULL, NULL, TO_ROOM );
			spell_fly( skill_lookup( "fly" ), level, ch, victim );
			return TRUE;
    }

	return FALSE;
}

bool spec_cast_water( CHAR_DATA *ch )
{
	CHAR_DATA *victim;

	if ( !IS_AWAKE( ch ) || ch->fighting )
	 return FALSE;

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
		|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

	for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( victim != ch && can_see( ch, victim ) && is_same_group( ch, victim ) )
			break;
	}

	if ( !victim || IS_NPC( victim ) )
		return FALSE;

	switch ( number_bits( 3 ) )
	{
		case 0:
		act( "$n 口中喃喃念著咒文：「tehctah」", ch, NULL, NULL, TO_ROOM );
		spell_armor( skill_lookup( "armor" ), ch->level, ch, victim );
		return TRUE;

		case 1:
		act( "$n 口中喃喃念著咒文：「nhak」",    ch, NULL, NULL, TO_ROOM );
		spell_bless( skill_lookup( "bless" ), ch->level, ch, victim );
		return TRUE;

		case 2:
		act( "$n 口中喃喃念著咒文：「yeruf」",   ch, NULL, NULL, TO_ROOM );
		spell_cure_blindness( skill_lookup( "cure blindness" ), ch->level, ch, victim );
		return TRUE;

		case 3:
		act( "$n 口中喃喃念著咒文：「yekk garf」",    ch, NULL, NULL, TO_ROOM );
		spell_full_heal( skill_lookup( "full heal" ), ch->level, ch, victim );
		return TRUE;

		case 4:
		act( "$n 口中喃喃念著咒文：「rozar」",  ch, NULL, NULL, TO_ROOM );
		spell_cure_poison( skill_lookup( "cure poison" ), ch->level, ch, victim );
		return TRUE;

		case 5:
		act( "$n 口中喃喃念著咒文：「yekk nadroj」", ch, NULL, NULL, TO_ROOM );
		spell_full_refresh( skill_lookup( "full refresh" ), ch->level, ch, victim );
		return TRUE;

		case 6:
		act( "$n 口中喃喃念著咒文：「suinoleht」", ch, NULL, NULL, TO_ROOM );
		spell_combat_mind( skill_lookup( "combat mind" ), ch->level, ch, victim );
		return TRUE;
		case 7:
		act( "$n 口中喃喃念著咒文：「yekk nadroj」", ch, NULL, NULL, TO_ROOM );
		spell_sanctuary( skill_lookup( "sanctuary" ), ch->level, ch, victim );
		return TRUE;
	}

	return FALSE;
}


bool spec_cast_cleric( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	char *spell;
	int sn;

	if ( ch->position != POS_FIGHTING )
		return FALSE;

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
		|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

	for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( victim->fighting == ch && can_see( ch, victim )
			&& number_bits( 2 ) == 0 )
			break;
	}

	if ( !victim )
		return FALSE;

	for ( ; ; )
	{
		int min_level;

		switch ( number_bits( 4 ) )
		{
			case  0: min_level =  0; spell = "blindness"; break;
			case  1: min_level =  3; spell = "cause serious"; break;
			case  2: min_level =  7; spell = "earthquake"; break;
			case  3: min_level =  9; spell = "cause critical"; break;
			case  4: min_level = 10; spell = "dispel evil"; break;
			case  5: min_level = 12; spell = "curse"; break;
			// case  6: min_level = 12; spell = "change sex"; break;
			case  7: min_level = 13; spell = "flamestrike"; break;
			case  8: min_level = 17; spell = "turn undead"; break;
			case  9: min_level = 14; spell = "mute"; break;
			case 10: min_level = 15; spell = "harm"; break;
			default: min_level = 16; spell = "dispel magic"; break;
		}

		if ( ch->level >= min_level )
			break;
	}

	if ( ( sn = skill_lookup( spell ) ) < 0 )
		return FALSE;
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
	return TRUE;
}



bool spec_cast_judge( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->fighting == ch && can_see( ch, victim )
	    && number_bits( 2 ) == 0 )
	    break;
    }

    if ( !victim )
	return FALSE;

    spell = "high explosive";
    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->fighting == ch && can_see( ch, victim )){
	    if( !IS_SET(victim->special, EXPERT) && number_bits(2) == 1 ) break;
	    else if ( IS_SET(victim->special, EXPERT) && number_bits(1) == 1 ) break;
	}
	    
    }

    if ( !victim )
	return FALSE;

    for ( ; ; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";	   break;
	case  1: min_level =  3; spell = "chill touch";	   break;
	case  2: min_level =  7; spell = "weaken";	   break;
	case  3: min_level =  8; spell = "faerie fire";	   break;
	case  4: min_level = 11; spell = "colour spray";   break;
	// case  5: min_level = 12; spell = "change sex";	   break;
	case  6: min_level = 13; spell = "energy drain";   break;
	case  7: min_level = 28; spell = "powerball";	   break;
	case  8: min_level = 32; spell = "starfire";	   break;
	case  9: min_level = 15; spell = "fireball";	   break;
	case 10: min_level = 35; spell = "seal";	   break;
	case 11: min_level = 30; 
		 spell = ((IS_SET(ch->special, EXPERT) ? "whirlwind":"soulsteal"));	   break;
	case 12: min_level = 42;  spell = ((IS_SET(ch->special, EXPERT) ? "polymorph other":"paralyze"));break;
	case 13: min_level = 38; spell = ((IS_SET(ch->special, EXPERT) ? "teleport":"2 fireball"));	   break;
	case 14: min_level = 18; spell = "magic miss";	   break;
	default: min_level = 20; spell = "acid blast";	   break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_earth( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;
/*
    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->fighting == ch && can_see( ch, victim )
	    && number_bits( 1 ) == 0 )
	    break;
    }
*/
    if ( number_bits( 1 ) == 0 && can_see( ch, ch->fighting) )
	    victim = ch->fighting;
    else
    	    return FALSE;

	switch ( number_bits( 3 ) )
	{
	case  0: spell = "paralyze";	   break;
	//case  1: spell = "fireball";	   break;
	case  2: spell = "weaken";	   break;
	case  3: spell = "faerie fire";	   break;
	case  4: spell = "earthqu";    break;
	//case  5: spell = "seal";	   break;
	case  6: spell = "power";       break;
	case  7: spell = "shocking";	   break;
	default: spell = "earthquake";	   break;
	}

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}

bool spec_cast_wind( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;
/*
    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->fighting == ch && can_see( ch, victim )
	    && number_bits( 1 ) == 0 )
	    break;
    }
*/
    if ( number_bits( 1 ) == 0 && can_see( ch, ch->fighting) )
	    victim = ch->fighting;
    else
    	    return FALSE;

	switch ( number_bits( 3 ) )
	{
	case  0: spell = "mute";	   break;
	case  1: spell = "lifesteal";	   break;
	//case  2: spell = "weaken";	   break;
	//case  3: spell = "faerie fire";	   break;
	case  4: spell = "whirlwind";    break;
	case  5: spell = "seal";	   break;
	case  6: spell = "teleport";       break;
	case  7: spell = "lightn";	   break;
	default: spell = "call li";	   break;
	}

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}

bool spec_cast_cold( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	char *spell;
	int sn;

	if ( ch->position != POS_FIGHTING )
	return FALSE;

	if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;
	/*
		for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
		{
		if ( victim->fighting == ch && can_see( ch, victim )
			&& number_bits( 1 ) == 0 )
			break;
		}
	*/
	if ( number_bits( 1 ) == 0 && can_see( ch, ch->fighting) )
		victim = ch->fighting;
	else
			return FALSE;

	switch ( number_bits( 3 ) )
	{
		case  0: spell = "paralyze";	   break;
		//case  1: spell = "fireball";	   break;
		case  2: spell = "weaken";	   break;
		//case  3: spell = "faerie fire";	   break;
		//case  4: spell = "acid blast";    break;
		//case  5: spell = "seal";	   break;
		case  6: spell = "poison";       break;
		case  7: spell = "chill";	   break;
		default: spell = "colour";	   break;
	}

	if ( ( sn = skill_lookup( spell ) ) < 0 )
		return FALSE;
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
	return TRUE;
}

bool spec_cast_fire( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char *spell;
    int sn;

	if ( ch->position != POS_FIGHTING )
		return FALSE;

	if ( IS_AFFECTED( ch, AFF_MUTE )
		|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
		|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
		|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
		return FALSE;
	/*
		for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
		{
		if ( victim->fighting == ch && can_see( ch, victim )
			&& number_bits( 1 ) == 0 )
			break;
		}
	*/
	if ( number_bits( 1 ) == 0 && can_see( ch, ch->fighting) )
		victim = ch->fighting;
	else
			return FALSE;

	switch ( number_bits( 3 ) )
	{
		case  0: spell = "blindness"; break;
		case  1: spell = "magic missile"; break;
		//case  2: spell = "high exp"; break;
		case  3: spell = "faerie fire"; break;
		case  4: spell = "fireball"; break;
		//case  5: spell = "seal";	   break;
		case  6: spell = "star fire"; break;
		case  7: spell = "shocking"; break;
		default: spell = "burning hands"; break;
	}

	if ( ( sn = skill_lookup( spell ) ) < 0 )
		return FALSE;
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
	return TRUE;
}

bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->fighting == ch && can_see( ch, victim )
	    && number_bits( 2 ) == 0 )
	    break;
    }


    if ( !victim )
	return FALSE;

    for ( ; ; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";	   break;
	case  1: min_level =  3; spell = "weaken";	   break;
	case  2: min_level =  6; spell = "chill touch";	   break;
	case  3: min_level =  9; spell = "blindness";	   break;
	case  4: min_level = 12; spell = "poison";	   break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";	   break;
	case  7: min_level = 30; spell =((IS_SET(ch->special, EXPERT) ?  "teleport":"paralyze"));	   break;
	case  8: min_level = 15; spell = "lifesteal";      break;
	case  9: min_level = 15; spell = "energy drain";   break;
	case 10: if ( ch->race == race_lookup( "Vampire" ) )
		 {
		     min_level = 24;
		     spell = "vampiric bite";  break;
		 }
	default: min_level = 24; spell = "gate";	   break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_executioner( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	char *crime;
	char buf [ MAX_STRING_LENGTH ];

	if ( !IS_AWAKE( ch ) || ch->fighting )
		return FALSE;

	crime = "";
	for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( ch->deleted )
			continue;

		if ( victim->deleted )
			continue;

		// 同隊不攻擊判定 2022/11/22
		if ( is_same_group( ch, victim ) )
			continue;

		// 看得見目標才會攻擊 2022/11/22
		if ( !can_see( ch, victim ) )
			continue;

		// safe room 判定 2022/11/22
		if( IS_SET(victim->in_room->room_flags, ROOM_SAFE) || IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
			return;

		if ( !IS_NPC( victim ) && victim->level >= L_APP )
			continue;

		if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER ) )
		{
			crime = "兇手";
			break;
		}

		if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF  ) )
		{
			crime = "小偷";
			break;
		}

		if ( IS_NPC( victim ) && victim->level > 9 && IS_SET( victim->act, ACT_AGGRESSIVE ) )
		{
			crime = "殘暴份子";
			break;
		}
	}

	if ( !victim || IS_SET(victim->in_room->room_flags, ROOM_SAFE) )
		return FALSE;

	sprintf( buf, "%s是個可惡的%s！我要維持正義！！！", victim->short_descr, crime );
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	//char_to_room( create_mobile( get_mob_index( MOB_VNUM_CITYGUARD ) ),
	// ch->in_room );
	// char_to_room( create_mobile( get_mob_index( MOB_VNUM_CITYGUARD ) ),
	// ch->in_room );
	return TRUE;
}



bool spec_fido( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	OBJ_DATA *corpse;
	OBJ_DATA *corpse_next;

	if ( !IS_AWAKE( ch ) )
		return FALSE;

	for ( corpse = ch->in_room->contents; corpse; corpse = corpse_next )
	{
		corpse_next = corpse->next_content;
		if ( corpse->deleted )
			continue;
		if ( corpse->item_type != ITEM_CORPSE_NPC )
			continue;

		act( "$n 啃食著屍體.", ch, NULL, NULL, TO_ROOM );
		for ( obj = corpse->contains; obj; obj = obj_next )
		{
			obj_next = obj->next_content;
			if ( obj->deleted )
				continue;
			obj_from_obj( obj );
			obj_to_room( obj, ch->in_room );
		}
		extract_obj( corpse );
		return TRUE;
	}

	return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
	CHAR_DATA *victim;
	CHAR_DATA *ech;
	CHAR_DATA *vch; // 2022/11/22
	char *crime;
	char buf [ MAX_STRING_LENGTH ];
	//int max_evil;
	int diff_victim = 0;
	int diff_victim_f = 0;

	if ( !IS_AWAKE( ch ) || ch->fighting )
		return FALSE;

	// 修改為判定雙方陣營 2022/11/22
	//max_evil = EVIL_BASE;
	vch      = NULL;
	ech	     = NULL;
	crime    = "";

	for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
	{
		if ( ch->deleted )
			continue;

		if ( victim->deleted )
			continue;

		if ( victim->level >= L_APP )
			continue;

		// 同隊不攻擊判定 2022/11/22
		if ( is_same_group( ch, victim ) )
			continue;

		// 看得見目標才會攻擊 2022/11/22
		if ( !can_see( ch, victim ) )
			continue;

		// safe room 判定 2022/11/22
		if( IS_SET(victim->in_room->room_flags, ROOM_SAFE) || IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
			return;

		if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_KILLER ) )
		{
			vch = victim;
			crime = "兇手";
			break;
		}

		if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_THIEF  ) )
		{
			vch = victim;
			crime = "小偷";
			break;
		}

		/*  修改 guard 幫忙攻擊的判定 2022/11/22
			if ( victim->fighting
				&& victim->fighting != ch
				&& IS_GOOD(victim->fighting)
				&& victim->alignment < victim->fighting->alignment
				&& victim->alignment < max_evil )
			{
				max_evil = victim->alignment;
				ech	     = victim;
			}
		*/
		if ( victim->fighting && victim->fighting != ch )
		{
			diff_victim = ch->alignment - victim->alignment;
			diff_victim_f = ch->alignment - victim->fighting->alignment;
			diff_victim = abs(diff_victim);
			diff_victim_f = abs(diff_victim_f);

			if( IS_GOOD(ch) ) // ch 善良 (guard 善良陣營)
			{
				if( IS_GOOD(victim) ) // victim 為善良
				{
					if( IS_GOOD(victim->fighting) )
					{
						if( victim->fighting->alignment > victim->alignment + 50 )
						{
							ech = victim;
							break;
						}
					}
					continue;
				}else{ // victim 不為善良
					if( IS_GOOD(victim->fighting) ){
						ech = victim;
						break;
					}
					if( IS_EVIL(victim->fighting) )
						continue;

					if( IS_EVIL(victim) )
						diff_victim = diff_victim + 120;

					if( diff_victim_f < diff_victim - 20 ){
						ech = victim;
						break;
					}
				}
			}

			if( IS_EVIL(ch) ) // ch 邪惡 (guard 邪惡陣營)
			{
				if( IS_EVIL(victim) ) // victim 為邪惡
				{
					if( IS_EVIL(victim->fighting) )
					{
						if( victim->fighting->alignment < victim->alignment - 50 )
						{
							ech = victim;
							break;
						}
					}
					continue;
				}else{ // victim 不為邪惡
					if( IS_EVIL(victim->fighting) ){
						ech = victim;
						break;
					}
					if( IS_GOOD(victim->fighting) )
						continue;

					if( IS_GOOD(victim) )
						diff_victim = diff_victim + 120;

					if( diff_victim_f < diff_victim - 20){
						ech = victim;
						break;
					}
				}
			}

			if( IS_NEUTRAL(ch) ) // ch 中立 (guard 中立陣營)
			{
				if( IS_NEUTRAL(victim) ){ // victim 為中立
					if( IS_NEUTRAL(victim->fighting) )
						diff_victim = diff_victim - 50;
					else
						diff_victim = diff_victim - 30;
				}else{ // victim 不為中立
					if( IS_NEUTRAL(victim->fighting) )
						diff_victim = diff_victim + 50;
				}
				if ( diff_victim_f < diff_victim - 20 ){
					ech = victim;
					break;
				}
			}
		}
	}

	// killer thief 判定修改為 do_say 及 將 victim 換為 vch 2022/11/22
	//if ( victim )
	if( vch )
	{
		//sprintf( buf, "%s 是個可惡的 %s！  保護無辜市民！！", victim->name, crime );
		//do_shout( ch, buf );
		//multi_hit( ch, victim, TYPE_UNDEFINED );
		sprintf( buf, "%s 是個可惡的 %s！  保護無辜市民！！", vch->name, crime );
		do_say( ch, buf );
		multi_hit( ch, vch, TYPE_UNDEFINED );
		return TRUE;
	}

	if ( ech )
	{
		// 修改為 do_say 2022/11/22
		//act( "$n 大喊著 '保護無辜市民！！'", ch, NULL, NULL, TO_ROOM );
		do_say( ch , "可惡的傢伙，膽敢在這裡鬧事！" );
		multi_hit( ch, ech, TYPE_UNDEFINED );
		return TRUE;
	}

	return FALSE;
}



bool spec_janitor( CHAR_DATA *ch )
{
	OBJ_DATA *trash;
	OBJ_DATA *trash_next;

	if ( !IS_AWAKE( ch ) )
		return FALSE;

	if( !ch->in_room )
		return FALSE;
	do_sacrifice(ch, "corpse");

	for ( trash = ch->in_room->contents; trash; trash = trash_next )
	{
		trash_next = trash->next_content;
		if ( trash->deleted )
			continue;
		if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) )
			continue;
		if ( trash->item_type == ITEM_DRINK_CON
			|| trash->item_type == ITEM_FOOD
			|| trash->item_type == ITEM_TRASH
			|| trash->cost < 1000 )
		{
			act( "$n 撿起一些垃圾.", ch, NULL, NULL, TO_ROOM );
			obj_from_room( trash );
			obj_to_char( trash, ch );
			return TRUE;
		}
	}

	return FALSE;
}



bool spec_mayor( CHAR_DATA *ch )
{
    static const char *path;
    static const char  open_path  [ ] =
	"W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";
    static const char  close_path [ ] =
	"W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";
    static	 int   pos;
    static	 bool  move;

    if ( !move )
    {
	if ( time_info.hour ==	6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos	 = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos	 = 0;
	}
    }

    if ( ch->fighting )
	return spec_cast_cleric( ch );
    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0' );
	break;

    case 'W':
	ch->position = POS_STANDING;
	act( "$n 醒了過來.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
	act( "$n 躺了下來，進入夢鄉.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
	act( "$n 說道 '嗨！親愛的！'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'b':
	act( "$n 說道 '我得想個辦法整頓垃圾場.'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'c':
	act( "$n 說道 'Vandals!	 Youngsters have no respect for anything!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'd':
	act( "$n 說道 '日安，親愛的市民'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'e':
	act( "$n 說道 'I hereby declare the city of Skara brae open!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'E':
	act( "$n says 'I hereby declare the city of Skara brae closed!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'O':
	do_unlock( ch, "gate" );
	do_open	 ( ch, "gate" );
	break;

    case 'C':
	do_close ( ch, "gate" );
	do_lock	 ( ch, "gate" );
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
	|| !( victim = ch->fighting )
	|| number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "你咬向 $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n 咬了你一口！", ch, NULL, victim, TO_VICT    );
    act( "$n 咬了 $N 一口！",  ch, NULL, victim, TO_NOTVICT );
    spell_poison( gsn_poison, ch->level, ch, victim );
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    int	       gold;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim;
	 victim = victim->next_in_room )
    {
	if ( IS_NPC( victim )
	    || victim->level >= LEVEL_IMMORTAL
	    || number_bits( 3 ) != 0
	    || !can_see( ch, victim ) )	/* Thx Glop */
	    continue;

	/* Thanks to Zeke from MudX for pointing the percent bug */
	if ( IS_AWAKE( victim ) && victim->level > 5
	    && number_percent( ) + victim->level - ch->level >= 33 )
	{
	    act( "你發現 $n 的手正摸著你的錢包！",
		ch, NULL, victim, TO_VICT );
	    act( "$N 發現 $n 的手正摸著$S錢包！",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    gold = victim->gold * number_range( 1, 20 ) / 100;
	    ch->gold	 += 3 * gold / 4;
	    victim->gold -= gold;
	    return TRUE;
	}
    }

    return FALSE;
}


/*
 * Psionicist spec_fun by Thelonius for EnvyMud.
 */
bool spec_cast_psionicist( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_AFFECTED(ch, AFF_SEAL_MAGIC)
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->fighting == ch && can_see( ch, victim )
	    && number_bits( 2 ) == 0 )
	    break;
    }

    if ( !victim )
	return FALSE;

    for ( ; ; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "mind thrust";		 break;
	case  1: min_level =  4; spell = "psychic drain";	 break;
	case  2: min_level =  6; spell = "agitation";		 break;
	case  3: min_level =  8; spell = "psychic crush";	 break;
	case  4: min_level =  9; spell = "project force";	 break;
	case  5: min_level = 13; spell = "ego whip";		 break;
	case  6: min_level = 14; spell = "energy drain";	 break;
	case  7:
	case  8: min_level = 17; spell = "psionic blast";	 break;
	case  9: min_level = 20; spell = "detonate";		 break;
	case 10: min_level = 27; spell = "disintegrate";	 break;
	default: min_level = 25; spell = "ultrablast";		 break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_ghost( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char      *spell;
    int	       sn;

    if	( weather_info.sunlight != SUN_DARK )
    {

      if ( !ch->in_room )
      {
	  bug( "Spec_cast_ghost: NULL in_room.", 0 );
	  return FALSE;
      }

      if ( ch->fighting )
	  stop_fighting( ch, TRUE );

      act( "一道陽光射向 $n, $m 消失了.",
	  ch, NULL, NULL, TO_ROOM);

      extract_char ( ch, TRUE );
      return TRUE;

    }

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_MUTE )
	|| IS_SET( race_table[ch->race].race_abilities, RACE_MUTE )
	|| IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
	return FALSE;

    for ( victim = ch->in_room->people; victim; victim = victim->next_in_room )
    {
	if ( victim->deleted )
	    continue;
	if ( victim->fighting == ch && can_see( ch, victim )
	    && number_bits( 2 ) == 0 )
	    break;
    }

    if ( !victim )
	return FALSE;

    for ( ; ; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";	   break;
	case  1: min_level =  3; spell = "weaken";	   break;
	case  2: min_level =  6; spell = "chill touch";	   break;
	case  3: min_level =  9; spell = "blindness";	   break;
	case  4: min_level = 12; spell = "poison";	   break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";	   break;
	case  7: min_level = 21; spell = ((IS_SET(ch->special, EXPERT) ? "teleport":"paralyze"));	   break;
	default: min_level = 24; spell = "gate";	   break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}


/*
 * spec_fun to repair bashed doors by Thelonius for EnvyMud.
 */
bool spec_repairman( CHAR_DATA *ch )
{
		 EXIT_DATA	 *pexit;
		 EXIT_DATA	 *pexit_rev;
		 ROOM_INDEX_DATA *to_room;
    extern const int		  rev_dir [ ];
		 int		  door;

    if ( !IS_AWAKE( ch ) )
	return FALSE;

    door = number_range( 0, 5 );
    /*
     *	Could search through all doors randomly, but deathtraps would 
     *	freeze the game!  And I'd prefer not to go through from 1 to 6...
     *	too boring.  Instead, just check one direction at a time.  There's
     *	a 51% chance they'll find the door within 4 tries anyway.
     *	-- Thelonius (Monk)
     */
    if ( !( pexit = ch->in_room->exit[door] ) )
	return FALSE;

    if ( IS_SET( pexit->exit_info, EX_BASHED ) )
    {
	REMOVE_BIT( pexit->exit_info, EX_BASHED );
	act( "你修好了 $d.", ch, NULL, pexit->keyword, TO_CHAR );
	act( "$n 修好了 $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* Don't forget the other side! */
	if (   ( to_room   = pexit->to_room		  )
	    && ( pexit_rev = to_room->exit[rev_dir[door]] )
	    && pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_BASHED );

	    for ( rch = to_room->people; rch; rch = rch->next_in_room )
		act( "$d 被重新裝好了.",
		    rch, NULL, pexit_rev->keyword, TO_CHAR );
	}

	return TRUE;
    }

    return FALSE;
}



/*
 * Special Functions Table.	OLC
 */
const	struct	spec_type	spec_table	[ ] =
{
    /*
     * Special function commands.
     */
    { "spec_breath_any",	spec_breath_any		},
    { "spec_breath_acid",	spec_breath_acid	},
    { "spec_breath_fire",	spec_breath_fire	},
    { "spec_breath_frost",	spec_breath_frost	},
    { "spec_breath_gas",	spec_breath_gas		},
    { "spec_breath_lightning",	spec_breath_lightning	},
    { "spec_cast_adept",	spec_cast_adept		},
    { "spec_cast_cleric",	spec_cast_cleric	},
    { "spec_cast_ghost",	spec_cast_ghost		},
    { "spec_cast_judge",	spec_cast_judge		},
    { "spec_cast_mage",		spec_cast_mage		},
    { "spec_cast_psionicist",	spec_cast_psionicist	},
    { "spec_cast_undead",	spec_cast_undead	},
    { "spec_executioner",	spec_executioner	},
    { "spec_fido",		spec_fido		},
    { "spec_guard",		spec_guard		},
    { "spec_janitor",		spec_janitor		},
    { "spec_mayor",		spec_mayor		},
    { "spec_poison",		spec_poison		},
    { "spec_repairman",		spec_repairman		},
    { "spec_thief",		spec_thief		},
    { "spec_cast_fire",		spec_cast_fire		},
    { "spec_cast_cold",		spec_cast_cold		},
    { "spec_cast_wind",		spec_cast_wind		},
    { "spec_cast_fire",		spec_cast_fire},
    { "spec_assassin",		spec_assassin},
    { "spec_cast_earth",		spec_cast_earth		},

    /*
     * End of list.
     */
    { "",			0	}
};



/*****************************************************************************
 Name:		spec_string
 Purpose:	Given a function, return the appropriate name.
 Called by:	<???>
 ****************************************************************************/
char *spec_string( SPEC_FUN *fun )	/* OLC */
{
    int cmd;

    for ( cmd = 0; *spec_table[cmd].spec_fun; cmd++ )	/* OLC 1.1b */
	if ( fun == spec_table[cmd].spec_fun )
	    return spec_table[cmd].spec_name;

    return 0;
}



/*****************************************************************************
 Name:		spec_lookup
 Purpose:	Given a name, return the appropriate spec fun.
 Called by:	do_mset(act_wiz.c) load_specials,reset_area(db.c)
 ****************************************************************************/
SPEC_FUN *spec_lookup( const char *name )	/* OLC */
{
    int cmd;

    for ( cmd = 0; *spec_table[cmd].spec_name; cmd++ )	/* OLC 1.1b */
	if ( !str_cmp( name, spec_table[cmd].spec_name ) )
	    return spec_table[cmd].spec_fun;

    return 0;
}
