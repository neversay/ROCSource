>room_defun fire_room2~
echo �q�D��ǼQ�������P���I�I�I
if rand(50)
	rcall fire_catch
endif
if rand(60)
	rcall fire_catch
endif
if rand(70)
	rcall fire_catch
endif
~
>room_defun fire_catch~
if ispc($r)
	rcall fire_check $r
endif
~
>room_defun reset_roomvalue~
if rnplayer($) == 0
	if roomvalue($0) != 0
		modrvt v0 = 0
	endif
endif
~
>room_defun fire_check~
if rand(20)
	echoat $r �r�P���������A [1;32m* �k�h���� *[m!(151-190)
	echoaround $r �r�P��������$R [1;32m* �k�h���� *[m!
	modhmv $r 01.151 190 0
	break
endif
if rand(40)
	echoat $r �r�P���������A [1;32m** �k�h���� **[m!(191-230)
	echoaround $r �r�P��������$R [1;32m** �k�h���� **[m!
	modhmv $r 01.191 230 0
	break
endif
if rand(60)
	echoat $r �r�P���������o�A [1;32m** �y�n�s�s **[m!(231-280)
	echoaround $r �r�P���������o$R [1;32m** �y�n�s�s **[m!
	modhmv $r 01.231 280 0
	break
endif
if rand(80)
	echoat $r �r�P���������o�A [1;32m*** �y�n�s�s ***[m!(281-330)
	echoaround $r �r�P���������o$R [1;32m*** �y�n�s�s ***[m!
	modhmv $r 01.281 330 0
else
	echoat $r �r�P���������o�A [1;32m*** �A�媽�y ***[m.(331-380)
	echoaround $r �r�P���������o$R [1;32m*** �A�媽�y ***[m.
	modhmv $r 01.331 380 0
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
