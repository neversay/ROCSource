>room_rand 20~
if pcnum($) >= 1 
	if ispc($r)
		echoat $r 走在這裡，你感到渾身不自在。
		modhmv $r 11.100 200 00000000
		modhmv $r 21.100 200 00000000
	endif
	if rand(50)
		echo 有幾隻蚊子被吸引來了...
		mload 59017
		mload 59017
	else
		echo 四周傳來奇怪的叫聲...
		if rand(25)
			mload 59104
		endif
	endif
	if rand(25)
		echo 走在滿地的爛泥裡快讓你受不了了。
	endif
endif
~
|