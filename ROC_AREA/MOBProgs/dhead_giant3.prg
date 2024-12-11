>repop_prog 100~
    mpoload 50014
    mpoload 50015
    mpat 50000 wear club two
    mpat 50000 wear belt
~
>fight_prog 50~
    if isaffected($i) & 1
        mpdis_magic $i blindness
        wink
        mpecho $I身上的盲目法術失效了!!
    else
        wear club two
        grow
        skilldrv multihit
    endif
~
|