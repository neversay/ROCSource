>talk_prog p gem-bank~
tell $n �o�O�w������j�q�]�O�_�۫o�S�a��񪺫_�I�̩ҥ[�}���A�ȡC
tell $n �u�n���������N��b�U�a�����]�O�_�۪A�Ȫ��Ȧ��x�s�δ���C
tell $n �}�᪺���欰 100000�A�C���]�O�_���x�s�����欰 500�C
tell $n �C�Ӥ��Y���x�s�̰� 9999 �ӡC
tell $n �C���������H 1 , 5 , 10 , 50 �����i��s�J�λ�X�C
tell $n �u�n�i�D�ڥH�U�����e�A�N�ઽ��i�����C
tell $n �s�J�ƶq���O���Ggem1 , gem5 , gem10 , gem50�C
tell $n ��X�ƶq���O���Grgem1 , rgem5 , rgem10 , rgem50�C
tell $n �t�~�Ъ`�N�A����|�N�s�J�ܮw���_�ۧ@����ơA�ҥH��X���_�ۮįq�i��U���C
tell $n �p�ݭn�}��(gem-account)�i�H������ڻ��C
~
>talk_prog p gem-account~
if goldamt($n) >= 100000
	mpgold $n -100000
	mpoload 3012 $n 1 target
	mpechoat $n $I���F�A�]�O�_�۫���(contract of magic gem).
	tell $n �Ч����O�ޱz�������A�򥢱N���|�ɵo�C
	tell $n �p�Q�A���߰ݸԲӨϥΤ�k�i�H�����ݧ�(gem-bank)�C
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A�S�������������O�ΡC�v
endif
~
>talk_prog p gem1~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 500
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���������ФH�O���]�O�_�ۡC�v
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 1
		mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�ۡC�v
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_1 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p gem5~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 2500
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���������ФH�O�޳o�ƶq���]�O�_�ۡC�v
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 5
		mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���������]�O�_�ۡC�v
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_5 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p gem10~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 5000
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���������ФH�O�޳o�ƶq���]�O�_�ۡC�v
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 10
		mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���������]�O�_�ۡC�v
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_10 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p gem50~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 25000
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���������ФH�O�޳o�ƶq���]�O�_�ۡC�v
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 50
		mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���������]�O�_�ۡC�v
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_50 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p rgem1~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 1
or canweight($n) < 3
	mpechoat $n $I�W���ۻ��G�u�A���W�뤣�U��h�F��F�C�v
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I�q�᭱���w�бN�����ƶq���]�O�_��(magic gem)�浹�A�C
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_1 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p rgem5~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 5
or canweight($n) < 15
	mpechoat $n $I�W���ۻ��G�u�A���W�뤣�U��h�F��F�C�v
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I�q�᭱���w�бN�����ƶq���]�O�_��(magic gem)�浹�A�C
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_5 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p rgem10~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 10
or canweight($n) < 30
	mpechoat $n $I�W���ۻ��G�u�A���W�뤣�U��h�F��F�C�v
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I�q�᭱���w�бN�����ƶq���]�O�_��(magic gem)�浹�A�C
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_10 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
>talk_prog p rgem50~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 50
or canweight($n) < 150
	mpechoat $n $I�W���ۻ��G�u�A���W�뤣�U��h�F��F�C�v
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I�q�᭱���w�бN�����ƶq���]�O�_��(magic gem)�浹�A�C
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_50 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I�W���ۻ��G�u�藍�_�A�A���W�S���]�O�_�۫����A�p�ݨϥΥ��A�Ƚж}��(gem-account)�C�v
endif
~
|