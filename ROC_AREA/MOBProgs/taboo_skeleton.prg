>fight_prog 60~
mpdis_magic self all
mpecho $I �����߳Q�·t���F���¶�I
mpechoat $n $I [1;36m�a�ժ��Ⱙ���b�A���W�I[m
if is_qmark($n) == taboo_skeleton 100005
	mpechoat $n $I ��[1;36;44m�H�B��Ĳ[1;36m����[1;31m�ᵲ[m�F�A�I
	mpechoat $n [1;36m�A�w�g���F�B�N���Ͷ�...�S���Pı...�S����ı...[m
	mpforcer here modhmv $n 03.100 100 0
	mpechoaround $n $I ��[1;36;44m�H�B��Ĳ[m�����ᵲ�F $n �I
	c 'chill touch' $n
	break
endif
c 'chill touch' $n
mpnowait
if is_qmark($n) == taboo_skeleton 100004
	mpmod_qmark $n taboo_skeleton v0 = 5
	mpechoat $n [1;36m�Aı�o�ۤv�������B�N���w�A�s��Ŧ���֤���F�I[m
	break
endif
if is_qmark($n) == taboo_skeleton 100003
	mpmod_qmark $n taboo_skeleton v0 = 4
	mpechoat $n [1;36m�Aı�o�ۤv�|�Ϻ������w�_�ӡI[m
	break
endif
if is_qmark($n) == taboo_skeleton 100002
	mpmod_qmark $n taboo_skeleton v0 = 3
	mpechoat $n [1;36m�Aı�o�ۤv���}�������h��ı...[m
	break
endif
if is_qmark($n) == taboo_skeleton 100001
	mpmod_qmark $n taboo_skeleton v0 = 2
	mpechoat $n [1;36m�Aı�o�}�y���I�B�N...[m
	break
endif
if is_qmark($n) == taboo_skeleton 100000
	mpmod_qmark $n taboo_skeleton v0 = 1
	mpechoat $n [1;36m�Aı�o���I�B�N...[m
	break
endif
mpadd_qmark $n taboo_skeleton 6 0 0 0 0 1
mpechoaround $n $I ���u�\��b $n �����W��F�@�U�A$n ���G�P��Q���h�W�I
~
>death_prog 100~
if class($n) == 0
or class($n) == 5
or class($n) == 6
or class($n) == 7
or class($n) == 8
or class($n) == 15
or class($n) == 16
	mpecho $I �H���������u......
	mpgreward exp $n -35 100
	mpgreward exp $n 36 50
	mpat 5 mppurge self
	break
else
	if class($n) == 14
	or class($n) == 18
	or class($n) == 19
		if rand(30)
			mpechoat $n $I ���Y���Q�A[1;31m�����@��[m�����H�F�I
			mpechoaround $n $I ���Y���Q $n [1;31m�r�P�@��[m������F�I
			mpecho $I ���O�_�F�u�����w�����@�a...
			mpgreward exp $n -35 200
			mpgreward exp $n 36 100
			if rand(5)
				mpoload 32137
			endif
			mpat 5 mppurge self
			break
		endif
	endif
	if rand(5)
		mpecho $I �b���O���U�H���������u�I
		mpat 5 mppurge self
		break
	endif
	mpecho $I �Q�����@�����z�}�ӡI
	mpecho $I ���H���|���I�Ů𤤥R���F�¦⪺����I
	mpforcer here modhmv pc 03.9 10 0
	mpforcer here modhmv pc 11.100 200 0
	mpecho �A�Q�¦⪺�����o�Y������B�|�ϵL�O......
	mpecho �N��A�P����O�ॢ�������A�A�o�{�H�����դF�_�ӡI
	if rand(25)
		mpmload 32067
		mpecho $I ����հ�����F�a�W�I
		mpecho $I �����]�o�X�����I���G�ܤ��n�H�I
		mpecho $I ����a�W����հ��l�����񪺸H�����F�޲����Ϊ��I
		mpmload 32068
		mpforce fido-fr yell �q���������������������
	else
		mpmload 32067
	endif
	mpforce communution-fr get all
	mpforce comminution-fr mpkill $n
	yell �I�IOn Dead Road�I�ILet me Die�I�I
	mpat 5 mppurge self
endif
~
>repop_prog 100~
mpadd_qmark self delayed_cast -1 99999 0 0 1 0
mpadd_qmark self taboo_skeleton_hurt 6 0 0 0 0 1
~
>hurt_prog 92~
mpnowait
mpecho $I ����հ�����F�a�W�I
mpecho $I �����]�o�X�����I���G�ܤ��n�H�I
mpecho $I ����a�W����հ��l�����񪺸H�����F�޲����Ϊ��I
mpmload 32068
mpforce fido-fr yell �q���������������������
~
>hurt_prog 1005~
if rand(50)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 108~
if rand(60)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 138~
if rand(60)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1000~
if rand(55)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1006~
if rand(40)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1007~
if rand(45)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1008~
if rand(50)
	mpcall taboo_skeleton_load
endif
~
>defun_prog taboo_skeleton_load~
if is_qmark($i) != taboo_skeleton_hurt
	mpadd_qmark self taboo_skeleton_hurt 6 0 0 0 0 1
endif
if is_qmark($i) == taboo_skeleton_hurt 100005
	mpecho $I [1;30;47m�H���b�a�A���F�쥻���H��[m......
	if rand(40)
		mpechoat $n �@���H���u��A���W
		mpoload 32137 $n viclv target
	endif
	mpgreward exp $n -35 300
	mpgreward exp $n 36 200
	mpat 5 mppurge self
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100004
	mpmod_qmark self taboo_skeleton_hurt v0 = 5
	mpecho $I [1;30;47m���G���F�U�ظH���������A�ݨӧ֭n���F�I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100003
	mpmod_qmark self taboo_skeleton_hurt v0 = 4
	mpecho $I [1;30;47m���Y���]�o�X�F�H�����n���I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100002
	mpmod_qmark self taboo_skeleton_hurt v0 = 3
	mpecho $I [1;30;47m���L���o�X�H�����n���I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100001
	mpmod_qmark self taboo_skeleton_hurt v0 = 2
	mpecho $I [1;30;47m���ݰ��W�H���F�@���I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100000
	mpmod_qmark self taboo_skeleton_hurt v0 = 1
	mpecho $I [1;30;47m�����X�{�F�@�I�����I[m
	break
endif
~
|