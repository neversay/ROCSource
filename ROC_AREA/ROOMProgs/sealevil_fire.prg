>room_enter 127 100~
if ispc($n)
	if isaffected($n) / prot. from fire
		echoat $n �`���ұa�Ӫ��������A�P�즳�������C
		echoat $n ���W�����K�������A��F������O�W�����ӡC
	else
        	echoat $n �`���ұa�Ӫ����ťO�H�P��۷������C
        	echoat $n �A��������G���I�Ө����F�A���骬�p���_�U���C
        	modhmv $n 03.05 15 0
        	modhmv $n 13.05 15 0
        	modhmv $n 23.05 15 0
	endif
endif
~
>room_defun sealevil_room_fire~
if ispc($r)
	if isaffected($r) / prot. from fire
		echoat $r �`���ұa�Ӫ��������A�P�즳�������C
		echoat $r ���W�����K�������A��F������O�W�����ӡC
	else
        	echoat $r �`���ұa�Ӫ����ťO�H�P��۷������C
        	echoat $r �A��������G���I�Ө����F�A���骬�p���_�U���C
        	modhmv $r 03.05 15 0
        	modhmv $r 13.05 15 0
        	modhmv $r 23.05 15 0
	endif
endif
~
|