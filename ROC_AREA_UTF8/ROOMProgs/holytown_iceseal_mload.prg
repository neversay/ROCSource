>room_defun holytown_iceseal_mload~
if pcnum($) > 0
    if pcnum($) < 6
        if rmobcount(24044) < 4
            echo [1;37m你看到寒冰守衛的身影慢慢地從風雪中顯現出來。[0m
            mload 24044
        endif
        break
    else
        if pcnum($) < 10
            if rmobcount(24044) < 5
                echo [1;37m你看到寒冰守衛的身影慢慢地從風雪中顯現出來。[0m
                mload 24044
            endif
            break
        else
            if pcnum($) < 15
                if rmobcount(24044) < 6
                    echo [1;37m你看到寒冰守衛的身影慢慢地從風雪中顯現出來。[0m
                    mload 24044
                endif
                break
            else
                if pcnum($) < 20
                    if rmobcount(24044) < 7
                        echo [1;37m你看到寒冰守衛的身影慢慢地從風雪中顯現出來。[0m
                        mload 24044
                    endif
                else
                    if rmobcount(24044) < 8
                        echo [1;37m你看到寒冰守衛的身影慢慢地從風雪中顯現出來。[0m
                        mload 24044
                    endif
                endif
            endif
        endif
    endif
endif
~
|
