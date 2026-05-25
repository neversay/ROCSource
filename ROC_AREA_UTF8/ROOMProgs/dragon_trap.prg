>room_time 10~
if roomvalue($0) == 1
	modrvt v0 = 0
endif
~
>room_command disable trap~
echoat $n 你嘗試解除這間房間內的陷阱....
echoaround $n $n 嘗試解除這間房間內的陷阱....
if roomvalue($0) == 0
	if class($n) == 10
		if int($n) <= 15
			if rand(20)
				callout 2 echoat $n [1;36m你成功的解除了房間之內的陷阱！。[m
				callout 2 echoaround $n $n [1;36m成功的解除了房間之內的陷阱....。[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m你失敗了！[m
				callout 2 echoaround $n $n [1;31m解除房間內的陷阱失敗了！[m
				if rand(60)
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 17
			if rand(40)
				callout 2 echoat $n [1;36m你成功的解除了房間之內的陷阱！。[m
				callout 2 echoaround $n $n [1;36m成功的解除了房間之內的陷阱....。[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m你失敗了！[m
				callout 2 echoaround $n $n [1;31m解除房間內的陷阱失敗了！[m
				if rand(40)
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 19
			if rand(60)
				callout 2 echoat $n [1;36m你成功的解除了房間之內的陷阱！。[m
				callout 2 echoaround $n $n [1;36m成功的解除了房間之內的陷阱....。[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m你失敗了！[m
				callout 2 echoaround $n $n [1;31m解除房間內的陷阱失敗了！[m
				if rand(20)
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 21
			if rand(80)
				callout 2 echoat $n [1;36m你成功的解除了房間之內的陷阱！。[m
				callout 2 echoaround $n $n [1;36m成功的解除了房間之內的陷阱....。[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m你失敗了！[m
				callout 2 echoaround $n $n [1;31m解除房間內的陷阱失敗了！[m
				if rand(10)
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 23
			callout 2 echoat $n [1;36m你成功的解除了房間之內的陷阱！。[m
			callout 2 echoaround $n $n [1;36m成功的解除了房間之內的陷阱....。[m
			callout 2 modrvt v0 = 1
		endif
	else
		callout 1 echoat $n [1;33m你的職業不是忍者，並不會解除房間陷阱....。[m
		callout 1 echoaround $n $n [1;33m嘗試解除房間的陷阱，但是失敗了....。[m
		callout 2 echoat $n [1;33m你觸動了房間內的陷阱....。[m
		callout 2 echoaround $n $n [1;33m觸動了房間內的陷阱....。[m
		callout 2 call dragon_trap $n
	endif
else
	if rand(50)
		echo [1;36m這間房間的陷阱已經被解除了....。[m
	else
		echo [1;36m這間房間似乎沒有什麼異樣....。[m
	endif
endif
~
>room_command search around~
echoat $n 你嘗試搜索這間房間....
echoaround $n $n 嘗試搜索這間房間....
if roomvalue($0) == 0
	if class($n) == 10
		if int($n) <= 15
			if rand(20)
				callout 2 echoat $n [1;31m你發現這間房間似乎有陷阱存在，必須要解除(disable trap)。[m
				callout 2 echoaround $n $n [1;31m發現這間房間似乎有陷阱存在....。[m
			else
				if rand(40)
					callout 2 echo [1;36m這間房間似乎沒有什麼異樣....。[m
				else
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 17
			if rand(40)
				callout 2 echoat $n [1;31m你發現這間房間似乎有陷阱存在，必須要解除(disable trap)。[m
				callout 2 echoaround $n $n [1;31m發現這間房間似乎有陷阱存在....。[m
			else
				if rand(60)
					callout 2 echo [1;36m這間房間似乎沒有什麼異樣....。[m
				else
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 19
			if rand(60)
				callout 2 echoat $n [1;31m你發現這間房間似乎有陷阱存在，必須要解除(disable trap)。[m
				callout 2 echoaround $n $n [1;31m發現這間房間似乎有陷阱存在....。[m
			else
				if rand(80)
					callout 2 echo [1;36m這間房間似乎沒有什麼異樣....。[m
				else
					callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 21
			if rand(80)
				callout 2 echoat $n [1;31m你發現這間房間似乎有陷阱存在，必須要解除(disable trap)。[m
				callout 2 echoaround $n $n [1;31m發現這間房間似乎有陷阱存在....。[m
			else
				callout 2 echo [1;36m這間房間似乎沒有什麼異樣....。[m
			endif
			break
		endif
		if int($n) <= 23
			callout 2 echoat $n [1;31m你發現這間房間似乎有陷阱存在，必須要解除(disable trap)。[m
			callout 2 echoaround $n $n [1;31m發現這間房間似乎有陷阱存在....。[m
		endif
	else
		if rand(5)
			callout 2 echoat $n [1;31m你發現這間房間似乎有陷阱存在，必須要解除(disable trap)。[m
			callout 2 echoaround $n $n [1;31m發現這間房間似乎有陷阱存在....。[m
		else
			if rand(20)
				callout 2 echo [1;36m這間房間似乎沒有什麼異樣....。[m
			else
				callout 2 echo [1;31m糟糕！你觸發了陷阱....！[m
				callout 2 call dragon_trap $n
			endif
		endif
	endif
else
	if rand(50)
		echo [1;36m這間房間的陷阱已經被解除了....。[m
	else
		echo [1;36m這間房間似乎沒有什麼異樣....。[m
	endif
endif
~
>room_trap 251720655 80~
echo [1;31m喀啦一聲！你似乎觸動了機關....！[m
if roomvalue($0) == 1
	callout 1 echo [1;31m但是似乎沒有什麼事情發生....。[m
else
	call dragon_trap $n
endif
~
>room_defun dragon_trap~
if rand(20)
	echo [1;31m忽然一隻龍忽然飛了出來向你猛烈攻擊....[m
	mload 2222
	callout 1 force dragon hunt $r
	break
endif
if rand(25)
	echo [1;31m天花板與四周的牆壁忽然射出了幾支飛鏢....[m
	if class($n) == 10
		callout 1 echoat $n [1;36m身為忍者的你以純熟的技巧檔住了這幾支飛鏢....。[m
		callout 1 echoaround $n [1;36m身為忍者的 $n 以純熟的技巧檔住了這幾支飛鏢....。[m
		callout 1 oload 10208 viclv $n floor
	else
		if rand(50)
			callout 1 echo [1;33m這幾支飛鏢造成了你不小的傷害....。[m
			callout 1 modhmv pc 03.10 25 0
		else
			callout 1 echo [0;36m你趕緊一閃，飛鏢掠身而過....。[m
		endif
		callout 2 oload 10208 viclv $n floor
	endif
	break
endif
if rand(33)
	echo [1;31m一群龍獸忽然出現向你攻了過來....[m
	mload 10253
	mload 10252
	force medium mpgroup self leader 0 1
	mload 10250
	force patrol mpgroup self leader 0 1
	force leader hunt $n
	force leader c fireball $n
else
	echo [1;32m天花板與四周的牆壁忽然噴出了毒水....[m
	if rand(40)
		callout 1 echo [1;32m這些毒水讓你中毒了....。[m
		callout 1 add_affect pc poison 0 50 1 -5 4096
		break
	endif
	if rand(45)
		callout 1 echo [1;32m這些毒水嚴重侵蝕你的皮膚....。[m
		callout 1 modhmv pc 03.10 25 0
	else
		callout 1 echo [1;32m這些毒水讓你中毒了....。[m
		callout 1 echo [1;32m這些毒水嚴重侵蝕你的皮膚....。[m
		callout 1 add_affect pc poison 0 50 1 -5 4096
		callout 1 modhmv pc 03.10 25 0
	endif
endif
~
|