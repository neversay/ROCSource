>room_enter 127 100~
cast flamestrike $n 30
~
>room rand 80~
if ispc($r)
	if rnplayer($) <= 3
		cast flamestrike $r 30
	endif
	if rnplayer($) <= 2
		cast flamestrike $r 30
	endif
	if rnplayer($) <= 1
		cast flamestrike $r 30
	endif
endif
~	
>room_mask cast gate~
~
>room_mask cast clay golem~
~
>room_mask cast water spirit~
~
>room_mask cast fire elemental~
~
>room_mask hsummon~
~
|
