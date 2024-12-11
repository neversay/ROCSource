>room_enter 127 100~
if ispc($n)
	if isaffected($n) / prot. from wind
		echoat $n 四周的濃霧剛要在你身邊聚集時，便被疾風防壁給震散了。
	else
        	echoat $n 這股濃霧似乎帶有些許的腐蝕性，你的身體被霧氣侵蝕了！
        	echoat $n 要是你再不快點脫離，你會被濃霧腐蝕掉的。
        	modhmv $n 03.10 10 0
	endif
endif
~
>room_defun sealevil_room_mist~
if ispc($r)
	if isaffected($r) / prot. from wind
		echoat $r 四周的濃霧剛要在你身邊聚集時，便被疾風防壁給震散了。
	else
        	echoat $r 這股濃霧似乎帶有些許的腐蝕性，你的身體被霧氣侵蝕了！
        	echoat $r 要是你再不快點脫離，你會被濃霧腐蝕掉的。
        	modhmv $r 03.10 10 0
	endif
endif
~
>room_mask sca default~
rcall scan1 $n
~
>room_mask scan default~
rcall scan1 $n
~
>room_defun scan1~
if ispc($n)
	if isaffected($n) / day of brightness
		force $n scan
	else
        	echoat $n 霧氣實在是太濃了，你什麼也搜尋不到！
	endif
endif
~
|