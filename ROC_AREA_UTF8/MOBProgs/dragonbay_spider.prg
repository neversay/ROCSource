>death_prog 100~
        mpecho $I [1;32m死後，具有腐蝕性的的體液濺得四處都是。[0m
        mpcall dragonbay_spider_dead_oload $n
        mpcall dragonbay_spider_bonus $n
        mpcall dragonbay_spider_dead
        mpcall dragonbay_spider_dead
        mpcall dragonbay_spider_dead
        mpcall dragonbay_spider_dead
        mpcall dragonbay_spider_dead
        mpcall dragonbay_spider_dead
        mpcall dragonbay_spider_dead
~
>defun_prog dragonbay_spider_dead~
if pcnum($) == 0
        break
endif
mpechoat $r [1;31m你不幸地被那噁心的體液給濺到！！[0m
mpechoaround $r [1;31m你看到[0m $R [1;31m不幸的被那噁心的體液給濺到！！[0m
if rand(33)
        if race($r) != vampire
                mpechoat $r [32m你感到全身都相當的不舒服！[0m
                mpechoaround $r [32m你看到[0m $R [0;32m全身都在顫抖著，似乎相當的不舒服。[0m
                mpforcer here add_affect $r 'disease' 1130 5 1 -2 1099511627776
                mpforcer here add_affect $r 'disease' 1130 5 2 -2 1099511627776
                mpforcer here add_affect $r 'disease' 1130 5 3 -2 1099511627776
                mpforcer here add_affect $r 'disease' 1130 5 4 -2 1099511627776
                mpforcer here add_affect $r 'disease' 1130 5 5 -2 1099511627776
        endif
        if isaffected($r) !& 128
                mpechoat $r 強烈的腐蝕性造成你[1;33m臉孔扭曲，內臟出血[0m.(701-850)
                mpechoaround $r 強烈的腐蝕性造成$R[1;33m臉孔扭曲，內臟出血[0m.
                mpforcer here modhmv $r 01.701 850 0
        else
                mpechoat $r 強烈的腐蝕性造成你[1;32m**** 血流如注 ****[0m.(381-450)
                mpechoaround $r 強烈的腐蝕性造成$R[1;32m**** 血流如注 ****[0m.
                mpforcer here modhmv $r 01.381 450 0
        endif
else
        if rand(50)
                if isaffected($r) !& 1
                        mpechoat $r [1;30m你的雙眼遭受強酸腐蝕而失明了！[0m
                        mpechoaround $r [1;30m你看到[0m $R [1;30m的雙眼遭受到強酸的腐蝕而失明了！[0m
                        mpforcer here add_affect $r 'blindness' 1130 -1 18 -20 1
                endif
                if isaffected($r) !& 128
                        mpechoat $r 強烈的腐蝕性造成你[1;32m猛噴鮮血，齒牙迸裂[0m(521-600)
                        mpechoaround $r 強烈的腐蝕性造成$R[1;32m猛噴鮮血，齒牙迸裂[0m.
                        mpforcer here modhmv $r 01.521 600 0
                else
                        mpechoat $r 強烈的腐蝕性造成你[1;32m*** 尖聲哀叫 ***[0m(231-280)
                        mpechoaround $r 強烈的腐蝕性造成$R[1;32m*** 尖聲哀叫 ***[0m.
                        mpforcer here modhmv $r 01.231 280 0
                endif
        else
                if race($r) != vampire
                        mpechoat $r [1;35m你感到全身發噁！[0m
                        mpechoaround $r [1;35m你看到[0m $R [1;35m嘴角開始發青，看來中毒不淺。[0m
                        mpforcer here add_affect $r 'poison' 1130 25 5 -5 4096
                endif
                if isaffected($r) !& 128
                        mpechoat $r 強烈的腐蝕性造成你[1;32m***** 血流如注 *****[0m.(451-520)
                        mpechoaround $r 強烈的腐蝕性造成$R[1;32m***** 血流如注 *****[0m.
                        mpforcer here modhmv $r 01.451 520 0
                else
                        mpechoat $r 強烈的腐蝕性造成你[1;32m*** 尖聲哀叫 ***[0m.(231-280)
                        mpechoaround $r 強烈的腐蝕性造成$R[1;32m*** 尖聲哀叫 ***[0m.
                        mpforcer here modhmv $r 01.231 280 0
                endif
        endif
endif
~
>hurt_prog 46~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 8~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 58~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 90~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 43~
if hpnum($i) <= 1000
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 45~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 96~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 165~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 204~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 185~
if hpnum($i) <= 1000
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 57~
if hpnum($i) <= 1000
        mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 7~
if hpnum($i) <= 200
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
        mpcall dragonbay_spider_oload $n
endif
~
>hurt_prog 265~
if hpnum($i) <= 500
				mpadd_qmark self dragonbay_spider_deadcheck 1 0 0 0 0 1
				mpcall dragonbay_spider_oload $n
endif
~
>defun_prog dragonbay_spider_oload~
if hpnum($i) <= 100
        mpecho $I[1;30m嚎叫一聲之後，全身焦黑的縮成一團的死了。[0m
        mpoload 24210 40 floor
        mpcall dragonbay_spider_bonus $n
        mppurge self
else
				mpdel_qmark self dragonbay_spider_deadcheck
endif
~
>defun_prog dragonbay_spider_bonus~
if pcnum($) > 20
        mpgreward exp $n 30 500
else
        if pcnum($) > 15
                mpgreward exp $n 30 600
        else
                if pcnum($) > 10
                        mpgreward exp $n 30 700
                else
                        if pcnum($) > 5
                                mpgreward exp $n 30 800
                        else
                                mpgreward exp $n 30 1000
                        endif
                endif
        endif
endif
~
|
