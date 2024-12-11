>room_defun holytown_fireseal_hunt~
if rand(60)
        if isaffected($r) / prot. from fire
                echo [1;36m¥|©Pªº¥Û¾À¬ðµMÂ«¥X¤@¹D¤õ­]¡A­è¥©¥´¦b[0m $r [1;36mªº¤õµK¨¾¾À¤W¡C[0m
                modhmv $r 01.50 80 00011401
        else
                if rand(30)
                        echo [1;36m¥|©Pªº¥Û¾À¬ðµMÂ«¥X¤@¹D¤õ­]¡A­è¥©¥´¦b[0m $r [1;36mªº¨­¤W¡C[0m
                        echoat $r [1;31m¦]¬°¨S¦³¤õµK¨¾¾Àªº«OÅ@¡A©Ò¥H§A¶Ëªº¤£»´¡C[0m
                        modhmv $r 03.40 50 00011601
                        echoaround $r [1;31m§A¬Ý¨ì[0m $r [1;31m¦ü¥G³Q¤õ¶Ëªº¤£»´¡C¤ß¤¤¤£§K¼y©¯¨Ã¤£¬O¿N¨ì§A¡C[0m
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
                echo [1;36m¥|©Pªº¥Û¾À¬ðµMÂ«¥X¤@¹D¤õ­]¡A¤£¤[«á´NÅÜ¦¨¤F¤@¹Î­¸¤õ´ÂµÛ§A¨­Ãä­¸¹L¨Ó¡C[0m
                mload 24043
        else
                if rand(40)
                        mload 24046
                else
                        c fireball $r 40
                endif
        endif
endif
echo [1;35m¥|©P¨`¼öªº·Å«×¡A¥[¤W³B©óºë¯«ºòÁ^ªºª¬ºA¡AÅý§A¤£¸T¦½¬y¯H­I¡AÅé¤O¤j¶qªº¬y¥¢¡C[0m
modhmv pc 23.20 30 00011600
~
|
