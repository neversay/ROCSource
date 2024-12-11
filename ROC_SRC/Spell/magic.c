/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael    *
 *  Chastain, Michael Quan, and Mitchell Tse.          *
 *                     *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.         *
 *                     *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and    *
 *  Mitchell Tse.                *
 *                     *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.     *
 *                     *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in    *
 *  'license.txt', as well as the Envy license in 'license.nvy'.     *
 *  In particular, you may not remove either of these copyright notices.   *
 *                     *
 *  Much time and thought has gone into this software and you are    *
 *  benefitting.  We hope that you share your changes too.  What goes    *
 *  around, comes around.              *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : magic.c                                        *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940612 jye                                    *
//*  PURPOSE     :         修改magic_missile死後還會追打之bug     *
//*              :             增加chk_room判斷被打者是否死亡     *
//*              :         修改delayed_do_cast多重施法死後        *
//*              :             還會追打之bug                      *
//*              :             增加chk_room判斷被打者是否死亡     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940615 jye                                    *
//*  PURPOSE     :         修改被paralyzed後不能施法              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940710 jye                                    *
//*  PURPOSE     :         修改spell_change_sex ppl不能使用       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940908 Razgriz                                *
//*  PURPOSE     :         修改 spell_armageddon, 在 mob 無失明時 *
//*              :         玩家才會進入睡眠狀態, 假如 mob 失明時, *
//*              :         則強制玩家與 mob 進入戰鬥,             *
//*              :         並加入 stun 時間 16 秒左右.            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940909 Razgriz                                *
//*  PURPOSE     :         修改 spell_earthquake, 加入 qmark      *
//*              :         earthquake_stun, 解決無限 stun 的 bug  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940920 jye                                    *
//*  PURPOSE     :         修改protect target 可擋法術            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940924 jye                                    *
//*  PURPOSE     :         修改do_cast 有evacuate QMARK           *
//*              :         一段時間無法施法                       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941006 Razgriz                                *
//*  PURPOSE     :         修改 spell_earthquake, 增加 tardy 判斷 *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950225 jye                                    *
//*  PURPOSE     :         修改NPC被Pro成功的几率                 *      
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950310 jye                                    *
//*  PURPOSE     :         修改NPC被變族不脫EQ                    *  
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950508 jye                                    *
//*  PURPOSE     :         修改ay_spell依sklv變色                 *
//*****************************************************************
//*  MODIFIED BY : 0950916 keelar                                 *
//*  PURPOSE     :         提升法術威力倍數的隱含上限             *
//*****************************************************************
//*  MODIFIED BY : 0991224 tale                                 *
//*  PURPOSE     :         修改 mystic_mirror 為不能反彈吐息             *
//*****************************************************************
/* $Id: magic.c 1745 2005-06-18 16:52:48Z jye $ */
#include "skill.h"

extern int  get_ppl_race_bit(int nRace);

/*
 * Local functions.
 */
void say_spell  args( ( CHAR_DATA *ch, int sn ) );
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim );
void fall_effect( CHAR_DATA *ch );
extern void spell_flaming_armor_strike( int sn, int level, CHAR_DATA *ch, void *vo );
/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
	int sn;
	char *tmp1,*tmp2, arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];

	tmp1 = one_argument((char*)name, arg1);
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
		if ( !skill_table[sn].name )
			break;
		if ( LOWER( name[0] ) == LOWER( skill_table[sn].name[0] )
			&& !str_prefix( name, skill_table[sn].name ) )
			return sn;

		if ( LOWER( arg1[0] ) == LOWER( skill_table[sn].name[0] )
			&& !str_prefix( arg1, skill_table[sn].name ) )
		{
			tmp2 = one_argument(skill_table[sn].name, arg2);
			tmp2 = one_argument(tmp2, arg2);
			tmp1 = one_argument(tmp1, arg1);
			while( arg2[0] != '\0' && arg1[0] != '\0')
			{
				if ( LOWER( arg1[0] ) != LOWER( arg2[0] )
					|| str_prefix( arg1, arg2 ) )
				{
					break;
				}
				tmp2 = one_argument(tmp2, arg2);
				tmp1 = one_argument(tmp1, arg1);
			}
			if( arg2[0] == '\0' && arg1[0] == '\0' )
				return sn;
			else
				tmp1 = one_argument((char*)name, arg1);
		}
	}


	return -1;
}



/*
 * Utter mystical words for an sn.
 */
//20060508 modify by jye
void say_spell( CHAR_DATA *ch, int sn )
{
	CHAR_DATA *rch;
	char    *pName;
	char     buf       [ BUF_STRING_LENGTH ];
	char     buf2      [ BUF_STRING_LENGTH ];
	char     buf3      [ BUF_STRING_LENGTH ];
	int    iSyl;
	int    length;
	int sklv = get_skill_level(ch, sn);

	struct syl_type
	{
		char *     old;
		char *     new;
	};

	static const struct syl_type   syl_table [ ] =
	{
	{ " ",    " "   },
	{ "ar",   "abra"    },
	{ "au",   "kada"    },
	{ "bless",  "fido"    },
	{ "blind",  "nose"    },
	{ "bur",  "mosa"    },
	{ "cu",   "judi"    },
	{ "de",   "oculo"   },
	{ "en",   "unso"    },
	{ "light",  "dies"    },
	{ "lo",   "hi"    },
	{ "mor",  "zak"   },
	{ "move", "sido"    },
	{ "ness", "lacri"   },
	{ "ning", "illa"    },
	{ "per",  "duda"    },
	{ "ra",   "gru"   },
	{ "re",   "candus"  },
	{ "son",  "sabru"   },
	{ "tect", "infra"   },
	{ "tri",  "cula"    },
	{ "ven",  "nofo"    },
	{ "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
	};

	buf[0]  = '\0';
	for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
	{
		for ( iSyl = 0;( length = strlen( syl_table[iSyl].old ) ) != 0; iSyl++ )
		{
			if ( !str_prefix( syl_table[iSyl].old, pName ) )
			{
				strcat( buf, syl_table[iSyl].new );
				break;
			}
		}

		if ( length == 0 )
			length = 1;
	}

	/*
	  if ( !IS_IMMORTAL( ch ) && skill_table[sn].name == "change sex" )
		return;
	*/

	//20060508 modify by jye
	//依sklv變色
	if (sklv == 5) 
	{
		sprintf( buf2, "$n口中喃喃念著咒文：「"HIC"%s"NOR"」", buf );
		sprintf( buf,  "$n口中喃喃念著咒文：「"HIC"%s"NOR"」", skill_table[sn].name );
		sprintf( buf3,  "你口中喃喃念著咒文：「"HIC"%s"NOR"」", skill_table[sn].name );
	}
	else if (sklv == 6)
	{
		sprintf( buf2, "$n口中喃喃念著咒文：「"HIW"%s"NOR"」", buf );
		sprintf( buf,  "$n口中喃喃念著咒文：「"HIW"%s"NOR"」", skill_table[sn].name );
		sprintf( buf3,  "你口中喃喃念著咒文：「"HIW"%s"NOR"」", skill_table[sn].name );
	}
	else
	{
		sprintf( buf2, "$n口中喃喃念著咒文：「%s」", buf );
		sprintf( buf,  "$n口中喃喃念著咒文：「%s」", skill_table[sn].name );
		sprintf( buf3,  "你口中喃喃念著咒文：「%s」", skill_table[sn].name );
	}

	for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	{
		if ( rch != ch )
			act( (ch->class==rch->class || IS_IMMORTAL(rch)) ? buf : buf2, ch, NULL, rch, TO_VICT );
		else
			act( buf3, ch, NULL, rch, TO_CHAR);
	}

	return;
}

/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.'
 */
int saves_spell( int pp, CHAR_DATA *victim )
{
	int save;

	save = GET_SVS( victim ) * 2;
	save = 100 + URANGE( -250, save, 400 ) + pp*3;
	return URANGE(15,save,500);
}


int new_saves_spell( int pp, CHAR_DATA *victim, int type )
{
	int save;
	int nSvs = NEW_GET_SVS( victim, type );

	if(nSvs < 0){
		if(IS_NPC(victim)){
			save = (100 + pp * 4) * 2500 / (2500 + nSvs * nSvs);
		}else{
			save = (100 + pp * 4) * 1000 / (1000 + nSvs * nSvs);
		}
	}else{
		save = (100 + pp * 4) * (100 + nSvs * 2) / 100;
	}

	// save = NEW_GET_SVS( victim, type ) * 2;
	// save = 100 + URANGE( -250, save, 400 ) + pp*3 ;
	return save;
}

//add by coconet to let mag/swd's skill use this old function to count pp's effect
//2004/07/18
int old_saves_spell( int pp, CHAR_DATA *victim, int type )
{
	int save;
	int nSvs = NEW_GET_SVS( victim, type );

	if(nSvs < 0){
		if(IS_NPC(victim)){
			save = (100 + pp * 3 / 2) * 2000 / (2000 + nSvs * nSvs);
		}else{
			save = (100 + pp * 3 / 2) * 1000 / (1000 + nSvs * nSvs);
		}
	}else{
		save = (100 + pp * 3 / 2) * (100 + nSvs * 2) / 100;
	}
	// save = NEW_GET_SVS( victim, type ) * 2;
	// save = 100 + URANGE( -250, save, 400 ) + pp;
	return save;
}

//add by whisperwind to let spell timer effect by svs. 9.24.2004
int new_saves_timer( int pp, CHAR_DATA *victim, int type )
{
	int save;

	save = NEW_GET_SVS( victim, type );
	save = 120 + URANGE( -120, save, 200 );
	return save;
}

// add by whisperwind 2.17.2005
double int_effect( CHAR_DATA* ch )
{
	double effect;
	if ( get_curr_int(ch) > 16 )
		effect = (double)( 200 + ( get_curr_int(ch) - 16) * ( get_curr_int(ch) - 16 )) / 200;
	else
		effect = (double)( 200 - ( 16 - get_curr_int(ch)) * ( 16 - get_curr_int(ch) )) / 200;
	return effect;
}

// add by whisperwind 2.17.2005
double wis_effect( CHAR_DATA* ch )
{
	double effect;
	if ( get_curr_wis(ch) > 16 )
		effect = (double)( 200 + ( get_curr_wis(ch) - 16) * ( get_curr_wis(ch) - 16 )) / 200;
	else
		effect = (double)( 200 + ( 16 - get_curr_wis(ch)) * ( 16 - get_curr_wis(ch) )) / 200;
	return effect;
}
/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

// delayed_do_cast modified by jye 6.15.2005
void delayed_do_cast( CHAR_DATA *ch, char *argument )
{
	void      *vo;
	OBJ_DATA  *obj;
	CHAR_DATA *victim;//, *gch;
	CHAR_DATA *tmp_victim;
	int flag = 0;
	QMARK   * qm;
	ROOM_INDEX_DATA *	chk_room;
	char       arg1 [ MAX_INPUT_LENGTH ];
	char       arg2 [ MAX_INPUT_LENGTH ];
	int        mana;
	int        sn;
	int   times = 1;
	int   timesMA = 0;  // mystic armor 扣除發數 2020/10/26
	int   bonus;
	int   i;
	int   delay = 0;
	int trym = 0;
	int success=100;
	int baset = 0;
	int chint = 0;
	int diff=0;
	int IsMirror = 0;

	AFFECT_DATA *temp_paf;

	if( ch->deleted || !ch->in_room ) 
		return;

	if( !(qm = is_qmark(ch, "delayed_cast")) || qm->v0 == 0 )
	{
		send_to_char("什麼？\n\r", ch);
		return;
	}

	// 從判定 gsn 修改為 判定是否有 aff_paralyzed 2022/06/14
	//if( is_affected( ch, gsn_paralyze ) )
	if( IS_AFFECTED( ch, AFF_PARALYZED ) )
	{  
		send_to_char("你痲痺了.\n\r", ch);
		return;
	}
	qm->v0 -= 1;

	target_name = one_argument( argument, arg1 );
	if( is_number( arg1 ) )
	{
		times = abs(atoi( arg1 ));

		times = UMIN(times, 100);
		target_name = one_argument( target_name, arg1 );
		if( !IS_NPC( ch ) && ( number_percent() > get_skill_percent(ch, gsn_multicasting) || times < 1 ) )
		{
			send_to_char( "你的多重施法失敗了！\n\r", ch );
			return; 
		}
	}
	else 
		times = 1;
     
	one_argument( target_name, arg2 );

	sn = skill_lookup( arg1 ) ;

	if( times > 1 ) 
		bonus = 2;
	else 
		bonus = 1;
      
	times = UMIN(30, times);

	if( !IS_NPC( ch ) && number_percent() < get_skill_percent(ch, gsn_spellmaster) )
	{
		int sklv = get_skill_level(ch, gsn_spellmaster);
		mana = MANA_COST( ch, sn ) * ( times * times ) / bonus;
		//    73% 67% 60% 53% 47% 40% 33%
		mana = mana - (( mana * (sklv * sklv / 2 + sklv * 2 + 10)) / 50 );
		mana = UMAX(1, mana);
	}
	else
		mana = MANA_COST( ch, sn ) * ( times * times ) / bonus;


	if( !IS_NPC(ch) ) 
	{
		if( ch->max_mana + ( ch->max_mana / 5 ) < mana && !IS_NPC(ch))
		{
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}


		while( ch->mana < mana && !IS_NPC(ch))
		{
			if( ch->pcdata->autocombat == AUTOC_POWER ) 
			{
				if( do_autopower(ch, "gem")) continue;
			}
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}

		trym = MANA_COST(ch, sn);
		if( ch->pcdata->autocombat == AUTOC_INSTANT ) 
		{
			int nICSklv = get_skill_level(ch, gsn_instant_cast);
			trym = trym * (30 - nICSklv * 2.5) / 10;
			mana = mana * (30 - nICSklv * 2.5) / 10;
		}
		if(( ch->mana < mana) ||( mana < 1 && trym > 0 ))
		{
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}

		if( trym > 0 && trym * times > trym * times * times)
		{
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}
	}

   /*
	* Locate targets.
	*/
	victim  = NULL;
	vo    = NULL;
	tmp_victim = NULL;
  
  switch( skill_table[sn].target )
  {
    default:
      bug( "Do_cast: bad target for sn %d.", sn );
      return;
    
    case TAR_ARG:
      vo = (char*)arg2;
      break;
    
    case TAR_IGNORE:
      break;
    
    case TAR_CHAR_OFFENSIVE:
      if( ch->in_room && IS_SET(ch->in_room->room_flags, ROOM_SAFE))
      {
          send_to_char("不行.\n\r", ch);
          return ;
      }
      
      if( arg2[0] == '\0' )
      {
          if( !( victim = ch->fighting ) )
          {
              send_to_char( "你要把法術施在誰的身上?\n\r", ch );
              return;
          }
          
          if( !can_see( ch, victim ) && number_bits(1) == 1 )
          {
              send_to_char( "你的法術落空了！\n\r", ch );
              return;
          }
      }
      else
      {
          if( !( victim = get_char_room( ch, arg2 ) ) )
          {
              send_to_char( "喔喔, 你的目標現在不在這裡喔.\n\r", ch );
              return;
          }
      }
      
      if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
      {
          send_to_char( "你不能對你的同伴這樣做!\n\r",ch );
          return;
      }
      
      if( is_safe( ch, victim ) )
          return;
      
       //Cast目標有被pro
	   //修改NPC被Pro成功的機率
       if( (tmp_victim = victim->protector) )
       {
           if( IS_NPC(victim)) //NPC
           {
               //是否Sleep
               if( IS_AFFECTED( tmp_victim, AFF_SLEEP ))
                   flag = 1;

               //是否在同一個room
               if( !( tmp_victim = get_char_room( victim, tmp_victim->name ) ) )
                   flag = 1;
               if( flag == 0 && 
                   UMAX( victim->level/2, number_percent() ) > 
                   number_percent())
                   victim = tmp_victim;
           }
           else                 //PC
           {
               //ch是否看的到tmp_victim
               if( !can_see( ch, tmp_victim ) && number_bits(1) == 1 )
                   flag = 1;

               //ch跟tmp_victim是否在同一個room   
               if( !( tmp_victim = get_char_room( ch, tmp_victim->name ) ) )    
                   flag = 1;

               //can cast tmp_victim
               if( flag == 0 && get_skill_level(tmp_victim,skill_lookup("protect target")) > 4)
                   victim = tmp_victim;
           } 
       }
       
      check_killer( ch, victim );
      
      vo = (void *) victim;
      break;
    
    case TAR_CHAR_DEFENSIVE:
      if( arg2[0] == '\0' )
      {
          victim = ch;
      }
      else
      {
          if( !( victim = get_char_room( ch, arg2 ) ) )
          {
              send_to_char( "沒有你要的東西.\n\r", ch );
              return;
          }
      }
      
      vo = (void *) victim;
      break;
      
    case TAR_CHAR_SELF:
      if( arg2[0] != '\0' && !is_pfxname( arg2, ch->name ) )
      {
          send_to_char( "你不能對其他人施用這個魔法!\n\r", ch );
          return;
      }
      
      vo = (void *) ch;
      break;
    
    case TAR_OBJ:
      if( arg2[0] == '\0' )
      {
          send_to_char( "你要把法術施在什麼東西上？\n\r", ch );
          return;
      }
      if( !( obj = get_obj_here( ch, arg2 ) ) )
      {
          send_to_char( "這裡沒有這樣東西.\n\r", ch );
          return;
      }
      
      vo = (void *) obj;
      break;
    case TAR_OBJ_INV:
      if( arg2[0] == '\0' )
      {
          send_to_char( "你要把法術施在什麼東西上？\n\r", ch );
          return;
      }
      
      if( !( obj = get_obj_carry( ch, arg2 ) ) )
      {
          send_to_char( "你身上沒有這樣東西.\n\r", ch );
          return;
      }
      
      vo = (void *) obj;
      break;
  }

  if( IS_NPC(ch) || ch->pcdata->autocombat != AUTOC_INSTANT ) 
  {
      delay  = UMAX( 2, skill_table[sn].beats - UMIN( 0, ch->played * 0.00001 ) );
      WAIT_MAGIC_STATE( ch, (delay) );
  }
  else
  {
      //modified by coco for post stun when autoc instant
      int nICSklv = get_skill_level(ch, gsn_instant_cast);
      delay  = UMAX( 2, skill_table[sn].beats - UMIN( 0, ch->played * 0.00001 ) );
      WAIT_MAGIC_STATE( ch, 11 - nICSklv + delay );
  }

	if( times > 1 ) 
		WAIT_MAGIC_STATE( ch, times * (6 - get_skill_level(ch, gsn_spellcraft)) );
      
	if( !IS_NPC( ch ) && number_percent( ) > 10 + get_skill_percent(ch, sn) + 5 * get_skill_level(ch, sn))
	{
		send_to_char( "你沒有辦法集中精神.\n\r", ch );
		ch->mana -= mana / 2;
	}
	else
	{
		if( !IS_NPC(ch))
			ch->mana -= mana;
      
		if( victim )
		{
			if( victim->deleted ) 
				return;

			if( (IS_SET( victim->special, REF_MAGIC) )
				&& skill_table[sn].target == TAR_CHAR_OFFENSIVE 
				&& sn != gsn_wizard_rage
				&& sn != gsn_crushing_hand
				&& sn != gsn_ice_comet
				&& sn != gsn_fire_breath
				&& sn != gsn_acid_breath
				&& sn != gsn_frost_breath
				&& sn != gsn_gas_breath // gas breath 轉換範圍 2022/12/23
				&& sn != gsn_lightning_breath)
			{
				vo = ch;
				ch = victim;
				victim = vo;
			}
			else if( IS_AFFECTED(victim, AFF_MYSTIC_MIRROR) 
					&& skill_table[sn].target == TAR_CHAR_OFFENSIVE
					&& sn != gsn_wizard_rage 
					&& sn != gsn_crushing_hand
					&& sn != gsn_ice_comet
					&& sn != gsn_high_explosive
					&& sn != gsn_fire_breath
					&& sn != gsn_acid_breath
					&& sn != gsn_frost_breath
					&& sn != gsn_lightning_breath
					&& sn != gsn_gas_breath // gas breath 轉換範圍 2022/12/23
					&& sn != gsn_magic_missile)
			{

				int nMirSklv = get_skill_level(victim, gsn_mystic_mirror);
				if( nMirSklv > 2 ) {
					victim->mana -= UMAX( mana / ((nMirSklv - 1) * nMirSklv / 3), 1);
				}else{
					victim->mana -= UMAX(mana * (5 - nMirSklv) / 7, 1);
				}
				if( victim->mana < 1 && !IS_NPC(victim))
				{
					send_to_char("你的魔鏡無法得到充分的魔力而消散！\n\r",victim);

					for( temp_paf = victim->affected; temp_paf; temp_paf = temp_paf->next )
					{
						if( temp_paf->type == gsn_mystic_mirror )
							affect_remove( victim, temp_paf );
					}
				}else if(mana > 900 + ((nMirSklv * nMirSklv * nMirSklv) / 10) * 100){
					act( "你的魔法能量突破了$N的魔力之鏡！", ch, NULL, victim, TO_CHAR );
					act( "$n 的魔法能量突破了你的魔力之鏡！", ch, NULL, victim, TO_VICT );
					act( "$n 的魔法能量突破了$N的魔力之鏡！", ch, NULL, victim, TO_NOTVICT );
				}else{
						vo = ch;
						ch = victim;
						victim = vo;
						IsMirror = nMirSklv;
				}
			}
			//else if(ch->class != WIZARD
			//		&& IS_SET( victim->special, ANTI_MAGIC ) )
			// 魔免 mob 可以對自己施法 2020/10/29
			else if( IS_SET( victim->special, ANTI_MAGIC ) && ch != victim && ch->class != WIZARD )
			{
				act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
				return;
			}
			/*  mystic armor 拆分到三處  2020/10/26
					判定多重施法無傷害的負面法術
					magic_damage 及 magic__damage , 判定傷害魔法
					obj_cast_spell 判定無傷害的負面法術

			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
				&& sn != gsn_wizard_rage 
				&& sn != gsn_fire_breath 
				&& sn != gsn_acid_breath
				&& sn != gsn_frost_breath
				&& sn != gsn_lightning_breath
				&& sn != gsn_will_fortress
				&& sn != gsn_salvation
				&& sn != gsn_song_of_clenase
				&& sn != gsn_crushing_hand )
			{
				if( get_skill_level(victim, gsn_mystic_armor > 2 ))
					victim->mana += (mana * get_skill_level(victim, gsn_mystic_armor)) * 0.1;
                  
				victim->mana = UMAX( 0, victim->mana);
				act( "$n 的魔力障壁吸收了魔法！", victim, NULL, NULL, TO_ROOM );
				if( affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) > 1 ){
				  
					AFFECT_DATA af;
					af.type  = gsn_mystic_armor;
					af.duration = 0;
					af.location = APPLY_MAGIC_IMMUNE;
					af.modifier = -1;
					af.bitvector = AFF_MYSTIC_ARMOR;
					affect_join( victim, &af );
					  
					send_to_char( "你的魔力障壁吸收了魔法！\n\r", victim );
					return;
				}else{
					affect_strip(victim, gsn_mystic_armor);
					REMOVE_BIT(victim->affected_by, AFF_MYSTIC_ARMOR);
					send_to_char( "你的魔力障壁吸收魔法後破碎了！\n\r", victim );
					return;
				}
			}
			*/

			// 無dam debuff 法術每次 mutling cast 成功後, 判定 mystic armor 2020/10/26
			for( i = 0; i < times; i++){
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
					&& (   sn == gsn_mute
						|| sn == skill_lookup( "remove curse" )
						|| sn == skill_lookup( "dispel magic" )
						|| sn == gsn_seal_magic 
						|| sn == gsn_charm_person
						|| sn == gsn_blindness
						|| sn == gsn_fear
						|| sn == gsn_paralyze
						|| sn == gsn_disease
						|| sn == gsn_poison
						|| sn == gsn_faerie_fire
						|| sn == gsn_tardy
						|| sn == gsn_sleep
						|| sn == gsn_weaken
						|| sn == gsn_curse
						|| sn == gsn_polymorph
						|| sn == gsn_crushing_hand
						|| sn == gsn_soulsteal ) )
				{
					timesMA += 1;
					
					int MALv = affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);
					if( sn == skill_lookup( "dispel magic" ) )
					{
						if( !IS_NPC(victim) )
							MALv -= 300 * (1 + get_skill_level(ch , sn) );
						else
							MALv -= 3000 * (1 + get_skill_level(ch , sn) );
					}
					else if( sn == gsn_crushing_hand )
					{
						act("$n釋放出強大的魔法能量，嘗試粉碎$N身上的魔力障壁！" , ch, NULL, victim, TO_NOTVICT);
						act("$n釋放出強大的魔法能量，嘗試粉碎你身上的魔力障壁！" , ch, NULL, victim, TO_VICT);
						act("你釋放出強大的魔法能量，嘗試粉碎$N身上的魔力障壁！" , ch, NULL, victim, TO_CHAR);
						if( !IS_NPC(victim) )
							MALv -= 600 * (1 + get_skill_level(ch , sn) );
						else
							MALv -= 10000 * (1 + get_skill_level(ch , sn) );
					}
					else
					{
						if( !IS_NPC(victim) )
							MALv -= 50 * (1 + get_skill_level(ch , sn) );
						else
							MALv -= 500 * (1 + get_skill_level(ch , sn) );
					}
					if( MALv > 0 ){
						
						if( get_skill_level(victim, gsn_mystic_armor > 2 ))
							victim->mana += (affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) - MALv) /50;
						victim->mana = UMIN( victim->max_mana , victim->mana);

						AFFECT_DATA af;
						af.type  = gsn_mystic_armor;
						af.duration = 0;
						af.location = APPLY_MAGIC_IMMUNE;
						af.modifier = MALv - affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);
						af.bitvector = AFF_MYSTIC_ARMOR;
						affect_join( victim, &af );

						act( "$n 的魔力障壁吸收了魔法！", victim, NULL, NULL, TO_ROOM );
						send_to_char( "你的魔力障壁吸收了魔法！\n\r", victim );
						continue;
					}else{

						if( get_skill_level(victim, gsn_mystic_armor > 2 ))
							victim->mana += affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) /100;
						victim->mana = UMIN( victim->max_mana , victim->mana);

						affect_strip(victim, gsn_mystic_armor);
						REMOVE_BIT(victim->affected_by, AFF_MYSTIC_ARMOR);

						act( "$n 的魔力障壁吸收魔法後破碎了！", victim, NULL, NULL, TO_ROOM );
						send_to_char( "你的魔力障壁吸收魔法後破碎了！\n\r", victim );
						continue;
					}
				}else{
					break;
				}
			}
		}
		// 扣除 mystic armor 耗損發數 2020/10/26
		times -= timesMA;

		if( times < 1 ) return;

		int nMultiSklv = get_skill_level(ch, gsn_multicasting);
		chint = get_curr_int(ch);
		//if( times > 1 )
		// 成功率受原發數影響 2020/10/26
		if( times + timesMA > 1 )
		{
			chint = get_curr_int(ch);
			if(IsMirror == 0){
				if(!IS_NPC(ch))
					success = chint + get_skill_percent(ch, sn) + nMultiSklv * 10;
				else
					success = chint + 100 + nMultiSklv * 10;
			}else{
				nMultiSklv = IsMirror;
				success = chint * 2 + IsMirror * (IsMirror + 4) * 3 / 2;
			}
			diff = 0;
		}
		else 
		{
			success = 1000;
			diff = 0;
			baset = 100;
		}
      
		if( times + timesMA < baset ) 
			diff = 0;
          
		chk_room = ch->in_room;
      
		for( i = 0; i < times; i++)
		{
			if( number_percent() < success )
			{
				(*skill_table[sn].spell_fun) ( sn,URANGE( 1, ch->level, L_APP - 1 ),ch, vo );
				success = success * (80 + nMultiSklv * 3) / 100;
			}
			else 
			{
				send_to_char("你的精神無法承受連續的施法。\n\r", ch);
				continue;
			}
           
			if( victim
				&& ( victim->position == POS_DEAD 
					|| victim->in_room != ch->in_room 
					|| victim->in_room != chk_room ) )
			{
				return;
			}
           
			if( !IS_NPC( ch ) && ch->position < skill_table[sn].minimum_position )
				return;
		}
  }

  if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
       && victim->master != ch 
       && victim != ch 
       && IS_AWAKE( victim ) )
  {
       CHAR_DATA *vch;
       
       for( vch = ch->in_room->people; vch; vch = vch->next_in_room )
       {
            if( vch->deleted )
                continue;
            if( victim == vch && !victim->fighting )
            {
                multi_hit( victim, ch, TYPE_UNDEFINED );
                break;
            }
       }
  }

  return;
}

void do_cast( CHAR_DATA *ch, char *argument )
{
	void      *vo;
	OBJ_DATA  *obj;
	QMARK *qm;
	CHAR_DATA *victim, *gch, *tmp_victim;
	int flag = 0;
	CALL_OUT_DATA *cod;
	char       arg1 [ MAX_INPUT_LENGTH ];
	char       arg2 [ MAX_INPUT_LENGTH ];
	char * argback = argument;
	int        mana;
	int        sn;
	int   times = 1;
	int   bonus;
	//int   i;
	double   delay = 0;
	int trym ;
	QMARK *qmark = NULL;

	if( ch->deleted ) return;

	// 從判定 gsn_paralyze 修改為判定 aff_paralyzed 2022/06/14
	//if( is_affected( ch, gsn_paralyze ) )
	if( IS_AFFECTED( ch, AFF_PARALYZED ) ){
		send_to_char( "你痲痺了.\n\r", ch );
		return;
	}
      
	if( (qmark = is_qmark( ch, "cast_evacuate_mark" ))!= NULL ) 
	{
		send_to_char( "你目前處於虛脫狀態無法施法！\n\r", ch );
		return;
	}

	target_name = one_argument( argument, arg1 );
	if ( is_number( arg1 ) )
	{
		times = abs(atoi( arg1 ));
		target_name = one_argument( target_name, arg1 );
		if( !IS_IMMORTAL(ch) )
			times = UMIN(times, 100);
		if ( !IS_NPC( ch )
			&& ( number_percent() > get_skill_percent(ch, gsn_multicasting) || times < 1 ) )
			//( number_percent() > ch->pcdata->learned[gsn_multicasting] || times < 1 )
		{
			send_to_char( "你的多重施法失敗了！\n\r", ch );
			return; 
		}
	}
	else times = 1;

	one_argument( target_name, arg2 );

	if ( arg1[0] == '\0' )
	{
		send_to_char( "你要施什麼法術啊?\n\r", ch );
		return;
	}

	//if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) || ch->clan ) )
	if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) ) )
		return;

	/*
		if(ch->ordered)
			act( "$n is ordered", ch, NULL, NULL, TO_ROOM );
		else
			act( "$n is not ordered", ch, NULL, NULL, TO_ROOM );
		if ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_SUMMONED ) && ( ch->ordered == TRUE ) )
		{
			ch->ordered = FALSE;
			return;
		}
	*/

	// 避免 sn = 0 時跑到 get_skill_percent 產生 bug log 2021/12/20
	if( ( sn = skill_lookup( arg1 ) ) == 0 ){
		send_to_char( "你還沒學會這個法術.\n\r", ch );
		return;
	}

	if ( ( sn = skill_lookup( arg1 ) ) < 0
		|| ( !IS_NPC( ch ) && get_skill_percent(ch, sn) == 0 ))
							//ch->pcdata->learned[sn] == 0 ) )
	{
		send_to_char( "你還沒學會這個法術.\n\r", ch );
		return;
	}

	if (IsSorGateMobByChar(ch) && ch->ordered == TRUE) {
		int nMobIndex = ch->pIndexData->vnum;
		bool hasLearned = FALSE;
		ch->ordered = FALSE;
		if (nMobIndex == MOB_VNUM_SOR_WATER2){
			if (   sn == gsn_mass_heal || sn == gsn_sanctuary || sn == gsn_bless){
				hasLearned = TRUE;
				ch->wait += 20;
			}
			if (sn == gsn_divine_light){
				if (ch->master && get_skill_level(ch->master, gsn_water_elemental) >= 6){
					hasLearned = TRUE;
					ch->wait += 25;
				}
			}
		}
		if (hasLearned == FALSE) {
			send_to_char("它還沒學會這個法術.\n\r", ch->master);
			return;
		}
	}

	if ( !IS_NPC( ch ) && ch->position < skill_table[sn].minimum_position )
	{
		if ( ch->fighting ){
			for ( gch = ch->in_room->people; gch; gch = gch->next_in_room ){
				if ( gch->fighting == ch )
				{
					if ( ch->pcdata->autocombat == AUTOC_CONCENTRATION
						&& get_skill_percent(ch, gsn_concentration )
						+ get_skill_level(ch, gsn_concentration) * 2 < number_percent() / 2 + 5 )
					{
						send_to_char( "你無法專心.\n\r", ch );
						return;
					}
					else if ( ch->pcdata->autocombat != AUTOC_CONCENTRATION
							&& get_skill_percent(ch, gsn_concentration)
							+ get_skill_level(ch, gsn_concentration) * 3 < number_percent() + 30 )
					{
						send_to_char( "你無法專心.\n\r", ch );
						return;
					}
				}
			}
		}
		else
		{
			send_to_char( "你無力施法.\n\r", ch );
			return;
		}
	}


	if( IS_AFFECTED( ch, AFF_SEAL_MAGIC ) && ! IS_IMMORTAL(ch) ) {
		send_to_char("你周圍的力場阻絕了你的魔法！\n\r", ch);
		return;
	}

	if (   IS_AFFECTED( ch, AFF_MUTE ) || IS_SET( race_table[ch->race].race_abilities, RACE_MUTE ) ){
		send_to_char( "你的嘴唇在動, 但是卻發不出聲音!\n\r", ch );
		return;
    }

	if ( IS_SET( ch->in_room->room_flags, ROOM_CONE_OF_SILENCE ) ){
		send_to_char( "你正處在一個不准施法的地方!\n\r", ch );
		return;
	}

	if ( times > 1 ) bonus = 2;
	else bonus = 1;
	times = UMIN(30, times);
	if ( !IS_NPC( ch )
		&& number_percent() < get_skill_percent(ch, gsn_spellmaster)){
		//ch->pcdata->learned[gsn_spellmaster] ){
		int sklv = get_skill_level(ch, gsn_spellmaster);
		mana = MANA_COST( ch, sn ) * ( times * times ) / bonus;
		mana = mana - (( mana * (sklv*2+8)) / 30 );
		mana = UMAX(1, mana);
	}
	else
		mana = MANA_COST( ch, sn ) * ( times * times ) / bonus;

	if( !IS_NPC(ch) ) {
		trym = MANA_COST(ch, sn);
		if( ch->pcdata->autocombat == AUTOC_INSTANT ) {
			int nICSklv = get_skill_level(ch, gsn_instant_cast);
			trym = trym * (30 - nICSklv * 2.5) / 10;
			mana = mana * (30 - nICSklv * 2.5) / 10;
		}
		if( ch->max_mana + ( ch->max_mana / 5 ) < mana && !IS_NPC(ch)){
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}

		while ( ch->mana < mana && !IS_NPC(ch))
		{
			if( ch->pcdata->autocombat == AUTOC_POWER )
			{
				if( do_autopower(ch, "gem")) continue;
			}
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}

		if(( ch->mana < mana) ||( mana < 1 && trym > 0 )){
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}

		if( trym > 0 && trym * times > trym * times * times){
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}
	}

    /*
     * Locate targets.
     */
	victim  = NULL;
	vo    = NULL;
	tmp_victim = NULL;

	//add by coconet 2005/04/11 for fix stun bug when not magic.
	if( skill_table[sn].spell_fun == spell_null )
		return;
      
	switch ( skill_table[sn].target )
	{
		default:
			bug( "Do_cast: bad target for sn %d.", sn );
		return;

		case TAR_ARG:
			vo = (char*)arg2;
		break;

		case TAR_IGNORE:
		break;

		case TAR_CHAR_OFFENSIVE:
			if( ch->in_room && IS_SET(ch->in_room->room_flags, ROOM_SAFE)){
				send_to_char("不行.\n\r", ch);
				return;
			}
			if ( arg2[0] == '\0' ){
				if ( !( victim = ch->fighting ) )
				{
					send_to_char( "你要把法術施在誰的身上?\n\r", ch );
					return;
				}
				if ( !can_see( ch, victim ) && number_bits(1) == 1 )
				{
					send_to_char( "你的法術落空了！\n\r", ch );
					return;
				}
			}else{
				if ( !( victim = get_char_room( ch, arg2 ) ) ){
					send_to_char( "喔喔, 你的目標現在不在這裡喔.\n\r", ch );
					return;
				}
			}
			if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim ){
				send_to_char( "你不能對你的同伴這樣做!\n\r", ch );
				return;
			}
			if ( is_safe( ch, victim ) )
				return;
  
			//Cast目標有被pro
			//修改NPC被Pro成功的機率
			if( (tmp_victim = victim->protector) )
			{
				//NPC
				if( IS_NPC(victim) ){
					//是否Sleep
					if( IS_AFFECTED( tmp_victim, AFF_SLEEP ) )
						flag = 1;
					//是否在同一個room
					if( !( tmp_victim = get_char_room( victim, tmp_victim->name ) ) )
						flag = 1;
					if( flag == 0 && UMAX( victim->level/2, number_percent() ) > number_percent())
						victim = tmp_victim;
				//PC
				}else{
					//ch是否看的到tmp_victim
					if( !can_see( ch, tmp_victim ) && number_bits(1) == 1 )
						flag = 1;

					//ch跟tmp_victim是否在同一個room   
					if( !( tmp_victim = get_char_room( ch, tmp_victim->name ) ) )    
						flag = 1;

					//can cast tmp_victim
					if( flag == 0 && get_skill_level(tmp_victim,skill_lookup("protect target")) > 4)
						victim = tmp_victim;
				}
			}
			check_killer( ch, victim );
			vo = (void *) victim;
		break;

		case TAR_CHAR_DEFENSIVE:
			if ( arg2[0] == '\0' )
			{
				victim = ch;
			}else{
				if ( !( victim = get_char_room( ch, arg2 ) ) )
				{
					send_to_char( "沒有你要的東西.\n\r", ch );
					return;
				}
			}
			vo = (void *) victim;
		break;

		case TAR_CHAR_SELF:
			if ( arg2[0] != '\0' && !is_pfxname( arg2, ch->name ) )
			{
				send_to_char( "你不能對其他人施用這個魔法!\n\r", ch );
				return;
			}
			vo = (void *) ch;
		break;

		case TAR_OBJ:
			if ( arg2[0] == '\0' )
			{
				send_to_char( "你要把法術施在什麼東西上？\n\r", ch );
				return;
			}
			if ( !( obj = get_obj_here( ch, arg2 ) ) )
			{
				send_to_char( "這裡沒有這樣東西.\n\r", ch );
				return;
			}
			vo = (void *) obj;
		break;

		case TAR_OBJ_INV:
			if ( arg2[0] == '\0' )
			{
				send_to_char( "你要把法術施在什麼東西上？\n\r", ch );
				return;
			}
			if ( !( obj = get_obj_carry( ch, arg2 ) ) )
			{
				send_to_char( "你身上沒有這樣東西.\n\r", ch );
				return;
			}
			vo = (void *) obj;
		break;
	}

	/*
		while ( ch->mana < mana && !IS_NPC(ch))
		{
			if( ch->pcdata->autocombat == AUTOC_POWER ) {
				if( do_autopower(ch, "gem"))
					continue;
			}
			send_to_char( "你的法力不夠了.\n\r", ch );
			return;
		}
	*/


	if( skill_table[sn].beats ){
		double dSkillDelay = (double) skill_table[sn].beats;
		if (!IS_NPC(ch)){
			int nSpellCraftSklv = get_skill_level(ch, gsn_spellcraft);
			delay = URANGE(1.0, dSkillDelay / (4 + nSpellCraftSklv), 4.0);
		}else{
			delay = URANGE(1.0, dSkillDelay / 4, 4.0);
		}
	}else{
		delay = 0;
	}

	if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
		say_spell( ch, sn );

	if( (qm = is_qmark(ch, "delayed_cast")))
		qm->v0 += 1;
	else{
		sprintf(arg1, "self delayed_cast -1 1 0 0 1 0");
		do_add_qmark(ch, arg1);
	}
	if( !delay
		|| skill_table[sn].spell_fun == spell_null
		|| (IS_NPC(ch) && qm && qm->v0 > 1 )
		|| (!IS_NPC(ch) && ch->pcdata->autocombat == AUTOC_INSTANT))
	{
		delayed_do_cast(ch, argback);
		return;
	}

	cod = new_call_out();
	cod->owner = ch;
	cod->timer = delay;
	sprintf(arg1, "cast_delay猜猜看 %s", argback);
	cod->cmd = str_dup(arg1);
	add_call_out(cod);
	WAIT_STATE(ch, delay * PULSE_CALL_OUT );
	/*
		// add at 2020/10/26
		if (get_skill_level(ch, sn) > 0){
			WAIT_STATE(ch, delay * PULSE_CALL_OUT / (2 * get_skill_level(ch, sn)) );
		}else{
			WAIT_STATE(ch, delay * PULSE_CALL_OUT );
		}
	*/

    /*
		if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
			&& victim->master != ch && victim != ch && IS_AWAKE( victim ) )
		{
			CHAR_DATA *vch;

			for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
			{
				if ( vch->deleted )
					continue;
				if ( victim == vch && !2 )
				{
					multi_hit( victim, ch, TYPE_UNDEFINED );
					break;
				}
			}
		}
	*/
	return;
}


int weakness_percent( CHAR_DATA *ch, int sn)
{
    MOB_WEAKNESS * weak;
    if( !ch->weakness )
  return 100;
    for( weak = ch->weakness; weak ; weak = weak->next ) {
  if( sn == skill_lookup( weak->name ) )
      break;
    }

    if( ! weak ) return 100;
    else return weak->percent;
}

int weakness_damage( CHAR_DATA *ch, int sn)
{
    MOB_WEAKNESS * weak;
    if( !ch->weakness )
  return 100;
    for( weak = ch->weakness; weak ; weak = weak->next ) {
  if( sn == skill_lookup( weak->name ) )
      break;
    }

    if( ! weak ) return 100;
    else return weak->damage;
}


void room_cast_spell( int sn, int level,  CHAR_DATA *victim, CHAR_DATA *ch, ROOM_INDEX_DATA *obj )
{
	void *vo;

	if ( sn <= 0 )
		return;

	if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
	{
		bug( "Room_cast_spell: bad sn %d.", sn );
		return;
	}

	switch ( skill_table[sn].target )
	{
		default:
			bug( "Room_cast_spell: bad target for sn %d.", sn );
			return;

		case TAR_IGNORE:
			bug( "Room_cast_spell: must have target for sn %d.", sn );
			return;
			break;

		case TAR_CHAR_OFFENSIVE:
			if ( !victim ){
				bug( "Room_cast_spell: must have target for sn %d.", sn );
				return;
			}

			//check_killer( ch, victim );
			level = URANGE( 1, level, 90);

			vo = (void *) victim;
			break;

		case TAR_CHAR_DEFENSIVE:
			if ( !victim ){
				bug( "Room_cast_spell: must have target for sn %d.", sn );
				return;
			}
			vo = (void *) victim;
			break;

		case TAR_CHAR_SELF:
			vo = (void *)victim;
			break;

		case TAR_OBJ_INV:
			bug( "Room_cast_spell: can't cast sn %d.", sn );
			return;
			break;
	}

	target_name = "";
	
	// 把 room cast 加入 sklv 設定 2022/05/08
	if ( ch )
		//(*skill_table[sn].spell_fun) ( sn, level, ch, vo );
		(*skill_table[sn].spell_fun) ( sn, level*1000, ch, vo );
	else
		//(*skill_table[sn].spell_fun) ( sn, level, ch, vo );
		(*skill_table[sn].spell_fun) ( sn, level*1000, vo, vo );

	return;
}
/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;

	// 加入 artifacts mastery 對物品 cast 的影響 2022/04/29
	int ArtiMasSklv = 0;
	ArtiMasSklv += get_skill_level(ch, gsn_artifacts_mastery);
	if( get_skill_level(ch, gsn_artifacts_mastery) > 3 ) ArtiMasSklv += 2;
	if( get_skill_level(ch, gsn_artifacts_mastery) > 4 ) ArtiMasSklv += 3;
	if( get_skill_level(ch, gsn_artifacts_mastery) > 5 ) ArtiMasSklv += 4;
	level = level + ArtiMasSklv;

    if ( sn <= 0 ) return;

    if( ch->deleted ) return;
	
	if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
	{
		bug( "Obj_cast_spell: bad sn %d.", sn );
		return;
	}

	if( IS_AFFECTED( ch, AFF_SEAL_MAGIC ) ) {
		send_to_char("你周圍的力場阻絕了你的魔法！\n\r", ch);
		return;
	}

	switch ( skill_table[sn].target )
	{
		default:
			bug( "Obj_cast_spell: bad target for sn %d.", sn );
			return;

		case TAR_ARG:
			vo = NULL;
		break;

		case TAR_IGNORE:
			//vo = NULL;
			// 針對裝備附魔作目標修正 2021/10/02
			if (   sn == skill_lookup( "magic enchantment" )
				|| sn == skill_lookup( "wind enchantment" )
				|| sn == skill_lookup( "flame enchantment" )
				|| sn == skill_lookup( "water enchantment" )
				|| sn == skill_lookup( "earth enchantment" )
				|| sn == skill_lookup( "lightning enchantment" )
				|| sn == skill_lookup( "saint enchantment" )
				|| sn == skill_lookup( "dark enchantment" ) ){
					if ( !obj )
					{
						send_to_char( "你不能這麼做.\n\r", ch );
						return;
					}
					vo = (void *) obj;
				}else{
					vo = NULL;
				}
		break;

		case TAR_CHAR_OFFENSIVE:
			if ( !victim )
				victim = ch->fighting;
			if ( !victim )
			{
				send_to_char( "你不能這麼做.\n\r", ch );
				return;
			}

			if ( is_safe( ch, victim ) ) return;

			check_killer( ch, victim );
			level = level * level / 40;
			level = URANGE( 1, level, 90);

			vo = (void *) victim;
		break;

		case TAR_CHAR_DEFENSIVE:
			if ( !victim ) victim = ch;
			vo = (void *) victim;
		break;

		case TAR_CHAR_SELF:
			if (   sn == skill_lookup( "teleport" )
				|| sn == skill_lookup( "word of recall" )
				|| sn == skill_lookup( "dispel magic" ) )
			return;
			vo = (void *) ch;
		break;

		case TAR_OBJ_INV:
		if ( !obj )
		{
			send_to_char( "你不能這麼做.\n\r", ch );
			return;
		}
		vo = (void *) obj;
		break;
	}

	if ( sn == skill_lookup( "teleport" ) || sn == skill_lookup( "word of recall" ) )
		return;

	// 無dam debuff 法術每次 mutling cast 成功後, 判定 mystic armor 2020/10/26
	if( !(skill_table[sn].target == TAR_OBJ_INV) ){
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch != victim
			&& ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
			return;
		}
		if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR) && ch != victim
			&& (   sn == gsn_mute
				|| sn == skill_lookup( "remove curse" )
				|| sn == skill_lookup( "dispel magic" )
				|| sn == gsn_seal_magic 
				|| sn == gsn_charm_person
				|| sn == gsn_blindness
				|| sn == gsn_fear
				|| sn == gsn_paralyze
				|| sn == gsn_disease
				|| sn == gsn_poison
				|| sn == gsn_faerie_fire
				|| sn == gsn_tardy
				|| sn == gsn_sleep
				|| sn == gsn_weaken
				|| sn == gsn_curse
				|| sn == gsn_polymorph
				|| sn == gsn_crushing_hand
				|| sn == gsn_soulsteal ) )
		{
			int MALv = affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);
			int obj_sklv = 0;
			if( level >= 10 ) obj_sklv += 1;
			if( level >= 20 ) obj_sklv += 1;
			if( level >= 30 ) obj_sklv += 1;
			if( level >= 40 ) obj_sklv += 1;
			if( level >= 50 ) obj_sklv += 1;
			if( level >= 60 ) obj_sklv += 1;
			if( sn == skill_lookup( "dispel magic" ) )
			{
				if( !IS_NPC(victim) )
					MALv -= 300 * (1 + obj_sklv );
				else
					MALv -= 3000 * (1 + obj_sklv );
			}
			else if( sn == gsn_crushing_hand )
			{
				act("$n釋放出強大的魔法能量，嘗試粉碎$N身上的魔力障壁！" , ch, NULL, victim, TO_NOTVICT);
				act("$n釋放出強大的魔法能量，嘗試粉碎你身上的魔力障壁！" , ch, NULL, victim, TO_VICT);
				act("你釋放出強大的魔法能量，嘗試粉碎$N身上的魔力障壁！" , ch, NULL, victim, TO_CHAR);
				if( !IS_NPC(victim) )
					MALv -= 600 * (1 + obj_sklv );
				else
					MALv -= 10000 * (1 + obj_sklv );
			}
			else
			{
				if( !IS_NPC(victim) )
					MALv -= 50 * (1 + obj_sklv );
				else
					MALv -= 500 * (1 + obj_sklv );
			}
			if( MALv > 0 ){
				
				if( get_skill_level(victim, gsn_mystic_armor > 2 ))
					victim->mana += (affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) - MALv) /50;
				victim->mana = UMIN( victim->max_mana , victim->mana);
				
				AFFECT_DATA af;
				af.type  = gsn_mystic_armor;
				af.duration = 0;
				af.location = APPLY_MAGIC_IMMUNE;
				af.modifier = MALv - affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE);
				af.bitvector = AFF_MYSTIC_ARMOR;
				affect_join( victim, &af );

				act( "$n 的魔力障壁吸收了魔法！", victim, NULL, NULL, TO_ROOM );
				send_to_char( "你的魔力障壁吸收了魔法！\n\r", victim );
				return;
			}else{

				if( get_skill_level(victim, gsn_mystic_armor > 2 ))
					victim->mana += affected_value(victim, gsn_mystic_armor, APPLY_MAGIC_IMMUNE) /100;
				victim->mana = UMIN( victim->max_mana , victim->mana);

				affect_strip(victim, gsn_mystic_armor);
				REMOVE_BIT(victim->affected_by, AFF_MYSTIC_ARMOR);
				act( "$n 的魔力障壁吸收魔法後破碎了！", victim, NULL, NULL, TO_ROOM );
				send_to_char( "你的魔力障壁吸收魔法後破碎了！\n\r", victim );
				return;
			}
		}
	}
	target_name = "";
	// 放大 1000 倍代入 spell 計算 obj hp sklv
	//(*skill_table[sn].spell_fun) ( sn, level, ch, vo );
	// 加入 alc 自產 magic ammo 的判定 2022/04/29
	if( obj && obj->pIndexData->vnum == OBJ_VNUM_MAGIC_AMMO )
		(*skill_table[sn].spell_fun) ( sn, level, ch, vo );
	else
		(*skill_table[sn].spell_fun) ( sn, level*1000, ch, vo );

	if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE && victim->master != ch && ch != victim )
	{
		CHAR_DATA *vch = NULL;
		if( ch->deleted ) return;

		for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		{
			if ( vch->deleted ) continue;

			if ( victim == vch && !victim->fighting )
			{
				multi_hit( victim, ch, TYPE_UNDEFINED );
				break;
			}
		}
	}

	return;
}



/*
 * Spell functions.
 */
 
// acid blast modified by whisperwind 8.23.2004
void ppl_spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

        int sklv = get_skill_level(ch, sn) ;
        int i, place;
        OBJ_DATA *armor;
  double dam = (40 + level)/2 ;

  dam = dam * int_effect( ch );
  
  if( !IS_NPC(ch) && get_skill_level( ch, sn) > 0 )
    dam = dam * ( 10 + get_skill_level( ch, sn )) / 10 ;
  else if ( IS_NPC(ch) )
    dam *= 1.4;

  if( get_skill_level(ch,sn) > 2 )
          dam *= 1.08;
  if( get_skill_level(ch,sn) > 3 )
          dam *= 1.08;
  if( get_skill_level(ch,sn) > 4 )
          dam *= 1.08;
  if( get_skill_level(ch,sn) > 5 )
          dam *= 1.08;
    
        dam  = dam * weakness_damage( victim, sn ) / 100;
        dam  = dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
  dam = number_range( dam * 2.5 , dam * 3 );

    if( dam > 0 ) {
        while( sklv-- ) {
            if ( sklv < 0 )
                break;

  for(i=0 ;i<15;++i ){ //max try 15 times
      place = number_range (1,15);
      if( (armor = get_eq_char(victim, place) ) 
                    && armor->item_type == ITEM_ARMOR){
    waste_dura(armor);
    waste_dura(armor);
    waste_dura(armor);
    waste_dura(armor);
    break;
      }
  }
        }
    }

        magic_damage( ch, victim, dam, sn, WEAR_NONE );

        return;
}

void spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam;
	int i, place;
	OBJ_DATA *armor;

	if( !IS_NPC(ch) ) {
		ppl_spell_acid_blast( sn, ch->level, ch, vo );
		return;
	}
	dam = dice( ch->level, 10 ) * new_saves_spell( ch->pp, victim , MAGIC_EARTH) / 100;
	dam = dam * weakness_damage( victim, sn ) / 100;
	if( dam > 0 ) {
		for(i=0 ;i<15;++i ){ //max try 15 times
			place = number_range (1,15);
			if( ( armor = get_eq_char(victim, place)) && armor->item_type == ITEM_ARMOR){
				waste_dura(armor);
				waste_dura(armor);
				waste_dura(armor);
				waste_dura(armor);
				waste_dura(armor);
				waste_dura(armor);
				break;
			}
		}
	}

	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}

// modified by whisperwind 8.25.2004


void spell_cause_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
	// cause 系列法術修改為無 stun 高燃費的純法術傷害技能 2022/02/06

	CHAR_DATA *victim = (CHAR_DATA *) vo;
	double dam; // basic value;
	int sklv = get_skill_level (ch, sn);
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
		dam = (40 + obj_lv)/2;
	}else{
		dam = (40 + level)/2;
	}
	
	// sklv effect
	if( !IS_NPC(ch) && sklv > 0 )
		dam = dam * ( 20 + sklv) / 20 ;
	else if ( IS_NPC(ch) )
		dam = dam * 1.1;

	dam = dam * wis_effect( ch );

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 1.5 , dam * 1.8 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam , dam * 1.2 );
  
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}



void spell_cause_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
	// cause 系列法術修改為無 stun 高燃費的純法術傷害技能 2022/02/06

	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int    sklv = get_skill_level (ch, sn);
	double dam; // basic value;
	int    obj_lv = level;
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

	dam = dam * wis_effect( ch );

	// sklv effect
	if( !IS_NPC(ch) && sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;
	else if ( IS_NPC(ch) )
		dam = dam * 1.2;

	if( sklv > 2 )
		dam = dam * 1.03;
	if( sklv > 3 )
		dam = dam * 1.03;
	if( sklv > 4 )
		dam = dam * 1.04;
	if( sklv > 5 )
		dam = dam * 1.04;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 2.3 , dam * 2.4 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam , dam * 1.3 );
  
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}



void spell_cause_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
	// cause 系列法術修改為無 stun 高燃費的純法術傷害技能 2022/02/06

	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int    sklv = get_skill_level (ch, sn);
	double dam; // basic value;
	int    obj_lv = level;
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

	dam = dam * wis_effect( ch );

	// sklv effect
	if( !IS_NPC(ch) && sklv > 0 )
		dam = dam * ( 9 + sklv) / 10 ;
	else if ( IS_NPC(ch) )
		dam = dam * 1.1;

	if( sklv > 2 )
		dam = dam * 1.02;
	if( sklv > 3 )
		dam = dam * 1.02;
	if( sklv > 4 )
		dam = dam * 1.03;
	if( sklv > 5 )
		dam = dam * 1.03;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;

	if( !IS_NPC(ch) )
		dam = number_range( dam * 1.9 , dam * 2.2 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam , dam * 1.2 );
  
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}



void spell_change_sex( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA  *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if( (!IS_NPC( ch ) && !IS_IMMORTAL( ch )))
  {
      send_to_char( "一股神聖力場阻絕了你的企圖！\n\r", ch );
      return;
  }
  
  if( IS_AFFECTED( victim, AFF_CHANGE_SEX ) )
  {
      act( "$E 的性別已經改變過了.", ch, NULL, victim, TO_CHAR );
      return;
  }

  af.type  = sn;
  af.duration  = 10 * (level+get_skill_level(ch,sn));
  af.location  = APPLY_SEX;
  do
  {
      af.modifier  = number_range( 0, 2 ) - victim->sex;
  }while ( af.modifier == 0 );
  
  af.bitvector = AFF_CHANGE_SEX;
  affect_to_char( victim, &af );
  
  if( ch != victim )
      send_to_char( "好的。\n\r", ch );
      
  send_to_char( "你覺得身體起了點異樣的變化.\n\r", victim );
  dm_effect( ch );
  return;
}


// earth bind add by whisperwind 10.31.2004
void spell_earth_bind ( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *victim, *victim_next;
	AFFECT_DATA af;
	int sklv = get_skill_level(ch,sn);
	int dam;

	send_to_char( YEL "大地回應著你的呼喚，在對方身上施加了強大的束縛！\n\r"NOR"", ch );
	act(  YEL"大地回應著$n"YEL"的呼喚, 從 $n "YEL"身旁竄出閃耀著光輝的奇妙文字！"NOR"", ch, NULL, NULL, TO_ROOM );
	for( victim = ch->in_room->people; victim; victim = victim_next)
	{
		victim_next = victim->next_in_room;
		if ( is_same_group( ch, victim ) )
		{
			continue;
		}
		if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
			continue;
	  
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
			continue;
		}
		if ( IS_NPC( ch ) )
		{
			if ( victim->fighting == ch || !IS_NPC( victim ) )
			{
				dam = 0;
				dam = (40 + ch->level)/2 ;
				dam = dam * int_effect( ch );
			
				if( get_skill_level( ch, sn) > 0 )
					dam = dam * ( 10 + get_skill_level( ch, sn )) / 10 ;
				if( get_skill_level(ch,sn) > 3 )
					dam *= 1.1;
				if( get_skill_level(ch,sn) > 4 )
					dam *= 1.1;
				if( get_skill_level(ch,sn) > 5 )
					dam *= 1.1;

				dam = dam * weakness_damage( victim, sn ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
				dam = number_range( dam * 8 , dam * 10 );

				magic_damage( ch, victim, dam, sn, WEAR_NONE );

				if (is_affected( victim, skill_lookup("prot. from earth")))
					continue;
				// 有 mystic armor 不作用負面 2020/10/26
				if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
					continue;
				af.type      = sn;
				af.bitvector = 0;
				//af.type      = sn;
				//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_EARTH)) / 100;
				// 修改作用時間及debuff 上限 2020/11/06
				if( affected_value(victim,sn,APPLY_SAVING_EARTH) < (affected_value(victim,sn,APPLY_SAVING_EARTH) - NEW_GET_SVS(victim, MAGIC_EARTH) ) ){
					if(affected_value(victim,sn,APPLY_SAVING_EARTH) > 0){
						af.duration  = 0;
					}else{
						af.duration  = 2;
					}
				af.location  = APPLY_SAVING_EARTH;
				af.modifier  = 4 + sklv;
				//affect_to_char( victim, &af );
				affect_join( victim, &af );
				}

				int nWaitTick = sklv * 2;
				nWaitTick = nWaitTick * (new_saves_spell(ch->pp, victim, MAGIC_EARTH)) / 100;
				if (new_saves_spell(ch->pp, victim, MAGIC_EARTH) * ch->pp / 100 > number_percent()) {
					send_to_char("剎那間，你彷彿與大地合為一體，無法動彈！\n\r", victim);
				act(  "剎那間，$n彷彿與大地合為一體，無法動彈！"NOR"", victim, NULL, NULL, TO_ROOM );
				WAIT_MAGIC_STATE(victim, nWaitTick);
				}
			}
		} else if ( victim->fighting == ch
					|| ( IS_NPC( victim )
					&& !IS_AFFECTED( victim, AFF_CHARM )
					&& !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
		{
			dam = (40 + ch->level)/2 ;
			dam = dam * int_effect( ch );
        
			if( get_skill_level( ch, sn) > 0 )
				dam = dam * ( 10 + get_skill_level( ch, sn )) / 10 ;
			if( get_skill_level(ch,sn) > 3 )
				dam *= 1.1;
			if( get_skill_level(ch,sn) > 5 )
				dam *= 1.1;

			dam = dam * weakness_damage( victim, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
			dam = number_range( dam * 8 , dam * 10 );
      
			magic_damage( ch, victim, dam, sn, WEAR_NONE );
                  
			if (is_affected( victim, skill_lookup("prot. from earth")))
				continue;
			// 有 mystic armor 不作用負面 2020/10/26
			if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
				continue;
			//af.type      = sn;
			//af.duration  = 3 * ( new_saves_timer( ch->pp, victim,MAGIC_EARTH)) / 100;
			// 修改作用時間及debuff 上限 2020/11/06
			af.type      = sn;
			af.bitvector = 0;
			if( affected_value(victim,sn,APPLY_SAVING_EARTH) < (affected_value(victim,sn,APPLY_SAVING_EARTH) - NEW_GET_SVS(victim, MAGIC_EARTH) ) ){
				if(affected_value(victim,sn,APPLY_SAVING_EARTH) > 0){
					af.duration  = 0;
				}else{
					af.duration  = 2;
				}
			af.location  = APPLY_SAVING_EARTH;
			af.modifier  = 4 + sklv;
			//affect_to_char( victim, &af );
			affect_join( victim, &af );
			}
          
			WAIT_MAGIC_STATE(victim, sklv * 2);
		}
	}
}

// soul of magma add by whisperwind 11.26.2004
void spell_soul_of_magma ( int sn, int level, CHAR_DATA *ch, void *vo)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv = level;
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
	
  dam = dam * int_effect( ch );

  if( sklv > 0 )
    dam = dam * ( 10 + sklv) / 10 ;
  if( sklv > 2 )
          dam = dam * 1.1;
  if( sklv > 3 )
          dam = dam * 1.1;
  if( sklv > 4 )
          dam = dam * 1.1;
  if( sklv > 5 )
          dam = dam * 1.1;
    
        dam = dam * weakness_damage( victim, sn ) / 100;
        if( victim->saving_throw[MAGIC_FIRE] < victim->saving_throw[MAGIC_EARTH] )
          dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
        else
          dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_FIRE)) / 100;
        
  if( !IS_NPC(ch) )
    dam = number_range( dam * 3 , dam * 5 );
  else if ( IS_NPC(ch) )
    dam = number_range( dam * 1.5 , dam * 2.5 );

        magic_damage( ch, victim, dam, sn, WEAR_NONE );

        return;
}

// fang of slime add by whisperwind 11.26.2004
void spell_fang_of_slime ( int sn, int level, CHAR_DATA *ch, void *vo)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv = level;
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
	
  dam = dam * int_effect( ch );

  if( sklv > 0 )
    dam = dam * ( 10 + sklv) / 10 ;
  if( sklv > 2 )
          dam = dam * 1.1;
  if( sklv > 3 )
          dam = dam * 1.1;
  if( sklv > 4 )
          dam = dam * 1.1;
  if( sklv > 5 )
          dam = dam * 1.1;
    
        dam = dam * weakness_damage( victim, sn ) / 100;
        if( victim->saving_throw[MAGIC_COLD] < victim->saving_throw[MAGIC_EARTH] )
          dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_EARTH)) / 100;
        else
          dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_COLD)) / 100;
        
  if( !IS_NPC(ch) )
    dam = number_range( dam * 3 , dam * 5 );
  else if ( IS_NPC(ch) )
    dam = number_range( dam * 1.5 , dam * 2.5 );
  
        magic_damage( ch, victim, dam, sn, WEAR_NONE );

        return;
}

// 將 room->fall 的值預設為 0 , 並改寫 fall_effect 判定 2023/01/14
void fall_effect( CHAR_DATA *ch )
{
	ROOM_INDEX_DATA *location;

	if( ch->in_room->fall == 0 )
		return;

	location = get_room_index( ch->in_room->fall );

	if( !location ){
		bug("Rtransfer - No such room: %d." , ch->in_room->vnum );
		return;
	}

	if( ch->deleted || !ch || !ch->in_room )
		return;

	if( ch->in_room->sector_type == SECT_AIR )
	{
		if( !is_flying( ch ) )
		{
			if( !IS_NPC(ch) ){
				bug( ch->name , 0 );
				bug( "fall_effect to %d.", ch->in_room->vnum );
			}
			act( "$n 掉了下去。", ch, NULL, NULL, TO_ROOM );
			char_from_room( ch );
			char_to_room( ch, location );
			act( "$n 從高處掉了下來。", ch, NULL, NULL, TO_ROOM );
			send_to_char("你從高處掉了下來。\n\r", ch);
		}
	}

	/* 2023/01/14
	if ( !ch->deleted && ch->in_room->sector_type == 9 && ch->in_room->fall != '\0' )
	{
		if ( !IS_AFFECTED( ch, AFF_FLYING ) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_FLY ) )
		{
			location = get_room_index( ch->in_room->fall );
			if( !location )
				bug("Rtransfer - No such room: %d." , ch->in_room->vnum );
			else
			{
				act( "$n 掉了下去。", ch, NULL, NULL, TO_ROOM );
				char_from_room( ch );
				char_to_room( ch, location );
				act( "$n 從高處掉了下來。", ch, NULL, NULL, TO_ROOM );
				send_to_char("你從高處掉了下來。\n\r", ch);
				//damage( ch, ch, number_range(ch->max_hit/5, ch->max_hit/3), gsn_flying, WEAR_NONE );
				//damage( ch, ch, UMIN(number_range(ch->max_hit/5, ch->max_hit/3),ch->hit-1), gsn_flying, WEAR_NONE );
			}
		}
	} */
	return;
}

void align_effect( CHAR_DATA *gch )
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	char buf[ BUF_STRING_LENGTH ];

	for ( obj = gch->carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;
		if ( obj->deleted )
			continue;
		if ( obj->wear_loc == WEAR_NONE )
			continue;
		if( is_broken(obj) ){
			act( "$p損壞了.", gch, obj, NULL, TO_CHAR );
			unequip_char(gch, obj);
			continue;
		}

		if ( ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL  )
			&& IS_EVIL   ( gch ) )
			|| ( IS_OBJ_STAT( obj, ITEM_ANTI_GOOD  )
			&& IS_GOOD   ( gch ) )
			|| ( IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
			&& IS_NEUTRAL( gch ) ) )
		{
			act( "你被 $p 給排斥了.", gch, obj, NULL, TO_CHAR );
			act( "$n 被 $p 排斥了.",   gch, obj, NULL, TO_ROOM );
			unequip_char(gch, obj);
			obj_from_char( obj );
			if ( IS_SET( obj->extra_flags , ITEM_PERSONAL ) )
				obj_to_char( obj, gch );
			else
				obj_to_room( obj, gch->in_room );
			continue;
		}
		if ( obj->pIndexData->max_level > 0 && gch->level > obj->pIndexData->max_level )
		{
			sprintf( buf, "你必須在等級 %d 以內才能使用這樣物品.\n\r",
			obj->pIndexData->max_level );
			send_to_char( buf, gch );
			unequip_char(gch, obj);
			continue;
		}
	}
}

bool IsBeholderCanWear(OBJ_DATA *obj)
{
	if(obj->wear_loc == WEAR_WAIST    ||
	   obj->wear_loc == WEAR_LEGS     ||
	   obj->wear_loc == WEAR_FEET     ||
	   obj->wear_loc == WEAR_TWO_HAND ||
	   obj->wear_loc == WEAR_WIELD    ||
	   obj->wear_loc == WEAR_WIELD_2  ){
		return FALSE;
	}
	return TRUE;
}

void dm_effect( CHAR_DATA *ch )
{
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;

  for ( obj = ch->carrying; obj; obj = obj_next )
  {
      obj_next = obj->next_content;
      if ( obj->deleted )
          continue;
      if ( obj->wear_loc == WEAR_NONE )
          continue;
      
    if ( obj->race && !IS_NPC( ch ) ){
        int nRaceBit = get_ppl_race_bit(ch->race);
        if (    ( IS_SET( obj->race, ALLOW_BIT) && !IS_SET( obj->race, NUM_BIT(nRaceBit) ) )
             || ( IS_SET( obj->race, DENY_BIT ) &&  IS_SET( obj->race, NUM_BIT(nRaceBit) ) ) )
        {
          act( "你被 $p 給排斥了.", ch, obj, NULL, TO_CHAR );
          act( "$n 被 $p 排斥了.",   ch, obj, NULL, TO_ROOM );
          unequip_char(ch, obj);
          continue;
        }
    }
    if (    ( CAN_WEAR( obj, ITEM_WIELD) && !IS_SET( race_table[ ch->race ].race_abilities, RACE_WEAPON_WIELD ) )
         || ( !IsBeholderCanWear(obj) && IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) ) )
    {
      act( "你被 $p 給排斥了.", ch, obj, NULL, TO_CHAR );
      act( "$n 被 $p 排斥了.",   ch, obj, NULL, TO_ROOM );
      unequip_char(ch, obj);
      continue;
    }
    if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER )
      && ( get_eq_char( ch, WEAR_FINGER_3 ) || get_eq_char( ch, WEAR_FINGER_4 ) || 
           get_eq_char( ch, WEAR_HANDS_2 ) || get_eq_char( ch, WEAR_ARMS_2 ) ) )
    {
      act( "你被 $p 給排斥了.", ch, obj, NULL, TO_CHAR );
      act( "$n 被 $p 排斥了.",   ch, obj, NULL, TO_ROOM );
      unequip_char(ch, obj);
      continue;
    }
  }
  return;
}

void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo )
{
		CHAR_DATA *vch;
		CHAR_DATA *vch_next; // add at 2020/10/30
//    AFFECT_DATA af;
  
	int sklv = get_skill_level (ch, sn);
	int chance = 30;
	double dam; // basic value;
	int    obj_lv = level;

	if( sn == skill_lookup ("earthquake") )
	{
		send_to_char( YEL "你將力量送入大地，使附近的地面產生劇烈的震動！"NOR"\n\r", ch );
		act( "$n"YEL"催動魔力，一股震波自他腳下的地面往周圍擴散出去！"NOR"", ch, NULL, NULL, TO_ROOM );
	}

	if( sklv > 5 ){
		ch->mana -= 15;
	}
	//for ( vch = char_list; vch; vch = vch->next ){
	for( vch = ch->in_room->people; vch; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if ( ch == vch || vch->deleted || !vch->in_room )
			continue;
		if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS))
			continue;
		//Modified by Razgriz 20051006
		if( is_flying( vch ) && !IS_AFFECTED( vch, AFF_TARDY ) && sklv < 5)
			continue;
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, vch, TO_CHAR);
			continue;
		}

		if ( vch->in_room == ch->in_room && !is_same_group( ch, vch ) )
		{
			if ( IS_NPC( ch ) )
			{
				if ( vch->fighting == ch || !IS_NPC( vch ) )
				{
					dam  = (40 + ch->level)/2;
					dam = dam * wis_effect( ch );
					dam = dam * ( 10 + sklv) / 10 ;
					if( sklv > 2 )
						dam = dam * 1.03;
					if( sklv > 3 )
						dam = dam * 1.03;
					if( sklv > 4 )
						dam = dam * 1.03;
					if( sklv > 5 )
						dam = dam * 1.03;
				
					dam = dam * ( new_saves_spell( ch->pp, vch,MAGIC_EARTH)) / 100;
					dam = number_range( dam * 1 , dam * 2 );
					
					magic_damage( ch, vch, dam, sn, WEAR_NONE );
				}
				continue;
			}
			else if ( (vch->fighting == ch && !IS_NPC(vch)) ||
								( IS_NPC( vch ) && !IS_AFFECTED( vch, AFF_CHARM ) && !IS_AFFECTED( vch, AFF_SUMMONED ) ) )
			{
				dam = 0;
				if( level >= 1000){
					obj_lv = obj_lv/1000;
					sklv = 0;
					if( obj_lv >= 10) sklv = 1;
					if( obj_lv >= 20) sklv = 2;
					if( obj_lv >= 30) sklv = 3;
					if( obj_lv >= 40) sklv = 4;
					if( obj_lv >= 50) sklv = 5;
					if( obj_lv >= 60) sklv = 6;
					dam  = (40 + obj_lv)/2;
				}else{
					dam  = (40 + ch->level)/2;
				}

				chance = 30;
				dam = dam * wis_effect( ch );

					dam = dam * ( 10 + sklv) / 10 ;
				if( sklv > 2 )
					dam = dam * 1.08;
				if( sklv > 3 )
					dam = dam * 1.08;
				if( sklv > 4 )
					dam = dam * 1.08;
				if( sklv > 5 )
					dam = dam * 1.08;

				dam = dam * weakness_damage( vch, sn ) / 100;
				dam = dam * ( new_saves_spell( ch->pp, vch,MAGIC_EARTH)) / 100;
				dam = number_range( dam * 2 , dam * 3 );
      
				magic_damage( ch, vch, dam, sn, WEAR_NONE );
            
				chance = chance * wis_effect( ch );

				if( sklv > 0 )
					chance = chance * ( 10 + sklv) / 10 ;
    
				chance = chance * weakness_percent( vch, sn ) / 100;
				chance = chance * ( new_saves_spell( ch->pp, vch,MAGIC_EARTH)) / 100;
				chance = UMIN(chance,50);

//      magic_damage( ch, vch, (get_skill_level(ch,sn)*2 +level + dice((level/4)+ 7+race_table[vch->race].size, 12)-vch->mdr) * weakness_damage( vch, sn ) / 100, sn, WEAR_NONE );
//      if ( ( IS_NPC(vch) && (IS_SET( vch->special, NO_PARA)) )||
//           IS_AFFECTED(vch, AFF_PARALYZED))
//           continue;
//      if( get_skill_level(ch,sn) > 4 && ch->mana > 80 && ch->move> 50) 
//      if ( saves_spell(ch->pp,vch) + get_curr_wis(ch) - get_curr_wis(vch) > number_percent()*100/weakness_percent(vch,sn)
//            - get_skill_level(ch,sn) - get_skill_level(ch,sn)/2 )

				if ( IS_AFFECTED( vch, sn ) || IS_SET( vch->special, NO_PARA)
						|| IS_SET( race_table[vch->race].race_abilities, RACE_NO_POISON ) 
            || number_percent() > chance || number_percent() > get_skill_percent(ch,sn) )
						continue;

				//Added by Razgriz	20050908
				if( check_qmark ( vch, "earthquake_stun" ) )
						continue;
				// 有 mystic armor 不作用負面 2020/10/26
				if( IS_AFFECTED( vch, AFF_MYSTIC_ARMOR ) )
						continue;

				if( sklv > 5)
				{
					if (!affected_value(vch,sn,APPLY_DODGE) > 0 )
					{
						act( "$N 突然動彈不得！", ch, NULL, vch, TO_CHAR  );
						send_to_char( "你全身一麻，突然動彈不得了！\n\r", vch);
						act( "$N 突然動彈不得！", ch, NULL, vch, TO_NOTVICT );

						//Added by Razgriz	20050908
						if ( IS_NPC (vch) )
							do_mpadd_qmark ( vch, "self earthquake_stun -1 0 0 0 0 1" );
						else
							do_add_qmark ( vch, "self earthquake_stun -1 0 0 0 0 1" );

						WAIT_MAGIC_STATE(vch, sklv * sklv * 2);
                        /*af.type  = gsn_paralyze;
                        af.duration  = 1;
                        af.location  = APPLY_DODGE;
                        af.modifier  = 100;
                        af.bitvector = AFF_PARALYZED;*/
						// affect_to_char( vch, &af );
					}
				}

			}//end else if ( (vch->fighting == ch && !IS_NPC(vch)) ||
			continue;
		}//end if ( vch->in_room == ch->in_room && !is_same_group( ch, vch ) )

		if ( !vch->in_room->area )
		{
			bug( "Earthquake error!!!, room %d isn't in any area!", vch->in_room->vnum );
			bug( vch->name, 0 );
		}

		if ( vch->in_room->area == ch->in_room->area )
			send_to_char( "你感到腳下的地面激烈地震動.\n\r", vch );
	}//end for ( vch = char_list ....
	return;
}

// modified by whisperwind 8.24.2004
void spell_wizard_rage( int sn, int level, CHAR_DATA *ch, void *vo )
{
	int age;
	int dam;
	QMARK *qm;
	int x = 1;
	char buf[MAX_INPUT_LENGTH];
	CHAR_DATA *vch = (CHAR_DATA*)vo;
	int sklv = get_skill_level(ch,sn); // 2023/01/12

	if( IS_NPC(ch) ){
		return;
	}

	if( (qm = is_qmark(ch, "wizard_rage_timer"))){
		send_to_char("你還沒從上次施法的損耗中回復過來。\n\r", ch);
		x = qm->v0;
		return;
	}

	if( ch->mana < ch->max_mana / 2 || ch->move < ch->max_move/2){
		send_to_char("你的身體的狀態不足以施展戰巫天怒。\n\r", ch);
		return;
	}

	// 將 age 代為 get_age, 並取代 playd 作判定 2023/01/12
	/*if( ch->played < 86400 * x){
		send_to_char("你的能力不足以施展戰巫天怒。\n\r", ch);
		return;
	}else{
		int ped = ch->played;
		for( age = 0; ped > 0; ++age)
			ped -= ( 86400 * age );
	} */

	age = get_age( ch );

	if( age + sklv < 40 ){
		send_to_char("你的能力不足以施展戰巫天怒。\n\r", ch);
		return;
	}

	if( qm ){
		sprintf(buf, "self wizard_rage_timer timer + %d", 24 - 3 * sklv );
		do_mod_qmark(ch, buf);
		sprintf(buf, "self wizard_rage_timer v0 + %d", x*1);
		do_mod_qmark(ch, buf);
	}else{
		sprintf(buf, "self wizard_rage_timer %d 2 0 0 0 1", 24 - 3 * sklv );
		do_add_qmark(ch, buf);
	}

	act(HIB"你從體內抽出一團混沌之火，直擊"NOR"$N"HIB"的因果律！"NOR, ch, NULL, vch, TO_CHAR);
	act("$n"HIB"從體內抽出一團混沌之火，直擊你的因果律！"NOR, ch, NULL, vch, TO_VICT);
	act("$n"HIB"從體內抽出一團混沌之火，直擊"NOR"$N"HIB"的因果律！"NOR, ch, NULL, vch, TO_NOTVICT);

	dam = age;

	if( age > 100)
		age = 100;

	dam += age * (ch->level)/2;

	ch->mana = 0;
	ch->move = 0;

	// 降低屬性 2023/01/12
	AFFECT_DATA af;
	af.type	 = gsn_wizard_rage;
	af.duration	 = 24 - 3 * sklv;
	af.location	 = APPLY_STR;
	af.modifier	 = 0 - (10 - sklv);
	af.bitvector = 0;
	affect_to_char( ch, &af );

	af.type	 = gsn_wizard_rage;
	af.duration	 = 24 - 3 * sklv;
	af.location	 = APPLY_DEX;
	af.modifier	 = 0 - (10 - sklv);
	af.bitvector = 0;
	affect_to_char( ch, &af );

	af.type	 = gsn_wizard_rage;
	af.duration	 = 24 - 3 * sklv;
	af.location	 = APPLY_INT;
	af.modifier	 = 0 - (10 - sklv);
	af.bitvector = 0;
	affect_to_char( ch, &af );

	af.type	 = gsn_wizard_rage;
	af.duration	 = 24 - 3 * sklv;
	af.location	 = APPLY_WIS;
	af.modifier	 = 0 - (10 - sklv);
	af.bitvector = 0;
	affect_to_char( ch, &af );

	af.type	 = gsn_wizard_rage;
	af.duration	 = 24 - 3 * sklv;
	af.location	 = APPLY_CON;
	af.modifier	 = 0 - (10 - sklv);
	af.bitvector = 0;
	affect_to_char( ch, &af );

	//ch->max_hit = UMAX( 1, ch->max_hit - 5*x);
	//ch->max_mana = UMAX( 1, ch->max_mana - 5*x);
	//ch->max_move = UMAX( 1, ch->max_move - 5*x);

	if( !IS_NPC(vch) && !IS_NPC(ch))
		dam /= 2;

	dam = dam * weakness_damage( vch, sn ) / 100;
	dam_message( ch, vch, dam, sn, WEAR_NONE );

	if( vch->hit >= dam)
		vch->hit -= dam;
	else
		raw_kill( ch, vch );

	WAIT_MAGIC_STATE(ch, 100 - 15 * sklv );
	send_to_char("你感到全身的力量伴隨著這一擊而逐漸離你遠去。\n\r", ch);
}


/* powerball spell added by Amenda 12/26/00 */
// modified by whisperwind 8.23.2004
void spell_powerball( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    sklv = get_skill_level (ch, sn);
	double dam; // basic value;
	int    obj_lv = level;
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

	dam = dam * int_effect( ch );

	// sklv effect
	if( !IS_NPC(ch) && sklv > 0 )
		dam = dam * ( 10 + sklv) / 10 ;
	else if ( IS_NPC(ch) )
		dam = dam * 1.4;

	if( sklv > 2 )
		dam = dam * 1.08;
	if( sklv > 3 )
		dam = dam * 1.08;
	if( sklv > 4 )
		dam = dam * 1.08;
	if( sklv > 5 )
		dam = dam * 1.08;

	dam = dam * weakness_damage( victim, sn ) / 100;
	dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;
	if( !IS_NPC(ch) )
		dam = number_range( dam * 7 , dam * 10 );
	else if ( IS_NPC(ch) )
		dam = number_range( dam * 5 , dam * 8 );

	// Keric modfied to magic_damage
		//dam -= victim->mdr;
  
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	return;
}


/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
/*
 * Modified by Amenda. The more group members, the more powerful spell.
 */

void spell_general_purpose( int sn, int level, CHAR_DATA *ch, void *vo )
{

	CHAR_DATA *victim = (CHAR_DATA *) vo;
	CHAR_DATA *gch;
	CHAR_DATA *vgch;
	int dam;
	int sklv = get_skill_level(ch, sn);
	int gch_count = 0;
	int vgch_count = 0;
	int dam_times = 0;
	int dam_bonus = 0;
	
	dam = ch->level;

	if( !IS_NPC(ch) ){
		dam += UMIN( 2000 , ch->pcdata->respect*2*sklv ) + ch->pcdata->respect*sklv/6;
		dam = dam * (1 + ch->pcdata->merit / ( ch->pcdata->merit + 1000 ) );
	}else{
		if( IS_SET(ch->special, EXPERT ) )
			dam = UMIN( 2000 , dam * ch->level/6 );
		else
			dam = UMIN( 2000 , dam * ch->level/8 );
	}

	if( (ch->move/ch->max_move) > (ch->mana/ch->max_mana) )
		dam = dam * ch->move/ch->max_move;
	else
		dam = dam * ch->mana/ch->max_mana;

	if( !IS_NPC(ch) ){
		if( ch->move < ch->max_move/10 ){
			send_to_char( "你剩餘的體力不適合發動群眾之審判！\n\r", ch );
			return;
		}
		if( ch->mana < ch->mana/10 ){
			send_to_char( "你剩餘的魔力不適合發動群眾之審判！\n\r", ch );
			return;
		}
		ch->move = ch->move - ch->max_move/10;
		ch->mana = ch->mana - ch->max_mana/10;
	}

	for ( gch = ch->cooperator; gch ; gch = gch->next_in_coop )
	{
		if( ch->in_room != gch->in_room ) continue;
		if( gch->move < gch->max_move/15 ) continue;
		if( gch->mana < gch->max_mana/15 ) continue;
		gch_count++;
		if( IS_GOOD(ch) ){
			if( IS_GOOD(gch) ) dam_times += 30;
			if( IS_NEUTRAL(gch) ) dam_times += 20;
			if( IS_EVIL(gch) ) dam_times += 10;
		}
		if( IS_NEUTRAL(ch) ){
			dam_times += 20;
		}
		if( IS_EVIL(ch) ){
			if( IS_GOOD(gch) ) dam_times += 10;
			if( IS_NEUTRAL(gch) ) dam_times += 20;
			if( IS_EVIL(gch) ) dam_times += 30;
		}
		if( IS_NPC(gch) ){
			if( IS_SET(gch->special, EXPERT ) )
				dam_times += gch->level/2;
			else
				dam_times += gch->level/3;
		}else{
			dam_times += gch->pcdata->respect/3;
			dam_bonus += UMIN( 200 , gch->pcdata->merit/1000);
		}

		if( (gch->move/gch->max_move) > (gch->mana/gch->max_mana) )
			dam_times = dam_times * gch->move/gch->max_move;
		else
			dam_times = dam_times * gch->mana/gch->max_mana;

		gch->move = (gch->move) - (gch->max_move)/15;
		gch->mana = (gch->mana) - (gch->max_mana)/15;
		WAIT_STATE(gch, 12);

		act( HIW "你協助"NOR"$n"HIW"發動群眾之審判！"NOR"" , ch, NULL, gch, TO_VICT);

		if ( gch_count > 4 ) break;
	}
	
	for( vgch = victim->in_room->people ; vgch ; vgch = vgch->next_in_room )
	{
		if( !is_same_group( victim, vgch ) ) continue;
		if( victim->in_room != vgch->in_room ) continue;
		vgch_count++;
		if(vgch_count > 4) break;
	}
	
	dam_times = dam_times - vgch_count*10;

	dam = dam * (1 + dam_times/100);

	dam = dam + dam_bonus;

	if( IS_GOOD(ch) ){
		if( IS_GOOD(victim) )
			dam = dam * 0.85;
		if( IS_EVIL(victim) )
			dam = dam * 1.15;
	}
	if( IS_EVIL(ch) ){
		if( IS_GOOD(victim) )
			dam = dam * 1.15;
		if( IS_EVIL(victim) )
			dam = dam * 0.85;
	}

	act( HIW "你號召群眾的意志，對"NOR"$N"HIW"發起精神層面的打擊！"NOR"", ch, NULL, victim, TO_CHAR );
	act( "$n"HIW"號召群眾的意志，對你的發起精神層面的打擊！"NOR"" , ch, NULL, victim, TO_VICT);
	act( "$n"HIW"號召群眾的意志，對"NOR"$N"HIW"發起精神層面的打擊！"NOR"", ch, NULL, victim, TO_NOTVICT );
	
	if( !IS_NPC(victim) && !IS_NPC(ch)) dam /= 2;
	
	if( is_affected( ch, gsn_mental_empower) ) dam = dam * 1.1;
	
	if( is_affected(victim, gsn_will_fortress) ) dam = dam * 0.9;

	dam = dam * weakness_damage( victim, sn ) / 100;
	magic__damage( ch, victim, dam, sn, WEAR_NONE );

	WAIT_STATE(ch, 12);

	/*
		重寫 general purpose 判定 2021/11/21

		if ( ch-> hit < 200 || ch->move < 200 || ch->mana < 70)
		{
			send_to_char( "你的狀況不適合發動群眾審判！\n\r", ch );    
			return; 
		}

		for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
		{
			if ( is_same_group( ch, rch ) && !IS_NPC( rch ) && rch->hit * 2 >= rch->max_hit && rch->move * 2 >= rch->max_move){
				count++;
				rch->hit -= ( rch->max_hit / 30 );
				rch->move -= (rch->max_move / 20);
			}
		}
		

		dam = 10 * UMAX(10, count*2+get_skill_level(ch,sn)*5) * saves_spell(ch->pp, victim ) / 100;

		// Keric modfied to magic_damage
		//dam -= victim->mdr;

		dam = dam * weakness_damage( victim, sn ) /100;
		magic__damage( ch, victim, dam, sn, WEAR_NONE );

		WAIT_MAGIC_STATE(ch, 16);
	*/
	return;
}


void spell_null( int sn, int level, CHAR_DATA *ch, void *vo )
{
    send_to_char( "那不是法術！\n\r", ch );
    return;
}

// poison explosion add by whisperwind 11.26.2004
void spell_poison_explosion( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim, *victim_next;
	OBJ_DATA *obj, *obj_next;
	int dam;
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
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

	if( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_SUMMONED ) )
		return;

	for ( obj = ch->in_room->contents; obj; obj = obj_next )
	{	
		obj_next = obj->next_content;

		// 2020/10/29
		if ( obj->pIndexData->vnum != OBJ_VNUM_CORPSE_NPC )
			continue;

		//if ( obj->pIndexData->vnum == OBJ_VNUM_CORPSE_NPC ){
		extract_obj(obj);

		send_to_char( HIG "你召來神秘的力量，讓眼前的屍體瞬間崩成齎粉，變幻成七彩的毒霧！\n\r"NOR"", ch );
		act( HIG "附近的死屍冷不防爆炸，一股絢麗的彩色毒霧隨即充斥在四周！"NOR"", ch, NULL, NULL, TO_ROOM );
		for( victim = ch->in_room->people; victim; victim = victim_next)
		{
			victim_next = victim->next_in_room;
			if ( is_same_group( ch, victim ) )
				continue;

			if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
				continue;
			
			// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
			if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
			{
				act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
				continue;
			}

			if ( IS_NPC( ch ) )
			{
				if ( victim->fighting == ch || !IS_NPC( victim ) )
				{
					dam = (40 + ch->level)/2 ;
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
					dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;
					dam = number_range( dam * 0.5 , dam * 2 );
					dam += NEW_GET_SVS(victim,MAGIC_POISON) * ( 100 + sklv * 20 ) / 100;

					magic_damage( ch, victim, dam, sn, WEAR_NONE );

					spell_poison( sn, ch->level, ch, victim );
				}
			}
			else if ( victim->fighting == ch
					|| ( IS_NPC( victim ) && !IS_AFFECTED( victim, AFF_CHARM ) && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
			{
				dam = (40 + ch->level)/2 ;
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
				dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_SPELL)) / 100;
				dam = number_range( dam * 0.5 , dam * 2 );
				dam += NEW_GET_SVS(victim,MAGIC_POISON) * ( 100 + sklv * 20 ) / 100;

				magic_damage( ch, victim, dam, sn, WEAR_NONE );

				spell_poison( sn, ch->level, ch, victim );
			}
		}
		//break;
		return;
		/* remove at 2020/10/29
		}
		else
		{
			send_to_char( "地上並沒有屍體可以引爆！\n\r"NOR"", ch );
			break;
		}
		*/
	}
	send_to_char( "地上並沒有屍體可以引爆！\n\r"NOR"", ch );
	return;
}

void spell_polymorph_other( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    char  buf[ MAX_STRING_LENGTH ];
    int   newrace;

    if ( IS_AFFECTED( victim, AFF_POLYMORPH) )
    {
        act( "$E已經處於改變的狀態了.", ch, NULL, victim, TO_CHAR );
        return;
    }

    affect_strip(victim, gsn_werewolf);
		affect_strip(victim, gsn_stone_form);

		af.type  = sn;
    af.duration  = dice( 10, ch->level );
    af.location  = APPLY_RACE;

    do
    {
        af.modifier  = number_range( 0, MAX_RACE - 1 ) - victim->race;
    }
    while ( af.modifier == 0 );

    af.bitvector = AFF_POLYMORPH;
    newrace = victim->race + af.modifier;
    affect_to_char( victim, &af );
    if ( ch != victim )
        send_to_char( "好的。\n\r", ch );
    send_to_char( "你的身體產生了一些異變.\n\r", victim );
  //modified by jye 0950310
    if (!IS_NPC( victim ))
        dm_effect( victim );

    if ( get_trust( ch ) >= L_APP )
    {
        sprintf( buf, "Log %s: casting Polymorph other", ch->name );
        log_string( buf );
    }

    return;

}

// shocking grasp modified by whisperwind 8.23.2004
void spell_shocking_grasp( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

	int    nSklv = get_skill_level(ch,sn);
	double dam; // basic value;
	int    obj_lv = level;
	
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 2)  nSklv = 1;
		if( obj_lv >= 10) nSklv = 2;
		if( obj_lv >= 22) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 61) nSklv = 5;
		if( obj_lv >= 90) nSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}
	
  dam = dam * int_effect( ch );
  dam = dam * weakness_damage( victim, sn ) / 100;
  dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;

  if( !IS_NPC(ch) )
    dam = number_range( 1 + dam * nSklv*2/3 , dam * ( 10 + nSklv * 4) /5  );
  else if ( IS_NPC(ch) )
    dam = number_range( 1 + dam * nSklv*4/5 , dam * ( 48 + nSklv * 2) /8 );
    
  if( is_flying( victim ) )
    if(nSklv > 0) dam = dam * ( 24 + nSklv ) / 20 ;
	if(nSklv > 2) dam = dam * 1.05;
	if(nSklv > 3) dam = dam * 1.05;
	if(nSklv > 4) dam = dam * 1.05;
	if(nSklv > 5) dam = dam * 1.05;

  magic_damage( ch, victim, dam, sn, WEAR_NONE );

  if ( ( saves_spell( ch->pp, victim ) / 2 + nSklv*2 + WIS_DIFF( ch, victim )) 
       * weakness_percent(victim, sn) / 97 > number_percent() )
    disarm( ch, victim );

  return;
}


void spell_agitation ( int sn, int level, CHAR_DATA *ch, void *vo )
{
     CHAR_DATA *victim   = (CHAR_DATA *) vo;
    static const int      dam_each [ ] =
    {
  0,
   0,  0,  0,  0,  0,  12, 15, 18, 21, 24,
  24, 24, 25, 25, 26,  26, 26, 27, 27, 27,
  28, 28, 28, 29, 29,  29, 30, 30, 30, 31,
  31, 31, 32, 32, 32,  33, 33, 33, 34, 34,
  34, 35, 35, 35, 36,  36, 36, 37, 37, 37
    };
     int      dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam      = number_range( dam_each[level] / 2, dam_each[level] * 2 );
   
    dam = saves_spell( ch->pp, victim ) / 100+ ( INT_DIFF( ch, victim ) * 4 );

    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * weakness_damage (victim, sn) / 100;
    magic_damage( ch, victim, dam, sn, WEAR_NONE );

    return;
}



void spell_aura_sight ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    sn = skill_lookup( "know alignment" );

    (*skill_table[sn].spell_fun) ( sn, level, ch, vo );

    return;
}



void spell_awe ( int sn, int level, CHAR_DATA *ch, void *vo )
  {
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( victim->fighting == ch && !saves_spell( ch->pp, victim ) )
    {
  stop_fighting ( victim, TRUE);
  act( "$N is in AWE of you!", ch, NULL, victim, TO_CHAR    );
  act( "You are in AWE of $n!",ch, NULL, victim, TO_VICT    );
  act( "$N is in AWE of $n!",  ch, NULL, victim, TO_NOTVICT );
    }
    return;
}



void spell_ballistic_attack ( int sn, int level, CHAR_DATA *ch, void *vo )
  {
     CHAR_DATA *victim   = (CHAR_DATA *) vo;
    static const int      dam_each [ ] =
    {
   0,
   3,  4,  4,  5,  6,   6,  6,  7,  7,  7,
   7,  7,  8,  8,  8,   9,  9,  9, 10, 10,
  10, 11, 11, 11, 12,  12, 12, 13, 13, 13,
  14, 14, 14, 15, 15,  15, 16, 16, 16, 17,
  17, 17, 18, 18, 18,  19, 19, 19, 20, 20
    };
     int      dam;
  
    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam      = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    dam = saves_spell( ch->pp, victim ) / 100 + ( INT_DIFF( ch, victim ) * 2 );

    dam = dam * weakness_damage (victim, sn) / 100;
    act( "You chuckle as a stone strikes $N.", ch, NULL, victim,
  TO_CHAR );

    magic_damage( ch, victim, dam, sn, WEAR_NONE );

    return;
}



void spell_biofeedback ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    sn = skill_lookup( "Sanctuary" );

    (*skill_table[sn].spell_fun) ( sn, level, ch, vo );

    return;
}



void spell_cell_adjustment ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
   
    if ( is_affected( victim, gsn_poison ) )
    {
  affect_strip( victim, gsn_poison );
  send_to_char( "A warm feeling runs through your body.\n\r", victim );
  act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
    }
    if ( is_affected( victim, gsn_curse  ) )
    {
  affect_strip( victim, gsn_curse  );
  send_to_char( "You feel better.\n\r", victim );
    } 
    send_to_char( "好的。\n\r", ch );
    return;
}


void spell_control_flames ( int sn, int level, CHAR_DATA *ch, void *vo )
{
     CHAR_DATA *victim   = (CHAR_DATA *) vo;
    static const int      dam_each [ ] = 
    {
   0,
   0,  0,  0,  0,  0,   0,  0, 16, 20, 24,
  28, 32, 35, 38, 40,  42, 44, 45, 45, 45,
  46, 46, 46, 47, 47,  47, 48, 48, 48, 49,
  49, 49, 50, 50, 50,  51, 51, 51, 52, 52,
  52, 53, 53, 53, 54,  54, 54, 55, 55, 55
    };
     int      dam;

    if ( !get_eq_char( ch, WEAR_LIGHT ) )
    {
  send_to_char( "You must be carrying a light source.\n\r", ch );
  return;
    }

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam      = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( ch->pp, victim ) < number_percent() )
  dam /= 2;

    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * weakness_damage (victim, sn) / 100;
    magic_damage( ch, victim, dam, sn, WEAR_NONE );

    return;
}



void spell_create_sound ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    char       buf1    [ MAX_STRING_LENGTH ];
    char       buf2    [ MAX_STRING_LENGTH ];
    char       speaker [ MAX_INPUT_LENGTH  ];

    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s says '%s'.\n\r", speaker, target_name );
    sprintf( buf2, "Someone makes %s say '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER( buf1[0] );

    for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
    {
  if ( !is_name( speaker, vch->name ) )
      send_to_char( saves_spell( ch->pp, vch ) < number_percent() ? 
      buf2 : buf1, vch );
    }
    return;
}



void spell_death_field ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int        dam;
    int        hpch;

    if ( !IS_EVIL( ch ) )
    {
  send_to_char( "You are not evil enough to do that!\n\r", ch);
  return;
    }

    send_to_char( "A black haze emanates from you!\n\r", ch );
    act ( "A black haze emanates from $n!", ch, NULL, ch, TO_ROOM );

    for ( vch = ch->in_room->people; vch; vch = vch_next )
    {
  vch_next = vch->next_in_room;
  if ( vch->deleted )
    continue;

  if ( !IS_NPC( ch ) ? IS_NPC( vch ) : IS_NPC( vch ) )
  {
      hpch = URANGE( 10, ch->hit, 999 );
      if ( saves_spell( ch->pp, vch )
    && (   level <= vch->level + 5
        && level >= vch->level - 5 ) )
      {
    dam = 4; /* Enough to compensate for sanct. and prot. */
    vch->hit = 1;
    update_pos( vch );
    send_to_char( "The haze envelops you!\n\r", vch );
    act( "The haze envelops $N!", ch, NULL, vch, TO_ROOM );
      }
      else
    dam = number_range( hpch / 16 + 1, hpch / 8 );

      // Keric modfied to magic_damage
            //dam -= vch->mdr;
      magic_damage( ch, vch, dam, sn, WEAR_NONE );
  }
    }
    return;
}



void spell_detonate ( int sn, int level, CHAR_DATA *ch, void *vo )
{
     CHAR_DATA *victim   = (CHAR_DATA *) vo;
    static const int      dam_each [ ] =
    {
    0,
    0,   0,   0,   0,   0,  0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,  0,   0,   0,   0,  75,
   80,  85,  90,  95, 100,      102, 104, 106, 108, 110,
  112, 114, 116, 118, 120,      122, 124, 126, 128, 130,
  132, 134, 136, 138, 140,      142, 144, 146, 148, 150
    };
     int      dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam      = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    dam = saves_spell( ch->pp, victim ) / 100 + ( INT_DIFF( ch, victim ) * 8 );
    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    dam = dam * weakness_damage (victim, sn) / 100;
    magic_damage( ch, victim, dam, sn, WEAR_NONE );
    return;
}



void spell_disintegrate ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA  *obj_lose;
    OBJ_DATA  *obj_next;

    if ( number_percent( ) < 2 * level && 
        saves_spell( ch->pp, victim ) < number_percent() )
      for ( obj_lose = victim->carrying; obj_lose; obj_lose = obj_next )
      {
    obj_next = obj_lose->next_content;
    if ( obj_lose->deleted )
        continue;

    if ( number_bits( 2 ) != 0 )
        continue;

    act( "$p disintegrates!", victim, obj_lose, NULL, TO_CHAR );
    act( "$p disintegrates!", victim, obj_lose, NULL, TO_ROOM );
    extract_obj( obj_lose ) ;
      }

    if ( saves_spell( ch->pp, victim ) > number_percent() )
    /*
     * Disintegrate char, do not generate a corpse, do not
     * give experience for kill.  Extract_char will take care
     * of items carried/wielded by victim.  Needless to say,
     * it would be bad to be a target of this spell!
     * --- Thelonius (Monk)
     */
    {
  act( "You have DISINTEGRATED $N!",     ch, NULL, victim, TO_CHAR );
  act( "You have been DISINTEGRATED by $n!", ch, NULL, victim, TO_VICT );
  act( "$n's spell DISINTEGRATES $N!",     ch, NULL, victim, TO_ROOM );
  
  if ( IS_NPC( victim ) )
      extract_char( victim, TRUE );
  else
      extract_char( victim, FALSE );
    }
    return;
}



void spell_displacement ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
  return;

    af.type  = sn;
    af.duration  = level - 4;
    af.location  = APPLY_AC;
    af.modifier  = 4 - level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "Your form shimmers, and you appear displaced.\n\r",
     victim );
    act( "$N shimmers and appears in a different location.",
  ch, NULL, victim, TO_NOTVICT );
    return;
}



void spell_domination ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
  send_to_char( "Dominate yourself?  You're weird.\n\r", ch );
  return;
    }

    if (   IS_AFFECTED( victim, AFF_CHARM )
  || IS_AFFECTED( ch, AFF_CHARM )
  || level < victim->level
  || saves_spell( ch->pp, victim ) < number_percent() )
  return;

    if ( victim->master )
  stop_follower( victim );
    add_follower( victim, ch );

    af.type  = sn;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    act( "Your will dominates $N!", ch, NULL, victim, TO_CHAR );
    act( "Your will is dominated by $n!", ch, NULL, victim, TO_VICT );
    return;
}



void spell_ectoplasmic_form ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED( victim, AFF_PASS_DOOR ) )
  return;

    af.type  = sn;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );

    send_to_char( "You turn translucent.\n\r", victim );
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_ego_whip ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || 
        saves_spell( ch->pp, victim ) < number_percent() )
  return;

    af.type  = sn;
    af.duration  = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 2;
    affect_to_char( victim, &af );

    af.location  = APPLY_AC;
    af.modifier  = level / 2;
    affect_to_char( victim, &af );

    act( "You ridicule $N about $S childhood.", ch, NULL, victim, TO_CHAR    );
    send_to_char( "Your ego takes a beating.\n\r", victim );
    act( "$N's ego is crushed by $n!",    ch, NULL, victim, TO_NOTVICT );

    return;
}



void spell_energy_containment ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
  return;

    af.type  = sn;
    af.duration  = level / 2 + 7;
    af.modifier  = -level / 5;
    af.location  = APPLY_SAVING_SPELL;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You can now absorb some forms of energy.\n\r", ch );
    return;
}





void spell_enhanced_strength ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
  return;

    af.type  = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 1 + ( level >= 15 ) + ( level >= 25 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You are HUGE!\n\r", victim );
    return;
}



void spell_flesh_armor ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
  return;

    af.type  = sn;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "Your flesh turns to steel.\n\r", victim );
    act( "$N's flesh turns to steel.", ch, NULL, victim, TO_NOTVICT);
    return;
}



void spell_inertial_barrier ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *gch;
    AFFECT_DATA af;

    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
  if ( !is_same_group( gch, ch ) || IS_AFFECTED( gch, AFF_PROTECT ) )
      continue;

  act( "An inertial barrier forms around $n.", gch, NULL, NULL,
      TO_ROOM );
  send_to_char( "An inertial barrier forms around you.\n\r", gch );

  af.type      = sn;
  af.duration  = 24;
  af.modifier  = 0;
  af.location  = APPLY_NONE;
  af.bitvector = AFF_PROTECT;
  affect_to_char( gch, &af );
    }
    return;
}



void spell_inflict_pain ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    magic_damage( ch, (CHAR_DATA *) vo, dice( 2, 10 ) + level / 2, sn, WEAR_NONE );
    return;
}



void spell_intellect_fortress ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *gch;
    AFFECT_DATA af;

    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
  if ( !is_same_group( gch, ch ) || is_affected( gch, sn ) )
      continue;

  send_to_char( "A virtual fortress forms around you.\n\r", gch );
  act( "A virtual fortress forms around $N.", gch, NULL, gch, TO_ROOM );

  af.type      = sn;
  af.duration  = 24;
  af.location  = APPLY_AC;
  af.modifier  = -40;
  af.bitvector = 0;
  affect_to_char( gch, &af );
    }
    return;
}



void spell_lend_health ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int        hpch;

    if ( ch == victim )
    {
  send_to_char( "Lend health to yourself?  What a weirdo.\n\r", ch );
  return;
    }
    hpch = UMIN( 50, victim->max_hit - victim->hit );
    if ( hpch == 0 )
    {
  act( "Nice thought, but $N doesn't need healing.", ch, NULL,
      victim, TO_CHAR );
  return;
    }
    if ( ch->hit-hpch < 50 )
    {
  send_to_char( "You aren't healthy enough yourself!\n\r", ch );
  return;
    }
    victim->hit += hpch;
    ch->hit -= hpch;
    update_pos( victim );
    update_pos( ch );

    act( "You lend some of your health to $N.", ch, NULL, victim, TO_CHAR );
    act( "$n lends you some of $s health.", ch, NULL, victim, TO_VICT );

    return;
}



void spell_levitation ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED( victim, AFF_FLYING ) )
  return;

    af.type  = sn;
    af.duration  = level + 3;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );

    send_to_char( "Your feet rise off the ground.\n\r", victim );
    act( "$n's feet rise off the ground.", victim, NULL, NULL, TO_ROOM );
    return;
}


void spell_share_strength ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
  send_to_char( "You can't share strength with yourself.\n\r", ch );
  return;
    }
    if ( is_affected( victim, sn ) )
    {
  act( "$N already shares someone's strength.", ch, NULL, victim,
      TO_CHAR );
  return;
    }
    if ( get_curr_str( ch ) <= 5 )
    {
  send_to_char( "You are too weak to share your strength.\n\r", ch );
  return;
    }

    af.type  = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  =  1 + ( level >= 20 ) + ( level >= 30 );
    af.bitvector = 0;
    affect_to_char( victim, &af );
    
    af.modifier  = -1 - ( level >= 20 ) - ( level >= 30 );
    affect_to_char( ch,     &af );

    act( "You share your strength with $N.", ch, NULL, victim, TO_CHAR );
    act( "$n shares $s strength with you.",  ch, NULL, victim, TO_VICT );
    return;
}


void spell_vampiric_bite( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
	int   sklv = get_skill_level(ch,sn);
	int   obj_lv = level;
	int   dam;

	if( level >= 1000){
		obj_lv = obj_lv/1000;
		sklv = 0;
		if( obj_lv >= 2)  sklv = 1;
		if( obj_lv >= 10) sklv = 2;
		if( obj_lv >= 22) sklv = 3;
		if( obj_lv >= 40) sklv = 4;
		if( obj_lv >= 61) sklv = 5;
		if( obj_lv >= 90) sklv = 6;
		dam = (40 + obj_lv)/20; // base dam
	}else{
		dam = (40 + ch->level)/20; // base dam
	}
	if(IS_NPC(ch))
		dam = dam * ch->damroll/2;
	else
		dam = dam * ch->damroll * (1 + sklv/4);

	//dam += (victim->armor) * (victim->armor) / (0 - 4500);
	dam = UMIN( dam + dam * GET_AC(victim)/1600 , dam + GET_AC(victim)/8 );

	if( dam < 0) dam = 0;

	if( ch->apdr > 0 ){
		if(IS_NPC(ch))
			dam += ch->apdr + ch->apdr * sklv/6;
		else
			dam += ch->apdr + ch->apdr * sklv/2;
	}
	dam = dam - victim->pdr*3;

	if( dam < 0) dam = 0;

	//dam = UMIN( dam , dam + dam * victim->saving_throw[MAGIC_DARK]/100);
	dam = dam * weakness_damage( victim, sn ) / 100;
	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	if( dam <= 0) return;

    if ( victim->race == race_lookup("Vampire") && ch->level < victim->level)
		return;

	if( !IS_AFFECTED( ch , AFF_NO_HEAL ) && IS_AFFECTED( victim, AFF_VAMP_BITE ) )
		ch->hit = UMIN( ch->max_hit, ch->hit + dam * (1 + sklv) / 10 );

    if ( number_percent() + 10 * sklv < 10 - victim->saving_throw[MAGIC_DARK] / 2)
		return;

	// 有 mystic armor 不作用負面 2020/10/26
	if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
		return;

    if( affected_value(victim, sn, APPLY_DAMAGE_COUNT) < 50 + 50 * sklv ){
		af.type = sn;
		if( affected_value(victim, sn, APPLY_DAMAGE_COUNT) > 0 )
			af.duration = 0;
		else
			af.duration = 2;
		af.location = APPLY_DAMAGE_COUNT;
		af.modifier = 25;
		af.bitvector = AFF_VAMP_BITE;
		affect_join( victim, &af );
		send_to_char( "你渾身的血液正在翻騰著！\n\r", victim );
	}
    return;
}


/* Whirl wind spell added by Amenda, 12/25/00 */
// modified by whisperwind 8.25.2004
// modified by Chris 7.17.2005
void spell_whirlwind( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim, *victim_next;
  double dam;
  double basic_dam;
  int    nSklv = get_skill_level( ch, sn );
  int    obj_lv = level;

  if( sn == skill_lookup("whirlwind"))
  {
        send_to_char( ""HIG"你舞動魔力，招來一股狂亂的風元素能量摧毀週遭一切敵人！\n\r"NOR"", ch );
        act(  "$n "HIG"舞動魔力，招來一股狂亂的風元素能量摧毀週遭一切敵人！"NOR"", ch, NULL, NULL, TO_ROOM );
  }
    
	if( level >= 1000){
		obj_lv = obj_lv/1000;
		nSklv = 0;
		if( obj_lv >= 10) nSklv = 1;
		if( obj_lv >= 20) nSklv = 2;
		if( obj_lv >= 30) nSklv = 3;
		if( obj_lv >= 40) nSklv = 4;
		if( obj_lv >= 50) nSklv = 5;
		if( obj_lv >= 60) nSklv = 6;
		dam  = (40 + obj_lv)/2;
	}else{
		dam  = (40 + level)/2;
	}

  dam = dam * int_effect( ch );
  
  basic_dam = dam;
  
  for( victim = ch->in_room->people; victim; victim = victim_next)
  {
	victim_next = victim->next_in_room;
	if ( is_same_group( ch, victim ) )
		continue;
	if( !IS_NPC(victim) && IS_SET( victim->act, PLR_WIZINVIS))
		continue;

	// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
	if( IS_SET( victim->special, ANTI_MAGIC ) && ch->class != WIZARD )
	{
		act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, victim, TO_CHAR);
		continue;
	}

    if ( IS_NPC( ch ) ) // NPC Casting
    {
      //  added by Chris, extra target for summoned 
      if(IS_AFFECTED(ch, AFF_SUMMONED ) || ch->master ){
        if(   victim->fighting == ch->master
           || victim->fighting == ch
           || (    IS_NPC( victim )
               && !IS_AFFECTED( victim, AFF_CHARM )
               && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
        {
          if (!IsSorGateMobWind(ch->pIndexData->vnum) ||
                victim->position == POS_FIGHTING) {
            dam = basic_dam * weakness_damage( victim, sn ) / 100;
            dam = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
            dam = number_range( 1 , dam * ( 38 + nSklv * 2) /10 );

            if( is_flying( victim ) )
              dam = dam * ( 24 + nSklv ) / 20 ;
		  
			if(nSklv > 2) dam = dam * 1.08;
			if(nSklv > 3) dam = dam * 1.08;
			if(nSklv > 4) dam = dam * 1.08;
			if(nSklv > 5) dam = dam * 1.08;

            magic_damage( ch, victim, dam, sn, WEAR_NONE );
          }
        }
      }else{
        if ( victim->fighting == ch || !IS_NPC( victim ) ){
          dam  = basic_dam * weakness_damage( victim, sn ) / 100;
          dam  = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
          dam  = number_range( 1 , dam * ( 38 + nSklv * 2) /10 );
      
          if( is_flying( victim ) )
            dam = dam * ( 24 + nSklv ) / 20 ;
					if(nSklv > 3) dam = dam * 1.1;
					if(nSklv > 4) dam = dam * 1.1;
					if(nSklv > 5) dam = dam * 1.1;
          magic_damage( ch, victim, dam, sn, WEAR_NONE );
        }
      }
    }else if (   victim->fighting == ch
              ||(    IS_NPC( victim )
                 && !IS_AFFECTED( victim, AFF_CHARM )
                 && !IS_AFFECTED( victim, AFF_SUMMONED ) ) )
    { //  PPL Casting
      dam  = basic_dam * weakness_damage( victim, sn ) / 100;
      dam  = dam * ( new_saves_spell( ch->pp, victim,MAGIC_WIND)) / 100;
      dam  = number_range( 1 + dam * nSklv * 4/5 , dam * ( 16 + nSklv * 4) /5  );
      
      if( is_flying( victim ) )
        dam = dam * ( 24 + nSklv ) / 20 ;
			if(nSklv > 3) dam = dam * 1.1;
			if(nSklv > 4) dam = dam * 1.1;
			if(nSklv > 5) dam = dam * 1.1;
      magic_damage( ch, victim, dam, sn, WEAR_NONE );
    }
  }
}


/* Armageddon spell added by Amenda 2/24/01 */
void spell_armageddon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    //QMARK *qmark;
    int dam;
    int mod;
    char buf  [MAX_INPUT_LENGTH];
    
    if ( IS_NPC( ch ) && ( IS_AFFECTED( ch, AFF_CHARM ) || IS_AFFECTED( ch, AFF_SUMMONED ) ) ) {
    if ( ch->mana + ch->hit + 150 < ch->max_mana + ch->max_hit )
    {
      send_to_char( "你現在並不是在最佳狀況，無法施展這麼大型的法術。\n\r", ch );
  return;
    }
    }

    if (  is_qmark(ch, "spell_armageddon_timer") ){
  send_to_char( "你還沒從上次施法的損耗中回復過來。\n\r", ch );
  return;
    }



    mod = 35 - UMIN( 15, (ch->played / 200000 - 10));
    dam =( victim->hit * mod )/ 100;
    
    // Keric modfied to magic_damage
    //dam -= victim->mdr;
    
    dam = dam * weakness_damage( victim, sn ) / 100;
    dam = UMIN( dam, victim->hit );
    victim->hit -= dam;
//    ch->hit = 1;

    sprintf( buf, "self spell_armageddon_timer %d 0 0 0 0 1", 24-get_skill_level(ch,sn) -get_skill_level(ch,sn)/2 );
    do_add_qmark( ch, buf );

  //if ( ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_CHINESE ) )
  //  || ( IS_NPC( ch ) && !IS_NPC( victim ) && !IS_SET( victim->act, PLR_CHINESE ) ) )
    dam_message( ch, victim, dam, gsn_armageddon, WEAR_NONE );

    af.type  = sn;
    af.duration  = 5;
    af.location  = APPLY_DODGE;
    af.modifier  = ch->level * 2;
    af.bitvector = 0;
    affect_join( ch, &af );

      send_to_char( "你覺得全身似乎被抽乾，意識慢慢模糊....\n\r", ch );
          act( ""HIP"$n 突然全身虛脫地倒下來！"NOR"", ch, NULL, NULL, TO_ROOM );
          stop_fighting(victim,FALSE);
          set_fighting( victim, ch );
    if( !IS_NPC(ch) && check_blind ( victim ) )	//Modified by Razgriz	20050908
          ch->position = POS_SLEEPING;
		else {																			//Added by Razgriz	20050908
			victim->position = POS_FIGHTING;
			stop_fighting( ch, FALSE );
			set_fighting ( ch, victim );
		}
		
		WAIT_MAGIC_STATE( ch, 8 * PULSE_VIOLENCE ); //about 16 seconds, added by Razgriz	20050908

    return;
}

// magic_missile modified by jye 6.12.2005
void spell_magic_missile( int sn, int level, CHAR_DATA *ch, void *vo )
{  
  CHAR_DATA *victim ;
  ROOM_INDEX_DATA *	chk_room;
  
  int dam = 0;
  int times = 0;
  int Etimes = 0;
  int sklv = get_skill_level(ch,sn);
  int    obj_lv = level;
  
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
  
  victim = (CHAR_DATA*) vo;
  times = 2;
	if(sklv > 3) times = times + 1;
	if(sklv > 5) times = times + 1;
  while(times)
  {
	 if( number_range(1,5) < Etimes )
		 return;
	 
     if( ch->mana > 20 ){
         ch->mana = ch->mana - 20;
		 Etimes += 1;
	 }
     else
         return;
	 
	 dam = UMIN(ch->level , 60);
	 
     dam = number_range( dam , dam * 1.2 );
	 
	 dam += affected_value(victim, sn, APPLY_DODGE) * 0.33;
	 
	 if( !IS_NPC(ch) )
		 dam += ch->pp * 0.66;
	 
     dam = dam * weakness_damage(victim, sn) * 0.01;

	// 有 mystic armor 不作用負面 2020/10/26
	if( !IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) ){

     if( number_percent() < 10 + sklv * sklv /2 && affected_value(victim, sn, APPLY_DODGE) < 150 ){
		 
         AFFECT_DATA af;
		 af.type     = sn;
		 af.duration  = 1;
		 af.bitvector = 0;
		 af.location  = APPLY_DODGE;
		 af.modifier  = 2;
		 affect_join( victim, &af );
         act(HIC"你的魔法飛彈干擾著"NOR"$N"HIC"的行動。"NOR, ch, NULL, victim, TO_CHAR);
         act("$n"HIC"的魔法飛彈干擾著你的行動。"NOR, ch, NULL, victim, TO_VICT);
         act("$n"HIC"的魔法飛彈干擾著"NOR"$N"HIC"的行動。"NOR, ch, NULL, victim, TO_NOTVICT);
	 }
	}
	 
     if( !IS_NPC(ch) && !IS_NPC(victim) )
         dam *= number_range(4,12);
     
     chk_room = victim->in_room;
     magic__damage( ch, victim, dam, sn, WEAR_NONE );

  	 //modify by jye 0940612
     if( victim->position == POS_DEAD 
         || victim->in_room != chk_room 
         || victim->in_room != ch->in_room )
         return;
         
     times--;
  }
/* old version of MM
  static const int       dam_each [ ] =
  {
     0,
     4,  4,  5,  5,  6,      7,  7,  7,  7,  7,
     8,  8,  8,  8,  8,      9,  9,  9,  9,  9,
     10, 10, 10, 10, 10,
     11, 11, 11, 11, 11,     12, 12, 12, 12, 12,
     13, 13, 13, 13, 13,     14, 14, 14, 14, 14,
     15, 15, 15, 15, 15,     16, 16, 16, 16, 16
  };
  
  CHAR_DATA *victim ;
  ROOM_INDEX_DATA *	chk_room;
  
  int i = 0, sklv = get_skill_level(ch,sn);
  int dam= 0;
  
  victim = (CHAR_DATA*) vo;
  level += sklv;
  dam = level / 6 +  URANGE( 1, level - 37, 3 );
	if(sklv > 3) dam = dam * 1.1;
	if(sklv > 4) dam = dam * 1.1;
	if(sklv > 5) dam = dam * 1.1;
  while(dam)
  {
     if( ch->mana > 3 )
         ch->mana = ch->mana - 3 + sklv/4 + sklv/6;
     else 
         return;
     
     level = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
     level = UMAX( 0, level );
     i = number_range( dam_each[level] , dam_each[level] * 2 );
     //i = i - victim->mdr;

     i = i * weakness_damage(victim, sn) * 0.01;

     if( !IS_NPC(ch) && !IS_NPC(victim) )
         i *= number_range(4,12);
     
     chk_room = victim->in_room;
     magic__damage( ch, victim, i, sn, WEAR_NONE );
     WAIT_MAGIC_STATE( ch, number_range(1,3) );

  	 //modify by jye 0940612
     if( victim->position == POS_DEAD 
         || victim->in_room != chk_room 
         || victim->in_room != ch->in_room )
         return;
         
     dam--;
  }
											*/
	WAIT_MAGIC_STATE( ch, 3 );
  return;
}


void spell_wind_screaming( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam, timer;//timer add by Neversay for endless loop bug.
	ROOM_INDEX_DATA *pRoomIndex;

	dam = number_range( 40, 80 ) * UMAX( 1, WIS_DIFF( ch, victim ) ) * 
	new_saves_spell( ch->pp, victim, MAGIC_WIND ) / 100;

	// Keric modfied to magic_damage
	//dam -= victim->mdr;
	dam = dam * weakness_damage(victim, sn) / 100;

	if( is_flying(victim) )
		dam += dam / 2;

	magic_damage( ch, victim, dam, sn, WEAR_NONE );

	if ( !victim->deleted && victim->position != POS_DEAD &&
	     victim->in_room == ch->in_room 
	     && !( IS_NPC(victim) && IS_SET(victim->act, ACT_SENTINEL))
	     && old_saves_spell( ch->pp, victim, MAGIC_WIND ) - 20 > number_percent( ) )
	{
		if ( is_affected( victim, skill_lookup("prot. from wind"))) {
			send_to_char(HIC"風之障壁替你阻擋了陰風怒嚎的強風"NOR"\n\r", victim);
			return;
		}
		// 有 mystic armor 不作用負面 2020/10/26
		if( IS_AFFECTED(victim, AFF_MYSTIC_ARMOR ) )
			return;
		for ( timer = 0; timer <300; timer++)
		{
			pRoomIndex = get_room_index( number_range( 0, MAX_VNUM ) );
			if ( pRoomIndex ){
				if ( pRoomIndex->area->recall != victim->in_room->area->recall )
					continue;
				if (   !IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE  )
						&& !IS_SET( pRoomIndex->area->area_flags, AREA_CHANGED)
						&& !IS_SET( pRoomIndex->area->area_flags, AREA_BUILDING)
						&& !IS_SET( pRoomIndex->area->area_flags, AREA_CLAN)
						&& !IS_SET( pRoomIndex->room_flags, ROOM_SOLITARY )
						&& !IS_SET( pRoomIndex->room_flags, ROOM_NO_TELEPORT_TO) )
				break;
			}
		}

		//add by Neversay for debug.
		if ( timer >= 300 )
			return;

		if( victim->in_room->progtypes & ROOM_LEAVE )
			rprog_leave_trigger( victim->in_room, victim, 8 );
		if ( victim->fighting )
			stop_fighting( victim, TRUE );
		act( "$n 的身影逐漸消失.", victim, NULL, NULL, TO_ROOM );
		char_from_room( victim );
		char_to_room( victim, pRoomIndex );
		act( "$n 的身影逐漸出現.",   victim, NULL, NULL, TO_ROOM );
		do_look( victim, "auto" );
		if( pRoomIndex->progtypes & ROOM_ENTER)
			rprog_entry_trigger(pRoomIndex, victim, 6);
		if ( victim->position > POS_DEAD ) // Added by Amethyst
			mprog_greet_trigger( victim ); // Added by Amethyst
	}
	return;
}




void spell_meteor_shower( int sn, int level, CHAR_DATA *ch, void *vo )
{
	QMARK *qm;
	char buf[MAX_INPUT_LENGTH];
	CHAR_DATA *vch, *vchNext;
	int sklv    = get_skill_level(ch,sn);
	int nEQSklv = get_skill_level(ch, skill_lookup("earthquake"));
	double dam; // basic value;
	//int old_dam;
	int obj_lv = level;
	// 2022/04/16
	CHAR_DATA *rand_vch;
	int times = 0;
	int target_count = 0;
	int mana_cost = 0;
	int nSpellSklv = get_skill_level(ch, gsn_spellmaster);

	if( level >= 1000){
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

	if( !ch->in_room ) return;

	// meteor shower 修改為每發判定消耗 mana 2022/04/15
	if( !IS_NPC(ch) ){
		if( get_skill_percent( ch , sn ) > 0 )
			mana_cost = 90 - sklv*2;
		else
			mana_cost = 100 - sklv*2;

		// instant 消耗增加
		if(ch->pcdata->autocombat == AUTOC_INSTANT)
			mana_cost = mana_cost * (30 - get_skill_level(ch, gsn_instant_cast) * 2.5) / 10;

		// spellmaster lv 6 效果減少耗費 36%
		mana_cost = mana_cost - (( mana_cost * (nSpellSklv * nSpellSklv / 2 + nSpellSklv * 2 + 6)) / 100 );

		if( ch->mana < mana_cost ){
			send_to_char( "你的法力不夠了.\n\r", ch);
			return;
		}
	}

	if( !IS_AFFECTED(ch, AFF_CAST_ACC) ) {
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		SET_BIT(ch->affected_by, AFF_CAST_ACC);
		// npc 的 del_qmark 必須使用 mpdel_qmark 2022/04/14
		if( !IS_NPC(ch) )
			do_del_qmark(ch, buf);
		else
			do_mpdel_qmark(ch, buf);
	}

	if( !(qm = is_qmark(ch, "cast_accu_mark"))){
		sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
		act( YEL "$n 周圍逐漸形成一團淡黃色的光暈！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你開始集中精神....\n\r", ch);
		do_add_qmark(ch, buf);
		return;
	}else if( qm->v0 != sn ) {
		qm->v0 = sn;
		qm->v1 = 1;
		act( "$n 周圍的光暈逐漸散去..."NOR"", ch, NULL, NULL, TO_ROOM );
		act( YEL "$n 周圍逐漸形成一團淡黃色的光暈！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你釋放了原有的咒文，重新開始集中精神....\n\r", ch);
		return;
	}else if( qm->v1 < 2 -sklv / 6) {
		++qm->v1;
		act( YEL "$n 周圍的淡黃色的光暈愈來愈濃！"NOR"", ch, NULL, NULL, TO_ROOM );
		send_to_char("你的精神逐漸凝聚....\n\r", ch);
		return;
	}
	send_to_char("你釋放出全部的魔法能量！\n\r", ch);
	act( HIY "$n 周圍的淡黃色光暈突然向遙遠的高空飛去！"NOR"", ch, NULL, NULL, TO_ROOM );
	send_to_char( HIY "你頭頂的天空忽然變成了亮紅色，瞬間，陣陣耀眼的高溫光束自天而降！\n\r"NOR"", ch);
	act( "$n"HIY"的正上方閃過一道亮光，緊接著，數不清的光束自天空降下！"NOR"" , ch, NULL, NULL, TO_ROOM );

	sprintf(buf, "self cast_accu_mark -1 %d 1 0 1 0", sn);
	// npc 的 del_qmark 必須使用 mpdel_qmark 2022/04/14
	if( !IS_NPC(ch) )
		do_del_qmark(ch, buf);
	else
		do_mpdel_qmark(ch, buf);
	REMOVE_BIT(ch->affected_by, AFF_CAST_ACC);

	// meteor shower 發數受目標數量影響
	for ( vch = ch->in_room->people ; vch; vch = vchNext ){
		vchNext = vch->next_in_room;

		if ( ch == vch ) continue;
		
		if ( is_same_group( ch, vch ) ) continue;

		if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;
		
		// 魔法免疫 mob 除了 wiz 外, 無法作用任何 spell 2020/10/29
		if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
		{
			//act("你的魔法對$N起不了作用，看來他可能是魔法免疫." , ch, NULL, vch, TO_CHAR);
			continue;
		}
		
		if( IS_NPC(ch) ){
			if( IS_NPC(vch) && vch->fighting != ch )
				continue;
		}else{
			if( !IS_NPC(vch) && vch->fighting != ch )
				continue;
		}
		times++;
	}

	// 發數計算
	if( IS_NPC(ch) )
		times = 1 + (times * sklv * ch->level)/200;
	else
		times = 1 + times + sklv*2;

	if( times < 1 ) return;

	while( times-- )
	{
		target_count = 0;
		rand_vch = NULL;
		for ( vch = ch->in_room->people ; vch; vch = vchNext ){
			vchNext = vch->next_in_room;

			if( !IS_NPC(ch) && ch->mana < mana_cost )
				return;

			if ( ch == vch ) continue;
			
			if ( is_same_group( ch, vch ) ) continue;

			if( !IS_NPC(vch) && IS_SET( vch->act, PLR_WIZINVIS)) continue;

			if( IS_SET( vch->special, ANTI_MAGIC ) && ch->class != WIZARD )
				continue;
			
			if( IS_NPC(ch) ){
				if( IS_NPC(vch) && vch->fighting != ch )
					continue;
			}else{
				if( !IS_NPC(vch) && vch->fighting != ch )
					continue;
			}
			
			// 每發等機率判定可攻擊目標
			if( number_range( 0, target_count ) == 0 )
				rand_vch = vch;

			target_count++;
		}
		// 如果沒有目標就結束攻擊
		if( !rand_vch ) return;

		if ( IS_NPC( ch ) )
		{
			dam = 0;
			//dam = ch->level + sklv + dice( sklv + (ch->level/2)+8+race_table[rand_vch->race].size, 12);
			dam = ch->level + sklv + dice( sklv + (ch->level/2) + 8 + get_curr_size( rand_vch ) , 12); // 替換體型 size 公式 2022/12/23
			dam = dam * weakness_damage( rand_vch, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, rand_vch,MAGIC_EARTH)) / 100;
			// 高傷害浮動
			dam = number_range( dam * 0.4, dam * (2 + sklv/4) );
			magic__damage( ch, rand_vch, dam/2 , sn, WEAR_NONE );
		}
		else
		{
			// 有 mystic armor 不受負面效果影響 2020/10/26
			if( !IS_AFFECTED(rand_vch, AFF_MYSTIC_ARMOR ) ){
				if( sklv > 4 ) {
					if ( IS_AFFECTED( rand_vch, AFF_FLYING ) ){
						affect_strip( rand_vch, skill_lookup("fly"));
						REMOVE_BIT(rand_vch->affected_by, AFF_FLYING);
						send_to_char( "隕石強大的力量將你拉回地面！\n\r", rand_vch);
					}
				}
			}

			//double dam = (40 + level)/2 ;
			dam = 0;
			dam = (40 + ch->level)/2;
			dam = dam * int_effect( ch );
			if( sklv > 0 ) dam = dam * ( 10 + sklv) / 10 ;
			if( sklv > 2 ) dam = dam * 1.05;
			if( sklv > 3 ) dam = dam * 1.05;
			if( sklv > 4 ) dam = dam * 1.06;
			if( sklv > 5 ) dam = dam * 1.06;
			dam = dam * (180 + nEQSklv * (nEQSklv + 4)) / 180;
			dam = dam * weakness_damage( rand_vch, sn ) / 100;
			dam = dam * ( new_saves_spell( ch->pp, rand_vch,MAGIC_EARTH)) / 100;
			//old_dam = dam;
			dam = number_range( dam * (2 + sklv/4), dam * (5 + sklv/2) );

			// 因考慮 spell fury 等因素, 額外 STUN 效果寫在 magic_damage 裡
			magic_damage( ch, rand_vch, dam, sn, WEAR_NONE );
			ch->mana -= mana_cost;
		}
	}
	return;
}
// modified by whisperwind 8.25.2004

void do_cast_delay( CHAR_DATA *ch, char *argument )
{
    send_to_char("什麼？\n\r", ch);
    return;
}
