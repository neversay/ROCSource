>room_enter 63 100~
if being(elf-soldier-marshal)
	if name($n) / elf-soldier
		if rand(20)
			echo 旁邊的山林裡忽然冒出了一隻地靈朝你衝來！
			mload 66100
			force kobold-soldier-striker bs $n
		endif
	endif
	if is_qmark($n) != kobold_cave_hero
	or is_qmark($n) <= kobold_cave_hero 100012
		if rand(5)
			echo 旁邊的山林裡忽然冒出了一隻地靈朝你衝來！
			mload 66100
			force kobold-soldier-striker bs $n
		endif
	endif
else
	if rand(5)
		mload 66121
		if race($n) != werewolf
			echo 旁邊的山林裡忽然冒出了一隻巨型野狼朝你衝來！
			force wolf-kobold mpkill $n
		else
			echo 一隻巨型野狼忽然冒了出來，牠朝著你靠近並且一副親熱的樣子。
			force wolf-kobold follow $n
		endif
	endif
endif
~
|