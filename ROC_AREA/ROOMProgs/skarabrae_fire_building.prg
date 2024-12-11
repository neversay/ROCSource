>room_enter 127 100~
if roomvalue($0) == 0
	break
endif
if roomvalue($0) <= 3
	echoat $n [1;36m³o¸Ì§e²{»´·L·l¶Ë¡C[0m
	break
endif
if roomvalue($0) <= 6
	echoat $n [1;32m³o¸Ì§e²{ÄY­«·l¶Ë¡A¦h³B©Ð«Î°_¤õ¡C[0m
	break
endif
if roomvalue($0) <= 9
	echoat $n [1;33m³o¸Ì§e²{«D±`ÄY­«ªº·l¶Ë¡A¥|©P³´¤J¤@¤ù¤õ®ü¡C[0m
	break
else
	echoat $n [1;31m³o¸Ì§e²{·´·Àª¬ºA¡A²¿®§ªº°ª·Å¹M§G¥|©P¡C[0m
	break
endif
~
|