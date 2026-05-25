>fight_prog 60~
mpdis_magic self all
mpecho $I 的身軀被黑暗的靈氣圍繞！
mpechoat $n $I [1;36m蒼白的手骨按在你身上！[m
if is_qmark($n) == taboo_skeleton 100005
	mpechoat $n $I 的[1;36;44m寒冰之觸[1;36m完全[1;31m凍結[m了你！
	mpechoat $n [1;36m你已經成了冰冷的屍塊...沒有感覺...沒有知覺...[m
	mpforcer here modhmv $n 03.100 100 0
	mpechoaround $n $I 的[1;36;44m寒冰之觸[m完全凍結了 $n ！
	c 'chill touch' $n
	break
endif
c 'chill touch' $n
mpnowait
if is_qmark($n) == taboo_skeleton 100004
	mpmod_qmark $n taboo_skeleton v0 = 5
	mpechoat $n [1;36m你覺得自己渾身都冰冷僵硬，連心臟都快不行了！[m
	break
endif
if is_qmark($n) == taboo_skeleton 100003
	mpmod_qmark $n taboo_skeleton v0 = 4
	mpechoat $n [1;36m你覺得自己四肢漸漸僵硬起來！[m
	break
endif
if is_qmark($n) == taboo_skeleton 100002
	mpmod_qmark $n taboo_skeleton v0 = 3
	mpechoat $n [1;36m你覺得自己雙腳漸漸失去知覺...[m
	break
endif
if is_qmark($n) == taboo_skeleton 100001
	mpmod_qmark $n taboo_skeleton v0 = 2
	mpechoat $n [1;36m你覺得腳尖有點冰冷...[m
	break
endif
if is_qmark($n) == taboo_skeleton 100000
	mpmod_qmark $n taboo_skeleton v0 = 1
	mpechoat $n [1;36m你覺得有點冰冷...[m
	break
endif
mpadd_qmark $n taboo_skeleton 6 0 0 0 0 1
mpechoaround $n $I 的骷髏手在 $n 的身上抓了一下，$n 似乎感到十分痛苦！
~
>death_prog 100~
if class($n) == 0
or class($n) == 5
or class($n) == 6
or class($n) == 7
or class($n) == 8
or class($n) == 15
or class($n) == 16
	mpecho $I 碎成粉末飛逝......
	mpgreward exp $n -35 100
	mpgreward exp $n 36 50
	mpat 5 mppurge self
	break
else
	if class($n) == 14
	or class($n) == 18
	or class($n) == 19
		if rand(30)
			mpechoat $n $I 的頭骨被你[1;31m重重一擊[m給打碎了！
			mpechoaround $n $I 的頭骨被 $n [1;31m猛烈一擊[m給打爛了！
			mpecho $I 像是斷了線的傀儡散落一地...
			mpgreward exp $n -35 200
			mpgreward exp $n 36 100
			if rand(5)
				mpoload 32137
			endif
			mpat 5 mppurge self
			break
		endif
	endif
	if rand(5)
		mpecho $I 在巨力之下碎成粉末飛逝！
		mpat 5 mppurge self
		break
	endif
	mpecho $I 被重重一擊打爆開來！
	mpecho $I 的碎骨四散！空氣中充滿了黑色的邪氣！
	mpforcer here modhmv pc 03.9 10 0
	mpforcer here modhmv pc 11.100 200 0
	mpecho 你被黑色的邪氣嗆得頭昏眼花、四肢無力......
	mpecho 就當你感到體力喪失的瞬間，你發現碎骨重組了起來！
	if rand(25)
		mpmload 32067
		mpecho $I 的脊椎骨掉到了地上！
		mpecho $I 的眼窟發出紅光！似乎很不爽？！
		mpecho $I 掉到地上的脊椎骨吸收附近的碎骨成了詭異的形狀！
		mpmload 32068
		mpforce fido-fr yell 吼嗚～～～～～嗚～～～～嗚～～～～～～～～
	else
		mpmload 32067
	endif
	mpforce communution-fr get all
	mpforce comminution-fr mpkill $n
	yell ！！On Dead Road！！Let me Die！！
	mpat 5 mppurge self
endif
~
>repop_prog 100~
mpadd_qmark self delayed_cast -1 99999 0 0 1 0
mpadd_qmark self taboo_skeleton_hurt 6 0 0 0 0 1
~
>hurt_prog 92~
mpnowait
mpecho $I 的脊椎骨掉到了地上！
mpecho $I 的眼窟發出紅光！似乎很不爽？！
mpecho $I 掉到地上的脊椎骨吸收附近的碎骨成了詭異的形狀！
mpmload 32068
mpforce fido-fr yell 吼嗚～～～～～嗚～～～～嗚～～～～～～～～
~
>hurt_prog 1005~
if rand(50)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 108~
if rand(60)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 138~
if rand(60)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1000~
if rand(55)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1006~
if rand(40)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1007~
if rand(45)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1008~
if rand(50)
	mpcall taboo_skeleton_load
endif
~
>defun_prog taboo_skeleton_load~
if is_qmark($i) != taboo_skeleton_hurt
	mpadd_qmark self taboo_skeleton_hurt 6 0 0 0 0 1
endif
if is_qmark($i) == taboo_skeleton_hurt 100005
	mpecho $I [1;30;47m碎裂在地，成了原本的碎骨[m......
	if rand(40)
		mpechoat $n 一片碎骨彈到你身上
		mpoload 32137 $n viclv target
	endif
	mpgreward exp $n -35 300
	mpgreward exp $n 36 200
	mpat 5 mppurge self
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100004
	mpmod_qmark self taboo_skeleton_hurt v0 = 5
	mpecho $I [1;30;47m表面佈滿了各種碎裂的紋路，看來快要散了！[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100003
	mpmod_qmark self taboo_skeleton_hurt v0 = 4
	mpecho $I [1;30;47m的頭骨也發出了碎裂的聲音！[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100002
	mpmod_qmark self taboo_skeleton_hurt v0 = 3
	mpecho $I [1;30;47m的腿骨發出碎裂的聲音！[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100001
	mpmod_qmark self taboo_skeleton_hurt v0 = 2
	mpecho $I [1;30;47m的胸骨上碎裂了一塊！[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100000
	mpmod_qmark self taboo_skeleton_hurt v0 = 1
	mpecho $I [1;30;47m的表面出現了一點裂痕！[m
	break
endif
~
|