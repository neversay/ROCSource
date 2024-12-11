>room_rand 10~
    if rand(50)
        echo 一陣強大的風襲來，差點讓你站不住腳而跌落山谷。
    else
        echoat $r 一陣強大的風襲來，你被吹下去了！
        echoataround $r 一陣強大的風襲來，$r被吹下去了！
        transfer $r 50052
    endif
~
>room_enter 127 100~
    echoat $n 攀爬峭壁讓你消耗了大量的體力。
    modhmv $n 23.20 40 00000000
~
|