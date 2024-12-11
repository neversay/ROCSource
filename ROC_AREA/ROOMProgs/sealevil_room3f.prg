>room_enter 127 60~
if ispc($n)
        if rand(33)
                echoat $n ¤@°¦©ÇÂÎ©¿µM¦Û¦å´ò¤¤ÅD¥X¡A«r¤F§A¤@¤f«á¤S±¼¶i´ò¤¤¡C
                echoat $n §AÄ±±o³Q«r¨ìªº¦a¤è¦³ÂI»Ä³Â¡A¨­Åé¦n¹³¤]¤£¤ÓµÎªA¡C
                add_affect $n 'poison' 1140 10 19 -25 4096
        else
                if rand(50)
                        echoat $n §A¨­®Ç©¿µM¼Q¥X¤@ªÑ·t¬õ¦âªºÃú®ð¡I
                        echoat $n ¤~§l¶i¤@ÂI¡A§A´NÄ±±o«ÜÃø¨ü¡A¬Æ¦Ü·Q¦R¡C
                        add_affect $n 'disease' 1140 10 18 -25 1099511627776
                else
                        echoat $n §A©¿µM·P¨ì¨­Åé¬Y³B¶Ç¨Ó¤@°}³Âµh·P¡A¦n¹³³Q¤°»òªF¦è¨ë¨ì¡C
			if isaffected($n) / adrenaline control
				echoat $n [1;36m§AÅé¤ºªºµÇ¤W¸¢¯À¦bÀþ¶¡¿Eµo¦Ó¥X¡A¨Ï§A·P¨ì¥þ¨­¬Û·í¶¶ºZ¡C[0m
				if rand(15)
					echoat $n [1;37m¦ý¤]¦b¦¹®É¡A§A¨­¤WªºµÇ¸¢³N±±¨î³N¦ü¥G¤]®ø¥¢¤F¡C[0m
					del_affect $n 'adrenaline control'
				endif
			else
                        	echoat $n Àþ¶¡¡A³Â·ô·PÂX±i¨ì¤F¥þ¨­¡AÁV¿|¤F......
                        	add_affect $n 'paralyze' 0 5 39 200 268435456
			endif
                endif
        endif
endif
~
>room_defun sealevil_room_3f~
if ispc($r)
        if rand(33)
                echoat $r ¤@°¦©ÇÂÎ©¿µM¦Û¦å´ò¤¤ÅD¥X¡A«r¤F§A¤@¤f«á¤S±¼¶i´ò¤¤¡C
                echoat $r §AÄ±±o³Q«r¨ìªº¦a¤è¦³ÂI»Ä³Â¡A¨­Åé¦n¹³¤]¤£¤ÓµÎªA¡C
                add_affect $r 'poison' 1140 10 19 -25 4096
        else
                if rand(50)
                        echoat $r §A¨­®Ç©¿µM¼Q¥X¤@ªÑ·t¬õ¦âªºÃú®ð¡I
                        echoat $r ¤~§l¶i¤@ÂI¡A§A´NÄ±±o«ÜÃø¨ü¡A¬Æ¦Ü·Q¦R¡C
                        add_affect $r 'disease' 1140 10 18 -25 1099511627776
                else
                        echoat $r §A©¿µM·P¨ì¨­Åé¬Y³B¶Ç¨Ó¤@°}³Âµh·P¡A¦n¹³³Q¤°»òªF¦è¨ë¨ì¡C
			if isaffected($r) / adrenaline control
				echoat $r [1;36m§AÅé¤ºªºµÇ¤W¸¢¯À¦bÀþ¶¡¿Eµo¦Ó¥X¡A¨Ï§A·P¨ì¥þ¨­¬Û·í¶¶ºZ¡C[0m
				if rand(15)
					echoat $r [1;37m¦ý¤]¦b¦¹®É¡A§A¨­¤WªºµÇ¸¢³N±±¨î³N¦ü¥G¤]®ø¥¢¤F¡C[0m
					del_affect $r 'adrenaline control'
				endif
			else
                        	echoat $r Àþ¶¡¡A³Â·ô·PÂX±i¨ì¤F¥þ¨­¡AÁV¿|¤F......
                        	add_affect $r 'paralyze' 0 5 39 200 268435456
			endif
                endif
        endif
endif
~
|