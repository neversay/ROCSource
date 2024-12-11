>room_defun holytown_iceseal_hunt~
if rand(30)
    if isaffected($r) / prot. from cold
        c 'chill touch' $r 60
    else
        echo [1;37m¨ë°©ªº´H­·¤S¦A¦¸ªº§j°_¡AÅý§A§Ô¤£¦íªº±N¨­ÅéÁY¦¨¤@¹Î¡C[0m
        echoat $r [1;36m¦B§Nªº´H­·­áªº§A¥þ¨­³£³Â·ô¤F¡C[0m
        echoaround $r [1;36m§A¬Ý¨ì[0m $r [1;36m³Q¨º¦B§Nªº´H­·µ¹­á¨ì¥þ¨­³Â·ô¤F¡C[0m
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
