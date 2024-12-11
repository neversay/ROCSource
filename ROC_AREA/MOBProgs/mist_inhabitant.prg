>tick_prog 100~
if hour($) == 6
	if rand(5)
		mpgoto 4527
		break
	endif
	if rand(5)
		mpgoto 4529
		break
	endif
	if rand(5)
		mpgoto 4537
		break
	endif
	if rand(5)
		mpgoto 4539
		break
	endif
	if rand(5)
		mpgoto 4541
		break
	endif
	if rand(5)
		mpgoto 4542
		break
	endif
	if rand(5)
		mpgoto 4544
		break
	endif
	if rand(5)
		mpgoto 4546
		break
	endif
	if rand(5)
		mpgoto 4560
		break
	endif
	if rand(5)
		mpgoto 4565
		break
	endif
	if rand(5)
		mpgoto 4576
		break
	endif
	if rand(5)
		mpgoto 4577
		break
	endif
	if rand(5)
		mpgoto 4585
		break
	endif
	if rand(5)
		mpgoto 4587
	else
		mpgoto 4590
	endif
	break
endif
if hour($) == 22
	if isfight($i)
		break
	else
		mpgoto 4500
	endif
endif
~
|