>room_defun wander~
if robjcount(59119) == 0
	if rand(75)
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
if pcnum($) >= 1
	if ispc($r)
		echoat $r ���b�o�̡A�A�P��������ۦb�C	
		modhmv $r 11.100 100 00000000
		modhmv $r 21.100 100 00000000
		del_affect $r 'detect invis'
		del_affect $r 'detect hidden'
		del_affect $r fly
	endif
	if rand(50)
		echo �|�P�Ǩөǲ��������n�C
	else
		echo �|�P�ǨӦy�U���~�q�n�C
	endif
	if rand(50)
		echo ���񪺯��O�b�F�F���@�T�C
	else
		echo �A�̵}ť��p��K�Q���_���n���C
	endif
endif
~
>room_defun mirage_beetle~
if insameroom(beetle-wander)
	force beetle-wander mpcall mirage_beetle $n
endif
~
|