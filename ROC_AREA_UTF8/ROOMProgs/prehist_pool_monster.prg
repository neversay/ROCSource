>room_rand 25~
if pcnum($) >= 2
	if ispc($r)
		if rand(50)
			echo [1;33m忽然池塘底的土壤中鑽出了一隻怪魚向你攻擊！[m
			mload 15437
			force fish-prehist hunt $r
		else
			echo [1;33m忽然池塘底的土壤鑽出了一條蠕蟲向你攻擊！[m
			mload 15436
			force worm-prehist hunt $r
		endif
	endif
endif
~
|