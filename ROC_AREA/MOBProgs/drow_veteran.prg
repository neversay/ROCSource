>greet_prog 100~
	if isnpc($n)
		break
	endif
	if race($n) == Drow
		break
	endif
	if isimmort($n)
		bow $n
		break
	endif
	if is_equ_obj($n) == 15025
	if is_equ_obj($n) == 15026
	if is_equ_obj($n) == 15027
	if is_equ_obj($n) == 15028
	if is_equ_obj($n) == 15029
		break
	endif
	endif
	endif
	endif
	endif
	mpkill $n
~
>fight_prog 50~
	if rand(50)
		wear all
		disarm
		bash
		take all
		wear all
	else
		skilldrv multihit
		skilldrv multihit
		skilldrv multihit
	endif
~
|
