>room_enter 127 100~
if ispc($n)
	if isaffected($n) / bless
	or isaffected($n) / day of brightness
		echoat $n 周遭那死者的怨氣似乎無法依附在你身上。
	else
        	echoat $n 死者的怨氣似乎有著神秘的力量，你感到有些不適。
        	echoat $n 你的魔力和體力似乎被吸走了一些......
        	modhmv $n 13.5 5 0
		modhmv $n 11.50 50 0
        	modhmv $n 23.5 5 0
		modhmv $n 21.50 50 0
	endif
endif
~
>room_defun sealevil_room_1f~
if ispc($r)
	if isaffected($r) / bless
	or isaffected($n) / day of brightness
		echoat $r 周遭那死者的怨氣似乎無法依附在你身上。
	else
        	echoat $r 死者的怨氣似乎有著神秘的力量，你感到有些不適。
        	echoat $r 你的魔力和體力似乎被吸走了一些......
        	modhmv $r 13.5 5 0
		modhmv $r 11.50 50 0
        	modhmv $r 23.5 5 0
		modhmv $r 21.50 50 0
	endif
endif
~
|