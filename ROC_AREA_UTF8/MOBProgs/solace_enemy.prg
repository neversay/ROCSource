>death_prog 100~
mpdis_magic self blindness
mpat 5 c inf
mplog killed by $n
if is_qmark($n) != solace_enemy
	mpadd_qmark $n solace_enemy 200 0 0 0 1 0
else
	mpmod_qmark $n solace_enemy timer + 200
endif
~
|