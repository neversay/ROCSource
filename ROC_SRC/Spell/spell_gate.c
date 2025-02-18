//*****************************************************************
//*  PROGRAM ID  : spell_gate.c                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940701 Razgriz                                *
//*  PURPOSE     :         修改 spell_gate() 在施法者為 npc 時，  *
//*              :         加入 qmark 檢查，假如超過召喚上限，就  *
//*              :         不予召喚                               *
//*              :         假如該 npc 事先無此 qmark，將會自動    *
//*              :         幫他加上，並把召喚上限設為 5           *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940710 jye                                    *
//*  PURPOSE     :         修改spell_gate ppl不能使用             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940711 jye                                    *
//*  PURPOSE     :         新增spell_flame_elemental              *
//*              :             spell_water_elemental              *
//*              :             spell_earth_elemental              *
//*              :             spell_wind_elemental               *
//*              :             spell_lightning_elemental          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950403 jye                                    *
//*  PURPOSE     :         修改spell_gate被charm or summoned 的mob*
//*              :         不能使用                               *
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
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 300;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚火之招換獸，恐怕只有大神做得到。\n\r", ch );
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
  	  	  	  send_to_char( "你身上的水之契約和火之契約相衝，無法招換火之招換獸！\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_lightning_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "你身上的雷之契約和火之契約相衝，無法招換火之招換獸！\n\r", ch );
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
              send_to_char( "你無法再召喚出更多的火之招換獸！\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_flame_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的火元素！\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 4 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_flame_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的火妖！\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv >= 5 )
          {
              if( (qmark = is_qmark( ch, "max_flame_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的火鳳凰！\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
              
      }
      else 
      {
      	  //寫max_flame_mobs 至ppl身上
      	  //max_flame_lv qmark至ppl身上
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

  do_say( ch, "我願意簽訂血的盟約，交換你強大的火炎力量！" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
      act( "半空一陣巨響，一團劇烈燃燒的火元素突然出現在$n前面！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 300;
  	  	  return;
  	  }
  	  
      act( "半空一陣巨響，一團劇烈燃燒的火妖突然出現在$n前面！", ch, NULL, NULL, TO_ROOM );
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
  	      	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	      	  ch->mana += 300;
  	      	  return;
  	      }
  	  }
  	  
	    if( (qmark = is_qmark( ch, "fire_phoenix_reborn" ))!= NULL ) 
		    act( "呼應著$n的招換，燃燒的火鳳凰從地上的灰燼中一躍而起！", ch, NULL, NULL, TO_ROOM );
	    else
		    act( "半空一陣巨響，一團劇烈燃燒的火鳳凰突然出現在$n前面！", ch, NULL, NULL, TO_ROOM );
      
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
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_DESERT
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚水之招換獸，恐怕只有大神做得到。\n\r", ch );
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
  	  	  	  send_to_char( "你身上的地之契約和水之契約相衝，無法招換水之招換獸！\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_flame_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "你身上的火之契約和水之契約相衝，無法招換水之招換獸！\n\r", ch );
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
              send_to_char( "你無法再召喚出更多的水之招換獸！\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_water_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的水精靈！\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 4 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_water_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的水靈！\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv >= 5 )
          {
              if( (qmark = is_qmark( ch, "max_water_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的水仙子！\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
              
      }
      else 
      {
      	  //寫max_water_mobs 至ppl身上
      	  //寫max_water_lv qmark至ppl身上
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
  
  do_say( ch, "依照古老的約定，請妳們前來保護我的身軀！" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "呼應$n的意志，一位水精靈從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  
  	  act( "呼應$n的意志，一位水靈從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  
  	  act( "呼應$n的意志，一位水仙子從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 300;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚土之招換獸，恐怕只有大神做得到。\n\r", ch );
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
  	  	  	  send_to_char( "你身上的風之契約和土之契約相衝，無法招換土之招換獸！\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	if( (qmark = is_qmark( ch, "max_water_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "你身上的水之契約和土之契約相衝，無法招換土之招換獸！\n\r", ch );
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
              send_to_char( "你無法再召喚出更多的土之招換獸！\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_earth_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的土巨人！\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 4 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_earth_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的鐵石牛！\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv >= 5 )
          {
              if( (qmark = is_qmark( ch, "max_earth_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的鋼鐵巨人！\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
      }
      else 
      {
      	  //寫max_earth_mobs 至ppl身上
      	  //寫max_earth_lv qmark至ppl身上
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
  
  do_say( ch, "遵循我的使命，我命令你自土地重生！" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "根據 $n 的命令，一尊土巨人從泥土裡爬出來！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 300;
  	  	  return;
  	  }
  	  
  	  act( "根據 $n 的命令，一尊鐵石牛從泥土裡爬出來！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 300;
  	  	  return;
  	  }
  	  
  	  act( "根據 $n 的命令，一尊鋼鐵巨人從泥土裡爬出來！", ch, NULL, NULL, TO_ROOM );
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
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 200;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚風之招換獸，恐怕只有大神做得到。\n\r", ch );
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
  	  	  	  send_to_char( "你身上的雷之契約和風之契約相衝，無法招換風之招換獸！\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_earth_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "你身上的土之契約和風之契約相衝，無法招換風之招換獸！\n\r", ch );
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
              send_to_char( "你無法再召喚出更多的風之招換獸！\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_wind_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的風元素！\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 5 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_wind_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的風精！\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv == 6 )
          {
              if( (qmark = is_qmark( ch, "max_wind_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的獅鷲獸！\n\r", ch );
                  	  return;
                  }
          }
          else
              return;
      }
      else 
      {
      	  //寫max_wind_mobs 至ppl身上
      	  //寫max_wind_lv qmark至ppl身上
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
  
  do_say( ch, "依照古老的約定，請妳們前來保護我的身軀！" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "呼應$n的意志，一位風元素從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 200;
  	  	  return;
  	  }
  	  
  	  act( "呼應$n的意志，一位風精從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 200;
  	  	  return;
  	  }
  	  
  	  act( "呼應$n的意志，一位獅鷲獸從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚雷之招換獸，恐怕只有大神做得到。\n\r", ch );
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
  	  	  	  send_to_char( "你身上的火之契約和雷之契約相衝，無法招換雷之招換獸！\n\r", ch );
  	  	      return;
  	      }
  	  }
  	  
    	  if( (qmark = is_qmark( ch, "max_wind_mobs" ))!= NULL ) 
  	  {
  	  	  if( qmark->v0 > 0)
  	  	  {
  	  	  	  send_to_char( "你身上的風之契約和雷之契約相衝，無法招換雷之招換獸！\n\r", ch );
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
              send_to_char( "你無法再召喚出更多的雷之招換獸！\n\r", ch );
              return;
          }
          
          if( sklv <= 2 || !str_cmp(arg, "1" ) )
          {
              if( (qmark = is_qmark( ch, "max_lightning_lv0" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的雷元素！\n\r", ch );
                  	  return;
                  }
          }
          else if( (sklv <= 5 && arg[0] == '\0') || ( sklv >= 3 && !str_cmp( arg , "2")) )
          {
              if( (qmark = is_qmark( ch, "max_lightning_lv1" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的雷獸！\n\r", ch );
                  	  return;
                  }
          }
          else if( sklv == 6 )
          {
              if( (qmark = is_qmark( ch, "max_lightning_lv2" ))!= NULL ) 
                  if( qmark->v0 >= qmark->v1 ) 
                  {
                  	  send_to_char( "你無法再召喚出更多的泰坦！\n\r", ch );
                  	  return;
                  }
          }
          else
              return;

      }
      else 
      {
      	  //寫max_lightning_mobs 至ppl身上
      	  //寫max_lightning_lv qmark至ppl身上
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
  
  do_say( ch, "依照古老的約定，請妳們前來保護我的身軀！" );

  if( sklv < 3 || !str_cmp(arg, "1" ) )
  {
  	  act( "呼應$n的意志，一位雷元素從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  act( "呼應$n的意志，一位雷獸從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
  	  	  send_to_char( "妳的魔力不足以打開讓招喚獸通過的通道口！\n\r", ch );
  	  	  ch->mana += 250;
  	  	  return;
  	  }
  	  
  	  act( "呼應$n的意志，一位泰坦從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );
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
      send_to_char( "一股神聖力場阻絕了你的企圖！\n\r", ch );
      return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }
  
  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚土巨人，恐怕只有大神做得到。\n\r", ch );
      
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
      do_say( ch, "這裡有太多召喚來的生物，一定要弄走一些！" );
      if( !IS_NPC(ch) )
          ch->mana += 50;
      return;
  }
  
  do_say( ch, "遵循我的使命，我命令你自土地重生！" );
  act( "根據 $n 的命令，一尊土巨人從泥土裡爬出來！", ch, NULL, NULL, TO_ROOM );
  
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
      send_to_char( "一股神聖力場阻絕了你的企圖！\n\r", ch );
      return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 200;
      return;
  }

  if( ch->in_room->sector_type == SECT_DESERT
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚水精靈，恐怕只有大神做得到。\n\r", ch );
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
      do_say( ch, "這裡有太多召喚來的生物，一定要弄走一些！" );
      return;
  }

  do_say( ch, "依照古老的約定，請妳們前來保護我的身軀！" );
  act( "呼應$n的意志，一位水精靈從異世界過來幫助$e！", ch, NULL, NULL, TO_ROOM );

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
      send_to_char( "一股神聖力場阻絕了你的企圖！\n\r", ch );
      return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
  {
      send_to_char( "神聖的地方，不允許\異物侵入！\n\r", ch );
      if( !IS_NPC(ch) )
          ch->mana += 250;
      return;
  }

  if( ch->in_room->sector_type == SECT_WATER_SWIM
      || ch->in_room->sector_type == SECT_WATER_NOSWIM
      || ch->in_room->sector_type == SECT_AIR )
  {
      send_to_char( "要在這種地形召喚火元素，恐怕只有大神做得到。\n\r", ch );
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
      do_say( ch, "這裡有太多召喚來的生物，一定要弄走一些！" );
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
          send_to_char( "你的精神力無法控制更多火元素。\n\r", ch );
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

  do_say( ch, "我願意簽訂血的盟約，交換你強大的火炎力量！" );
  act( "半空一陣巨響，一團劇烈燃燒的火焰突然出現在$n前面！", ch, NULL, NULL, TO_ROOM );

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
      send_to_char( "一股神聖力場阻絕了你的企圖！\n\r", ch );
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
      do_say( ch, "這裡已經有太多受召喚來的生物了！" );
      return;
  }

  //Added by Razgriz    20050701
  if( IS_NPC( ch ) )
  {
      if( ( qmark = is_qmark ( ch, "max_gate_mobs" )  ) != NULL ) 
      {
          if( qmark->v0 >= qmark->v1 ) 
          {
              send_to_char ( "你無法再召喚出更多的生物！\n\r", ch );
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
      do_say( ch, "來吧，兄弟！讓我們一同享受這場血腥的盛宴！" );
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
          act( "[36m$N加入你的隊伍。[m", ch, NULL, vam, TO_CHAR    );
          act( "[36m$N加入$n的隊伍。[m", ch, NULL, vam, TO_NOTVICT );
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
      do_say( ch, "來吧，兄弟！讓我們一同享受這場血腥的盛宴！" );
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
      act( "[36m$N加入你的隊伍。[m", ch, NULL, vam, TO_CHAR    );
      act( "[36m$N加入$n的隊伍。[m", ch, NULL, vam, TO_NOTVICT );
      add_protector(vam, ch);
      vam->gold = 0;
  }
  return;
}

