>room_defun dhead_R_air_fallenkill~
    if being($n)
        if indoor($n) == 50062
            echoat $n �������h����A�A���h�F�@������ı�F...
            echoaround $n $n ���F�I
            modhmv $n 03.150 200 00000000
            transfer $n 50000
        endif
    endif
~
>room_defun dhead_R_air_fallenkillr~
    if being($r)
        if indoor($r) == 50062
            echoat $r �������h����A�A���h�F�@������ı�F...
            echoaround $r $r ���F�I
            modhmv $r 03.150 200 00000000
            transfer $r 50000
        endif
    endif
~
|