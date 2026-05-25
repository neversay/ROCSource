>room_enter 127 100~
if roomvalue($0) == 0
    echo [1;32m四周的岩壁開始晃動了起來，使得你差點失去平衡而跌倒。[0m
    if rand(25)
        modrvt v0 = 1
    else
        if rand(33)
            modrvt v0 = 2
        else
            if rand(50)
                modrvt v0 = 3
            else
                modrvt v0 = 4
            endif
        endif
    endif
endif
~
|
