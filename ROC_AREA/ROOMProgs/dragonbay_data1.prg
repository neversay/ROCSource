>room_mask scout north~
rcall dragonbay_mask_scout_north_check $n
~
>room_mask scout nort~
rcall dragonbay_mask_scout_north_check $n
~
>room_mask scout nor~
rcall dragonbay_mask_scout_north_check $n
~
>room_mask scout no~
rcall dragonbay_mask_scout_north_check $n
~
>room_mask scout n~
rcall dragonbay_mask_scout_north_check $n
~
>room_defun dragonbay_mask_scout_north~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
        force $n scout north
        break
endif
if roomvalue($0) & 1
if roomvalue($0) & 2
if roomvalue($0) & 4
        force $n scout north
        break
endif
endif
endif
echoat $n [1;30m�_��Q�@��ФH���j�ﵷ�����F�A�ϱo�A�L�k���Q���氻��ʧ@�C[0m
echoaround $n [1;30m�A�ݨ� $N [1;30m���G�L���j�ﵷ���s�b�A���յ۩��_�谵����ʧ@�C[0m
~
>room_defun dragonbay_mask_north~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
	if ispc($n)
        	force $n north
        	break
	else
		transfer $n north
		break
	endif
endif
if roomvalue($0) & 1
if roomvalue($0) & 2
if roomvalue($0) & 4
	if ispc($n)
        	force $n north
        	break
	else
		transfer $n north
		break
	endif
endif
endif
endif
if roomvalue($0) !& 1
        rcall dragonbay_north_flaming $n
else
        if roomvalue($0) !& 2
                rcall dragonbay_north_flaming $n
        else
                rcall dragonbay_north_flaming $n
        endif
endif
~
>room_defun dragonbay_north_flaming~
if isaffected($n) !& 134217728
        echoat $n [1;37m�A�յ۩��_�訫�A���O�o�Q���ФH���j�ﵷ�����Ӥ��o�ʼu�C[0m
        echoaround $n [1;37m�A�ݨ�$N[1;37m�յ۩��_�訫�A���o�Q�j�ﵷ�����Ӥ��o�ʼu�C[0m
        rcall dragonbay_para_check2 $n
        echo [1;30m�Ӿ�ӻj����]�]���Ӳ��ͤF�ǳ\�̰ʡI[0m
        rcall dragonbay_mload $n
else
        echoat $n �A�յ۩��_�訫�F�L�h�C
        echoaround $n �A�ݨ�$N���_�訫�F�L�h�C
        if mpnum($n) < 20
                echoat $n [1;34m�A���W���F�O�w�g�����l�ӡA�K�Ҥ]�b���������C[0m
                echoaround $n [1;34m�A�ݨ� $N [1;34m�~�����X�}�B�A���W���K�ҴN�����F�C[0m
                del_affect $n 'flaming armor'
                break
        endif
        if roomvalue($0) !& 1
                if rand(70)
                        echoat $n [1;32m�A���W���K�ұN�j����N���F�@�����C[0m
                        echoaround $n [1;32m�A�ݨ�j����Q $N [1;32m���K�ҿN���F�@�����C[0m
                        if roomtimer(1) == 0
				if mobcount(24209) > 0
                                	modrvt t1 = 24
				endif
                        endif
                        rcall dragonbay_north_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 1
                else
                        rcall dragonbay_north_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 2
                if rand(70)
                        echoat $n [1;33m�A���W���K�ұN�j������N���F�@�j�b�C[0m
                        echoaround $n [1;33m�A�ݨ�j����Q $N [1;33m���W���K�ҵ��N�F�@�j�b�C[0m
                        rcall dragonbay_north_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 2
                else
                        rcall dragonbay_north_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 4
                if rand(70)
                        echoat $n [1;36m�A���\���N�_�䪺�j������N�Ӻ���I�_��ש�i�H�q��F�C[0m
                        echoaround $n [1;36m�A�ݨ� $N [1;36m���\�a�N�_�誺�j����M���F�A�_��]�]���i�H�q��F�C[0m
                        rcall dragonbay_north_flaming_end $n
                        rcall dragonbay_north_linkroom $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 4
                else
                        rcall dragonbay_north_miss_message $n
                endif
        endif
endif
~
>room_defun dragonbay_south_linkroom_message~
echo [1;36m�A�ݨ�쥻���_��X�f���j�ﵷ��M���Q�N�ɡA�_��]�]���i�H�q��F�I[0m
modrvt v0 | 7
~
>room_defun dragonbay_south_linkroom_message2~
if roomvalue($0) & 7
        echo [1;36m�A�ݨ�_��쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 7
else
        if roomvalue($0) & 3
                echo [1;36m�A�ݨ�_��쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 3
        else
                echo [1;36m�A�ݨ�_��y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 1
        endif
endif
~
>room_defun dragonbay_north_flaming_end~
echoat $n [1;35m�A�P��ۤv���F�O�b�������ӤF���֡C[0m
modhmv $n 11.20 20 0
echo [1;30m�P�D���j�ﵷ�]�]���Ӳ��ͤF�̰ʡC[0m
rcall dragonbay_mload $n
if rand(15)
        echoat $n [1;31m�A���W���K�ҫo�]�b���ɮ����F�C[0m
        echoaround $n [1;31m�A�ݨ� $N [1;31m���K�Ҥ]�b�P�ɶ������F�C[0m
        del_affect $n 'flaming armor'
endif
~
>room_defun dragonbay_north_miss_message~
echoat $n [1;34m�A�յ۬�V���O�H���Ъ��j�ﵷ�A�i���o�Q�j������ӥ��ѤF�C[0m
echoaround $n [1;34m�A�ݨ� $N [1;34m�b��X�}�B�S�h�[����K�Q�j������Ӥ�B���e�C[0m
rcall dragonbay_para_check $n
rcall dragonbay_north_flaming_end $n
~
|
