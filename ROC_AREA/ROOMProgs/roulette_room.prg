>room_defun roulette_timeline~
if roomvalue($1) == 0
	break
endif
if roomvalue($1) == 1
	force $n say 遊戲準備開始，還有人要下注嗎？
endif
if roomvalue($1) < 12
	modrvt v1 + 1
	if roomvalue($1) == 11
		echo 叮，叮！
		force $n say 最後下注時間！
	endif
endif
if roomvalue($1) == 12
	echo 叮！
	say 遊戲開始！
	rcall roulette_interval_1 $n
	modrvt v1 = 13
endif
~
>room_defun roulette_reset_all~
echo 遊戲結束後$N整理完桌面，準備開始下一局。
mod_qmark $n roulette_dealer v0 = 0
modrvt v0 = 0
modrvt v1 = 0
~
>room_defun roulette_interval_1~
echo $N以兩指夾起銀色的小鋼珠，循著輪盤的軌道使勁滑出。
echo 轉眼間小鋼珠以肉眼難以辨識的速度在輪盤上劃出一道銀色的軌跡。
callout 1 echo 唰、唰、唰.....鋼珠與輪盤磨擦發出響亮的聲音。
callout 2 echo 唰、唰、唰...
callout 3 echo 唰、唰..唰啦...鋼珠的速度逐漸緩慢下來。
callout 4 echo 唰啦..唰啦..喀搭....
callout 5 echo 喀搭..唰啦..喀搭...鋼珠的速度慢到隨時會從軌道掉下。
callout 6 echo 喀搭..喀搭..喀！ 鋼珠從軌道掉下！開始在輪盤上彈跳！
callout 7 echo 喀喀、喀喀、喀！
callout 8 echo 最後落到的號碼是.....
callout 9 rcall roulette_interval_2 $n
~
>room_defun roulette_interval_2~
if rand(50)
	if rand(11)
		rcall roulette_random_1 $n
	else
		if rand(13)
			rcall roulette_random_2 $n
		else
			if rand(14)
				rcall roulette_random_3 $n
			else
				if rand(17)
					rcall roulette_random_4 $n
				else
					if rand(20)
						rcall roulette_random_5 $n
					else
						if rand(25)
							rcall roulette_random_6 $n
						else
							if rand(33)
								rcall roulette_random_7 $n
							else
								if rand(50)
									rcall roulette_random_8 $n
								else
									rcall roulette_random_9 $n
								endif
							endif
						endif
					endif
				endif
			endif
		endif
	endif
else
	if rand(11)
		rcall roulette_random_9 $n
	else
		if rand(13)
			rcall roulette_random_8 $n
		else
			if rand(14)
				rcall roulette_random_7 $n
			else
				if rand(17)
					rcall roulette_random_6 $n
				else
					if rand(20)
						rcall roulette_random_5 $n
					else
						if rand(25)
							rcall roulette_random_4 $n
						else
							if rand(33)
								rcall roulette_random_3 $n
							else
								if rand(50)
									rcall roulette_random_2 $n
								else
									rcall roulette_random_1 $n
								endif
							endif
						endif
					endif
				endif
			endif
		endif
	endif
endif
~
>room_defun roulette_win_1~
if is_qmark($n) & roulette_bet_single 100001
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100001
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100001
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_2~
if is_qmark($n) & roulette_bet_single 100002
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100001
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100001
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_3~
if is_qmark($n) & roulette_bet_single 100004
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100001
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100001
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_4~
if is_qmark($n) & roulette_bet_single 100008
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100002
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100001
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100002
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_5~
if is_qmark($n) & roulette_bet_single 100016
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100002
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100001
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100002
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_6~
if is_qmark($n) & roulette_bet_single 100032
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 100512
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100002
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200001
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100001
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100002
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_7~
if is_qmark($n) & roulette_bet_single 100064
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 101024
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 102048
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100004
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100002
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100004
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_8~
if is_qmark($n) & roulette_bet_single 100128
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 101024
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 104096
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 108192
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100004
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100002
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100004
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_9~
if is_qmark($n) & roulette_bet_single 100256
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 100512
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 104096
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 116384
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100004
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200002
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100002
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100004
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_10~
if is_qmark($n) & roulette_bet_single 100512
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 102048
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 132768
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100008
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100004
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100008
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_11~
if is_qmark($n) & roulette_bet_single 200001
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 108192
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 132768
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100008
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100004
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100008
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_12~
if is_qmark($n) & roulette_bet_single 200002
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 116384
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100008
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200004
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100004
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100008
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100008
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_13~
if is_qmark($n) & roulette_bet_single 200004
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 200001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100016
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100008
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100016
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_14~
if is_qmark($n) & roulette_bet_single 200008
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 200004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100016
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100008
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100016
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_15~
if is_qmark($n) & roulette_bet_single 200016
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 200008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100016
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200008
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100008
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100016
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_16~
if is_qmark($n) & roulette_bet_single 200032
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 200032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200512
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 201024
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100032
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100016
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100032
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_17~
if is_qmark($n) & roulette_bet_single 200064
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 200128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 200512
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 202048
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 204096
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100032
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100016
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100032
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_18~
if is_qmark($n) & roulette_bet_single 200128
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 200256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 202048
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 208192
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100032
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200016
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100016
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100032
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100064
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_19~
if is_qmark($n) & roulette_bet_single 200256
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 201024
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 216384
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 232768
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100064
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100032
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100064
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_20~
if is_qmark($n) & roulette_bet_single 200512
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 204096
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 216384
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100064
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100032
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100064
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_21~
if is_qmark($n) & roulette_bet_single 300001
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 208192
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100064
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200032
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100032
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100064
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_22~
if is_qmark($n) & roulette_bet_single 300002
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 232768
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100128
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100064
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100128
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_23~
if is_qmark($n) & roulette_bet_single 300004
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 300002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100128
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100064
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100128
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_24~
if is_qmark($n) & roulette_bet_single 300008
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 300004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100128
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200064
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100064
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100128
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100016
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_25~
if is_qmark($n) & roulette_bet_single 300016
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 300016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300512
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100256
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100128
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100256
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_26~
if is_qmark($n) & roulette_bet_single 300032
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 300064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 300256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 301024
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 302048
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100256
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100128
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100256
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_27~
if is_qmark($n) & roulette_bet_single 300064
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 300128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 301024
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 304096
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100256
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200128
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100128
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100256
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_28~
if is_qmark($n) & roulette_bet_single 300128
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 300512
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 308192
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 316384
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100512
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100256
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100512
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_29~
if is_qmark($n) & roulette_bet_single 300256
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 302048
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 308192
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 332768
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100512
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 100512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100256
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100512
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_30~
if is_qmark($n) & roulette_bet_single 300512
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 304096
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split 332768
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 100512
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200256
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100256
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 100512
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_31~
if is_qmark($n) & roulette_bet_single 301024
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split 316384
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 101024
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 101024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100512
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 101024
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_32~
if is_qmark($n) & roulette_bet_single 302048
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split_2 100001
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100004
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 101024
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 100512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 101024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 200512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 201024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100512
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 101024
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_33~
if is_qmark($n) & roulette_bet_single 304096
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split_2 100002
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100016
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 101024
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 200512
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 201024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 100512
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_line 101024
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_34~
if is_qmark($n) & roulette_bet_single 308192
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split_2 100008
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 102048
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 101024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 101024
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100001
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_35~
if is_qmark($n) & roulette_bet_single 316384
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split_2 100032
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100128
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 102048
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 101024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_corner 201024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 101024
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100002
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100512
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 101024
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_win_36~
if is_qmark($n) & roulette_bet_single 332768
	mod_qmark $n roulette_bet v1 + 36
endif
if is_qmark($n) & roulette_bet_split_2 100064
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_split_2 100256
	mod_qmark $n roulette_bet v1 + 18
endif
if is_qmark($n) & roulette_bet_street 102048
	mod_qmark $n roulette_bet v1 + 12
endif
if is_qmark($n) & roulette_bet_corner 201024
	mod_qmark $n roulette_bet v1 + 9
endif
if is_qmark($n) & roulette_bet_line 101024
	mod_qmark $n roulette_bet v1 + 6
endif
if is_qmark($n) & roulette_bet_other 100004
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100032
	mod_qmark $n roulette_bet v1 + 3
endif
if is_qmark($n) & roulette_bet_other 100256
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 102048
	mod_qmark $n roulette_bet v1 + 2
endif
if is_qmark($n) & roulette_bet_other 100128
	mod_qmark $n roulette_bet v1 + 2
endif
~
>room_defun roulette_check_win~
if roomvalue($0) == 0
	if is_qmark($n) == roulette_bet_single_0
		mod_qmark $n roulette_bet v1 + 36
	endif
else
	if roomvalue($0) == 1
		rcall roulette_win_1 $n
	endif
	if roomvalue($0) == 2
		rcall roulette_win_2 $n
	endif
	if roomvalue($0) == 3
		rcall roulette_win_3 $n
	endif
	if roomvalue($0) == 4
		rcall roulette_win_4 $n
	endif
	if roomvalue($0) == 5
		rcall roulette_win_5 $n
	endif
	if roomvalue($0) == 6
		rcall roulette_win_6 $n
	endif
	if roomvalue($0) == 7
		rcall roulette_win_7 $n
	endif
	if roomvalue($0) == 8
		rcall roulette_win_8 $n
	endif
	if roomvalue($0) == 9
		rcall roulette_win_9 $n
	endif
	if roomvalue($0) == 10
		rcall roulette_win_10 $n
	endif
	if roomvalue($0) == 11
		rcall roulette_win_11 $n
	endif
	if roomvalue($0) == 12
		rcall roulette_win_12 $n
	endif
	if roomvalue($0) == 13
		rcall roulette_win_13 $n
	endif
	if roomvalue($0) == 14
		rcall roulette_win_14 $n
	endif
	if roomvalue($0) == 15
		rcall roulette_win_15 $n
	endif
	if roomvalue($0) == 16
		rcall roulette_win_16 $n
	endif
	if roomvalue($0) == 17
		rcall roulette_win_17 $n
	endif
	if roomvalue($0) == 18
		rcall roulette_win_18 $n
	endif
	if roomvalue($0) == 19
		rcall roulette_win_19 $n
	endif
	if roomvalue($0) == 20
		rcall roulette_win_20 $n
	endif
	if roomvalue($0) == 21
		rcall roulette_win_21 $n
	endif
	if roomvalue($0) == 22
		rcall roulette_win_22 $n
	endif
	if roomvalue($0) == 23
		rcall roulette_win_23 $n
	endif
	if roomvalue($0) == 24
		rcall roulette_win_24 $n
	endif
	if roomvalue($0) == 25
		rcall roulette_win_25 $n
	endif
	if roomvalue($0) == 26
		rcall roulette_win_26 $n
	endif
	if roomvalue($0) == 27
		rcall roulette_win_27 $n
	endif
	if roomvalue($0) == 28
		rcall roulette_win_28 $n
	endif
	if roomvalue($0) == 29
		rcall roulette_win_29 $n
	endif
	if roomvalue($0) == 30
		rcall roulette_win_30 $n
	endif
	if roomvalue($0) == 31
		rcall roulette_win_31 $n
	endif
	if roomvalue($0) == 32
		rcall roulette_win_32 $n
	endif
	if roomvalue($0) == 33
		rcall roulette_win_33 $n
	endif
	if roomvalue($0) == 34
		rcall roulette_win_34 $n
	endif
	if roomvalue($0) == 35
		rcall roulette_win_35 $n
	endif
	if roomvalue($0) == 36
		rcall roulette_win_36 $n
	endif
endif
if is_qmark($n) >= roulette_bet 200001
	echoat $n 你壓的位置中獎了！
	echoat $n 莊家把相應賠率的報酬給了你。
	rcall roulette_take_win $n
endif
if is_qmark($n) == roulette_bet_single_0
	del_qmark $n roulette_bet_single_0
endif
if is_qmark($n) == roulette_bet
	del_qmark $n roulette_bet
endif
del_qmark $n roulette_bet_single
del_qmark $n roulette_bet_split
del_qmark $n roulette_bet_street
del_qmark $n roulette_bet_corner
del_qmark $n roulette_bet_line
del_qmark $n roulette_bet_other
~
>room_enter 64 100~
if is_qmark($n) == roulette_bet
else
	break
endif
if is_qmark($n) == roulette_dealer
	break
endif
if roomvalue($1) == 14
	rcall roulette_check_win $n
endif
~
>room_leave 511 100~
if is_qmark($n) == roulette_bet
else
	break
endif
if is_qmark($n) == roulette_dealer
	break
endif
if roomvalue($1) == 14
	break
endif
echoat $n 你放棄了遊戲，下注的籌碼一去不復反。
del_qmark $n roulette_bet
del_qmark $n roulette_bet_single
del_qmark $n roulette_bet_split
del_qmark $n roulette_bet_street
del_qmark $n roulette_bet_corner
del_qmark $n roulette_bet_line
del_qmark $n roulette_bet_other
~
>room_command bet default~
echoat $n 你要下注在哪邊？
echoat $n bet 'single [1;37m號碼[0m'
echoat $n bet 'split [1;37m號碼1[0m,[1;37m號碼2[0m'
echoat $n bet 'corner down[1;37m數字[0m' 或 bet 'corner upper[1;37m數字[0m'
echoat $n bet 'street [1;37m最小號碼[0mto[1;37m最大號碼[0m'
echoat $n bet 'line [1;37m最小號碼[0mto[1;37m最大號碼[0m'
echoat $n bet 'column 第幾行'
echoat $n bet 'dozen 第幾打'
echoat $n bet 1to18
echoat $n bet 19to36
echoat $n bet even
echoat $n bet odd
echoat $n bet red
echoat $n bet black
~
>room_command bet p single 0~
if is_qmark($n) == roulette_bet_single_0
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	add_qmark $n roulette_bet_single_0 -1 0 0 0 1 0
	echoat $n 你在０號下注。
	echoaround $n $N在０號下注。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 1~
if is_qmark($n) & roulette_bet_single 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 1
	echoat $n 你下注在１號。
	echoaround $n $N下注在１號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 2~
if is_qmark($n) & roulette_bet_single 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 2
	echoat $n 你下注在２號。
	echoaround $n $N下注在２號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 3~
if is_qmark($n) & roulette_bet_single 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 4
	echoat $n 你下注在３號。
	echoaround $n $N下注在３號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 4~
if is_qmark($n) & roulette_bet_single 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 8
	echoat $n 你下注在４號。
	echoaround $n $N下注在４號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 5~
if is_qmark($n) & roulette_bet_single 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 16
	echoat $n 你下注在５號。
	echoaround $n $N下注在５號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 6~
if is_qmark($n) & roulette_bet_single 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 32
	echoat $n 你下注在６號。
	echoaround $n $N下注在６號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 7~
if is_qmark($n) & roulette_bet_single 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 64
	echoat $n 你下注在７號。
	echoaround $n $N下注在７號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 8~
if is_qmark($n) & roulette_bet_single 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 128
	echoat $n 你下注在８號。
	echoaround $n $N下注在８號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 9~
if is_qmark($n) & roulette_bet_single 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 256
	echoat $n 你下注在９號。
	echoaround $n $N下注在９號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 10~
if is_qmark($n) & roulette_bet_single 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v0 | 512
	echoat $n 你下注在１０號。
	echoaround $n $N下注在１０號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 11~
if is_qmark($n) & roulette_bet_single 200001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 1
	echoat $n 你下注在１１號。
	echoaround $n $N下注在１１號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 12~
if is_qmark($n) & roulette_bet_single 200002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 2
	echoat $n 你下注在１２號。
	echoaround $n $N下注在１２號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 13~
if is_qmark($n) & roulette_bet_single 200004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 4
	echoat $n 你下注在１３號。
	echoaround $n $N下注在１３號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 14~
if is_qmark($n) & roulette_bet_single 200008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 8
	echoat $n 你下注在１４號。
	echoaround $n $N下注在１４號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 15~
if is_qmark($n) & roulette_bet_single 200016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 16
	echoat $n 你下注在１５號。
	echoaround $n $N下注在１５號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 16~
if is_qmark($n) & roulette_bet_single 200032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 32
	echoat $n 你下注在１６號。
	echoaround $n $N下注在１６號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 17~
if is_qmark($n) & roulette_bet_single 200064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 64
	echoat $n 你下注在１７號。
	echoaround $n $N下注在１７號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 18~
if is_qmark($n) & roulette_bet_single 200128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 128
	echoat $n 你下注在１８號。
	echoaround $n $N下注在１８號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 19~
if is_qmark($n) & roulette_bet_single 200256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 256
	echoat $n 你下注在１９號。
	echoaround $n $N下注在１９號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 20~
if is_qmark($n) & roulette_bet_single 200512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v1 | 512
	echoat $n 你下注在２０號。
	echoaround $n $N下注在２０號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 21~
if is_qmark($n) & roulette_bet_single 300001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 1
	echoat $n 你下注在２１號。
	echoaround $n $N下注在２１號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 22~
if is_qmark($n) & roulette_bet_single 300002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 2
	echoat $n 你下注在２２號。
	echoaround $n $N下注在２２號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 23~
if is_qmark($n) & roulette_bet_single 300004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 4
	echoat $n 你下注在２３號。
	echoaround $n $N下注在２３號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 24~
if is_qmark($n) & roulette_bet_single 300008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 8
	echoat $n 你下注在２４號。
	echoaround $n $N下注在２４號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 25~
if is_qmark($n) & roulette_bet_single 300016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 16
	echoat $n 你下注在２５號。
	echoaround $n $N下注在２５號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 26~
if is_qmark($n) & roulette_bet_single 300032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 32
	echoat $n 你下注在２６號。
	echoaround $n $N下注在２６號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 27~
if is_qmark($n) & roulette_bet_single 300064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 64
	echoat $n 你下注在２７號。
	echoaround $n $N下注在２７號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 28~
if is_qmark($n) & roulette_bet_single 300128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 128
	echoat $n 你下注在２８號。
	echoaround $n $N下注在２８號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 29~
if is_qmark($n) & roulette_bet_single 300256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 256
	echoat $n 你下注在２９號。
	echoaround $n $N下注在２９號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 30~
if is_qmark($n) & roulette_bet_single 300512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 512
	echoat $n 你下注在３０號。
	echoaround $n $N下注在３０號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 31~
if is_qmark($n) & roulette_bet_single 301024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 1024
	echoat $n 你下注在３１號。
	echoaround $n $N下注在３１號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 32~
if is_qmark($n) & roulette_bet_single 302048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 2048
	echoat $n 你下注在３２號。
	echoaround $n $N下注在３２號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 33~
if is_qmark($n) & roulette_bet_single 304096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 4096
	echoat $n 你下注在３３號。
	echoaround $n $N下注在３３號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 34~
if is_qmark($n) & roulette_bet_single 308192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 8192
	echoat $n 你下注在３４號。
	echoaround $n $N下注在３４號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 35~
if is_qmark($n) & roulette_bet_single 316384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 16384
	echoat $n 你下注在３５號。
	echoaround $n $N下注在３５號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p single 36~
if is_qmark($n) & roulette_bet_single 332768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_single v2 | 32768
	echoat $n 你下注在３６號。
	echoaround $n $N下注在３６號。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 1,2~
if is_qmark($n) & roulette_bet_split 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 1
	echoat $n 你下注在１跟２號之間。
	echoaround $n $N下注在１跟２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 2,1~
if is_qmark($n) & roulette_bet_split 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 1
	echoat $n 你下注在１跟２號之間。
	echoaround $n $N下注在１跟２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 1,4~
if is_qmark($n) & roulette_bet_split 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 2
	echoat $n 你下注在１跟４號之間。
	echoaround $n $N下注在１跟４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 4,1~
if is_qmark($n) & roulette_bet_split 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 2
	echoat $n 你下注在１跟４號之間。
	echoaround $n $N下注在１跟４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 2,3~
if is_qmark($n) & roulette_bet_split 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 4
	echoat $n 你下注在２跟３號之間。
	echoaround $n $N下注在２跟３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 3,2~
if is_qmark($n) & roulette_bet_split 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 4
	echoat $n 你下注在２跟３號之間。
	echoaround $n $N下注在２跟３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 2,5~
if is_qmark($n) & roulette_bet_split 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 8
	echoat $n 你下注在２跟５號之間。
	echoaround $n $N下注在２跟５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 5,2~
if is_qmark($n) & roulette_bet_split 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 8
	echoat $n 你下注在２跟５號之間。
	echoaround $n $N下注在２跟５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 3,6~
if is_qmark($n) & roulette_bet_split 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 16
	echoat $n 你下注在３跟６號之間。
	echoaround $n $N下注在３跟６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 6,3~
if is_qmark($n) & roulette_bet_split 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 16
	echoat $n 你下注在３跟６號之間。
	echoaround $n $N下注在３跟６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 4,5~
if is_qmark($n) & roulette_bet_split 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 32
	echoat $n 你下注在４跟５號之間。
	echoaround $n $N下注在４跟５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 5,4~
if is_qmark($n) & roulette_bet_split 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 32
	echoat $n 你下注在４跟５號之間。
	echoaround $n $N下注在４跟５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 4,7~
if is_qmark($n) & roulette_bet_split 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 64
	echoat $n 你下注在４跟７號之間。
	echoaround $n $N下注在４跟７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 7,4~
if is_qmark($n) & roulette_bet_split 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 64
	echoat $n 你下注在４跟７號之間。
	echoaround $n $N下注在４跟７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 5,6~
if is_qmark($n) & roulette_bet_split 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 128
	echoat $n 你下注在５跟６號之間。
	echoaround $n $N下注在５跟６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 6,5~
if is_qmark($n) & roulette_bet_split 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 128
	echoat $n 你下注在５跟６號之間。
	echoaround $n $N下注在５跟６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 5,8~
if is_qmark($n) & roulette_bet_split 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 256
	echoat $n 你下注在５跟８號之間。
	echoaround $n $N下注在５跟８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 8,5~
if is_qmark($n) & roulette_bet_split 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 256
	echoat $n 你下注在５跟８號之間。
	echoaround $n $N下注在５跟８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 6,9~
if is_qmark($n) & roulette_bet_split 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 512
	echoat $n 你下注在６跟９號之間。
	echoaround $n $N下注在６跟９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 9,6~
if is_qmark($n) & roulette_bet_split 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 512
	echoat $n 你下注在６跟９號之間。
	echoaround $n $N下注在６跟９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 7,8~
if is_qmark($n) & roulette_bet_split 101024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 1024
	echoat $n 你下注在７跟８號之間。
	echoaround $n $N下注在７跟８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 8,7~
if is_qmark($n) & roulette_bet_split 101024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 1024
	echoat $n 你下注在７跟８號之間。
	echoaround $n $N下注在７跟８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 7,10~
if is_qmark($n) & roulette_bet_split 102048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 2048
	echoat $n 你下注在７跟１０號之間。
	echoaround $n $N下注在７跟１０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 10,7~
if is_qmark($n) & roulette_bet_split 102048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 2048
	echoat $n 你下注在７跟１０號之間。
	echoaround $n $N下注在７跟１０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 8,9~
if is_qmark($n) & roulette_bet_split 104096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 4096
	echoat $n 你下注在８跟９號之間。
	echoaround $n $N下注在８跟９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 9,8~
if is_qmark($n) & roulette_bet_split 104096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 4096
	echoat $n 你下注在８跟９號之間。
	echoaround $n $N下注在８跟９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 8,11~
if is_qmark($n) & roulette_bet_split 108192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 8192
	echoat $n 你下注在８跟１１號之間。
	echoaround $n $N下注在８跟１１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 11,8~
if is_qmark($n) & roulette_bet_split 108192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 8192
	echoat $n 你下注在８跟１１號之間。
	echoaround $n $N下注在８跟１１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 9,12~
if is_qmark($n) & roulette_bet_split 116384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 16384
	echoat $n 你下注在９跟１２號之間。
	echoaround $n $N下注在９跟１２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 12,9~
if is_qmark($n) & roulette_bet_split 116384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 16384
	echoat $n 你下注在９跟１２號之間。
	echoaround $n $N下注在９跟１２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 10,11~
if is_qmark($n) & roulette_bet_split 132768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 32768
	echoat $n 你下注在１０跟１１號之間。
	echoaround $n $N下注在１０跟１１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 11,10~
if is_qmark($n) & roulette_bet_split 132768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v0 | 32768
	echoat $n 你下注在１０跟１１號之間。
	echoaround $n $N下注在１０跟１１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 10,13~
if is_qmark($n) & roulette_bet_split 200001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 1
	echoat $n 你下注在１０跟１３號之間。
	echoaround $n $N下注在１０跟１３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 13,10~
if is_qmark($n) & roulette_bet_split 200001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 1
	echoat $n 你下注在１０跟１３號之間。
	echoaround $n $N下注在１０跟１３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 11,12~
if is_qmark($n) & roulette_bet_split 200002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 2
	echoat $n 你下注在１１跟１２號之間。
	echoaround $n $N下注在１１跟１２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 12,11~
if is_qmark($n) & roulette_bet_split 200002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 2
	echoat $n 你下注在１１跟１２號之間。
	echoaround $n $N下注在１１跟１２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 11,14~
if is_qmark($n) & roulette_bet_split 200004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 4
	echoat $n 你下注在１１跟１４號之間。
	echoaround $n $N下注在１１跟１４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 14,11~
if is_qmark($n) & roulette_bet_split 200004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 4
	echoat $n 你下注在１１跟１４號之間。
	echoaround $n $N下注在１１跟１４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 12,15~
if is_qmark($n) & roulette_bet_split 200008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 8
	echoat $n 你下注在１２跟１５號之間。
	echoaround $n $N下注在１２跟１５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 15,12~
if is_qmark($n) & roulette_bet_split 200008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 8
	echoat $n 你下注在１２跟１５號之間。
	echoaround $n $N下注在１２跟１５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 13,14~
if is_qmark($n) & roulette_bet_split 200016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 16
	echoat $n 你下注在１３跟１４號之間。
	echoaround $n $N下注在１３跟１４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 14,13~
if is_qmark($n) & roulette_bet_split 200016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 16
	echoat $n 你下注在１３跟１４號之間。
	echoaround $n $N下注在１３跟１４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 13,16~
if is_qmark($n) & roulette_bet_split 200032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 32
	echoat $n 你下注在１３跟１６號之間。
	echoaround $n $N下注在１３跟１６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 16,13~
if is_qmark($n) & roulette_bet_split 200032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 32
	echoat $n 你下注在１３跟１６號之間。
	echoaround $n $N下注在１３跟１６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 14,15~
if is_qmark($n) & roulette_bet_split 200064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 64
	echoat $n 你下注在１４跟１５號之間。
	echoaround $n $N下注在１４跟１５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 15,14~
if is_qmark($n) & roulette_bet_split 200064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 64
	echoat $n 你下注在１４跟１５號之間。
	echoaround $n $N下注在１４跟１５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 14,17~
if is_qmark($n) & roulette_bet_split 200128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 128
	echoat $n 你下注在１４跟１７號之間。
	echoaround $n $N下注在１４跟１７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 17,14~
if is_qmark($n) & roulette_bet_split 200128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 128
	echoat $n 你下注在１４跟１７號之間。
	echoaround $n $N下注在１４跟１７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 15,18~
if is_qmark($n) & roulette_bet_split 200256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 256
	echoat $n 你下注在１５跟１８號之間。
	echoaround $n $N下注在１５跟１８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 18,15~
if is_qmark($n) & roulette_bet_split 200256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 256
	echoat $n 你下注在１５跟１８號之間。
	echoaround $n $N下注在１５跟１８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 16,17~
if is_qmark($n) & roulette_bet_split 200512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 512
	echoat $n 你下注在１６跟１７號之間。
	echoaround $n $N下注在１６跟１７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 17,16~
if is_qmark($n) & roulette_bet_split 200512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 512
	echoat $n 你下注在１６跟１７號之間。
	echoaround $n $N下注在１６跟１７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 16,19~
if is_qmark($n) & roulette_bet_split 201024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 1024
	echoat $n 你下注在１６跟１９號之間。
	echoaround $n $N下注在１６跟１９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 19,16~
if is_qmark($n) & roulette_bet_split 201024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 1024
	echoat $n 你下注在１６跟１９號之間。
	echoaround $n $N下注在１６跟１９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 17,18~
if is_qmark($n) & roulette_bet_split 202048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 2048
	echoat $n 你下注在１８跟１７號之間。
	echoaround $n $N下注在１８跟１７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 18,17~
if is_qmark($n) & roulette_bet_split 202048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 2048
	echoat $n 你下注在１８跟１７號之間。
	echoaround $n $N下注在１８跟１７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 17,20~
if is_qmark($n) & roulette_bet_split 204096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 4096
	echoat $n 你下注在１７跟２０號之間。
	echoaround $n $N下注在１７跟２０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 20,17~
if is_qmark($n) & roulette_bet_split 204096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 4096
	echoat $n 你下注在１７跟２０號之間。
	echoaround $n $N下注在１７跟２０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 18,21~
if is_qmark($n) & roulette_bet_split 208192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 8192
	echoat $n 你下注在１８跟２１號之間。
	echoaround $n $N下注在１８跟２１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 21,18~
if is_qmark($n) & roulette_bet_split 208192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 8192
	echoat $n 你下注在１８跟２１號之間。
	echoaround $n $N下注在１８跟２１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 19,20~
if is_qmark($n) & roulette_bet_split 216384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 16384
	echoat $n 你下注在１９跟２０號之間。
	echoaround $n $N下注在１９跟２０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 20,19~
if is_qmark($n) & roulette_bet_split 216384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 16384
	echoat $n 你下注在１９跟２０號之間。
	echoaround $n $N下注在１９跟２０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 19,22~
if is_qmark($n) & roulette_bet_split 232768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 32768
	echoat $n 你下注在１９跟２２號之間。
	echoaround $n $N下注在１９跟２２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 22,19~
if is_qmark($n) & roulette_bet_split 232768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v1 | 32768
	echoat $n 你下注在１９跟２２號之間。
	echoaround $n $N下注在１９跟２２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 20,21~
if is_qmark($n) & roulette_bet_split 300001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 1
	echoat $n 你下注在２０跟２１號之間。
	echoaround $n $N下注在２０跟２１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 21,20~
if is_qmark($n) & roulette_bet_split 300001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 1
	echoat $n 你下注在２０跟２１號之間。
	echoaround $n $N下注在２０跟２１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 20,23~
if is_qmark($n) & roulette_bet_split 300002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 2
	echoat $n 你下注在２０跟２３號之間。
	echoaround $n $N下注在２０跟２３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 23,20~
if is_qmark($n) & roulette_bet_split 300002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 2
	echoat $n 你下注在２０跟２３號之間。
	echoaround $n $N下注在２０跟２３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 21,24~
if is_qmark($n) & roulette_bet_split 300004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 4
	echoat $n 你下注在２１跟２４號之間。
	echoaround $n $N下注在２１跟２４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 24,21~
if is_qmark($n) & roulette_bet_split 300004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 4
	echoat $n 你下注在２１跟２４號之間。
	echoaround $n $N下注在２１跟２４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 22,23~
if is_qmark($n) & roulette_bet_split 300008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 8
	echoat $n 你下注在２２跟２３號之間。
	echoaround $n $N下注在２２跟２３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 23,22~
if is_qmark($n) & roulette_bet_split 300008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 8
	echoat $n 你下注在２２跟２３號之間。
	echoaround $n $N下注在２２跟２３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 22,25~
if is_qmark($n) & roulette_bet_split 300016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 16
	echoat $n 你下注在２２跟２５號之間。
	echoaround $n $N下注在２２跟２５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 25,22~
if is_qmark($n) & roulette_bet_split 300016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 16
	echoat $n 你下注在２２跟２５號之間。
	echoaround $n $N下注在２２跟２５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 23,24~
if is_qmark($n) & roulette_bet_split 300032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 32
	echoat $n 你下注在２３跟２４號之間。
	echoaround $n $N下注在２３跟２４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 24,23~
if is_qmark($n) & roulette_bet_split 300032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 32
	echoat $n 你下注在２３跟２４號之間。
	echoaround $n $N下注在２３跟２４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 23,26~
if is_qmark($n) & roulette_bet_split 300064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 64
	echoat $n 你下注在２３跟２６號之間。
	echoaround $n $N下注在２３跟２６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 26,23~
if is_qmark($n) & roulette_bet_split 300064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 64
	echoat $n 你下注在２３跟２６號之間。
	echoaround $n $N下注在２３跟２６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 24,27~
if is_qmark($n) & roulette_bet_split 300128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 128
	echoat $n 你下注在２４跟２７號之間。
	echoaround $n $N下注在２４跟２７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 27,24~
if is_qmark($n) & roulette_bet_split 300128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 128
	echoat $n 你下注在２４跟２７號之間。
	echoaround $n $N下注在２４跟２７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 25,26~
if is_qmark($n) & roulette_bet_split 300256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 256
	echoat $n 你下注在２５跟２６號之間。
	echoaround $n $N下注在２５跟２６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 26,25~
if is_qmark($n) & roulette_bet_split 300256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 256
	echoat $n 你下注在２５跟２６號之間。
	echoaround $n $N下注在２５跟２６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 25,28~
if is_qmark($n) & roulette_bet_split 300512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 512
	echoat $n 你下注在２５跟２８號之間。
	echoaround $n $N下注在２５跟２８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 28,25~
if is_qmark($n) & roulette_bet_split 300512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 512
	echoat $n 你下注在２５跟２８號之間。
	echoaround $n $N下注在２５跟２８號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 26,27~
if is_qmark($n) & roulette_bet_split 301024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 1024
	echoat $n 你下注在２６跟２７號之間。
	echoaround $n $N下注在２６跟２７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 27,26~
if is_qmark($n) & roulette_bet_split 301024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 1024
	echoat $n 你下注在２６跟２７號之間。
	echoaround $n $N下注在２６跟２７號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 26,29~
if is_qmark($n) & roulette_bet_split 302048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 2048
	echoat $n 你下注在２６跟２９號之間。
	echoaround $n $N下注在２６跟２９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 29,26~
if is_qmark($n) & roulette_bet_split 302048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 2048
	echoat $n 你下注在２６跟２９號之間。
	echoaround $n $N下注在２６跟２９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 27,30~
if is_qmark($n) & roulette_bet_split 304096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 4096
	echoat $n 你下注在２７跟３０號之間。
	echoaround $n $N下注在２７跟３０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 30,27~
if is_qmark($n) & roulette_bet_split 304096
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 4096
	echoat $n 你下注在２７跟３０號之間。
	echoaround $n $N下注在２７跟３０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 28,29~
if is_qmark($n) & roulette_bet_split 308192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 8192
	echoat $n 你下注在２８跟２９號之間。
	echoaround $n $N下注在２８跟２９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 29,28~
if is_qmark($n) & roulette_bet_split 308192
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 8192
	echoat $n 你下注在２８跟２９號之間。
	echoaround $n $N下注在２８跟２９號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 28,31~
if is_qmark($n) & roulette_bet_split 316384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 16384
	echoat $n 你下注在２８跟３１號之間。
	echoaround $n $N下注在２８跟３１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 31,28~
if is_qmark($n) & roulette_bet_split 316384
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 16384
	echoat $n 你下注在２８跟３１號之間。
	echoaround $n $N下注在２８跟３１號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 29,30~
if is_qmark($n) & roulette_bet_split 332768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 32768
	echoat $n 你下注在２９跟３０號之間。
	echoaround $n $N下注在２９跟３０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 30,29~
if is_qmark($n) & roulette_bet_split 332768
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split v2 | 32768
	echoat $n 你下注在２９跟３０號之間。
	echoaround $n $N下注在２９跟３０號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 29,32~
if is_qmark($n) & roulette_bet_split_2 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 1
	echoat $n 你下注在２９跟３２號之間。
	echoaround $n $N下注在２９跟３２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 32,29~
if is_qmark($n) & roulette_bet_split_2 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 1
	echoat $n 你下注在２９跟３２號之間。
	echoaround $n $N下注在２９跟３２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 30,33~
if is_qmark($n) & roulette_bet_split_2 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 2
	echoat $n 你下注在３０跟３３號之間。
	echoaround $n $N下注在３０跟３３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 33,30~
if is_qmark($n) & roulette_bet_split_2 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 2
	echoat $n 你下注在３０跟３３號之間。
	echoaround $n $N下注在３０跟３３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 31,32~
if is_qmark($n) & roulette_bet_split_2 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 4
	echoat $n 你下注在３１跟３２號之間。
	echoaround $n $N下注在３１跟３２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 32,31~
if is_qmark($n) & roulette_bet_split_2 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 4
	echoat $n 你下注在３１跟３２號之間。
	echoaround $n $N下注在３１跟３２號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 31,34~
if is_qmark($n) & roulette_bet_split_2 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 8
	echoat $n 你下注在３１跟３４號之間。
	echoaround $n $N下注在３１跟３４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 34,31~
if is_qmark($n) & roulette_bet_split_2 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 8
	echoat $n 你下注在３１跟３４號之間。
	echoaround $n $N下注在３１跟３４號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 32,33~
if is_qmark($n) & roulette_bet_split_2 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 16
	echoat $n 你下注在３２跟３３號之間。
	echoaround $n $N下注在３２跟３３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 33,32~
if is_qmark($n) & roulette_bet_split_2 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 16
	echoat $n 你下注在３２跟３３號之間。
	echoaround $n $N下注在３２跟３３號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 32,35~
if is_qmark($n) & roulette_bet_split_2 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 32
	echoat $n 你下注在３２跟３５號之間。
	echoaround $n $N下注在３２跟３５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 35,32~
if is_qmark($n) & roulette_bet_split_2 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 32
	echoat $n 你下注在３２跟３５號之間。
	echoaround $n $N下注在３２跟３５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 33,36~
if is_qmark($n) & roulette_bet_split_2 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 64
	echoat $n 你下注在３３跟３６號之間。
	echoaround $n $N下注在３３跟３６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 36,33~
if is_qmark($n) & roulette_bet_split_2 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 64
	echoat $n 你下注在３３跟３６號之間。
	echoaround $n $N下注在３３跟３６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 34,35~
if is_qmark($n) & roulette_bet_split_2 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 128
	echoat $n 你下注在３４跟３５號之間。
	echoaround $n $N下注在３４跟３５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 35,34~
if is_qmark($n) & roulette_bet_split_2 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 128
	echoat $n 你下注在３４跟３５號之間。
	echoaround $n $N下注在３４跟３５號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 35,36~
if is_qmark($n) & roulette_bet_split_2 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 256
	echoat $n 你下注在３５跟３６號之間。
	echoaround $n $N下注在３５跟３６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p split 36,35~
if is_qmark($n) & roulette_bet_split_2 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_split_2 v0 | 256
	echoat $n 你下注在３５跟３６號之間。
	echoaround $n $N下注在３５跟３６號之間。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 1to3~
if is_qmark($n) & roulette_bet_street 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 1
	echoat $n 你下注在１,２,３號的列上。
	echoaround $n $N下注在１,２,３號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 4to6~
if is_qmark($n) & roulette_bet_street 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 2
	echoat $n 你下注在４,５,６號的列上。
	echoaround $n $N下注在４,５,６號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 7to9~
if is_qmark($n) & roulette_bet_street 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 4
	echoat $n 你下注在７,８,９號的列上。
	echoaround $n $N下注在７,８,９號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 10to12~
if is_qmark($n) & roulette_bet_street 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 8
	echoat $n 你下注在１０,１１,１２號的列上。
	echoaround $n $N下注在１０,１１,１２號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 13to15~
if is_qmark($n) & roulette_bet_street 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 16
	echoat $n 你下注在１３,１４,１５號的列上。
	echoaround $n $N下注在１３,１４,１５號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 16to18~
if is_qmark($n) & roulette_bet_street 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 32
	echoat $n 你下注在１６,１７,１８號的列上。
	echoaround $n $N下注在１６,１７,１８號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 19to21~
if is_qmark($n) & roulette_bet_street 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 64
	echoat $n 你下注在１９,２０,２１號的列上。
	echoaround $n $N下注在１９,２０,２１號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 22to24~
if is_qmark($n) & roulette_bet_street 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 128
	echoat $n 你下注在２２,２３,２４號的列上。
	echoaround $n $N下注在２２,２３,２４號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 25to27~
if is_qmark($n) & roulette_bet_street 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 256
	echoat $n 你下注在２５,２６,２７號的列上。
	echoaround $n $N下注在２５,２６,２７號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 28to30~
if is_qmark($n) & roulette_bet_street 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 512
	echoat $n 你下注在２８,２９,３０號的列上。
	echoaround $n $N下注在２８,２９,３０號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 31to33~
if is_qmark($n) & roulette_bet_street 101024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 1024
	echoat $n 你下注在３１,３２,３３號的列上。
	echoaround $n $N下注在３１,３２,３３號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p street 34to36~
if is_qmark($n) & roulette_bet_street 102048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_street v0 | 2048
	echoat $n 你下注在３４,３５,３６號的列上。
	echoaround $n $N下注在３４,３５,３６號的列上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down1~
if is_qmark($n) & roulette_bet_corner 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 1
	echoat $n 你下注在１,２,４,５的交點上。
	echoaround $n $N下注在１,２,４,５的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down2~
if is_qmark($n) & roulette_bet_corner 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 2
	echoat $n 你下注在４,５,７,８的交點上。
	echoaround $n $N下注在４,５,７,８的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down3~
if is_qmark($n) & roulette_bet_corner 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 4
	echoat $n 你下注在７,８,１０,１１的交點上。
	echoaround $n $N下注在７,８,１０,１１的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down4~
if is_qmark($n) & roulette_bet_corner 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 8
	echoat $n 你下注在１０,１１,１３,１４的交點上。
	echoaround $n $N下注在１０,１１,１３,１４的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down5~
if is_qmark($n) & roulette_bet_corner 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 16
	echoat $n 你下注在１３,１４,１６,１７的交點上。
	echoaround $n $N下注在１３,１４,１６,１７的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down6~
if is_qmark($n) & roulette_bet_corner 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 32
	echoat $n 你下注在１６,１７,１９,２０的交點上。
	echoaround $n $N下注在１６,１７,１９,２０的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down7~
if is_qmark($n) & roulette_bet_corner 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 64
	echoat $n 你下注在１９,２０,２２,２３的交點上。
	echoaround $n $N下注在１９,２０,２２,２３的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down8~
if is_qmark($n) & roulette_bet_corner 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 128
	echoat $n 你下注在２２,２３,２５,２６的交點上。
	echoaround $n $N下注在２２,２３,２５,２６的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down9~
if is_qmark($n) & roulette_bet_corner 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 256
	echoat $n 你下注在２５,２６,２８,２９的交點上。
	echoaround $n $N下注在２５,２６,２８,２９的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down10~
if is_qmark($n) & roulette_bet_corner 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 512
	echoat $n 你下注在２８,２９,３１,３２的交點上。
	echoaround $n $N下注在２８,２９,３１,３２的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner down11~
if is_qmark($n) & roulette_bet_corner 101024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v0 | 1024
	echoat $n 你下注在３１,３２,３４,３５的交點上。
	echoaround $n $N下注在３１,３２,３４,３５的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper1~
if is_qmark($n) & roulette_bet_corner 200001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 1
	echoat $n 你下注在２,３,５,６的交點上。
	echoaround $n $N下注在２,３,５,６的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper2~
if is_qmark($n) & roulette_bet_corner 200002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 2
	echoat $n 你下注在５,６,８,９的交點上。
	echoaround $n $N下注在５,６,８,９的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper3~
if is_qmark($n) & roulette_bet_corner 200004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 4
	echoat $n 你下注在８,９,１１,１２的交點上。
	echoaround $n $N下注在８,９,１１,１２的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper4~
if is_qmark($n) & roulette_bet_corner 200008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 8
	echoat $n 你下注在１１,１２,１４,１５的交點上。
	echoaround $n $N下注在１１,１２,１４,１５的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper5~
if is_qmark($n) & roulette_bet_corner 200016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 16
	echoat $n 你下注在１４,１５,１７,１８的交點上。
	echoaround $n $N下注在１４,１５,１７,１８的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper6~
if is_qmark($n) & roulette_bet_corner 200032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 32
	echoat $n 你下注在１７,１８,２０,２１的交點上。
	echoaround $n $N下注在１７,１８,２０,２１的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper7~
if is_qmark($n) & roulette_bet_corner 200064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 64
	echoat $n 你下注在２０,２１,２３,２４的交點上。
	echoaround $n $N下注在２０,２１,２３,２４的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper8~
if is_qmark($n) & roulette_bet_corner 200128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 128
	echoat $n 你下注在２３,２４,２６,２７的交點上。
	echoaround $n $N下注在２３,２４,２６,２７的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper9~
if is_qmark($n) & roulette_bet_corner 200256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 256
	echoat $n 你下注在２６,２７,２９,３０的交點上。
	echoaround $n $N下注在２６,２７,２９,３０的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper10~
if is_qmark($n) & roulette_bet_corner 200512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 512
	echoat $n 你下注在２９,３０,３２,３３的交點上。
	echoaround $n $N下注在２９,３０,３２,３３的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p corner upper11~
if is_qmark($n) & roulette_bet_corner 201024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_corner v1 | 1024
	echoat $n 你下注在３２,３３,３５,３６的交點上。
	echoaround $n $N下注在３２,３３,３５,３６的交點上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 1to6~
if is_qmark($n) & roulette_bet_line 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 1
	echoat $n 你下注在１至６號的底線上。
	echoaround $n $N下注在１至６號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 4to9~
if is_qmark($n) & roulette_bet_line 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 2
	echoat $n 你下注在４至９號的底線上。
	echoaround $n $N下注在４至９號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 7to12~
if is_qmark($n) & roulette_bet_line 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 4
	echoat $n 你下注在７至１２號的底線上。
	echoaround $n $N下注在７至１２號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 10to15~
if is_qmark($n) & roulette_bet_line 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 8
	echoat $n 你下注在１０至１５號的底線上。
	echoaround $n $N下注在１０至１５號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 13to18~
if is_qmark($n) & roulette_bet_line 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 16
	echoat $n 你下注在１３至１８號的底線上。
	echoaround $n $N下注在１３至１８號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 16to21~
if is_qmark($n) & roulette_bet_line 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 32
	echoat $n 你下注在１６至２１號的底線上。
	echoaround $n $N下注在１６至２１號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 19to24~
if is_qmark($n) & roulette_bet_line 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 64
	echoat $n 你下注在１９至２４號的底線上。
	echoaround $n $N下注在１９至２４號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 22to27~
if is_qmark($n) & roulette_bet_line 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 128
	echoat $n 你下注在２２至２７號的底線上。
	echoaround $n $N下注在２２至２７號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 25to30~
if is_qmark($n) & roulette_bet_line 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 256
	echoat $n 你下注在２５至３０號的底線上。
	echoaround $n $N下注在２５至３０號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 28to33~
if is_qmark($n) & roulette_bet_line 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 512
	echoat $n 你下注在２８至３３號的底線上。
	echoaround $n $N下注在２８至３３號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p line 31to36~
if is_qmark($n) & roulette_bet_line 101024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_line v0 | 1024
	echoat $n 你下注在３１至３６號的底線上。
	echoaround $n $N下注在３１至３６號的底線上。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p column 1~
if is_qmark($n) & roulette_bet_other 100001
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 1
	echoat $n 你下注在第 １ 行的號碼。
	echoaround $n $N下注在第 １ 行的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p column 2~
if is_qmark($n) & roulette_bet_other 100002
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 2
	echoat $n 你下注在第 ２ 行的號碼。
	echoaround $n $N下注在第 ２ 行的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p column 3~
if is_qmark($n) & roulette_bet_other 100004
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 4
	echoat $n 你下注在第 ３ 行的號碼。
	echoaround $n $N下注在第 ３ 行的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p dozen 1~
if is_qmark($n) & roulette_bet_other 100008
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 8
	echoat $n 你下注在第 １ 打的號碼。
	echoaround $n $N下注在第 １ 打的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p dozen 2~
if is_qmark($n) & roulette_bet_other 100016
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 16
	echoat $n 你下注在第 ２ 打的號碼。
	echoaround $n $N下注在第 ２ 打的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p dozen 3~
if is_qmark($n) & roulette_bet_other 100032
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 32
	echoat $n 你下注在第 ３ 打的號碼。
	echoaround $n $N下注在第 ３ 打的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p 1to18~
if is_qmark($n) & roulette_bet_other 100064
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 64
	echoat $n 你下注在１至１８的號碼。
	echoaround $n $N下注在１至１８的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p 19to36~
if is_qmark($n) & roulette_bet_other 100128
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 128
	echoat $n 你下注在１９至３６的號碼。
	echoaround $n $N下注在１９至３６的號碼。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p even~
if is_qmark($n) & roulette_bet_other 100256
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 256
	echoat $n 你下注在偶數。
	echoaround $n $N下注在偶數。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p odd~
if is_qmark($n) & roulette_bet_other 100512
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 512
	echoat $n 你下注在奇數。
	echoaround $n $N下注在奇數。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p red~
if is_qmark($n) & roulette_bet_other 101024
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 1024
	echoat $n 你下注在紅色。
	echoaround $n $N下注在紅色。
	del_qmark $n roulette_success_pay
endif
~
>room_command bet p black~
if is_qmark($n) & roulette_bet_other 102048
	echoat $n 你已經在這下注了。
	break
endif
rcall roulette_check_pay $n
if is_qmark($n) == roulette_success_pay
	mod_qmark $n roulette_bet_other v0 | 2048
	echoat $n 你下注在黑色。
	echoaround $n $N下注在黑色。
	del_qmark $n roulette_success_pay
endif
~
>room_defun roulette_random_1~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 1
			else
				modrvt v0 = 2
			endif
		else
			if rand(50)
				modrvt v0 = 3
			else
				modrvt v0 = 4
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_2~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 5
			else
				modrvt v0 = 6
			endif
		else
			if rand(50)
				modrvt v0 = 7
			else
				modrvt v0 = 8
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_3~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 9
			else
				modrvt v0 = 10
			endif
		else
			if rand(50)
				modrvt v0 = 11
			else
				modrvt v0 = 12
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_4~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 13
			else
				modrvt v0 = 14
			endif
		else
			if rand(50)
				modrvt v0 = 15
			else
				modrvt v0 = 16
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_5~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 17
			else
				modrvt v0 = 18
			endif
		else
			if rand(50)
				modrvt v0 = 19
			else
				modrvt v0 = 20
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_6~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 21
			else
				modrvt v0 = 22
			endif
		else
			if rand(50)
				modrvt v0 = 23
			else
				modrvt v0 = 24
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_7~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 25
			else
				modrvt v0 = 26
			endif
		else
			if rand(50)
				modrvt v0 = 27
			else
				modrvt v0 = 28
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_8~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 29
			else
				modrvt v0 = 30
			endif
		else
			if rand(50)
				modrvt v0 = 31
			else
				modrvt v0 = 32
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 33
						else
							modrvt v0 = 34
						endif
					else
						if rand(50)
							modrvt v0 = 35
						else
							modrvt v0 = 36
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_defun roulette_random_9~
if rand(3)
	modrvt v0 = 0
else
	if rand(11)
		if rand(50)
			if rand(50)
				modrvt v0 = 33
			else
				modrvt v0 = 34
			endif
		else
			if rand(50)
				modrvt v0 = 35
			else
				modrvt v0 = 36
			endif
		endif
	else
		if rand(50)
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 5
						else
							modrvt v0 = 6
						endif
					else
						if rand(50)
							modrvt v0 = 7
						else
							modrvt v0 = 8
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 9
						else
							modrvt v0 = 10
						endif
					else
						if rand(50)
							modrvt v0 = 11
						else
							modrvt v0 = 12
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 13
						else
							modrvt v0 = 14
						endif
					else
						if rand(50)
							modrvt v0 = 15
						else
							modrvt v0 = 16
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 17
						else
							modrvt v0 = 18
						endif
					else
						if rand(50)
							modrvt v0 = 19
						else
							modrvt v0 = 20
						endif
					endif
				endif
			endif
		else
			if rand(50)
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 21
						else
							modrvt v0 = 22
						endif
					else
						if rand(50)
							modrvt v0 = 23
						else
							modrvt v0 = 24
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 25
						else
							modrvt v0 = 26
						endif
					else
						if rand(50)
							modrvt v0 = 27
						else
							modrvt v0 = 28
						endif
					endif
				endif
			else
				if rand(50)
					if rand(50)
						if rand(50)
							modrvt v0 = 29
						else
							modrvt v0 = 30
						endif
					else
						if rand(50)
							modrvt v0 = 31
						else
							modrvt v0 = 32
						endif
					endif
				else
					if rand(50)
						if rand(50)
							modrvt v0 = 1
						else
							modrvt v0 = 2
						endif
					else
						if rand(50)
							modrvt v0 = 3
						else
							modrvt v0 = 4
						endif
					endif
				endif
			endif
		endif
	endif
endif
rcall roulette_final_point $n
~
>room_command check default~
echoat $n 　
echoat $n 　
echoat $n ╭──�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w─�煍w────��
nonechoat $n │    │ [1;31m３ [0m
if is_qmark($n) & roulette_bet_split 100016
	nonechoat $n [1;32m●[0m 
else
	nonechoat $n │ 
endif
nonechoat $n [1;30m６ [0m
if is_qmark($n) & roulette_bet_split 100512
	nonechoat $n [1;32m●[0m 
else
	nonechoat $n │ 
endif
nonechoat $n [1;31m９ [0m
if is_qmark($n) & roulette_bet_split 116384
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m１２[0m
if is_qmark($n) & roulette_bet_split 200008
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m１５[0m
if is_qmark($n) & roulette_bet_split 200256
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m１８[0m
if is_qmark($n) & roulette_bet_split 208192
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m２１[0m
if is_qmark($n) & roulette_bet_split 300004
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m２４[0m
if is_qmark($n) & roulette_bet_split 300128
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m２７[0m
if is_qmark($n) & roulette_bet_split 304096
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m３０[0m
if is_qmark($n) & roulette_bet_split_2 100002
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m３３[0m
if is_qmark($n) & roulette_bet_split_2 100064
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
echoat $n [1;30m３６[0m│ [1;37mColumn 3 [0m│
nonechoat $n │    
if is_qmark($n) & roulette_bet_single 100004
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100032
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100256
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200002
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200016
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200128
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300001
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300008
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300064
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300512
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 304096
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 332768
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_other 100004
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
echoat $n │
nonechoat $n │    ��
if is_qmark($n) & roulette_bet_split 100004
	nonechoat $n [0m[0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200001
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 100128
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200002
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 104096
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200004
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 200002
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200008
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 200064
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200016
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 202048
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200032
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 300001
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200064
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 300032
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200128
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 301024
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200256
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 332768
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 200512
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split_2 100016
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 201024
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split_2 100256
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
echoat $n �屣w────��
rcall check_table_2 $n
~
>room_defun check_table_2~
nonechoat $n │ [1;37m０ [0m│ [1;30m２ [0m
if is_qmark($n) & roulette_bet_split 100008
	nonechoat $n [1;32m●[0m 
else
	nonechoat $n │ 
endif
nonechoat $n [1;31m５ [0m
if is_qmark($n) & roulette_bet_split 100256
	nonechoat $n [1;32m●[0m 
else
	nonechoat $n │ 
endif
nonechoat $n [1;30m８ [0m
if is_qmark($n) & roulette_bet_split 108192
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m１１[0m
if is_qmark($n) & roulette_bet_split 200004
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m１４[0m
if is_qmark($n) & roulette_bet_split 200128
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m１７[0m
if is_qmark($n) & roulette_bet_split 204096
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m２０[0m
if is_qmark($n) & roulette_bet_split 300002
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m２３[0m
if is_qmark($n) & roulette_bet_split 300064
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m２６[0m
if is_qmark($n) & roulette_bet_split 302048
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m２９[0m
if is_qmark($n) & roulette_bet_split_2 100001
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m３２[0m
if is_qmark($n) & roulette_bet_split_2 100032
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
echoat $n [1;31m３５[0m│ [1;37mColumn 2 [0m│
if is_qmark($n) == roulette_bet_single_0
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100002
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100016
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100128
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200001
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200008
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200064
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200512
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300004
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300032
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300256
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 302048
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 316384
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_other 100002
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
echoat $n │
nonechoat $n │    ��
if is_qmark($n) & roulette_bet_split 100001
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100001
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 100032
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100002
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 101024
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100004
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 132768
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100008
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 200016
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100016
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 200512
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100032
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 216384
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100064
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 300008
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100128
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 300256
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100256
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split 308192
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 100512
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split_2 100004
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_corner 101024
	nonechoat $n [1;32m●[0m
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_split_2 100128
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
echoat $n �屣w────��
nonechoat $n │    │ [1;31m１ [0m
if is_qmark($n) & roulette_bet_split 100002
	nonechoat $n [1;32m●[0m 
else
	nonechoat $n │ 
endif
nonechoat $n [1;30m４ [0m
if is_qmark($n) & roulette_bet_split 100064
	nonechoat $n [1;32m●[0m 
else
	nonechoat $n │ 
endif
nonechoat $n [1;31m７ [0m
if is_qmark($n) & roulette_bet_split 102048
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m１０[0m
if is_qmark($n) & roulette_bet_split 200001
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m１３[0m
if is_qmark($n) & roulette_bet_split 200032
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m１６[0m
if is_qmark($n) & roulette_bet_split 201024
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m１９[0m
if is_qmark($n) & roulette_bet_split 232768
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m２２[0m
if is_qmark($n) & roulette_bet_split 300016
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m２５[0m
if is_qmark($n) & roulette_bet_split 300512
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;30m２８[0m
if is_qmark($n) & roulette_bet_split 316384
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
nonechoat $n [1;31m３１[0m
if is_qmark($n) & roulette_bet_split_2 100008
	nonechoat $n [1;32m●[0m
else
	nonechoat $n │
endif
echoat $n [1;30m３４[0m│ [1;37mColumn 1 [0m│
rcall check_table_3 $n
~
>room_defun check_table_3~
nonechoat $n │    
if is_qmark($n) & roulette_bet_single 100001
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100008
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100064
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 100512
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200004
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200032
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 200256
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300002
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300016
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 300128
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 301024
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_single 308192
	nonechoat $n │ [1;32m● [0m
else
	nonechoat $n │    
endif
if is_qmark($n) & roulette_bet_other 100001
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
echoat $n │
nonechoat $n ╰──��
if is_qmark($n) & roulette_bet_street 100001
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100001
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100002
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100002
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100004
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100004
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100008
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100008
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100016
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100016
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100032
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100032
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100064
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100064
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100128
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100128
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100256
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100256
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 100512
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 100512
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 101024
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
if is_qmark($n) & roulette_bet_line 101024
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ��
endif
if is_qmark($n) & roulette_bet_street 102048
	nonechoat $n [0m-[1;32m●[0m-
else
	nonechoat $n ──
endif
echoat $n �屣w────��
echoat $n 　　　│                      │                      │                      │
echoat $n 　　　│      [1;37m１st Dozen      [0m│      [1;37m２rd Dozen      [0m│      [1;37m３th Dozen      [0m│
nonechoat $n 　　　
if is_qmark($n) & roulette_bet_other 100008
	nonechoat $n │          [1;32m●          [0m
else
	nonechoat $n │                      
endif
if is_qmark($n) & roulette_bet_other 100016
	nonechoat $n │          [1;32m●          [0m
else
	nonechoat $n │                      
endif
if is_qmark($n) & roulette_bet_other 100032
	nonechoat $n │          [1;32m●          [0m
else
	nonechoat $n │                      
endif
echoat $n │
echoat $n 　　　�遉w────�煍w────�屣w────�煍w────�屣w────�煍w────��            
echoat $n 　　　│ [1;37m１∼１８ [0m│   [1;37mEVEN   [0m│   [1;31mRED    [0m│  [1;30mBLACK   [0m│   [1;37mODD    [0m│[1;37m１９∼３６[0m│
nonechoat $n 　　　
if is_qmark($n) & roulette_bet_other 100064
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
if is_qmark($n) & roulette_bet_other 100256
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
if is_qmark($n) & roulette_bet_other 101024
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
if is_qmark($n) & roulette_bet_other 102048
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
if is_qmark($n) & roulette_bet_other 100512
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
if is_qmark($n) & roulette_bet_other 100128
	nonechoat $n │    [1;32m●    [0m
else
	nonechoat $n │          
endif
echoat $n │
echoat $n 　　　�聝w────�銚w────�銚w────�銚w────�銚w────�銚w────��    
~
|
