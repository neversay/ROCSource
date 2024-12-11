>room_mask west default~
if rmobcount(24042) > 0
    rcall holytown_rockseal_soldier4 $n
else
    if isfight($n)
        echoat $n 在戰鬥中你要怎麼移動？先想法子停下您的戰鬥吧！
    else
        if hitprcnt($n) < 50
        or hpnum($n) < 600
            echoat $n 你的健康狀態已經糟到無法讓你順利的穿越這岩石迷陣了。
        else
            if roomvalue($0) == 4
                if dex($n) > 23
                    if rand(60)
                        rcall holytown_rockseal_pass_west $n
                    else
                        rcall holytown_rockseal_move_west_miss $n
                    endif
                else
                    if dex($n) > 20
                        if rand(45)
                            rcall holytown_rockseal_pass_west $n
                        else
                            rcall holytown_rockseal_move_west_miss $n
                        endif
                    else
                        if rand(30)
                            rcall holytown_rockseal_pass_west $n
                        else
                            rcall holytown_rockseal_move_west_miss $n
                        endif
                    endif
                endif
            else
                rcall holytown_rockseal_wrongmove4 $n
            endif
        endif
    endif
endif
~
|
