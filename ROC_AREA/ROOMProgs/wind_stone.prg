>room_defun wind_stone~
echo ¥|©P±²°_¤@°}²r¯Pªº­·¼É¡I
if rand(10)
	rcall wind_catch
endif
if rand(30)
	rcall wind_catch
endif
if rand(50)
	rcall wind_catch
endif
~
>room_defun wind_catch~
if ispc($r)
	rcall wind_check $r
endif
~
>room_defun reset_roomvalue~
if rnplayer($) == 0
	if roomvalue($0) != 0
		modrvt v0 = 0
	endif
endif
~
>room_command step stone~
if isfight($n)
	echoat $n §AÁÙ¦b¾Ô°«¤¤¡A¨S¦³®É¶¡¥h½ñ¥Û¶ô¡I
else
	echoat $n ·í§A½ñ¤W¥Û¶ôªº®É¡A³o¸Ì¥ß§Y±²°_¤@°}±j­·¡I
	echoaround $n ·í$N½ñ¤W¥Û¶ôªº®É¡A³o¸Ì¥ß§Y±²°_¤@°}±j­·¡I
	rcall wind_check
	rcall change_roomvalue
endif
~
>room_defun wind_check~
if rand(10)
	echoat $r ²r¯Pªº­·¼ÉÅý§A [1;35m¦³¨ÇÀ¿¶Ë[m!(41-60)
	echoaround $r ²r¯Pªº­·¼ÉÅý$R [1;35m¦³¨ÇÀ¿¶Ë[m!
	modhmv $r 01.41 60 0
	break
endif
if rand(20)
	echoat $r ²r¯Pªº­·¼ÉÅý§A [1;35m¶}©l¯}¥Ö[m!(61-85)
	echoaround $r ²r¯Pªº­·¼ÉÅý$R [1;35m¶}©l¯}¥Ö[m!
	modhmv $r 01.61 85 0
	break
endif
if rand(30)
	echoat $r ²r¯Pªº­·¼ÉÅý§A [1;35m¯Q«C·ï¦å[m!(86-115)
	echoaround $r ²r¯Pªº­·¼ÉÅý$R [1;35m¯Q«C·ï¦å[m!
	modhmv $r 01.86 115 0
	break
endif
if rand(40)
	echoat $r ²r¯Pªº­·¼ÉÅý§A [1;35m* ¯Q«C·ï¦å *[m!(116-150)
	echoaround $r ²r¯Pªº­·¼ÉÅý$R [1;35m* ¯Q«C·ï¦å *[m!
	modhmv $r 01.116 150 0
	break
endif
if rand(50)
	echoat $r ²r¯Pªº­·¼ÉÅý§A [1;32m* ¯kµh¤£³ô *[m!(151-190)
	echoaround $r ²r¯Pªº­·¼ÉÅý$R [1;32m* ¯kµh¤£³ô *[m!
	modhmv $r 01.151 190 0
	break
endif
if rand(60)
	echoat $r ²r¯Pªº­·¼ÉÅý§A [1;32m** ¯kµh¤£³ô **[m!(191-230)
	echoaround $r ²r¯Pªº­·¼ÉÅý$R [1;32m** ¯kµh¤£³ô **[m!
	modhmv $r 01.191 230 0
	break
endif
if rand(80)
	echoat $r ²r¯Pªº­·¼É¥´±o§A [1;32m** ¦yÁn«s¥s **[m!(231-280)
	echoaround $r ²r¯Pªº­·¼É¥´±o$R [1;32m** ¦yÁn«s¥s **[m!
	modhmv $r 01.231 280 0
	break
endif
if rand(90)
	echoat $r ²r¯Pªº­·¼É¥´±o§A [1;32m*** ¦yÁn«s¥s ***[m!(281-330)
	echoaround $r ²r¯Pªº­·¼É¥´±o$R [1;32m*** ¦yÁn«s¥s ***[m!
	modhmv $r 01.281 330 0
	break
endif
if rand(95)
	echoat $r ²r¯Pªº­·¼É¥´±o§A [1;32m*** ÂA¦åª½¬y ***[m.(331-380)
	echoaround $r ²r¯Pªº­·¼É¥´±o$R [1;32m*** ÂA¦åª½¬y ***[m.
	modhmv $r 01.331 380 0
else
	echoat $r ²r¯Pªº­·¼É¥´±o§A [1;32m**** ÂA¦åª½¬y ****[m.(381-450)
	echoaround $r ²r¯Pªº­·¼É¥´±o$R [1;32m**** ÂA¦åª½¬y ****[m.
	modhmv $r 01.381 450 0
endif
~
>room_mask cast g~
~
>room_mask cast ga~
~
>room_mask cast gat~
~
>room_mask cast gate~
~
>room_mask cast c~
~
>room_mask cast cl~
~
>room_mask cast cla~
~
>room_mask cast clay~
~
>room_mask cast clay g~
~
>room_mask cast clay go~
~
>room_mask cast clay gol~
~
>room_mask cast clay gole~
~
>room_mask cast clay golem~
~
>room_mask cast w~
~
>room_mask cast wa~
~
>room_mask cast wat~
~
>room_mask cast wate~
~
>room_mask cast water~
~
>room_mask cast water s~
~
>room_mask cast water sp~
~
>room_mask cast water spi~
~
>room_mask cast water spir~
~
>room_mask cast water spiris~
~
>room_mask cast water spirit~
~
>room_mask cast fire e~
~
>room_mask cast fire el~
~
>room_mask cast fire ele~
~
>room_mask cast fire elem~
~
>room_mask cast fire eleme~
~
>room_mask cast fire elemen~
~
>room_mask cast fire element~
~
>room_mask cast fire elementa~
~
>room_mask cast fire elemental~
~
>room_mask h~
~
>room_mask hs~
~
>room_mask hsu~
~
>room_mask hsum~
~
>room_mask hsumm~
~
>room_mask hsummo~
~
>room_mask hsummon~
~
|
