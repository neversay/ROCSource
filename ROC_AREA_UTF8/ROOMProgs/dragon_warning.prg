>room_enter 63 10~
echo [1;31m當你踏進了這裡，四周忽然響起了警報聲....[m
if rand(50)
	echo [1;31m一隻巨大的龍忽然出現，向你猛攻過來....[m
	if rand(20)
		mload 2221
		force dragon hunt $r
		break
	endif
	if rand(25)
		mload 2222
		force dragon hunt $r
		break
	endif
	if rand(33)
		mload 2223
		force dragon hunt $r
	else
		mload 2225
		force dragon hunt $r
	endif
endif
~
|
