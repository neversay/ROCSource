>room_enter 31 80~
    if race($n)==VAMPIRE
        echoat $n �A�P����@�بӦ۴�����t���O�q�A���A���ͩR�ֳt�^�_�I
        c heal $n 40
        echoat $n �o���O�q�P�ɦb�I�k�A����O�P�]�O�I
    else
        echoat $n �A�P����@�بӦ۴�����t���O�q�A�v���I�k�A����O�P�]�O�I
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
    echo �A�P����@�بӦ۴�����t���O�q�A�v���I�k�A����O�P�]�O�I
    modhmv pc 13.5 15 00010400
    modhmv pc 23.1 5  00000000
~
|