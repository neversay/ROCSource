>room_command disable default~
echoat $n 你要將什麼東西解體？
~
>room_command disable kobold-soldier-ballista ballista~
if isfight($n)
	echoat $n 你正在戰鬥中，無法進行解體！
else
	if roomvalue($0) == 1
		if int($n) > 23
		or primeattr($n) == 2
			force kobold-soldier-ballista mpechoat $n 你成功找到弩砲結構上的弱點，將它解體了！
			force kobold-soldier-ballista mpechoaround $n $N成功找到弩砲結構上的弱點，將它解體了！
			mpmod_qmark $n war_reward v0 + 6
			force kobold-soldier-ballista mpat elf-soldier-marshal mpmod_qmark elf-soldier-marshal wartime v1 + 6
			purge kobold-soldier-ballista
		else
			force kobold-soldier-ballista mpechoat $n 你在弩砲上摸索許久，什麼也摸不出來。
			force kobold-soldier-ballista mpechoaround $n $N在弩砲上摸來摸去，不知道在做些什麼？
		endif
	else
		echoat $n 你要將什麼東西解體？
	endif
endif
~
|