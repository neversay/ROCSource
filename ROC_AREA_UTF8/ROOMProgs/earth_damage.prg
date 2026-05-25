>room_defun earth_damage~
if pcnum($) > 1
	if rand(20)
		echo 強烈的地震使你們[1;32m*** 尖聲哀叫 ***[m(281-330)！
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(40)
		echo 強烈的地震使你們[1;32m*** 鮮血直流 ***[m(331-380)！
		modhmv pc 01.331 380 00000000
		break
	endif
	if rand(60)
		echo 強烈的地震使你們[1;32m**** 鮮血直流 ****[m(381-450)！
		modhmv pc 01.381 450 00000000
		break
	endif
	if rand(80)
		echo 強烈的地震使你們[1;32m**** 血流如注 ****[m(451-520)！
		modhmv pc 01.451 520 00000000
		break
	else
		echo 強烈的地震使你們[1;32m***** 血流如注 *****[m(521-600)！
		modhmv pc 01.521 600 00000000
		break
	endif
else
	if rand(20)
		echo 強烈的地震使你[1;32m*** 尖聲哀叫 ***[m(281-330)！
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(40)
		echo 強烈的地震使你[1;32m*** 鮮血直流 ***[m(331-380)！
		modhmv pc 01.331 380 00000000
		break
	endif
	if rand(60)
		echo 強烈的地震使你[1;32m**** 鮮血直流 ****[m(381-450)！
		modhmv pc 01.381 450 00000000
		break
	endif
	if rand(80)
		echo 強烈的地震使你[1;32m**** 血流如注 ****[m(451-520)！
		modhmv pc 01.451 520 00000000
		break
	else
		echo 強烈的地震使你[1;32m***** 血流如注 *****[m(521-600)！
		modhmv pc 01.521 600 00000000
		break
	endif
endif
~
>room_mask cast gate~
~
>room_mask cast clay~
~
>room_mask cast clay golem~
~
>room_mask cast water spirit~
~
>room_mask cast fire elemental~
~
>room_mask hsummon~
~
|