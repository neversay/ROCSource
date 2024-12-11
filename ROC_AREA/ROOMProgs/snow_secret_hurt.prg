>room_enter 127 100~
if ispc($n)
or ishorse($n)
	echoat $n 一陣強烈的不適感猛然襲來，你感到頭昏眼花！
	echoat $n 這裡絕對藏有什麼機關。
	modhmv $n 03.05 05 0
	modhmv $n 13.05 05 0
	modhmv $n 23.05 05 0
endif
~
>room_defun snow_secrethurt~
if ispc($r)
or ishorse($r)
	echo 你感覺到全身的精力正不斷流失......
	echo 身體狀況越來越不妙，你還打算待在這裡嗎？
	modhmv pc 03.05 05 0
	modhmv pc 13.05 05 0
	modhmv pc 23.05 05 0
endif
~
|