>defun_prog dhead_restguard_tickprog~
    if isfight(self)
        yell ���H��ŧ�u�áI
    else
        if is_qmark(self) != dhead_lizgurad_unequiped
            yawn
            mpecho $I���U�Ҧ��˳�...
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
        mpecho $I�����W�˳�...
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