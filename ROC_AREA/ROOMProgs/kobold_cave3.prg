>room_command disable default~
echoat $n �A�n�N����F�����H
~
>room_command disable kobold-soldier-ballista ballista~
if isfight($n)
	echoat $n �A���b�԰����A�L�k�i�����I
else
	if roomvalue($0) == 1
		if int($n) > 23
		or primeattr($n) == 2
			force kobold-soldier-ballista mpechoat $n �A���\��쩸�����c�W���z�I�A�N������F�I
			force kobold-soldier-ballista mpechoaround $n $N���\��쩸�����c�W���z�I�A�N������F�I
			mpmod_qmark $n war_reward v0 + 6
			force kobold-soldier-ballista mpat elf-soldier-marshal mpmod_qmark elf-soldier-marshal wartime v1 + 6
			purge kobold-soldier-ballista
		else
			force kobold-soldier-ballista mpechoat $n �A�b�����W�N���\�[�A����]�N���X�ӡC
			force kobold-soldier-ballista mpechoaround $n $N�b�����W�N�ӺN�h�A�����D�b���Ǥ���H
		endif
	else
		echoat $n �A�n�N����F�����H
	endif
endif
~
|