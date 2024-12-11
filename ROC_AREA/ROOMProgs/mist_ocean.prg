>room_rand 5~
if rand(50)
	echo 你忽然看到有東西被海水給沖上岸來....。
else
	echo 忽然間，海浪帶著一個不知名的東西，沖到你的腳邊....。
endif
if rand(25)
	if rand(20)
		oload 4599 40
	else
		if rand(40)
			oload 4599 15
		else
			oload 4599 1
		endif
	endif
else
	if rand(33)
		if rand(20)
			oload 4600 40
		else
			if rand(40)
				oload 4600 15
			else
				oload 4600 1
			endif
		endif
	else
		if rand(50)
			oload 4601 1
		else
			if rand(50)
				if rand(20)
					oload 4602 40
				else
					if rand(40)
						oload 4602 15
					else
						oload 4602 1
					endif
				endif
			else
				if rand(20)
					oload 4603 40
				else
					if rand(40)
						oload 4603 15
					else
						oload 4603 1
					endif
				endif
			endif
		endif
	endif
endif
~
|