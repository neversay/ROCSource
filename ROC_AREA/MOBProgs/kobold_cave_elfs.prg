>repop_prog 100~
	mpadd_qmark $i delayed_stun -1 0 0 0 1 0
	mpadd_qmark $i delayed_cast -1 99999 0 0 1 0
	mpadd_qmark $i enable_fight_tick -1 0 0 0 1 0
~

>death_prog 100~
yell ����کҼ��R�����F����ڡ���
if is_qmark($n) == kobold_cave_hero
	mpechoat $n �A�N$I���H�Y�٤F�U�ӡI
	mpechoaround $n $N�N$I���H�Y�٤F�U�ӡI
	mpoload 13668 floor
endif
~

>rand_prog 100~
if is_qmark(elf-soldier-marshal) >= wartime 300002
	if is_qmark($r) > kobold_cave_hero 100012
		mpkill $r
	endif
endif
mpkill kobold-soldier
~

>greet_prog 100~
if is_qmark(elf-soldier-marshal) >= wartime 300002
	if is_qmark($n) > kobold_cave_hero 100012
		mpkill $n
	endif
endif
mpkill kobold-soldier
~

>entry_prog 100~
if is_qmark(elf-soldier-marshal) >= wartime 300002
	if is_qmark($r) > kobold_cave_hero 100012
		mpkill $r
	endif
endif
mpkill kobold-soldier
~

>tick_prog 100~
if notbeing(elf-soldier-marshal)
	mppurge self
endif
if is_qmark(elf-soldier-marshal) == wartime 300002
	if rand(50)
		if rand(50)
			if inroom($i) != 66112
				west
			else
				if rand(50)
					north
				else
					south
				endif
			endif
		else
			if inroom($i) != 66115
				east
			else
				if rand(50)
					north
				else
					south
				endif
			endif
		endif
	else
		if rand(50)
			north
		else
			south
		endif
	endif
endif
if is_qmark(elf-soldier-marshal) == wartime 300003
	mpgoto elf-soldier-marshal
endif
if notbeing(elf-soldier-marshal)
or is_qmark(elf-soldier-marshal) >= wartime 100105
	mpecho $I���h�F�C
	mppurge self
endif
~

>rand_prog 100~
if notbeing(elf-soldier-marshal)
	mpecho $I���h�F�C
	mppurge self
endif
if is_qmark(elf-soldier-marshal) == wartime 300002
	if rand(10)
		yell ���˨��c�a�F�A�u�@���F����I
	endif
	if rand(50)
		if rand(50)
			if inroom($i) != 66112
				west
			else
				if rand(50)
					north
				else
					south
				endif
			endif
		else
			if inroom($i) != 66115
				east
			else
				if rand(50)
					north
				else
					south
				endif
			endif
		endif
	else
		if rand(50)
			north
		else
			south
		endif
	endif
endif
if is_qmark(elf-soldier-marshal) == wartime 300002
or insameroom(elf-soldier-marshal)
else
	if inroom($i) != 66109
		mpecho $I�ਭ�µۻ����x���Ӫ���V�ӥh....
		mpgoto elf-soldier-marshal
		mpecho $I�ӤF�C
	endif
endif
~
|