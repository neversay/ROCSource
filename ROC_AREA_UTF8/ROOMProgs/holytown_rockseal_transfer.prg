>room_defun holytown_rockseal_transfer~
if inroom($n) == 24113
    transfer $n 24114
else
    if inroom($n) == 24114
        transfer $n 24115
    else
        if inroom($n) == 24115
            transfer $n 24116
        else
            if inroom($n) == 24116
                transfer $n 24117
            else
                if inroom($n) == 24117
                    transfer $n 24118
                endif
            endif
        endif
    endif
endif
force $n look
~
|