>room_defun fire_damage~
if pcnum($) > 1
	if rand(10)
		echo 突如其來的火炎使你們[1;35m有些擦傷[m(41-60)！
		modhmv pc 01.41 60 0
		break
	endif
	if rand(20)
		echo 突如其來的火炎使你們[1;35m開始破皮[m(61-85)！
		modhmv pc 01.61 85 0
		break
	endif
	if rand(30)
		echo 突如其來的火炎使你們[1;35m烏青瘀血[m(86-115)！
		modhmv pc 01.86 115 0
		break
	endif
	if rand(40)
		echo 突如其來的火炎使你們[1;35m* 烏青瘀血 *[m(116-150)！
		modhmv pc 01.116 150 0
		break
	endif
	if rand(50)
		echo 突如其來的火炎使你們[1;32m* 疼痛不堪 *[m(151-190)！
		modhmv pc 01.151 190 0
		break
	endif
	if rand(60)
		echo 突如其來的火炎使你們[1;32m** 疼痛不堪 **[m(191-230)！
		modhmv pc 01.191 230 0
		break
	endif
	if rand(70)
		echo 突如其來的火炎使你們[1;32m** 尖聲哀叫 **[m(231-280)！
		modhmv pc 01.231 280 0
		break
	endif
	if rand(80)
		echo 突如其來的火炎使你們[1;32m*** 尖聲哀叫 ***[m(281-330)！
		modhmv pc 01.281 330 0
		break
	endif
	if rand(90)
		echo 突如其來的火炎使你們[1;32m*** 鮮血直流 ***[m(331-380)！
		modhmv pc 01.331 380 0
		break
	else
		echo 突如其來的火炎使你們[1;32m**** 鮮血直流 ****[m(381-450)！
		modhmv pc 01.381 450 0
	endif
else
	if rand(10)
		echo 突如其來的火炎使你[1;35m有些擦傷[m(41-60)！
		modhmv pc 01.41 60 0
		break
	endif
	if rand(20)
		echo 突如其來的火炎使你[1;35m開始破皮[m(61-85)！
		modhmv pc 01.61 85 0
		break
	endif
	if rand(30)
		echo 突如其來的火炎使你[1;35m烏青瘀血[m(86-115)！
		modhmv pc 01.86 115 0
		break
	endif
	if rand(40)
		echo 突如其來的火炎使你[1;35m* 烏青瘀血 *[m(116-150)！
		modhmv pc 01.116 150 0
		break
	endif
	if rand(50)
		echo 突如其來的火炎使你[1;32m* 疼痛不堪 *[m(151-190)！
		modhmv pc 01.151 190 0
		break
	endif
	if rand(60)
		echo 突如其來的火炎使你[1;32m** 疼痛不堪 **[m(191-230)！
		modhmv pc 01.191 230 0
		break
	endif
	if rand(70)
		echo 突如其來的火炎使你[1;32m** 尖聲哀叫 **[m(231-280)！
		modhmv pc 01.231 280 0
		break
	endif
	if rand(80)
		echo 突如其來的火炎使你[1;32m*** 尖聲哀叫 ***[m(281-330)！
		modhmv pc 01.281 330 0
		break
	endif
	if rand(90)
		echo 突如其來的火炎使你[1;32m*** 鮮血直流 ***[m(331-380)！
		modhmv pc 01.331 380 0
		break
	else
		echo 突如其來的火炎使你[1;32m**** 鮮血直流 ****[m(381-450)！
		modhmv pc 01.381 450 0
	endif
endif
~
>room_mask cast gate~
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