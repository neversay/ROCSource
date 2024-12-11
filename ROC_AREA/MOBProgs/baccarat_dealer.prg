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
mpecho $I§Ö³tªº±qµP²°¤¤¨ú¥X¥|±i·tµP¡A¨C¦¸¨úµP«á¨Ì§Çµoµ¹¶¢®a¡A²ø®a¡A¶¢®a¡A²ø®a¡C
mpcallout 1 mpecho ÀH«á$I¥ý«áÂ½¶}¶¢®a¤Î²ø®aÂÐ»\ªºµP...
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
mpechoat $n ¡@    PLAYER    ¢x    BANKER
mpechoat $n ¡@              ¢x
mpnonechoat $n ¡@
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
		mpnonechoat $n ¡@¢Ï [0m
	endif
	if is_qmark($i) & baccarat_player1 100002
		mpnonechoat $n ¡@¢± [0m
	endif
	if is_qmark($i) & baccarat_player1 100004
		mpnonechoat $n ¡@¢² [0m
	endif
	if is_qmark($i) & baccarat_player1 100008
		mpnonechoat $n ¡@¢³ [0m
	endif
	if is_qmark($i) & baccarat_player1 100016
		mpnonechoat $n ¡@¢´ [0m
	endif
	if is_qmark($i) & baccarat_player1 100032
		mpnonechoat $n ¡@¢µ [0m
	endif
	if is_qmark($i) & baccarat_player1 100064
		mpnonechoat $n ¡@¢¶ [0m
	endif
	if is_qmark($i) & baccarat_player1 100128
		mpnonechoat $n ¡@¢· [0m
	endif
	if is_qmark($i) & baccarat_player1 100256
		mpnonechoat $n ¡@¢¸ [0m
	endif
	if is_qmark($i) & baccarat_player1 100512
		mpnonechoat $n ¢°¢¯ [0m
	endif
	if is_qmark($i) & baccarat_player1 101024
		mpnonechoat $n ¡@¢Ø [0m
	endif
	if is_qmark($i) & baccarat_player1 102048
		mpnonechoat $n ¡@¢ß [0m
	endif
	if is_qmark($i) & baccarat_player1 104096
		mpnonechoat $n ¡@¢Ù [0m
	endif
else
	mpnonechoat $n ¡@   
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
		mpnonechoat $n ¡@¢Ï [0m
	endif
	if is_qmark($i) & baccarat_player2 100002
		mpnonechoat $n ¡@¢± [0m
	endif
	if is_qmark($i) & baccarat_player2 100004
		mpnonechoat $n ¡@¢² [0m
	endif
	if is_qmark($i) & baccarat_player2 100008
		mpnonechoat $n ¡@¢³ [0m
	endif
	if is_qmark($i) & baccarat_player2 100016
		mpnonechoat $n ¡@¢´ [0m
	endif
	if is_qmark($i) & baccarat_player2 100032
		mpnonechoat $n ¡@¢µ [0m
	endif
	if is_qmark($i) & baccarat_player2 100064
		mpnonechoat $n ¡@¢¶ [0m
	endif
	if is_qmark($i) & baccarat_player2 100128
		mpnonechoat $n ¡@¢· [0m
	endif
	if is_qmark($i) & baccarat_player2 100256
		mpnonechoat $n ¡@¢¸ [0m
	endif
	if is_qmark($i) & baccarat_player2 100512
		mpnonechoat $n ¢°¢¯ [0m
	endif
	if is_qmark($i) & baccarat_player2 101024
		mpnonechoat $n ¡@¢Ø [0m
	endif
	if is_qmark($i) & baccarat_player2 102048
		mpnonechoat $n ¡@¢ß [0m
	endif
	if is_qmark($i) & baccarat_player2 104096
		mpnonechoat $n ¡@¢Ù [0m
	endif
else
	mpnonechoat $n ¡@   
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
		mpnonechoat $n ¡@¢Ï[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100002
		mpnonechoat $n ¡@¢±[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100004
		mpnonechoat $n ¡@¢²[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100008
		mpnonechoat $n ¡@¢³[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100016
		mpnonechoat $n ¡@¢´[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100032
		mpnonechoat $n ¡@¢µ[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100064
		mpnonechoat $n ¡@¢¶[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100128
		mpnonechoat $n ¡@¢·[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100256
		mpnonechoat $n ¡@¢¸[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 100512
		mpnonechoat $n ¢°¢¯[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 101024
		mpnonechoat $n ¡@¢Ø[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 102048
		mpnonechoat $n ¡@¢ß[0m¢x
	endif
	if is_qmark($i) & baccarat_player3 104096
		mpnonechoat $n ¡@¢Ù[0m¢x
	endif
else
	mpnonechoat $n ¡@  ¢x
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
		mpnonechoat $n ¡@¢Ï [0m
	endif
	if is_qmark($i) & baccarat_banker1 100002
		mpnonechoat $n ¡@¢± [0m
	endif
	if is_qmark($i) & baccarat_banker1 100004
		mpnonechoat $n ¡@¢² [0m
	endif
	if is_qmark($i) & baccarat_banker1 100008
		mpnonechoat $n ¡@¢³ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100016
		mpnonechoat $n ¡@¢´ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100032
		mpnonechoat $n ¡@¢µ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100064
		mpnonechoat $n ¡@¢¶ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100128
		mpnonechoat $n ¡@¢· [0m
	endif
	if is_qmark($i) & baccarat_banker1 100256
		mpnonechoat $n ¡@¢¸ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100512
		mpnonechoat $n ¢°¢¯ [0m
	endif
	if is_qmark($i) & baccarat_banker1 101024
		mpnonechoat $n ¡@¢Ø [0m
	endif
	if is_qmark($i) & baccarat_banker1 102048
		mpnonechoat $n ¡@¢ß [0m
	endif
	if is_qmark($i) & baccarat_banker1 104096
		mpnonechoat $n ¡@¢Ù [0m
	endif
else
	mpnonechoat $n ¡@ 
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
		mpnonechoat $n ¡@¢Ï [0m
	endif
	if is_qmark($i) & baccarat_banker2 100002
		mpnonechoat $n ¡@¢± [0m
	endif
	if is_qmark($i) & baccarat_banker2 100004
		mpnonechoat $n ¡@¢² [0m
	endif
	if is_qmark($i) & baccarat_banker2 100008
		mpnonechoat $n ¡@¢³ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100016
		mpnonechoat $n ¡@¢´ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100032
		mpnonechoat $n ¡@¢µ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100064
		mpnonechoat $n ¡@¢¶ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100128
		mpnonechoat $n ¡@¢· [0m
	endif
	if is_qmark($i) & baccarat_banker2 100256
		mpnonechoat $n ¡@¢¸ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100512
		mpnonechoat $n ¢°¢¯ [0m
	endif
	if is_qmark($i) & baccarat_banker2 101024
		mpnonechoat $n ¡@¢Ø [0m
	endif
	if is_qmark($i) & baccarat_banker2 102048
		mpnonechoat $n ¡@¢ß [0m
	endif
	if is_qmark($i) & baccarat_banker2 104096
		mpnonechoat $n ¡@¢Ù [0m
	endif
else
	mpnonechoat $n ¡@ 
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
		mpechoat $n ¡@¢Ï [0m
	endif
	if is_qmark($i) & baccarat_banker3 100002
		mpechoat $n ¡@¢± [0m
	endif
	if is_qmark($i) & baccarat_banker3 100004
		mpechoat $n ¡@¢² [0m
	endif
	if is_qmark($i) & baccarat_banker3 100008
		mpechoat $n ¡@¢³ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100016
		mpechoat $n ¡@¢´ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100032
		mpechoat $n ¡@¢µ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100064
		mpechoat $n ¡@¢¶ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100128
		mpechoat $n ¡@¢· [0m
	endif
	if is_qmark($i) & baccarat_banker3 100256
		mpechoat $n ¡@¢¸ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100512
		mpechoat $n ¢°¢¯ [0m
	endif
	if is_qmark($i) & baccarat_banker3 101024
		mpechoat $n ¡@¢Ø [0m
	endif
	if is_qmark($i) & baccarat_banker3 102048
		mpechoat $n ¡@¢ß [0m
	endif
	if is_qmark($i) & baccarat_banker3 104096
		mpechoat $n ¡@¢Ù [0m
	endif
else
	mpechoat $n ¡@
endif
mpechoat $n ¡@              ¢x
mpechoat $n ¡@
mpechoat $n ¡@ùÝ¢w¢w¢w¢wùÞ¢w¢w¢wùÞ¢w¢w¢w¢wùß
mpechoat $n ¡@¢x PLAYER ¢x      ¢x BANKER ¢x
mpechoat $n ¡@¢x  PIAR  ¢x TIE  ¢x  PAIR  ¢x
mpechoat $n ¡@¢x        ¢x      ¢x        ¢x
if is_qmark($n) >= baccarat_player_pair 100001
	mpnonechoat $n ¡@¢x   [1;32m¡´[0m   
else
	mpnonechoat $n ¡@¢x        
endif
if is_qmark($n) >= baccarat_tie_game 100001
	mpnonechoat $n ¢x  [1;32m¡´[0m  
else
	mpnonechoat $n ¢x      
endif
if is_qmark($n) >= baccarat_banker_pair 100001
	mpnonechoat $n ¢x   [1;32m¡´[0m   
else
	mpnonechoat $n ¢x        
endif
mpechoat $n ¢x
mpechoat $n ¡@ùà¢w¢w¢w¢wùä¢w¢w¢wùä¢w¢w¢w¢wùâ
mpechoat $n ¡@¢x                          ¢x
mpechoat $n ¡@¢x         BANKER           ¢x
if is_qmark($n) >= baccarat_banker_win 100001
	mpechoat $n ¡@¢x            [1;32m¡´[0m            ¢x
else
	mpechoat $n ¡@¢x                          ¢x
endif
mpechoat $n ¡@ùà¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢wùâ
mpechoat $n ¡@¢x                          ¢x
mpechoat $n ¡@¢x         PLAYER           ¢x
if is_qmark($n) >= baccarat_player_win 100001
	mpechoat $n ¡@¢x            [1;32m¡´[0m            ¢x
else
	mpechoat $n ¡@¢x                          ¢x
endif
mpechoat $n ¡@ùã¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢wùå
if is_qmark($n) == baccarat_banker_win 100001
	mpechoat $n §A©ã ¢° Äw½X½ä²ø®aÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_win 100002
	mpechoat $n §A©ã ¢± Äw½X½ä²ø®aÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_win 100003
	mpechoat $n §A©ã ¢² Äw½X½ä²ø®aÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_win 100004
	mpechoat $n §A©ã ¢³ Äw½X½ä²ø®aÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_win 100005
	mpechoat $n §A©ã ¢´ Äw½X½ä²ø®aÄ¹¡C
endif
if is_qmark($n) == baccarat_player_win 100001
	mpechoat $n §A©ã ¢° Äw½X½ä¶¢®aÄ¹¡C
endif
if is_qmark($n) == baccarat_player_win 100002
	mpechoat $n §A©ã ¢± Äw½X½ä¶¢®aÄ¹¡C
endif
if is_qmark($n) == baccarat_player_win 100003
	mpechoat $n §A©ã ¢² Äw½X½ä¶¢®aÄ¹¡C
endif
if is_qmark($n) == baccarat_player_win 100004
	mpechoat $n §A©ã ¢³ Äw½X½ä¶¢®aÄ¹¡C
endif
if is_qmark($n) == baccarat_player_win 100005
	mpechoat $n §A©ã ¢´ Äw½X½ä¶¢®aÄ¹¡C
endif
if is_qmark($n) == baccarat_tie_game 100001
	mpechoat $n §A©ã ¢° Äw½X½ä³o³õ©M§½¡C
endif
if is_qmark($n) == baccarat_tie_game 100002
	mpechoat $n §A©ã ¢± Äw½X½ä³o³õ©M§½¡C
endif
if is_qmark($n) == baccarat_tie_game 100003
	mpechoat $n §A©ã ¢² Äw½X½ä³o³õ©M§½¡C
endif
if is_qmark($n) == baccarat_tie_game 100004
	mpechoat $n §A©ã ¢³ Äw½X½ä³o³õ©M§½¡C
endif
if is_qmark($n) == baccarat_tie_game 100005
	mpechoat $n §A©ã ¢´ Äw½X½ä³o³õ©M§½¡C
endif
if is_qmark($n) == baccarat_banker_pair 100001
	mpechoat $n §A©ã ¢° Äw½X½ä²ø®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_pair 100002
	mpechoat $n §A©ã ¢± Äw½X½ä²ø®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_pair 100003
	mpechoat $n §A©ã ¢² Äw½X½ä²ø®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_pair 100004
	mpechoat $n §A©ã ¢³ Äw½X½ä²ø®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_banker_pair 100005
	mpechoat $n §A©ã ¢´ Äw½X½ä²ø®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_player_pair 100001
	mpechoat $n §A©ã ¢° Äw½X½ä¶¢®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_player_pair 100002
	mpechoat $n §A©ã ¢± Äw½X½ä¶¢®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_player_pair 100003
	mpechoat $n §A©ã ¢² Äw½X½ä¶¢®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_player_pair 100004
	mpechoat $n §A©ã ¢³ Äw½X½ä¶¢®a¥X¹ï¤lÄ¹¡C
endif
if is_qmark($n) == baccarat_player_pair 100005
	mpechoat $n §A©ã ¢´ Äw½X½ä¶¢®a¥X¹ï¤lÄ¹¡C
endif
~
>defun_prog baccarat_show_card~
if is_qmark($n) > baccarat_player1 100000
	mpnonecho ¶¢®a¬°¡G
	if is_qmark($n) == baccarat_player1 200001
		mpnonecho [1;31m¬õ¤ß[0m
	endif
	if is_qmark($n) == baccarat_player1 200002
		mpnonecho [1;31m¤è¶ô[0m
	endif
	if is_qmark($n) == baccarat_player1 200003
		mpnonecho [1;30m¶Â®ç[0m
	endif
	if is_qmark($n) == baccarat_player1 200004
		mpnonecho [1;30m±öªá[0m
	endif
	if is_qmark($n) & baccarat_player1 100001
		mpnonecho [1;37m¢Ï[0m
	endif
	if is_qmark($n) & baccarat_player1 100002
		mpnonecho [1;37m¢±[0m
	endif
	if is_qmark($n) & baccarat_player1 100004
		mpnonecho [1;37m¢²[0m
	endif
	if is_qmark($n) & baccarat_player1 100008
		mpnonecho [1;37m¢³[0m
	endif
	if is_qmark($n) & baccarat_player1 100016
		mpnonecho [1;37m¢´[0m
	endif
	if is_qmark($n) & baccarat_player1 100032
		mpnonecho [1;37m¢µ[0m
	endif
	if is_qmark($n) & baccarat_player1 100064
		mpnonecho [1;37m¢¶[0m
	endif
	if is_qmark($n) & baccarat_player1 100128
		mpnonecho [1;37m¢·[0m
	endif
	if is_qmark($n) & baccarat_player1 100256
		mpnonecho [1;37m¢¸[0m
	endif
	if is_qmark($n) & baccarat_player1 100512
		mpnonecho [1;37m¢°¢¯[0m
	endif
	if is_qmark($n) & baccarat_player1 101024
		mpnonecho [1;37m¢Ø[0m
	endif
	if is_qmark($n) & baccarat_player1 102048
		mpnonecho [1;37m¢ß[0m
	endif
	if is_qmark($n) & baccarat_player1 104096
		mpnonecho [1;37m¢Ù[0m
	endif
endif
if is_qmark($n) > baccarat_player2 100000
	mpnonecho  ¡B 
	if is_qmark($n) == baccarat_player2 200001
		mpnonecho [1;31m¬õ¤ß[0m
	endif
	if is_qmark($n) == baccarat_player2 200002
		mpnonecho [1;31m¤è¶ô[0m
	endif
	if is_qmark($n) == baccarat_player2 200003
		mpnonecho [1;30m¶Â®ç[0m
	endif
	if is_qmark($n) == baccarat_player2 200004
		mpnonecho [1;30m±öªá[0m
	endif
	if is_qmark($n) & baccarat_player2 100001
		mpnonecho [1;37m¢Ï[0m
	endif
	if is_qmark($n) & baccarat_player2 100002
		mpnonecho [1;37m¢±[0m
	endif
	if is_qmark($n) & baccarat_player2 100004
		mpnonecho [1;37m¢²[0m
	endif
	if is_qmark($n) & baccarat_player2 100008
		mpnonecho [1;37m¢³[0m
	endif
	if is_qmark($n) & baccarat_player2 100016
		mpnonecho [1;37m¢´[0m
	endif
	if is_qmark($n) & baccarat_player2 100032
		mpnonecho [1;37m¢µ[0m
	endif
	if is_qmark($n) & baccarat_player2 100064
		mpnonecho [1;37m¢¶[0m
	endif
	if is_qmark($n) & baccarat_player2 100128
		mpnonecho [1;37m¢·[0m
	endif
	if is_qmark($n) & baccarat_player2 100256
		mpnonecho [1;37m¢¸[0m
	endif
	if is_qmark($n) & baccarat_player2 100512
		mpnonecho [1;37m¢°¢¯[0m
	endif
	if is_qmark($n) & baccarat_player2 101024
		mpnonecho [1;37m¢Ø[0m
	endif
	if is_qmark($n) & baccarat_player2 102048
		mpnonecho [1;37m¢ß[0m
	endif
	if is_qmark($n) & baccarat_player2 104096
		mpnonecho [1;37m¢Ù[0m
	endif
endif
if is_qmark($n) > baccarat_player3 100000
	mpnonecho  ¡B 
	if is_qmark($n) == baccarat_player3 200001
		mpnonecho [1;31m¬õ¤ß[0m
	endif
	if is_qmark($n) == baccarat_player3 200002
		mpnonecho [1;31m¤è¶ô[0m
	endif
	if is_qmark($n) == baccarat_player3 200003
		mpnonecho [1;30m¶Â®ç[0m
	endif
	if is_qmark($n) == baccarat_player3 200004
		mpnonecho [1;30m±öªá[0m
	endif
	if is_qmark($n) & baccarat_player3 100001
		mpnonecho [1;37m¢Ï[0m
	endif
	if is_qmark($n) & baccarat_player3 100002
		mpnonecho [1;37m¢±[0m
	endif
	if is_qmark($n) & baccarat_player3 100004
		mpnonecho [1;37m¢²[0m
	endif
	if is_qmark($n) & baccarat_player3 100008
		mpnonecho [1;37m¢³[0m
	endif
	if is_qmark($n) & baccarat_player3 100016
		mpnonecho [1;37m¢´[0m
	endif
	if is_qmark($n) & baccarat_player3 100032
		mpnonecho [1;37m¢µ[0m
	endif
	if is_qmark($n) & baccarat_player3 100064
		mpnonecho [1;37m¢¶[0m
	endif
	if is_qmark($n) & baccarat_player3 100128
		mpnonecho [1;37m¢·[0m
	endif
	if is_qmark($n) & baccarat_player3 100256
		mpnonecho [1;37m¢¸[0m
	endif
	if is_qmark($n) & baccarat_player3 100512
		mpnonecho [1;37m¢°¢¯[0m
	endif
	if is_qmark($n) & baccarat_player3 101024
		mpnonecho [1;37m¢Ø[0m
	endif
	if is_qmark($n) & baccarat_player3 102048
		mpnonecho [1;37m¢ß[0m
	endif
	if is_qmark($n) & baccarat_player3 104096
		mpnonecho [1;37m¢Ù[0m
	endif
endif
mpecho .
if is_qmark($n) > baccarat_banker1 100000
	mpnonecho ²ø®a¬°¡G
	if is_qmark($n) == baccarat_banker1 200001
		mpnonecho [1;31m¬õ¤ß[0m
	endif
	if is_qmark($n) == baccarat_banker1 200002
		mpnonecho [1;31m¤è¶ô[0m
	endif
	if is_qmark($n) == baccarat_banker1 200003
		mpnonecho [1;30m¶Â®ç[0m
	endif
	if is_qmark($n) == baccarat_banker1 200004
		mpnonecho [1;30m±öªá[0m
	endif
	if is_qmark($n) & baccarat_banker1 100001
		mpnonecho [1;37m¢Ï[0m
	endif
	if is_qmark($n) & baccarat_banker1 100002
		mpnonecho [1;37m¢±[0m
	endif
	if is_qmark($n) & baccarat_banker1 100004
		mpnonecho [1;37m¢²[0m
	endif
	if is_qmark($n) & baccarat_banker1 100008
		mpnonecho [1;37m¢³[0m
	endif
	if is_qmark($n) & baccarat_banker1 100016
		mpnonecho [1;37m¢´[0m
	endif
	if is_qmark($n) & baccarat_banker1 100032
		mpnonecho [1;37m¢µ[0m
	endif
	if is_qmark($n) & baccarat_banker1 100064
		mpnonecho [1;37m¢¶[0m
	endif
	if is_qmark($n) & baccarat_banker1 100128
		mpnonecho [1;37m¢·[0m
	endif
	if is_qmark($n) & baccarat_banker1 100256
		mpnonecho [1;37m¢¸[0m
	endif
	if is_qmark($n) & baccarat_banker1 100512
		mpnonecho [1;37m¢°¢¯[0m
	endif
	if is_qmark($n) & baccarat_banker1 101024
		mpnonecho [1;37m¢Ø[0m
	endif
	if is_qmark($n) & baccarat_banker1 102048
		mpnonecho [1;37m¢ß[0m
	endif
	if is_qmark($n) & baccarat_banker1 104096
		mpnonecho [1;37m¢Ù[0m
	endif
endif
if is_qmark($n) > baccarat_banker2 100000
	mpnonecho  ¡B 
	if is_qmark($n) == baccarat_banker2 200001
		mpnonecho [1;31m¬õ¤ß[0m
	endif
	if is_qmark($n) == baccarat_banker2 200002
		mpnonecho [1;31m¤è¶ô[0m
	endif
	if is_qmark($n) == baccarat_banker2 200003
		mpnonecho [1;30m¶Â®ç[0m
	endif
	if is_qmark($n) == baccarat_banker2 200004
		mpnonecho [1;30m±öªá[0m
	endif
	if is_qmark($n) & baccarat_banker2 100001
		mpnonecho [1;37m¢Ï[0m
	endif
	if is_qmark($n) & baccarat_banker2 100002
		mpnonecho [1;37m¢±[0m
	endif
	if is_qmark($n) & baccarat_banker2 100004
		mpnonecho [1;37m¢²[0m
	endif
	if is_qmark($n) & baccarat_banker2 100008
		mpnonecho [1;37m¢³[0m
	endif
	if is_qmark($n) & baccarat_banker2 100016
		mpnonecho [1;37m¢´[0m
	endif
	if is_qmark($n) & baccarat_banker2 100032
		mpnonecho [1;37m¢µ[0m
	endif
	if is_qmark($n) & baccarat_banker2 100064
		mpnonecho [1;37m¢¶[0m
	endif
	if is_qmark($n) & baccarat_banker2 100128
		mpnonecho [1;37m¢·[0m
	endif
	if is_qmark($n) & baccarat_banker2 100256
		mpnonecho [1;37m¢¸[0m
	endif
	if is_qmark($n) & baccarat_banker2 100512
		mpnonecho [1;37m¢°¢¯[0m
	endif
	if is_qmark($n) & baccarat_banker2 101024
		mpnonecho [1;37m¢Ø[0m
	endif
	if is_qmark($n) & baccarat_banker2 102048
		mpnonecho [1;37m¢ß[0m
	endif
	if is_qmark($n) & baccarat_banker2 104096
		mpnonecho [1;37m¢Ù[0m
	endif
endif
if is_qmark($n) > baccarat_banker3 100000
	mpnonecho  ¡B 
	if is_qmark($n) == baccarat_banker3 200001
		mpnonecho [1;31m¬õ¤ß[0m
	endif
	if is_qmark($n) == baccarat_banker3 200002
		mpnonecho [1;31m¤è¶ô[0m
	endif
	if is_qmark($n) == baccarat_banker3 200003
		mpnonecho [1;30m¶Â®ç[0m
	endif
	if is_qmark($n) == baccarat_banker3 200004
		mpnonecho [1;30m±öªá[0m
	endif
	if is_qmark($n) & baccarat_banker3 100001
		mpnonecho [1;37m¢Ï[0m
	endif
	if is_qmark($n) & baccarat_banker3 100002
		mpnonecho [1;37m¢±[0m
	endif
	if is_qmark($n) & baccarat_banker3 100004
		mpnonecho [1;37m¢²[0m
	endif
	if is_qmark($n) & baccarat_banker3 100008
		mpnonecho [1;37m¢³[0m
	endif
	if is_qmark($n) & baccarat_banker3 100016
		mpnonecho [1;37m¢´[0m
	endif
	if is_qmark($n) & baccarat_banker3 100032
		mpnonecho [1;37m¢µ[0m
	endif
	if is_qmark($n) & baccarat_banker3 100064
		mpnonecho [1;37m¢¶[0m
	endif
	if is_qmark($n) & baccarat_banker3 100128
		mpnonecho [1;37m¢·[0m
	endif
	if is_qmark($n) & baccarat_banker3 100256
		mpnonecho [1;37m¢¸[0m
	endif
	if is_qmark($n) & baccarat_banker3 100512
		mpnonecho [1;37m¢°¢¯[0m
	endif
	if is_qmark($n) & baccarat_banker3 101024
		mpnonecho [1;37m¢Ø[0m
	endif
	if is_qmark($n) & baccarat_banker3 102048
		mpnonecho [1;37m¢ß[0m
	endif
	if is_qmark($n) & baccarat_banker3 104096
		mpnonecho [1;37m¢Ù[0m
	endif
endif
mpecho .
~
>defun_prog baccarat_show_card2~
if is_qmark($n) > baccarat_player3 100000
	mpnonecho $I§Ö³tªº±qµP²°¤¤¨ú¥X¤@±i 
	if is_qmark($n) == baccarat_player3 200001
		mpnonecho [1;31m¬õ¤ß[0m 
	endif
	if is_qmark($n) == baccarat_player3 200002
		mpnonecho [1;31m¤è¶ô[0m 
	endif
	if is_qmark($n) == baccarat_player3 200003
		mpnonecho [1;30m¶Â®ç[0m 
	endif
	if is_qmark($n) == baccarat_player3 200004
		mpnonecho [1;30m±öªá[0m 
	endif
	if is_qmark($n) & baccarat_player3 100001
		mpnonecho [1;37m¢Ï[0m 
	endif
	if is_qmark($n) & baccarat_player3 100002
		mpnonecho [1;37m¢±[0m 
	endif
	if is_qmark($n) & baccarat_player3 100004
		mpnonecho [1;37m¢²[0m 
	endif
	if is_qmark($n) & baccarat_player3 100008
		mpnonecho [1;37m¢³[0m 
	endif
	if is_qmark($n) & baccarat_player3 100016
		mpnonecho [1;37m¢´[0m 
	endif
	if is_qmark($n) & baccarat_player3 100032
		mpnonecho [1;37m¢µ[0m 
	endif
	if is_qmark($n) & baccarat_player3 100064
		mpnonecho [1;37m¢¶[0m 
	endif
	if is_qmark($n) & baccarat_player3 100128
		mpnonecho [1;37m¢·[0m 
	endif
	if is_qmark($n) & baccarat_player3 100256
		mpnonecho [1;37m¢¸[0m 
	endif
	if is_qmark($n) & baccarat_player3 100512
		mpnonecho [1;37m¢°¢¯[0m 
	endif
	if is_qmark($n) & baccarat_player3 101024
		mpnonecho [1;37m¢Ø[0m 
	endif
	if is_qmark($n) & baccarat_player3 102048
		mpnonecho [1;37m¢ß[0m 
	endif
	if is_qmark($n) & baccarat_player3 104096
		mpnonecho [1;37m¢Ù[0m 
	endif
endif
mpecho µoµ¹¶¢®a¡C
~
>defun_prog baccarat_show_card3~
if is_qmark($n) > baccarat_banker3 100000
	mpnonecho $I§Ö³tªº±qµP²°¤¤¨ú¥X¤@±i 
	if is_qmark($n) == baccarat_banker3 200001
		mpnonecho [1;31m¬õ¤ß[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200002
		mpnonecho [1;31m¤è¶ô[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200003
		mpnonecho [1;30m¶Â®ç[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200004
		mpnonecho [1;30m±öªá[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100001
		mpnonecho [1;37m¢Ï[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100002
		mpnonecho [1;37m¢±[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100004
		mpnonecho [1;37m¢²[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100008
		mpnonecho [1;37m¢³[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100016
		mpnonecho [1;37m¢´[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100032
		mpnonecho [1;37m¢µ[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100064
		mpnonecho [1;37m¢¶[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100128
		mpnonecho [1;37m¢·[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100256
		mpnonecho [1;37m¢¸[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100512
		mpnonecho [1;37m¢°¢¯[0m 
	endif
	if is_qmark($n) & baccarat_banker3 101024
		mpnonecho [1;37m¢Ø[0m 
	endif
	if is_qmark($n) & baccarat_banker3 102048
		mpnonecho [1;37m¢ß[0m 
	endif
	if is_qmark($n) & baccarat_banker3 104096
		mpnonecho [1;37m¢Ù[0m 
	endif
endif
mpecho µoµ¹²ø®a¡C
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
	say ¶¢®a¸ÉµP¡C
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
	say ²ø®a¸ÉµP¡C
	mpcall baccarat_banker_random_3 $i
	break
endif
if is_qmark($i) == baccarat_banker_point 100003
	if is_qmark($i) & baccarat_player3 100128
	else
		say ²ø®a¸ÉµP¡C
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
		say ²ø®a¸ÉµP¡C
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
		say ²ø®a¸ÉµP¡C
		mpcall baccarat_banker_random_3 $i
		break
	endif
endif
if is_qmark($i) == baccarat_banker_point 100006
	if is_qmark($i) > baccarat_player3 100000
		if is_qmark($i) & baccarat_player3 100032
		or is_qmark($i) & baccarat_player3 100064
			say ²ø®a¸ÉµP¡C
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
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢¯[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100001
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢°[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100002
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢±[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100003
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢²[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100004
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢³[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100005
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢´[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100006
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢µ[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100007
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢¶[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100008
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢·[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_player_point 100009
	mpnonecho $I[1;32m»¡µÛ¡u¶¢®a [1;37m¢¸[1;32m ÂI¡A[0m
endif
if is_qmark($i) == baccarat_banker_point 100000
	mpecho [1;32m²ø®a [1;37m¢¯[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100001
	mpecho [1;32m²ø®a [1;37m¢°[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100002
	mpecho [1;32m²ø®a [1;37m¢±[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100003
	mpecho [1;32m²ø®a [1;37m¢²[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100004
	mpecho [1;32m²ø®a [1;37m¢³[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100005
	mpecho [1;32m²ø®a [1;37m¢´[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100006
	mpecho [1;32m²ø®a [1;37m¢µ[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100007
	mpecho [1;32m²ø®a [1;37m¢¶[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100008
	mpecho [1;32m²ø®a [1;37m¢·[1;32m ÂI¡C¡v[0m
endif
if is_qmark($i) == baccarat_banker_point 100009
	mpecho [1;32m²ø®a [1;37m¢¸[1;32m ÂI¡C¡v[0m
endif
~
>defun_prog baccarat_end_game~
if is_qmark($i) == baccarat_player_point 100000
	if is_qmark($i) > baccarat_banker_point 100000
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100000
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
endif
if is_qmark($i) == baccarat_player_point 100001
	if is_qmark($i) > baccarat_banker_point 100001
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100002
	if is_qmark($i) > baccarat_banker_point 100001
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100003
	if is_qmark($i) > baccarat_banker_point 100001
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100004
	if is_qmark($i) > baccarat_banker_point 100001
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100005
	if is_qmark($i) > baccarat_banker_point 100001
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100006
	if is_qmark($i) > baccarat_banker_point 100006
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100006
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100006
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100007
	if is_qmark($i) > baccarat_banker_point 100007
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100007
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100007
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100008
	if is_qmark($i) > baccarat_banker_point 100008
		say ²ø®aÄ¹¡C
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100008
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100008
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100009
	if is_qmark($i) == baccarat_banker_point 100009
		say ©M§½¡C
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100009
		say ¶¢®aÄ¹¡C
		mpforcer here modrvt v0 | 2
	endif
endif
mpforcer here rcall baccarat_ending_transfor $i
~
>defun_prog baccarat_reset_all~
mpecho ¹CÀ¸µ²§ô«á$I¾ã²z§¹®à­±¡A·Ç³Æ¶}©l¤U¤@§½¡C
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
tell $n ·Q¤F¸Ñ¦Ê®a¼Öªºª±ªk¡H(baccarat-rule)
~
>talk_prog p baccarat-rule~
tell $n ¦Ê®a¼Ö¬O¤@ºØ§¹¥þ¹Lµ{§¹¥þ¥Ñ²ü©x¶i¦æªº¹CÀ¸¡A°Ñ¥[ªºª±®a¥u»Ý­n¤Uª`¨ì·Q­nªº¦ì¸m´N¦æ¤F¡C
tell $n ¤Uª`ªº¤èªk¬° bet '¦ì¸m ¹S½X¼Æ¶q'¡A¥i¥H¨C­Ó¦ì¸m³£¤Uª`¡A¦ý¦P³õ¹CÀ¸¤º¨C¤H³Ì¦h¤Uª` 5 ¹S½X¬°¤W­­¡C
tell $n ¦b¹CÀ¸¤º¯àÀH®É¨Ï¥Î check ¨Ó¬Ý¦Û¤v¤Uª`ªºª¬ºA¤Î¥Ø«e¶}µPªº±¡ªp¡C
tell $n ¤Uª`¦ì¸m¦³¡G²ø®aÄ¹(Banker)¡B¶¢®aÄ¹(Player)¡B©M§½(Tie)¡B²ø®a¥X¹ï¤l(Banker-Pair)¡B¶¢®a¥X¹ï¤l(Player-Pair)¡C
tell $n ¥u­n¤Uª`¦¨¥\§Y¥i¥[¤J¸Ó³õ¹CÀ¸¡C
tell $n ¦pªG·Q¤F¸Ñ§ó¦h¦Ê®a¼Ö¬ÛÃöªº³W«h¥i¥HÄ~Äò°Ý§Ú¡C(baccarat-rule2)
~
>talk_prog p baccarat-rule2~
tell $n ¦Ê®a¼Ö¬O¤@ºØ¥H²ø¡B¶¢ÂI¼Æ¤ñ¤j¤pªº¯ÈµP¹CÀ¸¡AÂI¼ÆÁ`¦X¤jªº¤@¤è¯àÄ¹±o¤ñÁÉ¡C
tell $n ¼Æ¦rµPªº³¡¤À ¢± ¨ì ¢¸ ªºÂI¼Æ»P¥»¨­ªº¼Æ¦r¬Û¦P¡A¦Ó ¢°¢¯ «h¥Nªí 0 ÂI¡C
tell $n ¤å¦rµPªº³¡¤À¡A¢Ï ¥Nªí 1 ÂI¡A¢Ø¡B¢ß¡B¢Ù «h¥Nªí 0 ÂI¡C
tell $n ÂI¼Æ­pºâ¬O±N²ø®a©Î¶¢®a¦U¦Û«ù¦³ªº¤âµPÂI¼Æ¥[Á`¡A¦ý¥[°_¨Ó³Ì¤j¤£¯à¶W¹L 9¡C
tell $n ¦pªGÁ`¦X¶W¹L 9 «h¥HÁ`¦X«áªº­Ó¦ì¼Æ·í¦¨³Ì²×ªºÂI¼Æ¡C
tell $n ¨Ò¡G¢Ï + ¢¸ = 0 ÂI, ¢¸ + ¢¸ = 8 ÂI, ¢°¢¯ + ¢Ù = 0 ÂI, ¢· + ¢µ = 4 ÂI¡C
tell $n Â²³æ¨ÓÁ¿¡A¦b¹CÀ¸¤º³Ì¤jªºÂI¼Æ¬O 9 ¡A³Ì¤pªº«h¬° 0 ÂI¡C
tell $n ------------------------------------------------------------------------------
tell $n ¦Ê®a¼Ö¦b¹CÀ¸¶}©l¶i¦æ®É¡A²ü©x·|¤À§O¬£µP¨â±iµP¦Ü¶¢®a(player)»P²ø®a(banker)ªºÄæ¦ì¤W¨Ã¥B¶}µP¡C
tell $n ¶}µP«á¶i¦æ²Ä¤@¦¸ªºÂI¼Æ¥[Á`¨Ã¥B§P©w©Ò±oªºÂI¼Æ¬O§_¶i¦æ¯S®í³W«h¸ÉµP¡C
tell $n Âù¤è¸ÉµP³Ì¦h¶È·|¦³¤@±i¡AÂ²³æ¨ÓÁ¿¡A¦pªG»Ý­n¸ÉµP¡A¦bÂù¤è«ù¦³²Ä¤T±iµPªº®É­Ô¥²©w·|¦³µ²ªG¡C
tell $n ¦Ó¬O§_»Ý­n¸ÉµP¡A«h¥Ñ¶}µP«áªºÂI¼Æ¨Ó§P©w¡G
tell $n ------------------------------------------------------------------------------
tell $n ¥u­n²ø¡B¶¢¥ô¤@¥X²{¨â±iµPªºÂI¼Æ©M¬° 8 ©Î 9 ®É¡AÂù¤è³£¤£¶i¦æ¸ÉµP¡Aª½±µ¤ñ¸ûÂI¼Æ«áµ²§ô¹CÀ¸¡C
tell $n ¶¢®a¦bÂI¼Æ¤p©ó 5 ªº±¡ªp¤U¡A»Ý­n¸ÉµP¡A¤j©ó 6 «h¤£¸ÉµP¡C
tell $n ²ø®a¦bÂI¼Æ¤p©ó 2 ªº±¡ªp¤U¡A»Ý­n¸ÉµP¡C
tell $n ²ø®aÂI¼Æ¬° 3 ®É¡A²ø®a»Ý­n¸ÉµP¡A°£¤F¶¢®aªº²Ä¤T±iµP¬° 8 ÂI®É¡A²ø®a¤£»Ý­n¸ÉµP¡C
tell $n ²ø®aÂI¼Æ¬° 4 ®É¡A¶¢®aªº²Ä¤T±iµP¬° 0, 1, 8, 9 ®É¡A²ø®a¤£»Ý¸ÉµP¡A¨ä¾l±¡ªp­n¸ÉµP¡C
tell $n ²ø®aÂI¼Æ¬° 5 ®É¡A¶¢®aªº²Ä¤T±iµP¬° 0, 1, 2, 3, 8, 9 ®É¡A²ø®a¤£»Ý¸ÉµP¡A¨ä¾l±¡ªp­n¸ÉµP¡C
tell $n ²ø®aÂI¼Æ¬° 6 ®É¡A¶¢®aªº²Ä¤T±iµP¬° 6 ©Î 7 ÂI®É¡A²ø®a»Ý­n¸ÉµP¡A¨ä¾l±¡ªp¤£»Ý­n¸ÉµP¡C
tell $n ²ø®aÂI¼Æ¤j©ó 7 ¥H¤W®É«h¤£»Ý­n¸ÉµP¡C
tell $n ¦Ê®a¼Öªº¸ÉµP¨Ã¤£¤@©w¬O¤@ºØÀu¶Õ¡A¤Ï¦Ó¥i¯à·|¨ÏÂI¼Æ­°§C¡C
tell $n ------------------------------------------------------------------------------
tell $n ³Ó­t¥u·|¦³¤TºØ±¡ªp¡G³Ì«áÂI¼Æ¡A·í²ø®a¤j©ó¶¢®a®É¡A²ø®aÄ¹¡A¤Ï¤§«h¬O¶¢®aÄ¹¡C
tell $n ¦pªG²ø¡B¶¢ªº³Ì«áÂI¼Æ¬Û¦P«h©M§½(tie)¡A¦¹±¡ªp¤Uª`²ø¡B¶¢ªºÄw½X±N°hÁÙ¡C
tell $n ¦ý­nª`·N©ã²ø®aÄ¹¥B²ø®a¯u±oÄ¹ªº±¡ªp¤U¡AÄ¹±oªº³¡¤À·|³Q©â¨ú 5% ¦þª÷¡C
tell $n ¹ï¤l(pair)«h¨Ì²ø¡B¶¢¶}µP«áªº¨â±iµP¡A¤£¬Ýªá¦â¥u¬Ý¼Æ¦r©Î¤å¦r¬O§_¦¨¹ï¡C
tell $n ¦Ê®a¼Ö¤¤©Ò¦³ªº©ãª`¯à¦P®É¦s¦b¡A¥u­n±ø¥ó¹F¦¨´N¯à±o¨ì³ø¹S¡C
tell $n ¦pªGÁÙ·Q¤F¸Ñ¦U¦ì¸mªº½ß²v¥i¥HÄ~Äò°Ý§Ú¡C(baccarat-rule3)
~
>talk_prog p baccarat-rule3~
tell $n ¨C¦¸¤Uª`¬° 1 ³æ¦ì¡A¦P³õ¹CÀ¸¤º¨C¦ì¶¢®a³Ì¦h¤Uª` 5 ¦¸¡C
tell $n ---------------------------------------------
tell $n ¶¢®a¥X¹ï¤l(Player-Pair)           1 ½ß 11
tell $n ²ø®a¥X¹ï¤l(Banker-Pair)           1 ½ß 11
tell $n ©M§½(Tie)                         1 ½ß 8
tell $n ¶¢®aÄ¹(Player)                    1 ½ß 1
tell $n ²ø®aÄ¹(Banker)                    1 ½ß 0.95
tell $n ¦pªG½ß²vªºµ²ªG¤p©ó 1 ³æ¦ì¡A«hµ¹©óµ¥­È¤p©ó 1 ³æ¦ìªºÄw½X¡C
tell $n ---------------------------------------------
tell $n ¥t¥~¥»®à»O¯à§I´«¤â¤W¤j¤p¤£¦PªºÄw½X(chips)¡C
~
>talk_prog chips~
tell $n Äw½XºØ¦@¦³¤­ºØ­±ÃB¤À§O¬O 100¡A50¡A25¡A10 ¤Î 5¡C
tell $n ¦pªG·Q§â¤â¤W©Ò¦³¤pÃBÄw½X´«¦¨¤jªº¡Aª½±µ¸ò§Ú»¡(change bigger)¡C
tell $n Á|¨Ò¡G¦³¥|­Ó 10 ¤Î¥|­Ó 25 ­±ÃBªº¹S½X¡A§i¶D§Ú change bigger¡C
tell $n §Ú±N´«µ¹§A¤@­Ó 100 ¡A¤@­Ó 25 ¡A¤@­Ó 10 ¤Î ¤@­Ó 5 ­±ÃBªºÄw½X¡C
tell $n ¦pªG·Q§â¯S©w­±ÃBªºÄw½X´«¦¨³Ì¤j­±ÃBªº¡A¥i¥Hª½±µ¸ò§Ú»¡(change Äw½X­±ÃB)¡C
tell $n Á|¨Ò¡G¦³¤»­Ó 25 ­±ÃBªº¹S½X¡A§i¶D§Ú change 25¡C
tell $n §Ú±N´«µ¹§A¤@­Ó 100 ¤Î¤@­Ó 50 ªºÄw½X¡C
tell $n ¦pªG·Q§â¯S©w­±ÃBªºÄw½X´«¦¨¯S©w­±ÃBªº¡A¥i¥Hª½±µ¸ò§Ú»¡(change ±ý´«Äw½X­±ÃBto¯S©wÄw½X­±ÃB)¡C
tell $n Á|¨Ò¡G¦³¤­­Ó 25 ­±ÃBªº¹S½X¡A§i¶D§Ú change 25to100¡C
tell $n §Ú±N´«µ¹§A¤@­Ó 100 ¤Î¤@­Ó 25 ªºÄw½X¡C
tell $n ©Î¬O¦³¤@­Ó 100 ªº¹S½X·Q´«¦¨¨â­Ó 50 ªºÄw½X¡A¥i¥H§i¶D§Ú change 100to50¡C
tell $n ´Nºâ§A¤â«ùªº¼Æ¶q¤£¨¬¡A§Ú¤]·|À°§A¬~½X¡C
tell $n ¦ý¬O¦pªG´£¥X¹³¬O 100to100¡A50to50¡Achange 100¤§ÃþªºµL²á­n¨D¡C
tell $n §Ú¬O¤£·|²z§Aªº¡C
~
>talk_prog p change bigger~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall chips_count $n
mpforcer here rcall chips_change_all $n
tell $n ©Ò¦³Äw½X§I´«§¹¦¨¡C
~
>talk_prog p change 100to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_50 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
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
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
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
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
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
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
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
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
|