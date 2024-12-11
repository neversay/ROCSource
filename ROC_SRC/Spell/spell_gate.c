//*****************************************************************
//*  PROGRAM ID  : spell_gate.c                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940701 Razgriz                                *
//*  PURPOSE     :         �ק� spell_gate() �b�I�k�̬� npc �ɡA  *
//*              :         �[�J qmark �ˬd�A���p�W�L�l��W���A�N  *
//*              :         �����l��                               *
//*              :         ���p�� npc �ƥ��L�� qmark�A�N�|�۰�    *
//*              :         ���L�[�W�A�ç�l��W���]�� 5           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940710 jye                                    *
//*  PURPOSE     :         �ק�spell_gate ppl����ϥ�             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940711 jye                                    *
//*  PURPOSE     :         �s�Wspell_flame_elemental              *
//*              :             spell_water_elemental              *
//*              :             spell_earth_elemental              *
//*              :             spell_wind_elemental               *
//*              :             spell_lightning_elemental          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950403 jye                                    *
//*  PURPOSE     :         �ק�spell_gate�Qcharm or summoned ��mob*
//*              :         ����ϥ�                               *
//*****************************************************************
#include "skill.h"
#include "../include/merc.h"
//aspell_flame_elemental added by jye 0940711
void AddOwnerNameToElemental(CHAR_DATA *victim, CHAR_DATA *ch, char szEleType[], char count[])
{
  char buf[MAX_INPUT_LENGTH];
  strcpy(buf, "");
  strcat(buf, szEleType);
  strcat(buf, ch->name);
  strcat(buf, "-");
  strcat(buf, count);
  strcat(buf, " ");
  strcat(buf, victim->name);
  free_string(victim->name);
  victim->name = str_dup(buf);
  victim->alignment = ch->alignment;
}

void spell_flame_elemental( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;
  char *arg    = (char*) vo;
  int sklv     = get_skill_level(ch,sn);
  QMARK *qmark = NULL;

  char buf[MAX_INPUT_LENGTH];
  char szEleType[20];
  char count[20];
  strcpy(count, "");
  strcpy(szEleType, "f");
  
  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 300;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl������۴��~�A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 300;
      return;
  }
  
  if( !IS_NPC(ch) )
  {
	if (sklv < 6) {
  	  if( (qmark = is_qmark( ch, "max_water_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W�����������M���������۽ġA�L�k�۴������۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_lightning_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W���p�������M���������۽ġA�L�k�۴������۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
  	}
      if( (qmark = is_qmark( ch, "max_flame_mobs" ))!= NULL ) 
      {
          sprintf(buf, "self max_flame_mobs v2 = %d", sklv);
          do_mod_qmark( ch, buf );
          
          if( sklv == 6 )
          {
              do_mod_qmark( ch, "self max_flame_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 3" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 1" );
          }                                          
          else if( sklv == 5 )                       
          {                                          
              do_mod_qmark( ch, "self max_flame_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 1" );
          }                                          
          else if( sklv == 4 )                       
          {                                          
              do_mod_qmark( ch, "self max_flame_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 0" );
          }                                          
          else if( sklv == 3 )                       
          {                                          
              do_mod_qmark( ch, "self max_flame_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 0" );
          }                                          
          else if( sklv == 2 )                       
          {                                          
              do_mod_qmark( ch, "self max_flame_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 0" );
          }                                          
      	  else if( sklv == 1 )                       
      	  {                                          
              do_mod_qmark( ch, "self max_flame_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 0" );
          }                                          
      	  else                                       
      	  {                                          
              do_mod_qmark( ch, "self max_flame_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_flame_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_flame_lv2  v1 = 0" );
          }
          
          qmark = is_qmark( ch, "max_flame_mobs" );
          
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char( "�A�L�k�A�l��X��h�������۴��~�I\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_flame_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h���������I\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 4 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_flame_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�������I\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv >= 5 )
          {
              if( (qmark = is_qmark( ch, "max_flame_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h������ġI\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
              
      }
      else 
      {
      	  //�gmax_flame_mobs ��ppl���W
      	  //max_flame_lv qmark��ppl���W
          if( sklv == 6 )
          {
              do_add_qmark( ch, "self max_flame_mobs -1 0 3 6 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 1 0 0 0" );
          }
          else if( sklv == 5 )
          {
              do_add_qmark( ch, "self max_flame_mobs -1 0 3 5 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 1 0 0 0" );
          }
          else if( sklv == 4 )
          {
              do_add_qmark( ch, "self max_flame_mobs -1 0 3 4 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 3 )
          {
              do_add_qmark( ch, "self max_flame_mobs -1 0 2 3 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 2 )
          {
              do_add_qmark( ch, "self max_flame_mobs -1 0 2 2 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 0 0 0 0" );
          }
      	  else if( sklv == 1 )
      	  {
              do_add_qmark( ch, "self max_flame_mobs -1 0 1 1 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 0 0 0 0" );
          }
      	  else
      	  {
              do_add_qmark( ch, "self max_flame_mobs -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_flame_lv2 -1 0 0 0 0 0" );
          }
      }
  }

  do_say( ch, "���@�Nñ�q�媺�����A�洫�A�j�j�������O�q�I" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
      act( "�b�Ť@�}���T�A�@�μ@�P�U�N����������M�X�{�b$n�e���I", ch, NULL, NULL, TO_ROOM );
      victim    = create_mobile( get_mob_index( MOB_VNUM_SOR_FLAME0 ) );
      strcat(szEleType, "1-");
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_flame_lv0 v0 + 1" );
	  
	if( (qmark = is_qmark( ch, "max_flame_lv0" ))!= NULL ){
		if( !(qmark->v2 & 1) ){
			do_mod_qmark( ch, "self max_flame_lv0 v2 | 1" );
			do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
			strcpy(count , "A");
		}else if( !(qmark->v2 & 2)){
			do_mod_qmark( ch, "self max_flame_lv0 v2 | 2" );
			do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
			strcpy(count , "B");
		}else if( !(qmark->v2 & 4)){
			do_mod_qmark( ch, "self max_flame_lv0 v2 | 4" );
			do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
			strcpy(count , "C");
		}
	}
  }
  else if( (sklv < 5 && arg[0] == '\0') || ( sklv > 2 && !str_cmp( arg , "2")) )
  {
  	  if( ch->mana < 500)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 300;
  	  	  return;
  	  }
  	  
      act( "�b�Ť@�}���T�A�@�μ@�P�U�N��������M�X�{�b$n�e���I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 500;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_FLAME1 ) );
      strcat(szEleType, "2-");

  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_flame_lv1 v0 + 1" );
	  
	if( (qmark = is_qmark( ch, "max_flame_lv1" ))!= NULL ){
		if( !(qmark->v2 & 1) ){
			do_mod_qmark( ch, "self max_flame_lv1 v2 | 1" );
			do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
			strcpy(count , "A");
		}else if( !(qmark->v2 & 2)){
			do_mod_qmark( ch, "self max_flame_lv1 v2 | 2" );
			do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
			strcpy(count , "B");
		}else if( !(qmark->v2 & 4)){
			do_mod_qmark( ch, "self max_flame_lv1 v2 | 4" );
			do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
			strcpy(count , "C");
		}
	}
  }
  else if( sklv > 4 )
  {
  	  if( (qmark = is_qmark( ch, "fire_phoenix_reborn" )) == NULL ) 
  	  {
  	      if( ch->mana < 1200)
  	      {
  	      	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	      	  ch->mana += 300;
  	      	  return;
  	      }
  	  }
  	  
	    if( (qmark = is_qmark( ch, "fire_phoenix_reborn" ))!= NULL ) 
		    act( "�I����$n���۴��A�U�N������ıq�a�W�����u���@�D�Ӱ_�I", ch, NULL, NULL, TO_ROOM );
	    else
		    act( "�b�Ť@�}���T�A�@�μ@�P�U�N������Ĭ�M�X�{�b$n�e���I", ch, NULL, NULL, TO_ROOM );
      
	    if( (qmark = is_qmark( ch, "fire_phoenix_reborn" )) == NULL ) 
		    ch->mana -= 1200;

      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_FLAME2 ) );
      strcat(szEleType, "3-");
      
      if( !IS_NPC(ch) )
          do_mod_qmark( ch, "self max_flame_lv2 v0 + 1" );
	  
	if( (qmark = is_qmark( ch, "max_flame_lv2" ))!= NULL ){
		if( !(qmark->v2 & 1) ){
			do_mod_qmark( ch, "self max_flame_lv2 v2 | 1" );
			do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
			strcpy(count , "A");
		}else if( !(qmark->v2 & 2)){
			do_mod_qmark( ch, "self max_flame_lv2 v2 | 2" );
			do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
			strcpy(count , "B");
		}else if( !(qmark->v2 & 4)){
			do_mod_qmark( ch, "self max_flame_lv2 v2 | 4" );
			do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
			strcpy(count , "C");
		}
	}
  }
  else
      return;
  
  if( !IS_NPC(ch) )
      do_mod_qmark( ch, "self max_flame_mobs v0 + 1" );
  
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);

  if( (qmark = is_qmark( ch, "fire_phoenix_reborn" ))!= NULL ) 
	  do_mpadd_qmark( victim, "self fire_phoenix_reborn 10 0 0 0 0 1" );

  do_mpadd_qmark( victim, "self sum_life_time 30 0 0 0 0 1" );
  
  if( ( qmark = is_qmark(victim, "sum_life_time")))
      qmark->timer += sklv * 3 + (sklv - 3) ;

	if( (qmark = is_qmark(victim, "sum_count" ))!= NULL ){
		qmark->v2 = 0;
		if( sklv >= 1) qmark->v2 += 1;
		if( sklv >= 3) qmark->v2 += 1;
		if( sklv >= 5) qmark->v2 += 1;
		if( sklv == 6) qmark->v2 += 1;
	}

  add_rember(victim, ch);
  add_follower( victim, ch );
  victim->gold = 0;
  victim->level    +=  ch->level + sklv;
  victim->max_mana +=  ch->max_mana + sklv * ch->max_mana/5;
  victim->mana      =  victim->max_mana;
  victim->pp       += (get_curr_int(ch) - 15 + ch->pp / 4) * 2;
  if(victim->pp < 0)   victim->pp = 0;
  if(ch->amdr > 0)     victim->amdr += ch->amdr * 0.8;
  if(sklv == 6)        victim->pp += 10;
  
  if( !ch->group )
      do_group(ch, "form");
  
  //strcat(szEleType, count);
  add_member(ch, victim);
  if(!IS_NPC(ch))AddOwnerNameToElemental(victim, ch, szEleType, count);
	return;
} 

//aspell_water_elemental added by jye 0940711
//remove sepc_fun by snarl 0940717
void spell_water_elemental( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;
  char *arg = (char*) vo;
  int sklv = get_skill_level(ch,sn);
  QMARK *qmark = NULL;

  char buf[MAX_INPUT_LENGTH];
	char szEleType[20];
  char count[20];
  strcpy(count, "");
	strcpy(szEleType, "w");
  
  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_DESERT
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl������۴��~�A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  if( !IS_NPC(ch) )
  {
	if (sklv < 6) {
  	  if( (qmark = is_qmark( ch, "max_earth_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W���a�������M���������۽ġA�L�k�۴������۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_flame_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W�����������M���������۽ġA�L�k�۴������۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
	}  	  
      if( (qmark = is_qmark( ch, "max_water_mobs" ))!= NULL ) 
      {
      	
          sprintf(buf, "self max_water_mobs v2 = %d", sklv);
          do_mod_qmark( ch, buf );
          
          if( sklv == 6 )
          {
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 1" );
          }                                          
          else if( sklv == 5 )                       
          {                                          
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 1" );
          }                                          
          else if( sklv == 4 )                       
          {                                          
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 0" );
          }                                          
          else if( sklv == 3 )                       
          {                                          
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 0" );
          }                                          
          else if( sklv == 2 )                       
          {                                          
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 0" );
          }                                          
      	  else if( sklv == 1 )                       
      	  {                                          
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 0" );
          }                                          
      	  else                                       
      	  {                                          
              do_mod_qmark( ch, "self max_water_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_water_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_water_lv2  v1 = 0" );
          }
          
          qmark = is_qmark( ch, "max_water_mobs" );
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char( "�A�L�k�A�l��X��h�������۴��~�I\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_water_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�������F�I\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 4 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_water_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�����F�I\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv >= 5 )
          {
              if( (qmark = is_qmark( ch, "max_water_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�����P�l�I\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
              
      }
      else 
      {
      	  //�gmax_water_mobs ��ppl���W
      	  //�gmax_water_lv qmark��ppl���W
          if( sklv == 6 )
          {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 6 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 1 0 0 0" );
          }
          else if( sklv == 5 )
          {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 5 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 1 0 0 0" );
          }
          else if( sklv == 4 )
          {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 4 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 3 )
          {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 3 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 2 )
          {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 2 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 0 0 0 0" );
          }
      	  else if( sklv == 1 )
      	  {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 1 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 0 0 0 0" );
          }
      	  else
      	  {
              do_add_qmark( ch, "self max_water_mobs -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_water_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_water_lv2 -1 0 0 0 0 0" );
          }



      }
  }
  
  do_say( ch, "�̷ӥj�Ѫ����w�A�Щp�̫e�ӫO�@�ڪ����ߡI" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "�I��$n���N�ӡA�@������F�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WATER0 ) );
      strcat(szEleType, "1-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_water_lv0 v0 + 1" );
	  
	if( (qmark = is_qmark( ch, "max_water_lv0" ))!= NULL ){
		if( !(qmark->v2 & 1) ){
			do_mod_qmark( ch, "self max_water_lv0 v0 | 1" );
			strcpy(count , "A");
		}else if( !(qmark->v2 & 2)){
			do_mod_qmark( ch, "self max_water_lv0 v0 | 2" );
			strcpy(count , "B");
		}else if( !(qmark->v2 & 4)){
			do_mod_qmark( ch, "self max_water_lv0 v0 | 4" );
			strcpy(count , "C");
		}
	}
  }
  else if( (sklv < 5 && arg[0] == '\0') || ( sklv > 2 && !str_cmp( arg , "2")) )
  {
  	  if( ch->mana < 750)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  
  	  act( "�I��$n���N�ӡA�@����F�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 750;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WATER1 ) );
      strcat(szEleType, "2-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_water_lv1 v0 + 1" );
	  
	if( (qmark = is_qmark( ch, "max_water_lv1" ))!= NULL ){
		if( !(qmark->v2 & 1) ){
			do_mod_qmark( ch, "self max_water_lv1 v0 | 1" );
			strcpy(count , "A");
		}else if( !(qmark->v2 & 2)){
			do_mod_qmark( ch, "self max_water_lv1 v0 | 2" );
			strcpy(count , "B");
		}else if( !(qmark->v2 & 4)){
			do_mod_qmark( ch, "self max_water_lv1 v0 | 4" );
			strcpy(count , "C");
		}
	}
  }
  else if( sklv > 4 )
  {
  	  if( ch->mana < 1600)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  
  	  act( "�I��$n���N�ӡA�@����P�l�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 1600;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WATER2 ) );
      strcat(szEleType, "3-");
      
      if( !IS_NPC(ch) )
          do_mod_qmark( ch, "self max_water_lv2 v0 + 1" );
	  
	if( (qmark = is_qmark( ch, "max_water_lv2" ))!= NULL ){
		if( !(qmark->v2 & 1) ){
			do_mod_qmark( ch, "self max_water_lv2 v2 | 1" );
			do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
			strcpy(count , "A");
		}else if( !(qmark->v2 & 2)){
			do_mod_qmark( ch, "self max_water_lv2 v2 | 2" );
			do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
			strcpy(count , "B");
		}else if( !(qmark->v2 & 4)){
			do_mod_qmark( ch, "self max_water_lv2 v2 | 4" );
			do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
			strcpy(count , "C");
		}
	}
  }
  else
      return;
      
  if( !IS_NPC(ch) )
      do_mod_qmark( ch, "self max_water_mobs v0 + 1" );
  
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self sum_life_time 30 0 0 0 0 1" );
  
  if( ( qmark = is_qmark(victim, "sum_life_time")))
      qmark->timer += sklv * 3 + (sklv - 3) ;
  
	if( (qmark = is_qmark(victim, "sum_count" ))!= NULL ){
		qmark->v2 = 0;
		if( sklv >= 1) qmark->v2 += 1;
		if( sklv >= 3) qmark->v2 += 1;
		if( sklv >= 5) qmark->v2 += 1;
		if( sklv == 6) qmark->v2 += 1;
	}
      
  add_rember(victim, ch);
  add_follower( victim, ch );
  victim->gold = 0;
  victim->level    +=  ch->level + sklv;
  victim->max_mana +=  ch->max_mana + sklv * ch->max_mana/40;
  victim->mana      =  victim->max_mana;
  victim->pp       +=  ch->pp / 2;
  //if(sklv == 6) victim->pp += 10;
  if( !ch->group )
      do_group(ch, "form");
  
  //strcat(szEleType, count);
  add_member(ch, victim);
	if(!IS_NPC(ch))AddOwnerNameToElemental(victim, ch, szEleType, count);
  
	return;
}

//aspell_earth_elemental added by jye 0940711
void spell_earth_elemental( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;
  char *arg = (char*) vo;
  int sklv = get_skill_level(ch,sn);
  QMARK *qmark = NULL;

  char buf[MAX_INPUT_LENGTH];
  char szEleType[20];
  char count[20];
  strcpy(count, "");
  strcpy(szEleType, "e");
	
  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 300;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl��g���۴��~�A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 300;
      return;
  }
  if( !IS_NPC(ch) )
  {
      if (sklv < 6) {
  	  if( (qmark = is_qmark( ch, "max_wind_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W�����������M�g�������۽ġA�L�k�۴��g���۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	if( (qmark = is_qmark( ch, "max_water_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W�����������M�g�������۽ġA�L�k�۴��g���۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
      }
      if( (qmark = is_qmark( ch, "max_earth_mobs" ))!= NULL ) 
      {
      	
          sprintf(buf, "self max_earth_mobs v2 = %d", sklv);
          do_mod_qmark( ch, buf );
          
          if( sklv == 6 )
          {
              do_mod_qmark( ch, "self max_earth_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 2" );
          }                                          
          else if( sklv == 5 )                       
          {                                          
              do_mod_qmark( ch, "self max_earth_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 1" );
          }                                          
          else if( sklv == 4 )                       
          {                                          
              do_mod_qmark( ch, "self max_earth_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 0" );
          }                                          
          else if( sklv == 3 )                       
          {                                          
              do_mod_qmark( ch, "self max_earth_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 0" );
          }                                          
          else if( sklv == 2 )                       
          {                                          
              do_mod_qmark( ch, "self max_earth_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 0" );
          }                                          
      	  else if( sklv == 1 )                       
      	  {                                          
              do_mod_qmark( ch, "self max_earth_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 0" );
          }                                          
      	  else                                       
      	  {                                          
              do_mod_qmark( ch, "self max_earth_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_earth_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_earth_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_earth_lv2  v1 = 0" );
          }
          
          qmark = is_qmark( ch, "max_earth_mobs" );
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char( "�A�L�k�A�l��X��h���g���۴��~�I\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_earth_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h���g���H�I\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 4 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_earth_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h���K�ۤ��I\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv >= 5 )
          {
              if( (qmark = is_qmark( ch, "max_earth_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�����K���H�I\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
      }
      else 
      {
      	  //�gmax_earth_mobs ��ppl���W
      	  //�gmax_earth_lv qmark��ppl���W
          if( sklv == 6 )
          {
              do_add_qmark( ch, "self max_earth_mobs -1 0 2 6 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 2 0 0 0" );
          }
          else if( sklv == 5 )
          {
              do_add_qmark( ch, "self max_earth_mobs -1 0 2 5 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 1 0 0 0" );
          }
          else if( sklv == 4 )
          {
              do_add_qmark( ch, "self max_earth_mobs -1 0 2 4 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 3 )
          {
              do_add_qmark( ch, "self max_earth_mobs -1 0 2 3 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 2 )
          {
              do_add_qmark( ch, "self max_earth_mobs -1 0 2 2 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 0 0 0 0" );
          }
      	  else if( sklv == 1 )
      	  {
              do_add_qmark( ch, "self max_earth_mobs -1 0 1 1 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 0 0 0 0" );
          }
          else
      	  {
              do_add_qmark( ch, "self max_earth_mobs -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_earth_lv2 -1 0 0 0 0 0" );
          }
      }
  }
  
  do_say( ch, "��`�ڪ��ϩR�A�کR�O�A�ۤg�a���͡I" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "�ھ� $n ���R�O�A�@�L�g���H�q�d�g�̪��X�ӡI", ch, NULL, NULL, TO_ROOM );
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH0 ) );
      strcat(szEleType, "1-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_earth_lv0 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_earth_lv0" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_earth_lv0 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_earth_lv0 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_earth_lv0 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else if( (sklv < 5 && arg[0] == '\0') || ( sklv > 2 && !str_cmp( arg , "2")) )
  {
  	
  	  if( ch->mana < 350)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 300;
  	  	  return;
  	  }
  	  
  	  act( "�ھ� $n ���R�O�A�@�L�K�ۤ��q�d�g�̪��X�ӡI", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 350;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH1 ) );
      strcat(szEleType, "2-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_earth_lv1 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_earth_lv1" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_earth_lv1 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_earth_lv1 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_earth_lv1 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else if( sklv > 4 )
  {
  	  if( ch->mana < 900)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 300;
  	  	  return;
  	  }
  	  
  	  act( "�ھ� $n ���R�O�A�@�L���K���H�q�d�g�̪��X�ӡI", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 900;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH2 ) );
      strcat(szEleType, "3-");
      if(sklv == 6) victim->pdr      +=  get_curr_int(ch) * 2;
  	        
      if( !IS_NPC(ch) )
          do_mod_qmark( ch, "self max_earth_lv2 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_earth_lv2" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_earth_lv2 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_earth_lv2 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_earth_lv2 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else
      return;
  
  if( !IS_NPC(ch) )
      do_mod_qmark( ch, "self max_earth_mobs v0 + 1" );
  
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self sum_life_time 30 0 0 0 0 1" );
  
  if( ( qmark = is_qmark(victim, "sum_life_time")))
      qmark->timer += sklv * 3 + (sklv - 3) ;
  
	if( (qmark = is_qmark(victim, "sum_count" ))!= NULL ){
		qmark->v2 = 0;
		if( sklv >= 1) qmark->v2 += 1;
		if( sklv >= 3) qmark->v2 += 1;
		if( sklv >= 5) qmark->v2 += 1;
		if( sklv == 6) qmark->v2 += 1;
	}
      
  add_rember(victim, ch);
  add_follower( victim, ch );
  victim->gold = 0;
  victim->level    +=  ch->level + sklv;
  victim->max_mana +=  ch->max_mana/3;
  victim->max_hit  +=  ch->max_mana * (1 + sklv)/24;
  victim->mana      =  victim->max_mana;
  victim->hit       =  victim->max_hit;
  victim->pdr      +=  get_curr_int(ch) * (1 + sklv)/4 + ch->pp * (1 + sklv)/7;
  if(ch->amdr > 0)     victim->mdr  += ch->amdr *(1 + sklv)/28;
  if(sklv == 6)        victim->pdr  += 25;
  
  if( !ch->group )
      do_group(ch, "form");
  
  //strcat(szEleType, count);
  add_member(ch, victim);
  add_protector(victim, ch);
  if(!IS_NPC(ch))AddOwnerNameToElemental(victim, ch, szEleType, count);
	return;
}

//aspell_wind_elemental added by jye 0940711
//remove sepc_fun by snarl 0940717
void spell_wind_elemental( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;
  char *arg = (char*) vo;
  int sklv = get_skill_level(ch,sn);
  QMARK *qmark = NULL;

  char buf[MAX_INPUT_LENGTH];
  char szEleType[20];
  char count[20];
  strcpy(count, "");
  strcpy(szEleType, "i");

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 200;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl�ꭷ���۴��~�A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 200;
      return;
  }
  if( !IS_NPC(ch) )
  {
	if (sklv < 6) {
  	  if( (qmark = is_qmark( ch, "max_lightning_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W���p�������M���������۽ġA�L�k�۴������۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_earth_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W���g�������M���������۽ġA�L�k�۴������۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
	}  	  
      if( (qmark = is_qmark( ch, "max_wind_mobs" ))!= NULL ) 
      {
          sprintf(buf, "self max_wind_mobs v2 = %d", sklv);
          do_mod_qmark( ch, buf );
          
          if( sklv == 6 )
          {
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 2" );
          }                                          
          else if( sklv == 5 )                       
          {                                          
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 0" );
          }                                          
          else if( sklv == 4 )                       
          {                                          
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 0" );
          }                                          
          else if( sklv == 3 )                       
          {                                          
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 0" );
          }                                          
          else if( sklv == 2 )                       
          {                                          
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 0" );
          }                                          
      	  else if( sklv == 1 )                       
      	  {                                          
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 0" );
          }                                          
      	  else                                       
      	  {                                          
              do_mod_qmark( ch, "self max_wind_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_wind_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_wind_lv2  v1 = 0" );
          }
          
          qmark = is_qmark( ch, "max_wind_mobs" );
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char( "�A�L�k�A�l��X��h�������۴��~�I\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_wind_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h���������I\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 5 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_wind_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h������I\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv == 6 )
          {
              if( (qmark = is_qmark( ch, "max_wind_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�����l�~�I\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
      }
      else 
      {
      	  //�gmax_wind_mobs ��ppl���W
      	  //�gmax_wind_lv qmark��ppl���W
          if( sklv == 6 )
          {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 6 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 2 0 0 0" );
          }
          else if( sklv == 5 )
          {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 5 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 4 )
          {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 4 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 3 )
          {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 3 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 0 0 0 0" );
          }
      	  else if( sklv == 2 )
      	  {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 2 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 0 0 0 0" );
          }
      	  else if( sklv == 1 )
      	  {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 1 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 0 0 0 0" );
          }
      	  else
      	  {
              do_add_qmark( ch, "self max_wind_mobs -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_wind_lv2 -1 0 0 0 0 0" );
          }


      }
  }
  
  do_say( ch, "�̷ӥj�Ѫ����w�A�Щp�̫e�ӫO�@�ڪ����ߡI" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "�I��$n���N�ӡA�@�쭷�����q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WIND0 ) );
      strcat(szEleType, "1-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_wind_lv0 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_wind_lv0" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_wind_lv0 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_wind_lv0 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_wind_lv0 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else if( (sklv < 6 && arg[0] == '\0') || ( sklv > 2 && !str_cmp( arg , "2")) )
  {
  	  if( ch->mana < 350)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 200;
  	  	  return;
  	  }
  	  
  	  act( "�I��$n���N�ӡA�@�쭷��q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 350;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WIND1 ) );
      strcat(szEleType, "2-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_wind_lv1 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_wind_lv1" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_wind_lv1 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_wind_lv1 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_wind_lv1 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else if( sklv == 6 )
  {
  	  if( ch->mana < 900)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 200;
  	  	  return;
  	  }
  	  
  	  act( "�I��$n���N�ӡA�@����l�~�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 900;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WIND2 ) );
      strcat(szEleType, "3-");
      
      if( !IS_NPC(ch) )
          do_mod_qmark( ch, "self max_wind_lv2 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_wind_lv2" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_wind_lv2 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_wind_lv2 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_wind_lv2 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else
      return;
  
  if( !IS_NPC(ch) )
      do_mod_qmark( ch, "self max_wind_mobs v0 + 1" );
  
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self sum_life_time 30 0 0 0 0 1" );
  
  if( ( qmark = is_qmark(victim, "sum_life_time")))
      qmark->timer += sklv * 3 + (sklv - 3) ;
  
	if( (qmark = is_qmark(victim, "sum_count" ))!= NULL ){
		qmark->v2 = 0;
		if( sklv >= 1) qmark->v2 += 1;
		if( sklv >= 3) qmark->v2 += 1;
		if( sklv >= 5) qmark->v2 += 1;
		if( sklv == 6) qmark->v2 += 1;
	}
      
  add_rember(victim, ch);
  add_follower( victim, ch );
  victim->gold = 0;
  victim->level    +=  ch->level + sklv;
  victim->max_mana +=  ch->max_mana + sklv * ch->max_mana/10;
  victim->mana      =  victim->max_mana;
  victim->pp       += (get_curr_int(ch) - 22 + ch->pp / 5);
  victim->amdr     +=  ch->pp * 3;
  if(victim->pp < 0)   victim->pp = 0;
  if(ch->amdr > 0)     victim->amdr += ch->amdr/2;
  if(sklv == 6)        victim->pp += 5;

  if( !ch->group )
      do_group(ch, "form");
  
  //strcat(szEleType, count);
  add_member(ch, victim);
  if(!IS_NPC(ch))AddOwnerNameToElemental(victim, ch, szEleType, count); 
	return;
}

//aspell_lightning_elemental added by jye 0940711
//remove sepc_fun by snarl 0940717
void spell_lightning_elemental( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;
  char *arg = (char*) vo;
  int sklv = get_skill_level(ch,sn);
  QMARK *qmark = NULL;

  char buf[MAX_INPUT_LENGTH];
  char szEleType[20];
  char count[20];
  strcpy(count, "");
  strcpy(szEleType, "l");
  
  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl��p���۴��~�A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  if( !IS_NPC(ch) )
  {
	if (sklv < 6) {
  	  if( (qmark = is_qmark( ch, "max_flame_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W�����������M�p�������۽ġA�L�k�۴��p���۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_wind_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "�A���W�����������M�p�������۽ġA�L�k�۴��p���۴��~�I\n\r", ch );
  	  	      return;
  	      }
  	  }
	}  	  
  	  
      if( (qmark = is_qmark( ch, "max_lightning_mobs" ))!= NULL ) 
      {
          sprintf(buf, "self max_lightning_mobs v2 = %d", sklv);
          do_mod_qmark( ch, buf );
          
          if( sklv == 6 )
          {
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 1" );
          }                                          
          else if( sklv == 5 )                       
          {                                          
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 0" );
          }                                          
          else if( sklv == 4 )                       
          {                                          
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 2" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 0" );
          }                                          
          else if( sklv == 3 )                       
          {                                          
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 3" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 1" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 0" );
          }                                          
          else if( sklv == 2 )                       
          {                                          
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 2" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 0" );
          }                                          
      	  else if( sklv == 1 )                       
      	  {                                          
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 2" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 0" );
          }                                          
      	  else                                       
      	  {                                          
              do_mod_qmark( ch, "self max_lightning_mobs v1 = 1" );
              do_mod_qmark( ch, "self max_lightning_lv0  v1 = 1" );
              do_mod_qmark( ch, "self max_lightning_lv1  v1 = 0" );
              do_mod_qmark( ch, "self max_lightning_lv2  v1 = 0" );
          }
          
          qmark = is_qmark( ch, "max_lightning_mobs" );
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char( "�A�L�k�A�l��X��h���p���۴��~�I\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_lightning_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h���p�����I\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 5 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_lightning_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h���p�~�I\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv == 6 )
          {
              if( (qmark = is_qmark( ch, "max_lightning_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "�A�L�k�A�l��X��h�����Z�I\n\r", ch );
                  	  return;
                  }
          }
          else
              return;

      }
      else 
      {
      	  //�gmax_lightning_mobs ��ppl���W
      	  //�gmax_lightning_lv qmark��ppl���W
          if( sklv == 6 )
          {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 3 6 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 1 0 0 0" );
          }
          else if( sklv == 5 )
          {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 3 5 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 4 )
          {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 3 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 3 )
          {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 3 3 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 3 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 0 0 0 0" );
          }
          else if( sklv == 2 )
          {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 2 2 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 2 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 0 0 0 0" );
          }
      	  else if( sklv == 1 )
      	  {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 2 1 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 0 0 0 0" );
          }
      	  else
      	  {
              do_add_qmark( ch, "self max_lightning_mobs -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv0 -1 0 1 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv1 -1 0 0 0 0 0" );
              do_add_qmark( ch, "self max_lightning_lv2 -1 0 0 0 0 0" );
          }
      }
  }
  
  do_say( ch, "�̷ӥj�Ѫ����w�A�Щp�̫e�ӫO�@�ڪ����ߡI" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "�I��$n���N�ӡA�@��p�����q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_LIGHTNING0 ) );
      strcat(szEleType, "1-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_lightning_lv0 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_lightning_lv0" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_lightning_lv0 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_lightning_lv0 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_lightning_lv0 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else if( (sklv < 6 && arg[0] == '\0') || ( sklv > 2 && !str_cmp( arg , "2")) )
  {
  	  if( ch->mana < 300)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  act( "�I��$n���N�ӡA�@��p�~�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 300;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_LIGHTNING1 ) );
      strcat(szEleType, "2-");
  	  
  	  if( !IS_NPC(ch) )
  	      do_mod_qmark( ch, "self max_lightning_lv1 v0 + 1" );

		if( (qmark = is_qmark( ch, "max_lightning_lv1" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_lightning_lv1 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_lightning_lv1 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_lightning_lv1 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else if( sklv == 6 )
  {
  	  if( ch->mana < 1100)
  	  {
  	  	  send_to_char( "�p���]�O�����H���}���۳��~�q�L���q�D�f�I\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  
  	  act( "�I��$n���N�ӡA�@����Z�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );
  	  ch->mana -= 1100;
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_LIGHTNING2 ) );
      victim->apdr     +=  get_curr_int(ch) * 2;
      strcat(szEleType, "3-");
      
      if( !IS_NPC(ch) )
          do_mod_qmark( ch, "self max_lightning_lv2 v0 + 1" );
	  
		if( (qmark = is_qmark( ch, "max_lightning_lv2" ))!= NULL ){
			if( !(qmark->v2 & 1) ){
				do_mod_qmark( ch, "self max_lightning_lv2 v2 | 1" );
				do_mpadd_qmark( victim, "self sum_count -1 1 0 0 0 1" );
				strcpy(count , "A");
			}else if( !(qmark->v2 & 2)){
				do_mod_qmark( ch, "self max_lightning_lv2 v2 | 2" );
				do_mpadd_qmark( victim, "self sum_count -1 2 0 0 0 1" );
				strcpy(count , "B");
			}else if( !(qmark->v2 & 4)){
				do_mod_qmark( ch, "self max_lightning_lv2 v2 | 4" );
				do_mpadd_qmark( victim, "self sum_count -1 4 0 0 0 1" );
				strcpy(count , "C");
			}
		}
  }
  else
      return;
  
  if( !IS_NPC(ch) )
      do_mod_qmark( ch, "self max_lightning_mobs v0 + 1" );
  
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self sum_life_time 30 0 0 0 0 1" );
  
  if( ( qmark = is_qmark(victim, "sum_life_time")))
      qmark->timer += sklv * 3 + (sklv - 3) ;
  
	if( (qmark = is_qmark(victim, "sum_count" ))!= NULL ){
		qmark->v2 = 0;
		if( sklv >= 1) qmark->v2 += 1;
		if( sklv >= 3) qmark->v2 += 1;
		if( sklv >= 5) qmark->v2 += 1;
		if( sklv == 6) qmark->v2 += 1;
	}
      
  add_rember(victim, ch);
  add_follower( victim, ch );
  victim->gold = 0;
  victim->level    +=  ch->level + sklv;
  victim->max_mana +=  ch->max_mana + sklv * ch->max_mana/48;
  victim->mana      =  victim->max_mana;
  if(ch->pp > 0)       victim->apdr += ch->pp;
  if(ch->amdr > 0)     victim->apdr += ch->amdr / 5;
  
  if( !ch->group )
      do_group(ch, "form");

  //strcat(szEleType, count);
  add_member(ch, victim);
  if(!IS_NPC(ch))AddOwnerNameToElemental(victim, ch, szEleType, count);
	return;
}

/* Clay golem spell added by Amenda 2/25/01 */
void spell_clay_golem( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  QMARK *qm;
  int sklv = get_skill_level(ch,sn);
  int        npccount  = 0;
  int        pccount   = 0;
  
  if( (!IS_NPC( ch ) && !IS_IMMORTAL( ch ))
      || !strcmp( ch->in_room->area->identity, "school" )
      || !strcmp( ch->in_room->area->identity, "skarabrae" )
      || IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "�@�ѯ��t�O�������F�A�����ϡI\n\r", ch );
      return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl��g���H�A���ȥu���j�����o��C\n\r", ch );
      
      if( !IS_NPC(ch) )
           ch->mana += 250;
      return;
  }
  
  for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
  {
  	
      if( IS_NPC( gch ) && !IS_AFFECTED( gch, AFF_CHARM ) )
          npccount++;
      if( !IS_NPC( gch ) )
          pccount++;
  }

  if( !IS_NPC( ch ) && npccount > pccount + sklv )
  {
      do_say( ch, "�o�̦��Ӧh�l��Ӫ��ͪ��A�@�w�n�˨��@�ǡI" );
      if( !IS_NPC(ch) )
          ch->mana += 50;
      return;
  }
  
  do_say( ch, "��`�ڪ��ϩR�A�کR�O�A�ۤg�a���͡I" );
  act( "�ھ� $n ���R�O�A�@�L�g���H�q�d�g�̪��X�ӡI", ch, NULL, NULL, TO_ROOM );
  
  if( level < 20 )
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH0 ) );
  else if( level < 30 )
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH0 ) );
  else if( sklv < 4 )
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH1 ) );
  else
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_EARTH2 ) );
      
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self golem_life_time 35 0 0 0 0 1" );
  
  if( ( qm = is_qmark(victim, "golem_life_time")))
      qm->timer += sklv * 3 + (sklv - 3) * 2;
      
  add_follower( victim, ch );
  add_rember( victim, ch);
  victim->rembch = ch;
  if( !ch->group ) 
      do_group(ch, "form");

  //victim->leader = ch;
  add_member(ch, victim);
  victim->gold = 0;
  if( sklv > 2 )
      victim->max_hit += sklv * 300;
  else if( sklv > 4 ) 
      victim->max_hit += sklv * 300;

  if( sklv > 5 ) 
      victim->armor -= sklv * 30;
    
  victim->hit = victim->max_hit;
  /*
  victim->next_in_protect = ch->protector;
  ch->protector = victim;
  victim->protecting = ch;
  */
  add_protector(victim, ch);
  return;
}

/* Water spirit spell added by Amenda 3/31/01 */
void spell_water_spirit( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  int        npccount  = 0;
  int        pccount   = 0;
  int sklv = get_skill_level(ch,sn);
  QMARK   *qm;

   if( (!IS_NPC( ch ) && !IS_IMMORTAL( ch ))
      || !strcmp( ch->in_room->area->identity, "school" )
      || !strcmp( ch->in_room->area->identity, "skarabrae" )
      || IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "�@�ѯ��t�O�������F�A�����ϡI\n\r", ch );
      return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 200;
      return;
  }

  if( ch->in_room->sector_type == SECT_DESERT
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl������F�A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 200;
      return;
  }
  
  for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
  {
       if( IS_NPC( gch ) && !IS_AFFECTED( gch, AFF_CHARM ) )
           npccount++;
       if( !IS_NPC( gch ) )
           pccount++;
  }

  if( !IS_NPC( ch ) && npccount > pccount + sklv)
  {
      do_say( ch, "�o�̦��Ӧh�l��Ӫ��ͪ��A�@�w�n�˨��@�ǡI" );
      return;
  }

  do_say( ch, "�̷ӥj�Ѫ����w�A�Щp�̫e�ӫO�@�ڪ����ߡI" );
  act( "�I��$n���N�ӡA�@������F�q���@�ɹL�����U$e�I", ch, NULL, NULL, TO_ROOM );

  if( level < 30 )
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WATER0 ) );
  else if ( sklv < 4 )
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WATER1 ) );
  else      
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_WATER2 ) );
      
  if( sklv > 2 )
      victim->max_hit += sklv * 200;
  else if( sklv > 4 ) 
      victim->max_hit += sklv * 100;
  
  if( sklv > 5 )
      victim->dodge -= sklv * 10;

  victim->hit = victim->max_hit;
  SET_BIT( victim->affected_by, AFF_SUMMONED );
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self water_life_time 30 0 0 0 0 1" );
  
  if( ( qm = is_qmark(victim, "water_life_time")))
      qm->timer += sklv * 3 + (sklv - 3) ;
      
  victim->spec_fun = spec_cast_water;
  add_rember(victim, ch);
  add_follower( victim, ch );
  victim->gold = 0;
  
  if( !ch->group )
      do_group(ch, "form");

  //victim->leader = ch;
  add_member(ch, victim);
  return;
}

/* Fire element spell added by Amenda 4/1/01 */
void spell_fire_elemental( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  int        npccount  = 0;
  int        pccount   = 0;
  char    buf[50];
  int sklv = get_skill_level(ch,sn);
  QMARK   *qmark;
  QMARK   *qm;

  if( (!IS_NPC( ch ) && !IS_IMMORTAL( ch ))
      || !strcmp( ch->in_room->area->identity, "school" )
      || !strcmp( ch->in_room->area->identity, "skarabrae" )
      || IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "�@�ѯ��t�O�������F�A�����ϡI\n\r", ch );
      return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "���t���a��A�����\\�����I�J�I\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }

  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "�n�b�o�ئa�Υl��������A���ȥu���j�����o��C\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }

  for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
  {
       if( IS_NPC( gch ) && !IS_AFFECTED( gch, AFF_CHARM ) )
           npccount++;
       if( !IS_NPC( gch ) )
           pccount++;
  }
    
  if( !IS_NPC( ch ) && npccount > pccount + sklv )
  {
      do_say( ch, "�o�̦��Ӧh�l��Ӫ��ͪ��A�@�w�n�˨��@�ǡI" );
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }

  for( qmark = ch->qmark; qmark; qmark = qmark->next )
       if( !str_cmp( qmark->mark_name, "spell_fire_elemental" ) )
           break;
           
  if( qmark )
  {
      if( qmark->v0 > ( 4+sklv/2 + UMAX( 0, (ch->level - 36)*2  ) ) || qmark->v0 < 0 )
      {
          send_to_char( "�A���믫�O�L�k�����h�������C\n\r", ch );
          ch->mana += 100;
          return;
      }
      qmark->v0 ++;
  }
  else
  {
      one_argument( ch->name, buf );
      strcat( buf, " spell_fire_elemental -1 1 0 0 0 0" );
      do_add_qmark( ch, buf );
  }

  do_say( ch, "���@�Nñ�q�媺�����A�洫�A�j�j�������O�q�I" );
  act( "�b�Ť@�}���T�A�@�μ@�P�U�N�����K��M�X�{�b$n�e���I", ch, NULL, NULL, TO_ROOM );

  if( level < 30 )
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_FLAME1 ) );
  else
      victim = create_mobile( get_mob_index( MOB_VNUM_SOR_FLAME2 ) );

  SET_BIT( victim->affected_by, AFF_SUMMONED );
  victim->level += sklv;
  char_to_room( victim, ch->in_room );
  mprog_repop_trigger(victim);
  
  do_mpadd_qmark( victim, "self fire_life_time 20 0 0 0 0 1" );
  
  if( ( qm = is_qmark(victim, "fire_life_time")))
      qm->timer += sklv * 2 + (sklv - 3) ;
  
  victim->spec_fun = spec_cast_fire;
  add_rember(victim, ch);
  add_follower( victim, ch );
  
  if( sklv > 3 )
  {
      victim->max_hit += sklv * 300;
      victim->hit = victim->max_hit;
  }
  
  if( sklv > 4 )
      victim->hitroll += sklv*2;

  if( sklv > 5 ) 
      victim->damroll += sklv * 2 + sklv / 2;

  if( !ch->group )
       do_group(ch, "form");
       
    //victim->leader = ch;
  add_member(ch, victim);
  victim->pp += ( get_curr_int( ch ) - 10 + ch->level - 35 ) * 2 + sklv * 3;
  victim->gold = 0;
  return;
}


void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *gch;
  int        npccount  = 0;
  int        pccount   = 0;
  CHAR_DATA * vam = NULL;
  MOB_INDEX_DATA *pMobIndex;
  QMARK *qmark = NULL;              //Added by Razgriz  20050701
  int sklv = IS_IMMORTAL(ch) ? 6 : get_skill_level(ch, sn);
  
	//20060403 modify by jye
  if( (!IS_NPC( ch ) && !IS_IMMORTAL( ch ))
	  || IS_AFFECTED( ch, AFF_SUMMONED )
	  || IS_AFFECTED( ch, AFF_CHARM )
      || !strcmp( ch->in_room->area->identity, "school" )
      || !strcmp( ch->in_room->area->identity, "skarabrae" )
      || IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "�@�ѯ��t�O�������F�A�����ϡI\n\r", ch );
      return;
  }

  for( gch = ch->in_room->people; gch; gch = gch->next_in_room )
  {
      if( IS_NPC( gch ) && !IS_AFFECTED( gch, AFF_CHARM ) )
          npccount++;
      if( !IS_NPC( gch ) 
          || ( IS_NPC( gch ) && IS_AFFECTED( gch, AFF_CHARM ) ) )
          pccount++;
  }

  if( !IS_NPC( ch ) && npccount > pccount + sklv )
  {
      do_say( ch, "�o�̤w�g���Ӧh���l��Ӫ��ͪ��F�I" );
      return;
  }

  //Added by Razgriz    20050701
  if( IS_NPC( ch ) )
  {
      if( ( qmark = is_qmark ( ch, "max_gate_mobs" )  ) != NULL ) 
      {
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char ( "�A�L�k�A�l��X��h���ͪ��I\n\r", ch );
              return;
          }
          else 
          {
              do_mpmod_qmark ( ch, "self max_gate_mobs v0 + 1" );
          }
      }
      else 
      {
          do_mpadd_qmark ( ch, "self max_gate_mobs -1 1 5 0 0 1" );
      }
  }

  if( level < 20 
      || ( IS_NPC( ch ) && !IS_SET(ch->special, EXPERT)&& number_percent() > ch->level - 10)
      || ( ( sklv < 5 ) && number_percent() > level * 2 ) )
  {
      vam = create_mobile(get_mob_index(MOB_VNUM_VAMPIRE));
      do_say( ch, "�ӧa�A�S�̡I���ڭ̤@�P�ɨ��o����{�����b�I" );
      char_to_room(  vam, ch->in_room );
      mprog_repop_trigger(vam);

      REMOVE_BIT(vam->act, ACT_AGGRESSIVE);
      REMOVE_BIT(vam->act, ACT_SCAVENGER);
      SET_BIT(vam->act, ACT_SENTINEL);
      
      if( vam->spec_fun == spec_lookup( "spec_thief"     ) 
          || vam->spec_fun == spec_lookup( "spec_janitor"      ) )
          vam->spec_fun = 0;

      if( IS_EVIL(ch) ) 
      {
          do_mpadd_qmark( vam, "self gated_mob -1 0 0 0 0 1" );   //Added by Razgriz  20050701
          vam->gater = ch;              //Added by Razgriz  20050701
          add_follower(vam,ch);
          vam->master = ch;
          if( !ch->group ) 
              do_group(ch, "form");

          //victim->leader = ch;
          add_member(ch, vam);
          act( "[36m$N�[�J�A������C[m", ch, NULL, vam, TO_CHAR    );
          act( "[36m$N�[�J$n������C[m", ch, NULL, vam, TO_NOTVICT );
          add_protector(vam, ch);
      }
      else 
      {
          if( IS_NPC( ch ) )           //Added by Razgriz  20050701
              do_mpmod_qmark ( ch, "self max_gate_mobs v0 - 1" ); //Added by Razgriz  20050701
              
          set_fighting(ch, vam);
      }
      vam->gold = 0;
  }
  else
  {
      for(;;)
      {
          pMobIndex = get_mob_index( number_range( 0, MAX_VNUM  ) );
          if( pMobIndex 
              && pMobIndex->count > 1 
              && !pMobIndex->mobprogs
              && IS_SET( pMobIndex->act, ACT_AGGRESSIVE ) )
          {
            if( pMobIndex->level > level - 21 + sklv * 2 
                && pMobIndex->level < level + sklv * 2 )
            {
                if( sklv != 6 )
                    break;
                else if ( IS_SET( pMobIndex->special, EXPERT ) )
                    break;
            }
          }
      }
      
      vam = create_mobile( pMobIndex );
      do_say( ch, "�ӧa�A�S�̡I���ڭ̤@�P�ɨ��o����{�����b�I" );
      char_to_room(  vam, ch->in_room );
      do_mpadd_qmark( vam, "self gated_mob -1 0 0 0 0 1" );
      vam->gater = ch;              //Added by Razgriz  20050701
      REMOVE_BIT(vam->act, ACT_AGGRESSIVE);
      REMOVE_BIT(vam->act, ACT_SCAVENGER);
      SET_BIT(vam->act, ACT_SENTINEL);
      
      if( vam->spec_fun == spec_lookup( "spec_thief"     ) 
          || vam->spec_fun == spec_lookup( "spec_janitor"      ) )
          vam->spec_fun = 0;
      
      ch->mana = UMAX( 0, ch->mana - vam->level*2 );
      
      add_follower(vam,ch);
      vam->master = ch;
      if( !ch->group )
          do_group(ch, "form");

      //victim->leader = ch;
      add_member(ch, vam);
      act( "[36m$N�[�J�A������C[m", ch, NULL, vam, TO_CHAR    );
      act( "[36m$N�[�J$n������C[m", ch, NULL, vam, TO_NOTVICT );
      add_protector(vam, ch);
      vam->gold = 0;
  }
  return;
}

