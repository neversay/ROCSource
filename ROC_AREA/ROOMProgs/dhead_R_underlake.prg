>room_enter 31 80~
    if race($n)==VAMPIRE
        echoat $n 你感受到一種來自湖水的負面力量，讓你的生命快速回復！
        c heal $n 40
        echoat $n 這份力量同時在侵蝕你的體力與魔力！
    else
        echoat $n 你感受到一種來自湖水的負面力量，逐漸侵蝕你的體力與魔力！
    endif
    modhmv $n 13.5 15 00010400
    modhmv $n 23.1 5  00000000
    del_affect $n waterbreath
    del_affect $n shield
    del_affect $n armor
    del_affect $n 'ston skin'
~
>room_leave 63 50~
    if pcnum($i)==0
        purge all
    endif
~
>room_time 100~
    echo 你感受到一種來自湖水的負面力量，逐漸侵蝕你的體力與魔力！
    modhmv pc 13.5 15 00010400
    modhmv pc 23.1 5  00000000
~
|