
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.			   *
 *									   *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and	   *
 *  Mitchell Tse.							   *
 *									   *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.	   *
 *									   *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in	   *
 *  'license.txt', as well as the Envy license in 'license.nvy'.	   *
 *  In particular, you may not remove either of these copyright notices.   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define __ROC_CHINESE_C__
#include "merc.h"
#include "ansi.h"


bool 
is_chinese_char(unsigned char c, bool cFlag)
{
	if( cFlag )
		return FALSE;

	if ( c > '\x7F') // always true: && c <= '\xff' )
		return TRUE;
	
	return FALSE;
}

bool is_brief(CHAR_DATA* ch, CHAR_DATA* victim)
{
    if( !ch ) return ( IS_NPC(victim) ? FALSE : IS_SET(ch->act, PLR_BRIEF));
    if( !victim ) {
        if( IS_NPC(ch) ) 
            return TRUE;
        else {
	          return (IS_SET(ch->act, PLR_BRIEF));
        }
    }
    if( IS_NPC(ch) ) {
        if( IS_NPC(victim) ) 
            return TRUE;
        else{ 
            return (IS_SET(victim->act, PLR_BRIEF));
        }
    }else{
        return (IS_SET(ch->act, PLR_BRIEF));
    }

    return FALSE;
}
bool is_chinese(CHAR_DATA* ch, CHAR_DATA* victim)
{
    if( !ch ) return TRUE;
    if( !victim ) {
	if( IS_NPC(ch) ) return TRUE;
	else {
	    return (IS_SET(ch->act, PLR_CHINESE)) ? TRUE : FALSE;
	}
    }
    if( IS_NPC(ch) ) {
	if( IS_NPC(victim) ) return TRUE;
	else{ 
	    return (IS_SET(victim->act, PLR_CHINESE)) ? TRUE : FALSE;
	}
    }
    else {
	return (IS_SET(ch->act, PLR_CHINESE)) ? TRUE : FALSE;
    }

    return TRUE;
}

void strip_ansi(char* src, char* dup)
{
    int i = 0;
    int j = 0;
    while(src[i] != '\0')
    {
	if( src[i] == '' )
	{
	    while(src[i] != 'm' )
	    {
		i++;	
	    }
	    i++;
	}
	else
	{
	    dup[j++] = src[i++];
	}
    }
    dup[j] = '\0';
}



/*
 * Return ascii name of an item type.
 */
char *c_item_type_name( OBJ_DATA *obj )
{
    OBJ_DATA *in_obj;
    char      buf [ MAX_STRING_LENGTH ];

    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return "����";
    case ITEM_SCROLL:		return "���b";
    case ITEM_WAND:		return "�]��";
    case ITEM_STAFF:		return "�k��";
    case ITEM_WEAPON:		return "�Z��";
    case ITEM_TREASURE:		return "�_��";
    case ITEM_ARMOR:		return "�˥�";
    case ITEM_POTION:		return "�ľ�";
    case ITEM_FURNITURE:	return "�a��";
    case ITEM_TRASH:		return "�U��";
    case ITEM_CONTAINER:	return "�e��";
    case ITEM_DRINK_CON:	return "�����e��";
    case ITEM_KEY:		return "�_��";
    case ITEM_FOOD:		return "����";
    case ITEM_MONEY:		return "����";
    case ITEM_BOAT:		return "�";
    case ITEM_CORPSE_NPC:	return "NPC����";
    case ITEM_CORPSE_PC:	return "���̫���";
    case ITEM_FOUNTAIN:		return "�u��";
    case ITEM_PILL:		return "�ĤY";
    case ITEM_ARROW:		return "�}��";
    case ITEM_DART:		return "�t��";
    case ITEM_HORSE:		return "���M";
	case ITEM_AMMO:		return "�u��"; // 2022/04/29
    }

    for ( in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj )
      ;

    if ( in_obj->carried_by )
      sprintf( buf, "Item_type_name: unknown type %d from %s owned by %s.",
	      obj->item_type, obj->name, obj->carried_by->name );
    else
      sprintf( buf,
	      "Item_type_name: unknown type %d from %s owned by <����>.",
	      obj->item_type, obj->name );

    bug( buf, 0 );
    return "<����>";
}



/*
 * Return ascii name of an affect location.
 */
char *c_affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:            return "�L";
    case APPLY_STR:             return "�O�q";
    case APPLY_DEX:             return "�ӱ�";
    case APPLY_INT:             return "���O";
    case APPLY_WIS:             return "���z";
    case APPLY_CON:             return "���";
    case APPLY_SEX:             return "�ʧO";
    case APPLY_CLASS:           return "¾�~";
    case APPLY_LEVEL:           return "�ŧO";
    case APPLY_AGE:             return "�~��";
    case APPLY_HEIGHT:          return "�髬";
    case APPLY_WEIGHT:          return "�t��";
    case APPLY_MANA:            return "�k�O";
    case APPLY_HIT:             return "�ͩR�O";
    case APPLY_MOVE:            return "���ʤO";
    case APPLY_GOLD:            return "����";
    case APPLY_EXP:             return "�g���";
    case APPLY_AC:              return "���m�O";
    case APPLY_HITROLL:         return "�R���v";
    case APPLY_DAMROLL:         return "�ˮ`�O";
    case APPLY_SAVING_PARA:     return "�·��^�ײv";
    case APPLY_SAVING_LIFE:     return "�ͩR�^�ײv";    // �Ω� mar ���� 2020/10/28
    case APPLY_SAVING_PETRI:    return "�ۤƦ^�ײv";
    case APPLY_SAVING_BREATH:   return "�s���^�ײv";
    case APPLY_SAVING_SPELL:    return "�k�N�^�ײv";
    case APPLY_RACE:            return "�ر�";
    case APPLY_POWER_POTENTIAL: return "�]�k��O";
    case APPLY_DODGE:           return "�{�ײv";
    case APPLY_RDAMAGE:         return "�ܪ��z�ˮ`";
    case APPLY_RMAGIC:          return "���]�k�ˮ`";
    case APPLY_ADAMAGE:         return "���z�ˮ`�O";
    case APPLY_AMAGIC:          return "�]�k�ˮ`�O";
    case APPLY_SAVING_ANCIENT:  return "�W�j�k�N�^�ײv";
    case APPLY_SAVING_SPIRIT:   return "�F��k�N�^�ײv";
    case APPLY_SAVING_FIRE:     return "�����k�N�^�ײv";
    case APPLY_SAVING_COLD:     return "�H�N�k�N�^�ײv";
    case APPLY_SAVING_LIGHTNING:return "�{�q�k�N�^�ײv";
    case APPLY_SAVING_EARTH:    return "�j�a�k�N�^�ײv";
    case APPLY_SAVING_WIND:     return "���k�N�^�ײv";
    case APPLY_SAVING_SAINT:    return "���t�k�N�^�ײv";
    case APPLY_SAVING_DARK:     return "�·t�k�N�^�ײv";
    case APPLY_SAVING_POISON:   return "�@�r�ˮ`";
    case APPLY_SAVING_ELEMENT:  return "�����^�ײv";
    case APPLY_SAVING_GOOD:     return "���}�k�N�^�ײv";
    case APPLY_SAVING_EVIL:     return "���c�k�N�^�ײv";
    case APPLY_DOB_SAVING_FLASH:return "���z��ܤO";
    case APPLY_SAVING_ILLUSION: return "�۳N�^�ײv";
    case APPLY_ACTION_ABILITY:  return "��ʤO";
	case APPLY_WEAPON_SPELL:    return "�k�N�ݩʶˮ`"; // �Z���B�~�l�[�k�N�¤O 2021/09/19
	case APPLY_WEAPON_WIND:     return "���ݩʶˮ`";   // �Z���B�~�l�[���t�¤O
	case APPLY_WEAPON_FIRE:     return "���ݩʶˮ`";   // �Z���B�~�l�[���t�¤O
	case APPLY_WEAPON_COLD:     return "���ݩʶˮ`";   // �Z���B�~�l�[���t�¤O
	case APPLY_WEAPON_EARTH:    return "�a�ݩʶˮ`";   // �Z���B�~�l�[�a�t�¤O
	case APPLY_WEAPON_LIGHTNING:return "�p�ݩʶˮ`";   // �Z���B�~�l�[�p�t�¤O
	case APPLY_WEAPON_SAINT:    return "�t�ݩʶˮ`";   // �Z���B�~�l�[�t�t�¤O
	case APPLY_WEAPON_DARK:     return "�t�ݩʶˮ`";   // �Z���B�~�l�[�t�t�¤O
	case APPLY_MAGIC_IMMUNE:    return "�]�k��K";     // �Ω� mystic armor 2021/11/03
	case APPLY_TOXICITY_POISON: return "�r�ʱj��";     // �Ω� poison item �p�� 2021/11/10
	case APPLY_DAMAGE_COUNT:    return "�֭p�ˮ`��";   // �Ω� blood thirsty �p�� 2022/02/21
	case APPLY_SHOOT_HITROLL:   return "�g���R���v";     // �b�کκj�u�����~�Q�g���ɪ��R���[�� 2022/05/06
	case APPLY_SHOOT_DAMROLL:   return "�g���ˮ`�O";     // �b�کκj�u�����~�Q�g���ɪ��ˮ`�[�� 2022/05/06
	case APPLY_SHOOT_ADAMAGE:   return "�g�����z�ˮ`�O"; // �b�کκj�u�����~�Q�g���ɪ����z�ˮ`�[�� 2022/05/06
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "<����>";
}


/*
 * Return ascii name of an affect bit vector.
 */
char *c_affect_bit_name( long long int vector )
{
    static char buf [ 1024 ];

    buf[0] = '\0';
    if ( vector & AFF_BLIND )           strcat( buf, " ����" );
    if ( vector & AFF_INVISIBLE )       strcat( buf, " ����" );
    if ( vector & AFF_DETECT_EVIL )     strcat( buf, " �������c" );
    if ( vector & AFF_DETECT_INVIS )    strcat( buf, " ��������" );
    if ( vector & AFF_DETECT_MAGIC )    strcat( buf, " �����]�k" );
    if ( vector & AFF_DETECT_HIDDEN )   strcat( buf, " �����ð�" );
    if ( vector & AFF_HOLD )            strcat( buf, " HOLD" );
    if ( vector & AFF_SANCTUARY )       strcat( buf, " �t��" );
    if ( vector & AFF_FAERIE_FIRE )     strcat( buf, " �ѨϤ���" );
    if ( vector & AFF_INFRARED )        strcat( buf, " �]��" );
    if ( vector & AFF_CURSE )           strcat( buf, " �A�G" );
    if ( vector & AFF_TARDY )           strcat( buf, " ��w" );
    if ( vector & AFF_CHANGE_SEX )      strcat( buf, " �ʧO�ܧ�" );
    if ( vector & AFF_POISON )          strcat( buf, " ���r" );
    if ( vector & AFF_PROTECT )         strcat( buf, " �Ѩϥ[�@" );
    if ( vector & AFF_POLYMORPH )       strcat( buf, " �ر��ܧ�" );
    if ( vector & AFF_SLEEP )           strcat( buf, " �I��" );
    if ( vector & AFF_SNEAK )           strcat( buf, " ���" );
    if ( vector & AFF_HIDE )            strcat( buf, " �ð�" );
    if ( vector & AFF_CHARM )           strcat( buf, " �g��" );
    if ( vector & AFF_FLYING )          strcat( buf, " ����" );
    if ( vector & AFF_WATERWALK)        strcat( buf, " �����樫" );
    if ( vector & AFF_PASS_DOOR )       strcat( buf, " �b�z��" );
    if ( vector & AFF_MUTE )            strcat( buf, " �I�q" );
    if ( vector & AFF_SEAL_MAGIC )      strcat( buf, " �ʩG" );
	// if ( vector & AFF_GILLS )        strcat( buf, " gills" );
    if ( vector & AFF_VAMP_BITE )       strcat( buf, " �l�尭���k" );
    if ( vector & AFF_GHOUL )           strcat( buf, " �����ڶA�G" ); // 2022/01/16
    if ( vector & AFF_FLAMING )         strcat( buf, " ����" );
    if ( vector & AFF_PARALYZED )       strcat( buf, " �·�" );
    if ( vector & AFF_CRAZY )           strcat( buf, " �ƨg" );
    if ( vector & AFF_WEAKEN )          strcat( buf, " �I�z" );
    if ( vector & AFF_NIGHTSTALK )      strcat( buf, " �t�]����" );
	if ( vector & AFF_DISEASE )         strcat( buf, " �ͯf" );
	if ( vector & AFF_FEAR )            strcat( buf, " ����" );
    return ( buf[0] != '\0' ) ? buf+1 : "�L";
}



/*
 * Return ascii name of extra flags vector.
 */
char *c_extra_bit_name( int extra_flags )
{
    static char buf [ 1024 ];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW	 ) strcat( buf, " �o��"		);
    if ( extra_flags & ITEM_HUM		 ) strcat( buf, " �C��"		);
    if ( extra_flags & ITEM_DARK	 ) strcat( buf, " ���"		);
    if ( extra_flags & ITEM_LOCK	 ) strcat( buf, " �W��"		);
    if ( extra_flags & ITEM_EVIL	 ) strcat( buf, " ���c"		);
    if ( extra_flags & ITEM_INVIS	 ) strcat( buf, " ����"	);
    if ( extra_flags & ITEM_MAGIC	 ) strcat( buf, " �]�O"	);
    if ( extra_flags & ITEM_NODROP	 ) strcat( buf, " �L�k���"	);
    if ( extra_flags & ITEM_BLESS	 ) strcat( buf, " ����"	);
    if ( extra_flags & ITEM_ANTI_GOOD	 ) strcat( buf, " �ϵ��}"	);
    if ( extra_flags & ITEM_ANTI_EVIL	 ) strcat( buf, " �Ϩ��c"	);
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " �Ϥ���" );
    if ( extra_flags & ITEM_NOREMOVE	 ) strcat( buf, " �L�k����"	);
    if ( extra_flags & ITEM_INVENTORY	 ) strcat( buf, " ���~"	);
    if ( extra_flags & ITEM_POISONED	 ) strcat( buf, " �@�r"	);
    if ( extra_flags & ITEM_VAMPIRE_BANE ) strcat( buf, " BANE" );
    if ( extra_flags & ITEM_HOLY	 ) strcat( buf, " ���t"		);
    if ( extra_flags & ITEM_NOSAVE) strcat( buf, " �L�k�x�s"		);
    if ( extra_flags & ITEM_UNIQUE) strcat( buf, " �W�@�L�G"		);
    if ( extra_flags & ITEM_LORE) strcat( buf, " ��@"		);
    if ( extra_flags & ITEM_NOSAC) strcat( buf, " �L�k�^�m"		);
    if ( extra_flags & ITEM_PERSONAL) strcat( buf, " �ӤH�M��"		);
    if ( extra_flags & ITEM_RECOVER) strcat( buf, " ���l�״_"		);
    if ( extra_flags & ITEM_NO_MALE) strcat( buf, " �k�ʸT��"		);
    if ( extra_flags & ITEM_NO_FEMALE) strcat( buf, " �k�ʸT��"		);
    if ( extra_flags & ITEM_NO_NEUTRAL) strcat( buf, " ���ʸT��"		);
	if ( extra_flags & ITEM_SPECIAL) strcat( buf, " ¾�~����"	);
	if ( extra_flags & ITEM_SPELL) strcat( buf, " ���]"	); // ITEM_SPELL add at 2021/09/19
//	if ( extra_flags & ITEM_MALE) strcat( buf, "�k�ʱM��"		);
//	if ( extra_flags & ITEM_FEMALE) strcat( buf, "�k�ʱM��"		);
//	if ( extra_flags & ITEM_NEUTRAL) strcat( buf, "���ʱM��"	);
    return ( buf[0] != '\0' ) ? buf+1 : "�L";
}


char *	const			c_title_table [ MAX_CLASS ][ MAX_LEVEL+1 ][ 2 ] =
{
    {
	{ "Man",			"Woman"				},

	{ "�]�k�Ǯ{",	"�]�k�Ǯ{"		},
	{ "�]�k�Ǯ{",		"�]�k�Ǯ{"			},
	{ "�]�k�ǲߪ�",		"�]�k�ǲߪ�"		},
	{ "�]�k��s��",		"�]�k��s��"		},
	{ "�]�k�F�C",		"�]�k�F�C"		},

	{ "�]�k�ۼg��",		"�]�k�ۼg��"		},
	{ "����",			"�k����"			},
	{ "����",			"����"				},
	{ "�۳N�h",		"�۳N�h"			},
	{ "�G�N�h",			"�G�N�h"			},

	{ "���N�h",			"���N�h"			},
	{ "�]�ɤh",			"�]�ɤh"			},
	{ "�l��h",			"�l��h"			},
	{ "�]�N�v",			"�]�N�v"				},
	{ "�гy��",			"�гy��"			},

	{ "���",			"���"			},
	{ "�k�v",			"�k�k�v"			},
	{ "�Ův",			"�k��"			},
	{ "�j�k�v",			"�j�k�v"			},
	{ "�]�k�v",			"�]�k�v"			},

	{ "�]�k����",		"�]�k����"		},
	{ "�����]�k����",		"�����]�k����"		},
	{ "�j�]�k�v",		"�j�]�k�v"		},
	{ "�����s�y��",		"�����s�y��"			},
	{ "�����s�y�j�v",	"�����s�y�j�v"		},

	{ "�]�ۻs�y��",		"�]�ۻs�y��",		},
	{ "�]�Ļs�y��",		"�]�Ļs�y��",		},
	{ "���b�s�y��",		"���b�s�y��",		},
	{ "�k���s�y��",		"�k���s�y��",		},
	{ "�v���s�y��",		"�v���s�y��",		},

	{ "�c�]�l��h",		"�c�]�l��h"		},
	{ "�c�]�l��v",	"�c�]�l��v"	},
	{ "�y�s�N�h",		"�y�s�N�h"		},
	{ "�y�s�N�v",	"�y�s�N�v"	},
	{ "�]�k�j�v",	"�]�k�j�v"		},

	{ "�]�k�j�v",	"�]�k�j�v"		},
	{ "�]�k�j�v",	"�]�k�j�v"		},
	{ "�]�k�j�v",	"�]�k�j�v"		},
	{ "�k�v�^��",			"�k�k�v�^��"			},
	{ "�k�v�^��",			"�k�k�v�^��"			},

	{ "�k�v�^��",			"�k�k�v�^��"			},
	{ "�k�v�^��",			"�k�k�v�^��"			},
	{ "�]�k�Ѩ�",		"�]�k�Ѩ�"		},
	{ "�]�k����",		"�]�k����"		},

	{ "�]�k�j��",		"�]�k�j��"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "�y���D",		"�Х@�k��"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "�H�{",			"�k�H�{"			},
	{ "�ͱq",			"�ͱq"			},
	{ "�͹�",			"�͹�"			},
	{ "�s�i�Ю{",			"�s�i�Ю{"			},
	{ "�ǱФh",			"�ǱФh"			},

	{ "�Фh��",			"�Фh��"				},
	{ "����",			"�k����"			},
	{ "�q��",			"�q��"			},
	{ "���q",			"�k���q"			},
	{ "���v",			"���v"			},

	{ "�Ѩ�",			"�Ѩ�"				},
	{ "�Ѩƪ�",			"�Ѩƪ�"			},
	{ "����",			"����"			},
	{ "�פh",			"�פk"				},
	{ "���",			"�k���"			},

	{ "�������v",			"�������v"			},
	{ "�ĹD��",			"�ĹD��"			},
	{ "�D��",			"�D��"			},
	{ "�j�D��",		"�j�D��"	},
	{ "�`�D��",			"�`�D��"			},

	{ "�D�Ъ���",			"�D�Ъ���"		},
	{ "�j�D�Ъ���",			"�j�D�Ъ���"		},
	{ "�`�D�Ъ���",			"�`�D�Ъ���"		},
	{ "���]��",		"���]��"			},
	{ "���]�t��",	"���]�t��"		},

	{ "�����Ͼ��D��",	"�����Ͼ��D��"		},
	{ "�j�a���Ͼ��D��",	"�j�a���Ͼ��D��"		},
	{ "�����Ͼ��D��",	"�����Ͼ��D��"		},
	{ "�A�Ӷ��Ͼ��D��",	"�A�Ӷ��Ͼ��D��"		},
	{ "�Ѧ�Ͼ��D��",	"�Ѧ�Ͼ��D��"	},

	{ "�����t��",	"�����t��"		},
	{ "���t�ƨ�",		"���t�ƨ�"		},
	{ "���t�ƨ�",		"���t�ƨ�"		},
	{ "�ѯ����@",	"�ѯ����@"	},
	{ "���t�]�k�j�v",	"���t�]�k�j�v"	},

	{ "���t�]�k�j�v",	"���t�]�k�j�v"	},
	{ "���t�]�k�j�v",	"���t�]�k�j�v"	},
	{ "���t�]�k�j�v",	"���t�]�k�j�v"	},
	{ "���v�^��",			"���v�k�^��"			},
	{ "���v�^��",			"���v�k�^��"			},

	{ "���v�^��",			"���v�k�^��"			},
	{ "���v�^��",			"���v�k�^��"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "���",			"���"			},
	{ "�d���s",			"�d���s"			},
	{ "�p��",			"�k�p��"			},
	{ "����",		"����"			},
	{ "�s��",			"�s��"				},

	{ "�Ѹ�",			"�Ѹ�"			},
	{ "���ť���",			"���ť���"			},
	{ "�j�s",			"�j�s"			},
	{ "�F�l",			"�k�F�l"			},
	{ "�y�]",			"�y�]"			},
	{ "�c��",			"�c��"				},

	{ "�s��",			"�s��"			},
	{ "����",			"����"			},
	{ "�]��s",			"�]��s"			},
	{ "����",			"����"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "����",			"����"			},
	{ "�s��",			"�s��"			},
	{ "�j�s",			"�j�s"			},

	{ "����",			"����"				},
	{ "�Ѥⶡ��",			"�Ѥⶡ��"			},
	{ "���Ҥj�v",			"���Ҥj�v"			},
	{ "���",			"���"			},
	{ "���Ũ��",			"���Ũ��"			},

	{ "�s���M�a",		"�s���M�a"		},
	{ "��ť�M�a",		"��ť�M�a"		},
	{ "�}��M�a",		"�}��M�a"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "�Ǹo����",	"�Ǹo����"	},
	{ "�Ǹo�M�a",	"�Ǹo�M�a"	},
	{ "�Ǹo�j�v",	"�Ǹo�j�v"	},
	{ "�Ǹo����",			"�Ǹo���Z"		},
	{ "�µ�Ф�",			"�µ�Х�"			},
	{ "�µ�Ф�",			"�µ�Х�"			},
	{ "�µ�Ф�",			"�µ�Х�"			},
	{ "�µ�Ф�",			"�µ�Х�"			},

	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Thief Hero",		"Thief Heroine"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "�C�N���ߥ�",			"�C�N���ߥ�"			},
	{ "�s�L",			"�s�L"			},
	{ "�B��",			"�B��"			},
	{ "�Ԥh",			"�k�Ԥh"			},
	{ "�h�L",			"�k�L"			},

	{ "�Z�h",			"�k�Z�h"			},
	{ "�ѧL",			"�ѧL"			},
	{ "�C��",			"�C��"			},
	{ "���C��",			"���C��"			},
	{ "���h",			"�k���h"			},

	{ "�^��",			"�k�^��"			},
	{ "�i�h",			"�k�i�h"			},
	{ "�g�C�h",		"�g�C�h"		},
	{ "�ħL",			"�k�ħL"			},
	{ "�C�v",		"�k�C�v"			},

	{ "�ƭ���",			"�ƭ���"			},
	{ "����",			"�k����"			},
	{ "�s�M�L",			"�s�M�L"			},
	{ "�s�M�N",			"�s�M�N"			},
	{ "���M�h",			"�k�M�h"			},

	{ "�W���M�h",		"�W���M�h"			},
	{ "�M�h��",		"�M�h��"			},
	{ "�t���M�h",			"�t���M�h"			},
	{ "�t���T��",		"�t���T��"			},
	{ "���]��",		"���]��"			},

	{ "�W�찣�]��",	"�W�찣�]��"		},
	{ "�O�s��",		"�O�s��"			},
	{ "�O�s�i��",	"�O�s�i��"		},
	{ "�ԱN",			"�ԱN"			},
	{ "�s�ʾԱN",			"�s�ʾԱN"			},

	{ "�p��k��",		"�p��k��"		},
	{ "���ɤl��",		"���ɤk�l��"		},
	{ "�g���B��",		"�g���k�B��"		},
	{ "�����J��",	"�����k�J��"	},
	{ "�y�P����",		"�y�P�k����"		},

	{ "�F��F����","�F��F�k����"		},
	{ "����ܤ���","����ܤk����"	},
	{ "�_�m������","�_�m���k����"		},
	{ "�n�ϪL����",	"�n�ϪL�k����"		},
	//{ "Knight of the Black Rose",	"Baroness of Meteors"		},

//	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
	//{ "Knight of the White Rose",	"Baroness of Meteors"		},
	{ "�^���Ԥh",   "�^���k�Ԥh"},
	{ "�^���Ԥh",   "�^���k�Ԥh"},
	{ "�^���Ԥh",   "�^���k�Ԥh"},
	/*{ "Knight of the Silver Moon",	"Baroness of Meteors"		},
	{ "Knight of the Golden Sun",	"Baroness of Meteors"		},
	{ "Squire of the Realm",	"Baroness of Meteors"		},

	{ "Herald of the Realm",	"Baroness of Meteors"		},
	{ "Knight of the Realm",	"Baroness of Meteors"		},
	{ "Lord of the Realm",		"Baroness of Meteors"		},
	{ "Highlord of the Realm",	"Baroness of Meteors"		},
	{ "Knight Hero",		"Knight Heroine"		},
*/
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "�C�N���ߥ�",			"�C�N���ߥ�"			},
	{ "�s�L",			"�s�L"			},
	{ "�B��",			"�B��"			},
	{ "�Ԥh",			"�k�Ԥh"			},
	{ "�h�L",			"�k�L"			},

	{ "�Z�h",			"�k�Z�h"			},
	{ "�ѧL",			"�ѧL"			},
	{ "�C��",			"�C��"			},
	{ "���C��",			"���C��"			},
	{ "���h",			"�k���h"			},

	{ "�^��",			"�k�^��"			},
	{ "�i�h",			"�k�i�h"			},
	{ "�g�C�h",		"�g�C�h"		},
	{ "�ħL",			"�k�ħL"			},
	{ "�C�v",		"�k�C�v"			},

	{ "�ƭ���",			"�ƭ���"			},
	{ "����",			"�k����"			},
	{ "�s�M�L",			"�s�M�L"			},
	{ "�s�M�N",			"�s�M�N"			},
	{ "���M�h",			"�k�M�h"			},

	{ "�W���M�h",		"�W���M�h"			},
	{ "�M�h��",		"�M�h��"			},
	{ "�t���M�h",			"�t���M�h"			},
	{ "�t���T��",		"�t���T��"			},
	{ "���]��",		"���]��"			},

	{ "�W�찣�]��",	"�W�찣�]��"		},
	{ "�O�s��",		"�O�s��"			},
	{ "�O�s�i��",	"�O�s�i��"		},
	{ "�ԱN",			"�ԱN"			},
	{ "�s�ʾԱN",			"�s�ʾԱN"			},

	{ "�p��k��",		"�p��k��"		},
	{ "���ɤl��",		"���ɤk�l��"		},
	{ "�g���B��",		"�g���k�B��"		},
	{ "�����J��",	"�����k�J��"	},
	{ "�y�P����",		"�y�P�k����"		},

	{ "�F��F����","�F��F�k����"		},
	{ "����ܤ���","����ܤk����"	},
	{ "�_�m������","�_�m���k����"		},
	{ "�n�ϪL����",	"�n�ϪL�k����"		},
	//{ "Knight of the Black Rose",	"Baroness of Meteors"		},

//	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
	//{ "Knight of the White Rose",	"Baroness of Meteors"		},
	{ "�^���Z�L",   "�^���k�Z�L"},
	{ "�^���Z�L",   "�^���k�Z�L"},
	{ "�^���Z�L",   "�^���k�Z�L"},
	/*{ "Knight of the Silver Moon",	"Baroness of Meteors"		},
	{ "Knight of the Golden Sun",	"Baroness of Meteors"		},
	{ "Squire of the Realm",	"Baroness of Meteors"		},

	{ "Herald of the Realm",	"Baroness of Meteors"		},
	{ "Knight of the Realm",	"Baroness of Meteors"		},
	{ "Lord of the Realm",		"Baroness of Meteors"		},
	{ "Highlord of the Realm",	"Baroness of Meteors"		},
	{ "Knight Hero",		"Knight Heroine"		},
*/
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Psychic",			"Psychic",			},
	{ "Medium",			"Medium",			},
	{ "Gypsy",			"Gypsy",			},
	{ "Meditator",			"Meditator",			},
	{ "Mind Prober",		"Mind Prober",			},

	{ "Soul Searcher",		"Soul Searcher",		},
	{ "Astral Voyager",		"Astral Voyager",		},
	{ "Seeker",			"Seeker",			},
	{ "Empath",			"Empath",			},
	{ "Mind Reader",		"Mind Reader"			},

	{ "Telepath",			"Telepath",			},
	{ "Mental Adept",		"Mental Adept",			},
	{ "Spoonbender",		"Spoonbender",			},
	{ "Perceptive",			"Perceptive",			},
	{ "Clever",			"Clever",			},

	{ "Wise",			"Wise",				},
	{ "Genius",			"Genius",			},
	{ "Oracle",			"Oracle",			},
	{ "Soothsayer",			"Soothsayer",			},
	{ "Truthteller",		"Truthteller",			},

	{ "Sage",			"Sage",				},
	{ "Master Psychic",		"Mistress Psychic",		},
	{ "Master Meditator",		"Mistress Meditator",		},
	{ "Master Empath",		"Mistress Empath",		},
	{ "Master Clairvoyant",		"Mistress Clairvoyant",		},

	{ "Master Mind Reader",		"Mistress Mind Reader",		},
	{ "Master Telepath",		"Mistress Telepath",		},
	{ "Master Spoonbender",		"Mistress Spoonbender",		},
	{ "Grand Master Psychic",	"Grand Mistress Psychic",	},
	{ "Grand Master Meditator",	"Grand Mistress Meditator",	},

	{ "Grand Master Empath",	"Grand Mistress Empath",	},
	{ "Grand Master Clairvoyant",	"Grand Mistress Clairvoyant",	},
	{ "Grand Master Mind Reader",	"Grand Mistress Mind Reader",	},
	{ "Grand Master Telepath",	"Grand Mistress Telepath",	},
	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},

	{ "�j�k�v",	"�k�j�k�v",	},
	{ "�Ѥ~�j�k�v",	"���L���~��",	},
	{ "�Ův",		"�k�Ův",		},
	{ "�j�Ův",		"�k�j�Ův",		},
	{ "�ԧ�",		"�k�ԧ�",		},
	{ "�Ұ�ԧ�",		"���k���~��",		},
	{ "�U���޾ɪ�",		"�U���޾ɪ�",		},

	{ "Psionicist Angel",		"Psionicist Angel"		},
	{ "Deity of Psionics",		"Deity of Psionics"		},

	{ "Supreme Psionicst",		"Supreme Psionicist"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Psychic",			"Psychic",			},
	{ "Medium",			"Medium",			},
	{ "Gypsy",			"Gypsy",			},
	{ "Meditator",			"Meditator",			},
	{ "Mind Prober",		"Mind Prober",			},

	{ "Soul Searcher",		"Soul Searcher",		},
	{ "Astral Voyager",		"Astral Voyager",		},
	{ "Seeker",			"Seeker",			},
	{ "Empath",			"Empath",			},
	{ "Mind Reader",		"Mind Reader"			},

	{ "Telepath",			"Telepath",			},
	{ "Mental Adept",		"Mental Adept",			},
	{ "Spoonbender",		"Spoonbender",			},
	{ "Perceptive",			"Perceptive",			},
	{ "Clever",			"Clever",			},

	{ "Wise",			"Wise",				},
	{ "Genius",			"Genius",			},
	{ "Oracle",			"Oracle",			},
	{ "Soothsayer",			"Soothsayer",			},
	{ "Truthteller",		"Truthteller",			},

	{ "Sage",			"Sage",				},
	{ "Master Psychic",		"Mistress Psychic",		},
	{ "Master Meditator",		"Mistress Meditator",		},
	{ "Master Empath",		"Mistress Empath",		},
	{ "Master Clairvoyant",		"Mistress Clairvoyant",		},

	{ "Master Mind Reader",		"Mistress Mind Reader",		},
	{ "Master Telepath",		"Mistress Telepath",		},
	{ "Master Spoonbender",		"Mistress Spoonbender",		},
	{ "Grand Master Psychic",	"Grand Mistress Psychic",	},
	{ "Grand Master Meditator",	"Grand Mistress Meditator",	},

	{ "Grand Master Empath",	"Grand Mistress Empath",	},
	{ "Grand Master Clairvoyant",	"Grand Mistress Clairvoyant",	},
	{ "Grand Master Mind Reader",	"Grand Mistress Mind Reader",	},
	{ "Grand Master Telepath",	"Grand Mistress Telepath",	},
	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},

	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},
	{ "Grand Master Spoonbender",	"Grand Mistress Spoonbender",	},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "Alchemist Hero",		"Alchemist Herione",		},
	{ "�гy��",		"�гy��",		},
	{ "Psionicist Angel",		"Psionicist Angel"		},
	{ "Deity of Psionics",		"Deity of Psionics"		},

	{ "Supreme Psionicst",		"Supreme Psionicist"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "�k�H",			"�k�H"				},

	{ "�M��",			"�M��"			},
	{ "�ͱq",			"�ͱq"			},
	{ "�F��",			"�k�F��"			},
	{ "�s�L",			"�k�L"			},
	{ "�ǱФh",			"�k�ǱФh"			},

	{ "����",			"�k����"				},
	{ "�ƥq��",			"�k�ƥq��"			},
	{ "�N�z���v",			"�N�z���v"			},
	{ "���v",			"�k���v"			},
	{ "���",			"�k���"			},

	{ "�з|�Ѩ�",			"�з|�Ѩ�"				},
	{ "�����ƥD��",			"�����k�ƥD��"			},
	{ "�U�z����",			"�U�z����"			},
	{ "�¦�פh",			"����פk"				},
	{ "���",			"�k���"			},

	{ "�H�x���v",			"�H�x�k���v"			},
	{ "�ѹD��",			"�k�ѹD��"			},
	{ "�D��",			"�D��"			},
	{ "�����D��",		"�k�����D��"	},
	{ "�j�D��",			"�k�j�D��"			},

	{ "����j�D��",		"����k�j�D��"		},
	{ "�ХD",		"�k�ХD"		},
	{ "�j�ХD",		"�k�j�ХD"		},
	{ "�c�]����",		"�c�]����"			},
	{ "�j�c�]����",	"�j�c�]����"		},

	{ "���v�Ͼ��D��",	"���v�Ͼ��D��"		},
	{ "�j�a�Ͼ��D��",	"�j�a�Ͼ��D��"		},
	{ "�j��Ͼ��D��",	"�j��Ͼ��D��"		},
	{ "�H�ӼϾ��D��",	"�H�ӼϾ��D��"		},
	{ "�Ѱ�Ͼ��D��",	"�Ѱ�Ͼ��D��"	},

	{ "������",	"������"		},
	{ "�²b������",		"�²b������"		},
	{ "�t��",	"�t��"		},
	{ "�y���t��",	"�y���t��"	},
	{ "�²b�t��",	"�²b�t��"	},

	{ "���a�t��",	"���a�t��"	},
	{ "�����t��",	"�����t��"	},
	{ "�j�����t��",	"�j�����t��"	},
	{ "�P�������",			"�P�������"			},
	{ "��M�����",			"��M�����"			},
	{ "��������",			"��������"			},
	{ "�ϥ@�t�D",			"�ϥ@�t�D"			},
	{ "�Ѩ�",			"�Ѩ�"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "�k�H",			"�k�H"				},

	{ "�M��",			"�M��"			},
	{ "�ͱq",			"�ͱq"			},
	{ "�F��",			"�k�F��"			},
	{ "�s�L",			"�k�L"			},
	{ "�ǱФh",			"�k�ǱФh"			},

	{ "����",			"�k����"				},
	{ "�ƥq��",			"�k�ƥq��"			},
	{ "�N�z���v",			"�N�z���v"			},
	{ "���v",			"�k���v"			},
	{ "���",			"�k���"			},

	{ "�з|�Ѩ�",			"�з|�Ѩ�"				},
	{ "�����ƥD��",			"�����k�ƥD��"			},
	{ "�U�z����",			"�U�z����"			},
	{ "�¦�פh",			"����פk"				},
	{ "���",			"�k���"			},

	{ "�H�x���v",			"�H�x�k���v"			},
	{ "�ѹD��",			"�k�ѹD��"			},
	{ "�D��",			"�D��"			},
	{ "�����D��",		"�k�����D��"	},
	{ "�j�D��",			"�k�j�D��"			},

	{ "����j�D��",		"����k�j�D��"		},
	{ "�ХD",		"�k�ХD"		},
	{ "�j�ХD",		"�k�j�ХD"		},
	{ "�c�]����",		"�c�]����"			},
	{ "�j�c�]����",	"�j�c�]����"		},

	{ "���v�Ͼ��D��",	"���v�Ͼ��D��"		},
	{ "�j�a�Ͼ��D��",	"�j�a�Ͼ��D��"		},
	{ "�j��Ͼ��D��",	"�j��Ͼ��D��"		},
	{ "�H�ӼϾ��D��",	"�H�ӼϾ��D��"		},
	{ "�Ѱ�Ͼ��D��",	"�Ѱ�Ͼ��D��"	},

	{ "������",	"������"		},
	{ "�²b������",		"�²b������"		},
	{ "�t��",	"�t��"		},
	{ "�y���t��",	"�y���t��"	},
	{ "�²b�t��",	"�²b�t��"	},

	{ "���a�t��",	"���a�t��"	},
	{ "�����t��",	"�����t��"	},
	{ "�j�����t��",	"�j�����t��"	},
	{ "�ӤѦ�Ͼ��D��",			"�ӤѦ�Ͼ��D��"			},
	{ "�Щv",			"�Щv"			},
	{ "�ƱЬ�",			"�ƱЬ�"			},
	{ "�Ь�",			"�k�Ь�"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
	{ "Master Assassin",		"Master Assassin "		},
	{ "Master Assassin",		"Master Assassin "		},
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "�ۥ@����",		"�ۥ@����"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Ninja Master",		"Ninja Mistress"		},
	{ "Lord of Shadow",		"Lady of Shadow"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "�]�Z�M�h","�����M�h"		},
	{ "�����M�h","�Y���M�h"	},
	{ "�t�C�M�h","�����M�h"		},
	{ "�ӫa�M�h",	"�����M�h"		},
	{ "�V�P�M�h",   "�V�P�M�h"},
	{ "�Ѧ�V�P�M�h",   "�Ѧ�V�P�M�h"},
	{ "�ӤѦ�V�P�M�h",   "�ӤѦ�V�P�M�h"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "�j�a�C�h","�j�a�C�h"		},
	{ "�K���C�h","�K���C�h"	},
	{ "�Q�ӼC�h","�Q�ӼC�h"		},
	{ "�ƭ��C�h",	"�ƭ��C�h"		},
	{ "�C�t",   "�k�C�t"},
	{ "�Ѧ�C�t",   "�Ѧ�k�C�t"},
	{ "�ӤѦ�C�t",   "�ӤѦ�k�C�t"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "�F��",			"�F��"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "�øg���@�k","�øg���@�k"		},
	{ "�F���󭺮y",	"�F���󭺮y"		},
	{ "�٫߰|���y","�٫߰|���y"	},
	{ "ù�~�󭺮y","ù�~�󭺮y"		},
	{ "���ù�~",   "���ù�~"},
	{ "���sù�~",   "���sù�~"},
	{ "�j��ù�~",   "�j��ù�~"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "�����B��",	"�����k��"	},
	{ "�k�P�B��",		"�k�P�k��"		},

	{ "�F���B��","�F���k��"		},
	{ "��B�s�B��","��B�s�k��"	},
	{ "�_�j�z�B��","�_�j�z�k��"		},
	{ "�n�j�v�B��",	"�n�j�v�k��"		},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Shaman Hero",   "Shaman Heroine"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },
    
    {
	{ "Man",			"Woman"				},

	{ "Apprentice of Magic",	"Apprentice of Magic"		},
	{ "Spell Student",		"Spell Student"			},
	{ "Scholar of Magic",		"Scholar of Magic"		},
	{ "Delver in Spells",		"Delveress in Spells"		},
	{ "Medium of Magic",		"Medium of Magic"		},

	{ "Scribe of Magic",		"Scribess of Magic"		},
	{ "Seer",			"Seeress"			},
	{ "Sage",			"Sage"				},
	{ "Illusionist",		"Illusionist"			},
	{ "Abjurer",			"Abjuress"			},

	{ "Invoker",			"Invoker"			},
	{ "Enchanter",			"Enchantress"			},
	{ "Conjurer",			"Conjuress"			},
	{ "Magician",			"Witch"				},
	{ "Creator",			"Creator"			},

	{ "Savant",			"Savant"			},
	{ "Magus",			"Craftess"			},
	{ "Wizard",			"Wizard"			},
	{ "Warlock",			"War Witch"			},
	{ "Sorcerer",			"Sorceress"			},

	{ "Elder Sorcerer",		"Elder Sorceress"		},
	{ "Grand Sorcerer",		"Grand Sorceress"		},
	{ "Great Sorcerer",		"Great Sorceress"		},
	{ "Golem Maker",		"Golem Maker"			},
	{ "Greater Golem Maker",	"Greater Golem Maker"		},

	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Master of all Magic",	"Master of all Magic"		},

	{ "Master of all Magic",	"Master of all Magic"		},
	{ "Master of all Magic",	"Master of all Magic"		},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Summoner Hero",			"Summoner Heroine"			},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},

	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},

	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},

	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},
	{ "Monk",			"Nun"				},
	{ "Healer",			"Healess"			},

	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},

	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},

	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},

	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},

	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Champion of the Realm",			"Champion of the Realm"			},
	{ "Lord of the Realm",			"Lady of the Realm"			},
	{ "King of the Realm",			"Queen of the Realm"			},
	{ "Guadian of the Realm",			"Guadian of the Realm"			},
	{ "Angel",			"Angel"				},
	{ "Deity",			"Deity"				},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Ranger Hero",		"Ranger Heroine"		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Deity of Assassins",		"Deity of Assassins"		},

	{ "Supreme Master",		"Supreme Mistress"		},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "�����B��",	"�����k��"	},
	{ "�k�P�B��",		"�k�P�k��"		},

	{ "�F���B��","�F���k��"		},
	{ "��B�s�B��","��B�s�k��"	},
	{ "�_�j�z�B��","�_�j�z�k��"		},
	{ "�n�j�v�B��",	"�n�j�v�k��"		},
	{ "�ԯ�",   "�k�ԯ�"},
	{ "�ԯ�",   "�k�ԯ�"},
	{ "�ԯ�",   "�k�ԯ�"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    },    

    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Paladin",			"Paladin"			},
	{ "Grand Paladin",		"Grand Paladin"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "�����B��",   "�����k��"	},
	{ "�k�P�B��",	    "�k�P�k��"	    },

	{ "�F���B��","�F���k��"	    },
	{ "��B�s�B��","��B�s�k��" },
	{ "�_�j�z�B��","�_�j�z�k��"	    },
	{ "�n�j�v�B��",	"�n�j�v�k��"	    },

	{ "�Z��",   "�k�Z��"},
	{ "�Z��",   "�k�Z��"},
	{ "�Z��",   "�k�Z��"},
	{ "Angel of War",		"Angel of War"			},
	{ "Deity of War",		"Deity of War"			},

	{ "Supreme Master of War",	"Supreme Mistress of War"	},
	{ "�y���D",		"�Х@�k��"			},
	{ "Implementor",		"Implementress"			}
    }
};
