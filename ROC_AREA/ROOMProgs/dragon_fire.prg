>room_enter 63 100~
if ispc($n)
	if is_equ_obj($n) == 4249
		modrvt v0 = 1
		break
	endif
	if roomvalue($0) == 0
		echo [1;31m�o�̪����šA���A�P��Q�������ΪA...[m
		modhmv $n 03.3 3 0
	endif
endif
~
>room_rand 10~
if roomvalue($0) == 0
	echo [1;31m�o�̪����šA���A�P��Q�������ΪA....�C[m
	modhmv pc 03.3 3 0
endif
~
>room_leave 255 100~
if ispc($n)
	modrvt v0 = 0
endif
~
|
