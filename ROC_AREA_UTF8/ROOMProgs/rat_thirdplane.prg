>room_mask scan default~
		echoat $n 詭異的極度黑暗遮蔽了你的搜索！
~
>room_enter 127 33~
		if isaffected($n) == bless
				echoat $n [0m[30;1m大量的邪氣將你身上的祝福瓦解了！[0m
				del_affect $n 'bless'
		else
				if isaffected($n) == curse
				else
						echoat $n [0m[30;1m你心中突然閃過一絲不安，彷彿有某種東西在你身旁......[0m
				endif
				del_affect $n 'curse'
				add_affect $n 'curse' 1130 3 1 -3 0
				add_affect $n 'curse' 1130 3 2 -3 0
				add_affect $n 'curse' 1130 3 3 -3 0
				add_affect $n 'curse' 1130 3 4 -3 0
				add_affect $n 'curse' 1130 3 5 -3 0
				add_affect $n 'curse' 1130 3 35 10 0
				add_affect $n 'curse' 1130 3 24 10 0
		endif
~
|