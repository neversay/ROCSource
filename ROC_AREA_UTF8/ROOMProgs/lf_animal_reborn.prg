>room_command search default~
if is_qmark($n) == LF_hunter
if roomtimer($0) == 0
modrvt t0 = 4
echoat $n 你小心翼翼的搜索著四周的動靜。
  if rand(70)
	echoat $n 草叢中似乎有什麼東西出沒的聲音！
	if rand(50)
		mload 22028
	else
		if rand(50)
			mload 22026
		else
		  if rand(70)
			mload 22027
		  else
			mload 22025
		  endif
		endif
	endif
  else
	echoat $n 你什麼都沒發現
  endif
else
echoat $n 這裡看來才剛有人走過......
endif
else
	echoat $n 你什麼都沒發現
endif
~
|