>room_enter 127 100~
if ispc($n)
        if isaffected($n) / sanctuary
        or isaffected($n) / divine light
                echoat $n �ϩ��O�P����A���W���t������A�j�q������ۥ|���K��g�b�ӨӡI
		if isaffected($n) / protection
			echoat $n ���O�o�Ǩ���ϩ��L�k�˪�A����������C
		else
                	echoat $n �j�P����������O�A���I�Ө�����I
                	modhmv $n 03.15 25 0
                	modhmv $n 13.15 25 0
                	modhmv $n 23.15 25 0
		endif
        endif
endif
~
>room_defun sealevil_room_stone~
if ispc($r)
        if isaffected($r) / sanctuary
        or isaffected($r) / divine light
                echoat $r �ϩ��O�P����A���W���t������A�j�q������ۥ|���K��g�b�ӨӡI
		if isaffected($r) / protection
			echoat $r ���O�o�Ǩ���ϩ��L�k�˪�A����������C
		else
                	echoat $r �j�P����������O�A���I�Ө�����I
                	modhmv $r 03.15 25 0
                	modhmv $r 13.15 25 0
                	modhmv $r 23.15 25 0
		endif
        endif
endif
~
|