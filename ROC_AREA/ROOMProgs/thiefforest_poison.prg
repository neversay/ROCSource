>room_enter 127 100~
if roomvalue($45340$v2) == 1
	if ispc($n)
		def_rembch $n
		rcall snare_poison $b
	endif
endif
~
>room_defun snare_poison~
if race($b) != vampire
	echoat $b ��өж��G���Ӿ���������A�ϧA������ı�o���ΪA�I
	echoaround $b ��өж��G���Ӿ���������A��$B������ı�o���ΪA�I
	echoat $b �Aı�o��������l.
	add_affect $b poison 0 -1 5 -10 0
	modhmv $b 03.5 5 0
endif
def_rembch null-tar
~
>room_mask quit~
if roomvalue($n45340$v2) == 1
	if race($n) != vampire
		del_affect $n poison
		add_affect $b poison 0 20 5 -10 0
	endif
	transfer $n 3001
	modhmv $n 23.50 50 0
	disconnect $n
endif
~
>room_mask recall~
if roomvalue($n45340$v2) == 0
	modhmv $n 23.50 50 0
	transfer $n 3001
else
	echoat $n [3;31m�����F�A![m
endif
~
>room_mask /~
if roomvalue($n45340$v2) == 0
	modhmv $n 23.50 50 0
	transfer $n 3001
else
	echoat $n [3;31m�����F�A![m
endif
~
|
