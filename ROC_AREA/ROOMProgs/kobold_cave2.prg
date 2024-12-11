>room_defun climbup~
if inroom($n) == 66118
	transfer $n 66126
	force 66126 force $n look
	force 66126 echoaround $n $N來了。
endif
if inroom($n) == 66119
	transfer $n 66127
	force 66127 force $n look
	force 66127 echoaround $n $N來了。
endif
if inroom($n) == 66120
	transfer $n 66128
	force 66128 force $n look
	force 66128 echoaround $n $N來了。
endif
if inroom($n) == 66121
	transfer $n 66129
	force 66129 force $n look
	force 66129 echoaround $n $N來了。
endif
if inroom($n) == 66122
	transfer $n 66130
	force 66130 force $n look
	force 66130 echoaround $n $N來了。
endif
if inroom($n) == 66123
	transfer $n 66131
	force 66131 force $n look
	force 66131 echoaround $n $N來了。
endif
if inroom($n) == 66124
	transfer $n 66132
	force 66132 force $n look
	force 66132 echoaround $n $N來了。
endif
if inroom($n) == 66125
	transfer $n 66133
	force 66133 force $n look
	force 66133 echoaround $n $N來了。
endif
~
>room_command climb default~
echoat $n 你要往哪裡爬？
~
>room_command climb up~
if isfight($n)
	echoat $n 你正在戰鬥中，無法攀爬！
else
	if dex($n) > 19
		if rand(70)
			echoat $n 你試圖向上攀登....
			echoaround $n $N試圖向上攀登....
			rcall climbup $n
		else
			echoat $n 你試圖向上攀登，但是很快地就跌了下來，摔了一身泥！
			echoaround $n $N試圖向上攀登，但是很快地就跌了下來，摔了一身泥！
			modhmv $n 01.250 500 10100001
		endif
	else
		if rand(5)
			echoat $n 你試圖向上攀登....
			echoaround $n $N試圖向上攀登....
			rcall climbup $n
		else
			echoat $n 你試圖向上攀登，但是很快地就跌了下來，摔了一身泥！
			echoaround $n $N試圖向上攀登，但是很快地就跌了下來，摔了一身泥！
			modhmv $n 01.250 500 10100001
		endif
	endif
endif
~
>room_command fly up~
if isaffected($n) & 524288
or race($n) == fairy
or race($n) == beholder
or race($n) == vampire
	echoat $n 你順著山崖朝上飛去....
	echoaround $n $N順著山崖朝上飛去....
	rcall climbup $n
else
	echoat $n 你無法飛翔！
endif
~
|