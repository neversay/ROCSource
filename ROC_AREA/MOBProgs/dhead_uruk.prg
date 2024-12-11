>defun_prog dhead_uruk_questreward~
    if class($n) <5
        mpexp $n 2000
    endif
    mpexp    $n 1000
    mpgold   $n 100000
    mpechoat $n $I給了你一個袋子，裡頭有10萬枚金幣。
    mpechoaround $n $I給了 $n 一袋金幣。
~
>defun_prog dhead_uruk_questcomplete~
    mpdel_qmark $n dhead_uruk_merchantquest
    if is_qmark($n) != dhead_allquest
        mpadd_qmark $n dhead_allquest -1 0 0 0 0 1
    endif    
    mpmod_qmark $n dhead_allquest v0 | 1
~
>repop_prog 100~
    mpoload 50010 20
    mpoload 50012 20
    mpat 50000 wield scimitar
    mpat 50000 wear khaki
~
>talk_prog topic~
    if is_qmark(self) != dhead_allmob_speaking
        if is_qmark(self) &! dhead_allquest 100001
            if is_qmark(self) != dhead_uruk_merchantquest
                mpadd_qmark $n dhead_uruk_merchantquest -1 0 0 0 0 1
            endif
            if is_qmark($n) == dhead_uruk_merchantquest 100000
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 gasp $n
                mpcallout 2 say 喔喔喔...
                mpcallout 3 say 站在烏魯克(uruk)面前的是冒險者嘛? (yes/no)
                mpcallout 4 mpmod_qmark $n dhead_uruk_merchantquest v0 = 1
                mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>talk_prog no~
    if is_qmark(self) != dhead_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        if is_qmark($n) == dhead_uruk_merchantquest 100001
            mpdel_qmark self dhead_allmob_speaking
            mpcallout 1 -
            mpcallout 2 wave $n
            mpcallout 3 sigh
            mpcallout 4 mpdel_qmark self dhead_allmob_speaking
        else
            mpdel_qmark self dhead_allmob_speaking
            mpcallout 1 - $n
            mpcallout 2 curse
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog yes~
    if is_qmark(self) != dhead_allmob_speaking
        if level($n) < 31
            mpadd_qmark $i dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 ponder
            mpcallout 3 shake $n
            mpcallout 4 say 太年輕的冒險者...還是不行的。
            mpcallout 5 mpdel_qmark self dhead_allmob_speaking
        else 
            if is_qmark($n) == dhead_uruk_merchantquest 100001
                mpadd_qmark $i dhead_allmob_speaking 1 0 0 0 1 0
                mpmod_qmark $n dhead_uruk_merchantquest v0 = 2
        	mpcallout 1 dance
        	mpcallout 2 nuzzle $n
        	mpcallout 4 say 烏魯克(uruk)有個工作需要冒險者幫忙呀。
        	mpcallout 5 moan
        	mpcallout 7 去找珍(jean)...就說烏魯克(uruk)的請託。
        	mpcallout 8 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark $i dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 ?
                mpcallout 2 dunno $n
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>talk_prog restart~
    if is_qmark(self) != dhead_allmob_speaking
        if is_qmark($n) & dhead_allquest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 shake $n
            mpcallout 2 tell $n 任務已經不能重來了。
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        else
            mpdel_qmark $n dhead_uruk_merchantquest
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 forget
            mpcallout 2 tell $n 耶...烏魯克全部都忘了...任務可以重來囉。
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>give_prog 50000~
    if is_qmark(self) != dhead_allmob_speaking
        if is_qmark($n) == dhead_uruk_merchantquest 300099
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 gasp $n
            mpcallout 2 sob
            mpcallout 5 say 烏魯克真是太高興了。
            mpcallout 6 mpcall dhead_uruk_questreward $n 100
            mpcallout 6 mpcall dhead_uruk_questcomplete $n 100
            mpcallout 7 wave $n
            mpcallout 8 mpdel_qmark self dhead_allmob_speaking
        endif
    else
        drop all
    endif
~
>give_prog all~
    drop all
~
|