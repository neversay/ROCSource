>room_defun holytown_fireseal_hunt~
if rand(60)
        if isaffected($r) / prot. from fire
                echo [1;36m四周的石壁突然竄出一道火苗，剛巧打在[0m $r [1;36m的火焰防壁上。[0m
                modhmv $r 01.50 80 00011401
        else
                if rand(30)
                        echo [1;36m四周的石壁突然竄出一道火苗，剛巧打在[0m $r [1;36m的身上。[0m
                        echoat $r [1;31m因為沒有火焰防壁的保護，所以你傷的不輕。[0m
                        modhmv $r 03.40 50 00011601
                        echoaround $r [1;31m你看到[0m $r [1;31m似乎被火傷的不輕。心中不免慶幸並不是燒到你。[0m
                else
                        if rand(60)
                                c flamestrike $r 40
                        else
                                mload 24046
                        endif
                endif
        endif
else
        if rmobcount(24043) < 4
                echo [1;36m四周的石壁突然竄出一道火苗，不久後就變成了一團飛火朝著你身邊飛過來。[0m
                mload 24043
        else
                if rand(40)
                        mload 24046
                else
                        c fireball $r 40
                endif
        endif
endif
echo [1;35m四周灼熱的溫度，加上處於精神緊繃的狀態，讓你不禁汗流浹背，體力大量的流失。[0m
modhmv pc 23.20 30 00011600
~
|
