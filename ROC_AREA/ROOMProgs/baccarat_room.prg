>room_command bet default~
echoat $n �A�n�U����H�U�h�֡H(bet '��m �ƶq')
~
>room_command bet p banker 1~
add_qmark $n baccarat_bet_buf 2 0 0 0 1 0
mod_qmark $n baccarat_bet_buf v0 = 1
rcall baccarat_bet_check $n
if is_qmark($n) == baccarat_bet_buf
	echoat $n �A�U�` �� �w�X����aĹ�C
	echoaround $n $N�U�` �� �w�X����aĹ�C
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
	echoat $n �A�U�` �� �w�X����aĹ�C
	echoaround $n $N�U�` �� �w�X����aĹ�C
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
	echoat $n �A�U�` �� �w�X����aĹ�C
	echoaround $n $N�U�` �� �w�X����aĹ�C
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
	echoat $n �A�U�` �� �w�X����aĹ�C
	echoaround $n $N�U�` �� �w�X����aĹ�C
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
	echoat $n �A�U�` �� �w�X����aĹ�C
	echoaround $n $N�U�` �� �w�X����aĹ�C
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
	echoat $n �A�U�` �� �w�X�䶢�aĹ�C
	echoaround $n $N�U�` �� �w�X�䶢�aĹ�C
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
	echoat $n �A�U�` �� �w�X�䶢�aĹ�C
	echoaround $n $N�U�` �� �w�X�䶢�aĹ�C
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
	echoat $n �A�U�` �� �w�X�䶢�aĹ�C
	echoaround $n $N�U�` �� �w�X�䶢�aĹ�C
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
	echoat $n �A�U�` �� �w�X�䶢�aĹ�C
	echoaround $n $N�U�` �� �w�X�䶢�aĹ�C
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
	echoat $n �A�U�` �� �w�X�䶢�aĹ�C
	echoaround $n $N�U�` �� �w�X�䶢�aĹ�C
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
	echoat $n �A�U�` �� �w�X����a��P�����l�C
	echoaround $n $N�U�` �� �w�X����a��P�����l�C
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
	echoat $n �A�U�` �� �w�X����a��P�����l�C
	echoaround $n $N�U�` �� �w�X����a��P�����l�C
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
	echoat $n �A�U�` �� �w�X����a��P�����l�C
	echoaround $n $N�U�` �� �w�X����a��P�����l�C
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
	echoat $n �A�U�` �� �w�X����a��P�����l�C
	echoaround $n $N�U�` �� �w�X����a��P�����l�C
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
	echoat $n �A�U�` �� �w�X����a��P�����l�C
	echoaround $n $N�U�` �� �w�X����a��P�����l�C
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
	echoat $n �A�U�` �� �w�X�䶢�a��P�����l�C
	echoaround $n $N�U�` �� �w�X�䶢�a��P�����l�C
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
	echoat $n �A�U�` �� �w�X�䶢�a��P�����l�C
	echoaround $n $N�U�` �� �w�X�䶢�a��P�����l�C
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
	echoat $n �A�U�` �� �w�X�䶢�a��P�����l�C
	echoaround $n $N�U�` �� �w�X�䶢�a��P�����l�C
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
	echoat $n �A�U�` �� �w�X�䶢�a��P�����l�C
	echoaround $n $N�U�` �� �w�X�䶢�a��P�����l�C
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
	echoat $n �A�U�` �� �w�X�䶢�a��P�����l�C
	echoaround $n $N�U�` �� �w�X�䶢�a��P�����l�C
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
	echoat $n �A�U�` �� �w�X��o���C���M���C
	echoaround $n $N�U�` �� �w�X��o���C���M���C
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
	echoat $n �A�U�` �� �w�X��o���C���M���C
	echoaround $n $N�U�` �� �w�X��o���C���M���C
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
	echoat $n �A�U�` �� �w�X��o���C���M���C
	echoaround $n $N�U�` �� �w�X��o���C���M���C
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
	echoat $n �A�U�` �� �w�X��o���C���M���C
	echoaround $n $N�U�` �� �w�X��o���C���M���C
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
	echoat $n �A�U�` �� �w�X��o���C���M���C
	echoaround $n $N�U�` �� �w�X��o���C���M���C
	mod_qmark $n baccarat_tie_game v0 + 40
	mod_qmark $n baccarat_tie_game v1 + 5
	mod_qmark $n baccarat_bet v0 + 5
	del_qmark $n baccarat_bet_buf
endif
~
>room_command check default~
if is_qmark($n) == baccarat_bet
else
	echoat $n ����H
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
			echoat $n ���x�⧹ 5% ������A��U�`���a��Ĺ�o���w�X���F�A�C
			rcall baccarat_banker_winner $n
		endif
	endif
	if roomvalue($0) & 2
		if is_qmark($n) > baccarat_player_win 100000
			echoat $n ���x��U�`���a��Ĺ�o���w�X���F�A�C
			rcall baccarat_player_winner $n
		endif
	endif
	if roomvalue($0) & 4
		if is_qmark($n) > baccarat_tie_game 100000
			rcall baccarat_tie_winner $n
		endif
		if is_qmark($n) > baccarat_banker_win 100000
			echoat $n ���x��A����b���a���w�X�h�١C
			rcall baccarat_tie_banker $n
		endif
		if is_qmark($n) > baccarat_player_win 100000
			echoat $n ���x��A����b���a���w�X�h�١C
			rcall baccarat_tie_player $n
		endif
	endif
	if roomvalue($0) & 8
		if is_qmark($n) > baccarat_player_pair 100000
			echoat $n ��U�`���a��l��Ĺ�o���w�X���F�A�C
			rcall baccarat_ppair_winner $n
		endif
	endif
	if roomvalue($0) & 16
		if is_qmark($n) > baccarat_banker_pair 100000
			echoat $n ��U�`���a��l��Ĺ�o���w�X���F�A�C
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
	echoat $n �A���F�C���A�U�`���w�X�@�h���_�ϡC
	del_qmark $n baccarat_bet
	del_qmark $n baccarat_banker_win
	del_qmark $n baccarat_player_win
	del_qmark $n baccarat_banker_pair
	del_qmark $n baccarat_player_pair
	del_qmark $n baccarat_tie_game
endif
~
|
