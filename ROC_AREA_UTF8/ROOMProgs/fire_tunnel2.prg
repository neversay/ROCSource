>room_enter 127 100~
cast star $n 20
~
>room rand 70~
if ispc($r)
	if rnplayer($) <= 3
		cast star $r 20
	endif
	if rnplayer($) <= 2
		cast star $r 20
	endif
	if rnplayer($) <= 1
		cast star $r 20
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
