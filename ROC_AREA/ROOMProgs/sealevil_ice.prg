>room_enter 127 33~
if ispc($n)
	if isaffected($n) / prot. from cold
		echoat $n �@�D�H���j�_�A���W���H�B������������F�H���A���Iŧ�C
	else
        	if rand(33)
                	echoat $n �쥻���R���}�]�N�������X�h��B�W�A���L�A������I
                	echoat $n ���M�A�S���줰��ˡA���O�A�oı�o���馳�I�H�N�C
                	add_affect $n 'glacier strike' 1140 10 01 -6 1073741824
        	else
                	if rand(50)
                        	echoat $n �ۥ|���K���Ӫ��H��O�A�����o�N�I
                        	echoat $n �����D������A�H�𤧤����G�����۳��H���]��......
                        	add_affect $n 'chill touch' 1140 10 43 -50 1073741824
                	else
                        	echoat $n �Aı�o�V�ӶV�Q�ΡA���֤]�V�ӶV���C
                        	echoat $n �ܧ֦a�A�A���z���w�g�Q�������ˡA�ߦw�F...ZZZZzzzz......
                        	add_affect $n 'sleep' 0 5 39 250 131072
                        	force $n sleep
			endif
                endif
        endif
endif
~
>room_defun sealevil_room_ice~
if ispc($r)
	if isaffected($r) / prot. from cold
		echoat $r �@�D�H���j�_�A���W���H�B������������F�H���A���Iŧ�C
	else
        	if rand(33)
                	echoat $r �쥻���R���}�]�N�������X�h��B�W�A���L�A������I
                	echoat $r ���M�A�S���줰��ˡA���O�A�oı�o���馳�I�H�N�C
                	add_affect $r 'glacier strike' 1140 10 01 -6 1073741824
        	else
                	if rand(50)
                        	echoat $r �ۥ|���K���Ӫ��H��O�A�����o�N�I
                        	echoat $r �����D������A�H�𤧤����G�����۳��H���]��......
                        	add_affect $r 'chill touch' 1140 10 43 -50 1073741824
                	else
                        	echoat $r �Aı�o�V�ӶV�Q�ΡA���֤]�V�ӶV���C
                        	echoat $r �ܧ֦a�A�A���z���w�g�Q�������ˡA�ߦw�F...ZZZZzzzz......
                        	add_affect $r 'sleep' 0 5 39 250 131072
                        	force $r sleep
			endif
                endif
        endif
endif
~
|