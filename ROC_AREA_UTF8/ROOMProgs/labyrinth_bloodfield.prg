>room_command hew default~
stun $n 10
if is_equ_obj($n) == 38239
or is_equ_obj($n) == 38240
or is_equ_obj($n) == 38241
	if roomvalue($0) < 100
		if str($n) >= 22
			echoat $n [1;32m你用力地揮舞柴刀，試圖開出一條路。[m
			echoaround $n [1;32m$N用力地揮舞柴刀，砍下許多枝藤。[m
			if rand(33)
				modrvt v0 + 10
			else
				if rand(50)
					modrvt v0 + 11
				else
					modrvt v0 + 12
				endif
			endif
		else
			if str($n) >= 18
				echoat $n [1;32m你用力地揮舞柴刀，試圖開出一條路。[m
				echoaround $n [1;32m$N用力地揮舞柴刀，砍下許多枝藤。[m
				if rand(33)
					modrvt v0 + 7
				else
					if rand(50)
						modrvt v0 + 8
					else
						modrvt v0 + 9
					endif
				endif
			else
				if str($n) >= 12
					echoat $n [1;32m你用力地揮舞柴刀，試圖開出一條路。[m
					echoaround $n [1;32m$N用力地揮舞柴刀，砍下許多枝藤。[m
					if rand(33)
						modrvt v0 + 4
					else
						if rand(50)
							modrvt v0 + 5
						else
							modrvt v0 + 6
						endif
					endif
				else
					echoat $n [1;32m你用力地揮舞柴刀，試圖開出一條路。[m
					echoaround $n [1;32m$N用力地揮舞柴刀，砍下許多枝藤。[m
					if rand(33)
						modrvt v0 + 1
					else
						if rand(50)
							modrvt v0 + 2
						else
							modrvt v0 + 3
						endif
					endif
				endif
			endif
		endif
	else
		modrvt v0 = 0
		rcall makedoor_prog $n
	endif
else
	echoat $n [1;31m你試圖開出一條路，卻被身旁的藤蔓給割傷了....[m
	echoaround $n [1;31m$N試圖開出一條路，但是被刮得滿身是傷....[m
	modhmv $n 03.20 20
endif
~
>room_enter 127 100~
if rand(80)
	echoat $n [1;31m你在狹小的樹叢裡移動，身上多處被割傷。[m
	if con($n) >= 23
		modhmv $n 03.1 3 00000001
	else
		if con($n) >= 20
			modhmv $n 01.50 150 00000001
		else
			if con($n) >= 17
				modhmv $n 01.100 200 00000001
			else
				if con($n) >= 14
					modhmv $n 01.150 250 00000001
				else
					modhmv $n 01.200 300 00000001
				endif
			endif
		endif
	endif
endif
if rand(20)
	if dex($n) >= 21
		echoat $r [1;33m一根突然彈起的藤枝往你這揮過來，但是你靈敏的躲過了。[m
		echoaround $r [1;33m$R被突然彈起的藤枝嚇了一跳。[m
	else
		if dex($n) >= 16
			echoat $r [1;33m一根突然彈起的藤枝往你這揮過來，但是你只被輕輕地劃過。[m
			echoaround $r [1;33m$R被突然彈起的藤枝嚇了一跳。[m
			cast poison $r 100
		else
			echoat $r [1;33m一根突然彈起的藤枝往你這揮過來，在你的皮膚上留下長長的口子。[m
			echoaround $r [1;33m$R被突然彈起的藤枝嚇了一跳。[m
			cast disease $r 100
		endif
	endif
endif
~
>room_defun makedoor_prog~
if hasdoor(0)
	if hasdoor(1)
		if hasdoor(2)
			if hasdoor(3)
				echoat $n [1;32m你用力地揮舞柴刀，砍下許多枝藤。[m
				echoaround $n [1;32m$N用力地揮舞柴刀，砍下許多枝藤。[m
			else
				rcall makedoor_west $n
			endif
		else
			if hasdoor(3)
				rcall makedoor_south $n
			else
				if rand(50)
					rcall makedoor_south $n
				else
					rcall makedoor_west $n
				endif
			endif
		endif
	else
		if hasdoor(2)
			if hasdoor(3)
				rcall makedoor_east $n
			else
				if rand(50)
					rcall makedoor_east $n
				else
					rcall makedoor_west $n
				endif
			endif
		else
			if hasdoor(3)
				if rand(50)
					rcall makedoor_east $n
				else
					rcall makedoor_south $n
				endif
			else
				if rand(33)
					rcall makedoor_east $n
				else
					if rand(50)
						rcall makedoor_south $n
					else
						rcall makedoor_west $n
					endif
				endif
			endif
		endif
	endif
else
	if hasdoor(1)
		if hasdoor(2)
			if hasdoor(3)
				rcall makedoor_north $n
			else
				if rand(50)
					rcall makedoor_north $n
				else
					rcall makedoor_west $n
				endif
			endif
		else
			if hasdoor(3)
				if rand(50)
					rcall makedoor_north $n
				else
					rcall makedoor_south $n
				endif
			else
				if rand(33)
					rcall makedoor_north $n
				else
					if rand(50)
						rcall makedoor_south $n
					else
						rcall makedoor_west $n
					endif
				endif
			endif
		endif
	else
		if hasdoor(2)
			if hasdoor(3)
				if rand(50)
					rcall makedoor_north $n
				else
					rcall makedoor_east $n
				endif
			else
				if rand(33)
					rcall makedoor_north $n
				else
					if rand(50)
						rcall makedoor_east $n
					else
						rcall makedoor_west $n
					endif
				endif
			endif
		else
			if hasdoor(3)
				if rand(33)
					rcall makedoor_north $n
				else
					if rand(50)
						rcall makedoor_east $n
					else
						rcall makedoor_south $n
					endif
				endif
			else
				if rand(25)
					rcall makedoor_north $n
				else
					if rand(33)
						rcall makedoor_east $n
					else
						if rand(50)
							rcall makedoor_south $n
						else
							rcall makedoor_west $n
						endif
					endif
				endif
			endif
		endif
	endif
endif
~
|