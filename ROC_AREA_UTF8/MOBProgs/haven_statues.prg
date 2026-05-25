>tick_prog 100~
if being($b)
	break
else
	mppurge self
endif
~
>repop_prog 100~
mpadd_qmark self statue_hurt -1 0 0 0 0 1
~
>hitprcnt_prog 50~
mpat 5 c 'full heal'
mpgoto self
~
>fight_prog 100~
mpgoto self
~
>hurt_prog 43~
mpat $b mpecho $N的火球術打得你[1;33m猛噴鮮血，齒牙迸裂[m.(601-700)
mpechoat $n 你的火球術讓冰像開始大量溶化了
mpechoaround $n $N的火球術讓冰像開始大量溶化了
mpat $b mpforcer here modhmv pc 01.601 700 0
mpmod_qmark self statue_hurt v0 + 5
if is_qmark($i) >= statue_hurt 100015
	mpecho $B從溶化的冰雕中走了出來
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 45~
mpat $b mpecho $N的星星之火打得你[1;31m！筋-脈-寸-斷， 血-肉-模-糊！[m(1151-1300)
mpechoat $n 你的星星之火讓冰像開始大量溶化了
mpechoaround $n $N的星星之火讓冰像開始大量溶化了
mpat $b mpforcer here modhmv pc 01.1151 1300 0
mpmod_qmark self statue_hurt v0 + 7
if is_qmark($i) >= statue_hurt 100015
	mpecho $B從溶化的冰雕中走了出來
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 7~
mpat $b mpecho $N的灼熱之掌[1;36m嚇到[m(13-18)你.
mpechoat $n 你的灼熱之掌讓冰像開始溶化了
mpechoaround $n $N的灼熱之掌讓冰像開始溶化了
mpat $b mpforcer here modhmv pc 01.13 18 0
mpmod_qmark self statue_hurt v0 + 1
if is_qmark($i) >= statue_hurt 100015
	mpecho $B從溶化的冰雕中走了出來
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 46~
mpat $b mpecho $N的聖火擊打得你[1;35m烏青瘀血[m.(61-85)
mpechoat $n 你的聖火擊讓冰像開始溶化了
mpechoaround $n $N的聖火擊讓冰像開始溶化了
mpat $b mpforcer here modhmv pc 01.61 85 0
mpmod_qmark self statue_hurt v0 + 3
if is_qmark($i) >= statue_hurt 100015
	mpecho $B從溶化的冰雕中走了出來
	mptransfer $b
	mppurge self
endif
~
>death_prog 100~
mpecho $I被外力擊碎了.
mppurge self
~
|