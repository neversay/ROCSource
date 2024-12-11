//*****************************************************************
//*  PROGRAM ID  : spell_aid.c                                    *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940605 jye                                    *
//*  PURPOSE     :         修改spell_blindness之變數相乘為0       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940606 jye                                    *
//*  PURPOSE     :         調整程式風格及增加可Debug之Function    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940611 jacquex                                *
//*  PURPOSE     :         修正程式風格，減少Function Call        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940714 Razgriz                                *
//*  PURPOSE     :         修改 spell_charm_person()，            *
//*              :         加入 charm_person 的 qmark 處理        *
//*****************************************************************

#include "skill.h"


/* $Id: spell_aid.c 1372 2005-06-12 07:56:26Z jye $ */
//add Debug Function by jye 0940606
void Show_Debug( CHAR_DATA *ch, const char *txt, int int_num );
extern bool IsPK(CHAR_DATA *ch, CHAR_DATA *victim);
extern int  GetWillFortressEffect(CHAR_DATA *victim);

void Show_Debug( CHAR_DATA *ch, const char *txt, int int_num )
{
  char        buf     [ MAX_STRING_LENGTH * 2];

  sprintf(buf, "%s %s %d %s \n\r", HIY,txt,int_num,NOR);
  send_to_char( buf, ch );
  return;
}
// blindness modified by jye 6.05.2005
void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int    sklv        = get_skill_level(ch,sn);
  double chance      = 10;
  int    intblind    = skill_lookup("blindness");
  int    intSunflash = skill_lookup("sun flash");
  
  if(IsPK(ch, victim)) chance /= 2;

  chance = chance * int_effect( ch );
  
  if( sklv > 0 )
      chance = chance * ( 10 + 1.5 * sklv ) * 0.1 ;

  chance *= ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) * 0.01;
  
  chance = UMIN(chance, chance + sklv );
  
  //modify by jye 0940605 修改變數相乘為0
  chance *= weakness_percent( victim, sn ) * 0.01;

  if( !IS_NPC(ch) && ch->level < 31 )
      chance += 40;
  
  if ( IS_SET( victim->special, NO_BLIND ) 
       || IS_SET( race_table[ victim->race ].race_abilities, RACE_BEHOLDER ) 
       || ( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 2 )
       || number_percent() > chance )

  {                                     
    send_to_char( "你失敗了.\n\r", ch );
    return;
  }

  if ( IS_AFFECTED( victim, AFF_BLIND )) 
  {
    if ( is_affected( victim, intblind))
        affect_strip( victim, intblind);
    else if ( is_affected( victim, intSunflash))
        affect_strip( victim, intSunflash);
    else
        return; 
  }
      
  af.type  = sn;
  af.duration  = 1 + sklv;
  if(IsPK(ch, victim)) af.duration /= 4;
  af.location  = APPLY_HITROLL;
  af.modifier  = (-4 - sklv) * weakness_damage( victim, sn ) * 0.01;
  af.bitvector = AFF_BLIND;

  if(IsPK(ch, victim)) af.duration /= 2;

  affect_to_char( victim, &af );

  act( HIR "指間的火燄化成耀目的火光，刺中了"NOR"$N"HIR"的眼睛！"NOR"", ch, NULL, victim, TO_CHAR );
  act( HIR "眼前閃過異常明亮的火光，緊接而來的就是無盡的黑暗！"NOR"" , ch, NULL, victim, TO_VICT);
  act( "$n"HIR"手中射出的火光命中了"NOR"$N"HIR"的眼睛！"NOR"", ch, NULL, victim, TO_NOTVICT );
  
  mprog_hurt_trigger(victim, ch, sn);
  
  return;
}

// paralyse modified by jye 6.06.2005
void spell_paralyze( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	int sklv = get_skill_level(ch,sn);
	double chance = 30;
	//int intParalyze = skill_lookup("paralyze");
	// add at 2022/06/14
	int DcReduce = 0;
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

	if(IsPK(ch, victim)) chance /= 2;
		chance = chance * wis_effect( ch );

	if( sklv > 0 )
		chance = chance * ( 10 + sklv) * 0.1 ;
	if( get_skill_level(ch,sn) > 3 )
		chance = chance * 1.1;

	chance = chance * weakness_percent( victim, sn ) * 0.01;
	chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) * 0.01;

	if( !IS_NPC(ch) && ch->level < 31 )
		chance += 25;

	if( (IS_SET( victim->special, NO_PARA ))
		|| ( victim->race == race_lookup( "Vampire" )) 
		|| (victim->race == race_lookup( "Undead" ))
		|| (IS_AFFECTED( victim, AFF_GHOUL ))  // 轉變為不死生物時等同不死系(undead) 2022/06/15
		|| ( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 3 )
		|| ( number_percent() > chance ))
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	/*  判定修改為, 目標已經被麻痺 或 目標身上帶有 paralyze 的 affect 則不會被重覆施法 2022/06/14
	
		if( IS_AFFECTED( victim, AFF_PARALYZED )) 
		{
			if( is_affected( victim, intParalyze))
				affect_strip( victim, intParalyze);
			else
				return; 
		}
	*/
	if( IS_AFFECTED( victim, AFF_PARALYZED ) || is_affected( victim, gsn_paralyze) )
		return;

	// dc 降低量受 sklv 影響 2022/06/14
	DcReduce = 150 + sklv * 100;

	if( !IS_NPC(victim) ) DcReduce = DcReduce/2;

	af.type  = gsn_paralyze;
	af.duration  = 1;
	//if(IsPK(ch, victim)) af.duration /= 4;
	af.location  = APPLY_DODGE;
	af.modifier  = DcReduce * weakness_damage( victim, sn ) * 0.01;
	af.bitvector = AFF_PARALYZED;
	affect_to_char( victim, &af );

	act( HIG "看不見的毒之縛鍊纏住了"NOR"$N"HIG"，令他動彈不得！"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIG "你覺得自己的身體變得極為僵硬，無法隨心所欲地挪動！"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$N"HIG"的動作逐漸遲緩下來，最後竟然完全停止了！"NOR"", ch, NULL, victim, TO_NOTVICT );

	mprog_hurt_trigger(victim, ch, sn);
	return;
}

// seal magic modified by jye 6.06.2005
void spell_seal_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int sklv = get_skill_level(ch,sn);
  double chance = 30;
  int intSeal = skill_lookup("seal magic");

  if(IsPK(ch, victim)) chance /= 2;
  chance = chance * int_effect( ch );

  if( !IS_NPC(ch) && sklv > 0 )
      chance = chance * ( 10 + sklv) * 0.1 ;
  else if ( IS_NPC(ch) )
      chance = chance * 1.4;
    
  if( sklv > 4 )
      chance = chance * 1.1;

  chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) * 0.01;
  chance = UMIN(chance, 50 + sklv * 4);
  chance = chance * weakness_percent( victim, sn ) * 0.01;

  if( IS_SET( victim->special, NO_SEALMAGIC) || number_percent() > chance )
  {
      send_to_char( "你失敗了.\n\r", ch );
      return;
  }

  if( IS_AFFECTED( victim, AFF_SEAL_MAGIC )) 
  {
      if( is_affected( victim, intSeal))
          affect_strip( victim, intSeal);
      else
          return; 
  }
        
  af.type  = gsn_seal_magic;
  af.duration  = sklv;
  af.location  = APPLY_NONE;
  af.modifier  = 0;
  af.bitvector = AFF_SEAL_MAGIC;
  if(IsPK(ch, victim)) af.duration /= 4;
  affect_to_char( victim, &af );

  act( YEL "高位大地精靈化身成為寫滿了咒文的束帶，圍住了$N" YEL "！"NOR"", ch, NULL, victim, TO_CHAR );
  act( YEL "數不清的束帶圍繞著你，上面的文字正侵入你的身體之中！"NOR"" , ch, NULL, victim, TO_VICT);
  act( YEL "高位大地精靈化身成為寫滿了咒文的束帶，圍住了$N" YEL "！"NOR"", ch, NULL, victim, TO_NOTVICT );
  
  mprog_hurt_trigger(victim, ch, sn); 
  
  return;
}

// charm person modified by jye 6.06.2005
void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	int sklv = get_skill_level(ch,sn);
	char  buf[ MAX_STRING_LENGTH ];

	if ( victim == ch )
	{
		 send_to_char( "你覺得自己還不夠愛自己嗎？\n\r", ch );
		 return;
	}

	if ( get_trust( ch ) >= L_APP && get_trust( ch ) < L_SEN )
	{
		send_to_char( "以你的能力並不需要魅惑別的生物。\n\r",ch );
		sprintf( buf, "%s charming", ch->name );
		log_string( buf );
		return;
	}

	if( IS_IMMORTAL( victim ) )
	{
	  send_to_char( "你的能力並不足以魅惑眼前的目標。\n\r", ch );
	  sprintf( buf, "%s charming", ch->name );
	  log_string( buf );
	  return;
	}

	if( IsPK(ch, victim) ) 
	{
		if( !IS_SET( ch->act, PLR_REGISTER ) && (ch->in_room && !IS_SET(ch->in_room->room_flags, ROOM_PK))) 
		{
			 send_to_char("你並沒有參加 PK！\n\r", ch);
			 return;
		}
		if( is_safe( ch, victim ) )
			return;
	}

	if ( IS_SET( victim->special, NO_CHARM ) || (( IS_NPC( victim ) && ( victim->pIndexData->pShop ))) ){
		// victim 沒有在戰鬥才會對目標作 multi_hit 2022/12/31
		if ( !(victim->fighting) )
			multi_hit( victim, ch, TYPE_UNDEFINED );
		return;
	}

	if ( IS_AFFECTED( victim, AFF_CHARM )
		 || IS_AFFECTED( ch,  AFF_CHARM )
		 || ch->level + get_skill_level(ch,sn) < victim->level
		 || saves_spell( ch->pp, victim ) < (number_percent( ) * 100 / weakness_percent( victim, sn)) )
		return;
	if (IsPK(ch, victim) && saves_spell( ch->pp, victim ) < number_percent( )){
		return;
	}

	if ( victim->master )
		stop_follower( victim );
		 
	add_follower( victim, ch );
	af.type      = sn;
	af.duration  = number_fuzzy( ch->level / 4 )+1 + (sklv > 1 ? : sklv * 2, 0);
	if(IsPK(ch, victim)) af.duration /= 8;
	af.location  = APPLY_INT;
	af.modifier  = -2;
	af.bitvector = AFF_CHARM;
	affect_to_char( victim, &af );
	if ( victim->position == POS_FIGHTING )
		stop_fighting( victim, TRUE );
	//Added by Razgriz    20050714
	if ( IS_NPC ( victim ) )
	{
		sprintf ( buf, "self roc_mob_charmed -1 %d 0 0 0 1", victim->in_room->vnum );
		do_mpadd_qmark ( victim, buf );
	}

	send_to_char( "好的。\n\r", ch );
	act( "$n 看起來真是太有魅力了。", ch, NULL, victim, TO_VICT );
	WAIT_MAGIC_STATE(ch, (24 - sklv));

	mprog_hurt_trigger(victim, ch, sn);

	return;
}

// virtual swords modified by jye 6.06.2005
void spell_virtual_swords( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *rch = NULL,  *swd = NULL, *sor = NULL;
	CHAR_DATA *high_pp = NULL, *high_wis = NULL, *high_int = NULL;
	int lv;
	int sklv = get_skill_level(ch, sn);
	int rsklv = 0;
	int nVirChance = 0;
	int nWillFort = 0;
	int sormana = 40 - get_skill_level(ch,sn);
	/* Don't allow charmed mobs to do this, check player's level */
	if( ch->in_room && IS_SET(ch->in_room->room_flags,ROOM_SAFE)){
		send_to_char("這裡禁止戰鬥。\n\r", ch);
		return ;
	}

	if( !ch->in_room || !ch->fighting)
		return;

	if( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) )){
		send_to_char("你失敗了。\n\r", ch);
		return;
	}

	if( IS_NPC(ch) ){
		swd = ch;
		sor = ch;
		rch = ch;
	}else{
		if( ch->class == SORCERER ){
			sor = ch;
			// 修改為判定 cooperator 2021/11/23
			//for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room )
			for( rch = ch->cooperator ; rch ; rch = rch->next_in_coop )
			{
				if( !IS_NPC( rch )
					&& rch != ch
					&& rch->class == SWORDSMAN
					&& IS_AWAKE( rch )
					// ch 也需 cooperate rch , 能指定配合者 2021/11/23
					//&& is_same_group( ch, rch )
					//&& rch->pcdata->autocombat == AUTOC_COOPERATE)
					&& ( ch->in_room == rch->in_room )
					&& ( ch->cooperating == rch ) )
				{
					swd = rch;
					break;
				}
			}
				/*
					幻術劍修改為指定配合角色, 故移除沒有配合角色的情況 2021/11/23

				if( !rch ){
					for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room ){
						if( !IS_NPC( rch )
							&& rch != ch
							&& rch->class == SWORDSMAN
							&& IS_AWAKE( rch ) 
							&& is_same_group( ch, rch ) )
						{
							swd = rch;
							break;
						}
					}
				}
				*/
		}else{
			send_to_char("必須由幻法師發動！\n\r", ch);
			return;
		}
	}

	if( !swd) 
	{
		send_to_char("你找不到人配合你。\n\r", ch );
		return;
	}

    if( !IS_NPC(ch ) )
    {
        if( swd->position <= POS_RESTING )
        {
            send_to_char("你的同伴無法配合你施展法術。\n\r", ch);
            return;
        }
        if( swd->wait || number_percent() > get_skill_percent(swd, gsn_virtual_swords))
        {
            send_to_char("你的同伴無法配合你施展法術。\n\r", ch);
            return;
        }
        if( swd->move < 60 ) 
        {
            send_to_char("你的同伴無法配合你施展法術。\n\r", ch);
            return;
        }
        ch->mana += sklv * 6 + (sklv - 3) * 2;
    }

    if( sor->pp >= swd->pp ) 
        high_pp = sor;
    else 
        high_pp = swd;
    
    if( get_curr_wis(sor) >= get_curr_wis(swd)) 
        high_wis = sor;
    else 
        high_wis = swd;
    
    if( get_curr_int(sor) >= get_curr_int(swd)) 
        high_int = sor;
    else 
        high_int = swd;
    

    rsklv = get_skill_level(rch, sn);
    if(sklv > rsklv) 
        nVirChance = sklv * 6 + rsklv * 3 + UMIN(40 + sklv * 5, high_pp->pp) + get_curr_int(high_int);
    else
        nVirChance = rsklv * 6 + sklv * 3 + UMIN(40 + sklv * 5, high_pp->pp) + get_curr_int(high_int);
    lv = ( sor->level + swd->level + rsklv + sklv ) / 2;

    if( ch != rch ) 
    {
        act("你集中精神，為$N的劍上附加魔力！", sor, NULL, swd, TO_CHAR);
        act("你感到你的劍充滿了強大的魔力！", sor, NULL, swd, TO_VICT);
        act("$n喃喃念著咒文，只見$N的劍上逐漸發出耀眼的光芒！", sor, NULL, swd, TO_NOTVICT);
        act("$N長嘯一聲，凜烈的劍氣隨之而發！", sor, NULL, swd, TO_CHAR);
        act("你長嘯一聲，兵器一揮，以劍氣逼出劍上的魔力！", sor, NULL, swd, TO_VICT);
        act("$N長嘯一聲，揮出數道凜烈的劍氣！", sor, NULL, swd, TO_NOTVICT);
    }

    spell_faerie_fog(sn, level, high_pp, high_pp);
    
    if( lv < 38 ) 
    {
        if( !IS_NPC(swd))
             swd->move -= 60;
             
        for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room )
        {
            if( is_same_group(ch, rch) ) 
                continue;
                
            if( !IS_NPC(rch) && IS_SET( rch->act, PLR_WIZINVIS))
                continue;
                
            if( is_safe(sor, rch) || is_safe(swd, rch)) 
                continue;
            
            nWillFort = GetWillFortressEffect(rch);
            if(number_percent() < nVirChance - get_curr_wis(rch) + nWillFort){
                spell_poison( sn, level, high_wis, rch);
                spell_chill_touch(sn, level, high_int, rch);
                spell_faerie_fire(sn, level, sor, rch);
            }else if(nWillFort < 0){
                send_to_char(HIC"心靈的障壁使你免於幻術的侵擾"NOR"\n\r", rch);
						}
            stop_fighting(ch, FALSE);
            set_fighting(ch, rch);
        }
    }
    
    if( lv >= 39 && swd->move > 60 && sor->mana > sormana) 
    {
        act("$N長嘯一聲，凜烈的劍氣隨之而發！", sor, NULL, swd, TO_CHAR);
        act("你長嘯一聲，兵器一揮，以劍氣逼出劍上的魔力！", sor, NULL, swd, TO_VICT);
        act("$N長嘯一聲，揮出數道凜烈的劍氣！", sor, NULL, swd, TO_NOTVICT);
        
        if( !IS_NPC(swd))
            swd->move -= 60;
            
        if( !IS_NPC(sor))
            sor->mana -= sormana;
            
        for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room )
        {
            if( is_same_group(ch, rch) ) 
                continue;
                
            if( !IS_NPC(rch) && IS_SET( rch->act, PLR_WIZINVIS))
                continue;
                
            if( is_safe(sor, rch) || is_safe(swd, rch)) 
                continue;
                
            nWillFort = GetWillFortressEffect(rch);
            if(number_percent() < nVirChance - get_curr_wis(rch) + nWillFort){
                spell_seal_magic(sn, level, high_wis, rch);
                spell_weaken( sn, level, high_pp, rch);
                spell_curse( sn, level, high_wis, rch);
            }else if(nWillFort < 0){
                send_to_char(HIC"心靈的障壁使你免於幻術的詛咒"NOR"\n\r", rch);
						}
        }
    }
    
    if( lv >= 41 && swd->move > 60 && sor->mana > sormana) 
    {
        act("$N長嘯一聲，凜烈的劍氣隨之而發！", sor, NULL, swd, TO_CHAR);
        act("你長嘯一聲，兵器一揮，以劍氣逼出劍上的魔力！", sor, NULL, swd, TO_VICT);
        act("$N長嘯一聲，揮出數道凜烈的劍氣！", sor, NULL, swd, TO_NOTVICT);
        
        if( !IS_NPC(swd))
            swd->move -= 60;
            
        if( !IS_NPC(sor))
            sor->mana -= sormana;
            
        for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room )
        {
            if( is_same_group(ch, rch) ) 
                continue;
                
            if( !IS_NPC(rch) && IS_SET( rch->act, PLR_WIZINVIS))
                continue;
                
            if( is_safe(sor, rch) || is_safe(swd, rch)) 
                continue;
                
            nWillFort = GetWillFortressEffect(rch);
            if(number_percent() < nVirChance - get_curr_wis(rch) + nWillFort){
                spell_paralyze(sn, level, high_pp, rch);
                spell_blindness( sn, level, high_wis, rch);
            }else if(nWillFort < 0){
               send_to_char(HIC"心靈的障壁使你免於受到幻術的癱瘓"NOR"\n\r", rch);	
						}
        }
    }
    if( lv >= 43 && swd->move > 60 && sor->mana > sormana) 
    {
       act("$N長嘯一聲，凜烈的劍氣隨之而發！", sor, NULL, swd, TO_CHAR);
       act("你長嘯一聲，兵器一揮，以劍氣逼出劍上的魔力！", sor, NULL, swd, TO_VICT);
       act("$N長嘯一聲，揮出數道凜烈的劍氣！", sor, NULL, swd, TO_NOTVICT);
       
       if( !IS_NPC(swd))
           swd->move -= 60;
           
       if( !IS_NPC(sor))
           sor->mana -= sormana;
           
       for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room )
       {
           if( is_same_group(ch, rch) ) 
               continue;
               
           if( !IS_NPC(rch) && IS_SET( rch->act, PLR_WIZINVIS))
               continue;
               
           if( is_safe(sor, rch) || is_safe(swd, rch)) 
               continue;
               
           nWillFort = GetWillFortressEffect(rch);
           if(number_percent() < nVirChance - get_curr_wis(rch) + nWillFort){
               spell_mute( skill_lookup("mute"), level, high_wis, rch);
           }else if(nWillFort < 0){
               send_to_char(HIC"心靈的障壁使你免於受到幻術的禁咒"NOR"\n\r", rch);
					 }
       }
       spell_sun_flash( sn, level, high_wis, rch);
    }
    WAIT_MAGIC_STATE(sor, skill_table[sn].beats);
    WAIT_MAGIC_STATE(swd, skill_table[sn].beats);
     
    return;
}

// curse modified by jye 6.06.2005
void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int sklv = get_skill_level(ch,sn);
  int chance = 50;
  int hreffect;
  int svseffect;
  int weakness = weakness_damage( victim, sn );
  
  if(IsPK(ch, victim)) chance /= 2;
  
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
		svseffect = 2 + UMAX( 0, obj_lv - 40 ) + sklv * (sklv + 4) / 3;
	}else{
		svseffect = 2 + UMAX( 0, level - 36 ) + sklv * (sklv + 4) / 3;
	}
  if(IS_NPC(ch)) svseffect /= 2;
//  if( sklv > 4 )
//      svseffect += 2;
      
//  if( sklv > 5 )
//      svseffect += 3;
    
  chance = chance * int_effect( ch );

  if( sklv > 0 )
    chance = chance * ( 10 + sklv) / 10 ;
    
  if( sklv > 3 )
    chance = chance * 1.1;

  chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_DARK)) * 0.01;
  chance = UMIN(chance,75 + sklv * 4);
  chance = chance * weakness_percent( victim, sn ) * 0.01;
  
  if( ch->level < 30)
      chance += 20;

  if( IS_SET( victim->special, NO_CURSE ) || number_percent() > chance )
  {
      send_to_char( "你失敗了.\n\r", ch );
      return;
  }
  
  if( IS_AFFECTED( victim, AFF_CURSE )) 
  {
      if( is_affected( victim, sn)) 
      {
          if( affected_value(victim,sn,APPLY_SAVING_SPELL) <= svseffect )
              affect_strip( victim, sn );
          else
              return; 
      }
      else
          return; 
  }

  af.type  = sn;
  af.duration  = ( 10 + sklv *2 ) * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) * 0.01;
  if(IsPK(ch, victim)) af.duration /= 4;

  af.bitvector = AFF_CURSE; 
  if( sklv > 3 ) {
    hreffect     = - sklv * sklv;
    af.location  = APPLY_HITROLL;
    af.modifier  = hreffect * weakness * 0.01;
    if(IS_NPC(ch)) af.modifier /= 2;
    affect_to_char( victim, &af );
  }

  if( sklv > 4 ) {
    af.modifier  = -((sklv - 4) * 10 + (sklv - 5) * 5) * 2;
    if(IS_NPC(ch)) af.modifier /= 2;
    af.location  = APPLY_RMAGIC;
    af.modifier  = af.modifier * weakness * 0.01;
    affect_to_char( victim, &af );
  }

  af.location  = APPLY_SAVING_SPELL;
  af.modifier  = svseffect * weakness * 0.01;
  affect_to_char( victim, &af );

  af.location  = APPLY_SAVING_ELEMENT;
  af.modifier  = svseffect * weakness * 0.01;
  affect_to_char( victim, &af );

  act( HIB "一抹黑暗的烏影無聲無息地依附在"NOR"$N"HIB"背後！"NOR"", ch, NULL, victim, TO_CHAR );
  act( HIB "你心中突然閃過一絲不安，彷彿有某種東西在你身旁......"NOR"" , ch, NULL, victim, TO_VICT);
  act( "$N"HIB"的背後隱約浮現出一團不明黑霧。"NOR"", ch, NULL, victim, TO_NOTVICT );

  mprog_hurt_trigger(victim, ch, sn);
  
  return;
}

// tardy modified by jye 6.06.2005
void spell_tardy( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA  *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
    
  int sklv = get_skill_level(ch,sn);
  int chance = 50;
  int effect;
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
		effect   = obj_lv + get_curr_wis(ch) * ( 3 + 2 * sklv);
	}else{
		effect   = level + get_curr_wis(ch) * ( 3 + 2 * sklv);
	}

	if(IsPK(ch, victim)) chance /= 2;
  
	if( sklv > 4 )
		effect += 50;
      
	if( sklv > 5 )
		effect += 50;

	chance = chance * wis_effect( ch );

	if( sklv > 0 )
		chance = chance * ( 10 + sklv) / 10 ;
    
	if( sklv > 3 )
		chance = chance * 1.1;

	chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) * 0.01;
	chance = UMIN(chance, 75 + sklv * 4);
	chance = chance * weakness_percent( victim, sn ) * 0.01;
  
	if( !IS_NPC(ch) && ch->level < 31 )
		chance += 25;
    
	if( IS_SET(victim->special, NO_TARDY) || number_percent() > chance )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}
  
	if( sklv > 3 || !IS_AFFECTED( victim, AFF_TARDY ))
		WAIT_MAGIC_STATE( victim, PULSE_VIOLENCE);
      
	if( IS_AFFECTED( victim, AFF_TARDY )) 
	{
		if( is_affected( victim, skill_lookup("tardy"))) 
		{
			if( affected_value(victim,sn,APPLY_DODGE) < effect )
				affect_strip( victim, skill_lookup("tardy"));
			else{
				act( "$N身上已經遲緩術效果.", ch, NULL, victim, TO_CHAR );
				return;
			}
		}
		else{
			act( "$N身上已經遲緩術效果.", ch, NULL, victim, TO_CHAR );
			return;
		}
	}
	
	effect += UMAX( 0 , GET_DODGE( victim )/(sklv - 11) );

	af.type  = sn;
	//af.duration  = ( 10 + sklv *2 ) * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) * 0.01;
	//if(IsPK(ch, victim)) af.duration /= 4;
	// 修改作用時間 2020/11/06
	if(affected_value(victim,sn,APPLY_DODGE) < 0){
		af.duration  = 0;
	}else{
		af.duration  = 3;
	}
	af.location  = APPLY_DODGE;
	af.modifier  = effect * weakness_damage( victim, sn ) * 0.01;
	af.bitvector = AFF_TARDY;
	affect_join( victim, &af );

	act( HIY "周圍的空氣在你的魔力影響下，形成了淡黃色的風之鎖鍊，縛住了"NOR"$N"HIY"！"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIY "大量的風之鎖鍊從"NOR"$n"HIY"身旁往你火速延伸過來，縛住了你！"NOR"" , ch, NULL, victim, TO_VICT);
	act( HIY "淡黃色的風之鎖鍊憑空出現在"NOR"$n"HIY"身旁，縛住了"NOR"$N"HIY"！"NOR"", ch, NULL, victim, TO_NOTVICT );    

	mprog_hurt_trigger(victim, ch, sn);

    return;
}


// mute modified by jye 6.06.2005
void spell_mute( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int sklv = get_skill_level(ch,sn);
  int chance = 30;
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

  if(IsPK(ch, victim)) chance /= 2;
  chance = chance * int_effect( ch );

  if( sklv > 0 )
      chance = chance * ( 10 + sklv) / 10 ;
    
  if( sklv > 4 )
      chance = chance * 1.1;

  chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) * 0.01;
  chance = UMIN(chance,50+(sklv*4));
  chance = chance * weakness_percent( victim, sn ) * 0.01;
  
  if( !IS_NPC(ch) && ch->level < 31 )
       chance += 40;

  if( IS_SET(victim->special, NO_MUTE) || number_percent() > chance )
  {
      send_to_char( "你失敗了.\n\r", ch );
      return;
  }

  if( !IS_NPC(ch) && ch->level < 31 )
       chance += 40;
    
  if( IS_AFFECTED( victim, AFF_MUTE )) 
  {
      if( is_affected( victim, sn))
          affect_strip( victim, sn);
      else
          return; 
  }
        
  af.type  = sn;
  af.duration  = ( 10 + sklv *2 ) * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) * 0.02;
  if(IsPK(ch, victim)) af.duration /= 4;
  af.location  = 0;
  af.modifier  = 0;
  af.bitvector = AFF_MUTE;
  affect_to_char( victim, &af );
  
  act( HIG "一股詭譎的氣體逐漸散開，使得"NOR"$N"HIG"周圍的聲音逐漸消失！"NOR"", ch, NULL, victim, TO_CHAR    );
  act( HIG "周圍的風之精靈慢慢陷入了沉眠，你發現$E的聲音逐漸被封鎖了！"NOR"",  ch, NULL, victim, TO_VICT    );
  act( "$n"HIG"施法釋放出詭譎的不明氣體，影響到"NOR"$N"HIG"身旁的空氣，你逐漸聽不到$E的聲音了！"NOR"", ch, NULL, victim, TO_NOTVICT );
  mprog_hurt_trigger(victim, ch, sn);

  return;
}
              
// poison modified by jye 6.06.2005
void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	int sklv = get_skill_level(ch,sn);
	int nVenomSklv = get_skill_level(ch, skill_lookup("venomous strike"));
	int chance = 50;

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

	if(IsPK(ch, victim)) chance /= 2;
	chance = chance * wis_effect( ch );
	if(sklv < nVenomSklv) sklv = nVenomSklv;
  
	if( sklv > 0 )
		chance = chance * ( 10 + sklv) / 10 ;
    
	if( sklv > 3 )
		chance = chance * 1.1;
      
	chance = chance * weakness_percent( victim, sn ) * 0.01;
	chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) * 0.01;

	if( !IS_NPC(ch) && ch->level < 31 )
		chance += 25;
/*   將 chance , no_poison  分開判定 , 移除 invoke 不中毒判定 2021/11/04
	if ( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) 
		|| ( IS_SET( victim->special, NO_POISON ))
		|| ( victim->race == race_lookup( "Vampire" )) 
		|| ( victim->race == race_lookup( "Undead" ))
		|| ( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 3 )
		|| ( number_percent() > chance ))
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}
*/

	// chance 判定
	if( number_percent() > chance ){
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	// 判定 no_poison
	if(    IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON )
		|| IS_SET( victim->special, NO_POISON ) )
	{
		// 判定 no_poison 的 race_undead 上 aff_poison 數值 2021/11/04
		// 加入 aff_ghoul 設定 2022/01/16
		if( IS_SET( race_table[victim->race].race_abilities , RACE_UNDEAD ) || IS_AFFECTED( victim, AFF_GHOUL ) ){
			af.type  = gsn_poison;
			af.duration = UMAX( 0 , 1 + sklv - get_curr_con(victim)/5);
			if(IsPK(ch, victim)) af.duration /= 4;
			af.bitvector = AFF_POISON;
			if( affected_value(victim, gsn_poison, APPLY_SAVING_POISON) < (14 + sklv*6)*10 )
			{
				af.location = APPLY_SAVING_POISON;
				af.modifier = ((14 + sklv*6) * 25 / get_curr_con(victim)) * weakness_damage( victim, gsn_poison ) * 0.01;
				affect_join( victim, &af );
			}
			act( ""HIG"毒素侵入"NOR"$N"HIG"的體內，但似乎不對他造成任何影響。"NOR"", ch, NULL, victim, TO_CHAR );
			act( ""HIG"毒素侵入你的體內，但不對你造成任何影響。"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIG"的身體被毒素侵入，但似乎不對他造成任何影響。"NOR"", ch, NULL, victim, TO_NOTVICT );
		}else{
			send_to_char( "你失敗了.\n\r", ch );
			return;
		}
	}else{
		af.type  = gsn_poison;
		af.duration = UMAX( 0 , 1 + sklv - get_curr_con(victim)/5);
		if(IsPK(ch, victim)) af.duration /= 4;
		af.bitvector = AFF_POISON;

	/*  移除 con 影響 2021/11/04
		if( affected_value(victim, gsn_poison, APPLY_CON) > -8)
		{
			af.location = APPLY_CON;
			af.modifier = -2 * weakness_damage( victim, gsn_poison ) * 0.01;
			affect_join( victim, &af );
		}
	*/

		if( affected_value(victim, gsn_poison, APPLY_SAVING_POISON) < (14 + sklv*6)*10 )
		{
			af.location = APPLY_SAVING_POISON;
			af.modifier = ((14 + sklv*6) * 25 / get_curr_con(victim)) * weakness_damage( victim, gsn_poison ) * 0.01;
			affect_join( victim, &af );
		}

		act( "$N"HIG"臉色略為泛青，毒素已經侵入了他的體內！"NOR"", ch, NULL, victim, TO_CHAR );
		act( HIG "噁心感與刺痛感同時襲來，你覺得相當地難受！"NOR"" , ch, NULL, victim, TO_VICT);
		act( "$N"HIG"的身體顫抖了一下，表情變得有些苦澀！"NOR"", ch, NULL, victim, TO_NOTVICT );

		mprog_hurt_trigger(victim, ch, sn);
	}

	return;
}

// disease modified by jye 6.06.2005
void spell_disease( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA  *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int sklv   = get_skill_level(ch,sn);
  int chance = new_saves_spell( ch->pp, victim, MAGIC_COLD ) + ( WIS_DIFF( ch, victim ) * 4 + sklv*2 );
  chance     = chance * weakness_percent(victim, sn) / 92;
  if(IsPK(ch, victim)) chance /= 2;
  if( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) 
      || IS_SET( victim->special, NO_POISON ) )
      return;

//  if( new_saves_spell( ch->pp, victim, MAGIC_COLD ) + ( WIS_DIFF( ch, victim ) * 4 + sklv*2 )
//      < (number_percent() * 92 / weakness_percent(victim, sn)))
  if(chance < number_percent())
      return;

  af.type  = sn;
  af.duration  = level /2 + INT_DIFF( ch, victim ) + get_skill_level(ch,sn);
  if(IsPK(ch, victim)) af.duration /= 4;
  af.location  = APPLY_STR;
  af.modifier  = -1;
  af.bitvector = AFF_DISEASE;
  affect_join( victim, &af );

  af.location  = APPLY_CON;
  af.modifier  = -1;
  affect_join( victim, &af );

  af.location  = APPLY_DEX;
  af.modifier  = -1;
  affect_join( victim, &af );

  af.location  = APPLY_INT;
  af.modifier  = -2;
  affect_join( victim, &af );
 
  af.location  = APPLY_WIS;
  af.modifier  = -1;
  affect_join( victim, &af );
  if( ch != victim )
      send_to_char( "好的。\n\r", ch );
      
  send_to_char( "你身體感到不適.\n\r", victim );
  return;
}

// fear modified by jye 6.06.2005
void spell_fear( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA  *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  int sklv   = get_skill_level(ch,sn);
  int chance = new_saves_spell( ch->pp, victim, MAGIC_COLD ) + ( WIS_DIFF( ch, victim ) * 2 );
  chance = chance * weakness_percent(victim, sn) / 92;
  if( IS_SET( victim->special, NO_FEAR ) )
      return;
  if( IsPK(ch, victim)) chance /= 2;
  if( chance < number_percent() ) return;
  if( is_affected(victim, gsn_will_fortress)
      && number_percent() < -(affected_value(victim,gsn_will_fortress, APPLY_SAVING_ILLUSION) - sklv * 4)){
      send_to_char( HIW"你強大的意志力，讓你感受不到恐懼！"NOR"\n\r", victim);
      return;
  }
//  if( new_saves_spell( ch->pp, victim, MAGIC_COLD ) + ( WIS_DIFF( ch, victim ) * 4 + sklv * 2)
//      < (number_percent() * 92 / weakness_percent(victim, sn)))
//      return;

  af.type  = sn;
  af.duration  = level /2 + INT_DIFF( ch, victim )+sklv;
  if(IsPK(ch, victim)) af.duration /= 4;
  af.location  = APPLY_INT;
  af.modifier  = -2;
  af.bitvector = AFF_FEAR;
  affect_join( victim, &af );

  if( ch != victim )
      send_to_char( "好的。\n\r", ch );
  
  send_to_char( "你開始莫名地恐懼了起來.\n\r", victim );
  return;
}

// sleep modified by jye 6.06.2005
void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA  *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
         
  int sklv   = get_skill_level(ch,sn);  
  int chance = saves_spell( ch->pp, victim )+sklv + sklv/2;
  chance     = chance * weakness_percent(victim, sn) / 100;
  if(IsPK(ch, victim)) chance /= 2;
	
  if( IS_AFFECTED( victim, AFF_SLEEP )
      || ch->level < victim->level - sklv
//      || (saves_spell( ch->pp, victim )+sklv + sklv/2 < (number_percent() * 100 / weakness_percent(victim, sn)))
      || chance < number_percent()
      || victim->race == race_lookup( "Vampire" ) 
      || IS_SET( victim->special, NO_SLEEP ) 
      || IS_IMMORTAL(victim))
  {
      send_to_char( "你失敗了.\n\r", ch );
      return;
  }

  af.type      = sn;
  af.duration  = ch->level/8 + INT_DIFF( ch, victim ) + sklv;
  if(IsPK(ch, victim)) af.duration /= 8;
  af.location  = APPLY_NONE;
  af.modifier  = 0;
  af.bitvector = AFF_SLEEP;
  affect_join( victim, &af );

  if( IS_AWAKE( victim ) )
  {
      send_to_char( "一股睡意襲來..... zzzzzz.\n\r", victim );
      
      if( victim->position == POS_FIGHTING )
          stop_fighting( victim, TRUE );
          
      do_sleep( victim, "" );
  }

  WAIT_MAGIC_STATE(ch, 12);
  return;
}

// sleep modified by jye 6.06.2005
void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int sklv = get_skill_level(ch,sn);
  int chance = 50;
  int intWeaken = skill_lookup("weaken");

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

	if(IsPK(ch, victim)) chance /= 2;
	chance = chance * int_effect( ch );

	if( sklv > 0 )
		chance = chance * ( 10 + sklv) / 10 ;
    
	if( sklv > 3 )
		chance = chance * 1.1;

	chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) * 0.01;
	chance = UMIN(chance,75 + sklv*4);
	chance = chance * weakness_percent( victim, sn ) * 0.01;
  
	if( !IS_NPC(ch) && ch->level < 31 )
		chance += 25;

	if(( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 2 ) || number_percent() > chance )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	if( IS_AFFECTED( victim, AFF_WEAKEN )) 
	{
		if( is_affected( victim, intWeaken)) 
		{
			if( sklv > 2
				&& ( affected_value(victim,sn,APPLY_DAMROLL)
					> (get_damroll(victim, WEAR_WIELD)/(sklv - 11))
					* weakness_damage( victim, sn ) * 0.01 ) )
			{
				affect_strip( victim, intWeaken);
			}
			else if( affected_value(victim,sn,APPLY_STR) > -1 - sklv/3 )
			{
				affect_strip( victim, intWeaken);
			}
			else
			{
				act( "$N身上已經有衰弱術效果."NOR"", ch, NULL, victim, TO_CHAR );
				return;
			}
		}
		else
		{
			act( "$N身上已經有衰弱術效果."NOR"", ch, NULL, victim, TO_CHAR );
			return;
		}
	}
      
	af.type  = gsn_weaken;
	//af.duration  = 1;
	//if(IsPK(ch, victim)) af.duration /= 4;
	// 修改作用時間 2020/11/06
	if(affected_value(victim,sn,APPLY_STR) < 0){
		af.duration  = 0;
	}else{
		af.duration  = 3;
	}
	af.location  = APPLY_STR;
	af.modifier  = (-1 - sklv/3) * weakness_damage( victim, sn ) * 0.01;
	af.bitvector = AFF_WEAKEN;
	affect_to_char( victim, &af );
  
 /* if( sklv > 3 ) {
      af.type  = gsn_weaken;
      af.duration  = ( 10 + sklv *2 ) * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) * 0.01;
      af.location  = APPLY_ADAMAGE;
      af.modifier  = (-8 - (sklv-5) * 12)* weakness_damage( victim, sn ) * 0.01;
      af.bitvector = AFF_WEAKEN;
      affect_to_char( victim, &af );
  }*/


	// 新增降低 dr 2020/11/06

	if( sklv > 2 ){
		af.type  = gsn_weaken;
		if(affected_value(victim,sn,APPLY_DAMROLL) < 0){
			af.duration  = 0;
		}else{
			af.duration  = 3;
		}
		af.location  = APPLY_DAMROLL;
		af.modifier  = (get_damroll(victim, WEAR_WIELD)/(sklv - 11)) * weakness_damage( victim, sn ) * 0.01;
		af.bitvector = AFF_WEAKEN;
		affect_to_char( victim, &af );
	}

  
	if( sklv > 3 ) {
		af.type  = gsn_weaken;
		//af.duration  = 1;
		// 修改作用時間 2020/11/06
		if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
			af.duration  = 0;
		}else{
			af.duration  = 3;
		}
		af.location  = APPLY_ADAMAGE;
		af.modifier  = - (10 + sklv * 15) * weakness_damage( victim, sn ) * 0.01;
		af.bitvector = AFF_WEAKEN;
		affect_to_char( victim, &af );
	}

	act( "$N"HIC"皮膚冒出絲絲水氣，他看起來似乎變得比剛才來得虛弱！"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIC "你感到汗流浹背，身體有點使不上力氣。"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$N"HIC"看起來汗流不止，臉色也變得蒼白了些。"NOR"", ch, NULL, victim, TO_NOTVICT );
	mprog_hurt_trigger(victim, ch, sn);
  
	return;
}

// lifeshare modified by jye 6.06.2005
void spell_lifeshare( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch,sn);

	if( IS_NPC(ch) && !is_real_npc(ch) )
	{
		if(!IsSorGateMobEarth(ch->pIndexData->vnum)){
			send_to_char( "你失敗了。\n\r", ch);
			return;
		}
	}

	if( ( !is_same_group( ch, victim) ) ){
		send_to_char("他不在你的隊伍中。\n\r", ch);
		return;
	}

	if( victim == ch ){
		if( ch->linking )
			stop_linking( ch , ch->linking );
		return;
	}

	if( victim->linking ){
		if( victim == ch->linking ){
			if( victim == ch )
				stop_linking( ch , ch->linking );
			else
				send_to_char("你已經跟他有著靈魂聯繫了。\n\r", ch);
		}else
			send_to_char("他已經跟別人有著靈魂聯繫了。\n\r", ch);
		return;
	}

	if( ch->mana < (100 + ch->max_mana/10) ){
		send_to_char("你的魔力不足。\n\r", ch);
		return;
	}

	ch->mana -= (100 + ch->max_mana/10);

	if( ch->linking )
		stop_linking( ch , ch->linking );

	add_linking(ch, victim);

	act( HIU "你與"NOR"$N"HIU"建立起靈魂的聯繫，共同享有彼此的生命。"NOR"", ch, NULL, victim, TO_CHAR );
	act( "$n"HIU"與你建立起靈魂的聯繫，共同享有彼此的生命。"NOR"" , ch, NULL, victim, TO_VICT);

	af.type  = sn;
	af.duration  = 10;
	af.location  = 0;
	af.modifier  = 0;
	af.bitvector = 0;
	affect_to_char( ch, &af );
	affect_to_char( victim, &af );
/*
	修改 lifeshare 效果 2021/11/20

  int total=0, members=0, exceed=0;
  int avg = 0;
  int max_total = 0;
  CHAR_DATA *vch;
  if( IS_NPC(ch) ) return;
  if( ch->deleted || !ch->in_room ) return;
  int sklv = get_skill_level(ch,sn);

  for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
  {
       if( is_same_group(ch, vch) )
       {
           ++ members;
           total += vch->hit;
           max_total += vch->max_hit;
       }
  }

  if( members < 2 ) 
  {
      send_to_char("你不需要把生命分給自己。\n\r", ch);
      return;
  }
    
  if( total == max_total ) 
  {
      send_to_char("大家都精力充足，不需要分享了。\n\r", ch);
      return;
  }


  total += ((max_total - total) * sklv ) * 0.06;
  avg = (total / members);

  for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
  {
       if( is_same_group(ch, vch) )
       {
           vch->hit = URANGE(0, avg, vch->max_hit);
           
           if( vch->max_hit < avg ) exceed += ( avg - vch->max_hit);
               update_pos(vch);
       }
  }

  if( exceed ) 
  {
      exceed /= members;
      
      for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
      {
           if( is_same_group(ch, vch) && vch->hit < vch->max_hit)
           {
               vch->hit +=  exceed;
               update_pos(vch);
           }
      }
  }

  send_to_char("好的.\n\r", ch);
  WAIT_MAGIC_STATE(ch, 16);
*/
	return;
}

// crushing hand modified by jye 6.06.2005 
void spell_crushing_hand( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA*) vo;
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

	if( IS_SET( victim->special, NO_DM ) ) {
		act("$n釋放出強大的魔法能量，但$N似乎絲毫不受任何影響！" , ch, NULL, victim, TO_NOTVICT);
		act("你釋放出強大的魔法能量，但$N似乎絲毫不受任何影響！" , ch, NULL, victim, TO_CHAR);
		return;
	}

	if (!IS_NPC(victim)) {
		int nInvokeSklv = get_skill_level(victim, gsn_invoke);
		if (nInvokeSklv >= 6) {
			if (is_affected(victim, gsn_invoke)) {
				send_to_char(HIC"你因為神靈的保護免於受到粉碎術的影響"NOR".\n\r", victim);
				act("$n釋放出強大的魔法能量，但$N似乎絲毫不受任何影響！" , ch, NULL, victim, TO_NOTVICT);
				act("你釋放出強大的魔法能量，但$N似乎絲毫不受任何影響！" , ch, NULL, victim, TO_CHAR);
				return;
			}
		}
	}

	//ch->mana += sklv * 15 + (sklv - 3) * 10;

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{ 
		send_to_char("這裡禁止戰鬥. \n\r",ch);
		return;
	}

	affect_strip( victim, skill_lookup("bless"));
	affect_strip( victim, skill_lookup("barrier"));
	affect_strip( victim, skill_lookup("prot. from fire"));
	affect_strip( victim, skill_lookup("prot. from cold"));
	affect_strip( victim, skill_lookup("prot. from wind"));
	affect_strip( victim, skill_lookup("prot. from earth"));
	affect_strip( victim, skill_lookup("prot. from lightning"));
	affect_strip( victim, skill_lookup("day of power"));
	affect_strip( victim, skill_lookup("day of protection"));
	affect_strip( victim, skill_lookup("day of brightness"));
	affect_strip( victim, skill_lookup("holy crown"));
	affect_strip( victim, gsn_mass_invis);
	affect_strip( victim, gsn_invis );
	affect_strip( victim, skill_lookup("detect invis"));
	affect_strip( victim, skill_lookup("detect hidden"));
	affect_strip( victim, skill_lookup("infravision"));
	affect_strip( victim, gsn_invoke);
	affect_strip( victim, skill_lookup("armor"));
	affect_strip( victim, skill_lookup("stone skin"));
	affect_strip( victim, skill_lookup("shield"));
	affect_strip( victim, skill_lookup("fly"));
	REMOVE_BIT(victim->affected_by, AFF_FLYING);
	affect_strip( victim, gsn_giant);
	affect_strip( victim, skill_lookup("combat mind"));
	affect_strip( victim, gsn_adrenaline);
	affect_strip( victim, gsn_mental_empower );

	if( sklv > 1 )
	{
		affect_strip( victim, gsn_mystic_mirror);
		REMOVE_BIT(victim->affected_by, AFF_MYSTIC_MIRROR);
		affect_strip( victim, skill_lookup("flaming armor"));
		REMOVE_BIT(victim->affected_by, AFF_FLAMING);
		affect_strip( victim, gsn_mana_shield );
		REMOVE_BIT(victim->affected_by, AFF_MANA_SHIELD);
		REMOVE_BIT(victim->affected_by, AFF_INFRARED);
	}

	if( sklv > 1 || !IS_SET(victim->special, ALY_INVIS ) )
		REMOVE_BIT(victim->affected_by, AFF_INVISIBLE);

	if( sklv > 2 && !IS_SET(victim->special, ALY_SAN ) )
	{
		affect_strip( victim, gsn_sanctuary );
		REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
		affect_strip( victim, skill_lookup("protection"));
		REMOVE_BIT(victim->affected_by, AFF_PROTECT);
	}

	if( sklv > 4 || !IS_SET(victim->special, ALY_DH ) )
		REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);
	  
	if( sklv > 5 || !IS_SET(victim->special, ALY_DI ) )
		REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);


	//affect_strip( victim, gsn_hasten );
	//if( sklv > 2 ) affect_strip( victim, skill_lookup("divine light"));
	// mystic armor 更新 , 作成不一定能拔除 2020/10/27
	//affect_strip( victim, gsn_mystic_armor);
	//REMOVE_BIT(victim->affected_by, AFF_MYSTIC_ARMOR);
	  
	act("$n釋放出強大的魔法能量，粉碎了$N身上的護身法術！" , ch, NULL, victim, TO_NOTVICT);
	act("$n釋放出強大的魔法能量，粉碎了你身上的護身法術！" , ch, NULL, victim, TO_VICT);
	act("你釋放出強大的魔法能量，粉碎了$N身上的護身法術！" , ch, NULL, victim, TO_CHAR);
	mprog_hurt_trigger(victim, ch, sn);
	WAIT_MAGIC_STATE(ch, 20);
}

// faerie fire modified by jye 6.06.2005
void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
  
	int sklv = get_skill_level(ch,sn);
	int chance = 50;
	int effect;
	int intfaerie_fire = skill_lookup("faerie fire");
  
	if(IsPK(ch, victim)) chance /= 2;
  
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
		if( victim->armor < 0 && ch->pp > 0){
			effect = 4 * (obj_lv + sklv) - (victim->armor * ch->pp * sklv)/2200;
		}else{
			effect = 4 * (obj_lv + sklv);
		}
	}else{
		if( victim->armor < 0 && ch->pp > 0){
			effect = 4 * (level + sklv) - (victim->armor * ch->pp * sklv)/2200;
		}else{
			effect = 4 * (level + sklv);
		}
	}
  
	//effect = 4 * (level) + INT_DIFF( ch, victim ) * 5 + sklv*20;
  
	/*if( sklv > 4 )
		effect += 40;
  
	if( sklv > 5 )
		effect += 40;
  
	if( !IS_NPC(ch) )
		effect = effect * 1.5; */

	chance = chance * int_effect( ch );

	if( sklv > 0 )
		chance = chance * ( 10 + sklv) / 10 ;
    
	if( sklv > 3 )
		chance = chance * 1.1;

	chance = chance * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) * 0.01;
	chance = UMIN(chance,75 + sklv * 4);
	chance = chance * weakness_percent( victim, sn ) * 0.01;
  
	if( !IS_NPC(ch) && ch->level < 31 )
		chance += 25;

	if( IS_SET( victim->special, NO_FAEFIRE ) || number_percent() > chance )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	if( IS_AFFECTED( victim, AFF_FAERIE_FIRE )) 
	{
		if( is_affected( victim, intfaerie_fire)) 
		{
			if( affected_value(victim,sn,APPLY_AC) < effect )
				affect_strip( victim, intfaerie_fire);
			else
			{
				act( "$N身上已經有天使之火效果."NOR"", ch, NULL, victim, TO_CHAR );
				return;
			}
		}
		else
		{
			act( "$N身上已經有天使之火效果."NOR"", ch, NULL, victim, TO_CHAR );
			return;
		}
	}
	
	effect += UMAX( 0 , GET_AC( victim )/(sklv - 10) );
      
	af.type  = sn;
	//af.duration  = 1;
	//if(IsPK(ch, victim)) af.duration /= 4;
	// 修改作用時間 2020/11/06
	if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
		af.duration  = 0;
	}else{
		af.duration  = 3;
	}
	af.location  = APPLY_AC;
	af.modifier  = effect * weakness_damage( victim, sn ) * 0.01;
	af.bitvector = AFF_FAERIE_FIRE;
	affect_join( victim, &af );

	if( sklv > 3 && !IS_NPC(ch)) {
		af.type  = sn;
		//af.duration  = 1;
		//if(IsPK(ch, victim)) af.duration /= 4;
		// 修改作用時間 2020/11/06
		if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
			af.duration  = 0;
		}else{
			af.duration  = 3;
		}
		af.location  = APPLY_RDAMAGE;
		af.modifier  = (-8 - (sklv)*7)* weakness_damage( victim, sn ) * 0.01;
		af.bitvector = AFF_FAERIE_FIRE;
		affect_join( victim, &af );
	}else if( IS_NPC(ch)) {
		af.type  = sn;
		//af.duration  = 1;
		// 修改作用時間 2020/11/06
		if(affected_value(victim,sn,APPLY_ADAMAGE) < 0){
			af.duration  = 0;
		}else{
			af.duration  = 3;
		}
		af.location  = APPLY_RDAMAGE;
		af.modifier  = (-10 - (sklv)*15) * weakness_damage( victim, sn ) * 0.01;
		af.bitvector = AFF_FAERIE_FIRE;
		affect_join( victim, &af );
	}

	act( HIP"你喚來神秘的力量，一片詭異的紫霧圍繞在"NOR"$N"HIP"身旁！"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIP"$n"HIP"身旁冒出的怪異紫霧纏上了你！"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$n"HIP"召喚出暗紫色的霧氣，依附在"NOR"$N"HIP"的身上！"NOR"", ch, NULL, victim, TO_NOTVICT );    
     
	mprog_hurt_trigger(victim, ch, sn);
  
	return;
}


