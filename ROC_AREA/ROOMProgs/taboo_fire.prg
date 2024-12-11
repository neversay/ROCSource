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
	    echo 這裡高熱的氣溫還有火焰令你感覺人都快脫水了......
	    modhmv pc 01.50 150 0
	    modhmv pc 13.1 10 0
            modrvt t0 = 1
        endif
endif
~
|
