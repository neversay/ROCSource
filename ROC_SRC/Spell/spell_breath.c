//*****************************************************************
//*  PROGRAM ID  : spell_breath                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0960212 Razgriz                                *
//*  PURPOSE     :         將原先 slash_breath 拆成 S 和 B        *
//*****************************************************************/
#define BREATH_DEBUG   0

#include "skill.h"
#include <math.h>

//************************************************ /
//  所有吐息於 2022/04/17 修改為隨機目標多次傷害   /
//  傷害取消負屬性影響, 僅看正屬性並加入傷害累計   /
//  連續受到攻擊的目標會受到更高的傷害             /
// ----------------------------------------------- /
//  舊版吐息置於文件下方                           /
//************************************************ /
int GetBreathDamage(int nBaseDmg, CHAR_DATA *ch, CHAR_DATA *victim, int nSklv, int nMainSVS, int sn)
{
	int nDamage  = 0;
	int nAttSVS  = NEW_GET_SVS(ch, nMainSVS);
	int nDefSVS  = NEW_GET_SVS(victim, nMainSVS)/2 + 10;
	int nAttBase = 40;
	int nDefBase = 50;

	// 龍族 race 加成
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

	// 基本 damage 會受 hp 百分比下降到最多 1/3 2022/04/16
	if( ch->max_hit > ch->hit )
		nBaseDmg = UMAX( nBaseDmg/3 , (nBaseDmg * ch->hit) / ch->max_hit );
	
	// mob 死前一擊
	if( IS_NPC(ch) && ch->position == POS_DEAD )
		nBaseDmg = nBaseDmg * 3;

	// 被連續吐息增傷 , sklv 6 打出 3000 傷害每下累計值約為 165 上下 累計的部分寫在 fight_damage 2022/04/09
	if( affected_value(victim, sn, APPLY_DAMAGE_COUNT) > 0 ){
		if( IS_NPC(ch) )
			nBaseDmg = nBaseDmg + affected_value(victim, sn, APPLY_DAMAGE_COUNT)/14;
		else
			nBaseDmg = nBaseDmg + affected_value(victim, sn, APPLY_DAMAGE_COUNT)/11;
	}
	
	// 毒息判定與一般不同 2022/04/09
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
			send_to_char( "你的法力不夠了.\n\r", ch);
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
						act( "$p 被強酸濺到，逐漸損壞變形！", victim, obj_lose, NULL, TO_CHAR );
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
					act( "$p 被強酸濺到，腐蝕掉了！",
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

	// 酸系 spell 應為地系技能 2022/04/06
	nDamage  = GetBreathDamage(nDamage, ch, victim, nSklv, MAGIC_EARTH, sn);
	nDamage  = number_range(nDamage * 5, nDamage * 5.5);
	nDamage  = nDamage * ( 85 + nMainState ) / 100;
	nDamage  = nDamage * weakness_damage( victim, sn ) / 100;
	// 以 magic_damage 項的 wpn 代入裝備 sklv 以計算傷害後的加成效果 2022/04/16
	magic_damage( ch, victim, nDamage, sn, nSklv );


	for( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted )
			continue;

		if( is_same_group(ch, vch) ){
			// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) )
			{
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
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

			// 每多一下降 mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// 針對 wizinv 的情況 2022/11/20
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

			// 前兩次有較高機會噴中指定目標
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

		// 每下傷害降低
		nRandDam = old_nDamage * (18 + nSklv) /(25 + HitCount*2);
		nRandDam = GetBreathDamage(nRandDam, ch, rand_vch, nSklv, MAGIC_EARTH, sn);
		nRandDam = number_range(nRandDam * 5, nRandDam * 5.5);
		nRandDam = nRandDam * ( 85 + nMainState ) / 100;
		nRandDam = nRandDam * weakness_damage( rand_vch, sn ) / 100;

		if( !IS_NPC(ch) )
			ch->mana -= UMAX( 1 , (10 * mana_cost)/(10 + HitCount) );
		// 以 magic_damage 項的 wpn 代入裝備 sklv 以計算傷害後的加成效果 2022/04/16
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
			send_to_char( "你的法力不夠了.\n\r", ch);
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
				case ITEM_CONTAINER: msg = "$p 遭龍燄波及，突然起火燃燒！";	  break;
				case ITEM_POTION:	 msg = "$p 遭龍燄波及，沸騰而蒸發了！";	  break;
				case ITEM_SCROLL:	 msg = "$p 遭龍燄波及，從邊緣開始延燒！";  break;
				case ITEM_STAFF:	 msg = "$p 遭龍燄波及，冒出陣陣黑煙！";	  break;
				case ITEM_WAND:      msg = "$p 遭龍燄波及，濺出點點火星！"; break;
				case ITEM_FOOD:      msg = "$p 遭龍燄波及，發出焦臭味！"; break;
				case ITEM_PILL:      msg = "$p 遭龍燄波及，一顆顆融化了！";	  break;
				case ITEM_FURNITURE: msg = "$p 遭龍燄波及，燒成一堆焦炭！";  break;
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
			// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
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

			// 每多一下降 mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// 針對 wizinv 的情況 2022/11/20
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

			// 前兩次有較高機會噴中指定目標
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

		// 每下傷害降低
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
			send_to_char( "你的法力不夠了.\n\r", ch);
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
				case ITEM_POTION:	msg = "$p 被四周的寒氣凍結，然後化為碎片！"; break;
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
			// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
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

			// 每多一下降 mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// 針對 wizinv 的情況 2022/11/20
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

			// 前兩次有較高機會噴中指定目標
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

		// 每下傷害降低
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
			send_to_char( "你的法力不夠了.\n\r", ch);
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
		send_to_char("這裡禁止戰鬥. \n\r",ch);
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
			// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
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

			// 每多一下降 mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// 針對 wizinv 的情況 2022/11/20
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

			// 前兩次有較高機會噴中指定目標
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

		// 每下傷害降低
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
			send_to_char( "你的法力不夠了.\n\r", ch);
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
			// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
			if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
				Pc_Group = TRUE;
			}
			continue;
		}

		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
			// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
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

			// 每多一下降 mana cost
			if( !IS_NPC(ch) && ch->mana < UMAX( 1 , (10 * mana_cost)/(10 + HitCount) ) )
				return;

			if( vch->deleted )
				continue;

			if( is_same_group(ch, vch) ){
				// 如果是 pet 的 npc (sor mob , pet ), 則會判定隊伍中是否存在 pc 2022/12/28
				if( IS_NPC(ch) && IS_SET( ch->act, ACT_PET ) && !IS_NPC(vch) ){
					Pc_Group = TRUE;
				}
				continue;
			}

			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
				// 隊伍中有 pc 且 vch 為 pc , 除非 vch 去打 ch 否則 則不會打到其他 pc 2022/12/28
				if( Pc_Group && !IS_NPC(vch) ){
					if( vch->fighting != ch )
						continue;
				}
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}

			// 針對 wizinv 的情況 2022/11/20
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

			// 前兩次有較高機會噴中指定目標
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

		// 每下傷害降低
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

/*  舊版龍息 , 2022/04/17

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

	// Sklv 6, 抗差 -110, nAttSVS = -100, 打 NPC (25 + 100) / 25  ->  5 倍
	// Sklv 6, 從抗差 -10 起, 每 -25 點抗差, 加一倍威力
	// Sklv N, 從抗差 -40 + N * 5 起, 每差 (-40, -38, -36, -34, -32, -29, -25)點抗差, 加一倍威力
	// 抗差 -110, Sklv 0 ~ 6, 威力倍數為 (70/40, 75/38, 80/36, 85/34, 90/32, 95/29, 100/25) + 1, 
	// 即 2.75, 2.97, 3.22, 3.5, 3.81, 4,27, 5
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
	// FOR NPC, 抗息及抗主屬性/2相加 < -30 後, 每 40 點抗性威力少一半
	// FOR PPL, 抗息及抗主屬性/2相加 < -15 後, 每 25 點抗性威力少一半
	// 也就是天人的 day of protection 對玩家大約可以減少 30% ~ 40% 龍息傷害
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
						act( "$p 逐漸損壞變形！",
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
					act( "$p 被腐蝕而消失了！",
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
			send_to_char("你身上的酒氣強化了你的強酸吐息！\n\r", ch);
			
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
			act( "$N"HIG"被酸性的吐息噴中，酸液附著到他身上。"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIG "酸性的吐息朝你噴來，你身上沾附到腐蝕的酸液！"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIG"被酸性的吐息噴中，酸液附著到他身上。"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	if(nDmgRate > 100 && ch->pcdata != NULL){
		if(ch->pcdata->condition[COND_DRUNK] <= 1)
			send_to_char("你身上的酒氣隨著強酸吐息漸漸散去 ... \n\r", ch);
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
				case ITEM_CONTAINER: msg = "$p 突然起火燃燒！";	  break;
				case ITEM_POTION:	 msg = "$p 沸騰而蒸發了！";	  break;
				case ITEM_SCROLL:	 msg = "$p 從邊緣開始延燒！";  break;
				case ITEM_STAFF:	 msg = "$p 冒出陣陣黑煙！";	  break;
				case ITEM_WAND:	 msg = "$p 濺出點點火星！"; break;
				case ITEM_FOOD:	 msg = "$p 發出焦臭味！"; break;
				case ITEM_PILL:	 msg = "$p 一顆顆融化了！";	  break;
				case ITEM_FURNITURE: msg = "$p 燒成一堆焦炭！";  break;
			}

			act( msg, victim, obj_lose, NULL, TO_CHAR );
			if( obj_lose->item_type == ITEM_CONTAINER ){
				check_fire_container( obj_lose, victim );
			}
			extract_obj( obj_lose );
		}
	}

	if(IS_AFFECTED(ch, AFF_FLAMING)){
		send_to_char( ""HIP"你身上的魔燄強化了你的灼熱吐息！"NOR"\n\r", ch );
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
			act( "$N"HIR"被灼熱的吐息噴中，身體冒出陣陣熱煙。"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIR "灼熱的吐息朝你噴來，你身體冒出陣陣熱煙！"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIR"被灼熱的吐息噴中，身體冒出陣陣熱煙。"NOR"", ch, NULL, victim, TO_NOTVICT );
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
				case ITEM_POTION:	msg = "$p 突然結凍，然後變成碎片！"; break;
			}

			act( msg, victim, obj_lose, NULL, TO_CHAR );
			extract_obj( obj_lose );
		}
	}

	//if (nChillRate < -400) nChillRate = -400;
	if (nChillRate < -200) nChillRate = -200;
	if(nDmgRate > 100){
		send_to_char(HIC"你運用寒霜的傷害，強化了冰凍吐息的威力！"NOR"\n\r", ch);
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
			act( "$N"HIC"被冰冷的吐息噴中，皮膚凍結出層層冰霜。"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIC "冰冷的吐息朝你噴來，你的皮膚結出層層冰霜！"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIC"被冰冷的吐息噴中，皮膚凍結出層層冰霜。"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}

//////////////////////////////////////////////////////////////////
//	gas breath modified by keelar 04.29 2005
//  毒息所引發的施毒術將與毒息的 sklv 有關、而不是 poison
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
		send_to_char(HIG"體內的毒素隨著毒氣吐息散發出來，發出更強的威力！"NOR"\n\r", ch);
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
		send_to_char("這裡禁止戰鬥. \n\r",ch);
		return;
	}

	for ( vch = ch->in_room->people; vch; vch = vch_next ){
		vch_next = vch->next_in_room;
		if( vch->deleted ) continue;
		if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
		if( is_same_group(ch, vch) ) continue;
		
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, vch, TO_CHAR);
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
			act( "$N"HIY"被雷電吐息噴中，全體流串著高壓電流。"NOR"", ch, NULL, victim, TO_CHAR );
			act( HIY "雷電吐息朝你噴來，你全體流串著高壓電流！"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIY"被雷電吐息噴中，全體流串著高壓電流。"NOR"", ch, NULL, victim, TO_NOTVICT );
		}
	}
	//WAIT_MAGIC_STATE(ch, 16);
	return;
}*/