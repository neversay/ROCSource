>room_defun dhead_R_air_fallenkill~
    if being($n)
        if indoor($n) == 50062
            echoat $n 瞬間的痛楚後，你失去了一切的知覺了...
            echoaround $n $n 死了！
            modhmv $n 03.150 200 00000000
            transfer $n 50000
        endif
    endif
~
>room_defun dhead_R_air_fallenkillr~
    if being($r)
        if indoor($r) == 50062
            echoat $r 瞬間的痛楚後，你失去了一切的知覺了...
            echoaround $r $r 死了！
            modhmv $r 03.150 200 00000000
            transfer $r 50000
        endif
    endif
~
|