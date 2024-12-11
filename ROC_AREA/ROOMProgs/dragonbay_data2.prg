>room_mask scout east~
rcall dragonbay_mask_scout_east_check $n
~
>room_mask scout eas~
rcall dragonbay_mask_scout_east_check $n
~
>room_mask scout ea~
rcall dragonbay_mask_scout_east_check $n
~
>room_mask scout e~
rcall dragonbay_mask_scout_east_check $n
~
>room_defun dragonbay_mask_scout_east~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
        force $n scout east
        break
endif
if roomvalue($0) & 8
if roomvalue($0) & 16
if roomvalue($0) & 32
        force $n scout east
        break
endif
endif
endif
echoat $n [1;30m�F��Q�@��ФH���j�ﵷ�����F�A�ϱo�A�L�k���Q���氻��ʧ@�C[0m
echoaround $n [1;30m�A�ݨ� $N [1;30m���G�L���j�ﵷ���s�b�A���յ۩��F�谵����ʧ@�C[0m
~
>room_defun dragonbay_mask_east~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
	if ispc($n)
        	force $n east
        	break
	else
		transfer $n east
		break
	endif
endif
if roomvalue($0) & 8
if roomvalue($0) & 16
if roomvalue($0) & 32
	if ispc($n)
        	force $n east
        	break
	else
		transfer $n east
		break
	endif
endif
endif
endif
if roomvalue($0) !& 8
        rcall dragonbay_east_flaming $n
else
        if roomvalue($0) !& 16
                rcall dragonbay_east_flaming $n
        else
                rcall dragonbay_east_flaming $n
        endif
endif
~
>room_defun dragonbay_east_flaming~
if isaffected($n) !& 134217728
        echoat $n [1;37m�A�յ۩��F�訫�A���O�o�Q���ФH���j�ﵷ�����Ӥ��o�ʼu�C[0m
        echoaround $n [1;37m�A�ݨ�$N[1;37m�յ۩��F�訫�A���o�Q�j�ﵷ�����Ӥ��o�ʼu�C[0m
        rcall dragonbay_para_check2 $n
        echo [1;30m�Ӿ�ӻj����]�]���Ӳ��ͤF�ǳ\�̰ʡI[0m
        rcall dragonbay_mload $n
else
        echoat $n �A�յ۩��F�訫�F�L�h�C
        echoaround $n �A�ݨ�$N���F�訫�F�L�h�C
        if mpnum($n) < 20
                echoat $n [1;34m�A���W���F�O�w�g�����l�ӡA�K�Ҥ]�b���������C[0m
                echoaround $n [1;34m�A�ݨ� $N [1;34m�~�����X�}�B�A���W���K�ҴN�����F�C[0m
                del_affect $n 'flaming armor'
                break
        endif
        if roomvalue($0) !& 8
                if rand(70)
                        echoat $n [1;32m�A���W���K�ұN�j����N���F�@�����C[0m
                        echoaround $n [1;32m�A�ݨ�j����Q $N [1;32m���K�ҿN���F�@�����C[0m
                        if roomtimer(1) == 0
				if mobcount(24209) > 0
                                	modrvt t1 = 24
				endif
                        endif
                        rcall dragonbay_east_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 8
                else
                        rcall dragonbay_east_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 16
                if rand(70)
                        echoat $n [1;33m�A���W���K�ұN�j������N���F�@�j�b�C[0m
                        echoaround $n [1;33m�A�ݨ�j����Q $N [1;33m���W���K�ҵ��N�F�@�j�b�C[0m
                        rcall dragonbay_east_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 16
                else
                        rcall dragonbay_east_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 32
                if rand(70)
                        echoat $n [1;36m�A���\���N�F�䪺�j������N�Ӻ���I�F��ש�i�H�q��F�C[0m
                        echoaround $n [1;36m�A�ݨ� $N [1;36m���\�a�N�F�誺�j����M���F�A�F��]�]���i�H�q��F�C[0m
                        rcall dragonbay_east_flaming_end $n
                        rcall dragonbay_east_linkroom $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 32
                else
                        rcall dragonbay_east_miss_message $n
                endif
        endif
endif
~
>room_defun dragonbay_west_linkroom_message~
echo [1;36m�A�ݨ�쥻���F��X�f���j�ﵷ��M���Q�N�ɡA�F��]�]���i�H�q��F�I[0m
modrvt v0 | 56
~
>room_defun dragonbay_west_linkroom_message2~
if roomvalue($0) & 56
        echo [1;36m�A�ݨ�F��쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 56
else
        if roomvalue($0) & 24
                echo [1;36m�A�ݨ�F��쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 24
        else
                echo [1;36m�A�ݨ�F��y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 8
        endif
endif
~
>room_defun dragonbay_east_flaming_end~
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
>room_defun dragonbay_east_miss_message~
echoat $n [1;34m�A�յ۬�V���O�H���Ъ��j�ﵷ�A�i���o�Q�j������ӥ��ѤF�C[0m
echoaround $n [1;34m�A�ݨ� $N [1;34m�b��X�}�B�S�h�[����K�Q�j������Ӥ�B���e�C[0m
rcall dragonbay_para_check $n
rcall dragonbay_east_flaming_end $n
~
|
