>room_enter 127 15~
if ispc($n)
	if rand(50)
		if rand(50)
			add_affect $n 'faerie fire' 0 24 17 200 256
			echoat $n �Aı�o���餣�ӵΪA�C
		else
			add_affect $n 'tardy' 0 24 39 200 34359738368
			echoat $n �Aı�o���餣�ӵΪA�C
		endif
	else
		if rand(50)
			add_affect $n 'curse' 0 24 40 -30 1024
			echoat $n �Aı�o���餣�ӵΪA�C
		else
			add_affect $n 'weaken' 0 24 19 -30 1073741824
			echoat $n �Aı�o���餣�ӵΪA�C
		endif
	endif
endif
~
|