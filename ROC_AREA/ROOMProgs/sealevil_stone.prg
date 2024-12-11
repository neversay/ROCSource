>room_enter 127 100~
if ispc($n)
        if isaffected($n) / sanctuary
        or isaffected($n) / divine light
                echoat $n 彷彿是感應到你身上的聖潔光輝，大量的邪氣自四面八方狂奔而來！
		if isaffected($n) / protection
			echoat $n 但是這些邪氣彷彿無法親近你的身體似的。
		else
                	echoat $n 強烈的邪氣衝擊令你有點承受不住！
                	modhmv $n 03.15 25 0
                	modhmv $n 13.15 25 0
                	modhmv $n 23.15 25 0
		endif
        endif
endif
~
>room_defun sealevil_room_stone~
if ispc($r)
        if isaffected($r) / sanctuary
        or isaffected($r) / divine light
                echoat $r 彷彿是感應到你身上的聖潔光輝，大量的邪氣自四面八方狂奔而來！
		if isaffected($r) / protection
			echoat $r 但是這些邪氣彷彿無法親近你的身體似的。
		else
                	echoat $r 強烈的邪氣衝擊令你有點承受不住！
                	modhmv $r 03.15 25 0
                	modhmv $r 13.15 25 0
                	modhmv $r 23.15 25 0
		endif
        endif
endif
~
|