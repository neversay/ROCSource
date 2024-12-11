//*****************************************************************
//*  PROGRAM ID  : spell_assist.c                                 *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940610 jye                                    *
//*  PURPOSE     :         �ק�fly�u���ѤH���|�Q�\��              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940611 jye                                    *
//*  PURPOSE     :         �ק�armor,shield,stone                 *
//*              :             lv4�H�W���|�Qlv4�H�U�\��           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991229 tale                                    *
//*  PURPOSE     :         �ק�flaming_armor                *
//*              :             �N mdr & svfire �[�J�v�T�ˮ`�ܼ�           *
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
	// �Τ@ armor , stone , shield ��pdr mdr �[���� 2022/01/13
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
		// �l�[ cle base class ��ۤv��]���⭿ 2022/01/13
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
		send_to_char( "�n���C\n\r", ch );
    
	send_to_char( "�A�Pı����O�@.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P��ɰ_�F�@�D�H�B����.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P��ɰ_�F�@�D�e������.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P��ɰ_�F�@�D�j�a����.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P��ɰ_�F�@�D���K����.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P��ɰ_�F�@�D�{�q����.\n\r", victim );
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
		// �]�O�����P�w 2022/01/08
		if( ch->mana < mana_cost ){
			send_to_char( "�A���]�O������Ҧ������I��t�Ԥ��֡C\n\r", ch );
			break;
		}
		if( !is_same_group(ch, victim))
			continue;
		if( victim->deleted )
			continue;

		// �����԰����L�k���[���]�w 2022/01/08
		//if(victim->position != POS_FIGHTING) {

		if( is_affected( victim, skill_lookup("holy crown"))){
			if(victim->pp > victim->damroll)
				continue;
			affect_strip( victim, skill_lookup("holy crown")); 
		}
		if( is_affected( victim, skill_lookup("day of power"))) {
			affect_strip( victim, skill_lookup("day of power")); 
		}

		// mana �q�T�w 20 �դɬ� 20 + 5 * sklv , �B�I�k�̦b�԰��ɱN���� 3 �� 2022/01/08
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
		// �s�W ac �[�� , �� con �v�T 2022/01/08
		af.location   = APPLY_AC;
		af.modifier   = -( get_curr_con(ch)*2 + get_skill_level(ch, sn)*get_skill_level(ch, sn) + get_skill_level(ch, sn)*30 );
		affect_to_char( victim, &af );

		send_to_char( HIW"�A�Pı��@���t�Ըֽg�b�A�ߤ��u�۵�."NOR"\n\r", victim );
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
			send_to_char("�A�䤣���D�C\n\r", ch);
			return;
		} else if (nSklv >= 6 && victim == NULL) {
			victim = ch;
		}
		if( victim->position != POS_STANDING ){
			isBattleCast = TRUE;
			if(nSklv < 6) {
				send_to_char("�A���ޥ��٤����H�b�԰����I�i��������C\n\r", ch);
				return;
			}
			if(ch->mana < 125) {
				send_to_char("�A���k�O�����H�b�԰����I�i��������C\n\r", ch);
				return;
			}
			ch->mana -= 125;
		}
		if( victim->mana < 150 ) {
			send_to_char("�A���٦�L�k�t�X�A�C\n\r", ch);
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
				send_to_char( HIC"�����絹�A�����]�k����O."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect magic"))){
				affect_strip( victim, skill_lookup("detect magic"));
				af.type   = skill_lookup("detect magic");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_MAGIC;
				affect_to_char( victim, &af );
				send_to_char( HIC"�����絹�A�����]�k����O."NOR"\n\r", victim );
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
				send_to_char( HIC"�����礩�A���머�c����O."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect evil"))){
				affect_strip( victim, skill_lookup("detect evil"));
				af.type   = skill_lookup("detect evil");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_EVIL;
				affect_to_char( victim, &af );
				send_to_char( HIC"�����礩�A���머�c����O."NOR"\n\r", victim );
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
				send_to_char( HIC"���������O�O�A�P�������@�}��h."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect invis"))){
				affect_strip( victim, skill_lookup("detect invis"));
				af.type   = skill_lookup("detect invis");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_INVIS;
				affect_to_char( victim, &af );
				send_to_char( HIC"���������O�O�A�P�������@�}��h."NOR"\n\r", victim );
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
				send_to_char( HIC"���������O�O�A�o�H��ı�ðΥͪ�."NOR"\n\r", victim );
			}else if ( is_affected( victim, skill_lookup("detect hidden"))){
				affect_strip( victim, skill_lookup("detect hidden"));
				af.type   = skill_lookup("detect hidden");
				af.duration   = ch->level + get_skill_level(ch, af.type)*2;
				af.location   = APPLY_NONE;
				af.modifier   = 0;
				af.bitvector = AFF_DETECT_HIDDEN;
				affect_to_char( victim, &af );
				send_to_char( HIC"���������O�O�A�o�H��ı�ðΥͪ�."NOR"\n\r", victim );
			}
		}

		if ( is_affected( victim, sn))
		{
			affect_strip( victim, sn);
		}
		// �ܥ��z
		af.type      = sn;
		af.duration   = UMAX(0,ch->level - 33) + nSklv * 2;
		af.location   = APPLY_DOB_SAVING_FLASH;
		af.modifier   = ( nSklv + 1 ) * 9 + UMAX( get_curr_wis(ch) - 16, 0)* 3 ;
		af.bitvector = 0;
		affect_to_char( victim, &af );

		af.location   = APPLY_SAVING_SPELL;
		// �ק� dob svs ���u 2023/01/12
		//af.modifier   = - 2 - (get_curr_wis(ch)-10)/3 - nSklv - nSklv/3;
		af.modifier   = 0 - ( 2 + nSklv + nSklv*nSklv / 6 );
		af.bitvector = 0;
		affect_to_char( victim, &af );

		if( nSklv > 3 ){
			// dob element �[���������קאּ mdr 2023/01/12
			//af.location  = APPLY_SAVING_ELEMENT;
			af.location  = APPLY_RMAGIC;
			af.modifier  = (nSklv*4 + nSklv*nSklv)/2;
			af.bitvector = 0;
			affect_to_char( victim, &af );
			send_to_char( HIC"�A�Pı����������[�@."NOR"\n\r", victim );
		}
    
		ch->wait = wait + 1;
		if ( ch != victim )
			send_to_char( "�n���C\n\r", ch );
			send_to_char( HIW"�ѤW���U�@�D�����A�ӫG�A������."NOR"\n\r", victim );
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

	// ���s�]�p dop �� sklv ���u 2023/01/12
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
				send_to_char("�A�䤣���̡C\n\r", ch);
				return;
			}
			if( victim->position != POS_STANDING || victim->mana < 100 ) {
				send_to_char("�A���٦�L�k�t�X�A�C\n\r", ch);
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
			send_to_char( "�n���C\n\r", ch );
		send_to_char( "�A�P��ɰ_�F�@�D���t����.\n\r", victim );
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
		// �]�O�����P�w 2022/01/08
		if( ch->mana < mana_cost ){
			send_to_char( "�A���]�O������Ҧ������I��t���a�áC\n\r", ch );
			break;
		}
		if( !is_same_group(ch, victim))
			continue;
		if( victim->deleted )
			continue;

		// �����԰����L�k���[���]�w 2022/01/08
		//if (victim->position != POS_FIGHTING) {

		if( is_affected( victim, skill_lookup("holy crown")) ){
			affect_strip( victim, skill_lookup("holy crown"));
		}
		if( is_affected( victim, skill_lookup("day of power")) ){
			if(victim->pp < victim->damroll) continue;
			affect_strip( victim, skill_lookup("day of power"));
		}

		// mana �q�T�w 20 �դɬ� 20 + 5 * sklv , �B�Q�I�k�̦b�԰��ɱN���� 3 �� 2022/01/08
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

		send_to_char( HIW"�A���|�P�ɰ_�@�ѱj�j���F��!"NOR"\n\r", victim );
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
		// �ץ� bless �л\�ƭȻP�[���Ȥ��۵������D, �ç� > �קאּ >= ���P���ů��л\�ª� buff 2023/01/12
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�Pı���쯬��.\n\r", victim );
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

    send_to_char( "�A�{�b�i�H�b���̩I�l�F.\n\r", victim );
    act( "$n �{�b�i�H�b���̩I�l�F.", victim, NULL, NULL, TO_ROOM );
    return;
}


void spell_cone_of_silence( int sn, int level, CHAR_DATA *ch, void *vo )
{
    ROOM_INDEX_DATA *pRoomIndex;

    if ( !( pRoomIndex = ch->in_room ) )
		return;

    if ( IS_SET( pRoomIndex->room_flags, ROOM_SAFE ) )
    {
		send_to_char( "�o�̤���o��.\n\r", ch );
		return;
    }

    if ( !IS_SET( pRoomIndex->room_flags, ROOM_CONE_OF_SILENCE ) )
    {
		SET_BIT( pRoomIndex->room_flags, ROOM_CONE_OF_SILENCE );
			
		send_to_char( "�A�b�o�̫إߤF�H�q�O���A�P���M�w�R�U�ӡC\n\r", ch );
		act( "$n �b�o�̫إߤF�H�q�O���A�P���n���������F�C", ch, NULL, NULL, TO_ROOM );
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
	send_to_char ("�A�n���Ѯ��ܦn(better)���ܮt(worse)�H\n\r", ch );

    send_to_char( HIC "�ƭӯ������Ϲ���r�X�{�b�A���e�A���̪��C���ܤ۵ۡA�Ѯ���G�]�]���Ө���F�v�T�C\n\r"NOR"", ch);
    act( HIC "��"NOR"$n"HIC"�����믫���P�ɡA�L���e�B�{�X�_�����Ϲ���r�C"NOR"" , ch, NULL, NULL, TO_ROOM );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�Pı��A����머�c���𮧤F.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�}�l����ĵı.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P�������@�}��h.\n\r", victim );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A�P��A������]�k���𮧤F.\n\r", victim );
	WAIT_MAGIC_STATE(ch, 8);
	return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *obj = (OBJ_DATA *) vo;

	if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
	{
		if ( obj->value[3] != 0 )
			send_to_char( "�A�D��@�Ѽ@�r�����.\n\r", ch );
		else
			send_to_char( "��A�ݨӤQ���i�f.\n\r", ch );
	}
	else
	{
		send_to_char( "���ݰ_�ӨèS���t�r.\n\r", ch );
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
				send_to_char(" �A�L�k���h����.\n\r", ch);
				continue;
			}

			if( paf->type == gsn_defensive )
				continue;
				
			if( paf->type == gsn_blood_thirsty )
				continue;
			
			if( paf->type == gsn_meditate ) {
				send_to_char( "�A�L�k���h�߫�.\n\r", ch);
				continue;
			}
			// 2021/11/23
			if ( paf->type == gsn_lifeshare )
			{
				send_to_char("�A�L�k���h�P�ͦ@��.\n\r", ch );
				continue;
			}
			if( paf->type == gsn_nightstalk) {
				send_to_char("�A�L�k���h����.\n\r", ch);
				continue;
			}
			if( paf->type == gsn_sprinkle_slash) {
				send_to_char("�A�L�k�����i���C����¶.\n\r", ch);
				continue;
			}
			if( paf->type == gsn_resurrection ) {
				send_to_char("�A�L�k���h�_������.\n\r", ch);
				continue;
			}
			/*
			if (( paf->type == skill_lookup( "polymorph other" )
				|| paf->type == gsn_morphing)
				&& get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char( "�A�L�k���h�ر��ܲ�.\n\r", ch );
				continue;
			}
			*/
			if ( paf->type == gsn_vampiric_bite && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("�A�L�k���h�l�尭�A�G.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_disease && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("�A�L�k���h�e�f.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_poison && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("�A�L�k���h�@�r.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_paralyze && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char( "�A�L�k���h�·�����.\n\r", ch );
				continue;
			}
			if ( paf->type == gsn_dark_containment && get_trust( ch ) < LEVEL_IMMORTAL )
			{
				send_to_char("�A�L�k���h��G����.\n\r", ch );
				continue;
			}
			if ( paf->type == skill_lookup("demon tremor") && get_trust( ch ) < LEVEL_IMMORTAL)
			{
				send_to_char( "�A�L�k�X���ߤ��L�ڪ��c�].\n\r", ch );
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
						// �N fall_effect ����̫�P�w 2023/01/15
						//if( temp_paf->type == gsn_flying )
						//	fall_effect( victim );
					}
				}
			}else{
				send_to_char( "�A���ѤF�I\n\r", ch );
				continue;
			}
		}

		if ( victim == ch )
		{
			act( "�A���h�F�ۤv���W���k�N����.", ch, NULL, NULL, TO_CHAR );
			act( "$n���h�F�ۤv���W���k�N����.", ch, NULL, NULL, TO_ROOM );
		}
		else
		{
			act( "�A���h�F$N���W���k�N����.", ch, NULL, victim, TO_CHAR );
			act( "$n���h�F�A���W���k�N����.", ch, NULL, victim, TO_VICT );
			act( "$n���h�F$N���W���k�N����.", ch, NULL, victim, TO_NOTVICT );
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
				act( "$n���A���� '$t' ���@�μv�T.", victim, skill_table[paf->type].name, NULL, TO_ROOM );
				affect_strip( victim, paf->type );
				return;
			}
			else
			{
				send_to_char( "�A���ѤF�I\n\r", ch );
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

				// pet ���|�Q���� charm 2022/12/24
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
						act( "�A���h�F$N���W���k�N����.", ch, NULL, victim, TO_CHAR );
						act( "$n���h�F$N���W���k�N����.", ch, NULL, victim, TO_NOTVICT );
					break;
				}
				else
				{
						send_to_char( "�A���ѤF�I\n\r", ch );
					if ( ( !victim->fighting || !is_same_group( ch, victim->fighting ) ) && number_bits( 4 ) == 1 )
						multi_hit( victim, ch, TYPE_UNDEFINED );
					return;
				}
			}
		}
    }

	mprog_hurt_trigger(victim, ch, sn);

	// fly �P�w�b fall_effect �������ݭn�t�~�P�w 2023/01/15
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

	send_to_char( "�A����@���A�ۨӤF�@�}�������.\n\r", ch );
	act( "$n ����@���A�ۨӤF�@�}�������.", ch, NULL, NULL, TO_ROOM );

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

		act( "$n �����v�q�������B�{�I", ich, NULL, NULL, TO_ROOM );
		send_to_char( "�A�Q�o�{�F�I\n\r", ich );
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

	// fly �C lv ���� dc �[�� , lv 6 dc �[���W�[ 2022/01/13
	if( sklv > 0 ) fly_dc -= 6;
	if( sklv > 1 ) fly_dc -= 7;
	if( sklv > 2 ) fly_dc -= 8;
	if( sklv > 3 ) fly_dc -= 9;
	if( sklv > 4 ) fly_dc -= 8;
	if( sklv > 5 ) fly_dc -= 7;

	if( is_affected( victim, skill_lookup("fly"))) 
	{
		// �קאּ�P�w dc �ȬO�_�j�� fly_dc �[�� 2022/01/13
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

	send_to_char( "�A�����}���������F�a��.\n\r", victim );
	act( "$n �����}���������F�a��.", victim, NULL, NULL, TO_ROOM );
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
	// �԰� pd �[�� 2022/01/13
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
        send_to_char( "�n���C\n\r", ch );
    send_to_char( "�A�P��ۤv��j���F.\n\r", victim );
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
		send_to_char( "�A�������{�{�۬���.\n\r", victim );
		act( "$n �������{�{�۬���.\n\r", ch, NULL, NULL, TO_ROOM );
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

	act( "$n �����v�v������.", victim, NULL, NULL, TO_ROOM );

	af.type	 = sn;
	af.duration	 = ch->level/2 + get_curr_wis( ch ) + get_skill_level(ch,sn) * 6;
	af.location	 = APPLY_NONE;
	af.modifier	 = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( victim, &af );

	send_to_char( "�A�����v�v������.\n\r", victim );

	WAIT_MAGIC_STATE(ch, 8);
	return;
}



void spell_know_alignment( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	char *msg;
	int ap;

	ap = victim->alignment;

	// �ק� know alignment �d�� 2022/10/25

	/*if ( ap >  700 ) msg = "$N �P��Ţ�n�ۼ�զ��������~.";
	else if ( ap >  350 ) msg = "$N �O�A�D�w�W����d.";
	else if ( ap >  100 ) msg = "$N �ݨӤ��O����K.";
	else if ( ap > -100 ) msg = "$N ���~�w���I�p�ʳ�.";
	else if ( ap > -350 ) msg = "$N �s�n�B�ͳ��|���F.";
	else if ( ap > -700 ) msg = "$N �ݨӷQ�����A�I";
	else msg = "�٬O�O�b $N ���e�û��ܦn�F...�_�h�i�঳�M�I."; */

	if ( ap >  899 ) msg = "$N �P��Ţ�n�ۼ�զ��������~.";
	else if ( ap >  749 ) msg = "$N �O�A�D�w�W����d.";
	else if ( ap >  499 ) msg = "$N �ݰ_�Ӥ��O���}.";
	else if ( ap >  349 ) msg = "$N �Y�u�D�w�W��.";
	else if ( ap >   99 ) msg = "$N ��H���A�שM��.";
	else if ( ap > -100 ) msg = "$N �����O�D���c������.";
	else if ( ap > -350 ) msg = "$N ���~�w���ǯʳ�.";
	else if ( ap > -500 ) msg = "$N �s�B�ͳ���X��.";
	else if ( ap > -750 ) msg = "$N �����R���ɼF����.";
	else if ( ap > -900 ) msg = "$N �ݨӷQ�����A�I";
	else msg = "�٬O�O�b $N ���e�û��ܦn�F...�_�h�i�঳�M�I.";

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

		send_to_char( "�A�����v�v������.\n\r", gch );
		act( "$n �����v�v������.", gch, NULL, NULL, TO_ROOM );

		af.type	     = sn;
		af.duration  = ch->level/10 + get_curr_wis( ch ) + get_skill_level(ch,sn)*4;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_INVISIBLE;
		affect_to_char( gch, &af );
	}
	send_to_char( "�n���C\n\r", ch );

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

	send_to_char( "�A�����麥���ܦ��b�z��.\n\r", victim );
	act( "$n �����麥���ܦ��b�z��.", victim, NULL, NULL, TO_ROOM );
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
		send_to_char( "�n���C\n\r", ch );
	send_to_char( "�A���|�P�ɰ_�F���t���O�@�O��.\n\r", victim );
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
			send_to_char( "�A�����F�o�̪��H�q�O���I\n\r", ch );
			act( "$n �����F�o�̪��H�q�O���I", ch, NULL, NULL, TO_ROOM );
			DidSomething = TRUE;
		}

		if ( IS_AFFECTED( victim, AFF_MUTE ) )
		{
			affect_strip( victim, gsn_mute );
			send_to_char( "�A�����F���W���T���N.\n\r", ch );
			act( "$n �����F�ۤv���W���T���N.", ch, NULL, victim, TO_VICT );
			DidSomething = TRUE;
		}

		if ( !DidSomething )
			send_to_char( "����]�S�o��...\n\r", ch );

		return;
	}

	if ( IS_AFFECTED( victim, AFF_MUTE ) )
    {
		affect_strip( victim, gsn_mute );
		act( "�A�����F $N ���W���T���N.", ch, NULL, victim, TO_CHAR );
		act( "$n �����F�A���W���T���N.", ch, NULL, victim, TO_VICT );
		act( "$n �����F $N ���W���T���N.", ch, NULL, victim, TO_NOTVICT );
	}
	else
		act( "$N �ݨӦn����.",	ch, NULL, victim, TO_CHAR    );

	return;
}


// modified by whisperwind 9.18.2004
// ���s�w�q sanctuary �ĪG 2022/10/26
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
		�t���N�קאּ�����t,�t�ܩʭ�, �åB�Q�I�k�̷|�o�� aff_sanctuary �� affect
		�֦� aff_sanctuary �i�H�̧ܩʭȤ��P���t�t�k�N�t���ĪG�@���
		�[�����`�ƭȬ� cast_total , ���ȷ|�� ch �� alignment �������t�����[���t,�t�ܩʭ�
		�Ӥ��t���t,�t�ܩʭȷ|�P ch ���}��ۤ�
		�� : ch �� alignment ���ͪ� 1000 , �h sv dark �|������h, �Ϥ��ͪ� -1000 �h sv saint �|�����h

		�t���N���k�N�|�\�H�`�ƭȧ@�P�w, �p�G cast_total �j�� victim ���W����, �~�|�@�|�\�k�N���欰

		���~, sanctuary ���j�׷|�� divine light sklv �v�T , divine light ��P�|�� sanctuary �v�T
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
			send_to_char( "�L���W���t���N��A�I���ٱj�O!\n\r", ch );
			return;
		}else{
			affect_strip( victim, gsn_sanctuary );
		}
	}

	nSvSaint = 0 - (cast_total * ( 1000 - ch->alignment )/ 2000 );
	nSvDark = 0 - (cast_total * ( 1000 + ch->alignment )/ 2000 );

	// �קאּ�P�w�[���O�_�� 0 �M�w�O�_�[�J affect 2022/11/04
	//if(ch->alignment < 1000){
	if( nSvSaint != 0 ){
		af.type	     = sn;
		//af.duration	 = number_fuzzy( ( nLevel + get_curr_wis( ch ) - 10 ) / 9 )+get_skill_level(ch,sn) + get_skill_level(ch,sn)/2;
		af.duration  = 1 + sklv + ch->level/10;
		af.location	 = APPLY_SAVING_SAINT;
		af.modifier	 = nSvSaint;
		// �ѥ� sanctuary �����p�h���W�[ bitvector 2022/10/26
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

	send_to_char( "�@�D�զ�t��Ţ�n�F�A����.\n\r", victim );
	act( "�@�D�զ�t��Ţ�n�F $n.", victim, NULL, NULL, TO_ROOM );
	return;
}

// modified by whisperwind 9.18.2004
// ���s�w�q divine light �ĪG 2022/10/26
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

	act( ""HIY"�ѤW�w�w���U���䪺�t���A�@��"NOR"$n"HIY"�M$s���͡I"NOR"", ch, NULL, NULL, TO_ROOM );
	act( ""HIY"�ѤW�w�w���U���䪺�t���A�@��"NOR"�A"HIY"�M�A�����͡I"NOR"", ch, NULL, NULL, TO_CHAR );

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
		// �קאּ�P�w�[���O�_�� 0 �M�w�O�_�[�J affect 2022/11/04
		//if(ch->alignment < 1000){
		if( nSvSaint != 0 ){
			//af.type	 = sn;  �ק� buffer name �� sanctuary 2022/10/26
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

	//send_to_char( "�n��.\n\r", ch );

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
  	// �Τ@ armor , stone , shield ��pdr mdr �[���� 2022/01/13
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

	send_to_char( "�@�ѤO����¶�b�A����.\n\r", victim );
	act( "�@�ѤO����¶�b $n ����.", victim, NULL, NULL, TO_ROOM );
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
  	// �Τ@ armor , stone , shield ��pdr mdr �[���� 2022/01/13
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
		// �l�[ cle base class ��ۤv��]���⭿ 2022/01/13
		if(!IS_NPC(ch) && ch == vo && (ch->pcdata->prime_attr == APPLY_INT || ch->pcdata->prime_attr == APPLY_WIS) )
			af.modifier += af.modifier / 2;
		af.bitvector = 0;
		affect_to_char( victim, &af );
	}

  
	send_to_char( "�A���ֽ��v���w��.\n\r", victim );
	act( "$n ���ֽ��v���w��.", victim, NULL, NULL, TO_ROOM );
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
    
	send_to_char( "�A�P��@�ѯ��O�b��ެy«�I\n\r", victim );
	act( "$N��$n�����ը�!", victim, NULL, ch, TO_ROOM );

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
		send_to_char( "�n��.\n\r", ch );
	send_to_char( "�A���԰���ı�v���ܱo�ӾU.\n\r", victim );
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
			send_to_char("�A����O�����H�i��o�l�C\n\r", ch);
			return;
		}
		ch->move -= 100;
	}


	af.type      = sn;
	af.duration  = level / 4 + sklv;
	af.location  = APPLY_DAMROLL;
	// �j�� powerup �� sklv �v�T 2022/02/18
	//af.modifier  = ( get_curr_str(ch) + get_curr_wis(ch) )/ 5 + get_skill_level(ch,sn);
	af.modifier  = ( get_curr_str(ch) + get_curr_wis(ch) )/ 5 + sklv + (sklv * sklv)/ 3;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	send_to_char("�A�餺���l�O����������X�C\n\r", victim);
	if( ch != victim )
		send_to_char("�n���C\n\r", ch);
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
            send_to_char("�A����O�����H�I�i��𬡨��k�C\n\r", ch);
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

    send_to_char("�Aı�o�ۤv������ܱo�ӱ��\\�h�C\n\r", victim);
    if( ch != victim )
        send_to_char("�n���C\n\r", ch);
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

    send_to_char( "�A�b���ǫإ߰_�@�D�O��.\n\r", victim );
    return;
}

/* New style and function of lore, designed by keelar
 * 
 * �A���]�k�}�x�L�����F�A�o��Q�n����ơC
 *
 * ������������������������������������������������������������
 * ���Z��(Peltephius) ����ơG                                // sklv 2 for HP
 * ���šG44  �ʧO�G�k  �}��G 0
 * �O�q�G22  ���O�G18  ���z�G20  �ӱ��G22  ���G22
 * �ͩR�O�G19541/19541   �]�k�O�G1002/1002  ���ʤO�G9770/9770 
 *
 * �԰���O���                                               // sklv 1 ~ 5
 * �D�n�Z���R���G   47   �D�n�Z���ˮ`�G 126
 *     �]�k��O�G    0   ���z�ˮ`�G  70  �]�k�ˮ`�G 150
 *
 * ���m�򥻸��                                               // sklv 4 ~ 5
 *     �@�Ҩ��áG-4136   ���z�ܩʡG 300  ���z�پ��G   0
 *     �{�ׯ�O�G   20   �]�k�ܩʡG 100  �]�k�پ��G   0
 *
 * �����]�k�ܩʡG                                             // sklv 6
 *     �ܪ��z�G -29   �ܪ��V�G -38   �ܵ����G  31
 *     �ܹp�{�G -43   �ܦB���G -39   �ܤj�a�G -39
 *
 * �S���]�k�ܩʡG                                             // sklv 4
 *     ���s���G -71   �ܯ��t�G -10   �ܶ���G -10
 *
 * ��S��ޯ઺�ܩʱM���ήz�I�G                               // sklv 6 and lv41
 * [              circle] �ޯ�R���[�� -25%�A�ˮ`�[�� -99%�C  // while mob with low HP
 * [            backstab] �ޯ�R���[��   0%�A�ˮ`�[�� -99%�C
 * [     general purpose] �ޯ�R���[��   0%�A�ˮ`�[�� -99%�C
 * [            bashhead] �ޯ�R���[�� -99%�A�ˮ`�[�� -99%�C
 * [           soulsteal] �ޯ�R���[�� -99%�A�ˮ`�[�� -99%�C
 * [                feed] �ޯ�R���[�� -99%�A�ˮ`�[�� -99%�C
 * [         faerie fire] �ޯ�R���[�� 100%�A�ˮ`�[�� 100%�C
 * [               tardy] �ޯ�R���[�� 100%�A�ˮ`�[�� 100%�C
 * [              weaken] �ޯ�R���[�� 100%�A�ˮ`�[�� 100%�C
 * [               curse] �ޯ�R���[�� 100%�A�ˮ`�[�� 100%�C
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
		send_to_char( "�A�L�k�γo���k�N�o����L�_�I�̪����\n\r", ch );
		return;
	}else if(IS_SET(victim->special, NO_LORE) && !IS_IMMORTAL(ch)){
		send_to_char( "�L���N�ӤO��b�ӱj�F�I�A�����L�k�ݳz�L�����F�I\n\r", ch );
		return;
	}

  if(IS_IMMORTAL(ch)) nSklv = 99;
	
	rgLore[0] = 0;
  sprintf(rgBuf, "�A���]�k�}�x�L�����F�A�o��Q�n����ơC\n\n\r"); 
	strcat(rgLore, rgBuf);

  // Mob basic Info
	sprintf(rgBuf, "������������������������������������������������������������\n\r" );
	strcat(rgLore, rgBuf);
	sprintf(rgBuf, "%s - %s(%s):\n\r���šG%2d  �ʧO�G%s  �}��G %d\n\r", 
	               victim->short_descr, 
	               race_table[ victim->race ].cname, race_table[ victim->race ].name,
	               victim->level, 
	               victim->sex == SEX_MALE ? "�k" :
	               victim->sex == SEX_FEMALE ? "�k" : "��",
								 victim->alignment);
	strcat(rgLore, rgBuf);

	// Five Basic Attributes
	sprintf(rgBuf, "�O�q�G%2d  ���O�G%2d  ���z�G%2d  �ӱ��G%2d  ���G%2d\n\r",
	               get_curr_str( victim ), get_curr_int( victim ),
								 get_curr_wis( victim ), get_curr_dex( victim ),
	               get_curr_con( victim ));
	strcat(rgLore, rgBuf);

	// HP/MP/MV Info
	if(nSklv >= 3){
		sprintf(rgBuf, "�ͩR�O�G%4d/%4d   �]�k�O�G%4d/%4d  ���ʤO�G%4d/%4d\n\r",
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
		sprintf(rgBuf, "�ͩR�O�G%s   �]�k�O�G%4d/%4d  ���ʤO�G%4d/%4d\n\r",
		               rgSec3, victim->mana, victim->max_mana,
									         victim->move, victim->max_move);
	}
	strcat(rgLore, rgBuf);

	// Fighting Related
	strcat(rgLore, "\n\r�԰���O��ơG\n\r");
	strcpy(rgSec1, " ����");
	strcpy(rgSec3, "����");
	if(nSklv >= 1) sprintf(rgSec1, "%5d", get_hitroll(victim, WEAR_WIELD));
	if(nSklv >= 3) sprintf(rgSec3, "%4d", get_damroll(victim, WEAR_WIELD));
	sprintf(rgBuf, "�D�n�Z���R���G%s   �D�n�Z���ˮ`�G%s\n\r", 
	               rgSec1, rgSec3);
	strcat(rgLore, rgBuf);

	if(get_eq_char( victim, WEAR_WIELD_2 )){
		strcpy(rgSec2, " ����");
		strcpy(rgSec4, "����");
		if(nSklv >= 2) sprintf(rgSec2, "%5d", get_hitroll(victim, WEAR_WIELD_2));
		if(nSklv >= 4) sprintf(rgSec4, "%4d", get_damroll(victim, WEAR_WIELD_2));
 		sprintf(rgBuf, "���n�Z���R���G%s   ���n�Z���ˮ`�G%s\n\r",
		             rgSec2, rgSec4);
		strcat(rgLore, rgBuf);
	}

	strcpy(rgSec4, " ����"); if(nSklv >= 4) sprintf(rgSec4, "%5d", victim->pp);
	strcpy(rgSec5, "����");  if(nSklv >= 5) sprintf(rgSec5, "%4d", victim->apdr);
	strcpy(rgSec6, "����");  if(nSklv >= 5) sprintf(rgSec6, "%4d", victim->amdr);
	sprintf(rgBuf, "    �]�k��O�G%s   ���z�ˮ`�G%s  �]�k�ˮ`�G%s\n\r", 
	               rgSec4, rgSec5, rgSec6);
	strcat(rgLore, rgBuf);

	// Physical Resistance 
	strcat(rgLore, "\n\r���m�򥻸�ơG\n\r");
	strcpy(rgSec4, "����"); if(nSklv >= 4) sprintf(rgSec4, "%4d", victim->pdr);
	strcpy(rgSec5, "����"); if(nSklv >= 5) sprintf(rgSec5, "%4d", victim->pad);
	
	sprintf(rgBuf, "    �@�Ҩ��áG%5d   ���z�ܩʡG%s  ���z�پ��G%s\n\r",
			            GET_AC( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Dodge and Magical Resistance 
	strcpy(rgSec4, "����"); if(nSklv >= 4) sprintf(rgSec4, "%4d", victim->mdr);
	strcpy(rgSec5, "����"); if(nSklv >= 5) sprintf(rgSec5, "%4d", victim->mad);
	
	sprintf(rgBuf, "    �{�ׯ�O�G%5d   �]�k�ܩʡG%s  �]�k�پ��G%s\n\r",
	                GET_DODGE( victim ), rgSec4, rgSec5);
	strcat(rgLore, rgBuf);

	// Magical Resistance in Elemental
	strcat(rgLore, "\n\r�����]�k�ܩʡG\n\r");
	if(nSklv >= 6){
		sprintf(rgSec6, "    ���]�k�G%4d   �ܪ��V�G%4d   �ܵ����G%4d\n\r",
		                GET_SVS(victim),
		                NEW_GET_SVS(victim, MAGIC_FIRE),
		                NEW_GET_SVS(victim, MAGIC_WIND));
	}else{
		sprintf(rgSec6, "    ���]�k�G%4d   �ܪ��V�G ???   �ܵ����G ???\n\r", 
				            GET_SVS(victim));
	}
	strcat(rgLore, rgSec6);

	if(nSklv >= 6){
		sprintf(rgSec6, "    �ܹp�{�G%4d   �ܦB���G%4d   �ܤj�a�G%4d\n\r", 
		                NEW_GET_SVS(victim, MAGIC_LIGHTNING),
		                NEW_GET_SVS(victim, MAGIC_COLD),
		                NEW_GET_SVS(victim, MAGIC_EARTH));
	}else{
		sprintf(rgSec6, "    �ܹp�{�G ???   �ܦB���G ???   �ܤj�a�G ???\n\r");
	}
	strcat(rgLore, rgSec6);

	strcat(rgLore, "\n\r�S���]�k�ܩʡG\n\r");
	if(nSklv >= 4) {
		sprintf(rgSec4, "    ���s���G%4d   �ܯ��t�G%4d   ����¡G%4d\n\r",
						NEW_GET_SVS(victim, MAGIC_BREATH), 
						NEW_GET_SVS(victim, MAGIC_SAINT), 
						NEW_GET_SVS(victim, MAGIC_DARK));
	}else{
		sprintf(rgSec4, "    ���s���G ???   �ܯ��t�G ???   �ܶ���G ???\n\r");
	}
	strcat(rgLore, rgSec4);

	// �C tick ���줤�r���ˮ` 2021/11/03
	if(nSklv >= 4) {
		sprintf(rgSec4, "    �r�ˮ`�G%4d\n\r",
						NEW_GET_SVS(victim,MAGIC_POISON));
	}else{
		sprintf(rgSec4, "    �r�ˮ`�G ???\n\r");
	}
	strcat(rgLore, rgSec4);

	// Weakness
  strcat(rgLore, "\n\r��S��ޯ઺�ܩʱM���ήz�I�G\n\r");
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
					sprintf(rgSec6, "[%20s] �ޯ�R���[�� %3d%%�A�ˮ`�[�� %3d%%�C\n\r",
					                pWeak->name, 
													pWeak->percent - 100, 
													pWeak->damage  - 100);
					strcat(rgBuf, rgSec6);
				}
				if(nWeakCnt >= 20 + nSklv*5) 
					strcat(rgBuf, "���ĤH���ܩʱM���ήz�I�h��A�L�k����}�x�I\n\r");
			}else{
				sprintf(rgBuf, "    �H�A���}��O�A�A�o�������z�ɤ~���[��X�L���M���ήz�I�C\n\r");
			}
		}else{
			sprintf(rgBuf, "    �L�M���ίS��z�I\n\r");
		}
	}else{
		sprintf(rgBuf, "    �A����O�ε��ũ|�L�k�}�x�ĤH���M���ήz�I�C\n\r");
	}
	strcat(rgLore, rgBuf);

	strcat(rgLore, "\n\r");
	send_to_char(rgLore, ch);
	// �W�[ lore �� hurt prog 2022/10/30
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
		send_to_char( "�A�L�k�γo���k�N�o����L�_�I�̪����\n\r", ch );
		return;	
	}
	if ( IS_SET( victim->special, NO_LORE ) && !IS_IMMORTAL( ch ))
	{
		send_to_char( "�L���N�ӤO��b�ӱj�F�I�A�����L�k�ݳz�L�����F�I\n\r", ch );
		return;	
	}	

    	buf1[0] = '\0';	
	sprintf( buf, "�A���]�k�}�x�L�����F�A�o��Q�n����ơC\n\r" );
	strcat( buf1, buf );
	sprintf( buf, "\n\r������������������������������������������������������\n\r" );
	strcat( buf1, buf );
	sprintf( buf, "Mob �W�١G%s. ���šG%d. �ʧO�G%s", victim->short_descr, victim->level,
	victim->sex == SEX_MALE ? "�k��\n\r" : 
	victim->sex == SEX_FEMALE ? "�k��\n\r" : "����\n\r" );
	strcat( buf1, buf );
    	sprintf( buf, "�ͩR�O�G%d/%d, �]�k�O�G%d/%d, ���ʤO�G%d/%d\n\r",
	victim->hit,  victim->max_hit,
	victim->mana, victim->max_mana,
	victim->move, victim->max_move );
	strcat( buf1, buf );	    
    	sprintf( buf,
	    "�O�q�G%d  ���ӡG%d  ���z�G%d  �ӱ��G%d  ���G%d\n\r",
	    get_curr_str( victim ),
	    get_curr_int( victim ),
	    get_curr_wis( victim ),
	    get_curr_dex( victim ),
	    get_curr_con( victim ) );
    	strcat( buf1, buf );
    	sprintf( buf, "�@�Ҩ��äO�G%d, �}��G%d\n\r", GET_AC( victim ), victim->alignment );
    	strcat( buf1, buf );
    	sprintf( buf, "����r�G%s\n\r", victim->short_descr );
    	strcat( buf1, buf );
    	sprintf( buf, "�򥻴y�z�G%s\n\r", victim->long_descr );
    	strcat( buf1, buf );
    	if ( level > L_APP - 2 )
    	{
		sprintf( buf, "�{�ײv�G%d\n\r", GET_DODGE( victim ) );
    		strcat( buf1, buf );
    	}
    	if ( level > L_APP - 1 )
    	{
		sprintf( buf, "���]�k�[�ߧ�ʪZ�����m�O�G%d\n\r", GET_SVS( victim ) );
    		strcat( buf1, buf );
    	}
    	if ( level > L_APP  )
    	{
		strcat ( buf1, "�D�n�Z�� " );
		sprintf( buf, "�R���v: %d	�ˮ`�O: %d.\n\r",
		get_hitroll( victim, WEAR_WIELD ),
		get_damroll( victim, WEAR_WIELD ) );
    		strcat( buf1, buf );

    		if ( get_eq_char( victim, WEAR_WIELD_2 ) )
    		{
			sprintf( buf, "���n�Z�� �R���v: %d  �ˮ`�O: %d.\n\r",
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

    send_to_char( ""HIP"�A�Q�@�麵���U�N�����K�]��I"NOR"\n\r", ch );
    act( ""HIP"$n �Q�@�麵���U�N��������K�]��I"NOR"", ch, NULL, NULL, TO_ROOM );
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

    send_to_char( ""HIB"�A���|�P�ɰ_�@�D�H�Ŧ⪺���w�I"NOR"\n\r", ch );
    act( ""HIB"$n ����C�C�ɰ_�@�D�H�Ŧ⪺���w�I"NOR"", ch, NULL, NULL, TO_ROOM );
    return;
}

void spell_sober_up( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int drunk;
	AFFECT_DATA af;

	if( IS_NPC( victim ) ){
		send_to_char("�u��缾�a�I�i�I\n\r",ch);
		return;
	}

	drunk = victim->pcdata->condition[COND_DRUNK];

	if( drunk <= 0 ){
		send_to_char((victim == ch ) ? "�A�@�I�]�S�K�C\n\r" :"�L�ݰ_�ӫܲM���C\n\r", ch);
		return;
	}

	// mv �קאּ�̦h�ܦ��@�b 2023/01/13
	victim->move = (100 * victim->move)/( 100 + drunk );

	victim->pcdata->condition[COND_DRUNK] -= drunk;

	if( victim != ch )
		send_to_char( "�n��.\n\r", ch ); // �[�J���� 2023/01/13

	send_to_char("�A�餺���s��v�����h�A�A�������]�v���M���F�C\n\r", victim);

	if( !is_affected( victim, gsn_weaken ) ) {
		af.type      = gsn_weaken;
		af.duration  = 2 + drunk/10;
		af.location  = APPLY_STR;
		af.modifier  = -2;
		af.bitvector = 0;
		affect_to_char( victim, &af );

		send_to_char( ""HIU"�A��Mı�o�n��z�I"NOR"\n\r", victim );
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

    send_to_char( ""HIW"�A���|�P�ɰ_�@�D�z�����]�O�Ϯg��I"NOR"\n\r", ch );
    act( ""HIW"$n ����C�C�ɰ_�@�D�z�����]�O�Ϯg��I"NOR"", ch, NULL, NULL, TO_ROOM );
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

    send_to_char( ""HIC"�A���|�P�ɰ_�@�D�C�m�������I"NOR"\n\r", ch );
    act( ""HIC"$n ����C�C�ɰ_�@�D�C�m�������I"NOR"", ch, NULL, NULL, TO_ROOM );
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
		send_to_char("�A����F�۴x�������H��O�H�I�i���F�W�T.\n\r", ch);
		return;
	}

	af.type	     = sn;
	af.duration	 = ch->level / 6 + nSklv + get_curr_int(ch) / 2;

	af.location	 = APPLY_INT;
	af.modifier	 = 1 + nSklv / 2;
	// �� sor ��ۤv�I�k���B�~�[�� 2022/01/13
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

	send_to_char( "�A���ߴ���M�@���ũ�.\n\r", victim );
	if(ch != victim)
		send_to_char( "�n��.\n\r", ch );
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
	// �[�J ch ���żv�T�@�ήɶ� 2022/01/13
	//af.duration	 = nSklv * 2 + UMAX(nSklv - 4, 0) * 4;
	af.duration	 = ch->level/20 + nSklv + UMAX(nSklv - 4, 0) * 2;
	af.location	 = APPLY_SAVING_SPELL;
	af.modifier	 = -1;
	af.bitvector =  0;
	for( victim = ch->in_room->people ; victim ; victim = victim->next_in_room ) {
		if( !is_same_group(ch, victim)) continue;
		if( victim->deleted ) continue;

		// ���԰������ؼЬI�� will fortress , ���C�ӥؼз|���� mana 2022/01/13
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
		send_to_char( "�A���N���ܱo�Q����j.\n\r", victim );
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
			if( !IS_NPC(victim) && !IS_SET( victim->act, ACT_PET ) ) // pet ���|�Q���� charm 2022/12/24
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
			// paralyze �ݦP�ɦ� skill �W�� buff �� aff_paralyzed �~�|�@�� 2022/06/14
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

	act("$n"HIW"�ۥX���t���q���A�b�ƤF$N�C"NOR"", ch, NULL, victim, TO_NOTVICT);
	act(""HIW"�A�ۥX���t���q���A�b�ƤF"NOR"$N"HIW"�C"NOR"", ch, NULL, victim, TO_CHAR);
	act("$n"HIW"�ۥX���t���q���A�b�ƤF�A�C"NOR"", ch, NULL, victim, TO_VICT);
	// �[�J pc ���w �� exp �t�ȧP�w ( imm �����p ) 2023/01/14
	if( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER) ){
		if( victim->exp >= 0 ){
			if( ( victim->exp - 1000 ) < base ){
				send_to_char("�A�ΥؼЪ��g��Ȥ����C\n\r", ch);
				return;
			}
		}
		REMOVE_BIT(victim->act, PLR_KILLER);
		victim->exp -= base;
		send_to_char("KILLER �Y�Τw�����C\n\r", victim);
	}
	if( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF) ){
		if( victim->exp >= 0 ){
			if( ( victim->exp - 1000 ) < base ){
				send_to_char("�A�ΥؼЪ��g��Ȥ����C\n\r", ch);
				return;
			}
		}
		REMOVE_BIT(victim->act, PLR_THIEF);
		victim->exp -= base;
		send_to_char("THIEF �Y�Τw�����C\n\r", victim);
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
        send_to_char("�b�԰����L�k�����믫�I\n\r", ch);
        return;
    }

    if( !( qm = is_qmark(ch, "spell_mark") )) {
        send_to_char("�A�٨S�]�w�]�O�аO�C\n\r", ch);
        return;
    }

    if( !(to_room = get_room_index( qm->v0 ) )){
        send_to_char("�A�٨S�]�w�]�O�аO�C\n\r", ch);
        return;
    }

   //  in different recall ? 
    if( ch->in_room->area->recall != to_room->area->recall ){
		// �[�J recall sklv �]�w add at 2020/10/22
        if(get_skill_level(ch, gsn_mark) < 6 && nRecallSklv < 6){
            send_to_char("�A�L�k�ǰe�줣�P���ɪŤ��C\n\r", ch);
            return;
        }
    }
	
   //  sklv = 0 , �����϶ǰe add at 2020/10/22
    if( ch->in_room->area != to_room->area ){
		if(get_skill_level(ch, gsn_mark) < 1 && nRecallSklv < 1){
			send_to_char("�A�L�k�ǰe�줣�P�ϰ�C\n\r", ch);
			return;
		}
    }

   //  in different area add at 18/8/8
    if( IS_SET(to_room->room_flags, ROOM_NO_RECALL_IN)
		&& ch->in_room->area != to_room->area )
	{
        send_to_char("�@�ѤO�����קA�ǰe�쨺�̡C\n\r", ch);
        return;
    }

	// �ק� room_is_private �N�J�� 2022/12/20
    //if( room_is_private( to_room ) )
	if( room_is_private( ch , to_room ) )
	{
		//send_to_char( "[33m���̲{�b�w�g�Q�����F�C[m\n\r", ch );
		send_to_char( "[33m���̪��Ŷ��j�p�A�e�Ǥ��U�A����n�C[m\n\r", ch );
		return;
	}

   //  area immhall can't mark/recall add at 18/8/8
    if( ch->in_room->vnum >= 11001 && ch->in_room->vnum <= 11099 ) {
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}

	// ���� mark �� qmark �ܬ����s�b�� room �y��crash
	if(to_room == NULL){
		send_to_char("�A���ѤF�C\n\r" , ch );
		return;
	}

    ch->mana += nManaRegain;

    if( ch->in_room->progtypes & ROOM_LEAVE)
        rprog_leave_trigger( ch->in_room, ch, 8 );
    if ( ch->fighting )
        stop_fighting( ch, TRUE );
    act( "$n �����v�v������.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch);
    char_to_room( ch, to_room);
    act( "$n �����v�v���X�{.",   ch, NULL, NULL, TO_ROOM );
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
	// �L�kmark���a��
    if( IS_SET(ch->in_room->room_flags, ROOM_NO_MARK) ){
				send_to_char("�A���ѤF�C\n\r", ch);
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
				send_to_char("�A���ѤF�C\n\r", ch);
				return;
	}
    if( ch->in_room->vnum == 4159 ) // ninja trial
        dura = 1;
    dura = UMIN( (dura * ch->level) / 4, (dura * ch->level) / 2);
    if( dura < 1 ) {
        send_to_char("�A���ѤF�C\n\r", ch);
        return;
    }
    send_to_char("�A�����k�O�A�b���a�Ȯɳ]�U�]�O�L�O�C\n\r", ch);
    if( ( qm = is_qmark(ch, "spell_mark" ) )){
        qm->v0 = ch->in_room->vnum;
        qm->timer = dura;
        return;
    }
    
    sprintf( buf, "self spell_mark %d %d 0 0 0 0" , dura, ch->in_room->vnum);
    do_add_qmark( ch, buf );
}



