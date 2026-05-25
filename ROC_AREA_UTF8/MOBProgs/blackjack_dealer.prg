>repop_prog 100~
mpadd_qmark self poker_blackjack_dealer -1 0 0 0 0 1
mpadd_qmark self poker_random_timer -1 0 0 0 0 1
mpadd_qmark self poker_21_point -1 0 0 0 1 0
mpadd_qmark self poker_21_first -1 0 0 0 1 0
mpadd_qmark self poker_21_second -1 0 0 0 1 0
mpadd_qmark self poker_21_thrid -1 0 0 0 1 0
mpadd_qmark self poker_21_fourth -1 0 0 0 1 0
mpadd_qmark self poker_21_fifth -1 0 0 0 1 0
mpforcer here def_rembch $i
~
>defun_prog poker_21_check~
if is_qmark($i) == poker_21_second 100000
	mpnonechoat $n $I手上有一張暗牌及一張
else
	mpnonechoat $n $I手上的牌有：
endif
if is_qmark($i) > poker_21_first 100000
	if is_qmark($i) == poker_21_first 200001
		mpnonechoat $n [1;31m紅心[0m
	endif
	if is_qmark($i) == poker_21_first 200002
		mpnonechoat $n [1;31m方塊[0m
	endif
	if is_qmark($i) == poker_21_first 200003
		mpnonechoat $n [1;30m黑桃[0m
	endif
	if is_qmark($i) == poker_21_first 200004
		mpnonechoat $n [1;30m梅花[0m
	endif
	if is_qmark($i) & poker_21_first 100001
		mpnonechoat $n [1;37mＡ[0m
	endif
	if is_qmark($i) & poker_21_first 100002
		mpnonechoat $n [1;37m２[0m
	endif
	if is_qmark($i) & poker_21_first 100004
		mpnonechoat $n [1;37m３[0m
	endif
	if is_qmark($i) & poker_21_first 100008
		mpnonechoat $n [1;37m４[0m
	endif
	if is_qmark($i) & poker_21_first 100016
		mpnonechoat $n [1;37m５[0m
	endif
	if is_qmark($i) & poker_21_first 100032
		mpnonechoat $n [1;37m６[0m
	endif
	if is_qmark($i) & poker_21_first 100064
		mpnonechoat $n [1;37m７[0m
	endif
	if is_qmark($i) & poker_21_first 100128
		mpnonechoat $n [1;37m８[0m
	endif
	if is_qmark($i) & poker_21_first 100256
		mpnonechoat $n [1;37m９[0m
	endif
	if is_qmark($i) & poker_21_first 100512
		mpnonechoat $n [1;37m１０[0m
	endif
	if is_qmark($i) & poker_21_first 101024
		mpnonechoat $n [1;37mＪ[0m
	endif
	if is_qmark($i) & poker_21_first 102048
		mpnonechoat $n [1;37mＱ[0m
	endif
	if is_qmark($i) & poker_21_first 104096
		mpnonechoat $n [1;37mＫ[0m
	endif
endif
if is_qmark($i) > poker_21_second 100000
	mpnonechoat $n  、 
	if is_qmark($i) == poker_21_second 200001
		mpnonechoat $n [1;31m紅心[0m
	endif
	if is_qmark($i) == poker_21_second 200002
		mpnonechoat $n [1;31m方塊[0m
	endif
	if is_qmark($i) == poker_21_second 200003
		mpnonechoat $n [1;30m黑桃[0m
	endif
	if is_qmark($i) == poker_21_second 200004
		mpnonechoat $n [1;30m梅花[0m
	endif
	if is_qmark($i) & poker_21_second 100001
		mpnonechoat $n [1;37mＡ[0m
	endif
	if is_qmark($i) & poker_21_second 100002
		mpnonechoat $n [1;37m２[0m
	endif
	if is_qmark($i) & poker_21_second 100004
		mpnonechoat $n [1;37m３[0m
	endif
	if is_qmark($i) & poker_21_second 100008
		mpnonechoat $n [1;37m４[0m
	endif
	if is_qmark($i) & poker_21_second 100016
		mpnonechoat $n [1;37m５[0m
	endif
	if is_qmark($i) & poker_21_second 100032
		mpnonechoat $n [1;37m６[0m
	endif
	if is_qmark($i) & poker_21_second 100064
		mpnonechoat $n [1;37m７[0m
	endif
	if is_qmark($i) & poker_21_second 100128
		mpnonechoat $n [1;37m８[0m
	endif
	if is_qmark($i) & poker_21_second 100256
		mpnonechoat $n [1;37m９[0m
	endif
	if is_qmark($i) & poker_21_second 100512
		mpnonechoat $n [1;37m１０[0m
	endif
	if is_qmark($i) & poker_21_second 101024
		mpnonechoat $n [1;37mＪ[0m
	endif
	if is_qmark($i) & poker_21_second 102048
		mpnonechoat $n [1;37mＱ[0m
	endif
	if is_qmark($i) & poker_21_second 104096
		mpnonechoat $n [1;37mＫ[0m
	endif
endif
if is_qmark($i) > poker_21_thrid 100000
	mpnonechoat $n  、 
	if is_qmark($i) == poker_21_thrid 200001
		mpnonechoat $n [1;31m紅心[0m
	endif
	if is_qmark($i) == poker_21_thrid 200002
		mpnonechoat $n [1;31m方塊[0m
	endif
	if is_qmark($i) == poker_21_thrid 200003
		mpnonechoat $n [1;30m黑桃[0m
	endif
	if is_qmark($i) == poker_21_thrid 200004
		mpnonechoat $n [1;30m梅花[0m
	endif
	if is_qmark($i) & poker_21_thrid 100001
		mpnonechoat $n [1;37mＡ[0m
	endif
	if is_qmark($i) & poker_21_thrid 100002
		mpnonechoat $n [1;37m２[0m
	endif
	if is_qmark($i) & poker_21_thrid 100004
		mpnonechoat $n [1;37m３[0m
	endif
	if is_qmark($i) & poker_21_thrid 100008
		mpnonechoat $n [1;37m４[0m
	endif
	if is_qmark($i) & poker_21_thrid 100016
		mpnonechoat $n [1;37m５[0m
	endif
	if is_qmark($i) & poker_21_thrid 100032
		mpnonechoat $n [1;37m６[0m
	endif
	if is_qmark($i) & poker_21_thrid 100064
		mpnonechoat $n [1;37m７[0m
	endif
	if is_qmark($i) & poker_21_thrid 100128
		mpnonechoat $n [1;37m８[0m
	endif
	if is_qmark($i) & poker_21_thrid 100256
		mpnonechoat $n [1;37m９[0m
	endif
	if is_qmark($i) & poker_21_thrid 100512
		mpnonechoat $n [1;37m１０[0m
	endif
	if is_qmark($i) & poker_21_thrid 101024
		mpnonechoat $n [1;37mＪ[0m
	endif
	if is_qmark($i) & poker_21_thrid 102048
		mpnonechoat $n [1;37mＱ[0m
	endif
	if is_qmark($i) & poker_21_thrid 104096
		mpnonechoat $n [1;37mＫ[0m
	endif
endif
if is_qmark($i) > poker_21_fourth 100000
	mpnonechoat $n  、 
	if is_qmark($i) == poker_21_fourth 200001
		mpnonechoat $n [1;31m紅心[0m
	endif
	if is_qmark($i) == poker_21_fourth 200002
		mpnonechoat $n [1;31m方塊[0m
	endif
	if is_qmark($i) == poker_21_fourth 200003
		mpnonechoat $n [1;30m黑桃[0m
	endif
	if is_qmark($i) == poker_21_fourth 200004
		mpnonechoat $n [1;30m梅花[0m
	endif
	if is_qmark($i) & poker_21_fourth 100001
		mpnonechoat $n [1;37mＡ[0m
	endif
	if is_qmark($i) & poker_21_fourth 100002
		mpnonechoat $n [1;37m２[0m
	endif
	if is_qmark($i) & poker_21_fourth 100004
		mpnonechoat $n [1;37m３[0m
	endif
	if is_qmark($i) & poker_21_fourth 100008
		mpnonechoat $n [1;37m４[0m
	endif
	if is_qmark($i) & poker_21_fourth 100016
		mpnonechoat $n [1;37m５[0m
	endif
	if is_qmark($i) & poker_21_fourth 100032
		mpnonechoat $n [1;37m６[0m
	endif
	if is_qmark($i) & poker_21_fourth 100064
		mpnonechoat $n [1;37m７[0m
	endif
	if is_qmark($i) & poker_21_fourth 100128
		mpnonechoat $n [1;37m８[0m
	endif
	if is_qmark($i) & poker_21_fourth 100256
		mpnonechoat $n [1;37m９[0m
	endif
	if is_qmark($i) & poker_21_fourth 100512
		mpnonechoat $n [1;37m１０[0m
	endif
	if is_qmark($i) & poker_21_fourth 101024
		mpnonechoat $n [1;37mＪ[0m
	endif
	if is_qmark($i) & poker_21_fourth 102048
		mpnonechoat $n [1;37mＱ[0m
	endif
	if is_qmark($i) & poker_21_fourth 104096
		mpnonechoat $n [1;37mＫ[0m
	endif
endif
if is_qmark($i) > poker_21_fifth 100000
	mpnonechoat $n  、 
	if is_qmark($i) == poker_21_fifth 200001
		mpnonechoat $n [1;31m紅心[0m
	endif
	if is_qmark($i) == poker_21_fifth 200002
		mpnonechoat $n [1;31m方塊[0m
	endif
	if is_qmark($i) == poker_21_fifth 200003
		mpnonechoat $n [1;30m黑桃[0m
	endif
	if is_qmark($i) == poker_21_fifth 200004
		mpnonechoat $n [1;30m梅花[0m
	endif
	if is_qmark($i) & poker_21_fifth 100001
		mpnonechoat $n [1;37mＡ[0m
	endif
	if is_qmark($i) & poker_21_fifth 100002
		mpnonechoat $n [1;37m２[0m
	endif
	if is_qmark($i) & poker_21_fifth 100004
		mpnonechoat $n [1;37m３[0m
	endif
	if is_qmark($i) & poker_21_fifth 100008
		mpnonechoat $n [1;37m４[0m
	endif
	if is_qmark($i) & poker_21_fifth 100016
		mpnonechoat $n [1;37m５[0m
	endif
	if is_qmark($i) & poker_21_fifth 100032
		mpnonechoat $n [1;37m６[0m
	endif
	if is_qmark($i) & poker_21_fifth 100064
		mpnonechoat $n [1;37m７[0m
	endif
	if is_qmark($i) & poker_21_fifth 100128
		mpnonechoat $n [1;37m８[0m
	endif
	if is_qmark($i) & poker_21_fifth 100256
		mpnonechoat $n [1;37m９[0m
	endif
	if is_qmark($i) & poker_21_fifth 100512
		mpnonechoat $n [1;37m１０[0m
	endif
	if is_qmark($i) & poker_21_fifth 101024
		mpnonechoat $n [1;37mＪ[0m
	endif
	if is_qmark($i) & poker_21_fifth 102048
		mpnonechoat $n [1;37mＱ[0m
	endif
	if is_qmark($i) & poker_21_fifth 104096
		mpnonechoat $n [1;37mＫ[0m
	endif
endif
mpechoat $n .
~
>rand_prog 100~
if is_qmark($i) == poker_blackjack_dealer 100001
	if is_qmark($i) == poker_random_timer 100002
		mpforcer here rcall poker_21_timeline $i
		mpmod_qmark self poker_random_timer v0 = 0
	else
		if rand(80)
			if rand(90)
				mpforcer here rcall poker_21_timeline $i
			else
				mpmod_qmark self poker_random_timer v0 + 1
			endif
		else
			if rand(50)
				mpforcer here rcall poker_21_timeline $i
			else
				mpmod_qmark self poker_random_timer v0 + 1
			endif
		endif
	endif
endif
~
>talk_prog topic~
tell $n 想了解本賭場二十一點的玩法嗎？(blackjack-rule)
~
>talk_prog p blackjack-rule~
tell $n 本賭場二十一點的規則很簡單，只要手牌持有點數不超過 21 點且大於莊家就算勝利。
tell $n 只要有足夠的籌碼下注就能加入遊戲，方法是輸入 bet '數字 chip'，目前數字為 1 到 10，請不要輸入中文。
tell $n 加入遊戲後莊家會發給閒家兩張公開的牌，並且將有一段最後下注時間，時間內所有人都能下注。
tell $n 結束最後下注後，莊家會發給自己兩張牌，第一張是蓋上的暗牌另張則是公開的明牌，然後將給予閒家一段抉擇時間。
tell $n 隨時能以 check card 或 check dealer 來看手上的牌或荷官手上的牌。
tell $n -------------------------------------------
tell $n 在抉擇時間中，閒家能選擇是否要加注進行一次分牌(splitcard)或投降(surrender)。
tell $n 分牌：閒家在發牌後持有相同數字的手牌時，可以用相同的注碼將兩張牌分開下成兩注，最多進行一次。
tell $n 分出來的牌與原本的牌當下會再各補一張牌，並且隨時能以 check card2 來看手上的分牌。
tell $n 投降：在分牌及加牌前，可以直接結束該次遊戲，但將失去一半的注碼。
tell $n 但請注意，分牌後投降等同於輸給莊家，將失去所有注碼不會歸還一半。
tell $n -------------------------------------------
tell $n 選擇時間結束後，莊家會翻開暗牌，此時會有加牌時間，閒家可以要求加牌(hit)最多能補到 5 張。
tell $n 本賭場加牌規則無先後順序，且加牌皆為公開的明牌，只要在一定時間內都可敲桌加牌。
tell $n 如不需要加牌，只要等加牌時間結束就行，本賭場不需要特別喊停牌。
tell $n -------------------------------------------
tell $n 結束加牌時間後，莊家的點數如果不超過 17 點，則至少要加牌至大於等於 17 點。
tell $n 結束莊家補牌後，如果沒超過 21 爆牌，則開始計算勝負(blackjack-rule2)。
~
>talk_prog p blackjack-rule2~
tell $n 本賭場二十一點的勝負計算規則：
tell $n 閒家牌面點數比莊家大時，則閒家可以獲得額外 1 倍的注碼。
tell $n 反之，莊家比閒家牌面大時則會失去所有注碼。
tell $n 當閒家與莊家同點時平手，閒家可以取回注碼。
tell $n 當莊家持有點數超過 21 爆牌時，所有閒家都可以獲得額外 1 倍的注碼。
tell $n -------------------------------------------
tell $n 本賭場的特殊規則：
tell $n 閒家在加牌前直接穫得 21 點，則直接贏得該場遊戲並獲得 1.5 倍的注碼。
tell $n 但分牌的情況下補牌湊得 21 點不會直接判定閒家勝利。
tell $n 且該閒家將要等到該場遊戲開始後才能進行下一場遊戲。
tell $n 莊家在翻開暗牌後獲得 21 點則直接贏得遊戲，剩餘的閒家將直接賠掉注碼。
tell $n -------------------------------------------
tell $n 閒家在加牌後持有 5 張牌並且剛好等於 21 點，且贏過莊家則可以獲得額外 2 倍的注碼。
tell $n 但如果持有 5 張牌與莊家同點，則算平手。
tell $n 另外請注意，下注後如果閒家因各種原因從在臺桌前消失，則視為閒家無條件輸掉下注籌碼。
tell $n 如果對了解以上內容，可以看看牌面點數的計算(blackjack-rule3)方式。
~
>talk_prog p blackjack-rule3~
tell $n 紙牌的點數與紙牌上的數字相同，分別為：２、３、４、５、６、７、８、９、１０。
tell $n 文字紙牌的部分Ｊ、Ｑ、Ｋ代表著 10 點，而 Ａ 能當成 1 點或 11 點。
tell $n 閒家發牌時獲得任一 10 點牌及 Ａ 牌，即獲得為 21 點。
tell $n -------------------------------------------
tell $n 而紙牌的花色在本遊戲中不影響任何點數及勝負，僅有賞心悅目的功用。
tell $n 此外本賭場牌池中每少一張就會補進一張相同的牌並重新洗牌，可以視牌池有無限副牌。
tell $n 另外本桌臺都兌換手上大小不同的籌碼(chips)。
tell $n 如果對規則沒有任何疑問，則可以下注(bet)進行遊戲。
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