>tick_prog 100~
if being($b)
	break
else
	mppurge self
endif
~
>repop_prog 100~
mpadd_qmark self statue_hurt -1 0 0 0 0 1
~
>hitprcnt_prog 50~
mpat 5 c 'full heal'
mpgoto self
~
>fight_prog 100~
mpgoto self
~
>hurt_prog 43~
mpat $b mpecho $N�����y�N���o�A[1;33m�r�Q�A��A�����n��[m.(601-700)
mpechoat $n �A�����y�N���B���}�l�j�q���ƤF
mpechoaround $n $N�����y�N���B���}�l�j�q���ƤF
mpat $b mpforcer here modhmv pc 01.601 700 0
mpmod_qmark self statue_hurt v0 + 5
if is_qmark($i) >= statue_hurt 100015
	mpecho $B�q���ƪ��B�J�����F�X��
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 45~
mpat $b mpecho $N���P�P�������o�A[1;31m�I��-��-�o-�_�A ��-��-��-�k�I[m(1151-1300)
mpechoat $n �A���P�P�������B���}�l�j�q���ƤF
mpechoaround $n $N���P�P�������B���}�l�j�q���ƤF
mpat $b mpforcer here modhmv pc 01.1151 1300 0
mpmod_qmark self statue_hurt v0 + 7
if is_qmark($i) >= statue_hurt 100015
	mpecho $B�q���ƪ��B�J�����F�X��
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 7~
mpat $b mpecho $N���`�����x[1;36m�~��[m(13-18)�A.
mpechoat $n �A���`�����x���B���}�l���ƤF
mpechoaround $n $N���`�����x���B���}�l���ƤF
mpat $b mpforcer here modhmv pc 01.13 18 0
mpmod_qmark self statue_hurt v0 + 1
if is_qmark($i) >= statue_hurt 100015
	mpecho $B�q���ƪ��B�J�����F�X��
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 46~
mpat $b mpecho $N���t�������o�A[1;35m�Q�C���[m.(61-85)
mpechoat $n �A���t�������B���}�l���ƤF
mpechoaround $n $N���t�������B���}�l���ƤF
mpat $b mpforcer here modhmv pc 01.61 85 0
mpmod_qmark self statue_hurt v0 + 3
if is_qmark($i) >= statue_hurt 100015
	mpecho $B�q���ƪ��B�J�����F�X��
	mptransfer $b
	mppurge self
endif
~
>death_prog 100~
mpecho $I�Q�~�O���H�F.
mppurge self
~
|