>talk_prog topic~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
	stare $n
	say �A�O�ְڡH�S�ƧO�ӷЧڡC
else
	glare $n
	mpcallout 1 say �o���O�Ʀ��Ѥ������x�A���U�ڭ���Ӫ�$N�ܡH
	mpcallout 2 say �Ч��I�Ч��I
	mpcallout 2 mpecho $I�q�@�Ǫ��������X�@�i�y���ǲ����Ȥl�X�ӡC
	mpcallout 2 say ������O�ڥi�H���A�ĳ�(Help)���ܡH
endif
~
>defun_prog destroyarmor~
if is_inv_obj($n) == 13667
or is_inv_obj($n) == 13669
or is_inv_obj($n) == 13670
	mppurge $n energy-elftown
	mpcall destroyarmor $n
endif
~
>talk_prog restart~
if is_qmark($n) == elftown_armorcraft 110000
	if is_inv_obj($n) == 13667
	or is_inv_obj($n) == 13669
	or is_inv_obj($n) == 13670
		mpcall destroyarmor $n
		mpechoat $n �A�N$I���e���A�һs����q�F���ٵ��F$I�C
		mpechoaround $n $N�N$I���e��$N�һs����q�F���ٵ��F$I�C
		mpmod_qmark $n elftown_armorcraft v0 = 0
		mpmod_qmark $n elftown_armorcraft v1 = 0
		mpmod_qmark $n elftown_armorcraft v2 = 0
		mpcallout 1 mpecho $I�ΩG�y�N$N����q�F�𵹾P���F�C
	else
		say �N�ڤ��e���A�j�ƪ��˳Ʊa�ӧa�I
	endif
endif
~
>talk_prog help~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
or is_qmark($n) == elftown_armorcraft 110000
	if is_qmark($n) == elftown_armorcraft 110000
		say �ڤ��O�w�g���A�j�ƹL�˳ƤF�ܡH
		mpcallout 1 say �ڳ̦h�u�|���@�ӤH���y�@��˳ơC
		mpcallout 2 say ���L�A�p�G�A��ڤ��e���A�j�ƹL���˳Ƥ����N�C
		mpcallout 3 say �A�i�H��ڤ��e���A�j�ƪ��˳��ٵ��ڡC
		mpcallout 4 say �o�˧ڴN�|���S��(Restart)���A�A�ȹL�C
	else
		stare $n
		say �A�O�ְڡH�S�ƧO�ӷЧڡC
	endif
else
	mpadd_qmark $n elftown_armorcraft -1 0 0 0 0 1
	mpechoat $n �A�X�o�Ӿ��|�V�ܥd�����Ӷq���ۤv�j�Ƹ˳ƪ��Ʊ��C
	mpechoaround $n $N�X�o�Ӿ��|�V�ܥd�����Ӷq���ۤv�j�Ƹ˳ƪ��Ʊ��C
	mpcallout 1 ponder
	mpcallout 2 say �ڬO�w�g�ܤ[�S�����O�H���y�L�˳ƤF��....
	mpcallout 3 say ���L�J�M�O���F�ڪ��B�ʹ��X���A���ڤ]���n�m�����z�C
	mpcallout 4 say �]�}�I�ڳ̪񥿦n�b���զp�����P�t�������ĦX�b�@�_�C
	mpcallout 5 say �ھڧڪ��q�Q�A�o�N�i�H�ͦ��²b�B�j�j���]�k��q�C
	mpcallout 6 say �󭫭n���O�A�o���]�k��q�i�H�g�ѯS�O����q�����ޱ��A
	mpcallout 7 say �Ǧ��i�H�N�����]�k�˳ƭ��s�ҤơA�Ϩ�ŦX��D�H���ݨD�C
	mpcallout 8 say ���L�o�ӹ����٨S�������A���t�����n�������Ҥ~�A��ڤ]�٤��M���C
	mpcallout 9 say �p�G�z������Ѩ������k�O���A�ӥB����(Material)�]�R�����ܡA
	mpcallout 10 say �ڭˬO�����N�Ӭ��z�դ@��(Try)�աC
endif
~
>talk_prog material~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
or is_qmark($n) == elftown_armorcraft 110000
or is_qmark($n) != elftown_armorcraft
	if is_qmark($n) == elftown_armorcraft 110000
		say �ڤ��O�w�g���A�j�ƹL�˳ƤF�ܡH
		mpcallout 1 say �ڳ̦h�u�|���@�ӤH���y�@��˳ơC
		mpcallout 2 say ���L�A�p�G�A��ڤ��e���A�j�ƹL���˳Ƥ����N�C
		mpcallout 3 say �A�i�H��ڤ��e���A�j�ƪ��˳��ٵ��ڡC
		mpcallout 4 say �o�˧ڴN�|���S��(Restart)���A�A�ȹL�C
	else
		stare $n
		say �A�O�ְڡH�S�ƧO�ӷЧڡC
	endif
else
	say �n�s�y�X�ڻ����o�ظ˳ơA�ݭn�j�qĭ�t���P�t���������ơC
	mpcallout 1 say ���~�A�ٻݭn�j�q���]�O�_�ۥH���ѯ෽�C
	if is_qmark($n) > elftown_armorcraft 100029
		mpcallout 2 say �ثe�w�и֦̾��D�`�R�Ϊ����������C
	else
		if is_qmark($n) > elftown_armorcraft 100014
			mpcallout 2 say �ثe�w�и̦��ٺ�R�Ϊ����������C
		else
			if  is_qmark($n) > elftown_armorcraft 100004
				mpcallout 2 say �ثe�w�и̦��j�j���Ϊ����������C
			else
				mpcallout 2 say �ثe�w�и̪������������������ΡC
			endif
		endif
	endif
	if is_qmark($n) > elftown_armorcraft 200029
		mpcallout 3 say �٦��D�`�R�Ϊ��t�������C
	else
		if is_qmark($n) > elftown_armorcraft 200014
			mpcallout 3 say �٦��ٺ�R�Ϊ��t�������C
		else
			if  is_qmark($n) > elftown_armorcraft 200004
				mpcallout 3 say �٦��j�j���Ϊ��t�������C
			else
				mpcallout 3 say �٦����������Ϊ��t�������C
			endif
		endif
	endif
	if is_qmark($n) > elftown_armorcraft 300199
		mpcallout 4 say ���~�A�]�O�_�۪��s�q�D�`�R�ΡC
	else
		if is_qmark($n) > elftown_armorcraft 300099
			mpcallout 4 say ���~�A�]�O�_�۪��s�q�ٺ�R�ΡC
		else
			if  is_qmark($n) > elftown_armorcraft 300025
				mpcallout 4 say ���~�A�]�O�_�۪��s�q�j�j���ΡC
			else
				mpcallout 4 say  ���~�A�]�O�_�۪��s�q���������C
			endif
		endif
	endif
	if is_qmark($n) < elftown_armorcraft 100005
	or is_qmark($n) < elftown_armorcraft 200005
	or is_qmark($n) < elftown_armorcraft 300025
		mpcallout 5 say ���ȨS��k�{�b�N���z����(Try)���y�˳ơC
	else
		mpcallout 5 say �A�T�w�n�{�b�N�n�ڬ��z����(Try)���y�˳ƶܡH
	endif
	mpcallout 6 say �p�G�z�Q�n�٧U���ƪ��ܡA�����浹�ӤU���`���ش˴N�i�H�F�C
endif
~
>talk_prog try~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
or is_qmark($n) == elftown_armorcraft 110000
or is_qmark($n) != elftown_armorcraft
	if is_qmark($n) == elftown_armorcraft 110000
		say �ڤ��O�w�g���A�j�ƹL�˳ƤF�ܡH
		mpcallout 1 say �ڳ̦h�u�|���@�ӤH���y�@��˳ơC
		mpcallout 2 say ���L�A�p�G�A��ڤ��e���A�j�ƹL���˳Ƥ����N�C
		mpcallout 3 say �A�i�H��ڤ��e���A�j�ƪ��˳��ٵ��ڡC
		mpcallout 4 say �o�˧ڴN�|���S��(Restart)���A�A�ȹL�C
	else
		stare $n
		say �A�O�ְڡH�S�ƧO�ӷЧڡC
	endif
else
	if is_qmark($n) < elftown_armorcraft 100005
	or is_qmark($n) < elftown_armorcraft 200005
	or is_qmark($n) < elftown_armorcraft 300025
	or is_qmark($i) == working
		if is_qmark($i) == working
			say �ڳ̪�ܦ��A�L�X�ѦA���a�I
		else
			say �ثe�w�Ф��������Y�������A�L�k���z���y�˳Ƴ�I
		endif
	else
		nod $n
		mpcallout 1 say �J�M�z�Ʊ檺�ܡA�ڴN���z���լݬݧa�I
		mpcallout 2 say �Х浹�ڤ@��z�Q�n�����ݩʪ��˳ơA
		mpcallout 3 say �˳ƥ������ݩʶV�n�A����᪺�ݩʴN����V�j�C
		mpcallout 4 say ��M�A�U�@�һs���Ѫ��ܡA�o��˳Ƥ]�|��۷��a�C
		mpcallout 5 say ��F�A�Ӯt���˳Ƨڥi�O�|�ڦ����C
		mpcallout 5 mpadd_qmark $n likemyer_workfor -1 0 0 0 0 0
	endif
endif
~
>defun_prog trycraft~
mpecho $I�N�A���˳ƥ��F�]�k���l�̡C
mpcallout 1 mpecho $I�q�w�и̷h�X�F�@����ơC
mpcallout 2 ponder
mpcallout 3 mpecho $I�@�f��N�Ҧ����Ƴ��˦b���l�̤F�I�H
mpcallout 4 mpecho ....�o�ˤ��|�����D�ܡH
mpcallout 5 ponder
mpcallout 6 mpecho $I�@����a��_�G��A�@�䩹���l�K�[���]�O�_��....
mpcallout 7 mpecho �A���˳Ʀb���ŤU�v���Ĥ�....
mpcallout 8 mpecho $I���a��۩G��....
mpcallout 9 mpecho $I���a��۩G��....
mpcallout 10 mpecho $I���a��۩G��....
mpcallout 11 mpecho $I���a��۩G��....
mpcallout 12 mpecho $I���a��۩G��....
mpcallout 13 mpecho $I���a��۩G��....
mpcallout 14 mpecho $I���a��۩G��....
mpcallout 15 mpecho $I���a��۩G��....
mpcallout 16 mpecho $I���a��۩G��....
mpcallout 17 mpecho $I���a��۩G��....
mpcallout 19 mpecho $I���|����A�N��W���E���Ϊ�����q�]�O���J�F���l�����I
mpcallout 20 mpecho �]�k���l�̦��G�G�_�F�@�Υ��~�C
mpcallout 21 mpecho �]�k���l�̪����~�w�w�����....
mpcallout 22 mpecho �]�k���l�̪����~�w�w�����....
mpcallout 23 mpecho �]�k���l�̪����~�w�w�����....
mpcallout 24 mpecho �]�k���l�̪����~�w�w�����....
mpcallout 25 mpecho �]�k���l�̪����~�w�w�����....
if is_qmark($n) < elftown_armorcraft 100005
or is_qmark($n) < elftown_armorcraft 200005
or is_qmark($n) < elftown_armorcraft 300025
or is_qmark($n) != elftown_armorcraft
	mpcall workfail $n
else
	if is_qmark($n) < elftown_armorcraft 100010
	or is_qmark($n) < elftown_armorcraft 200010
	or is_qmark($n) < elftown_armorcraft 300050
		if rand(75)
			mpcall workfail $n
		else
			mpcall worksuccess $n
		endif
	else
		if is_qmark($n) < elftown_armorcraft 100015
		or is_qmark($n) < elftown_armorcraft 200015
		or is_qmark($n) < elftown_armorcraft 300075
			if rand(50)
				mpcall workfail $n
			else
				mpcall worksuccess $n
			endif
		else
			if is_qmark($n) < elftown_armorcraft 100020
			or is_qmark($n) < elftown_armorcraft 200020
			or is_qmark($n) < elftown_armorcraft 300100
				if rand(25)
					mpcall workfail $n
				else
					mpcall worksuccess $n
				endif
			else
				if is_qmark($n) < elftown_armorcraft 100030
				or is_qmark($n) < elftown_armorcraft 200030
				or is_qmark($n) < elftown_armorcraft 300200
					if rand(10)
						mpcall workfail $n
					else
						mpcall worksuccess $n
					endif
				else
					mpcall worksuccess $n
				endif
			endif
		endif
	endif
endif
~
>defun_prog workfail~
mpcallout 26 mpecho �]�k���l�̪����~�C�C�f�H�F�U��....
mpcallout 27 mpecho �]�k���l�̪����~�C�C�f�H�F�U��....
mpcallout 28 mpecho �]�k���l�̪����~�C�C�f�H�F�U��....
mpcallout 29 mpecho �]�k���l�̪����~�C�C�����F�C
mpcallout 30 glare
mpcallout 31 say �ݨӳo�������դS���ѤF�ڡI
mpcallout 32 say �ݨӧ��ٻݭn�A�Q�Q�~��....
mpcallout 33 mpechoat $n $I���F����A���G�b��A�U�v�ȥO�F�C
mpcallout 33 mpechoaround $n $I���F����A���G�b��$N�U�v�ȥO�F�C
mpmod_qmark self working timer = 500
mpdel_qmark $n likemyer_workfor
mpmod_qmark $n elftown_armorcraft v0 = 0
mpmod_qmark $n elftown_armorcraft v1 = 0
mpmod_qmark $n elftown_armorcraft v2 = 0
mpjunk energy-elftown
~
>defun_prog worksuccess~
mpcallout 26 mpecho �]�k���l�̪����~�V�ӶV�G�B�V�ӶV�G....
mpcallout 27 mpecho �]�k���l�̪����~�V�ӶV�G�B�V�ӶV�G....
mpcallout 28 mpecho �]�k���l�̪����~�V�ӶV�G�B�V�ӶV�G....
mpcallout 29 mpecho �]�k���l�̪����~�C�C�������@��í�w�����y�C
mpcallout 30 say ��....���\�F�I�ڦ��\�F�I
mpcallout 31 say �ܦn�A�{�b�ЧA�i�D�ڧA�ҧƱ檺�˳Ư�O�a�I
mpcallout 32 say �ڷQ���ݭn�ڴ����A�A�ү���[�b�˳ƤW���]�k�j�׬O�������C
mpcallout 33 say �]�k�ˮ`(Md)�B�R��(Hr)�B�ˮ`(Dr)�٬O�]�k��O(Pp)�O�H
mpcallout 33 say �����ܩ�(Sve)�B���z�ˮ`�ܩ�(Pdr)�B�]�k�ˮ`�ܩ�(Mdr)�B���z�ˮ`(Pd)�B
mpcallout 33 say ���(Con)�B���m�O(Ac)�B�{�פO(Dc)�B�k�N�P�s���ܩ�(Svs)�B
mpcallout 33 say �A�Ʊ�˳Ƭ��A�W�[���O�O�q(Str)�B���O(Int)�B���z(Wis)�B�ӱ�(Dex)�B
mpcallout 33 mpmod_qmark $n likemyer_workfor v0 = 1
mpmod_qmark $n elftown_armorcraft v0 = 0
mpmod_qmark $n elftown_armorcraft v1 = 0
mpmod_qmark $n elftown_armorcraft v2 = 0
~
>talk_prog str~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300001
	if is_qmark($i) & working 300001
		say �A���˳Ƥw�g�]�t�O�q�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 1
	say �A�Ʊ�W�[�h�ֵ{�ת��O�q�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(3)
endif
~
>talk_prog int~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300002
	if is_qmark($i) & working 300002
		say �A���˳Ƥw�g�]�t���O�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 2
	say �A�Ʊ�W�[�h�ֵ{�ת����O�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(3)
endif
~
>talk_prog wis~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300004
	if is_qmark($i) & working 300004
		say �A���˳Ƥw�g�]�t���z�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 3
	say �A�Ʊ�W�[�h�ֵ{�ת����z�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(3)
endif
~
>talk_prog dex~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300008
	if is_qmark($i) & working 300008
		say �A���˳Ƥw�g�]�t�ӱ��ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 4
	say �A�Ʊ�W�[�h�ֵ{�ת��ӱ��ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(3)
endif
~
>talk_prog con~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300016
	if is_qmark($i) & working 300016
		say �A���˳Ƥw�g�]�t����ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 5
	say �A�Ʊ�W�[�h�ֵ{�ת�����ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(3)
endif
~
>talk_prog ac~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300032
	if is_qmark($i) & working 300032
		say �A���˳Ƥw�g�]�t���m�O�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 6
	say �A�Ʊ�W�[�h�ֵ{�ת����m�O�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(10)
endif
~
>talk_prog dc~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300064
	if is_qmark($i) & working 300064
		say �A���˳Ƥw�g�]�t�{�פO�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 7
	say �A�Ʊ�W�[�h�ֵ{�ת��{�פO�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(10)
endif
~
>talk_prog svs~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300128
	if is_qmark($i) & working 300128
		say �A���˳Ƥw�g�]�t�k�N�P�s���ܩ��ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 8
	say �A�Ʊ�W�[�h�ֵ{�ת��k�N�P�s���ܩ��ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(10)
endif
~
>talk_prog pdr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300256
	if is_qmark($i) & working 300256
		say �A���˳Ƥw�g�]�t���z�ˮ`�ܩ��ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 9
	say �A�Ʊ�W�[�h�ֵ{�ת����z�ˮ`�ܩ��ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(8)
endif
~
>talk_prog mdr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300512
	if is_qmark($i) & working 300512
		say �A���˳Ƥw�g�]�t�]�k�ˮ`�ܩ��ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 10
	say �A�Ʊ�W�[�h�ֵ{�ת��]�k�ˮ`�ܩ��ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(8)
endif
~
>talk_prog pd~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 301024
	if is_qmark($i) & working 301024
		say �A���˳Ƥw�g�]�t���z�ˮ`�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 11
	say �A�Ʊ�W�[�h�ֵ{�ת����z�ˮ`�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(8)
endif
~
>talk_prog md~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 302048
	if is_qmark($i) & working 302048
		say �A���˳Ƥw�g�]�t�]�k�ˮ`�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 12
	say �A�Ʊ�W�[�h�ֵ{�ת��]�k�ˮ`�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(8)
endif
~
>talk_prog hr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 304096
	if is_qmark($i) & working 304096
		say �A���˳Ƥw�g�]�t�R���ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 13
	say �A�Ʊ�W�[�h�ֵ{�ת��R���ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(4)
endif
~
>talk_prog dr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 308192
	if is_qmark($i) & working 308192
		say �A���˳Ƥw�g�]�t�ˮ`�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 14
	say �A�Ʊ�W�[�h�ֵ{�ת��ˮ`�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(5)
endif
~
>talk_prog pp~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 316384
	if is_qmark($i) & working 316384
		say �A���˳Ƥw�g�]�t�]�k��O�ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 15
	say �A�Ʊ�W�[�h�ֵ{�ת��]�k��O�ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(4)
endif
~
>talk_prog sve~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 332768
	if is_qmark($i) & working 332768
		say �A���˳Ƥw�g�]�t�����ܩ��ݩʤF��I
	endif
else
	mpmod_qmark self working v1 = 16
	say �A�Ʊ�W�[�h�ֵ{�ת������ܩ��ݩʩO�H
	say �˳ƥu����[�P�@���ݩʤ@���A���ԷV�Ҽ{��I
	mpechoat $n �@�@�@��(1)��h(8)
endif
~
>talk_prog 0~
if is_qmark($n) != likemyer_workfor 100001
else
	mpmod_qmark self working v1 = 0
	say �i�D�ڧA�ҧƱ檺�˳��ݩʧa�I
endif
~
>defun_prog addability~
nod $n
mpcallout 1 mpecho $I���a��_�G��A�H�]�O�ޱ����]�k���l�̪�����....
mpcallout 2 mpecho �]�k���l�̪����νw�w�����....
mpcallout 3 mpecho �]�k���l�̪����νw�w�����....
mpcallout 4 mpecho �]�k���l�̪����νw�w�����....
mpcallout 6 mpcall isdone $n
~
>defun_prog isdone~
if is_qmark($i) < working 100004
	mpcall doneitem $n
else
	say �p�G�Aı�o�o��˳Ƥw�g�����j���ܡA�N�i�D��(Done)�a�I
endif
~
>defun_prog doneitem~
if is_qmark($i) > working 100000
	mpmod_qmark self working v0 - 1
	mpmodify_obj_affect self energy-elftown 13 + 5 -1
	mpcall doneitem $n
else
	mpmod_qmark $n elftown_armorcraft v0 = 10000
	mpdel_qmark $n likemyer_workfor
	mpdel_qmark self working
	mpecho �]�k���l�̪����νw�w�������ΡC
	mpcallout 1 mpforce $n keep energy-elftown
	mpcallout 1 give energy-elftown $n
	mpcallout 1 say �j�\�i���F�A�h�լ�@�U�A���s�˳Ƨa�I
endif
~
>talk_prog done~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	mpcall doneitem $n
endif
~
>talk_prog 1~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200001
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1
			mpset_obj_affect self energy-elftown -1 1 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�O�q�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200002
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2
			mpset_obj_affect self energy-elftown -1 3 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200003
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4
			mpset_obj_affect self energy-elftown -1 4 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200004
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8
			mpset_obj_affect self energy-elftown -1 2 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ӱ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200005
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16
			mpset_obj_affect self energy-elftown -1 5 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W����ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -10
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -5
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -1
			mpset_obj_affect self energy-elftown -1 24 -1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�R���ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100005
			mpcall addability $n
			mpmod_qmark self working v0 - 5
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k��O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100005
			mpcall addability $n
			mpmod_qmark self working v0 - 5
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -1
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 2~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200001
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1
			mpset_obj_affect self energy-elftown -1 1 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�O�q�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200002
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2
			mpset_obj_affect self energy-elftown -1 3 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200003
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4
			mpset_obj_affect self energy-elftown -1 4 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200004
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8
			mpset_obj_affect self energy-elftown -1 2 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ӱ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200005
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16
			mpset_obj_affect self energy-elftown -1 5 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W����ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -20
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -10
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -2
			mpset_obj_affect self energy-elftown -1 24 -2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 6
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 6
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�R���ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100010
			mpcall addability $n
			mpmod_qmark self working v0 - 10
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k��O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100010
			mpcall addability $n
			mpmod_qmark self working v0 - 10
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -2
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 3~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200001
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1
			mpset_obj_affect self energy-elftown -1 1 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�O�q�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200002
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2
			mpset_obj_affect self energy-elftown -1 3 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200003
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4
			mpset_obj_affect self energy-elftown -1 4 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200004
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8
			mpset_obj_affect self energy-elftown -1 2 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ӱ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200005
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16
			mpset_obj_affect self energy-elftown -1 5 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W����ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -30
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -15
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -3
			mpset_obj_affect self energy-elftown -1 24 -3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 9
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 9
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 6
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 6
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�R���ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100015
			mpcall addability $n
			mpmod_qmark self working v0 - 15
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k��O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100015
			mpcall addability $n
			mpmod_qmark self working v0 - 15
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -3
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 4~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -40
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -20
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -4
			mpset_obj_affect self energy-elftown -1 24 -4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 12
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 12
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 8
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 8
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�R���ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k��O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -4
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 5~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -50
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -25
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -5
			mpset_obj_affect self energy-elftown -1 24 -5
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 15
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 15
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 10
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 10
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100025
			mpcall addability $n
			mpmod_qmark self working v0 - 25
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 5
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100025
			mpcall addability $n
			mpmod_qmark self working v0 - 25
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -5
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 6~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -60
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -30
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -6
			mpset_obj_affect self energy-elftown -1 24 -6
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 18
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 18
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 12
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 12
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100030
			mpcall addability $n
			mpmod_qmark self working v0 - 30
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -6
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 7~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -70
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -35
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -7
			mpset_obj_affect self energy-elftown -1 24 -7
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 21
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 21
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 14
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 14
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100035
			mpcall addability $n
			mpmod_qmark self working v0 - 35
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -7
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 8~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -80
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -40
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -8
			mpset_obj_affect self energy-elftown -1 24 -8
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 24
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 24
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 16
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���z�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 16
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�]�k�ˮ`�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -8
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�����ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 9~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100036
			mpcall addability $n
			mpmod_qmark self working v0 - 36
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -90
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100036
			mpcall addability $n
			mpmod_qmark self working v0 - 36
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -45
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100036
			mpcall addability $n
			mpmod_qmark self working v0 - 36
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -9
			mpset_obj_affect self energy-elftown -1 24 -9
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>talk_prog 10~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -100
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W���m�O�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -50
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�{�פO�ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -10
			mpset_obj_affect self energy-elftown -1 24 -10
			mpcallout 5 say �n���A�ڤw�g���o��˳ƪ��W�k�N�P�s���ܩ��ݩʤF�I
		else
			say �ثe����q�w�g�����z�i��o�˪��W�j�F��I
		endif
	endif
endif
~
>give_prog 37037~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�Υ���ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 80189~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 992~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�i�޵Pĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37036~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�Υ���ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63366~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63367~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63368~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63369~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63080~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o����lĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 80183~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���_��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63341~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�u���Nĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63340~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���Z��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63128~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���y�aĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63129~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o��ܭ�ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 70987~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�Ӥ���ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63132~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���Z��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59172~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���Z��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63001~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���@��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59249~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�u�@����ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59475~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o������ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59474~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�����Yĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59477~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���Y�lĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24221~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���@��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24224~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�ӭ���ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24225~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o��ԳTĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24227~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���H��ĭ�t���D�`�j�j�����t�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 403~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14417~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�����ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14423~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�����ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 70998~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���޵Pĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 40 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12061~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o������ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 38 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 402~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�oŧ���Oĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 38 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 70469~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�������yĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59248~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o������ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12067~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o����Oĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12058~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���Y��ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12059~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���y�aĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59250~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���@�uĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59248~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o������ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59253~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���y�aĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14412~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���Y��ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59254~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���u�lĭ�t���Q���޲�����q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24223~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���@�uĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 80185~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�T�٫�ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14409~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o���@�uĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14404~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o�ڤ���ĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 598~
if is_qmark($n) == likemyer_workfor 100000
	say �@�@....�o����lĭ�t���D�`�j�j�����c�O�q�ڡI
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14211~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 13485~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59251~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59252~
if is_qmark($n) == likemyer_workfor 100000
	say �o�����yĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59156~
if is_qmark($n) == likemyer_workfor 100000
	say �o�i����ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59169~
if is_qmark($n) == likemyer_workfor 100000
	say �o������ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59170~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59171~
if is_qmark($n) == likemyer_workfor 100000
	say �o���@��ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59255~
if is_qmark($n) == likemyer_workfor 100000
	say �o������Zĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63127~
if is_qmark($n) == likemyer_workfor 100000
	say �o���Y�yĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63311~
if is_qmark($n) == likemyer_workfor 100000
	say �o��Ц�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37034~
if is_qmark($n) == likemyer_workfor 100000
	say �o���@�uĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37035~
if is_qmark($n) == likemyer_workfor 100000
	say �o���@�uĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63053~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63054~
if is_qmark($n) == likemyer_workfor 100000
	say �o�����uĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63055~
if is_qmark($n) == likemyer_workfor 100000
	say �o���y�aĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63056~
if is_qmark($n) == likemyer_workfor 100000
	say �o����Mĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 6~
if is_qmark($n) == likemyer_workfor 100000
	say �o����Mĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 8~
if is_qmark($n) == likemyer_workfor 100000
	say �o�ڤ��ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63051~
if is_qmark($n) == likemyer_workfor 100000
	say �o���ñaĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 584~
if is_qmark($n) == likemyer_workfor 100000
	say �o���ߴ�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 596~
if is_qmark($n) == likemyer_workfor 100000
	say �o�i�֥�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37116~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 32130~
if is_qmark($n) == likemyer_workfor 100000
	say �o�Τ��Vĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 15094~
if is_qmark($n) == likemyer_workfor 100000
	say �o�T�٫�ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 15091~
if is_qmark($n) == likemyer_workfor 100000
	say �o���Y��ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63075~
if is_qmark($n) == likemyer_workfor 100000
	say �o���Y�yĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24232~
if is_qmark($n) == likemyer_workfor 100000
	say �o�����ĭ�ê��]�O�ٺ⤣���A�N�Υ��F�a�I
	mpcallout 1 say ���ڱN�����s�ҤƦ��˥ҧa�I
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog all~
if is_qmark($n) == likemyer_workfor
	say �o��˳Ƥ���I
	drop $o
else
	? $n
	drop $o
endif
~
|
