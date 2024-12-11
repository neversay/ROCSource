>room_defun dhead_R_steel_fall~
    transfer $n 50063
    at $n force $n look
    callout 5 force 50063 rcall dhead_R_fallenkill $n
~
>room_defun dhead_R_steel_fallr~
    transfer $r 50063
    at $r force $r look
    callout 5 force 50063 rcall dhead_R_fallenkillr $r
~
>room_rand 20~
    if rand(50)
        echo 一陣強大的風襲來，差點讓你站不住腳而跌落山谷。
    else
        echoat $r 一陣大風襲來，你被吹下去了！
        echoataround $r 一陣大風襲來，$r被吹下去了！
        rcall dhead_R_steel_fallr $r
    endif
~
>room_leave 48 100~
    rcall dhead_R_steel_fall $n
~
>room_enter 127 100~
    echoat $n 攀爬峭壁讓你消耗了大量的體力。
    modhmv $n 23.10 20 00000000
~
|