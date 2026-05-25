>room_defun mirage_circle~
if pcnum($) >= 1
	if hour($) == 18
		echo [1;33m旭日東昇，太陽緩緩從東方升起。[m
	endif
	if hour($) == 7
		echo [33m夕陽西下，最後一道陽光漸漸消失。[m
	endif
	if hour($) == 8
		echo [1;30m黑夜降臨，夜幕籠罩著大地。[m
	endif
	if hour($) == 9
		if day($) == 1
		or day($) == 3
		or day($) == 5
		or day($) == 8
		or day($) == 10
		or day($) == 12
		or day($) == 15
		or day($) == 17
		or day($) == 19
		or day($) == 22
		or day($) == 24
		or day($) == 26
		or day($) == 29
		or day($) == 31
		or day($) == 33
			echo [1;34m藍色的月亮緩緩升起。[m
			echo [1;34m四周的景物像灑上了一層藍光。[m
		endif
		if day($) == 2
		or day($) == 4
		or day($) == 6
		or day($) == 9
		or day($) == 11
		or day($) == 13
		or day($) == 16
		or day($) == 18
		or day($) == 20
		or day($) == 23
		or day($) == 25
		or day($) == 27
		or day($) == 30
		or day($) == 32
		or day($) == 34
			echo [1;31m紅色的月亮緩緩升起。[m
			echo [1;31m四周的景物像灑上了一層紅光。[m
		endif
		if day($) == 7
		or day($) == 14
		or day($) == 21
		or day($) == 28
		or day($) == 35
			echo [1;35m紅色和藍色的月亮皆緩緩升起。[m
			echo [1;35m四周的景物像灑上了一層紫光。[m
		endif
	endif
endif
~
>room_defun mirage_weather~
if pcnum($) >= 1
	if hour($) == 12
	or hour($) == 13
	or hour($) == 14
	or hour($) == 15
	or hour($) == 16
	or hour($) == 9
	or hour($) == 10
	or hour($) == 11
		echo [1;30m夜深人靜，四周幾乎沒有半點聲響。[m
		if day($) == 1
		or day($) == 3
		or day($) == 5
		or day($) == 8
		or day($) == 10
		or day($) == 12
		or day($) == 15
		or day($) == 17
		or day($) == 19
		or day($) == 22
		or day($) == 24
		or day($) == 26
		or day($) == 29
		or day($) == 31
		or day($) == 33
			echo [1;34m藍色的月亮高掛在天上。[m
			echo [1;34m四周的景物像灑上了一層藍光。[m
			echo [1;34m一個藍色的夜晚。[m
		endif
		if day($) == 2
		or day($) == 4
		or day($) == 6
		or day($) == 9
		or day($) == 11
		or day($) == 13
		or day($) == 16
		or day($) == 18
		or day($) == 20
		or day($) == 23
		or day($) == 25
		or day($) == 27
		or day($) == 30
		or day($) == 32
		or day($) == 34
			echo [1;31m紅色的月亮高掛在天上。[m
			echo [1;31m四周的景物像灑上了一層紅光。[m
			echo [1;31m一個紅色的夜晚。[m
		endif
		if day($) == 7
		or day($) == 14
		or day($) == 21
		or day($) == 28
		or day($) == 35
			echo [1;35m紅色和藍色的月亮高掛在天上。[m
			echo [1;35m四周的景物像灑上了一層紫光。[m
			echo [1;35m一個紫色的夜晚。[m
		endif
	endif
	if hour($) == 18
	or hour($) == 19
	or hour($) == 20
	or hour($) == 21
	or hour($) == 22
	or hour($) == 23
		echo [1;36m一個爽朗的早晨，微風徐徐的吹到你的臉龐上。[m
	endif
	if hour($) == 1
	or hour($) == 2
	or hour($) == 3
	or hour($) == 4
	or hour($) == 5
		echo [1;36m一個快活的下午，四周都充滿了活力。[m
	endif
endif
~
|
