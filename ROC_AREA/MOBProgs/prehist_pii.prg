>talk_prog topic~
if is_qmark($i) == prehist_pii_quest
	if being($b)
		if is_qmark($i) == prehist_pii_quest 100000
			if rembch($n)
				say �ֱa�ڥh��ڪ��ݷݧa�I
			endif
			break
		endif
		if is_qmark($i) == prehist_pii_quest 100003
			if rembch($n)
				sm $b
				say �ܺa���P $b �z�@�_�b�o��Q�צ����߻P�M���Ʊ��C
			endif
			break
		else
			if rembch($n)
				sm $n
			else
				say ��p�A�ڲ{�b�S�šC
			endif
		endif
	else
		mpdel_qmark self prehist_pii_quest
		say ����A�A������A�A�A���@�M�I
		break
	endif
endif
if is_qmark($i) == prehist_talking
or is_qmark($n) == prehist_enemy
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say ���}�A���n�צ�ڪ����C
	break
endif
say ��p�A�ڲ{�b�ܦ��A������Ƨֻ��I
mpechoat $n -- �E�аݤ@�U�����]�M�����I��(background)�ΥѨӡC
mpechoat $n -- �E�аݤ@�U�������j�˪L(prehist forest)���@�ǨƱ��C
mpechoat $n -- �E�аݤ@�U�z�O������(class)�H
if is_qmark($n) == prehist_xue_quest 100001
	mpechoat $n -- �E�аݤ@�U�z���O�Τ���ۼƨ���(please)��L�H�̪��H
	if is_qmark($n) & prehist_xue_quest 200001
		if is_qmark($n) !& prehist_xue_quest 300001
			mpechoat $n -- �E��������ӭn�������A(please pii)�~��H
		endif
	endif
endif
if is_qmark($n) !& prehist_quest 100004
	if is_qmark($n) == prehist_pii_quest 100000
		mpechoat $n -- �E�A���A�Q�n�@��ۤv���M(your owned knife)�A���O�ݷݫo�������A�H
	endif
	if is_qmark($n) == prehist_pii_quest 100001
		mpechoat $n -- �E�ڸյ������A(try to help you)�a�I�����A�h�ݰݪ����ѡC
		mpechoat $n -- �E�A�o�ӥ��ʪ��p�l(little damn guy)�A�A�ݷݤ@�w���L���Ҷq�ڡI
	endif
	if is_qmark($n) == prehist_pii_quest 100004
		mpechoat $n -- �E�p�l�A�A�ݷݻ����D�A������I���i�A���M�L���|�ЧA��(directly present)�C
	endif
	if is_qmark($n) == prehist_pii_quest 100014
		mpechoat $n -- �E�p�l�A�ǳƦn�F��(are you ready)�H�ڭ̭n�X�o�h�� ��(Lan) �Ѯv�ǲߤM�ޤF�C
	endif
endif
~
>rand_prog 50~
if being($b)
	if is_qmark($i) == prehist_pii_quest
		if insameroom($b)
		else
			if is_qmark($i) >= prehist_pii_quest 1010
				mpmod_qmark self prehist_pii_quest timer - 5
			endif
		endif
	else
		tell $b �A���ʧ@��b�O�ӺC�F�A�ڭ̤U���A���n�F�I
		if is_qmark($b) < prehist_pii_quest 100012
			mpat $b mpmod_qmark $b prehist_pii_quest v0 = 2
			mpat $b mpmod_qmark $b prehist_pii_quest timer = 72
		else
			mpat $b mpmod_qmark $b prehist_pii_quest v0 = 13
			mpat $b mpadd_qmark $b prehist_pii_waiting 72 0 0 0 0 1
		endif
		mppurge self
	endif
else
	if is_qmark($i) == prehist_pii_quest
		say �S���H�S���������ȴN���}�C���F�C
		curse
		if area($i) != prehistoria
			mpecho $I ���ۤv�����F�a�m�A�K���a�m����V���h....
			mppurge self
		else
			mpdel_qmark self prehist_pii_quest
			mpgoto 15634
		endif
	endif
endif
~
>talk_prog restart~
if is_qmark($n) == prehist_enemy
or is_qmark($i) == prehist_pii_quest
or is_qmark($i) == prehist_talking
or is_qmark($n) & prehist_quest 100004
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say �n���ӡA�����@�ѧڮ���F�A���C
	break
endif
if is_qmark($n) < prehist_pii_quest 100012
	mpdel_qmark $n prehist_pii_quest
	mpechoat $n ����ǨӤ@�}�n�T�u�������Ȥw�g�i�H���s�}�l�F�I�v
else
	if is_qmark($n) != prehist_pii_waiting
		mpmod_qmark $n prehist_pii_quest v0 = 14
		mpmod_qmark $n prehist_pii_quest v2 = 0
		mpechoat $n ����ǨӤ@�}�n�T�u�������Ȥw�g�i�H�ѲĤG���q�}�l�F�I�v
		if mobcount(15656) == 0
			mpat 15634 mpmload 15656
			mppurge self
		else
			mpgoto 15634
		endif
	else
		say �S�O�A�A�ڬݨ�A�N�@�{�l��A
		say �n���ӡA�����@�ѧڮ���F�A���C
	endif
endif
~
>talk_prog p please pii~
if is_qmark($n) == prehist_enemy
or is_qmark($i) == prehist_pii_quest
or is_qmark($i) == prehist_talking
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say ���}�A���n�צ�ڪ����C
	break
endif
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		mpadd_qmark self prehist_talking 5 0 0 0 1 0
		mpforce $n say ��������ӭn�������A(please pii)�~��H
		grin $n
		say �o�ӹ��I�ڷQ�Q��....�A
		mpcallout 2 say ��I�ڱo�n�n�Q�Q��....�A
		mpcallout 4 say �o�ӹ�....�I
		mpcallout 5 mpcall prehist_pii_please $n
	endif
endif
~
>defun_prog prehist_pii_please~
if rand(33)
	sm $n
	mpcallout 1 say �o�˦n�F�A�ڲ{�b����ʿ�....�A
	mpcallout 2 say �����I��Ӫ��n�F....�A
	mpcallout 3 say ������ 50000 �����A���Ӥ����h�a�I
	mpcallout 3 mpmod_qmark $n prehist_xue_quest v1 | 2
	mpcallout 3 mpdel_qmark self prehist_talking
	break
endif
if rand(50)
	grin $n
	mpcallout 1 say �ڷQ�n�Y�n�Y���J�|�C....�A
	mpcallout 2 say �ڷQ....�A�ڷQ�Yù�J�������ѨϳJ�|(angelfood cake)�C
	mpcallout 2 mpmod_qmark $n prehist_xue_quest v1 | 4
	mpcallout 2 mpdel_qmark self prehist_talking
else
	ponder $n
	mpcallout 1 say ����....�A
	mpcallout 2 say �ڷQ�n�Y�ë����������o�z�̪�(cream puffed-rice)�C
	mpcallout 2 mpmod_qmark $n prehist_xue_quest v1 | 8
	mpcallout 2 mpdel_qmark self prehist_talking
endif
~
>bribe_prog 50000~
mpjunk all.coins
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		if is_qmark($n) & prehist_xue_quest 200002
			nod $n
			say �o���ٮt���h�A��A�������\�F�C
			mpmod_qmark $n prehist_xue_quest v2 | 1
		else
			say �ڤ��O�s�A���o�ӪF�赹�ڧa�I
			mpgold $n 50000
		endif
	else
		say �A���ǿ����ڭn������H
		mpgold $n 50000
	endif
else
	say �A���ǿ����ڭn������H
	mpgold $n 50000
endif
~
>give_prog 8253~
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		if is_qmark($n) & prehist_xue_quest 200004
			nod $n
			say �o���ٮt���h�A��A�������\�F�C
			mpmod_qmark $n prehist_xue_quest v2 | 1
		else
			say �ڤ��O�s�A���o�ӪF�赹�ڧa�I
			drop angelcake
		endif
	else
		say �A���o�ӪF�赹�ڷF����H
		drop angelcake
	endif
else
	say �A���o�ӪF�赹�ڷF����H
	drop angelcake
endif
~
>give_prog 13402~
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		if is_qmark($n) & prehist_xue_quest 200008
			nod $n
			say �o���ٮt���h�A��A�������\�F�C
			mpmod_qmark $n prehist_xue_quest v2 | 1
		else
			say �ڤ��O�s�A���o�ӪF�赹�ڧa�I
			drop puffed-rice
		endif
	else
		say �A���o�ӪF�赹�ڷF����H
		drop puffed-rice
	endif
else
	say �A���o�ӪF�赹�ڷF����H
	drop puffed-rice
endif
~
>talk_prog please~
if is_qmark($n) == prehist_enemy
or is_qmark($i) == prehist_pii_quest
or is_qmark($i) == prehist_talking
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say ���}�A���n�צ�ڪ����C
	break
endif
if is_qmark($n) == prehist_xue_quest 100001
	mpadd_qmark self prehist_talking 5 0 0 0 1 0
	mpforce $n say �аݤ@�U�z���O�Τ���ۼƨ���(please)��L�H�̪��H
	if is_qmark($n) !& prehist_xue_quest 300001
		hmm
		say ����o�ӴN�O�ڦb�檺....�A
		mpcallout 1 say ���L�p�G�n�ڧi�D�A�������O�H....�A
		mpcallout 2 say �A���ӭn���Q��k�����ڤ~��C
		mpcallout 2 gig
		mpcallout 2 mpmod_qmark $n prehist_xue_quest v1 | 1
		mpcallout 2 mpdel_qmark self prehist_talking
	else
		nod $n
		say �������H�ڡI�a�@�i�L��....�A
		mpcallout 1 say �����A�n������[���诫�����ޥ�....�A
		mpcallout 2 say �~������D�Ӹ��軡���Ǹ�....�A
		mpcallout 3 say ��M�A�o�ӨS�����������O�O�줣�쪺�աI
		mpcallout 3 cac
		mpcallout 3 mpmod_qmark $n prehist_xue_quest v2 | 1
		mpcallout 3 mpdel_qmark self prehist_talking
	endif
endif
~
>talk_prog background~
if is_qmark($i) == prehist_pii_quest
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say ���}�A���n�צ�ڪ����C
	break
endif
mpforce $n say �аݤ@�U�����]�M�����I��(background)�ΥѨӡC
faint $n
say �ڲ{�b�����ɶ���A�ͳo�ǰڡH
wave $n
n
e
s
w
~
>talk_prog p prehist forest~
if is_qmark($i) == prehist_pii_quest
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say ���}�A���n�צ�ڪ����C
	break
endif
mpforce $n say �аݤ@�U�������j�˪L(prehist forest)���@�ǨƱ��C
angry
say ���U�A�ګܦ��A���n���o������[�֪��p�ƨӷЧڦ�ܡH
kick $n
~
>talk_prog class~
if is_qmark($i) == prehist_pii_quest
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say �S�O�A�A�ڬݨ�A�N�@�{�l��A
	say ���}�A���n�צ�ڪ����C
	break
endif
mpforce $n say �аݤ@�U�z�O������(class)�H
mpecho $I �ΫD�`�Y���������ݵۧA....
say ��è�A�A�ݤ��X�ӶܡH�ڬO�L�ҨƨưڡI
laugh $n
~
>death_prog 100~
mpdis_magic self blindness
mapt 5 c inf
if ispc($n)
	if is_qmark($n) != prehist_enemy
		mpadd_all_qmark $n prehist_enemy -1 0 0 0 1 1
	endif
	yell �����ѷݷݡI�аO�o���ڳ����ڡI
	if mobcount(15633) > 0
		mpat golden-prehist mpforce golden-prehist yell �O�֡H�O�ֱ��F�p�l�H
		mptransfer golden-prehist
		mpecho ���M ������(Golden Elder) �X�{�b�o��....
		mpforce golden-prehist hunt $n
	endif
endif
~
|