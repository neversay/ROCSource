>defun_prog dhead_areamatch3_endofcombat~
    if is_qmark(self) == dhead_arenamatch_beaffected
        mpmod_qmark $n dhead_instructor_contestquest v2 + 1
    endif
    mpechoat $n 全場觀眾一片歡呼聲下，你獲得了最後的勝利。
    mpechoat $n 接著你立刻被帶到中場的休息室。
    mptransfer $n 50128
    mpat 50128 mpforce assistant-dhead mpcall dhead-assistant-cong $r
~
>fight_prog 100~
    if isaffected(self) | 268435457
        if is_qmark(self) != dhead_arenamatch_beaffected
            mpadd_qmark self dhead_arenamatch_beaffected -1 0 0 0 1 0
        endif
        if rand(75)
            say 卑鄙的傢伙！
            combat howling
            mpdis_magic self blindness
            mpdis_magic self paralyze
        endif
    else
        if rand(33)
            chuckle
            combat howling
            mpnowait
            circular
            mpnowait
        else
            if rand(50)
                pk $n
                frenzy
            else
                domo
                skilldrv multihit
            endif
        endif
    endif
~
>death_prog 100~
    mpdis_magic self all
    mpcall dhead_areamatch1_endofcombat $n
    mppurge self
~
|