>room_enter 63 100~
if hour($) >= 0
	if hour($) < 6
		if rand(50)
			echoat $n 深夜裡的光明之鎮，路上幾乎沒有人在走動。
		else
			echoat $n 你感到十分的寧靜，深夜裡的光明之鎮幾乎沒有任何聲音。
		endif
		break
	endif
endif
if hour($) >= 6
	if hour($) < 13
		if rand(50)
			echoat $n 光明之鎮的早晨，到處都是人聲鼎沸。
		else
			echoat $n 隨著一天的到來，光明之鎮開始展現她的生命力。
		endif
		break
	endif
endif
if hour($) >= 13
	if hour($) < 20
		if rand(50)
			echoat $n 下午的光明之鎮，到處依舊人來人往....。
		endif
		break
	endif
endif
if hour($) >= 20
	if hour($) < 24
		if rand(50)
			echoat $n 入夜後的光明之鎮，街上幾乎沒有什麼人。
		else
			echoat $n 你覺得十分的安靜....。
		endif
	endif
endif
~
|
