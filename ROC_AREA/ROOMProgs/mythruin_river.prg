>room_time 60~
if pcnum($) > 0
	if ispc($r)
		if rand(50)
			modhmv $r 03.8 16 0
			echoat $r 河水中鹹腥的氣味讓你感覺不適...
		else
			modhmv $r 13.6 12 0
			echoat $r 河水中鹹腥的氣味讓你感覺不適...
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
		modhmv $n 03.4 8 0
        	modhmv $n 13.4 8 0
		modhmv $n 23.20 25 0
		echoat $n 這濕黏的河水似乎掠奪著你的能量....
	endif
endif
~
|