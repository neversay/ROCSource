>room_enter 127 35~
if ispc($n)
        if dex($n) > 20
                echoat $n ���j���K�����§A������m���F�U�ӡI
                echoat $n �d���@�v���ڡA�A�̵۹L�H���t�׻P�����{�}�F�I
                echoaround $n �A�ݨ�$N�}�G�a�{�L�F���U�Ӫ����j�K���C
        else
                echoat $n ���j���K�����§A������m���F�U�ӡI
		if isaffected($n) / stone skin
			echoat $n �A�Ӥ��ΰ{�}�Q�{�ӥ��ۡA���n���W���ۥҳN���A�S�˪������Y���C
			echoaround $n �A�ݨ�$N�Q���U�Ӫ����j�K�����{�ӥ��ۡA���ݰ_�ӥu���I���ˡC
			modhmv $n 01.200 200 0
		else
                	echoat $n �A�Ӥ��ΰ{�}�A�Q�{�����ˡI
                	echoaround $n �A�ݨ�$N�Q���U�Ӫ����j�K�����{�ӥ��ۡA�u�G......
                	modhmv $n 01.800 1200 0
		endif
        endif
endif
~
>room_defun sealevil_room_5f~
if ispc($r)
        if dex($r) > 20
                echoat $r ���j���K�����§A������m���F�U�ӡI
                echoat $r �d���@�v���ڡA�A�̵۹L�H���t�׻P�����{�}�F�I
                echoaround $r �A�ݨ�$R�}�G�a�{�L�F���U�Ӫ����j�K���C
        else
                echoat $r ���j���K�����§A������m���F�U�ӡI
		if isaffected($r) / stone skin
			echoat $r �A�Ӥ��ΰ{�}�Q�{�ӥ��ۡA���n���W���ۥҳN���A�S�˪������Y���C
			echoaround $r �A�ݨ�$R�Q���U�Ӫ����j�K�����{�ӥ��ۡA���ݰ_�ӥu���I���ˡC
			modhmv $r 01.200 200 0
		else
                	echoat $r �A�Ӥ��ΰ{�}�A�Q�{�����ˡI
                	echoaround $r �A�ݨ�$R�Q���U�Ӫ����j�K�����{�ӥ��ۡA�u�G......
                	modhmv $r 01.800 1200 0
		endif
        endif
endif
~
|