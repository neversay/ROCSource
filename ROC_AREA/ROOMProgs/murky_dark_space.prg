>room_rand 20~
if pcnum($) > 0
	if ispc($r)
		echo [1;32m�Ŷ��������O���M�ഫ�A $r ����F�������ƴ�F�X�h....[m
		if rand(20)
			force $r n
			break
		endif
		if rand(25)
			force $r s
			break
		endif
		if rand(33)
			force $r e
		else
			force $r w
		endif
	endif
endif
~
>room_rand 10~
if pcnum($) > 0
	echo [0;37m���M���e�Ŷ��}�l�ᦱ�A�åB�ƥX��v�V�A����........[m
	mload 10732
	force shade hunt $r
	force shade skilldrv multihit $r
endif
~
|