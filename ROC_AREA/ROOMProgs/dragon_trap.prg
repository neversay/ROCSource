>room_time 10~
if roomvalue($0) == 1
	modrvt v0 = 0
endif
~
>room_command disable trap~
echoat $n �A���ոѰ��o���ж���������....
echoaround $n $n ���ոѰ��o���ж���������....
if roomvalue($0) == 0
	if class($n) == 10
		if int($n) <= 15
			if rand(20)
				callout 2 echoat $n [1;36m�A���\���Ѱ��F�ж������������I�C[m
				callout 2 echoaround $n $n [1;36m���\���Ѱ��F�ж�����������....�C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m�A���ѤF�I[m
				callout 2 echoaround $n $n [1;31m�Ѱ��ж������������ѤF�I[m
				if rand(60)
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 17
			if rand(40)
				callout 2 echoat $n [1;36m�A���\���Ѱ��F�ж������������I�C[m
				callout 2 echoaround $n $n [1;36m���\���Ѱ��F�ж�����������....�C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m�A���ѤF�I[m
				callout 2 echoaround $n $n [1;31m�Ѱ��ж������������ѤF�I[m
				if rand(40)
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 19
			if rand(60)
				callout 2 echoat $n [1;36m�A���\���Ѱ��F�ж������������I�C[m
				callout 2 echoaround $n $n [1;36m���\���Ѱ��F�ж�����������....�C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m�A���ѤF�I[m
				callout 2 echoaround $n $n [1;31m�Ѱ��ж������������ѤF�I[m
				if rand(20)
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 21
			if rand(80)
				callout 2 echoat $n [1;36m�A���\���Ѱ��F�ж������������I�C[m
				callout 2 echoaround $n $n [1;36m���\���Ѱ��F�ж�����������....�C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m�A���ѤF�I[m
				callout 2 echoaround $n $n [1;31m�Ѱ��ж������������ѤF�I[m
				if rand(10)
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 23
			callout 2 echoat $n [1;36m�A���\���Ѱ��F�ж������������I�C[m
			callout 2 echoaround $n $n [1;36m���\���Ѱ��F�ж�����������....�C[m
			callout 2 modrvt v0 = 1
		endif
	else
		callout 1 echoat $n [1;33m�A��¾�~���O�Ԫ̡A�ä��|�Ѱ��ж�����....�C[m
		callout 1 echoaround $n $n [1;33m���ոѰ��ж��������A���O���ѤF....�C[m
		callout 2 echoat $n [1;33m�AĲ�ʤF�ж���������....�C[m
		callout 2 echoaround $n $n [1;33mĲ�ʤF�ж���������....�C[m
		callout 2 call dragon_trap $n
	endif
else
	if rand(50)
		echo [1;36m�o���ж��������w�g�Q�Ѱ��F....�C[m
	else
		echo [1;36m�o���ж����G�S�����򲧼�....�C[m
	endif
endif
~
>room_command search around~
echoat $n �A���շj���o���ж�....
echoaround $n $n ���շj���o���ж�....
if roomvalue($0) == 0
	if class($n) == 10
		if int($n) <= 15
			if rand(20)
				callout 2 echoat $n [1;31m�A�o�{�o���ж����G�������s�b�A�����n�Ѱ�(disable trap)�C[m
				callout 2 echoaround $n $n [1;31m�o�{�o���ж����G�������s�b....�C[m
			else
				if rand(40)
					callout 2 echo [1;36m�o���ж����G�S�����򲧼�....�C[m
				else
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 17
			if rand(40)
				callout 2 echoat $n [1;31m�A�o�{�o���ж����G�������s�b�A�����n�Ѱ�(disable trap)�C[m
				callout 2 echoaround $n $n [1;31m�o�{�o���ж����G�������s�b....�C[m
			else
				if rand(60)
					callout 2 echo [1;36m�o���ж����G�S�����򲧼�....�C[m
				else
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 19
			if rand(60)
				callout 2 echoat $n [1;31m�A�o�{�o���ж����G�������s�b�A�����n�Ѱ�(disable trap)�C[m
				callout 2 echoaround $n $n [1;31m�o�{�o���ж����G�������s�b....�C[m
			else
				if rand(80)
					callout 2 echo [1;36m�o���ж����G�S�����򲧼�....�C[m
				else
					callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 21
			if rand(80)
				callout 2 echoat $n [1;31m�A�o�{�o���ж����G�������s�b�A�����n�Ѱ�(disable trap)�C[m
				callout 2 echoaround $n $n [1;31m�o�{�o���ж����G�������s�b....�C[m
			else
				callout 2 echo [1;36m�o���ж����G�S�����򲧼�....�C[m
			endif
			break
		endif
		if int($n) <= 23
			callout 2 echoat $n [1;31m�A�o�{�o���ж����G�������s�b�A�����n�Ѱ�(disable trap)�C[m
			callout 2 echoaround $n $n [1;31m�o�{�o���ж����G�������s�b....�C[m
		endif
	else
		if rand(5)
			callout 2 echoat $n [1;31m�A�o�{�o���ж����G�������s�b�A�����n�Ѱ�(disable trap)�C[m
			callout 2 echoaround $n $n [1;31m�o�{�o���ж����G�������s�b....�C[m
		else
			if rand(20)
				callout 2 echo [1;36m�o���ж����G�S�����򲧼�....�C[m
			else
				callout 2 echo [1;31m�V�|�I�AĲ�o�F����....�I[m
				callout 2 call dragon_trap $n
			endif
		endif
	endif
else
	if rand(50)
		echo [1;36m�o���ж��������w�g�Q�Ѱ��F....�C[m
	else
		echo [1;36m�o���ж����G�S�����򲧼�....�C[m
	endif
endif
~
>room_trap 251720655 80~
echo [1;31m�ذդ@�n�I�A���GĲ�ʤF����....�I[m
if roomvalue($0) == 1
	callout 1 echo [1;31m���O���G�S������Ʊ��o��....�C[m
else
	call dragon_trap $n
endif
~
>room_defun dragon_trap~
if rand(20)
	echo [1;31m���M�@���s���M���F�X�ӦV�A�r�P����....[m
	mload 2222
	callout 1 force dragon hunt $r
	break
endif
if rand(25)
	echo [1;31m�Ѫ�O�P�|�P��������M�g�X�F�X�䭸��....[m
	if class($n) == 10
		callout 1 echoat $n [1;36m�����Ԫ̪��A�H�¼����ޥ��ɦ�F�o�X�䭸��....�C[m
		callout 1 echoaround $n [1;36m�����Ԫ̪� $n �H�¼����ޥ��ɦ�F�o�X�䭸��....�C[m
		callout 1 oload 10208 viclv $n floor
	else
		if rand(50)
			callout 1 echo [1;33m�o�X�䭸��y���F�A���p���ˮ`....�C[m
			callout 1 modhmv pc 03.10 25 0
		else
			callout 1 echo [0;36m�A����@�{�A���𱰨��ӹL....�C[m
		endif
		callout 2 oload 10208 viclv $n floor
	endif
	break
endif
if rand(33)
	echo [1;31m�@�s�s�~���M�X�{�V�A��F�L��....[m
	mload 10253
	mload 10252
	force medium mpgroup self leader 0 1
	mload 10250
	force patrol mpgroup self leader 0 1
	force leader hunt $n
	force leader c fireball $n
else
	echo [1;32m�Ѫ�O�P�|�P��������M�Q�X�F�r��....[m
	if rand(40)
		callout 1 echo [1;32m�o�Ǭr�����A���r�F....�C[m
		callout 1 add_affect pc poison 0 50 1 -5 4096
		break
	endif
	if rand(45)
		callout 1 echo [1;32m�o�Ǭr���Y���I�k�A���ֽ�....�C[m
		callout 1 modhmv pc 03.10 25 0
	else
		callout 1 echo [1;32m�o�Ǭr�����A���r�F....�C[m
		callout 1 echo [1;32m�o�Ǭr���Y���I�k�A���ֽ�....�C[m
		callout 1 add_affect pc poison 0 50 1 -5 4096
		callout 1 modhmv pc 03.10 25 0
	endif
endif
~
|