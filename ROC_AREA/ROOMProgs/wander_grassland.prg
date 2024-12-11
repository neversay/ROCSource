>room_defun wander~
if robjcount(59119) == 0
	if rand(75)
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
	if rand(10)
		oload 59120 1 floor
	endif
else
	if rand(25)
		purge tree1-wander
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
>room_enter 63 25~
if rmobcount(59124) >= 1
or rmobcount(59125) >= 1
	force mammoth wake
endif
~
>room_defun deathflinger~
if pcnum($) > 0
	modrvt v0 + 1
	if roomvalue($0) >= 5
		if hour($) == 8
		or hour($) == 9
		or hour($) == 10
		or hour($) == 11
		or hour($) == 12
		or hour($) == 13
		or hour($) == 14
		or hour($) == 15
		or hour($) == 16
			mload 59128
			modrvt v0 - 5
		endif
	endif
endif
~
>room_defun mirage_beetle~
if insameroom(beetle-wander)
	force beetle-wander mpcall mirage_beetle $n
endif
~
|