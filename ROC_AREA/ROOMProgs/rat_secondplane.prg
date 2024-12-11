>room_mask east default~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 33 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你試圖在混亂的空間裡找出一條可通往東邊的通道...
		echoaround $n $N突然消失了！
		if is_qmark($n) > wererat_plane 100040
				if is_qmark($n) > wererat_plane 100044
						if is_qmark($n) > wererat_plane 100046
								if is_qmark($n) == wererat_plane 100048
										transfer $n 545
								else
										transfer $n 548
								endif
						else
								if is_qmark($n) == wererat_plane 100046
										transfer $n 547
								else
										transfer $n 546
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100042
								if is_qmark($n) == wererat_plane 100044
										transfer $n 541
								else
										transfer $n 544
								endif
						else
								if is_qmark($n) == wererat_plane 100042
										transfer $n 543
								else
										transfer $n 542
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100036
						if is_qmark($n) > wererat_plane 100038
								if is_qmark($n) == wererat_plane 100040
										transfer $n 537
								else
										transfer $n 540
								endif
						else
								if is_qmark($n) == wererat_plane 100038
										transfer $n 539
								else
										transfer $n 538
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100034
								if is_qmark($n) == wererat_plane 100036
										transfer $n 533
								else
										transfer $n 536
								endif
						else
								if is_qmark($n) == wererat_plane 100034
										transfer $n 535
								else
										transfer $n 534
								endif
						endif
				endif
		endif
		endif
~
>room_mask west default~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 33 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你試圖在混亂的空間裡找出一條可通往西邊的通道...
		echoaround $n $N突然消失了！
		if is_qmark($n) > wererat_plane 100040
				if is_qmark($n) > wererat_plane 100044
						if is_qmark($n) > wererat_plane 100046
								if is_qmark($n) == wererat_plane 100048
										transfer $n 547
								else
										transfer $n 546
								endif
						else
								if is_qmark($n) == wererat_plane 100046
										transfer $n 545
								else
										transfer $n 548
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100042
								if is_qmark($n) == wererat_plane 100044
										transfer $n 543
								else
										transfer $n 542
								endif
						else
								if is_qmark($n) == wererat_plane 100042
										transfer $n 541
								else
										transfer $n 544
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100036
						if is_qmark($n) > wererat_plane 100038
								if is_qmark($n) == wererat_plane 100040
										transfer $n 539
								else
										transfer $n 548
								endif
						else
								if is_qmark($n) == wererat_plane 100038
										transfer $n 537
								else
										transfer $n 540
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100034
								if is_qmark($n) == wererat_plane 100036
										transfer $n 535
								else
										transfer $n 534
								endif
						else
								if is_qmark($n) == wererat_plane 100034
										transfer $n 533
								else
										transfer $n 536
								endif
						endif
				endif
		endif
		endif
~
>room_mask north default~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 33 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你試圖在混亂的空間裡找出一條可通往北邊的通道...
		echoaround $n $N突然消失了！
		if is_qmark($n) > wererat_plane 100040
				if is_qmark($n) > wererat_plane 100044
						if is_qmark($n) > wererat_plane 100046
								if is_qmark($n) == wererat_plane 100048
										transfer $n 544
								else
										transfer $n 543
								endif
						else
								if is_qmark($n) == wererat_plane 100046
										transfer $n 542
								else
										transfer $n 541
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100042
								if is_qmark($n) == wererat_plane 100044
										transfer $n 540
								else
										transfer $n 539
								endif
						else
								if is_qmark($n) == wererat_plane 100042
										transfer $n 538
								else
										transfer $n 537
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100036
						if is_qmark($n) > wererat_plane 100038
								if is_qmark($n) == wererat_plane 100040
										transfer $n 536
								else
										transfer $n 535
								endif
						else
								if is_qmark($n) == wererat_plane 100038
										transfer $n 534
								else
										transfer $n 533
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100034
								if is_qmark($n) == wererat_plane 100036
										transfer $n 548
								else
										transfer $n 547
								endif
						else
								if is_qmark($n) == wererat_plane 100034
										transfer $n 546
								else
										transfer $n 545
								endif
						endif
				endif
		endif
		endif
~
>room_mask south default~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 33 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你試圖在混亂的空間裡找出一條可通往南邊的通道...
		echoaround $n $N突然消失了！
		if is_qmark($n) > wererat_plane 100040
				if is_qmark($n) > wererat_plane 100044
						if is_qmark($n) > wererat_plane 100046
								if is_qmark($n) == wererat_plane 100048
										transfer $n 536
								else
										transfer $n 535
								endif
						else
								if is_qmark($n) == wererat_plane 100046
										transfer $n 534
								else
										transfer $n 533
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100042
								if is_qmark($n) == wererat_plane 100044
										transfer $n 548
								else
										transfer $n 547
								endif
						else
								if is_qmark($n) == wererat_plane 100042
										transfer $n 546
								else
										transfer $n 545
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100036
						if is_qmark($n) > wererat_plane 100038
								if is_qmark($n) == wererat_plane 100040
										transfer $n 544
								else
										transfer $n 543
								endif
						else
								if is_qmark($n) == wererat_plane 100038
										transfer $n 542
								else
										transfer $n 541
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100034
								if is_qmark($n) == wererat_plane 100036
										transfer $n 540
								else
										transfer $n 539
								endif
						else
								if is_qmark($n) == wererat_plane 100034
										transfer $n 538
								else
										transfer $n 537
								endif
						endif
				endif
		endif
		endif
~
|