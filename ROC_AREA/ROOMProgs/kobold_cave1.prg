>room_enter 63 100~
if being(elf-soldier-marshal)
	if name($n) / elf-soldier
		if rand(20)
			echo ���䪺�s�L�̩��M�_�X�F�@���a�F�§A�ĨӡI
			mload 66100
			force kobold-soldier-striker bs $n
		endif
	endif
	if is_qmark($n) != kobold_cave_hero
	or is_qmark($n) <= kobold_cave_hero 100012
		if rand(5)
			echo ���䪺�s�L�̩��M�_�X�F�@���a�F�§A�ĨӡI
			mload 66100
			force kobold-soldier-striker bs $n
		endif
	endif
else
	if rand(5)
		mload 66121
		if race($n) != werewolf
			echo ���䪺�s�L�̩��M�_�X�F�@���������T�§A�ĨӡI
			force wolf-kobold mpkill $n
		else
			echo �@���������T���M�_�F�X�ӡA�e�µۧA�a��åB�@�ƿ˼����ˤl�C
			force wolf-kobold follow $n
		endif
	endif
endif
~
|