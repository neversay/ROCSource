>defun_prog dhead_areamatch1_surrender~
    if is_qmark(self) == dhead_arenamatch_beaffected
        say ���M�ܤ��̤�...���O�ڤ��M��F�C
    else
        say �ڻ{��F...
        say �Ĥ@�����ӧQ�N�ݩ�A�C
        mpmod_qmark $n dhead_instructor_contestquest v1 + 5
    endif
~
>defun_prog dhead_areamatch1_endofcombat~
    if is_qmark(self) == dhead_arenamatch_beaffected
        mpmod_qmark $n dhead_instructor_contestquest v2 + 1
    endif
    mpechoat $n �����[���@���w�I�n�U�A�A��o�F�Ĥ@�����ӧQ�C
    mpechoat $n ���ۧA�ߨ�Q�a�줤�����𮧫ǡC
    mptransfer $n 50128
    mpat 50128 mpforce assistant-dhead mpcall dhead-assistant-cure1 $r
~
>fight_prog 100~
    if isaffected(self) | 268435457
        if is_qmark(self) != dhead_arenamatch_beaffected
            mpadd_qmark self dhead_arenamatch_beaffected -1 0 0 0 1 0
        endif
        if rand(25)
            say �������å�...
        endif
    else
        if rand(33)
            cackle $n
            disarm
            mpnowait
            disarm
            mpnowait
        else
            if rand(50)
                domo
                bashhead
                mpnowait
                bashhead
                mpnowait
            endif
        endif
    endif
~
>hitprcnt_prog 10~
    if rand(40)
        mpdis_magic self all
        mpcall dhead_areamatch1_surrender $n
        mpcall dhead_areamatch1_endofcombat $n
        mppurge self
    endif
~
>hitprcnt_prog 20~
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