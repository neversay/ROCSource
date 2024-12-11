>room_mask scan default~
		echoat $n ¸Þ²§ªº·¥«×¶Â·t¾B½ª¤F§Aªº·j¯Á¡I
~
>room_enter 127 33~
		if isaffected($n) == bless
				echoat $n [0m[30;1m¤j¶qªº¨¸®ð±N§A¨­¤Wªº¯¬ºÖ¥Ë¸Ñ¤F¡I[0m
				del_affect $n 'bless'
		else
				if isaffected($n) == curse
				else
						echoat $n [0m[30;1m§A¤ß¤¤¬ðµM°{¹L¤@µ·¤£¦w¡A§Ï©»¦³¬YºØªF¦è¦b§A¨­®Ç......[0m
				endif
				del_affect $n 'curse'
				add_affect $n 'curse' 1130 3 1 -3 0
				add_affect $n 'curse' 1130 3 2 -3 0
				add_affect $n 'curse' 1130 3 3 -3 0
				add_affect $n 'curse' 1130 3 4 -3 0
				add_affect $n 'curse' 1130 3 5 -3 0
				add_affect $n 'curse' 1130 3 35 10 0
				add_affect $n 'curse' 1130 3 24 10 0
		endif
~
|