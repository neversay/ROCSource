>room_defun dhead_R_ankheg_escape~
    if isfight($n)
        echoat $n 你正忙著戰鬥，沒辦法脫身！！
    else
        echoat $n 你來到一條地道。
        transfer $n 50024
    endif
~
|