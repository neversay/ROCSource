>room_enter 127 100~
if ispc($n)
	if isaffected($n) / prot. from fire
		echoat $n 灼熱所帶來的高溫讓你感到有些難受。
		echoat $n 身上的火焰防壁讓你減除了不少體力上的消耗。
	else
        	echoat $n 灼熱所帶來的高溫令人感到相當難受。
        	echoat $n 你的身體似乎有點承受不了，身體狀況不斷下降。
        	modhmv $n 03.05 15 0
        	modhmv $n 13.05 15 0
        	modhmv $n 23.05 15 0
	endif
endif
~
>room_defun sealevil_room_fire~
if ispc($r)
	if isaffected($r) / prot. from fire
		echoat $r 灼熱所帶來的高溫讓你感到有些難受。
		echoat $r 身上的火焰防壁讓你減除了不少體力上的消耗。
	else
        	echoat $r 灼熱所帶來的高溫令人感到相當難受。
        	echoat $r 你的身體似乎有點承受不了，身體狀況不斷下降。
        	modhmv $r 03.05 15 0
        	modhmv $r 13.05 15 0
        	modhmv $r 23.05 15 0
	endif
endif
~
|