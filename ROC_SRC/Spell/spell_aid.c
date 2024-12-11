//*****************************************************************
//*  PROGRAM ID  : spell_aid.c                                    *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940605 jye                                    *
//*  PURPOSE     :         �ק�spell_blindness���ܼƬۭ���0       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940606 jye                                    *
//*  PURPOSE     :         �վ�{������μW�[�iDebug��Function    *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940611 jacquex                                *
//*  PURPOSE     :         �ץ��{������A���Function Call        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940714 Razgriz                                *
//*  PURPOSE     :         �ק� spell_charm_person()�A            *
//*              :         �[�J charm_person �� qmark �B�z        *
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
  
  //modify by jye 0940605 �ק��ܼƬۭ���0
  chance *= weakness_percent( victim, sn ) * 0.01;

  if( !IS_NPC(ch) && ch->level < 31 )
      chance += 40;
  
  if ( IS_SET( victim->special, NO_BLIND ) 
       || IS_SET( race_table[ victim->race ].race_abilities, RACE_BEHOLDER ) 
       || ( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 2 )
       || number_percent() > chance )

  {                                     
    send_to_char( "�A���ѤF.\n\r", ch );
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

  act( HIR "���������V�Ʀ�ģ�ت������A�뤤�F"NOR"$N"HIR"�������I"NOR"", ch, NULL, victim, TO_CHAR );
  act( HIR "���e�{�L���`���G�������A�򱵦ӨӪ��N�O�L�ɪ��·t�I"NOR"" , ch, NULL, victim, TO_VICT);
  act( "$n"HIR"�⤤�g�X�������R���F"NOR"$N"HIR"�������I"NOR"", ch, NULL, victim, TO_NOTVICT );
  
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
		|| (IS_AFFECTED( victim, AFF_GHOUL ))  // ���ܬ������ͪ��ɵ��P�����t(undead) 2022/06/15
		|| ( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 3 )
		|| ( number_percent() > chance ))
	{
		send_to_char( "�A���ѤF.\n\r", ch );
		return;
	}

	/*  �P�w�קאּ, �ؼФw�g�Q�·� �� �ؼШ��W�a�� paralyze �� affect �h���|�Q���ЬI�k 2022/06/14
	
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

	// dc ���C�q�� sklv �v�T 2022/06/14
	DcReduce = 150 + sklv * 100;

	if( !IS_NPC(victim) ) DcReduce = DcReduce/2;

	af.type  = gsn_paralyze;
	af.duration  = 1;
	//if(IsPK(ch, victim)) af.duration /= 4;
	af.location  = APPLY_DODGE;
	af.modifier  = DcReduce * weakness_damage( victim, sn ) * 0.01;
	af.bitvector = AFF_PARALYZED;
	affect_to_char( victim, &af );

	act( HIG "�ݤ������r���������F"NOR"$N"HIG"�A�O�L�ʼu���o�I"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIG "�Aı�o�ۤv�������ܱo�������w�A�L�k�H�ߩұ��a���ʡI"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$N"HIG"���ʧ@�v����w�U�ӡA�̫ᳺ�M��������F�I"NOR"", ch, NULL, victim, TO_NOTVICT );

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
      send_to_char( "�A���ѤF.\n\r", ch );
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

  act( YEL "����j�a���F�ƨ������g���F�G�媺���a�A���F$N" YEL "�I"NOR"", ch, NULL, victim, TO_CHAR );
  act( YEL "�Ƥ��M�����a��¶�ۧA�A�W������r���I�J�A�����餧���I"NOR"" , ch, NULL, victim, TO_VICT);
  act( YEL "����j�a���F�ƨ������g���F�G�媺���a�A���F$N" YEL "�I"NOR"", ch, NULL, victim, TO_NOTVICT );
  
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
		 send_to_char( "�Aı�o�ۤv�٤����R�ۤv�ܡH\n\r", ch );
		 return;
	}

	if ( get_trust( ch ) >= L_APP && get_trust( ch ) < L_SEN )
	{
		send_to_char( "�H�A����O�ä��ݭn�y�b�O���ͪ��C\n\r",ch );
		sprintf( buf, "%s charming", ch->name );
		log_string( buf );
		return;
	}

	if( IS_IMMORTAL( victim ) )
	{
	  send_to_char( "�A����O�ä����H�y�b���e���ؼСC\n\r", ch );
	  sprintf( buf, "%s charming", ch->name );
	  log_string( buf );
	  return;
	}

	if( IsPK(ch, victim) ) 
	{
		if( !IS_SET( ch->act, PLR_REGISTER ) && (ch->in_room && !IS_SET(ch->in_room->room_flags, ROOM_PK))) 
		{
			 send_to_char("�A�èS���ѥ[ PK�I\n\r", ch);
			 return;
		}
		if( is_safe( ch, victim ) )
			return;
	}

	if ( IS_SET( victim->special, NO_CHARM ) || (( IS_NPC( victim ) && ( victim->pIndexData->pShop ))) ){
		// victim �S���b�԰��~�|��ؼЧ@ multi_hit 2022/12/31
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

	send_to_char( "�n���C\n\r", ch );
	act( "$n �ݰ_�ӯu�O�Ӧ��y�O�F�C", ch, NULL, victim, TO_VICT );
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
		send_to_char("�o�̸T��԰��C\n\r", ch);
		return ;
	}

	if( !ch->in_room || !ch->fighting)
		return;

	if( ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) )){
		send_to_char("�A���ѤF�C\n\r", ch);
		return;
	}

	if( IS_NPC(ch) ){
		swd = ch;
		sor = ch;
		rch = ch;
	}else{
		if( ch->class == SORCERER ){
			sor = ch;
			// �קאּ�P�w cooperator 2021/11/23
			//for( rch = ch->in_room->people ; rch ; rch = rch->next_in_room )
			for( rch = ch->cooperator ; rch ; rch = rch->next_in_coop )
			{
				if( !IS_NPC( rch )
					&& rch != ch
					&& rch->class == SWORDSMAN
					&& IS_AWAKE( rch )
					// ch �]�� cooperate rch , ����w�t�X�� 2021/11/23
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
					�۳N�C�קאּ���w�t�X����, �G�����S���t�X���⪺���p 2021/11/23

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
			send_to_char("�����Ѥ۪k�v�o�ʡI\n\r", ch);
			return;
		}
	}

	if( !swd) 
	{
		send_to_char("�A�䤣��H�t�X�A�C\n\r", ch );
		return;
	}

    if( !IS_NPC(ch ) )
    {
        if( swd->position <= POS_RESTING )
        {
            send_to_char("�A���P��L�k�t�X�A�I�i�k�N�C\n\r", ch);
            return;
        }
        if( swd->wait || number_percent() > get_skill_percent(swd, gsn_virtual_swords))
        {
            send_to_char("�A���P��L�k�t�X�A�I�i�k�N�C\n\r", ch);
            return;
        }
        if( swd->move < 60 ) 
        {
            send_to_char("�A���P��L�k�t�X�A�I�i�k�N�C\n\r", ch);
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
        act("�A�����믫�A��$N���C�W���[�]�O�I", sor, NULL, swd, TO_CHAR);
        act("�A�P��A���C�R���F�j�j���]�O�I", sor, NULL, swd, TO_VICT);
        act("$n�����۩G��A�u��$N���C�W�v���o�Xģ�������~�I", sor, NULL, swd, TO_NOTVICT);
        act("$N���S�@�n�A���P���C���H���ӵo�I", sor, NULL, swd, TO_CHAR);
        act("�A���S�@�n�A�L���@���A�H�C��G�X�C�W���]�O�I", sor, NULL, swd, TO_VICT);
        act("$N���S�@�n�A���X�ƹD���P���C��I", sor, NULL, swd, TO_NOTVICT);
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
                send_to_char(HIC"���F���پ��ϧA�K��۳N���I�Z"NOR"\n\r", rch);
						}
            stop_fighting(ch, FALSE);
            set_fighting(ch, rch);
        }
    }
    
    if( lv >= 39 && swd->move > 60 && sor->mana > sormana) 
    {
        act("$N���S�@�n�A���P���C���H���ӵo�I", sor, NULL, swd, TO_CHAR);
        act("�A���S�@�n�A�L���@���A�H�C��G�X�C�W���]�O�I", sor, NULL, swd, TO_VICT);
        act("$N���S�@�n�A���X�ƹD���P���C��I", sor, NULL, swd, TO_NOTVICT);
        
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
                send_to_char(HIC"���F���پ��ϧA�K��۳N���A�G"NOR"\n\r", rch);
						}
        }
    }
    
    if( lv >= 41 && swd->move > 60 && sor->mana > sormana) 
    {
        act("$N���S�@�n�A���P���C���H���ӵo�I", sor, NULL, swd, TO_CHAR);
        act("�A���S�@�n�A�L���@���A�H�C��G�X�C�W���]�O�I", sor, NULL, swd, TO_VICT);
        act("$N���S�@�n�A���X�ƹD���P���C��I", sor, NULL, swd, TO_NOTVICT);
        
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
               send_to_char(HIC"���F���پ��ϧA�K�����۳N������"NOR"\n\r", rch);	
						}
        }
    }
    if( lv >= 43 && swd->move > 60 && sor->mana > sormana) 
    {
       act("$N���S�@�n�A���P���C���H���ӵo�I", sor, NULL, swd, TO_CHAR);
       act("�A���S�@�n�A�L���@���A�H�C��G�X�C�W���]�O�I", sor, NULL, swd, TO_VICT);
       act("$N���S�@�n�A���X�ƹD���P���C��I", sor, NULL, swd, TO_NOTVICT);
       
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
               send_to_char(HIC"���F���پ��ϧA�K�����۳N���T�G"NOR"\n\r", rch);
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
      send_to_char( "�A���ѤF.\n\r", ch );
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

  act( HIB "�@�ٶ·t���Q�v�L�n�L���a�̪��b"NOR"$N"HIB"�I��I"NOR"", ch, NULL, victim, TO_CHAR );
  act( HIB "�A�ߤ���M�{�L�@�����w�A�ϩ����Y�تF��b�A����......"NOR"" , ch, NULL, victim, TO_VICT);
  act( "$N"HIB"���I�������B�{�X�@�Τ��������C"NOR"", ch, NULL, victim, TO_NOTVICT );

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
		send_to_char( "�A���ѤF.\n\r", ch );
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
				act( "$N���W�w�g��w�N�ĪG.", ch, NULL, victim, TO_CHAR );
				return;
			}
		}
		else{
			act( "$N���W�w�g��w�N�ĪG.", ch, NULL, victim, TO_CHAR );
			return;
		}
	}
	
	effect += UMAX( 0 , GET_DODGE( victim )/(sklv - 11) );

	af.type  = sn;
	//af.duration  = ( 10 + sklv *2 ) * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) * 0.01;
	//if(IsPK(ch, victim)) af.duration /= 4;
	// �ק�@�ήɶ� 2020/11/06
	if(affected_value(victim,sn,APPLY_DODGE) < 0){
		af.duration  = 0;
	}else{
		af.duration  = 3;
	}
	af.location  = APPLY_DODGE;
	af.modifier  = effect * weakness_damage( victim, sn ) * 0.01;
	af.bitvector = AFF_TARDY;
	affect_join( victim, &af );

	act( HIY "�P�򪺪Ů�b�A���]�O�v�T�U�A�Φ��F�H���⪺��������A����F"NOR"$N"HIY"�I"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIY "�j�q����������q"NOR"$n"HIY"���ǩ��A���t�����L�ӡA����F�A�I"NOR"" , ch, NULL, victim, TO_VICT);
	act( HIY "�H���⪺��������̪ťX�{�b"NOR"$n"HIY"���ǡA����F"NOR"$N"HIY"�I"NOR"", ch, NULL, victim, TO_NOTVICT );    

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
      send_to_char( "�A���ѤF.\n\r", ch );
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
  
  act( HIG "�@�Ѹ��Ԫ�����v�����}�A�ϱo"NOR"$N"HIG"�P���n���v�������I"NOR"", ch, NULL, victim, TO_CHAR    );
  act( HIG "�P�򪺭������F�C�C���J�F�I�v�A�A�o�{$E���n���v���Q����F�I"NOR"",  ch, NULL, victim, TO_VICT    );
  act( "$n"HIG"�I�k����X���Ԫ���������A�v�T��"NOR"$N"HIG"���Ǫ��Ů�A�A�v��ť����$E���n���F�I"NOR"", ch, NULL, victim, TO_NOTVICT );
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
/*   �N chance , no_poison  ���}�P�w , ���� invoke �����r�P�w 2021/11/04
	if ( IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON ) 
		|| ( IS_SET( victim->special, NO_POISON ))
		|| ( victim->race == race_lookup( "Vampire" )) 
		|| ( victim->race == race_lookup( "Undead" ))
		|| ( is_affected( victim, gsn_invoke ) && get_skill_level(victim, gsn_invoke) > 3 )
		|| ( number_percent() > chance ))
	{
		send_to_char( "�A���ѤF.\n\r", ch );
		return;
	}
*/

	// chance �P�w
	if( number_percent() > chance ){
		send_to_char( "�A���ѤF.\n\r", ch );
		return;
	}

	// �P�w no_poison
	if(    IS_SET( race_table[victim->race].race_abilities, RACE_NO_POISON )
		|| IS_SET( victim->special, NO_POISON ) )
	{
		// �P�w no_poison �� race_undead �W aff_poison �ƭ� 2021/11/04
		// �[�J aff_ghoul �]�w 2022/01/16
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
			act( ""HIG"�r���I�J"NOR"$N"HIG"���餺�A�����G����L�y������v�T�C"NOR"", ch, NULL, victim, TO_CHAR );
			act( ""HIG"�r���I�J�A���餺�A������A�y������v�T�C"NOR"" , ch, NULL, victim, TO_VICT);
			act( "$N"HIG"������Q�r���I�J�A�����G����L�y������v�T�C"NOR"", ch, NULL, victim, TO_NOTVICT );
		}else{
			send_to_char( "�A���ѤF.\n\r", ch );
			return;
		}
	}else{
		af.type  = gsn_poison;
		af.duration = UMAX( 0 , 1 + sklv - get_curr_con(victim)/5);
		if(IsPK(ch, victim)) af.duration /= 4;
		af.bitvector = AFF_POISON;

	/*  ���� con �v�T 2021/11/04
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

		act( "$N"HIG"�y�Ⲥ���x�C�A�r���w�g�I�J�F�L���餺�I"NOR"", ch, NULL, victim, TO_CHAR );
		act( HIG "���߷P�P��h�P�P��ŧ�ӡA�Aı�o�۷�a�����I"NOR"" , ch, NULL, victim, TO_VICT);
		act( "$N"HIG"������Ÿ�ݤF�@�U�A���ܱo���ǭW�ߡI"NOR"", ch, NULL, victim, TO_NOTVICT );

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
      send_to_char( "�n���C\n\r", ch );
      
  send_to_char( "�A����P�줣�A.\n\r", victim );
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
      send_to_char( HIW"�A�j�j���N�ӤO�A���A�P�����쮣�ߡI"NOR"\n\r", victim);
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
      send_to_char( "�n���C\n\r", ch );
  
  send_to_char( "�A�}�l���W�a���ߤF�_��.\n\r", victim );
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
      send_to_char( "�A���ѤF.\n\r", ch );
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
      send_to_char( "�@�ѺηNŧ��..... zzzzzz.\n\r", victim );
      
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
		send_to_char( "�A���ѤF.\n\r", ch );
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
				act( "$N���W�w�g���I�z�N�ĪG."NOR"", ch, NULL, victim, TO_CHAR );
				return;
			}
		}
		else
		{
			act( "$N���W�w�g���I�z�N�ĪG."NOR"", ch, NULL, victim, TO_CHAR );
			return;
		}
	}
      
	af.type  = gsn_weaken;
	//af.duration  = 1;
	//if(IsPK(ch, victim)) af.duration /= 4;
	// �ק�@�ήɶ� 2020/11/06
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


	// �s�W���C dr 2020/11/06

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
		// �ק�@�ήɶ� 2020/11/06
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

	act( "$N"HIC"�ֽ��_�X��������A�L�ݰ_�Ӧ��G�ܱo���~�ӱo��z�I"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIC "�A�P�즽�y�H�I�A���馳�I�Ϥ��W�O��C"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$N"HIC"�ݰ_�Ӧ��y����A�y��]�ܱo�a�դF�ǡC"NOR"", ch, NULL, victim, TO_NOTVICT );
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
			send_to_char( "�A���ѤF�C\n\r", ch);
			return;
		}
	}

	if( ( !is_same_group( ch, victim) ) ){
		send_to_char("�L���b�A������C\n\r", ch);
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
				send_to_char("�A�w�g��L�����F���pô�F�C\n\r", ch);
		}else
			send_to_char("�L�w�g��O�H�����F���pô�F�C\n\r", ch);
		return;
	}

	if( ch->mana < (100 + ch->max_mana/10) ){
		send_to_char("�A���]�O�����C\n\r", ch);
		return;
	}

	ch->mana -= (100 + ch->max_mana/10);

	if( ch->linking )
		stop_linking( ch , ch->linking );

	add_linking(ch, victim);

	act( HIU "�A�P"NOR"$N"HIU"�إ߰_�F��pô�A�@�P�ɦ��������ͩR�C"NOR"", ch, NULL, victim, TO_CHAR );
	act( "$n"HIU"�P�A�إ߰_�F��pô�A�@�P�ɦ��������ͩR�C"NOR"" , ch, NULL, victim, TO_VICT);

	af.type  = sn;
	af.duration  = 10;
	af.location  = 0;
	af.modifier  = 0;
	af.bitvector = 0;
	affect_to_char( ch, &af );
	affect_to_char( victim, &af );
/*
	�ק� lifeshare �ĪG 2021/11/20

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
      send_to_char("�A���ݭn��ͩR�����ۤv�C\n\r", ch);
      return;
  }
    
  if( total == max_total ) 
  {
      send_to_char("�j�a����O�R���A���ݭn���ɤF�C\n\r", ch);
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

  send_to_char("�n��.\n\r", ch);
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
		act("$n����X�j�j���]�k��q�A��$N���G���@��������v�T�I" , ch, NULL, victim, TO_NOTVICT);
		act("�A����X�j�j���]�k��q�A��$N���G���@��������v�T�I" , ch, NULL, victim, TO_CHAR);
		return;
	}

	if (!IS_NPC(victim)) {
		int nInvokeSklv = get_skill_level(victim, gsn_invoke);
		if (nInvokeSklv >= 6) {
			if (is_affected(victim, gsn_invoke)) {
				send_to_char(HIC"�A�]�����F���O�@�K����쯻�H�N���v�T"NOR".\n\r", victim);
				act("$n����X�j�j���]�k��q�A��$N���G���@��������v�T�I" , ch, NULL, victim, TO_NOTVICT);
				act("�A����X�j�j���]�k��q�A��$N���G���@��������v�T�I" , ch, NULL, victim, TO_CHAR);
				return;
			}
		}
	}

	//ch->mana += sklv * 15 + (sklv - 3) * 10;

	if( IS_SET(ch->in_room->room_flags, ROOM_SAFE ) )
	{ 
		send_to_char("�o�̸T��԰�. \n\r",ch);
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
	// mystic armor ��s , �@�����@�w��ް� 2020/10/27
	//affect_strip( victim, gsn_mystic_armor);
	//REMOVE_BIT(victim->affected_by, AFF_MYSTIC_ARMOR);
	  
	act("$n����X�j�j���]�k��q�A���H�F$N���W���@���k�N�I" , ch, NULL, victim, TO_NOTVICT);
	act("$n����X�j�j���]�k��q�A���H�F�A���W���@���k�N�I" , ch, NULL, victim, TO_VICT);
	act("�A����X�j�j���]�k��q�A���H�F$N���W���@���k�N�I" , ch, NULL, victim, TO_CHAR);
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
		send_to_char( "�A���ѤF.\n\r", ch );
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
				act( "$N���W�w�g���ѨϤ����ĪG."NOR"", ch, NULL, victim, TO_CHAR );
				return;
			}
		}
		else
		{
			act( "$N���W�w�g���ѨϤ����ĪG."NOR"", ch, NULL, victim, TO_CHAR );
			return;
		}
	}
	
	effect += UMAX( 0 , GET_AC( victim )/(sklv - 10) );
      
	af.type  = sn;
	//af.duration  = 1;
	//if(IsPK(ch, victim)) af.duration /= 4;
	// �ק�@�ήɶ� 2020/11/06
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
		// �ק�@�ήɶ� 2020/11/06
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
		// �ק�@�ήɶ� 2020/11/06
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

	act( HIP"�A��ӯ������O�q�A�@���޲���������¶�b"NOR"$N"HIP"���ǡI"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIP"$n"HIP"���ǫ_�X���ǲ�������W�F�A�I"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$n"HIP"�l��X�t���⪺����A�̪��b"NOR"$N"HIP"�����W�I"NOR"", ch, NULL, victim, TO_NOTVICT );    
     
	mprog_hurt_trigger(victim, ch, sn);
  
	return;
}


