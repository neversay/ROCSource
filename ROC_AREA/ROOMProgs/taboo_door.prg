>room_enter 127 100~
if roomtimer($0) == 0
        if is_inv_obj($n) == 32112
                if rand(20)
                        echoat $n �A���W������Ǥ��_�o�X�F���������~�I
                        echoaround $n $n ���W��������Ǥ��_�o�X�F���������~�I
                        echo �_�ͪ����~�M�Ӧb�a�O�W�X�{�F�@�D�����Ϊ��I
                        echo ���������~�}�l�~���A�@�Ӫ����}�զ����I
                        echoat $n �A�⤤���_�͵o�X�������A������I
                        force $n drop key-locked
                        echo �_�ֱͧ���a�W�e�N�����b���~���F!
                        purge key-locked
                        modrvt v1 = 1
                        modrvt t0 = 3
                else
                        modrvt t0 = 5
                endif
        endif
endif
~
>room_mask down default~
if roomvalue($1) == 1
        if pcnum($) == 1
          echoat $n ��A��L�լ}����A�լ}�b�A��������F�I
          callout 2 at $n echoaround $n �� $n ��L�լ}��A�լ}�����F�I
          transfer $n 32265
          modrvt v1 = 0
        else
          echoat $n �A��L�o�G���լ}���U���h�C
          echoaround $n $n ��L�o�����լ}���U���h�I
          transfer $n 32265
        endif
else
        echoat $n [0;33m�u�r�A�������q�C[m
endif
~
|
