>room_rand 70~
if pcnum($) >= 1
	if ispc($r)
		echoat $r ���b�o�̡A�A�P��������ۦb�C
		modhmv $r 11.100 200 00000000
		modhmv $r 21.100 200 00000000
	endif
	if rand(50)
		echo ���䪺�p���O��M�ʤF�@�U...
		if rand(50)
			mload 59009
		endif
	else
		echo ��M���|�P�w�R�F�U�ӡA�A�Pı��n�����F��b����a��ݧA...
		if rand(25)
			mload 59006
		endif
	endif
	if rand(40)
		echo �@���p�Τl����F�A���c�l�W�C
	endif
	if rand(25)
		echo �Aı�o�e�����Ů�}�l�ᦱ...
		c 'cone of silence'
	endif
endif
~
|