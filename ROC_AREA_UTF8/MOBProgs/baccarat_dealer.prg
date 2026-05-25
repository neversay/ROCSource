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
mpecho $I快速的從牌盒中取出四張暗牌，每次取牌後依序發給閒家，莊家，閒家，莊家。
mpcallout 1 mpecho 隨後$I先後翻開閒家及莊家覆蓋的牌...
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
mpechoat $n 　    PLAYER    │    BANKER
mpechoat $n 　              │
mpnonechoat $n 　
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
		mpnonechoat $n 　Ａ [0m
	endif
	if is_qmark($i) & baccarat_player1 100002
		mpnonechoat $n 　２ [0m
	endif
	if is_qmark($i) & baccarat_player1 100004
		mpnonechoat $n 　３ [0m
	endif
	if is_qmark($i) & baccarat_player1 100008
		mpnonechoat $n 　４ [0m
	endif
	if is_qmark($i) & baccarat_player1 100016
		mpnonechoat $n 　５ [0m
	endif
	if is_qmark($i) & baccarat_player1 100032
		mpnonechoat $n 　６ [0m
	endif
	if is_qmark($i) & baccarat_player1 100064
		mpnonechoat $n 　７ [0m
	endif
	if is_qmark($i) & baccarat_player1 100128
		mpnonechoat $n 　８ [0m
	endif
	if is_qmark($i) & baccarat_player1 100256
		mpnonechoat $n 　９ [0m
	endif
	if is_qmark($i) & baccarat_player1 100512
		mpnonechoat $n １０ [0m
	endif
	if is_qmark($i) & baccarat_player1 101024
		mpnonechoat $n 　Ｊ [0m
	endif
	if is_qmark($i) & baccarat_player1 102048
		mpnonechoat $n 　Ｑ [0m
	endif
	if is_qmark($i) & baccarat_player1 104096
		mpnonechoat $n 　Ｋ [0m
	endif
else
	mpnonechoat $n 　   
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
		mpnonechoat $n 　Ａ [0m
	endif
	if is_qmark($i) & baccarat_player2 100002
		mpnonechoat $n 　２ [0m
	endif
	if is_qmark($i) & baccarat_player2 100004
		mpnonechoat $n 　３ [0m
	endif
	if is_qmark($i) & baccarat_player2 100008
		mpnonechoat $n 　４ [0m
	endif
	if is_qmark($i) & baccarat_player2 100016
		mpnonechoat $n 　５ [0m
	endif
	if is_qmark($i) & baccarat_player2 100032
		mpnonechoat $n 　６ [0m
	endif
	if is_qmark($i) & baccarat_player2 100064
		mpnonechoat $n 　７ [0m
	endif
	if is_qmark($i) & baccarat_player2 100128
		mpnonechoat $n 　８ [0m
	endif
	if is_qmark($i) & baccarat_player2 100256
		mpnonechoat $n 　９ [0m
	endif
	if is_qmark($i) & baccarat_player2 100512
		mpnonechoat $n １０ [0m
	endif
	if is_qmark($i) & baccarat_player2 101024
		mpnonechoat $n 　Ｊ [0m
	endif
	if is_qmark($i) & baccarat_player2 102048
		mpnonechoat $n 　Ｑ [0m
	endif
	if is_qmark($i) & baccarat_player2 104096
		mpnonechoat $n 　Ｋ [0m
	endif
else
	mpnonechoat $n 　   
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
		mpnonechoat $n 　Ａ[0m│
	endif
	if is_qmark($i) & baccarat_player3 100002
		mpnonechoat $n 　２[0m│
	endif
	if is_qmark($i) & baccarat_player3 100004
		mpnonechoat $n 　３[0m│
	endif
	if is_qmark($i) & baccarat_player3 100008
		mpnonechoat $n 　４[0m│
	endif
	if is_qmark($i) & baccarat_player3 100016
		mpnonechoat $n 　５[0m│
	endif
	if is_qmark($i) & baccarat_player3 100032
		mpnonechoat $n 　６[0m│
	endif
	if is_qmark($i) & baccarat_player3 100064
		mpnonechoat $n 　７[0m│
	endif
	if is_qmark($i) & baccarat_player3 100128
		mpnonechoat $n 　８[0m│
	endif
	if is_qmark($i) & baccarat_player3 100256
		mpnonechoat $n 　９[0m│
	endif
	if is_qmark($i) & baccarat_player3 100512
		mpnonechoat $n １０[0m│
	endif
	if is_qmark($i) & baccarat_player3 101024
		mpnonechoat $n 　Ｊ[0m│
	endif
	if is_qmark($i) & baccarat_player3 102048
		mpnonechoat $n 　Ｑ[0m│
	endif
	if is_qmark($i) & baccarat_player3 104096
		mpnonechoat $n 　Ｋ[0m│
	endif
else
	mpnonechoat $n 　  │
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
		mpnonechoat $n 　Ａ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100002
		mpnonechoat $n 　２ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100004
		mpnonechoat $n 　３ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100008
		mpnonechoat $n 　４ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100016
		mpnonechoat $n 　５ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100032
		mpnonechoat $n 　６ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100064
		mpnonechoat $n 　７ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100128
		mpnonechoat $n 　８ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100256
		mpnonechoat $n 　９ [0m
	endif
	if is_qmark($i) & baccarat_banker1 100512
		mpnonechoat $n １０ [0m
	endif
	if is_qmark($i) & baccarat_banker1 101024
		mpnonechoat $n 　Ｊ [0m
	endif
	if is_qmark($i) & baccarat_banker1 102048
		mpnonechoat $n 　Ｑ [0m
	endif
	if is_qmark($i) & baccarat_banker1 104096
		mpnonechoat $n 　Ｋ [0m
	endif
else
	mpnonechoat $n 　 
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
		mpnonechoat $n 　Ａ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100002
		mpnonechoat $n 　２ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100004
		mpnonechoat $n 　３ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100008
		mpnonechoat $n 　４ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100016
		mpnonechoat $n 　５ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100032
		mpnonechoat $n 　６ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100064
		mpnonechoat $n 　７ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100128
		mpnonechoat $n 　８ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100256
		mpnonechoat $n 　９ [0m
	endif
	if is_qmark($i) & baccarat_banker2 100512
		mpnonechoat $n １０ [0m
	endif
	if is_qmark($i) & baccarat_banker2 101024
		mpnonechoat $n 　Ｊ [0m
	endif
	if is_qmark($i) & baccarat_banker2 102048
		mpnonechoat $n 　Ｑ [0m
	endif
	if is_qmark($i) & baccarat_banker2 104096
		mpnonechoat $n 　Ｋ [0m
	endif
else
	mpnonechoat $n 　 
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
		mpechoat $n 　Ａ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100002
		mpechoat $n 　２ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100004
		mpechoat $n 　３ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100008
		mpechoat $n 　４ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100016
		mpechoat $n 　５ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100032
		mpechoat $n 　６ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100064
		mpechoat $n 　７ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100128
		mpechoat $n 　８ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100256
		mpechoat $n 　９ [0m
	endif
	if is_qmark($i) & baccarat_banker3 100512
		mpechoat $n １０ [0m
	endif
	if is_qmark($i) & baccarat_banker3 101024
		mpechoat $n 　Ｊ [0m
	endif
	if is_qmark($i) & baccarat_banker3 102048
		mpechoat $n 　Ｑ [0m
	endif
	if is_qmark($i) & baccarat_banker3 104096
		mpechoat $n 　Ｋ [0m
	endif
else
	mpechoat $n 　
endif
mpechoat $n 　              │
mpechoat $n 　
mpechoat $n 　╔────╦───╦────╗
mpechoat $n 　│ PLAYER │      │ BANKER │
mpechoat $n 　│  PIAR  │ TIE  │  PAIR  │
mpechoat $n 　│        │      │        │
if is_qmark($n) >= baccarat_player_pair 100001
	mpnonechoat $n 　│   [1;32m●[0m   
else
	mpnonechoat $n 　│        
endif
if is_qmark($n) >= baccarat_tie_game 100001
	mpnonechoat $n │  [1;32m●[0m  
else
	mpnonechoat $n │      
endif
if is_qmark($n) >= baccarat_banker_pair 100001
	mpnonechoat $n │   [1;32m●[0m   
else
	mpnonechoat $n │        
endif
mpechoat $n │
mpechoat $n 　╠────╩───╩────╣
mpechoat $n 　│                          │
mpechoat $n 　│         BANKER           │
if is_qmark($n) >= baccarat_banker_win 100001
	mpechoat $n 　│            [1;32m●[0m            │
else
	mpechoat $n 　│                          │
endif
mpechoat $n 　╠─────────────╣
mpechoat $n 　│                          │
mpechoat $n 　│         PLAYER           │
if is_qmark($n) >= baccarat_player_win 100001
	mpechoat $n 　│            [1;32m●[0m            │
else
	mpechoat $n 　│                          │
endif
mpechoat $n 　╚─────────────╝
if is_qmark($n) == baccarat_banker_win 100001
	mpechoat $n 你押 １ 籌碼賭莊家贏。
endif
if is_qmark($n) == baccarat_banker_win 100002
	mpechoat $n 你押 ２ 籌碼賭莊家贏。
endif
if is_qmark($n) == baccarat_banker_win 100003
	mpechoat $n 你押 ３ 籌碼賭莊家贏。
endif
if is_qmark($n) == baccarat_banker_win 100004
	mpechoat $n 你押 ４ 籌碼賭莊家贏。
endif
if is_qmark($n) == baccarat_banker_win 100005
	mpechoat $n 你押 ５ 籌碼賭莊家贏。
endif
if is_qmark($n) == baccarat_player_win 100001
	mpechoat $n 你押 １ 籌碼賭閒家贏。
endif
if is_qmark($n) == baccarat_player_win 100002
	mpechoat $n 你押 ２ 籌碼賭閒家贏。
endif
if is_qmark($n) == baccarat_player_win 100003
	mpechoat $n 你押 ３ 籌碼賭閒家贏。
endif
if is_qmark($n) == baccarat_player_win 100004
	mpechoat $n 你押 ４ 籌碼賭閒家贏。
endif
if is_qmark($n) == baccarat_player_win 100005
	mpechoat $n 你押 ５ 籌碼賭閒家贏。
endif
if is_qmark($n) == baccarat_tie_game 100001
	mpechoat $n 你押 １ 籌碼賭這場和局。
endif
if is_qmark($n) == baccarat_tie_game 100002
	mpechoat $n 你押 ２ 籌碼賭這場和局。
endif
if is_qmark($n) == baccarat_tie_game 100003
	mpechoat $n 你押 ３ 籌碼賭這場和局。
endif
if is_qmark($n) == baccarat_tie_game 100004
	mpechoat $n 你押 ４ 籌碼賭這場和局。
endif
if is_qmark($n) == baccarat_tie_game 100005
	mpechoat $n 你押 ５ 籌碼賭這場和局。
endif
if is_qmark($n) == baccarat_banker_pair 100001
	mpechoat $n 你押 １ 籌碼賭莊家出對子贏。
endif
if is_qmark($n) == baccarat_banker_pair 100002
	mpechoat $n 你押 ２ 籌碼賭莊家出對子贏。
endif
if is_qmark($n) == baccarat_banker_pair 100003
	mpechoat $n 你押 ３ 籌碼賭莊家出對子贏。
endif
if is_qmark($n) == baccarat_banker_pair 100004
	mpechoat $n 你押 ４ 籌碼賭莊家出對子贏。
endif
if is_qmark($n) == baccarat_banker_pair 100005
	mpechoat $n 你押 ５ 籌碼賭莊家出對子贏。
endif
if is_qmark($n) == baccarat_player_pair 100001
	mpechoat $n 你押 １ 籌碼賭閒家出對子贏。
endif
if is_qmark($n) == baccarat_player_pair 100002
	mpechoat $n 你押 ２ 籌碼賭閒家出對子贏。
endif
if is_qmark($n) == baccarat_player_pair 100003
	mpechoat $n 你押 ３ 籌碼賭閒家出對子贏。
endif
if is_qmark($n) == baccarat_player_pair 100004
	mpechoat $n 你押 ４ 籌碼賭閒家出對子贏。
endif
if is_qmark($n) == baccarat_player_pair 100005
	mpechoat $n 你押 ５ 籌碼賭閒家出對子贏。
endif
~
>defun_prog baccarat_show_card~
if is_qmark($n) > baccarat_player1 100000
	mpnonecho 閒家為：
	if is_qmark($n) == baccarat_player1 200001
		mpnonecho [1;31m紅心[0m
	endif
	if is_qmark($n) == baccarat_player1 200002
		mpnonecho [1;31m方塊[0m
	endif
	if is_qmark($n) == baccarat_player1 200003
		mpnonecho [1;30m黑桃[0m
	endif
	if is_qmark($n) == baccarat_player1 200004
		mpnonecho [1;30m梅花[0m
	endif
	if is_qmark($n) & baccarat_player1 100001
		mpnonecho [1;37mＡ[0m
	endif
	if is_qmark($n) & baccarat_player1 100002
		mpnonecho [1;37m２[0m
	endif
	if is_qmark($n) & baccarat_player1 100004
		mpnonecho [1;37m３[0m
	endif
	if is_qmark($n) & baccarat_player1 100008
		mpnonecho [1;37m４[0m
	endif
	if is_qmark($n) & baccarat_player1 100016
		mpnonecho [1;37m５[0m
	endif
	if is_qmark($n) & baccarat_player1 100032
		mpnonecho [1;37m６[0m
	endif
	if is_qmark($n) & baccarat_player1 100064
		mpnonecho [1;37m７[0m
	endif
	if is_qmark($n) & baccarat_player1 100128
		mpnonecho [1;37m８[0m
	endif
	if is_qmark($n) & baccarat_player1 100256
		mpnonecho [1;37m９[0m
	endif
	if is_qmark($n) & baccarat_player1 100512
		mpnonecho [1;37m１０[0m
	endif
	if is_qmark($n) & baccarat_player1 101024
		mpnonecho [1;37mＪ[0m
	endif
	if is_qmark($n) & baccarat_player1 102048
		mpnonecho [1;37mＱ[0m
	endif
	if is_qmark($n) & baccarat_player1 104096
		mpnonecho [1;37mＫ[0m
	endif
endif
if is_qmark($n) > baccarat_player2 100000
	mpnonecho  、 
	if is_qmark($n) == baccarat_player2 200001
		mpnonecho [1;31m紅心[0m
	endif
	if is_qmark($n) == baccarat_player2 200002
		mpnonecho [1;31m方塊[0m
	endif
	if is_qmark($n) == baccarat_player2 200003
		mpnonecho [1;30m黑桃[0m
	endif
	if is_qmark($n) == baccarat_player2 200004
		mpnonecho [1;30m梅花[0m
	endif
	if is_qmark($n) & baccarat_player2 100001
		mpnonecho [1;37mＡ[0m
	endif
	if is_qmark($n) & baccarat_player2 100002
		mpnonecho [1;37m２[0m
	endif
	if is_qmark($n) & baccarat_player2 100004
		mpnonecho [1;37m３[0m
	endif
	if is_qmark($n) & baccarat_player2 100008
		mpnonecho [1;37m４[0m
	endif
	if is_qmark($n) & baccarat_player2 100016
		mpnonecho [1;37m５[0m
	endif
	if is_qmark($n) & baccarat_player2 100032
		mpnonecho [1;37m６[0m
	endif
	if is_qmark($n) & baccarat_player2 100064
		mpnonecho [1;37m７[0m
	endif
	if is_qmark($n) & baccarat_player2 100128
		mpnonecho [1;37m８[0m
	endif
	if is_qmark($n) & baccarat_player2 100256
		mpnonecho [1;37m９[0m
	endif
	if is_qmark($n) & baccarat_player2 100512
		mpnonecho [1;37m１０[0m
	endif
	if is_qmark($n) & baccarat_player2 101024
		mpnonecho [1;37mＪ[0m
	endif
	if is_qmark($n) & baccarat_player2 102048
		mpnonecho [1;37mＱ[0m
	endif
	if is_qmark($n) & baccarat_player2 104096
		mpnonecho [1;37mＫ[0m
	endif
endif
if is_qmark($n) > baccarat_player3 100000
	mpnonecho  、 
	if is_qmark($n) == baccarat_player3 200001
		mpnonecho [1;31m紅心[0m
	endif
	if is_qmark($n) == baccarat_player3 200002
		mpnonecho [1;31m方塊[0m
	endif
	if is_qmark($n) == baccarat_player3 200003
		mpnonecho [1;30m黑桃[0m
	endif
	if is_qmark($n) == baccarat_player3 200004
		mpnonecho [1;30m梅花[0m
	endif
	if is_qmark($n) & baccarat_player3 100001
		mpnonecho [1;37mＡ[0m
	endif
	if is_qmark($n) & baccarat_player3 100002
		mpnonecho [1;37m２[0m
	endif
	if is_qmark($n) & baccarat_player3 100004
		mpnonecho [1;37m３[0m
	endif
	if is_qmark($n) & baccarat_player3 100008
		mpnonecho [1;37m４[0m
	endif
	if is_qmark($n) & baccarat_player3 100016
		mpnonecho [1;37m５[0m
	endif
	if is_qmark($n) & baccarat_player3 100032
		mpnonecho [1;37m６[0m
	endif
	if is_qmark($n) & baccarat_player3 100064
		mpnonecho [1;37m７[0m
	endif
	if is_qmark($n) & baccarat_player3 100128
		mpnonecho [1;37m８[0m
	endif
	if is_qmark($n) & baccarat_player3 100256
		mpnonecho [1;37m９[0m
	endif
	if is_qmark($n) & baccarat_player3 100512
		mpnonecho [1;37m１０[0m
	endif
	if is_qmark($n) & baccarat_player3 101024
		mpnonecho [1;37mＪ[0m
	endif
	if is_qmark($n) & baccarat_player3 102048
		mpnonecho [1;37mＱ[0m
	endif
	if is_qmark($n) & baccarat_player3 104096
		mpnonecho [1;37mＫ[0m
	endif
endif
mpecho .
if is_qmark($n) > baccarat_banker1 100000
	mpnonecho 莊家為：
	if is_qmark($n) == baccarat_banker1 200001
		mpnonecho [1;31m紅心[0m
	endif
	if is_qmark($n) == baccarat_banker1 200002
		mpnonecho [1;31m方塊[0m
	endif
	if is_qmark($n) == baccarat_banker1 200003
		mpnonecho [1;30m黑桃[0m
	endif
	if is_qmark($n) == baccarat_banker1 200004
		mpnonecho [1;30m梅花[0m
	endif
	if is_qmark($n) & baccarat_banker1 100001
		mpnonecho [1;37mＡ[0m
	endif
	if is_qmark($n) & baccarat_banker1 100002
		mpnonecho [1;37m２[0m
	endif
	if is_qmark($n) & baccarat_banker1 100004
		mpnonecho [1;37m３[0m
	endif
	if is_qmark($n) & baccarat_banker1 100008
		mpnonecho [1;37m４[0m
	endif
	if is_qmark($n) & baccarat_banker1 100016
		mpnonecho [1;37m５[0m
	endif
	if is_qmark($n) & baccarat_banker1 100032
		mpnonecho [1;37m６[0m
	endif
	if is_qmark($n) & baccarat_banker1 100064
		mpnonecho [1;37m７[0m
	endif
	if is_qmark($n) & baccarat_banker1 100128
		mpnonecho [1;37m８[0m
	endif
	if is_qmark($n) & baccarat_banker1 100256
		mpnonecho [1;37m９[0m
	endif
	if is_qmark($n) & baccarat_banker1 100512
		mpnonecho [1;37m１０[0m
	endif
	if is_qmark($n) & baccarat_banker1 101024
		mpnonecho [1;37mＪ[0m
	endif
	if is_qmark($n) & baccarat_banker1 102048
		mpnonecho [1;37mＱ[0m
	endif
	if is_qmark($n) & baccarat_banker1 104096
		mpnonecho [1;37mＫ[0m
	endif
endif
if is_qmark($n) > baccarat_banker2 100000
	mpnonecho  、 
	if is_qmark($n) == baccarat_banker2 200001
		mpnonecho [1;31m紅心[0m
	endif
	if is_qmark($n) == baccarat_banker2 200002
		mpnonecho [1;31m方塊[0m
	endif
	if is_qmark($n) == baccarat_banker2 200003
		mpnonecho [1;30m黑桃[0m
	endif
	if is_qmark($n) == baccarat_banker2 200004
		mpnonecho [1;30m梅花[0m
	endif
	if is_qmark($n) & baccarat_banker2 100001
		mpnonecho [1;37mＡ[0m
	endif
	if is_qmark($n) & baccarat_banker2 100002
		mpnonecho [1;37m２[0m
	endif
	if is_qmark($n) & baccarat_banker2 100004
		mpnonecho [1;37m３[0m
	endif
	if is_qmark($n) & baccarat_banker2 100008
		mpnonecho [1;37m４[0m
	endif
	if is_qmark($n) & baccarat_banker2 100016
		mpnonecho [1;37m５[0m
	endif
	if is_qmark($n) & baccarat_banker2 100032
		mpnonecho [1;37m６[0m
	endif
	if is_qmark($n) & baccarat_banker2 100064
		mpnonecho [1;37m７[0m
	endif
	if is_qmark($n) & baccarat_banker2 100128
		mpnonecho [1;37m８[0m
	endif
	if is_qmark($n) & baccarat_banker2 100256
		mpnonecho [1;37m９[0m
	endif
	if is_qmark($n) & baccarat_banker2 100512
		mpnonecho [1;37m１０[0m
	endif
	if is_qmark($n) & baccarat_banker2 101024
		mpnonecho [1;37mＪ[0m
	endif
	if is_qmark($n) & baccarat_banker2 102048
		mpnonecho [1;37mＱ[0m
	endif
	if is_qmark($n) & baccarat_banker2 104096
		mpnonecho [1;37mＫ[0m
	endif
endif
if is_qmark($n) > baccarat_banker3 100000
	mpnonecho  、 
	if is_qmark($n) == baccarat_banker3 200001
		mpnonecho [1;31m紅心[0m
	endif
	if is_qmark($n) == baccarat_banker3 200002
		mpnonecho [1;31m方塊[0m
	endif
	if is_qmark($n) == baccarat_banker3 200003
		mpnonecho [1;30m黑桃[0m
	endif
	if is_qmark($n) == baccarat_banker3 200004
		mpnonecho [1;30m梅花[0m
	endif
	if is_qmark($n) & baccarat_banker3 100001
		mpnonecho [1;37mＡ[0m
	endif
	if is_qmark($n) & baccarat_banker3 100002
		mpnonecho [1;37m２[0m
	endif
	if is_qmark($n) & baccarat_banker3 100004
		mpnonecho [1;37m３[0m
	endif
	if is_qmark($n) & baccarat_banker3 100008
		mpnonecho [1;37m４[0m
	endif
	if is_qmark($n) & baccarat_banker3 100016
		mpnonecho [1;37m５[0m
	endif
	if is_qmark($n) & baccarat_banker3 100032
		mpnonecho [1;37m６[0m
	endif
	if is_qmark($n) & baccarat_banker3 100064
		mpnonecho [1;37m７[0m
	endif
	if is_qmark($n) & baccarat_banker3 100128
		mpnonecho [1;37m８[0m
	endif
	if is_qmark($n) & baccarat_banker3 100256
		mpnonecho [1;37m９[0m
	endif
	if is_qmark($n) & baccarat_banker3 100512
		mpnonecho [1;37m１０[0m
	endif
	if is_qmark($n) & baccarat_banker3 101024
		mpnonecho [1;37mＪ[0m
	endif
	if is_qmark($n) & baccarat_banker3 102048
		mpnonecho [1;37mＱ[0m
	endif
	if is_qmark($n) & baccarat_banker3 104096
		mpnonecho [1;37mＫ[0m
	endif
endif
mpecho .
~
>defun_prog baccarat_show_card2~
if is_qmark($n) > baccarat_player3 100000
	mpnonecho $I快速的從牌盒中取出一張 
	if is_qmark($n) == baccarat_player3 200001
		mpnonecho [1;31m紅心[0m 
	endif
	if is_qmark($n) == baccarat_player3 200002
		mpnonecho [1;31m方塊[0m 
	endif
	if is_qmark($n) == baccarat_player3 200003
		mpnonecho [1;30m黑桃[0m 
	endif
	if is_qmark($n) == baccarat_player3 200004
		mpnonecho [1;30m梅花[0m 
	endif
	if is_qmark($n) & baccarat_player3 100001
		mpnonecho [1;37mＡ[0m 
	endif
	if is_qmark($n) & baccarat_player3 100002
		mpnonecho [1;37m２[0m 
	endif
	if is_qmark($n) & baccarat_player3 100004
		mpnonecho [1;37m３[0m 
	endif
	if is_qmark($n) & baccarat_player3 100008
		mpnonecho [1;37m４[0m 
	endif
	if is_qmark($n) & baccarat_player3 100016
		mpnonecho [1;37m５[0m 
	endif
	if is_qmark($n) & baccarat_player3 100032
		mpnonecho [1;37m６[0m 
	endif
	if is_qmark($n) & baccarat_player3 100064
		mpnonecho [1;37m７[0m 
	endif
	if is_qmark($n) & baccarat_player3 100128
		mpnonecho [1;37m８[0m 
	endif
	if is_qmark($n) & baccarat_player3 100256
		mpnonecho [1;37m９[0m 
	endif
	if is_qmark($n) & baccarat_player3 100512
		mpnonecho [1;37m１０[0m 
	endif
	if is_qmark($n) & baccarat_player3 101024
		mpnonecho [1;37mＪ[0m 
	endif
	if is_qmark($n) & baccarat_player3 102048
		mpnonecho [1;37mＱ[0m 
	endif
	if is_qmark($n) & baccarat_player3 104096
		mpnonecho [1;37mＫ[0m 
	endif
endif
mpecho 發給閒家。
~
>defun_prog baccarat_show_card3~
if is_qmark($n) > baccarat_banker3 100000
	mpnonecho $I快速的從牌盒中取出一張 
	if is_qmark($n) == baccarat_banker3 200001
		mpnonecho [1;31m紅心[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200002
		mpnonecho [1;31m方塊[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200003
		mpnonecho [1;30m黑桃[0m 
	endif
	if is_qmark($n) == baccarat_banker3 200004
		mpnonecho [1;30m梅花[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100001
		mpnonecho [1;37mＡ[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100002
		mpnonecho [1;37m２[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100004
		mpnonecho [1;37m３[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100008
		mpnonecho [1;37m４[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100016
		mpnonecho [1;37m５[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100032
		mpnonecho [1;37m６[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100064
		mpnonecho [1;37m７[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100128
		mpnonecho [1;37m８[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100256
		mpnonecho [1;37m９[0m 
	endif
	if is_qmark($n) & baccarat_banker3 100512
		mpnonecho [1;37m１０[0m 
	endif
	if is_qmark($n) & baccarat_banker3 101024
		mpnonecho [1;37mＪ[0m 
	endif
	if is_qmark($n) & baccarat_banker3 102048
		mpnonecho [1;37mＱ[0m 
	endif
	if is_qmark($n) & baccarat_banker3 104096
		mpnonecho [1;37mＫ[0m 
	endif
endif
mpecho 發給莊家。
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
	say 閒家補牌。
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
	say 莊家補牌。
	mpcall baccarat_banker_random_3 $i
	break
endif
if is_qmark($i) == baccarat_banker_point 100003
	if is_qmark($i) & baccarat_player3 100128
	else
		say 莊家補牌。
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
		say 莊家補牌。
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
		say 莊家補牌。
		mpcall baccarat_banker_random_3 $i
		break
	endif
endif
if is_qmark($i) == baccarat_banker_point 100006
	if is_qmark($i) > baccarat_player3 100000
		if is_qmark($i) & baccarat_player3 100032
		or is_qmark($i) & baccarat_player3 100064
			say 莊家補牌。
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
	mpnonecho $I[1;32m說著「閒家 [1;37m０[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100001
	mpnonecho $I[1;32m說著「閒家 [1;37m１[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100002
	mpnonecho $I[1;32m說著「閒家 [1;37m２[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100003
	mpnonecho $I[1;32m說著「閒家 [1;37m３[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100004
	mpnonecho $I[1;32m說著「閒家 [1;37m４[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100005
	mpnonecho $I[1;32m說著「閒家 [1;37m５[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100006
	mpnonecho $I[1;32m說著「閒家 [1;37m６[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100007
	mpnonecho $I[1;32m說著「閒家 [1;37m７[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100008
	mpnonecho $I[1;32m說著「閒家 [1;37m８[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_player_point 100009
	mpnonecho $I[1;32m說著「閒家 [1;37m９[1;32m 點，[0m
endif
if is_qmark($i) == baccarat_banker_point 100000
	mpecho [1;32m莊家 [1;37m０[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100001
	mpecho [1;32m莊家 [1;37m１[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100002
	mpecho [1;32m莊家 [1;37m２[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100003
	mpecho [1;32m莊家 [1;37m３[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100004
	mpecho [1;32m莊家 [1;37m４[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100005
	mpecho [1;32m莊家 [1;37m５[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100006
	mpecho [1;32m莊家 [1;37m６[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100007
	mpecho [1;32m莊家 [1;37m７[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100008
	mpecho [1;32m莊家 [1;37m８[1;32m 點。」[0m
endif
if is_qmark($i) == baccarat_banker_point 100009
	mpecho [1;32m莊家 [1;37m９[1;32m 點。」[0m
endif
~
>defun_prog baccarat_end_game~
if is_qmark($i) == baccarat_player_point 100000
	if is_qmark($i) > baccarat_banker_point 100000
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100000
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
endif
if is_qmark($i) == baccarat_player_point 100001
	if is_qmark($i) > baccarat_banker_point 100001
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100002
	if is_qmark($i) > baccarat_banker_point 100001
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100003
	if is_qmark($i) > baccarat_banker_point 100001
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100004
	if is_qmark($i) > baccarat_banker_point 100001
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100005
	if is_qmark($i) > baccarat_banker_point 100001
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100001
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100001
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100006
	if is_qmark($i) > baccarat_banker_point 100006
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100006
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100006
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100007
	if is_qmark($i) > baccarat_banker_point 100007
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100007
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100007
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100008
	if is_qmark($i) > baccarat_banker_point 100008
		say 莊家贏。
		mpforcer here modrvt v0 | 1
	endif
	if is_qmark($i) == baccarat_banker_point 100008
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100008
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
if is_qmark($i) == baccarat_player_point 100009
	if is_qmark($i) == baccarat_banker_point 100009
		say 和局。
		mpforcer here modrvt v0 | 4
	endif
	if is_qmark($i) < baccarat_banker_point 100009
		say 閒家贏。
		mpforcer here modrvt v0 | 2
	endif
endif
mpforcer here rcall baccarat_ending_transfor $i
~
>defun_prog baccarat_reset_all~
mpecho 遊戲結束後$I整理完桌面，準備開始下一局。
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
tell $n 想了解百家樂的玩法？(baccarat-rule)
~
>talk_prog p baccarat-rule~
tell $n 百家樂是一種完全過程完全由荷官進行的遊戲，參加的玩家只需要下注到想要的位置就行了。
tell $n 下注的方法為 bet '位置 酬碼數量'，可以每個位置都下注，但同場遊戲內每人最多下注 5 酬碼為上限。
tell $n 在遊戲內能隨時使用 check 來看自己下注的狀態及目前開牌的情況。
tell $n 下注位置有：莊家贏(Banker)、閒家贏(Player)、和局(Tie)、莊家出對子(Banker-Pair)、閒家出對子(Player-Pair)。
tell $n 只要下注成功即可加入該場遊戲。
tell $n 如果想了解更多百家樂相關的規則可以繼續問我。(baccarat-rule2)
~
>talk_prog p baccarat-rule2~
tell $n 百家樂是一種以莊、閒點數比大小的紙牌遊戲，點數總合大的一方能贏得比賽。
tell $n 數字牌的部分 ２ 到 ９ 的點數與本身的數字相同，而 １０ 則代表 0 點。
tell $n 文字牌的部分，Ａ 代表 1 點，Ｊ、Ｑ、Ｋ 則代表 0 點。
tell $n 點數計算是將莊家或閒家各自持有的手牌點數加總，但加起來最大不能超過 9。
tell $n 如果總合超過 9 則以總合後的個位數當成最終的點數。
tell $n 例：Ａ + ９ = 0 點, ９ + ９ = 8 點, １０ + Ｋ = 0 點, ８ + ６ = 4 點。
tell $n 簡單來講，在遊戲內最大的點數是 9 ，最小的則為 0 點。
tell $n ------------------------------------------------------------------------------
tell $n 百家樂在遊戲開始進行時，荷官會分別派牌兩張牌至閒家(player)與莊家(banker)的欄位上並且開牌。
tell $n 開牌後進行第一次的點數加總並且判定所得的點數是否進行特殊規則補牌。
tell $n 雙方補牌最多僅會有一張，簡單來講，如果需要補牌，在雙方持有第三張牌的時候必定會有結果。
tell $n 而是否需要補牌，則由開牌後的點數來判定：
tell $n ------------------------------------------------------------------------------
tell $n 只要莊、閒任一出現兩張牌的點數和為 8 或 9 時，雙方都不進行補牌，直接比較點數後結束遊戲。
tell $n 閒家在點數小於 5 的情況下，需要補牌，大於 6 則不補牌。
tell $n 莊家在點數小於 2 的情況下，需要補牌。
tell $n 莊家點數為 3 時，莊家需要補牌，除了閒家的第三張牌為 8 點時，莊家不需要補牌。
tell $n 莊家點數為 4 時，閒家的第三張牌為 0, 1, 8, 9 時，莊家不需補牌，其餘情況要補牌。
tell $n 莊家點數為 5 時，閒家的第三張牌為 0, 1, 2, 3, 8, 9 時，莊家不需補牌，其餘情況要補牌。
tell $n 莊家點數為 6 時，閒家的第三張牌為 6 或 7 點時，莊家需要補牌，其餘情況不需要補牌。
tell $n 莊家點數大於 7 以上時則不需要補牌。
tell $n 百家樂的補牌並不一定是一種優勢，反而可能會使點數降低。
tell $n ------------------------------------------------------------------------------
tell $n 勝負只會有三種情況：最後點數，當莊家大於閒家時，莊家贏，反之則是閒家贏。
tell $n 如果莊、閒的最後點數相同則和局(tie)，此情況下注莊、閒的籌碼將退還。
tell $n 但要注意押莊家贏且莊家真得贏的情況下，贏得的部分會被抽取 5% 佣金。
tell $n 對子(pair)則依莊、閒開牌後的兩張牌，不看花色只看數字或文字是否成對。
tell $n 百家樂中所有的押注能同時存在，只要條件達成就能得到報酬。
tell $n 如果還想了解各位置的賠率可以繼續問我。(baccarat-rule3)
~
>talk_prog p baccarat-rule3~
tell $n 每次下注為 1 單位，同場遊戲內每位閒家最多下注 5 次。
tell $n ---------------------------------------------
tell $n 閒家出對子(Player-Pair)           1 賠 11
tell $n 莊家出對子(Banker-Pair)           1 賠 11
tell $n 和局(Tie)                         1 賠 8
tell $n 閒家贏(Player)                    1 賠 1
tell $n 莊家贏(Banker)                    1 賠 0.95
tell $n 如果賠率的結果小於 1 單位，則給於等值小於 1 單位的籌碼。
tell $n ---------------------------------------------
tell $n 另外本桌臺能兌換手上大小不同的籌碼(chips)。
~
>talk_prog chips~
tell $n 籌碼種共有五種面額分別是 100，50，25，10 及 5。
tell $n 如果想把手上所有小額籌碼換成大的，直接跟我說(change bigger)。
tell $n 舉例：有四個 10 及四個 25 面額的酬碼，告訴我 change bigger。
tell $n 我將換給你一個 100 ，一個 25 ，一個 10 及 一個 5 面額的籌碼。
tell $n 如果想把特定面額的籌碼換成最大面額的，可以直接跟我說(change 籌碼面額)。
tell $n 舉例：有六個 25 面額的酬碼，告訴我 change 25。
tell $n 我將換給你一個 100 及一個 50 的籌碼。
tell $n 如果想把特定面額的籌碼換成特定面額的，可以直接跟我說(change 欲換籌碼面額to特定籌碼面額)。
tell $n 舉例：有五個 25 面額的酬碼，告訴我 change 25to100。
tell $n 我將換給你一個 100 及一個 25 的籌碼。
tell $n 或是有一個 100 的酬碼想換成兩個 50 的籌碼，可以告訴我 change 100to50。
tell $n 就算你手持的數量不足，我也會幫你洗碼。
tell $n 但是如果提出像是 100to100，50to50，change 100之類的無聊要求。
tell $n 我是不會理你的。
~
>talk_prog p change bigger~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall chips_count $n
mpforcer here rcall chips_change_all $n
tell $n 所有籌碼兌換完成。
~
>talk_prog p change 100to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_50 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
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
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_25 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
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
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_10 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_10 $n
tell $n 籌碼兌換完成。
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
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
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
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n 籌碼兌換完成。
mpdel_qmark $n casino_chips_count
~
|