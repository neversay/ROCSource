>all_greet_prog 100~
    if is_qmark($n) == dhead_uruk_merchantquest 300001
      if is_qmark($i) == dhead_bulette_questready
        mpmod_qmark $n dhead_uruk_merchantquest v2 = 2
        mpdel_qmark $i dhead_bulette_questready
      endif
    endif
~
>fight_prog 25~
    if isaffected($i) & 1
        mpdis_magic $i blind
        wink
        echo $I���W�����تk�N���ĤF!!
    else
        if isaffected($i) & 268435456
        mpdis_magic $i paralyze
        grow
        echo $I���W���·��k�N���ĤF!!
        endif
    endif
~
>death_prog 100~
    mpdis_magic $i all
    if is_qmark($n) == dhead_uruk_merchantquest 300001
        if is_qmark($i) == dhead_bulette_questready
        mpdel_qmark $i dhead_bulette_questready
        mpmod_qmark $n dhead_uruk_merchantquest v2 = 9998
        mpgrewrd exp $n 1 1000
	       	if class($b) < 5
	                mpexp $b 1000
	        endif
        endif
        mpdel_qmark $i dhead_bulette_questready
    else
        if is_qmark($n) == dhead_uruk_merchantquest 300002
            mpmod_qmark $b dhead_uruk_merchantquest v2 = 9998
            if class($b) < 5
                mpexp $b 1000
            endif
        endif
    endif
    if groupwith(tiredliz-dhead) == $n
        mpat tiredliz-dhead mpadd_qmark tiredliz-dhead dhead_tiredliz_complete 1 0 0 0 1 0
    endif
    echo $n��$I��������X�@����O�O�����j�xŦ�C
    mpoload 50000 floor
~
|