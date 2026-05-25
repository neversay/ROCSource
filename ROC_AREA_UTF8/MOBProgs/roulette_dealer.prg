>repop_prog 100~
mpadd_qmark self roulette_dealer -1 0 0 0 0 1
mpadd_qmark self roulette_random_timer -1 0 0 0 0 1
mpforcer here def_rembch $i
~
>rand_prog 100~
if is_qmark($i) == roulette_dealer 100001
	if is_qmark($i) == roulette_random_timer 100002
		mpforcer here rcall roulette_timeline $i
		mpmod_qmark self roulette_random_timer v0 = 0
	else
		if rand(80)
			if rand(90)
				mpforcer here rcall roulette_timeline $i
			else
				mpmod_qmark self roulette_random_timer v0 + 1
			endif
		else
			if rand(50)
				mpforcer here rcall roulette_timeline $i
			else
				mpmod_qmark self roulette_random_timer v0 + 1
			endif
		endif
	endif
endif
~
>talk_prog topic~
tell $n 想了解本輪盤的玩法嗎？(roulette-rule)
~
>talk_prog p roulette-rule~
tell $n 只要對著想要的位置下注即可加入遊戲，最終輪盤的數字與下注數字相同則可以得到不同賠率的報酬。
tell $n 下注的方法為 bet '位置 數字格式'，每次下注為 1 單位酬碼，同樣位置只可以下一次，同場遊戲內最多下注 10 次。
tell $n 在遊戲內能隨時使用 check 來看台桌的樣貌及目前自己下注的情況。
tell $n 下注位置有：單數字(single)、數字間(split)、列(street)、角(corner)、排(line)、行(column)、打(dozen)、
tell $n 　　　　　　奇數(odd)、偶數(even)、１～１８(1to18)、１９～３６(19to36)、黑(black)、紅(red)。
tell $n 每種下注位置都有不同的數字格式下面將逐一解釋。(roulette-rule2)
~
>talk_prog p roulette-rule2~
tell $n 單數字(single)：bet 'single [1;37m號碼[1;33m'
tell $n 此方法能直接下注在任一數字上，而號碼為 0 到 36。
tell $n 例：下注號碼 0 則輸入 bet 'single 0'，下注號碼 3 則輸入 bet 'single 3'
tell $n ------------------------------------------------------------------------------
tell $n 數字間(split)：bet 'split [1;37m號碼1[1;33m',[1;37m號碼2[1;33m'
tell $n 在相鄰的兩號碼間的線上下注，能一次選注 2 個數字，但是不包含號碼 0。
tell $n 號碼1 與 號碼2 為台桌上兩個相鄰的號碼，號碼是否相鄰可由 check 指令查看。
tell $n 例：下注 1 與 2 之間則輸入 bet 'split 1,2'，下注 4 與 7 之間則輸入 bet 'split 4,7'
tell $n 註：兩數字以 , 符號區隔，順序可以變更。
tell $n ------------------------------------------------------------------------------
tell $n 列(street)：bet 'street [1;37m最小號碼[1;33m'to[1;37m最大號碼[1;33m'
tell $n 在第一行(column 1)底線上於單數線上下注，能選注一列 3 個號碼，但不包含號碼 0。
tell $n 一列中最小的數字為最小號碼，而最大的數字則是最大號碼，每個列包含的號碼可由 check 指令查看。
tell $n 例：下注 1, 2, 3 的列則輸入 bet 'street 1to3'，下注 25, 26, 27 的列則輸入 bet 'street 25to27'
tell $n ------------------------------------------------------------------------------
tell $n 角(corner)：bet 'corner down[1;37m數字[1;33m' 或 bet 'corner upper[1;37m數字[1;33m'
tell $n 四個號碼的交叉點稱為角，在角下注能一次選注 4 個數字，但不包含號碼 0。
tell $n 台桌上共有 22 個角，由第二行(column 2)的位置區分為下(down)與上(upper)，數字由左到右增加，分別為 1 至 11。
tell $n 每個角包含的號碼可由 check 指令查看。
tell $n 例：下注 1, 2, 4, 5 的角則輸入 bet 'corner down1'，下注 29, 30, 32, 33 的角則輸入 bet 'corner upper10'
tell $n ------------------------------------------------------------------------------
tell $n 排(line)：bet 'line [1;37m最小號碼[1;33m'to[1;37m最大號碼[1;33m'
tell $n 在第一行(column 1)底線上於兩數間線上下注，能選注一排 6 個號碼，但不包含號碼 0。
tell $n 一排中最小的數字為最小號碼，而最大的數字則是最大號碼，每個排包含的號碼可由 check 指令查看。
tell $n 例：下注 1, 2, 3, 4, 5, 6 的排則輸入 bet 'line 1to6'
tell $n 　　下注 22, 23, 24, 25, 26 的排則輸入 bet 'street 22to27'
tell $n ------------------------------------------------------------------------------
tell $n 行(column)：bet 'column [1;37m行數[1;33m'
tell $n 對特定行數下注能一次選注一行中的 12 個數字，但不包含號碼 0。
tell $n 每個行包含的號碼可由 check 指令查看。
tell $n 例：下注第一行則輸入 bet 'column 1'，下注第二行則輸入 bet 'column 2'，下注第三行則輸入 bet 'column 3'
tell $n ------------------------------------------------------------------------------
tell $n 打(dozen)：bet 'dozen [1;37m第幾打[1;33m'
tell $n 對特定打數下注能一次選注一打中的 12 個數字，但不包含號碼 0。
tell $n 每個打包含的號碼可由 check 指令查看。
tell $n 例：下注第一打則輸入 bet 'dozen 1'，下注第二打則輸入 bet 'dozen 2'，下注第三打則輸入 bet 'dozen 3'
tell $n ------------------------------------------------------------------------------
tell $n 偶數(even)：bet even，選注台桌上不包含 0 的所有偶數號碼。
tell $n 奇數(odd)：bet odd，選注台桌上不包含 0 的所有奇數號碼。
tell $n １～１８(1to18)：bet 1to18，選注號碼 1 至 18。
tell $n １９～３６(19to36)：bet 19to36，選注號碼 19 至 36。
tell $n 紅(red)：bet red，選注紅色號碼，號碼的顏色可由 check 指令查看。
tell $n 黑(black)：bet black，選注黑色號碼，號碼的顏色可由 check 指令查看。
tell $n ------------------------------------------------------------------------------
tell $n 如果還想了解押中各位置的賠率可以繼續問我。(roulette-rule3)
~
>talk_prog p roulette-rule3~
tell $n 每次下注為 1 單位，同場遊戲內每位閒家最多下注 10 次。
tell $n ---------------------------------------------
tell $n 單數字(single)                    1 賠 35
tell $n 數字間(split)                     1 賠 17
tell $n 列(street)                        1 賠 11
tell $n 角(corner)                        1 賠 8
tell $n 排(line)                          1 賠 5
tell $n 行(column)                        1 賠 2
tell $n 打(dozen)                         1 賠 2
tell $n 偶數(even)                        1 賠 1
tell $n 奇數(odd)                         1 賠 1
tell $n １～１８(1to18)                   1 賠 1
tell $n １９～３６(19to36)                1 賠 1
tell $n 紅(red)                           1 賠 1
tell $n 黑(black)                         1 賠 1
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