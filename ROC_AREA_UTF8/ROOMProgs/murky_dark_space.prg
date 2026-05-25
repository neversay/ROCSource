>room_rand 20~
if pcnum($) > 0
	if ispc($r)
		echo [1;32m空間中的重力忽然轉換， $r 控制不了身型而飄游了出去....[m
		if rand(20)
			force $r n
			break
		endif
		if rand(25)
			force $r s
			break
		endif
		if rand(33)
			force $r e
		else
			force $r w
		endif
	endif
endif
~
>room_rand 10~
if pcnum($) > 0
	echo [0;37m忽然眼前空間開始扭曲，並且飄出闇影向你攻擊........[m
	mload 10732
	force shade hunt $r
	force shade skilldrv multihit $r
endif
~
|