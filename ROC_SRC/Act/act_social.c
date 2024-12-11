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
//*****************************************************************
//*  PROGRAM ID  : act_social.c                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 Razgriz                                *
//*  PURPOSE     :         �ץ� check_social()                    *
//*              :         �b�԰�����O���ж������a�� social      *
//*              :         �ɭP mob ��譱�Ѱ��԰�                *
//*****************************************************************
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
#include "merc.h"
#include "ansi.h"


bool	
check_social	args( ( CHAR_DATA *ch, char *command, char *argument ) );


/* New socials contributed by Katrina and Binky */
const	struct	social_type	social_table [] =
{
      {
	"?",
	"�A�S�X�ôb�����C",
	"$n�S�X�ôb�����C",
	"�A��$N�S�X�ôb�����C",
	"$n��$N�S�X�ôb�����C",
	"$n��A�S�X�ôb�����C",
	"",
	""
    },
      {
	  "--",
	  "�A�S�X\"-_-\"�����C",
	  "$n�S�X\"-_-\"�����C",
	  "�A��$N�S�X\"-_-\"�����C",
	  "$n��$N�S�X\"-_-\"�����C",
	  "$n��A�S�X\"-_-\"�����C",
	  "�A��ۤv�S�X\"-_-\"�����C",
	  "$n��ۤv�S�X\"-_-\"�����C"
      },

 {
	"accuse",
	"Accuse �֡H",
	"",
	"�A���h�n�N���n��$N�@�C",
	"$n���h�n�N���n��$N�@�C",
	"$n���h�n�N���n�ۧA�@�C",
	"�A�P��}�ߤ��w�C",
	"$n�ݰ_�Ө}�ߤ��w�C"
    },

    {
	"ack",
	"�A�S�X�@�I�u���|���O�v�����C",
	"$n�S�X�@�I�u���|���O�v�����C",
	"",
	"",
	"",
	"",
	""
    },

    {
	"addoil",
	"�A��Ҧ��H�j�ۡG�u�[�o�I�v",
	"$n��Ҧ��H�j�ۡG�u�[�o�I�v",
	"�A�V$N�j�ۡG�u�[�o�I�v",
	"$n�V$N�j�ۡG�u�[�o�I�v",
	"$n��A�j�ۡG�u�[�o�I�v",
	"�A���ۤv�[�o����C",
	"$n��ۤv���G�u�[�o�I�v"
    },

    {
	"agree",
	"�A�����P�N�C",
	"$n�����P�N�C",
	"�A�����P�N$N���ݪk�C",
	"$n�����P�N$N���ݪk�C",
	"$n�����P�N�A���ݪk�C",
	"�A��ۤv���ݪk�P��ܦۻ��C",
	"$n��$e�ۤv���ݪk�P��ܦۻ��C"
    },
    { // by chinko@ROC
        "aluba",
            "�A�n���|�ڽ֡H",
        //"$n��������઺�M���y�����C",
        "$n�f�������ۡG�u���|�ڡ�ڭn���|�ڡ�v�@��|�B�M���H�C",
        "�A��$N��h���|��, ���L�Pı�ͤ��p���I",
        "$n���ݼɪ���$N�I�i�ǻ�������H��}���|�ڡC�ڡI�O�A��F��",
        "$n��$N��L�h, ��}�r���L����U��, $N�o�U�V�F�I",
        "�A�ƤF�ܡH",
        "$n���b��ۤv���ۧU�����|�ڡI�o�H"
    },

    {
	"angry",
	"�A�S�X�@�I�ܥͮ𪺪��C",
	"$n�S�X�@�I�ܥͮ𪺪��C",
	"�A�ͮ�a����$N�C",
	"$n�ͮ�a����$N�C",
	"$n�ͮ�a���ۧA�C",
	"�A��ۤv���M���欰�P��ܥͮ�C",
	"$n��ۤv���M���欰�P��ܥͮ�C"
    },

    {
	"applaud",
	"�A���Ħa���x.",
	"$n���Ħa���x.",
	"�A���$N���x�ܪ��I",
	"$n���$N���x�ܪ�.",
	"$n��A���x�ܪ��A�A�@�w�O���F����F���_���ơC",
	"��ۤv���x�H�A�ܤF���_�ܡH",
	"$n���$e�ۤv���x�A�n���H���ۤv�ܤF���_�C"
    },

    {
	"bark",
	"�A�s�F�@�n�u�L�v�I",
	"$n�Ǫ��s�F�@�n�u�L�v�C",
	"�A��$N�j�n�H���C",
	"$n��$N�j�n�H���C",
	"$n��A�j�n�H���C",
	"�A��ۦۤv�Ǫ��s�A�u�L�v�u�L�v�I",
	"$n���$e�ۤv�Ǫ��s�C"
    },

    {
	"beer",
	"�A���_�@�M�B��s�A���Y�@���ӺɡA�z�K�n�n�I",
	"$n���_�@�M�B��s�A���Y�@�f��ܰ��A�y�W�S�X���������C",
	"�A���q�a��$N�ˤF�@�M�M�D���B��s�C",
	"$n���q�a��$N�ˤF�@�M�B�D����s�C",
	"$n���q�a���A�ˤF�@�M�B��s�C",
	"�A���ۤv�ˤF�@�M�s�C",
	"$n��$e�ۤv�ˤF�@�M�s�C"
    },

    {
	"beg",
	"�A�i���������s�D�G�u�n�ߪ��ӭ��B���k��ЬI�˧ڴX�����a��v",
	"$n�i���������s�D�G�u�n�ߪ��ӭ��B���k��ЬI�˧ڴX�����a��v",
	"�A���$N�i���������s�D�G�u�n�ߪ��ӭ��B���k��ЬI�˧ڴX�����a��v",
	"$n���$N�i���������s�D�G�u�n�ߪ��ӭ��B���k��ЬI�˧ڴX�����a��v",
	"$n��ۧA�i���������s�D�G�u�n�ߪ��ӭ��B���k��I�˧ڴX�����a��v",
	"�A���檺�j�s�G�u���ڡ㬰����ڪ��R�o��W�ڡ�v",
	"$n���檺�j�s�G�u���ڡ㬰����ڪ��R�o��W�ڡ�v"
    },
    {
	"bf",  /* for rimu */
	"�A��Ťj�ۡG�u���ڡ��ڤ@�Ӭ��֦~�a��v",
	"$n��Ťj�ۡG�u���ڡ��ڤ@�Ӭ��֦~�a��v",
	"�A�ͮ�a��$N�۹D�G�u�p�l�A���a�ʧڪ����֦~�I�v",
	"$n�ͮ�a��$N�۹D�G�u�p�l�A���a�ʧڪ����֦~�I�v",
	"$n�ͮ�a��A�۹D�G�u�p�l�A���a�ʧڪ����֦~�I�v",
	"�A�ͮ�a�۹D�G�u���@�ӳå봱�ʧڪ����֦~�I�v",
	"$n�ͮ�a�۹D�G�u���@�ӳå봱�ʧڪ����֦~�I�v"
    },

    {
        "stool",
        "�A���b�鹹�W�����æۤv�������C",
        "���M$n�S�X�@���a�J�y���b�鹹�W�A�i�O�A�o�Pı�����������C",
        "�A�@���_���ѤU���鹹�A�µ�$N���j�y�r�O���U�h�I",
        "$n��M��_���ѤU���鹹��$N���y�W���h�A�o�@�����A���T�g�ġu�n�鹹�I�v",
        "$n��M��_���ѤU���鹹�§A�y�W���U�ӡA�ܢݡ���",
        "$n���ۧ鹹�g���D�G�u�sĵ����ڨS���ڡI�v",
        "$n���ۧ鹹�g���D�G�u�sĵ����ڨS���I�v�z�A�G�M�O�C�j�Z�������I",
}
,
{
           "zc",
           "�A�b�Ť��ֳt���񹺤F�X�U�A�f�̳����ۡG�uZc�h���IBG���n�ӡI�v",
           "$n�b�Ť��ֳt���񹺤F�X�U�A�f�̳����ۡG�uZc�h���IBG���n�ӡI�v",
           "�A�V$N�����D�G�u�@Zc�}��P�A�P�b�v",
           "$n�V$N�����D�G�u�@Zc�}��P�A�P�b�v",
           "$n��A�G�D�G�u�@Zc�}��P�A�P�b�v�A�Aı�o�@�w�|BG",
           "�A�յ����ۤv����Zc���}��A�����Ħ��G�����ˡC",
           "$n�յ���$m����Zc���A�G�A�i�O�ݨ�......"
       },
 {
          "masterAsia",
          "�A�γ̤j�O��Ŵ��F�褣�Ѭy",
          "$n���n�j�ܡG�u�y���A�F�褣�ѤD���̤����I�v",
          "�A���$N�j�ܡG�u�A�o�Ӳ³J�̤l�I�v",
          "$n���$N�j�ܡG�u�A�o�Ӳ³J�̤l�I�v",
          "$n�j�n�H�v�������������d�A�A���A�@�Y����",
          "�A�۫H�ۤv���F�褣�Ѭy�O�̱j��",
          "$n��ۦۤv���G�u�ڬO�F�褣�ѡI�ڬO�̱j���I�v"
      },
 {
           "richman",
           "�A�ΤO�b�I�]��½��A���X�F�@�Ƥj�I�ΡA�[�ܵۤj�a�Ӫ��C",
           "$n�����D�q�����ܥX�Ӥ@�Ƥj�I�ΡA�n�ۡG�u�Ӫ����I�Ӫ����I�v",
           "�A�V$N���ۡG�u�a�K�y��A�A�ŧK�v���ڴN�n�աI�v",
           "$n�V$N�������i��p�U����A�G�F�A�S�n�����F�C",
           "$n��A�r�զ��G�u�ڭn�ŧK�A��L�H��򦺧ڤ��ޡI�v",
           "�A�߸̷Q�ۡG�u�V�I����j�s���N�n�}���F�C�v",
           "�A�ݵ�$n�y�W�g���F�~�{�A�H�H�A�S���@�a�n�ˤF�C"
       },
{
                "boring",
                "�A�V�ѧo�۹D�G�u�ѰڡI���o��L��ڡI�I�v",
                "$n�V�Ѥj�ۡG�u�n�L��ڡI�I�v",
                "�A�i�v�v�a��$N���G�u�ܵL��C...�v",
                "$n�i�v�v�a��$N���G�u�ܵL��C...�v",
                "$n�i�v�v�a��A���G�u�ܵL��C...�v",
                "�A�]���ӵL��S���Ʊ����A�����L�����n�۫e��C",
                "$n�]���ӵL��S���Ʊ����A�����L�������b����C"
        },
{
          "shining",
          "�A��������M�_�X�F���˪����~�I",
          "$n��������M�X�{�F���˪����~�I�ϩ��o�{�F����I",
          "�A�o�{�F$N�����K�F�I�K�K�K�K�I",
          "$n���G�o�{�F����A�޲��a�n��$N�@�Ӥ���",
          "$n�{�G�������n�ۧA�����ݡA���A�������ۦb�I",
          "�A�o�{�F�@�Ǥ����H�������K�I",
          "$n���G�o�{�F���򯦱K�I�����I"
      },
 {
  "wc",
  "�A���n�N��a��j�a���G�u�藍�_�A�ڥh�W�Ӭ~�ⶡ�I�v�C",
  "$n���n�N��a��j�a���G�u�藍�_�A�ڥh�W�Ӭ~�ⶡ�I�v�A�M���榣�����]�}�F�C",
  "�A�����a��$N���G�u�W�ӴZ�ҡI���ڤ@�U��C�v",
  "$n�����a��$N���G�u�W�ӴZ�ҡI���ڤ@�U��C�v",
  "$n�����a��A���G�u�W�ӴZ�ҡI���ڤ@�U��C�v",
  "",
  "",
      },
 {
          "phone",
          "�A���n�N��a��j�a���G�u���ӹq�ܡI�v�C",
          "$n�j�s���A�q���T�F�A�ڥh���q�ܡI",
          "�A���¦a��$N���G�u���ӹq�ܡI�еy�����C�v",
          "$n���¦a��$N���G�u���ӹq�ܡI�еy�����C�v",
          "$n���¦a��A���G�u���ӹq�ܡI�еy�����C�v",
          "�A��U�c�l�A��c�l��q�ܻ��G�u�ޡI���a�ܡH�v",
          "$n��U�c�l�A��c�l��q�ܻ��G�u�ޡI���a�ܡH�v",
      },
 {
      "candle",
      "�A�I�U�F�@�ڬ�������A�ߤ��U�_�F�@�����c�����Y�C",
      "$n�I�U�@�ڬ�������A�A�ݨ쨺�������o���_���w���A�٬O����{���@�I���n�C",
      "�A�N����o�w�b$N���W�A�O�L�o�X�u�r�֡v���s�n�I",
      "$n�N����o�w�b$N���W�A�O$N�z�z�j�s�A�z�����",
      "$n�N�u�S������o�w�b�A���W�A�ϡB�ϤH��I",
      "�A�N����o�w�b�ۤv���W�A�n�S�S�S�S�S......",
      "$n�~�M�b�ۤv���W�w����o�H$s�����ٯu�O�_�S�ڡC",
      },
{
      "charm",
      "�A�Q�n��֨Ӧ����A���ԧQ�~�O�H",
      "$n�����_���|�B�i��A���G�b�M��U�⪺�ؼСA�p�ߧO�Q$e�ݤW�F�C",
      "�g�L�@�}���b�A$N�ש󦨤F�A���ԧQ�~�F�C",
      "$N�T���_$e�����b�A�ש�ѭ˦b$n���ۺh�ȤU�C",
      "�A�T���_$n�����b�A�M�w�n�û����H�L���k�F�C",
      "�A�H���b���褤���R���μv�A�[�[����۩ޡC",
      "�A�ݨ�$n���K�b����l�e���A�ϩ��w�g���F������C",                              
},
 {
      "pardon",
      "�A�b�ݫe���ӤQ�r�A��仡�D�G�@�W�ү�e���A�̳o�Ǹo�H�������",
      "$n�b�ݫe���ӤQ�r�A��仡�D�G�@�W�ү�e���A�̳o�Ǹo�H�������",
      "�A�@����$N���ӻH�A�@��b�ݫe���ӤQ�r��$N���D�G�W�ҷ|��̧A���������",
      "$n�@���b$N���ӻH�A�@��b�ݫe���ӤQ�r��$N���D�G�W�ҷ|��̧A���������",
      "$n�@���b�A���ӻH�A�@��b�ݫe���ӤQ�r��A���D�G�W�ҷ|��̧A���������",
      "�A�b�ݫe���ӤQ�r�A�j�n�a���D�G���ڡ�мe���ڧa�������",
      "$n�b�ݫe���ӤQ�r�A�j�n�a���D�G���ڡ�мe���ڧa�������",
      },
{
          "meow",
          "�A�����i�R���p�߫}�@��}�}�a�s, �ݰ_�ӤQ���O�H�Q��b�h���n�n���N�C",
          "$n�����i�R���p�߫}�@��}�}�a�s, �ݰ_�ӤQ���O�H�Q��b�h���n�n���N�C",
          "�A�����i�R���p�߫}, �b$N���Ǥ����a���b, �Ʊ�ձo$N���P���C",
          "$n�����i�R���p�߫}, �b$N���Ǥ����a���b, �Ʊ�ձo$N���P���C",
          "$n�����i�R���p�߫}, �b�A���Ǥ����a���b, �Ʊ�ձo�A���P���C",
          "�A�����i�R���۰]�ߦb�����ۿߤ��C",
          "$n�����i�R���۰]�ߦb�����ۿߤ��C"
      },
{
          "newbie",
          "�A��ۤj�a���G�u�ڬO�s���Фj�a�h�h���U�ڳ��v",
          "$n��j�a���Ӱ`���D�G�u�ڬO�s��A�����ٽФj�a�h�h���U�ڳ��v",
          "�A�V$N���G�u�ڬO�s��A�����ٽЧA�h�h���U�� *^_^*�v",
          "$n�V$N���G�u�ڬO�s��A�����ٽЧA�h�h���U�� *^_^*�v",
          "$n�V�A���G�u�ڬO�s��A�����ٽЧA�h�h���U�� *^_^*�v",
          "�A��ۦۤv�[�o���D�G�u�ڤ@�w�|�O�ӰQ�H���w���s�⪺��v",
          "�A�ݨ�$n���˪���§���A���A���T�Q�n�n�����U$e�C",
          },
{
	"bingo",
	"�A�������j�s�G�u�Т�������v",
	"$n�������j�s�G�u�Т�������v",
	"�A���Ħa��$N���G�u���ߧA�����F�I�v",
	"$n���Ħa��$N���G�u���ߧA�����F�I�v",
	"$n���Ħa��A���G�u���ߧA�����F�I�v",
	"",
	""
    },

    {
	"bite",
	"Bite �֡H",
	"",
	"�A�i�}��֤j�f�A��$N�����a�r�F�@�f�I",
	"$n�i�}��֤j�f�A��$N�����a�r�F�@�f�C",
	"$n�i�}��֤j�f�A���A�����a�r�F�@�f�I",
	"�A�j�o�o�W�A��O�r�F�ۤv�@�f�C",
	"$n�r�F�ۤv�@�f�A�O���O�j�a�F�H"
    },
    {
        "blatant",
        "�A�h�W��ݳ���j�s�G�u�{���I�O�{���ڡI�v",
        "$n�h�W��ݳ���j�s�G�u�{���I�O�{���ڡI�v",
        "�A�h�W�����$N���G�u�n�벴�����~�ڡI�v",
        "$n�h�W�����$N���G�u�Ө벴�F�I�ڽM�F�I�v",
        "$n�h�W����ۧA���G�u�n�벴�����~�ڡI�v",
        "�A���۾���A�s�˪����ۡG�u�i�|�A�ڪ��i�|�O�H�v",
        "$n���۾���s�˪��M��ۤv���ɪ����C"

    },
    { // by chinko@ROC

        "bless",
            "��C�C�Ʊ�@�ɩM���r��",
            "$n�n���ܦ��F���@�ˡA�ݥL�������A�O�h�򪺩Mħ�r�I",
            "���A�@�I�I�����֡A���A��ƨƶ��ߡC",
            "$n�b$N���Y�W�A�x�U�@������A�n���O�b���L���֦����I",
            "$n���u�߯��֧A��@�����Q��I",
            "Oh�IGod Bless Me��",
                "$n���ۤv�������h�ۺ��ߪ����\\ �C"
    },

    {
	"blush",
	"�A���y�`�۪����F�_�ӡI",
	"$n���y�`�۪����F�_�ӡI",
	"�A�@����$N�A���W�y���ۮ`�۶]�}�F�C",
	"$n�@����$N�A���W�y���ۮ`�۶]�}�F�C",
	"$n�@����A�A���W�y���ۮ`�۶]�}�F�C",
	"�A��ۤv���欰�P��ۮ��C",
	"$n��$e���欰�P��ۮ��C"
    },

    {
	"bounce",
	"�A�����a���۸����C",
	"$n���Ӥp�Ĥl�@�˦a���۸����C",
	"�A�����a����$N�����W�C",
	"$n�����a����$N�����W�C",
	"$n�����a����A�����W�C",
	"",
	""
    },

    {
	"bow",
	"�A�u���������`�C",
	"$n�u���������`�C",
	"�A�V$N�`�`�@���`�C",
	"$n�V$N�`�`�@���`�C",
	"$n��A�`�`�@���`�C",
	"",
	""
    },
/*
    {
	"burp",
	"You burp loudly.",
	"$n burps loudly.",
	"You burp loudly to $M in response.",
	"$n burps loudly in response to $N's remark.",
	"$n burps loudly in response to your remark.",
	"You burp at yourself.",
	"$n burps at $mself.  What a sick sight."
    },
*/
    {
	"bye",
	"�A��j�a���⻡�A���I",
	"$n��j�a���⻡�A���I",
	"�A�V$N���D�G�u���n�A���e�F�C�v",
	"$n�V$N���D�G�u���n�A���e�F�C�v",
	"$n��A���D�G�u���n�A���e�F�C�v",
	"",
	""
    },

    {
	"cackle",
	"�A�ܻŦa���_�Y�A�y�n�a�g���_�ӡI",
	"$n�ܻŦa���_�Y�A�y�n�a�g���_�ӡI",
	"�A��$N�y�n�g���C",
	"$n��$N�y�n�g���C",
	"$n��A�y�n�g���ۡC �A�̦n��$m���@�I�C",
	"�A���o�F�Ư�a��ۤv�y�n�g���C",
	"$n��ۦۤv�y�n�g���A�ݰ_�Ӧn�����ӹ�l�C "
    },

    {
	"chuckle",
	"�A�����a���F�@�n�C",
	"$n �����a���F�@�n�C",
	"�A���$N�����a���F�@�n�C ",
	"$n���$N�����a���F�@�n�C",
	"$n��ۧA�����a���F�@�n�C",
	"",
	""
    },

    {
	"clap",
	"�A���Ħa���x.",
	"$n���Ħa���x.",
	"�A��$N���x�ܪ��I",
	"$n��$N���x�ܪ�.",
	"$n��A���x�ܪ��A�A�@�w�O���F����F���_���ơC",
	"��ۤv���x�H�A�ܤF���_�ܡH",
	"$n���$e�ۤv���x�A�n���H���ۤv�ܤF���_�C"
/*	"You clap your hands together.",
	"$n shows $s approval by clapping $s hands together.",
	"You clap at $S performance.",
	"$n claps at $N's performance.",
	"$n claps at your performance.",
	"You clap at your own performance.",
	"$n claps at $s own performance." */
    },
/*
    {
	"comb",
	"You comb your hair - perfect.",
	"$n combs $s hair, how dashing!",
	"You patiently untangle $N's hair - what a mess!",
	"$n tries patiently to untangle $N's hair.",
	"$n pulls your hair in an attempt to comb it.",
	"You pull your hair, but it will not be combed.",
	"$n tries to comb $s tangled hair."
    },
*/
    {
	"cold",
	"�A�P��@���H�N�C",
	"$n��M�Q��o�ʼu���o�I",
	"�A��M�������w�A�n���Q$N���N���ܭ��F�C",
	"$N���N���ܭ�o$n"HIR"�I�������w�A�|�ϳ¤�I"NOR,
	"$N���N���ܭ�o$n"HIR"�I�������w�A�|�ϳ¤�I"NOR,
	"$n���N���ܭ�o�A"HIR"�I�������w�A�|�ϳ¤�I"NOR,
	"$n���N���ܭ�o$e�ۤv"HIR"�I�������w�A�|�ϳ¤�I"NOR
    },
    {
	"comfort",
	"comfort �֡H",
	"",
	"�A�Ӥߦa�w����$N�C",
	"$n�Ӥߦa�w����$N�C",
	"$n�Ӥߦa�w���ۧA�C",
	"",
	""
    },
    
    {
	"cong",
	"�A�j�s�G�u�i�ߥi�֡A�i�ߥi�֡�v",
	"$n�j�s�G�u�i�ߥi�֡A�i�ߥi�֡�v",
	"�A���$N�j�s�G�u���ߵo�]�A���]�@�ӨӡI�v",
	"$n���$N�j�s�G�u���ߵo�]�A���]�@�ӨӡI�v",
	"$n��ۧA�j�s�G�u���ߵo�]�A���]�@�ӨӡI�v",
	"",
	""
    },

    {
	"cook",
	"�A���X�l�l�B��l�A�ݰ_�ӭn�ˤ@��������Ʋz�C",
	"$n���X�l�l�B��l�A�ݰ_�ӭn�ˤ@��������Ʋz�C",
	"�A�[�_���l�A��W�@����A��$N��F�i�h�A�٦b������ۭ��H�ڪ��R�СC",
	"$n�[�_���l�A��W�@����A��$N��F�i�h�A�٦b������ۭ��H�ڪ��R�СC",
	"$n�[�_���l�A��W�@����A��A��F�i�h�A�٦b������ۭ��H�ڪ��R�СC",
	"�A���F�@��o�A�M�ᨫ�F�i�h�A��ۤv����������A�ݰ_�ӤS���S�ܡC",
	"$n���F�@��o�A�M�ᨫ�F�i�h�A��$e����������A�ݰ_�ӤS���S�ܡC"
    },

    {
	"cringe",
	"�A�`�Ȫ����o�ݡI",
	"$n�`�Ȫ����o�ݡC",
	"�A�`��$N�ӵo�ݡC",
	"$n�`��$N�ӵo�ݡC",
	"$n��ۧA���o�ݡA�ݰ_�Ӧn���ܩȧA�C",
	"�Aı�o�ۤv���o�ܥi�ȶܡH",
	""
    },

    {
	"cry",
	"�A���n�j���A�z����",
	"$n���n�j���C",
	"�A�a�b$N���ӻH�W�A���n�j���C",
	"$n�a�b$N���ӻH�W�A���n�j���C",
	"$n�a�b�A���ӻH�W�A���n�j���C",
	"�A�����a�����ڳڡC",
	"$n�����a�����ڳڡC"
    },

    {
	"cuddle",
	"cuddle �֡H",
	"",
	"�A�����a�֩��$N�C",
	"$n�����a�֩��$N�C",
	"$n�����a�֩�ۧA�C",
	"�A��������A�I�K�b�ۤv���@�ɸ̡C",
	"$n��������A�I�K�b�ۤv���@�ɸ̡C"
    },

    {
	"curse",
	"�A�|�D�G�u#*X^%?*&!�v",
	"$n�|�D�G�u #*X^%?*&!�v",
	"�A���$N�}�f�j�|�I.",
	"$n���$N�}�f�j�|�C",
	"$n��ۧA�}�f�j�|�A�A�@�w���F����O�L���n���ơC",
	"",
	""
    },

    {
	"curtsey",
	"�A��b�����Ҧ��H�֤F�@�֡C",
	"$n��Ҧ��H�֤F�@�֡C",
	"�A��$N�`���֤F�@�֡C",
	"$n��$N�`���֤F�@�֡C",
	"$n��A�`���֤F�@�֡C",
	"",
	""
    },

    {
	"dance",
	"�A���������_�R�ӡI",
	"$n���������_�R�ӡI",
	"�A�Ե�$N����A���������_�R�ӡI",
	"$n�Ե�$N����A���������_�R�ӡI",
	"$n�ԵۧA����A���������_�R�ӡI",
	"",
	""
    },

    {	
	"die",
	"�A���F�C",
	"$n���F�C",
	"�A�R�X�@�f�A��A���b$N���h�̡C",
	"$n�R�X�@�f�A��A���b$N���h�̡C",
	"$n�R�X�@�f�A��A���b�A���h�̡C",
	"�A�۱i�a���D�G�u�ӧr�A�ӱ��ڧr��v",
	"$n�۱i�a���D�G�u�ӧr�A�ӱ��ڧr��v"
    },

    {
	"disagree",
	"�A�������P�N�C",
	"$n�������P�N�C",
	"�A�������P�N$N���ݪk�C",
	"$n�������P�N$N���ݪk�C",
	"$n�������P�N�A���ݪk�C",
	"�A�{���ۤv���ݪk�������~�C",
	"$n�{��$e�ۤv���ݪk�������~�C"
    },
    {
        "dive",
        "�A��F�Ӧa�}�p�F�i�h�C",
        "$n��F�Ӧa�}�p�F�i�h�C",
        "�A�p�i$N���h�̼��b�C",
        "$n�p�i$N���h�̼��b�C",
        "$n�p�i�A���h�̼��b�C",
        "�A�Q�ɿ�k��ۤv���Y�I�i�g�̡C",
        "$n�b�a�W���F�Ӭ}�A�M���ۤv���Y�I�i�h�C"
    },
    {
        "domon",
        "�A����m�˦a���G�u�ڪ��⤤�������U�N��!!�v",
        "$n����a���G�u�ڪ��⤤�������U�N��!!�v",
        "�A�V$N����a���G�u�ݧڪ� GOD~~~ FINGER~~~ !!�v, $N�Q������}�l�����U�N�ۡC",
        "$n�V$N����a���G�u�ݧڪ� GOD~~~ FINGER~~~ !!�v, $N�Q������}�l�����U�N�ۡC",
        "$n��A����a���G�u�ݧڪ� GOD~~~ FINGER~~~ !!�v, �A�Q������}�l�����U�N�ۡC",
        "�A�⤤���ۦL, �������o�۪�����~, �i�J�����������A�C",
        "$n�������o�۱j�P��������~�C"
    },


    /*
     * This one's for Baka, Penn, and Onethumb!
     */
    {
	"drool",
	"�A�@�Ӥ��p�ߡA�f���w�F���a�C",
	"$n�@�Ӥ��p�ߡA�f���w�F���a�C",
	"�A�n��$N�@�Ӥ����A�f�����y�F�X�ӡC",
	"$n�n��$N�@�Ӥ����A�f�����y�F�X�ӡC",
	"$n�n�ۧA�@�Ӥ����A�f�����y�F�X�ӡC",
	"�A�@��̯��A�f���@��y�F�����C",
	"$n�@��̯��A�f���@��y�F$m�����C",
    },
    {
	"drunk",
	"�A�ܱo�K�H�H���A���Ӥj�{�l�A���F���СA�u�c���v�C",
	"$n�ܱo�K�H�H���A���Ӥj�{�l�A�u�c�v���@�n���F���СC",
	"�A�����ߪ��y���$N���G�u�Ĥl�A�A�K�F�C�v",
	"$n�����ߪ��y���$N���G�u�Ĥl�A�A�K�F�C�v",
	"$n�����ߪ��y���A���G�u�Ĥl�A�A�K�F�C�v",
	"",
	""
    },
    { //by sword@ROC
        "duel",
        "�A�j�n����q�J�u�O�֡I�H���شN�X�ӨM�@�ͦ��I�v",
        "$n�j�n����q�G�u�O�֡I�H���شN�X�ӨM�@�ͦ��I�v",
        "�A���ӦV$N�U�F�ԮѡJ�u���餣�O�A���N�O�ڤ`�A���̨M�������I�v",
        "$n���ӦV$N�U�F�ԮѡJ�u���餣�O�A���N�O�ڤ`�A���̨M�������I�v",
        "$n���ӦV�A�U�F�ԮѡJ�u���餣�O�A���N�O�ڤ`�A���̨M�������I�v",
        "�A�D�]���s�ۡJ�u�S���H����ڨM�����I�H�v",
        "$n�D�]���s�ۡJ�u�S���H����ڨM�����I�H�v�C"
    },


    {
	"duh",
	"�A�j�s�@�n�G�u�ڡI�ګ��S�Q��I�v",
	"$n�j�s�@�n�G�u�ڡI�ګ��S�Q��I�v",
	"","","","",""
    },

    {
	"dunno",
	"�A�n�n�Y�A���G�u�ڤ����D�C�v",
	"$n�n�n�Y�A���G�u�ڤ����D�C�v",
	"�A��$N���G�u�A������H�ڤ��M���@�C�v",
	"$n��$N���G�u�A������H�ڤ��M���@�C�v",
	"$n��A���G�u�A������H�ڤ��M���@�C�v",
	"�A���n�۵ۡG�u�ա�ա�ա�ڤ��򳣤����D�C�v",
	"$n���n�۵ۡG�u�ա�ա�ա�ڤ��򳣤����D�C�v"
    },

    {
	"enter",
	"�Ӧ��I�A�� ENTER ��d��F�I\n\r\n\r\n\r",
	"$n�C���Y���G�u�藍�_�A�ڪ� ENTER �d��F�C�v",
	"�A�W�f�C�ߦa�U$N�G�u�Ӵ��� ENTER �F�C�v",
	"$n�W�f�C�ߦa�U$N�G�u�Ӵ��� ENTER �F�C�v",
	"$n�W�f�C�ߦa�U�A�G�u�Ӵ��� ENTER �F�C�v",
	"�A���o�Ʀ��a�ޱ��ۤv��L�W�� ENTER�C",
	"$n���Ƥl���a��ۤv��L�W�� ENTER �ޱ��A�٤@��g���G�u�A�A�d�ڡ�A�d�ڡI�v"
	    
    },
      {
          "envy",
          "�A�����y�S�X�r�}�������I",
          "$n�����̬y�S�X�r�}�������I",
          "�A�r�}�a�n��$N�@�Ӥ���",
          "$n�r�}�a�n��$N�@�Ӥ���",
          "$n�r�}�a�n�ۧA�����@�A���A�Pı���q�M�I",
          "�A�ۻ��a���ۡG��ˡI�r�}�a�I",
          "$n�ۻ��a���ۡG��ˡI�r�}�a�I�K�K�K�I"
      },

    {
        "eyebeam",
        "�A�������g�X�ǲ������u�¥|�P���g�C",
        "$n��������M�g�X�ǲ������u�¥|�P�J�ñ��g�I",
        "�A�β����g�u��$N�ܦ��@�y�J���C",
        "$n��������M�g�X�ǲ������u�A��$N�ܦ��@�y�J���I",
        "�A�Q$n�������g�u�ܦ��@�y�J���I",
        "�A���p�ߧⲴ�����u�g��ۤv�����W�A�n�h���I",
        "$n���p�ߧⲴ�����u�g��ۤv�����W�h���z�z�j�s�C"

    },
    {
	"faint",
	"�A�f�R�ժj�A���˦b�a�W�C",
	"$n�f�R�ժj�A�u�N�v���@�n���˦b�a�C",
	"�A���˦b$N���h�̡C",
	"$n���˦b$N���h�̡C",
	"$n���˦b�A���h�̡C",
	"",
	""
    },

    {
	"fakerep",
	"�A�����A�G30059/30059�I�ͩR�O�B30611/30582�I�]�k�O�B30130/30130�I���ʤO�B13500000�I�g��ȡC",                                                                  "$n�����A�G30059/30059�I�ͩR�O�B30611/30582�I�]�k�O�B30130/30130�I���ʤO�B13500000�I�g��ȡC",
	"",
	"",
	"",
	""
    },


    {
	"fart",
	"�A������§�����g���H",
	"$n �ΤO��F�@���T��...�@�}��⪺�Ϲ��C�C�ƹL�ӡI",
	"�A�µ� $N ��F�@�ӤS��S�T�����C �Ĥl�A�A�u�f�A�C",
	"$n �µ� $N ��F�@�ӤS��S�T�����C �A�̦n�b $e ��V�A�o���e�k�]�I",
	"$n �µۧA��F�@�ӤS��S�T�����C �Ѫ�!�A����I�l�F�C",
	"�A�V�ۦۤv��F�R�ѳs���䧾�C �o�O�A���o���C",
	"$n �V�ۦۤv��F�R�ѳs���䧾�C  $e ���ݦn�u�O�_�S���C"
    },
    {
	"fear",
	"�A�`�Ȫ����o�ݡC",
	"$n�`�Ȫ����o�ݡC",
	"�A���$N���G�u������ȤF�a�C�v",
	"$n���$N���G�u������ȤF�a�C�v",
	"$n��ۧA���G�u������ȤF�a�C�v",
	"�A�ƨg���D�G�u�ȧڰڡA�ڳ̳��w�O�H�ȧڤF�I�v",
	"$n�ƨg���D�G�u�ȧڰڡA�ڳ̳��w�O�H�ȧڤF�I�v"
	    
    },
        {
	    "five",
	    "�A���|�ۤ�A�����D�n�F����C",
	    "$n���|�ۤ�A�����D�Q�F���H",
	    "�A�������M$N���F�Ӥ��C",
	    "$n�������M$N���F�Ӥ��C",
	    "$n�������M�A���F�Ӥ��C",
	    "�A���L�H���G�uHey! Give me FIVE !!�v",
	    "$n��j�a���G�uHey! Give me FIVE !!�v"
	},

/*
    {
	"flip",
	"You flip head over heels.",
	"$n flips head over heels.",
	"You flip $M over your shoulder.",
	"$n flips $N over $s shoulder.",
	"$n flips you over $s shoulder.  Hmmmm.",
	"You tumble all over the room.",
	"$n does some nice tumbling and gymnastics."
    },
*/
    {
	"flop",
	"�A���q���^�F�@���C",
	"$n���q���^�F�@���C",
	"�A�@�Ӥ��p�ߡA�^�˦b$N���h�̡C",
	"$n�@�Ӥ��p�ߡA�^�˦b$N���h�̡C",
	"$n�@�Ӥ��p�ߡA�^�˦b�A���h�̡C",
	"",
	""
    },

    {
	"fondle",
	"fondle �֡H",
	"$n������b�Ť��ô��A�y�W�S�X���c�����C",
	"�A�`���a�R����$N�C",
	"$n�`���a�R����$N�C",
	"$n�`���a�R���ۧA",
	"�A������b�ۤv���W�N�ӺN�h�C",
	"$n������b$e�ۤv���W�N�ӺN�h�C"
    },
/*
    {
	"french",
	"Kiss whom?",
	"",
	"You give $N a long and passionate kiss.",
	"$n kisses $N passionately.",
	"$n gives you a long and passionate kiss.",
	"You gather yourself in your arms and try to kiss yourself.",
	"$n makes an attempt at kissing $mself."
    },
*/
    { // by skalt@ROC
        "forget",
        "�A���y�b�������U�|�P�A���o�F���Яg��",
        "$n���y�b�������U�|�P�A���o�F���Яg��",
        "�A�˶̦�����$N���G�u�A�b������A�ڤ��O�o�o�^�ưڡv",
        "$n�˶̦�����$N���G�u�A�b������A�ڤ��O�o�o�^�ưڡv",
        "$n�˶̦�����A���G�u�A�b������A�ڤ��O�o�o�^�ưڡv",
        "�A�յۦ^�а_�ۤv��Ѫ��m�W�C",
        "$n����ۡG�u�ڬO��....�ڨ쩳�O��><�v"
    },

    {
	"frown",
	"�A�Ҵo���K�_���Y�C",
	"$n�Ҵo���K�_���Y�C",
	"�A���$N�K�_�F���Y�C",
	"$n���$N�K�_�F���Y�C",
	"$n��ۧA�K�_�F���Y�C",
	"",
	""
    },
/*
    {
	"fume",
	"You grit your teeth and fume with rage.",
	"$n grits $s teeth and fumes with rage.",
	"You stare at $M, fuming.",
	"$n stares at $N, fuming with rage.",
	"$n stares at you, fuming with rage!",
	"That's right - hate yourself!",
	"$n clenches $s fists and stomps his feet, fuming with anger."
    },
*/
    {
	"gasp",
	"�A��Y�������X�ܨӡC",
	"$n��Y�������X�ܨӡC",
	"�A��$N���ҧ@�Ҭ��P��ܳY���C",
	"$n��$N���ҧ@�Ҭ��P��ܳY���C",
	"$n��A���ҧ@�Ҭ��P��ܳY���C",
	"�A�k�k�Y�Aı�o�ۤv���ҧ@�Ҭ��u�O���W�_���C",
	"$n�k�k�Y�Aı�o$e�ۤv���ҧ@�Ҭ��u�O���W�_���C"
    },

    {
	"gf",
	"�A��Ťj�ۡG�u���ڡ��ڤ@�Ӥk�B�ͧa��v",
	"$n��Ťj�ۡG�u���ڡ��ڤ@�Ӥk�B�ͧa��v",
	"�A�ͮ�a��$N�۹D�G�u�p�l�A���a�ʧڪ����l�I�v",
	"$n�ͮ�a��$N�۹D�G�u�p�l�A���a�ʧڪ����l�I�v",
	"$n�ͮ�a��A�۹D�G�u�p�l�A���a�ʧڪ����l�I�v",
	"�A�ͮ�a�۹D�G�u���@�ӳå봱�ʧڪ����l�I�v",
	"$n�ͮ�a�۹D�G�u���@�ӳå봱�ʧڪ����l�I�v"
    },

	{
	"gift",
	"�A�q�f�U���X�Ӹt�����A½�r½���A���G�Q�q�̭�½�X��§���H�I",
	"$n�q�f�U���X�Ӹt�����A½�r½���A���G�Q�q�̭�½�X��§���H�I",
	"�A�Ե�$N����A��$N���b�����G�u�n�ߪ�$N�A�e�ڭ�§���n���n���H�v",
	"$n�Ե�$N����A��$N���b�����G�u�n�ߪ�$N�A�e�ڭ�§���n���n���H�v",
	"$n�ԵۧA����A��A���b�����G�u�n�ߪ�$N�A�e�ڭ�§���n���n���H�v",
	"�A��Ѥj�ۡG�u���j���ѯ��A�e�ڭ�§���a�I�v",
	"$n��Ѥj�ۡG�u���j���ѯ��A�e�ڭ�§���a�I�v",
	},

    {
	"giggle",
	"�A��檺���F�_�ӡC",
	"$n��檺���F�_�ӡC",
	"�A���$N��檺���C",
	"$n���$N��檺���C",
	"$n��ۧA��檺���C",
	"",
	""
    },

    {
	"glare",
	"�A��_�Y�A�����a��ۤѪšC",
	"$n��_�Y�A�����a��ۤѪšC",
	"�A�C�j�����A�n��$N�@�Ӥ����C",
	"$n�C�j�����A�n��$N�@�Ӥ����C",
	"$n�C�j�����A�n�ۧA�@�Ӥ����C",
	"�A�S��k�ݨ�ۤv�C",
	""
    },

    {
	"god",
	"�A��Mı�o�o�@�ɤW�@�w�������s�b�C",
	"$n��Mı�o�o�@�ɤW�@�w�������s�b�C",
	"�A��Mı�o$N²���򯫤@�ˡA�A���$E�����a�����C",
	"$n��Mı�o$N²���򯫤@�ˡA$e���$E�����a�����C",
	"$n��Mı�o�A²���򯫤@�ˡA$e��ۧA�����a�����C",
	"�A��Mı�o�A²���򯫤@��",
	"$n��Mı�o$e²���򯫤@��"
    },

    {
	"good",
	"�A�j�n�a���G�u�F���n�I�v",
	"$n�j�n�a���G�u�F���n�I�v",
	"�A�j�n�a���$N���G�u�n�I�v",
	"$n�j�n�a���$N���G�u�n�I�v",
	"$n�j�n�a��ۧA���G�u�n�I�v",
	"�Aı�o�ۤv�u�O�F���_�C",
	"$nı�o�ۤv�u�O�F���_�C"
    },

    {
	"grin",
	"�A�S�X���c�����e�C",
	"$n�S�X���c�����e�C",
	"�A��$N�S�X���c�����e�C",
	"$n��$N�S�X���c�����e�C",
	"$n��A�S�X���c�����e.. �A�̦n����ĵı��!!",
	"",
	""
    },

    {
	"groan",
	"�A�������D�u�ۡC",
	"$n�������D�u�ۡC",
	"�A���$N�������D�u�ۡC",
	"$n���$N�������D�u�ۡC",
	"$n��ۧA�������D�u�ۡC",
	"",
	""
    },
/*
    {
	"grope",
	"Whom do you wish to grope?",
	"",
	"Well, what sort of noise do you expect here?",
	"$n gropes $N.",
	"$n gropes you.",
	"You grope yourself - YUCK.",
	"$n gropes $mself - YUCK."
    },
*/
	{
	"gropen",
	"�A��Ѥj�ۡG�u�{�b���S���Ϊ��H�v",
	"$n��Ѥj�ۡG�u�{�b���S���Ϊ��H�v",
	"�A�Ե�$N���S�l�A��$N���G�u���j��Leader $N�A�}�ӹΦn���n�H�v",
	"$n�Ե�$N���S�l�A��$N���G�u���j��Leader $N�A�}�ӹΦn���n�H�v",
	"$n�ԵۧA���S�l�A��$N���G�u���j��Leader $N�A�}�ӹΦn���n�H�v",
	"�A��Ѥj�ۡG�u�ڭn�}�ΤF�A�֨Ӹ��H�ڧa�I�v",
	"$n��Ѥj�ۡG�u�ڭn�}�ΤF�A�֨Ӹ��H�ڧa�I�v",
	},
/*
	{
	"grovel",
	"You grovel in the dirt.",
	"$n grovels in the dirt.",
	"You grovel before $M.",
	"$n grovels in the dirt before $N.",
	"$n grovels in the dirt before you.",
	"That seems a little silly to me.",
	""
    },
*/
    {
	"growl",
	"�A��Ű��ۡA�u����v",
	"$n��Ű��ۡA�u����v",
	"�A��F�@�n�A���G�u�i�c��$N�A�ݩۡI�v",
	"$n��$N�ͮ𪺭�F�@�n�A�ݨӰ��W�N���@���c���C",
	"$n��ۧA���G�u�i�c���å�A�ݩۡI�v",
	"�A�çq�ås�A�߱��ΪA�F�@�I�C",
	"$n���$e�ۤv�çq�ås�A�u�O���W�_���C"
    },

    {
	"grumble",
	"�A�����a���ۡC",
	"$n�����a���ۡC",
	"�A���$N�����a���ۡC",
	"$n���$N�����a���ۡC",
	"$n��ۧA�����a���ۡC",
	"",
	""
    },
/*
    {
	"grunt",
	"GRNNNHTTTT.",
	"$n grunts like a pig.",
	"GRNNNHTTTT.",
	"$n grunts to $N.  What a pig!",
	"$n grunts to you.  What a pig!",
	"GRNNNHTTTT.",
	"$n grunts to nobody in particular.  What a pig!"
    },
*/
    {
	"hand",
	"�M�ִ���H",
	"",
	"�A���V$N�A�M$E�ΤO�����ۤ�C",
	"$n���V$N�A�M$E�ΤO�����ۤ�C",
	"$n���F�L�ӡA�M�A�ΤO�����ۤ�C",
	"�A�L�᪺���ۦۤv������C",
	"$n�L�᪺����$e�ۤv����C"
    },
/*
    {
	"hangover",
	"You pop a few aspirin and put on your sunglasses.  Ow, your head hurts!",
	"$n holds $s head and says 'Quit breathing so loud!'",
	"Won't $N be quiet?  Your head is gonna split in two if $E keeps talking!",
	"$n complains to $N 'Be quiet!	I have a hangover!",
	"$n complains to you 'Be quiet!	 I have a hangover!",
	"You shoosh yourself.  Be quiet!  You have a hangover!",
	"$n shooshes $mself.  Wow, what a hangover that must be!"
    },
*/
    {
        "happy",
        "�A�}�ߪ���۰��I�V�A�g�D���H�x�ᡯ*��\(�á���)/��*��",
        "$n�}�ߪ���۰��A�V���䪺�H�x�����j�a���P�V�F�ּ֪��𮧡I",
        "�A�o�_$N����}�ߪ���۰��A�g����G�]�P����ּ֪��𮧡I",
        "$n���֪��԰_$N���p��}�ߪ���۰��I",
        "$n���֪��԰_�A����P�A�۱o��֪���۰��",
        "�A�}�ߪ���۰��I�V�A�g�D���H�x�ᡯ*��\(�á���)/��*��",
        "$n�A�}�ߪ���۰��I�V�A�g�D���H�x�ᡯ*��\(�á���)/��*��"

    },
    {
	"hate",
	"�A�r�������A���D�G�u�ڦn���ڦn���v",
	"$n�r�������A���D�G�u�ڦn���ڦn���v",
	"�A��$N���}�A�ͮ�a���G�u�ګ�A�I�v",
	"$n��$N���}�A�ͮ�a���G�u�ګ�A�I�v",
	"$n��A���}�A�ͮ�a���G�u�ګ�A�I�v",
	"�A�ƨg�a���D�G�u��ڧr�A�ڳ̳��w�O�H��ڤF�I�v",
	"$n�ƨg�a���D�G�u��ڧr�A�ڳ̳��w�O�H��ڤF�I�v"
    },

    {
	"hehe",
	"�A�u�K�K�K��v�a�l���F�X�n�C",
	"$n�u�K�K�K��v�a�l���F�X�n�C",
	"�A�u�K�K�K��v�a��$N�l���F�X�n�C",
	"$n�u�K�K�K��v�a��$N�l���F�X�n�C",
	"$n�u�K�K�K��v�a��A�l���F�X�n�C",
	"",
	""
    },

    {
	"hello",
	"�A���Ħa��Ҧ��H���۩I�I",
	"$n���Ħa��Ҧ��H���۩I�I",
	"�A���Ħa��$N���G�u�١I�v",
	"$n���Ħa��$N���G�u�١I�v",
	"$n���Ħa��A���G�u�١I�v",
	"",
	""
    },

    {
	"help!",
	"�A�W�i�a�j�s�G�u�ϩR�r��ϩR�r��v",
	"$n�W�i�a�j�s�G�u�ϩR�r��ϩR�r��v",
	"�A�W�i�a��$N�j�s�G�u�ϩR�r��֨ӱϧڧr��v",
	"$n�W�i�a��$N�j�s�G�u�ϩR�r��֨ӱϧڧr��v",
	"$n�W�i�a��A�j�s�G�u�ϩR�r��֨ӱϧڧr��v",
	"�A��j�a���G�u��������D���ިӧ�ڦn�F�C�v",
	"$n��j�a���G�u��������D���ިӧ�ڦn�F�C�v"
    },


    {
	"hkpk",
	"�A���o�F�Ư몺�j�s�G�u�A�̳o�Ǽo��A���������Z�P�ڤ@�ԡI�v",
	"$n���o�F�Ư몺�j�s�G�u�A�̳o�Ǽo��A���������Z�P�ڤ@�ԡI�v",
	"�A�۱i�a��$N���G�u�f��f��f��㤵��کw�n�N�A�o�o���F���աI�I�v",
	"$n�۱i�a��$N���G�u�f��f��f��㤵��کw�n�N�A�o�o���F���աI�I�v",
	"$n�۱i�a��A���G�u�f��f��f��㤵��کw�n�N�A�o�o���F���աI�I�v",
	"�A���Y��Ѱ��ۡG�u�S�i��I�S�i�઺�I�ګ�|�ѰڡI�I�v",
	"$n���Y��Ѱ��ۡG�u�S�i��I�S�i�઺�I�ګ�|�ѰڡI�I�v",
    },
/*
    {
      "hero",
      "You heroically risk life and limb to save the day.",
      "$n heroically risks life and limb to save the day.",
      "You say to $M, '$N, YOU'RE MY HERO!'",
      "$n says to $N, '$N, YOU'RE MY HERO!'",
      "$n says to you, '$N, YOU'RE MY HERO!'",
      "You try to look heroic.",
      "$n does $s best impression of a hero."
    },

    {
	"highfive",
	"You jump in the air...oops, better get someone else to join you.",
	"$n jumps in the air by $mself.	 Is $e a cheerleader, or just daft?",
	"You jump in the air and give $M a big highfive!",
	"$n jumps in the air and gives $N a big highfive!",
	"$n jumps in the air and gives you a big highfive!",
	"You jump in the air and congratulate yourself!",
	"$n jumps in the air and gives $mself a big highfive!  Wonder what $e did?"
    },

    {
      "holdon",
      "You pause for a moment, hesitant.",
      "$n pauses for a moment, somewhat hesitant.",
      "You hold $M closely, hugging $M.",
      "$n holds $N closely, hugging $M.",
      "$n holds you closely, hugging you.",
      "You have to hold yourself up you're so weary. Go to Sleep!",
      "$n has to hold $mself up $e's so tired. $e should go to bed and get some sleep."
    },
*/
    {
	"hmm",
	"�A�u��v���@�n�A�@�Ʊ����S��ˤl�C",
	"$n�u��v���@�n�A�@�Ʊ����S��ˤl�C",
	"�A��$N�u��v�F�@�n�C",
	"$n��$N�u��v�F�@�n�C",
	"$n��A�u��v�F�@�n�C",
	"",
	""
    },

    {
	"hop",
	"�A���������Ӥp�Ĥl��a���۸����C",
	"$n���������Ӥp�Ĥl��a���۸����C",
	"�A�����a����$N�����W�C",
	"$n�����a����$N�����W�C",
	"$n�����a����A�����W�C",
	"",
	""
    },
/*
    {
	"howl",
	"AHOOOOOOOOOOOOooooooooooooo.",
	"$n howls over the smoking corpse.",
	"You howl at $M.",
	"$n howls at $N.  Someone's gonna die!",
	"$n howls at you.  You must be good looking.",
	"You howl at yourself.",
	"$n howls at $mself."
    },
  */  
    {
	"hug",
	"Hug �֡H",
	"",
	"�A�����a�ֵ�$N�C",
	"$n�����a�ֵ�$N�C",
	"$n�����a�ֵۧA�C",
	"",
	""
    },

    {
	"hungry",
	"�A�γ̫᪺�@���O��۹D�G�u�n�ߪ��j�ݡ�н�ڤ@�I�F��Y�a�v",
	"$n�γ̫᪺�@���O��۹D�G�u�n�ߪ��j�ݡ�н�ڤ@�I�F��Y�a�v",
	"�A��$N�L�O�a�۹D�G�u�n�ߪ��j�ݡ�н�ڤ@�I�F��Y�a�v",
	"$n��$N�L�O�a�۹D�G�u�n�ߪ��j�ݡ�н�ڤ@�I�F��Y�a�v",
	"$n��A�L�O�a�۹D�G�u�n�ߪ��j�ݡ�н�ڤ@�I�F��Y�a�v",
	"�A���{�l�B�P�B�P�a�s���n�j�n",
	"$n���{�l�B�P�B�P�a�s���n�j�n"
    },
/*
    {
	"hum",
	"Hmm Hmm Hmm Hmmmmmmm.",
	"$n hums like a bee with a chest cold.",
	"You hum a little ditty for $M.	 Hmm Hmm Hmm Hmmmmmm.",
	"$n hums a little ditty for $N.	 Hmm Hmm Hmm Hmmmmmm.",
	"$n hums a little ditty for you.  Hmm Hmm Hmm Hmmmmmm.",
	"Hmm Hmm Hmmmmmmm.",
	"$n hums like a bee with a chest cold."
    }, 
 
    {
      "icecube",
      "You wonder who would like an ice cube down their shirt.",
      "With a devious look on $s face, $n hides something behind $s back.",
      "You stuff an ice cube down $S shirt. You laugh as $E hops around.",
      "$n stuffs an ice cube down $N's shirt, better hope that was $s last ice cube.",
      "$n stuffs an ice cube down your shirt. Brrr! $n laughs as you struggle to get it out.",
      "You stick an ice cube down your shirt. Brrr!",
      "$n sticks an icecube down $s shirt, trying to cool off."
    },

    {
	"invite",
	"You speak your best French in hopes of getting lucky.",
	"$n tells you, 'Voulez-vous couche avec moi ce soir?'",	 
	"You speak your best French to $M in hopes of getting lucky.",
	"$n tells $N, 'Voulez-vous couche avec moi ce soir?'",
	"$n tells you, 'Voulez-vous couche avec moi ce soir?'",
	"You speak your best French in hopes of getting lucky, with yourself???",
	"$n says to $mself, 'Voulez-vous couche avec moi ce soir?'"
    },
  */
    {
	"idiot",
	"�A���h�a���G�u��I�@�s�շ��C�v",
	"$n���h�a���G�u��I�@�s�շ��C�v",
	"�A��$N���G�u�A�u�O�@�Ӥj�շ��I�v",
	"$n��$N���G�u�A�u�O�@�Ӥj�շ��I�v",
	"$n��A���G�u�A�u�O�@�Ӥj�շ��I�v",
	"�A�y�ۤf���A�̯��ۻ��G�u�ڬO�շ���ڬO�շ��ڡ�v",
	"$n�y�ۤf���A�̯��ۻ��G�u�ڬO�շ���ڬO�շ��ڡ�v"
    },

    {
	"idle",
	"�A���b�a�W�A�éæa�o�_�b�ӤF�C",
	"$n���b�a�W�A�éæa�o�_�b�ӤF�C",
	"",
	"",
	"",
	"",
	""
    },

    {
	"inn",
	"�Aı�o�ۤv�n�e���C",
	"$nı�o�ۤv�n�e���C",
	"�A�����R���\\���A�L�d�a���$N�C",
	"$n�����R���\\���A�L�d�a���$N�C",
	"$n�����R���\\���A�L�d�a��ۧA�C",
	"�Aı�o�ۤv�O�@�ɤW�̯¯u���H�C",
	"$nı�o�ۤv�O�@�ɤW�̯¯u���H�C"
    },

    {
	"kickass",
	"�A�m�߼A�L�C",
	"$n�ۤv�b�m�߼A�L�C",
	"�A�µ�$N�����ѡA������U�h�I",
	"$n�µ�$N�����ѡA������U�h�I",
	"$n�µۧA�����Ѭ�����U�h�A�z�A�n�h�ڡI",
	"�A�θ}����ۤv�����ѡA�u�L��C",
	"$n�θ}����ۤv�����ѡA�ݨӬO�L��ƤF�C"
    },

    {
	"kiss",
	"Kiss �֡H",
	"",
	"�A�`���a�ݵ�$N�A���F$E�`�`���@�k�C",
	"$n�`���a�ݵ�$N�A���F$E�`�`���@�k�C",
	"$n�`���a�ݵۧA�A���F�A�`�`���@�k�C",
	"�A�ˤF�@�U�ۤv����C",
	"$n�ˤF�@�U$e�ۤv����C"
    },


    {
	"kok",
	"�A���Ⱚ���|�_�A�n������V����F��C",
	"$n���Ⱚ���|�_�A�n������V����F��C",
	"�A���Ⱚ���|�_�A��$N���Y�W�����a�V�F�U�h�A���o$E���_���P�C",
	"$n���Ⱚ���|�_�A��$N���Y�W�����a�V�F�U�h�A���o$E���_���P�C",
	"$n���Ⱚ���|�_�A���A���Y�W�����a�V�F�U�h�A���o�A���_���P�C",
	"�A���ۤv�Y�W�����a�V�@�U�A��ۤv�������F�C",
	"$n���ۤv�Y�W�����a�V�@�U�A��$e�������F�C"
    },

    {
	"kowtow",
	"�A��M�Q���}�A��ۤv���Y���a�W�@�����Ӥ����C",
	"$n��M�Q���}�A��ۤv���Y���a�W�@�����Ӥ����C",
	"�A�s���]��$N��e�A�W�Y�D�G�u�Ѥj���{�A�p�̥�������A���o�㦺�o�C�v",
	"$n�s���]��$N��e�A�W�Y�D�G�u�Ѥj���{�A�p�̥�������A���o�㦺�o�C�v",
	"$n�s���]��A��e�A�W�Y�D�G�u�Ѥj���{�A�p�̥�������A���o�㦺�o�C�v",
	"�A�N�N�a���G�u�ݨ�ڨӤF�A��򤣨ӽW�Y�¸o�H�v",
	"$n�N�N�a���G�u�ݨ�ڨӤF�A��򤣨ӽW�Y�¸o�H�v"
    },

    {
	"lag",
	"�A��������A�z����o��C��",
	"$n�w�w�a��������A�z����o��C��",
	"",
	"",
	"",
	"",
	""
    },

    {
	"laugh",
	"�A�˦b�a�W�A��ۨ{�l�����u�Ӻu�h�C",
	"$n�˦b�a�W�A��ۨ{�l�����u�Ӻu�h�C",
	"�A����$N����l�j���C",
	"$n����$N����l�j���C",
	"$n���ۧA����l�j���C",
	"�A��ۦۤv�������̯��C",
	"$n���$e�ۤv�������̯��C"
    },

    {
	"lean",
	"lean �֡H",
	"",
	"�A�����p�߯�a�̰��b$N���h�̡C",
	"$n�����p�߯�a�̰��b$N���h�̡C",
	"$n�����p�߯�a�̰��b�A���h�̡C",
	"",
	""
    },

    {
	"leave",
	"�A��榣���a���}�F�C",
	"$n��榣���a���}�F�C",
	"�A���]���a�k��$N�����ǡC",
	"$n���]���a�k��$N�����ǡC",
	"$n���]���a�k���A�����ǡC",
	"",
	""
    },

    {
	"lick",
	"�A�Q�F�Q�ۤv���L��C",
	"$n�Q�F�Q$e�ۤv���L��C",
	"�A�Q�F�Q$N���y�A�˱o$E���y�f���A�n���@��",
	"$n�Q�F�Q$N���y�A�˱o$E���y�f���A�n���@��",
	"$n�Q�F�Q�A���y�A�˱o�A���y�f���A�n���@��",
	"�A�Q�F�Q�ۤv����C",
	"$n�Q�F�Q$e����C"
    },
/*
    {
      "listen",
      "You listen intently.",
      "$n listens intently.",
      "You listen intently to what $E is saying.",
      "$n listens intently to what $N is saying.",
      "$n listens intently to what you are saying.",
      "You pay close attention to what you are saying, trying to avoid saying something you shouldn't.",
      "$n listens intently to $mself. $e must enjoy the sound of $s own voice."
    },
*/
    {
	"lonely",
	"�A�S�X���檺�����C",
	"$n�S�X���檺�����C",
	"�A��$N���G�uHEY�I�A�I��ܡH�v",
	"$n��$N���G�uHEY�I�A�I��ܡH�v",
	"$n��A���G�uHEY�I�A�I��ܡH�v",
	"�A���Mı�o�ۤv�n�t��I",
	"$n���Mı�o�ۤv�n�t��I"
    },

	{
	"lostway",
    "�A�L�U�a���۵ۡG�u�ڰg���F�A�n�ߪ��j�����j�j�j�֨����� ��_�ߡv",
    "$n�L�U�a���۵ۡG�u�ڰg���F�A�n�ߪ��j�����j�j�j�֨����� ��_�ߡv",
    "�A�����$N���j�L�D�U�G�u���U�I�a�����}�o�Ӧ������a�� ��_�ߡv",
    "$n�����$N���j�L�D�U�G�u���U�I�a�����}�o�Ӧ������a�� ��_�ߡv",
    "$n�����A���j�L�D�U�G�u���U�I�a�����}�o�Ӧ������a�� ��_�ߡv",
    "�A�R�F�R���Y�A�˥i�R�����G�u�K�K....�n���g���F�C >////<�v",
    "$n�R�F�R���Y�A�˥i�R�����G�u�K�K....�n���g���F�C >////<�v"
    },

    {
	"love",
	"Love �֡H",
	"",
	"�A�`�������$N���G�u�ڷR�A�㦹���ä�����v",
	"$n�`�������$N���G�u�ڷR�A�㦹���ä�����v",
	"$n�`������ۧA���G�u�ڷR�A�㦹���ä�����v",
	"",
	""
    },
/*
    {
	"marvelous",
	"You say 'Ah dahling, you look MAHVELLOUS!'",
	"$n says 'Ah dahling, you look MAHVELLOUS!'",
	"You say to $M, 'Ah dahling, you look MAHVELLOUS!'",
	"$n says to $N, 'Ah dahling, you look MAHVELLOUS!'",
	"$n says to you, 'Ah dahling, you look MAHVELLOUS!'",
	"You say 'Ah dahling, I look MAHVELLOUS!'",
	"$n says 'Ah dahling, I look MAHVELLOUS!'"
    },
*/
    {
	"massage",
	"Massage �֡H",
	"$n���X����b�Ť��ç�C",
	"�A��$N�����A��$E�˱o�ܵΪA�C",
	"$n��$N�����A��$E�˱o�ܵΪA�C",
	"$n���A�����A�ڡ�u�ΪA��",
	"",
	""
    },
/*
    {
	"meow",
	"MEOW.",
	"$n meows.  What's $e going to do next, wash $mself with $s tongue?",
	"You meow at $M, hoping $E will give you some milk.",
	"$n meows at $N, hoping $E will give $m some milk. ",
	"$n meows at you.  Maybe $e wants some milk.",
	"You meow like a kitty cat.",
	"$n meows like a kitty cat."
    },
*/
    {
	"miss",
	"Miss �֡H",
	"$n�N�s�n�Q���֤]�����D�C",
	"�A�������ĦV$N�A��$E���G�u�n�[�����A�e�A���^�ΡC�v",
	"$n�������ĦV$N�A��$E���G�u�n�[�����A�e�A���^�ΡC�v",
	"$n�������ĤF�L�ӡA��A���G�u�n�[�����A�e�A���^�ΡC�v",
	"",
	""
    },

    {
    "mmm",
    "mmmmmmmmmm.......",
    "$n���D 'mmMMmmMMmmMMmm.'",
    "�A��$N���D 'mmMMmmMMmmMMmm.'",
    "$n��$N���D 'mmMMmmMMmmMMmm.'",
    "$n��A���D 'mmMMmmMMmmMMmm.'",
    "",
    ""
    },
    
    {
	"moan",
	"�A�G�s�@�n�A��ۤv�����~�P��Ҵo�C",
	"$n�G�s�@�n�C�S�X�@�ƫܾҴo�����C",
	"",
	"",
	"",
	"",
	""
    },

    {
	"monster",
	"�A��W�a�j�s�G�u�Ǫ��Ӱա�ְk�R�ڡ�v",
	"$n��W�a�j�s�G�u�Ǫ��Ӱա�ְk�R�ڡ�v",
	"�A���$N��W�a�j�s�G�u�Ǫ��Ӱա�ϩR�ڡ�v",
	"$n���$N��W�a�j�s�G�u�Ǫ��Ӱա�ϩR�ڡ�v",
	"$n��ۧA��W�a�j�s�G�u�Ǫ��Ӱա�ϩR�ڡ�v",
	"�A��W�Ǫ��ˡA�X���~�L�d�����H�C",
	"$n��W�Ǫ��ˡA�X���~�L�d�����H�C"
    },
/*
    {
	"mooch",
	"You beg for money, weapons, coins.",
	"$n says 'Spare change?'",
	"You beg $N for money, weapons, coins.",
	"$n begs you for favors of the insidious type...",
	"$n begs you for favors of the insidious type...",
	"You beg for money, weapons, coins.",
	"$n says 'Spare change?'"
    },
    
    {
	"moocow",
	"You make cow noises.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises at $M.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow at $N.",
	"$n looks at you and Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow."
    },
    
    {
	"moon",
	"Gee your butt is big.",
	"$n moons the entire room.",
	"You show your big butt to $M.",
	"$n shows $s big butt to $N.  Find a paddle, quick!",
	"$n shows $s big ugly butt to you.  How do you like it?",
	"You moon yourself.",
	"$n moons $mself.  Ugliest butt you ever saw."
    },*/
    

  	{
     	"morning",
     	"�A�ܦ��®�a��ۤj�a���G�u�� �w ~�v",
     	"$n�ܦ��®�a��ۤj�a���G�u�� �w ~�v",
     	"�A�}�ߦa���$N���@�n��",
     	"$n�}�ߦa��$N���G�u�� �w ~�v",
     	"$n�}�ߦa��A���n���w",
     	"�A���M�`�N��~���g�J�������A�~�o�{�w�g�ѫG�F",
     	"$n��M�o�{�Ӷ��w�g�X�ӤF"
	},
/*
    {
	"mosh",
	"You MOSH insanely about the room.",
	"$n MOSHES insanely about the room.",
	"You MOSH $M into a pile of Jello pudding.  Ewww!",
	"$n MOSHES $N into Jello pudding.  Ah, the blood!",
	"$n MOSHES you into the ground.",
	"You MOSH yourself.  Ah the blood!",
	"$n MOSHES and MOSHES and MOSHES and MOSHES..."
    },
    
    {
	"muhaha",
	"You laugh diabolically.  MUHAHAHAHAHAHA!",
	"$n laughs diabolically.  MUHAHAHAHAHAHA!",
	"You laugh at $M diabolically.	MUHAHAHAHAHAHA!",
	"$n laughs at $N diabolically.	MUHAHAHAHAHAHA!",
	"$n laughs at you diabolically.	 MUHAHAHAHAHAHA!",
	"Muhaha at yourself??  Wierd.",
	""
    },

    {
	"mwalk",
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.",      
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room."
    },

    {
	"nail",
	"You nibble nervously on your nails.",
	"$n nibbles nervously on $s fingernails.",
	"You nibble nervously on your nails.",
	"$n nibbles nervously on $s fingernails.",
	"$n nibbles nervously on your fingernails.  Yuck!",
	"You nibble nervously on your nails.",
	"$n nibbles nervously on $s fingernails."
    },
    
    {
	"nasty",
	"You do the best imitation of the nasty you can.",
	"$n does the nasty solo? -- wow.",
	"You do the nasty with $M.",
	"$n does the nasty with $N.  Find a firehose quick!",
	"$n does the nasty with you.  How do you like it?",
	"You do the nasty with yourself.",
	"$n does the nasty with $mself.	 Hoo boy."
    },
    
    {
	"ni",
	"You exclaim proudly, 'I am a knight that goes NI!'",
	"$n exclaims proudly, 'I am a knight that goes NI!'",
	"You exclaim proudly to $M, 'I am a knight that goes NI!'",
	"$n exclaims proudly to $N, 'I am a knight that goes NI!'",
	"$n proudly exclaims to you, 'I am a knight that goes NI!'",
	"You exclaim proudly, 'I am a knight that goes NI!'",
	"$n exclaims proudly, 'I am a knight that goes NI!'"
    },
*/
    {
	"nibble",
	"Nibble �֡H",
	"",
	"�A�����a�r��$N���զ��C",
	"$n�����a�r��$N���զ��C",
	"$n�����a�r�ۧA���զ��C",
	"�A�r����ۤv���զ��C",
	""
    },

    {
	"no!",
	"�A����a��Ѥj�s�G�u����v",
	"$n����a��Ѥj�s�G�u����v",
	"�A��$N�s�D�G�u���n�ڡ�A���i�H�o�˰ڡ�v",
	"$n��$N�s�D�G�u���n�ڡ�A���i�H�o�˰ڡ�v",
	"$n��A�s�D�G�u���n�ڡ�A���i�H�o�˰ڡ�v",
	"�A�s�D�G�u���n�㤣�n�㤣�n����v",
	"$n�s�D�G�u���n�㤣�n�㤣�n����v"
    },

    {
	"nod",
	"�A�I�F�I�Y�C",
	"$n�I�F�I�Y�C",
	"�A��$N�I�F�I�Y�C",
	"$n��$N�I�F�I�Y�C",
	"$n��A�I�F�I�Y�C",
	"",
	""
    },
/*
    {
      "noogie",
      "You look around, wondering who would like to get a noogie.",
      "$n looks around, wondering who would like to get a noogie.",
      "You put $M in a headlock and give $M a severe noogie.",
      "$n puts $N in a headlock and gives $M a noogie.",
      "$n puts you in a headlock and gives you a severe noogie.",
      "You give yourself a noogie. What a goof!",
      "$n gives $mself a noogie. What a goof!"
    },

    {
	"nose",
	"Gee your nose is big.",
	"$n noses around in your business.",
	"You tweak $S nose.",
	"$n tweaks $N's nose.  Ow!",
	"$n tweaks your nose.  Ow!",
	"You wiggle your nose.",
	"$n wiggles $s nose."
    },

   {
	"nudge",
	"Nudge whom?",
	"",
	"You nudge $M.",
	"$n nudges $N.",
	"$n nudges you.",
	"You nudge yourself, for some strange reason.",
	"$n nudges $mself, to keep $mself awake."
    },

    {
	"nuke",
	"Who do you want to nuke?",
	"$n tries to put something in the microwave.",
	"You envelop $M in a mushroom cloud.",
	"$n detonates a nuclear warhead at $N.	Uh oh!",
	"$n envelops you in a mushroom cloud.",
	"You nuke yourself.",
	"$n puts $mself in the microwave."
    },
*/
    {
	"nuzzle",
	"Nuzzle �֡H",
	"",
	"�A�λ�l�b$N�����W�O���O�h�C",
	"$n�λ�l�b$N�����W�O���O�h�C",
	"$n�λ�l�b�A�����W�O���O�h�C",
	"",
	""
    },
/*
    {
	"ogle",
	"Whom do you want to ogle?",
	"",
	"You ogle $M like $E was a piece of meat.",
	"$n ogles $N.  Maybe you should leave them alone for awhile?",
	"$n ogles you.	Guess what $e is thinking about?",
	"You ogle yourself.  You may just be too wierd for this mud.",
	"$n ogles $mself.  Better hope that $e stops there."
    },

    {
	"ohno",
	"Oh no!	 You did it again!",
	"Oh no!	 $n did it again!",
	"You exclaim to $M, 'Oh no!  I did it again!'",
	"$n exclaims to $N, 'Oh no!  I did it again!'",
	"$n exclaims to you, 'Oh no!  I did it again!'",
	"You exclaim to yourself, 'Oh no!  I did it again!'",
	"$n exclaims to $mself, 'Oh no!	 I did it again!'"
    },
    
    {
	"oink",
	"You make pig noises.  OINK!",
	"$n oinks 'OINK OINK OINK!'",
	"You make pig noises at $M.  OINK!",
	"$n oinks at $N: 'OINK OINK OINK!'",
	"$n oinks at you: 'OINK OINK OINK!'",
	"You make pig noises.  OINK!",
	"$n oinks 'OINK OINK OINK!'"
    },
 
    {
	"ooo",
	"You go ooOOooOOooOOoo.",
	"$n says, 'ooOOooOOooOOoo.'",
	"You go ooOOooOOooOOoo.",
	"$n says, 'ooOOooOOooOOoo.'",
	"$n thinks of you and says, 'ooOOooOOooOOoo.'",
	"You go ooOOooOOooOOoo.",
	"$n says, 'ooOOooOOooOOoo.'"
    },
*/
	{
	"ok!",
	"�A���F��OK����աA���F�n�G�u�S���D�I�v",
	"$n���F��OK����աA���F�n�G�u�S���D�I�v",
	"�A���$N���F��OK����աA��$N���G�u�S���D�I�v",
	"$n���$N���F��OK����աA��$N���G�u�S���D�I�v",
	"$n��ۧA���F��OK����աA��A���G�u�S���D�I�v",
	"�A���F��OK����աA���Ѫ����G�u�ڴN���D�S���Ʊ����o�˧ڪ��I�v",
	"$n���F��OK����աA���Ѫ����G�u�ڴN���D�S���Ʊ����o�˧ڪ��I�v",
	},

    {
	"ooo",
	"�A�ۦb�𨤡A���y�e�}�a�b�a�W�e���C",
	"$n�ۦb�𨤡A���y�e�}�a�b�a�W�e���C",
	"�A�ۦb�𨤡A�I���$N�b�a�W�e���C",
	"$n�ۦb�𨤡A�I���$N�b�a�W�e���C",
	"$n�ۦb�𨤡A�I��ۧA�b�a�W�e���C",
	"",
	""
    },
    {
        "opost",
        "�A�h�W������Y���Ѫ��S�G�u�ѱ�I�I�o�O�@�Ӧѱ�ڡI�v",
        "$n�h�W������Y���Ѫ��S�G�u�ѱ�I�I�o�O�@�Ӧѱ�ڡI�v",
        "�Aݳ��$N���L��i�a�ݤF�ݥ|�P���G�u�o��h�m���A�ѱ���A���I�v",
        "$nݳ��$N���L��i�a�ݤF�ݥ|�P���G�u�o��h�m���A�ѱ���A���I�v",
        "$nݳ��A���L��i�a�ݤF�ݥ|�P���G�u�o��h�m���A�ѱ���A���I�v",
        "�A��ä��w�����ۻy�G�u�ڤS�ѱ�F��.....�H�v",
        "$n��ä��w�����ۻy�G�u�ڤS�ѱ�F��.....�H�v"
		},
	{
	"orz",
	"�A�@�ɵL�k�����o�ӥ����A���N���s�U���h �ݢ��C ",
	"$n���G����ܤj���ˮ`�A�Ƿt���N�����s���� �ݢ��C ������",
	"�A�o�{�ۤv��$N�����ǹ�b�t�Ӧh�F�A���۸T���\\�X�F �ݢ��C ������",
	"$n�o�{�ۤv��$N�����ǹ�b�t�Ӧh�F�A���۸T���\\�X�F �ݢ��C ������",
	"$n�o�{�ۤv��A�����ǹ�b�t�Ӧh�F�A�u�o�\\�X�Ѫ̼лx �ݢ��C �A�z����",
	"�A�ˤ����L�h�W�������A�\\�X�F���N��e�s���ʧ@ �ݢ��C ",
	"$n�ˤ����L�h�W�������A�\\�X�F���N��e�s���ʧ@ �ݢ��C "
	},
    {
	"ouch",
	"�A�j�s�G�u�n�h�ڡ�h���H�ա�v",
	"$n�j�s�G�u�n�h�ڡ�h���H�ա�v",
	"",
	"",
	"",
	"�A�h���b�a�W���u�C",
	"$n�h���b�a�W���u�C"
    },

    {
	"pat",
	"Pat �֡H",
	"",
	"�A�������$N���Y�C",
	"$n�������$N���Y�C",
	"$n�������A���Y�C",
	"�A�N�N�ۤv���Y�C",
	"$n�N�N$e�ۤv���Y�C"
    },
    {
	"pig",
	"�A���G�u�n�ޮ@�I�v",
	"$n���G�u�n�ޮ@�I�v",
	"�Aı�o$N²���ª����Y�ޡC",
	"$nı�o$N²���ª����Y�ޡC",
	"$nı�o�A²���ª����Y�ޡC",
	"�Aı�o�ۤv�ª����Y�ޡC",
	"$nı�o�ۤv�ª���ޤ@�ˡC"
    },

    /*{
	"pinch",
	"You toss a pinch of salt over your shoulder.",
	"$n tosses a pinch of salt over $s shoulder.",
	"You pinch $S rosy cheeks.",
	"$n pinches $N's rosy cheeks.",
	"$n pinches your chubby cheeks.",
	"You need a pinch of salt.",
	"$n needs a pinch of salt."
    },

    {
	"ping",
	"Ping who?",
	"",
	"You ping $N.  $N is unreachable.",
	"$n pings $N.  $N is unreachable.",
	"$n pings you.	Is your server dead or alive?",
	"You ping yourself. Ooof!",
	"$n pings $mself. Ooof!"
    },
*/
    { //by Zeel@ROC
        "pili",
        "�A�Q��p��Ӫ��{�q�����������¡I",
        "$n�l��ѹp�i��L�t�O�����I",
        "�A��$N�������¤@���I",
        "$n�l��ѹp��$N���o�O���¤@���I",
        "$n��A�I��ѹp�l��A���o�A���¤@���I",
        "�A�յۥl��ѹp�����ۤv�C",
        "$n�յ۹�ۤv�I�H�p�q�����H"
    },
    {
	"pk",
	"�A���o�F�Ư몺�j�s�G�u���@�Ӥ��n�R���A���ںu�X�ӡI�v",
	"$n���o�F�Ư몺�j�s�G�u���@�Ӥ��n�R���A���ںu�X�ӡI�v",
	"�A��$N���G�u�p�l�A���~���ѴN�O�A���Ҥ�A�ݩۡI�v",
	"$n��$N���G�u�p�l�A���~���ѴN�O�A���Ҥ�A�ݩۡI�v",
	"$n��A���G�u�p�l�A���~���ѴN�O�A���Ҥ�A�ݩۡI�v",
	"�A���X�@��M�A�¦ۤv�����W��F�i�h�A�H�A���F�I",
	"$n���X�@��M�A��$e�ۤv���W��F�i�h�A�H�A���F�I"
    },

    {
	"point",
	"Point �֡H",
	"",
	"�A����$N�C",
	"$n����$N�C",
	"$n���ۧA�C",
	"�A���ۦۤv�C",
	"$n����$e�ۤv�C"
    },

    {
	"poke",
	"Poke �֡H",
	"",
	"�A�L��a�W�F$N�@�U�C",
	"$n�L��a�W�F$N�@�U�C",
	"$n�L��a�W�F�A�@�U�C",
	"�A�W�F�A�ۤv�@�U�C",
	"$n�W�F$e�ۤv�@�U�C"
    },

/* I will use 'think' to replace 'ponder'   */
   {
	"ponder",
	"�A�C�Y�I��.",
	"$n �C�Y�I��.",
	"�A�{�u�Ҽ{$N�����D�C",
	"$n�{�u�Ҽ{$N�����D�C",
	"$n�{�u�Ҽ{�A�����D�C",
	"�A�{�u�Ҽ{�ۦۤv�����ӡC",
	"$n�{�u�Ҽ{��$e�����ӡC"
    },
/*
    {
	"possum",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"$n hits the ground... DEAD.",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD."
    },
    
    {
	"potato",
	"You juggle a hot potato in your hands.",
	"$n juggles a hot potato in $s hands.",
	"You juggle a hot potato and pass it onto $M.",
	"$n juggles a hot potato and gives it to $N!",
	"$n juggles a hot potato and gives it to YOU!",
	"You juggle a hot potato in your hands.",
	"$n juggles a hot potato in $s hands."
    },
    
    {
	"poupon",
	"You say 'Pardon me, do you have any Grey Poupon?'.",
	"$n says 'Pardon me, do you have any Grey Poupon?'",
	"You say to $M, 'Pardon me, do you have any Grey Poupon?'.",
	"$n says to $N, 'Pardon me, do you have any Grey Poupon?'",
	"$n says to you, 'Pardon me, do you have any Grey Poupon?'",
	"You say 'Pardon me, do you have any Grey Poupon?'.",
	"$n says 'Pardon me, do you have any Grey Poupon?'"
    },
*/
    {
	"pout",
	"�A��������¼�_�L�ڡC",
	"$n��������¼�_�L�ڡC",
	"",
	"",
	"",
	"",
	""
    },
/*
    {
	"pray",
	"You feel righteous, and maybe a little foolish.",
	"$n begs and grovels to the powers that be.",
	"You crawl in the dust before $M.",
	"$n falls down and grovels in the dirt before $N.",
	"$n kisses the dirt at your feet.",
	"Talk about narcissism ...",
	"$n mumbles a prayer to $mself."
    },
    {	
	"pretend",
	"You pretend you are a GOD, and slay everyone in sight!",
	"$n is pretending $e is an implementor again.  *sigh*",
	"You pretend you are a GOD, and demote $M to level 1.",
	"$n pretends $e is a GOD, and says, '$N, you're demoted to level 1!'",
	"$n pretends $e is a GOD, and says, 'You are demoted to level 1!'",
	"You pretend you are an implementor, and you demote yourself to level 1.",
	"$n pretends $e is a GOD, and demotes $mself to level 1."
    },

    {
	"propose",
	"You propose a contract game of bridge.",
	"$n proposes a contract game of bridge.",
	"You propose marriage to $M.",
	"$n gets down on one knee and proposes to $N.",
	"$n says quietly 'Will you marry me?'",
	"Even you wouldn't marry yourself.",
	"$n refuses the proposal made by $mself."
    },
    */
    {
	"puke",
	"�A�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F�@�a�C",
	"$n�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F�@�a�C",
	"�A�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F$N�@���C",
	"$n�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F$N�@���C",
	"$n�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F�A�@���C",
	"�A�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F�ۤv�@���C",
	"$n�y��@�}�C�@�}�աA��M�u�z�v���@�n�A�R�F$e�ۤv�@���C"
    },

    {
	"pull",
	"Pull �֡H",
	"",
	"�A�Ե�$N���S�l�A�ˤ��o��$E���}�C",
	"$n�Ե�$N���S�l�A�ˤ��o��$E���}�C",
	"$n�ԵۧA���S�l�A�ˤ��o���A���}�C",
	"�A�ԤF�Ԧۤv����S�C",
	"$n�ԤF��$e�ۤv����S�C"
    },

    {
	"punch",
	"Punch �֡H",
	"",
	"�A�@�����b$N���{�l�W�A�n�ڡI",
	"$n�@�����b$N���{�l�W�C",
	"$n�@�����b�A���{�l�W�A�n�h�I",
	"�A�Ϥ�@���A��ۤv���o�˦b�a�W�C",
	"$n�Ϥ�@���A��$e�ۤv���o�˦b�a�W�C"
    },
/*
    {
	"purr",
	"MMMMEEEEEEEEOOOOOOOOOWWWWWWWWWWWW.",
	"$n purrs contentedly.",
	"You purr contentedly in $M lap.",
	"$n purrs contentedly in $N's lap.",
	"$n purrs contentedly in your lap.",
	"You purr at yourself.",
	"$n purrs at $mself.  Must be a cat thing."
    },

    {
      "push",
      "You push the history-eraser button! BUT, nothing seems to happen.",
      "$n pushes the history-eraser button! BUT, nothing seems to happen.",
      "You push $M away in disgust.",
      "$n pushes $N away in disgust.",
      "$n pushes you away in disgust.",
      "You push yourself to the limit of your abilities.",
      "$n pushes $mself to the limits of $s abilites."
    },
*/
    {
	"quick",
	"�A�۫檺���G�u���I�r�A���M�N�Ӥ��ΤF�C�v",
	"$n�۫檺���G�u���I�r�A���M�N�Ӥ��ΤF�C�v",
	"�A�����a�ʫP��$N�G�u�A�ʧ@���I�n���n�ڡI�v",
	"$n�����a�ʫP��$N�G�u�A�ʧ@���I�n���n�ڡI�v",
	"$n�����a�ʫP�ۧA�G�u�A�ʧ@���I�n���n�ڡI�v",
	"�A��ۤv���������t�P��ܺ��N�C",
	"$n��$e�ۤv���������t�P��ܺ��N�C"
    },

    {
	"rice",
	"�A�j�s�G�u�̬���̬��㵹�ڦ̬���l�K�͡I�v",
	"$n�j�s�G�u�̬���̬��㵹�ڦ̬���l�K�͡I�v",
	"�A�Ե�$N���稤�A��$e���G�u$N $N�A���ڦ̤@�U�a��v",
	"$n�Ե�$N���稤�A��$e���G�u$N $N�A���ڦ̤@�U�a��v",
	"$n�ԵۧA���稤�A��A���G�u$N $N�A���ڦ̤@�U�a��v",
	"�A��j�a�ť��G�u�n�̧ڪ��N�ӧa�I�v",
	"$n��j�a�ť��G�u�n�̧ڪ��N�ӧa�I�v",
    },
/*
    {
	"raise",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"$n raises $s hand in response to you.",
	"You raise your hand in response.",
	"$n raises $s hand in response."
    },
   
    {
      "reassure",
      "You reassure everyone that everything will be ok.",
      "$n reassures everyone that everything will be ok.",
      "You hug $M reassuringly.",
      "$n hugs $N reassuringly.",
      "$n hugs you reassuringly.",
      "You try to reassure yourself that everything will be ok.",
      "$n tries to reassure $mself that everything will be ok."
    },

    {	
	"renandstimpy",
	"You say, 'Oh Happy Happy, Joy Joy!'",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!'",
	"You exclaim, 'Oh Happy Happy, Joy Joy!' at the mere thought of $M.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' as $e sees $N enter the room.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' when $e sees you approach.",
	"You exclaim, 'Oh Happy Happy, Joy Joy!' at the thought of yourself.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' at the thought of $mself."
    },	

    {
	"rofl",
	"You roll on the floor laughing hysterically.",
	"$n rolls on the floor laughing hysterically.",
	"You laugh your head off at $S remark.",
	"$n rolls on the floor laughing at $N's remark.",
	"$n can't stop laughing at your remark.",
	"You roll on the floor and laugh at yourself.",
	"$n laughs at $mself.  Join in the fun."
    },
    */
    {
	    "roll",
	    "�A��F��A������ @_@ �C",
	    "$n��������F�@�� @_@ �C",
	    "",
	    "",
	    "",
	    "",
	   "" 
	},

/*
    {
	"rub",
	"You rub your eyes.  How long have you been at this?",
	"$n rubs $s eyes.  $n must have been playing all day.",
	"You rub your eyes.  Has $N been playing as long as you have?",
	"$n rubs $s eyes.  $n must have been playing all day.",
	"$n rubs $s eyes.  Have you been playing as long as $m?",
	"You rub your eyes.  How long have you been at this?",
	"$n rubs $s eyes.  $n must have been playing all day."
    },
*/
    {
	"ruffle",
	"�A���F���Q���j�ê��Y�v�C",
	"$n���F���Q���j�ê��Y�v�C",
	"�A��$N���F���Q���j�ê��Y�v�C",
	"$n��$N���F���Q���j�ê��Y�v�C",
	"$n���A���F���Q���j�ê��Y�v�C",
	"",
	""
    },
/*
    {
	"russian",
	"You speak Russian.  Yeah, right.  Dream on.",
	"$n says 'Ya horosho stari malenky koshka.'  Huh?",
	"You speak Russian to $M.  Yeah, right.	 Dream on.",
	"$n says to $N 'Ya horosho stari malenky koshka.'  Huh?",
	"$n says to you 'Ya horosho stari malenky koshka.'  Huh?",
	"You speak Russian.  Yeah, right.  Dream on.",
	"$n says 'Ya horosho stari malenky koshka.'  Huh?"
    },
    */
    {
	"sad",
	"�A���L�a���U�\\�ӡC",
	"$n���L�a���U���\\�C",
	"�A���$N�A�ߤ��@�}�ķ��A���\\���T�ܲ��ӥX�C",
	"$n�������L�����$N�A���\\�ܲ��ӥX�C",
	"$n�������L����ۧA�A���\\�ܲ��ӥX�C",
	"",""
    },

    {
	"salute",
	"�A�t�x�a��F���|��§�C",
	"$n�t�x�a��F���|��§�C",
	"�A��$N��F���|��§�C",
	"$n��$N��F���|��§�C",
	"$n��A��F���|��§�C",
	"Huh?",
	""
    },
/*
    {
	"scold",
	"You scold everyone for being so naughty!",
	"$n scolds everyone for being so naughty!",
	"You scold $M for being so naughty!",
	"$n scolds $N for being so naughty!",
	"$n scolds you for being so naughty!",
	"You scold yourself for being so naughty!",
	"$n scolds $mself for being so naughty!"
    },
*/
    {
	"scream",
	"�A�j�s�G�u�ڡ�����v",
	"$n�j�n�a�y�s�I",
	"",
	"",
	"",
	"",
	""
    },

	{
    "silent",
    "�A�j�ܤ@�n�F�u�j�a�w�R�I�v",
    "$n�j�ܤ@�n�G�u�j�a�w�R�I�v",
    "�A�p�n����$N���F�u�Цw�R�@�U�n�ܡH�v",
    "$n�p�n����$N���F�u�Цw�R�@�U�n�ܡH�v",
    "$n�p�n����A���F�u�Цw�R�@�U�n�ܡH�v�ݨӧA���˰Q�@�U�F",
    "�A����ۤv���L�ڡA�n�����F���򤣸ӻ����ܡC",
    "$n����ۤv���L�ڡA�n�����F���򤣸ӻ����ܡC",
	},

    {
	"shake",
	"�A�n�F�n�Y�C",
	"$n�n�F�n�Y�C",
	"�A��$N�n�F�n�Y�C",
	"$n��$N�n�F�n�Y�C",
	"$n��A�n�F�n�Y�C",
	"",
	""
    },

    {
	"shiver",
	"�A�N���o�ݡI",
	"$n�N���o�ݡI",
	"�A�`��$N�ӵo�ݡC",
	"$n�`��$N�ӵo�ݡC",
	"$n��ۧA���o�ݡA�ݰ_�Ӧn���ܩȧA�C",
	"�A�Y�ۨ��l�A�o�ݵۻ��G�u�H�a�n�Ȧn�Ȯ@��v",
	"$n�Y�ۨ��l�A�o�ݵۻ��G�u�H�a�n�Ȧn�Ȯ@��v"
    },

    {
	"shrug",
	"�A�L�`�a�q�F�q�ӡC",
	"$n�L�`�a�q�F�q�ӡC",
	"�A���$N�L�`�a�q�F�q�ӡC",
	"$n���$N�L�`�a�q�F�q�ӡC",
	"$n��ۧA�L�`�a�q�F�q�ӡC",
	"",
	""
    },

    {
	"sigh",
	"�A�������ĤF�@�f��A�u�����v",
	"$n�������ĤF�@�f��C",
	"�A���$N�������ĤF�@�f��C",
	"$n���$N�������ĤF�@�f��C",
	"$n��ۧA�������ĤF�@�f��C",
	"",
	""
    },

    {
	"sing",
	"�A�����a��_�F�p��.",
	"$n���n�a�۰_�q��.",
	"�A��$N�r�֪���ۨ�����ť���p��.",
	"$n��$N�۵ۡG�u�A�ֶּܡH�ګܧּ֡I�v",
	"$n��A�۵ۡG�u�A�ֶּܡH�ګܧּ֡I�v",
	"�A�@��@��L�ơA�@���ۤv��ۺq�C",
	"$n�@��@��L�ơA�@���ۤv��ۺq�C"
    },

    {
	"slap",
	"Slap �֡H",
	"",
	"�A�����a���F$N�X�Ӧե��A���o$E��i�y���~�F�_�ӡC",
	"$n�����a���F$N�X�Ӧե��A���o$E��i�y���~�F�_�ӡC",
	"$n�����a���F�A�X�Ӧե��A���o�A��i�y���~�F�_�ӡC",
	"�A�����a���F�ۤv�X�Ӧե��A�T�w�ۤv���O�b�@�ڡC",
	"$n�����a���F�ۤv�X�Ӧե��A�T�w$e���O�b�@�ڡC"
    },
/*
    {
	"slobber",
	"You slobber all over the floor.",
	"$n slobbers all over the floor.",
	"You slobber all over $M.",
	"$n slobbers all over $N.",
	"$n slobbers all over you.",
	"You slobber all down your front.",
	"$n slobbers all over $mself."
    },
*/
	{
     "strangle",
     "�A�|�_���⤣���D�Q�t�֪���l�H",
     "$n�|�_���⤣���D�O���O�Q�t�A����l�H",
     "�A�|�_����ΤO���t��$N����l�A�n���n�Ǧ��H�F",
	 "$n�|�_����ΤO���t��$N����l�A�n���n�Ǧ��H�F",
     "$n�|�_����ΤO���t�ۧA����l�A���A�ֳݤ��L��F",
     "�A�|�_����ΤO���t�ۦۤv����l�Q�n�ۺ�",
     "$n�|�_����ΤO���t�ۦۤv����l���G�n�ۺɡH"
	},

    {
	"smile",
	"�A�r�֪����ۡC",
	"$n�r�֪����ۡC",
	"�A��$N�r�֪��L���ۡC",
	"$n��$N�S�X�r�֪������C",
	"$n��A�r�֪����ۡC",
	"",
	""
    },
{
           "smoking",
           "�A�����l�F�f�ϡA�R�F�ӷϰ�A�����ƦV����...",
           "$n�����l�F�f�ϡA�R�F�ӷϰ�A�����ƦV����...",
           "�A��$N�I�W�F�ҡA�]���F�ۤv�@�ڡA�R�۷϶��C",
           "$n��$N�I�W�F�ҡA�]���F�ۤv�@�ڡA�R�۷϶��C",
           "$n���A�I�F�L�W���ҡA��A�L�L�@���A�@���ɦb������...",
           "�A�ʵ���}�A�I�_�⤤�̫�@�ڵҡA�����a�ݵۤѪ�...",
           "$n�t�W�a�ʦb����A�ݵۻ���A�I�_�⤤�̫�@�ڵ�..."
       },
    {
	"smirk",
	"�A���h�a���F�@�n�C",
	"$n���h�a���F�@�n�C",
	"�Aı�o$N���ܯu�O�i���C",
	"$nı�o$N���ܯu�O�i���C",
	"$nı�o�A���ܯu�O�i���C",
	"�Aı�o�A�ۤv�ݰ_�ӫܥi���C",
	"$n�ݰ_�ӫܥi���C"
    },
    {
	"sm!",
	"�A�q���W���X���@�B����A�}�l�M���H....",
	"$n�q$m���W���X���@�M����A�y�W�S�X���c�����e�C",
	"�A�����a�@����$N�G�u�֡I�s�ڤk���I�v",
	"$n�����a�@����$N�A�@��j�۵ۡG�u�֡I�s�ڤk���I�v",
	"$n�����a�@���ۧA�A�٭n�A�s$m�k���C",
	"",
	""
    }, 

/*
    {
	"smoke",
	"You calmly light a cigarette and take a puff.",
	"$n calmly lights a cigarette and take a puff.",
	"You blow smoke into $S eyes.",
	"$n blows smoke into $N's eyes.",
	"$n blows smoke rings into your eyes.",
	"You call down lightning and SMOKE yourself.",
	"$n calls down lightning and SMOKES $mself."
    },
	  
    {
	"smooch",
	"You are searching for someone to smooch.",
	"$n is looking for someone to smooch.",
	"You give $M a nice, wet smooch.",
	"$n and $N are smooching in the corner.",
	"$n smooches you passionately on the lips.",
	"You smooch yourself.",
	"$n smooches $mself.  Yuck."
    },
    
    {
	"snap",
	"PRONTO ! You snap your fingers.",
	"$n snaps $s fingers.",
	"You snap back at $M.",
	"$n snaps back at $N.",
	"$n snaps back at you!",
	"You snap yourself to attention.",
	"$n snaps $mself to attention."
    },

   {
	"snap",
	"PRONTO ! You snap your fingers.",
	"$n snaps $s fingers.",
	"You snap back at $M.",
	"$n snaps back at $N.",
	"$n snaps back at you!",
	"You snap yourself to attention.",
	"$n snaps $mself to attention."
    },

    {
	"snarl",
	"You grizzle your teeth and look mean.",
	"$n snarls angrily.",
	"You snarl at $M.",
	"$n snarls at $N.",
	"$n snarls at you, for some reason.",
	"You snarl at yourself.",
	"$n snarls at $mself."
    },
*/
    {
	"sneeze",
	"�A���F�@�ӼQ���A�u�����v",
	"$n���F�@�ӼQ���C",
	"",
	"",
	"",
	"",
	""
    },

    {
	"snicker",
	"�A���b�@�ǰ��������C",
	"$n���b�@�ǰ��������C",
	"�A�I���$N�A���b�@�ǰ��������C",
	"$n�I���$N�A���b�@�ǰ��������C",
	"$n�I��ۧA�A���b�@�ǰ��������C",
	"�A��M���F�@�Ө��c�����Y�A��O���������F�_�ӡC",
	"$n��M�������F�_�ӡA�@�w�����򤣹�l�C"
    },

    {
	"sniff",
	"�A�C�n��_�C",
	"$n�C�n��_�C",
	"�A���$N�����a�����ڳڡC",
	"$n���$N�����a�����ڳڡC",
	"$n��ۧA�����a�����ڳڡC",
	"�A�@�ӤH���b�·t�������A�@�Ʒݷݤ��k�A�������R���ˤl�C",
	"$n�@�ӤH���b�·t�������A�@�Ʒݷݤ��k�A�������R���ˤl�C"
    },

    {
	"snore",
	"�A�j�n�����I�G�uZzzzzzzzzzzzzzzzz�v",
	"$n�j�n�����I�C",
	"",
	"",
	"",
	"",
	""
    },

    {
	"snowball",
	"Snowball �֡H",
	"",
	"�A��$N�y�W��F�@�����y�C",
	"$n��$N�y�W��F�@�����y�C",
	"$n�§A�y�W��F�@�����y�C",
	"�A�¦ۤv�y�W��F�@�����y�C",
	"$n��$e�ۤv�y�W��F�@�����y�C"
    },

    {
	"snuggle",
	"�A�|�|�����A�@�ƷQ��ı���ˤl�C",
	"$n�|�|�����A�@�ƷQ��ı���ˤl�C",
	"",
	"",
	"",
	"",
	""
    },

    {
	"sob",
	"�A���n��_�C",
	"$n�����a�����ڳڡC",
	"�A�a�b$N�h�̤����a�����ڳڡC",
	"$n�a�b$N�h�̤����a�����ڳڡC",
	"$n�a�b�A�h�̤����a�����ڳڡC",
	"",""
    },
    {
	"soga",
	"�A�I�F�I�Y�A�S�X��M�j�������C",
	"$n�I�F�I�Y�A�y�W�@�ơu��Ӧp���v�����C",
	"",
	"",
	"",
	"",
	""
    },
    {
	"sorry",
	"�A�P��ܩ�p�C",
	"$n�P��ܩ�p�C",
	"�A��$N�G�����D�p�C",
	"$n��$N�G�����D�p�C",
	"$n��A�G�����D�p�C",
	"�Aı�o�A�\\����@�ɡC",
	"$nı�o�ۤv�\\����@�ɡC"
    },

    {
	"spank",
	"Spank �֡H",
	"",
	"�A�����a��$N�����ѥ��F�@�U�A�h��$E���F�_�ӡC",
	"$n�����a��$N�����ѥ��F�@�U�A�h��$E���F�_�ӡC",
	"$n�����a�§A�����ѥ��F�@�U�A�h���A���F�_�ӡC",
	"�A�~�F�@�U�ۤv�����ѡA�n�h�I",
	"$n�~�F�@�U�ۤv�����ѡC"
    },
/*
    {
	"spin",
	"You twirl in a graceful pirouette.",
	"$n twirls in a graceful pirouette.",
	"You spin $M on one finger.",
	"$n spins $N on $s finger.",
	"$n spins you around on $s finger.",
	"You spin yourself around and around and around....",
	"$n spins $mself around and around and around..."
    },
    
    {
	"squeal",
	"You squeal with delight.",
	"$n squeals with delight.",
	"You squeal at $M.",
	"$n squeals at $N.  Wonder why?",
	"$n squeals at you.  You must be doing something good.",
	"You squeal at yourself.",
	"$n squeals at $mself."
    },
    
    {
	"squeeze",
	"Where, what, how, whom?",
	"",
	"You squeeze $M fondly.",
	"$n squeezes $N fondly.",
	"$n squeezes you fondly.",
	"You squeeze yourself - try to relax a little!",
	"$n squeezes $mself."
    },

    {
	"squirm",
	"You squirm guiltily.",
	"$n squirms guiltily.  Looks like $e did it.",
	"You squirm in front of $M.",
	"$n squirms in front of $N.",
	"$n squirms in front of you.  You make $m nervous.",
	"You squirm and squirm and squirm....",
	"$n squirms and squirms and squirm....."
    },
    
    {
	"squish",
	"You squish your toes into the sand.",
	"$n squishes $s toes into the sand.",
	"You squish $M between your legs.",
	"$n squishes $N between $s legs.",
	"$n squishes you between $s legs.",
	"You squish yourself.",
	"$n squishes $mself.  OUCH."
    },
*/
    {
	"stare",
	"�A�Τ@�ز��˪����������ۦb�����C�@�ӤH�C",
	"$n�Τ@�ز��˪����������ۦb�����C�@�ӤH�C",
	"�A��M��L�Y�h�A��$N�������F�@���C",
	"$n��M��L�F�Y�A��$N�������F�@���C",
	"$n��M��L�Y�ӡA�§A�������F�@���C",
	"�A�@�ۦۤv�b�褤���ҼˡA���ۤv�������ҳ��K�C",
	"$n�@�ۦۤv�b�褤���ҼˡA��$e�ۤv�������ҳ��K�C"
    },
/*
    {
	"stickup",
	"You don a mask and ask for everyone's gold.",
	"$n says 'This is a stickup.  Gimme yer gold, NOW!'",
	"You don a mask and ask for $S gold.",
	"$n says to $N, 'This is a stickup.  Gimme yer gold, NOW!'",
	"$n says to you, 'This is a stickup.  Gimme yer gold, NOW!'",
	"You extort money from yourself.",
	"$n holds $s weapon to $s throat and says 'Ok, me, give me all my money!'"
    },
   
    {
	"stretch",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it."
    },
    
    {
	"strip",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively around the room.",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively at $N.",
	"$n shows $s bare shoulder and glances seductively at you.",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively around the room."
    },

    {
	"strut",
	"Strut your stuff.",
	"$n struts proudly.",
	"You strut to get $S attention.",
	"$n struts, hoping to get $N's attention.",
	"$n struts, hoping to get your attention.",
	"You strut to yourself, lost in your own world.",
	"$n struts to $mself, lost in $s own world."
    },

    {
	"suffer",
	"No xp again?  You suffer at the hands of fate.",
	"$n is suffering.  Looks like $e can't seem to level.",
	"You tell $M how you suffer whenever you're away from $M.",
	"$n tells $N that $e suffers whenever they're apart.",
	"$n tells you that $e suffers whenever you're apart.",
	"No xp again?  You suffer at the hands of fate.",
	"$n is suffering.  Looks like $e can't seem to level."
    },	
    
    {
	"sulk",
	"You sulk.",
	"$n sulks in the corner.",
	"",
	"",
	"",
	"",
	""
    },

    {
	"surf",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave."
    },
    */
    {
	"sweat",
	"�A���F���B�Y�W�������C",
	"$n���F���B�Y�W�������C",
	"�A��$N���F���B�Y�W�������C",
	"$n��$N���F���B�Y�W�������C",
	"$n���A���F���B�Y�W�������C",
	"",
	""
    },
/*
    {
      "sweep",
      "You grab a broom and sweep up the mess you made.",
      "$n grabs a broom and sweeps up the mess $e made.",
      "You sweep $M up off $S feet and swirl $M around in happiness.",
      "$n sweeps $N up off $S feet and swirls $M around in happiness.",
      "$n sweeps you off your feet and swirls you around in happiness.",
      "You swirl around in happiness.",
      "$n swirls $mself around.	 Pretty silly, huh?"
    },

    {
	"swoon",
	"You swoon in ecstacy.",
	"$n swoons in ecstacy.",
	"You swoon in ecstacy at the thought of $M.",
	"$n swoons in ecstacy at the thought of $N.",
	"$n swoons in ecstacy as $e thinks of you.",
	"You swoon in ecstacy.",
	"$n swoons in ecstacy."
    },

    {
	"tackle",
	"You can't tackle the AIR!",
	"",
	"You run over to $M and bring $M down!",
	"$n runs over to $N and tackles $M to the ground!",
	"$n runs over to you and tackles you to the ground!",
	"You wrap your arms around yourself, and throw yourself to the ground.",
	"$n wraps $s arms around $mself and brings $mself down!?"
    },
    
    {
	"tap",
	"You tap your foot impatiently.",
	"$n taps $s foot impatiently.",
	"You tap your foot impatiently.	 Will $E ever be ready?",
	"$n taps $s foot impatiently as $e waits for $N.",
	"$n taps $s foot impatiently as $e waits for you.",
	"You tap yourself on the head.	Ouch!",
	"$n taps $mself on the head."
    },
*/	
    {
	"swing",
	"�A�������n�ӷn�h�C",
	"$n�������n�ӷn�h�C",
	"�A�����a�n��$N�A�֧�$E�����Y���n���F�C",
	"$n�����a�n��$N�A�֧�$E�����Y���n���F�C",
	"$n�����a�n�ۧA�A�֧�A�����Y���n���F�C",
	"�A�����a�n�ӷn�h�A�ݨӭn�n��~�C���~�|���U�ӡC",
	"$n�����a�n�ӷn�h�A�ݨӭn�n��~�C���~�|���U�ӡC"
    },

    {
	"tender",
	"�A�ŬX�����G�u�S���Y�I�v",
	"$n�ŬX�����G�u�S���Y�I�v",
	"�A��$N��H���R�������C",
	"$n��$N��H���R�������C",
	"$n��A��H���R�������C",
	"�A�۹D�G�u�ڤ����ӥB�ګܷŬX��v",
	"$n�۹D�G�u�ڤ����ӥB�ګܷŬX��v"
    },

    {
	"thank",
	"�A��b�����Ҧ��H�D�¡I",
	"$n��b�����Ҧ��H�D�¡I",
	"�A�J�ߦa��$N��F�P�¤��N�C",
	"$n�J�ߦa��$N��F�P�¤��N�C",
	"$n�J�ߦa��A��F�P�¤��N�C",
	"�§A�ۤv�H",
	""
    },

    {
	"think",
	"�A�C�Y�I��C",
	"$n�C�Y�I��C",
	"�A�{�u�Ҽ{$N�����D�C",
	"$n�{�u�Ҽ{$N�����D�C",
	"$n�{�u�Ҽ{�A�����D�C",
	"�A�{�u�Ҽ{�ۦۤv�����ӡC",
	"$n�{�u�Ҽ{��$e�����ӡC"
    },

    {
	"thirsty",
	"�A����L�O���۹D�G�u���㵹�ڤ���v",
	"$n����L�O���۹D�G�u���㵹�ڤ���v",
	"",
	"",
	"",
	"",
	""
    },
  /* 
    {
	"throttle",
	"Whom do you want to throttle?",
	"",
	"You throttle $M till $E is blue in the face.",
	"$n throttles $N about the neck, until $E passes out.  THUNK!",
	"$n throttles you about the neck until you pass out.  THUNK!",
	"That might hurt!  Better not do it!",
	"$n is getting a crazy look in $s eye again."
    },
*/
    {
	"tickle",
	"Tickle �֡H",
	"",
	"�A�����$N���ުϺۤU���A�˱o$E���Ӥ����C",
	"$n�����$N���ުϺۤU���A�˱o$E���Ӥ����C",
	"$n�����A���ުϺۤU���A�˱o�A���Ӥ����C",
	"�A�����ۤv���ުϺۤU���A�˱o�ۤv���Ӥ����C",
	"$n�����$e�ۤv���ުϺۤU���A�˱o$e���Ӥ����C"
    },
/*
    {
	"twiddle",
	"You patiently twiddle your thumbs.",
	"$n patiently twiddles $s thumbs.",
	"You twiddle $S ears.",
	"$n twiddles $N's ears.",
	"$n twiddles your ears.",
	"You twiddle your ears like Dumbo.",
	"$n twiddles $s own ears like Dumbo."
    },

    {
	"timeout",
	"You take a 'T' and drink some Gatorade (tm).",
	"$n takes a 'T' and drinks some Gatorade (tm).",
	"You take a 'T' and offer $M some Gatorade (tm).",
	"$n takes a 'T' and offers $N some Gatorade (tm).",
	"$n takes a 'T' and offers you some Gatorade (tm).",
	"You take a 'T' and drink some Gatorade (tm).",
	"$n takes a 'T' and drinks some Gatorade (tm)."
    },
    
    {
	"tongue",
	"You stick out your tongue.",
	"$n sticks out $s tongue.",
	"You stick your tongue out at $M.",
	"$n sticks $s tongue out at $N.",
	"$n sticks $s tongue out at you.",
	"You stick out your tongue and touch your nose.",
	"$n sticks out $s tongue and touches $s nose."
    },

    {
	"torture",
	"You have to torture someone!",
	"",
	"You torture $M with rusty weapons, Mwaahhhhh!!",
	"$n tortures $N with rusty weapons, $E must have been REAL bad!",
	"$n tortures you with rusty weapons!  What did you DO!?!",
	"You torture yourself with rusty weapons.  Was it good for you?",
	"$n tortures $mself with rusty weapons.	 Looks like $e enjoys it!?"
    },

    {	
	"tummy",
	"You rub your tummy and wish you'd bought a pie at the bakery.",
	"$n rubs $s tummy and wishes $e'd bought a pie at the bakery.",
	"You rub your tummy and ask $M for some food.",
	"$n rubs $s tummy and asks $N for some food.",
	"$n rubs $s tummy and asks you for some food.  Please?",
	"You rub your tummy and wish you'd bought a pie at the bakery.",
	"$n rubs $s tummy and wishes $e'd bought a pie at the bakery."
    },

    {
	"tweety",
	"You exclaim 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims 'I TAWT I TAW A PUTTY TAT!!'",
	"You exclaim to $M, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $N, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to you, 'I TAWT I TAW A PUTTY TAT!!'",
	"You exclaim to yourself, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $mself, 'I TAWT I TAW A PUTTY TAT!!'"
    },
    
    {
	"twiddle",
	"You patiently twiddle your thumbs.",
	"$n patiently twiddles $s thumbs.",
	"You twiddle $S ears.",
	"$n twiddles $N's ears.",
	"$n twiddles your ears.",
	"You twiddle your ears like Dumbo.",
	"$n twiddles $s own ears like Dumbo."
    },

    {
	"type",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard."
    },
    */
    {
	"tsk",
	"�A�ɹɺ٩_�C",
	"$n�ɹɺ٩_�C",
	"�A���$N�ɹɺ٩_�C",
	"$n���$N�ɹɺ٩_�C",
	"$n��ۧA�ɹɺ٩_�C",
	"�A�����a�o�X�ɡB�ɡB�ɪ��n���C",
	"$n�����a�o�X�ɡB�ɡB�ɪ��n���C"
    },
    
    {
	"understand",
	"�A�I�F�I�Y�A�S�X��M�j�������C",
	"$n�I�F�I�Y�A�y�W�@�ơu�����F�v�����C",
	"�A���S�����A��$N���G�u�ݨӡA�@���A�����D�F�H�v",
	"$n���S�����A��$N���G�u�ݨӡA�@���A�����D�F�H�v",
	"$n���S�����A��A���G�u�ݨӡA�@���A�����D�F�H�v",
	"�Aı�o�o�ӥ@�ɤW�S���O�H�A�ѧA�C",
	"$nı�o�o�ӥ@�ɤW�S���H���A��$e�C"
    },

    {
	"viva",
	"�A�j�ۤ@�n�G�u�U���I�v",
	"$n�j�ۤ@�n�G�u�U���I�v",
	"�A�U�����q�a��$N�����U�h�A���I�G�u�U���A�U���A�U�U���I�v",
	"$n�U�����q�a��$N�����U�h�A���I�G�u�U���A�U���A�U�U���I�v",
	"$n�U�����q�a�§A�����U�h�A���I�G�u�U���A�U���A�U�U���I�v",
	"",
	""
    },
    
    {
	"waggle",
	"Waggle �֡H",
	"",
	"�A��$N�n�n����G�u�p�B�͡A�o�ˤ��n�@�I�v",
	"$n��$N�n�n����G�u�p�B�͡A�o�ˤ��n�@�I�v",
	"$n��A�n�n����G�u�p�B�͡A�o�ˤ��n�@�I�v",
	"",
	""
    },
    {
	"wait",
	"�A�b��a���F�U�ӡA�}�l���ݡC",
	"$n�b��a���F�U�ӡA�}�l���ݡC",
	"�A�b��a���F�U�ӡA�}�l��$N�C",
	"$n�b��a���F�U�ӡA�}�l��$N�C",
	"$n�b��a���F�U�ӡA�}�l���A�C",
	"�A�@�ߪ����ݵۤ�42�Ū��ɭԨ�ӡC",
	"$n�@�ߪ����ݵۤ�42�Ū��ɭԨ�ӡC"
    },

    {
	"wave",
	"�A���F����C",
	"$n���F����C",
	"�A��$N����D�O�C",
	"$n��$N����D�O�C",
	"$n��A����D�O�C",
	"�n���}�C���A��QUIT����֡C",
	""
    },

    {
	"weak",
	"�A�ݰ_�ӷ����i�����A�ͩȧO�H�ۭt�A�C",
	"$n�ݰ_�ӷ����i�����A�A�̤��|�ۭt$e�a�H",
	"�A�۱i�a��$N���G�u��I�^�h�m�@�ʦ~�A�Ӹ�ڥ��a�I�v",
	"$n�۱i�a��$N���G�u��I�^�h�m�@�ʦ~�A�Ӹ�ڥ��a�I�v",
	"$n�۱i�a��A���G�u��I�^�h�m�@�ʦ~�A�Ӹ�ڥ��a�I�v",
	"",
	""
    },

    {
	"welcome",
	"�A�N��ѯ��w��j�a�Ө�o�ӥ@�ɡC",
	"$n�N��ѯ��w��j�a�Ө�o�ӥ@�ɡC",
	"�A��$N���G�u�O�Ȯ�A���ѧڽЫȡA�ɶq�Y�C�v",
	"$n��$N���G�u�O�Ȯ�A���ѧڽЫȡA�ɶq�Y�C�v",
	"$n��A���G�u�O�Ȯ�A���ѧڽЫȡA�ɶq�Y�C�v",
	"�A��j�a���G�u�O�Ȯ�A���ѧڽЫȡA�j�a�ɶq�Y�C�v",
	"$n��j�a���G�u�O�Ȯ�A���ѧڽЫȡA�j�a�ɶq�Y�C�v"
    },

    {
	"whistle",
	"�A�r�֦a�j�ۤf��C",
	"$n�r�֦a�j�ۤf��C",
	"�A��$N�j�n���j�ۤf��C",
	"$n��$N�j�n���j�ۤf��C",
	"$n��A�j�n���j�ۤf��C",
	"�A�j�ۤf��A�۱o��֡C",
	"$n�j�ۤf��A�۱o��֡C"
    },
/*
    {
	"wedgie",
	"You look around for someone to wedgie.",
	"$n is looking around for someone to wedgie!  Run!",
	"You wedgie $M.	 Must be fun! ",
	"$n wedgies $N to the heavens.",
	"$n wedgies you!  Ouch!",
	"You delight in pinning your underwear to the sky.",
	"$n wedgies $mself and revels with glee."
    },

    {
	"whine",
	"You whine like the great whiners of the century.",
	"$n whines 'I want to be a god already.	 I need more hitpoints..I...'",
	"You whine to $M like the great whiners of the century.",
	"$n whines to $N 'I want to be an immortal already.  I need more hp...I..'",
	"$n whines to you 'I want to be an immortal already.  I need more hp...I...'",
	"You whine like the great whiners of the century.",
	"$n whines 'I want to be a god already.	 I need more hitpoints..I...'"
    },
    
    {
	"wiggle",
	"Your wiggle your bottom.",
	"$n wiggles $s bottom.",
	"You wiggle your bottom toward $M.",
	"$n wiggles $s bottom toward $N.",
	"$n wiggles his bottom toward you.",
	"You wiggle about like a fish.",
	"$n wiggles about like a fish."
    },

    {
	"wince",
	"You wince.  Ouch!",
	"$n winces.  Ouch!",
	"You wince at $M.",
	"$n winces at $N.",
	"$n winces at you.",
	"You wince at yourself.  Ouch!",
	"$n winces at $mself.  Ouch!"
    },

*/
    {
	"wink",
	"�A�w�F�w�����I",
	"$n�w�F�w�����I",
	"�A��$N�w�F�w�����I",
	"$n��$N�w�F�w�����I",
	"$n��A�w�F�w�����I",
	"��ۤv�w���H�ݱo��ܡH",
	""
    },
/*
    {
	"wish",
	"You make a wish upon a star for a dream you have.",
	"$n sighs... and makes a wish upon a shooting star.",
	"You wish to be loved by $M.",
	"$n wishes to be loved by $N.",
	"$n wishes to be loved by you.",
	"You wish for more attention.",
	"$n wishes for more attention."
    },
*/
    /*{
	"worship",
	"You worship the powers that be.",
	"$n worships the powers that be.",
	"You drop to your knees in homage of $M.",
	"$n prostrates $mself before $N.",
	"$n believes you are all powerful.",
	"You worship yourself.",
	"$n worships $mself - ah, the conceitedness of it all."
    },*/
	
    { // by skalt@ROC
        "worm",
        "�A�����ί�b�a�Wį�ʵۡC",
        "$n�����ί�b�a�Wį�ʵۡC",
        "�A�����a�����Υ��F$N����A�̡C",
        "$n�����a�����Υ��F$N����A�̡C",
        "$n�����a�����Υ��F�A����A�̡C",
        "�A�j�s�@�n�A�յۧ�X�b�I�W���ʪ����ΡC",
        "$n�W�i�����$s�I�A�@���j�ۡG�u���ΡA���ΰ�~><~�v"
    },

    {
	"wow",
	"�A��@�����P��ܳY���C",
	"$n�����f�b�A�n���ݨ�F���򤣥i��ĳ���Ʊ��C",
	"",
	"",
	"",
	"",
	""
    },
    /*
    {
	"wrap",
	"You wrap a present for your love.",
	"$n wraps a present for someone special.",
	"You wrap your legs around $M.",
	"$n wraps $s legs around $N.",
	"$n wraps $s legs around you.  Wonder what's coming next?",
	"You wrap yourself with some paper.",
	"$n wraps $mself with some wrapping paper.  Is it Christmas?"
    },
*/
    {
	"worry",
	"�A��M�P����W����ߡI",
	"$n��M�P����W����ߡI",
	"�A��$N�W�f�C�ߦa���G�u�Ĥl�A�A�o�˹�b���H�ܾ�߭C�C�v",
	"$n��$N�W�f�C�ߦa���G�u�Ĥl�A�A�o�˹�b���H�ܾ�߭C�C�v",
	"$n��A�W�f�C�ߦa���G�u�Ĥl�A�A�o�˹�b���H�ܾ�߭C�C�v",
	"�A��ۤv�����p�P��ܾ�ߡC",
	"$n��$e�ۤv�����p�P��ܾ�ߡC"
    },

    {
	"ya",
	"�A�S�X�ӧQ�����A�uYA��I�v",
	"$n�S�X�ӧQ�����A�uYA��I�v",
	"�A��$N�S�X�ӧQ�����A�uYA��I�v",
	"$n��$N�S�X�ӧQ�����A�uYA��I�v",
	"$n��A�S�X�ӧQ�����A�uYA��I�v",
	"",
	""
    },
    
    {
	"yabba",
	"YABBA-DABBA-DOO!",
	"$n �j�۵� '�K�I�º� -- YABBA DABBA DOO!'",
	"�A�j�۵� '�K�I$N -- YABBA DABBA DOO!'",
	"$n �j�۵� '�K�I$N -- YABBA DABBA DOO!'",
	"$n �j�۵� '�K�I$N -- YABBA DABBA DOO!'",
	"YABBA-DABBA-DOO!",
	"$n �j�۵� '�K�A�º� -- YABBA DABBA DOO!'"
    },
  /* 
    {
	"yahoo",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, kid, that was one in a million!'",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, $N, that was one in a million!'",
	"$n exclaims, 'YAHOO!  Great shot, $N, that was one in a million!'",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, $n, that was one in a million!'"
    },*/
    {
	"yawn",
	"�A�u����v���@�n�A���F�@�ӫ���C",
	"$n�u����v���@�n�A���F�@�ӫ���C",
	"�A�u����v���@�n�A��$N���F�@�ӫ���C",
	"$n�u����v���@�n�A��$N���F�@�ӫ���C",
	"$n�u����v���@�n�A��A���F�@�ӫ���C",
	"",
	""
    },

	{
	"tea",
	"�A�X�۶��ӵL�ƪ��ɭ��P�F�@���n���A�D�ۯ����n���y�v��",
	"$n�y���۱o���P�F�@�����A�H�H�������ƴ��b�|�P��",
	"�A��$N�ˤW�F�M���A�]���F�ۤv�@�M�A�H���N�s�B�������ͱ��ˡC",
	"$n��$N�ˤW�F�M���A�]���F�ۤv�@�M�A�ǤH�b�������Ϥ��Z�ֹ�͡C",
	"$n���A�ˤF�M���A�M�����O�ͱ��]�O����A���ͦp���A���ͦ�D�H",
	"�A���ۤv�ˤF�M���A�ݵ۵[�Ħ⪺�����Q�۩��骺�غءC",
	"$n�b�@�ǳܰ_�F�ѤH���A�Q�۹L�h�غػP����n�͡C"
	},

	{
	"twoone",
	"�A�V�W�Ѭ�ë���n�Q�G�@�A���L�B�����G�ܦ��F�G�@�r��....",
	"$n�~�{���ݵۤѪšA����ë�ۤ��n�Q�G�@",
	"�A�ݤF��$N�A��l���䮳�X���j��\"�G�@�L\"��$N�y�W�����ӥh�I",
	"$n�b$N�y�W�\\�F�@�Ӥj�j���G�@�A�u�O�ݧԡC",
	"$n�L������A�ϥΤF�ǻ�����\"�G�@�L\"�A�A�y��ı�o�e�~�L�G",
	"�A���X�Ҫ�}�l�C�C��Ǥ�....���A�j�դw�h....",
	"$n���۽Ҫ�b�@���t����A�M��ĹD�G�u�G�E�@�E�F....�v"
	},

	{
	"marry",
	"�A���X�@���A��A�ù�j�a���G�u�֭n�����ڧr�I�v",
	"$n���X�@���A��Ҧ��H�j�ۡG�u�֭n�����ڧr�I�v",
	"�A���X�@���A��V$N���������G�u�����ڧa�A�̲﨩���I�v",
	"$n���X�@���A��V$N���������G�u�����ڧa�A�̲﨩���I�v",
	"$n���X�@���A�ᮨ������A���G�u�����ڧa�A�̲﨩���I�v",
	"�A���D�n��ۤv���B�H",
	"$n��ۤv���G�u�ڭn���B�I�v"
	},

	{
	"bed",
	"�A�q�I�᮳�X�F�@�i�ɡA�ý��b�W��",
	"$n�q�I�᮳�X�@�i�R���p�ɡA���ǬN�����b�W��",
	"�A���X�@�i�R���p�ɡA�ù�$N���G�u$N��$N��ڭ̤@�_�Χa�I�v",
	"$n���X�@�i�R���p�ɦV$N���G�u$N��$N��ڭ̤@�_�Χa�I�v",
	"$n��X�@�i�R���p�ɦV�A���G�u$N��$N��ڭ̤@�_�Χa�I�v",
	"�A���X�@�i�R���p�ɡA�ۤv�@�ӤH�Φb�ɤW",
	"$n���X�R���p�ɡA�ۤv�@�ӤH�Φb�ɤW�A�n�~�D��"
	},

	{
	"ohoho",
	"�p���Ӥk����A�o�X�����������n�C",
	"$n���e�y�A�t�@���b�L��A�o�X�����������n�C�@�����������",
	"�p���e�y�A�t�@���b�L��A���$N�o�X�����������n�C",
	"$n���$N�o�X�����������n�C�@�����������",
	"$n���e�y�A�t�@���b�L��A��۩p�o�X�����������n�C�@�����������",
	"�p���Ӥk����A�o�X�����������n�C",
	"$n���Ӥk����A�o�X�����������n�C  �@�����������"
	},

	{
	"oldbie",
	"�A��ۤj�a���𪺻��G�u�ڬO�Ѥ��ݭn������ڴN��F�I�v",
	"$n��ۤj�a���𪺻��G�u�ڬO�Ѥ��ݭn������ڴN��F�I�v",
	"�A�N$N��i����»���A�\\�쯫�¤W�}�l�����C",
	"$n�N$N��i����»�̡A�\\�b����W�@�۪������ۡC",
	"$n��A�������\\�쯫��W�A��ۧA�@�۪������ۡC",
	"�A��ۤj�a�g���D�G�u��ڳo�ӦѤ��A�A�̳��ӹ�F�ڡI�v",
	"$n��ۤj�a�g���D�G�u��ڳo�ӦѤ��A�A�̳��ӹ�F�ڡI�v"
	},

	{
	"avert",
	"�A�w�w�N�إ��ಾ�A���ˤ��򳣨S�ݨ�C",
	"$n��M�N�Y�n�V�@��A���˦b�Y��ۻ��誺�����C",
	"�A���F�{��$N���إ��A���۵M���N�Y��V����C",
	"$n���F�{��$N���إ��A���۵M���N�Y��V����C",
	"$n���F�{�קA���إ��A���۵M���N�Y��V����C",
	"�A�b���H���إ��U�⨬�L���A�u��|�B�i��ۡC",
	"$n�ݰ_�Ӥ⨬�L���A��i���|�B�i��ۡC"
	},

	{
    "sting",
    "�A�ǥX���Ǥp���w��A�G�p��H�A�ǳƶ}�l�@�k�C",
    "$n���ۤ@�ڲӪ������w�A�٦l�c�����ۡA�����D�Q�Fԣ�a�ơC",
    "�A�b��H�W�g�W�F$N���W�r�A�}�l�ƨg����H���w�C���C",
    "$n���b���t�������A�f������ۡG�u�i�c��$N....�ڴ��A�Ӥp�H�Y....�v�C",
    "�A�ݨ�$n���w�µۯ�H��U�h�A�y�ɧA������h�A������g�C",
    "�A�b��H�W�g�W�ۤv���W�r�A�M��r�O�@��--OUCH�I�n�h��><�C",
    "$n��F��H�@�U�A�H�Y�h�����W�b�Ťj�ۡG�u�n�Q�����Pı�ڡI�v�C"
	},

	{
	"strut",
	"�A�k������a�̨Ӯ̥h�C",
	"$n�k������a�̨Ӯ̥h�C",
	"�A�o�N���b$N���e�̨Ӯ̥h�C",
	"$n�o�N���b$N���e�̨Ӯ̥h�C",
	"$n�o�N���b�A���e�̨Ӯ̥h, �@�ƫܤ�󪺼ˤl�C",
	"�A�N�𭷵o���b�o��Ӧ^��B�C",
	"$n�N�𭷵o���b�o��Ӧ^��B�C"
	},

	{
	"fossil",
	"�A�|�B����, �åB���a��ۡu�o�䤣���D���S���ƥ۩O�H�v",
	"$n�|�B����, ��o�a����o�@�Ӭ}�@�Ӭ}�a�C",
	"�A��ۤj�a�j�ۡu�ڵo�{�öQ���ƥۤF�I�v, �åB�Τ����$N",
	"$n��ۤj�a�j�ۡu�ڵo�{�öQ���ƥۤF�I�v, �åB�Τ����$N",
	"$n��ۤj�a�j�ۡu�ڵo�{�öQ���ƥۤF�I�v, �åB�Τ���ۧA",
	"�A�j�n�a�۵ۡu���n�A�s�ڤƥۤF >_<�v",
	"$n�j�n�a�۵ۡu���n�A�s�ڤƥۤF >_<�v"
	},
	{
	"tear",
	"�A��_�Y�A�q�q�a�ݵۤѪ�, ���\\���ѦۥD���y�U....�C",
	"$n��_�Y�A�q�q�a�ݵۤѪ�, ���\\���ѦۥD���y�U....�C",
	"�A���_�⩬�A�ŬX����$N���h�������\\��....�C",
	"$n���_�⩬�A�ŬX����$N���h�������\\��....�C",
	"$n���_�⩬�A�ŬX�����p���h�������\\��....�C",
	"�A�C�U�Y�ӡA�����ۤv�������\\��....�C",
	"$n�C�U�Y�ӡA�����ۤv�������\\��....�C"
	},
	{
	"pant",
	"�A�����j���A���y�q���a�j�n�ݮ�C",
	"$n�����j���A���y�q�����j�n�ݮ�C",
	"�A���y�q���a�b$N�����仴���a�ݮ��C",
	"$n���y�q�����µ�$N������ݮ�C",
	"$n���y�q���a�b�A�����仴���a�ݮ��C",
	"�A�����y�A�ɤO����ۦۤv���ݮ��C",
	"$n�����y�A�ɤO����ۦۤv���ݮ��C"
	},
	{
	"kneel",
	"�A�宣�����U�A�j�n�a���G�u���~�U�������F�I�v",
	"$n�宣�����U�A�j�n�a���G�u���~�U�������F�I�v",
	"�A���q�a�潥���b$N�����e�A�˧k�L����I�C",
	"$n���q�a�潥���b$N�����e�A�˧k�L����I�C",
	"$n���q�a�b�A���e�潥���U�A�˧k�A����I�C",
	"�A���ۤ���A�����𨤸��b��L�W�C",
	"$n���ۤ���A����𨤸��b��L�W�C"
	},
	{
	"angel",
	"�A�����s��a���D�G�u���٦b�M���ݩ�ڪ��Ѩ�... �v",
	"$n�����s��a���D�G�u���٦b�M���ݩ�ڪ��Ѩ�... �v",
	"�A��Mı�o$N�N���ѨϡA���T�E�ʦa����o�����⻡�D�G�u$N�A�A�u�O�ڪ��ѨϡI�v",
	"$n��M���o�@�y�a����$N������A�H�ʼ}�������復���D�G�u$N�A�A�O�ڪ��ѨϡI�v",
	"$n�r�M���o�@�y�a����A������A�����ʼ}�a��A���D�G�u$N�A�A�O�ڪ��ѨϡI�v",
	"�b�o�@�����A�Aı�o�ۤv���ѨϤ@�믫�t�¼�C",
	"�b�A���Ǫ�$n��Mı�o�o�ۤv���ѨϤ@�믫�t�¼�C"
	},
	{
     	"niceman",
     	"�A���n�j�ۡG�u���S���H�n�[�J�n�H�Ѽֳ��ڡI�v",
     	"$n���n�j�ۡG�u���S���H�n�[�J�n�H�Ѽֳ��ڡI�v",
     	"�A�a�ۺp����������$N���G�u�A�O�@�Ӧn�H....�v",
     	"$n�a�ۺp����������$N���G�u�A�O�@�Ӧn�H....�v�A�n�H�Ѽֳ��S�h�@�ӤH�F�I",
     	"$n�a�ۺp����������A���G�u�A�O�@�Ӧn�H....�v",
	"�A���檺�j�ۡG�u�ڤ��Q��n�H�ڡI�v",
     	"$n���檺�j�ۡG�u�ڤ��Q��n�H�ڡI�v"
	},
	{
     	"notfunny",
     	"�Aı�o�@���u�O�ӵL��F�C",
     	"$n�ĵۤ@�f�𻡡G�u�� ... �u�L��C�v",
     	"�A�ܻ{�u�a��$N���G�u�@ �I �� �� �n �� ...�C�v",
     	"$n�a�ۻ{�u��������$N���G�u�@ �I �� �� �n �� ...�C�v",
     	"$n�a�ۻ{�u��������A���G�u�@ �I �� �� �n �� ...�C�v",
     	"�A��ۤv�����ܷP���h�æa���G�u��~ �o�������ܡH�v",
     	"$n�s��a���G�u��~ �o�������ܡH�v",
	},
	{
    	"crash",
    	"�A�J�ߦa��D�@�����w�A���n������N�~�o�͡C",
     	"$n��M��Ѭ�ë�A�Ʊ椵�Ѥ��ncrash�C",
   	"�A�ͮ�a��$N���G�u���O�A�` ROC crash ����!!�v",
    	"$n�ͮ�a��$N���G�u���O�A�` ROC crash ����!!�v",
     	"$n�ͮ�a��A���G�u���O�A�` ROC crash ����!!�v",
     	"�A�}�l�h�æۤv�O���O�� ROC crash�H",
     	"$n�ߵ�a���G�u���|�O�]���� crash ���a ...�C�v"
	},
	{
     	"impossible",
     	"�Aı�o�@�ɤW���|���o�بƱ��o�͡C",
     	"$n�K�۬��Y���G�u���i��a ...�C �v",
     	"�ھڧA���g��A�Aı�o$N�줣��C",
     	"$n�n�ߦa�U$N���G�u���i��a ...�C �v",
     	"$n��A���R�Ʊ����u�ۡA�A�}�l��ۤv���H�߰ʷn�F�C",
     	"�Aı�o�o�بƤ��|�o�ͦb�A���W�C",
     	"$n��ۤv���G�u�o�بƹ�ڨӻ��O�S�i�઺�r�C�v"
	},
	{
     	"cantbe",
     	"�A�}�l�`�ȨƱ��w�g���O�A�ү�Q�����C",
     	"$n�D�`���౵���a���G�u��...���|�a...�C�v",
     	"�A�}�l��$N�_�F�äߡC",
     	"$n���h�n�N�a��$N���G�u�Ӥ��|�O�A�a ...�C�v",
     	"$n���h�n�N�a��A���G�u�Ӥ��|�O�A�a ...�C�v",
     	"�A���}�l�h�ð_�ۤv�ӤF�H�I",
     	"$n�]�]�R�R�a���G�u�� ... ���|�O�ڧa .. �C�v"
	},
	{
    	 "nosmoking",
     	"�A�}�l�j�n�Ŵ��۩ڧl���դG��Ϫ��z���C",
     	"$n�}�l�j�n�Ŵ��۩ڧl�G��Ϫ��z���C",
     	"�A�µ�$N��F�@���, �åB�j�n�a���G�u�ڧl�G���!�v",
     	"$n�µ�$N��F�@���, �åB�j�n�a���G�u�ڧl�G���!�v",
     	"$n�µۧA��F�@���, �åB�j�n�a���G�u�ڧl�G���!�v",
     	"�A��B�K�۩ڧl�G��Ϫ��s�i�C",
     	"$n��B�K�۩ڧl�G��Ϫ��s�i�C��!, $e���M��s�i�K��A�y�W�F!!"
	},
   /* 
    {
	"yeehaw",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'"
    },
*/
    {
	"",
	"", "", "", "", "", "", ""
    }
};



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
	CHAR_DATA *victim;
	char       arg [ MAX_INPUT_LENGTH ];
	int	       cmd;
	bool       found;

	found  = FALSE;
	for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( command[0] == social_table[cmd].name[0]
				&& !str_prefix( command, social_table[cmd].name ) )
		{
			found = TRUE;
			break;
		}
	}

	if ( !found )
		return FALSE;

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
	{
		send_to_char( "�A�L�k�ϥΥ�������O�I\n\r", ch );
		return TRUE;
	}

	switch ( ch->position )
	{
		case POS_DEAD:
			send_to_char( "�R�R���ۧa�A�A�w�g���F.\n\r", ch );
			return TRUE;

		case POS_INCAP:
		case POS_MORTAL:
			send_to_char( "�A�����p���V�F.\n\r", ch );
			return TRUE;

		case POS_STUNNED:
			send_to_char( "�A�Y������.\n\r", ch );
			return TRUE;

		case POS_SLEEPING:
			/*
			 * I just know this is the path to a 12" 'if' statement.  :(
			 * But two players asked for it already!  -- Furey
			 */
			if ( !str_cmp( social_table[cmd].name, "snore" ) )
				break;
			send_to_char( "�A���b�ιڤ��A�Х� wake �_�ӡC\n\r", ch );
			return TRUE;

	}

	one_argument( argument, arg );
	victim = NULL;
	if ( arg[0] == '\0' )
	{
		act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR );
		act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM );
	}
	else if ( !( victim = get_char_world( ch, arg ) ) )
	{
		send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
	}
	else if ( victim == ch )
	{
		act( social_table[cmd].char_auto, ch, NULL, victim, TO_CHAR );
		act( social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM );
	}
	else if ( !get_char_room( ch, arg ) && can_see( ch, victim ) )
	{
		if ( !IS_NPC( victim ) )
		{

			ROOM_INDEX_DATA *original;
			char *ldbase = "�q�������a��, ";
			char ldmsg [ MAX_STRING_LENGTH ];
			CHAR_DATA	*   fighting = NULL;

			if( ch->fighting ) fighting = ch->fighting;

			original = ch->in_room;
			char_from_room( ch );
			char_to_room( ch, original );
			strcpy( ldmsg, ldbase );
			if( social_table[cmd].char_found[0] != '\0' )
			{
				strcat( ldmsg, social_table[cmd].char_found );
				act( ldmsg, ch, NULL, victim, TO_CHAR );
			}
			if( social_table[cmd].vict_found[0] != '\0' ) {
				strcpy( ldmsg, ldbase );
				strcat( ldmsg, social_table[cmd].vict_found );
				act( ldmsg, ch, NULL, victim, TO_VICT );

			}
			char_from_room( ch );
			char_to_room( ch, original );
			if(fighting){ 
				stop_fighting(ch, FALSE);
				set_fighting(ch, fighting);
				set_fighting(fighting, ch);	//Added by Razgriz    20050618
			}
		}
		else
		{
			send_to_char( "�o�̨S���o�ӤH.\n\r", ch );
		}
	}
	else
	{
		act( social_table[cmd].char_found, ch, NULL, victim, TO_CHAR );
		act( social_table[cmd].vict_found, ch, NULL, victim, TO_VICT );
		act( social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT );

		if ( !IS_NPC( ch )
				&& IS_NPC( victim )
				&& !IS_AFFECTED( victim, AFF_CHARM )
				&& IS_AWAKE( victim ) )
		{
			if ( !can_see( victim, ch ) )
			{
				send_to_char( "�L�ݤ���A�I\n\r", ch );
				return TRUE;
			}

			if (mprog_act_trigger( social_table[cmd].name,ch, victim))
				return TRUE;

			switch ( number_bits( 4 ) )
			{
				case 0:
					if ( ( victim->level < ch->level ) && !( victim->fighting ) ){
						multi_hit( victim, ch, TYPE_UNDEFINED );
					}
					break;

				case 1: case 2: case 3: case 4:
				case 5: case 6: case 7: case 8:
					act( social_table[cmd].char_found, victim, NULL, ch, TO_CHAR );
					act( social_table[cmd].vict_found, victim, NULL, ch, TO_VICT );
					act( social_table[cmd].others_found, victim, NULL, ch, TO_NOTVICT );
					break;

				case 9: case 10: case 11: case 12:
					act( "�A�����a���F$N�X�Ӧե��A����$E��i�y���~�F�_�ӡC", victim, NULL, ch, TO_CHAR );
					act( "$n�����a���F�A�X�Ӧե��A�����A��i�y���~�F�_�ӡC", victim, NULL, ch, TO_VICT );
					act( "$n�����a���F$N�X�Ӧե��A����$E��i�y���~�F�_�ӡC", victim, NULL, ch, TO_NOTVICT );
					break;
			}
		}
	}

	return TRUE;
}
