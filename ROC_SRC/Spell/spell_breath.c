//*****************************************************************
//*  PROGRAM ID  : spell_breath                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0960212 Razgriz                                *
//*  PURPOSE     :         �N��� slash_breath � S �M B        *
//*****************************************************************/
#define BREATH_DEBUG   0

#include "skill.h"
#include <math.h>

//************************************************ /
//  �Ҧ��R���� 2022/04/17 �קאּ�H���ؼЦh���ˮ`   /
//  �ˮ`�����t�ݩʼv�T, �Ȭݥ��ݩʨå[�J�ˮ`�֭p   /
//  �s�����������ؼз|����󰪪��ˮ`             /
// ----------------------------------------------- /
//  �ª��R���m����U��                           /
//************************************************ /
int GetBreathDamage(int nBaseDmg, CHAR_DATA *ch, CHAR_DATA *victim, int nSklv, int nMainSVS, int sn)
{
	int nDamage  = 0;
	int nAttSVS  = NEW_GET_SVS(ch, nMainSVS);
	int nDefSVS  = NEW_GET_SVS(victim, nMainSVS)/2 + 10;
	int nAttBase = 40;
	int nDefBase = 50;

	// �s�� race �[��
	if(ch->race == race_lookup("Dragon"))
		nSklv += 2;
	else if(ch->race == race_lookup("Drake"))
		nSklv += 1;
	else if(ch->race == race_lookup("Wyrm"))
		nSklv += 2;
	else if(ch->race == race_lookup("Wyvern"))
		nSklv += 1;
	else if(ch->race == race_lookup("Hydra"))
		nSklv += 2;

	if(IS_NPC(ch))
		nAttSVS += 20;

	if(!IS_NPC(victim)) {
		nAttBase += 10;
		nDefBase -= 25;
	}
	nAttBase -= nSklv * 2;
	if(nSklv > 4) nAttBase --;
	if(nSklv > 5) nAttBase -= 2;

	if( ch->max_hit <= 0 )
		return;

	// �� damage �|�� hp �ʤ���U����̦h 1/3 2022/04/16
	if( ch->max_hit > ch->hit )
		nBaseDmg = UMAX( nBaseDmg/3 , (nBaseDmg * ch->hit) / ch->max_hit );
	
	// mob ���e�@��
	if( IS_NPC(ch) && ch->position == POS_DEAD )
		nBaseDmg = nBaseDmg * 3;

	// �Q�s��R���W�� , sklv 6 ���X 3000 �ˮ`�C�U�֭p�Ȭ��� 165 �W�U �֭p�������g�b fight_damage 2022/04/09
	if( affected_value(victim, sn, APPLY_DAMAGE_COUNT) > 0 ){
		if( IS_NPC(ch) )
			nBaseDmg = nBaseDmg + affected_value(victim, sn, APPLY_DAMAGE_COUNT)/14;
		else
			nBaseDmg = nBaseDmg + affected_value(victim, sn, APPLY_DAMAGE_COUNT)/11;
	}
	
	// �r���P�w�P�@�뤣�P 2022/04/09
	if( sn == gsn_gas_breath && affected_value(victim, sn, APPLY_SAVING_POISON) > 0 ){
		if( IS_NPC(ch) )
			nBaseDmg = nBaseDmg + affected_value(victim, sn, APPLY_SAVING_POISON)/14;
		else
			nBaseDmg = nBaseDmg + affected_value(victim, sn, APPLY_SAVING_POISON)/11;
	}

	if(nAttSVS < 0){
		nDamage = nBaseDmg * (nAttBase - nAttSVS) / nAttBase;
	}else{
		nDamage = nBaseDmg * nAttBase / (nAttBase + nAttSVS);
	}

	if(nDefSVS < 0){
		nDamage = nDamage * nDefBase / (nDefBase - nDefSVS);
	}else{
		nDamage = nDamage * (nDefBase + nDefSVS) / nDefBase;
	}

	return nDamage;
}

void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj_lose;
    OBJ_DATA  *obj_next;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

	int nSklv = get_skill_level(ch,sn);
	int obj_lv = level;
	int nCurrHP = 0;
	int nDamage = 0;
	int nMainState = 0;
	int EnemyCount = 0;
	int times = 0;
	int mana_cost = 0;
	int nRandDam = 0;
	int old_nDamage = 0;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);
	int HitCount = 0;
	bool Pc_Group = FALSE; // 2022/12/28
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rand_vch;
	
	if( ch->max_hit <= 0)
		return;

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

	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			mana_cost = 80;
		else
			mana_cost = 200;

		mana_cost = mana_cost - (( mana_cost * (nSpellSklv * nSpellSklv / 2 + nSpellSklv * 2 + 10)) / 50 );

		if( ch->mana < mana_cost ){
			send_to_char( "�A���k�O�����F.\n\r", ch);
			return;
		}
	}

	if( ch->hit > ch->max_hit )
		nCurrHP = ch->hit;
	else
		nCurrHP = ch->max_hit;

	nCurrHP = UMIN(100000/6 * nSklv, nCurrHP/12 * ( 6 + nSklv ) );

	nDamage = (int)sqrt((int)(nCurrHP));

	if ( (number_percent() *100/ weakness_percent(victim, sn ) ) < 2 * ch->level && 
	      new_saves_spell( ch->pp, victim, MAGIC_BREATH ) > (number_percent() *100/ weakness_percent(victim, sn)) )
	{
		for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next ){
			int iWear;

			obj_next = obj_lose->next_content;
			if ( obj_lose->deleted ) continue;
			if( IS_SET(obj_lose->extra_flags, ITEM_BLESS)) continue;
			if ( number_bits( 2 ) != 0 ) continue;

			switch ( obj_lose->item_type ){
				case ITEM_ARMOR:
					if ( obj_lose->value[0] > 0 ){
						int ac;
						/* Correction on math by IvoryTiger */ 
						act( "$p �Q�j���q��A�v���l�a�ܧΡI", victim, obj_lose, NULL, TO_CHAR );
						if ( ( iWear = obj_lose->wear_loc ) != WEAR_NONE ){
							ac = apply_ac( obj_lose, iWear );
							if( !IS_NPC(victim) && victim->pcdata->learned[gsn_armor_mastery] > 0 )
							ac += ac/3;
							victim->armor += ac;
						}
						obj_lose->value[0] -= 1;
						obj_lose->value[1] += 1;
						obj_lose->cost	= 0;
						if ( iWear != WEAR_NONE ){
							ac = apply_ac( obj_lose, iWear );
							if( !IS_NPC(victim) && victim->pcdata->learned[gsn_armor_mastery] > 0 )
							ac += ac/3;
							victim->armor -= ac;
						}
					}
				break;
				case ITEM_CONTAINER:
				case ITEM_FURNITURE:
					act( "$p �Q�j���q��A�G�k���F�I",
					victim, obj_lose, NULL, TO_CHAR );
					extract_obj( obj_lose );
				break;
			}
		}
	}

	if( !IS_NPC(ch) ){
		if( ch->pcdata->prime_attr == APPLY_STR )
			nMainState = get_curr_str(ch);
		if( ch->pcdata->prime_attr == APPLY_INT )
			nMainState = get_curr_int(ch);
		if( ch->pcdata->prime_attr == APPLY_WIS )
			nMainState = get_curr_wis(ch);
		if( ch->pcdata->prime_attr == APPLY_DEX )
			nMainState = get_curr_dex(ch);
		if( ch->pcdata->prime_attr == APPLY_CON )
			nMainState = get_curr_con(ch);
	}else{
		nMainState = ( get_curr_str(ch) + get_curr_int(ch) + get_curr_wis(ch) + get_curr_dex(ch) + get_curr_con(ch) ) / 5;
	}

	old_nDamage = nDamage;

	// �Ĩt spell �����a�t�ޯ� 2022/04/06
	nDamage  = GetBreathDamage(nDamage, ch, victim, nSklv, MAGIC_EARTH, sn);
	nDamage  = number_range(nDamage * 5, nDamage * 5.5);
	nDamage  = nDamage * ( 85 + nMainState ) / 100;
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	// �H magic_damage ���� wpn �N�J�˳� sklv �H�p��ˮ`�᪺�[���ĪG 2022/04/16
	magic_damage( ch, victim, nDamage, sn, nSklv );


	for( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted )
			continue;

		if( is_same_group(ch, vch) ){
			// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) )
			{
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
			if( Pc_Group && !IS_NPC(vch) ){
				if( vch->fighting != ch )
					continue;
			}
		}else{
			if( !IS_NPC(vch) && vch->fighting != ch )
				continue;
		}
		times++;
	}
	
	times = UMAX( nSklv/2 , times * (1 + nSklv/12) );

	while( times-- )
	{
		EnemyCount = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people; vch; vch = vch_next ){
			vch_next = vch->next_in_room;
			QMARK *qm = NULL;

			// �C�h�@�U�� mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// �w�� wizinv �����p 2022/11/20
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS) ) continue;

			if( ( qm = is_qmark( vch, "acid_breath_damcount" ) ) != NULL ){
				if( times <= 0 )
				{
					if( IS_NPC(vch) )
						do_mpdel_qmark( vch, "self acid_breath_damcount" );
					else
						do_del_qmark( vch, "self acid_breath_damcount" );
					qm = NULL;

					if( vch == victim )
						continue;

				}else if( qm->v0 >= 2 )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_vch = vch;

			// �e�⦸���������|�Q�����w�ؼ�
			if( victim && vch == victim ){
				if( number_percent() > 50 + 35 * HitCount
					&& ch->in_room == victim->in_room
					&& !victim->deleted )
				{
					rand_vch = victim;
				}
			}

			EnemyCount++;
		}

		if( !rand_vch )
			continue;

		// �C�U�ˮ`���C
		nRandDam = old_nDamage * (18 + nSklv) /(25 + HitCount*2);
		nRandDam = GetBreathDamage(nRandDam, ch, rand_vch, nSklv, MAGIC_EARTH, sn);
		nRandDam = number_range(nRandDam * 5, nRandDam * 5.5);
		nRandDam = nRandDam * ( 85 + nMainState ) / 100;
		nRandDam = nRandDam * weakness_damage( rand_vch, sn ) / 100;

		if( !IS_NPC(ch) )
			ch->mana -= UMAX( 1 , (10 * mana_cost)/(10 + HitCount) );
		// �H magic_damage ���� wpn �N�J�˳� sklv �H�p��ˮ`�᪺�[���ĪG 2022/04/16
		magic_damage( ch, rand_vch, nRandDam, sn, nSklv );

		if( times > 0 ){
			if( is_qmark( rand_vch, "acid_breath_damcount" ) != NULL ){
				if( IS_NPC(rand_vch) )
					do_mpmod_qmark( rand_vch, "self acid_breath_damcount v0 + 1" );
				else
					do_mod_qmark( rand_vch, "self acid_breath_damcount v0 + 1" );
			}
			else
				do_add_qmark( rand_vch, "self acid_breath_damcount 2 1 0 0 1 0" );
		}

		HitCount++;
	}

	return;
}

void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA  *obj_lose;
	OBJ_DATA  *obj_next;
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int nSklv = get_skill_level(ch,sn);
	int obj_lv = level;
	int nCurrHP = 0;
	int nDamage = 0;
	int nMainState = 0;
	int EnemyCount = 0;
	int times = 0;
	int mana_cost = 0;
	int nRandDam = 0;
	int old_nDamage = 0;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);
	int HitCount = 0;
	bool Pc_Group = FALSE; // 2022/12/28
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rand_vch;

	if( ch->max_hit <= 0)
		return;

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

	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			mana_cost = 80;
		else
			mana_cost = 200;

		mana_cost = mana_cost - (( mana_cost * (nSpellSklv * nSpellSklv / 2 + nSpellSklv * 2 + 10)) / 50 );

		if( ch->mana < mana_cost ){
			send_to_char( "�A���k�O�����F.\n\r", ch);
			return;
		}
	}

	if( ch->hit > ch->max_hit )
		nCurrHP = ch->hit;
	else
		nCurrHP = ch->max_hit;

	nCurrHP = UMIN(100000/6 * nSklv, nCurrHP/12 * ( 6 + nSklv ) );

	nDamage = (int)sqrt((int)(nCurrHP));

	if ( (number_percent() *100/ weakness_percent(victim, sn ) ) < 2 * ch->level && 
	      get_skill_level(ch,sn)*get_skill_level(ch,sn) > (number_percent()*100/weakness_percent(victim,sn)))
	{
		for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next ){
			char *msg;

			obj_next = obj_lose->next_content;
			if ( obj_lose->deleted ) continue;
			if ( number_bits( 2 ) != 0 ) continue;

			if( IS_SET(obj_lose->extra_flags, ITEM_BLESS)) continue;

			switch ( obj_lose->item_type ){
				default:		 continue;
				case ITEM_CONTAINER: msg = "$p �D�s�V�i�ΡA��M�_���U�N�I";	  break;
				case ITEM_POTION:	 msg = "$p �D�s�V�i�ΡA�m�˦ӻ]�o�F�I";	  break;
				case ITEM_SCROLL:	 msg = "$p �D�s�V�i�ΡA�q��t�}�l���N�I";  break;
				case ITEM_STAFF:	 msg = "$p �D�s�V�i�ΡA�_�X�}�}�·ϡI";	  break;
				case ITEM_WAND:      msg = "$p �D�s�V�i�ΡA�q�X�I�I���P�I"; break;
				case ITEM_FOOD:      msg = "$p �D�s�V�i�ΡA�o�X�J����I"; break;
				case ITEM_PILL:      msg = "$p �D�s�V�i�ΡA�@�����ĤƤF�I";	  break;
				case ITEM_FURNITURE: msg = "$p �D�s�V�i�ΡA�N���@��J���I";  break;
			}

			act( msg, victim, obj_lose, NULL, TO_CHAR );
			if( obj_lose->item_type == ITEM_CONTAINER ){
				check_fire_container( obj_lose, victim );
			}
			extract_obj( obj_lose );
		}
	}


	if( !IS_NPC(ch) ){
		if( ch->pcdata->prime_attr == APPLY_STR )
			nMainState = get_curr_str(ch);
		if( ch->pcdata->prime_attr == APPLY_INT )
			nMainState = get_curr_int(ch);
		if( ch->pcdata->prime_attr == APPLY_WIS )
			nMainState = get_curr_wis(ch);
		if( ch->pcdata->prime_attr == APPLY_DEX )
			nMainState = get_curr_dex(ch);
		if( ch->pcdata->prime_attr == APPLY_CON )
			nMainState = get_curr_con(ch);
	}else{
		nMainState = ( get_curr_str(ch) + get_curr_int(ch) + get_curr_wis(ch) + get_curr_dex(ch) + get_curr_con(ch) ) / 5;
	}

	old_nDamage = nDamage;

	nDamage  = GetBreathDamage(nDamage, ch, victim, nSklv, MAGIC_FIRE, sn);
	nDamage  = number_range(nDamage * 5, nDamage * 5.75);
	nDamage  = nDamage * ( 85 + nMainState ) / 100;
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, nSklv );

	for( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted )
			continue;

		if( is_same_group(ch, vch) ){
			// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
			if( Pc_Group && !IS_NPC(vch) ){
				if( vch->fighting != ch )
					continue;
			}
		}else{
			if( !IS_NPC(vch) && vch->fighting != ch )
				continue;
		}
		times++;
	}
	
	times = UMAX( nSklv/2 , times * (1 + nSklv/12) );

	while( times-- )
	{
		EnemyCount = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people; vch; vch = vch_next ){
			vch_next = vch->next_in_room;
			QMARK *qm = NULL;

			// �C�h�@�U�� mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// �w�� wizinv �����p 2022/11/20
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS) ) continue;

			if( ( qm = is_qmark( vch, "fire_breath_damcount" ) ) != NULL ){
				if( times <= 0 )
				{
					if( IS_NPC(vch) )
						do_mpdel_qmark( vch, "self fire_breath_damcount" );
					else
						do_del_qmark( vch, "self fire_breath_damcount" );
					qm = NULL;

					if( vch == victim )
						continue;

				}else if( qm->v0 >= 2 )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_vch = vch;

			// �e�⦸���������|�Q�����w�ؼ�
			if( victim && vch == victim ){
				if( number_percent() > 50 + 35 * HitCount
					&& ch->in_room == victim->in_room
					&& !victim->deleted )
				{
					rand_vch = victim;
				}
			}

			EnemyCount++;
		}

		if( !rand_vch )
			continue;

		// �C�U�ˮ`���C
		nRandDam = old_nDamage * (18 + nSklv) /(25 + HitCount*2);
		nRandDam = GetBreathDamage(nRandDam, ch, rand_vch, nSklv, MAGIC_FIRE, sn);
		nRandDam = number_range(nRandDam * 5, nRandDam * 5.5);
		nRandDam = nRandDam * ( 85 + nMainState ) / 100;
		nRandDam = nRandDam * weakness_damage( rand_vch, sn ) / 100;

		if( !IS_NPC(ch) )
			ch->mana -= UMAX( 1 , (10 * mana_cost)/(10 + HitCount) );
		magic_damage( ch, rand_vch, nRandDam, sn, nSklv );

		if( times > 0 ){
			if( is_qmark( rand_vch, "fire_breath_damcount" ) != NULL ){
				if( IS_NPC(rand_vch) )
					do_mpmod_qmark( rand_vch, "self fire_breath_damcount v0 + 1" );
				else
					do_mod_qmark( rand_vch, "self fire_breath_damcount v0 + 1" );
			}
			else
				do_add_qmark( rand_vch, "self fire_breath_damcount 2 1 0 0 1 0" );
		}

		HitCount++;
	}
	return;
}

void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA  *obj_lose;
	OBJ_DATA  *obj_next;
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int nSklv = get_skill_level(ch,sn);
	int obj_lv = level;
	int nCurrHP = 0;
	int nDamage = 0;
	int nMainState = 0;
	int EnemyCount = 0;
	int times = 0;
	int mana_cost = 0;
	int nRandDam = 0;
	int old_nDamage = 0;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);
	int HitCount = 0;
	bool Pc_Group = FALSE; // 2022/12/28
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rand_vch;

	if( ch->max_hit <= 0)
		return;

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

	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			mana_cost = 80;
		else
			mana_cost = 200;

		mana_cost = mana_cost - (( mana_cost * (nSpellSklv * nSpellSklv / 2 + nSpellSklv * 2 + 10)) / 50 );

		if( ch->mana < mana_cost ){
			send_to_char( "�A���k�O�����F.\n\r", ch);
			return;
		}
	}

	if( ch->hit > ch->max_hit )
		nCurrHP = ch->hit;
	else
		nCurrHP = ch->max_hit;

	nCurrHP = UMIN(100000/6 * nSklv, nCurrHP/12 * ( 6 + nSklv ) );

	nDamage = (int)sqrt((int)(nCurrHP));

	if ( (number_percent() *100/ weakness_percent(victim, sn ) ) < 2 * level && 
	      get_skill_level(ch,sn)*get_skill_level(ch,sn) > (number_percent()*100/weakness_percent(victim,sn)))
	{
		for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next ){
			char *msg;

			obj_next = obj_lose->next_content;
			if ( obj_lose->deleted ) continue;
			if ( number_bits( 2 ) != 0 ) continue;
			if ( IS_SET(obj_lose->extra_flags, ITEM_BLESS) ) continue;

			switch ( obj_lose->item_type ){
				default:		continue;
				case ITEM_CONTAINER:
				case ITEM_DRINK_CON:
				case ITEM_POTION:	msg = "$p �Q�|�P���H��ᵲ�A�M��Ƭ��H���I"; break;
			}

			act( msg, victim, obj_lose, NULL, TO_CHAR );
			extract_obj( obj_lose );
		}
	}

	old_nDamage = nDamage;

	nDamage  = GetBreathDamage(nDamage, ch, victim, nSklv, MAGIC_COLD, sn);
	nDamage  = number_range(nDamage * 5, nDamage * 5.75);
	nDamage  = nDamage * ( 85 + nMainState ) / 100;
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, nSklv );

	for( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted )
			continue;

		if( is_same_group(ch, vch) ){
			// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
			if( Pc_Group && !IS_NPC(vch) ){
				if( vch->fighting != ch )
					continue;
			}
		}else{
			if( !IS_NPC(vch) && vch->fighting != ch )
				continue;
		}
		times++;
	}
	
	times = UMAX( nSklv/2 , times * (1 + nSklv/12) );

	while( times-- )
	{
		EnemyCount = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people; vch; vch = vch_next ){
			vch_next = vch->next_in_room;
			QMARK *qm = NULL;

			// �C�h�@�U�� mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// �w�� wizinv �����p 2022/11/20
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS) ) continue;

			if( ( qm = is_qmark( vch, "frost_breath_damcount" ) ) != NULL ){
				if( times <= 0 )
				{
					if( IS_NPC(vch) )
						do_mpdel_qmark( vch, "self frost_breath_damcount" );
					else
						do_del_qmark( vch, "self frost_breath_damcount" );
					qm = NULL;

					if( vch == victim )
						continue;

				}else if( qm->v0 >= 2 )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_vch = vch;

			// �e�⦸���������|�Q�����w�ؼ�
			if( victim && vch == victim ){
				if( number_percent() > 50 + 35 * HitCount
					&& ch->in_room == victim->in_room
					&& !victim->deleted )
				{
					rand_vch = victim;
				}
			}

			EnemyCount++;
		}

		if( !rand_vch )
			continue;

		// �C�U�ˮ`���C
		nRandDam = old_nDamage * (18 + nSklv) /(25 + HitCount*2);
		nRandDam = GetBreathDamage(nRandDam, ch, rand_vch, nSklv, MAGIC_COLD, sn);
		nRandDam = number_range(nRandDam * 5, nRandDam * 5.5);
		nRandDam = nRandDam * ( 85 + nMainState ) / 100;
		nRandDam = nRandDam * weakness_damage( rand_vch, sn ) / 100;

		if( !IS_NPC(ch) )
			ch->mana -= UMAX( 1 , (10 * mana_cost)/(10 + HitCount) );
		magic_damage( ch, rand_vch, nRandDam, sn, nSklv );

		if( times > 0 ){
			if( is_qmark( rand_vch, "frost_breath_damcount" ) != NULL ){
				if( IS_NPC(rand_vch) )
					do_mpmod_qmark( rand_vch, "self frost_breath_damcount v0 + 1" );
				else
					do_mod_qmark( rand_vch, "self frost_breath_damcount v0 + 1" );
			}
			else
				do_add_qmark( rand_vch, "self frost_breath_damcount 2 1 0 0 1 0" );
		}

		HitCount++;
	}
	return;
}


void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{

	CHAR_DATA *victim  = (CHAR_DATA *) vo;

	int nSklv = get_skill_level(ch,sn);
	int obj_lv = level;
	// 2022/04/10
	int nCurrHP = 0;
	int nDamage = 0;
	int nMainState = 0;
	int EnemyCount = 0;
	int times = 0;
	int mana_cost = 0;
	int nRandDam = 0;
	int old_nDamage = 0;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);
	int HitCount = 0;
	bool Pc_Group = FALSE; // 2022/12/28
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rand_vch;

	if( ch->max_hit <= 0)
		return;

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

	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			mana_cost = 80;
		else
			mana_cost = 200;

		mana_cost = mana_cost - (( mana_cost * (nSpellSklv * nSpellSklv / 2 + nSpellSklv * 2 + 10)) / 50 );

		if( ch->mana < mana_cost ){
			send_to_char( "�A���k�O�����F.\n\r", ch);
			return;
		}
	}

	if( ch->hit > ch->max_hit )
		nCurrHP = ch->hit;
	else
		nCurrHP = ch->max_hit;

	nCurrHP = UMIN(100000/6 * nSklv, nCurrHP/12 * ( 6 + nSklv ) );

	nDamage = (int)sqrt((int)(nCurrHP));

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{
		send_to_char("�o�̸T��԰�. \n\r",ch);
		return;
	}

	if( !IS_NPC(ch) ){
		if( ch->pcdata->prime_attr == APPLY_STR )
			nMainState = get_curr_str(ch);
		if( ch->pcdata->prime_attr == APPLY_INT )
			nMainState = get_curr_int(ch);
		if( ch->pcdata->prime_attr == APPLY_WIS )
			nMainState = get_curr_wis(ch);
		if( ch->pcdata->prime_attr == APPLY_DEX )
			nMainState = get_curr_dex(ch);
		if( ch->pcdata->prime_attr == APPLY_CON )
			nMainState = get_curr_con(ch);
	}else{
		nMainState = ( get_curr_str(ch) + get_curr_int(ch) + get_curr_wis(ch) + get_curr_dex(ch) + get_curr_con(ch) ) / 5;
	}

	old_nDamage = nDamage;

	nDamage  = GetBreathDamage(nDamage, ch, victim, nSklv, MAGIC_WIND, sn);
	nDamage  = number_range(nDamage * 5, nDamage * 5.75);
	nDamage  = nDamage * ( 85 + nMainState ) / 100;
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, nSklv );

	for( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted )
			continue;

		if( is_same_group(ch, vch) ){
			// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
			if( Pc_Group && !IS_NPC(vch) ){
				if( vch->fighting != ch )
					continue;
			}
		}else{
			if( !IS_NPC(vch) && vch->fighting != ch )
				continue;
		}
		times++;
	}
	
	times = UMAX( nSklv/2 , times * (1 + nSklv/12) );

	while( times-- )
	{
		EnemyCount = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people; vch; vch = vch_next ){
			vch_next = vch->next_in_room;
			QMARK *qm = NULL;

			// �C�h�@�U�� mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// �w�� wizinv �����p 2022/11/20
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS) ) continue;

			if( ( qm = is_qmark( vch, "gas_breath_damcount" ) ) != NULL ){
				if( times <= 0 )
				{
					if( IS_NPC(vch) )
						do_mpdel_qmark( vch, "self gas_breath_damcount" );
					else
						do_del_qmark( vch, "self gas_breath_damcount" );
					qm = NULL;

					if( vch == victim )
						continue;

				}else if( qm->v0 >= 2 )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_vch = vch;

			// �e�⦸���������|�Q�����w�ؼ�
			if( victim && vch == victim ){
				if( number_percent() > 50 + 35 * HitCount
					&& ch->in_room == victim->in_room
					&& !victim->deleted )
				{
					rand_vch = victim;
				}
			}

			EnemyCount++;
		}

		if( !rand_vch )
			continue;

		// �C�U�ˮ`���C
		nRandDam = old_nDamage * (18 + nSklv) /(25 + HitCount*2);
		nRandDam = GetBreathDamage(nRandDam, ch, rand_vch, nSklv, MAGIC_WIND, sn);
		nRandDam = number_range(nRandDam * 5, nRandDam * 5.5);
		nRandDam = nRandDam * ( 85 + nMainState ) / 100;
		nRandDam = nRandDam * weakness_damage( rand_vch, sn ) / 100;

		if( !IS_NPC(ch) )
			ch->mana -= UMAX( 1 , (10 * mana_cost)/(10 + HitCount) );
		magic_damage( ch, rand_vch, nRandDam, sn, nSklv );

		if( times > 0 ){
			if( is_qmark( rand_vch, "gas_breath_damcount" ) != NULL ){
				if( IS_NPC(rand_vch) )
					do_mpmod_qmark( rand_vch, "self gas_breath_damcount v0 + 1" );
				else
					do_mod_qmark( rand_vch, "self gas_breath_damcount v0 + 1" );
			}
			else
				do_add_qmark( rand_vch, "self gas_breath_damcount 2 1 0 0 1 0" );
		}

		HitCount++;
	}
	return;
}

void spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim  = (CHAR_DATA *) vo;

	int nSklv = get_skill_level(ch,sn);
	int obj_lv = level;
	int nCurrHP = 0;
	int nDamage = 0;
	int nMainState = 0;
	int EnemyCount = 0;
	int times = 0;
	int mana_cost = 0;
	int nRandDam = 0;
	int old_nDamage = 0;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);
	int HitCount = 0;
	bool Pc_Group = FALSE; // 2022/12/28
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rand_vch;

	if( ch->max_hit <= 0)
		return;

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

	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			mana_cost = 80;
		else
			mana_cost = 200;

		mana_cost = mana_cost - (( mana_cost * (nSpellSklv * nSpellSklv / 2 + nSpellSklv * 2 + 10)) / 50 );

		if( ch->mana < mana_cost ){
			send_to_char( "�A���k�O�����F.\n\r", ch);
			return;
		}
	}

	if( ch->hit > ch->max_hit )
		nCurrHP = ch->hit;
	else
		nCurrHP = ch->max_hit;

	nCurrHP = UMIN(100000/6 * nSklv, nCurrHP/12 * ( 6 + nSklv ) );

	nDamage = (int)sqrt((int)(nCurrHP));

	if( !IS_NPC(ch) ){
		if( ch->pcdata->prime_attr == APPLY_STR )
			nMainState = get_curr_str(ch);
		if( ch->pcdata->prime_attr == APPLY_INT )
			nMainState = get_curr_int(ch);
		if( ch->pcdata->prime_attr == APPLY_WIS )
			nMainState = get_curr_wis(ch);
		if( ch->pcdata->prime_attr == APPLY_DEX )
			nMainState = get_curr_dex(ch);
		if( ch->pcdata->prime_attr == APPLY_CON )
			nMainState = get_curr_con(ch);
	}else{
		nMainState = ( get_curr_str(ch) + get_curr_int(ch) + get_curr_wis(ch) + get_curr_dex(ch) + get_curr_con(ch) ) / 5;
	}

	old_nDamage = nDamage;

	nDamage  = GetBreathDamage(nDamage, ch, victim, nSklv, MAGIC_LIGHTNING, sn);
	nDamage  = number_range(nDamage * 5, nDamage * 5.5);
	nDamage  = nDamage * ( 85 + nMainState ) / 100;
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, nSklv );

	for( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted )
			continue;

		if( is_same_group(ch, vch) ){
			// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
			if( Pc_Group && !IS_NPC(vch) ){
				if( vch->fighting != ch )
					continue;
			}
		}else{
			if( !IS_NPC(vch) && vch->fighting != ch )
				continue;
		}
		times++;
	}
	
	times = UMAX( nSklv/2 , times * (1 + nSklv/12) );

	while( times-- )
	{
		EnemyCount = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people; vch; vch = vch_next ){
			vch_next = vch->next_in_room;
			QMARK *qm = NULL;

			// �C�h�@�U�� mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// �p�G�O pet �� npc (sor mob , pet ), �h�|�P�w����O�_�s�b pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// ����� pc �B vch �� pc , ���D vch �h�� ch �_�h �h���|�����L pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// �w�� wizinv �����p 2022/11/20
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS) ) continue;

			if( ( qm = is_qmark( vch, "lightning_breath_damcount" ) ) != NULL ){
				if( times <= 0 )
				{
					if( IS_NPC(vch) )
						do_mpdel_qmark( vch, "self lightning_breath_damcount" );
					else
						do_del_qmark( vch, "self lightning_breath_damcount" );
					qm = NULL;

					if( vch == victim )
						continue;

				}else if( qm->v0 >= 2 )
					continue;
			}

			if( number_range( 0, EnemyCount ) == 0 )
				rand_vch = vch;

			// �e�⦸���������|�Q�����w�ؼ�
			if( victim && vch == victim ){
				if( number_percent() > 50 + 35 * HitCount
					&& ch->in_room == victim->in_room
					&& !victim->deleted )
				{
					rand_vch = victim;
				}
			}

			EnemyCount++;
		}

		if( !rand_vch )
			continue;

		// �C�U�ˮ`���C
		nRandDam = old_nDamage * (18 + nSklv) /(25 + HitCount*2);
		nRandDam = GetBreathDamage(nRandDam, ch, rand_vch, nSklv, MAGIC_LIGHTNING, sn);
		nRandDam = number_range(nRandDam * 5, nRandDam * 5.5);
		nRandDam = nRandDam * ( 85 + nMainState ) / 100;
		nRandDam = nRandDam * weakness_damage( rand_vch, sn ) / 100;

		if( !IS_NPC(ch) )
			ch->mana -= UMAX( 1 , (10 * mana_cost)/(10 + HitCount) );
		magic_damage( ch, rand_vch, nRandDam, sn, nSklv );

		if( times > 0 ){
			if( is_qmark( rand_vch, "lightning_breath_damcount" ) != NULL ){
				if( IS_NPC(rand_vch) )
					do_mpmod_qmark( rand_vch, "self lightning_breath_damcount v0 + 1" );
				else
					do_mod_qmark( rand_vch, "self lightning_breath_damcount v0 + 1" );
			}
			else
				do_add_qmark( rand_vch, "self lightning_breath_damcount 2 1 0 0 1 0" );
		}

		HitCount++;
	}
	return;
}

/*  �ª��s�� , 2022/04/17

int GetBreathDamage(int nBaseDmg, CHAR_DATA *ch, CHAR_DATA *victim, int nSklv, int nMainSVS, int nSecSVS)
{
#if BREATH_DEBUG
	char debugstr[200];
#endif
	int nDamage  = 0;
	int nAttSVS  = NEW_GET_SVS(ch, nMainSVS) - (UMIN(NEW_GET_SVS(ch, nSecSVS), nSklv * 5 + 20)) + 40;
	int nDefSVS  = NEW_GET_SVS(victim, MAGIC_BREATH) + NEW_GET_SVS(victim, nMainSVS)/4 + 40;
	int nAttBase = 40;
	int nDefBase = 50;
 
	if(IS_NPC(ch) && nSecSVS < 0) nAttSVS = NEW_GET_SVS(ch, nMainSVS) + 40;
	
#if BREATH_DEBUG
	sprintf(debugstr, "race = %d, dragon = %d", ch->race, race_lookup("Dragon"));
	send_to_char(debugstr, ch);
	send_to_char(debugstr, victim);
#endif
	if(ch->race == race_lookup("Dragon"))
		nSklv += 2;
	else if(ch->race == race_lookup("Drake"))
		nSklv += 1;
	else if(ch->race == race_lookup("Wyrm"))
		nSklv += 2;
	else if(ch->race == race_lookup("Wyvern"))
		nSklv += 1;
	else if(ch->race == race_lookup("Hydra"))
		nSklv += 2;
	else if(IS_NPC(ch))
		nAttSVS += 20;

	if(nSklv > 6) nSklv = 6;

	if(!IS_NPC(victim)) {
		nDefSVS   = NEW_GET_SVS(victim, MAGIC_BREATH) + NEW_GET_SVS(victim, nMainSVS)/3 + 33;
		nAttBase += 10;
		nDefBase -= 25;
	}
	nAttBase -= nSklv * 2;
	if(nSklv > 4) nAttBase --;
	if(nSklv > 5) nAttBase -= 2;

	// Sklv 6, �ܮt -110, nAttSVS = -100, �� NPC (25 + 100) / 25  ->  5 ��
	// Sklv 6, �q�ܮt -10 �_, �C -25 �I�ܮt, �[�@���¤O
	// Sklv N, �q�ܮt -40 + N * 5 �_, �C�t (-40, -38, -36, -34, -32, -29, -25)�I�ܮt, �[�@���¤O
	// �ܮt -110, Sklv 0 ~ 6, �¤O���Ƭ� (70/40, 75/38, 80/36, 85/34, 90/32, 95/29, 100/25) + 1, 
	// �Y 2.75, 2.97, 3.22, 3.5, 3.81, 4,27, 5
#if BREATH_DEBUG
	sprintf(debugstr, "OrgDmg = %d\n\r", nBaseDmg);
	send_to_char(debugstr, ch);
#endif
	if(nAttSVS < 0){
		nDamage = nBaseDmg * (nAttBase - nAttSVS) / nAttBase;
	}else{
		nDamage = nBaseDmg * nAttBase / (nAttBase + nAttSVS);
	}
#if BREATH_DEBUG
	sprintf(debugstr, "AttDmg = %d\n\r", nDamage);
	send_to_char(debugstr, ch);
#endif
	// FOR NPC, �ܮ��ΧܥD�ݩ�/2�ۥ[ < -30 ��, �C 40 �I�ܩʫ¤O�֤@�b
	// FOR PPL, �ܮ��ΧܥD�ݩ�/2�ۥ[ < -15 ��, �C 25 �I�ܩʫ¤O�֤@�b
	// �]�N�O�ѤH�� day of protection �缾�a�j���i�H��� 30% ~ 40% �s���ˮ`
	if(nDefSVS < 0){
		nDamage = nDamage * nDefBase / (nDefBase - nDefSVS);
	}else{
		nDamage = nDamage * (nDefBase + nDefSVS) / nDefBase;
	}
#if BREATH_DEBUG	
	sprintf(debugstr, "DefDmg = %d\n\r", nDamage);
	send_to_char(debugstr, ch);
#endif

#if BREATH_DEBUG
	sprintf(debugstr, "FinDmg = %d\n\r", nDamage);
	send_to_char(debugstr, ch);
#endif
	return nDamage;
}

void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj_lose;
    OBJ_DATA  *obj_next;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af; // add at 2020.0702
	int SvDecay = 0; // add at 2020.0702

	int ABSklv  = get_skill_level(ch,sn);
	int nCurrHP = UMIN(100000, ch->hit / PPL_HP_MULTIPLYER + ABSklv * 400 + 100);
	// HP 0 ~ 10K (0, 31, 44, 54, 63, 70, 77, 83, 89, 94, 100)
	int nDamage = (int)sqrt((int)(nCurrHP));
	int nDmgRate= 100;
	int obj_lv = level;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		ABSklv = 0;
		if( obj_lv >= 2)  ABSklv = 1;
		if( obj_lv >= 10) ABSklv = 2;
		if( obj_lv >= 22) ABSklv = 3;
		if( obj_lv >= 40) ABSklv = 4;
		if( obj_lv >= 61) ABSklv = 5;
		if( obj_lv >= 90) ABSklv = 6;
	}

	if ( (number_percent() *100/ weakness_percent(victim, sn ) ) < 2 * ch->level && 
	      new_saves_spell( ch->pp, victim, MAGIC_BREATH ) > (number_percent() *100/ weakness_percent(victim, sn)) )
	{
		for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next ){
			int iWear;

			obj_next = obj_lose->next_content;
			if ( obj_lose->deleted ) continue;
			if( IS_SET(obj_lose->extra_flags, ITEM_BLESS)) continue;
			if ( number_bits( 2 ) != 0 ) continue;

			switch ( obj_lose->item_type ){
				case ITEM_ARMOR:
					if ( obj_lose->value[0] > 0 ){
						int ac;
						// Correction on math by IvoryTiger
						act( "$p �v���l�a�ܧΡI",
						victim, obj_lose, NULL, TO_CHAR );
						if ( ( iWear = obj_lose->wear_loc ) != WEAR_NONE ){
							ac = apply_ac( obj_lose, iWear );
							if( !IS_NPC(victim) && victim->pcdata->learned[gsn_armor_mastery] > 0 )
							ac += ac/3;
							victim->armor += ac;
						}
						obj_lose->value[0] -= 1;
						obj_lose->value[1] += 1;
						obj_lose->cost	= 0;
						if ( iWear != WEAR_NONE ){
							ac = apply_ac( obj_lose, iWear );
							if( !IS_NPC(victim) && victim->pcdata->learned[gsn_armor_mastery] > 0 )
							ac += ac/3;
							victim->armor -= ac;
						}
					}
				break;
				case ITEM_CONTAINER:
				case ITEM_FURNITURE:
					act( "$p �Q�G�k�Ӯ����F�I",
					victim, obj_lose, NULL, TO_CHAR );
					extract_obj( obj_lose );
				break;
			}
		}
	}

	if(ch->pcdata != NULL){
		if(ch->pcdata->condition[COND_DRUNK] > 2){
#if BREATH_DEBUG
		char debugstr[200];
		sprintf(debugstr, "DRUNK_COND = %d\n\r", ch->pcdata->condition[COND_DRUNK]);
		send_to_char(debugstr, ch);
#endif
			send_to_char("�A���W���s��j�ƤF�A���j�ĦR���I\n\r", ch);
			
			if(ch->pcdata->condition[COND_DRUNK] > 10){
				nDmgRate += (ch->pcdata->condition[COND_DRUNK] - 10) * 2 + 5;
				ch->pcdata->condition[COND_DRUNK] = (ch->pcdata->condition[COND_DRUNK] + 10) / 2;
			}else{
				nDmgRate += (ch->pcdata->condition[COND_DRUNK] + 1)/ 2;
				ch->pcdata->condition[COND_DRUNK]--;
			}
		}
	}

	nDamage  = GetBreathDamage(nDamage, ch, victim, ABSklv, MAGIC_WIND, MAGIC_EARTH);
	nDamage  = number_range(nDamage * 4.25, nDamage * 5.5);
	nDamage  = nDamage * (75 + get_curr_int(ch)) / 100;
	nDamage  = nDamage * nDmgRate / 100;
	nDamage += affected_value(victim, sn, APPLY_NONE);  // add at 2020.0702
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, WEAR_NONE );
	if ( nDamage > 0 ){   // add at 2020.0702
		SvDecay = 12 + get_skill_level(ch,sn) * get_skill_level(ch,sn) * 3;
		if (is_affected( victim, skill_lookup("prot. from wind"))) SvDecay = SvDecay/2;
		if( affected_value(victim,sn, APPLY_NONE) < SvDecay ){
			af.type  = sn;
			af.duration  = 1;
			af.location  = APPLY_NONE;
			af.modifier  = 10;
			af.bitvector = 0;
			affect_join( victim, &af );
			act( "$N"HIG"�Q�ĩʪ��R���Q���A�ĲG���ۨ�L���W�C"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIG "�ĩʪ��R���§A�Q�ӡA�A���W�g����G�k���ĲG�I"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIG"�Q�ĩʪ��R���Q���A�ĲG���ۨ�L���W�C"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	if(nDmgRate > 100 && ch->pcdata != NULL){
		if(ch->pcdata->condition[COND_DRUNK] <= 1)
			send_to_char("�A���W���s���H�۱j�ĦR���������h ... \n\r", ch);
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}

void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA  *obj_lose;
	OBJ_DATA  *obj_next;
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af; // add at 2020.0702
	int SvDecay = 0; // add at 2020.0702

	int        nFBSklv = get_skill_level(ch,sn);
	int        nCurrHP = UMIN(100000, ch->hit / PPL_HP_MULTIPLYER + nFBSklv * 400 + 100);
	           // HP 0 ~ 10K (0, 31, 44, 54, 63, 70, 77, 83, 89, 94, 100)
	int        nDamage = (int)sqrt((int)(nCurrHP));
	int        nDmgRate= 100;
	int        nGsnFlameArmor = skill_lookup("flaming armor");
	int        obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nFBSklv = 0;
		if( obj_lv >= 2)  nFBSklv = 1;
		if( obj_lv >= 10) nFBSklv = 2;
		if( obj_lv >= 22) nFBSklv = 3;
		if( obj_lv >= 40) nFBSklv = 4;
		if( obj_lv >= 61) nFBSklv = 5;
		if( obj_lv >= 90) nFBSklv = 6;
	}

	if ( (number_percent() *100/ weakness_percent(victim, sn ) ) < 2 * ch->level && 
	      get_skill_level(ch,sn)*get_skill_level(ch,sn) > (number_percent()*100/weakness_percent(victim,sn)))
	{
		for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next ){
			char *msg;

			obj_next = obj_lose->next_content;
			if ( obj_lose->deleted ) continue;
			if ( number_bits( 2 ) != 0 ) continue;

			if( IS_SET(obj_lose->extra_flags, ITEM_BLESS)) continue;

			switch ( obj_lose->item_type ){
				default:		 continue;
				case ITEM_CONTAINER: msg = "$p ��M�_���U�N�I";	  break;
				case ITEM_POTION:	 msg = "$p �m�˦ӻ]�o�F�I";	  break;
				case ITEM_SCROLL:	 msg = "$p �q��t�}�l���N�I";  break;
				case ITEM_STAFF:	 msg = "$p �_�X�}�}�·ϡI";	  break;
				case ITEM_WAND:	 msg = "$p �q�X�I�I���P�I"; break;
				case ITEM_FOOD:	 msg = "$p �o�X�J����I"; break;
				case ITEM_PILL:	 msg = "$p �@�����ĤƤF�I";	  break;
				case ITEM_FURNITURE: msg = "$p �N���@��J���I";  break;
			}

			act( msg, victim, obj_lose, NULL, TO_CHAR );
			if( obj_lose->item_type == ITEM_CONTAINER ){
				check_fire_container( obj_lose, victim );
			}
			extract_obj( obj_lose );
		}
	}

	if(IS_AFFECTED(ch, AFF_FLAMING)){
		send_to_char( ""HIP"�A���W���]�V�j�ƤF�A���`���R���I"NOR"\n\r", ch );
		nDmgRate = 115;
		if(is_affected(ch, nGsnFlameArmor)){
			nDmgRate     += 15;
		}
	}

	nDamage  = GetBreathDamage(nDamage, ch, victim, nFBSklv, MAGIC_FIRE, MAGIC_COLD);
	nDamage  = number_range(nDamage * 4.75, nDamage * 5.75);
	nDamage  = nDamage * (75 + get_curr_int(ch)) / 100;
	nDamage  = nDamage * nDmgRate / 100;
	nDamage += affected_value(victim, sn, APPLY_NONE);  // add at 2020.0702
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, WEAR_NONE );
	if ( nDamage > 0 ){   // add at 2020.0702
		SvDecay = 12 + get_skill_level(ch,sn) * get_skill_level(ch,sn) * 3;
		if (is_affected( victim, skill_lookup("prot. from fire"))) SvDecay = SvDecay/2;
		if( affected_value(victim,sn, APPLY_NONE) < SvDecay ){
			af.type  = sn;
			af.duration  = 1;
			af.location  = APPLY_NONE;
			af.modifier  = 10;
			af.bitvector = 0;
			affect_join( victim, &af );
			act( "$N"HIR"�Q�`�����R���Q���A����_�X�}�}���ϡC"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIR "�`�����R���§A�Q�ӡA�A����_�X�}�}���ϡI"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIR"�Q�`�����R���Q���A����_�X�}�}���ϡC"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}

void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA  *obj_lose;
	OBJ_DATA  *obj_next;
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af; // add at 2020.0702
	int SvDecay = 0; // add at 2020.0702

	int        nGsnChillTouch = skill_lookup("chill touch");
    int        nChillRate = affected_value(ch, nGsnChillTouch, APPLY_ADAMAGE);
	int        nFBSklv = get_skill_level(ch,sn);
	int        nCurrHP = UMIN(100000, ch->hit / PPL_HP_MULTIPLYER + nFBSklv * 400 + 100);
	           // HP 0 ~ 10K (0, 31, 44, 54, 63, 70, 77, 83, 89, 94, 100)
	int        nDamage = (int)sqrt((int)(nCurrHP));
	int        nDmgRate= 100 - nChillRate/2;
	int        obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nFBSklv = 0;
		if( obj_lv >= 2)  nFBSklv = 1;
		if( obj_lv >= 10) nFBSklv = 2;
		if( obj_lv >= 22) nFBSklv = 3;
		if( obj_lv >= 40) nFBSklv = 4;
		if( obj_lv >= 61) nFBSklv = 5;
		if( obj_lv >= 90) nFBSklv = 6;
	}

	if ( (number_percent() *100/ weakness_percent(victim, sn ) ) < 2 * level && 
	      get_skill_level(ch,sn)*get_skill_level(ch,sn) > (number_percent()*100/weakness_percent(victim,sn)))
	{
		for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next ){
			char *msg;

			obj_next = obj_lose->next_content;
			if ( obj_lose->deleted ) continue;
			if ( number_bits( 2 ) != 0 ) continue;
			if ( IS_SET(obj_lose->extra_flags, ITEM_BLESS) ) continue;

			switch ( obj_lose->item_type ){
				default:		continue;
				case ITEM_CONTAINER:
				case ITEM_DRINK_CON:
				case ITEM_POTION:	msg = "$p ��M����A�M���ܦ��H���I"; break;
			}

			act( msg, victim, obj_lose, NULL, TO_CHAR );
			extract_obj( obj_lose );
		}
	}

	//if (nChillRate < -400) nChillRate = -400;
	if (nChillRate < -200) nChillRate = -200;
	if(nDmgRate > 100){
		send_to_char(HIC"�A�B�δH�����ˮ`�A�j�ƤF�B��R�����¤O�I"NOR"\n\r", ch);
		//nDmgRate += 10;
	}
	
	nDamage  = GetBreathDamage(nDamage, ch, victim, nFBSklv, MAGIC_COLD, MAGIC_FIRE);
	nDamage  = number_range(nDamage * 4.75, nDamage * 5.75);
	nDamage  = nDamage * (75 + get_curr_int(ch)) / 100;
	nDamage  = nDamage * nDmgRate / 100;
	nDamage += affected_value(victim, sn, APPLY_NONE);  // add at 2020.0702
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, WEAR_NONE );
	if ( nDamage > 0 ){   // add at 2020.0702
		SvDecay = 12 + get_skill_level(ch,sn) * get_skill_level(ch,sn) * 3;
		if (is_affected( victim, skill_lookup("prot. from cold"))) SvDecay = SvDecay/2;
		if( affected_value(victim,sn, APPLY_NONE) < SvDecay ){
			af.type  = sn;
			af.duration  = 1;
			af.location  = APPLY_NONE;
			af.modifier  = 10;
			af.bitvector = 0;
			affect_join( victim, &af );
			act( "$N"HIC"�Q�B�N���R���Q���A�ֽ��ᵲ�X�h�h�B���C"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIC "�B�N���R���§A�Q�ӡA�A���ֽ����X�h�h�B���I"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIC"�Q�B�N���R���Q���A�ֽ��ᵲ�X�h�h�B���C"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}

//////////////////////////////////////////////////////////////////
//	gas breath modified by keelar 04.29 2005
//  �r���Ҥ޵o���I�r�N�N�P�r���� sklv �����B�Ӥ��O poison
void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	int        nGBSklv = get_skill_level(ch,sn);
	int        nCurrHP = UMIN(100000, ch->hit / PPL_HP_MULTIPLYER + nGBSklv * 400 + 100);
	           // HP 0 ~ 10K (0, 31, 44, 54, 63, 70, 77, 83, 89, 94, 100)
	int        nDamage = (int)sqrt((int)(nCurrHP));
	int        nDmgRate= 100;
	int        nPoison = affected_value(ch, gsn_poison, APPLY_SAVING_POISON);
	int        obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nGBSklv = 0;
		if( obj_lv >= 2)  nGBSklv = 1;
		if( obj_lv >= 10) nGBSklv = 2;
		if( obj_lv >= 22) nGBSklv = 3;
		if( obj_lv >= 40) nGBSklv = 4;
		if( obj_lv >= 61) nGBSklv = 5;
		if( obj_lv >= 90) nGBSklv = 6;
	}

#if BREATH_DEBUG
	char debugstr[200];
	sprintf(debugstr, "nPoison = %d\n\r", nPoison);
	send_to_char(debugstr, ch);
#endif
	if(nPoison > 0){
		send_to_char(HIG"�餺���r���H�۬r��R�����o�X�ӡA�o�X��j���¤O�I"NOR"\n\r", ch);
		nDmgRate += nPoison / 8;
		if(nGBSklv > 5) nDmgRate += (nGBSklv - 4) * 5;
	}
#if BREATH_DEBUG
	sprintf(debugstr, "nDmgRate = %d\n\r", nDmgRate);
	send_to_char(debugstr, ch);
#endif
	if(nDmgRate > 200) nDmgRate = 200;


	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{ 
		send_to_char("�o�̸T��԰�. \n\r",ch);
		return;
	}

	for ( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted ) continue;
		if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
		if( is_same_group(ch, vch) ) continue;
		
		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, vch, TO_CHAR);
			continue;
		}

		if ( IS_NPC( ch ) ? !IS_NPC( vch ) : IS_NPC( vch ) ){
			nDamage  = (int)sqrt((int)(nCurrHP));
			nDamage  = GetBreathDamage(nDamage, ch, vch, nGBSklv, MAGIC_EARTH, MAGIC_WIND);
			nDamage  = number_range(nDamage * 4.00, nDamage * 5.00);
			nDamage  = nDamage * (75 + get_curr_int(ch)) / 100;
			nDamage  = nDamage * nDmgRate / 100;
			nDamage  = nDamage * weakness_damage( vch, sn ) / 100;
			if(magic_damage( ch, vch, nDamage, sn, WEAR_NONE ) == -2)
				continue;

			spell_poison( sn, ch->level, ch, vch );
		}
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}

void spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim  = (CHAR_DATA *) vo;
	AFFECT_DATA af; // add at 2020.0702
	int SvDecay = 0; // add at 2020.0702
	int        nLBSklv = get_skill_level(ch,sn);
	int        nCurrHP = UMIN(100000, ch->hit / PPL_HP_MULTIPLYER + nLBSklv * 400 + 100);
	           // HP 0 ~ 10K (0, 31, 44, 54, 63, 70, 77, 83, 89, 94, 100)
	int        nDamage = (int)sqrt((int)(nCurrHP));
	int        nDmgRate= 100;
	int        obj_lv = level;
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nLBSklv = 0;
		if( obj_lv >= 2)  nLBSklv = 1;
		if( obj_lv >= 10) nLBSklv = 2;
		if( obj_lv >= 22) nLBSklv = 3;
		if( obj_lv >= 40) nLBSklv = 4;
		if( obj_lv >= 61) nLBSklv = 5;
		if( obj_lv >= 90) nLBSklv = 6;
	}


	if(weather_info.sky == SKY_CLOUDLESS){
		nDmgRate = 100;
	}else if(weather_info.sky == SKY_CLOUDY){
		nDmgRate = 110;
	}else if(weather_info.sky == SKY_RAINING){
		nDmgRate = 120;
	}else if(weather_info.sky == SKY_LIGHTNING){
		nDmgRate = 130;
	}else{ // SKY_SNOWING ,SKY_SNOWSTORM add at 2018.11.06
		nDmgRate = 110;
	}
	
	nDamage  = GetBreathDamage(nDamage, ch, victim, nLBSklv, MAGIC_LIGHTNING, MAGIC_SPELL);
	nDamage  = number_range(nDamage * 4.2, nDamage * 7);
	nDamage  = nDamage * (75 + get_curr_int(ch)) / 100;
	nDamage  = nDamage * nDmgRate / 100;
	nDamage += affected_value(victim, sn, APPLY_NONE);  // add at 2020.0702
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, nDamage, sn, WEAR_NONE );
	if ( nDamage > 0 ){   // add at 2020.0702
		SvDecay = 22 + get_skill_level(ch,sn) * get_skill_level(ch,sn) * 3;
		if (is_affected( victim, skill_lookup("prot. from lightning"))) SvDecay = SvDecay/2;
		if( affected_value(victim,sn, APPLY_NONE) < SvDecay ){
			af.type  = sn;
			af.duration  = 1;
			af.location  = APPLY_NONE;
			af.modifier  = 10;
			af.bitvector = 0;
			affect_join( victim, &af );
			act( "$N"HIY"�Q�p�q�R���Q���A����y��۰����q�y�C"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIY "�p�q�R���§A�Q�ӡA�A����y��۰����q�y�I"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIY"�Q�p�q�R���Q���A����y��۰����q�y�C"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}*/