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
#include "ansi.h"
/* $Id: spell_cure.c 968 2005-05-14 07:06:59Z keric $ */
/* for vim formatting */
/* vim: sw=8 tw=78 */
extern char *target_name;

extern void	say_spell	args( ( CHAR_DATA *ch, int sn ) );
extern void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim );
extern void fall_effect( CHAR_DATA *ch );
extern int saves_spell( int pp, CHAR_DATA *victim );
extern int new_saves_spell( int pp, CHAR_DATA *victim, int type );
extern int old_saves_spell( int pp, CHAR_DATA *victim, int type );
extern int new_saves_timer( int pp, CHAR_DATA *victim, int type );
extern double int_effect( CHAR_DATA* ch );
extern double wis_effect( CHAR_DATA* ch );
extern void delayed_do_cast( CHAR_DATA *ch, char *argument );
extern void do_cast(CHAR_DATA *ch, char *argument);
extern int weakness_percent( CHAR_DATA *ch, int sn);
extern int weakness_damage( CHAR_DATA *ch, int sn);

void spell_cure_blindness( int sn, int level, CHAR_DATA *ch, void *vo );
void spell_calm( int sn, int level, CHAR_DATA *ch, void *vo );

/* 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
int GetHealUnderBloodyBerserk(CHAR_DATA *ch, int heal)
{
	if(IS_NPC(ch)) return heal;
	if(is_affected(ch, gsn_berserk) && get_skill_level(ch, gsn_blood_thirsty) > 0){
		int nBtSklv = get_skill_level(ch, gsn_blood_thirsty);
		return heal * (9 - nBtSklv) / 10;
	}
	return heal;
}
*/

// lifeshare 分配 heal 2021/11/23
int LifeShareHeal(CHAR_DATA *ch, int heal)
{
	CHAR_DATA *lch;
	if( !is_affected(ch, gsn_lifeshare) ) return heal;
	if( ch->linking == NULL ) return heal;
	lch = ch->linking;
	if( ch->in_room != lch->in_room ) return heal;
	heal = heal - heal/5;
	lch->hit = UMIN( lch->max_hit , lch->hit + heal/5 );
	send_to_char( "微弱的暖流充斥著你的身體.\n\r", lch );
	return heal;
}

long int __power(int x, unsigned int y) 
{
	int i = 0;
	long ret = x;

	if( x <= 1 || y == 1 ) return ret;
	if( y < 1 ) return 1L;

	for( i = y ; i > 1;)
	{
		if( i & 1 ) {// odd
			--i;
			ret *= x;
		}
		else { //even
			ret *= ret;
			i >>= 1;
		}
	}
	return ret;
}

void spell_cure_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int fCured = FALSE;

	if ( is_affected( victim, gsn_blindness ) ){
		affect_strip( victim, gsn_blindness );
		fCured = TRUE;
	}
	if ( is_affected( victim, skill_lookup( "sun flash" ) ) ) {
		affect_strip(victim ,skill_lookup("sun flash"));
		fCured = TRUE;
	}

	if( !fCured ) return;

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你的眼前一亮，你又能看見東西了！\n\r", victim );
	return;
}

void spell_calm( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = NULL;
	int  crazy    = skill_lookup("demon tremor");
    int  fear     = skill_lookup("fear");
	int  sklv     = get_skill_level(ch, sn);
	bool isCalmed = FALSE;
	int  obj_lv   = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}
	
	/* Don't allow charmed mobs to do this, check player's
	 * level */
	if (  IS_NPC( ch ) 
	    && (   IS_AFFECTED( ch, AFF_CHARM ) 
	        || IS_AFFECTED( ch, AFF_SUMMONED ) ) )
	{
		send_to_char( "你失敗了。\n\r", ch);
		return;
	}

	for(victim = ch->in_room->people ; 
	    victim ; 
	    victim = victim->next_in_room )
	{
		if(IS_AFFECTED(victim, AFF_CRAZY)){
			affect_strip(victim, crazy);
			act("你不再感到瘋狂。", ch, NULL, victim, TO_VICT);
		}
		if(is_same_group(ch, victim) && sklv >= 4 && is_affected(victim, fear)){
			affect_strip(victim, fear);
			act("你不再感到恐懼。", ch, NULL, victim, TO_VICT);
		}

		if ( !IS_AWAKE( victim ) )
		{
			continue;
		}

		if( !victim->fighting ) continue;
		isCalmed = FALSE;
		if(is_same_group(ch, victim)){
			if ( (get_curr_wis(ch) * 2 + get_curr_int(ch) + ch->pp + sklv * (4 + sklv)) 
			    * weakness_percent(victim, sn) > number_percent() * 100 ){
				isCalmed = TRUE;
			}
		}else if(  (WIS_DIFF(ch, victim) + INT_DIFF(ch, victim) + 4 ) * 7
		         + saves_spell( ch->pp, victim ) + sklv * 8
		         > (number_percent() * 105 / weakness_percent(victim, sn)) ){
			isCalmed = TRUE;
		}
		if(isCalmed){
			act("$N的表情突然平和下來，動作也跟著減慢。", ch, NULL, victim, TO_NOTVICT);
			act("$N的表情突然平和下來，動作也跟著減慢。", ch, NULL, victim, TO_CHAR);
			act("你的心裡感到一陣平靜。", ch, NULL, victim, TO_VICT);
			stop_fighting( victim, FALSE);		    
			if(ch != victim) WAIT_MAGIC_STATE( victim, 8);
		}
	}
	send_to_char( "好的.\n\r", ch);
	WAIT_MAGIC_STATE( ch, skill_table[gsn_calm].beats * 2);
	return;
}

void spell_vivification( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( !is_affected( victim, gsn_paralyze ) )
		return;

	// paralyze 追加免疫效果, 故追加需帶有 paralyze 才作用 2022/06/14
	if ( !IS_AFFECTED( victim, AFF_PARALYZED ) )
		return;

	affect_strip( victim, gsn_paralyze );

	send_to_char( "好的。\n\r", ch );
	send_to_char( "你的肌肉慢慢鬆弛，你又能活動了！\n\r", victim );
	return;
}

void spell_group_cure_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *rch, *gch;
	bool found = FALSE;
	bool fCured = FALSE;

	for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	{
		if (rch == ch)
			continue;
		if (rch->fighting == ch)
		{
			send_to_char( "你無法在戰鬥中集中精神！\n\r", ch);
			return;
		}
	}

	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) )
			continue;
		if ( gch->class == LORD )
			found = TRUE;
	}

	if ( !found )
	{
		send_to_char( "沒有領主的配合，你無法獨力完成這項艱難的法術。\n\r", ch );
		return;
	}

	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		fCured = FALSE;
		if ( !is_same_group( gch, ch ) )//|| !is_affected( gch, gsn_blindness ))
			continue;

		if( is_affected( gch, gsn_blindness )){
			affect_strip( gch, gsn_blindness );
			fCured = TRUE;
		}
		if( is_affected( gch, skill_lookup("sun flash"))){
			affect_strip(gch, skill_lookup("sun flash"));
			fCured = TRUE;
		}
		if( fCured )
			send_to_char( "你的眼前一亮，你又能看見東西了！\n\r", gch );
	}
	ch->mana += get_skill_level(ch, sn) * 6 + (get_skill_level(ch, sn) - 3) * 2;
	send_to_char( "好的。\n\r", ch );

	return;
}

void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int	 heal;
	int  sklv    = get_skill_level(ch,sn);
	int  obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}
	
	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的高級治療術對他不起作用。\n\r", ch );
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
	return;
	}
	
	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的高級治療術對他不起作用。\n\r", ch );
			send_to_char( "你的身體無法受到治癒。\n\r", victim );
	return;
	}
	
	//heal = dice( 5, 8 ) + level +get_skill_level(ch,sn)*4;
	heal = 40 + UMAX( (3 + sklv/2 ) * victim->max_hit / 100 , 70 * ( 1 + sklv / 6 ));

	/* Keric Modified for different alignment will affect
	 * the heal */
	//if( IS_NPC(ch) ) 
	if( IS_NPC(ch) ) 
	{   
	   /* NPC Will not be affected */
		;
	} else {
		if( IS_GOOD(ch) ) 
		{
			if( IS_GOOD(victim) ) heal = heal*5/4;
			else if ( IS_EVIL(victim) && ch->class != BISHOP ) 
			{
				heal = heal*3/4;
			}
		}	
		else if( IS_NEUTRAL(ch) ) 
		{
			if( IS_NEUTRAL(victim) ) heal = heal*5/4;
		}
		else
		{
			if( IS_EVIL(victim) ) heal = heal*5/4;
			else if( IS_GOOD(victim) && ch->class != BISHOP ) heal = heal*3/4;
		}
	}
	// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
	//heal = GetHealUnderBloodyBerserk(victim, heal);
	heal = LifeShareHeal(victim, heal); // add at 2021/11/23
	victim->hit = UMIN( victim->hit + heal, victim->max_hit );
	victim->move = UMIN( victim->move + heal/2, victim->max_move);
	update_pos( victim );

	if ( ch->class == BISHOP && !is_affected( victim, sn ) && !IS_AFFECTED( victim, AFF_CONTINUE_CURE ))
	{
		af.type	 = sn;
		af.duration	 = sklv * 2 + 5;
		af.modifier	 = 0;
		af.location	 = APPLY_NONE;
		af.bitvector = AFF_CONTINUE_CURE;
		affect_to_char( victim, &af );
	}

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你覺得好多了。\n\r", victim );
	return;
}

void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int	       heal;
	int  sklv    = get_skill_level(ch,sn);
	int  obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}

	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的初級治療術對他不起作用。\n\r", ch );
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
	return;
	}
	
	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的初級治療術對他不起作用。\n\r", ch );
			send_to_char( "你的身體無法受到治癒。\n\r", victim );
	return;
	}
	
	//heal = dice( 1, 8 ) + level / 3+get_skill_level(ch,sn);
	heal = 20 + UMAX( (1 + sklv/2 ) * victim->max_hit / 100 , 50 * ( 1 + sklv / 6 ));

	/* Keric Modified for different alignment will affect the
	 * heal */
	//if( IS_NPC(ch) )
	if( IS_NPC(ch) )
	{  
	   /* NPC Will not be affected */
		;
	} else {
		if( IS_GOOD(ch) )
		{
			if( IS_GOOD(victim) ) heal = heal*5/4;
			else if ( IS_EVIL(victim) && ch->class != BISHOP )
			{
				/*if ( IS_NPC( victim )
				 * && !IS_AFFECTED( victim, AFF_SUMMONED )
				 * && !IS_AFFECTED( victim, AFF_CHARM)
				 * && (victim->race == race_lookup("Vampire")
				 * || victim->race == race_lookup("Undead") ) )
				 * {
				 * int dam = heal;
				 * send_to_char( HIC"你集中精神，散發出神聖的力量！\n\r"NOR"", ch);
				 * act( HIC "當"NOR"$n"HIC"朝著穢物散發出神聖的力量！"NOR"" ,
				 * ch, NULL, NULL, TO_ROOM );
				 * dam = dam * new_saves_spell( ch->pp, victim, MAGIC_GOOD);
				 * magic_damage( ch, victim, dam, sn, WEAR_NONE );
				 * 	return;
				 * 	else{*/
				heal = heal*3/4;
				/*}}*/
			}
		}
		else if( IS_NEUTRAL(ch) )
		{
			if( IS_NEUTRAL(victim) ) heal = heal*5/4;
		}
		else
		{
			if( IS_EVIL(victim) ) heal = heal*5/4;
			else if( IS_GOOD(victim) && ch->class != BISHOP ) heal = heal*3/4;
		}
	}
	// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
	//heal = GetHealUnderBloodyBerserk(victim, heal);
	heal = LifeShareHeal(victim, heal); // add at 2021/11/23
	victim->hit = UMIN( victim->hit + heal, victim->max_hit );
	victim->move = UMIN( victim->move + heal/2, victim->max_move);
	update_pos( victim );
	
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你覺得好一點了。\n\r", victim );
	//WAIT_MAGIC_STATE(ch, 1);
	return;
}

void spell_cure_disease( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( !is_affected( victim, gsn_disease ) )
		return;

	affect_strip( victim, gsn_disease );

	send_to_char( "好的。\n\r", ch );
	send_to_char( "一陣清爽的感覺充滿你全身.\n\r", victim );
	act( "$N 看來好多了.", ch, NULL, victim, TO_NOTVICT );

	return;
}

void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if(    !is_affected( victim, gsn_poison )
		&& !is_affected( victim, gsn_venomous_strike )
		&& !is_affected( victim, gsn_gas_breath) )
		return;

	affect_strip( victim, gsn_poison );
	affect_strip( victim, gsn_venomous_strike );
	if ( is_affected( victim, gsn_gas_breath) )
	{
		if( affected_value(victim, gsn_gas_breath, APPLY_SAVING_POISON) > 0 ){
			AFFECT_DATA af;
			af.type      = gsn_gas_breath;
			af.duration  = 0;
			af.location  = APPLY_SAVING_POISON;
			if( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) || IS_SET( victim->special, NO_POISON ) )
				af.bitvector = 0;
			else
				af.bitvector = AFF_POISON;
			af.modifier  = 0 - 50 - affected_value(victim, gsn_gas_breath, APPLY_SAVING_POISON)/2;
			affect_join( victim, &af );
		}
		if( affected_value(victim, gsn_gas_breath, APPLY_SAVING_POISON) <= 0 )
			affect_strip ( victim, gsn_gas_breath );
	}
	send_to_char( "好的。\n\r", ch );
	send_to_char( "一陣清新的感覺充滿你全身.\n\r", victim );
	act( "$N 看來好多了.", ch, NULL, victim, TO_NOTVICT );

	return;
}

void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int	       heal;
	int  sklv    = get_skill_level(ch,sn);
	int  obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}
	
	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的中級治療術對他不起作用。\n\r", ch );
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
	return;
	}
	
	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的中級治療術對他不起作用。\n\r", ch );
			send_to_char( "你的身體無法受到治癒。\n\r", victim );
	return;
	}
	
	//heal = dice( 3, 8 ) + level / 2 +get_skill_level(ch,sn)*2;
	heal = 30 + UMAX( (2 + sklv/2 ) * victim->max_hit / 100 , 60 * ( 1 + sklv / 6 ));
	
	/* Keric Modified for different alignment will affect the heal */
	//if( IS_NPC(ch) )
	if( IS_NPC(ch) )
	{ 
	  /* NPC Will not be affected */
		;
	} else {
		if( IS_GOOD(ch) )
		{
			if( IS_GOOD(victim) ) heal = heal*5/4;
			else if ( IS_EVIL(victim) && ch->class != BISHOP )
			{
				heal = heal*3/4;
			}
		}
		else if( IS_NEUTRAL(ch) )
		{
			if( IS_NEUTRAL(victim) ) heal = heal*5/4;
		}
		else
		{
			if( IS_EVIL(victim) ) heal = heal*5/4;
			else if( IS_GOOD(victim) && ch->class != BISHOP ) heal = heal*3/4;
		}
	}
	// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
	//heal = GetHealUnderBloodyBerserk(victim, heal);
	heal = LifeShareHeal(victim, heal); // add at 2021/11/23
	victim->hit = UMIN( victim->hit + heal, victim->max_hit );
	victim->move = UMIN( victim->move + heal/2, victim->max_move);
	update_pos( victim );
	send_to_char( "你覺得好多了。\n\r", victim );
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	//WAIT_MAGIC_STATE(ch, 2);
	return;
}

void spell_cont_heal( int sn, int level, CHAR_DATA *ch, void *vo ) // continous heal , gsn_cont_heal
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int  sklv    = get_skill_level(ch,sn);
	int  obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}

	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的持續治療術對他不起作用。\n\r", ch );
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
	return;
	}

	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的持續治療術對他不起作用。\n\r", ch );
			send_to_char( "你的身體無法受到治癒。\n\r", victim );
	return;
	}

	// continous heal 等級判定是否取代技能 2022/04/08
	if( is_affected(victim, sn) ){
		// 如果身上帶有的等級比較高則不取代
		if( affected_value(ch, sn, APPLY_LEVEL ) > 1 + sklv )
			return;
		else
			affect_strip( victim, sn);
	}

	af.type = sn;
	af.duration = 6 + sklv;
	af.location	 = APPLY_LEVEL; // 新增 continous heal 作用效果強度 , 以等級代表 2022/04/08
	af.modifier	 = 1 + sklv;
	af.bitvector = 0;
	affect_to_char(victim, &af);
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "一股和緩的暖流流遍你全身.\n\r", victim );
	return;
}

void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	//    int heal = UMAX( (level+get_skill_level(ch,sn)) * 5 ,
	//    100+get_skill_level(ch,sn)*20);
	//    //modified by coco 0520, for let healing magic will be
	//    influenced by pp and wis
	//    //算式: [法術基礎值] * [wis effect] * [pp effect] * [sklv
	//    effect] 

	double heal; // basic value;
	int    sklv   = get_skill_level(ch,sn);
	int    obj_lv = level;
	
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}
	
	heal = 20 + ch->level * 7 / 4; // lv 41 , 91.75
	
	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的治療術對他不起作用。\n\r", ch );
		send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
		return;
	}
	
	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的治療術對他不起作用。\n\r", ch );
		send_to_char( "你的身體無法受到治癒。\n\r", victim );
		return;
	}
	
	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16)
					 * ( get_curr_wis(ch) - 16 ) / 200;  // lv 41 , wis 23 , 164
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) 
					 * ( 16 - get_curr_wis(ch) ) / 200;
	//pp effect
	if ( !IS_NPC(ch) )
		heal = heal * ( 100 + ((ch->pp > 0) ? ch->pp*2 : 0 ) ) / 100 ;

	// sklv effect
	if( sklv > 0 )
		heal = heal * ( 10 + sklv) / 10 ;

	if( sklv == 5 )
		heal = heal + 50;
	else if( sklv == 6)
		heal = heal + 100;
	
	if( heal < 50 ) heal = 50;  // 給予 heal 基本值 2020/10/22

	/* Keric Modified for different alignment will affect the heal */
	// 加入BISHOP 不受陣營負面影響 2020/10/22
	if( IS_NPC(ch) )
	{   /* NPC Will not be affected */
		;
	} else {
		if( IS_GOOD(ch) )
		{
			if( IS_GOOD(victim) ) heal = heal*5/4;
			else if ( IS_EVIL(victim) && ch->class != BISHOP )
			{
				heal = heal*3/4;
			}
		}
		else if( IS_NEUTRAL(ch) )
		{
			if( IS_NEUTRAL(victim) ) heal = heal*5/4;
		}
		else
		{
			if( IS_EVIL(victim) ) heal = heal*5/4;
			else if( IS_GOOD(victim) && ch->class != BISHOP ) heal = heal*3/4;
		}
	}
	// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
	//heal = GetHealUnderBloodyBerserk(victim, heal);
    heal *= PPL_HP_MULTIPLYER;
	heal = LifeShareHeal(victim, heal); // add at 2021/11/23
	victim->hit = UMIN( victim->hit + heal, victim->max_hit );
	update_pos( victim );

	if ( ch != victim ) send_to_char( "好的。\n\r", ch );
	send_to_char( "一股暖流充斥著你的身體.\n\r", victim );
	return;
}

void spell_double_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

	//modified by coco 0520, for let healing magic will be influenced by pp and wis
	//算式: [法術基礎值] * [wis effect] * [pp effect] * [sklv effect] 
	//    int heal = UMAX( 200+get_skill_level(ch,sn)*40, (level+get_skill_level(ch,sn)) * 10);
	double heal; // basic value;
	int    sklv   = get_skill_level( ch, sn );
	int    obj_lv = level;
	
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}
	// 修改為 spell_heal level base 相同 , 在算式最後乘2  2020/10/22
	// heal = 100 + level * 15 / 4 ;

	heal = 20 + ch->level * 7 / 4; // lv 41 , 91.75

	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的雙重治療術對他不起作用。\n\r", ch );
		send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
		return;
	}
	
	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的雙重治療術對他不起作用。\n\r", ch );
		send_to_char( "你的身體無法受到治癒。\n\r", victim );
		return;
	}

	/*  修改為與 spell_heal 完全一樣  2020/10/22

	//wis effecta
	// 改寫為比較簡略的寫法。 Keric
	heal += heal 
			*(( get_curr_wis(ch) > 16 )? 1 : (-1) )
			*__power( get_curr_wis(ch) - 16, 2 ) / 200;

	// sklv effect
	heal *= ( 1. + ((sklv > 0)? (.1 * (double)sklv): 0.));
	//if( get_skill_level( ch, sn) > 0 )
	//	heal = heal * ( 1. + .1 * get_skill_level( ch,sn ) );

	heal += (sklv > 4)?((sklv == 5)? 80 : 200) : 0;

	// 當 pp < 0 時，一律以 pp 0 來計算。 Keric
	if ( !IS_NPC(ch))
		heal = heal * ( 100 + ((ch->pp > 0)? ch->pp : 0) ) / 100 ;
	if(sklv > 5) heal += ch->pp;
	//if ( get_skill_level(ch,sn) == 5)
	//	heal += 80;
	//else if( get_skill_level(ch,sn) == 6 )
	//	heal += 200;
	*/

	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16)
					 * ( get_curr_wis(ch) - 16 ) / 200;  // lv 41 , wis 23 , 164
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) 
					 * ( 16 - get_curr_wis(ch) ) / 200;
	//pp effect
	if ( !IS_NPC(ch) )
		heal = heal * ( 100 + ((ch->pp > 0) ? ch->pp*2 : 0 ) ) / 100 ;

	// sklv effect
	if( sklv > 0 )
		heal = heal * ( 10 + sklv) / 10 ;

	if( sklv == 5 )
		heal = heal + 50;
	else if( sklv == 6)
		heal = heal + 100;
	
	if( heal < 50 ) heal = 50;  // 給予 heal 基本值 2020/10/22
	
	heal *= 2;  // double heal  2020/10/22
	
	/* Keric Modified for different alignment will affect the heal */
	if( IS_NPC(ch) )
	{   /* NPC Will not be affected */
		;
	} else {
		if( IS_GOOD(ch) )
		{
			if( IS_GOOD(victim) ) heal = heal*5/4;
			else if ( IS_EVIL(victim) )
			{
				heal = heal*3/4;
			}
		}	
		else if( IS_NEUTRAL(ch) )
		{
			if( IS_NEUTRAL(victim) ) heal = heal*5/4;
		}
		else
		{
			if( IS_EVIL(victim) ) heal = heal*5/4;
			else if( IS_GOOD(victim) ) heal = heal*3/4;
		}
	}
	// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
	//heal = GetHealUnderBloodyBerserk(victim, heal);
    heal *= PPL_HP_MULTIPLYER;
	heal = LifeShareHeal(victim, heal); // add at 2021/11/23
	victim->hit = UMIN( victim->hit + heal, victim->max_hit );
	update_pos( victim );

	if ( ch != victim ) send_to_char( "好的。\n\r", ch );
	send_to_char( "一股強大的暖流充斥著你的身體.\n\r", victim );
	return;
}

void spell_group_double_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *rch, *gch;
//  modified by coconet 0520 for let gdb will be influenced by pp and wis and sklv
//    int heal = UMAX( 200+get_skill_level(ch,sn)*40, (level+get_skill_level(ch,sn)) * 8);
//算式: [法術基礎值] * [wis effect] * [pp effect] * [sklv effect] 
	double heal; // basic value;
	int    sklv   = get_skill_level(ch,sn);
	int    nFinalHeal = 0;
	int    nAutoCastSklv = get_skill_level(ch, gsn_concentration);

	heal = 20 + ch->level * 7 / 4; // lv 41 , 91.75

	if (!IS_NPC(ch)) {
		if (nAutoCastSklv < 4 || ch->pcdata->autocombat != AUTOC_CONCENTRATION) {
			for( rch = ch->in_room->people; rch; rch = rch->next_in_room ) {
				if (rch == ch) continue;
				if (rch->fighting == ch) {
					send_to_char( "你無法在戰鬥中集中精神！\n\r", ch);
					return;
				}
			}
		}
	}

	/*  修改為與 spell_mass_heal 相同  2020/10/22
	
    //wis effect
    if ( get_curr_wis(ch) > 16 )
        heal += heal * ( get_curr_wis(ch) - 16) * ( get_curr_wis(ch) - 16 ) / 200;
    else
        heal -= heal * ( 16 - get_curr_wis(ch)) * ( 16 - get_curr_wis(ch) ) / 200;


    // sklv effect
    if( !IS_NPC(ch) && get_skill_level( ch, sn) > 0 )
        heal = heal * ( 10 + get_skill_level( ch, sn )) / 10 ;
    else if ( IS_NPC(ch) )
        heal = heal * 1.4;

    if( get_skill_level(ch,sn) == 5)
        heal += 60;
    else if ( get_skill_level(ch,sn) == 6)
        heal += 150;
    
    //pp effect
    if ( !IS_NPC(ch) )
        heal = heal * ( 100 + ch->pp ) / 100  ;
    
    if(heal < 0) return;

    for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
        if ( !is_same_group( gch, ch ) ) continue;
		
		if ( is_same_group( gch, ch ) && is_affected(gch, gsn_dark_containment) ){
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", gch );
			continue;
		}
		
		if ( is_same_group( gch, ch ) && IS_AFFECTED( gch, AFF_NO_HEAL ) ){
				send_to_char( "你的身體無法受到治癒。\n\r", gch );
			continue;
		}
	*/

	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16)
					 * ( get_curr_wis(ch) - 16 ) / 200;  // lv 41 , wis 23 , 164
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) 
					 * ( 16 - get_curr_wis(ch) ) / 200;

	//pp effect
	if ( !IS_NPC(ch) )
		heal = heal * ( 100 + ((ch->pp > 0) ? ch->pp : 0 ) ) / 100 ;

	// sklv effect
	if( sklv > 0 )
		heal = heal * ( 10 + sklv) / 10 ;

	if( sklv == 5 )
		heal = heal + 50;
	else if( sklv == 6)
		heal = heal + 100;

	if( heal < 50 ) heal = 50;  // 給予 heal 基本值 2020/10/22
	
	heal *= 2;  // double heal  2020/10/22

    for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
        if ( !is_same_group( gch, ch ) ) continue;
		
		if ( is_same_group( gch, ch ) && is_affected(gch, gsn_dark_containment) ){
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", gch );
			continue;
		}
		
		if ( is_same_group( gch, ch ) && IS_AFFECTED( gch, AFF_NO_HEAL ) ){
				send_to_char( "你的身體無法受到治癒。\n\r", gch );
			continue;
		}
		/*   heal 有最基本值, 故移除 2020/10/22
			//add by coconet 20050505
			if( heal < 0 && IS_NPC(gch) ) return;
		*/
		// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
        //nFinalHeal = GetHealUnderBloodyBerserk(gch, heal);
		nFinalHeal = heal;
        nFinalHeal *= PPL_HP_MULTIPLYER;
		// 加入陣營影響  2020/10/22
		if( IS_NPC(ch) )
		{
			;
		} else {
			if( IS_GOOD(ch) )
			{
				if( IS_GOOD(gch) ) nFinalHeal = nFinalHeal*5/4;
				else if ( IS_EVIL(gch) && ch->class != BISHOP )
				{
					nFinalHeal = nFinalHeal*3/4;
				}
			}
			else if( IS_NEUTRAL(ch) )
			{
				if( IS_NEUTRAL(gch) ) nFinalHeal = nFinalHeal*5/4;
			}
			else
			{
				if( IS_EVIL(gch) ) nFinalHeal = nFinalHeal*5/4;
				else if( IS_GOOD(gch) && ch->class != BISHOP ) nFinalHeal = nFinalHeal*3/4;
			}
		}
		nFinalHeal = LifeShareHeal(gch, nFinalHeal); // add at 2021/11/23
        gch->hit = (int)UMIN( gch->hit + nFinalHeal, gch->max_hit );
        update_pos( gch );

        send_to_char( "一股強大的暖流充斥著你的身體.\n\r", gch );
    }
    send_to_char( "好的.\n\r", ch );

    return;
}

void spell_full_heal ( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	// 修改 mana 判定 2022/10/30
	//ch->mana += get_skill_level(ch, sn) * 4 + (get_skill_level(ch, sn) - 3) * 2;

	if ( is_affected(victim, gsn_dark_containment) ){
		if ( ch != victim )
			send_to_char( "你的完全治癒術對他不起作用。\n\r", ch );
		send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", victim );
		return;
	}
	
	if ( IS_AFFECTED(victim, AFF_NO_HEAL ) ){
		if ( ch != victim )
			send_to_char( "你的完全治癒術對他不起作用。\n\r", ch );
		send_to_char( "你的身體無法受到治癒。\n\r", victim );
		return;
	}

	// victim->hit = UMIN(victim->hit + 6000,victim->max_hit);  marked by coconet 0520
    victim->hit = victim->max_hit;
    update_pos( victim );
    if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
    send_to_char( "你身上所受的傷回復了！\n\r", victim );
    WAIT_MAGIC_STATE(ch, 12 - get_skill_level(ch, sn)*2);
    return;
}


void spell_group_full_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *rch, *gch;
	char buf [MAX_INPUT_LENGTH];

	// 修改 mana 判定 2022/10/30
	//ch->mana += get_skill_level(ch, sn) * 6 + (get_skill_level(ch, sn) - 3) * 3;

	for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	{
		if (rch == ch)
			continue;
		if (rch->fighting == ch)
		{
			send_to_char( "你無法在戰鬥中集中精神！\n\r", ch);
			return;
		}
	}

	if (  is_qmark(ch, "spell_group_full_heal") ){
		send_to_char( "你還沒從上次施法的損耗中回復過來。\n\r", ch );
		return;
	}

	sprintf( buf, "self spell_group_full_heal %d 0 0 0 0 1", 24 - get_skill_level(ch,sn)*3 );
	do_add_qmark( ch, buf );


	act( ""HIP"一道溫暖的光芒從"NOR"$n"HIP"全身發出，籠罩整個隊伍！"NOR"", ch, NULL, NULL, TO_ROOM );
	act( ""HIP"一道溫暖的光芒從"NOR"你"HIP"全身發出，籠罩整個隊伍！"NOR"", ch, NULL, NULL, TO_CHAR );
	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) )
			continue;

		if ( is_same_group( gch, ch ) && is_affected(gch, gsn_dark_containment) ){
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", gch );
			continue;
		}
		
		if ( is_same_group( gch, ch ) && IS_AFFECTED( gch, AFF_NO_HEAL ) ){
				send_to_char( "你的身體無法受到治癒。\n\r", gch );
			continue;
		}

		//gch->hit = gch->max_hit;
		// 修改 gfu 效果 , 分開 mob 及 pc 判定 2022/10/30
		//gch->hit = UMIN(gch->hit + 6000,gch->max_hit);
		if( IS_NPC(gch) )
			gch->hit = gch->hit + UMAX( 12000 , gch->max_hit/2 );
		else
			gch->hit = gch->max_hit;
		gch->hit = UMIN(gch->hit , gch->max_hit);
		update_pos( gch );
	}
	WAIT_MAGIC_STATE(ch, 12 - get_skill_level(ch, sn)*2);
	send_to_char( "好的.\n\r", ch );

	return;
}


void spell_full_recharge( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if( IS_NPC(ch) &&( IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))) return;
    victim->mana = victim->max_mana ;

    if ( ch != victim ) send_to_char( "好的。\n\r", ch );
    send_to_char( "你感到自己的魔力完全回復了.\n\r", victim );
    return;
}


void spell_double_recharge( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int amount = 200 + level/250;

    if( IS_NPC(ch) &&( IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))) return;
    victim->mana = UMIN( victim->mana + amount, victim->max_mana );

    if ( ch != victim ) send_to_char( "好的。\n\r", ch );
    send_to_char( "一股強大的能量流進你體內.\n\r", victim );
    return;
}


void spell_mana_recharge( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int amount = 100 + level/500;

    if( IS_NPC(ch) &&( IS_AFFECTED(ch, AFF_CHARM) || IS_AFFECTED(ch, AFF_SUMMONED))) return;

    victim->mana = UMIN( victim->mana + amount, victim->max_mana );

    if ( ch != victim ) send_to_char( "好的。\n\r", ch );
    send_to_char( "一股能量緩緩流進你體內.\n\r", victim );
    return;
}


void spell_mass_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *gch;
	//modified by coco 0520, for let healing magic will be influenced by pp and wis
	//算式: [法術基礎值] * [wis effect] * [pp effect] * [sklv effect] 
	double heal; // basic value;
	int sklv = get_skill_level(ch,sn);
	int nFinalHeal = 0;

	heal = 20 + ch->level * 7 / 4; // lv 41 , 91.75

	/*  修改為與 spell_heal 相同  2020/10/22

	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16) * ( get_curr_wis(ch) - 16 ) / 200;
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) * ( 16 - get_curr_wis(ch) ) / 200;


	// sklv effect
	if( !IS_NPC(ch) && get_skill_level( ch, sn) > 0 )
		heal = heal * ( 10 + get_skill_level( ch, sn )) / 10 ;
	else if ( IS_NPC(ch) )
		heal = heal * 1.4;

	if( get_skill_level(ch,sn) == 5)
		heal += 60;
	else if ( get_skill_level(ch,sn) == 6)
		heal += 150;

	//pp effect
	if ( !IS_NPC(ch) )
		heal = heal * ( 100 + ch->pp ) / 100  ;

	if(heal < 0) return;

	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) ) continue;
		
		if ( is_same_group( gch, ch ) && is_affected(gch, gsn_dark_containment) ){
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", gch );
			continue;
		}
		
		if ( is_same_group( gch, ch ) && IS_AFFECTED( gch, AFF_NO_HEAL ) ){
				send_to_char( "你的身體無法受到治癒。\n\r", gch );
			continue;
		}
	*/

	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16)
					 * ( get_curr_wis(ch) - 16 ) / 200;  // lv 41 , wis 23 , 164
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) 
					 * ( 16 - get_curr_wis(ch) ) / 200;

	//pp effect
	if ( !IS_NPC(ch) )
		heal = heal * ( 100 + ((ch->pp > 0) ? ch->pp : 0 ) ) / 100 ;

	// sklv effect
	if( sklv > 0 )
		heal = heal * ( 10 + sklv) / 10 ;

	if( sklv == 5 )
		heal = heal + 50;
	else if( sklv == 6)
		heal = heal + 100;

	if( heal < 50 ) heal = 50;  // 給予 heal 基本值 2020/10/22

	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) ) continue;
		
		if ( is_same_group( gch, ch ) && is_affected(gch, gsn_dark_containment) ){
			send_to_char( "一股邪惡力場阻絕了外界的治癒魔法。\n\r", gch );
			continue;
		}
		
		if ( is_same_group( gch, ch ) && IS_AFFECTED( gch, AFF_NO_HEAL ) ){
				send_to_char( "你的身體無法受到治癒。\n\r", gch );
			continue;
		}
		//heal = dice( 24, 3+get_skill_level(ch,sn)/2 ) + level + get_skill_level(ch,sn)*10;
		// 修改 blood thirsty 設定 , 故移除降低補血 2022/02/21
		//nFinalHeal = GetHealUnderBloodyBerserk(gch, heal);
		nFinalHeal = heal;
		nFinalHeal *= PPL_HP_MULTIPLYER;
		// 加入陣營影響  2020/10/22
		if( IS_NPC(ch) )
		{   /* NPC Will not be affected */
			;
		} else {
			if( IS_GOOD(ch) )
			{
				if( IS_GOOD(gch) ) nFinalHeal = nFinalHeal*5/4;
				else if ( IS_EVIL(gch) && ch->class != BISHOP )
				{
					nFinalHeal = nFinalHeal*3/4;
				}
			}
			else if( IS_NEUTRAL(ch) )
			{
				if( IS_NEUTRAL(gch) ) nFinalHeal = nFinalHeal*5/4;
			}
			else
			{
				if( IS_EVIL(gch) ) nFinalHeal = nFinalHeal*5/4;
				else if( IS_GOOD(gch) && ch->class != BISHOP ) nFinalHeal = nFinalHeal*3/4;
			}
		}
		nFinalHeal = LifeShareHeal(gch, nFinalHeal); // add at 2021/11/23
		gch->hit = (int)UMIN( gch->hit + nFinalHeal, gch->max_hit );
		update_pos( gch );

		send_to_char( "一股暖流充斥著你的身體.\n\r", gch );
	}
	send_to_char( "好的.\n\r", ch );

	return;
}

void spell_double_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	//算式: [法術基礎值] * [wis effect] * [pp effect] * [sklv effect] 
	//modified by coconet 0520
	double heal = 100 + ch->level * 15 / 4; // basic value;
	int sklv = get_skill_level( ch, sn);
	int obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}

	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16) * ( get_curr_wis(ch) - 16 ) / 200;
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) * ( 16 - get_curr_wis(ch) ) / 200;

	// sklv effect
	if( sklv > 0 )
		heal = heal * ( 10 + sklv )/ 10 ;

	if ( sklv == 5)
		heal += 80;
	else if( sklv == 6 )
		heal += 200;

	// 當 pp < 0 時，一律以 pp 0 來計算。 Keric
	if ( !IS_NPC(ch))
		heal = heal * ( 100 + ((ch->pp > 0)? ch->pp : 0) ) / 100 ;
	if(sklv > 5) heal += ch->pp;
	// victim->move = UMIN( victim->move + dice( level, 6)+55, victim->max_move );
	heal *= PPL_HP_MULTIPLYER;
	victim->move = UMIN( victim->move + heal , victim->max_move );

	if ( ch != victim ) send_to_char( "好的。\n\r", ch );
		send_to_char( "你的體力逐漸回復了。\n\r", victim );
	return;
}

void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	//算式: [法術基礎值] * [wis effect] * [pp effect] * [sklv effect] 
	//modified by coconet 0520
	double heal = 20 + ch->level; // basic value;
	int    sklv = get_skill_level( ch, sn);
	int  obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}

	//wis effect
	if ( get_curr_wis(ch) > 16 )
		heal += heal * ( get_curr_wis(ch) - 16) * ( get_curr_wis(ch) - 16 ) / 200;
	else
		heal -= heal * ( 16 - get_curr_wis(ch)) * ( 16 - get_curr_wis(ch) ) / 200;

	// sklv effect
	if( sklv > 0 )
		heal = heal * ( 10 + sklv) / 10 ;

	if ( sklv == 5)
		heal += 20;
	else if( sklv == 6 )
		heal += 50;
		
	// 當 pp < 0 時，一律以 pp 0 來計算。 Keric
	if ( !IS_NPC(ch))
		heal = heal * ( 100 + ((ch->pp > 0)? ch->pp : 0) ) / 100 ;

	heal *= PPL_HP_MULTIPLYER;
	victim->move = (int)UMIN( victim->move + heal , victim->max_move );

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你覺得沒有那麼累了。\n\r", victim );
	return;
}


void spell_full_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	victim->move = victim->max_move;
	ch->mana += get_skill_level(ch, sn) * 4 + (get_skill_level(ch, sn) - 3) * 2;

	if ( ch != victim ) send_to_char( "好的。\n\r", ch );
		send_to_char( "你的疲倦一掃而空，全身煥然一新。\n\r", victim );
	WAIT_MAGIC_STATE(ch, 24 - get_skill_level(ch, sn)*2);
	return;
}

void spell_group_full_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *rch, *gch;
	bool found = FALSE;
	char buf [MAX_INPUT_LENGTH];

	ch->mana += get_skill_level(ch, sn) * 8 + (get_skill_level(ch, sn) - 3) * 3;
	for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	{
		if (rch == ch)
			continue;
		if (rch->fighting == ch)
		{
			send_to_char( "你無法在戰鬥中集中精神！\n\r", ch);
			return;
		}
	}

	if (  is_qmark(ch, "spell_group_full_refresh") ){
		send_to_char( "你還沒從上次施法的損耗中回復過來。\n\r", ch );
		return;
	}

	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) )
			continue;
		if ( gch->class == LORD )
			found = TRUE;
	}

	if ( !found )
	{
		send_to_char( "沒有領主的配合，你無法獨力完成這項艱難的法術。\n\r", ch );
		return;
	}

	sprintf( buf, "self spell_group_full_refresh %d 0 0 0 0 1", 24 - get_skill_level(ch,sn) * 3  );
	do_add_qmark( ch, buf );

	act( "$n"HIG"虔誠地祈禱，希望上天再度賜與$e與$s隊友全新的活力！"NOR"", ch, NULL, NULL, TO_ROOM );
	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) )
			continue;

		gch->move = gch->max_move;

		send_to_char( "你覺得煥然一新，全身充滿精力！\n\r", gch );
	}
	WAIT_MAGIC_STATE(ch, 12 - get_skill_level(ch, sn)*2);
	return;
}

void spell_salvation( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA* victim;
	int sklv = get_skill_level(ch,sn);
	int obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}

	act( HIW "七彩的異光自天而降，籠罩了你整個隊伍！"NOR,
	ch, NULL, NULL, TO_CHAR    );
	act( HIW "七彩的異光自天而降，籠罩了$n"HIW"整個隊伍！"NOR,
	ch, NULL, NULL, TO_ROOM    );
	for (victim = ch->in_room->people; victim; victim = victim->next_in_room) {

		if (!is_same_group(victim, ch)) continue;
		switch(sklv) {
			case 6:
				affect_strip( victim, gsn_virtual_swords);
				affect_strip( victim, gsn_mute);
				affect_strip( victim, gsn_seal_magic);
				affect_strip( victim, gsn_sun_flash);
				affect_strip( victim, gsn_fear);
				affect_strip( victim, gsn_charm_person);
			case 5:
				affect_strip( victim, gsn_phlogistic_verse);
				affect_strip( victim, gsn_wail_of_tempest);
				affect_strip( victim, gsn_fulgurant_dance);
				affect_strip( victim, gsn_agglomerate_descant);
				affect_strip( victim, gsn_earth_bind);
				affect_strip( victim, gsn_finger_of_death);
				affect_strip( victim, gsn_hell_southencross);
			case 4:
				// paralyze 需同時有 skill 名的 buff 及 aff_paralyzed 才會作用 2022/06/14
				if( IS_AFFECTED( victim, AFF_PARALYZED ) && is_affected( victim, gsn_paralyze ) )
					affect_strip( victim, gsn_paralyze);
				affect_strip( victim, gsn_disease);
				affect_strip( victim, gsn_poison);
				if ( is_affected( victim, gsn_gas_breath) )
				{
					if( affected_value(victim, gsn_gas_breath, APPLY_SAVING_POISON) > 0 ){
						AFFECT_DATA af;
						af.type      = gsn_gas_breath;
						af.duration  = 0;
						af.location  = APPLY_SAVING_POISON;
						if( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) || IS_SET( victim->special, NO_POISON ) )
							af.bitvector = 0;
						else
							af.bitvector = AFF_POISON;
						af.modifier  = 0 - 50 - affected_value(victim, gsn_gas_breath, APPLY_SAVING_POISON)/2;
						affect_join( victim, &af );
					}
					if( affected_value(victim, gsn_gas_breath, APPLY_SAVING_POISON) <= 0 )
						affect_strip ( victim, gsn_gas_breath );
				}
			case 3:
				affect_strip( victim, gsn_burning_hands);
				affect_strip( victim, gsn_chill_touch);
				affect_strip( victim, gsn_thunder_strike);
			case 2:
				affect_strip( victim, gsn_faerie_fire);
				affect_strip( victim, gsn_tardy);
			case 1:
				affect_strip( victim, gsn_sleep);
				affect_strip( victim, gsn_blindness);
			case 0:
				affect_strip( victim, gsn_weaken);
				affect_strip( victim, gsn_curse);
			break;
		}
		act( "$n"HIW"的頭頂浮出守護天使的幻影，燦爛的光輝讓你感到舒暢無比。"NOR"",ch, NULL, victim, TO_VICT );
	}
}


