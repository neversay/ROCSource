>repop_prog 100~
	mpadd_qmark $i delayed_stun -1 0 0 0 1 0
	mpadd_qmark $i enable_fight_tick -1 0 0 0 1 0
~
>rand_prog 100~
if is_qmark($r) != kobold_cave_hero
or is_qmark($r) <= kobold_cave_hero 100012
	mpkill $r
endif
mpkill elf-soldier
~
>greet_prog 100~
if ispc($n)
	if is_qmark($n) != kobold_cave_hero
	or is_qmark($n) <= kobold_cave_hero 100012
		mpkill $n
	endif
endif
~
>entry_prog 100~
if rand(50)
	if is_qmark($r) != kobold_cave_hero
	or is_qmark($r) < kobold_cave_hero 100012
		mpkill $r
	endif
	mpkill elf-soldier
else
	mpkill elf-soldier
endif
~
|