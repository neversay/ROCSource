>defun_prog dhead_worker_tickprog~
    if nplayer($) > 0
        if hour($) >= 18 or hour($) < 6
            if objcount(50009) == 0
                mpoload 50009 floor
                mpecho $I�ɰ_�F�@�D���
            endif
        else
            if objcount(50009) != 0
                mppurge all
                mpecho $I�˺��F����A�òM���F�@�������C
            else
                mppurge all
                mpecho $I�˺��F����A�òM���F�@�������C
            endif
        endif
    endif
~
|