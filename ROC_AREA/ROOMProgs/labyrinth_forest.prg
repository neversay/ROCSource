>room_command pray default~
echoat $n �V�A������D�O���a��
~
>room_command pray pluto~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100001
	modrvt v0 | 1
	stun $n 30
	echoat $n �A�V ��ߤ��P���|�hPluto �j�H��D���֡��
	echoaround $n $N�V ��ߤ��P���|�hPluto �j�H��D���֡��
	callout 1 echoat $n �@�}�X�M���ե��]��ۧA....�����A�w�g�^����l�̤F�I
	callout 1 echoaround $n $N�����o�X�G��....�����F�I
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n ��ߤ��P���|�hPluto ���@���U�A....
endif
~
>room_command pray amenda~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100002
	modrvt v0 | 1
	stun $n 30
	echoat $n �A�V �����z�C���C�ҹFAmenda �j�H��D���֡��
	echoaround $n $N�V �����z�C���C�ҹFAmenda �j�H��D���֡��
	callout 1 echoat $n �@�}�X�M���ե��]��ۧA....�����A�w�g�^����l�̤F�I
	callout 1 echoaround $n $N�����o�X�G��....�����F�I
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n �����z�C���C�ҹFAmenda ���@���U�A....
endif
~
>room_command pray zcecil~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100003
	modrvt v0 | 1
	stun $n 30
	echoat $n �A�V �v�lZcecil �j�H��D���֡��
	echoaround $n $N�V �v�lZcecil �j�H��D���֡��
	callout 1 echoat $n �@�}�X�M���ե��]��ۧA....�����A�w�g�^����l�̤F�I
	callout 1 echoaround $n $N�����o�X�G��....�����F�I
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n �v�lZcecil ���@���U�A....
endif
~
>room_command pray coconet~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100004
	modrvt v0 | 1
	stun $n 30
	echoat $n �A�V ���l��Coconet �j�H��D���֡��
	echoaround $n $N�V ���l��Coconet �j�H��D���֡��
	callout 1 echoat $n �@�}�X�M���ե��]��ۧA....�����A�w�g�^����l�̤F�I
	callout 1 echoaround $n $N�����o�X�G��....�����F�I
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n ���l��Coconet ���@���U�A....
endif
~
>room_command pray blaz~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100005
	modrvt v0 | 1
	stun $n 30
	echoat $n �A�V �j�]�����p��Blaz �j�H��D���֡��
	echoaround $n $N�V �j�]�����p��Blaz �j�H��D���֡��
	callout 1 echoat $n �@�}�X�M���ե��]��ۧA....�����A�w�g�^����l�̤F�I
	callout 1 echoaround $n $N�����o�X�G��....�����F�I
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n �A�ڥ����h�� �j�]�����p��Blaz ��ë....
endif
~
|