#ifndef __MACROS_H__
#define __MACROS_H__

/*
 * Utility macros.
 */
#define UMIN( a, b )		    ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UMAX( a, b )		    ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define URANGE( a, b, c )	 ( ( b ) < ( a ) ? ( a )		       \
						: ( ( b ) > ( c ) ? ( c )     \
								  : ( b ) ) )
/*a <= URANGE(a,b,c) <= c */
#define LOWER( c )		  ( ( c ) >= 'A' && ( c ) <= 'Z'		\
						? ( c ) + 'a' - 'A' : ( c ) )
#define UPPER( c )		  ( ( c ) >= 'a' && ( c ) <= 'z'		\
						? ( c ) + 'A' - 'a' : ( c ) )
#define IS_SET( flag, bit )	   ( (flag)?( flag ) & ( bit ) : 0  )
#define SET_BIT( var, bit )	   ( ( var )  |=  ( bit ) )
#define REMOVE_BIT( var, bit )	      ( ( var )	 &= ~( bit ) )
#define NUM_BIT( var )			( 1 << var )
// #define R_NUM_BIT( var )		( var >> 1 )

/*
 * Character macros.
 */
#define IS_NPC( ch )		    ( IS_SET( ( ch )->act, ACT_IS_NPC ) )
#define IS_IMMORTAL( ch )	 ( get_trust( ch ) >= LEVEL_IMMORTAL && \
								!IS_SET( ch->act, PLR_HORSE ) )
#define IS_HERO( ch )		     ( get_trust( ch ) >= LEVEL_HERO	 )
#define IS_AFFECTED( ch, sn )	     ( IS_SET( ( ch )->affected_by, ( sn ) ) )
/*
#define IS_AFFECTED( ch, sn )	     ( ( IS_SET( ( sn ), AFF_MORE ) ) ?\
					( IS_SET( ( ch )->affected_by2, ( sn ) & 1073741823) ) :\
					( IS_SET( ( ch )->affected_by, ( sn ) & 1073741823 ) ) )
*/
//#define IS_AFFECTED2( ch, sn )	     ( IS_SET( ( ch )->affected_by2, ( sn ) ) )

#define GOOD_BASE	300
#define EVIL_BASE	-300
    
#define IS_GOOD( ch )		     ( ch->alignment >=	 GOOD_BASE )
#define IS_EVIL( ch )		     ( ch->alignment <=  EVIL_BASE )
#define IS_NEUTRAL( ch )	( !IS_GOOD( ch ) && !IS_EVIL( ch ) )

#define IS_AWAKE( ch )		      ( ch->position > POS_SLEEPING )
/*#define GET_AC( ch )		    ( ( ( ch )->armor 					\
				    + con_app[get_curr_con( ch )].armor ) )
                                    */

#define IS_OUTSIDE( ch )	       ( (ch)->in_room && !IS_SET(				      \
				    ( ch )->in_room->room_flags,		    \
				    ROOM_INDOORS ) )

//#define WAIT_STATE( ch, pulse ) ( ( ch )->wait = UMAX( ( ch )->wait, (pulse)))

#define MANA_COST(ch,sn) ( skill_table[sn].min_mana )
//#define MANA_COST( ch, sn )	( IS_NPC( ch ) ? 0 : skill_table[sn].min_mana )						      
//#define MANA_COST( ch, sn )	( skill_table[(sn)].min_mana * (IS_NPC((ch)) ? 1 : (class_table[(ch)->class].fMana ? 1 : 2)))						      
/*
#define MANA_COST( ch, sn )	( IS_NPC( ch ) ? 0 : UMAX (		     \
				skill_table[sn].min_mana,		     \
				100 / ( 2 + UMAX ( 0, ch->level -	     \
				skill_table[sn].skill_level[ch->class] ) ) ) )
*/
#define IS_SWITCHED( ch )	( ch->pcdata->switched )

#define INT_DIFF( ch, victim )	( get_curr_int( ch ) - get_curr_int( victim ) )
#define WIS_DIFF( ch, victim )	( get_curr_wis( ch ) - get_curr_wis( victim ) )
#define DEX_DIFF( ch, victim )  ( get_curr_dex( ch ) - get_curr_dex( victim ) )
#define STR_DIFF( ch, victim )  ( get_curr_str( ch ) - get_curr_str( victim ) )
#define CON_DIFF( ch, victim )  ( get_curr_con( ch ) - get_curr_con( victim ) )
/*
 * Object macros.
 */
#define CAN_WEAR( obj, part )	     ( IS_SET( ( obj)->wear_flags,  ( part ) ) )
#define IS_OBJ_STAT( obj, stat )( IS_SET( ( obj)->extra_flags, ( stat ) ) )



/*
 * Description macros.
 *
#define PERS( ch, looker )	( can_see( looker, ( ch ) ) ?	\
	strcat(strcat(strcat(strcpy(name_buf, (ch)->short_descr), "("), (ch)->name), ")") :	\
	"不明人物" )
*/
#define PERS( ch, looker )	( can_see( looker, ( ch ) ) ?	\
	strcat(strcpy(name_buf, (ch)->short_descr), "") :	\
	"不明人物 " )

#endif
