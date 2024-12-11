>room_defun earth_damage~
if pcnum($) > 1
	if rand(20)
		echo ±j¯Pªº¦a¾_¨Ï§A­Ì[1;32m*** ¦yÁn«s¥s ***[m(281-330)¡I
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(40)
		echo ±j¯Pªº¦a¾_¨Ï§A­Ì[1;32m*** ÂA¦åª½¬y ***[m(331-380)¡I
		modhmv pc 01.331 380 00000000
		break
	endif
	if rand(60)
		echo ±j¯Pªº¦a¾_¨Ï§A­Ì[1;32m**** ÂA¦åª½¬y ****[m(381-450)¡I
		modhmv pc 01.381 450 00000000
		break
	endif
	if rand(80)
		echo ±j¯Pªº¦a¾_¨Ï§A­Ì[1;32m**** ¦å¬y¦pª` ****[m(451-520)¡I
		modhmv pc 01.451 520 00000000
		break
	else
		echo ±j¯Pªº¦a¾_¨Ï§A­Ì[1;32m***** ¦å¬y¦pª` *****[m(521-600)¡I
		modhmv pc 01.521 600 00000000
		break
	endif
else
	if rand(20)
		echo ±j¯Pªº¦a¾_¨Ï§A[1;32m*** ¦yÁn«s¥s ***[m(281-330)¡I
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(40)
		echo ±j¯Pªº¦a¾_¨Ï§A[1;32m*** ÂA¦åª½¬y ***[m(331-380)¡I
		modhmv pc 01.331 380 00000000
		break
	endif
	if rand(60)
		echo ±j¯Pªº¦a¾_¨Ï§A[1;32m**** ÂA¦åª½¬y ****[m(381-450)¡I
		modhmv pc 01.381 450 00000000
		break
	endif
	if rand(80)
		echo ±j¯Pªº¦a¾_¨Ï§A[1;32m**** ¦å¬y¦pª` ****[m(451-520)¡I
		modhmv pc 01.451 520 00000000
		break
	else
		echo ±j¯Pªº¦a¾_¨Ï§A[1;32m***** ¦å¬y¦pª` *****[m(521-600)¡I
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