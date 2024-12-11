>room_rand 70~
if pcnum($) >= 1
	if ispc($r)
		echoat $r 走在這裡，你感到渾身不自在。
		modhmv $r 11.100 200 00000000
		modhmv $r 21.100 200 00000000
	endif
	if rand(50)
		echo 旁邊的小樹叢突然動了一下...
		if rand(50)
			mload 59009
		endif
	else
		echo 突然間四周安靜了下來，你感覺到好像有東西在什麼地方看你...
		if rand(25)
			mload 59006
		endif
	endif
	if rand(40)
		echo 一隻小蟲子爬到了你的鞋子上。
	endif
	if rand(25)
		echo 你覺得前面的空氣開始扭曲...
		c 'cone of silence'
	endif
endif
~
|