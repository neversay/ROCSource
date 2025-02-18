//*****************************************************************
//*  PROGRAM ID  : spell_assist.c                                 *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940610 jye                                    *
//*  PURPOSE     :         修改fly只有天人不會被蓋掉              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940611 jye                                    *
//*  PURPOSE     :         修改armor,shield,stone                 *
//*              :             lv4以上不會被lv4以下蓋掉           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991229 tale                                    *
//*  PURPOSE     :         修改flaming_armor                *
//*              :             將 mdr & svfire 加入影響傷害變數           *
//*****************************************************************
/* $Id: spell_assist.c 1402 2005-06-12 13:24:20Z jye $ */
#include "skill.h"
// modified by jye 6.11.2005
void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn) ;
	int intArmor, intShield ,intStone;
	int nLevel   = ch->level;
	// obj lv effect , add at 2020/10/21
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}
	if(nLevel > 42) nLevel = 42;
	int nAcBonus = sklv * nLevel / 2;
	// 統一 armor , stone , shield 的pdr mdr 加成值 2022/01/13
	int nSkBonus = 0;
	if( sklv > 0 ) nSkBonus += 4;
	if( sklv > 1 ) nSkBonus += 8;
	if( sklv > 2 ) nSkBonus += 8;
	if( sklv > 3 ) nSkBonus += 8;
	if( sklv > 4 ) nSkBonus += 6;
	if( sklv > 5 ) nSkBonus += 6;
  
	intArmor  = skill_lookup("armor");
	intShield = skill_lookup("shield");
	intStone  = skill_lookup("stone skin");

	if(sklv >= 4){
		nAcBonus += nLevel * (sklv - 3) * (sklv - 3) / 2;
	}
	if(nAcBonus < 200) nAcBonus = (nAcBonus + 200) / 2;
  
	if(!IS_NPC(ch) && ch == vo && ch->pcdata->prime_attr == APPLY_INT) nAcBonus += nAcBonus / 2;

	if( is_affected( victim, intArmor)) 
	{
		if( affected_value(victim,intArmor,APPLY_SAVING_SPELL) == 0)
			if( affected_value(victim,sn,APPLY_AC) > -2 - nAcBonus)
				affect_strip( victim, intArmor);
			else
				return; 
		else 
		{
			if( sklv > 3)
				affect_strip( victim, intArmor);
			else
				return; 
		}
	}
	else if( is_affected( victim, intShield))
	{
		if( affected_value(victim,intShield,APPLY_RMAGIC) != 0 && sklv < 4)
			return; 
		else
			affect_strip( victim, intShield);
	}
	else if( is_affected( victim, intStone))
	{
		if( affected_value(victim,intStone,APPLY_RDAMAGE) != 0 && sklv < 4)
			return; 
		else
			affect_strip( victim, intStone);
	}

	af.type      = sn;
	af.duration  = 10 + nLevel/2 + sklv * 4;
	af.location  = APPLY_AC;
	af.modifier  = 0 - nAcBonus;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	//if( get_skill_level(ch,sn) > 3 )
	if( sklv > 0 )
	{
		af.type     = sn;
		af.duration = 10 + nLevel/2 + sklv * 4;
		af.location = APPLY_RDAMAGE;
		//af.modifier = (sklv * sklv - 6)/2;
		af.modifier = nSkBonus/2;
		// 追加 cle base class 對自己放也有兩倍 2022/01/13
		if(!IS_NPC(ch) && ch == vo && (ch->pcdata->prime_attr == APPLY_INT || ch->pcdata->prime_attr == APPLY_WIS) )
			af.modifier += af.modifier / 2;
		af.bitvector = 0;
		affect_to_char( victim, &af );

		af.duration  = 10 + nLevel/2 + sklv * 4;
		af.location  = APPLY_RMAGIC;
		//af.modifier = (sklv * sklv - 6)/2;
		af.modifier = nSkBonus/2;
		if(!IS_NPC(ch) && ch == vo && (ch->pcdata->prime_attr == APPLY_INT || ch->pcdata->prime_attr == APPLY_WIS) )
			af.modifier += af.modifier / 2;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}

	if( ch != victim )
		send_to_char( "好的。\n\r", ch );
    
	send_to_char( "你感覺受到保護.\n\r", victim );
	return;
}
// modified by whisperwind 9.17.2004

// prot. from spells modified by whisperwind 9.18.2004
void spell_pcold( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv   = get_skill_level(ch, sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( is_affected( victim, skill_lookup("prot. from fire")))
		affect_strip( victim, skill_lookup("prot. from fire"));
	if ( is_affected( victim, skill_lookup("prot. from cold"))) {
		if(affected_value(victim,sn,APPLY_SAVING_COLD) > -2 - ch->level / 3 - sklv*2 - ( UMAX( 0, sklv - 4) * 2) )
			affect_strip( victim, skill_lookup("prot. from cold"));
		else
			return;
	}
	if ( is_affected( victim, skill_lookup("prot. from wind")))
		affect_strip( victim, skill_lookup("prot. from wind"));
	if ( is_affected( victim, skill_lookup("prot. from earth")))
		affect_strip( victim, skill_lookup("prot. from earth"));
	if ( is_affected( victim, skill_lookup("prot. from lightning")))
		affect_strip( victim, skill_lookup("prot. from lightning"));

	af.type	 = sn;
	af.duration	 = 4 + (ch->level / 8) + get_skill_level(ch, sn) * 2 / 3;
	af.bitvector = 0;
	af.location	 = APPLY_SAVING_COLD;
	af.modifier	 = -1 - ch->level / 3 - get_skill_level(ch, sn)*2 - ( UMAX( 0, get_skill_level(ch,sn) - 4) * 2);
	affect_to_char( victim, &af );
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你周圍升起了一道寒冰防壁.\n\r", victim );
	return;
}

void spell_pwind( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( is_affected( victim, skill_lookup("prot. from fire")))
		affect_strip( victim, skill_lookup("prot. from fire"));
	if ( is_affected( victim, skill_lookup("prot. from cold")))
		affect_strip( victim, skill_lookup("prot. from cold"));
	if ( is_affected( victim, skill_lookup("prot. from wind"))) {
		if(affected_value(victim,sn,APPLY_SAVING_WIND) > -2 - ch->level / 3 - sklv*2 - ( UMAX( 0, sklv - 4) * 2) )
			affect_strip( victim, skill_lookup("prot. from wind"));
		else
			return;
	}
	if ( is_affected( victim, skill_lookup("prot. from earth")))
		affect_strip( victim, skill_lookup("prot. from earth"));
	if ( is_affected( victim, skill_lookup("prot. from lightning")))
		affect_strip( victim, skill_lookup("prot. from lightning"));

	af.type	 = sn;
	af.duration	 = 4 + (ch->level / 8) + get_skill_level(ch, sn) * 2 / 3;
	af.bitvector = 0;
	af.location	 = APPLY_SAVING_WIND;
	af.modifier	 = -1 - ch->level / 3 - get_skill_level(ch, sn)*2 - ( UMAX( 0, get_skill_level(ch,sn) - 4) * 2);
	affect_to_char( victim, &af );
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你周圍升起了一道疾風防壁.\n\r", victim );
	return;
}

void spell_pearth( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( is_affected( victim, skill_lookup("prot. from fire")))
		affect_strip( victim, skill_lookup("prot. from fire"));
	if ( is_affected( victim, skill_lookup("prot. from cold")))
		affect_strip( victim, skill_lookup("prot. from cold"));
	if ( is_affected( victim, skill_lookup("prot. from wind")))
		affect_strip( victim, skill_lookup("prot. from wind"));
	if ( is_affected( victim, skill_lookup("prot. from earth"))) {
		if(affected_value(victim,sn,APPLY_SAVING_EARTH) > -2 - ch->level / 3 - sklv*2 - ( UMAX( 0, sklv - 4) * 2) )
			affect_strip( victim, skill_lookup("prot. from earth"));
		else
			return;
	}
	if ( is_affected( victim, skill_lookup("prot. from lightning")))
		affect_strip( victim, skill_lookup("prot. from lightning"));

	af.type	 = sn;
	af.duration	 = 4 + (ch->level / 8) + get_skill_level(ch, sn) * 2 / 3;
	af.bitvector = 0;
	af.location	 = APPLY_SAVING_EARTH;
	af.modifier	 = -1 - ch->level / 3 - get_skill_level(ch, sn)*2 - ( UMAX( 0, get_skill_level(ch,sn) - 4) * 2);
	affect_to_char( victim, &af );
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你周圍升起了一道大地防壁.\n\r", victim );
	return;
}

void spell_pfire( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( is_affected( victim, skill_lookup("prot. from fire"))) {
		if(affected_value(victim,sn,APPLY_SAVING_FIRE) > -2 - ch->level / 3 - sklv*2 - ( UMAX( 0, sklv - 4) * 2) )
			affect_strip( victim, skill_lookup("prot. from fire"));
		else
			return;
	}
	if ( is_affected( victim, skill_lookup("prot. from cold")))
		affect_strip( victim, skill_lookup("prot. from cold"));
	if ( is_affected( victim, skill_lookup("prot. from wind")))
		affect_strip( victim, skill_lookup("prot. from wind"));
	if ( is_affected( victim, skill_lookup("prot. from earth")))
		affect_strip( victim, skill_lookup("prot. from earth"));
	if ( is_affected( victim, skill_lookup("prot. from lightning")))
		affect_strip( victim, skill_lookup("prot. from lightning"));

	af.type	 = sn;
	af.duration	 = 4 + (ch->level / 8) + get_skill_level(ch, sn) * 2 / 3;
	af.bitvector = 0;
	af.location	 = APPLY_SAVING_FIRE;
	af.modifier	 = -1 - ch->level / 3 - sklv*2 - ( UMAX( 0, sklv - 4) * 2);
	affect_to_char( victim, &af );
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你周圍升起了一道火焰防壁.\n\r", victim );
	return;
}

void spell_plightning( int sn, int level, CHAR_DATA *ch, void* vo )
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( is_affected( victim, skill_lookup("prot. from fire")))
		affect_strip( victim, skill_lookup("prot. from fire"));
	if ( is_affected( victim, skill_lookup("prot. from cold"))) 
		affect_strip( victim, skill_lookup("prot. from cold"));
	if ( is_affected( victim, skill_lookup("prot. from wind")))
		affect_strip( victim, skill_lookup("prot. from wind"));
	if ( is_affected( victim, skill_lookup("prot. from earth")))
		affect_strip( victim, skill_lookup("prot. from earth"));
	if ( is_affected( victim, skill_lookup("prot. from lightning")))
	{
		if(affected_value(victim,sn,APPLY_SAVING_LIGHTNING) > -2 - ch->level / 3 - sklv*2 - ( UMAX( 0, sklv - 4) * 2) )
			affect_strip( victim, skill_lookup("prot. from lightning"));
		else
			return;
	}

	af.type 	 = sn;
	af.duration  = 4 + (ch->level/8) + get_skill_level(ch,sn)*2/3;
	af.bitvector = 0;
	af.location  = APPLY_SAVING_LIGHTNING;
	af.modifier	 = -1 - ch->level / 3 - get_skill_level(ch, sn)*2 - ( UMAX( 0, get_skill_level(ch,sn) - 4) * 2);
	affect_to_char( victim, &af );
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你周圍升起了一道閃電防壁.\n\r", victim );
	return;
}


// day of power modified by whisperwind 10.31.2004
void spell_day_of_power( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int mana_cost = 20 + 5 * get_skill_level(ch, sn); // add at 2022/01/08

	for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room )
	{
		// 魔力不足判定 2022/01/08
		if( ch->mana < mana_cost ){
			send_to_char( "你的魔力不夠對所有隊員施放聖戰之詩。\n\r", ch );
			break;
		}
		if( !is_same_group(ch, victim))
			continue;
		if( victim->deleted )
			continue;

		// 移除戰鬥中無法附加的設定 2022/01/08
		//if(victim->position != POS_FIGHTING) {

		if( is_affected( victim, skill_lookup("holy crown"))){
			if(victim->pp > victim->damroll)
				continue;
			affect_strip( victim, skill_lookup("holy crown")); 
		}
		if( is_affected( victim, skill_lookup("day of power"))) {
			affect_strip( victim, skill_lookup("day of power")); 
		}

		// mana 從固定 20 調升為 20 + 5 * sklv , 且施法者在戰鬥時將消耗 3 倍 2022/01/08
		if( victim->fighting ) {
			if ( ch->mana < mana_cost * 3 )
				continue;
			ch->mana -= mana_cost * 3;
		}else{
			ch->mana -= mana_cost;
		}

		af.type       = sn;
		af.duration   = 1 + get_skill_level(ch, sn)/5 + get_skill_level(ch, sn)/6;
		af.bitvector  = 0;
		af.location   = APPLY_HITROLL;
		af.modifier   = get_curr_wis(ch)/5 + get_skill_level(ch, sn)*2;
		affect_to_char( victim, &af );

		af.location   = APPLY_DAMROLL;
		af.modifier   = get_curr_wis(ch)/5 + get_skill_level(ch, sn)*2;
		affect_to_char( victim, &af );

		af.location   = APPLY_ADAMAGE;
		af.modifier   = get_curr_wis(ch)/5 + get_skill_level(ch, sn)*get_skill_level(ch, sn) + get_skill_level(ch, sn)*2;
		affect_to_char( victim, &af );
		// 新增 ac 加成 , 受 con 影響 2022/01/08
		af.location   = APPLY_AC;
		af.modifier   = -( get_curr_con(ch)*2 + get_skill_level(ch, sn)*get_skill_level(ch, sn) + get_skill_level(ch, sn)*30 );
		affect_to_char( victim, &af );

		send_to_char( HIW"你感覺到一首聖戰詩篇在你心中吟唱著."NOR"\n\r", victim );
	}
	return;
}

void spell_day_of_brightness( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	CHAR_DATA  *lord;

	int wait, time = 0;
	int  nSklv = get_skill_level(ch, sn);
	bool isBattleCast = FALSE;
	AFFECT_DATA af;
   
	if( !ch->in_room ) return;

	if( !IS_NPC(ch) ) {
		for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room )
		{
			if( IS_NPC(ch) ) continue;
			if( !is_same_group( ch, victim ) || ch == victim ) continue;
			if( victim -> class == LORD) break;
		}
		if( nSklv < 6 && !victim ) {
			send_to_char("你找不到領主。\n\r", ch);
			return;
		} else if (nSklv >= 6 && victim == NULL) {
			victim = ch;
		}
		if( victim->position != POS_STANDING ){
			isBattleCast = TRUE;
			if(nSklv < 6) {
				send_to_char("你的技巧還不足以在戰鬥中施展明光之鏡。\n\r", ch);
				return;
			}
			if(ch->mana < 125) {
				send_to_char("你的法力不足以在戰鬥中施展明光之鏡。\n\r", ch);
				return;
			}
			ch->mana -= 125;
		}
		if( victim->mana < 150 ) {
			send_to_char("你的夥伴無法配合你。\n\r", ch);
			return;
		}
		WAIT_MAGIC_STATE(victim,skill_table[sn].beats/2);
		victim->mana -= 150;
		lord = victim;
		ch->mana += nSklv * 3 + nSklv * 1;
	}else{
		lord = ch;
	}

	time = 24 + nSklv * 4;

	for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room )
	{
		if( !is_same_group(victim,ch)) continue;
		if( victim->deleted ) continue;
		wait = ch->wait;

		if(is_affected( victim, skill_lookup( "sun flash" ) ) || is_affected( victim, gsn_blindness ))
		{
			if(!isBattleCast) spell_cure_blindness(sn, level, ch, (void*)victim);
		}

		if (!(IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_MAGIC))){
			if (!( IS_AFFECTED( victim, AFF_DETECT_MAGIC ))){
				af.type   = skill_lookup("detect magic");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_MAGIC;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神賜給你分辨魔法的能力."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect magic"))){
				affect_strip( victim, skill_lookup("detect magic"));
				af.type   = skill_lookup("detect magic");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_MAGIC;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神賜給你分辨魔法的能力."NOR"\n\r", victim );
			}
		}

		if (!(IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_ALIGN))){
			if (!( IS_AFFECTED( victim, AFF_DETECT_EVIL ))){
				af.type   = skill_lookup("detect evil");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_EVIL;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神賜予你分辨邪惡的能力."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect evil"))){
				affect_strip( victim, skill_lookup("detect evil"));
				af.type   = skill_lookup("detect evil");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_EVIL;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神賜予你分辨邪惡的能力."NOR"\n\r", victim );
			}
		}

		if (!(IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_INVIS))){
			if (!( IS_AFFECTED( victim, AFF_DETECT_INVIS ))){
				af.type   = skill_lookup("detect invis");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_INVIS;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神的神力令你感到雙眼一陣刺痛."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect invis"))){
				affect_strip( victim, skill_lookup("detect invis"));
				af.type   = skill_lookup("detect invis");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_INVIS;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神的神力令你感到雙眼一陣刺痛."NOR"\n\r", victim );
			}
		}
    
		if (!(IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_HIDDEN)))
		{
			if (!( IS_AFFECTED( victim, AFF_DETECT_HIDDEN ))){
				af.type   = skill_lookup("detect hidden");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_HIDDEN;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神的神力令你得以察覺藏匿生物."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect hidden"))){
				affect_strip( victim, skill_lookup("detect hidden"));
				af.type   = skill_lookup("detect hidden");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_HIDDEN;
				affect_to_char( victim, &af );
				send_to_char( HIC"水神的神力令你得以察覺藏匿生物."NOR"\n\r", victim );
			}
		}

		if ( is_affected( victim, sn))
		{
			affect_strip( victim, sn);
		}
		// 抗光爆
		af.type      = sn;
		af.duration   = UMAX(0,ch->level - 33) + nSklv * 2;
		af.location   = APPLY_DOB_SAVING_FLASH;
		af.modifier   = ( nSklv + 1 ) * 9 + UMAX( get_curr_wis(ch) - 16, 0)* 3 ;
		af.bitvector = 0;
		affect_to_char( victim, &af );

		af.location   = APPLY_SAVING_SPELL;
		// 修改 dob svs 曲線 2023/01/12
		//af.modifier   = - 2 - (get_curr_wis(ch)-10)/3 - nSklv - nSklv/3;
		af.modifier   = 0 - ( 2 + nSklv + nSklv*nSklv / 6 );
		af.bitvector = 0;
		affect_to_char( victim, &af );

		if( nSklv > 3 ){
			// dob element 加成的部分修改為 mdr 2023/01/12
			//af.location  = APPLY_SAVING_ELEMENT;
			af.location  = APPLY_RMAGIC;
			af.modifier  = (nSklv*4 + nSklv*nSklv)/2;
			af.bitvector = 0;
			affect_to_char( victim, &af );
			send_to_char( HIC"你感覺受到水神的加護."NOR"\n\r", victim );
		}
    
		ch->wait = wait + 1;
		if ( ch != victim )
			send_to_char( "好的。\n\r", ch );
			send_to_char( HIW"天上降下一道神光，照亮你的雙眼."NOR"\n\r", victim );
	}
	return;
}

void spell_day_of_prot( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	int nDopSn =  skill_lookup("day of protection");
	int mod = 0; // 2023/01/12

	// 重新設計 dop 的 sklv 曲線 2023/01/12
	mod = 0 - ( 3 + sklv + (sklv*sklv)/6 );

	if( !ch->in_room ) return;
	if( !IS_NPC(ch) ) {
		for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room ) {
			if( IS_NPC(ch) ) continue;
			if( !is_same_group( ch, victim ) || ch == victim ) continue;
			if( victim -> class == SAVANT ) break;
		}
		if( sklv < 6 ){
			if( !victim ) {
				send_to_char("你找不到賢者。\n\r", ch);
				return;
			}
			if( victim->position != POS_STANDING || victim->mana < 100 ) {
				send_to_char("你的夥伴無法配合你。\n\r", ch);
				return;
			}
			WAIT_MAGIC_STATE(victim,skill_table[sn].beats/2);
			victim->mana -= 100;
		}
		ch->mana += sklv * 11 - 9;
	}

	af.type	 = nDopSn;
	af.duration	 = 6 + sklv*3;
	af.bitvector = 0;
	for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room ) {
		if( !is_same_group(ch, victim)) continue;
		if( victim->deleted ) continue;
		if ( is_affected( victim, nDopSn)) {
			if(affected_value(victim,nDopSn,APPLY_SAVING_ELEMENT) >= mod )
				affect_strip( victim, nDopSn);
			else
				continue; 
		}
		//af.modifier    = -1 - sklv - sklv/4*3;
		af.modifier    = mod;
		af.location    = APPLY_SAVING_ELEMENT;
		affect_to_char( victim, &af );
		af.location    = APPLY_SAVING_BREATH;
		affect_to_char( victim, &af );
		if ( ch != victim )
			send_to_char( "好的。\n\r", ch );
		send_to_char( "你周圍升起了一道神聖防壁.\n\r", victim );
	}
	return;
}

// holy crown modified by whisperwind 10.31.2004
void spell_holy_crown( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int mana_cost = 20 + 5 * get_skill_level(ch, sn); // add at 2022/01/08

	for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room ) {
		// 魔力不足判定 2022/01/08
		if( ch->mana < mana_cost ){
			send_to_char( "你的魔力不夠對所有隊員施放聖之冠冕。\n\r", ch );
			break;
		}
		if( !is_same_group(ch, victim))
			continue;
		if( victim->deleted )
			continue;

		// 移除戰鬥中無法附加的設定 2022/01/08
		//if (victim->position != POS_FIGHTING) {

		if( is_affected( victim, skill_lookup("holy crown")) ){
			affect_strip( victim, skill_lookup("holy crown"));
		}
		if( is_affected( victim, skill_lookup("day of power")) ){
			if(victim->pp < victim->damroll) continue;
			affect_strip( victim, skill_lookup("day of power"));
		}

		// mana 從固定 20 調升為 20 + 5 * sklv , 且被施法者在戰鬥時將消耗 3 倍 2022/01/08
		if( victim->fighting ) {
			if( ch->mana < mana_cost * 3 )
				continue;
			ch->mana -= mana_cost * 3;
		}else{
			ch->mana -= mana_cost;
		}

		af.type      = sn;
		af.duration	 = 1 + get_skill_level(ch, sn)/5 + get_skill_level(ch, sn)/6;
		af.bitvector = 0;

		af.location	 = APPLY_STR;
		af.modifier	 = 1 + get_skill_level(ch, sn) / 3;
		affect_to_char( victim, &af );
		
		af.location	 = APPLY_INT;
		af.modifier	 = 1 + get_skill_level(ch, sn) / 3;
		affect_to_char( victim, &af );
		
		af.location	 = APPLY_WIS;
		af.modifier	 = 1 + get_skill_level(ch, sn) / 3;
		affect_to_char( victim, &af );
		
		af.location	 = APPLY_DEX;
		af.modifier	 = 1 + get_skill_level(ch, sn) / 3;
		affect_to_char( victim, &af );
		
		af.location	 = APPLY_CON;
		af.modifier	 = 1 + get_skill_level(ch, sn) / 3;
		affect_to_char( victim, &af );
		
		af.location	 = APPLY_POWER_POTENTIAL;
		af.modifier	 = 1 + (get_curr_wis(ch) - 10)/5 + get_skill_level(ch, sn)/2;
		affect_to_char( victim, &af );
		
		af.location	 = APPLY_AMAGIC;
		af.modifier	 = get_curr_wis(ch) / 5 + get_skill_level(ch, sn) * 2 + get_skill_level(ch, sn) / 2;
		affect_to_char( victim, &af );

		send_to_char( HIW"你的四周升起一股強大的靈氣!"NOR"\n\r", victim );
	}
	return;
}

void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}

	if ( victim->position == POS_FIGHTING )
		return;

	if ( is_affected( victim, skill_lookup("bless")))
	{
		// 修正 bless 覆蓋數值與加成值不相等的問題, 並把 > 修改為 >= 讓同等級能覆蓋舊的 buff 2023/01/12
		//if( affected_value(victim,sn,APPLY_SAVING_SPELL) > -3 - (get_curr_wis(ch)-10)/3 - sklv - sklv/3 )
		if( affected_value(victim,sn,APPLY_SAVING_SPELL) >= - 5 - (get_curr_wis(ch)-10)/3 - sklv * 2 - sklv/2 )
			affect_strip( victim, skill_lookup("bless"));
		else
			return;
	}
		
	af.type	 = sn;
	af.duration	 = 1 + sklv * 3 + ch->level/4;
	af.location	 = APPLY_SAVING_SPELL;
	af.modifier	 = - 5 - (get_curr_wis(ch)-10)/3 - sklv * 2 - sklv/2;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	af.location  = APPLY_SAVING_SAINT;
	af.modifier  = - 1 - ( sklv + sklv * (1 + sklv) * 0.2 );
	af.bitvector = 0;
	affect_to_char( victim, &af );

	af.location  = APPLY_SAVING_DARK;
	af.modifier  = - 1 - ( sklv + sklv * (1 + sklv) * 0.2 );
	af.bitvector = 0;
	affect_to_char( victim, &af );

	if( sklv > 3 ) {
		af.location  = APPLY_SAVING_BREATH;
		af.modifier  = - (sklv * 4 - 8);
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}
	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你感覺受到祝福.\n\r", victim );
	return;
}


void spell_breathe_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

    if ( IS_AFFECTED( victim, AFF_GILLS ) )
		return;

    af.type	 = sn;
    af.duration	 = ch->level + get_skill_level(ch, sn) * 6;
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_GILLS;
    affect_to_char( victim, &af );

    send_to_char( "你現在可以在水裡呼吸了.\n\r", victim );
    act( "$n 現在可以在水裡呼吸了.", victim, NULL, NULL, TO_ROOM );
    return;
}


void spell_cone_of_silence( int sn, int level, CHAR_DATA *ch, void *vo )
{
    ROOM_INDEX_DATA *pRoomIndex;

    if ( !( pRoomIndex = ch->in_room ) )
		return;

    if ( IS_SET( pRoomIndex->room_flags, ROOM_SAFE ) )
    {
		send_to_char( "這裡不能這麼做.\n\r", ch );
		return;
    }

    if ( !IS_SET( pRoomIndex->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
		SET_BIT( pRoomIndex->room_flags, ROOM_CONE_OF_SILENCE );
			
		send_to_char( "你在這裡建立了沈默力場，周圍突然安靜下來。\n\r", ch );
		act( "$n 在這裡建立了沈默力場，周圍的聲音都消失了。", ch, NULL, NULL, TO_ROOM );
    }

    return;
}

void spell_control_weather( int sn, int level, CHAR_DATA *ch, void *vo )
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 3+get_skill_level(ch,sn)*2, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 3+get_skill_level(ch,sn)*2, 4 );
    else
	send_to_char ("你要讓天氣變好(better)或變差(worse)？\n\r", ch );

    send_to_char( HIC "數個神秘的圖像文字出現在你眼前，它們的顏色變幻著，天氣似乎也因此而受到了影響。\n\r"NOR"", ch);
    act( HIC "當"NOR"$n"HIC"集中精神的同時，他面前浮現出奇異的圖像文字。"NOR"" , ch, NULL, NULL, TO_ROOM );
    return;
}


void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if( IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_ALIGN) || IS_AFFECTED( victim, AFF_DETECT_EVIL) ){
	if ( is_affected( victim, skill_lookup("detect evil")))
			affect_strip( victim, skill_lookup("detect evil"));
	else
			return;
	}

	af.type	 = sn;
	af.duration	 = ch->level + get_skill_level(ch,sn)*2;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_DETECT_EVIL;
	affect_to_char( victim, &af );

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你感覺到你能分辨邪惡的氣息了.\n\r", victim );
	WAIT_MAGIC_STATE(ch, 8);
	return;
}



void spell_detect_hidden( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if( IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_HIDDEN) || IS_AFFECTED( victim, AFF_DETECT_HIDDEN) ){
	if ( is_affected( victim, skill_lookup("detect hidden")))
			affect_strip( victim, skill_lookup("detect hidden"));
	else
			return;
	}

	af.type	 = sn;
	af.duration	 = ch->level + get_skill_level(ch,sn)*2;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_DETECT_HIDDEN;
	affect_to_char( victim, &af );

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你開始提高警覺.\n\r", victim );
	WAIT_MAGIC_STATE(ch, 8);
	return;
}

void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if( IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_INVIS) || IS_AFFECTED( victim, AFF_DETECT_INVIS ) ){
	if ( is_affected( victim, skill_lookup("detect invis")))
			affect_strip( victim, skill_lookup("detect invis"));
	else
			return;
	}

	af.type	 = sn;
	af.duration	 = ch->level + get_skill_level(ch,sn)*2;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_DETECT_INVIS;
	affect_to_char( victim, &af );

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你感到雙眼一陣刺痛.\n\r", victim );
	WAIT_MAGIC_STATE(ch, 8);
	return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if( IS_SET(race_table[victim->race].race_abilities, RACE_DETECT_MAGIC) || IS_AFFECTED( victim, AFF_DETECT_MAGIC )){
		if ( is_affected( victim, skill_lookup("detect magic")))
			affect_strip( victim, skill_lookup("detect magic"));
		else
			return;
	}

	af.type	 = sn;
	af.duration	 = ch->level + get_skill_level(ch,sn)*2;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_DETECT_MAGIC;
	affect_to_char( victim, &af );

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你感到你能分辨魔法的氣息了.\n\r", victim );
	WAIT_MAGIC_STATE(ch, 8);
	return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *obj = (OBJ_DATA *) vo;

	if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
	{
		if ( obj->value[3] != 0 )
			send_to_char( "你聞到一股劇毒的氣味.\n\r", ch );
		else
			send_to_char( "嗯，看來十分可口.\n\r", ch );
	}
	else
	{
		send_to_char( "那看起來並沒有含毒.\n\r", ch );
	}

	WAIT_MAGIC_STATE(ch, 8);
	return;
}

/* Dispel Magic recoded by Thelonius for EnvyMud */
void spell_dispel_magic ( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA *paf, *temp_paf;
	long long int                aff = 1LL;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( IS_SET( victim->special, NO_DM ) )
			return;
			
	if ( !IS_NPC( ch ) && ( !IS_NPC( victim ) || ( IS_AFFECTED( victim, AFF_CHARM ) && is_same_group( ch, victim->master ) ) ) )
	  /* Defensive spell - remove ALL effects */
	{
		for ( paf = victim->affected; paf; paf = paf->next )
		{
			if ( paf->deleted )
				continue;

			if( paf->type == gsn_snare ){
				send_to_char(" 你無法移去陷阱.\n\r", ch);
				continue;
			}

			if( paf->type == gsn_defensive )
				continue;
				
			if( paf->type == gsn_blood_thirsty )
				continue;
			
			if( paf->type == gsn_meditate ) {
				send_to_char( "你無法移去冥思.\n\r", ch);
				continue;
			}
			// 2021/11/23
			if ( paf->type == gsn_lifeshare )
			{
				send_to_char("你無法移去同生共死.\n\r", ch );
				continue;
			}
			if( paf->type == gsn_nightstalk) {
				send_to_char("你無法移去匿蹤.\n\r", ch);
				continue;
			}
			if( paf->type == gsn_sprinkle_slash) {
				send_to_char("你無法移除波紋劍氣的纏繞.\n\r", ch);
				continue;
			}
			if( paf->type == gsn_resurrection ) {
				send_to_char("你無法移去復活效應.\n\r", ch);
				continue;
			}
			/*
			if (( paf->type == skill_lookup( "polymorph other" )
				|| paf->type == gsn_morphing)
				&& get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char( "你無法移去種族變異.\n\r", ch );
				continue;
			}
			*/
			if ( paf->type == gsn_vampiric_bite && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("你無法移去吸血鬼詛咒.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_disease && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("你無法移去疾病.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_poison && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("你無法移去劇毒.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_paralyze && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char( "你無法移去麻痺效應.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_dark_containment && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("你無法移去闇咒封鎖.\n\r", ch );
				continue;
			}
			if ( paf->type == skill_lookup("demon tremor") && get_trust( ch ) < LEVEL_IMMORTAL)
			{
				send_to_char( "你無法驅趕心中盤據的惡魔.\n\r", ch );
				continue;
			}

			if ( victim->special != 0
				&& (   ( paf->type == skill_lookup( "sanctuary" ) && IS_SET( victim->special, ALY_SAN ) )
					|| ( paf->type == gsn_invis && IS_SET( victim->special, ALY_INVIS ) )
					|| ( paf->type == gsn_hide && IS_SET( victim->special, ALY_HIDE ) )
					|| ( paf->type == gsn_sneak && IS_SET( victim->special, ALY_SNEAK ) )
					|| ( paf->type == skill_lookup( "detect invis" ) && IS_SET( victim->special, ALY_DI ) )
					|| ( paf->type == skill_lookup( "detect hidden" ) && IS_SET( victim->special, ALY_DH ) )
					|| ( paf->type == skill_lookup( "infravision" ) && IS_SET( victim->special, ALY_INF ) )
					|| ( paf->type == skill_lookup( "barrier" ) && IS_SET( victim->special, ALY_BAR ) ) ) )
				continue;

			if( paf->duration < 0 ) continue;
			if( ( number_percent( ) * 100 / weakness_percent( victim, sn) )
				> (paf->duration - ch->level - ( get_curr_wis( ch ) * 2 ) - get_skill_level(ch,sn)*6 ) )
			{
				for ( temp_paf = victim->affected; temp_paf; temp_paf = temp_paf->next )
				{
					if (temp_paf->type == paf->type )
					{
						affect_remove( victim, temp_paf );
						if(temp_paf->type == gsn_polymorph || temp_paf->type == gsn_morphing )
						{
							if( temp_paf->type == gsn_morphing )
								victim->temp_race = '\0' ; //NULL;
							dm_effect( victim );
						}
						// 將 fall_effect 移到最後判定 2023/01/15
						//if( temp_paf->type == gsn_flying )
						//	fall_effect( victim );
					}
				}
			}else{
				send_to_char( "你失敗了！\n\r", ch );
				continue;
			}
		}

		if ( victim == ch )
		{
			act( "你除去了自己身上的法術效應.", ch, NULL, NULL, TO_CHAR );
			act( "$n除去了自己身上的法術效應.", ch, NULL, NULL, TO_ROOM );
		}
		else
		{
			act( "你除去了$N身上的法術效應.", ch, NULL, victim, TO_CHAR );
			act( "$n除去了你身上的法術效應.", ch, NULL, victim, TO_VICT );
			act( "$n除去了$N身上的法術效應.", ch, NULL, victim, TO_NOTVICT );
		}
		return;
	}
	else
	{   /* Offensive spell - enforced by multi_hit whether succeeds or fails */
		for ( paf = victim->affected; paf; paf = paf->next )
		{
			if ( paf->deleted )
				continue;

			if ( victim->special != 0
				&& (   ( paf->type == skill_lookup( "sanctuary" ) && IS_SET( victim->special, ALY_SAN ) )
					|| ( paf->type == gsn_invis && IS_SET( victim->special, ALY_INVIS ) )
					|| ( paf->type == gsn_hide && IS_SET( victim->special, ALY_HIDE ) )
					|| ( paf->type == gsn_sneak && IS_SET( victim->special, ALY_SNEAK ) )
					|| ( paf->type == skill_lookup( "detect invis" ) && IS_SET( victim->special, ALY_DI ) )
					|| ( paf->type == skill_lookup( "detect hidden" ) && IS_SET( victim->special, ALY_DH ) )
					|| ( paf->type == skill_lookup( "infravision" ) && IS_SET( victim->special, ALY_INF ) )
					|| ( paf->type == skill_lookup( "barrier" ) && IS_SET( victim->special, ALY_BAR ) ) ) )
			continue;

			if ( IS_IMMORTAL( ch ) || UMAX( 5, saves_spell( ch->pp, victim ) + ( WIS_DIFF(ch, victim) * 4 ) - 23 + get_skill_level(ch,sn)*2 )
				> (number_percent( ) * 100 / weakness_percent( victim, sn)) )
			{
				send_to_char( skill_table[paf->type].msg_off, victim );
				send_to_char( "\n\r", victim );
				// send_to_char( "OK\n\r", ch );
				act( "$n不再受到 '$t' 的作用影響.", victim, skill_table[paf->type].name, NULL, TO_ROOM );
				affect_strip( victim, paf->type );
				return;
			}
			else
			{
				send_to_char( "你失敗了！\n\r", ch );
				if ( ( !victim->fighting || !is_same_group( ch, victim->fighting ) ) && number_bits( 4 ) == 1 ){
					multi_hit( victim, ch, TYPE_UNDEFINED );
				}
				return;
			}
			/* ALWAYS give a shot at removing sanctuary
			if ( IS_AFFECTED( victim, AFF_SANCTUARY)
				&& !saves_spell( ch->pp, victim ) )
			{
				REMOVE_BIT( victim->affected_by, AFF_SANCTUARY );
				send_to_char( "The white aura around your body fades.\n\r",
										victim );
				act( "The white aura around $n's body fades.",
					victim, NULL, NULL, TO_ROOM );
			}*/
		}

		if ( victim->affected_by != 0 )
		{
			for ( ; aff <= 21990232555552LL; aff <<= 1 )
			{
				if ( !IS_SET( victim->affected_by, aff ) )
					continue;

				// pet 不會被移除 charm 2022/12/24
				if ( aff == AFF_CHARM && ( IS_NPC(victim) && IS_SET( victim->act, ACT_PET ) ) )
					continue;

				if ( victim->special != 0
					&& (   ( aff == AFF_INVISIBLE && IS_SET( victim->special, ALY_INVIS ) )
						|| ( aff == AFF_DETECT_INVIS && IS_SET( victim->special, ALY_DI ) )
						|| ( aff == AFF_DETECT_HIDDEN && IS_SET( victim->special, ALY_DH ) )
						|| ( aff == AFF_SANCTUARY && IS_SET( victim->special, ALY_SAN ) )
						|| ( aff == AFF_HIDE && IS_SET( victim->special, ALY_HIDE ) )
						|| ( aff == AFF_SNEAK && IS_SET( victim->special, ALY_SNEAK ) )
						|| ( aff == AFF_INFRARED && IS_SET( victim->special, ALY_INF ) ) ) )
					continue;

				if ( IS_IMMORTAL( ch ) || UMAX( 2, saves_spell( ch->pp, victim ) + ( WIS_DIFF(ch, victim) * 4 ) - 23 + get_skill_level(ch,sn)*2 )
					> (number_percent( ) * 100 / weakness_percent( victim, sn)) )
				{
						REMOVE_BIT( victim->affected_by, aff );
						act( "你除去了$N身上的法術效應.", ch, NULL, victim, TO_CHAR );
						act( "$n除去了$N身上的法術效應.", ch, NULL, victim, TO_NOTVICT );
					break;
				}
				else
				{
						send_to_char( "你失敗了！\n\r", ch );
					if ( ( !victim->fighting || !is_same_group( ch, victim->fighting ) ) && number_bits( 4 ) == 1 )
						multi_hit( victim, ch, TYPE_UNDEFINED );
					return;
				}
			}
		}
    }

	mprog_hurt_trigger(victim, ch, sn);

	// fly 判定在 fall_effect 中有不需要另外判定 2023/01/15
	fall_effect( victim );

    return;
}


//modified by tale 12/29/2010
void spell_flaming_armor_strike( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    double dam;
    int nSklv  = get_skill_level(ch,sn);

    dam = dice( 4 + nSklv, ch->level + nSklv );
    dam = dam * 0.5 * int_effect( ch ) * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
    if(get_eq_char( victim, WEAR_WIELD ) == NULL) dam = dam * 6 / 5;

    if( !IS_NPC(ch) )
    {
		switch( nSklv )
		{
			case 0:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 2 : 0.8) + ch->amdr * 0.7;
				break;
			case 1:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 1.8 : 1) + ch->amdr * 0.75;
				break;
			case 2:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 1.6 : 1.2) + ch->amdr * 0.8;
				break;
			case 3:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 1.4 : 1.4) + ch->amdr * 0.85;
				break;
			case 4:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 1.2 : 1.6) + ch->amdr * 0.9;
				break;
			case 5:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 1 : 1.8) + ch->amdr * 0.95;
				break;
			case 6:
				dam = dam - victim->mdr * (victim->mdr >= 0 ? 0.8 : 2) + ch->amdr * 1.1;
				break;
		}
		dam = number_range( dam * 0.9 , dam * 1.2 );
    }
    else if ( IS_NPC(ch) )
	{
		if (victim->mdr >= 0)
		{
			if (nSklv < 3)  dam = dam - victim->mdr * 2 + ch->amdr * 0.4;
			if (nSklv >= 3 && nSklv < 6 )  dam = dam - victim->mdr * 1.5 + ch->amdr * 0.6;
			if (nSklv > 5)  dam = dam - victim->mdr * 0.8 + ch->amdr * 0.8;
		}
		else if (victim->mdr < 0)
		{
			if (nSklv < 3)  dam = dam - victim->mdr * 0.8 + ch->amdr * 0.4;
			if (nSklv >= 3 && nSklv < 6)  dam = dam - victim->mdr * 1.5 + ch->amdr * 0.6;
			if (nSklv > 5)  dam = dam - victim->mdr * 2 + ch->amdr * 0.8;
		}
		dam = number_range( dam * 2.8 , dam * 3.2 );
	}

	if ( nSklv * 2 + new_saves_spell( ch->pp, victim, MAGIC_FIRE ) < (number_percent() * 100 / weakness_percent(victim, sn)) )
		dam /= 2;
	//ex_dam = ch->amdr * (6 + nSklv) / 6;

	dam = dam * weakness_damage(victim, sn) / 100;
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}



void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *ich;
    int sklv = get_skill_level(ch,sn);
	// obj level effect add at 2020/10/21
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
	}else{
		obj_lv = 0;
	}

	send_to_char( "你伸手一揮，招來了一陣紫色煙霧.\n\r", ch );
	act( "$n 伸手一揮，招來了一陣紫色煙霧.", ch, NULL, NULL, TO_ROOM );

	for ( ich = ch->in_room->people; ich; ich = ich->next_in_room )
	{
		if ( IS_IMMORTAL( ich ) && IS_SET( ich->act, PLR_WIZINVIS ) )
			continue;

		if ( ich == ch || new_saves_spell( ch->pp, ich,MAGIC_WIND ) + sklv*3 < (number_percent() * 103 / weakness_percent(ich, sn) ) )
			continue;

		affect_strip ( ich, gsn_invis			);
		affect_strip ( ich, gsn_mass_invis		);
		affect_strip ( ich, gsn_shadow			);
		REMOVE_BIT   ( ich->affected_by, AFF_HIDE	);
		REMOVE_BIT   ( ich->affected_by, AFF_INVISIBLE	);
		if(!IS_SET( ich->special, ALY_SNEAK)){
			affect_strip( ich, gsn_sneak );
			REMOVE_BIT( ich->affected_by, AFF_SNEAK );
		}
		if( sklv > 5 ){
			affect_strip( ich, gsn_nightstalk );
			REMOVE_BIT(ich->affected_by, AFF_NIGHTSTALK);
		}

		act( "$n 的身影從煙霧中浮現！", ich, NULL, NULL, TO_ROOM );
		send_to_char( "你被發現了！\n\r", ich );
    }

    return;
}


// modified by jye 6.10.2005
void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch,sn);
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	// add at 2022/01/13
	int fly_dc = 0;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}

	// fly 低 lv 給予 dc 加成 , lv 6 dc 加成增加 2022/01/13
	if( sklv > 0 ) fly_dc -= 6;
	if( sklv > 1 ) fly_dc -= 7;
	if( sklv > 2 ) fly_dc -= 8;
	if( sklv > 3 ) fly_dc -= 9;
	if( sklv > 4 ) fly_dc -= 8;
	if( sklv > 5 ) fly_dc -= 7;

	if( is_affected( victim, skill_lookup("fly"))) 
	{
		// 修改為判定 dc 值是否大於 fly_dc 加成 2022/01/13
		//if( affected_value(victim,sn,APPLY_DODGE) != 0 && sklv < 6)
		if( affected_value(victim,sn,APPLY_DODGE) < fly_dc )
			return; 
		else
		affect_strip( victim, skill_lookup("fly"));
	}

	if( !is_flying( victim ) )
	{
		af.type	 = sn;
		af.duration	 = ch->level + 3+sklv*3;
		af.location	 = APPLY_NONE;
		af.modifier	 = 0;
		af.bitvector = AFF_FLYING;
		affect_to_char( victim, &af );
	}

	//if( sklv > 5 )
	if( sklv > 0 )
	{
		af.type	 = sn;
		af.duration	 = ch->level + 3+sklv*3;
		af.location	 = APPLY_DODGE;
		af.modifier	 = fly_dc;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}

	send_to_char( "你的雙腳漸漸升離了地面.\n\r", victim );
	act( "$n 的雙腳漸漸升離了地面.", victim, NULL, NULL, TO_ROOM );
	WAIT_MAGIC_STATE(ch, 8);
	return;
}


void spell_giant_strength( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	int nPDBonus = 0;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}
	// 拉高 pd 加成 2022/01/13
	nPDBonus = UMAX( 5 , (get_curr_int(ch) - 5) / 2 + sklv + sklv * sklv * sklv / 4 );

    if ( is_affected( victim, skill_lookup("combat mind")))
        affect_strip( victim, skill_lookup("combat mind"));

    if ( is_affected( victim, skill_lookup("giant strength"))) {
        if( affected_value(victim,sn, APPLY_ADAMAGE) <= nPDBonus)
            affect_strip( victim, skill_lookup("giant strength"));
        else
            return; 
    }

    if ( is_affected( victim, skill_lookup("adrenaline control")))
        affect_strip( victim, skill_lookup("adrenaline control"));

    af.type   = sn;
    af.duration   = 10 + ch->level/2 + sklv * 4;
    af.location   = APPLY_STR;
    af.modifier   = 1 + sklv/3;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.type   = sn;
    af.duration   = 10 + ch->level/2 + sklv * 4;
    af.location   = APPLY_ADAMAGE;
    af.modifier   = nPDBonus;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ( ch != victim )
        send_to_char( "好的。\n\r", ch );
    send_to_char( "你感到自己更強壯了.\n\r", victim );
    return;
}


// modified by whisperwind 9.22.2004
void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if( IS_AFFECTED( victim, AFF_INFRARED) ){
		if ( is_affected( victim, skill_lookup("infravision")))
			affect_strip( victim, skill_lookup("infravision"));
		else
			return;
	}

	af.type	 = sn;
	af.duration	 = 2 * ch->level + get_skill_level(ch,sn)*6;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_INFRARED;
	affect_to_char( victim, &af );

	if ( !(IS_NPC(victim) && ( victim->pIndexData->progtypes & DEATH_PROG ) ) )
	{
		send_to_char( "你的雙眼閃爍著紅光.\n\r", victim );
		act( "$n 的雙眼閃爍著紅光.\n\r", ch, NULL, NULL, TO_ROOM );
	}
	WAIT_MAGIC_STATE(ch, 8);
	return;
}


// modified by whisperwind 9.18.2004
void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if ( IS_AFFECTED( victim, AFF_INVISIBLE ) )
	{
		if ( is_affected( victim, skill_lookup("invis")))
			affect_strip( victim, skill_lookup("invis"));
		else if ( is_affected( victim, skill_lookup("mass invis")))
			affect_strip( victim, skill_lookup("mass invis"));
		else
			return;
	}

	act( "$n 的身影逐漸消失.", victim, NULL, NULL, TO_ROOM );

	af.type	 = sn;
	af.duration	 = ch->level/2 + get_curr_wis( ch ) + get_skill_level(ch,sn) * 6;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( victim, &af );

	send_to_char( "你的身影逐漸消失.\n\r", victim );

	WAIT_MAGIC_STATE(ch, 8);
	return;
}



void spell_know_alignment( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	char *msg;
	int ap;

	ap = victim->alignment;

	// 修改 know alignment 範圍 2022/10/25

	/*if ( ap >  700 ) msg = "$N 周圍籠罩著潔白似雪的光芒.";
	else if ( ap >  350 ) msg = "$N 是你道德上的典範.";
	else if ( ap >  100 ) msg = "$N 看來仁慈而體貼.";
	else if ( ap > -100 ) msg = "$N 的品德有點小缺陷.";
	else if ( ap > -350 ) msg = "$N 連好朋友都會欺騙.";
	else if ( ap > -700 ) msg = "$N 看來想殺死你！";
	else msg = "還是別在 $N 面前亂說話好了...否則可能有危險."; */

	if ( ap >  899 ) msg = "$N 周圍籠罩著潔白似雪的光芒.";
	else if ( ap >  749 ) msg = "$N 是你道德上的典範.";
	else if ( ap >  499 ) msg = "$N 看起來仁慈善良.";
	else if ( ap >  349 ) msg = "$N 嚴守道德規模.";
	else if ( ap >   99 ) msg = "$N 對人的態度和善.";
	else if ( ap > -100 ) msg = "$N 不受是非善惡的約束.";
	else if ( ap > -350 ) msg = "$N 的品德有些缺陷.";
	else if ( ap > -500 ) msg = "$N 連朋友都能出賣.";
	else if ( ap > -750 ) msg = "$N 眼中充斥暴厲之氣.";
	else if ( ap > -900 ) msg = "$N 看來想殺死你！";
	else msg = "還是別在 $N 面前亂說話好了...否則可能有危險.";

	act( msg, ch, NULL, victim, TO_CHAR );
	return;
}


void spell_mass_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *gch;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) )
			continue;
			
		if (IS_AFFECTED( gch, AFF_INVISIBLE ) ) {
			if ( is_affected( gch, skill_lookup("invis")))
					affect_strip( gch, skill_lookup("invis"));
			else if ( is_affected( gch, skill_lookup("mass invis")))
					affect_strip( gch, skill_lookup("mass invis"));
				else
					continue;
		}

		send_to_char( "你的身影逐漸消失.\n\r", gch );
		act( "$n 的身影逐漸消失.", gch, NULL, NULL, TO_ROOM );

		af.type	     = sn;
		af.duration  = ch->level/10 + get_curr_wis( ch ) + get_skill_level(ch,sn)*4;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_INVISIBLE;
		affect_to_char( gch, &af );
	}
	send_to_char( "好的。\n\r", ch );

	WAIT_MAGIC_STATE(ch, 24);
	return;
}

void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn); // add at 2022/01/13
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}

	if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
		return;

	af.type	 = sn;
	af.duration	 = number_fuzzy( ch->level / 4 ) + sklv*2;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_PASS_DOOR;
	affect_to_char( victim, &af );

	send_to_char( "你的身體漸漸變成半透明.\n\r", victim );
	act( "$n 的身體漸漸變成半透明.", victim, NULL, NULL, TO_ROOM );
	return;
}

// modified by whisperwind 9.18.2004
void spell_protection( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	if (IS_AFFECTED( victim, AFF_PROTECT )){ 
	// || IS_SET(race_table[victim->race].race_abilities, RACE_PROTECTION)){
	   if ( is_affected( victim, skill_lookup("protection")))
		   affect_strip( victim, skill_lookup("protection"));
	   else
		   return;
	}

	af.type	 = sn;
	af.duration	 = ch->level/2 + get_skill_level(ch,sn)*2;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_PROTECT;
	affect_to_char( victim, &af );

	if ( ch != victim )
		send_to_char( "好的。\n\r", ch );
	send_to_char( "你的四周升起了神聖的保護力場.\n\r", victim );
	return;
}


void spell_remove_silence( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

    /*
     *	Note: because this spell is declared as TAR_CHAR_DEFENSIVE,
     *	do_cast will set vo = ch with no argument.  Since it is
     *	impossible for a silenced character to cast anyway, if
     *	victim == ch, that means there was no argument supplied
     *	(or the spellcaster tried to specify themself), i.e., trying
     *	to remove a "Cone of Silence" from ch->in_room.
     *	This might seem pointless, but it will work within obj_cast_spell.
     *	Thelonius (Monk) 5/94
     */
	if ( victim == ch )
	{
		ROOM_INDEX_DATA *pRoomIndex;
		bool DidSomething;

		pRoomIndex   = ch->in_room;
		DidSomething = FALSE;

		if ( IS_SET( pRoomIndex->room_flags, ROOM_CONE_OF_SILENCE ) )
		{
			REMOVE_BIT( pRoomIndex->room_flags, ROOM_CONE_OF_SILENCE );
			send_to_char( "你消除了這裡的沈默力場！\n\r", ch );
			act( "$n 消除了這裡的沈默力場！", ch, NULL, NULL, TO_ROOM );
			DidSomething = TRUE;
		}

		if ( IS_AFFECTED( victim, AFF_MUTE ) )
		{
			affect_strip( victim, gsn_mute );
			send_to_char( "你消除了身上的禁言術.\n\r", ch );
			act( "$n 消除了自己身上的禁言術.", ch, NULL, victim, TO_VICT );
			DidSomething = TRUE;
		}

		if ( !DidSomething )
			send_to_char( "什麼也沒發生...\n\r", ch );

		return;
	}

	if ( IS_AFFECTED( victim, AFF_MUTE ) )
    {
		affect_strip( victim, gsn_mute );
		act( "你消除了 $N 身上的禁言術.", ch, NULL, victim, TO_CHAR );
		act( "$n 消除了你身上的禁言術.", ch, NULL, victim, TO_VICT );
		act( "$n 消除了 $N 身上的禁言術.", ch, NULL, victim, TO_NOTVICT );
	}
	else
		act( "$N 看來好的很.",	ch, NULL, victim, TO_CHAR    );

	return;
}


// modified by whisperwind 9.18.2004
// 重新定義 sanctuary 效果 2022/10/26
void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
	int cast_total = 0; // add at 2022/10/26
	int vch_total = 0; // add at 2022/10/26
	int sklv = get_skill_level(ch,sn); // add at 2022/10/26
	int div_sklv = get_skill_level(ch,gsn_divine_light); // add at 2022/10/26
	// level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	int nSvSaint = 0; // add at 2022/11/04
	int nSvDark = 0; // add at 2022/11/04
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

	/*  2022/10/26
		聖光術修改為給予聖,暗抗性值, 並且被施法者會得到 aff_sanctuary 的 affect
		擁有 aff_sanctuary 可以依抗性值不同的聖暗法術負面效果作抵抗
		加成的總數值為 cast_total , 此值會依 ch 的 alignment 平均分配給附加的聖,暗抗性值
		而分配的聖,暗抗性值會與 ch 的陣營相反
		例 : ch 為 alignment 較趨近 1000 , 則 sv dark 會給比較多, 反之趨近 -1000 則 sv saint 會給較多

		聖光術的法術疊蓋以總數值作判定, 如果 cast_total 大於 victim 身上的值, 才會作疊蓋法術的行為

		此外, sanctuary 的強度會受 divine light sklv 影響 , divine light 亦同會受 sanctuary 影響
	*/

	cast_total = 1 + (sklv + div_sklv) * 2 + (sklv * sklv)/2 + ch->level/10;

	/*if ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT))
	{
		if ( is_affected( victim, skill_lookup("sanctuary")))
			affect_strip( victim, skill_lookup("sanctuary"));
		else if ( is_affected( victim, skill_lookup("divine light")))
			affect_strip( victim, skill_lookup("divine light"));
		else
			return;
	} */

	if ( is_affected( victim, gsn_sanctuary) ){
		vch_total = 0 - (affected_value(victim,gsn_sanctuary,APPLY_SAVING_SAINT) + affected_value(victim,gsn_sanctuary,APPLY_SAVING_DARK) );
		if ( cast_total < vch_total ){
			send_to_char( "他身上的聖光術比你施放的還強呢!\n\r", ch );
			return;
		}else{
			affect_strip( victim, gsn_sanctuary );
		}
	}

	nSvSaint = 0 - (cast_total * ( 1000 - ch->alignment )/ 2000 );
	nSvDark = 0 - (cast_total * ( 1000 + ch->alignment )/ 2000 );

	// 修改為判定加成是否為 0 決定是否加入 affect 2022/11/04
	//if(ch->alignment < 1000){
	if( nSvSaint != 0 ){
		af.type	     = sn;
		//af.duration	 = number_fuzzy( ( nLevel + get_curr_wis( ch ) - 10 ) / 9 )+get_skill_level(ch,sn) + get_skill_level(ch,sn)/2;
		af.duration  = 1 + sklv + ch->level/10;
		af.location	 = APPLY_SAVING_SAINT;
		af.modifier	 = nSvSaint;
		// 天生 sanctuary 的情況則不增加 bitvector 2022/10/26
		if( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
			af.bitvector = 0;
		else
			af.bitvector = AFF_SANCTUARY;
		affect_to_char( victim, &af );
	}
	//if(ch->alignment > -1000){
	if( nSvDark != 0 ){
		af.type	     = sn;
		af.duration  = 1 + sklv + ch->level/10;
		af.location	 = APPLY_SAVING_DARK;
		af.modifier	 = nSvDark;
		if( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
			af.bitvector = 0;
		else
			af.bitvector = AFF_SANCTUARY;
		affect_to_char( victim, &af );
	}

	send_to_char( "一道白色聖光籠罩了你全身.\n\r", victim );
	act( "一道白色聖光籠罩了 $n.", victim, NULL, NULL, TO_ROOM );
	return;
}

// modified by whisperwind 9.18.2004
// 重新定義 divine light 效果 2022/10/26
void spell_divine_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
	AFFECT_DATA af;
	CHAR_DATA *gch;
	//int duration;
	int cast_total = 0; // add at 2022/10/26
	int gch_total = 0; // add at 2022/10/26
	int sklv = get_skill_level(ch,sn); // add at 2022/10/26
	int san_sklv = get_skill_level(ch, gsn_sanctuary); // add at 2022/10/26
	int nSvSaint = 0; // add at 2022/11/04
	int nSvDark = 0; // add at 2022/11/04


	cast_total = 1 + (sklv + san_sklv) * 2 + (sklv * sklv)/2 + level/10;

	nSvSaint = 0 - (cast_total * ( 1000 - ch->alignment )/ 2000 );
	nSvDark = 0 - (cast_total * ( 1000 + ch->alignment )/ 2000 );

	act( ""HIY"天上緩緩降下皎潔的聖光，護衛"NOR"$n"HIY"和$s隊友！"NOR"", ch, NULL, NULL, TO_ROOM );
	act( ""HIY"天上緩緩降下皎潔的聖光，護衛"NOR"你"HIY"和你的隊友！"NOR"", ch, NULL, NULL, TO_CHAR );

	//level += get_skill_level(ch,sn);
	//duration = number_fuzzy( ( level + get_curr_wis( ch ) - 10 ) / 9 ) + get_skill_level(ch,sn) + get_skill_level(ch,sn)/3;

	for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
	{
		if ( !is_same_group( gch, ch ) )
			continue;

		/*if ( IS_AFFECTED( gch, AFF_SANCTUARY ) )
		{
			if ( is_affected( gch, skill_lookup("sanctuary")))
				affect_strip( gch, skill_lookup("sanctuary"));
			else if ( is_affected( gch, skill_lookup("divine light")))
				affect_strip( gch, skill_lookup("divine light"));
			else
				continue;
		} */

		if ( is_affected( gch, gsn_sanctuary) ){
			gch_total = 0 - (affected_value(gch,gsn_sanctuary,APPLY_SAVING_SAINT) + affected_value(gch,gsn_sanctuary,APPLY_SAVING_DARK) );
			if ( cast_total < gch_total ){
				continue;
			}else{
				affect_strip( gch, gsn_sanctuary );
			}
		}
		// 修改為判定加成是否為 0 決定是否加入 affect 2022/11/04
		//if(ch->alignment < 1000){
		if( nSvSaint != 0 ){
			//af.type	 = sn;  修改 buffer name 為 sanctuary 2022/10/26
			af.type      = gsn_sanctuary;
			af.duration	 = 1 + sklv + san_sklv + level/10;
			//af.location	 = APPLY_NONE; 
			af.location	 = APPLY_SAVING_SAINT;
			af.modifier	 = nSvSaint;
			if( IS_AFFECTED( gch, AFF_SANCTUARY ) || IS_SET(race_table[gch->race].race_abilities, RACE_SANCT) )
				af.bitvector = 0;
			else
				af.bitvector = AFF_SANCTUARY;
			affect_to_char( gch, &af );
		}
		//if(ch->alignment > -1000){
		if( nSvDark != 0 ){
			af.type      = gsn_sanctuary;
			af.duration	 = 1 + sklv + san_sklv + level/10;
			af.location	 = APPLY_SAVING_DARK;
			af.modifier	 = nSvDark;
			if( IS_AFFECTED( gch, AFF_SANCTUARY ) || IS_SET(race_table[gch->race].race_abilities, RACE_SANCT) )
				af.bitvector = 0;
			else
				af.bitvector = AFF_SANCTUARY;
			affect_to_char( gch, &af );
		}
	}

	//send_to_char( "好的.\n\r", ch );

	return;
}

// modified by jye 6.11.2005
void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	int intArmor, intShield ,intStone;
	int nLevel   = ch->level;
	// obj level effect add at 2020/10/21
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}
	if(nLevel > 42) nLevel = 42;
	int nAcBonus = sklv * nLevel / 2;
  	// 統一 armor , stone , shield 的pdr mdr 加成值 2022/01/13
	int nSkBonus = 0;
	if( sklv > 0 ) nSkBonus += 4;
	if( sklv > 1 ) nSkBonus += 8;
	if( sklv > 2 ) nSkBonus += 8;
	if( sklv > 3 ) nSkBonus += 8;
	if( sklv > 4 ) nSkBonus += 6;
	if( sklv > 5 ) nSkBonus += 6;
  
	intArmor = skill_lookup("armor");
	intShield = skill_lookup("shield");
	intStone = skill_lookup("stone skin");

	if(sklv >= 4){
		nAcBonus += nLevel * (sklv - 3) * (sklv - 3) / 2;
	}
	if(nAcBonus < 200) nAcBonus = (nAcBonus + 200) / 2;

	if( is_affected( victim, intShield)) 
	{
		if( affected_value(victim,intArmor,APPLY_RMAGIC) == 0)
			if( affected_value(victim,sn,APPLY_AC) > -1 - nAcBonus)
				affect_strip( victim, intShield);
			else
				return; 
		else 
		{
			if( sklv > 3)
				affect_strip( victim, intShield);
			else
				return; 
		}
	}
	else if( is_affected( victim, intArmor))
	{
		if( affected_value(victim,intArmor,APPLY_SAVING_SPELL) != 0 && sklv < 4)
			return; 
		else
			affect_strip( victim, intArmor);
	}
	else if( is_affected( victim, intStone))
	{
		if( affected_value(victim,intStone,APPLY_RDAMAGE) != 0 && sklv < 4)
			return; 
		else
			affect_strip( victim, intStone);
	}

	af.type      = sn;
	af.duration  = 10 + nLevel/2 + sklv * 4;
	af.location  = APPLY_AC;
	af.modifier  = 0 - nAcBonus;
	af.bitvector = 0;
	affect_to_char( victim, &af );
  
	//if( get_skill_level(ch,sn) > 3 ) 
	if( sklv > 0 )
	{
		af.type      = sn;
		af.duration  = 10 + nLevel/2 + sklv * 4;
		af.location  = APPLY_RMAGIC;
		//af.modifier  = sklv * sklv + 4;
		af.modifier  = nSkBonus;
		if(!IS_NPC(ch) && ch == vo && (ch->pcdata->prime_attr == APPLY_INT || ch->pcdata->prime_attr == APPLY_WIS) )
			af.modifier += af.modifier / 2;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}

	send_to_char( "一股力場環繞在你身旁.\n\r", victim );
	act( "一股力場環繞在 $n 身旁.", victim, NULL, NULL, TO_ROOM );
	return;
}

// modified by jye 6.11.2005
void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch,sn);
	int intArmor, intShield ,intStone;
	int nLevel = ch->level;
	// obj level effect add at 2020/10/21
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}
	if(nLevel > 42) nLevel = 42;
	int nAcBonus = sklv * nLevel / 2;
  	// 統一 armor , stone , shield 的pdr mdr 加成值 2022/01/13
	int nSkBonus = 0;
	if( sklv > 0 ) nSkBonus += 4;
	if( sklv > 1 ) nSkBonus += 8;
	if( sklv > 2 ) nSkBonus += 8;
	if( sklv > 3 ) nSkBonus += 8;
	if( sklv > 4 ) nSkBonus += 6;
	if( sklv > 5 ) nSkBonus += 6;

	intArmor = skill_lookup("armor");
	intShield = skill_lookup("shield");
	intStone = skill_lookup("stone skin");

	if(sklv >= 4){
		nAcBonus += nLevel * (sklv - 3) * (sklv - 3) / 2;
	}
	if(nAcBonus < 200) nAcBonus = (nAcBonus + 200) / 2;
  
	if( is_affected( victim, intStone)) 
	{
		if( affected_value(victim,intArmor,APPLY_RDAMAGE) == 0)
			if( affected_value(victim,sn,APPLY_AC) > -1 - nAcBonus)
				affect_strip( victim, intStone);
			else
				return; 
		else 
		{
			if( sklv > 3)
				affect_strip( victim, intStone);
			else
				return; 
		}
	}
	else if( is_affected( victim, intArmor))
	{
		if( affected_value(victim,intArmor,APPLY_SAVING_SPELL) != 0 && sklv < 4)
			return; 
		else
			affect_strip( victim, intArmor);
	}
	else if( is_affected( victim, intShield))
	{
		if( affected_value(victim,intShield,APPLY_RMAGIC) != 0 && sklv < 4)
			return; 
		else
			affect_strip( victim, intShield);
	}

	af.type      = sn;
	af.duration  = 10 + nLevel/2 + sklv * 4;
	af.location  = APPLY_AC;
	af.modifier  = 0 - nAcBonus;
	af.bitvector = 0;
	affect_to_char( victim, &af );
	//if( get_skill_level(ch,sn) > 3 ) 
	if( sklv > 0 )
	{
		af.type     = sn;
		af.duration = 10 + nLevel/2 + sklv * 4;
		af.location = APPLY_RDAMAGE;
		//af.modifier = sklv * sklv - 1;
		af.modifier = nSkBonus;
		// 追加 cle base class 對自己放也有兩倍 2022/01/13
		if(!IS_NPC(ch) && ch == vo && (ch->pcdata->prime_attr == APPLY_INT || ch->pcdata->prime_attr == APPLY_WIS) )
			af.modifier += af.modifier / 2;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}

  
	send_to_char( "你的皮膚逐漸硬化.\n\r", victim );
	act( "$n 的皮膚逐漸硬化.", victim, NULL, NULL, TO_ROOM );
	return;
}
// modified by whisperwind 9.17.2004



/*
 * This is for muds that want scrolls of recall.
 */
void spell_word_of_recall( int sn, int level, CHAR_DATA *ch, void *vo )
{
	do_recall( (CHAR_DATA *) vo, "" );
	return;
}


/*
 * Code for Psionicist spells/skills by Thelonius
 */
void spell_adrenaline_control ( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	int nDrBonus = 0;
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}

	nDrBonus = get_curr_con(ch) / 5 + sklv + (sklv * sklv * sklv) / 50;

	if ( is_affected( victim, skill_lookup("combat mind")))
		affect_strip( victim, skill_lookup("combat mind"));
	if ( is_affected( victim, skill_lookup("adrenaline control")))
	{
		if( affected_value(victim,sn,APPLY_DAMROLL) <= nDrBonus )
			affect_strip( victim, skill_lookup("adrenaline control"));
		else
			return;
	}
	if ( is_affected( victim, skill_lookup("giant strength")))
		affect_strip( victim, skill_lookup("giant strength"));

	af.type	 = gsn_adrenaline;
	af.duration	 = 10 + ch->level/2 + sklv * 4;
	af.location	 = APPLY_DAMROLL;
	af.modifier	 = nDrBonus;
	af.bitvector = 0;
	affect_to_char( victim, &af ); 
	
	af.location	= APPLY_CON;
	af.modifier	 = 1 + sklv/3;
	af.bitvector = 0;	
	affect_to_char( victim, &af );

		/* if( get_skill_level(ch,sn) < 5 ){
			af.type	 = skill_lookup( "poison" );
			af.location	= APPLY_NONE;
			af.bitvector = AFF_POISON;
			affect_join( victim, &af );
			}*/
    
	send_to_char( "你感到一股神力在血管流竄！\n\r", victim );
	act( "$N讓$n面紅耳赤!", victim, NULL, ch, TO_ROOM );

		/* if ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) )
				multi_hit( victim, ch, TYPE_UNDEFINED );*/
	return;
}

void spell_combat_mind ( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch, sn);
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}

	if ( is_affected( victim, skill_lookup("giant strength")))
		affect_strip( victim, skill_lookup("giant strength"));
	if ( is_affected( victim, skill_lookup("combat mind")))
	{
		if( affected_value(victim,sn,APPLY_HITROLL) < 1 + get_curr_wis(ch)/6 + sklv + sklv/5 + sklv/6)
			affect_strip( victim, skill_lookup("combat mind"));
		else
			return;
	}
	if ( is_affected( victim, skill_lookup("adrenaline control")))
		affect_strip( victim, skill_lookup("adrenaline control"));

	af.type	 = sn;
	af.duration	 = 10 + ch->level/2 + sklv * 4;
	af.location	 = APPLY_HITROLL;
	af.modifier	 = get_curr_wis(ch)/6 + sklv + sklv/5 + sklv/6;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	af.location	 = APPLY_DEX;
	af.modifier	 = 1 + sklv/3;
	affect_to_char( victim, &af );

	if ( victim != ch )
		send_to_char( "好的.\n\r", ch );
	send_to_char( "你的戰鬥直覺逐漸變得敏銳.\n\r", victim );
	return;
}


void spell_powerup( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA*) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch,sn); // 2022/02/18

	if( is_affected(ch, sn) )
		return;

	if( !IS_NPC(ch) ){
		if( ch->move < 100 ) {
			send_to_char("你的體力不足以進行發勁。\n\r", ch);
			return;
		}
		ch->move -= 100;
	}


	af.type      = sn;
	af.duration  = level / 4 + sklv;
	af.location  = APPLY_DAMROLL;
	// 強化 powerup 的 sklv 影響 2022/02/18
	//af.modifier  = ( get_curr_str(ch) + get_curr_wis(ch) )/ 5 + get_skill_level(ch,sn);
	af.modifier  = ( get_curr_str(ch) + get_curr_wis(ch) )/ 5 + sklv + (sklv * sklv)/ 3;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	send_to_char("你體內的勁力源源不絕湧出。\n\r", victim);
	if( ch != victim )
		send_to_char("好的。\n\r", ch);
	WAIT_MAGIC_STATE(ch, 12);
}

void spell_hasten( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA*) vo;
    AFFECT_DATA af;

    if( is_affected(ch, sn) )
        return;

    if( !IS_NPC(ch) ){
        if( ch->move < 100 ) {
            send_to_char("你的體力不足以施展行氣活身法。\n\r", ch);
            return;
        }
        ch->move -= 100;
    }


    af.type      = sn;
    af.duration  = level / 6+get_skill_level(ch,sn)+get_skill_level(ch,sn)/2;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char("你覺得自己的行動變得敏捷許\多。\n\r", victim);
    if( ch != victim )
        send_to_char("好的。\n\r", ch);
    WAIT_MAGIC_STATE(ch, 12);
}

void spell_barrier ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
	int sklv = get_skill_level(ch,sn);
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		sklv = 0;
		if( obj_lv >= 10) sklv = 1;
		if( obj_lv >= 20) sklv = 2;
		if( obj_lv >= 30) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 50) sklv = 5;
		if( obj_lv >= 60) sklv = 6;
	}else{
		obj_lv = 0;
	}

    if ( is_affected( victim, sn ))
    	affect_strip( victim, sn );

    af.type	 = sn;
    af.duration	 = 1 + ch->level/10 + sklv * (sklv + 4) / 2;
    af.location	 = APPLY_SAVING_ELEMENT;
    af.modifier	 = -1 - (get_curr_wis(ch) - 10)/3 - sklv - sklv/2 - sklv/3 - sklv/5;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location	 = APPLY_SAVING_SPELL;
    af.modifier	 = -1 - (get_curr_wis(ch) - 10)/3 - sklv - sklv/2 - sklv/3 - sklv/5;
    affect_to_char( victim, &af );
    if(sklv > 4){
        af.location   = APPLY_RMAGIC;
        af.modifier   = (sklv * 3 - 13) * 5;
        affect_to_char( victim, &af );
    }

    send_to_char( "你在身旁建立起一道力場.\n\r", victim );
    return;
}

/* New style and function of lore, designed by keelar
 * 
 * 你用魔法洞悉他的心靈，得到想要的資料。
 *
 * ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
 * 巨鎧鼴(Peltephius) 的資料：                                // sklv 2 for HP
 * 等級：44  性別：男  陣營： 0
 * 力量：22  智力：18  智慧：20  敏捷：22  體質：22
 * 生命力：19541/19541   魔法力：1002/1002  移動力：9770/9770 
 *
 * 戰鬥能力資料                                               // sklv 1 ~ 5
 * 主要武器命中：   47   主要武器傷害： 126
 *     魔法潛力：    0   物理傷害：  70  魔法傷害： 150
 *
 * 防禦基本資料                                               // sklv 4 ~ 5
 *     護甲防衛：-4136   物理抗性： 300  物理障壁：   0
 *     閃避能力：   20   魔法抗性： 100  魔法障壁：   0
 *
 * 元素魔法抗性：                                             // sklv 6
 *     抗物理： -29   抗炎燄： -38   抗翔風：  31
 *     抗雷閃： -43   抗冰霜： -39   抗大地： -39
 *
 * 特殊魔法抗性：                                             // sklv 4
 *     抗龍息： -71   抗神聖： -10   抗黑闇： -10
 *
 * 對特殊技能的抗性專長及弱點：                               // sklv 6 and lv41
 * [              circle] 技能命中加成 -25%，傷害加成 -99%。  // while mob with low HP
 * [            backstab] 技能命中加成   0%，傷害加成 -99%。
 * [     general purpose] 技能命中加成   0%，傷害加成 -99%。
 * [            bashhead] 技能命中加成 -99%，傷害加成 -99%。
 * [           soulsteal] 技能命中加成 -99%，傷害加成 -99%。
 * [                feed] 技能命中加成 -99%，傷害加成 -99%。
 * [         faerie fire] 技能命中加成 100%，傷害加成 100%。
 * [               tardy] 技能命中加成 100%，傷害加成 100%。
 * [              weaken] 技能命中加成 100%，傷害加成 100%。
 * [               curse] 技能命中加成 100%，傷害加成 100%。
 *
 */
void spell_lore(int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA    *victim = (CHAR_DATA *) vo;
  int           nSklv  = get_skill_level(ch, sn), nWeakCnt;
	int					  nWeakStat = 100;
	MOB_WEAKNESS *pWeak  = NULL;

	char          rgLore[MAX_STRING_LENGTH];
	char          rgBuf[MAX_STRING_LENGTH];
	char          rgSec1[200], rgSec2[200], rgSec3[200];
	char          rgSec4[200], rgSec5[200], rgSec6[500];


	if(!IS_NPC(victim)){
		send_to_char( "你無法用這項法術得知其他冒險者的資料\n\r", ch );
		return;
	}else if(IS_SET(victim->special, NO_LORE) && !IS_IMMORTAL(ch)){
		send_to_char( "他的意志力實在太強了！你完全無法看透他的心靈！\n\r", ch );
		return;
	}

  if(IS_IMMORTAL(ch)) nSklv = 99;
	
	rgLore[0] = 0;
  sprintf(rgBuf, "你用魔法洞悉他的心靈，得到想要的資料。\n\n\r"); 
	strcat(rgLore, rgBuf);

  // Mob basic Info
	sprintf(rgBuf, "☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n\r" );
	strcat(rgLore, rgBuf);
	sprintf(rgBuf, "%s - %s(%s):\n\r等級：%2d  性別：%s  陣營： %d\n\r", 
	               victim->short_descr, 
	               race_table[ victim->race ].cname, race_table[ victim->race ].name,
	               victim->level, 
	               victim->sex == SEX_MALE ? "男" :
	               victim->sex == SEX_FEMALE ? "女" : "中",
								 victim->alignment);
	strcat(rgLore, rgBuf);

	// Five Basic Attributes
	sprintf(rgBuf, "力量：%2d  智力：%2d  智慧：%2d  敏捷：%2d  體質：%2d\n\r",
	               get_curr_str( victim ), get_curr_int( victim ),
								 get_curr_wis( victim ), get_curr_dex( victim ),
	               get_curr_con( victim ));
	strcat(rgLore, rgBuf);

	// HP/MP/MV Info
	if(nSklv >= 3){
		sprintf(rgBuf, "生命力：%4d/%4d   魔法力：%4d/%4d  移動力：%4d/%4d\n\r",
		               victim->hit,  victim->max_hit,
		               victim->mana, victim->max_mana,
		               victim->move, victim->max_move);
	}else{
		if(victim->max_hit > 9999){
			if(victim->hit > 9999) {
				sprintf(rgSec3, "????/????");
            }
			else {
				sprintf(rgSec3, "%4d/????", victim->hit);
            }
		}else{
			sprintf(rgSec3, "%4d/%4d", victim->hit, victim->max_hit);
		}
		sprintf(rgBuf, "生命力：%s   魔法力：%4d/%4d  移動力：%4d/%4d\n\r",
		               rgSec3, victim->mana, victim->max_mana,
									         victim->move, victim->max_move);
	}
	strcat(rgLore, rgBuf);

	// Fighting Related
	strcat(rgLore, "\n\r戰鬥能力資料：\n\r");
	strcpy(rgSec1, " 不詳");
	strcpy(rgSec3, "不詳");
	if(nSklv >= 1) sprintf(rgSec1, "%5d", get_hitroll(victim, WEAR_WIELD));
	if(nSklv >= 3) sprintf(rgSec3, "%4d", get_damroll(victim, WEAR_WIELD));
	sprintf(rgBuf, "主要武器命中：%s   主要武器傷害：%s\n\r", 
	               rgSec1, rgSec3);
	strcat(rgLore, rgBuf);

	if(get_eq_char( victim, WEAR_WIELD_2 )){
		strcpy(rgSec2, " 不詳");
		strcpy(rgSec4, "不詳");
		if(nSklv >= 2) sprintf(rgSec2, "%5d", get_hitroll(victim, WEAR_WIELD_2));
		if(nSklv >= 4) sprintf(rgSec4, "%4d", get_damroll(victim, WEAR_WIELD_2));
 		sprintf(rgBuf, "次要武器命中：%s   次要武器傷害：%s\n\r",
		             rgSec2, rgSec4);
		strcat(rgLore, rgBuf);
	}

	strcpy(rgSec4, " 不詳"); if(nSklv >= 4) sprintf(rgSec4, "%5d", victim->pp);
	strcpy(rgSec5, "不詳");  if(nSklv >= 5) sprintf(rgSec5, "%4d", victim->apdr);
	strcpy(rgSec6, "不詳");  if(nSklv >= 5) sprintf(rgSec6, "%4d", victim->amdr);
	sprintf(rgBuf, "    魔法潛力：%s   物理傷害：%s  魔法傷害：%s\n\r", 
	               rgSec4, rgSec5, rgSec6);
	strcat(rgLore, rgBuf);

	// Physical Resistance 
	strcat(rgLore, "\n\r防禦基本資料：\n\r");
	strcpy(rgSec4, "不詳"); if(nSklv >= 4) sprintf(rgSec4, "%4d", victim->pdr);
	strcpy(rgSec5, "不詳"); if(nSklv >= 5) sprintf(rgSec5, "%4d", victim->pad);
	
	sprintf(rgBuf, "    護甲防衛：%5d   物理抗性：%s  物理障壁：%s\n\r",
			            GET_AC( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Dodge and Magical Resistance 
	strcpy(rgSec4, "不詳"); if(nSklv >= 4) sprintf(rgSec4, "%4d", victim->mdr);
	strcpy(rgSec5, "不詳"); if(nSklv >= 5) sprintf(rgSec5, "%4d", victim->mad);
	
	sprintf(rgBuf, "    閃避能力：%5d   魔法抗性：%s  魔法障壁：%s\n\r",
	                GET_DODGE( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Magical Resistance in Elemental
	strcat(rgLore, "\n\r元素魔法抗性：\n\r");
	if(nSklv >= 6){
		sprintf(rgSec6, "    抗魔法：%4d   抗炎燄：%4d   抗翔風：%4d\n\r",
		                GET_SVS(victim),
		                NEW_GET_SVS(victim, MAGIC_FIRE),
		                NEW_GET_SVS(victim, MAGIC_WIND));
	}else{
		sprintf(rgSec6, "    抗魔法：%4d   抗炎燄： ???   抗翔風： ???\n\r", 
				            GET_SVS(victim));
	}
	strcat(rgLore, rgSec6);

	if(nSklv >= 6){
		sprintf(rgSec6, "    抗雷閃：%4d   抗冰霜：%4d   抗大地：%4d\n\r", 
		                NEW_GET_SVS(victim, MAGIC_LIGHTNING),
		                NEW_GET_SVS(victim, MAGIC_COLD),
		                NEW_GET_SVS(victim, MAGIC_EARTH));
	}else{
		sprintf(rgSec6, "    抗雷閃： ???   抗冰霜： ???   抗大地： ???\n\r");
	}
	strcat(rgLore, rgSec6);

	strcat(rgLore, "\n\r特殊魔法抗性：\n\r");
	if(nSklv >= 4) {
		sprintf(rgSec4, "    抗龍息：%4d   抗神聖：%4d   抗闇黑：%4d\n\r",
						NEW_GET_SVS(victim, MAGIC_BREATH), 
						NEW_GET_SVS(victim, MAGIC_SAINT), 
						NEW_GET_SVS(victim, MAGIC_DARK));
	}else{
		sprintf(rgSec4, "    抗龍息： ???   抗神聖： ???   抗黑闇： ???\n\r");
	}
	strcat(rgLore, rgSec4);

	// 每 tick 受到中毒的傷害 2021/11/03
	if(nSklv >= 4) {
		sprintf(rgSec4, "    毒傷害：%4d\n\r",
						NEW_GET_SVS(victim,MAGIC_POISON));
	}else{
		sprintf(rgSec4, "    毒傷害： ???\n\r");
	}
	strcat(rgLore, rgSec4);

	// Weakness
  strcat(rgLore, "\n\r對特殊技能的抗性專長及弱點：\n\r");
	if(nSklv >= 6 && ch->level >= 41){
		if(victim->weakness){
			nWeakStat = 100;
			if(get_skill_exp(ch) > 1000000) nWeakStat += (get_skill_exp(ch) - 1000000) / 20000;
			if(nWeakStat > 120) nWeakStat = 120;
			nWeakStat += get_curr_wis(ch) -  victim->level * 2;
			if(nWeakStat < 10) nWeakStat = 10;
			if(victim->level <= get_curr_wis(ch) + 10 || victim->hit * 100 < victim->max_hit * nWeakStat){
				for(pWeak = victim->weakness, nWeakCnt = 0, rgBuf[0] = 0; 
				    pWeak != NULL && nWeakCnt < 20; 
				    pWeak = pWeak->next, nWeakCnt++){
					sprintf(rgSec6, "[%20s] 技能命中加成 %3d%%，傷害加成 %3d%%。\n\r",
					                pWeak->name, 
													pWeak->percent - 100, 
													pWeak->damage  - 100);
					strcat(rgBuf, rgSec6);
				}
				if(nWeakCnt >= 20 + nSklv*5) 
					strcat(rgBuf, "此敵人的抗性專長及弱點多到你無法完整洞悉！\n\r");
			}else{
				sprintf(rgBuf, "    以你的洞察力，你得等對方更虛弱時才能觀察出他的專長及弱點。\n\r");
			}
		}else{
			sprintf(rgBuf, "    無專長及特殊弱點\n\r");
		}
	}else{
		sprintf(rgBuf, "    你的能力及等級尚無法洞悉敵人的專長及弱點。\n\r");
	}
	strcat(rgLore, rgBuf);

	strcat(rgLore, "\n\r");
	send_to_char(rgLore, ch);
	// 增加 lore 的 hurt prog 2022/10/30
	mprog_hurt_trigger(victim, ch, gsn_lore);
}
/**/

/* Previous version of Lore: added by Amenda 12/10/00 *
void spell_lore( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	char buf[ MAX_STRING_LENGTH ];
	char buf1[ MAX_STRING_LENGTH ];

        level += get_skill_level(ch,sn);
	if ( !IS_NPC( victim ) )
	{
		send_to_char( "你無法用這項法術得知其他冒險者的資料\n\r", ch );
		return;	
	}
	if ( IS_SET( victim->special, NO_LORE ) && !IS_IMMORTAL( ch ))
	{
		send_to_char( "他的意志力實在太強了！你完全無法看透他的心靈！\n\r", ch );
		return;	
	}	

    	buf1[0] = '\0';	
	sprintf( buf, "你用魔法洞悉他的心靈，得到想要的資料。\n\r" );
	strcat( buf1, buf );
	sprintf( buf, "\n\r☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n\r" );
	strcat( buf1, buf );
	sprintf( buf, "Mob 名稱：%s. 等級：%d. 性別：%s", victim->short_descr, victim->level,
	victim->sex == SEX_MALE ? "男性\n\r" : 
	victim->sex == SEX_FEMALE ? "女性\n\r" : "中性\n\r" );
	strcat( buf1, buf );
    	sprintf( buf, "生命力：%d/%d, 魔法力：%d/%d, 移動力：%d/%d\n\r",
	victim->hit,  victim->max_hit,
	victim->mana, victim->max_mana,
	victim->move, victim->max_move );
	strcat( buf1, buf );	    
    	sprintf( buf,
	    "力量：%d  智商：%d  智慧：%d  敏捷：%d  體質：%d\n\r",
	    get_curr_str( victim ),
	    get_curr_int( victim ),
	    get_curr_wis( victim ),
	    get_curr_dex( victim ),
	    get_curr_con( victim ) );
    	strcat( buf1, buf );
    	sprintf( buf, "護甲防衛力：%d, 陣營：%d\n\r", GET_AC( victim ), victim->alignment );
    	strcat( buf1, buf );
    	sprintf( buf, "關鍵字：%s\n\r", victim->short_descr );
    	strcat( buf1, buf );
    	sprintf( buf, "基本描述：%s\n\r", victim->long_descr );
    	strcat( buf1, buf );
    	if ( level > L_APP - 2 )
    	{
		sprintf( buf, "閃避率：%d\n\r", GET_DODGE( victim ) );
    		strcat( buf1, buf );
    	}
    	if ( level > L_APP - 1 )
    	{
		sprintf( buf, "對魔法暨拋投性武器防禦力：%d\n\r", GET_SVS( victim ) );
    		strcat( buf1, buf );
    	}
    	if ( level > L_APP  )
    	{
		strcat ( buf1, "主要武器 " );
		sprintf( buf, "命中率: %d	傷害力: %d.\n\r",
		get_hitroll( victim, WEAR_WIELD ),
		get_damroll( victim, WEAR_WIELD ) );
    		strcat( buf1, buf );

    		if ( get_eq_char( victim, WEAR_WIELD_2 ) )
    		{
			sprintf( buf, "次要武器 命中率: %d  傷害力: %d.\n\r",
			get_hitroll( victim, WEAR_WIELD_2 ),
			get_damroll( victim, WEAR_WIELD_2 ) );
			strcat( buf1, buf );
		}
    	}
    	send_to_char( buf1, ch );
    	
    	mprog_hurt_trigger(victim, ch, sn);
    	
}
*/

/* Flaming armor spell added by Amenda 2/24/01 */
// modified by whisperwind 9.18.2004
void spell_flaming_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    int nSklv = get_skill_level(ch,sn);
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

    if ( IS_AFFECTED( ch, AFF_FLAMING ) ) {
        if ( is_affected( ch, skill_lookup("flaming armor")))
            affect_strip( ch, skill_lookup("flaming armor"));
        else
            return;
    }


    af.type	 = sn;
    af.duration	 = number_fuzzy( ( ch->level + get_curr_int( ch ) ) / 10 ) + nSklv * (nSklv + 4) / 3;
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_FLAMING;
    affect_to_char( ch, &af );

    send_to_char( ""HIP"你被一圈熊熊燃燒的火焰包圍！"NOR"\n\r", ch );
    act( ""HIP"$n 被一圈熊熊燃燒的紫色火焰包圍！"NOR"", ch, NULL, NULL, TO_ROOM );
    return;
}

/* Mana Shield spell added by Amenda 2/24/01 */
void spell_mana_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    int nSklv     = get_skill_level(ch,sn);
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}
    int nDuration = number_fuzzy( ( ch->level + get_curr_int( ch ) ) / 5 ) + nSklv * (nSklv + 4) / 3;

    if ( IS_AFFECTED( ch, AFF_MANA_SHIELD ) ){
       if ( is_affected( ch, skill_lookup("mana shield")))
           affect_strip( ch, skill_lookup("mana shield"));
       else
           return; }

    af.type	 = gsn_mana_shield;
    af.duration	 = nDuration;
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_MANA_SHIELD;
    affect_to_char( ch, &af );

    if(nSklv > 4){
        af.type	     = sn;
        af.duration	 = nDuration;
        af.location	 = APPLY_AC;
        af.modifier  = -50 * (nSklv - 4);
        af.bitvector = 0;
        affect_to_char( ch, &af );
        if( nSklv > 5 ) 
        {
            af.type	     = sn;
            af.duration	 = nDuration;
            af.location	 = APPLY_RDAMAGE;
            af.modifier	 = 15;
            af.bitvector = 0;
            affect_to_char( ch, &af );
        }
    }

    send_to_char( ""HIB"你的四周升起一道淡藍色的光暈！"NOR"\n\r", ch );
    act( ""HIB"$n 身邊慢慢升起一道淡藍色的光暈！"NOR"", ch, NULL, NULL, TO_ROOM );
    return;
}

void spell_sober_up( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int drunk;
	AFFECT_DATA af;

	if( IS_NPC( victim ) ){
		send_to_char("只能對玩家施展！\n\r",ch);
		return;
	}

	drunk = victim->pcdata->condition[COND_DRUNK];

	if( drunk <= 0 ){
		send_to_char((victim == ch ) ? "你一點也沒醉。\n\r" :"他看起來很清醒。\n\r", ch);
		return;
	}

	// mv 修改為最多變成一半 2023/01/13
	victim->move = (100 * victim->move)/( 100 + drunk );

	victim->pcdata->condition[COND_DRUNK] -= drunk;

	if( victim != ch )
		send_to_char( "好的.\n\r", ch ); // 加入換行 2023/01/13

	send_to_char("你體內的酒精逐漸散去，你的神智也逐漸清楚了。\n\r", victim);

	if( !is_affected( victim, gsn_weaken ) ) {
		af.type      = gsn_weaken;
		af.duration  = 2 + drunk/10;
		af.location  = APPLY_STR;
		af.modifier  = -2;
		af.bitvector = 0;
		affect_to_char( victim, &af );

		send_to_char( ""HIU"你突然覺得好虛弱！"NOR"\n\r", victim );
	}
	return;
}

// modified by whisperwind 9.18.2004
void spell_mystic_mirror( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    int nSklv = get_skill_level(ch,sn);
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

    if ( IS_AFFECTED( ch, AFF_MYSTIC_MIRROR) ) {
       if (is_affected( ch, skill_lookup("mystic mirror")))
           affect_strip( ch, skill_lookup("mystic mirror"));
       else
           return; 
    }
           
    af.type	 = gsn_mystic_mirror;
    af.duration	 = number_fuzzy( ( ch->level + get_curr_int( ch ) ) / 12 ) + nSklv * (nSklv + 4) / 3;
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_MYSTIC_MIRROR;
    affect_to_char( ch, &af );

    send_to_char( ""HIW"你的四周升起一道透明的魔力反射鏡！"NOR"\n\r", ch );
    act( ""HIW"$n 身邊慢慢升起一道透明的魔力反射鏡！"NOR"", ch, NULL, NULL, TO_ROOM );
    return;
}

void spell_mystic_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    int nSklv = get_skill_level(ch,sn);
	int MAnumber = 0; // 2020/10
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 10) nSklv = 1;
		if( obj_lv >= 20) nSklv = 2;
		if( obj_lv >= 30) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 50) nSklv = 5;
		if( obj_lv >= 60) nSklv = 6;
		nLevel = obj_lv;
	}else{
		obj_lv = 0;
	}

    if ( IS_AFFECTED( ch, AFF_MYSTIC_ARMOR) ) {
   	    if ( is_affected( ch, skill_lookup("mystic armor")))
            affect_strip( ch, skill_lookup("mystic armor"));
        else
            return;
    }
	if( !IS_NPC(ch) )
		MAnumber = 100 * ( 1 + nSklv ) + nSklv * nSklv * ch->max_mana / 50;
	else
		MAnumber = 10 * (10 + nSklv * nSklv) * URANGE(100 , ch->max_mana , 30000) / 100;

    af.type	 = gsn_mystic_armor;
    af.duration	 = 3 + ch->level / 10 + nSklv*2;
    af.location	 = APPLY_MAGIC_IMMUNE;
    af.modifier	 = MAnumber;
    af.bitvector = AFF_MYSTIC_ARMOR;
    affect_to_char( ch, &af );

    send_to_char( ""HIC"你的四周升起一道七彩的防壁！"NOR"\n\r", ch );
    act( ""HIC"$n 身邊慢慢升起一道七彩的防壁！"NOR"", ch, NULL, NULL, TO_ROOM );
    if( ch->fighting )
        WAIT_MAGIC_STATE(ch, 15);
    return;
}

// modified by whisperwind 9.20.2004
void spell_mental_empower( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int nSklv = get_skill_level(ch,sn);
	int isSelf = (victim == ch);
	// obj level effect add at 2020/10/21
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		// add at 2022/01/13
		nSklv = 0;
		if( obj_lv >= 10) nSklv = 1;
		if( obj_lv >= 20) nSklv = 2;
		if( obj_lv >= 30) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 50) nSklv = 5;
		if( obj_lv >= 60) nSklv = 6;
	}else{
		obj_lv = 0;
	}

	if ( victim->position == POS_FIGHTING || is_affected( victim, sn )) {
		if ( is_affected( victim, skill_lookup("mental empower")))
			affect_strip( victim, skill_lookup("mental empower"));
		else
			return;
	}

	if(!isSelf && nSklv < 6){
		send_to_char("你對心靈著掌控不足以對別人施展心靈增幅.\n\r", ch);
		return;
	}

	af.type	     = sn;
	af.duration	 = ch->level / 6 + nSklv + get_curr_int(ch) / 2;

	af.location	 = APPLY_INT;
	af.modifier	 = 1 + nSklv / 2;
	// 僅 sor 對自己施法有額外加成 2022/01/13
	//if(!isSelf) af.modifier /= 2;
	if( !isSelf || (ch->class != SORCERER) ) af.modifier /= 2;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	af.location	 = APPLY_CON;
	af.modifier	 = -2 + nSklv / 6;
	af.bitvector = 0;
	affect_to_char( victim, &af );
	if( nSklv > 3 ) {
		af.location	 = APPLY_POWER_POTENTIAL;
		af.modifier	 = (nSklv - 3) * 3;
		//if(!isSelf) af.modifier /= 3;
		if( !isSelf || (ch->class != SORCERER) ) af.modifier /= 3;
		af.modifier += (get_curr_int(ch) - 1) / 4 - 3;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}
	if( nSklv > 4 ) {
		af.location	 = APPLY_RMAGIC;
		af.modifier	 = (nSklv - 4) * 10;
		af.bitvector = 0;
		//if(isSelf) affect_to_char( victim, &af );
		if(isSelf && (ch->class == SORCERER) ) affect_to_char( victim, &af );
	}
	if( nSklv > 5 ) {
		af.location	 = APPLY_AMAGIC;
		af.modifier	 = 20;
		//if(!isSelf) af.modifier /= 4;
		if( !isSelf || (ch->class != SORCERER) ) af.modifier /= 4;
		af.modifier += get_curr_int(ch) - 15;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}

	send_to_char( "你的心智突然一片空明.\n\r", victim );
	if(ch != victim)
		send_to_char( "好的.\n\r", ch );
	return;
}

void spell_will_fortress( int sn, int level, CHAR_DATA *ch, void *vo )
{
	int nSklv = get_skill_level(ch, gsn_will_fortress);
	int nCurrInt = get_curr_int(ch);
	CHAR_DATA  *victim   = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	// obj level effect add at 2022/01/13
	int nLevel = ch->level;
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLevel = obj_lv;
		nSklv = 0;
		if( obj_lv >= 10) nSklv = 1;
		if( obj_lv >= 20) nSklv = 2;
		if( obj_lv >= 30) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 50) nSklv = 5;
		if( obj_lv >= 60) nSklv = 6;
	}else{
		obj_lv = 0;
	}
	af.type	     = gsn_will_fortress;
	// 加入 ch 等級影響作用時間 2022/01/13
	//af.duration	 = nSklv * 2 + UMAX(nSklv - 4, 0) * 4;
	af.duration	 = ch->level/20 + nSklv + UMAX(nSklv - 4, 0) * 2;
	af.location	 = APPLY_SAVING_SPELL;
	af.modifier	 = -1;
	af.bitvector =  0;
	for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room ) {
		if( !is_same_group(ch, victim)) continue;
		if( victim->deleted ) continue;

		// 能對戰鬥中的目標施放 will fortress , 但每個目標會扣除 mana 2022/01/13
		//if(victim->position != POS_FIGHTING) {
		if(victim->position == POS_FIGHTING){
			if( ch->mana < 100 )
				continue;
			ch->mana -= 100;
		}

		if(is_affected(victim, gsn_will_fortress)) {
			affect_strip(victim, gsn_will_fortress); 
		}
		af.location	 = APPLY_SAVING_SPELL;
		af.modifier	 = -nSklv - UMAX(nCurrInt / 5 - 3, 0);
		af.bitvector =  0;
		affect_to_char( victim, &af );

		af.location	 = APPLY_SAVING_ILLUSION;
		af.modifier	 = -(nSklv * 7) - nCurrInt * 2;
		af.bitvector =  0;
		affect_to_char( victim, &af );

		if(nSklv >= 4){
			af.location	 =  APPLY_RMAGIC;
			af.modifier  = (nCurrInt - 5) / (7 - nSklv);
			affect_to_char( victim, &af );
		}
		send_to_char( "你的意志變得十分堅強.\n\r", victim );
	}
}


void spell_song_of_cleanse( int sn, int level, CHAR_DATA *ch, void *vo )
{

	CHAR_DATA *victim = (CHAR_DATA*) vo;
	int base = 0;
	int sklv = get_skill_level(ch,sn);
	int obj_lv = level;

	if( IS_SET(victim->act, PLR_KILLER) ) base += 15 - sklv;
	if( IS_SET(victim->act, PLR_THIEF) ) base += 6 - sklv/2;
	if( victim->class > BARBARIAN ) base *= 2;
	base *= victim->level;

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

	switch(sklv) {
		case 6:
			affect_strip( victim, gsn_mute);
			affect_strip( victim, gsn_seal_magic);
			affect_strip( victim, gsn_dark_containment);
			affect_strip( victim, gsn_vampiric_bite);
			if( !IS_NPC(victim) && !IS_SET( victim->act, ACT_PET ) ) // pet 不會被移除 charm 2022/12/24
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
			affect_strip( victim, gsn_fear);
			affect_strip( victim, gsn_virtual_swords);
			affect_strip( victim, gsn_sun_flash);
			affect_strip( victim, gsn_burning_hands);
			affect_strip( victim, gsn_chill_touch);
			affect_strip( victim, gsn_thunder_strike);
		case 3:
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

	act("$n"HIW"唱出神聖的歌曲，淨化了$N。"NOR"", ch, NULL, victim, TO_NOTVICT);
	act(""HIW"你唱出神聖的歌曲，淨化了"NOR"$N"HIW"。"NOR"", ch, NULL, victim, TO_CHAR);
	act("$n"HIW"唱出神聖的歌曲，淨化了你。"NOR"", ch, NULL, victim, TO_VICT);
	// 加入 pc 限定 及 exp 負值判定 ( imm 的情況 ) 2023/01/14
	if( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER) ){
		if( victim->exp >= 0 ){
			if( ( victim->exp - 1000 ) < base ){
				send_to_char("你或目標的經驗值不夠。\n\r", ch);
				return;
			}
		}
		REMOVE_BIT(victim->act, PLR_KILLER);
		victim->exp -= base;
		send_to_char("KILLER 頭銜已移除。\n\r", victim);
	}
	if( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF) ){
		if( victim->exp >= 0 ){
			if( ( victim->exp - 1000 ) < base ){
				send_to_char("你或目標的經驗值不夠。\n\r", ch);
				return;
			}
		}
		REMOVE_BIT(victim->act, PLR_THIEF);
		victim->exp -= base;
		send_to_char("THIEF 頭銜已移除。\n\r", victim);
	}
}


void spell_recall( int sn, int level, CHAR_DATA *ch, void *vo )
{
    QMARK *qm;
    ROOM_INDEX_DATA *to_room;
    int nRecallSklv = get_skill_level(ch, gsn_recall);
	int obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nRecallSklv = 0;
		if( obj_lv >= 10) nRecallSklv = 1;
		if( obj_lv >= 20) nRecallSklv = 2;
		if( obj_lv >= 30) nRecallSklv = 3;
		if( obj_lv >= 40) nRecallSklv = 4;
		if( obj_lv >= 50) nRecallSklv = 5;
		if( obj_lv >= 60) nRecallSklv = 6;
	}
    int nManaRegain = nRecallSklv * 15;
    if(nRecallSklv > 4) nManaRegain = (nRecallSklv - 1) * nRecallSklv * (nRecallSklv + 1);

    if( ch->fighting && nRecallSklv < 5) {
        send_to_char("在戰鬥中無法集中精神！\n\r", ch);
        return;
    }

    if( !( qm = is_qmark(ch, "spell_mark") )) {
        send_to_char("你還沒設定魔力標記。\n\r", ch);
        return;
    }

    if( !(to_room = get_room_index( qm->v0 ) )){
        send_to_char("你還沒設定魔力標記。\n\r", ch);
        return;
    }

   //  in different recall ? 
    if( ch->in_room->area->recall != to_room->area->recall ){
		// 加入 recall sklv 設定 add at 2020/10/22
        if(get_skill_level(ch, gsn_mark) < 6 && nRecallSklv < 6){
            send_to_char("你無法傳送到不同的時空中。\n\r", ch);
            return;
        }
    }
	
   //  sklv = 0 , 不能跨區傳送 add at 2020/10/22
    if( ch->in_room->area != to_room->area ){
		if(get_skill_level(ch, gsn_mark) < 1 && nRecallSklv < 1){
			send_to_char("你無法傳送到不同區域。\n\r", ch);
			return;
		}
    }

   //  in different area add at 18/8/8
    if( IS_SET(to_room->room_flags, ROOM_NO_RECALL_IN)
		&& ch->in_room->area != to_room->area )
	{
        send_to_char("一股力場阻擋你傳送到那裡。\n\r", ch);
        return;
    }

	// 修改 room_is_private 代入值 2022/12/20
    //if( room_is_private( to_room ) )
	if( room_is_private( ch , to_room ) )
	{
		//send_to_char( "[33m那裡現在已經被擠滿了。[m\n\r", ch );
		send_to_char( "[33m那裡的空間大小，容納不下你的體積。[m\n\r", ch );
		return;
	}

   //  area immhall can't mark/recall add at 18/8/8
    if( ch->in_room->vnum >= 11001 && ch->in_room->vnum <= 11099 ) {
		send_to_char("你失敗了。\n\r", ch);
		return;
	}

	// 防止 mark 的 qmark 變為不存在的 room 造成crash
	if(to_room == NULL){
		send_to_char("你失敗了。\n\r" , ch );
		return;
	}

    ch->mana += nManaRegain;

    if( ch->in_room->progtypes & ROOM_LEAVE)
        rprog_leave_trigger( ch->in_room, ch, 8 );
    if ( ch->fighting )
        stop_fighting( ch, TRUE );
    act( "$n 的身影逐漸消失.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch);
    char_to_room( ch, to_room);
    act( "$n 的身影逐漸出現.",   ch, NULL, NULL, TO_ROOM );
    do_look(ch, "auto" );
    if( to_room->progtypes & ROOM_ENTER)
        rprog_entry_trigger(to_room, ch, 6);

}

void spell_mark( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[BUF_STRING_LENGTH];
    int dura = ch->level/4+get_skill_level(ch,sn);
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
    QMARK * qm;
    if( IS_NPC(ch) ) return;
	// 無法mark的地方
    if( IS_SET(ch->in_room->room_flags, ROOM_NO_MARK) ){
				send_to_char("你失敗了。\n\r", ch);
				return;
    }
    if ( !IS_SET( ch->in_room->room_flags, ROOM_SAVE )
	    && str_cmp( ch->in_room->area->identity, "skarabrae" )
	    && !IS_IMMORTAL( ch) )
        dura -= 2;
    if( IS_SET(ch->in_room->room_flags, ROOM_SAVE))
        dura += 4;
    if( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)){
        if( sklv < 2 )
            dura = -100;
        else
            dura -= 2;
    }
    if( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ){
        if( sklv < 4 )
            dura = -100;
        else
            dura -= 2;
    }
    if( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT_TO) ){
        if( sklv < 5 )
            dura = -100;
        else
            dura -= 2;
    }
    if( ch->in_room->vnum >= 11000 && ch->in_room->vnum <= 11099 ) {
				send_to_char("你失敗了。\n\r", ch);
				return;
	}
    if( ch->in_room->vnum == 4159 ) // ninja trial
        dura = 1;
    dura = UMIN( (dura * ch->level) / 4, (dura * ch->level) / 2);
    if( dura < 1 ) {
        send_to_char("你失敗了。\n\r", ch);
        return;
    }
    send_to_char("你集中法力，在此地暫時設下魔力印記。\n\r", ch);
    if( ( qm = is_qmark(ch, "spell_mark" ) )){
        qm->v0 = ch->in_room->vnum;
        qm->timer = dura;
        return;
    }
    
    sprintf( buf, "self spell_mark %d %d 0 0 0 0" , dura, ch->in_room->vnum);
    do_add_qmark( ch, buf );
}



