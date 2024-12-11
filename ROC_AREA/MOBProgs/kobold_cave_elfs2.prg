>death_prog 100~
if is_qmark($n) == kobold_cave_hero
	mpechoat $n 你將$I的人頭斬了下來！
	mpechoaround $n $N將$I的人頭斬了下來！
	mpoload 13668 floor
endif
~
|