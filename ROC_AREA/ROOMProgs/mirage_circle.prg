>room_defun mirage_circle~
if pcnum($) >= 1
	if hour($) == 18
		echo [1;33m����F�@�A�Ӷ��w�w�q�F��ɰ_�C[m
	endif
	if hour($) == 7
		echo [33m�i����U�A�̫�@�D�������������C[m
	endif
	if hour($) == 8
		echo [1;30m�©]���{�A�]��Ţ�n�ۤj�a�C[m
	endif
	if hour($) == 9
		if day($) == 1
		or day($) == 3
		or day($) == 5
		or day($) == 8
		or day($) == 10
		or day($) == 12
		or day($) == 15
		or day($) == 17
		or day($) == 19
		or day($) == 22
		or day($) == 24
		or day($) == 26
		or day($) == 29
		or day($) == 31
		or day($) == 33
			echo [1;34m�Ŧ⪺��G�w�w�ɰ_�C[m
			echo [1;34m�|�P���������x�W�F�@�h�ť��C[m
		endif
		if day($) == 2
		or day($) == 4
		or day($) == 6
		or day($) == 9
		or day($) == 11
		or day($) == 13
		or day($) == 16
		or day($) == 18
		or day($) == 20
		or day($) == 23
		or day($) == 25
		or day($) == 27
		or day($) == 30
		or day($) == 32
		or day($) == 34
			echo [1;31m���⪺��G�w�w�ɰ_�C[m
			echo [1;31m�|�P���������x�W�F�@�h�����C[m
		endif
		if day($) == 7
		or day($) == 14
		or day($) == 21
		or day($) == 28
		or day($) == 35
			echo [1;35m����M�Ŧ⪺��G�ҽw�w�ɰ_�C[m
			echo [1;35m�|�P���������x�W�F�@�h�����C[m
		endif
	endif
endif
~
>room_defun mirage_weather~
if pcnum($) >= 1
	if hour($) == 12
	or hour($) == 13
	or hour($) == 14
	or hour($) == 15
	or hour($) == 16
	or hour($) == 9
	or hour($) == 10
	or hour($) == 11
		echo [1;30m�]�`�H�R�A�|�P�X�G�S���b�I�n�T�C[m
		if day($) == 1
		or day($) == 3
		or day($) == 5
		or day($) == 8
		or day($) == 10
		or day($) == 12
		or day($) == 15
		or day($) == 17
		or day($) == 19
		or day($) == 22
		or day($) == 24
		or day($) == 26
		or day($) == 29
		or day($) == 31
		or day($) == 33
			echo [1;34m�Ŧ⪺��G�����b�ѤW�C[m
			echo [1;34m�|�P���������x�W�F�@�h�ť��C[m
			echo [1;34m�@���Ŧ⪺�]�ߡC[m
		endif
		if day($) == 2
		or day($) == 4
		or day($) == 6
		or day($) == 9
		or day($) == 11
		or day($) == 13
		or day($) == 16
		or day($) == 18
		or day($) == 20
		or day($) == 23
		or day($) == 25
		or day($) == 27
		or day($) == 30
		or day($) == 32
		or day($) == 34
			echo [1;31m���⪺��G�����b�ѤW�C[m
			echo [1;31m�|�P���������x�W�F�@�h�����C[m
			echo [1;31m�@�Ӭ��⪺�]�ߡC[m
		endif
		if day($) == 7
		or day($) == 14
		or day($) == 21
		or day($) == 28
		or day($) == 35
			echo [1;35m����M�Ŧ⪺��G�����b�ѤW�C[m
			echo [1;35m�|�P���������x�W�F�@�h�����C[m
			echo [1;35m�@�ӵ��⪺�]�ߡC[m
		endif
	endif
	if hour($) == 18
	or hour($) == 19
	or hour($) == 20
	or hour($) == 21
	or hour($) == 22
	or hour($) == 23
		echo [1;36m�@�Ӳn�Ԫ�����A�L���}�}���j��A���y�e�W�C[m
	endif
	if hour($) == 1
	or hour($) == 2
	or hour($) == 3
	or hour($) == 4
	or hour($) == 5
		echo [1;36m�@�ӧ֬����U�ȡA�|�P���R���F���O�C[m
	endif
endif
~
|
