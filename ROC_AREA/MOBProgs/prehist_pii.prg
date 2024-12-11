>talk_prog topic~
if is_qmark($i) == prehist_pii_quest
	if being($b)
		if is_qmark($i) == prehist_pii_quest 100000
			if rembch($n)
				say 快帶我去找我的爺爺吧！
			endif
			break
		endif
		if is_qmark($i) == prehist_pii_quest 100003
			if rembch($n)
				sm $b
				say 很榮幸與 $b 您一起在這邊討論有關心與刀的事情。
			endif
			break
		else
			if rembch($n)
				sm $n
			else
				say 抱歉，我現在沒空。
			endif
		endif
	else
		mpdel_qmark self prehist_pii_quest
		say 什麼，你說什麼，你再說一遍！
		break
	endif
endif
if is_qmark($i) == prehist_talking
or is_qmark($n) == prehist_enemy
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say 又是你，我看到你就一肚子氣，
	say 走開，不要擋住我的路。
	break
endif
say 抱歉，我現在很忙，有什麼事快說！
mpechoat $n -- ‧請問一下有關魔刀村的背景(background)或由來。
mpechoat $n -- ‧請問一下有關遠古森林(prehist forest)的一些事情。
mpechoat $n -- ‧請問一下您是做什麼的(class)？
if is_qmark($n) == prehist_xue_quest 100001
	mpechoat $n -- ‧請問一下您都是用什麼招數取悅(please)其他人們的？
	if is_qmark($n) & prehist_xue_quest 200001
		if is_qmark($n) !& prehist_xue_quest 300001
			mpechoat $n -- ‧那麼我應該要怎麼取悅你(please pii)才行？
		endif
	endif
endif
if is_qmark($n) !& prehist_quest 100004
	if is_qmark($n) == prehist_pii_quest 100000
		mpechoat $n -- ‧你說你想要一把自己的刀(your owned knife)，但是爺爺卻不做給你？
	endif
	if is_qmark($n) == prehist_pii_quest 100001
		mpechoat $n -- ‧我試著幫幫你(try to help you)吧！我幫你去問問金長老。
		mpechoat $n -- ‧你這個任性的小子(little damn guy)，你爺爺一定有他的考量啊！
	endif
	if is_qmark($n) == prehist_pii_quest 100004
		mpechoat $n -- ‧小痞，你爺爺說除非你能夠有點長進，不然他不會教你的(directly present)。
	endif
	if is_qmark($n) == prehist_pii_quest 100014
		mpechoat $n -- ‧小痞你準備好了嗎(are you ready)？我們要出發去找 藍(Lan) 老師學習刀技了。
	endif
endif
~
>rand_prog 50~
if being($b)
	if is_qmark($i) == prehist_pii_quest
		if insameroom($b)
		else
			if is_qmark($i) >= prehist_pii_quest 1010
				mpmod_qmark self prehist_pii_quest timer - 5
			endif
		endif
	else
		tell $b 你的動作實在是太慢了，我們下次再見好了！
		if is_qmark($b) < prehist_pii_quest 100012
			mpat $b mpmod_qmark $b prehist_pii_quest v0 = 2
			mpat $b mpmod_qmark $b prehist_pii_quest timer = 72
		else
			mpat $b mpmod_qmark $b prehist_pii_quest v0 = 13
			mpat $b mpadd_qmark $b prehist_pii_waiting 72 0 0 0 0 1
		endif
		mppurge self
	endif
else
	if is_qmark($i) == prehist_pii_quest
		say 又有人沒有完成任務就離開遊戲了。
		curse
		if area($i) != prehistoria
			mpecho $I 見自己遠離了家鄉，便往家鄉的方向走去....
			mppurge self
		else
			mpdel_qmark self prehist_pii_quest
			mpgoto 15634
		endif
	endif
endif
~
>talk_prog restart~
if is_qmark($n) == prehist_enemy
or is_qmark($i) == prehist_pii_quest
or is_qmark($i) == prehist_talking
or is_qmark($n) & prehist_quest 100004
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
	say 又是你，我看到你就一肚子氣，
	say 要重來，等哪一天我氣消了再說。
	break
endif
if is_qmark($n) < prehist_pii_quest 100012
	mpdel_qmark $n prehist_pii_quest
	mpechoat $n 遠方傳來一陣聲響「本項任務已經可以重新開始了！」
else
	if is_qmark($n) != prehist_pii_waiting
		mpmod_qmark $n prehist_pii_quest v0 = 14
		mpmod_qmark $n prehist_pii_quest v2 = 0
		mpechoat $n 遠方傳來一陣聲響「本項任務已經可以由第二階段開始了！」
		if mobcount(15656) == 0
			mpat 15634 mpmload 15656
			mppurge self
		else
			mpgoto 15634
		endif
	else
		say 又是你，我看到你就一肚子氣，
		say 要重來，等哪一天我氣消了再說。
	endif
endif
~
>talk_prog p please pii~
if is_qmark($n) == prehist_enemy
or is_qmark($i) == prehist_pii_quest
or is_qmark($i) == prehist_talking
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say 又是你，我看到你就一肚子氣，
	say 走開，不要擋住我的路。
	break
endif
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		mpadd_qmark self prehist_talking 5 0 0 0 1 0
		mpforce $n say 那麼我應該要怎麼取悅你(please pii)才行？
		grin $n
		say 這個嘛！我想想看....，
		mpcallout 2 say 嗯！我得好好想想看....，
		mpcallout 4 say 這個嘛....！
		mpcallout 5 mpcall prehist_pii_please $n
	endif
endif
~
>defun_prog prehist_pii_please~
if rand(33)
	sm $n
	mpcallout 1 say 這樣好了，我現在身邊缺錢....，
	mpcallout 2 say 先擋點瑯來花花好了....，
	mpcallout 3 say 先給個 50000 金幣，應該不嫌多吧！
	mpcallout 3 mpmod_qmark $n prehist_xue_quest v1 | 2
	mpcallout 3 mpdel_qmark self prehist_talking
	break
endif
if rand(50)
	grin $n
	mpcallout 1 say 我想要吃好吃的蛋糕耶....，
	mpcallout 2 say 我想....，我想吃羅胡城堡的天使蛋糕(angelfood cake)。
	mpcallout 2 mpmod_qmark $n prehist_xue_quest v1 | 4
	mpcallout 2 mpdel_qmark self prehist_talking
else
	ponder $n
	mpcallout 1 say 那麼....，
	mpcallout 2 say 我想要吃永恆之塔的雙倍奶油爆米花(cream puffed-rice)。
	mpcallout 2 mpmod_qmark $n prehist_xue_quest v1 | 8
	mpcallout 2 mpdel_qmark self prehist_talking
endif
~
>bribe_prog 50000~
mpjunk all.coins
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		if is_qmark($n) & prehist_xue_quest 200002
			nod $n
			say 這樣還差不多，算你取悅成功了。
			mpmod_qmark $n prehist_xue_quest v2 | 1
		else
			say 我不是叫你拿這個東西給我吧！
			mpgold $n 50000
		endif
	else
		say 你拿些錢給我要做什麼？
		mpgold $n 50000
	endif
else
	say 你拿些錢給我要做什麼？
	mpgold $n 50000
endif
~
>give_prog 8253~
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		if is_qmark($n) & prehist_xue_quest 200004
			nod $n
			say 這樣還差不多，算你取悅成功了。
			mpmod_qmark $n prehist_xue_quest v2 | 1
		else
			say 我不是叫你拿這個東西給我吧！
			drop angelcake
		endif
	else
		say 你拿這個東西給我幹什麼？
		drop angelcake
	endif
else
	say 你拿這個東西給我幹什麼？
	drop angelcake
endif
~
>give_prog 13402~
if is_qmark($n) & prehist_xue_quest 200001
	if is_qmark($n) !& prehist_xue_quest 300001
		if is_qmark($n) & prehist_xue_quest 200008
			nod $n
			say 這樣還差不多，算你取悅成功了。
			mpmod_qmark $n prehist_xue_quest v2 | 1
		else
			say 我不是叫你拿這個東西給我吧！
			drop puffed-rice
		endif
	else
		say 你拿這個東西給我幹什麼？
		drop puffed-rice
	endif
else
	say 你拿這個東西給我幹什麼？
	drop puffed-rice
endif
~
>talk_prog please~
if is_qmark($n) == prehist_enemy
or is_qmark($i) == prehist_pii_quest
or is_qmark($i) == prehist_talking
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say 又是你，我看到你就一肚子氣，
	say 走開，不要擋住我的路。
	break
endif
if is_qmark($n) == prehist_xue_quest 100001
	mpadd_qmark self prehist_talking 5 0 0 0 1 0
	mpforce $n say 請問一下您都是用什麼招數取悅(please)其他人們的？
	if is_qmark($n) !& prehist_xue_quest 300001
		hmm
		say 說到這個就是我在行的....，
		mpcallout 1 say 不過如果要我告訴你怎麼取悅別人....，
		mpcallout 2 say 你應該要先想辦法取悅我才對。
		mpcallout 2 gig
		mpcallout 2 mpmod_qmark $n prehist_xue_quest v1 | 1
		mpcallout 2 mpdel_qmark self prehist_talking
	else
		nod $n
		say 怎麼取悅人啊！靠一張嘴巴....，
		mpcallout 1 say 首先你要有能夠觀察對方神情的技巧....，
		mpcallout 2 say 才能夠知道該跟對方說哪些話....，
		mpcallout 3 say 當然，這個沒有足夠的智力是辦不到的啦！
		mpcallout 3 cac
		mpcallout 3 mpmod_qmark $n prehist_xue_quest v2 | 1
		mpcallout 3 mpdel_qmark self prehist_talking
	endif
endif
~
>talk_prog background~
if is_qmark($i) == prehist_pii_quest
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say 又是你，我看到你就一肚子氣，
	say 走開，不要擋住我的路。
	break
endif
mpforce $n say 請問一下有關魔刀村的背景(background)或由來。
faint $n
say 我現在哪有時間跟你談這些啊？
wave $n
n
e
s
w
~
>talk_prog p prehist forest~
if is_qmark($i) == prehist_pii_quest
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say 又是你，我看到你就一肚子氣，
	say 走開，不要擋住我的路。
	break
endif
mpforce $n say 請問一下有關遠古森林(prehist forest)的一些事情。
angry
say 拜託，我很忙，不要哪這種雞毛蒜皮的小事來煩我行嗎？
kick $n
~
>talk_prog class~
if is_qmark($i) == prehist_pii_quest
	break
endif
if is_qmark($n) == prehist_pii_quest 100002
or is_qmark($n) == prehist_pii_quest 100013
	say 又是你，我看到你就一肚子氣，
	say 走開，不要擋住我的路。
	break
endif
mpforce $n say 請問一下您是做什麼的(class)？
mpecho $I 用非常訝異的眼光看著你....
say 白癡，你看不出來嗎？我是無所事事啊！
laugh $n
~
>death_prog 100~
mpdis_magic self blindness
mapt 5 c inf
if ispc($n)
	if is_qmark($n) != prehist_enemy
		mpadd_all_qmark $n prehist_enemy -1 0 0 0 1 1
	endif
	yell 金長老爺爺！請記得為我報仇啊！
	if mobcount(15633) > 0
		mpat golden-prehist mpforce golden-prehist yell 是誰？是誰殺了小痞？
		mptransfer golden-prehist
		mpecho 忽然 金長老(Golden Elder) 出現在這邊....
		mpforce golden-prehist hunt $n
	endif
endif
~
|