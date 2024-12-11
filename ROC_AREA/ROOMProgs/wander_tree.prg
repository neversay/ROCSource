>room_rand 50~
if pcnum($) >= 1
	if ispc($r)
		echoat $r 走在這裡，你感到渾身不自在。
		modhmv $r 11.100 200 00000000
		modhmv $r 21.100 200 00000000
	endif
	if rand(50)
		echo 四周的樹葉被你弄得沙沙作響。
	else
		echo 四周傳來奇怪的鳥鳴聲。
	endif
	if rand(25)
		echo 你感覺到好像有東西在看著你。
		if rand(50)
			mload 59009
		else
			mload 59020
		endif
	endif
endif
~
|