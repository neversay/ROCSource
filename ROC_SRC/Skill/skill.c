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
//*  PROGRAM ID  : skill.c                                        *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940606 jye                                    *
//*  PURPOSE     :         circular chop �b���P�ն��|             *
//*              :         �L�k�����ؼЪ�bug�ץ�                  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         �ק�circular chop �B��autoc stop��     *
//*              :             ����move                           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940616 Razgriz                                *
//*  PURPOSE     :         �ק�frenzy, meteoric arrow             *
//*              :         �B��autoc stop��, ����hp��move         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 jye                                    *
//*  PURPOSE     :         �ק�do_charge �bSave Room����԰�      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940714 Razgriz                                *
//*  PURPOSE     :         �ק� do_stare()�A�[�J charm_person ��  *
//*              :         qmark �B�z                             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940908 Razgriz                                *
//*  PURPOSE     :         �ק� do_combat_howl(), �b�S�X�f���ж�  *
//*              :         �N���|�I�s do_flee()                   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941016 Razgriz                                *
//*  PURPOSE     :         do_backstab()                          *
//*              :         �ק� bs �԰��� mob ���P�_              *
//*              :         �s�W �԰��� bs mob ���P�_              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991229 tale                                *
//*  PURPOSE     :         do_frenzy                          *
//*              :         ����npc �� frenzy �q��             *
//*              :         �q�ƤW���ꦺ�קK�] memory �����y�� crash             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050822 jye                                    *
//*  PURPOSE     :         �ק� do_frenzy ()                      *
//*              :         ����npc �� frenzy �q�ƤU��(26, 16, 6)  *
//*****************************************************************
/* $Id: skill.c 3444 2005-10-16 05:29:54Z akira $ */
#include "skill.h"

void check_alignment_eq( CHAR_DATA *ch );
bool psionic_check(CHAR_DATA *ch, int sn );
extern void do_defensive( CHAR_DATA *ch, char *argument );
extern void do_empslash( CHAR_DATA * ch, CHAR_DATA* victim );
extern int get_critical_damage(int dam, CHAR_DATA *ch, CHAR_DATA *victim, 
                               int dexdiff, bool fCritical, int dt, int darkraid);
extern int  artifact( CHAR_DATA *ch);
extern int GetBonusedValue_LanceMaster(CHAR_DATA *ch, int nPerc);

bool CheckSleepCharm(CHAR_DATA *ch, CHAR_DATA *victim); // 2022/12/31

// skill �o�ʪZ�� hp sleep �� charm �����p����԰�, �קK�y��crash 2022/12/31
bool CheckSleepCharm(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if( !victim || ( victim == NULL ) || victim->deleted ) return TRUE;
	if( ch->deleted ) return TRUE;
	if( IS_AFFECTED( victim, AFF_SLEEP ) )
		return TRUE;
	if( IS_AFFECTED( ch, AFF_SLEEP ) ) // ch ���� multi_hit ���i��Q�����y�� sleep
		return TRUE;

	// charm �������ק�, �[�J�����̬O�_�� master 2022/12/31
	if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master == ch )
		return TRUE;
	if( IS_AFFECTED( ch, AFF_CHARM ) && ch->fighting == victim && ch->master == victim ) // ch ���� multi_hit ���i��Q�����y�� charm
		return TRUE;

	return FALSE;
}

void do_morphing(CHAR_DATA *ch, char* argument)
{
    char arg[ MAX_INPUT_LENGTH];
    AFFECT_DATA af;
    int to_race;
    int sklv = get_skill_level(ch, gsn_morphing);
    QMARK *q;

    if ( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
	|| ( !IS_NPC( ch )
	    && get_skill_percent(ch, gsn_morphing) < number_percent( ) ) )
    {
	send_to_char(
	    "�A���ѤF�C\n\r", ch);
	return;
    }

    if(!IS_NPC(ch) ) {
	if( ch-> move < 200 - sklv * 10) {
	    send_to_char("�A�����ʤO�����C\n\r", ch);
	    return;
	}
	ch->move -= 200 - sklv * 10;
    }
    one_argument(argument, arg);

    WAIT_STATE(ch, skill_table[gsn_morphing].beats );
    // do_fremove(ch, "all");
    if( arg[0] == '\0' ) {
		send_to_char("�A�Ѱ��F�ۤv�����e�C\n\r", ch);
		affect_strip(ch, gsn_morphing);
		ch->temp_race = '\0'; //NULL;
		dm_effect( ch );
		return;	
    }
    else if ( !str_cmp(arg, "drow")
    || !str_cmp(arg, "gnome" )
    || !str_cmp(arg, "hobbit" )
    || !str_cmp(arg, "elf" )
    || !str_cmp(arg, "fairy" )
    || !str_cmp(arg, "human" )
    || !str_cmp(arg, "dwarf" )
    ){
		affect_strip(ch, gsn_morphing);
        if( ( to_race = race_lookup( arg ) ) < 0 )
		{
            send_to_char("�S���o�ӺرڡC\n\r", ch);
            return;
        }
		af.type	     = gsn_morphing;
		af.duration  = 10 + (ch->level / 2 ) + get_skill_level( ch, gsn_morphing ) * 10 + get_skill_percent( ch, gsn_morphing ) / 5;
		//af.location  = APPLY_RACE;
		//af.modifier  = to_race - ch->race;
		//af.bitvector = AFF_POLYMORPH;
		af.location  = 0;
		af.modifier = 0;
		af.bitvector = 0;
		affect_to_char( ch, &af );
		ch->temp_race = to_race + 1;
		act("�A���e��$T�C", ch, NULL, race_table[ to_race ]. cname, TO_CHAR); 
		act("$n���e��$T�C", ch, NULL, race_table[ to_race ]. cname, TO_ROOM); 
		dm_effect( ch );
		return;
    }
    else {
        char qm[MAX_INPUT_LENGTH];
        sprintf(qm, "skill_morph_%s", arg);
        if( (q = is_qmark(ch, qm)) && sklv >= q->v0){
            affect_strip(ch, gsn_morphing);
            to_race = race_lookup( arg );
            af.type	     = gsn_morphing;
            af.duration  = 1 + (ch->level / 7 );
            //af.location  = APPLY_RACE;
            //af.modifier  = to_race - ch->race;
            //af.bitvector = AFF_POLYMORPH;
			af.location  = 0;
			af.modifier = 0;
			af.bitvector = 0;
            affect_to_char( ch, &af );
			ch->temp_race = to_race + 1;
            act("�A���e��$T�C", ch, NULL, race_table[ to_race ]. cname, TO_CHAR);
            act("$n���e��$T�C", ch, NULL, race_table[ to_race ]. cname, TO_ROOM); 
			dm_effect( ch );
        }
    }
    send_to_char("�A�L�k���e���o�ӺرڡC\n\r", ch);
    return;

    
}

// �]�u���Y 2022/04/29
void do_ammo_casting( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
	int times = obj->value[0];
	int EnemyCount = 0;
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rand_vch;

	if( times <= 0 )
		times = 1;

	if( obj->value[3] <= 0 ){
		obj_from_char(obj);
		extract_obj(obj);
		return;
	}

	// �Ĥ@�o�w��Q��u���ؼ�
	if( victim && !victim->deleted ){
		if ( skill_table[obj->value[3]].target == TAR_CHAR_SELF && victim != ch ){
			obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
		}else if( skill_table[obj->value[3]].target == TAR_IGNORE ){
			if (   obj->value[3] == 314
				|| obj->value[3] == 315
				|| obj->value[3] == 316
				|| obj->value[3] == 317
				|| obj->value[3] == 318
				|| obj->value[3] == 319
				|| obj->value[3] == 320
				|| obj->value[3] == 321 )
				obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
			else
				obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
		}else if( skill_table[obj->value[3]].target == TAR_ARG ){
			obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
		}else{
			obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, victim, obj );
		}
	}
	// �S�ǹL�]��M�a�����⤣�|Ĳ�o�᭱�o��
	if( !IS_NPC(ch) && get_skill_percent(ch, gsn_artifacts_mastery) < 1 ){
		obj_from_char(obj);
		extract_obj(obj);
		return;
	}
	// �]�u�o�Ƥp�� 1 �����p�U�u��X�@�o
	times--;
	if( times <= 0 ){
		obj_from_char(obj);
		extract_obj(obj);
		return;
	}

	// �H���� room �����ؼЩ� spell ����o�Ƭ� 0
	while( times-- )
	{
		EnemyCount = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people; vch; vch = vch_next ){
			vch_next = vch->next_in_room;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) )
				continue;

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_vch = vch;
			EnemyCount++;
		}

		if( !rand_vch ) break;

		if ( skill_table[obj->value[3]].target == TAR_CHAR_SELF && rand_vch != ch ){
			obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
		}else if( skill_table[obj->value[3]].target == TAR_IGNORE ){
			if (   obj->value[3] == 314
				|| obj->value[3] == 315
				|| obj->value[3] == 316
				|| obj->value[3] == 317
				|| obj->value[3] == 318
				|| obj->value[3] == 319
				|| obj->value[3] == 320
				|| obj->value[3] == 321 )
				obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
			else
				obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
		}else if( skill_table[obj->value[3]].target == TAR_ARG ){
			obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, ch, obj );
		}else{
			obj_cast_spell( obj->value[3], (obj->value[2] * 2)/3, ch, rand_vch, obj );
		}
	}

	obj_from_char(obj);
	extract_obj(obj);
	return;
}

void do_coin_dart( CHAR_DATA *ch, char *argument )
{
    int amount;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int nCoinSklv = get_skill_level(ch,gsn_dart_mastery);
    int nCurrDex  = get_curr_dex(ch);
    int nCurrInt  = get_curr_int(ch);
    int nCurrStr  = get_curr_str(ch);
    int nEDmgSklv = get_skill_level(ch, gsn_enhanced_damage);
    int nEDmgPerc = get_skill_percent(ch, gsn_enhanced_damage);
    int nHitRoll  = get_hitroll(ch, (get_eq_char(ch, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) );
    int nHitRate  = 0;
    int nDamage   = 0;
    int nCtlDmg   = 0;

    one_argument( argument, arg);

    if( get_skill_percent(ch, gsn_dart_mastery) * (10 + nCoinSklv) * nCurrDex * nCurrInt / 23 / 23 / 10 <= 
        number_percent()  ) {
        send_to_char("�A���ѤF�C\n\r", ch);
        return;
    }
    if( arg[0] == '\0' ) {
        if( !(victim = ch->fighting ) ) {
            send_to_char("�A�n��֡H\n\r", ch);
            return;
        }
    }else if ( !(victim = get_char_room( ch, arg)) ) {
        send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
        return;
    }

    if( ch == victim ) {
        send_to_char("�A����q�k���쥪��C\n\r", ch);
        return;
    }
    if( is_same_group(ch, victim) ) {
        send_to_char("�� give ���|����n�C\n\r", ch);
        return;
    }
    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
        return;
    }

    if ( ch->move < nCoinSklv * 10 + 15) {
        send_to_char("�A�Ӳ֤F.\n\r", ch);
        return;
    }

    amount = (30 - nCurrInt) * 200 + nCoinSklv * 500;
    if( !IS_NPC(ch) && ch->gold < amount ) {
        send_to_char("�A���������F�A�U���h�a�I�a�C\n\r", ch);
        return;
    }

    ch->move -=  nCoinSklv * 10 + 15;
    ch->gold -=  amount;
    if(ch->gold < 0 && IS_NPC(ch)) ch->gold = 0;

	nDamage   =  100 + nCoinSklv * (nCoinSklv + 14);
	nDamage   =  number_range(nDamage * 2, nDamage * 3);

    if(ch->apdr > 0) nDamage +=  ch->apdr * ((nCurrStr - 10) * 2 + nCoinSklv * (nCoinSklv + 4)) / 12;

    nDamage   =  nDamage  * ( nEDmgPerc + 125 ) / 200;
    nDamage   =  nDamage  * ( 100 + nEDmgSklv * (nEDmgSklv + 4) / 3) / 100;
    nCtlDmg   =  get_critical_damage(nDamage, ch, victim, nCurrDex - get_curr_dex(victim), 0, 0, 0);
    if(nDamage != nCtlDmg) 
        nDamage = nCtlDmg;
    else if(number_percent() < nCoinSklv * 4 + 16)
        nDamage = get_critical_damage(nDamage, ch, victim, nCurrDex - get_curr_dex(victim), 0, 0, 0);
    else
        nDamage = nDamage;
    nDamage  -= (victim->pdr + get_curr_con(victim) * 2) * 5;
    if(!IS_NPC(victim)) nDamage  -= (victim->pdr + get_curr_con(victim) * 2) * 5;
    if(nDamage < 0) nDamage = 0;

    nHitRate  = (nHitRoll + 25) * (100 + nCoinSklv * (nCoinSklv + 4)) / 100;

    if(IS_NPC(victim)){
        if(GET_AC(victim) < 0)
            nDamage = nDamage * 5000 / (5000 - GET_DODGE(victim));
        else
            nDamage = nDamage * (5000 + GET_DODGE(victim)) / 5000;
		
        if(GET_DODGE(victim) + 400 < 0)
            nHitRate = nHitRate * 1000 / (1000 - (GET_DODGE(victim) + 400));
        else
            nHitRate = nHitRate * (1000 + (GET_DODGE(victim) + 400)) / 1000;
    }else{
        if(GET_AC(victim) < 0)
            nDamage = nDamage * 2500 / (2500 - GET_DODGE(victim));
        else
            nDamage = nDamage * (2500 + GET_DODGE(victim)) / 2500;
		
        if(GET_DODGE(victim) < 0)
            nHitRate = nHitRate * 1000 / (1000 - GET_DODGE(victim));
        else
            nHitRate = nHitRate * (1000 + GET_DODGE(victim)) / 1000;
		}

	// weakness_damage �]�w add at 2020/09/18
	nDamage = nDamage * weakness_damage(victim , gsn_dart_mastery) / 100;

	// weakness_percent �]�w add at 2020/09/18
    if( nHitRate * weakness_percent(victim , gsn_dart_mastery) / 100 > number_percent() ){
        damage( ch, victim, nDamage, gsn_dart_mastery, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
    }else{
        dam_message( ch, victim, -1, gsn_dart_mastery, TYPE_UNDEFINED);
        set_fighting(victim, ch);
    }

	WAIT_STATE(ch, skill_table[gsn_dart_mastery].beats);
	if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
		stop_fighting(victim, FALSE);
}



void do_trueshot( CHAR_DATA *ch, char *argument )
{

    AFFECT_DATA af;
    int sklv = get_skill_level(ch, gsn_shot_arrow);

    if( is_affected(ch, gsn_trueshot)) return;
    if( ch->move < 100 ){
        send_to_char("�A����O�����F�C\n\r", ch);
        return;
    }

    ch->move -= 100;
    af.type      = gsn_trueshot;
    af.duration  = 1 + (ch->level / 4 );
    af.location  = APPLY_HITROLL;
    af.modifier  = (sklv > 5 ? 25 : 11);
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char("�ǥѦ۵M���O�A�A�o��F���Ǫ���O�C\n\r",ch);
}

void do_blood_thirsty( CHAR_DATA *ch, char *argument )
{

	AFFECT_DATA af;
	int sklv = get_skill_level(ch, gsn_blood_thirsty);
	char buf  [MAX_INPUT_LENGTH];

	if( affected_value(ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) ) return;

	sprintf( buf, "self skill_bloodthirsty_mark %d 0 0 0 0 1", 12 - sklv );
	do_add_qmark( ch, buf );

	// pd �[������e�԰����ˮ`�Ȳ֭p�v�T 2022/02/20
	af.type      = gsn_blood_thirsty;
	af.duration  = 3;
	af.location  = APPLY_ADAMAGE;
	if(affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT) >= 1)
		af.modifier  = 10 + sklv * sklv + affected_value(ch, gsn_blood_thirsty, APPLY_DAMAGE_COUNT)/30;
	else
		af.modifier  = 10 + sklv * sklv;
	af.bitvector = AFF_NO_HEAL;
	affect_to_char( ch, &af );

	// �W�[ blood thirsty 5 round �����P�w add at 2020/10/16
	af.type      = gsn_blood_thirsty;
	af.duration  = 3;
	af.location  = APPLY_SAVING_LIFE;
	af.modifier  = 3 + sklv/2;
	af.bitvector = AFF_NO_HEAL;
	affect_to_char( ch, &af );

	send_to_char("�A�˱�ͪ�����A�ƨ����������ݦ���~�u���������e���ĤH�I\n\r",ch);
	act( "$n �H�ͩR����`�A�u�D���겴�e���ĤH�I", ch, NULL, NULL, TO_ROOM );
}

void do_reg_adore( CHAR_DATA *ch, char *argument )
{

    AFFECT_DATA af;
    int sklv = get_skill_level(ch, gsn_adore);
	int hit_count = 0;
	char buf  [MAX_INPUT_LENGTH];
	
	if(!is_affected( ch, gsn_adore )){
		send_to_char("�A���������S�����󴺥����ؼСC\n\r", ch);
		return;
	}
	if(affected_value(ch, gsn_adore, APPLY_DAMROLL) <= 0){
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}
    if( is_qmark(ch, "skill_reg_adore" )){
        send_to_char( "�A�������٨S�q�W������}����_�C\n\r", ch );
        return;
    }
    sprintf( buf, "self skill_reg_adore %d 0 0 0 0 1", 6 - sklv/3 );
    do_add_qmark( ch, buf );

	hit_count = 20 + ( affected_value(ch, gsn_adore, APPLY_DAMROLL) * (10 + sklv) ) / 10;

	affect_strip( ch, gsn_adore);

	if( is_affected( ch, gsn_regeneracy) )
		affect_strip( ch, gsn_regeneracy);

    af.type      = gsn_regeneracy;
    af.duration  = sklv/2;
    af.location  = APPLY_HIT;
    af.modifier  = hit_count;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char(HIP"�A�W�V�ߤ��������ؼСA��}�ͩR�������C"NOR"\n\r",ch);
	ch->hit = UMIN( ch->max_hit , ch->hit + hit_count );
}

void do_powerbolt( CHAR_DATA *ch, char *argument )
{

    AFFECT_DATA af;
    
    int nShotSklv     = get_skill_level(ch, gsn_shot_arrow);
    int nArrowSklv    = get_skill_level(ch, gsn_magic_arrow);

    if( is_affected(ch, gsn_powerbolt)) return;
    if( ch->move < 150 ){
        send_to_char("�A����O�����F�C\n\r", ch);
        return;
    }

    ch->move -= 150;
    af.type      = gsn_powerbolt;
    af.duration  = 1 + (ch->level / 4 );
    af.location  = APPLY_ADAMAGE;
    af.modifier  = nShotSklv * 3 + (nShotSklv / 3) * 5 + (nShotSklv / 5) * 5;
    if(nArrowSklv == 6) af.modifier += nShotSklv * 10;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char("�ǥѦ۵M���O�A�A�N�B�~���O�q�̪��b�b���W�C\n\r",ch);
}

void do_perform( CHAR_DATA *ch, char *argument )
{

    char arg[ MAX_INPUT_LENGTH];
    CHAR_DATA *victim = NULL;
    int sn = 0;

    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
        return;
    }

    one_argument(argument, arg);
    if( arg[0] == '\0' ) {
        send_to_char("�n�ϥΤ���ޥ��H\n\r", ch);
        return;
    }
    if( !str_prefix(arg, "empslash") ) {
	if ( !ch->fighting )
	{
	    send_to_char( "�A���O�b�԰����C\n\r", ch );
	    return;	
	}
	argument = one_argument(argument, arg);
	one_argument(argument, arg);
	if( arg[0] == '\0') victim = ch->fighting;
	else if( !(victim = get_char_room(ch,arg))
		|| !can_see(ch, victim)){
	    send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
	    return;
	}
	do_empslash(ch, victim);
	return;
    }
    else if( !str_prefix(arg, "morphing")){
	if( ch->fighting) {
	    send_to_char("�԰���������e�C\n\r", ch);
	    return;
	}
	argument = one_argument(argument, arg);
	do_morphing(ch, argument);
	return;
    }
    else if (!str_prefix( arg, "defensive" ) ){
        QMARK *q = NULL;
        if( ( q = is_qmark( ch, "skill_defensive_mark" ) )){
            if( q->timer > 36 ) {
                send_to_char("�A�ٻݭn�@�ѥb���k���ɶ��~��^�_��O�C\n\r", ch);
            }
            else if( q->timer > 24 ) {
                send_to_char("�A�ٻݭn�@�ѥ��k���ɶ��~��^�_��O�C\n\r", ch);
            }
            else if( q->timer > 12 ) {
                send_to_char("�A�ٻݭn�b�ѥ��k���ɶ��~��^�_��O�C\n\r", ch);
            }
            else
            send_to_char("�A�٨S�q���e���h�֤��^�_�L�ӡC\n\r", ch);
            return;
        }
        if( get_skill_level(ch,gsn_defensive) < 6 && !ch->fighting ) {
            send_to_char("�A���O�b�԰����C\n\r", ch);
            return;
        }
        do_defensive( ch, "");
        return;
    }
    else if( !str_prefix(arg, "coin-dart")){
        /*if( !ch->fighting) {
            send_to_char("�A���O�b�԰����C\n\r", ch);
            return;
        }*/
		argument = one_argument(argument, arg);
        do_coin_dart(ch, argument);
        return;
    }
    else if( !str_prefix(arg, "hasten") ) {
        if( get_skill_percent(ch, gsn_hasten) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
        spell_hasten(gsn_hasten, ch->level,ch,  ch);
        return;
    }
    else if( !str_prefix(arg, "powerup")){
        if( get_skill_percent(ch, gsn_powerup) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
        spell_powerup(gsn_powerup, ch->level,ch, ch);
        return;
    }
    else if( !str_prefix(arg, "general purpose")){
        if( get_skill_percent(ch, skill_lookup("general purpose")) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
        argument = one_argument(argument, arg); 
        argument = one_argument(argument, arg); 
        if( arg[0] == '\0' && !(victim = ch->fighting)){
            send_to_char("�n��֨ϥΡH\n\r", ch);
            return;
        }
        if( !victim && !(victim = get_char_room(ch, arg) ) ){
            send_to_char("�S���o�ӤH�C\n\r", ch);
            return;
        }
        spell_general_purpose(skill_lookup("general purpose"), ch->level, ch, victim);
        return;
    }
    else if( !str_prefix(arg, "lifeshare")){ // add at 2021/11/21
        if( get_skill_percent(ch, gsn_lifeshare) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
        argument = one_argument(argument, arg); 
        argument = one_argument(argument, arg); 
        if( arg[0] == '\0' ){
            send_to_char("�n��֨ϥΡH\n\r", ch);
            return;
        }
        if( !victim && !(victim = get_char_room(ch, arg) ) ){
            send_to_char("�S���o�ӤH�C\n\r", ch);
            return;
        }
        spell_lifeshare(gsn_lifeshare, ch->level, ch, victim);
        return;
    }
    else if( !str_prefix(arg, "assistance")){
    
        if( !ch->group ) 
        {
            send_to_char("�A�èS���b����C\n\r", ch);
            return;
        }

        argument = one_argument(argument, arg); 
        argument = one_argument(argument, arg); 
        
        if( arg[0] == '\0' )
            victim = ch;
        else if(( !victim && !(victim = get_char_room(ch, arg) ) )||( victim == NULL)){
            send_to_char("�S���o�ӤH�C\n\r", ch);
            return;
        }
        do_lead_assistance(ch, victim);
        return;
    }
    else if( !str_prefix(arg, "assault")){
    	do_lead_assault(ch, NULL);
	return;
    }
    else if( !str_prefix(arg, "ucharge")){
    	do_lead_ucharge(ch, NULL);
	return;
    }
    else if( !str_prefix(arg, "ufire")){
	do_lead_ufire(ch, NULL);
	return;
    }
    else if( !str_prefix(arg, "hidemarch")){
        do_lead_hidemarch(ch, NULL);
        return;
    }
    else if( !str_prefix(arg, "envelopment")){
        do_lead_envelopment(ch, NULL);
        return;
    }
    else if ( !str_prefix(arg, "trueshot")){
        if( ch->class != RANGER
        ||  get_skill_percent(ch, gsn_shot_arrow) < 1
        ||  get_skill_level(ch, gsn_shot_arrow) < 5 ){
            send_to_char("�A�b�b�N�W���y���٤����C\n\r", ch);
            return;
        }
        do_trueshot(ch, argument);
        return;
    }
    else if( !str_prefix(arg, "powerbolt")){
        if( ch->class != RANGER
        ||  get_skill_percent(ch, gsn_shot_arrow) < 1
        ||  get_skill_level(ch, gsn_shot_arrow) < 3 ){
            send_to_char("�A�b�b�N�W���y���٤����C\n\r", ch);
            return;
        }
        do_powerbolt(ch, argument);
        return;
    }
    else if( !str_prefix(arg, "demon tremor") 
            && (sn = skill_lookup("demon tremor")) > 0)
    {
	if(psionic_check(ch, sn))
            spell_demontremor(sn, ch->level, ch, NULL);
	
        WAIT_STATE(ch, skill_table[sn].beats*2);
        return;
    }
    else if( !str_prefix(arg, "glacier strike") 
            && (sn = skill_lookup("glacier strike")) > 0)
    {
	if(psionic_check(ch, sn))
            spell_glacier_strike(sn, ch->level, ch, NULL);
	
        WAIT_STATE(ch, skill_table[sn].beats*2);
        return;
    }
    else if( !str_prefix(arg, "green breath") 
            && (sn = skill_lookup("green breath")) > 0)
    {
	if(psionic_check(ch, sn))
            spell_green_breath(sn, ch->level, ch, NULL);
	
        WAIT_STATE(ch, skill_table[sn].beats*2);
        return;
    }
    else if( !str_prefix(arg, "sand tide") 
            && (sn = skill_lookup("sand tide")) > 0)
    {
	if(psionic_check(ch, sn))
            spell_sand_tide(sn, ch->level, ch, NULL);
	
        WAIT_STATE(ch, skill_table[sn].beats*2);
        return;
    }
    else if( !str_prefix(arg, "banshee wail") && (sn = skill_lookup("banshee wail")) > 0){
	if(psionic_check(ch, sn))spell_banshee_wail(sn, ch->level, ch, NULL);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
    else if( !str_prefix(arg, "succubus kiss") && (sn = skill_lookup("succubus kiss")) > 0){
	if(psionic_check(ch, sn))spell_succubus_kiss(sn, ch->level, ch, NULL);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
    else if( !str_prefix(arg, "mental empower") && (sn = skill_lookup("mental empower")) > 0){
	if(psionic_check(ch, sn))spell_mental_empower(sn, ch->level, ch, ch);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
    else if( !str_prefix(arg, "will fortress") && (sn = skill_lookup("will fortress")) > 0){
	if(psionic_check(ch, sn))spell_will_fortress(sn, ch->level, ch, ch);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
    else if( !str_prefix(arg, "ghosts celebration") && (sn = skill_lookup("ghosts celebration")) > 0){
	if(psionic_check(ch, sn))spell_ghosts_celebration(sn, ch->level, ch, ch);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
    else if( !str_prefix(arg, "disenchant") && (sn = skill_lookup("disenchant")) > 0){
	if(psionic_check(ch, sn))spell_disenchant(sn, ch->level, ch, NULL);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
    else if( !str_prefix(arg, "mind blast") && (sn = skill_lookup("mind blast")) > 0){
	if(psionic_check(ch, sn))spell_mind_blast(sn, ch->level, ch, NULL);
	WAIT_STATE(ch, skill_table[sn].beats*2);return;
    }
	// add perform adore 18/7/17
	// �����ˮ`�ĪG, ���������ͩR�ƭȱj�� 2020/10/16
    else if( !str_prefix(arg, "adore")){
        if( get_skill_percent(ch, gsn_adore) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
		do_reg_adore(ch, argument);
		return;
    }
	// add perform blood thirsty 18/7/17
    else if( !str_prefix(arg, "blood thirsty")){
		if( is_qmark(ch, "skill_bloodthirsty_mark" )){
			send_to_char( "�A�������٨S�q�W�����z�o����_�C\n\r", ch );
			return;
		}
		if ( !ch->fighting )
		{
			send_to_char( "�A���O�b�԰����C\n\r", ch );
			return;	
		}
        if( get_skill_percent(ch, gsn_blood_thirsty) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
		do_blood_thirsty(ch, argument);
		return;
    }

    send_to_char("�S���o���ޥ��C\n\r", ch);
    return;
}


/*Check on the complete condition of psionic spell.*/
bool psionic_check( CHAR_DATA *ch, int sn){
    CHAR_DATA * gch = NULL;
    int percent = get_skill_percent(ch, sn), mana = MANA_COST( ch, sn );
    char buf[BUF_STRING_LENGTH];
    if(!IS_NPC(ch)){
	if(percent == 0 )
	{
	    send_to_char( "�A�٨S�Ƿ|�o�Ӫk�N.\n\r", ch );
	    return FALSE;
	}
	if((ch->max_mana + ( ch->max_mana / 5 ) < mana)){
	    send_to_char( "�A���k�O�����F.\n\r", ch );
	    return FALSE;
	}
	if(percent < number_percent()){
	    send_to_char("�A���ѤF�I\n\r", ch);
	    ch->mana -= (mana>>1);
	    ch->move -= mana;
	    return FALSE;
	}
	if(ch->position < skill_table[sn].minimum_position){
	    if(ch->fighting)
		for(gch = ch->in_room->people;gch;gch = gch->next_in_room){
		    if(gch->fighting == ch)
			if (( ch->pcdata->autocombat == AUTOC_CONCENTRATION &&
				get_skill_percent(ch, gsn_concentration ) + get_skill_level(ch, gsn_concentration) * 2 < number_percent() / 2 + 5 ) ||
				( ch->pcdata->autocombat != AUTOC_CONCENTRATION && get_skill_percent(ch, gsn_concentration) + get_skill_level(ch, gsn_concentration) * 3< number_percent() + 30 ))
			{
				send_to_char( "�A�L�k�M��.\n\r", ch );
				return FALSE;
			}
		}
	    else{
		send_to_char("�A�L�O�I�k�I\n\r", ch);
		return FALSE;
	    }
	}
    }
    sprintf(buf, "�A�o�X���F���O�q�A�I��"HIW"%s!"NOR"\n\r", skill_table[sn].cname);
    send_to_char(buf, ch);
    if( IS_AFFECTED( ch, AFF_SEAL_MAGIC ) && ! IS_IMMORTAL(ch) ) {
	send_to_char("�A�P�򪺤O�������F�A���]�k�I\n\r", ch);
	return FALSE;
    }
    if( ch->in_room && IS_SET(ch->in_room->room_flags, ROOM_SAFE)){
	send_to_char("����.\n\r", ch);
	return FALSE;
    }

    if (ch->mana < mana || ch->move < mana) {
      send_to_char ( "�A�Ӳ֤F.\n\r" , ch);
      return FALSE;
    }
    ch->mana -= mana;
    ch->move -= mana;
    return TRUE;
}

// �D�� blood thirsty �P�w 2021/12/25
bool isBloodThirsty( CHAR_DATA *ch ){

	if( IS_NPC(ch) ) return FALSE;
	if( ch->class != MARTIALMAN ) return FALSE;
	if( get_skill_percent(ch, gsn_blood_thirsty) <= 0 ) return FALSE;
	if( is_affected( ch, gsn_blood_thirsty)
		&& affected_value(ch, gsn_blood_thirsty, APPLY_SAVING_LIFE) > 0 )
		return TRUE;

	return FALSE;
}

//  lizardman �ϥΧޯ�ɧP�w�ѥͧ� 2022/02/23
void LizardmanSkill( CHAR_DATA *ch , CHAR_DATA *victim , int multi){

	if( IS_NPC( ch ) ) return;

	if( !IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) ) return;
	
	if( victim->deleted ) return;

	int nSklv = get_skill_level(ch, gsn_inborn_ability);

	// ��������
	CHAR_DATA *vch      = NULL;
	CHAR_DATA *vch_next = NULL;
	int nTailAttackRate = 16 + nSklv * (nSklv + 3);
	int nTripRate       = get_curr_dex(ch) / 2 + nSklv * 3;
	if( number_percent() < nTailAttackRate )
	{
		if( mone_hit ( ch, victim, &gsn_tactics, -2, MSG_NORMAL, DT_NOCHANGE ) >= 0
			&& !victim->deleted && number_percent() < nTripRate )
		{
			trip(ch, victim);
		}
	}
	if(nSklv > 3 && multi == 0 )
	{
		for( vch = ch->in_room->people; vch; vch = vch_next){
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( vch->fighting != ch ) continue;
			if( vch == victim) continue;
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
			if( number_percent() < (nTailAttackRate / 3) ){
				if( mone_hit ( ch, vch, &gsn_tactics, -2, MSG_NORMAL, DT_NOCHANGE ) >= 0
					&& !vch->deleted && number_percent() < nTripRate )
				{
					trip(ch, vch);
				}
			}
		}
	}

	if( victim->deleted ) return;

	//  �Q��
	if( ch->pcdata->condition[COND_DRUNK] > 10 
		&& number_percent() < UMAX(40, ch->pcdata->condition[COND_DRUNK] )/4 + nSklv * nSklv / 3)
	{
		AFFECT_DATA af;
		int nSVE        = NEW_GET_SVS(victim, MAGIC_EARTH);
		int nPoisonRate = 0; if(nSklv > 3) nPoisonRate =   5 * (nSklv - 3);
		int nBlindRate  = 0; if(nSklv > 5) nBlindRate  =   7;
		int nDamage     = dice( 4 + nSklv, ch->level + nSklv) + ch->amdr * nSklv / 5;

		nPoisonRate = nPoisonRate * (100 + nSVE) / 100; if(nPoisonRate < 0) nPoisonRate = 0;
		nBlindRate  = nBlindRate  * (100 + nSVE) / 100; if(nBlindRate  < 0) nBlindRate  = 0;
		nDamage     = nDamage     * (100 + nSVE) / 100; 
		nDamage    -= victim->mdr;
		if(nDamage  < 0) nDamage     = 0;

		nPoisonRate = nPoisonRate *  weakness_percent(victim, gsn_poison) / 100;
		nBlindRate  = nBlindRate  *  weakness_percent(victim, gsn_blindness) / 100;

		if(number_percent() < nPoisonRate && !IS_SET(victim->special, NO_POISON))
		{
			af.type      = gsn_poison;
			af.duration  = UMAX( 0, 3 + nSklv - get_curr_con(victim) / 5);
			af.bitvector = AFF_POISON;
			if( affected_value(victim, gsn_poison, APPLY_CON) > -8){
				af.location = APPLY_CON;
				af.modifier = -2 * weakness_damage( victim, gsn_poison ) * 0.01;
				affect_join( victim, &af );
			}

			if( affected_value(victim, gsn_poison, APPLY_SAVING_POISON) < (14 + nSklv * 6) * 10){
				af.location = APPLY_SAVING_POISON;
				af.modifier = ((14 + nSklv * 6) * 25 / get_curr_con(victim)) 
								* weakness_damage( victim, gsn_poison ) * 0.01;
				affect_join( victim, &af );
			}
			act( "$N"HIG"�y�Ⲥ���x�C�A�r���w�g�I�J�F�L���餺�I"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIG "���߷P�P��h�P�P��ŧ�ӡA�Aı�o�۷�a�����I"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIG"������Ÿ�ݤF�@�U�A���ܱo���ǭW�ߡI"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
		else if(number_percent() < nBlindRate && !IS_AFFECTED( victim, AFF_BLIND )
				&& !IS_SET( victim->special, NO_BLIND )
				&& !(is_affected( victim, gsn_invoke )
				&& get_skill_level(victim, gsn_invoke) > 2) )
		{
			af.type      = gsn_blindness;
			af.duration  = 0;
			af.location  = APPLY_HITROLL;
			af.modifier  = (-10) * weakness_damage( victim, gsn_blindness ) * 0.01;
			af.bitvector = AFF_BLIND;
			affect_to_char( victim, &af );

			act( "�A"HIG"���ĲG�˨�F$N"HIG"�������I�ϱo�L�����F�I"NOR"", ch, NULL, victim, TO_CHAR );
			act( "$n"HIG"���ĲG�˨�F�A"HIG"�������I�A���򳣬ݤ���F�I"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$n"HIG"���ĲG�˨�F$N"HIG"�������I�ϱo�L�����F�I"NOR"", ch, NULL, victim, TO_NOTVICT );
		}

		damage( ch, victim, nDamage, gsn_poison_weapon, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
	}

	return;
}


/*
 * I'm only allowing backstabbing with the primary weapon...immortals
 * who wield two weapons, with the first not being a dagger, will be
 * unable to backstab or circle.  Tough cookie.	 --- Thelonius
 */
void do_backstab( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	CHAR_DATA *victim; //, *gch;
	int stun = 0;
	int dmg = 0;
	int chance = 0;
	int bs_sklv = 0; //Added by Razgriz 20051016
	bool can_bs_fighting_mob= FALSE;
	bool fighting_bs = FALSE; //Added by Razgriz 20060717
	bool isdual = FALSE;  // dual dagger check add at 2020/09/17
	char arg [ MAX_INPUT_LENGTH ];

	if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		return;

	if ( !IS_NPC( ch )
	&& ch->pcdata->learned[gsn_backstab] == 0 )
	{
		send_to_char( "����H\n\r", ch );
		return;
	}
	if( !IS_NPC(ch) )
	{
		if(ch->pcdata->autocombat == AUTOC_STOP) return;
		if(ch->pcdata->autocombat == AUTOC_DODGE) return;
		if(ch->pcdata->autocombat == AUTOC_PARRY) return;
		if(ch->pcdata->autocombat == AUTOC_CONCENTRATION) return;
		if(ch->pcdata->autocombat == AUTOC_INSTANT) return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "�I���?\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		return;
	}

	if ( victim == ch )
	{
		send_to_char( "�A�뤣��ۤv�I�᪺�n�`.\n\r", ch );
		return;
	}

	if ( is_safe( ch, victim ) )
		return;

	if ( !( obj = get_eq_char( ch, WEAR_WIELD ) )){
		if( !(obj = get_eq_char( ch, WEAR_TWO_HAND) )){
			send_to_char( "�D��Z�������˳ƤP�����Z��.\n\r", ch );
			return;
		}
	}
	if(!IS_NPC(ch) && obj->value[3] != 11 ){
		send_to_char( "�D��Z�������˳ƤP�����Z��.\n\r", ch );
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	/* ��� bs �԰��� mob ���P�_, Mark by Razgriz 20051016
		if(!IS_NPC(ch) && !ch->fighting && ch->leader && ch->leader->class == LORD && 
			get_skill_percent(ch->leader, skill_lookup("lead"))> number_percent())
			for(gch=ch->in_room->people;gch;gch=gch->next_in_room)
			if(gch == ch->leader){
			can_bs_fighting_mob = TRUE;
			break;
			}

		if ( !IS_NPC(ch) && IS_NPC(victim) && victim->fighting )
		{
			if(IS_AFFECTED( ch, AFF_HIDE ) && (ch->class==ASSASSIN || can_bs_fighting_mob))
				send_to_char( "�A�����N�N�a�q�t�B�I��I\n\r", ch);
			else{
			send_to_char( "�A���ఽŧ�԰������ؼ�.\n\r", ch );
			return;
		}
		}*/

    //��� bs �԰��� mob ���P�_, Added by Razgriz 20051016
	bs_sklv = get_skill_level ( ch, gsn_backstab );

	/*  �קאּlv 4 �N�i�����԰������ؼ�
	if (    ( bs_sklv == 4 && IS_AFFECTED ( ch, AFF_HIDE ) && IS_AFFECTED ( ch, AFF_SNEAK ) ) 
		 || ( bs_sklv == 5 && IS_AFFECTED ( ch, AFF_HIDE ) )
		 || ( bs_sklv == 6 )    )*/

	if ( bs_sklv >= 4 )
	{
		can_bs_fighting_mob = TRUE;
	}
	if ( !IS_NPC(ch) && !ch->fighting && IS_NPC(victim) && victim->fighting )
	{
		if (    can_bs_fighting_mob
			 && ( ch->class == ASSASSIN || ch->class == NINJA || ch->class == RANGER )    )
		{
			send_to_char ( "�A�����N�N�a�q�t�B�I��I\n\r", ch );
		}
		else
		{
			send_to_char ( "�A�x������ĤH���`���A��ŧ����԰������ؼ�.\n\r" , ch );
			return;
		}
	}
	//�԰����A�� bs mob ���P�_, Added by Razgriz 20051016
	if ( !IS_NPC (ch) && ch->fighting && IS_NPC (victim) )
	{
		// �԰� bs �P�w�[�J tricky pacing ���]�w
		if ( (get_skill_level ( ch, gsn_ass ) + get_skill_level ( ch, gsn_tricky_pacing )) < 4 ){
			send_to_char ( "�A�x������ĤH���`���A��ŧ����԰������ؼ�.\n\r" , ch );
			return;
		}
		/* �����]�w���������� class �]��԰������� bs
		if ( ch->pcdata->prime_attr != APPLY_STR && ch->pcdata->prime_attr != APPLY_CON )
			return;
		*/
		if ( victim->fighting && !can_bs_fighting_mob )
		{
			send_to_char ( "�A�x������ĤH���`���A��ŧ����԰������ؼ�.\n\r" , ch );
			return;
		}

		/*  �����ݬ��t���ؼЮɤ~�i�԰��� bs
		if ( !ch->assassinating || ch->assassinating != victim )
		{
			send_to_char ( "�L�ä��O�A�n�t�����ؼ�.\n\r" , ch );
			return;
		}*/

		if( time_info.hour > 6 && time_info.hour < 18 )
		{
			if(ch->class == ASSASSIN){
				if(is_affected( ch, gsn_berserk )){
				}else if( get_skill_level ( ch, gsn_ass ) + get_skill_level ( ch, gsn_tricky_pacing ) < 6 ){
					send_to_char ( "�դѵ��u�}�n�A�ĤH��ı��A���N��.\n\r" , ch );
					return;
				}
			}else{
				send_to_char ( "�դѵ��u�}�n�A�ĤH��ı��A���N��.\n\r" , ch );
				return;
			}
		}

		if( ch->move < 300)
		{
			send_to_char ( "�A����O�L�k�����԰��I�몺�ʧ@.\n\r", ch);
			return;
		}
		ch->move -= 300;

		if( is_affected ( ch, gsn_berserk ) )
			send_to_char ( "�A�̵۳��ʪ���ı�A�x����ĤH�������`���A�A�׮i�}�I�����.\n\r" , ch );
		else
			send_to_char ( "�A���a�ۨ�����I��A�A�׮i�}�I�����.\n\r" , ch );

		fighting_bs = TRUE;

	}else if(ch->move > bs_sklv * 20){
		ch->move -= bs_sklv * 20;
	}else{
		send_to_char ( "�A�Ӳ֤F.\n\r" , ch);
	}

	check_killer( ch, victim );

	if ( !IS_AWAKE( victim )
		|| IS_NPC( ch )
		|| IS_AFFECTED( ch, AFF_DARK_RAID )
		|| number_percent( ) < get_skill_percent(ch, gsn_backstab) + bs_sklv * 3)
	{

		if ( IS_NPC( ch ) || can_see(ch, victim) )
		{
			dmg = one_hit( ch, victim, gsn_backstab , WEAR_WIELD);
			if( dmg >= 0 ) 
				stun += 4;
			else if(dmg == -1)
				stun += 2;
			//WAIT_STATE( victim, stun );
			//WAIT_STATE( ch, stun );
		}

		if ( obj = get_eq_char( ch, WEAR_WIELD_2 ) ){
			if( obj->value[3] == 11 ){
				isdual = TRUE;
			}
		}

		if( !IS_NPC(ch) && !victim->deleted && can_see(ch, victim) &&
			( ch->class == NINJA || ch->class == ASSASSIN || ch->class == RANGER || ch->class == THIEF ) )
		{
			chance = get_skill_percent(ch, gsn_backstab) / 10; // 10
			if( ch->pcdata->autocombat == AUTOC_CRITICAL ) chance += get_skill_level(ch, gsn_deadly_strike) * 10;
			if( ch->pcdata->prime_attr == APPLY_DEX ) chance += 15;
			if( ch->pcdata->prime_attr == APPLY_STR ) chance += 10;
			if( ch->pcdata->snd_attr == APPLY_STR ) chance += 8;
			if( ch->pcdata->snd_attr == APPLY_DEX ) chance += 10;
			if( ch->pcdata->snd_attr == APPLY_INT ) chance += 8;
			// �[�J dual �P�w 2020/09/17
			if( isdual ) chance += 5 * get_skill_level(ch, gsn_dual) + get_skill_percent(ch, gsn_dual)/10; // 30 + 10
			chance += get_skill_level(ch, gsn_backstab) * 6; // 36
			if(fighting_bs) chance /= 2;
			if(ch->move < 10 + bs_sklv * 5) chance  = 0;
			if( ( IS_AFFECTED( ch, AFF_DARK_RAID ) || number_percent() < chance * weakness_percent(victim, gsn_backstab) / 100 ) )
			{
				// �[�J dual �Ƥ�Z���P�w 2020/09/17
				if( isdual )
					dmg = one_hit( ch, victim, gsn_backstab , WEAR_WIELD_2);
				else
					dmg = one_hit( ch, victim, gsn_backstab , WEAR_WIELD);
				if( dmg >=  0 ) stun += 2;
				if( dmg == -1 ) stun += 1;
				ch->move -= 10 + bs_sklv * 5;
			}
		}

		// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
		if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
			LizardmanSkill( ch , victim , 0);

		//�[�J �԰���bs ���B�~ stun, Added by Razgriz 20060717
		if ( fighting_bs )
		{
			WAIT_STATE( ch, stun * 3 / 2 );
		}
		else
		{
			WAIT_STATE( victim, stun );
			WAIT_STATE( ch, stun );
		}

		// �w�� artifact �����p, ���Q sleep ��charm ���ؼз|�T�갱�����
		/*if( IS_AFFECTED( victim, AFF_SLEEP )) return;
		if( IS_AFFECTED( ch, AFF_SLEEP )) return;
		if( IS_AFFECTED( victim, AFF_CHARM )) return;
		if( IS_AFFECTED( ch, AFF_CHARM )) return; */
		if( CheckSleepCharm( ch , victim ) ) return; // 2022/12/31

		//bs ���ަ��S����, ���n�}��    Modified by Razgriz  20060620
		//if( !IS_NPC(victim) && !IS_NPC(ch) )
		//{
		if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
			stop_fighting(victim, FALSE);


		// victim �}�ԲĤ@�^�X�N���`�h���򤣧P�w, �H�K ch ��Ů𥴬[ 2022/07/14
		if( !victim || victim->deleted ) return;

		// �ؼаk�]�� ch �|��Ů𥴬[ �G���� 2022/07/15
		//set_fighting(victim, ch);
		//}
	}
	else
	{
		// �w�� artifact �����p, ���Q sleep ��charm ���ؼз|�T�갱�����
		/*if( IS_AFFECTED( victim, AFF_SLEEP )) return;
		if( IS_AFFECTED( ch, AFF_SLEEP )) return;
		if( IS_AFFECTED( victim, AFF_CHARM )) return;
		if( IS_AFFECTED( ch, AFF_CHARM )) return; */
		if( CheckSleepCharm( ch , victim ) ) return; // 2022/12/31
		
		WAIT_STATE( ch, skill_table[gsn_backstab].beats * 2);
		////damage( ch, victim, -1, gsn_backstab, WEAR_WIELD );
		damage( ch, victim, -1, gsn_backstab, WEAR_WIELD, MSG_NORMAL, DMG_NORMAL );
		if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
			stop_fighting(victim, FALSE);

		// victim �}�ԲĤ@�^�X�N���`�h���򤣧P�w, �H�K ch ��Ů𥴬[ 2022/07/14
		if( !victim || victim->deleted ) return;

		// �ؼаk�]�� ch �|��Ů𥴬[ �G���� 2022/07/15
		//set_fighting(victim, ch);
	}

	return;
}



void do_circle( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char       arg [ MAX_INPUT_LENGTH ];
    int sklv = get_skill_level(ch, gsn_circle);
	int TrickySklv = get_skill_level(ch, gsn_tricky_pacing);

	if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_circle] == 0 )
	{
		send_to_char( "����H\n\r", ch );
		return;
	}

	if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		return;

	if ( !ch->fighting )
	{
		send_to_char( "¶��ޯ�u��b�԰����ϥ�.\n\r", ch );
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
        victim = ch->fighting;
    else
        if ( !( victim = get_char_room( ch, arg ) ) )
        {
            send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
            return;
        }

    if ( victim == ch || !can_see(ch, victim) )
    {
        send_to_char( "�A��F�Ӱ�Aı�o�Y���I��.\n\r", ch );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

    if ( victim != ch->fighting )
    {
        send_to_char( "�L���O�A���԰���H.\n\r", ch );
        return;
    }

    if ( !victim->fighting )
    {
        act( "$E ê�ۧA����F�ܡH", ch, NULL, victim, TO_CHAR );
        return;
    }
		/*
       if ( !is_same_group( ch, victim->fighting ) )
       {
       send_to_char( "�A�٬O�����U�n�ۤv�a�C\n\r", ch );
       return;
       }*/

    for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
        if ( rch->fighting == ch )
            break;

    if ( !IS_NPC(ch) && rch )
    {
        send_to_char( "�A�ۤv�����۾԰��I\n\r", ch );
        return;
    }

    if ( !( obj = get_eq_char( ch, WEAR_WIELD ) )){
        if( !(obj = get_eq_char( ch, WEAR_TWO_HAND) )){
            send_to_char( "�A�S���˳ƪZ��.\n\r", ch );
            return;
        }
    }
    if( obj->value[3] != 11 )
    {
        send_to_char( "�A�����˳ƤP�����Z��.\n\r", ch );
        return;
    }
    
	if(!IS_NPC(ch) && ch->move < (250 - sklv * 15) )
	{
		send_to_char( "�A����O�L�k���A�A��¶��I\n\r",ch );
		return;
	}
	
	if(!IS_NPC(ch)) ch->move -= 250 - sklv * 15;

    act( "�A�p���l�l�a¶��F $N ����...", ch, NULL, victim, TO_CHAR	);
    act( "$n �p���l�l�a¶��F $N ����...", ch, NULL, victim, TO_NOTVICT );

    check_killer( ch, victim );
    
	/*  old version
	if ( IS_NPC( ch )
		|| ( number_percent( ) * weakness_percent(victim, gsn_circle) ) / 100 <
			( ( sklv*2+(sklv-8)  + ch->hitroll ) / 2 + sklv + (sklv/3)*5 + get_curr_dex( ch )*4 + get_curr_wis( ch )*3 )/4 
			+ ( ch->pcdata->autocombat == AUTOC_CRITICAL ) ? get_skill_percent(ch, gsn_deadly_strike) / 3 : 0 
			+  ( ch->pcdata->prime_attr == APPLY_DEX ) ? 5 : 0 )
	*/
	// �[�J weapon type �R���]�w
	if ( IS_NPC( ch )
		|| ( ( ch->pcdata->prime_attr == APPLY_DEX ) && ( (number_percent( ) < (100 + TrickySklv*3)
			* weakness_percent(victim, gsn_circle) * weakness_percent(victim, gsn_pierce_attack) / 10000)) )
		|| ( number_percent( ) < ( ch->hitroll/3 + sklv/2 * get_curr_dex(ch) + TrickySklv*3 )
			* weakness_percent(victim, gsn_circle) * weakness_percent(victim, gsn_pierce_attack) / 10000 ) )
	{
		/* �����]�w  2020/09/16
        CHAR_DATA *old_fighting;
        old_fighting = victim->fighting;
		*/
        //stop_fighting( ch, FALSE );//break victim's current fighting, and
        //stop_fighting( victim, FALSE );//break victim's current fighting, and
        multi_hit( ch, victim, gsn_circle ); //change victim's fighting target to ch
        if( victim && !victim->deleted && get_skill_percent(ch, gsn_circle) /2 + sklv * 3 > number_percent() ){
            multi_hit( ch, victim, gsn_circle ); //change victim's fighting target to ch
        }

		// �w�� artifact �����p, ���Q sleep ��charm ���ؼз|�T�갱�����
		/*if( IS_AFFECTED( victim, AFF_SLEEP )) return;
		if( IS_AFFECTED( ch, AFF_SLEEP )) return;
		if( IS_AFFECTED( victim, AFF_CHARM )) return;
		if( IS_AFFECTED( ch, AFF_CHARM )) return; */
		if( CheckSleepCharm( ch , victim ) ) return; // 2022/12/31

		/* �����]�w �� asn ���|�] circle �Q�ର�ؼ� 2020/09/16
        if( victim && !victim->deleted && victim->in_room == old_fighting->in_room
                && number_percent() + 30 < sklv * 6 +get_skill_percent(ch, gsn_circle) /2){
            act( "$n �յ��ಾ�԰��ؼЧ��� $N ", victim, NULL, old_fighting, TO_ROOM );
            stop_fighting(victim, FALSE);
            set_fighting(victim, old_fighting);
        }
		*/
		/*  �ثe�L�@��  2020/09/16
        if( number_percent() < get_skill_percent( ch, gsn_circle ) ) {
           if (! is_qmark(victim, "circle_stun_mark" ) )
           {
              WAIT_STATE( victim, 8 );
              sprintf( arg, "self circle_stun_mark 1 0 0 0 0 1" );
              do_add_qmark(victim, arg);
           }
        }*/
    }
    else
        act( "�A¶���� $N ���I��...", ch, NULL, victim, TO_CHAR );

	// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

    WAIT_STATE( ch, 10 );
    return;
}

void do_berserk( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	int bonus = 0;
	int sklv = get_skill_level(ch, gsn_berserk);
	/* Don't allow charmed mobs to do this, check player's level */
	if (   ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		|| ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_berserk] == 0 ) )
	{
		if (IsSorGateMobByChar(ch)){
			int nMobIndex = ch->pIndexData->vnum;
			int accepted = FALSE;
			if (nMobIndex == MOB_VNUM_SOR_LIGHTNING2) {
				if (ch->master && get_skill_level(ch->master, gsn_lightning_elemental) >= 6) {
					accepted = TRUE;
				}
			}
			if (accepted == FALSE) {
				if (ch->master)
					send_to_char("�����|�o�ӧޥ�.\n\r", ch->master);
				return;
			}
		}else{
			send_to_char( "����H\n\r", ch );
			return;
		}
	}

	if ( !ch->fighting )                                         
	{
		send_to_char( "�A�èS���b�԰�.\n\r", ch );
		return;                                                             
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	if ( is_affected( ch, gsn_berserk ) )
		return;

	if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_berserk] == 0 )
	{
		send_to_char( "�Q�n�o�g�H�A�O����L�F�C", ch );
		return;
	}

	send_to_char( "�A�r�P���R�۪Z���A����|�����q!\n\r", ch );
	send_to_char( "�A�媺�����u�n�A��{���A�o�g!\n\r", ch );

	if ( !IS_NPC( ch ) )
		bonus = get_skill_percent(ch, gsn_berserk) / 10 + sklv + 7;
	else
		bonus = ch->level / 2;

	af.type      = gsn_berserk;
	af.duration  = -1;
	af.location  = APPLY_DAMROLL ;
	af.modifier  = bonus;
	af.bitvector = 0;
	affect_to_char( ch, &af ); 


	af.location  = APPLY_RDAMAGE;
	af.modifier  = ( bonus * -3 );
	affect_to_char( ch, &af );

	if( sklv > 5 ){
		af.location = APPLY_ADAMAGE;
		af.modifier = 50;
		affect_to_char( ch, &af );
	}
	af.location  = APPLY_DODGE;
	af.modifier  = bonus * 9;
	affect_to_char( ch, &af );

	if( sklv > 4 ){
		af.location = APPLY_DAMROLL;
		af.modifier = sklv * sklv - 20;
		affect_join( ch, &af );
	}

	send_to_char( "�A���U�@���A���O�r��!\n\r", ch );
	act( "$n ���o�o�g!", ch, NULL, NULL, TO_ROOM );

	return;
}



void do_change( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [ MAX_INPUT_LENGTH ];

	if( !check_blind( ch ) ) return;

	/* Don't allow charmed mobs to do this, check player's level */
	if (   ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		|| ( !IS_NPC( ch ) && number_percent() > get_skill_percent( ch, gsn_change_fighting) ) )
	{
		send_to_char( "����H\n\r", ch );
		return;
	}

	if ( is_affected( ch, gsn_berserk ) )
	{
		send_to_char( "�A�����o�����I\n\r", ch );
		return;
	}

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "�אּ�����֡H\n\r", ch );
		return;
	}

	if ( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		return;
	}

	if ( victim == ch )
	{
		send_to_char( "�Q�۱��]���γo�ˡC\n\r", ch );
		return;
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	if ( !can_see(ch, victim) ) 
	{
		send_to_char("�o�̨S���o�ӤH.\n\r", ch);
		return;
	}

	if ( ch->fighting == victim )
	{
		send_to_char( "�A�w�g�b�M�L�@�ԤF.\n\r", ch );
		return;
	}

	if ( !victim->fighting )
	{
		send_to_char( "�L�èS���b�԰�.\n\r", ch );
		return;
	}

	if ( is_same_group( ch, victim ) )
	{
		send_to_char( "�����ۤv���P��H\n\r", ch );
		return;
	}

	if ( !check_blind ( ch ) )
		return;

	// ch ���԰��ؼгQ rescue ��@�w�^�X���L�k change 2023/01/12
	if( is_qmark( ch , "skill_rescue_round" )->v0 > 0 )
	{
		if( !is_chinese(ch, victim)){
			act( "You change fighting target to $N, but failed...", ch, NULL, victim, TO_CHAR );
			act( "$n turn $s target to You, but failed...", ch, NULL, victim, TO_VICT );
			act( "$n turn $s target to $N, but failed...", ch, NULL, victim, TO_NOTVICT );
		}else{
			act( "�A�յ۶}�l���� $N�A�����ѤF�C", ch, NULL, victim, TO_CHAR );
			act( "$n �յ��ਭ�V�A�o�ʧ����A�����ѤF�C", ch, NULL, victim, TO_VICT );
			act( "$n �յ��ਭ�V $N �o�ʧ����A�����ѤF�C", ch, NULL, victim, TO_NOTVICT );
		}
		return;
	}

	//if( !IS_NPC( ch ) &&!IS_SET( ch->act, PLR_CHINESE ) )
	if( !is_chinese(ch, victim))
	{
		act( "You change your fighting target to $N!", ch, NULL, victim, TO_CHAR );
		act( "$n change $s fighting target to $N!", ch, NULL, victim, TO_NOTVICT );
	}
	else
	{
		act( "�A�}�l���� $N�I", ch, NULL, victim, TO_CHAR );
		act( "$n �ਭ�V $N �o�ʧ����I", ch, NULL, victim, TO_NOTVICT );
	}

	if( !is_chinese(victim, ch) )
		act( "$n turn $s target to You!", ch, NULL, victim, TO_VICT );
	else
		act( "$n �ਭ�V�A�o�ʧ����I", ch, NULL, victim, TO_VICT );


	ch->fighting	= NULL;
	ch->position	= POS_STANDING;
	WAIT_STATE( ch, skill_table[gsn_change_fighting].beats );
	update_pos( ch );
	stop_fighting(ch, FALSE);
	set_fighting( ch, victim );

	return;
}

void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA      *victim, *rch;
	OBJ_DATA       *feet = NULL; // add at 200122
    char            arg [ MAX_INPUT_LENGTH ];
    EXIT_DATA      *pexit;
    int	 door       = 0;
	int  nDamBonus  = 0; // add at 2021/12/21
    bool ringout    = FALSE;
    int  nKickSklv  = get_skill_level(ch, gsn_kick);
    int  nPwUpSklv  = get_skill_level(ch, gsn_powerup);
	int  nFistSklv  = get_skill_level(ch, gsn_fist_fight);      // add at 200122
	int  nEDSklv    = get_skill_level(ch, gsn_enhanced_damage); // add at 200122
	int  nKickHit   = 0; // add at 2021/12/21
    int  nDamage    = 0;
	int  Old_Dam    = 0; // add at 200122
    int  nKickDmg   = 0;
    int  nMonkBonus = 0;
    int  nMvCost    = 90;
    int  nCurrWis   = get_curr_wis( ch );
    int  nCurrDex   = get_curr_dex( ch );
	int  nCurrStr   = get_curr_str( ch );
	int  times = 3;  // add 200122

	// 2022/11/21
	if( !IS_NPC(ch) && get_skill_percent(ch, gsn_kick) <= 0 ){
		send_to_char("�A���|����A���p�G�Q��H���ѽХ� kickass.\n\r", ch);
		return;
	}

	// 2022/11/21
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}

	// �קאּ pc �ޯ൥�� 5 �H�U�u��b�԰����ϥ� 2022/11/21
	if ( !IS_NPC(ch) && !ch->fighting && nKickSklv < 5 )
	{
		// kick social �קאּ kickass 2022/11/21
		//check_social( ch, "kick", argument );
		send_to_char("�A�ä��O�b�԰���.\n\r", ch);
		return;
	}
    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
        return;
    }
    /* Don't allow charmed mobs to do this, check player's level */
    if (   ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		|| ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_kick] == 0 ) )
    {
        send_to_char("����H\n\r", ch);
        return;
    }
    if ( !check_blind( ch ) && get_skill_percent(ch, gsn_blind_fight) <= number_percent() )
        return;

    one_argument( argument, arg );

    if ( arg[0] != '\0' )
    {
        CHAR_DATA * vch;

        if ( is_affected( ch, gsn_berserk ) && ( vch = get_char_room( ch, arg ) ) != victim ) 
        {
            send_to_char( "�A�����o�����A�u�Q���겴�e���ĤH�I\n\r", ch );
            return;
        }

        if ( !( vch = get_char_room( ch, arg ) ) )
        {
            send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
            return;
        }

		victim = vch;
		
    }else if( ch->fighting ){
		victim = ch->fighting;
	}else{
		send_to_char("�A�Q��֭���H\n\r", ch);
		return;
	}

	// �קאּ��ۤv���� 2022/11/21
	//if( is_same_group( ch, victim ) )
	if( victim == ch )
	{
		// kick social �קאּ kickass 2022/11/21
		check_social( ch, "kickass", argument );
		return;
	}

    if(!is_affected(ch, gsn_powerup))
		nPwUpSklv = 0;

	nMvCost += nPwUpSklv*10;

    if( ch->move < nMvCost)
    {
        send_to_char( "�A�Ӳ֤F.\n\r", ch );
        return;
    }
	
	// �s�W�h���ƭȼv�T�P�w kick �R���P�_ , �H nKickHit �@���R���P�w 2021/12/21
	if( !IS_NPC(ch) ){  // pc �R�� base
		nKickHit = get_skill_percent(ch, gsn_kick) + nKickSklv;
	}else{              // npc �R�� base
		if( IS_SET(ch->special, EXPERT ) )
			nKickHit = 10 + ch->level + nKickSklv*2;
		else
			nKickHit = ch->level + nKickSklv*2;
	}

	nKickHit = nKickHit + DEX_DIFF( ch , victim); // dex �t�ȼv�T
	if( !IS_NPC(victim) ){
			// �ؼ� autododge ���C�R���v
		if( victim->pcdata->autocombat == AUTOC_DODGE )
			nKickHit = nKickHit - (get_skill_percent(victim, gsn_autododge)/20 + get_skill_level(victim, gsn_autododge)*2);
			// �ؼ� dodge �� fool fee ���ŭ��C�R���v
		nKickHit = nKickHit - (get_skill_level(victim, gsn_dodge) + get_skill_level(victim, gsn_fool));
	}

	if(ch->class == MONK || IS_NPC(ch)) nMonkBonus = 1;

	nDamage  = number_range(ch->level - 5, ch->level ) + nKickSklv * (nKickSklv + 4) / 2;  // base dam

	if(number_percent() < get_skill_percent(ch, gsn_enhanced_damage) + nEDSklv*2)
		nDamage = nDamage * (4 + nEDSklv) / 3;
	
	nDamage += (ch->damroll + ch->apdr) * (4 + nKickSklv) * (nKickSklv + 4 + nMonkBonus) / 100;
	
	nDamage  = nDamage * (nCurrStr + 10) / 28;

	if( IS_SET( ch->act, PLR_FIST_FIGHT) )
		nDamage += nDamage * (4 + nFistSklv) * (4 + nFistSklv) / 350;

    if( is_affected(ch, gsn_powerup) )
		nDamage += nDamage * (4 + nPwUpSklv) * (4 + nPwUpSklv) / 350;

		// �s�W pc �Ť�������Ź� kick �[���R��
	if( !IS_NPC(ch) )
		nKickHit += nFistSklv*2; // �Ť�������Ź� kick �[���R��

		// �s�W npc class 32 �ˮ`�ΩR���v�T 2021/12/21
	if( IS_NPC(ch) ){
		if( IS_SET( ch->class, 32 ) )
			nKickHit += 10;
		else
			nDamage = nDamage - nDamage/3;
	}

	/*
		���� kick �R����B�z , �קK�y���S���H�S�𭸪����D
		�÷s�W nDamBonus �@�����v�ˮ`�[�����P�w�� 2021/12/21

		// ringout max = 48%
		if ( (nCurrDex + nCurrWis)/2 + nKickSklv * 4 > number_percent()){
			if(nMonkBonus > 0){
				door = number_door( ); // generate a random number from 0-5
				if ( ( pexit = ch->in_room->exit[door] ) != 0
						&& pexit->to_room
						&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
					ringout = TRUE;
				}
			}
			nDamage *= 2;
		}
	*/
	nDamBonus = (nCurrDex + nCurrWis)/2 + nKickSklv * 4;
	if( IS_NPC(ch) ) nDamBonus = nDamBonus/2;
	
	// blood thirsty ���A�U�ˮ`�[�����v���� 2021/12/25
	if ( isBloodThirsty(ch) )
		nDamBonus += get_skill_level(ch, gsn_blood_thirsty)*4;

	nDamage -= victim->pdr / 2;

    if(nDamage < 0) nDamage = 0;

	// �h�q kick �C�o�ˮ`�ȸ���
	Old_Dam = nDamage;

	if(!IS_NPC(ch)) ch->move -= nMvCost;

	/*   old version before 2020.01.21

		if ( ( !IS_NPC( ch) && number_percent( ) < get_skill_percent(ch, gsn_kick) + nKickSklv * 5 + nMonkBonus * 5)
				|| ( IS_NPC(ch) && number_percent() < ch->level - 20 ))
		{
			nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			if ( victim->deleted ) return;

			// Monk �s�򭸽�P�w
			if (nMonkBonus > 0)
			{
				if ( ch->move >= nMvCost && nCurrWis + nCurrDex + nKickSklv * (nKickSklv + 4) / 2 > number_percent() )
				{
					ch->move -= nMvCost;
					WAIT_STATE( ch, 3 );
					nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
					damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					if ( victim->deleted ) return;
				}

				if ( ch->move >= nMvCost && nCurrWis + ch->level > number_percent() )
				{
					ch->move -= nMvCost;
					WAIT_STATE( ch, 3 );
					nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
					damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					if ( victim->deleted ) return;
				}

				if ( ch->move >= nMvCost && !IS_NPC( ch ) && (ch->pcdata->respect * 10 + ch->hitroll > number_percent() ) )
				{
					ch->move -= nMvCost;
					nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
					damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					WAIT_STATE( ch, 3 );
					if ( victim->deleted ) return;
				}
			}
			if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
			{
				nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
				damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
				if ( victim->deleted ) return;
				if(nKickSklv >= 6)
				{
					nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
					damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
					if ( victim->deleted ) return;
				}
			}
		}
		else
		{
			damage( ch, victim, -1, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		} 
	*/

		// Beholder �l�[����P�w
	if ( IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER )){
		if( nKickHit * weakness_percent(victim, gsn_kick) / 100 > number_percent() )
		{
			nDamage = number_range( Old_Dam , Old_Dam * 2 );
				// �𭸶ˮ`�[���ν𭸧P�w 2021/12/21
			if ( nDamBonus > number_percent() ){
				if(nMonkBonus > 0){
					door = number_door( ); // generate a random number from 0-5
					if ( ( pexit = ch->in_room->exit[door] ) != 0
							&& pexit->to_room
							&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
						ringout = TRUE;
					}
				}
				nDamage *= 2;
			}
			nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			nKickDmg = nKickDmg * weakness_damage(victim, gsn_kick)/100;
			damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			if ( victim->deleted ) return;
			if(nKickSklv >= 5){
				nDamage = number_range( Old_Dam , Old_Dam * 2 );
					// add at 2021/12/21
				if ( nDamBonus > number_percent() ){
					if(!ringout && nMonkBonus > 0){
						door = number_door( ); // generate a random number from 0-5
						if ( ( pexit = ch->in_room->exit[door] ) != 0
								&& pexit->to_room
								&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
							ringout = TRUE;
						}
					}
					nDamage *= 2;
				}
				nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
				damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
				if ( victim->deleted ) return;
			}
		}else{
				// kick �L�R�������p�U�N�J c_dam_message ���2021/10/06
			c_dam_message( ch, victim, -1, gsn_kick, WEAR_NONE );
				//damage( ch, victim, -1, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			if ( victim->deleted ) return;
		}
	}
		// �e�� kick �P�w�𭸪����p�U , �h���i��᭱�� kick 2021/12/21
	if( !ringout ){
			// �� 1 �� kick
		if( nKickHit * weakness_percent(victim, gsn_kick) / 100 > number_percent() )
		{
			nDamage = number_range( Old_Dam , Old_Dam * 2 );
				// add at 2021/12/21
			if ( nDamBonus > number_percent()){
				if(nMonkBonus > 0){
					door = number_door( ); // generate a random number from 0-5
					if ( ( pexit = ch->in_room->exit[door] ) != 0
							&& pexit->to_room
							&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
						ringout = TRUE;
					}
				}
				nDamage *= 2;
			}
			nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			nKickDmg = nKickDmg * weakness_damage(victim, gsn_kick)/100;
			damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			feet = get_eq_char( ch, WEAR_FEET );
			if(feet && number_bits(5) < 8) waste_dura(feet); // 33%
			if ( victim->deleted ) return;
			// �W�[ obj_hit ���P�w 2022/10/23
			if ( feet && feet->pIndexData->progtypes & OBJ_HIT )
				oprog_hit_trigger(feet);
			// �B�z�Ĥ@��}�� artifact
			if( feet && (feet->hp || feet->material->hp) )
			{
				OBJ_HP * ohp = feet->hp;
				for( ; ohp; ohp = ohp->next )
				{
					if( IS_SET( ohp->event, HP_FIGHT ) )
					{
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
					}
					if( IS_SET( ohp->event, HP_WEAPON ) ) 
					{
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
					}
				}
				if( feet->material->hp )
				{
					for( ohp=feet->material->hp ; ohp; ohp = ohp->next )
					{
						if( IS_SET( ohp->event, HP_FIGHT ) )
						{
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
						if( IS_SET( ohp->event, HP_WEAPON ) ) {
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
					}
				}
			}
			if ( victim->deleted ) return;
		}else{
				// kick �L�R�������p�U�N�J c_dam_message ���2021/10/06
			c_dam_message( ch, victim, -1, gsn_kick, WEAR_NONE );
				//damage( ch, victim, -1, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			if ( victim->deleted ) return;
		}
	}
		// �� 2 ~ 3 �� kick
	while( times-- && !victim->deleted && victim->in_room == ch->in_room )
	{
		// �P�w�O�_�w�g�� , �p�G�w�g�𭸫h���� kick ���P�w 2021/12/21
		if ( ringout ) break;
		if( ch->move < nMvCost) break;
		if(IS_NPC(ch) && number_percent() > times * nCurrDex + ch->level/4){
			break;
		}else if( IS_SET( ch->act, PLR_FIST_FIGHT)
					&& number_percent() > times * nCurrDex * (1 + nMonkBonus) + nKickSklv * 4 ){
			break;
		}else if(number_percent() > times * nCurrDex * (1 + nMonkBonus) + nKickSklv * 2 ){
			break;
		}

		if(!IS_NPC(ch)) ch->move -= nMvCost;
		// wait_state �Τ@����̫᭱ 2022/02/24
		//WAIT_STATE( ch, skill_table[gsn_kick].beats - nKickSklv/2);

		if( nKickHit * weakness_percent(victim, gsn_kick) / 100 > number_percent() )
		{
			nDamage = number_range( Old_Dam , Old_Dam * 2 );
				// add at 2021/12/21
			if ( nDamBonus > number_percent()){
				if(nMonkBonus > 0){
					door = number_door( ); // generate a random number from 0-5
					if ( ( pexit = ch->in_room->exit[door] ) != 0
							&& pexit->to_room
							&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
						ringout = TRUE;
					}
				}
				nDamage *= 2;
			}
			nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			nKickDmg = nKickDmg * weakness_damage(victim, gsn_kick)/100;
			damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			feet = get_eq_char( ch, WEAR_FEET );
			if(feet && number_bits(5) < 8) waste_dura(feet); // 33%
			if ( victim->deleted ) return;
			// �W�[ obj_hit ���P�w 2022/10/23
			if ( feet && feet->pIndexData->progtypes & OBJ_HIT )
				oprog_hit_trigger(feet);
			// �B�z�Ĥ@��}�� artifact
			if( feet && (feet->hp || feet->material->hp) )
			{
				OBJ_HP * ohp = feet->hp;
				for( ; ohp; ohp = ohp->next )
				{
					if( IS_SET( ohp->event, HP_FIGHT ) )
					{
						if ( (!IS_NPC(ch)
							&& IS_SET( ch->act, PLR_ARTIFACT )
							&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
								+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
							|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
						{
							trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
					if( IS_SET( ohp->event, HP_WEAPON ) ) {
						if ( (!IS_NPC(ch)
							&& IS_SET( ch->act, PLR_ARTIFACT )
							&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
								+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
							|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
						{
							trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
				}
				if( feet->material->hp )
				{
					for( ohp=feet->material->hp ; ohp; ohp = ohp->next )
					{
						if( IS_SET( ohp->event, HP_FIGHT ) )
						{
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
						if( IS_SET( ohp->event, HP_WEAPON ) ) {
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
					}
				}
			}
			if ( victim->deleted ) return;
		}else{
				// kick �L�R�������p�U�N�J c_dam_message ���2021/10/06
			c_dam_message( ch, victim, -1, gsn_kick, WEAR_NONE );
				//damage( ch, victim, -1, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			if ( victim->deleted ) return;
		}
	}
		// Centaur �ĤG���}�l�[����P�w , �p�G�ؼШS�Q�P�w��𭸤~�@�� 2021/12/21
	if( !ringout && IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ){
		if( nKickHit * weakness_percent(victim, gsn_kick) / 100 > number_percent() )
		{
			nDamage = number_range( Old_Dam , Old_Dam * 2 );
				// add at 2021/12/21
			if ( nDamBonus > number_percent()){
				if(nMonkBonus > 0){
					door = number_door( ); // generate a random number from 0-5
					if ( ( pexit = ch->in_room->exit[door] ) != 0
							&& pexit->to_room
							&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
						ringout = TRUE;
					}
				}
				nDamage *= 2;
			}
			nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			nKickDmg = nKickDmg * weakness_damage(victim, gsn_kick)/100;
			damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			feet = get_eq_char( ch, WEAR_FEET_2 );
			if(feet && number_bits(5) < 8) waste_dura(feet); // 33%
			if ( victim->deleted ) return;
			// �W�[ obj_hit ���P�w 2022/10/23
			if ( feet && feet->pIndexData->progtypes & OBJ_HIT )
				oprog_hit_trigger(feet);
			// Centaur �P�w�ĤG��}�� artifact
			if( feet && (feet->hp || feet->material->hp) )
			{
				OBJ_HP * ohp = feet->hp;
				for( ; ohp; ohp = ohp->next )
				{
					if( IS_SET( ohp->event, HP_FIGHT ) )
					{
						if ( (!IS_NPC(ch)
							&& IS_SET( ch->act, PLR_ARTIFACT )
							&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
								+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
							|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
						{
							trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
					if( IS_SET( ohp->event, HP_WEAPON ) ) {
						if ( (!IS_NPC(ch)
							&& IS_SET( ch->act, PLR_ARTIFACT )
							&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
								+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
							|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
						{
							trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
				}
				if( feet->material->hp )
				{
					for( ohp=feet->material->hp ; ohp; ohp = ohp->next )
					{
						if( IS_SET( ohp->event, HP_FIGHT ) )
						{
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
						if( IS_SET( ohp->event, HP_WEAPON ) ) {
							if ( (!IS_NPC(ch)
								&& IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * get_curr_wis( ch ) / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								|| (IS_NPC( ch ) && ohp->percent * get_curr_wis( ch ) / 25 > number_percent() ) )
							{
								trigger_obj_hp(feet, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
					}
				}
			}
			if ( victim->deleted ) return;
		}else{
				// kick �L�R�������p�U�N�J c_dam_message ���2021/10/06
			c_dam_message( ch, victim, -1, gsn_kick, WEAR_NONE );
				//damage( ch, victim, -1, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
			if ( victim->deleted ) return;
		}
	}

	// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);
	
	// blood thirsty ���A�U stun �T�w�� 1 2021/12/25
	if ( isBloodThirsty(ch) )
		WAIT_STATE( ch, 1 );
	else
		WAIT_STATE( ch, skill_table[gsn_kick].beats - nKickSklv/2);

	if ( ringout && !victim->deleted && victim->in_room == ch->in_room )
	{
		// defensive ����׳Q�� 2021/12/21
		if( is_affected(victim, gsn_defensive )	){
			send_to_char( "�A���K�몺���m�A���A�קK�D���𭸡C\n\r", victim );
			return;
		}
		// autoparry �����|����Q�� 2021/12/21
		if( !IS_NPC(victim) ){
			if( victim->pcdata->autocombat == AUTOC_PARRY
				&& ((get_skill_percent(victim, gsn_autoparry)/11 + get_skill_level(victim, gsn_autoparry) * 15)
					> number_percent() )){
				return;
			}
		}
		// �髬���׽� 2022/12/24
		if( ( get_curr_str(ch) * ( 12 + nPwUpSklv ) - get_curr_size(victim) * 2 ) / 3 < number_percent() )
			return;

		if ( !IS_NPC(victim) || !IS_SET(victim->act, ACT_SENTINEL)){
			act( "$n �Q $N �𭸤F�I", victim, NULL, ch, TO_ROOM	 );
			send_to_char ( "�A�Q�𭸤F�I\n\r", victim );
			for( rch = victim->in_room->people; rch; rch = rch->next_in_room ){
				if ( rch == victim || rch->fighting == victim ){
					rch->fighting	= NULL;
					rch->position	= POS_STANDING;
					if ( is_affected( rch, gsn_berserk ) )
						affect_strip( rch, gsn_berserk );
					update_pos( rch );
				}
			}
			move_char( victim, door );
		}
	}

	// �w�� artifact �����p, ���Q sleep ��charm ���ؼз|�T�갱�����
	/*if( IS_AFFECTED( victim, AFF_SLEEP )) return;
	if( IS_AFFECTED( ch, AFF_SLEEP )) return;
	if( IS_AFFECTED( victim, AFF_CHARM )) return;
	if( IS_AFFECTED( ch, AFF_CHARM )) return;*/
	if( CheckSleepCharm( ch , victim ) ) return; // 2022/12/31
	
	// ���ަ��S����, ���n�}�� 2022/11/21
	if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
		stop_fighting(victim, FALSE);

    return;
}
/*
void do_shadowless_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA      *victim, *rch;
    char            arg [ MAX_INPUT_LENGTH ];
    EXIT_DATA      *pexit;
    int	 door       = 0;
    bool ringout    = FALSE;
		int  nShadSklv  = get_skill_level(ch, gsn_shadowless_kick);
    int  nKickSklv  = get_skill_level(ch, gsn_kick);
    int  nPwUpSklv  = get_skill_level(ch, gsn_powerup);
    int  nDamage    = 0;
    int  nKickDmg   = 0;
    int  nMonkBonus = 0;
    int  nMvCost    = nKickSklv + 4;
    int  nCurrWis   = get_curr_wis( ch );
    int  nCurrDex   = get_curr_dex( ch );
    int  nCurrStr   = get_curr_str( ch );

    if ( !ch->fighting )
    {
        check_social( ch, "kick", argument );
        return;
    }
    // Don't allow charmed mobs to do this, check player's level 
    if ( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
            || ( !IS_NPC( ch )
                && ch->pcdata->learned[gsn_shadowless_kick] == 0 ) )
    {
        send_to_char( "����H\n\r", ch);
        return;
    }

    if ( !check_blind( ch ) && get_skill_percent(ch, gsn_blind_fight) <= number_percent() )
        return;

    one_argument( argument, arg );

    victim = ch->fighting;

    if ( arg[0] != '\0' )
    {
        CHAR_DATA * vch;

        if ( is_affected( ch, gsn_berserk )
                && ( vch = get_char_room( ch, arg ) ) != victim ) 
        {
            send_to_char( "�A�����o�����I\n\r", ch );
            return;
        }

        if ( !( victim = get_char_room( ch, arg ) ) )
        {
            send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
            return;
        }
    }

    if( is_same_group( ch, victim ) )
    {
        check_social( ch, "kick", argument );
        return;
    }

    if(!is_affected(ch, gsn_powerup)) nPwUpSklv = 0;
    nMvCost += nPwUpSklv;

    if( ch->move < nMvCost)
    {
        send_to_char( "�A�Ӳ֤F.\n\r", ch );
        return;
    }
    ch->move -= nMvCost;
    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    nDamage  = number_range(1, ch->level ) + nKickSklv * (nKickSklv + 4) / 2;

    if(ch->class == MONK || IS_NPC(ch)) nMonkBonus = 2;
    if ( number_percent() - nKickSklv * 3 > nCurrDex + nCurrWis ){
        nDamage *= 3 + nMonkBonus;
		}
    else
    {
        if(nMonkBonus > 0)
        {
            door = number_door( ); //generate a random number from 0-5
            if ( ( pexit = ch->in_room->exit[door] ) != 0
                    && pexit->to_room
                    && ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
                ringout = TRUE;
            }
        }
        nDamage *= 5 + nMonkBonus;
    }

    nDamage += ch->apdr * (4 + nKickSklv) * (nKickSklv + 4 + nMonkBonus) / 100;
    nDamage  = nDamage * (nCurrStr + 10) / 28;
    if( is_affected(ch, gsn_powerup) )
        nDamage += nDamage * (nPwUpSklv * (nPwUpSklv + 4) + 10) / 200;
    nDamage -= victim->pdr / 2;
    if(nDamage < 0) nDamage = 0;

    if ( ( !IS_NPC( ch) && number_percent( ) < get_skill_percent(ch, gsn_kick) + nKickSklv * 5 + nMonkBonus * 5)
            || ( IS_NPC(ch) && number_percent() < ch->level - 20 ))
    {
				if ( ch->move >= nMvCost && nCurrWis + nCurrDex + nKickSklv * (nKickSklv + 4) / 2 > number_percent() )
				{
						ch->move -= nMvCost;
						WAIT_STATE( ch, 3 );
						nKickDmg = get_critical_damage(nDamage, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
						damage( ch, victim, nKickDmg, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
						if ( victim->deleted ) return;
				}

    }
    else
    {
        damage( ch, victim, -1, gsn_kick, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
    }

    return;
}
*/

/* Hunt skill added by Amenda, 00/12/6 */
void do_hunt( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim, *vch, *rch;
	char arg [ MAX_INPUT_LENGTH ];

	if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		return;

	if( !ch->in_room || ch->deleted ) return;

	if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_hunt] == 0 )
	{
		send_to_char( "����H\n\r", ch);
		return;
	}

	// autocombat ���]�w�קאּ �L�k�ϥδ����~�L�k�ϥ� hunt 2022/07/14
	//if ( !IS_NPC( ch ) && ch->pcdata->autocombat != AUTOC_NORMAL && ch->pcdata->autocombat != AUTOC_CRITICAL && ch->pcdata->autocombat != AUTOC_CHARGE)
	if ( !IS_NPC( ch )
		&& (   ch->pcdata->autocombat == AUTOC_STOP
			|| ch->pcdata->autocombat == AUTOC_DODGE
			|| ch->pcdata->autocombat == AUTOC_PARRY
			|| ch->pcdata->autocombat == AUTOC_CONCENTRATION
			|| ch->pcdata->autocombat == AUTOC_INSTANT ) )
	{
		send_to_char("�o�ا@�ԫ��A�U�L�k�ϥ��y��.\n\r",ch);
		return;
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	one_argument( argument, arg );


	if ( !ch->fighting )
	{
		if ( arg[0] == '\0' )
		{
			if ( (victim = ch->hunting) )
			{
				act( "�A�����y�� $N.\n\r", ch, NULL, victim, TO_CHAR);
				act( "$n �����y���A.\n\r", ch, NULL, victim, TO_VICT);
				ch->hunting = NULL;
				if ( victim->hunted == ch )
				{
					victim->hunted = ch->next_in_hunt;
					ch->next_in_hunt = NULL;
					return;
				}
				for( vch = victim->hunted; vch; vch = vch->next_in_hunt )
					if ( vch->next_in_hunt == ch )
					{
						vch->next_in_hunt = ch->next_in_hunt;
						ch->next_in_hunt = NULL;
						return;
					}
			}
			send_to_char( "�y���֡H\n\r", ch );
			return;
		}
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
			return;
		}

		if( victim == ch ) return;

		if ( !check_blind( ch ) || ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) )
			return;

		if ( is_safe( ch, victim ) || victim->deleted)
			return;

		if ( IS_NPC( ch ) || number_percent( ) < get_skill_percent(ch, gsn_hunt) )
		{
			if ( ( vch = ch->hunting ) )
			{
				if ( vch->hunted == ch )
					vch->hunted = ch->next_in_hunt;
				else
					for( rch = vch->hunted; rch; rch = rch->next_in_hunt )
						if ( rch->next_in_hunt == ch )
							rch->next_in_hunt = ch->next_in_hunt;
				act( "$N �����y���A�C", vch, NULL, ch, TO_CHAR );
			}
			ch->hunting = victim;
			ch->next_in_hunt = victim->hunted;
			victim->hunted = ch;
			act( "�A�}�l�y�� $N!\n\r", ch, NULL, victim, TO_CHAR);
			act( "$n �}�l�y���A�I\n\r", ch, NULL, victim, TO_VICT);

			// pc �� autocombat ��e���P�w, ���B�P�w�|�� npc �� hunt ���|�i�J�԰�, �G���� 2022/07/14
			//if( ch->pcdata && ch->pcdata->autocombat != AUTOC_STOP )
				multi_hit( ch, victim, TYPE_UNDEFINED );
			return;
		}
		multi_hit( victim, ch, TYPE_UNDEFINED );
		WAIT_STATE( ch, ( ch->class == KNIGHT ) ? 2 : skill_table[gsn_hunt].beats );
		send_to_char( "�A���ѤF�I\n\r", ch );
		return;
	}

	if ( !check_blind( ch ) || ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) )
		return;

	victim = ch->fighting;

	if ( arg[0] != '\0' )
	{
		CHAR_DATA * vch;

		if ( !( vch = get_char_room( ch, arg ) ) )
		{
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
			return;
		}
		if ( vch != victim && !IS_NPC( ch ) )
		{
			send_to_char( "�A���M�O�H���o�����O�I\n\r", ch );
			return;
		}
		if ( ch->hunting == victim )
		{
			send_to_char( "�A�w�g�b�y���L�F.\n\r", ch );
			return;
		}

		if ( ( vch = ch->hunting ) )
		{
			if ( vch->hunted == ch )
				vch->hunted = ch->next_in_hunt;
			else
				for( rch = vch->hunted; rch; rch = rch->next_in_hunt )
					if ( rch->next_in_hunt == ch )
						rch->next_in_hunt = ch->next_in_hunt;
			act( "$N �����y���A�C", vch, NULL, ch, TO_CHAR );
		}
		ch->hunting = victim;
		ch->next_in_hunt = victim->hunted;
		victim->hunted = ch;
		act( "�A�}�l�y�� $N!\n\r", ch, NULL, victim, TO_CHAR);
		act( "$n �}�l�y���A�I\n\r", ch, NULL, victim, TO_VICT);
		return;
	}
	if ( ch->hunting == victim )
	{
		send_to_char( "�A�w�g�b�y���L�F.\n\r", ch );
		return;
	}
	if ( ( vch = ch->hunting ) )
	{
		if ( vch->hunted == ch )
			vch->hunted = ch->next_in_hunt;
		else
			for( rch = vch->hunted; rch; rch = rch->next_in_hunt )
				if ( rch->next_in_hunt == ch )
					rch->next_in_hunt = ch->next_in_hunt;
		act( "$N �����y���A�C", vch, NULL, ch, TO_CHAR );
	}
	ch->hunting = victim;
	ch->next_in_hunt = victim->hunted;
	victim->hunted = ch;
	act( "�A�}�l�y�� $N!\n\r", ch, NULL, victim, TO_CHAR);
	act( "$n �}�l�y���A�I\n\r", ch, NULL, victim, TO_VICT);
	return;
}


/* Assassination skill added by Amenda, 00/12/22 */
void do_ass( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim, *vch, *rch;
	OBJ_DATA  *obj;
	char       arg [ MAX_INPUT_LENGTH ];

	int chance = 0;

	if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		return;
	if( !ch->in_room ) return;

	if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_ass] == 0 ){
		send_to_char("����H\n\r", ch);
		return;
	}
    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
        return;
    }

	if ( !check_blind( ch ) ) return;

	one_argument( argument, arg );
	if ( arg[0] == '\0' ){
		if ( (victim = ch->assassinating) ){
			act( "�A����t�� $N.\n\r", ch, NULL, victim, TO_CHAR);
			//act( "$n ����t���A.\n\r", ch, NULL, victim, TO_VICT);  �ؼгQ�t���ɤ��|���D�ۤv�Q�t��, �G����.
			ch->assassinating = NULL;
			if ( victim->assassinated == ch ){
				victim->assassinated = ch->next_in_ass;
				ch->next_in_ass = NULL;
				return;
			}
			for( vch = victim->assassinated; vch; vch = vch->next_in_ass ){
				if ( vch->next_in_ass == ch ){
					vch->next_in_ass = ch->next_in_ass;
					ch->next_in_ass = NULL;
					return;
				}
			}
		}
	}

	if ( !ch->fighting ){
		if ( arg[0] == '\0' ){
			send_to_char( "�n�����?\n\r", ch );
			return;
		}

		if ( !( victim = get_char_room( ch, arg ) ) ){
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
			return;
		}

		if ( is_safe( ch, victim ) )
			return;

		if ( IS_NPC( ch ) || number_percent( ) < get_skill_percent(ch, gsn_ass) )
		{
			if ( ( vch = ch->assassinating ) )
			{
				if ( vch->assassinated == ch )
					vch->assassinated = ch->next_in_ass;
				else
					for( rch = vch->assassinated; rch; rch = rch->next_in_ass )
						if ( rch->next_in_ass == ch )
							rch->next_in_ass = ch->next_in_ass;
				//act( "$N ����t���A�C", vch, NULL, ch, TO_CHAR );
			}
			ch->assassinating = victim;
			ch->next_in_ass = victim->assassinated;
			victim->assassinated = ch;

			if ( !( obj = get_eq_char( ch, WEAR_WIELD ) )
					 || obj->value[3] != 11 )
			{
				if ( !(obj = get_eq_char( ch, WEAR_TWO_HAND) )
				|| obj->value[3] != 11 )
				{
					send_to_char( "�A�����˳ƤP�����Z��.\n\r", ch );
					return;
				}
			}
			if ( victim->fighting )
			{
				send_to_char( "�A����t���԰������ؼ�.\n\r", ch );
				return;
			}
			if ( IS_NPC( ch ) || number_percent( ) < get_skill_percent(ch, gsn_ass) )
			{
				int dmg = 0, stun = 0;
				if( !IS_NPC(ch)) {
					chance = get_skill_percent(ch, gsn_ass) / 10;
					if( ch->pcdata->prime_attr == APPLY_DEX ) chance += 10;
					if( ch->pcdata->prime_attr == APPLY_STR ) chance += 5;
					if( ch->pcdata->snd_attr == APPLY_STR ) chance += 5;
					if( ch->pcdata->autocombat == AUTOC_CRITICAL ) chance += get_skill_percent(ch, gsn_deadly_strike) / 2;
					if( ch->pcdata->snd_attr == APPLY_DEX ) chance += 3;
					chance += get_skill_level(ch, gsn_ass) + get_skill_level(ch, gsn_ass)/2;
					if( ( IS_AFFECTED( ch, AFF_DARK_RAID ) || number_percent() < chance )){
						dmg = mone_hit( ch, victim, &gsn_backstab, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
						if( dmg >= 0 ) stun += 8;
					}
				}

				//multi_hit( ch, victim, gsn_backstab );
				dmg = mone_hit( ch, victim, &gsn_backstab, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
				if( dmg >= 0 ) stun += 10;
				WAIT_STATE(victim, stun);
				WAIT_STATE(ch, 6);
				if( victim->fighting != ch )
					stop_fighting(victim, FALSE);
				set_fighting(victim, ch);
			}else{
				dam_message( ch, victim, -1, gsn_backstab, WEAR_WIELD );
			}
			WAIT_STATE( ch, 8 - get_skill_level(ch, gsn_ass));
			return;
		}
		send_to_char( "�A���ѤF�I\n\r", ch );
		WAIT_STATE( ch, skill_table[gsn_backstab].beats );
		return;
	}

	victim = ch->fighting;

	if ( arg[0] != '\0' )
	{
		CHAR_DATA * vch;

		if ( !( vch = get_char_room( ch, arg ) ) )
		{
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
			return;
		}
		if ( vch != victim)
		{
			send_to_char( "�A���b�԰��I\n\r", ch );
			return;
		}
		if ( ch->assassinating == victim )
		{
			send_to_char( "�A�w�g�b�t���L�F.\n\r", ch );
			return;
		}

		if ( ( vch = ch->assassinating ) )
		{
			if ( vch->assassinated == ch )
				vch->assassinated = ch->next_in_ass;
			else
				for( rch = vch->assassinated; rch; rch = rch->next_in_ass )
					if ( rch->next_in_ass == ch )
						rch->next_in_ass = ch->next_in_ass;
			//act( "�A���A���� $N ���y���F�I", vch, NULL, ch, TO_CHAR );
		}
		ch->assassinating = victim;
		ch->next_in_ass = victim->assassinated;
		victim->assassinated = ch;
		act( "�A�}�l�t�� $N!\n\r", ch, NULL, victim, TO_CHAR);
		return;
	}
	if ( ch->assassinating == victim )
	{
		send_to_char( "�A�w�g�b�t���L�F.\n\r", ch );
		return;
	}
	if ( ( vch = ch->assassinating ) )
	{
		if ( vch->assassinated == ch )
			vch->assassinated = ch->next_in_ass;
		else
			for( rch = vch->assassinated; rch; rch = rch->next_in_ass )
				if ( rch->next_in_ass == ch )
					rch->next_in_ass = ch->next_in_ass;
		//act( "$N ����t���A�I", vch, NULL, ch, TO_CHAR );
	}   
	ch->assassinating = victim;
	ch->next_in_ass = victim->assassinated;
	victim->assassinated = ch;
	act( "�A�}�l�t�� $N!\n\r", ch, NULL, victim, TO_CHAR);
	WAIT_STATE( ch, 1 );
	return;
}


/* Frenzy skill added by Amenda, 00/12/9 */
void do_frenzy( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [ MAX_INPUT_LENGTH ];
    int i, extrahit;
    int wait = 0;
    int chance = 0;
    int sklv = get_skill_level(ch, gsn_frenzy);
    int hit = 0, move = 0;
    int lv6hit = 0;
    int nRaceBonus = 0;
    int nDualPrac = get_skill_percent(ch, gsn_dual);
    int nDualSklv = get_skill_level(ch, gsn_dual);
    int nDualPerc = -100;
    if(!IS_NPC(ch) && get_eq_char( ch, WEAR_WIELD_2 ) && nDualSklv >= 4){
        nDualPerc = (nDualSklv - 1) * (nDualSklv - 1) * 4;
        if(nDualSklv < 6) nDualPerc = nDualPerc * nDualPrac / 100;
    }
   /*
		�ק� beholder �� werewolf race �[���P�w���B�@�����D
		�ѧP�w race �W�קאּ�P�w race abilities 2022/03/17

		if(str_cmp( race_table[ ch->race ].name, "Beholder" ) == 0){
			int nInbSklv = get_skill_level(ch, gsn_inborn_ability);
			nRaceBonus = 1 + nInbSklv/3;
		}else if(str_cmp( race_table[ ch->race ].name, "Werewolf" ) == 0){
			int nInbSklv = get_skill_level(ch, gsn_inborn_ability);
			nRaceBonus = 1 + nInbSklv/2;
		}
	*/
	if( !IS_NPC(ch)
		&& ( IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER )
			|| IS_SET( race_table[ ch->race ].race_abilities, RACE_WEREWOLF ) ) )
	{
		nRaceBonus = get_skill_level(ch, gsn_inborn_ability);
	}
	
    if ( IS_NPC( ch ) 
            && ( IS_AFFECTED( ch, AFF_CHARM ) 
                || IS_AFFECTED( ch, AFF_SUMMONED ) 
                || ch->clan ) )
    {
        return;
    }

    if ( !IS_NPC( ch ) &&
            ch->pcdata->learned[gsn_frenzy] == 0 )
    {
        send_to_char(
                "����H\n\r", ch );
        return;
    }

    one_argument( argument, arg );

    if ( !ch->fighting )
    {
        send_to_char( "�A�èS���b�԰�.\n\r", ch );
        return;
    }

    if ( !IS_NPC( ch ) && IS_SET( ch->pcdata->autocombat, AUTOC_STOP ) )
    {
      send_to_char( "�A�õL�N�԰�.\n\r", ch );
        return;
    }
    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
        return;
    }
    
	if( !IS_NPC(ch) ) 
	{
		// �קאּ mar ���� base class ���P�w mv 2021/12/22
		//if( ch->pcdata->prime_attr != APPLY_CON || ch->class != MARTIALMAN )
		if( ch->class != MARTIALMAN )
		{
			if( ch->hit * 4 < ch->max_hit ) 
			{
				send_to_char( "�A����O�����F.\n\r", ch );
				return;
			}
		}
		if( ch->move < 150 ) 
		{
			send_to_char("�A����O�����F.\n\r", ch);
			return;
		}
	}

	// Modified by Keric for Frenzy chance, 2005/2/13 
	if( !IS_NPC( ch ) ) {
		if ( ch->class == SHAMAN ) chance = 5;
		else if ( ch->class == WARLORD ) chance = 5;
		else if ( ch->class == MARTIALMAN ) chance = 0;
		else chance = 10;

		if ( ch->pcdata->prime_attr != APPLY_CON )
			chance += 5;
	}

	if ( !IS_NPC( ch ) 
		&& number_percent() + chance > get_skill_percent(ch, gsn_frenzy))
	{
		send_to_char( "�A���ѤF�I\n\r", ch );
		return;
	}

	victim = ch->fighting;

	if ( arg[0] != '\0' )
	{
		CHAR_DATA * vch;

		if ( !( vch = get_char_room( ch, arg ) ) )
		{
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
			return;
		}
		if ( vch != victim)
		{
			send_to_char( "�A���b�M�O�H�@�ԡC\n\r", ch );
			return;
		}
	}
	send_to_char( "�A��������m��!!!\n\r", ch );
	act( "$n �����o���A�z�ܤ@�n�V $N �i�}�r�P�����I", ch, NULL, victim, TO_NOTVICT);
	act( "$n �����o���A�z�ܤ@�n�V�A�i�}�L���������I", ch, NULL, victim, TO_VICT );
	extrahit = ( IS_NPC( ch ) ) ? 9 : get_skill_percent(ch, gsn_frenzy) / 11;
	extrahit += sklv/2;

	if ( !IS_NPC(ch) && ch->class == MARTIALMAN ){
		extrahit += 3;
		// mar blood thirsty ���A�U�[���o�� 2021/12/22
		if ( isBloodThirsty(ch) )
			extrahit += get_skill_level(ch, gsn_blood_thirsty);
	}

	if( sklv < 4 && !IS_NPC(ch)){
		extrahit += nRaceBonus;
		// ���� race size �v�T frenzy �o�ƪ��]�w 2022/03/17
		//for( i = 0; i < 4 + extrahit + race_table[ch->race].size; i ++ )
		for( i = 0; i < 4 + extrahit; i ++ )
		{
			if ( !( victim->in_room == ch->in_room )
				|| ( victim->position == 0 ) 
				|| ch->hit < 0 )
				break;
            
			one_hit( ch, victim, gsn_frenzy, WEAR_WIELD );
			if(number_percent() <= nDualPerc)
				one_hit( ch, victim, gsn_frenzy, WEAR_WIELD_2 );
			wait += ch->wait;
			ch->wait = 0;

			if( !IS_NPC(ch) ) 
			{
				switch( ch->class )
				{
					default:
						ch->hit -= 20-sklv/3;
						ch->move -= 15-sklv/3;
						break;
					case MARTIALMAN:
						ch->hit -= 20-sklv/3;
						break;
					case SHAMAN:
						ch->hit -= 10-sklv/3;
						ch->move -= 10-sklv/3;
						break;
					case WARLORD:
						ch->hit -= 15-sklv/3;
						ch->move -= 5-sklv/3;
						break;
					case RANGER:
						ch->hit -= 10-sklv/3;
						ch->move -= 20-sklv/3;
				}
				if( ch->move < 0 ) break;
				if( ch->hit < -sklv*4 ) break;
			}
		}
	}
	else 
	{
		// hp per hit old version lv6 100 , lv5 120 , lv 4 150 
		// hp per hit new version lv6 150 , lv5 180 , lv 4 225 
		hit = UMAX(ch->hit, (ch->max_hit / 13) );
		move = UMAX(ch->move, (ch->max_move / 13) );
		if(sklv == 6)
			lv6hit = ch->max_hit / (150 * PPL_HP_MULTIPLYER);
		else if(sklv == 5)
			lv6hit = ch->max_hit / (180 * PPL_HP_MULTIPLYER);
		else
			lv6hit = ch->max_hit / (225 * PPL_HP_MULTIPLYER);

		//1050822 modify by jye ����npc �� frenzy �q�ƤU��(20, 15, 10)
		if (IS_NPC(ch) )
		{
			if(sklv == 6)
				extrahit = URANGE(UMAX(4+extrahit, 20), UMAX(lv6hit, 20), 20);
			else if(sklv == 5)
				extrahit = URANGE(UMAX(4+extrahit, 15), UMAX(lv6hit, 15), 15);
			else
				extrahit = URANGE(UMAX(4+extrahit, 10), UMAX(lv6hit, 10), 10);

			//char        buf     [ MAX_STRING_LENGTH * 2];
			//sprintf(buf, "%s %s %d %s \n\r", HIY,"frenzy",extrahit,NOR);
			//act( buf, ch, NULL, victim, TO_ROOM_IMM);
		}
		else
		{
			/*  ��� memory �ӷl , frenzy �o�ƤU�� 2020/10/20
				if(sklv == 6)
					extrahit=URANGE(4+extrahit, lv6hit, 36);
				else if(sklv == 5)
					extrahit=URANGE(4+extrahit, lv6hit, 24);
				else
					extrahit=URANGE(4+extrahit, lv6hit, 18);
				extrahit += nRaceBonus;
			*/
			if(sklv == 6)
				extrahit = URANGE(5+extrahit, lv6hit, 24);
			else if(sklv == 5)
				extrahit = URANGE(5+extrahit, lv6hit, 18);
			else
				extrahit = URANGE(5+extrahit, lv6hit, 12);
			extrahit += nRaceBonus;
			if ( ch->class == MARTIALMAN ){
				extrahit += 3;
				// mar blood thirsty ���A�U�[���o�� 2021/12/22
				if ( isBloodThirsty(ch) )
					extrahit += get_skill_level(ch, gsn_blood_thirsty);
			}
		}

		//modified by tale 12/29/2010 for frenzy times of NPC constrain by using hard code
		if (IS_NPC(ch) ) 
		{
			hit = UMAX(ch->hit, (ch->max_hit / 30) );
			move = UMAX(ch->move, (ch->max_move / 30) );
		}

		for(i = 0;i<extrahit;++i)
		{
			if( !( victim->in_room == ch->in_room )
				|| ( !IS_NPC(ch)  // pos_dead �� ch �p�� 0 �����p �[�J pc ���w 2022/12/29
					&& ( ( victim->position == 0 )
						|| ch->hit < 0 ) ) )
				break;
			one_hit( ch, victim, gsn_frenzy, WEAR_WIELD );
			if(number_percent() <= nDualPerc)
				one_hit( ch, victim, gsn_frenzy, WEAR_WIELD_2 );
			wait += ch->wait;
			ch->wait = 0;
			if( !IS_NPC(ch))
			{
				switch( ch->class )
				{
					default:
						ch->hit -= 20-sklv/3;
						ch->move -= 15-sklv/3;
						hit -= 20-sklv/3;
						move -= 15-sklv/3;
						break;
					case MARTIALMAN:
						ch->hit -= 20-sklv/3;
						hit -= 20-sklv/3;
						break;
					case SHAMAN:
						ch->hit -= 10-sklv/3;
						ch->move -= 10-sklv/3;
						hit -= 10-sklv/3;
						move -= 10-sklv/3;
						break;
					case WARLORD:
						ch->hit -= 15-sklv/3;
						hit -= 15-sklv/3;
						move -= 5-sklv/3;
						ch->move -= 5-sklv/3;
						break;
					case RANGER:
						ch->hit -= 10-sklv/3;
						hit -= 10-sklv/3;
						move -= 20-sklv/3;
						ch->move -= 20-sklv/3;
				}
				//if( hit < 0 || move < 0 ) break;
				if( ch->hit < 0 ){
					// mar ��blood ���A�U hp 1 �ɨ̵M��X���㪺 frenzy 2021/12/22
					if ( isBloodThirsty(ch) )
					{
						ch->hit = 1;
					}else{
						ch->hit = 1;
						break;
					}
				}
				if( ch->move < 0 )
					break;
				if( wait > 299 )
					break;
			}
		}
	}

	// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

	if ( !IS_NPC(ch) && ch->class == MARTIALMAN )
		WAIT_STATE( ch, skill_table[gsn_frenzy].beats + wait + (i)/2);
	else
		WAIT_STATE( ch, skill_table[gsn_frenzy].beats + wait + (i));

	return;
}

/* Bashhead skill added by Amenda, 00/12/9 */
void do_bashhead( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char		arg [ MAX_INPUT_LENGTH ];
    int			sklv = get_skill_level(ch, gsn_bashhead);
	int         old_dam;
    //int			dam;
    //int			reduce_bonus = 0;
    if ( IS_NPC( ch ) 
            && ( IS_AFFECTED( ch, AFF_CHARM ) 
                || IS_AFFECTED( ch, AFF_SUMMONED ) 
                || ch->clan ) )
        return;

    if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_bashhead] == 0 )
    {
        send_to_char("����H\n\r", ch );
        return;
    }

    one_argument( argument, arg );

	// �קאּ pc �ޯ൥�� 5 �H�U�u��b�԰����ϥ� 2022/11/21
    if ( !IS_NPC(ch) && !ch->fighting && sklv < 5 )
    {
        send_to_char( "�A�ä��O�b�԰���.\n\r", ch );
        return;
    }

	// safe room �L�k�ϥ� 2021/12/22
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}

	if( !IS_NPC(ch) )
	{
		if( ch->pcdata->autocombat == AUTOC_STOP
			|| ch->pcdata->autocombat == AUTOC_CONCENTRATION
			|| ch->pcdata->autocombat == AUTOC_PARRY
			|| ch->pcdata->autocombat == AUTOC_DODGE
			|| ch->pcdata->autocombat == AUTOC_INSTANT)
		{
			return;
		}
	}
    if( is_affected(ch, gsn_stone_form) ){
        send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
        return;
    }
	/*
    if ( ( ch->class == MONK ) && !IS_SET( ch->act, PLR_FIST_FIGHT ) )
        reduce_bonus = 15;
    */
	if( !IS_NPC(ch)){
		if (!IS_SET( ch->act, PLR_FIST_FIGHT )
			&& !get_eq_char( ch, WEAR_WIELD )
			&& !get_eq_char( ch, WEAR_WIELD_2 ) 
			&& !get_eq_char( ch, WEAR_TWO_HAND ) )
		{
			send_to_char( "�A�ܤ֥�������Z��.\n\r", ch );
			return;
		}
	}

    if ( !IS_NPC(ch) && ch->move < 300 )
    {
        send_to_char( "�Aı�o�Ӳ֤F.\n\r", ch );
        return;
    }

	/*if ( !IS_NPC( ch ) 
			&& number_percent( ) > sklv*3 + get_skill_percent(ch, gsn_bashhead) )
	{
		send_to_char( "�u���y�I�s�o���ޥ��]����.\n\r", ch );
		return;
	}*/

	/*  �קאּ�P kick �ۦP�P�w 2021/12/19
    if ( arg[0] != '\0' )
    {
        CHAR_DATA * vch;

        if ( !( vch = get_char_room( ch, arg ) ) )
        {
            send_to_char( "�o�ӤH���b�o�̰ڡH\n\r", ch );
            return;
        }
		victim = vch;

		// 2021/12/19 �����@�w�n����e�ؼ�

        if ( vch != victim)
        {
            send_to_char( "�٬O���ѨM�A���諸�ĤH�a.\n\r", ch );
            return;
        }
    }*/

    if ( arg[0] != '\0' )
    {
        CHAR_DATA * vch;

        if ( is_affected( ch, gsn_berserk ) && ( vch = get_char_room( ch, arg ) ) != victim ) 
        {
            send_to_char( "�A�����o�����A�u�Q���겴�e���ĤH�I\n\r", ch );
            return;
        }

        if ( !( vch = get_char_room( ch, arg ) ) )
        {
            send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
            return;
        }

		victim = vch;
		
    }else if( ch->fighting ){
		victim = ch->fighting;
	}else{
		send_to_char("�A�Q����Y���r���H\n\r", ch);
		return;
	}


    /* in order to reduce the power of bashhead, we use WEAR_WIELD_2 here */
    //ch->move -= 20;
    //sklv = ( IS_NPC(ch) ? 5 : get_skill_level(ch, gsn_bashhead) );

/*   old version
    if( ( ( ( ( sklv == 6 ) ? 18: ( ( sklv == 5 ) ? 8 : 0 ) ) 
                    + ( sklv * 2 + get_curr_dex( ch ) * 2 ) 
                    + ( ( get_curr_dex( ch ) > 21 ) 
                        ? 20 : ( ( get_curr_dex( ch ) > 20 ) 
                            ? 10 : 0 ) ) ) ) 
            * weakness_percent(victim, gsn_bashhead) / 100
            > number_percent() + reduce_bonus + get_curr_dex( victim ) / 2 )
    {
        ch->move = ch->move - 60 + sklv * 4;

        //mone_hit( ch, victim, gsn_bashhead, WEAR_WIELD );
        dam = ( 50 * ( sklv + 1 ) ) + get_curr_str( ch ) * ( sklv + 1 ) 
            + get_curr_dex( ch ) * ( sklv / 2 ) - get_curr_con( victim ) * 5;
        dam = dam * weakness_damage(victim, gsn_bashhead) / 100;

        if ( sklv > 3 && victim->position > POS_STUNNED && 
                ((ch->class == MONK) ? ( (sklv==6) ? 8 : ( (sklv == 5) ? 5 : 3 ) ) + get_curr_str(ch) / 2 + 
                 UMAX( 8 ,( race_table[ ch->race ].size - race_table[ victim->race ].size ) * 8 )
                 : ( race_table[ ch->race ].size - race_table[ victim->race ].size ) * 8 )
                > ( number_percent() * weakness_percent(victim, gsn_bashhead) / 100 ) - reduce_bonus + 5 - ( ( sklv == 6 ) ? sklv * 2 : ( ( sklv == 5 ) ? sklv : sklv / 2 ) ) )
        {
            //number_range(dam * sklv / 2, dam * sklv / 3);
            if( number_percent() < 5 )
                dam = dam * sklv / 2;
            dam -= reduce_bonus * 10;
            //damage( ch, victim, number_range( dam * sklv / 3, dam * sklv * 2 / 3 ), gsn_bashhead, WEAR_WIELD );
            damage( ch, victim, number_range( dam * sklv / 3, dam * sklv * 2 / 3 ), gsn_bashhead, WEAR_WIELD, MSG_NORMAL, DMG_NORMAL );
            victim->position = POS_STUNNED;
            WAIT_STATE( victim, skill_table[gsn_bashhead].beats + ( sklv * 3 / 2 ) );
            send_to_char( "�z�I�A���Y���r����L���w�F�I\n\r", ch );
            act( "$n �@�O�r�P���Y���� $N ���w�F�I", ch, NULL, victim, TO_ROOM );
            act( "$n ���Y���r�����o�A�ѱۦa��I", ch, NULL, victim, TO_VICT );
        }
        else
        {
            dam = dam * ( ( sklv < 4 ) ? 2 : 1 );
            dam -= reduce_bonus * 3;
            //damage( ch, victim, number_range( dam / 5 , dam ), gsn_bashhead, WEAR_WIELD );
            damage( ch, victim, number_range( dam / 5 , dam ), gsn_bashhead, WEAR_WIELD, MSG_NORMAL, DMG_NORMAL );
        }
    }
*/

	if ( !IS_NPC(ch) )
		ch->move -= 250;
	
	old_dam = ch->damroll;

    if( (!IS_NPC(ch) && ((get_skill_percent(ch, gsn_bashhead)/2 + sklv*6) * weakness_percent(victim, gsn_bashhead)/100)
			> number_percent() + (get_curr_dex(ch) - get_curr_dex(victim)))
		|| (IS_NPC(ch) && ch->level > number_percent() + (get_curr_dex(ch) - get_curr_dex(victim))) )
    {
		/*  �] bashhead ���R���P�_�����D , stun �P�w����ˮ`�P�w�U 2020/11/01

        if ( sklv > 3 && victim->position > POS_STUNNED && 
                ((ch->class == MONK) ? ( (sklv==6) ? 8 : ( (sklv == 5) ? 5 : 3 ) ) + get_curr_str(ch) / 2 + 
                 UMAX( 8 ,( race_table[ ch->race ].size - race_table[ victim->race ].size ) * 8 )
                 : ( race_table[ ch->race ].size - race_table[ victim->race ].size ) * 8 )
                > ( number_percent() * weakness_percent(victim, gsn_bashhead) / 100 ) + 5 - ( ( sklv == 6 ) ? sklv * 2 : ( ( sklv == 5 ) ? sklv : sklv / 2 ) ) )
        {
			ch->damroll += ch->damroll * sklv/12;
			mone_hit( ch, victim, &gsn_bashhead, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			ch->damroll = old_dam;
            victim->position = POS_STUNNED;
            WAIT_STATE( victim, skill_table[gsn_bashhead].beats );
            send_to_char( "�z�I�A���Y���r����L���w�F�I\n\r", ch );
            act( "$n �@�O�r�P���Y���� $N ���w�F�I", ch, NULL, victim, TO_ROOM );
            act( "$n ���Y���r�����o�A�ѱۦa��I", ch, NULL, victim, TO_VICT );
        }else{
            ch->damroll += ch->damroll * sklv/15;
			mone_hit( ch, victim, &gsn_bashhead, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			ch->damroll = old_dam;
		} */
		if( IS_NPC(ch) )
			ch->damroll += (ch->damroll * ch->level)/200;
		else
			ch->damroll += ch->damroll * sklv/15;

		// �˳Ʀ��n�Z���B�L�˳ƥD�n������Z��(�z�פW���|�o��) �� , �P�w�Z���ର���n�Z�� 2020/11/01
		if( get_eq_char( ch , WEAR_WIELD_2 )
			&& !get_eq_char( ch , WEAR_WIELD )
			&& !get_eq_char( ch , WEAR_TWO_HAND ))
		{
			//�קאּ one_hit , �� bashhead ��Q protect 2021/12/21
			//mone_hit( ch, victim, &gsn_bashhead, WEAR_WIELD_2, MSG_NORMAL, DT_NOCHANGE );
			one_hit( ch, victim, gsn_bashhead, WEAR_WIELD_2 );
		}else{
			//mone_hit( ch, victim, &gsn_bashhead, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			one_hit( ch, victim, gsn_bashhead, WEAR_WIELD );
			// mar blood thirsty ���A�U�[���o�� 2021/12/22
			if ( isBloodThirsty(ch) )
				one_hit( ch, victim, gsn_bashhead, WEAR_WIELD );
		}
		ch->damroll = old_dam;
    }
	else
	{
		// bashhead ����
		c_dam_message( ch, victim, -1, gsn_bashhead, WEAR_WIELD );
	}

	// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

	// �w�� artifact �����p, ���Q sleep ��charm ���ؼз|�T�갱�����
	/*if( IS_AFFECTED( victim, AFF_SLEEP )) return;
	if( IS_AFFECTED( ch, AFF_SLEEP )) return;
	if( IS_AFFECTED( victim, AFF_CHARM )) return;
	if( IS_AFFECTED( ch, AFF_CHARM )) return; */
	if( CheckSleepCharm( ch , victim ) ) return; // 2022/12/31
	
	// ���ަ��S����, ���n�}�� 2022/11/21
	if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
		stop_fighting(victim, FALSE);

	// mar blood thirsty ���A�Ustun ���C 2021/12/22
	if ( isBloodThirsty(ch) )
	{
		WAIT_STATE( ch, 10 - sklv - get_skill_level(ch, gsn_blood_thirsty)/2 );
	}
	else
		WAIT_STATE( ch, 10 - sklv );

    return;
}


void check_alignment_eq( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	for ( obj = ch->carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;
	    if ( obj->deleted )
		continue;
	    if ( obj->wear_loc == WEAR_NONE )
		continue;

	    if ( ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_EVIL ( ch ) )
		  || ( IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) && IS_GOOD ( ch ) )
		  || ( IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL ) && IS_NEUTRAL( ch ) ) )
	    {
			act( "�A�Q $p ���ƥ��F.", ch, obj, NULL, TO_CHAR );
			act( "$n �Q $p �ƥ��F.",   ch, obj, NULL, TO_ROOM );
			unequip_char(ch, obj);
			//obj_from_char( obj );
			//obj_to_room( obj, gch->in_room );
	    }
	}
}


void do_stare( CHAR_DATA *ch, char *argument )/*{{{*/
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	char       arg [ MAX_INPUT_LENGTH ];
	int sklv = get_skill_level(ch, gsn_inborn_ability);

	one_argument( argument, arg );

	if( ch->race != race_lookup("Beholder")
		&& ch->race != race_lookup("God")){
		send_to_char("����H\n\r", ch);
		return;
	}

	if( !( victim = get_char_room( ch, arg ) ) )
	{
		send_to_char( "�A�䤣��A���y���C\n\r", ch );
		return;
	}

	if( ch == victim ) {
		send_to_char("�Aı�o�ۤv�u�O�g�H���F�I\n\r", ch);
		return;
	}

	if( is_safe( ch, victim ) ) return;

	if( !IS_NPC( victim )
		&& ( !registered( ch, victim )
			|| ( !licensed( ch ) && victim->race != race_lookup( "Vampire" ) ) ) )
		return;

	/* Don't allow charmed mobs to do this */
	if( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
		|| IS_AFFECTED( victim, AFF_CHARM ) )
		return;

	if( IS_SET( victim->special, NO_CHARM )
		||(( IS_NPC( victim ) && ( victim->pIndexData->pShop ))))
	{
		multi_hit( victim, ch, TYPE_UNDEFINED );
		return;
    }

	if ( get_trust( ch ) >= L_APP && get_trust( ch ) < L_SEN )
	{
		send_to_char( "�H�A����O�ä��ݭn�y�b�O���ͪ��C\n\r", ch );
		return;
	}

	if ( ch->move > 200 )
		ch->move -= 200;
	else
	{
		send_to_char( "�N��O�����A�]�n��@����O���C\n\r", ch );
		return;
	}


	send_to_char( "�A�Ψ��������A���y���I\n\r", ch );

	if ( saves_spell( ch->level + sklv , victim ) > number_percent() + 20)
	{
		// charm
		if ( ch->level >= victim->level - 1 - sklv
			&& !IS_SET( victim->special, NO_CHARM ) )
		{
			if ( victim->master )
			stop_follower( victim );
			add_follower( victim, ch );
			af.type	 = gsn_charm_person;
			af.duration	 = number_fuzzy( (ch->level + sklv) / 4 + 2 );
			af.location	 = APPLY_NONE;
			af.modifier	 = 0;
			af.bitvector = AFF_CHARM;
			affect_to_char( victim, &af );
			act( "$n �ݰ_�ӯu�O�Ӧ��y�O�F�C", ch, NULL, victim, TO_VICT );
			if ( victim->position == POS_FIGHTING )
				stop_fighting( victim, TRUE );
			//Added by Razgriz    20050714
			if ( IS_NPC ( victim ) )
			{
				sprintf ( arg, "self roc_mob_charmed -1 %d 0 0 0 1", victim->in_room->vnum );
				do_mpadd_qmark ( victim, arg );
			}
			WAIT_STATE( ch, 12 );
			return;
		}
		// sleep
		if ( ch->level >= victim->level - 2 - sklv
			&& !IS_SET( victim->special, NO_SLEEP ) )
		{
			af.type	 = gsn_sleep;
			af.duration	 = 4 + sklv;
			af.location	 = APPLY_NONE;
			af.modifier	 = 0;
			af.bitvector = AFF_SLEEP;
			affect_join( victim, &af );

			if ( IS_AWAKE( victim ) )
			{
				send_to_char( "�@�ѺηNŧ��..... zzzzzz.\n\r", victim );
				if ( victim->position == POS_FIGHTING )
					stop_fighting( victim, TRUE );
				do_sleep( victim, "" );
			}
			WAIT_STATE( ch, 12 );
			return;
		}
		/*
		// blindness
		if ( ch->level >= victim->level - 4 - sklv
			&& !IS_SET( victim->special, NO_BLIND ) )
		{
			af.type	 = gsn_blindness;
			af.duration	 = 1 + sklv/2;
			af.location	 = APPLY_HITROLL;
			af.modifier	 = -(4 + sklv*3);
			af.bitvector = AFF_BLIND;
			affect_to_char( victim, &af );

			act( "$N ��M���h�F���O�I", ch, NULL, victim, TO_CHAR	);
			send_to_char( "�A�����e�@�¡A�A����]�ݤ���F�I\n\r", victim );
			act( "$N ��M���h�F���O�I", ch, NULL, victim, TO_NOTVICT );
			WAIT_STATE( ch, 12 );
			return;
		}
		*/
		// crazy
		if ( ch->level >= victim->level - 3 - sklv
			&& !IS_SET( victim->special, NO_CRAZY ) )
		{
			af.type	 = skill_lookup("demon tremor");
			af.bitvector = AFF_CRAZY;
			af.duration	 = sklv/3;

			af.location	 = APPLY_DAMROLL;
			af.modifier	 = sklv*2;
			affect_to_char( victim, &af );

			af.location   = APPLY_HITROLL;
			af.modifier   = -sklv*3;
			affect_to_char( victim, &af );

			af.location   = APPLY_INT;
			af.modifier   = -sklv;

			act( "$N ����A�������������J�ƨg���A�I", ch, NULL, victim, TO_CHAR	);
			send_to_char( "�A���쨸�������A��M�����������J�ƨg���A�I\n\r", victim );
			act( "$N ���쨸���������J�ƨg���A�I", ch, NULL, victim, TO_NOTVICT );
			WAIT_STATE( ch, 12 );
			return;
		}
	}
	send_to_char("�L�����A�������v�T�C\n\r", ch);
	WAIT_STATE( ch, 12 );
	multi_hit( victim, ch, TYPE_UNDEFINED );
	return;
}/*}}}*/

// circular modified by jye 6.12.2005
void do_circular( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch, *next_vch;
	OBJ_DATA *obj;
	int wp;
	int wp2 = 0;
	int two = 0;
	int wait;
	int hr_bonus = 0 ,bonus_judge = 0;
	int sklv = get_skill_level(ch,gsn_circular);
	int nTacticSklv = get_skill_level(ch,gsn_tactics);
	int nDualCir = 0;  // 2022/05/03
    
	// �P�w circular wp2 ���X�{�v 2022/05/03
	if( IS_NPC(ch) )
		nDualCir = ch->level;
	else
		nDualCir = 3 + get_skill_percent(ch, gsn_dual)/2 + get_skill_level(ch, gsn_dual)*6;
	
	/* Don't allow charmed mobs to do this, check player's level */
	if ( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
			|| ( !IS_NPC( ch ) && get_skill_percent(ch, gsn_circular) + sklv*3 < number_percent( ) ) )
	{
		send_to_char( "�A���ѤF�C\n\r", ch);
		return;
	}
	
	// �[�J autoc dodge, parry, instant �@�ԼҦ����P�w 2022/05/03
	if( !IS_NPC(ch) ){
		if( ch->pcdata->autocombat == AUTOC_STOP
			|| ch->pcdata->autocombat == AUTOC_DODGE
			|| ch->pcdata->autocombat == AUTOC_PARRY
			|| ch->pcdata->autocombat == AUTOC_CONCENTRATION
			|| ch->pcdata->autocombat == AUTOC_INSTANT )
			return;
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	if ( !check_blind( ch ) || !ch->fighting ){
		send_to_char("�A�䤣��i�H�������ؼСI\n\r", ch);
		return;
	}

	if ( !IS_NPC(ch) && !(obj = get_eq_char( ch, WEAR_WIELD )) && !(obj = get_eq_char( ch, WEAR_TWO_HAND )) )
	{
		if ( !(obj = get_eq_char( ch, WEAR_WIELD_2 )) )
		{
			send_to_char( "�n�﮳��Z���a�H\n\r", ch );
			return;
		}
		wp = WEAR_WIELD_2;
	}
	else
		wp = WEAR_WIELD;

	if( get_eq_char( ch, WEAR_WIELD_2 ))
		wp2 = WEAR_WIELD_2;
	else if ( IS_NPC(ch))
		wp2 = WEAR_WIELD;
	if( get_eq_char( ch, WEAR_TWO_HAND))
		two = 1;

	if( !IS_NPC(ch) ) 
	{
		if ( ch->move < 100 + 22*sklv )
		{
			send_to_char( "�A�S����������O�C\n\r", ch );
			return;
		}
	}

	//modified the hr bonus with ppl group, let only lord can give hr bonus
	//modified by coco 0613
	if(!IS_NPC(ch) && ch->class == WARLORD)
	{
		for(vch= ch->in_room->people; vch; vch = vch->next_in_room)
		{
			if(!vch->deleted)
			{
				if( is_same_group(vch,ch) && !IS_NPC(vch) && vch->class == LORD )
				{
					bonus_judge = 1;
				}
				else if(is_same_group(vch->fighting,ch))
					bonus_judge = 2;
			}
		}
	}

	/* old version sklv 6 hr bonus max = 17.6
	hr_bonus += sklv+ sklv/2 + (sklv/5)*3 + (sklv/6)*5; */

	hr_bonus = 1;

	if(bonus_judge == 1)
		hr_bonus = ch->hitroll * (30 + sklv*6 + sklv*sklv)/100;
	else if(bonus_judge == 2)
		hr_bonus = ch->hitroll * (15 + sklv*5 + sklv*sklv)/100;
	else
		hr_bonus = ch->hitroll * (15 + sklv*4 + sklv*sklv)/100;

	if(hr_bonus < 0)
		hr_bonus = 0;

	if( !IS_NPC(ch) ) ch->hitroll += hr_bonus;

	if(hr_bonus > 0)
		send_to_char(HIW"�A�H�ۨ������ߡA�V�|���K������I\n\r"NOR"", ch);

	if( !IS_NPC(ch) ) ch->move -= 100 + 22*sklv;

	wait = ch->wait;
	for ( vch = ch->in_room->people; vch; vch = next_vch )
	{
		next_vch = vch->next_in_room;
		// 0940606 modified by jye
		// �ק� vch->fighting != ch �� vch = ch->fighting �Ϥ��Xcircular
		if ( ch!=vch && !vch->deleted && ( is_same_group(vch->fighting ,ch) || (ch->fighting == vch) ) )
		{
			/* old version 
				if( !IS_NPC(ch) && ( obj->value[3] == 13 || number_percent() < 
												get_skill_percent(ch, gsn_circular)/2)+sklv*2) */
			if( !IS_NPC(ch) 
				&& ( number_percent() < (get_skill_percent(ch, gsn_circular)/3) + sklv*(13 + sklv)/2 
										+ get_curr_int(ch)/(7 - nTacticSklv) ) )
			{
				// wear_wield_2 �s�b gsn �a�J��L�k���������D
				//multi_hit(ch, vch, gsn_circular);
				if( wp2 ){
					one_hit( ch, vch, gsn_circular, wp );
					one_hit( ch, vch, gsn_circular, wp2 );
					one_hit( ch, vch, gsn_circular, wp );
					// �ĤG�� wp2 �X�{�v�� dual �v�T 2022/05/03
					if( nDualCir >= number_percent() )
						one_hit( ch, vch, gsn_circular, wp2 );
					// mar blood thirsty ���A�U�[���o��, ���M�z�פW mar ���| dual
					if ( isBloodThirsty(ch) )
					{
						one_hit( ch, vch, gsn_circular, wp );
						if( nDualCir >= number_percent() )
							one_hit( ch, vch, gsn_circular, wp2 );
					}
					if(sklv > 3){
						one_hit( ch, vch, gsn_circular, wp );
					}
					if(sklv > 4){
						one_hit( ch, vch, gsn_circular, wp2 );
					}
					if(sklv > 5){
						one_hit( ch, vch, gsn_circular, wp );
						if( nDualCir >= number_percent() )
							one_hit( ch, vch, gsn_circular, wp2 );
					}
				}else{
					one_hit( ch, vch, gsn_circular, wp );
					one_hit( ch, vch, gsn_circular, wp );
					// mar blood thirsty ���A�U�[���o�� 2021/12/22
					if ( isBloodThirsty(ch) )
					{
						one_hit( ch, vch, gsn_circular, wp );
					}
					if(sklv > 3)
						one_hit( ch, vch, gsn_circular, wp );
					if(sklv > 5)
						one_hit( ch, vch, gsn_circular, wp );
				}
			}else{
				if( wp2 ){
					one_hit( ch, vch, gsn_circular, wp );
					// npc �� wp2 �X�{�v�Ҩ����żv�T
					if( nDualCir + 20 >= number_percent() )
						one_hit( ch, vch, gsn_circular, wp2 );
					if(sklv > 3)
						one_hit( ch, vch, gsn_circular, wp );
					if( nDualCir >= number_percent() && sklv > 5 )
						one_hit( ch, vch, gsn_circular, wp2 );
				}else{
					one_hit( ch, vch, gsn_circular, wp );
					if(sklv > 3)
						one_hit( ch, vch, gsn_circular, wp );
				}
			}
			/* wait = ch->wait;
				ch->wait = 0;
				if( wp2 ){
					one_hit( ch, vch, gsn_circular, wp2 );
					if(sklv > 5)
					   one_hit( ch, vch, gsn_circular, wp2 );
				}
				else if ( two )
					one_hit( ch, vch, gsn_circular, wp );

				wait = ch->wait;
				ch->wait = 0;*/

			// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
			if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
				LizardmanSkill( ch , vch , 1);
		}
	}

	ch->wait = 0;
	if( !IS_NPC(ch) ) ch->hitroll -= hr_bonus;

	WAIT_STATE(ch, wait);
	// mar blood thirsty ���A�Ustun���C 2021/12/25
	if ( isBloodThirsty(ch) )
	{
		WAIT_STATE( ch, skill_table[gsn_circular].beats - get_skill_level(ch, gsn_blood_thirsty) );
	}
	else
		WAIT_STATE( ch, skill_table[gsn_circular].beats );
}

void do_meteoric_arrow( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *gch, *victim;
	OBJ_DATA  *obj;
	OBJ_DATA *arrow;
	int wait = 0;
	int times = 0; // �y�P�b�o�ƭp�� 2022/05/02
	int sklv = get_skill_level(ch, gsn_meteoric_arrow);
	int nRideSklv = get_skill_level(ch, gsn_ride);
	int nTrickySklv = get_skill_level(ch, gsn_tricky_pacing);
	char arg[MAX_INPUT_LENGTH];
	int dt_undefined = TYPE_UNDEFINED; //Added by Razgriz 20051014

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		nRideSklv = get_skill_level(ch, gsn_inborn_ability) + 1;
	}
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) ){
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}

      /* Don't allow charmed mobs to do this, check player's level */
	if( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		|| ( !IS_NPC( ch ) && get_skill_percent(ch, gsn_meteoric_arrow) < number_percent( ) ) )
	{
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	if( !( victim = ch->fighting )){
		//  send_to_char( "�A���O�b�԰����C\n\r", ch );
		//  return;  
		one_argument(argument, arg);
		if( arg[0] == '\0' || ! (victim = get_char_room(ch, arg)))
		{
			send_to_char("�A�䤣��ؼСC\n\r", ch);
			return;
		}
	}

	if ( !IS_NPC( ch ) && IS_SET( ch->pcdata->autocombat, AUTOC_STOP ) ){
		send_to_char( "�A�õL�N�԰�.\n\r", ch );
		return;
	}
      
	if( !IS_NPC( ch ) ){
		if (number_percent() < nTrickySklv * 25){
			send_to_char( "�F��������A���A�s�y�X�Ԥ}���žסC\n\r", ch);
		}else{
			if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
				if(number_percent() > (nRideSklv - 2) * 75 || !get_eq_char( ch , WEAR_RIDE )){
					for( gch = ch->in_room->people; gch; gch = gch->next_in_room ){
						if( gch->fighting == ch )
						{
							send_to_char( "�A�Q���o�⦣�}�áA�S�ɶ��Ԥ}�C\n\r", ch );
							WAIT_STATE( ch, skill_table[gsn_meteoric_arrow ].beats * 2 / 3);
							return;
						}
					}
				}else{
					if (number_percent() > nRideSklv * 25 + 50) {
						if( gch->fighting == ch )
						{
							send_to_char( "�A�Q���o�⦣�}�áA�S�ɶ��Ԥ}�C\n\r", ch );
							WAIT_STATE( ch, skill_table[gsn_meteoric_arrow ].beats * 2 / 3);
							return;
						}
					}
					send_to_char( "�A���Q�a�B���M�N�Զ}�F�P�Ĥ誺�Z���A�ϧA�o�H�Ԥ}�I\n\r", ch);
				}
			}else{
				send_to_char( "�A�B�ΤH����ʤW���u�աA�Զ}�F�P�Ĥ誺�Z���I\n\r", ch);
			}
		}
	}
	if( !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) || obj->value[3] != 14 ){
		send_to_char( "�n�﮳��}�a�H\n\r", ch );
		return;
	}

    if( !( arrow = get_eq_char( ch, WEAR_ARROW ) ) || arrow->value[0] == 0 ){
		send_to_char( "���A�S�b�F!\n\r", ch );
		return;
	}

	if( !IS_NPC(ch) && ch->move < 150 + sklv * 30){
		send_to_char( "�Aı�o�n�֡I\n\r", ch );
		return;
	}
	
	if( !IS_NPC(ch) ) ch->move -= 150 + sklv * 30;

	act( "$n���}�@�g�A�\\�h�b�ڦp�y�P��V$N���b�ӥh�I", ch, NULL, victim, TO_ROOM );
	send_to_char( "�A���}�@�g�A�\\�h�b�ڦp�y�P��g�X�I\n\r", ch );

	// �Nnpc �o�ƻP pc ���}, �קK�ϥ� mete ���a������o�ƹL�� (fight_prog ���߷�U���X����) 2021/10/24
	//if( victim && !victim->deleted )


	// �w�� �y�P�b �b�L�b���p�U�ܬ� �y�P�� �����D�@�ץ�    2022/05/02
	// �� multi_hit �N�J�� dt ���ܬ� gsn_meteoric_arrow
	// �ξ�Ҧ� mone_hit �H while �ӭp��C�o����

	if(!IS_NPC(ch)){     // pc �o�ƭp�� 2021/10/24

		// �y�P�b mone_hit �έp�o��
		times = 6;  // base mone hit
		if( sklv >= 2 && number_percent() <= 60 ) times += 1;
		if( sklv >= 3 ) times += 3;
		if( sklv >= 4 ) times += 3;
		if( sklv >= 5 ) times += 4;
		if( sklv == 6 ) times += 4;

		// �y�P�b mone_hit ����X 2022/05/02
		if (victim && !victim->deleted) {
			while( times-- )
			{
				if (victim == NULL || victim->deleted) break;
				// mone_hit �C�U������|�έp�b�ڶq , �p�G�����b���N�ఱ����� 2022/05/02
				if( !( arrow = get_eq_char( ch, WEAR_ARROW ) ) || arrow->value[0] == 0 ) break;
				mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
				wait += ch->wait;
				ch->wait = 0;
			}
		}

		// �y�P�b multi_hit ����X 2022/05/02
		//if(victim && !victim->deleted) multi_hit( ch, victim, TYPE_UNDEFINED );
		if(victim && !victim->deleted) multi_hit( ch, victim, gsn_meteoric_arrow );  // base multi hit
		wait += ch->wait;
		ch->wait = 0;

		if( sklv >= 1 && victim && !victim->deleted )
		{
			if(number_percent() <= 40 && victim && !victim->deleted){
				//multi_hit( ch, victim, TYPE_UNDEFINED );
				multi_hit( ch, victim, gsn_meteoric_arrow );
				wait += ch->wait;
				ch->wait = 0;
			}
		}

		// sklv �B�~���v�[���o��
		// 7.00 7.25 7.75 8.50 10.0 13.0 18.0
		// 7.00 7.40 8.00 9.00 11.0 15.0 22.0
		if (victim && !victim->deleted) {
			int nBonusHit = 0;
			// old version int nMaxBonusHit = nRideSklv * nRideSklv / 6 + nRideSklv;
			int nMaxBonusHit = sklv * sklv / 6 + sklv;
			for (nBonusHit = 0; nBonusHit <= nMaxBonusHit; nBonusHit++) {
				if (victim == NULL || victim->deleted) break;
				// mone_hit �C�U������|�έp�b�ڶq , �p�G�����b���N�ఱ����� 2022/05/02
				if( !( arrow = get_eq_char( ch, WEAR_ARROW ) ) || arrow->value[0] == 0 ) break;
				if (number_percent() <= 80) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			}
			ch->wait = 0;
		}
	}

	if( IS_NPC(ch) ){        // npc sklv �o�ƭp�� 2021/10/24
		// npc base �o��
		if( victim && !victim->deleted ) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		if( victim && !victim->deleted ) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		// sklv �o�ưl�[
		if(sklv >= 1){
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		}
		if(sklv >= 2){
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		}
		if(sklv >= 3){
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		}
		if(sklv >= 4){
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		}
		if(sklv >= 5){
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		}
		if(sklv == 6){
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
			if(victim && !victim->deleted) mone_hit( ch, victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		}
		wait += ch->wait;
		ch->wait = 0;
	}

	if( !ch->fighting ) set_fighting(ch, victim);

	if( ch->temp_char ) // amethyst mete modify
	{
		stop_fighting( ch->temp_char, FALSE );
		set_fighting(ch->temp_char, ch);
		ch->temp_char = FALSE;
	}

	if( sklv < 5 || ch->level < 42 ){
		WAIT_STATE( ch, skill_table[gsn_meteoric_arrow].beats );
	}else{
		WAIT_STATE( ch, skill_table[gsn_meteoric_arrow ].beats );
		WAIT_STATE( ch, skill_table[gsn_meteoric_arrow ].beats / 3);
	}

	WAIT_STATE(ch, wait);

	return;
}

// �b�B, ���ޯ�P�y�P�b�P�w�ۦP, �u���ؼФ��P 2022/05/02
void do_rainfall_arrow( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *gch;
	CHAR_DATA *victim;
	CHAR_DATA *victim_next;
	CHAR_DATA *rand_victim;
	OBJ_DATA  *obj;
	OBJ_DATA *arrow;
	int wait = 0;
	int times = 0;
	int EnemyCount = 0;
	int TotalSize = 0;
	int sklv = get_skill_level(ch, gsn_meteoric_arrow);
	int nRideSklv = get_skill_level(ch, gsn_ride);
	int nTrickySklv = get_skill_level(ch, gsn_tricky_pacing);
	int dt_undefined = TYPE_UNDEFINED;

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		nRideSklv = get_skill_level(ch, gsn_inborn_ability) + 1;
	}
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) ){
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}
	if( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
		|| ( !IS_NPC( ch ) && get_skill_percent(ch, gsn_rainfall_arrow) < number_percent( ) ) )
	{
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}
	if ( !IS_NPC( ch ) && IS_SET( ch->pcdata->autocombat, AUTOC_STOP ) ){
		send_to_char( "�A�õL�N�԰�.\n\r", ch );
		return;
	}
	if( !IS_NPC( ch ) ){
		if (number_percent() < nTrickySklv * 25){
			send_to_char( "�F��������A���A�s�y�X�Ԥ}���žסC\n\r", ch);
		}else{
			if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
				if(number_percent() > (nRideSklv - 2) * 75 || !get_eq_char( ch , WEAR_RIDE )){
					for( gch = ch->in_room->people; gch; gch = gch->next_in_room ){
						if( gch->fighting == ch )
						{
							send_to_char( "�A�Q���o�⦣�}�áA�S�ɶ��Ԥ}�C\n\r", ch );
							WAIT_STATE( ch, skill_table[gsn_rainfall_arrow].beats * 2 / 3);
							return;
						}
					}
				}else{
					if (number_percent() > nRideSklv * 25 + 50) {
						if( gch->fighting == ch )
						{
							send_to_char( "�A�Q���o�⦣�}�áA�S�ɶ��Ԥ}�C\n\r", ch );
							WAIT_STATE( ch, skill_table[gsn_rainfall_arrow].beats * 2 / 3);
							return;
						}
					}
					send_to_char( "�A���Q�a�B���M�N�Զ}�F�P�Ĥ誺�Z���A�ϧA�o�H�Ԥ}�I\n\r", ch);
				}
			}else{
				send_to_char( "�A�B�ΤH����ʤW���u�աA�Զ}�F�P�Ĥ誺�Z���I\n\r", ch);
			}
		}
	}
	if( !( obj = get_eq_char( ch, WEAR_TWO_HAND ) ) || obj->value[3] != 14 ){
		send_to_char( "�n�﮳��}�a�H\n\r", ch );
		return;
	}
    if( !( arrow = get_eq_char( ch, WEAR_ARROW ) ) || arrow->value[0] == 0 ){
		send_to_char( "���A�S�b�F!\n\r", ch );
		return;
	}
	if( !IS_NPC(ch) && ch->move < 150 + sklv * 30){
		send_to_char( "�Aı�o�n�֡I\n\r", ch );
		return;
	}
	
	if( !IS_NPC(ch) ) ch->move -= 150 + sklv * 30;

	act( "$n�¤W���}�@�g�A�b�������a�b�p�B�U�I", ch, NULL, NULL, TO_ROOM );
	send_to_char( "�A�¤W���}�@�g�A�b�������a�b�p�B�U�I\n\r", ch );

	// pc �o�ƭp��
	if(!IS_NPC(ch)){
		// times total = 15 , 7 ~ 15
		times = 7;
		// multi hit
		if(number_percent() <= 95)
			times += 1;
		if(number_percent() <= 80)
			times += 1;
		if(number_percent() <= 50)
			times += 1;
		if(number_percent() <= 25)
			times += 1;
		if( sklv >= 1 && number_percent() <= 40 )
		{
			// multi hit
			if(number_percent() <= 95)
				times += 1;
			if(number_percent() <= 80)
				times += 1;
			if(number_percent() <= 50)
				times += 1;
			if(number_percent() <= 25)
				times += 1;
		}
		// times total = 19
		if( sklv >= 2 && number_percent() <= 60 ) times += 3;
		if( sklv >= 3 ) times += 3;
		if( sklv >= 4 ) times += 4;
		if( sklv >= 5 ) times += 4;
		if( sklv >= 6 ) times += 5;
	}else{
		times = 4;
		times = times + sklv;
		if(sklv >= 5) times += 5;
		if(sklv == 6) times += 5;
	}

	// �H���� room �����ؼЩ�b����o�Ƭ� 0
	while( times-- )
	{
		EnemyCount = 0;
		TotalSize = 0;
		rand_victim = NULL;

		if( !( arrow = get_eq_char( ch, WEAR_ARROW ) ) || arrow->value[0] == 0 )
			break;

		// �ĤH�`��n�p��
		for ( victim = ch->in_room->people; victim; victim = victim_next ){
			victim_next = victim->next_in_room;

			if( victim->deleted )
				continue;

			if( is_same_group(ch, victim) )
				continue;

			if( IS_NPC(ch) ){
				if( IS_NPC(victim) && victim->fighting != ch )
					continue;
			}else{
				if( !IS_NPC(victim) && victim->fighting != ch )
					continue;
			}
			// race size max is 9. avg 3.6
			//TotalSize += 1 + race_table[victim->race].size;
			TotalSize += 1 + get_curr_size( victim ); // �����髬 size ���� 2022/12/23
		}

		// �p�G�S���ؼ�
		if( TotalSize == 0 ) break;

		// �O�ҷ|�P�w�R�����o�Ƭ� 9 - sklv ������ , �ѤU���o�ƶ]�ĤH�`��n�P�w
		if( ( times % (9 - sklv) != 0 ) && ( number_percent() > (2 * TotalSize * (12 + sklv*sklv))/12 ) ){
			if( ( arrow = get_eq_char( ch, WEAR_ARROW ) ) && arrow->value[0] > 0 ){
				arrow->value[0] -= 1;
				if( is_chinese(ch, victim) )
					send_to_char( "�A���g�X�b��[1;37m�S������[m�ؼ�.(0)\n\r", ch );
				else
					send_to_char( "your shot is [1;37mOut of Target[m.(0)\n\r", ch );
			}
			continue;
		}
		for ( victim = ch->in_room->people; victim; victim = victim_next ){
			victim_next = victim->next_in_room;

			if( victim->deleted )
				continue;

			if( is_same_group(ch, victim) )
				continue;

			if( IS_NPC(ch) ){
				if( IS_NPC(victim) && victim->fighting != ch )
					continue;
			}else{
				if( !IS_NPC(victim) && victim->fighting != ch )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_victim = victim;

			//if( number_percent() <= ((1 + race_table[victim->race].size)/TotalSize)*100 )
			if( number_percent() <= ( (1 + get_curr_size( victim ) )/TotalSize)*100 ) // �����髬 size ���� 2022/12/23
				rand_victim = victim;

			EnemyCount++;
		}
		if( !rand_victim )
			break;
		if( !( arrow = get_eq_char( ch, WEAR_ARROW ) ) || arrow->value[0] == 0 )
			break;
		mone_hit( ch, rand_victim, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
		wait += ch->wait;
		ch->wait = 0;
	}

	if( sklv < 5 || ch->level < 42 ){
		WAIT_STATE( ch, skill_table[gsn_rainfall_arrow].beats );
	}else{
		WAIT_STATE( ch, skill_table[gsn_rainfall_arrow ].beats );
		WAIT_STATE( ch, skill_table[gsn_rainfall_arrow ].beats / 3);
	}

	WAIT_STATE(ch, wait);

	return;
}

int get_charge_hit_rate(int nHitRate, int nRideSklv, int nDodge) {
	if (nRideSklv > 6) nRideSklv = 6;
	int Final_Rate;
	//nDodge = number_range(nDodge * 0.9, nDodge * 1.1);
	if(nDodge < 0){
		//return nHitRate * (1000 + nRideSklv * 150) / (1000 - nDodge * 1.2);
		Final_Rate = nHitRate + nDodge / ( 1 + nRideSklv * 3 );
	}else{
		//return nHitRate * (1000 + nRideSklv * 150 + nDodge) / 1000;
		Final_Rate = nHitRate + nDodge / ( 20 - nRideSklv );
	}
	// �����P�w 2022/04/03
	if ( nHitRate > 0 && Final_Rate < 0)
		Final_Rate = nRideSklv;

	return Final_Rate;
}

void do_charge( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA * victim,* rch  = NULL;
	OBJ_DATA         *horse   = NULL;
	OBJ_DATA         *wield   = NULL;
	ROOM_INDEX_DATA  *in_room = NULL;
	EXIT_DATA        *pexit   = NULL;

	int    dam      = -1;
	int    door     =  0;
	//true sklv of charging.
	int    nChrgSklv = get_skill_level(ch,gsn_charge);
	int    nEDmgSklv = get_skill_level(ch, gsn_enhanced_damage);
	int    nRideSklv = get_skill_level(ch,gsn_ride);
	int    nEDmPerc  = get_skill_percent(ch, gsn_enhanced_damage);
	//sklv of leading charging by lord.
	int    nLeadSklv = UMAX(nChrgSklv, (nChrgSklv + (argument?atoi(argument):0))>>1);
	bool   ringout   = FALSE;
	int    nRoomType = ch->in_room->sector_type;
	int    nDmgRate  = 100;
	// damroll add at 2022/04/03
	int    nDamRoll  = get_damroll(ch, (get_eq_char(ch, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) );
	int    nHitRoll  = get_hitroll(ch, (get_eq_char(ch, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) );
	int    nSelfDmg  = 0;
	int    nHitRate  = 0;
	int    nDodge    = 0;
	int    nArmor    = 0;
	int    nWpnAvg   = 0;
	int    nLanceBonus = GetBonusedValue_LanceMaster(ch, 100);
	int    final_dam = 0;
	int    nCurrStr  = 0;
	// charge hp trigger 2022/04/04
	int    chance    = 0;
	int    nCurrWis  = get_curr_wis(ch);

	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		nRideSklv = get_skill_level(ch, gsn_inborn_ability);
	}

	in_room = ch->in_room;

	//add by jye 0940709
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}
  
	if ( ! (horse = get_eq_char( ch , WEAR_RIDE ) ) && !IS_NPC(ch) )
	{
		if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
			send_to_char( "�A�����M�b���M�W���C\n\r",ch);
			return;
		}
	}

	if ( !in_room )
		return;

	if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ) {
		if ( ( nRoomType == SECT_HILLS && !(horse->value[1] & 3776)) // 64 | 128 | 512 | 1024 | 2048
			|| ((nRoomType == SECT_MOUNTAIN || nRoomType == SECT_FOREST ) && !(horse->value[1] & 2048) ) 
			|| ( nRoomType == SECT_WATER_SWIM && !(horse->value[1] & 1536)) // 512 | 1024
			|| ((nRoomType == SECT_WATER_NOSWIM || nRoomType == SECT_UNDERWATER) && !(horse->value[1] & 1024) )
			|| ((nRoomType == SECT_AIR || nRoomType == SECT_DESERT ) && !(horse->value[1] & 512)) )
		{
			if( nRideSklv < 5 ){
				send_to_char( "�o�ئa�εL�k�ϥγo�ا��M�ļ���I\n\r",ch);
				return;
			}else{
				// ride 5 �H�W��ϥΦa�Τ��A�X���y�M�ļ�, ���ˮ`���C 2022/02/25
				nDmgRate = nRideSklv * (nRideSklv + 4) + 30;
			}
		}
	}else{
		// centaur �a�ΧP�w
		int nInbornSklv = get_skill_level(ch, gsn_inborn_ability);
		nDmgRate = 100;
		switch(nRoomType) {
			case SECT_HILLS:
			case SECT_MOUNTAIN:
				nDmgRate = 100;
				break;
			case SECT_DESERT:
			case SECT_WATER_SWIM:
				nDmgRate = nInbornSklv * 5 + 60;
				break;
			case SECT_WATER_NOSWIM:
			case SECT_UNDERWATER:
			case SECT_AIR:
				nDmgRate = nInbornSklv * (nInbornSklv + 4) + 30;
			break;
		}
	}

	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	/* Don't allow charmed mobs to do this, check player's level */
	if ( !(victim = ch->fighting) )
	{
		send_to_char( "�A���O�b�԰����C\n\r", ch );
		return;
	}

	if ( !can_see( ch , ch->fighting ) )
	{
		send_to_char( "�A�ݤ��������I\n\r", ch);
		return;
	}

	if ( !victim )
	{
		send_to_char( "�A�Q�ļ��֡H\n\r",ch);
		return;
	}

	//add by coconet 2005/04/12 to fix charge self crash bug
	if ( victim == ch )
	{
		send_to_char( "�A�L�k�ۤv�ļ��ۤv��I\n\r",ch);
		return;
	}

	if( get_eq_char( ch , WEAR_WIELD ) )
		wield = get_eq_char(ch, WEAR_WIELD) ;
	else if( get_eq_char( ch, WEAR_WIELD_2 ) )
		wield = get_eq_char(ch, WEAR_WIELD_2) ;
	else if( get_eq_char( ch, WEAR_TWO_HAND ) )
		wield = get_eq_char(ch, WEAR_TWO_HAND );
	
	// �N�L�˳ƪZ���P�˳ƪZ�����}�P�w 2022/04/04
	//if( !wield || ( wield->value[3] != 2 && nChrgSklv != 6 ) )
	if ( !wield )
	{
		send_to_char("�����˳ƪZ���~��ϥνĨ�����C\n\r",ch);
		return;
	}

	// �[�J lance mastery �Z���P�w 2022/04/04
	if( wield->value[3] != 2 && nChrgSklv != 6 )
	{
		if( get_skill_level(ch, gsn_lance_mastery) > 5 )
		{
			if(    wield->value[3] != 1
				&& wield->value[3] != 3
				&& wield->value[3] != 7
				&& wield->value[3] != 9 )
			{
				send_to_char("�A�L�k�ϥγo�تZ���Ĩ�����C\n\r",ch);
				return;
			}
		}else{
			send_to_char("�A�L�k�ϥγo�تZ���Ĩ�����C\n\r",ch);
			return;
		}
	}

	if(strcmp(argument, "ultra") == 0){
		nDmgRate = nDmgRate * (110 + nChrgSklv * 5 + number_range(nChrgSklv , nChrgSklv * nChrgSklv)) / 100;
	}
	else if( get_skill_percent(ch, gsn_charge) + nChrgSklv * 4 < number_percent( ) )
	{
		send_to_char( "�A���ѤF�C\n\r", ch);
		return;
	}

	// ���� lv 5 �H�W�ɪ� mv cost , ucharge �����p�U�N�j�q���� mv 20220/04/04
	//if ( ch->move < 50 + nLeadSklv * 20 )
	if ( ch->move < 40 + nLeadSklv * nLeadSklv * nLeadSklv )
	{
		send_to_char( "�A����O�L�k�t���A���Ĩ�I\n\r",ch);
		return;
	}

	ch->move -= 40 + nLeadSklv * nLeadSklv * nLeadSklv;

	nWpnAvg = number_range( wield->value[1], wield->value[2] );
	if( IS_SET( wield->extra_flags, ITEM_CLAN ) && nWpnAvg > 75 )
		nWpnAvg = 75 + (nWpnAvg - 75) * 0.1;

	//dam = nWpnAvg * (get_curr_str(ch) * (nLeadSklv + 6) + ch->damroll * nLeadSklv) / 18;
	//dam = dam * get_vrace_drp(victim, wield) / 100;
	//dam += nWpnAvg * (get_vrace_drq(victim, wield) * nLeadSklv) / 18;

	dam = nWpnAvg * (1.5 + nEDmgSklv/6);
	dam += nDamRoll * (nWpnAvg + 44) * 4 * get_vrace_drp(victim, wield) / (8 * 5500);
	dam += 4 * (nWpnAvg + 44) * get_vrace_drq(victim, wield) / (8 * 55);

	// ed �[���B�z 2022/04/03
	if( !IS_NPC(ch) ){
		if( get_skill_percent(ch, gsn_enhanced_damage) + nEDmgSklv * 5 > number_percent() ){
			dam = dam * 2 + (dam * nEDmgSklv )/ 4;
			if( ch->class == KNIGHT )
				dam += (dam + dam/3);
			if( ch->class == LORD )
				dam += ( dam /3);
			if( ch->pcdata->prime_attr == APPLY_STR){
				dam += dam / 2;
			}else if( ch->pcdata->snd_attr == APPLY_STR){
				dam += dam / 3;
			}else{
				dam += dam * get_curr_str(ch) / 100;
			}
		}
	}
	
	dam = dam * (1 + nLeadSklv/4);

	/*  �@�Ҵ�קאּ�P��L�����ۦP 2022/04/03
	nArmor   = GET_AC( ch->fighting );

	if(nArmor < 0){
		dam = dam * 1000 / (1000 - nArmor);
	}else{
		dam = dam * (1000 + nArmor) / 1000;
	}
	*/

    if ( !IS_NPC( ch->fighting ) )
    {
		nArmor = GET_AC( ch->fighting ) * 0.1;
		if(GET_AC(ch->fighting) <= -1000){
			dam = dam * 3150 / ( nArmor * nArmor + 500 );
		}else{
			dam = UMIN( dam + dam * GET_AC(ch->fighting)/1429 , dam + GET_AC(ch->fighting) /8 );
		}
    }
    else
    {
		nArmor = GET_AC( ch->fighting );
		if(nArmor > 0){
			dam = dam + dam * UMIN( GET_AC(ch->fighting)/100 , 1);
		}
		else
		{
			if(nArmor < -7000){
				dam = dam + nArmor + 7000 - dam * 4 / 5;
			}
			else if(nArmor >= -2000  ){
				dam = UMIN( dam + dam * nArmor/8000 , dam + nArmor/7 );
			}
			else if(nArmor >= -4000){
				dam = dam + dam * (nArmor + 2000)/8500 - dam/4;
			}
			else {
				dam = dam + dam * (nArmor + 4000)/9000 - (dam * 20)/85 - dam/4;
			}
		}
    }

	if (ch->class == KNIGHT)
		nDmgRate += 10;

	if (!IS_NPC(ch)) {
		if (nCurrStr > 18) {
			nDmgRate += nDmgRate * (nCurrStr - 18) / 10;
		} else {
			nDmgRate -= (18 - nCurrStr);
		}
	}

	//nLeadSklv = nLeadSklv * nLanceBonus / 100;  ���� lance �� charge �ˮ`�[�� 2022/04/03
	dam += ch->apdr * (20 + nLeadSklv * 4) / 10;
	dam = dam * nDmgRate / 100;
	dam -= victim->pdr;
	if(dam < 0) dam = 0;

	nHitRate = ( nHitRoll + get_curr_dex( ch ) * 2 )
				* get_vrace_hrp(victim,wield) / 100
				+ get_vrace_hrq(victim, wield);

	//if(nRideSklv > 4) nHitRate += (nRideSklv - 4) * (nRideSklv - 3) * 2;

	if(nRideSklv >= 4)
		nHitRate = nHitRate + nHitRate/(10 - nRideSklv);

	nHitRate = nHitRate * (100 + nLanceBonus) / 200;

	nDodge   = GET_DODGE( ch->fighting );
    
	if (IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		nRideSklv = get_skill_level(ch, gsn_inborn_ability) + 1;
	}

	/*  ���� ride �� charge �ˮ`���[�� 2022/04/03
	if (!IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR )) {
		if(horse->value[1] & 64) { 
			dam += dam / 3;
		} else if(nRideSklv + nChrgSklv >= 12) {
			dam += dam / 4;
		}
	} else {
		// here, nRideSklv = centure's inborn sklv + 1, so max +35%
		dam += dam * (20 + nRideSklv) / 20;
	}
	*/

	// charge weakness_damage �]�w add at 2020/09/18
	dam = dam * weakness_damage(victim , gsn_charge) / 100;

    /* Debug Message *
    char str[200];
    sprintf(str, "nHitRoll = %d, nHitRate = %d nDodge = %d nArmor = %d\n\r", nHitRoll, nHitRate, nDodge, nArmor);
    send_to_char(str, ch);
    */

	nSelfDmg  = number_range( wield->value[1], wield->value[2] ) + ch->damroll;
	nSelfDmg  = nSelfDmg * number_range(100 - nLeadSklv * 10, 100 + nLeadSklv * 5) / 100;
	nSelfDmg += victim->pdr * 5;
	nSelfDmg -= ch->pdr * 6 + get_curr_con(ch) * 8;
	if (ch->class == KNIGHT) {
		nSelfDmg -= 100;
		nSelfDmg *= 0.9;
	}
	nSelfDmg  = nSelfDmg * (100 - nRideSklv * 5) / 100;
	if(nSelfDmg < 0) nSelfDmg = 0;

	int nHitCount = 0;

	final_dam = get_critical_damage(dam, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
	final_dam = number_range(final_dam * 0.8, final_dam * 1.2);

	// hp ���v�[�� 2022/04/04
	if( ch->class == LORD ) chance += 20;
	nCurrWis = nCurrWis * GetBonusedValue_LanceMaster(ch, 100) / 100;
	chance += (GetBonusedValue_LanceMaster(ch, 100) - 100) / 100;

	// charge weakness_percent �]�w add at 2020/09/18
	if ( get_charge_hit_rate(nHitRate, nRideSklv, nDodge) * weakness_percent(victim , gsn_charge) / 100 > number_percent() ) {
		damage( ch, victim, final_dam, gsn_charge, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		// artifact �� �Z���ӷl 2022/04/04
		if( wield )
		{
			// �W�[ obj_hit ���P�w 2022/10/23
			if ( !victim->deleted && wield->pIndexData->progtypes & OBJ_HIT )
				oprog_hit_trigger(wield);
			OBJ_HP * ohp = wield->hp;
			// artifact
			if ( wield->hp || wield->material->hp )
			{
				for( ; ohp; ohp = ohp->next )
				{
					if( IS_SET( ohp->event, HP_FIGHT ) )
					{
						if ( IS_SET( ch->act, PLR_ARTIFACT )
							&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
								+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
						{
							trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
					  
					if( IS_SET( ohp->event, HP_WEAPON ) ) {
						if ( IS_SET( ch->act, PLR_ARTIFACT )
							&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
								+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
						{
							trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
						}
					}
				}
				if( wield->material->hp )
				{
					for( ohp=wield->material->hp ; ohp; ohp = ohp->next )
					{
						if( IS_SET( ohp->event, HP_FIGHT ) )
						{
							if ( IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
							{
								trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
						
						if( IS_SET( ohp->event, HP_WEAPON ) ) {
							if ( IS_SET( ch->act, PLR_ARTIFACT )
								&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
									+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
							{
								trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
							}
						}
					}
				}
			}
			// �Z���l��
			if ( wield->value[3] != 14 && number_bits(5) < 8) waste_dura(wield); // 33%
		}
		nHitCount++;
	} else {
		dam_message(ch, victim, -1, gsn_charge, WEAR_WIELD);
	}

	if( ( get_skill_percent(ch,gsn_charge) / 3 + nLeadSklv * nLeadSklv * nLeadSklv / 5 ) > number_percent() ){
		if( victim && victim->position != POS_DEAD ) {
			final_dam = get_critical_damage(dam, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			final_dam = number_range(final_dam * 0.8, final_dam * 1.2);
			// charge weakness_percent �]�w add at 2020/09/18
			if ( get_charge_hit_rate(nHitRate, nRideSklv, nDodge) * weakness_percent(victim , gsn_charge) / 100 > number_percent() ) {
				damage( ch, victim, final_dam, gsn_charge, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
				// artifact �� �Z���ӷl 2022/04/04
				if( wield )
				{
					// �W�[ obj_hit ���P�w 2022/10/23
					if ( !victim->deleted && wield->pIndexData->progtypes & OBJ_HIT )
						oprog_hit_trigger(wield);
					OBJ_HP * ohp = wield->hp;
					// artifact
					if ( wield->hp || wield->material->hp )
					{
						for( ; ohp; ohp = ohp->next )
						{
							if( IS_SET( ohp->event, HP_FIGHT ) )
							{
								if ( IS_SET( ch->act, PLR_ARTIFACT )
									&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
										+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								{
									trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
								}
							}
							  
							if( IS_SET( ohp->event, HP_WEAPON ) ) {
								if ( IS_SET( ch->act, PLR_ARTIFACT )
									&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
										+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								{
									trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
								}
							}
						}
						if( wield->material->hp )
						{
							for( ohp=wield->material->hp ; ohp; ohp = ohp->next )
							{
								if( IS_SET( ohp->event, HP_FIGHT ) )
								{
									if ( IS_SET( ch->act, PLR_ARTIFACT )
										&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
											+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
									{
										trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
									}
								}
								
								if( IS_SET( ohp->event, HP_WEAPON ) ) {
									if ( IS_SET( ch->act, PLR_ARTIFACT )
										&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
											+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
									{
										trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
									}
								}
							}
						}
					}
					// �Z���l��
					if ( wield->value[3] != 14 && number_bits(5) < 8) waste_dura(wield); // 33%
				}
				nHitCount++;
			} else {
				dam_message(ch, victim, -1, gsn_charge, WEAR_WIELD);
			}
			//ch->move -= 30;
		}
	}

	if( ch->class == KNIGHT &&
		( ( get_skill_percent(ch,gsn_charge) / 3 + nLeadSklv * nLeadSklv * nLeadSklv / 5 ) > number_percent() )){
		if( victim && victim->position != POS_DEAD ) {
			final_dam = get_critical_damage(dam, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			final_dam = number_range(final_dam * 0.8, final_dam * 1.2);
			// charge weakness_percent �]�w add at 2020/09/18
			if ( get_charge_hit_rate(nHitRate, nRideSklv, nDodge) * weakness_percent(victim , gsn_charge) / 100 > number_percent() ) {
				damage( ch, victim, final_dam, gsn_charge, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
				// artifact �� �Z���ӷl 2022/04/04
				if( wield ){
					// �W�[ obj_hit ���P�w 2022/10/23
					if ( !victim->deleted && wield->pIndexData->progtypes & OBJ_HIT )
						oprog_hit_trigger(wield);
					OBJ_HP * ohp = wield->hp;
					// artifact
					if ( wield->hp || wield->material->hp )
					{
						for( ; ohp; ohp = ohp->next )
						{
							if( IS_SET( ohp->event, HP_FIGHT ) )
							{
								if ( IS_SET( ch->act, PLR_ARTIFACT )
									&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
										+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								{
									trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
								}
							}
							  
							if( IS_SET( ohp->event, HP_WEAPON ) ) {
								if ( IS_SET( ch->act, PLR_ARTIFACT )
									&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
										+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
								{
									trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
								}
							}
						}

						if( wield->material->hp )
						{
							for( ohp=wield->material->hp ; ohp; ohp = ohp->next )
							{
								if( IS_SET( ohp->event, HP_FIGHT ) )
								{
									if ( IS_SET( ch->act, PLR_ARTIFACT )
										&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
											+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
									{
										trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
									}
								}
								
								if( IS_SET( ohp->event, HP_WEAPON ) ) {
									if ( IS_SET( ch->act, PLR_ARTIFACT )
										&& (artifact(ch) * ohp->percent * nCurrWis / 2500 
											+ (get_skill_level(ch,gsn_artifact) == 6 ? 40 : 30)) > number_percent() )
									{
										trigger_obj_hp(wield, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
									}
								}
							}
						}
					}
					// �Z���l��
					if ( wield->value[3] != 14 && number_bits(5) < 8) waste_dura(wield); // 33%
				}
				nHitCount++;
			} else {
				dam_message(ch, victim, -1, gsn_charge, WEAR_WIELD);
			}
			//ch->move -= 30;
		}
	}
	
	// ���z�޷|�P�w lizardman �ѥͧ� 2022/02/23
	if( !IS_NPC( ch ) && IS_SET( race_table[ ch->race ].race_abilities, RACE_LIZARDMAN ) )
		LizardmanSkill( ch , victim , 0);

	if (nHitCount == 0) nSelfDmg = 0;
	if(nSelfDmg > 0) damage( ch, ch, nSelfDmg, gsn_charge, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );

	// �԰� lead �����p�U��� stun ���ĪG , lead lv 6 charge �୰�C�@�b�� stun 2022/04/04
	// WAIT_STATE( ch, skill_table[gsn_charge].beats - nChrgSklv);
	WAIT_STATE( ch, skill_table[gsn_charge].beats - (nLeadSklv * nLeadSklv * nLeadSklv)/24 );

	if (victim == NULL || victim->position == POS_DEAD) return;

	// �T�w�������L�ؼФ~�|�] ringout �άO stun 2022/04/03
	if ( nHitCount > 0 && !IS_NPC(ch) ){
		if( IS_SET( race_table[ ch->race ].race_abilities, RACE_CENTAUR ) ||
			// �ץ� horse obj �ƭȨϥο��~ 2022/06/16
			//( horse->value[1] & 1 )
			( horse->value[1] & 64 )
			//&& ( race_table[ ch->race ].size - race_table[ victim->race ].size ) * 8 + get_skill_percent( ch, gsn_charge )/4 + nChrgSklv > number_percent() )
			&& ( get_curr_size( ch ) - get_curr_size( victim ) ) * 8 // �����髬 size ���� 2022/12/23
				+ get_skill_percent( ch, gsn_charge )/4
				+ nChrgSklv > number_percent() )
		{
			door = number_door( );/*generate a random number from 0-5*/
			if ( ( pexit = ch->in_room->exit[door] ) != 0
				&& pexit->to_room
				&& ( !IS_SET( pexit->exit_info, EX_CLOSED ) ) ){
				ringout = TRUE;
			}

			if ( ringout )
			{
				if ( !IS_NPC(victim) || !IS_SET(victim->act, ACT_SENTINEL))
				{
					act( "$n �Q $N �����F�I", victim, NULL, ch, TO_ROOM     );
					send_to_char ( "�A�Q�����F�I\n\r", victim );
					for( rch = victim->in_room->people; rch; rch = rch->next_in_room )
					if ( rch == victim || rch->fighting == victim )
					{
						rch->fighting = NULL;
						rch->position = POS_STANDING;
						if ( is_affected( rch, gsn_berserk ) )
							affect_strip( rch, gsn_berserk );
						update_pos( rch );
					}
					move_char( victim, door );
					WAIT_STATE( victim, skill_table[gsn_charge].beats - nChrgSklv/2 );
				}
				return;
			}
		}

		if ( victim->position > POS_STUNNED && get_curr_str(ch) + nChrgSklv + nRideSklv - get_curr_con(victim) > number_percent() )
		{
			victim->position = POS_STUNNED;
			WAIT_STATE( victim, PULSE_VIOLENCE * 2); // skill_table[gsn_charge].beats);
			send_to_char( "�z�I�A���[�t�Ĩ��L�����F�I\n\r", ch );
			act( "$n ���[�t�Ĩ�� $N �����F�I", ch, NULL, victim, TO_ROOM );
			act( "$n ���[�t�Ĩ뼲�o�A�ѱۦa��I", ch, NULL, victim, TO_VICT );
		}
	}

	if ( ch && ch->fighting )
	{
		stop_fighting( ch->fighting, FALSE );
		set_fighting( ch->fighting, ch );
	}
}


void do_throw( CHAR_DATA *ch, char* argument)
{
	OBJ_DATA *obj;
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int dmg = 0;
	CHAR_DATA *victim;
	int sklv = get_skill_level(ch, gsn_throwing);
	int dart_sklv = get_skill_level(ch, gsn_dart_mastery); // �t�� 2022/04/29
	int cri_dmg = 0; // �t��critical �ˮ` 2022/04/29
	int firearm_sklv = get_skill_level(ch, gsn_firearm_mastery); // alc ���Y 2022/04/29
	int stun_time = 0;
	// �P�w���Y�O�_�R�� 2022/04/29
	int HitRate = 0;
	int ChHitRoll = get_hitroll(ch, (get_eq_char(ch, WEAR_TWO_HAND) ? WEAR_TWO_HAND : WEAR_WIELD) );
	int isHit = 0;

	argument = one_argument(argument, arg);
	one_argument(argument, arg2);

	if ( IS_NPC( ch ) )
	{
		if ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan )
			return;
	}

	// safe room �L�k�ϥ� 2021/12/22
	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}

	else if( (sklv + dart_sklv + firearm_sklv) * 5 + get_skill_percent( ch, gsn_throwing ) < number_percent() )
	{
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}

	if( arg[0] == '\0' ) {
		send_to_char("�A�n�ᤰ��H\n\r", ch);
		return;
	}
	else if( !(obj = get_obj_carry(ch, arg))) {
		send_to_char("�A�S���o�˪F��C\n\r", ch);
		return;
	}
	// magic ammo �i�H�Q alc ���Y 2022/04/30
	if( !can_drop_obj(ch, obj) && obj->pIndexData->vnum != OBJ_VNUM_MAGIC_AMMO ) {
		send_to_char("�A�ᤣ�����C\n\r", ch);
		return;
	}
	if( !IS_NPC(ch) && obj->weight > str_app[get_curr_str(ch)].wield +sklv*5) {
		send_to_char("�A�ᤣ�ʡC\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}
	if ( ch->move < 150) {
		send_to_char("�A�Ӳ֤F.\n\r", ch);
		return;
	}
	if( arg2[0] == '\0') {
		/*if( !(victim = ch->fighting) ) {
			send_to_char("�A�n��֡H\n\r", ch);
			return;
		}*/
		send_to_char("�A�n��֡H\n\r", ch);
		return;
	}
	else if( !(victim = get_char_room(ch, arg2))){
		send_to_char("�o�̨S���o�ӤH�C\n\r", ch);
		return;
	}

	// �[�J²�檺 hr �� dc �P�w 2022/04/29
	if( !IS_NPC(ch) ){  //pc
		HitRate = get_skill_percent( ch, gsn_throwing ) + ChHitRoll/(7 - sklv);

		if( ch->class == NINJA )
			HitRate += 20;

		if( obj->item_type == ITEM_DART ){
			HitRate = UMAX( HitRate , get_skill_percent( ch, gsn_dart_mastery ) + ChHitRoll/(7 - dart_sklv) );
			HitRate += 10 + dart_sklv * 10;
			HitRate = HitRate * weakness_percent(victim , gsn_dart_mastery) / 100;
		}

		if( obj->item_type == ITEM_AMMO ){
			HitRate = UMAX( HitRate , get_skill_percent( ch, gsn_firearm_mastery ) + ChHitRoll/(7 - firearm_sklv) );
			HitRate += 10 + firearm_sklv * 10;
		}
		
		if( obj->item_type == ITEM_POTION
			|| obj->item_type == ITEM_PILL
			|| obj->item_type == ITEM_SCROLL
			|| obj->item_type == ITEM_WAND
			|| obj->item_type == ITEM_STAFF	)
		{
			HitRate += firearm_sklv * 10;
		}

		HitRate = HitRate * weakness_percent(victim , gsn_throwing) / 100;

		HitRate += GET_DODGE(victim)/15;

		// �p�� stun ���C�ȤΫO�٧C���v�R��
		stun_time = sklv + firearm_sklv + dart_sklv;
		
		if( stun_time > 6 ) stun_time = 6;

		if( HitRate < 1 ){
			HitRate = 1;
		}
		if( ch->class == NINJA && HitRate < 10 ){
			stun_time = stun_time + 1;
			HitRate += 10;
		}
		if( ch->class == ALCHEMIST && HitRate < 5 )
			HitRate += 5;
	}else{  //npc
		HitRate = (1000 + ( GET_DODGE(victim)/10 + ChHitRoll + ch->level*2 ))/ 10;
		HitRate = HitRate * weakness_percent(victim , gsn_throwing) / 100;
		if( HitRate < 1 )
			HitRate = 1;
	}

	if( HitRate >= number_percent() ) isHit = 1;

	//if( 1 ) // hit
	if( isHit == 1 )
	{
		int obj_avg = number_range( obj->value[1], obj->value[2] );
		switch( obj->item_type) {
			default: 
				dmg = obj->weight * (1 + sklv/3 + dart_sklv/3 );
				break;
			case ITEM_ARROW:
				dmg = obj_avg + UMIN(150 * (1 + sklv/12 + dart_sklv/12 ),obj->value[0] / 4);
				break;
			case ITEM_DART:
				if( get_skill_percent( ch, gsn_dart_mastery ) <= 0 )
					dmg = obj_avg;
				else
					dmg = obj_avg * (1 + dart_sklv/2 ) + obj->value[3];
				break;
			case ITEM_AMMO:
				dmg = obj->value[1] * (1 + firearm_sklv );
				break;
			case ITEM_WEAPON:
				dmg = obj_avg * (1 + sklv/4  + dart_sklv/4 ) + obj->weight;
				break;
			case ITEM_WAND:
			case ITEM_STAFF:
				if( obj->level > ch->level + firearm_sklv ) {
					send_to_char("�A������ۤv���Ű����]�k���~\n\r", ch);
					return;
				}
				obj_cast_spell( obj->value[3], obj->value[0] , ch, victim, NULL );
				dmg = obj->weight ;
				break;
			case ITEM_SCROLL:
			case ITEM_PILL:
			case ITEM_POTION:
				dmg = obj->weight ;
				if( obj->level > ch->level + firearm_sklv ) {
					send_to_char("�A������ۤv���Ű����]�k���~\n\r", ch);
					return;
				}
				obj_cast_spell( obj->value[1], obj->value[0], ch, victim, NULL );
				obj_cast_spell( obj->value[2], obj->value[0], ch, victim, NULL );
				obj_cast_spell( obj->value[3], obj->value[0], ch, victim, NULL );
				break;
		}

		// �j���Q���Y���ܦ����W���~���Ĥ@����
		obj_from_char( obj );
		obj_to_char( obj, ch );
		
		// ��ܸ������ޯ൥�Ũӭp��ˮ`�[�� 2022/05/05
		if( dart_sklv > sklv )
			dmg = dmg + dmg * dart_sklv/3;
		else
			dmg = dmg + dmg * sklv/3;
		
		// �t�� critical �ήz�I�]�w 2022/05/02
		if( obj->item_type == ITEM_DART && get_skill_percent( ch, gsn_dart_mastery ) > 0 ){
			cri_dmg = get_critical_damage(dmg, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			if(dmg != cri_dmg) 
				dmg = cri_dmg;
			else if(number_percent() < dart_sklv * 4 + 16)
				dmg = get_critical_damage(dmg, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			else
				dmg = dmg;
			// �t�� weakness_damage �O�|�P throwing �� weakness_damage �|�[��
			dmg = dmg * weakness_damage(victim , gsn_dart_mastery) / 100;
		}
		// weakness_damage �]�w
		dmg = dmg * weakness_damage(victim , gsn_throwing) / 100;

		// �l�[���]�ˮ` 2021/10/02
		if( IS_OBJ_STAT( obj , ITEM_SPELL) ){
			AFFECT_DATA *paf;
			int EncDam = 0;
			int EncTemp = 0;
			for ( paf = obj->affected; paf; paf = paf->next )
			{
				EncTemp = paf->modifier;

				if ( paf->location == APPLY_WEAPON_SPELL )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SPELL)) / 100);
				if ( paf->location == APPLY_WEAPON_WIND )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_WIND)) / 100);
				if ( paf->location == APPLY_WEAPON_FIRE )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_FIRE)) / 100);
				if ( paf->location == APPLY_WEAPON_COLD )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_COLD)) / 100);
				if ( paf->location == APPLY_WEAPON_EARTH )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_EARTH)) / 100);
				if ( paf->location == APPLY_WEAPON_LIGHTNING )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_LIGHTNING)) / 100);
				if ( paf->location == APPLY_WEAPON_SAINT )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100);
				if ( paf->location == APPLY_WEAPON_DARK )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100);
			}
			for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
			{
				EncTemp = paf->modifier;

				if ( paf->location == APPLY_WEAPON_SPELL )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SPELL)) / 100);
				if ( paf->location == APPLY_WEAPON_WIND )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_WIND)) / 100);
				if ( paf->location == APPLY_WEAPON_FIRE )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_FIRE)) / 100);
				if ( paf->location == APPLY_WEAPON_COLD )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_COLD)) / 100);
				if ( paf->location == APPLY_WEAPON_EARTH )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_EARTH)) / 100);
				if ( paf->location == APPLY_WEAPON_LIGHTNING )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_LIGHTNING)) / 100);
				if ( paf->location == APPLY_WEAPON_SAINT )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100);
				if ( paf->location == APPLY_WEAPON_DARK )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100);
			}
			for ( paf = obj->material->affected; paf; paf = paf->next )
			{
				EncTemp = paf->modifier;

				if ( paf->location == APPLY_WEAPON_SPELL )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SPELL)) / 100);
				if ( paf->location == APPLY_WEAPON_WIND )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_WIND)) / 100);
				if ( paf->location == APPLY_WEAPON_FIRE )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_FIRE)) / 100);
				if ( paf->location == APPLY_WEAPON_COLD )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_COLD)) / 100);
				if ( paf->location == APPLY_WEAPON_EARTH )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_EARTH)) / 100);
				if ( paf->location == APPLY_WEAPON_LIGHTNING )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_LIGHTNING)) / 100);
				if ( paf->location == APPLY_WEAPON_SAINT )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_SAINT)) / 100);
				if ( paf->location == APPLY_WEAPON_DARK )
					EncDam += UMAX( 0 , EncTemp * (100 + NEW_GET_SVS(victim, MAGIC_DARK)) / 100);
			}
			if(IS_SET( race_table[ victim->race ].race_abilities, RACE_ANTI_MAGIC )){
				if(IS_NPC(victim)){
					EncDam = EncDam * 85 / 100;
				}else{
					EncDam = EncDam * (99 - 34 * (get_skill_level(victim, gsn_inborn_ability) 
									* get_skill_level(victim, gsn_inborn_ability) ) / 36) / 100;
				}
			}
			if(IS_NPC(ch) && EncDam >= 0){
				EncDam = EncDam/2;
			}
			EncDam = UMAX(0 , EncDam - victim->mdr);
			if( IS_NPC(victim) && EncDam < victim->mad ){ 
				EncDam = 0;
			}
			dmg += EncDam;
		}

		// �w�藍�P item �������P�� dt ��
		//damage( ch, victim, dmg, 1015, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
		if( obj->item_type == ITEM_DART )
			damage( ch, victim, dmg, 1015, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
		else if( obj->item_type == ITEM_AMMO )
			// �ץ� throw ammo �� dt �� 2022/08/27
			//damage( ch, victim, dmg, gsn_firearm_mastery, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
			damage( ch, victim, dmg, gsn_artifacts_mastery, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
		else
			damage( ch, victim, dmg, gsn_throwing, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
	}
	else  // �S���R��
	{
		//damage( ch, victim, -1, 1015, TYPE_UNDEFINED, MSG_NORMAL, DMG_NORMAL );
		if( obj->item_type == ITEM_DART )
			c_dam_message( ch, victim, -1, 1015, WEAR_NONE );
		else if( obj->item_type == ITEM_AMMO )
			// �ץ� throw ammo �� dt �� 2022/08/27
			//c_dam_message( ch, victim, -1, gsn_firearm_mastery, WEAR_NONE );
			c_dam_message( ch, victim, -1, gsn_artifacts_mastery, WEAR_NONE );
		else
			c_dam_message( ch, victim, -1, gsn_throwing, WEAR_NONE );
	}

	// �T�w�R���~�|�P�w hp 2022/04/29
	if( isHit == 1 ){
		// �W�[ obj_hit ���P�w 2022/10/23
		if ( !victim->deleted && obj->pIndexData->progtypes & OBJ_HIT )
			oprog_hit_trigger(obj);
		// obj artifact trigger
		OBJ_HP * ohp = obj->hp;
		if( obj && (obj->hp || obj->material->hp) ){
			for( ; ohp; ohp = ohp->next ){
				if( IS_SET(ohp->event, HP_WEAPON) || IS_SET(ohp->event, HP_ARROW) || IS_SET(ohp->event, HP_DART))
				if( IS_SET(ch->act, PLR_ARTIFACT) 
					&& ohp->percent * (get_curr_int(ch)+sklv) / 25 > number_percent() )
				trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
			}
			if( obj->material->hp ) {
				for( ohp=obj->material->hp ; ohp; ohp = ohp->next ){
					if( IS_SET(ohp->event, HP_WEAPON) || IS_SET(ohp->event, HP_ARROW) || IS_SET(ohp->event, HP_DART))
					if( IS_SET(ch->act, PLR_ARTIFACT) 
						&& ohp->percent * (get_curr_int(ch)+sklv) / 25 > number_percent() )
					trigger_obj_hp(obj, skill_lookup(ohp->hp_name), ohp->level, ch, victim);
				}
			}
		}
	}
	
	ch->move -= 150;
    WAIT_STATE(ch, skill_table[gsn_throwing].beats - stun_time);

    /*check obj_act when throw a item 19/12/16 */
	if( IS_SET(obj->pIndexData->progtypes , OBJ_ACT)){
		oprog_act_trigger("throw", ch, obj);
		//return;
	}

	// �D�����
	if( obj->item_type != ITEM_DART && obj->item_type != ITEM_AMMO ){
		obj_from_char(obj);
		extract_obj(obj);
	}

	// �t���o�ƴ��
	if( obj->item_type == ITEM_DART ){
		obj->value[0] -= 1;
		if (obj->value[0] <= 0){
			obj_from_char(obj);
			extract_obj(obj);
		}
	}

	// �]�u�P�w  2022/04/29
	if( obj->item_type == ITEM_AMMO ){
		if( isHit == 1 ){ // ��u�R���P�w casting
			do_ammo_casting( ch , victim , obj );
		}else{            // ��u miss
			obj_from_char(obj);
			extract_obj(obj);
		}
	}

    if( !victim->deleted && victim->fighting != ch && victim->in_room == ch->in_room )//victim may flee!
        stop_fighting(victim, FALSE);

	// victim �}�ԲĤ@�^�X�N���`�h���򤣧P�w, �H�K ch ��Ů𥴬[ 2022/07/14
	if( !victim || victim->deleted ) return;

	// �ؼаk�]�� ch �|��Ů𥴬[ �G���� 2022/07/15
	//set_fighting(victim, ch);

	return;
}


void do_combat_howl( CHAR_DATA *ch, char *argument )/*{{{*/
{
	DESCRIPTOR_DATA *d;
	CHAR_DATA *victim, *victim_next;
	int dam;
	int sklv = get_skill_level(ch, gsn_combat_howling);

	if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_combat_howling] < number_percent() )
	{
		send_to_char( "�A�յۤj�n�q�s�A�o�X�n�x���n���C\n\r", ch );
		WAIT_STATE( ch, skill_table[gsn_combat_howling].beats );
		return;
	}
	if ( ch->hit * 6 < ch->max_hit )
	{
		send_to_char( "�A�{�b�ӵ�z�A�ӵL�k����ĤH�C\n\r", ch );
		WAIT_STATE( ch, 4 );
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
		return;
	}

	ch->hit -= 10 * ch->level;

	for ( d = descriptor_list; d; d = d->next )
	{
		CHAR_DATA *och;
		CHAR_DATA *vch;

		och = d->original ? d->original : d->character;
		vch = d->character;
		if( !och ) continue;
		if( !vch) continue;

		if ( d->connected == CON_PLAYING
			&& !IS_SET( och->in_room->room_flags, ROOM_CONE_OF_SILENCE ) )
		{
			if ( vch->in_room->area != ch->in_room->area  || vch->in_room == ch->in_room )
				continue;

			send_to_char( HIG"�Ať��@�}�}�Y�F���z�s�n�A���������֪��D���ݰ_�ӡI\n\r"NOR, vch );
		}
	}

	for( victim = ch->in_room->people; victim; victim = victim_next )
	{
		victim_next = victim->next_in_room;
		if(sklv > 4 && is_same_group( ch, victim )) continue;
		if ( !IS_NPC( ch ) && !IS_NPC( victim )
			&& ( !registered( ch, victim ) || ( !licensed( ch ) && victim->race != race_lookup( "Vampire" ) ) ) )
		{
			continue;
		}

		if( IS_NPC(ch) && is_same_group(ch, victim) ) continue;

		if( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_WIZINVIS)) continue;

		if ( victim != ch )
		{
			dam = ( ch->level + get_curr_con( ch ) ) * 4 + sklv * 5;
			////damage( ch, victim, dam, gsn_combat_howling, WEAR_NONE );
			damage( ch, victim, dam, gsn_combat_howling, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );

			if ( is_affected(victim, gsn_will_fortress)
				&& number_percent() < -(affected_value(victim, gsn_will_fortress, APPLY_SAVING_ILLUSION)))
				continue;
			
			if ( victim->hit > 0 && victim->wait == 0 
				&& ( ch->level + 3 + sklv - victim->level ) > number_bits(3)
				&& check_exit ( victim, 6 ) > 0 )					//Added by Razgriz 20050908
				do_flee( victim, "" );
		}
	}
	WAIT_STATE( ch, skill_table[gsn_combat_howling].beats );
}/*}}}*/

void do_track( CHAR_DATA *ch, char *argument )/*{{{*/
{
    CHAR_DATA *victim, *vch;
    char       arg [ MAX_INPUT_LENGTH ];

    one_argument( argument, arg );

    if ( !ch->fighting )
    {
		if ( arg[0] == '\0' )
		{
			if ( ( victim = ch->tracking ) )
			{
				act( "�A����� $N ���l�ܡC\n\r", ch, NULL, victim, TO_CHAR);
				if ( victim->tracked == ch )
				{
					victim->tracked = ch->next_in_track;
					ch->tracking = NULL;
					ch->next_in_track = NULL;
					return;
				}
				else
				{
					for( vch = victim->tracked; vch; vch = vch->next_in_track )
						if ( vch->next_in_track == ch )
						{
							vch->next_in_track = ch->next_in_track;
							ch->tracking = NULL;
							ch->next_in_track = NULL;
							return;
						}
				}
			}
			send_to_char( "�A�Q�n�l�ܽ֡H\n\r", ch );
			return;
		}
		if ( !( victim = get_char_room( ch, arg ) ) )
		{
			send_to_char( "�A�䤣��L�C\n\r", ch );
			return;
		}

		if( victim == ch ) {
			send_to_char("�A���ݭn���ܦۤv�C\n\r", ch);
			return;
		}
		if( is_affected(ch, gsn_stone_form) ){
			send_to_char("�A�L�k�b�ۤƪ��A�U�ϥΧޥ�.\n\r", ch);
			return;
		}

		if ( IS_NPC( ch ) || ( !IS_NPC( ch ) && number_percent() > ch->pcdata->learned[gsn_track] ) )
		{
			send_to_char("�A���g�礣���A�ݤ��X��誺�N�V�C\n\r", ch );
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_track].beats );
		ch->tracking = victim;
		ch->next_in_track = victim->tracked;
		victim->tracked = ch;
		act( "�A�}�l�l�� $N!\n\r", ch, NULL, victim, TO_CHAR);
		return;
    }

	victim = ch->fighting;

	if ( arg[0] != '\0' )
	{
		if ( ( vch = get_char_room( ch, arg ) ) )
		{
			if ( vch == ch )
			{
				act( "�A����� $N ���l�ܡC\n\r", ch, NULL, victim, TO_CHAR);
				if ( victim->tracked == ch )
				{
					victim->tracked = ch->next_in_track;
					ch->tracking = NULL;
					ch->next_in_track = NULL;
					return;
				}
				else
				{
					for( vch = victim->tracked; vch; vch = vch->next_in_track )
						if ( vch->next_in_track == ch )
						{
							vch->next_in_track = ch->next_in_track;
							ch->tracking = NULL;
							ch->next_in_track = NULL;
							return;
						}
				}
			}
			else if ( vch != victim )
			{
				send_to_char( "�A���M�O�H���o�����O�I\n\r", ch );
				return;
			}
		}
		else
		{
			send_to_char( "�A�䤣��L�C\n\r", ch );
			return;
		}
	}
	if ( IS_NPC( ch ) || ( !IS_NPC( ch ) && number_percent() > ch->pcdata->learned[gsn_track] ) )
	{
		send_to_char( "�A���g�礣���A�ݤ��X��誺�N�V�C\n\r", ch );
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_track].beats );
	ch->tracking = victim;
	ch->next_in_track = victim->tracked;
	victim->tracked = ch;
	act( "�A�}�l�l�� $N!\n\r", ch, NULL, victim, TO_CHAR);
	return;
}/*}}}*/

void do_stone_form(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int nRaceSklv = get_skill_level(ch, gsn_inborn_ability);

	if ( IS_AFFECTED( ch, AFF_POLYMORPH) ){
		send_to_char("�A�L�k�b�ܱڪ��A�U�ϥΥۤ�.\n\r", ch);
		return;
	}

	if( is_affected(ch, gsn_meditate) ){
		send_to_char("�A�L�k�b�߷Q���A�U�ϥΥۤ�.\n\r", ch);
		return;
	}

	if( is_affected(ch, gsn_stone_form) )
	{
		// �[�J stone form �@�� apply_race �ɧP�w�O�_��ۤv���� stone form 2022/12/05
		if( affected_value( ch, gsn_stone_form, APPLY_RACE ) == -16 )
		{
			// �p�G��race statue �h�ԭz���P 2022/12/05
			if( ch->race == race_lookup( "Statue" ) ){
				send_to_char("�A�Ѱ��F���W���ۤƪ��A�A�q�a�����ͭ�����Ť��C\n\r"NOR, ch);
				act( "$n"CYN"���U�F�J���몺�~�ߡA�q�a�����ͭ�����Ť��C"NOR, ch, NULL, NULL, TO_NOTVICT );
			}else{
				send_to_char("�A�Ѱ��F���W���ۤƪ��A�C\n\r"NOR, ch);
			}
			affect_strip(ch, gsn_stone_form);
			return;
		}
		return;
	}

	if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_GARGOYLE ) ){
		send_to_char("�A���رڤ��㦳�ۤƪ���O.\n\r", ch);
		return;
	}

	if(ch->move < 80 + nRaceSklv * 20){
		send_to_char("�A����O�����H���A�ۤơC\n\r", ch);
		return;
	}

	if( is_affected(ch, gsn_stone_form)) return;

	ch->move -= 80 + nRaceSklv * 20;

	af.type      = gsn_stone_form;
	af.duration  = -1;
	af.location  = APPLY_CON;
	af.modifier  = 1 + nRaceSklv;
	af.bitvector = AFF_HOLD;
	affect_to_char( ch, &af );

	af.location  = APPLY_DODGE;
	af.modifier  = +1000;
	affect_to_char( ch, &af );

	af.location  = APPLY_AC;
	af.modifier  = -100 - nRaceSklv * (nRaceSklv + 4) * 12;
	affect_to_char( ch, &af );

	af.location  = APPLY_RACE;
	af.modifier  = -16;
	affect_to_char( ch, &af );

	if(nRaceSklv >= 4){    
		af.location  = APPLY_RDAMAGE;
		af.modifier  = nRaceSklv * (nRaceSklv + 4) + 20;
		affect_to_char( ch, &af );
	}

	if(nRaceSklv >= 5){    
		af.location  = APPLY_RMAGIC;
		af.modifier  = nRaceSklv * (nRaceSklv + 4) - 10;
		affect_to_char( ch, &af );
	}

	send_to_char(HIB"�A�q�Ť�������a���A�N������Ʀ��۹�.\n\r"NOR, ch);
	act( "$n"HIB"�q�Ť�������a���A�಴�����M�ܦ��@���J���I"NOR, ch, NULL, NULL, TO_NOTVICT );
}