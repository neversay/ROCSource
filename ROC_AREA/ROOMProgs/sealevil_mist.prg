>room_enter 127 100~
if ispc($n)
	if isaffected($n) / prot. from wind
		echoat $n �|�P���@����n�b�A����E���ɡA�K�Q�e���������_���F�C
	else
        	echoat $n �o�ѿ@�����G�a���ǳ\���G�k�ʡA�A������Q����I�k�F�I
        	echoat $n �n�O�A�A�����I�����A�A�|�Q�@���G�k�����C
        	modhmv $n 03.10 10 0
	endif
endif
~
>room_defun sealevil_room_mist~
if ispc($r)
	if isaffected($r) / prot. from wind
		echoat $r �|�P���@����n�b�A����E���ɡA�K�Q�e���������_���F�C
	else
        	echoat $r �o�ѿ@�����G�a���ǳ\���G�k�ʡA�A������Q����I�k�F�I
        	echoat $r �n�O�A�A�����I�����A�A�|�Q�@���G�k�����C
        	modhmv $r 03.10 10 0
	endif
endif
~
>room_mask sca default~
rcall scan1 $n
~
>room_mask scan default~
rcall scan1 $n
~
>room_defun scan1~
if ispc($n)
	if isaffected($n) / day of brightness
		force $n scan
	else
        	echoat $n �����b�O�ӿ@�F�A�A����]�j�M����I
	endif
endif
~
|