>talk_prog topic~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
	stare $n
	say 你是誰啊？沒事別來煩我。
else
	glare $n
	mpcallout 1 say 這不是數次參予遠征軍，幫助我們獲勝的$N嗎？
	mpcallout 2 say 請坐！請坐！
	mpcallout 2 mpecho $I從一旁的雜物堆找出一張造型怪異的椅子出來。
	mpcallout 2 say 有什麼是我可以為你效勞(Help)的嗎？
endif
~
>defun_prog destroyarmor~
if is_inv_obj($n) == 13667
or is_inv_obj($n) == 13669
or is_inv_obj($n) == 13670
	mppurge $n energy-elftown
	mpcall destroyarmor $n
endif
~
>talk_prog restart~
if is_qmark($n) == elftown_armorcraft 110000
	if is_inv_obj($n) == 13667
	or is_inv_obj($n) == 13669
	or is_inv_obj($n) == 13670
		mpcall destroyarmor $n
		mpechoat $n 你將$I之前為你煉製的能量靈氣還給了$I。
		mpechoaround $n $N將$I之前為$N煉製的能量靈氣還給了$I。
		mpmod_qmark $n elftown_armorcraft v0 = 0
		mpmod_qmark $n elftown_armorcraft v1 = 0
		mpmod_qmark $n elftown_armorcraft v2 = 0
		mpcallout 1 mpecho $I用咒語將$N的能量靈氣給銷毀了。
	else
		say 將我之前替你強化的裝備帶來吧！
	endif
endif
~
>talk_prog help~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
or is_qmark($n) == elftown_armorcraft 110000
	if is_qmark($n) == elftown_armorcraft 110000
		say 我不是已經幫你強化過裝備了嗎？
		mpcallout 1 say 我最多只會幫一個人打造一件裝備。
		mpcallout 2 say 不過，如果你對我之前幫你強化過的裝備不滿意。
		mpcallout 3 say 你可以把我之前替你強化的裝備還給我。
		mpcallout 4 say 這樣我就會當做沒有(Restart)幫你服務過。
	else
		stare $n
		say 你是誰啊？沒事別來煩我。
	endif
else
	mpadd_qmark $n elftown_armorcraft -1 0 0 0 0 1
	mpechoat $n 你趁這個機會向萊卡梅爾商量為自己強化裝備的事情。
	mpechoaround $n $N趁這個機會向萊卡梅爾商量為自己強化裝備的事情。
	mpcallout 1 ponder
	mpcallout 2 say 我是已經很久沒有為別人打造過裝備了啦....
	mpcallout 3 say 不過既然是精靈族的朋友提出的，那我也不好置之不理。
	mpcallout 4 say 也罷！我最近正好在嘗試如何把光與暗的元素融合在一起。
	mpcallout 5 say 根據我的猜想，這將可以生成純淨且強大的魔法能量。
	mpcallout 6 say 更重要的是，這股魔法能量可以經由特別的手段完全操控，
	mpcallout 7 say 藉此可以將任何魔法裝備重新煉化，使其符合其主人的需求。
	mpcallout 8 say 不過這個實驗還沒有完成，光暗元素要成什麼比例才適當我也還不清楚。
	mpcallout 9 say 如果您能夠提供足夠的法力源，而且材料(Material)也充足的話，
	mpcallout 10 say 我倒是不介意來為您試一試(Try)啦。
endif
~
>talk_prog material~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
or is_qmark($n) == elftown_armorcraft 110000
or is_qmark($n) != elftown_armorcraft
	if is_qmark($n) == elftown_armorcraft 110000
		say 我不是已經幫你強化過裝備了嗎？
		mpcallout 1 say 我最多只會幫一個人打造一件裝備。
		mpcallout 2 say 不過，如果你對我之前幫你強化過的裝備不滿意。
		mpcallout 3 say 你可以把我之前替你強化的裝備還給我。
		mpcallout 4 say 這樣我就會當做沒有(Restart)幫你服務過。
	else
		stare $n
		say 你是誰啊？沒事別來煩我。
	endif
else
	say 要製造出我說的這種裝備，需要大量蘊含光與暗元素的材料。
	mpcallout 1 say 此外，還需要大量的魔力寶石以提供能源。
	if is_qmark($n) > elftown_armorcraft 100029
		mpcallout 2 say 目前庫房裡擁有非常充裕的光之素材。
	else
		if is_qmark($n) > elftown_armorcraft 100014
			mpcallout 2 say 目前庫房裡有還算充裕的光之素材。
		else
			if  is_qmark($n) > elftown_armorcraft 100004
				mpcallout 2 say 目前庫房裡有勉強夠用的光之素材。
			else
				mpcallout 2 say 目前庫房裡的光之素材完全不夠用。
			endif
		endif
	endif
	if is_qmark($n) > elftown_armorcraft 200029
		mpcallout 3 say 還有非常充裕的暗之素材。
	else
		if is_qmark($n) > elftown_armorcraft 200014
			mpcallout 3 say 還有還算充裕的暗之素材。
		else
			if  is_qmark($n) > elftown_armorcraft 200004
				mpcallout 3 say 還有勉強夠用的暗之素材。
			else
				mpcallout 3 say 還有完全不夠用的暗之素材。
			endif
		endif
	endif
	if is_qmark($n) > elftown_armorcraft 300199
		mpcallout 4 say 此外，魔力寶石的存量非常充裕。
	else
		if is_qmark($n) > elftown_armorcraft 300099
			mpcallout 4 say 此外，魔力寶石的存量還算充裕。
		else
			if  is_qmark($n) > elftown_armorcraft 300025
				mpcallout 4 say 此外，魔力寶石的存量勉強夠用。
			else
				mpcallout 4 say  此外，魔力寶石的存量完全不足。
			endif
		endif
	endif
	if is_qmark($n) < elftown_armorcraft 100005
	or is_qmark($n) < elftown_armorcraft 200005
	or is_qmark($n) < elftown_armorcraft 300025
		mpcallout 5 say 恐怕沒辦法現在就為您嘗試(Try)打造裝備。
	else
		mpcallout 5 say 你確定要現在就要我為您嘗試(Try)打造裝備嗎？
	endif
	mpcallout 6 say 如果您想要贊助材料的話，直接交給樓下的奈斯佩森就可以了。
endif
~
>talk_prog try~
if is_qmark($n) < war_reward 200010
or is_qmark($n) != war_reward
or is_qmark($n) == elftown_armorcraft 110000
or is_qmark($n) != elftown_armorcraft
	if is_qmark($n) == elftown_armorcraft 110000
		say 我不是已經幫你強化過裝備了嗎？
		mpcallout 1 say 我最多只會幫一個人打造一件裝備。
		mpcallout 2 say 不過，如果你對我之前幫你強化過的裝備不滿意。
		mpcallout 3 say 你可以把我之前替你強化的裝備還給我。
		mpcallout 4 say 這樣我就會當做沒有(Restart)幫你服務過。
	else
		stare $n
		say 你是誰啊？沒事別來煩我。
	endif
else
	if is_qmark($n) < elftown_armorcraft 100005
	or is_qmark($n) < elftown_armorcraft 200005
	or is_qmark($n) < elftown_armorcraft 300025
	or is_qmark($i) == working
		if is_qmark($i) == working
			say 我最近很忙，過幾天再說吧！
		else
			say 目前庫房內的材料嚴重不足，無法為您打造裝備喔！
		endif
	else
		nod $n
		mpcallout 1 say 既然您希望的話，我就為您嘗試看看吧！
		mpcallout 2 say 請交給我一件您想要重塑屬性的裝備，
		mpcallout 3 say 裝備本身的屬性越好，重塑後的屬性就能夠越強。
		mpcallout 4 say 當然，萬一煉製失敗的話，這件裝備也會跟著毀壞。
		mpcallout 5 say 對了，太差的裝備我可是會拒收的。
		mpcallout 5 mpadd_qmark $n likemyer_workfor -1 0 0 0 0 0
	endif
endif
~
>defun_prog trycraft~
mpecho $I將你的裝備丟到了魔法熔爐裡。
mpcallout 1 mpecho $I從庫房裡搬出了一堆材料。
mpcallout 2 ponder
mpcallout 3 mpecho $I一口氣將所有材料都倒在熔爐裡了！？
mpcallout 4 mpecho ....這樣不會有問題嗎？
mpcallout 5 ponder
mpcallout 6 mpecho $I一邊喃喃地唸起咒文，一邊往熔爐添加著魔力寶石....
mpcallout 7 mpecho 你的裝備在高溫下逐漸融化....
mpcallout 8 mpecho $I喃喃地唸著咒文....
mpcallout 9 mpecho $I喃喃地唸著咒文....
mpcallout 10 mpecho $I喃喃地唸著咒文....
mpcallout 11 mpecho $I喃喃地唸著咒文....
mpcallout 12 mpecho $I喃喃地唸著咒文....
mpcallout 13 mpecho $I喃喃地唸著咒文....
mpcallout 14 mpecho $I喃喃地唸著咒文....
mpcallout 15 mpecho $I喃喃地唸著咒文....
mpcallout 16 mpecho $I喃喃地唸著咒文....
mpcallout 17 mpecho $I喃喃地唸著咒文....
mpcallout 19 mpecho $I高舉雙手，將手上凝聚成團的高質量魔力探入了熔爐之中！
mpcallout 20 mpecho 魔法熔爐裡似乎亮起了一團光芒。
mpcallout 21 mpecho 魔法熔爐裡的光芒緩緩旋轉著....
mpcallout 22 mpecho 魔法熔爐裡的光芒緩緩旋轉著....
mpcallout 23 mpecho 魔法熔爐裡的光芒緩緩旋轉著....
mpcallout 24 mpecho 魔法熔爐裡的光芒緩緩旋轉著....
mpcallout 25 mpecho 魔法熔爐裡的光芒緩緩旋轉著....
if is_qmark($n) < elftown_armorcraft 100005
or is_qmark($n) < elftown_armorcraft 200005
or is_qmark($n) < elftown_armorcraft 300025
or is_qmark($n) != elftown_armorcraft
	mpcall workfail $n
else
	if is_qmark($n) < elftown_armorcraft 100010
	or is_qmark($n) < elftown_armorcraft 200010
	or is_qmark($n) < elftown_armorcraft 300050
		if rand(75)
			mpcall workfail $n
		else
			mpcall worksuccess $n
		endif
	else
		if is_qmark($n) < elftown_armorcraft 100015
		or is_qmark($n) < elftown_armorcraft 200015
		or is_qmark($n) < elftown_armorcraft 300075
			if rand(50)
				mpcall workfail $n
			else
				mpcall worksuccess $n
			endif
		else
			if is_qmark($n) < elftown_armorcraft 100020
			or is_qmark($n) < elftown_armorcraft 200020
			or is_qmark($n) < elftown_armorcraft 300100
				if rand(25)
					mpcall workfail $n
				else
					mpcall worksuccess $n
				endif
			else
				if is_qmark($n) < elftown_armorcraft 100030
				or is_qmark($n) < elftown_armorcraft 200030
				or is_qmark($n) < elftown_armorcraft 300200
					if rand(10)
						mpcall workfail $n
					else
						mpcall worksuccess $n
					endif
				else
					mpcall worksuccess $n
				endif
			endif
		endif
	endif
endif
~
>defun_prog workfail~
mpcallout 26 mpecho 魔法熔爐裡的光芒慢慢黯淡了下來....
mpcallout 27 mpecho 魔法熔爐裡的光芒慢慢黯淡了下來....
mpcallout 28 mpecho 魔法熔爐裡的光芒慢慢黯淡了下來....
mpcallout 29 mpecho 魔法熔爐裡的光芒慢慢熄滅了。
mpcallout 30 glare
mpcallout 31 say 看來這次的嘗試又失敗了啊！
mpcallout 32 say 看來我還需要再想想才行....
mpcallout 33 mpechoat $n $I揮了揮手，似乎在對你下逐客令了。
mpcallout 33 mpechoaround $n $I揮了揮手，似乎在對$N下逐客令了。
mpmod_qmark self working timer = 500
mpdel_qmark $n likemyer_workfor
mpmod_qmark $n elftown_armorcraft v0 = 0
mpmod_qmark $n elftown_armorcraft v1 = 0
mpmod_qmark $n elftown_armorcraft v2 = 0
mpjunk energy-elftown
~
>defun_prog worksuccess~
mpcallout 26 mpecho 魔法熔爐裡的光芒越來越亮、越來越亮....
mpcallout 27 mpecho 魔法熔爐裡的光芒越來越亮、越來越亮....
mpcallout 28 mpecho 魔法熔爐裡的光芒越來越亮、越來越亮....
mpcallout 29 mpecho 魔法熔爐裡的光芒慢慢凝結成一團穩定的光球。
mpcallout 30 say 成....成功了！我成功了！
mpcallout 31 say 很好，現在請你告訴我你所希望的裝備能力吧！
mpcallout 32 say 我想不需要我提醒，你所能附加在裝備上的魔法強度是有限的。
mpcallout 33 say 魔法傷害(Md)、命中(Hr)、傷害(Dr)還是魔法潛力(Pp)呢？
mpcallout 33 say 元素抗性(Sve)、物理傷害抗性(Pdr)、魔法傷害抗性(Mdr)、物理傷害(Pd)、
mpcallout 33 say 體質(Con)、防禦力(Ac)、閃避力(Dc)、法術與龍息抗性(Svs)、
mpcallout 33 say 你希望裝備為你增加的是力量(Str)、智力(Int)、智慧(Wis)、敏捷(Dex)、
mpcallout 33 mpmod_qmark $n likemyer_workfor v0 = 1
mpmod_qmark $n elftown_armorcraft v0 = 0
mpmod_qmark $n elftown_armorcraft v1 = 0
mpmod_qmark $n elftown_armorcraft v2 = 0
~
>talk_prog str~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300001
	if is_qmark($i) & working 300001
		say 你的裝備已經包含力量屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 1
	say 你希望增加多少程度的力量屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(3)
endif
~
>talk_prog int~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300002
	if is_qmark($i) & working 300002
		say 你的裝備已經包含智力屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 2
	say 你希望增加多少程度的智力屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(3)
endif
~
>talk_prog wis~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300004
	if is_qmark($i) & working 300004
		say 你的裝備已經包含智慧屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 3
	say 你希望增加多少程度的智慧屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(3)
endif
~
>talk_prog dex~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300008
	if is_qmark($i) & working 300008
		say 你的裝備已經包含敏捷屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 4
	say 你希望增加多少程度的敏捷屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(3)
endif
~
>talk_prog con~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300016
	if is_qmark($i) & working 300016
		say 你的裝備已經包含體質屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 5
	say 你希望增加多少程度的體質屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(3)
endif
~
>talk_prog ac~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300032
	if is_qmark($i) & working 300032
		say 你的裝備已經包含防禦力屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 6
	say 你希望增加多少程度的防禦力屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(10)
endif
~
>talk_prog dc~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300064
	if is_qmark($i) & working 300064
		say 你的裝備已經包含閃避力屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 7
	say 你希望增加多少程度的閃避力屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(10)
endif
~
>talk_prog svs~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300128
	if is_qmark($i) & working 300128
		say 你的裝備已經包含法術與龍息抗性屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 8
	say 你希望增加多少程度的法術與龍息抗性屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(10)
endif
~
>talk_prog pdr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300256
	if is_qmark($i) & working 300256
		say 你的裝備已經包含物理傷害抗性屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 9
	say 你希望增加多少程度的物理傷害抗性屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(8)
endif
~
>talk_prog mdr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 300512
	if is_qmark($i) & working 300512
		say 你的裝備已經包含魔法傷害抗性屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 10
	say 你希望增加多少程度的魔法傷害抗性屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(8)
endif
~
>talk_prog pd~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 301024
	if is_qmark($i) & working 301024
		say 你的裝備已經包含物理傷害屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 11
	say 你希望增加多少程度的物理傷害屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(8)
endif
~
>talk_prog md~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 302048
	if is_qmark($i) & working 302048
		say 你的裝備已經包含魔法傷害屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 12
	say 你希望增加多少程度的魔法傷害屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(8)
endif
~
>talk_prog hr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 304096
	if is_qmark($i) & working 304096
		say 你的裝備已經包含命中屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 13
	say 你希望增加多少程度的命中屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(4)
endif
~
>talk_prog dr~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 308192
	if is_qmark($i) & working 308192
		say 你的裝備已經包含傷害屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 14
	say 你希望增加多少程度的傷害屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(5)
endif
~
>talk_prog pp~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 316384
	if is_qmark($i) & working 316384
		say 你的裝備已經包含魔法潛力屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 15
	say 你希望增加多少程度的魔法潛力屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(4)
endif
~
>talk_prog sve~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) & working 332768
	if is_qmark($i) & working 332768
		say 你的裝備已經包含元素抗性屬性了喔！
	endif
else
	mpmod_qmark self working v1 = 16
	say 你希望增加多少程度的元素抗性屬性呢？
	say 裝備只能附加同一種屬性一次，請謹慎考慮喔！
	mpechoat $n 　　　少(1)～多(8)
endif
~
>talk_prog 0~
if is_qmark($n) != likemyer_workfor 100001
else
	mpmod_qmark self working v1 = 0
	say 告訴我你所希望的裝備屬性吧！
endif
~
>defun_prog addability~
nod $n
mpcallout 1 mpecho $I喃喃地唸起咒文，以魔力操控著魔法熔爐裡的光團....
mpcallout 2 mpecho 魔法熔爐裡的光團緩緩旋轉著....
mpcallout 3 mpecho 魔法熔爐裡的光團緩緩旋轉著....
mpcallout 4 mpecho 魔法熔爐裡的光團緩緩旋轉著....
mpcallout 6 mpcall isdone $n
~
>defun_prog isdone~
if is_qmark($i) < working 100004
	mpcall doneitem $n
else
	say 如果你覺得這件裝備已經足夠強的話，就告訴我(Done)吧！
endif
~
>defun_prog doneitem~
if is_qmark($i) > working 100000
	mpmod_qmark self working v0 - 1
	mpmodify_obj_affect self energy-elftown 13 + 5 -1
	mpcall doneitem $n
else
	mpmod_qmark $n elftown_armorcraft v0 = 10000
	mpdel_qmark $n likemyer_workfor
	mpdel_qmark self working
	mpecho 魔法熔爐裡的光團緩緩凝結成形。
	mpcallout 1 mpforce $n keep energy-elftown
	mpcallout 1 give energy-elftown $n
	mpcallout 1 say 大功告成了，去試穿一下你的新裝備吧！
endif
~
>talk_prog done~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	mpcall doneitem $n
endif
~
>talk_prog 1~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200001
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1
			mpset_obj_affect self energy-elftown -1 1 1
			mpcallout 5 say 好的，我已經為這件裝備附上力量屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200002
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2
			mpset_obj_affect self energy-elftown -1 3 1
			mpcallout 5 say 好的，我已經為這件裝備附上智力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200003
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4
			mpset_obj_affect self energy-elftown -1 4 1
			mpcallout 5 say 好的，我已經為這件裝備附上智慧屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200004
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8
			mpset_obj_affect self energy-elftown -1 2 1
			mpcallout 5 say 好的，我已經為這件裝備附上敏捷屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200005
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16
			mpset_obj_affect self energy-elftown -1 5 1
			mpcallout 5 say 好的，我已經為這件裝備附上體質屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -10
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -5
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -1
			mpset_obj_affect self energy-elftown -1 24 -1
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 3
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 3
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 2
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100004
			mpcall addability $n
			mpmod_qmark self working v0 - 4
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 2
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 1
			mpcallout 5 say 好的，我已經為這件裝備附上命中屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100005
			mpcall addability $n
			mpmod_qmark self working v0 - 5
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 1
			mpcallout 5 say 好的，我已經為這件裝備附上傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100006
			mpcall addability $n
			mpmod_qmark self working v0 - 6
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 1
			mpcallout 5 say 好的，我已經為這件裝備附上魔法潛力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100005
			mpcall addability $n
			mpmod_qmark self working v0 - 5
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -1
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 2~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200001
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1
			mpset_obj_affect self energy-elftown -1 1 2
			mpcallout 5 say 好的，我已經為這件裝備附上力量屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200002
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2
			mpset_obj_affect self energy-elftown -1 3 2
			mpcallout 5 say 好的，我已經為這件裝備附上智力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200003
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4
			mpset_obj_affect self energy-elftown -1 4 2
			mpcallout 5 say 好的，我已經為這件裝備附上智慧屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200004
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8
			mpset_obj_affect self energy-elftown -1 2 2
			mpcallout 5 say 好的，我已經為這件裝備附上敏捷屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200005
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16
			mpset_obj_affect self energy-elftown -1 5 2
			mpcallout 5 say 好的，我已經為這件裝備附上體質屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -20
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -10
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -2
			mpset_obj_affect self energy-elftown -1 24 -2
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 6
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 6
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 4
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100008
			mpcall addability $n
			mpmod_qmark self working v0 - 8
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 4
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 2
			mpcallout 5 say 好的，我已經為這件裝備附上命中屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100010
			mpcall addability $n
			mpmod_qmark self working v0 - 10
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 2
			mpcallout 5 say 好的，我已經為這件裝備附上傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 2
			mpcallout 5 say 好的，我已經為這件裝備附上魔法潛力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100010
			mpcall addability $n
			mpmod_qmark self working v0 - 10
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -2
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 3~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200001
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1
			mpset_obj_affect self energy-elftown -1 1 3
			mpcallout 5 say 好的，我已經為這件裝備附上力量屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200002
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2
			mpset_obj_affect self energy-elftown -1 3 3
			mpcallout 5 say 好的，我已經為這件裝備附上智力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200003
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4
			mpset_obj_affect self energy-elftown -1 4 3
			mpcallout 5 say 好的，我已經為這件裝備附上智慧屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200004
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8
			mpset_obj_affect self energy-elftown -1 2 3
			mpcallout 5 say 好的，我已經為這件裝備附上敏捷屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200005
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16
			mpset_obj_affect self energy-elftown -1 5 3
			mpcallout 5 say 好的，我已經為這件裝備附上體質屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -30
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -15
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -3
			mpset_obj_affect self energy-elftown -1 24 -3
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 9
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 9
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 6
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100012
			mpcall addability $n
			mpmod_qmark self working v0 - 12
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 6
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 3
			mpcallout 5 say 好的，我已經為這件裝備附上命中屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100015
			mpcall addability $n
			mpmod_qmark self working v0 - 15
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 3
			mpcallout 5 say 好的，我已經為這件裝備附上傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100018
			mpcall addability $n
			mpmod_qmark self working v0 - 18
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 3
			mpcallout 5 say 好的，我已經為這件裝備附上魔法潛力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100015
			mpcall addability $n
			mpmod_qmark self working v0 - 15
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -3
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 4~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -40
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -20
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -4
			mpset_obj_affect self energy-elftown -1 24 -4
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 12
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 12
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 8
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100016
			mpcall addability $n
			mpmod_qmark self working v0 - 16
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 8
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200013
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 4096
			mpset_obj_affect self energy-elftown -1 18 4
			mpcallout 5 say 好的，我已經為這件裝備附上命中屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 4
			mpcallout 5 say 好的，我已經為這件裝備附上傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200015
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 16384
			mpset_obj_affect self energy-elftown -1 40 4
			mpcallout 5 say 好的，我已經為這件裝備附上魔法潛力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -4
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 5~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -50
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -25
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -5
			mpset_obj_affect self energy-elftown -1 24 -5
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 15
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 15
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 10
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100020
			mpcall addability $n
			mpmod_qmark self working v0 - 20
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 10
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200014
		if is_qmark($i) >= working 100025
			mpcall addability $n
			mpmod_qmark self working v0 - 25
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 8192
			mpset_obj_affect self energy-elftown -1 19 5
			mpcallout 5 say 好的，我已經為這件裝備附上傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100025
			mpcall addability $n
			mpmod_qmark self working v0 - 25
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -5
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 6~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -60
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -30
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -6
			mpset_obj_affect self energy-elftown -1 24 -6
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 18
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 18
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 12
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100024
			mpcall addability $n
			mpmod_qmark self working v0 - 24
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 12
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100030
			mpcall addability $n
			mpmod_qmark self working v0 - 30
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -6
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 7~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -70
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -35
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -7
			mpset_obj_affect self energy-elftown -1 24 -7
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 21
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 21
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 14
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100028
			mpcall addability $n
			mpmod_qmark self working v0 - 28
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 14
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100035
			mpcall addability $n
			mpmod_qmark self working v0 - 35
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -7
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 8~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -80
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -40
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -8
			mpset_obj_affect self energy-elftown -1 24 -8
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200009
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 256
			mpset_obj_affect self energy-elftown -1 41 24
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200010
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 512
			mpset_obj_affect self energy-elftown -1 42 24
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200011
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 1024
			mpset_obj_affect self energy-elftown -1 43 16
			mpcallout 5 say 好的，我已經為這件裝備附上物理傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200012
		if is_qmark($i) >= working 100032
			mpcall addability $n
			mpmod_qmark self working v0 - 32
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 2048
			mpset_obj_affect self energy-elftown -1 44 16
			mpcallout 5 say 好的，我已經為這件裝備附上魔法傷害屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200016
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32768
			mpset_obj_affect self energy-elftown -1 35 -8
			mpcallout 5 say 好的，我已經為這件裝備附上元素抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 9~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100036
			mpcall addability $n
			mpmod_qmark self working v0 - 36
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -90
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100036
			mpcall addability $n
			mpmod_qmark self working v0 - 36
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -45
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100036
			mpcall addability $n
			mpmod_qmark self working v0 - 36
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -9
			mpset_obj_affect self energy-elftown -1 24 -9
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>talk_prog 10~
if is_qmark($n) != likemyer_workfor 100001
or is_qmark($i) <= working 100000
else
	if is_qmark($i) == working 200006
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 32
			mpset_obj_affect self energy-elftown -1 17 -100
			mpcallout 5 say 好的，我已經為這件裝備附上防禦力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200007
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 64
			mpset_obj_affect self energy-elftown -1 39 -50
			mpcallout 5 say 好的，我已經為這件裝備附上閃避力屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
	if is_qmark($i) == working 200008
		if is_qmark($i) >= working 100040
			mpcall addability $n
			mpmod_qmark self working v0 - 40
			mpmod_qmark self working v1 = 0
			mpmod_qmark self working v2 | 128
			mpset_obj_affect self energy-elftown -1 23 -10
			mpset_obj_affect self energy-elftown -1 24 -10
			mpcallout 5 say 好的，我已經為這件裝備附上法術與龍息抗性屬性了！
		else
			say 目前的能量已經不夠您進行這樣的增強了喔！
		endif
	endif
endif
~
>give_prog 37037~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這團光輝蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 80189~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 992~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這張盾牌蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37036~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這團光輝蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63366~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63367~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63368~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63369~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63080~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這對羽翼蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 80183~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這顆寶玉蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63341~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這只手鐲蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63340~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這件鎧甲蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63128~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這條腰帶蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63129~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這件披風蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 70987~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這個手環蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63132~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這件鎧甲蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59172~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這件鎧甲蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63001~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這副護腕蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59249~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這只護身符蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59475~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這顆水晶蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59474~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這顆石頭蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59477~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這條墜子蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24221~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這個護腕蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24224~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這個面具蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24225~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這件戰袍蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24227~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這對脛甲蘊含有非常強大的神聖力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13669 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 403~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14417~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這把爪杖蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14423~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這把爪杖蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 44 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 70998~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這面盾牌蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 40 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12061~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這顆晶石蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 38 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 402~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這襲斗篷蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 38 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 70469~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這顆水晶球蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59248~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這條項鍊蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12067~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這件斗篷蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12058~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這頂頭盔蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 12059~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這條腰帶蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 32 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59250~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這對護臂蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59248~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這條項鍊蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59253~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這條腰帶蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14412~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這頂頭盔蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59254~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這雙靴子蘊含有十分詭異的能量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 30 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24223~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這對護臂蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 80185~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這枚戒指蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14409~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這對護臂蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14404~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這根牛角蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 598~
if is_qmark($n) == likemyer_workfor 100000
	say 哦哦....這對肉翼蘊含有非常強大的邪惡力量啊！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 28 0 0 1 0
	mpcallout 34 mpoload 13670 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 14211~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 13485~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59251~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59252~
if is_qmark($n) == likemyer_workfor 100000
	say 這顆光球蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59156~
if is_qmark($n) == likemyer_workfor 100000
	say 這張面具蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59169~
if is_qmark($n) == likemyer_workfor 100000
	say 這條項鍊蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59170~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59171~
if is_qmark($n) == likemyer_workfor 100000
	say 這個護腕蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 59255~
if is_qmark($n) == likemyer_workfor 100000
	say 這件全身鎧蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63127~
if is_qmark($n) == likemyer_workfor 100000
	say 這條頭巾蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63311~
if is_qmark($n) == likemyer_workfor 100000
	say 這件羽衣蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37034~
if is_qmark($n) == likemyer_workfor 100000
	say 這對護臂蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37035~
if is_qmark($n) == likemyer_workfor 100000
	say 這對護臂蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63053~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63054~
if is_qmark($n) == likemyer_workfor 100000
	say 這雙長靴蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63055~
if is_qmark($n) == likemyer_workfor 100000
	say 這條腰帶蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63056~
if is_qmark($n) == likemyer_workfor 100000
	say 這雙手套蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 6~
if is_qmark($n) == likemyer_workfor 100000
	say 這雙手套蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 8~
if is_qmark($n) == likemyer_workfor 100000
	say 這根手杖蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63051~
if is_qmark($n) == likemyer_workfor 100000
	say 這條腕帶蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 584~
if is_qmark($n) == likemyer_workfor 100000
	say 這個軀殼蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 596~
if is_qmark($n) == likemyer_workfor 100000
	say 這張皮件蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 37116~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 32130~
if is_qmark($n) == likemyer_workfor 100000
	say 這團火燄蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 15094~
if is_qmark($n) == likemyer_workfor 100000
	say 這枚戒指蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 15091~
if is_qmark($n) == likemyer_workfor 100000
	say 這頂頭盔蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 63075~
if is_qmark($n) == likemyer_workfor 100000
	say 這條頭巾蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog 24232~
if is_qmark($n) == likemyer_workfor 100000
	say 這串手鍊蘊藏的魔力還算不錯，就用它了吧！
	mpcallout 1 say 讓我將它重新煉化成裝甲吧！
	mpadd_qmark self working 100 24 0 0 1 0
	mpcallout 34 mpoload 13667 36
	mpmod_qmark $n likemyer_workfor v0 = 2
	mpjunk $o
	mpcallout 2 mpcall trycraft $n
else
	? $n
	drop $o
endif
~
>give_prog all~
if is_qmark($n) == likemyer_workfor
	say 這件裝備不行！
	drop $o
else
	? $n
	drop $o
endif
~
|
