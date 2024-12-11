>room_defun wander~
if robjcount(59119) == 0
	if rand(30)
		oload 59119 1 floor
	endif
else
	if rand(25)
		purge grass-wander
	endif
endif
if robjcount(59118) == 0
	if rand(25)
		oload 59118 1 floor
	endif
else
	if rand(25)
		purge rock-wander
	endif
endif
if robjcount(59136) == 0
	if rand(30)
		oload 59136 1 floor
	endif
else
	if rand(25)
		purge tree2-wander
	endif
endif
if robjcount(59137) == 0
	if rand(30)
		oload 59137 1 floor
	endif
else
	if rand(25)
		purge bush1-wander
	endif
endif
if pcnum($) >= 1
	modrvt v0 + 1
	modrvt t0 + 25
	if ispc($r)
		echoat $r 走在這裡，你感到渾身不自在。
		modhmv $r 11.100 100 00000000
		modhmv $r 21.100 100 00000000
		del_affect $r 'detect invis'
		del_affect $r 'detect hidden'
		del_affect $r fly
	endif
	if rand(50)
		echo 四周傳來怪異的鳥鳴聲。
	else
		echo 四周傳來尖銳的獸吼聲。
	endif
	if rand(50)
		echo 附近的草叢在沙沙的作響。
	else
		echo 你依稀聽到小樹枝被採斷的聲音。
	endif
	if roomvalue($0) > 3
		mload 59132
		modrvt v0 -3
	endif
endif
~
>room_mask north default~
if isnpc($n)
	transfer $n north
endif
modhmv $n 21.100 100 00000000
if rand(25)
	at $n force $n look
	transfer $n north
	break
endif
if rand(33)
	at $n force $n look
	transfer $n east
	break
endif
if rand(50)
	at $n force $n look
	transfer $n south
else
	at $n force $n look
	transfer $n west
endif
~
>room_mask east default~
if isnpc($n)
	transfer $n east
endif
modhmv $n 21.100 100 00000000
if rand(25)
	at $n force $n look
	transfer $n north
	break
endif
if rand(33)
	at $n force $n look
	transfer $n east
	break
endif
if rand(50)
	at $n force $n look
	transfer $n south
else
	at $n force $n look
	transfer $n west
endif
~
>room_mask south default~
if isnpc($n)
	transfer $n south
endif
modhmv $n 21.100 100 00000000
if rand(25)
	at $n force $n look
	transfer $n north
	break
endif
if rand(33)
	at $n force $n look
	transfer $n east
	break
endif
if rand(50)
	at $n force $n look
	transfer $n south
else
	at $n force $n look
	transfer $n west
endif
~
>room_mask west default~
if isnpc($n)
	transfer $n west
endif
modhmv $n 21.100 100 00000000
if rand(25)
	at $n force $n look
	transfer $n north
	break
endif
if rand(33)
	at $n force $n look
	transfer $n east
	break
endif
if rand(50)
	at $n force $n look
	transfer $n south
else
	at $n force $n look
	transfer $n west
endif
~
>room_defun purge~
if rnplayer($) >= 1
	if robjcount(20) >= 1
	or robjcount(21) >= 1
	or robjcount(22) >= 1
		purge ball
		purge spring
		purge magic
	endif
endif
~
>room_defun mirage_beetle~
if insameroom(beetle-wander)
	force beetle-wander mpcall mirage_beetle $n
endif
~
>room_mask scan default~
echoat $n 這裡霧太濃了，你什麼也看不到。
~
>room_mask scout default~
echoat $n 這裡霧太濃了，你什麼也看不到。
~
|
