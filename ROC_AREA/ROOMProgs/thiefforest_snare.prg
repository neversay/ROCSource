>room_enter 127~
if ispc($n)
	def_rembch $n
	if roomvalue($45301$v2) == 0
		if rand(40)
			snare $b
			rcall snare_arrow_level1 $b
		endif
	else
		if rand(80)
			snare $b
			rcall snare_arrow_level2 $b
		endif
	endif
	def_rembch null-tar
endif
~
>room_rand 80~
if ispc($r)
	def_rembch $r
	if roomvalue($45301$v2) == 0
		if rand(40)
			rcall snare_arrow_level1 $b
		endif
	else
		if rand(70)
			rcall snare_arrow_level2 $b
		endif
	endif
	def_rembch null-tar
endif
~
>room_defun damage~
echoat $b �b�ڨϧA[1;32m*** �A�媽�y ***[m.(331-380)
echoaround $b �b�ڨ� $b [1;32m*** �A�媽�y ***[m.
modhmv $b 01.331 380 0
~
>room_defun none~
echoat $b �b��[1;37m�S������[m�A�I
echoaround $b �b��[1;37m�S������[m $b �I
~
>room_defun snare_arrow_level1~
echoat $b �@�K�b��M�q���O���V�A�g�ӡI�I�I�I�I�I
echoaround $b �@�K�b��M�q���O���V $b �g�h�I�I�I�I�I�I
if dex($n) <= 11
	if rand(30)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 13
	if rand(29)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 15
	if rand(28)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 17
	if rand(27)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 19
	if rand(26)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 21
	if rand(24)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 23
	if rand(22)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 25
	if rand(20)
		rcall damage $b
	else
		rcall none $b
	endif
endif
~
>room_defun snare_arrow_level2~
choat $b �@�K�b��M�q���O���V�A�g�ӡI�I�I�I�I�I
echoaround $b �@�K�b��M�q���O���V $b �g�h�I�I�I�I�I�I
if dex($n) <= 11
	if rand(40)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 13
	if rand(39)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 15
	if rand(38)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 17
	if rand(37)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 19
	if rand(36)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 21
	if rand(34)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 23
	if rand(32)
		rcall damage $b
	else
		rcall none $b
	endif
	break
endif
if dex($n) <= 25
	if rand(30)
		rcall damage $b
	else
		rcall none $b
	endif
endif
~
|