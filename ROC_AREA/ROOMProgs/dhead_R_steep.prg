>room_defun dhead_R_steel_fall~
    transfer $n 50063
    at $n force $n look
    callout 5 force 50063 rcall dhead_R_fallenkill $n
~
>room_defun dhead_R_steel_fallr~
    transfer $r 50063
    at $r force $r look
    callout 5 force 50063 rcall dhead_R_fallenkillr $r
~
>room_rand 20~
    if rand(50)
        echo �@�}�j�j����ŧ�ӡA�t�I���A������}�Ӷ^���s���C
    else
        echoat $r �@�}�j��ŧ�ӡA�A�Q�j�U�h�F�I
        echoataround $r �@�}�j��ŧ�ӡA$r�Q�j�U�h�F�I
        rcall dhead_R_steel_fallr $r
    endif
~
>room_leave 48 100~
    rcall dhead_R_steel_fall $n
~
>room_enter 127 100~
    echoat $n �k���k�����A���ӤF�j�q����O�C
    modhmv $n 23.10 20 00000000
~
|