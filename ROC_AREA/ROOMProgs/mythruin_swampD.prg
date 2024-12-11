>room_time 60~
if pcnum($) > 0
	if ispc($r)
		if rand(50)
			modhmv $r 03.15 30 0
			echoat $r 沼氣侵蝕著你的身體...
		else
			modhmv $r 13.8 16 0
			echoat $r 沼氣侵蝕著你的身體...
		endif
	endif
endif
~
>room_leave 63 100~
if ispc($n)
	if isaffected($n) & 524288
	or race($n) == vampire
	or race($n) == gargoyle
	or race($n) == beholder
	or race($n) == fairy
		break
	else
		modhmv $n 03.9 15 0
		modhmv $n 23.30 45 0
	endif
endif
~
|