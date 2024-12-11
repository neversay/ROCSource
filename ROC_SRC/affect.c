//*****************************************************************
//*  PROGRAM ID  : affect.c                                       *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         修改remove eq會將原有cast移除之bug     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940616 jye                                    *
//*  PURPOSE     :         修改remove eq會將                      *
//*              :             原有cast移除之bug不由此判斷        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940714 Razgriz                                *
//*  PURPOSE     :         修改 affect_remove()，加入對           *
//*              :         charm_person 的 qmark 處理             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950331 jye                                    *
//*              :         修改 affect_remove()，加入停止跟隨     *
//*****************************************************************
#include <stdio.h>
#include <stdlib.h>

#include "merc.h"
/*
 * Apply or remove an affect to a character.
 */
// modified by jye 6.15.2005
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
	char      buf [ MAX_STRING_LENGTH ];
	int	      mod;
		
	if( !ch || paf->deleted ) return;
	mod = paf->modifier;

	if( fAdd )
	{
		SET_BIT( ch->affected_by, paf->bitvector );
	}
	else
	{
      REMOVE_BIT( ch->affected_by, paf->bitvector );
      mod = 0 - mod;
	}

	switch ( paf->location )
	{
		default:
			sprintf( buf, "Affect_modify: unknown location %d on %s.",
					paf->location, ch->name );
			bug ( buf, 0 );
			return;

		case APPLY_NONE:
			break;
		case APPLY_STR:
			ch->mod_str += mod;
			break;
		case APPLY_DEX:
			ch->mod_dex += mod;
			break;
		case APPLY_INT:
			ch->mod_int += mod;
			break;
		case APPLY_WIS:
			ch->mod_wis += mod;
			break;
		case APPLY_CON:
			ch->mod_con += mod;
			break;
		case APPLY_SEX:
			ch->sex += mod;
			break;
		case APPLY_RACE:
		{
			//	Check for natural_fly and inborn ability skill -- added by keelar
			int  nNewRace = ch->race + mod;
			int  nRaceSklv = get_skill_level(ch, gsn_inborn_ability);
			bool bOrgFlyStat, bNewFlyStat;

			while(nNewRace >= MAX_RACE){
				nNewRace -= MAX_RACE;
			}
			while(nNewRace < 0){
				nNewRace += MAX_RACE;
			}

			bOrgFlyStat = IS_SET(race_table[ ch->race ].race_abilities, RACE_FLY);
			bNewFlyStat = IS_SET(race_table[ nNewRace ].race_abilities, RACE_FLY);
			if(bOrgFlyStat){
				ch->dodge += nRaceSklv * 2 + (nRaceSklv * nRaceSklv) / 2;
				ch->dodge -= race_table[ch->race].size * 5;
			}
			if(bNewFlyStat){
				ch->dodge -= nRaceSklv * 2 + (nRaceSklv * nRaceSklv) / 2;
				ch->dodge += race_table[nNewRace].size * 5;
			}
			ch->race = nNewRace;
			break;
		}
		case APPLY_CLASS:
			break;
		case APPLY_LEVEL:
			break;
		case APPLY_AGE:
			break;
		case APPLY_HEIGHT:
			ch->mod_size += mod; // 體型 2022/12/23
			break;
		case APPLY_WEIGHT:
			ch->carry_weight += mod;
			break;
		case APPLY_MANA:
			ch->max_mana += mod;
			break;
		case APPLY_HIT:
			ch->max_hit += mod;
			break;
		case APPLY_MOVE:
			ch->max_move += mod;
			break;
		case APPLY_GOLD:
			break;
		case APPLY_EXP:
			break;
		case APPLY_AC:
			ch->armor += mod;
			break;
		case APPLY_HITROLL:
			ch->hitroll	+= mod;
			break;
		case APPLY_DAMROLL:
			ch->damroll	+= mod;
			break;
		case APPLY_SAVING_PARA:
			ch->saving_throw[0]	+= mod;
			break;
		case APPLY_SAVING_LIFE:   // 用於 mar 不死判定 2020/10/28
			//ch->saving_throw[MAGIC_BREATH] += mod; 
			break;
		case APPLY_SAVING_SPELL:
		//case APPLY_SAVING_SPIRIT:
			ch->saving_throw[0]	+= mod;
			break;
		case APPLY_SAVING_SPIRIT:
			break;
		case APPLY_SAVING_PETRI:
			ch->saving_throw[MAGIC_EARTH] += mod;
			break;
		case APPLY_SAVING_EARTH:
			ch->saving_throw[MAGIC_EARTH] += mod;
			break;
		case APPLY_SAVING_GOOD:
		case APPLY_SAVING_SAINT:
			ch->saving_throw[MAGIC_SAINT] += mod;
			break;
		case APPLY_SAVING_EVIL:
		case APPLY_SAVING_DARK:
			ch->saving_throw[MAGIC_DARK]  += mod;
			break;
		case APPLY_SAVING_LIGHTNING:
			ch->saving_throw[MAGIC_LIGHTNING] += mod;
			break;
		case APPLY_SAVING_WIND:
			ch->saving_throw[MAGIC_WIND] += mod;
			break;
		case APPLY_SAVING_ANCIENT: // 將 sv ancient 與 sv poison 分開 2021/11/03
			break;
		case APPLY_SAVING_POISON:
			ch->saving_throw[MAGIC_POISON] += mod;
			break;
		case APPLY_SAVING_ELEMENT:
			ch->saving_throw[MAGIC_FIRE]		+= mod; 
			ch->saving_throw[MAGIC_WIND]		+= mod; 
			ch->saving_throw[MAGIC_COLD]		+= mod; 
			ch->saving_throw[MAGIC_EARTH]		+= mod; 
			ch->saving_throw[MAGIC_LIGHTNING]	+= mod;
			break;
		case APPLY_SAVING_BREATH:
			ch->saving_throw[MAGIC_BREATH] += mod;
			break;
		case APPLY_SAVING_FIRE:
			ch->saving_throw[MAGIC_FIRE] += mod;
			break;
		case APPLY_SAVING_COLD:
			ch->saving_throw[MAGIC_COLD] += mod;
			break;
		case APPLY_RDAMAGE:
			ch->pdr += mod;
			break;
		case APPLY_RMAGIC:
			ch->mdr += mod;
			break;
		case APPLY_ADAMAGE:
			ch->apdr += mod;
			break;
		case APPLY_AMAGIC:
			ch->amdr += mod;
			break;
		case APPLY_GPHYSICAL:
			if( IS_NPC(ch) ) ch->pad += mod;
			break;
		case APPLY_GMAGIC:
			if( IS_NPC(ch) ) ch->mad += mod;
			break;
		case APPLY_POWER_POTENTIAL:
			ch->pp += mod; 
			break;
		case APPLY_DODGE:
			ch->dodge += mod;
			break;
		case APPLY_DOB_SAVING_FLASH:
			break;
		case APPLY_SAVING_ILLUSION:
			break;
		case APPLY_ACTION_ABILITY:
			break;
		case APPLY_WEAPON_SPELL:
			break;
		case APPLY_WEAPON_WIND:
			break;
		case APPLY_WEAPON_FIRE:
			break;
		case APPLY_WEAPON_COLD:
			break;
		case APPLY_WEAPON_EARTH:
			break;
		case APPLY_WEAPON_LIGHTNING:
			break;
		case APPLY_WEAPON_SAINT:
			break;
		case APPLY_WEAPON_DARK:
			break;
		case APPLY_MAGIC_IMMUNE:     // 用於 mystic armor 判定 2021/11/03
			break;
		case APPLY_TOXICITY_POISON:  // 用於 poison item 計算 2021/11/10
			break;
		case APPLY_DAMAGE_COUNT:     // 統計傷害值, blood thirsty 用 2022/02/21
			break;
		case APPLY_SHOOT_HITROLL:    // 飛行武器使用時的 hitroll 增加量 2022/05/06
			break;
		case APPLY_SHOOT_DAMROLL:    // 飛行武器使用時的 damroll 增加量 2022/05/06
			break;
		case APPLY_SHOOT_ADAMAGE:    // 飛行武器使用時的 apdr 增加量 2022/05/06
			break;
	}

	/* Remove the excess general stats */
	ch->hit  = UMIN( ch->hit, ch->max_hit );
	ch->mana = UMIN( ch->mana, ch->max_mana );
	ch->move = UMIN( ch->move, ch->max_move );

	if ( IS_NPC( ch ) )
		return;

}



/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    if( !ch ) return;
    paf_new		= new_affect();

    *paf_new		= *paf;
    paf_new->deleted	= FALSE;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE );
    return;
}

/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
	//AFFECT_DATA *vpaf;
	QMARK *qmark = NULL;                //Added by Razgriz    20050714
	ROOM_INDEX_DATA *pRoomIndex = NULL; //Added by Razgriz    20050714

	if( !ch ) return;
	if ( !ch->affected )
	{
		bug( "Affect_remove: no affect.", 0 );
		return;
	}

	if (paf->type == gsn_sleep)
		ch->position = POS_STANDING;

	affect_modify( ch, paf, FALSE );

	paf->deleted = TRUE;

	/*
	 * Added by Razgriz    20050714
	 * for charm_person qmark handling
	 */
	if (   ( IS_NPC ( ch )                                          )
		&& ( ch->in_room                                            )
		&& ( paf->type == gsn_charm_person                          )
		&& ( ( qmark = is_qmark ( ch, "roc_mob_charmed" ) ) != NULL )
		&& ( ( pRoomIndex = get_room_index ( qmark->v0 )  ) != NULL )    )
	{
		if ( ch->in_room->area != pRoomIndex->area )
		{
			act ( "$n 消失在時空的狹縫中。", ch, NULL, NULL, TO_ROOM );
			char_from_room ( ch );
			char_to_room   ( ch, pRoomIndex );
			act ( "$n 從時空的狹縫中冒了出來。", ch, NULL, NULL, TO_ROOM );
			send_to_char ( "你做回真正的自己，回到了原本你該在的地方。\n\r", ch );
		}
		do_mpdel_qmark ( ch, "self roc_mob_charmed" );
		//add by jye 20060331
		//停止跟隨
		stop_follower(ch);
	}

	/*    if (paf->type == gsn_adore && paf->next )
		{
			vpaf = paf->next;
			if (vpaf->type == gsn_adore )
			{
				affect_modify(ch,vpaf,FALSE);
			vpaf->deleted = TRUE;
			}
		}
	*/
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
	AFFECT_DATA *paf;

	if( !ch ) return;
	for ( paf = ch->affected; paf; paf = paf->next ){
		if ( paf->deleted ) 
			continue;

		if ( paf->type == sn )
			affect_remove( ch, paf );
	}
	return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
	AFFECT_DATA *paf;

	for ( paf = ch->affected; paf; paf = paf->next ){
		if ( paf->deleted ) 
			continue;

		if ( paf->type == sn )
			return TRUE;
	}

	return FALSE;
}

/*  
 * Return 0 if the char or mob does not affect by this sn
 * or does not affect by this kind of apply type.
 * it return the modifier value when it matchs the input condition.
 */ 
int affected_value( CHAR_DATA *ch, int sn, int apply_type )
{   
    AFFECT_DATA *paf;
    
    for ( paf = ch->affected; paf; paf = paf->next )
    {
        if ( paf->deleted )
            continue;
        if ( paf->type == sn && paf->location == apply_type)
            return paf->modifier;
    }

    return 0;
}   



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
	AFFECT_DATA *paf_old;
	int nOrgDuration = -1;

	if( !ch ) return;
	for ( paf_old = ch->affected; paf_old; paf_old = paf_old->next ){
		if ( paf_old->deleted ) continue;
		if ( paf_old->type == paf->type && paf_old->location == paf->location ){
			if ( IS_NPC( ch ) && paf->location < 6 && paf_old->modifier < 0 
				&& number_percent() < ( - paf_old->modifier * 8 ) )
				return;
			nOrgDuration   = paf_old->duration;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;
			affect_remove( ch, paf_old );
			break;
		}
	}

	if(nOrgDuration < 0 || paf->duration >= 0)
		affect_to_char( ch, paf );
	return;
}

