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
//*  PROGRAM ID  : skill_lead.c                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 Razgriz                                *
//*  PURPOSE     :         �ץ� do_lead_assistance                *
//*              :         �b�ˬd ch �O�_�� leader �e             *
//*              :         �[�J ch �P protector �O�_���P�����P�_  *
//*              :         �H�קK���᪺ leader �ˬd�y�� crash     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940709 Jye                                    *
//*  PURPOSE     :         �ץ� do_lead_ucharge �y��crash��bug    *
//*****************************************************************
/* $Id: skill_lead.c 3439 2005-10-14 15:55:48Z akira $ */
#include "skill.h"

void check_alignment_eq( CHAR_DATA *ch );
bool psionic_check(CHAR_DATA *ch, int sn );

//Add by Neversay at 2004/July/11
void do_lead_ucharge(CHAR_DATA *ch, char* argument)
{
	CHAR_DATA *rider;
	char arg[MAX_INPUT_LENGTH];
	int sklv = get_skill_level(ch, skill_lookup("lead"));

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}
    
	if(!ch->fighting){
		send_to_char( "�A���b�԰���. \n\r",ch);
		return;
	}
    
	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}
    
	if(ch->move >= 200-(sklv<<3))
	{
		ch->move -= 200 - (sklv<<3);
		send_to_char(""HIW"�A�v���M�L�V�e�ľW�I"NOR"\n\r", ch);
		act("$n�v���M�L�ľW�I\n\r", ch, NULL, NULL, TO_ROOM);
		sprintf(arg, "%d", sklv);
		do_charge(ch, arg);
	}
	else
	{
		send_to_char("�A����O�����F�I\n\r", ch);
		return;
	}

	/*
			2022/04/05
			�����ݬ� kni �~�|��۽ľW , �N���� lord �]���۽Ĩ�
			�����U���O�̥��ݬ� leader ���]�w
			�קאּ�H rider �O�_�� cooperate ch �ӧP�w�O�_��۽Ĩ�
	for( rider = ch->in_room->people; rider; rider = rider->next_in_room)
	{
		if( !is_same_group(ch,rider) )
			continue;
		
		if(    !rider->group
			|| !rider->group->leader
			|| rider->deleted
			|| rider->class != KNIGHT )
				continue;
				
		if( rider->group->leader == ch && rider->wait <= skill_table[gsn_charge].beats )
		{
			sprintf(arg, "%d", sklv);
			do_charge(rider, arg);
		}
	}
	*/
	for( rider = ch->cooperator; rider; rider = rider->next_in_coop )
	{
		if( rider->deleted )
			continue;

		if( !is_same_group(ch,rider) )
			continue;
		
		if( rider->cooperating != ch )
			continue;

		if( rider->in_room != ch->in_room )
			continue;

		if( IS_NPC(rider) )
			continue;
		
		if( get_skill_percent( rider ,gsn_charge) <= 0 )
			continue;

		if( rider->wait <= skill_table[gsn_charge].beats )
		{
			sprintf(arg, "%d", sklv);
			do_charge(rider, arg);
		}
	}

	WAIT_STATE(ch, 6);
}

//Add by Neversay at 2004/July/11
void do_lead_ufire(CHAR_DATA *ch, char* argument)
{
	CHAR_DATA *archer = NULL, *rch;
	OBJ_DATA *obj, *arrow;
	int sn = skill_lookup("lead"), sklv, sklv_archery, sklv_mete, number_enemy = 0, wait, archer_hr, fire_number, temp;
	bool can_fire = FALSE;
	AFFECT_DATA aff;
	int dt_undefined = TYPE_UNDEFINED; //Added by Razgriz 20051014

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
        send_to_char( "�o�̸T��԰�. \n\r",ch);
        return;
    }
    if(!ch->fighting){
        send_to_char( "�A���b�԰���. \n\r",ch);
        return;
    }
    if( get_skill_percent(ch, sn) <= number_percent()){
        send_to_char("�A���ѤF�C\n\r", ch);
        return;
    }
    sklv = get_skill_level(ch, sn);
    if(sklv < 1){
        send_to_char("�A����ɧޥ������¼��C\n\r", ch);
        return;
    }

    if(ch->move >= 100-(sklv<<3))
    {
        ch->move -= 100 - (sklv<<3);
        send_to_char(""HIW"�A�����}�b��b�ڻ���I"NOR"\n\r", ch);
        act("$n�����}�b��b�ڻ���I", ch, NULL, NULL, TO_ROOM);
    }
    else
    {
        send_to_char("�A����O�����F�I\n\r", ch);
        return;
    }

    //Calculate the number of enemy and archer in group.
    for(rch = ch->in_room->people;rch;rch = rch->next_in_room)
    {
        if(rch->deleted)continue;
        if(rch->fighting && is_same_group(rch->fighting, ch))
            number_enemy++;
    }

    if(!number_enemy)
    {
        send_to_char("�S���ĭx�i�H���A�g���C\n\r",ch);
        return;
    }

    //The penalty of hitroll when use this skill.
    aff.type	 = gsn_meteoric_arrow;
    aff.duration = 1;
    aff.location = APPLY_HITROLL;

    //Fire by archers who leading by lord.
	// �קאּ �H cooperate ���P�w 2022/04/05
    //for(archer = ch->in_room->people; archer; archer=archer->next_in_room)
	for(archer = ch->cooperator; archer; archer = archer->next_in_coop)
    {
		
		/*
			2022/04/05

			�����U���O�̥��ݬ� leader ���]�w
			�קאּ�H archer �O�_�� cooperate ch �ӧP�w�O�_��۽Ĩ�
			�ñN�C�ӱ�����}�P�w, �H�[�J��L����

        if( archer->deleted
                || get_skill_percent(archer, gsn_meteoric_arrow )<= 0 
                || archer->wait > skill_table[gsn_meteoric_arrow].beats 
                || !archer->group 
                || archer->group->leader != ch 
                || !archer->fighting 
                || (IS_NPC( archer ) 
                    && ( IS_AFFECTED( archer, AFF_CHARM )
                        || IS_AFFECTED( archer, AFF_SUMMONED ) 
                        || archer->clan )))
        {
            continue;
        }
		*/

		if( archer->deleted )
			continue;
		
		if( !is_same_group(ch,archer) )
			continue;

		if( archer->cooperating != ch )
			continue;

		if( archer->in_room != ch->in_room )
			continue;

		if( IS_NPC( archer ) ){
			if( IS_AFFECTED( archer, AFF_CHARM ) )
				continue;
			// mob class 16 is archer
			if( !IS_SET( archer->class, 16 ) )
				continue;
		}

		if( !IS_NPC( archer ) && get_skill_percent(archer, gsn_meteoric_arrow ) <= 0 )
			continue;

		if( archer->wait > skill_table[gsn_meteoric_arrow].beats )
			continue;

        if ( !( obj = get_eq_char( archer, WEAR_TWO_HAND ) ) || obj->value[3] != 14 )
        {
            //send_to_char( "�n�﮳��}�a�H\n\r", archer );
            continue;
        }

        if ( !( arrow = get_eq_char( archer, WEAR_ARROW ) ) || arrow->value[0] == 0 )
        {
            //send_to_char( "���A�S�b�F!\n\r", archer );
            continue;
        }

        if(!IS_NPC(archer)
			&&( (get_skill_percent(archer, gsn_meteoric_arrow)>>1)
				+ (sklv<<3) < number_percent() ) )
        {
            send_to_char("�A�չ�ť�q��D�������A�i�����ѤF�I",archer);
            continue;
        }

        for( can_fire = TRUE, rch = ch->in_room->people; rch; rch = rch->next_in_room )
        {
            if ( rch->fighting == archer )
            {
				int ride_sklv   = get_skill_level(archer, gsn_ride );
				int tricky_sklv = get_skill_level(archer, gsn_tricky_pacing );

				// �i�H�ۥѮg�b������ 2022/04/05
				if( get_eq_char( archer, WEAR_RIDE ) && ride_sklv > 2 && ride_sklv >= tricky_sklv )
				{
					if( number_percent( ) < 10 + ride_sklv * 15 )
						break;
				}
				else if( tricky_sklv > 2 && number_percent( ) < 10 + tricky_sklv * 15 )
				{
					break;
				}
				send_to_char( "�A�Q���o�⦣�}�áA�S�ɶ��Ԥ}�C\n\r", archer );
				can_fire = FALSE;
				break;
            }
        }
        
        if(!can_fire)
            continue;

        if ( archer->move < 150 )
        {
            send_to_char( "�Aı�o�n�֡I\n\r", archer );
            continue;
        }

        sklv_mete = get_skill_level(archer, gsn_meteoric_arrow);
        sklv_archery = get_skill_level(archer, gsn_shot_arrow);
        fire_number = UMAX((3+sklv_mete*3)/number_enemy, 1);
        archer_hr = (((sklv<<1)
					+sklv_archery
					-(sklv>number_bits(2)?20:50))<<2)/number_enemy;
        archer->move -= 150;
        send_to_char( "�A���}���F�A�\\�h�b�ڦp�y�P�멹�İ}���h�I\n\r", archer );

        //Join the affect to archer and remove it after firing.
        if((archer->hitroll>>2) + archer_hr < 0)
            send_to_char("�A�����b�g�n�F�I\n\r", archer);
        aff.modifier = archer_hr;
        aff.bitvector = 0;
        affect_to_char( archer, &aff );
        wait = archer->wait;archer->wait = 0;

        //archer fire to enemies.
        for(rch = ch->in_room->people; rch; rch = rch->next_in_room){
            if(rch->deleted || !rch->fighting) continue;
            if(is_same_group(rch->fighting, ch))
                for(temp = 0;temp < fire_number; temp++) {
                    ////mone_hit( archer, rch, TYPE_UNDEFINED, WEAR_WIELD );
                    mone_hit( archer, rch, &dt_undefined, WEAR_WIELD, MSG_NORMAL, DT_NOCHANGE );
                    wait += archer->wait;archer->wait = 0;
                }
        }

        affect_strip( archer, gsn_meteoric_arrow);

        if ( sklv_mete < 5 || archer->level < 42 )
            WAIT_STATE( archer, skill_table[gsn_meteoric_arrow].beats );
        else
        {
            WAIT_STATE( archer, skill_table[gsn_meteoric_arrow ].beats );
            WAIT_STATE( archer, skill_table[gsn_meteoric_arrow ].beats / 3);
        }

        WAIT_STATE(archer, wait);
    }

    WAIT_STATE(ch, (skill_table[gsn_meteoric_arrow].beats<<1));
}

//Add by Neversay at 2004/July/12
void do_lead_assault(CHAR_DATA *ch, char* argument)
{
    CHAR_DATA *rch;
    int sklv;

    if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
    {
        send_to_char( "�o�̸T��԰�. \n\r",ch);
        return;
    }
    if(!ch->fighting || ch->fighting->deleted )
    {
        send_to_char("�A�|�����B�԰����I\n\r",ch);
        return;
    }
    
    if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent())
    {
        send_to_char("�A���ѤF�C\n\r", ch);
        return;
    }
    
    sklv = get_skill_level(ch, skill_lookup("lead"));
    
    if(sklv < 3)
    {
        send_to_char("�A����ɧޥ������¼��C\n\r", ch);
        return;
    }
    
    if(ch->move >= 150-(sklv<<3))
    {
        ch->move -= 150 - (sklv<<3);
        act("�A�a�Y�ĦV�ĤH$N�I", ch, NULL, ch->fighting, TO_CHAR);
        act("$n�a��j�a�Ķi�İ}�A��ŧ$N�I", ch, NULL, ch->fighting, TO_ROOM);
    }
    else
    {
        send_to_char("�A����O�����F�I\n\r", ch);
        return;
    }

    for(rch = ch->in_room->people;rch;rch = rch->next_in_room){
        if(!is_same_group(rch, ch)
                || !check_blind(rch)
                || !can_see(rch, ch->fighting)
                || rch->fighting == ch->fighting 
                || (IS_NPC( rch ) 
                    && ( IS_AFFECTED( rch, AFF_CHARM )
                        || IS_AFFECTED( rch, AFF_SUMMONED ) 
                        || rch->clan )) 
                || (!IS_NPC(rch) 
                    && (ch->pcdata->autocombat 
                        & (AUTOC_CONCENTRATION 
                            | AUTOC_DODGE 
                            | AUTOC_PARRY 
                            | AUTOC_STOP))) )
            continue;
        //Changing percentage from 20%~100%
        if((UMAX(get_skill_percent( ch, gsn_change_fighting),get_skill_percent( rch, gsn_change_fighting))>>2)
                + get_curr_int(rch) + get_curr_dex(rch) + sklv*5 < number_percent()){
            if( !is_chinese(rch, ch->fighting))
                send_to_char("You cannot break through the defend of enemies�I\n\r", rch);
            else
                send_to_char("�A�Ĥ��L�Ĥ誺���u�I\n\r", rch);
        }
        else{
            if( !is_chinese(rch, ch->fighting))
                act( "You change your fighting target to $N!",  rch, NULL, ch->fighting, TO_CHAR);
            else
                act( "�A�}�l���� $N�I",  rch, NULL, ch->fighting, TO_CHAR);
            if( !is_chinese(ch->fighting, rch))
                act( "$n turn $s target to you!", rch, NULL, ch->fighting, TO_VICT);
            else
                act( "$n �ਭ�V�A�o�ʧ����I", rch, NULL, ch->fighting, TO_VICT  );
            //Do changing fighting.
            rch->fighting        = NULL;
            rch->position        = POS_STANDING;
            WAIT_STATE( rch, skill_table[gsn_change_fighting].beats );
            update_pos( rch );
            stop_fighting(rch, FALSE);
            set_fighting( rch, ch->fighting );
        }
    }
    WAIT_STATE( ch, (skill_table[gsn_change_fighting].beats<<2) - sklv*3 );
    return;
}

//Add by Neversay at 2004/July/12
void do_lead_assistance(CHAR_DATA *ch, CHAR_DATA *protector)
{
	CHAR_DATA *rch;
	// ���� number_rescue �Ѽ�, �� wait �b�C�� rescue �ɧP�w 2023/01/12
	//int sklv, wait, number_rescue = 0;
	int sklv;
	int wait;
	int p_move_cost; // 2023/01/12

	if( !ch->group ) 
	{
		send_to_char("�A�èS���b����C\n\r", ch);
		return;
	}

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char( "�o�̸T��԰�. \n\r",ch);
		return;
	}
	if(!ch->fighting || ch->fighting->deleted){
		send_to_char("�A�|�����B�԰����I\n\r",ch);
		return;
	}
	if(!protector) {
		send_to_char("�S���H�i�H�O�@����I\n\r",ch);
		return;
	}
	
	if( !is_same_group( ch, protector ) )			//Added by Razgriz  20050618
	{
		send_to_char("��褣�b�A������I\n\r",ch);
		return;
	}
	
	if( protector->group->leader && protector->group->leader != ch)
	{
		send_to_char("��褣���A�����I\n\r",ch);
		return;
	}

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}
	sklv = get_skill_level(ch, skill_lookup("lead"));

	if(sklv < 2){
		send_to_char("�A����ɧޥ������¼��C\n\r", ch);
		return;
	}

	// stone form �����A�U�L�k���� lead 2022/12/05
	if( is_affected(protector, gsn_stone_form) ){
		send_to_char("�L���B�b�ۤƪ��A�U�A�L�k����A�������C\n\r", ch);
		return;
	}

	// �P�w protector �� mv �O�_�����ϥ� rescue 2023/01/12
	p_move_cost = 200 - get_skill_level(protector, gsn_rescue)*20;

	if( protector->move < p_move_cost ){
		send_to_char("�L����O�����H����o�ӫ��ܡC\n\r", ch);
		return;
	}

	// �P�w protector wait �O�_�Ӱ� 2023/01/12
	if( protector->wait > 11 + sklv * 2 )
	{
		send_to_char( "[33m�L�w�g�⦣�}�äF�I[m\n\r", ch );
		return;
	}

	if( ch->move >= 100 - (sklv<<3) ){
		ch->move -= 100 - (sklv<<3);
		act("�A����$N�O�@��Ӷ���I", ch, NULL, protector, TO_CHAR);
		act("$n�����A�O�@��Ӷ���I", ch, NULL, protector, TO_VICT);
		act("$n����$N�O�@��Ӷ���I", ch, NULL, protector, TO_NOTVICT);
	}
	else{
		send_to_char("�A����O�����F�I\n\r", ch);
		return;
	}

	wait = protector->wait;

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room)
	{
		// protector �� mv �p���ϥ� rescue ���Ȯ�, �������X�{�� 2023/01/12
		if( protector->move < p_move_cost ){
			send_to_char( "�A����O�����H�~�������ܡC\n\r", protector );
			break;
		}

		if( rch->deleted || !is_same_group(rch, ch) || protector == rch )
			continue;

		// �ؼФ��b�԰��� (config -assist �����p) 2023/01/12
		if ( !rch->fighting )
			continue;

		/* 2023/01/12
			�ק� assistance , �����H do_rescue �P�w�O�_���\
			�ê����H wait �@���C�� rescue �᪺�`��

		if(sklv + 1 >= number_bits(3)){
			do_rescue(protector, rch->name);
			number_rescue++;
		}else if(rch->fighting)
			act("�A�չϫO�@$N�A�i�O���ѤF�I", protector, NULL, rch, TO_CHAR); */

		do_rescue( protector, rch->name );
		// �C�� rescue �̤� wait + 1 2023/01/12
		wait += UMAX( 1 , protector->wait - wait - sklv/2 );
	}

	WAIT_STATE( ch, 12 - sklv );
	// �����N protector �� wait �ܧ� wait( ��l + �h��rescue ���`��) 2023/01/12
	//WAIT_STATE( protector, number_rescue*(2-sklv/6) );
	protector->wait = wait;
}

//Add by Neversay at 2004/July/16
void do_lead_hidemarch(CHAR_DATA * ch, char* argument)
{
	CHAR_DATA * rch;
	AFFECT_DATA af;
	int number_of_scout, sklv;

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
	sklv = get_skill_level(ch, skill_lookup("lead"));
        if(sklv < 4){
            send_to_char("�A����ɧޥ������¼��C\n\r", ch);
            return;
        }
        if(ch->move >= 200-(sklv<<4)){
	    ch->move -= 200 - (sklv<<4);
	    act("�A���������������ΡA�ðΦ�x�I", ch, NULL, NULL, TO_CHAR);
	    act("$n���������������ΡA�ðΦ�x", ch, NULL, NULL, TO_ROOM);
	}
	else{
	    send_to_char("�A����O�����F�I\n\r", ch);
	    return;
	}

	for(number_of_scout = 0,rch = ch->in_room->people;rch && number_of_scout <2;rch=rch->next_in_room)
	    if(!IS_NPC(rch) && (rch->class == NINJA || rch->class == ASSASSIN) &&
		is_same_group(rch,ch))
		number_of_scout++;

	if(number_of_scout < 2){
	    send_to_char("�A��������J�����I\n\r", ch);
	    return;
	}

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(is_same_group(rch,ch) ){
		if((time_info.hour < 4+(sklv>>1) || 
		    time_info.hour > 20-(sklv>>1)) &&
		    !IS_AFFECTED( rch, AFF_HIDE)){
		    SET_BIT( rch->affected_by, AFF_HIDE);
		    send_to_char("�A�ӵۻ�D�������ðΤF�_�ӡC\n\r", rch);
		}
		if(!is_affected(rch, gsn_sneak)){
		    af.type      = gsn_sneak;
		    af.duration  = ch->level;
		    af.location  = APPLY_NONE;
		    af.modifier  = 0;
		    af.bitvector = AFF_SNEAK;
		    affect_to_char( rch, &af );
		    send_to_char("�A�ӵۻ�D�����ܩ񻴸}�B�C\n\r", rch);
		}
	    }
	}
}

//Snare all enemy.
void do_lead_envelopment(CHAR_DATA * ch, char * argument){
	CHAR_DATA * rch;
	AFFECT_DATA af;
	int number_front = 0,number_back = 0,number_member = 1,
	    number_enemy = 0,sklv,chance = 0,sector_type, class_array[20];

	if( !ch->in_room ) return;
	sector_type = ch->in_room->sector_type;

	if( get_skill_percent(ch, skill_lookup("lead")) <= number_percent()){
            send_to_char("�A���ѤF�C\n\r", ch);
            return;
        }
	if(!ch->fighting || ch->fighting->deleted){
	    send_to_char("�A�|�����B�԰����I\n\r",ch);return;
	}
	if(ch->wait > skill_table[gsn_snare].beats){
	    send_to_char("�A�⦣�}�áA�L�k��������I\n\r",ch);return;
	}
	sklv = get_skill_level(ch, skill_lookup("lead"));
        if(sklv < 5){
            send_to_char("�A����ɧޥ������¼��C\n\r", ch);
            return;
        }
        if(ch->move >= 300-(sklv<<5)){
	    ch->move -= 300 - (sklv<<5);
	    act("�A��������A�]�ۦX��ĭx�I", ch, NULL, NULL, TO_CHAR);
	    act("$n��������A�]�ۦX��ĭx�I", ch, NULL, NULL, TO_ROOM);
	}
	else{
	    send_to_char("�A����O�����F�I\n\r", ch);
	    return;
	}

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(rch->deleted)continue;
	    else if(!IS_NPC(rch) && rch->group && rch->group->leader == ch){
		number_member++;
		class_array[rch->class]++;
	    }
	    else if(rch->fighting && is_same_group(rch->fighting, ch))
		number_enemy++;
	}

	if(number_enemy >= number_member){
	    send_to_char("�A���έ��ƶq�Ӥ֡A�����H�]��ĭx�I\n\r", ch);
	    return;
	}
	if(!(number_front = class_array[SHAMAN]+class_array[KNIGHT]+
	    class_array[WARLORD]+class_array[MARTIALMAN])){
	    send_to_char("�A���e�u�έ��ƶq�Ӥ֡A�����H�]��ĭx�I\n\r", ch);
	    return;
	}
	if(!(number_back = class_array[NINJA]+class_array[RANGER])){
	    send_to_char("�A�ү�䬣���]�ۤH���Ӥ֡A�����H�]��ĭx�I\n\r", ch);
	    return;
	}

	//calculate chance.
	//basic chance 0~85%
	chance = UMIN((((sklv*sklv)<<1)*(number_member+number_back+number_front))/(number_enemy<<2), 85);
	//Adject 1~12%
	chance+= (get_curr_int(ch)+get_curr_wis(ch))>>2;
	//for per mage or cleric, get -4%
	chance-= (class_array[MAGE]+class_array[WIZARD]+class_array[ALCHEMIST]+ class_array[SAVANT]+class_array[BISHOP])<<2;
	//Adject by sector of room and class.
	switch(sector_type){
	    case SECT_INSIDE:
		chance+=(class_array[NINJA]*10+class_array[MONK]*5+class_array[ASSASSIN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[RANGER]);
		break;
	    case SECT_CITY:
		chance+=(class_array[SWORDSMAN]*10+class_array[RANGER]*5+class_array[WARLORD]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[ASSASSIN]);
		break;
	    case SECT_FIELD:
		chance+=(class_array[KNIGHT]*10+class_array[WARLORD]*5+class_array[MARTIALMAN]*5);
		chance-=((class_array[ASSASSIN]<<1)+class_array[NINJA]);
		break;
	    case SECT_FOREST:
		chance+=(class_array[RANGER]*10+class_array[WARLORD]*5+class_array[SHAMAN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[MARTIALMAN]);
		break;
	    case SECT_HILLS:
		chance+=(class_array[WARLORD]*10+class_array[MONK]*5+class_array[MARTIALMAN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[NINJA]);
		break;
	    case SECT_MOUNTAIN:
		chance+=(class_array[WARLORD]*10+class_array[SHAMAN]*5+class_array[MARTIALMAN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[NINJA]);
		break;
	    case SECT_WATER_SWIM:
	    case SECT_WATER_NOSWIM:
	    case SECT_UNDERWATER:
		chance+=(class_array[MONK]*10+class_array[NINJA]*5+class_array[ASSASSIN]*5);
		chance-=((class_array[KNIGHT]<<1)+class_array[LORD]);
		break;
	    case SECT_AIR:
		chance+=(class_array[NINJA]*10+class_array[SHAMAN]*5+class_array[ASSASSIN]*5);
		chance-=((class_array[WARLORD]<<1)+class_array[MONK]);
		break;
	    case SECT_DESERT:
		chance+=(class_array[WARLORD]*10+class_array[MONK]*5+class_array[KNIGHT]*5);
		chance-=((class_array[RANGER]<<1)+class_array[ASSASSIN]);
		break;
	}

	//final chance from 12.5% to 100%
	chance = URANGE((get_curr_wis(ch)+get_curr_int(ch))>>1,chance,100);
	if(number_percent() > chance){
	    send_to_char("�A�]��ĭx�����ϳQ�ѯ}�I\n\r",ch);
	    return;
	}
	send_to_char("�A���\\�a�]��ĭx�I\n\r",ch);
	act("$n���\\�a�����A�X��]�ۼĭx�I",ch, NULL, NULL, TO_ROOM);

	for(rch = ch->in_room->people;rch;rch=rch->next_in_room){
	    if(rch->deleted)continue;
	    if(IS_NPC(rch) && rch->fighting && is_same_group(rch->fighting, ch)){
		affect_strip( rch, gsn_snare );
		af.type      = gsn_snare;
		af.duration  = 1 + sklv;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_HOLD;
		affect_to_char( rch, &af );
		send_to_char("�A�Q�ĤH�]��F�I\n\r", rch);
		act("�A���Q�]��$N�I",ch, NULL, rch, TO_CHAR);
		mprog_hurt_trigger(rch, ch, gsn_snare);
	    }
	}
	WAIT_STATE( ch, skill_table[gsn_snare].beats<<1 );
}

