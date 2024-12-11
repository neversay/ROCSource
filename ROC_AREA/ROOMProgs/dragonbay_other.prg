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
        	echo [1;37m§A·PÄ±¨ì¥|©PªºÂøÁn¦ü¥G¶V¨Ó¶V¦h¡A¤]¶V¨Ó¶V¤jÁn¡C[0m
	endif
	if roomtimer($0) == 2
        	echo [1;32m§A¬Ý¨ì¤@¤j°ïÅ]²´»jµï¥¿±q¥|­±¤K¤è»E¶°¦Ó¨Ó¡A¨Ï±o§A¤£¸T§N¦½ª½¬y¡C[0m
        	echo [1;32m¤ßÀY¤]¶}©l¥´©w¥D·N¡A´Nºâ¬O«÷¤F¦Ñ©R¤]­n»°ºòÂ÷¶}³o­Ó¦a¤è¡I[0m
	endif
	if roomtimer($0) == 1
        	echo [1;36m¦b§AÁÙ·W®£ªº·Q©Þ»L´N¶]®É¡A¤@¤j°ïÅ]²´»jµï¤w¸g´Â§Aªï­±¦Ó¨Ó¤F¡I[0m
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
                echo [1;35m§A¬Ý¨ì¤@¨ÇÅ]²´»jµï¥¿¶}©l¦b¦RµÛ»jµïµ·¸ÕµÛ­×¸É³Q¿N·´ªº»jµïºô¡C[0m
                echo [1;32m¦Ó©P¾Dªº¾ðªK¤]¦]¦¹¦Ó²£¥Í¤F¨Ç³\ªº®Ì°Ê¡C[0m
		if pcnum($) > 0
                	rcall dragonbay_mload $r
                	rcall dragonbay_mload $r
		endif
        endif
        if roomtimer(1) == 1
                echo [1;35m§A¬Ý¨ì§ó¦hªºÅ]²´»jµï¶}©l»E¶°¦b§Aªº¤WÀY¦RµÛ¥Õ¦âªº»jµïµ·¡A[0m
                echo [1;35m¤£¤@·|¨à¥\¤Ò¤w¸g¯}·lªº»jµïºô´N³Q­×¸É¦n¤F¡C[0m
                echo [1;32m©Ó¨ü¤£¤F¨º»ò¦hÅ]²´»jµïªº¾ðªK¤]¦]¦¹¦Ó²£¥Í¤F§ó¤jªº®Ì°Ê¡I[0m
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
        echo [1;32m§A¬Ý¨ì¥_Ãä­ì¥»¤w¸g³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
        modrvt v0 r 7
else
        if roomvalue($0) & 3
                echo [1;32m§A¬Ý¨ì¥_Ãä­ì¥»¤w¸g§Ö³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 3
        else
                echo [1;32m§A¬Ý¨ì¥_Ãäµy³Q¿N·´ªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 1
        endif
endif
~
>room_defun dragonbay_exit_fix_east~
if roomvalue($0) & 56
        echo [1;32m§A¬Ý¨ìªFÃä­ì¥»¤w¸g³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
        modrvt v0 r 56
else
        if roomvalue($0) & 24
                echo [1;32m§A¬Ý¨ìªFÃä­ì¥»¤w¸g§Ö³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 24
        else
                echo [1;32m§A¬Ý¨ìªFÃäµy³Q¿N·´ªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 8
        endif
endif
~
>room_defun dragonbay_exit_fix_south~
if roomvalue($0) & 448
        echo [1;32m§A¬Ý¨ì«nÃä­ì¥»¤w¸g³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
        modrvt v0 r 448
else
        if roomvalue($0) & 192
                echo [1;32m§A¬Ý¨ì«nÃä­ì¥»¤w¸g§Ö³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 192
        else
                echo [1;32m§A¬Ý¨ì«nÃäµy³Q¿N·´ªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 64
        endif
endif
~
>room_defun dragonbay_exit_fix_west~
if roomvalue($0) & 3584
        echo [1;32m§A¬Ý¨ì¦èÃä­ì¥»¤w¸g³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
        modrvt v0 r 3584
else
        if roomvalue($0) & 1536
                echo [1;32m§A¬Ý¨ì¦èÃä­ì¥»¤w¸g§Ö³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 1536
        else
                echo [1;32m§A¬Ý¨ì¦èÃäµy³Q¿N·´ªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 512
        endif
endif
~
>room_defun dragonbay_oload_skeleton~
if inroom($n) > 24211
if inroom($n) < 24269
	if rand(20)
		echoat $n ¦b§A¥¿¥´ºâ°±¤U°±¨B·²®§®É¡A¬ðµMÅ¥¨ì©N¡ãªº¤@Án¡C
		echoat $n ¦b»jµïºô³Q¿N±¼¤§«á¡A­ì¥»³Q§¨Âø¨ä¤¤ªºÀe°©¸òÂøª«ÀH§Y±¼¸¨¦b¦a¤W¡C
		echoaround $n §A¬Ý¨ì¨º­ì¥»³QªÈµ²¦b»jµïºô¤§¤¤ªºÀe°©¸òÂøª«¤@¦P±¼¸¨¦b¦a¤W¡C
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
                echoat $n [1;36m§A·P¨ì¥þ¨­³£³Â·ô¤F¡A§AÅé¤ºªºµÇ¤W¸¢¯À¦ü¥G¨S¦³µo´§¥X§@¥Î¡C[0m
                add_affect $n 'paralyze' 1130 3 39 300 268435456
        else
                echoat $n [1;36m§AÅé¤ºªºµÇ¤W¸¢¯À¦bÀþ¶¡¿Eµo¦Ó¥X¡A¨Ï§A·P¨ì¥þ¨­¬Û·í¶¶ºZ¡C[0m
                if rand(15)
                        echoat $n [1;37m¦ý¤]¦b¦¹®É¡A§A¨­¤WªºµÇ¸¢³N±±¨î³N¦ü¥G¤]®ø¥¢¤F¡C[0m
                        del_affect $n 'adrenaline control'
                endif
        endif
else
        echoat $n [1;36m¦b±µÄ²¨ì»jµïµ·«á¨S¦h¤[¡A§A«K·PÄ±¥þ¨­¦ü¥G³£³Â·ô¤F¡C[0m
        add_affect $n 'paralyze' 1130 5 39 300 268435456
endif
~
>room_defun dragonbay_para_check2~
if isaffected($n) / adrenaline control
        if rand(60)
                echoat $n [1;36m§A·P¨ì¥þ¨­³£³Â·ô¤F¡A§AÅé¤ºªºµÇ¤W¸¢¯À¦ü¥G¨S¦³µo´§¥X§@¥Î¡C[0m
                add_affect $n 'paralyze' 1130 5 39 300 268435456
        else
                echoat $n [1;36m§AÅé¤ºªºµÇ¤W¸¢¯À¦bÀþ¶¡¿Eµo¦Ó¥X¡A¨Ï§A·P¨ì¥þ¨­¬Û·í¶¶ºZ¡C[0m
                if rand(30)
                        echoat $n [1;37m¦ý¤]¦b¦¹®É¡A§A¨­¤WªºµÇ¸¢³N±±¨î³N¦ü¥G¤]®ø¥¢¤F¡C[0m
                        del_affect $n 'adrenaline control'
                endif
        endif
else
        echoat $n [1;36m¦b±µÄ²¨ì»jµïµ·«á¨S¦h¤[¡A§A«K·PÄ±¥þ¨­¦ü¥G³£³Â·ô¤F¡C[0m
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
echoat $n ¥|©P¥þ³£¬O³·¥Õªº»jµïºô¡A§A¹ê¦b¬OµLªk¤À¿ì¥X¦U¤è¦V¦³¤°»òªF¦è¡C
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
	echoat $n ©P¾Dªº¤H¤w¸g³Q»jµïÀ~±o¥¢¥h²z´¼¤F¡A¨Ï±o§AµLªkÅý¥L­ÌÂíÀR¤U¨Ó¡C
else
	force $n cast calm
endif
~
>room_defun dragonbay_rand_message~
if rand(30)
	if mobcount(24209) > 0
		echoat $r §AÅ¥¨ìªKÀY¤W¶Ç¨Ó¤@°}§s§s³å³åªºÁn­µ¡C
		break
	endif
else
	if rand(30)
		echoat $r ¤@°}´H­·±q§A¨­®Ç¨í¹L¡AÅý§A·L·L¥´¤F­Ó§NÅ¸¡C
		break
	else
		if rand(40)
			if mobcount(24209) > 0
				echoat $r ¦b«e¤W¤èªºªK¸­¤W§Ï©»°{¹L¤F¤@­Ó¼v¤l¡C
				break
			endif
		else
			if rand(70)
				echo ¬ðµM¨í°_¤F¤@°}¤j­·¡A¤WÀYªºªK¸­¤]¦]¦¹²£¥Í¤F¨Ç³\ªº®Ì°Ê¡C
				rcall dragonbay_mload2 $r
				rcall dragonbay_mload2 $r
			else
				echo ¬ðµM¨í°_¤F¤@°}±j­·¡A¤WÀYªºªK¸­¤]¦]¦¹²£¥Í¤F¼@¯Pªº®Ì°Ê¡C
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
                echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦Å]²´»jµï¡I[0m
                mload 24201
        else
                echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[32mÅ]²´»jµï¡I[0m
                mload 24202
        endif
else
        if rand(33)
                if rand(50)
                	echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[33mÅ]²´»jµï¡I[0m
                        mload 24203
                else
                	echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[34mÅ]²´»jµï¡I[0m
                        mload 24204
                endif
        else
                if rand(50)
                        if rand(50)
                		echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[35mÅ]²´»jµï¡I[0m
                                mload 24205
                        else
                		echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[36mÅ]²´»jµï¡I[0m
                                mload 24206
                        endif
                else
                        if rand(50)
                		echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[37mÅ]²´»jµï¡I[0m
                                mload 24207
                        else
                		echo [1;31m§A¬Ý¨ì¤Ñ¤W±¼¤U¤F¤@°¦[30mÅ]²´»jµï¡I[0m
                                mload 24208
                        endif
                endif
        endif
endif
~
|
