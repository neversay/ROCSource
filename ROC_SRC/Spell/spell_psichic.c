/* $Id: spell_psichic.c 916 2005-05-10 03:56:16Z zcecil $ */
#include "skill.h"


int GetWillFortressEffect(CHAR_DATA *victim)
{
    int nResist = 0;
    if(is_affected(victim, gsn_will_fortress)){
        nResist = affected_value(victim, gsn_will_fortress, APPLY_SAVING_ILLUSION);
    }
    return nResist;
}

/**
 * lv40 sklv0: 40 * 1.2
 * lv40 sklv6: 46 * 1.6 / 1.2 = 83.2 / 1.2
 */
double GetIllusionBaseDamage(int level, int sklv, bool isNpc) {
    double base_damage = (40 + level) / 2 + sklv / 2;
    if (isNpc) base_damage /= 2;
    base_damage = base_damage * (10 + sklv) / 12;

    return base_damage;
}

bool isAffectedByIllusion(CHAR_DATA *victim, int sn) {
  if (sn != gsn_green_breath && is_affected(victim, gsn_green_breath))
    return TRUE;
  if (sn != gsn_glacier_strike && is_affected(victim, gsn_glacier_strike))
    return TRUE;
  if (sn != gsn_banshee_wail && is_affected(victim, gsn_banshee_wail))
    return TRUE;
  if (sn != gsn_sand_tide && is_affected(victim, gsn_sand_tide))
    return TRUE;
  if (sn != gsn_succubus_kiss && is_affected(victim, gsn_succubus_kiss))
    return TRUE;
  return FALSE;
}

/* Demon tremor spell added by Amenda, 12/18/00 */
void spell_demontremor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next = NULL, *vch = NULL;
    AFFECT_DATA af;
    int dam, dice_point = 0, dice_max = 0;
    int sklv    = get_skill_level(ch,sn), sk_counter, dam_percent = 100;

    if ( IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
        send_to_char( HIB"在一個發不出聲音的地方製造聲音沒有生物會相信。\n\r"NOR, ch );
        return;
    }

    if( ch->deleted || !ch->in_room) return;
    send_to_char(HIB"你集中精神，製造出許\多幻象！\n\r"NOR, ch);

    //Every body dice once, the max dice winner will be attacked target!
    //add by Neversay at 2004/June/23
    for(victim = ch->in_room->people; victim; victim = victim->next_in_room){
        if(victim->deleted || (!IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS)))continue;
        dice_point = number_range(0, 100) + ((ch == victim)?(20-sklv):0);
        if(dice_point > dice_max){
            vch = victim;
            dice_max = dice_point;
        }
    }

    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
        victim_next = victim->next_in_room;
        if( ch == victim || is_same_group( ch, victim )) continue;

        if ((  IS_NPC(ch) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
            ( !IS_NPC(ch) && ( victim->fighting == ch ||
                               (    IS_NPC(victim)
                                 && !IS_AFFECTED( victim, AFF_CHARM ) 
                                 && !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) ) ){

            if ( !IS_AWAKE( victim ) || ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS)))
                continue;

            if( IS_SET(victim->special, ANTI_MAGIC)) continue;
            //Race check add by Neversay at 2004/June/23
            if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
                victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
                victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
                victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
                victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
            {
                act("$N"HIW"不受惡魔戰歌影響!"NOR"", ch, NULL, victim, TO_CHAR);
                continue;
            }

            //Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
            if (  ( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 +
                    ( UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv * 5 )
                > number_percent() * 100 / weakness_percent(victim, sn) )
            {
                if(number_percent() > 100 + GetWillFortressEffect(victim)){
									  send_to_char(HIC"心靈的障壁使你免於惡魔戰歌的影響"NOR"\n\r", victim);
                    continue;
								}
                act( "$N"HIB"被你的幻術迷惑，開始顫抖!"NOR"", ch, NULL, victim, TO_CHAR );
                act( "$N"HIB"似乎被什麼東西驚嚇，全身開始顫抖!"NOR"", ch, NULL, victim, TO_NOTVICT );
                act( ""HIR"一隻九呎高的巨大惡魔突然出現，向你猛烈攻擊"NOR"!",
                        ch, NULL, victim, TO_VICT );

                //The damage percent add for sklv+1 times, it has 0~88% to
                //add 10~22% damage per time.
                //The damage power of mind blast heavyly depending on int.
                for(dam_percent = 100, 
                    sk_counter = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);
                    sk_counter>0; sk_counter--)
                    if((get_curr_int(ch)<<2)-12 > number_percent())
                        dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);

                if(!IS_NPC(ch))
                    dam = dice( 20 + WIS_DIFF( ch, victim ), ch->level+sklv );
                else
                    dam = dice( 15 + WIS_DIFF( ch, victim ), ch->level );
                
                // Keric modfied to magic_damage
                //dam -= victim->mdr;
                dam = (dam * dam_percent * weakness_damage(victim, sn)) / 10000;

                if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
                    continue;

                if ( is_affected( victim, sn ) || victim->deleted || IS_SET(victim->special, NO_CRAZY))
                    continue;

                af.type         = sn;
                af.duration         = ch->level / 10;
                af.location         = APPLY_AC;
                af.modifier         = 100+sklv*10;
                af.bitvector = AFF_CRAZY;
                affect_to_char( victim, &af );

                af.location         = APPLY_HITROLL;
                af.modifier         = -10-sklv;
                affect_to_char( victim, &af );

                af.location         = APPLY_DAMROLL;
                af.modifier         = 15+sklv;
                affect_to_char( victim, &af );

                af.location         = APPLY_STR;
                af.modifier         = 2;
                affect_to_char( victim, &af );

                af.location         = APPLY_INT;
                af.modifier         = -2;
                affect_to_char( victim, &af );

                af.location = APPLY_AMAGIC;
                af.modifier = -10-sklv*3;
                affect_to_char( victim, &af );
                af.location = APPLY_RMAGIC;
                af.modifier = -5-sklv*2;
                affect_to_char( victim, &af );

                if( !vch->deleted && victim->fighting && vch != victim)
                    stop_fighting(victim, FALSE);
                    set_fighting(victim, vch);
            }else if ( number_bits(3) == 1 ){
                if( victim->fighting || vch != victim)
                    stop_fighting(victim, FALSE);
                set_fighting(victim, vch);
            }
        }
    }
    return;
}

/* GreenDragon breath spell added by Amenda, 8/27/01 */
/* GreenDragon breath spell modify by Neversay, 2004/June/25 */
void spell_green_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    int  dam, sklv= UMIN(get_skill_level(ch, sn), 6), counter = 0;
    int  sk_counter, dam_percent = 100;
    bool is_powerful_mode = FALSE;
    static const int percent_array[] = {0, -2, -4, -7, -11, -16, -30};
	int obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}
    
    double base_damage = GetIllusionBaseDamage(ch->level, sklv, IS_NPC(ch));
    base_damage = base_damage * int_effect(ch);

    if ( ch->in_room->sector_type == SECT_UNDERWATER)
    {
         send_to_char(HIB"在水底製造巨龍幻象沒有生物會相信的。\n\r"NOR, ch);
         return;
    }

    if(sklv>0 && (ch->move >= sklv*25) && is_affected(ch, skill_lookup("mental empower")))
        for(victim = ch->in_room->people; victim; victim = victim->next_in_room)
            if(!IS_NPC(victim) && (victim->pcdata->prime_attr == APPLY_CON) &&
                (ch!=victim) && is_same_group(ch, victim))
                is_powerful_mode = TRUE;

    if(is_powerful_mode == TRUE){
            send_to_char(HIG"你消耗大量體力，增幅你的心靈能量！\n\r"NOR, ch);
        ch->move -= sklv*25;
        send_to_char(HIG"你集中精神，藉由同伴的心靈增幅，製造出綠龍噴火的幻象！\n\r\n\r"NOR, ch);
    }else
        send_to_char(HIB"你集中精神，製造出綠龍噴毒的幻象！\n\r"NOR, ch);
    
    // damage incrases under powerful mode (at sklv 6 is 130%)
    if (is_powerful_mode) {
        base_damage = base_damage * (112 + sklv * 3) / 100;
    }

    for( victim = ch->in_room->people; victim;victim = victim_next)
    {
        victim_next = victim->next_in_room;
        if( ch == victim || is_same_group( ch, victim ))
            continue;


        if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
            ( !IS_NPC( ch ) && 
               ( victim->fighting == ch ||
                 (  IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && 
                   !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) ) ){
            if ( !check_blind ( victim ) || 
                 !IS_AWAKE( victim ) || 
                  IS_SET(victim->special, ANTI_MAGIC) ||
                (!IS_NPC(victim) && (IS_SET(victim->act, PLR_WIZINVIS) || 
                  is_affected( victim, gsn_blindness))))
                continue;

            //Race check add by Neversay at 2004/June/23
            if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
                victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
                victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
                victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
                victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
            {
                act("$N"HIW"不受綠龍狂燄影響!"NOR"", ch, NULL, victim, TO_CHAR);
                continue;
            }

            //Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
            if (  ( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 +
                    ( UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv * 5 )
                > number_percent() * 100 / weakness_percent(victim, sn) )
            {
                if(number_percent() > 100 + GetWillFortressEffect(victim)){
									  send_to_char(HIC"心靈的障壁使你免於幻術的影響\n\r", victim);
                    continue;
								}
                act( "$N"HIB"似乎被火焰焚身，開始在地上亂滾!"NOR"", ch, NULL, victim, TO_NOTVICT );
                if(is_powerful_mode == TRUE)
                    act( ""HIG"巨大綠龍的毒性烈焰噴得你猶如身處煉獄!"NOR"", ch, NULL, victim, TO_VICT );
                else
                    act( ""HIB"大型綠龍的惡臭火焰噴得你七暈八素!"NOR"", ch, NULL, victim, TO_VICT );

                //The damage percent add for sklv+1 times, it has 0~88% to
                //add 10~22% damage per time.
                //The damage power of mind blast heavyly depending on int.
                for(dam_percent = 100,
                    sk_counter = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);
                    sk_counter > 0;
                    sk_counter--){
                    if((get_curr_int(ch)<<2)-12 > number_percent())
                        dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);
                }
                dam = base_damage * new_saves_spell(ch->pp, victim, MAGIC_FIRE) / 100;
                dam = number_range(dam * 3, dam * 5);

                dam = (dam * dam_percent* weakness_damage( victim, sn )) / 10000;
                //The damage does not reduce halt by sanctuary.
                magic__damage( ch, victim, dam, sn, WEAR_NONE );

                if ( victim->deleted || is_affected( victim, sn ))
                    continue;

                if(!IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_INT){
                    af.type = sn;
                    af.duration  = 1 + (sklv<<1);
                    af.bitvector = 0;
                    af.location  = APPLY_SAVING_FIRE;
                    af.modifier  = 1 + (sklv<<1);
                    affect_to_char( victim, &af );
                }

                if (isAffectedByIllusion(victim, sn)) continue;

                if(is_powerful_mode == TRUE)
                    act( "$N"HIG"被你的幻術迷惑，開始驚惶失措!"NOR"", ch, NULL, victim, TO_CHAR );
                else
                    act( "$N"HIB"被你的幻術迷惑，開始驚惶失措!"NOR"", ch, NULL, victim, TO_CHAR );


                af.type      = sn;
                af.duration  = sklv + 1;
                af.bitvector = 0;
                //Special power mode, for weakening enemies more.
                if( is_powerful_mode == TRUE && ch->mana >= 5*sklv && ch->move >= 5*sklv ){
                    if(counter < 10){
                        ch->mana -= sklv*5;
                        ch->move -= sklv*5;
                        counter++;
                    }
                    af.location  = APPLY_AC;
                    af.modifier  = UMAX((percent_array[sklv]*victim->armor)/100, sklv * 15) + sklv * 15;
                    affect_to_char( victim, &af );
                    af.location  = APPLY_RDAMAGE;
                    af.modifier  = UMIN((percent_array[sklv]*victim->pdr)/100, 0 - sklv * 5) - sklv * 5;
                    affect_to_char( victim, &af );
                    act( "$N"HIG"被你的幻術迷惑，身陷其中，無法自拔!"NOR"", ch, NULL, victim, TO_CHAR );
                }
                af.location  = APPLY_CON;
                af.modifier  = -sklv / 2 - ((sklv >= 6)? 2:1);
                affect_to_char( victim, &af );
            }else{
                act( "$N"HIB"識破你的幻術!"NOR"", ch, NULL, victim, TO_CHAR );
            }
        }
    }
    return;
}


/* Sand tide spell add by Neversay, 2004/June/25 */
void spell_sand_tide( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    int  dam, sklv= UMIN(get_skill_level(ch, sn), 6), counter = 0;
    int  sk_counter, dam_percent = 100;
    bool is_powerful_mode = FALSE;
    static const int percent_array[] = {0, -2, -4, -6, -8, -12, -25};
	int obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

    double base_damage = GetIllusionBaseDamage(ch->level, sklv, IS_NPC(ch));
    base_damage = base_damage * int_effect(ch);

    if ( ch->in_room->sector_type == SECT_FOREST)
    {
         send_to_char(HIB"在森林裡製造沙浪幻象沒有生物會相信的。\n\r"NOR, ch);
         return;
    }

    if(sklv>0 && (ch->move>=sklv*25) && is_affected(ch, skill_lookup("mental empower")))
        for(victim = ch->in_room->people; victim; victim = victim->next_in_room)
            if(!IS_NPC(victim) && (victim->pcdata->prime_attr == APPLY_DEX) &&
                (ch!=victim) && is_same_group(ch, victim))
                is_powerful_mode = TRUE;

    if(is_powerful_mode == TRUE){
        send_to_char(HIY"你消耗大量體力，增幅你的心靈能量！\n\r"NOR, ch);
        ch->move -= sklv*25;
        send_to_char(HIY"你集中精神，藉由同伴的心靈增幅，製造出沙浪崩岸的幻象！\n\r\n\r"NOR, ch);
    }else{
        send_to_char(HIB"你集中精神，製造出沙浪襲擊的幻象！\n\r"NOR, ch);
    }
    
    // damage incrases under powerful mode (at sklv 6 is 130%)
    if (is_powerful_mode) {
        base_damage = base_damage * (112 + sklv * 3) / 100;
    }

    for( victim = ch->in_room->people; victim;victim = victim_next)
    {
        victim_next = victim->next_in_room;
        if( ch == victim || is_same_group( ch, victim ))continue;

        if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
            ( !IS_NPC( ch ) && ( victim->fighting == ch ||
                                 ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && 
                                  !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) ) ){
            if ( !check_blind ( victim ) || !IS_AWAKE( victim ) || IS_SET(victim->special, ANTI_MAGIC) ||
                 (!IS_NPC(victim) && 
                  (IS_SET(victim->act, PLR_WIZINVIS) || is_affected( victim, gsn_blindness))))
                continue;

            //Race check add by Neversay at 2004/June/23
            if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
                victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
                victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
                victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
                victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
            {
                act("$N"HIW"不受沙浪翻騰影響!"NOR"", ch, NULL, victim, TO_CHAR);
                continue;
            }

            //Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
            if (  ( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 +
                    ( UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv * 5 )
                > number_percent() * 100 / weakness_percent(victim, sn) )
            {
                if(number_percent() > 100 + GetWillFortressEffect(victim)){
									  send_to_char(HIC"心靈的障壁使你免於幻術的影響"NOR"\n\r", victim);
                    continue;
								}
                act( "$N"HIB"似乎被淹埋在地底，開始痛苦掙扎!"NOR"", ch, NULL, victim, TO_NOTVICT );
                if(is_powerful_mode == TRUE)
                    act(""HIY"洶湧莫名的沙浪將你淹入地底，讓你無法動作!"NOR"", ch, NULL, victim, TO_VICT);
                else
                    act(""HIB"一波沙浪淹沒你的雙腳，讓你身形受制!"NOR"", ch, NULL, victim, TO_VICT);

                //The damage percent add for sklv+1 times, it has 0~88% to
                //add 10~22% damage per time.
                //The damage power of mind blast heavyly depending on int.
                for(dam_percent = 100, 
                    sk_counter = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);
                    sk_counter > 0;
                    sk_counter--)
                    if((get_curr_int(ch)<<2)-12 > number_percent())
                        dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);

                dam = base_damage * new_saves_spell(ch->pp, victim, MAGIC_EARTH) / 100;
                dam = number_range(dam * 2, dam * 6);
                //The damage does not reduce halt by sanctuary.
                magic__damage( ch, victim, dam, sn, WEAR_NONE );

                if ( victim->deleted || is_affected( victim, sn ))
                    continue;

                if(!IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_INT){
                    af.type = sn;
                    af.duration  = 1 + (sklv<<1);
                    af.bitvector = 0;
                    af.location  = APPLY_SAVING_EARTH;
                    af.modifier  = 1 + (sklv<<1);
                    affect_to_char( victim, &af );
                }

                if (isAffectedByIllusion(victim, sn)) continue;

                if(is_powerful_mode == TRUE)
                    act( "$N"HIY"被你的幻術迷惑，身形開始緩慢!"NOR"", ch, NULL, victim, TO_CHAR );
                else
                    act( "$N"HIB"被你的幻術迷惑，身形開始緩慢!"NOR"", ch, NULL, victim, TO_CHAR );

                af.type      = sn;
                af.duration  = sklv + 1;
                af.bitvector = 0;
                //Special power mode, for weakening enemies more.
                if( (is_powerful_mode == TRUE) && (ch->mana >= 5*sklv) && (ch->move >= 5*sklv)
                    ){
                    if(counter < 10){
                        ch->mana -= sklv*5;
                        ch->move -= sklv*5;
                        counter++;
                    }
                    af.location  = APPLY_DODGE;
                    af.modifier  = UMAX((percent_array[sklv]*victim->dodge) / 100, sklv * 10) + sklv * 10;
                    affect_to_char( victim, &af );
                    af.location  = APPLY_HITROLL;
                    af.modifier  = UMIN((percent_array[sklv]*victim->hitroll)/100, 0 - sklv * 2) - sklv * 2;
                    affect_to_char( victim, &af );
                    act( "$N"HIY"被你的幻術迷惑，身陷其中，無法自拔!"NOR"", ch, NULL, victim, TO_CHAR );
                }
                af.location  = APPLY_DEX;
                af.modifier  = -sklv / 2 - ((sklv >= 6)? 2:1);
                affect_to_char( victim, &af );
            }else{
                act( "$N"HIB"識破你的幻術!"NOR"", ch, NULL, victim, TO_CHAR );
            }
        }
    }
    return;
}


/* Banshee scream spell add by Neversay, 2004/June/25 */
void spell_banshee_wail( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    int  dam, sklv= UMIN(get_skill_level(ch, sn), 6), counter = 0;
    int  sk_counter, dam_percent = 100;
    bool is_powerful_mode = FALSE;
    static const int percent_array[] = {0, -2, -4, -6, -8, -12, -25};
	int obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

    double base_damage = GetIllusionBaseDamage(ch->level, sklv, IS_NPC(ch));
    base_damage = base_damage * int_effect(ch);

    if ( IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
        send_to_char( HIB"在一個發不出聲音的地方製造聲音沒有生物會相信。\n\r"NOR, ch );
        return;
    }

    if(sklv>0 && (ch->move>=sklv*25) && is_affected(ch, skill_lookup("mental empower")))
        for(victim = ch->in_room->people; victim; victim = victim->next_in_room)
            if(!IS_NPC(victim) && (victim->pcdata->prime_attr == APPLY_INT) &&
                (ch!=victim) && is_same_group(ch, victim))
                is_powerful_mode = TRUE;

    if(is_powerful_mode == TRUE){
        send_to_char(HIU"你消耗大量體力，增幅你的心靈能量！\n\r"NOR, ch);
        ch->move -= sklv*25;
        send_to_char(HIU"你集中精神，藉由同伴的心靈增幅，製造出群妖厲嚎的幻象！\n\r\n\r"NOR, ch);
    }else{
        send_to_char(HIB"你集中精神，製造出女妖哭號的幻象！\n\r"NOR, ch);
    }
    
    // damage incrases under powerful mode (at sklv 6 is 130%)
    if (is_powerful_mode) {
        base_damage = base_damage * (112 + sklv * 3) / 100;
    }

    for( victim = ch->in_room->people; victim;victim = victim_next)
    {
        victim_next = victim->next_in_room;
        if( ch == victim || is_same_group( ch, victim ))continue;

        if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
            ( !IS_NPC( ch ) && ( victim->fighting == ch ||
                                 (    IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) 
                                  && !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) ) ){
            if ( !IS_AWAKE( victim ) || IS_SET(victim->special, ANTI_MAGIC) ||
                 ( !IS_NPC(victim) && 
                   (IS_SET(victim->act, PLR_WIZINVIS) || is_affected( victim, gsn_blindness))))
                continue;


            //Race check add by Neversay at 2004/June/23
            if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
                victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
                victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
                victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
                victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
            {
                act("$N"HIW"不受喪妖厲嚎影響!"NOR"", ch, NULL, victim, TO_CHAR);
                continue;
            }

            //Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
            if (  ( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 +
                    ( UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv * 5 )
                > number_percent() * 100 / weakness_percent(victim, sn) )
            {
                if(number_percent() > 100 + GetWillFortressEffect(victim)){
									  send_to_char(HIC"心靈的障壁使你免於幻術的影響\n\r", victim);
                    continue;
								}
                act( "$N"HIB"似乎聽到不可思議的聲音，四肢痙孿!"NOR"", ch, NULL, victim, TO_NOTVICT );
                if(is_powerful_mode == TRUE)
                    act( ""HIU"一群報喪女妖的淒厲奪命尖嚎讓你驚懼莫名!"NOR"", ch, NULL, victim, TO_VICT );
                else
                    act( ""HIB"報喪女妖的哭號讓你神智不清!"NOR"", ch, NULL, victim, TO_VICT );

                //The damage percent add for sklv+1 times, it has 0~88% to
                //add 10~22% damage per time.
                //The damage power of mind blast heavyly depending on int.
                for(dam_percent = 100,
                    sk_counter  = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);
                    sk_counter > 0; 
                    sk_counter--)
                    if((get_curr_int(ch)<<2)-12 > number_percent())
                        dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);

                dam = base_damage * new_saves_spell(ch->pp, victim, MAGIC_SPELL) / 100;
                dam = number_range(dam * 3.5, dam * 4.5);

                dam = (dam * dam_percent* weakness_damage( victim, sn )) / 10000;
                //The damage does not reduce halt by sanctuary.
                magic__damage( ch, victim, dam, sn, WEAR_NONE );

                if ( victim->deleted || is_affected( victim, sn ))
                    continue;

                if(!IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_INT){
                    af.type = sn;
                    af.duration  = 1 + (sklv<<1);
                    af.bitvector = 0;
                    af.location  = APPLY_SAVING_WIND;
                    af.modifier  = 1 + (sklv<<1);
                    affect_to_char( victim, &af );
                }

                if(is_powerful_mode == TRUE)
                    act( "$N"HIU"被你的幻術迷惑，顯出一副癡呆樣!"NOR"", ch, NULL, victim, TO_CHAR );
                else
                    act( "$N"HIB"被你的幻術迷惑，顯出一副癡呆樣!"NOR"", ch, NULL, victim, TO_CHAR );

                af.type      = sn;
                af.duration  = sklv + 1;
                af.bitvector = 0;
                //Special power mode, for weakening enemies more.

                if (isAffectedByIllusion(victim, sn)) continue;

                if( (is_powerful_mode == TRUE) && (ch->mana >= 5*sklv) && (ch->move >= 5*sklv) ){
                    if(counter < 10){
                        ch->mana -= sklv*5;
                        ch->move -= sklv*5;
                        counter++;
                    }
                    af.location  = APPLY_POWER_POTENTIAL;
                    af.modifier  = UMIN((percent_array[sklv]*victim->pp)/100, 0 - sklv * 2) - sklv * 2;
                    affect_to_char( victim, &af );
                    af.location  = APPLY_AMAGIC;
                    af.modifier  = UMIN((percent_array[sklv]*victim->amdr)/100, 0 - sklv * 5) - sklv * 5;
                    affect_to_char( victim, &af );
                    act( "$N"HIU"被你的幻術迷惑，身陷其中，無法自拔!"NOR"", ch, NULL, victim, TO_CHAR );
                }
                af.location  = APPLY_INT;
                af.modifier  = -sklv / 2 - ((sklv >= 6)? 2:1);
                affect_to_char( victim, &af );
            }else{
                act( "$N"HIB"識破你的幻術!"NOR"", ch, NULL, victim, TO_CHAR );
            }
        }
    }
    return;
}


/* Succubus kiss spell add by Neversay, 2004/June/25 */
void spell_succubus_kiss( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim, *victim_next;
  AFFECT_DATA af;
  int dam, sklv= UMIN(get_skill_level(ch, sn), 6), counter = 0;
  int sk_counter, dam_percent = 100;
  bool is_powerful_mode = FALSE;
  static const int percent_array[] = {0, -2, -4, -7, -11, -16, -30};
  int obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

  double base_damage = GetIllusionBaseDamage(ch->level, sklv, IS_NPC(ch));
  base_damage = base_damage * int_effect(ch);

  if( sklv > 0 && (ch->move>=sklv*25)
      && is_affected(ch, skill_lookup("mental empower"))) {
    for(victim = ch->in_room->people;
        victim;
        victim = victim->next_in_room) {
      if(!IS_NPC(victim) && (victim->pcdata->prime_attr == APPLY_WIS) &&
          (ch!=victim) && is_same_group(ch, victim)) {
        is_powerful_mode = TRUE;
        break;
      }
    }
  }

  if(is_powerful_mode == TRUE){
    send_to_char(HIP"你消耗大量體力，增幅你的心靈能量！\n\r"NOR, ch);
    ch->move -= sklv*25;
    send_to_char(HIP"你集中精神，藉由同伴的心靈增幅，製造出月之女神的幻象！\n\r\n\r"NOR, ch);
  } else {
    send_to_char(HIB"你集中精神，製造出魅魔降臨的幻象！\n\r"NOR, ch);
  }
    // damage incrases under powerful mode (at sklv 6 is 130%)
    if (is_powerful_mode) {
        base_damage = base_damage * (112 + sklv * 3) / 100;
    }

  for( victim = ch->in_room->people; victim;victim = victim_next)
  {
    victim_next = victim->next_in_room;
    if( ch == victim || is_same_group( ch, victim ))continue;


    if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
        ( !IS_NPC( ch ) && ( victim->fighting == ch ||
        ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) ) ){
      if ( !check_blind ( victim ) || !IS_AWAKE( victim ) || IS_SET(victim->special, ANTI_MAGIC) ||
           (!IS_NPC(victim) && (IS_SET(victim->act, PLR_WIZINVIS) || is_affected( victim, gsn_blindness))))
        continue;


      //Race check add by Neversay at 2004/June/23
      if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
          victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
          victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
          victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
          victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
      {
        act("$N"HIW"不受魅魔香吻影響!"NOR"", ch, NULL, victim, TO_CHAR);
        continue;
      }

      //Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
      if (  ( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 +
              ( UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv * 5 )
          > number_percent() * 100 / weakness_percent(victim, sn) )
      {
        if(number_percent() > 100 + GetWillFortressEffect(victim)){
            send_to_char(HIC"心靈的障壁使你免於幻術的影響\n\r", victim);
            continue;
        }
        act( "$N"HIB"似乎看到不可思議的景象，令其瞠目結舌!"NOR"", ch, NULL, victim, TO_NOTVICT );
        if(is_powerful_mode == TRUE)
            act( ""HIP"你看到傳說中的月之女神降臨，深情地擁吻著你!"NOR"", ch, NULL, victim, TO_VICT );
        else
            act( ""HIB"你看到魅魔化身成你心目中的情人，擁吻著你!"NOR"", ch, NULL, victim, TO_VICT );

        //The damage percent add for sklv+1 times, it has 0~88% to
        //add 10~22% damage per time.
        //The damage power of mind blast heavyly depending on int.
        for(dam_percent = 100,
            sk_counter = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);
            sk_counter > 0;
            sk_counter--)
            if((get_curr_int(ch)<<2)-12 > number_percent())
                dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);

        dam = base_damage * new_saves_spell(ch->pp, victim, MAGIC_SPELL) / 100;
        dam = number_range(dam * 3, dam * 5);

        dam = (dam * dam_percent* weakness_damage( victim, sn )) / 10000;
        //The damage does not reduce halt by sanctuary.
        magic__damage( ch, victim, dam, sn, WEAR_NONE );

        if ( victim->deleted || is_affected( victim, sn ))
            continue;

        if(!IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_INT){
          af.type = sn;
          af.duration  = 1 + (sklv<<1);
          af.bitvector = 0;
          af.location  = APPLY_SAVING_LIGHTNING;
          af.modifier  = 1 + (sklv<<1);
          affect_to_char( victim, &af );
        }

        if (isAffectedByIllusion(victim, sn)) continue;

        if(is_powerful_mode == TRUE)
          act( "$N"HIP"被你的幻術迷惑，一臉意亂情迷!"NOR"", ch, NULL, victim, TO_CHAR );
        else
          act( "$N"HIB"被你的幻術迷惑，一臉意亂情迷!"NOR"", ch, NULL, victim, TO_CHAR );

        af.type      = sn;
        af.duration  = sklv + 1;
        af.bitvector = 0;
        //Special power mode, for weakening enemies more.
        if( (is_powerful_mode == TRUE) && (ch->mana >= 5*sklv) && (ch->move >= 5*sklv)
            ){
            if(counter < 10){
                ch->mana -= sklv*5;
                ch->move -= sklv*5;
                counter++;
            }
            af.location  = APPLY_SAVING_SPELL;
            af.modifier  = UMIN((percent_array[sklv]*victim->saving_throw[MAGIC_SPELL])/100, 0 + sklv) + sklv * 2;
            affect_to_char( victim, &af );
            af.location  = APPLY_RMAGIC;
            af.modifier  = UMIN((percent_array[sklv]*victim->mdr)/100, 0 - sklv * 5) - sklv * 5;
            affect_to_char( victim, &af );
            act( "$N"HIP"被你的幻術迷惑，身陷其中，無法自拔!"NOR"", ch, NULL, victim, TO_CHAR );
        }
        af.location  = APPLY_WIS;
        af.modifier  = -sklv / 2 - ((sklv >= 6)? 2:1);
        affect_to_char( victim, &af );
      }else{
        act( "$N"HIB"識破你的幻術!"NOR"", ch, NULL, victim, TO_CHAR );
      }
    }
  }
  return;
}
/*
void spell_psichic_storm( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    int dam, dam_percent;
		int nPstoSklv = get_skill_level(ch, gsn_psichic_storm);
		int nMindSklv = get_skill_level(ch, gsn_mind_storm);
}
*/
void spell_mind_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    int dam, dam_percent, sklv = get_skill_level(ch, sn), is_will_strong;
    int dodge_aff;

    ch->mana -= sklv*5;
    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
        victim_next = victim->next_in_room;
        if( is_same_group( ch, victim ) )continue;

        if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
            ( !IS_NPC( ch ) && ( victim->fighting == ch ||
            ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) )
             ){
            if( !IS_AWAKE( victim ) || IS_SET(victim->special, ANTI_MAGIC) ||
                (!IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS)))
                continue;


            //Race check add by Neversay at 2004/June/23
            if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
                victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
                victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
                victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
                victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
            {
                act("$N"HIW"不受心靈爆震影響!"NOR"", ch, NULL, victim, TO_CHAR);
                continue;
            }

            is_will_strong = (is_affected(victim, gsn_will_fortress)? 2:1);

            //Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
            //If victim has spell "will fortress", the change reduce half.
            if (( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 ) +
                (UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv*5
                > (is_will_strong * number_percent() * 100 / weakness_percent(victim, sn)) )
            {
                //The damage percent add for 0~36 times,
                //it add 5% damage per time.
                //The damage power of mind blast heavyly depending on int.
                dodge_aff = affected_value(victim, gsn_mind_blast, APPLY_DODGE);
                dam_percent = 100 + (5*((get_curr_int(ch)<<2)-12)*dodge_aff*dodge_aff)/3600;

                //basic damage range 0~360.
                dam =  ((dodge_aff>>2)+sklv)*(level>>1) + ch->pp;
                //For NPC, basic damage is upto 360.
                if(IS_NPC(ch)) dam = UMAX(dam, 360);

                dam = (dam * dam_percent * weakness_damage( victim, sn )) /(10000*is_will_strong);
                //The damage does not reduce halt by sanctuary.
                magic__damage( ch, victim, dam, sn, WEAR_NONE );

                if(!is_affected(victim, sn)){
                    af.type      = sn;
                    af.duration  = sklv + 1;
                    af.bitvector = 0;
                    af.location  = APPLY_SAVING_SPELL;
                    af.modifier = 1+sklv;
                    affect_to_char( victim, &af );
                    af.location  = APPLY_DODGE;
                    af.modifier = sklv;
                    affect_to_char( victim, &af );
                    act( "$N"HIB"心靈受到重創，神智混亂!"NOR"", ch, NULL, victim, TO_CHAR );
                    act( ""HIB"你的心智受到猛然重創!"NOR"", ch, NULL, victim, TO_VICT );
                }else if(affected_value(victim, gsn_mind_blast, APPLY_DODGE) < sklv*6){
                    af.type      = sn;
                    af.duration  = 1;
                    af.bitvector = 0;
                    af.location  = APPLY_DODGE;
                    af.modifier = sklv;
                    affect_join(victim, &af);
                    act( HIB"你進一步撕裂$N的心靈!"NOR"", ch, NULL, victim, TO_CHAR );
                }
            }else{
                act( ""HIB"$N不受你的心靈影響!"NOR"", ch, NULL, victim, TO_CHAR );
            }
        }
    }
}

void spell_disenchant( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *gch;
    int curr_int = get_curr_int(ch),dam = 0,
	spell_glacier = skill_lookup("glacier strike"),
	spell_green = skill_lookup("green breath"),
	spell_sand = skill_lookup("sand tide"),
	spell_banshee = skill_lookup("banshee wail"),
	spell_ghosts_celebration = skill_lookup("ghosts celebration"),  // add at 2022/10/13
	spell_succubus = skill_lookup("succubus kiss");
    if(ch->move <= (30 - curr_int)*10 )
	{
		send_to_char( "你的體力少得不夠使用本法術！\n\r", ch );
		ch->mana += (MANA_COST( ch, sn )>>1);
		return;
    }

    for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
		if ( !is_same_group( gch, ch ) )
		{
			//If the victim is phantom, this spell cause damage to it!
			if(gch->race == race_lookup("Phantom"))
			{
				dam = 6 * UMIN( 100, ( INT_DIFF( ch, gch ) + WIS_DIFF( ch, gch ) * 4  + saves_spell( ch->pp, gch ) - 20 ) ) * UMAX( 0.5, number_bits(3) / 8 );
				// Keric modfied to magic_damage
				//dam -= gch->mdr;
				dam = dam * weakness_damage(gch, sn ) / 100;
				magic_damage( ch, gch, dam, sn, WEAR_NONE );
			}
			continue;
		}

		affect_strip( gch, spell_glacier);
		affect_strip( gch, spell_green);
		affect_strip( gch, spell_sand);
		affect_strip( gch, spell_banshee);
		affect_strip( gch, spell_succubus);
		affect_strip( gch, spell_ghosts_celebration); // add at 2022/10/13
		affect_strip( gch, gsn_virtual_swords);
		affect_strip( gch, gsn_fear);

		send_to_char( ""HIB"你眼前的幻象與耳邊的幻聲漸漸消失！"NOR"\n\r", gch );
    }
}


void spell_ventriloquate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    char       buf1    [ MAX_STRING_LENGTH ];
    char       buf2    [ MAX_STRING_LENGTH ];
    char       speaker [ MAX_INPUT_LENGTH  ];

    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s 說 '%s'.\n\r",		     speaker, target_name );
    sprintf( buf2, "某人命令 %s 說 '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER( buf1[0] );

    for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
    {
	if ( !is_name( speaker, vch->name ) )
	    send_to_char( saves_spell( ch->pp, vch ) ? buf2 : buf1, vch );
    }

    WAIT_MAGIC_STATE(ch, 12);
    return;
}


void spell_mind_thrust ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    magic_damage( ch, (CHAR_DATA *) vo, dice( 1, 10 ) + level / 2, sn, WEAR_NONE );
    return;
}


void spell_project_force ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    magic_damage( ch, (CHAR_DATA *) vo, dice( 4, 6 ) + level, sn, WEAR_NONE );
    return;
}


void spell_psionic_blast ( int sn, int level, CHAR_DATA *ch, void *vo )
{
		 CHAR_DATA *victim	 = (CHAR_DATA *) vo;
    static const int	    dam_each [ ] =
    {
	  0,
	  0,   0,   0,	 0,   0,	0,   0,	  0,   0,   0,
	  0,   0,   0,	 0,   0,	0,  45,	 50,  55,  60,
	 64,  68,  72,	76,  80,       82,  84,	 86,  88,  90,
	 92,  94,  96,	98, 100,      102, 104, 106, 108, 100,
	112, 114, 116, 118, 120,      122, 124, 126, 128, 130
    };
		 int	    dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam	     = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( ch->pp, victim ) < number_percent() )
	dam /= 2;

    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * weakness_damage (victim, sn) / 100;
    magic_damage( ch, victim, dam, sn, WEAR_NONE );

    return;
}



void spell_psychic_crush ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    magic_damage( ch, (CHAR_DATA *) vo, dice( 3, 5 ) + level, sn, WEAR_NONE );
    return;
}



void spell_psychic_drain ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( ch->pp, victim ) )
	return;

    af.type	 = sn;
    af.duration	 = level / 2;
    af.location	 = APPLY_STR;
    af.modifier	 = -1 - ( level >= 10 ) - ( level >= 20 ) - ( level >= 30 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You feel drained.\n\r", victim );
    act( "$n appears drained of strength.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_psychic_healing ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice( 3, 6 ) + 2 * level / 3 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );

    send_to_char( "You feel better!\n\r", victim );
    return;
}


void spell_thought_shield ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    af.type	 = sn;
    af.duration	 = level;
    af.location	 = APPLY_AC;
    af.modifier	 = -20;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You have created a shield around yourself.\n\r", ch );
    return;
}

void spell_ultrablast ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int	       dam;
    int	       hpch;

    for ( vch = ch->in_room->people; vch; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	if ( vch->deleted )
	    continue;

	if ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) )
	{
	    hpch = UMAX( 10, ch->hit );
	    dam	 = number_range( hpch / 8+1, hpch / 4 );
	    if ( saves_spell( ch->pp, vch ) < number_percent() )
		dam /= 2;
	    magic_damage( ch, vch, dam, sn, WEAR_NONE );
	}
    }
    return;
}

/* Glacier strike spell added by Amenda, 2/25/01 */
/* Glacier strike spell modify by Neversay, 2004/June/24 */
void spell_glacier_strike( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    int	       dam,	sklv= UMIN(get_skill_level(ch, sn), 6), counter = 0,
    	is_will_strong = 1, sk_counter, dam_percent = 100;
    bool is_powerful_mode = FALSE;
    static const int percent_array[] = {0, -2, -4, -6, -8, -12, -25};
	int obj_lv  = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

    double base_damage = GetIllusionBaseDamage(ch->level, sklv, IS_NPC(ch));
    base_damage = base_damage * int_effect(ch);

    if ( ch->in_room->sector_type == SECT_DESERT)
    {
	 send_to_char(HIB"在沙漠製造冰河幻象沒有生物會相信的。\n\r"NOR, ch);
	 return;
    }

    if(sklv>0 && (ch->move>=sklv*25) && is_affected(ch, skill_lookup("mental empower")))
	for(victim = ch->in_room->people; victim; victim = victim->next_in_room)
	    if(!IS_NPC(victim) && (victim->pcdata->prime_attr == APPLY_STR) &&
		(victim!=ch) && is_same_group(ch, victim))
		is_powerful_mode = TRUE;

    if(is_powerful_mode == TRUE){
    	send_to_char(HIC"你消耗大量體力，增幅你的心靈能量！\n\r"NOR, ch);
	ch->move -= sklv*25;
	send_to_char(HIC"你集中精神，藉由同伴的心靈增幅，製造出冰河崩裂的幻象！\n\r\n\r"NOR, ch);
    } else {
        send_to_char(HIB"你集中精神，製造出冰河爆刺的幻象！\n\r"NOR, ch);
    }

    // damage incrases under powerful mode (at sklv 6 is 130%)
    if (is_powerful_mode) {
        base_damage = base_damage * (112 + sklv * 3) / 100;
    }

    for( victim = ch->in_room->people; victim;victim = victim_next)
    {
    	victim_next = victim->next_in_room;
        if( ch == victim || is_same_group( ch, victim ))
		continue;

	if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) ) ||
	    ( !IS_NPC( ch ) && ( victim->fighting == ch ||
	    ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) )
	     ){
		if ( !check_blind ( victim ) || !IS_AWAKE( victim ) || IS_SET(victim->special, ANTI_MAGIC) ||
		     (!IS_NPC(victim) && (IS_SET(victim->act, PLR_WIZINVIS) || is_affected( victim, gsn_blindness))))
			continue;


		//Race check add by Neversay at 2004/June/23
		if((victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
		    victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
		    victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
		    victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
		    victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost")))
		{
			act("$N"HIW"不受冰河刺擊影響!"NOR"", ch, NULL, victim, TO_CHAR);
			continue;
		}

		is_will_strong = (is_affected(victim, gsn_will_fortress)? 2:1);

		//Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
		//If victim has spell "will fortress", the change reduce half.
		if (( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 ) +
		    (UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv*5
		    > (is_will_strong * number_percent() * 100 / weakness_percent(victim, sn)) )
		{
		    act( "$N"HIB"似乎被什麼東西擊中，開始哀嚎!"NOR"", ch, NULL, victim, TO_NOTVICT );
		    if(is_powerful_mode == TRUE)
			act( ""HIC"無數片巨大的冰河崩毀碎片向你迎面飛來!"NOR"", ch, NULL, victim, TO_VICT );
		    else
			act( ""HIB"無數根巨大的尖冰長刺向你迎面飛來!"NOR"", ch, NULL, victim, TO_VICT );

		    //The damage percent add for sklv+1 times, it has 0~88% to
		    //add 10~22% damage per time.
		    //The damage power of mind blast heavyly depending on int.
		    for(dam_percent = 100,sk_counter = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);sk_counter>0;sk_counter--)
			if((get_curr_int(ch)<<2)-12 > number_percent())
			    dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);

        dam = base_damage * new_saves_spell(ch->pp, victim, MAGIC_COLD) / 100;
        dam = number_range(dam * 2, dam * 6);

        dam = (dam * dam_percent* weakness_damage( victim, sn )) / 10000;
		    magic__damage( ch, victim, dam, sn, WEAR_NONE );

		    if ( victim->deleted || is_affected( victim, sn ))
			continue;

		    if(!IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_INT){
			af.type = sn;
			af.duration  = 1 + (sklv<<1);
			af.bitvector = 0;
			af.location  = APPLY_SAVING_COLD;
			af.modifier  = 1 + (sklv<<1);
			affect_to_char( victim, &af );
		    }

        if (isAffectedByIllusion(victim, sn)) continue;

		    if(is_powerful_mode == TRUE)
			act( "$N"HIC"被你的幻術迷惑，受到重創!"NOR"", ch, NULL, victim, TO_CHAR );
		    else
			act( "$N"HIB"被你的幻術迷惑，受到重創!"NOR"", ch, NULL, victim, TO_CHAR );

		    af.type      = sn;
		    af.duration  = sklv + 1;
		    af.bitvector = 0;
		    //Special power mode, for weakening enemies more.
		    if( (is_powerful_mode == TRUE) && (ch->mana >= 5*sklv) && (ch->move >= 5*sklv)
		    	){
			if(counter < 10){
			    ch->mana -= sklv*5;
			    ch->move -= sklv*5;
			    counter++;
			}
			af.location  = APPLY_DAMROLL;
			af.modifier  = UMIN((percent_array[sklv]*victim->damroll)/100, 0 - sklv * 2) - sklv * 2;
			affect_to_char( victim, &af );
			af.location  = APPLY_ADAMAGE;
			af.modifier  = UMIN((percent_array[sklv]*victim->apdr)/100,0 - sklv * 5) - sklv * 5;
			affect_to_char( victim, &af );
			act( "$N"HIC"被你的幻術迷惑，身陷其中，無法自拔!"NOR"", ch, NULL, victim, TO_CHAR );
		    }
		    af.location  = APPLY_STR;
		    af.modifier  = -sklv / 2 - ((sklv >= 6)? 2:1);
		    affect_to_char( victim, &af );
		}
		else
		    act( "$N"HIB"識破你的幻術!"NOR"", ch, NULL, victim, TO_CHAR );
	}
    }
    return;
}

//spell create by Neversay at 2004/July/12
void spell_ghosts_celebration( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim, *victim_next = NULL;
    AFFECT_DATA af;
    int	       dam, is_will_strong = 1;
    int sklv = get_skill_level(ch,skill_lookup("banshee wail")), sk_counter, dam_percent = 100;

    if ( IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
	send_to_char( HIB"在一個發不出聲音的地方製造聲音沒有生物會相信。\n\r"NOR, ch );
	return;
    }
    if ( time_info.hour > 4+(sklv>>1) && time_info.hour < 20-(sklv>>1) )
    {
        send_to_char( HIB"大白天製造鬼魂的聲音誰會信？"NOR"\n\r", ch );
        return;
    }

    if( ch->deleted || !ch->in_room) return;
    send_to_char(HIB"你集中精神，製造出許\多幻象！\n\r"NOR, ch);

    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
	victim_next = victim->next_in_room;
        if( ch == victim || is_same_group( ch, victim )) continue;

	if (( IS_NPC( ch ) && ( victim->fighting == ch || !IS_NPC( victim ) ) )
		|| ( !IS_NPC( ch )
			&& ( victim->fighting == ch
				|| ( IS_NPC( victim )
					&& !IS_AFFECTED( victim, AFF_CHARM )
					&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) ) ) )
		{

			if ( !IS_AWAKE( victim ) || ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS)))
				continue;

			if( IS_SET(victim->special, ANTI_MAGIC)) continue;
			//Race check add by Neversay at 2004/June/23
			if (victim->race == race_lookup("Vampire") || victim->race == race_lookup("Undead") ||
				victim->race == race_lookup("Elemental") || victim->race == race_lookup("Object") ||
				victim->race == race_lookup("Mist") || victim->race == race_lookup("Elementx") ||
				victim->race == race_lookup("Phantom") || victim->race == race_lookup("Statue") ||
				victim->race == race_lookup("Robot") || victim->race == race_lookup("Ghost") || 
				IS_SET( victim->special, NO_FEAR ))
			{
				act("$N"HIW"不受百鬼夜行影響!"NOR"", ch, NULL, victim, TO_CHAR);
				continue;
			}

			is_will_strong = (is_affected(victim, gsn_will_fortress)? 2:1);

			//Hit chance, from 30% to 80% to boss (if boss svs = -40, pp of ch is 0~40, sklv = 0~6)
			//If victim has spell "will fortress", the change reduce half.
			if (( ( UMAX(get_curr_int(ch) - get_curr_wis(victim), 0) + 5 ) * 4 ) +
				(UMAX(saves_spell(ch->pp,victim),0)>>1) + sklv*5
				> (is_will_strong * number_percent() * 100 / weakness_percent(victim, sn)) )
			{
				act( "$N"HIB"被你的幻術迷惑，開始害怕莫名!"NOR"", ch, NULL, victim, TO_CHAR );
				act( "$N"HIB"似乎被什麼東西驚嚇，全身開始害怕莫名!"NOR"", ch, NULL, victim, TO_NOTVICT );
				act( ""HIB"四周突然爆出許\多幽魂的哭泣及交談聲"NOR"!", ch, NULL, victim, TO_VICT );

				//The damage percent add for sklv+1 times, it has 0~88% to
				//add 10~22% damage per time.
				//The damage power of mind blast heavyly depending on int.
				for(dam_percent = 100,sk_counter = affected_value(victim, gsn_mind_blast, APPLY_SAVING_SPELL);sk_counter>0;sk_counter--)
					if((get_curr_int(ch)<<2)-12 > number_percent())
					dam_percent += UMAX(get_curr_int(ch)-(get_curr_wis(victim)>>1), 5);

				if(!IS_NPC(ch)) dam = dice( 20 + WIS_DIFF( ch, victim ), 
				level+sklv ); else dam = dice( 15 + WIS_DIFF( ch, victim ), 
				level );

				// Keric modfied to magic_damage
						//dam -= victim->mdr;
				dam = (dam * dam_percent * weakness_damage(victim, sn)) / 10000;

				if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
					continue;

				if ( victim->deleted || is_affected( victim, sn ) ||
					(IS_NPC(victim) && IS_SET( victim->act, ACT_SENTINEL) ))
					continue;

				af.type      = sn;
				af.duration  = 8;
				af.location  = APPLY_WIS;
				af.modifier  = -1;
				af.bitvector = AFF_FEAR;
				affect_join( victim, &af );
			}
    	}
    }
}

