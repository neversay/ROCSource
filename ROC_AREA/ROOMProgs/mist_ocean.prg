>room_rand 5~
if rand(50)
	echo �A���M�ݨ즳�F��Q�������R�W����....�C
else
	echo ���M���A�����a�ۤ@�Ӥ����W���F��A�R��A���}��....�C
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