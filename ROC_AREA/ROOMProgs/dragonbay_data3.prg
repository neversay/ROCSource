>room_mask scout south~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout sout~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout sou~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout so~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout s~
rcall dragonbay_mask_scout_south_check $n
~
>room_defun dragonbay_mask_scout_south~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
        force $n scout south
        break
endif
if roomvalue($0) & 64
if roomvalue($0) & 128
if roomvalue($0) & 256
        force $n scout south
        break
endif
endif
endif
echoat $n [1;30m�n��Q�@��ФH���j�ﵷ�����F�A�ϱo�A�L�k���Q���氻��ʧ@�C[0m
echoaround $n [1;30m�A�ݨ� $N [1;30m���G�L���j�ﵷ���s�b�A���յ۩��n�谵����ʧ@�C[0m
~
>room_defun dragonbay_mask_south~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
	if ispc($n)
        	force $n south
        	break
	else
		transfer $n south
		break
	endif
endif
if roomvalue($0) & 64
if roomvalue($0) & 128
if roomvalue($0) & 256
	if ispc($n)
        	force $n south
        	break
	else
		transfer $n south
		break
	endif
endif
endif
endif
if roomvalue($0) !& 64
        rcall dragonbay_south_flaming $n
else
        if roomvalue($0) !& 128
                rcall dragonbay_south_flaming $n
        else
                rcall dragonbay_south_flaming $n
        endif
endif
~
>room_defun dragonbay_south_flaming~
if isaffected($n) !& 134217728
        echoat $n [1;37m�A�յ۩��n�訫�A���O�o�Q���ФH���j�ﵷ�����Ӥ��o�ʼu�C[0m
        echoaround $n [1;37m�A�ݨ�$N[1;37m�յ۩��n�訫�A���o�Q�j�ﵷ�����Ӥ��o�ʼu�C[0m
        rcall dragonbay_para_check2 $n
        echo [1;30m�Ӿ�ӻj����]�]���Ӳ��ͤF�ǳ\�̰ʡI[0m
        rcall dragonbay_mload $n
else
        echoat $n �A�յ۩��n�訫�F�L�h�C
        echoaround $n �A�ݨ�$N���n�訫�F�L�h�C
        if mpnum($n) < 20
                echoat $n [1;34m�A���W���F�O�w�g�����l�ӡA�K�Ҥ]�b���������C[0m
                echoaround $n [1;34m�A�ݨ� $N [1;34m�~�����X�}�B�A���W���K�ҴN�����F�C[0m
                del_affect $n 'flaming armor'
                break
        endif
        if roomvalue($0) !& 64
                if rand(70)
                        echoat $n [1;32m�A���W���K�ұN�j����N���F�@�����C[0m
                        echoaround $n [1;32m�A�ݨ�j����Q $N [1;32m���K�ҿN���F�@�����C[0m
                        if roomtimer(1) == 0
				if mobcount(24209) > 0
                                	modrvt t1 = 24
				endif
                        endif
                        rcall dragonbay_south_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 64
                else
                        rcall dragonbay_south_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 128
                if rand(70)
                        echoat $n [1;33m�A���W���K�ұN�j������N���F�@�j�b�C[0m
                        echoaround $n [1;33m�A�ݨ�j����Q $N [1;33m���W���K�ҵ��N�F�@�j�b�C[0m
                        rcall dragonbay_south_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 128
                else
                        rcall dragonbay_south_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 256
                if rand(70)
                        echoat $n [1;36m�A���\���N�n�䪺�j������N�Ӻ���I�n��ש�i�H�q��F�C[0m
                        echoaround $n [1;36m�A�ݨ� $N [1;36m���\�a�N�n�誺�j����M���F�A�n��]�]���i�H�q��F�C[0m
                        rcall dragonbay_south_flaming_end $n
                        rcall dragonbay_south_linkroom $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 256
                else
                        rcall dragonbay_south_miss_message $n
                endif
        endif
endif
~
>room_defun dragonbay_north_linkroom_message~
echo [1;36m�A�ݨ�쥻���n��X�f���j�ﵷ��M���Q�N�ɡA�n��]�]���i�H�q��F�I[0m
modrvt v0 | 448
~
>room_defun dragonbay_north_linkroom_message2~
if roomvalue($0) & 448
        echo [1;36m�A�ݨ�n��쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 448
else
        if roomvalue($0) & 192
                echo [1;36m�A�ݨ�n��쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 192
        else
                echo [1;36m�A�ݨ�n��y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 64
        endif
endif
~
>room_defun dragonbay_south_flaming_end~
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
>room_defun dragonbay_south_miss_message~
echoat $n [1;34m�A�յ۬�V���O�H���Ъ��j�ﵷ�A�i���o�Q�j������ӥ��ѤF�C[0m
echoaround $n [1;34m�A�ݨ� $N [1;34m�b��X�}�B�S�h�[����K�Q�j������Ӥ�B���e�C[0m
rcall dragonbay_para_check $n
rcall dragonbay_south_flaming_end $n
~
|
