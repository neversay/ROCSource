>room_rand 50~
if pcnum($) >= 1
	if ispc($r)
		echoat $r ���b�o�̡A�A�P��������ۦb�C
		modhmv $r 11.100 200 00000000
		modhmv $r 21.100 200 00000000
	endif
	if rand(50)
		echo �|�P���𸭳Q�A�˱o�F�F�@�T�C
	else
		echo �|�P�Ǩө_�Ǫ������n�C
	endif
	if rand(25)
		echo �A�Pı��n�����F��b�ݵۧA�C
		if rand(50)
			mload 59009
		else
			mload 59020
		endif
	endif
endif
~
|