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
echoat $n [1;30m«n¤è³Q¤@°ï·Ð¤Hªº»jµïµ·µ¹Äñ¦í¤F¡A¨Ï±o§AµLªk¶¶§Q°õ¦æ°»¹îªº°Ê§@¡C[0m
echoaround $n [1;30m§A¬Ý¨ì $N [1;30m¦ü¥GµLµø»jµïµ·ªº¦s¦b¡A¥¿¸ÕµÛ©¹«n¤è°µ°»¹îªº°Ê§@¡C[0m
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
        echoat $n [1;37m§A¸ÕµÛ©¹«n¤è¨«¡A¦ý¬O«o³Q¨º·Ð¤Hªº»jµïµ·µ¹Äñ¦í¦Ó¤£±o°Ê¼u¡C[0m
        echoaround $n [1;37m§A¬Ý¨ì$N[1;37m¸ÕµÛ©¹«n¤è¨«¡A¦ý«o³Q»jµïµ·µ¹Äñ¦í¦Ó¤£±o°Ê¼u¡C[0m
        rcall dragonbay_para_check2 $n
        echo [1;30m¦Ó¾ã­Ó»jµïºô¤]¦]¦¹¦Ó²£¥Í¤F¨Ç³\®Ì°Ê¡I[0m
        rcall dragonbay_mload $n
else
        echoat $n §A¸ÕµÛ©¹«n¤è¨«¤F¹L¥h¡C
        echoaround $n §A¬Ý¨ì$N©¹«n¤è¨«¤F¹L¥h¡C
        if mpnum($n) < 20
                echoat $n [1;34m§A¨­¤WªºÆF¤O¤w¸g¤£³ô·l¯Ó¡AµK¥Ò¤]¦bÀþ¶¡®ø¥¢¡C[0m
                echoaround $n [1;34m§A¬Ý¨ì $N [1;34m¤~¨«­è¸ó¥X¸}¨B¡A¨­¤WªºµK¥Ò´N®ø¥¢¤F¡C[0m
                del_affect $n 'flaming armor'
                break
        endif
        if roomvalue($0) !& 64
                if rand(70)
                        echoat $n [1;32m§A¨­¤WªºµK¥Ò±N»jµïºô¿N±¼¤F¤@³¡¥÷¡C[0m
                        echoaround $n [1;32m§A¬Ý¨ì»jµïºô³Q $N [1;32mªºµK¥Ò¿N±¼¤F¤@³¡¥÷¡C[0m
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
                        echoat $n [1;33m§A¨­¤WªºµK¥Ò±N»jµïºôµ¹¿N±¼¤F¤@¤j¥b¡C[0m
                        echoaround $n [1;33m§A¬Ý¨ì»jµïºô³Q $N [1;33m¨­¤WªºµK¥Òµ¹¿N¤F¤@¤j¥b¡C[0m
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
                        echoat $n [1;36m§A¦¨¥\ªº±N«nÃäªº»jµïºôµ¹¿N­Óºë¥ú¡I«nÃä²×©ó¥i¥H³q¦æ¤F¡C[0m
                        echoaround $n [1;36m§A¬Ý¨ì $N [1;36m¦¨¥\¦a±N«n¤èªº»jµïºô²M¥ú¤F¡A«nÃä¤]¦]¦¹¥i¥H³q¦æ¤F¡C[0m
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
echo [1;36m§A¬Ý¨ì­ì¥»Äñ¦í«nÃä¥X¤fªº»jµïµ·¬ðµM¥þ³Q¿NºÉ¡A«nÃä¤]¦]¦¹¥i¥H³q¦æ¤F¡I[0m
modrvt v0 | 448
~
>room_defun dragonbay_north_linkroom_message2~
if roomvalue($0) & 448
        echo [1;36m§A¬Ý¨ì«nÃä­ì¥»¤w¸g³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
        modrvt v0 r 448
else
        if roomvalue($0) & 192
                echo [1;36m§A¬Ý¨ì«nÃä­ì¥»¤w¸g§Ö³Q¿NºÉªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 192
        else
                echo [1;36m§A¬Ý¨ì«nÃäµy³Q¿N·´ªº»jµïµ·¤S³Q¤@¸sÅ]²´»jµïµ¹­×¸É¦n¤F¡C[0m
                modrvt v0 r 64
        endif
endif
~
>room_defun dragonbay_south_flaming_end~
echoat $n [1;35m§A·P¨ì¦Û¤vªºÆF¤O¦bÀþ¶¡®ø¯Ó¤F¤£¤Ö¡C[0m
modhmv $n 11.20 20 0
echo [1;30m©P¾Dªº»jµïµ·¤]¦]¦¹¦Ó²£¥Í¤F®Ì°Ê¡C[0m
rcall dragonbay_mload $n
if rand(15)
        echoat $n [1;31m§A¨­¤WªºµK¥Ò«o¤]¦b¦¹®É®ø¥¢¤F¡C[0m
        echoaround $n [1;31m§A¬Ý¨ì $N [1;31mªºµK¥Ò¤]¦b¦P®É¶¡®ø¥¢¤F¡C[0m
        del_affect $n 'flaming armor'
endif
~
>room_defun dragonbay_south_miss_message~
echoat $n [1;34m§A¸ÕµÛ¬ï¶V¨º¥O¤H¹½·Ðªº»jµïµ·¡A¥i±¤«o³Q»jµïºôÄñ¦í¦Ó¥¢±Ñ¤F¡C[0m
echoaround $n [1;34m§A¬Ý¨ì $N [1;34m¦b¸ó¥X¸}¨B¨S¦h¤[¤§«á«K³Q»jµïºôÄñ¦í¦Ó¤î¨B¤£«e¡C[0m
rcall dragonbay_para_check $n
rcall dragonbay_south_flaming_end $n
~
|
