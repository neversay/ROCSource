>room_defun fire_damage~
if pcnum($) > 1
	if rand(10)
		echo ��p��Ӫ������ϧA��[1;35m��������[m(41-60)�I
		modhmv pc 01.41 60 0
		break
	endif
	if rand(20)
		echo ��p��Ӫ������ϧA��[1;35m�}�l�}��[m(61-85)�I
		modhmv pc 01.61 85 0
		break
	endif
	if rand(30)
		echo ��p��Ӫ������ϧA��[1;35m�Q�C���[m(86-115)�I
		modhmv pc 01.86 115 0
		break
	endif
	if rand(40)
		echo ��p��Ӫ������ϧA��[1;35m* �Q�C��� *[m(116-150)�I
		modhmv pc 01.116 150 0
		break
	endif
	if rand(50)
		echo ��p��Ӫ������ϧA��[1;32m* �k�h���� *[m(151-190)�I
		modhmv pc 01.151 190 0
		break
	endif
	if rand(60)
		echo ��p��Ӫ������ϧA��[1;32m** �k�h���� **[m(191-230)�I
		modhmv pc 01.191 230 0
		break
	endif
	if rand(70)
		echo ��p��Ӫ������ϧA��[1;32m** �y�n�s�s **[m(231-280)�I
		modhmv pc 01.231 280 0
		break
	endif
	if rand(80)
		echo ��p��Ӫ������ϧA��[1;32m*** �y�n�s�s ***[m(281-330)�I
		modhmv pc 01.281 330 0
		break
	endif
	if rand(90)
		echo ��p��Ӫ������ϧA��[1;32m*** �A�媽�y ***[m(331-380)�I
		modhmv pc 01.331 380 0
		break
	else
		echo ��p��Ӫ������ϧA��[1;32m**** �A�媽�y ****[m(381-450)�I
		modhmv pc 01.381 450 0
	endif
else
	if rand(10)
		echo ��p��Ӫ������ϧA[1;35m��������[m(41-60)�I
		modhmv pc 01.41 60 0
		break
	endif
	if rand(20)
		echo ��p��Ӫ������ϧA[1;35m�}�l�}��[m(61-85)�I
		modhmv pc 01.61 85 0
		break
	endif
	if rand(30)
		echo ��p��Ӫ������ϧA[1;35m�Q�C���[m(86-115)�I
		modhmv pc 01.86 115 0
		break
	endif
	if rand(40)
		echo ��p��Ӫ������ϧA[1;35m* �Q�C��� *[m(116-150)�I
		modhmv pc 01.116 150 0
		break
	endif
	if rand(50)
		echo ��p��Ӫ������ϧA[1;32m* �k�h���� *[m(151-190)�I
		modhmv pc 01.151 190 0
		break
	endif
	if rand(60)
		echo ��p��Ӫ������ϧA[1;32m** �k�h���� **[m(191-230)�I
		modhmv pc 01.191 230 0
		break
	endif
	if rand(70)
		echo ��p��Ӫ������ϧA[1;32m** �y�n�s�s **[m(231-280)�I
		modhmv pc 01.231 280 0
		break
	endif
	if rand(80)
		echo ��p��Ӫ������ϧA[1;32m*** �y�n�s�s ***[m(281-330)�I
		modhmv pc 01.281 330 0
		break
	endif
	if rand(90)
		echo ��p��Ӫ������ϧA[1;32m*** �A�媽�y ***[m(331-380)�I
		modhmv pc 01.331 380 0
		break
	else
		echo ��p��Ӫ������ϧA[1;32m**** �A�媽�y ****[m(381-450)�I
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