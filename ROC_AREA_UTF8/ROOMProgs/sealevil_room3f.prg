>room_enter 127 60~
if ispc($n)
        if rand(33)
                echoat $n 一隻怪蟲忽然自血湖中躍出，咬了你一口後又掉進湖中。
                echoat $n 你覺得被咬到的地方有點酸麻，身體好像也不太舒服。
                add_affect $n 'poison' 1140 10 19 -25 4096
        else
                if rand(50)
                        echoat $n 你身旁忽然噴出一股暗紅色的霧氣！
                        echoat $n 才吸進一點，你就覺得很難受，甚至想吐。
                        add_affect $n 'disease' 1140 10 18 -25 1099511627776
                else
                        echoat $n 你忽然感到身體某處傳來一陣麻痛感，好像被什麼東西刺到。
			if isaffected($n) / adrenaline control
				echoat $n [1;36m你體內的腎上腺素在瞬間激發而出，使你感到全身相當順暢。[0m
				if rand(15)
					echoat $n [1;37m但也在此時，你身上的腎腺術控制術似乎也消失了。[0m
					del_affect $n 'adrenaline control'
				endif
			else
                        	echoat $n 瞬間，麻痺感擴張到了全身，糟糕了......
                        	add_affect $n 'paralyze' 0 5 39 200 268435456
			endif
                endif
        endif
endif
~
>room_defun sealevil_room_3f~
if ispc($r)
        if rand(33)
                echoat $r 一隻怪蟲忽然自血湖中躍出，咬了你一口後又掉進湖中。
                echoat $r 你覺得被咬到的地方有點酸麻，身體好像也不太舒服。
                add_affect $r 'poison' 1140 10 19 -25 4096
        else
                if rand(50)
                        echoat $r 你身旁忽然噴出一股暗紅色的霧氣！
                        echoat $r 才吸進一點，你就覺得很難受，甚至想吐。
                        add_affect $r 'disease' 1140 10 18 -25 1099511627776
                else
                        echoat $r 你忽然感到身體某處傳來一陣麻痛感，好像被什麼東西刺到。
			if isaffected($r) / adrenaline control
				echoat $r [1;36m你體內的腎上腺素在瞬間激發而出，使你感到全身相當順暢。[0m
				if rand(15)
					echoat $r [1;37m但也在此時，你身上的腎腺術控制術似乎也消失了。[0m
					del_affect $r 'adrenaline control'
				endif
			else
                        	echoat $r 瞬間，麻痺感擴張到了全身，糟糕了......
                        	add_affect $r 'paralyze' 0 5 39 200 268435456
			endif
                endif
        endif
endif
~
|