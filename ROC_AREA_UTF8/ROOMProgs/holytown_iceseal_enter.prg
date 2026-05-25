>room_enter 127 100~
if ispc($n)
    if rand(60)
        if rand(40)
            if isaffected($n) / prot. from cold
                c 'chill touch' $n 60
            else
                echoat $n [1;36m冰冷的寒風凍的你全身都麻痺了。[0m
                echoaround $n [1;36m你看到[0m $n [1;36m被那冰冷的寒風給凍到全身麻痺了。[0m
                add_affect $n 'paralyze' 1135 2 39 100 268435456
            endif
        else
            if rand(60)
                c 'colour spray' $n 60
            else
                c 'chill touch' $n 60
            endif
        endif
    else
        echoat $n [1;37m迎面而來的寒風讓你身體不斷的顫抖著，而體力也正快速的下降中。[0m
        modhmv $n 23.20 30 00012300
        modhmv $n 03.5 10 00012300
    endif
endif
if roomvalue($n24130v1) != 1
    if pcnum($24125) > 0
    if pcnum($24126) > 0
    if pcnum($24127) > 0
    if pcnum($24128) > 0
    if pcnum($24129) > 0
    if pcnum($24130) > 0
        at 24125 echo [1;36m結界因為無法承受負荷，開始產生劇烈的搖晃。[0m
        at 24125 echo [1;36m在劇烈的搖晃之際，寒冰守衛再次地從風雪中顯形而出。[0m
        at 24125 mload 24044
        at 24125 mload 24044
        at 24126 echo [1;36m結界因為無法承受負荷，開始產生劇烈的搖晃。[0m
        at 24126 echo [1;36m在劇烈的搖晃之際，寒冰守衛再次地從風雪中顯形而出。[0m
        at 24126 mload 24044
        at 24126 mload 24044
        at 24127 echo [1;36m結界因為無法承受負荷，開始產生劇烈的搖晃。[0m
        at 24127 echo [1;36m在劇烈的搖晃之際，寒冰守衛再次地從風雪中顯形而出。[0m
        at 24127 mload 24044
        at 24127 mload 24044
        at 24128 echo [1;36m結界因為無法承受負荷，開始產生劇烈的搖晃。[0m
        at 24128 echo [1;36m在劇烈的搖晃之際，寒冰守衛再次地從風雪中顯形而出。[0m
        at 24128 mload 24044
        at 24128 mload 24044
        at 24129 echo [1;36m結界因為無法承受負荷，開始產生劇烈的搖晃。[0m
        at 24129 echo [1;36m在劇烈的搖晃之際，寒冰守衛再次地從風雪中顯形而出。[0m
        at 24129 mload 24044
        at 24129 mload 24044
        at 24130 echo [1;36m結界因為無法承受負荷，開始產生劇烈的搖晃。[0m
        at 24130 echo [1;36m在劇烈的搖晃之際，寒冰守衛再次地從風雪中顯形而出。[0m
        at 24130 mload 24044
        at 24130 mload 24044
        modrvt 24130 v1 = 1
        callout 1 rcall holytown_iceseal_door $n
    endif
    endif
    endif
    endif
    endif
    endif
endif
if roomvalue($n24130v1) != 1
    rcall holytown_iceseal_mload $n
endif
~
|
