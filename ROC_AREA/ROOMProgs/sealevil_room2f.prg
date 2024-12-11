>room_enter 127 30~
if ispc($n)
        echoat $n 一股噁心的腥氣迎面襲來，你感到頭昏眼花。
        echoat $n 你覺得身體狀況似乎變差了點。
        modhmv $n 03.5 5 0
	modhmv $n 01.100 100 0
endif
~
>room_defun sealevil_room_2f~
if ispc($r)
        echoat $r 一股噁心的腥氣迎面襲來，你感到頭昏眼花。
        echoat $r 你覺得身體狀況似乎變差了點。
        modhmv $r 03.5 5 0
	modhmv $r 01.100 100 0
endif
~
|