>defun_prog dhead-assistant-cure1~
    mpadd_qmark self dhead_assistant_tickcount 3 0 0 0 1 0
    mpadd_qmark self dhead_assistant_turncount -1 0 0 0 1 0
    mpdef_rembch $n
    mpcallout 1 say 恭喜你獲得第一場勝利。
    mpcallout 2 say 在第二場比賽開始前，你有四個小時的時間可以休息。
    mpcallout 3 say 我會為你施一些回復法術
~
>defun_prog dhead-assistant-cure2~
    mpadd_qmark self dhead_assistant_tickcount 3 0 0 0 1 0
    mpmod_qmark self dhead_assistant_turncount v0 = 1
    mpcallout 1 say 恭喜你獲得第一場勝利。
    mpcallout 2 say 在第二場比賽開始前，你有四個小時的時間可以休息。
    mpcallout 3 say 我會為你施一些回復法術
~
>defun_prog dhead-assistant-cong~
    mpmod_qmark self dhead_assistant_turncount v0 = 2
    say 恭喜你獲得最後的勝利。
    mpcall dhead-assistant-reward $n
~
>defun_prog dhead_assistant-questcomplete~
    mpexp $n 5000
    mpdel_qmark $n dhead_instructor_contestquest
    mpdel_qmark $n dhead_instructor_tickcount
    if is_qmark($n) != dhead_allquest
        mpadd_qmark $n dhead_allquest -1 0 0 0 0 1
    endif
    mpmod_qmark $n dhead_allquest v0 | 8
    yell 我宣佈 $n 通過蜥蜴人戰士試煉！
    mplog $n pass the lizardman quest
    mpgoto corecle-dhead mppurge corecle-dhead
    mpat 50125 mpmload 50130
    mpgoto instructor-dhead mppurge instructor-dhead
    mpat 50126 mpmload 50131
    mpgoto $b
    mpechoat $b 接著，你被送出了競技場。
    mptransfer $b 50126
    mppurge self
~
>defun_prog dhead_assistant-questreward~
    if is_qmark($n) & dhead_instructor_contestquest 100002
        mpmod_qmark $n dhead_instructor_contestquest v2 + 1
    else
        if is_qmark($n) & dhead_instructor_contestquest 100004
            mpmod_qmark $n dhead_instructor_contestquest v1 + 3
        else
            if is_qmark($n) & dhead_instructor_contestquest 100008
                mpmod_qmark $n dhead_instructor_contestquest v1 + 8
            endif
        endif
    endif
    if is_qmark($n) == dhead_instructor_contestquest 200004
        mpcallout 1 say 雖然你的作戰方式並不符合戰士的驕傲，
        mpcallout 2 say 但能堅持、貫徹自我的原則，仍舊是令人欽佩的。
        mpcallout 3 say 為求勝利不擇手段...這是你應得的獎勵。
        mpcallout 4 mpoload 50100 37 $n
        mpcallout 4 mpechoat $n $I將一件暗綠色的披肩交給了你。
        mpcallout 4 mpcall dhead_assistant-questcomplete $n
    else
        if is_qmark($n) > dhead_instructor_contestquest 100012
            mpcallout 1 say 刀光劍影的危機下，尚有餘力顧及到彼此的生命。
            mpcallout 2 say 終以仁慈之心征服對手的頑抗，獲得最後的勝利。
            mpcallout 3 say 啊～高貴的戰士，這是你應得的獎勵。
            mpcallout 4 mpoload 50101 37 $n
            mpcallout 4 mpechoat $n $I將一件亮白色的甲殼交給了你。
            mpcallout 4 mpcall dhead_assistant-questcomplete $n
        else
            if is_qmark($n) < dhead_instructor_tickprog 100050
                mpcallout 1 say 你的勇氣與武技無與倫比，
                mpcallout 2 say 每每一出手，勝利的號角就迅速地為你響起。
                mpcallout 3 say 啊～勇猛的戰士，這是你應得的獎勵。
                mpcallout 4 mpoload 50102 37 $n
                mpcallout 4 mpechoat $n $I將一面深紅色的鱗盾交給了你。
                mpcallout 4 mpcall dhead_assistant-questcomplete $n
            else
                if is_qmark($n) > dhead_instructor_contestquest 200000
                    mpcallout 1 say 雖然你曾經做出有違戰士精神的舉動，
                    mpcallout 2 say 但那份嘗試挑戰的勇氣仍然十分可取。
                    mpcallout 3 say 無論如何，這是你應得的獎勵。
                    mpcallout 4 mpoload 50103 37 $n
                    mpcallout 4 mpechoat $n $I將交給了你。
                    mpcallout 4 mpcall dhead_assistant-questcomplete $n
                else
                    mpcallout 1 say 你的表現相當地傑出。
                    mpcallout 2 say 尤其那份嘗試挑戰的勇氣仍然十分可取。
                    mpcallout 3 say 這是你應得的獎勵。
                    mpcallout 4 mpoload 50104 37 $n
                    mpcallout 4 mpechoat $n $I將交給了你。
                    mpcallout 4 mpcall dhead_assistant-questcomplete $n
                endif
            endif
        endif
    endif
~
>defun_prog dhead_assistant_randprog~
    if is_qmark(self) == dhead_assistant_tickcount
        mpadd_qmark self dhead_assistant_casting -1 0 0 0 1 0
        if rand(20)
            c 'heal' $r
        else
            if rand(25)
                c 'refresh' $r
            else
                if rand(33)
                    c bless $r
                else
                    if rand(50)
                        c 'cure critical' $r
                    else
                        c 'giant strength' $r
                    endif
                endif
            endif
        endif
        mpdel_qmark self dhead_assistant_casting
    endif
~
>defun_prog dhead_assistant_tickprog~
    if pcnum($) > 0
        if is_qmark(self) == dhead_assistant_tickcount
            break
        else
            if rembch($b)
                if being($b)
                    if is_qmark(self) == dhead_assistant_tickout
                        mpdel_qmark self dhead_assistant_tickout
                        mpmod_qmark self dhead_assistant_turncount v0 + 1
                        mpechoat $b 你聽到戰鬥即將開始的鈴聲。
                        mpechoat $b 接著就被帶到競技場上了...
                        mptransfer $b 50128
                        mpgoto 50129
                        if is_qmark(self) == dhead_assistant_turncount 100001
                            mpmload 50141
                        else
                            mpmload 50142
                        endif
                        mpgoto 50127
                    else
                        mpadd_qmark self dhead_assistant_tickout -1 0 0 0 1 0
                        wake $b
                        say 下一場戰鬥即將開始，請做好準備。
                    endif
                endif
            endif
        endif
    endif
~
|