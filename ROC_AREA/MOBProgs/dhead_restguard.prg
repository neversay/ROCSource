>defun_prog dhead_restguard_tickprog~
    if isfight(self)
        yell 有人偷襲守衛！
    else
        if is_qmark(self) != dhead_lizgurad_unequiped
            yawn
            mpecho $I卸下所有裝備...
            mpat 50000 remove all
            mpjunk all.lizardman
            mpjunk broadsword
            mpadd_qmark self dhead_lizgurad_unequiped -1 0 0 0 1 0
        endif
        sleep
    endif
~
>repop_prog 100~
    mpgroup 2.guard-dhead self 0 0
    sleep
~
>fight_prog 80~
    if is_qmark(self) == dhead_lizgurad_unequiped
        mpecho $I趕緊穿上裝備...
        mpoload 50090
        mpoload 50091
        mpoload 50092
        mpoload 50093
        mpoload 50094
        mpoload 50095
        wear all
        mpdel_qmark self dhead_lizgurad_unequiped
    endif
~
|