>room_command search default~
if is_qmark($n) == LF_hunter
if roomtimer($0) == 0
modrvt t0 = 4
echoat $n �A�p���l�l���j���ۥ|�P�����R�C
  if rand(80)
	echoat $n ���O�����G������F��X�S���n���I
	if rand(50)
		mload 22028
	else
	  if rand(60)
		mload 22026
	  else
		mload 22027
	  endif
	endif
  else
	echoat $n �A���򳣨S�o�{
  endif
else
echoat $n �o�̬ݨӤ~�観�H���L......
endif
else
	echoat $n �A���򳣨S�o�{
endif
~
|