>room_defun dragonbay_timer~
if roomvalue($1) & 8
	break
endif
if mobcount(24209) == 0
	modrvt t0 = 0
	modrvt t1 = 0
	break
endif
if pcnum($) > 0
	if roomtimer($0) == 3
        	echo [1;37m你感覺到四周的雜聲似乎越來越多，也越來越大聲。[0m
	endif
	if roomtimer($0) == 2
        	echo [1;32m你看到一大堆魔眼蜘蛛正從四面八方聚集而來，使得你不禁冷汗直流。[0m
        	echo [1;32m心頭也開始打定主意，就算是拼了老命也要趕緊離開這個地方！[0m
	endif
	if roomtimer($0) == 1
        	echo [1;36m在你還慌恐的想拔腿就跑時，一大堆魔眼蜘蛛已經朝你迎面而來了！[0m
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
       		rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	rcall dragonbay_mload_fun $r
        	modrvt t0 = 0
	endif
else
	if roomtimer($0) > 60
		modrvt t0 = 1000
		modrvt v1 | 8
	else
		modrvt t0 + 2
	endif
endif
if roomvalue($0) == 0
        break
else
        if roomtimer(1) == 2
                echo [1;35m你看到一些魔眼蜘蛛正開始在吐著蜘蛛絲試著修補被燒毀的蜘蛛網。[0m
                echo [1;32m而周遭的樹枝也因此而產生了些許的晃動。[0m
		if pcnum($) > 0
                	rcall dragonbay_mload $r
                	rcall dragonbay_mload $r
		endif
        endif
        if roomtimer(1) == 1
                echo [1;35m你看到更多的魔眼蜘蛛開始聚集在你的上頭吐著白色的蜘蛛絲，[0m
                echo [1;35m不一會兒功夫已經破損的蜘蛛網就被修補好了。[0m
                echo [1;32m承受不了那麼多魔眼蜘蛛的樹枝也因此而產生了更大的晃動！[0m
		if pcnum($) > 0
                	rcall dragonbay_mload2 $r
                	rcall dragonbay_mload2 $r
                	rcall dragonbay_mload2 $r
		endif
                if roomvalue($0) & 1
                        rcall dragonbay_exit_fix_north
                        rcall dragonbay_north_linkroom2
                endif
                if roomvalue($0) & 8
                        rcall dragonbay_exit_fix_east
                        rcall dragonbay_east_linkroom2
                endif
                if roomvalue($0) & 64
                        rcall dragonbay_exit_fix_south
                        rcall dragonbay_south_linkroom2
                endif
                if roomvalue($0) & 512
                        rcall dragonbay_exit_fix_west
                        rcall dragonbay_west_linkroom2
                endif
        endif
endif
~
>room_defun dragonbay_exit_fix_north~
if roomvalue($0) & 7
        echo [1;32m你看到北邊原本已經被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
        modrvt v0 r 7
else
        if roomvalue($0) & 3
                echo [1;32m你看到北邊原本已經快被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 3
        else
                echo [1;32m你看到北邊稍被燒毀的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 1
        endif
endif
~
>room_defun dragonbay_exit_fix_east~
if roomvalue($0) & 56
        echo [1;32m你看到東邊原本已經被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
        modrvt v0 r 56
else
        if roomvalue($0) & 24
                echo [1;32m你看到東邊原本已經快被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 24
        else
                echo [1;32m你看到東邊稍被燒毀的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 8
        endif
endif
~
>room_defun dragonbay_exit_fix_south~
if roomvalue($0) & 448
        echo [1;32m你看到南邊原本已經被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
        modrvt v0 r 448
else
        if roomvalue($0) & 192
                echo [1;32m你看到南邊原本已經快被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 192
        else
                echo [1;32m你看到南邊稍被燒毀的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 64
        endif
endif
~
>room_defun dragonbay_exit_fix_west~
if roomvalue($0) & 3584
        echo [1;32m你看到西邊原本已經被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
        modrvt v0 r 3584
else
        if roomvalue($0) & 1536
                echo [1;32m你看到西邊原本已經快被燒盡的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 1536
        else
                echo [1;32m你看到西邊稍被燒毀的蜘蛛絲又被一群魔眼蜘蛛給修補好了。[0m
                modrvt v0 r 512
        endif
endif
~
>room_defun dragonbay_oload_skeleton~
if inroom($n) > 24211
if inroom($n) < 24269
	if rand(20)
		echoat $n 在你正打算停下停步歇息時，突然聽到咚～的一聲。
		echoat $n 在蜘蛛網被燒掉之後，原本被夾雜其中的骸骨跟雜物隨即掉落在地上。
		echoaround $n 你看到那原本被糾結在蜘蛛網之中的骸骨跟雜物一同掉落在地上。
		oload 24208 36 floor
		if rand(10)
			oload 14218 36 floor
		else
			if rand(11)
				oload 14219 36 floor
			else
				if rand(13)
					oload 14220 36 floor
				else
					if rand(14)
						oload 14221 36 floor
					else
						if rand(16)
							oload 14222 36 floor
						else
							if rand(20)
								oload 14223 36 floor
							else
								if rand(25)
									oload 14224 36 floor
								else
									if rand(34)
										oload 14225 36 floor
									else
										if rand(50)
											oload 14226 36 floor
										else
											oload 14227 36 floor
										endif
									endif
								endif
							endif
						endif
					endif
				endif
			endif
		endif
		if rand(20)
			if rand(10)
				oload 14218 36 floor
			else
				if rand(11)
					oload 14219 36 floor
				else
					if rand(13)
						oload 14220 36 floor
					else
						if rand(14)
							oload 14221 36 floor
						else
							if rand(16)
								oload 14222 36 floor
							else
								if rand(20)
									oload 14223 36 floor
								else
									if rand(25)
										oload 14224 36 floor
									else
										if rand(34)
											oload 14225 36 floor
										else
											if rand(50)
												oload 14226 36 floor
											else
												oload 14227 36 floor
											endif
										endif
									endif
								endif
							endif
						endif
					endif
				endif
			endif
		endif
		if rand(5)
			oload 14228 36 floor
		endif
		if rand(10)
			if rand(20)
				oload 14229 36 floor
			else
				if rand(25)
					oload 14230 36 floor
				else
					if rand(34)
						oload 14231 36 floor
					else
						if rand(50)
							oload 14232 36 floor
						else
							oload 14233 36 floor
						endif
					endif
				endif
			endif
		endif
	endif
endif
endif
~
>room_defun dragonbay_para_check~
if isaffected($n) / adrenaline control
        if rand(10)
                echoat $n [1;36m你感到全身都麻痺了，你體內的腎上腺素似乎沒有發揮出作用。[0m
                add_affect $n 'paralyze' 1130 3 39 300 268435456
        else
                echoat $n [1;36m你體內的腎上腺素在瞬間激發而出，使你感到全身相當順暢。[0m
                if rand(15)
                        echoat $n [1;37m但也在此時，你身上的腎腺術控制術似乎也消失了。[0m
                        del_affect $n 'adrenaline control'
                endif
        endif
else
        echoat $n [1;36m在接觸到蜘蛛絲後沒多久，你便感覺全身似乎都麻痺了。[0m
        add_affect $n 'paralyze' 1130 5 39 300 268435456
endif
~
>room_defun dragonbay_para_check2~
if isaffected($n) / adrenaline control
        if rand(60)
                echoat $n [1;36m你感到全身都麻痺了，你體內的腎上腺素似乎沒有發揮出作用。[0m
                add_affect $n 'paralyze' 1130 5 39 300 268435456
        else
                echoat $n [1;36m你體內的腎上腺素在瞬間激發而出，使你感到全身相當順暢。[0m
                if rand(30)
                        echoat $n [1;37m但也在此時，你身上的腎腺術控制術似乎也消失了。[0m
                        del_affect $n 'adrenaline control'
                endif
        endif
else
        echoat $n [1;36m在接觸到蜘蛛絲後沒多久，你便感覺全身似乎都麻痺了。[0m
        add_affect $n 'paralyze' 1130 8 39 300 268435456
endif
~
>room_enter 15 100~
if isimmort($n)
or mobcount(24209) == 0
or roomvalue($1) & 8
or roomvalue($1) & 64
        break
endif
if roomtimer($0) == 0
        modrvt t0 = 36
endif
if inroom($n) > 24210
if inroom($n) < 24269
        if mobcount(24210) == 0
                if rand(10)
                        callout 2 at 24200 mload 24210
                endif
        endif
endif
endif
modrvt v1 | 64
callout 4 modrvt v1 r 64
~
>room_mask exits default~
echoat $n 四周全都是雪白的蜘蛛網，你實在是無法分辦出各方向有什麼東西。
~
>room_mask cast calm~
if rmobcount(24201) > 0
or rmobcount(24202) > 0
or rmobcount(24203) > 0
or rmobcount(24204) > 0
or rmobcount(24205) > 0
or rmobcount(24206) > 0
or rmobcount(24207) > 0
or rmobcount(24208) > 0
	echoat $n 周遭的人已經被蜘蛛嚇得失去理智了，使得你無法讓他們鎮靜下來。
else
	force $n cast calm
endif
~
>room_defun dragonbay_rand_message~
if rand(30)
	if mobcount(24209) > 0
		echoat $r 你聽到枝頭上傳來一陣吱吱喳喳的聲音。
		break
	endif
else
	if rand(30)
		echoat $r 一陣寒風從你身旁刮過，讓你微微打了個冷顫。
		break
	else
		if rand(40)
			if mobcount(24209) > 0
				echoat $r 在前上方的枝葉上彷彿閃過了一個影子。
				break
			endif
		else
			if rand(70)
				echo 突然刮起了一陣大風，上頭的枝葉也因此產生了些許的晃動。
				rcall dragonbay_mload2 $r
				rcall dragonbay_mload2 $r
			else
				echo 突然刮起了一陣強風，上頭的枝葉也因此產生了劇烈的晃動。
				rcall dragonbay_mload $r
				rcall dragonbay_mload $r
				rcall dragonbay_mload $r
				rcall dragonbay_mload $r
			endif
		endif
	endif
endif
~
>room_defun dragonbay_mload~
if mobcount(24209) != 0
        if rand(25)
                rcall dragonbay_mload_fun
        endif
endif
~
>room_defun dragonbay_mload2~
if mobcount(24209) != 0
	if rand(40)
        	rcall dragonbay_mload_fun
	endif
endif
~
>room_defun dragonbay_mload_fun~
if rand(25)
        if rand(50)
                echo [1;31m你看到天上掉下了一隻魔眼蜘蛛！[0m
                mload 24201
        else
                echo [1;31m你看到天上掉下了一隻[32m魔眼蜘蛛！[0m
                mload 24202
        endif
else
        if rand(33)
                if rand(50)
                	echo [1;31m你看到天上掉下了一隻[33m魔眼蜘蛛！[0m
                        mload 24203
                else
                	echo [1;31m你看到天上掉下了一隻[34m魔眼蜘蛛！[0m
                        mload 24204
                endif
        else
                if rand(50)
                        if rand(50)
                		echo [1;31m你看到天上掉下了一隻[35m魔眼蜘蛛！[0m
                                mload 24205
                        else
                		echo [1;31m你看到天上掉下了一隻[36m魔眼蜘蛛！[0m
                                mload 24206
                        endif
                else
                        if rand(50)
                		echo [1;31m你看到天上掉下了一隻[37m魔眼蜘蛛！[0m
                                mload 24207
                        else
                		echo [1;31m你看到天上掉下了一隻[30m魔眼蜘蛛！[0m
                                mload 24208
                        endif
                endif
        endif
endif
~
|
