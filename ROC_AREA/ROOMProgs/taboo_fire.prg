>room_enter 127 100~
if isnpc($n)
	break
else
if rand(70)
if rand(10)
	c fireball $n 40
endif
if rand(10)
	c flamestrike $n 40
endif
if rand(10)
	c 'star fire' $n 40
endif
endif
        if roomtimer($0) > 0
            break
        else
	    echo �o�̰���������٦����K�O�A�Pı�H���ֲ���F......
	    modhmv pc 01.50 150 0
	    modhmv pc 13.1 10 0
            modrvt t0 = 1
        endif
endif
~
|
