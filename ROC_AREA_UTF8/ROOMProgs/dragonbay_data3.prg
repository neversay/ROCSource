>room_mask scout south~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout sout~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout sou~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout so~
rcall dragonbay_mask_scout_south_check $n
~
>room_mask scout s~
rcall dragonbay_mask_scout_south_check $n
~
>room_defun dragonbay_mask_scout_south~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
        force $n scout south
        break
endif
if roomvalue($0) & 64
if roomvalue($0) & 128
if roomvalue($0) & 256
        force $n scout south
        break
endif
endif
endif
echoat $n [1;30m南方被一堆煩人的蜘蛛絲給纏住了，使得你無法順利執行偵察的動作。[0m
echoaround $n [1;30m你看到 $N [1;30m似乎無視蜘蛛絲的存在，正試著往南方做偵察的動作。[0m
~
>room_defun dragonbay_mask_south~
if isimmort($n)
or roomvalue($1) & 8
or mobcount(24209) == 0
	if ispc($n)
        	force $n south
        	break
	else
		transfer $n south
		break
	endif
endif
if roomvalue($0) & 64
if roomvalue($0) & 128
if roomvalue($0) & 256
	if ispc($n)
        	force $n south
        	break
	else
		transfer $n south
		break
	endif
endif
endif
endif
if roomvalue($0) !& 64
        rcall dragonbay_south_flaming $n
else
        if roomvalue($0) !& 128
                rcall dragonbay_south_flaming $n
        else
                rcall dragonbay_south_flaming $n
        endif
endif
~
>room_defun dragonbay_south_flaming~
if isaffected($n) !& 134217728
        echoat $n [1;37m你試著往南方走，但是卻被那煩人的蜘蛛絲給纏住而不得動彈。[0m
        echoaround $n [1;37m你看到$N[1;37m試著往南方走，但卻被蜘蛛絲給纏住而不得動彈。[0m
        rcall dragonbay_para_check2 $n
        echo [1;30m而整個蜘蛛網也因此而產生了些許晃動！[0m
        rcall dragonbay_mload $n
else
        echoat $n 你試著往南方走了過去。
        echoaround $n 你看到$N往南方走了過去。
        if mpnum($n) < 20
                echoat $n [1;34m你身上的靈力已經不堪損耗，焰甲也在瞬間消失。[0m
                echoaround $n [1;34m你看到 $N [1;34m才走剛跨出腳步，身上的焰甲就消失了。[0m
                del_affect $n 'flaming armor'
                break
        endif
        if roomvalue($0) !& 64
                if rand(70)
                        echoat $n [1;32m你身上的焰甲將蜘蛛網燒掉了一部份。[0m
                        echoaround $n [1;32m你看到蜘蛛網被 $N [1;32m的焰甲燒掉了一部份。[0m
                        if roomtimer(1) == 0
				if mobcount(24209) > 0
                                	modrvt t1 = 24
				endif
                        endif
                        rcall dragonbay_south_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 64
                else
                        rcall dragonbay_south_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 128
                if rand(70)
                        echoat $n [1;33m你身上的焰甲將蜘蛛網給燒掉了一大半。[0m
                        echoaround $n [1;33m你看到蜘蛛網被 $N [1;33m身上的焰甲給燒了一大半。[0m
                        rcall dragonbay_south_flaming_end $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 128
                else
                        rcall dragonbay_south_miss_message $n
                endif
        break
        endif
        if roomvalue($0) !& 256
                if rand(70)
                        echoat $n [1;36m你成功的將南邊的蜘蛛網給燒個精光！南邊終於可以通行了。[0m
                        echoaround $n [1;36m你看到 $N [1;36m成功地將南方的蜘蛛網清光了，南邊也因此可以通行了。[0m
                        rcall dragonbay_south_flaming_end $n
                        rcall dragonbay_south_linkroom $n
                        rcall dragonbay_oload_skeleton $n
                        modrvt v0 | 256
                else
                        rcall dragonbay_south_miss_message $n
                endif
        endif
endif
~
>room_defun dragonbay_north_linkroom_message~
echo [1;36m你看到原本纏住南邊出口的蜘蛛絲突然全被燒盡，南邊也因此可以通行了！[0m
modrvt v0 | 448
~
>room_defun dragonbay_north_linkroom_message2~
if roomvalue($0) & 448
        echo [1;36m你看到南邊原本已經被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
        modrvt v0 r 448
else
        if roomvalue($0) & 192
                echo [1;36m你看到南邊原本已經快被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 192
        else
                echo [1;36m你看到南邊稍被燒毀的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 64
        endif
endif
~
>room_defun dragonbay_south_flaming_end~
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
>room_defun dragonbay_south_miss_message~
echoat $n [1;34m你試著穿越那令人厭煩的蜘蛛絲，可惜卻被蜘蛛網纏住而失敗了。[0m
echoaround $n [1;34m你看到 $N [1;34m在跨出腳步沒多久之後便被蜘蛛網纏住而止步不前。[0m
rcall dragonbay_para_check $n
rcall dragonbay_south_flaming_end $n
~
|
