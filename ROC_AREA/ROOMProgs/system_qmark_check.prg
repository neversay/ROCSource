>room_defun quest_list~
at $b echoat $b [1;37m�A�i�H�d�ݪ����Ȱϰ�:[0m
at $b echoat $b �Ĺs�� [1;33m���s���W�[�a�h�]��             [37m(      00       )[0m
at $b echoat $b �Ĥ@�� [1;32m�v�d�Ԩ��ζg��ϰ� I           [37m(      01       )[0m
at $b echoat $b �ĤG�� [1;32m�v�d�Ԩ��ζg��ϰ� II          [37m(      02       )[0m
at $b echoat $b �ĤT�� [1;32m�v�d�Ԩ�����ϰ� I             [37m(      03       )[0m
at $b echoat $b �ĥ|�� [1;32m�v�d�Ԩ�����ϰ� II            [37m(      04       )[0m
at $b echoat $b �Ĥ��� [1;32m�v�d�Ԩ��F��ϰ�Τj�F�z�� I   [37m(      05       )[0m
at $b echoat $b �Ĥ��� [1;32m�v�d�Ԩ��F��ϰ�Τj�F�z�� II  [37m(      06       )[0m
at $b echoat $b �ĤC�� [1;32m����@�ɥH�Υ~��a�� I         [37m(      07       )[0m
at $b echoat $b �ĤK�� [1;32m����@�ɥH�Υ~��a�� II        [37m(      08       )[0m
at $b echoat $b �ĤE�� [1;36m�g�����q�D��                   [37m(      09       )[0m
at $b echoat $b �ĤQ�� [1;36m�g�����q�`�Ϥ���               [37m(      10       )[0m
at $b echoat $b ��11�� [1;36m�g�����q����L�ϰ� I           [37m(      11       )[0m
at $b echoat $b ��12�� [1;36m�g�����q����L�ϰ� II          [37m(      12       )[0m
at $b echoat $b ��13�� [1;36m�g�����q����L�ϰ� III         [37m(      13       )[0m
def_rembch null-tar
~
>room_defun q00~
at $b echoat $b .             [1;36m[���s���W�[�a�h�]��][0m
at $b echoat $b .             [1;37m{���s���W}[0m
if is_qmark($b) == dragonbay_mayor_secret 100010
or is_qmark($b) & dragonbay_quest_mark 100004
        at $b echoat $b [1;35m���蠟�H                     ����[0m
else
        at $b echoat $b [1;32m���蠟�H                     ����[0m
endif
at $b echoat $b .             [1;37m{�]���O�L}[0m
if is_qmark($b) & dragonbay_quest_mark 200001
        at $b echoat $b [1;35m�j���Ѫ��Ʊ�                 ����[0m
else
        at $b echoat $b [1;32m�j���Ѫ��Ʊ�                 ����[0m
endif
if is_qmark($b) & dragonbay_quest_mark 200002
        at $b echoat $b [1;35m�U�N                         ����[0m
else
        at $b echoat $b [1;32m�U�N                         ����[0m
endif
if is_qmark($b) & dragonbay_quest_mark 200004
        at $b echoat $b [1;35m�귽�^��                     ����[0m
else
        at $b echoat $b [1;32m�귽�^��                     ����[0m
endif
def_rembch null-tar
~
>room_defun q01~
at $b echoat $b .             [1;36m[�v�d�Ԩ��ζg��ϰ� I][0m
at $b echoat $b .             [1;37m{�v�d�Ԩ���}[0m
if is_qmark($b) == skara_baby_quest1 100005
        at $b echoat $b [1;35m�Ѫ̪��U�I                   ����[0m
else
        at $b echoat $b [1;32m�Ѫ̪��U�I                   ����[0m
endif
if is_qmark($b) == skara_baby_quest2 100003
        at $b echoat $b [1;35m��̪��s��                   ����[0m
else
        at $b echoat $b [1;32m��̪��s��                   ����[0m
endif
if is_qmark($b) == skara_baby_quest3 100003
        at $b echoat $b [1;35m�ζ��P�X�@                   ����[0m
else
        at $b echoat $b [1;32m�ζ��P�X�@                   ����[0m
endif
if is_qmark($b) == midennir_lostboy_exp
        at $b echoat $b [1;35m�J�{������                   ����[0m
else
        at $b echoat $b [1;32m�J�{������                   ����[0m
endif
if primeattr($b) == 2
or class($b) == 2
or class($b) == 9
or class($b) == 10
or class($b) == 17
        at $b echoat $b [1;30m(��)[37m�s�餽�|����:[0m
if is_qmark($b) !& thief_quest_list 100001
        at $b echoat $b [1;35m--���K�լd                   ����[0m
else
        at $b echoat $b [1;32m--���K�լd                   ����[0m
endif
if is_qmark($b) !& thief_quest_list 100002
        at $b echoat $b [1;35m--�������                 ����[0m
else
        at $b echoat $b [1;32m--�������                 ����[0m
endif
if is_qmark($b) !& thief_quest_list 100008
        at $b echoat $b [1;35m--����ŪŨ�                 ����[0m
else
        at $b echoat $b [1;32m--����ŪŨ�                 ����[0m
endif
if is_qmark($b) !& thief_quest_list 100004
        at $b echoat $b [1;35m--�t�q����                   ����[0m
else
        at $b echoat $b [1;32m--�t�q����                   ����[0m
endif
if is_qmark($b) !& thief_quest_list 100032
        at $b echoat $b [1;35m--�T��                       ����[0m
else
        at $b echoat $b [1;32m--�T��                       ����[0m
endif
if is_qmark($b) !& thief_quest_list 100016
        at $b echoat $b [1;35m--�t��                       ����[0m
else
        at $b echoat $b [1;32m--�t��                       ����[0m
endif
if is_qmark($b) !& thief_quest_list 100064
        at $b echoat $b [1;35m--��{�t��                   ����[0m
else
        at $b echoat $b [1;32m--��{�t��                   ����[0m
endif
if is_qmark($b) !& thief_quest_list 100128
        at $b echoat $b [1;35m--�Ť�J�դb                 ����[0m
else
        at $b echoat $b [1;32m--�Ť�J�դb                 ����[0m
endif
if is_qmark($b) == thief_quest_reward 100001
        at $b echoat $b [1;35m--�L���ŷt���c               ����[0m
else
        at $b echoat $b [1;32m--�L���ŷt���c               ����[0m
endif
endif
if primeattr($b) == 3
or class($b) == 0
or class($b) == 5
or class($b) == 6
or class($b) == 15
        at $b echoat $b [1;30m(��)[37m�k�v���|����:[0m
if is_qmark($b) == mage_envoy_quest 100024
        at $b echoat $b [1;35m--���a�K��                   ����[0m
else
        at $b echoat $b [1;32m--���a�K��                   ����[0m
endif
endif
if is_qmark($b) == enable_level_40
        at $b echoat $b [1;35m�i�̺ٸ�                     ����[0m
else
        at $b echoat $b [1;32m�i�̺ٸ�                     ����[0m
endif
if is_qmark($b) == enable_level_41
        at $b echoat $b [1;35m�^���ٸ�                     ����[0m
else
        at $b echoat $b [1;32m�^���ٸ�                     ����[0m
endif
if is_qmark($b) == skarabrae_joe_quest 100004
        at $b echoat $b [1;35m���Ŧh���дo                 ����[0m
else
        at $b echoat $b [1;32m���Ŧh���дo                 ����[0m
endif
if is_qmark($b) == commerce_crystal_quest 100006
        at $b echoat $b [1;35m�]�_�˩w                     ����[0m
else
        at $b echoat $b [1;32m�]�_�˩w                     ����[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100001
        at $b echoat $b [1;35m�x�����ѥ�                   ����[0m
else
        at $b echoat $b [1;32m�x�����ѥ�                   ����[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100002
        at $b echoat $b [1;35m�U�h���L���e�U               ����[0m
else
        at $b echoat $b [1;32m�U�h���L���e�U               ����[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100004
        at $b echoat $b [1;35m�O�K����                     ����[0m
else
        at $b echoat $b [1;32m�O�K����                     ����[0m
endif
if is_qmark($b) == valentine_quest 300511
or is_qmark($b) == valentine_quest 301023
        at $b echoat $b [1;35m�R��§��                     ����[0m
else
        at $b echoat $b [1;32m�R��§��                     ����[0m
endif
if is_qmark($b) != commerce_enemy
at $b echoat $b .             [1;37m{�v�d�Ԩ��ӷ|}[0m
if is_qmark($b) == adv_guild_level
	at $b echoat $b [1;30m�_�I�̤��|����:[0m
	if is_qmark($b) & adv_guild_level 300004
		at $b echoat $b [1;35m--�N���e�U�R�n��             ����[0m
	else
		if is_qmark($b) & adv_guild_level 300002
			at $b echoat $b [1;35m--���Ư��                   ����[0m
		else
			if is_qmark($b) & adv_guild_level 300001
				at $b echoat $b [1;35m--���|�s�Ѫ�                 ����[0m
			endif
		endif
	endif
endif
if is_qmark($b) == commerce_noob_mage 300001
or is_qmark($b) == commerce_noob_mage 300002
or is_qmark($b) & commerce_subquest 100001
or is_qmark($b) & commerce_subquest 200001
		at $b echoat $b [1;35m��X�T�f���k�v               ����[0m
else
		at $b echoat $b [1;32m��X�T�f���k�v               ����[0m
endif
if is_qmark($b) == scar_adv_helping 300001
or is_qmark($b) == scar_adv_helping 300002
or is_qmark($b) & commerce_subquest 100002
or is_qmark($b) & commerce_subquest 200002
		at $b echoat $b [1;35m�_�I�̪����q                 ����[0m
else
		at $b echoat $b [1;32m�_�I�̪����q                 ����[0m
endif
if is_qmark($b) & commerce_subquest 100004
		at $b echoat $b [1;35m�a�U����U                   ����[0m
endif
if is_qmark($b) & commerce_subquest 100008
		at $b echoat $b [1;35m�ҥ�������                   ����[0m
endif
if is_qmark($b) == pharmacist_level
	if is_qmark($b) & pharmacist_level 100004
		at $b echoat $b [1;30m(��)[0m[1;35m�ī~�妨 I               ����[0m
	else
		at $b echoat $b [1;30m(��)[0m[1;32m�ī~�妨 I               ����[0m
	endif
endif
endif
def_rembch null-tar
~
>room_defun q02~
at $b echoat $b .             [1;36m[�v�d�Ԩ��ζg��ϰ� II][0m
at $b echoat $b .             [1;37m{�p���F��}[0m
if is_qmark($b) == smurf_save 100002
        at $b echoat $b [1;35m���p���F�X�f��               ����[0m
else
        at $b echoat $b [1;32m���p���F�X�f��               ����[0m
endif
at $b echoat $b .             [1;37m{������}[0m
if is_qmark($b) & circus_quest 100001
        at $b echoat $b [1;35m�ѱϤu�@�H��                 ����[0m
else
        at $b echoat $b [1;32m�ѱϤu�@�H��                 ����[0m
endif
if is_qmark($b) & circus_quest 100002
        at $b echoat $b [1;35m�p�j�񪺶���                 ����[0m
else
        at $b echoat $b [1;32m�p�j�񪺶���                 ����[0m
endif
at $b echoat $b .             [1;37m{�K�n���˪L}[0m
if is_qmark($b) == midennir_bard_quest 100002
        at $b echoat $b [1;35m�u�C�֤H���q                 ����[0m
else
        at $b echoat $b [1;32m�u�C�֤H���q                 ����[0m
endif
if is_qmark($b) >= midennir_quest 100001
        at $b echoat $b [1;35m��������L�`��               ����[0m
else
        at $b echoat $b [1;32m��������L�`��               ����[0m
endif
at $b echoat $b .             [1;37m{�_�޳h����}[0m
if is_qmark($b) & slum_libei_quest 100001
        at $b echoat $b [1;35m���s�u                       ����[0m
else
        at $b echoat $b [1;32m���s�u                       ����[0m
endif
at $b echoat $b .             [1;37m{���H�_��}[0m
if is_qmark($b) == wererat_save 100002
        at $b echoat $b [1;35m�i�����}��                   ����[0m
else
        at $b echoat $b [1;32m�i�����}��                   ����[0m
endif
at $b echoat $b .             [1;37m{�_�襭��}[0m
if is_qmark($b) == plains_slaver_quest 200004
        at $b echoat $b [1;35m�����c�l                     ����[0m
else
        at $b echoat $b [1;32m�����c�l                     ����[0m
endif
at $b echoat $b .             [1;37m{�а�ӵ�}[0m
if is_qmark($b) == chapel_question 100004
        at $b echoat $b [1;35m�Ӷ餧��                     ����[0m
else
        at $b echoat $b [1;32m�Ӷ餧��                     ����[0m
endif
at $b echoat $b .             [1;37m{���s����}[0m
if is_qmark($b) == cult_question_end
        at $b echoat $b [1;35m�@�ϮL����                   ����[0m
else
        at $b echoat $b [1;32m�@�ϮL����                   ����[0m
endif
if is_qmark($b) == cult_rock_end
        at $b echoat $b [1;35m�j�a����                     ����[0m
else
        at $b echoat $b [1;32m�j�a����                     ����[0m
endif
at $b echoat $b .             [1;37m{�a���}[0m
if is_qmark($b) & gnome_quest 100002
        at $b echoat $b [1;35m�ڪ����дo                   ����[0m
else
        at $b echoat $b [1;32m�ڪ����дo                   ����[0m
endif
if is_qmark($b) & gnome_quest 100004
        at $b echoat $b [1;35m�j�c���ɪ�                   ����[0m
else
        at $b echoat $b [1;32m�j�c���ɪ�                   ����[0m
endif
at $b echoat $b .             [1;37m{�q�íx����}[0m
if is_qmark($b) > rebel_questmark 100004
        at $b echoat $b [1;35m�٤F���Ӫ����骺�å�I       ����[0m
else
        at $b echoat $b [1;32m�٤F���Ӫ����骺�å�I       ����[0m
endif
if is_qmark($b) > rebel_questmark 100017
        at $b echoat $b [1;35m�N�a����                     ����[0m
else
        at $b echoat $b [1;32m�N�a����                     ����[0m
endif
if is_qmark($b) > rebel_questmark 100021
        at $b echoat $b [1;35m�N�a�����E�䤧�G             ����[0m
else
        at $b echoat $b [1;32m�N�a�����E�䤧�G             ����[0m
endif
if is_qmark($b) > rebel_questmark 100029
        at $b echoat $b [1;35m�m���B�m³�B�m......         ����[0m
else
        at $b echoat $b [1;32m�m���B�m³�B�m......         ����[0m
endif
if is_qmark($b) > rebel_questmark 100034
        at $b echoat $b [1;35m�U�N�a�A����                 ����[0m
else
        at $b echoat $b [1;32m�U�N�a�A����                 ����[0m
endif
if is_qmark($b) > rebel_questmark 100038
        at $b echoat $b [1;35m���u�]ı������               ����[0m
else
        at $b echoat $b [1;32m���u�]ı������               ����[0m
endif
if is_qmark($b) > rebel_questmark 100042
        at $b echoat $b [1;35m�}�a�ɨ�                     ����[0m
else
        at $b echoat $b [1;32m�}�a�ɨ�                     ����[0m
endif
if is_qmark($b) > rebel_questmark 100048
        at $b echoat $b [1;35m�a�L���٬O�a���Y�H           ����[0m
else
        at $b echoat $b [1;32m�a�L���٬O�a���Y�H           ����[0m
endif
if is_qmark($b) > rebel_questmark 100055
        at $b echoat $b [1;35m�̫᪺���                   ����[0m
else
        at $b echoat $b [1;32m�̫᪺���                   ����[0m
endif
if is_qmark($b) > rebel_questmark 100059
        at $b echoat $b [1;35m�Q�f�Ԫ�����                 ����[0m
else
        at $b echoat $b [1;32m�Q�f�Ԫ�����                 ����[0m
endif
if is_qmark($b) == rebel_questmark 100066
        at $b echoat $b [1;30m(��)[0m[1;35m�{�ת����F�P�·t����     ����[0m
endif
def_rembch null-tar
~
>room_defun q03~
at $b echoat $b .             [1;36m[�v�d�Ԩ�����ϰ� I][0m
at $b echoat $b .             [1;37m{�ȳD�Z�h��}[0m
if class($b) == 11
or class($b) == 12
or class($b) == 13
        at $b echoat $b [1;30m(��)[37m�Ԥh�i������:[0m
if is_qmark($b) & warrior_quest 100001
or is_qmark($b) & warrior_quest 100002
or is_qmark($b) & warrior_quest 100004
        at $b echoat $b [1;35m--�Ԥh�t�̲׸շ�             ����[0m
else
        at $b echoat $b [1;32m--�Ԥh�t�̲׸շ�             ����[0m
endif
endif
if is_qmark($b) & warrior_quest 100008
        at $b echoat $b [1;35m���                         ����[0m
else
        at $b echoat $b [1;32m���                         ����[0m
endif
at $b echoat $b .             [1;37m{���´˪L}[0m
if is_qmark($b) == western_hermit 300005
        at $b echoat $b [1;35m�˪L���̪�����               ����[0m
else
        at $b echoat $b [1;32m�˪L���̪�����               ����[0m
endif
at $b echoat $b .             [1;37m{��w�տ��}[0m
if is_qmark($b) >= ianua_supplies 100004
or is_qmark($b) & ianua_quest 100001
        at $b echoat $b [1;35m����B�e I                   ����[0m
else
        at $b echoat $b [1;32m����B�e I                   ����[0m
endif
if is_qmark($b) >= ianua_supplies 100010
or is_qmark($b) & ianua_quest 100002
        at $b echoat $b [1;35m�T�_����                     ����[0m
else
        at $b echoat $b [1;32m�T�_����                     ����[0m
endif
if is_qmark($b) == ianua_teleporter
        at $b echoat $b [1;35m�ǰe�̤���                   ����[0m
else
        at $b echoat $b [1;32m�ǰe�̤���                   ����[0m
endif
at $b echoat $b .             [1;37m{������}[0m
if is_qmark($b) == greenwoods_war_over
        at $b echoat $b [1;35m�d���@�`�S                   ����[0m
else
        at $b echoat $b [1;32m�d���@�`�S                   ����[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100001
        at $b echoat $b [1;35m���^���Ĥ@�q               ����[0m
else
        at $b echoat $b [1;32m���^���Ĥ@�q               ����[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100002
        at $b echoat $b [1;35m���^���ĤG�q               ����[0m
else
        at $b echoat $b [1;32m���^���ĤG�q               ����[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100003
        at $b echoat $b [1;35m���^���ĤT�q               ����[0m
else
        at $b echoat $b [1;32m���^���ĤT�q               ����[0m
endif
if is_qmark($b) == greenwoods_santa_brother
        at $b echoat $b [1;35m�t��¥��d�S�̷|             ����[0m
else
        at $b echoat $b [1;32m�t��¥��d�S�̷|             ����[0m
endif
if is_qmark($b) == greenwoods_frowned_quest 100005
        at $b echoat $b [1;35m�C�ߪ��R��                   ����[0m
else
        at $b echoat $b [1;32m�C�ߪ��R��                   ����[0m
endif
at $b echoat $b .             [1;37m{�j�����}[0m
if is_qmark($b) == arachnos_quest 100007
        at $b echoat $b [1;35m���U�ͭ�                     ����[0m
else
        at $b echoat $b [1;32m���U�ͭ�                     ����[0m
endif
at $b echoat $b .             [1;37m{�پ���}[0m
if is_qmark($b) & shire_quest 100002
        at $b echoat $b [1;35m�����������J(����)           ����[0m
else
        at $b echoat $b [1;32m�����������J(����)           ����[0m
endif
if is_qmark($b) & shire_quest 100004
        at $b echoat $b [1;35m�����������J(�U�J)           ����[0m
else
        at $b echoat $b [1;32m�����������J(�U�J)           ����[0m
endif
if is_qmark($b) & shire_quest 100001
        at $b echoat $b [1;35m�g�s�@��                     ����[0m
else
        at $b echoat $b [1;32m�g�s�@��                     ����[0m
endif
def_rembch null-tar
~
>room_defun q04~
at $b echoat $b .             [1;36m[�v�d�Ԩ�����ϰ� II][0m
at $b echoat $b .             [1;37m{�]�k����}[0m
if is_qmark($b) & hitower_questmark 100001
        at $b echoat $b [1;35m���������M                   ����[0m
else
        at $b echoat $b [1;32m���������M                   ����[0m
endif
if is_qmark($b) & hitower_questmark 100002
        at $b echoat $b [1;35m�T�Ҳͺq                     ����[0m
else
        at $b echoat $b [1;32m�T�Ҳͺq                     ����[0m
endif
if race($b) == Drow
if is_qmark($b) & hitower_questmark 100004
        at $b echoat $b [1;35m���~�P���������l             ����[0m
else
        at $b echoat $b [1;32m���~�P���������l             ����[0m
endif
endif
if is_qmark($b) & hitower_questmark 100008
        at $b echoat $b [1;35m�R���P���q                   ����[0m
else
        at $b echoat $b [1;32m�R���P���q                   ����[0m
endif
at $b echoat $b .             [1;37m{�խ߲`�W}[0m
if is_qmark($b) & murky_quest 100001
        at $b echoat $b [1;35m��m���C�l                   ����[0m
else
        at $b echoat $b [1;32m��m���C�l                   ����[0m
endif
if is_qmark($b) & murky_quest 100002
        at $b echoat $b [1;35m���˪��p�k��                 ����[0m
else
        at $b echoat $b [1;32m���˪��p�k��                 ����[0m
endif
at $b echoat $b .             [1;37m{�T�_����}[0m
if is_qmark($b) == taboo_darkside 100001
        at $b echoat $b [1;35m(��)�Ԥk��-�a���Ȥ���        ����[0m
else
if race($b) != Vampire
if is_qmark($b) == taboo_quest 100011
        at $b echoat $b [1;35m�ٵ����D�u��                 ����[0m
else
        at $b echoat $b [1;32m�ٵ����D�u��                 ����[0m
endif
endif
endif
if is_qmark($b) == taboo_scroll 100002
        at $b echoat $b [1;35m�a�Jù�^�a                   ����[0m
else
        at $b echoat $b [1;32m�a�Jù�^�a                   ����[0m
endif
if is_qmark($b) == taboo_fapei_death
        at $b echoat $b [1;35m������ǻ�                   ����[0m
else
        at $b echoat $b [1;32m������ǻ�                   ����[0m
endif
at $b echoat $b .             [1;37m{����}[0m
if is_qmark($b) == ocean_captain_quest 300003
or is_qmark($b) & mistocean_quest 100001
        at $b echoat $b [1;35m�ɪŸt�ԤT����(�@)           ����[0m
else
        at $b echoat $b [1;32m�ɪŸt�ԤT����(�@)           ����[0m
endif
if is_qmark($b) == ocean_cleo_help 100007
or is_qmark($b) == ocean_cleo_help 100009
or is_qmark($b) & mistocean_quest 100002
or is_qmark($b) & mistocean_quest 100004
        at $b echoat $b [1;35m�J���ڪ��e�U                 ����[0m
else
        at $b echoat $b [1;32m�J���ڪ��e�U                 ����[0m
endif
if is_qmark($b) == ocean_alc_stone 300003
or is_qmark($b) & mistocean_quest 100008
        at $b echoat $b [1;35m��̤���                     ����[0m
else
        at $b echoat $b [1;32m��̤���                     ����[0m
endif
at $b echoat $b .             [1;37m{���v}[0m
if is_qmark($b) == ocean_potion_over 100001
or is_qmark($b) & ocean_quest 300001
        at $b echoat $b [1;35m�s�y�Ĥ�                     ����[0m
else
        at $b echoat $b [1;32m�s�y�Ĥ�                     ����[0m
endif
if is_qmark($b) == ocean_quest 200008
or is_qmark($b) & ocean_quest 300002
        at $b echoat $b [1;35m��������                     ����[0m
else
        at $b echoat $b [1;32m��������                     ����[0m
endif
at $b echoat $b .             [1;37m{�֪L�x}[0m
if is_qmark($b) == shaolin_quest 200003
        at $b echoat $b [1;35m�k���c��                     ����[0m
else
        at $b echoat $b [1;32m�k���c��                     ����[0m
endif
def_rembch null-tar
~
>room_defun q05~
at $b echoat $b .             [1;36m[�v�d�Ԩ��F��ϰ�Τj�F�z�� I][0m
at $b echoat $b .             [1;37m{���t��L}[0m
if is_qmark($b) & grove_quest 100001
        at $b echoat $b [1;35m���Y�Ӧ���                   ����[0m
else
        at $b echoat $b [1;32m���Y�Ӧ���                   ����[0m
endif
at $b echoat $b .             [1;37m{���F����}[0m
if is_qmark($b) == elftown_quest_over 100001
        at $b echoat $b [1;35m���F�M�Ԧa�F                 ����[0m
else
        at $b echoat $b [1;32m���F�M�Ԧa�F                 ����[0m
endif
if is_qmark($b) == war_reward 300100
        at $b echoat $b [1;35m���F�^��                     ����[0m
else
        at $b echoat $b [1;32m���F�^��                     ����[0m
endif
at $b echoat $b .             [1;37m{�a�F�}�]}[0m
if is_qmark($b) & kobold_cave_hero 300512
        at $b echoat $b [1;35m�a�F�^��                     ����[0m
else
        at $b echoat $b [1;32m�a�F�^��                     ����[0m
endif
at $b echoat $b .             [1;37m{�ɥﴵ�촶}[0m
if is_qmark($b) & hobbit_all_quests 100016
        at $b echoat $b [1;35m�s��j�v                     ����[0m
else
        at $b echoat $b [1;32m�s��j�v                     ����[0m
endif
if is_qmark($b) & hobbit_all_quests 100008
        at $b echoat $b [1;35m���z�k�����                 ����[0m
else
        at $b echoat $b [1;32m���z�k�����                 ����[0m
endif
if is_qmark($b) & hobbit_all_quests 100001
        at $b echoat $b [1;35m�j���Ȧ�n�� I               ����[0m
else
        at $b echoat $b [1;32m�j���Ȧ�n�� I               ����[0m
endif
if is_qmark($b) & hobbit_all_quests 100002
        at $b echoat $b [1;35m�j���Ȧ�n�� II              ����[0m
else
        at $b echoat $b [1;32m�j���Ȧ�n�� II              ����[0m
endif
if is_qmark($b) & hobbit_all_quests 100004
        at $b echoat $b [1;35m�]��                         ����[0m
else
        at $b echoat $b [1;32m�]��                         ����[0m
endif
at $b echoat $b .             [1;37m{������}[0m
if is_qmark($b) == thalos_practice_quest 100001
        at $b echoat $b [1;35m�j�]�ɪ��W�h                 ����[0m
else
        at $b echoat $b [1;32m�j�]�ɪ��W�h                 ����[0m
endif
if is_qmark($b) == thalos_mayor_quest 100005
        at $b echoat $b [1;35m�u�ö������b��               ����[0m
else
        at $b echoat $b [1;32m�u�ö������b��               ����[0m
endif
if is_qmark($b) & thalos_darkforce_quest 132768
or is_qmark($b) & thalos_darkforce_quest 165536
        at $b echoat $b [1;35m�����·t�դO                 ����[0m
else
        at $b echoat $b [1;32m�����·t�դO                 ����[0m
endif
at $b echoat $b .             [1;37m{�׮ڤ���}[0m
if is_qmark($b) == juargan_quest 100012
        at $b echoat $b [1;35m���^�v��                     ����[0m
else
        at $b echoat $b [1;32m���^�v��                     ����[0m
endif
at $b echoat $b .             [1;37m{�H����}[0m
if is_qmark($b) == wyvern_tower 100011
        at $b echoat $b [1;35m�M�����s��                   ����[0m
else
        at $b echoat $b [1;32m�M�����s��                   ����[0m
endif
if is_qmark($b) == wyvern_tower 200016
        at $b echoat $b [1;35m�s�h�X�S�`�N                 ����[0m
else
        at $b echoat $b [1;32m�s�h�X�S�`�N                 ����[0m
endif
if is_qmark($b) == wyvern_tower 300031
        at $b echoat $b [1;35m�F���쵲                     ����[0m
else
        at $b echoat $b [1;32m�F���쵲                     ����[0m
endif
at $b echoat $b .             [1;37m{�G�H����}[0m
if is_qmark($b) == cata_quest2_explorer 100005
        at $b echoat $b [1;35m��s������s                 ����[0m
else
        at $b echoat $b [1;32m��s������s                 ����[0m
endif
if is_qmark($b) == cata_quest_templar 100006
        at $b echoat $b [1;35m�t�����D��                   ����[0m
else
        at $b echoat $b [1;32m�t�����D��                   ����[0m
endif
at $b echoat $b .             [1;37m{�H����}[0m
if is_qmark($b) == muppet_grover_save 100006
        at $b echoat $b [1;35m�@�ϤH����                   ����[0m
else
        at $b echoat $b [1;32m�@�ϤH����                   ����[0m
endif
at $b echoat $b .             [1;37m{�ɫB�a��}[0m
if is_qmark($b) & cemetery_mark2 200002
        at $b echoat $b [1;35m�ŰȪȯ�                     ����[0m
else
        at $b echoat $b [1;32m�ŰȪȯ�                     ����[0m
endif
if is_qmark($b) & cemetery_mark2 200004
        at $b echoat $b [1;35m���̫��                     ����[0m
else
        at $b echoat $b [1;32m���̫��                     ����[0m
endif
def_rembch null-tar
~
>room_defun q06~
at $b echoat $b .             [1;36m[�v�d�Ԩ��F��ϰ�Τj�F�z�� II][0m
at $b echoat $b .             [1;37m{�ë���}[0m
if class($b) == 0
or class($b) == 1
or class($b) == 5
or class($b) == 6
or class($b) == 7
or class($b) == 8
or class($b) == 15
or class($b) == 16
or class($b) == 14
        at $b echoat $b [1;30m(��)[37m�k�v�򥻥���:[0m
if is_qmark($b) & eternity_quest 100001
        at $b echoat $b [1;35m--�]�k����˩w               ����[0m
else
        at $b echoat $b [1;32m--�]�k����˩w               ����[0m
endif
endif
if is_qmark($b) & eternity_quest 100002
        at $b echoat $b [1;35m���˪�����                   ����[0m
else
        at $b echoat $b [1;32m���˪�����                   ����[0m
endif
if is_qmark($b) & eternity_quest 100004
        at $b echoat $b [1;35m�x�����@�~                   ����[0m
else
        at $b echoat $b [1;32m�x�����@�~                   ����[0m
endif
if is_qmark($b) & eternity_quest 100008
        at $b echoat $b [1;35m�C�l�u                       ����[0m
else
        at $b echoat $b [1;32m�C�l�u                       ����[0m
endif
if is_qmark($b) & eternity_quest 100016
        at $b echoat $b [1;35m�g�H�̪�����                 ����[0m
else
        at $b echoat $b [1;32m�g�H�̪�����                 ����[0m
endif
if is_qmark($b) & eternity_quest 100032
        at $b echoat $b [1;35m���~�l��}                   ����[0m
else
        at $b echoat $b [1;32m���~�l��}                   ����[0m
endif
if class($b) == 0
or class($b) == 5
or class($b) == 6
or class($b) == 15
        at $b echoat $b [1;30m(��)[37m��´����:[0m
if is_qmark($b) >= wfs_clan_member 100001
        at $b echoat $b [1;35m--�������{�@                 ����[0m
else
        at $b echoat $b [1;32m--�������{�@                 ����[0m
endif
endif
if is_qmark($b) & eternity_quest 100064
        at $b echoat $b [1;35m�s�y�]�O�k��                 ����[0m
else
        at $b echoat $b [1;32m�s�y�]�O�k��                 ����[0m
endif
if is_qmark($b) & eternity_quest 100128
        at $b echoat $b [1;35m�M��                         ����[0m
else
        at $b echoat $b [1;32m�M��                         ����[0m
endif
if is_qmark($b) & eternity_quest 100256
        at $b echoat $b [1;35m�p���F���@��                 ����[0m
else
        at $b echoat $b [1;32m�p���F���@��                 ����[0m
endif
at $b echoat $b .             [1;37m{�Ҧ������S}[0m
if is_qmark($b) != mahntor_ogre_quest2 100017
if is_qmark($b) == mahntor_ogre_quest1 200004
or is_qmark($b) == mahntor_ogre_quest1 100004
        at $b echoat $b [1;35m�������ߨ�                   ����[0m
else
        at $b echoat $b [1;32m�������ߨ�                   ����[0m
endif
endif
if is_qmark($b) != mahntor_ogre_quest2 100017
if is_qmark($b) == mahntor_shaman_nut 100004
        at $b echoat $b [1;35m�Ѧ~���b������               ����[0m
else
        at $b echoat $b [1;32m�Ѧ~���b������               ����[0m
endif
endif
if is_qmark($b) != mahntor_ogre_quest1 200004
if is_qmark($b) != mahntor_ogre_quest1 100004
if is_qmark($b) == mahntor_ogre_quest2 100017
        at $b echoat $b [1;35m���S���M��                   ����[0m
else
        at $b echoat $b [1;32m���S���M��                   ����[0m
endif
endif
endif
at $b echoat $b .             [1;37m{�Z��˪L}[0m
if is_qmark($b) == forest_quest
        at $b echoat $b [1;35m�ǲߤg�H�y��                 ����[0m
else
        at $b echoat $b [1;32m�ǲߤg�H�y��                 ����[0m
endif
if is_qmark($b) == mist_clean_quest 100020
        at $b echoat $b [1;35m�ͯf���g�H                   ����[0m
else
        at $b echoat $b [1;32m�ͯf���g�H                   ����[0m
endif
if is_qmark($b) & forest_quest 300128
        at $b echoat $b [1;35m�������U�I                   ����[0m
else
        at $b echoat $b [1;32m�������U�I                   ����[0m
endif
if is_qmark($b) >= forest_quest 100004
        at $b echoat $b [1;35m���K���]                     ����[0m
else
        at $b echoat $b [1;32m���K���]                     ����[0m
endif
if is_qmark($b) == forest_quest 100007
or is_qmark($b) & forest_quest 101024
        at $b echoat $b [1;35m���s�ɤڬ��q                 ����[0m
else
        at $b echoat $b [1;32m���s�ɤڬ��q                 ����[0m
endif
if class($b) == 14
or class($b) == 18
or class($b) == 19
        at $b echoat $b [1;30m(��)[37m�Z�L�շҥ���:[0m
if is_qmark($b) == forest_barbarian_exam 100002
        at $b echoat $b [1;35m--���Z�H�շ�                 ����[0m
else
        at $b echoat $b [1;32m--���Z�H�շ�                 ����[0m
endif
endif
at $b echoat $b .             [1;37m{���櫰}[0m
if is_qmark($b) == mega1_arena_owner 300001
        at $b echoat $b [1;35m�v�޳�����                   ����[0m
else
        at $b echoat $b [1;32m�v�޳�����                   ����[0m
endif
if is_qmark($b) == mega1_alibaba_hunting 100001
        at $b echoat $b [1;35m�y�������ڤ�                 ����[0m
else
        at $b echoat $b [1;32m�y�������ڤ�                 ����[0m
endif
if is_qmark($b) == mega1_mayor_vsjudge 100004
        at $b echoat $b [1;35m���h�����                   ����[0m
else
        at $b echoat $b [1;32m���h�����                   ����[0m
endif
at $b echoat $b .             [1;37m{�㴵�����ħL����a}[0m
if is_qmark($b) & sandrock_questmark 100001
        at $b echoat $b [1;35m���ɻy��                     ����[0m
else
        at $b echoat $b [1;32m���ɻy��                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100002
        at $b echoat $b [1;35m���ݦ���                     ����[0m
else
        at $b echoat $b [1;32m���ݦ���                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100004
        at $b echoat $b [1;35m�L��ű�y                     ����[0m
else
        at $b echoat $b [1;32m�L��ű�y                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100008
        at $b echoat $b [1;35m�]�k�P��                     ����[0m
else
        at $b echoat $b [1;32m�]�k�P��                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100016
        at $b echoat $b [1;35m�}�ƽհt                     ����[0m
else
        at $b echoat $b [1;32m�}�ƽհt                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100032
        at $b echoat $b [1;35m�\���լd                     ����[0m
else
        at $b echoat $b [1;32m�\���լd                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100064
        at $b echoat $b [1;35m�w���ШD                     ����[0m
else
        at $b echoat $b [1;32m�w���ШD                     ����[0m
endif
if is_qmark($b) & sandrock_questmark 100128
        at $b echoat $b [1;35m�M��u��                     ����[0m
else
        at $b echoat $b [1;32m�M��u��                     ����[0m
endif
def_rembch null-tar
~
>room_defun q07~
at $b echoat $b .             [1;36m[����@�ɥH�Υ~��a�� I][0m
at $b echoat $b .             [1;37m{�Х@��}[0m
if is_qmark($b) >= ultima_destroy_evil 100008
        at $b echoat $b [1;35m�K�j���w������               ����[0m
else
        at $b echoat $b [1;32m�K�j���w������               ����[0m
endif
if is_qmark($b) == ultima_destroy_evil 100011
        at $b echoat $b [1;35m�Х@���t��                   ����[0m
else
        at $b echoat $b [1;32m�Х@���t��                   ����[0m
endif
at $b echoat $b .             [1;37m{���L�����s}[0m
if is_qmark($b) == olympus_weapon_quest 200009
or is_qmark($b) == olympus_weapon_quest 100009
        at $b echoat $b [1;35m�Ѭɤ���                     ����[0m
else
        at $b echoat $b [1;32m�Ѭɤ���                     ����[0m
endif
if is_qmark($b) == olympus_jail_quest 100021
        at $b echoat $b [1;35m����}��                     ����[0m
else
        at $b echoat $b [1;32m����}��                     ����[0m
endif
at $b echoat $b .             [1;37m{�t��}[0m
if is_qmark($b) == saintarea_quest_main 100040
        at $b echoat $b [1;35m�����Q�G�c����               ����[0m
else
        at $b echoat $b [1;32m�����Q�G�c����               ����[0m
endif
if is_qmark($b) == saintarea_quest_geminimask 100002
        at $b echoat $b [1;35m���y�t��                     ����[0m
else
        at $b echoat $b [1;32m���y�t��                     ����[0m
endif
if is_qmark($b) == saintarea_perseus 100019
        at $b echoat $b [1;35m���y����                     ����[0m
else
        at $b echoat $b [1;32m���y����                     ����[0m
endif
at $b echoat $b .             [1;37m{�P��}[0m
if is_qmark($b) & astral_quest 100001
        at $b echoat $b [1;35m�@�ϬP��                     ����[0m
else
        at $b echoat $b [1;32m�@�ϬP��                     ����[0m
endif
if is_qmark($b) & astral_quest 100002
        at $b echoat $b [1;35m�@�Ϭ��s                     ����[0m
else
        at $b echoat $b [1;32m�@�Ϭ��s                     ����[0m
endif
if is_qmark($b) & astral_quest 100004
        at $b echoat $b [1;35m���y�@��                     ����[0m
else
        at $b echoat $b [1;32m���y�@��                     ����[0m
endif
at $b echoat $b .             [1;37m{�P������}[0m
if is_qmark($b) & thoran_quest 100001
        at $b echoat $b [1;35m���ϩ`��                     ����[0m
else
        at $b echoat $b [1;32m���ϩ`��                     ����[0m
endif
if is_qmark($b) & thoran_quest 100002
        at $b echoat $b [1;35m�D���q����                   ����[0m
else
        at $b echoat $b [1;32m�D���q����                   ����[0m
endif
if is_qmark($b) & thoran_quest 100004
        at $b echoat $b [1;35m�s���Z�h���q��               ����[0m
else
        at $b echoat $b [1;32m�s���Z�h���q��               ����[0m
endif
if is_qmark($b) & thoran_quest 100008
        at $b echoat $b [1;35m�~�˦u�ê��@��               ����[0m
else
        at $b echoat $b [1;32m�~�˦u�ê��@��               ����[0m
endif
if is_qmark($b) & thoran_quest 100016
        at $b echoat $b [1;35m�Q�U�A�G�����D               ����[0m
else
        at $b echoat $b [1;32m�Q�U�A�G�����D               ����[0m
endif
def_rembch null-tar
~
>room_defun q08~
at $b echoat $b .             [1;36m[����@�ɥH�Υ~��a�� II][0m
at $b echoat $b .             [1;37m{�n���j��}[0m
if is_qmark($b) & southend_quest 100002
        at $b echoat $b [1;35m�������g�Y                   ����[0m
else
        at $b echoat $b [1;32m�������g�Y                   ����[0m
endif
if is_qmark($b) & southend_quest 100004
        at $b echoat $b [1;35m�֯S���дo                   ����[0m
else
        at $b echoat $b [1;32m�֯S���дo                   ����[0m
endif
if is_qmark($b) & southend_quest 100008
        at $b echoat $b [1;35m�������M��                   ����[0m
else
        at $b echoat $b [1;32m�������M��                   ����[0m
endif
if is_qmark($b) & southend_quest 100016
        at $b echoat $b [1;35m�������g�k                   ����[0m
else
        at $b echoat $b [1;32m�������g�k                   ����[0m
endif
if is_qmark($b) & southend_quest 100032
        at $b echoat $b [1;35m���������{                   ����[0m
else
        at $b echoat $b [1;32m���������{                   ����[0m
endif
if is_qmark($b) & southend_quest 100064
        at $b echoat $b [1;35m�o�H�p�T��                   ����[0m
else
        at $b echoat $b [1;32m�o�H�p�T��                   ����[0m
endif
def_rembch null-tar
~
>room_defun q09~
at $b echoat $b .             [1;36m[�g�����q�D��][0m
at $b echoat $b .             [1;37m{��������}[0m
if is_qmark($b) & mist_quest 100001
        at $b echoat $b [1;35m�s�y�Ĥ�                     ����[0m
else
        at $b echoat $b [1;32m�s�y�Ĥ�                     ����[0m
endif
if is_qmark($b) & mist_quest 100002
        at $b echoat $b [1;35m���`������                   ����[0m
else
        at $b echoat $b [1;32m���`������                   ����[0m
endif
if is_qmark($b) & mist_quest 100004
        at $b echoat $b [1;35m��������U���ƥ�             ����[0m
else
        at $b echoat $b [1;32m��������U���ƥ�             ����[0m
endif
if is_qmark($b) == mist_protector 100001
        at $b echoat $b [1;35m�������s                     ����[0m
else
        at $b echoat $b [1;32m�������s                     ����[0m
endif
if is_qmark($b) & mist_council_over 100002
        at $b echoat $b [1;35m��ĳ�|����--²���e�f I       ����[0m
else
        at $b echoat $b [1;32m��ĳ�|����--²���e�f I       ����[0m
endif
if is_qmark($b) & mist_council_over 100004
        at $b echoat $b [1;35m��ĳ�|����--²���e�f II      ����[0m
else
        at $b echoat $b [1;32m��ĳ�|����--²���e�f II      ����[0m
endif
if is_qmark($b) & mist_council_over 100008
        at $b echoat $b [1;35m��ĳ�|����--�t�� I           ����[0m
else
        at $b echoat $b [1;32m��ĳ�|����--�t�� I           ����[0m
endif
at $b echoat $b .             [1;37m{������}[0m
if is_qmark($b) >= crystal_reward 100001
        at $b echoat $b [1;35m�@�ϻ��s                     ����[0m
else
        at $b echoat $b [1;32m�@�ϻ��s                     ����[0m
endif
at $b echoat $b .             [1;37m{�����L}[0m
if is_qmark($b) == maple_ack 100004
        at $b echoat $b [1;35m�۵M���w��                   ����[0m
else
        at $b echoat $b [1;32m�۵M���w��                   ����[0m
endif
if is_qmark($b) == maple_camus_attack 100003
        at $b echoat $b [1;35m�խ߲`�W���Ϯ{               ����[0m
else
        at $b echoat $b [1;32m�խ߲`�W���Ϯ{               ����[0m
endif
if class($b) == 12
        at $b echoat $b [1;30m(��)[37m�C�h�i������:[0m
if is_qmark($b) == maple_swd_trail 100006
        at $b echoat $b [1;35m--�C�h�i���շ�               ����[0m
else
        at $b echoat $b [1;32m--�C�h�i���շ�               ����[0m
endif
endif
if class($b) == 2
or class($b) == 9
or class($b) == 10
or class($b) == 17
at $b echoat $b .             [1;37m{�s��n��}[0m
if is_qmark($b) >= bandit_thief_exam 100005
        at $b echoat $b [1;35m�s��u�h                     ����[0m
else
        at $b echoat $b [1;32m�s��u�h                     ����[0m
endif
endif
if class($b) == 10
        at $b echoat $b [1;30m(��)[37m�Ԫ̶i������:[0m
if is_qmark($b) == bandit_thief_exam 100007
        at $b echoat $b [1;35m--�Ԫ̸շ�                   ����[0m
else
        at $b echoat $b [1;32m--�Ԫ̸շ�                   ����[0m
endif
endif
def_rembch null-tar
~
>room_defun q10~
at $b echoat $b .             [1;36m[�g�����q�`�Ϥ���][0m
at $b echoat $b .             [1;37m{�`�ϫ���}[0m
if is_qmark($b) >= naytil_quest 100008
        at $b echoat $b [1;35m�Ϯ��]�����c�]               ����[0m
else
        at $b echoat $b [1;32m�Ϯ��]�����c�]               ����[0m
endif
if is_qmark($b) != fight_naytil 100008
if is_qmark($b) == fight_highland 100005
        at $b echoat $b [1;35m�M�԰��a                     ����[0m
else
        at $b echoat $b [1;32m�M�԰��a                     ����[0m
endif
endif
if is_qmark($b) == naytil_princess 100001
        at $b echoat $b [1;35m�M��p��                     ����[0m
else
        at $b echoat $b [1;32m�M��p��                     ����[0m
endif
if is_qmark($b) == naytil_rebels 200001
or is_qmark($b) == naytil_rebels 200002
        at $b echoat $b [1;35m�`��/�ϫq�x�j�Խg            ����[0m
else
        at $b echoat $b [1;32m�`��/�ϫq�x�j�Խg            ����[0m
endif
at $b echoat $b .             [1;37m{���a}[0m
if is_qmark($b) != fight_highland 100005
if is_qmark($b) == fight_naytil 100008
        at $b echoat $b [1;35m�M�ԩ`��                     ����[0m
else
        at $b echoat $b [1;32m�M�ԩ`��                     ����[0m
endif
endif
if is_qmark($b) == for_rose 100005
        at $b echoat $b [1;35m��������                     ����[0m
else
        at $b echoat $b [1;32m��������                     ����[0m
endif
at $b echoat $b .             [1;37m{���p����}[0m
if is_qmark($b) & mist_quest 100008
        at $b echoat $b [1;35m�_�I�̪��@��                 ����[0m
else
        at $b echoat $b [1;32m�_�I�̪��@��                 ����[0m
endif
at $b echoat $b .             [1;37m{����}[0m
if is_qmark($b) & gtown_quest_list 100001
        at $b echoat $b [1;35m���`���J�I                   ����[0m
else
        at $b echoat $b [1;32m���`���J�I                   ����[0m
endif
if race($b) == vampire
if is_qmark($b) == gtown_vampire_finish
        at $b echoat $b [1;35m�l�尭����                   ����[0m
else
        at $b echoat $b [1;32m�l�尭����                   ����[0m
endif
endif
if is_qmark($b) & gtown_quest_list 100002
        at $b echoat $b [1;35m�p�c�T�S�̤���               ����[0m
else
        at $b echoat $b [1;32m�p�c�T�S�̤���               ����[0m
endif
if is_qmark($b) & gtown_quest_list 100004
        at $b echoat $b [1;35m�ڤۤ���                     ����[0m
else
        at $b echoat $b [1;32m�ڤۤ���                     ����[0m
endif
if is_qmark($b) & gtown_quest_list 100008
        at $b echoat $b [1;35m�p���a���ϥ�                 ����[0m
else
        at $b echoat $b [1;32m�p���a���ϥ�                 ����[0m
endif
def_rembch null-tar
~
>room_defun q11~
at $b echoat $b .             [1;36m[�g�����q����L�ϰ� I][0m
at $b echoat $b .             [1;37m{���j�˪L}[0m
if is_qmark($b) & prehist_quest 100001
        at $b echoat $b [1;35m�@�ϳ��~                     ����[0m
else
        at $b echoat $b [1;32m�@�ϳ��~                     ����[0m
endif
if is_qmark($b) & prehist_quest 100002
        at $b echoat $b [1;35m�����B����                   ����[0m
else
        at $b echoat $b [1;32m�����B����                   ����[0m
endif
if is_qmark($b) & prehist_quest 100004
        at $b echoat $b [1;35m���p�l���o�t�M               ����[0m
else
        at $b echoat $b [1;32m���p�l���o�t�M               ����[0m
endif
if is_qmark($b) & prehist_quest 100008
        at $b echoat $b [1;35m�����J�ײz�Фl               ����[0m
else
        at $b echoat $b [1;32m�����J�ײz�Фl               ����[0m
endif
if is_qmark($b) & prehist_quest 100016
        at $b echoat $b [1;35m�p�k�Ī��_��                 ����[0m
else
        at $b echoat $b [1;32m�p�k�Ī��_��                 ����[0m
endif
if is_qmark($b) & prehist_quest 100032
        at $b echoat $b [1;35m���U���j�˪L���_�I�̲���     ����[0m
else
        at $b echoat $b [1;32m���U���j�˪L���_�I�̲���     ����[0m
endif
if is_qmark($b) & prehist_quest 100064
        at $b echoat $b [1;35m���o�W�����P�j�J�J           ����[0m
else
        at $b echoat $b [1;32m���o�W�����P�j�J�J           ����[0m
endif
if is_qmark($b) & prehist_quest 100128
        at $b echoat $b [1;35m�M���B�w���ͪ�               ����[0m
else
        at $b echoat $b [1;32m�M���B�w���ͪ�               ����[0m
endif
if is_qmark($b) & prehist_quest 100512
        at $b echoat $b [1;35m���Ѯv���дo                 ����[0m
else
        at $b echoat $b [1;32m���Ѯv���дo                 ����[0m
endif
at $b echoat $b .             [1;37m{���]�}�]}[0m
if class($b) == 7
or class($b) == 8
if is_qmark($b) & sealevil_quest 100001
        at $b echoat $b [1;35m�u�@�̴���                   ����[0m
else
        at $b echoat $b [1;32m�u�@�̴���                   ����[0m
endif
endif
if is_qmark($b) & sealevil_quest 100002
or is_qmark($b) > sealevil_Aleru_quest 100018
        at $b echoat $b [1;35m�a�ڪ��ϩR�Ĥ@�q             ����[0m
else
        at $b echoat $b [1;32m�a�ڪ��ϩR�Ĥ@�q             ����[0m
endif
if is_qmark($b) & sealevil_quest 100002
        at $b echoat $b [1;35m�a�ڪ��ϩR�ĤG�q             ����[0m
else
        at $b echoat $b [1;32m�a�ڪ��ϩR�ĤG�q             ����[0m
endif
if is_qmark($b) & sealevil_quest 100004
        at $b echoat $b [1;35m�n�f������                   ����[0m
else
        at $b echoat $b [1;32m�n�f������                   ����[0m
endif
if is_qmark($b) & sealevil_quest 100008
        at $b echoat $b [1;35m���F���p                     ����[0m
else
        at $b echoat $b [1;32m���F���p                     ����[0m
endif
if is_qmark($b) & sealevil_quest 100016
        at $b echoat $b [1;35m���b�X�@                     ����[0m
else
        at $b echoat $b [1;32m���b�X�@                     ����[0m
endif
at $b echoat $b .             [1;37m{���ث���}[0m
if is_qmark($b) & snow_questcheck 100001
or is_qmark($b) == snow_Porak01_quest 100011
        at $b echoat $b [1;35m���K����                     ����[0m
else
        at $b echoat $b [1;32m���K����                     ����[0m
endif
if is_qmark($b) & snow_questcheck 100002
or is_qmark($b) == snow_Porak02_quest 100010
        at $b echoat $b [1;35m����                         ����[0m
else
        at $b echoat $b [1;32m����                         ����[0m
endif
if is_qmark($b) & snow_questcheck 100004
or is_qmark($b) == snow_Porak03_quest 100015
        at $b echoat $b [1;35m�ĥ|����                     ����[0m
else
        at $b echoat $b [1;32m�ĥ|����                     ����[0m
endif
if is_qmark($b) & snow_questcheck 100008
or is_qmark($b) == sealevil_Tylas_quest 100015
        at $b echoat $b [1;35m���F���pII                   ����[0m
else
        at $b echoat $b [1;32m���F���pII                   ����[0m
endif
if is_qmark($b) & snow_questcheck 100016
or is_qmark($b) >= sealevil_Aleru_quest 100089
        at $b echoat $b [1;35m�a�ڪ��ϩR�ĤT�q             ����[0m
else
        at $b echoat $b [1;32m�a�ڪ��ϩR�ĤT�q             ����[0m
endif
def_rembch null-tar
~
>room_defun q12~
at $b echoat $b .             [1;36m[�g�����q����L�ϰ� II][0m
at $b echoat $b .             [1;37m{�R�����q}[0m
if is_qmark($b) & Ageemephy_questmark 200001
        at $b echoat $b [1;35m�����{�����١E����           ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100001
                at $b echoat $b [1;35m�����{������                 ����[0m
        else
                at $b echoat $b [1;32m�����{������                 ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200002
        at $b echoat $b [1;35m��¾�L�ҩx�E�O��             ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100002
                at $b echoat $b [1;35m��¾�L�ҩx                   ����[0m
        else
                at $b echoat $b [1;32m��¾�L�ҩx                   ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200004
        at $b echoat $b [1;35m�c�]���h�E�I�q               ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100004
                at $b echoat $b [1;35m�c�]���h                     ����[0m
        else
                at $b echoat $b [1;32m�c�]���h                     ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200008
        at $b echoat $b [1;35m�ªZ�̤��C�E��ū             ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100008
                at $b echoat $b [1;35m�ªZ�̤��C                   ����[0m
        else
                at $b echoat $b [1;32m�ªZ�̤��C                   ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200016
        at $b echoat $b [1;35m�t���Ҩ㪺�֤k�E�e��         ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100016
                at $b echoat $b [1;35m�t���Ҩ㪺�֤k               ����[0m
        else
                at $b echoat $b [1;32m�t���Ҩ㪺�֤k               ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200032
        at $b echoat $b [1;35m�]�k�}��s�E�d�@             ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100032
                at $b echoat $b [1;35m�]�k�}��s                   ����[0m
        else
                at $b echoat $b [1;32m�]�k�}��s                   ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200064
        at $b echoat $b [1;35m����C�ǡE�aģ               ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100064
                at $b echoat $b [1;35m����C��                     ����[0m
        else
                at $b echoat $b [1;32m����C��                     ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200128
        at $b echoat $b [1;35m�I�Φb�R��ɥ����E�ڭ�     ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100128
                at $b echoat $b [1;35m�I�Φb�R��ɥ���           ����[0m
        else
                at $b echoat $b [1;32m�I�Φb�R��ɥ���           ����[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200256
        at $b echoat $b [1;35m�l�M�e�H���}�B�E�y��         ����[0m
else
        if is_qmark($b) & Ageemephy_questmark 100256
                at $b echoat $b [1;35m�l�M�e�H���}�B               ����[0m
        else
                at $b echoat $b [1;32m�l�M�e�H���}�B               ����[0m
        endif
endif
at $b echoat $b .             [1;37m{�R�����q�P��}[0m
if is_qmark($b) & TideIsland_questmark 200001
        at $b echoat $b [1;35m�Q�ʦL���j����               ����[0m
else
        at $b echoat $b [1;32m�Q�ʦL���j����               ����[0m
endif
if class($b) == 11
or class($b) == 16
if is_qmark($b) & TideIsland_questmark 200002
        at $b echoat $b [1;35m�y���ǻ�                     ����[0m
else
        at $b echoat $b [1;32m�y���ǻ�                     ����[0m
endif
endif
if is_qmark($b) >= sealevil_Aleru_quest 100104
        at $b echoat $b [1;35m�a�ڪ��ϩR�ĥ|�q             ����[0m
else
        at $b echoat $b [1;32m�a�ڪ��ϩR�ĥ|�q             ����[0m
endif
at $b echoat $b .             [1;37m{�շt�K�L}[0m
if is_qmark($b) == mirkwood_quest 100006
        at $b echoat $b [1;35m�k��                         ����[0m
else
        at $b echoat $b [1;32m�k��                         ����[0m
endif
at $b echoat $b .             [1;37m{���԰ǫ�}[0m
if is_qmark($b) >= malathar_quest 100003
        at $b echoat $b [1;35m�p�c�������               ����[0m
else
        at $b echoat $b [1;32m�p�c�������               ����[0m
endif
if is_qmark($b) >= malathar_quest 100008
        at $b echoat $b [1;35m�]�O���m                     ����[0m
else
        at $b echoat $b [1;32m�]�O���m                     ����[0m
endif
if is_qmark($b) >= malathar_quest 100012
        at $b echoat $b [1;35m���ҲM����                   ����[0m
else
        at $b echoat $b [1;32m���ҲM����                   ����[0m
endif
if is_qmark($b) >= malathar_quest 100029
        at $b echoat $b [1;35mĳ�M                         ����[0m
else
        at $b echoat $b [1;32mĳ�M                         ����[0m
endif
if is_qmark($b) >= malathar_quest 100033
        at $b echoat $b [1;35m�l���ɨ�                     ����[0m
else
        at $b echoat $b [1;32m�l���ɨ�                     ����[0m
endif
if is_qmark($b) >= malathar_quest 100038
        at $b echoat $b [1;35m���@���O                     ����[0m
else
        at $b echoat $b [1;32m���@���O                     ����[0m
endif
if is_qmark($b) == malathar_quest 100043
        at $b echoat $b [1;35m�·t������                   ����[0m
else
        at $b echoat $b [1;32m�·t������                   ����[0m
endif
def_rembch null-tar
~
>room_defun q13~
at $b echoat $b .             [1;36m[�g�����q�g�����հϰ�][0m
at $b echoat $b .             [1;37m{�g�����ը��a}[0m
if is_qmark($b) >= myth_seamstress 100020
or is_qmark($b) & myth_greenleaf_end 100001
        at $b echoat $b [1;35m¾�H�믫                     ����[0m
else
        at $b echoat $b [1;32m¾�H�믫                     ����[0m
endif
if is_qmark($b) & myth_greenleaf_end 100002
        at $b echoat $b [1;35m�]�O�ɭ���}��s             ����[0m
else
        at $b echoat $b [1;32m�]�O�ɭ���}��s             ����[0m
endif
if is_qmark($b) & myth_greenleaf_end 100004
or is_qmark($b) & myth_greenleaf_end 200004
        at $b echoat $b [1;35m¼�Z����F���x               ����[0m
else
        at $b echoat $b [1;32m¼�Z����F���x               ����[0m
endif
if is_qmark($b) & myth_family_recipe 300001
        at $b echoat $b [1;35m�l�����H                     ����[0m
else
        at $b echoat $b [1;32m�l�����H                     ����[0m
endif
if is_qmark($b) & myth_family_recipe 300002
        at $b echoat $b [1;35m�����~�Ӥ���                 ����[0m
else
        at $b echoat $b [1;32m�����~�Ӥ���                 ����[0m
endif
if is_qmark($b) & myth_elven_end 100001
or is_qmark($b) & myth_elven_end 200001
        at $b echoat $b [1;35m�������g��                   ����[0m
else
        at $b echoat $b [1;32m�������g��                   ����[0m
endif
if is_qmark($b) & myth_templar_end 100001
or is_qmark($b) & myth_templar_end 200001
        at $b echoat $b [1;35m�йνլd��                   ����[0m
else
        at $b echoat $b [1;32m�йνլd��                   ����[0m
endif
if is_qmark($b) & myth_templar_end 100002
or is_qmark($b) & myth_templar_end 200002
        at $b echoat $b [1;35m���ܤ������M�h               ����[0m
else
        at $b echoat $b [1;32m���ܤ������M�h               ����[0m
endif
if is_qmark($b) & myth_templar_end 100004
or is_qmark($b) & myth_templar_end 200004
        at $b echoat $b [1;35m�йνլd��II                 ����[0m
else
        at $b echoat $b [1;32m�йνլd��II                 ����[0m
endif
if is_qmark($b) == myth_templar_enemy
or is_qmark($b) == templar_absolution
        if is_qmark($b) == templar_absolution 100005
                at $b echoat $b [1;35m�з|ū�o��                   ����[0m
        else
                at $b echoat $b [1;32m�з|ū�o��                   ����[0m
        endif
endif
at $b echoat $b .             [1;37m{�t�º��F��a}[0m
if is_qmark($b) == enclave_quest 100007
        at $b echoat $b [1;35m�M��H��                     ����[0m
else
        at $b echoat $b [1;32m�M��H��                     ����[0m
endif
def_rembch null-tar
~
>room_defun boss_list~
at $b echoat $b [1;37m�A�i�H�d�ݪ��ؼЪ��ϰ�:[0m
at $b echoat $b �Ĥ@�� [1;32m�v�d�Ԩ��ζg��ϰ� I           [37m(    boss01     )[0m
at $b echoat $b �ĤG�� [1;32m�v�d�Ԩ��ζg��ϰ� II          [37m(    boss02     )[0m
at $b echoat $b �ĤT�� �ŭ�
at $b echoat $b �ĥ|�� [1;32m�v�d�Ԩ�����ϰ� I             [37m(    boss04     )[0m
at $b echoat $b �Ĥ��� [1;32m�v�d�Ԩ�����ϰ� II            [37m(    boss05     )[0m
at $b echoat $b �Ĥ��� �ŭ�
at $b echoat $b �ĤC�� [1;32m�v�d�Ԩ��F��ϰ�Τj�F�z�� I   [37m(    boss07     )[0m
at $b echoat $b �ĤK�� [1;32m�v�d�Ԩ��F��ϰ�Τj�F�z�� II  [37m(    boss08     )[0m
at $b echoat $b �ĤE�� �ŭ�
at $b echoat $b �ĤQ�� [1;32m����@�ɥH�Υ~��a�� I         [37m(    boss10     )[0m
at $b echoat $b ��11�� [1;32m����@�ɥH�Υ~��a�� II        [37m(    boss11     )[0m
at $b echoat $b ��12�� �ŭ�
at $b echoat $b ��13�� [1;36m�g�����q�D��                   [37m(    boss13     )[0m
at $b echoat $b ��14�� �ŭ�
at $b echoat $b ��15�� �ŭ�
at $b echoat $b ��16�� [1;36m�g�����q�`�Ϥ���               [37m(    boss16     )[0m
at $b echoat $b ��17�� �ŭ�
at $b echoat $b ��18�� �ŭ�
at $b echoat $b ��19�� [1;36m�g�����q����L�ϰ�             [37m(    boss19     )[0m
at $b echoat $b ��20�� �ŭ�
at $b echoat $b ��21�� �ŭ�
at $b echoat $b ��22�� [1;33m���s���W�[�a�h�]��             [37m(    boss22     )[0m
def_rembch null-tar
~
>room_defun b01~
at $b echoat $b .		[1;36m[�v�d�Ԩ��ζg��ϰ� I][0m
if is_qmark($b) & adv_book_1_main 100001
	at $b echoat $b [1;35m���k��                  �v�d�Ԩ�       �w����[0m
else
	at $b echoat $b [1;32m���k��                  �v�d�Ԩ�       ������[0m
endif
if is_qmark($b) & adv_book_1_main 100002
	at $b echoat $b [1;35m�׵���                  �v�d�Ԩ�       �w����[0m
else
	at $b echoat $b [1;32m�׵���                  �v�d�Ԩ�       ������[0m
endif
if is_qmark($b) & adv_book_1_main 100004
	at $b echoat $b [1;35m�L�g��                  �v�d�Ԩ�       �w����[0m
else
	at $b echoat $b [1;32m�L�g��                  �v�d�Ԩ�       ������[0m
endif
if is_qmark($b) & adv_book_1_main 100008
	at $b echoat $b [1;35m�������S                �v�d�Ԩ�       �w����[0m
else
	at $b echoat $b [1;32m�������S                �v�d�Ԩ�       ������[0m
endif
def_rembch null-tar
~
>room_defun b02~
at $b echoat $b .		[1;36m[�v�d�Ԩ��ζg��ϰ� II][0m
if is_qmark($b) & adv_book_1_main 200001
	at $b echoat $b [1;35m�p�c���~��x�Ϊ�        �K�n���˪L     �w����[0m
else
	at $b echoat $b [1;32m�p�c���~��x�Ϊ�        �K�n���˪L     ������[0m
endif
if is_qmark($b) & adv_book_1_main 200002
	at $b echoat $b [1;35m�p�c���T�íx�Ϊ�        �K�n���˪L     �w����[0m
else
	at $b echoat $b [1;32m�p�c���T�íx�Ϊ�        �K�n���˪L     ������[0m
endif
if is_qmark($b) & adv_book_1_main 200004
	at $b echoat $b [1;35m�p�c���`�ѿѼڤڮ�      �K�n���˪L     �w����[0m
else
	at $b echoat $b [1;32m�p�c���`�ѿѼڤڮ�      �K�n���˪L     ������[0m
endif
if is_qmark($b) & adv_book_1_main 200256
	at $b echoat $b [1;35m�p�c������              �K�n���˪L     �w����[0m
else
	at $b echoat $b [1;32m�p�c������              �K�n���˪L     ������[0m
endif
if is_qmark($b) & adv_book_1_main 200512
	at $b echoat $b [1;35m�p�c���𫰺l            �K�n���˪L     �w����[0m
else
	at $b echoat $b [1;32m�p�c���𫰺l            �K�n���˪L     ������[0m
endif
if is_qmark($b) & adv_book_1_main 201024
	at $b echoat $b [1;35m�p�c����۾�            �K�n���˪L     �w����[0m
else
	at $b echoat $b [1;32m�p�c����۾�            �K�n���˪L     ������[0m
endif
def_rembch null-tar
~
>room_defun b03~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b04~
at $b echoat $b .		[1;36m[�v�d�Ԩ�����ϰ� I][0m
if is_qmark($b) & adv_book_1_main 100016
	at $b echoat $b [1;35m�����C��                �ȳD�Z�h��     �w����[0m
else
	at $b echoat $b [1;32m�����C��                �ȳD�Z�h��     ������[0m
endif
if is_qmark($b) & adv_book_1_main 100032
	at $b echoat $b [1;35m�ȳD�����D              �ȳD�Z�h��     �w����[0m
else
	at $b echoat $b [1;32m�ȳD�����D              �ȳD�Z�h��     ������[0m
endif
if is_qmark($b) & adv_book_1_main 100064
	at $b echoat $b [1;35m�C�Ⱥ��F                �ȳD�Z�h��     �w����[0m
else
	at $b echoat $b [1;32m�C�Ⱥ��F                �ȳD�Z�h��     ������[0m
endif
if is_qmark($b) & adv_book_1_main 200032
	at $b echoat $b [1;35m���]��ɪ̼ީԼީ�      �j�h�a         �w����[0m
else
	at $b echoat $b [1;32m���]��ɪ̼ީԼީ�      �j�h�a         ������[0m
endif
if is_qmark($b) & adv_book_1_main 100128
	at $b echoat $b [1;35m��s���J                ������       �w����[0m
else
	at $b echoat $b [1;32m��s���J                ������       ������[0m
endif
if is_qmark($b) & adv_book_1_main 100256
	at $b echoat $b [1;35m���ö�����            ������       �w����[0m
else
	at $b echoat $b [1;32m���ö�����            ������       ������[0m
endif
if is_qmark($b) & adv_book_1_main 100512
	at $b echoat $b [1;35m���L��                  ������       �w����[0m
else
	at $b echoat $b [1;32m���L��                  ������       ������[0m
endif
if is_qmark($b) & adv_book_1_main 101024
	at $b echoat $b [1;35m�d���                  ������       �w����[0m
else
	at $b echoat $b [1;32m�d���                  ������       ������[0m
endif
if is_qmark($b) & adv_book_1_main 102048
	at $b echoat $b [1;35m���ö��ƶ���          ������       �w����[0m
else
	at $b echoat $b [1;32m���ö��ƶ���          ������       ������[0m
endif
if is_qmark($b) & adv_book_1_main 104096
	at $b echoat $b [1;35m�h��j��                ������       �w����[0m
else
	at $b echoat $b [1;32m�h��j��                ������       ������[0m
endif
if is_qmark($b) & adv_book_1_main 108192
	at $b echoat $b [1;35m���Q                    ������       �w����[0m
else
	at $b echoat $b [1;32m���Q                    ������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 200001
	at $b echoat $b [1;35m�{�����S                �s��           �w����[0m
else
	at $b echoat $b [1;32m�{�����S                �s��           ������[0m
endif
if is_qmark($b) & adv_book_1_west 200002
	at $b echoat $b [1;35m�]�k�k��                �]�k����       �w����[0m
else
	at $b echoat $b [1;32m�]�k�k��                �]�k����       ������[0m
endif
def_rembch null-tar
~
>room_defun b05~
at $b echoat $b .		[1;36m[�v�d�Ԩ�����ϰ� II][0m
if is_qmark($b) & adv_book_1_west 100001
	at $b echoat $b [1;35mù�J����                ù�J����       �w����[0m
else
	at $b echoat $b [1;32mù�J����                ù�J����       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100002
	at $b echoat $b [1;35mù�J�ҤH                ù�J����       �w����[0m
else
	at $b echoat $b [1;32mù�J�ҤH                ù�J����       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100004
	at $b echoat $b [1;35m�ܺإ��s                ù�J����       �w����[0m
else
	at $b echoat $b [1;32m�ܺإ��s                ù�J����       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100008
	at $b echoat $b [1;35m�խߥ��~                �խ߲`�W       �w����[0m
else
	at $b echoat $b [1;32m�խߥ��~                �խ߲`�W       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100016
	at $b echoat $b [1;35m��v�c�]1               �խ߲`�W       �w����[0m
else
	at $b echoat $b [1;32m��v�c�]1               �խ߲`�W       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100032
	at $b echoat $b [1;35m��v�c�]2               �խ߲`�W       �w����[0m
else
	at $b echoat $b [1;32m��v�c�]2               �խ߲`�W       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100064
	at $b echoat $b [1;35m��v����                �խ߲`�W       �w����[0m
else
	at $b echoat $b [1;32m��v����                �խ߲`�W       ������[0m
endif
if is_qmark($b) & adv_book_1_west 116384
	at $b echoat $b [1;35m�j�N���s                �T�_����       �w����[0m
else
	at $b echoat $b [1;32m�j�N���s                �T�_����       ������[0m
endif
if is_qmark($b) & adv_book_1_west 132768
	at $b echoat $b [1;35m��¤��F                �T�_����       �w����[0m
else
	at $b echoat $b [1;32m��¤��F                �T�_����       ������[0m
endif
if is_qmark($b) & adv_book_1_west 200004
	at $b echoat $b [1;35m�c�]�M�h                �T�_����       �w����[0m
else
	at $b echoat $b [1;32m�c�]�M�h                �T�_����       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100128
	at $b echoat $b [1;35m���]�s                  ��������       �w����[0m
else
	at $b echoat $b [1;32m���]�s                  ��������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100256
	at $b echoat $b [1;35m�ڭ{                    ��������       �w����[0m
else
	at $b echoat $b [1;32m�ڭ{                    ��������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 100512
	at $b echoat $b [1;35m�ڵ�                    ��������       �w����[0m
else
	at $b echoat $b [1;32m�ڵ�                    ��������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 101024
	at $b echoat $b [1;35m�J���S                  ��������       �w����[0m
else
	at $b echoat $b [1;32m�J���S                  ��������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 102048
	at $b echoat $b [1;35m�{���D�R                ��������       �w����[0m
else
	at $b echoat $b [1;32m�{���D�R                ��������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 104096
	at $b echoat $b [1;35m�k��                    ��������       �w����[0m
else
	at $b echoat $b [1;32m�k��                    ��������       ������[0m
endif
if is_qmark($b) & adv_book_1_west 108192
	at $b echoat $b [1;35m�¤����s���w            ��������       �w����[0m
else
	at $b echoat $b [1;32m�¤����s���w            ��������       ������[0m
endif
def_rembch null-tar
~
>room_defun b06~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b07~
at $b echoat $b .		[1;36m[�v�d�Ԩ��F��ϰ�Τj�F�z�� I][0m
if is_qmark($b) & adv_book_1_main 200008
	at $b echoat $b [1;35m�j����                  ���F����       �w����[0m
else
	at $b echoat $b [1;32m�j����                  ���F����       ������[0m
endif
if is_qmark($b) & adv_book_1_main 200016
	at $b echoat $b [1;35m�j�Ԩ�                  ���F����       �w����[0m
else
	at $b echoat $b [1;32m�j�Ԩ�                  ���F����       ������[0m
endif
if is_qmark($b) & adv_book_1_main 200064
	at $b echoat $b [1;35m�̹D��                  �ɥﴵ�촶     �w����[0m
else
	at $b echoat $b [1;32m�̹D��                  �ɥﴵ�촶     ������[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100016
	at $b echoat $b [1;35m��������                ������         �w����[0m
else
	at $b echoat $b [1;32m��������                ������         ������[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100032
	at $b echoat $b [1;35m����                    ������         �w����[0m
else
	at $b echoat $b [1;32m����                    ������         ������[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100064
	at $b echoat $b [1;35m���쨸�s����R          ������         �w����[0m
else
	at $b echoat $b [1;32m���쨸�s����R          ������         ������[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100128
	at $b echoat $b [1;35m�ͧ��ܿ�                ������         �w����[0m
else
	at $b echoat $b [1;32m�ͧ��ܿ�                ������         ������[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100256
	at $b echoat $b [1;35m�C�ɥ��H�𬥴�          ������         �w����[0m
else
	at $b echoat $b [1;32m�C�ɥ��H�𬥴�          ������         ������[0m
endif
if is_qmark($b) & adv_book_1_east 100512
	at $b echoat $b [1;35m�ȼܬ}�ޥ��H            �`�g��         �w����[0m
else
	at $b echoat $b [1;32m�ȼܬ}�ޥ��H            �`�g��         ������[0m
endif
if is_qmark($b) & adv_book_1_east 100008
	at $b echoat $b [1;35m�Ӥk���f��              �`�g��         �w����[0m
else
	at $b echoat $b [1;32m�Ӥk���f��              �`�g��         ������[0m
endif
if is_qmark($b) & adv_book_1_east 100016
	at $b echoat $b [1;35m�Ԯu�d                  �`�g��         �w����[0m
else
	at $b echoat $b [1;32m�Ԯu�d                  �`�g��         ������[0m
endif
if is_qmark($b) & adv_book_1_east 100032
	at $b echoat $b [1;35m������L�i              �`�g��         �w����[0m
else
	at $b echoat $b [1;32m������L�i              �`�g��         ������[0m
endif
if is_qmark($b) & adv_book_1_east 100064
	at $b echoat $b [1;35m���k��                  �`�g��         �w����[0m
else
	at $b echoat $b [1;32m���k��                  �`�g��         ������[0m
endif
if is_qmark($b) & adv_book_1_east 100128
	at $b echoat $b [1;35m�V�P�E�Y�D              �`�g�h�A       �w����[0m
else
	at $b echoat $b [1;32m�V�P�E�Y�D              �`�g�h�A       ������[0m
endif
if is_qmark($b) & adv_book_1_east 100256
	at $b echoat $b [1;35m�U�j��                �`�g�h�A       �w����[0m
else
	at $b echoat $b [1;32m�U�j��                �`�g�h�A       ������[0m
endif
if is_qmark($b) & adv_book_1_east 101024
	at $b echoat $b [1;35m�����s��                �`�g�h�A       �w����[0m
else
	at $b echoat $b [1;32m�����s��                �`�g�h�A       ������[0m
endif
def_rembch null-tar
~
>room_defun b08~
at $b echoat $b .		[1;36m[�v�d�Ԩ��F��ϰ�Τj�F�z�� II][0m
if is_qmark($b) & adv_book_1_east 200002
	at $b echoat $b [1;35m�j�c�]                  �ë���       �w����[0m
else
	at $b echoat $b [1;32m�j�c�]                  �ë���       ������[0m
endif
if is_qmark($b) & adv_book_1_east 100001
	at $b echoat $b [1;35m���J                    �Z��˪L       �w����[0m
else
	at $b echoat $b [1;32m���J                    �Z��˪L       ������[0m
endif
if is_qmark($b) & adv_book_1_east 100002
	at $b echoat $b [1;35m�㽬�R                  �Z��˪L       �w����[0m
else
	at $b echoat $b [1;32m�㽬�R                  �Z��˪L       ������[0m
endif
if is_qmark($b) & adv_book_1_east 100004
	at $b echoat $b [1;35m�ɤڬ��q                �Z��˪L       �w����[0m
else
	at $b echoat $b [1;32m�ɤڬ��q                �Z��˪L       ������[0m
endif
if is_qmark($b) & adv_book_1_east 200001
	at $b echoat $b [1;35m����k��                ���櫰         �w����[0m
else
	at $b echoat $b [1;32m����k��                ���櫰         ������[0m
endif
def_rembch null-tar
~
>room_defun b09~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b10~
at $b echoat $b .		[1;36m[����@�ɥH�Υ~��a�� I][0m
if is_qmark($b) & adv_book_1_other 100001
	at $b echoat $b [1;35m�j���C�A�^��            �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�j���C�A�^��            �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100002
	at $b echoat $b [1;35m�t��                    �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�t��                    �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100004
	at $b echoat $b [1;35m�ɥ�����                �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�ɥ�����                �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100008
	at $b echoat $b [1;35m�X��                    �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�X��                    �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100016
	at $b echoat $b [1;35m�̮R                    �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�̮R                    �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100032
	at $b echoat $b [1;35m�J���w��                �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�J���w��                �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100064
	at $b echoat $b [1;35m�t�¤���                �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�t�¤���                �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_other 100128
	at $b echoat $b [1;35m�u�@��                  �Х@��         �w����[0m
else
	at $b echoat $b [1;32m�u�@��                  �Х@��         ������[0m
endif
if is_qmark($b) & adv_book_1_main 200128
	at $b echoat $b [1;35m�«Ҵ�                  ���L�����s     �w����[0m
else
	at $b echoat $b [1;32m�«Ҵ�                  ���L�����s     ������[0m
endif
if is_qmark($b) & adv_book_1_main 116384
	at $b echoat $b [1;35m�N����                  �P��           �w����[0m
else
	at $b echoat $b [1;32m�N����                  �P��           ������[0m
endif
if is_qmark($b) & adv_book_1_main 132768
	at $b echoat $b [1;35m���Ժ���                �I�L�j�a       �w����[0m
else
	at $b echoat $b [1;32m���Ժ���                �I�L�j�a       ������[0m
endif
def_rembch null-tar
~
>room_defun b11~
at $b echoat $b .		[1;36m[����@�ɥH�Υ~��a�� I][0m
if is_qmark($b) & adv_book_1_other 200001
	at $b echoat $b [1;35m���Z                    �n���j�a       �w����[0m
else
	at $b echoat $b [1;32m���Z                    �n���j�a       ������[0m
endif
if is_qmark($b) & adv_book_1_other 200002
	at $b echoat $b [1;35m�h�q��                  �M�e��         �w����[0m
else
	at $b echoat $b [1;32m�h�q��                  �M�e��         ������[0m
endif
def_rembch null-tar
~
>room_defun b12~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b13~
at $b echoat $b .		[1;36m[�g�����q�D��][0m
if is_qmark($b) & adv_book_2_1 100032
	at $b echoat $b [1;35m�d�����@                �����L         �w����[0m
else
	at $b echoat $b [1;32m�d�����@                �����L         ������[0m
endif
def_rembch null-tar
~
>room_defun b14~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b15~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b16~
at $b echoat $b .		[1;36m[�g�����q�`�Ϥ���][0m
if is_qmark($b) & adv_book_2_1 100064
	at $b echoat $b [1;35m���t�O����              ���a�@         �w����[0m
else
	at $b echoat $b [1;32m���t�O����              ���a�@         ������[0m
endif
if is_qmark($b) & adv_book_2_1 100128
	at $b echoat $b [1;35m�l�尭���@�@            ����@�@       �w����[0m
else
	at $b echoat $b [1;32m�l�尭���@�@            ����@�@       ������[0m
endif
if is_qmark($b) & adv_book_2_1 100256
	at $b echoat $b [1;35m���F�����@�@            ����@�@       �w����[0m
else
	at $b echoat $b [1;32m���F�����@�@            ����@�@       ������[0m
endif
if is_qmark($b) & adv_book_2_1 100512
	at $b echoat $b [1;35m���̴c�]                ����@�@       �w����[0m
else
	at $b echoat $b [1;32m���̴c�]                ����@�@       ������[0m
endif
if is_qmark($b) & adv_book_2_1 101024
	at $b echoat $b [1;35m�·t�����@�@            ����@�@       �w����[0m
else
	at $b echoat $b [1;32m�·t�����@�@            ����@�@       ������[0m
endif
if is_qmark($b) & adv_book_2_1 102048
	at $b echoat $b [1;35m�Z�ѨϪ��@              ����@�@       �w����[0m
else
	at $b echoat $b [1;32m�Z�ѨϪ��@              ����@�@       ������[0m
endif
if is_qmark($b) & adv_book_2_1 104096
	at $b echoat $b [1;35m��v�̡@                ����@�@       �w����[0m
else
	at $b echoat $b [1;32m��v�̡@                ����@         ������[0m
endif
if is_qmark($b) & adv_book_2_1 108192
	at $b echoat $b [1;35m�H���c�]�@              ����@         �w����[0m
else
	at $b echoat $b [1;32m�H���c�]�@              ����@         ������[0m
endif
if is_qmark($b) & adv_book_2_1 200001
	at $b echoat $b [1;35m�Һ��c�]                ����@         �w����[0m
else
	at $b echoat $b [1;32m�Һ��c�]                ����@         ������[0m
endif
if is_qmark($b) & adv_book_2_1 200002
	at $b echoat $b [1;35m�|�l�Z�Ѩ�              ����@         �w����[0m
else
	at $b echoat $b [1;32m�|�l�Z�Ѩ�              ����@         ������[0m
endif
if is_qmark($b) & wander_quest 200001
	at $b echoat $b [1;35m�Q������@�@            �۴b�g�L       �w����[0m
else
	at $b echoat $b [1;32m�Q������@�@            �۴b�g�L       ������[0m
endif
if is_qmark($b) & wander_quest 200004
	at $b echoat $b [1;35m�r�r�r���@�@            �۴b�g�L       �w����[0m
else
	at $b echoat $b [1;32m�r�r�r���@�@            �۴b�g�L       ������[0m
endif
if is_qmark($b) & wander_quest 200016
	at $b echoat $b [1;35m���̤��a                �۴b�g�L       �w����[0m
else
	at $b echoat $b [1;32m���̤��a                �۴b�g�L       ������[0m
endif
if is_qmark($b) & wander_quest 200064
	at $b echoat $b [1;35m��ļ�~�֡@�@            �۴b�g�L       �w����[0m
else
	at $b echoat $b [1;32m��ļ�~�֡@�@            �۴b�g�L       ������[0m
endif
if is_qmark($b) & wander_quest 200256
	at $b echoat $b [1;35m��{�����@              �۴b�g�L       �w����[0m
else
	at $b echoat $b [1;32m��{�����@              �۴b�g�L       ������[0m
endif
if is_qmark($b) & wander_quest 201024
	at $b echoat $b [1;35m�������ҡ@              �۴b�g�L       �w����[0m
else
	at $b echoat $b [1;32m�������ҡ@              �۴b�g�L       ������[0m
endif
def_rembch null-tar
~
>room_defun b17~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b18~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b19~
at $b echoat $b .		[1;36m[�g�����q����L�ϰ�][0m
if is_qmark($b) & adv_book_2_1 100001
	at $b echoat $b [1;35m���j�c�]                ���j�˪L       �w����[0m
else
	at $b echoat $b [1;32m���j�c�]                ���j�˪L       ������[0m
endif
if is_qmark($b) & adv_book_2_1 100002
	at $b echoat $b [1;35m���Z���H����            ���j�˪L       �w����[0m
else
	at $b echoat $b [1;32m���Z���H����            ���j�˪L       ������[0m
endif
if is_qmark($b) & adv_book_2_1 100004
	at $b echoat $b [1;35m������  �@�@            ���j�˪L       �w����[0m
else
	at $b echoat $b [1;32m������  �@�@            ���j�˪L       ������[0m
endif
if is_qmark($b) & adv_book_2_1 100008
	at $b echoat $b [1;35m���j���饨�~            ���j�˪L       �w����[0m
else
	at $b echoat $b [1;32m���j���饨�~            ���j�˪L       ������[0m
endif
if is_qmark($b) & adv_book_2_1 100016
	at $b echoat $b [1;35m�������l�~�@            ���j�˪L       �w����[0m
else
	at $b echoat $b [1;32m�������l�~�@            ���j�˪L       ������[0m
endif
if is_qmark($b) & sealevil_quest 300001
	at $b echoat $b [1;35m���ǡ@�@�@�@            ���]�}�]       �w����[0m
else
	at $b echoat $b [1;32m���ǡ@�@�@�@            ���]�}�]       ������[0m
endif
if is_qmark($b) & sealevil_quest 300002
	at $b echoat $b [1;35m��ù�񴵡@�@            ���]�}�]       �w����[0m
else
	at $b echoat $b [1;32m��ù�񴵡@�@            ���]�}�]       ������[0m
endif
if is_qmark($b) & sealevil_quest 300004
	at $b echoat $b [1;35m������@                ���]�}�]       �w����[0m
else
	at $b echoat $b [1;32m������@                ���]�}�]       ������[0m
endif
if is_qmark($b) & sealevil_quest 300008
	at $b echoat $b [1;35m���k�@�@�@�@            ���]�}�]       �w����[0m
else
	at $b echoat $b [1;32m���k�@�@�@�@            ���]�}�]       ������[0m
endif
if is_qmark($b) & sealevil_quest 300016
	at $b echoat $b [1;35m�}�纸�@�@              ���]�}�]       �w����[0m
else
	at $b echoat $b [1;32m�}�纸�@�@              ���]�}�]       ������[0m
endif
def_rembch null-tar
~
>room_defun b20~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b21~
at $b echoat $b �ŭ�
def_rembch null-tar
~
>room_defun b22~
at $b echoat $b .		[1;36m[���s���W�[�a�h�]��][0m
if is_qmark($b) & dragonbay_quest_list 100008
	at $b echoat $b [1;35m�ȼܥ��H���@            �a�h�]��       �w����[0m
else
	at $b echoat $b [1;32m�ȼܥ��H���@            �a�h�]��       ������[0m
endif
if is_qmark($b) & dragonbay_quest_list 100016
	at $b echoat $b [1;35m�]���j��ӦZ            �]���O�L       �w����[0m
else
	at $b echoat $b [1;32m�]���j��ӦZ            �]���O�L       ������[0m
endif
if is_qmark($b) & dragonbay_quest_list 100032
	at $b echoat $b [1;35m���]��        �@        �a�h�]��       �w����[0m
else
	at $b echoat $b [1;32m���]��        �@        �a�h�]��       ������[0m
endif
if is_qmark($b) & dragonbay_quest_list 100064
	at $b echoat $b [1;35m�ݦ�N�x�@�@            �a�h�]��       �w����[0m
else
	at $b echoat $b [1;32m�ݦ�N�x�@�@            �a�h�]��       ������[0m
endif
if is_qmark($b) & dragonbay_quest_list 100128
	at $b echoat $b [1;35m�U�v�����@              �a�h�]��       �w����[0m
else
	at $b echoat $b [1;32m�U�v�����@              �a�h�]��       ������[0m
endif
if is_qmark($b) & dragonbay_quest_list 100256
	at $b echoat $b [1;35m�k�����Z�@              �a�h�]��       �w����[0m
else
	at $b echoat $b [1;32m�k�����Z�@              �a�h�]��       ������[0m
endif
def_rembch null-tar
~
|
