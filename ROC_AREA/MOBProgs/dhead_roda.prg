>repop_prog 100~
    mpgroup $i jean-dhead 1 0
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        mpcallout 1 say 有什麼需要，直接用list/buy 吧。
        mpcallout 2 say 放心，不會坑你錢的啦。
        mpcallout 3 snick
        mpcallout 4 mpdel_qmark self dhead_allmob_speaking
    endif
~
|