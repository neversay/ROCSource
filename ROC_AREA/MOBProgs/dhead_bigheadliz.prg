>death_prog 100~
    if level($n) < 42
        if str($n) > 17
            mpechoat $n �A���H����a���p�M�A�N$I���ֳΤU�ӡC
            mpechoaround $n �A�ݨ� $n ���H����a���p�M�A�N$I���ֳΤU�ӡC
            mpoload 50060 floor
        else
            if is_qmark($n) == dhead_fericle_winequest 100002
                mpechoat $n �A�յۨϥ��H����a���p�M�A�N$I���ֳΤU�C
                mpechoat $n �i�O$I���ֹ�b�ӵw�F�A�H�A���O�q�L�k���C
                mpechoaround $n �A�ݨ� $n �յۥ��H����a���p�M�A�N$I���ֳΤU�ӡC
                mpechoaround $n �i�O�O�F�n�j���\���٬O���ѤF�C
            endif
        endif
    endif
~
|