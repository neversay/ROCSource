>room_mask scan default~
		echoat $n �޲������׶·t�B���F�A���j���I
~
>room_enter 127 33~
		if isaffected($n) == bless
				echoat $n [0m[30;1m�j�q������N�A���W�����֥˸ѤF�I[0m
				del_affect $n 'bless'
		else
				if isaffected($n) == curse
				else
						echoat $n [0m[30;1m�A�ߤ���M�{�L�@�����w�A�ϩ����Y�تF��b�A����......[0m
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