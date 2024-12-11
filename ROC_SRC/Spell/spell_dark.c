/* $Id: spell_dark.c 925 2005-05-10 07:43:30Z keric $ */
#include "skill.h"

extern void align_effect( CHAR_DATA *gch );
extern bool IsPK(CHAR_DATA *ch, CHAR_DATA *victim);

// Apocalypse add by whisperwind 11.13.2004
void spell_apocalypse ( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim, *victim_next;

    int dam;
    int ali_diff =0;
    int sklv = get_skill_level(ch,sn);
	int obj_lv = level;
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

    send_to_char( HIB "�A���e�X�{�F�·t�������A�i���R���a�¼ĤH���h�I\n\r"NOR"", ch );
    act( HIB "�·t�������ۦa��«�X�A�P"NOR"$n"HIB"���]�O�X�@�A�Ʀ��¦⪺���V���~�����ӨӡI"NOR"", ch, NULL, NULL, TO_ROOM );
    
    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
		victim_next = victim->next_in_room;
		if ( is_same_group( ch, victim ) )
		{
			continue;
		}
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
			continue;
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
				continue;
		}
		if ( IS_NPC( ch ) )
		{
			if ( victim->fighting == ch || !IS_NPC( victim ) )
			{
				ali_diff = 0;
				if ( victim ) ali_diff = victim->alignment - ch->alignment;
				dam = 0;
				dam  = (40 + ch->level)/2;
				dam = dam * wis_effect( ch );
					
				if( sklv > 0 ) dam = dam * ( 10 + sklv) / 10 ;
				if( sklv > 3 ) dam *= 1.2;
				if( sklv > 4 ) dam *= 1.2;
				if( sklv > 5 ) dam *= 1.2;

				dam = dam * weakness_damage( victim, sn ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
				dam = number_range( dam * 5.8 , dam * 7.2 );
				dam += ali_diff / 10;

				magic__damage( ch, victim, dam, sn, WEAR_NONE );
			}
		}
		else if ( victim->fighting == ch || ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
       	{
			ali_diff = 0;
			if ( victim ) ali_diff = victim->alignment - ch->alignment;
			dam = 0;
			dam = (40 + ch->level)/2;
			dam = dam * wis_effect( ch );

			if( sklv > 0 ) dam = dam * ( 10 + sklv) / 10 ;
			if( sklv > 3 ) dam *= 1.2;
			if( sklv > 4 ) dam *= 1.2;
			if( sklv > 5 ) dam *= 1.2;

			dam = dam * weakness_damage( victim, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
			dam = number_range( dam * 11 , dam * 13 );
			dam += dam * ali_diff / 4000;

			magic__damage( ch, victim, dam, sn, WEAR_NONE );
       	}
    }
}

// dispel good modified by whisperwind 8.23.2004
void spell_dispel_good( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

	int ali_diff = 0;

	if( victim ) ali_diff = victim->alignment - ch->alignment;

	if ( !IS_NPC( ch ) && IS_GOOD( ch )) {
		send_to_char( "�A�����F�L�k���E���c���O�q.\n\r", ch );
		return;
	}

	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam = dam/3;
	}

	if ( IS_EVIL( victim ) )
	{
		act( "�@����ª��O�qŢ�n�F $N.", ch, NULL, victim, TO_ROOM );
		act( "$N �ݨӧ��������v�T.", ch, NULL, victim, TO_CHAR );
		return;
	}

	dam = dam * wis_effect( ch );

	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam = dam * (1 + get_skill_level(ch, gsn_inborn_ability)/12);

	if( sklv > 0 ) dam = dam * ( 10 + sklv) / 10 ;
	if( sklv > 2 ) dam = dam * 1.08;
	if( sklv > 3 ) dam = dam * 1.08;
	if( sklv > 4 ) dam = dam * 1.08;
	if( sklv > 5 ) dam = dam * 1.08;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( old_saves_spell( ch->pp, victim,MAGIC_DARK)) / 60;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.5 , dam * 3.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1 , dam * 2 );

	dam += ali_diff / 10;

	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}

// finger of death add by whisperwind 11.13.2004
void spell_finger_of_death ( int sn, int level, CHAR_DATA *ch, void *vo)
{
	int dam;
	int nChance = 10;
	char buf	[MAX_INPUT_LENGTH];
 	CHAR_DATA *victim = (CHAR_DATA *) vo;
 	
 	int ali_diff =0;
	if ( victim ) ali_diff = victim->alignment - ch->alignment;

	nChance = nChance *( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 200;
	
	nChance = nChance * weakness_percent(victim, sn)/100;

	// �a���t���N�B�~��K 2022/10/26
	if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
		&& number_percent() < ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) ) )
	{
		if( is_affected( victim, gsn_sanctuary) && number_percent() > ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) )/2 ){
			affect_strip( victim, gsn_sanctuary );
			send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
		}
	}

	if ( victim->level < ch->level + 5 && number_percent() < nChance )
	{
		act( HIB "�����N�������X�{�b"NOR"$N"HIB"������A���_��W���I�M��$E�A�h�I"NOR"", ch, NULL, victim, TO_CHAR    );
		act( HIB "�����N�������X�{�b�A������A�A���N�ѳv���ҽk...."NOR"", ch, NULL, victim, TO_VICT    );
		act( HIB "�����N�������X�{�b"NOR"$N"HIB"������A���_��W���I�M��$E�A�h�I"NOR"", ch, NULL, victim, TO_NOTVICT );
		sprintf( buf, "self spell_finger_of_death %d 0 0 0 0 1", -1 );
		do_add_qmark( victim, buf );
		dam = 9999;
		if(IS_NPC(ch))
			victim->hit = 0;
		magic__damage( ch, victim, dam, sn, WEAR_NONE );
		ch->mana -= 200;
		WAIT_MAGIC_STATE(ch, 12 - get_skill_level(ch, gsn_spellcraft) * 2);
		return;
	}
	
	dam = (40 + ch->level)/2 ;
		
	dam = dam * int_effect( ch );

		dam = dam * 2.2;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
		
	if( !IS_NPC(ch) )
		dam = number_range( dam * 4 , dam * 7 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 3 , dam * 6 );
		
	dam += ali_diff / 10;
		
	magic_damage( ch, victim, dam, sn, WEAR_NONE );
	WAIT_MAGIC_STATE(ch, 12 - get_skill_level(ch, gsn_spellcraft) * 2);
	return;
}

void spell_dark_containment( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	
	int    ali_diff = 0;
	int    nSklv    = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv   = level;
	int nChance = 0;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}
	
	if( victim ) ali_diff = victim->alignment - ch->alignment;
	if ( !IS_NPC( ch ) && IS_GOOD( ch )) {
		send_to_char( "�A�����F�L�k���E���c���O�q.\n\r", ch );
		return; 
	}
	
	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam = dam/2;
	}
	
	dam = dam * wis_effect( ch );

	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

	if( nSklv > 0 )
		dam = dam * ( 12 + nSklv * (nSklv + 4) / 3) / 10 ;
	if( nSklv > 2 )
		dam = dam * 1.08;
	if( nSklv > 3 )
		dam = dam * 1.08;
	if( nSklv > 4 )
		dam = dam * 1.08;
	if( nSklv > 5 )
		dam = dam * 1.08;
	
	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
	
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.1 , dam * 2.3 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.25 , dam * 1.75 );
	
	dam += ali_diff / 10;

	// �� mystic armor ���@�έt�� 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
	{
		nChance = 5 + ali_diff/100;
		nChance = nChance + NEW_GET_SVS(victim, MAGIC_DARK) - NEW_GET_SVS(ch, MAGIC_DARK); // NEW_GET_SVS �t�Ȭ�����
		nChance = (nChance * ( 36 + nSklv * nSklv ) )/36;
		// inborn DETECT_ALIGN ���B�~�[�� 2022/12/12
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
			nChance = (nChance * ( 120 + get_skill_level(ch, gsn_inborn_ability) * get_skill_level(ch, gsn_inborn_ability) ) )/120;
		if( IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD) ||
			IS_SET( race_table[victim->race].race_abilities , RACE_VAMPIRE) ||
			IS_AFFECTED( victim, AFF_GHOUL ) || // aff_ghoul ���P�� race �ܬ� undead 2022/01/16
			is_affected(victim, gsn_stone_form))
		{
			nChance = nChance/3;
		}
		nChance = nChance * weakness_percent( victim, sn) / 100;
		// �a���t���N�B�~��K 2022/10/26
		if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
			&& number_percent() < ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) ) )
		{
			if( is_affected( victim, gsn_sanctuary) && number_percent() > ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) )/2 ){
				affect_strip( victim, gsn_sanctuary );
				send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
			}
			nChance = nChance/2;
		}
		if( nChance > number_percent() && !is_affected(victim, gsn_dark_containment) )
		{
			af.type      = sn;
			af.duration  = 1;
			af.bitvector = AFF_CURSE;
			af.location  = APPLY_SAVING_DARK;
			af.modifier  = 1 + nSklv*4;
			affect_to_char( victim, &af );
			act( ""HIB"�A������Ƭ��E��µ�"NOR"$N"HIB"�������k���ӤW�C"NOR"", ch, NULL, victim, TO_CHAR );
			act( ""HIB"�@�����������E��v���Ǻ�A�����X�C"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$n"HIB"������Ƭ��E��µ�"NOR"$N"HIB"�������k���ӤW�C"NOR"", ch, NULL, victim, TO_NOTVICT );    
		}
	}
	magic_damage( ch, victim, dam, sn, WEAR_NONE );
	return;
}

void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	
	int    nSklv    = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv   = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

	if ( !IS_NPC( ch ) && IS_GOOD( ch )) {
		send_to_char( "�A�����F�L�k���E���c���O�q.\n\r", ch );
		return; 
	}
	/* remove neutral damage limit 18/6/13
	if ( !IS_NPC( ch ) && IS_NEUTRAL( ch )) {
		dam = dam/2;
	}
	*/
	dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

	if( nSklv > 0 )
		dam = dam * ( 10 + nSklv) / 10 ;
	if( nSklv > 2 )
		dam = dam * 1.08;
	if( nSklv > 3 )
		dam = dam * 1.08;
	if( nSklv > 4 )
		dam = dam * 1.08;
	if( nSklv > 5 )
		dam = dam * 1.09;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
	//vch align effect 18/6/13
	dam = dam + dam * (victim->alignment + UMIN(2 * ch->amdr , 1000))/2000;
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.2 , dam * 3.2 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam  , dam * 1.5 );

	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}

void spell_lifesteal( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        nSklv  = get_skill_level( ch, sn);
	bool       isNeu  = (!IS_NPC( ch ) && IS_NEUTRAL( ch ));
	double dam; // basic value;
	int    obj_lv   = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}
  
	if ( !IS_NPC( ch ) && IS_GOOD( ch )) {
		send_to_char( "�A�����F�L�k���E���c���O�q.\n\r", ch );
		return;
	}

	dam = dam * wis_effect( ch );
	
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
		dam *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

	if( nSklv > 0 )
		dam = dam * ( 10 + nSklv) / 10 ;
	if( nSklv > 2 ) dam = dam * 1.08;
	if( nSklv > 3 ) dam = dam * 1.07;
	if( nSklv > 4 ) dam = dam * 1.06;
	if( nSklv > 5 ) dam = dam * 1.05;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * new_saves_spell( ch->pp, victim, MAGIC_DARK) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 3.5 , dam * 4.5 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.5 );

	if(victim->hit < 0) dam = URANGE(0, dam, victim->hit );

	magic_damage( ch, victim, dam, sn, WEAR_NONE );
	
	// affect by no heal wouldn't gain hitpoint add at 2018.11.20
	if( IS_AFFECTED(ch , AFF_NO_HEAL ) ) return;
	
	if ( dam < 0 ){
		send_to_char( "�L���ͮ𾮦Ӥ����A�A���ͩR�ϦӳQ�o�ޥX�h�I\n\r", ch );
		ch->hit	+= dam;
	}else{
		// �a���t���N�B�~��K 2022/10/26
		if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
			&& NEW_GET_SVS(ch, MAGIC_DARK) > NEW_GET_SVS(victim, MAGIC_DARK) )
			dam = ( dam * 2 ) / 3;
		if(!isNeu)
			ch->hit += dam * (1 + nSklv * nSklv ) / 74; // hp gain max 50% damage
		else
			ch->hit += dam * (1 + nSklv * nSklv) / 50; // hp gain max 74% damage
	}
	ch->hit = UMIN( ch->hit, (ch->max_hit + ch->max_hit/4 ));

	return;
}

void spell_soulsteal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int        dam    = 0;
    int        nSklv  = get_skill_level(ch,sn);
    int        nAffNum= 0;
	int        obj_lv = level;
	
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
	}

    if( !class_table[ch->class].fMana ) return;
    
    if ( !IS_NPC( ch ) && !IS_EVIL( ch ) && nSklv < 5)
    {
        send_to_char( "�A�����F�L�k���E���c���O�q.\n\r", ch );
        return; 
    }

    if ( saves_spell( ch->pp, victim ) < (number_percent( ) * 100 / weakness_percent( victim, sn)) )
    {
        send_to_char( "����]�S�o�͡C\n\r", ch );
        return;
    }
    
    dam = 3 * (dice(1, ch->level + nSklv * 2) + ( INT_DIFF( ch, victim ) * 5 ));
    if (dam > 0) {
      dam = dam * (10 + nSklv) / 10;
      dam = dam * new_saves_spell( ch->pp, victim, MAGIC_DARK) / 100;
    } else {
      dam = dam * 10 / (10 + nSklv);
      dam = dam * new_saves_spell( victim->pp, ch, MAGIC_DARK) / 100;
    }
    dam   += ch->amdr;
    dam   -= UMAX(-150 + nSklv * nSklv * 2, victim->mdr);

    if(is_affected(ch, gsn_mana_shield))   nAffNum += 2;
    if(is_affected(ch, gsn_mystic_mirror)) nAffNum ++;
    if(IS_AFFECTED(ch,AFF_MYSTIC_ARMOR))   nAffNum ++;
	
	// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
	if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN)
		&& get_skill_level(ch, gsn_inborn_ability)){
		if(nAffNum > 0){
			nAffNum = UMAX( 0 , nAffNum - get_skill_level(ch, gsn_inborn_ability) );
		}
	}
    if(nAffNum > 0){
        if(nSklv < 6){
            send_to_char("�A�g�D���k�N����A�I�i�]�O���P�C\n",ch);
            return;
        }else{
            nAffNum = 2 + nAffNum;
        }
    }

    if(nAffNum <= 0){
        if(!IS_NPC(ch) && !IS_EVIL(ch)){
            if(IS_GOOD(ch))
                dam  = dam / 2;
            else
                dam  = dam * 3 / 4;
        }
    }else{
        if(!IS_NPC(ch) && !IS_EVIL(ch)){
            if(IS_GOOD(ch))
                nAffNum += 2;
            else
                nAffNum += 1;
        }
        dam /= nAffNum;
    }

    if ( !victim->fighting )
         set_fighting( victim, ch );
    if ( !ch->fighting )
        set_fighting( ch, victim );

    dam = dam * weakness_damage( victim, sn ) / 100;
    if(IsPK(ch, victim)) dam /= PK_DMG_DENOMINATOR;

    multi_hit( victim, ch, TYPE_UNDEFINED );
    if ( victim->mana < dam )
    {
        if(victim->mana <= 0){
            send_to_char( "�o�ӥi�����å�w�g�Q�c���F�I\n\r", ch );
            return;
        }else{
            send_to_char( "�o�ӥi�����å�w�g�Q�c���F�I\n\r", ch );
            dam = victim->mana;
        }
    }

    victim->mana -= dam;
    victim->mana  = UMIN( victim->mana, (victim->max_mana + (victim->max_mana / 10)));
    ch->mana     += dam;
    ch->mana      = UMIN( ch->mana, (ch->max_mana + (ch->max_mana / 5)));
    ch->mana      = UMAX( ch->mana, -100);

    if ( dam >= 0 )
    {
        send_to_char( "�A�q��⨺����o�@���]�O�ɥR�C\n\r", ch );	
        send_to_char( "�A���k�O�V�~�y���I\n\r", victim );
    }else{
        send_to_char( "�]�O�Ͼ��I\n\r", ch );
        send_to_char( "�A�]�]�O�Ͼ��q��⨺����o�@���]�O�ɥR�I\n\r", victim );
    }
    return;
}

/*
 * Drain XP, MANA, HP.
 * Caster gains HP, XP.
 */
void spell_energy_drain( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int nDamage, nHpDmg, nMpDmg, nMvDmg;
	int nSklv  = get_skill_level(ch,sn);
	int obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
	}
	
	if ( saves_spell( ch->pp, victim ) < (number_percent( ) * 100 / weakness_percent( victim, sn)) ){
		send_to_char( "����]�S�o�͡C\n\r", ch );
		return;
	}

	ch->alignment = UMAX(-1000, ch->alignment - 8);

	if ( victim->level <= 10 ){
		nHpDmg   = ch->hit + 1;
	}else{
		nDamage  = dice( 5 + nSklv / 3, ch->level * (100 + nSklv * (nSklv + 4)) / 100);
		nDamage  = nDamage * new_saves_spell(ch->pp, victim, MAGIC_DARK) / 100;
		nDamage  = nDamage * weakness_damage(victim, sn ) / 100;
		if(nSklv > 2) nDamage = nDamage * 1.08;
		if(nSklv > 3) nDamage = nDamage * 1.08;
		if(nSklv > 4) nDamage = nDamage * 1.08;
		if(nSklv > 5) nDamage = nDamage * 1.08;

		// �[�J aff_ghoul �]�w 2022/01/16
		if( IS_SET(race_table[victim->race].race_abilities , RACE_UNDEAD) || IS_AFFECTED( victim, AFF_GHOUL ) ) 
			nDamage = nDamage / 3 - victim->level * 3;

		// inborn DETECT_ALIGN �� race (fairy , gnome) ���B�~�[�� 2020/05/29
		if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
			nDamage = nDamage * (1 + get_skill_level(ch, gsn_inborn_ability)/20);

		nHpDmg   = nDamage * (40 - get_curr_con(victim)) / 20;
		nMpDmg   = (nDamage + ch->amdr) * (40 - get_curr_wis(victim)) / 20;
		nMvDmg   = (nDamage + ch->amdr) * (40 - get_curr_dex(victim)) / 20;

		nMpDmg   = nMpDmg * number_range( 5,  8 + nSklv * 2) / 100;
		nMvDmg   = nMvDmg * number_range(10, 22 + nSklv * 3) / 100;

		nMpDmg  -= victim->mdr;
		nMvDmg  -= victim->mdr;
		if(IsPK(ch, victim)){
			nMpDmg /= PK_DMG_DENOMINATOR;
			nMvDmg /= PK_DMG_DENOMINATOR;
		}
		// �� mystic armor ���@�έt�� 2020/10/26
		if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
			if(nMpDmg < 0){
				if(number_percent() < get_curr_wis(victim) + victim->pp){
					nMpDmg        = nMpDmg * new_saves_spell(victim->pp, ch, MAGIC_DARK) / 80;
					ch->mana     += nMpDmg;
					victim->mana  = UMIN(victim->mana - nMpDmg, victim->max_mana * 11 / 10);
					send_to_char( "�]�O�Ͼ��I\n\r", ch );
					send_to_char( "�A�]�]�O�Ͼ��q��⨺����o�@���]�O�ɥR�I\n\r", victim );
				}
			}else if(number_percent() < nSklv * 5 + get_curr_int(ch) - get_curr_wis(victim)){
				if(nMpDmg > victim->mana) nMpDmg = victim->mana;
				victim->mana -= nMpDmg;
				if(number_percent() < get_curr_int(ch) + ch->pp / 2){
					ch->mana   += nMpDmg * (nSklv + 10) / 10;
					ch->mana    = UMIN(ch->mana, ch->max_mana * 11 / 10);
					send_to_char( "�A�q��⨺����o�@���]�O�ɥR�C\n\r", ch );
				}
				send_to_char( "�A���k�O�V�~�y���I\n\r", victim );
			}
			if(nMvDmg < 0){
				if(number_percent() < get_curr_dex(victim) + victim->pp){
					nMvDmg        = nMvDmg * new_saves_spell(victim->pp, ch, MAGIC_DARK) / 80;
					ch->move     += nMvDmg;
					victim->move  = UMIN(victim->move - nMvDmg, victim->max_move * 11 / 10);
					send_to_char( "��O�Ͼ��I\n\r", ch );
					send_to_char( "�A�]��O�Ͼ��q��⨺����o�@����O�ɥR�I\n\r", victim );
				}
			}else if(number_percent() < nSklv * 5 + get_curr_int(ch) - get_curr_dex(victim)){
				if(nMvDmg > victim->move) nMvDmg = victim->move;
				victim->move -= nMvDmg;
				if(number_percent() < get_curr_int(ch) + ch->pp / 2){
					ch->move   += nMvDmg * (nSklv + 10) / 10;
					ch->move    = UMIN(ch->move, ch->max_move * 11 / 10);
					send_to_char( "�A�q��⨺����o�@����O�ɥR�C\n\r", ch );
				}
				send_to_char( "�A����O�V�~�y���I\n\r", victim );
			}
			// affect by no heal wouldn't gain hitpoint add at 2018.11.20
			if(nHpDmg > 0 && number_percent() < nSklv * 5 + get_curr_int(ch) - get_curr_con(victim) * 2
				&& !IS_AFFECTED(ch , AFF_NO_HEAL ))
			{
				if(!IS_NPC(ch) && !IS_NPC(victim)){
					ch->hit += nHpDmg * (nSklv + 10) / 100;
				}else{
					ch->hit += nHpDmg * (nSklv + 10) /  30;
				}
				send_to_char( "�A�q��⨺����o�@�ǥͩR�O�ɥR�C\n\r", ch );
				ch->hit  = UMIN(ch->hit, ch->max_hit * 2);
			}
		}
	}

	magic_damage( ch, victim, nHpDmg, sn, WEAR_NONE );
	if ( !IS_NPC(ch) )
		align_effect( ch );

	return;
}

// Hell Southencross add by whisperwind 10.31.2004
void spell_hell_southencross ( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
 	
    int    sklv = get_skill_level(ch,sn);
    int    dam;
	int    obj_lv = level;

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
    
    send_to_char( HIB "�@�Ѷ·t���O�q�r�M«�W�����A�ô¥|�P���o�X�h�I\n\r"NOR"", ch );
    act(  "$n "HIB"�Y�W�B�{�X�|����´���A���ծ𮧻P�·t�i�ʦ۴���¥|�P�o���A�������F�I"NOR"", ch, NULL, NULL, TO_ROOM );
    for( victim = ch->in_room->people; victim; victim = victim_next)
    {
		victim_next = victim->next_in_room;
		if ( is_same_group( ch, victim ) )
		{
			continue;
		}
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
			continue;
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
			continue;
		}
		int orig_hp = (victim->max_hit) - affected_value(victim,sn,APPLY_HIT);
		int orig_mp = (victim->max_mana) - affected_value(victim,sn,APPLY_MANA);
		int orig_mv = (victim->max_move) - affected_value(victim,sn,APPLY_MOVE);
		if ( IS_NPC( ch ) )
		{
			if ( victim->fighting == ch || !IS_NPC( victim ) )
			{
				dam = 0;
				dam = (40 + ch->level)/2 ;
				dam = dam * int_effect( ch );
					
				if( sklv > 0 )
					dam = dam * ( 10 + sklv) / 10 ;
				if( sklv > 3 )
						dam *= 1.1;
				if( sklv > 4 )
						dam *= 1.1;
				if( sklv > 5 )
						dam *= 1.1;

					dam = dam * weakness_damage( victim, sn ) / 100;
					dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
				dam  = number_range( dam * 9 , dam * 11 );
				dam -= victim->mdr*5 ;
				
				if(magic_damage( ch, victim, dam, sn, WEAR_NONE ) == -2 )
					continue;
				// �� mystic armor ���@�έt�� 2020/10/26
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
					continue;
				// �a���t���N�B�~��K 2022/10/26
				if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
					&& number_percent() < ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) ) )
				{
					if( is_affected( victim, gsn_sanctuary) && number_percent() > ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) )/2 ){
						affect_strip( victim, gsn_sanctuary );
						send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
					}
					continue;
				}
				/*
				af.type      = sn;
				af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
				af.bitvector = 0;
				af.location  = APPLY_SAVING_DARK;
				af.modifier  = 3 + sklv + sklv/3;
				affect_join( victim, &af );

				af.location = APPLY_HIT;
				af.modifier = -(victim->max_hit/10);
				affect_join( victim, &af );

				af.location = APPLY_MANA;
				af.modifier = -(victim->max_mana/10);
				affect_join( victim, &af );
					
				af.location = APPLY_MOVE;
				af.modifier = -(victim->max_move/10);
				affect_join( victim, &af );
				*/

				af.type      = sn;
				af.bitvector = 0;
				// �ק�@�ήɶ���debuff �W�� 2020/11/06
				if( affected_value(victim,sn,APPLY_SAVING_DARK) < (affected_value(victim,sn,APPLY_SAVING_DARK) - NEW_GET_SVS(victim, MAGIC_DARK) ) ){
					if(affected_value(victim,sn,APPLY_SAVING_DARK) > 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_SAVING_DARK;
					af.modifier  = 4 + sklv;
					affect_join( victim, &af );
				}
				
				if( affected_value(victim,sn,APPLY_HIT) > -(orig_hp/2) ){
					if(affected_value(victim,sn,APPLY_HIT) < 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_HIT;
					af.modifier  = -(orig_hp/10);
					affect_join( victim, &af );
				}

				if(affected_value(victim,sn,APPLY_MANA) > -(orig_mp/2) ){
					if(affected_value(victim,sn,APPLY_MANA) < 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_MANA;
					af.modifier  = -(orig_mp/10);
					affect_join( victim, &af );
				}

				if(affected_value(victim,sn,APPLY_MOVE) > -(orig_mv/2) ){
					if(affected_value(victim,sn,APPLY_MOVE) < 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_MOVE;
					af.modifier  = -(orig_mv/10);
					affect_join( victim, &af );
				}
			}
		}
		else if ( victim->fighting == ch
				|| ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
		{
			dam = 0;
			dam = (40 + ch->level)/2 ;
			dam = dam * int_effect( ch );
					
			if( sklv > 0 )
				dam = dam * ( 10 + sklv) / 10 ;
			if( sklv > 3 )
				dam *= 1.1;
			if( sklv > 4 )
				dam *= 1.1;
			if( sklv > 5 )
				dam *= 1.1;

			dam = dam * weakness_damage( victim, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) / 100;
			dam = number_range( dam * 9 , dam * 11 );
			dam += UMAX( 0, victim->mdr*5 );
				
			if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
				continue;
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			// �a���t���N�B�~��K 2022/10/26
			if( ( IS_AFFECTED( victim, AFF_SANCTUARY ) || IS_SET(race_table[victim->race].race_abilities, RACE_SANCT) )
				&& number_percent() < ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) ) )
			{
				if( is_affected( victim, gsn_sanctuary) && number_percent() > ( 0 - NEW_GET_SVS(victim, MAGIC_DARK) )/2 ){
					affect_strip( victim, gsn_sanctuary );
					send_to_char( "�A���t���N�Q�}�a�F.\n\r", victim );
				}
				continue;
			}
			/*
				af.type      = sn;
				af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
				af.bitvector = 0;
				af.location  = APPLY_SAVING_DARK;
				af.modifier  = 3 + sklv + sklv/3;
				affect_join( victim, &af );

				af.location = APPLY_HIT;
				af.modifier = -(victim->max_hit/10);
				affect_join( victim, &af );

				af.location = APPLY_MANA;
				af.modifier = -(victim->max_mana/10);
				affect_join( victim, &af );
					
				af.location = APPLY_MOVE;
				af.modifier = -(victim->max_move/10);
				affect_join( victim, &af );
			*/
			af.type      = sn;
			af.bitvector = 0;
			// �ק�@�ήɶ���debuff �W�� 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_DARK) < (affected_value(victim,sn,APPLY_SAVING_DARK) - NEW_GET_SVS(victim, MAGIC_DARK) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_DARK) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_DARK;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
				
			if( affected_value(victim,sn,APPLY_HIT) > -(orig_hp/2) ){
				if(affected_value(victim,sn,APPLY_HIT) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_HIT;
				af.modifier  = -(orig_hp/10);
				affect_join( victim, &af );
			}

			if(affected_value(victim,sn,APPLY_MANA) > -(orig_mp/2) ){
				if(affected_value(victim,sn,APPLY_MANA) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_MANA;
				af.modifier  = -(orig_mp/10);
				affect_join( victim, &af );
			}

			if(affected_value(victim,sn,APPLY_MOVE) > -(orig_mv/2) ){
				if(affected_value(victim,sn,APPLY_MOVE) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_MOVE;
				af.modifier  = -(orig_mv/10);
				affect_join( victim, &af );
			}
		}
    }
}
