>defun_prog dhead_corecle_attack~
    if isfight(self)
        break
    else
        say 既然你放棄先攻的機會，那我就不客氣了！
        mpkill $n
    endif
~
>defun_prog dhead_corecle_beaffected~
    if is_qmark(self) !& dhead_corecle_questing 100001
        mpmod_qmark self dhead_corecle_questing v0 | 1
    endif
    if rand(20)
        say 卑鄙的傢伙...
    endif
~
>repop_prog 100~
    mpadd_qmark self delayed_cast -1 9999 0 0 1 0
    mpoload 50090
    mpoload 50091
    mpoload 50092
    mpoload 50093
    mpoload 50094
    mpoload 50095
    mpat 50000 wear all
~
>kill_prog 100~
    say 果然還是太嫩...
    if is_qmark($n) == dhead_instructor_contestquest 100001
        mpat $n mpmod_qmark $n dhead_instructor_contestquest v0 = 0
    endif
    mppurge self
~
>fight_prog 100~
    if is_qmark(self) == dhead_corecle_questing
        if is_qmark($n) != dhead_instructor_contestquest 100001
            if is_qmark(self) !& dhead_corecle_questing 100002
                mpmod_qmark self dhead_corecle_questing v0 | 2
            endif
            break
        else
            if isaffected(self) & 268435457
                mpcallout dhead_corecle_beaffected $n
                break
            else
                if rand(33)
                    say 看我的...快速劍
                    skilldrv multihit
                    mpnowait
                else
                    if rand(50)
                        say 有破綻！
                        disarm
                        mpnowait
                    endif
                endif
            endif
        endif
    endif
~
>hitprcnt_prog 20~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark(self) == dhead_corecle_questing 100000
            if is_qmark($n) == dhead_instructor_contestquest 100001
                if rand(75)
                    mpgoto $n
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 say 夠了，我投降了！
                    mpcallout 2 clap $n
                    mpcallout 4 say 你的表現相當優異，懂得有效攻擊而不至於殺害對方。
                    mpcallout 5 say 你已經取得資格了，回競技場參加試煉吧！
                    mpcallout 6 addoil $n
                    mpcallout 6 mpmod_qmark $n dhead_instructor_contestquest v0 | 24
                    mpcallout 7 mppurge self
                 endif
            endif
        endif
    endif
~
>death_prog 100~
    if is_qmark(self) == dhead_corecle_questing
        if is_qmark($n) == dhead_instructor_contestquest 100001
            mpdis_magic self all
            if is_qmark(self) == dhead_corecle_questing 100000
                say 出手真狠...你已經取得資格了！
                mpmod_qmark $n dhead_instructor_contestquest v0 | 20
                break
            else
                say 雖然用了很不光榮的手段...但你還是取得資格了！
                mpmod_qmark $n dhead_instructor_contestquest v0 | 18
            endif
        endif
    endif
~
>greet_prog 100~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark(self) == dhead_corecle_questing
            mpkill $n
            break
        else
            if is_qmark($n) == dhead_instructor_contestquest 100000
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1  hmm
                mpcallout 2  say 想要參加試煉的新人就是你嗎？
                mpcallout 3  mpecho $I仔細地打量著$n。
                mpcallout 5  tsk $n
                mpcallout 6  mpecho $I小聲地說著：不錯不錯...從外表來看莫測高深。
                mpcallout 8  say 咳...咳...
                mpcallout 9  say 聽好...你要先取得參加試煉的資格。
                mpcallout 10 say 說穿了就是跟我打一架，證明你的實力。
                mpcallout 11 hmm $n
                mpcallout 12 say 注意了...我會在對戰中對你的表現做出評量。
                mpcallout 13 say 這也會列入將來試煉的評分之中喔。
                mpcallout 14 say 如果你覺得準備好了，就跟我說聲(ready)。
                mpcallout 14 mpmod_qmark $n dhead_instructor_contestquest v0 = 1
                mpcallout 15 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>talk_prog ready~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_instructor_contestquest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpdis_magic self all
            mpat 50000 c 'full heal'
            mpat 50000 c 'detect invis'
            mpat 50000 c 'detect hidden'
            mpcallout 1 say 好，讓我們開始吧。
            mpcallout 2 say 你可以先出手(kill corecle)。
            mpcallout 2 mpadd_qmark self dhead_corecle_questing -1 0 0 0 1 0
            mpcallout 2 mpmod_qmark $n dhead_instructor_contestquest v0 = 2
            mpcallout 4 mpcall dhead_corecle_attack $n
            mpcallout 5 mpdel_qmark self dhead_allmob_speaking
         endif
    endif
~
|