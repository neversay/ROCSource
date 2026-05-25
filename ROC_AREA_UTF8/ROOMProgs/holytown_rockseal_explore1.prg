>room_command explore north~
if isfight($n)
    echoat $n [1;36m你正在忙著戰鬥呢！那來的時間探索呀！[0m
else
echoat $n 你開始探索著北方的石壁。
    if wis($n) > 23
        callout 1 rcall holytown_rockseal_check_ending1 $n
    else
        callout 1 echoat $n [1;36m在你仔細探索之後，還是不曉得北方的石壁究竟有沒有異狀。[0m
        callout 1 modhmv $n 33.5 10 0
    endif
endif
~
|
