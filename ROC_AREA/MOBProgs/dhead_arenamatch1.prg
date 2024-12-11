>defun_prog dhead_areamatch1_surrender~
    if is_qmark(self) == dhead_arenamatch_beaffected
        say 雖然很不甘心...但是我仍然輸了。
    else
        say 我認輸了...
        say 第一場的勝利將屬於你。
        mpmod_qmark $n dhead_instructor_contestquest v1 + 5
    endif
~
>defun_prog dhead_areamatch1_endofcombat~
    if is_qmark(self) == dhead_arenamatch_beaffected
        mpmod_qmark $n dhead_instructor_contestquest v2 + 1
    endif
    mpechoat $n 全場觀眾一片歡呼聲下，你獲得了第一場的勝利。
    mpechoat $n 接著你立刻被帶到中場的休息室。
    mptransfer $n 50128
    mpat 50128 mpforce assistant-dhead mpcall dhead-assistant-cure1 $r
~
>fight_prog 100~
    if isaffected(self) | 268435457
        if is_qmark(self) != dhead_arenamatch_beaffected
            mpadd_qmark self dhead_arenamatch_beaffected -1 0 0 0 1 0
        endif
        if rand(25)
            say 卑鄙的傢伙...
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