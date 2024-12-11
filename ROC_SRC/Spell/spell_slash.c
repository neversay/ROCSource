//*****************************************************************
//*  PROGRAM ID  : spell_slash                                    *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0970212 Keelar                                 *
//*  PURPOSE     :         將 spell_slash_breath 拆成 S 和 B      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1060109 Jye                                    *
//*  PURPOSE     :         修改spell_earth_slash                  *
//*              :             判斷 sklv 4以才打不到flying目標    *
//*****************************************************************/
#include "skill.h"

extern int get_critical_damage(int dam, CHAR_DATA *ch, CHAR_DATA *victim, 
                               int dexdiff, bool fCritical, int dt, int darkraid);

// flaming slash modified by whisperwind 2.18.2005
// flam 第一段廣域火焰波部分
void spell_flaming_slash( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	double dam;
	int sklv = get_skill_level(ch, gsn_flaming_slash);
	int ex_pp = 0;
	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
		return;
	}
	sn = skill_lookup("flaming slash");
	ex_pp = ch->pp;

	if( get_skill_level(ch, skill_lookup("fireball")) > 3 && sklv > 3){
		ex_pp += UMAX( ch->pp - 20 , 0 );
	}
			
	if( get_skill_level(ch, skill_lookup("fireball")) > 5 && sklv > 3){
		ex_pp += UMAX( ch->pp - 30 , 0 );
	}
	if( get_skill_level(ch, skill_lookup("fireball")) > 5 && sklv > 3)
	{
		ex_pp += UMAX( ch->pp - 50 , 0 ) * 3;
	}
	    
	for( victim = ch->in_room->people; victim ; victim = victim->next_in_room ) {
		if( victim->deleted ) continue;
		if( victim->fighting != ch && ch->fighting != victim ) continue;
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS)) continue;

		dam = (40 + level)/2 ; // basic damage
		
		//int,dex effect
		dam += dam * (( get_curr_int(ch) - 10)*2 + ( get_curr_dex(ch) - 10 )*2) / 100;

		dam *= 1 + 0.1*sklv ; // sklv effect
			
		if( sklv > 3) dam = dam * 1.1;
		if( sklv > 4) dam = dam * 1.1;
	  if( sklv > 5) dam = dam * 1.1;
		
		dam = dam * weakness_damage( victim, sn ) / 100;
	  dam = dam * ( old_saves_spell( ex_pp, victim,MAGIC_FIRE)) / 100;
		if( !IS_NPC(ch) )
			dam = number_range( dam * 1.5 , dam * 2 );
		else if ( IS_NPC(ch) )
			dam = number_range( dam * 1.5 , dam * 2 );
	
		magic_damage( ch, victim, dam, sn, WEAR_NONE );
	}
}

// flam 第二段強化火焰坡部分
void spell_flaming_slash2( int sn, int level, CHAR_DATA *ch, void *vo)
{
 	CHAR_DATA *victim = (CHAR_DATA *) vo;
 	double dam = (40 + level)/2; // basic damage
 	int sklv = get_skill_level(ch, gsn_flaming_slash);
 	int ex_pp = ch->pp;

	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}

	sn = skill_lookup("flaming slash");

	// swordmaster bonus
	
	dam *= 1 + get_skill_level(ch, gsn_swordmaster) * 0.02 ;
	
	// int dex effect
	dam += dam * (( get_curr_int(ch) - 10)*2 + ( get_curr_dex(ch) - 10 )*2) / 100;
	
	// sklv effect
	dam *= 1 + 0.1*sklv;
		
	if( sklv > 3 )
		dam = dam * 1.1;
	if( sklv > 4 )
		dam = dam * 1.1;
	if( sklv > 5 )
		dam = dam * 1.1;
		
	// flam maximize effect, increase pp effect when pp is high
	// the part of pp > 20 , double effect
	// the part of pp > 30 , triple effect
	if( get_skill_level(ch, skill_lookup("fireball")) > 3 && sklv > 3)
	{
		ex_pp += UMAX( ch->pp - 20 , 0 );
	}
    		
	if( get_skill_level(ch, skill_lookup("fireball")) > 4 && sklv > 3)
	{
		ex_pp += UMAX( ch->pp - 35 , 0 );
	}
    		
	if( get_skill_level(ch, skill_lookup("fireball")) > 5 && sklv > 3)
	{
		ex_pp += UMAX( ch->pp - 50 , 0 ) * 3;
	}

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( old_saves_spell( ex_pp, victim,MAGIC_FIRE)) / 100;
        
	if( !IS_NPC(ch) )
		dam = number_range( dam * 6 , dam * 7 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 5 , dam * 6 );
	
	magic_damage( ch, victim, dam, sn, WEAR_NONE );
}

// storm slash modified by whisperwind 2.18.2005
void spell_storm_slash( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	CHAR_DATA *vch= (CHAR_DATA *) vo;

	double dam;
	int times = 4; 
	int sklv = get_skill_level(ch, gsn_storm_slash);
	int sklvCri = get_skill_level(ch, gsn_critical);

	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
		return;
	}

	sn = skill_lookup("storm slash");

	// 段數設定
	if( sklv > 1 ) times += 1;
	if( sklv > 3 ) times += 1;
	if( ch->level > 36 ) times += 1;
	if( ch->level > 38 ) times += 1;
	if( ch->level > 40 ) times += 1;

	times = UMIN(times, 7);
	if( sklv > 5) times++;
	if( sklv > 4 ) times += (sklv - 4) * 2;
	if( get_curr_dex(ch) > 22) times++;
	if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_DEX ) times += 2; // thi/swd 額外段數加成

	while(times-- && ch->mana >= 15 && ch->move >= 15){
		if ( number_percent() > 50){ // 命中到與自己的目標
			dam = (40 + level)/2;

			// swordmaster bonus
			dam *= 1 + get_skill_level(ch, gsn_swordmaster) * 0.02;

			//int,dex effect , thi/swd dex 影響較 int 大 add at 19/2/19
			if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_DEX ){
				dam += dam * (( get_curr_int(ch) - 10)*1 + ( get_curr_dex(ch) - 10 )*3) / 100;
			}else{
				dam += dam * (( get_curr_int(ch) - 10)*2 + ( get_curr_dex(ch) - 10 )*2) / 100;
			}
				
			// sklv effect
			dam *= 1 + 0.1*sklv + 0.1*sklv/3;
		
			dam = dam * weakness_damage( victim, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;

			if( !is_affected(victim, skill_lookup("prot. from wind")) && is_flying( victim ) ){
				dam = dam * (7 + sklv) / 6;
			}

			dam = number_range( dam * 0.2 , dam * (3 + sklvCri/3)); // critical 對storm 上限傷害加成 add at 19/2/19

			magic_damage( ch, victim, dam, sn, WEAR_NONE );
        		
			if ( !IS_NPC(ch)){
				ch->move -= 15;
				ch->mana -= 15;
			}
		}else{ // 命中到其他目標
			for( vch = ch->in_room->people; vch; vch = vch->next_in_room){
				if( vch->deleted ) continue;
				if( vch->fighting != ch && ch->fighting != vch) continue;
				if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;

				dam = (40 + level)/2;
				
				// swordmaster bonus
				dam *= 1 + get_skill_level(ch, gsn_swordmaster) * 0.02;

				//int,dex effect , thi/swd dex 影響較 int 大 add at 19/2/19
				if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_DEX ){
					dam += dam * (( get_curr_int(ch) - 10)*1 + ( get_curr_dex(ch) - 10 )*3) / 100;
				}else{
					dam += dam * (( get_curr_int(ch) - 10)*2 + ( get_curr_dex(ch) - 10 )*2) / 100;
				}

				// sklv effect
				dam *= 1 + 0.1*sklv + 0.1*sklv/3;

				dam = dam * weakness_damage( vch, sn ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, vch,MAGIC_WIND)) / 100;

				if( !is_affected(vch , skill_lookup("prot. from wind")) && is_flying( vch ) ){
					dam = dam * (7 + sklv) / 6;
				}

				if( !IS_NPC(ch) )
					dam = number_range( dam * 0.2 , dam * (3 + sklvCri/3)); // critical 對storm 上限傷害加成 add at 19/2/19
				else if ( IS_NPC(ch) )
					dam = number_range( dam * 0.2 , dam * 4 );

				magic_damage( ch, vch, dam, sn, WEAR_NONE );
				if ( !IS_NPC(ch)){
					ch->move -= 15;
					ch->mana -= 15;
				}
				break;
			}
		}
	}
}

void spell_fulgurant_slash( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	double dam = (40 + level)/2;
	int sklv = get_skill_level(ch, gsn_fulgurant_slash);

	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
		return;
	}

	sn = skill_lookup("fulgurant slash");

	// int,dex effect
	dam += dam * (( get_curr_int(ch) - 10)*2 + ( get_curr_dex(ch) - 10 )*2) / 100;

	// sklv effect
	dam *= 1 + 0.1*sklv + 0.1*sklv/3;
	
	if(sklv > 2) dam *= 1.06;
	if(sklv > 3) dam *= 1.06;
	if(sklv > 4) dam *= 1.05;
	if(sklv > 5) dam *= 1.045;
	
	// pp 對 sv lightning 穿透性
	int nSvT = NEW_GET_SVS( victim, MAGIC_LIGHTNING );
	if(nSvT >= 0){
		nSvT = UMAX(nSvT + ch->pp/(11-sklv) , nSvT);
	}else if(ch->pp > 0){
		nSvT = UMIN(nSvT + ch->pp/(10-sklv) , 0);
	}
	if(nSvT > 0){
		dam = dam * (60 + nSvT/2) / 60;
	}else{
		dam = dam * 60 / (60 - nSvT);
	}

	dam = dam * weakness_damage( victim, sn ) / 100;
	//dam = dam * ( old_saves_spell( ch->pp, victim, MAGIC_LIGHTNING)) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.6 , dam * 3.2 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 2.2 , dam * 2.8 );

	if(ch->amdr > victim->mdr){
		// dam += (ch->amdr - UMAX(0, victim->mdr)) * (8 + sklv * sklv / 3) / 10;
		// dam += (ch->amdr - UMAX(0, victim->mdr)) * (7 + sklv * sklv / 2) / 10; // from 2 to 2.5
		dam += (ch->amdr - UMAX(0, victim->mdr)) * (10 + sklv * 2.3) / 10; // 降低sklv 級距 提升低sklv 效果 add at 19.2.17
		dam -= UMIN(0, victim->mdr);
	}else{
		// dam -= victim->mdr - ch->amdr;
		dam -= UMAX(0, victim->mdr - ch->amdr * (1 + sklv / 2)); // 追加 mdr 較 md 高時 , 高 sklv 對 mdr 有穿透性 add at 19.2.17
	}

	// now svLightning will plays an important role
	//dam = dam * new_saves_spell(ch->pp / 4, victim, MAGIC_LIGHTNING) / 100;
	dam = dam * ( old_saves_spell( ch->pp, victim, MAGIC_LIGHTNING)) / 100;

	magic_damage( ch, victim, dam, sn, WEAR_NONE );
        
	if ( !IS_NPC(ch)){
		ch->move -= 10;
		ch->mana -= 10;
	}

	return;
}


//add by coconet 20041013
// modified by whisperwnd 2.18.2005
void spell_icecore_slash( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	CHAR_DATA *vch= (CHAR_DATA *) vo;
	CHAR_DATA *vch_next;

	double dam;
	double pp_dam;
	double dr_dam; // for maximize icecore physical damage use
	int nIccSklv   = get_skill_level(ch, gsn_icecore_slash);
	int nSprSklv   = get_skill_level(ch, gsn_sprinkle_slash);
	int nBodySklv  = get_skill_level(ch, gsn_bodycontrol);
	int nInt       = get_curr_int(ch);
	int nDex       = get_curr_dex(ch);
	int dr_curve   = 0;  // 2020/09/19
	int times = 1 + nIccSklv/3; // 段數設定

	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}
	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
		return;
	}

	sn = skill_lookup("icecore slash");

	dam = (40 + level)/2;

	// swordmaster bonus

	dam *= 1 + get_skill_level(ch, gsn_swordmaster) * 0.02 ; // 1.12

	// sklv effect
	// for lv41 .. dam = 70
	pp_dam = dam * (100 + nIccSklv * 10 + nIccSklv * nIccSklv) / 100; // 1.92
	dr_dam = dam * (1 + 0.125 * nIccSklv);  // 1.75

	// weather effect icc spell damage bonus
	if( weather_info.sky == SKY_SNOWING )
		pp_dam = pp_dam * 1.1;
	if( weather_info.sky == SKY_SNOWSTORM )
		pp_dam = pp_dam * 1.2;

	// bodycotrol sklv effect to maximize icecore physical damage
	// for icc 6 and body 6 with lv41 int 25 dex 20, the ex_dam = 192
	// for lv41, icc6, body 6, dam = 112
	pp_dam = pp_dam * (100 + nSprSklv * nSprSklv + nSprSklv * 6) / 140; //  1.23
	dr_dam = dr_dam * (100 + nBodySklv * nBodySklv + nBodySklv * 6) / 140; //  1.23

	// int,dex effect
	// for int 25, lv41, pp_dam = 157.92
	// for dex 25, lv41, dr_dam = 157.92, dex 21, dr_dam = 120
	pp_dam = pp_dam * (200 + (nInt - 16) * abs(nInt - 16)) / 200; // int 25 , 1.4  , int 21 1.125
	dr_dam = dr_dam * (200 + (nDex - 16) * abs(nDex - 16)) / 200; // dex 25 , 1.4 , dex 21 1.125
	if(dr_dam < 0) dr_dam = 0;
	if(pp_dam < 0) pp_dam = 0;

	// high dr curve for balance physical damage 2020/09/19 
	if( ch->damroll > 50) dr_curve += (ch->damroll - 50)/4;
	if( ch->damroll > 60) dr_curve += (ch->damroll - 60)/3 - (ch->damroll - 50)/4;
	if( ch->damroll > 70) dr_curve += (ch->damroll - 70)/2 - (ch->damroll - 60)/3;

	times = UMIN(times, 3);

	while( times-- )
	{

		if ( !IS_NPC(ch) && (ch->move < 85 || ch->mana < 85))
			break;

		if ( !IS_NPC(ch) ){
			ch->move -= 85;
			ch->mana -= 85;
		}

		// maximize icecore part , add extra physical damage to all enemy that beats you
		// dr replace pp to increase damage
		// ac replace svs to reduce damage
		if( !IS_NPC(ch) && nBodySklv > 3 && nIccSklv > 3)
		{
			dam = dr_dam * weakness_damage( victim, sn ) / 100;
			//dam = dam * ch->damroll/4;
			dam = dam * (ch->damroll - dr_curve) * 0.3;
			// increase icc agaist high ac mob 2020/09/21
			// ac -3600 dam reduce, old 2880  new 2400
			// ac -4500 dam reduce, old 4500  new 3000
			// ac -6000 dam reduce, old 8000  new 4000
			//dam += (victim->armor) * (victim->armor) / (0 - 4500);
			if(victim->armor < -3000)
			dam += ( (victim->armor + 3000)* 2 / 3) - 2000;
			else
			dam += (victim->armor) * (victim->armor) / (0 - 4500);
			if(dam < 0) dam = 0;
			dam += ch->apdr + (ch->apdr * nIccSklv)/15;
			dam -= victim->pdr;
			////_damage( ch, victim, dam, sn, WEAR_NONE );
			dam = get_critical_damage(dam, ch, victim, get_curr_dex(ch) - get_curr_dex(victim), 0, 0, 0);
			// dam = check_spellfury(ch, victim, dam);
			damage( ch, victim, dam, sn, WEAR_NONE, MSG_NORMAL, DMG_ADJUST );
		}

		// damage to your victim
		dam = pp_dam * weakness_damage( victim, sn ) / 100;
		dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
		if( !IS_NPC(ch) )
			dam = number_range( dam * 3 , dam * 4 );
		else if ( IS_NPC(ch) )
			dam = number_range( dam * 2 , dam * 3 );

		int nSpriSlow = affected_value(victim, gsn_sprinkle_slash, APPLY_ACTION_ABILITY);

		if(!is_affected(victim , skill_lookup("prot. from cold")) && is_affected(victim, gsn_sprinkle_slash) && nSpriSlow < 0)
			dam = dam * 125 / (125 + nSpriSlow * 0.7);

		if(nSprSklv > 4)
			magic__damage( ch, victim, dam * 2.0, sn, WEAR_NONE );
		else
			magic_damage( ch, victim, dam, sn, WEAR_NONE );


		// hit other enemy that beats you except your victim
		for( vch = ch->in_room->people; vch; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if( vch->deleted ) continue;
			if( vch->fighting != ch ) continue;
			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
			if( victim == vch ) continue;

			if( !IS_NPC(ch) && nBodySklv > 3 && nIccSklv > 3)
			{
				dam = dr_dam * weakness_damage( vch, sn ) / 100;
				dam = dam * (ch->damroll - dr_curve) * 0.3;
				if(vch->armor < -3000)
				dam += ( (vch->armor + 3000)* 2 / 3) - 2000;
				else
				dam += (vch->armor) * (vch->armor) / (0 - 4500);
				if(dam < 0) dam = 0;
				dam += ch->apdr + (ch->apdr * nIccSklv)/15;
				dam -= vch->pdr;
				dam = get_critical_damage(dam, ch, vch, get_curr_dex(ch) - get_curr_dex(vch), 0, 0, 0);
				damage( ch, vch, dam, sn, WEAR_NONE, MSG_NORMAL, DMG_ADJUST );
			}

			if(vch->deleted)
				continue;

			dam = pp_dam * weakness_damage( vch, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, vch,MAGIC_COLD)) / 100;

			if( !IS_NPC(ch) )
				dam = number_range( dam * 3 , dam * 4 );
			else if ( IS_NPC(ch) )
				dam = number_range( dam * 2 , dam * 3 );

			int nSpriSlow = affected_value(vch, gsn_sprinkle_slash, APPLY_ACTION_ABILITY);

			if(!is_affected(victim , skill_lookup("prot. from cold")) && is_affected(vch, gsn_sprinkle_slash) && nSpriSlow < 0)
				dam = dam * 125 / (125 + nSpriSlow);

			if(nSprSklv > 4)
				magic__damage( ch, vch, dam * 2.0, sn, WEAR_NONE );
			else
				magic_damage( ch, vch, dam, sn, WEAR_NONE );
		}
	}
	return;
}

void spell_sun_slash(int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *vch, *vch_next;
	AFFECT_DATA af;
	int nDamage    = 0;
	int nSpellSklv = get_skill_level(ch, gsn_sun_flash);
	int nSlashSklv = get_skill_level(ch, gsn_sun_slash);
	int nCurrInt   = get_curr_int(ch);
	int nCurrStr   = get_curr_str(ch);
	int nBlindRate = 0;
	int nBlindBase = nSlashSklv * 2 / 3 + nSpellSklv;


	if( is_affected(ch, gsn_stone_form) ){
		send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
		return;
	}
	if(IS_NPC(ch) && nBlindBase < 20) nBlindBase = 20;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
		vch_next = vch->next_in_room;
		if ( vch == ch || vch->deleted || vch->position == POS_DEAD ) continue;
		if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
		if (  !IS_NPC(vch) && is_affected(vch, gsn_invoke)
		    && get_skill_level(vch, gsn_invoke) > 3 )
			continue;

		if (   !is_same_group( ch, vch )
		    && (    ( IS_NPC( ch ) && ( vch->fighting == ch || !IS_NPC( vch ) ) )
		         || ( !IS_NPC( ch ) && ( vch->fighting == ch
		                                || (    IS_NPC( vch ) && !IS_AFFECTED( vch, AFF_CHARM )
		                                    && !IS_AFFECTED( vch, AFF_SUMMONED ) )
		                               )
		            )
		       )
		   )
		{
			nDamage  = (40 + level) * (100 + (nCurrInt - 10) * 3 + (nCurrStr - 10) * 3) / 200;

			// inborn DETECT_ALIGN 的 race (fairy , gnome) 有額外加成 2020/05/29
			if(IS_SET( race_table[ ch->race ].race_abilities, RACE_DETECT_ALIGN ))
				nDamage *= 1 + get_skill_level(ch, gsn_inborn_ability) * 0.08;

			// swordmaster bonus
			nDamage *= 1 + get_skill_level(ch, gsn_swordmaster) * 0.02 ;
			
			nDamage *= 1 + 0.25 * (nSlashSklv) + 0.07 * (nSpellSklv);
			
			if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_STR )
				nDamage *= 1 + ch->apdr/620;

			nDamage  = nDamage * weakness_damage( vch, sn ) / 100;
			nDamage  = nDamage * ( 1 * new_saves_spell( ch->pp, vch, MAGIC_SPELL) 
			                       + ( 1 + nCurrStr * nCurrStr / 300) * new_saves_spell( (ch->apdr * 2)/9, vch, MAGIC_SAINT)) / 350;
			nDamage  = number_range( nDamage * 4 , nDamage * 5.3 );
			
			if( IS_NPC(ch)) nDamage /= 2;
			// 有 mystic armor 不作用負面 2020/10/26
			if( !IS_AFFECTED(vch, AFF_MYSTIC_ARMOR ) ){
				if(weather_info.sky < SKY_RAINING || IS_NPC(ch)){
					if (affected_value( vch, skill_lookup("day of brightness"), APPLY_DOB_SAVING_FLASH) > number_percent( ) )
					{
						send_to_char( HIW"水神的力量張出一面光鏡，守護你的視覺不受這波光爆的侵襲！"NOR"\n\r", vch );
						nDamage = nDamage * 75 / 100;
					}else{
						nBlindRate  = nBlindBase * old_saves_spell( ch->pp , vch, MAGIC_SAINT) / 100;
						nBlindRate += WIS_DIFF(ch, vch);
						nBlindRate  = nBlindRate * weakness_percent(vch, gsn_sun_flash) / 100;
						nBlindRate  = nBlindRate * weakness_percent(vch, gsn_sun_slash) / 100;
						nBlindRate  = nBlindRate * weakness_percent(vch, gsn_blindness) / 100;
						if ( !IS_OUTSIDE( ch ) && !IS_NPC( ch )) nBlindRate = nBlindRate * 2 / 3;
						if ( number_percent() < nBlindRate)
						{

							if (!(IS_AFFECTED( vch, AFF_BLIND ) || IS_SET( vch->special, NO_BLIND ))
								&& number_percent() < nBlindBase)
							{
								af.type	     = gsn_sun_flash;
								af.duration  = 2;
								af.location  = APPLY_HITROLL;
								af.modifier  = -12 - nSlashSklv*3 - nSpellSklv;
								af.bitvector = AFF_BLIND;
								affect_to_char( vch, &af );

								act( "$N "HIG"突然失去了視力!"NOR"", ch, NULL, vch, TO_CHAR	);
								send_to_char( "你的眼前一亮，世界都變黑了！\n\r", vch );
								act( "$N "HIG"突然失去了視力!"NOR"", ch, NULL, vch, TO_NOTVICT );
							}
						}
					}
				}
			}
			magic__damage( ch, vch, nDamage, gsn_sun_slash, WEAR_NONE );
		}
	}
}


void spell_elemental_slash( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	double dam     = 0;
	int nFlameSklv = get_skill_level(ch, gsn_flaming_slash);
	int nStormSklv = get_skill_level(ch, gsn_storm_slash);
	int nIceCoSklv = get_skill_level(ch, gsn_icecore_slash);
	int nEarthSklv = get_skill_level(ch, gsn_earth);
	int nFulguSklv = get_skill_level(ch, gsn_fulgurant_slash);
	int nInt       = get_curr_int(ch);
	int nDex       = get_curr_dex(ch);
	int ex_pp      = ch->pp;

	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}
  if( is_affected(ch, gsn_stone_form) ){
      send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
      return;
  }

	/////////////////////////////////////////////////////////////
	// Fire Element
	int nFireSklv =  get_skill_level(ch, skill_lookup("fireball"));
	sn   = skill_lookup("flaming slash");
	dam  = ((40 + level) / 2) * (100 + (nInt - 10) * 2 + (nDex - 10) * 2) / 100;
	dam  = dam * (10 + nFlameSklv + nFireSklv) / 10;
	if( nFlameSklv > 3) dam = dam * 1.1;
	if( nFlameSklv > 5) dam = dam * 1.2;
	
	if(nFireSklv > 5) ex_pp += UMAX( ch->pp - 20 , 0 );
	if(nFireSklv > 4) ex_pp += UMAX( ch->pp - 30 , 0 );
	if(nFireSklv > 3) ex_pp += UMAX( ch->pp - 40 , 0 );
	if(nFireSklv > 2) ex_pp += UMAX( ch->pp - 50 , 0 );

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( old_saves_spell( ex_pp, victim,MAGIC_FIRE)) / 100;
	dam = number_range( dam * 6 , dam * 8 );
	if ( IS_NPC(ch) )	dam /= 2;	
	magic__damage( ch, victim, dam, sn, WEAR_NONE );
	if ( victim == NULL || victim->deleted || victim->in_room != ch->in_room ) return;

	////////////////////////////////////////////////////////////
	// Wind Element
	int nWhirlSklv = get_skill_level(ch, skill_lookup("whirlwind"));
	sn   = skill_lookup("storm slash");
	dam  = ((40 + level) / 2) * (100 + (nInt - 10) * 2 + (nDex - 10) * 2) / 100;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * old_saves_spell( ch->pp, victim,MAGIC_WIND) / 100;
		
	dam = number_range( 1 , dam * (6 + 2 * (nStormSklv + nWhirlSklv)));

	if( is_flying( victim ) ){
		dam = dam * (10 + (nStormSklv + 1) * (nWhirlSklv + 1) / 7) / 10;
	}
	if ( IS_NPC(ch) ) dam /= 2;
	magic__damage( ch, victim, dam, sn, WEAR_NONE );
	if ( victim == NULL || victim->deleted || victim->in_room != ch->in_room ) return;

	////////////////////////////////////////////////////////////
	// Water Element
	
	int nBodySklv  = get_skill_level(ch, gsn_bodycontrol);
	sn   = skill_lookup("icecore slash");
	
	dam  = (40 + level)/2;
	dam  = dam * (10 + nIceCoSklv * nIceCoSklv / 3) / 10;
	dam  = dam * (10 + nBodySklv  * nBodySklv  / 3) / 10;
	
	dam  = dam * (200 + (nInt - 16) * abs(nInt - 16)) / 200;
	if(dam < 0) dam = 0;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;      
	dam = number_range( dam * 2.5 , dam * 4.5 );
	if ( IS_NPC(ch) ) dam /= 2;

	magic__damage( ch, victim, dam, sn, WEAR_NONE );
	if ( victim == NULL || victim->deleted || victim->in_room != ch->in_room ) return;

	////////////////////////////////////////////////////////////
	// Earth Element
	int nBerSklv = get_skill_level(ch, gsn_berserk);
	sn   = skill_lookup("earth slash");
	dam  = ((40 + level) / 2) * (100 + (nInt - 10) * 2 + (nDex - 10) * 2) / 100;
	dam *= 1 + 0.2 * nEarthSklv;

	dam  = dam * weakness_damage( victim, sn ) / 100;
	dam *= ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
	dam  = number_range( dam * 2.5 , dam * 4.0 );
					// maximize earth part : berserk earth slash
					// when sklv > 3 & berserk sklv > 3 & victim affected tardy
					// cause (2.5 + con/5)x damage
	if ( is_affected( ch, skill_lookup("berserk"))){
		dam = dam * (13 + nBerSklv * nBerSklv / 3) / 10;
	}
	if ( is_affected( victim, skill_lookup("tardy")))
	{
		dam = dam * (20 + get_curr_con(ch)) / 20;
	}
	if ( IS_NPC(ch) ) dam /= 2;

	magic__damage( ch, victim, dam, sn, WEAR_NONE );
	if ( victim == NULL || victim->deleted || victim->in_room != ch->in_room ) return;

	////////////////////////////////////////////////////////////
	// Fulgurant Element
	int nSwordSklv  = get_skill_level(ch, gsn_swordmaster);
	sn   = skill_lookup("fulgurant slash");
	dam  = ((40 + level) / 2) * (100 + (nInt - 10) * 2 + (nDex - 10) * 2) / 100;
	dam  = dam * (10 + nFulguSklv + nSwordSklv) / 10;
				
	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( old_saves_spell( ch->pp, victim, MAGIC_LIGHTNING)) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.5 , dam * 4.0 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 2.5 );

	if(ch->amdr > victim->mdr){
		dam += (ch->amdr - UMAX(0, victim->mdr)) * (1 + (nSwordSklv + 1) * (nFulguSklv + 1)) / 5;
		dam -= UMIN(0, victim->mdr);
	}else{
		dam -= victim->mdr - ch->amdr;
	}
	magic__damage( ch, victim, dam, sn, WEAR_NONE );
        
	return;
}

void spell_sprinkle_slash( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim	 = (CHAR_DATA *) vo;

    if( sn != -1 )
    {
        send_to_char("那不是法術！\n\r", ch);
        return;
    }

    sn = skill_lookup("sprinkle slash");

    spell_colour_spray(sn, level, ch, vo);

    return;
}

void spell_earth_slash( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim, *victim_next;
	double dam;
	double basic_dam;
	int sklv = get_skill_level(ch, gsn_earth);
	int times = 1 + sklv/2; // 段數設定
	int EXtimes = 3; //1060109 add by jye 追加段數設定
	int mobCnt = 0; //1060109 add by jye 判斷同格戰鬥 mob 數
	int Randpercent = 0; //1060109 add by jye 判斷rand percent
	int RandpercentMax = 0; //1060109 add by jye 判斷rand percentMax
	int sklv_ber = get_skill_level(ch, gsn_berserk);//1060109 add by jye
	CHAR_DATA *victim_temp = NULL;
	
	if( sn != -1 ){
		send_to_char("那不是法術！\n\r", ch);
		return;
	}
  if( is_affected(ch, gsn_stone_form) ){
      send_to_char("你無法在石化狀態下使用技巧.\n\r", ch);
      return;
  }

	sn = skill_lookup("earth slash");

	//if(is_affected( ch, skill_lookup("berserk")))
	//	act( "$n強烈的劍氣使得周圍的地面激烈地震動.", ch, NULL, NULL, TO_ROOM );
	//else
	//	act( "$n的劍氣使得周圍的地面激烈地震動.", ch, NULL, NULL, TO_ROOM );
  if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER))  {
        if( IS_GOOD(ch) ) {
            act(YEL"你將鬥氣集中劍尖朝地面一插，大地隨之產生強大的震波向敵人衝去！"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"YEL"將鬥氣集中劍尖朝地面一插，大地隨之產生強大的震波向敵人衝去！"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(YEL"你將殺意灌輸劍身朝地面一插，前方地面產生巨大的裂痕向敵人衝去！"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"YEL"將殺意灌輸劍身朝地面一插，前方地面產生巨大的裂痕向敵人衝去！"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(YEL"你將劍氣圍繞劍刃朝地面一插，地表產生數道劍波朝向敵人衝去！"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"YEL"將劍氣圍繞劍刃朝地面一插，地表產生數道劍波朝向敵人衝去！"NOR, ch, NULL, NULL, TO_ROOM); 
        }
  }else{
        if( IS_GOOD(ch) ) {
            act(YEL"你將精神力幻化為巨大的魔法劍朝地面插入，產生震波朝敵人衝去！"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"YEL"將精神力幻化為巨大的魔法劍朝地面插入，產生震波朝敵人衝去！"NOR, ch, NULL, NULL, TO_ROOM); 
        }else if ( IS_EVIL(ch) ) {
            act(YEL"你將精神力幻化為巨大的魔法劍朝地面插入，產生地裂朝敵人衝去！"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"YEL"將精神力幻化為巨大的魔法劍朝地面插入，產生地裂朝敵人衝去！"NOR, ch, NULL, NULL, TO_ROOM); 
        }else {
            act(YEL"你將精神力幻化為巨大的魔法劍朝地面插入，產生劍波朝敵人衝去！"NOR, ch, NULL, NULL, TO_CHAR); 
            act("$n"YEL"將精神力幻化為巨大的魔法劍朝地面插入，產生劍波朝敵人衝去！"NOR, ch, NULL, NULL, TO_ROOM);
        }
  }

	dam = (40 + level)/2; // 40.5
	
	// swordmaster bonus

	dam *= 1 + get_skill_level(ch, gsn_swordmaster) * 0.02 ; // 1.12 , 45.36

	// int,con effect
	//dam += dam * (( get_curr_int(ch) - 10)*2 + ( get_curr_con(ch) - 10 )*2) / 100;
	if( !IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_CON )
		dam += dam * UMAX( get_curr_int(ch)/50 + (get_curr_con(ch) - 18) * 0.2 , 0.1 ); // 1.9
	else
		dam += dam * UMAX( (get_curr_int(ch) - 18) * 0.2 , 0.1 ); // 1.4

	// sklv effect
	dam *= 1 + 0.2*sklv + 0.1*sklv/3 ; // 2.4 
	
	if(!IS_NPC(ch) && ch->pcdata->prime_attr == APPLY_CON && ch->apdr > 0)
		dam += ch->apdr/10; // pd 300 , +30

	basic_dam = dam;
	
	while( times-- ){
		mobCnt = 0;
		
		if(ch->move < 55 || ch->mana < 55) break;

		if ( !IS_NPC(ch)){
			ch->move -= 55;
			ch->mana -= 55;
		}
		for( victim = ch->in_room->people; victim; victim = victim_next){
			victim_next = victim->next_in_room;
			if( victim->deleted ) continue;
			if( is_same_group( ch, victim ) ) continue;
			if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS)) continue;
			//1060109 modify by jye 修改判斷 sklv 4 以下才打不到flying目標
			//if( is_flying( victim ) && !IS_AFFECTED( victim, AFF_TARDY ) ) continue;
			if( sklv <= 4 && is_flying( victim ) && !IS_AFFECTED( victim, AFF_TARDY ) ) continue;

			if ( IS_NPC( ch ) ){
				if ( victim->fighting == ch || !IS_NPC( victim ) ){
					dam  = basic_dam * weakness_damage( victim, sn ) / 100;
					//dam *= ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
					dam = UMAX(dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100, basic_dam);
					dam  = number_range( dam * 1 , dam * 2 );
					if(is_affected( ch, skill_lookup("berserk"))){
						dam = dam * (300 + ch->level + get_curr_con(ch)) / 300;
					}
					if(!is_affected( victim, skill_lookup("prot. from earth")) && is_affected( victim, gsn_tardy ))
					{
						// tardy 數值作用 2022/11/06
						if( affected_value(victim, gsn_tardy , APPLY_DODGE ) > 0 )
							dam = dam * ( 110 + affected_value(victim, gsn_tardy , APPLY_DODGE )/17 ) / 100;
						else
							dam = dam * 110 / 100;
						magic__damage( ch, victim, dam, sn, WEAR_NONE );
					} else {
						magic_damage( ch, victim, dam, sn, WEAR_NONE );
					}
					continue;
				}
			}else if ( victim->fighting == ch || ( IS_NPC( victim ) && 
			           !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) ){
				mobCnt += 1;
			  
				dam  = basic_dam * weakness_damage( victim, sn ) / 100;
				//dam *= ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
				dam = UMAX(dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100, basic_dam);
				
				dam  = number_range( dam , dam * 1.5 );

				//if(is_affected( victim, skill_lookup("tardy"))) dam = dam * 11 / 10;
				if(is_affected( ch, skill_lookup("berserk"))){
					//int nBerSklv = get_skill_level(ch, gsn_berserk);
					//dam = dam * (150 + nBerSklv * (nBerSklv + get_curr_con(ch)/5)) / 150;
					dam = dam * (150 + sklv_ber * (sklv_ber + get_curr_con(ch)/3)) / 150;
				}

				if(!is_affected( victim, skill_lookup("prot. from earth")) && is_affected( victim, gsn_tardy ) )
				{
					// tardy 數值作用 2022/11/06
					if( affected_value(victim, gsn_tardy , APPLY_DODGE ) > 0 )
						dam = dam * ( 105 + affected_value(victim, gsn_tardy , APPLY_DODGE )/14 ) / 100;
					else
						dam = dam * 105 / 100;
					magic__damage( ch, victim, dam, sn, WEAR_NONE );
				} else {
					magic_damage( ch, victim, dam, sn, WEAR_NONE );
				}
				continue;
			}
		}
	}
	
	if(ch->move < 30 || ch->mana < 30)
		return;
	
	//1060109 add by jye 追加段數
	if (sklv_ber == 6) 
		EXtimes = 4;
	else if (sklv_ber == 5) 
		EXtimes = 3;
	else if (sklv_ber == 4) 
		EXtimes = 2;
	else
		EXtimes = 1;

	if (mobCnt == 1 && sklv_ber == 6)
		EXtimes = 4;

	victim_temp = victim;

	if(is_affected( ch, skill_lookup("berserk")) && !IS_NPC(ch) && EXtimes > 0)
	{
	
		if(!IS_SET(race_table[ ch->race ].race_abilities, RACE_BEHOLDER))
		{
			if( IS_GOOD(ch) ) {
				act(YEL"強大的震波讓地表延伸出尖岩，將敵人應聲轟起！"NOR, ch, NULL, NULL, TO_CHAR);
				act(YEL"強大的震波讓地表延伸出尖岩，將敵人應聲轟起！"NOR, ch, NULL, NULL, TO_ROOM);
			}else if ( IS_EVIL(ch) ){
				act(YEL"從巨大的地裂噴出數塊巨岩，朝敵人襲去！"NOR, ch, NULL, NULL, TO_CHAR);
				act(YEL"從巨大的地裂噴出數塊巨岩，朝敵人襲去！"NOR, ch, NULL, NULL, TO_ROOM);
			}else{
				act(YEL"劍波揚起地表的岩石，朝敵人飛去！"NOR, ch, NULL, NULL, TO_CHAR);
				act(YEL"劍波揚起地表的岩石，朝敵人飛去！"NOR, ch, NULL, NULL, TO_ROOM);
			}
		}else{
			if( IS_GOOD(ch) ) {
				act(YEL"你將精神力幻化為巨大的魔法劍插入地表延伸出尖岩，朝敵人襲去！"NOR, ch, NULL, NULL, TO_CHAR);
				act("$n"YEL"將精神力幻化為巨大的魔法劍插入地表延伸出尖岩，朝敵人襲去！"NOR, ch, NULL, NULL, TO_ROOM);
			}else if ( IS_EVIL(ch) ) {
				act(YEL"你將精神力幻化為巨大的魔法劍朝地面插入噴出數塊巨岩，朝敵人襲去！"NOR, ch, NULL, NULL, TO_CHAR);
				act("$n"YEL"將精神力幻化為巨大的魔法劍朝地面插入噴出數塊巨岩，朝敵人襲去！"NOR, ch, NULL, NULL, TO_ROOM);
			}else {
				act(YEL"你將精神力幻化為巨大的魔法劍揚起地表的岩石，朝敵人襲去！"NOR, ch, NULL, NULL, TO_CHAR);
				act("$n"YEL"將精神力幻化為巨大的魔法劍揚起地表的岩石，朝敵人襲去！"NOR, ch, NULL, NULL, TO_ROOM);
			}
		}
		while( EXtimes-- )
		{
			if(ch->move < 30 || ch->mana < 30) break;
			
			ch->move -= 30;
			ch->mana -= 30;
			for( victim = ch->in_room->people; victim; victim = victim_next){
				victim_next = victim->next_in_room;
				if( victim->deleted ) continue;
				if( is_same_group( ch, victim ) ) continue;
				if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS)) continue;

				// berserk 的加成段不會受到 fly 影響 2022/11/06
				//if( is_flying( victim ) && !IS_AFFECTED( victim, AFF_TARDY ) ) continue;
	   
				if ( victim->fighting == ch || ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
				{
					Randpercent = number_percent();
					if (RandpercentMax <= Randpercent || mobCnt == 1){
						RandpercentMax = Randpercent;
						victim_temp = victim;
						break;
					}
					if (RandpercentMax <= Randpercent || mobCnt > 1){
						RandpercentMax = Randpercent;
						victim_temp = victim;
						break;
					}
				}
			}

			if ( victim_temp == NULL || victim_temp->deleted || victim_temp->in_room != ch->in_room ) return;
	  	
 			dam  = basic_dam * weakness_damage( victim_temp, sn ) / 100;
			dam = UMAX(dam * ( new_saves_spell( ch->pp, victim_temp,MAGIC_EARTH)) / 100, basic_dam);
			dam  = number_range( dam * 2 , dam * 3 );
			dam = dam * (150 + sklv_ber * (sklv_ber + get_curr_con(ch) / 3)) / 150;
 			dam += UMAX((ch->damroll * 1.2 + ch->apdr * 1.8 ) * UMAX((get_curr_con(ch) - 15) * 0.35 , 0.8 ) , 0);
 			dam -= victim_temp->pdr * 4 / 7;
    
 			if(!is_affected( victim_temp, skill_lookup("prot. from earth")) && is_affected( victim_temp, gsn_tardy) )
			{
				// tardy 數值作用 2022/11/06
				if( affected_value(victim_temp, gsn_tardy , APPLY_DODGE ) > 0 )
					dam = dam * ( 105 + affected_value(victim_temp, gsn_tardy , APPLY_DODGE )/13 ) / 100;
				else
					dam = dam * 105 / 100;
 				magic__damage( ch, victim_temp, dam , sn, WEAR_NONE );
 			} else {
 				magic_damage( ch, victim_temp, dam, sn, WEAR_NONE );
			}
		}
	}
	return;
}