>greet_prog 100~
	if is_qmark($n) == forest_enemy 100000
		if is_qmark($n) != forest_quest
			mpechoat $n $I �f�������D����ۤ���F��....�I
			mpechoaround $n $I ���M���_�Z���� $n �ĤF�L�h....�I
			mpechoat $n $I ���M���_�Z�����A�ĤF�L��....�I
			mpat 5 invoke
			hunt $n
		else
			mpechoat $n $I ���G�������ݱ��ڭ̱��ت��H....�I
			mpechoat $n �d���� $I ���_�Z���V�A�ĤF�L��....�I
			mpechoaround $n $I ���M���_�Z���� $n �ĤF�L�h....�I
			mpat 5 invoke
			hunt $n
		endif
	endif
~
>death_prog 100~
	mpdis_magic self 1
	mpat 5 c inf
	if is_qmark($n) != forest_enemy
		mpadd_qmark $n forest_enemy 50 0 0 0 0 1
	else
		mpmod_qmark $n forest_enemy timer + 20
	endif
~
|