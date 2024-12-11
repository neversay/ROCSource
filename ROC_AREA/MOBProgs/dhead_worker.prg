>defun_prog dhead_worker_tickprog~
    if nplayer($) > 0
        if hour($) >= 18 or hour($) < 6
            if objcount(50009) == 0
                mpoload 50009 floor
                mpecho $I升起了一道營火
            endif
        else
            if objcount(50009) != 0
                mppurge all
                mpecho $I弄熄了營火，並清掉了一些雜物。
            else
                mppurge all
                mpecho $I弄熄了營火，並清掉了一些雜物。
            endif
        endif
    endif
~
|