>greet_prog 100~
if is_qmark($n) == drow_empire_enemy
	star $n
	say 消滅敵人！保衛帝國！
	mpkill $n
	break
endif
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
if is_qmark($r) == drow_empire_enemy
	star $r
	say 消滅敵人！保衛帝國！
	mpkill $r
	break
endif
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
>fight_prog 50~
if rand(50)
	wear broadsword
	disarm
	bash
else
	skilldrv multihit
	skilldrv multihit
endif
~
|
