>greet_prog 100~
if isnpc($n)
or isimmort($n)
or race($n) == Drow
	break
endif
if is_equ_obj($n) != 15025
or is_equ_obj($n) != 15026
or is_equ_obj($n) != 15027
or is_equ_obj($n) != 15028
or is_equ_obj($n) != 15029
	mpkill $n
endif
~
>rand_prog 50~
if race($r) == Drow
	break
endif
if is_equ_obj($r) != 15025
or is_equ_obj($r) != 15026
or is_equ_obj($r) != 15027
or is_equ_obj($r) != 15028
or is_equ_obj($r) != 15029
	mpkill $r
endif
~
|
