>greet_prog 100~
	if is_qmark($n) == forest_enemy 100000
		if is_qmark($n) != forest_quest
			mpechoat $n $I 口中不知道喃喃唸著什麼東西....！
			mpechoaround $n $I 忽然提起武器往 $n 衝了過去....！
			mpechoat $n $I 忽然提起武器往你衝了過來....！
			mpat 5 invoke
			hunt $n
		else
			mpechoat $n $I 說：為什麼濫殺我們族裏的人....！
			mpechoat $n 悲憤的 $I 拿起武器向你衝了過來....！
			mpechoaround $n $I 忽然提起武器往 $n 衝了過去....！
		endif
		break
	endif
	if is_qmark($i) == forest_fiesta 300000
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if is_qmark($n) !& forest_quest 300256
			mpechoat $n 你見到了 $I ，你心想：該怎麼開始(start)跟他說呢？
		endif
		break
	endif
	if ispc($n)
		if class($n) == 13
or class($n) == 14
or class($n) == 15
			if is_qmark($n) != forest_barbarian_exam
				if is_qmark($n) < forest_quest 100009
					mpechoat $n $I 說：你到魔力之塔的二樓進行野蠻人試煉了嗎？
				endif
			endif
		endif
		break
	endif
~
>talk_prog topic~
	if is_qmark($i) == forest_fiesta 300000
		if is_qmark($n) != forest_quest
			if is_qmark($i) > forest_fiesta 200024
				if is_qmark($i) < forest_fiesta 200048
					mpechoat $n $I 正在靜靜的祈禱，完全不想理你。
				endif
			else
				mpechoat $n $I 說：AST Q ADHGDY GDYRER VHX DDY DSYTVESTV
				mpechoat $n 你完全聽不懂他到底在說些什麼。
				mpechoat $n 不過你可以明顯感覺出來這裡豐年祭的熱鬧氣氛....。
			endif
		else
			if is_qmark($i) < forest_fiesta 200013
or is_qmark($i) > forest_fiesta 200047
				if rand(50)
					mpechoaround $n $n 正用熟練的土語與 $I 說話....。
					mpechoat $n $I 說：你好， $n ，要不要一起跳個舞....。
					mpechoat $n 接著 $I 拉著你的手，就跳起舞來了....。
					mpechoaround $n $I 拉著 $n 的手跳起舞來了....。
				else
					mpechoaround $n $n 正用熟練的土語與 $I 說話....。
					mpechoat $n $I 說：這不是 $n 吧！歡迎參加我們的豐年祭。
					mpechoat $n $I 說：這是我們狩獵動物的肉，來，你吃吃看....。
					mpoload 4253
					give leg $n
					mpforce $n eat leg
					gig
					mpechoat $n $I 說：如何？感覺不錯吧！
				endif
				break
			endif
			if is_qmark($i) > forest_fiesta 200012
				if is_qmark($i) < forest_fiesta 200025
					if rand(50)
						mpechoat $n $I 說：哇！吃的好飽，你自己玩吧！
						gig
					else
						mpechoat $n $I 醉的不省人事，你還是不要管他好了。
					endif
				endif
				break
			endif
			if is_qmark($i) > forest_fiesta 200024
				if is_qmark($i) < forest_fiesta 200048
					mpechoat $n $I 靜靜的在主持祈禱的儀式，完全不理你。
				endif
			endif
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100010
		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		mpechoat $n $I 說：您好，我們蠻荒森林的英雄！
		break
	endif
	if is_qmark($n) == forest_quest 100001
		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		mpechoat $n $I 說：您好，我是這個土人部落的首領。
		mpechoat $n $I 說：我想有關這裡的一些事情，你已經相當的瞭解了。
		mpechoat $n $I 說：我這邊有一些事情(quest)需要你親身去完成，
		mpechoat $n $I 說：如果你把他們給完成了，就代表你有前往火山深處消滅賽巴洛司。
		mpechoat $n $I 說：我會提供我所有的協助，幫助你消滅火龍。
		mpmod_qmark $n forest_quest v0 + 1
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) == forest_heart 100001
			 if is_inv_obj($n) == 4248
			 	mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			 	mpechoat $n $I 說：快把[1;36m巨型火蠑螈的心臟(heart of huge fire newt)[0m拿給我吧！
			 endif
			 break
		endif
		if is_qmark($n) == forest_scale 100001
			 if is_inv_obj($n) == 4244
			 	mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			 	mpechoat $n $I 說：快把[1;36m蛇后的鱗片(scale of snake queen)[0m拿給我吧！
			 endif
			 break
		endif
		if is_qmark($n) == forest_eyes 100001
			 if is_inv_obj($n) == 4245
			 	mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			 	mpechoat $n $I 說：快把[1;36m獅王的眼睛(eyes of lion king)[0m拿給我吧！
			 endif
			 break
		endif
		if is_qmark($n) == forest_skin 100001
			 if is_inv_obj($n) == 4246
			 	mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			 	mpechoat $n $I 說：快把[1;36m洞穴巨人王的皮膚(skin of troll king)[0m拿給我吧！
			 endif
			 break
		endif
		if is_qmark($n) < forest_burgeon 100003
			 if is_inv_obj($n) == 4247
			 	mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			 	mpechoat $n $I 說：快把[1;36m神木之頂的嫩芽(burgeon of old tree)[0m拿給我吧！
			 endif
			 break
		endif
		break
	endif
	if is_qmark($n) != forest_quest
		mpechoat $n $I 說：GEWR AG AEWTABTS SDY ESVEUUs SEVY QW！
		mpechoat $n 你完全聽不懂他到底在說些什麼。
	endif
~
>talk_prog identify~
	if is_qmark($n) & forest_quest 300512
		if is_qmark($n) !& forest_quest 301024
			mpecho $I 的臉上露出十分欣喜的笑容....。
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：我們的恩人，沒想到還得麻煩你做這種雞毛蒜皮的事。
			mpechoat $n $I 說：我相信你訓練的成果，我同意他們的婚事了。
			mpechoat $n 艾蓮娜(Alleina) 與 霍克（Hawk) 兩個人緊緊的抱在一起。
			mpechoat $n $I 說：[1;36m好了，你們兩個先離開吧！我有話(words)要跟 $n 說。[0m
			mpecho 霍克 與 艾蓮娜 手牽著手到外面去逛了....。
			mpmod_qmark $n forest_quest v2 + 1024
			mptransfer hawk
			mppurge hawk
			mptransfer alleina
			mppurge alleina
		endif
	endif
~
>talk_prog words~
	if is_qmark($n) & forest_quest 301024
		mpechoat $n $I 見 霍克 與 艾蓮娜 都離開了之後，忽然向你跪了下來。
		mpechoaround $n $I 見 霍克 與 艾蓮娜 都離開了之後，忽然向 $n 跪了下來。
		mpechoat $n 你連忙將 $I 給扶了起來....。
		mpechoround $n $n 連忙將 $I 給扶了起來。
		mpechoat $n $I 說：謝謝，連我女兒的終生大事都要麻煩您，真是....。
		mpechoat $n $I 說：我都不知道怎麼感謝您了。
		mpechoat $n [1;32m你心想：是否應該問問他們何時舉辦婚禮(marriage)呢？[0m
	endif
~
>talk_prog marriage~
	 if is_qmark($n) & forest_quest 301024
	 	if is_qmark($n) !& forest_quest 302048
	 		hop
	 		mpechoat $n $I 說：您能夠來參加當然是最好不過的了。
	 		mpechoat $n $I 說：如果小女與霍克的婚禮舉行，我一定立刻去通知您。
	 		mpechoat $n $I 說：不過您幫了我們土人部落這麼多的忙，我時在無以回報。
	 		mpechoat $n $I 說：我這把隨身的[1;36m魔法鋼刀(Magicla Steel Knife)[0m就送給您吧！[0m
	 		mpoload 4215 36
	 		mpat 5 c enc knife
	 		mpmod_qmark $n forest_quest v2 + 2048
	 		give knife $n
	 		wave $n
	 	endif
	 endif
~
>talk_prog train~
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				bow $n
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n $I 說：實在太感謝您了，希望您能帶著小子出去見見世面。
				mpechoat $n $I 說： 霍克 根本不知道外面世界的凶險。
				mpforce hawk blush
				mpechoat $n $I 說：隨著 霍克 戰鬥經驗的提升，他會慢慢進步。
				mpechoat $n $I 說：[1;36m當 霍克 真正的由戰鬥中瞭解野蠻人戰鬥技巧的精髓時，[0m
				mpechoat $n $I 說：[1;36m我就把女兒嫁給他。當 霍克 準備(ready)好，就可以出發。[0m
				mpforce alleina follow self
				mpforce alleina mpmod_qmark self forest_fiesta v0 = 8
				mpforce hawk mpmod_qmark self forest_fiesta v0 = 8
			endif
		endif
	endif
~
>talk_prog stop~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				mpechoaround $n $n 開始開口調停他們的紛爭....。
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n [1;32m你向 $I 表明了自己願意協助 霍克 成長的意願。[0m
				gasp $n
				mpechoat $n $I 說：當一個訓練的老師是相當繁瑣累人的，
				mpechoat $n $I 說：您真的願意訓練(train)這不成材的小子，幫助他成長？
			endif
		endif
	endif
~
>talk_prog start~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoaround $n [1;32m他們好像爭吵了起來....！[0m
				mpechoat $n $I 說： 霍克 你來做什麼？難道你去求 $n 幫忙？
				mpechoat $n $I 說：你難道不瞭解你的經歷仍然不夠嗎？
				mpechoat $n $I 說：竟敢去麻煩我們土人部落的恩人....！
				mpforce alleina shake
				mpechoat $n 艾蓮娜(Alleina) 說：爸爸，你誤會了，是我找來的....。
				mpechoat $n [1;32m當他們吵的不可開交時你心想：是你自己該出面的時候了(stop)！[0m
			else
				mpechoat $n 遠方傳來一陣聲響：人未到齊喔！
			endif
		else
			mpechoat $n 遠方傳來一陣聲響：人未到齊喔！
		endif
	endif
~
>talk_prog quest~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 300063
		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		mpechoat $n $I 說：你已經完成我需要你做的所有任務....。
		break
	endif
	if is_qmark($n) == forest_quest 100002
		mpechoat $n $I 說：以下是我希望你能夠完成的任務：                                       
		if is_qmark($n) !& forest_quest 300002
			mpechoat $n $I 說：   ‧[1;36m取得沼澤蛇后的鱗片(Scale of Snake Queen)[0m
		endif
		if is_qmark($n) !& forest_quest 300004
			mpechoat $n $I 說：   ‧[1;36m取得獅子之王的眼睛(Eyes of Lion King)[0m
		endif
		if is_qmark($n) !& forest_quest 300008
			mpechoat $n $I 說：   ‧[1;36m取得洞穴巨人王的皮膚(Skin of Troll King)[0m
		endif
		if is_qmark($n) !& forest_quest 300016
			mpechoat $n $I 說：   ‧[1;36m取得神木之頂的嫩芽(Burgeon of Old Tree)[0m
		endif
		if is_qmark($n) !& forest_quest 300032
			mpechoat $n $I 說：   ‧[1;36m取得巨型火蠑螈的心臟(Heart of Fire Newt)[0m
		endif  
		mpechoat $n $I 說：以上那些東西，可以幫助祭師做出可以抵抗火龍高熱的物品。
		mpechoat $n $I 說：請你完成以上的任務，以證明你有資格前往消滅火龍賽巴洛司。
	endif
~
>talk_prog restart~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) == forest_quest 200001
			if is_qmark($n) == forest_scale 100000
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n $I 說：你所接的[1;36m蛇后鱗片[0m任務已經重新開始了。
				mpdel_qmark $n forest_scale
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_eyes 100000
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n $I 說：你所接的[1;36m獅王之眼[0m任務已經重新開始了。
				mpdel_qmark $n forest_eyes
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_skin 100000
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n $I 說：你所接的[1;36m巨人王皮膚[0m任務已經重新開始了。
				mpdel_qmark $n forest_skin
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) < forest_burgeon 100002
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n $I 說：你所接的[1;36m神木嫩芽[0m任務已經重新開始了。
				mpdel_qmark $n forest_burgeon
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_heart 100000
				mpechoaround $n $n 正用熟練的土語與 $I 說話....。
				mpechoat $n $I 說：你所接的[1;36m火蠑螈之心[0m任務已經重新開始了。
				mpdel_qmark $n forest_heart
				mpmod_qmark $n forest_quest v1 = 0
			endif
		endif
	endif
~	
>talk_prog assistant~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 100004
		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		mpmod_qmark $n forest_quest v0 + 1
		mpechoat $n $I 說：沒問題，我立刻叫我族兩個戰士前來幫助你！
		mpecho $I 大喝一聲，立刻出現兩個強壯的土人戰士。
		mpechoat $n [1;32m這兩個菁英戰士，加入了你的隊伍之中。[0m
		mpechoaround $n [1;32m這兩個菁英戰士，加入了[0m $n [1;32m的隊伍之中。[0m
		mpmload 4245
		mpforce native mpoload 4213
		mpforce native mpoload 4218
		mpforce native mpoload 4219
		mpforce native mpdef_rembch $n
		mpforce native mpat 5 wear all
		mpforce native mpgroup $n self 1 0
		mpmload 4245
		mpforce native mpoload 4213
		mpforce native mpoload 4220
		mpforce native mpoload 4223
		mpforce native mpdef_rembch $n
		mpforce native mpat 5 wear all
		mpforce native mpgroup $n self 1 0
		mpechoat $n $I 說：趕緊前往火山口中消滅可惡的 賽巴洛司 吧！
		wave $n
	endif
~		
>talk_prog p heart of fire newt~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_heart
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在正在進行其他的任務。
			mpechoat $n $I 說：請將你先前接的任務給完成。
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_heart 100002
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在不是正在進行這項任務嗎？
			mpechoat $n $I 說：如果你想要重新來過，跟我說 restart 。
			break
		endif
		if is_qmark($n) != forest_heart
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			nod $n
			mpechoat $n $I 說：善於噴火的火蠑螈生長在蠻荒火山的山腰處。
			mpechoat $n $I 說：但是小心火山的高溫會影響你的生理狀態。
			mpechoat $n $I 說：而且附近也有洞穴巨人的出沒，不可不防。
			mpechoat $n $I 說：你必須「親自」完成這個任務，否則無法通過認定。
			wave $n
			mpadd_qmark $n forest_heart -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4248~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		give heart $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_heart 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 		clap $n
		 		mpechoat $n $I 說：恭喜你完成取得[1;36m火蠑螈之心(Heart of Fire Newt)[0m的任務。
		 		mpmod_qmark $n forest_quest v2 + 32
		 		mpdel_qmark $n forest_heart
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk heart
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 			hop
		 			mpechoat $n $I 說：終於拿到五項製作[1;31m火焰神珠(Orb of Flame)[0m的原料了。
		 			mpechoat $n $I 說：我現在立刻將這些原料拿去給祭師，你趕緊隨後跟來。
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n 你看見 $I 出門往祭師住處的方向走去。
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p burgeon of old tree~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_burgeon
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在正在進行其他的任務。
			mpechoat $n $I 說：請將你先前接的任務給完成。
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_burgeon 100003
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在不是正在進行這項任務嗎？
			mpechoat $n $I 說：如果你想要重新來過，跟我說 restart 。
			break
		endif
		if is_qmark($n) != forest_burgeon
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			nod $n
			mpechoat $n $I 說：阻隔東方大沙漠與蠻荒平原的中間，有個濃密的森林地帶。
			mpechoat $n $I 說：森林的中央，有一棵巨大的神木。
			mpechoat $n $I 說：請把神木尖端的嫩芽交給我，但是請小心那裡的老鷹們。
			mpechoat $n $I 說：你必須「親自」完成這個任務，否則無法通過認定。
			mpat 4245 mpoload 4247 floor
			wave $n
			mpadd_qmark $n forest_burgeon -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4247~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		give burgeon $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_burgeon 100002
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 		clap $n
		 		mpechoat $n $I 說：恭喜你完成取得[1;36m神木之頂的嫩芽(Burgeon of Old Tree)[0m的任務。
		 		mpmod_qmark $n forest_quest v2 + 16
		 		mpdel_qmark $n forest_burgeon
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk burgeon
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 			hop
		 			mpechoat $n $I 說：終於拿到五項製作[1;31m火焰神珠(Orb of Flame)[0m的原料了。
		 			mpechoat $n $I 說：我現在立刻將這些原料拿去給祭師，你趕緊隨後跟來。
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n 你看見 $I 出門往祭師住處的方向走去。
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p skin of troll king~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_skin
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在正在進行其他的的任務。
			mpechoat $n $I 說：請將你先前接的任務給完成。
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_skin 100002
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在不是正在進行這項任務嗎？
			mpechoat $n $I 說：如果你想要重新來過，跟我說 restart 。
			break
		endif
		if is_qmark($n) != forest_skin
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			nod $n
			mpechoat $n $I 說：洞穴巨人的巢穴在北方蠻荒火山的山腳下。
			mpechoat $n $I 說：除了巨人王十分的強悍之外，其他的巨人也會攻擊你。
			mpechoat $n $I 說：洞穴巨人的力量非常非常大，希望你千萬小心。
			mpechoat $n $I 說：你必須「親自」完成這個任務，否則無法通過認定。
			wave $n
			mpadd_qmark $n forest_skin -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4246~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		give skin $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_skin 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 		clap $n
		 		mpechoat $n $I 說：恭喜你完成取得[1;36m洞穴巨人王皮膚(Skin of Troll King)[0m的任務。
		 		mpmod_qmark $n forest_quest v2 + 8
		 		mpdel_qmark $n forest_skin
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk skin
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 			hop
		 			mpechoat $n $I 說：終於拿到五項製作[1;31m火焰神珠(Orb of Flame)[0m的原料了。
		 			mpechoat $n $I 說：我現在立刻將這些原料拿去給祭師，你趕緊隨後跟來。
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n 你看見 $I 出門往祭師住處的方向走去。
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p eyes of lion king~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_eyes
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在正在進行其他的任務。
			mpechoat $n $I 說：請將你先前接的任務給完成。
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_eyes 100002
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在不是正在進行這項任務嗎？
			mpechoat $n $I 說：如果你想要重新來過，跟我說 restart 。
			break
		endif
		if is_qmark($n) != forest_eyes
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			nod $n
			mpechoat $n $I 說：獅子們的領域在蠻荒平原的東南方。
			mpechoat $n $I 說：除了獅王十分的強悍之外，其他的獅子也會攻擊你。
			mpechoat $n $I 說：如沒有相當把握，請充實自己的實力之後再去完成任務。
			mpechoat $n
			mpechoat $n $I 說：你必須「親自」完成這個任務，否則無法通過認定。
			wave $n
			mpadd_qmark $n forest_eyes -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4245~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		give eyes $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_eyes 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 		clap $n
		 		mpechoat $n $I 說：恭喜你完成取得[1;36m獅王眼睛(Eyes of Lion King)[0m的任務。
		 		mpmod_qmark $n forest_quest v2 + 4
		 		mpdel_qmark $n forest_eyes
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk eyes
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 			hop
		 			mpechoat $n $I 說：終於拿到五項製作[1;31m火焰神珠(Orb of Flame)[0m的原料了。
		 			mpechoat $n $I 說：我現在立刻將這些原料拿去給祭師，你趕緊隨後跟來。
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n 你看見 $I 出門往祭師住處的方向走去。
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p scale of snake queen~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_scale
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在正在進行其他的任務。
			mpechoat $n $I 說：請將你先前接的任務給完成。
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_scale 100002
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			mpechoat $n $I 說：你現在不是正在進行這項任務嗎？
			mpechoat $n $I 說：如果你想要重新來過，跟我說 restart 。
			break
		endif
		if is_qmark($n) != forest_scale
			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
			nod $n
			mpechoat $n $I 說：蛇后在蠻荒平原的東方沼澤區的最裡面。
			mpechoat $n $I 說：整個沼澤區的沼氣與侵蝕性的沼澤水會傷害你的皮膚。
			mpechoat $n $I 說：因此請多加小心。
			mpechoat $n $I 說：你必須「親自」完成這個任務，否則無法通過認定。
			wave $n
			mpadd_qmark $n forest_scale -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4244~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I 說：抱歉，現在正是豐年祭的重要日子。
		mpechoat $n $I 說：有什麼事請待豐年祭結束之後再說。
		give scale $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_scale 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 		clap $n
		 		mpechoat $n $I 說：恭喜你完成取得[1;36m蛇后鱗片(Scale of Snake Queen)[0m的任務。
		 		mpmod_qmark $n forest_quest v2 + 2
		 		mpdel_qmark $n forest_scale
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk scale
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n 正用熟練的土語與 $I 說話....。
		 			hop
		 			mpechoat $n $I 說：終於拿到五項製作[1;31m火焰神珠(Orb of Flame)[0m的原料了。
		 			mpechoat $n $I 說：我現在立刻將這些原料拿去給祭師，你趕緊隨後跟來。
		 			mpmod_qmark $n forest_quest v0 + 1
		 			open e
		 			close e
		 			mpecho 你看見 $I 出門往祭師住處的方向走去。
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>fight_prog 60~
	if insameroom(alleina)
		mpgroup self alleina 1 0
		protect alleina
		break
	endif
	if isaffected($i) !& 128
		mpat 5 c sanctuary
		break
	endif
	if isaffected($i) & 1
		mpat c 'cure blindness'
	endif
~
>fight_prog 40~
	mpat 5 c heal
	wear sword
	wear sword
	disarm
	if rand(30)
		coh
		break
	endif
	if rand(30)
		frenzy
		break
	endif
	if rand(10)
		mpat 5 invoke
	else
		c blind
		c heal
		hunt $r
	endif
~
>death_prog 50~
	mpdis_magic self 1
	mpat 5 c inf
	if is_qmark($n) != forest_enemy
		mpadd_qmark $n forest_enemy 50 0 0 0 0 1
	else
		mpmod_qmark $n forest_enemy timer + 50
	endif
~
>tick_prog 100~
	if is_qmark($i) == forest_fiesta 300000
		mpmod_qmark self forest_fiesta v1 + 1
		if is_qmark($i) == forest_fiesta 200012
			mpmod_qmark self forest_fiesta v0 = 1
			mpforce alleina mpmod_qmark self forest_fiesta v0 + 1			break
		endif
		if is_qmark($i) == forest_fiesta 200024
			mpmod_qmark self forest_fiesta v0 = 2
			mpforce alleina mpmod_qmark self forest_fiesta v0 + 1
			mpecho $I [1;32m忽然大喝一聲，接著在廣場中央跪了下來....。[0m
			mpecho $I [1;32m不斷的向廣場中央的神像喃喃唸著聽不懂的話。[0m
			break
		endif
		if is_qmark($i) > forest_fiesta 200024
			if is_qmark($i) < forest_fiesta 200048
				if rand(30)
					mpecho $I [1;32m不斷的向廣場中央神像說些你聽不懂的話。[0m
					break
				endif
				if rand(30)
					mpecho 索巴(Thorba) [1;32m在地上慢慢爬行接近神像，好像代表什麼特殊意義。[0m
					break
				endif
				if rand(30)
					mpecho 索巴(Thorba) [1;32m開始念一些咒語，好像是祈求平安的。[0m
				endif
			endif
			break
		endif
		if is_qmark($i) == forest_fiesta 200048
			mpecho $I [1;32m似乎不繼續向廣場中央神像參拜了。[0m
			mpecho $I [1;32m起身結束祈禱儀式，豐年祭的熱鬧氣氛又再度開始了。[0m
			break
		endif
		if is_qmark($i) == forest_fiesta 200072
			mpdel_qmark self forest_fiesta
			rumor [1;32m據聞蠻荒森林土人豐年祭已經結束了。[0m
			mptransfer thorba 4304
			mpgoto 4203
			mppurge feetsteps
			mpgoto 4206
			mppurge feetsteps
			mpgoto 4209
			mppurge feetsteps
			mpgoto 4224
			if insameroom(hawk)
				mppurge hawk
			endif
			if insameroom(alleina)
				mpforce alleina mpdel_qmark self forest_fiesta
				mptransfer alleina 4296
			endif
			mpgoto 4296
		endif
		break
	endif
	if month($) == 0
	or month($) == 6
	or month($) == 12
		if day($) == 1
			if hour($) == 6
				rumor [1;32m據聞六個月一度的蠻荒森林土人豐年祭即將開始。[0m
				endif
			break
			endif
		endif
		if day($) == 18
			if is_qmark($i) != forest_fiesta
				rumor [1;32m據聞蠻荒森林土人豐年祭已經開始了。[0m
				mpat 4304 mpforce thorba mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpforce alleina mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpforce alleina mpgoto 4297
				mpat 4304 mpforce thorba mpgoto 4297
				mpgoto 4297
				mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
			endif
			break
		endif
	endif
	if hour($) > 5
		if hour($) < 7
			wak
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4305
					mpecho $I 起床伸了伸懶腰，似乎要去巡視了。
					open e
					e
					close w
					e
					e
				else
					mpgoto 4305
					mpecho $I 起床伸了伸懶腰，似乎要去巡視了。
					open e
					e
					close w
					e
					e
				endif
			endif
		endif
		break
	endif
	if hour($) > 12
		if hour($) < 14
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4296
					mpecho $I 點了點頭，然後往自己的住處走去。
					w
					w
					open w
					w
					close e
					rest
				else
					mpgoto 4296
					mpecho $I 點了點頭，然後往自己的住處走去。
					w
					w
					open w
					w
					close e
					rest
				endif
			endif
		endif
		break
	endif
	if hour($) > 0
		if hour($) < 2
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4305
					mpecho $I 打了一個哈欠，然後倒頭便睡。
					sleep
				else
					mpgoto 4305
					mpecho $I 打了一個哈欠，然後倒頭便睡。
					sleep
				endif
			endif
		endif
	endif
~
|