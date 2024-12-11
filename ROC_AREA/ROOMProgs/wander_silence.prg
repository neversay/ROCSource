>room_defun wander~
if robjcount(59119) == 0
	if rand(50)
		oload 59119 1 floor
	endif
else
	if rand(25)
		purge grass-wander
	endif
endif
if robjcount(59118) == 0
	if rand(25)
		oload 59118 1 floor
	endif
else
	if rand(25)
		purge rock-wander
	endif
endif
if robjcount(59136) == 0
	if rand(40)
		oload 59136 1 floor
	endif
else
	if rand(25)
		purge tree2-wander
	endif
endif
if robjcount(59137) == 0
	if rand(40)
		oload 59137 1 floor
	endif
else
	if rand(25)
		purge bush1-wander
	endif
endif
if pcnum($) >= 1
	modrvt v0 + 1
	modrvt t0 + 25
	if ispc($r)
		echoat $r ¨«¦b³o¸Ì¡A§A·P¨ì´ý¨­¤£¦Û¦b¡C
		modhmv $r 11.100 100 00000000
		modhmv $r 21.100 100 00000000
		del_affect $r 'detect invis'
		del_affect $r 'detect hidden'
		del_affect $r fly
	endif
	if roomvalue($0) > 3
		if mobcount(59156) > 0
			wtransfer wisp-wander here
			force wisp-wander mpcall wisp_denote wisp_denote
			modrvt v0 - 3
		endif
	endif
endif
~
>room_defun purge~
if rnplayer($) >= 1
	if robjcount(20) >= 1
	or robjcount(21) >= 1
	or robjcount(22) >= 1
		purge ball
		purge spring
		purge magic
	endif
endif
~
>room_defun mirage_beetle~
if insameroom(beetle-wander)
	force beetle-wander mpcall mirage_beetle $n
endif
~
>room_mask cc default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask music default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask question default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask answer default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask herotalk default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask shout default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask yell default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask rumor default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask gtell default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask tell default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask ; default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask . default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask chat default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
>room_mask chat* default~
echoat $n [33m§AªºÁn­µ¶Ç¤£¥X¥h¡C[m
~
|