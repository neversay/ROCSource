>repop_prog 100~
mpadd_qmark self poker_blackjack_dealer -1 0 0 0 0 1
mpadd_qmark self poker_random_timer -1 0 0 0 0 1
mpadd_qmark self poker_21_point -1 0 0 0 1 0
mpadd_qmark self poker_21_first -1 0 0 0 1 0
mpadd_qmark self poker_21_second -1 0 0 0 1 0
mpadd_qmark self poker_21_thrid -1 0 0 0 1 0
mpadd_qmark self poker_21_fourth -1 0 0 0 1 0
mpadd_qmark self poker_21_fifth -1 0 0 0 1 0
mpforcer here def_rembch $i
~
>defun_prog poker_21_check~
if is_qmark($i) == poker_21_second 100000
	mpnonechoat $n $I¤â¤W¦³¤@±i·tµP¤Î¤@±i
else
	mpnonechoat $n $I¤â¤WªºµP¦³¡G
endif
if is_qmark($i) > poker_21_first 100000
	if is_qmark($i) == poker_21_first 200001
		mpnonechoat $n [1;31m¬õ¤ß[0m
	endif
	if is_qmark($i) == poker_21_first 200002
		mpnonechoat $n [1;31m¤è¶ô[0m
	endif
	if is_qmark($i) == poker_21_first 200003
		mpnonechoat $n [1;30m¶Â®ç[0m
	endif
	if is_qmark($i) == poker_21_first 200004
		mpnonechoat $n [1;30m±öªá[0m
	endif
	if is_qmark($i) & poker_21_first 100001
		mpnonechoat $n [1;37m¢Ï[0m
	endif
	if is_qmark($i) & poker_21_first 100002
		mpnonechoat $n [1;37m¢±[0m
	endif
	if is_qmark($i) & poker_21_first 100004
		mpnonechoat $n [1;37m¢²[0m
	endif
	if is_qmark($i) & poker_21_first 100008
		mpnonechoat $n [1;37m¢³[0m
	endif
	if is_qmark($i) & poker_21_first 100016
		mpnonechoat $n [1;37m¢´[0m
	endif
	if is_qmark($i) & poker_21_first 100032
		mpnonechoat $n [1;37m¢µ[0m
	endif
	if is_qmark($i) & poker_21_first 100064
		mpnonechoat $n [1;37m¢¶[0m
	endif
	if is_qmark($i) & poker_21_first 100128
		mpnonechoat $n [1;37m¢·[0m
	endif
	if is_qmark($i) & poker_21_first 100256
		mpnonechoat $n [1;37m¢¸[0m
	endif
	if is_qmark($i) & poker_21_first 100512
		mpnonechoat $n [1;37m¢°¢¯[0m
	endif
	if is_qmark($i) & poker_21_first 101024
		mpnonechoat $n [1;37m¢Ø[0m
	endif
	if is_qmark($i) & poker_21_first 102048
		mpnonechoat $n [1;37m¢ß[0m
	endif
	if is_qmark($i) & poker_21_first 104096
		mpnonechoat $n [1;37m¢Ù[0m
	endif
endif
if is_qmark($i) > poker_21_second 100000
	mpnonechoat $n  ¡B 
	if is_qmark($i) == poker_21_second 200001
		mpnonechoat $n [1;31m¬õ¤ß[0m
	endif
	if is_qmark($i) == poker_21_second 200002
		mpnonechoat $n [1;31m¤è¶ô[0m
	endif
	if is_qmark($i) == poker_21_second 200003
		mpnonechoat $n [1;30m¶Â®ç[0m
	endif
	if is_qmark($i) == poker_21_second 200004
		mpnonechoat $n [1;30m±öªá[0m
	endif
	if is_qmark($i) & poker_21_second 100001
		mpnonechoat $n [1;37m¢Ï[0m
	endif
	if is_qmark($i) & poker_21_second 100002
		mpnonechoat $n [1;37m¢±[0m
	endif
	if is_qmark($i) & poker_21_second 100004
		mpnonechoat $n [1;37m¢²[0m
	endif
	if is_qmark($i) & poker_21_second 100008
		mpnonechoat $n [1;37m¢³[0m
	endif
	if is_qmark($i) & poker_21_second 100016
		mpnonechoat $n [1;37m¢´[0m
	endif
	if is_qmark($i) & poker_21_second 100032
		mpnonechoat $n [1;37m¢µ[0m
	endif
	if is_qmark($i) & poker_21_second 100064
		mpnonechoat $n [1;37m¢¶[0m
	endif
	if is_qmark($i) & poker_21_second 100128
		mpnonechoat $n [1;37m¢·[0m
	endif
	if is_qmark($i) & poker_21_second 100256
		mpnonechoat $n [1;37m¢¸[0m
	endif
	if is_qmark($i) & poker_21_second 100512
		mpnonechoat $n [1;37m¢°¢¯[0m
	endif
	if is_qmark($i) & poker_21_second 101024
		mpnonechoat $n [1;37m¢Ø[0m
	endif
	if is_qmark($i) & poker_21_second 102048
		mpnonechoat $n [1;37m¢ß[0m
	endif
	if is_qmark($i) & poker_21_second 104096
		mpnonechoat $n [1;37m¢Ù[0m
	endif
endif
if is_qmark($i) > poker_21_thrid 100000
	mpnonechoat $n  ¡B 
	if is_qmark($i) == poker_21_thrid 200001
		mpnonechoat $n [1;31m¬õ¤ß[0m
	endif
	if is_qmark($i) == poker_21_thrid 200002
		mpnonechoat $n [1;31m¤è¶ô[0m
	endif
	if is_qmark($i) == poker_21_thrid 200003
		mpnonechoat $n [1;30m¶Â®ç[0m
	endif
	if is_qmark($i) == poker_21_thrid 200004
		mpnonechoat $n [1;30m±öªá[0m
	endif
	if is_qmark($i) & poker_21_thrid 100001
		mpnonechoat $n [1;37m¢Ï[0m
	endif
	if is_qmark($i) & poker_21_thrid 100002
		mpnonechoat $n [1;37m¢±[0m
	endif
	if is_qmark($i) & poker_21_thrid 100004
		mpnonechoat $n [1;37m¢²[0m
	endif
	if is_qmark($i) & poker_21_thrid 100008
		mpnonechoat $n [1;37m¢³[0m
	endif
	if is_qmark($i) & poker_21_thrid 100016
		mpnonechoat $n [1;37m¢´[0m
	endif
	if is_qmark($i) & poker_21_thrid 100032
		mpnonechoat $n [1;37m¢µ[0m
	endif
	if is_qmark($i) & poker_21_thrid 100064
		mpnonechoat $n [1;37m¢¶[0m
	endif
	if is_qmark($i) & poker_21_thrid 100128
		mpnonechoat $n [1;37m¢·[0m
	endif
	if is_qmark($i) & poker_21_thrid 100256
		mpnonechoat $n [1;37m¢¸[0m
	endif
	if is_qmark($i) & poker_21_thrid 100512
		mpnonechoat $n [1;37m¢°¢¯[0m
	endif
	if is_qmark($i) & poker_21_thrid 101024
		mpnonechoat $n [1;37m¢Ø[0m
	endif
	if is_qmark($i) & poker_21_thrid 102048
		mpnonechoat $n [1;37m¢ß[0m
	endif
	if is_qmark($i) & poker_21_thrid 104096
		mpnonechoat $n [1;37m¢Ù[0m
	endif
endif
if is_qmark($i) > poker_21_fourth 100000
	mpnonechoat $n  ¡B 
	if is_qmark($i) == poker_21_fourth 200001
		mpnonechoat $n [1;31m¬õ¤ß[0m
	endif
	if is_qmark($i) == poker_21_fourth 200002
		mpnonechoat $n [1;31m¤è¶ô[0m
	endif
	if is_qmark($i) == poker_21_fourth 200003
		mpnonechoat $n [1;30m¶Â®ç[0m
	endif
	if is_qmark($i) == poker_21_fourth 200004
		mpnonechoat $n [1;30m±öªá[0m
	endif
	if is_qmark($i) & poker_21_fourth 100001
		mpnonechoat $n [1;37m¢Ï[0m
	endif
	if is_qmark($i) & poker_21_fourth 100002
		mpnonechoat $n [1;37m¢±[0m
	endif
	if is_qmark($i) & poker_21_fourth 100004
		mpnonechoat $n [1;37m¢²[0m
	endif
	if is_qmark($i) & poker_21_fourth 100008
		mpnonechoat $n [1;37m¢³[0m
	endif
	if is_qmark($i) & poker_21_fourth 100016
		mpnonechoat $n [1;37m¢´[0m
	endif
	if is_qmark($i) & poker_21_fourth 100032
		mpnonechoat $n [1;37m¢µ[0m
	endif
	if is_qmark($i) & poker_21_fourth 100064
		mpnonechoat $n [1;37m¢¶[0m
	endif
	if is_qmark($i) & poker_21_fourth 100128
		mpnonechoat $n [1;37m¢·[0m
	endif
	if is_qmark($i) & poker_21_fourth 100256
		mpnonechoat $n [1;37m¢¸[0m
	endif
	if is_qmark($i) & poker_21_fourth 100512
		mpnonechoat $n [1;37m¢°¢¯[0m
	endif
	if is_qmark($i) & poker_21_fourth 101024
		mpnonechoat $n [1;37m¢Ø[0m
	endif
	if is_qmark($i) & poker_21_fourth 102048
		mpnonechoat $n [1;37m¢ß[0m
	endif
	if is_qmark($i) & poker_21_fourth 104096
		mpnonechoat $n [1;37m¢Ù[0m
	endif
endif
if is_qmark($i) > poker_21_fifth 100000
	mpnonechoat $n  ¡B 
	if is_qmark($i) == poker_21_fifth 200001
		mpnonechoat $n [1;31m¬õ¤ß[0m
	endif
	if is_qmark($i) == poker_21_fifth 200002
		mpnonechoat $n [1;31m¤è¶ô[0m
	endif
	if is_qmark($i) == poker_21_fifth 200003
		mpnonechoat $n [1;30m¶Â®ç[0m
	endif
	if is_qmark($i) == poker_21_fifth 200004
		mpnonechoat $n [1;30m±öªá[0m
	endif
	if is_qmark($i) & poker_21_fifth 100001
		mpnonechoat $n [1;37m¢Ï[0m
	endif
	if is_qmark($i) & poker_21_fifth 100002
		mpnonechoat $n [1;37m¢±[0m
	endif
	if is_qmark($i) & poker_21_fifth 100004
		mpnonechoat $n [1;37m¢²[0m
	endif
	if is_qmark($i) & poker_21_fifth 100008
		mpnonechoat $n [1;37m¢³[0m
	endif
	if is_qmark($i) & poker_21_fifth 100016
		mpnonechoat $n [1;37m¢´[0m
	endif
	if is_qmark($i) & poker_21_fifth 100032
		mpnonechoat $n [1;37m¢µ[0m
	endif
	if is_qmark($i) & poker_21_fifth 100064
		mpnonechoat $n [1;37m¢¶[0m
	endif
	if is_qmark($i) & poker_21_fifth 100128
		mpnonechoat $n [1;37m¢·[0m
	endif
	if is_qmark($i) & poker_21_fifth 100256
		mpnonechoat $n [1;37m¢¸[0m
	endif
	if is_qmark($i) & poker_21_fifth 100512
		mpnonechoat $n [1;37m¢°¢¯[0m
	endif
	if is_qmark($i) & poker_21_fifth 101024
		mpnonechoat $n [1;37m¢Ø[0m
	endif
	if is_qmark($i) & poker_21_fifth 102048
		mpnonechoat $n [1;37m¢ß[0m
	endif
	if is_qmark($i) & poker_21_fifth 104096
		mpnonechoat $n [1;37m¢Ù[0m
	endif
endif
mpechoat $n .
~
>rand_prog 100~
if is_qmark($i) == poker_blackjack_dealer 100001
	if is_qmark($i) == poker_random_timer 100002
		mpforcer here rcall poker_21_timeline $i
		mpmod_qmark self poker_random_timer v0 = 0
	else
		if rand(80)
			if rand(90)
				mpforcer here rcall poker_21_timeline $i
			else
				mpmod_qmark self poker_random_timer v0 + 1
			endif
		else
			if rand(50)
				mpforcer here rcall poker_21_timeline $i
			else
				mpmod_qmark self poker_random_timer v0 + 1
			endif
		endif
	endif
endif
~
>talk_prog topic~
tell $n ·Q¤F¸Ñ¥»½ä³õ¤G¤Q¤@ÂIªºª±ªk¶Ü¡H(blackjack-rule)
~
>talk_prog p blackjack-rule~
tell $n ¥»½ä³õ¤G¤Q¤@ÂIªº³W«h«ÜÂ²³æ¡A¥u­n¤âµP«ù¦³ÂI¼Æ¤£¶W¹L 21 ÂI¥B¤j©ó²ø®a´Nºâ³Ó§Q¡C
tell $n ¥u­n¦³¨¬°÷ªºÄw½X¤Uª`´N¯à¥[¤J¹CÀ¸¡A¤èªk¬O¿é¤J bet '¼Æ¦r chip'¡A¥Ø«e¼Æ¦r¬° 1 ¨ì 10¡A½Ð¤£­n¿é¤J¤¤¤å¡C
tell $n ¥[¤J¹CÀ¸«á²ø®a·|µoµ¹¶¢®a¨â±i¤½¶}ªºµP¡A¨Ã¥B±N¦³¤@¬q³Ì«á¤Uª`®É¶¡¡A®É¶¡¤º©Ò¦³¤H³£¯à¤Uª`¡C
tell $n µ²§ô³Ì«á¤Uª`«á¡A²ø®a·|µoµ¹¦Û¤v¨â±iµP¡A²Ä¤@±i¬O»\¤Wªº·tµP¥t±i«h¬O¤½¶}ªº©úµP¡AµM«á±Nµ¹¤©¶¢®a¤@¬q§à¾Ü®É¶¡¡C
tell $n ÀH®É¯à¥H check card ©Î check dealer ¨Ó¬Ý¤â¤WªºµP©Î²ü©x¤â¤WªºµP¡C
tell $n -------------------------------------------
tell $n ¦b§à¾Ü®É¶¡¤¤¡A¶¢®a¯à¿ï¾Ü¬O§_­n¥[ª`¶i¦æ¤@¦¸¤ÀµP(splitcard)©Î§ë­°(surrender)¡C
tell $n ¤ÀµP¡G¶¢®a¦bµoµP«á«ù¦³¬Û¦P¼Æ¦rªº¤âµP®É¡A¥i¥H¥Î¬Û¦Pªºª`½X±N¨â±iµP¤À¶}¤U¦¨¨âª`¡A³Ì¦h¶i¦æ¤@¦¸¡C
tell $n ¤À¥X¨ÓªºµP»P­ì¥»ªºµP·í¤U·|¦A¦U¸É¤@±iµP¡A¨Ã¥BÀH®É¯à¥H check card2 ¨Ó¬Ý¤â¤Wªº¤ÀµP¡C
tell $n §ë­°¡G¦b¤ÀµP¤Î¥[µP«e¡A¥i¥Hª½±µµ²§ô¸Ó¦¸¹CÀ¸¡A¦ý±N¥¢¥h¤@¥bªºª`½X¡C
tell $n ¦ý½Ðª`·N¡A¤ÀµP«á§ë­°µ¥¦P©ó¿éµ¹²ø®a¡A±N¥¢¥h©Ò¦³ª`½X¤£·|ÂkÁÙ¤@¥b¡C
tell $n -------------------------------------------
tell $n ¿ï¾Ü®É¶¡µ²§ô«á¡A²ø®a·|Â½¶}·tµP¡A¦¹®É·|¦³¥[µP®É¶¡¡A¶¢®a¥i¥H­n¨D¥[µP(hit)³Ì¦h¯à¸É¨ì 5 ±i¡C
tell $n ¥»½ä³õ¥[µP³W«hµL¥ý«á¶¶§Ç¡A¥B¥[µP¬Ò¬°¤½¶}ªº©úµP¡A¥u­n¦b¤@©w®É¶¡¤º³£¥iºV®à¥[µP¡C
tell $n ¦p¤£»Ý­n¥[µP¡A¥u­nµ¥¥[µP®É¶¡µ²§ô´N¦æ¡A¥»½ä³õ¤£»Ý­n¯S§O³Û°±µP¡C
tell $n -------------------------------------------
tell $n µ²§ô¥[µP®É¶¡«á¡A²ø®aªºÂI¼Æ¦pªG¤£¶W¹L 17 ÂI¡A«h¦Ü¤Ö­n¥[µP¦Ü¤j©óµ¥©ó 17 ÂI¡C
tell $n µ²§ô²ø®a¸ÉµP«á¡A¦pªG¨S¶W¹L 21 ÃzµP¡A«h¶}©l­pºâ³Ó­t(blackjack-rule2)¡C
~
>talk_prog p blackjack-rule2~
tell $n ¥»½ä³õ¤G¤Q¤@ÂIªº³Ó­t­pºâ³W«h¡G
tell $n ¶¢®aµP­±ÂI¼Æ¤ñ²ø®a¤j®É¡A«h¶¢®a¥i¥HÀò±oÃB¥~ 1 ­¿ªºª`½X¡C
tell $n ¤Ï¤§¡A²ø®a¤ñ¶¢®aµP­±¤j®É«h·|¥¢¥h©Ò¦³ª`½X¡C
tell $n ·í¶¢®a»P²ø®a¦PÂI®É¥­¤â¡A¶¢®a¥i¥H¨ú¦^ª`½X¡C
tell $n ·í²ø®a«ù¦³ÂI¼Æ¶W¹L 21 ÃzµP®É¡A©Ò¦³¶¢®a³£¥i¥HÀò±oÃB¥~ 1 ­¿ªºª`½X¡C
tell $n -------------------------------------------
tell $n ¥»½ä³õªº¯S®í³W«h¡G
tell $n ¶¢®a¦b¥[µP«eª½±µÃ¬±o 21 ÂI¡A«hª½±µÄ¹±o¸Ó³õ¹CÀ¸¨ÃÀò±o 1.5 ­¿ªºª`½X¡C
tell $n ¦ý¤ÀµPªº±¡ªp¤U¸ÉµP´ê±o 21 ÂI¤£·|ª½±µ§P©w¶¢®a³Ó§Q¡C
tell $n ¥B¸Ó¶¢®a±N­nµ¥¨ì¸Ó³õ¹CÀ¸¶}©l«á¤~¯à¶i¦æ¤U¤@³õ¹CÀ¸¡C
tell $n ²ø®a¦bÂ½¶}·tµP«áÀò±o 21 ÂI«hª½±µÄ¹±o¹CÀ¸¡A³Ñ¾lªº¶¢®a±Nª½±µ½ß±¼ª`½X¡C
tell $n -------------------------------------------
tell $n ¶¢®a¦b¥[µP«á«ù¦³ 5 ±iµP¨Ã¥B­è¦nµ¥©ó 21 ÂI¡A¥BÄ¹¹L²ø®a«h¥i¥HÀò±oÃB¥~ 2 ­¿ªºª`½X¡C
tell $n ¦ý¦pªG«ù¦³ 5 ±iµP»P²ø®a¦PÂI¡A«hºâ¥­¤â¡C
tell $n ¥t¥~½Ðª`·N¡A¤Uª`«á¦pªG¶¢®a¦]¦UºØ­ì¦]±q¦b»O®à«e®ø¥¢¡A«hµø¬°¶¢®aµL±ø¥ó¿é±¼¤Uª`Äw½X¡C
tell $n ¦pªG¹ï¤F¸Ñ¥H¤W¤º®e¡A¥i¥H¬Ý¬ÝµP­±ÂI¼Æªº­pºâ(blackjack-rule3)¤è¦¡¡C
~
>talk_prog p blackjack-rule3~
tell $n ¯ÈµPªºÂI¼Æ»P¯ÈµP¤Wªº¼Æ¦r¬Û¦P¡A¤À§O¬°¡G¢±¡B¢²¡B¢³¡B¢´¡B¢µ¡B¢¶¡B¢·¡B¢¸¡B¢°¢¯¡C
tell $n ¤å¦r¯ÈµPªº³¡¤À¢Ø¡B¢ß¡B¢Ù¥NªíµÛ 10 ÂI¡A¦Ó ¢Ï ¯à·í¦¨ 1 ÂI©Î 11 ÂI¡C
tell $n ¶¢®aµoµP®ÉÀò±o¥ô¤@ 10 ÂIµP¤Î ¢Ï µP¡A§YÀò±o¬° 21 ÂI¡C
tell $n -------------------------------------------
tell $n ¦Ó¯ÈµPªºªá¦â¦b¥»¹CÀ¸¤¤¤£¼vÅT¥ô¦óÂI¼Æ¤Î³Ó­t¡A¶È¦³½à¤ß®®¥Øªº¥\¥Î¡C
tell $n ¦¹¥~¥»½ä³õµP¦À¤¤¨C¤Ö¤@±i´N·|¸É¶i¤@±i¬Û¦PªºµP¨Ã­«·s¬~µP¡A¥i¥HµøµP¦À¦³µL­­°ÆµP¡C
tell $n ¥t¥~¥»®à»O³£§I´«¤â¤W¤j¤p¤£¦PªºÄw½X(chips)¡C
tell $n ¦pªG¹ï³W«h¨S¦³¥ô¦óºÃ°Ý¡A«h¥i¥H¤Uª`(bet)¶i¦æ¹CÀ¸¡C
~
>talk_prog chips~
tell $n Äw½XºØ¦@¦³¤­ºØ­±ÃB¤À§O¬O 100¡A50¡A25¡A10 ¤Î 5¡C
tell $n ¦pªG·Q§â¤â¤W©Ò¦³¤pÃBÄw½X´«¦¨¤jªº¡Aª½±µ¸ò§Ú»¡(change bigger)¡C
tell $n Á|¨Ò¡G¦³¥|­Ó 10 ¤Î¥|­Ó 25 ­±ÃBªº¹S½X¡A§i¶D§Ú change bigger¡C
tell $n §Ú±N´«µ¹§A¤@­Ó 100 ¡A¤@­Ó 25 ¡A¤@­Ó 10 ¤Î ¤@­Ó 5 ­±ÃBªºÄw½X¡C
tell $n ¦pªG·Q§â¯S©w­±ÃBªºÄw½X´«¦¨³Ì¤j­±ÃBªº¡A¥i¥Hª½±µ¸ò§Ú»¡(change Äw½X­±ÃB)¡C
tell $n Á|¨Ò¡G¦³¤»­Ó 25 ­±ÃBªº¹S½X¡A§i¶D§Ú change 25¡C
tell $n §Ú±N´«µ¹§A¤@­Ó 100 ¤Î¤@­Ó 50 ªºÄw½X¡C
tell $n ¦pªG·Q§â¯S©w­±ÃBªºÄw½X´«¦¨¯S©w­±ÃBªº¡A¥i¥Hª½±µ¸ò§Ú»¡(change ±ý´«Äw½X­±ÃBto¯S©wÄw½X­±ÃB)¡C
tell $n Á|¨Ò¡G¦³¤­­Ó 25 ­±ÃBªº¹S½X¡A§i¶D§Ú change 25to100¡C
tell $n §Ú±N´«µ¹§A¤@­Ó 100 ¤Î¤@­Ó 25 ªºÄw½X¡C
tell $n ©Î¬O¦³¤@­Ó 100 ªº¹S½X·Q´«¦¨¨â­Ó 50 ªºÄw½X¡A¥i¥H§i¶D§Ú change 100to50¡C
tell $n ´Nºâ§A¤â«ùªº¼Æ¶q¤£¨¬¡A§Ú¤]·|À°§A¬~½X¡C
tell $n ¦ý¬O¦pªG´£¥X¹³¬O 100to100¡A50to50¡Achange 100¤§ÃþªºµL²á­n¨D¡C
tell $n §Ú¬O¤£·|²z§Aªº¡C
~
>talk_prog p change bigger~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall chips_count $n
mpforcer here rcall chips_change_all $n
tell $n ©Ò¦³Äw½X§I´«§¹¦¨¡C
~
>talk_prog p change 100to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_50 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_10 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n Äw½X§I´«§¹¦¨¡C
mpdel_qmark $n casino_chips_count
~
|