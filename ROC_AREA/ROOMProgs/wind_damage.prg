>room_defun wind_damage~
if pcnum($) > 1
	echo ¤@°}¨g­·¬ðµM¦V§A­Ì§j¨Ó¡C
	if rand(10)
		echo ³o°}¨g­·¨Ï§A­Ì[1;35m¦³¨ÇÀ¿¶Ë[m(41-60)¡I
		modhmv pc 01.41 60 00000000
		break
	endif
	if rand(20)
		echo ³o°}¨g­·¨Ï§A­Ì[1;35m¶}©l¯}¥Ö[m(61-85)¡I
		modhmv pc 01.61 85 00000000
		break
	endif
	if rand(30)
		echo ³o°}¨g­·¨Ï§A­Ì[1;35m¯Q«C·ï¦å[m(86-115)¡I
		modhmv pc 01.86 115 00000000
		break
	endif
	if rand(40)
		echo ³o°}¨g­·¨Ï§A­Ì[1;35m* ¯Q«C·ï¦å *[m(116-150)¡I
		modhmv pc 01.116 150 00000000
		break
	endif
	if rand(50)
		echo ³o°}¨g­·¨Ï§A­Ì[1;32m* ¯kµh¤£³ô *[m(151-190)¡I
		modhmv pc 01.151 190 00000000
		break
	endif
	if rand(60)
		echo ³o°}¨g­·¨Ï§A­Ì[1;32m** ¯kµh¤£³ô **[m(191-230)¡I
		modhmv pc 01.191 230 00000000
		break
	endif
	if rand(70)
		echo ³o°}¨g­·¨Ï§A­Ì[1;32m** ¦yÁn«s¥s **[m(231-280)¡I
		modhmv pc 01.231 280 00000000
		break
	endif
	if rand(80)
		echo ³o°}¨g­·¨Ï§A­Ì[1;32m*** ¦yÁn«s¥s ***[m(281-330)¡I
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(90)
		echo ³o°}¨g­·¨Ï§A­Ì[1;32m*** ÂA¦åª½¬y ***[m(331-380)¡I
		modhmv pc 01.331 380 00000000
		break
	else
		echo ³o°}¨g­·¨Ï§A­Ì[1;32m**** ÂA¦åª½¬y ****[m(381-450)¡I
		modhmv pc 01.381 450 00000000
	endif
else
	echo ¤@°}¨g­·¬ðµM¦V§A§j¨Ó¡C
	if rand(10)
		echo ³o°}¨g­·¨Ï§A[1;35m¦³¨ÇÀ¿¶Ë[m(41-60)¡I
		modhmv pc 01.41 60 00000000
		break
	endif
	if rand(20)
		echo ³o°}¨g­·¨Ï§A[1;35m¶}©l¯}¥Ö[m(61-85)¡I
		modhmv pc 01.61 85 00000000
		break
	endif
	if rand(30)
		echo ³o°}¨g­·¨Ï§A[1;35m¯Q«C·ï¦å[m(86-115)¡I
		modhmv pc 01.86 115 00000000
		break
	endif
	if rand(40)
		echo ³o°}¨g­·¨Ï§A[1;35m* ¯Q«C·ï¦å *[m(116-150)¡I
		modhmv pc 01.116 150 00000000
		break
	endif
	if rand(50)
		echo ³o°}¨g­·¨Ï§A[1;32m* ¯kµh¤£³ô *[m(151-190)¡I
		modhmv pc 01.151 190 00000000
		break
	endif
	if rand(60)
		echo ³o°}¨g­·¨Ï§A[1;32m** ¯kµh¤£³ô **[m(191-230)¡I
		modhmv pc 01.191 230 00000000
		break
	endif
	if rand(70)
		echo ³o°}¨g­·¨Ï§A[1;32m** ¦yÁn«s¥s **[m(231-280)¡I
		modhmv pc 01.231 280 00000000
		break
	endif
	if rand(80)
		echo ³o°}¨g­·¨Ï§A[1;32m*** ¦yÁn«s¥s ***[m(281-330)¡I
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(90)
		echo ³o°}¨g­·¨Ï§A[1;32m*** ÂA¦åª½¬y ***[m(331-380)¡I
		modhmv pc 01.331 380 00000000
		break
	else
		echo ³o°}¨g­·¨Ï§A[1;32m**** ÂA¦åª½¬y ****[m(381-450)¡I
		modhmv pc 01.381 450 00000000
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