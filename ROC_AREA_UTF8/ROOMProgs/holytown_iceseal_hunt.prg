>room_defun holytown_iceseal_hunt~
if rand(30)
    if isaffected($r) / prot. from cold
        c 'chill touch' $r 60
    else
        echo [1;37m刺骨的寒風又再次的吹起，讓你忍不住的將身體縮成一團。[0m
        echoat $r [1;36m冰冷的寒風凍的你全身都麻痺了。[0m
        echoaround $r [1;36m你看到[0m $r [1;36m被那冰冷的寒風給凍到全身麻痺了。[0m
        add_affect $r 'paralyze' 1135 2 39 100 268435456
    endif
else
    if rand(70)
        c 'colour spray' $r 60
    else
        c 'glacier strike' $r 60
    endif
endif
~
|
