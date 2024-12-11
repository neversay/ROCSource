//*****************************************************************
//*  PROGRAM ID  : spell_lightning.c                              *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         �ק�lightning_bolt�����ٷ|�l����bug    *
//*              :             �W�[chk_room�P�_�Q���̬O�_���`     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 coconet                                *
//*  PURPOSE     :         �ץ�Chain lightning�l�[�a�誺md �P�_   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1060131 jye                                    *
//*  PURPOSE     :         �ק�spell_call_lightning               *
//*****************************************************************
/* Id*/
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"

extern void say_spell args( ( CHAR_DATA *ch, int sn ) );
extern void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim );
extern void fall_effect( CHAR_DATA *ch );
extern void spell_flaming_armor_strike( int sn, int level, CHAR_DATA *ch, void *vo );
/*
 * Lookup a skill by name.
 */
extern int skill_lookup( const char *name );
extern int saves_spell( int pp, CHAR_DATA *victim );
extern int new_saves_spell( int pp, CHAR_DATA *victim, int type );
extern int old_saves_spell( int pp, CHAR_DATA *victim, int type );
extern int new_saves_timer( int pp, CHAR_DATA *victim, int type );
extern double int_effect( CHAR_DATA* ch );
extern double wis_effect( CHAR_DATA* ch );
extern char *target_name;
extern int weakness_percent( CHAR_DATA *ch, int sn);
extern int weakness_damage( CHAR_DATA *ch, int sn);

void spell_call_lightning( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next; // add vch_next , 2020/10/30
	int dam;
	int sklv   = get_skill_level(ch,sn);

	//int scale = IS_NPC(ch) ? level * 10 : (level+get_skill_level(ch,sn)/2) * (28+get_skill_level(ch,sn));
	
	if ( !IS_NPC(ch) ){
		int obj_lv = level;
		if( level >= 1000){
			// call lightning �] obj_cast_spell �� level URANGE ���@�� �ҥH�N�J obj lv ���
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
		//if ( !IS_OUTSIDE( ch ) && sklv < 4){
		// �P�w���B�~ obj lv ��I�k���󪺼v�T( weapon master , wand , staves , scroll) add at 2020/10/14
		if ( !IS_OUTSIDE( ch ) && sklv < 4 && obj_lv < 40){
			send_to_char( "�A�����b��~�~��I�i�{�q�l��N.\n\r", ch );
			return;
		}
		if ( weather_info.sky < SKY_RAINING ){
			if ( ch->class != SAVANT){
				send_to_char( "�Ѯ��٤Ӧn�F.\n\r", ch );
				return;
			//}else if (sklv != 6){
			// �P�w���B�~ obj lv ��I�k���󪺼v�T( weapon master , wand , staves , scroll) add at 2020/10/14
			}else if (sklv < 6 && obj_lv < 60){
				send_to_char( "�Ѯ��٤Ӧn�F.\n\r", ch );
				return;
			}
		}
	}
	//dam = dice( level, 8 );

	send_to_char( HIW"�@�D�Ӧۤѻڪ��{�q�A�A���b�A���e���a���I\n\r"NOR, ch );
	act( HIW"�@�D�p������$n"HIW"���I���A�q���żA����$s���e�I"NOR,  ch, NULL, NULL, TO_ROOM );

	//for ( vch = char_list; vch; vch = vch->next ){
	for( vch = ch->in_room->people; vch; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if ( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
		if ( ch == vch || vch->deleted || !vch->in_room ) continue;
		if ( is_same_group(ch, vch) ) continue;

		// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, vch, TO_CHAR);
			continue;
		}

		if ( IS_NPC( ch )){
		
			if ( vch->fighting == ch || !IS_NPC( vch ) ){
				dam  = (40 + ch->level)/2;
				dam += get_curr_wis(ch)/2 + WIS_DIFF( ch, vch );
				dam  = dam * (new_saves_spell( ch->pp, vch, MAGIC_LIGHTNING )) / 100;
				dam  = number_range( dam * 10 , dam * 16 );
				dam += ch->amdr * 1.5;
				dam -= vch->mdr * 1.5;
				magic_damage( ch, vch, dam, sn, WEAR_NONE );
				
			}
				continue;
		}
		else if ( (vch->fighting == ch && !IS_NPC(vch)) || ( IS_NPC( vch ) && !IS_AFFECTED( vch, AFF_CHARM ) && !IS_AFFECTED( vch, AFF_SUMMONED ) ) )
		{
			dam  = (40 + ch->level)/2; 
			dam += get_curr_wis(ch)/2 + WIS_DIFF( ch, vch );
			dam  = dam * weakness_damage( vch, sn ) / 100;
			
			if (ch->class == SAVANT){
				if ( IS_GOOD( ch ) ) {
					dam = dam * ((new_saves_spell( ch->pp * 2, vch, MAGIC_SAINT ) >= (new_saves_spell( ch->pp * 1.5, vch, MAGIC_LIGHTNING )))
								? (new_saves_spell( ch->pp * 2, vch, MAGIC_SAINT )) / 100 : (new_saves_spell( ch->pp * 1.5, vch, MAGIC_LIGHTNING )) / 100);
				}else if ( IS_EVIL( ch ) ) {
					dam = dam * ((new_saves_spell( ch->pp * 2, vch, MAGIC_DARK ) >= (new_saves_spell( ch->pp * 1.5, vch, MAGIC_LIGHTNING )))
								? (new_saves_spell( ch->pp * 2, vch, MAGIC_DARK )) / 100 : (new_saves_spell( ch->pp * 1.5, vch, MAGIC_LIGHTNING )) / 100);
				}else{
					dam = dam * (new_saves_spell( ch->pp * 2, vch, MAGIC_LIGHTNING )) / 100;
				}
			}else{
				dam = dam * (new_saves_spell( ch->pp, vch, MAGIC_LIGHTNING )) / 100;
			}
			
			dam = number_range( dam * ( 1 + sklv * 1.5 ) , dam * ( 1 + sklv * 2.5 ) );
			dam += ch->amdr * 2;
			dam -= vch->mdr * 2;
			// savant ingnore sanctuary effect 20170131
			if ( ch->class == SAVANT ){
				magic__damage( ch, vch, dam * 2, sn, WEAR_NONE );
			}else{
				magic_damage( ch, vch, dam, sn, WEAR_NONE );
			}
			continue;
		}
		
	if ( vch->in_room->area == ch->in_room->area
		&& IS_OUTSIDE( vch )
		&& IS_AWAKE( vch ) )
		send_to_char(HIY "�@�D�{�q���L�ѻڡA�A���b�a��W."NOR"\n\r", vch );
	}
  return;
}

// fulgurant dance add by whisperwind 10.31.2004
void spell_fulgurant_dance ( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
  
    int sklv = get_skill_level(ch,sn);
    int dam;
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

	send_to_char( HIW "�A���}�U�G�F�_�ӡA�@�D�D�զ⪺�p�q���W�ƨg�a���L���ǡI\n\r"NOR"", ch );
	act(  "$n "HIW"�r�M�i�}����A�}�U�B�{�X���j�����~�P�}�Aģ�����զ�p���۰}���g�S�ĥX�I"NOR"", ch, NULL, NULL, TO_ROOM );
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
				dam = 0;
				dam = (40 + ch->level)/2;
				dam = dam * int_effect( ch );

				if( sklv > 0 )
					dam = dam * ( 10 + sklv) / 10 ;
				if( sklv > 3 )
					dam = dam * 1.1;
				if( sklv > 4 )
					dam = dam * 1.1;
				if( sklv > 5 )
					dam = dam * 1.1;

				dam = dam * weakness_damage( victim, sn ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) / 100;
				dam = number_range( dam * 6 , dam * 8 );
      
				if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
					continue;
				if (is_affected( victim, skill_lookup("prot. from lightning")))
					continue;
				// �� mystic armor ���@�έt�� 2020/10/26
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
					continue;
				/*
				af.type      = sn;
				af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
				af.bitvector = 0;
				af.location  = APPLY_SAVING_LIGHTNING;
				af.modifier  = 3 + sklv + sklv/3;
				affect_join( victim, &af );

				af.location  = APPLY_INT;
				af.modifier  = -3 - sklv;
				affect_join( victim, &af );

				af.location  = APPLY_POWER_POTENTIAL;
				af.modifier  = -12 - sklv*3;
				affect_join( victim, &af );

				af.location  = APPLY_AMAGIC;
				af.modifier  = -30 - sklv*4;
				affect_join( victim, &af );
				*/
				af.type      = sn;
				af.bitvector = 0;
				//af.duration  = 2 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
				// �ק�@�ήɶ���debuff �W�� 2020/11/06
				if( affected_value(victim,sn,APPLY_SAVING_LIGHTNING) < (affected_value(victim,sn,APPLY_SAVING_LIGHTNING) - NEW_GET_SVS(victim, MAGIC_LIGHTNING) ) ){
					if(affected_value(victim,sn,APPLY_SAVING_LIGHTNING) > 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_SAVING_LIGHTNING;
					af.modifier  = 4 + sklv;
					affect_join( victim, &af );
				}
				
				if( affected_value(victim,sn,APPLY_INT) > -3 ){
					if(affected_value(victim,sn,APPLY_INT) < 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_INT;
					af.modifier  = -1;
					affect_join( victim, &af );
				}

				if(affected_value(victim,sn,APPLY_POWER_POTENTIAL) > -30 ){
					if(affected_value(victim,sn,APPLY_POWER_POTENTIAL) < 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_POWER_POTENTIAL;
					af.modifier  = -4 - sklv;
					affect_join( victim, &af );
				}

				if( affected_value(victim,sn,APPLY_AMAGIC) > -100 ){
					if(affected_value(victim,sn,APPLY_AMAGIC) < 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_AMAGIC;
					af.modifier  = -20 - sklv*5;
					affect_join( victim, &af );
				}
			}
		}
		else if ( victim->fighting == ch
					|| ( IS_NPC( victim )
					&& !IS_AFFECTED( victim, AFF_CHARM )
					&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
		{
			dam = 0;
			dam = (40 + ch->level)/2;
			dam = dam * int_effect( ch );
        
			if( sklv > 0 )
				dam = dam * ( 10 + sklv) / 10 ;
			if( sklv > 3 )
				dam = dam * 1.1;
			if( sklv > 4 )
				dam = dam * 1.1;
			if( sklv > 5 )
				dam = dam * 1.1;

			dam = dam * weakness_damage( victim, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) / 100;
			dam = number_range( dam * 6 , dam * 8 );
      
			if(magic_damage( ch, victim, dam, sn, WEAR_NONE ))
				continue;
			if (is_affected( victim, skill_lookup("prot. from lightning")))
				continue;
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			/*
			af.type      = sn;
			af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			af.bitvector = 0;
			af.location  = APPLY_SAVING_LIGHTNING;
			af.modifier  = 3 + sklv + sklv/3;
			affect_join( victim, &af );

			af.location  = APPLY_INT;
			af.modifier  = -3 - sklv;
			affect_join( victim, &af );

			af.location  = APPLY_POWER_POTENTIAL;
			af.modifier  = -12 - sklv*3;
			affect_join( victim, &af );
          
			af.location  = APPLY_AMAGIC;
			af.modifier  = -30 - sklv*4;
			affect_join( victim, &af );
			*/
			af.type      = sn;
			af.bitvector = 0;
			//af.duration  = 2 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			// �ק�@�ήɶ���debuff �W�� 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_LIGHTNING) < (affected_value(victim,sn,APPLY_SAVING_LIGHTNING) - NEW_GET_SVS(victim, MAGIC_LIGHTNING) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_LIGHTNING) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_LIGHTNING;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
			
			if( affected_value(victim,sn,APPLY_INT) > -3 ){
				if(affected_value(victim,sn,APPLY_INT) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_INT;
				af.modifier  = -1;
				affect_join( victim, &af );
			}

			if(affected_value(victim,sn,APPLY_POWER_POTENTIAL) > -30 ){
				if(affected_value(victim,sn,APPLY_POWER_POTENTIAL) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_POWER_POTENTIAL;
				af.modifier  = -4 - sklv;
				affect_join( victim, &af );
			}

			if( affected_value(victim,sn,APPLY_AMAGIC) > -100 ){
				if(affected_value(victim,sn,APPLY_AMAGIC) < 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_AMAGIC;
				af.modifier  = -20 - sklv*5;
				affect_join( victim, &af );
			}
        }
	}
}


// chain lightning added by whisperwind 10.1.2004
void spell_chain_lightning( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	CHAR_DATA *victim_next ;
	CHAR_DATA *victim_temp = NULL;

	int sklv     = get_skill_level( ch, sn );
	int nLBSklv  = get_skill_level(ch, skill_lookup("lightning bolt"));
	int times    = 2 + sklv;
	int nDmgPerc = 100;
	int obj_lv = level;
	double dam;
	double basic_dam;
	double ex_md;

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
  
	act( HIW "�j�q���{�q�y�̪ťX�{�b�A�|�P�A�����Ʀ�ģ�����{�q���W���V"NOR"$N"HIW"�I"NOR"", ch, NULL, victim, TO_CHAR );
	act( HIW "�L�ƪ��{�q���W�ۥ|���K��§A��ŧ�ӨӡI"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$n"HIW"�g�X�ƶq�e�j���{�q���W�A��´���K�����{�q�����I"NOR"", ch, NULL, victim, TO_NOTVICT );
  
	dam  = dam * int_effect( ch );
	dam *= 1 + 0.1*sklv + 0.1*sklv/2;
	if(sklv > 2) dam = dam * 1.065;
	if(sklv > 3) dam = dam * 1.065;
	if(sklv > 4) dam = dam * 1.055;
	if(sklv > 5) dam = dam * 1.055;
      
	basic_dam = dam;

	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) / 100;
	if(!IS_NPC(ch)) dam = dam * (250 + nLBSklv * (nLBSklv + 4)) / 250;

  if( !IS_NPC(ch) )
    dam = number_range( dam * 4 , dam * 6 );
  else if ( IS_NPC(ch) )
    dam = number_range( dam * 3 , dam * 4 );
    
	if (IS_NPC(ch) ) dam += ch->level*2;
  
	ex_md = ch->amdr * (12 + sklv) / 6;
	dam  += ex_md;

	dam  -= victim->mdr * 3;
	dam   = dam * weakness_damage( victim, sn ) / 100;
  
	magic_damage( ch, victim, dam, sn, WEAR_NONE );
	victim_temp = victim;
        
	while(times--){
		for( victim = ch->in_room->people; victim ; victim = victim_next) {
			victim_next = victim->next_in_room;
			if ( victim->deleted ) continue;
			if ( is_same_group( ch, victim ) ) continue;
			if ( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS)) continue;
			if ( IS_NPC( ch ) && IS_NPC( victim ) && !(victim->fighting == ch) ) continue; 
			if ( !IS_NPC( victim ) && !(victim->fighting == ch) )  continue; 
            if ( IS_NPC( victim ) && IS_AFFECTED( victim, AFF_CHARM ) && IS_AFFECTED( victim, AFF_SUMMONED ) )
				continue;
			// �]�k�K�� mob ���F wiz �~, �L�k�@�Υ��� spell 2020/10/29
			if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
			{
				act("�A���]�k��$N�_���F�@�ΡA�ݨӥL�i��O�]�k�K��." , ch, NULL, victim, TO_CHAR);
				continue;
			}
			if ( number_percent() > 66 - sklv * 6 ) {
				if( !victim_temp || victim_temp != victim ){
					ch->mana -= 20;
					nDmgPerc = nDmgPerc * (100 - 51 + sklv * 6) / 100; 
					dam = basic_dam;
					dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) / 100;
					if(!IS_NPC(ch)) dam = dam * (250 + nLBSklv * (nLBSklv + 4)) / 250;

					if( !IS_NPC(ch) )
						dam = number_range( dam * 4 , dam * 6 );
					else if ( IS_NPC(ch) )
						dam = number_range( dam * 3 , dam * 4 );
						
					if (IS_NPC(ch) ) dam += ch->level*2;
					
					dam  += ex_md;
					dam   = dam * nDmgPerc / 100;

					dam  -= victim->mdr * 3;
					dam   = dam * weakness_damage( victim, sn ) / 100;
					
					magic_damage( ch, victim, dam, sn, WEAR_NONE );

					victim_temp = victim;
					break;
				}
			}
		}
		if( ch->mana < 20 ) break;
  }
}	


// electrostatic bolt modified by whisperwind 8.24.2004
void spell_electrostatic_bolt( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  int sklv = get_skill_level(ch,sn);
  int times = 0 + sklv/2;
  double dam;
  double ex_dam;
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
		dam  = (40 + obj_lv)/2;
		times = 0 + sklv/2;
	}else{
		dam  = (40 + level)/2;
	}

  if(sklv > 4) times += 1;

  dam = dam * int_effect( ch );

  if (IS_NPC(ch) )
    dam += ch->level*2;
  
  if( !IS_NPC(ch) )
    dam = number_range( dam * 2.3 , dam * 2.5 );
  else if ( IS_NPC(ch) )
    dam = number_range( dam * 1.8 , dam * 2 );

  //dam = dam * weakness_damage( victim, sn ) / 100;
  //dam = dam * ( old_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) / 100;

  ex_dam = ch->amdr - 0.45*UMAX( ch->amdr-40-sklv*5,0) - 0.45*UMAX( ch->amdr-50-sklv*5,0);
  dam += ex_dam*1.2;
  
  dam *= 1 + 0.15*sklv;
  if(sklv > 2) dam = dam * 1.03;
  if(sklv > 3) dam = dam * 1.03;
  if(sklv > 4) dam = dam * 1.02;
  if(sklv > 5) dam = dam * 1.02;
  
  int nSvT = NEW_GET_SVS( victim, MAGIC_LIGHTNING );

  if(nSvT >= 0){
      nSvT = UMAX(nSvT + ch->pp/(13-sklv) , nSvT);
  }else if(ch->pp > 0){
      nSvT = UMIN(nSvT + ch->pp/(12-sklv) , 0);
  }

  if(nSvT > 0){
      dam = dam * (60 + nSvT) / 60;
  }else{
      dam = dam * 60 / (60 - nSvT);
  }

  dam = dam * weakness_damage( victim, sn ) / 100;

  dam -= victim->mdr;

  dam = dam * ( old_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) / 100;

  if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
  {}
  else
  {
	// �� mystic armor ���@�έt�� 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
     if ( sklv * 2 +old_saves_spell( ch->pp, victim, MAGIC_LIGHTNING ) > (number_percent( ) * 100 / weakness_percent( victim, sn)) 
          && dam > 0) // ���ˮ`�~�|�I�[�ĪG add at 19/02/04
     {
         af.type     = sn;
         //af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
		// �ק�@�ήɶ� 2020/11/06
		af.bitvector = 0;
         if(sklv >= 4 && affected_value(victim,sn,APPLY_POWER_POTENTIAL) > -sklv * 2 )
         {
			if(affected_value(victim,sn,APPLY_POWER_POTENTIAL) < 0){
				af.duration  = 0;
			}else{
				af.duration  = 3;
			}
            af.location  = APPLY_POWER_POTENTIAL;
            af.modifier  = -1-sklv/2;
            affect_join( victim, &af );
            act( HIW "�Q�p���������b���A"NOR"$N"HIW"���������h�F�J�I�C"NOR"", ch, NULL, victim, TO_CHAR );
            act( HIW "�{�q�����A�������A�@�����H���몺�w�t�P����ŧ�ӡI"NOR"" , ch, NULL, victim, TO_VICT);
            act( HIW "�Q�p���������b���A"NOR"$N"HIW"���������h�F�J�I�C"NOR"", ch, NULL, victim, TO_NOTVICT );  
         }
     }
	}
  }
  while( times-- && !victim->deleted&& victim->in_room == ch->in_room ) {
     if ( number_percent() > 66 - sklv*5 ) {
		 
        if( ch->mana > 20 )
            ch->mana -= 20;
        else
            break;
		
        if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
        {}
        else
        {
			// �� mystic armor ���@�έt�� 2020/10/26
			if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){
				if ( sklv * 2 +new_saves_spell( ch->pp, victim, MAGIC_LIGHTNING ) > (number_percent( ) * 100 / weakness_percent( victim, sn)) 
					&& dam > 0)  // ���ˮ`�~�|�I�[�ĪG add at 19/02/04
				{
					af.type     = sn;
					//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
					// �ק�@�ήɶ� 2020/11/06
					af.bitvector = 0;
					if(sklv > 4 && affected_value(victim,sn,APPLY_POWER_POTENTIAL) > -sklv * 2 )
					{
						if(affected_value(victim,sn,APPLY_POWER_POTENTIAL) < 0){
							af.duration  = 0;
						}else{
							af.duration  = 3;
						}
						af.location  = APPLY_POWER_POTENTIAL;
						af.modifier  = -1-sklv/2;
						affect_join( victim, &af );
						act( HIW "�Q�p���������b���A"NOR"$N"HIP"���������h�F�J�I�C"NOR"", ch, NULL, victim, TO_CHAR );
						act( HIW "�{�q�����A�������A�@�����H���몺�w�t�P����ŧ�ӡI"NOR"" , ch, NULL, victim, TO_VICT);
						act( HIW "�Q�p���������b���A"NOR"$N"HIP"���������h�F�J�I�C"NOR"", ch, NULL, victim, TO_NOTVICT );  
					}
				}
			}
        }
     }
  }
            return;
}

// lightning bolt modified by jye 6.12.2005
void spell_lightning_bolt( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  ROOM_INDEX_DATA *	chk_room;


  
  int sklv = get_skill_level( ch, sn );
  int times = 0 + sklv/2 + sklv/3;
  double dam;
  double ex_md;
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
		dam  = (40 + obj_lv)/2;
		times = 0 + sklv/2;
	}else{
		dam  = (40 + level)/2;
	}
  
  dam = dam * int_effect( ch );

  if( IS_NPC(ch) )
      dam += ch->level*2;


  //dam *= ( old_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) * 0.01;
  
  
  
	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.8 , dam * 3 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 2.1 , dam * 2.3 );

  // the previous formulation
  // ex_md = ch->amdr - 0.33 * UMAX( ch->amdr-45-sklv*5,0) 
  //       - 0.33 * UMAX( ch->amdr-70-sklv*5,0);
  // for md 200 sklv 6:
  //     before: 200 - 0.33 * (200 - 75) - 0.33 * (200 - 100)
  //           = 200 - 125 / 3 - 100 / 3 = 200 - 225 / 3 = 125
  //     after : 200 - 0.2 * (200 - 80) - 0.2 * (200 - 120)
  //           = 200 - 120 / 5 - 80 / 5 = 200 - 200 / 5 = 160
  // for md 150 sklv 6:
  //     before: 150 - 75 / 3 - 50 / 3 = 150 - 125 / 3 = 111
  //     after : 150 - 70 / 5 - 30 / 5 = 150 - 100 / 5 = 130
  // for md 100 sklv 6:
  //     before: 100 - 25 / 3 = 92
  //     after : 100 - 20 / 5 = 96
  ex_md = ch->amdr - 0.4 * UMAX( ch->amdr - 50 - sklv * 5, 0)
          - 0.4 * UMAX( ch->amdr - 80 - sklv * 5, 0);
  dam  += ex_md * 1.5;
  
  dam *= 1 + 0.15*sklv;
  if(sklv > 2) dam = dam * 1.045;
  if(sklv > 3) dam = dam * 1.045;
  if(sklv > 4) dam = dam * 1.035;
  if(sklv > 5) dam = dam * 1.035;

  if (dam < 0) dam = 0;

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
/* old version
  dam -= victim->mdr * 3;
  
  dam  *= weakness_damage( victim, sn ) * 0.01;
*/

  // ��� mdr �����z�I�v�T�A�z�I�ĪG�N�ܱo�����
  dam  *= weakness_damage( victim, sn ) * 0.01;

  dam -= victim->mdr;

  dam *= ( old_saves_spell( ch->pp, victim,MAGIC_LIGHTNING)) * 0.01;

  chk_room = victim->in_room;
  magic_damage( ch, victim, dam, sn, WEAR_NONE );
  if(sklv > 4){
    magic_damage( ch, victim, dam, sn, WEAR_NONE );
	times -= 1;
  }
  while( times-- 
         && !victim->deleted
         && victim->in_room == ch->in_room 
         && victim->in_room == chk_room) 
  {
         if( number_percent() > 66 - sklv*5 ) 
         {
             if( ch->mana > 40 )
                 ch->mana -= 40;
             else 
                 break;
             
             magic_damage( ch, victim, dam, sn, WEAR_NONE );
         } 
  }
  return;
}

// wail of tempest add by whisperwind 10.31.2004
void spell_wail_of_tempest ( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim, *victim_next;
    AFFECT_DATA af;
    OBJ_DATA *_armor;
  
    int sklv = get_skill_level(ch,sn);
    int dam;
    int i, place;
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
    
	send_to_char( HIG "�Ů�]�A���]�O�Ӳ��Ͳ��ܡA�@�D�D���l���I�S�ӹL�I\n\r"NOR"", ch );
	act(  "$n "HIG"�P�򪺪Ů𲣥Ͳ��ܡA�಴���A�Ƥ��M���g�P�Q�b�p�P�L�u�ɫB��ŧ�ӡI"NOR"", ch, NULL, NULL, TO_ROOM );
	for( victim = ch->in_room->people; victim; victim = victim_next)
	{
		victim_next = victim->next_in_room;
		if ( is_same_group( ch, victim ) )
			continue;

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
				dam = 0;
				dam = (40 + ch->level)/2;
				dam = dam * int_effect( ch );
				
				if( sklv > 0 )
					dam = dam * ( 10 + sklv) / 10 ;
			
				if( sklv > 3 )
					dam = dam * 1.1;
				if( sklv > 4 )
					dam = dam * 1.1;
				if( sklv > 5 )
					dam = dam * 1.1;

				dam = dam * weakness_damage( victim, sn ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
				dam = number_range( dam * 8 , dam * 10 );
			  
				if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
					continue;

				if (is_affected( victim, skill_lookup("prot. from wind")))
					continue;
				// �� mystic armor ���@�έt�� 2020/10/26
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
					continue;
				/*
					af.type      = sn;
					af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
					af.bitvector = 0;
					af.location  = APPLY_SAVING_WIND;
					af.modifier  = 3 + sklv + sklv/3;
					affect_join( victim, &af );
				*/
				af.type      = sn;
				af.bitvector = 0;
				//af.type      = sn;
				//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
				// �ק�@�ήɶ���debuff �W�� 2020/11/06
				if( affected_value(victim,sn,APPLY_SAVING_WIND) < (affected_value(victim,sn,APPLY_SAVING_WIND) - NEW_GET_SVS(victim, MAGIC_WIND) ) ){
					if(affected_value(victim,sn,APPLY_SAVING_WIND) > 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
					af.location  = APPLY_SAVING_WIND;
					af.modifier  = 4 + sklv;
					affect_join( victim, &af );
				}

				while( sklv-- ) 
				{
					for( i=0 ; i<15; ++i )
					{   //max try 15 times
						place = number_range (1,15);

						if( ( _armor = get_eq_char(victim, place) ) 
							&& ( _armor->item_type == ITEM_ARMOR ) )
						{
							waste_dura(_armor);
							waste_dura(_armor);
							waste_dura(_armor);
							waste_dura(_armor);
							break;
						}
					}
							
					if ( (sklv < 0) || (sklv > 6) )
						break;
				}
			}
		}
		else if ( victim->fighting == ch
					|| ( IS_NPC( victim ) 
					&& !IS_AFFECTED( victim, AFF_CHARM ) 
					&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
		{
			dam = 0;
			dam = (40 + ch->level)/2;
			dam = dam * int_effect( ch );
        
			if( sklv > 0 )
				dam = dam * ( 10 + sklv) / 10 ;
    
			if( sklv > 3 )
				dam *= 1.1;
    
			if( sklv > 5 )
				dam *= 1.1;

			dam = dam * weakness_damage( victim, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
			dam = number_range( dam * 8 , dam * 10 );
      
			if(magic_damage( ch, victim, dam, sn, WEAR_NONE )==-2)
				continue;
			if (is_affected( victim, skill_lookup("prot. from wind")))
				continue;
			// �� mystic armor ���@�έt�� 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			/*
			af.type      = sn;
			af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_SPELL)) / 100;
			af.bitvector = 0;
			af.location  = APPLY_SAVING_WIND;
			af.modifier  = 3 + sklv + sklv/3;
			affect_join( victim, &af );
			*/
			af.type      = sn;
            af.bitvector = 0;
            //af.type      = sn;
            //af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_FIRE)) / 100;
			// �ק�@�ήɶ���debuff �W�� 2020/11/06
			if( affected_value(victim,sn,APPLY_SAVING_WIND) < (affected_value(victim,sn,APPLY_SAVING_WIND) - NEW_GET_SVS(victim, MAGIC_WIND) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_WIND) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
				af.location  = APPLY_SAVING_WIND;
				af.modifier  = 4 + sklv;
				affect_join( victim, &af );
			}
		}
    }
}