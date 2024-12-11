>room_enter 127 100~
if roomtimer($0) == 0
        if is_inv_obj($n) == 32112
                if rand(20)
                        echoat $n §A¨­¤WªºÂêªù©Ç¤§Æ_µo¥X¤Fª÷¶Àªº¥ú¨~¡I
                        echoaround $n $n ¨­¤WªºªºÂêªù©Ç¤§Æ_µo¥X¤Fª÷¶Àªº¥ú¨~¡I
                        echo Æ_°Íªº¥ú¨~¬M·Ó¦b¦aªO¤W¥X²{¤F¤@¹Dªùªº§Îª¬¡I
                        echo ªù«¬ªº¥ú¨~¶}©l¥~¬ª¡A¤@­Óªùª¬¬}¤Õ¦¨«¬¡I
                        echoat $n §A¤â¤¤ªºÆ_°Íµo¥X°ª¼öÅý§A´¤¤£¦í¡I
                        force $n drop key-locked
                        echo Æ_°Í§Ö±¼¨ì¦a¤W«e´N®ø¥¢¦b¥ú¨~¤¤¤F!
                        purge key-locked
                        modrvt v1 = 1
                        modrvt t0 = 3
                else
                        modrvt t0 = 5
                endif
        endif
endif
~
>room_mask down default~
if roomvalue($1) == 1
        if pcnum($) == 1
          echoat $n ·í§A¬ï¹L¤Õ¬}¤§«á¡A¤Õ¬}¦b§A¨­«á®ø¥¢¤F¡I
          callout 2 at $n echoaround $n ·í $n ¬ï¹L¤Õ¬}«á¡A¤Õ¬}®ø¥¢¤F¡I
          transfer $n 32265
          modrvt v1 = 0
        else
          echoat $n §A¬ï¹Lµo«Gªº¤Õ¬}©¹¤U¨«¥h¡C
          echoaround $n $n ¬ï¹Lµo¥úªº¤Õ¬}©¹¤U¨«¥h¡I
          transfer $n 32265
        endif
else
        echoat $n [0;33m«u§r¡A¦¹¸ô¤£³q¡C[m
endif
~
|
