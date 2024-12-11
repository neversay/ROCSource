>room_enter 127 100~
if ispc($n)
        if isaffected($n) / prot. from fire
                echoat $n [1;36m©¯¦n¦³µÛ¤õµK¨¾¾Àªº¨¾Å@¡A¤£µM³o¨`¼öªº°ª·Å¥i¯à·|ª½±µ­n¤F§Aªº¤p©R¡C[0m
                modhmv $n 03.5 10 00011401
        else
                echoat $n [1;31m¦]¬°¨S¦³¤õµK¨¾¾Àªº¨¾Å@¡A©Ò¥H¤~­è¨«¶i³o¸Ì¡A§A«K³Q¥|©Pªº°ª·Åµ¹¨`¶Ë¡C[0m
                echoaround $n [1;31m§A¬Ý¨ì[0m $n [1;31m­@¤£¦í¥|©Pªº°ª·Å¡A¦Ó³QÄY­«¨`¶Ë¡C[0m
                modhmv $n 03.30 40 00011601
                if rand(60)
                        c 'star fire' $n 40
                else
                        c 'angry fire storm' $n 40
                endif
        endif
endif
echoat $n [1;35m¥|©P¨`¼öªº·Å«×¡A¥[¤W³B©óºë¯«ºòÁ^ªºª¬ºA¡AÅý§A¤£¸T¦½¬y¯H­I¡AÅé¤O¤j¶qªº¬y¥¢¡C[0m
modhmv $n 23.30 40 00011600
modhmv $n 21.100 150 00011600
~
|
