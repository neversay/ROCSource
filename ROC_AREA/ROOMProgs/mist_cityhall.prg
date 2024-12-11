>room_enter 63 100~
if isnpc($n)
	break
endif
if mobcount(4561) > 8
	break
endif
if hour($) < 8
or hour($) > 17
	echo 當你踏入這裡的時候，忽然整個市議會響起警示的聲音。
	echo 守護市議會的守衛忽然出現了....。
	mload 4561
	force council mpdef_rembch $n
	force council follow $n
	mload 4561
	force council mpdef_rembch $n
	force council mpgroup 2.council self 1 0
	force council follow $n
endif
~
|