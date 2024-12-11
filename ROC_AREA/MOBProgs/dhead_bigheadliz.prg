>death_prog 100~
    if level($n) < 42
        if str($n) > 17
            mpechoat $n 你用隨身攜帶的小刀，將$I的皮割下來。
            mpechoaround $n 你看見 $n 用隨身攜帶的小刀，將$I的皮割下來。
            mpoload 50060 floor
        else
            if is_qmark($n) == dhead_fericle_winequest 100002
                mpechoat $n 你試著使用隨身攜帶的小刀，將$I的皮割下。
                mpechoat $n 可是$I的皮實在太硬了，以你的力量無法辦到。
                mpechoaround $n 你看見 $n 試著用隨身攜帶的小刀，將$I的皮割下來。
                mpechoaround $n 可是費了好大的功夫還是失敗了。
            endif
        endif
    endif
~
|