>room_enter 127 100~
if ispc($n)
	echoat $n �|�P����ۨϧA���ʧ@�ܱo�w�C�I
	add_affect $n tardy 0 100 2 -5 34359738368
endif
~
>room_leave 127 100~
if ispc($n)
	echoat $n �A�Pı������ۧA����ۮ����F�I
	del_affect $n tardy
endif
~
>room_defun wind_altar~
echo �|����������ɼQ�X�������P��!
if rand(20)
	rcall wind_catch
endif
if rand(40)
	rcall wind_catch
endif
if rand(60)
	rcall wind_catch
endif
~
>room_defun wind_catch~
if ispc($r)
	rcall wind_check $r
endif
~
>room_defun wind_check~
if rand(10)
	echoat $r �r�P���������A [1;35m��������[m!(41-60)
	echoaround $r �r�P��������$R [1;35m��������[m!
	modhmv $r 01.41 60 0
	break
endif
if rand(20)
	echoat $r �r�P���������A [1;35m�}�l�}��[m!(61-85)
	echoaround $r �r�P��������$R [1;35m�}�l�}��[m!
	modhmv $r 01.61 85 0
	break
endif
if rand(30)
	echoat $r �r�P���������A [1;35m�Q�C���[m!(86-115)
	echoaround $r �r�P��������$R [1;35m�Q�C���[m!
	modhmv $r 01.86 115 0
	break
endif
if rand(40)
	echoat $r �r�P���������A [1;35m* �Q�C��� *[m!(116-150)
	echoaround $r �r�P��������$R [1;35m* �Q�C��� *[m!
	modhmv $r 01.116 150 0
	break
endif
if rand(50)
	echoat $r �r�P���������A [1;32m* �k�h���� *[m!(151-190)
	echoaround $r �r�P��������$R [1;32m* �k�h���� *[m!
	modhmv $r 01.151 190 0
	break
endif
if rand(60)
	echoat $r �r�P���������A [1;32m** �k�h���� **[m!(191-230)
	echoaround $r �r�P��������$R [1;32m** �k�h���� **[m!
	modhmv $r 01.191 230 0
	break
endif
if rand(70)
	echoat $r �r�P�����ɥ��o�A [1;32m** �y�n�s�s **[m!(231-280)
	echoaround $r �r�P�����ɥ��o$R [1;32m** �y�n�s�s **[m!
	modhmv $r 01.231 280 0
	break
endif
if rand(90)
	echoat $r �r�P�����ɥ��o�A [1;32m*** �y�n�s�s ***[m!(281-330)
	echoaround $r �r�P�����ɥ��o$R [1;32m*** �y�n�s�s ***[m!
	modhmv $r 01.281 330 0
	break
endif
if rand(95)
	echoat $r �r�P�����ɥ��o�A [1;32m*** �A�媽�y ***[m.(331-380)
	echoaround $r �r�P�����ɥ��o$R [1;32m*** �A�媽�y ***[m.
	modhmv $r 01.331 380 0
else
	echoat $r �r�P�����ɥ��o�A [1;32m**** �A�媽�y ****[m.(381-450)
	echoaround $r �r�P�����ɥ��o$R [1;32m**** �A�媽�y ****[m.
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
