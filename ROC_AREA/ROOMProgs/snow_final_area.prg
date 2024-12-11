>room_enter 127 100~
if ispc($n)
or ishorse($n)
	if rand(20)
		echoat $n 你忽然感到腳下一滑，接著，你滑倒在早已結冰的地面上！
		echoaround $n $n腳步不穩，碰的一聲摔倒在地！
		modhmv $n 01.50 50 0
	endif
endif
~
|