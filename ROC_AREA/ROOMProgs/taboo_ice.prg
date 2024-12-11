>room_enter 127 100~
if isnpc($n)
	break
else
if rand(60)
if rand(20)
	c 'chill touch' $n 42
endif
if rand(20)
	c 'weaken' $n 42
endif
if rand(20)
	c 'curse' $n 42
endif
endif
if isgood($n)
    if roomtimer($0) > 0
        break
    else
	echo 四周詭異的闇黑能量讓你覺得體力還有精神力都被抽走了一般......
	modhmv pc 03.9 10 0
	modhmv pc 11.100 200 0
        modrvt t0 = 1
        if rand(20)
	    echo 地上的碎骨中忽然站起了兩隻碎骨兵！
	    mload 32067
	    mload 32067
        endif
    endif
endif
endif
~
|
