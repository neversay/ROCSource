//*****************************************************************
//*  PROGRAM ID  : spell_obj.c                                    *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940710 jye                                    *
//*  PURPOSE     :         ­×§ïspell_destroy_cursed ppl¤£¯à¨Ï¥Î   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940831 Razgriz                                *
//*  PURPOSE     :         ­×§ï spell_evacuate ¤£¯à¶Ç°e mob       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940921 jye                                    *
//*  PURPOSE     :         ­×§ïspell_identify ¥i¬Ý¨ìZ4            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940924 jye                                    *
//*  PURPOSE     :         ­×§ïspell_evacuate ¨Ï¥Î«á¥[¤WQMARK     *
//*              :         ¤@¬q®É¶¡µLªk²¾°Ê,¬Iªk¤Îhp mp mv ³£     *
//*              :         ±N¤£·|¦^´_                             *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0991217 tale                                    *
//*  PURPOSE     :         ­×§ï enc_weapon ´î­««áª±®a­t­«¤Î     *
//*              :         °{Á×µLªk¥¿±`¤ÏÀ³    *
//*****************************************************************
/* $Id: spell_obj.c 1956 2005-07-01 14:07:51Z akira $ */
#include "skill.h"
extern int HpMultiply_Affect(AFFECT_DATA *paf);
extern int HpDeMultiply_Affect(AFFECT_DATA *paf);
bool EncWpAffCheck( OBJ_DATA *obj , int type );

// §P©wª««~¬O§_¥»¨­¦³ÄÝ©Ê, ¦pªG¦³«h¥u¯à¥[¤W¬Û¦PÄÝ©Ê 2022/12/10
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
		if( type == 1 && a > 0 ) // ªk³N
			return TRUE;
		if( type == 2 && b > 0 ) // ­·
			return TRUE;
		if( type == 3 && c > 0 ) // ¤õ
			return TRUE;
		if( type == 4 && d > 0 ) // ¤ô
			return TRUE;
		if( type == 5 && e > 0 ) // ¦a
			return TRUE;
		if( type == 6 && f > 0 ) // ¹p
			return TRUE;
		// ¥úîî°ß¦³¦s¦b«ú§ÜÄÝ©Êªº±¡ªp¤UµLªkªþÅ]
		if( type == 7 ){ // ¥ú
			if( g > 0 ) // Àu¥ý§P©w¬O§_¦³¥úÄÝ©Ê
				return TRUE;
			if( h > 0 ) // ª««~¦³îîÄÝ©Ê«h¥¢±Ñ
				return FALSE;
			return TRUE;
		}
		if( type == 8 ){ // îî
			if( h > 0 )
				return TRUE;
			if( g > 0 ) // ª««~¦³¥úÄÝ©Ê«h¥¢±Ñ
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

	// ¹ï©Ð¶¡¤ºÅ]ªk¥ú²y¼Æ¶q§@­­¨î 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_LIGHT_BALL ) )
			count += 1;
	}

	if( count >= 100 ){
		send_to_char( "§A¥¢±Ñ¤F.\n\r",ch);
		return;
	}

	// sklv ³Ì§C¼Æ¶qªº¥ú²y 2022/06/12
	sklv = sklv + 1;

	for(; sklv > 0; --sklv ) {
		// ©Ð¶¡¤º¼Æ¶q¤Ó¦h«h¤£¦A©ñ¥ú²y 2022/06/12
		if( count >= 100 ) break;
		light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
		light->value[2] += (1 + sklv) * 4;
		obj_to_room( light, ch->in_room );

		act( "§A¦ù¤â¤@«ü¡AÅÜ¥X¤F$p.", ch, light, NULL, TO_CHAR );
		act( "$n ¦ù¤â¤@«ü¡AÅÜ¥X¤F$p.", ch, light, NULL, TO_ROOM );
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

	// ¹ï©Ð¶¡¤ºÅ]ªk­¹ª«¼Æ¶q§@­­¨î 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_MUSHROOM ) )
			count += 1;
	}

	if( count >= 50 ){
		send_to_char( "§A¥¢±Ñ¤F.\n\r",ch);
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
			mushroom->short_descr = str_dup("Å]ªk¬õÅÚ½³(magic carrot)");
			mushroom->description = str_dup("Å]ªk¬õÅÚ½³(magic carrot)");
			mushroom->value[0] = 5 + ch->level/2 ;
			break;
		case 2:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("manto magic");
			mushroom->short_descr = str_dup("Å]ªkÄCÀY(magic manto)");
			mushroom->description = str_dup("Å]ªkÄCÀY(magic manto)");
			mushroom->value[0] = 5 + ch->level ;
			break;
		case 3:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("wing chicken-wing magic");
			mushroom->short_descr = str_dup("Å]ªk¯NÂû¯Í(magic chicken wing)");
			mushroom->description = str_dup("Å]ªk¯NÂû¯Í(magic chicken wing)");
			mushroom->value[0] = 5 + ch->level + ch->level / 2;
			break;
		case 4:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("steak magic");
			mushroom->short_descr = str_dup("Å]ªk¯N¤û±Æ(magic steak)");
			mushroom->description = str_dup("Å]ªk¯N¤û±Æ(magic steak)");
			mushroom->value[0] = 5 + ch->level + ch->level;
			break;
		case 5:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("chicken full-chicken magic");
			mushroom->short_descr = str_dup("Å]ªk¤â¥¶Âû(magic full chicken)");
			mushroom->description = str_dup("Å]ªk¤â¥¶Âû(magic full chicken)");
			mushroom->value[0] = 5 + ch->level + ch->level/2 + ch->level;
			break;
		case 6:
			free_string( mushroom->name );
			free_string(mushroom->short_descr);
			free_string(mushroom->description);
			mushroom->name = str_dup("pig roast-pig magic");
			mushroom->short_descr = str_dup("Å]ªk¯N¨Å½Þ(magic roast pig)");
			mushroom->description = str_dup("Å]ªk¯N¨Å½Þ(magic roast pig)");
			mushroom->value[0] = 5 + ch->level + ch->level + ch->level;
			break;
		default:
			mushroom->value[0] = 5 + ch->level / 4;
			break;
	}
	mushroom->value[2] += get_skill_level(ch,sn) * 12;
	mushroom->timer = 2 + get_skill_level(ch,sn)/3;
	obj_to_room( mushroom, ch->in_room );

	act( "$p ¬ðµM¥X²{¦b§A­±«e.", ch, mushroom, NULL, TO_CHAR );
	act( "$p ¬ðµM¥X²{¦b§A­±«e.", ch, mushroom, NULL, TO_ROOM );
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

	// ¹ï©Ð¶¡¤ºÅ]ªk¬u¤ô¼Æ¶q§@­­¨î 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_SPRING ) )
			count += 1;
	}

	if( count >= 5 ){
		send_to_char( "§A¥¢±Ñ¤F.\n\r",ch);
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
			sprintf(buf, "Å]ªk%s¬u(%s spring)", 
							liq_table[spring->value[2]].liq_name,
							liq_table[spring->value[2]].liq_ename);
			spring->short_descr = str_dup(buf);
			spring->description = str_dup(buf);
			SET_BIT(spring->wear_flags, ITEM_TAKE);
		}

		obj_to_room( spring, ch->in_room );
		act( "¦a­±¬ðµMµõ¶}¡A¬y¥X¤F¤@¹D$p.", ch, spring, NULL, TO_CHAR );
		act( "¦a­±¬ðµMµõ¶}¡A¬y¥X¤F¤@¹D$p.", ch, spring, NULL, TO_ROOM );
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
					sprintf(buf, "Å]ªk%s¬u(%s spring)", 
									liq_table[spring->value[2]].liq_name,
									liq_table[spring->value[2]].liq_ename);
					spring->short_descr = str_dup(buf);
					spring->description = str_dup(buf);
					SET_BIT(spring->wear_flags, ITEM_TAKE);
				}
				obj_to_room( spring, ch->in_room );

				act( "¦a­±¬ðµMµõ¶}¡A¬y¥X¤F¤@¹D$p.", ch, spring, NULL, TO_CHAR );
				act( "¦a­±¬ðµMµõ¶}¡A¬y¥X¤F¤@¹D$p.", ch, spring, NULL, TO_ROOM );
				return;
			}
		send_to_char( "¨S¦³³oºØ¬u³á¡I\n\r",ch);
		return;
	}

}

void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj   = (OBJ_DATA *) vo;
    int       water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
  send_to_char( "¨º¼ËªF¦è¤£¯à¸Ë¤ô.\n\r", ch );
  return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
  send_to_char( "¨º¸Ì­±¤w¸g¦³§OºØ²GÅé¤F.\n\r", ch );
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
  act( "$p ²{¦b¸Ëº¡¤F.", ch, obj, NULL, TO_CHAR );
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
      send_to_char( "¤@ªÑ¯«¸t¤O³õªýµ´¤F§Aªº¥ø¹Ï¡I\n\r", ch );
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
           act( "§A¥þ¤O§â $p ©¹¦a¤W¤@ºL¡C", victim, obj, NULL, TO_CHAR );
           act( "$n ¥þ¤O§â $p ©¹¦a¤W¤@ºL¡C", victim, obj, NULL, TO_ROOM );
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
      send_to_char( "¦nªº¡C\n\r", ch );
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
  send_to_char( "¨º¤£¬OÅ@¨ã.\n\r", ch );
  return;
    }

    if ( obj->wear_loc != WEAR_NONE ) {
  send_to_char("§A¥²¶·¥ý²æ¤U¥¦.\n\r", ch );
  return;
    }

    if ( !IS_NPC( ch ) && obj->level - 2 > ch->level + sklv)
    {
  send_to_char( "§AµLªk­×Å@³o»ò°ª¯ÅªºÅ@¨ã.\n\r",
         ch);
  return;
    }

    if( obj->value[1] < 1 ) {
  send_to_char("¨º¥óÅ@¨ã¨Ã¨S¦³·lÃa.\n\r", ch);
  return;
    }


    act( "¤@°}±j¥úÅ¢¸nµÛ$p¡AÀHµÛ¥ú¨~´²¥h¡A$pªº·l¶ËºCºC­×´_¤F¡C",   ch, obj, NULL, TO_CHAR );

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
    send_to_char( "¦nªº¡C\n\r", ch );
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
  send_to_char( "¨ºµLªk¦A³Q¼W±j¤F.\n\r", ch );
  return;
    }

    if ( obj->wear_loc != WEAR_NONE )
  remove_obj( ch, obj->wear_loc, TRUE );

    /* Thanks to Magoo for this little insight */
    if ( !IS_NPC(ch) && obj->level > ch->level + sklv )
    {
  send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªZ¾¹¼W±j.\n\r",
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
  act( "$p ªx°_¤@°}ÂÅ¥ú.",   ch, obj, NULL, TO_CHAR );
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
  act( "$p ªx°_¤@°}¬õ¥ú.",   ch, obj, NULL, TO_CHAR );
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
  act( "$p ªx°_¤@°}¶À¥ú.", ch, obj, NULL, TO_CHAR );
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
                act( "$p ªx°_¤@°}ª÷¥ú.", ch, obj, NULL, TO_CHAR );
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

    send_to_char( "¦nªº¡C\n\r", ch );
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
					"ª««~¦WºÙ: %s   ª««~§Î¦¡: %s(%s)\n\rª««~ª¬ºA: %s.\n\r",
					obj->short_descr,
					c_item_type_name( obj ),
					item_type_name(obj),
					c_extra_bit_name( obj->extra_flags )
			);
	send_to_char( buf, ch );

	if( obj->pIndexData->vnum == OBJ_VNUM_POWER_GEM ){
		sprintf(buf,"Å]¤O¦^´_¶q: %d - %d ÂI.\n\r", obj->value[0],obj->value[1] );
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_CONTAINER ) {
		if( obj->value[3] > 0 ) {
			sprintf(buf,"®e¶q: %d, ´î­«¤ñ²v: %d%%.\n\r", obj->value[0], 100 - obj->value[3] );
		}
		else if( obj->value[3] < 0 )
			// ­×§ï¼Æ¶q´î¤ÖÅã¥Ü¬°¥¿­È 2022/12/01
			//sprintf(buf,"®e¶q: %d, ´î¤Öª««~¼Æ: %d¥ó.\n\r",obj->value[0], obj->value[3]);
			sprintf(buf,"®e¶q: %d, ´î¤Öª««~¼Æ: %d¥ó.\n\r",obj->value[0], 0 - obj->value[3] );
		else
			sprintf(buf,"®e¶q: %d\n\r", obj->value[0]);
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_DRINK_CON ) {
		if( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ) {
			bug( "Spell_identify: bad liquid number %d.", obj );
			obj->value[2] = 0;
		}
		/* ±N¨C¤@¶µ¤À¶}, ¥H§@¤£¦P§P©wÅã¥Ü 2023/01/05

		sprintf(buf,"®e¶q: %d, ²±¸Ë¶q: %d, ºØÃþ: %s.\n\r", obj->value[0], obj->value[1], liq_table[obj->value[2]].liq_name);
		send_to_char(buf, ch); */

		if( obj->value[0] > 0 )
			sprintf( buf, "®e¶q: %d, ", obj->value[0] );
		else
			sprintf( buf, "®e¶q: 0, ");
		send_to_char(buf, ch);

		if( obj->value[1] > 0 ){
			sprintf( buf, "²±¸Ë¶q: %d, ", obj->value[1] );
		}else{
			if( obj->value[1] == -1 )  // ·s¼W -1 ªº±¡ªp 2023/01/05
				sprintf( buf, "²±¸Ë¶q: ¡Û, ");
			else
				sprintf( buf, "²±¸Ë¶q: 0, ");
		}
		send_to_char(buf, ch);

		sprintf( buf,"ºØÃþ: %s.\n\r", liq_table[obj->value[2]].liq_name );
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_FOUNTAIN ) {
		if( obj->value[2] >= LIQ_MAX || obj->value[2] < 0 ) {
			bug( "Spell_identify: bad liquid number %d.", obj );
			obj->value[2] = 0;
		}
		/* ±N¨C¤@¶µ¤À¶}, ¥H§@¤£¦P§P©wÅã¥Ü 2023/01/05

		sprintf(buf,"¬u¤ô³W¼Ò: %d, ´Ý¯d¶q: %d, ºØÃþ: %s.\n\r", obj->value[0], obj->value[1], liq_table[obj->value[2]].liq_name);
		send_to_char(buf, ch); */

		if( obj->value[0] > 0 )
			sprintf( buf, "¬u¤ô³W¼Ò: %d, ", obj->value[0] );
		else
			sprintf( buf, "¬u¤ô³W¼Ò: 0, ");
		send_to_char(buf, ch);

		if( obj->value[1] > 0 ){
			sprintf( buf, "´Ý¦s¶q: %d, ", obj->value[1] );
		}else{
			if( obj->value[1] == -1 )  // ·s¼W -1 ªº±¡ªp 2023/01/05
				sprintf( buf, "´Ý¦s¶q: ¡Û, ");
			else
				sprintf( buf, "´Ý¦s¶q: 0, ");
		}
		send_to_char(buf, ch);

		sprintf( buf,"ºØÃþ: %s.\n\r", liq_table[obj->value[2]].liq_name );
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_FOOD ) {
		if( obj->value[1] > 0 && obj->value[1] <= 49 ) {
			sprintf(buf,"¹¡¨¬«×: %d, ­¹ª«¥[¦¨: %s %d ÂI.\n\r", obj->value[0], c_affect_loc_name( obj->value[1] ), obj->value[2]);
		}
		else
			sprintf(buf,"¹¡¨¬«×: %d.\n\r", obj->value[0]);
		send_to_char(buf, ch);
	}

	if( obj->item_type == ITEM_ARMOR && obj->wear_flags ) {
		send_to_char("¸Ë³Æ¦ì¸m¡G", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_ABOUT))
			send_to_char(" ³òÂ¶¨­Åé", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_ARMS))
			send_to_char(" ¤âÁu", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_BODY))
			send_to_char(" ¨­Åé", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_ARROW))
			send_to_char(" ­I±¾", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_FEET))
			send_to_char(" ¸}³¡", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_HANDS))
			send_to_char(" ¤â³¡", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_FINGER))
			send_to_char(" ¤â«ü Ä²¤â", ch);
		if( CAN_WEAR(obj, ITEM_WEAR_HEAD))
			send_to_char(" ÀY³¡", ch);
		if( CAN_WEAR(obj, ITEM_HOLD))
			send_to_char(" ¤â´¤", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_LEGS))
			send_to_char(" »L³¡", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_NECK))
			send_to_char(" ÀV³¡", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_SHIELD))
			send_to_char(" ¬ÞµP", ch);
		if( CAN_WEAR( obj, ITEM_TWO_HAND))
			send_to_char(" Âù¤âªZ¾¹", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_WAIST))
			send_to_char(" ¸y³¡", ch);
		if( CAN_WEAR( obj, ITEM_WIELD))
			send_to_char(" ªZ¾¹", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_WRIST))
			send_to_char(" µÃ³¡", ch);
		if( CAN_WEAR( obj, ITEM_WEAR_AMMO)) // ¼uÃÄ³¡¦ì 2022/04/30
			send_to_char(" ¼uÃÄ", ch);
		send_to_char("\n\r", ch);
	}
	if( obj->pIndexData->max_level ) {
		sprintf( buf, "§÷½è: %s  ­«¶q: %d  »ù­È: %d  µ¥¯Å: %d - %d\n\r",
			obj->material->short_descr,
			obj->weight,
			obj->cost,
			obj->level,
			obj->pIndexData->max_level);
	}else{
		sprintf( buf, "§÷½è: %s  ­«¶q: %d  »ù­È: %d  µ¥¯Å: %d\n\r",
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
			sprintf( buf, "ªþ¦³µ¥¯Å %d ªºªk³N:", obj->value[0] );
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
			sprintf( buf, "©|¦³ %d/%d ¦¸®Ä¥Î¡Aµ¥¯Å¡G %d", obj->value[2], obj->value[1], obj->value[0] );
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
			// ¦pªGªZ¾¹¬°ºj±ñ, «h¼W¥[®gÀ»ºë·Ç«×ªº±Ô­z 2022/05/01
			if(obj->value[3] == 16)
			{
				sprintf( buf, "ªZ¾¹¶Ë®`¤O: %d - %d (¥­§¡ %d).\n\r®gÀ»ºë·Ç«×: %d - %d (¥­§¡ %d).\n\r",
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
				sprintf( buf, "ªZ¾¹¶Ë®`¤O: %d - %d (¥­§¡ %d).\n\r",
								obj->value[1],
								obj->value[2],
								(obj->value[1] + obj->value[2] ) / 2 );
				send_to_char( buf, ch );
			}
			if( !indestructible(obj) )
			{
				sprintf( buf, "­@¥Î«×¡G%d/%d.\n\r", get_curr_dura(obj), get_max_dura(obj));
				send_to_char( buf, ch );
			}
			break;

		case ITEM_ARROW:
			// ±N¼Æ¶q»P¶Ë®`¤À¶} 2023/01/05
			//sprintf( buf, "½b¤ä¶Ë®`¤O: %d - %d (¥­§¡ %d+%d).\n\r½b¥Ú¼Æ: %d ¤ä.\n\r",
			sprintf( buf, "½b¤ä¶Ë®`¤O: %d - %d (¥­§¡ %d+%d).",
							obj->value[1], obj->value[2],
							( obj->value[1] + obj->value[2] ) / 2 ,
							obj->value[3] );
							//obj->value[0] );
			send_to_char( buf, ch );
			if( obj->value[0] > 0 )
				sprintf( buf, "\n\r½b¥Ú¼Æ: %d ¤ä.\n\r", obj->value[0] );
			else{
				if( obj->value[0] == -1 )  // ·s¼W -1 ªº±¡ªp 2023/01/05
					sprintf( buf, "\n\r½b¥Ú¼Æ: ¡Û ¤ä.\n\r");
				else
					sprintf( buf, "\n\r½b¥Ú¼Æ: 0 ¤ä.\n\r");
			}
			send_to_char( buf, ch );
			break;
  
		case ITEM_DART:
			// ±N¼Æ¶q»P¶Ë®`¤À¶} 2023/01/05
			//sprintf( buf, "·t¾¹¶Ë®`¤O: %d - %d (¥­§¡ %d+%d).\n\r·t¾¹¼Æ: %d §â.\n\r",
			sprintf( buf, "·t¾¹¶Ë®`¤O: %d - %d (¥­§¡ %d+%d).",
							obj->value[1], obj->value[2],
							( obj->value[1] + obj->value[2] ) / 2 ,
							obj->value[3] );
							//obj->value[0] );
			if( obj->value[0] > 0 )
				sprintf( buf, "\n\r·t¾¹¼Æ: %d ¥ó.\n\r", obj->value[0] );
			else{
				if( obj->value[0] == -1 )  // ·s¼W -1 ªº±¡ªp 2023/01/05
					sprintf( buf, "\n\r·t¾¹¼Æ: ¡Û ¥ó.\n\r");
				else
					sprintf( buf, "\n\r·t¾¹¼Æ: 0 ¥ó.\n\r");
			}
			send_to_char( buf, ch );
			break;

		case ITEM_AMMO:
			// ·s¼W¼uÃÄ type 2022/05/01
			sprintf( buf,"¼uÃÄ±þ¶Ë¤O: %d - %d (¥­§¡ %d).", (obj->value[1]*19)/20, (obj->value[1]*20)/19, obj->value[1] );
			send_to_char(buf, ch);
			if( obj->value[3] > 0 && obj->value[3] < MAX_SKILL )
			{
				sprintf( buf, " ªþ¦³µ¥¯Å %d ªºªk³N:", obj->value[2] );
				send_to_char( buf, ch );
				send_to_char( " '", ch );
				send_to_char( skill_table[obj->value[3]].cname, ch );
				send_to_char("(", ch);
				send_to_char( skill_table[obj->value[3]].name, ch );
				send_to_char( ")'", ch );
			}
			if( obj->value[0] > 0 )
				sprintf( buf, "\n\r¼uÃÄ¼Æ: %d µo.\n\r", obj->value[0] );
			else{
				if( obj->value[0] == -1 )  // ·s¼W -1 ªº±¡ªp 2023/01/05
					sprintf( buf, "\n\r¼uÃÄ¼Æ: ¡Û µo.\n\r");
				else
					sprintf( buf, "\n\r¼uÃÄ¼Æ: 0 µo.\n\r");
			}
			send_to_char( buf, ch );
			break;

		case ITEM_ARMOR:
			sprintf( buf, "Å@¥Ò¨¾½Ã¤O(AC): %d.\n\r", obj->value[0] );
			send_to_char( buf, ch );
			if( get_curr_dura(obj) != DURA_INDESTRUCTIBLE )
			{
				sprintf( buf, "­@¥Î«×: %d/%d.\n\r", get_curr_dura(obj), get_max_dura(obj));
				send_to_char( buf, ch );
			}
			break;
	}

	if( IS_SET(obj->pIndexData->souvenir, Z1) )
	{
		send_to_char( "³Æµù: ¦¹ª««~¬°¬ö©À«~\n\r", ch );
	}

	if( IS_SET(obj->pIndexData->souvenir, Z2) )
	{
		send_to_char( "Mob ±M¥Îª««~\n\r", ch );
	}
  
	if( IS_SET(obj->pIndexData->souvenir, Z4) )
	{
		send_to_char( "³Æµù: ¦¹ª««~¬°¯S®í¬ÃÂÃ«~\n\r", ch );
	}

	for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		if ( paf->location != APPLY_NONE && paf->modifier != 0 )
		{
			HpMultiply_Affect(paf);
			// ¥[¤J¦³®É®Äªºaffect ¯à¥H id Åã¥Ü®É¶¡ 2021/09/29
			if ( paf->duration > 0 )
			{
				sprintf( buf, "¼vÅT %s %d ÂI. ©|¦³ %d ¤p®Éªº®Ä¥Î.\n\r", c_affect_loc_name( paf->location ), paf->modifier , paf->duration);
			}else{
				sprintf( buf, "¼vÅT %s %d ÂI.\n\r", c_affect_loc_name( paf->location ), paf->modifier );
			}
			// sprintf( buf, "¼vÅT %s %d ÂI.\n\r",
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
			// ¥[¤J¦³®É®Äªºaffect ¯à¥H id Åã¥Ü®É¶¡ 2021/09/29
			if ( paf->duration >= 0 )
			{
				sprintf( buf, "¼vÅT %s %d ÂI. ®ÄªG©|¦³ %d ¤p®Éªº®Ä¥Î.\n\r", c_affect_loc_name( paf->location ), paf->modifier , paf->duration);
			}else{
				sprintf( buf, "¼vÅT %s %d ÂI.\n\r", c_affect_loc_name( paf->location ), paf->modifier );
			}
			//sprintf( buf, "¼vÅT %s %d ÂI.\n\r",
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
				// ¥[¤J¦³®É®Äªºaffect ¯à¥H id Åã¥Ü®É¶¡ 2021/09/29
				if ( paf->duration >= 0 )
				{
					sprintf( buf, "§÷½è¼vÅT %s %d ÂI. ©|¦³ %d ¤p®Éªº®Ä¥Î.\n\r", c_affect_loc_name( paf->location ), paf->modifier , paf->duration);
				}else{
					sprintf( buf, "§÷½è¼vÅT %s %d ÂI.\n\r", c_affect_loc_name( paf->location ), paf->modifier );
				}
				//sprintf( buf, "§÷½è¼vÅT %s %d ÂI.\n\r",
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
			send_to_char( "±M¥ÎÂ¾·~¡G", ch );
			if ( IS_SET( obj->class, NUM_BIT( MAGE ) ) )
				send_to_char( "Å]ªk®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "ªª®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( THIEF ) ) )
				send_to_char( "§®¸é ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "¾Ô¤h ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "³¥ÆZ¤H ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "§Å®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "Áåª÷®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "½åªÌ ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "¥D±Ð ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "¨ë«È ", ch );
			if ( IS_SET( obj->class, NUM_BIT( RANGER ) ) )
				send_to_char( "´å«L ", ch );
			if ( IS_SET( obj->class, NUM_BIT( NINJA ) ) )
				send_to_char( "§ÔªÌ ", ch );
			if ( IS_SET( obj->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "ÃM¤h ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "¼C«È ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MONK ) ) )
				send_to_char( "¹¬«Q ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "§ÅÂå ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "¤Ûªk®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( LORD ) ) )
				send_to_char( "»â¥D ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "¾Ô¤ý ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "¨g¾Ô¤h ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->class, DENY_BIT ) )
		{
			send_to_char( "¸T¥ÎÂ¾·~¡G", ch );
			if ( IS_SET( obj->class, NUM_BIT( MAGE ) ) )
				send_to_char( "Å]ªk®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "ªª®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( THIEF ) ) )
				send_to_char( "§®¸é ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "¾Ô¤h ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "³¥ÆZ¤H ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "§Å®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "Áåª÷®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "½åªÌ ", ch );
			if ( IS_SET( obj->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "¥D±Ð ", ch );
			if ( IS_SET( obj->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "¨ë«È ", ch );
			if ( IS_SET( obj->class, NUM_BIT( RANGER ) ) )
				send_to_char( "´å«L ", ch );
			if ( IS_SET( obj->class, NUM_BIT( NINJA ) ) )
				send_to_char( "§ÔªÌ ", ch );
			if ( IS_SET( obj->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "ÃM¤h ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "¼C«È ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MONK ) ) )
				send_to_char( "¹¬«Q ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "§ÅÂå ", ch );
			if ( IS_SET( obj->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "¤Ûªk®v ", ch );
			if ( IS_SET( obj->class, NUM_BIT( LORD ) ) )
				send_to_char( "»â¥D ", ch );
			if ( IS_SET( obj->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "¾Ô¤ý ", ch );
			if ( IS_SET( obj->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "¨g¾Ô¤h ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->material->class ) {
		if ( IS_SET( obj->material->class, ALLOW_BIT ) )
		{
			send_to_char( "§÷½è¼vÅT±M¥ÎÂ¾·~¡G", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MAGE ) ) )
				send_to_char( "Å]ªk®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "ªª®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( THIEF ) ) )
				send_to_char( "§®¸é ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "¾Ô¤h ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "³¥ÆZ¤H ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "§Å®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "Áåª÷®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "½åªÌ ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "¥D±Ð ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "¨ë«È ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( RANGER ) ) )
				send_to_char( "´å«L ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( NINJA ) ) )
				send_to_char( "§ÔªÌ ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "ÃM¤h ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "¼C«È ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MONK ) ) )
				send_to_char( "¹¬«Q ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "§ÅÂå ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "¤Ûªk®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( LORD ) ) )
				send_to_char( "»â¥D ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "¾Ô¤ý ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "¨g¾Ô¤h ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->material->class, DENY_BIT ) )
		{
			send_to_char( "§÷½è¼vÅTÂ¾·~­­¨î¡G", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MAGE ) ) )
				send_to_char( "Å]ªk®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( CLERIC ) ) )
				send_to_char( "ªª®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( THIEF ) ) )
				send_to_char( "§®¸é ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "¾Ô¤h ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "³¥ÆZ¤H ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WIZARD ) ) )
				send_to_char( "§Å®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "Áåª÷®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SAVANT ) ) )
				send_to_char( "½åªÌ ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( BISHOP ) ) )
				send_to_char( "¥D±Ð ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "¨ë«È ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( RANGER ) ) )
				send_to_char( "´å«L ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( NINJA ) ) )
				send_to_char( "§ÔªÌ ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "ÃM¤h ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "¼C«È ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MONK ) ) )
				send_to_char( "¹¬«Q ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "§ÅÂå ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( SORCERER ) ) )
				send_to_char( "¤Ûªk®v ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( LORD ) ) )
				send_to_char( "»â¥D ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( WARLORD ) ) )
				send_to_char( "¾Ô¤ý ", ch );
			if ( IS_SET( obj->material->class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "¨g¾Ô¤h ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if ( obj->race )
	{
		if ( IS_SET( obj->race, ALLOW_BIT ) )
		{
			send_to_char( "±M¥ÎºØ±Ú¡G", ch );
			if ( IS_SET( obj->race, HUMAN ) )
				send_to_char( "¤HÃþ ", ch );
			if ( IS_SET( obj->race, ELF ) )
				send_to_char( "ºëÆF ", ch );
			if ( IS_SET( obj->race, DWARF ) )
				send_to_char( "¸G¤H ", ch );
			if ( IS_SET( obj->race, ORC ) )
				send_to_char( "Ã~¤H ", ch );
			if ( IS_SET( obj->race, FAIRY ) )
				send_to_char( "§¯ºë ", ch );
			if ( IS_SET( obj->race, GNOME ) )
				send_to_char( "¦aºë ", ch );
			if ( IS_SET( obj->race, HOBBIT ) )
				send_to_char( "¶P¤ñ¯S ", ch );
			if ( IS_SET( obj->race, DROW ) )
				send_to_char( "·t¶ÂºëÆF ", ch );
			if ( IS_SET( obj->race, VAMPIRE ) )
				send_to_char( "§l¦å°­ ", ch );
			if ( IS_SET( obj->race, BEHOLDER ) )
				send_to_char( "²´Å] ", ch );
			if ( IS_SET( obj->race, LIZARDMAN ) )
				send_to_char( "»h»i¤H ", ch );
			if ( IS_SET( obj->race, WEREWOLF ) )
				send_to_char( "¯T¤H ", ch );
			if ( IS_SET( obj->race, MUMMY ) )
				send_to_char( "¤ì¤D¥ì ", ch );
			if ( IS_SET( obj->race, GARGOYLE ) )
				send_to_char( "¥Û¹³°­ ", ch );
			if ( IS_SET( obj->race, CENTAUR ) )
				send_to_char( "¤H°¨ ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->race, DENY_BIT ) )
		{
			send_to_char( "ºØ±Ú­­¨î¡G", ch );
			if ( IS_SET( obj->race, HUMAN ) )
				send_to_char( "¤HÃþ ", ch );
			if ( IS_SET( obj->race, ELF ) )
				send_to_char( "ºëÆF ", ch );
			if ( IS_SET( obj->race, DWARF ) )
				send_to_char( "¸G¤H ", ch );
			if ( IS_SET( obj->race, ORC ) )
				send_to_char( "Ã~¤H ", ch );
			if ( IS_SET( obj->race, FAIRY ) )
				send_to_char( "§¯ºë ", ch );
			if ( IS_SET( obj->race, GNOME ) )
				send_to_char( "¦aºë ", ch );
			if ( IS_SET( obj->race, HOBBIT ) )
				send_to_char( "¶P¤ñ¯S ", ch );
			if ( IS_SET( obj->race, DROW ) )
				send_to_char( "·t¶ÂºëÆF ", ch );
			if ( IS_SET( obj->race, VAMPIRE ) )
				send_to_char( "§l¦å°­ ", ch );
			if ( IS_SET( obj->race, BEHOLDER ) )
				send_to_char( "²´Å] ", ch );
			if ( IS_SET( obj->race, LIZARDMAN ) )
				send_to_char( "»h»i¤H ", ch );
			if ( IS_SET( obj->race, WEREWOLF ) )
				send_to_char( "¯T¤H ", ch );
			if ( IS_SET( obj->race, MUMMY ) )
				send_to_char( "¤ì¤D¥ì ", ch );
			if ( IS_SET( obj->race, GARGOYLE ) )
				send_to_char( "¥Û¹³°­ ", ch );
			if ( IS_SET( obj->race, CENTAUR ) )
				send_to_char( "¤H°¨ ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if ( obj->material->race )
	{
		if ( IS_SET( obj->material->race, ALLOW_BIT ) )
		{
			send_to_char( "§÷½è¼vÅT±M¥ÎºØ±Ú¡G", ch );
			if ( IS_SET( obj->material->race, HUMAN ) )
				send_to_char( "¤HÃþ ", ch );
			if ( IS_SET( obj->material->race, ELF ) )
				send_to_char( "ºëÆF ", ch );
			if ( IS_SET( obj->material->race, DWARF ) )
				send_to_char( "¸G¤H ", ch );
			if ( IS_SET( obj->material->race, ORC ) )
				send_to_char( "Ã~¤H ", ch );
			if ( IS_SET( obj->material->race, FAIRY ) )
				send_to_char( "§¯ºë ", ch );
			if ( IS_SET( obj->material->race, GNOME ) )
				send_to_char( "¦aºë ", ch );
			if ( IS_SET( obj->material->race, HOBBIT ) )
				send_to_char( "¶P¤ñ¯S ", ch );
			if ( IS_SET( obj->material->race, DROW ) )
				send_to_char( "·t¶ÂºëÆF ", ch );
			if ( IS_SET( obj->material->race, VAMPIRE ) )
				send_to_char( "§l¦å°­ ", ch );
			if ( IS_SET( obj->material->race, BEHOLDER ) )
				send_to_char( "²´Å] ", ch );
			if ( IS_SET( obj->material->race, LIZARDMAN ) )
				send_to_char( "»h»i¤H ", ch );
			if ( IS_SET( obj->material->race, WEREWOLF ) )
				send_to_char( "¯T¤H ", ch );
			if ( IS_SET( obj->material->race, MUMMY ) )
				send_to_char( "¤ì¤D¥ì ", ch );
			if ( IS_SET( obj->material->race, GARGOYLE ) )
				send_to_char( "¥Û¹³°­ ", ch );
			if ( IS_SET( obj->material->race, CENTAUR ) )
				send_to_char( "¤H°¨ ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->material->race, DENY_BIT ) )
		{
			send_to_char( "§÷½è¼vÅTºØ±Ú­­¨î¡G", ch );
			if ( IS_SET( obj->material->race, HUMAN ) )
				send_to_char( "¤HÃþ ", ch );
			if ( IS_SET( obj->material->race, ELF ) )
				send_to_char( "ºëÆF ", ch );
			if ( IS_SET( obj->material->race, DWARF ) )
				send_to_char( "¸G¤H ", ch );
			if ( IS_SET( obj->material->race, ORC ) )
				send_to_char( "Ã~¤H ", ch );
			if ( IS_SET( obj->material->race, FAIRY ) )
				send_to_char( "§¯ºë ", ch );
			if ( IS_SET( obj->material->race, GNOME ) )
				send_to_char( "¦aºë ", ch );
			if ( IS_SET( obj->material->race, HOBBIT ) )
				send_to_char( "¶P¤ñ¯S ", ch );
			if ( IS_SET( obj->material->race, DROW ) )
				send_to_char( "·t¶ÂºëÆF ", ch );
			if ( IS_SET( obj->material->race, VAMPIRE ) )
				send_to_char( "§l¦å°­ ", ch );
			if ( IS_SET( obj->material->race, BEHOLDER ) )
				send_to_char( "²´Å] ", ch );
			if ( IS_SET( obj->material->race, LIZARDMAN ) )
				send_to_char( "»h»i¤H ", ch );
			if ( IS_SET( obj->material->race, WEREWOLF ) )
				send_to_char( "¯T¤H ", ch );
			if ( IS_SET( obj->material->race, MUMMY ) )
				send_to_char( "¤ì¤D¥ì ", ch );
			if ( IS_SET( obj->material->race, GARGOYLE ) )
				send_to_char( "¥Û¹³°­ ", ch );
			if ( IS_SET( obj->material->race, CENTAUR ) )
				send_to_char( "¤H°¨ ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_race && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_race, ALLOW_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q±M¥ÎºØ±Ú¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HUMAN ) )
				send_to_char( "¤HÃþ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ELF ) )
				send_to_char( "ºëÆF ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DWARF ) )
				send_to_char( "¸G¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ORC ) )
				send_to_char( "Ã~¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, FAIRY ) )
				send_to_char( "§¯ºë ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GNOME ) )
				send_to_char( "¦aºë ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HOBBIT ) )
				send_to_char( "¶P¤ñ¯S ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DROW ) )
				send_to_char( "·t¶ÂºëÆF ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, VAMPIRE ) )
				send_to_char( "§l¦å°­ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, BEHOLDER ) )
				send_to_char( "²´Å] ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, LIZARDMAN ) )
				send_to_char( "»h»i¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, WEREWOLF ) )
				send_to_char( "¯T¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, MUMMY ) )
				send_to_char( "¤ì¤D¥ì ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GARGOYLE ) )
				send_to_char( "¥Û¹³°­ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, CENTAUR ) )
				send_to_char( "¤H°¨ ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_race, DENY_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q¸T¥ÎºØ±Ú¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HUMAN ) )
				send_to_char( "¤HÃþ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ELF ) )
				send_to_char( "ºëÆF ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DWARF ) )
				send_to_char( "¸G¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, ORC ) )
				send_to_char( "Ã~¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, FAIRY ) )
				send_to_char( "§¯ºë ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GNOME ) )
				send_to_char( "¦aºë ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, HOBBIT ) )
				send_to_char( "¶P¤ñ¯S ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, DROW ) )
				send_to_char( "·t¶ÂºëÆF ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, VAMPIRE ) )
				send_to_char( "§l¦å°­ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, BEHOLDER ) )
				send_to_char( "²´Å] ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, LIZARDMAN ) )
				send_to_char( "»h»i¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, WEREWOLF ) )
				send_to_char( "¯T¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, MUMMY ) )
				send_to_char( "¤ì¤D¥ì ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, GARGOYLE ) )
				send_to_char( "¥Û¹³°­ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_race, CENTAUR ) )
				send_to_char( "¤H°¨ ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_class && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_class, ALLOW_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q±M¥ÎÂ¾·~¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MAGE ) ) )
				send_to_char( "Å]ªk®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( CLERIC ) ) )
				send_to_char( "ªª®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( THIEF ) ) )
				send_to_char( "§®¸é ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "¾Ô¤h ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "³¥ÆZ¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WIZARD ) ) )
				send_to_char( "§Å®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "Áåª÷®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SAVANT ) ) )
				send_to_char( "½åªÌ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BISHOP ) ) )
				send_to_char( "¥D±Ð ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "¨ë«È ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( RANGER ) ) )
				send_to_char( "´å«L ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( NINJA ) ) )
				send_to_char( "§ÔªÌ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "ÃM¤h ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "¼C«È ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MONK ) ) )
				send_to_char( "¹¬«Q ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "§ÅÂå ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SORCERER ) ) )
				send_to_char( "¤Ûªk®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( LORD ) ) )
				send_to_char( "»â¥D ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARLORD ) ) )
				send_to_char( "¾Ô¤ý ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "¨g¾Ô¤h ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_class, DENY_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q¸T¥ÎÂ¾·~¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MAGE ) ) )
				send_to_char( "Å]ªk®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( CLERIC ) ) )
				send_to_char( "ªª®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( THIEF ) ) )
				send_to_char( "§®¸é ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARRIOR ) ) )
				send_to_char( "¾Ô¤h ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BARBARIAN ) ) )
				send_to_char( "³¥ÆZ¤H ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WIZARD ) ) )
				send_to_char( "§Å®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ALCHEMIST ) ) )
				send_to_char( "Áåª÷®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SAVANT ) ) )
				send_to_char( "½åªÌ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( BISHOP ) ) )
				send_to_char( "¥D±Ð ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( ASSASSIN ) ) )
				send_to_char( "¨ë«È ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( RANGER ) ) )
				send_to_char( "´å«L ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( NINJA ) ) )
				send_to_char( "§ÔªÌ ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( KNIGHT ) ) )
				send_to_char( "ÃM¤h ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SWORDSMAN ) ) )
				send_to_char( "¼C«È ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MONK ) ) )
				send_to_char( "¹¬«Q ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SHAMAN ) ) )
				send_to_char( "§ÅÂå ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( SORCERER ) ) )
				send_to_char( "¤Ûªk®v ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( LORD ) ) )
				send_to_char( "»â¥D ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( WARLORD ) ) )
				send_to_char( "¾Ô¤ý ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_class, NUM_BIT( MARTIALMAN ) ) )
				send_to_char( "¨g¾Ô¤h ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_sex && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_sex, ALLOW_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q±M¥Î©Ê§O¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_MALE ) ) )
				send_to_char( "¨k©Ê ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_FEMALE ) ) )
				send_to_char( "¤k©Ê ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_NEUTRAL ) ) )
				send_to_char( "¤¤©Ê ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_sex, DENY_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q¸T¥Î©Ê§O¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_MALE ) ) )
				send_to_char( "¨k©Ê ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_FEMALE ) ) )
				send_to_char( "¤k©Ê ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_sex, NUM_BIT( SEX_NEUTRAL ) ) )
				send_to_char( "¤¤©Ê ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	if( obj->pIndexData->hp_use_align && level >= L_JUN )
	{
		if ( IS_SET( obj->pIndexData->hp_use_align, ALLOW_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q±M¥Î°}Àç¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_EVIL ) ) )
				send_to_char( "¨¸´c ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_GOOD ) ) )
				send_to_char( "µ½¨} ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
				send_to_char( "¤¤¥ß ", ch );
			send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_use_align, DENY_BIT ) )
		{
			send_to_char( "ÁôÂÃ¤O¶q¸T¥Î°}Àç¡G", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_EVIL ) ) )
				send_to_char( "¨¸´c ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_GOOD ) ) )
				send_to_char( "µ½¨} ", ch );
			if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
				send_to_char( "¤¤¥ß ", ch );
			send_to_char( "\n\r", ch );
		}
	}
	/*
	if( obj->pIndexData->hp_affect_race && level >= L_JUN )
	{
		send_to_char( "ÁôÂÃ¤O¶q§J¨îºØ±Ú¡G", ch );
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

		send_to_char( "ÁôÂÃ¤O¶qµL®ÄºØ±Ú¡G", ch );
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
		  send_to_char( "ÁôÂÃ¤O¶q§J¨îÂ¾·~¡G", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MAGE ) ) )
			send_to_char( "Å]ªk®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( CLERIC ) ) )
			send_to_char( "ªª®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( THIEF ) ) )
			send_to_char( "§®¸é ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARRIOR ) ) )
			send_to_char( "¾Ô¤h ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BARBARIAN ) ) )
			send_to_char( "³¥ÆZ¤H ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WIZARD ) ) )
			send_to_char( "§Å®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ALCHEMIST ) ) )
			send_to_char( "Áåª÷®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SAVANT ) ) )
			send_to_char( "½åªÌ ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BISHOP ) ) )
			send_to_char( "¥D±Ð ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ASSASSIN ) ) )
			send_to_char( "¨ë«È ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( RANGER ) ) )
			send_to_char( "´å«L ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( NINJA ) ) )
			send_to_char( "§ÔªÌ ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( KNIGHT ) ) )
			send_to_char( "ÃM¤h ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SWORDSMAN ) ) )
			send_to_char( "¼C«È ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MONK ) ) )
			send_to_char( "¹¬«Q ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SHAMAN ) ) )
			send_to_char( "§ÅÂå ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SORCERER ) ) )
			send_to_char( "¤Ûªk®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( LORD ) ) )
			send_to_char( "»â¥D ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARLORD ) ) )
			send_to_char( "¾Ô¤ý ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MARTIALMAN ) ) )
			send_to_char( "¨g¾Ô¤h ", ch );
		  send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_affect_class, DENY_BIT ) )
		{
		  send_to_char( "ÁôÂÃ¤O¶qµL®ÄÂ¾·~¡G", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MAGE ) ) )
			send_to_char( "Å]ªk®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( CLERIC ) ) )
			send_to_char( "ªª®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( THIEF ) ) )
			send_to_char( "§®¸é ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARRIOR ) ) )
			send_to_char( "¾Ô¤h ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BARBARIAN ) ) )
			send_to_char( "³¥ÆZ¤H ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WIZARD ) ) )
			send_to_char( "§Å®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ALCHEMIST ) ) )
			send_to_char( "Áåª÷®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SAVANT ) ) )
			send_to_char( "½åªÌ ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( BISHOP ) ) )
			send_to_char( "¥D±Ð ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( ASSASSIN ) ) )
			send_to_char( "¨ë«È ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( RANGER ) ) )
			send_to_char( "´å«L ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( NINJA ) ) )
			send_to_char( "§ÔªÌ ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( KNIGHT ) ) )
			send_to_char( "ÃM¤h ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SWORDSMAN ) ) )
			send_to_char( "¼C«È ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MONK ) ) )
			send_to_char( "¹¬«Q ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SHAMAN ) ) )
			send_to_char( "§ÅÂå ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( SORCERER ) ) )
			send_to_char( "¤Ûªk®v ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( LORD ) ) )
			send_to_char( "»â¥D ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( WARLORD ) ) )
			send_to_char( "¾Ô¤ý ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_class, NUM_BIT( MARTIALMAN ) ) )
			send_to_char( "¨g¾Ô¤h ", ch );
		  send_to_char( "\n\r", ch );
		}
		}
		if( obj->pIndexData->hp_affect_sex && level >= L_JUN )
		{
		if ( IS_SET( obj->pIndexData->hp_affect_sex, ALLOW_BIT ) )
		{
		  send_to_char( "ÁôÂÃ¤O¶q§J¨î©Ê§O¡G", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_MALE ) ) )
			send_to_char( "¨k©Ê ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_FEMALE ) ) )
			send_to_char( "¤k©Ê ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_NEUTRAL ) ) )
			send_to_char( "¤¤©Ê ", ch );
		  send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_affect_sex, DENY_BIT ) )
		{
		  send_to_char( "ÁôÂÃ¤O¶qµL®Ä©Ê§O¡G", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_MALE ) ) )
			send_to_char( "¨k©Ê ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_FEMALE ) ) )
			send_to_char( "¤k©Ê ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_sex, NUM_BIT( SEX_NEUTRAL ) ) )
			send_to_char( "¤¤©Ê ", ch );
		  send_to_char( "\n\r", ch );
		}
		}
		if( obj->pIndexData->hp_affect_align && level >= L_JUN )
		{
		if ( IS_SET( obj->pIndexData->hp_affect_sex, ALLOW_BIT ) )
		{
		  send_to_char( "ÁôÂÃ¤O¶q§J¨î°}Àç¡G", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_align, NUM_BIT( ALIGN_EVIL ) ) )
			send_to_char( "¨¸´c ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_align, NUM_BIT( ALIGN_GOOD ) ) )
			send_to_char( "µ½¨} ", ch );
		  if ( IS_SET( obj->pIndexData->hp_affect_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
			send_to_char( "¤¤¥ß ", ch );
		  send_to_char( "\n\r", ch );
		}
		if ( IS_SET( obj->pIndexData->hp_affect_sex, DENY_BIT ) )
		{
		  send_to_char( "ÁôÂÃ¤O¶qµL®Ä°}Àç¡G", ch );
		  if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_EVIL ) ) )
			send_to_char( "¨¸´c ", ch );
		  if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_GOOD ) ) )
			send_to_char( "µ½¨} ", ch );
		  if ( IS_SET( obj->pIndexData->hp_use_align, NUM_BIT( ALIGN_NEUTRAL ) ) )
			send_to_char( "¤¤¥ß ", ch );
		  send_to_char( "\n\r", ch );
		}
	}
	*/
	if ( level > L_APP - 2 )
	{
		OBJ_VRACE * ovr = NULL;
		char buf2 [ MAX_STRING_LENGTH * 2];
		if( obj->vrace ) {
			sprintf(buf, "¹ïºØ±Ú¥[¦¨¡G\n\r" );
			send_to_char(buf, ch);
			for( ovr = obj->vrace ; ovr ; ovr = ovr->next ) {
				// ¥[¤JºØ±Ú­^¤å¦W¡A¨Ã±N¨C¤@¶µ°T®§¤À¶}°e¥X¡A¥H¥[¤J¥¿­t¥[¦¨¤£¦PªºÃC¦â 2023/03/24
				sprintf(buf2, "%s(%s)", race_table[ovr->race].cname, race_table[ovr->race].name );
				sprintf(buf, "%-19s", buf2 );
				send_to_char(buf, ch);
				if( ovr->hr_mod_q > 0 )
					sprintf(buf, "©R¤¤²v "HIC"%d"NOR" ÂI¡A", ovr->hr_mod_q );
				else if( ovr->hr_mod_q < 0 )
					sprintf(buf, "©R¤¤²v "HIR"%d"NOR" ÂI¡A", ovr->hr_mod_q );
				else
					sprintf(buf, "©R¤¤²v %d ÂI¡A", ovr->hr_mod_q );
				send_to_char(buf, ch);
				if( ovr->hr_mod_p > 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIC"%d%%"NOR"¡F", ovr->hr_mod_p );
				else if( ovr->hr_mod_p < 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIR"%d%%"NOR"¡F", ovr->hr_mod_p );
				else
					sprintf(buf, "¦Ê¤À¤ñ %d%%¡F", ovr->hr_mod_p );
				send_to_char(buf, ch);
				if( ovr->dr_mod_q > 0 )
					sprintf(buf, "¶Ë®`¤O "HIC"%d"NOR" ÂI¡A", ovr->dr_mod_q );
				else if( ovr->dr_mod_q < 0 )
					sprintf(buf, "¶Ë®`¤O "HIR"%d"NOR" ÂI¡A", ovr->dr_mod_q );
				else
					sprintf(buf, "¶Ë®`¤O %d ÂI¡A", ovr->dr_mod_q );
				send_to_char(buf, ch);
				if( ovr->dr_mod_p > 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIC"%d%%"NOR"¡C\n\r", ovr->dr_mod_p );
				else if( ovr->dr_mod_p < 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIR"%d%%"NOR"¡C\n\r", ovr->dr_mod_p );
				else
					sprintf(buf, "¦Ê¤À¤ñ %d%%¡C\n\r", ovr->dr_mod_p );
				send_to_char(buf, ch);
				/*sprintf(buf, "¹ï%s¥[¦¨©R¤¤²v%dÂI¡A¦Ê¤À¤ñ%d%%¡F¶Ë®`¤O%dÂI¡A¦Ê¤À¤ñ%d%%¡C\n\r",
							race_table[ovr->race].cname,
							ovr->hr_mod_q,
							ovr->hr_mod_p,
							ovr->dr_mod_q,
							ovr->dr_mod_p);
				send_to_char(buf, ch);*/
			}
		}
		if( obj->material->vrace ) {
			sprintf(buf, "§÷½è¹ïºØ±Ú¥[¦¨¡G\n\r" );
			send_to_char(buf, ch);
			for( ovr = obj->material->vrace ; ovr ; ovr = ovr->next ) {
				// ¥[¤JºØ±Ú­^¤å¦W¡A¨Ã±N¨C¤@¶µ°T®§¤À¶}°e¥X¡A¥H¥[¤J¥¿­t¥[¦¨¤£¦PªºÃC¦â 2023/03/24
				sprintf(buf2, "%s(%s)", race_table[ovr->race].cname, race_table[ovr->race].name );
				sprintf(buf, "%-19s", buf2 );
				send_to_char(buf, ch);
				if( ovr->hr_mod_q > 0 )
					sprintf(buf, "©R¤¤²v "HIC"%d"NOR" ÂI¡A", ovr->hr_mod_q );
				else if( ovr->hr_mod_q < 0 )
					sprintf(buf, "©R¤¤²v "HIR"%d"NOR" ÂI¡A", ovr->hr_mod_q );
				else
					sprintf(buf, "©R¤¤²v %d ÂI¡A", ovr->hr_mod_q );
				send_to_char(buf, ch);
				if( ovr->hr_mod_p > 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIC"%d%%"NOR"¡F", ovr->hr_mod_p );
				else if( ovr->hr_mod_p < 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIR"%d%%"NOR"¡F", ovr->hr_mod_p );
				else
					sprintf(buf, "¦Ê¤À¤ñ %d%%¡F", ovr->hr_mod_p );
				send_to_char(buf, ch);
				if( ovr->dr_mod_q > 0 )
					sprintf(buf, "¶Ë®`¤O "HIC"%d"NOR" ÂI¡A", ovr->dr_mod_q );
				else if( ovr->dr_mod_q < 0 )
					sprintf(buf, "¶Ë®`¤O "HIR"%d"NOR" ÂI¡A", ovr->dr_mod_q );
				else
					sprintf(buf, "¶Ë®`¤O %d ÂI¡A", ovr->dr_mod_q );
				send_to_char(buf, ch);
				if( ovr->dr_mod_p > 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIC"%d%%"NOR"¡C\n\r", ovr->dr_mod_p );
				else if( ovr->dr_mod_p < 100 )
					sprintf(buf, "¦Ê¤À¤ñ "HIR"%d%%"NOR"¡C\n\r", ovr->dr_mod_p );
				else
					sprintf(buf, "¦Ê¤À¤ñ %d%%¡C\n\r", ovr->dr_mod_p );
				send_to_char(buf, ch);
				/*sprintf(buf, "§÷½è¹ï%s±Ú¥[¦¨©R¤¤²v%dÂI¡A¦Ê¤À¤ñ%d%%¡F¶Ë®`¤O%dÂI¡A¦Ê¤À¤ñ%d%%¡C\n\r",
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
			sprintf( buf, "ª««~©|¥i¨Ï¥Î %d ¤p®É\n\r", obj->timer );
			send_to_char( buf, ch );
		}
		else
			send_to_char( "ª««~µL®É¶¡­­¨î\n\r", ch );
	}

	if ( obj->hp || obj->material->hp)
	{
		if (IS_IMMORTAL(ch) || level > L_APP )
		{
			OBJ_HP * ohp ;
			if( obj->hp ){
				for( ohp = obj->hp; ohp; ohp = ohp->next ) {
					if( IS_SET( ohp->event, HP_WEAR ) )
						sprintf( buf, "¸Ë³ÆÄ²µoÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_FIGHT ) )
						sprintf( buf, "©R¤¤Ä²µoÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_WEAPON ) )
						sprintf( buf, "©R¤¤Ä²µoÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_BLOCK ) )
						sprintf( buf, "®æ¾×Ä²µoÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_ARROW ) )
						sprintf( buf, "®gÀ»Ä²µoÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_REMOVE ) )
						sprintf( buf, "ÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					if( IS_SET( ohp->event, HP_DART ) )
						sprintf( buf, "§ëÂYÄ²µoÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
										ohp->level, ohp->hp_name, ohp->percent);
					send_to_char( buf, ch );
				}
			}
			if( obj->material->hp ){
				for( ohp = obj->material->hp; ohp; ohp = ohp->next ) {
					sprintf( buf, "§÷½èÁôÂÃ¤O¶q: %d ¯Åªº %s, ¾÷²v %d%%\n\r", 
									ohp->level, ohp->hp_name, ohp->percent);
					send_to_char( buf, ch );
				}
			}
		}
		else
			send_to_char( "§AÁôÁô·P¨ì¦³ªÑÅ]¤OÂÃ¦b¨ä¤¤¡A«o¤S»¡¤£¥X¬O¤°»ò¤O¶q¡C", ch );
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

	// ªý¾× npc ¨Ï¥Î locate object 2022/10/14
	if( IS_NPC(ch) ) return;

	buf1[0] = '\0';
	found = FALSE;
	// ²¾°£ sklv 6 locate ¤£­­¼Æ¶q 2022/10/14
	//for ( obj = object_list; obj && (sklv > 5 || count <= maxcount) ; obj = obj->next )
	for ( obj = object_list; obj && count <= maxcount ; obj = obj->next )
	{
		if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ) )
			continue;

		//found = TRUE;

		for ( in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj )
			;

		/*  2022/10/14
			­×§ï¬° carried_by ¤Î !carried_by ¨âÃþ , ±N can_see ©ñ¤J carried_by ¤U§P©w
			¦p¦¹ !carried_by ªº³¡¤À±N¤£·|§P©w¨ì carried_by ªºª««~
		*/
		//if ( in_obj->carried_by && can_see( ch, in_obj->carried_by) )
		if ( in_obj->carried_by )
		{
			// ¬Ý¤£¨£«ù¦³ªÌªº±¡ªp , ª½±µ²¤¹Lª««~ 2022/10/14
			if( !can_see( ch, in_obj->carried_by) )
				continue;

			found = TRUE;

			++count;
			++count_buf;
			// ·s¼W§Þ¯àµ¥¯Å 5 ¥H¤W®É·|Åã¥Ü°Ï°ì¦ì¸m 2022/10/14
			if( sklv >= 5 ){
				sprintf( buf, "%s ¥Ñ %s Äâ±a ¦ì©ó %s.\n\r",
								obj->short_descr,
								PERS( in_obj->carried_by, ch ),
								in_obj->carried_by->in_room->area->name
						);
			}
			else
				sprintf( buf, "%s ¥Ñ %s Äâ±a.\n\r",
								obj->short_descr,
								PERS( in_obj->carried_by, ch )
								);
		}
		else
		{
			found = TRUE;
			++count;
			++count_buf;
			// ·s¼W§Þ¯àµ¥¯Å 5 ¥H¤W®É·|Åã¥Ü°Ï°ì¦ì¸m 2022/10/14
			if( sklv >= 5 )
				sprintf( buf, "%s Â\\¦b %s %s %s.\n\r",
								obj->short_descr,
								!in_obj->in_room ? "¬Y³B" : in_obj->in_room->name,
								!in_obj->in_room ? "" : "¦ì©ó",
								!in_obj->in_room ? "" : in_obj->in_room->area->name
						);
			else
				// ²¾°£ obj->name 2022/10/14
				//sprintf( buf, "%s(%s) Â\\¦b %s.\n\r", obj->short_descr, obj->name, !in_obj->in_room ? "¬Y³B" : in_obj->in_room->name);

				sprintf( buf, "%s Â\\¦b %s.\n\r",
								obj->short_descr,
								!in_obj->in_room ? "¬Y³B" : in_obj->in_room->name
						);
		}

		buf[0] = UPPER( buf[0] );
		strcat( buf1, buf );

		// §â count ´«¬° count_buf ¨Ó§P©w , ¨Ã­×§ï¬°·í«e pagelen ³Ì¤j¦æ¼Æ(70) 2022/10/14
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
		send_to_char( "¥@¬É¤W¨S¦³³o¼ËªF¦è.\n\r", ch );

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


	sprintf( buf2, "§A¥l³ê­·ªººëÆF¡A½Ð¦o­Ì§j©Ø¨ì³o­Ó¥@¬Éªº¨C­Ó¨¤¸¨....\n\r\n\r" );

	found = FALSE;
	buf1[0] = '\0';
	// ²¾°£ sklv 6 ¤£­­¼Æ¶qªº³]©w 2022/10/14
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
			sprintf( buf, "%s ¦ì¦b %s¦a°Ï - %s\n\r",
							victim->short_descr,
							victim->in_room->area->name,
							victim->in_room->name
					);
			strcat( buf1, buf );
			// ±N count ­×§ï¬° count_buf ,¨Ã­×§ï¬°²Å¦X³Ì¤j pagelen 2022/10/14
			//if( count >= 50 ) {
			if( count_buf >= 80 )
			{
				//count = 0;
				count_buf = 0;
				// ¦pªG§ìcount >= 80 ¥H¤W, «h buf2 ¦b²Ä¤@¦¸¿é¥X®É¥ý¿é¥X 2022/10/14
				strcat( buf2, "­·ªººëÆF¦^¨Ó¡A®U®U§i¶D§A¦o¦b®È³~¤W¹J¨ìªº¥Íª«¡G\n\r" );
				send_to_char( buf2, ch );
				send_to_char( buf1, ch );
				buf1[0] = '\0';
			}
		}
	}

	if ( !found )
	{
		strcat( buf2, "­·ªººëÆF«Ü¿ò¾Ñ¦a§i¶D§A¡A¦o­Ì¨S¦³¬Ý¨ì¥ô¦óÃþ¦ü§A´y­zªº¥Íª«¡C\n\r" );
		send_to_char( buf2, ch );
	}
	else
	{
		// count >= 80 ªº±¡ªp¤U, ¦b for ¤º·|¹w¥ý¿é¥X buf2  2022/10/14
		if( count <= 79 )
		{
			strcat( buf2, "­·ªººëÆF¦^¨Ó¡A®U®U§i¶D§A¦o¦b®È³~¤W¹J¨ìªº¥Íª«¡G\n\r" );
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
		// §â difflv ­×¥¿¬°¥¿­È, §_«h¤U¤è§P©w¦¡¤£§@¥Î 2023/01/13
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

		// ­×§ï¬°¥u¦³ ch ·|¦©°£ exp 2023/01/13
		//if( ch->exp < difflv + 1000 || victim->exp < 1000 + difflv )
		if( ch->exp < difflv + 1000 )
		{
			send_to_char("§Aªº¯à¤O¤£¨¬¥HÀ°§U¥L.\n\r", ch);
			return;
		}

		// ±N difflv Âà´«¬°­t­È 2023/01/13
		difflv = 0 - difflv;

		// ­×§ï¬°¥u¦³ ch ·|¦©°£ exp 2023/01/13
		gain_exp( ch , difflv );
		//gain_exp( victim, difflv);
		sprintf(buf,"§A¥¢¥h¤F %d ÂI¸gÅç­È.\n\r", difflv * -1 );
		send_to_char(buf, ch);
		//send_to_char(buf, victim);
	}

	// §â grave ²¾¨ì exp §P©w«á 2023/01/13

	sprintf( buf, "%s ªº«ÍÅé(corpse)", victim->name );

	sprintf( buf2, "%s", victim->short_descr );


	for ( obj = object_list; obj; obj = obj->next )
	{
		if ( !can_see_obj( ch, obj ) || ( str_cmp( buf, obj->short_descr ) ) )
			continue;

		// ªý¾×«D pc corpse ³Q grave ¨ì 2023/01/13
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
		// ­×§ï¬° act Åã¥Ü«ÍÅé¤Î ch 2023/01/13
		//act( "¤@¨ã«ÍÅé¬ðµM¥X²{¦b§A­±«e¡I", ch, NULL, NULL, TO_ROOM );
		//send_to_char( "§A§â«ÍÅé²¾°Ê¨ì¤F³o¸Ì¡C\n\r", ch );
		act( "$n§â¤@¨ã$Tªº«ÍÅé²¾°Ê¨ì¤F³o¸Ì¡C", ch, NULL, buf2, TO_ROOM );
		act( "§A§â¤@¨ã$Tªº«ÍÅé²¾°Ê¨ì¤F³o¸Ì¡C", ch, NULL, buf2, TO_CHAR );
		++count;
		if( !IS_NPC(ch) && sklv < 6 && count > sklv/2 )
			break; 
	}

	if ( !found )
	{
		send_to_char( "¥L¬Ý¨Ó¤£»Ý­n§AªºÀ°§U.\n\r", ch );
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
		send_to_char( "¨º¤£¯à³Q¥R¯à.\n\r", ch );
		return;
	}

	nSuccess = get_curr_wis( ch ) + nSklv * (nSklv + 4) + nPerc / 2 + 2 * (ch->level - obj->value[0]);

	if (!IS_NPC( ch ) && number_percent() < nSuccess && obj->value[2] < obj->value[1])
	{
		act( "$p ªx°_¤@°}«G¥ú¡AµM«á¤Sº¥º¥«ì´_­ì¼Ë.", ch, obj, NULL, TO_CHAR );
		act( "$p ªx°_¤@°}«G¥ú¡AµM«á¤Sº¥º¥«ì´_­ì¼Ë.", ch, obj, NULL, TO_ROOM );
		obj->value[2] = obj->value[1];
		obj->cost     = 1;
	}else{
		// ¦³ z4 ªº wand staff ¤£·|³Q¥RÃz add at 2020/10/18
		if( IS_SET(obj->pIndexData->souvenir, Z4) )
		{
			act( "$p ªx°_¤@°}«G¥ú¡A¤S¬ðµM·t¤F¤U¨Ó...", ch, obj, NULL, TO_CHAR );
			act( "$p ªx°_¤@°}«G¥ú¡A¤S¬ðµM·t¤F¤U¨Ó...", ch, obj, NULL, TO_ROOM );
		}
		else
		{
			act( "$p ªx°_¤@°}«G¥ú¡AµM«á¬ðµM¬µ¦¨¸H¤ù¡I", ch, obj, NULL, TO_CHAR );
			act( "$p ªx°_¤@°}«G¥ú¡AµM«á¬ðµM¬µ¦¨¸H¤ù¡I", ch, obj, NULL, TO_ROOM );
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
  send_to_char( "¨S¦³¤°»ò­È±o´y­zªº¨Æµo¥Í.\n\r", ch );
  return;
    }

  SET_BIT( obj->extra_flags, ITEM_REMOVE_NODROP );
  act( "¤@¹Dª÷¦âªº¥ú¨~Å¢¸nµÛ$p.",
      ch, obj, NULL, TO_CHAR );
  act( "¤@¹Dª÷¦âªº¥ú¨~Å¢¸nµÛ$p.",
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
  send_to_char( "¤°»ò¨Æ¤]¨Sµo¥Í.\n\r", ch );
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
  act( "$p µo¥X¤@°}§C»ï¡AµM«á¤S¦wÀR¤U¨Ó.", ch, obj, NULL, TO_CHAR );
  act( "$p µo¥X¤@°}§C»ï¡AµM«á¤S¦wÀR¤U¨Ó.", ch, obj, NULL, TO_ROOM );
  return;
    }

    SET_BIT( obj->extra_flags, ITEM_NODROP );
    obj->wear_flags = 1;      /* Useless   */
    obj->cost     = 0;      /* Worthless */
    act( "$p µo¥X¤@°}±j¥ú¡AµM«áÅÜ¦¨¦Ç¦â.", ch, obj, NULL, TO_CHAR );
    act( "$p µo¥X¤@°}±j¥ú¡AµM«áÅÜ¦¨¦Ç¦â.", ch, obj, NULL, TO_ROOM );

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
		// ·s¼W¸Ë³Æ¦ì¸m ¼uÃÄ(WEAR_AMMO) ¬G¼W¥[ iWear ¼Æ­È 2022/04/30
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
		 
				act( "§A¥á¤U $p.",  victim, obj, NULL, TO_CHAR );
				act( "$n ¥á¤U $p.", victim, obj, NULL, TO_ROOM );
				yesno = 1;
			}
		}
	}
    if ( is_affected( victim, gsn_curse ) )
    {
		affect_strip( victim, gsn_curse );
		send_to_char( "§AÄ±±o¦n¦h¤F.\n\r", victim );
		yesno = 1;
	}

	if ( ch != victim && yesno )
		send_to_char( "¦nªº¡C\n\r", ch );
	return;
}


void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim;

	// ¤À¶} summon ¥¢±Ñ±ø¥ó, ¨Ã¥[¤J·s³]©w 2022/12/20
	if( !( victim = get_char_world( ch, target_name ) ) )
	{
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
		return;
	}

	// room ¤Î area §P©w 2022/12/20
	if(    room_is_private( victim , ch->in_room )
		|| victim->in_room->area != ch->in_room->area
		|| IS_SET( victim->in_room->room_flags, ROOM_SAFE )
		|| IS_SET( victim->in_room->room_flags, ROOM_SUMMON_BIT )
		|| IS_SET( ch->in_room->room_flags, ROOM_SUMMON_BIT ) )
	{
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
		return;
	}

	// ¨¤¦âª¬ºA§P©w 2022/12/20
	if(    victim == ch
		|| !victim->in_room
		|| ( !IS_NPC(victim) && !victim->desc )  // ³s½uª¬ºA§P©w¥[¤J pc ­­©w 2022/12/20
		|| ( get_skill_level(ch,sn) < 5 && IS_AFFECTED( victim, AFF_CURSE ) )
		|| victim->level > ch->level + get_skill_level( ch, sn )
		|| victim->fighting )
	{
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
		return;
	}

	// ­×§ï npc ³Q summon ªº§P©w 2022/12/20
	if( IS_NPC( victim ) )
	{
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
		return;
	}

	act( "$n ¬ðµM±q§A­±«e®ø¥¢.", victim, NULL, NULL, TO_ROOM );

	// ¼W¥[ summon ¹ï room_leave §@§P©w 2022/12/20
	if( victim->in_room->roomprogs && victim->in_room->progtypes & ROOM_LEAVE )
		rprog_leave_trigger( victim->in_room, victim , 8);

	char_from_room( victim );
	char_to_room( victim, ch->in_room );

	act( "$n ¥l³ê¤F§A¡I", ch, NULL, victim, TO_VICT );
	act( "$n ¬ðµM¥X²{¦b³o¸Ì.", victim, NULL, NULL, TO_ROOM );
	do_look( victim, "auto" );

	// ¼W¥[ summon ¹ï room_enter , entry_prog , greet_prog §P©w 2022/12/20
	if ( victim->in_room->roomprogs && ( victim->in_room->progtypes & ROOM_ENTER ) && victim->position > POS_DEAD )
		rprog_entry_trigger( victim->in_room , victim, 6 );

	// ·s¼W npc hurt prog 2022/12/20
	if( IS_NPC(victim) )
		mprog_hurt_trigger( victim, ch, sn );

		/*if ( IS_SET( victim->act, PLR_HORSE ) )
		{
			horse = victim->pcdata->horse;
			stop_fighting( horse, TRUE );
			act( "[36m$n ¬ðµM±q§A­±«e®ø¥¢¡C[m", horse, NULL, NULL, TO_ROOM );
			char_from_room( horse );
			char_to_room( horse, ch->in_room );
			act( "[36m$n ¬ðµM¥X²{¦b³o¸Ì¡C[m", horse, NULL, NULL, TO_ROOM );
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
		send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
		return;
	}

	if( auction_list ) {
		AUC_DATA * auction, *auc_next;
		for( auction = auction_list ; auction; auction = auc_next )
		{
			if ( victim == auction->owner || victim == auction->buyer )
			{
				send_to_char( "§A¥¢±Ñ¤F.\n\r", ch );
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
		send_to_char( "³o­ÓªÅ¶¡¦ü¥G¨S¦³¥i¥HÅý§AµÛ³°ªº¦a¤è!\n\r", ch );
		return;
	}

	if( !str_cmp( ch->in_room->area->identity, "immhall" ) )
	{
		send_to_char( "³o­ÓªÅ¶¡¦ü¥G¨S¦³¥i¥HÅý§AµÛ³°ªº¦a¤è!\n\r", ch );
		return;
	}

	if( victim->in_room->progtypes & ROOM_LEAVE)
		rprog_leave_trigger( victim->in_room, victim, 8 );
	if ( victim->fighting )
		stop_fighting( victim, TRUE );
	act( "$n ªº¨­¼v³vº¥®ø¥¢.", victim, NULL, NULL, TO_ROOM );
	send_to_char( "§A³Q±j­¢¶Ç°e¨ì¥t¤@­ÓªÅ¶¡¡I\n\r", victim );
	char_from_room( victim );
	char_to_room( victim, pRoomIndex );
	act( "$n ªº¨­¼v³vº¥¥X²{.",   victim, NULL, NULL, TO_ROOM );
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
		act( "[36m$n ªº¨­¼v³vº¥®ø¥¢¡C[m", horse, NULL, NULL, TO_ROOM );
		char_from_room( horse );
		char_to_room( horse, pRoomIndex );
		act( "[36m$n ªº¨­¼v³vº¥¥X²{¡C[m", horse, NULL, NULL, TO_ROOM );
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
		send_to_char("§Aªºªk¤O¤£°÷¤F¡C\n\r", ch);
		return;
	}
	ch->mana -= level * 8 ;
	ch->mana  = ch->mana + nSklv * 10;
	send_to_char("§A¶°¤¤ºë¯«¡A³Ð³y¥XÅ]ªk½b¡I\n\r", ch);
	pArrow = fcreate_object( get_obj_index(OBJ_VNUM_MAGIC_ARROW), level );

	// ½b¤ä¶Ë®`¤O: pArrow->value[1] - pArrow->value[2] (¥­§¡ 52 + arrow->value[3]).
		// ÁÙ¦³ arrow->value[0] ¤ä.
	pArrow->value[0] = 200 + nSklv * 100;
	pArrow->value[1] = UMIN( 60 + nSklv * (10 + (ch->max_mana)/550) , 50 + nSklv * (4 + (ch->max_mana)/300) );
	pArrow->value[2] = 60 + nSklv * (10 + (ch->max_mana)/550);
	pArrow->value[3] = 30 + nSklv * 20;

	//  ±N apply ­×§ï¬° apply_shoot, ¨Ï³o¨Ç¶Ë®`¶È¦b shot ®É§@¥Î  2022/05/06
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

// Å]ªk¼u»s§@ 2022/04/30
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
		send_to_char( "¤°»ò¡H\n\r", ch );
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
		send_to_char( "§A¨­¤W¨Ã¨S¦³±aÅ]´¹¥Û.\n\r", ch );
		return;
	}

	if( SpellName[0] == '\0' ){
		send_to_char( "»s§@Å]ªk¼u¿é¤J ammo '§ðÀ»ªk³N¦W' \n\r", ch );
		return;
	}

	if( ( sn = skill_lookup( SpellName ) ) == 0 ){
		send_to_char( "§AÁÙ¨S¾Ç·|³o­Óªk³N.\n\r", ch );
		return;
	}

	if( ( sn = skill_lookup( SpellName ) ) < 0 || ( !IS_NPC( ch ) && get_skill_percent(ch, sn) == 0 ) ){
		send_to_char( "§AÁÙ¨S¾Ç·|³o­Óªk³N.\n\r", ch );
		return;
	}

	if( skill_table[sn].target != TAR_CHAR_OFFENSIVE ){
		if(    sn != 42    // faerie fog
			&& sn != 95    // whirlwind
			&& sn != 205 ) // sun flash
		{
			send_to_char( "ªþ¥[ªºªk³N¥²¶·¬°§ðÀ»©Êªk³N.\n\r", ch );
			return;
		}
	}

	if( ch->move < 200 - nSklv ){
		send_to_char( "§AªºÅé¤O¤£°÷¤F.\n\r", ch );
		return;
	}

	ch->move -= 200 - nSklv;

    if( ( !IS_NPC( ch ) && number_percent() > 1 + get_skill_percent(ch, gsn_artifacts_mastery) ) )
    {
		send_to_char( "§A¸ÕµÛ±NÅ]¤OÄ_¥Û»s§@¦¨Å]ªk¼uÃÄ¡A¦ý¥¢±Ñ¤F¡C\n\r", ch );
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
	ammo->value[0] = 1 + GemQT/(18 - nSklv);                     // ¼Æ¶q
	ammo->value[1] = 30 + nSklv*10 + (GemQT*(10 + nSklv/2))/10;  // ¶Ë®`
	ammo->value[2] = UMAX( 1 , ch->level - 15 );                 // ªk³Nµ¥¯Å
	ammo->value[3] = sn;                                         // ªk³NºØÃþ
	if( ammo->value[2] > 0 )
	{
		free_string( ammo->short_descr );
		free_string( ammo->name );
		free_string( ammo->description );
		sprintf( buf, "%s magic ammo", skill_table[sn].name );
		ammo->name = str_dup( buf );
		sprintf( buf, "%sÅ]ªk¼u(%s magic ammo)", skill_table[sn].cname, skill_table[sn].name );
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

    act( "§A§âÅ]¤OÄ_¥Û»s§@¦¨ $p¡C", ch, ammo, NULL, TO_CHAR );

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

	// ¹ï©Ð¶¡¤ºÅ]¤OÄ_¥Û¼Æ¶q§@­­¨î 2022/05/21
	for( obj = ch->in_room->contents; obj; obj = obj->next_content ){
		if( obj->pIndexData == get_obj_index( OBJ_VNUM_POWER_GEM ) )
			count += 1;
	}

	if( count >= 300 ){
		send_to_char( "§A¥¢±Ñ¤F.\n\r",ch);
		return;
	}


    if ( ch->mana < 500 )
    {
      send_to_char( "§A¨S¦³¨¬°÷ªºªk¤O¡I\n\r", ch );
      return;
    }

	if ( ( IS_NPC( ch ) && (IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED(ch, AFF_SUMMONED)))
		|| ( !IS_NPC( ch ) &&
		number_percent() > 1 + get_skill_percent(ch, gsn_abstraction)))//ch->pcdata->learned[gsn_abstraction] ) )
	{
		send_to_char( "§A¸ÕµÛ±NÅ]¤O´£·Ò¦¨Å]¤OÄ_¥Û¡A¦ý¥¢±Ñ¤F¡C\n\r", ch );
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

    act( "§A±N¥þ¨­Å]¤O¶°¤¤¡A¤@Áû $p ºCºC¦b²´«e¦¨§Î¡I", ch, gem, NULL, TO_CHAR );
    act( "$n ³¬¤W²´·ú¹ª°Êªk¤O¡A¤@Áû $p ºCºC¥X²{¦b $s ­±«e¡I\n\r", ch, gem, NULL, TO_ROOM );
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
      send_to_char( "§A¸ÕµÛ±NÅ]´¹¥Û¤¤ªºªk¤O¤Þµo¥X¨Ó¡A¦ý¥¢±Ñ¤F¡C\n\r", ch );
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
    send_to_char( "§A±NÅ]´¹¥Ûªºªk¤OÄÀ©ñ¡A¦]¦Ó±o¨ì¤F¸É¥R¡C\n\r", ch );
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
		send_to_char( "§A·Q¤Þµo¤°»ò?\n\r", ch );
		return;
	}

	if ( !( obj = get_powergem_carry( ch, arg ) ) )
	{
		send_to_char( "§A¨­¤W¨Ã¨S¦³±aÅ]´¹¥Û.\n\r", ch );
		return;
	}

	if ( ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) ) || ( !IS_NPC( ch )
		&& number_percent() > get_skill_percent(ch, gsn_abstraction)))
	{
		send_to_char( "§A¸ÕµÛ±NÅ]´¹¥Û¤¤ªºªk¤O¤Þµo¥X¨Ó¡A¦ý¥¢±Ñ¤F¡C\n\r", ch );
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
	send_to_char( "§A±NÅ]´¹¥Ûªºªk¤OÄÀ©ñ¡A¦]¦Ó±o¨ì¤F¸É¥R¡C\n\r", ch );
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
		send_to_char("§A§ä¤£¨ì«ÍÅé¡C\n\r", ch);
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
		send_to_char("§Aªº´_¬¡³NÁÙ¤£°÷¯Â¼ô¡A¥²¶·¦³¤@¬°½åªÌ»²§U§A¬I®i¡C\n\r", ch);
		return;
	}

	if( !IS_NPC(rch) )
	{
		if( rch->mana < 50 )
		{
			send_to_char("¥LµLªkÀ°§A¡C\n\r", ch);
			return;
		}
		rch->mana -= 100;
		ch->mana -= 200;
	}

	if( corpse->item_type != ITEM_CORPSE_PC)
	{
		send_to_char("¨º¤£¬Oª±®aªº«ÍÅé¡C\n\r", ch);
		return;
	}

	if( corpse->level > level + sklv  )
	{
		send_to_char("§Aªºµ¥¯Å¤£°÷¡C\n\r", ch);
		return;
	}

	arg1 = corpse->name;
	arg = one_argument( arg1, temp) /* "corpse" */;

	if( !(vch = get_char_world( ch, arg)) || IS_NPC(vch) || !vch->in_room)
	{
		send_to_char("¨S¦³³o­Óª±®a¡C\n\r", ch);
		return;
	}

	if( IS_SET(vch->in_room->room_flags, ROOM_NO_RECALL))
	{
		send_to_char("§A¥¢±Ñ¤F¡C\n\r", ch);
		return;
	}

	if ( is_qmark(vch, "spell_resurrection_timer") )
	{
		send_to_char("¥LªºÆF»î±j«×ÁÙ¨S¦³¾®Áå¨ì¯à¦A¦¸±µ¨ü´_¬¡¡I\n\r",ch);
		return;
	}

	act( "$nªº¨­¼vÀHµÛ¤@°}·ÏÃú¦Ó®ø¥¢ .", vch, NULL, NULL, TO_ROOM );
	old_inroom = ch->in_room;
	char_from_room( vch );
	char_to_room(vch, old_inroom);
	act( "$n¬ðµM¥X²{¦b³o¸Ì.", vch, NULL, NULL, TO_ROOM );
	act( "$n±N§A¥l³ê¦^«ÍÅé©Ò¦b¤§³B.", ch, NULL, vch, TO_VICT );
	send_to_char("¦nªº¡C\n\r", ch);
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
		send_to_char("³o¸Ì¤£¯à¶i¦æ¶Ç°e.\n\r" , ch );
		return;
	}

	//to_room = get_room_index(ch->in_room->area->recall);

	if(    !(to_room = get_room_index(ch->in_room->area->recall))
		&& !(qmark = is_qmark( ch, "evacuate_loc" ))== NULL )
	{
		send_to_char("³o¸Ì¤£¯à¶i¦æ¶Ç°e.\n\r" , ch );
		return;
	}

	if(    (get_skill_level(ch,sn) < 5 && IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
		|| (get_skill_level(ch,sn) < 4 && IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)) )
	{
		send_to_char("§Aªº¯à¤O¤£¨¬¥H¦b³o¸Ì¶i¦æ¶Ç°e.\n\r", ch);
		return;
	}

	// °l¥[ evacuate ¥i¦b¯S®íªºroom flag¤U, ¼Ð°O¶i¦æ¥þ¶¤¶Ç°e 19/12/07
	if( !str_cmp( target_name, "mark" )){
		if (IS_SET(ch->in_room->room_flags, ROOM_CAMP))
		{
			if(get_skill_level(ch,sn) < 3){
				send_to_char("§Aªº¯à¤O¤£¨¬¥H³]©w¼Ð°O.\n\r" , ch );
				return;
			}
			if( ch->mana < cost * 3 ){
				send_to_char("§Aªºªk¤O¤£°÷¤F.\n\r" , ch );
				return;
			}
			send_to_char("§A¦b¦a­±³]©w¤F¶Ç°e¼Ð°O.\n\r" , ch );
			ch->mana -= cost * 3;
			if( (qmark = is_qmark(ch, "evacuate_loc")) ){
				qmark->v0 = ch->in_room->vnum;
				qmark->timer = get_skill_level(ch,sn) * 10; // ¥[¤J timer ­«¸m 2022/12/20
				return;
			}
			sprintf( buf, "self evacuate_loc %d %d 0 0 0 0" , get_skill_level(ch,sn)*10, ch->in_room->vnum);
			do_add_qmark( ch, buf );
			return;
		}
		send_to_char("§AµLªk¦b³o¸Ì³]©w¶Ç°e¼Ð°O.\n\r" , ch );
		return;
	}else if( !str_cmp( target_name, "cancel") ){
			if( ( qmark = is_qmark(ch, "evacuate_loc") )){
				send_to_char("§A¨ú®ø¤F¶Ç°e¼Ð°O.\n\r" , ch );
				do_del_qmark( ch, "self evacuate_loc");
				return;
			}
			send_to_char("§A¨S¦³³]©w¥ô¦óªº¶Ç°e¼Ð°O.\n\r" , ch );
			return;
	}else{
		if( (qmark = is_qmark( ch, "evacuate_loc" )) ){
			to_room = get_room_index(qmark->v0);
		}else{
			to_room = get_room_index(ch->in_room->area->recall);
		}
		
		if(to_room == NULL){
			send_to_char("§A¥¢±Ñ¤F.\n\r" , ch );
			return;
		}

		if(get_skill_level(ch,sn) < 6 && (ch->in_room->area->recall != to_room->area->recall))
		{
			send_to_char("§Aªº¯à¤O¤£¨¬¥H¶Ç°e¨ì¤£¦Pªº®ÉªÅ¤¤.\n\r", ch);
			return;
		}
		if(get_skill_level(ch,sn) < 4 && IS_SET(to_room->room_flags, ROOM_NO_TELEPORT_TO))
		{
			send_to_char("§Aªº¯à¤O¤£¨¬¥H¶Ç°e¨ì¨º¸Ì.\n\r", ch);
			return;
		}
	}

	send_to_char( HIG "Ä£²´ªºÅ]ªk°}¥X²{¦b¦a­±¡A¦ÛÅ]ªk°}¤¤¥X²{ªº­·¯«¤Û¼v¸ÕµÛ§A¤Î¶¤¤Í±aÂ÷³o­Ó¦a¤è¡I\n\r"NOR"", ch);
	act( HIG "¦a­±©¿µM¥X²{¤FÄ£²´ªº¥|¨¤Å]ªk°}¡Aºò±µµÛ²´«eªº´º¦â¶}©l§á¦±ÅÜ§Î¡C\n\r"NOR"" , ch, NULL, NULL, TO_ROOM );


	for( gch = ch->in_room->people ; gch ; gch = gch_next ) 
	{

		if( ch->mana < cost ){
			send_to_char("\n\r§Aªºªk¤O¤£°÷¶Ç°e§ó¦h¤H¤F.\n\r" , ch );
			break;
		}

		gch_next = gch->next_in_room;

		if( gch == ch ) continue;
		if( IS_NPC ( gch ) ) continue;		//Added by Razgriz	20050831
		//if( !can_see( ch, gch ) ) continue;  ²¾°£¬Ý¤£¨£ªº¶¤¤ÍµLªk³Q¶Ç°e 19/12/07

		if( is_same_group( ch, gch ) ) 
		{
			ch->mana -= cost; // ¶Ç°e¨C­Ó¶¤¤Í©Ò¯Ó¶Oªº mana

			if( gch->in_room->progtypes & ROOM_LEAVE)
				rprog_leave_trigger( gch->in_room, gch, 8 );

			if( gch->fighting )
				stop_fighting( gch, TRUE );

			send_to_char( HIG "\n\r§A¸}¤U¥X²{ªº­·¯«¤Û¼v¡A±N§A±aÂ÷³o­Ó¦a¤è¡I\n\r"NOR"", gch);
			act( "$n ªº¨­¼v³vº¥®ø¥¢.", gch, NULL, NULL, TO_ROOM );
			char_from_room( gch);
			char_to_room( gch, to_room);
			act( "$n ªº¨­¼v³vº¥¥X²{.", gch, NULL, NULL, TO_ROOM );
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

	act( "$n ªº¨­¼v³vº¥®ø¥¢.", ch, NULL, NULL, TO_ROOM );
	char_from_room( ch);
	char_to_room( ch, to_room);
	act( "$n ªº¨­¼v³vº¥¥X²{.", ch, NULL, NULL, TO_ROOM );
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
  send_to_char( "¨ºµLªk¦A³Q¼W±j¤F.\n\r", ch );
  return;
    }

    if ( obj->wear_loc != WEAR_NONE )
  remove_obj( ch, obj->wear_loc, TRUE );

    /* Thanks to Magoo for this little insight */
    if ( !IS_NPC(ch) && obj->level > level )
    {
  send_to_char( "§AµLªk¬°§ó°ª¯ÅªºÅ@¨ã¼W±j.\n\r",
         ch);
  return;
    }
    hr = 1 + URANGE(0,((ch->level - 14)/7), 4);
    dr = 3 + URANGE(0,((ch->level - 15)/5),5) + URANGE(0, ((ch->level - 35) /2), 4);


    if ( IS_GOOD( ch ) )
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL);
  act( "$p ªx°_¤@°}ÂÅ¥ú.",   ch, obj, NULL, TO_CHAR );
        hr += 1;
        dr -= 3;
    }
    else if ( IS_EVIL( ch ) )
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
  act( "$p ªx°_¤@°}¬õ¥ú.",   ch, obj, NULL, TO_CHAR );
        hr -= 1;
        dr += 4;
    }
    else
    {
  SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
  SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
  act( "$p ªx°_¤@°}¶À¥ú.", ch, obj, NULL, TO_CHAR );
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

    send_to_char( "¦nªº¡C\n\r", ch );
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
       
  send_to_char( "¦nªº¡C\n\r", ch );
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

	/*  ­×§ï¥[¦¨§P©w 2023/01/05
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

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					//send_to_char( "¨S§ì¨ìobj.\n\r", ch );
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
			// ·s¼W¸Ë³Æ ammo 2022/04/30
			else if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
				obj = get_eq_char( ch, WEAR_AMMO );
			}else{
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
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
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 1 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIU"§A©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIU"©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIU"©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIU"©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIU"§A©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIU"©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIU"©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIU"§A©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIU"©PÃäªºÅ]¤O¾®»E¨ì"NOR"$p"HIU"¤§¤¤¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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
	
	/*  ­×§ï¥[¦¨§P©w 2023/01/05
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

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 2 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIG"§A¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIG"¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIG"¥|©PªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIG"¥|©PªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIG"§A¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIG"¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIG"¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIG"§A¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIG"¨­®ÇªÅ¶¡§Î¦¨¤@¹D®ð±Û¡AÄñÂ¶¦b"NOR"$p"HIG"©P³ò¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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

	/*  ­×§ï¥[¦¨§P©w 2023/01/05
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

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 3 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIR"§A¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIR"¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIR"§A¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIR"¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIR"¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIR"§A¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIR"¨­¤Wªº"NOR"$p"HIR"¬ðµM°n¥X¨`¼öªº¤õªá¡A¨ª¬õ¦a¿U¿N°_¨Ó¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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

	/*  ­×§ï¥[¦¨§P©w 2023/01/05
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

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 4 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIC"§A¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIC"¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIC"¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIC"¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIC"§A¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIC"¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIC"¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIC"§A¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIC"¥|©Pªº·Å«×ÆJ­°¡A¤ô®ð¦b"NOR"$p"HIC"ªí­±¾®µ²¬°ÂIÂI¦BÁ÷¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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
	
	/*  ­×§ï¥[¦¨§P©w 2023/01/05
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

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 5 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""YEL"§A©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"YEL"©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""YEL"©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""YEL"©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""YEL"§A©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"YEL"©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"YEL"©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""YEL"§A©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"YEL"©P³òªº¸H¥Û¨ü¨ì§l¤Þ¡A±N"NOR"$p"YEL"¥]ÂÐ±o±K¤£³z­·¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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

	/*  ­×§ï¥[¦¨§P©w 2023/01/05
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

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 6 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIY"§A¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIY"¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""HIY"¥|©PªºªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""HIY"¥|©PªºªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIY"§A¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIY"¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIY"¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIY"§A¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIY"¨­®ÇªÅ®ð´þ´þ§@ÅT¡A«b¨º¶¡"NOR"$p"HIY"Ãzµo¥X±j¤jªº¹q®ð¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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
		send_to_char( "§Aªº¤ßÆFµLªk¾®»E¯«¸tªº¤O¶q.\n\r", ch );
		return; 
	}

	/*  ­×§ï¥[¦¨§P©w 2023/01/05
		if( get_curr_wis( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_wis( ch ) - 21) * get_curr_wis( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_wis( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_wis( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_wis( ch ) + 24 + sklv * 2 ) )/60; // md

	if ( !IS_NPC( ch ) ) {  // align ¼vÅT
		if( ch->alignment >= 0 )
			AliDiff = 299 + ch->alignment;
		else
			AliDiff = 300 + ch->alignment;
		enc_mod = UMAX( ch->level/2 , enc_mod * AliDiff /1299 );
	}
	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch
		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 7 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIW"§A¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIW"¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIW"§A¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIW"¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIW"¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIW"§A¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIW"¨­¤Wªº"NOR"$p"HIW"¨ü¨ì¯¬ºÖ¡A¦ÛÅé´²µoÄ£²´ªº¯«¸t´T¥ú¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
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
		send_to_char( "§Aªº¤ßÆFµLªk¾®»E¨¸´cªº¤O¶q.\n\r", ch );
		return;
	}

	/*  ­×§ï¥[¦¨§P©w 2023/01/05
		if( get_curr_wis( ch ) > 21 )
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( (get_curr_wis( ch ) - 21) * get_curr_wis( ch ) + 10 + UMAX( 0 , ch->pp ) ) * (1 + sklv)) / 2;
		else
			enc_mod = (ch->level + UMAX( 0 , ch->amdr ) + ( get_curr_wis( ch ) + 5 + UMAX( 0 , ch->pp ) ) * (1 + sklv))/2;
	*/
	enc_mod = ch->level/2; // base

	enc_mod += ( UMAX( 0 , ch->pp ) * (get_curr_wis( ch ) + 30 + 2 * sklv * sklv) )/14; // pp , max 9 times

	enc_mod += ( UMAX( 0 , ch->amdr ) * ( get_curr_wis( ch ) + 24 + sklv * 2 ) )/60; // md
	
	if ( !IS_NPC( ch ) ) {  // align ¼vÅT
		if( ch->alignment > 0 )
			AliDiff = 300 - ch->alignment;
		else
			AliDiff = 299 - ch->alignment;
		enc_mod = UMAX( ch->level/2 , enc_mod * AliDiff / 1299);
	}
	if( !isObj ){
		target_name = one_argument( target_name , arg); // obj

		target_name = one_argument( target_name , arg1); // vch

		if( arg1[0] == '\0' ){ // µLvch
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
				// ­×§ï¬°¤£»Ý­n§¹¾ãªº¦WºÙ 2023/01/05
				//if( !can_see_obj( ch, obj ) || !is_name( arg , obj->name ) ){
				if( !can_see_obj( ch, obj ) ){
					send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
					return;
				}
				if ( obj == NULL ){
					return;
				}
			}else{
				send_to_char( "³o¸Ì¨S¦³³o­ÓªF¦è.\n\r", ch );
				return;
			}
		}
	}
	if( isChar ){
		if( arg[0] == '\0' ){ // µL arg ¥BµL arg1 ( ch )
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
				send_to_char( "§A¨S¦³¬ïÀ¹¥i¥HªþÅ]ªºª««~.\n\r", ch);
				return;
			}
		}else{ // ¦³ arg ¥B¦³ arg1
			if( !(vch = get_char_room(ch , arg1)) ){  // room ¤¤§ä¤£¨ì arg1 ³o­Ó vch
				send_to_char( "³o¸Ì¨S¦³³o­Ó¤H.\n\r", ch );
				return;
			}
			if( !str_cmp( arg , "wield" ) || !str_cmp( arg , "wp" ) ){
				if( !get_eq_char( vch, WEAR_WIELD ) && (!get_eq_char( vch, WEAR_TWO_HAND )) ){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
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
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_WIELD_2 );
				}
			}else if( !str_cmp( arg , "hands" )){
				if( !get_eq_char( vch, WEAR_HANDS )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS );
				}
			}else if( !str_cmp( arg , "hands2" )){
				if( !get_eq_char( vch, WEAR_HANDS_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_HANDS_2 );
				}
			}else if( !str_cmp( arg , "feet" )){
				if( !get_eq_char( vch, WEAR_FEET )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET );
				}
			}else if( !str_cmp( arg , "feet2" )){
				if( !get_eq_char( vch, WEAR_FEET_2 )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_FEET_2 );
				}
			}else if( !str_cmp( arg , "arrow" )){
				if( !get_eq_char( vch, WEAR_ARROW )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_ARROW );
					if(!(obj->item_type == ITEM_ARROW)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else if( !str_cmp( arg , "ammo" )){ // ¥[¤J¼uÃÄ¸Ë³Æ 2022/04/30
				if( !get_eq_char( vch, WEAR_AMMO )){
					if (vch == ch)
						send_to_char( "§A³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					else
						send_to_char( "¥L³o­Ó³¡¦ì¨S¦³¬ïÀ¹¸Ë³Æ.\n\r", ch );
					return;
				}else{
					obj = get_eq_char( vch, WEAR_AMMO );
					if(!(obj->item_type == ITEM_AMMO)){
						send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
						return;
					}
				}
			}else{
				send_to_char( "³o­Ó¸Ë³Æ¦ì¸mµLªkªþÅ].\n\r", ch );
				return;
			}
		}
		if( vch == NULL ){
			return;
		}
	}
	
	if( obj == NULL ){
		if(isObj)
			send_to_char( "³o¸Ì¨S¦³³o¼ËªF¦è.\n\r", ch );
		if(isChar){
			if(vch == ch)
				send_to_char( "§A¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
			else
				send_to_char( "¥L¨­¤W¨S¦³¸Ë³Æ³o¥óª««~.\n\r", ch );
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
		send_to_char( "³o¥óª««~µLªkªþÅ].\n\r", ch );
		return;
	}

	if ( IS_OBJ_STAT( obj, ITEM_SPELL ) ){
		send_to_char( "³o¥óª««~¤w¸g³QªþÅ]¤F.\n\r", ch );
		return;
	}

	if ( !IS_NPC(ch) && obj->level > ch->level + sklv ){
		send_to_char( "§AµLªk¬°§ó°ª¯ÅªºªþÅ].\n\r", ch);
		return;
	}

	// ª««~ÄÝ©Ê­­¨î 2022/12/10
	if( !EncWpAffCheck( obj , 8 ) ){
		send_to_char( "ª««~¨ã¦³ÄÝ©Ê¡A§AµLªkªþ¥[¨ä¥LÄÝ©Ê¡C\n\r", ch);
		return;
	}

	// ¼uÃÄ¦³¸û°ªªºªþÅ]®É¶¡ 2022/05/01
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
			act( ""HIB"§A¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIB"¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( ""NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( ""NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}
	}
	if( isChar ){
		if( vch == ch ){
			act( ""HIB"§A¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"" , ch, obj, NULL, TO_CHAR);
			act( "$n"HIB"¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"",   ch, obj, NULL, TO_ROOM );
		}else{
			act( "$N"HIB"¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"" , ch, obj, vch, TO_CHAR);
			act( ""HIB"§A¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"" , ch, obj, vch, TO_VICT);
			act( "$N"HIB"¨­¤Wªº"NOR"$p"HIB"³Q¤@¹D³±¼vÅ¢¸n³vº¥Åf²H¡A§l¦¬µÛ©P³òªº¥ú¨~¡C"NOR"", ch, obj, vch, TO_NOTVICT );
		}
	}

	act( "¦nªº.",   ch, obj, NULL, TO_CHAR );
    return;
}