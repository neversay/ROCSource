>repop_prog 100~
mpadd_qmark self baccarat_dealer -1 0 0 0 0 1
mpadd_qmark self baccarat_player1 -1 0 0 0 0 1
mpadd_qmark self baccarat_player2 -1 0 0 0 0 1
mpadd_qmark self baccarat_player3 -1 0 0 0 0 1
mpadd_qmark self baccarat_player_point -1 0 0 0 0 1
mpadd_qmark self baccarat_banker1 -1 0 0 0 0 1
mpadd_qmark self baccarat_banker2 -1 0 0 0 0 1
mpadd_qmark self baccarat_banker3 -1 0 0 0 0 1
mpadd_qmark self baccarat_banker_point -1 0 0 0 0 1
mpadd_qmark self baccarat_random_timer -1 0 0 0 0 1
mpforcer here def_rembch $i
~
>rand_prog 100~
if is_qmark($i) == baccarat_random_timer 100002
	mpforcer here rcall baccarat_timeline $i
	mpmod_qmark self baccarat_random_timer v0 = 0
else
	if rand(80)
		if rand(90)
			mpforcer here rcall baccarat_timeline $i
		else
			mpmod_qmark self baccarat_random_timer v0 + 1
		endif
	else
		if rand(50)
			mpforcer here rcall baccarat_timeline $i
		else
			mpmod_qmark self baccarat_random_timer v0 + 1
		endif
	endif
endif
~
>defun_prog baccarat_interval_1~
mpecho $I�ֳt���q�P�������X�|�i�t�P�A�C�����P��̧ǵo�����a�A���a�A���a�A���a�C
mpcallout 1 mpecho �H��$I����½�}���a�β��a�л\���P...
mpcallout 1 mpcall baccarat_player_random_1 $i
mpcallout 1 mpcall baccarat_player_random_2 $i
mpcallout 1 mpcall baccarat_banker_random_1 $i
mpcallout 1 mpcall baccarat_banker_random_2 $i
mpcallout 2 mpcall baccarat_pp_check $i
mpcallout 2 mpcall baccarat_bp_check $i
mpcallout 2 mpcall baccarat_show_card $i
mpcallout 3 mpcall baccarat_p_3rd_check $i
mpcallout 3 mpcall baccarat_show_point $i
~
>defun_prog baccarat_show_bet~
mpechoat $n �@    PLAYER    �x    BANKER
mpechoat $n �@              �x
mpnonechoat $n �@
if is_qmark($i) > baccarat_player1 100000
	if is_qmark($i) == baccarat_player1 200001
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_player1 200002
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_player1 200003
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_player1 200004
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) & baccarat_player1 100001
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100002
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100004
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100008
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100016
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100032
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100064
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100128
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100256
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 100512
		mpnonechoat $n ���� [0m
	endif
	if is_qmark($i) & baccarat_player1 101024
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 102048
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player1 104096
		mpnonechoat $n �@�� [0m
	endif
else
	mpnonechoat $n �@   
endif
if is_qmark($i) > baccarat_player2 100000
	if is_qmark($i) == baccarat_player2 200001
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_player2 200002
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_player2 200003
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_player2 200004
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) & baccarat_player2 100001
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100002
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100004
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100008
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100016
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100032
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100064
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100128
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100256
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 100512
		mpnonechoat $n ���� [0m
	endif
	if is_qmark($i) & baccarat_player2 101024
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 102048
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_player2 104096
		mpnonechoat $n �@�� [0m
	endif
else
	mpnonechoat $n �@   
endif
if is_qmark($i) > baccarat_player3 100000
	if is_qmark($i) == baccarat_player3 200001
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_player3 200002
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_player3 200003
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_player3 200004
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) & baccarat_player3 100001
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100002
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100004
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100008
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100016
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100032
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100064
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100128
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100256
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 100512
		mpnonechoat $n ����[0m�x
	endif
	if is_qmark($i) & baccarat_player3 101024
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 102048
		mpnonechoat $n �@��[0m�x
	endif
	if is_qmark($i) & baccarat_player3 104096
		mpnonechoat $n �@��[0m�x
	endif
else
	mpnonechoat $n �@  �x
endif
if is_qmark($i) > baccarat_banker1 100000
	if is_qmark($i) == baccarat_banker1 200001
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_banker1 200002
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_banker1 200003
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_banker1 200004
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) & baccarat_banker1 100001
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100002
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100004
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100008
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100016
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100032
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100064
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100128
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100256
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 100512
		mpnonechoat $n ���� [0m
	endif
	if is_qmark($i) & baccarat_banker1 101024
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 102048
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker1 104096
		mpnonechoat $n �@�� [0m
	endif
else
	mpnonechoat $n �@ 
endif
if is_qmark($i) > baccarat_banker2 100000
	if is_qmark($i) == baccarat_banker2 200001
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_banker2 200002
		mpnonechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_banker2 200003
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_banker2 200004
		mpnonechoat $n [1;30m
	endif
	if is_qmark($i) & baccarat_banker2 100001
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100002
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100004
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100008
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100016
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100032
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100064
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100128
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100256
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 100512
		mpnonechoat $n ���� [0m
	endif
	if is_qmark($i) & baccarat_banker2 101024
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 102048
		mpnonechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker2 104096
		mpnonechoat $n �@�� [0m
	endif
else
	mpnonechoat $n �@ 
endif
if is_qmark($i) > baccarat_banker3 100000
	if is_qmark($i) == baccarat_banker3 200001
		mpechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_banker3 200002
		mpechoat $n [1;31m
	endif
	if is_qmark($i) == baccarat_banker3 200003
		mpechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_banker3 200004
		mpechoat $n [1;30m
	endif
	if is_qmark($i) == baccarat_banker3 100001
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100002
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100004
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100008
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100016
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100032
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100064
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100128
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100256
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 100512
		mpechoat $n ���� [0m
	endif
	if is_qmark($i) & baccarat_banker3 101024
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 102048
		mpechoat $n �@�� [0m
	endif
	if is_qmark($i) & baccarat_banker3 104096
		mpechoat $n �@�� [0m
	endif
else
	mpechoat $n �@
endif
mpechoat $n �@              �x
mpechoat $n �@
mpechoat $n �@�ݢw�w�w�w�ޢw�w�w�ޢw�w�w�w��
mpechoat $n �@�x PLAYER �x      �x BANKER �x
mpechoat $n �@�x  PIAR  �x TIE  �x  PAIR  �x
mpechoat $n �@�x        �x      �x        �x
if is_qmark($n) >= baccarat_player_pair 100001
	mpnonechoat $n �@�x   [1;32m��[0m   
else
	mpnonechoat $n �@�x        
endif
if is_qmark($n) >= baccarat_tie_game 100001
	mpnonechoat $n �x  [1;32m��[0m  
else
	mpnonechoat $n �x      
endif
if is_qmark($n) >= baccarat_banker_pair 100001
	mpnonechoat $n �x   [1;32m��[0m   
else
	mpnonechoat $n �x        
endif
mpechoat $n �x
mpechoat $n �@��w�w�w�w��w�w�w��w�w�w�w��
mpechoat $n �@�x                          �x
mpechoat $n �@�x         BANKER           �x
if is_qmark($n) >= baccarat_banker_win 100001
	mpechoat $n �@�x            [1;32m��[0m            �x
else
	mpechoat $n �@�x                          �x
endif
mpechoat $n �@��w�w�w�w�w�w�w�w�w�w�w�w�w��
mpechoat $n �@�x                          �x
mpechoat $n �@�x         PLAYER           �x
if is_qmark($n) >= baccarat_player_win 100001
	mpechoat $n �@�x            [1;32m��[0m            �x
else
	mpechoat $n �@�x                          �x
endif
mpechoat $n �@��w�w�w�w�w�w�w�w�w�w�w�w�w��
if is_qmark($n) == baccarat_banker_win 100001
	mpechoat $n �A�� �� �w�X����aĹ�C
endif
if is_qmark($n) == baccarat_banker_win 100002
	mpechoat $n �A�� �� �w�X����aĹ�C
endif
if is_qmark($n) == baccarat_banker_win 100003
	mpechoat $n �A�� �� �w�X����aĹ�C
endif
if is_qmark($n) == baccarat_banker_win 100004
	mpechoat $n �A�� �� �w�X����aĹ�C
endif
if is_qmark($n) == baccarat_banker_win 100005
	mpechoat $n �A�� �� �w�X����aĹ�C
endif
if is_qmark($n) == baccarat_player_win 100001
	mpechoat $n �A�� �� �w�X�䶢�aĹ�C
endif
if is_qmark($n) == baccarat_player_win 100002
	mpechoat $n �A�� �� �w�X�䶢�aĹ�C
endif
if is_qmark($n) == baccarat_player_win 100003
	mpechoat $n �A�� �� �w�X�䶢�aĹ�C
endif
if is_qmark($n) == baccarat_player_win 100004
	mpechoat $n �A�� �� �w�X�䶢�aĹ�C
endif
if is_qmark($n) == baccarat_player_win 100005
	mpechoat $n �A�� �� �w�X�䶢�aĹ�C
endif
if is_qmark($n) == baccarat_tie_game 100001
	mpechoat $n �A�� �� �w�X��o���M���C
endif
if is_qmark($n) == baccarat_tie_game 100002
	mpechoat $n �A�� �� �w�X��o���M���C
endif
if is_qmark($n) == baccarat_tie_game 100003
	mpechoat $n �A�� �� �w�X��o���M���C
endif
if is_qmark($n) == baccarat_tie_game 100004
	mpechoat $n �A�� �� �w�X��o���M���C
endif
if is_qmark($n) == baccarat_tie_game 100005
	mpechoat $n �A�� �� �w�X��o���M���C
endif
if is_qmark($n) == baccarat_banker_pair 100001
	mpechoat $n �A�� �� �w�X����a�X��lĹ�C
endif
if is_qmark($n) == baccarat_banker_pair 100002
	mpechoat $n �A�� �� �w�X����a�X��lĹ�C
endif
if is_qmark($n) == baccarat_banker_pair 100003
	mpechoat $n �A�� �� �w�X����a�X��lĹ�C
endif
if is_qmark($n) == baccarat_banker_pair 100004
	mpechoat $n �A�� �� �w�X����a�X��lĹ�C
endif
if is_qmark($n) == baccarat_banker_pair 100005
	mpechoat $n �A�� �� �w�X����a�X��lĹ�C
endif
if is_qmark($n) == baccarat_player_pair 100001
	mpechoat $n �A�� �� �w�X�䶢�a�X��lĹ�C
endif
if is_qmark($n) == baccarat_player_pair 100002
	mpechoat $n �A�� �� �w�X�䶢�a�X��lĹ�C
endif
if is_qmark($n) == baccarat_player_pair 100003
	mpechoat $n �A�� �� �w�X�䶢�a�X��lĹ�C
endif
if is_qmark($n) == baccarat_player_pair 100004
	mpechoat $n �A�� �� �w�X�䶢�a�X��lĹ�C
endif
if is_qmark($n) == baccarat_player_pair 100005
	mpechoat $n �A�� �� �w�X�䶢�a�X��lĹ�C
endif
~
>defun_prog baccarat_show_card~
if is_qmark($n) > baccarat_player1 100000
	mpnonecho ���a���G
	if is_qmark($n) == baccarat_player1 200001
		mpnonecho [1;31m����[0m
	endif
	if is_qmark($n) == baccarat_player1 200002
		mpnonecho [1;31m���[0m
	endif
	if is_qmark($n) == baccarat_player1 200003
		mpnonecho [1;30m�®�[0m
	endif
	if is_qmark($n) == baccarat_player1 200004
		mpnonecho [1;30m����[0m
	endif
	if is_qmark($n) & baccarat_player1 100001
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100002
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100004
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100008
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100016
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100032
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100064
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100128
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100256
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 100512
		mpnonecho [1;37m����[0m
	endif
	if is_qmark($n) & baccarat_player1 101024
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 102048
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player1 104096
		mpnonecho [1;37m��[0m
	endif
endif
if is_qmark($n) > baccarat_player2 100000
	mpnonecho  �B 
	if is_qmark($n) == baccarat_player2 200001
		mpnonecho [1;31m����[0m
	endif
	if is_qmark($n) == baccarat_player2 200002
		mpnonecho [1;31m���[0m
	endif
	if is_qmark($n) == baccarat_player2 200003
		mpnonecho [1;30m�®�[0m
	endif
	if is_qmark($n) == baccarat_player2 200004
		mpnonecho [1;30m����[0m
	endif
	if is_qmark($n) & baccarat_player2 100001
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100002
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100004
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100008
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100016
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100032
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100064
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100128
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100256
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 100512
		mpnonecho [1;37m����[0m
	endif
	if is_qmark($n) & baccarat_player2 101024
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 102048
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player2 104096
		mpnonecho [1;37m��[0m
	endif
endif
if is_qmark($n) > baccarat_player3 100000
	mpnonecho  �B 
	if is_qmark($n) == baccarat_player3 200001
		mpnonecho [1;31m����[0m
	endif
	if is_qmark($n) == baccarat_player3 200002
		mpnonecho [1;31m���[0m
	endif
	if is_qmark($n) == baccarat_player3 200003
		mpnonecho [1;30m�®�[0m
	endif
	if is_qmark($n) == baccarat_player3 200004
		mpnonecho [1;30m����[0m
	endif
	if is_qmark($n) & baccarat_player3 100001
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100002
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100004
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100008
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100016
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100032
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100064
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100128
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100256
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 100512
		mpnonecho [1;37m����[0m
	endif
	if is_qmark($n) & baccarat_player3 101024
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 102048
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_player3 104096
		mpnonecho [1;37m��[0m
	endif
endif
mpecho .
if is_qmark($n) > baccarat_banker1 100000
	mpnonecho ���a���G
	if is_qmark($n) == baccarat_banker1 200001
		mpnonecho [1;31m����[0m
	endif
	if is_qmark($n) == baccarat_banker1 200002
		mpnonecho [1;31m���[0m
	endif
	if is_qmark($n) == baccarat_banker1 200003
		mpnonecho [1;30m�®�[0m
	endif
	if is_qmark($n) == baccarat_banker1 200004
		mpnonecho [1;30m����[0m
	endif
	if is_qmark($n) & baccarat_banker1 100001
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100002
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100004
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100008
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100016
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100032
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100064
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100128
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100256
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 100512
		mpnonecho [1;37m����[0m
	endif
	if is_qmark($n) & baccarat_banker1 101024
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 102048
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker1 104096
		mpnonecho [1;37m��[0m
	endif
endif
if is_qmark($n) > baccarat_banker2 100000
	mpnonecho  �B 
	if is_qmark($n) == baccarat_banker2 200001
		mpnonecho [1;31m����[0m
	endif
	if is_qmark($n) == baccarat_banker2 200002
		mpnonecho [1;31m���[0m
	endif
	if is_qmark($n) == baccarat_banker2 200003
		mpnonecho [1;30m�®�[0m
	endif
	if is_qmark($n) == baccarat_banker2 200004
		mpnonecho [1;30m����[0m
	endif
	if is_qmark($n) & baccarat_banker2 100001
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100002
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100004
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100008
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100016
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100032
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100064
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100128
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100256
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 100512
		mpnonecho [1;37m����[0m
	endif
	if is_qmark($n) & baccarat_banker2 101024
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 102048
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker2 104096
		mpnonecho [1;37m��[0m
	endif
endif
if is_qmark($n) > baccarat_banker3 100000
	mpnonecho  �B 
	if is_qmark($n) == baccarat_banker3 200001
		mpnonecho [1;31m����[0m
	endif
	if is_qmark($n) == baccarat_banker3 200002
		mpnonecho [1;31m���[0m
	endif
	if is_qmark($n) == baccarat_banker3 200003
		mpnonecho [1;30m�®�[0m
	endif
	if is_qmark($n) == baccarat_banker3 200004
		mpnonecho [1;30m����[0m
	endif
	if is_qmark($n) & baccarat_banker3 100001
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100002
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100004
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100008
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100016
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100032
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100064
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100128
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100256
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 100512
		mpnonecho [1;37m����[0m
	endif
	if is_qmark($n) & baccarat_banker3 101024
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 102048
		mpnonecho [1;37m��[0m
	endif
	if is_qmark($n) & baccarat_banker3 104096
		mpnonecho [1;37m��[0m
	endif
endif
mpecho .
~
>defun_prog baccarat_show_card2~
if is_qmark($n) > baccarat_player3 100000
	mpnonecho $I�ֳt���q�P�������X�@�i 
	if is_qmark($n) == baccarat_player3 200001
		mpnonecho [1;31m����[0m 
	endif
	if is_qmark($n) == baccarat_player3 200002
		mpnonecho [1;31m���[0m 
	endif
	if is_qmark($n) == baccarat_player3 200003
		mpnonecho [1;30m�®�[0m 
	endif
	if is_qmark($n) == baccarat_player3 200004
		mpnonecho [1;30m����[0m 
	endif
	if is_qmark($n) & baccarat_player3 100001
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100002
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100004
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100008
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100016
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100032
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100064
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100128
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100256
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 100512
		mpnonecho [1;37m����[0m 
	endif
	if is_qmark($n) & baccarat_player3 101024
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 102048
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_player3 104096
		mpnonecho [1;37m��[0m 
	endif
endif
mpecho �o�����a�C
~
>defun_prog baccarat_show_card3~
if is_qmark($n) > baccarat_banker3 100000
	mpnonecho $I�ֳt���q�P�������X�@�i 
	if is_qmark($n) == baccarat_banker3 200001
		mpnonecho [1;31m����[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200002
		mpnonecho [1;31m���[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200003
		mpnonecho [1;30m�®�[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200004
		mpnonecho [1;30m����[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100001
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100002
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100004
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100008
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100016
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100032
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100064
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100128
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100256
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100512
		mpnonecho [1;37m����[0m 
	endif
	if is_qmark($n) & baccarat_banker3 101024
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 102048
		mpnonecho [1;37m��[0m 
	endif
	if is_qmark($n) & baccarat_banker3 104096
		mpnonecho [1;37m��[0m 
	endif
endif
mpecho �o�����a�C
mpcall baccarat_check_pair $i
mpcallout 1 mpcall baccarat_end_game $i
mpcallout 1 mpcall baccarat_show_point $i
~
>defun_prog baccarat_player_random_1~
if rand(25)
	mpmod_qmark $n baccarat_player1 v1 = 1
else
	if rand(33)
		mpmod_qmark $n baccarat_player1 v1 = 2
	else
		if rand(50)
			mpmod_qmark $n baccarat_player1 v1 = 3
		else
			mpmod_qmark $n baccarat_player1 v1 = 4
		endif
	endif
endif
if rand(7)
	mpmod_qmark self baccarat_player1 v0 | 1
else
	if rand(50)
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_player1 v0 | 2
			else
				if rand(50)
					mpmod_qmark self baccarat_player1 v0 | 4
				else
					mpmod_qmark self baccarat_player1 v0 | 8
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_player1 v0 | 16
			else
				if rand(50)
					mpmod_qmark self baccarat_player1 v0 | 32
				else
					mpmod_qmark self baccarat_player1 v0 | 64
				endif
			endif
		endif
	else
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_player1 v0 | 128
			else
				if rand(50)
					mpmod_qmark self baccarat_player1 v0 | 256
				else
					mpmod_qmark self baccarat_player1 v0 | 512
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_player1 v0 | 1024
			else
				if rand(50)
					mpmod_qmark self baccarat_player1 v0 | 2048
				else
					mpmod_qmark self baccarat_player1 v0 | 4096
				endif
			endif
		endif
	endif
endif
~
>defun_prog baccarat_player_random_2~
if rand(25)
	mpmod_qmark $n baccarat_player2 v1 = 1
else
	if rand(33)
		mpmod_qmark $n baccarat_player2 v1 = 2
	else
		if rand(50)
			mpmod_qmark $n baccarat_player2 v1 = 3
		else
			mpmod_qmark $n baccarat_player2 v1 = 4
		endif
	endif
endif
if rand(7)
	mpmod_qmark self baccarat_player2 v0 | 1
else
	if rand(50)
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_player2 v0 | 2
			else
				if rand(50)
					mpmod_qmark self baccarat_player2 v0 | 4
				else
					mpmod_qmark self baccarat_player2 v0 | 8
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_player2 v0 | 16
			else
				if rand(50)
					mpmod_qmark self baccarat_player2 v0 | 32
				else
					mpmod_qmark self baccarat_player2 v0 | 64
				endif
			endif
		endif
	else
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_player2 v0 | 128
			else
				if rand(50)
					mpmod_qmark self baccarat_player2 v0 | 256
				else
					mpmod_qmark self baccarat_player2 v0 | 512
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_player2 v0 | 1024
			else
				if rand(50)
					mpmod_qmark self baccarat_player2 v0 | 2048
				else
					mpmod_qmark self baccarat_player2 v0 | 4096
				endif
			endif
		endif
	endif
endif
~
>defun_prog baccarat_player_random_3~
if rand(25)
	mpmod_qmark $n baccarat_player3 v1 = 1
else
	if rand(33)
		mpmod_qmark $n baccarat_player3 v1 = 2
	else
		if rand(50)
			mpmod_qmark $n baccarat_player3 v1 = 3
		else
			mpmod_qmark $n baccarat_player3 v1 = 4
		endif
	endif
endif
if rand(7)
	mpmod_qmark self baccarat_player3 v0 | 1
else
	if rand(50)
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_player3 v0 | 2
			else
				if rand(50)
					mpmod_qmark self baccarat_player3 v0 | 4
				else
					mpmod_qmark self baccarat_player3 v0 | 8
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_player3 v0 | 16
			else
				if rand(50)
					mpmod_qmark self baccarat_player3 v0 | 32
				else
					mpmod_qmark self baccarat_player3 v0 | 64
				endif
			endif
		endif
	else
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_player3 v0 | 128
			else
				if rand(50)
					mpmod_qmark self baccarat_player3 v0 | 256
				else
					mpmod_qmark self baccarat_player3 v0 | 512
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_player3 v0 | 1024
			else
				if rand(50)
					mpmod_qmark self baccarat_player3 v0 | 2048
				else
					mpmod_qmark self baccarat_player3 v0 | 4096
				endif
			endif
		endif
	endif
endif
mpcall baccarat_pp_check2 $i
~
>defun_prog baccarat_banker_random_1~
if rand(25)
	mpmod_qmark $n baccarat_banker1 v1 = 1
else
	if rand(33)
		mpmod_qmark $n baccarat_banker1 v1 = 2
	else
		if rand(50)
			mpmod_qmark $n baccarat_banker1 v1 = 3
		else
			mpmod_qmark $n baccarat_banker1 v1 = 4
		endif
	endif
endif
if rand(7)
	mpmod_qmark self baccarat_banker1 v0 | 1
else
	if rand(50)
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_banker1 v0 | 2
			else
				if rand(50)
					mpmod_qmark self baccarat_banker1 v0 | 4
				else
					mpmod_qmark self baccarat_banker1 v0 | 8
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_banker1 v0 | 16
			else
				if rand(50)
					mpmod_qmark self baccarat_banker1 v0 | 32
				else
					mpmod_qmark self baccarat_banker1 v0 | 64
				endif
			endif
		endif
	else
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_banker1 v0 | 128
			else
				if rand(50)
					mpmod_qmark self baccarat_banker1 v0 | 256
				else
					mpmod_qmark self baccarat_banker1 v0 | 512
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_banker1 v0 | 1024
			else
				if rand(50)
					mpmod_qmark self baccarat_banker1 v0 | 2048
				else
					mpmod_qmark self baccarat_banker1 v0 | 4096
				endif
			endif
		endif
	endif
endif
~
>defun_prog baccarat_banker_random_2~
if rand(25)
	mpmod_qmark $n baccarat_banker2 v1 = 1
else
	if rand(33)
		mpmod_qmark $n baccarat_banker2 v1 = 2
	else
		if rand(50)
			mpmod_qmark $n baccarat_banker2 v1 = 3
		else
			mpmod_qmark $n baccarat_banker2 v1 = 4
		endif
	endif
endif
if rand(7)
	mpmod_qmark self baccarat_banker2 v0 | 1
else
	if rand(50)
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_banker2 v0 | 2
			else
				if rand(50)
					mpmod_qmark self baccarat_banker2 v0 | 4
				else
					mpmod_qmark self baccarat_banker2 v0 | 8
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_banker2 v0 | 16
			else
				if rand(50)
					mpmod_qmark self baccarat_banker2 v0 | 32
				else
					mpmod_qmark self baccarat_banker2 v0 | 64
				endif
			endif
		endif
	else
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_banker2 v0 | 128
			else
				if rand(50)
					mpmod_qmark self baccarat_banker2 v0 | 256
				else
					mpmod_qmark self baccarat_banker2 v0 | 512
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_banker2 v0 | 1024
			else
				if rand(50)
					mpmod_qmark self baccarat_banker2 v0 | 2048
				else
					mpmod_qmark self baccarat_banker2 v0 | 4096
				endif
			endif
		endif
	endif
endif
~
>defun_prog baccarat_banker_random_3~
if rand(25)
	mpmod_qmark $n baccarat_banker3 v1 = 1
else
	if rand(33)
		mpmod_qmark $n baccarat_banker3 v1 = 2
	else
		if rand(50)
			mpmod_qmark $n baccarat_banker3 v1 = 3
		else
			mpmod_qmark $n baccarat_banker3 v1 = 4
		endif
	endif
endif
if rand(7)
	mpmod_qmark self baccarat_banker3 v0 | 1
else
	if rand(50)
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_banker3 v0 | 2
			else
				if rand(50)
					mpmod_qmark self baccarat_banker3 v0 | 4
				else
					mpmod_qmark self baccarat_banker3 v0 | 8
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_banker3 v0 | 16
			else
				if rand(50)
					mpmod_qmark self baccarat_banker3 v0 | 32
				else
					mpmod_qmark self baccarat_banker3 v0 | 64
				endif
			endif
		endif
	else
		if rand(50)
			if rand(33)
				mpmod_qmark self baccarat_banker3 v0 | 128
			else
				if rand(50)
					mpmod_qmark self baccarat_banker3 v0 | 256
				else
					mpmod_qmark self baccarat_banker3 v0 | 512
				endif
			endif
		else
			if rand(33)
				mpmod_qmark self baccarat_banker3 v0 | 1024
			else
				if rand(50)
					mpmod_qmark self baccarat_banker3 v0 | 2048
				else
					mpmod_qmark self baccarat_banker3 v0 | 4096
				endif
			endif
		endif
	endif
endif
mpcall baccarat_bp_check2 $i
~
>defun_prog baccarat_pp_check~
if is_qmark($i) & baccarat_player1 100001
	mpmod_qmark self baccarat_player_point v0 + 1
endif
if is_qmark($i) & baccarat_player1 100002
	mpmod_qmark self baccarat_player_point v0 + 2
endif
if is_qmark($i) & baccarat_player1 100004
	mpmod_qmark self baccarat_player_point v0 + 3
endif
if is_qmark($i) & baccarat_player1 100008
	mpmod_qmark self baccarat_player_point v0 + 4
endif
if is_qmark($i) & baccarat_player1 100016
	mpmod_qmark self baccarat_player_point v0 + 5
endif
if is_qmark($i) & baccarat_player1 100032
	mpmod_qmark self baccarat_player_point v0 + 6
endif
if is_qmark($i) & baccarat_player1 100064
	mpmod_qmark self baccarat_player_point v0 + 7
endif
if is_qmark($i) & baccarat_player1 100128
	mpmod_qmark self baccarat_player_point v0 + 8
endif
if is_qmark($i) & baccarat_player1 100256
	mpmod_qmark self baccarat_player_point v0 + 9
endif
if is_qmark($i) & baccarat_player2 100001
	mpmod_qmark self baccarat_player_point v0 + 1
endif
if is_qmark($i) & baccarat_player2 100002
	mpmod_qmark self baccarat_player_point v0 + 2
endif
if is_qmark($i) & baccarat_player2 100004
	mpmod_qmark self baccarat_player_point v0 + 3
endif
if is_qmark($i) & baccarat_player2 100008
	mpmod_qmark self baccarat_player_point v0 + 4
endif
if is_qmark($i) & baccarat_player2 100016
	mpmod_qmark self baccarat_player_point v0 + 5
endif
if is_qmark($i) & baccarat_player2 100032
	mpmod_qmark self baccarat_player_point v0 + 6
endif
if is_qmark($i) & baccarat_player2 100064
	mpmod_qmark self baccarat_player_point v0 + 7
endif
if is_qmark($i) & baccarat_player2 100128
	mpmod_qmark self baccarat_player_point v0 + 8
endif
if is_qmark($i) & baccarat_player2 100256
	mpmod_qmark self baccarat_player_point v0 + 9
endif
if is_qmark($i) == baccarat_player_point 100018
	mpmod_qmark self baccarat_player_point v0 = 8
endif
if is_qmark($i) == baccarat_player_point 100017
	mpmod_qmark self baccarat_player_point v0 = 7
endif
if is_qmark($i) == baccarat_player_point 100016
	mpmod_qmark self baccarat_player_point v0 = 6
endif
if is_qmark($i) == baccarat_player_point 100015
	mpmod_qmark self baccarat_player_point v0 = 5
endif
if is_qmark($i) == baccarat_player_point 100014
	mpmod_qmark self baccarat_player_point v0 = 4
endif
if is_qmark($i) == baccarat_player_point 100013
	mpmod_qmark self baccarat_player_point v0 = 3
endif
if is_qmark($i) == baccarat_player_point 100012
	mpmod_qmark self baccarat_player_point v0 = 2
endif
if is_qmark($i) == baccarat_player_point 100011
	mpmod_qmark self baccarat_player_point v0 = 1
endif
if is_qmark($i) == baccarat_player_point 100010
	mpmod_qmark self baccarat_player_point v0 = 0
endif
~
>defun_prog baccarat_bp_check~
if is_qmark($i) & baccarat_banker1 100001
	mpmod_qmark self baccarat_banker_point v0 + 1
endif
if is_qmark($i) & baccarat_banker1 100002
	mpmod_qmark self baccarat_banker_point v0 + 2
endif
if is_qmark($i) & baccarat_banker1 100004
	mpmod_qmark self baccarat_banker_point v0 + 3
endif
if is_qmark($i) & baccarat_banker1 100008
	mpmod_qmark self baccarat_banker_point v0 + 4
endif
if is_qmark($i) & baccarat_banker1 100016
	mpmod_qmark self baccarat_banker_point v0 + 5
endif
if is_qmark($i) & baccarat_banker1 100032
	mpmod_qmark self baccarat_banker_point v0 + 6
endif
if is_qmark($i) & baccarat_banker1 100064
	mpmod_qmark self baccarat_banker_point v0 + 7
endif
if is_qmark($i) & baccarat_banker1 100128
	mpmod_qmark self baccarat_banker_point v0 + 8
endif
if is_qmark($i) & baccarat_banker1 100256
	mpmod_qmark self baccarat_banker_point v0 + 9
endif
if is_qmark($i) & baccarat_banker2 100001
	mpmod_qmark self baccarat_banker_point v0 + 1
endif
if is_qmark($i) & baccarat_banker2 100002
	mpmod_qmark self baccarat_banker_point v0 + 2
endif
if is_qmark($i) & baccarat_banker2 100004
	mpmod_qmark self baccarat_banker_point v0 + 3
endif
if is_qmark($i) & baccarat_banker2 100008
	mpmod_qmark self baccarat_banker_point v0 + 4
endif
if is_qmark($i) & baccarat_banker2 100016
	mpmod_qmark self baccarat_banker_point v0 + 5
endif
if is_qmark($i) & baccarat_banker2 100032
	mpmod_qmark self baccarat_banker_point v0 + 6
endif
if is_qmark($i) & baccarat_banker2 100064
	mpmod_qmark self baccarat_banker_point v0 + 7
endif
if is_qmark($i) & baccarat_banker2 100128
	mpmod_qmark self baccarat_banker_point v0 + 8
endif
if is_qmark($i) & baccarat_banker2 100256
	mpmod_qmark self baccarat_banker_point v0 + 9
endif
if is_qmark($i) == baccarat_banker_point 100018
	mpmod_qmark self baccarat_banker_point v0 = 8
endif
if is_qmark($i) == baccarat_banker_point 100017
	mpmod_qmark self baccarat_banker_point v0 = 7
endif
if is_qmark($i) == baccarat_banker_point 100016
	mpmod_qmark self baccarat_banker_point v0 = 6
endif
if is_qmark($i) == baccarat_banker_point 100015
	mpmod_qmark self baccarat_banker_point v0 = 5
endif
if is_qmark($i) == baccarat_banker_point 100014
	mpmod_qmark self baccarat_banker_point v0 = 4
endif
if is_qmark($i) == baccarat_banker_point 100013
	mpmod_qmark self baccarat_banker_point v0 = 3
endif
if is_qmark($i) == baccarat_banker_point 100012
	mpmod_qmark self baccarat_banker_point v0 = 2
endif
if is_qmark($i) == baccarat_banker_point 100011
	mpmod_qmark self baccarat_banker_point v0 = 1
endif
if is_qmark($i) == baccarat_banker_point 100010
	mpmod_qmark self baccarat_banker_point v0 = 0
endif
~
>defun_prog baccarat_pp_check2~
if is_qmark($i) & baccarat_player3 100001
	mpmod_qmark self baccarat_player_point v0 + 1
endif
if is_qmark($i) & baccarat_player3 100002
	mpmod_qmark self baccarat_player_point v0 + 2
endif
if is_qmark($i) & baccarat_player3 100004
	mpmod_qmark self baccarat_player_point v0 + 3
endif
if is_qmark($i) & baccarat_player3 100008
	mpmod_qmark self baccarat_player_point v0 + 4
endif
if is_qmark($i) & baccarat_player3 100016
	mpmod_qmark self baccarat_player_point v0 + 5
endif
if is_qmark($i) & baccarat_player3 100032
	mpmod_qmark self baccarat_player_point v0 + 6
endif
if is_qmark($i) & baccarat_player3 100064
	mpmod_qmark self baccarat_player_point v0 + 7
endif
if is_qmark($i) & baccarat_player3 100128
	mpmod_qmark self baccarat_player_point v0 + 8
endif
if is_qmark($i) & baccarat_player3 100256
	mpmod_qmark self baccarat_player_point v0 + 9
endif
if is_qmark($i) == baccarat_player_point 100018
	mpmod_qmark self baccarat_player_point v0 = 8
endif
if is_qmark($i) == baccarat_player_point 100017
	mpmod_qmark self baccarat_player_point v0 = 7
endif
if is_qmark($i) == baccarat_player_point 100016
	mpmod_qmark self baccarat_player_point v0 = 6
endif
if is_qmark($i) == baccarat_player_point 100015
	mpmod_qmark self baccarat_player_point v0 = 5
endif
if is_qmark($i) == baccarat_player_point 100014
	mpmod_qmark self baccarat_player_point v0 = 4
endif
if is_qmark($i) == baccarat_player_point 100013
	mpmod_qmark self baccarat_player_point v0 = 3
endif
if is_qmark($i) == baccarat_player_point 100012
	mpmod_qmark self baccarat_player_point v0 = 2
endif
if is_qmark($i) == baccarat_player_point 100011
	mpmod_qmark self baccarat_player_point v0 = 1
endif
if is_qmark($i) == baccarat_player_point 100010
	mpmod_qmark self baccarat_player_point v0 = 0
endif
mpcall baccarat_show_card2 $i
~
>defun_prog baccarat_bp_check2~
if is_qmark($i) & baccarat_banker3 100001
	mpmod_qmark self baccarat_banker_point v0 + 1
endif
if is_qmark($i) & baccarat_banker3 100002
	mpmod_qmark self baccarat_banker_point v0 + 2
endif
if is_qmark($i) & baccarat_banker3 100004
	mpmod_qmark self baccarat_banker_point v0 + 3
endif
if is_qmark($i) & baccarat_banker3 100008
	mpmod_qmark self baccarat_banker_point v0 + 4
endif
if is_qmark($i) & baccarat_banker3 100016
	mpmod_qmark self baccarat_banker_point v0 + 5
endif
if is_qmark($i) & baccarat_banker3 100032
	mpmod_qmark self baccarat_banker_point v0 + 6
endif
if is_qmark($i) & baccarat_banker3 100064
	mpmod_qmark self baccarat_banker_point v0 + 7
endif
if is_qmark($i) & baccarat_banker3 100128
	mpmod_qmark self baccarat_banker_point v0 + 8
endif
if is_qmark($i) & baccarat_banker3 100256
	mpmod_qmark self baccarat_banker_point v0 + 9
endif
if is_qmark($i) == baccarat_banker_point 100018
	mpmod_qmark self baccarat_banker_point v0 = 8
endif
if is_qmark($i) == baccarat_banker_point 100017
	mpmod_qmark self baccarat_banker_point v0 = 7
endif
if is_qmark($i) == baccarat_banker_point 100016
	mpmod_qmark self baccarat_banker_point v0 = 6
endif
if is_qmark($i) == baccarat_banker_point 100015
	mpmod_qmark self baccarat_banker_point v0 = 5
endif
if is_qmark($i) == baccarat_banker_point 100014
	mpmod_qmark self baccarat_banker_point v0 = 4
endif
if is_qmark($i) == baccarat_banker_point 100013
	mpmod_qmark self baccarat_banker_point v0 = 3
endif
if is_qmark($i) == baccarat_banker_point 100012
	mpmod_qmark self baccarat_banker_point v0 = 2
endif
if is_qmark($i) == baccarat_banker_point 100011
	mpmod_qmark self baccarat_banker_point v0 = 1
endif
if is_qmark($i) == baccarat_banker_point 100010
	mpmod_qmark self baccarat_banker_point v0 = 0
endif
mpcall baccarat_show_card3 $i
~
>defun_prog baccarat_check_pair~
if is_qmark($i) & baccarat_player1 100001
	if is_qmark($i) & baccarat_player2 100001
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100002
	if is_qmark($i) & baccarat_player2 100002
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100004
	if is_qmark($i) & baccarat_player2 100004
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100008
	if is_qmark($i) & baccarat_player2 100008
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100016
	if is_qmark($i) & baccarat_player2 100016
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100032
	if is_qmark($i) & baccarat_player2 100032
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100064
	if is_qmark($i) & baccarat_player2 100064
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100128
	if is_qmark($i) & baccarat_player2 100128
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100256
	if is_qmark($i) & baccarat_player2 100256
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 100512
	if is_qmark($i) & baccarat_player2 100512
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 101024
	if is_qmark($i) & baccarat_player2 101024
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 102048
	if is_qmark($i) & baccarat_player2 102048
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_player1 104096
	if is_qmark($i) & baccarat_player2 104096
		mpforcer here modrvt v0 | 8
	endif
endif
if is_qmark($i) & baccarat_banker1 100001
	if is_qmark($i) & baccarat_banker2 100001
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100002
	if is_qmark($i) & baccarat_banker2 100002
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100004
	if is_qmark($i) & baccarat_banker2 100004
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100008
	if is_qmark($i) & baccarat_banker2 100008
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100016
	if is_qmark($i) & baccarat_banker2 100016
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100032
	if is_qmark($i) & baccarat_banker2 100032
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100064
	if is_qmark($i) & baccarat_banker2 100064
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100128
	if is_qmark($i) & baccarat_banker2 100128
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100256
	if is_qmark($i) & baccarat_banker2 100256
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 100512
	if is_qmark($i) & baccarat_banker2 100512
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 101024
	if is_qmark($i) & baccarat_banker2 101024
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 102048
	if is_qmark($i) & baccarat_banker2 102048
		mpforcer here modrvt v0 | 16
	endif
endif
if is_qmark($i) & baccarat_banker1 104096
	if is_qmark($i) & baccarat_banker2 104096
		mpforcer here modrvt v0 | 16
	endif
endif
~
>defun_prog baccarat_p_3rd_check~
if is_qmark($i) == baccarat_player_point 100008
or is_qmark($i) == baccarat_player_point 100009
or is_qmark($i) == baccarat_banker_point 100008
or is_qmark($i) == baccarat_banker_point 100009
	mpcall baccarat_check_pair $i
	mpcall baccarat_end_game $i
	break
endif
if is_qmark($i) == baccarat_player_point 100000
or is_qmark($i) == baccarat_player_point 100001
or is_qmark($i) == baccarat_player_point 100002
or is_qmark($i) == baccarat_player_point 100003
or is_qmark($i) == baccarat_player_point 100004
or is_qmark($i) == baccarat_player_point 100005
	say ���a�ɵP�C
	mpcall baccarat_player_random_3 $i
	mpcallout 1 mpcall baccarat_b_3rd_check $i
	break
endif
mpcall baccarat_b_3rd_check $i
~
>defun_prog baccarat_b_3rd_check~
if is_qmark($i) == baccarat_banker_point 100000
or is_qmark($i) == baccarat_banker_point 100001
or is_qmark($i) == baccarat_banker_point 100002
	say ���a�ɵP�C
	mpcall baccarat_banker_random_3 $i
	break
endif
if is_qmark($i) == baccarat_banker_point 100003
	if is_qmark($i) & baccarat_player3 100128
	else
		say ���a�ɵP�C
		mpcall baccarat_banker_random_3 $i
		break
	endif
endif
if is_qmark($i) == baccarat_banker_point 100004
	if is_qmark($i) & baccarat_player3 100001
	or is_qmark($i) & baccarat_player3 100128
	or is_qmark($i) & baccarat_player3 100256
	or is_qmark($i) & baccarat_player3 100512
	or is_qmark($i) & baccarat_player3 101024
	or is_qmark($i) & baccarat_player3 102048
	or is_qmark($i) & baccarat_player3 104096
	else
		say ���a�ɵP�C
		mpcall baccarat_banker_random_3 $i
		break
	endif
endif
if is_qmark($i) == baccarat_banker_point 100005
	if is_qmark($i) & baccarat_player3 100001
	or is_qmark($i) & baccarat_player3 100002
	or is_qmark($i) & baccarat_player3 100004
	or is_qmark($i) & baccarat_player3 100128
	or is_qmark($i) & baccarat_player3 100256
	or is_qmark($i) & baccarat_player3 100512
	or is_qmark($i) & baccarat_player3 101024
	or is_qmark($i) & baccarat_player3 102048
	or is_qmark($i) & baccarat_player3 104096
	else
		say ���a�ɵP�C
		mpcall baccarat_banker_random_3 $i
		break
	endif
endif
if is_qmark($i) == baccarat_banker_point 100006
	if is_qmark($i) > baccarat_player3 100000
		if is_qmark($i) & baccarat_player3 100032
		or is_qmark($i) & baccarat_player3 100064
			say ���a�ɵP�C
			mpcall baccarat_banker_random_3 $i
			break
		endif
	endif
endif
if is_qmark($i) > baccarat_player3 100000
	mpcall baccarat_check_pair $i
	mpcall baccarat_show_point $i
	mpcall baccarat_end_game $i
else
	mpcall baccarat_check_pair $i
	mpcall baccarat_end_game $i
endif
~
>defun_prog baccarat_show_point~
if is_qmark($i) == baccarat_player_point 100000
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100001
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100002
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100003
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100004
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100005
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100006
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100007
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100008
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_player_point 100009
	mpnonecho $I[1;32m���ۡu���a [1;37m��[1;32m �I�A[0m
endif
if is_qmark($i) == baccarat_banker_point 100000
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100001
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100002
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100003
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100004
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100005
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100006
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100007
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100008
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
if is_qmark($i) == baccarat_banker_point 100009
	mpecho [1;32m���a [1;37m��[1;32m �I�C�v[0m
endif
~
>defun_prog baccarat_end_game~
if is_qmark($i) == baccarat_player_point 100000
	if is_qmark($i) > baccarat_banker_point 100000
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100000
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
endif
if is_qmark($i) == baccarat_player_point 100001
	if is_qmark($i) > baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100002
	if is_qmark($i) > baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100003
	if is_qmark($i) > baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100004
	if is_qmark($i) > baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100005
	if is_qmark($i) > baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100006
	if is_qmark($i) > baccarat_banker_point 100006
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100006
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100006
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100007
	if is_qmark($i) > baccarat_banker_point 100007
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100007
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100007
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100008
	if is_qmark($i) > baccarat_banker_point 100008
		say ���aĹ�C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100008
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100008
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100009
	if is_qmark($i) == baccarat_banker_point 100009
		say �M���C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100009
		say ���aĹ�C
		mpforcer here modrvt v0 | 2
	endif
endif
mpforcer here rcall baccarat_ending_transfor $i
~
>defun_prog baccarat_reset_all~
mpecho �C��������$I��z���ୱ�A�ǳƶ}�l�U�@���C
mpmod_qmark self baccarat_dealer v0 = 0
mpmod_qmark self baccarat_player1 v0 = 0
mpmod_qmark self baccarat_player1 v1 = 0
mpmod_qmark self baccarat_player2 v0 = 0
mpmod_qmark self baccarat_player2 v1 = 0
mpmod_qmark self baccarat_player3 v0 = 0
mpmod_qmark self baccarat_player3 v1 = 0
mpmod_qmark self baccarat_player_point v0 = 0
mpmod_qmark self baccarat_banker1 v0 = 0
mpmod_qmark self baccarat_banker1 v1 = 0
mpmod_qmark self baccarat_banker2 v0 = 0
mpmod_qmark self baccarat_banker2 v1 = 0
mpmod_qmark self baccarat_banker3 v0 = 0
mpmod_qmark self baccarat_banker3 v1 = 0
mpmod_qmark self baccarat_banker_point v0 = 0
mpforcer here modrvt v0 = 0
mpforcer here modrvt v1 = 0
mpforcer here modrvt t0 = 0
~
>talk_prog topic~
tell $n �Q�F�Ѧʮa�֪����k�H(baccarat-rule)
~
>talk_prog p baccarat-rule~
tell $n �ʮa�֬O�@�ا����L�{�����Ѳ��x�i�檺�C���A�ѥ[�����a�u�ݭn�U�`��Q�n����m�N��F�C
tell $n �U�`����k�� bet '��m �S�X�ƶq'�A�i�H�C�Ӧ�m���U�`�A���P���C�����C�H�̦h�U�` 5 �S�X���W���C
tell $n �b�C�������H�ɨϥ� check �Ӭݦۤv�U�`�����A�Υثe�}�P�����p�C
tell $n �U�`��m���G���aĹ(Banker)�B���aĹ(Player)�B�M��(Tie)�B���a�X��l(Banker-Pair)�B���a�X��l(Player-Pair)�C
tell $n �u�n�U�`���\�Y�i�[�J�ӳ��C���C
tell $n �p�G�Q�F�ѧ�h�ʮa�֬������W�h�i�H�~��ݧڡC(baccarat-rule2)
~
>talk_prog p baccarat-rule2~
tell $n �ʮa�֬O�@�إH���B���I�Ƥ�j�p���ȵP�C���A�I���`�X�j���@���Ĺ�o���ɡC
tell $n �Ʀr�P������ �� �� �� ���I�ƻP�������Ʀr�ۦP�A�� ���� �h�N�� 0 �I�C
tell $n ��r�P�������A�� �N�� 1 �I�A�ءB�ߡB�� �h�N�� 0 �I�C
tell $n �I�ƭp��O�N���a�ζ��a�U�۫�������P�I�ƥ[�`�A���[�_�ӳ̤j����W�L 9�C
tell $n �p�G�`�X�W�L 9 �h�H�`�X�᪺�Ӧ�Ʒ��̲ת��I�ơC
tell $n �ҡG�� + �� = 0 �I, �� + �� = 8 �I, ���� + �� = 0 �I, �� + �� = 4 �I�C
tell $n ²������A�b�C�����̤j���I�ƬO 9 �A�̤p���h�� 0 �I�C
tell $n ------------------------------------------------------------------------------
tell $n �ʮa�֦b�C���}�l�i��ɡA���x�|���O���P��i�P�ܶ��a(player)�P���a(banker)�����W�åB�}�P�C
tell $n �}�P��i��Ĥ@�����I�ƥ[�`�åB�P�w�ұo���I�ƬO�_�i��S��W�h�ɵP�C
tell $n ����ɵP�̦h�ȷ|���@�i�A²������A�p�G�ݭn�ɵP�A�b��������ĤT�i�P���ɭԥ��w�|�����G�C
tell $n �ӬO�_�ݭn�ɵP�A�h�Ѷ}�P�᪺�I�ƨӧP�w�G
tell $n ------------------------------------------------------------------------------
tell $n �u�n���B�����@�X�{��i�P���I�ƩM�� 8 �� 9 �ɡA���賣���i��ɵP�A��������I�ƫᵲ���C���C
tell $n ���a�b�I�Ƥp�� 5 �����p�U�A�ݭn�ɵP�A�j�� 6 �h���ɵP�C
tell $n ���a�b�I�Ƥp�� 2 �����p�U�A�ݭn�ɵP�C
tell $n ���a�I�Ƭ� 3 �ɡA���a�ݭn�ɵP�A���F���a���ĤT�i�P�� 8 �I�ɡA���a���ݭn�ɵP�C
tell $n ���a�I�Ƭ� 4 �ɡA���a���ĤT�i�P�� 0, 1, 8, 9 �ɡA���a���ݸɵP�A��l���p�n�ɵP�C
tell $n ���a�I�Ƭ� 5 �ɡA���a���ĤT�i�P�� 0, 1, 2, 3, 8, 9 �ɡA���a���ݸɵP�A��l���p�n�ɵP�C
tell $n ���a�I�Ƭ� 6 �ɡA���a���ĤT�i�P�� 6 �� 7 �I�ɡA���a�ݭn�ɵP�A��l���p���ݭn�ɵP�C
tell $n ���a�I�Ƥj�� 7 �H�W�ɫh���ݭn�ɵP�C
tell $n �ʮa�֪��ɵP�ä��@�w�O�@���u�աA�Ϧӥi��|���I�ƭ��C�C
tell $n ------------------------------------------------------------------------------
tell $n �ӭt�u�|���T�ر��p�G�̫��I�ơA����a�j�󶢮a�ɡA���aĹ�A�Ϥ��h�O���aĹ�C
tell $n �p�G���B�����̫��I�ƬۦP�h�M��(tie)�A�����p�U�`���B�����w�X�N�h�١C
tell $n ���n�`�N����aĹ�B���a�u�oĹ�����p�U�AĹ�o�������|�Q��� 5% �����C
tell $n ��l(pair)�h�̲��B���}�P�᪺��i�P�A���ݪ��u�ݼƦr�Τ�r�O�_����C
tell $n �ʮa�֤��Ҧ�����`��P�ɦs�b�A�u�n����F���N��o����S�C
tell $n �p�G�ٷQ�F�ѦU��m���߲v�i�H�~��ݧڡC(baccarat-rule3)
~
>talk_prog p baccarat-rule3~
tell $n �C���U�`�� 1 ���A�P���C�����C�춢�a�̦h�U�` 5 ���C
tell $n ---------------------------------------------
tell $n ���a�X��l(Player-Pair)           1 �� 11
tell $n ���a�X��l(Banker-Pair)           1 �� 11
tell $n �M��(Tie)                         1 �� 8
tell $n ���aĹ(Player)                    1 �� 1
tell $n ���aĹ(Banker)                    1 �� 0.95
tell $n �p�G�߲v�����G�p�� 1 ���A�h���󵥭Ȥp�� 1 ��쪺�w�X�C
tell $n ---------------------------------------------
tell $n �t�~����O��I����W�j�p���P���w�X(chips)�C
~
>talk_prog chips~
tell $n �w�X�ئ@�����ح��B���O�O 100�A50�A25�A10 �� 5�C
tell $n �p�G�Q���W�Ҧ��p�B�w�X�����j���A������ڻ�(change bigger)�C
tell $n �|�ҡG���|�� 10 �Υ|�� 25 ���B���S�X�A�i�D�� change bigger�C
tell $n �ڱN�����A�@�� 100 �A�@�� 25 �A�@�� 10 �� �@�� 5 ���B���w�X�C
tell $n �p�G�Q��S�w���B���w�X�����̤j���B���A�i�H������ڻ�(change �w�X���B)�C
tell $n �|�ҡG������ 25 ���B���S�X�A�i�D�� change 25�C
tell $n �ڱN�����A�@�� 100 �Τ@�� 50 ���w�X�C
tell $n �p�G�Q��S�w���B���w�X�����S�w���B���A�i�H������ڻ�(change �����w�X���Bto�S�w�w�X���B)�C
tell $n �|�ҡG������ 25 ���B���S�X�A�i�D�� change 25to100�C
tell $n �ڱN�����A�@�� 100 �Τ@�� 25 ���w�X�C
tell $n �άO���@�� 100 ���S�X�Q������� 50 ���w�X�A�i�H�i�D�� change 100to50�C
tell $n �N��A������ƶq�����A�ڤ]�|���A�~�X�C
tell $n ���O�p�G���X���O 100to100�A50to50�Achange 100�������L��n�D�C
tell $n �ڬO���|�z�A���C
~
>talk_prog p change bigger~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall chips_count $n
mpforcer here rcall chips_change_all $n
tell $n �Ҧ��w�X�I�������C
~
>talk_prog p change 100to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_50 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
|