>room_mask scout east~
rcall dragonbay_mask_scout_east_check $n
~
>room_mask scout eas~
rcall dragonbay_mask_scout_east_check $n
~
>room_mask scout ea~
rcall dragonbay_mask_scout_east_check $n
~
>room_mask scout e~
rcall dragonbay_mask_scout_east_check $n
~
>room_defun dragonbay_mask_scout_east~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
        force $n scout east
        break
endif
if roomvalue($0) & 8
if roomvalue($0) & 16
if roomvalue($0) & 32
        force $n scout east
        break
endif
endif
endif
echoat $n [1;30m東方被一堆煩人的蜘蛛絲給纏住了，使得你無法順利執行偵察的動作。[0m
echoaround $n [1;30m你看到 $N [1;30m似乎無視蜘蛛絲的存在，正試著往東方做偵察的動作。[0m
~
>room_defun dragonbay_mask_east~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
	if ispc($n)
        	force $n east
        	break
	else
		transfer $n east
		break
	endif
endif
if roomvalue($0) & 8
if roomvalue($0) & 16
if roomvalue($0) & 32
	if ispc($n)
        	force $n east
        	break
	else
		transfer $n east
		break
	endif
endif
endif
endif
if roomvalue($0) !& 8
        rcall dragonbay_east_flaming $n
else
        if roomvalue($0) !& 16
                rcall dragonbay_east_flaming $n
        else
                rcall dragonbay_east_flaming $n
        endif
endif
~
>room_defun dragonbay_east_flaming~
if isaffected($n) !& 134217728
        echoat $n [1;37m你試著往東方走，但是卻被那煩人的蜘蛛絲給纏住而不得動彈。[0m
        echoaround $n [1;37m你看到$N[1;37m試著往東方走，但卻被蜘蛛絲給纏住而不得動彈。[0m
        rcall dragonbay_para_check2 $n
        echo [1;30m而整個蜘蛛網也因此而產生了些許晃動！[0m
        rcall dragonbay_mload $n
else
        echoat $n 你試著往東方走了過去。
        echoaround $n 你看到$N往東方走了過去。
        if mpnum($n) < 20
                echoat $n [1;34m你身上的靈力已經不堪損耗，焰甲也在瞬間消失。[0m
                echoaround $n [1;34m你看到 $N [1;34m才走剛跨出腳步，身上的焰甲就消失了。[0m
                del_affect $n 'flaming armor'
                break
        endif
        if roomvalue($0) !& 8
                if rand(70)
                        echoat $n [1;32m你身上的焰甲將蜘蛛網燒掉了一部份。[0m
                        echoaround $n [1;32m你看到蜘蛛網被 $N [1;32m的焰甲燒掉了一部份。[0m
                        if roomtimer(1) == 0
				if mobcount(24209) > 0
                                	modrvt t1 = 24
				endif
                        endif
                        rcall dragonbay_east_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 8
                else
                        rcall dragonbay_east_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 16
                if rand(70)
                        echoat $n [1;33m你身上的焰甲將蜘蛛網給燒掉了一大半。[0m
                        echoaround $n [1;33m你看到蜘蛛網被 $N [1;33m身上的焰甲給燒了一大半。[0m
                        rcall dragonbay_east_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 16
                else
                        rcall dragonbay_east_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 32
                if rand(70)
                        echoat $n [1;36m你成功的將東邊的蜘蛛網給燒個精光！東邊終於可以通行了。[0m
                        echoaround $n [1;36m你看到 $N [1;36m成功地將東方的蜘蛛網清光了，東邊也因此可以通行了。[0m
                        rcall dragonbay_east_flaming_end $n
                        rcall dragonbay_east_linkroom $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 32
                else
                        rcall dragonbay_east_miss_message $n
                endif
        endif
endif
~
>room_defun dragonbay_west_linkroom_message~
echo [1;36m你看到原本纏住東邊出口的蜘蛛絲突然全被燒盡，東邊也因此可以通行了！[0m
modrvt v0 | 56
~
>room_defun dragonbay_west_linkroom_message2~
if roomvalue($0) & 56
        echo [1;36m你看到東邊原本已經被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
        modrvt v0 r 56
else
        if roomvalue($0) & 24
                echo [1;36m你看到東邊原本已經快被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 24
        else
                echo [1;36m你看到東邊稍被燒毀的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 8
        endif
endif
~
>room_defun dragonbay_east_flaming_end~
echoat $n [1;35m你感到自己的靈力在瞬間消耗了不少。[0m
modhmv $n 11.20 20 0
echo [1;30m周遭的蜘蛛絲也因此而產生了晃動。[0m
rcall dragonbay_mload $n
if rand(15)
        echoat $n [1;31m你身上的焰甲卻也在此時消失了。[0m
        echoaround $n [1;31m你看到 $N [1;31m的焰甲也在同時間消失了。[0m
        del_affect $n 'flaming armor'
endif
~
>room_defun dragonbay_east_miss_message~
echoat $n [1;34m你試著穿越那令人厭煩的蜘蛛絲，可惜卻被蜘蛛網纏住而失敗了。[0m
echoaround $n [1;34m你看到 $N [1;34m在跨出腳步沒多久之後便被蜘蛛網纏住而止步不前。[0m
rcall dragonbay_para_check $n
rcall dragonbay_east_flaming_end $n
~
|
