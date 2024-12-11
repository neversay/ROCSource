>room_enter 127 100~
if ispc($n)
or ishorse($n)
	echoat $n 強烈的風雪颳得你渾身發疼，寸步難行！
	echoat $n 你感到相當寒冷......
	modhmv $n 03.05 05 0
endif
~
>room_defun snow_snowstorm~
if ispc($r)
or ishorse($r)
	echo 強烈的風雪颳得你渾身發疼，寸步難行！
	echo 你感到相當寒冷......
	modhmv pc 03.05 05 0
endif
~
|