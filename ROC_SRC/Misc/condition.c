
#include <stdio.h>
#include "merc.h"

bool
show_position_to_char(CHAR_DATA* ch) 
{
	switch( ch->position )
	{
		case POS_DEAD:
			send_to_char("�R�R���ۧa�A�A�w�g���F.\n\r", ch );
			return TRUE;

		case POS_MORTAL:
		case POS_INCAP:
			send_to_char("�A�����p���V�F.\n\r", ch );
			return TRUE;

		case POS_STUNNED:
			send_to_char("�A�Y������.\n\r", ch );
			return TRUE;

		case POS_PARALYZED:
			send_to_char("�A�·��F.\n\r", ch );
			return TRUE;

		case POS_SLEEPING:
			send_to_char("�A���b�ιڤ��A�Х� wake �_�ӡC\n\r", ch );
			return TRUE;

		case POS_RESTING:
			send_to_char("��...�Aı�o���I�i��...(�A������'��'�_��)\n\r", ch );
			return TRUE;

		case POS_FIGHTING:
			send_to_char( "����I  �A�٦b�԰����I\n\r", ch );
			return TRUE;

	}
	return FALSE;
}
