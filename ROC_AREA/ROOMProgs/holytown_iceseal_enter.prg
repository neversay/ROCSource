>room_enter 127 100~
if ispc($n)
    if rand(60)
        if rand(40)
            if isaffected($n) / prot. from cold
                c 'chill touch' $n 60
            else
                echoat $n [1;36m¦B§Nªº´H­·­áªº§A¥þ¨­³£³Â·ô¤F¡C[0m
                echoaround $n [1;36m§A¬Ý¨ì[0m $n [1;36m³Q¨º¦B§Nªº´H­·µ¹­á¨ì¥þ¨­³Â·ô¤F¡C[0m
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
        echoat $n [1;37mªï­±¦Ó¨Óªº´H­·Åý§A¨­Åé¤£Â_ªºÅ¸§ÝµÛ¡A¦ÓÅé¤O¤]¥¿§Ö³tªº¤U­°¤¤¡C[0m
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
        at 24125 echo [1;36mµ²¬É¦]¬°µLªk©Ó¨ü­t²ü¡A¶}©l²£¥Í¼@¯Pªº·n®Ì¡C[0m
        at 24125 echo [1;36m¦b¼@¯Pªº·n®Ì¤§»Ú¡A´H¦B¦u½Ã¦A¦¸¦a±q­·³·¤¤Åã§Î¦Ó¥X¡C[0m
        at 24125 mload 24044
        at 24125 mload 24044
        at 24126 echo [1;36mµ²¬É¦]¬°µLªk©Ó¨ü­t²ü¡A¶}©l²£¥Í¼@¯Pªº·n®Ì¡C[0m
        at 24126 echo [1;36m¦b¼@¯Pªº·n®Ì¤§»Ú¡A´H¦B¦u½Ã¦A¦¸¦a±q­·³·¤¤Åã§Î¦Ó¥X¡C[0m
        at 24126 mload 24044
        at 24126 mload 24044
        at 24127 echo [1;36mµ²¬É¦]¬°µLªk©Ó¨ü­t²ü¡A¶}©l²£¥Í¼@¯Pªº·n®Ì¡C[0m
        at 24127 echo [1;36m¦b¼@¯Pªº·n®Ì¤§»Ú¡A´H¦B¦u½Ã¦A¦¸¦a±q­·³·¤¤Åã§Î¦Ó¥X¡C[0m
        at 24127 mload 24044
        at 24127 mload 24044
        at 24128 echo [1;36mµ²¬É¦]¬°µLªk©Ó¨ü­t²ü¡A¶}©l²£¥Í¼@¯Pªº·n®Ì¡C[0m
        at 24128 echo [1;36m¦b¼@¯Pªº·n®Ì¤§»Ú¡A´H¦B¦u½Ã¦A¦¸¦a±q­·³·¤¤Åã§Î¦Ó¥X¡C[0m
        at 24128 mload 24044
        at 24128 mload 24044
        at 24129 echo [1;36mµ²¬É¦]¬°µLªk©Ó¨ü­t²ü¡A¶}©l²£¥Í¼@¯Pªº·n®Ì¡C[0m
        at 24129 echo [1;36m¦b¼@¯Pªº·n®Ì¤§»Ú¡A´H¦B¦u½Ã¦A¦¸¦a±q­·³·¤¤Åã§Î¦Ó¥X¡C[0m
        at 24129 mload 24044
        at 24129 mload 24044
        at 24130 echo [1;36mµ²¬É¦]¬°µLªk©Ó¨ü­t²ü¡A¶}©l²£¥Í¼@¯Pªº·n®Ì¡C[0m
        at 24130 echo [1;36m¦b¼@¯Pªº·n®Ì¤§»Ú¡A´H¦B¦u½Ã¦A¦¸¦a±q­·³·¤¤Åã§Î¦Ó¥X¡C[0m
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
