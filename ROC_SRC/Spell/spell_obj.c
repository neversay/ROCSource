//*****************************************************************
//*  PROGRAM ID  : spell_obj.c                                    *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940710 jye                                    *
//*  PURPOSE     :         修改spell_destroy_cursed ppl不能使用   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940831 Razgriz                                *
//*  PURPOSE     :         修改 spell_evacuate 不能傳送 mob       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 jye                                    *
//*  PURPOSE     :         修改spell_identify 可看到Z4            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940924 jye                                    *
//*  PURPOSE     :         修改spell_evacuate 使用後加上QMARK     *
//*              :         一段時間無法移動,施法及hp mp mv 都     *
//*              :         將不會回復                             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991217 tale                                    *
//*  PURPOSE     :         修改 enc_weapon 減重後玩家負重及     *
//*              :         閃避無法正常反應    *
//*****************************************************************
/* $Id: spell_obj.c 1956 2005-07-01 14:07:51Z akira $ */
#include "skill.h"
extern int HpMultiply_Affect(AFFECT_DATA *paf);
extern int HpDeMultiply_Affect(AFFECT_DATA *paf);
bool EncWpAffCheck( OBJ_DATA *obj , int type );

// 判定物品是否本身有屬性, 如果有則只能加上相同屬性 2022/12/10
bool EncWpAffCheck( OBJ_DATA *obj , int type )
{
	AFFECT_DATA *paf;
	if( !obj ) return FALSE;
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	int e = 0;
	int f = 0;
	int g = 0;
	int h = 0;
	for ( paf = obj->affected; paf; paf = paf->next )
	{
		if ( paf->location == APPLY_WEAPON_SPELL )
			a = a + paf->modifier;
		if ( paf->location == APPLY_WEAPON_WIND )
			b = b + paf->modifier;
		if ( paf->location == APPLY_WEAPON_FIRE )
			c = c + paf->modifier;
		if ( paf->location == APPLY_WEAPON_COLD )
			d = d + paf->modifier;
		if ( paf->location == APPLY_WEAPON_EARTH )
			e = e + paf->modifier;
		if ( paf->location == APPLY_WEAPON_LIGHTNING )
			f = f + paf->modifier;
		if ( paf->location == APPLY_WEAPON_SAINT )
			g = g + paf->modifier;
		if ( paf->location == APPLY_WEAPON_DARK )
			h = h + paf->modifier;
	}
	for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		if ( paf->location == APPLY_WEAPON_SPELL )
			a = a + paf->modifier;
		if ( paf->location == APPLY_WEAPON_WIND )
			b = b + paf->modifier;
		if ( paf->location == APPLY_WEAPON_FIRE )
			c = c + paf->modifier;
		if ( paf->location == APPLY_WEAPON_COLD )
			d = d + paf->modifier;
		if ( paf->location == APPLY_WEAPON_EARTH )
			e = e + paf->modifier;
		if ( paf->location == APPLY_WEAPON_LIGHTNING )
			f = f + paf->modifier;
		if ( paf->location == APPLY_WEAPON_SAINT )
			g = g + paf->modifier;
		if ( paf->location == APPLY_WEAPON_DARK )
			h = h + paf->modifier;
	}
	for ( paf = obj->material->affected; paf; paf = paf->next )
	{
		if ( paf->location == APPLY_WEAPON_SPELL )
			a = a + paf->modifier;
		if ( paf->location == APPLY_WEAPON_WIND )
			b = b + paf->modifier;
		if ( paf->location == APPLY_WEAPON_FIRE )
			c = c + paf->modifier;
		if ( paf->location == APPLY_WEAPON_COLD )
			d = d + paf->modifier;
		if ( paf->location == APPLY_WEAPON_EARTH )
			e = e + paf->modifier;
		if ( paf->location == APPLY_WEAPON_LIGHTNING )
			f = f + paf->modifier;
		if ( paf->location == APPLY_WEAPON_SAINT )
			g = g + paf->modifier;
		if ( paf->location == APPLY_WEAPON_DARK )
			h = h + paf->modifier;
	}
	if( a > 0 || b > 0 || c > 0 || d > 0 || e > 0 || f > 0 || g > 0 || h > 0 )
	{
		if( type == 1 && a > 0 ) // 法術
			return TRUE;
		if( type == 2 && b > 0 ) // 風
			return TRUE;
		if( type == 3 && c > 0 ) // 火
			return TRUE;
		if( type == 4 && d > 0 ) // 水
			return TRUE;
		if( type == 5 && e > 0 ) // 地
			return TRUE;
		if( type == 6 && f > 0 ) // 雷
			return TRUE;
		// 光闇唯有存在拮抗屬性的情況下無法附魔
		if( type == 7 ){ // 光
			if( g > 0 ) // 優先判定是否有光屬性
				return TRUE;
			if( h > 0 ) // 物品有闇屬性則失敗
				return FALSE;
			return TRUE;
		}
		if( type == 8 ){ // 闇
			if( h > 0 )
				return TRUE;
			if( g > 0 ) // 物品有光屬性則失敗
				return FALSE;
			return TRUE;
		}
	}else{
		return TRUE;
	}
	return FALSE;
}

void spell_continual_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *light;
    int sklv = get_skill_level(ch,sn);
	OBJ_DATA *obj;  // 2022/05/21
	int count = 0; // 2022/05/21

	// 對房間內魔法光球數量作限制 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_LIGHT_BALL ) )
			count += 1;
	}

	if( count >= 100 ){
		send_to_char( "你失敗了.\n\r",ch);
		return;
	}

	// sklv 最低數量的光球 2022/06/12
	sklv = sklv + 1;

	for(; sklv > 0; --sklv ) {
		// 房間內數量太多則不再放光球 2022/06/12
		if( count >= 100 ) break;
		light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
		light->value[2] += (1 + sklv) * 4;
		obj_to_room( light, ch->in_room );

		act( "你伸手一指，變出了$p.", ch, light, NULL, TO_CHAR );
		act( "$n 伸手一指，變出了$p.", ch, light, NULL, TO_ROOM );
		count = count + 1;
    }
    WAIT_MAGIC_STATE(ch, 6 - sklv);
    return;
}


void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *mushroom;
	int type;

	OBJ_DATA *obj;  // 2022/05/21
	int count = 0; // 2022/05/21

	// 對房間內魔法食物數量作限制 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_MUSHROOM ) )
			count += 1;
	}

	if( count >= 50 ){
		send_to_char( "你失敗了.\n\r",ch);
		return;
	}

	if( ch->level < 20 ) type = 0;
	else if (ch->level < 30) type = number_bits(1);
	else if( ch->level < 37) type = number_bits(2);
	else type = number_range(0,6);

	mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
	switch(type){
		case 1:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("carrot magic");
			mushroom->short_descr = str_dup("魔法紅蘿蔔(magic carrot)");
			mushroom->description = str_dup("魔法紅蘿蔔(magic carrot)");
			mushroom->value[0] = 5 + ch->level/2 ;
			break;
		case 2:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("manto magic");
			mushroom->short_descr = str_dup("魔法饅頭(magic manto)");
			mushroom->description = str_dup("魔法饅頭(magic manto)");
			mushroom->value[0] = 5 + ch->level ;
			break;
		case 3:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("wing chicken-wing magic");
			mushroom->short_descr = str_dup("魔法烤雞翅(magic chicken wing)");
			mushroom->description = str_dup("魔法烤雞翅(magic chicken wing)");
			mushroom->value[0] = 5 + ch->level + ch->level / 2;
			break;
		case 4:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("steak magic");
			mushroom->short_descr = str_dup("魔法烤牛排(magic steak)");
			mushroom->description = str_dup("魔法烤牛排(magic steak)");
			mushroom->value[0] = 5 + ch->level + ch->level;
			break;
		case 5:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("chicken full-chicken magic");
			mushroom->short_descr = str_dup("魔法手扒雞(magic full chicken)");
			mushroom->description = str_dup("魔法手扒雞(magic full chicken)");
			mushroom->value[0] = 5 + ch->level + ch->level/2 + ch->level;
			break;
		case 6:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("pig roast-pig magic");
			mushroom->short_descr = str_dup("魔法烤乳豬(magic roast pig)");
			mushroom->description = str_dup("魔法烤乳豬(magic roast pig)");
			mushroom->value[0] = 5 + ch->level + ch->level + ch->level;
			break;
		default:
			mushroom->value[0] = 5 + ch->level / 4;
			break;
	}
	mushroom->value[2] += get_skill_level(ch,sn) * 12;
	mushroom->timer = 2 + get_skill_level(ch,sn)/3;
	obj_to_room( mushroom, ch->in_room );

	act( "$p 突然出現在你面前.", ch, mushroom, NULL, TO_CHAR );
	act( "$p 突然出現在你面前.", ch, mushroom, NULL, TO_ROOM );
	return;
}


void spell_create_spring( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *spring;
	char buf[MAX_STRING_LENGTH];
	char *arg = (char*) vo;
	int sklv = get_skill_level(ch,sn);
	int i;
	OBJ_DATA *obj;  // 2022/05/21
	int count = 0; // 2022/05/21

	// 對房間內魔法泉水數量作限制 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_SPRING ) )
			count += 1;
	}

	if( count >= 5 ){
		send_to_char( "你失敗了.\n\r",ch);
		return;
	}


	if( sklv < 6 || !arg || arg[0] == '\0'  )
	{
		sklv = UMIN(sklv, 6);
		spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
		spring->timer = 2 + sklv;
		spring->value[2] = number_range( 0, UMIN(LIQ_MAX,sklv * 3 -1 ));
		if( spring->value[2] != 0 ) 
		{
			free_string(spring->short_descr);
			free_string(spring->name);
			free_string(spring->description);
			sprintf(buf, "%s spring", liq_table[spring->value[2]].liq_ename);
			spring->name = str_dup(buf);
			sprintf(buf, "魔法%s泉(%s spring)", 
							liq_table[spring->value[2]].liq_name,
							liq_table[spring->value[2]].liq_ename);
			spring->short_descr = str_dup(buf);
			spring->description = str_dup(buf);
			SET_BIT(spring->wear_flags, ITEM_TAKE);
		}

		obj_to_room( spring, ch->in_room );
		act( "地面突然裂開，流出了一道$p.", ch, spring, NULL, TO_CHAR );
		act( "地面突然裂開，流出了一道$p.", ch, spring, NULL, TO_ROOM );
		return;
	}
	else
	{
		for( i = 0 ; i < LIQ_MAX ; i ++ )
			if( !str_prefix( arg , liq_table[i].liq_ename) )
			{
				spring = create_object( get_obj_index(OBJ_VNUM_SPRING) , 0 );
				spring->timer = 2 + sklv;
				spring->value[2] = i;
				if( spring->value[2] != 0 ) 
				{
					free_string(spring->short_descr);
					free_string(spring->name);
					free_string(spring->description);
					sprintf(buf, "%s spring", liq_table[spring->value[2]].liq_ename);
					spring->name = str_dup(buf);
					sprintf(buf, "魔法%s泉(%s spring)", 
									liq_table[spring->value[2]].liq_name,
									liq_table[spring->value[2]].liq_ename);
					spring->short_descr = str_dup(buf);
					spring->description = str_dup(buf);
					SET_BIT(spring->wear_flags, ITEM_TAKE);
				}
				obj_to_room( spring, ch->in_room );

				act( "地面突然裂開，流出了一道$p.", ch, spring, NULL, TO_CHAR );
				act( "地面突然裂開，流出了一道$p.", ch, spring, NULL, TO_ROOM );
				return;
			}
		send_to_char( "沒有這種泉喔！\n\r",ch);
		return;
	}

}

void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj   = (OBJ_DATA *) vo;
    int       water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
  send_to_char( "那樣東西不能裝水.\n\r", ch );
  return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
  send_to_char( "那裡面已經有別種液體了.\n\r", ch );
  return;
    }

    water = UMIN( level * ( weather_info.sky >= SKY_RAINING ? 4 : 2 ),
     obj->value[0] - obj->value[1] );
  
    if ( water > 0 )
    {
  obj->value[2] = LIQ_WATER;
  obj->value[1] += water;
  if ( !is_name( "water", obj->name ) )
  {
      char buf [ MAX_STRING_LENGTH ];

      sprintf( buf, "%s water", obj->name );
      free_string( obj->name );
      obj->name = str_dup( buf );
  }
  act( "$p 現在裝滿了.", ch, obj, NULL, TO_CHAR );
    }

    return;
}

void spell_destroy_cursed( int sn, int level, CHAR_DATA *ch, void *vo )
{
  OBJ_DATA  *obj;
  OBJ_DATA  *obj_next;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int        yesno  = 0;

  if( !IS_NPC( ch ) && !IS_IMMORTAL( ch ) )
  {
      send_to_char( "一股神聖力場阻絕了你的企圖！\n\r", ch );
      return;
  }
  
  if( IS_NPC(victim) && victim->pIndexData->pShop ) 
  {
      return;
  }
  
  for( obj = victim->carrying; obj; obj = obj_next )
  {
       obj_next = obj->next_content;

       if( IS_SET( obj->extra_flags, ITEM_NODROP )
           && obj->wear_loc == WEAR_NONE )
       {
           act( "你全力把 $p 往地上一摔。", victim, obj, NULL, TO_CHAR );
           act( "$n 全力把 $p 往地上一摔。", victim, obj, NULL, TO_ROOM );
           //extract_obj( obj );
           obj_from_char( obj );
           
           if( IS_SET( obj->extra_flags , ITEM_PERSONAL ) )
             obj_to_char( obj, victim);
           else
             obj_to_room( obj, victim->in_room );
       
                 //obj_to_room(obj, victim->in_room );
           obj->timer = URANGE( 0, obj->timer,  (obj->level / 10 +1));
           yesno = 1;
       }
  }

  if( ch != victim && yesno )
      send_to_char( "好的。\n\r", ch );
  return;
}

void spell_repair_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj = (OBJ_DATA *) vo;
	
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


    if ( obj->item_type != ITEM_ARMOR )
    {
  send_to_char( "那不是護具.\n\r", ch );
  return;
    }

    if ( obj->wear_loc != WEAR_NONE ) {
  send_to_char("你必須先脫下它.\n\r", ch );
  return;
    }

    if ( !IS_NPC( ch ) && obj->level - 2 > ch->level + sklv)
    {
  send_to_char( "你無法修護這麼高級的護具.\n\r",
         ch);
  return;
    }

    if( obj->value[1] < 1 ) {
  send_to_char("那件護具並沒有損壞.\n\r", ch);
  return;
    }


    act( "一陣強光籠罩著$p，隨著光芒散去，$p的損傷慢慢修復了。",   ch, obj, NULL, TO_CHAR );

    if( obj->value[1] < sklv+1 )
    {
      obj->value[0] += obj->value[1];
      obj->value[1] = 0;
    }
    else
    {
      obj->value[0] += sklv + 1;
      obj->value[1] -= sklv + 1;
    }

    ch->mana += sklv * 10 + (sklv - 3) * 3;
    send_to_char( "好的。\n\r", ch );
    return;
}

void spell_enchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int hr=0, dr=0;
    int skhr = 0, skdr = 0;
    int sklv = get_skill_level(ch,sn);
    bool fStats= FALSE;
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

    if ( obj->item_type != ITEM_WEAPON
  || IS_OBJ_STAT( obj, ITEM_MAGIC )
  || obj->affected )
    {
  send_to_char( "那無法再被增強了.\n\r", ch );
  return;
    }

    if ( obj->wear_loc != WEAR_NONE )
  remove_obj( ch, obj->wear_loc, TRUE );

    /* Thanks to Magoo for this little insight */
    if ( !IS_NPC(ch) && obj->level > ch->level + sklv )
    {
  send_to_char( "你無法為更高級的武器增強.\n\r",
         ch);
  return;
    }
    SET_BIT(obj->extra_flags, ITEM_MAGIC);
    hr = 1 + URANGE(0,((ch->level - 14)/7), 4);
    skhr = 1 + URANGE(0, (sklv/3 + sklv/2), 5);
    dr = 3 + URANGE(0,((ch->level - 15)/5),5) + URANGE(0, ((ch->level - 35) /2), 4);
    skdr = 3 + URANGE(0, (sklv), 6);
    dr = UMAX(skdr,dr);
    hr = UMAX(skhr,hr);


    if ( IS_GOOD( ch ) )
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL);
  act( "$p 泛起一陣藍光.",   ch, obj, NULL, TO_CHAR );
        if( sklv > 0 ) {
            hr += 2;
            dr -= 3;
        }
        if( sklv > 1 ) {
            hr += 1;
            dr -= 1;
        }
        if( sklv > 2 ) {
            hr += 1;
        }
    }
    else if ( IS_EVIL( ch ) )
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
  act( "$p 泛起一陣紅光.",   ch, obj, NULL, TO_CHAR );
        if( sklv > 0 ) {
            hr -= 4;
            dr += 1;
        }
        if( sklv > 1 ) {
            hr += 1;
            dr += 1;
        }
        if( sklv > 2 ) {
            hr += 1;
            dr += 2;
        }
    }
    else
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
  SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
  act( "$p 泛起一陣黃光.", ch, obj, NULL, TO_CHAR );
        if( sklv > 0 ) {
            hr += 1;
        }
        if( sklv > 1 ) {
            dr += 2;
        }
        if( sklv > 2 ) {
            hr += 1;
            dr += 1;
        }
        if( sklv > 5 ) {
            if( number_bits(7) == 2 ) {
                act( "$p 泛起一陣金光.", ch, obj, NULL, TO_CHAR );
                SET_BIT( obj->extra_flags, ITEM_BLESS);
            }
        }
    }
    if( hr > 0 ) {
        paf     = new_affect();

        paf->type   = obj->pIndexData->vnum;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = hr;
        paf->bitvector  = 0;
        paf->next   = obj->affected;
        obj->affected = paf;
    }

    if( dr > 0 ) {

        paf     = new_affect();

        paf->type   = obj->pIndexData->vnum;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = dr;//3 + URANGE(0,((ch->level - 15)/5),5)
        //+ URANGE(0, ((ch->level - 35) /2), 4);
        paf->bitvector  = 0;
        paf->next   = obj->affected;
        obj->affected = paf;
    }
    switch(sklv) {
        case 5:
        case 6:
            if( number_percent() < sklv*3  ) {
                switch(number_bits(3)){
                    case 1:
                        fStats = TRUE;
                        paf     = new_affect();

                        paf->type   = obj->pIndexData->vnum;
                        paf->duration = -1;
                        paf->location = APPLY_STR;
                        paf->modifier = 1;
                        paf->bitvector  = 0;
                        paf->next   = obj->affected;
                        obj->affected = paf;
                        break;
                    case 2:
                        fStats = TRUE;
                        paf     = new_affect();

                        paf->type   = obj->pIndexData->vnum;
                        paf->duration = -1;
                        paf->location = APPLY_CON;
                        paf->modifier = 1;
                        paf->bitvector  = 0;
                        paf->next   = obj->affected;
                        obj->affected = paf;
                        break;
                    case 3:
                        fStats = TRUE;
                        paf     = new_affect();

                        paf->type   = obj->pIndexData->vnum;
                        paf->duration = -1;
                        paf->location = APPLY_DEX;
                        paf->modifier = 1;
                        paf->bitvector  = 0;
                        paf->next   = obj->affected;
                        obj->affected = paf;
                        break;
                    case 6:
                        fStats = TRUE;
                        paf     = new_affect();

                        paf->type   = obj->pIndexData->vnum;
                        paf->duration = -1;
                        paf->location = APPLY_WIS;
                        paf->modifier = 1;
                        paf->bitvector  = 0;
                        paf->next   = obj->affected;
                        obj->affected = paf;
                        break;
                    case 5:
                        fStats = TRUE;
                        paf     = new_affect();

                        paf->type   = obj->pIndexData->vnum;
                        paf->duration = -1;
                        paf->location = APPLY_INT;
                        paf->modifier = 1;
                        paf->bitvector  = 0;
                        paf->next   = obj->affected;
                        obj->affected = paf;
                        break;
                }
            }
        case 4:
             //modified by tale for ch carry_weight updated in real time as case 4 happened
            if( !fStats && number_percent() < sklv * 3 ) {
	 ch->carry_weight -= UMAX(0, obj->weight / 5);
                obj->weight = UMAX(0, (obj->weight - obj->weight / 5));
                fStats = TRUE;
            }
            break;
        default:
            break;
    }
    if( !fStats && (sklv < 6 || number_percent() < get_skill_percent(ch,sn) / 4 ) && obj->level < level -3 - sklv ){
        int olv = number_fuzzy( level - 4 - sklv );
        obj->level    = UMAX(obj->level, olv);
    }
    if ( obj->timer > 20 )
  obj->timer = UMAX( 20, obj->timer - 60 );

    send_to_char( "好的。\n\r", ch );
    return;
}


void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA  *obj = (OBJ_DATA *) vo;
	AFFECT_DATA *paf;
	char buf [ MAX_STRING_LENGTH * 2];
	//char *arg;

	int sklv   = get_skill_level(ch,sn);
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
		level = obj_lv + sklv;
	}else{
		level += sklv;
	}
	
	sprintf( buf,
					"物品名稱: %s   物品形式: %s(%s)\n\r物品狀態: %s.\n\r",
					obj->short_descr,
					c_item_type_name( obj ),
					item_type_name(obj),
					c_extra_bit_name( obj->extra_flags )
			);
	send_to_char( buf, ch );

	if( obj->pIndexData->vnum == OBJ_VNUM_POWER_GEM ){
		sprintf(buf,"魔力回復量: %d - %d 點.\n\r", obj->value[0],obj->value[1] );
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_CONTAINER ) {
		if( obj->value[3] > 0 ) {
			sprintf(buf,"容量: %d, 減重比率: %d%%.\n\r", obj->value[0], 100 - obj->value[3] );
		}
		else if( obj->value[3] < 0 )
			// 修改數量減少顯示為正值 2022/12/01
			//sprintf(buf,"容量: %d, 減少物品數: %d件.\n\r",obj->value[0], obj->value[3]);
			sprintf(buf,"容量: %d, 減少物品數: %d件.\n\r",obj->value[0], 0 - obj->value[3] );
		else
			sprintf(buf,"容量: %d\n\r", obj->value[0]);
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_DRINK_CON ) {
		if( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ) {
			bug( "Spell_identify: bad liquid number %d.", obj );
			obj->value[2] = 0;
		}
		/* 將每一項分開, 以作不同判定顯示 2023/01/05

		sprintf(buf,"容量: %d, 盛裝量: %d, 種類: %s.\n\r", obj->value[0], obj->value[1], liq_table[obj->value[2]].liq_name);
		send_to_char(buf, ch); */

		if( obj->value[0] > 0 )
			sprintf( buf, "容量: %d, ", obj->value[0] );
		else
			sprintf( buf, "容量: 0, ");
		send_to_char(buf, ch);

		if( obj->value[1] > 0 ){
			sprintf( buf, "盛裝量: %d, ", obj->value[1] );
		}else{
			if( obj->value[1] == -1 )  // 新增 -1 的情況 2023/01/05
				sprintf( buf, "盛裝量: ∞, ");
			else
				sprintf( buf, "盛裝量: 0, ");
		}
		send_to_char(buf, ch);

		sprintf( buf,"種類: %s.\n\r", liq_table[obj->value[2]].liq_name );
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_FOUNTAIN ) {
		if( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ) {
			bug( "Spell_identify: bad liquid number %d.", obj );
			obj->value[2] = 0;
		}
		/* 將每一項分開, 以作不同判定顯示 2023/01/05

		sprintf(buf,"泉水規模: %d, 殘留量: %d, 種類: %s.\n\r", obj->value[0], obj->value[1], liq_table[obj->value[2]].liq_name);
		send_to_char(buf, ch); */

		if( obj->value[0] > 0 )
			sprintf( buf, "泉水規模: %d, ", obj->value[0] );
		else
			sprintf( buf, "泉水規模: 0, ");
		send_to_char(buf, ch);

		if( obj->value[1] > 0 ){
			sprintf( buf, "殘存量: %d, ", obj->value[1] );
		}else{
			if( obj->value[1] == -1 )  // 新增 -1 的情況 2023/01/05
				sprintf( buf, "殘存量: ∞, ");
			else
				sprintf( buf, "殘存量: 0, ");
		}
		send_to_char(buf, ch);

		sprintf( buf,"種類: %s.\n\r", liq_table[obj->value[2]].liq_name );
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_FOOD ) {
		if( obj->value[1] > 0 && obj->value[1] <= 49 ) {
			sprintf(buf,"飽足度: %d, 食物加成: %s %d 點.\n\r", obj->value[0], c_affect_loc_name( obj->value[1] ), obj->value[2]);
		}
		else
			sprintf(buf,"飽足度: %d.\n\r", obj->value[0]);
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_ARMOR && obj->wear_flags ) {
		send_to_char("裝備位置：", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_ABOUT))
			send_to_char(" 圍繞身體", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_ARMS))
			send_to_char(" 手臂", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_BODY))
			send_to_char(" 身體", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_ARROW))
			send_to_char(" 背掛", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_FEET))
			send_to_char(" 腳部", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_HANDS))
			send_to_char(" 手部", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_FINGER))
			send_to_char(" 手指 觸手", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_HEAD))
			send_to_char(" 頭部", ch);
		if( CAN_WEAR(obj, ITEM_HOLD))
			send_to_char(" 手握", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_LEGS))
			send_to_char(" 腿部", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_NECK))
			send_to_char(" 頸部", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_SHIELD))
			send_to_char(" 盾牌", ch);
		if( CAN_WEAR( obj, ITEM_TWO_HAND))
			send_to_char(" 雙手武器", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_WAIST))
			send_to_char(" 腰部", ch);
		if( CAN_WEAR( obj, ITEM_WIELD))
			send_to_char(" 武器", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_WRIST))
			send_to_char(" 腕部", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_AMMO)) // 彈藥部位 2022/04/30
			send_to_char(" 彈藥", ch);
		send_to_char("\n\r", ch);
	}
	if( obj->pIndexData->max_level ) {
		sprintf( buf, "材質: %s  重量: %d  價值: %d  等級: %d - %d\n\r",
			obj->material->short_descr,
			obj->weight,
			obj->cost,
			obj->level,
			obj->pIndexData->max_level);
	}else{
		sprintf( buf, "材質: %s  重量: %d  價值: %d  等級: %d\n\r",
			obj->material->short_descr,
			obj->weight,
			obj->cost,
			obj->level );
	}
	send_to_char( buf, ch );


	switch ( obj->item_type )
	{
		case ITEM_PILL:  
		case ITEM_SCROLL: 
		case ITEM_POTION:
			sprintf( buf, "附有等級 %d 的法術:", obj->value[0] );
			send_to_char( buf, ch );
			if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
			{
				send_to_char( " '", ch );
				send_to_char( skill_table[obj->value[1]].cname, ch );
				send_to_char("(", ch);
				send_to_char( skill_table[obj->value[1]].name, ch );
				send_to_char( ")'", ch );
			}
			if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
			{
				send_to_char( " '", ch );
				send_to_char( skill_table[obj->value[2]].cname, ch );
				send_to_char("(", ch);
				send_to_char( skill_table[obj->value[2]].name, ch );
				send_to_char( ")'", ch );
			}
			if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
			{
				send_to_char( " '", ch );
				send_to_char( skill_table[obj->value[3]].cname, ch );
				send_to_char("(", ch);
				send_to_char( skill_table[obj->value[3]].name, ch );
				send_to_char( ")'", ch );
			}
			send_to_char( ".\n\r", ch );
			break;

		case ITEM_WAND: 
		case ITEM_STAFF: 
			sprintf( buf, "尚有 %d/%d 次效用，等級： %d", obj->value[2], obj->value[1], obj->value[0] );
			send_to_char( buf, ch );
			if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
			{
				send_to_char( " '", ch );
				send_to_char( skill_table[obj->value[3]].cname, ch );
				send_to_char("(", ch);
				send_to_char( skill_table[obj->value[3]].name, ch );
				send_to_char( ")'", ch );
			}
			send_to_char( ".\n\r", ch );
			break;

		case ITEM_WEAPON:
			// 如果武器為槍械, 則增加射擊精準度的敘述 2022/05/01
			if(obj->value[3] == 16)
			{
				sprintf( buf, "武器傷害力: %d - %d (平均 %d).\n\r射擊精準度: %d - %d (平均 %d).\n\r",
								obj->value[1],
								obj->value[2],
								(obj->value[1] + obj->value[2] ) / 2,
								obj->value[1]*2,
								obj->value[2]*2,
								obj->value[1] + obj->value[2] );
				send_to_char( buf, ch );
			}
			else
			{
				sprintf( buf, "武器傷害力: %d - %d (平均 %d).\n\r",
								obj->value[1],
								obj->value[2],
								(obj->value[1] + obj->value[2] ) / 2 );
				send_to_char( buf, ch );
			}
			if( !indestructible(obj) )
			{
				sprintf( buf, "耐用度：%d/%d.\n\r", get_curr_dura(obj), get_max_dura(obj));
				send_to_char( buf, ch );
			}
			break;

		case ITEM_ARROW:
			// 將數量與傷害分開 2023/01/05
			//sprintf( buf, "箭支傷害力: %d - %d (平均 %d+%d).\n\r箭矢數: %d 支.\n\r",
			sprintf( buf, "箭支傷害力: %d - %d (平均 %d+%d).",
							obj->value[1], obj->value[2],
							( obj->value[1] + obj->value[2] ) / 2 ,
							obj->value[3] );
							//obj->value[0] );
			send_to_char( buf, ch );
			if( obj->value[0] > 0 )
				sprintf( buf, "\n\r箭矢數: %d 支.\n\r", obj->value[0] );
			else{
				if( obj->value[0] == -1 )  // 新增 -1 的情況 2023/01/05
					sprintf( buf, "\n\r箭矢數: ∞ 支.\n\r");
				else
					sprintf( buf, "\n\r箭矢數: 0 支.\n\r");
			}
			send_to_char( buf, ch );
			break;
  
		case ITEM_DART:
			// 將數量與傷害分開 2023/01/05
			//sprintf( buf, "暗器傷害力: %d - %d (平均 %d+%d).\n\r暗器數: %d 把.\n\r",
			sprintf( buf, "暗器傷害力: %d - %d (平均 %d+%d).",
							obj->value[1], obj->value[2],
							( obj->value[1] + obj->value[2] ) / 2 ,
							obj->value[3] );
							//obj->value[0] );
			if( obj->value[0] > 0 )
				sprintf( buf, "\n\r暗器數: %d 件.\n\r", obj->value[0] );
			else{
				if( obj->value[0] == -1 )  // 新增 -1 的情況 2023/01/05
					sprintf( buf, "\n\r暗器數: ∞ 件.\n\r");
				else
					sprintf( buf, "\n\r暗器數: 0 件.\n\r");
			}
			send_to_char( buf, ch );
			break;

		case ITEM_AMMO:
			// 新增彈藥 type 2022/05/01
			sprintf( buf,"彈藥殺傷力: %d - %d (平均 %d).", (obj->value[1]*19)/20, (obj->value[1]*20)/19, obj->value[1] );
			send_to_char(buf, ch);
			if( obj->value[3] > 0 && obj->value[3] < MAX_SKILL )
			{
				sprintf( buf, " 附有等級 %d 的法術:", obj->value[2] );
				send_to_char( buf, ch );
				send_to_char( " '", ch );
				send_to_char( skill_table[obj->value[3]].cname, ch );
				send_to_char("(", ch);
				send_to_char( skill_table[obj->value[3]].name, ch );
				send_to_char( ")'", ch );
			}
			if( obj->value[0] > 0 )
				sprintf( buf, "\n\r彈藥數: %d 發.\n\r", obj->value[0] );
			else{
				if( obj->value[0] == -1 )  // 新增 -1 的情況 2023/01/05
					sprintf( buf, "\n\r彈藥數: ∞ 發.\n\r");
				else
					sprintf( buf, "\n\r彈藥數: 0 發.\n\r");
			}
			send_to_char( buf, ch );
			break;

		case ITEM_ARMOR:
			sprintf( buf, "護甲防衛力(AC): %d.\n\r", obj->value[0] );
			send_to_char( buf, ch );
			if( get_curr_dura(obj) != DURA_INDESTRUCTIBLE )
			{
				sprintf( buf, "耐用度: %d/%d.\n\r", get_curr_dura(obj), get_max_dura(obj));
				send_to_char( buf, ch );
			}
			break;
	}

	if( IS_SET(obj->pIndexData->souvenir, Z1) )
	{
		send_to_char( "備註: 此物品為紀念品\n\r", ch );
	}

	if( IS_SET(obj->pIndexData->souvenir, Z2) )
	{
		send_to_char( "Mob 專用物品\n\r", ch );
	}
  
	if( IS_SET(obj->pIndexData->souvenir, Z4) )
	{
		send_to_char( "備註: 此物品為特殊珍藏品\n\r", ch );
	}

	for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		if ( paf->location != APPLY_NONE && paf->modifier != 0 )
		{
			HpMultiply_Affect(paf);
			// 加入有時效的affect 能以 id 顯示時間 2021/09/29
			if ( paf->duration > 0 )
			{
				sprintf( buf, "影響 %s %d 點. 尚有 %d 小時的效用.\n\r", c_affect_loc_name( paf->location ), paf->modifier , paf->duration);
			}else{
				sprintf( buf, "影響 %s %d 點.\n\r", c_affect_loc_name( paf->location ), paf->modifier );
			}
			// sprintf( buf, "影響 %s %d 點.\n\r",
			// c_affect_loc_name( paf->location ), paf->modifier );
			send_to_char( buf, ch );
			HpDeMultiply_Affect(paf);
		}
	}

	for ( paf = obj->affected; paf; paf = paf->next )
	{
		if ( paf->location != APPLY_NONE && paf->modifier != 0 )
		{
			HpMultiply_Affect(paf);
			// 加入有時效的affect 能以 id 顯示時間 2021/09/29
			if ( paf->duration >= 0 )
			{
				sprintf( buf, "影響 %s %d 點. 效果尚有 %d 小時的效用.\n\r", c_affect_loc_name( paf->location ), paf->modifier , paf->duration);
			}else{
				sprintf( buf, "影響 %s %d 點.\n\r", c_affect_loc_name( paf->location ), paf->modifier );
			}
			//sprintf( buf, "影響 %s %d 點.\n\r",
			//c_affect_loc_name( paf->location ), paf->modifier );
			send_to_char( buf, ch );
			HpDeMultiply_Affect(paf);
		}
	}

	if( level > 36 ) {
		for ( paf = obj->material->affected; paf; paf = paf->next )
		{
			if ( paf->location != APPLY_NONE && paf->modifier != 0 )
			{
				HpMultiply_Affect(paf);
				// 加入有時效的affect 能以 id 顯示時間 2021/09/29
				if ( paf->duration >= 0 )
				{
					sprintf( buf, "材質影響 %s %d 點. 尚有 %d 小時的效用.\n\r", c_affect_loc_name( paf->location ), paf->modifier , paf->duration);
				}else{
					sprintf( buf, "材質影響 %s %d 點.\n\r", c_affect_loc_name( paf->location ), paf->modifier );
				}
				//sprintf( buf, "材質影響 %s %d 點.\n\r",
				//c_affect_loc_name( paf->location ), paf->modifier );
				send_to_char( buf, ch );
				HpDeMultiply_Affect(paf);
			}
		}
	}
	if ( obj->class )
	{
		if ( IS_SET( obj->class, ALLOW_BIT ) )
		{
			send_to_char( "專用職業：", ch );
			if ( IS_SET( obj->class, NUM_BIT( MAGE ) ) )
				send_to_char( "魔法師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "牧師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( THIEF ) ) )
				send_to_char( "妙賊 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "戰士 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "野蠻人 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "巫師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "鍊金師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "賢者 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "主教 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "刺客 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( RANGER ) ) )
				send_to_char( "游俠 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( NINJA ) ) )
				send_to_char( "忍者 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "騎士 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "劍客 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MONK ) ) )
				send_to_char( "僧侶 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "巫醫 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "幻法師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( LORD ) ) )
				send_to_char( "領主 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "戰王 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "狂戰士 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->class, DENY_BIT ) )
		{
			send_to_char( "禁用職業：", ch );
			if ( IS_SET( obj->class, NUM_BIT( MAGE ) ) )
				send_to_char( "魔法師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "牧師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( THIEF ) ) )
				send_to_char( "妙賊 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "戰士 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "野蠻人 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "巫師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "鍊金師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "賢者 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "主教 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "刺客 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( RANGER ) ) )
				send_to_char( "游俠 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( NINJA ) ) )
				send_to_char( "忍者 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "騎士 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "劍客 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MONK ) ) )
				send_to_char( "僧侶 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "巫醫 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "幻法師 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( LORD ) ) )
				send_to_char( "領主 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "戰王 ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "狂戰士 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->material->class ) {
		if ( IS_SET( obj->material->class, ALLOW_BIT ) )
		{
			send_to_char( "材質影響專用職業：", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MAGE ) ) )
				send_to_char( "魔法師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "牧師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( THIEF ) ) )
				send_to_char( "妙賊 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "戰士 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "野蠻人 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "巫師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "鍊金師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "賢者 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "主教 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "刺客 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( RANGER ) ) )
				send_to_char( "游俠 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( NINJA ) ) )
				send_to_char( "忍者 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "騎士 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "劍客 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MONK ) ) )
				send_to_char( "僧侶 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "巫醫 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "幻法師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( LORD ) ) )
				send_to_char( "領主 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "戰王 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "狂戰士 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->material->class, DENY_BIT ) )
		{
			send_to_char( "材質影響職業限制：", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MAGE ) ) )
				send_to_char( "魔法師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "牧師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( THIEF ) ) )
				send_to_char( "妙賊 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "戰士 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "野蠻人 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "巫師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "鍊金師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "賢者 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "主教 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "刺客 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( RANGER ) ) )
				send_to_char( "游俠 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( NINJA ) ) )
				send_to_char( "忍者 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "騎士 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "劍客 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MONK ) ) )
				send_to_char( "僧侶 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "巫醫 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "幻法師 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( LORD ) ) )
				send_to_char( "領主 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "戰王 ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "狂戰士 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if ( obj->race )
	{
		if ( IS_SET( obj->race, ALLOW_BIT ) )
		{
			send_to_char( "專用種族：", ch );
			if ( IS_SET( obj->race, HUMAN ) )
				send_to_char( "人類 ", ch );
			if ( IS_SET( obj->race, ELF ) )
				send_to_char( "精靈 ", ch );
			if ( IS_SET( obj->race, DWARF ) )
				send_to_char( "矮人 ", ch );
			if ( IS_SET( obj->race, ORC ) )
				send_to_char( "獸人 ", ch );
			if ( IS_SET( obj->race, FAIRY ) )
				send_to_char( "妖精 ", ch );
			if ( IS_SET( obj->race, GNOME ) )
				send_to_char( "地精 ", ch );
			if ( IS_SET( obj->race, HOBBIT ) )
				send_to_char( "賀比特 ", ch );
			if ( IS_SET( obj->race, DROW ) )
				send_to_char( "暗黑精靈 ", ch );
			if ( IS_SET( obj->race, VAMPIRE ) )
				send_to_char( "吸血鬼 ", ch );
			if ( IS_SET( obj->race, BEHOLDER ) )
				send_to_char( "眼魔 ", ch );
			if ( IS_SET( obj->race, LIZARDMAN ) )
				send_to_char( "蜥蜴人 ", ch );
			if ( IS_SET( obj->race, WEREWOLF ) )
				send_to_char( "狼人 ", ch );
			if ( IS_SET( obj->race, MUMMY ) )
				send_to_char( "木乃伊 ", ch );
			if ( IS_SET( obj->race, GARGOYLE ) )
				send_to_char( "石像鬼 ", ch );
			if ( IS_SET( obj->race, CENTAUR ) )
				send_to_char( "人馬 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->race, DENY_BIT ) )
		{
			send_to_char( "種族限制：", ch );
			if ( IS_SET( obj->race, HUMAN ) )
				send_to_char( "人類 ", ch );
			if ( IS_SET( obj->race, ELF ) )
				send_to_char( "精靈 ", ch );
			if ( IS_SET( obj->race, DWARF ) )
				send_to_char( "矮人 ", ch );
			if ( IS_SET( obj->race, ORC ) )
				send_to_char( "獸人 ", ch );
			if ( IS_SET( obj->race, FAIRY ) )
				send_to_char( "妖精 ", ch );
			if ( IS_SET( obj->race, GNOME ) )
				send_to_char( "地精 ", ch );
			if ( IS_SET( obj->race, HOBBIT ) )
				send_to_char( "賀比特 ", ch );
			if ( IS_SET( obj->race, DROW ) )
				send_to_char( "暗黑精靈 ", ch );
			if ( IS_SET( obj->race, VAMPIRE ) )
				send_to_char( "吸血鬼 ", ch );
			if ( IS_SET( obj->race, BEHOLDER ) )
				send_to_char( "眼魔 ", ch );
			if ( IS_SET( obj->race, LIZARDMAN ) )
				send_to_char( "蜥蜴人 ", ch );
			if ( IS_SET( obj->race, WEREWOLF ) )
				send_to_char( "狼人 ", ch );
			if ( IS_SET( obj->race, MUMMY ) )
				send_to_char( "木乃伊 ", ch );
			if ( IS_SET( obj->race, GARGOYLE ) )
				send_to_char( "石像鬼 ", ch );
			if ( IS_SET( obj->race, CENTAUR ) )
				send_to_char( "人馬 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if ( obj->material->race )
	{
		if ( IS_SET( obj->material->race, ALLOW_BIT ) )
		{
			send_to_char( "材質影響專用種族：", ch );
			if ( IS_SET( obj->material->race, HUMAN ) )
				send_to_char( "人類 ", ch );
			if ( IS_SET( obj->material->race, ELF ) )
				send_to_char( "精靈 ", ch );
			if ( IS_SET( obj->material->race, DWARF ) )
				send_to_char( "矮人 ", ch );
			if ( IS_SET( obj->material->race, ORC ) )
				send_to_char( "獸人 ", ch );
			if ( IS_SET( obj->material->race, FAIRY ) )
				send_to_char( "妖精 ", ch );
			if ( IS_SET( obj->material->race, GNOME ) )
				send_to_char( "地精 ", ch );
			if ( IS_SET( obj->material->race, HOBBIT ) )
				send_to_char( "賀比特 ", ch );
			if ( IS_SET( obj->material->race, DROW ) )
				send_to_char( "暗黑精靈 ", ch );
			if ( IS_SET( obj->material->race, VAMPIRE ) )
				send_to_char( "吸血鬼 ", ch );
			if ( IS_SET( obj->material->race, BEHOLDER ) )
				send_to_char( "眼魔 ", ch );
			if ( IS_SET( obj->material->race, LIZARDMAN ) )
				send_to_char( "蜥蜴人 ", ch );
			if ( IS_SET( obj->material->race, WEREWOLF ) )
				send_to_char( "狼人 ", ch );
			if ( IS_SET( obj->material->race, MUMMY ) )
				send_to_char( "木乃伊 ", ch );
			if ( IS_SET( obj->material->race, GARGOYLE ) )
				send_to_char( "石像鬼 ", ch );
			if ( IS_SET( obj->material->race, CENTAUR ) )
				send_to_char( "人馬 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->material->race, DENY_BIT ) )
		{
			send_to_char( "材質影響種族限制：", ch );
			if ( IS_SET( obj->material->race, HUMAN ) )
				send_to_char( "人類 ", ch );
			if ( IS_SET( obj->material->race, ELF ) )
				send_to_char( "精靈 ", ch );
			if ( IS_SET( obj->material->race, DWARF ) )
				send_to_char( "矮人 ", ch );
			if ( IS_SET( obj->material->race, ORC ) )
				send_to_char( "獸人 ", ch );
			if ( IS_SET( obj->material->race, FAIRY ) )
				send_to_char( "妖精 ", ch );
			if ( IS_SET( obj->material->race, GNOME ) )
				send_to_char( "地精 ", ch );
			if ( IS_SET( obj->material->race, HOBBIT ) )
				send_to_char( "賀比特 ", ch );
			if ( IS_SET( obj->material->race, DROW ) )
				send_to_char( "暗黑精靈 ", ch );
			if ( IS_SET( obj->material->race, VAMPIRE ) )
				send_to_char( "吸血鬼 ", ch );
			if ( IS_SET( obj->material->race, BEHOLDER ) )
				send_to_char( "眼魔 ", ch );
			if ( IS_SET( obj->material->race, LIZARDMAN ) )
				send_to_char( "蜥蜴人 ", ch );
			if ( IS_SET( obj->material->race, WEREWOLF ) )
				send_to_char( "狼人 ", ch );
			if ( IS_SET( obj->material->race, MUMMY ) )
				send_to_char( "木乃伊 ", ch );
			if ( IS_SET( obj->material->race, GARGOYLE ) )
				send_to_char( "石像鬼 ", ch );
			if ( IS_SET( obj->material->race, CENTAUR ) )
				send_to_char( "人馬 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_race && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_race, ALLOW_BIT ) )
		{
			send_to_char( "隱藏力量專用種族：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HUMAN ) )
				send_to_char( "人類 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ELF ) )
				send_to_char( "精靈 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DWARF ) )
				send_to_char( "矮人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ORC ) )
				send_to_char( "獸人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, FAIRY ) )
				send_to_char( "妖精 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GNOME ) )
				send_to_char( "地精 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HOBBIT ) )
				send_to_char( "賀比特 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DROW ) )
				send_to_char( "暗黑精靈 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, VAMPIRE ) )
				send_to_char( "吸血鬼 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, BEHOLDER ) )
				send_to_char( "眼魔 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, LIZARDMAN ) )
				send_to_char( "蜥蜴人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, WEREWOLF ) )
				send_to_char( "狼人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, MUMMY ) )
				send_to_char( "木乃伊 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GARGOYLE ) )
				send_to_char( "石像鬼 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, CENTAUR ) )
				send_to_char( "人馬 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_race, DENY_BIT ) )
		{
			send_to_char( "隱藏力量禁用種族：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HUMAN ) )
				send_to_char( "人類 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ELF ) )
				send_to_char( "精靈 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DWARF ) )
				send_to_char( "矮人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ORC ) )
				send_to_char( "獸人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, FAIRY ) )
				send_to_char( "妖精 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GNOME ) )
				send_to_char( "地精 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HOBBIT ) )
				send_to_char( "賀比特 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DROW ) )
				send_to_char( "暗黑精靈 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, VAMPIRE ) )
				send_to_char( "吸血鬼 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, BEHOLDER ) )
				send_to_char( "眼魔 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, LIZARDMAN ) )
				send_to_char( "蜥蜴人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, WEREWOLF ) )
				send_to_char( "狼人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, MUMMY ) )
				send_to_char( "木乃伊 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GARGOYLE ) )
				send_to_char( "石像鬼 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, CENTAUR ) )
				send_to_char( "人馬 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_class && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_class, ALLOW_BIT ) )
		{
			send_to_char( "隱藏力量專用職業：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MAGE ) ) )
				send_to_char( "魔法師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( CLERIC ) ) )
				send_to_char( "牧師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( THIEF ) ) )
				send_to_char( "妙賊 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "戰士 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "野蠻人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WIZARD ) ) )
				send_to_char( "巫師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "鍊金師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SAVANT ) ) )
				send_to_char( "賢者 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BISHOP ) ) )
				send_to_char( "主教 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "刺客 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( RANGER ) ) )
				send_to_char( "游俠 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( NINJA ) ) )
				send_to_char( "忍者 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "騎士 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "劍客 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MONK ) ) )
				send_to_char( "僧侶 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "巫醫 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SORCERER ) ) )
				send_to_char( "幻法師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( LORD ) ) )
				send_to_char( "領主 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARLORD ) ) )
				send_to_char( "戰王 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "狂戰士 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_class, DENY_BIT ) )
		{
			send_to_char( "隱藏力量禁用職業：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MAGE ) ) )
				send_to_char( "魔法師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( CLERIC ) ) )
				send_to_char( "牧師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( THIEF ) ) )
				send_to_char( "妙賊 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "戰士 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "野蠻人 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WIZARD ) ) )
				send_to_char( "巫師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "鍊金師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SAVANT ) ) )
				send_to_char( "賢者 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BISHOP ) ) )
				send_to_char( "主教 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "刺客 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( RANGER ) ) )
				send_to_char( "游俠 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( NINJA ) ) )
				send_to_char( "忍者 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "騎士 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "劍客 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MONK ) ) )
				send_to_char( "僧侶 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "巫醫 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SORCERER ) ) )
				send_to_char( "幻法師 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( LORD ) ) )
				send_to_char( "領主 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARLORD ) ) )
				send_to_char( "戰王 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "狂戰士 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_sex && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_sex, ALLOW_BIT ) )
		{
			send_to_char( "隱藏力量專用性別：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_MALE ) ) )
				send_to_char( "男性 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_FEMALE ) ) )
				send_to_char( "女性 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_NEUTRAL ) ) )
				send_to_char( "中性 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_sex, DENY_BIT ) )
		{
			send_to_char( "隱藏力量禁用性別：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_MALE ) ) )
				send_to_char( "男性 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_FEMALE ) ) )
				send_to_char( "女性 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_NEUTRAL ) ) )
				send_to_char( "中性 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_align && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_align, ALLOW_BIT ) )
		{
			send_to_char( "隱藏力量專用陣營：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_EVIL ) ) )
				send_to_char( "邪惡 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_GOOD ) ) )
				send_to_char( "善良 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
				send_to_char( "中立 ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_align, DENY_BIT ) )
		{
			send_to_char( "隱藏力量禁用陣營：", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_EVIL ) ) )
				send_to_char( "邪惡 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_GOOD ) ) )
				send_to_char( "善良 ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
				send_to_char( "中立 ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	/*
	if( obj->pIndexData->hp_affect_race && level >= L_JUN )
	{
		send_to_char( "隱藏力量克制種族：", ch );
		int race_count = 0;
		for( long long int temp_race = obj->pIndexData->hp_affect_race; temp_race != 0; )
		{
		  if( IS_SET( obj->pIndexData->hp_affect_race, NUM_BIT( race_count ) )
		  {
			arg = race_table[race_count].cname;
			sent_to_char( strcat(arg, " "), ch );
			temp_race -= NUM_BIT( race_count );
		  }
		  race_count++;
		}
		send_to_char( "\n\r", ch );
		}
		else if(  obj->pIndexData->hp_noeffect_race && level >= L_JUN )
		{

		send_to_char( "隱藏力量無效種族：", ch );
		int race_count = 0;
		for( long long int temp_race = obj->pIndexData->hp_noeffect_race; temp_race != 0; )
		{
		  if( IS_SET( obj->pIndexData->hp_noeffect_race, NUM_BIT( race_count ) )
		  {
			arg = race_table[race_count].cname;
			sent_to_char( strcat(arg, " "), ch );
			temp_race -= NUM_BIT( race_count );
		  }
		  race_count++;
		}
		send_to_char( "\n\r", ch );
		}
		if( obj->pIndexData->hp_affect_class && level >= L_JUN )
		{
		if ( IS_SET( obj->pIndexData->hp_affect_class, ALLOW_BIT ) )
		{
		  send_to_char( "隱藏力量克制職業：", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MAGE ) ) )
			send_to_char( "魔法師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( CLERIC ) ) )
			send_to_char( "牧師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( THIEF ) ) )
			send_to_char( "妙賊 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARRIOR ) ) )
			send_to_char( "戰士 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BARBARIAN ) ) )
			send_to_char( "野蠻人 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WIZARD ) ) )
			send_to_char( "巫師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ALCHEMIST ) ) )
			send_to_char( "鍊金師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SAVANT ) ) )
			send_to_char( "賢者 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BISHOP ) ) )
			send_to_char( "主教 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ASSASSIN ) ) )
			send_to_char( "刺客 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( RANGER ) ) )
			send_to_char( "游俠 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( NINJA ) ) )
			send_to_char( "忍者 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( KNIGHT ) ) )
			send_to_char( "騎士 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SWORDSMAN ) ) )
			send_to_char( "劍客 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MONK ) ) )
			send_to_char( "僧侶 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SHAMAN ) ) )
			send_to_char( "巫醫 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SORCERER ) ) )
			send_to_char( "幻法師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( LORD ) ) )
			send_to_char( "領主 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARLORD ) ) )
			send_to_char( "戰王 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MARTIALMAN ) ) )
			send_to_char( "狂戰士 ", ch );
		  send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_affect_class, DENY_BIT ) )
		{
		  send_to_char( "隱藏力量無效職業：", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MAGE ) ) )
			send_to_char( "魔法師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( CLERIC ) ) )
			send_to_char( "牧師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( THIEF ) ) )
			send_to_char( "妙賊 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARRIOR ) ) )
			send_to_char( "戰士 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BARBARIAN ) ) )
			send_to_char( "野蠻人 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WIZARD ) ) )
			send_to_char( "巫師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ALCHEMIST ) ) )
			send_to_char( "鍊金師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SAVANT ) ) )
			send_to_char( "賢者 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BISHOP ) ) )
			send_to_char( "主教 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ASSASSIN ) ) )
			send_to_char( "刺客 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( RANGER ) ) )
			send_to_char( "游俠 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( NINJA ) ) )
			send_to_char( "忍者 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( KNIGHT ) ) )
			send_to_char( "騎士 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SWORDSMAN ) ) )
			send_to_char( "劍客 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MONK ) ) )
			send_to_char( "僧侶 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SHAMAN ) ) )
			send_to_char( "巫醫 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SORCERER ) ) )
			send_to_char( "幻法師 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( LORD ) ) )
			send_to_char( "領主 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARLORD ) ) )
			send_to_char( "戰王 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MARTIALMAN ) ) )
			send_to_char( "狂戰士 ", ch );
		  send_to_char( "\n\r", ch );
		}
		}
		if( obj->pIndexData->hp_affect_sex && level >= L_JUN )
		{
		if ( IS_SET( obj->pIndexData->hp_affect_sex, ALLOW_BIT ) )
		{
		  send_to_char( "隱藏力量克制性別：", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_MALE ) ) )
			send_to_char( "男性 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_FEMALE ) ) )
			send_to_char( "女性 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_NEUTRAL ) ) )
			send_to_char( "中性 ", ch );
		  send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_affect_sex, DENY_BIT ) )
		{
		  send_to_char( "隱藏力量無效性別：", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_MALE ) ) )
			send_to_char( "男性 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_FEMALE ) ) )
			send_to_char( "女性 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_NEUTRAL ) ) )
			send_to_char( "中性 ", ch );
		  send_to_char( "\n\r", ch );
		}
		}
		if( obj->pIndexData->hp_affect_align && level >= L_JUN )
		{
		if ( IS_SET( obj->pIndexData->hp_affect_sex, ALLOW_BIT ) )
		{
		  send_to_char( "隱藏力量克制陣營：", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_align, NUM_BIT( ALIGN_EVIL ) ) )
			send_to_char( "邪惡 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_align, NUM_BIT( ALIGN_GOOD ) ) )
			send_to_char( "善良 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
			send_to_char( "中立 ", ch );
		  send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_affect_sex, DENY_BIT ) )
		{
		  send_to_char( "隱藏力量無效陣營：", ch );
		  if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_EVIL ) ) )
			send_to_char( "邪惡 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_GOOD ) ) )
			send_to_char( "善良 ", ch );
		  if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
			send_to_char( "中立 ", ch );
		  send_to_char( "\n\r", ch );
		}
	}
	*/
	if ( level > L_APP - 2 )
	{
		OBJ_VRACE * ovr = NULL;
		char buf2 [ MAX_STRING_LENGTH * 2];
		if( obj->vrace ) {
			sprintf(buf, "對種族加成：\n\r" );
			send_to_char(buf, ch);
			for( ovr = obj->vrace ; ovr ; ovr = ovr->next ) {
				// 加入種族英文名，並將每一項訊息分開送出，以加入正負加成不同的顏色 2023/03/24
				sprintf(buf2, "%s(%s)", race_table[ovr->race].cname, race_table[ovr->race].name );
				sprintf(buf, "%-19s", buf2 );
				send_to_char(buf, ch);
				if( ovr->hr_mod_q > 0 )
					sprintf(buf, "命中率 "HIC"%d"NOR" 點，", ovr->hr_mod_q );
				else if( ovr->hr_mod_q < 0 )
					sprintf(buf, "命中率 "HIR"%d"NOR" 點，", ovr->hr_mod_q );
				else
					sprintf(buf, "命中率 %d 點，", ovr->hr_mod_q );
				send_to_char(buf, ch);
				if( ovr->hr_mod_p > 100 )
					sprintf(buf, "百分比 "HIC"%d%%"NOR"；", ovr->hr_mod_p );
				else if( ovr->hr_mod_p < 100 )
					sprintf(buf, "百分比 "HIR"%d%%"NOR"；", ovr->hr_mod_p );
				else
					sprintf(buf, "百分比 %d%%；", ovr->hr_mod_p );
				send_to_char(buf, ch);
				if( ovr->dr_mod_q > 0 )
					sprintf(buf, "傷害力 "HIC"%d"NOR" 點，", ovr->dr_mod_q );
				else if( ovr->dr_mod_q < 0 )
					sprintf(buf, "傷害力 "HIR"%d"NOR" 點，", ovr->dr_mod_q );
				else
					sprintf(buf, "傷害力 %d 點，", ovr->dr_mod_q );
				send_to_char(buf, ch);
				if( ovr->dr_mod_p > 100 )
					sprintf(buf, "百分比 "HIC"%d%%"NOR"。\n\r", ovr->dr_mod_p );
				else if( ovr->dr_mod_p < 100 )
					sprintf(buf, "百分比 "HIR"%d%%"NOR"。\n\r", ovr->dr_mod_p );
				else
					sprintf(buf, "百分比 %d%%。\n\r", ovr->dr_mod_p );
				send_to_char(buf, ch);
				/*sprintf(buf, "對%s加成命中率%d點，百分比%d%%；傷害力%d點，百分比%d%%。\n\r",
							race_table[ovr->race].cname,
							ovr->hr_mod_q,
							ovr->hr_mod_p,
							ovr->dr_mod_q,
							ovr->dr_mod_p);
				send_to_char(buf, ch);*/
			}
		}
		if( obj->material->vrace ) {
			sprintf(buf, "材質對種族加成：\n\r" );
			send_to_char(buf, ch);
			for( ovr = obj->material->vrace ; ovr ; ovr = ovr->next ) {
				// 加入種族英文名，並將每一項訊息分開送出，以加入正負加成不同的顏色 2023/03/24
				sprintf(buf2, "%s(%s)", race_table[ovr->race].cname, race_table[ovr->race].name );
				sprintf(buf, "%-19s", buf2 );
				send_to_char(buf, ch);
				if( ovr->hr_mod_q > 0 )
					sprintf(buf, "命中率 "HIC"%d"NOR" 點，", ovr->hr_mod_q );
				else if( ovr->hr_mod_q < 0 )
					sprintf(buf, "命中率 "HIR"%d"NOR" 點，", ovr->hr_mod_q );
				else
					sprintf(buf, "命中率 %d 點，", ovr->hr_mod_q );
				send_to_char(buf, ch);
				if( ovr->hr_mod_p > 100 )
					sprintf(buf, "百分比 "HIC"%d%%"NOR"；", ovr->hr_mod_p );
				else if( ovr->hr_mod_p < 100 )
					sprintf(buf, "百分比 "HIR"%d%%"NOR"；", ovr->hr_mod_p );
				else
					sprintf(buf, "百分比 %d%%；", ovr->hr_mod_p );
				send_to_char(buf, ch);
				if( ovr->dr_mod_q > 0 )
					sprintf(buf, "傷害力 "HIC"%d"NOR" 點，", ovr->dr_mod_q );
				else if( ovr->dr_mod_q < 0 )
					sprintf(buf, "傷害力 "HIR"%d"NOR" 點，", ovr->dr_mod_q );
				else
					sprintf(buf, "傷害力 %d 點，", ovr->dr_mod_q );
				send_to_char(buf, ch);
				if( ovr->dr_mod_p > 100 )
					sprintf(buf, "百分比 "HIC"%d%%"NOR"。\n\r", ovr->dr_mod_p );
				else if( ovr->dr_mod_p < 100 )
					sprintf(buf, "百分比 "HIR"%d%%"NOR"。\n\r", ovr->dr_mod_p );
				else
					sprintf(buf, "百分比 %d%%。\n\r", ovr->dr_mod_p );
				send_to_char(buf, ch);
				/*sprintf(buf, "材質對%s族加成命中率%d點，百分比%d%%；傷害力%d點，百分比%d%%。\n\r",
							race_table[ovr->race].cname,
							ovr->hr_mod_q,
							ovr->hr_mod_p,
							ovr->dr_mod_q,
							ovr->dr_mod_p); */
			}
			//send_to_char(buf, ch);
		}
	}

	if( level > L_APP -1 ) 
	{
		if ( obj->timer > 0 )
		{
			sprintf( buf, "物品尚可使用 %d 小時\n\r", obj->timer );
			send_to_char( buf, ch );
		}
		else
			send_to_char( "物品無時間限制\n\r", ch );
	}

	if ( obj->hp || obj->material->hp)
	{
		if (IS_IMMORTAL(ch) || level > L_APP )
		{
			OBJ_HP * ohp ;
			if( obj->hp ){
				for( ohp = obj->hp; ohp; ohp = ohp->next ) {
					if( IS_SET( ohp->event, HP_WEAR ) )
						sprintf( buf, "裝備觸發隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_FIGHT ) )
						sprintf( buf, "命中觸發隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_WEAPON ) )
						sprintf( buf, "命中觸發隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_BLOCK ) )
						sprintf( buf, "格擋觸發隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_ARROW ) )
						sprintf( buf, "射擊觸發隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_REMOVE ) )
						sprintf( buf, "隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_DART ) )
						sprintf( buf, "投擲觸發隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					send_to_char( buf, ch );
				}
			}
			if( obj->material->hp ){
				for( ohp = obj->material->hp; ohp; ohp = ohp->next ) {
					sprintf( buf, "材質隱藏力量: %d 級的 %s, 機率 %d%%\n\r", 
									ohp->level, ohp->hp_name, ohp->percent);
					send_to_char( buf, ch );
				}
			}
		}
		else
			send_to_char( "你隱隱感到有股魔力藏在其中，卻又說不出是什麼力量。", ch );
	}
//    WAIT_MAGIC_STATE(ch, 8);
	return;
}


void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *obj;
	OBJ_DATA *in_obj;
	char buf [ MAX_INPUT_LENGTH ];
	char buf1 [ MAX_STRING_LENGTH*8 ] ;
	bool found;
	int count = 0;
	int count_buf = 0; // 2022/10/14
	int sklv = get_skill_level(ch,sn);
	//int maxcount = level / 3 + sklv * 4 + (sklv /2)*3;  2022/10/14
	int maxcount = level / 2 + 2 * (sklv * sklv) + 8;

	// 阻擋 npc 使用 locate object 2022/10/14
	if( IS_NPC(ch) ) return;

	buf1[0] = '\0';
	found = FALSE;
	// 移除 sklv 6 locate 不限數量 2022/10/14
	//for ( obj = object_list; obj && (sklv > 5 || count <= maxcount) ; obj = obj->next )
	for ( obj = object_list; obj && count <= maxcount ; obj = obj->next )
	{
		if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ) )
			continue;

		//found = TRUE;

		for ( in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj )
			;

		/*  2022/10/14
			修改為 carried_by 及 !carried_by 兩類 , 將 can_see 放入 carried_by 下判定
			如此 !carried_by 的部分將不會判定到 carried_by 的物品
		*/
		//if ( in_obj->carried_by && can_see( ch, in_obj->carried_by) )
		if ( in_obj->carried_by )
		{
			// 看不見持有者的情況 , 直接略過物品 2022/10/14
			if( !can_see( ch, in_obj->carried_by) )
				continue;

			found = TRUE;

			++count;
			++count_buf;
			// 新增技能等級 5 以上時會顯示區域位置 2022/10/14
			if( sklv >= 5 ){
				sprintf( buf, "%s 由 %s 攜帶 位於 %s.\n\r",
								obj->short_descr,
								PERS( in_obj->carried_by, ch ),
								in_obj->carried_by->in_room->area->name
						);
			}
			else
				sprintf( buf, "%s 由 %s 攜帶.\n\r",
								obj->short_descr,
								PERS( in_obj->carried_by, ch )
								);
		}
		else
		{
			found = TRUE;
			++count;
			++count_buf;
			// 新增技能等級 5 以上時會顯示區域位置 2022/10/14
			if( sklv >= 5 )
				sprintf( buf, "%s 擺\在 %s %s %s.\n\r",
								obj->short_descr,
								!in_obj->in_room ? "某處" : in_obj->in_room->name,
								!in_obj->in_room ? "" : "位於",
								!in_obj->in_room ? "" : in_obj->in_room->area->name
						);
			else
				// 移除 obj->name 2022/10/14
				//sprintf( buf, "%s(%s) 擺\在 %s.\n\r", obj->short_descr, obj->name, !in_obj->in_room ? "某處" : in_obj->in_room->name);

				sprintf( buf, "%s 擺\在 %s.\n\r",
								obj->short_descr,
								!in_obj->in_room ? "某處" : in_obj->in_room->name
						);
		}

		buf[0] = UPPER( buf[0] );
		strcat( buf1, buf );

		// 把 count 換為 count_buf 來判定 , 並修改為當前 pagelen 最大行數(70) 2022/10/14
		//if( count >= 50 )
		if( count_buf >= 80 )
		{
			//count = 0;
			count_buf = 0;
			send_to_char(buf1, ch);
			buf1[0] = '\0';
		}
	}

	if ( found )
		send_to_char( buf1, ch );
	else
		send_to_char( "世界上沒有這樣東西.\n\r", ch );

	return;
}

void spell_endless_sight( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;
	char buf  [ MAX_STRING_LENGTH   ];
	char buf1 [ MAX_STRING_LENGTH*8 ];
	char buf2 [ MAX_STRING_LENGTH   ];
	bool found;
	int count = 0;
	int count_buf = 0; // 2022/10/14
	int sklv = get_skill_level(ch,sn);
	//int maxcount = level / 3 + sklv * 4 + (sklv /2)*3;  2022/10/14
	int maxcount = level / 2 + 2 * (sklv * sklv) + 8;


	sprintf( buf2, "你召喚風的精靈，請她們吹拂到這個世界的每個角落....\n\r\n\r" );

	found = FALSE;
	buf1[0] = '\0';
	// 移除 sklv 6 不限數量的設定 2022/10/14
	//for ( victim = char_list; victim && (sklv > 5 || count <= maxcount); victim = victim->next )
	for ( victim = char_list; victim && count <= maxcount ; victim = victim->next )
	{
		// 2022/10/14
		if( is_qmark( victim, "disable_endless_sight") )
			continue;

		if ( victim->in_room
			&& can_see( ch, victim )
			// && IS_NPC( victim )
			&& is_name( target_name, victim->name ) )
		{
			found = TRUE;
			++count;
			++count_buf;
			sprintf( buf, "%s 位在 %s地區 - %s\n\r",
							victim->short_descr,
							victim->in_room->area->name,
							victim->in_room->name
					);
			strcat( buf1, buf );
			// 將 count 修改為 count_buf ,並修改為符合最大 pagelen 2022/10/14
			//if( count >= 50 ) {
			if( count_buf >= 80 )
			{
				//count = 0;
				count_buf = 0;
				// 如果抓count >= 80 以上, 則 buf2 在第一次輸出時先輸出 2022/10/14
				strcat( buf2, "風的精靈回來，娓娓告訴你她在旅途上遇到的生物：\n\r" );
				send_to_char( buf2, ch );
				send_to_char( buf1, ch );
				buf1[0] = '\0';
			}
		}
	}

	if ( !found )
	{
		strcat( buf2, "風的精靈很遺憾地告訴你，她們沒有看到任何類似你描述的生物。\n\r" );
		send_to_char( buf2, ch );
	}
	else
	{
		// count >= 80 的情況下, 在 for 內會預先輸出 buf2  2022/10/14
		if( count <= 79 )
		{
			strcat( buf2, "風的精靈回來，娓娓告訴你她在旅途上遇到的生物：\n\r" );
			send_to_char( buf2, ch );
		}
		send_to_char( buf1, ch );
	}
	return;
}

void spell_grave_robber( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *obj;
	CHAR_DATA *victim = ( CHAR_DATA *) vo;
	char buf [ MAX_INPUT_LENGTH ];
	char buf2 [ MAX_INPUT_LENGTH ]; // 2023/01/13
	bool found = FALSE;
	int difflv = 0;
	int count = 0;
	int sklv = get_skill_level(ch,sn);

	// 2023/01/13
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
		level = obj_lv + sklv;
	}else{
		level += sklv;
	}

	if ( IS_NPC( victim ) ) return;
	if( IS_NPC(ch) || IS_IMMORTAL(ch) ) sklv = 100;
	//if ( ch == victim && sklv <=3 ) return;

	level += sklv;

	if( !IS_NPC(ch) && victim->level > level )
	{
		// 把 difflv 修正為正值, 否則下方判定式不作用 2023/01/13
		//difflv = level - victim->level;
		difflv = victim->level - level;
		if( victim->level > 41 )
			difflv *= 10000;
		else if( victim->level > 40 )
			difflv *= 3000;
		else if( victim->level > 39 )
			difflv *= 1000;
		else if( victim->class > BARBARIAN )
			difflv *= 500;
		else
			difflv *= 300;

		// 修改為只有 ch 會扣除 exp 2023/01/13
		//if( ch->exp < difflv + 1000 || victim->exp < 1000 + difflv )
		if( ch->exp < difflv + 1000 )
		{
			send_to_char("你的能力不足以幫助他.\n\r", ch);
			return;
		}

		// 將 difflv 轉換為負值 2023/01/13
		difflv = 0 - difflv;

		// 修改為只有 ch 會扣除 exp 2023/01/13
		gain_exp( ch , difflv );
		//gain_exp( victim, difflv);
		sprintf(buf,"你失去了 %d 點經驗值.\n\r", difflv * -1 );
		send_to_char(buf, ch);
		//send_to_char(buf, victim);
	}

	// 把 grave 移到 exp 判定後 2023/01/13

	sprintf( buf, "%s 的屍體(corpse)", victim->name );

	sprintf( buf2, "%s", victim->short_descr );


	for ( obj = object_list; obj; obj = obj->next )
	{
		if ( !can_see_obj( ch, obj ) || ( str_cmp( buf, obj->short_descr ) ) )
			continue;

		// 阻擋非 pc corpse 被 grave 到 2023/01/13
		if ( obj->item_type != ITEM_CORPSE_PC )
			continue;

		found = TRUE;

		if( obj->carried_by )
			obj_from_char(obj);
		else
			obj_from_room( obj );

		if( !victim || !victim->in_room || victim->in_room != ch->in_room )
			return;

		obj_to_room( obj, ch->in_room );
		// 修改為 act 顯示屍體及 ch 2023/01/13
		//act( "一具屍體突然出現在你面前！", ch, NULL, NULL, TO_ROOM );
		//send_to_char( "你把屍體移動到了這裡。\n\r", ch );
		act( "$n把一具$T的屍體移動到了這裡。", ch, NULL, buf2, TO_ROOM );
		act( "你把一具$T的屍體移動到了這裡。", ch, NULL, buf2, TO_CHAR );
		++count;
		if( !IS_NPC(ch) && sklv < 6 && count > sklv/2 )
			break; 
	}

	if ( !found )
	{
		send_to_char( "他看來不需要你的幫助.\n\r", ch );
		return;
	}

	return;
}


void spell_recharge_item ( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	int nSuccess  = 0;
	int nSklv     = get_skill_level(ch,sn);
	int nPerc     =  get_skill_percent(ch, sn);

	if (obj->item_type != ITEM_WAND && obj->item_type != ITEM_STAFF )
	{
		send_to_char( "那不能被充能.\n\r", ch );
		return;
	}

	nSuccess = get_curr_wis( ch ) + nSklv * (nSklv + 4) + nPerc / 2 + 2 * (ch->level - obj->value[0]);

	if (!IS_NPC( ch ) && number_percent() < nSuccess && obj->value[2] < obj->value[1])
	{
		act( "$p 泛起一陣亮光，然後又漸漸恢復原樣.", ch, obj, NULL, TO_CHAR );
		act( "$p 泛起一陣亮光，然後又漸漸恢復原樣.", ch, obj, NULL, TO_ROOM );
		obj->value[2] = obj->value[1];
		obj->cost     = 1;
	}else{
		// 有 z4 的 wand staff 不會被充爆 add at 2020/10/18
		if( IS_SET(obj->pIndexData->souvenir, Z4) )
		{
			act( "$p 泛起一陣亮光，又突然暗了下來...", ch, obj, NULL, TO_CHAR );
			act( "$p 泛起一陣亮光，又突然暗了下來...", ch, obj, NULL, TO_ROOM );
		}
		else
		{
			act( "$p 泛起一陣亮光，然後突然炸成碎片！", ch, obj, NULL, TO_CHAR );
			act( "$p 泛起一陣亮光，然後突然炸成碎片！", ch, obj, NULL, TO_ROOM );
			extract_obj( obj );
			damage( ch, ch, ch->max_hit / 16, sn, WEAR_NONE, MSG_NORMAL, DMG_NORMAL );
		}
	}

	return;
}

void spell_remove_nodrop ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj= (OBJ_DATA *) vo;

    if ( !IS_SET( obj->extra_flags, ITEM_NODROP ) )
    {
  send_to_char( "沒有什麼值得描述的事發生.\n\r", ch );
  return;
    }

  SET_BIT( obj->extra_flags, ITEM_REMOVE_NODROP );
  act( "一道金色的光芒籠罩著$p.",
      ch, obj, NULL, TO_CHAR );
  act( "一道金色的光芒籠罩著$p.",
      ch, obj, NULL, TO_ROOM );

    return;
}



/* Remove Alignment by Thelonius for EnvyMud */
void spell_remove_alignment( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( !IS_SET( obj->extra_flags, ITEM_EVIL   )
    &&   !IS_SET( obj->extra_flags, ITEM_ANTI_GOOD  )
    &&   !IS_SET( obj->extra_flags, ITEM_ANTI_EVIL  )
    &&   !IS_SET( obj->extra_flags, ITEM_ANTI_NEUTRAL ) )
    {
  send_to_char( "什麼事也沒發生.\n\r", ch );
  return;
    }

    if ( !IS_NPC( ch ) && number_percent( ) + 15 < get_skill_percent(ch, sn)* //ch->pcdata->learned[sn] *
   ( 33 +
    ( 33 * ( ch->level - obj->level ) / (float)LEVEL_HERO ) ) / 100.0 )
    {
  REMOVE_BIT( obj->extra_flags, ITEM_EVIL   );
  REMOVE_BIT( obj->extra_flags, ITEM_ANTI_GOOD  );
  REMOVE_BIT( obj->extra_flags, ITEM_ANTI_EVIL  );
  REMOVE_BIT( obj->extra_flags, ITEM_ANTI_NEUTRAL );
  //obj->level    = UMAX(obj->level, number_fuzzy( level - 3 ));
  obj->level = 38;
  act( "$p 發出一陣低鳴，然後又安靜下來.", ch, obj, NULL, TO_CHAR );
  act( "$p 發出一陣低鳴，然後又安靜下來.", ch, obj, NULL, TO_ROOM );
  return;
    }

    SET_BIT( obj->extra_flags, ITEM_NODROP );
    obj->wear_flags = 1;      /* Useless   */
    obj->cost     = 0;      /* Worthless */
    act( "$p 發出一陣強光，然後變成灰色.", ch, obj, NULL, TO_CHAR );
    act( "$p 發出一陣強光，然後變成灰色.", ch, obj, NULL, TO_ROOM );

    return;

}



/* Expulsion of ITEM_NOREMOVE addition by Katrina */
void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int        iWear;
    int        yesno  = 0;

	if ( !IS_NPC( victim ) || IS_AFFECTED( victim, AFF_CHARM ) ||
		IS_AFFECTED( victim, AFF_SUMMONED ) ){
		// 新增裝備位置 彈藥(WEAR_AMMO) 故增加 iWear 數值 2022/04/30
		//for ( iWear = 0; iWear < 29; iWear ++ )
		for ( iWear = 0; iWear < 30; iWear ++ )
		{
			if ( !( obj = get_eq_char( victim, iWear ) ) )
				continue;
			if ( IS_SET( obj->extra_flags, ITEM_NOREMOVE ) && !(obj->pIndexData->horse))
			{
				unequip_char( victim, obj );
				obj_from_char( obj );
				if ( IS_SET( obj->extra_flags , ITEM_PERSONAL) )
					obj_to_char( obj, victim );
				else
					obj_to_room( obj, victim->in_room );
		 
				act( "你丟下 $p.",  victim, obj, NULL, TO_CHAR );
				act( "$n 丟下 $p.", victim, obj, NULL, TO_ROOM );
				yesno = 1;
			}
		}
	}
    if ( is_affected( victim, gsn_curse ) )
    {
		affect_strip( victim, gsn_curse );
		send_to_char( "你覺得好多了.\n\r", victim );
		yesno = 1;
	}

	if ( ch != victim && yesno )
		send_to_char( "好的。\n\r", ch );
	return;
}


void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;

	// 分開 summon 失敗條件, 並加入新設定 2022/12/20
	if( !( victim = get_char_world( ch, target_name ) ) )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	// room 及 area 判定 2022/12/20
	if(    room_is_private( victim , ch->in_room )
		|| victim->in_room->area != ch->in_room->area
		|| IS_SET( victim->in_room->room_flags, ROOM_SAFE )
		|| IS_SET( victim->in_room->room_flags, ROOM_SUMMON_BIT )
		|| IS_SET( ch->in_room->room_flags, ROOM_SUMMON_BIT ) )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	// 角色狀態判定 2022/12/20
	if(    victim == ch
		|| !victim->in_room
		|| ( !IS_NPC(victim) && !victim->desc )  // 連線狀態判定加入 pc 限定 2022/12/20
		|| ( get_skill_level(ch,sn) < 5 && IS_AFFECTED( victim, AFF_CURSE ) )
		|| victim->level > ch->level + get_skill_level( ch, sn )
		|| victim->fighting )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	// 修改 npc 被 summon 的判定 2022/12/20
	if( IS_NPC( victim ) )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	act( "$n 突然從你面前消失.", victim, NULL, NULL, TO_ROOM );

	// 增加 summon 對 room_leave 作判定 2022/12/20
	if( victim->in_room->roomprogs && victim->in_room->progtypes & ROOM_LEAVE )
		rprog_leave_trigger( victim->in_room, victim , 8);

	char_from_room( victim );
	char_to_room( victim, ch->in_room );

	act( "$n 召喚了你！", ch, NULL, victim, TO_VICT );
	act( "$n 突然出現在這裡.", victim, NULL, NULL, TO_ROOM );
	do_look( victim, "auto" );

	// 增加 summon 對 room_enter , entry_prog , greet_prog 判定 2022/12/20
	if ( victim->in_room->roomprogs && ( victim->in_room->progtypes & ROOM_ENTER ) && victim->position > POS_DEAD )
		rprog_entry_trigger( victim->in_room , victim, 6 );

	// 新增 npc hurt prog 2022/12/20
	if( IS_NPC(victim) )
		mprog_hurt_trigger( victim, ch, sn );

		/*if ( IS_SET( victim->act, PLR_HORSE ) )
		{
			horse = victim->pcdata->horse;
			stop_fighting( horse, TRUE );
			act( "[36m$n 突然從你面前消失。[m", horse, NULL, NULL, TO_ROOM );
			char_from_room( horse );
			char_to_room( horse, ch->in_room );
			act( "[36m$n 突然出現在這裡。[m", horse, NULL, NULL, TO_ROOM );
		}*/
	return;
}



void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo )
{
	//The max times the for loop search a legal room to teleport in.
	const int TIMES_LIMIT = 500;
	//The count of for loop.
	int times_of_search = 0;

	CHAR_DATA *victim = (CHAR_DATA *) vo;
	// CHAR_DATA *horse;
	ROOM_INDEX_DATA *pRoomIndex = NULL;

	if ( !victim->in_room || !victim->desc
		|| IS_SET( victim->in_room->room_flags, ROOM_NO_RECALL)
		|| IS_SET( victim->in_room->room_flags, ROOM_NO_TELEPORT)
		|| ( !IS_NPC( ch ) && victim->fighting )
		|| ( victim != ch
			&& ( new_saves_spell( ch->pp, victim, MAGIC_WIND )
				+ get_skill_level(ch,sn) < (number_percent() * 100 / weakness_percent(victim, sn)) ) ) )
	{
		send_to_char( "你失敗了.\n\r", ch );
		return;
	}

	if( auction_list ) {
		AUC_DATA * auction, *auc_next;
		for( auction = auction_list ; auction; auction = auc_next )
		{
			if ( victim == auction->owner || victim == auction->buyer )
			{
				send_to_char( "你失敗了.\n\r", ch );
				return;
			}
			auc_next = auction->next;
		}
	}

	for( times_of_search=0 ;times_of_search < TIMES_LIMIT;times_of_search++ )
	{
		pRoomIndex = get_room_index( number_range( 0, MAX_VNUM  ) );
		if ( pRoomIndex )
		{
			if ( pRoomIndex->area->recall != victim->in_room->area->recall )
				continue;
			if (   !IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE  )
				//&& !IS_SET( pRoomIndex->room_flags, ROOM_SOLITARY )
				&& !IS_SET( pRoomIndex->room_flags, ROOM_NO_TELEPORT_TO)
				&& !IS_SET( pRoomIndex->area->area_flags, AREA_CHANGED)
				&& !IS_SET( pRoomIndex->area->area_flags, AREA_BUILDING)
				&& !IS_SET( pRoomIndex->area->area_flags, AREA_CLAN)
				&& pRoomIndex->area->recall == victim->in_room->area->recall )
			break;
		}
	}

	if( times_of_search == TIMES_LIMIT )
	{
		send_to_char( "這個空間似乎沒有可以讓你著陸的地方!\n\r", ch );
		return;
	}

	if( !str_cmp( ch->in_room->area->identity, "immhall" ) )
	{
		send_to_char( "這個空間似乎沒有可以讓你著陸的地方!\n\r", ch );
		return;
	}

	if( victim->in_room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger( victim->in_room, victim, 8 );
	if ( victim->fighting )
		stop_fighting( victim, TRUE );
	act( "$n 的身影逐漸消失.", victim, NULL, NULL, TO_ROOM );
	send_to_char( "你被強迫傳送到另一個空間！\n\r", victim );
	char_from_room( victim );
	char_to_room( victim, pRoomIndex );
	act( "$n 的身影逐漸出現.",   victim, NULL, NULL, TO_ROOM );
	do_look( victim, "auto" );
	if( pRoomIndex->progtypes & ROOM_ENTER)
		rprog_entry_trigger(pRoomIndex, victim, 6);
	if ( victim->position > POS_DEAD ) // Added by Amethyst
		mprog_greet_trigger( victim ); // Added by Amethyst
	/*
		if ( IS_SET( victim->act, PLR_HORSE ) )
		{
		horse = victim->pcdata->horse;
		stop_fighting( horse, TRUE );
		act( "[36m$n 的身影逐漸消失。[m", horse, NULL, NULL, TO_ROOM );
		char_from_room( horse );
		char_to_room( horse, pRoomIndex );
		act( "[36m$n 的身影逐漸出現。[m", horse, NULL, NULL, TO_ROOM );
		}
	*/
	return;
}


void spell_magic_arrow( int sn, int level, CHAR_DATA *ch, void *vo )
{
	OBJ_DATA    *pArrow;
	AFFECT_DATA *pAffect;
	int          nSklv = get_skill_level(ch,sn);

	if( IS_NPC(ch) ) return;
	if( ch->mana < level * 10 ) {
		send_to_char("你的法力不夠了。\n\r", ch);
		return;
	}
	ch->mana -= level * 8 ;
	ch->mana  = ch->mana + nSklv * 10;
	send_to_char("你集中精神，創造出魔法箭！\n\r", ch);
	pArrow = fcreate_object( get_obj_index(OBJ_VNUM_MAGIC_ARROW), level );

	// 箭支傷害力: pArrow->value[1] - pArrow->value[2] (平均 52 + arrow->value[3]).
		// 還有 arrow->value[0] 支.
	pArrow->value[0] = 200 + nSklv * 100;
	pArrow->value[1] = UMIN( 60 + nSklv * (10 + (ch->max_mana)/550) , 50 + nSklv * (4 + (ch->max_mana)/300) );
	pArrow->value[2] = 60 + nSklv * (10 + (ch->max_mana)/550);
	pArrow->value[3] = 30 + nSklv * 20;

	//  將 apply 修改為 apply_shoot, 使這些傷害僅在 shot 時作用  2022/05/06
	if(nSklv >= 4){
		pAffect            = new_affect();
		pAffect->type      = pArrow->pIndexData->vnum;
		pAffect->duration  = -1;
		//pAffect->location  = APPLY_HITROLL;
		pAffect->location  = APPLY_SHOOT_HITROLL;
		pAffect->modifier  = 15 + (nSklv - 4) * 5;
		pAffect->bitvector = 0;
		pAffect->next      = pArrow->affected;
		pArrow->affected   = pAffect;
	}

	if(nSklv >= 4){
		pAffect            = new_affect();
		pAffect->type      = pArrow->pIndexData->vnum;
		pAffect->duration  = -1;
		//pAffect->location  = APPLY_DAMROLL;
		pAffect->location  = APPLY_SHOOT_DAMROLL;
		pAffect->modifier  = 10 + (nSklv - 4) * 5;
		pAffect->bitvector = 0;
		pAffect->next      = pArrow->affected;
		pArrow->affected   = pAffect;
	}

	if(nSklv >= 5){
		pArrow->value[0]  += 400;
		pAffect            = new_affect();
		pAffect->type      = pArrow->pIndexData->vnum;
		pAffect->duration  = -1;
		//pAffect->location  = APPLY_ADAMAGE;
		pAffect->location  = APPLY_SHOOT_ADAMAGE;
		pAffect->modifier  = 10 + (nSklv - 4) * ch->max_mana / 150;
		pAffect->bitvector = 0;
		pAffect->next      = pArrow->affected;
		pArrow->affected   = pAffect;
	}
	obj_to_char( pArrow, ch);
}

// 魔法彈製作 2022/04/30
void do_ammo( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *gem;
	OBJ_DATA *ammo;
	AFFECT_DATA *pAffect;
	int GemCount = 0;
	int GemQT = 0;
	int sn;
	int nSklv = get_skill_level(ch, gsn_artifacts_mastery );
	char SpellName [ MAX_INPUT_LENGTH ];
	char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, SpellName );

	if( ( IS_NPC(ch) && (IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED(ch, AFF_SUMMONED) ) ) )
		return;

	if( !IS_NPC( ch ) && get_skill_percent(ch, gsn_artifacts_mastery) == 0 ){
		send_to_char( "什麼？\n\r", ch );
		return;
	}

	for( gem = ch->carrying; gem; gem = gem->next_content )
	{
		if ( gem->wear_loc == WEAR_NONE
			&& can_see_obj( ch, gem )
			&& (gem->pIndexData == get_obj_index( OBJ_VNUM_POWER_GEM ) ) )
		{
			GemCount++;
			break;
		}
	}

	if( GemCount <= 0 )
	{
		send_to_char( "你身上並沒有帶魔晶石.\n\r", ch );
		return;
	}

	if( SpellName[0] == '\0' ){
		send_to_char( "製作魔法彈輸入 ammo '攻擊法術名' \n\r", ch );
		return;
	}

	if( ( sn = skill_lookup( SpellName ) ) == 0 ){
		send_to_char( "你還沒學會這個法術.\n\r", ch );
		return;
	}

	if( ( sn = skill_lookup( SpellName ) ) < 0 || ( !IS_NPC( ch ) && get_skill_percent(ch, sn) == 0 ) ){
		send_to_char( "你還沒學會這個法術.\n\r", ch );
		return;
	}

	if( skill_table[sn].target != TAR_CHAR_OFFENSIVE ){
		if(    sn != 42    // faerie fog
			&& sn != 95    // whirlwind
			&& sn != 205 ) // sun flash
		{
			send_to_char( "附加的法術必須為攻擊性法術.\n\r", ch );
			return;
		}
	}

	if( ch->move < 200 - nSklv ){
		send_to_char( "你的體力不夠了.\n\r", ch );
		return;
	}

	ch->move -= 200 - nSklv;

    if( ( !IS_NPC( ch ) && number_percent() > 1 + get_skill_percent(ch, gsn_artifacts_mastery) ) )
    {
		send_to_char( "你試著將魔力寶石製作成魔法彈藥，但失敗了。\n\r", ch );
		extract_obj( gem );
		return;
    }

	/* abs sklv GemQT
		sklv 0 = 280 - 380  , 16 - 20
		sklv 1 = 301 - 403  , 17 - 20
		sklv 2 = 328 - 444  , 18 - 21
		sklv 3 = 367 - 521  , 19 - 23
		sklv 4 = 424 - 652  , 21 - 26
		sklv 5 = 505 - 855  , 22 - 29
		sklv 6 = 616 - 1148 , 25 - 34
	*/
	GemQT = number_range( gem->value[0] , gem->value[1] );

	if( GemQT < 0 ) GemQT = 0;

	GemQT = (int)sqrt((int)(GemQT));

	ammo = create_object( get_obj_index( OBJ_VNUM_MAGIC_AMMO ), 0 );
	ammo->value[0] = 1 + GemQT/(18 - nSklv);                     // 數量
	ammo->value[1] = 30 + nSklv*10 + (GemQT*(10 + nSklv/2))/10;  // 傷害
	ammo->value[2] = UMAX( 1 , ch->level - 15 );                 // 法術等級
	ammo->value[3] = sn;                                         // 法術種類
	if( ammo->value[2] > 0 )
	{
		free_string( ammo->short_descr );
		free_string( ammo->name );
		free_string( ammo->description );
		sprintf( buf, "%s magic ammo", skill_table[sn].name );
		ammo->name = str_dup( buf );
		sprintf( buf, "%s魔法彈(%s magic ammo)", skill_table[sn].cname, skill_table[sn].name );
		ammo->short_descr = str_dup( buf );
		ammo->description = str_dup( buf );
	}

	pAffect            = new_affect();
	pAffect->type      = ammo->pIndexData->vnum;
	pAffect->duration  = -1;
	pAffect->location  = APPLY_SHOOT_HITROLL;
	pAffect->modifier  = 5 + nSklv * 2;
	pAffect->bitvector = 0;
	pAffect->next      = ammo->affected;
	ammo->affected     = pAffect;

	obj_to_char( ammo , ch );
	extract_obj( gem );

    act( "你把魔力寶石製作成 $p。", ch, ammo, NULL, TO_CHAR );

    WAIT_MAGIC_STATE( ch, skill_table[gsn_artifacts_mastery].beats );
    return;
}

/* Abstraction skill added by Amenda 12/31/00 */
void do_abstraction( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *gem;
    int sn    = skill_lookup("abstraction");
    int nSklv = get_skill_level(ch,sn);
	OBJ_DATA *obj;  // 2022/05/21
	int count = 0; // 2022/05/21

	// 對房間內魔力寶石數量作限制 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_POWER_GEM ) )
			count += 1;
	}

	if( count >= 300 ){
		send_to_char( "你失敗了.\n\r",ch);
		return;
	}


    if ( ch->mana < 500 )
    {
      send_to_char( "你沒有足夠的法力！\n\r", ch );
      return;
    }

	if ( ( IS_NPC( ch ) && (IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED(ch, AFF_SUMMONED)))
		|| ( !IS_NPC( ch ) &&
		number_percent() > 1 + get_skill_percent(ch, gsn_abstraction)))//ch->pcdata->learned[gsn_abstraction] ) )
	{
		send_to_char( "你試著將魔力提煉成魔力寶石，但失敗了。\n\r", ch );
		ch->mana -= 200;
		return;
	}

    ch->mana -= number_range( 400, 500 ) + nSklv * nSklv * nSklv * 2;
    gem = create_object( get_obj_index( OBJ_VNUM_POWER_GEM ), 0 );
    obj_to_room( gem, ch->in_room );
    gem->value[0] = 280 + nSklv * 20 + nSklv * nSklv * nSklv; // normal power min
    gem->value[1] = 380 + nSklv * 20 + nSklv * nSklv * nSklv * 3; // normal power max
    gem->value[2] = 180 + nSklv * 10; // autopower min
    gem->value[3] = 260 + nSklv * 10 + nSklv * nSklv * nSklv * 2; // autopower max
    //gem->weight -= UMAX(0, nSklv - 4);

    act( "你將全身魔力集中，一顆 $p 慢慢在眼前成形！", ch, gem, NULL, TO_CHAR );
    act( "$n 閉上眼睛鼓動法力，一顆 $p 慢慢出現在 $s 面前！\n\r", ch, gem, NULL, TO_ROOM );
    WAIT_MAGIC_STATE( ch, skill_table[gsn_abstraction].beats );
    return;
}

bool do_autopower( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char  arg [ MAX_INPUT_LENGTH ];
    int   mana;
    int   nSklv = get_skill_level(ch, skill_lookup("abstraction"));
    int   nRate = 60 + nSklv * 10;
    if(nRate > 100) nRate = 100;

    if ( !check_blind( ch ) )  return FALSE;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        return FALSE;
    }

    if ( !( obj = get_powergem_carry( ch, arg ) ) )
    {
        return FALSE;
    }

    if ( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
      || ( !IS_NPC( ch ) &&
      number_percent() > get_skill_percent(ch, gsn_abstraction)))//ch->pcdata->learned[gsn_abstraction] ) )
    {
      send_to_char( "你試著將魔晶石中的法力引發出來，但失敗了。\n\r", ch );
      extract_obj( obj );
      return TRUE;
    }
    
    if( obj->value[2] > 0 ) {
        mana = number_range( obj->value[2], obj->value[3]);
    }
    else
        mana = number_range( 200, 280 );
    mana = mana * nRate / 100;
    extract_obj( obj );
    ch->mana = UMIN( ch->mana + mana , ch->max_mana + ch->max_mana / 5 );
    send_to_char( "你將魔晶石的法力釋放，因而得到了補充。\n\r", ch );
    return TRUE;
}
/* Power gem skill added by Amenda 12/31/00 */
void do_power( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char  arg [ MAX_INPUT_LENGTH ];
	int   mana;
	int   nSklv = get_skill_level(ch, skill_lookup("abstraction"));
	int   nRate = 60 + nSklv * 10;
	if(nRate > 100) nRate = 100;

	if ( !check_blind( ch ) )
		return;

	one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
		send_to_char( "你想引發什麼?\n\r", ch );
		return;
	}

	if ( !( obj = get_powergem_carry( ch, arg ) ) )
	{
		send_to_char( "你身上並沒有帶魔晶石.\n\r", ch );
		return;
	}

	if ( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) || ( !IS_NPC( ch )
		&& number_percent() > get_skill_percent(ch, gsn_abstraction)))
	{
		send_to_char( "你試著將魔晶石中的法力引發出來，但失敗了。\n\r", ch );
		extract_obj( obj );
		return;
	}

	if( obj->value[1] > 0 ) {
		mana = number_range( obj->value[1], obj->value[2]);
	}
	else
		mana = number_range( 300, 400 );
	mana = mana * nRate / 100;
	extract_obj( obj );
	ch->mana = UMIN( ch->mana + mana , ch->max_mana + ch->max_mana / 5 );
	send_to_char( "你將魔晶石的法力釋放，因而得到了補充。\n\r", ch );
	WAIT_MAGIC_STATE( ch, 6);
	return;
}
    
/*
 * Find an power gem in player's inventory.'
 */
OBJ_DATA *get_powergem_carry( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char      arg [ MAX_INPUT_LENGTH ];
	int       number;
	int       count;

	number = number_argument( argument, arg );
	count  = 0;
	for ( obj = ch->carrying; obj; obj = obj->next_content )
	{
		if ( obj->wear_loc == WEAR_NONE
			&& can_see_obj( ch, obj )
			&& (obj->pIndexData == get_obj_index( OBJ_VNUM_POWER_GEM ) ) )
		{
			if ( ++count == number )
				return obj;
		}
	}

	return NULL;
}


void spell_resurrection( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *phorse;
	OBJ_DATA *corpse;
	CHAR_DATA *rch, *vch;
	char *arg, *arg1;
	char temp[MAX_INPUT_LENGTH];
	int sklv = get_skill_level(ch,sn);
	ROOM_INDEX_DATA *old_inroom;
	char buf  [MAX_INPUT_LENGTH];

	corpse = (OBJ_DATA*) vo;

	if( !ch->in_room ) return;
	if( !corpse )
	{
		send_to_char("你找不到屍體。\n\r", ch);
		return;
	}

	level += sklv/2;
	if( IS_NPC(ch) ){
		rch = ch;
	}else if (sklv >= 6){
		rch = ch;
	}else{
		for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
		{
			if( rch == ch || IS_NPC(rch) ) continue;
			if( !IS_AWAKE( rch ) ) continue;
			if( !is_same_group(ch, rch) ) continue;
			if( rch->pcdata->autocombat != AUTOC_COOPERATE ) continue;
			if( rch->class == SAVANT ) break;
		}
		if( !rch ) {
			for( rch = ch->in_room->people; rch; rch = rch->next_in_room )
			{
				if( rch == ch || IS_NPC(rch) ) continue;
				if( !IS_AWAKE( rch ) ) continue;
				if( !is_same_group(ch, rch) ) continue;
				if( rch->class == SAVANT ) break;
			}
		}
	}

	if( !rch )
	{
		send_to_char("你的復活術還不夠純熟，必須有一為賢者輔助你施展。\n\r", ch);
		return;
	}

	if( !IS_NPC(rch) )
	{
		if( rch->mana < 50 )
		{
			send_to_char("他無法幫你。\n\r", ch);
			return;
		}
		rch->mana -= 100;
		ch->mana -= 200;
	}

	if( corpse->item_type != ITEM_CORPSE_PC)
	{
		send_to_char("那不是玩家的屍體。\n\r", ch);
		return;
	}

	if( corpse->level > level + sklv  )
	{
		send_to_char("你的等級不夠。\n\r", ch);
		return;
	}

	arg1 = corpse->name;
	arg = one_argument( arg1, temp) /* "corpse" */;

	if( !(vch = get_char_world( ch, arg)) || IS_NPC(vch) || !vch->in_room)
	{
		send_to_char("沒有這個玩家。\n\r", ch);
		return;
	}

	if( IS_SET(vch->in_room->room_flags, ROOM_NO_RECALL))
	{
		send_to_char("你失敗了。\n\r", ch);
		return;
	}

	if ( is_qmark(vch, "spell_resurrection_timer") )
	{
		send_to_char("他的靈魂強度還沒有凝鍊到能再次接受復活！\n\r",ch);
		return;
	}

	act( "$n的身影隨著一陣煙霧而消失 .", vch, NULL, NULL, TO_ROOM );
	old_inroom = ch->in_room;
	char_from_room( vch );
	char_to_room(vch, old_inroom);
	act( "$n突然出現在這裡.", vch, NULL, NULL, TO_ROOM );
	act( "$n將你召喚回屍體所在之處.", ch, NULL, vch, TO_VICT );
	send_to_char("好的。\n\r", ch);
	//WAIT_MAGIC_STATE(rch, skill_table[sn].beats);
	WAIT_MAGIC_STATE(vch, 12);
	sprintf( buf, "self spell_resurrection_timer %d 0 0 0 0 1",12 - sklv );
	do_add_qmark( vch, buf );
	if ( IS_SET( vch->act, PLR_HORSE ) && vch->desc )
	{
		phorse = vch->pcdata->horse;
		char_from_room( phorse );
		char_to_room( phorse, old_inroom );
		if ( phorse->hunting || phorse->assassinating || phorse->hunted || phorse->assassinated )
			check_asshunt( phorse );
	}
}


void spell_evacuate( int sn, int level, CHAR_DATA *ch, void *vo )
{
	int cost = 450 - get_skill_level(ch,sn)*50;
	char buf[BUF_STRING_LENGTH];
	CHAR_DATA *gch, *gch_next;
	ROOM_INDEX_DATA * to_room;
	QMARK *qmark = NULL;

	if( IS_NPC(ch) ) return;

	if( !str_cmp( ch->in_room->area->identity, "immhall" ) )
	{
		send_to_char("這裡不能進行傳送.\n\r" , ch );
		return;
	}

	//to_room = get_room_index(ch->in_room->area->recall);

	if(    !(to_room = get_room_index(ch->in_room->area->recall))
		&& !(qmark = is_qmark( ch, "evacuate_loc" ))== NULL )
	{
		send_to_char("這裡不能進行傳送.\n\r" , ch );
		return;
	}

	if(    (get_skill_level(ch,sn) < 5 && IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
		|| (get_skill_level(ch,sn) < 4 && IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)) )
	{
		send_to_char("你的能力不足以在這裡進行傳送.\n\r", ch);
		return;
	}

	// 追加 evacuate 可在特殊的room flag下, 標記進行全隊傳送 19/12/07
	if( !str_cmp( target_name, "mark" )){
		if (IS_SET(ch->in_room->room_flags, ROOM_CAMP))
		{
			if(get_skill_level(ch,sn) < 3){
				send_to_char("你的能力不足以設定標記.\n\r" , ch );
				return;
			}
			if( ch->mana < cost * 3 ){
				send_to_char("你的法力不夠了.\n\r" , ch );
				return;
			}
			send_to_char("你在地面設定了傳送標記.\n\r" , ch );
			ch->mana -= cost * 3;
			if( (qmark = is_qmark(ch, "evacuate_loc")) ){
				qmark->v0 = ch->in_room->vnum;
				qmark->timer = get_skill_level(ch,sn) * 10; // 加入 timer 重置 2022/12/20
				return;
			}
			sprintf( buf, "self evacuate_loc %d %d 0 0 0 0" , get_skill_level(ch,sn)*10, ch->in_room->vnum);
			do_add_qmark( ch, buf );
			return;
		}
		send_to_char("你無法在這裡設定傳送標記.\n\r" , ch );
		return;
	}else if( !str_cmp( target_name, "cancel") ){
			if( ( qmark = is_qmark(ch, "evacuate_loc") )){
				send_to_char("你取消了傳送標記.\n\r" , ch );
				do_del_qmark( ch, "self evacuate_loc");
				return;
			}
			send_to_char("你沒有設定任何的傳送標記.\n\r" , ch );
			return;
	}else{
		if( (qmark = is_qmark( ch, "evacuate_loc" )) ){
			to_room = get_room_index(qmark->v0);
		}else{
			to_room = get_room_index(ch->in_room->area->recall);
		}
		
		if(to_room == NULL){
			send_to_char("你失敗了.\n\r" , ch );
			return;
		}

		if(get_skill_level(ch,sn) < 6 && (ch->in_room->area->recall != to_room->area->recall))
		{
			send_to_char("你的能力不足以傳送到不同的時空中.\n\r", ch);
			return;
		}
		if(get_skill_level(ch,sn) < 4 && IS_SET(to_room->room_flags, ROOM_NO_TELEPORT_TO))
		{
			send_to_char("你的能力不足以傳送到那裡.\n\r", ch);
			return;
		}
	}

	send_to_char( HIG "耀眼的魔法陣出現在地面，自魔法陣中出現的風神幻影試著你及隊友帶離這個地方！\n\r"NOR"", ch);
	act( HIG "地面忽然出現了耀眼的四角魔法陣，緊接著眼前的景色開始扭曲變形。\n\r"NOR"" , ch, NULL, NULL, TO_ROOM );


	for( gch = ch->in_room->people ; gch ; gch = gch_next ) 
	{

		if( ch->mana < cost ){
			send_to_char("\n\r你的法力不夠傳送更多人了.\n\r" , ch );
			break;
		}

		gch_next = gch->next_in_room;

		if( gch == ch ) continue;
		if( IS_NPC ( gch ) ) continue;		//Added by Razgriz	20050831
		//if( !can_see( ch, gch ) ) continue;  移除看不見的隊友無法被傳送 19/12/07

		if( is_same_group( ch, gch ) ) 
		{
			ch->mana -= cost; // 傳送每個隊友所耗費的 mana

			if( gch->in_room->progtypes & ROOM_LEAVE)
				rprog_leave_trigger( gch->in_room, gch, 8 );

			if( gch->fighting )
				stop_fighting( gch, TRUE );

			send_to_char( HIG "\n\r你腳下出現的風神幻影，將你帶離這個地方！\n\r"NOR"", gch);
			act( "$n 的身影逐漸消失.", gch, NULL, NULL, TO_ROOM );
			char_from_room( gch);
			char_to_room( gch, to_room);
			act( "$n 的身影逐漸出現.", gch, NULL, NULL, TO_ROOM );
			do_look(gch, "auto" );

			/* 
			if( (qmark = is_qmark( gch, "cast_evacuate_mark" ))== NULL )
			do_add_qmark(gch, "self cast_evacuate_mark -1 2 0 0 0 0");
			*/
			if( (qmark = is_qmark( gch, "cast_evacuate_mark" ))== NULL ){
				sprintf(buf, "self cast_evacuate_mark -1 %d 0 0 0 1", 7 - get_skill_level(ch,sn));
				do_add_qmark(gch, buf);
			}

			if( to_room->progtypes & ROOM_ENTER)
				rprog_entry_trigger(to_room, gch, 6);

			gch->move = 0;
			//gch->mana = UMAX( 0, gch->mana - 80 );

			if( gch->position > POS_DEAD )  // Added by Amethyst
				mprog_greet_trigger( gch ); // Added by Amethyst
		}
	}
  
	if( ch->in_room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger( ch->in_room, ch, 8 );
	if( ch->fighting )
		stop_fighting( ch, TRUE );

	act( "$n 的身影逐漸消失.", ch, NULL, NULL, TO_ROOM );
	char_from_room( ch);
	char_to_room( ch, to_room);
	act( "$n 的身影逐漸出現.", ch, NULL, NULL, TO_ROOM );
	do_look(ch, "auto" );

	/* 
	if( (qmark = is_qmark( ch, "cast_evacuate_mark" ))== NULL )
	do_add_qmark(ch, "self cast_evacuate_mark -1 2 0 0 0 0");
	*/
	if( (qmark = is_qmark( ch, "cast_evacuate_mark" ))== NULL ){
		sprintf(buf, "self cast_evacuate_mark -1 %d 0 0 0 1", 7 - get_skill_level(ch,sn));
		do_add_qmark(ch, buf);
	}

	if( to_room->progtypes & ROOM_ENTER)
		rprog_entry_trigger(to_room, ch, 6);

	ch->move = 0;

	if( ch->mana < cost )
		ch->mana = 0;
	else 
		ch->mana -= cost;

	if( ch->position > POS_DEAD )  // Added by Amethyst
		mprog_greet_trigger( ch ); // Added by Amethyst
	// ch->mana = 0;

	return;
}


void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    //int type=0, value=0,ac = 0, wearbit =0;
    int hr,dr;


    if ( obj->item_type != ITEM_ARMOR
  || IS_OBJ_STAT( obj, ITEM_MAGIC )
  || obj->affected )
    {
  send_to_char( "那無法再被增強了.\n\r", ch );
  return;
    }

    if ( obj->wear_loc != WEAR_NONE )
  remove_obj( ch, obj->wear_loc, TRUE );

    /* Thanks to Magoo for this little insight */
    if ( !IS_NPC(ch) && obj->level > level )
    {
  send_to_char( "你無法為更高級的護具增強.\n\r",
         ch);
  return;
    }
    hr = 1 + URANGE(0,((ch->level - 14)/7), 4);
    dr = 3 + URANGE(0,((ch->level - 15)/5),5) + URANGE(0, ((ch->level - 35) /2), 4);


    if ( IS_GOOD( ch ) )
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL);
  act( "$p 泛起一陣藍光.",   ch, obj, NULL, TO_CHAR );
        hr += 1;
        dr -= 3;
    }
    else if ( IS_EVIL( ch ) )
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
  act( "$p 泛起一陣紅光.",   ch, obj, NULL, TO_CHAR );
        hr -= 1;
        dr += 4;
    }
    else
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
  SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
  act( "$p 泛起一陣黃光.", ch, obj, NULL, TO_CHAR );
        hr += 1;
        dr += 1;
    }
    if( hr > 0 ) {
        paf     = new_affect();

        paf->type   = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = hr;
        paf->bitvector  = 0;
        paf->next   = obj->affected;
        obj->affected = paf;
    }

    if( dr > 0 ) {

        paf     = new_affect();

        paf->type   = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = dr;//3 + URANGE(0,((ch->level - 15)/5),5)
        //+ URANGE(0, ((ch->level - 35) /2), 4);
        paf->bitvector  = 0;
        paf->next   = obj->affected;
        obj->affected = paf;
    }
    if( obj->level < level -3 )
    obj->level    = UMAX(obj->level, number_fuzzy( level - 3 ));

    send_to_char( "好的。\n\r", ch );
    return;
}

void spell_enhance_armor (int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_ARMOR
  || IS_OBJ_STAT( obj, ITEM_MAGIC )
  || obj->affected )
    {
  send_to_char( "That item cannot be enhanced.\n\r", ch );
  return;
    }

    paf     = new_affect();

    paf->type   = sn;
    paf->duration = -1;
    paf->location = APPLY_AC;
    paf->bitvector  = 0;
    paf->next   = obj->affected;
    obj->affected = paf;

    if ( ( IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CHARM )  && !IS_AFFECTED(ch, AFF_SUMMONED))|| 
  ( !IS_NPC( ch ) && number_percent() <get_skill_percent(ch, sn)/2 //ch->pcdata->learned[sn]/2
  + 3 * ( level - obj->level ) ) )

    /* Good enhancement */
    {
  paf->modifier = -level / 8;

       if ( IS_GOOD( ch ) )
  {
      SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
      act( "$p glows blue.",   ch, obj, NULL, TO_CHAR );
  }
  else if ( IS_EVIL( ch ) )
  {
      SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
      act( "$p glows red.",    ch, obj, NULL, TO_CHAR );
  }
  else
  {
      SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
      SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
      act( "$p glows yellow.", ch, obj, NULL, TO_CHAR );
  }
       
  send_to_char( "好的。\n\r", ch );
    }
    else
    /* Bad Enhancement ... opps! :) */
    {
  paf->modifier = level / 8;
  obj->cost = 0;

  SET_BIT( obj->extra_flags, ITEM_NODROP );
  act( "$p turns black.", ch, obj, NULL, TO_CHAR );
    }

    return;
}

void spell_magic_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	//char *buf[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	bool isObj = FALSE;
	bool isChar = FALSE;


	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

	/*  修改加成判定 2023/01/05
		if( get_curr_int( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_int( ch ) - 21) * get_curr_int( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_int( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_int( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_int( ch ) + 24 + sklv * 2 ) )/60; // md

	//buf = one_argument( target_name , arg);
	//one_argument( buf , arg1);
	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch
		/*
			strcat( arg, "\n\r" );
			send_to_char( arg, ch );
			
			strcat( arg1, "\n\r" );
			send_to_char( arg1, ch );
			
			strcat( target_name, "" );
			send_to_char( target_name, ch );
			return;
		*/

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				//send_to_char( "bug arg1 exist but invaild arg.\n\r", ch );
				return;
			}
		}
		
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					//send_to_char( "沒抓到obj.\n\r", ch );
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			// 新增裝備 ammo 2022/04/30
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			//send_to_char( "bug no arg1 vch.\n\r", ch );
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}else{
			//send_to_char( "bug no arg , arg1.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 1 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_SPELL;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIU"你周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIU"周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIU"周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIU"周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIU"你周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIU"周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIU"周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIU"你周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIU"周邊的魔力凝聚到"NOR"$p"HIU"之中。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_wind_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}
	
	/*  修改加成判定 2023/01/05
		if( get_curr_int( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_int( ch ) - 21) * get_curr_int( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_int( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_int( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_int( ch ) + 24 + sklv * 2 ) )/60; // md

	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 2 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_WIND;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIG"你身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIG"身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIG"四周空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIG"四周空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIG"你身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIG"身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIG"身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIG"你身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIG"身旁空間形成一道氣旋，纏繞在"NOR"$p"HIG"周圍。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_flame_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

	/*  修改加成判定 2023/01/05
		if( get_curr_int( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_int( ch ) - 21) * get_curr_int( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_int( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_int( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_int( ch ) + 24 + sklv * 2 ) )/60; // md

	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 3 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_FIRE;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIR"你身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIR"身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIR"你身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIR"身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIR"身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIR"你身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIR"身上的"NOR"$p"HIR"突然迸出灼熱的火花，赤紅地燃燒起來。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_water_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

	/*  修改加成判定 2023/01/05
		if( get_curr_int( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_int( ch ) - 21) * get_curr_int( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_int( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_int( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_int( ch ) + 24 + sklv * 2 ) )/60; // md

	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 4 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_COLD;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIC"你四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIC"四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIC"四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIC"四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIC"你四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIC"四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIC"四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIC"你四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIC"四周的溫度驟降，水氣在"NOR"$p"HIC"表面凝結為點點冰霜。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_earth_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}
	
	/*  修改加成判定 2023/01/05
		if( get_curr_int( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_int( ch ) - 21) * get_curr_int( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_int( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_int( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_int( ch ) + 24 + sklv * 2 ) )/60; // md

	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 5 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_EARTH;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""YEL"你周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"YEL"周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""YEL"周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""YEL"周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""YEL"你周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"YEL"周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"YEL"周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""YEL"你周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"YEL"周圍的碎石受到吸引，將"NOR"$p"YEL"包覆得密不透風。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_lightning_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

	/*  修改加成判定 2023/01/05
		if( get_curr_int( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_int( ch ) - 21) * get_curr_int( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_int( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_int( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_int( ch ) + 24 + sklv * 2 ) )/60; // md

	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 6 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_LIGHTNING;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIY"你身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIY"身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIY"四周的空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIY"四周的空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIY"你身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIY"身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIY"身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIY"你身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIY"身旁空氣滋滋作響，剎那間"NOR"$p"HIY"爆發出強大的電氣。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_saint_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	int  AliDiff;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

	if ( !IS_NPC( ch ) && IS_EVIL( ch )) {
		send_to_char( "你的心靈無法凝聚神聖的力量.\n\r", ch );
		return; 
	}

	/*  修改加成判定 2023/01/05
		if( get_curr_wis( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_wis( ch ) - 21) * get_curr_wis( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_wis( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_wis( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_wis( ch ) + 24 + sklv * 2 ) )/60; // md

	if ( !IS_NPC( ch ) ) {  // align 影響
		if( ch->alignment >= 0 )
			AliDiff = 299 + ch->alignment;
		else
			AliDiff = 300 + ch->alignment;
		enc_mod = UMAX( ch->level/2 , enc_mod * AliDiff /1299 );
	}
	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch
		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 7 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_SAINT;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIW"你身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIW"身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIW"你身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIW"身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIW"身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIW"你身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIW"身上的"NOR"$p"HIW"受到祝福，自體散發耀眼的神聖幅光。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}

void spell_dark_enchantment( int sn, int level, CHAR_DATA *ch, void *vo )
{

    OBJ_DATA  *obj;
	CHAR_DATA *vch;
    AFFECT_DATA *paf;
	char arg[ MAX_INPUT_LENGTH ];
	char arg1[ MAX_INPUT_LENGTH ];
	int  obj_lv = level;
	int  sklv = get_skill_level(ch,sn);
	int  enc_mod;
	int  AliDiff = 0;
	bool isObj = FALSE;
	bool isChar = FALSE;

	if( level >= 1000){
		obj = (OBJ_DATA *) vo;
		isObj = TRUE;
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
	}

	if ( !IS_NPC( ch ) && IS_GOOD( ch ) ) {
		send_to_char( "你的心靈無法凝聚邪惡的力量.\n\r", ch );
		return;
	}

	/*  修改加成判定 2023/01/05
		if( get_curr_wis( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_wis( ch ) - 21) * get_curr_wis( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_wis( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_wis( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_wis( ch ) + 24 + sklv * 2 ) )/60; // md
	
	if ( !IS_NPC( ch ) ) {  // align 影響
		if( ch->alignment > 0 )
			AliDiff = 300 - ch->alignment;
		else
			AliDiff = 299 - ch->alignment;
		enc_mod = UMAX( ch->level/2 , enc_mod * AliDiff / 1299);
	}
	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // 無vch
			if( arg[0] != '\0' ){
				isObj = TRUE;
			}else{
				isChar = TRUE;
			}
		}else{
			if( arg[0] != '\0' ){
				isChar = TRUE;
				vch = get_char_room(ch , arg1);
			}else{
				return;
			}
		}
		
		if( isObj ){
			if( obj = get_obj_here( ch, arg ) ){
				// 修改為不需要完整的名稱 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "這裡沒有這個東西.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "這裡沒有這個東西.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // 無 arg 且無 arg1 ( ch )
			vch = ch;
			if( get_eq_char( ch, WEAR_WIELD ) ){
				obj = get_eq_char( ch, WEAR_WIELD );
			}
			else if( get_eq_char( ch, WEAR_TWO_HAND ) ){
				obj = get_eq_char( ch, WEAR_TWO_HAND );
			}
			else if( get_eq_char( ch, WEAR_WIELD_2 ) ){
				obj = get_eq_char( ch, WEAR_WIELD_2 );
			}
			else if( get_eq_char( ch, WEAR_HANDS ) ){
				obj = get_eq_char( ch, WEAR_HANDS );
			}
			else if( get_eq_char( ch, WEAR_HANDS_2 ) ){
				obj = get_eq_char( ch, WEAR_HANDS_2 );
			}
			else if( get_eq_char( ch, WEAR_FEET ) ){
				obj = get_eq_char( ch, WEAR_FEET );
			}
			else if( get_eq_char( ch, WEAR_FEET_2 ) ){
				obj = get_eq_char( ch, WEAR_FEET_2 );
			}
			else if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
				obj = get_eq_char( ch, WEAR_ARROW );
			}
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "你沒有穿戴可以附魔的物品.\n\r", ch);
				return;
			}
		}else{ // 有 arg 且有 arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room 中找不到 arg1 這個 vch
				send_to_char( "這裡沒有這個人.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					if(get_eq_char( vch, WEAR_WIELD ))
						obj = get_eq_char( vch, WEAR_WIELD );
					if(get_eq_char( vch, WEAR_TWO_HAND ))
						obj = get_eq_char( vch, WEAR_TWO_HAND );
				}
			}else if( !str_cmp( arg , "wield2" ) || !str_cmp( arg , "wp2" ) ){
				if( !get_eq_char( vch, WEAR_WIELD_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // 加入彈藥裝備 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "你這個部位沒有穿戴裝備.\n\r", ch );
					else
						send_to_char( "他這個部位沒有穿戴裝備.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "這件物品無法附魔.\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "這個裝備位置無法附魔.\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "這裡沒有這樣東西.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "你身上沒有裝備這件物品.\n\r", ch );
			else
				send_to_char( "他身上沒有裝備這件物品.\n\r", ch );
		}
		return;
	}
	
	if(    !CAN_WEAR(obj, ITEM_WIELD)
		&& !CAN_WEAR(obj, ITEM_TWO_HAND)
		&& !CAN_WEAR(obj, ITEM_WEAR_HANDS)
		&& !CAN_WEAR(obj, ITEM_WEAR_FEET)
		&& !(obj->item_type == ITEM_ARROW)
		&& !(obj->item_type == ITEM_DART)
		&& !(obj->item_type == ITEM_AMMO) ) // item ammo 2022/04/30
	{
		send_to_char( "這件物品無法附魔.\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "這件物品已經被附魔了.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "你無法為更高級的附魔.\n\r", ch);
		return;
	}

	// 物品屬性限制 2022/12/10
	if( !EncWpAffCheck( obj , 8 ) ){
		send_to_char( "物品具有屬性，你無法附加其他屬性。\n\r", ch);
		return;
	}

	// 彈藥有較高的附魔時間 2022/05/01
	if( obj->item_type == ITEM_AMMO ) sklv = 8 + sklv;

    SET_BIT( obj->extra_flags, ITEM_SPELL );
	paf     = new_affect();
	paf->type   = obj->pIndexData->vnum;
	paf->duration = sklv/2;
	paf->location = APPLY_WEAPON_DARK;
	paf->modifier = enc_mod;
	paf->bitvector  = 0;
	paf->next   = obj->affected;
	obj->affected = paf;

	if( isObj ){
		if( obj->carried_by ){
			act( ""HIB"你身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIB"身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIB"你身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIB"身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIB"身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIB"你身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIB"身上的"NOR"$p"HIB"被一道陰影籠罩逐漸黯淡，吸收著周圍的光芒。"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "好的.",   ch, obj, NULL, TO_CHAR );
    return;
}