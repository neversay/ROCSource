>room_enter 127 60~
if ispc($n)
        if rand(33)
                echoat $n �@�����Ω��M�ۦ���D�X�A�r�F�A�@�f��S���i�򤤡C
                echoat $n �Aı�o�Q�r�쪺�a�観�I�ĳ¡A����n���]���ӵΪA�C
                add_affect $n 'poison' 1140 10 19 -25 4096
        else
                if rand(50)
                        echoat $n �A���ǩ��M�Q�X�@�ѷt���⪺����I
                        echoat $n �~�l�i�@�I�A�A�Nı�o�������A�ƦܷQ�R�C
                        add_affect $n 'disease' 1140 10 18 -25 1099511627776
                else
                        echoat $n �A���M�P�쨭��Y�B�ǨӤ@�}�µh�P�A�n���Q����F����C
			if isaffected($n) / adrenaline control
				echoat $n [1;36m�A�餺���ǤW�����b�����E�o�ӥX�A�ϧA�P������۷��Z�C[0m
				if rand(15)
					echoat $n [1;37m���]�b���ɡA�A���W���Ǹ��N����N���G�]�����F�C[0m
					del_affect $n 'adrenaline control'
				endif
			else
                        	echoat $n �����A�·��P�X�i��F�����A�V�|�F......
                        	add_affect $n 'paralyze' 0 5 39 200 268435456
			endif
                endif
        endif
endif
~
>room_defun sealevil_room_3f~
if ispc($r)
        if rand(33)
                echoat $r �@�����Ω��M�ۦ���D�X�A�r�F�A�@�f��S���i�򤤡C
                echoat $r �Aı�o�Q�r�쪺�a�観�I�ĳ¡A����n���]���ӵΪA�C
                add_affect $r 'poison' 1140 10 19 -25 4096
        else
                if rand(50)
                        echoat $r �A���ǩ��M�Q�X�@�ѷt���⪺����I
                        echoat $r �~�l�i�@�I�A�A�Nı�o�������A�ƦܷQ�R�C
                        add_affect $r 'disease' 1140 10 18 -25 1099511627776
                else
                        echoat $r �A���M�P�쨭��Y�B�ǨӤ@�}�µh�P�A�n���Q����F����C
			if isaffected($r) / adrenaline control
				echoat $r [1;36m�A�餺���ǤW�����b�����E�o�ӥX�A�ϧA�P������۷��Z�C[0m
				if rand(15)
					echoat $r [1;37m���]�b���ɡA�A���W���Ǹ��N����N���G�]�����F�C[0m
					del_affect $r 'adrenaline control'
				endif
			else
                        	echoat $r �����A�·��P�X�i��F�����A�V�|�F......
                        	add_affect $r 'paralyze' 0 5 39 200 268435456
			endif
                endif
        endif
endif
~
|