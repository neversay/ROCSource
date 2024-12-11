>room_command bet default~
echoat $n 你要下哪邊？下多少？(bet '位置 數量')
~
>room_command bet p banker 1~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 1
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 １ 籌碼賭莊家贏。
	echoaround $n $N下注 １ 籌碼賭莊家贏。
	mod_qmark $n baccarat_banker_win v0 + 1
	mod_qmark $n baccarat_banker_win v1 + 1
	mod_qmark $n baccarat_bet v0 + 1
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker 2~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 2
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ２ 籌碼賭莊家贏。
	echoaround $n $N下注 ２ 籌碼賭莊家贏。
	mod_qmark $n baccarat_banker_win v0 + 2
	mod_qmark $n baccarat_banker_win v1 + 2
	mod_qmark $n baccarat_bet v0 + 2
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker 3~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 3
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ３ 籌碼賭莊家贏。
	echoaround $n $N下注 ３ 籌碼賭莊家贏。
	mod_qmark $n baccarat_banker_win v0 + 3
	mod_qmark $n baccarat_banker_win v1 + 3
	mod_qmark $n baccarat_bet v0 + 3
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker 4~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 4
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ４ 籌碼賭莊家贏。
	echoaround $n $N下注 ４ 籌碼賭莊家贏。
	mod_qmark $n baccarat_banker_win v0 + 4
	mod_qmark $n baccarat_banker_win v1 + 4
	mod_qmark $n baccarat_bet v0 + 4
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker 5~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 5
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ５ 籌碼賭莊家贏。
	echoaround $n $N下注 ５ 籌碼賭莊家贏。
	mod_qmark $n baccarat_banker_win v0 + 5
	mod_qmark $n baccarat_banker_win v1 + 5
	mod_qmark $n baccarat_bet v0 + 5
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player 1~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 1
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 １ 籌碼賭閒家贏。
	echoaround $n $N下注 １ 籌碼賭閒家贏。
	mod_qmark $n baccarat_player_win v0 + 1
	mod_qmark $n baccarat_bet v0 + 1
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player 2~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 2
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ２ 籌碼賭閒家贏。
	echoaround $n $N下注 ２ 籌碼賭閒家贏。
	mod_qmark $n baccarat_player_win v0 + 2
	mod_qmark $n baccarat_bet v0 + 2
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player 3~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 3
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ３ 籌碼賭閒家贏。
	echoaround $n $N下注 ３ 籌碼賭閒家贏。
	mod_qmark $n baccarat_player_win v0 + 3
	mod_qmark $n baccarat_bet v0 + 3
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player 4~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 4
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ４ 籌碼賭閒家贏。
	echoaround $n $N下注 ４ 籌碼賭閒家贏。
	mod_qmark $n baccarat_player_win v0 + 4
	mod_qmark $n baccarat_bet v0 + 4
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player 5~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 5
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ５ 籌碼賭閒家贏。
	echoaround $n $N下注 ５ 籌碼賭閒家贏。
	mod_qmark $n baccarat_player_win v0 + 5
	mod_qmark $n baccarat_bet v0 + 5
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker-pair 1~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 1
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 １ 籌碼賭莊家手牌拿到對子。
	echoaround $n $N下注 １ 籌碼賭莊家手牌拿到對子。
	mod_qmark $n baccarat_banker_pair v0 + 11
	mod_qmark $n baccarat_banker_pair v0 + 1
	mod_qmark $n baccarat_bet v0 + 1
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker-pair 2~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 2
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ２ 籌碼賭莊家手牌拿到對子。
	echoaround $n $N下注 ２ 籌碼賭莊家手牌拿到對子。
	mod_qmark $n baccarat_banker_pair v0 + 22
	mod_qmark $n baccarat_banker_pair v0 + 2
	mod_qmark $n baccarat_bet v0 + 2
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker-pair 3~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 3
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ３ 籌碼賭莊家手牌拿到對子。
	echoaround $n $N下注 ３ 籌碼賭莊家手牌拿到對子。
	mod_qmark $n baccarat_banker_pair v0 + 33
	mod_qmark $n baccarat_banker_pair v0 + 3
	mod_qmark $n baccarat_bet v0 + 3
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker-pair 4~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 4
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ４ 籌碼賭莊家手牌拿到對子。
	echoaround $n $N下注 ４ 籌碼賭莊家手牌拿到對子。
	mod_qmark $n baccarat_banker_pair v0 + 44
	mod_qmark $n baccarat_banker_pair v0 + 4
	mod_qmark $n baccarat_bet v0 + 4
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p banker-pair 5~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 5
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ５ 籌碼賭莊家手牌拿到對子。
	echoaround $n $N下注 ５ 籌碼賭莊家手牌拿到對子。
	mod_qmark $n baccarat_banker_pair v0 + 55
	mod_qmark $n baccarat_banker_pair v0 + 5
	mod_qmark $n baccarat_bet v0 + 5
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player-pair 1~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 1
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 １ 籌碼賭閒家手牌拿到對子。
	echoaround $n $N下注 １ 籌碼賭閒家手牌拿到對子。
	mod_qmark $n baccarat_player_pair v0 + 11
	mod_qmark $n baccarat_player_pair v1 + 1
	mod_qmark $n baccarat_bet v0 + 1
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player-pair 2~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 2
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ２ 籌碼賭閒家手牌拿到對子。
	echoaround $n $N下注 ２ 籌碼賭閒家手牌拿到對子。
	mod_qmark $n baccarat_player_pair v0 + 22
	mod_qmark $n baccarat_player_pair v1 + 2
	mod_qmark $n baccarat_bet v0 + 2
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player-pair 3~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 3
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ３ 籌碼賭閒家手牌拿到對子。
	echoaround $n $N下注 ３ 籌碼賭閒家手牌拿到對子。
	mod_qmark $n baccarat_player_pair v0 + 33
	mod_qmark $n baccarat_player_pair v1 + 3
	mod_qmark $n baccarat_bet v0 + 3
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player-pair 4~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 4
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ４ 籌碼賭閒家手牌拿到對子。
	echoaround $n $N下注 ４ 籌碼賭閒家手牌拿到對子。
	mod_qmark $n baccarat_player_pair v0 + 44
	mod_qmark $n baccarat_player_pair v1 + 4
	mod_qmark $n baccarat_bet v0 + 4
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p player-pair 5~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 5
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ５ 籌碼賭閒家手牌拿到對子。
	echoaround $n $N下注 ５ 籌碼賭閒家手牌拿到對子。
	mod_qmark $n baccarat_player_pair v0 + 55
	mod_qmark $n baccarat_player_pair v1 + 5
	mod_qmark $n baccarat_bet v0 + 5
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p tie 1~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 1
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 １ 籌碼賭這場遊戲和局。
	echoaround $n $N下注 １ 籌碼賭這場遊戲和局。
	mod_qmark $n baccarat_tie_game v0 + 8
	mod_qmark $n baccarat_tie_game v1 + 1
	mod_qmark $n baccarat_bet v0 + 1
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p tie 2~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 2
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ２ 籌碼賭這場遊戲和局。
	echoaround $n $N下注 ２ 籌碼賭這場遊戲和局。
	mod_qmark $n baccarat_tie_game v0 + 16
	mod_qmark $n baccarat_tie_game v1 + 2
	mod_qmark $n baccarat_bet v0 + 2
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p tie 3~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 3
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ３ 籌碼賭這場遊戲和局。
	echoaround $n $N下注 ３ 籌碼賭這場遊戲和局。
	mod_qmark $n baccarat_tie_game v0 + 24
	mod_qmark $n baccarat_tie_game v1 + 3
	mod_qmark $n baccarat_bet v0 + 3
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p tie 4~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 4
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ４ 籌碼賭這場遊戲和局。
	echoaround $n $N下注 ４ 籌碼賭這場遊戲和局。
	mod_qmark $n baccarat_tie_game v0 + 32
	mod_qmark $n baccarat_tie_game v1 + 4
	mod_qmark $n baccarat_bet v0 + 4
	del_qmark $n baccarat_bet_buf
endif
~
>room_command bet p tie 5~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 5
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n 你下注 ５ 籌碼賭這場遊戲和局。
	echoaround $n $N下注 ５ 籌碼賭這場遊戲和局。
	mod_qmark $n baccarat_tie_game v0 + 40
	mod_qmark $n baccarat_tie_game v1 + 5
	mod_qmark $n baccarat_bet v0 + 5
	del_qmark $n baccarat_bet_buf
endif
~
>room_command check default~
if is_qmark($n) == baccarat_bet
else
	echoat $n 什麼？
	break
endif
force $b mpcall baccarat_show_bet $n
~
>room_enter 64 100~
if is_qmark($n) == baccarat_bet
else
	break
endif
if is_qmark($n) == baccarat_dealer
	break
endif
if roomvalue($1) == 7
	if roomvalue($0) & 1
		if is_qmark($n) > baccarat_banker_win 100000
			echoat $n 荷官抽完 5% 佣金後，把下注莊家及贏得的籌碼給了你。
			rcall baccarat_banker_winner $n
		endif
	endif
	if roomvalue($0) & 2
		if is_qmark($n) > baccarat_player_win 100000
			echoat $n 荷官把下注閒家及贏得的籌碼給了你。
			rcall baccarat_player_winner $n
		endif
	endif
	if roomvalue($0) & 4
		if is_qmark($n) > baccarat_tie_game 100000
			rcall baccarat_tie_winner $n
		endif
		if is_qmark($n) > baccarat_banker_win 100000
			echoat $n 荷官把你的押在莊家的籌碼退還。
			rcall baccarat_tie_banker $n
		endif
		if is_qmark($n) > baccarat_player_win 100000
			echoat $n 荷官把你的押在閒家的籌碼退還。
			rcall baccarat_tie_player $n
		endif
	endif
	if roomvalue($0) & 8
		if is_qmark($n) > baccarat_player_pair 100000
			echoat $n 把下注閒家對子及贏得的籌碼給了你。
			rcall baccarat_ppair_winner $n
		endif
	endif
	if roomvalue($0) & 16
		if is_qmark($n) > baccarat_banker_pair 100000
			echoat $n 把下注莊家對子及贏得的籌碼給了你。
			rcall baccarat_bpair_winner $n
		endif
	endif
endif
del_qmark $n baccarat_bet
del_qmark $n baccarat_banker_win
del_qmark $n baccarat_player_win
del_qmark $n baccarat_banker_pair
del_qmark $n baccarat_player_pair
del_qmark $n baccarat_tie_game
~
>room_leave 511 100~
if is_qmark($n) == baccarat_dealer
	break
endif
if roomvalue($1) == 7
	break
endif
if is_qmark($n) == baccarat_bet
	echoat $n 你放棄了遊戲，下注的籌碼一去不復反。
	del_qmark $n baccarat_bet
	del_qmark $n baccarat_banker_win
	del_qmark $n baccarat_player_win
	del_qmark $n baccarat_banker_pair
	del_qmark $n baccarat_player_pair
	del_qmark $n baccarat_tie_game
endif
~
|
