>room_enter 15 100~
if ispc($n)
	if level($n) < 36
		break
	else
		echoat $n �e�Y������A���A�P�줣�ƵΪA....�C
		if rand(10)
			echoat $n �Aı�o�A���G�Q�A�G�F....�C
			add_affect $n curse 0 30 18 -10 1024
			break
		endif
		if rand(10)
			if race($n) == vampire
				break
			else
				echoat $n �Aı�o�A���G���r�F....�C
				add_affect $n poison 0 30 1 -3 4096
			endif
			break
		endif
		if rand(10)
			echoat $n �Aı�o�A���G���Ʀwí�F....�C
			if rand(50)
				add_affect $n 'faerie fire' 0 30 17 50 256
			endif
			break
		endif
		if rand(5)
			echoat $n �Aı�o�A���G�����·��F....�C
			add_affect $n paralyze 0 3 17 -100 268435456
		endif
	endif
endif
~
|
