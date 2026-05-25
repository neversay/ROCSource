>room_defun wind_damage~
if pcnum($) > 1
	echo 一陣狂風突然向你們吹來。
	if rand(10)
		echo 這陣狂風使你們[1;35m有些擦傷[m(41-60)！
		modhmv pc 01.41 60 00000000
		break
	endif
	if rand(20)
		echo 這陣狂風使你們[1;35m開始破皮[m(61-85)！
		modhmv pc 01.61 85 00000000
		break
	endif
	if rand(30)
		echo 這陣狂風使你們[1;35m烏青瘀血[m(86-115)！
		modhmv pc 01.86 115 00000000
		break
	endif
	if rand(40)
		echo 這陣狂風使你們[1;35m* 烏青瘀血 *[m(116-150)！
		modhmv pc 01.116 150 00000000
		break
	endif
	if rand(50)
		echo 這陣狂風使你們[1;32m* 疼痛不堪 *[m(151-190)！
		modhmv pc 01.151 190 00000000
		break
	endif
	if rand(60)
		echo 這陣狂風使你們[1;32m** 疼痛不堪 **[m(191-230)！
		modhmv pc 01.191 230 00000000
		break
	endif
	if rand(70)
		echo 這陣狂風使你們[1;32m** 尖聲哀叫 **[m(231-280)！
		modhmv pc 01.231 280 00000000
		break
	endif
	if rand(80)
		echo 這陣狂風使你們[1;32m*** 尖聲哀叫 ***[m(281-330)！
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(90)
		echo 這陣狂風使你們[1;32m*** 鮮血直流 ***[m(331-380)！
		modhmv pc 01.331 380 00000000
		break
	else
		echo 這陣狂風使你們[1;32m**** 鮮血直流 ****[m(381-450)！
		modhmv pc 01.381 450 00000000
	endif
else
	echo 一陣狂風突然向你吹來。
	if rand(10)
		echo 這陣狂風使你[1;35m有些擦傷[m(41-60)！
		modhmv pc 01.41 60 00000000
		break
	endif
	if rand(20)
		echo 這陣狂風使你[1;35m開始破皮[m(61-85)！
		modhmv pc 01.61 85 00000000
		break
	endif
	if rand(30)
		echo 這陣狂風使你[1;35m烏青瘀血[m(86-115)！
		modhmv pc 01.86 115 00000000
		break
	endif
	if rand(40)
		echo 這陣狂風使你[1;35m* 烏青瘀血 *[m(116-150)！
		modhmv pc 01.116 150 00000000
		break
	endif
	if rand(50)
		echo 這陣狂風使你[1;32m* 疼痛不堪 *[m(151-190)！
		modhmv pc 01.151 190 00000000
		break
	endif
	if rand(60)
		echo 這陣狂風使你[1;32m** 疼痛不堪 **[m(191-230)！
		modhmv pc 01.191 230 00000000
		break
	endif
	if rand(70)
		echo 這陣狂風使你[1;32m** 尖聲哀叫 **[m(231-280)！
		modhmv pc 01.231 280 00000000
		break
	endif
	if rand(80)
		echo 這陣狂風使你[1;32m*** 尖聲哀叫 ***[m(281-330)！
		modhmv pc 01.281 330 00000000
		break
	endif
	if rand(90)
		echo 這陣狂風使你[1;32m*** 鮮血直流 ***[m(331-380)！
		modhmv pc 01.331 380 00000000
		break
	else
		echo 這陣狂風使你[1;32m**** 鮮血直流 ****[m(381-450)！
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