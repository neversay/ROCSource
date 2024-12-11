>room_enter 127 100~
if ispc($n)
	if hour($) == 0
	or hour($) == 1
	or hour($) == 2
	or hour($) == 3
	or hour($) == 4
	or hour($) == 21
	or hour($) == 22
	or hour($) == 23
		if day($) == 7
		or day($) == 14
		or day($) == 21
		or day($) == 28
		or day($) == 35
			if roomvalue($0) == 0
				echo [1;35mµµ¯[¼þ¦Ê¦X(Purple Lazurite Lily)¦b¤ë¥úªº·Ó®g¤U°{°{µo¥ú¡C[m
				if rand(10)
					echoat $n ¤@°}·QªkªoµM¦Ó¥Í¡A§A·Q¥hºKªá(pluck lily)¡C
				endif
			endif
		endif
	endif
endif
~
>room_command pluck lily~
if hour($) == 0
or hour($) == 1
or hour($) == 2
or hour($) == 3
or hour($) == 4
or hour($) == 21
or hour($) == 22
or hour($) == 23
	if day($) == 7
	or day($) == 14
	or day($) == 21
	or day($) == 28
	or day($) == 35
		if roomvalue($0) == 0
			echoat $n §AºK¤U¤F¤@¦·[1;35mµµ¯[¼þ¦Ê¦X(Purple Lazurite Lily)[m
			echoaround $n $NºK¤U¤F¤@¦·[1;35mµµ¯[¼þ¦Ê¦X(Purple Lazurite Lily)[m
			oload 59206 30 $n
			modrvt v0 = 1
		endif
	endif
endif
~
|