>room_defun dragonbay_timer~
if roomvalue($1) & 8
	break
endif
if mobcount(24209) == 0
	modrvt t0 = 0
	modrvt t1 = 0
	break
endif
if pcnum($) > 0
	if roomtimer($0) == 3
        	echo [1;37m�A�Pı��|�P�����n���G�V�ӶV�h�A�]�V�ӶV�j�n�C[0m
	endif
	if roomtimer($0) == 2
        	echo [1;32m�A�ݨ�@�j���]���j�勵�q�|���K��E���ӨӡA�ϱo�A���T�N�����y�C[0m
        	echo [1;32m���Y�]�}�l���w�D�N�A�N��O���F�ѩR�]�n�������}�o�Ӧa��I[0m
	endif
	if roomtimer($0) == 1
        	echo [1;36m�b�A�ٷW�����Q�޻L�N�]�ɡA�@�j���]���j��w�g�§A�ﭱ�ӨӤF�I[0m
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
       		rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	modrvt t0 = 0
	endif
else
	if roomtimer($0) > 60
		modrvt t0 = 1000
		modrvt v1 | 8
	else
		modrvt t0 + 2
	endif
endif
if roomvalue($0) == 0
        break
else
        if roomtimer(1) == 2
                echo [1;35m�A�ݨ�@���]���j�勵�}�l�b�R�ۻj�ﵷ�յۭ׸ɳQ�N�����j����C[0m
                echo [1;32m�өP�D����K�]�]���Ӳ��ͤF�ǳ\���̰ʡC[0m
		if pcnum($) > 0
                	rcall dragonbay_mload $r
                	rcall dragonbay_mload $r
		endif
        endif
        if roomtimer(1) == 1
                echo [1;35m�A�ݨ��h���]���j��}�l�E���b�A���W�Y�R�ۥզ⪺�j�ﵷ�A[0m
                echo [1;35m���@�|��\�Ҥw�g�}�l���j����N�Q�׸ɦn�F�C[0m
                echo [1;32m�Ө����F����h�]���j�諸��K�]�]���Ӳ��ͤF��j���̰ʡI[0m
		if pcnum($) > 0
                	rcall dragonbay_mload2 $r
                	rcall dragonbay_mload2 $r
                	rcall dragonbay_mload2 $r
		endif
                if roomvalue($0) & 1
                        rcall dragonbay_exit_fix_north
                        rcall dragonbay_north_linkroom2
                endif
                if roomvalue($0) & 8
                        rcall dragonbay_exit_fix_east
                        rcall dragonbay_east_linkroom2
                endif
                if roomvalue($0) & 64
                        rcall dragonbay_exit_fix_south
                        rcall dragonbay_south_linkroom2
                endif
                if roomvalue($0) & 512
                        rcall dragonbay_exit_fix_west
                        rcall dragonbay_west_linkroom2
                endif
        endif
endif
~
>room_defun dragonbay_exit_fix_north~
if roomvalue($0) & 7
        echo [1;32m�A�ݨ�_��쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 7
else
        if roomvalue($0) & 3
                echo [1;32m�A�ݨ�_��쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 3
        else
                echo [1;32m�A�ݨ�_��y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 1
        endif
endif
~
>room_defun dragonbay_exit_fix_east~
if roomvalue($0) & 56
        echo [1;32m�A�ݨ�F��쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 56
else
        if roomvalue($0) & 24
                echo [1;32m�A�ݨ�F��쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 24
        else
                echo [1;32m�A�ݨ�F��y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 8
        endif
endif
~
>room_defun dragonbay_exit_fix_south~
if roomvalue($0) & 448
        echo [1;32m�A�ݨ�n��쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 448
else
        if roomvalue($0) & 192
                echo [1;32m�A�ݨ�n��쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 192
        else
                echo [1;32m�A�ݨ�n��y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 64
        endif
endif
~
>room_defun dragonbay_exit_fix_west~
if roomvalue($0) & 3584
        echo [1;32m�A�ݨ����쥻�w�g�Q�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
        modrvt v0 r 3584
else
        if roomvalue($0) & 1536
                echo [1;32m�A�ݨ����쥻�w�g�ֳQ�N�ɪ��j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 1536
        else
                echo [1;32m�A�ݨ����y�Q�N�����j�ﵷ�S�Q�@�s�]���j�ﵹ�׸ɦn�F�C[0m
                modrvt v0 r 512
        endif
endif
~
>room_defun dragonbay_oload_skeleton~
if inroom($n) > 24211
if inroom($n) < 24269
	if rand(20)
		echoat $n �b�A�����ⰱ�U���B�����ɡA��Mť��N�㪺�@�n�C
		echoat $n �b�j����Q�N������A�쥻�Q�����䤤���e���������H�Y�����b�a�W�C
		echoaround $n �A�ݨ쨺�쥻�Q�ȵ��b�j����������e���������@�P�����b�a�W�C
		oload 24208 36 floor
		if rand(10)
			oload 14218 36 floor
		else
			if rand(11)
				oload 14219 36 floor
			else
				if rand(13)
					oload 14220 36 floor
				else
					if rand(14)
						oload 14221 36 floor
					else
						if rand(16)
							oload 14222 36 floor
						else
							if rand(20)
								oload 14223 36 floor
							else
								if rand(25)
									oload 14224 36 floor
								else
									if rand(34)
										oload 14225 36 floor
									else
										if rand(50)
											oload 14226 36 floor
										else
											oload 14227 36 floor
										endif
									endif
								endif
							endif
						endif
					endif
				endif
			endif
		endif
		if rand(20)
			if rand(10)
				oload 14218 36 floor
			else
				if rand(11)
					oload 14219 36 floor
				else
					if rand(13)
						oload 14220 36 floor
					else
						if rand(14)
							oload 14221 36 floor
						else
							if rand(16)
								oload 14222 36 floor
							else
								if rand(20)
									oload 14223 36 floor
								else
									if rand(25)
										oload 14224 36 floor
									else
										if rand(34)
											oload 14225 36 floor
										else
											if rand(50)
												oload 14226 36 floor
											else
												oload 14227 36 floor
											endif
										endif
									endif
								endif
							endif
						endif
					endif
				endif
			endif
		endif
		if rand(5)
			oload 14228 36 floor
		endif
		if rand(10)
			if rand(20)
				oload 14229 36 floor
			else
				if rand(25)
					oload 14230 36 floor
				else
					if rand(34)
						oload 14231 36 floor
					else
						if rand(50)
							oload 14232 36 floor
						else
							oload 14233 36 floor
						endif
					endif
				endif
			endif
		endif
	endif
endif
endif
~
>room_defun dragonbay_para_check~
if isaffected($n) / adrenaline control
        if rand(10)
                echoat $n [1;36m�A�P��������·��F�A�A�餺���ǤW�������G�S���o���X�@�ΡC[0m
                add_affect $n 'paralyze' 1130 3 39 300 268435456
        else
                echoat $n [1;36m�A�餺���ǤW�����b�����E�o�ӥX�A�ϧA�P������۷��Z�C[0m
                if rand(15)
                        echoat $n [1;37m���]�b���ɡA�A���W���Ǹ��N����N���G�]�����F�C[0m
                        del_affect $n 'adrenaline control'
                endif
        endif
else
        echoat $n [1;36m�b��Ĳ��j�ﵷ��S�h�[�A�A�K�Pı�������G���·��F�C[0m
        add_affect $n 'paralyze' 1130 5 39 300 268435456
endif
~
>room_defun dragonbay_para_check2~
if isaffected($n) / adrenaline control
        if rand(60)
                echoat $n [1;36m�A�P��������·��F�A�A�餺���ǤW�������G�S���o���X�@�ΡC[0m
                add_affect $n 'paralyze' 1130 5 39 300 268435456
        else
                echoat $n [1;36m�A�餺���ǤW�����b�����E�o�ӥX�A�ϧA�P������۷��Z�C[0m
                if rand(30)
                        echoat $n [1;37m���]�b���ɡA�A���W���Ǹ��N����N���G�]�����F�C[0m
                        del_affect $n 'adrenaline control'
                endif
        endif
else
        echoat $n [1;36m�b��Ĳ��j�ﵷ��S�h�[�A�A�K�Pı�������G���·��F�C[0m
        add_affect $n 'paralyze' 1130 8 39 300 268435456
endif
~
>room_enter 15 100~
if isimmort($n)
or mobcount(24209) == 0
or roomvalue($1) & 8
or roomvalue($1) & 64
        break
endif
if roomtimer($0) == 0
        modrvt t0 = 36
endif
if inroom($n) > 24210
if inroom($n) < 24269
        if mobcount(24210) == 0
                if rand(10)
                        callout 2 at 24200 mload 24210
                endif
        endif
endif
endif
modrvt v1 | 64
callout 4 modrvt v1 r 64
~
>room_mask exits default~
echoat $n �|�P�����O���ժ��j����A�A��b�O�L�k����X�U��V������F��C
~
>room_mask cast calm~
if rmobcount(24201) > 0
or rmobcount(24202) > 0
or rmobcount(24203) > 0
or rmobcount(24204) > 0
or rmobcount(24205) > 0
or rmobcount(24206) > 0
or rmobcount(24207) > 0
or rmobcount(24208) > 0
	echoat $n �P�D���H�w�g�Q�j���~�o���h�z���F�A�ϱo�A�L�k���L�����R�U�ӡC
else
	force $n cast calm
endif
~
>room_defun dragonbay_rand_message~
if rand(30)
	if mobcount(24209) > 0
		echoat $r �Ať��K�Y�W�ǨӤ@�}�s�s��媺�n���C
		break
	endif
else
	if rand(30)
		echoat $r �@�}�H���q�A���Ǩ�L�A���A�L�L���F�ӧNŸ�C
		break
	else
		if rand(40)
			if mobcount(24209) > 0
				echoat $r �b�e�W�誺�K���W�ϩ��{�L�F�@�Ӽv�l�C
				break
			endif
		else
			if rand(70)
				echo ��M��_�F�@�}�j���A�W�Y���K���]�]�����ͤF�ǳ\���̰ʡC
				rcall dragonbay_mload2 $r
				rcall dragonbay_mload2 $r
			else
				echo ��M��_�F�@�}�j���A�W�Y���K���]�]�����ͤF�@�P���̰ʡC
				rcall dragonbay_mload $r
				rcall dragonbay_mload $r
				rcall dragonbay_mload $r
				rcall dragonbay_mload $r
			endif
		endif
	endif
endif
~
>room_defun dragonbay_mload~
if mobcount(24209) != 0
        if rand(25)
                rcall dragonbay_mload_fun
        endif
endif
~
>room_defun dragonbay_mload2~
if mobcount(24209) != 0
	if rand(40)
        	rcall dragonbay_mload_fun
	endif
endif
~
>room_defun dragonbay_mload_fun~
if rand(25)
        if rand(50)
                echo [1;31m�A�ݨ�ѤW���U�F�@���]���j��I[0m
                mload 24201
        else
                echo [1;31m�A�ݨ�ѤW���U�F�@��[32m�]���j��I[0m
                mload 24202
        endif
else
        if rand(33)
                if rand(50)
                	echo [1;31m�A�ݨ�ѤW���U�F�@��[33m�]���j��I[0m
                        mload 24203
                else
                	echo [1;31m�A�ݨ�ѤW���U�F�@��[34m�]���j��I[0m
                        mload 24204
                endif
        else
                if rand(50)
                        if rand(50)
                		echo [1;31m�A�ݨ�ѤW���U�F�@��[35m�]���j��I[0m
                                mload 24205
                        else
                		echo [1;31m�A�ݨ�ѤW���U�F�@��[36m�]���j��I[0m
                                mload 24206
                        endif
                else
                        if rand(50)
                		echo [1;31m�A�ݨ�ѤW���U�F�@��[37m�]���j��I[0m
                                mload 24207
                        else
                		echo [1;31m�A�ݨ�ѤW���U�F�@��[30m�]���j��I[0m
                                mload 24208
                        endif
                endif
        endif
endif
~
|
