>defun_prog dhead_instructor_quest1~
    mpadd_qmark $n dhead_instructor_contestquest -1 0 0 0 0 1
    mpadd_qmark self dhead_instructor_bewaiting 24 0 0 0 1 0
    mpgoto corecle
    mppurge corecle
    mpmload 50130
    mpgoto 50126
~
>defun_prog dhead_instructor_quest2~
    mpmod_qmark $n dhead_instructor_contestquest v0 r 32
    mpmod_qmark $n dhead_instructor_contestquest v0 | 64
    mpadd_qmark $n dhead_instructor_tickcount -1 0 0 0 1 0
    mpdef_rembch $n
    say 加油！你好自為之吧！
    mpat 50127 mppurge
    mpat 50128 mppurge
    mpat 50129 mppurge
    mptransfer $n 50128
    mpat $n mpechoat $n 你被帶進了競技場...
    mpat $n mpechoat $n 四周湧起觀眾們興奮的鼓譟與歡呼聲。
    mpat $n mpforce $n look
    mpat 50127 mpmload 50132
    mpat 50129 mpmload 50140
~
>defun_prog dhead_instructor_tickprog~
    if rembch($b)
        if being($b)
            if is_qmark($b) dhead_instructor_tickcount
                mpat $b mpmod_qmark $b dhead_instructor_tickcount v0 + 1
            else
                mpgoto 50126
                mpmload 50131
                mppurge self
            endif
        endif
    endif
~
>greet_prog 100~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        if is_qmark($n) & dhead_instructor_contestquest 100016
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpmod_qmark $n dhead_instructor_contestquest v0 r 16
            mpmod_qmark $n dhead_instructor_contestquest v0 | 32            
            mpcallout 1  clap $n
            mpcallout 3  say 看來你已經取得喀爾克的認可了。
            mpcallout 4  say 但是先別高興得太早。
            mpcallout 5  say 接下來的試煉，面臨的對手只會更強！
            mpcallout 6  hmm 
            mpcallout 8  say 先跟你提示一下試煉的內容。
            mpcallout 9  say 你必須前往競技場，打敗三位強力的對手。
            mpcallout 11 say 雖然只求勝負，但激烈的戰鬥往往會打到一方死亡為止。
            mpcallout 13 say 如果你覺得已經做好準備(ready)，跟我說一聲就可以開始了。
            mpcallout 14 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) & dhead_instructor_contestquest 100064
                mpechoat $n 你先前挑戰失敗了！
                mpechoat $n 跟$I說一聲(ready)就可以重來。
                mpmod_qmark $n dhead_instructor_contestquest v0 | 32
                mpmod_qmark $n dhead_instructor_contestquest v1 = 0
                mpmod_qmark $n dhead_instructor_contestquest v2 = 0
            endif
        endif
    endif
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        if race($n) == 1024
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say 呵呵...年輕人...
            mpcallout 2 say 你是來詢問戰士系試煉(contest)的事嗎? 
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        else
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 hmm $n
            mpcallout 2 say 如果沒有別的事，請離開這裡吧。
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog contest~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        mpcallout 1 say 咳...咳...
        mpcallout 2 say 參加試煉(contest)挑戰高手，是我們蜥蜴人的最高榮耀。
        mpcallout 3 say 如果能通過試練，不但是對自我實力的一種肯定，
        mpcallout 4 say 同時還可以獲得優厚的獎品。
        mpcallout 6 say 族裡年輕的戰士，終日鍛鍊自身的武藝。
        mpcallout 7 say 他們都以參加(apply)試煉為目標。
        mpcallout 8 mpdel_qmark self dhead_allmob_speaking
    endif
~
>talk_prog apply~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) !& dhead_allquest 100008
            if race($n) == 1024
                if is_qmark($n) != dhead_instructor_contestquest
                    if level($n) >= 36
                        if is_qmark(self) == dhead_instructor_bewaiting
                            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                            mpcallout 1 say 抱歉，剛剛有人報名參加了
                            mpcallout 2 say 請等晚一點再來。
                            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                        else
                            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                            mpcallout 1 echo $I仔細地打量著$n。
                            mpcallout 3 tsk $n
                            mpcallout 4 say 如果你要參加試煉...那得先取得資格才行。
                            mpcallout 5 say 去找喀爾克(corecle)吧！
                            mpcallout 6 say 我的好朋友會告訴你該怎麼做的。
                            mpcallout 7 wave $n
                            mpcallout 8 mpcall dhead_instructor_quest1 $n
                            mpdel_qmark self dhead_allmob_speaking
                        endif
                    else
                        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                        mpcallout 1 echo $I仔細地打量著$n。
                        mpcallout 3 shake $n
                        mpcallout 4 say 也許你的膽識與勇氣過人，但歷練上畢竟還是不夠。
                        mpcallout 5 say 為了安全上的考量，我不能放行。
                        mpcallout 6 say 等你獲得更多實戰經驗後再來吧。
                        mpcallout 7 addoil $n
                        mpcallout 8 mpdel_qmark self dhead_allmob_speaking
                    endif
                else
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 ? $n
                    mpcallout 2 say 你不是已經報名參加嗎？
                    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                endif
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 say 抱歉，本試煉只有蜥蜴人的族人可以參加。
                mpcallout 2 say 異族人請回去吧。
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say 你不是已經通過試煉了嗎？
            mpcallout 2 giggle
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog ready~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        if pcnum($50127) > 0
          or pcnum($50128) > 0
          or pcnum($50129) > 0
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 shake $n
            mpcallout 2 say 目前有人正在進行試煉。
            mpcallout 3 say 請稍後再來。
            mpcallout 4 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) & dhead_instructor_contestquest 100032
                mpcall dhead_instructor_quest2 $n
            else
                slap $n
            endif
        endif
    endif
~
|