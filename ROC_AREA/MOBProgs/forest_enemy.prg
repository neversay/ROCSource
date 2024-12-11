>greet_prog 100~
	if is_qmark($n) == forest_enemy 100000
		if is_qmark($n) != forest_quest
			mpechoat $n $I 口中不知道喃喃唸著什麼東西....！
			mpechoaround $n $I 忽然提起武器往 $n 衝了過去....！
			mpechoat $n $I 忽然提起武器往你衝了過來....！
			mpat 5 invoke
			hunt $n
		else
			mpechoat $n $I 說：為什麼濫殺我們族裏的人....！
			mpechoat $n 悲憤的 $I 拿起武器向你衝了過來....！
			mpechoaround $n $I 忽然提起武器往 $n 衝了過去....！
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