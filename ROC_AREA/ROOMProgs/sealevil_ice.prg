>room_enter 127 33~
if ispc($n)
	if isaffected($n) / prot. from cold
		echoat $n 一道寒風吹起，身上的寒冰防壁瞬間阻止了寒氣對你的侵襲。
	else
        	if rand(33)
                	echoat $n 原本平靜的洞窟冷不防長出多支冰柱，劃過你的身體！
                	echoat $n 雖然你沒受到什麼傷，但是你卻覺得身體有點寒冷。
                	add_affect $n 'glacier strike' 1140 10 01 -6 1073741824
        	else
                	if rand(50)
                        	echoat $n 自四面八方湧來的寒氣令你全身發冷！
                        	echoat $n 不知道為什麼，寒氣之中似乎夾雜著陰寒的魔氣......
                        	add_affect $n 'chill touch' 1140 10 43 -50 1073741824
                	else
                        	echoat $n 你覺得越來越想睡，眼皮也越來越重。
                        	echoat $n 很快地，你的理智已經被慾望擊倒，晚安了...ZZZZzzzz......
                        	add_affect $n 'sleep' 0 5 39 250 131072
                        	force $n sleep
			endif
                endif
        endif
endif
~
>room_defun sealevil_room_ice~
if ispc($r)
	if isaffected($r) / prot. from cold
		echoat $r 一道寒風吹起，身上的寒冰防壁瞬間阻止了寒氣對你的侵襲。
	else
        	if rand(33)
                	echoat $r 原本平靜的洞窟冷不防長出多支冰柱，劃過你的身體！
                	echoat $r 雖然你沒受到什麼傷，但是你卻覺得身體有點寒冷。
                	add_affect $r 'glacier strike' 1140 10 01 -6 1073741824
        	else
                	if rand(50)
                        	echoat $r 自四面八方湧來的寒氣令你全身發冷！
                        	echoat $r 不知道為什麼，寒氣之中似乎夾雜著陰寒的魔氣......
                        	add_affect $r 'chill touch' 1140 10 43 -50 1073741824
                	else
                        	echoat $r 你覺得越來越想睡，眼皮也越來越重。
                        	echoat $r 很快地，你的理智已經被慾望擊倒，晚安了...ZZZZzzzz......
                        	add_affect $r 'sleep' 0 5 39 250 131072
                        	force $r sleep
			endif
                endif
        endif
endif
~
|