>defun_prog dhead_areamatch2_surrender~
    if is_qmark(self) == dhead_arenamatch_beaffected
        say �i�c...�ڿ�F�C
    else
        say �i�q�����...�ڻ{��F...
        say �ĤG�����ӧQ���A�a�C
        mpmod_qmark $n dhead_instructor_contestquest v1 + 5
    endif
~
>defun_prog dhead_areamatch2_endofcombat~
    if is_qmark(self) == dhead_arenamatch_beaffected
        mpmod_qmark $n dhead_instructor_contestquest v2 + 1
    endif
    mpechoat $n �����[���@���w�I�n�U�A�A��o�F�ĤG�����ӧQ�C
    mpechoat $n ���ۧA�ߨ�Q�a�줤�����𮧫ǡC	
    mptransfer $n 50128
    mpat 50128 mpforce assistant-dhead mpcall dhead-assistant-cure2 $r
~
>fight_prog 100~
    if isaffected(self) | 268435457
        if is_qmark(self) != dhead_arenamatch_beaffected
            mpadd_qmark self dhead_arenamatch_beaffected -1 0 0 0 1 0
        endif
        if rand(25)
            say �������å�...
            mpdis_magic self blindness
        endif
    else
        if rand(33)
            chuckle
            disarm
            mpnowait
            disarm
            mpnowait
        else
            if rand(50)
                grin $n
                skilldrv multihit
            endif
        endif
    endif
~
>hitprcnt_prog 15~
    if rand(20)
        mpdis_magic self all
        mpcall dhead_areamatch1_surrender $n
        mpcall dhead_areamatch1_endofcombat $n
        mppurge self
    endif
~
>death_prog 100~
    mpdis_magic self all
    mpcall dhead_areamatch1_endofcombat $n
    mppurge self
~
|