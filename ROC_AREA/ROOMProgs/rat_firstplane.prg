>room_defun randomtwo~
		if rand(50)
				if rand(50)
						if rand(50)
								if rand(50)
										modrvt v0 = 1
								else
										modrvt v0 = 2
								endif
						else
								if rand(50)
										modrvt v0 = 3
								else
										modrvt v0 = 4
								endif
						endif
				else
						if rand(50)
								if rand(50)
										modrvt v0 = 5
								else
										modrvt v0 = 6
								endif
						else
								if rand(50)
										modrvt v0 = 7
								else
										modrvt v0 = 8
								endif
						endif
				endif
		else
				if rand(50)
						if rand(50)
								if rand(50)
										modrvt v0 = 9
								else
										modrvt v0 = 10
								endif
						else
								if rand(50)
										modrvt v0 = 11
								else
										modrvt v0 = 12
								endif
						endif
				else
						if rand(50)
								if rand(50)
										modrvt v0 = 13
								else
										modrvt v0 = 14
								endif
						else
								modrvt v0 = 15
						endif
				endif
		endif
~
>room_defun radomfour~
		echo 周圍的光線不停旋轉變化著...
		modrvt v1 = 0
		if rand(50)
				if rand(50)
						if rand(33)
								modrvt v1 + 4
								if rand (50)
										modrvt v1 + 32
										modrvt v1 + 192
								else
										modrvt v1 + 48
										modrvt v1 + 128
								endif
						else
								if rand(50)
										modrvt v1 + 8
										if rand(50)
												modrvt v1 + 16
												modrvt v1 + 192
										else
												modrvt v1 + 48
												modrvt v1 + 64
										endif
								else
										modrvt v1 + 12
										if rand(50)
												modrvt v1 + 16
												modrvt v1 + 128
										else
												modrvt v1 + 32
												modrvt v1 + 64
										endif
								endif
						endif
				else
						modrvt v1 + 1
						if rand(33)
								modrvt v1 + 0
								if rand (50)
										modrvt v1 + 32
										modrvt v1 + 192
								else
										modrvt v1 + 48
										modrvt v1 + 128
								endif
						else
								if rand(50)
										modrvt v1 + 8
										if rand(50)
												modrvt v1 + 0
												modrvt v1 + 192
										else
												modrvt v1 + 48
												modrvt v1 + 0
										endif
								else
										modrvt v1 + 12
										if rand(50)
												modrvt v1 + 32
												modrvt v1 + 0
										else
												modrvt v1 + 0
												modrvt v1 + 128
										endif
								endif
						endif
				endif
		else
				if rand(50)
						modrvt v1 + 2
						if rand(33)
								modrvt v1 + 0
								if rand (50)
										modrvt v1 + 16
										modrvt v1 + 192
								else
										modrvt v1 + 48
										modrvt v1 + 64
								endif
						else
								if rand(50)
										modrvt v1 + 4
										if rand(50)
												modrvt v1 + 48
												modrvt v1 + 0
										else
												modrvt v1 + 0
												modrvt v1 + 192
										endif
								else
										modrvt v1 + 12
										if rand(50)
												modrvt v1 + 16
												modrvt v1 + 0
										else
												modrvt v1 + 0
												modrvt v1 + 64
										endif
								endif
						endif
				else
						modrvt v1 + 3
						if rand(33)
								modrvt v1 + 0
								if rand (50)
										modrvt v1 + 16
										modrvt v1 + 128
								else
										modrvt v1 + 32
										modrvt v1 + 64
								endif
						else
								if rand(50)
										modrvt v1 + 4
										if rand(50)
												modrvt v1 + 0
												modrvt v1 + 128
										else
												modrvt v1 + 32
												modrvt v1 + 0
										endif
								else
										modrvt v1 + 8
										if rand(50)
												modrvt v1 + 0
												modrvt v1 + 64
										else
												modrvt v1 + 16
												modrvt v1 + 0
										endif
								endif
						endif
				endif
		endif
~
>room_defun movedark~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 13 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你滑動著雙手，往深邃無邊的黑暗游去...
		echoaround $n $N滑動著雙手，往深邃無邊的黑暗游去...
		if rand(33)
				callout 2 transfer $n 530
		else
				if rand(50)
						callout 2 transfer $n 531
				else
						callout 2 transfer $n 532
				endif
		endif
		endif
~
>room_defun movered~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 13 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你滑動著雙手，往血似腥紅的暗芒游去...
		echoaround $n $N滑動著雙手，往血似腥紅的暗芒游去...
		if is_qmark($n) > wererat_plane 100016
				if is_qmark($n) > wererat_plane 100024
						if is_qmark($n) > wererat_plane 100028
								if is_qmark($n) == wererat_plane 100029
										callout 2 transfer $n 511
								else
										callout 2 transfer $n 526
								endif
						else
								if is_qmark($n) > wererat_plane 100026
										if is_qmark($n) == wererat_plane 100028
												callout 2 transfer $n 529
										else
												callout 2 transfer $n 529
										endif
								else
										if is_qmark($n) == wererat_plane 100026
												callout 2 transfer $n 500
										else
												callout 2 transfer $n 521
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100020
								if is_qmark($n) > wererat_plane 100022
										if is_qmark($n) == wererat_plane 100024
												callout 2 transfer $n 525
										else
												callout 2 transfer $n 524
										endif
								else
										if is_qmark($n) == wererat_plane 100022
												callout 2 transfer $n 523
										else
												callout 2 transfer $n 522
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100018
										if is_qmark($n) == wererat_plane 100020
												callout 2 transfer $n 516
										else
												callout 2 transfer $n 520
										endif
								else
										if is_qmark($n) == wererat_plane 100018
												callout 2 transfer $n 519
										else
												callout 2 transfer $n 518
										endif
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100008
						if is_qmark($n) > wererat_plane 100012
								if is_qmark($n) > wererat_plane 100014
										if is_qmark($n) == wererat_plane 100016
												callout 2 transfer $n 517
										else
												callout 2 transfer $n 526
										endif
								else
										if is_qmark($n) == wererat_plane 100014
												callout 2 transfer $n 515
										else
												callout 2 transfer $n 514
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100010
										if is_qmark($n) == wererat_plane 100012
												callout 2 transfer $n 513
										else
												callout 2 transfer $n 512
										endif
								else
										if is_qmark($n) == wererat_plane 100010
												callout 2 transfer $n 506
										else
												callout 2 transfer $n 510
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100004
								if is_qmark($n) > wererat_plane 100006
										if is_qmark($n) == wererat_plane 100008
												callout 2 transfer $n 509
										else
												callout 2 transfer $n 508
										endif
								else
										if is_qmark($n) == wererat_plane 100006
												callout 2 transfer $n 507
										else
												callout 2 transfer $n 501
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100002
										if is_qmark($n) == wererat_plane 100004
												callout 2 transfer $n 505
										else
												callout 2 transfer $n 504
										endif
								else
										if is_qmark($n) > wererat_plane 100001
												callout 2 transfer $n 503
										else
												if is_qmark($n) == wererat_plane 100001
														callout 2 transfer $n 502
												else
														callout 2 transfer $n 529
												endif
										endif
								endif
						endif
				endif
		endif
		endif
~
>room_defun moveblue~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 13 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你滑動著雙手，往一望無際的深藍游去...
		echoaround $n $N滑動著雙手，往一望無際的深藍游去...
		if is_qmark($n) > wererat_plane 100016
				if is_qmark($n) > wererat_plane 100024
						if is_qmark($n) > wererat_plane 100028
								if is_qmark($n) == wererat_plane 100029
										callout 2 transfer $n 527
								else
										callout 2 transfer $n 528
								endif
						else
								if is_qmark($n) > wererat_plane 100026
										if is_qmark($n) == wererat_plane 100028
												callout 2 transfer $n 500
										else
												callout 2 transfer $n 503
										endif
								else
										if is_qmark($n) == wererat_plane 100026
												callout 2 transfer $n 527
										else
												callout 2 transfer $n 505
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100020
								if is_qmark($n) > wererat_plane 100022
										if is_qmark($n) == wererat_plane 100024
												callout 2 transfer $n 504
										else
												callout 2 transfer $n 528
										endif
								else
										if is_qmark($n) == wererat_plane 100022
												callout 2 transfer $n 502
										else
												callout 2 transfer $n 501
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100018
										if is_qmark($n) == wererat_plane 100020
												callout 2 transfer $n 525
										else
												callout 2 transfer $n 524
										endif
								else
										if is_qmark($n) == wererat_plane 100018
												callout 2 transfer $n 523
										else
												callout 2 transfer $n 522
										endif
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100008
						if is_qmark($n) > wererat_plane 100012
								if is_qmark($n) > wererat_plane 100014
										if is_qmark($n) == wererat_plane 100016
												callout 2 transfer $n 521
										else
												callout 2 transfer $n 520
										endif
								else
										if is_qmark($n) == wererat_plane 100014
												callout 2 transfer $n 519
										else
												callout 2 transfer $n 518
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100010
										if is_qmark($n) == wererat_plane 100012
												callout 2 transfer $n 517
										else
												callout 2 transfer $n 516
										endif
								else
										if is_qmark($n) == wererat_plane 100010
												callout 2 transfer $n 515
										else
												callout 2 transfer $n 514
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100004
								if is_qmark($n) > wererat_plane 100006
										if is_qmark($n) == wererat_plane 100008
												callout 2 transfer $n 513
										else
												callout 2 transfer $n 512
										endif
								else
										if is_qmark($n) == wererat_plane 100006
												callout 2 transfer $n 511
										else
												callout 2 transfer $n 510
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100002
										if is_qmark($n) == wererat_plane 100004
												callout 2 transfer $n 509
										else
												callout 2 transfer $n 508
										endif
								else
										if is_qmark($n) > wererat_plane 100001
												callout 2 transfer $n 507
										else
												if is_qmark($n) == wererat_plane 100001
														callout 2 transfer $n 506
												else
														callout 2 transfer $n 527
												endif
										endif
								endif
						endif
				endif
		endif
		endif
~
>room_defun movesilver~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 13 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你滑動著雙手，往銀輝色的光河游去...
		echoaround $n $N滑動著雙手，往銀輝色的光河游去...
		if is_qmark($n) > wererat_plane 100016
				if is_qmark($n) > wererat_plane 100024
						if is_qmark($n) > wererat_plane 100028
								if is_qmark($n) == wererat_plane 100029
										callout 2 transfer $n 500
								else
										callout 2 transfer $n 529
								endif
						else
								if is_qmark($n) > wererat_plane 100026
										if is_qmark($n) == wererat_plane 100028
												callout 2 transfer $n 526
										else
												callout 2 transfer $n 526
										endif
								else
										if is_qmark($n) == wererat_plane 100026
												callout 2 transfer $n 515
										else
												callout 2 transfer $n 524
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100020
								if is_qmark($n) > wererat_plane 100022
										if is_qmark($n) == wererat_plane 100024
												callout 2 transfer $n 523
										else
												callout 2 transfer $n 522
										endif
								else
										if is_qmark($n) == wererat_plane 100022
												callout 2 transfer $n 521
										else
												callout 2 transfer $n 525
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100018
										if is_qmark($n) == wererat_plane 100020
												callout 2 transfer $n 519
										else
												callout 2 transfer $n 518
										endif
								else
										if is_qmark($n) == wererat_plane 100018
												callout 2 transfer $n 517
										else
												callout 2 transfer $n 516
										endif
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100008
						if is_qmark($n) > wererat_plane 100012
								if is_qmark($n) > wererat_plane 100014
										if is_qmark($n) == wererat_plane 100016
												callout 2 transfer $n 520
										else
												callout 2 transfer $n 514
										endif
								else
										if is_qmark($n) == wererat_plane 100014
												callout 2 transfer $n 513
										else
												callout 2 transfer $n 512
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100010
										if is_qmark($n) == wererat_plane 100012
												callout 2 transfer $n 511
										else
												callout 2 transfer $n 529
										endif
								else
										if is_qmark($n) == wererat_plane 100010
												callout 2 transfer $n 509
										else
												callout 2 transfer $n 508
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100004
								if is_qmark($n) > wererat_plane 100006
										if is_qmark($n) == wererat_plane 100008
												callout 2 transfer $n 507
										else
												callout 2 transfer $n 506
										endif
								else
										if is_qmark($n) == wererat_plane 100006
												callout 2 transfer $n 510
										else
												callout 2 transfer $n 504
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100002
										if is_qmark($n) == wererat_plane 100004
												callout 2 transfer $n 503
										else
												callout 2 transfer $n 502
										endif
								else
										if is_qmark($n) > wererat_plane 100001
												callout 2 transfer $n 501
										else
												if is_qmark($n) == wererat_plane 100001
														callout 2 transfer $n 505
												else
														callout 2 transfer $n 526
												endif
										endif
								endif
						endif
				endif
		endif
		endif
~
>room_defun movegreen~
		if is_qmark($n) != wererat_plane
				add_qmark $n wererat_plane -1 13 0 0 1 0
		endif
		if is_qmark($n) != ismoving
		add_qmark $n ismoving 2 0 0 0 1 0
		echoat $n 你滑動著雙手，往青與紫的電光游去...
		echoaround $n $N滑動著雙手，往青與紫的電光游去...
		if is_qmark($n) > wererat_plane 100016
				if is_qmark($n) > wererat_plane 100024
						if is_qmark($n) > wererat_plane 100028
								if is_qmark($n) == wererat_plane 100029
										callout 2 transfer $n 528
								else
										callout 2 transfer $n 527
								endif
						else
								if is_qmark($n) > wererat_plane 100026
										if is_qmark($n) == wererat_plane 100028
												callout 2 transfer $n 523
										else
												callout 2 transfer $n 500
										endif
								else
										if is_qmark($n) == wererat_plane 100026
												callout 2 transfer $n 528
										else
												callout 2 transfer $n 520
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100020
								if is_qmark($n) > wererat_plane 100022
										if is_qmark($n) == wererat_plane 100024
												callout 2 transfer $n 519
										else
												callout 2 transfer $n 518
										endif
								else
										if is_qmark($n) == wererat_plane 100022
												callout 2 transfer $n 517
										else
												callout 2 transfer $n 516
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100018
										if is_qmark($n) == wererat_plane 100020
												callout 2 transfer $n 515
										else
												callout 2 transfer $n 514
										endif
								else
										if is_qmark($n) == wererat_plane 100018
												callout 2 transfer $n 513
										else
												callout 2 transfer $n 512
										endif
								endif
						endif
				endif
		else
				if is_qmark($n) > wererat_plane 100008
						if is_qmark($n) > wererat_plane 100012
								if is_qmark($n) > wererat_plane 100014
										if is_qmark($n) == wererat_plane 100016
												callout 2 transfer $n 511
										else
												callout 2 transfer $n 510
										endif
								else
										if is_qmark($n) == wererat_plane 100014
												callout 2 transfer $n 509
										else
												callout 2 transfer $n 508
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100010
										if is_qmark($n) == wererat_plane 100012
												callout 2 transfer $n 507
										else
												callout 2 transfer $n 506
										endif
								else
										if is_qmark($n) == wererat_plane 100010
												callout 2 transfer $n 505
										else
												callout 2 transfer $n 504
										endif
								endif
						endif
				else
						if is_qmark($n) > wererat_plane 100004
								if is_qmark($n) > wererat_plane 100006
										if is_qmark($n) == wererat_plane 100008
												callout 2 transfer $n 503
										else
												callout 2 transfer $n 502
										endif
								else
										if is_qmark($n) == wererat_plane 100006
												callout 2 transfer $n 501
										else
												callout 2 transfer $n 525
										endif
								endif
						else
								if is_qmark($n) > wererat_plane 100002
										if is_qmark($n) == wererat_plane 100004
												callout 2 transfer $n 524
										else
												callout 2 transfer $n 527
										endif
								else
										if is_qmark($n) > wererat_plane 100001
												callout 2 transfer $n 522
										else
												if is_qmark($n) == wererat_plane 100001
														callout 2 transfer $n 521
												else
														callout 2 transfer $n 528
												endif
										endif
								endif
						endif
				endif
		endif
		endif
~
>room_command move left~
		if roomvalue($0) < 6
				rcall movedark $n
				break
		endif
		if roomvalue($1) & 1
				if roomvalue($1) &2
						rcall movered $n
				else
						rcall moveblue $n
				endif
		else
				if roomvalue($1) &2
						rcall movesilver $n
				else
						rcall movegreen $n
				endif
		endif
~
>room_command move right~
		if roomvalue($0) == 1
		or roomvalue($0) == 6
		or roomvalue($0) == 7
		or roomvalue($0) == 8
		or roomvalue($0) == 9
				rcall movedark $n
				break
		endif
		if roomvalue($0) < 6
				if roomvalue($1) & 1
						if roomvalue($1) &2
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &2
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
		else
				if roomvalue($1) & 4
						if roomvalue($1) &8
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &8
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
		endif
~
>room_command move front~
		if roomvalue($0) == 2
		or roomvalue($0) == 6
		or roomvalue($0) == 10
		or roomvalue($0) == 11
		or roomvalue($0) == 12
				rcall movedark $n
				break
		endif
		if roomvalue($0) == 1
				if roomvalue($1) & 1
						if roomvalue($1) &2
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &2
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
				break
		endif
		if roomvalue($0) < 10
				if roomvalue($1) & 4
						if roomvalue($1) &8
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &8
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
				break
		endif
		if roomvalue($0) > 12
				if roomvalue($1) & 16
						if roomvalue($1) &32
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &32
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
		endif
~
>room_command move back~
		if roomvalue($0) == 3
		or roomvalue($0) == 7
		or roomvalue($0) == 10
		or roomvalue($0) == 13
		or roomvalue($0) == 14
				rcall movedark $n
				break
		endif
		if roomvalue($0) == 1
		or roomvalue($0) == 2
		or roomvalue($0) == 6
				if roomvalue($1) & 4
						if roomvalue($1) &8
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &8
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
				break
		endif
		if roomvalue($0) == 4
		or roomvalue($0) == 5
		or roomvalue($0) == 8
		or roomvalue($0) == 9
		or roomvalue($0) == 11
		or roomvalue($0) == 12
				if roomvalue($1) & 16
						if roomvalue($1) &32
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &32
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
				break
		endif
		if roomvalue($0) == 15
				if roomvalue($1) & 64
						if roomvalue($1) &128
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) &128
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
		endif
~
>room_command move up~
		if roomvalue($0) == 4
		or roomvalue($0) == 8
		or roomvalue($0) == 11
		or roomvalue($0) == 13
		or roomvalue($0) == 15
				rcall movedark $n
				break
		endif
		if roomvalue($0) == 1
		or roomvalue($0) == 2
		or roomvalue($0) == 3
		or roomvalue($0) == 6
		or roomvalue($0) == 7
		or roomvalue($0) == 10
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) & 32
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
		else
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								rcall movered $n
						else
								rcall moveblue $n
						endif
				else
						if roomvalue($1) & 128
								rcall movesilver $n
						else
								rcall movegreen $n
						endif
				endif
		endif
~
>room_command move down~
		if roomvalue($0) == 5
		or roomvalue($0) == 9
		or roomvalue($0) == 12
		or roomvalue($0) == 14
		or roomvalue($0) == 15
				rcall movedark $n
				break
		endif
		if roomvalue($1) & 64
				if roomvalue($1) & 128
						rcall movered $n
				else
						rcall moveblue $n
				endif
		else
				if roomvalue($1) & 128
						rcall movesilver $n
				else
						rcall movegreen $n
				endif
		endif
~
>room_command move default~
echoat $n 你要移動到哪裡(exits)去？
~
>room_mask east default~
echoat $n 你判斷不出哪邊是東方！
~
>room_mask west default~
echoat $n 你判斷不出哪邊是西方！
~
>room_mask north default~
echoat $n 你判斷不出哪邊是北方！
~
>room_mask south default~
echoat $n 你判斷不出哪邊是南方！
~
>room_mask up default~
echoat $n 在這個沒有重力的地方，你判斷不出哪邊是上方！
~
>room_mask down default~
echoat $n 在這個沒有重力的地方，你判斷不出哪邊是下方！
~
>room_defun exita~
				echoat $n 你的左手邊(left)是深邃無邊的黑暗
				echoat $n 你的右手邊(right)是深邃無邊的黑暗
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitb~
				echoat $n 你的左手邊(left)是深邃無邊的黑暗
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				echoat $n 你的前方(front)是深邃無邊的黑暗
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitc~
				echoat $n 你的左手邊(left)是深邃無邊的黑暗
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				echoat $n 你的身後(back)是深邃無邊的黑暗
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitd~
				echoat $n 你的左手邊(left)是深邃無邊的黑暗
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				echoat $n 你的頭頂(up)是深邃無邊的黑暗
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exite~
				echoat $n 你的左手邊(left)是深邃無邊的黑暗
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				echoat $n 你的腳底(down)是深邃無邊的黑暗
~
>room_defun exitf~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				echoat $n 你的右手邊(right)是深邃無邊的黑暗
				echoat $n 你的前方(front)是深邃無邊的黑暗
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitg~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				echoat $n 你的右手邊(right)是深邃無邊的黑暗
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				echoat $n 你的身後(back)是深邃無邊的黑暗
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exith~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				echoat $n 你的右手邊(right)是深邃無邊的黑暗
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				echoat $n 你的頭頂(up)是深邃無邊的黑暗
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exiti~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				echoat $n 你的右手邊(right)是深邃無邊的黑暗
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				echoat $n 你的腳底(down)是深邃無邊的黑暗
~
>room_defun exitj~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				echoat $n 你的前方(front)是深邃無邊的黑暗
				echoat $n 你的身後(back)是深邃無邊的黑暗
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitk~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				echoat $n 你的前方(front)是深邃無邊的黑暗
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				echoat $n 你的頭頂(up)是深邃無邊的黑暗
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitl~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				echoat $n 你的前方(front)是深邃無邊的黑暗
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				echoat $n 你的腳底(down)是深邃無邊的黑暗
~
>room_defun exitm~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				echoat $n 你的身後(back)是深邃無邊的黑暗
				echoat $n 你的頭頂(up)是深邃無邊的黑暗
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的腳底(down)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的腳底(down)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的腳底(down)
						else
								echoat $n 青與紫的電光在你的腳底(down)交互纏繞
						endif
				endif
~
>room_defun exitn~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				echoat $n 你的身後(back)是深邃無邊的黑暗
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的頭頂(up)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的頭頂(up)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的頭頂(up)
						else
								echoat $n 青與紫的電光在你的頭頂(up)交互纏繞
						endif
				endif
				echoat $n 你的腳底(down)是深邃無邊的黑暗
~
>room_defun exito~
				if roomvalue($1) & 1
						if roomvalue($1) & 2
								echoat $n 你的左手邊(left)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的左手邊(left)延伸而去
						endif
				else
						if roomvalue($1) & 2
								echoat $n 銀輝色的光河橫亙在你的左手邊(left)
						else
								echoat $n 青與紫的電光在你的左手邊(left)交互纏繞
						endif
				endif
				if roomvalue($1) & 4
						if roomvalue($1) & 8
								echoat $n 你的右手邊(right)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的右手邊(right)延伸而去
						endif
				else
						if roomvalue($1) & 8
								echoat $n 銀輝色的光河橫亙在你的右手邊(right)
						else
								echoat $n 青與紫的電光在你的右手邊(right)交互纏繞
						endif
				endif
				if roomvalue($1) & 16
						if roomvalue($1) & 32
								echoat $n 你的前方(front)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的前方(front)延伸而去
						endif
				else
						if roomvalue($1) & 32
								echoat $n 銀輝色的光河橫亙在你的前方(front)
						else
								echoat $n 青與紫的電光在你的前方(front)交互纏繞
						endif
				endif
				if roomvalue($1) & 64
						if roomvalue($1) & 128
								echoat $n 你的身後(back)閃爍著血似腥紅的暗芒
						else
								echoat $n 一望無際的深藍往你的身後(back)延伸而去
						endif
				else
						if roomvalue($1) & 128
								echoat $n 銀輝色的光河橫亙在你的身後(back)
						else
								echoat $n 青與紫的電光在你的身後(back)交互纏繞
						endif
				endif
				echoat $n 你的頭頂(up)是深邃無邊的黑暗
				echoat $n 你的腳底(down)是深邃無邊的黑暗
~

>room_mask exits default~
		echoat $n 　
		if roomvalue($0) == 1
				rcall exita $n
		endif
		if roomvalue($0) == 2
				rcall exitb $n
		endif
		if roomvalue($0) == 3
				rcall exitc $n
		endif
		if roomvalue($0) == 4
				rcall exitd $n
		endif
		if roomvalue($0) == 5
				rcall exite $n
		endif
		if roomvalue($0) == 6
				rcall exitf $n
		endif
		if roomvalue($0) == 7
				rcall exitg $n
		endif
		if roomvalue($0) == 8
				rcall exith $n
		endif
		if roomvalue($0) == 9
				rcall exiti $n
		endif
		if roomvalue($0) == 10
				rcall exitj $n
		endif
		if roomvalue($0) == 11
				rcall exitk $n
		endif
		if roomvalue($0) == 12
				rcall exitl $n
		endif
		if roomvalue($0) == 13
				rcall exitm $n
		endif
		if roomvalue($0) == 14
				rcall exitn $n
		endif
		if roomvalue($0) == 15
				rcall exito $n
		endif
~
|