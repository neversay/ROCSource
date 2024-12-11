>room_enter 15 100~
if ispc($n)
	if level($n) < 36
		break
	else
		echoat $n 前頭的死氣，讓你感到不甚舒服....。
		if rand(10)
			echoat $n 你覺得你似乎被詛咒了....。
			add_affect $n curse 0 30 18 -10 1024
			break
		endif
		if rand(10)
			if race($n) == vampire
				break
			else
				echoat $n 你覺得你似乎中毒了....。
				add_affect $n poison 0 30 1 -3 4096
			endif
			break
		endif
		if rand(10)
			echoat $n 你覺得你似乎不甚安穩了....。
			if rand(50)
				add_affect $n 'faerie fire' 0 30 17 50 256
			endif
			break
		endif
		if rand(5)
			echoat $n 你覺得你似乎全身麻痺了....。
			add_affect $n paralyze 0 3 17 -100 268435456
		endif
	endif
endif
~
|
