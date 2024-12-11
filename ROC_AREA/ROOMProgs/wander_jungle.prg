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
if robjcount(59120) == 0
	if rand(30)
		oload 59120 1 floor
	endif
else
	if rand(25)
		purge tree1-wander
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
endif
~
>room_defun purge~
if pcnum($) >= 1
	if robjcount(20) >= 1
	or robjcount(21) >= 1
	or robjcount(22) >= 1
	or robjcount(59121) >= 1
	or robjcount(59122) >= 1
		purge ball
		purge spring
		purge magic
		mload 59114
		echo 附近的草叢動了動....
		force andrewsarchus2-wander mpkill $r
	endif
endif
~
>room_defun mirage_beetle~
if insameroom(beetle-wander)
	force beetle-wander mpcall mirage_beetle $n
endif
~
|