>room_enter 63 100~
if hour($) >= 0
	if hour($) < 6
		if rand(50)
			echoat $n �`�]�̪���������A���W�X�G�S���H�b���ʡC
		else
			echoat $n �A�P��Q�������R�A�`�]�̪���������X�G�S�������n���C
		endif
		break
	endif
endif
if hour($) >= 6
	if hour($) < 13
		if rand(50)
			echoat $n ������������A��B���O�H�n���m�C
		else
			echoat $n �H�ۤ@�Ѫ���ӡA��������}�l�i�{�o���ͩR�O�C
		endif
		break
	endif
endif
if hour($) >= 13
	if hour($) < 20
		if rand(50)
			echoat $n �U�Ȫ���������A��B���¤H�ӤH��....�C
		endif
		break
	endif
endif
if hour($) >= 20
	if hour($) < 24
		if rand(50)
			echoat $n �J�]�᪺��������A��W�X�G�S������H�C
		else
			echoat $n �Aı�o�Q�����w�R....�C
		endif
	endif
endif
~
|
